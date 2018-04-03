//---------------------------------------------------------------------------
#pragma hdrstop
#pragma argsused
#include <windows.h>
#include "ffrmLogin.h"

long WINAPI FormThread(long lParam)
{

	Sleep(3000);

	WaitForInputIdle((void*)-1,INFINITE);

	SetUnhandledExceptionFilter( MyExceptionFilter );

	dwMyPID = GetCurrentProcessId();

	//hGameHWND = FindWindowA(NULL,(LPSTR)KO_WNDA);

	DWORD dwHWNDTick = GetTickCount();

	while( !hGameHWND )
	{
		if( GetTickCount() - dwHWNDTick > 15000 )
			break;

		hGameHWND = (HWND)*(DWORD*)KO_HWND;

		Sleep(1000);
    }

	if(!hGameHWND)
	{
		MessageBox(0,"hGAMEHWND == NULL","Hata!",0);
		ExitProcess(0);
	}


	GetAPIChecks();
	GetFuncChecks();

	MakeTerminateFunc();
	CheckLoader();	//loader kontrol
	RandomizeFunction();	//randomize
	__asm
	{
		LEA EAX,&dwDLLCheckPointer
		PUSH EAX
		PUSH iDLLCheckFuncIndex
		SHL [ESP],2
		POP EAX
		ADD [ESP],EAX
		POP EAX
		PUSH [EAX]
		POP EAX
		PUSH [EAX]
		CALL [ESP]
		POP EAX
		CALL CheckDLLName
		CALL CheckModule
	}

	*(DWORD*)KO_APRA = 0;	// APR
	*(DWORD*)KO_APRB = 0;	// APR
	*(DWORD*)KO_APRC = 0;	// APR


	dwCheckFunctionVal = *(DWORD*) (DWORD)&CheckFunction;

	if(Make0x54Byte() - (int)dwSendPointerAdd != 1)
		OwnTerminate();

	dwServerARG += GetServerARG();

	memset(strGameID,0,25);
	memset(strGamePW,0,25);
	memset(strBotID,0,25);
	memset(strBotPW,0,25);

	memset(strCheckFile,0,256);

	char strLoginFile[256]; memset(strLoginFile,0,256);
	GetCurrentDirectoryA(255,strLoginFile);
	GetCurrentDirectoryA(255,strCheckFile);
	strcat(strLoginFile,"\\autologin.kojd");
	strcat(strCheckFile,"\\loginstatus.kojd");
	ifstream fCheck;
	fCheck.open(strLoginFile);
	if(fCheck.is_open())
	{
		char tmp[4]; *(DWORD*)tmp = 0;
		fCheck.getline(tmp,4);
		bool bRet = (bool)atoi(tmp);
		if(bRet)
		{
			bReLogin = true;
			if( ( *(DWORD*)KO_CHAR == 0 || *(DWORD*)(*(DWORD*)(KO_CHAR) + KO_MYID) == 0 ) && *(DWORD*)KO_INTR == 0 )
			{
				bAutoLogin = true;
				*(DWORD*)tmp = 0;
				fCheck.getline(tmp,4);
				bTrialLogin = (bool)atoi(tmp);

				fCheck.getline(strGameID,24);
				fCheck.getline(strGamePW,24);
				fCheck.getline(strBotID,24);
				fCheck.getline(strBotPW,24);

				StrDecrypt(strGameID);
				//StrDecrypt(strGamePW);
				StrDecrypt(strBotID);
				StrDecrypt(strBotPW);

				*(DWORD*)tmp = 0;
				fCheck.getline(tmp,4);
				iServerIndex = atoi(tmp);

				*(DWORD*)tmp = 0;
				fCheck.getline(tmp,4);
				iServerNo = atoi(tmp);

				*(DWORD*)tmp = 0;
				fCheck.getline(tmp,4);
				iCharSlot = atoi(tmp);

				dwLoginFuncPointer = (DWORD)&LoginFunction;

				CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)LoginCall,NULL,0,0);
			}
		}

		fCheck.close();
		DeleteFileA(strLoginFile);
	}

	memset(strTrialID,0,22);

	while( *(DWORD*)KO_CHAR == 0 || *(DWORD*)(*(DWORD*)(KO_CHAR) + KO_MYID) == 0 || CheckLoading() == true )
	{

		*(DWORD*)KO_APRA = 0;	// APR
		*(DWORD*)KO_APRB = 0;	// APR
		*(DWORD*)KO_APRC = 0;	// APR


		if( *(DWORD*)KO_INTR == 1 )
		{
			if(bHookPatched == false)
			{
				InitializeCriticalSection(&csSendFunc);
				TestPatch();
				PatchTPT();
				_CPacket.PatchHooked();
				bHookPatched = true;
			}
		}

		Sleep(1000);
	}

	Sleep(1000);

	bGameLoaded = true;

	_CCharBase = *(DWORD*)(KO_CHAR);

	if(!_CCharBase.GetBase())
	{
		TerminateGame(1);
		MessageBox(0,"_CharBase == NULL","Hata!",0);
		ExitProcess(0);
	}

	//PatchGameAPIs();


	*(DWORD*)KO_APRA = 0;	// APR
	*(DWORD*)KO_APRB = 0;	// APR
	*(DWORD*)KO_APRC = 0;	// APR


frmLogin = new TfrmLogin(NULL);
Application->Initialize();
Application->CreateForm(__classid(TfrmLogin), &frmLogin);
Application->Run();
Application->ProcessMessages();

frmLogin->Show();
frmLogin->BringToFront();
SetForegroundWindow(frmLogin->Handle);
return 1;
}

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		EditDbgBreakPoint();
		hDLLThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)FormThread,NULL,0,&dwDLLThread);
		break;
		case DLL_PROCESS_DETACH:
		break;
	}
	return 1;
}

//---------------------------------------------------------------------------
