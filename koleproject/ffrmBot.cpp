//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ffrmBot.h"

CSocket CMainSocket;

char strTrialID[22];
bool bHookPatched = false;

char strCheckFile[256];
bool bAutoLogin = false;
bool bReLogin = false;
char strGameID[25];
char strGamePW[25];
char strBotID[25];
char strBotPW[25];
int iCharSlot = 1;
int iServerIndex = 0;
int iServerNo = 0;
bool bTrialLogin = false;

BYTE bDisabledRecvByte = 0x00;	//not disabled

DWORD dwW7APRTest_Old = 0;
bool bTPTPatched = false;
bool bTPTPatched2 = false;

bool bOwnCallHandleExA = false;

HANDLE 	hDLLThread = 0;
DWORD	dwDLLThread = 0;

DWORD dwTPTOffset = 0;

bool bLoadSettings = true;
bool bGameLoaded = false;
bool bInterClient = false;

HWND hGameHWND = 0;

char* strServerIP 	= 	"77.95.226.9";
char* strWebAddress	=	"http://onlinehile.org/kojdbot/main.html";

char* strDNS1 = "ksrv1.kojdbot.biz";
char* strDNS2 = "ksrv2.kojdbot.biz";
char* strDNS3 = "ksrv3.kojdbot.biz";
char* strDNS4 = "ksrv4.kojdbot.biz";
char* strDNS5 = "ksrv5.kojdbot.biz";
char* strDNS6 = "ksrv6.kojdbot.biz";

BYTE bPacketKey 	= 	0;
int iRemaining 		= 	0;
int iMyUserIndex	=	-1;
int iMyUserID		=	-1;
int iMyLogOutKey  	=	0;
int iAttackType		=	0;	//none
BYTE bPacketKeyOld	=	0;
bool bUseOldKey		=	false;

DWORD dwLastThreadID = 0;

CRITICAL_SECTION csSendFunc;
CRITICAL_SECTION csAPR;
CRITICAL_SECTION csAPR2;

DWORD 	dwLoginFuncPointer 	= 	(DWORD)&QuitBot;
DWORD 	dwTPTPatchPointer 	= 	(DWORD)&QuitBot;
DWORD 	dwSendPointer 		=  	(DWORD)&SendFunction / 10;
DWORD 	dwSendPointerAdd 	= 	-1;
bool 	bSendOwnCall 		=  	false;
DWORD	dwTerminatePointer	=	(DWORD)&TerminateProcess;
DWORD	dwDLLCheckPointer[3]=	{(DWORD)&QuitBot,(DWORD)&QuitBot,(DWORD)&QuitBot};
DWORD	dwDLLCheckFunc		=	(DWORD)&CheckDLLFile;
int		iDLLCheckFuncIndex	=	-1;
DWORD	dwNTTerminateFunc	=	0;
HMODULE	hModuleHandle		=	0;
int 	iICPartyPass		=	0;
bool	bICPartyFirstUser	=	false;
unsigned long 	ulHDSerialNo=	(DWORD)&SendFunction / 10;
unsigned long 	ulRealHDSerialNo = -1;
DWORD dwServerARG = (DWORD)&SendFunction / 10;
DWORD dwDebuggerFree = 0xFFFFFFFE;
int iDebuggerCheck = 0;
DWORD dwTPTThreadIDAddr = 0;
DWORD	dwSysCallAddrTmp = 0;
DWORD	dwSysCallAddrTmp2 = 0;
DWORD	dwVCTReturn = 0;
DWORD	dwSysCallFnc = 0;
DWORD	dwTmpAPRBase = 0;
DWORD 	dwVirtualCTPtr = (DWORD) &VirtualCreateThread;
DWORD 	dwSysCallRet = 0;
DWORD 	dwSysCallCode = 0;
DWORD 	dwSysCallRet2 = 0;
DWORD 	dwSysCallCode2 = 0;
DWORD 	dwVirtualSysCallPtr = (DWORD) &VirtualSysCall;
DWORD 	dwVirtualSysCallPtr2 = (DWORD) &VirtualSysCall2;
DWORD	dwRealSysCallPtr = 0;
DWORD	dwRealSysCallPtr2 = 0;
bool	bFirstSysCall = true;

DWORD	dwAPIAddr[9] = {0,0,0,0,0,0,0,0,0};
DWORD	dwAPIVal[9] = {0,0,0,0,0,0,0,0,0};

DWORD	dwFuncAddr[20] = {(DWORD)&SendFunction, (DWORD)&CheckFunction, (DWORD)&CheckDebugger, (DWORD)&GetAPIChecks, (DWORD)&GetServerARG, (DWORD)&GetServerARGSub, (DWORD)&GetServerARGSub1, (DWORD)&GetServerARGSub2, (DWORD)&GetServerARGSub3, (DWORD)&GetServerARGSub4, (DWORD)&GetServerARGSub5, (DWORD)&GetServerARGSub6, (DWORD)&CheckLoader, (DWORD)&CheckModule, (DWORD)&CheckDLLName, (DWORD)&CheckDLLFile, (DWORD)&MakeTerminateFunc, (DWORD)&CheckRemoteSend, (DWORD)&Make0x54Byte, (DWORD)&GetFuncChecks};
DWORD	dwFuncVal[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int		iAPIOrFuncCheck = 0;

DWORD	dwCheckFunctionVal = 0;
DWORD	dwCheckFuncRet = 0;

DWORD	dwMyPID = 0;

DWORD	dwThreadID_Addition		=	0;
DWORD	dwThreadID_CZ			=	0;
DWORD	dwThreadID_IC			=	0;
DWORD	dwThreadID_Maden		=	0;
DWORD	dwThreadID_Party		=	0;
DWORD	dwThreadID_Login		=	0;
DWORD	dwThreadID_Bot			=	0;

DWORD dwMyID = -1;
DWORD dwRecvdSockHandle = -1;
BYTE pBuf_BuffSelf1[24] = {0x00,0x01,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
BYTE pBuf_BuffSelf2[24] = {0x00,0x03,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
BYTE pBuf_RemoveSelf[24] = {0x00,0x06,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

BYTE	pBuf_Attack1[24] = {0x00,0x01,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
BYTE	pBuf_Attack2[22] = {0x00,0x02,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
BYTE	pBuf_Attack3[24] = {0x00,0x03,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
BYTE	pBuf_Attack4[24] = {0x00,0x04,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x9B,0xFF,0x00,0x00,0x00,0x00,0x00,0x00};

BYTE	pBuf_FakeSkill[24] = {0x31,0x01,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

BYTE pBuf_RAttack[10] = {0x08,0x01,0x01,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00};

std::list<_CHEST*> 					lstChestList;
std::list<_CHEST*>::iterator		itChest;

std::list<_DROP*> 					lstDropList;
std::list<_DROP*>::iterator			itDrop;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmBot *frmBot;
//---------------------------------------------------------------------------
__fastcall TfrmBot::TfrmBot(TComponent* Owner)
	: TForm(Owner)
{
	bBotStatus			=	false;

	iServerCheckCounter	=	0;
	iPutKeyCounter		=	0;
	iCheckTick			=	0;

	iSHBoost			=	5;
	iSHDelay			=	50;
	iSkillIndex			=	0;
	bBonus3_1			=	false;
	bBonus5_1			=	false;
	bBonus3				=	false;
	bBonus5				=	false;
	iMobCount			=	0;
	iBonusDistance		=	3;
	iTargetingType		=	0;
	iAttackDelay		=	1300;
	bShowEffect		  	=	false;
	bShowMob			=	false;
	bWaitForKill		=	false;
	bMinor				=	false;
	bMana				=	false;
	bHP					=	false;
	dwManaSkillNum		=	SKILL_MP120;
	dwManaItemNum		=	ITEM_MP120;
	dwHPSkillNum		=	SKILL_HP45;
	dwHPItemNum			=	ITEM_HP45;
	iManaType			=	0;
	iManaLimit			=	40;
	iHPType				=	0;
	iHPLimit			=	60;
	bWolf				=	false;
	bTS					=	false;
	bSprint				=	false;
	bHide				=	false;
	bScaled				=	false;
	bCure				=	false;
	bSW					=	false;
	bEvade				=	false;
	bLup				=	false;
	bLF					=	false;
	bSafety				=	false;
	bCats				=	false;
	bMoving				=	false;
	bMovePaused			=	false;
	iMoveCount			=	0;
	iCurrentRoute		=	1;
	iLastPos			=	0;
	iChatDelay			=	1000;
	iChatType			=   0;
	bLoot				=	false;
	bLootPaused			=	false;
	bLootCoins			=	false;
	bMoveToChest 		=	false;
	iMaxChestDistance	=	10;
	iCenterX			=	0;
	iCenterY			=	0;
	bLootUnique		  	=	false;
	bLootUpgrade		=	false;
	bLootPriceLimit		=	false;
	iLootUpgradeLimit	=	7;
	iLootPriceLimit		=	10000;
	bLootList			=	false;
	iLootCount			=	0;
	bSund				=	false;
	bPot				=	false;
	bInn				=	false;
	bBuyWolf			=	false;
	bBuyArrow			=	false;
	bBuyMana			=	false;
	iBuyManaLimit		=	200;
	bBuyHP				=	false;
	iBuyHPLimit			=	200;
	iInnBeginSlot		=	5;
	bInnDepot			=	false;
	bFollowLeader		=	false;
	bFollowSelected		=	false;
	bAutoParty			=	false;
	bAutoInvite			=	false;
	bTownOnGM			=	false;
	bTownOnMob			=	false;
	bTownOnNoTS			=	false;
	bTownOnNoArrow		=	false;
	bTownOnNoMana		=	false;
	bTownOnNoParty		=	false;
	bRingOnDie			=	false;
	bRingOnTown			=	false;
	bInformAfterSupply	=	false;

	iLastMinorTick		=	0;
	iLastManaTick		=	0;
	iLastHPTick			=	0;
	iLastWolfTick		=	0;
	iLastTSTick			=	0;
	iLastSprintTick		=	0;
	iLastHideTick		=	0;
	iLastScaledTick		=	0;
	iLastCureTick		=	0;
	iLastSWTick			=	0;
	iLastEvadeTick		=	0;
	iLastLupTick		=	0;
	iLastLFTick			=	0;
	iLastSafetyTick		=	0;
	iLastCatsTick		=	0;
	iLastMage62Tick		=	0;

	iLastAttackTick		=	0;

	wLastTarget			=	0;

	dwCastNum			=	0;
	dwCastNum_3			=	0;
	dwCastNum_5			=	0;

	bWaitingNewDrop		=	false;

	dwLastChestID		=	0;
	fLastChestDist		=	0;

	iAlertMobCount		=	0;

	mciAlarm			=	0;

	bOnSundWay			=	false;
	bOnPotWay		  	=	false;
	bOnInnWay			=	false;

	iSundCount			=	0;
	iPotCount			=	0;
	iInnCount			=	0;

	iSundLastPos		=	0;
	iSundRoute			=	0;
	iPotLastPos			=	0;
	iPotRoute			=	0;
	iInnLastPos			=	0;
	iInnRoute			=	0;

	bBotPaused			=	false;

	bMinimized			=	false;

	bChatTP				=	false;
	bLimitTP		 	=	false;
	iTPLimit			=	25;
	bAutoGate			=	false;
	iGateLimit			=	25;

	iLastTPTick			=	0;
	iLastGateTick	  	=	0;

	bAntiFreeze			=	false;

	bMage62				=	false;

	dwCastNum18			=	0;
	dwCastNum33			=	0;
	dwCastNum43			=	0;
	dwCastNum45			=	0;
	dwCastNum51			=	0;
	dwCastNum54			=	0;
	dwCastNum56			=	0;
	dwCastNum57			=	0;
	dwCastNum60			=	0;
	dwCastNum70			=	0;

	iSkillType_18		=	1;
	iSkillType_33		=	1;
	iSkillType_43		=	1;
	iSkillType_45		=	1;
	iSkillType_51		=	1;
	iSkillType_54		=	1;
	iSkillType_56		=	1;
	iSkillType_57		=	1;
	iSkillType_60		=	1;
	iSkillType_70		=	1;

	bCast18				=	false;
	bCast33				=	false;
	bCast45				=	false;
	bCast51				=	false;
	bCast54				=	false;
	bCast57				=	false;
	bCast60				=	false;
	bCast70				=	false;

	bMageCloseAttack	=	false;
	iSkillIndex_CloseAttack		=	1;

	iLastCastTick_18	=	0;
	iLastCastTick_33	=	0;
	iLastCastTick_45	=	0;
	iLastCastTick_51	=	0;
	iLastCastTick_54	=	0;
	iLastCastTick_57	=	0;
	iLastCastTick_60	=	0;
	iLastCastTick_70	=	0;

	lMageCastData18	=	0;
	lMageCastData33	=	0;
	lMageCastData43	=	0;
	lMageCastData45	=	0;
	lMageCastData51	=	0;
	lMageCastData54	=	0;
	lMageCastData56	=	0;
	lMageCastData57	=	0;
	lMageCastData60	=	0;
	lMageCastData62 =	0;
	lMageCastData70	=	0;

	bOnlyEnemy	   	=	false;
	bOnlyTarget		=	false;

	iLastDurabilityL	=	-1;
	iLastDurabilityR	=	-1;

	bFormLoaded		=	false;

	iLastChestTick	=	0;

	for (int i = 0; i < 8; i++) {
		bPriestMembers[i] = true;
	}
	bPriestHeal			=	false;
	iHealType			=	1;
	iHealCastIndex		=	0;
	iHealLimit			=	50;
	bHealLessHPFirst  	=	true;
	bBuff				=	false;
	bAC					=	false;
	bResist				=	false;
	iBuffCastIndex		=	0;
	iACCastIndex		=	0;
	iResistCastIndex	=	0;
	bGroupHeal			=	false;
	bRestore			=	false;
	iGroupCastIndex		=	0;
	iRestoreCastIndex	=	0;
	bMalice				=	false;
	bParasite			=	false;
	bTorment			=	false;
	bMassive			=	false;
	bChatHeal			=	false;
	bChatBuff			=	false;
	bChatAC				=	false;
	bChatResist			=	false;
	bSitWhenIdle		=	false;
	bPriAutoCure		=	false;
	bPriAutoSTR			=	false;
	bPriAutoBook		=	false;
	bPriestPaused		=	false;

	iLastHealTick		=	0;
	iLastBuffTick		=	0;
	iLastACTick			=	0;
	iLastResistTick		=	0;
	iLastGroupTick		=	0;
	iLastRestoreTick	=	0;
	iLastMaliceTick		=	0;
	iLastParasiteTick	=	0;
	iLastTormentTick	=	0;
	iLastMassiveTick	=	0;
	iLastPriCureTick	=	0;
	iLastPriDiseaseTick	=	0;
	iLastPriSTRTick		=	0;
	iLastPriBookTick	=	0;

	for (int i = 0; i < 8; i++) {
		iLastUserMaxHP[i] = 0;
		iLastBuffedUserTick[i] = 0;
		bUserWaitingForNewHP[i] = false;
		iLastRestoredUserTick[i] = 0;
	}

	bPriPrintName		=	false;
	bUndyForWarriors	=	false;

	dwPriHealCastNum	=	0;
	dwPriBuffCastNum	=	0;
	dwPriACCastNum		=	0;
	dwPriResistCastNum	=	0;
	dwPriGroupCastNum	=	0;
	dwPriRestoreCastNum	=	0;
	dwPriCureCastNum	=	0;
	dwPriDiseaseCastNum	=	0;
	dwPriSTRCastNum		=	0;
	dwPriUndyCastNum	=	0;

	for (int i = 0; i < 8; i++) {
		bHealCastArray[i] = false;
		dwHealCastArray[i] = 0;
	}

	bRAttack			=	false;
	bRunToMob			=	false;
	iRunDist          	=	10;
	bRock				=	false;
	bAsasPercent5		=	false;
	bAsasPercent10		=	false;
	bBinding			=	false;
	bProvoke			=	false;
	bPriestAttack		=	false;
	iPriestAttackType 	=	1;
	bLightStrike		=	false;
	bLightCounter		=	false;
	bBrightness			=	false;
	bCriticalLight		=	false;
	bLightAttack		=	false;
	bSTR30				=	false;
	bAsasAttack			=	false;
	bDebuff				=	false;

	bSprintWarrior		=	false;
	bDefenseWarrior		=	false;
	bGainWarrior		=	false;

	for (int i = 0; i < 9; i++) {
		bAsasCastArray[i] = false;
		dwAsasCastArray[i] = 0;
		iLastAsasCastTick[i] = 0;
	}

	iLastAsasAttackTick	=	0;
	iLastRockTick	 	=	0;
	wLastRockTarget	=	0;

	iBotCenterX			=	0;
	iBotCenterY			=	0;

	iLastRAttackTick   	=	0;

	dwPercent10Num		=	0;
	dwPercent5Num		=	0;

	iLastPercent10Tick	=	0;
	iLastPercent5Tick	=	0;

	for (int i = 0; i < 18; i++) {
		dwWarriorCastArray[i] = 0;
	}
	iWarriorCastIndex	= 0;
	dwBindingCastNum	= 0;
	dwProvokeCastNum	= 0;
	iLastWarriorAttackTick = 0;
	iLastBindingTick	=	0;
	iLastProvokeTick	=	0;
	wLastBindingTarget	=	0;

	iLastDefenseTick	=	0;
	iLastGainTick		=	0;

	bWarriorAttack	=	false;

	for (int i = 0; i < 8; i++) {
		bPriestCastArray[i]		= false;
		dwPriestCastArray[i] 	= 0;
		iLastPriestCastTick[i]	= 0;
	}

	iPriestCastIndex 		= 0;
	iLastPriestAttackTick 	= 0;
	dwPriestSTR30CastNum	= 0;
	iLastPriestSTR30Tick	= 0;

	for (int i = 0; i < 5; i++) {
		iLastPriestFarCastTick[i] = 0;
		dwPriestFarCastArray[i] = 0;
	}

	for (int i = 0; i < 4; i++) {
		iLastPriestDebuffTick[i] = 0;
		dwPriestDebuffArray[i] = 0;
	}

	iLastPriestDebuffCastTick	= 0;

	iPriestMyLastRestoreTick	= 0;
	iPriestMyLastBuffTick		= 0;
	iPriestMyLastACTick			= 0;
	iPriestMyLastResistTick		= 0;
	iPriestMyLastSTRTick		= 0;

	iPriestHealLockIndex		= -1;
	bMoveChestPaused			=	false;
	bRunMobPaused				=	false;

	bBotState					=	false;
	bArcheryAttack				=	false;

	bAutoSell					=	false;
	bSellSlotLimit				=	true;
	iSellBeginSlot				=	5;
	bSellPriceLimitAbove		=	false;
	iSellPriceLimitAbove	   	=	0;
	bSellPriceLimitLess			=	false;
	iSellPriceLimitLess			=	0;

	bSellWaitForRPR				=	false;

	bRouteSH					=	true;
	bStunChar					=	false;

	iMinorLimit					=	90;

	fLastMovedX					=	0;
	fLastMovedY					=	0;

	bBuyTS			=	false;
	bBuyBook		=	false;
	bBuyRock		=	false;
	iBuyTS			=	3;
	iBuyBook		=	10;
	iBuyRock	  	=	1000;

	for (int i = 0; i < 20; i++) {
		bArcheryCastArray[i] = false;
		dwArcheryCastArray[i] = 0;
		iArcheryCastArray[i] = 0;
	}

	dwCastNum_Blow	=	0;
	bBlow			=	false;

	for (int i = 0; i < 5; i++) {
		bMageBasicCastArray[i] = false;
		dwMageBasicCastArray[i] = 0;
		iMageBasicCastArray[i] = 0;
		iMageBasicDataArray[i] = 0;
	}

	for (int i = 0; i < 21; i++) {
		bMageFlameCastArray[i] = false;
		dwMageFlameCastArray[i] = 0;
		iMageFlameCastArray[i] = 0;
		iMageFlameDataArray[i] = 0;

		bMageIceCastArray[i] = false;
		dwMageIceCastArray[i] = 0;
		iMageIceCastArray[i] = 0;
		iMageIceDataArray[i] = 0;

		bMageLightCastArray[i] = false;
		dwMageLightCastArray[i] = 0;
		iMageLightCastArray[i] = 0;
		iMageLightDataArray[i] = 0;
	}

	bMageAttack = false;

	iWentToChestTime = 0;
	bOnChestWay		=	false;

	iLastArcheryBonusTick	=	0;

	bFarChest	=	false;

	bRingOnDC	=	false;
	bCloseOnDC	=	false;

	bFollowPaused	=	false;

	bBuyMaster		=	false;
	iBuyMaster		=	1000;

	bSuspendTurn	= true;
	hGameThread		= NULL;
	hRecvThread		= NULL;
	dwRecvThread	= NULL;
	bRecvThreadOpened = false;

	iAddCount		=	0;

	for (int i = 0; i < 8; i++) {
		ZeroMemory(&pThreadArray[i],sizeof(THREADENTRY32));
		hThreadArray[i] = NULL;
	}

	hMainMenu	=	NULL;
	iActiveMin	=	0;

	dwSelfWndProc = 0;
	dwGameWndProc = 0;

	dwLastTPTPacketTick = 0;

	bNoTrade	=	false;
	bNoParty	=	false;
	bNoCombat	=	false;

	iSuspendMin = 0;

	for (int i = 0; i < 8; i++) {
		hWindowArray[i] = NULL;
		hPopupArray[i] = NULL;
	}

	bGetInnHP	=	false;
	bGetInnMP	=	false;
	iGetInnHP	=	200;
	iGetInnMP	=	200;

	bSaveCPU	=	false;
	iSleepRatio =	50;
	lRecvLastSleep	=	0;

	bLegalAttack	=	false;
	bPartySW		=	false;

	iLastPTSWTick	=	0;
	iLastBonus5Tick	=	0;

	bAttackWithLeader	=	false;

	bKSMod				=	false;
	bAutoMeat			=	false;
	iLastMeatTick		=	0;

	bStickSH			=	false;

	iSubTargetingType	=	1;
	iTargetMobIDCount	=	0;
	iTargetNickCount	=	0;

	bAttackLeader		=	false;
}
//---------------------------------------------------------------------------

BYTE HexToByte(char* strHex){
	long lRet = 0;
	sscanf(strHex,"%2X",&lRet);
	return (BYTE)lRet;
}

short HexToShort(char* strHex){
	long lRet = 0;
	sscanf(strHex,"%4X",&lRet);
	return (short)lRet;
}

long HexToLong(char* strHex){
	long lRet = 0;
	sscanf(strHex,"%8X",&lRet);
	return lRet;
}
//---------------------------------------------------------------------------


void __fastcall TfrmBot::FormShow(TObject *Sender)
{
	if(bFormLoaded)	// sadece baslangicta
		return;

	if(iTotalDecryption < iRecvSizeLogin)	//login recv size
	{
    	LogError(16);
		__asm
		{
			PUSH 0xFFFFFFFF
			POP dwRecvdSockHandle
			XOR EAX,EAX
			SUB ESP,8
			XCHG [ESP+4],EAX
			OR EAX,0xFFFFFFFF
			XCHG [ESP],EAX
			CALL dwTerminatePointer
		}
	}

	switch(_CCharBase.GetDWORD(KO_CLSS)){
		case 102:
		case 107:
		case 108:
		case 202:
		case 207:
		case 208:
		iAttackType = 1;	//rogue
		break;

		case 103:
		case 109:
		case 110:
		case 203:
		case 209:
		case 210:
		iAttackType = 2;	//mage
		break;

		case 104:
		case 111:
		case 112:
		case 204:
		case 211:
		case 212:
		iAttackType = 3;	//priest
		break;

		case 101:
		case 105:
		case 106:
		case 201:
		case 205:
		case 206:
		iAttackType = 4;	//warrior
		break;
	}

	if(!iAttackType){
		TerminateGame(3);
		MessageBox(this->Handle,"Char class initialization failed. (m_iAttackType=0)","error",MB_ICONEXCLAMATION);
		CloseBot(false);
		QuitBot();
	}

	*(BYTE*)(pBuf_BuffSelf1) = bPacketKey;
	*(BYTE*)(pBuf_BuffSelf2) = bPacketKey;
	*(BYTE*)(pBuf_RemoveSelf) = bPacketKey;
	*(BYTE*)(pBuf_Attack1) = bPacketKey;
	*(BYTE*)(pBuf_Attack2) = bPacketKey;
	*(BYTE*)(pBuf_Attack3) = bPacketKey;
	*(BYTE*)(pBuf_Attack4) = bPacketKey;

	__asm{NOT dwMyID};
	__asm{AND dwMyID,0xFFFF};

	*(WORD*)(pBuf_BuffSelf1 + 6) = dwMyID;
	*(WORD*)(pBuf_BuffSelf1 + 8) = dwMyID;
	*(WORD*)(pBuf_BuffSelf2 + 6) = dwMyID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = dwMyID;

	*(WORD*)(pBuf_RemoveSelf + 6) = dwMyID;
	*(WORD*)(pBuf_RemoveSelf + 8) = dwMyID;

	*(WORD*)(pBuf_Attack1 + 6) = dwMyID;
	*(WORD*)(pBuf_Attack2 + 6) = dwMyID;
	*(WORD*)(pBuf_Attack3 + 6) = dwMyID;
	*(WORD*)(pBuf_Attack4 + 6) = dwMyID;

	dwCastNum 	= _CCast.MakeCastNum(_CCharBase,"003");
	dwCastNum_3 = _CCast.MakeCastNum(_CCharBase,"515");
	dwCastNum_5 = _CCast.MakeCastNum(_CCharBase,"555");

	if(iAttackType != 1){	//mage-warrior-priest
		txtatakdelay->Text = "1050";
		iAttackDelay = 1050;

		if(iAttackType == 2){	//mage
			DWORD dwTmpSkillNum = _CCast.MakeCastNum(_CCharBase,"802");
			DWORD dwTmpSkillBase = _CTest.GetSkillBase(dwTmpSkillNum);
			if(dwTmpSkillBase)
				lMageCastData62 = *(DWORD*)(dwTmpSkillBase + 0x78);
		}
	}

	char caption[64];
	memset(caption,0,64);

	strcpy(caption,"[");
	strcat(caption,_CCharBase.GetName());
	strcat(caption,"] ");

	this->Caption = caption;

	memset(caption,0,64);
	strcpy(caption,_CCharBase.GetName());
	strcat(caption,".kojd");
	txtloadsavefile->Text = caption;

	memset(caption,0,64);
	strcpy(caption,_CCharBase.GetName());
	strcat(caption,"_sund.kojd");
	txtsundsave->Text = caption;

	memset(caption,0,64);
	strcpy(caption,_CCharBase.GetName());
	strcat(caption,"_banka.kojd");
	txtinnsave->Text = caption;

	memset(caption,0,64);
	strcpy(caption,_CCharBase.GetName());
	strcat(caption,"_pot.kojd");
	txtpotsave->Text = caption;

	ClearDrops();
	txtreport->Text = "";

	dlgParty = new TdlgParty(NULL);
	Application->CreateForm(__classid(TdlgParty), &dlgParty);
	dlgParty->Hide();

	dlgAddition = new TdlgAddition(NULL);
	Application->CreateForm(__classid(TdlgAddition), &dlgAddition);
	dlgAddition->Hide();

	dlgCZ = new TdlgCZ(NULL);
	Application->CreateForm(__classid(TdlgCZ), &dlgCZ);
	dlgCZ->Hide();

	dlgMaden = new TdlgMaden(NULL);
	Application->CreateForm(__classid(TdlgMaden), &dlgMaden);
	dlgMaden->Hide();

	dlgInterClient = new TdlgInterClient(NULL);
	Application->CreateForm(__classid(TdlgInterClient), &dlgInterClient);
	dlgInterClient->Hide();

	dwThreadID_Addition		=	GetWindowThreadProcessId( dlgAddition->Handle,		0 );
	dwThreadID_CZ			=	GetWindowThreadProcessId( dlgCZ->Handle,			0 );
	dwThreadID_IC			=	GetWindowThreadProcessId( dlgInterClient->Handle,	0 );
	dwThreadID_Maden		=	GetWindowThreadProcessId( dlgMaden->Handle,			0 );
	dwThreadID_Party		=	GetWindowThreadProcessId( dlgParty->Handle,			0 );
	dwThreadID_Login		=	GetWindowThreadProcessId( frmLogin->Handle,			0 );
	dwThreadID_Bot			=	GetWindowThreadProcessId( this->Handle,				0 );

	//frmLogin->Close();

	tmservercheck->Enabled 	= true;

	if(bHookPatched == true)
		_CPacket.RepairGameFunction();
	else
		InitializeCriticalSection(&csSendFunc);

	__asm{NOT dwRecvdSockHandle};

	_CRecv.Enable();
	_CPacket.PatchGameFunction();

	*(DWORD*)KO_QUIT = (DWORD)&PostQuitMsg;


	*(DWORD*)KO_APRA = 0;	// APR
	*(DWORD*)KO_APRB = 0;	// APR
	*(DWORD*)KO_APRC = 0;	// APR


	hGameThread = OpenThread(THREAD_SUSPEND_RESUME,false,*(DWORD*)KO_THRD);

	hMainMenu = CreateMenu();
	::SetMenu(hGameHWND,hMainMenu);
	AppendMenu(hMainMenu,MF_POPUP,(int)popup1->Handle,"KojdBot");
	AppendMenu(hMainMenu,MF_DISABLED,NULL,"Online [0 dk]");
	AppendMenu(hMainMenu,MF_POPUP,(int)popup2->Handle,"Eklenti");

	hPopupArray[0] = pclient1->Handle;
	hPopupArray[1] = pclient2->Handle;
	hPopupArray[2] = pclient3->Handle;
	hPopupArray[3] = pclient4->Handle;
	hPopupArray[4] = pclient5->Handle;
	hPopupArray[5] = pclient6->Handle;
	hPopupArray[6] = pclient7->Handle;
	hPopupArray[7] = pclient8->Handle;

	AppendMenu(hMainMenu,MF_DISABLED,NULL,"-");
	AppendMenu(hMainMenu,MF_DISABLED,NULL,"-");
	AppendMenu(hMainMenu,MF_DISABLED,NULL,"-");
	AppendMenu(hMainMenu,MF_DISABLED,NULL,"-");
	AppendMenu(hMainMenu,MF_DISABLED,NULL,"-");
	AppendMenu(hMainMenu,MF_DISABLED,NULL,"-");
	AppendMenu(hMainMenu,MF_DISABLED,NULL,"-");
	AppendMenu(hMainMenu,MF_DISABLED,NULL,"-");

	StoreMenuIDs();

	DrawMenuBar(hGameHWND);

	trayMain->BalloonHint = _CCharBase.GetName();
	trayMain->Hint = _CCharBase.GetName();

	if(bLoadSettings == true)
		LoadSettings();

	dwSelfWndProc = (WNDPROC)GetWindowLongPtr(this->Handle,GWLP_WNDPROC);
	dwGameWndProc = (WNDPROC)GetWindowLongPtr(hGameHWND,GWLP_WNDPROC);

	SetWindowLongPtr(hGameHWND,GWLP_WNDPROC,(DWORD)&VirtualWndProc);

	if(bAutoLogin == true)
	{
		tmgocenter->Enabled = true;
		this->WindowState = wsMinimized;
	}

	if(bReLogin == true)
		tmlogincheck->Enabled = true;

	if( bInterClient )
	{
		ofstream fWindow;
		fWindow.open("C://wndlist.kojd",std::ios_base::out | std::ios_base::app);
		if(fWindow.is_open() == true)
		{
			fWindow << (char*)KO_WNDA << "\n";
			fWindow.close();
		}

		UpdateMenu();
	}

	LoadInit();

	tmcheck->Enabled = true;

	if(iDLLCheckFuncIndex == -1)
	{
    	LogError(2);
		OwnTerminate();

	}

	if(dwTerminatePointer != (DWORD)&TerminateProcess)
	{
		LogError(3);
		OwnTerminate();
	}

	CheckDLLFile();

	if( !dwRecvdSockHandle || (long)dwRecvdSockHandle == -1 )
	{
		LogError(4);
		OwnTerminate();
	}

	bFormLoaded = true;	// loaded
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmservercheckTimer(TObject *Sender)
{

	if(!_CPacket.GetConnState())
		return;

	if(iTotalConnection == 0)
	{
    	LogError(17);
		__asm
		{
			PUSH 0xFFFFFFFF
			POP dwRecvdSockHandle
			XOR EAX,EAX
			SUB ESP,8
			XCHG [ESP+4],EAX
			OR EAX,0xFFFFFFFF
			XCHG [ESP],EAX
			CALL dwTerminatePointer
		}

	}

	DWORD dwCheckFunc = (DWORD) &CheckFunction;
	if( *(DWORD*)dwCheckFunc != dwCheckFunctionVal )
	{
		LogError(5);
		CloseBot(false);
		OwnTerminate();
	}

	static int iSec = 0;
	static int iMin = 0;
	iSec ++;
	if(iSec == 6)
	{
		iMin ++;
		iSec = 0;
		iActiveMin++;
		char *pText = new char[32];
		memset(pText,0,32);
		strcpy(pText,"Online [");
		char tmp[16]; memset(tmp,0,16);
		itoa(iActiveMin,tmp,10);
		strcat(pText,tmp);
		strcat(pText," dk]");
		ModifyMenu(hMainMenu,1,MF_BYPOSITION | MF_DISABLED,NULL,pText);
		DrawMenuBar(hGameHWND);
		delete [] pText;

		//iSuspendMin++;
	}

	if(iMin == 3)
	{
		//SendTPTPacket();
		iMin = 0;
	}

	if(bInterClient)
		UpdateMenu();

	iServerCheckCounter++;
	iPutKeyCounter++;

	if(iServerCheckCounter == iServerCheckInterval){
		CheckServer();
		iServerCheckCounter = 0;
		bPacketKey = 0x00;
	}

	if(iPutKeyCounter == iPutKeyInterval){
		iPutKeyCounter = 0;

		if(bUseOldKey)
			bPacketKey = bPacketKeyOld;

		*(BYTE*)(pBuf_BuffSelf1) = bPacketKey;
		*(BYTE*)(pBuf_BuffSelf2) = bPacketKey;
		*(BYTE*)(pBuf_Attack1) = bPacketKey;
		*(BYTE*)(pBuf_Attack2) = bPacketKey;
		*(BYTE*)(pBuf_Attack3) = bPacketKey;
		*(BYTE*)(pBuf_Attack4) = bPacketKey;

		*(WORD*)(pBuf_BuffSelf1 + 6) = dwMyID;
		*(WORD*)(pBuf_BuffSelf1 + 8) = dwMyID;
		*(WORD*)(pBuf_BuffSelf2 + 6) = dwMyID;
		*(WORD*)(pBuf_BuffSelf2 + 8) = dwMyID;

		*(WORD*)(pBuf_Attack1 + 6) = dwMyID;
		*(WORD*)(pBuf_Attack2 + 6) = dwMyID;
		*(WORD*)(pBuf_Attack3 + 6) = dwMyID;
		*(WORD*)(pBuf_Attack4 + 6) = dwMyID;

		if(!bPacketKey)
			TerminateGame(5);
	}
}
//---------------------------------------------------------------------------

void CheckServer(){

	CMainSocket = _CWinsock.NewSocket();
	if((long)CMainSocket.GetSocket() == INVALID_SOCKET)
		return;

	//DWORD dwTrue = 1;
	//setsockopt(CMainSocket.GetSocket(),SOL_SOCKET,SO_LINGER,(char*)&dwTrue,4);

	//DWORD dwTrue = 1;
	//ioctlsocket(CMainSocket.GetSocket(), FIONBIO, (u_long FAR*) &dwTrue);

	int iRetry = 0;

	while(1){
		if(CMainSocket.Client_Connect(strServerIP,iServerPort))
			break;

		if(iRetry == 5){
			CMainSocket.Close();
			CloseBot(true);
			TerminateGame(6);
			if(bAutoLogin == false)
				MessageBox(frmBot->Handle,"Bot ile server baglantisi koptu. Lutfen cikip tekrar giris yapin. Server reset'leniyor olabilir. Detayli bilgi icin www.onlinehile.org ","Hata",MB_ICONEXCLAMATION);
			else
				SetAutoLogin(0,1);
			QuitBot();
			return;
		}

		iRetry++;
		Sleep(250);
	}

	char* pSend = new char[iCheckPacketSize];
	memset(pSend,0,iCheckPacketSize);

	int index(0);

	CMainSocket.PutPacketHead(pSend,index);
	CMainSocket.PutBYTE(pSend,bSend_CHECK,index);
	CMainSocket.PutWORD(pSend,(WORD)iMyUserIndex,index);
	CMainSocket.PutWORD(pSend,(WORD)iMyUserID,index);
	CMainSocket.PutPacketTail(pSend,index);

	CMainSocket.Client_Send(pSend,iCheckPacketSize);
	frmBot->iCheckTick = GetTickCount();

	bUseOldKey = false;

	frmBot->tmcheckrecv->Enabled = true;
	delete [] pSend;
}

void __fastcall TfrmBot::tmcheckrecvTimer(TObject *Sender)
{
	char* pRecv = new char[iRecvSizeCheck];
	memset(pRecv,0,iRecvSizeCheck);
	DWORD nRecv(0),nRecvd(0);

	ioctlsocket(CMainSocket.GetSocket(),FIONREAD,&nRecv);
	if(!nRecv)
	{
		delete [] pRecv;
		if(GetTickCount() - iCheckTick >= 8000){
			/*
			CMainSocket.Close();
			tmcheckrecv->Enabled = false;
			CloseBot(true);
			TerminateGame(7);
			if(bAutoLogin == false)
				MessageBox(this->Handle,"Anlik bot guncelleme hatasi! (Cevap yok!)","Hata",MB_ICONEXCLAMATION);
			else
				SetAutoLogin(0,1);
			QuitBot();
			*/
			CMainSocket.Close();
			tmcheckrecv->Enabled = false;
			bUseOldKey = true;
		}
		return;
	}

	if((int)nRecv != iRecvSizeCheck){
		delete [] pRecv;
		CMainSocket.Close();
		tmcheckrecv->Enabled = false;
		CloseBot(true);
		TerminateGame(8);
		if(bAutoLogin == false)
			MessageBox(this->Handle,"Anlik bot guncelleme hatasi! (overflow)","Hata",MB_ICONEXCLAMATION);
		else
			SetAutoLogin(0,1);
		QuitBot();
		return;
	}

	do{
		nRecvd += CMainSocket.Client_Recv(pRecv+nRecvd,nRecv-nRecvd);
	} while(nRecvd < nRecv);

	int iDataHead,iDataTail,iDataSize;
	char* pData;

	iDataHead = CMainSocket.FindPacketHead(pRecv,nRecvd);
	if((DWORD)iDataHead < sizeof(wPACKETHEAD)){
		delete [] pRecv;
		CMainSocket.Close();
		tmcheckrecv->Enabled = false;
		CloseBot(true);
		TerminateGame(8);
		if(bAutoLogin == false)
			MessageBox(this->Handle,"Anlik bot guncelleme hatasi! (invalid packet)","Hata",MB_ICONEXCLAMATION);
		else
			SetAutoLogin(0,1);
		QuitBot();
		return;
	}

	iDataTail = CMainSocket.FindPacketTail(pRecv+iDataHead,nRecvd-iDataHead);
	if((DWORD)iDataTail < 1){
		delete [] pRecv;
		CMainSocket.Close();
		tmcheckrecv->Enabled = false;
		CloseBot(true);
		TerminateGame(8);
		if(bAutoLogin == false)
			MessageBox(this->Handle,"Anlik bot guncelleme hatasi! (invalid packet)","Hata",MB_ICONEXCLAMATION);
		else
			SetAutoLogin(0,1);
		QuitBot();
		return;
	}

	iDataSize = iDataTail;
	pData = pRecv + iDataHead;

	int index = 0;
	BYTE bRecvByte = CMainSocket.GetBYTE(pData,index);

	if(bRecvByte != bRecv_CHECK){
		delete [] pRecv;
		CMainSocket.Close();
		tmcheckrecv->Enabled = false;
		CloseBot(true);
		if(bRecvByte == bRecv_UNKNOWN)
		{
			TerminateGame(12);
			if(bAutoLogin == false)
				MessageBox(this->Handle,"Bilinmeyen cevap. Bu hatayi aldiginizda lutfen bot yetkilileri ile iletisime gecip hatayi iletiniz...","Hata",MB_ICONEXCLAMATION);
			else
				SetAutoLogin(0,1);
		}
		else
		{
			TerminateGame(8);
			if(bAutoLogin == false)
				MessageBox(this->Handle,"Anlik bot guncelleme hatasi! (unknown prefix)","Hata",MB_ICONEXCLAMATION);
			else
				SetAutoLogin(0,1);
		}
		QuitBot();
		return;
	}

	CMainSocket.Close();	//end
	BYTE bPrefix = CMainSocket.GetBYTE(pData,index);
	switch(bPrefix){

		case bCheck_NOLOGIN:
		{
			delete [] pRecv;
			tmcheckrecv->Enabled = false;
			CloseBot(true);
			TerminateGame(9);
			if(bAutoLogin == false)
				MessageBox(this->Handle,"Uyelik girisiniz server'dan dusmus durumdadir. Lutfen cikip tekrar giris yapiniz.\nDikkat: Bot uyeliginize baska bir IP den giris yapilmis olabilir!","Hata",MB_ICONEXCLAMATION);
			else
				SetAutoLogin(0,1);
			QuitBot();
			break;
		}

		case bCheck_SUCCESS:
		{
			bPacketKey = CMainSocket.GetBYTE(pData,index);
			dwMyID = CMainSocket.GetWORD(pData,index);
			__asm{NOT dwMyID};
			__asm{AND dwMyID,0xFFFF};
			dwRecvdSockHandle = CMainSocket.GetDWORD(pData,index);
			__asm{NOT dwRecvdSockHandle}
			tmcheckrecv->Enabled = false;
			break;
		}

		case bCheck_TIMEOUT:
		{
			delete [] pRecv;
			tmcheckrecv->Enabled = false;
			CloseBot(true);
			TerminateGame(11);
			MessageBox(this->Handle,"Bot kullanim sureniz dolmustur!","Hata",MB_ICONEXCLAMATION);
			QuitBot();
			break;
		}

		case bCheck_WRONGIP:
		{
			delete [] pRecv;
			tmcheckrecv->Enabled = false;
			CloseBot(true);
			TerminateGame(10);
			MessageBox(this->Handle,"Anlik bot guncelleme hatasi! (Hatali IP Adresi)\nDikkat: Bot uyeliginize baska bir IP den giris yapilmis olabilir!","Hata",MB_ICONEXCLAMATION);
			QuitBot();
			break;
		}

	}
	delete [] pRecv;
}
//---------------------------------------------------------------------------

void CloseBot(bool bGoTown){

	StopBot();

	if(bGoTown)
	{
		char pSend[1];
		*pSend = 0x48;
		_CPacket.PSend(pSend,1);
	}

	LogOut();
	CMainSocket.Close();
	_CWinsock.CleanWinsock();

	_CPacket.RepairGameFunction();
	_CRecv.Disable();

	DeleteCriticalSection(&csSendFunc);

	//DWORD dwPostQuitMessage = (DWORD)GetProcAddress(GetModuleHandle("user32.dll"),"PostQuitMessage");
	DWORD dwTerminate = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"),"TerminateProcess");

	*(DWORD*)KO_QUIT = dwTerminate;

	DWORD dwAPI1 = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"),"GetModuleFileNameA");
	DWORD dwAPI2 = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"),"GetModuleHandleA");
	DWORD dwAPI3 = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"),"GetProcAddress");
	DWORD dwAPI4 = (DWORD)GetProcAddress(GetModuleHandle("user32.dll"),"GetWindowThreadProcessId");
	DWORD dwAPI5 = (DWORD)GetProcAddress(GetModuleHandle("user32.dll"),"GetWindowTextA");
	DWORD dwAPI6 = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"),"LoadLibraryA");
	DWORD dwAPI7 = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"),"CreateFileA");
	*(DWORD*)KO_API1 = dwAPI1;
	*(DWORD*)KO_API2 = dwAPI2;
	*(DWORD*)KO_API3 = dwAPI3;
	*(DWORD*)KO_API4 = dwAPI4;
	*(DWORD*)KO_API5 = dwAPI5;
	*(DWORD*)KO_API6 = dwAPI6;
	*(DWORD*)KO_API7 = dwAPI7;

	RepairCreateThread();

	//ResumeThread(frmBot->hGameThread);	// fix
	//if(frmBot->hRecvThread)
	//	ResumeThread(frmBot->hRecvThread);

	CloseHandle(frmBot->hGameThread);
	//CloseHandle(frmBot->hRecvThread);

	SetWindowLongPtr(hGameHWND,GWLP_WNDPROC,(DWORD)frmBot->dwGameWndProc);

	DestroyMenu(frmBot->hMainMenu);
	CloseHandle(frmBot->hMainMenu);
	::SetMenu(hGameHWND,NULL);
	DrawMenuBar(hGameHWND);

	CloseHandle(hGameHWND);
}

void LogOut(){
	CMainSocket = _CWinsock.NewSocket();
	if((long)CMainSocket.GetSocket() == INVALID_SOCKET)
		return;

	int iRetry = 0;

	while(1){
		if(CMainSocket.Client_Connect(strServerIP,iServerPort))
			break;

		if(iRetry == 5){
			CMainSocket.Close();
			//TerminateGame();
			if(bAutoLogin == false)
				MessageBox(frmBot->Handle,"Server'dan cikis yapilamadi. (Baglanti hatasi)","Hata",MB_ICONEXCLAMATION);
			return;
		}

		iRetry++;
		Sleep(100);
	}

	char* pSend = new char[iLogoutPacketSize];
	memset(pSend,0,iLogoutPacketSize);

	int index(0);

	CMainSocket.PutPacketHead(pSend,index);
	CMainSocket.PutBYTE(pSend,bSend_LOGOUT,index);
	CMainSocket.PutWORD(pSend,(WORD)iMyUserIndex,index);
	CMainSocket.PutWORD(pSend,(WORD)iMyUserID,index);
	CMainSocket.PutWORD(pSend,(WORD)iMyLogOutKey,index);
	CMainSocket.PutPacketTail(pSend,index);

	CMainSocket.Client_Send(pSend,iLogoutPacketSize);
	CMainSocket.Close();
	delete [] pSend;
}

void BanOther(){
	CMainSocket = _CWinsock.NewSocket();
	if((long)CMainSocket.GetSocket() == INVALID_SOCKET)
		return;

	int iRetry = 0;

	while(1){
		if(CMainSocket.Client_Connect(strServerIP,iServerPort))
			break;

		if(iRetry == 5){
			CMainSocket.Close();
			//TerminateGame();
			MessageBox(frmBot->Handle,"Baglanti sonlandirma yapilamadi! (Baglanti hatasi)","Hata",MB_ICONEXCLAMATION);
			return;
		}

		iRetry++;
		Sleep(100);
	}

	char* pSend = new char[iBanOtherPacketSize];
	memset(pSend,0,iBanOtherPacketSize);

	int index(0);

	CMainSocket.PutPacketHead(pSend,index);
	CMainSocket.PutBYTE(pSend,bSend_BANOTHER,index);
	CMainSocket.PutWORD(pSend,(WORD)iMyUserIndex,index);
	CMainSocket.PutWORD(pSend,(WORD)iMyUserID,index);
	CMainSocket.PutWORD(pSend,(WORD)iMyLogOutKey,index);
	CMainSocket.PutPacketTail(pSend,index);

	CMainSocket.Client_Send(pSend,iBanOtherPacketSize);
	CMainSocket.Close();
	delete [] pSend;
}

void __fastcall TfrmBot::FormClose(TObject *Sender, TCloseAction &Action)
{
	CloseBot(false);
	QuitBot();
}
//---------------------------------------------------------------------------

void TerminateGame(int iFailIndex){
	_CGeneral.DisconnectServer();

	switch(iFailIndex){

		case 1:
		_CGeneral.PostMsgBox("Bot initialization failed. (MEMORY [1])",1,1);
		break;

		case 2:
		_CGeneral.PostMsgBox("Bot initialization failed. (HWND [2])",1,1);
		break;

		case 3:
		_CGeneral.PostMsgBox("Bot initialization failed. (CLASS [3])",1,1);
		break;

		case 4:
		_CGeneral.PostMsgBox("Bot initialization failed. (RECV_PATCH [4])",1,1);
		break;

		case 5:
		_CGeneral.PostMsgBox("Bot initialization failed. (NULL_KEY [5])",1,1);
		break;

		case 6:
		_CGeneral.PostMsgBox("Bot initialization failed. (CONNECTION_FAILED [6])",1,1);
		break;

		case 7:
		_CGeneral.PostMsgBox("Bot initialization failed. (NO_RESPONSE [7])",1,1);
		break;

		case 8:
		_CGeneral.PostMsgBox("Bot initialization failed. (INVALID_PACKET [8])",1,1);
		break;

		case 9:
		_CGeneral.PostMsgBox("Uyeliginiz serverdan kopmustur. Uyeliginize baskasi girmis olabilir!(NO_LOGIN [9])",1,1);
		break;

		case 10:
		_CGeneral.PostMsgBox("Bot initialization failed. (WRONG_IP [10])",1,1);
		break;

		case 11:
		_CGeneral.PostMsgBox("Bot kullanim sureniz dolmustur. (TIMEOUT [11])",1,1);
		break;

		case 12:
		_CGeneral.PostMsgBox("Bilinmeyen cevap. Bu hatayi aldiginizda lutfen bot yetkilileri ile iletisime gecip hatayi iletiniz... (UNKNOWN [12])",1,1);
		break;

	}
}

void __fastcall TfrmBot::chshenabledClick(TObject *Sender)
{
	tmsh->Interval = iSHDelay;
	tmsh->Enabled = chshenabled->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::scshChange(TObject *Sender)
{
	iSHBoost = scsh->Position;
	char tmp[4];
	itoa(iSHBoost,tmp,10);
	lbshboost->Caption = tmp;
}
//---------------------------------------------------------------------------


void __fastcall TfrmBot::txtshdelayChange(TObject *Sender)
{
	iSHDelay = atoi(txtshdelay->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmshTimer(TObject *Sender)
{
	if(CheckLoading() == true)
		return;

	if(_CCharBase.GetDWORD(KO_CRHP) == 0)
		return;

	if(!GetAsyncKeyState(VK_LSHIFT))
		return;

	if(_CCharBase.IsSit())
		_CTest.CommandSit(false);

	int iBoost = iSHBoost;
	float fPtrX = _CCharBase.GetFLOAT(KO_PTRX);
	float fPtrY = _CCharBase.GetFLOAT(KO_PTRY);

	if(_CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),fPtrX,fPtrY) > 0.5)
	{
		if(bInterClient && bStickSH)
			SendStickSH();

		SHTeleport(fPtrX,fPtrY,iBoost);
	}

}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::oparcheryClick(TObject *Sender)
{
	iSkillIndex = 0;
	dwCastNum = _CCast.MakeCastNum(_CCharBase,"003");
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::opperfectClick(TObject *Sender)
{
	iSkillIndex = 1;
	dwCastNum = _CCast.MakeCastNum(_CCharBase,"525");
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::oparcClick(TObject *Sender)
{
	iSkillIndex = 2;
	dwCastNum = _CCast.MakeCastNum(_CCharBase,"540");
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chbonus3_1Click(TObject *Sender)
{
	bBonus3_1 = chbonus3_1->Checked;

	if(bAutoLogin == true)
		return;

	if(bBonus3_1)
		MessageBox(this->Handle,"Dikkat! Bu ozelligi coklu char aciyorsaniz kullanmanizi tavsiye etmiyoruz. Bu ozelligi kullandiginizda etrafinizdakilerin dc alma olasiligi cok yuksektir, o yuzden lutfen charlarýnýzýn dc olmamasý icin bu ozelligi ya tek charda kullanin, ya da forumda yazan yontemi uygulayarak charlarinizda dc almayi engelleyin.","Dikkat!",MB_ICONEXCLAMATION);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::opautomobClick(TObject *Sender)
{
	iTargetingType = 0;
	pntarget->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::opmanualmobClick(TObject *Sender)
{
	iTargetingType = 1;
	pntarget->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::oplistmobClick(TObject *Sender)
{
	iTargetingType = 2;
	pntarget->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtatakdelayChange(TObject *Sender)
{
	iAttackDelay = atoi(txtatakdelay->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chbonus3Click(TObject *Sender)
{
	bBonus3 = chbonus3->Checked;
	if(bBonus3)
	{
		chbonus5->Checked = false;
		bBonus5 = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chbonus5Click(TObject *Sender)
{
	bBonus5 = chbonus5->Checked;
	if(bBonus5)
	{
		chbonus3->Checked = false;
		bBonus3 = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtbonusdistChange(TObject *Sender)
{
	iBonusDistance = atoi(txtbonusdist->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chshoweffectClick(TObject *Sender)
{
	bShowEffect = chshoweffect->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chshowmobClick(TObject *Sender)
{
	bShowMob = chshowmob->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chwaitforkillClick(TObject *Sender)
{
	bWaitForKill = chwaitforkill->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chminorClick(TObject *Sender)
{
	bMinor = chminor->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chmanaClick(TObject *Sender)
{
	bMana = chmana->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::commanaChange(TObject *Sender)
{
	iManaType = commana->ItemIndex;

	switch(iManaType){

		case 0:
		dwManaSkillNum = SKILL_MP120;
		dwManaItemNum = ITEM_MP120;
		break;

		case 1:
		dwManaSkillNum = SKILL_MP240;
		dwManaItemNum = ITEM_MP240;
		break;

		case 2:
		dwManaSkillNum = SKILL_MP480;
		dwManaItemNum = ITEM_MP480;
		break;

		case 3:
		dwManaSkillNum = SKILL_MP960;
		dwManaItemNum = ITEM_MP960;
		break;

		case 4:
		dwManaSkillNum = SKILL_MP1920;
		dwManaItemNum = ITEM_MP1920;
		break;

		case 5:
		dwManaSkillNum = SKILL_PREPOT_MP;
		dwManaItemNum = ITEM_PREPOT_MP;
		break;

		case 6:
		dwManaSkillNum = SKILL_DCPOT_MP;
		dwManaItemNum = ITEM_DCPOT_MP;
		break;

		default:
		break;

	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtmanalimitChange(TObject *Sender)
{
	iManaLimit = atoi(txtmanalimit->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chwolfClick(TObject *Sender)
{
	bWolf = chwolf->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chtsClick(TObject *Sender)
{
	bTS = chts->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chsprintClick(TObject *Sender)
{
	bSprint = chsprint->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chhideClick(TObject *Sender)
{
	bHide = chhide->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chscaledClick(TObject *Sender)
{
	bScaled = chscaled->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chcureClick(TObject *Sender)
{
	bCure = chcure->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chswClick(TObject *Sender)
{
	bSW = chsw->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chevadeClick(TObject *Sender)
{
	bEvade = chevade->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chlupClick(TObject *Sender)
{
	bLup = chlup->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chlfClick(TObject *Sender)
{
	bLF = chlf->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chsafetyClick(TObject *Sender)
{
	bSafety = chsafety->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chcatsClick(TObject *Sender)
{
	bCats = chcats->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chmovingClick(TObject *Sender)
{
	iCurrentRoute = 1;
	iLastPos = 0;
	bMoving = chmoving->Checked;
	//tmmove->Enabled = bMoving;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtchatdelayChange(TObject *Sender)
{
	iChatDelay = atoi(txtchatdelay->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::comchattypeChange(TObject *Sender)
{
	iChatType = comchattype->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chlootClick(TObject *Sender)
{
	bLoot = chloot->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chmovechestClick(TObject *Sender)
{

	if(bMoveToChest && (!iCenterX || !iCenterY)){
		MessageBox(this->Handle,"Once merkez almalisiniz","hata",MB_ICONEXCLAMATION);
		chmovechest->Checked = false;
		bMoveToChest = false;
		return;
	}
	bMoveToChest = chmovechest->Checked;

	bMoveChestPaused = false;

	if(!bMoveToChest)
		tmmovechest->Enabled = false;

	//if(bMoveToChest && !bBotState)
	//	return;

	//tmmovechest->Enabled = bMoveToChest;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtchestdistChange(TObject *Sender)
{
	iMaxChestDistance = atoi(txtchestdist->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chonlyuniqueClick(TObject *Sender)
{
	bLootUnique = chonlyunique->Checked;
}

//---------------------------------------------------------------------------

void __fastcall TfrmBot::chupgradelimitClick(TObject *Sender)
{
	bLootUpgrade = chupgradelimit->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtupgradelimitChange(TObject *Sender)
{
	iLootUpgradeLimit = atoi(txtupgradelimit->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chsundClick(TObject *Sender)
{
	bSund = chsund->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chpotClick(TObject *Sender)
{
	bPot = chpot->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chinnClick(TObject *Sender)
{
	bInn = chinn->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chbuywolfClick(TObject *Sender)
{
	bBuyWolf = chbuywolf->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chbuyarrowClick(TObject *Sender)
{
	bBuyArrow = chbuyarrow->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnbuymanaClick(TObject *Sender)
{
	bBuyMana = btnbuymana->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtbuymanaChange(TObject *Sender)
{
	iBuyManaLimit = atoi(txtbuymana->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtinnslotChange(TObject *Sender)
{
	iInnBeginSlot = atoi(txtinnslot->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chinndepotClick(TObject *Sender)
{
	bInnDepot = chinndepot->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chfollowptClick(TObject *Sender)
{
	bFollowLeader = chfollowpt->Checked;

	if(bFollowLeader || bFollowSelected){
		tmfollow->Enabled = true;
		chwh->Checked = true;
		_CCharBase.SetDWORD(KO_AUTH,!chwh->Checked);
	}
	else
		tmfollow->Enabled = false;

	   /*
	if(!bFollowLeader && !bFollowSelected){
		tmfollow->Enabled = false;
	}
	else{
		chwh->Checked = true;
		_CCharBase.SetDWORD(KO_AUTH,!chwh->Checked);
	}
	*/
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chfollowselectedClick(TObject *Sender)
{
	bFollowSelected = chfollowselected->Checked;

	if(bFollowLeader || bFollowSelected){
		tmfollow->Enabled = true;
		chwh->Checked = true;
		_CCharBase.SetDWORD(KO_AUTH,!chwh->Checked);
	}
	else
		tmfollow->Enabled = false;

	/*
	if(!bFollowLeader && !bFollowSelected)
		tmfollow->Enabled = false;
	else{
		chwh->Checked = true;
		_CCharBase.SetDWORD(KO_AUTH,!chwh->Checked);
	}
	*/
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chotoptClick(TObject *Sender)
{
	bAutoParty = chotopt->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chgmtownClick(TObject *Sender)
{
	bTownOnGM = chgmtown->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chmobtownClick(TObject *Sender)
{
	bTownOnMob = chmobtown->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chtstownClick(TObject *Sender)
{
	bTownOnNoTS = chtstown->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::charrowtownClick(TObject *Sender)
{
	bTownOnNoArrow = charrowtown->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chmanatownClick(TObject *Sender)
{
	bTownOnNoMana = chmanatown->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chpartytownClick(TObject *Sender)
{
	bTownOnNoParty = chpartytown->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chringondieClick(TObject *Sender)
{
	bRingOnDie = chringondie->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chringwhentownClick(TObject *Sender)
{
	bRingOnTown = chringwhentown->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chinformsupplyClick(TObject *Sender)
{
	bInformAfterSupply = chinformsupply->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnclearmobClick(TObject *Sender)
{
	if(!iMobCount)
		return;

	lstatakmobs->Clear();

	for (int i = 0; i < 10; i++) {
		if(strMobList[i])
			delete [] strMobList[i];
		strMobList[i] = NULL;
	}

	iMobCount = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btngetmobnameClick(TObject *Sender)
{
	DWORD dwID = _CCharBase.GetTargetID();

	if(!dwID || dwID == 0xFFFF)
		return;

	CCharBase pMob = _CArea.GetNPCBase(dwID);
	if(!pMob.GetBase())
		return;

	txtatakmob->Text = pMob.GetName();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnaddmobClick(TObject *Sender)
{
	if(iMobCount == 10)
		return;

	if(!txtatakmob->Text.Length() || txtatakmob->Text.Length() >= 32)
		return;

	char *pNewName = new char [32];
	memset(pNewName,0,32);

	strcpy(pNewName,txtatakmob->Text.c_str());
	strMobList[iMobCount] = pNewName;
	lstatakmobs->AddItem(pNewName,Sender);
	iMobCount++;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmbuffingTimer(TObject *Sender)
{

	if(CheckLoading() == true)
		return;

	if(!_CCharBase.GetDWORD(KO_CRHP)){
		if(bAutoRevive){
			char pRevive[1];
			*pRevive = 0x12;
			_CPacket.PSend(pRevive,1);
			SupplyChat();
		}
		return;
	}

	if(!bSitWhenIdle){
		if(_CCharBase.IsSit())
			return;
	}

	DWORD dwSkillNum;
	DWORD dwCurrentTick = GetTickCount();

	int iPosX,iPosY,iPosZ;

	if(bMinor){
		if(dwCurrentTick - iLastMinorTick >= 175){
			if(_CMath.iPerc(_CCharBase.GetDWORD(KO_MXHP),_CCharBase.GetDWORD(KO_CRHP)) <= (float)iMinorLimit){
				dwSkillNum = _CCast.MakeCastNum(_CCharBase,"705");
				*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;
				_CPacket.PSend(pBuf_BuffSelf2,24);
				iLastMinorTick = dwCurrentTick;
			}
		}
	}

	if(bMana){
		if(dwCurrentTick - iLastManaTick >= 2100){
			if(!_CCharBase.GetDWORD(KO_CRMP) || _CMath.iPerc(_CCharBase.GetDWORD(KO_MXMP),_CCharBase.GetDWORD(KO_CRMP)) <= (float)iManaLimit){
				if(_CBag.FindInvItem(0,dwManaItemNum) != -1){

					if(bSitWhenIdle){	//priest
						if(_CCharBase.IsSit())
							_CTest.CommandSit(false);
					}

					*(DWORD*)(pBuf_BuffSelf2 + 2) = dwManaSkillNum;
					_CPacket.PSend(pBuf_BuffSelf2,24);
					iLastManaTick = dwCurrentTick;

					if(bSitWhenIdle)	//priest
						_CTest.CommandSit(true);

				}
			}
		}
	}

	if(bHP){
		if(dwCurrentTick - iLastHPTick >= 2100){
			if(_CCharBase.GetDWORD(KO_CRHP) && _CMath.iPerc(_CCharBase.GetDWORD(KO_MXHP),_CCharBase.GetDWORD(KO_CRHP)) <= (float)iHPLimit){
				if(_CBag.FindInvItem(0,dwHPItemNum) != -1){

					if(bSitWhenIdle){	//priest
						if(_CCharBase.IsSit())
							_CTest.CommandSit(false);
					}

					*(DWORD*)(pBuf_BuffSelf2 + 2) = dwHPSkillNum;
					_CPacket.PSend(pBuf_BuffSelf2,24);
					iLastHPTick = dwCurrentTick;

					if(bSitWhenIdle)	//priest
						_CTest.CommandSit(true);

				}
			}
		}
	}

	if(bWolf){
		if(dwCurrentTick - iLastWolfTick >= 3000){
			dwSkillNum = _CCast.MakeCastNum(_CCharBase,"030");
            if(!_CCast.IsCasted(dwSkillNum)){
				if(_CBag.FindInvItem(0,ITEM_WOLF) != -1){

					iPosX = (int)_CCharBase.GetFLOAT(KO_POSX);
					iPosY = (int)_CCharBase.GetFLOAT(KO_POSY);
					iPosZ = (int)_CCharBase.GetFLOAT(KO_POSZ);

					*(DWORD*)(pBuf_BuffSelf1 + 2) = dwSkillNum;
					*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;

					*(WORD*)(pBuf_BuffSelf1 + 8) = 0xFFFF;
					*(WORD*)(pBuf_BuffSelf2 + 8) = 0xFFFF;

					*(WORD*)(pBuf_BuffSelf1 + 10) = iPosX;
					*(WORD*)(pBuf_BuffSelf2 + 10) = iPosX;
					*(WORD*)(pBuf_BuffSelf1 + 12) = iPosZ;
					*(WORD*)(pBuf_BuffSelf2 + 12) = iPosZ;
					*(WORD*)(pBuf_BuffSelf1 + 14) = iPosY;
					*(WORD*)(pBuf_BuffSelf2 + 14) = iPosY;

					*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x11;

					_CPacket.PSend(pBuf_BuffSelf1,24);
					_CPacket.PSend(pBuf_BuffSelf2,22);

					*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x00;
					*(WORD*)(pBuf_BuffSelf1 + 8) = dwMyID;
					*(WORD*)(pBuf_BuffSelf2 + 8) = dwMyID;
					*(WORD*)(pBuf_BuffSelf1 + 10) = 0x0000;
					*(WORD*)(pBuf_BuffSelf2 + 10) = 0x0000;
					*(WORD*)(pBuf_BuffSelf1 + 12) = 0x0000;
					*(WORD*)(pBuf_BuffSelf2 + 12) = 0x0000;
					*(WORD*)(pBuf_BuffSelf1 + 14) = 0x0000;
					*(WORD*)(pBuf_BuffSelf2 + 14) = 0x0000;

					iLastWolfTick = dwCurrentTick;
				}
			}
		}
	}

	if(bTS){
		if(dwCurrentTick - iLastTSTick >= 3000){

			if(iAttackType == 1){
				if(bAsasAttack || bRAttack)
					dwSkillNum = SKILL_TS_KECOON;
				else
					dwSkillNum = SKILL_TS_BOWMAN;
			}
			else
				dwSkillNum = SKILL_TS_KECOON;

			if(!_CCast.IsCasted(dwSkillNum)){
				if(_CBag.FindInvItem(0,ITEM_TS_SCROLL) != -1 || _CBag.FindInvItem(0,ITEM_TS_GEM) != -1){

					if(bSitWhenIdle){	//priest
						if(_CCharBase.IsSit())
							_CTest.CommandSit(false);
					}

					*(DWORD*)(pBuf_BuffSelf1 + 2) = SKILL_TS_CAST;
					*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;
					_CPacket.PSend(pBuf_BuffSelf1,24);
					_CPacket.PSend(pBuf_BuffSelf2,22);
					iLastTSTick = dwCurrentTick;

					if(bSitWhenIdle)	//priest
						_CTest.CommandSit(true);

				}
			}
		}
	}

	if(bSprint){
		if(dwCurrentTick - iLastSprintTick >= 7000){
			dwSkillNum = _CCast.MakeCastNum(_CCharBase,"725");
			if(!_CCast.IsCasted(dwSkillNum)){
				dwSkillNum = _CCast.MakeCastNum(_CCharBase,"010");
				if(!_CCast.IsCasted(dwSkillNum)){
					dwSkillNum = _CCast.MakeCastNum(_CCharBase,"002");
					if(!_CCast.IsCasted(dwSkillNum)){
						*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;
						_CPacket.PSend(pBuf_BuffSelf2,24);
						iLastSprintTick = dwCurrentTick;
					}
				}
			}
		}
	}

	if(bHide){
		if(dwCurrentTick - iLastHideTick >= 10000){
			dwSkillNum = _CCast.MakeCastNum(_CCharBase,"700");
			if(_CCast.IsCasted(dwSkillNum) == false){
				*(DWORD*)(pBuf_BuffSelf1 + 2) = dwSkillNum;
				*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;
				*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x0F;
				_CPacket.PSend(pBuf_BuffSelf1,24);
				_CPacket.PSend(pBuf_BuffSelf2,22);
				*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x00;
				iLastHideTick = dwCurrentTick;
			}
		}
	}

	if(bScaled){
		if(dwCurrentTick - iLastScaledTick >= 40000){
			dwSkillNum = _CCast.MakeCastNum(_CCharBase,"710");
			if(_CCast.IsCasted(dwSkillNum) == false){
				dwSkillNum = _CCast.MakeCastNum(_CCharBase,"730");
				if(_CCast.IsCasted(dwSkillNum) == false){
					dwSkillNum = _CCast.MakeCastNum(_CCharBase,"760");
					if(_CCast.IsCasted(dwSkillNum) == false){
						*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;
						_CPacket.PSend(pBuf_BuffSelf2,22);
						iLastScaledTick = dwCurrentTick;
					}
				}
			}
		}
	}

	if(bSW){
		if(dwCurrentTick - iLastSWTick >= 3000){
			dwSkillNum = _CCast.MakeCastNum(_CCharBase,"725");
			if(!_CCast.IsCasted(dwSkillNum)){
				dwSkillNum = _CCast.MakeCastNum(_CCharBase,"002");
				if(!_CCast.IsCasted(dwSkillNum)){
					dwSkillNum = _CCast.MakeCastNum(_CCharBase,"010");
					if(!_CCast.IsCasted(dwSkillNum)){
						*(DWORD*)(pBuf_BuffSelf1 + 2) = dwSkillNum;
						*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;
						*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x0F;
						_CPacket.PSend(pBuf_BuffSelf1,24);
						_CPacket.PSend(pBuf_BuffSelf2,22);
						*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x00;
						iLastSWTick = dwCurrentTick;
					}
				}
			}
		}
	}

	if(bEvade){
		if(dwCurrentTick - iLastEvadeTick >= 40000){
			dwSkillNum = _CCast.MakeCastNum(_CCharBase,"760");
			if(_CCast.IsCasted(dwSkillNum) == false){
				dwSkillNum = _CCast.MakeCastNum(_CCharBase,"730");
				if(_CCast.IsCasted(dwSkillNum) == false){
					dwSkillNum = _CCast.MakeCastNum(_CCharBase,"710");
					if(_CCast.IsCasted(dwSkillNum) == false){
						*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;
						_CPacket.PSend(pBuf_BuffSelf2,22);
						iLastEvadeTick = dwCurrentTick;
					}
				}
			}
		}
	}

	if(bSafety){
		if(dwCurrentTick - iLastSafetyTick >= 40000){
			dwSkillNum = _CCast.MakeCastNum(_CCharBase,"760");
			if(_CCast.IsCasted(dwSkillNum) == false){
				dwSkillNum = _CCast.MakeCastNum(_CCharBase,"710");
				if(_CCast.IsCasted(dwSkillNum) == false){
					dwSkillNum = _CCast.MakeCastNum(_CCharBase,"730");
					if(_CCast.IsCasted(dwSkillNum) == false){
						*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;
						_CPacket.PSend(pBuf_BuffSelf2,22);
						iLastSafetyTick = dwCurrentTick;
					}
				}
			}
		}
	}

	if(bLup){
		if(dwCurrentTick - iLastLupTick >= 10000){
			dwSkillNum = _CCast.MakeCastNum(_CCharBase,"715");
			if(_CCast.IsCasted(dwSkillNum) == false){
				dwSkillNum = _CCast.MakeCastNum(_CCharBase,"735");
				if(_CCast.IsCasted(dwSkillNum) == false){
					*(DWORD*)(pBuf_BuffSelf1 + 2) = dwSkillNum;
					*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;
					*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x14;
					_CPacket.PSend(pBuf_BuffSelf1,24);
					_CPacket.PSend(pBuf_BuffSelf2,22);
					*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x00;
					iLastLupTick = dwCurrentTick;
				}
			}
		}
	}

	if(bLF){
		if(dwCurrentTick - iLastLFTick >= 10000){
			dwSkillNum = _CCast.MakeCastNum(_CCharBase,"002");
			if(!_CCast.IsCasted(dwSkillNum)){
				dwSkillNum = _CCast.MakeCastNum(_CCharBase,"010");
				if(!_CCast.IsCasted(dwSkillNum)){
					dwSkillNum = _CCast.MakeCastNum(_CCharBase,"725");
					if(!_CCast.IsCasted(dwSkillNum)){
						*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;
						_CPacket.PSend(pBuf_BuffSelf2,24);
						iLastLFTick = dwCurrentTick;
					}
				}
			}
		}
	}

	if(bCats){
		if(dwCurrentTick - iLastCatsTick >= 5000){
			dwSkillNum = _CCast.MakeCastNum(_CCharBase,"735");
			if(_CCast.IsCasted(dwSkillNum) == false){
				dwSkillNum = _CCast.MakeCastNum(_CCharBase,"715");
				if(_CCast.IsCasted(dwSkillNum) == false){
					*(DWORD*)(pBuf_BuffSelf1 + 2) = dwSkillNum;
					*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;
					*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x0F;
					_CPacket.PSend(pBuf_BuffSelf1,24);
					_CPacket.PSend(pBuf_BuffSelf2,22);
					*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x00;
					iLastCatsTick = dwCurrentTick;
				}
			}
		}
	}

	if(bAntiFreeze){
		if(_CCharBase.GetBYTE(KO_BSTN)){
			_CCharBase.SetBYTE(KO_BSTN,0);
			_CCharBase.SetFLOAT(KO_FSTN,0);
		}
	}

	if(bMage62){
		if(dwCurrentTick - iLastMage62Tick >= 60000){
			dwSkillNum = _CCast.MakeCastNum(_CCharBase,"802");
			if(_CCast.IsCasted(dwSkillNum) == false){
				*(DWORD*)(pBuf_BuffSelf1 + 2) = dwSkillNum;
				*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;
				*(BYTE*)(pBuf_BuffSelf1 + 22) = lMageCastData62;
				_CPacket.PSend(pBuf_BuffSelf1,24);
				_CPacket.PSend(pBuf_BuffSelf2,22);
				*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x00;
				iLastMage62Tick = dwCurrentTick;
			}
		}
	}

	if(bPriAutoBook){
		if(dwCurrentTick - iLastPriBookTick >= 3000){
			dwSkillNum = _CCast.MakeCastNum(_CCharBase,"020");
			if(_CCast.IsCasted(dwSkillNum) == false){

				if(_CBag.FindInvItem(0,ITEM_BOOK) != -1){

					if(bSitWhenIdle){	//priest
						if(_CCharBase.IsSit())
							_CTest.CommandSit(false);
					}

					*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;
					_CPacket.PSend(pBuf_BuffSelf2,22);
					iLastPriBookTick = dwCurrentTick;

					if(bSitWhenIdle)	//priest
						_CTest.CommandSit(true);

				}
			}
		}
	}

	if(bCure){
		if(dwCurrentTick - iLastCureTick >= 3000){
			if(CheckSelfCure() == true){
				dwSkillNum = _CCast.MakeCastNum(_CCharBase,"736");
				*(DWORD*)(pBuf_BuffSelf1 + 2) = dwSkillNum;
				*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;
				*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x0F;
				_CPacket.PSend(pBuf_BuffSelf1,24);
				_CPacket.PSend(pBuf_BuffSelf2,22);
				*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x00;
				iLastCureTick = dwCurrentTick;
			}
		}
	}

	if(bSprintWarrior){
		if(dwCurrentTick - iLastSprintTick >= 7000){
			dwSkillNum = 108010; //swift
			if(!_CCast.IsCasted(dwSkillNum)){
				dwSkillNum = _CCast.MakeCastNum(_CCharBase,"002");
				if(!_CCast.IsCasted(dwSkillNum)){
					*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;
					_CPacket.PSend(pBuf_BuffSelf2,24);
					iLastSprintTick = dwCurrentTick;
				}
			}
		}
	}

	if(bDefenseWarrior){
		if(dwCurrentTick - iLastDefenseTick >= 10000){
			dwSkillNum = _CCast.MakeCastNum(_CCharBase,"007");
			if(!_CCast.IsCasted(dwSkillNum)){
				*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;
				_CPacket.PSend(pBuf_BuffSelf2,24);
				iLastDefenseTick = dwCurrentTick;
			}
		}
	}

	if(bGainWarrior){
		if(dwCurrentTick - iLastGainTick >= 10000){
			dwSkillNum = _CCast.MakeCastNum(_CCharBase,"705");	//test
			if(!_CCast.IsCasted(dwSkillNum)){
				*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;
				_CPacket.PSend(pBuf_BuffSelf2,24);
				iLastGainTick = dwCurrentTick;
			}
		}
	}

	if(bSTR30){
		if(dwCurrentTick - iLastPriestSTR30Tick >= 3000){
			if(_CCast.IsCasted(dwPriestSTR30CastNum) == false){
				if(_CCast.IsCasted(dwPriSTRCastNum) == false){

					if(bSitWhenIdle){	//priest
						if(_CCharBase.IsSit())
							_CTest.CommandSit(false);
					}

					*(DWORD*)(pBuf_BuffSelf2 + 2) = dwPriestSTR30CastNum;
					_CPacket.PSend(pBuf_BuffSelf2,24);
					iLastPriestSTR30Tick = dwCurrentTick;

					if(bSitWhenIdle)	//priest
						_CTest.CommandSit(true);
				}
			}
		}
	}

	if(bAutoMeat)
	{
		if(dwCurrentTick - iLastMeatTick >= 3000)
		{
			if(_CCast.IsCasted(SKILL_MEAT) == false)
			{
				*(DWORD*)(pBuf_BuffSelf2 + 2) = SKILL_MEAT;
				*(BYTE*)(pBuf_BuffSelf2 + 22) = 0x64;
				_CPacket.PSend(pBuf_BuffSelf2,24);
				*(BYTE*)(pBuf_BuffSelf2 + 22) = 0x00;
				iLastMeatTick = dwCurrentTick;
            }
        }
    }

}
//---------------------------------------------------------------------------

void QuitBot(){
	//FreeLibraryAndExitThread(GetModuleHandle("kojdbot.dll"),0);

	HMODULE hModule = NULL;
	bOwnCallHandleExA = true;
	GetModuleHandleEx(NULL,"kojdbot.dll",&hModule);
	bOwnCallHandleExA = false;
	FreeLibraryAndExitThread(hModule,0);
}

void __fastcall TfrmBot::btnmoveClick(TObject *Sender)
{
	int iPosX = atoi(txtmovex->Text.c_str());
	int iPosY = atoi(txtmovey->Text.c_str());

	if(iPosX && iPosY)
		_CCharBase.MoveTo((float)iPosX,(float)iPosY);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnclearmoveClick(TObject *Sender)
{
	lstmove->Clear();
	iMoveCount = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnaddmoveClick(TObject *Sender)
{
	if(iMoveCount == 20)
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

	lstmove->AddItem(buf,Sender);

	iMoveX[iMoveCount] = (int)fMyX;
	iMoveY[iMoveCount] = (int)fMyY;
	iMoveCount++;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chchatClick(TObject *Sender)
{
	tmchat->Interval = txtchatdelay->Text.ToInt();
	tmchat->Enabled = chchat->Checked;

	//if(chchat->Checked == false)
	//	tmchat->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmchatTimer(TObject *Sender)
{

	if(CheckLoading() == true)
		return;

	if(!txtchat->Text.Length())
		return;

	int iPacketSize = txtchat->Text.Length() + 4;
	char *pBuf = new char[iPacketSize];
	memset(pBuf,0,iPacketSize);

	_CPacket.PutBYTE(pBuf,0x10);

	switch(iChatType){
		case 0:
		_CPacket.PutBYTE(pBuf+1,0x01);
		break;
		case 1:
		_CPacket.PutBYTE(pBuf+1,0x03);
		break;
		case 2:
		_CPacket.PutBYTE(pBuf+1,0x0E);
		break;
	}

	_CPacket.PutWORD(pBuf+2,txtchat->Text.Length());
	strcpy(pBuf+4,txtchat->Text.c_str());

	_CPacket.PSend(pBuf,iPacketSize);
	delete [] pBuf;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnstartClick(TObject *Sender)
{
	if(iTotalCryption < 24)
	{
		LogError(18);
		__asm
		{
			PUSH 0xFFFFFFFF
			POP dwRecvdSockHandle
			XOR EAX,EAX
			SUB ESP,8
			XCHG [ESP+4],EAX
			OR EAX,0xFFFFFFFF
			XCHG [ESP],EAX
			CALL dwTerminatePointer
		}

	}

	if(dlgMaden->bEnabled)
	{
		InitMineZone();
		shpstatus->Brush->Color = clLime;
		bBotState = true;
		dlgMaden->iCurrentAction = 1;
		dlgMaden->bTowned = false;
		dlgMaden->bTownOK = false;
		dlgMaden->bOnMineWay = true;
		dlgMaden->bOnSundWay = false;
		iLastDurabilityL = -1;
		iLastDurabilityR = -1;
		dlgMaden->tmmaden->Enabled = true;
		return;
	}


	if(iAttackType == 1){	//archer

		dwArcheryCastArray[0] = _CCast.MakeCastNum(_CCharBase,"003");
		dwArcheryCastArray[1] = _CCast.MakeCastNum(_CCharBase,"007");
		dwArcheryCastArray[2] = _CCast.MakeCastNum(_CCharBase,"500");
		dwArcheryCastArray[3] = _CCast.MakeCastNum(_CCharBase,"505");
		dwArcheryCastArray[4] = _CCast.MakeCastNum(_CCharBase,"510");
		dwArcheryCastArray[5] = _CCast.MakeCastNum(_CCharBase,"520");
		dwArcheryCastArray[6] = _CCast.MakeCastNum(_CCharBase,"525");
		dwArcheryCastArray[7] = _CCast.MakeCastNum(_CCharBase,"530");
		dwArcheryCastArray[8] = _CCast.MakeCastNum(_CCharBase,"535");
		dwArcheryCastArray[9] = _CCast.MakeCastNum(_CCharBase,"540");
		dwArcheryCastArray[10] = _CCast.MakeCastNum(_CCharBase,"545");
		dwArcheryCastArray[11] = _CCast.MakeCastNum(_CCharBase,"550");
		dwArcheryCastArray[12] = _CCast.MakeCastNum(_CCharBase,"552");
		dwArcheryCastArray[13] = _CCast.MakeCastNum(_CCharBase,"557");
		dwArcheryCastArray[14] = _CCast.MakeCastNum(_CCharBase,"560");
		dwArcheryCastArray[15] = _CCast.MakeCastNum(_CCharBase,"562");
		dwArcheryCastArray[16] = _CCast.MakeCastNum(_CCharBase,"566");
		dwArcheryCastArray[17] = _CCast.MakeCastNum(_CCharBase,"570");
		dwArcheryCastArray[18] = _CCast.MakeCastNum(_CCharBase,"580");
		dwArcheryCastArray[19] = _CCast.MakeCastNum(_CCharBase,"585");

		dwCastNum_3 = _CCast.MakeCastNum(_CCharBase,"515");
		dwCastNum_5 = _CCast.MakeCastNum(_CCharBase,"555");
		dwCastNum_Blow = _CCast.MakeCastNum(_CCharBase,"575");

		dwAsasCastArray[0] = _CCast.MakeCastNum(_CCharBase,"001");
		dwAsasCastArray[1] = _CCast.MakeCastNum(_CCharBase,"005");
		dwAsasCastArray[2] = _CCast.MakeCastNum(_CCharBase,"006");
		dwAsasCastArray[3] = _CCast.MakeCastNum(_CCharBase,"600");
		dwAsasCastArray[4] = _CCast.MakeCastNum(_CCharBase,"615");
		dwAsasCastArray[5] = _CCast.MakeCastNum(_CCharBase,"620");
		dwAsasCastArray[6] = _CCast.MakeCastNum(_CCharBase,"635");
		dwAsasCastArray[7] = _CCast.MakeCastNum(_CCharBase,"640");
		dwAsasCastArray[8] = _CCast.MakeCastNum(_CCharBase,"655");
		dwPercent10Num = _CCast.MakeCastNum(_CCharBase,"650");
		dwPercent5Num = _CCast.MakeCastNum(_CCharBase,"610");

		if(bAsasAttack || bRAttack)
			tmasasbot->Enabled = true;
		else if(bArcheryAttack)
			tmattack->Enabled = true;

	}
	else if(iAttackType == 2){	//mage

		dwMageBasicCastArray[0] = _CCast.MakeCastNum(_CCharBase,"002");
		dwMageBasicCastArray[1] = _CCast.MakeCastNum(_CCharBase,"003");
		dwMageBasicCastArray[2] = _CCast.MakeCastNum(_CCharBase,"005");
		dwMageBasicCastArray[3] = _CCast.MakeCastNum(_CCharBase,"007");
		dwMageBasicCastArray[4] = _CCast.MakeCastNum(_CCharBase,"009");

		dwMageFlameCastArray[0] = _CCast.MakeCastNum(_CCharBase,"503");
		dwMageFlameCastArray[1] = _CCast.MakeCastNum(_CCharBase,"509");
		dwMageFlameCastArray[2] = _CCast.MakeCastNum(_CCharBase,"515");
		dwMageFlameCastArray[3] = _CCast.MakeCastNum(_CCharBase,"518");
		dwMageFlameCastArray[4] = _CCast.MakeCastNum(_CCharBase,"527");
		dwMageFlameCastArray[5] = _CCast.MakeCastNum(_CCharBase,"533");
		dwMageFlameCastArray[6] = _CCast.MakeCastNum(_CCharBase,"535");
		dwMageFlameCastArray[7] = _CCast.MakeCastNum(_CCharBase,"539");
		dwMageFlameCastArray[8] = _CCast.MakeCastNum(_CCharBase,"542");
		dwMageFlameCastArray[9] = _CCast.MakeCastNum(_CCharBase,"543");
		dwMageFlameCastArray[10] = _CCast.MakeCastNum(_CCharBase,"545");
		dwMageFlameCastArray[11] = _CCast.MakeCastNum(_CCharBase,"551");
		dwMageFlameCastArray[12] = _CCast.MakeCastNum(_CCharBase,"554");
		dwMageFlameCastArray[13] = _CCast.MakeCastNum(_CCharBase,"556");
		dwMageFlameCastArray[14] = _CCast.MakeCastNum(_CCharBase,"557");
		dwMageFlameCastArray[15] = _CCast.MakeCastNum(_CCharBase,"560");
		dwMageFlameCastArray[16] = _CCast.MakeCastNum(_CCharBase,"571");
		dwMageFlameCastArray[17] = _CCast.MakeCastNum(_CCharBase,"570");
		dwMageFlameCastArray[18] = _CCast.MakeCastNum(_CCharBase,"572");
		dwMageFlameCastArray[19] = _CCast.MakeCastNum(_CCharBase,"574");
		dwMageFlameCastArray[20] = _CCast.MakeCastNum(_CCharBase,"575");

		dwMageIceCastArray[0] = _CCast.MakeCastNum(_CCharBase,"603");
		dwMageIceCastArray[1] = _CCast.MakeCastNum(_CCharBase,"609");
		dwMageIceCastArray[2] = _CCast.MakeCastNum(_CCharBase,"615");
		dwMageIceCastArray[3] = _CCast.MakeCastNum(_CCharBase,"618");
		dwMageIceCastArray[4] = _CCast.MakeCastNum(_CCharBase,"627");
		dwMageIceCastArray[5] = _CCast.MakeCastNum(_CCharBase,"633");
		dwMageIceCastArray[6] = _CCast.MakeCastNum(_CCharBase,"635");
		dwMageIceCastArray[7] = _CCast.MakeCastNum(_CCharBase,"639");
		dwMageIceCastArray[8] = _CCast.MakeCastNum(_CCharBase,"642");
		dwMageIceCastArray[9] = _CCast.MakeCastNum(_CCharBase,"643");
		dwMageIceCastArray[10] = _CCast.MakeCastNum(_CCharBase,"645");
		dwMageIceCastArray[11] = _CCast.MakeCastNum(_CCharBase,"651");
		dwMageIceCastArray[12] = _CCast.MakeCastNum(_CCharBase,"656");
		dwMageIceCastArray[13] = _CCast.MakeCastNum(_CCharBase,"657");
		dwMageIceCastArray[14] = _CCast.MakeCastNum(_CCharBase,"660");
		dwMageIceCastArray[15] = _CCast.MakeCastNum(_CCharBase,"671");
		dwMageIceCastArray[16] = _CCast.MakeCastNum(_CCharBase,"670");
		dwMageIceCastArray[17] = _CCast.MakeCastNum(_CCharBase,"672");

		dwMageLightCastArray[0] = _CCast.MakeCastNum(_CCharBase,"703");
		dwMageLightCastArray[1] = _CCast.MakeCastNum(_CCharBase,"709");
		dwMageLightCastArray[2] = _CCast.MakeCastNum(_CCharBase,"715");
		dwMageLightCastArray[3] = _CCast.MakeCastNum(_CCharBase,"718");
		dwMageLightCastArray[4] = _CCast.MakeCastNum(_CCharBase,"727");
		dwMageLightCastArray[5] = _CCast.MakeCastNum(_CCharBase,"733");
		dwMageLightCastArray[6] = _CCast.MakeCastNum(_CCharBase,"735");
		dwMageLightCastArray[7] = _CCast.MakeCastNum(_CCharBase,"739");
		dwMageLightCastArray[8] = _CCast.MakeCastNum(_CCharBase,"742");
		dwMageLightCastArray[9] = _CCast.MakeCastNum(_CCharBase,"743");
		dwMageLightCastArray[10] = _CCast.MakeCastNum(_CCharBase,"745");
		dwMageLightCastArray[11] = _CCast.MakeCastNum(_CCharBase,"751");
		dwMageLightCastArray[12] = _CCast.MakeCastNum(_CCharBase,"754");
		dwMageLightCastArray[13] = _CCast.MakeCastNum(_CCharBase,"756");
		dwMageLightCastArray[14] = _CCast.MakeCastNum(_CCharBase,"757");
		dwMageLightCastArray[15] = _CCast.MakeCastNum(_CCharBase,"760");
		dwMageLightCastArray[16] = _CCast.MakeCastNum(_CCharBase,"771");
		dwMageLightCastArray[17] = _CCast.MakeCastNum(_CCharBase,"770");
		dwMageLightCastArray[18] = _CCast.MakeCastNum(_CCharBase,"772");

		DWORD dwSkillBase;

		for (int i = 0; i < 5; i++) {
			dwSkillBase = _CTest.GetSkillBase(dwMageBasicCastArray[i]);
			if(dwSkillBase)
				iMageBasicDataArray[i] = *(DWORD*)(dwSkillBase + 0x78);
		}

		for (int i = 0; i < 21; i++) {

			dwSkillBase = _CTest.GetSkillBase(dwMageFlameCastArray[i]);
			if(dwSkillBase)
				iMageFlameDataArray[i] = *(DWORD*)(dwSkillBase + 0x78);

			dwSkillBase = _CTest.GetSkillBase(dwMageIceCastArray[i]);
			if(dwSkillBase)
				iMageIceDataArray[i] = *(DWORD*)(dwSkillBase + 0x78);

			dwSkillBase = _CTest.GetSkillBase(dwMageLightCastArray[i]);
			if(dwSkillBase)
				iMageLightDataArray[i] = *(DWORD*)(dwSkillBase + 0x78);
		}

		tmmageattack->Enabled = true;
	}
	else if(iAttackType == 3){	//priest
		RefreshPriestCasts();
		CheckHealCasts();

		dwPriestCastArray[0] = _CCast.MakeCastNum(_CCharBase,"001");
		dwPriestCastArray[1] = _CCast.MakeCastNum(_CCharBase,"006");

		switch(iPriestAttackType){
			case 1:	//heal
			dwPriestCastArray[2] = _CCast.MakeCastNum(_CCharBase,"511");
			dwPriestCastArray[3] = _CCast.MakeCastNum(_CCharBase,"520");
			dwPriestCastArray[4] = _CCast.MakeCastNum(_CCharBase,"542");
			dwPriestCastArray[5] = _CCast.MakeCastNum(_CCharBase,"551");
			dwPriestSTR30CastNum = _CCast.MakeCastNum(_CCharBase,"529");
			break;

			case 2:	//buff
			dwPriestCastArray[2] = _CCast.MakeCastNum(_CCharBase,"611");
			dwPriestCastArray[3] = _CCast.MakeCastNum(_CCharBase,"620");
			dwPriestCastArray[4] = _CCast.MakeCastNum(_CCharBase,"641");
			dwPriestCastArray[5] = _CCast.MakeCastNum(_CCharBase,"650");
			dwPriestSTR30CastNum = _CCast.MakeCastNum(_CCharBase,"629");
			break;

			case 3:	//debuff
			dwPriestCastArray[2] = _CCast.MakeCastNum(_CCharBase,"712");
			dwPriestCastArray[3] = _CCast.MakeCastNum(_CCharBase,"721");
			dwPriestCastArray[4] = _CCast.MakeCastNum(_CCharBase,"739");
			dwPriestCastArray[5] = _CCast.MakeCastNum(_CCharBase,"750");
			dwPriestSTR30CastNum = _CCast.MakeCastNum(_CCharBase,"729");
			break;
		}

		dwPriestCastArray[6] = _CCast.MakeCastNum(_CCharBase,"802");
		dwPriestCastArray[7] = _CCast.MakeCastNum(_CCharBase,"815");

		dwPriestFarCastArray[0] = _CCast.MakeCastNum(_CCharBase,"003");
		dwPriestFarCastArray[1] = _CCast.MakeCastNum(_CCharBase,"008");
		dwPriestFarCastArray[2] = _CCast.MakeCastNum(_CCharBase,"018");
		dwPriestFarCastArray[3] = _CCast.MakeCastNum(_CCharBase,"024");
		dwPriestFarCastArray[4] = _CCast.MakeCastNum(_CCharBase,"033");

		dwPriestDebuffArray[0] = _CCast.MakeCastNum(_CCharBase,"703");
		dwPriestDebuffArray[1] = _CCast.MakeCastNum(_CCharBase,"745");
		dwPriestDebuffArray[2] = _CCast.MakeCastNum(_CCharBase,"757");
		dwPriestDebuffArray[3] = _CCast.MakeCastNum(_CCharBase,"760");

		tmpriest->Enabled = true;
		if(bPriestAttack || bRAttack || bDebuff)
			tmpribot->Enabled = true;
	}
	else if(iAttackType == 4){	//warrior
		dwWarriorCastArray[0] = _CCast.MakeCastNum(_CCharBase,"001");
		dwWarriorCastArray[1] = _CCast.MakeCastNum(_CCharBase,"003");
		dwWarriorCastArray[2] = _CCast.MakeCastNum(_CCharBase,"005");
		dwWarriorCastArray[3] = _CCast.MakeCastNum(_CCharBase,"009");
		dwWarriorCastArray[4] = _CCast.MakeCastNum(_CCharBase,"500");
		dwWarriorCastArray[5] = _CCast.MakeCastNum(_CCharBase,"505");
		dwWarriorCastArray[6] = _CCast.MakeCastNum(_CCharBase,"510");
		dwWarriorCastArray[7] = _CCast.MakeCastNum(_CCharBase,"515");
		dwWarriorCastArray[8] = _CCast.MakeCastNum(_CCharBase,"525");
		dwWarriorCastArray[9] = _CCast.MakeCastNum(_CCharBase,"530");
		dwWarriorCastArray[10] = _CCast.MakeCastNum(_CCharBase,"535");
		dwWarriorCastArray[11] = _CCast.MakeCastNum(_CCharBase,"545");
		dwWarriorCastArray[12] = _CCast.MakeCastNum(_CCharBase,"550");
		dwWarriorCastArray[13] = _CCast.MakeCastNum(_CCharBase,"555");
		dwWarriorCastArray[14] = _CCast.MakeCastNum(_CCharBase,"557");
		dwWarriorCastArray[15] = _CCast.MakeCastNum(_CCharBase,"560");
		dwWarriorCastArray[16] = _CCast.MakeCastNum(_CCharBase,"570");
		dwWarriorCastArray[17] = _CCast.MakeCastNum(_CCharBase,"580");

		dwBindingCastNum = _CCast.MakeCastNum(_CCharBase,"630");
		dwProvokeCastNum = _CCast.MakeCastNum(_CCharBase,"645");

		tmwarriorbot->Enabled = true;
	}

	//
	shpstatus->Brush->Color = clLime;
	ClearDrops();
	tmmovechest->Enabled = bMoveToChest;
	tmsupply->Enabled		= true;
	tmbuffing->Enabled 		= true;
	tmtown->Enabled			= true;

	/*if(bFollowLeader || bFollowSelected)
		tmfollow->Enabled = true;*/

	iCurrentRoute = 1;
	iLastPos = 0;
	tmmove->Enabled = bMoving;

	//tmchat->Interval = atoi(txtchatdelay->Text.c_str());
	//tmchat->Enabled = chchat->Checked;
	tmtargetsize->Enabled = chtargetsize->Checked;

	if(bAutoGate || bLimitTP)
		tmtp->Enabled = true;

	SundReturned();
	InnReturned();
	PotReturned();

	bMoveChestPaused = false;
	bRunMobPaused = false;

	//merkez al
	iBotCenterX = (int)_CCharBase.GetFLOAT(KO_POSX);
	iBotCenterY = (int)_CCharBase.GetFLOAT(KO_POSY);
	iCenterX = iBotCenterX;
	iCenterY = iBotCenterY;

	char _tmp[8];
	memset(_tmp,0,8);

	char tmp[10];
	memset(tmp,0,10);

	itoa(iBotCenterX,tmp,10);
	strcat(tmp,",");
	itoa(iBotCenterY,_tmp,10);
	strcat(tmp,_tmp);
	lbbotcenter->Caption = tmp;
	lbcenter->Caption = tmp;
	//

	bBotState = true;
	//_CRecvFunction.Resume();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnstopClick(TObject *Sender)
{
	StopBot();
}
//---------------------------------------------------------------------------


void __fastcall TfrmBot::tmattackTimer(TObject *Sender)
{
	if(bBotPaused)
		return;

	if(!bArcheryAttack)
		return;

	if(CheckLoading() == true)
		return;

	if(!_CCharBase.GetDWORD(KO_CRHP))
		return;

	if(_CCharBase.IsSit() == true)
		return;

	DWORD	dwCurrentTick = GetTickCount();

	//if(dwCurrentTick - iLastAttackTick < (DWORD)iAttackDelay)
	//	return;

	bool bCastAble = (bool)(dwCurrentTick - iLastAttackTick >= (DWORD)iAttackDelay);
	bool bTurnPercent10 = (bool)(bCastAble && bAsasPercent10 && dwCurrentTick - iLastPercent10Tick >= 60000);
	bool bTurnPercent5 = (bool)(bCastAble && bAsasPercent5 && dwCurrentTick - iLastPercent5Tick >= 60000);
	bool bTurn[20];
	bool bTurnBonus5 = false;
	bool bTurnBlow = (bool)(bBlow && dwCurrentTick - iLastAttackTick >= 1050);
	//bool bTurn3_1 = (bool)(bBonus3_1 && dwCurrentTick - iLastArcheryBonusTick >= (DWORD)iAttackDelay);
	bool bTurn3_1 = (bool)(bBonus3_1 && bCastAble && dwCurrentTick - iLastArcheryBonusTick >= (DWORD)iAttackDelay );
	if(dwCurrentTick - iLastArcheryBonusTick < (DWORD)iAttackDelay)
		bCastAble = false;

	if(bCastAble == false){
		for (int i = 0; i < 20; i++) {
			bTurn[i] = false;
		}
	}
	else{
		bTurn[0] = bArcheryCastArray[0];
		bTurn[1] = bArcheryCastArray[1];
		bTurn[2] = bArcheryCastArray[2];
		bTurn[3] = (bool)(bArcheryCastArray[3] && dwCurrentTick - iArcheryCastArray[3] >= 4500);
		bTurn[4] = (bool)(bArcheryCastArray[4] && dwCurrentTick - iArcheryCastArray[4] >= 4500);
		bTurn[5] = bArcheryCastArray[5];
		bTurn[6] = bArcheryCastArray[6];
		bTurn[7] = (bool)(bArcheryCastArray[7] && dwCurrentTick - iArcheryCastArray[7] >= 5000);
		bTurn[8] = (bool)(bArcheryCastArray[8] && dwCurrentTick - iArcheryCastArray[8] >= 5000);
		bTurn[9] = bArcheryCastArray[9];
		bTurn[10] = (bool)(bArcheryCastArray[10] && dwCurrentTick - iArcheryCastArray[10] >= 5000);
		bTurn[11] = (bool)(bArcheryCastArray[11] && dwCurrentTick - iArcheryCastArray[11] >= 5000);
		bTurn[12] = (bool)(bArcheryCastArray[12] && dwCurrentTick - iArcheryCastArray[12] >= 60000); //cs
		bTurn[13] = bArcheryCastArray[13];
		bTurn[14] = bArcheryCastArray[14];
		bTurn[15] = (bool)(bArcheryCastArray[15] && dwCurrentTick - iArcheryCastArray[15] >= 7500);
		bTurn[16] = (bool)(bArcheryCastArray[16] && dwCurrentTick - iArcheryCastArray[16] >= 7500);
		bTurn[17] = bArcheryCastArray[17];
		bTurn[18] = (bool)(bArcheryCastArray[18] && dwCurrentTick - iArcheryCastArray[18] >= 60000); //blinding
		bTurn[19] = (bool)(bArcheryCastArray[19] && dwCurrentTick - iArcheryCastArray[19] >= 60000); //ps
	}

	CCharBase pTarget = NULL;
	CCharBase pTmp = NULL;
	float fDistance;
	DWORD TmpVal;
	int i;

	DWORD dwCurrentCast = 0;
	int iCurrentCast = -1;

	if( bAttackWithLeader )
	{
		_CParty.Refresh();
		if(_CParty.GetCount() < 2)
			return;
		if(wLastTarget && wLastTarget != 0xFFFF){
			pTmp = _CArea.GetAnyBase(wLastTarget);
			if(pTmp.GetBase()){
				if(pTmp.IsAlive()){
					fDistance = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pTmp.GetFLOAT(KO_POSX),pTmp.GetFLOAT(KO_POSY));
					if(fDistance <= 12.5)
					{
						goto SELECT;
					}
				}
			}
		}
		return;
	}

	if(bWaitForKill){
		if(wLastTarget && wLastTarget != 0xFFFF){
			pTmp = _CArea.GetAnyBase(wLastTarget);
			if(pTmp.GetBase()){
				if(pTmp.IsAlive()){
					fDistance = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pTmp.GetFLOAT(KO_POSX),pTmp.GetFLOAT(KO_POSY));
					if(fDistance <= 12.5)
					{
						if(bInterClient)
							SendTarget();
						goto SELECT;
					}
				}
			}
		}
	}

	switch(iTargetingType){

		case 0:	//auto
		{
			if(bOnlyEnemy)
				pTarget = _CArea.GetNearstUPC(true,true,12.5);
			else
				pTarget = _CArea.GetNearstEnemy(0,true,12.5);
			break;
		}

		case 1:	//manuel
		{
			wLastTarget = _CCharBase.GetTargetID();
			if(wLastTarget == 0xFFFF)
			{
				if(bInterClient)
					SendTarget();
				return;
			}

			pTarget = _CArea.GetAnyBase(wLastTarget);
			if(!pTarget.GetBase())
			{
				if(bInterClient)
				{
					wLastTarget = 0xFFFF;
					SendTarget();
				}
				return;
			}

			if(!pTarget.IsAlive())
			{
				if(bInterClient)
				{
					wLastTarget = 0xFFFF;
					SendTarget();
				}
				return;
			}

			if(bOnlyEnemy){
				if(!pTarget.IsUPC())
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}

				if(pTarget.GetDWORD(KO_NATN) == _CCharBase.GetDWORD(KO_NATN))
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}
			}

			if(bOnlyTarget){
				if(!pTarget.IsEnemy())
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}
			}

			break;
		}

		case 2:	//list
		{
			if(!iMobCount)
			{
				if(bInterClient)
				{
					wLastTarget = 0xFFFF;
					SendTarget();
				}
				return;
			}

			pTarget = _CArea.GetNPCByName(iMobCount,strMobList,NULL,true,true,true,12.5,false);
			break;
		}

		case 3: //sub
		{
			if(iSubTargetingType == 1)
			{
				if(!iTargetMobIDCount)
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}

				pTarget = _CArea.GetNearstNPCByID(iTargetMobID,iTargetMobIDCount,true,12.5);
			}
			else if(iSubTargetingType == 2)
			{
				if(!iTargetNickCount)
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}

				pTarget = _CArea.GetUPCByName(iTargetNickCount,strTargetNick,0,true,12.5,false,true);
			}
			break;
		}

		default:
		return;
		break;
	}

SELECT:
	if(pTarget.GetBase() == NULL)
		pTarget.SetBase(pTmp.GetBase());

	if(!pTarget.GetBase())
	{
		if(bInterClient)
		{
			wLastTarget = 0xFFFF;
			SendTarget();
		}
		return;
	}

	wLastTarget = pTarget.GetID();

	if(bInterClient)
		SendTarget();

	fDistance = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pTarget.GetFLOAT(KO_POSX),pTarget.GetFLOAT(KO_POSY));
	if(fDistance > 12.5)
		return;

	if(bCastAble || bTurnBlow)
	{
		if(bShowMob)
			_CArea.SelectObject(pTarget);
		else
			SendTargetHP(wLastTarget);
	}

CAST:

	if(pTarget.GetBase() == NULL)
		pTarget.SetBase(pTmp.GetBase());

	if(bTurnPercent10){
		*(DWORD*)(pBuf_Attack1 + 2) = dwPercent10Num;
		*(DWORD*)(pBuf_Attack3 + 2) = dwPercent10Num;
		*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack1 + 22) = 0x10;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack1,24);
		_CPacket.PSend(pBuf_Attack3,22);
		iLastAttackTick = dwCurrentTick;
		iLastPercent10Tick = dwCurrentTick;
		return;
	}

	if(bTurnPercent5 && !bTurnPercent10){
		*(DWORD*)(pBuf_Attack1 + 2) = dwPercent5Num;
		*(DWORD*)(pBuf_Attack3 + 2) = dwPercent5Num;
		*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack1 + 22) = 0x10;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack1,24);
		_CPacket.PSend(pBuf_Attack3,22);
		iLastAttackTick = dwCurrentTick;
		iLastPercent5Tick = dwCurrentTick;
		return;
	}

	*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
	*(WORD*)(pBuf_Attack2 + 8) = wLastTarget;
	*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
	*(WORD*)(pBuf_Attack4 + 8) = wLastTarget;

	*(BYTE*)(pBuf_Attack1 + 22) = 0x0D;
	*(BYTE*)(pBuf_Attack2 + 16) = 0x01;
	*(BYTE*)(pBuf_Attack3 + 16) = 0x01;
	*(BYTE*)(pBuf_Attack4 + 18) = 0x01;

	//timed:
	if(bTurn[18]){	//strafe
		dwCurrentCast = dwArcheryCastArray[18];
		iCurrentCast = 18;
	}
	else if(bTurn[19]){	//ps
		dwCurrentCast = dwArcheryCastArray[19];
		iCurrentCast = 19;
	}
	else if(bTurn[12]){	//cs
		dwCurrentCast = dwArcheryCastArray[12];
		iCurrentCast = 12;
	}
	else if(bTurn[16]){	//light
		dwCurrentCast = dwArcheryCastArray[16];
		iCurrentCast = 16;
	}
	else if(bTurn[15]){	//ice
		dwCurrentCast = dwArcheryCastArray[15];
		iCurrentCast = 15;
	}
	else if(bTurn[11]){
		dwCurrentCast = dwArcheryCastArray[11];
		iCurrentCast = 11;
	}
	else if(bTurn[10]){
		dwCurrentCast = dwArcheryCastArray[10];
		iCurrentCast = 10;
	}
	else if(bTurn[8]){
		dwCurrentCast = dwArcheryCastArray[8];
		iCurrentCast = 8;
	}
	else if(bTurn[7]){
		dwCurrentCast = dwArcheryCastArray[7];
		iCurrentCast = 7;
	}
	else if(bTurn[4]){
		dwCurrentCast = dwArcheryCastArray[4];
		iCurrentCast = 4;
	}
	else if(bTurn[3]){
		dwCurrentCast = dwArcheryCastArray[3];
		iCurrentCast = 3;
	}
	//no-timed:
	else if(bTurn[17]){	//pursuer
		dwCurrentCast = dwArcheryCastArray[17];
		iCurrentCast = 17;
	}
	else if(bTurn[14]){
		dwCurrentCast = dwArcheryCastArray[14];
		iCurrentCast = 14;
	}
	else if(bTurn[13]){
		dwCurrentCast = dwArcheryCastArray[13];
		iCurrentCast = 13;
	}
	else if(bTurn[9]){
		dwCurrentCast = dwArcheryCastArray[9];
		iCurrentCast = 9;
	}
	else if(bTurn[6]){
		dwCurrentCast = dwArcheryCastArray[6];
		iCurrentCast = 6;
	}
	else if(bTurn[5]){
		dwCurrentCast = dwArcheryCastArray[5];
		iCurrentCast = 5;
	}
	else if(bTurn[2]){
		dwCurrentCast = dwArcheryCastArray[2];
		iCurrentCast = 2;
	}
	else if(bTurn[1]){
		dwCurrentCast = dwArcheryCastArray[1];
		iCurrentCast = 1;
	}
	else if(bTurn[0]){
		dwCurrentCast = dwArcheryCastArray[0];
		iCurrentCast = 0;
	}

	if(bTurnBlow && fDistance < 2.7){
		if((!dwCurrentCast || iCurrentCast == -1) && (!bTurn[18] && !bTurn[19] && !bTurn[12] && !bTurn[16] && !bTurn[15]))
		*(DWORD*)(pBuf_Attack3 + 2) = dwCastNum_Blow;
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		*(BYTE*)(pBuf_Attack3 + 16) = 0x01;
		iLastAttackTick = dwCurrentTick;
	}

	bTurnBonus5 = (bBonus5 && dwCurrentTick - iLastAttackTick >= (DWORD)iAttackDelay && dwCurrentTick - iLastBonus5Tick >= 1500 && (int)fDistance <= iBonusDistance);

	if(dwCurrentTick - iLastAttackTick >= (DWORD)iAttackDelay && !bTurnBonus5){

		if(dwCurrentCast && iCurrentCast > -1){

			if( bLegalAttack )
				_CTest.CastMagic(dwCurrentCast,(int)wLastTarget);
			else
			{
				*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
				*(DWORD*)(pBuf_Attack2 + 2) = dwCurrentCast;
				*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;
				*(DWORD*)(pBuf_Attack4 + 2) = 108585;

				if(iCurrentCast == 12)	//cs
					*(BYTE*)(pBuf_Attack1 + 22) = 0x0A;

				_CPacket.PSend(pBuf_Attack1,24);
				_CPacket.PSend(pBuf_Attack2,22);
				_CPacket.PSend(pBuf_Attack3,24);
				if(bShowEffect)
					_CPacket.PSend(pBuf_Attack4,24);
			}

			iLastAttackTick = dwCurrentTick;
			iArcheryCastArray[iCurrentCast] = dwCurrentTick;
		}

	}

		if(bCastAble && (int)fDistance <= iBonusDistance){
			if(bBonus3){
				*(DWORD*)(pBuf_Attack1 + 2) = dwCastNum_3;
				*(DWORD*)(pBuf_Attack2 + 2) = dwCastNum_3;
				*(DWORD*)(pBuf_Attack3 + 2) = dwCastNum_3;
				*(DWORD*)(pBuf_Attack4 + 2) = dwCastNum_3;

				_CPacket.PSend(pBuf_Attack1,24);
				_CPacket.PSend(pBuf_Attack2,22);

				for (i = 0; i < 3; i++) {
					*(BYTE*)(pBuf_Attack3 + 16) = (BYTE)i;
					_CPacket.PSend(pBuf_Attack3,24);

					if(bShowEffect){
						*(BYTE*)(pBuf_Attack4 + 18) = (BYTE)i;
						_CPacket.PSend(pBuf_Attack4,24);
					}
					Sleep(10);
				}
				return;
			}

			if(bTurnBonus5){
				*(DWORD*)(pBuf_Attack1 + 2) = dwCastNum_5;
				*(DWORD*)(pBuf_Attack2 + 2) = dwCastNum_5;
				*(DWORD*)(pBuf_Attack3 + 2) = dwCastNum_5;
				*(DWORD*)(pBuf_Attack4 + 2) = dwCastNum_5;

				*(BYTE*)(pBuf_Attack1 + 22) = 0x0F;

				_CPacket.PSend(pBuf_Attack1,24);
				_CPacket.PSend(pBuf_Attack2,22);

				for (i = 0; i < 5; i++) {
					*(BYTE*)(pBuf_Attack3 + 16) = (BYTE)i;
					_CPacket.PSend(pBuf_Attack3,24);

					if(bShowEffect){
						*(BYTE*)(pBuf_Attack4 + 18) = (BYTE)i;
						_CPacket.PSend(pBuf_Attack4,24);
					}
					Sleep(10);
				}
				iLastAttackTick = dwCurrentTick;
				iLastBonus5Tick = dwCurrentTick;
				return;
			}
		}

	if(bTurn3_1)
	{
		*(DWORD*)(pBuf_Attack1 + 2) = dwCastNum_3;
		*(DWORD*)(pBuf_Attack2 + 2) = dwCastNum_3;
		*(DWORD*)(pBuf_Attack3 + 2) = dwCastNum_3;
		*(DWORD*)(pBuf_Attack4 + 2) = dwCastNum_3;

		_CPacket.PSend(pBuf_Attack1,24);
		_CPacket.PSend(pBuf_Attack2,22);
		_CPacket.PSend(pBuf_Attack3,24);
		//if(bShowEffect)
		//	_CPacket.PSend(pBuf_Attack4,24);

		iLastArcheryBonusTick = dwCurrentTick;
	}
}
//---------------------------------------------------------------------------


void __fastcall TfrmBot::tmmoveTimer(TObject *Sender)
{
	int iPosX,iPosY;

	if(bMovePaused)
		return;

	if(iMoveCount < 2)
		return;

	iPosX = (int) _CCharBase.GetFLOAT(KO_POSX);
	iPosY = (int) _CCharBase.GetFLOAT(KO_POSY);

	if(iCurrentRoute == 1){
		if(_CMath.fDist(iPosX,iPosY,iMoveX[iLastPos],iMoveY[iLastPos]) < 0.5)
			iLastPos++;

		if(iLastPos == iMoveCount){
			iLastPos--;
			iCurrentRoute = 2;	//Come
			return;
		}

		_CCharBase.MoveTo(iMoveX[iLastPos],iMoveY[iLastPos]);
	}
	else if(iCurrentRoute == 2){
		if(_CMath.fDist(iPosX,iPosY,iMoveX[iLastPos],iMoveY[iLastPos]) < 0.5)
			iLastPos--;

		if(iLastPos == -1){
			iLastPos++;
			iCurrentRoute = 1;	//Go
			return;
		}

		_CCharBase.MoveTo(iMoveX[iLastPos],iMoveY[iLastPos]);
	}

}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmmovechestTimer(TObject *Sender)
{
	if(bLootPaused || bMoveChestPaused)
		return;

	if(CheckLoading() == true)
		return;

	if(!_CCharBase.GetDWORD(KO_CRHP))
		return;

	float fCenterDist = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iCenterX,(float)iCenterY);

	if(lstChestList.empty() || lstDropList.empty()){
		//ClearDrops();
		if(iCenterX && iCenterY){
			if(fCenterDist >= 0.5 && fCenterDist <= (float)iMaxChestDistance){
				if(bSitWhenIdle){
					if(_CCharBase.IsSit() == true)
					_CTest.CommandSit(false);
				}
				SHChestTeleport((float)iCenterX,(float)iCenterY,5);
			}
			else{
				if(bRunToMob){
					bRunMobPaused = false;
					bMoveChestPaused = true;
					if(bSitWhenIdle && _CCharBase.IsSit() == false)
						_CTest.CommandSit(true);
				}
			}
		}
		return;
	}

	if(bRunToMob)
		bRunMobPaused = true;

	_CHEST *pCurrentChest = lstChestList.front();

	if(!pCurrentChest)
		return;

	if(!_CArea.GetAnyBase(pCurrentChest->m_sSourceID)){
		delete (_CHEST*)lstChestList.front();
		lstChestList.pop_front();

		delete (_DROP*)lstDropList.front();
		lstDropList.pop_front();
		return;
	}

	if(fCenterDist <= (float)iMaxChestDistance){
		if(SHChestTeleport(pCurrentChest->m_fPosX,pCurrentChest->m_fPosY,5) == true){
				_DROP *pCurrentDrop = lstDropList.front();

				if(!pCurrentDrop)
					return;

				char pSend[9];

				*(BYTE*)pSend = 0x24;
				*(DWORD*)(pSend + 1) = pCurrentChest->m_lChestID;
				_CPacket.PSend(pSend,5);

				*(BYTE*)pSend = 0x26;
				*(DWORD*)(pSend + 1) = pCurrentChest->m_lChestID;

				for (int i = 0; i < 4; i++) {
					if(pCurrentDrop->m_lItemID[i]){
						*(DWORD*)(pSend + 5) = pCurrentDrop->m_lItemID[i];
						_CPacket.PSend(pSend,9);
					}
				}

				delete (_CHEST*)lstChestList.front();
				lstChestList.pop_front();

				delete (_DROP*)lstDropList.front();
				lstDropList.pop_front();
		}
	}

}
//---------------------------------------------------------------------------


void __fastcall TfrmBot::btncenterClick(TObject *Sender)
{
	iCenterX = (int)_CCharBase.GetFLOAT(KO_POSX);
	iCenterY = (int)_CCharBase.GetFLOAT(KO_POSY);
	iBotCenterX = iCenterX;
	iBotCenterY = iCenterY;

	char _tmp[8];
	memset(_tmp,0,8);

	char tmp[10];
	memset(tmp,0,10);

	itoa(iCenterX,tmp,10);
	strcat(tmp,",");
	itoa(iCenterY,_tmp,10);
	strcat(tmp,_tmp);
	lbcenter->Caption = tmp;
	lbbotcenter->Caption = tmp;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmcheckTimer(TObject *Sender)
{
	dwCheckFuncRet = 0xFFFFFFFF;
	CheckFunction();

	if( dwCheckFuncRet == 1 )
		return;

	LogError(6);
	OwnTerminate();
}

void CheckFunction()
{
	if( *(BYTE*)KO_CRYP != 0x53 )
	{
    	LogError(7);
		CloseBot(false);
		OwnTerminate();
		return;
	}

	BYTE Idx = _CPacket.bFuncIndex;
	if( Idx == (BYTE)-1)
		Idx = _CPacket.GetIndex();

	DWORD dwCallAddr = (Idx * 4) + *(DWORD*)KO_SOCK + 0x40064;
	DWORD dwFunc = *(DWORD*)dwCallAddr;
	if(dwFunc >= 0x500000 && dwFunc != (DWORD)&SendFunction && dwFunc != (DWORD)&HookedSendFunction)
	{
    	LogError(8);
		CloseBot(false);
		OwnTerminate();
		return;
	}

	iAPIOrFuncCheck++;

	if(iAPIOrFuncCheck == 3)
	{
		iAPIOrFuncCheck = 0;

		for (int i = 0; i < 9; i++)
		{
			if( dwAPIAddr[i] )
			{
				if( *(DWORD*)dwAPIAddr[i] != dwAPIVal[i] )
				{
                	LogError(9);
					OwnTerminate();
					return;
				}
			}
		}

		for (int i = 0; i < 20; i++)
		{
			if( *(DWORD*)dwFuncAddr[i] != dwFuncVal[i] )
			{
            	LogError(10);
				OwnTerminate();
				return;
			}
		}

	}


	DWORD dwAddrOwn = (DWORD) &OwnTerminate;
	DWORD dwBytes = *(DWORD*)dwAddrOwn;
	dwBytes += *(WORD*)(dwAddrOwn + 4);

	if( dwBytes != 0xFF6A00D4 )
	{
    	LogError(11);
		ExitProcess(-1);
		return;
	}

	iDebuggerCheck++;
	if(iDebuggerCheck == 3)
	{
		CheckDebugger();
		iDebuggerCheck = 0;
	}
	else
		__asm XOR dwCheckFuncRet,0xFFFFFFFE
}
//---------------------------------------------------------------------------

bool SHChestTeleport(float fToX, float fToY, int iBoosting){

if(_CCharBase.IsSit())
	_CTest.CommandSit(false);

int iBoost = iBoosting;
float fCurX,fCurY;
float fPtrX,fPtrY;
float fNewX,fNewY,fNewZ;

bool bArrivedX = false, bArrivedY = false;

char pBuf[10];	//packet

_CPacket.PutBYTE(pBuf,0x06);
_CPacket.PutWORD(pBuf+7,0x2D);
_CPacket.PutBYTE(pBuf+9,0x00);

if(frmBot->bStunChar){
	if(!frmBot->fLastMovedX || !frmBot->fLastMovedY){
		frmBot->fLastMovedX = _CCharBase.GetFLOAT(KO_POSX);
		frmBot->fLastMovedY = _CCharBase.GetFLOAT(KO_POSY);
	}
	fCurX = frmBot->fLastMovedX;
	fCurY = frmBot->fLastMovedY;
}
else{
	fCurX = _CCharBase.GetFLOAT(KO_POSX);
	fCurY = _CCharBase.GetFLOAT(KO_POSY);
}

fPtrX = fToX;
fPtrY = fToY;

fNewX = fCurX;
fNewY = fCurY;
fNewZ = 0;

if(_CMath.fDist(fCurX,fCurY,fPtrX,fPtrY) > 0.1){

	if(fCurX != fPtrX){
		bArrivedX = false;
		if(fCurX > fPtrX){
			if(fCurX - fPtrX >= (float)iBoost)
				fNewX = fCurX - (float)iBoost;
			else
				fNewX = fPtrX;
		}
		else
		{
			if(fPtrX - fCurX >= (float)iBoost)
				fNewX = fCurX + (float)iBoost;
			else
				fNewX = fPtrX;
		}
	}else bArrivedX = true;

	if(fCurY != fPtrY){
		bArrivedY = false;
		if(fCurY > fPtrY){
			if(fCurY - fPtrY >= (float)iBoost)
				fNewY = fCurY - (float)iBoost;
			else
				fNewY = fPtrY;
		}
		else
		{
			if(fPtrY - fCurY >= (float)iBoost)
				fNewY = fCurY + (float)iBoost;
			else
				fNewY = fPtrY;
		}
	}else bArrivedY = true;

	fNewZ = _CTest.GetPosZ(fNewX,fNewY);

	if(frmBot->bStunChar){	//sabit
		frmBot->fLastMovedX = fNewX;
		frmBot->fLastMovedY = fNewY;
	}
	else
		_CTest.InitPos2(fNewX,fNewY,fNewZ);

	_CPacket.PutWORD(pBuf+1,(int)(fNewX) * 10);
	_CPacket.PutWORD(pBuf+3,(int)(fNewY) * 10);
	_CPacket.PutWORD(pBuf+5,(int)(fNewZ) * 10);
	_CPacket.PutWORD(pBuf+7,0x2D);
	*(pBuf+9) = 3;	//moving
	_CPacket.PSend(pBuf,10);
	return false;
}
else{
	*(pBuf+9) = 0;	//stop
	_CPacket.PutWORD(pBuf+7,0x00);

	if(frmBot->bStunChar){
		_CPacket.PutWORD(pBuf+1,(int)(frmBot->fLastMovedX) * 10);
		_CPacket.PutWORD(pBuf+3,(int)(frmBot->fLastMovedY) * 10);
		_CPacket.PutWORD(pBuf+5,(int)(_CCharBase.GetFLOAT(KO_POSZ)) * 10);
	}
	else{
		_CPacket.PutWORD(pBuf+1,(int)(_CCharBase.GetFLOAT(KO_POSX)) * 10);
		_CPacket.PutWORD(pBuf+3,(int)(_CCharBase.GetFLOAT(KO_POSY)) * 10);
		_CPacket.PutWORD(pBuf+5,(int)(_CCharBase.GetFLOAT(KO_POSZ)) * 10);
	}
	_CPacket.PSend(pBuf,10);
	return true;
}

}

bool SHTeleport(float fToX, float fToY, int iBoosting){

if(CheckLoading() == true)
		return false;

if(_CCharBase.GetDWORD(KO_CRHP) == 0)
		return false;

if(_CCharBase.IsSit())
	_CTest.CommandSit(false);


int iBoost = iBoosting;
float fCurX,fCurY;
float fPtrX,fPtrY;
float fNewX,fNewY,fNewZ;

bool bArrivedX = false, bArrivedY = false;

char pBuf[10];	//packet

_CPacket.PutBYTE(pBuf,0x06);
_CPacket.PutWORD(pBuf+7,0x2D);
_CPacket.PutBYTE(pBuf+9,0x00);

fCurX = _CCharBase.GetFLOAT(KO_POSX);
fCurY = _CCharBase.GetFLOAT(KO_POSY);

fPtrX = fToX;
fPtrY = fToY;

fNewX = fCurX;
fNewY = fCurY;
fNewZ = 0;

if(_CMath.fDist(fCurX,fCurY,fPtrX,fPtrY) > 0.1){

	if(fCurX != fPtrX){
		bArrivedX = false;
		if(fCurX > fPtrX){
			if(fCurX - fPtrX >= (float)iBoost)
				fNewX = fCurX - (float)iBoost;
			else
				fNewX = fPtrX;
		}
		else
		{
			if(fPtrX - fCurX >= (float)iBoost)
				fNewX = fCurX + (float)iBoost;
			else
				fNewX = fPtrX;
		}
	}else bArrivedX = true;

	if(fCurY != fPtrY){
		bArrivedY = false;
		if(fCurY > fPtrY){
			if(fCurY - fPtrY >= (float)iBoost)
				fNewY = fCurY - (float)iBoost;
			else
				fNewY = fPtrY;
		}
		else
		{
			if(fPtrY - fCurY >= (float)iBoost)
				fNewY = fCurY + (float)iBoost;
			else
				fNewY = fPtrY;
		}
	}else bArrivedY = true;

	//float fVal1,fVal2;

	//fVal1 = fCurY - fNewY;
	//fVal2 = fCurX - fNewX;

	//fNewY += 5 * (fVal1 / fVal2);
	fNewZ = _CTest.GetPosZ(fNewX,fNewY);
	_CTest.InitPos2(fNewX,fNewY,fNewZ);

	_CPacket.PutWORD(pBuf+1,(int)(fNewX) * 10);
	_CPacket.PutWORD(pBuf+3,(int)(fNewY) * 10);
	_CPacket.PutWORD(pBuf+5,(int)(fNewZ) * 10);
	_CPacket.PutWORD(pBuf+7,0x2D);
	*(pBuf+9) = 3;	//moving
	_CPacket.PSend(pBuf,10);
	return false;
}
else{
	*(pBuf+9) = 0;	//stop
	_CPacket.PutWORD(pBuf+7,0x00);

	_CPacket.PutWORD(pBuf+1,(int)(_CCharBase.GetFLOAT(KO_POSX)) * 10);
	_CPacket.PutWORD(pBuf+3,(int)(_CCharBase.GetFLOAT(KO_POSY)) * 10);
	_CPacket.PutWORD(pBuf+5,(int)(_CCharBase.GetFLOAT(KO_POSZ)) * 10);
	_CPacket.PSend(pBuf,10);
	return true;
}

}

void __fastcall TfrmBot::chwhClick(TObject *Sender)
{
	_CCharBase.SetDWORD(KO_AUTH,!chwh->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmfollowTimer(TObject *Sender)
{
	if(bFollowPaused)
		return;

	if(bStickSH)
		return;

	if(CheckLoading() == true)
		return;

	if(_CCharBase.GetDWORD(KO_CRHP) == 0)
		return;

	DWORD dwUser;
	WORD wUserID;
	CCharBase pUser;
	FLOAT fDist;

	if(bFollowLeader){
		_CParty.Refresh();
		if(_CParty.GetCount() >= 2){
			dwUser = _CParty.GetUser(0);
			if(dwUser){
				wUserID = *(WORD*)(dwUser + KO_PT_ID);
				if(wUserID != _CCharBase.GetID()){
					pUser = _CArea.GetUPCBase(wUserID);
					if(pUser.GetBase()){
						fDist = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pUser.GetFLOAT(KO_POSX),pUser.GetFLOAT(KO_POSY));
						if(fDist > 0.5){
							if(fDist <= 2.0)
								_CCharBase.MoveTo(pUser.GetFLOAT(KO_POSX),pUser.GetFLOAT(KO_POSY));
							else{
								SHTeleport(pUser.GetFLOAT(KO_POSX),pUser.GetFLOAT(KO_POSY),5);
								_CCharBase.SetDWORD(KO_MOV1,0);
								_CCharBase.SetDWORD(KO_MOV2,0);
							}
						}
					}
				}
			}
		}
	}

	if(bFollowSelected){
		if(_CCharBase.GetTargetID() && _CCharBase.GetTargetID() != 0xFFFF){
			pUser = _CArea.GetUPCBase(_CCharBase.GetTargetID());
			if(pUser.GetBase()){
				fDist = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pUser.GetFLOAT(KO_POSX),pUser.GetFLOAT(KO_POSY));
				if(fDist > 0.5){
					if(fDist <= 2.0)
						_CCharBase.MoveTo(pUser.GetFLOAT(KO_POSX),pUser.GetFLOAT(KO_POSY));
					else{
						SHTeleport(pUser.GetFLOAT(KO_POSX),pUser.GetFLOAT(KO_POSY),5);
						_CCharBase.SetDWORD(KO_MOV1,0);
						_CCharBase.SetDWORD(KO_MOV2,0);
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnexitClick(TObject *Sender)
{
	if( MessageBox(this->Handle,"Oyunu kapat?","Kapat",MB_ICONINFORMATION | MB_YESNO) == IDNO )
		return;

	CloseBot(false);
	TerminateProcess((void*)-1,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmtownTimer(TObject *Sender)
{

	if(CheckLoading() == true)
		return;

	if(mciAlarm)
		return;

	if(bBotPaused)
		return;

	if(!bBotState)
		return;

	if(bRingOnDC){
		if(_CPacket.GetConnState() == false){
			StopBot();
			if(!frmBot->mciAlarm)
				frmBot->mciAlarm = MCIWndCreate(frmBot->Handle, GetModuleHandle(NULL), WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR | MCIWNDF_NOMENU, "alarm.mp3");
			else
				MCIWndHome(frmBot->mciAlarm);

			MCIWndPlay(frmBot->mciAlarm);
		}
	}

	if(bCloseOnDC){
		if(_CPacket.GetConnState() == false){
			StopBot();
			InitiateSystemShutdown(NULL,"KojdBot PC'nizi kapatiyor...",10,true,false);
		}
	}

	if(bTownOnGM){
		if(_CArea.CheckGM()){
			TownAlert();
			return;
		}
	}

	if(bTownOnMob){
		if(_CArea.GetNPCByName(iAlertMobCount,strAlertMobList,NULL,false,true,true,5.0,false)){
			TownAlert();
			return;
		}
	}

	if(bTownOnNoTS){
		if(_CBag.FindInvItem(0,ITEM_TS_SCROLL) == -1){
			TownAlert();
			return;
		}
	}

	if(bTownOnNoArrow){
		if(_CBag.FindInvItem(0,ITEM_ARROW) == -1){
			TownAlert();
			return;
		}
	}

	if(bTownOnNoMana){
		if(_CBag.FindInvItem(0,dwManaItemNum) == -1){
			TownAlert();
			return;
		}
	}

	if(bTownOnNoParty){
		_CParty.Refresh();
		if(_CParty.GetCount() < 2){
			TownAlert();
			return;
		}
	}

	if(bRingOnDie){
		if(_CCharBase.GetDWORD(KO_CRHP) == 0){
			TownAlert();
			return;
		}
	}
}
//---------------------------------------------------------------------------

void TownAlert(){

	StopBot();

	BYTE pSend[1] = {0x48};
	_CPacket.PSend(pSend,1);

	if(frmBot->bRingOnDie && _CCharBase.GetDWORD(KO_CRHP) == 0){
		if(!frmBot->mciAlarm)
			frmBot->mciAlarm = MCIWndCreate(frmBot->Handle, GetModuleHandle(NULL), WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR | MCIWNDF_NOMENU, "alarm.mp3");
		else
			MCIWndHome(frmBot->mciAlarm);

		MCIWndPlay(frmBot->mciAlarm);
		return;
	}

	if(frmBot->bRingOnTown){
		if(!frmBot->mciAlarm)
			frmBot->mciAlarm = MCIWndCreate(frmBot->Handle, GetModuleHandle(NULL), WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR | MCIWNDF_NOMENU, "alarm.mp3");
		else
			MCIWndHome(frmBot->mciAlarm);

		MCIWndPlay(frmBot->mciAlarm);
	}
}

void __fastcall TfrmBot::btntownmobClick(TObject *Sender)
{
	DWORD dwID = _CCharBase.GetTargetID();

	if(!dwID || dwID == 0xFFFF)
		return;

	CCharBase pMob = _CArea.GetNPCBase(dwID);
	if(!pMob.GetBase())
		return;

	txttownmob->Text = pMob.GetName();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnaddtownmobClick(TObject *Sender)
{
	if(iAlertMobCount == 10)
		return;

	if(!txttownmob->Text.Length() || txttownmob->Text.Length() >= 32)
		return;

	char *pNewName = new char [32];
	memset(pNewName,0,32);

	strcpy(pNewName,txttownmob->Text.c_str());
	strAlertMobList[iAlertMobCount] = pNewName;
	lsttownmobs->AddItem(pNewName,Sender);
	iAlertMobCount++;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btncleartownmobClick(TObject *Sender)
{
	if(!iAlertMobCount)
		return;

	lsttownmobs->Clear();

	for (int i = 0; i < 10; i++) {
		delete [] strAlertMobList[i];
		strAlertMobList[i] = NULL;
	}

	iAlertMobCount = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnstopalarmClick(TObject *Sender)
{
	MCIWndDestroy(mciAlarm);
	mciAlarm = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBot::chlootlistClick(TObject *Sender)
{
	bLootList = chlootlist->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btngetfirstitemClick(TObject *Sender)
{
	CInvSlot pItem;
	pItem.SelectSlot(0);

	char tmp[16];
	ltoa(pItem.dwItemNum(),tmp,10);
	txtlootid->Text = tmp;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnadditemClick(TObject *Sender)
{
	if(iLootCount == 10)
		return;

	long lItemNum = atol(txtlootid->Text.c_str());

	if(!lItemNum)
		return;

	lLootID[iLootCount] = lItemNum;
	iLootCount++;
	lstlootids->AddItem(txtlootid->Text,Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnclearitemsClick(TObject *Sender)
{
	lstlootids->Clear();
	iLootCount = 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chpricelimitClick(TObject *Sender)
{
	bLootPriceLimit = chpricelimit->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtpricelimitChange(TObject *Sender)
{
	iLootPriceLimit = atoi(txtpricelimit->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmsupplyTimer(TObject *Sender)
{
	if(bOnSundWay || bOnPotWay || bOnInnWay)
		return;

	if(CheckLoading() == true)
		return;

	if(_CCharBase.GetDWORD(KO_CRHP) == 0)
		return;

	bool bSundGo = false;
	bool bPotGo = false;
	bool bInnGo = false;

	if(bSund){
		if(iSundCount){
			CInvSlot pLItem;	pLItem.SelectLeft();
			CInvSlot pRItem;	pRItem.SelectRight();
			CInvSlot pItem;

			if((pLItem.GetBase() && iLastDurabilityL != pLItem.iItemDur() &&  pLItem.iItemDur() <= 1000) || (pRItem.GetBase() && iLastDurabilityR != pRItem.iItemDur() && pRItem.iItemDur() <= 1000))
				bSundGo = true;

			if(bBuyWolf){
				pItem = _CBag.FindInvItem(0,ITEM_WOLF);
				if(pItem.IsFree()){
					if(_CBag.FindFreeInvSlot(0) > -1)
						bSundGo = true;
				}else{
					if(pItem.iItemN() < 5)
						bSundGo = true;
					}
			}

			if(bBuyArrow){
				pItem = _CBag.FindInvItem(0,ITEM_ARROW);
				if(pItem.IsFree()){
					if(_CBag.FindFreeInvSlot(0) > -1)
						bSundGo = true;
				}else{
					if(pItem.iItemN() < 100)
						bSundGo = true;
				}
			}

			if(bBuyTS){
				pItem = _CBag.FindInvItem(0,ITEM_TS_GEM);
				if(pItem.IsFree()){
					if(_CBag.FindInvItem(0,ITEM_TS_SCROLL) == -1 && _CBag.FindFreeInvSlot(0) > -1)
						bSundGo = true;
				}
			}

			if(bBuyBook){
				pItem = _CBag.FindInvItem(0,ITEM_BOOK);
				if(pItem.IsFree()){
					if(_CBag.FindFreeInvSlot(0) > -1)
						bSundGo = true;
				}
			}

			if(bBuyRock){
				pItem = _CBag.FindInvItem(0,ITEM_ROCK);
				if(pItem.IsFree()){
					if(_CBag.FindFreeInvSlot(0) > -1)
						bSundGo = true;
				}else{
					if(pItem.iItemN() <= 5)
						bSundGo = true;
				}
			}

			if(bBuyMaster){
				DWORD dwMasterNum = 0;
				switch(iAttackType){

					case 1:	//rogue
					dwMasterNum = ITEM_MASTER_ROGUE;
					break;

					case 2:	//mage
					dwMasterNum = ITEM_MASTER_MAGE;
					break;

					case 3:	//priest
					dwMasterNum = ITEM_MASTER_PRIEST;
					break;

					case 4:	//warrior
					dwMasterNum = ITEM_MASTER_WARRIOR;
					break;
				}

				pItem = _CBag.FindInvItem(0,dwMasterNum);
				if(pItem.IsFree()){
					if(_CBag.FindFreeInvSlot(0) > -1)
						bSundGo = true;
				}else{
					if(pItem.iItemN() <= 5)
						bSundGo = true;
				}
			}

			if(bAutoSell){
            	if(bSellWaitForRPR == false){
					if(_CBag.FindFreeInvSlot(0) == -1)
						bSundGo = true;
				}
			}

		}
	}

	if(bPot){
		if(iPotCount){

		CInvSlot pPot;

			if(bBuyMana){
				if(dwManaItemNum != ITEM_PREPOT_MP && dwManaItemNum != ITEM_DCPOT_MP){	//premium pot
					pPot = _CBag.FindInvItem(0,dwManaItemNum);
					if(pPot.IsFree()){
						if(_CBag.FindFreeInvSlot(0) > -1)
							bPotGo = true;
					}else{
						if(pPot.iItemN() < 5)
							bPotGo = true;
					}
				}
			}

			if(bBuyHP){
				if(dwHPItemNum != ITEM_PREPOT_HP && dwHPItemNum != ITEM_DCPOT_HP){	//premium pot
					pPot = _CBag.FindInvItem(0,dwHPItemNum);
					if(pPot.IsFree()){
						if(_CBag.FindFreeInvSlot(0) > -1)
							bPotGo = true;
					}else{
						if(pPot.iItemN() < 5)
							bPotGo = true;
					}
				}
			}

		}
	}

	if(bInn){
		if(iInnCount){

			if(bInnDepot){
				if(_CBag.FindFreeInvSlot(0) == -1)
					bInnGo = true;
			}

			CInvSlot pPot2;

			if(bGetInnHP)
			{
				pPot2 = _CBag.FindInvItem(0,dwHPItemNum);
				if(pPot2.IsFree()){
					if(_CBag.FindFreeInvSlot(0) > -1)
						bInnGo = true;
				}else{
					if(pPot2.iItemN() < 5)
						bInnGo = true;
				}
			}

			if(bGetInnMP)
			{
				pPot2 = _CBag.FindInvItem(0,dwManaItemNum);
				if(pPot2.IsFree()){
					if(_CBag.FindFreeInvSlot(0) > -1)
						bInnGo = true;
				}else{
					if(pPot2.iItemN() < 5)
						bInnGo = true;
				}
			}

		}
	}

	if(bSundGo){
		bMovePaused = true;
		bLootPaused	= true;
		bBotPaused	= true;
		bOnSundWay = true;
		bFollowPaused = true;

		bWaitingNewDrop = false;
		ClearDrops();

		if(_CCharBase.IsSit())
			_CTest.CommandSit(false);

		if( iAttackType == 1 && !bLF){
			if( _CTest.CheckSkillAvailable(_CCast.MakeCastNum(_CCharBase,"725")) == true){
				bLF = true;
			}
		}

		iSundRoute = 1;	//go
		iSundLastPos = 0;
		tmsundgo->Enabled = true;
		return;
	}

	if(bPotGo){

		bMovePaused = true;
		bLootPaused	= true;
		bBotPaused	= true;
		bOnPotWay = true;
		bFollowPaused = true;

		bWaitingNewDrop = false;
		ClearDrops();

		if(_CCharBase.IsSit())
			_CTest.CommandSit(false);

		if( iAttackType == 1 && !bLF){
			if( _CTest.CheckSkillAvailable(_CCast.MakeCastNum(_CCharBase,"725")) == true){
				bLF = true;
			}
		}

		iPotRoute = 1; //go
		iPotLastPos = 0;
		tmpotgo->Enabled = true;
		return;
	}

	if(bInnGo){

		bMovePaused = true;
		bLootPaused	= true;
		bBotPaused	= true;
		bOnInnWay = true;
		bFollowPaused = true;

		bWaitingNewDrop = false;
		ClearDrops();

		if(_CCharBase.IsSit())
			_CTest.CommandSit(false);

		if( iAttackType == 1 && !bLF){
			if( _CTest.CheckSkillAvailable(_CCast.MakeCastNum(_CCharBase,"725")) == true){
				bLF = true;
			}
		}

		iInnRoute = 1; //go
		iInnLastPos = 0;
		tminngo->Enabled = true;
		return;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmsundgoTimer(TObject *Sender)
{

	if(CheckLoading() == true)
		return;

	if(_CCharBase.GetDWORD(KO_CRHP) == 0)
		return;

	if(_CCharBase.IsSit())
		_CTest.CommandSit(false);

	int iPosX,iPosY;

	iPosX = (int) _CCharBase.GetFLOAT(KO_POSX);
	iPosY = (int) _CCharBase.GetFLOAT(KO_POSY);

	if(iSundRoute == 1){	//gidiþ xd
		if(bRouteSH){
			if(SHTeleport(frmBot->iSundX[iSundLastPos],frmBot->iSundY[iSundLastPos],5))
				iSundLastPos++;
		}else{
			if(MoveToPos(frmBot->iSundX[iSundLastPos],frmBot->iSundY[iSundLastPos]))
				iSundLastPos++;
		}

		if(iSundLastPos == frmBot->iSundCount){
			SundArrived();
			iSundLastPos--;
			iSundRoute = 2;
		}
	}
	else if (iSundRoute == 2){	//geliþ xd

		if(_CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),frmBot->iSundX[0],frmBot->iSundY[0]) <= 5.0)
			iSundLastPos = 0;

		if(bRouteSH){
			if(SHTeleport(frmBot->iSundX[iSundLastPos],frmBot->iSundY[iSundLastPos],5))
				iSundLastPos--;
		}
		else{
			if(MoveToPos(frmBot->iSundX[iSundLastPos],frmBot->iSundY[iSundLastPos]))
				iSundLastPos--;
		}

		if(iSundLastPos == -1){
			SundReturned();
			iSundLastPos++;
			tmsundgo->Enabled = false;
			return;
		}
	}
}
//---------------------------------------------------------------------------

void SundArrived(){

	CCharBase pSund = _CArea.GetNPCByName(1,NULL,frmBot->txtsund->Text.c_str(),false,false,false,3,false);
	if(!pSund.GetBase())
		return;

	char pSend[20];
	_CPacket.PutBYTE(pSend,0x3B);
	_CPacket.PutBYTE(pSend+1,0x01);

	CInvSlot pLItem;
	CInvSlot pRItem;
	CInvSlot pItem;
	DWORD	dwItemBase;

	pLItem.SelectLeft();
	if(pLItem.GetBase()){
		_CPacket.PutBYTE(pSend+2,0x08);
		_CPacket.PutWORD(pSend+3,pSund.GetID());
		_CPacket.PutDWORD(pSend+5,pLItem.dwItemNum());
		_CPacket.PSend(pSend,9);
		Sleep(100);
	}

	pRItem.SelectRight();
	if(pRItem.GetBase()){
		_CPacket.PutBYTE(pSend+2,0x06);
		_CPacket.PutWORD(pSend+3,pSund.GetID());
		_CPacket.PutDWORD(pSend+5,pRItem.dwItemNum());
		_CPacket.PSend(pSend,9);
		Sleep(100);
	}


	if(pLItem.GetBase())
		frmBot->iLastDurabilityL = pLItem.iItemDur();
	else
		frmBot->iLastDurabilityL = -1;

	if(pRItem.GetBase())
		frmBot->iLastDurabilityR = pRItem.iItemDur();
	else
		frmBot->iLastDurabilityR = -1;

	pItem.SelectHelmet();
	if(pItem.GetBase()){
		_CPacket.PutBYTE(pSend+2,1);
		_CPacket.PutWORD(pSend+3,pSund.GetID());
		_CPacket.PutDWORD(pSend+5,pItem.dwItemNum());
		_CPacket.PSend(pSend,9);
		Sleep(100);
	}

	pItem.SelectPauldron();
	if(pItem.GetBase()){
		_CPacket.PutBYTE(pSend+2,4);
		_CPacket.PutWORD(pSend+3,pSund.GetID());
		_CPacket.PutDWORD(pSend+5,pItem.dwItemNum());
		_CPacket.PSend(pSend,9);
		Sleep(100);
	}

	pItem.SelectPads();
	if(pItem.GetBase()){
		_CPacket.PutBYTE(pSend+2,10);
		_CPacket.PutWORD(pSend+3,pSund.GetID());
		_CPacket.PutDWORD(pSend+5,pItem.dwItemNum());
		_CPacket.PSend(pSend,9);
		Sleep(100);
	}

	pItem.SelectGloves();
	if(pItem.GetBase()){
		_CPacket.PutBYTE(pSend+2,12);
		_CPacket.PutWORD(pSend+3,pSund.GetID());
		_CPacket.PutDWORD(pSend+5,pItem.dwItemNum());
		_CPacket.PSend(pSend,9);
		Sleep(100);
	}

	pItem.SelectBoots();
	if(pItem.GetBase()){
		_CPacket.PutBYTE(pSend+2,13);
		_CPacket.PutWORD(pSend+3,pSund.GetID());
		_CPacket.PutDWORD(pSend+5,pItem.dwItemNum());
		_CPacket.PSend(pSend,9);
		Sleep(100);
	}

	if(frmBot->bAutoSell){
		_CPacket.PutBYTE(pSend,0x21);
		_CPacket.PutBYTE(pSend+1,0x02);
		_CPacket.PutDWORD(pSend+2,255000);
		_CPacket.PutWORD(pSend+6,pSund.GetID());
		for (int i = (frmBot->iSellBeginSlot - 1); i < iMaxInventorySlot; i++) {
			pItem = i;
			if(pItem.IsFree())
				continue;

			if(frmBot->bSellPriceLimitAbove || frmBot->bSellPriceLimitLess){
				dwItemBase = _CBag.GetItemBase(pItem.dwItemNum());
				if(dwItemBase){
					if(frmBot->bSellPriceLimitAbove){
						if(*(DWORD*)(dwItemBase + 0x50) < (DWORD)frmBot->iSellPriceLimitAbove)
							continue;
					}

					if(frmBot->bSellPriceLimitLess){
						if(*(DWORD*)(dwItemBase + 0x50) > (DWORD)frmBot->iSellPriceLimitLess)
							continue;
					}
				}
			}

			_CPacket.PutDWORD(pSend+8,pItem.dwItemNum());
			_CPacket.PutBYTE(pSend+12,i);
			_CPacket.PutWORD(pSend+13,pItem.iItemN());
			_CPacket.PSend(pSend,15);
			Sleep(10);
		}
	}

	if(frmBot->bBuyWolf){
		int iWolfSlot = _CBag.FindInvItem(0,ITEM_WOLF);

		if(iWolfSlot == -1)
			iWolfSlot = _CBag.FindFreeInvSlot(0);

		if(iWolfSlot > -1){
			int iWolfN;
			pItem = iWolfSlot;
			if(pItem.IsFree())
				iWolfN = 100;
			else
				iWolfN = 100 - pItem.iItemN();

			if(iWolfN){
				_CPacket.PutBYTE(pSend,0x21);
				_CPacket.PutBYTE(pSend+1,0x01);
				_CPacket.PutDWORD(pSend+2,255000);
				_CPacket.PutWORD(pSend+6,pSund.GetID());
				_CPacket.PutDWORD(pSend+8,ITEM_WOLF);
				_CPacket.PutBYTE(pSend+12,iWolfSlot);
				_CPacket.PutWORD(pSend+13,iWolfN);
				_CPacket.PutBYTE(pSend+15,0x00);
				_CPacket.PutBYTE(pSend+16,0x07);
				_CPacket.PSend(pSend,17);
				Sleep(100);
			}
		}
	}

	if(frmBot->bBuyArrow){
		int iArrowSlot = _CBag.FindInvItem(0,ITEM_ARROW);

		if(iArrowSlot == -1)
			iArrowSlot = _CBag.FindFreeInvSlot(0);

		if(iArrowSlot > -1){
			int iArrowN;
			pItem = iArrowSlot;
			if(pItem.IsFree())
				iArrowN = 9000;
			else
				iArrowN = 9000 - pItem.iItemN();

			if(iArrowN){
				_CPacket.PutBYTE(pSend,0x21);
				_CPacket.PutBYTE(pSend+1,0x01);
				_CPacket.PutDWORD(pSend+2,255000);
				_CPacket.PutWORD(pSend+6,pSund.GetID());
				_CPacket.PutDWORD(pSend+8,ITEM_ARROW);
				_CPacket.PutBYTE(pSend+12,iArrowSlot);
				_CPacket.PutBYTE(pSend+15,0x00);
				_CPacket.PutBYTE(pSend+16,0x00);
				while(iArrowN >= 1000){
					_CPacket.PutWORD(pSend+13,1000);
					_CPacket.PSend(pSend,17);
					iArrowN -= 1000;
					Sleep(100);
				}
				if(iArrowN){
					_CPacket.PutWORD(pSend+13,iArrowN);
					_CPacket.PSend(pSend,17);
					Sleep(100);
				}
			}
		}
	}

	if(frmBot->bBuyTS){
		int iTSSlot = _CBag.FindInvItem(0,ITEM_TS_GEM);
		bool bScroll = false;

		if(iTSSlot == -1){
			iTSSlot = _CBag.FindInvItem(0,ITEM_TS_SCROLL);
			bScroll = true;
		}

		if(iTSSlot == -1){
			iTSSlot = _CBag.FindFreeInvSlot(0);
			bScroll = false;
		}

		if(iTSSlot > -1){
			int iTSN;
			pItem = iTSSlot;
			if(pItem.IsFree())
				iTSN = frmBot->iBuyTS;
			else
				iTSN = frmBot->iBuyTS - pItem.iItemN();

			if(iTSN){
				_CPacket.PutBYTE(pSend,0x21);
				_CPacket.PutBYTE(pSend+1,0x01);
				_CPacket.PutDWORD(pSend+2,255000);
				_CPacket.PutWORD(pSend+6,pSund.GetID());
				if(bScroll)
					_CPacket.PutDWORD(pSend+8,ITEM_TS_SCROLL);
				else
					_CPacket.PutDWORD(pSend+8,ITEM_TS_GEM);
				_CPacket.PutBYTE(pSend+12,iTSSlot);
				_CPacket.PutWORD(pSend+13,iTSN);
				if(bScroll){
					_CPacket.PutBYTE(pSend+15,0x00);
					_CPacket.PutBYTE(pSend+16,0x13);
				}else{
					_CPacket.PutBYTE(pSend+15,0x01);
					_CPacket.PutBYTE(pSend+16,0x02);
				}
				_CPacket.PSend(pSend,17);
				Sleep(100);
			}
		}
	}

	if(frmBot->bBuyBook){
		int iBookSlot = _CBag.FindInvItem(0,ITEM_BOOK);

		if(iBookSlot == -1)
			iBookSlot = _CBag.FindFreeInvSlot(0);

		if(iBookSlot > -1){
			int iBookN;
			pItem = iBookSlot;
			if(pItem.IsFree())
				iBookN = frmBot->iBuyBook;
			else
				iBookN = frmBot->iBuyBook - pItem.iItemN();

			if(iBookN){
				_CPacket.PutBYTE(pSend,0x21);
				_CPacket.PutBYTE(pSend+1,0x01);
				_CPacket.PutDWORD(pSend+2,255000);
				_CPacket.PutWORD(pSend+6,pSund.GetID());
				_CPacket.PutDWORD(pSend+8,ITEM_BOOK);
				_CPacket.PutBYTE(pSend+12,iBookSlot);
				_CPacket.PutWORD(pSend+13,iBookN);
				_CPacket.PutBYTE(pSend+15,0x00);
				_CPacket.PutBYTE(pSend+16,0x0C);
				_CPacket.PSend(pSend,17);
				Sleep(100);
			}
		}
	}

	if(frmBot->bBuyRock){
		int iRockSlot = _CBag.FindInvItem(0,ITEM_ROCK);

		if(iRockSlot == -1)
			iRockSlot = _CBag.FindFreeInvSlot(0);

		if(iRockSlot > -1){
			int iRockN;
			pItem = iRockSlot;
			if(pItem.IsFree())
				iRockN = frmBot->iBuyRock;
			else
				iRockN = frmBot->iBuyRock - pItem.iItemN();

			if(iRockN){
				_CPacket.PutBYTE(pSend,0x21);
				_CPacket.PutBYTE(pSend+1,0x01);
				_CPacket.PutDWORD(pSend+2,255000);
				_CPacket.PutWORD(pSend+6,pSund.GetID());
				_CPacket.PutDWORD(pSend+8,ITEM_ROCK);
				_CPacket.PutBYTE(pSend+12,iRockSlot);
				_CPacket.PutBYTE(pSend+15,0x00);
				_CPacket.PutBYTE(pSend+16,0x11);
				while(iRockN >= 1000){
					_CPacket.PutWORD(pSend+13,1000);
					_CPacket.PSend(pSend,17);
					iRockN -= 1000;
					Sleep(100);
				}
				if(iRockN){
					_CPacket.PutWORD(pSend+13,iRockN);
					_CPacket.PSend(pSend,17);
					Sleep(100);
				}
			}
		}
	}

	if(frmBot->bBuyMaster){

		DWORD dwMasterNum = 0;
		BYTE bItemSellingSlot;
		switch(iAttackType){

			case 1:	//rogue
			dwMasterNum = ITEM_MASTER_ROGUE;
			bItemSellingSlot = 0x15;
			break;

			case 2:	//mage
			dwMasterNum = ITEM_MASTER_MAGE;
			bItemSellingSlot = 0x16;
			break;

			case 3:	//priest
			dwMasterNum = ITEM_MASTER_PRIEST;
			bItemSellingSlot = 0x17;
			break;

			case 4:	//warrior
			dwMasterNum = ITEM_MASTER_WARRIOR;
			bItemSellingSlot = 0x14;
			break;
		}


		int iMasterSlot = _CBag.FindInvItem(0,dwMasterNum);

		if(iMasterSlot == -1)
			iMasterSlot = _CBag.FindFreeInvSlot(0);

		if(iMasterSlot > -1){
			int iMasterN;
			pItem = iMasterSlot;
			if(pItem.IsFree())
				iMasterN = frmBot->iBuyMaster;
			else
				iMasterN = frmBot->iBuyMaster - pItem.iItemN();

			if(iMasterN){
				_CPacket.PutBYTE(pSend,0x21);
				_CPacket.PutBYTE(pSend+1,0x01);
				_CPacket.PutDWORD(pSend+2,255000);
				_CPacket.PutWORD(pSend+6,pSund.GetID());
				_CPacket.PutDWORD(pSend+8,dwMasterNum);
				_CPacket.PutBYTE(pSend+12,iMasterSlot);
				_CPacket.PutBYTE(pSend+15,0x00);
				_CPacket.PutBYTE(pSend+16,bItemSellingSlot);
				while(iMasterN >= 1000){
					_CPacket.PutWORD(pSend+13,1000);
					_CPacket.PSend(pSend,17);
					iMasterN -= 1000;
					Sleep(100);
				}
				if(iMasterN){
					_CPacket.PutWORD(pSend+13,iMasterN);
					_CPacket.PSend(pSend,17);
					Sleep(100);
				}
			}
		}
	}

	_CPacket.PutBYTE(pSend,0x6A);
	_CPacket.PutBYTE(pSend+1,0x02);
	_CPacket.PSend(pSend,2);

	if(frmBot->bInformAfterSupply)
		SupplyChat();
}

void SundReturned(){
	frmBot->bLF = frmBot->chlf->Checked;
	frmBot->bOnSundWay = false;
	frmBot->bMovePaused = false;
	frmBot->bLootPaused	= false;
	frmBot->bBotPaused	= false;
	frmBot->bFollowPaused = false;
	ClearDrops();
}


void __fastcall TfrmBot::tmpotgoTimer(TObject *Sender)
{

	if(CheckLoading() == true)
		return;

	if(_CCharBase.GetDWORD(KO_CRHP) == 0)
		return;

	if(_CCharBase.IsSit())
		_CTest.CommandSit(false);

	int iPosX,iPosY;

	iPosX = (int) _CCharBase.GetFLOAT(KO_POSX);
	iPosY = (int) _CCharBase.GetFLOAT(KO_POSY);

	if(iPotRoute == 1){	//gidiþ xd
		if(bRouteSH){
			if(SHTeleport(frmBot->iPotX[iPotLastPos],frmBot->iPotY[iPotLastPos],5))
				iPotLastPos++;
		}else{
			if(MoveToPos(frmBot->iPotX[iPotLastPos],frmBot->iPotY[iPotLastPos]))
				iPotLastPos++;
        }

		if(iPotLastPos == frmBot->iPotCount){
			PotArrived();
			iPotLastPos--;
			iPotRoute = 2;
		}
	}
	else if (iPotRoute == 2){	//geliþ xd

		if(_CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),frmBot->iPotX[0],frmBot->iPotY[0]) <= 5.0)
			iPotLastPos = 0;

		if(bRouteSH){
			if(SHTeleport(frmBot->iPotX[iPotLastPos],frmBot->iPotY[iPotLastPos],5))
				iPotLastPos--;
		}else{
			if(MoveToPos(frmBot->iPotX[iPotLastPos],frmBot->iPotY[iPotLastPos]))
				iPotLastPos--;
        }

		if(iPotLastPos == -1){
			PotReturned();
			iPotLastPos++;
			tmpotgo->Enabled = false;
			return;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tminngoTimer(TObject *Sender)
{

	if(CheckLoading() == true)
		return;

	if(_CCharBase.GetDWORD(KO_CRHP) == 0)
		return;

	if(_CCharBase.IsSit())
		_CTest.CommandSit(false);

	int iPosX,iPosY;

	iPosX = (int) _CCharBase.GetFLOAT(KO_POSX);
	iPosY = (int) _CCharBase.GetFLOAT(KO_POSY);

	if(iInnRoute == 1){	//gidiþ xd
		if(bRouteSH){
			if(SHTeleport(frmBot->iInnX[iInnLastPos],frmBot->iInnY[iInnLastPos],5))
				iInnLastPos++;
		}else{
			if(MoveToPos(frmBot->iInnX[iInnLastPos],frmBot->iInnY[iInnLastPos]))
				iInnLastPos++;
        }

		if(iInnLastPos == frmBot->iInnCount){
			InnArrived();
			iInnLastPos--;
			iInnRoute = 2;
		}
	}
	else if (iInnRoute == 2){	//geliþ xd

		if(_CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),frmBot->iInnX[0],frmBot->iInnY[0]) <= 5.0)
			iInnLastPos = 0;

		if(bRouteSH){
			if(SHTeleport(frmBot->iInnX[iInnLastPos],frmBot->iInnY[iInnLastPos],5))
				iInnLastPos--;
		}else{
			if(MoveToPos(frmBot->iInnX[iInnLastPos],frmBot->iInnY[iInnLastPos]))
				iInnLastPos--;
        }

		if(iInnLastPos == -1){
			InnReturned();
			iInnLastPos++;
			tminngo->Enabled = false;
			return;
		}
	}
}
//---------------------------------------------------------------------------

void PotArrived(){
	CCharBase pPot = _CArea.GetNPCByName(1,NULL,frmBot->txtpot->Text.c_str(),false,false,false,3,false);
	if(!pPot.GetBase())
		return;

	int iPotSellSlot,iPotSlot,iPotN;
	char pSend[20];

	if(frmBot->bBuyMana){
		switch(frmBot->iManaType){
			case 0:
			iPotSellSlot = 6;
			break;

			case 1:
			iPotSellSlot = 7;
			break;

			case 2:
			iPotSellSlot = 8;
			break;

			case 3:
			iPotSellSlot = 9;
			break;

			case 4:
			iPotSellSlot = 10;
			break;

			default:
			iPotSellSlot = -1;
			break;

		}

		iPotSlot = _CBag.FindInvItem(0,frmBot->dwManaItemNum);

		if(iPotSlot == -1)
			iPotSlot = _CBag.FindFreeInvSlot(0);

		if(iPotSlot > -1){
			CInvSlot pItem = iPotSlot;
			if(pItem.IsFree())
				iPotN = frmBot->iBuyManaLimit;
			else
				iPotN = frmBot->iBuyManaLimit - pItem.iItemN();

			if(iPotN){
				_CPacket.PutBYTE(pSend,0x21);
				_CPacket.PutBYTE(pSend+1,0x01);
				_CPacket.PutDWORD(pSend+2,253000);
				_CPacket.PutWORD(pSend+6,pPot.GetID());
				_CPacket.PutDWORD(pSend+8,frmBot->dwManaItemNum);
				_CPacket.PutBYTE(pSend+12,iPotSlot);
				_CPacket.PutWORD(pSend+13,iPotN);
				_CPacket.PutBYTE(pSend+15,0x00);
				_CPacket.PutBYTE(pSend+16,iPotSellSlot);
				_CPacket.PSend(pSend,17);
				Sleep(100);
			}
		}
	}

	if(frmBot->bBuyHP){
		switch(frmBot->iHPType){

			case 0:
			iPotSellSlot = 0;
			break;

			case 1:
			iPotSellSlot = 1;
			break;

			case 2:
			iPotSellSlot = 2;
			break;

			case 3:
			iPotSellSlot = 3;
			break;

			case 4:
			iPotSellSlot = 4;
			break;

			default:
			iPotSellSlot = -1;
			break;

		}

		iPotSlot = _CBag.FindInvItem(0,frmBot->dwHPItemNum);

		if(iPotSlot == -1)
			iPotSlot = _CBag.FindFreeInvSlot(0);

		if(iPotSlot > -1){
			CInvSlot pItem = iPotSlot;
			if(pItem.IsFree())
				iPotN = frmBot->iBuyHPLimit;
			else
				iPotN = frmBot->iBuyHPLimit - pItem.iItemN();

			if(iPotN){
				_CPacket.PutBYTE(pSend,0x21);
				_CPacket.PutBYTE(pSend+1,0x01);
				_CPacket.PutDWORD(pSend+2,253000);
				_CPacket.PutWORD(pSend+6,pPot.GetID());
				_CPacket.PutDWORD(pSend+8,frmBot->dwHPItemNum);
				_CPacket.PutBYTE(pSend+12,iPotSlot);
				_CPacket.PutWORD(pSend+13,iPotN);
				_CPacket.PutBYTE(pSend+15,0x00);
				_CPacket.PutBYTE(pSend+16,iPotSellSlot);
				_CPacket.PSend(pSend,17);
				Sleep(100);
			}
		}
	}

	_CPacket.PutBYTE(pSend,0x6A);
	_CPacket.PutBYTE(pSend+1,0x02);
	_CPacket.PSend(pSend,2);

	if(frmBot->bInformAfterSupply)
		SupplyChat();

}

void PotReturned(){
	frmBot->bLF = frmBot->chlf->Checked;
	frmBot->bOnPotWay = false;
	frmBot->bMovePaused = false;
	frmBot->bLootPaused	= false;
	frmBot->bBotPaused	= false;
	frmBot->bFollowPaused = false;
	ClearDrops();
}

void InnArrived(){
	CCharBase pInn = _CArea.GetNPCByName(1,NULL,frmBot->txtinn->Text.c_str(),false,false,false,3,false);
	if(!pInn.GetBase())
		return;

	char pSend[16];
	long lArriveTick = GetTickCount();
	bool bInnLaunched = false;

	while(GetTickCount() - lArriveTick < 10000){
		_CPacket.PutBYTE(pSend,0x45);
		_CPacket.PutBYTE(pSend+1,0x01);
		_CPacket.PutWORD(pSend+2,pInn.GetID());
		_CPacket.PutDWORD(pSend+4,0xFFFFFFFF);
		_CPacket.PSend(pSend,8);

		Sleep(3000);
		bInnLaunched = *(BYTE*)(*(DWORD*)(*(DWORD*)(KO_DLGA) + OFF_WAREHOUSE) + 0xC0);
		if(bInnLaunched)
			break;
	}

	if(bInnLaunched)
		CloseInnDlg();
	else
		return;

	if(frmBot->bInnDepot){
		_CPacket.PutBYTE(pSend,0x45);
		_CPacket.PutBYTE(pSend+1,0x02);
		_CPacket.PutWORD(pSend+2,pInn.GetID());

		CInvSlot pInvSlot; CInnSlot pInnSlot;
		int iTotalInnSlot = 0,iInnSlot = 0,iInnPage = 0,iFreeInnSlot = 0,iQuantity = 0;

		for (int i = (frmBot->iInnBeginSlot - 1); i < iMaxInventorySlot; i++) {
			pInvSlot = i;

			if(pInvSlot.IsFree())
				continue;

			pInnSlot = _CBag.FindInnItem(0,pInvSlot.dwItemNum());
			if(pInnSlot.IsFree() || pInnSlot.iItemN() <= 1 || pInnSlot.iItemN() + pInvSlot.iItemN() > 9998){
				iTotalInnSlot = _CBag.FindFreeInnSlot(iFreeInnSlot);
				iFreeInnSlot = iTotalInnSlot + 1;
			}
			else
				iTotalInnSlot = pInnSlot.GetCurrentSlot();

			if(iTotalInnSlot == -1)
				break;
			_CBag.SlotToPaged(iTotalInnSlot,iInnPage,iInnSlot);

			_CPacket.PutDWORD(pSend+4,pInvSlot.dwItemNum());
			_CPacket.PutBYTE(pSend+8,iInnPage);
			_CPacket.PutBYTE(pSend+9,i);
			_CPacket.PutBYTE(pSend+10,iInnSlot);
			_CPacket.PutDWORD(pSend+11,pInvSlot.iItemN());
			_CPacket.PSend(pSend,15);
			Sleep(10);
		}
	}

	int iTmpPage,iTmpSlot;

	if(frmBot->bGetInnHP)
	{
		_CPacket.PutBYTE(pSend,0x45);
		_CPacket.PutBYTE(pSend+1,0x03);
		_CPacket.PutWORD(pSend+2,pInn.GetID());

		CInnSlot pInnHPSlot = _CBag.FindInnItem(0,frmBot->dwHPItemNum);
		CInnSlot pInvHPSlot = _CBag.FindInvItem(0,frmBot->dwHPItemNum);

		if(pInnHPSlot.GetBase() > 0)
		{
			int i_nHP = frmBot->iGetInnHP;
			if(pInvHPSlot.GetBase() > 0)
			{
				i_nHP -= pInvHPSlot.iItemN();
			}

			int iInnHPSlot,iInvHPSlot;
			iInnHPSlot = pInnHPSlot.GetCurrentSlot();
			if(pInvHPSlot.GetBase() > 0)
				iInvHPSlot = pInvHPSlot.GetCurrentSlot();
			else
				iInvHPSlot = _CBag.FindFreeInvSlot(0);

			if(iInvHPSlot > -1 && i_nHP > 0)
			{
				_CBag.SlotToPaged(iInnHPSlot,iTmpPage,iTmpSlot);
				*(DWORD*)(pSend+4) = frmBot->dwHPItemNum;
				*(BYTE*)(pSend+8) = iTmpPage;
				*(BYTE*)(pSend+9) = iTmpSlot;
				*(BYTE*)(pSend+10) = iInvHPSlot;
				*(DWORD*)(pSend+11) = i_nHP;
				_CPacket.PSend(pSend,15);
			}
		}
	}

	if(frmBot->bGetInnMP)
	{
		_CPacket.PutBYTE(pSend,0x45);
		_CPacket.PutBYTE(pSend+1,0x03);
		_CPacket.PutWORD(pSend+2,pInn.GetID());

		CInnSlot pInnMPSlot = _CBag.FindInnItem(0,frmBot->dwManaItemNum);
		CInnSlot pInvMPSlot = _CBag.FindInvItem(0,frmBot->dwManaItemNum);

		if(pInnMPSlot.GetBase() > 0)
		{
			int i_nMP = frmBot->iGetInnMP;
			if(pInvMPSlot.GetBase() > 0)
			{
				i_nMP -= pInvMPSlot.iItemN();
			}

			int iInnMPSlot,iInvMPSlot;
			iInnMPSlot = pInnMPSlot.GetCurrentSlot();
			if(pInvMPSlot.GetBase() > 0)
				iInvMPSlot = pInvMPSlot.GetCurrentSlot();
			else
				iInvMPSlot = _CBag.FindFreeInvSlot(0);

			if(iInvMPSlot > -1 && i_nMP > 0)
			{
				_CBag.SlotToPaged(iInnMPSlot,iTmpPage,iTmpSlot);
				*(DWORD*)(pSend+4) = frmBot->dwManaItemNum;
				*(BYTE*)(pSend+8) = iTmpPage;
				*(BYTE*)(pSend+9) = iTmpSlot;
				*(BYTE*)(pSend+10) = iInvMPSlot;
				*(DWORD*)(pSend+11) = i_nMP;
				_CPacket.PSend(pSend,15);
			}
		}
	}

	_CPacket.PutBYTE(pSend,0x6A);
	_CPacket.PutBYTE(pSend+1,0x02);
	_CPacket.PSend(pSend,2);

	if(frmBot->bInformAfterSupply)
		SupplyChat();
}

void InnReturned(){
	frmBot->bLF = frmBot->chlf->Checked;
	frmBot->bOnInnWay = false;
	frmBot->bMovePaused = false;
	frmBot->bLootPaused	= false;
	frmBot->bBotPaused	= false;
	frmBot->bFollowPaused = false;
	ClearDrops();
}


void __fastcall TfrmBot::btnsundClick(TObject *Sender)
{
	DWORD dwID = _CCharBase.GetTargetID();

	if(!dwID || dwID == 0xFFFF)
		return;

	CCharBase pNpc = _CArea.GetNPCBase(dwID);
	if(!pNpc.GetBase())
		return;

	txtsund->Text = pNpc.GetName();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnpotClick(TObject *Sender)
{
	DWORD dwID = _CCharBase.GetTargetID();

	if(!dwID || dwID == 0xFFFF)
		return;

	CCharBase pNpc = _CArea.GetNPCBase(dwID);
	if(!pNpc.GetBase())
		return;

	txtpot->Text = pNpc.GetName();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btninnClick(TObject *Sender)
{
	DWORD dwID = _CCharBase.GetTargetID();

	if(!dwID || dwID == 0xFFFF)
		return;

	CCharBase pNpc = _CArea.GetNPCBase(dwID);
	if(!pNpc.GetBase())
		return;

	txtinn->Text = pNpc.GetName();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnsundaddClick(TObject *Sender)
{
	if(iSundCount == 20)
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

	lstsund->AddItem(buf,Sender);

	iSundX[iSundCount] = (int)fMyX;
	iSundY[iSundCount] = (int)fMyY;
	iSundCount++;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnaddpotClick(TObject *Sender)
{
	if(iPotCount == 20)
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

	lstpot->AddItem(buf,Sender);

	iPotX[iPotCount] = (int)fMyX;
	iPotY[iPotCount] = (int)fMyY;
	iPotCount++;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnaddinnClick(TObject *Sender)
{
	if(iInnCount == 20)
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

	lstinn->AddItem(buf,Sender);

	iInnX[iInnCount] = (int)fMyX;
	iInnY[iInnCount] = (int)fMyY;
	iInnCount++;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnsundclearClick(TObject *Sender)
{
	iSundCount = 0;
	lstsund->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnclearpotClick(TObject *Sender)
{
	iPotCount = 0;
	lstpot->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnclearinnClick(TObject *Sender)
{
	iInnCount = 0;
	lstinn->Clear();
}
//---------------------------------------------------------------------------

void SupplyChat(){
	int iChatSize = frmBot->txtinformsupply->Text.Length();

	if( !iChatSize )
		return;

	char *pSend = new char[iChatSize + 4];

	_CPacket.PutBYTE(pSend,0x10);
	_CPacket.PutBYTE(pSend+1,0x03);
	_CPacket.PutWORD(pSend+2,iChatSize);
	_CPacket.PutSTR(pSend+4,frmBot->txtinformsupply->Text.c_str(),iChatSize);
	_CPacket.PSend(pSend,iChatSize+4);

	delete [] pSend;
}

void LoadSund(){
	char Buffer[16];
	char NameBuffer[64];
	ifstream fSund (frmBot->txtsundsave->Text.c_str());
	if(!fSund.is_open()) return;

	frmBot->lstsund->Clear();
	frmBot->iSundCount = 0;

	memset(Buffer,0,16);
	memset(NameBuffer,0,64);
	bool retLine;
	int iBufferLen;

	fSund.getline(NameBuffer,63);
	frmBot->txtsund->Text = NameBuffer;
	// 1455,186

	int iXLen;
	char tmp1[16], tmp2[16];

	while(1){
		retLine = fSund.getline(Buffer,15);
		if(!retLine)break;
		iBufferLen = strlen(Buffer);
		if(!iBufferLen) continue;
		frmBot->lstsund->AddItem(Buffer,NULL);

		memset(tmp1,0,16);
		memset(tmp2,0,16);

		iXLen = (strchr(Buffer,',') - Buffer);
		memcpy(tmp1,Buffer,iXLen);

		frmBot->iSundX[frmBot->iSundCount] = atoi(tmp1);

		memcpy(tmp2,Buffer+iXLen+1,iBufferLen-(iXLen+1));
		frmBot->iSundY[frmBot->iSundCount] = atoi(tmp2);

		frmBot->iSundCount++;
	}
	fSund.close();
}

void LoadPot(){
	char Buffer[16];
	char NameBuffer[64];
	ifstream fPot (frmBot->txtpotsave->Text.c_str());
	if(!fPot.is_open()) return;

	frmBot->lstpot->Clear();
	frmBot->iPotCount = 0;

	memset(Buffer,0,16);
	memset(NameBuffer,0,64);
	bool retLine;
	int iBufferLen;

	fPot.getline(NameBuffer,63);
	frmBot->txtpot->Text = NameBuffer;

	int iXLen;
	char tmp1[16], tmp2[16];

	while(1){
		retLine = fPot.getline(Buffer,15);
		if(!retLine)break;
		iBufferLen = strlen(Buffer);
		if(!iBufferLen) continue;
		frmBot->lstpot->AddItem(Buffer,NULL);

		memset(tmp1,0,16);
		memset(tmp2,0,16);

		iXLen = (strchr(Buffer,',') - Buffer);
		memcpy(tmp1,Buffer,iXLen);

		frmBot->iPotX[frmBot->iPotCount] = atoi(tmp1);

		memcpy(tmp2,Buffer+iXLen+1,iBufferLen-(iXLen+1));
		frmBot->iPotY[frmBot->iPotCount] = atoi(tmp2);

		frmBot->iPotCount++;
	}
	fPot.close();
}

void LoadInn(){
	char Buffer[16];
	char NameBuffer[64];
	ifstream fInn (frmBot->txtinnsave->Text.c_str());
	if(!fInn.is_open()) return;

	frmBot->lstinn->Clear();
	frmBot->iInnCount = 0;

	memset(Buffer,0,16);
	memset(NameBuffer,0,64);
	bool retLine;
	int iBufferLen;

	fInn.getline(NameBuffer,63);
	frmBot->txtinn->Text = NameBuffer;

	int iXLen;
	char tmp1[16], tmp2[16];

	while(1){
		retLine = fInn.getline(Buffer,15);
		if(!retLine)break;
		iBufferLen = strlen(Buffer);
		if(!iBufferLen) continue;
		frmBot->lstinn->AddItem(Buffer,NULL);

		memset(tmp1,0,16);
		memset(tmp2,0,16);

		iXLen = (strchr(Buffer,',') - Buffer);
		memcpy(tmp1,Buffer,iXLen);

		frmBot->iInnX[frmBot->iInnCount] = atoi(tmp1);

		memcpy(tmp2,Buffer+iXLen+1,iBufferLen-(iXLen+1));
		frmBot->iInnY[frmBot->iInnCount] = atoi(tmp2);

		frmBot->iInnCount++;
	}
	fInn.close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnsundsaveClick(TObject *Sender)
{
	pnsavesund->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnsavepotClick(TObject *Sender)
{
	pnpotsave->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnsaveinnClick(TObject *Sender)
{
	pninnsave->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtreportChange(TObject *Sender)
{
	int iRemainingCharacters = 128 - txtreport->Text.Length();
	char tmp[4]; itoa(iRemainingCharacters,tmp,10);
	lbreportcount->Caption = tmp;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnreportClick(TObject *Sender)
{
	if(!txtreport->Text.Length())
		return;

	int iReportSize = txtreport->Text.Length();

	if(iReportSize > 128)
		return;

	char* strReport = txtreport->Text.c_str();

	for (int i = 0; i < iReportSize; i++)
	{
		if( *(strReport + i) == '%' ||
			*(strReport + i) == '#' ||
			*(strReport + i) == '$' ||
			*(strReport + i) == '^' ||
			*(strReport + i) == ';' ||
			*(strReport + i) == '='	||
			*(strReport + i) == 0x22 ||
			*(strReport + i) == 0x27 ||
			*(strReport + i) == '@' ||
			*(strReport + i) == '>' ||
			*(strReport + i) == '|' ||
			*(strReport + i) == '[' ||
			*(strReport + i) == ']' )
		{
			*(strReport + i) = '_';
        }
	}

	CMainSocket = _CWinsock.NewSocket();
	if((long)CMainSocket.GetSocket() == INVALID_SOCKET){
		MessageBox(frmBot->Handle,"report yapilamadi! socket failed.","Hata",MB_ICONEXCLAMATION);
		return;
	}

	int iRetry = 0;

	while(1){
		if(CMainSocket.Client_Connect(strServerIP,iServerPort))
			break;

		if(iRetry == 5){
			CMainSocket.Close();
			MessageBox(frmBot->Handle,"report yapilamadi! baglanti yok!","Hata",MB_ICONEXCLAMATION);
			return;
		}

		iRetry++;
		Sleep(100);
	}

	int iSendSize = iReportSize + 8;
	char *pSend = new char[iSendSize];

	memset(pSend,0,iSendSize);

	int index(0);
	CMainSocket.PutPacketHead(pSend,index);
	CMainSocket.PutBYTE(pSend,bSend_REPORT,index);
	CMainSocket.PutWORD(pSend,iMyUserID,index);
	CMainSocket.PutBYTE(pSend,iReportSize,index);
	CMainSocket.PutSTR(pSend,strReport,iReportSize,index);
	CMainSocket.PutPacketTail(pSend,index);
	CMainSocket.Client_Send(pSend,iSendSize);
	CMainSocket.Close();
	delete [] pSend;
	MessageBox(frmBot->Handle,"Report yapildi.","Report",MB_ICONINFORMATION);
	btnreport->Enabled = false;
	btnreport->Caption = "60";
	tmreport->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmreportTimer(TObject *Sender)
{
	int iSeconds = atoi(btnreport->Caption.c_str());
	iSeconds--;
	char tmp[4];
	itoa(iSeconds,tmp,10);
	btnreport->Caption = tmp;

	if (!iSeconds){
		btnreport->Enabled = true;
		btnreport->Caption = "rapor et";
		tmreport->Enabled = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chhpClick(TObject *Sender)
{
		bHP = chhp->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::comhpChange(TObject *Sender)
{
	iHPType = comhp->ItemIndex;

	switch(iHPType){

		case 0:
		dwHPSkillNum = SKILL_HP45;
		dwHPItemNum = ITEM_HP45;
		break;

		case 1:
		dwHPSkillNum = SKILL_HP90;
		dwHPItemNum = ITEM_HP90;
		break;

		case 2:
		dwHPSkillNum = SKILL_HP180;
		dwHPItemNum = ITEM_HP180;
		break;

		case 3:
		dwHPSkillNum = SKILL_HP360;
		dwHPItemNum = ITEM_HP360;
		break;

		case 4:
		dwHPSkillNum = SKILL_HP720;
		dwHPItemNum = ITEM_HP720;
		break;

		case 5:
		dwHPSkillNum = SKILL_PREPOT_HP;
		dwHPItemNum = ITEM_PREPOT_HP;
		break;

		case 6:
		dwHPSkillNum = SKILL_DCPOT_HP;
		dwHPItemNum = ITEM_DCPOT_HP;
		break;

		default:
		break;

	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txthplimitChange(TObject *Sender)
{
	iHPLimit = atoi(txthplimit->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnpnokClick(TObject *Sender)
{
	pnpot->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnpotoptionsClick(TObject *Sender)
{
	pnpot->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnbuyhpClick(TObject *Sender)
{
	bBuyHP = btnbuyhp->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtbuyhpChange(TObject *Sender)
{
	iBuyHPLimit = atoi(txtbuyhp->Text.c_str());
}
//---------------------------------------------------------------------------

void ClearDrops(){
	if(!lstChestList.empty()){
		while(lstChestList.size()){
			delete (_CHEST*)lstChestList.front();
			lstChestList.pop_front();
		}
		lstChestList.clear();
	}

	if(!lstDropList.empty()){
		while(lstDropList.size()){
			delete (_DROP*)lstDropList.front();
			lstDropList.pop_front();
		}
		lstDropList.clear();
	}
}

void __fastcall TfrmBot::btnminimizeClick(TObject *Sender)
{
	bMinimized = !bMinimized;

	if(bMinimized){
		btnminimize->Caption = "+";
		this->ClientHeight = 59;
	}
	else{
		btnminimize->Caption = "-";
		this->ClientHeight = 713;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnclosebotClick(TObject *Sender)
{
	CloseBot(false);
	QuitBot();
}
//---------------------------------------------------------------------------
void __fastcall TfrmBot::chtpchatClick(TObject *Sender)
{
	bChatTP = chtpchat->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBot::chhptpClick(TObject *Sender)
{
	bLimitTP = chhptp->Checked;

	if(!bLimitTP && !bAutoGate)
		tmtp->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chgateClick(TObject *Sender)
{
	bAutoGate = chgate->Checked;

	if(!bLimitTP && !bAutoGate)
		tmtp->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txttplimitChange(TObject *Sender)
{
	iTPLimit = atoi(txttplimit->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtgatelimitChange(TObject *Sender)
{
	iGateLimit = atoi(txtgatelimit->Text.c_str());
}
//---------------------------------------------------------------------------

void TeleportUser(DWORD ID){
	DWORD dwCastNum = _CCast.MakeCastNum(_CCharBase,"004");
	*(DWORD*)(pBuf_Attack1 + 2) = dwCastNum;
	*(DWORD*)(pBuf_Attack3 + 2) = dwCastNum;

	int iPosX = (int)_CCharBase.GetFLOAT(KO_POSX);
	int iPosY = (int)_CCharBase.GetFLOAT(KO_POSY);
	int iPosZ = (int)_CCharBase.GetFLOAT(KO_POSZ);

	*(WORD*)(pBuf_Attack3 + 10) = iPosX;
	*(WORD*)(pBuf_Attack3 + 12) = iPosZ;
	*(WORD*)(pBuf_Attack3 + 14) = iPosY;

	*(WORD*)(pBuf_Attack1 + 8) = ID;
	*(WORD*)(pBuf_Attack3 + 8) = ID;

	*(BYTE*)(pBuf_Attack1 + 22) = 0x0F;

	_CPacket.PSend(pBuf_Attack1,24);
	_CPacket.PSend(pBuf_Attack3,22);

	*(WORD*)(pBuf_Attack3 + 10) = 0x0000;
	*(WORD*)(pBuf_Attack3 + 12) = 0x0000;
	*(WORD*)(pBuf_Attack3 + 14) = 0x0000;
}

void Gate(){
	DWORD dwCastNum = _CCast.MakeCastNum(_CCharBase,"015");
	*(DWORD*)(pBuf_Attack1 + 2) = dwCastNum;
	*(DWORD*)(pBuf_Attack3 + 2) = dwCastNum;

	*(WORD*)(pBuf_Attack1 + 8) = dwMyID;
	*(WORD*)(pBuf_Attack3 + 8) = dwMyID;

	*(BYTE*)(pBuf_Attack1 + 22) = 0x0F;

	_CPacket.PSend(pBuf_Attack1,24);
	_CPacket.PSend(pBuf_Attack3,22);
}

void __fastcall TfrmBot::tmtpTimer(TObject *Sender)
{

	DWORD dwUser;
	int iHP, iMaxHP;

	if(CheckLoading() == true)
		return;

	if(!_CCharBase.GetDWORD(KO_CRHP))
    	return;

	if(GetTickCount() - iLastTPTick > 1500){
		if(bLimitTP){
			_CParty.Refresh();
			if(_CParty.GetCount() > 1){
				for (int i = 0; i < _CParty.GetCount(); i++) {
					dwUser = _CParty.GetUser(i);
					if(dwUser){
						DWORD ID = *(WORD*)(dwUser + KO_PT_ID);
						iHP = *(DWORD*)(dwUser + KO_PT_HP);
						iMaxHP = *(DWORD*)(dwUser + KO_PT_MAXHP);
						if(iHP && _CMath.iPerc(iMaxHP,iHP) <= (float)iTPLimit){
							CCharBase pUser = _CArea.GetUPCBase(ID);
							if(pUser.GetBase() && _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pUser.GetFLOAT(KO_POSX),pUser.GetFLOAT(KO_POSY)) >= 1.0){
								TeleportUser(ID);
								iLastTPTick = GetTickCount();
							}
						}
					}
				}
			}
		}
	}

	if(GetTickCount() - iLastGateTick >= 10000){
		if(bAutoGate){
			iHP = _CCharBase.GetDWORD(KO_CRHP);
			iMaxHP = _CCharBase.GetDWORD(KO_MXHP);
			if(iHP && _CMath.iPerc(iMaxHP,iHP) <= (float)iGateLimit){
				Gate();
				iLastGateTick = GetTickCount();
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chantifreezeClick(TObject *Sender)
{
	bAntiFreeze = chantifreeze->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::scmysizeChange(TObject *Sender)
{
	int iSize = scmysize->Position;

	float fSize = (float)(iSize / 10);
	_CCharBase.SetSize(fSize);

	char tmp[4];
	itoa(iSize,tmp,10);
	lbmysize->Caption = tmp;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnresetsizeClick(TObject *Sender)
{
	scmysize->Position = 10;
	lbmysize->Caption = "1";
	_CCharBase.SetSize(1.0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmtargetsizeTimer(TObject *Sender)
{

	if(CheckLoading() == true)
		return;

	if(GetAsyncKeyState(VK_CONTROL)){
		DWORD ID = _CCharBase.GetTargetID();
		if(ID && ID != 0xFFFF){
			CCharBase pTarget = _CArea.GetAnyBase(ID);
			if(pTarget.GetBase()){
				pTarget.SetSize(2.0);
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chtargetsizeClick(TObject *Sender)
{
	if(chtargetsize->Checked == false)
		tmtargetsize->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chmage62Click(TObject *Sender)
{
	bMage62 = chmage62->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmmageattackTimer(TObject *Sender)
{

	if(bBotPaused)
		return;

	if(!bMageAttack)
    	return;

	if(CheckLoading() == true)
		return;

	if(!_CCharBase.GetDWORD(KO_CRHP))
		return;

	if(_CCharBase.IsSit() == true)
		return;

	DWORD	dwCurrentTick = GetTickCount();
	//if(dwCurrentTick - iLastAttackTick < (DWORD)iAttackDelay)
	//return;

	bool bCastAble = (bool)(dwCurrentTick - iLastAttackTick >= (DWORD)iAttackDelay);
	bool bBasicTurn[5];
	bool bFlameTurn[17];
	bool bIceTurn[16];
	bool bLightTurn[17];

	bool bCast = false;

	bool bSkip = false; //is casted?

	bBasicTurn[0] = (bool)(bCastAble && bMageBasicCastArray[0] && dwCurrentTick - iMageBasicCastArray[0] >= 5000);
	bBasicTurn[1] = (bool)(bCastAble && bMageBasicCastArray[1] && dwCurrentTick - iMageBasicCastArray[1] >= 5000);
	bBasicTurn[2] = (bool)(bCastAble && bMageBasicCastArray[2] && dwCurrentTick - iMageBasicCastArray[2] >= 5000);
	bBasicTurn[3] = (bool)(bCastAble && bMageBasicCastArray[3] && dwCurrentTick - iMageBasicCastArray[3] >= 5000);
	bBasicTurn[4] = (bool)(bCastAble && bMageBasicCastArray[4] && dwCurrentTick - iMageBasicCastArray[4] >= 5000);

	bFlameTurn[0] = (bool)(bCastAble && bMageFlameCastArray[0]);
	bFlameTurn[1] = (bool)(bCastAble && bMageFlameCastArray[1] && dwCurrentTick - iMageFlameCastArray[1] >= 7000);
	bFlameTurn[2] = (bool)(bCastAble && bMageFlameCastArray[2] && dwCurrentTick - iMageFlameCastArray[2] >= 5000);
	bFlameTurn[3] = (bool)(bCastAble && bMageFlameCastArray[3]);
	bFlameTurn[4] = (bool)(bCastAble && bMageFlameCastArray[4] && dwCurrentTick - iMageFlameCastArray[4] >= 5000);
	bFlameTurn[5] = (bool)(bCastAble && bMageFlameCastArray[5] && dwCurrentTick - iMageFlameCastArray[5] >= 3000);
	bFlameTurn[6] = (bool)(bCastAble && bMageFlameCastArray[6] && dwCurrentTick - iMageFlameCastArray[6] >= 5000);
	bFlameTurn[7] = (bool)(bCastAble && bMageFlameCastArray[7] && dwCurrentTick - iMageFlameCastArray[7] >= 5000);
	bFlameTurn[8] = (bool)(bCastAble && bMageFlameCastArray[8]);
	bFlameTurn[9] = (bool)(bCastAble && bMageFlameCastArray[9]);
	bFlameTurn[10] = (bool)(bCastAble && bMageFlameCastArray[10] && dwCurrentTick - iMageFlameCastArray[10] >= 20000);
	bFlameTurn[11] = (bool)(bCastAble && bMageFlameCastArray[11] && dwCurrentTick - iMageFlameCastArray[11] >= 7000);
	bFlameTurn[12] = (bool)(bCastAble && bMageFlameCastArray[12] && dwCurrentTick - iMageFlameCastArray[12] >= 7000);
	bFlameTurn[13] = (bool)(bCastAble && bMageFlameCastArray[13]);
	bFlameTurn[14] = (bool)(bCastAble && bMageFlameCastArray[14] && dwCurrentTick - iMageFlameCastArray[14] >= 23000);
	bFlameTurn[15] = (bool)(bCastAble && bMageFlameCastArray[15] && dwCurrentTick - iMageFlameCastArray[15] >= 20000);
	bFlameTurn[16] = (bool)(bCastAble && bMageFlameCastArray[16] && dwCurrentTick - iMageFlameCastArray[16] >= 23000);
	bFlameTurn[17] = (bool)(bCastAble && bMageFlameCastArray[17] && dwCurrentTick - iMageFlameCastArray[17] >= 25000);
	bFlameTurn[18] = (bool)(bCastAble && bMageFlameCastArray[18]);
	bFlameTurn[19] = (bool)(bCastAble && bMageFlameCastArray[19] && dwCurrentTick - iMageFlameCastArray[19] >= 65000);
	bFlameTurn[20] = (bool)(bCastAble && bMageFlameCastArray[20] && dwCurrentTick - iMageFlameCastArray[20] >= 25000);

	bIceTurn[0] = (bool)(bCastAble && bMageIceCastArray[0]);
	bIceTurn[1] = (bool)(bCastAble && bMageIceCastArray[1] && dwCurrentTick - iMageIceCastArray[1] >= 7000);
	bIceTurn[2] = (bool)(bCastAble && bMageIceCastArray[2] && dwCurrentTick - iMageIceCastArray[2] >= 5000);
	bIceTurn[3] = (bool)(bCastAble && bMageIceCastArray[3]);
	bIceTurn[4] = (bool)(bCastAble && bMageIceCastArray[4] && dwCurrentTick - iMageIceCastArray[4] >= 5000);
	bIceTurn[5] = (bool)(bCastAble && bMageIceCastArray[5] && dwCurrentTick - iMageIceCastArray[5] >= 3000);
	bIceTurn[6] = (bool)(bCastAble && bMageIceCastArray[6] && dwCurrentTick - iMageIceCastArray[6] >= 5000);
	bIceTurn[7] = (bool)(bCastAble && bMageIceCastArray[7] && dwCurrentTick - iMageIceCastArray[7] >= 5000);
	bIceTurn[8] = (bool)(bCastAble && bMageIceCastArray[8]);
	bIceTurn[9] = (bool)(bCastAble && bMageIceCastArray[9]);
	bIceTurn[10] = (bool)(bCastAble && bMageIceCastArray[10] && dwCurrentTick - iMageIceCastArray[10] >= 20000);
	bIceTurn[11] = (bool)(bCastAble && bMageIceCastArray[11] && dwCurrentTick - iMageIceCastArray[11] >= 7000);
	bIceTurn[12] = (bool)(bCastAble && bMageIceCastArray[12]);
	bIceTurn[13] = (bool)(bCastAble && bMageIceCastArray[13] && dwCurrentTick - iMageIceCastArray[13] >= 23000);
	bIceTurn[14] = (bool)(bCastAble && bMageIceCastArray[14] && dwCurrentTick - iMageIceCastArray[14] >= 20000);
	bIceTurn[15] = (bool)(bCastAble && bMageIceCastArray[15] && dwCurrentTick - iMageIceCastArray[15] >= 23000);
	bIceTurn[16] = (bool)(bCastAble && bMageIceCastArray[16] && dwCurrentTick - iMageIceCastArray[16] >= 25000);
	bIceTurn[17] = (bool)(bCastAble && bMageIceCastArray[17]);

	bLightTurn[0] = (bool)(bCastAble && bMageLightCastArray[0]);
	bLightTurn[1] = (bool)(bCastAble && bMageLightCastArray[1] && dwCurrentTick - iMageLightCastArray[1] >= 7000);
	bLightTurn[2] = (bool)(bCastAble && bMageLightCastArray[2] && dwCurrentTick - iMageLightCastArray[2] >= 5000);
	bLightTurn[3] = (bool)(bCastAble && bMageLightCastArray[3]);
	bLightTurn[4] = (bool)(bCastAble && bMageLightCastArray[4] && dwCurrentTick - iMageLightCastArray[4] >= 5000);
	bLightTurn[5] = (bool)(bCastAble && bMageLightCastArray[5] && dwCurrentTick - iMageLightCastArray[5] >= 3000);
	bLightTurn[6] = (bool)(bCastAble && bMageLightCastArray[6] && dwCurrentTick - iMageLightCastArray[6] >= 5000);
	bLightTurn[7] = (bool)(bCastAble && bMageLightCastArray[7] && dwCurrentTick - iMageLightCastArray[7] >= 5000);
	bLightTurn[8] = (bool)(bCastAble && bMageLightCastArray[8]);
	bLightTurn[9] = (bool)(bCastAble && bMageLightCastArray[9]);
	bLightTurn[10] = (bool)(bCastAble && bMageLightCastArray[10] && dwCurrentTick - iMageLightCastArray[10] >= 20000);
	bLightTurn[11] = (bool)(bCastAble && bMageLightCastArray[11] && dwCurrentTick - iMageLightCastArray[11] >= 7000);
	bLightTurn[12] = (bool)(bCastAble && bMageLightCastArray[12] && dwCurrentTick - iMageLightCastArray[12] >= 7000);
	bLightTurn[13] = (bool)(bCastAble && bMageLightCastArray[13]);
	bLightTurn[14] = (bool)(bCastAble && bMageLightCastArray[14] && dwCurrentTick - iMageLightCastArray[14] >= 23000);
	bLightTurn[15] = (bool)(bCastAble && bMageLightCastArray[15] && dwCurrentTick - iMageLightCastArray[15] >= 20000);
	bLightTurn[16] = (bool)(bCastAble && bMageLightCastArray[16] && dwCurrentTick - iMageLightCastArray[16] >= 23000);
	bLightTurn[17] = (bool)(bCastAble && bMageLightCastArray[17] && dwCurrentTick - iMageLightCastArray[17] >= 25000);
	bLightTurn[18] = (bool)(bCastAble && bMageLightCastArray[18]);

	for (int i = 0; i < 21; i++) {

		if(i < 5){
			if(bBasicTurn[i] == true){
				bCast = true;
				break;
			}
		}

		if(bFlameTurn[i] == true){
			bCast = true;
			break;
		}

		if(bIceTurn[i] == true){
			bCast = true;
			break;
		}

		if(bLightTurn[i] == true){
			bCast = true;
			break;
		}

	}

	CCharBase pTarget = NULL;
	CCharBase pTmp = NULL;
	float fDistance;
	DWORD TmpVal;
	int i;
	int iMobX,iMobY,iMobZ;
	DWORD dwCurrentCast = 0;
	int		iCurrentCast;
	int		iCurrentData;

	if( bAttackWithLeader )
	{
		_CParty.Refresh();
		if(_CParty.GetCount() < 2)
			return;
		if(wLastTarget && wLastTarget != 0xFFFF){
			pTmp = _CArea.GetAnyBase(wLastTarget);
			if(pTmp.GetBase()){
				if(pTmp.IsAlive()){
					fDistance = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pTmp.GetFLOAT(KO_POSX),pTmp.GetFLOAT(KO_POSY));
					if(fDistance <= 7.7)
					{
						goto SELECT;
					}
				}
			}
		}
		return;
	}


	if(bWaitForKill){
		if(wLastTarget && wLastTarget != 0xFFFF){
			pTmp = _CArea.GetAnyBase(wLastTarget);
			if(pTmp.GetBase()){
				if(pTmp.IsAlive()){
					fDistance = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pTmp.GetFLOAT(KO_POSX),pTmp.GetFLOAT(KO_POSY));
					if(fDistance <= 7.7)
					{
						if(bInterClient)
							SendTarget();
						goto CAST;
					}
				}
			}
		}
	}

	switch(iTargetingType){

		case 0:	//auto
		{
			if(bOnlyEnemy)
				pTarget = _CArea.GetNearstUPC(true,true,7.7);
			else
				pTarget = _CArea.GetNearstEnemy(0,true,7.7);
			break;
		}

		case 1:	//manuel
		{
			wLastTarget = _CCharBase.GetTargetID();
			if(wLastTarget == 0xFFFF)
			{
				if(bInterClient)
					SendTarget();
				return;
			}

			pTarget = _CArea.GetAnyBase(wLastTarget);
			if(!pTarget.GetBase())
			{
				if(bInterClient)
				{
					wLastTarget = 0xFFFF;
					SendTarget();
				}
				return;
			}

			if(!pTarget.IsAlive())
			{
				if(bInterClient)
				{
					wLastTarget = 0xFFFF;
					SendTarget();
				}
				return;
			}

			if(bOnlyEnemy){
				if(!pTarget.IsUPC())
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}

				if(pTarget.GetDWORD(KO_NATN) == _CCharBase.GetDWORD(KO_NATN))
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}
			}

			if(bOnlyTarget){
				if(!pTarget.IsEnemy())
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}
			}

			break;
		}

		case 2:	//list
		{
			if(!iMobCount)
			{
				if(bInterClient)
				{
					wLastTarget = 0xFFFF;
					SendTarget();
				}
				return;
			}

			pTarget = _CArea.GetNPCByName(iMobCount,strMobList,NULL,true,true,true,7.7,false);
			break;
		}

		case 3: //sub
		{
			if(iSubTargetingType == 1)
			{
				if(!iTargetMobIDCount)
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}

				pTarget = _CArea.GetNearstNPCByID(iTargetMobID,iTargetMobIDCount,true,7.7);
			}
			else if(iSubTargetingType == 2)
			{
				if(!iTargetNickCount)
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}

				pTarget = _CArea.GetUPCByName(iTargetNickCount,strTargetNick,0,true,7.7,false,true);
			}
			break;
		}

		default:
		return;
		break;
	}

SELECT:
	if(pTarget.GetBase() == NULL)
		pTarget.SetBase(pTmp.GetBase());

	if(!pTarget.GetBase())
	{
		if(bInterClient)
		{
			wLastTarget = 0xFFFF;
			SendTarget();
		}
		return;
	}

	wLastTarget = pTarget.GetID();

	if(bInterClient)
		SendTarget();

	fDistance = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pTarget.GetFLOAT(KO_POSX),pTarget.GetFLOAT(KO_POSY));
	if(fDistance > 7.7)
		return;

	if(bCast)
	{
		if(bShowMob)
			_CArea.SelectObject(pTarget);
		else
			SendTargetHP(wLastTarget);
	}

CAST:

	if(pTarget.GetBase() == NULL)
		pTarget.SetBase(pTmp.GetBase());

	if(bFlameTurn[16] || bIceTurn[15] || bLightTurn[16]){	//70

		if(bFlameTurn[16]){
			dwCurrentCast = dwMageFlameCastArray[16];
			iCurrentData = iMageFlameDataArray[16];
			iCurrentCast = 1;
		}
		else if(bIceTurn[15]){
			dwCurrentCast = dwMageIceCastArray[15];
			iCurrentData = iMageIceDataArray[15];
			iCurrentCast = 2;
		}
		else if(bLightTurn[16]){
			dwCurrentCast = dwMageLightCastArray[16];
			iCurrentData = iMageLightDataArray[16];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;


		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

			*(WORD*)(pBuf_Attack1 + 8) = 0xFFFF;
			*(WORD*)(pBuf_Attack3 + 8) = 0xFFFF;

			*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

			iMobX = (int)pTarget.GetFLOAT(KO_POSX);
			iMobY = (int)pTarget.GetFLOAT(KO_POSY);
			iMobZ = (int)pTarget.GetFLOAT(KO_POSZ);

			*(WORD*)(pBuf_Attack1 + 10) = iMobX;
			*(WORD*)(pBuf_Attack3 + 10) = iMobX;

			*(WORD*)(pBuf_Attack1 + 12) = iMobZ;
			*(WORD*)(pBuf_Attack3 + 12) = iMobZ;

			*(WORD*)(pBuf_Attack1 + 14) = iMobY;
			*(WORD*)(pBuf_Attack3 + 14) = iMobY;

			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack3,22);

			if(iCurrentCast == 1)
				iMageFlameCastArray[16] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[15] = dwCurrentTick;
			else
				iMageLightCastArray[16] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			*(WORD*)(pBuf_Attack1 + 10) = 0x0000;
			*(WORD*)(pBuf_Attack3 + 10) = 0x0000;

			*(WORD*)(pBuf_Attack1 + 12) = 0x0000;
			*(WORD*)(pBuf_Attack3 + 12) = 0x0000;

			*(WORD*)(pBuf_Attack1 + 14) = 0x0000;
			*(WORD*)(pBuf_Attack3 + 14) = 0x0000;

			return;
		}
	}


	if(bFlameTurn[15] || bIceTurn[14] || bLightTurn[15]){	//60

		if(bFlameTurn[15]){
			dwCurrentCast = dwMageFlameCastArray[15];
			iCurrentData = iMageFlameDataArray[15];
			iCurrentCast = 1;
		}
		else if(bIceTurn[14]){
			dwCurrentCast = dwMageIceCastArray[14];
			iCurrentData = iMageIceDataArray[14];
			iCurrentCast = 2;
		}
		else if(bLightTurn[15]){
			dwCurrentCast = dwMageLightCastArray[15];
			iCurrentData = iMageLightDataArray[15];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

			*(WORD*)(pBuf_Attack1 + 8) = 0xFFFF;
			*(WORD*)(pBuf_Attack3 + 8) = 0xFFFF;

			*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

			iMobX = (int)pTarget.GetFLOAT(KO_POSX);
			iMobY = (int)pTarget.GetFLOAT(KO_POSY);
			iMobZ = (int)pTarget.GetFLOAT(KO_POSZ);

			*(WORD*)(pBuf_Attack1 + 10) = iMobX;
			*(WORD*)(pBuf_Attack3 + 10) = iMobX;

			*(WORD*)(pBuf_Attack1 + 12) = iMobZ;
			*(WORD*)(pBuf_Attack3 + 12) = iMobZ;

			*(WORD*)(pBuf_Attack1 + 14) = iMobY;
			*(WORD*)(pBuf_Attack3 + 14) = iMobY;

			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack3,22);

			if(iCurrentCast == 1)
				iMageFlameCastArray[15] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[14] = dwCurrentTick;
			else
				iMageLightCastArray[15] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			*(WORD*)(pBuf_Attack1 + 10) = 0x0000;
			*(WORD*)(pBuf_Attack3 + 10) = 0x0000;

			*(WORD*)(pBuf_Attack1 + 12) = 0x0000;
			*(WORD*)(pBuf_Attack3 + 12) = 0x0000;

			*(WORD*)(pBuf_Attack1 + 14) = 0x0000;
			*(WORD*)(pBuf_Attack3 + 14) = 0x0000;

			return;
		}
	}

	if(bFlameTurn[10] || bIceTurn[10] || bLightTurn[10]){	//45

		if(bFlameTurn[10]){
			dwCurrentCast = dwMageFlameCastArray[10];
			iCurrentData = iMageFlameDataArray[10];
			iCurrentCast = 1;
		}
		else if(bIceTurn[10]){
			dwCurrentCast = dwMageIceCastArray[10];
			iCurrentData = iMageIceDataArray[10];
			iCurrentCast = 2;
		}
		else if(bLightTurn[10]){
			dwCurrentCast = dwMageLightCastArray[10];
			iCurrentData = iMageLightDataArray[10];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

			*(WORD*)(pBuf_Attack1 + 8) = 0xFFFF;
			*(WORD*)(pBuf_Attack3 + 8) = 0xFFFF;

			*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

			iMobX = (int)pTarget.GetFLOAT(KO_POSX);
			iMobY = (int)pTarget.GetFLOAT(KO_POSY);
			iMobZ = (int)pTarget.GetFLOAT(KO_POSZ);

			*(WORD*)(pBuf_Attack1 + 10) = iMobX;
			*(WORD*)(pBuf_Attack3 + 10) = iMobX;

			*(WORD*)(pBuf_Attack1 + 12) = iMobZ;
			*(WORD*)(pBuf_Attack3 + 12) = iMobZ;

			*(WORD*)(pBuf_Attack1 + 14) = iMobY;
			*(WORD*)(pBuf_Attack3 + 14) = iMobY;

			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack3,22);

			if(iCurrentCast == 1)
				iMageFlameCastArray[10] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[10] = dwCurrentTick;
			else
				iMageLightCastArray[10] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			*(WORD*)(pBuf_Attack1 + 10) = 0x0000;
			*(WORD*)(pBuf_Attack3 + 10) = 0x0000;

			*(WORD*)(pBuf_Attack1 + 12) = 0x0000;
			*(WORD*)(pBuf_Attack3 + 12) = 0x0000;

			*(WORD*)(pBuf_Attack1 + 14) = 0x0000;
			*(WORD*)(pBuf_Attack3 + 14) = 0x0000;

			return;
		}
	}

	if(bFlameTurn[19]){	//flame-80 vampiric

		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack1 + 2) = dwMageFlameCastArray[19];
			*(DWORD*)(pBuf_Attack2 + 2) = dwMageFlameCastArray[19];
			*(DWORD*)(pBuf_Attack3 + 2) = dwMageFlameCastArray[19];
			*(DWORD*)(pBuf_Attack4 + 2) = dwMageFlameCastArray[19];

			*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack2 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack4 + 8) = wLastTarget;

			iMobX = (int)pTarget.GetFLOAT(KO_POSX);
			iMobY = (int)pTarget.GetFLOAT(KO_POSY);
			iMobZ = (int)pTarget.GetFLOAT(KO_POSZ);

			*(WORD*)(pBuf_Attack4 + 10) = iMobX;
			*(WORD*)(pBuf_Attack4 + 12) = iMobZ;
			*(WORD*)(pBuf_Attack4 + 14) = iMobY;
			*(BYTE*)(pBuf_Attack4 + 16) = 0x9B;
			*(BYTE*)(pBuf_Attack4 + 17) = 0xFF;

			*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iMageFlameDataArray[19];

			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack2,22);
			_CPacket.PSend(pBuf_Attack3,24);
			_CPacket.PSend(pBuf_Attack4,24);

			iMageFlameCastArray[19] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			return;
		}
	}

	if(bFlameTurn[20]){	//flame-80 igzination

		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack1 + 2) = dwMageFlameCastArray[20];
			*(DWORD*)(pBuf_Attack3 + 2) = dwMageFlameCastArray[20];

			*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

			*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iMageFlameDataArray[20];

			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack3,22);

			iMageFlameCastArray[20] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			return;
		}
	}

	if(bFlameTurn[17] || bIceTurn[16] || bLightTurn[17]){	//70_1

		if(bFlameTurn[17]){
			dwCurrentCast = dwMageFlameCastArray[17];
			iCurrentData = iMageFlameDataArray[17];
			iCurrentCast = 1;
		}
		else if(bIceTurn[16]){
			dwCurrentCast = dwMageIceCastArray[16];
			iCurrentData = iMageIceDataArray[16];
			iCurrentCast = 2;
		}
		else if(bLightTurn[17]){
			dwCurrentCast = dwMageLightCastArray[17];
			iCurrentData = iMageLightDataArray[17];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

			*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

			*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack3,22);

			if(iCurrentCast == 1)
				iMageFlameCastArray[17] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[16] = dwCurrentTick;
			else
				iMageLightCastArray[17] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			return;
		}
	}

	if(bFlameTurn[14] || bIceTurn[13] || bLightTurn[14]){	//57

		if(bFlameTurn[14]){
			dwCurrentCast = dwMageFlameCastArray[14];
			iCurrentData = iMageFlameDataArray[14];
			iCurrentCast = 1;
		}
		else if(bIceTurn[13]){
			dwCurrentCast = dwMageIceCastArray[13];
			iCurrentData = iMageIceDataArray[13];
			iCurrentCast = 2;
		}
		else if(bLightTurn[14]){
			dwCurrentCast = dwMageLightCastArray[14];
			iCurrentData = iMageLightDataArray[14];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			if(iCurrentCast == 3)	//light
			{
				*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
				*(DWORD*)(pBuf_Attack2 + 2) = dwCurrentCast;
				*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;
				*(DWORD*)(pBuf_Attack4 + 2) = dwCurrentCast;

				*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
				*(WORD*)(pBuf_Attack2 + 8) = wLastTarget;
				*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
				*(WORD*)(pBuf_Attack4 + 8) = wLastTarget;

				iMobX = (int)pTarget.GetFLOAT(KO_POSX);
				iMobY = (int)pTarget.GetFLOAT(KO_POSY);
				iMobZ = (int)pTarget.GetFLOAT(KO_POSZ);

				*(WORD*)(pBuf_Attack4 + 10) = iMobX;
				*(WORD*)(pBuf_Attack4 + 12) = iMobZ;
				*(WORD*)(pBuf_Attack4 + 14) = iMobY;
				*(BYTE*)(pBuf_Attack4 + 16) = 0x9B;
				*(BYTE*)(pBuf_Attack4 + 17) = 0xFF;

				*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

				_CPacket.PSend(pBuf_Attack1,24);
				_CPacket.PSend(pBuf_Attack2,22);
				_CPacket.PSend(pBuf_Attack3,24);
				_CPacket.PSend(pBuf_Attack4,24);
			}
			else
			{
				*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
				*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

				*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
				*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

				*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

				_CPacket.PSend(pBuf_Attack1,24);
				_CPacket.PSend(pBuf_Attack3,22);
			}

			if(iCurrentCast == 1)
				iMageFlameCastArray[14] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[13] = dwCurrentTick;
			else
				iMageLightCastArray[14] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			return;
		}
	}

	if(bFlameTurn[12] || bLightTurn[12]){	//54

		if(bFlameTurn[12]){
			dwCurrentCast = dwMageFlameCastArray[12];
			iCurrentData = iMageFlameDataArray[12];
			iCurrentCast = 1;
		}
		else if(bLightTurn[12]){
			dwCurrentCast = dwMageLightCastArray[12];
			iCurrentData = iMageLightDataArray[12];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			if(iCurrentCast == 3)	//light
			{
				*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
				*(DWORD*)(pBuf_Attack2 + 2) = dwCurrentCast;
				*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;
				*(DWORD*)(pBuf_Attack4 + 2) = dwCurrentCast;

				*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
				*(WORD*)(pBuf_Attack2 + 8) = wLastTarget;
				*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
				*(WORD*)(pBuf_Attack4 + 8) = wLastTarget;

				iMobX = (int)pTarget.GetFLOAT(KO_POSX);
				iMobY = (int)pTarget.GetFLOAT(KO_POSY);
				iMobZ = (int)pTarget.GetFLOAT(KO_POSZ);

				*(WORD*)(pBuf_Attack4 + 10) = iMobX;
				*(WORD*)(pBuf_Attack4 + 12) = iMobZ;
				*(WORD*)(pBuf_Attack4 + 14) = iMobY;
				*(BYTE*)(pBuf_Attack4 + 16) = 0x9B;
				*(BYTE*)(pBuf_Attack4 + 17) = 0xFF;

				*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

				_CPacket.PSend(pBuf_Attack1,24);
				_CPacket.PSend(pBuf_Attack2,22);
				_CPacket.PSend(pBuf_Attack3,24);
				_CPacket.PSend(pBuf_Attack4,24);
			}
			else
			{
				*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
				*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

				*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
				*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

				*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

				_CPacket.PSend(pBuf_Attack1,24);
				_CPacket.PSend(pBuf_Attack3,22);
			}

			if(iCurrentCast == 1)
				iMageFlameCastArray[12] = dwCurrentTick;
			else
				iMageLightCastArray[12] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			return;
		}
	}

	if(bFlameTurn[11] || bIceTurn[11] || bLightTurn[11]){	//51

		if(bFlameTurn[11]){
			dwCurrentCast = dwMageFlameCastArray[11];
			iCurrentData = iMageFlameDataArray[11];
			iCurrentCast = 1;
		}
		else if(bIceTurn[11]){
			dwCurrentCast = dwMageIceCastArray[11];
			iCurrentData = iMageIceDataArray[11];
			iCurrentCast = 2;
		}
		else if(bLightTurn[11]){
			dwCurrentCast = dwMageLightCastArray[11];
			iCurrentData = iMageLightDataArray[11];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			if(iCurrentCast == 3)	//light
			{
				*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
				*(DWORD*)(pBuf_Attack2 + 2) = dwCurrentCast;
				*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;
				*(DWORD*)(pBuf_Attack4 + 2) = dwCurrentCast;

				*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
				*(WORD*)(pBuf_Attack2 + 8) = wLastTarget;
				*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
				*(WORD*)(pBuf_Attack4 + 8) = wLastTarget;

				iMobX = (int)pTarget.GetFLOAT(KO_POSX);
				iMobY = (int)pTarget.GetFLOAT(KO_POSY);
				iMobZ = (int)pTarget.GetFLOAT(KO_POSZ);

				*(WORD*)(pBuf_Attack4 + 10) = iMobX;
				*(WORD*)(pBuf_Attack4 + 12) = iMobZ;
				*(WORD*)(pBuf_Attack4 + 14) = iMobY;
				*(BYTE*)(pBuf_Attack4 + 16) = 0x9B;
				*(BYTE*)(pBuf_Attack4 + 17) = 0xFF;

				*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

				_CPacket.PSend(pBuf_Attack1,24);
				_CPacket.PSend(pBuf_Attack2,22);
				_CPacket.PSend(pBuf_Attack3,24);
				_CPacket.PSend(pBuf_Attack4,24);
			}
			else
			{
				*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
				*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

				*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
				*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

				*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

				_CPacket.PSend(pBuf_Attack1,24);
				_CPacket.PSend(pBuf_Attack3,22);
			}

			if(iCurrentCast == 1)
				iMageFlameCastArray[11] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[11] = dwCurrentTick;
			else
				iMageLightCastArray[11] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			return;
		}
	}

	if(bFlameTurn[7] || bIceTurn[7] || bLightTurn[7]){	//39

		if(bFlameTurn[7]){
			dwCurrentCast = dwMageFlameCastArray[7];
			iCurrentData = iMageFlameDataArray[7];
			iCurrentCast = 1;
		}
		else if(bIceTurn[7]){
			dwCurrentCast = dwMageIceCastArray[7];
			iCurrentData = iMageIceDataArray[7];
			iCurrentCast = 2;
		}
		else if(bLightTurn[7]){
			dwCurrentCast = dwMageLightCastArray[7];
			iCurrentData = iMageLightDataArray[7];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

			*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

			*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack3,22);

			if(iCurrentCast == 1)
				iMageFlameCastArray[7] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[7] = dwCurrentTick;
			else
				iMageLightCastArray[7] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			return;
		}
	}

	if(bFlameTurn[6] || bIceTurn[6] || bLightTurn[6]){	//35

		if(bFlameTurn[6]){
			dwCurrentCast = dwMageFlameCastArray[6];
			iCurrentData = iMageFlameDataArray[6];
			iCurrentCast = 1;
		}
		else if(bIceTurn[6]){
			dwCurrentCast = dwMageIceCastArray[6];
			iCurrentData = iMageIceDataArray[6];
			iCurrentCast = 2;
		}
		else if(bLightTurn[6]){
			dwCurrentCast = dwMageLightCastArray[6];
			iCurrentData = iMageLightDataArray[6];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack2 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack4 + 2) = dwCurrentCast;

			*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack2 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack4 + 8) = wLastTarget;

			iMobX = (int)pTarget.GetFLOAT(KO_POSX);
			iMobY = (int)pTarget.GetFLOAT(KO_POSY);
			iMobZ = (int)pTarget.GetFLOAT(KO_POSZ);

			*(WORD*)(pBuf_Attack4 + 10) = iMobX;
			*(WORD*)(pBuf_Attack4 + 12) = iMobZ;
			*(WORD*)(pBuf_Attack4 + 14) = iMobY;
			*(BYTE*)(pBuf_Attack4 + 16) = 0x9B;
			*(BYTE*)(pBuf_Attack4 + 17) = 0xFF;

			*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack2,22);
			_CPacket.PSend(pBuf_Attack3,24);
			_CPacket.PSend(pBuf_Attack4,24);

			if(iCurrentCast == 1)
				iMageFlameCastArray[6] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[6] = dwCurrentTick;
			else
				iMageLightCastArray[6] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			return;
		}
	}

	if(bFlameTurn[5] || bIceTurn[5] || bLightTurn[5]){	//33

		if(bFlameTurn[5]){
			dwCurrentCast = dwMageFlameCastArray[5];
			iCurrentData = iMageFlameDataArray[5];
			iCurrentCast = 1;
		}
		else if(bIceTurn[5]){
			dwCurrentCast = dwMageIceCastArray[5];
			iCurrentData = iMageIceDataArray[5];
			iCurrentCast = 2;
		}
		else if(bLightTurn[5]){
			dwCurrentCast = dwMageLightCastArray[5];
			iCurrentData = iMageLightDataArray[5];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack2 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack4 + 2) = dwCurrentCast;

			*(WORD*)(pBuf_Attack1 + 8) = 0xFFFF;
			*(WORD*)(pBuf_Attack2 + 8) = 0xFFFF;
			*(WORD*)(pBuf_Attack3 + 8) = 0xFFFF;
			*(WORD*)(pBuf_Attack4 + 8) = 0xFFFF;

			*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

			iMobX = (int)pTarget.GetFLOAT(KO_POSX);
			iMobY = (int)pTarget.GetFLOAT(KO_POSY);
			iMobZ = (int)pTarget.GetFLOAT(KO_POSZ);

			*(WORD*)(pBuf_Attack1 + 10) = iMobX;
			*(WORD*)(pBuf_Attack2 + 10) = iMobX;
			*(WORD*)(pBuf_Attack3 + 10) = iMobX;
			*(WORD*)(pBuf_Attack4 + 10) = iMobX;

			*(WORD*)(pBuf_Attack1 + 12) = iMobZ;
			*(WORD*)(pBuf_Attack2 + 12) = iMobZ;
			*(WORD*)(pBuf_Attack3 + 12) = iMobZ;
			*(WORD*)(pBuf_Attack4 + 12) = iMobZ;

			*(WORD*)(pBuf_Attack1 + 14) = iMobY;
			*(WORD*)(pBuf_Attack2 + 14) = iMobY;
			*(WORD*)(pBuf_Attack3 + 14) = iMobY;
			*(WORD*)(pBuf_Attack4 + 14) = iMobY;

			*(BYTE*)(pBuf_Attack4 + 16) = 0x9B;
			*(BYTE*)(pBuf_Attack4 + 17) = 0xFF;

			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack2,22);
			_CPacket.PSend(pBuf_Attack3,24);
			_CPacket.PSend(pBuf_Attack4,24);

			if(iCurrentCast == 1)
				iMageFlameCastArray[5] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[5] = dwCurrentTick;
			else
				iMageLightCastArray[5] = dwCurrentTick;

			iLastAttackTick	= dwCurrentTick;

			*(WORD*)(pBuf_Attack1 + 10) = 0x0000;
			*(WORD*)(pBuf_Attack2 + 10) = 0x0000;
			*(WORD*)(pBuf_Attack3 + 10) = 0x0000;

			*(WORD*)(pBuf_Attack1 + 12) = 0x0000;
			*(WORD*)(pBuf_Attack2 + 12) = 0x0000;
			*(WORD*)(pBuf_Attack3 + 12) = 0x0000;

			*(WORD*)(pBuf_Attack1 + 14) = 0x0000;
			*(WORD*)(pBuf_Attack2 + 14) = 0x0000;
			*(WORD*)(pBuf_Attack3 + 14) = 0x0000;

			return;
		}
	}

	if(bFlameTurn[4] || bIceTurn[4] || bLightTurn[4]){	//27

		if(bFlameTurn[4]){
			dwCurrentCast = dwMageFlameCastArray[4];
			iCurrentData = iMageFlameDataArray[4];
			iCurrentCast = 1;
		}
		else if(bIceTurn[4]){
			dwCurrentCast = dwMageIceCastArray[4];
			iCurrentData = iMageIceDataArray[4];
			iCurrentCast = 2;
		}
		else if(bLightTurn[4]){
			dwCurrentCast = dwMageLightCastArray[4];
			iCurrentData = iMageLightDataArray[4];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack2 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack4 + 2) = dwCurrentCast;

			*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack2 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack4 + 8) = wLastTarget;

			iMobX = (int)pTarget.GetFLOAT(KO_POSX);
			iMobY = (int)pTarget.GetFLOAT(KO_POSY);
			iMobZ = (int)pTarget.GetFLOAT(KO_POSZ);

			*(WORD*)(pBuf_Attack4 + 10) = iMobX;
			*(WORD*)(pBuf_Attack4 + 12) = iMobZ;
			*(WORD*)(pBuf_Attack4 + 14) = iMobY;
			*(BYTE*)(pBuf_Attack4 + 16) = 0x9B;
			*(BYTE*)(pBuf_Attack4 + 17) = 0xFF;

			*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack2,22);
			_CPacket.PSend(pBuf_Attack3,24);
			_CPacket.PSend(pBuf_Attack4,24);

			if(iCurrentCast == 1)
				iMageFlameCastArray[4] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[4] = dwCurrentTick;
			else
				iMageLightCastArray[4] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			return;
		}
	}

	if( fDistance < 3.5 && (bFlameTurn[18] || bIceTurn[17] || bLightTurn[18]) ){	//72

		if(bFlameTurn[18]){
			dwCurrentCast = dwMageFlameCastArray[18];
			iCurrentData = iMageFlameDataArray[18];
			iCurrentCast = 1;
		}
		else if(bIceTurn[17]){
			dwCurrentCast = dwMageIceCastArray[17];
			iCurrentData = iMageIceDataArray[17];
			iCurrentCast = 2;
		}
		else if(bLightTurn[18]){
			dwCurrentCast = dwMageLightCastArray[18];
			iCurrentData = iMageLightDataArray[18];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

			_CPacket.PSend(pBuf_Attack3,24);

			if(iCurrentCast == 1)
				iMageFlameCastArray[18] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[17] = dwCurrentTick;
			else
				iMageLightCastArray[18] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			return;
		}
	}

	if( fDistance < 3.5 && (bFlameTurn[13] || bIceTurn[12] || bLightTurn[13]) ){	//56

		if(bFlameTurn[13]){
			dwCurrentCast = dwMageFlameCastArray[13];
			iCurrentData = iMageFlameDataArray[13];
			iCurrentCast = 1;
		}
		else if(bIceTurn[12]){
			dwCurrentCast = dwMageIceCastArray[12];
			iCurrentData = iMageIceDataArray[12];
			iCurrentCast = 2;
		}
		else if(bLightTurn[13]){
			dwCurrentCast = dwMageLightCastArray[13];
			iCurrentData = iMageLightDataArray[13];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

			*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

			*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack3,22);

			if(iCurrentCast == 1)
				iMageFlameCastArray[13] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[12] = dwCurrentTick;
			else
				iMageLightCastArray[13] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			return;
		}
	}

	if( fDistance < 3.5 && (bFlameTurn[9] || bIceTurn[9] || bLightTurn[9]) ){	//43

		if(bFlameTurn[9]){
			dwCurrentCast = dwMageFlameCastArray[9];
			iCurrentData = iMageFlameDataArray[9];
			iCurrentCast = 1;
		}
		else if(bIceTurn[9]){
			dwCurrentCast = dwMageIceCastArray[9];
			iCurrentData = iMageIceDataArray[9];
			iCurrentCast = 2;
		}
		else if(bLightTurn[9]){
			dwCurrentCast = dwMageLightCastArray[9];
			iCurrentData = iMageLightDataArray[9];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

			*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

			*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack3,22);

			if(iCurrentCast == 1)
				iMageFlameCastArray[9] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[9] = dwCurrentTick;
			else
				iMageLightCastArray[9] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			return;
		}
	}

	if( fDistance < 3.5 && (bFlameTurn[8] || bIceTurn[8] || bLightTurn[8]) ){	//42

		if(bFlameTurn[8]){
			dwCurrentCast = dwMageFlameCastArray[8];
			iCurrentData = iMageFlameDataArray[8];
			iCurrentCast = 1;
		}
		else if(bIceTurn[8]){
			dwCurrentCast = dwMageIceCastArray[8];
			iCurrentData = iMageIceDataArray[8];
			iCurrentCast = 2;
		}
		else if(bLightTurn[8]){
			dwCurrentCast = dwMageLightCastArray[8];
			iCurrentData = iMageLightDataArray[8];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

			_CPacket.PSend(pBuf_Attack3,24);

			if(iCurrentCast == 1)
				iMageFlameCastArray[8] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[8] = dwCurrentTick;
			else
				iMageLightCastArray[8] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			return;
		}
	}

	if(bFlameTurn[3] || bIceTurn[3] || bLightTurn[3]){	//18

		if(bFlameTurn[3]){
			dwCurrentCast = dwMageFlameCastArray[3];
			iCurrentData = iMageFlameDataArray[3];
			iCurrentCast = 1;
		}
		else if(bIceTurn[3]){
			dwCurrentCast = dwMageIceCastArray[3];
			iCurrentData = iMageIceDataArray[3];
			iCurrentCast = 2;
		}
		else if(bLightTurn[3]){
			dwCurrentCast = dwMageLightCastArray[3];
			iCurrentData = iMageLightDataArray[3];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

			*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

			*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack3,22);

			if(iCurrentCast == 1)
				iMageFlameCastArray[3] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[3] = dwCurrentTick;
			else
				iMageLightCastArray[3] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			return;
		}
	}

	if(bFlameTurn[2] || bIceTurn[2] || bLightTurn[2]){	//15

		if(bFlameTurn[2]){
			dwCurrentCast = dwMageFlameCastArray[2];
			iCurrentData = iMageFlameDataArray[2];
			iCurrentCast = 1;
		}
		else if(bIceTurn[2]){
			dwCurrentCast = dwMageIceCastArray[2];
			iCurrentData = iMageIceDataArray[2];
			iCurrentCast = 2;
		}
		else if(bLightTurn[2]){
			dwCurrentCast = dwMageLightCastArray[2];
			iCurrentData = iMageLightDataArray[2];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack2 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack4 + 2) = dwCurrentCast;

			*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack2 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack4 + 8) = wLastTarget;

			iMobX = (int)pTarget.GetFLOAT(KO_POSX);
			iMobY = (int)pTarget.GetFLOAT(KO_POSY);
			iMobZ = (int)pTarget.GetFLOAT(KO_POSZ);

			*(WORD*)(pBuf_Attack4 + 10) = iMobX;
			*(WORD*)(pBuf_Attack4 + 12) = iMobZ;
			*(WORD*)(pBuf_Attack4 + 14) = iMobY;
			*(BYTE*)(pBuf_Attack4 + 16) = 0x9B;
			*(BYTE*)(pBuf_Attack4 + 17) = 0xFF;

			*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack2,22);
			_CPacket.PSend(pBuf_Attack3,24);
			_CPacket.PSend(pBuf_Attack4,24);

			if(iCurrentCast == 1)
				iMageFlameCastArray[2] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[2] = dwCurrentTick;
			else
				iMageLightCastArray[2] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			return;
		}
	}

	if(bFlameTurn[1] || bIceTurn[1] || bLightTurn[1]){	//9

		if(bFlameTurn[1]){
			dwCurrentCast = dwMageFlameCastArray[1];
			iCurrentData = iMageFlameDataArray[1];
			iCurrentCast = 1;
		}
		else if(bIceTurn[1]){
			dwCurrentCast = dwMageIceCastArray[1];
			iCurrentData = iMageIceDataArray[1];
			iCurrentCast = 2;
		}
		else if(bLightTurn[1]){
			dwCurrentCast = dwMageLightCastArray[1];
			iCurrentData = iMageLightDataArray[1];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

			*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

			*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack3,22);

			if(iCurrentCast == 1)
				iMageFlameCastArray[1] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[1] = dwCurrentTick;
			else
				iMageLightCastArray[1] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			return;
		}
	}

	if(bFlameTurn[0] || bIceTurn[0] || bLightTurn[0]){	//9

		if(bFlameTurn[0]){
			dwCurrentCast = dwMageFlameCastArray[0];
			iCurrentData = iMageFlameDataArray[0];
			iCurrentCast = 1;
		}
		else if(bIceTurn[0]){
			dwCurrentCast = dwMageIceCastArray[0];
			iCurrentData = iMageIceDataArray[0];
			iCurrentCast = 2;
		}
		else if(bLightTurn[0]){
			dwCurrentCast = dwMageLightCastArray[0];
			iCurrentData = iMageLightDataArray[0];
			iCurrentCast = 3;
		}
		else
			dwCurrentCast = 0;

		if(dwCurrentCast){
			*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
			*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

			*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

			*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack3,22);

			if(iCurrentCast == 1)
				iMageFlameCastArray[0] = dwCurrentTick;
			else if(iCurrentCast == 2)
				iMageIceCastArray[0] = dwCurrentTick;
			else
				iMageLightCastArray[0] = dwCurrentTick;
			iLastAttackTick	= dwCurrentTick;

			return;
		}
	}

	if(bBasicTurn[4]){	//basic 4
		dwCurrentCast = dwMageBasicCastArray[4];
		iCurrentData = iMageBasicDataArray[4];

		*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
		*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

		*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

		*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

		_CPacket.PSend(pBuf_Attack1,24);
		_CPacket.PSend(pBuf_Attack3,22);

		iMageBasicCastArray[4] = dwCurrentTick;
		iLastAttackTick	= dwCurrentTick;

		return;
	}

	if(bBasicTurn[3]){	//basic 3
		dwCurrentCast = dwMageBasicCastArray[3];
		iCurrentData = iMageBasicDataArray[3];

		*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
		*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

		*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

		*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

		_CPacket.PSend(pBuf_Attack1,24);
		_CPacket.PSend(pBuf_Attack3,22);

		iMageBasicCastArray[3] = dwCurrentTick;
		iLastAttackTick	= dwCurrentTick;

		return;
	}

	if(bBasicTurn[2]){	//basic 2
		dwCurrentCast = dwMageBasicCastArray[2];
		iCurrentData = iMageBasicDataArray[2];

		*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
		*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

		*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

		*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

		_CPacket.PSend(pBuf_Attack1,24);
		_CPacket.PSend(pBuf_Attack3,22);

		iMageBasicCastArray[2] = dwCurrentTick;
		iLastAttackTick	= dwCurrentTick;

		return;
	}

	if(bBasicTurn[1]){	//basic 1
		dwCurrentCast = dwMageBasicCastArray[1];
		iCurrentData = iMageBasicDataArray[1];

		*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
		*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

		*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

		*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

		_CPacket.PSend(pBuf_Attack1,24);
		_CPacket.PSend(pBuf_Attack3,22);

		iMageBasicCastArray[1] = dwCurrentTick;
		iLastAttackTick	= dwCurrentTick;

		return;
	}

	if(bBasicTurn[0]){	//basic 0
		dwCurrentCast = dwMageBasicCastArray[0];
		iCurrentData = iMageBasicDataArray[0];

		*(DWORD*)(pBuf_Attack1 + 2) = dwCurrentCast;
		*(DWORD*)(pBuf_Attack3 + 2) = dwCurrentCast;

		*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

		*(BYTE*)(pBuf_Attack1 + 22) = (BYTE)iCurrentData;

		_CPacket.PSend(pBuf_Attack1,24);
		_CPacket.PSend(pBuf_Attack3,22);

		iMageBasicCastArray[0] = dwCurrentTick;
		iLastAttackTick	= dwCurrentTick;

		return;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chlootcoinsClick(TObject *Sender)
{
	bLootCoins = chlootcoins->Checked;
}
//---------------------------------------------------------------------------

void CloseInnDlg()
{
	__asm{
		MOV EAX,[KO_DLGA]
		MOV EAX,[EAX + OFF_WAREHOUSE]
		MOV ECX,EAX
		MOV EAX,[EAX]
		PUSH 0
		CALL [EAX+0x48]
		MOV ECX,[KO_DLGA]
		MOV ECX,[ECX + OFF_WAREHOUSE]
		MOV EAX,KO_INNC
		CALL EAX
	}
}

void __fastcall TfrmBot::btnrprinfoClick(TObject *Sender)
{
	MessageBox(this->Handle,"Oncelikle slota gidin ve ilk olarak kasilacak slotta bir kez X,Y ekleyin. Daha sonra rota cizerek (X,Y ekleyerek) sundriese gidin ve en son sundries'in dibinde X,Y ekleyin, sundries'i secip ismini alin. Aktif isaretini secin. Daha sonra kasmaya baslayabilirsiniz. Ayni islem potcu ve banka icin de gecerlidir.","RPR-POT-BANKA",MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chonlyenemyClick(TObject *Sender)
{
	bOnlyEnemy = chonlyenemy->Checked;
	oplistmob->Enabled = !bOnlyEnemy;

	if(oplistmob->Checked){
		opautomob->Checked = true;
		iTargetingType = 0;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chonlytargetClick(TObject *Sender)
{
	bOnlyTarget = chonlytarget->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chtopmostClick(TObject *Sender)
{
	_CGeneral.SetWindowTop(this->Handle,chtopmost->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btntrayClick(TObject *Sender)
{
	if(hGameHWND)
		ShowWindow(hGameHWND,SW_HIDE);	//hide

	this->Hide();
	this->WindowState = wsMinimized;
	trayMain->Visible = true;
	trayMain->Animate = true;
	trayMain->ShowBalloonHint();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::trayMainClick(TObject *Sender)
{
	if(hGameHWND){
		ShowWindow(hGameHWND,SW_SHOW);	//show
		//AnimateWindow(hGameHWND,300,AW_BLEND);
		SetForegroundWindow(hGameHWND);
	}
	trayMain->Visible = false;
	this->Show();
	this->WindowState = wsNormal;
	Application->BringToFront(); // focus
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btngamewndClick(TObject *Sender)
{
	if(hGameHWND){
		//SendMessage(hGameHWND,WM_SIZE,SIZE_MAXSHOW,NULL);
		SetForegroundWindow(hGameHWND);
		Application->BringToFront(); // focus bot window
	}
}
//---------------------------------------------------------------------------


void __fastcall TfrmBot::opthroughClick(TObject *Sender)
{
	iSkillIndex = 3;
	dwCastNum = _CCast.MakeCastNum(_CCharBase,"500");
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chprimember1Click(TObject *Sender)
{
	bPriestMembers[0] = chprimember1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chprimember2Click(TObject *Sender)
{
	bPriestMembers[1] = chprimember2->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chprimember3Click(TObject *Sender)
{
	bPriestMembers[2] = chprimember3->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chprimember4Click(TObject *Sender)
{
	bPriestMembers[3] = chprimember4->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chprimember5Click(TObject *Sender)
{
	bPriestMembers[4] = chprimember5->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chprimember6Click(TObject *Sender)
{
	bPriestMembers[5] = chprimember6->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chprimember7Click(TObject *Sender)
{
	bPriestMembers[6] = chprimember7->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chprimember8Click(TObject *Sender)
{
	bPriestMembers[7] = chprimember8->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chprihealClick(TObject *Sender)
{
	bPriestHeal = chpriheal->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::ophealcastClick(TObject *Sender)
{
	iHealType = 1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::ophealotoClick(TObject *Sender)
{
	iHealType = 2;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::comhealChange(TObject *Sender)
{
	iHealCastIndex = comheal->ItemIndex;
	switch(iHealCastIndex){
		case 0:
		dwPriHealCastNum = _CCast.MakeCastNum(_CCharBase,"002");
		break;

		case 1:
		dwPriHealCastNum = _CCast.MakeCastNum(_CCharBase,"005");
		break;

		case 2:
		dwPriHealCastNum = _CCast.MakeCastNum(_CCharBase,"500");
		break;

		case 3:
		dwPriHealCastNum = _CCast.MakeCastNum(_CCharBase,"509");
		break;

		case 4:
		dwPriHealCastNum = _CCast.MakeCastNum(_CCharBase,"518");
		break;

		case 5:
		dwPriHealCastNum = _CCast.MakeCastNum(_CCharBase,"527");
		break;

		case 6:
		dwPriHealCastNum = _CCast.MakeCastNum(_CCharBase,"536");
		break;

		case 7:
		dwPriHealCastNum = _CCast.MakeCastNum(_CCharBase,"545");
		break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtheallimitChange(TObject *Sender)
{
	iHealLimit = atoi(txtheallimit->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chfirstlesshpClick(TObject *Sender)
{
	bHealLessHPFirst = chfirstlesshp->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chbuffClick(TObject *Sender)
{
	bBuff = chbuff->Checked;

	if(bBuff){
		if(!chac->Enabled)
			chac->Enabled = true;
		if(!chresist->Enabled)
			chresist->Enabled = true;
		if(!chpristr->Enabled)
			chpristr->Enabled = true;
	}
	else{
		chac->Checked = false;
		chresist->Checked = false;
		chpristr->Checked = false;
		bAC = false;
		bResist = false;
		bPriAutoSTR = false;
		chac->Enabled = false;
		chresist->Enabled = false;
		chpristr->Enabled = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chacClick(TObject *Sender)
{
	bAC = chac->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chresistClick(TObject *Sender)
{
	bResist = chresist->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::combuffChange(TObject *Sender)
{
	iBuffCastIndex = combuff->ItemIndex;
	switch(iBuffCastIndex){
		case 0:
		dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"606");
		break;

		case 1:
		dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"615");
		break;

		case 2:
		dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"624");
		break;

		case 3:
		dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"633");
		break;

		case 4:
		dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"642");
		break;

		case 5:
		dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"654");
		break;

		case 6:
		dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"655");
		break;

		case 7:
		dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"657");
		break;

		case 8:
		dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"670");
		break;

		case 9:
		dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"675");
		break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::comacChange(TObject *Sender)
{
	iACCastIndex = comac->ItemIndex;
	switch(iACCastIndex){
		case 0:
		dwPriACCastNum = _CCast.MakeCastNum(_CCharBase,"603");
		break;

		case 1:
		dwPriACCastNum = _CCast.MakeCastNum(_CCharBase,"612");
		break;

		case 2:
		dwPriACCastNum = _CCast.MakeCastNum(_CCharBase,"621");
		break;

		case 3:
		dwPriACCastNum = _CCast.MakeCastNum(_CCharBase,"630");
		break;

		case 4:
		dwPriACCastNum = _CCast.MakeCastNum(_CCharBase,"639");
		break;

		case 5:
		dwPriACCastNum = _CCast.MakeCastNum(_CCharBase,"651");
		break;

		case 6:
		dwPriACCastNum = _CCast.MakeCastNum(_CCharBase,"660");
		break;

		case 7:
		dwPriACCastNum = _CCast.MakeCastNum(_CCharBase,"674");
		break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::comresistChange(TObject *Sender)
{
	iResistCastIndex = comresist->ItemIndex;
	switch(iResistCastIndex){
		case 0:
		dwPriResistCastNum = _CCast.MakeCastNum(_CCharBase,"609");
		break;

		case 1:
		dwPriResistCastNum = _CCast.MakeCastNum(_CCharBase,"627");
		break;

		case 2:
		dwPriResistCastNum = _CCast.MakeCastNum(_CCharBase,"636");
		break;

		case 3:
		dwPriResistCastNum = _CCast.MakeCastNum(_CCharBase,"645");
		break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chgroupClick(TObject *Sender)
{
	bGroupHeal = chgroup->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chrestoreClick(TObject *Sender)
{
	bRestore = chrestore->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::comgroupChange(TObject *Sender)
{
	iGroupCastIndex = comgroup->ItemIndex;
	switch(iGroupCastIndex){
		case 0:
		dwPriGroupCastNum = _CCast.MakeCastNum(_CCharBase,"557");
		break;

		case 1:
		dwPriGroupCastNum = _CCast.MakeCastNum(_CCharBase,"560");
		break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::comrestoreChange(TObject *Sender)
{
	iRestoreCastIndex = comrestore->ItemIndex;
	switch(iRestoreCastIndex){
		case 0:
		dwPriRestoreCastNum = _CCast.MakeCastNum(_CCharBase,"503");
		break;

		case 1:
		dwPriRestoreCastNum = _CCast.MakeCastNum(_CCharBase,"512");
		break;

		case 2:
		dwPriRestoreCastNum = _CCast.MakeCastNum(_CCharBase,"521");
		break;

		case 3:
		dwPriRestoreCastNum = _CCast.MakeCastNum(_CCharBase,"530");
		break;

		case 4:
		dwPriRestoreCastNum = _CCast.MakeCastNum(_CCharBase,"539");
		break;

		case 5:
		dwPriRestoreCastNum = _CCast.MakeCastNum(_CCharBase,"548");
		break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chmaliceClick(TObject *Sender)
{
	bMalice = chmalice->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chparasiteClick(TObject *Sender)
{
	bParasite = chparasite->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chtormentClick(TObject *Sender)
{
	bTorment = chtorment->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chmassiveClick(TObject *Sender)
{
	bMassive = chmassive->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chchathealClick(TObject *Sender)
{
	bChatHeal = chchatheal->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chchatbuffClick(TObject *Sender)
{
	bChatBuff = chchatbuff->Checked;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chprisitClick(TObject *Sender)
{
	bSitWhenIdle = chprisit->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chpricureClick(TObject *Sender)
{
	bPriAutoCure = chpricure->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chpristrClick(TObject *Sender)
{
	bPriAutoSTR = chpristr->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chpribookClick(TObject *Sender)
{
	bPriAutoBook = chpribook->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmpriestTimer(TObject *Sender)
{

	if(bBotPaused)	//paused
		return;

	if(CheckLoading() == true)
		return;

	if(!_CCharBase.GetDWORD(KO_CRHP))	//dead
		return;

	if(!bSitWhenIdle){
		if(_CCharBase.IsSit())
			return;
	}

	int i;
	DWORD dwTmpUser;
	FLOAT fPrevHPPerc = 0;
	int	iFirstHealIndex = -1;
	FLOAT	fTmpPerc;
	DWORD dwCurrentTick = GetTickCount();
	bool bTurnGroupHeal = (bGroupHeal && dwCurrentTick - iLastGroupTick >= 10000);

	DWORD iMyHP,iMyMaxHP;
	bool bMyCurse,bMyDisease;
	int iMyClass;

	//party info/////////
	bool bUserArray[8] = {0,0,0,0,0,0,0,0};
	DWORD dwIDArray[8];
	DWORD dwHPArray[8];
	DWORD dwMaxHPArray[8];
	int iClassArray[8];
	bool bCurseArray[8];
	bool bDiseaseArray[8];
	char* strNameArray[8];
	CCharBase pUserArray[8];
	float fDistArray[8];
	/////////////////////

	_CParty.Refresh();	//initialize

	int iPartyCount = _CParty.GetCount();
	if(iPartyCount < 2)
		goto SELF;

PARTY:

	for (i = 0; i < iPartyCount; i++) {
		dwTmpUser = _CParty.GetUser(i);
		if(!dwTmpUser) continue;
		dwIDArray[i] = *(WORD*)(dwTmpUser + KO_PT_ID);
		dwHPArray[i] = *(DWORD*)(dwTmpUser + KO_PT_HP);
		dwMaxHPArray[i] = *(DWORD*)(dwTmpUser + KO_PT_MAXHP);
		iClassArray[i] = *(DWORD*)(dwTmpUser + KO_PT_CLASS);
		bCurseArray[i] = *(BYTE*)(dwTmpUser + KO_PT_CURSE);
		bDiseaseArray[i] = *(BYTE*)(dwTmpUser + KO_PT_DISEASE);
		strNameArray[i] = (LPSTR)(*(DWORD*)(dwTmpUser + KO_PT_NICK));

		if(dwIDArray[i] == dwMyID){
			pUserArray[i] = _CCharBase;	//self
			fDistArray[i] = 0;
			if(iPriestHealLockIndex == i)
				iPriestHealLockIndex = -1;
		}
		else{
			pUserArray[i] = _CArea.GetUPCBase(dwIDArray[i]);
			if(pUserArray[i].GetBase())
				fDistArray[i] = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pUserArray[i].GetFLOAT(KO_POSX),pUserArray[i].GetFLOAT(KO_POSY));
			else{
				fDistArray[i] = -1;
				if(iPriestHealLockIndex == i)
					iPriestHealLockIndex = -1;
			}
		}

		bUserArray[i] = true;
	}

	for (i = iPartyCount; i < 8; i++) {
		iLastRestoredUserTick[i] = 0;
		iLastUserMaxHP[i] = 0;
		bUserWaitingForNewHP[i] = false;
		iLastBuffedUserTick[i] = 0;
		if(iPriestHealLockIndex == i)
			iPriestHealLockIndex = -1;
	}

	if(bUserArray[0])
		lbmember1->Caption = strNameArray[0];
	else
		lbmember1->Caption = "---";

	if(bUserArray[1])
		lbmember2->Caption = strNameArray[1];
	else
		lbmember2->Caption = "---";

	if(bUserArray[2])
		lbmember3->Caption = strNameArray[2];
	else
		lbmember3->Caption = "---";

	if(bUserArray[3])
		lbmember4->Caption = strNameArray[3];
	else
		lbmember4->Caption = "---";

	if(bUserArray[4])
		lbmember5->Caption = strNameArray[4];
	else
		lbmember5->Caption = "---";

	if(bUserArray[5])
		lbmember6->Caption = strNameArray[5];
	else
		lbmember6->Caption = "---";

	if(bUserArray[6])
		lbmember7->Caption = strNameArray[6];
	else
		lbmember7->Caption = "---";

	if(bUserArray[7])
		lbmember8->Caption = strNameArray[7];
	else
		lbmember8->Caption = "---";

	//HEAL////////////////////////////////////////////////////////////////////
	if(dwCurrentTick - iLastHealTick >= 900){

		if(bTurnGroupHeal){
			GroupHeal(true);
			iLastHealTick = dwCurrentTick;
			iLastGroupTick = dwCurrentTick;
		}

		if(!bTurnGroupHeal && iPriestHealLockIndex > -1){
			if(dwHPArray[iPriestHealLockIndex]
			&& bUserArray[iPriestHealLockIndex]
			&& bPriestMembers[iPriestHealLockIndex]
			&& fDistArray[iPriestHealLockIndex] > -1
			&& fDistArray[iPriestHealLockIndex] < 11.0){
				if(dwHPArray[iPriestHealLockIndex] != dwMaxHPArray[iPriestHealLockIndex]){
					if(HealUser(dwIDArray[iPriestHealLockIndex],dwMaxHPArray[iPriestHealLockIndex] - dwHPArray[iPriestHealLockIndex]) == true)
						iLastHealTick = dwCurrentTick;
				}
				else{
					iPriestHealLockIndex = -1;
                }
			}
		}

		if(!bTurnGroupHeal && iPriestHealLockIndex == -1 && bPriestHeal){
			for (i = 0; i < iPartyCount; i++) {
				if(dwHPArray[i] && bUserArray[i] && bPriestMembers[i] && fDistArray[i] > -1 && fDistArray[i] < 11.0){
					fTmpPerc = _CMath.iPerc(dwMaxHPArray[i],dwHPArray[i]);
					if(fTmpPerc <= iHealLimit){
						if(bHealLessHPFirst){
							if(!fPrevHPPerc){
								fPrevHPPerc = fTmpPerc;
								iFirstHealIndex = i;
							}
							else{
								if(fTmpPerc < fPrevHPPerc)
									fPrevHPPerc = fTmpPerc;
									iFirstHealIndex = i;
							}
						}
						else{
							if(HealUser(dwIDArray[i],dwMaxHPArray[i] - dwHPArray[i]) == true){
								iLastHealTick = dwCurrentTick;
								break;	//end
							}
						}
					}
				}
			}

				if(bHealLessHPFirst && iFirstHealIndex > -1){
					if(HealUser(dwIDArray[iFirstHealIndex],dwMaxHPArray[iFirstHealIndex] - dwHPArray[iFirstHealIndex]) == true)
						iLastHealTick = dwCurrentTick;
				}
		}

		if(bRestore){
			if(dwCurrentTick - iLastHealTick >= 900){
				for (i = 0; i < iPartyCount; i++) {
					if(dwHPArray[i] && bUserArray[i] && bPriestMembers[i] && fDistArray[i] > -1 && fDistArray[i] < 11.0){
						if(dwCurrentTick - iLastRestoredUserTick[i] >= 35000){
							if(RestoreUser(dwIDArray[i]) == true){
								iLastHealTick = dwCurrentTick;
								iLastRestoredUserTick[i] = dwCurrentTick;
								break;
							}
						}
					}
					else if(fDistArray[i] > -1 && (!dwHPArray[i] || !pUserArray[i].IsAlive()))
						iLastRestoredUserTick[i] = 0;
				}
			}
		}
	}


	///////////////////////////////////////////////////////////////

	//BUFF/////////////////////////////////////////////////////////
	if(bBuff){
		if(dwCurrentTick - iLastBuffTick >= 900){
			for (i = 0; i < iPartyCount; i++) {
				if(dwHPArray[i] && bUserArray[i] && bPriestMembers[i] && fDistArray[i] > -1 && fDistArray[i] < 11.0){
					if(dwCurrentTick - iLastBuffedUserTick[i] > 3000){
						if(bUserWaitingForNewHP[i]){
							iLastUserMaxHP[i] = dwMaxHPArray[i];
							bUserWaitingForNewHP[i] = false;
						}
						else{
							if((int)dwMaxHPArray[i] != iLastUserMaxHP[i]){
								if(bAC && dwCurrentTick - iLastACTick >= 900){
									if(ACUser(dwIDArray[i]) == true)
										iLastACTick = dwCurrentTick;
								}
								if(bResist && dwCurrentTick - iLastResistTick >= 900){
									if(ResistUser(dwIDArray[i]) == true)
										iLastResistTick = dwCurrentTick;
								}
								if(bPriAutoSTR && dwCurrentTick - iLastPriSTRTick >= 900){
									if(STRUser(dwIDArray[i]) == true)
										iLastPriSTRTick = dwCurrentTick;
								}
								if(BuffUser(dwIDArray[i],iClassArray[i]) == true){
									iLastBuffTick = dwCurrentTick;
									iLastBuffedUserTick[i] = dwCurrentTick;
									bUserWaitingForNewHP[i] = true;
									break;
								}
							}
						}
					}
				}
				else if(fDistArray[i] > -1 && (!dwHPArray[i] || !pUserArray[i].IsAlive())){
					iLastUserMaxHP[i] = 0;
					bUserWaitingForNewHP[i] = false;
				}
			}
		}
	}

	///CURE//////////////////////////////////////////////////////////////
	if(bPriAutoCure){
		if(dwCurrentTick - iLastPriCureTick >= 3000){
			for (i = 0; i < iPartyCount; i++) {
				if(dwHPArray[i] && bUserArray[i] && bPriestMembers[i] && fDistArray[i] > -1 && fDistArray[i] < 11.0){
					if(bCurseArray[i]){
						if(CureCurseUser(dwIDArray[i]) == true){
							iLastPriCureTick = dwCurrentTick;
							break;
						}
					}
				}
			}
		}

		if(dwCurrentTick - iLastPriDiseaseTick >= 3000){
			for (i = 0; i < iPartyCount; i++) {
				if(dwHPArray[i] && bUserArray[i] && bPriestMembers[i] && fDistArray[i] > -1 && fDistArray[i] < 11.0){
					if(bDiseaseArray[i]){
						if(CureDiseaseUser(dwIDArray[i]) == true){
							iLastPriDiseaseTick = dwCurrentTick;
							break;
						}
					}
				}
			}
		}
	}

	return;

SELF:

	for (i = 0; i < 8; i++) {
		iLastRestoredUserTick[i] = 0;
		iLastUserMaxHP[i] = 0;
		bUserWaitingForNewHP[i] = false;
		iLastBuffedUserTick[i] = 0;
	}

	iPriestHealLockIndex = -1;

	lbmember1->Caption = "---";
	lbmember2->Caption = "---";
	lbmember3->Caption = "---";
	lbmember4->Caption = "---";
	lbmember5->Caption = "---";
	lbmember6->Caption = "---";
	lbmember7->Caption = "---";
	lbmember8->Caption = "---";

	iMyHP = _CCharBase.GetDWORD(KO_CRHP);
	iMyMaxHP = _CCharBase.GetDWORD(KO_MXHP);
	bMyCurse = CheckSelfCure();//test
	//bMyDisease = false;//test
	bMyDisease = bMyCurse;
	iMyClass = _CCharBase.GetDWORD(KO_CLSS);

	if(bPriestHeal){
		if(dwCurrentTick - iLastHealTick >= 900){
			if(_CMath.iPerc(iMyMaxHP,iMyHP) <= (float)iHealLimit){
				if(HealUser(dwMyID,iMyMaxHP - iMyHP) == true)
					iLastHealTick = dwCurrentTick;
			}
		}
	}

	if(bRestore){
		if(dwCurrentTick - iLastHealTick >= 900 && dwCurrentTick - iPriestMyLastRestoreTick >= 3000){
			if(RestoreUser(dwMyID) == true){
				iLastHealTick = dwCurrentTick;
				iPriestMyLastRestoreTick = dwCurrentTick;
			}
		}
	}

	if(bBuff){
		if(dwCurrentTick - iLastBuffTick >= 900 && dwCurrentTick - iPriestMyLastBuffTick >= 3000){
			if(BuffUser(dwMyID,iMyClass) == true){
				iLastBuffTick = dwCurrentTick;
				iPriestMyLastBuffTick = dwCurrentTick;
			}
		}
		if(bAC && dwCurrentTick - iLastACTick >= 900 && dwCurrentTick - iPriestMyLastACTick >= 3000){
			if(ACUser(dwMyID) == true){
				iLastACTick = dwCurrentTick;
				iPriestMyLastACTick = dwCurrentTick;
			}
		}
		if(bResist && dwCurrentTick - iLastResistTick >= 900 && dwCurrentTick - iPriestMyLastResistTick >= 3000){
			if(ResistUser(dwMyID) == true){
				iLastResistTick = dwCurrentTick;
				iPriestMyLastResistTick = dwCurrentTick;
			}
		}
		if(bPriAutoSTR && dwCurrentTick - iLastPriSTRTick >= 900 && dwCurrentTick - iPriestMyLastSTRTick >= 3000){
			if(STRUser(dwMyID) == true){
				iLastPriSTRTick = dwCurrentTick;
				iPriestMyLastSTRTick = dwCurrentTick;
			}
		}
	}

	if(bPriAutoCure){
		if(dwCurrentTick - iLastPriCureTick >= 3000){
			if(bMyCurse){
				if(CureCurseUser(dwMyID) == true)
					iLastPriCureTick = dwCurrentTick;
			}
		}

		if(dwCurrentTick - iLastPriDiseaseTick >= 3000){
			if(bMyDisease){
				if(CureDiseaseUser(dwMyID) == true)
					iLastPriDiseaseTick = dwCurrentTick;
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chprintnameClick(TObject *Sender)
{
	bPriPrintName = chprintname->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chundyClick(TObject *Sender)
{
	bUndyForWarriors = chundy->Checked;
}
//---------------------------------------------------------------------------

bool CheckSelfCure(){
	DWORD dwTmp = *(DWORD*)(KO_DLGA);
	dwTmp = *(DWORD*)(dwTmp + 0x338);
	dwTmp = *(DWORD*)(dwTmp + 0xFC + 0x14);
	if(dwTmp)
		return true;
	else
		return false;
}

bool HealUser(DWORD ID,int iNeededHP){

	DWORD dwCastNum = frmBot->dwPriHealCastNum;

	if(frmBot->iHealType == 2)	//auto
		dwCastNum = MakeHealCastNum(iNeededHP);

	if(!dwCastNum)
		return false;

	if(frmBot->bSitWhenIdle){	//priest
		if(_CCharBase.IsSit())
			_CTest.CommandSit(false);
	}

	*(DWORD*)(pBuf_BuffSelf1 + 2) = dwCastNum;
	*(DWORD*)(pBuf_BuffSelf2 + 2) = dwCastNum;

	*(WORD*)(pBuf_BuffSelf1 + 8) = ID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = ID;

	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x0F;
	_CPacket.PSend(pBuf_BuffSelf1,24);
	_CPacket.PSend(pBuf_BuffSelf2,22);
	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x00;

	*(WORD*)(pBuf_BuffSelf1 + 8) = dwMyID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = dwMyID;

	if(frmBot->bSitWhenIdle)	//priest
		_CTest.CommandSit(true);

	return true;
}

DWORD MakeHealCastNum(int iNeededHP){
	if(!frmBot->bHealCastArray[0])
		return 0;

	if(iNeededHP <= 15)
		return frmBot->dwHealCastArray[0];
	if(iNeededHP <= 30 && frmBot->bHealCastArray[1])
		return frmBot->dwHealCastArray[1];
	if(iNeededHP <= 60 && frmBot->bHealCastArray[2])
		return frmBot->dwHealCastArray[2];
	if(iNeededHP <= 240 && frmBot->bHealCastArray[3])
		return frmBot->dwHealCastArray[3];
	if(iNeededHP <= 360 && frmBot->bHealCastArray[4])
		return frmBot->dwHealCastArray[4];
	if(iNeededHP <= 720 && frmBot->bHealCastArray[5])
		return frmBot->dwHealCastArray[5];
	if(iNeededHP <= 960 && frmBot->bHealCastArray[6])
		return frmBot->dwHealCastArray[6];
	if(iNeededHP > 960 && frmBot->bHealCastArray[7])
		return frmBot->dwHealCastArray[7];	//1920

	for (int i = 7; i >= 0; i--) {
		if(frmBot->bHealCastArray[i])
			return frmBot->dwHealCastArray[i];
	}

	return 0;
}

void CheckHealCasts(){
	DWORD dwCastNum;
	bool bAvailable;

	dwCastNum = _CCast.MakeCastNum(_CCharBase,"002");
	bAvailable = _CTest.CheckSkillAvailable(dwCastNum);
	frmBot->bHealCastArray[0] = bAvailable;
	frmBot->dwHealCastArray[0] = dwCastNum;

	dwCastNum = _CCast.MakeCastNum(_CCharBase,"005");
	bAvailable = _CTest.CheckSkillAvailable(dwCastNum);
	frmBot->bHealCastArray[1] = bAvailable;
	frmBot->dwHealCastArray[1] = dwCastNum;

	dwCastNum = _CCast.MakeCastNum(_CCharBase,"500");
	bAvailable = _CTest.CheckSkillAvailable(dwCastNum);
	frmBot->bHealCastArray[2] = bAvailable;
	frmBot->dwHealCastArray[2] = dwCastNum;

	dwCastNum = _CCast.MakeCastNum(_CCharBase,"509");
	bAvailable = _CTest.CheckSkillAvailable(dwCastNum);
	frmBot->bHealCastArray[3] = bAvailable;
	frmBot->dwHealCastArray[3] = dwCastNum;

	dwCastNum = _CCast.MakeCastNum(_CCharBase,"518");
	bAvailable = _CTest.CheckSkillAvailable(dwCastNum);
	frmBot->bHealCastArray[4] = bAvailable;
	frmBot->dwHealCastArray[4] = dwCastNum;

	dwCastNum = _CCast.MakeCastNum(_CCharBase,"527");
	bAvailable = _CTest.CheckSkillAvailable(dwCastNum);
	frmBot->bHealCastArray[5] = bAvailable;
	frmBot->dwHealCastArray[5] = dwCastNum;

	dwCastNum = _CCast.MakeCastNum(_CCharBase,"536");
	bAvailable = _CTest.CheckSkillAvailable(dwCastNum);
	frmBot->bHealCastArray[6] = bAvailable;
	frmBot->dwHealCastArray[6] = dwCastNum;

	dwCastNum = _CCast.MakeCastNum(_CCharBase,"545");
	bAvailable = _CTest.CheckSkillAvailable(dwCastNum);
	frmBot->bHealCastArray[7] = bAvailable;
	frmBot->dwHealCastArray[7] = dwCastNum;
}

bool BuffUser(DWORD ID,int iClass){

	DWORD dwCastNum = frmBot->dwPriBuffCastNum;

	if(ID == dwMyID){
		if(_CCast.IsCasted(dwCastNum) == true)
			return false;
	}

	if(frmBot->bUndyForWarriors){
		if(IsWarrior(iClass) == true)
			dwCastNum = frmBot->dwPriUndyCastNum;
	}

	if(frmBot->bSitWhenIdle){	//priest
		if(_CCharBase.IsSit())
			_CTest.CommandSit(false);
	}

	*(DWORD*)(pBuf_BuffSelf1 + 2) = dwCastNum;
	*(DWORD*)(pBuf_BuffSelf2 + 2) = dwCastNum;

	*(WORD*)(pBuf_BuffSelf1 + 8) = ID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = ID;

	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x0F;
	_CPacket.PSend(pBuf_BuffSelf1,24);
	_CPacket.PSend(pBuf_BuffSelf2,22);
	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x00;

	*(WORD*)(pBuf_BuffSelf1 + 8) = dwMyID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = dwMyID;

	if(frmBot->bSitWhenIdle)	//priest
		_CTest.CommandSit(true);

	return true;
}

bool IsWarrior(int iClass){

	switch(iClass){
		case 101:
		case 105:
		case 106:
		case 201:
		case 205:
		case 206:
		return true;
	}

	return false;
}

bool ACUser(DWORD ID){

	DWORD dwCastNum = frmBot->dwPriACCastNum;

	if(ID == dwMyID){
		if(_CCast.IsCasted(dwCastNum) == true)
			return false;
	}

	if(frmBot->bSitWhenIdle){	//priest
		if(_CCharBase.IsSit())
			_CTest.CommandSit(false);
	}

	*(DWORD*)(pBuf_BuffSelf1 + 2) = dwCastNum;
	*(DWORD*)(pBuf_BuffSelf2 + 2) = dwCastNum;

	*(WORD*)(pBuf_BuffSelf1 + 8) = ID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = ID;

	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x0F;
	_CPacket.PSend(pBuf_BuffSelf1,24);
	_CPacket.PSend(pBuf_BuffSelf2,22);
	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x00;

	*(WORD*)(pBuf_BuffSelf1 + 8) = dwMyID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = dwMyID;

	if(frmBot->bSitWhenIdle)	//priest
		_CTest.CommandSit(true);

	return true;
}

bool ResistUser(DWORD ID){

	DWORD dwCastNum = frmBot->dwPriResistCastNum;

	if(ID == dwMyID){
		if(_CCast.IsCasted(dwCastNum) == true)
			return false;
	}

	if(frmBot->bSitWhenIdle){	//priest
		if(_CCharBase.IsSit())
			_CTest.CommandSit(false);
	}

	*(DWORD*)(pBuf_BuffSelf1 + 2) = dwCastNum;
	*(DWORD*)(pBuf_BuffSelf2 + 2) = dwCastNum;

	*(WORD*)(pBuf_BuffSelf1 + 8) = ID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = ID;

	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x0F;
	_CPacket.PSend(pBuf_BuffSelf1,24);
	_CPacket.PSend(pBuf_BuffSelf2,22);
	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x00;

	*(WORD*)(pBuf_BuffSelf1 + 8) = dwMyID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = dwMyID;

	if(frmBot->bSitWhenIdle)	//priest
		_CTest.CommandSit(true);

	return true;
}

bool RestoreUser(DWORD ID){

	DWORD dwCastNum = frmBot->dwPriRestoreCastNum;

	if(ID == dwMyID){
		if(_CCast.IsCasted(dwCastNum) == true)
			return false;
	}

	if(frmBot->bSitWhenIdle){	//priest
		if(_CCharBase.IsSit())
			_CTest.CommandSit(false);
	}

	*(DWORD*)(pBuf_BuffSelf1 + 2) = dwCastNum;
	*(DWORD*)(pBuf_BuffSelf2 + 2) = dwCastNum;

	*(WORD*)(pBuf_BuffSelf1 + 8) = ID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = ID;

	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x0F;
	_CPacket.PSend(pBuf_BuffSelf1,24);
	_CPacket.PSend(pBuf_BuffSelf2,22);
	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x00;

	*(WORD*)(pBuf_BuffSelf1 + 8) = dwMyID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = dwMyID;

	if(frmBot->bSitWhenIdle)	//priest
		_CTest.CommandSit(true);

	return true;
}

bool GroupHeal(bool bParty){

	if(frmBot->bSitWhenIdle){	//priest
		if(_CCharBase.IsSit())
			_CTest.CommandSit(false);
	}

	DWORD dwCastNum = frmBot->dwPriGroupCastNum;

	*(DWORD*)(pBuf_BuffSelf1 + 2) = dwCastNum;
	*(DWORD*)(pBuf_BuffSelf2 + 2) = dwCastNum;

	if(bParty){
		*(WORD*)(pBuf_BuffSelf1 + 8) = 0xFFFF;
		*(WORD*)(pBuf_BuffSelf2 + 8) = 0xFFFF;
	}
	else{
		*(WORD*)(pBuf_BuffSelf1 + 8) = dwMyID;
		*(WORD*)(pBuf_BuffSelf2 + 8) = dwMyID;
	}

	int iPosX = (int)_CCharBase.GetFLOAT(KO_POSX);
	int iPosY = (int)_CCharBase.GetFLOAT(KO_POSY);
	int iPosZ = (int)_CCharBase.GetFLOAT(KO_POSZ);

	*(WORD*)(pBuf_BuffSelf1 + 10) = iPosX;
	*(WORD*)(pBuf_BuffSelf2 + 10) = iPosX;
	*(WORD*)(pBuf_BuffSelf1 + 12) = iPosZ;
	*(WORD*)(pBuf_BuffSelf2 + 12) = iPosZ;
	*(WORD*)(pBuf_BuffSelf1 + 14) = iPosY;
	*(WORD*)(pBuf_BuffSelf2 + 14) = iPosY;

	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x0F;
	_CPacket.PSend(pBuf_BuffSelf1,24);
	_CPacket.PSend(pBuf_BuffSelf2,22);
	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x00;

	*(WORD*)(pBuf_BuffSelf1 + 10) = 0x0000;
	*(WORD*)(pBuf_BuffSelf2 + 10) = 0x0000;
	*(WORD*)(pBuf_BuffSelf1 + 12) = 0x0000;
	*(WORD*)(pBuf_BuffSelf2 + 12) = 0x0000;
	*(WORD*)(pBuf_BuffSelf1 + 14) = 0x0000;
	*(WORD*)(pBuf_BuffSelf2 + 14) = 0x0000;

	*(WORD*)(pBuf_BuffSelf1 + 8) = dwMyID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = dwMyID;

	if(frmBot->bSitWhenIdle)	//priest
		_CTest.CommandSit(true);

	return true;
}

bool CureCurseUser(DWORD ID){

	if(frmBot->bSitWhenIdle){	//priest
		if(_CCharBase.IsSit())
			_CTest.CommandSit(false);
	}

	DWORD dwCastNum = frmBot->dwPriCureCastNum;

	*(DWORD*)(pBuf_BuffSelf1 + 2) = dwCastNum;
	*(DWORD*)(pBuf_BuffSelf2 + 2) = dwCastNum;

	*(WORD*)(pBuf_BuffSelf1 + 8) = ID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = ID;

	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x0F;
	_CPacket.PSend(pBuf_BuffSelf1,24);
	_CPacket.PSend(pBuf_BuffSelf2,22);
	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x00;

	*(WORD*)(pBuf_BuffSelf1 + 8) = dwMyID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = dwMyID;

	if(frmBot->bSitWhenIdle)	//priest
		_CTest.CommandSit(true);

	return true;
}

bool CureDiseaseUser(DWORD ID){

	if(frmBot->bSitWhenIdle){	//priest
		if(_CCharBase.IsSit())
			_CTest.CommandSit(false);
	}

	DWORD dwCastNum = frmBot->dwPriDiseaseCastNum;

	*(DWORD*)(pBuf_BuffSelf1 + 2) = dwCastNum;
	*(DWORD*)(pBuf_BuffSelf2 + 2) = dwCastNum;

	*(WORD*)(pBuf_BuffSelf1 + 8) = ID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = ID;

	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x0F;
	_CPacket.PSend(pBuf_BuffSelf1,24);
	_CPacket.PSend(pBuf_BuffSelf2,22);
	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x00;

	*(WORD*)(pBuf_BuffSelf1 + 8) = dwMyID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = dwMyID;

	if(frmBot->bSitWhenIdle)	//priest
		_CTest.CommandSit(true);

	return true;
}

bool STRUser(DWORD ID){

	DWORD dwCastNum = frmBot->dwPriSTRCastNum;

	if(ID == dwMyID){
		if(_CCast.IsCasted(dwCastNum) == true)
			return false;

		if(_CCast.IsCasted(frmBot->dwPriestSTR30CastNum) == true)
			return false;
	}

	if(frmBot->bSitWhenIdle){	//priest
		if(_CCharBase.IsSit())
			_CTest.CommandSit(false);
	}

	*(DWORD*)(pBuf_BuffSelf1 + 2) = dwCastNum;
	*(DWORD*)(pBuf_BuffSelf2 + 2) = dwCastNum;

	*(WORD*)(pBuf_BuffSelf1 + 8) = ID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = ID;

	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x0F;
	_CPacket.PSend(pBuf_BuffSelf1,24);
	_CPacket.PSend(pBuf_BuffSelf2,22);
	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x00;

	*(WORD*)(pBuf_BuffSelf1 + 8) = dwMyID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = dwMyID;

	if(frmBot->bSitWhenIdle)	//priest
		_CTest.CommandSit(true);

	return true;
}

void RefreshPriestCasts(){

	frmBot->iHealCastIndex = frmBot->comheal->ItemIndex;
	switch(frmBot->iHealCastIndex){
		case 0:
		frmBot->dwPriHealCastNum = _CCast.MakeCastNum(_CCharBase,"002");
		break;

		case 1:
		frmBot->dwPriHealCastNum = _CCast.MakeCastNum(_CCharBase,"005");
		break;

		case 2:
		frmBot->dwPriHealCastNum = _CCast.MakeCastNum(_CCharBase,"500");
		break;

		case 3:
		frmBot->dwPriHealCastNum = _CCast.MakeCastNum(_CCharBase,"509");
		break;

		case 4:
		frmBot->dwPriHealCastNum = _CCast.MakeCastNum(_CCharBase,"518");
		break;

		case 5:
		frmBot->dwPriHealCastNum = _CCast.MakeCastNum(_CCharBase,"527");
		break;

		case 6:
		frmBot->dwPriHealCastNum = _CCast.MakeCastNum(_CCharBase,"536");
		break;

		case 7:
		frmBot->dwPriHealCastNum = _CCast.MakeCastNum(_CCharBase,"545");
		break;
	}

	frmBot->iBuffCastIndex = frmBot->combuff->ItemIndex;
	switch(frmBot->iBuffCastIndex){
		case 0:
		frmBot->dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"606");
		break;

		case 1:
		frmBot->dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"615");
		break;

		case 2:
		frmBot->dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"624");
		break;

		case 3:
		frmBot->dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"633");
		break;

		case 4:
		frmBot->dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"642");
		break;

		case 5:
		frmBot->dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"654");
		break;

		case 6:
		frmBot->dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"655");
		break;

		case 7:
		frmBot->dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"657");
		break;

		case 8:
		frmBot->dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"670");
		break;

		case 9:
		frmBot->dwPriBuffCastNum = _CCast.MakeCastNum(_CCharBase,"675");
		break;
	}

	frmBot->iACCastIndex = frmBot->comac->ItemIndex;
	switch(frmBot->iACCastIndex){
		case 0:
		frmBot->dwPriACCastNum = _CCast.MakeCastNum(_CCharBase,"603");
		break;

		case 1:
		frmBot->dwPriACCastNum = _CCast.MakeCastNum(_CCharBase,"612");
		break;

		case 2:
		frmBot->dwPriACCastNum = _CCast.MakeCastNum(_CCharBase,"621");
		break;

		case 3:
		frmBot->dwPriACCastNum = _CCast.MakeCastNum(_CCharBase,"630");
		break;

		case 4:
		frmBot->dwPriACCastNum = _CCast.MakeCastNum(_CCharBase,"639");
		break;

		case 5:
		frmBot->dwPriACCastNum = _CCast.MakeCastNum(_CCharBase,"651");
		break;

		case 6:
		frmBot->dwPriACCastNum = _CCast.MakeCastNum(_CCharBase,"660");
		break;

		case 7:
		frmBot->dwPriACCastNum = _CCast.MakeCastNum(_CCharBase,"674");
		break;
	}

	frmBot->iResistCastIndex = frmBot->comresist->ItemIndex;
	switch(frmBot->iResistCastIndex){
		case 0:
		frmBot->dwPriResistCastNum = _CCast.MakeCastNum(_CCharBase,"609");
		break;

		case 1:
		frmBot->dwPriResistCastNum = _CCast.MakeCastNum(_CCharBase,"627");
		break;

		case 2:
		frmBot->dwPriResistCastNum = _CCast.MakeCastNum(_CCharBase,"636");
		break;

		case 3:
		frmBot->dwPriResistCastNum = _CCast.MakeCastNum(_CCharBase,"645");
		break;
	}

	frmBot->iGroupCastIndex = frmBot->comgroup->ItemIndex;
	switch(frmBot->iGroupCastIndex){
		case 0:
		frmBot->dwPriGroupCastNum = _CCast.MakeCastNum(_CCharBase,"557");
		break;

		case 1:
		frmBot->dwPriGroupCastNum = _CCast.MakeCastNum(_CCharBase,"560");
		break;
	}

	frmBot->iRestoreCastIndex = frmBot->comrestore->ItemIndex;
	switch(frmBot->iRestoreCastIndex){
		case 0:
		frmBot->dwPriRestoreCastNum = _CCast.MakeCastNum(_CCharBase,"503");
		break;

		case 1:
		frmBot->dwPriRestoreCastNum = _CCast.MakeCastNum(_CCharBase,"512");
		break;

		case 2:
		frmBot->dwPriRestoreCastNum = _CCast.MakeCastNum(_CCharBase,"521");
		break;

		case 3:
		frmBot->dwPriRestoreCastNum = _CCast.MakeCastNum(_CCharBase,"530");
		break;

		case 4:
		frmBot->dwPriRestoreCastNum = _CCast.MakeCastNum(_CCharBase,"539");
		break;

		case 5:
		frmBot->dwPriRestoreCastNum = _CCast.MakeCastNum(_CCharBase,"548");
		break;
	}

	frmBot->dwPriCureCastNum = _CCast.MakeCastNum(_CCharBase,"525");
	frmBot->dwPriDiseaseCastNum = _CCast.MakeCastNum(_CCharBase,"535");
	frmBot->dwPriSTRCastNum = _CCast.MakeCastNum(_CCharBase,"004");
	frmBot->dwPriUndyCastNum = _CCast.MakeCastNum(_CCharBase,"654");
}
void __fastcall TfrmBot::chrattackClick(TObject *Sender)
{
	bRAttack = chrattack->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chrunmobClick(TObject *Sender)
{
	if(bRunToMob && (!iBotCenterX || !iBotCenterY)){
		MessageBox(this->Handle,"Once merkez almalisiniz","hata",MB_ICONEXCLAMATION);
		chrunmob->Checked = false;
		bRunToMob = false;
		return;
	}

	bRunToMob = chrunmob->Checked;

	bRunMobPaused = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtrundistChange(TObject *Sender)
{
	iRunDist = atoi(txtrundist->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chrockClick(TObject *Sender)
{
	bRock = chrock->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chpercent5Click(TObject *Sender)
{
	bAsasPercent5 = chpercent5->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chpercent10Click(TObject *Sender)
{
	bAsasPercent10 = chpercent10->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chbindingClick(TObject *Sender)
{
	bBinding = chbinding->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chprovokeClick(TObject *Sender)
{
	bProvoke = chprovoke->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chpriattackClick(TObject *Sender)
{
	bPriestAttack = chpriattack->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::opbuffClick(TObject *Sender)
{
	iPriestAttackType = 2;

	dwPriestCastArray[2] = _CCast.MakeCastNum(_CCharBase,"611");
	dwPriestCastArray[3] = _CCast.MakeCastNum(_CCharBase,"620");
	dwPriestCastArray[4] = _CCast.MakeCastNum(_CCharBase,"642");
	dwPriestCastArray[5] = _CCast.MakeCastNum(_CCharBase,"651");
	dwPriestSTR30CastNum = _CCast.MakeCastNum(_CCharBase,"629");
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::ophealClick(TObject *Sender)
{
	iPriestAttackType = 1;

	dwPriestCastArray[2] = _CCast.MakeCastNum(_CCharBase,"511");
	dwPriestCastArray[3] = _CCast.MakeCastNum(_CCharBase,"520");
	dwPriestCastArray[4] = _CCast.MakeCastNum(_CCharBase,"542");
	dwPriestCastArray[5] = _CCast.MakeCastNum(_CCharBase,"551");
	dwPriestSTR30CastNum = _CCast.MakeCastNum(_CCharBase,"529");
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::opdebuffClick(TObject *Sender)
{
	iPriestAttackType = 3;

	dwPriestCastArray[2] = _CCast.MakeCastNum(_CCharBase,"711");
	dwPriestCastArray[3] = _CCast.MakeCastNum(_CCharBase,"720");
	dwPriestCastArray[4] = _CCast.MakeCastNum(_CCharBase,"742");
	dwPriestCastArray[5] = _CCast.MakeCastNum(_CCharBase,"751");
	dwPriestSTR30CastNum = _CCast.MakeCastNum(_CCharBase,"729");
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chlightstrikeClick(TObject *Sender)
{
	bLightStrike = chlightstrike->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chlightcounterClick(TObject *Sender)
{
	bLightCounter = chlightcounter->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chbrightnessClick(TObject *Sender)
{
	bBrightness = chbrightness->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chcriticallightClick(TObject *Sender)
{
	bCriticalLight = chcriticallight->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chlightattackClick(TObject *Sender)
{
	bLightAttack = chlightattack->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chstr30Click(TObject *Sender)
{
	bSTR30 = chstr30->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chasasClick(TObject *Sender)
{
	bAsasAttack = chasas->Checked;

	if(iAttackType == 1){
		if(bAsasAttack){
			txtatakdelay->Text = "1050";
			iAttackDelay = 1050;
		}
		else{
			txtatakdelay->Text = "1300";
			iAttackDelay = 1300;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chdebuffClick(TObject *Sender)
{
	bDebuff = chdebuff->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chsprint2Click(TObject *Sender)
{
	bSprintWarrior = chsprint2->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chdefenseClick(TObject *Sender)
{
	bDefenseWarrior = chdefense->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chgainClick(TObject *Sender)
{
	bGainWarrior = chgain->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::Button1Click(TObject *Sender)
{
	float b,c,d;
	DWORD a = _CTest.PickChar(15,15,b,c,d);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::lstasasClick(TObject *Sender)
{
	//int index = lstasas->ItemIndex;
	//bAsasCastArray[index] = lstasas->Checked[index];

	for (int i = 0; i < 9; i++) {
		bAsasCastArray[i] = lstasas->Checked[i];
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmasasbotTimer(TObject *Sender)
{
	if(bBotPaused)
		return;

	if(CheckLoading() == true)
		return;

	if(!_CCharBase.GetDWORD(KO_CRHP))
		return;

	if(_CCharBase.IsSit())
		return;

	if(!bAsasAttack && !bRAttack)
		return;

	if(bRunToMob && bRunMobPaused)
		return;

	DWORD	dwCurrentTick = GetTickCount();
	//if(dwCurrentTick - iLastAsasAttackTick < (DWORD)iAttackDelay)
	//	return;

	bool bRAvailable = (bool)(dwCurrentTick - iLastRAttackTick >= 1250);
	bool bCastAble = (bool)(dwCurrentTick - iLastAsasAttackTick >= (DWORD)iAttackDelay);
	bool bTurnSpike = (bool)(bCastAble && bAsasCastArray[8] && dwCurrentTick - iLastAsasCastTick[8] >= 13000);
	bool bTurnCut = (bool)(bCastAble && bAsasCastArray[7] && dwCurrentTick - iLastAsasCastTick[7] >= 7000);
	bool bTurnThrust = (bool)(bCastAble && bAsasCastArray[6] && dwCurrentTick - iLastAsasCastTick[6] >= 10000);
	bool bTurnShock = (bool)(bCastAble && bAsasCastArray[5] && dwCurrentTick - iLastAsasCastTick[5] >= 7000);
	bool bTurnPierce = (bool)(bCastAble && bAsasCastArray[4] && dwCurrentTick - iLastAsasCastTick[4] >= 10000);
	bool bTurnJab = (bool)(bCastAble && bAsasCastArray[3] && dwCurrentTick - iLastAsasCastTick[3] >= 6000);
	bool bTurnStab2 = (bool)(bCastAble && bAsasCastArray[2] && dwCurrentTick - iLastAsasCastTick[2] >= 6000);
	bool bTurnStab = (bool)(bCastAble && bAsasCastArray[1] && dwCurrentTick - iLastAsasCastTick[1] >= 6000);
	bool bTurnStroke = (bool)(bCastAble && bAsasCastArray[0]);

	bool bTurnPercent10 = (bool)(bCastAble && bAsasPercent10 && dwCurrentTick - iLastPercent10Tick >= 60000);
    bool bTurnPercent5 = (bool)(bCastAble && bAsasPercent5 && dwCurrentTick - iLastPercent5Tick >= 60000);

	CCharBase pTarget = NULL;
	CCharBase pTmp = NULL;
	float fDistance,fCenterDistance;
	DWORD TmpVal;
	int i;

	if( bAttackWithLeader )
	{
		_CParty.Refresh();
		if(_CParty.GetCount() < 2)
			return;
		if(wLastTarget && wLastTarget != 0xFFFF){
			pTmp = _CArea.GetAnyBase(wLastTarget);
			if(pTmp.GetBase()){
				if(pTmp.IsAlive()){
					fDistance = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pTmp.GetFLOAT(KO_POSX),pTmp.GetFLOAT(KO_POSY));
					if(fDistance <= iRunDist)
					{
						goto SELECT;
					}
				}
			}
		}
		return;
	}

	if(bWaitForKill){
		if(wLastTarget && wLastTarget != 0xFFFF){
			pTmp = _CArea.GetAnyBase(wLastTarget);
			if(pTmp.GetBase()){
				if(pTmp.IsAlive()){
					fDistance = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pTmp.GetFLOAT(KO_POSX),pTmp.GetFLOAT(KO_POSY));
					if(fDistance <= iRunDist)
					{
						if(bInterClient)
							SendTarget();
						goto SELECT;
					}
				}
			}
		}
	}

	switch(iTargetingType){

		case 0:	//auto
		{
			if(bOnlyEnemy)
				pTarget = _CArea.GetNearstUPC(true,true,0);
			else
				pTarget = _CArea.GetNearstEnemy(0,true,0);
			break;
		}

		case 1:	//manuel
		{
			wLastTarget = _CCharBase.GetTargetID();
			if(wLastTarget == 0xFFFF)
			{
				if(bInterClient)
					SendTarget();
				return;
			}

			pTarget = _CArea.GetAnyBase(wLastTarget);
			if(!pTarget.GetBase())
			{
				if(bInterClient)
				{
					wLastTarget = 0xFFFF;
					SendTarget();
				}
				return;
			}

			if(!pTarget.IsAlive())
			{
				if(bInterClient)
				{
					wLastTarget = 0xFFFF;
					SendTarget();
				}
				return;
			}

			if(bOnlyEnemy){
				if(!pTarget.IsUPC())
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}

				if(pTarget.GetDWORD(KO_NATN) == _CCharBase.GetDWORD(KO_NATN))
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}
			}

			if(bOnlyTarget){
				if(!pTarget.IsEnemy())
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}
			}

			break;
		}

		case 2:	//list
		{
			if(!iMobCount)
			{
				if(bInterClient)
				{
					wLastTarget = 0xFFFF;
					SendTarget();
				}
				return;
			}

			pTarget = _CArea.GetNPCByName(iMobCount,strMobList,NULL,true,true,true,0,false);
			break;
		}

		case 3: //sub
		{
			if(iSubTargetingType == 1)
			{
				if(!iTargetMobIDCount)
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}

				pTarget = _CArea.GetNearstNPCByID(iTargetMobID,iTargetMobIDCount,true,0);
			}
			else if(iSubTargetingType == 2)
			{
				if(!iTargetNickCount)
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}

				pTarget = _CArea.GetUPCByName(iTargetNickCount,strTargetNick,0,true,0,false,true);
			}
			break;
		}

		default:
		return;
		break;
	}

SELECT:
	if(pTarget.GetBase() == NULL)
		pTarget.SetBase(pTmp.GetBase());

	if(!pTarget.GetBase() || !pTarget.IsAlive()){
		if(bMoveToChest){
			bMoveChestPaused = false;
			bRunMobPaused = true;
		}
		else{
			if(bRunToMob && iBotCenterX && iBotCenterY){
				if(_CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) > 0.5 && _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) <= (float)iRunDist)
					SHTeleport((float)iBotCenterX,(float)iBotCenterY,5);
			}
		}

		if(bInterClient)
		{
			wLastTarget = 0xFFFF;
			SendTarget();
		}

		return;
	}

	wLastTarget = pTarget.GetID();

	if(bInterClient)
		SendTarget();

	if(bRAvailable || bCastAble)
	{
		if(bShowMob)
			_CArea.SelectObject(pTarget);
		else
			SendTargetHP(wLastTarget);
	}

	fDistance = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pTarget.GetFLOAT(KO_POSX),pTarget.GetFLOAT(KO_POSY));
	fCenterDistance = _CMath.fDist((float)iBotCenterX,(float)iBotCenterY,pTarget.GetFLOAT(KO_POSX),pTarget.GetFLOAT(KO_POSY));

	if(bRunToMob){
		if(fCenterDistance > (float)iRunDist){
			if(bMoveToChest){
				bMoveChestPaused = false;
				bRunMobPaused = true;
			}
			else{
				if(iBotCenterX && iBotCenterY){
					if(_CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) > 0.5 && _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) <= (float)iRunDist)
						SHTeleport((float)iBotCenterX,(float)iBotCenterY,5);
				}
			}
			return;
		}
	}

	if(fDistance > 2.0){
		if(bRock && fDistance < 10.0 && dwCurrentTick - iLastRockTick >= 3000 && wLastRockTarget != wLastTarget){
			if(_CBag.FindInvItem(0,ITEM_ROCK) > -1){
				CastRock(wLastTarget);
				iLastRockTick = dwCurrentTick;
				wLastRockTarget = wLastTarget;
			}
		}

		if(bRunToMob && iBotCenterX && iBotCenterY){
			bMoveChestPaused = true;
			if(fCenterDistance <= (float)iRunDist)
				SHTeleport(pTarget.GetFLOAT(KO_POSX),pTarget.GetFLOAT(KO_POSY),5);
			else{
				if(_CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) > 0.5 && _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) <= (float)iRunDist)
					SHTeleport((float)iBotCenterX,(float)iBotCenterY,5);
			}
		}
		return;
	}

CAST:

	if(pTarget.GetBase() == NULL)
		pTarget.SetBase(pTmp.GetBase());

	if(bRAttack && bRAvailable){
		CastRAttack(wLastTarget);
		iLastRAttackTick = dwCurrentTick;
	}

	if(!bAsasAttack)
		return;

	if(bTurnPercent10){
		*(DWORD*)(pBuf_Attack1 + 2) = dwPercent10Num;
		*(DWORD*)(pBuf_Attack3 + 2) = dwPercent10Num;
		*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack1 + 22) = 0x10;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack1,24);
		_CPacket.PSend(pBuf_Attack3,22);
		iLastAsasAttackTick = dwCurrentTick;
		iLastPercent10Tick = dwCurrentTick;
	}

	if(bTurnPercent5 && !bTurnPercent10){
		*(DWORD*)(pBuf_Attack1 + 2) = dwPercent5Num;
		*(DWORD*)(pBuf_Attack3 + 2) = dwPercent5Num;
		*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack1 + 22) = 0x10;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack1,24);
		_CPacket.PSend(pBuf_Attack3,22);
		iLastAsasAttackTick = dwCurrentTick;
		iLastPercent5Tick = dwCurrentTick;
	}

	if(bTurnSpike && !bTurnPercent5 && !bTurnPercent10){
		*(DWORD*)(pBuf_Attack3 + 2) = dwAsasCastArray[8];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		iLastAsasAttackTick = dwCurrentTick;
		iLastAsasCastTick[8] = dwCurrentTick;
	}

	if(bTurnCut && !bTurnSpike && !bTurnPercent5 && !bTurnPercent10){
		*(DWORD*)(pBuf_Attack3 + 2) = dwAsasCastArray[7];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		iLastAsasAttackTick = dwCurrentTick;
		iLastAsasCastTick[7] = dwCurrentTick;
	}

	if(bTurnThrust && !bTurnCut && !bTurnSpike && !bTurnPercent5 && !bTurnPercent10){
		*(DWORD*)(pBuf_Attack3 + 2) = dwAsasCastArray[6];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		iLastAsasAttackTick = dwCurrentTick;
		iLastAsasCastTick[6] = dwCurrentTick;
	}

	if(bTurnShock && !bTurnThrust && !bTurnCut && !bTurnSpike && !bTurnPercent5 && !bTurnPercent10){
		*(DWORD*)(pBuf_Attack3 + 2) = dwAsasCastArray[5];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		iLastAsasAttackTick = dwCurrentTick;
		iLastAsasCastTick[5] = dwCurrentTick;
	}

	if(bTurnPierce && !bTurnShock && !bTurnThrust && !bTurnCut && !bTurnSpike && !bTurnPercent5 && !bTurnPercent10){
		*(DWORD*)(pBuf_Attack3 + 2) = dwAsasCastArray[4];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		iLastAsasAttackTick = dwCurrentTick;
		iLastAsasCastTick[4] = dwCurrentTick;
	}

	if(bTurnJab && !bTurnPierce && !bTurnShock && !bTurnThrust && !bTurnCut && !bTurnSpike && !bTurnPercent5 && !bTurnPercent10){
		*(DWORD*)(pBuf_Attack3 + 2) = dwAsasCastArray[3];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		iLastAsasAttackTick = dwCurrentTick;
		iLastAsasCastTick[3] = dwCurrentTick;
	}

	if(bTurnStab2 && !bTurnJab && !bTurnPierce && !bTurnShock && !bTurnThrust && !bTurnCut && !bTurnSpike && !bTurnPercent5 && !bTurnPercent10){
		*(DWORD*)(pBuf_Attack3 + 2) = dwAsasCastArray[2];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		iLastAsasAttackTick = dwCurrentTick;
		iLastAsasCastTick[2] = dwCurrentTick;
	}

	if(bTurnStab && !bTurnStab2 && !bTurnJab && !bTurnPierce && !bTurnShock && !bTurnThrust && !bTurnCut && !bTurnSpike && !bTurnPercent5 && !bTurnPercent10){
		*(DWORD*)(pBuf_Attack3 + 2) = dwAsasCastArray[1];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		iLastAsasAttackTick = dwCurrentTick;
		iLastAsasCastTick[1] = dwCurrentTick;
	}

	if(bTurnStroke && !bTurnStab && !bTurnStab2 && !bTurnJab && !bTurnPierce && !bTurnShock && !bTurnThrust && !bTurnCut && !bTurnSpike && !bTurnPercent5 && !bTurnPercent10){
		*(DWORD*)(pBuf_Attack3 + 2) = dwAsasCastArray[0];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		iLastAsasAttackTick = dwCurrentTick;
		iLastAsasCastTick[0] = dwCurrentTick;
	}
}
//---------------------------------------------------------------------------

void CastRock(DWORD ID){
	*(DWORD*)(pBuf_Attack1 + 2) = SKILL_ROCK;
	*(WORD*)(pBuf_Attack1 + 8) = ID;

	*(DWORD*)(pBuf_Attack2 + 2) = SKILL_ROCK;
	*(WORD*)(pBuf_Attack2 + 8) = ID;

	*(DWORD*)(pBuf_Attack3 + 2) = SKILL_ROCK;
	*(WORD*)(pBuf_Attack3 + 8) = ID;

	*(BYTE*)(pBuf_Attack1 + 22) = 0x05;

	_CPacket.PSend(pBuf_Attack1,24);
	_CPacket.PSend(pBuf_Attack2,22);
	_CPacket.PSend(pBuf_Attack3,24);

	*(BYTE*)(pBuf_Attack1 + 22) = 0x00;
}

void CastRAttack(DWORD ID){
	*(WORD*)(pBuf_RAttack + 3) = ID;
	_CPacket.PSend(pBuf_RAttack,10);
}


void __fastcall TfrmBot::btnbotcenterClick(TObject *Sender)
{
	iBotCenterX = (int)_CCharBase.GetFLOAT(KO_POSX);
	iBotCenterY = (int)_CCharBase.GetFLOAT(KO_POSY);
	iCenterX = iBotCenterX;
	iCenterY = iBotCenterY;

	char _tmp[8];
	memset(_tmp,0,8);

	char tmp[10];
	memset(tmp,0,10);

	itoa(iBotCenterX,tmp,10);
	strcat(tmp,",");
	itoa(iBotCenterY,_tmp,10);
	strcat(tmp,_tmp);
	lbbotcenter->Caption = tmp;
	lbcenter->Caption = tmp;

}
//---------------------------------------------------------------------------
void __fastcall TfrmBot::lstwarriorClick(TObject *Sender)
{
	iWarriorCastIndex = lstwarrior->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmwarriorbotTimer(TObject *Sender)
{
	if(bBotPaused)
		return;

	if(CheckLoading() == true)
		return;

	if(!_CCharBase.GetDWORD(KO_CRHP))
		return;

	if(_CCharBase.IsSit())
		return;

	if(!bWarriorAttack && !bRAttack)
		return;

	if(bRunToMob && bRunMobPaused)
		return;

	DWORD	dwCurrentTick = GetTickCount();
	//if(dwCurrentTick - iLastWarriorAttackTick < (DWORD)iAttackDelay)
	//	return;

	bool bRAvailable = (bool)(dwCurrentTick - iLastRAttackTick >= 1350);
	bool bCastAble = (bool)(dwCurrentTick - iLastWarriorAttackTick >= (DWORD)iAttackDelay);
	bool bTurnBinding = (bool)(bBinding && dwCurrentTick - iLastBindingTick >= 6000);
	bool bTurnProvoke = (bool)(bProvoke && dwCurrentTick - iLastBindingTick >= 18000);//test

	CCharBase pTarget = NULL;
	CCharBase pTmp = NULL;
	float fDistance,fCenterDistance;
	DWORD TmpVal;
	int i;
	int iMobX,iMobY,iMobZ;

	if( bAttackWithLeader )
	{
		_CParty.Refresh();
		if(_CParty.GetCount() < 2)
			return;
		if(wLastTarget && wLastTarget != 0xFFFF){
			pTmp = _CArea.GetAnyBase(wLastTarget);
			if(pTmp.GetBase()){
				if(pTmp.IsAlive()){
					fDistance = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pTmp.GetFLOAT(KO_POSX),pTmp.GetFLOAT(KO_POSY));
					if(fDistance <= iRunDist)
					{
						goto SELECT;
					}
				}
			}
		}
		return;
	}

	if(bWaitForKill){
		if(wLastTarget && wLastTarget != 0xFFFF){
			pTmp = _CArea.GetAnyBase(wLastTarget);
			if(pTmp.GetBase()){
				if(pTmp.IsAlive()){
					fDistance = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pTmp.GetFLOAT(KO_POSX),pTmp.GetFLOAT(KO_POSY));
					if(fDistance <= iRunDist)
					{
						if(bInterClient)
							SendTarget();
						goto SELECT;
					}
				}
			}
		}
	}

	switch(iTargetingType){

		case 0:	//auto
		{
			if(bOnlyEnemy)
				pTarget = _CArea.GetNearstUPC(true,true,0);
			else
				pTarget = _CArea.GetNearstEnemy(0,true,0);
			break;
		}

		case 1:	//manuel
		{
			wLastTarget = _CCharBase.GetTargetID();
			if(wLastTarget == 0xFFFF)
			{
				if(bInterClient)
					SendTarget();
				return;
			}

			pTarget = _CArea.GetAnyBase(wLastTarget);
			if(!pTarget.GetBase())
			{
				if(bInterClient)
				{
					wLastTarget = 0xFFFF;
					SendTarget();
				}
				return;
			}

			if(!pTarget.IsAlive())
			{
				if(bInterClient)
				{
					wLastTarget = 0xFFFF;
					SendTarget();
				}
				return;
			}

			if(bOnlyEnemy){
				if(!pTarget.IsUPC())
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}

				if(pTarget.GetDWORD(KO_NATN) == _CCharBase.GetDWORD(KO_NATN))
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}
			}

			if(bOnlyTarget){
				if(!pTarget.IsEnemy())
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}
			}

			break;
		}

		case 2:	//list
		{
			if(!iMobCount)
			{
				if(bInterClient)
				{
					wLastTarget = 0xFFFF;
					SendTarget();
				}
				return;
			}

			pTarget = _CArea.GetNPCByName(iMobCount,strMobList,NULL,true,true,true,0,false);
			break;
		}

		case 3: //sub
		{
			if(iSubTargetingType == 1)
			{
				if(!iTargetMobIDCount)
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}

				pTarget = _CArea.GetNearstNPCByID(iTargetMobID,iTargetMobIDCount,true,0);
			}
			else if(iSubTargetingType == 2)
			{
				if(!iTargetNickCount)
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}

				pTarget = _CArea.GetUPCByName(iTargetNickCount,strTargetNick,0,true,0,false,true);
			}
			break;
		}

		default:
		return;
		break;
	}

SELECT:
	if(pTarget.GetBase() == NULL)
		pTarget.SetBase(pTmp.GetBase());

	if(!pTarget.GetBase() || !pTarget.IsAlive()){
		if(bMoveToChest){
			bMoveChestPaused = false;
			bRunMobPaused = true;
		}
		else{
			if(bRunToMob && iBotCenterX && iBotCenterY){
				if(_CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) > 0.5 && _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) <= (float)iRunDist)
					SHTeleport((float)iBotCenterX,(float)iBotCenterY,5);
			}
		}
		wLastBindingTarget = 0;
		wLastRockTarget = 0;
		if(bInterClient)
		{
			wLastTarget = 0xFFFF;
			SendTarget();
		}
		return;
	}

	wLastTarget = pTarget.GetID();

	if(bInterClient)
		SendTarget();

	if(bRAvailable || bCastAble)
	{
		if(bShowMob)
			_CArea.SelectObject(pTarget);
		else
			SendTargetHP(wLastTarget);
	}

	fDistance = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pTarget.GetFLOAT(KO_POSX),pTarget.GetFLOAT(KO_POSY));
	fCenterDistance = _CMath.fDist((float)iBotCenterX,(float)iBotCenterY,pTarget.GetFLOAT(KO_POSX),pTarget.GetFLOAT(KO_POSY));

	if(bRunToMob){
		if(fCenterDistance > (float)iRunDist){
			if(bMoveToChest){
				bMoveChestPaused = false;
				bRunMobPaused = true;
			}
			else{
				if(iBotCenterX && iBotCenterY){
					if(_CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) > 0.5 && _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) <= (float)iRunDist)
						SHTeleport((float)iBotCenterX,(float)iBotCenterY,5);
				}
			}
			wLastBindingTarget = 0;
			wLastRockTarget = 0;
			return;
		}
	}

	if(fDistance > 2.0){

		if(bWarriorAttack && bTurnProvoke && fDistance < 10.0){
			*(DWORD*)(pBuf_Attack1 + 2) = dwProvokeCastNum;
			*(DWORD*)(pBuf_Attack3 + 2) = dwProvokeCastNum;

			*(WORD*)(pBuf_Attack1 + 8) = 0xFFFF;
			*(WORD*)(pBuf_Attack3 + 8) = 0xFFFF;

			*(BYTE*)(pBuf_Attack1 + 22) = 0x0F;

			iMobX = (int)pTarget.GetFLOAT(KO_POSX);
			iMobY = (int)pTarget.GetFLOAT(KO_POSY);
			iMobZ = (int)pTarget.GetFLOAT(KO_POSZ);

			*(WORD*)(pBuf_Attack1 + 10) = iMobX;
			*(WORD*)(pBuf_Attack3 + 10) = iMobX;

			*(WORD*)(pBuf_Attack1 + 12) = iMobZ;
			*(WORD*)(pBuf_Attack3 + 12) = iMobZ;

			*(WORD*)(pBuf_Attack1 + 14) = iMobY;
			*(WORD*)(pBuf_Attack3 + 14) = iMobY;

			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack3,22);
			iLastProvokeTick = dwCurrentTick;

			*(WORD*)(pBuf_Attack1 + 10) = 0x0000;
			*(WORD*)(pBuf_Attack3 + 10) = 0x0000;

			*(WORD*)(pBuf_Attack1 + 12) = 0x0000;
			*(WORD*)(pBuf_Attack3 + 12) = 0x0000;

			*(WORD*)(pBuf_Attack1 + 14) = 0x0000;
			*(WORD*)(pBuf_Attack3 + 14) = 0x0000;
		}

		if(bWarriorAttack && bTurnBinding && !bTurnProvoke && fDistance < 10.0 && wLastBindingTarget != wLastTarget){
			*(DWORD*)(pBuf_Attack3 + 2) = dwBindingCastNum;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
			*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
			_CPacket.PSend(pBuf_Attack3,22);
			iLastBindingTick = dwCurrentTick;
			wLastBindingTarget = wLastTarget;
		}


		if(bRock && fDistance < 10.0 && dwCurrentTick - iLastRockTick >= 3000 && wLastRockTarget != wLastTarget){
			if(_CBag.FindInvItem(0,ITEM_ROCK) > -1){
				CastRock(wLastTarget);
				iLastRockTick = dwCurrentTick;
				wLastRockTarget = wLastTarget;
			}
		}

		if(bRunToMob && iBotCenterX && iBotCenterY){
			bMoveChestPaused = true;
			if(fCenterDistance <= (float)iRunDist)
				SHTeleport(pTarget.GetFLOAT(KO_POSX),pTarget.GetFLOAT(KO_POSY),5);
			else{
				if(_CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) > 0.5 && _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) <= (float)iRunDist)
					SHTeleport((float)iBotCenterX,(float)iBotCenterY,5);
				wLastBindingTarget = 0;
				wLastRockTarget = 0;
			}
		}
		return;	//break
	}

CAST:

	if(pTarget.GetBase() == NULL)
		pTarget.SetBase(pTmp.GetBase());

	if(bRAttack && bRAvailable){
		CastRAttack(wLastTarget);
		iLastRAttackTick = dwCurrentTick;
	}

	if(!bWarriorAttack || !bCastAble)
    	return;

	//*(DWORD*)(pBuf_Attack1 + 2) = dwWarriorCastArray[iWarriorCastIndex];
	*(DWORD*)(pBuf_Attack3 + 2) = dwWarriorCastArray[iWarriorCastIndex];
	//*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
	*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
	//*(BYTE*)(pBuf_Attack1 + 22) = 0x0D;
	*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
	//_CPacket.PSend(pBuf_Attack1,24);
	_CPacket.PSend(pBuf_Attack3,24);
	iLastWarriorAttackTick = dwCurrentTick;
}
//---------------------------------------------------------------------------


void __fastcall TfrmBot::chwarriorClick(TObject *Sender)
{
	bWarriorAttack = chwarrior->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::lstpriestClick(TObject *Sender)
{
	for (int i = 0; i < 8; i++) {
		bPriestCastArray[i] = lstpriest->Checked[i];
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnclosesundClick(TObject *Sender)
{
	pnsavesund->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnclosepotClick(TObject *Sender)
{
	pnpotsave->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btninncloseClick(TObject *Sender)
{
	pninnsave->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnsavesundClick(TObject *Sender)
{
	SaveSund();

	MessageBox(this->Handle,"Kaydedildi.","Kaydedildi.",MB_ICONINFORMATION);
}

void SaveSund(){
	if(!frmBot->lstsund->Count)
		return;

	ofstream fSund;

	DeleteFileA(frmBot->txtsundsave->Text);	//

	fSund.open(frmBot->txtsundsave->Text.c_str(),std::ios_base::out | std::ios_base::app);
	if(!fSund.is_open())return;

	fSund << frmBot->txtsund->Text.c_str() << "\n";

	for (int i = 0; i < frmBot->lstsund->Count; i++) {
		fSund << frmBot->lstsund->Items->operator [](i).c_str() << "\n";
	}

	fSund.close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnloadsundClick(TObject *Sender)
{
	LoadSund();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnloadpotClick(TObject *Sender)
{
	LoadPot();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btninnsaveClick(TObject *Sender)
{
	SaveInn();

	MessageBox(this->Handle,"Kaydedildi.","Kaydedildi.",MB_ICONINFORMATION);
}

void SaveInn(){
	if(!frmBot->lstinn->Count)
		return;

	ofstream fInn;

	DeleteFileA(frmBot->txtinnsave->Text);	//

	fInn.open(frmBot->txtinnsave->Text.c_str(),std::ios_base::out | std::ios_base::app);
	if(!fInn.is_open())return;

	fInn << frmBot->txtinn->Text.c_str() << "\n";

	for (int i = 0; i < frmBot->lstinn->Count; i++) {
		fInn << frmBot->lstinn->Items->operator [](i).c_str() << "\n";
	}

	fInn.close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btninnloadClick(TObject *Sender)
{
	LoadInn();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnpotsaveClick(TObject *Sender)
{
	SavePot();

	MessageBox(this->Handle,"Kaydedildi.","Kaydedildi.",MB_ICONINFORMATION);
}

void SavePot(){
	if(!frmBot->lstpot->Count)
		return;

	ofstream fPot;

	DeleteFileA(frmBot->txtpotsave->Text);

	fPot.open(frmBot->txtpotsave->Text.c_str(),std::ios_base::out | std::ios_base::app);
	if(!fPot.is_open())return;

	fPot << frmBot->txtpot->Text.c_str() << "\n";

	for (int i = 0; i < frmBot->lstpot->Count; i++) {
		fPot << frmBot->lstpot->Items->operator [](i).c_str() << "\n";
	}

	fPot.close();
}
//---------------------------------------------------------------------------


void __fastcall TfrmBot::chstealthhackClick(TObject *Sender)
{
	DWORD dwNum = _CCast.MakeCastNum(_CCharBase,"645");
	DWORD dwBase = _CTest.GetSkillBase(dwNum);
	if(dwBase){
		if(chstealthhack->Checked)
			*(DWORD*)(dwBase + 0x34) = 39;	//dagger animation
		else
			*(DWORD*)(dwBase + 0x34) = 0x13;	//normal
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmpribotTimer(TObject *Sender)
{
	if(bBotPaused)
		return;

	if(CheckLoading() == true)
		return;

	if(!_CCharBase.GetDWORD(KO_CRHP))
		return;

/*	if(_CCharBase.IsSit() == true){
		if(bSitWhenIdle)
			_CTest.CommandSit(false);
		else
			return;
	}*/

	if(!bPriestAttack && !bRAttack && !bDebuff)
		return;

	if(bRunToMob && bRunMobPaused)
		return;

	DWORD	dwCurrentTick = GetTickCount();
	//if(dwCurrentTick - iLastWarriorAttackTick < (DWORD)iAttackDelay)
	//	return;

	bool bRAvailable = (bool)(dwCurrentTick - iLastRAttackTick >= 1350);
	bool bCastAble = (bool)(dwCurrentTick - iLastPriestAttackTick >= (DWORD)iAttackDelay);

	bool bTurnStroke = (bool)(bCastAble && bPriestCastArray[0]);
	bool bTurnHoly = (bool)(bCastAble && bPriestCastArray[1]);
	bool bTurnCollision = (bool)(bCastAble && bPriestCastArray[2]);
	bool bTurnShuddering = (bool)(bCastAble && bPriestCastArray[3]);
	bool bTurnRuin = (bool)(bCastAble && bPriestCastArray[4] && dwCurrentTick - iLastPriestCastTick[4] >= 2000);
	bool bTurnHellis = (bool)(bCastAble && bPriestCastArray[5] && dwCurrentTick - iLastPriestCastTick[5] >= 3000);
	bool bTurnJudgment = (bool)(bCastAble && bPriestCastArray[6]);
	bool bTurnHelis = (bool)(bCastAble && bPriestCastArray[7]);

	bool bTurnCritical = (bool)(bCastAble && bCriticalLight && dwCurrentTick - iLastPriestFarCastTick[4] >= 6000);
	bool bTurnCounter = (bool)(bCastAble && bLightCounter && dwCurrentTick - iLastPriestFarCastTick[3] >= 6000);
	bool bTurnLightAttack = (bool)(bCastAble && bLightAttack && dwCurrentTick - iLastPriestFarCastTick[2] >= 6000);
	bool bTurnBrightness = (bool)(bCastAble && bBrightness && dwCurrentTick - iLastPriestFarCastTick[1] >= 6000);
	bool bTurnLightStrike = (bool)(bCastAble && bLightStrike && dwCurrentTick - iLastPriestFarCastTick[0] >= 6000);

	bool bDebuffAble = (bool)(dwCurrentTick - iLastPriestDebuffCastTick >= 1350);
	bool bTurnMalice = (bool)(bDebuffAble && bMalice && dwCurrentTick - iLastPriestDebuffTick[0] >= 8000);
	bool bTurnParasite = (bool)(bDebuffAble && bParasite && dwCurrentTick - iLastPriestDebuffTick[1] >= 8000);
	bool bTurnTorment = (bool)(bDebuffAble && bTorment && dwCurrentTick - iLastPriestDebuffTick[2] >= 10000);
	bool bTurnMassive = (bool)(bDebuffAble && bMassive && dwCurrentTick - iLastPriestDebuffTick[3] >= 10000);

	CCharBase pTarget = NULL;
	CCharBase pTmp = NULL;
	float fDistance,fCenterDistance;
	DWORD TmpVal;
	int i;
	int iMobX,iMobY,iMobZ;

	if( bAttackWithLeader )
	{
		_CParty.Refresh();
		if(_CParty.GetCount() < 2)
			return;
		if(wLastTarget && wLastTarget != 0xFFFF){
			pTmp = _CArea.GetAnyBase(wLastTarget);
			if(pTmp.GetBase()){
				if(pTmp.IsAlive()){
					fDistance = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pTmp.GetFLOAT(KO_POSX),pTmp.GetFLOAT(KO_POSY));
					if(fDistance <= iRunDist)
					{
						goto SELECT;
					}
				}
			}
		}
		return;
	}

	if(bWaitForKill){
		if(wLastTarget && wLastTarget != 0xFFFF){
			pTmp = _CArea.GetAnyBase(wLastTarget);
			if(pTmp.GetBase()){
				if(pTmp.IsAlive()){
					fDistance = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pTmp.GetFLOAT(KO_POSX),pTmp.GetFLOAT(KO_POSY));
					if(fDistance <= iRunDist)
					{
						if(bInterClient)
							SendTarget();
						goto SELECT;
					}
				}
			}
		}
	}

	switch(iTargetingType){

		case 0:	//auto
		{
			if(bOnlyEnemy)
				pTarget = _CArea.GetNearstUPC(true,true,0);
			else
				pTarget = _CArea.GetNearstEnemy(0,true,0);
			break;
		}

		case 1:	//manuel
		{
			wLastTarget = _CCharBase.GetTargetID();
			if(wLastTarget == 0xFFFF)
			{
				if(bInterClient)
					SendTarget();
				return;
			}

			pTarget = _CArea.GetAnyBase(wLastTarget);
			if(!pTarget.GetBase())
			{
				if(bInterClient)
				{
					wLastTarget = 0xFFFF;
					SendTarget();
				}
				return;
			}

			if(!pTarget.IsAlive())
			{
				if(bInterClient)
				{
					wLastTarget = 0xFFFF;
					SendTarget();
				}
				return;
			}

			if(bOnlyEnemy){
				if(!pTarget.IsUPC())
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}

				if(pTarget.GetDWORD(KO_NATN) == _CCharBase.GetDWORD(KO_NATN))
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}
			}

			if(bOnlyTarget){
				if(!pTarget.IsEnemy())
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}
			}

			break;
		}

		case 2:	//list
		{
			if(!iMobCount)
			{
				if(bInterClient)
				{
					wLastTarget = 0xFFFF;
					SendTarget();
				}
				return;
			}

			pTarget = _CArea.GetNPCByName(iMobCount,strMobList,NULL,true,true,true,0,false);
			break;
		}

		case 3: //sub
		{
			if(iSubTargetingType == 1)
			{
				if(!iTargetMobIDCount)
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}

				pTarget = _CArea.GetNearstNPCByID(iTargetMobID,iTargetMobIDCount,true,0);
			}
			else if(iSubTargetingType == 2)
			{
				if(!iTargetNickCount)
				{
					if(bInterClient)
					{
						wLastTarget = 0xFFFF;
						SendTarget();
					}
					return;
				}

				pTarget = _CArea.GetUPCByName(iTargetNickCount,strTargetNick,0,true,0,false,true);
			}
			break;
		}

		default:
		return;
		break;
	}

SELECT:
	if(pTarget.GetBase() == NULL)
		pTarget.SetBase(pTmp.GetBase());

	if(!pTarget.GetBase() || !pTarget.IsAlive()){
		if(bMoveToChest){
			bMoveChestPaused = false;
			bRunMobPaused = true;
		}
		else{
			if(bRunToMob && iBotCenterX && iBotCenterY){
				if(_CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) > 0.5 && _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) <= (float)iRunDist)
					SHTeleport((float)iBotCenterX,(float)iBotCenterY,5);
				else{
					if(bSitWhenIdle){
						if(_CCharBase.IsSit() == false)
							_CTest.CommandSit(true);
					}
				}
			}
		}
		if(bInterClient)
		{
			wLastTarget = 0xFFFF;
			SendTarget();
		}
		return;
	}

	wLastTarget = pTarget.GetID();

	if(bInterClient)
		SendTarget();

	if(bRAvailable || bCastAble || bDebuffAble)
	{
		if(bShowMob)
			_CArea.SelectObject(pTarget);
		else
			SendTargetHP(wLastTarget);
	}

	fDistance = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pTarget.GetFLOAT(KO_POSX),pTarget.GetFLOAT(KO_POSY));
	fCenterDistance = _CMath.fDist((float)iBotCenterX,(float)iBotCenterY,pTarget.GetFLOAT(KO_POSX),pTarget.GetFLOAT(KO_POSY));

	if(bRunToMob){
		if(fCenterDistance > (float)iRunDist){
			if(bMoveToChest){
				bMoveChestPaused = false;
				bRunMobPaused = true;
			}
			else{
				if(iBotCenterX && iBotCenterY){
					if(_CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) > 0.5 && _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) <= (float)iRunDist)
						SHTeleport((float)iBotCenterX,(float)iBotCenterY,5);
					else{
						if(bSitWhenIdle){
							if(_CCharBase.IsSit() == false)
								_CTest.CommandSit(true);
						}
					}
				}
			}
			return;
		}
	}

	if(fDistance > 2.0){

		if(bTurnCritical && fDistance < 5.0){

			if(_CCharBase.IsSit()){
				if(bSitWhenIdle)
					_CTest.CommandSit(false);
			else
				return;
			}

			*(DWORD*)(pBuf_Attack1 + 2) = dwPriestFarCastArray[4];
			*(DWORD*)(pBuf_Attack3 + 2) = dwPriestFarCastArray[4];
			*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
			*(BYTE*)(pBuf_Attack1 + 22) = 0x0F;
			*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack3,22);
			iLastPriestAttackTick = dwCurrentTick;
			iLastPriestFarCastTick[4] = dwCurrentTick;

		}

		if(bTurnCounter && !bTurnCritical && fDistance < 5.0){

			if(_CCharBase.IsSit()){
				if(bSitWhenIdle)
					_CTest.CommandSit(false);
			else
				return;
			}

			*(DWORD*)(pBuf_Attack1 + 2) = dwPriestFarCastArray[3];
			*(DWORD*)(pBuf_Attack3 + 2) = dwPriestFarCastArray[3];
			*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
			*(BYTE*)(pBuf_Attack1 + 22) = 0x0F;
			*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack3,22);
			iLastPriestAttackTick = dwCurrentTick;
			iLastPriestFarCastTick[3] = dwCurrentTick;
		}

		if(bTurnLightAttack && !bTurnCounter && !bTurnCritical && fDistance < 5.0){

			if(_CCharBase.IsSit()){
				if(bSitWhenIdle)
					_CTest.CommandSit(false);
			else
				return;
			}

			*(DWORD*)(pBuf_Attack1 + 2) = dwPriestFarCastArray[2];
			*(DWORD*)(pBuf_Attack3 + 2) = dwPriestFarCastArray[2];
			*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
			*(BYTE*)(pBuf_Attack1 + 22) = 0x0F;
			*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack3,22);
			iLastPriestAttackTick = dwCurrentTick;
			iLastPriestFarCastTick[2] = dwCurrentTick;
		}

		if(bTurnBrightness && !bTurnLightAttack && !bTurnCounter && !bTurnCritical && fDistance < 5.0){

			if(_CCharBase.IsSit()){
				if(bSitWhenIdle)
					_CTest.CommandSit(false);
			else
				return;
			}

			*(DWORD*)(pBuf_Attack1 + 2) = dwPriestFarCastArray[1];
			*(DWORD*)(pBuf_Attack3 + 2) = dwPriestFarCastArray[1];
			*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
			*(BYTE*)(pBuf_Attack1 + 22) = 0x0F;
			*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack3,22);
			iLastPriestAttackTick = dwCurrentTick;
			iLastPriestFarCastTick[1] = dwCurrentTick;
		}

		if(bTurnLightStrike && !bTurnBrightness && !bTurnLightAttack && !bTurnCounter && !bTurnCritical && fDistance < 5.0){

			if(_CCharBase.IsSit()){
				if(bSitWhenIdle)
					_CTest.CommandSit(false);
			else
				return;
			}

			*(DWORD*)(pBuf_Attack1 + 2) = dwPriestFarCastArray[0];
			*(DWORD*)(pBuf_Attack3 + 2) = dwPriestFarCastArray[0];
			*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;
			*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
			*(BYTE*)(pBuf_Attack1 + 22) = 0x0F;
			*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
			_CPacket.PSend(pBuf_Attack1,24);
			_CPacket.PSend(pBuf_Attack3,22);
			iLastPriestAttackTick = dwCurrentTick;
			iLastPriestFarCastTick[0] = dwCurrentTick;
		}

		if(bRock && fDistance < 10.0 && dwCurrentTick - iLastRockTick >= 3000 && wLastRockTarget != wLastTarget){
			if(_CBag.FindInvItem(0,ITEM_ROCK) > -1){

				if(_CCharBase.IsSit()){
					if(bSitWhenIdle)
						_CTest.CommandSit(false);
				else
					return;
				}

				CastRock(wLastTarget);
				iLastRockTick = dwCurrentTick;
				wLastRockTarget = wLastTarget;
			}
		}

		if(bRunToMob && iBotCenterX && iBotCenterY){
			bMoveChestPaused = true;
			if(fCenterDistance <= (float)iRunDist)
				SHTeleport(pTarget.GetFLOAT(KO_POSX),pTarget.GetFLOAT(KO_POSY),5);
			else{
				if(_CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) > 0.5 && _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),(float)iBotCenterX,(float)iBotCenterY) <= (float)iRunDist)
					SHTeleport((float)iBotCenterX,(float)iBotCenterY,5);
				else{
                    if(bSitWhenIdle){
						if(_CCharBase.IsSit() == false)
							_CTest.CommandSit(true);
					}
                }
			}
		}
		return;	//break
	}

CAST:

	if(pTarget.GetBase() == NULL)
		pTarget.SetBase(pTmp.GetBase());

	if(bTurnMalice){

		if(_CCharBase.IsSit()){
			if(bSitWhenIdle)
				_CTest.CommandSit(false);
			else
				return;
		}

		*(DWORD*)(pBuf_Attack1 + 2) = dwPriestDebuffArray[0];
		*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;

		*(DWORD*)(pBuf_Attack3 + 2) = dwPriestDebuffArray[0];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

		*(BYTE*)(pBuf_Attack1 + 22) = 0x0F;

		_CPacket.PSend(pBuf_Attack1,24);
		_CPacket.PSend(pBuf_Attack3,22);

		*(BYTE*)(pBuf_Attack1 + 22) = 0x00;
		iLastPriestDebuffTick[0] = dwCurrentTick;
		iLastPriestDebuffCastTick = dwCurrentTick;

		if(bPriPrintName)
			PrintDebuff(pTarget.GetName());
	}

	if(bTurnParasite && !bTurnMalice){

		if(_CCharBase.IsSit()){
			if(bSitWhenIdle)
				_CTest.CommandSit(false);
			else
				return;
		}

		*(DWORD*)(pBuf_Attack1 + 2) = dwPriestDebuffArray[1];
		*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;

		*(DWORD*)(pBuf_Attack3 + 2) = dwPriestDebuffArray[1];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

		*(BYTE*)(pBuf_Attack1 + 22) = 0x0F;

		_CPacket.PSend(pBuf_Attack1,24);
		_CPacket.PSend(pBuf_Attack3,22);

		*(BYTE*)(pBuf_Attack1 + 22) = 0x00;
		iLastPriestDebuffTick[1] = dwCurrentTick;
		iLastPriestDebuffCastTick = dwCurrentTick;

		if(bPriPrintName)
			PrintDebuff(pTarget.GetName());
	}

	if(bTurnTorment && !bTurnParasite && !bTurnMalice){

		if(_CCharBase.IsSit()){
			if(bSitWhenIdle)
				_CTest.CommandSit(false);
			else
				return;
		}

		*(DWORD*)(pBuf_Attack1 + 2) = dwPriestDebuffArray[2];
		*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;

		*(DWORD*)(pBuf_Attack3 + 2) = dwPriestDebuffArray[2];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

		*(WORD*)(pBuf_Attack1 + 8) = 0xFFFF;
		*(WORD*)(pBuf_Attack3 + 8) = 0xFFFF;

		*(BYTE*)(pBuf_Attack1 + 22) = 0x0F;

		iMobX = (int)pTarget.GetFLOAT(KO_POSX);
		iMobY = (int)pTarget.GetFLOAT(KO_POSY);
		iMobZ = (int)pTarget.GetFLOAT(KO_POSZ);

		*(WORD*)(pBuf_Attack1 + 10) = iMobX;
		*(WORD*)(pBuf_Attack3 + 10) = iMobX;

		*(WORD*)(pBuf_Attack1 + 12) = iMobZ;
		*(WORD*)(pBuf_Attack3 + 12) = iMobZ;

		*(WORD*)(pBuf_Attack1 + 14) = iMobY;
		*(WORD*)(pBuf_Attack3 + 14) = iMobY;

		_CPacket.PSend(pBuf_Attack1,24);
		_CPacket.PSend(pBuf_Attack3,22);
		iLastPriestDebuffTick[2] = dwCurrentTick;
		iLastPriestDebuffCastTick = dwCurrentTick;

		*(WORD*)(pBuf_Attack1 + 10) = 0x0000;
		*(WORD*)(pBuf_Attack3 + 10) = 0x0000;

		*(WORD*)(pBuf_Attack1 + 12) = 0x0000;
		*(WORD*)(pBuf_Attack3 + 12) = 0x0000;

		*(WORD*)(pBuf_Attack1 + 14) = 0x0000;
		*(WORD*)(pBuf_Attack3 + 14) = 0x0000;

		if(bPriPrintName)
			PrintDebuff(pTarget.GetName());
	}

	if(bTurnMassive && !bTurnTorment && !bTurnParasite && !bTurnMalice){

		if(_CCharBase.IsSit()){
			if(bSitWhenIdle)
				_CTest.CommandSit(false);
			else
				return;
		}

		*(DWORD*)(pBuf_Attack1 + 2) = dwPriestDebuffArray[3];
		*(WORD*)(pBuf_Attack1 + 8) = wLastTarget;

		*(DWORD*)(pBuf_Attack3 + 2) = dwPriestDebuffArray[3];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;

		*(BYTE*)(pBuf_Attack1 + 22) = 0x0F;

		_CPacket.PSend(pBuf_Attack1,24);
		_CPacket.PSend(pBuf_Attack3,22);

		*(BYTE*)(pBuf_Attack1 + 22) = 0x00;
		iLastPriestDebuffTick[3] = dwCurrentTick;
		iLastPriestDebuffCastTick = dwCurrentTick;

		if(bPriPrintName)
			PrintDebuff(pTarget.GetName());
	}

	if(bRAttack && bRAvailable){

		if(_CCharBase.IsSit()){
			if(bSitWhenIdle)
				_CTest.CommandSit(false);
			else
				return;
		}

		CastRAttack(wLastTarget);
		iLastRAttackTick = dwCurrentTick;
	}

	//refresh
	bCastAble = (bool)(dwCurrentTick - iLastPriestAttackTick >= (DWORD)iAttackDelay);

	if(!bPriestAttack || !bCastAble){
		if(bSitWhenIdle)
			_CTest.CommandSit(true);
		return;
	}

	if(_CCharBase.IsSit()){
		if(bSitWhenIdle)
			_CTest.CommandSit(false);
		else
			return;
	}

	if(bTurnHelis){
		*(DWORD*)(pBuf_Attack3 + 2) = dwPriestCastArray[7];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		iLastPriestAttackTick = dwCurrentTick;
	}

	if(bTurnJudgment && !bTurnHelis){
		*(DWORD*)(pBuf_Attack3 + 2) = dwPriestCastArray[6];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		iLastPriestAttackTick = dwCurrentTick;
	}

	if(bTurnHellis && !bTurnJudgment && !bTurnHelis){
		*(DWORD*)(pBuf_Attack3 + 2) = dwPriestCastArray[5];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		iLastPriestAttackTick = dwCurrentTick;
		iLastPriestCastTick[5] = dwCurrentTick;
	}

	if(bTurnRuin && !bTurnHellis && !bTurnJudgment && !bTurnHelis){
		*(DWORD*)(pBuf_Attack3 + 2) = dwPriestCastArray[4];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		iLastPriestAttackTick = dwCurrentTick;
		iLastPriestCastTick[4] = dwCurrentTick;
	}

	if(bTurnShuddering && !bTurnRuin && !bTurnHellis && !bTurnJudgment && !bTurnHelis){
		*(DWORD*)(pBuf_Attack3 + 2) = dwPriestCastArray[3];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		iLastPriestAttackTick = dwCurrentTick;
	}

	if(bTurnCollision && !bTurnShuddering && !bTurnRuin && !bTurnHellis && !bTurnJudgment && !bTurnHelis){
		*(DWORD*)(pBuf_Attack3 + 2) = dwPriestCastArray[2];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		iLastPriestAttackTick = dwCurrentTick;
	}

	if(bTurnHoly && !bTurnCollision && !bTurnShuddering && !bTurnRuin && !bTurnHellis && !bTurnJudgment && !bTurnHelis){
		*(DWORD*)(pBuf_Attack3 + 2) = dwPriestCastArray[1];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		iLastPriestAttackTick = dwCurrentTick;
	}

	if(bTurnStroke && !bTurnHoly && !bTurnCollision && !bTurnShuddering && !bTurnRuin && !bTurnHellis && !bTurnJudgment && !bTurnHelis){
		*(DWORD*)(pBuf_Attack3 + 2) = dwPriestCastArray[0];
		*(WORD*)(pBuf_Attack3 + 8) = wLastTarget;
		*(BYTE*)(pBuf_Attack3 + 16) = 0x00;
		_CPacket.PSend(pBuf_Attack3,24);
		iLastPriestAttackTick = dwCurrentTick;
	}

	if(_CCharBase.IsSit() == false && bRunToMob == false){
		if(bSitWhenIdle)
			_CTest.CommandSit(true);
	}
}
//---------------------------------------------------------------------------

void PrintDebuff(char* strName){
	if(strName == NULL)
		return;

	int iNameLen = strlen(strName);

	char *pSend = new char[iNameLen + 4];
	memset(pSend,0,iNameLen+4);

	_CPacket.PutBYTE(pSend,0x10);
	_CPacket.PutBYTE(pSend+1,0x03);
	_CPacket.PutWORD(pSend+2,iNameLen);
	_CPacket.PutSTR(pSend+4,strName,iNameLen);
	_CPacket.PSend(pSend,iNameLen + 4);
	delete [] pSend;
}

void __fastcall TfrmBot::charcheryClick(TObject *Sender)
{
	bArcheryAttack = charchery->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chautoreviveClick(TObject *Sender)
{
	bAutoRevive = chautorevive->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnsellClick(TObject *Sender)
{
	pnsell->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnclosesellClick(TObject *Sender)
{
	pnsell->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chsellClick(TObject *Sender)
{
	bAutoSell = chsell->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chsellslotClick(TObject *Sender)
{
	bSellSlotLimit = chsellslot->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtsellslotChange(TObject *Sender)
{
	iSellBeginSlot = atoi(txtsellslot->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chsellabovelimitClick(TObject *Sender)
{
	bSellPriceLimitAbove = chsellabovelimit->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chselllesslimitClick(TObject *Sender)
{
	bSellPriceLimitLess = chselllesslimit->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtsellabovelimitChange(TObject *Sender)
{
	iSellPriceLimitAbove = atoi(txtsellabovelimit->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtselllesslimitChange(TObject *Sender)
{
	iSellPriceLimitLess = atoi(txtselllesslimit->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::opsellwaitClick(TObject *Sender)
{
	bSellWaitForRPR = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::opsellgoClick(TObject *Sender)
{
	bSellWaitForRPR = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::oprouteshClick(TObject *Sender)
{
	bRouteSH = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::oproutenormalClick(TObject *Sender)
{
	bRouteSH = false;
	chwh->Checked = true;
	_CCharBase.SetDWORD(KO_AUTH,!chwh->Checked);
}
//---------------------------------------------------------------------------

bool MoveToPos(float fX, float fY){
	float fMyPosX = _CCharBase.GetFLOAT(KO_POSX);
	float fMyPosY = _CCharBase.GetFLOAT(KO_POSY);

	if(_CMath.fDist(fMyPosX,fMyPosY,fX,fY) > 0.1){
		_CCharBase.MoveTo(fX,fY);
		return false;
	}
	else
    	return true;
}

void __fastcall TfrmBot::chstuncharClick(TObject *Sender)
{
	bStunChar = chstunchar->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtminorlimitChange(TObject *Sender)
{
	iMinorLimit = atoi(txtminorlimit->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnbuycloseClick(TObject *Sender)
{
	pnbuy->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnbuyClick(TObject *Sender)
{
	pnbuy->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chbuytsClick(TObject *Sender)
{
	bBuyTS = chbuyts->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chbuybookClick(TObject *Sender)
{
	bBuyBook = chbuybook->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtbuytsChange(TObject *Sender)
{
	iBuyTS = atoi(txtbuyts->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtbuybookChange(TObject *Sender)
{
	iBuyBook = atoi(txtbuybook->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chbuyrockClick(TObject *Sender)
{
	bBuyRock = chbuyrock->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtbuyrockChange(TObject *Sender)
{
	iBuyRock = atoi(txtbuyrock->Text.c_str());
}
//---------------------------------------------------------------------------

void StopBot(){
	frmBot->tmattack->Enabled = false;
	frmBot->tmmageattack->Enabled = false;
	frmBot->shpstatus->Brush->Color = clRed;
	frmBot->tmmovechest->Enabled = false;
	frmBot->tmsupply->Enabled		= false;
	frmBot->tmasasbot->Enabled = false;
	frmBot->tmpribot->Enabled = false;
	frmBot->tmpriest->Enabled = false;
	frmBot->tmwarriorbot->Enabled = false;
	frmBot->tmbuffing->Enabled 		= false;
	frmBot->tmtown->Enabled			= false;
	frmBot->tmmove->Enabled = false;
	frmBot->bBotState = false;
	//_CRecvFunction.Pause();

	//frmBot->tmchat->Enabled = false;
	//frmBot->tmfollow->Enabled = false;
	frmBot->tmtargetsize->Enabled = false;
	frmBot->tmtp->Enabled = false;

	//terminate moving
	frmBot->tmsundgo->Enabled = false;
	frmBot->tminngo->Enabled = false;
	frmBot->tmpotgo->Enabled = false;

	SundReturned();
	InnReturned();
	PotReturned();

	frmBot->bMoveChestPaused = false;
	frmBot->bRunMobPaused = false;

	frmBot->tmgocenter->Enabled = false;

	dlgMaden->tmmaden->Enabled = false;
	dlgMaden->tmrpr->Enabled = false;
	dlgMaden->tmsundgo->Enabled = false;

	ClearDrops(); 	//test
}

void __fastcall TfrmBot::lstarcheryClick(TObject *Sender)
{
	for (int i = 0; i < 20; i++) {
		bArcheryCastArray[i] = lstarchery->Checked[i];
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chblowClick(TObject *Sender)
{
	bBlow = chblow->Checked;
}
//---------------------------------------------------------------------------

void SaveSettings(bool bMsgBox){

	SaveSund();
	SavePot();
	SaveInn();
	SaveMineSettings();

	if(frmBot->txtloadsavefile->Text.Length() == 0)
		return;

	DeleteFileA(frmBot->txtloadsavefile->Text);	//clear

	ofstream fSave;

	fSave.open(frmBot->txtloadsavefile->Text.c_str(),std::ios_base::out | std::ios_base::app);
	if(!fSave.is_open()){
		if(bMsgBox)
			MessageBox(frmBot->Handle,"Kayit basarisiz! (Dosya olusturulamadi)","hata!",MB_ICONEXCLAMATION);
		return;
	}

	char tmp[16];	memset(tmp,0,16);

	itoa(frmBot->iSHBoost,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iSHDelay,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bRouteSH,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bArcheryAttack,tmp,10);
	fSave << tmp << "\n";

	for (int i = 0; i < 20; i++) {
		itoa(frmBot->bArcheryCastArray[i],tmp,10);
		fSave << tmp << "\n";
	}

	itoa(frmBot->bBlow,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bBonus3_1,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iTargetingType,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iMobCount,tmp,10);
	fSave << tmp << "\n";

	if(frmBot->iMobCount){
		for (int i = 0; i < frmBot->iMobCount; i++) {
			fSave << frmBot->lstatakmobs->Items->operator [](i).c_str() << "\n";
		}
	}

	itoa(frmBot->iAttackDelay,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bShowEffect,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bShowMob,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bWaitForKill,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bOnlyEnemy,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bOnlyTarget,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bBonus3,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bBonus5,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iBonusDistance,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->chstealthhack->Checked,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bAutoRevive,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bMinor,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iMinorLimit,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bWolf,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bTS,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bMana,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iManaType,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iManaLimit,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bHP,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iHPType,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iHPLimit,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bSprint,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bHide,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bScaled,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bSW,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bEvade,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bLup,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bAntiFreeze,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bLF,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bSafety,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bCats,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bMage62,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bCure,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bMoving,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iMoveCount,tmp,10);
	fSave << tmp << "\n";

	if(frmBot->iMoveCount){
		for (int i = 0; i < frmBot->iMoveCount; i++) {
			fSave << frmBot->lstmove->Items->operator [](i).c_str() << "\n";
		}
	}

	if(frmBot->txtchat->Text.Length() > 0)
		fSave << frmBot->txtchat->Text.c_str() << "\n";
	else
		fSave << "-" << "\n";

	itoa(frmBot->iChatDelay,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iChatType,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->chchat->Checked,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bLoot,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bLootCoins,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bMoveToChest,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bStunChar,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iMaxChestDistance,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iCenterX,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iCenterY,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bLootUnique,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bLootUpgrade,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iLootUpgradeLimit,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bLootList,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bLootPriceLimit,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iLootPriceLimit,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iLootCount,tmp,10);
	fSave << tmp << "\n";

	if(frmBot->iLootCount){
		for (int i = 0; i < frmBot->iLootCount; i++) {
			fSave << frmBot->lstlootids->Items->operator [](i).c_str() << "\n";
		}
	}

	itoa(frmBot->bSund,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bPot,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bInn,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bBuyMana,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bBuyHP,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iBuyManaLimit,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iBuyHPLimit,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bInnDepot,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iInnBeginSlot,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bAutoSell,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bSellSlotLimit,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iSellBeginSlot,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bSellPriceLimitAbove,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bSellPriceLimitLess,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iSellPriceLimitAbove,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iSellPriceLimitLess,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bSellWaitForRPR,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bBuyWolf,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bBuyArrow,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bBuyTS,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bBuyBook,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bBuyRock,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iBuyTS,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iBuyBook,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iBuyRock,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->chwh->Checked,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bFollowLeader,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bFollowSelected,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bAutoParty,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bInformAfterSupply,tmp,10);
	fSave << tmp << "\n";

	if(frmBot->txtinformsupply->Text.Length() == 0)
		fSave << "-" << "\n";
	else
		fSave << frmBot->txtinformsupply->Text.c_str() << "\n";

	itoa(frmBot->bAutoInvite,tmp,10);
	fSave << tmp << "\n";

	if(dlgParty->txtautoadd->Text.Length() == 0)
		fSave << "-" << "\n";
	else
		fSave << dlgParty->txtautoadd->Text.c_str() << "\n";

	itoa(frmBot->bTownOnGM,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bTownOnMob,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bTownOnNoTS,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bTownOnNoArrow,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bTownOnNoMana,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bTownOnNoParty,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bRingOnDie,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bRingOnTown,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iAlertMobCount,tmp,10);
	fSave << tmp << "\n";

	if(frmBot->iAlertMobCount){
		for (int i = 0; i < frmBot->iAlertMobCount; i++) {
			fSave << frmBot->lsttownmobs->Items->operator [](i).c_str() << "\n";
		}
	}

	itoa(frmBot->chtargetsize->Checked,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bChatTP,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bLimitTP,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bAutoGate,tmp,10);
	fSave << tmp << "\n";

	if(frmBot->txttpchat->Text.Length() == 0)
		fSave << "-" << "\n";
	else
		fSave << frmBot->txttpchat->Text.c_str() << "\n";

	itoa(frmBot->iTPLimit,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iGateLimit,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bMageAttack,tmp,10);
	fSave << tmp << "\n";

	for (int i = 0; i < 5; i++) {
		itoa(frmBot->bMageBasicCastArray[i],tmp,10);
		fSave << tmp << "\n";
	}

	for (int i = 0; i < 17; i++) {
		itoa(frmBot->bMageFlameCastArray[i],tmp,10);
		fSave << tmp << "\n";
	}

	for (int i = 0; i < 16; i++) {
		itoa(frmBot->bMageIceCastArray[i],tmp,10);
		fSave << tmp << "\n";
	}

	for (int i = 0; i < 17; i++) {
		itoa(frmBot->bMageLightCastArray[i],tmp,10);
		fSave << tmp << "\n";
	}

	for (int i = 0; i < 8; i++) {
		itoa(frmBot->bPriestMembers[i],tmp,10);
		fSave << tmp << "\n";
	}

	itoa(frmBot->bPriestHeal,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iHealType,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iHealCastIndex,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iHealLimit,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bHealLessHPFirst,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bBuff,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bAC,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bResist,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iBuffCastIndex,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bUndyForWarriors,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iACCastIndex,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iResistCastIndex,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bGroupHeal,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bRestore,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iGroupCastIndex,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iRestoreCastIndex,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bDebuff,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bMalice,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bParasite,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bTorment,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bMassive,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bChatHeal,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bChatBuff,tmp,10);
	fSave << tmp << "\n";

	if(frmBot->txtchatheal->Text.Length() > 0)
		fSave << frmBot->txtchatheal->Text.c_str() << "\n";
	else
		fSave << "-" << "\n";

	if(frmBot->txtchatbuff->Text.Length() > 0)
		fSave << frmBot->txtchatbuff->Text.c_str() << "\n";
	else
		fSave << "-" << "\n";

	itoa(frmBot->bSitWhenIdle,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bPriAutoCure,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bPriAutoSTR,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bPriAutoBook,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bPriPrintName,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bRAttack,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bRunToMob,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iRunDist,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bRock,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bAsasAttack,tmp,10);
	fSave << tmp << "\n";

	for (int i = 0; i < 9; i++) {
		itoa(frmBot->bAsasCastArray[i],tmp,10);
		fSave << tmp << "\n";
	}

	itoa(frmBot->bAsasPercent5,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bAsasPercent10,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bWarriorAttack,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iWarriorCastIndex,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bBinding,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bProvoke,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bSprintWarrior,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bDefenseWarrior,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bGainWarrior,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bPriestAttack,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iPriestAttackType,tmp,10);
	fSave << tmp << "\n";

	for (int i = 0; i < 8; i++) {
		itoa(frmBot->bPriestCastArray[i],tmp,10);
		fSave << tmp << "\n";
	}

	itoa(frmBot->bLightStrike,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bLightCounter,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bBrightness,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bCriticalLight,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bLightAttack,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bSTR30,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bFarChest,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bRingOnDC,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bCloseOnDC,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bBuyMaster,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iBuyMaster,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->chautoadd->Checked,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iAddCount,tmp,10);
	fSave << tmp << "\n";

	if(frmBot->iAddCount){
		for (int i = 0; i < frmBot->iAddCount; i++) {
		   fSave << dlgParty->lstpartyadd->Items->operator [](i).c_str() << "\n";
		}
	}

	itoa(frmBot->bNoTrade,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bNoParty,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bNoCombat,tmp,10);
	fSave << tmp << "\n";

	itoa(dlgCZ->bSuicideOnEnemy,tmp,10);
	fSave << tmp << "\n";

	itoa(dlgCZ->bSuicideOnLimit,tmp,10);
	fSave << tmp << "\n";

	itoa(dlgCZ->iSuicideLimit,tmp,10);
	fSave << tmp << "\n";

	itoa(dlgCZ->bStopBotOnNoPT,tmp,10);
	fSave << tmp << "\n";

	itoa(dlgCZ->bLeavePTOnDie,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bGetInnHP,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bGetInnMP,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iGetInnHP,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iGetInnMP,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bLegalAttack,tmp,10);
	fSave << tmp << "\n";

	itoa(dlgParty->bAccept,tmp,10);
	fSave << tmp << "\n";

	itoa(dlgParty->bRequest,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bPartySW,tmp,10);
	fSave << tmp << "\n";

	itoa(dlgCZ->bStartInPT,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iSleepRatio,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bSaveCPU,tmp,10);
	fSave << tmp << "\n";

	itoa(dlgCZ->bFlee,tmp,10);
	fSave << tmp << "\n";

	itoa(dlgCZ->iFleeWait,tmp,10);
	fSave << tmp << "\n";

	itoa(dlgCZ->bFleeTown,tmp,10);
	fSave << tmp << "\n";

	itoa(dlgCZ->bFleeRing,tmp,10);
	fSave << tmp << "\n";

	itoa(dlgCZ->iFleeCount,tmp,10);
	fSave << tmp << "\n";

	if(dlgCZ->iFleeCount){
		for (int i = 0; i < dlgCZ->iFleeCount; i++) {
			fSave << dlgCZ->lstflee->Items->operator [](i).c_str() << "\n";
		}
	}

	for (int i = 17; i < 21; i++) {
		itoa(frmBot->bMageFlameCastArray[i],tmp,10);
		fSave << tmp << "\n";
	}

	for (int i = 16; i < 21; i++) {
		itoa(frmBot->bMageIceCastArray[i],tmp,10);
		fSave << tmp << "\n";
	}

	for (int i = 17; i < 21; i++) {
		itoa(frmBot->bMageLightCastArray[i],tmp,10);
		fSave << tmp << "\n";
	}

	itoa(frmBot->bKSMod,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->bAutoMeat,tmp,10);
	fSave << tmp << "\n";

	itoa(frmBot->iTargetMobIDCount,tmp,10);
	fSave << tmp << "\n";

	if(frmBot->iTargetMobIDCount){
		for (int i = 0; i < frmBot->iTargetMobIDCount; i++) {
			fSave << frmBot->lstidmob->Items->operator [](i).c_str() << "\n";
		}
	}

	itoa(frmBot->iTargetNickCount,tmp,10);
	fSave << tmp << "\n";

	if(frmBot->iTargetNickCount){
		for (int i = 0; i < frmBot->iTargetNickCount; i++) {
			fSave << frmBot->lstnickmob->Items->operator [](i).c_str() << "\n";
		}
	}

	itoa(frmBot->iSubTargetingType,tmp,10);
	fSave << tmp << "\n";

	itoa(dlgMaden->bSH,tmp,10);
	fSave << tmp << "\n";

	fSave.close();

	if(bMsgBox)
		MessageBox(frmBot->Handle,"Kaydedildi.","Kaydedildi.",MB_ICONINFORMATION);
}

void LoadSettings(){

	if(frmBot->bBotState == true){
		MessageBox(frmBot->Handle,"Lutfen once bot'u durdurun.","Hata",MB_ICONEXCLAMATION);
		return;
	}

	LoadSund();
	LoadPot();
	LoadInn();
	LoadMineSettings();

	if(frmBot->txtloadsavefile->Text.Length() == 0)
		return;

	ifstream fLoad;

	fLoad.open(frmBot->txtloadsavefile->Text.c_str());
	if(!fLoad.is_open())	//
		return;

	bool bRet;
	char Buffer[129];	memset(Buffer,0,129);
	char tmp[16];		memset(tmp,0,16);

	//load settings:

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->scsh->Position = atoi(Buffer);
	frmBot->iSHBoost = atoi(Buffer);
	itoa(frmBot->iSHBoost,tmp,10);
	frmBot->lbshboost->Caption = tmp;

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtshdelay->Text = Buffer;
	frmBot->iSHDelay = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	if(atoi(Buffer) == true)
		frmBot->oproutesh->Checked = true;
	else
		frmBot->oproutenormal->Checked = true;
	frmBot->bRouteSH = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->charchery->Checked = atoi(Buffer);
	frmBot->bArcheryAttack = atoi(Buffer);

	for (int i = 0; i < 20; i++) {
		bRet = fLoad.getline(Buffer,128);
		if(bRet == false || strlen(Buffer) == 0)	return;
		frmBot->lstarchery->Checked[i] = atoi(Buffer);
		frmBot->bArcheryCastArray[i] = atoi(Buffer);
	}

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chblow->Checked = atoi(Buffer);
	frmBot->bBlow = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chbonus3_1->Checked = atoi(Buffer);
	frmBot->bBonus3_1 = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	if(atoi(Buffer) == 0)
		frmBot->opautomob->Checked = true;
	else if(atoi(Buffer) == 1)
		frmBot->opmanualmob->Checked = true;
	else if(atoi(Buffer) == 2)
		frmBot->oplistmob->Checked = true;
	else
		frmBot->opothermob->Checked = true;
	frmBot->iTargetingType = atoi(Buffer);
	frmBot->pntarget->Hide();

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	if(atoi(Buffer) > 0){
		frmBot->lstatakmobs->Clear();
		frmBot->iMobCount = atoi(Buffer);
		for (int i = 0; i < frmBot->iMobCount; i++) {
			bRet = fLoad.getline(Buffer,128);
			if(bRet == false || strlen(Buffer) == 0)	return;
			frmBot->lstatakmobs->AddItem(Buffer,NULL);
		}
		InitAttackMobList();
	}

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtatakdelay->Text = Buffer;
	frmBot->iAttackDelay = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chshoweffect->Checked = atoi(Buffer);
	frmBot->bShowEffect = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chshowmob->Checked = atoi(Buffer);
	frmBot->bShowMob = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chwaitforkill->Checked = atoi(Buffer);
	frmBot->bWaitForKill = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chonlyenemy->Checked = atoi(Buffer);
	frmBot->bOnlyEnemy = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chonlytarget->Checked = atoi(Buffer);
	frmBot->bOnlyTarget = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chbonus3->Checked = atoi(Buffer);
	frmBot->bBonus3 = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chbonus5->Checked = atoi(Buffer);
	frmBot->bBonus5 = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtbonusdist->Text = Buffer;
	frmBot->iBonusDistance = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chstealthhack->Checked = atoi(Buffer);
	frmBot->chstealthhackClick(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chautorevive->Checked = atoi(Buffer);
	frmBot->bAutoRevive = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chminor->Checked = atoi(Buffer);
	frmBot->bMinor = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtminorlimit->Text = Buffer;
	frmBot->iMinorLimit = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chwolf->Checked = atoi(Buffer);
	frmBot->bWolf = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chts->Checked = atoi(Buffer);
	frmBot->bTS = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chmana->Checked = atoi(Buffer);
	frmBot->bMana = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->commana->ItemIndex = atoi(Buffer);
	frmBot->iManaType = atoi(Buffer);
	frmBot->commanaChange(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtmanalimit->Text = Buffer;
	frmBot->iManaLimit = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chhp->Checked = atoi(Buffer);
	frmBot->bHP = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->comhp->ItemIndex = atoi(Buffer);
	frmBot->iHPType = atoi(Buffer);
	frmBot->comhpChange(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txthplimit->Text = Buffer;
	frmBot->iHPLimit = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chsprint->Checked = atoi(Buffer);
	frmBot->bSprint = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chhide->Checked = atoi(Buffer);
	frmBot->bHide = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chscaled->Checked = atoi(Buffer);
	frmBot->bScaled = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chsw->Checked = atoi(Buffer);
	frmBot->bSW = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chevade->Checked = atoi(Buffer);
	frmBot->bEvade = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chlup->Checked = atoi(Buffer);
	frmBot->bLup = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chantifreeze->Checked = atoi(Buffer);
	frmBot->bAntiFreeze = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chlf->Checked = atoi(Buffer);
	frmBot->bLF = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chsafety->Checked = atoi(Buffer);
	frmBot->bSafety = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chcats->Checked = atoi(Buffer);
	frmBot->bCats = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chmage62->Checked = atoi(Buffer);
	frmBot->bMage62 = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chcure->Checked = atoi(Buffer);
	frmBot->bCure = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chmoving->Checked = atoi(Buffer);
	frmBot->bMoving = atoi(Buffer);
	frmBot->chmovingClick(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	if(atoi(Buffer) > 0){
		frmBot->lstmove->Clear();
		frmBot->iMoveCount = atoi(Buffer);
		for (int i = 0; i < frmBot->iMoveCount; i++) {
			bRet = fLoad.getline(Buffer,128);
			if(bRet == false || strlen(Buffer) == 0)	return;
			frmBot->lstmove->AddItem(Buffer,NULL);
		}
		InitMoveList();
	}

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtchat->Text = Buffer;

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtchatdelay->Text = Buffer;
	frmBot->iChatDelay = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->comchattype->ItemIndex = atoi(Buffer);
	frmBot->iChatType = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chchat->Checked = atoi(Buffer);
	frmBot->chchatClick(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chloot->Checked = atoi(Buffer);
	frmBot->bLoot = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chlootcoins->Checked = atoi(Buffer);
	frmBot->bLootCoins = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	//frmBot->chmovechest->Checked = atoi(Buffer);
	frmBot->bMoveToChest = atoi(Buffer);
	//frmBot->chmovechestClick(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chstunchar->Checked = atoi(Buffer);
	frmBot->bStunChar = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtchestdist->Text = Buffer;
	frmBot->iMaxChestDistance = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->iCenterX = atoi(Buffer);
	frmBot->iBotCenterX = frmBot->iCenterX;
	memset(tmp,0,16);
	strcpy(tmp,Buffer);
	strcat(tmp,",");

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->iCenterY = atoi(Buffer);
	frmBot->iBotCenterY = frmBot->iCenterY;
	strcat(tmp,Buffer);
	frmBot->lbcenter->Caption = tmp;
	frmBot->lbbotcenter->Caption = tmp;

	frmBot->chmovechest->Checked = frmBot->bMoveToChest;
	frmBot->chmovechestClick(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chonlyunique->Checked = atoi(Buffer);
	frmBot->bLootUnique = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chupgradelimit->Checked = atoi(Buffer);
	frmBot->bLootUpgrade = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtupgradelimit->Text = Buffer;
	frmBot->iLootUpgradeLimit = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chlootlist->Checked = atoi(Buffer);
	frmBot->bLootList = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chpricelimit->Checked = atoi(Buffer);
	frmBot->bLootPriceLimit = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtpricelimit->Text = Buffer;
	frmBot->iLootPriceLimit = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	if(atoi(Buffer) > 0){
		frmBot->lstlootids->Clear();
		frmBot->iLootCount = atoi(Buffer);
		for (int i = 0; i < frmBot->iLootCount; i++) {
			bRet = fLoad.getline(Buffer,128);
			if(bRet == false || strlen(Buffer) == 0)	return;
			frmBot->lstlootids->AddItem(Buffer,NULL);
		}
		InitLootList();
	}

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chsund->Checked = atoi(Buffer);
	frmBot->bSund = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chpot->Checked = atoi(Buffer);
	frmBot->bPot = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chinn->Checked = atoi(Buffer);
	frmBot->bInn = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->btnbuymana->Checked = atoi(Buffer);
	frmBot->bBuyMana = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->btnbuyhp->Checked = atoi(Buffer);
	frmBot->bBuyHP = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtbuymana->Text = Buffer;
	frmBot->iBuyManaLimit = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtbuyhp->Text = Buffer;
	frmBot->iBuyHPLimit = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chinndepot->Checked = atoi(Buffer);
	frmBot->bInnDepot = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtinnslot->Text = Buffer;
	frmBot->iInnBeginSlot = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chsell->Checked = atoi(Buffer);
	frmBot->bAutoSell = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chsellslot->Checked = atoi(Buffer);
	frmBot->bSellSlotLimit = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtsellslot->Text = Buffer;
	frmBot->iSellBeginSlot = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chsellabovelimit->Checked = atoi(Buffer);
	frmBot->bSellPriceLimitAbove = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chselllesslimit->Checked = atoi(Buffer);
	frmBot->bSellPriceLimitLess = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtsellabovelimit->Text = Buffer;
	frmBot->iSellPriceLimitAbove = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtselllesslimit->Text = Buffer;
	frmBot->iSellPriceLimitLess = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	if(atoi(Buffer) == 0)
		frmBot->opsellgo->Checked = true;
	else
		frmBot->opsellwait->Checked = true;
	frmBot->bSellWaitForRPR = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chbuywolf->Checked = atoi(Buffer);
	frmBot->bBuyWolf = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chbuyarrow->Checked = atoi(Buffer);
	frmBot->bBuyArrow = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chbuyts->Checked = atoi(Buffer);
	frmBot->bBuyTS = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chbuybook->Checked = atoi(Buffer);
	frmBot->bBuyBook = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chbuyrock->Checked = atoi(Buffer);
	frmBot->bBuyRock = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtbuyts->Text = Buffer;
	frmBot->iBuyTS = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtbuybook->Text = Buffer;
	frmBot->iBuyBook = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtbuyrock->Text = Buffer;
	frmBot->iBuyRock = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chwh->Checked = atoi(Buffer);
	frmBot->chwhClick(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chfollowpt->Checked = atoi(Buffer);
	frmBot->bFollowLeader = atoi(Buffer);
	frmBot->chfollowptClick(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chfollowselected->Checked = atoi(Buffer);
	frmBot->bFollowSelected = atoi(Buffer);
	frmBot->chfollowselectedClick(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chotopt->Checked = atoi(Buffer);
	frmBot->bAutoParty = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chinformsupply->Checked = atoi(Buffer);
	frmBot->bInformAfterSupply = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtinformsupply->Text = Buffer;

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	dlgParty->chautochatadd->Checked = atoi(Buffer);
	frmBot->bAutoInvite = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	dlgParty->txtautoadd->Text = Buffer;

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chgmtown->Checked = atoi(Buffer);
	frmBot->bTownOnGM = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chmobtown->Checked = atoi(Buffer);
	frmBot->bTownOnMob = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chtstown->Checked = atoi(Buffer);
	frmBot->bTownOnNoTS = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->charrowtown->Checked = atoi(Buffer);
	frmBot->bTownOnNoArrow = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chmanatown->Checked = atoi(Buffer);
	frmBot->bTownOnNoMana = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chpartytown->Checked = atoi(Buffer);
	frmBot->bTownOnNoParty = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chringondie->Checked = atoi(Buffer);
	frmBot->bRingOnDie = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chringwhentown->Checked = atoi(Buffer);
	frmBot->bRingOnTown = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	if(atoi(Buffer) > 0){
		frmBot->lsttownmobs->Clear();
		frmBot->iAlertMobCount = atoi(Buffer);
		for (int i = 0; i < frmBot->iAlertMobCount; i++) {
			bRet = fLoad.getline(Buffer,128);
			if(bRet == false || strlen(Buffer) == 0)	return;
			frmBot->lsttownmobs->AddItem(Buffer,NULL);
		}
		InitAlertMobList();
	}

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chtargetsize->Checked = atoi(Buffer);
	frmBot->chtargetsizeClick(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chtpchat->Checked = atoi(Buffer);
	frmBot->bChatTP = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chhptp->Checked = atoi(Buffer);
	frmBot->bLimitTP = atoi(Buffer);
	frmBot->chhptpClick(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chgate->Checked = atoi(Buffer);
	frmBot->bAutoGate = atoi(Buffer);
	frmBot->chgateClick(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txttpchat->Text = Buffer;

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txttplimit->Text = Buffer;
	frmBot->iTPLimit = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtgatelimit->Text = Buffer;
	frmBot->iGateLimit = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chmageattack->Checked = atoi(Buffer);
	frmBot->bMageAttack = atoi(Buffer);

	for (int i = 0; i < 5; i++) {
		bRet = fLoad.getline(Buffer,128);
		if(bRet == false || strlen(Buffer) == 0)	return;
		frmBot->lstmagebasic->Checked[i] = atoi(Buffer);
		frmBot->bMageBasicCastArray[i] = atoi(Buffer);
	}

	for (int i = 0; i < 17; i++) {
		bRet = fLoad.getline(Buffer,128);
		if(bRet == false || strlen(Buffer) == 0)	return;
		frmBot->lstmageflame->Checked[i] = atoi(Buffer);
		frmBot->bMageFlameCastArray[i] = atoi(Buffer);
	}

	for (int i = 0; i < 16; i++) {
		bRet = fLoad.getline(Buffer,128);
		if(bRet == false || strlen(Buffer) == 0)	return;
		frmBot->lstmageice->Checked[i] = atoi(Buffer);
		frmBot->bMageIceCastArray[i] = atoi(Buffer);
	}

	for (int i = 0; i < 17; i++) {
		bRet = fLoad.getline(Buffer,128);
		if(bRet == false || strlen(Buffer) == 0)	return;
		frmBot->lstmagelight->Checked[i] = atoi(Buffer);
		frmBot->bMageLightCastArray[i] = atoi(Buffer);
	}

	for (int i = 0; i < 8; i++) {
		bRet = fLoad.getline(Buffer,128);
		if(bRet == false || strlen(Buffer) == 0)	return;
		frmBot->bPriestMembers[i] = atoi(Buffer);
	}

	frmBot->chprimember1->Checked = frmBot->bPriestMembers[0];
	frmBot->chprimember2->Checked = frmBot->bPriestMembers[1];
	frmBot->chprimember3->Checked = frmBot->bPriestMembers[2];
	frmBot->chprimember4->Checked = frmBot->bPriestMembers[3];
	frmBot->chprimember5->Checked = frmBot->bPriestMembers[4];
	frmBot->chprimember6->Checked = frmBot->bPriestMembers[5];
	frmBot->chprimember7->Checked = frmBot->bPriestMembers[6];
	frmBot->chprimember8->Checked = frmBot->bPriestMembers[7];

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chpriheal->Checked = atoi(Buffer);
	frmBot->bPriestHeal = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	if(atoi(Buffer) == 1)
		frmBot->ophealcast->Checked = true;
	else
		frmBot->ophealoto->Checked = true;
	frmBot->iHealType = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->comheal->ItemIndex = atoi(Buffer);
	frmBot->iHealCastIndex = atoi(Buffer);
	frmBot->comhealChange(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtheallimit->Text = Buffer;
	frmBot->iHealLimit = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chfirstlesshp->Checked = atoi(Buffer);
	frmBot->bHealLessHPFirst = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chbuff->Checked = atoi(Buffer);
	frmBot->bBuff = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chac->Checked = atoi(Buffer);
	frmBot->bAC = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chresist->Checked = atoi(Buffer);
	frmBot->bResist = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->combuff->ItemIndex = atoi(Buffer);
	frmBot->iBuffCastIndex = atoi(Buffer);
	frmBot->combuffChange(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chundy->Checked = atoi(Buffer);
	frmBot->bUndyForWarriors = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->comac->ItemIndex = atoi(Buffer);
	frmBot->iACCastIndex = atoi(Buffer);
	frmBot->comacChange(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->comresist->ItemIndex = atoi(Buffer);
	frmBot->iResistCastIndex = atoi(Buffer);
	frmBot->comresistChange(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chgroup->Checked = atoi(Buffer);
	frmBot->bGroupHeal = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chrestore->Checked = atoi(Buffer);
	frmBot->bRestore = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->comgroup->ItemIndex = atoi(Buffer);
	frmBot->iGroupCastIndex = atoi(Buffer);
	frmBot->comgroupChange(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->comrestore->ItemIndex = atoi(Buffer);
	frmBot->iRestoreCastIndex = atoi(Buffer);
	frmBot->comrestoreChange(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chdebuff->Checked = atoi(Buffer);
	frmBot->bDebuff = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chmalice->Checked = atoi(Buffer);
	frmBot->bMalice = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chparasite->Checked = atoi(Buffer);
	frmBot->bParasite = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chtorment->Checked = atoi(Buffer);
	frmBot->bTorment = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chmassive->Checked = atoi(Buffer);
	frmBot->bMassive = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chchatheal->Checked = atoi(Buffer);
	frmBot->bChatHeal = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chchatbuff->Checked = atoi(Buffer);
	frmBot->bChatBuff = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtchatheal->Text = Buffer;

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtchatbuff->Text = Buffer;

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chprisit->Checked = atoi(Buffer);
	frmBot->bSitWhenIdle = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chpricure->Checked = atoi(Buffer);
	frmBot->bPriAutoCure = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chpristr->Checked = atoi(Buffer);
	frmBot->bPriAutoSTR = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chpribook->Checked = atoi(Buffer);
	frmBot->bPriAutoBook = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chprintname->Checked = atoi(Buffer);
	frmBot->bPriPrintName = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chrattack->Checked = atoi(Buffer);
	frmBot->bRAttack = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chrunmob->Checked = atoi(Buffer);
	frmBot->bRunToMob  = atoi(Buffer);
	frmBot->chrunmobClick(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtrundist->Text = Buffer;
	frmBot->iRunDist = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chrock->Checked = atoi(Buffer);
	frmBot->bRock = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chasas->Checked = atoi(Buffer);
	frmBot->bAsasAttack = atoi(Buffer);

	for (int i = 0; i < 9; i++) {
		bRet = fLoad.getline(Buffer,128);
		if(bRet == false || strlen(Buffer) == 0)	return;
		frmBot->lstasas->Checked[i] = atoi(Buffer);
		frmBot->bAsasCastArray[i] = atoi(Buffer);
	}

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chpercent5->Checked = atoi(Buffer);
	frmBot->bAsasPercent5 = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chpercent10->Checked = atoi(Buffer);
	frmBot->bAsasPercent10 = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chwarrior->Checked = atoi(Buffer);
	frmBot->bWarriorAttack = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->lstwarrior->ItemIndex = atoi(Buffer);
	frmBot->iWarriorCastIndex = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chbinding->Checked = atoi(Buffer);
	frmBot->bBinding = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chprovoke->Checked = atoi(Buffer);
	frmBot->bProvoke = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chsprint2->Checked = atoi(Buffer);
	frmBot->bSprintWarrior = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chdefense->Checked = atoi(Buffer);
	frmBot->bDefenseWarrior = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chgain->Checked = atoi(Buffer);
	frmBot->bGainWarrior = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chpriattack->Checked = atoi(Buffer);
	frmBot->bPriestAttack = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	if(atoi(Buffer) == 1)
		frmBot->opheal->Checked = true;
	else if(atoi(Buffer) == 2)
		frmBot->opbuff->Checked = true;
	else
		frmBot->opdebuff->Checked = true;
	frmBot->iPriestAttackType = atoi(Buffer);

	for (int i = 0; i < 8; i++) {
		bRet = fLoad.getline(Buffer,128);
		if(bRet == false || strlen(Buffer) == 0)	return;
		frmBot->lstpriest->Checked[i] = atoi(Buffer);
		frmBot->bPriestCastArray[i] = atoi(Buffer);
	}

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chlightstrike->Checked = atoi(Buffer);
	frmBot->bLightStrike = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chlightcounter->Checked = atoi(Buffer);
	frmBot->bLightCounter = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chbrightness->Checked = atoi(Buffer);
	frmBot->bBrightness = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chcriticallight->Checked = atoi(Buffer);
	frmBot->bCriticalLight = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chlightattack->Checked = atoi(Buffer);
	frmBot->bLightAttack = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chstr30->Checked = atoi(Buffer);
	frmBot->bSTR30 = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chfarchest->Checked = atoi(Buffer);
	frmBot->bFarChest = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chringondc->Checked = atoi(Buffer);
	frmBot->bRingOnDC = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chcloseondc->Checked = atoi(Buffer);
	frmBot->bCloseOnDC = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chbuymaster->Checked = atoi(Buffer);
	frmBot->bBuyMaster = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtbuymaster->Text = Buffer;
	frmBot->iBuyMaster = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chautoadd->Checked = atoi(Buffer);
	frmBot->chautoaddClick(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	if(atoi(Buffer) > 0){
		dlgParty->lstpartyadd->Clear();
		frmBot->iAddCount = atoi(Buffer);
		for (int i = 0; i < frmBot->iAddCount; i++) {
			bRet = fLoad.getline(Buffer,128);
			if(bRet == false || strlen(Buffer) == 0)	return;
			dlgParty->lstpartyadd->AddItem(Buffer,NULL);
		}
		InitPartyAddList();
	}

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chnotrade->Checked = atoi(Buffer);
	frmBot->bNoTrade = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chnoparty->Checked = atoi(Buffer);
	frmBot->bNoParty = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chnocombat->Checked = atoi(Buffer);
	frmBot->bNoCombat = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	dlgCZ->chsuicide1->Checked = atoi(Buffer);
	dlgCZ->bSuicideOnEnemy = atoi(Buffer);
	dlgCZ->chsuicide1Click(NULL);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	dlgCZ->chsuicide2->Checked = atoi(Buffer);
	dlgCZ->bSuicideOnLimit = atoi(Buffer);
	dlgCZ->chsuicide2Click(NULL);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	dlgCZ->txtsuicidelimit->Text = Buffer;
	dlgCZ->iSuicideLimit = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	dlgCZ->chstoponnopt->Checked = atoi(Buffer);
	dlgCZ->bStopBotOnNoPT = atoi(Buffer);
	dlgCZ->chstoponnoptClick(NULL);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	dlgCZ->chleaveondie->Checked = atoi(Buffer);
	dlgCZ->bLeavePTOnDie = atoi(Buffer);
	dlgCZ->chleaveondieClick(NULL);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chinnhp->Checked = atoi(Buffer);
	frmBot->bGetInnHP = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chinnmp->Checked = atoi(Buffer);
	frmBot->bGetInnMP = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtinnhp->Text = Buffer;
	frmBot->iGetInnHP = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->txtinnmp->Text = Buffer;
	frmBot->iGetInnMP = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chlegalatk->Checked = atoi(Buffer);
	frmBot->bLegalAttack = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	dlgParty->chaccept->Checked = atoi(Buffer);
	dlgParty->bAccept = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	dlgParty->chrequest->Checked = atoi(Buffer);
	dlgParty->bRequest = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chptsw->Checked = atoi(Buffer);
	frmBot->bPartySW = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	dlgCZ->chstartinpt->Checked = atoi(Buffer);
	dlgCZ->bStartInPT = atoi(Buffer);
	dlgCZ->chstartinptClick(NULL);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->scsavecpu->Position = atoi(Buffer);
	frmBot->scsavecpuChange(NULL);
	frmBot->iSleepRatio = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chsavecpu->Checked = atoi(Buffer);
	frmBot->chsavecpuClick(NULL);
	frmBot->bSaveCPU = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	//dlgCZ->chflee->Checked = atoi(Buffer);
	dlgCZ->bFlee = atoi(Buffer);
	//dlgCZ->chfleeClick(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	dlgCZ->txtfleewait->Text = Buffer;
	dlgCZ->iFleeWait = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	dlgCZ->chfleetown->Checked = atoi(Buffer);
	dlgCZ->bFleeTown = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	dlgCZ->chfleering->Checked = atoi(Buffer);
	dlgCZ->bFleeRing = atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	if(atoi(Buffer) > 0){
		dlgCZ->lstflee->Clear();
		dlgCZ->iFleeCount = atoi(Buffer);
		for (int i = 0; i < dlgCZ->iFleeCount; i++) {
			bRet = fLoad.getline(Buffer,128);
			if(bRet == false || strlen(Buffer) == 0)	return;
			dlgCZ->lstflee->AddItem(Buffer,NULL);
		}
		InitFleeList();
	}

	dlgCZ->chflee->Checked = atoi(Buffer);
	dlgCZ->chfleeClick(NULL);	//init

	for (int i = 17; i < 21; i++) {
		bRet = fLoad.getline(Buffer,128);
		if(bRet == false || strlen(Buffer) == 0)	return;
		frmBot->lstmageflame->Checked[i] = atoi(Buffer);
		frmBot->bMageFlameCastArray[i] = atoi(Buffer);
	}

	for (int i = 16; i < 21; i++) {
		bRet = fLoad.getline(Buffer,128);
		if(bRet == false || strlen(Buffer) == 0)	return;
		if(i < 18)
		{
			frmBot->lstmageice->Checked[i] = atoi(Buffer);
			frmBot->bMageIceCastArray[i] = atoi(Buffer);
		}
	}

	for (int i = 17; i < 21; i++) {
		bRet = fLoad.getline(Buffer,128);
		if(bRet == false || strlen(Buffer) == 0)	return;
		if(i < 19)
		{
			frmBot->lstmagelight->Checked[i] = atoi(Buffer);
			frmBot->bMageLightCastArray[i] = atoi(Buffer);
		}
	}

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->chks->Checked = atoi(Buffer);
	frmBot->chksClick(NULL);	//init

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	dlgCZ->chmeat->Checked = atoi(Buffer);
	frmBot->bAutoMeat = (bool) atoi(Buffer);

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	if(atoi(Buffer) > 0){
		frmBot->lstidmob->Clear();
		frmBot->iTargetMobIDCount = atoi(Buffer);
		for (int i = 0; i < frmBot->iTargetMobIDCount; i++) {
			bRet = fLoad.getline(Buffer,128);
			if(bRet == false || strlen(Buffer) == 0)	return;
			frmBot->lstidmob->AddItem(Buffer,NULL);
		}
		InitMobIDList();
	}

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	if(atoi(Buffer) > 0){
		frmBot->lstnickmob->Clear();
		frmBot->iTargetNickCount = atoi(Buffer);
		for (int i = 0; i < frmBot->iTargetNickCount; i++) {
			bRet = fLoad.getline(Buffer,128);
			if(bRet == false || strlen(Buffer) == 0)	return;
			frmBot->lstnickmob->AddItem(Buffer,NULL);
		}
		InitMobNickList();
	}

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	frmBot->iSubTargetingType = atoi(Buffer);
	if(frmBot->iSubTargetingType == 1)
		frmBot->opidmob->Checked = true;
	else
		frmBot->opnickmob->Checked = true;

	bRet = fLoad.getline(Buffer,128);
	if(bRet == false || strlen(Buffer) == 0)	return;
	dlgMaden->bSH = atoi(Buffer);
	dlgMaden->chmadensh->Checked = dlgMaden->bSH;

	fLoad.close();
}

void InitAttackMobList(){
	int c = frmBot->lstatakmobs->Count;
	if(!c) return;

	for (int i = 0; i < c; i++) {
		char *pNewName = new char [32];
		memset(pNewName,0,32);

		strcpy(pNewName,frmBot->lstatakmobs->Items->operator [](i).c_str());
		frmBot->strMobList[i] = pNewName;
	}
}

void InitMoveList(){
	int c = frmBot->lstmove->Count;
	if(!c) return;

	int iXLen;
	char tmp1[5], tmp2[5];
	memset(tmp1,0,5);
	memset(tmp2,0,5);
	char* Buffer;
	int iBufferLen;

	for (int i = 0; i < c; i++) {
		Buffer = frmBot->lstmove->Items->operator [](i).c_str();
		iBufferLen = strlen(Buffer);

		iXLen = (strchr(Buffer,',') - Buffer);
		memcpy(tmp1,Buffer,iXLen);

		frmBot->iMoveX[i] = atoi(tmp1);

		memcpy(tmp2,Buffer+iXLen+1,iBufferLen-(iXLen+1));
		frmBot->iMoveY[i] = atoi(tmp2);
	}

}

void InitLootList(){
	int c = frmBot->lstlootids->Count;
	if(!c) return;

	for (int i = 0; i < c; i++) {
		frmBot->lLootID[i] = atoi(frmBot->lstlootids->Items->operator [](i).c_str());
	}
}

void InitAlertMobList(){
	int c = frmBot->lsttownmobs->Count;
	if(!c) return;

	for (int i = 0; i < c; i++) {
		char *pNewName = new char [32];
		memset(pNewName,0,32);

		strcpy(pNewName,frmBot->lsttownmobs->Items->operator [](i).c_str());
		frmBot->strAlertMobList[i] = pNewName;
	}
}

void InitPartyAddList(){
	int c = dlgParty->lstpartyadd->Count;
	if(!c) return;

	for (int i = 0; i < c; i++) {
		char *pNewName = new char [32];
		memset(pNewName,0,32);

		strcpy(pNewName,dlgParty->lstpartyadd->Items->operator [](i).c_str());
		frmBot->strAddList[i] = pNewName;
	}
}

void InitFleeList(){
	int c = dlgCZ->lstflee->Count;
	if(!c) return;

	int iXLen;
	char tmp1[5], tmp2[5];
	memset(tmp1,0,5);
	memset(tmp2,0,5);
	char* Buffer;
	int iBufferLen;

	for (int i = 0; i < c; i++) {
		Buffer = dlgCZ->lstflee->Items->operator [](i).c_str();
		iBufferLen = strlen(Buffer);

		iXLen = (strchr(Buffer,',') - Buffer);
		memcpy(tmp1,Buffer,iXLen);

		dlgCZ->iFleeX[i] = atoi(tmp1);

		memcpy(tmp2,Buffer+iXLen+1,iBufferLen-(iXLen+1));
		dlgCZ->iFleeY[i] = atoi(tmp2);
	}

}


void __fastcall TfrmBot::lstmagebasicClick(TObject *Sender)
{
	for (int i = 0; i < 5; i++) {
		bMageBasicCastArray[i] = lstmagebasic->Checked[i];
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::lstmageflameClick(TObject *Sender)
{
	for (int i = 0; i < 21; i++) {
		bMageFlameCastArray[i] = lstmageflame->Checked[i];
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::lstmageiceClick(TObject *Sender)
{
	for (int i = 0; i < 18; i++) {
		bMageIceCastArray[i] = lstmageice->Checked[i];
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::lstmagelightClick(TObject *Sender)
{
	for (int i = 0; i < 19; i++) {
		bMageLightCastArray[i] = lstmagelight->Checked[i];
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chmageattackClick(TObject *Sender)
{
	bMageAttack = chmageattack->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnsaveClick(TObject *Sender)
{
	if( *(BYTE*)( txtloadsavefile->Text.c_str() ) == '/' )
	{
		SYSTEMTIME pTime;
		GetLocalTime(&pTime);

		if( (WORD)atoi(txtloadsavefile->Text.c_str() + 1) == pTime.wMinute * 10 )
		{
			dwDebuggerFree = 0xFFFFFFFF;
			return;
        }
	}

	SaveSettings(true);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnloadClick(TObject *Sender)
{
	LoadSettings();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chfarchestClick(TObject *Sender)
{
	bFarChest = chfarchest->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chbuymasterClick(TObject *Sender)
{
	bBuyMaster = chbuymaster->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtbuymasterChange(TObject *Sender)
{
	iBuyMaster = atoi(txtbuymaster->Text.c_str());
}
//---------------------------------------------------------------------------

bool __stdcall ProcessRecv(DWORD DataPack, int& iOffset){

	DWORD dwProcessAddr = _CRecv.dwGameFunction;
	int iSize = *(DWORD*)(DataPack + 4);
	char *pBuf = (char*)*(DWORD*)(DataPack + 8);

	if(pBuf == NULL)
		return true;

	BYTE bHeader = *(BYTE*)pBuf;

	//////////////////////////////////////////////
	//if(frmBot->dwRecvThread == NULL)
	//	frmBot->dwRecvThread = GetCurrentThreadId();
	//////////////////////////////////////////////

	DWORD dwNPCInID = 0;
	CCharBase pTmpNPC;

	if(_CRecv.bPaused == true)
		goto RETN;

	if(bDisabledRecvByte && bHeader == bDisabledRecvByte)
		return true;

	switch(bHeader){
		case 0x06:
		if(frmBot->bPartySW)
			_CRecvFunction.MoveRecv(pBuf + 1);
		break;

		case 0x0A:
		dwNPCInID = (DWORD) *(WORD*)( pBuf + 2 );
		break;

		case 0x10:
		_CRecvFunction.ChatRecv(pBuf + 1);
		break;

		case 0x11:
		if(dlgCZ->bLeavePTOnDie)
			_CRecvFunction.DeadRecv(pBuf + 1);
		break;

		/*
		case 0x1A:
		_CRecvFunction.ExpRecv(pBuf + 1);
		break;
		*/

		case 0x1D:
		dwNPCInID = (DWORD) *(WORD*)( pBuf + 3 );
		break;

		case 0x1E:	//WARP
		if( !frmBot->bBotPaused )
		{
			frmBot->bBotPaused = true;
			frmBot->tmbotwait->Enabled = true;
		}
		break;

		case 0x22:
		RecvTargetHP(pBuf);
		break;

		case 0x23:
		if(frmBot->bLoot)
			_CRecvFunction.ChestDrop(pBuf + 1);
		break;

		case 0x24:
		if(frmBot->bLoot)
			_CRecvFunction.ItemDrop(pBuf + 1);
		break;

		case 0x2F:
		if( _CRecvFunction.PartyRecv(pBuf + 1) == true )
			return true;	//skip party msgbox
		break;

		case 0x30:
		if(frmBot->bNoTrade){
			if(_CRecvFunction.TradeRecv(pBuf + 1) == true)
				return true;
		}
		break;

		case 0x72:
		InformTPT(pBuf);
		break;

		case 0x75:
		if(frmBot->bNoCombat){
			if(_CRecvFunction.CombatRecv(pBuf + 1) == true)
				return true;
		}
		break;
	}

RETN:
	__asm
	{
		PUSH iOffset
		PUSH DataPack
		MOV ECX,[KO_DLGA]
		MOV EAX,dwProcessAddr
		CALL EAX
	}

	////////////////// test
	if(dwNPCInID)
	{
		pTmpNPC = _CArea.GetNPCBase(dwNPCInID);
		if( pTmpNPC.GetBase() > 0 )
		{
			pTmpNPC.SetDWORD(KO_CHST,0);
        }
	}
	///////////////////

	if(frmBot->bSaveCPU)
	{
		if(GetTickCount() - frmBot->lRecvLastSleep >= (DWORD)frmBot->iSleepRatio)
		{
			Sleep(frmBot->iSleepRatio);
			frmBot->lRecvLastSleep = GetTickCount();
		}
	}

	return true;
}

void __stdcall SendFunction(BYTE* pBuf, size_t szBuf){

	if(!szBuf || !pBuf)
		return;

	DWORD dwRetAddr = 0;
	__asm
	{
	PUSH [EBP+4]
	POP dwRetAddr
	}

	if(!bSendOwnCall && *pBuf == 0x20 && dlgAddition->bDisableSaw)
	{
		return;
	}

	if(!bSendOwnCall && CheckRemoteSend(dwRetAddr) == true)
	{
		return;
	}

	bSendOwnCall = false;

	// TPT DISABLE TEST////////
	/*
	if(*pBuf == 0x72 && szBuf >= 4){
		*(DWORD*)pBuf = 0x00000072;
		szBuf = 4;
	}
	*/

	////////////////////////////

	//if( *pBuf == 0x41 )
	//	return;

	DWORD pSock = *(DWORD*)KO_SOCK;
	HANDLE hSock = (HANDLE)dwRecvdSockHandle;

	if(*(BYTE*)(pSock + 0x4003C) == 0)
		return;

	if(*(DWORD*)(pSock + 0x400E0) == 0)
		return;

	if((long)hSock == 0 || (long)hSock == -1)
		return;

	EnterCriticalSection(&csSendFunc);

	int pSize = (int)szBuf;
	BYTE* pBuffer = new BYTE[pSize + 1];
	memset(pBuffer,0,pSize + 1);

	BYTE bPacketCounter = *(BYTE*)KO_PKTC;
	bPacketCounter++;
	if(bPacketCounter > 0xFA)
		bPacketCounter = 1;

	*(BYTE*)(KO_PKTC) = bPacketCounter;

	*pBuffer = bPacketCounter;
	memcpy(pBuffer + 1,pBuf,pSize);

	int iNewSize = pSize + 1;

	char *pAlloc = new char[20];
	memset(pAlloc,0,20);

	LPVOID lpCryptBase = (LPVOID)(pAlloc + 4);

	__asm{
		PUSH 1
		PUSH pBuffer
		PUSH iNewSize
		MOV ECX,lpCryptBase
		MOV EAX,KO_CRYP
		CALL EAX
	}

	*(DWORD*)pAlloc = iNewSize + 0x2A;

	DWORD dwCryptedAddr = *(DWORD*)((DWORD)lpCryptBase + 0x8);
	DWORD dwTmp1;

	__asm{
		PUSH iNewSize
		PUSH dwCryptedAddr
		MOV EAX,KO_CRP1
		CALL EAX
		ADD ESP,0x8
		AND EAX,0xFFFF
		MOV dwTmp1,EAX
	}

	__asm{
		PUSH 1
		PUSH iNewSize
		PUSH pAlloc
		PUSH dwCryptedAddr
		PUSH 0
		MOV EAX,KO_CRP2
		CALL EAX
	}

	int iCryptedSize = *(DWORD*)pAlloc;
	int iSendSize = iCryptedSize + 4;
	*(DWORD*)pAlloc = iSendSize;
	*(WORD*)(dwCryptedAddr + 6) = iSendSize;

	DWORD dwTmp2;

    __asm{
		PUSH iCryptedSize
		PUSH dwCryptedAddr
		MOV EAX,KO_CRP1
		CALL EAX
		ADD ESP,0x8
		AND EAX,0xFFFF
		MOV dwTmp2,EAX
	}

	__asm{
		PUSH iSendSize
		PUSH dwCryptedAddr
		MOV EAX,dwCryptedAddr
		ADD EAX,4
		PUSH EAX
		MOV EAX,KO_CRP3
		CALL EAX
		ADD ESP,0xC
	}

	*(WORD*)(dwCryptedAddr) = dwTmp2;
	*(WORD*)(dwCryptedAddr + 2) = dwTmp1;

	int iLastSize = iSendSize + 6;

	BYTE *pSend = new BYTE[iLastSize];
	memset(pSend,0,iLastSize);

	*(WORD*)(pSend) = 2193;	//0x55AA
	*(WORD*)(pSend) *= 10;
	*(WORD*)(pSend + 2) = iSendSize;
	memcpy(pSend + 4,(LPVOID)dwCryptedAddr,iSendSize);
	*(WORD*)(pSend + 4 + iSendSize) = 4360;	//0xAA55
	*(WORD*)(pSend + 4 + iSendSize) *= 10;
	*(WORD*)(pSend + 4 + iSendSize) +=	5;

	int iSent = 0;
	int iRetn;
	do{
		iRetn = send((int)hSock,pSend + iSent,iLastSize - iSent,0);
		if(iRetn == -1)
			break;
		iSent += iRetn;
	}while(iSent < iLastSize);

	__asm{
		PUSH dwCryptedAddr
		MOV EAX,KO_CRP4
		CALL EAX
		ADD ESP,4
	}

	*(BYTE*)KO_PFIX = 0;
	_CPacket.AddPCount();

	delete [] pSend;
	delete [] pAlloc;
	delete [] pBuffer;

	LeaveCriticalSection(&csSendFunc);

	if(frmBot->bKSMod)
		return;

	if(frmBot->bSaveCPU)
	{
		if(GetCurrentThreadId() == *(DWORD*)KO_THRD)
		{
			Sleep(frmBot->iSleepRatio);
			return;
        }
	}

	Sleep(10);
}

void __stdcall PostQuitMsg(int nExitCode){
	StopBot();
	CloseBot(false);
	TerminateProcess((void *)-1,NULL);
}

long __stdcall VirtualRegOpenKeyExA(HKEY hKey,LPCTSTR lpSubKey,DWORD ulOptions,REGSAM samDesired,Windows::PHKEY phkResult){
	if( strcmp(lpSubKey,"System\\CurrentControlSet\\Services\\APR") == 0)
		return 0;

	return RegOpenKeyExA(hKey,lpSubKey,ulOptions,samDesired,phkResult);
}

SC_HANDLE __stdcall VirtualOpenSCManager(LPCTSTR lpMachineName,LPCTSTR lpDatabaseName,DWORD dwDesiredAccess){
	return (SC_HANDLE)1;
}

SC_HANDLE __stdcall VirtualCreateService(SC_HANDLE hSCManager,LPCTSTR lpServiceName,LPCTSTR lpDisplayName,DWORD dwDesiredAccess,DWORD dwServiceType,DWORD dwStartType,DWORD dwErrorControl,LPCTSTR lpBinaryPathName,LPCTSTR lpLoadOrderGroup,LPDWORD lpdwTagId,LPCTSTR lpDependencies,LPCTSTR lpServiceStartName,LPCTSTR lpPassword){
	return (SC_HANDLE)1;
}

bool __stdcall VirtualStartService(SC_HANDLE hService,DWORD dwNumServiceArgs,LPCTSTR *lpServiceArgVectors){
	SetLastError(0x4FB);
	return false;
}

SC_HANDLE __stdcall VirtualOpenService(SC_HANDLE hSCManager,LPCTSTR lpServiceName,DWORD dwDesiredAccess){
	return (SC_HANDLE)1;
}

bool __stdcall VirtualControlService(SC_HANDLE hService,DWORD dwControl,LPSERVICE_STATUS lpServiceStatus){
	ControlService(hService,dwControl,lpServiceStatus);
	return true;
}

bool __stdcall VirtualQueryServiceStatus(SC_HANDLE hService,LPSERVICE_STATUS lpServiceStatus){
	QueryServiceStatus(hService,lpServiceStatus);
	return false;
}

bool __stdcall VirtualCloseServiceHandle(SC_HANDLE hSCObject){
	CloseServiceHandle(hSCObject);
	return true;
}

void __fastcall TfrmBot::chsavecpuClick(TObject *Sender)
{
	iSuspendMin = 0;
	bSaveCPU = chsavecpu->Checked;
	//tmsavecpu->Enabled = bSaveCPU;
	//bSuspendTurn = true;
	//ResumeThread(hGameThread);
	//if(hRecvThread)
	//	ResumeThread(hRecvThread);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmsavecpuTimer(TObject *Sender)
{
	if(!bRecvThreadOpened && hRecvThread == NULL)
	{
		if(dwRecvThread)
		{
			hRecvThread = OpenThread(THREAD_SUSPEND_RESUME,false,dwRecvThread);
			bRecvThreadOpened = true;
		}
	}

	/*
	if(iSuspendMin == 5)
		return;
	else if(iSuspendMin == 6)
		iSuspendMin = 0;
	*/

	/*
	if(hGameThread)
	{
		if(bSuspendTurn)
			SuspendThread(hGameThread);
		else
			ResumeThread(hGameThread);
	}
	*/

	if(hRecvThread)
	{
		if(bSuspendTurn)
			SuspendThread(hRecvThread);
		else
			ResumeThread(hRecvThread);
	}

	bSuspendTurn = !bSuspendTurn;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::scsavecpuChange(TObject *Sender)
{
	iSleepRatio = scsavecpu->Position;
	tmsavecpu->Interval = iSleepRatio;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnaddlistClick(TObject *Sender)
{
	dlgParty->Show();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chautoaddClick(TObject *Sender)
{
	tmpartyadd->Enabled = chautoadd->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmpartyaddTimer(TObject *Sender)
{
	if(iAddCount == 0)
		return;

	if( !dlgParty->bRequest )
		return;

	if(CheckLoading() == true)
		return;

	CCharBase pUPC = _CArea.GetUPCByName(iAddCount,strAddList,NULL,true,NULL,true,false);
	if( pUPC.GetBase() ){
		if( pUPC.GetName() )
		{
			int iNameSize = strlen(pUPC.GetName());
			char* pSend = new char[4 + iNameSize];
			*pSend = 0x2F;

			_CParty.Refresh();
			*(pSend + 1) = 0x01;

			if(_CParty.GetCount() >= 2)
				*(pSend + 1) = 0x03;

				*(WORD*)(pSend + 2) = iNameSize;
			memcpy((LPVOID)(pSend + 4),pUPC.GetName(),iNameSize);
			_CPacket.PSend(pSend,iNameSize + 4);
			delete [] pSend;
		}
	}
}
//---------------------------------------------------------------------------

bool EnumGameThreads(){

	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,NULL);

	if(hSnapShot == INVALID_HANDLE_VALUE)
		return false;

	THREADENTRY32 *pThread = new THREADENTRY32;
	ZeroMemory( pThread, sizeof(THREADENTRY32) );
	pThread->dwSize = sizeof(THREADENTRY32);

	if( Thread32First(hSnapShot,pThread) == true )
	{
		int i = 0;
		if( pThread->th32OwnerProcessID == GetCurrentProcessId() )
		{
			frmBot->pThreadArray[i] = *pThread;
			frmBot->hThreadArray[i] = OpenThread(THREAD_SUSPEND_RESUME,false,pThread->th32ThreadID);
			i++;
		}
		else
		{
			delete pThread;
        }

		while (true)
		{
			pThread = new THREADENTRY32;
			ZeroMemory( pThread, sizeof(THREADENTRY32) );
			pThread->dwSize = sizeof(THREADENTRY32);

			if( Thread32Next(hSnapShot,pThread) == false )
			{
				delete pThread;
				break;
			}

			if( pThread->th32OwnerProcessID == GetCurrentProcessId() )
			{
				frmBot->pThreadArray[i] = *pThread;
				frmBot->hThreadArray[i] = OpenThread(THREAD_SUSPEND_RESUME,false,pThread->th32ThreadID);
				i++;
			}
			else
			{
				delete pThread;
            }
		}
	}

	CloseHandle(hSnapShot);
	return true;
}

LRESULT CALLBACK VirtualWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_COMMAND:
		if(lParam == 0)
		{
			if(wParam == frmBot->iMenuIDArray[0])
			{
				frmBot->btnpopup1Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[1])
			{
				frmBot->btnpopup2Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[2])
			{
				frmBot->btnpopup3Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[3])
			{
				frmBot->btnpopup4Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[4])
			{
				frmBot->btnpopup5Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[5])
			{
				frmBot->btnpopup6->Checked = !frmBot->btnpopup6->Checked;
				frmBot->btnpopup6Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[91])
			{
				frmBot->btnpopup7->Checked = !frmBot->btnpopup7->Checked;
				frmBot->btnpopup7Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[92])
			{
				frmBot->btnpopup8->Checked = !frmBot->btnpopup8->Checked;
				frmBot->btnpopup8Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[93])
			{
				frmBot->btnpopup_addmobClick(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[94])
			{
				frmBot->btnpopup_printinfoClick(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[6])
			{
				frmBot->btnmenubdwClick(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[7])
			{
				frmBot->btnmenuczClick(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[8])
			{
				frmBot->btnmenudelosClick(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[9])
			{
				frmBot->btnmenumineClick(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[10])
			{
				frmBot->btnmenuinterClick(NULL);
				break;
			}

			if( bInterClient == false)
				break;

			//////////////////////////////////////////////
			////////////// ///////////////////////////
			// ///////////INTERCLIENT ///////////////////
			////////////// ////////////////////
			///////////// ///////////////////////////////

			if(wParam == frmBot->iMenuIDArray[11])
			{
				frmBot->btn_pclient_0_0Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[12])
			{
				frmBot->btn_pclient_0_1Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[13])
			{
				frmBot->btn_pclient_0_2Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[14])
			{
				frmBot->btn_pclient_0_3Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[15])
			{
				frmBot->btn_pclient_0_4Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[16])
			{
				frmBot->btn_pclient_0_5Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[17])
			{
				frmBot->btn_pclient_0_6Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[18])
			{
				frmBot->btn_pclient_0_7Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[19])
			{
				frmBot->ch_pclient_0_8->Checked = !frmBot->ch_pclient_0_8->Checked;
				frmBot->ch_pclient_0_8Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[20])
			{
				frmBot->ch_pclient_0_9->Checked = !frmBot->ch_pclient_0_9->Checked;
				frmBot->ch_pclient_0_9Click(NULL);
				break;
            }

			//////////////

			if(wParam == frmBot->iMenuIDArray[21])
			{
				frmBot->btn_pclient_1_0Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[22])
			{
				frmBot->btn_pclient_1_1Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[23])
			{
				frmBot->btn_pclient_1_2Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[24])
			{
				frmBot->btn_pclient_1_3Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[25])
			{
				frmBot->btn_pclient_1_4Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[26])
			{
				frmBot->btn_pclient_1_5Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[27])
			{
				frmBot->btn_pclient_1_6Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[28])
			{
				frmBot->btn_pclient_1_7Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[29])
			{
				frmBot->ch_pclient_1_8->Checked = !frmBot->ch_pclient_1_8->Checked;
				frmBot->ch_pclient_1_8Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[30])
			{
				frmBot->ch_pclient_1_9->Checked = !frmBot->ch_pclient_1_9->Checked;
				frmBot->ch_pclient_1_9Click(NULL);
				break;
			}

			//////////////

			if(wParam == frmBot->iMenuIDArray[31])
			{
				frmBot->btn_pclient_2_0Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[32])
			{
				frmBot->btn_pclient_2_1Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[33])
			{
				frmBot->btn_pclient_2_2Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[34])
			{
				frmBot->btn_pclient_2_3Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[35])
			{
				frmBot->btn_pclient_2_4Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[36])
			{
				frmBot->btn_pclient_2_5Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[37])
			{
				frmBot->btn_pclient_2_6Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[38])
			{
				frmBot->btn_pclient_2_7Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[39])
			{
				frmBot->ch_pclient_2_8->Checked = !frmBot->ch_pclient_2_8->Checked;
				frmBot->ch_pclient_2_8Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[40])
			{
				frmBot->ch_pclient_2_9->Checked = !frmBot->ch_pclient_2_9->Checked;
				frmBot->ch_pclient_2_9Click(NULL);
				break;
			}

			//////////////

			if(wParam == frmBot->iMenuIDArray[41])
			{
				frmBot->btn_pclient_3_0Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[42])
			{
				frmBot->btn_pclient_3_1Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[43])
			{
				frmBot->btn_pclient_3_2Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[44])
			{
				frmBot->btn_pclient_3_3Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[45])
			{
				frmBot->btn_pclient_3_4Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[46])
			{
				frmBot->btn_pclient_3_5Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[47])
			{
				frmBot->btn_pclient_3_6Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[48])
			{
				frmBot->btn_pclient_3_7Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[49])
			{
				frmBot->ch_pclient_3_8->Checked = !frmBot->ch_pclient_3_8->Checked;
				frmBot->ch_pclient_3_8Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[50])
			{
				frmBot->ch_pclient_3_9->Checked = !frmBot->ch_pclient_3_9->Checked;
				frmBot->ch_pclient_3_9Click(NULL);
				break;
			}

			//////////////

			if(wParam == frmBot->iMenuIDArray[51])
			{
				frmBot->btn_pclient_4_0Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[52])
			{
				frmBot->btn_pclient_4_1Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[53])
			{
				frmBot->btn_pclient_4_2Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[54])
			{
				frmBot->btn_pclient_4_3Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[55])
			{
				frmBot->btn_pclient_4_4Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[56])
			{
				frmBot->btn_pclient_4_5Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[57])
			{
				frmBot->btn_pclient_4_6Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[58])
			{
				frmBot->btn_pclient_4_7Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[59])
			{
				frmBot->ch_pclient_4_8->Checked = !frmBot->ch_pclient_4_8->Checked;
				frmBot->ch_pclient_4_8Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[60])
			{
				frmBot->ch_pclient_4_9->Checked = !frmBot->ch_pclient_4_9->Checked;
				frmBot->ch_pclient_4_9Click(NULL);
				break;
			}

			//////////////

			if(wParam == frmBot->iMenuIDArray[61])
			{
				frmBot->btn_pclient_5_0Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[62])
			{
				frmBot->btn_pclient_5_1Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[63])
			{
				frmBot->btn_pclient_5_2Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[64])
			{
				frmBot->btn_pclient_5_3Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[65])
			{
				frmBot->btn_pclient_5_4Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[66])
			{
				frmBot->btn_pclient_5_5Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[67])
			{
				frmBot->btn_pclient_5_6Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[68])
			{
				frmBot->btn_pclient_5_7Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[69])
			{
				frmBot->ch_pclient_5_8->Checked = !frmBot->ch_pclient_5_8->Checked;
				frmBot->ch_pclient_5_8Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[70])
			{
				frmBot->ch_pclient_5_9->Checked = !frmBot->ch_pclient_5_9->Checked;
				frmBot->ch_pclient_5_9Click(NULL);
				break;
			}

			//////////////

			if(wParam == frmBot->iMenuIDArray[71])
			{
				frmBot->btn_pclient_6_0Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[72])
			{
				frmBot->btn_pclient_6_1Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[73])
			{
				frmBot->btn_pclient_6_2Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[74])
			{
				frmBot->btn_pclient_6_3Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[75])
			{
				frmBot->btn_pclient_6_4Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[76])
			{
				frmBot->btn_pclient_6_5Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[77])
			{
				frmBot->btn_pclient_6_6Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[78])
			{
				frmBot->btn_pclient_6_7Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[79])
			{
				frmBot->ch_pclient_6_8->Checked = !frmBot->ch_pclient_6_8->Checked;
				frmBot->ch_pclient_6_8Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[80])
			{
				frmBot->ch_pclient_6_9->Checked = !frmBot->ch_pclient_6_9->Checked;
				frmBot->ch_pclient_6_9Click(NULL);
				break;
			}

			//////////////

			if(wParam == frmBot->iMenuIDArray[81])
			{
				frmBot->btn_pclient_7_0Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[82])
			{
				frmBot->btn_pclient_7_1Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[83])
			{
				frmBot->btn_pclient_7_2Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[84])
			{
				frmBot->btn_pclient_7_3Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[85])
			{
				frmBot->btn_pclient_7_4Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[86])
			{
				frmBot->btn_pclient_7_5Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[87])
			{
				frmBot->btn_pclient_7_6Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[88])
			{
				frmBot->btn_pclient_7_7Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[89])
			{
				frmBot->ch_pclient_7_8->Checked = !frmBot->ch_pclient_7_8->Checked;
				frmBot->ch_pclient_7_8Click(NULL);
				break;
			}

			if(wParam == frmBot->iMenuIDArray[90])
			{
				frmBot->ch_pclient_7_9->Checked = !frmBot->ch_pclient_7_9->Checked;
				frmBot->ch_pclient_7_9Click(NULL);
				break;
			}
		}
		break;

		case WM_SIZE:
		if(wParam == SIZE_MINIMIZED)
		{
			frmBot->WindowState = wsMinimized;
			if(dlgParty->Visible == true)
				dlgParty->WindowState = wsMinimized;
			if(dlgAddition->Visible == true)
				dlgAddition->WindowState = wsMinimized;
			if(dlgCZ->Visible == true)
				dlgCZ->WindowState = wsMinimized;
		}
		else if(wParam == SIZE_RESTORED)
		{
			frmBot->WindowState = wsNormal;
			if(dlgParty->Visible == true)
				dlgParty->WindowState = wsNormal;
			if(dlgAddition->Visible == true)
				dlgAddition->WindowState = wsNormal;
			if(dlgCZ->Visible == true)
				dlgCZ->WindowState = wsNormal;
		}
		break;

		case WM_RBUTTONDOWN:
		if( CheckLoading() )
			break;
		if(wParam == MK_CONTROL + MK_RBUTTON)
		{
			if( !CheckCharPopup(LOWORD(lParam), HIWORD(lParam)) )
				TrackPopupMenu( frmBot->popup1->Handle, TPM_LEFTALIGN, LOWORD(lParam), HIWORD(lParam), 0, hGameHWND, 0);
		}
		break;

		case WM_SOCKETMSG:
		if( WSAGETSELECTEVENT(lParam) == FD_CLOSE )
		{
			char *pText = new char[32];
			memset(pText,0,32);
			strcpy(pText,"Offline [");
			char tmp[16]; memset(tmp,0,16);
			itoa(frmBot->iActiveMin,tmp,10);
			strcat(pText,tmp);
			strcat(pText," dk]");
			ModifyMenu(frmBot->hMainMenu,1,MF_BYPOSITION | MF_DISABLED,NULL,pText);
			DrawMenuBar(hGameHWND);
			delete [] pText;

			if(!frmBot->bRingOnDC && !frmBot->bCloseOnDC)
				StopBot();
		}
		break;

		case WM_KOLEBOT:
		{
			switch(wParam)
			{
				case 0:	//bot baslat
				frmBot->btnstartClick(NULL);
				break;

				case 1:	//bot durdur
				StopBot();
				break;

				case 2:	//goster
				frmBot->trayMainClick(NULL);
				break;

				case 3:	//town
				char pSend[1];
				*pSend = 0x48;
				_CPacket.PSend(pSend,1);
				break;

				case 4:	//otur
				_CTest.CommandSit(true);
				break;

				case 5:	//kalk
				_CTest.CommandSit(false);
				break;

				case 6:	//takip
				if( lParam )
				{
					CCharBase pTmp = _CArea.GetUPCBase(lParam);
					if( pTmp.GetBase() > 0 )
					{
						_CArea.SelectObject(pTmp);
						frmBot->chfollowselected->Checked = true;
						frmBot->chfollowselectedClick(NULL);
					}
				}
				break;

				case 7:	//exit
				//frmBot->btnexitClick(NULL);
				CloseBot(false);
				TerminateProcess((void*)-1,0);
				break;

				case 8:	//pt baskani check
				frmBot->btnpopup6->Checked = (bool)lParam;
				frmBot->bAttackWithLeader = (bool)lParam;
				break;

				case 9:	//target recv
				frmBot->wLastTarget = (WORD)lParam;
				break;

				case 10:	//pt baskani takip
				frmBot->chfollowpt->Checked = (bool)lParam;
				frmBot->bFollowLeader = (bool)lParam;
				if( (bool)lParam )
				{
					frmBot->chfollowselected->Checked = false;
					frmBot->bFollowSelected = false;
				}
				break;

				case 11:
				RequestICParty( (int)lParam );
				frmBot->chotopt->Checked = true;
				frmBot->bAutoParty = true;
				break;

				case 12:
				AddMob((int)lParam);
				break;

				case 13:
				WearNoobItem();
				break;

				case 14:
				frmBot->chfollowpt->Checked = false;
				frmBot->chfollowselected->Checked = false;
				frmBot->tmfollow->Enabled = false;
				frmBot->bFollowLeader = false;
				frmBot->bFollowSelected = false;
				ExpandToSlot( (int)lParam );
				break;

				case 15:
				Suicide();
				break;

				case 16:
				SaveSettings(false);
				break;

				case 17:
				CloseBot(false);
				TerminateProcess((void*)-1,0);
				break;

				case 18:
				GoToPlayer( (int)lParam );
				break;

				case 19:
				if(frmBot->bFollowLeader)
					RecvStickSH( lParam );
				break;

				case 20:
				dlgInterClient->bLockClick = true;
				dlgInterClient->chstick->Checked = (bool)lParam;
				dlgInterClient->bLockClick = false;
				frmBot->bStickSH = (bool)lParam;
				break;

				case 21:
				LoadICRoutes();
				break;

			}
			break;
		}

		default:
		break;
	}

	return CallWindowProcA((FARPROC)frmBot->dwGameWndProc,hwnd,uMsg,wParam,lParam);

}

void __fastcall TfrmBot::btnpopup3Click(TObject *Sender)
{
	this->btnstartClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnpopup4Click(TObject *Sender)
{
	StopBot();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnpopup5Click(TObject *Sender)
{
	if( MessageBox(this->Handle,"Oyunu kapat?","Kapat",MB_ICONINFORMATION | MB_YESNO) == IDNO )
		return;

	CloseBot(false);
	TerminateProcess((void*)-1,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnpopup1Click(TObject *Sender)
{
	this->btntrayClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnpopup2Click(TObject *Sender)
{
	if(this->Visible == false)
		this->Show();

	this->WindowState = wsNormal;
	this->BringToFront();
}
//---------------------------------------------------------------------------

void SendTPTPacket(){
	/*
	char pSend[64];	memset(pSend,0,64);
	char* tmpStr = "Windows Live Messenger";
	int tmpLen = strlen(tmpStr);

	*pSend = 0x72;
	*(WORD*)(pSend + 1) = tmpLen;
	memcpy(LPVOID(pSend + 3),tmpStr,tmpLen);
	_CPacket.PSend(pSend,tmpLen + 3);
	*/

	char pSend[4];
	*pSend = 0x72;
	*(WORD*)(pSend + 1) = 0;
	*(pSend + 3) = 0;
	_CPacket.PSend(pSend,4);
}

void __fastcall TfrmBot::btnhidebotClick(TObject *Sender)
{

	if(chgamemenu->Checked == false)
	{
		chgamemenu->Checked = true;
		this->chgamemenuClick(NULL);
    }

	this->Hide();
	if(dlgParty->Visible == true)
		dlgParty->Hide();
	if(dlgAddition->Visible == true)
		dlgAddition->Hide();
	if(dlgCZ->Visible == true)
		dlgCZ->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnmenubdwClick(TObject *Sender)
{
	dlgAddition->Show();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chnotradeClick(TObject *Sender)
{
	bNoTrade = chnotrade->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chnopartyClick(TObject *Sender)
{
	bNoParty = chnoparty->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chnocombatClick(TObject *Sender)
{
	bNoCombat = chnocombat->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chgamemenuClick(TObject *Sender)
{
	if(chgamemenu->Checked == true)
	{
		::SetMenu(hGameHWND,hMainMenu);
		DrawMenuBar(hGameHWND);
	}
	else
	{
		::SetMenu(hGameHWND,NULL);
		DrawMenuBar(hGameHWND);
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnmenuczClick(TObject *Sender)
{
	dlgCZ->Show();
}
//---------------------------------------------------------------------------

void __stdcall HookedSendFunction(BYTE* pBuf, size_t szBuf)
{
	if(!szBuf || !pBuf)
		return;

	DWORD dwRetAddr = 0;
	__asm
	{
		PUSH [EBP+4]
		POP dwRetAddr
	}

	if( CheckRemoteSend(dwRetAddr) == true)
		return;

	EnterCriticalSection(&csSendFunc);

	//if( GetCurrentThreadId() != *(DWORD*)KO_THRD )
	//	return;

	// TPT DISABLE TEST////////
	/*
	if(*pBuf == 0x72 && szBuf >= 4){
		*(DWORD*)pBuf = 0x00000072;
		szBuf = 4;
	}
	*/
	////////////////////////////

	if( *pBuf == 0x04 )
	{
		memset(strTrialID,0,22);
		int iIDLen = *(WORD*)(pBuf + 1);
		memcpy(strTrialID,(LPVOID)(pBuf + 3),iIDLen);
		StrEncrypt(&strTrialID[0],0);
	}

	//if( *pBuf == 0x0D )
	//	return;

	LPVOID Buffer = pBuf;
	int Size = szBuf;
	DWORD dwCallAddr = _CPacket.dwGameFunction;

	__asm
	{
		MOV ECX,[KO_SOCK]
		PUSH Size
		PUSH Buffer
		MOV EAX,dwCallAddr
		CALL EAX
	}
	LeaveCriticalSection(&csSendFunc);
}

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

DWORD __stdcall VirtualGetModuleFileNameA(HMODULE hModule,LPTSTR lpFilename,DWORD nSize)
{
	/*
	HMODULE hOwnModule = NULL;
	bOwnCallHandleExA = true;
	GetModuleHandleEx(NULL,"kojdbot.dll",&hOwnModule);
	bOwnCallHandleExA = false;

	char strPath[256];	   memset(strPath,0,256);
	GetCurrentDirectory(255,strPath);
	strcat(strPath,"\\");
	strcat(strPath,"mss32.dll");
	int iPathLen = strlen(strPath);

	char strOwnPath[256]; memset(strOwnPath,0,256);
	GetCurrentDirectory(255,strOwnPath);
	strcat(strOwnPath,"\\");
	strcat(strOwnPath,"kojdbot.dll");
	int iPathLen2 = strlen(strOwnPath);

	if(hModule == hOwnModule)
	{
		memcpy(lpFilename,strPath,iPathLen);
		return iPathLen;
	}
	*/

	int iRet = GetModuleFileNameA(hModule,lpFilename,nSize);
	if( strstr(lpFilename,"kojdbot.dll") > NULL )
	{
		*lpFilename = 0x00;
		return 0;
	}

	return iRet;
}

HMODULE __stdcall VirtualGetModuleHandleA(LPCTSTR lpModuleName)
{
	if( strstr(lpModuleName,"kojdbot.dll") > NULL )
	{
		return NULL;
	}

	return GetModuleHandleA(lpModuleName);
}

bool __stdcall VirtualGetModuleHandleExA(DWORD dwFlags,LPCTSTR lpModuleName,HMODULE *phModule)
{
	if( strstr(lpModuleName,"kojdbot.dll") > NULL )
	{
		*phModule = NULL;
		return false;
	}

	return GetModuleHandleExA(NULL,lpModuleName,phModule);
}

void PatchGameAPIs()
{
	*(DWORD*)KO_API1 = (DWORD)&VirtualGetModuleFileNameA;
	*(DWORD*)KO_API2 = (DWORD)&VirtualGetModuleHandleA;
	*(DWORD*)KO_API3 = (DWORD)&VirtualGetProcAddress;
	*(DWORD*)KO_API4 = (DWORD)&VirtualGetWindowPID;
	*(DWORD*)KO_API5 = (DWORD)&VirtualGetWindowTextA;
	*(DWORD*)KO_API6 = (DWORD)&VirtualLoadLibrary;
	*(DWORD*)KO_API7 = (DWORD)&VirtualCreateFileA;
}

FARPROC __stdcall VirtualGetProcAddress(HMODULE hModule,LPCSTR lpProcName)
{
	if( !strcmp(lpProcName,"GetModuleFileName") || !strcmp(lpProcName,"GetModuleFileNameA") )
	{
		return (FARPROC)&VirtualGetModuleFileNameA;
	}
	else if( !strcmp(lpProcName,"GetModuleHandle") || !strcmp(lpProcName,"GetModuleHandleA")  )
	{
		return (FARPROC)&VirtualGetModuleHandleA;
	}
	else if( !strcmp(lpProcName,"GetModuleHandleEx") || !strcmp(lpProcName,"GetModuleHandleExA") )
	{
		return (FARPROC)&VirtualGetModuleHandleExA;
	}
	else if( !strcmp(lpProcName,"GetWindowThreadProcessId") )
	{
		return (FARPROC)&VirtualGetWindowPID;
	}
	else if( !strcmp(lpProcName,"GetWindowText") || !strcmp(lpProcName,"GetWindowTextA") )
	{
		return (FARPROC)&VirtualGetWindowTextA;
	}
	else if( !strcmp(lpProcName,"NtQueryInformationProcess") || !strcmp(lpProcName,"ZwQueryInformationProcess") )
	{
		return (FARPROC)&VirtualNtQueryInformationProcess;
	}
	else if( !strcmp(lpProcName,"NtQuerySystemInformation") || !strcmp(lpProcName,"ZwQuerySystemInformation") )
	{
		return (FARPROC)&VirtualNtQuerySystemInformation;
	}
	else if( !strcmp(lpProcName,"VirtualQuery") )
	{
		return (FARPROC)&VirtualVirtualQuery;
	}
	else if( !strcmp(lpProcName,"VirtualQueryEx") )
	{
		return (FARPROC)&VirtualVirtualQueryEx;
	}
	else if( !strcmp(lpProcName,"GetNativeSystemInfo") )
	{
		return (FARPROC)&VirtualGetNativeSystemInfo;
	}
	else if( !strcmp(lpProcName,"CreateFile") || !strcmp(lpProcName,"CreateFileA") )
	{
		return (FARPROC)&VirtualCreateFileA;
	}

	return GetProcAddress(hModule,lpProcName);
}

DWORD __stdcall VirtualGetWindowPID(HWND hWnd,LPDWORD lpdwProcessId)
{
	/*
	char title[256];	memset(title,0,256);
	GetWindowTextA(hWnd,title,255);

	if( strlen(title) > 0 )
	{
		if(	strstr(title,_CCharBase.GetName()) > 0	||
			strstr(title,"Cheat") > 0				||
			strstr(title,"Olly") > 0				||
			strstr(title,"bot") > 0					||
			strstr(title,"dbg") > 0        			||
			strstr(title,"hack") > 0                ||
			strstr(title,"debug") > 0
		)
		{
			*lpdwProcessId = NULL;
			return NULL;
		}
	}
	*/

	return GetWindowThreadProcessId(hWnd,lpdwProcessId);
}

int __stdcall VirtualGetWindowTextA(HWND hWnd,LPTSTR lpString,int nMaxCount)
{
	int iRet = GetWindowTextA(hWnd,lpString,nMaxCount);
	/*
	if( hWnd != hGameHWND && iRet )
	{
		if(	strstr(lpString,_CCharBase.GetName()) > 0	||
			strstr(lpString,"Cheat") > 0				||
			strstr(lpString,"Olly") > 0					||
			strstr(lpString,"bot") > 0					||
			strstr(lpString,"dbg") > 0                 	||
			strstr(lpString,"hack") > 0         		||
			strstr(lpString,"debug") > 0
		)
		{
			*lpString = 0x00;
			strcpy(lpString,"Notepad");
			iRet = strlen(lpString) + 1;
		}
	}
	*/

	return iRet;
}

void LoginFunction()
{
	//WaitForInputIdle((void*)-1,INFINITE);

	long lTick = GetTickCount();
	DWORD dwFunc = 0;

	while(*(DWORD*)KO_LGN1 == 0)
	{
		if(GetTickCount() - lTick >= 45000)
		{
			//SetAutoLogin(0,1);
			__asm
			{
				ADD ESP,4
				ADD ESP,-8
			}
			dwFunc = (DWORD)&SetAutoLogin;
			__asm
			{
				//MOV EAX,[EBP]
				//MOV [ESP],EAX
				ADD ESP,-8
				PUSH dwFunc
				POP EAX
				PUSH [ESP+8]
				POP [ESP]
				XOR EAX,EAX
				OR EAX,1
				XCHG [ESP+4],EAX
				AND EAX,0
				XCHG [ESP],EAX
				CALL EAX
				ADD ESP,8
				ADD ESP,-4
			}//
			return;
		}
		//Sleep(1000);
		__asm
		{
			ADD ESP,4
			SUB ESP,8
		}
		dwFunc = (DWORD)&Sleep;
		__asm
		{
			ADD ESP,-4
			PUSH dwFunc
			POP EAX
			PUSH [ESP+4]
			POP [ESP]
			MOV EAX,500
			ADD EAX,500
			XCHG [ESP],EAX
			CALL EAX
			ADD ESP,-2
			ADD ESP,6
		}//
	}

	//Sleep(1000);
	__asm
	{
		ADD ESP,15
		ADD ESP,-11
		SUB ESP,8
	}
	dwFunc = (DWORD)&Sleep;
	__asm
	{
		ADD ESP,-4
		PUSH dwFunc
		POP EAX
		PUSH [ESP+4]
		POP [ESP]
		MOV EAX,5000
		ADD EAX,-4000
		XCHG [ESP],EAX
		CALL EAX
		ADD ESP,1994
		ADD ESP,-1990
	}//

	while(*(DWORD*)KO_LGN2 == 0)
	{
		//Sleep(1000);
		__asm
		{
			ADD ESP,-16
			POP EAX
			POP EAX
			POP EAX
		}
		dwFunc = (DWORD)&Sleep;
		__asm
		{
			PUSH 1
			POP EAX
			SHL EAX,2
			SUB ESP,EAX
			PUSH dwFunc
			POP EAX
			PUSH [ESP+4]
			POP [ESP]
			MOV EAX,1500
			ADD EAX,-500
			XCHG [ESP],EAX
			CALL EAX
			PUSH ESP
			POP EAX
			POP EAX
		}//
	}

	DWORD dwTmp = *(DWORD*)KO_LGN2;
	dwTmp = *(DWORD*)(dwTmp + 0xE0); //base

	while(*(DWORD*)dwTmp == 0)
	{
		//Sleep(1000);
		__asm
		{
			XOR EAX,EAX
			OR EAX,1
			SHL EAX,4
			SUB ESP,EAX
			POP EAX
			POP EAX
			PUSH ECX
			POP ECX
			POP EAX
		}
		dwFunc = (DWORD)&Sleep;
		__asm
		{
			PUSH EAX
			SUB EAX,[ESP]
			ADD ESP,4
			OR EAX,1
			SHL EAX,3
			SUB ESP,EAX
			ADD ESP,4
			PUSH dwFunc
			POP EAX
			PUSH [ESP+4]
			POP [ESP]
			PUSH EAX
			SUB EAX,[ESP]
			ADD ESP,4
			OR EAX,1
			ADD EAX,999
			XCHG [ESP],EAX
			CALL EAX
			SUB ESP,4
			POP EAX
			POP EAX
		}//
	}

	// ID
	DWORD dwIDText = *(DWORD*)(dwTmp + 0xE0);
	DWORD dwTextAddrBase1 = dwIDText + 0x100;

	char *strID = new char[32];	memset(strID,0,32);
	strcpy(strID,strGameID);

	*(DWORD*)(dwTextAddrBase1)			= 0;
	*(DWORD*)(dwTextAddrBase1 + 4) 		= (DWORD)strID;
	*(DWORD*)(dwTextAddrBase1 + 8) 		= strlen(strID);
	*(DWORD*)(dwTextAddrBase1 + 12) 	= 0x1F;

	__asm
	{
		PUSH 3
		PUSH 1
		POP EAX
		ADD EAX,[ESP]
		ADD ESP,4
		SHL EAX,0
		SUB ESP,EAX
	}
	dwFunc = (DWORD)&Sleep;
	__asm
	{
		XOR EAX,EAX
		PUSH [ESP]
		PUSH EAX
		ADD [ESP],10
		CALL [ESP+4]
		POP EAX
		POP EAX
	}//

	// PW
	DWORD dwPWText = *(DWORD*)(dwTmp + 0xE4);
	DWORD dwTextAddrBase2 = dwPWText + 0xF0;

	char *strPW = new char[32];	memset(strPW,0,32);
	strcpy(strPW,strGamePW);
	StrDecrypt(strPW);

	*(DWORD*)(dwTextAddrBase2)			= 0;
	*(DWORD*)(dwTextAddrBase2 + 4) 		= (DWORD)strPW;
	*(DWORD*)(dwTextAddrBase2 + 8) 		= strlen(strPW);
	*(DWORD*)(dwTextAddrBase2 + 12) 	= 0x1F;

	//Sleep(500);
	__asm
	{
		PUSH 1
		PUSH 0
		POP EAX
		ADD EAX,[ESP]
		ADD ESP,4
		SHL EAX,3
		SHR EAX,1
		SHL EAX,0
		SUB ESP,EAX
	}
	dwFunc = (DWORD)&Sleep;
	__asm
	{
		PUSH 0
		POP EAX
		OR EAX,1
		SHL EAX,9
		ADD EAX,-12
		PUSH [ESP]
		PUSH EAX
		CALL [ESP+4]
		POP EAX
		POP EAX
	}//

	// LOGIN
	__asm
	{
	/*
		MOV ECX,dwIDText
		PUSH 0x407190
		PUSH 0x1000
		PUSH ECX
		MOV ECX,[KO_LGN2]
		MOV ECX,[ECX + 0xDC]
		MOV EAX,0x88A420
		CALL EAX
	*/

		//MOV ECX,[KO_LGN3]
		//MOV EAX,KO_LGN4
		//CALL EAX
		PUSH KO_LGN3
		PUSH KO_LGN4
		POP ECX
		PUSH 0xFFFFFFFF
		PUSH EAX
		SUB EAX,[ESP]
		PUSH [ESP+4]
		POP EAX
		ADD ESP,8
		AND EAX,1
		SHL EAX,2
		SUB ESP,EAX
		AND EAX,0
		MOV EAX,[ESP+4]
		MOV EAX,[EAX]
		MOV [ESP],EAX
		XCHG [ESP],ECX
		CALL [ESP]
		POP EAX
		POP EAX
		//

		//PUSH 500
		//CALL Sleep
		PUSH 1
		PUSH 0
		POP EAX
		ADD EAX,[ESP]
		ADD ESP,4
		SHL EAX,3
		SHR EAX,1
		SHL EAX,0
		SUB ESP,EAX
	}
	dwFunc = (DWORD)&Sleep;
	__asm
	{
		MOV EAX,0xFFFFFFFF
		AND EAX,1
		ADD EAX,-1
		OR EAX,1
		SHL EAX,8
		SHL EAX,2
		SHR EAX,1
		ADD EAX,88
		ADD EAX,-100
		PUSH [ESP]
		PUSH -1
		PUSH EAX
		CALL [ESP+8]
		POP EAX
		POP EAX
		POP EAX
		//

		//MOV ECX,[KO_LGN3]
		//MOV EAX,KO_LGN5
		//CALL EAX
		PUSH KO_LGN3
		PUSH KO_LGN5
		POP ECX
		PUSH 0xFFFFFFFF
		PUSH EAX
		SUB EAX,[ESP]
		PUSH [ESP+4]
		POP EAX
		ADD ESP,8
		AND EAX,1
		SHL EAX,2
		SUB ESP,EAX
		AND EAX,0
		MOV EAX,[ESP+4]
		MOV EAX,[EAX]
		MOV [ESP],EAX
		XCHG [ESP],ECX
		CALL [ESP]
		POP EAX
		POP EAX
		//
	}

	lTick = GetTickCount();

	//Sleep(500);
	__asm
	{
		PUSH 1
		PUSH 0
		POP EAX
		ADD EAX,[ESP]
		ADD ESP,4
		SHL EAX,3
		SHR EAX,1
		SHL EAX,0
		SUB ESP,EAX
	}
	dwFunc = (DWORD)&Sleep;
	__asm
	{
		PUSH 0
		POP EAX
		OR EAX,1
		SHL EAX,9
		ADD EAX,-12
		PUSH [ESP]
		PUSH EAX
		CALL [ESP+4]
		POP EAX
		POP EAX
	}//

	while(*(DWORD*)KO_LGN17 == 0)
	{
		if(GetTickCount() - lTick >= 10000)
		{
			//SetAutoLogin(0,1);
			__asm
			{
				PUSH 4
				MOV EAX,[ESP]
				ADD ESP,4
				SHL EAX,4
				ADD EAX,-60
				SUB ESP,EAX
			}
			dwFunc = (DWORD)&SetAutoLogin;
			__asm
			{
				ADD ESP,-8
				PUSH dwFunc
				POP EAX
				PUSH [ESP+8]
				POP [ESP]
				XOR EAX,EAX
				OR EAX,1
				XCHG [ESP+4],EAX
				AND EAX,0
				XCHG [ESP],EAX
				CALL EAX
            	POP EAX
			}//
			return;
		}
		//Sleep(500);
		__asm
		{
			PUSH 1
			XOR EAX,EAX
			ADD EAX,[ESP]
			ADD ESP,4
			SHL EAX,3
			SHR EAX,1
			SHL EAX,0
			SUB ESP,EAX
		}
		dwFunc = (DWORD)&Sleep;
		__asm
		{
			PUSH 0
			POP EAX
			OR EAX,1
			SHL EAX,9
			ADD EAX,-12
			PUSH [ESP]
			PUSH EAX
			CALL [ESP+4]
			POP EAX
			POP EAX
		}//
	}

	//delete [] strID;
	//memset(strPW,0,strlen(strPW));
	//delete [] strPW;

	//Sleep(500);
	__asm
	{
		PUSH 1
		XOR EAX,EAX
		ADD EAX,[ESP]
		ADD ESP,4
		SHL EAX,3
		SHR EAX,1
		SHL EAX,0
		SUB ESP,EAX
	}
	dwFunc = (DWORD)&Sleep;
	__asm
	{
		PUSH 0
		POP EAX
		OR EAX,1
		SHL EAX,9
		ADD EAX,-12
		PUSH [ESP]
		PUSH EAX
		CALL [ESP+4]
		POP EAX
		POP EAX
	}//

	// SERVER LOGIN
	DWORD dwNoticeOKButton = *(DWORD*)(dwTmp + 0x18C);
	int _iServerIndex = iServerIndex;
	int _iServerNo = iServerNo;
	__asm
	{
		//PASS NOTICE

		//MOV ECX,dwNoticeOKButton
		//MOV EDX,[ECX]
		//PUSH 0
		//CALL [EDX + 0x48]
		PUSH dwNoticeOKButton
		POP ECX
		PUSH 0xFFFFFFFF
		AND DWORD PTR SS:[ESP],0
		MOV EDX,[ECX]
		XOR EAX,EAX
		PUSH 0
		ADD [ESP],3
		SHL [ESP],5
		ADD [ESP],-24
		POP EAX
		CALL [EDX+EAX]
		//

		//MOV ECX,[KO_LGN2]
		//MOV ECX,[ECX + 0xDC]
		//MOV EAX,KO_LGN6
		//CALL EAX
		PUSH KO_LGN2
		POP EAX
		MOV EAX,[EAX]
		PUSH KO_LGN6
		XCHG [ESP],EAX
		PUSH EAX
		XOR EAX,EAX
		OR EAX,1
		SHL EAX,8
		ADD EAX,-32
		ADD [ESP+4],EAX
		POP EAX
		POP ECX
		MOV ECX,[ECX]
		CALL EAX
		//

		//PUSH 500
		//CALL Sleep
		PUSH 1
		XOR EAX,EAX
		ADD EAX,[ESP]
		ADD ESP,4
		SHL EAX,3
		SHR EAX,1
		SHL EAX,0
		SUB ESP,EAX
	}
	dwFunc = (DWORD)&Sleep;
	__asm
	{
		PUSH 0
		POP EAX
		OR EAX,1
		SHL EAX,9
		ADD EAX,-12
		PUSH [ESP]
		PUSH EAX
		CALL [ESP+4]
		POP EAX
		POP EAX
		//

		//SELECT SERVER
		//MOV ESI,_iServerIndex	//11-girakon
		//MOV ECX,[KO_LGN2]
		//MOV ECX,[ECX + 0xDC]
		//PUSH ESI
		//MOV EAX,KO_LGN7
		//CALL EAX
		MOV ESI,_iServerIndex	//11-girakon
		PUSH KO_LGN2
		MOV EAX,[ESP]
		PUSH 4
		POP ECX
		ADD ESP,ECX
		PUSH [EAX]
		POP ECX
		XOR EAX,EAX
		ADD EAX,4
		SUB ESP,EAX
		XOR EAX,EAX
		PUSH 5
		POP EAX
		SHL EAX,5
		MOV [ESP],EAX
		ADD [ESP],64
		POP EAX
		PUSH [ECX+EAX]
		POP ECX
		PUSH ESI
		PUSH KO_LGN7
		MOV EAX,[ESP]
		ADD ESP,4
		CALL EAX
		//

		//MOV ECX,[KO_LGN2]
		//MOV ECX,[ECX + 0xDC]
		//MOV EAX,KO_LGN8
		//CALL EAX
		PUSH KO_LGN2
		MOV EAX,[ESP]
		PUSH 4
		POP ECX
		ADD ESP,ECX
		PUSH [EAX]
		POP ECX
		XOR EAX,EAX
		ADD EAX,4
		SUB ESP,EAX
		XOR EAX,EAX
		PUSH 5
		POP EAX
		SHL EAX,5
		MOV [ESP],EAX
		ADD [ESP],64
		POP EAX
		PUSH [ECX+EAX]
		POP ECX
		PUSH KO_LGN8
		CALL [ESP]
		XOR EAX,EAX
		ADD EAX,4
		ADD ESP,EAX
		//

		//LEA EAX,[ESI+ESI*8]
		//LEA ECX,[ESI+EAX*2]
		//MOV ESI,EBP
		//MOV EBP,[KO_LGN2]
		//MOV EBP,[EBP + 0xDC]
		//LEA EAX,DWORD PTR SS:[EBP+ECX*4]
		//MOV EBP,ESI
		LEA EAX,[ESI+ESI*8]
		LEA ECX,[ESI+EAX*2]
		PUSH EBP
		POP ESI
		PUSH KO_LGN2
		MOV EBP,[ESP]
		ADD ESP,4
		PUSH [EBP]
		POP EBP
		XOR EAX,EAX
		ADD EAX,5
		SHL EAX,5
		ADD EAX,64
		ADD EBP,EAX
		PUSH [EBP]
		POP EBP
		LEA EAX,DWORD PTR SS:[EBP+ECX*4]
		PUSH ESI
		POP EBP
		//

		//SUB ESP,0x4C
		//LEA ESI,[EAX+0x1C4]
		//MOV ECX,0x13
		//MOV EDI,ESP
		//REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
		//MOV ECX,[KO_LGN2]
		//MOV ECX,[ECX + 0xDC]
		//MOV EAX,KO_LGN9
		//CALL EAX
		XOR ESI,ESI
		PUSH 3
		POP ESI
		SHL ESI,5
		ADD ESI,-20
		SUB ESP,ESI
		LEA ESI,[EAX+400]
		PUSH 56
		ADD ESI,[ESP]
		ADD ESP,4
		PUSH 0x13
		POP ECX
		MOV EDI,ESP
		REP MOVS DWORD PTR ES:[EDI],DWORD PTR DS:[ESI]
		PUSH KO_LGN2
		POP ECX
		PUSH [ECX]
		POP ECX
		PUSH 5
		SHL [ESP],5
		ADD [ESP],64
		POP EAX
		MOV ECX,[ECX+EAX]
		PUSH KO_LGN9
		POP EAX
		CALL EAX
		//

		//PUSH 500
		//CALL Sleep
		PUSH 1
		XOR EAX,EAX
		ADD EAX,[ESP]
		ADD ESP,4
		SHL EAX,3
		SHR EAX,1
		SHL EAX,0
		SUB ESP,EAX
	}
	dwFunc = (DWORD)&Sleep;
	__asm
	{
		PUSH 0
		POP EAX
		OR EAX,1
		SHL EAX,9
		ADD EAX,-12
		PUSH [ESP]
		PUSH EAX
		CALL [ESP+4]
		POP EAX
		POP EAX
		//

		//MOV ECX,[KO_LGN2]
		//MOV ECX,[ECX + 0xDC]
		//PUSH _iServerNo	//second server (girakon2)
		//MOV EAX,KO_LGN10
		//CALL EAX
		PUSH _iServerNo
		PUSH 256
		PUSH KO_LGN2
		POP EAX
		MOV ECX,[EAX]
		SUB [ESP],37
		ADD [ESP],5
		POP EAX
		MOV ECX,[ECX+EAX]
		POP EAX
		PUSH KO_LGN10
		PUSH EAX
		CALL [ESP+4]
		ADD ESP,4

		//PUSH 500
		//CALL Sleep
		PUSH 1
		XOR EAX,EAX
		ADD EAX,[ESP]
		ADD ESP,4
		SHL EAX,3
		SHR EAX,1
		SHL EAX,0
		SUB ESP,EAX
	}
	dwFunc = (DWORD)&Sleep;
	__asm
	{
		PUSH 0
		POP EAX
		OR EAX,1
		SHL EAX,9
		ADD EAX,-12
		PUSH [ESP]
		PUSH EAX
		CALL [ESP+4]
		POP EAX
		POP EAX
		//

		//CONNECT SERVER
		//MOV [KO_LGN11],1
		//MOV ECX,[KO_LGN3]
		//MOV EAX,KO_LGN12
		//CALL EAX
		PUSH KO_LGN11
		POP EAX
		MOV [EAX],0
		INC [EAX]
		PUSH KO_LGN3
		PUSH KO_LGN12
		POP ECX
		PUSH 0xFFFFFFFF
		PUSH EAX
		SUB EAX,[ESP]
		PUSH [ESP+4]
		POP EAX
		ADD ESP,8
		AND EAX,1
		SHL EAX,2
		SUB ESP,EAX
		AND EAX,0
		MOV EAX,[ESP+4]
		MOV EAX,[EAX]
		MOV [ESP],EAX
		XCHG [ESP],ECX
		CALL [ESP]
		POP EAX
		POP EAX
		//
	}

	lTick = GetTickCount();

	//Sleep(1000);
	__asm
	{
		ADD ESP,15
		ADD ESP,-11
		SUB ESP,8
	}
	dwFunc = (DWORD)&Sleep;
	__asm
	{
		ADD ESP,-4
		PUSH dwFunc
		POP EAX
		PUSH [ESP+4]
		POP [ESP]
		MOV EAX,5000
		ADD EAX,-4000
		XCHG [ESP],EAX
		CALL EAX
		ADD ESP,1994
		ADD ESP,-1990
	}//

	while( CheckIntro() == false )
	{
		if(GetTickCount() - lTick >= 10000)
		{
			//SetAutoLogin(0,1);
			__asm
			{
				PUSH 4
				MOV EAX,[ESP]
				ADD ESP,4
				SHL EAX,4
				ADD EAX,-60
				SUB ESP,EAX
			}
			dwFunc = (DWORD)&SetAutoLogin;
			__asm
			{
				ADD ESP,-8
				PUSH dwFunc
				POP EAX
				PUSH [ESP+8]
				POP [ESP]
				XOR EAX,EAX
				OR EAX,1
				XCHG [ESP+4],EAX
				AND EAX,0
				XCHG [ESP],EAX
				CALL EAX
            	POP EAX
			}//
			return;
		}
		//Sleep(1000);
		__asm
		{
			ADD ESP,15
			ADD ESP,-11
			SUB ESP,8
		}
		dwFunc = (DWORD)&Sleep;
		__asm
		{
			ADD ESP,-4
			PUSH dwFunc
			POP EAX
			PUSH [ESP+4]
			POP [ESP]
			MOV EAX,5000
			ADD EAX,-4000
			XCHG [ESP],EAX
			CALL EAX
			ADD ESP,1994
			ADD ESP,-1990
		}//
	}

	//Sleep(7000);
	__asm
	{
		ADD ESP,15
		ADD ESP,-11
		SUB ESP,8
	}
	dwFunc = (DWORD)&Sleep;
	__asm
	{
		ADD ESP,-4
		PUSH dwFunc
		POP EAX
		PUSH [ESP+4]
		POP [ESP]
		MOV EAX,10000
		ADD EAX,-3000
		XCHG [ESP],EAX
		CALL EAX
		ADD ESP,1994
		ADD ESP,-1990
	}//

	DWORD dwTmpUi = 0;
	DWORD dwUiVisible = 0;
	lTick = GetTickCount();
	while(1)
	{
		dwTmpUi = *(DWORD*)KO_LGN2;
		if(dwTmpUi)
		{
			dwTmpUi = *(DWORD*)(dwTmpUi + 0x88);
			if(dwTmpUi)
			{
				dwTmpUi = *(DWORD*)dwTmpUi;
				if(dwTmpUi)
				{
					dwTmpUi = *(DWORD*)(dwTmpUi + 0x8);
					if(dwTmpUi)
					{
						dwUiVisible = *(DWORD*)(dwTmpUi + 0xC0);
						if(dwUiVisible)
						{
							break;
						}
					}
				}
			}
		}

		if(GetTickCount() - lTick >= 5000)
		{
			//SetAutoLogin(0,1);
			__asm
			{
				PUSH 4
				MOV EAX,[ESP]
				ADD ESP,4
				SHL EAX,4
				ADD EAX,-60
				SUB ESP,EAX
			}
			dwFunc = (DWORD)&SetAutoLogin;
			__asm
			{
				ADD ESP,-8
				PUSH dwFunc
				POP EAX
				PUSH [ESP+8]
				POP [ESP]
				XOR EAX,EAX
				OR EAX,1
				XCHG [ESP+4],EAX
				AND EAX,0
				XCHG [ESP],EAX
				CALL EAX
            	POP EAX
			}//
			return;
		}

		//Sleep(1000);
		__asm
		{
			ADD ESP,15
			ADD ESP,-11
			SUB ESP,8
		}
		dwFunc = (DWORD)&Sleep;
		__asm
		{
			ADD ESP,-4
			PUSH dwFunc
			POP EAX
			PUSH [ESP+4]
			POP [ESP]
			MOV EAX,5000
			ADD EAX,-4000
			XCHG [ESP],EAX
			CALL EAX
			ADD ESP,1994
			ADD ESP,-1990
		}//
	}

	if(iCharSlot > 0)
	{
		if(iCharSlot == 1)
		{
			__asm
			{
				//MOV ECX,[KO_LGN13]
				//MOV EAX,KO_LGN14
				//CALL EAX
				PUSH KO_LGN13
				PUSH KO_LGN14
				POP ECX
				PUSH 0xFFFFFFFF
				PUSH EAX
				SUB EAX,[ESP]
				PUSH [ESP+4]
				POP EAX
				ADD ESP,8
				AND EAX,1
				SHL EAX,2
				SUB ESP,EAX
				AND EAX,0
				MOV EAX,[ESP+4]
				MOV EAX,[EAX]
				MOV [ESP],EAX
				XCHG [ESP],ECX
				CALL [ESP]
				POP EAX
				POP EAX
				//
			}
		}
		else if(iCharSlot == 2)
		{
			__asm
			{
				//MOV ECX,[KO_LGN13]
				//MOV EAX,KO_LGN15
				//CALL EAX
				PUSH KO_LGN13
				PUSH KO_LGN15
				POP ECX
				PUSH 0xFFFFFFFF
				PUSH EAX
				SUB EAX,[ESP]
				PUSH [ESP+4]
				POP EAX
				ADD ESP,8
				AND EAX,1
				SHL EAX,2
				SUB ESP,EAX
				AND EAX,0
				MOV EAX,[ESP+4]
				MOV EAX,[EAX]
				MOV [ESP],EAX
				XCHG [ESP],ECX
				CALL [ESP]
				POP EAX
				POP EAX
				//
			}
		}

		//Sleep(5000);
		__asm
		{
			ADD ESP,15
			ADD ESP,-11
			SUB ESP,8
		}
		dwFunc = (DWORD)&Sleep;
		__asm
		{
			ADD ESP,-4
			PUSH dwFunc
			POP EAX
			PUSH [ESP+4]
			POP [ESP]
			MOV EAX,6000
			ADD EAX,-1000
			XCHG [ESP],EAX
			CALL EAX
			ADD ESP,1994
			ADD ESP,-1990
		}//

		while(1)
		{
			dwUiVisible = *(DWORD*)(dwTmpUi + 0xC0);
			if(dwUiVisible)
				break;
		}
	}

	//Sleep(1000);
	__asm
	{
		ADD ESP,15
		ADD ESP,-11
		SUB ESP,8
	}
	dwFunc = (DWORD)&Sleep;
	__asm
	{
		ADD ESP,-4
		PUSH dwFunc
		POP EAX
		PUSH [ESP+4]
		POP [ESP]
		MOV EAX,5000
		ADD EAX,-4000
		XCHG [ESP],EAX
		CALL EAX
		ADD ESP,1994
		ADD ESP,-1990
	}//

	asm
	{
		//START CHAR
		//MOV ECX,[KO_LGN13]
		//MOV EAX,KO_LGN16
		//CALL EAX
		PUSH KO_LGN13
		PUSH KO_LGN16
		POP ECX
		PUSH 0xFFFFFFFF
		PUSH EAX
		SUB EAX,[ESP]
		PUSH [ESP+4]
		POP EAX
		ADD ESP,8
		AND EAX,1
		SHL EAX,2
		SUB ESP,EAX
		AND EAX,0
		MOV EAX,[ESP+4]
		MOV EAX,[EAX]
		MOV [ESP],EAX
		XCHG [ESP],ECX
		CALL [ESP]
		POP EAX
		POP EAX
		//
	}
}

bool CheckIntro()
{
	DWORD dwTmp = *(DWORD*)KO_LGN13;

	if(dwTmp)
	{
		dwTmp = *(DWORD*)(dwTmp + 0x3B0);

		if(dwTmp)
		{
			dwTmp = *(DWORD*)(dwTmp + 0x98);

			if(dwTmp)
				return true;
        }
	}

	return false;
}

void __fastcall TfrmBot::tmlogincheckTimer(TObject *Sender)
{
	if( _CPacket.GetConnState() == false )
	{
		SetAutoLogin(1,1);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmgocenterTimer(TObject *Sender)
{
	if(dlgMaden->bEnabled)
	{
		tmgocenter->Enabled = false;
		this->btnstartClick(NULL);
		return;
	}

	if(iBotCenterX == 0 || iBotCenterY == 0)
		tmgocenter->Enabled = false;

	if(SHTeleport(iBotCenterX,iBotCenterY,5) == true)
	{
		tmgocenter->Enabled = false;
		this->btnstartClick(NULL);
	}
}
//---------------------------------------------------------------------------


void __stdcall SetAutoLogin(bool bCloseBot, bool bCloseGame)
{
	DeleteFileA(strCheckFile);
	ofstream fStatus;
	fStatus.open(strCheckFile,std::ios_base::out | std::ios_base::app);
	if(!fStatus.is_open())return;

	fStatus << "1\n";
	fStatus.close();

	if(bCloseBot)
		CloseBot(false);
	if(bCloseGame)
		TerminateProcess((void*)-1,0);
}

void UpdateMenu()
{
	ifstream fWindowList;
	fWindowList.open("C://wndlist.kojd");

	if(fWindowList.is_open() == false)
		return;

	DeleteFileA("C://_wndlist.kojd");
	ofstream fNewList;
	fNewList.open("C://_wndlist.kojd",std::ios_base::out | std::ios_base::app);
	bool bWriterOpen = fNewList.is_open();

	char strWindow[25];	memset(strWindow,0,25);
	bool bRet;
	HWND hWindow = NULL;
	int iIndex = 0;
	bool bSkip = false;

	for (int i = 0; i < 8; i++)
	{
		frmBot->hWindowArray[i] = NULL;
	}

	while ( 1 )
	{
		memset(strWindow,0,25);
		bRet = fWindowList.getline(strWindow,24);

		if( !bRet )
			break;

		if( strlen(strWindow) == 0 )
			continue;

		hWindow = FindWindowA(NULL,strWindow);

		if( !hWindow )
			continue;

		bSkip = false;
		for (int i = 0; i < 8; i++)
		{
			if( frmBot->hWindowArray[i] == hWindow )
			{
				bSkip = true;
				break;
            }
		}

		if(bSkip)
        	continue;

		if(bWriterOpen)
			fNewList << strWindow << "\n";

		if( hWindow == hGameHWND )
			continue;

		frmBot->hWindowArray[iIndex] = hWindow;
		ModifyMenu(frmBot->hMainMenu,(iIndex + 3),MF_BYPOSITION | MF_POPUP,(int)frmBot->hPopupArray[iIndex],strWindow);

		iIndex++;
		if(iIndex == 8)
			break;
	}

	for (int i = 0; i < 8; i++)
	{
		if( frmBot->hWindowArray[i] == NULL )
			ModifyMenu(frmBot->hMainMenu,(i + 3),MF_BYPOSITION | MF_DISABLED,NULL,"-");
	}

	DrawMenuBar(hGameHWND);

	fWindowList.close();
	if(bWriterOpen)
		fNewList.close();

	DeleteFileA("C://wndlist.kojd");
	//rename("C://_wndlist.kojd","C://wndlist.kojd");
	CopyFileA("C://_wndlist.kojd","C://wndlist.kojd",false);
	DeleteFileA("C://_wndlist.kojd");
}
void __fastcall TfrmBot::btn_pclient_0_0Click(TObject *Sender)
{
	if(hWindowArray[0] == NULL)
		return;

	SendNotifyMessage(hWindowArray[0],WM_KOLEBOT,0,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_0_1Click(TObject *Sender)
{
	if(hWindowArray[0] == NULL)
		return;

	SendNotifyMessage(hWindowArray[0],WM_KOLEBOT,1,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_0_2Click(TObject *Sender)
{
	if(hWindowArray[0] == NULL)
		return;

	SendNotifyMessage(hWindowArray[0],WM_KOLEBOT,2,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_0_3Click(TObject *Sender)
{
	if(hWindowArray[0] == NULL)
		return;

	SendNotifyMessage(hWindowArray[0],WM_KOLEBOT,3,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_0_4Click(TObject *Sender)
{
	if(hWindowArray[0] == NULL)
		return;

	SendNotifyMessage(hWindowArray[0],WM_KOLEBOT,4,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_0_5Click(TObject *Sender)
{
	if(hWindowArray[0] == NULL)
		return;

	SendNotifyMessage(hWindowArray[0],WM_KOLEBOT,5,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_0_6Click(TObject *Sender)
{
	if(hWindowArray[0] == NULL)
		return;

	SendNotifyMessage(hWindowArray[0],WM_KOLEBOT,6,dwMyID);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_0_7Click(TObject *Sender)
{
	if( MessageBox(this->Handle,"Oyunu kapat?","Kapat",MB_ICONINFORMATION | MB_YESNO) == IDNO )
		return;

	if(hWindowArray[0] == NULL)
		return;

	SendNotifyMessage(hWindowArray[0],WM_KOLEBOT,7,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_1_0Click(TObject *Sender)
{
	if(hWindowArray[1] == NULL)
		return;

	SendNotifyMessage(hWindowArray[1],WM_KOLEBOT,0,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_1_1Click(TObject *Sender)
{
	if(hWindowArray[1] == NULL)
		return;

	SendNotifyMessage(hWindowArray[1],WM_KOLEBOT,1,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_1_2Click(TObject *Sender)
{
	if(hWindowArray[1] == NULL)
		return;

	SendNotifyMessage(hWindowArray[1],WM_KOLEBOT,2,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_1_3Click(TObject *Sender)
{
	if(hWindowArray[1] == NULL)
		return;

	SendNotifyMessage(hWindowArray[1],WM_KOLEBOT,3,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_1_4Click(TObject *Sender)
{
	if(hWindowArray[1] == NULL)
		return;

	SendNotifyMessage(hWindowArray[1],WM_KOLEBOT,4,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_1_5Click(TObject *Sender)
{
	if(hWindowArray[1] == NULL)
		return;

	SendNotifyMessage(hWindowArray[1],WM_KOLEBOT,5,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_1_6Click(TObject *Sender)
{
	if(hWindowArray[1] == NULL)
		return;

	SendNotifyMessage(hWindowArray[1],WM_KOLEBOT,6,dwMyID);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_1_7Click(TObject *Sender)
{
	if( MessageBox(this->Handle,"Oyunu kapat?","Kapat",MB_ICONINFORMATION | MB_YESNO) == IDNO )
		return;

	if(hWindowArray[1] == NULL)
		return;

	SendNotifyMessage(hWindowArray[1],WM_KOLEBOT,7,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_2_0Click(TObject *Sender)
{
	if(hWindowArray[2] == NULL)
		return;

	SendNotifyMessage(hWindowArray[2],WM_KOLEBOT,0,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_2_1Click(TObject *Sender)
{
	if(hWindowArray[2] == NULL)
		return;

	SendNotifyMessage(hWindowArray[2],WM_KOLEBOT,1,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_2_2Click(TObject *Sender)
{
	if(hWindowArray[2] == NULL)
		return;

	SendNotifyMessage(hWindowArray[2],WM_KOLEBOT,2,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_2_3Click(TObject *Sender)
{
	if(hWindowArray[2] == NULL)
		return;

	SendNotifyMessage(hWindowArray[2],WM_KOLEBOT,3,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_2_4Click(TObject *Sender)
{
	if(hWindowArray[2] == NULL)
		return;

	SendNotifyMessage(hWindowArray[2],WM_KOLEBOT,4,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_2_5Click(TObject *Sender)
{
	if(hWindowArray[2] == NULL)
		return;

	SendNotifyMessage(hWindowArray[2],WM_KOLEBOT,5,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_2_6Click(TObject *Sender)
{
	if(hWindowArray[2] == NULL)
		return;

	SendNotifyMessage(hWindowArray[2],WM_KOLEBOT,6,dwMyID);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_2_7Click(TObject *Sender)
{
	if( MessageBox(this->Handle,"Oyunu kapat?","Kapat",MB_ICONINFORMATION | MB_YESNO) == IDNO )
		return;

	if(hWindowArray[2] == NULL)
		return;

	SendNotifyMessage(hWindowArray[2],WM_KOLEBOT,7,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_3_0Click(TObject *Sender)
{
	if(hWindowArray[3] == NULL)
		return;

	SendNotifyMessage(hWindowArray[3],WM_KOLEBOT,0,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_3_1Click(TObject *Sender)
{
	if(hWindowArray[3] == NULL)
		return;

	SendNotifyMessage(hWindowArray[3],WM_KOLEBOT,1,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_3_2Click(TObject *Sender)
{
	if(hWindowArray[3] == NULL)
		return;

	SendNotifyMessage(hWindowArray[3],WM_KOLEBOT,2,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_3_3Click(TObject *Sender)
{
	if(hWindowArray[3] == NULL)
		return;

	SendNotifyMessage(hWindowArray[3],WM_KOLEBOT,3,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_3_4Click(TObject *Sender)
{
	if(hWindowArray[3] == NULL)
		return;

	SendNotifyMessage(hWindowArray[3],WM_KOLEBOT,4,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_3_5Click(TObject *Sender)
{
	if(hWindowArray[3] == NULL)
		return;

	SendNotifyMessage(hWindowArray[3],WM_KOLEBOT,5,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_3_6Click(TObject *Sender)
{
	if(hWindowArray[3] == NULL)
		return;

	SendNotifyMessage(hWindowArray[3],WM_KOLEBOT,6,dwMyID);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_3_7Click(TObject *Sender)
{
	if( MessageBox(this->Handle,"Oyunu kapat?","Kapat",MB_ICONINFORMATION | MB_YESNO) == IDNO )
		return;

	if(hWindowArray[3] == NULL)
		return;

	SendNotifyMessage(hWindowArray[3],WM_KOLEBOT,7,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_4_0Click(TObject *Sender)
{
	if(hWindowArray[4] == NULL)
		return;

	SendNotifyMessage(hWindowArray[4],WM_KOLEBOT,0,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_4_1Click(TObject *Sender)
{
	if(hWindowArray[4] == NULL)
		return;

	SendNotifyMessage(hWindowArray[4],WM_KOLEBOT,1,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_4_2Click(TObject *Sender)
{
	if(hWindowArray[4] == NULL)
		return;

	SendNotifyMessage(hWindowArray[4],WM_KOLEBOT,2,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_4_3Click(TObject *Sender)
{
	if(hWindowArray[4] == NULL)
		return;

	SendNotifyMessage(hWindowArray[4],WM_KOLEBOT,3,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_4_4Click(TObject *Sender)
{
	if(hWindowArray[4] == NULL)
		return;

	SendNotifyMessage(hWindowArray[4],WM_KOLEBOT,4,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_4_5Click(TObject *Sender)
{
	if(hWindowArray[4] == NULL)
		return;

	SendNotifyMessage(hWindowArray[4],WM_KOLEBOT,5,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_4_6Click(TObject *Sender)
{
	if(hWindowArray[4] == NULL)
		return;

	SendNotifyMessage(hWindowArray[4],WM_KOLEBOT,6,dwMyID);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_4_7Click(TObject *Sender)
{
	if( MessageBox(this->Handle,"Oyunu kapat?","Kapat",MB_ICONINFORMATION | MB_YESNO) == IDNO )
		return;

	if(hWindowArray[4] == NULL)
		return;

	SendNotifyMessage(hWindowArray[4],WM_KOLEBOT,7,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_5_0Click(TObject *Sender)
{
	if(hWindowArray[5] == NULL)
		return;

	SendNotifyMessage(hWindowArray[5],WM_KOLEBOT,0,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_5_1Click(TObject *Sender)
{
	if(hWindowArray[5] == NULL)
		return;

	SendNotifyMessage(hWindowArray[5],WM_KOLEBOT,1,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_5_2Click(TObject *Sender)
{
	if(hWindowArray[5] == NULL)
		return;

	SendNotifyMessage(hWindowArray[5],WM_KOLEBOT,2,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_5_3Click(TObject *Sender)
{
	if(hWindowArray[5] == NULL)
		return;

	SendNotifyMessage(hWindowArray[5],WM_KOLEBOT,3,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_5_4Click(TObject *Sender)
{
	if(hWindowArray[5] == NULL)
		return;

	SendNotifyMessage(hWindowArray[5],WM_KOLEBOT,4,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_5_5Click(TObject *Sender)
{
	if(hWindowArray[5] == NULL)
		return;

	SendNotifyMessage(hWindowArray[5],WM_KOLEBOT,5,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_5_6Click(TObject *Sender)
{
	if(hWindowArray[5] == NULL)
		return;

	SendNotifyMessage(hWindowArray[5],WM_KOLEBOT,6,dwMyID);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_5_7Click(TObject *Sender)
{
	if( MessageBox(this->Handle,"Oyunu kapat?","Kapat",MB_ICONINFORMATION | MB_YESNO) == IDNO )
		return;

	if(hWindowArray[5] == NULL)
		return;

	SendNotifyMessage(hWindowArray[5],WM_KOLEBOT,7,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_6_0Click(TObject *Sender)
{
	if(hWindowArray[6] == NULL)
		return;

	SendNotifyMessage(hWindowArray[6],WM_KOLEBOT,0,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_6_1Click(TObject *Sender)
{
	if(hWindowArray[6] == NULL)
		return;

	SendNotifyMessage(hWindowArray[6],WM_KOLEBOT,1,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_6_2Click(TObject *Sender)
{
	if(hWindowArray[6] == NULL)
		return;

	SendNotifyMessage(hWindowArray[6],WM_KOLEBOT,2,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_6_3Click(TObject *Sender)
{
	if(hWindowArray[6] == NULL)
		return;

	SendNotifyMessage(hWindowArray[6],WM_KOLEBOT,3,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_6_4Click(TObject *Sender)
{
	if(hWindowArray[6] == NULL)
		return;

	SendNotifyMessage(hWindowArray[6],WM_KOLEBOT,4,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_6_5Click(TObject *Sender)
{
	if(hWindowArray[6] == NULL)
		return;

	SendNotifyMessage(hWindowArray[6],WM_KOLEBOT,5,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_6_6Click(TObject *Sender)
{
	if(hWindowArray[6] == NULL)
		return;

	SendNotifyMessage(hWindowArray[6],WM_KOLEBOT,6,dwMyID);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_6_7Click(TObject *Sender)
{
	if( MessageBox(this->Handle,"Oyunu kapat?","Kapat",MB_ICONINFORMATION | MB_YESNO) == IDNO )
		return;

	if(hWindowArray[6] == NULL)
		return;

	SendNotifyMessage(hWindowArray[6],WM_KOLEBOT,7,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_7_0Click(TObject *Sender)
{
	if(hWindowArray[7] == NULL)
		return;

	SendNotifyMessage(hWindowArray[7],WM_KOLEBOT,0,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_7_1Click(TObject *Sender)
{
	if(hWindowArray[7] == NULL)
		return;

	SendNotifyMessage(hWindowArray[7],WM_KOLEBOT,1,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_7_2Click(TObject *Sender)
{
	if(hWindowArray[7] == NULL)
		return;

	SendNotifyMessage(hWindowArray[7],WM_KOLEBOT,2,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_7_3Click(TObject *Sender)
{
	if(hWindowArray[7] == NULL)
		return;

	SendNotifyMessage(hWindowArray[7],WM_KOLEBOT,3,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_7_4Click(TObject *Sender)
{
	if(hWindowArray[7] == NULL)
		return;

	SendNotifyMessage(hWindowArray[7],WM_KOLEBOT,4,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_7_5Click(TObject *Sender)
{
	if(hWindowArray[7] == NULL)
		return;

	SendNotifyMessage(hWindowArray[7],WM_KOLEBOT,5,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_7_6Click(TObject *Sender)
{
	if(hWindowArray[7] == NULL)
		return;

	SendNotifyMessage(hWindowArray[7],WM_KOLEBOT,6,dwMyID);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btn_pclient_7_7Click(TObject *Sender)
{
	if( MessageBox(this->Handle,"Oyunu kapat?","Kapat",MB_ICONINFORMATION | MB_YESNO) == IDNO )
		return;

	if(hWindowArray[7] == NULL)
		return;

	SendNotifyMessage(hWindowArray[7],WM_KOLEBOT,7,0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::chinnmpClick(TObject *Sender)
{
	bGetInnMP = chinnmp->Checked;
}
//---------------------------------------------------------------------------


void __fastcall TfrmBot::chinnhpClick(TObject *Sender)
{
	bGetInnHP = chinnhp->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtinnmpChange(TObject *Sender)
{
	iGetInnMP = atoi( txtinnmp->Text.c_str() );
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::txtinnhpChange(TObject *Sender)
{
	iGetInnHP = atoi( txtinnhp->Text.c_str() );
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnpnok2Click(TObject *Sender)
{
	pnpot2->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btninnpotClick(TObject *Sender)
{
	pnpot2->Show();
}
//---------------------------------------------------------------------------

void W7APRTest()
{
	DWORD dwTmp;

	dwTmp = *(DWORD*)KO_PRTY;
	dwTmp = *(DWORD*)dwTmp;
	dwTmp += 0x1C;

	dwW7APRTest_Old = *(DWORD*)dwTmp;
	*(DWORD*)dwTmp = (DWORD)&W7APRTest_Function;
}

void __stdcall W7APRTest_Function(DWORD dwPack, int &i)
{
	BYTE bOldBYTES[6] = {0xA1,0x14,0x8F,0xC2,0x00,0x89};
	BYTE bNewBYTES[6] = {0x68,0xF0,0xB8,0x91,0x00,0xC3};

	LPVOID lpPack = (LPVOID)dwPack;
	DWORD dwOffset = (DWORD)&i;

	memcpy((LPVOID)0x91B7ED,bNewBYTES,6);
	__asm
	{
		MOV EAX,dwW7APRTest_Old
		MOV ECX,[KO_PRTY]
		PUSH dwOffset
		PUSH lpPack
		CALL EAX
	}
	memcpy((LPVOID)0x91B7ED,bOldBYTES,6);

	DWORD dwTmp = *(DWORD*)KO_PRTY;
	dwTmp = *(DWORD*)dwTmp;
	dwTmp += 0x1C;
	*(DWORD*)dwTmp = dwW7APRTest_Old;
}

void LoadInit()
{

	if(bAutoLogin)	//init party req
	{
		char pSend[3];
		*pSend = 0x2F; *(pSend+1) = 0x02; *(pSend+2) = 0x01;
		_CPacket.PSend(pSend,3);
	}

}

bool CheckValidIDPW(char* pStr)
{
	if(!pStr)
		return false;

	int iLen = strlen(pStr);
	if( !iLen )
		return false;

	BYTE bValue;

	for (int i = 0; i < iLen; i++)
	{
		bValue = *(pStr+i);

		if( bValue < 0x30 )
			return false;

		if( bValue > 0x39 )
		{
			if( bValue > 0x60 )
			{
				if( bValue > 0x7A )
					return false;
			}
			else
			{
				if( bValue < 0x41 )
					return false;

				if( bValue > 0x5A )
				{
					if( bValue != 0x5F )
						return false;
				}
			}
		}
	}

	return true;
}

DWORD __stdcall VirtualAPRTest1(void* pArg1, void* pArg2, void* pArg3)
{
	return 0;
}

void __stdcall VirtualAPRTest2()
{
	return;
}

void CheckLoader()
{
	HANDLE hFile = CreateFileA("initloader.kojd",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		DeleteFileA("initloader.kojd");
	}
	else
	{
		/*
		ofstream fOut;
		fOut.open("kojdbot_hata.log",std::ios_base::out | std::ios_base::app);
		if(fOut.is_open() == true)
		{
			fOut << "loader error!\n";
			fOut.close();
		}
		*/

		FatalAppExit(0,"loader init error!");
		QuitBot();
		//TerminateProcess((void*)-1,0);
	}

	HANDLE hFile2 = CreateFileA("_dbg.kojd",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile2 != INVALID_HANDLE_VALUE)
	{
		dwDebuggerFree++;
		CloseHandle(hFile2);
	}
}

void PatchTPT()
{
	//kojd, 09.06.11
	//usko tpt-off patch

	//InitializeCriticalSection(&csAPR);
	//InitializeCriticalSection(&csAPR2);

	HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
	DWORD dwAPIAddr = (DWORD) GetProcAddress(hKernel32,"CreateThread");

	BYTE pJump[] = {0x8B,0xFF,0x68,0,0,0,0,0xC3,0x90};
	*(DWORD*)(pJump + 3) = (DWORD) &VirtualCT;
	WriteProcessMemory((void*)-1,(LPVOID)dwAPIAddr,pJump,sizeof(pJump),0);

	//patch functions
	/*
	BYTE bPatch[] = {0xE9,0xCC,0x00,0x00,0x00};
	DWORD dwFuncAddr = KO_APRD + 0x23;
	WriteProcessMemory(GetCurrentProcess(),(LPVOID)dwFuncAddr,bPatch,sizeof(bPatch),0);
	*/
}

__declspec(naked) void __stdcall VirtualCT()
{
	__asm
	{
		MOV EAX,[ESP]
		MOV dwSysCallAddrTmp,EAX
		MOV dwSysCallAddrTmp2,EAX
		MOV dwVCTReturn,EAX
		MOV dwTmpAPRBase,EBX
		PUSH dwVirtualCTPtr
		RET
	}
}

HANDLE __stdcall VirtualCreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes,SIZE_T dwStackSize,LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter,DWORD dwCreationFlags,LPDWORD lpThreadId)
{
	dwCheckFuncRet = 0xFFFFFFFF;
	CheckDebugger();

	if( dwCheckFuncRet != 1 )
	{
		LogError(12);
		OwnTerminate();
	}

	if( dwTPTThreadIDAddr )
	{
		if( dwTPTThreadIDAddr == (DWORD)lpThreadId )
		{
			lpStartAddress = (LPTHREAD_START_ROUTINE) &FakeTPTThread;
		}
	}

	if( !bTPTPatched )
	{
		if((DWORD)lpStartAddress >= KO_TPT1)
		{
			dwTPTThreadIDAddr = (DWORD)lpThreadId;
			lpStartAddress = (LPTHREAD_START_ROUTINE) &FakeTPTThread;

			PatchSysCall();
			PatchAPRAPIs();
			PatchAPRReturn();
			bTPTPatched = true;
		}
	}

	HANDLE hRet = CreateRemoteThread((void*)-1,lpThreadAttributes,dwStackSize,lpStartAddress,lpParameter,dwCreationFlags,lpThreadId);
	if( lpThreadId )
		dwLastThreadID = *(DWORD*)lpThreadId;
	TestPatch();

	return hRet;
}

void PatchAPRReturn()
{
	BYTE bComp[] = {0xC9,0xC2,0x04,0x00};
	DWORD dwAddr = dwVCTReturn;

	while(1)
	{
		if( !memcmp((LPVOID)dwAddr, bComp, 4) )
			break;

		dwAddr++;
	}

	dwAddr -= 2;

	BYTE Patch[] = {0x68,0,0,0,0,0xC3};
	*(DWORD*)(Patch + 1) = (DWORD)&PatchCalls;
	WriteProcessMemory((void*)-1, LPVOID(dwAddr), Patch, sizeof(Patch), 0);
}

__declspec(naked) void PatchCalls()
{
	PatchTPTCryption1();

	__asm
	{
		xor eax,eax
		pop edi
		pop ebx
		leave
		ret 4
    }
}

void PatchAPRAPIs()
{
	*(DWORD*)(dwTmpAPRBase + 0x404500) = (DWORD) &VirtualLoadLibrary;
	*(DWORD*)(dwTmpAPRBase + 0x404504) = (DWORD) &VirtualGetProcAddress;
	*(DWORD*)(dwTmpAPRBase + 0x404508) = (DWORD) &VirtualGetModuleHandleA;
}

void PatchSysCall()
{
	DWORD dwTmp = dwTmpAPRBase;
	DWORD dwBase1 = 0, dwBase2 = 0;

	dwSysCallAddrTmp += 0x28;
	dwTmp += *(DWORD*)dwSysCallAddrTmp;	//offset 1
	dwSysCallAddrTmp = dwTmp;
	dwTmp = dwTmpAPRBase;
	dwBase1 = dwSysCallAddrTmp - 0x3B;
	dwSysCallAddrTmp += 0x84;
	dwTmp += *(DWORD*)dwSysCallAddrTmp;	//offset 2
	dwBase2 = dwTmp;

	BYTE bPatch[] = {0x68,0,0,0,0,0xC3};
	*(DWORD*)(bPatch + 1) = (DWORD) &VTSysCall;

	BYTE bPatch2[] = {0x68,0,0,0,0,0xC3};
	*(DWORD*)(bPatch2 + 1) = (DWORD) &VTSysCall2;

	dwRealSysCallPtr = (DWORD)malloc(64);
	::ZeroMemory(LPVOID(dwRealSysCallPtr), 64);
	WriteProcessMemory((void*)-1, LPVOID(dwRealSysCallPtr), LPVOID(dwBase2), 59, 0);

	dwRealSysCallPtr2 = (DWORD)malloc(64);
	::ZeroMemory(LPVOID(dwRealSysCallPtr2), 64);
	WriteProcessMemory((void*)-1, LPVOID(dwRealSysCallPtr2), LPVOID(dwBase1), 59, 0);

	WriteProcessMemory((void*)-1, LPVOID(dwBase1), LPVOID(bPatch2), sizeof(bPatch2), 0);
	WriteProcessMemory((void*)-1, LPVOID(dwBase2), LPVOID(bPatch), sizeof(bPatch), 0);
}

void PatchSysCallCodes()
{
	DWORD dwRetAddr = dwSysCallAddrTmp2 + 0x16;
	DWORD dwFuncAddr = dwTmpAPRBase + *(DWORD*)dwRetAddr;

	DWORD dwOffset1 = *(DWORD*)(dwFuncAddr + 0x5A);	//ZwOpenProcess
	*(DWORD*)(dwTmpAPRBase + dwOffset1) = 0x1111;

	DWORD dwOffset2 = *(DWORD*)(dwFuncAddr + 0x8C);	//ZwCreateFile
	*(DWORD*)(dwTmpAPRBase + dwOffset2) = 0x2222;

	DWORD dwOffset3 = *(DWORD*)(dwFuncAddr + 0x96);	//ZwOpenKey
	*(DWORD*)(dwTmpAPRBase + dwOffset3) = 0x3333;

	DWORD dwOffset4 = *(DWORD*)(dwFuncAddr + 0x64);	//ZwReadVirtualMemory
	*(DWORD*)(dwTmpAPRBase + dwOffset4) = 0x4444;
}

__declspec(naked) void __stdcall VTSysCall()
{
	//EnterCriticalSection(&csAPR);

	if(bFirstSysCall)
		PatchSysCallCodes();

	__asm
	{
		MOV EAX,[ESP]
		MOV dwSysCallRet,EAX
		MOV EAX,[ESP+4]
		MOV dwSysCallCode,EAX
		PUSH dwVirtualSysCallPtr
	}

	//LeaveCriticalSection(&csAPR);

	__asm{ RET };
}

__declspec(naked) void __stdcall VTSysCall2()
{
	//EnterCriticalSection(&csAPR2);

	if(bFirstSysCall)
		PatchSysCallCodes();

	__asm
	{
		MOV EAX,[ESP]
		MOV dwSysCallRet2,EAX
		MOV EAX,[ESP+4]
		MOV dwSysCallCode2,EAX
		PUSH dwVirtualSysCallPtr2
	}

	//LeaveCriticalSection(&csAPR2);

	__asm{ RET };
}

__declspec(naked) void __stdcall VirtualSysCall()
{
	if(bFirstSysCall)
	{
		bFirstSysCall = false;
		__asm
		{
			ADD ESP,0x14
			MOV EAX,0xC0000034	//STATUS_OBJECT_NAME_NOT_FOUND
			PUSH dwSysCallRet
			RET
		}
	}

	if( dwSysCallCode == 0x3333 )	//ZwOpenKey
	{
		__asm
		{
			ADD ESP,0x14
			MOV EAX,0xC0000034	//STATUS_OBJECT_NAME_NOT_FOUND
		}
	}
	else if( dwSysCallCode == 0x2222 ) //ZwCreateFile
	{
		__asm
		{
			ADD ESP,0x34
			MOV EAX,0xC0000034	//STATUS_OBJECT_NAME_NOT_FOUND
		}
	}
	else if( dwSysCallCode == 0x4444 ) //ZwReadVirtualMemory
	{
		__asm
		{
			ADD ESP,0x1C
			MOV EAX,0x8000000D	//STATUS_PARTIAL_COPY
		}
	}
	else if( dwSysCallCode == 0x1111 ) //ZwOpenProcess
	{
		__asm
		{
			MOV EAX,[ESP+8]
			MOV [EAX],1
			ADD ESP,0x18
			XOR EAX,EAX
		}
	}

	/*
	else if( dwSysCallCode == 0xAD ) //ZwQuerySystemInformation
	{
		__asm
		{
			MOV EAX,[ESP+0xC]
			MOV [EAX],1
			ADD ESP,0x18
			XOR EAX,EAX
		}
	}
	else if( dwSysCallCode == 0x42 ) //ZwDeviceIoControlFile
	{
		__asm
		{
			ADD ESP,0x30
			MOV EAX,0xC0000022	//STATUS_ACCESS_DENIED
        }
	}
	else if( dwSysCallCode == 0xB2 ) //ZwQueryVirtualMemory
	{
		__asm
		{
			ADD ESP,0x20
			MOV EAX,0xFFFFFFFF
		}
	}

	else
	{
		MessageBox(0,"unknown syscall!",0,0);
	}
	*/
	else
	{
		__asm
		{
			PUSH dwRealSysCallPtr
			RET
		}
	}

	__asm
	{
		PUSH dwSysCallRet
		RET
	}
}

__declspec(naked) void __stdcall VirtualSysCall2()
{
	if(bFirstSysCall)
	{
		bFirstSysCall = false;
		__asm
		{
			ADD ESP,0x14
			MOV EAX,0xC0000034	//STATUS_OBJECT_NAME_NOT_FOUND
			PUSH dwSysCallRet2
			RET
		}
	}

	if( dwSysCallCode2 == 0x3333 )	//ZwOpenKey
	{
		__asm
		{
			ADD ESP,0x14
			MOV EAX,0xC0000034	//STATUS_OBJECT_NAME_NOT_FOUND
		}
	}
	else if( dwSysCallCode2 == 0x2222 ) //ZwCreateFile
	{
		__asm
		{
			ADD ESP,0x34
			MOV EAX,0xC0000034	//STATUS_OBJECT_NAME_NOT_FOUND
		}
	}
	else if( dwSysCallCode2 == 0x4444 ) //ZwReadVirtualMemory
	{
		__asm
		{
			ADD ESP,0x1C
			MOV EAX,0x8000000D	//STATUS_PARTIAL_COPY
		}
	}
	else if( dwSysCallCode2 == 0x1111 ) //ZwOpenProcess
	{
		__asm
		{
			MOV EAX,[ESP+8]
			MOV [EAX],1
			ADD ESP,0x18
			XOR EAX,EAX
		}
	}

	/*
	else if( dwSysCallCode2 == 0xAD ) //ZwQuerySystemInformation
	{
		__asm
		{
			MOV EAX,[ESP+0xC]
			MOV [EAX],1
			ADD ESP,0x18
			XOR EAX,EAX
		}
	}
	else if( dwSysCallCode2 == 0x42 ) //ZwDeviceIoControlFile
	{
		__asm
		{
			ADD ESP,0x30
			MOV EAX,0xC0000022	//STATUS_ACCESS_DENIED
        }
	}
	else if( dwSysCallCode2 == 0xB2 ) //ZwQueryVirtualMemory
	{
		__asm
		{
			ADD ESP,0x20
			MOV EAX,0xFFFFFFFF
		}
	}

	else
	{
		MessageBox(0,"unknown syscall!",0,0);
	}
	*/
	else
	{
		__asm
		{
			PUSH dwRealSysCallPtr2
			RET
		}
	}

	__asm
	{
		PUSH dwSysCallRet2
		RET
	}
}

void RepairCreateThread()
{
	HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
	DWORD dwAPIAddr = (DWORD) GetProcAddress(hKernel32,"CreateThread");

	BYTE pJump[] = {0x8B,0xFF,0x55,0x8B,0xEC,0xFF,0x75,0x1C,0xFF};
	WriteProcessMemory((void*)-1,(LPVOID)dwAPIAddr,pJump,sizeof(pJump),0);
}

void PatchBotProtection()
{
	DWORD dwTmp;
	BYTE Patch[] = {0xC2,0x08,0x00};

	for (int i = 0; i < 10; i++)
	{
		dwTmp = *(DWORD*)KO_SOCK + 0x40064 + (i * 4);
		WriteProcessMemory((void*)-1,(LPVOID)dwTmp,Patch,sizeof(Patch),0);
	}
}

void __fastcall TfrmBot::btnmenudelosClick(TObject *Sender)
{
	//550010373030315F64756E6765756E2E6C7561

	char pSend[19];

	*pSend = 0x55;
	*(pSend + 1) = 0x00;
	*(pSend + 2) = 16;
	memcpy(LPVOID(pSend + 3),"7001_dungeun.lua",16);

	_CPacket.PSend(pSend,19);
	_CPacket.PSend(pSend,19);

	char pSend2[3];
	*pSend2 = 0x29;
	*(pSend2 + 1) = 0x01;
	*(pSend2 + 2) = 0x03;

	_CPacket.PSend(pSend2,3);

	*pSend2 = 0x12;
	_CPacket.PSend(pSend2,1);

}
//---------------------------------------------------------------------------

void TestPatch()
{
	DWORD dwVal = (DWORD)&VirtualGetModuleFileNameA;
	WriteProcessMemory((void*)-1,(LPVOID)KO_API1,(LPVOID)&dwVal,4,0);

	dwVal = (DWORD)&VirtualGetModuleHandleA;
	WriteProcessMemory((void*)-1,(LPVOID)KO_API2,(LPVOID)&dwVal,4,0);

	dwVal = (DWORD)&VirtualGetProcAddress;
	WriteProcessMemory((void*)-1,(LPVOID)KO_API3,(LPVOID)&dwVal,4,0);

	dwVal = (DWORD)&VirtualGetWindowPID;
	WriteProcessMemory((void*)-1,(LPVOID)KO_API4,(LPVOID)&dwVal,4,0);

	dwVal = (DWORD)&VirtualGetWindowTextA;
	WriteProcessMemory((void*)-1,(LPVOID)KO_API5,(LPVOID)&dwVal,4,0);

	dwVal = (DWORD)&VirtualLoadLibrary;
	WriteProcessMemory((void*)-1,(LPVOID)KO_API6,(LPVOID)&dwVal,4,0);

	dwVal = (DWORD)&VirtualCreateFileA;
	WriteProcessMemory((void*)-1,(LPVOID)KO_API7,(LPVOID)&dwVal,4,0);

	HMODULE hPsapi = GetModuleHandle("psapi.dll");

	if(!hPsapi)
		return;

	BYTE Patch1[] = {0x8B,0xFF,0x8B,0x44,0x24,0x0C,0xC7,0x00,0x00,0x00,0x00,0x00,0xB8,0x01,0x00,0x00,0x00,0xC2,0x0C,0x00};
	DWORD tmpFuncAddr = (DWORD)GetProcAddress(hPsapi,"EnumProcesses");
	WriteProcessMemory((void*)-1,(LPVOID)tmpFuncAddr,Patch1,sizeof(Patch1),0);

	BYTE Patch2[] = {0x8B,0xFF,0xB8,0x01,0x00,0x00,0x00,0xC2,0x10,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hPsapi,"EnumProcessModules");
	WriteProcessMemory((void*)-1,(LPVOID)tmpFuncAddr,Patch2,sizeof(Patch2),0);

	BYTE Patch3[] = {0x8B,0xFF,0x8B,0x44,0x24,0x0C,0xC6,0x00,0x00,0x33,0xC0,0xC2,0x10,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hPsapi,"GetModuleFileNameExA");
	WriteProcessMemory((void*)-1,(LPVOID)tmpFuncAddr,Patch3,sizeof(Patch3),0);

	BYTE Patch4[] = {0x8B,0xFF,0x8B,0x44,0x24,0x0C,0xC6,0x00,0x00,0x33,0xC0,0xC2,0x10,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hPsapi,"GetModuleBaseNameA");
	WriteProcessMemory((void*)-1,(LPVOID)tmpFuncAddr,Patch4,sizeof(Patch4),0);

	BYTE Patch5[] = {0x6A,0x68,0x83,0xC4,0x04,0x33,0xC0,0xC2,0x10,0x00};
	tmpFuncAddr = (DWORD)GetProcAddress(hPsapi,"GetModuleInformation");
	WriteProcessMemory((void*)-1,(LPVOID)tmpFuncAddr,Patch5,sizeof(Patch5),0);
}

NTSTATUS __stdcall VirtualNtQueryInformationProcess(HANDLE ProcessHandle,PROCESSINFOCLASS ProcessInformationClass,PVOID ProcessInformation,ULONG ProcessInformationLength,PULONG ReturnLength)
{
	if(ProcessInformationClass == 7)
	{
		*(DWORD*)ProcessInformation = 0;
		return 0;
	}

	typedef NTSTATUS (__stdcall *MYPROC)(HANDLE,PROCESSINFOCLASS,PVOID,ULONG,PULONG);
	MYPROC pFunction = (MYPROC) GetProcAddress(GetModuleHandleA("ntdll.dll"),"ZwQueryInformationProcess");

	return (pFunction)(ProcessHandle,ProcessInformationClass,ProcessInformation,ProcessInformationLength,ReturnLength);
}

NTSTATUS __stdcall VirtualNtQuerySystemInformation(SYSTEM_INFORMATION_CLASS SystemInformationClass,PVOID SystemInformation,ULONG SystemInformationLength,PULONG ReturnLength)
{
	typedef NTSTATUS (__stdcall *MYPROC)(SYSTEM_INFORMATION_CLASS,PVOID,ULONG,PULONG);
	typedef DWORD (__stdcall *MYPROC2)(HANDLE,HMODULE,LPTSTR,DWORD);

	HMODULE hPsapi = GetModuleHandle("psapi.dll");
	MYPROC2 pFunction2 = (MYPROC2)GetProcAddress(hPsapi, "GetModuleFileNameExA");

	MYPROC pFunction = (MYPROC)GetProcAddress(hPsapi, "ZwQuerySystemInformation");

	NTSTATUS ntRet = (pFunction)(SystemInformationClass,SystemInformation,SystemInformationLength,ReturnLength);

	if( SystemInformationClass == 5 ) //SystemPerformanceInformation
	{
		SYSTEM_PROCESS_INFORMATION *pInfo = (SYSTEM_PROCESS_INFORMATION*)SystemInformation;
		char strPath[256];	memset(strPath,0,256);
		HANDLE hProcess = 0;
		int iRet = 0;

		while(pInfo->NextEntryOffset != NULL)
		{
			if( (DWORD)pInfo->UniqueProcessId == GetCurrentProcessId() )
			{
				pInfo->HandleCount = 0;
				pInfo->PeakPagefileUsage = 0;
				pInfo->PrivatePageCount = 0;
				*(DWORD*)pInfo->Reserved3 = 0;	//parent id
				*(DWORD*)pInfo->Reserved1 = *(DWORD*)pInfo->Reserved1 - 1;	//thread count
			}
			else
			{
				hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,false,(DWORD)pInfo->UniqueProcessId);
				if(hProcess > 0)
				{
					iRet = (pFunction2)(hProcess,(void*)-1,strPath,255);
					if(iRet && strstr(strPath,"kojdbot") > 0)
					{
						pInfo->UniqueProcessId = 0;
						pInfo->Reserved2[0] = 0;
						pInfo->Reserved2[1] = 0;
						pInfo->HandleCount = 0;
						pInfo->PeakPagefileUsage = 0;
						pInfo->PrivatePageCount = 0;
						*(DWORD*)pInfo->Reserved1 = 0;
					}
				}
			}
			pInfo += pInfo->NextEntryOffset;
		}
	}

	return ntRet;
}

SIZE_T __stdcall VirtualVirtualQuery(LPCVOID lpAddress,PMEMORY_BASIC_INFORMATION lpBuffer,SIZE_T dwLength)
{
	SIZE_T szRet = VirtualQuery(lpAddress,lpBuffer,dwLength);

	MEMORY_BASIC_INFORMATION *pInfo = (MEMORY_BASIC_INFORMATION*)lpBuffer;
	pInfo->AllocationBase = 0;
	pInfo->AllocationProtect = 1;
	pInfo->Protect = 0x20000;
	pInfo->State = 0x10000;
	pInfo->RegionSize = 0x50000000;

	return szRet;
}

SIZE_T __stdcall VirtualVirtualQueryEx(HANDLE hProcess,LPCVOID lpAddress,PMEMORY_BASIC_INFORMATION lpBuffer,SIZE_T dwLength)
{
	typedef DWORD (__stdcall *MYPROC)(HANDLE,HMODULE,LPTSTR,DWORD);
	HMODULE hPsapi = GetModuleHandle("psapi.dll");
	MYPROC pFunction = (MYPROC)GetProcAddress(hPsapi, "GetModuleFileNameExA");

	SIZE_T szRet = VirtualQueryEx(hProcess,lpAddress,lpBuffer,dwLength);

	MEMORY_BASIC_INFORMATION *pInfo = (MEMORY_BASIC_INFORMATION*)lpBuffer;

	if( (int)hProcess == (int)-1 || GetProcessId(hProcess) == GetCurrentProcessId() )
	{
		pInfo->AllocationBase = 0;
		pInfo->AllocationProtect = 1;
		pInfo->Protect = 0x20000;
		pInfo->State = 0x10000;
		pInfo->RegionSize = 0x50000000;
	}
	else
	{
		char strPath[256]; memset(strPath,0,256);
		int iRet = (pFunction)(hProcess,(void*)-1,strPath,255);
		if(iRet && strstr(strPath,"kojdbot") > 0)
		{
			pInfo->AllocationBase = 0;
			pInfo->AllocationProtect = 1;
			pInfo->Protect = 0x20000;
			pInfo->State = 0x10000;
			pInfo->RegionSize = 0x50000000;
		}
	}

	return szRet;
}

void __stdcall VirtualGetNativeSystemInfo(LPSYSTEM_INFO lpSystemInfo)
{
	GetNativeSystemInfo(lpSystemInfo);
	*(DWORD*)lpSystemInfo = 1;
}

HMODULE __stdcall VirtualLoadLibrary(LPCTSTR lpFileName)
{
	HMODULE hRet = LoadLibrary(lpFileName);
	if( !strcmp(lpFileName,"PSAPI.dll") || !strcmp(lpFileName,"psapi.dll") )
	{
		TestPatch();
	}
	return hRet;
}

__declspec(naked) void __stdcall VirtualTPTFix()
{
	__asm
	{
		PUSHAD
		SUB ESP,4
    }
	DWORD dwAddr = (DWORD)&VirtualGetProcAddress;

	__asm
	{
		MOV EAX,dwTPTOffset
		MOV BYTE PTR DS:[EBX+EAX],0
		MOV EAX,dwAddr
		MOV DWORD PTR DS:[EBX+0x404504],EAX
	}

	dwAddr = (DWORD)&VirtualGetModuleHandleA;

	__asm
	{
		MOV EAX,dwAddr
		MOV DWORD PTR DS:[EBX+0x404508],EAX
	}

	dwAddr = (DWORD)&VirtualLoadLibrary;

	__asm
	{
		MOV EAX,dwAddr
		MOV DWORD PTR DS:[EBX+0x404500],EAX
	}

	__asm
	{
		ADD ESP,4
		POPAD
		RET
    }
}

__declspec(naked) NTSTATUS __stdcall VN_NtQueryInformationProcess(HANDLE ProcessHandle,PROCESSINFOCLASS ProcessInformationClass,PVOID ProcessInformation,ULONG ProcessInformationLength,PULONG ReturnLength)
{
	__asm
	{
		MOV EAX,0x9A
		MOV EDX,0x7FFE0300
		CALL [EDX]
		RET 0x14
	}
}

__declspec(naked) NTSTATUS __stdcall VN_NtQuerySystemInformation(SYSTEM_INFORMATION_CLASS SystemInformationClass,PVOID SystemInformation,ULONG SystemInformationLength,PULONG ReturnLength)
{
	__asm
	{
		MOV EAX,0xAD
		MOV EDX,0x7FFE0300
		CALL [EDX]
		RET 0x10
	}
}

__declspec(naked) DWORD __stdcall M_GetEIPAddr()
{
	__asm
	{
		MOV EAX,DWORD PTR SS:[ESP]
		SUB EAX,5
		RET
	}
}

bool CheckRemoteSend(DWORD dwRetAddr)
{
	if( *(BYTE*)dwRetAddr == 0x61 )
	{
		return true;
	}

	if( *(WORD*)dwRetAddr == 0x05C6 )
	{
		if( *(BYTE*)(dwRetAddr + 7) == 0x61 )
		{
			return true;
		}
	}

	if( *(WORD*)(dwRetAddr - 2) == 0xD0FF || *(WORD*)(dwRetAddr - 2) == 0xD7FF || *(WORD*)(dwRetAddr - 2) == 0xD6FF || *(WORD*)(dwRetAddr - 2) == 0xD3FF || *(WORD*)(dwRetAddr - 2) == 0xD2FF )
	{
		return true;
	}

	return false;
}

__declspec(naked) void LoginCall()
{
	__asm
	{
		PUSH 1
		PUSH dwLoginFuncPointer
		MOV EAX,1
		RET 4
    }
}

__declspec(naked) void VirtualTPTCall()
{
	__asm
	{
		PUSH 2
		PUSH dwTPTPatchPointer
		MOV EAX,1
		RET 4
    }
}
void __fastcall TfrmBot::chlegalatkClick(TObject *Sender)
{
	bLegalAttack = chlegalatk->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnacceptlistClick(TObject *Sender)
{
	dlgParty->Show();
}
//---------------------------------------------------------------------------

bool CheckIllegal(char* pStr)
{
	if(!pStr)
		return false;

	int iLen = strlen(pStr);
	if( !iLen )
		return false;

	for (int i = 0; i < iLen; i++) {
		if( *(pStr+i) == 0x27 || *(pStr+i) == 0x22)
			return false;  // '
	}

	return true;
}

void __fastcall TfrmBot::chptswClick(TObject *Sender)
{
	bPartySW = chptsw->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::tmbotwaitTimer(TObject *Sender)
{
	bBotPaused = false;
	tmbotwait->Enabled = false;
}
//---------------------------------------------------------------------------

void CheckDLLFile()
{
	char strPath[256]; memset(strPath,0,256);
	bool bFail = false;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	char pFileOut[1024]; memset(pFileOut,0,1024);
	DWORD dwBytesRead = 0;
	bool bReadFile = false;

	GetCurrentDirectoryA(255,strPath);

	if(*(BYTE*)strPath == 0)
	{
		bFail = true;
	}
	else
	{
		strcat(strPath,"\\kojdbot.dll");
		hFile = CreateFileA(strPath,FILE_GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			if(GetLastError() != ERROR_SHARING_VIOLATION)
				bFail = true;
			goto Check;
		}

		bReadFile = ReadFile(hFile,pFileOut,1024,&dwBytesRead,0);

		CloseHandle(hFile);

		if( !bReadFile || !dwBytesRead )
			goto Check;

		if( pFileOut[0x370] != 0x54 )	// "T"
			bFail = true;
	}

Check:

	if( !bFail )
		return;

	/*
	ofstream fOut;
	fOut.open("kojdbot_hata.log",std::ios_base::out | std::ios_base::app);
	if(fOut.is_open() == true)
	{
		fOut << "invalid module!\n";
		fOut.close();
	}
	*/

	LogError(19);
	__asm
	{
		PUSH 0xFFFFFFFF
		POP dwRecvdSockHandle
		XOR EAX,EAX
		SUB ESP,8
		XCHG [ESP+4],EAX
		OR EAX,0xFFFFFFFF
		XCHG [ESP],EAX
		CALL dwTerminatePointer
	}
}

void RandomizeFunction()
{
	DWORD dwTick = GetTickCount();
	BYTE bTick = (BYTE)dwTick;
	int iAdd = bTick % 10;
	int iRand;

	if(iAdd <= 3)
		iRand = 0;
	else if(iAdd <= 6)
		iRand = 1;
	else
		iRand = 2;

	iDLLCheckFuncIndex = iRand;
	dwDLLCheckPointer[iRand] = (DWORD)&dwDLLCheckFunc;
}

void SendTarget()
{
	if( !frmBot->bAttackLeader )
		return;

	_CParty.Refresh();

	int iCount = _CParty.GetCount();

	if(iCount < 2)
		return;

	DWORD dwUser = _CParty.GetUser(0);

	if(!dwUser)
		return;

	DWORD ID = *(WORD*)(dwUser + KO_PT_ID);

	if(ID != (DWORD)_CCharBase.GetID())
		return;

	DWORD dwTargetID = (DWORD)frmBot->wLastTarget;

	for (int i = 0; i < iCount; i++)
	{
		if(!frmBot->hWindowArray[i] || frmBot->hWindowArray[i] == hGameHWND)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,9,dwTargetID);
	}
}

void __fastcall TfrmBot::ch_pclient_0_8Click(TObject *Sender)
{
	if(hWindowArray[0] == NULL)
		return;

	int iVal = (int)ch_pclient_0_8->Checked;

	SendNotifyMessage(hWindowArray[0],WM_KOLEBOT,8,iVal);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::ch_pclient_1_8Click(TObject *Sender)
{
	if(hWindowArray[1] == NULL)
		return;

	int iVal = (int)ch_pclient_1_8->Checked;

	SendNotifyMessage(hWindowArray[1],WM_KOLEBOT,8,iVal);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::ch_pclient_2_8Click(TObject *Sender)
{
	if(hWindowArray[2] == NULL)
		return;

	int iVal = (int)ch_pclient_2_8->Checked;

	SendNotifyMessage(hWindowArray[2],WM_KOLEBOT,8,iVal);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::ch_pclient_3_8Click(TObject *Sender)
{
	if(hWindowArray[3] == NULL)
		return;

	int iVal = (int)ch_pclient_3_8->Checked;

	SendNotifyMessage(hWindowArray[3],WM_KOLEBOT,8,iVal);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::ch_pclient_4_8Click(TObject *Sender)
{
	if(hWindowArray[4] == NULL)
		return;

	int iVal = (int)ch_pclient_4_8->Checked;

	SendNotifyMessage(hWindowArray[4],WM_KOLEBOT,8,iVal);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::ch_pclient_5_8Click(TObject *Sender)
{
	if(hWindowArray[5] == NULL)
		return;

	int iVal = (int)ch_pclient_5_8->Checked;

	SendNotifyMessage(hWindowArray[5],WM_KOLEBOT,8,iVal);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::ch_pclient_6_8Click(TObject *Sender)
{
	if(hWindowArray[6] == NULL)
		return;

	int iVal = (int)ch_pclient_6_8->Checked;

	SendNotifyMessage(hWindowArray[6],WM_KOLEBOT,8,iVal);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::ch_pclient_7_8Click(TObject *Sender)
{
	if(hWindowArray[7] == NULL)
		return;

	int iVal = (int)ch_pclient_7_8->Checked;

	SendNotifyMessage(hWindowArray[7],WM_KOLEBOT,8,iVal);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnpopup6Click(TObject *Sender)
{
	if( !bInterClient )
	{
		MessageBox(this->Handle,"InterClient ozelligi aktif olmadan bunu kullanamazsiniz!","Hata",MB_ICONEXCLAMATION);
		btnpopup6->Checked = false;
		return;
	}

	bAttackWithLeader = btnpopup6->Checked;
}
//---------------------------------------------------------------------------

void CheckDLLName()
{
	HMODULE hSelf = GetModuleHandleA("kojdbot.dll");
	hModuleHandle = hSelf;

	if(hSelf)
		return;

	LogError(13);
	OwnTerminate();
}

void MakeTerminateFunc()
{
	dwNTTerminateFunc = (DWORD) VirtualAlloc(0,128,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	DWORD dwNTTerminateAddr = (DWORD) GetProcAddress(GetModuleHandle("ntdll.dll"),"ZwTerminateProcess");
	memcpy((LPVOID)dwNTTerminateFunc,(LPVOID)dwNTTerminateAddr,64);
}

__declspec(naked) bool __stdcall OwnTerminate()
{
	__asm
	{
		PUSH 0
		PUSH -1
		PUSH 0
		PUSH dwNTTerminateFunc
		RETN
    }
}

void CheckModule()
{
	if( !hModuleHandle )
	{
		LogError(14);
		OwnTerminate();
	}

	DWORD dwTest = (DWORD)hModuleHandle;
	dwTest += 1000;
	dwTest -= 120;	// + 0x370
	dwSendPointerAdd += *(BYTE*)dwTest;
	bPacketKeyOld = dwSendPointerAdd + 1;

	char strVolumeName[256]; memset(strVolumeName,0,256);
	unsigned long ulHDNo = 0;
	bool bRet = GetVolumeInformation("C:\\",strVolumeName,255,&ulHDNo,0,0,0,0);
	if( !bRet )
	{
		MessageBoxA(0,"Volume error!","Error",MB_ICONERROR);
		OwnTerminate();
	}

	ulHDSerialNo += ulHDNo;
	//ulHDNo = ulHDNo / 10;
	__asm
	{
		PUSH ulHDNo
		POP ulRealHDSerialNo
	}
}

int Make0x54Byte()
{
	int iTick = GetTickCount();
	BYTE bTick = (BYTE)iTick;

	while(bTick < 42)
		bTick += 10;

	while(bTick > 60)
		bTick -= 5;

	while( (bTick - 42) != 42)
		bTick++;

	return (int)bTick;
}

HANDLE __stdcall VirtualCreateFileA(LPCTSTR lpFileName,DWORD dwDesiredAccess,DWORD dwShareMode,LPSECURITY_ATTRIBUTES lpSecurityAttributes,DWORD dwCreationDisposition,DWORD dwFlagsAndAttributes,HANDLE hTemplateFile)
{
	if( strstr(lpFileName,"USATrapDump") > 0 || strstr(lpFileName,"kojdbot.dll") > 0 )
	{
		return INVALID_HANDLE_VALUE;
	}

	return CreateFileA(lpFileName,dwDesiredAccess,dwShareMode,lpSecurityAttributes,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile);
}

void PatchTPTCryption1()
{
	BYTE Patch[] = {0x68,0,0,0,0,0xC3};
	*(DWORD*)(Patch+1) = (DWORD)&PatchTPTCryptionCall;
	DWORD dwAddr = KO_CRP2 + 0x5D;
	//memcpy((LPVOID)dwAddr,Patch,sizeof(Patch));
	WriteProcessMemory((void*)-1,(LPVOID)dwAddr,Patch,sizeof(Patch),0);
}

void PatchTPTCryption2()
{
	DWORD dwFuncBase;
	__asm{MOV dwFuncBase,EDX};

	DWORD dwAddr1 = dwFuncBase + 0xA00;
	BYTE cmpPatch1[] = {0x66,0x83,0xF8,0x0D,0x75,0x0F};

	while(1)
	{
		if( memcmp((LPVOID)dwAddr1,cmpPatch1,6) == 0)
			break;

		dwAddr1++;
	}

	dwAddr1 += 6;
	BYTE Patch1[] = {0xEB,0x1B};
	WriteProcessMemory((void*)-1,(LPVOID)dwAddr1,Patch1,sizeof(Patch1),0);

	DWORD dwAddr2 = dwAddr1;
	BYTE cmpPatch2[] = {0x33,0xC9,0x8B,0x8B};

	while(1)
	{
		if( memcmp((LPVOID)dwAddr2,cmpPatch2,4) == 0)
			break;

		dwAddr2++;
	}

	dwAddr2 += 2;

	DWORD dwAddr3 = dwAddr2 + 350;
	BYTE cmpPatch3[] = {0x00,0x75,0x05,0xE9};

	while(1)
	{
		if( memcmp((LPVOID)dwAddr3,cmpPatch3,4) == 0)
			break;

		dwAddr3++;
	}

	dwAddr3 += 8;

	DWORD dwTmpAddr = dwAddr3;

	while(1)
	{
		if( *(BYTE*)dwTmpAddr == 0xEB )
    	{
			if( *(WORD*)(dwTmpAddr + 2) == 0x7D83 )
				break;
		}

		dwTmpAddr++;
    }

	DWORD dwAddr4 = dwAddr3 + 225;
	BYTE cmpPatch4[] = {0x6A,0x00,0xFF,0x93};

	while(1)
	{
		if( memcmp((LPVOID)dwAddr4,cmpPatch4,4) == 0)
			break;

		dwAddr4++;
	}

	BYTE Patch4[] = {0x68,0,0,0,0,0xC3};
	*(DWORD*)(Patch4 + 1) = dwAddr4;
	WriteProcessMemory((void*)-1,(LPVOID)dwTmpAddr,Patch4,sizeof(Patch4),0);

	BYTE Patch2[] = {0x33,0xC0,0x89,0x45,0xB4,0x89,0x07,0x66,0x89,0x47,0x04,0x68,0,0,0,0,0xC3};
	*(DWORD*)(Patch2 + 12) = dwAddr3;
	//memcpy((LPVOID)dwAddr2,Patch2,sizeof(Patch2));
	WriteProcessMemory((void*)-1,(LPVOID)dwAddr2,Patch2,sizeof(Patch2),0);

	BYTE Patch3[] = {0x6A,0x00,0xFF,0x75,0x18,0xFF};
	DWORD dwAddr5 = KO_CRP2 + 0x5D;
	//memcpy((LPVOID)dwAddr3,Patch3,sizeof(Patch3));
	WriteProcessMemory((void*)-1,(LPVOID)dwAddr5,Patch3,sizeof(Patch3),0);
}

__declspec(naked) void __stdcall PatchTPTCryptionCall()
{
	__asm
	{
		PUSHAD
		CALL PatchTPTCryption2
		POPAD
		PUSH KO_CRP2
		ADD [ESP],0x5D
		RETN
	}
}

void RingAlarm()
{
	if(!frmBot->mciAlarm)
		frmBot->mciAlarm = MCIWndCreate(frmBot->Handle, GetModuleHandle(NULL), WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR | MCIWNDF_NOMENU, "alarm.mp3");
	else
		MCIWndHome(frmBot->mciAlarm);

	MCIWndPlay(frmBot->mciAlarm);
}

void __fastcall TfrmBot::btnmenumineClick(TObject *Sender)
{
	//MessageBox(this->Handle,"Henuz tamamlanmadi, diger guncellemeleri bekleyiniz.","Maden",MB_ICONINFORMATION);
	dlgMaden->Show();
}
//---------------------------------------------------------------------------

void WriteLog(char* strFileName, char* strLog)
{
ofstream fLogFile;
fLogFile.open(strFileName,std::ios_base::out | std::ios_base::app);
if(!fLogFile.is_open())return;
fLogFile << strLog << "\n";
fLogFile.close();
}

void __fastcall TfrmBot::chksClick(TObject *Sender)
{
	bKSMod = chks->Checked;

	if(bKSMod)
	{
		tmattack->Interval = 1;
		tmmageattack->Interval = 1;
	}
	else
	{
		tmattack->Interval = 25;
		tmmageattack->Interval = 25;
    }
}
//---------------------------------------------------------------------------

int GetZone()
{
	DWORD dwTmp = *(DWORD*)KO_DLGA;
	dwTmp = *(DWORD*)(dwTmp + 0x4A0);
	dwTmp = *(DWORD*)(dwTmp + 0x170);
	return (int)dwTmp;
}

void __fastcall TfrmBot::ch_pclient_0_9Click(TObject *Sender)
{
	if(hWindowArray[0] == NULL)
		return;

	int iVal = (int)ch_pclient_0_9->Checked;

	SendNotifyMessage(hWindowArray[0],WM_KOLEBOT,10,iVal);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::ch_pclient_1_9Click(TObject *Sender)
{
	if(hWindowArray[1] == NULL)
		return;

	int iVal = (int)ch_pclient_1_9->Checked;

	SendNotifyMessage(hWindowArray[1],WM_KOLEBOT,10,iVal);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::ch_pclient_2_9Click(TObject *Sender)
{
	if(hWindowArray[2] == NULL)
		return;

	int iVal = (int)ch_pclient_2_9->Checked;

	SendNotifyMessage(hWindowArray[2],WM_KOLEBOT,10,iVal);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::ch_pclient_3_9Click(TObject *Sender)
{
	if(hWindowArray[3] == NULL)
		return;

	int iVal = (int)ch_pclient_3_9->Checked;

	SendNotifyMessage(hWindowArray[3],WM_KOLEBOT,10,iVal);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::ch_pclient_4_9Click(TObject *Sender)
{
	if(hWindowArray[4] == NULL)
		return;

	int iVal = (int)ch_pclient_4_9->Checked;

	SendNotifyMessage(hWindowArray[4],WM_KOLEBOT,10,iVal);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::ch_pclient_5_9Click(TObject *Sender)
{
	if(hWindowArray[5] == NULL)
		return;

	int iVal = (int)ch_pclient_5_9->Checked;

	SendNotifyMessage(hWindowArray[5],WM_KOLEBOT,10,iVal);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::ch_pclient_6_9Click(TObject *Sender)
{
	if(hWindowArray[6] == NULL)
		return;

	int iVal = (int)ch_pclient_6_9->Checked;

	SendNotifyMessage(hWindowArray[6],WM_KOLEBOT,10,iVal);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::ch_pclient_7_9Click(TObject *Sender)
{
	if(hWindowArray[7] == NULL)
		return;

	int iVal = (int)ch_pclient_7_9->Checked;

	SendNotifyMessage(hWindowArray[7],WM_KOLEBOT,10,iVal);
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnmenuinterClick(TObject *Sender)
{
	if( !bInterClient )
	{
		MessageBox(this->Handle,"InterClient aktif etmeniz gerek!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	dlgInterClient->Show();
}
//---------------------------------------------------------------------------

void RequestICParty(int iPass)
{
	if(!iPass)
		return;

	char tmp[16]; memset(tmp,0,16);
	itoa(iPass,tmp,10);

	char pSend[8];
	*pSend = 0x10;
	*(pSend + 1) = 0x01;
	*(WORD*)(pSend + 2) = 4;
	memcpy((LPVOID)(pSend + 4),tmp,4);
	_CPacket.PSend(pSend,8);
}

void AddMob(int iMobID)
{
	if(frmBot->iMobCount == 10)
		return;

	CCharBase pMob = _CArea.GetNPCBase(iMobID);
	if(!pMob.GetBase())
		return;

	char *pNewName = new char [32];
	memset(pNewName,0,32);

	strcpy(pNewName,pMob.GetName());
	frmBot->strMobList[frmBot->iMobCount] = pNewName;
	frmBot->lstatakmobs->AddItem(pNewName,NULL);
	frmBot->iMobCount++;
}

void WearNoobItem()
{
	if( !_CCharBase.GetDWORD(KO_CRHP) || CheckLoading() == true )
		return;

	CInvSlot pItem = 0;

	if(pItem.GetBase() == 0)
		return;

	char pSend[8];
	*pSend = 0x1F;
	*(pSend + 1) = 0x01;
	*(DWORD*)(pSend + 2) = pItem.dwItemNum();
	*(pSend + 6) = 0x00;
	*(pSend + 7) = 0x06;
	_CPacket.PSend(pSend,8);
	RefreshInv();
}

void ExpandToSlot(int iRadius)
{
	if( !_CCharBase.GetDWORD(KO_CRHP) || CheckLoading() == true )
		return;

	int iX = (int) _CCharBase.GetFLOAT(KO_POSX);
	int iY = (int) _CCharBase.GetFLOAT(KO_POSY);

	int iNewX = iRandomPos(iX,iRadius);
	int iNewY = iRandomPos(iY,iRadius);

	_CCharBase.MoveTo( (float)iNewX, (float)iNewY );
}

void Suicide()
{
	if(CheckLoading() == true )
		return;

  char pSend[3];
  *pSend = 0x29;
  *(pSend + 1) = 0x01;
  *(pSend + 2) = 0x03;

  _CPacket.PSend(pSend,3);

  *pSend = 0x12;
  _CPacket.PSend(pSend,1);
}

void GoToPlayer(int iTargetID)
{
	if( !_CCharBase.GetDWORD(KO_CRHP) || CheckLoading() == true )
		return;

	CCharBase pUser = _CArea.GetUPCBase(iTargetID);

	if( !pUser.GetBase() )
		return;

	float fX = pUser.GetFLOAT(KO_POSX);
	float fY = pUser.GetFLOAT(KO_POSY);

	_CCharBase.MoveTo(fX,fY);
}

void SendStickSH()
{
	if( !IsLeader() )
		return;

	int iPosX = (int) _CCharBase.GetFLOAT(KO_POSX);
	int iPosY = (int) _CCharBase.GetFLOAT(KO_POSY);
	long lPos = 0;
	__asm
	{
		MOV EAX,iPosX
		ROL EAX,0x10
		MOV AX,WORD PTR DS:[&iPosY]
		MOV lPos,EAX
	}

	for (int i = 0; i < 8; i++)
	{
		if(frmBot->hWindowArray[i] == NULL)
			continue;

		SendNotifyMessage(frmBot->hWindowArray[i],WM_KOLEBOT,19,lPos);
	}
}

void RecvStickSH(long lPos)
{
	if(frmBot->bOnSundWay || frmBot->bOnPotWay || frmBot->bOnInnWay || frmBot->bFollowPaused)
		return;

	int iPosX = 0, iPosY = 0;
	__asm
	{
		MOV EAX,[ESP+0x10]
		MOV WORD PTR DS:[&iPosY],AX
		ROR EAX,0x10
		MOV WORD PTR DS:[&iPosX],AX
	}

	_CCharBase.SetDWORD(KO_MOV1,0);
	_CCharBase.SetDWORD(KO_MOV2,0);
	SHTeleport( (float)iPosX , (float)iPosY ,5);

}

void SHToPlayer(int iTargetID)
{
	if( !_CCharBase.GetDWORD(KO_CRHP) || CheckLoading() == true )
		return;

	CCharBase pUser = _CArea.GetUPCBase(iTargetID);

	if( !pUser.GetBase() )
		return;

	float fX = pUser.GetFLOAT(KO_POSX);
	float fY = pUser.GetFLOAT(KO_POSY);

	_CCharBase.SetDWORD(KO_MOV1,0);
	_CCharBase.SetDWORD(KO_MOV2,0);
	SHTeleport(fX,fY,5);
}

bool IsLeader()
{
	_CParty.Refresh();

	int iCount = _CParty.GetCount();

	if(iCount < 2)
		return false;

	DWORD dwUser = _CParty.GetUser(0);

	if(!dwUser)
		return false;

	DWORD ID = *(WORD*)(dwUser + KO_PT_ID);

	if(ID != (DWORD)_CCharBase.GetID())
		return false;

	return true;
}

void RecvTargetHP(char* pBuf)
{
	DWORD dwID = *(WORD*)(pBuf + 1);
	DWORD dwHP = *(DWORD*)(pBuf + 8);

	if( dwHP )
		return;

	if( dwID < 10000 )
		return;

	CCharBase pTarget = _CArea.GetNPCBase(dwID);

	if( !pTarget.GetBase() )
		return;

	if( pTarget.IsAlive() )
		pTarget.SetDWORD(0x398,8095);	//special
}

void SendTargetHP(DWORD dwID)
{
	char pSend[4];
	*pSend = 0x22;
	*(WORD*)(pSend + 1) = (WORD)dwID;
	*(pSend + 3) = 0x00;
	_CPacket.PSend(pSend,4);
}

void SaveSundIC(){
	if(!frmBot->lstsund->Count)
		return;

	ofstream fSund;

	DeleteFileA("C:\\_sund.kojd");

	fSund.open("C:\\_sund.kojd",std::ios_base::out | std::ios_base::app);
	if(!fSund.is_open())return;

	fSund << frmBot->txtsund->Text.c_str() << "\n";

	for (int i = 0; i < frmBot->lstsund->Count; i++) {
		fSund << frmBot->lstsund->Items->operator [](i).c_str() << "\n";
	}

	fSund.close();
}

void SaveInnIC(){
	if(!frmBot->lstinn->Count)
		return;

	ofstream fInn;

	DeleteFileA("C:\\_inn.kojd");	//

	fInn.open("C:\\_inn.kojd",std::ios_base::out | std::ios_base::app);
	if(!fInn.is_open())return;

	fInn << frmBot->txtinn->Text.c_str() << "\n";

	for (int i = 0; i < frmBot->lstinn->Count; i++) {
		fInn << frmBot->lstinn->Items->operator [](i).c_str() << "\n";
	}

	fInn.close();
}

void SavePotIC(){
	if(!frmBot->lstpot->Count)
		return;

	ofstream fPot;

	DeleteFileA("C:\\_pot.kojd");

	fPot.open("C:\\_pot.kojd",std::ios_base::out | std::ios_base::app);
	if(!fPot.is_open())return;

	fPot << frmBot->txtpot->Text.c_str() << "\n";

	for (int i = 0; i < frmBot->lstpot->Count; i++) {
		fPot << frmBot->lstpot->Items->operator [](i).c_str() << "\n";
	}

	fPot.close();
}

void LoadICRoutes()
{
	char strPath[256]; memset(strPath,0,256);

	GetCurrentDirectoryA(255,strPath);

	strcat(strPath,"\\");
	int iNamePoint = strlen(strPath);

	strcat(strPath,_CCharBase.GetName());
	strcat(strPath,"_sund.kojd");

	CopyFileA("C:\\_sund.kojd",strPath,false);

	*(strPath + iNamePoint) = 0x00;

	strcat(strPath,_CCharBase.GetName());
	strcat(strPath,"_banka.kojd");

	CopyFileA("C:\\_inn.kojd",strPath,false);

	*(strPath + iNamePoint) = 0x00;

	strcat(strPath,_CCharBase.GetName());
	strcat(strPath,"_pot.kojd");

	CopyFileA("C:\\_pot.kojd",strPath,false);

	Sleep(500);

	LoadSund();
	LoadPot();
	LoadInn();
}
void __fastcall TfrmBot::opothermobClick(TObject *Sender)
{
	iTargetingType = 3;
	pntarget->Show();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::opidmobClick(TObject *Sender)
{
	iSubTargetingType = 1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::opnickmobClick(TObject *Sender)
{
	iSubTargetingType = 2;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btntargetcloseClick(TObject *Sender)
{
	pntarget->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnaddidmobClick(TObject *Sender)
{
	if(iTargetMobIDCount == 20)
		return;

	DWORD ID = _CCharBase.GetTargetID();
	if(!ID || (WORD)ID == 0xFFFF)
		return;

	char tmp[16]; memset(tmp,0,16);

	ltoa(ID,tmp,10);
	lstidmob->AddItem(tmp,0);

	iTargetMobID[iTargetMobIDCount] = (int)ID;
	iTargetMobIDCount++;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnclearidmobClick(TObject *Sender)
{
	iTargetMobIDCount = 0;
	lstidmob->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btngetnickmobClick(TObject *Sender)
{
	DWORD ID = _CCharBase.GetTargetID();
	if(!ID || (WORD)ID == 0xFFFF)
		return;

	CCharBase pUser = _CArea.GetUPCBase(ID);
	if(!pUser.GetBase())
		return;

	txtnickmob->Text = pUser.GetName();
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnaddnickmobClick(TObject *Sender)
{
	if(iTargetNickCount == 10)
		return;

	int iLen = txtnickmob->Text.Length();
	if(!iLen || iLen >= 32)
		return;

	char *pNewName = new char [32];
	memset(pNewName,0,32);

	strcpy(pNewName,txtnickmob->Text.c_str());
	strTargetNick[iTargetNickCount] = pNewName;
	lstnickmob->AddItem(pNewName,0);
	iTargetNickCount++;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnclearnickmobClick(TObject *Sender)
{
	if(!iTargetNickCount)
		return;

	lstnickmob->Clear();

	for (int i = 0; i < 10; i++) {
		if(strTargetNick[i])
			delete [] strTargetNick[i];
		strTargetNick[i] = NULL;
	}

	iTargetNickCount = 0;
}
//---------------------------------------------------------------------------

void InitMobIDList()
{
	int c = frmBot->lstidmob->Items->Count;
	if(!c)	return;

	for (int i = 0; i < c; i++)
	{
		frmBot->iTargetMobID[i] = atoi( frmBot->lstidmob->Items->operator [](i).c_str() );
	}
}

void InitMobNickList()
{
	int c = frmBot->lstnickmob->Items->Count;
	if(!c)	return;

	for (int i = 0; i < c; i++)
	{
		char *pNewName = new char [32];
		memset(pNewName,0,32);

		strcpy(pNewName,frmBot->lstnickmob->Items->operator [](i).c_str());
		frmBot->strTargetNick[i] = pNewName;
	}
}

void InformTPT(char* pBuf)
{
	if( *(BYTE*)(pBuf + 1) != 0x02 || *(WORD*)(pBuf + 2) != 0xFFFF )
		return;

	if(!_CWinsock.GetState()){
		bool bStart = _CWinsock.StartWinsock();
		if(!bStart)
			return;
	}

	CMainSocket = _CWinsock.NewSocket();
	if((long)CMainSocket.GetSocket() == INVALID_SOCKET)
		return;

	if(!CMainSocket.Client_Connect(strServerIP,iServerPort))
	{
		CMainSocket.Close();
		return;
	}

	char *pSend = new char [7];
	memset(pSend,0,7);

	int index(0);

	CMainSocket.PutPacketHead(pSend,index);
	CMainSocket.PutBYTE(pSend,bSend_TPT,index);
	CMainSocket.PutWORD(pSend,(WORD)iMyUserID,index);
	CMainSocket.PutPacketTail(pSend,index);
	CMainSocket.Client_Send(pSend,7);

	delete[] pSend;
	CMainSocket.Close();
}

void GetAPIChecks()
{
	DWORD dwAddr = 0;
	HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");

	dwAddr = (DWORD) GetProcAddress( hKernel32, "TerminateProcess" );
	dwAPIAddr[0] = dwAddr;
	dwAPIVal[0] = *(DWORD*)dwAddr;

	dwAddr = (DWORD) GetProcAddress( hKernel32, "GetVolumeInformationA" );
	dwAPIAddr[1] = dwAddr;
	dwAPIVal[1] = *(DWORD*)dwAddr;

	dwAddr = (DWORD) GetProcAddress( hKernel32, "ExitProcess" );
	dwAPIAddr[2] = dwAddr;
	dwAPIVal[2] = *(DWORD*)dwAddr;

	dwAddr = (DWORD) GetProcAddress( hKernel32, "GetComputerNameA" );
	dwAPIAddr[3] = dwAddr;
	dwAPIVal[3] = *(DWORD*)dwAddr;

	dwAddr = (DWORD) GetProcAddress( hKernel32, "GetSystemRegistryQuota" );
	dwAPIAddr[4] = dwAddr;
	dwAPIVal[4] = *(DWORD*)dwAddr;

	dwAddr = (DWORD) GetProcAddress( hKernel32, "FindFirstVolumeA" );
	dwAPIAddr[5] = dwAddr;
	dwAPIVal[5] = *(DWORD*)dwAddr;

	dwAddr = (DWORD) GetProcAddress( GetModuleHandle("User32.dll"), "GetKeyboardType" );
	dwAPIAddr[6] = dwAddr;
	dwAPIVal[6] = *(DWORD*)dwAddr;

	dwAddr = (DWORD) GetProcAddress( GetModuleHandleA("ntdll.dll"), "ZwTerminateProcess" );
	dwAPIAddr[7] = dwAddr;
	dwAPIVal[7] = *(DWORD*)dwAddr;

	dwAddr = (DWORD) GetProcAddress( GetModuleHandleA("advapi32.dll"), "GetUserNameA" );
	dwAPIAddr[8] = dwAddr;
	dwAPIVal[8] = *(DWORD*)dwAddr;
}

DWORD GetServerARG()
{
	DWORD dwARG = 0;

	for (int i = 0; i < 6; i++)
	{
		dwARG += GetServerARGSub(i);
	}

	return dwARG;
}

DWORD GetServerARGSub(int iType)
{
	switch(iType)
	{
		case 0:
		return GetServerARGSub1();

		case 1:
		return GetServerARGSub2();

		case 2:
		return GetServerARGSub3();

		case 3:
		return GetServerARGSub4();

		case 4:
		return GetServerARGSub5();

		case 5:
		return GetServerARGSub6();
	}

	return 0;
}

DWORD GetServerARGSub1()
{
	char strName[64]; memset(strName,0,64);

	unsigned long ulSize = 32;

	GetComputerName(strName, &ulSize);

	DWORD dwARG = 0;

	for (int i = 0; i < ulSize; i++)
	{
		dwARG += *(strName + i);
	}

	return dwARG;
}

DWORD GetServerARGSub2()
{
	DWORD dwARG = 0;
	GetSystemRegistryQuota( &dwARG, 0);
	return dwARG;
}

DWORD GetServerARGSub3()
{
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;

	SYSTEM_INFO pInfo;
	ZeroMemory( &pInfo, sizeof(SYSTEM_INFO) );
	BOOL bIsWow64 = false;

	fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(GetModuleHandle("kernel32.dll"),"IsWow64Process");
	if( !(DWORD)fnIsWow64Process )
		return 0;

	fnIsWow64Process( (void*)-1, &bIsWow64 );

	if( bIsWow64 )
		GetNativeSystemInfo( &pInfo );
	else
		GetSystemInfo( &pInfo );

	DWORD dwARG = 0;

	dwARG += pInfo.dwNumberOfProcessors;
	/*
	dwARG += pInfo.wProcessorRevision;
	dwARG += pInfo.wProcessorArchitecture;
	dwARG += pInfo.wProcessorLevel;
	*/
	dwARG += pInfo.dwProcessorType;
	dwARG += pInfo.dwOemId;

	return dwARG;
}

DWORD GetServerARGSub4()
{
	char strName[256]; memset(strName,0,256);

	HANDLE hVolume = FindFirstVolume(strName,255);
	if( hVolume == INVALID_HANDLE_VALUE )
		return 0;

	FindVolumeClose(hVolume);

	int iLen = strlen(strName);
	if( !iLen )
    	return 0;

	DWORD dwARG = 0;

	for (int i = 0; i < iLen; i++)
	{
		dwARG += *(strName + i);
	}

	return dwARG;
}

DWORD GetServerARGSub5()
{
	DWORD dwARG = 0;

	for (int i = 0; i < 3; i++)
	{
		dwARG += GetKeyboardType( i );
	}

	return dwARG;
}

DWORD GetServerARGSub6()
{
	char strName[256]; memset(strName,0,256);
	unsigned long lSize = 255;

	if( !GetUserName(strName, &lSize) )
		return 0;

	DWORD dwARG = 0;

	for (int i = 0; i < lSize; i++)
	{
		dwARG += *(strName + i);
	}

	return dwARG;
}

void CheckDebugger()
{
	__asm XOR dwDebuggerFree,0xFFFFFFFE

	if( dwDebuggerFree == 1 )
	{
		dwDebuggerFree = 0xFFFFFFFF;
		__asm XOR dwCheckFuncRet,0xFFFFFFFE
		return;
	}
	else
		dwDebuggerFree = 0xFFFFFFFE;

	// ANTI-WPE test :D
	if( GetModuleHandle("WpeSpy.dll") )
	{
		LogError(62);	// :D
		return;
	}

	DWORD dwDebugEntry = (DWORD) GetProcAddress( GetModuleHandleA("ntdll.dll"), "DbgBreakPoint" );
	if( *(BYTE*)dwDebugEntry != 0xC3 ) 	//RETN
	{
		return;
	}

	typedef NTSTATUS (__stdcall *MYPROC)(HANDLE,PROCESSINFOCLASS,PVOID,ULONG,PULONG);
	MYPROC pFunction = (MYPROC) GetProcAddress(GetModuleHandleA("ntdll.dll"),"ZwQueryInformationProcess");

	if( !(DWORD)pFunction )
	{
		__asm XOR dwCheckFuncRet,0xFFFFFFFE
		return;
	}

	DWORD dwRet = 0;

	NTSTATUS ntRet = (pFunction)( (void*)-1 , 7, (PVOID)&dwRet, 4, 0);

	if(ntRet)
	{
		__asm XOR dwCheckFuncRet,0xFFFFFFFE
		return;
	}

	if(dwRet)	//debugger detected
	{
		typedef NTSTATUS (__stdcall *MYPROC2)(HANDLE,NTSTATUS);
		MYPROC2 pFunction2 = (MYPROC2) GetProcAddress(GetModuleHandleA("ntdll.dll"),"ZwTerminateProcess");
		LogError(15);
		if( (DWORD)pFunction2 )
			(pFunction2)( (void*)-1, 0 );
		else
		{
			__asm
			{
				push 0
				push -1
				push 0
				push dwNTTerminateFunc
				retn
            }
        }

	}
	else
		__asm XOR dwCheckFuncRet,0xFFFFFFFE
}

void GetFuncChecks()
{
	DWORD dwVal = 0;

	for (int i = 0; i < 20; i++)
	{
		dwFuncVal[i] = *(DWORD*)dwFuncAddr[i];
	}
}

void StoreMenuIDs()
{
	int index(0);

	HWND hPopup = frmBot->popup1->Handle;

	for (int i = 0; i < 6; i++)
	{
		frmBot->iMenuIDArray[index] = GetMenuItemID(hPopup, i);
		index++;
	}

	hPopup = frmBot->popup2->Handle;

	for (int i = 0; i < 5; i++)
	{
		frmBot->iMenuIDArray[index] = GetMenuItemID(hPopup, i);
		index++;
	}

	for (int i = 0; i < 8; i++)
	{

		hPopup = frmBot->hPopupArray[i];

		for (int x = 0; x < 10; x++)
		{
			frmBot->iMenuIDArray[index] = GetMenuItemID(hPopup, x);
			index++;
		}

	}

	hPopup = frmBot->popup1->Handle;

	frmBot->iMenuIDArray[index] = GetMenuItemID(hPopup, 6);
	index++;

	frmBot->iMenuIDArray[index] = GetMenuItemID(hPopup, 7);
	index++;

	hPopup = frmBot->popupchar->Handle;

	frmBot->iMenuIDArray[index] = GetMenuItemID(hPopup, 0);
	index++;

	frmBot->iMenuIDArray[index] = GetMenuItemID(hPopup, 1);
	index++;

}

void __fastcall TfrmBot::btnpopup7Click(TObject *Sender)
{
	frmBot->chfollowpt->Checked = btnpopup7->Checked;
	frmBot->bFollowLeader = btnpopup7->Checked;

	if( btnpopup7->Checked )
	{
		frmBot->chfollowselected->Checked = false;
		frmBot->bFollowSelected = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnpopup8Click(TObject *Sender)
{
	if( !bInterClient )
	{
		MessageBox(this->Handle,"InterClient ozelligi aktif olmadan bunu kullanamazsiniz!","Hata",MB_ICONEXCLAMATION);
		btnpopup8->Checked = false;
		return;
	}

	bAttackLeader = btnpopup8->Checked;
}
//---------------------------------------------------------------------------

bool CheckCharPopup(int iX, int iY)
{
	float fX,fY,fZ;
	DWORD dwBase = _CTest.PickChar(iX,iY,fX,fY,fZ);
	if(!dwBase)
		return false;

	frmBot->dwPopupBase = dwBase;
	TrackPopupMenu( frmBot->popupchar->Handle, TPM_LEFTALIGN, iX, iY, 0, hGameHWND, 0);
	return true;
}

void __fastcall TfrmBot::btnpopup_addmobClick(TObject *Sender)
{
	if(iMobCount == 10)
		return;

	if( !dwPopupBase )
		return;

	CCharBase pChar = dwPopupBase;

	char *pNewName = new char [32];
	memset(pNewName,0,32);

	strcpy(pNewName,pChar.GetName());
	strMobList[iMobCount] = pNewName;
	lstatakmobs->AddItem(pNewName,Sender);
	iMobCount++;
}
//---------------------------------------------------------------------------

void __fastcall TfrmBot::btnpopup_printinfoClick(TObject *Sender)
{
	if( !dwPopupBase )
		return;

	CCharBase pChar = dwPopupBase;

	char strInfo[64]; memset(strInfo,0,64);
	char tmp[32]; memset(tmp,0,32);

	strcpy(strInfo,"Name: ");
	strcat(strInfo,pChar.GetName());
	_CGeneral.PostMsgOut(strInfo,0xFF50FF50);

	memset(strInfo,0,64);

	strcpy(strInfo,"Level: ");
	itoa(pChar.GetDWORD(KO_LEVL),tmp,10);
	strcat(strInfo,tmp);
	_CGeneral.PostMsgOut(strInfo,0xFF50FF50);

	memset(strInfo,0,64);

	strcpy(strInfo,"Tip: ");
	if(pChar.IsUPC())
		strcat(strInfo,"Player");
	else
		strcat(strInfo,"NPC-MOB");
	_CGeneral.PostMsgOut(strInfo,0xFF50FF50);

	memset(strInfo,0,64);

	strcpy(strInfo,"Irk: ");
	switch(pChar.GetDWORD(KO_NATN))
	{
		case 1:
		strcpy(tmp,"Karus");
		break;

		case 2:
		strcpy(tmp,"Elmorad");
		break;

		default:
		strcpy(tmp,"Mob");
		break;
	}
	strcat(strInfo,tmp);
	_CGeneral.PostMsgOut(strInfo,0xFF50FF50);
}
//---------------------------------------------------------------------------

void LogError(int iError)
{
	HANDLE hFile =   CreateFileA("error_kojd.log",GENERIC_WRITE,FILE_SHARE_WRITE,0,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);

	if(hFile == INVALID_HANDLE_VALUE)
		return;

	char tmp[8]; memset(tmp,0,8);
	*tmp = '[';
	itoa(iError,(char*)(tmp+1),10);

	DWORD dwWritten = 0;

	int iSize = 3;

	if(iError >= 10)
		iSize++;

	*(tmp+iSize-1) = ']';

	WriteFile(hFile,tmp,iSize,&dwWritten,0);
	CloseHandle(hFile);
}

HWND __stdcall VirtualCreateWindowEx(DWORD dwExStyle,LPCTSTR lpClassName,LPCTSTR lpWindowName,DWORD dwStyle,int x,int y,int nWidth,int nHeight,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam)
{
	DWORD dwRetnAddr = 0;

	__asm
	{
		mov eax,[ebp+4]
		mov dwRetnAddr,eax
	}

	if( *(WORD*)dwRetnAddr == 0xF08B )
	{
		//dwStyle = WS_OVERLAPPEDWINDOW;
		dwStyle = dwStyle | WS_SIZEBOX;
	}

	return CreateWindowExA(dwExStyle,lpClassName,lpWindowName,dwStyle,x,y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam);
}

long WINAPI MyExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	return EXCEPTION_CONTINUE_EXECUTION;
}

void EditDbgBreakPoint()
{
	HANDLE hFile = CreateFileA("_dbg.kojd",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return;
	}

	DWORD dwEntry = (DWORD) GetProcAddress( GetModuleHandleA("ntdll.dll"), "DbgBreakPoint" );
	BYTE bEdit = 0xC3;

	WriteProcessMemory( (void*)-1, (LPVOID)dwEntry, (LPVOID)&bEdit, 1, 0 );

	CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DbgEntryCheck,NULL,0,0);
}

void DbgEntryCheck()
{
	typedef NTSTATUS (__stdcall *MYPROC)(HANDLE,NTSTATUS);
	MYPROC pTerminate = (MYPROC) GetProcAddress(GetModuleHandleA("ntdll.dll"),"ZwTerminateProcess");
	DWORD dwDebugEntry = (DWORD) GetProcAddress( GetModuleHandleA("ntdll.dll"), "DbgBreakPoint" );

	while(1)
	{
		if( *(BYTE*)dwDebugEntry != 0xC3 ) 	//RETN
		{
			LogError(31);	// :D
			(pTerminate)( (void*)-1, 0 );
			return;
		}

		// ANTI-WPE Test :D
		if( GetModuleHandle("WpeSpy.dll") )
		{
			LogError(62);	// :D
			(pTerminate)( (void*)-1, 0 );
			return;
		}

		Sleep(1000);

		if(bGameLoaded)
			return;
	}
}

void FakeTPTThread()
{
	while(1)
	{
		//uyutalim.
		Sleep(10000);
    }
}

bool NewConnect()
{

	HANDLE hDNS1,hDNS2,hDNS3,hDNS4,hDNS5,hDNS6;

	hDNS1 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DNSThread,strDNS1,0,0);
	if( WaitForSingleObject(hDNS1,1000) != WAIT_TIMEOUT )
		return true;

	TerminateThread(hDNS1,0);
	CloseHandle(hDNS1);

	hDNS2 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DNSThread,strDNS2,0,0);
	if( WaitForSingleObject(hDNS2,1000) != WAIT_TIMEOUT )
		return true;

	TerminateThread(hDNS2,0);
	CloseHandle(hDNS2);

	hDNS3 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DNSThread,strDNS3,0,0);
	if( WaitForSingleObject(hDNS3,1000) != WAIT_TIMEOUT )
		return true;

	TerminateThread(hDNS3,0);
	CloseHandle(hDNS3);

	hDNS4 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DNSThread,strDNS4,0,0);
	if( WaitForSingleObject(hDNS4,1000) != WAIT_TIMEOUT )
		return true;

	TerminateThread(hDNS4,0);
	CloseHandle(hDNS4);

	hDNS5 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DNSThread,strDNS5,0,0);
	if( WaitForSingleObject(hDNS5,1000) != WAIT_TIMEOUT )
		return true;

	TerminateThread(hDNS5,0);
	CloseHandle(hDNS5);

	hDNS6 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DNSThread,strDNS6,0,0);
	if( WaitForSingleObject(hDNS6,1000) != WAIT_TIMEOUT )
		return true;

	TerminateThread(hDNS6,0);
	CloseHandle(hDNS6);

	return false;
}

bool DNSThread(char* strDNS)
{
	return CMainSocket.Client_Connect(strServerIP,iServerPort);
}
