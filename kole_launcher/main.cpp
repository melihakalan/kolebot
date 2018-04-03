//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void Launch()
{
	/*
	HANDLE hFile = CreateFileA("klauncher.dat",FILE_GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if( hFile == INVALID_HANDLE_VALUE )
	{
		MessageBoxA(NULL,"'klauncher.dat' ???","Error",MB_ICONSTOP);
		Application->Terminate();
		return;
	}

	CloseHandle(hFile);
	*/

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	char strDir[256]; memset(strDir,0,256);
	GetCurrentDirectoryA(255,strDir);
	strcat(strDir,"//dat");

	bool bProcess = CreateProcess("dat//klauncher.dat"," kojd",NULL,NULL,true,NULL,NULL,strDir,&si,&pi);

	if(!bProcess)
	{
		MessageBoxA(NULL,"'klauncher.dat' failed","Error",MB_ICONSTOP);
		Application->Terminate();
		return;
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	Application->Terminate();
}
