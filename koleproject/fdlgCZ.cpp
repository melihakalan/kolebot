//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "fdlgCZ.h"
#include "ffrmBot.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TdlgCZ *dlgCZ;
//--------------------------------------------------------------------- 
__fastcall TdlgCZ::TdlgCZ(TComponent* AOwner)
	: TForm(AOwner)
{
	bSuicideOnEnemy	=	false;
	bSuicideOnLimit	=	false;
	iSuicideLimit	=	25;
	bFlee			=	false;
	iFleeWait		=	20;
	bFleeTown		=	false;
	bFleeRing		=	false;
	iFleeCount		=	0;
	iFleeAction		=	0;
	iFleeRoute		=	0;
	iFleeLastPos	=	0;
	lLeftTick		=	0;
	bStopBotOnNoPT	=	false;
	bLeavePTOnDie	=	false;
	bStartInPT		=	false;
}
//---------------------------------------------------------------------
void __fastcall TdlgCZ::btncloseClick(TObject *Sender)
{
	this->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TdlgCZ::chfleeClick(TObject *Sender)
{
	//chflee->Checked = false;
	//MessageBox(this->Handle,"Bu ozellik henuz tamamlanmamistir, lutfen daha sonraki guncellemeleri bekleyiniz.","Ozellik",MB_ICONINFORMATION);

	bFlee = chflee->Checked;

	iFleeAction = 1;
	tmflee->Enabled = bFlee;
}
//---------------------------------------------------------------------------

void __fastcall TdlgCZ::chsuicide2Click(TObject *Sender)
{
	bSuicideOnLimit = chsuicide2->Checked;

	if(bSuicideOnEnemy || bSuicideOnLimit)
		tmsuicide->Enabled = true;
	else
		tmsuicide->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TdlgCZ::chsuicide1Click(TObject *Sender)
{
	bSuicideOnEnemy = chsuicide1->Checked;

	if(bSuicideOnEnemy || bSuicideOnLimit)
		tmsuicide->Enabled = true;
	else
		tmsuicide->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TdlgCZ::txtsuicidelimitChange(TObject *Sender)
{
	iSuicideLimit = atoi(txtsuicidelimit->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TdlgCZ::tmsuicideTimer(TObject *Sender)
{
	if(CheckLoading() == true)
		return;

	if(_CCharBase.GetDWORD(KO_CRHP) == NULL)
		return;

	char pDie[3];
	*pDie = 0x29;
	*(pDie + 1) = 0x01;
	*(pDie + 2) = 0x03;

	char pRevive[1];
	*pRevive = 0x12;

	if(bSuicideOnEnemy)
	{
		CCharBase pUser = _CArea.GetNearstUPC(true,true,10.0);
		if(pUser.GetBase() != NULL)
		{
			_CPacket.PSend(pDie,3);
			_CPacket.PSend(pRevive,1);
			return;
		}

	}

	if(bSuicideOnLimit)
	{
		DWORD dwHP = _CCharBase.GetDWORD(KO_CRHP);
		DWORD dwMaxHP = _CCharBase.GetDWORD(KO_MXHP);
		if(_CMath.iPerc(dwMaxHP,dwHP) <= (float)iSuicideLimit)
		{
			_CPacket.PSend(pDie,3);
			_CPacket.PSend(pRevive,1);
        }
	}
}
//---------------------------------------------------------------------------

void __fastcall TdlgCZ::chstoponnoptClick(TObject *Sender)
{
	bStopBotOnNoPT = chstoponnopt->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TdlgCZ::chleaveondieClick(TObject *Sender)
{
	bLeavePTOnDie = chleaveondie->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TdlgCZ::chstartinptClick(TObject *Sender)
{
	bStartInPT = chstartinpt->Checked;
	tmptcheck->Enabled = bStartInPT;
}
//---------------------------------------------------------------------------

void __fastcall TdlgCZ::tmptcheckTimer(TObject *Sender)
{
	if(frmBot->bBotState)
		return;

	if(frmBot->bBotPaused)
		return;

	if(CheckLoading())
		return;

	if(_CCharBase.GetDWORD(KO_CRHP) == 0)
		return;

	_CParty.Refresh();
	if(_CParty.GetCount() >= 2)
		frmBot->btnstartClick(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TdlgCZ::txtfleewaitChange(TObject *Sender)
{
	iFleeWait = atoi(txtfleewait->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TdlgCZ::chfleetownClick(TObject *Sender)
{
	bFleeTown = chfleetown->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TdlgCZ::chfleeringClick(TObject *Sender)
{
	bFleeRing = chfleering->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TdlgCZ::btnfleeaddClick(TObject *Sender)
{
	if(iFleeCount == 20)
		return;

	float fMyX = _CCharBase.GetFLOAT(KO_POSX);
	float fMyY = _CCharBase.GetFLOAT(KO_POSY);

	char tmp1[8],tmp2[8];
	memset(tmp1,0,8);
	memset(tmp2,0,8);

	itoa((int)fMyX,tmp1,10);
	itoa((int)fMyY,tmp2,10);

	char buf[16];
	memset(buf,0,16);

	strcpy(buf,tmp1);
	strcat(buf,",");
	strcat(buf,tmp2);

	lstflee->AddItem(buf,Sender);

	iFleeX[iFleeCount] = (int)fMyX;
	iFleeY[iFleeCount] = (int)fMyY;
	iFleeCount++;
}
//---------------------------------------------------------------------------

void __fastcall TdlgCZ::btnfleeclearClick(TObject *Sender)
{
	lstflee->Clear();
	iFleeCount = 0;
}
//---------------------------------------------------------------------------

void __fastcall TdlgCZ::tmfleeTimer(TObject *Sender)
{
	if( !frmBot->bBotState )
		return;

	if(!iFleeCount)
		return;

	if(frmBot->bOnSundWay || frmBot->bOnPotWay || frmBot->bOnInnWay)
		return;

	if( CheckLoading() )
		return;


	if( !_CCharBase.GetDWORD(KO_CRHP) )
		return;

	if(_CCharBase.IsSit())
		_CTest.CommandSit(false);

	float fMyX = _CCharBase.GetFLOAT(KO_POSX);
	float fMyY = _CCharBase.GetFLOAT(KO_POSY);
	int iMyX = (int)fMyX;
	int iMyY = (int)fMyY;
	bool bTowned = false;

	if(iFleeAction == 1)	// scan
	{
		CCharBase pEnemy = _CArea.GetNearstUPC(true,true,20.0);

		if(!pEnemy.GetBase())
			return;

		iFleeRoute		= 1;		//leave
		iFleeAction		= 2;		//flee
		iFleeLastPos  	= 0;
		frmBot->bBotPaused = true;
		return;
	}
	else if(iFleeAction == 2)	//flee
	{
		if(SHTeleport(iFleeX[iFleeLastPos],iFleeY[iFleeLastPos],5))
			iFleeLastPos++;

		if(iFleeLastPos == iFleeCount)	//left
		{
			iFleeAction = 	3;	//wait
			iFleeLastPos--;
			iFleeRoute = 	2;
			lLeftTick = GetTickCount();
			return;
		}
	}
	else if(iFleeAction == 3)	//wait
	{
		if( GetTickCount() - lLeftTick < (iFleeWait * 1000) )
			return;

		iFleeAction = 4;	//return
		return;
	}
	else if(iFleeAction == 4)	//return
	{
		if(_CMath.fDist(fMyX,fMyY,iFleeX[0],iFleeY[0]) <= 5.0)
			iFleeLastPos = 0;

		if(SHTeleport(iFleeX[iFleeLastPos],iFleeY[iFleeLastPos],5))
			iFleeLastPos--;

		if(iFleeLastPos == -1)	//returned
		{
			CCharBase pEnemyCheck = _CArea.GetNearstUPC(true,true,20.0);
			if(pEnemyCheck.GetBase())
			{
				if(bFleeTown)
				{
					char pTown[1]; *pTown = 0x48;
					_CPacket.PSend(pTown,1);
					bTowned = true;
				}
				if(bFleeRing)
				{
					RingAlarm();
				}
			}

			if(bTowned)
			{
				frmBot->bBotPaused = false;
				StopBot();
				chflee->Checked = false;
				tmflee->Enabled = false;
				return;
			}

			frmBot->bBotPaused = false;
			iFleeAction = 1;	//scan
			return;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TdlgCZ::chmeatClick(TObject *Sender)
{
	frmBot->bAutoMeat = chmeat->Checked;
}
//---------------------------------------------------------------------------

