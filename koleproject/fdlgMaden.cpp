//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "fdlgMaden.h"
#include "ffrmBot.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TdlgMaden *dlgMaden;

BYTE pPacket_MineMain[6] = {0x64,0x07,0x54,0x4E,0x00,0x00};
BYTE pPacket_MineOre1[19] = {0x55,0x00,0x10,0x33,0x31,0x35,0x31,0x31,0x5F,0x50,0x69,0x74,0x6D,0x61,0x6E,0x2E,0x6C,0x75,0x61};
BYTE pPacket_MineOre2[19] = {0x55,0x01,0x10,0x33,0x31,0x35,0x31,0x31,0x5F,0x50,0x69,0x74,0x6D,0x61,0x6E,0x2E,0x6C,0x75,0x61};
//--------------------------------------------------------------------- 
__fastcall TdlgMaden::TdlgMaden(TComponent* AOwner)
	: TForm(AOwner)
{

	bEnabled 	  	=	false;

	iCurrentAction	=	0;	//nothing
	bTowned		   	=	false;

	bSundOK			=	false;
	bInnOK			=	false;
	bInnOpened		=	false;

	lItemList[0]	=	ITEM_HP45;			bStackable[0] = true;
	lItemList[1]	=	ITEM_HP90;			bStackable[1] = true;
	lItemList[2]	=	ITEM_HP180;         bStackable[2] = true;
	lItemList[3]	=	ITEM_HP360;         bStackable[3] = true;
	lItemList[4]	=	ITEM_HP720;         bStackable[4] = true;
	lItemList[5]	=	ITEM_MP120;         bStackable[5] = true;
	lItemList[6]	=	ITEM_MP240;         bStackable[6] = true;
	lItemList[7]	=	ITEM_MP480;         bStackable[7] = true;
	lItemList[8]	=	ITEM_MP960;         bStackable[8] = true;
	lItemList[9]	=	ITEM_MP1920;        bStackable[9] = true;
	lItemList[10]	=	ITEM_ROCK;          bStackable[10] = true;
	lItemList[11]	=	ITEM_ACID;          bStackable[11] = true;
	lItemList[12]	=	ITEM_BEZOAR;        bStackable[12] = true;
	lItemList[13]	=	ITEM_GLUTINOUS;     bStackable[13] = true;
	lItemList[14]	=	ITEM_FIRECRACKER;   bStackable[14] = true;
	lItemList[15]	=	ITEM_FLOWERFC;      bStackable[15] = true;
	lItemList[16]	=	ITEM_UP_LOWCLASS;   bStackable[16] = true;
	lItemList[17]	=	ITEM_CRYSTAL;       bStackable[17] = true;
	lItemList[18]	=	ITEM_OPAL;          bStackable[18] = true;
	lItemList[19]	=	ITEM_SAPPHIRE;      bStackable[19] = true;
	lItemList[20]	=	ITEM_CALLINGSC;     bStackable[20] = true;
	lItemList[21]	=	ITEM_TS_SCROLL;     bStackable[21] = true;
	lItemList[22]	=	ITEM_BUS;           bStackable[22] = true;
	lItemList[23]	=	ITEM_UP_SC;         bStackable[23] = true;
	lItemList[24]	=	ITEM_ABYSS;         bStackable[24] = true;
	lItemList[25]	=	ITEM_MP_RICE;       bStackable[25] = false;
	lItemList[26]	=	ITEM_MS;            bStackable[26] = false;
	lItemList[27]	=	ITEM_BLUE_CHEST;    bStackable[27] = true;
	lItemList[28]	=	ITEM_GREEN_CHEST;   bStackable[28] = true;
	lItemList[29]	=	ITEM_RED_CHEST;     bStackable[29] = true;
	lItemList[30]	=	ITEM_BRONZE_COIN;   bStackable[30] = false;
	lItemList[31]	=	ITEM_SILVER_COIN;  	bStackable[31] = false;
	lItemList[32]	=	ITEM_GOLD_COIN;   	bStackable[32] = false;

	strItemList[0] = "HP Pot(45)";
	strItemList[1] = "HP Pot(90)";
	strItemList[2] = "HP Pot(180)";
	strItemList[3] = "HP Pot(360)";
	strItemList[4] = "HP Pot(720)";
	strItemList[5] = "MP Pot(120)";
	strItemList[6] = "MP Pot(240)";
	strItemList[7] = "MP Pot(480)";
	strItemList[8] = "MP Pot(960)";
	strItemList[9] = "MP Pot(1920)";
	strItemList[10] = "Sling";
	strItemList[11] = "Acid potion";
	strItemList[12] = "Bezoar";
	strItemList[13] = "Glutinous";
	strItemList[14] = "Firecracker";
	strItemList[15] = "Flower Firecracker";
	strItemList[16] = "Low scroll";
	strItemList[17] = "Crystal";
	strItemList[18] = "Opal";
	strItemList[19] = "Sapphire";
	strItemList[20] = "Calling Friend Sc";
	strItemList[21] = "Transformation Scroll";
	strItemList[22] = "BUS";
	strItemList[23] = "Upgrade Scroll";
	strItemList[24] = "Abyss Gem";
	strItemList[25] = "MP Rice Cake";
	strItemList[26] = "Monster Stone";
	strItemList[27] = "Mavi kutu";
	strItemList[28] = "Yesil kutu";
	strItemList[29] = "Kirmizi kutu";
	strItemList[30] = "Bronze coin";
	strItemList[31] = "Silver coin";
	strItemList[32] = "Gold coin";

	lFragmentList[0] = ITEM_FRAGMENT1;
	lFragmentList[1] = ITEM_FRAGMENT2;
	lFragmentList[2] = ITEM_FRAGMENT3;
	lFragmentList[3] = ITEM_FRAGMENT4;
	lFragmentList[4] = ITEM_FRAGMENT5;
	lFragmentList[5] = ITEM_FRAGMENT6;
	lFragmentList[6] = ITEM_FRAGMENT7;

	strFragmentList[0] = "F.Arrogance";
	strFragmentList[1] = "F.Avarice";
	strFragmentList[2] = "F.Gluttony";
	strFragmentList[3] = "F.Jealousy";
	strFragmentList[4] = "F.Lechery";
	strFragmentList[5] = "F.Rage";
	strFragmentList[6] = "F.Sloth";

	iMineX = 640;	iMineY = 375;
	iPitmanX = 664;	iPitmanY = 411;

	iTownToInnX[0] = 820;	iTownToInnY[0] = 440;	//Town
	iTownToInnX[1] = 808;	iTownToInnY[1] = 528;
	iTownToInnX[2] = 760;	iTownToInnY[2] = 523;	//Sundries
	iTownToInnX[3] = 740;	iTownToInnY[3] = 573;
	iTownToInnX[4] = 727;	iTownToInnY[4] = 613;
	iTownToInnX[5] = 764;	iTownToInnY[5] = 616;
	iTownToInnX[6] = 763;	iTownToInnY[6] = 649;	//Inn

	iTownToMineX = 675;	iTownToMineY = 425;

	iNextX		=	0;
	iNextY		=	0;
	iLastPos	=	0;

	bTownOK		=	false;
	dwLastInnBase = 0;

	bOnSundWay	=	false;
	bOnMineWay 	=	false;

	lLastStartTick	=	0;

	bSH = true;

}
//---------------------------------------------------------------------

void __fastcall TdlgMaden::FormCreate(TObject *Sender)
{
	for (int i = 0; i < 4; i++)
	{
		lstsund->Checked[i] = true;
		iItemActionList[i] = ACT_SELL;
	}

	lstinn->Checked[4] = true;
	iItemActionList[4] = ACT_STORE;

	for (int i = 5; i < 8; i++)
	{
		lstsund->Checked[i] = true;
		iItemActionList[i] = ACT_SELL;
	}

	lstinn->Checked[8] = true;
	iItemActionList[8] = ACT_STORE;
	lstinn->Checked[9] = true;
	iItemActionList[9] = ACT_STORE;

	for (int i = 10; i < 17; i++)
	{
		lstsund->Checked[i] = true;
		iItemActionList[i] = ACT_SELL;
	}

	for (int i = 17; i < 25; i++)
	{
		lstinn->Checked[i] = true;
		iItemActionList[i] = ACT_STORE;
	}

	lstremove->Checked[25] = true;
	iItemActionList[25] = ACT_REMOVE;

	lstnop->Checked[26] = true;
	iItemActionList[26] = ACT_NOP;

	lstinn->ItemEnabled[25] = false;
	lstinn->ItemEnabled[26] = false;
	lstsund->ItemEnabled[25] = false;
	lstsund->ItemEnabled[26] = false;

	for (int i = 27; i < 30; i++)
	{
		lstinn->Checked[i] = true;
		iItemActionList[i] = ACT_STORE;
	}

	for (int i = 30; i < 33; i++)
	{
		lstsund->Checked[i] = true;
		iItemActionList[i] = ACT_SELL;
	}
}
//---------------------------------------------------------------------------

void __fastcall TdlgMaden::lstinnClick(TObject *Sender)
{
	ClickAction( ACT_STORE, lstinn->ItemIndex );
}
//---------------------------------------------------------------------------

void __fastcall TdlgMaden::lstsundClick(TObject *Sender)
{
	ClickAction( ACT_SELL, lstsund->ItemIndex );
}
//---------------------------------------------------------------------------

void __fastcall TdlgMaden::lstremoveClick(TObject *Sender)
{
	ClickAction( ACT_REMOVE, lstremove->ItemIndex );
}
//---------------------------------------------------------------------------

void __fastcall TdlgMaden::lstnopClick(TObject *Sender)
{
	ClickAction( ACT_NOP, lstnop->ItemIndex );
}
//---------------------------------------------------------------------------

void ClickAction(int iType, int iItemIndex)
{
	switch( iType )
	{
		case ACT_STORE:
		{
			dlgMaden->lstsund->Checked[iItemIndex] = false;
			dlgMaden->lstremove->Checked[iItemIndex] = false;
			dlgMaden->lstnop->Checked[iItemIndex] = false;
			dlgMaden->iItemActionList[iItemIndex] = ACT_STORE;
			break;
		}

		case ACT_SELL:
		{
			dlgMaden->lstinn->Checked[iItemIndex] = false;
			dlgMaden->lstremove->Checked[iItemIndex] = false;
			dlgMaden->lstnop->Checked[iItemIndex] = false;
			dlgMaden->iItemActionList[iItemIndex] = ACT_SELL;
			break;
		}

		case ACT_REMOVE:
		{
			dlgMaden->lstinn->Checked[iItemIndex] = false;
			dlgMaden->lstsund->Checked[iItemIndex] = false;
			dlgMaden->lstnop->Checked[iItemIndex] = false;
			dlgMaden->iItemActionList[iItemIndex] = ACT_REMOVE;
			break;
		}

		case ACT_NOP:
		{
			dlgMaden->lstinn->Checked[iItemIndex] = false;
			dlgMaden->lstsund->Checked[iItemIndex] = false;
			dlgMaden->lstremove->Checked[iItemIndex] = false;
			dlgMaden->iItemActionList[iItemIndex] = ACT_NOP;
			break;
		}
    }
}

void __fastcall TdlgMaden::btncloseClick(TObject *Sender)
{
	this->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TdlgMaden::chmadenClick(TObject *Sender)
{
	bEnabled = chmaden->Checked;
}
//---------------------------------------------------------------------------


int iRandomPos(int iVal, int iMaxChange)
{
	srand( GetCurrentProcessId() + GetTickCount() );
	int iChange = rand() % iMaxChange + 1;

	int iType = rand() % 2 + 1;

	if(iType == 1)
		return (iVal + iChange);

	return (iVal - iChange);
}

void Town()
{
	char pTown[1]; *pTown = 0x48;
	_CPacket.PSend(pTown,1);
}

void __fastcall TdlgMaden::tmmadenTimer(TObject *Sender)
{
	if( !frmBot->bBotState )
		return;

	if( frmBot->bBotPaused )
		return;

	if( CheckLoading() )
		return;

	if( !_CCharBase.GetDWORD(KO_CRHP) )
	{
		char pSpawn[1]; *pSpawn = 0x12;
		_CPacket.PSend(pSpawn,1);
		iCurrentAction = 1;
		bTowned = false;
		bTownOK = false;
		return;
	}

	if( !iCurrentAction )
		return;

	float fMyX = _CCharBase.GetFLOAT(KO_POSX);
	float fMyY = _CCharBase.GetFLOAT(KO_POSY);

	char pStrLog[128];
	char tmp[4];
	char pFileName[64];
	long lTmpTick;
	int iZone = _CCharBase.GetDWORD(KO_ZONE);

	if( iCurrentAction == 1 )	//going to mine
	{
		bDisabledRecvByte = 0x00;
		if( !bTowned )
		{
			bTowned = true;

			if( _CMath.fDist(fMyX,fMyY,iTownToInnX[0],iTownToInnY[0]) >= 5.0 )
				Town();

			if( iZone == 21)
			{
				iNextX = iTownToMineX;
				iNextY = iTownToMineY;
			}
			else if( iZone == 1 )
			{
				iNextX = 456;
				iNextY = 1634;
			}
			else if( iZone == 2 )
			{
				iNextX = 1594;
				iNextY = 430;
			}
			iLastPos = 0;
		}

		if( !bTownOK && _CMath.fDist(fMyX,fMyY,iTownToInnX[0],iTownToInnY[0]) >= 5.0 )
			return;

		bTownOK = true;

		if(Move(iNextX,iNextY,5))
		{
			if(iLastPos == 2)
			{
				iCurrentAction = 2;	//start mining
				return;
			}
			if( iZone == 21 )
			{
				iNextX = iRandomPos(iMineX,15);
				Sleep(10);
				iNextY = iRandomPos(iMineY,15);
				iLastPos = 2;
			}
			else
			{
				if(!iLastPos)
				{
					iNextX = iTownToMineX;
					iNextY = iTownToMineY;
				}
				else
				{
					iNextX = iRandomPos(iMineX,15);
					Sleep(10);
					iNextY = iRandomPos(iMineY,15);
                }
				iLastPos++;
			}
			return;
		}
	}
	else if( iCurrentAction == 2 )	//start mining
	{
		char pStart[2]; *(WORD*)pStart = 0x0186;
		_CPacket.PSend(pStart,2);
		iCurrentAction = 3;	//mining
		bOnSundWay = false;
		bOnMineWay = false;
		lLastStartTick = GetTickCount();

		if(!tmrpr->Enabled)
			tmrpr->Enabled = true;

		return;
	}
	else if( iCurrentAction == 3 )	//mining
	{
		if( !IsMining() && GetTickCount() - lLastStartTick >= 3000 )
		{
			iCurrentAction = 1;
			bTowned = false;
			bTownOK = false;
			return;
		}

		int iFirstFreeSlot = _CBag.FindFreeInvSlot(0);
		if(iFirstFreeSlot == -1 || iFirstFreeSlot == 27)
		{
			iCurrentAction = 4;	//going pitman
			bOnMineWay = true;
			return;
		}

		int iSecondFreeSlot = _CBag.FindFreeInvSlot(iFirstFreeSlot + 1);
		if(iSecondFreeSlot == -1)
		{
			iCurrentAction = 4;	//going pitman
			bOnMineWay = true;
			return;
		}
		return;
	}
	else if( iCurrentAction == 4 )	//going pitman
	{
		if( Move(iPitmanX,iPitmanY,5) )
		{
			iCurrentAction = 5; //pitman process
			return;
		}
	}
	else if( iCurrentAction == 5 )	//pitman process
	{
		CCharBase pPitman = _CArea.GetNPCByName(1,0,"[Pitman] kursat",false,false,false,5.0,false);
		if(!pPitman.GetBase())
		{
			Town();
			bTownOK = false;
			iCurrentAction = 6;	// going npc
			iLastPos = 0;
			bSundOK = false;
			bInnOK = false;
			bInnOpened = false;
			return;
		}

		RunNPC(pPitman.GetID());

		int iOreSlot;
		int iLastOreSlot = 0;
		while(1)
		{
			iOreSlot = _CBag.FindInvItem(iLastOreSlot,ITEM_NORMAL_ORE);

			if(iOreSlot == -1)
				break;

			_CPacket.PSend(pPacket_MineMain,6);
			_CPacket.PSend(pPacket_MineOre1,19);
			iLastOreSlot++;
			Sleep(100);
		}

		iLastOreSlot = 0;
		while(1)
		{
			iOreSlot = _CBag.FindInvItem(iLastOreSlot,ITEM_GOLD_ORE);

			if(iOreSlot == -1)
				break;

			_CPacket.PSend(pPacket_MineMain,6);
			_CPacket.PSend(pPacket_MineOre2,19);
			iLastOreSlot++;
			Sleep(100);
		}

		Town();
		bTownOK = false;
		iCurrentAction = 6;	// going npc
		iLastPos = 0;
		bSundOK = false;
		bInnOK = false;
		bInnOpened = false;
		return;
	}
	else if( iCurrentAction == 6 )	//going npc
	{
		if( !bTownOK && _CMath.fDist(fMyX,fMyY,iTownToInnX[0],iTownToInnY[0]) >= 5.0 )
			return;

		bTownOK = true;

		while( !iTownToInnX[iLastPos] || !iTownToInnY[iLastPos] )
			iLastPos++;

		if( bInnOpened || Move(iTownToInnX[iLastPos],iTownToInnY[iLastPos],5) )
		{
			if(!bInnOpened)
				iLastPos++;

			if(iLastPos == 3 && !bSundOK)	//sundries
			{
				bSundOK = true;
				CCharBase pSund;
				lTmpTick = GetTickCount();

				while(1)
				{
					if(iZone == 21)
						pSund = _CArea.GetNPCByName(1,0,"[Sundries]Zarta",false,false,false,5.0,false);
					else if(iZone == 1)
						pSund = _CArea.GetNPCByName(1,0,"[Sundries]Veron",false,false,false,5.0,false);
					else if(iZone == 2)
						pSund = _CArea.GetNPCByName(1,0,"[Sundries]Yasval",false,false,false,5.0,false);

					if(pSund.GetBase())
						break;

					if(GetTickCount() - lTmpTick >= 5000)
						return;

					Sleep(100);
				}

				RepairLeft(pSund.GetID());
				RepairRight(pSund.GetID());

				CInvSlot pSell;
				int iNextSlot;
				for (int i = 0; i < 33; i++)
				{
					iNextSlot = 0;
					if( iItemActionList[i] == ACT_SELL )
					{
						while(1)
						{
							pSell = _CBag.FindInvItem(iNextSlot,lItemList[i]);

							if(!pSell.GetBase())
								break;

							SellItem(pSund.GetID(),pSell.GetCurrentSlot());
							iNextSlot = pSell.GetCurrentSlot() + 1;
							Sleep(10);
						}
					}
				}

				CInvSlot pRemove;
				int iRemain;
				bDisabledRecvByte = 0x3F;
				for (int i = 0; i < 33; i++)
				{
					iNextSlot = 0;
					if( iItemActionList[i] == ACT_REMOVE )
					{
						//if(lItemList[i] == ITEM_MS)
							//continue;

						while(1)
						{
							pRemove = _CBag.FindInvItem(iNextSlot,lItemList[i]);

							if(!pRemove.GetBase())
								break;

							if(lItemList[i] == ITEM_MP_RICE)
							{
								iRemain = 3;
								while(iRemain)
								{
									*(DWORD*)(pBuf_RemoveSelf + 2) = SKILL_MPRICE;
									*(DWORD*)(pBuf_BuffSelf2 + 2) = SKILL_MPRICE;
									_CPacket.PSend(pBuf_RemoveSelf,24);
									_CPacket.PSend(pBuf_BuffSelf2,24);
									iRemain--;
									Sleep(100);
								}
								iNextSlot = pRemove.GetCurrentSlot() + 1;
							}
							else
							{
								RemoveItem(pRemove.GetCurrentSlot());
								iNextSlot = pRemove.GetCurrentSlot() + 1;
								Sleep(10);
							}
						}
					}
				}

				RefreshInv();
			}


			if(iLastPos == 7 && !bInnOK)	//inn
			{
				CCharBase pInn;
				if(!bInnOpened)
				{
					lTmpTick = GetTickCount();
					while(1)
					{
						if(iZone == 21)
							pInn = _CArea.GetNPCByName(1,0,"[InnHostess] Neria",false,false,false,5.0,false);
						else if(iZone == 1)
							pInn = _CArea.GetNPCByName(1,0,"[InnHost]Galid",false,false,false,5.0,false);
						else if(iZone == 2)
							pInn = _CArea.GetNPCByName(1,0,"[InnHost]Lined",false,false,false,5.0,false);

						if(pInn.GetBase())
							break;

						if(GetTickCount() - lTmpTick >= 5000)
						{
							bDisabledRecvByte = 0x00;
							bInnOK = true;
							bTowned = false;
							bTownOK = false;
							iCurrentAction = 1;	//going to mine
							return;
						}

						Sleep(100);
					}

					OpenInn(pInn.GetID());
					bInnOpened = true;
					dwLastInnBase = pInn.GetBase();
					return;
				}

				pInn.SetBase(dwLastInnBase);

				if( !CheckInn() )
					return;

				bDisabledRecvByte = 0x45;

				CloseInnDlg();

				bInnOK = true;
				bTowned = false;
				bTownOK = false;
				iCurrentAction = 1;	//going to mine

				CInvSlot pInv;
				int iNextSlot2;
				int iNextInnSlot = 0;
				int iInnSlot;
				bool bStoreOn = false;
				for (int i = 0; i < 33; i++)
				{
					iNextSlot2 = 0;
					if( iItemActionList[i] == ACT_STORE )
					{
						while(1)
						{
							pInv = _CBag.FindInvItem(iNextSlot2,lItemList[i]);

							if(!pInv.GetBase())
								break;

							if( bStackable[i] )
							{
								iInnSlot = _CBag.FindInnItem(0,lItemList[i]);
								bStoreOn = true;
								if(iInnSlot == -1)
								{
									iInnSlot = _CBag.FindFreeInnSlot(iNextInnSlot);
									bStoreOn = false;
								}
							}
							else
							{
								iInnSlot = _CBag.FindFreeInnSlot(iNextInnSlot);
								bStoreOn = false;
							}

							if(iInnSlot == -1)
								break;

							StoreItem(pInn.GetID(),pInv.GetCurrentSlot(),iInnSlot);
							iNextSlot2 = pInv.GetCurrentSlot() + 1;
							if(!bStoreOn)
								iNextInnSlot = iInnSlot + 1;

							memset(pStrLog,0,128);
							strcpy(pStrLog,"Bankaya koyuldu -> ");
							strcat(pStrLog,strItemList[i]);

							*(DWORD*)tmp = 0;
							itoa(pInv.iItemN(),tmp,10);
							strcat(pStrLog,"(");
							strcat(pStrLog,tmp);
							strcat(pStrLog,")");

							memset(pFileName,0,64);
							strcpy(pFileName,_CCharBase.GetName());
							strcat(pFileName,"_MADENBILGI.log");

							WriteLog(pFileName,pStrLog);

							Sleep(10);
						}
                    }
				}

				//iNextInnSlot = 0;
				for (int i = 0; i < 7; i++)
				{
					iNextSlot2 = 0;
					while(1)
					{
						pInv = _CBag.FindInvItem(iNextSlot2,lFragmentList[i]);
						if(!pInv.GetBase())
							break;

						iInnSlot = _CBag.FindInnItem(0,lFragmentList[i]);
						bStoreOn = true;
						if(iInnSlot == -1)
						{
							iInnSlot = _CBag.FindFreeInnSlot(iNextInnSlot);
							bStoreOn = false;
						}

						if(iInnSlot == -1)
							break;

						StoreItem(pInn.GetID(),pInv.GetCurrentSlot(),iInnSlot);
						iNextSlot2 = pInv.GetCurrentSlot() + 1;
						if(!bStoreOn)
							iNextInnSlot = iInnSlot + 1;

						memset(pStrLog,0,128);
						strcpy(pStrLog,"Bankaya koyuldu -> ");
						strcat(pStrLog,strFragmentList[i]);

						*(DWORD*)tmp = 0;
						itoa(pInv.iItemN(),tmp,10);
						strcat(pStrLog,"(");
						strcat(pStrLog,tmp);
						strcat(pStrLog,")");

						memset(pFileName,0,64);
						strcpy(pFileName,_CCharBase.GetName());
						strcat(pFileName,"_MADENBILGI.log");

						WriteLog(pFileName,pStrLog);

						Sleep(10);
					}
				}
				Sleep(1000);
				RefreshInv();
			}

		}
	}

}
//---------------------------------------------------------------------------

void SellItem(DWORD dwSundID, int iItemSlot)
{
	CInvSlot pItem = iItemSlot;

	if(!pItem.GetBase())
		return;

	char pSend[15];
	_CPacket.PutBYTE(pSend,0x21);
	_CPacket.PutBYTE(pSend+1,0x02);
	_CPacket.PutDWORD(pSend+2,255000);
	_CPacket.PutWORD(pSend+6,(WORD)dwSundID);
	_CPacket.PutDWORD(pSend+8,pItem.dwItemNum());
	_CPacket.PutBYTE(pSend+12,iItemSlot);
	_CPacket.PutWORD(pSend+13,pItem.iItemN());
	_CPacket.PSend(pSend,15);
}

void RefreshInv()
{
	char pSend[2];
	*(WORD*)pSend = 0x026A;
	_CPacket.PSend(pSend,2);
}

void StoreItem(DWORD dwInnID,int iSourceSlot, int iDestSlot)
{
	CInvSlot pItem = iSourceSlot;

	if(!pItem.GetBase())
		return;

	int iInnPage,iInnSlot;

	_CBag.SlotToPaged(iDestSlot,iInnPage,iInnSlot);

	char pSend[15];
	_CPacket.PutBYTE(pSend,0x45);
	_CPacket.PutBYTE(pSend+1,0x02);
	_CPacket.PutWORD(pSend+2,(WORD)dwInnID);
	_CPacket.PutDWORD(pSend+4,pItem.dwItemNum());
	_CPacket.PutBYTE(pSend+8,iInnPage);
	_CPacket.PutBYTE(pSend+9,iSourceSlot);
	_CPacket.PutBYTE(pSend+10,iInnSlot);
	_CPacket.PutDWORD(pSend+11,pItem.iItemN());
	_CPacket.PSend(pSend,15);
}

void OpenInn(DWORD dwInnID)
{
	char pSend[8];
	_CPacket.PutBYTE(pSend,0x45);
	_CPacket.PutBYTE(pSend+1,0x01);
	_CPacket.PutWORD(pSend+2,(WORD)dwInnID);
	_CPacket.PutDWORD(pSend+4,0xFFFFFFFF);
	_CPacket.PSend(pSend,8);
}

bool CheckInn()
{
	return (bool)(*(BYTE*)(*(DWORD*)(*(DWORD*)(KO_DLGA) + OFF_WAREHOUSE) + 0xC0));
}

void __fastcall TdlgMaden::tmrprTimer(TObject *Sender)
{
	if(bOnSundWay || bOnMineWay)
		return;

	if(!frmBot->bBotState)
		return;

	if(frmBot->bBotPaused)
		return;

	if(CheckLoading())
		return;

	if(!_CCharBase.GetDWORD(KO_CRHP))
		return;

	bool bSundGo = false;
	CInvSlot pLItem;	pLItem.SelectLeft();
	CInvSlot pRItem;	pRItem.SelectRight();

	if((pLItem.GetBase() && frmBot->iLastDurabilityL != pLItem.iItemDur() &&  pLItem.iItemDur() <= 500) || (pRItem.GetBase() && frmBot->iLastDurabilityR != pRItem.iItemDur() && pRItem.iItemDur() <= 500))
		bSundGo = true;

	if(bSundGo)
	{
		bOnSundWay = true;
		frmBot->bBotPaused = true;
		iCurrentAction = 1; //going
		bTowned = false;
		bTownOK = false;
		iLastPos = 0;
		tmsundgo->Enabled = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TdlgMaden::tmsundgoTimer(TObject *Sender)
{

	float fMyX = _CCharBase.GetFLOAT(KO_POSX);
	float fMyY = _CCharBase.GetFLOAT(KO_POSY);
	int iZone = _CCharBase.GetDWORD(KO_ZONE);

	if(iCurrentAction == 1)	//going
	{
		if( !bTowned )
		{
			bTowned = true;

			if( _CMath.fDist(fMyX,fMyY,iTownToInnX[0],iTownToInnY[0]) >= 5.0 )
				Town();
		}

		if( !bTownOK && _CMath.fDist(fMyX,fMyY,iTownToInnX[0],iTownToInnY[0]) >= 5.0 )
			return;

		bTownOK = true;

		while( !iTownToInnX[iLastPos] || !iTownToInnY[iLastPos] )
			iLastPos++;

		if(Move(iTownToInnX[iLastPos],iTownToInnY[iLastPos],5))
		{
			iLastPos++;

			if(iLastPos == 3)	//sundries
			{
				iCurrentAction = 2;	//return
				CCharBase pSund;
				if(iZone == 21)
					pSund = _CArea.GetNPCByName(1,0,"[Sundries]Zarta",false,false,false,5.0,false);
				else if(iZone == 1)
					pSund = _CArea.GetNPCByName(1,0,"[Sundries]Veron",false,false,false,5.0,false);
				else if(iZone == 2)
					pSund = _CArea.GetNPCByName(1,0,"[Sundries]Yasval",false,false,false,5.0,false);

				if( !pSund.GetBase() )
					return;

				RepairLeft(pSund.GetID());
				RepairRight(pSund.GetID());
				return;
			}
		}

	}
	else if(iCurrentAction == 2)	//return
	{
		iCurrentAction = 1;
		bTowned = false;
		bTownOK = false;
		frmBot->bBotPaused = false;
		tmsundgo->Enabled = false;
		return;
    }
}
//---------------------------------------------------------------------------

void RepairLeft(DWORD dwSundID)
{
	CInvSlot pLeft;
	pLeft.SelectLeft();

	if(!pLeft.GetBase())
	{
		frmBot->iLastDurabilityL = -1;
		return;
	}

	char pSend[9];
	_CPacket.PutBYTE(pSend,0x3B);
	_CPacket.PutBYTE(pSend+1,0x01);
	_CPacket.PutBYTE(pSend+2,0x08);
	_CPacket.PutWORD(pSend+3,(WORD)dwSundID);
	_CPacket.PutDWORD(pSend+5,pLeft.dwItemNum());
	_CPacket.PSend(pSend,9);

	frmBot->iLastDurabilityL = pLeft.iItemDur();
	pLeft.SetDurability(4000);
}

void RepairRight(DWORD dwSundID)
{
	CInvSlot pRight;
	pRight.SelectRight();

	if(!pRight.GetBase())
	{
		frmBot->iLastDurabilityR = -1;
		return;
	}

	char pSend[9];
	_CPacket.PutBYTE(pSend,0x3B);
	_CPacket.PutBYTE(pSend+1,0x01);
	_CPacket.PutBYTE(pSend+2,0x06);
	_CPacket.PutWORD(pSend+3,(WORD)dwSundID);
	_CPacket.PutDWORD(pSend+5,pRight.dwItemNum());
	_CPacket.PSend(pSend,9);

	frmBot->iLastDurabilityR = pRight.iItemDur();
	pRight.SetDurability(4000);
}

void SaveMineSettings()
{
	//16.07.2011 antalya-kemer * 16:36 * melih :D

	char pFile[64]; memset(pFile,0,64);

	strcpy(pFile,_CCharBase.GetName());
	strcat(pFile,"_maden.kojd");

	DeleteFileA(pFile);	//clear

	ofstream fSave;

	fSave.open(pFile,std::ios_base::out | std::ios_base::app);
	if(!fSave.is_open()){
		MessageBox(frmBot->Handle,"Kayit basarisiz! (Dosya olusturulamadi)","hata!",MB_ICONEXCLAMATION);
		return;
	}

	char tmp[16];	memset(tmp,0,16);

	itoa(dlgMaden->bEnabled,tmp,10);
	fSave << tmp << "\n";

	for (int i = 0; i < 33; i++)
	{
		itoa(dlgMaden->iItemActionList[i],tmp,10);
		fSave << tmp << "\n";
	}

	fSave.close();
}

void LoadMineSettings()
{
	//17.07.2011 antalya-kemer * 02:44 * melih :D

	char pFile[64]; memset(pFile,0,64);

	strcpy(pFile,_CCharBase.GetName());
	strcat(pFile,"_maden.kojd");

	ifstream fLoad;

	fLoad.open(pFile);
	if(!fLoad.is_open())	//
		return;

	bool bRet;
	char Buffer[129];	memset(Buffer,0,129);
	char tmp[16];		memset(tmp,0,16);

	//load settings:

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	dlgMaden->chmaden->Checked = (bool)atoi(Buffer);

	for (int i = 0; i < 33; i++)
	{
		bRet = fLoad.getline(Buffer,128);
		if(bRet == false || strlen(Buffer) == 0)	return;
		dlgMaden->iItemActionList[i] = atoi(Buffer);
	}

	fLoad.close();

	InitActionList();
}

void InitActionList()
{
	for (int i = 0; i < 33; i++)
	{
		ClickAction( dlgMaden->iItemActionList[i], i );
	}
}

void RemoveItem(int iItemSlot)
{
	CInvSlot pItem = iItemSlot;

	if(!pItem.GetBase())
		return;

	char pSend[7];
	*(WORD*)pSend = 0x23F;
	*(BYTE*)(pSend + 2) = (BYTE)iItemSlot;
	*(DWORD*)(pSend + 3) = pItem.dwItemNum();
	_CPacket.PSend(pSend,7);
}

void RunNPC(DWORD dwNpcID)
{
	char pSend[8];
	*(WORD*)pSend = 0x120;
	*(WORD*)(pSend + 2) = (WORD)dwNpcID;
	*(DWORD*)(pSend + 4) = 0xFFFFFFFF;
	_CPacket.PSend(pSend,8);
}
void __fastcall TdlgMaden::btnabyssClick(TObject *Sender)
{
	DWORD dwID = _CCharBase.GetTargetID();

	if(dwID == 0xFFFF)
	{
		MessageBox(this->Handle,"Kýrdýrmak için NPC'nin yanýna gidip NPC'yi seçmelisiniz.","Kýrdýr",MB_ICONINFORMATION);
		return;
	}

	CCharBase pNPC = _CArea.GetNPCBase((short)dwID);
	if(!pNPC.GetBase())
		return;

	if( strstr(pNPC.GetName(),"Operator") == 0)
	{
		MessageBox(this->Handle,"Yanlýþ NPC !!","Kýrdýr",MB_ICONEXCLAMATION);
		return;
	}

	if( _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pNPC.GetFLOAT(KO_POSX),pNPC.GetFLOAT(KO_POSY)) >= 2.0)
	{
		MessageBox(this->Handle,"NPC'ye biraz daha yakýnlaþýn :)","Kýrdýr",MB_ICONINFORMATION);
		return;
	}

	RunNPC(dwID);

	long lTick = GetTickCount();

	BYTE pAbyss1[] = {0x64,0x07,0x25,0x10,0x00,0x00};
	BYTE pAbyss2[] = {0x55,0x00,0x0F,0x31,0x36,0x30,0x34,0x37,0x5F,0x4D,0x6F,0x69,0x72,0x61,0x2E,0x6C,0x75,0x61,0xFF};

	while(1)
	{
		//if( GetTickCount() - lTick >= 5000 )
			//break;

		if( _CBag.FindFreeInvSlot(0) == -1)
			break;

		if( _CBag.FindInvItem(0,ITEM_ABYSS) == -1)
			break;

		_CPacket.PSend(pAbyss1,sizeof(pAbyss1));
		_CPacket.PSend(pAbyss2,sizeof(pAbyss2));
		Sleep(100);
	}
}
//---------------------------------------------------------------------------

void __fastcall TdlgMaden::btnblueClick(TObject *Sender)
{
	DWORD dwID = _CCharBase.GetTargetID();

	if(dwID == 0xFFFF)
	{
		MessageBox(this->Handle,"Kýrdýrmak için NPC'nin yanýna gidip NPC'yi seçmelisiniz.","Kýrdýr",MB_ICONINFORMATION);
		return;
	}

	CCharBase pNPC = _CArea.GetNPCBase((short)dwID);
	if(!pNPC.GetBase())
		return;

	if( strstr(pNPC.GetName(),"Operator") == 0)
	{
		MessageBox(this->Handle,"Yanlýþ NPC !!","Kýrdýr",MB_ICONEXCLAMATION);
		return;
	}

	if( _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pNPC.GetFLOAT(KO_POSX),pNPC.GetFLOAT(KO_POSY)) >= 2.0)
	{
		MessageBox(this->Handle,"NPC'ye biraz daha yakýnlaþýn :)","Kýrdýr",MB_ICONINFORMATION);
		return;
	}

	RunNPC(dwID);

	long lTick = GetTickCount();

	BYTE pChest1[] = {0x64,0x07,0xC9,0x0F,0x00,0x00};
	BYTE pChest2[] = {0x55,0x00,0x0F,0x31,0x36,0x30,0x34,0x37,0x5F,0x4D,0x6F,0x69,0x72,0x61,0x2E,0x6C,0x75,0x61,0xFF};

	while(1)
	{
		//if( GetTickCount() - lTick >= 5000 )
			//break;

		if( _CBag.FindFreeInvSlot(0) == -1)
			break;

		if( _CBag.FindInvItem(0,ITEM_BLUE_CHEST) == -1)
			break;

		_CPacket.PSend(pChest1,sizeof(pChest1));
		_CPacket.PSend(pChest2,sizeof(pChest2));
		Sleep(100);
	}
}
//---------------------------------------------------------------------------

void __fastcall TdlgMaden::btngreenClick(TObject *Sender)
{
	DWORD dwID = _CCharBase.GetTargetID();

	if(dwID == 0xFFFF)
	{
		MessageBox(this->Handle,"Kýrdýrmak için NPC'nin yanýna gidip NPC'yi seçmelisiniz.","Kýrdýr",MB_ICONINFORMATION);
		return;
	}

	CCharBase pNPC = _CArea.GetNPCBase((short)dwID);
	if(!pNPC.GetBase())
		return;

	if( strstr(pNPC.GetName(),"Operator") == 0)
	{
		MessageBox(this->Handle,"Yanlýþ NPC !!","Kýrdýr",MB_ICONEXCLAMATION);
		return;
	}

	if( _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pNPC.GetFLOAT(KO_POSX),pNPC.GetFLOAT(KO_POSY)) >= 2.0)
	{
		MessageBox(this->Handle,"NPC'ye biraz daha yakýnlaþýn :)","Kýrdýr",MB_ICONINFORMATION);
		return;
	}

	RunNPC(dwID);

	long lTick = GetTickCount();

	BYTE pChest1[] = {0x64,0x07,0xC9,0x0F,0x00,0x00};
	BYTE pChest2[] = {0x55,0x00,0x0F,0x31,0x36,0x30,0x34,0x37,0x5F,0x4D,0x6F,0x69,0x72,0x61,0x2E,0x6C,0x75,0x61,0xFF};

	while(1)
	{
		//if( GetTickCount() - lTick >= 5000 )
			//break;

		if( _CBag.FindFreeInvSlot(0) == -1)
			break;

		if( _CBag.FindInvItem(0,ITEM_GREEN_CHEST) == -1)
			break;

		_CPacket.PSend(pChest1,sizeof(pChest1));
		_CPacket.PSend(pChest2,sizeof(pChest2));
		Sleep(100);
	}
}
//---------------------------------------------------------------------------

void __fastcall TdlgMaden::btnredClick(TObject *Sender)
{
	DWORD dwID = _CCharBase.GetTargetID();

	if(dwID == 0xFFFF)
	{
		MessageBox(this->Handle,"Kýrdýrmak için NPC'nin yanýna gidip NPC'yi seçmelisiniz.","Kýrdýr",MB_ICONINFORMATION);
		return;
	}

	CCharBase pNPC = _CArea.GetNPCBase((short)dwID);
	if(!pNPC.GetBase())
		return;

	if( strstr(pNPC.GetName(),"Operator") == 0)
	{
		MessageBox(this->Handle,"Yanlýþ NPC !!","Kýrdýr",MB_ICONEXCLAMATION);
		return;
	}

	if( _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pNPC.GetFLOAT(KO_POSX),pNPC.GetFLOAT(KO_POSY)) >= 2.0)
	{
		MessageBox(this->Handle,"NPC'ye biraz daha yakýnlaþýn :)","Kýrdýr",MB_ICONINFORMATION);
		return;
	}

	RunNPC(dwID);

	long lTick = GetTickCount();

	BYTE pChest1[] = {0x64,0x07,0x81,0x26,0x00,0x00};
	BYTE pChest2[] = {0x55,0x00,0x0F,0x31,0x36,0x30,0x34,0x37,0x5F,0x4D,0x6F,0x69,0x72,0x61,0x2E,0x6C,0x75,0x61,0xFF};

	while(1)
	{
		//if( GetTickCount() - lTick >= 5000 )
			//break;

		if( _CBag.FindFreeInvSlot(0) == -1)
			break;

		if( _CBag.FindInvItem(0,ITEM_RED_CHEST) == -1)
			break;

		_CPacket.PSend(pChest1,sizeof(pChest1));
		_CPacket.PSend(pChest2,sizeof(pChest2));
		Sleep(100);
	}
}
//---------------------------------------------------------------------------

void InitMineZone()
{
	int iZone = _CCharBase.GetDWORD(KO_ZONE);

	if( iZone == 21 )
	{
		dlgMaden->iMineX = 640;	dlgMaden->iMineY = 375;
		dlgMaden->iPitmanX = 664;	dlgMaden->iPitmanY = 411;

		dlgMaden->iTownToInnX[0] = 820;	dlgMaden->iTownToInnY[0] = 440;	//Town
		dlgMaden->iTownToInnX[1] = 808;	dlgMaden->iTownToInnY[1] = 528;
		dlgMaden->iTownToInnX[2] = 760;	dlgMaden->iTownToInnY[2] = 523;	//Sundries
		dlgMaden->iTownToInnX[3] = 740;	dlgMaden->iTownToInnY[3] = 573;
		dlgMaden->iTownToInnX[4] = 727;	dlgMaden->iTownToInnY[4] = 613;
		dlgMaden->iTownToInnX[5] = 764;	dlgMaden->iTownToInnY[5] = 616;
		dlgMaden->iTownToInnX[6] = 763;	dlgMaden->iTownToInnY[6] = 649;	//Inn

		dlgMaden->iTownToMineX = 675;	dlgMaden->iTownToMineY = 425;
	}
	else if( iZone == 1 )
	{
		dlgMaden->iMineX = 640;	dlgMaden->iMineY = 1663;
		dlgMaden->iPitmanX = 619;	dlgMaden->iPitmanY = 1652;

		dlgMaden->iTownToInnX[0] = 445;	dlgMaden->iTownToInnY[0] = 1628;	//Town
		dlgMaden->iTownToInnX[1] = 0;		dlgMaden->iTownToInnY[1] = 0;
		dlgMaden->iTownToInnX[2] = 450;	dlgMaden->iTownToInnY[2] = 1687;	//Sundries
		dlgMaden->iTownToInnX[3] = 0;		dlgMaden->iTownToInnY[3] = 0;
		dlgMaden->iTownToInnX[4] = 0;		dlgMaden->iTownToInnY[4] = 0;
		dlgMaden->iTownToInnX[5] = 0;		dlgMaden->iTownToInnY[5] = 0;
		dlgMaden->iTownToInnX[6] = 367;	dlgMaden->iTownToInnY[6] = 1687;	//Inn

		dlgMaden->iTownToMineX = 531;	dlgMaden->iTownToMineY = 1559;
	}
	else if( iZone == 2 )
	{
		dlgMaden->iMineX = 1478;	dlgMaden->iMineY = 403;
		dlgMaden->iPitmanX = 1491;	dlgMaden->iPitmanY = 429;

		dlgMaden->iTownToInnX[0] = 1596;	dlgMaden->iTownToInnY[0] = 415;	//Town
		dlgMaden->iTownToInnX[1] = 0;		dlgMaden->iTownToInnY[1] = 0;
		dlgMaden->iTownToInnX[2] = 1619;	dlgMaden->iTownToInnY[2] = 366;	//Sundries
		dlgMaden->iTownToInnX[3] = 0;		dlgMaden->iTownToInnY[3] = 0;
		dlgMaden->iTownToInnX[4] = 0;		dlgMaden->iTownToInnY[4] = 0;
		dlgMaden->iTownToInnX[5] = 0;		dlgMaden->iTownToInnY[5] = 0;
		dlgMaden->iTownToInnX[6] = 1687;	dlgMaden->iTownToInnY[6] = 370;	//Inn

		dlgMaden->iTownToMineX = 1542;	dlgMaden->iTownToMineY = 481;
	}
	else
	{
		StopBot();
		MessageBox(dlgMaden->Handle,"Bilinmeyen maden bolgesi!","Hata",MB_ICONEXCLAMATION);
		return;
    }
}

void __fastcall TdlgMaden::chmadenshClick(TObject *Sender)
{
	bSH = chmadensh->Checked;
	if(!bSH)
	{
		frmBot->chwh->Checked = true;
		frmBot->chwhClick(NULL);
	}
}
//---------------------------------------------------------------------------

bool Move(float fToX, float fToY, int iBoosting)
{
	if(dlgMaden->bSH)
		return SHTeleport(fToX,fToY,iBoosting);
	else
		return MoveToPos(fToX,fToY);
}

