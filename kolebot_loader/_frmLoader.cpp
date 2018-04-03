//---------------------------------------------------------------------------

#include <vcl.h>
#include <string>
#pragma hdrstop

#include "_frmLoader.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmLoader *frmLoader;

char* DLLNAME 		=	"melih.dll";
char* strWebAddress	=	"http://kojdbot.com/loader_index.htm";

char* strKODIR = NULL;
char strKOWND[25];
long lLastTick = 0;

DWORD dwDebugAPI = 0;

bool bCheckDebugEntry = true;

//---------------------------------------------------------------------------
__fastcall TfrmLoader::TfrmLoader(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void HandleProcess(){
	BYTE pGetFileName[] = {0x60,0x68,0xFF,0x00,0x00,0x00,0x68,0,0,0,0,0x6A,0x00,0xBF,0,0,0,0,0xFF,0xD7,0x61,0xC3};

	if(frmLoader->txtWindow->Text.IsEmpty() == true)
		return;

	CheckDLL();

	HWND hWindow = FindWindowA(NULL,frmLoader->txtWindow->Text.c_str());
	if(!hWindow){
		MessageBox(frmLoader->Handle,"Pencere bulunamadi!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	DWORD dwPID = 0;
	GetWindowThreadProcessId(hWindow,&dwPID);
	if(!dwPID){
		MessageBox(frmLoader->Handle,"PID alinamadi!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, 0, &tkp, sizeof(tkp), NULL, NULL);
	}

	HANDLE hProcess = OpenProcess(0x1F0FFF,NULL,dwPID);
	if(!hProcess){
		long lErrorCode = GetLastError();
		char tmp[8]; memset(tmp,0,8);	ltoa(lErrorCode,tmp,10);
		char *pmsg = "OpenProcess handle error (";
		char msg[64]; memset(msg,0,64); memcpy(msg,pmsg,strlen(pmsg));
		strcat(msg,tmp);
		strcat(msg,")");
		MessageBox(frmLoader->Handle,msg,"Hata",MB_ICONEXCLAMATION);
		return;
	}

	LPVOID lpVirtualMem1 = VirtualAllocEx(hProcess, NULL, 512, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if(!lpVirtualMem1){
		MessageBox(frmLoader->Handle,"Hafiza acilamadi! (1)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	HMODULE hKernel32 = GetModuleHandle("kernel32.dll");
	FARPROC hGetModuleFileNameA = GetProcAddress(hKernel32, "GetModuleFileNameA");
	if(!hGetModuleFileNameA){
		MessageBox(frmLoader->Handle,"Fonksiyon bulunamadi! (GetModuleFileNameA)","Hata",MB_ICONEXCLAMATION);
		VirtualFreeEx(hProcess, lpVirtualMem1, 512, MEM_RELEASE);
		return;
	}

	*(DWORD*)(pGetFileName + 7) = (DWORD)lpVirtualMem1 + 128;
	*(DWORD*)(pGetFileName + 14) = (DWORD)hGetModuleFileNameA;

	WriteProcessMemory(hProcess,lpVirtualMem1,pGetFileName,sizeof(pGetFileName),NULL);
	HANDLE hFileNameThread = CreateRemoteThread(hProcess,NULL,NULL,(LPTHREAD_START_ROUTINE)lpVirtualMem1,NULL,NULL,NULL);

	if(!hFileNameThread){
		MessageBox(frmLoader->Handle,"Thread olusturulamadi! (1)","Hata",MB_ICONEXCLAMATION);
		VirtualFreeEx(hProcess, lpVirtualMem1, 512, MEM_RELEASE);
		return;
	}
	else{
		if(WaitForSingleObject(hFileNameThread,5000) == WAIT_TIMEOUT){
			MessageBox(frmLoader->Handle,"Thread'den cevap yok! (1)","Hata",MB_ICONEXCLAMATION);
			VirtualFreeEx(hProcess, lpVirtualMem1, 512, MEM_RELEASE);
			return;
		}
	}

	DWORD dwTmpAddr = (DWORD)lpVirtualMem1 + 128;

	char strPath[256]; memset(strPath,0,256);

	ReadProcessMemory(hProcess,(LPVOID)dwTmpAddr,strPath,255,NULL);
	if(strlen(strPath) == 0){
		MessageBox(frmLoader->Handle,"KO PATH alinamadi! (NULL)","Hata",MB_ICONEXCLAMATION);
		VirtualFreeEx(hProcess, lpVirtualMem1, 512, MEM_RELEASE);
		return;
	}

	char* pFileName = strrchr(strPath,'\\');

	int iFileName = strlen(pFileName + 1);
	char *strFileName = new char[iFileName + 1];
	memset(strFileName,0,iFileName + 1);
	strcpy(strFileName,pFileName + 1);

	int iPathDir = strlen(strPath) - (iFileName + 1);
	char *strPathDir = new char[iPathDir + 1];
	memset(strPathDir,0,iPathDir + 1);
	memcpy(strPathDir,strPath,iPathDir);

	*(pFileName + 1) = '\0';
	strcat(strPath,"loginstatus.kojd");
	DeleteFileA(strPath);

	*(pFileName + 1) = '\0';
	strcat(strPath,"autologin.kojd");
	DeleteFileA(strPath);

	if(frmLoader->chotologin->Checked)
	{
		ofstream fSaveLogin;
		fSaveLogin.open(strPath,std::ios_base::out | std::ios_base::app);
		if(fSaveLogin.is_open() == true){
			fSaveLogin << "1\n";

			bool bTrial = false;
			if(frmLoader->chtrial->Checked)
			{
				fSaveLogin << "1\n";
				bTrial = true;
			}
			else
				fSaveLogin << "0\n";

			char pStr[32];

			memset(pStr,0,32);
			strcpy(pStr,frmLoader->txtgameid->Text.c_str());
			StrEncrypt(pStr,0);
			fSaveLogin << pStr << "\n";

			memset(pStr,0,32);
			strcpy(pStr,frmLoader->txtgamepw->Text.c_str());
			StrEncrypt(pStr,0);
			fSaveLogin << pStr << "\n";

			if(bTrial)
			{
				fSaveLogin << "-\n";
				fSaveLogin << "-\n";
			}
			else
			{
				memset(pStr,0,32);
				strcpy(pStr,frmLoader->txtbotid->Text.c_str());
				StrEncrypt(pStr,0);
				fSaveLogin << pStr << "\n";

				memset(pStr,0,32);
				strcpy(pStr,frmLoader->txtbotpw->Text.c_str());
				StrEncrypt(pStr,0);
				fSaveLogin << pStr << "\n";
			}

			char tmp[4]; *(DWORD*)tmp = 0;
			itoa(frmLoader->comserverlist->ItemIndex,tmp,10);
			fSaveLogin << tmp << "\n";

			*(DWORD*)tmp = 0;
			itoa(frmLoader->comserverno->ItemIndex,tmp,10);
			fSaveLogin << tmp << "\n";

			if(frmLoader->opleft->Checked)
				fSaveLogin << "1\n";
			else if(frmLoader->opcenter->Checked)
				fSaveLogin << "0\n";
			else
				fSaveLogin << "2\n";

			fSaveLogin.close();
		}
	}

	*(pFileName + 1) = '\0';
	strcat(strPath,"kojdbot.dll");
	CopyFile("kmodule.dat",strPath,false);

	*(pFileName + 1) = '\0';
	strcat(strPath,"alarm.mp3");
	CopyFile("alarm.mp3",strPath,false);

	CloseHandle(hFileNameThread);
	VirtualFreeEx(hProcess, lpVirtualMem1, 512, MEM_RELEASE);

	int iMemSize = strlen("kojdbot.dll") + 1;

	LPVOID lpVirtualMem2 = VirtualAllocEx(hProcess, NULL, iMemSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if(!lpVirtualMem2){
		MessageBox(frmLoader->Handle,"Hafiza acilamadi! (2)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	FARPROC hLoadLibrary = GetProcAddress(hKernel32, "LoadLibraryA");
	if(!hLoadLibrary){
		MessageBox(frmLoader->Handle,"Fonksiyon bulunamadi! (LoadLibraryA)","Hata",MB_ICONEXCLAMATION);
		VirtualFreeEx(hProcess, lpVirtualMem2, iMemSize, MEM_RELEASE);
		return;
	}

	*(pFileName + 1) = '\0';
	strcat(strPath,"initloader.kojd");
	CreateFileA(strPath,GENERIC_READ,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	WriteProcessMemory(hProcess,lpVirtualMem2,"kojdbot.dll",iMemSize - 1,NULL);
	HANDLE hLoadThread = CreateRemoteThread(hProcess,NULL,NULL,(LPTHREAD_START_ROUTINE)hLoadLibrary,lpVirtualMem2,NULL,NULL);

	Sleep(1000);

	if(!hLoadThread){
		MessageBox(frmLoader->Handle,"Thread olusturulamadi! (2)","Hata",MB_ICONEXCLAMATION);
		VirtualFreeEx(hProcess, lpVirtualMem2, iMemSize, MEM_RELEASE);
		return;
	}
	else{
		if(WaitForSingleObject(hLoadThread,5000) == WAIT_TIMEOUT){
			MessageBox(frmLoader->Handle,"Thread'den cevap yok! (2)","Hata",MB_ICONEXCLAMATION);
			VirtualFreeEx(hProcess, lpVirtualMem2, iMemSize, MEM_RELEASE);
			return;
		}
	}

	GamePatch(hProcess);

	memset(strKOWND,0,25);
	ReadProcessMemory(hProcess,(LPVOID)KO_WNDA,strKOWND,20,0);

	CloseHandle(hLoadThread);
	VirtualFreeEx(hProcess, lpVirtualMem2, iMemSize, MEM_RELEASE);
	CloseHandle(hProcess);

	lLastTick = GetTickCount();

	if(frmLoader->chotologin->Checked == false)
		Application->Terminate();
	else{
		strKODIR = new char[iPathDir + 1];
		memset(strKODIR,0,iPathDir + 1);
		strcpy(strKODIR,strPathDir);
		delete [] strFileName;
		delete [] strPathDir;
		frmLoader->tmautologin->Enabled = true;
		//frmLoader->Caption = "K A P A T M A Y I N!";
		MakeRandomCaption();
		frmLoader->WindowState = wsMinimized;
	}
}

void LoadProcess(){

	int iPathLen = frmLoader->txtPath->Text.Length();

	if(!iPathLen){
		frmLoader->dlgPath->FileName = "KnightOnline";
		int	iTmpLen = frmLoader->dlgPath->FileName.Length();
		frmLoader->dlgPath->Execute(frmLoader->Handle);
		frmLoader->txtPath->Text = frmLoader->dlgPath->FileName;
		if(frmLoader->txtPath->Text.Length() == iTmpLen)
			frmLoader->txtPath->Text = "";
	}

	iPathLen = frmLoader->txtPath->Text.Length();
	if(!iPathLen){
		MessageBox(frmLoader->Handle,"KnightOnline bulunamadi!","Hata",MB_ICONSTOP);
		return;
	}

	CheckDLL();

	DeleteFileA("..//kopath.ini");

	ofstream fSave;
	fSave.open("..//kopath.ini",std::ios_base::out | std::ios_base::app);
	if(fSave.is_open() == true){
		fSave << frmLoader->txtPath->Text.c_str();
		fSave.close();
	}

	char *strPath = new char[iPathLen + 1];
	memset(strPath,0,iPathLen + 1);
	strcpy(strPath,frmLoader->txtPath->Text.c_str());

	char* pFileName = strrchr(strPath,'\\');
	if(!pFileName){
		MessageBox(frmLoader->Handle,"Hatali Path!","Hata",MB_ICONEXCLAMATION);
		delete []strPath;
		return;
	}

	ClearLogFiles();

	int iFileName = strlen(pFileName + 1);
	char *strFileName = new char[iFileName + 1];
	memset(strFileName,0,iFileName + 1);
	strcpy(strFileName,pFileName + 1);

	int iPathDir = strlen(strPath) - (iFileName + 1);
	char *strPathDir = new char[iPathDir + 1];
	memset(strPathDir,0,iPathDir + 1);
	memcpy(strPathDir,strPath,iPathDir);

	*(pFileName + 1) = '\0';
	strcat(strPath,"loginstatus.kojd");
	DeleteFileA(strPath);

	*(pFileName + 1) = '\0';
	strcat(strPath,"autologin.kojd");
	DeleteFileA(strPath);

	if(frmLoader->chotologin->Checked)
	{
		ofstream fSaveLogin;
		fSaveLogin.open(strPath,std::ios_base::out | std::ios_base::app);
		if(fSaveLogin.is_open() == true){

			fSaveLogin << "1\n";

			bool bTrial = false;
			if(frmLoader->chtrial->Checked)
			{
				fSaveLogin << "1\n";
				bTrial = true;
			}
			else
				fSaveLogin << "0\n";

			char pStr[32];

			memset(pStr,0,32);
			strcpy(pStr,frmLoader->txtgameid->Text.c_str());
			StrEncrypt(pStr,0);
			fSaveLogin << pStr << "\n";

			memset(pStr,0,32);
			strcpy(pStr,frmLoader->txtgamepw->Text.c_str());
			StrEncrypt(pStr,0);
			fSaveLogin << pStr << "\n";

			if(bTrial)
			{
				fSaveLogin << "-\n";
				fSaveLogin << "-\n";
			}
			else
			{
				memset(pStr,0,32);
				strcpy(pStr,frmLoader->txtbotid->Text.c_str());
				StrEncrypt(pStr,0);
				fSaveLogin << pStr << "\n";

				memset(pStr,0,32);
				strcpy(pStr,frmLoader->txtbotpw->Text.c_str());
				StrEncrypt(pStr,0);
				fSaveLogin << pStr << "\n";
			}

			char tmp[4]; *(DWORD*)tmp = 0;
			itoa(frmLoader->comserverlist->ItemIndex,tmp,10);
			fSaveLogin << tmp << "\n";

			*(DWORD*)tmp = 0;
			itoa(frmLoader->comserverno->ItemIndex,tmp,10);
			fSaveLogin << tmp << "\n";

			if(frmLoader->opleft->Checked)
				fSaveLogin << "1\n";
			else if(frmLoader->opcenter->Checked)
				fSaveLogin << "0\n";
			else
				fSaveLogin << "2\n";

			fSaveLogin.close();
		}
    }

	*(pFileName + 1) = '\0';
	strcat(strPath,"kojdbot.dll");
	CopyFile("kmodule.dat",strPath,false);

	*(pFileName + 1) = '\0';
	strcat(strPath,"alarm.mp3");

	HANDLE hFile = CreateFileA(strPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if( hFile == INVALID_HANDLE_VALUE)
		CopyFile("alarm.mp3",strPath,false);
	else
		CloseHandle(hFile);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	bool bProcess = CreateProcess(frmLoader->txtPath->Text.c_str()," USA_KnightOnline",NULL,NULL,true,NULL,NULL,strPathDir,&si,&pi);

	if(!bProcess){
		MessageBox(frmLoader->Handle,"CreateProcess failed!","Hata",MB_ICONSTOP);
		return;
	}

	BYTE test[1] = { 0x00 };
	while ( test[0] != 0x6A){
		ReadProcessMemory(pi.hProcess,(LPVOID)0x401000,test,1,0);
	}

	SuspendThread(pi.hThread);

	Sleep(100);

	HMODULE hKernel32 = GetModuleHandle("kernel32.dll");
	FARPROC hLoadLibrary = GetProcAddress(hKernel32, "LoadLibraryA");

	int iMemSize = strlen("kojdbot.dll") + 1;

	LPVOID lpVirtualMem = VirtualAllocEx(pi.hProcess, NULL, iMemSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if(!lpVirtualMem){
		MessageBox(frmLoader->Handle,"Hafiza acilamadi!","Hata",MB_ICONEXCLAMATION);
		TerminateProcess(pi.hProcess,0);
		return;
	}

	*(pFileName + 1) = '\0';
	strcat(strPath,"initloader.kojd");
	CreateFileA(strPath,GENERIC_READ,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	WriteProcessMemory(pi.hProcess,lpVirtualMem,"kojdbot.dll",iMemSize - 1,NULL);
	HANDLE hBotThread = CreateRemoteThread(pi.hProcess,NULL,NULL,(LPTHREAD_START_ROUTINE)hLoadLibrary,lpVirtualMem,NULL,NULL);

	//Sleep(500);

	if(!hBotThread){
		MessageBox(frmLoader->Handle,"Thread olusturulamadi!","Hata",MB_ICONEXCLAMATION);
		VirtualFreeEx(pi.hProcess, lpVirtualMem, iMemSize, MEM_RELEASE);
		TerminateProcess(pi.hProcess,0);
		return;
	}
	else{
		if(WaitForSingleObject(hBotThread,20000) == WAIT_TIMEOUT){
			MessageBox(frmLoader->Handle,"Thread'den cevap yok!","Hata",MB_ICONEXCLAMATION);
			VirtualFreeEx(pi.hProcess, lpVirtualMem, iMemSize, MEM_RELEASE);
			TerminateProcess(pi.hProcess,0);
			return;
		}
	}

	GamePatch(pi.hProcess);

	CloseHandle(hBotThread);
	VirtualFreeEx(pi.hProcess, lpVirtualMem, iMemSize, MEM_RELEASE);

	ResumeThread(pi.hThread);

	while ( test[0] != 0x4B){
		ReadProcessMemory(pi.hProcess,(LPVOID)KO_WNDA,test,1,0);
	}

	char free[20]; memset(free,0,20);
	WriteProcessMemory(pi.hProcess,(LPVOID)KO_WNDA,free,20,NULL);

	char tmp[25];	memset(tmp,0,25);

	if(frmLoader->oprandom->Checked){
		srand(GetTickCount());
		short sWnd =  rand() % 0xFFFF + 1;
		itoa(sWnd,tmp,10);
		WriteProcessMemory(pi.hProcess,(LPVOID)KO_WNDA,tmp,4,NULL);
		memset(strKOWND,0,25);
		memcpy(strKOWND,tmp,4);
	}
	else{
		memcpy(tmp,frmLoader->txtmanualwnd->Text.c_str(),frmLoader->txtmanualwnd->Text.Length());
		WriteProcessMemory(pi.hProcess,(LPVOID)KO_WNDA,tmp,20,NULL);
		memset(strKOWND,0,25);
		memcpy(strKOWND,tmp,20);
	}

	WaitForInputIdle(pi.hProcess,INFINITE);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	lLastTick = GetTickCount();

	if(frmLoader->chotologin->Checked == false)
		Application->Terminate();
	else{
		strKODIR = new char[iPathDir + 1];
		memset(strKODIR,0,iPathDir + 1);
		strcpy(strKODIR,strPathDir);
		delete [] strPath;
		delete [] strFileName;
		delete [] strPathDir;
		frmLoader->tmautologin->Enabled = true;
		//frmLoader->Caption = "K A P A T M A Y I N!";
		MakeRandomCaption();
		//frmLoader->WindowState = wsMinimized;
		frmLoader->tray->BalloonHint = strKOWND;
		frmLoader->tray->Hint = strKOWND;
		ShowWindow(frmLoader->Handle,SW_HIDE);
		frmLoader->tray->Visible = true;
		frmLoader->tray->Animate = true;
		frmLoader->tray->ShowBalloonHint();
	}

}

void __fastcall TfrmLoader::btnLoadClick(TObject *Sender)
{
	HandleProcess();
//	LoadProcess();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLoader::FormShow(TObject *Sender)
{

	MakeRandomCaption();

	/*
	fstream hFound;
	hFound.open("kojdbot.dll",ios::in);
	if(!hFound.is_open()){
		MessageBox(this->Handle,"kojdbot.dll bulunamadi!","hata",MB_ICONSTOP);
		Application->Terminate();
	}
	hFound.close();
	*/

	CheckDebugger();
	CheckDLL();

	ifstream fPath;
	fPath.open("..//kopath.ini");

	if(fPath.is_open() == true){
		char Buffer[256];	memset(Buffer,0,256);
		fPath.getline(Buffer,256);

		if(strlen(Buffer) > 0)
			frmLoader->txtPath->Text = Buffer;
		else
			frmLoader->txtPath->Text = "";
		fPath.close();
	}
	else
		frmLoader->txtPath->Text = "";

	ifstream fLogin;
	fLogin.open("..//loader.cfg");

	if(fLogin.is_open() == true)
	{
		char STRBuffer[32]; memset(STRBuffer,0,32);
		char INTBuffer[4]; *(DWORD*)INTBuffer = 0;
		fLogin.getline(INTBuffer,4);

		chotologin->Checked = (bool)atoi(INTBuffer);

		fLogin.getline(STRBuffer,32);
		StrDecrypt(STRBuffer);
		txtgameid->Text = STRBuffer;

		memset(STRBuffer,0,32);

		fLogin.getline(STRBuffer,32);
		StrDecrypt(STRBuffer);
		txtgamepw->Text = STRBuffer;

		memset(STRBuffer,0,32);

		fLogin.getline(STRBuffer,32);
		StrDecrypt(STRBuffer);
		txtbotid->Text = STRBuffer;

		memset(STRBuffer,0,32);

		fLogin.getline(STRBuffer,32);
		StrDecrypt(STRBuffer);
		txtbotpw->Text = STRBuffer;

		fLogin.getline(INTBuffer,4);
		if( atoi(INTBuffer) == 0 )
			opleft->Checked = true;
		else if( atoi(INTBuffer) == 2 )
			opright->Checked = true;

		fLogin.getline(INTBuffer,4);
		comserverlist->ItemIndex = atoi(INTBuffer);

		fLogin.getline(INTBuffer,4);
		comserverno->ItemIndex = atoi(INTBuffer);

		fLogin.close();
    }

	webMain->Navigate(strWebAddress);
}
//---------------------------------------------------------------------------

void __fastcall TfrmLoader::btnLoad2Click(TObject *Sender)
{
	LoadProcess();
}
//---------------------------------------------------------------------------


void __fastcall TfrmLoader::tmautologinTimer(TObject *Sender)
{
	if(strKODIR == NULL)
		return;

	static int iInterval = 0;
	iInterval++;

	if(iInterval == 5)
	{
		MakeRandomCaption();
		iInterval = 0;
		if(IsWindowVisible(this->Handle) == true)
		{
			frmLoader->tray->BalloonHint = strKOWND;
			frmLoader->tray->Hint = strKOWND;
			ShowWindow(this->Handle,SW_HIDE);
			tray->Visible = true;
			tray->Animate = true;
			tray->ShowBalloonHint();
		}
	}

	char strCheckFile[256];	memset(strCheckFile,0,256);

	strcpy(strCheckFile,strKODIR);
	strcat(strCheckFile,"\\loginstatus.kojd");

	if( GetTickCount() - lLastTick >= 5000)
	{
		if( FindWindowA(0,strKOWND) == NULL )
		{
			DeleteFileA(strCheckFile);
			tmautologin->Enabled = false;
			LoadProcess();
			return;
		}
	}

	ifstream fCheck;
	fCheck.open(strCheckFile);

	if(fCheck.is_open() == false)
		return;

	char tmp[4];	*(DWORD*)tmp = 0;
	fCheck.getline(tmp,4);
	bool bRet = (bool)atoi(tmp);

	fCheck.close();

	if(bRet == true)
	{
		DeleteFileA(strCheckFile);
		tmautologin->Enabled = false;
		LoadProcess();
	}
}
//---------------------------------------------------------------------------

void StrEncrypt(char* pStr, BYTE bUseKey)
{
	BYTE bKey = bUseKey;

	if(bKey == 0)
	{
		bKey = (BYTE)GetTickCount();

		while(bKey < 25)
			bKey += 25;

		while(bKey > 100)
			bKey -= 25;
	}

	int iLen = strlen(pStr);

	memmove(LPVOID(pStr + 1),pStr,iLen);

	*pStr = bKey;

	for (int i = 1; i <= iLen; i++) {
		*(pStr + i) += bKey;
		bKey--;
	}
}

void __fastcall TfrmLoader::chtrialClick(TObject *Sender)
{
	if(chtrial->Checked)
	{
		txtbotid->Text = "";
		txtbotpw->Text = "";
	}
}
//---------------------------------------------------------------------------

void GamePatch(HANDLE hProcess)
{

	if( !hProcess || (int)hProcess == -1)
		return;

	LoadLibrary("psapi.dll");

	HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");

	BYTE Patch1[] = {0x8B,0xFF,0xC2,0x08,0x00};
	DWORD tmpFuncAddr = (DWORD)GetProcAddress(hKernel32,"SetThreadAffinityMask");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch1,sizeof(Patch1),0);

	BYTE Patch2[] = {0x64,0xA1,0x18,0x00,0x00,0x00,0x33,0xC0,0xC3};
	tmpFuncAddr = (DWORD)GetProcAddress(hKernel32,"IsDebuggerPresent");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch2,sizeof(Patch2),0);

	BYTE Patch3[] = {0x8B,0xFF,0x33,0xC0,0xC2,0x08,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hKernel32,"Process32First");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch3,sizeof(Patch3),0);

	BYTE Patch4[] = {0x8B,0xFF,0x33,0xC0,0xC2,0x08,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hKernel32,"Process32Next");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch4,sizeof(Patch4),0);

	BYTE Patch5[] = {0x8B,0xFF,0x33,0xC0,0xC2,0x08,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hKernel32,"CreateToolhelp32Snapshot");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch5,sizeof(Patch5),0);

	BYTE Patch6[] = {0x8B,0xFF,0x33,0xC0,0xC2,0x14,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hKernel32,"Toolhelp32ReadProcessMemory");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch6,sizeof(Patch6),0);

	BYTE Patch7[] = {0x8B,0xFF,0x33,0xC0,0xC2,0x0C,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hKernel32,"OpenProcess");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch7,sizeof(Patch7),0);

	//BYTE Patch8[] = {0x8B,0xFF,0x33,0xC0,0xC2,0x14,0x00};
	//tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("advapi32.dll"),"RegOpenKeyExA");
	//WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch8,sizeof(Patch8));

	BYTE Patch9[] = {0x8B,0xFF,0xB8,0x01,0x00,0x00,0x00,0xC2,0x08,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("user32.dll"),"EnumWindows");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch9,sizeof(Patch9),0);

	BYTE Patch10[] = {0x8B,0xFF,0x8B,0x44,0x24,0x0C,0xC7,0x00,0x00,0x00,0x00,0x00,0xB8,0x01,0x00,0x00,0x00,0xC2,0x0C,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hKernel32,"EnumProcesses");
	if(!tmpFuncAddr)
		tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("psapi.dll"),"EnumProcesses");
	if(tmpFuncAddr)
		WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch10,sizeof(Patch10),0);

	BYTE Patch11[] = {0x8B,0xFF,0xB8,0x01,0x00,0x00,0x00,0xC2,0x10,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hKernel32,"EnumProcessModules");
	if(!tmpFuncAddr)
		tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("psapi.dll"),"EnumProcessModules");
	if(tmpFuncAddr)
		WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch11,sizeof(Patch11),0);

	BYTE Patch12[] = {0x8B,0xFF,0xB8,0x01,0x00,0x00,0x00,0xC2,0x14,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hKernel32,"ReadProcessMemory");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch12,sizeof(Patch12),0);

	BYTE Patch13[] = {0x8B,0xFF,0x8B,0x44,0x24,0x0C,0xC6,0x00,0x00,0x33,0xC0,0xC2,0x10,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hKernel32,"GetModuleFileNameExA");
	if(!tmpFuncAddr)
		tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("psapi.dll"),"GetModuleFileNameExA");
	if(tmpFuncAddr)
		WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch13,sizeof(Patch13),0);

	/*
	BYTE Patch14[] = {0x8B,0xFF,0x68,0,0,0,0,0xC3,0x90};
	LPVOID lpTestMem = VirtualAllocEx(hProcess, NULL, 128, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	BYTE FuncPatch[] = {0x55,0x8B,0xEC,0x56,0xFF,0x75,0x10,0x68,0,0,0,0,0xC3};
	tmpFuncAddr = (DWORD)GetProcAddress(hKernel32,"GetModuleHandleExA");
	*(DWORD*)(FuncPatch + 8) = tmpFuncAddr + 9;
	WriteProcessMemory(hProcess,lpTestMem,FuncPatch,sizeof(FuncPatch),0);
	*(DWORD*)(Patch14 + 3) = (DWORD)lpTestMem;
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch14,sizeof(Patch14),0);
	*/

	BYTE Patch15[] = {0x6A,0x14,0x83,0xC4,0x04,0x8B,0x44,0x24,0x08,0xC6,0x00,0x00,0x33,0xC0,0xC2,0x0C,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("user32.dll"),"GetClassNameA");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch15,sizeof(Patch15),0);

	BYTE Patch16[] = {0x8B,0xFF,0x33,0xC0,0xC2,0x08,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hKernel32,"Module32First");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch16,sizeof(Patch16),0);

	BYTE Patch17[] = {0x8B,0xFF,0x33,0xC0,0xC2,0x08,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hKernel32,"Module32Next");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch17,sizeof(Patch17),0);

	BYTE Patch18[] = {0x8B,0xFF,0x33,0xC0,0xC2,0x08,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("user32.dll"),"GetWindow");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch18,sizeof(Patch18),0);

	BYTE Patch19[] = {0xB8,0x01,0x00,0x00,0x00,0xC2,0x14,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("ntdll.dll"),"ZwReadVirtualMemory");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch19,sizeof(Patch19),0);

	//BYTE Patch20[] = {0x8B,0x44,0x24,0x08,0x33,0xD2,0x42,0x89,0x10,0x8B,0xC2,0xC2,0x10,0x00};
	//tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("ntdll.dll"),"ZwQuerySystemInformation");
	//WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch20,sizeof(Patch20),0);

	BYTE Patch21[] = {0xB8,0x01,0x00,0x00,0x00,0xC2,0x18,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("ntdll.dll"),"ZwQueryVirtualMemory");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch21,sizeof(Patch21),0);

	BYTE Patch22[] = {0xB8,0x01,0x00,0x00,0x00,0xC2,0x10,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("ntdll.dll"),"ZwOpenProcess");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch22,sizeof(Patch22),0);

	//BYTE Patch23[] = {0xB8,0xFF,0xFF,0xFF,0xFF,0xC2,0x14,0x00};
	//tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("ntdll.dll"),"ZwQueryInformationProcess");
	//WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch23,sizeof(Patch23),0);

	//BYTE Patch24[] = {};
	//tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("ntdll.dll"),"RtlGetNativeSystemInformation");
	//WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch24,sizeof(Patch24),0);

	//BYTE Patch25[] = {0x8B,0xFF,0x8B,0x44,0x24,0x08,0xC7,0x00,0x00,0x00,0x00,0x00,0x8B,0x44,0x24,0x04,0xC2,0x08,0x00};
	//tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("user32.dll"),"GetWindowThreadProcessId");
	//WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch25,sizeof(Patch25),0);

	BYTE Patch26[] = {0x8B,0xFF,0x8B,0x44,0x24,0x0C,0xC6,0x00,0x00,0x33,0xC0,0xC2,0x10,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hKernel32,"GetModuleBaseNameA");
	if(!tmpFuncAddr)
		tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("psapi.dll"),"GetModuleBaseNameA");
	if(tmpFuncAddr)
		WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch26,sizeof(Patch26),0);

	BYTE Patch27[] = {0x6A,0x68,0x83,0xC4,0x04,0x33,0xC0,0xC2,0x10,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hKernel32,"GetModuleInformation");
	if(!tmpFuncAddr)
		tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("psapi.dll"),"GetModuleInformation");
	if(tmpFuncAddr)
		WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch27,sizeof(Patch27),0);

	BYTE Patch28[] = {0x6A,0x30,0x58,0xB8,0,0,0,0,0x68,0x31,0x04,0x00,0x00,0xFF,0xD0,0x33,0xC0,0xC2,0x34,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("kernel32.dll"),"SetLastError");
	*(DWORD*)(Patch28 + 4) = tmpFuncAddr;
	tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("advapi32.dll"),"CreateServiceA");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch28,sizeof(Patch28),0);

	BYTE Patch29[] = {0x6A,0x14,0x58,0xB8,0,0,0,0,0x6A,0x78,0xFF,0xD0,0x33,0xC0,0xC2,0x0C,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("kernel32.dll"),"SetLastError");
	*(DWORD*)(Patch29 + 4) = tmpFuncAddr;
	tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("advapi32.dll"),"OpenSCManagerA");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch29,sizeof(Patch29),0);

	BYTE Patch30[] = {0x8B,0xFF,0xB8,0x01,0x00,0x00,0x00,0xC2,0x0C,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("user32.dll"),"EnumChildWindows");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch30,sizeof(Patch30),0);

	BYTE Patch31[] = {0x8B,0xFF,0x33,0xC0,0xC2,0x0C,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("user32.dll"),"EnumThreadWindows");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch31,sizeof(Patch31),0);

	BYTE Patch32[] = {0x8B,0xFF,0x8B,0x44,0x24,0x08,0xC7,0x00,0x00,0x00,0x00,0x00,0xB8,0x01,0x00,0x00,0x00,0xC2,0x08,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hKernel32,"CheckRemoteDebuggerPresent");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch32,sizeof(Patch32),0);

	BYTE Patch33[] = {0x6A,0x10,0x58,0xB8,0,0,0,0,0x68,0xFB,0x04,0x00,0x00,0xFF,0xD0,0x33,0xC0,0xC2,0x0C,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("kernel32.dll"),"SetLastError");
	*(DWORD*)(Patch33 + 4) = tmpFuncAddr;
	tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("advapi32.dll"),"StartServiceA");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch33,sizeof(Patch33),0);

	//BYTE Patch34[] = {0x8B,0xFF,0x33,0xC0,0xC2,0x14,0x00};
	//tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("Wininet.dll"),"InternetOpenA");
	//WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch34,sizeof(Patch34),0);

	BYTE Patch35[] = {0x8B,0xFF,0xB8,0x01,0x00,0x00,0x00,0xC2,0x14,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("Wininet.dll"),"FtpPutFileA");
	WriteProcessMemory(hProcess,(LPVOID)tmpFuncAddr,Patch35,sizeof(Patch35),0);
}


void MakeRandomCaption()
{
	char Wnd[32]; memset(Wnd,0,32);

	srand(GetTickCount());
	BYTE bNumber;
	for (int i = 0; i < 10; i++) {
		bNumber = rand() % 0x5A + 1;
		while(bNumber < 0x41)
			bNumber += 10;

		*(Wnd + i) = bNumber;
	}

	char tmp[64]; memset(tmp,0,64);
	long lRandom = (short)GetTickCount();
	ltoa(lRandom,tmp,10);
	strcat(Wnd,tmp);
	frmLoader->Caption = Wnd;
}

void __fastcall TfrmLoader::btnsaveClick(TObject *Sender)
{
	char strGameID[32],strGamePW[32],strBotID[32],strBotPW[32];
	memset(strGameID,0,32);
	memset(strGamePW,0,32);
	memset(strBotID,0,32);
	memset(strBotPW,0,32);

	int iCharIndex = 1;
	if(opleft->Checked)
		iCharIndex = 0;
	else if(opright->Checked)
		iCharIndex = 2;

	int iServer = comserverlist->ItemIndex;
	int iServerNo = comserverno->ItemIndex;

	char tmp[4]; *(DWORD*)tmp = 0;

	strcpy(strGameID,txtgameid->Text.c_str());
	StrEncrypt(strGameID,0);

	strcpy(strGamePW,txtgamepw->Text.c_str());
	StrEncrypt(strGamePW,0);

	strcpy(strBotID,txtbotid->Text.c_str());
	StrEncrypt(strBotID,0);

	strcpy(strBotPW,txtbotpw->Text.c_str());
	StrEncrypt(strBotPW,0);

	DeleteFileA("..//loader.cfg");

	ofstream fLogin;
	fLogin.open("..//loader.cfg",std::ios_base::out | std::ios_base::app);

	if(fLogin.is_open() == true)
	{
		itoa(chotologin->Checked,tmp,10);
		fLogin << tmp << "\n";

		fLogin << strGameID << "\n";
		fLogin << strGamePW << "\n";
		fLogin << strBotID << "\n";
		fLogin << strBotPW << "\n";

		itoa(iCharIndex,tmp,10);
		fLogin << tmp << "\n";

		itoa(iServer,tmp,10);
		fLogin << tmp << "\n";

		itoa(iServerNo,tmp,10);
		fLogin << tmp << "\n";

		fLogin.close();
		MessageBox(this->Handle,"Ayarlar kaydedildi !!!","kayit",MB_ICONINFORMATION);
		return;
	}

	MessageBox(this->Handle,"Kayit basarisiz !!!","kayit",MB_ICONEXCLAMATION);
}
//---------------------------------------------------------------------------

void StrDecrypt(char* pStr)
{
	int iLen = strlen(pStr);

	BYTE bKey = *pStr;

	int i;
	for (i = 0; i < (iLen - 1); i++) {
		*(pStr + i) = *(pStr + i + 1) - bKey;
		bKey--;
	}

	*(pStr + i) = 0x00;
}

void __fastcall TfrmLoader::trayClick(TObject *Sender)
{
	tray->Visible = false;
	ShowWindow(this->Handle,SW_SHOW);
	this->WindowState = wsNormal;
}
//---------------------------------------------------------------------------

void ClearLogFiles()
{
	int iPathLen = frmLoader->txtPath->Text.Length();
	char *strPath = new char[iPathLen + 1];
	memset(strPath,0,iPathLen + 1);
	strcpy(strPath,frmLoader->txtPath->Text.c_str());

	char* pFileName = strrchr(strPath,'\\');
	*(pFileName + 1) = '\0';
	char* pBasePath = new char[256]; memset(pBasePath,0,256);
	strcpy(pBasePath,strPath);
	char* pBaseFileName = strrchr(pBasePath,'\\');
	strcat(strPath,"*.klg");

	WIN32_FIND_DATA fData;

	HANDLE hSearch = FindFirstFileA(strPath,&fData);
	if(hSearch == INVALID_HANDLE_VALUE)
	{
		delete [] strPath;
		delete [] pBasePath;
		return;
	}

	strcat(pBasePath,fData.cFileName);
	DeleteFileA(pBasePath);

	while(1)
	{
		if( !FindNextFileA(hSearch,&fData) )
			break;

		*(pBaseFileName + 1) = 0x00;
		strcat(pBasePath,fData.cFileName);
		DeleteFileA(pBasePath);
	}

	FindClose(hSearch);
	delete [] strPath;
	delete [] pBasePath;
}

void CheckDLL()
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	char pFileOut[1024]; memset(pFileOut,0,1024);
	DWORD dwBytesRead = 0;
	bool bReadFile = false;

	hFile = CreateFileA("kmodule.dat",FILE_GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		if(GetLastError() == ERROR_SHARING_VIOLATION)
			return;
		else
		{
			MessageBox(frmLoader->Handle,"kmodule.dat bulunamadi!","Hata!",MB_ICONSTOP);
			Application->Terminate();
			return;
		}
	}


	bReadFile = ReadFile(hFile,pFileOut,1024,&dwBytesRead,0);

	CloseHandle(hFile);

	if( !bReadFile || !dwBytesRead )
		return;

	if( pFileOut[0x370] != 0x54 )	// "T"
	{
		MessageBox(frmLoader->Handle,"kmodule.dat bozuk! yeniden indiriniz!","Hata!",MB_ICONSTOP);
		Application->Terminate();
	}
}
void __fastcall TfrmLoader::tmdebuggerTimer(TObject *Sender)
{
	CheckDebugger();
}
//---------------------------------------------------------------------------

void CheckDebugger()
{
	if(!bCheckDebugEntry)
		return;

	DWORD dwDebugEntry = (DWORD) GetProcAddress( GetModuleHandleA("ntdll.dll"), "DbgBreakPoint" );
	if( *(BYTE*)dwDebugEntry != 0xC3 ) 	//RETN
	{
		Application->Terminate();
		frmLoader->Close();
		delete frmLoader;
	}

	typedef NTSTATUS (__stdcall *MYPROC)(HANDLE,PROCESSINFOCLASS,PVOID,ULONG,PULONG);

	if(!dwDebugAPI)
		dwDebugAPI = (DWORD) GetProcAddress(GetModuleHandleA("ntdll.dll"),"ZwQueryInformationProcess");

	MYPROC pFunction = (MYPROC)dwDebugAPI;

	if( !(DWORD)pFunction )
	{
		return;
	}

	DWORD dwRet = 0;

	NTSTATUS ntRet = (pFunction)( (void*)-1 , 7, (PVOID)&dwRet, 4, 0);

	if(ntRet)
	{
		return;
	}

	if(dwRet)	//debugger detected
	{
		typedef NTSTATUS (__stdcall *MYPROC2)(HANDLE,NTSTATUS);
		MYPROC2 pFunction2 = (MYPROC2) GetProcAddress(GetModuleHandleA("ntdll.dll"),"ZwTerminateProcess");
		if( (DWORD)pFunction2 )
			(pFunction2)( (void*)-1, 0 );
		else
			Application->Terminate();
	}
}

bool CheckCommandLine()
{
	char *pLine = GetCommandLine();

	if( strcmp(pLine," kojd") != 0 )
	{
		MessageBox(0,"parameter invalid!","error",MB_ICONSTOP);
		Application->Terminate();
		return false;
	}

	return true;
}

void EditDbgBreakPoint()
{
	HANDLE hFile = CreateFileA("_dbg.kojd",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		bCheckDebugEntry = false;
		return;
	}

	DWORD dwEntry = (DWORD) GetProcAddress( GetModuleHandleA("ntdll.dll"), "DbgBreakPoint" );
	BYTE bEdit = 0xC3;

	WriteProcessMemory( (void*)-1, (LPVOID)dwEntry, (LPVOID)&bEdit, 1, 0 );
}

