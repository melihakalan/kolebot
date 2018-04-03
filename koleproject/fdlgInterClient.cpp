//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "fdlgInterClient.h"
#include "ffrmBot.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TdlgInterClient *dlgInterClient;
//---------------------------------------------------------------------
__fastcall TdlgInterClient::TdlgInterClient(TComponent* AOwner)
	: TForm(AOwner)
{
	bLockClick = false;
}
//---------------------------------------------------------------------
void __fastcall TdlgInterClient::btnstartallClick(TObject *Sender)
{
	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,0,0);
	}
}
//---------------------------------------------------------------------------

void __fastcall TdlgInterClient::btnstopallClick(TObject *Sender)
{
	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,1,0);
	}
}
//---------------------------------------------------------------------------

void __fastcall TdlgInterClient::btnmakepartyClick(TObject *Sender)
{
	srand( GetTickCount() );
	iICPartyPass = rand() % 9999 + 1;
	bICPartyFirstUser = true;

	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,11,iICPartyPass);
	}
}
//---------------------------------------------------------------------------

void __fastcall TdlgInterClient::btnaddmobClick(TObject *Sender)
{
	int iMobID = _CCharBase.GetTargetID();

	if(!iMobID || iMobID == (int)0xFFFF)
		return;

	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,12,iMobID);
	}

	SendNotifyMessage(hGameHWND,WM_KOLEBOT,12,iMobID);
}
//---------------------------------------------------------------------------

void __fastcall TdlgInterClient::btnwearClick(TObject *Sender)
{
	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,13,0);
	}
	WearNoobItem();
}
//---------------------------------------------------------------------------

void __fastcall TdlgInterClient::btnfollowClick(TObject *Sender)
{
	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,10,1);
	}
}
//---------------------------------------------------------------------------

void __fastcall TdlgInterClient::btnnofollowClick(TObject *Sender)
{
	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,10,0);
	}
}
//---------------------------------------------------------------------------

void __fastcall TdlgInterClient::btntownClick(TObject *Sender)
{
	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,3,0);
	}
	SendNotifyMessage(hGameHWND,WM_KOLEBOT,3,0);
}
//---------------------------------------------------------------------------

void __fastcall TdlgInterClient::btnsitClick(TObject *Sender)
{
	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,4,0);
	}
	SendNotifyMessage(hGameHWND,WM_KOLEBOT,4,0);
}
//---------------------------------------------------------------------------

void __fastcall TdlgInterClient::btnstandClick(TObject *Sender)
{
	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,5,0);
	}
	SendNotifyMessage(hGameHWND,WM_KOLEBOT,5,0);
}
//---------------------------------------------------------------------------

void __fastcall TdlgInterClient::btncloseClick(TObject *Sender)
{
	this->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TdlgInterClient::btnexpandClick(TObject *Sender)
{
	int iVal = atoi(txtexpand->Text.c_str());

	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,14,iVal);
	}
}
//---------------------------------------------------------------------------

void __fastcall TdlgInterClient::btnsuicideClick(TObject *Sender)
{
	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,15,0);
	}
	SendNotifyMessage(hGameHWND,WM_KOLEBOT,15,0);
}
//---------------------------------------------------------------------------

void __fastcall TdlgInterClient::btnsaveClick(TObject *Sender)
{
	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,16,0);
	}
	SendNotifyMessage(hGameHWND,WM_KOLEBOT,16,0);
	MessageBox(this->Handle,"Hepsi kaydedildi!","Kayit",MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------

void __fastcall TdlgInterClient::btnexitClick(TObject *Sender)
{
	int iMsg = MessageBox(this->Handle,"Bütün clientler kapatýlsýn mý?","Kapat",MB_ICONINFORMATION | MB_YESNO);
	if(iMsg == ID_NO)
		return;

	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,17,0);
	}

	CloseBot(false);
	TerminateProcess((void*)-1,0);
}
//---------------------------------------------------------------------------

void __fastcall TdlgInterClient::btncallClick(TObject *Sender)
{
	int iMyID = (int) _CCharBase.GetID();
	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,18,iMyID);
	}
}
//---------------------------------------------------------------------------

void __fastcall TdlgInterClient::chstickClick(TObject *Sender)
{
	if(bLockClick)
		return;

	frmBot->bStickSH = chstick->Checked;
	int iVal = (int) frmBot->bStickSH;

	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,20,iVal);
	}
}
//---------------------------------------------------------------------------

void __fastcall TdlgInterClient::btnrotaClick(TObject *Sender)
{

	SaveSundIC();
	SavePotIC();
	SaveInnIC();

	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendMessage(frmBot->hWindowArray[i],WM_KOLEBOT,21,0);
	}

	Sleep(1000);

	DeleteFileA("C:\\_sund.kojd");
	DeleteFileA("C:\\_inn.kojd");
	DeleteFileA("C:\\_pot.kojd");

}
//---------------------------------------------------------------------------

