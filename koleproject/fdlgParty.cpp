//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "fdlgParty.h"
#include "ffrmBot.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TdlgParty *dlgParty;
//---------------------------------------------------------------------
__fastcall TdlgParty::TdlgParty(TComponent* AOwner)
	: TForm(AOwner)
{
	bAccept = true;
	bRequest = true;
}
//---------------------------------------------------------------------
void __fastcall TdlgParty::btnprintpartyClick(TObject *Sender)
{
	if(frmBot->iAddCount == 20)
		return;

	_CParty.Refresh();
	int iPartyCount = _CParty.GetCount();

	if(iPartyCount < 2)
		return;

	DWORD dwUser;

	for (int i = 0; i < iPartyCount; i++) {
		if(frmBot->iAddCount == 20)
			break;
		dwUser = _CParty.GetUser(i);
		if(dwUser){
			if(*(DWORD*)(dwUser + KO_PT_ID) != dwMyID){
				char *pNewName = new char [32];
				memset(pNewName,0,32);
				strcpy(pNewName,(LPSTR)*(DWORD*)(dwUser + KO_PT_NICK));
				frmBot->strAddList[frmBot->iAddCount] = pNewName;
				lstpartyadd->AddItem(pNewName,Sender);
				frmBot->iAddCount++;
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TdlgParty::btnaddselectedClick(TObject *Sender)
{
	if(frmBot->iAddCount == 20)
		return;

	CCharBase pSelected = _CArea.GetUPCBase(_CCharBase.GetTargetID());

	if(pSelected.GetBase()){

		char *pNewName = new char [32];
		memset(pNewName,0,32);
		strcpy(pNewName,pSelected.GetName());
		frmBot->strAddList[frmBot->iAddCount] = pNewName;
		lstpartyadd->AddItem(pNewName,Sender);
		frmBot->iAddCount++;
	}
}
//---------------------------------------------------------------------------

void __fastcall TdlgParty::btnaddClick(TObject *Sender)
{
	if(frmBot->iAddCount == 20)
		return;

	if(!txtname->Text.Length() || txtname->Text.Length() >= 32)
		return;

	char *pNewName = new char [32];
	memset(pNewName,0,32);

	strcpy(pNewName,txtname->Text.c_str());
	frmBot->strAddList[frmBot->iAddCount] = pNewName;
	lstpartyadd->AddItem(pNewName,Sender);
	frmBot->iAddCount++;
}
//---------------------------------------------------------------------------

void __fastcall TdlgParty::btnclearClick(TObject *Sender)
{
	if(!frmBot->iAddCount)
		return;

	lstpartyadd->Clear();

	for (int i = 0; i < 20; i++) {
		if(frmBot->strAddList[i])
			delete [] frmBot->strAddList[i];
		frmBot->strAddList[i] = NULL;
	}

	frmBot->iAddCount = 0;
}
//---------------------------------------------------------------------------

void __fastcall TdlgParty::chautochataddClick(TObject *Sender)
{
	frmBot->bAutoInvite = chautochatadd->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TdlgParty::btnOkClick(TObject *Sender)
{
	this->Hide();
}
//---------------------------------------------------------------------------

