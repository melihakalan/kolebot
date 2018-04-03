//---------------------------------------------------------------------------

#ifndef MainH
#define MainH

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>

#include <Classes.hpp>

#include "Memory.h"
#include "ffrmBot.h"

#define CopyBytes(Dest, Src) memcpy(Dest, (BYTE*)&Src, sizeof(Src))

//***** ITEMS *****//
#define ITEM_WOLF			370004000
#define ITEM_ARROW			391010000
#define ITEM_NOAH			900000000
#define ITEM_HP45			389010000
#define ITEM_HP90			389011000
#define ITEM_HP180			389012000
#define ITEM_HP360			389013000
#define ITEM_HP720			389014000
#define ITEM_MP120			389016000
#define ITEM_MP240			389017000
#define ITEM_MP480			389018000
#define ITEM_MP960			389019000
#define ITEM_MP1920			389020000
#define ITEM_KILLINGBLADE	700037000
#define ITEM_TS_SCROLL		381001000
#define ITEM_TS_GEM			379091000
#define ITEM_BLESSING_POT	370005000
#define ITEM_PREPOT_HP		389390000
#define ITEM_PREPOT_MP		389400000
#define ITEM_ROCK			389043000
#define ITEM_BOOK			389026000
#define ITEM_MASTER_ROGUE	379060000
#define ITEM_MASTER_MAGE	379061000
#define ITEM_MASTER_PRIEST	379062000
#define ITEM_MASTER_WARRIOR	379059000
#define ITEM_DCPOT_HP		399284000
#define ITEM_DCPOT_MP		399285000

#define ITEM_ACID   		389083000
#define ITEM_BEZOAR   		389034000
#define ITEM_GLUTINOUS     	389035000
#define ITEM_FIRECRACKER   	379092000
#define ITEM_FLOWERFC	  	379137000
#define ITEM_UP_LOWCLASS  	379221000
#define ITEM_CRYSTAL	  	389075000
#define ITEM_OPAL		  	389076000
#define ITEM_SAPPHIRE	   	389074000
#define ITEM_CALLINGSC	   	389050000
#define ITEM_BUS		   	379021000
#define ITEM_UP_SC		   	379016000
#define ITEM_ABYSS		   	379106000
#define ITEM_MS				900144023
#define ITEM_MP_RICE		800034000
#define ITEM_BRONZE_COIN	379110000
#define ITEM_SILVER_COIN	379109000
#define ITEM_GOLD_COIN		379107000

#define ITEM_ABYSS			379106000
#define ITEM_BLUE_CHEST		379156000
#define ITEM_GREEN_CHEST	379155000
#define ITEM_RED_CHEST	  	379154000

#define ITEM_FRAGMENT1		389160000
#define ITEM_FRAGMENT2      389166000
#define ITEM_FRAGMENT3      389161000
#define ITEM_FRAGMENT4      389165000
#define ITEM_FRAGMENT5      389164000
#define ITEM_FRAGMENT6      389162000
#define ITEM_FRAGMENT7      389163000


//***** SKILLS *****//
#define SKILL_HP45			490010
#define SKILL_HP90			490011
#define SKILL_HP180			490012
#define SKILL_HP360			490013
#define SKILL_HP720			490014
#define SKILL_MP120			490016
#define SKILL_MP240			490017
#define SKILL_MP480			490018
#define SKILL_MP960			490019
#define SKILL_MP1920		490020
#define SKILL_CHAOSSPRINT	490223
#define SKILL_CHAOSSWORD	490219
#define SKILL_CHAOSBLADE	490226
#define SKILL_CHAOSPOT		490222
#define SKILL_TS_CAST		472001
#define SKILL_TS_BOWMAN		472310
#define SKILL_TS_KECOON		472020
#define SKILL_TS_BULCAN		472040
#define SKILL_PREPOT_HP		500145
#define SKILL_PREPOT_MP		500146
#define SKILL_ROCK			490043
#define SKILL_DCPOT_HP		490074
#define SKILL_DCPOT_MP		490140
#define SKILL_MPRICE		501002
#define SKILL_MEAT			490141

//***** OTHER *****//

//Bag
const int iMaxInventorySlot = 28;
const int iMaxInnPageSlot = 24;
const int iMaxInnPage = 8;
const int iMaxInnSlot = iMaxInnPageSlot * iMaxInnPage;

//Trap
const int TRAP_iTrapDelay	= 1000;
const BYTE TRAP_bCryptByte	= 0x53;
const BYTE TRAP_bWSockByte	= 0x8B;
const int TRAP_iMaxSendAdr	= 0x500000;

//RecvFunction
const int RF_iExecDelay		= 250;

//////////////////////////////
//***** GLOBAL CLASSES *****//
/////////////////////////////

class CGeneral{
	public:
		//General
		inline void SetWindowTop(HWND hHWND, bool bTop){HWND hType = HWND_TOPMOST; if(!bTop)hType = HWND_NOTOPMOST; SetWindowPos(hHWND,hType,0,0,0,0,SWP_NOSIZE + SWP_NOMOVE);};
		bool ConnectServer(char* strIP, int iPort);
		void DisconnectServer();
		void PostMsgBox(char* strMsg, BYTE bButton, BYTE bFunction);
		void PostMsgOut(char* strMsg, DWORD dwColor);
		void PassEncrypt(char* strPass, char* pEncBuffer);
		//Default
		CGeneral(){};
};

class CCharBase{
	private:
		DWORD dwBase;
	public:
		//General
		inline void		SetBase(DWORD dwAddr){dwBase = dwAddr;};
		inline DWORD 	GetBase(){return dwBase;};
		inline WORD 	GetID(){return GetWORD(KO_MYID);};
		inline char*	GetName(){return (LPSTR)GetDWORD(KO_NICK);};
		inline bool 	IsUPC(){return(*(DWORD*)(dwBase + 0x30) == 2);};

		bool IsAlive()
		{
		if(		GetDWORD(0x398)==4
			|| 	GetDWORD(0x398)==5
			|| 	GetDWORD(0x398)==0xD
			|| 	GetDWORD(0x398)==8095
			|| 	GetDWORD(0x3A0) ==5
			|| 	GetDWORD(KO_CHST) != 0)
				return false;
			else
				return true;
		}

		inline bool		IsSit(){return(GetDWORD(0x398) == 7);};
		inline void		MoveTo(FLOAT X, FLOAT Y){SetFLOAT(KO_PTRX,X); SetFLOAT(KO_PTRY,Y); SetDWORD(KO_MOV1,2); SetDWORD(KO_MOV2,2);};
		inline void		SetSize(FLOAT fSize){SetFLOAT(KO_SIZE,fSize);};
		inline FLOAT	GetSize(){return GetFLOAT(KO_SIZE);};
		inline WORD		GetTargetID(){return GetWORD(KO_TGID);};
		bool IsEnemy();
		//Set memory
		inline void 	SetBYTE(DWORD dwOffset,BYTE bVal){*(BYTE*)(dwBase + dwOffset) = bVal;};
		inline void 	SetWORD(DWORD dwOffset,WORD wVal){*(WORD*)(dwBase + dwOffset) = wVal;};
		inline void 	SetDWORD(DWORD dwOffset,DWORD dwVal){*(DWORD*)(dwBase + dwOffset) = dwVal;};
		inline void		SetFLOAT(DWORD dwOffset,FLOAT fVal){*(FLOAT*)(dwBase + dwOffset) = fVal;};
		//Get memory
		inline BYTE 	GetBYTE(DWORD dwOffset){return *(BYTE*)(dwBase + dwOffset);};
		inline WORD 	GetWORD(DWORD dwOffset){return *(WORD*)(dwBase + dwOffset);};
		inline DWORD 	GetDWORD(DWORD dwOffset){return *(DWORD*)(dwBase + dwOffset);};
		inline FLOAT 	GetFLOAT(DWORD dwOffset){return *(FLOAT*)(dwBase + dwOffset);};
		//Default
		CCharBase(){dwBase = 0;};
		CCharBase(DWORD dwBaseAddr){dwBase = dwBaseAddr;};
};

//***** FUNCTION CLASSES *****//

class CPacket{
	private:
		int iSendCount;
	public:
		//General
		DWORD dwGameFunction;
		BYTE bFuncIndex;

		void PSend2(BYTE* pBuf,size_t szBuf);
		__declspec(naked) __stdcall void PSend(BYTE* pBuf,size_t szBuf);
		bool PatchGameFunction();
		void RepairGameFunction();
		void PatchHooked();
		BYTE GetIndex();
		inline bool GetConnState(){return *(BYTE*)(*(DWORD*)(KO_SOCK) + 0x4003C);};
		inline int GetPCount(){return iSendCount;};
		inline void AddPCount(){iSendCount += 1;};
		inline void ClearPCount(){iSendCount = 0;};
		//Set memory
		inline void PutBYTE(char* pBuffer,BYTE bVal){*(pBuffer) = bVal;};
		inline void PutWORD(char* pBuffer,WORD wVal){*(WORD*)(pBuffer) = wVal;};
		inline void PutDWORD(char* pBuffer,DWORD dwVal){*(DWORD*)(pBuffer) = dwVal;};
		inline void PutSTR(char* pBuffer,char* pStr,int iSize){memcpy(pBuffer,pStr,iSize);};
		//Get memory
		inline BYTE GetBYTE(char* pBuffer){return *(BYTE*)(pBuffer);};
		inline WORD GetWORD(char* pBuffer){ return *(WORD*)(pBuffer);};
		inline DWORD GetDWORD(char* pBuffer){return *(DWORD*)(pBuffer);};
		inline void GetSTR(char* sBuffer,char* dBuffer,int iSize){memcpy(dBuffer,sBuffer,iSize);};
		//Default
		CPacket(){iSendCount = 0; bFuncIndex = -1; dwGameFunction = 0;};
};

class CArea{
	private:
		DWORD dwLastNearstBase;
	public:
		//General
		bool CheckGM();
		int GetUPCCount();
		int GetNPCCount();
		inline int GetAllCount(){return (GetUPCCount() + GetNPCCount());};
		DWORD GetUPCBase(WORD wID);
		DWORD GetNPCBase(WORD wID);
		DWORD GetCorpseBase(WORD wID);
		DWORD GetAnyBase(WORD wID);
		DWORD GetZEnemyBase();
		DWORD GetNPCByName(int iNameCount, char* ptrName[], char* sName,bool bNearst, bool bEnemy,bool bAlive, FLOAT fMaxDist, bool bLoop);
		DWORD GetUPCByName(int iNameCount, char* ptrName[], char* sName, bool bAlive, FLOAT fMaxDist, bool bNotInParty,bool bEnemy);
		DWORD GetNearstUPC(bool bEnemy,bool bAlive,FLOAT fMaxDist);
		void SelectObject(CCharBase Object);
		DWORD GetNearstEnemy(int iLevel, bool bAlive, FLOAT fMaxDist);
		void LootChaosCubes();
		DWORD GetNearstNPCByID(int iIDArray[],int iIDCount, bool bAlive, FLOAT fMaxDist);
		DWORD NextNPC(DWORD dwOldBase);
		DWORD NextUPC(DWORD dwOldBase);
		//Default
		CArea(){dwLastNearstBase = 0;};
};

class CMath{
	public:
		//General
		FLOAT fDist(FLOAT X1, FLOAT Y1, FLOAT X2, FLOAT Y2);
		FLOAT iPerc(int HiVal, int LoVal);
		//Default
		CMath(){};
};

class CCast{
	public:
		//General
		int CastCount();
		int CastNum(int iNo);
		int MakeCastNum(CCharBase dwBase, char* strTail);
		bool IsCasted(int Num);
		//Default
		CCast(){};

};

class CInvSlot{
	private:
		DWORD dwBase;
		int iCurrentSlot;
	public:
		//General
		inline int GetCurrentSlot(){return iCurrentSlot;};
		void SelectSlot(int iSlot);
		void SelectLeft();
		void SelectRight();
		void SelectHelmet();
		void SelectPauldron();
		void SelectPads();
		void SelectGloves();
		void SelectBoots();
		inline DWORD GetBase(){return dwBase;};
		inline int dwItemNum(){return *(DWORD*)(*(DWORD*)(dwBase + 0x38)) + *(DWORD*)(*(DWORD*)(dwBase + 0x3C));};
		inline int iItemN(){return *(DWORD*)(dwBase + 0x40);};
		inline int iItemDur(){return *(DWORD*)(dwBase + 0x44);};
		inline char* strItemName(){return (LPSTR)(*(DWORD*)(*(DWORD*)(dwBase + 0x38) + 0xC));};
		inline bool IsFree(){if(dwBase) return false; else return true;};
		inline void SetDurability(int iDur){*(DWORD*)(dwBase + 0x44) = iDur;};
		//Default
		CInvSlot(int iSlot);
		CInvSlot(){dwBase = 0; iCurrentSlot = -1;};
};

class CInnSlot{
	private:
		DWORD dwBase;
		int iCurrentSlot;
	public:
		//General
		inline int GetCurrentSlot(){return iCurrentSlot;};
		void SelectSlot(int iSlot);
		inline DWORD GetBase(){return dwBase;};
		inline int dwItemNum(){return *(DWORD*)(*(DWORD*)(dwBase + 0x38)) + *(DWORD*)(*(DWORD*)(dwBase + 0x3C));};
		inline int iItemN(){return *(DWORD*)(dwBase + 0x40);};
		inline int iItemDur(){return *(DWORD*)(dwBase + 0x44);};
		inline char* strItemName(){return (LPSTR)(*(DWORD*)(*(DWORD*)(dwBase + 0x38) + 0xC));};
		inline bool IsFree(){if(dwBase) return false; else return true;};
		//Default
		CInnSlot(int iSlot);
		CInnSlot(){dwBase = 0; iCurrentSlot = -1;};
};

class CBag{
	public:
		//General
		inline void SlotToPaged(int iSlot, int &outPage, int &outSlot){outPage = iSlot / iMaxInnPageSlot; outSlot = iSlot % iMaxInnPageSlot;};
		inline int PagedToSlot(int iPage, int iSlot){return (iPage * iMaxInnPageSlot) + iSlot;};
		//Inventory
		int FindInvItem(int iBegin, int dwNum);
		int FindFreeInvSlot(int iBegin);
		DWORD GetItemBase(DWORD dwItemNum);
		DWORD GetItemExtBase(DWORD dwItemNum, BYTE bExtIndex);
		//Inn
		int FindInnItem(int iBegin, int dwNum);
		int FindFreeInnSlot(int iBegin);
		//Default
		CBag(){};
};

class CTrap{
	private:
		HANDLE hThread;
		DWORD dwThread;
		bool bState;

		inline static void _StartThread(void* param){((CTrap*)param)->Function();};
		void Function();
	public:
		//General
		inline bool IsSwitched(){return bState;};
		bool Switch(bool bOnOff);
		//Default
		CTrap(){bState = false;};
};

class CParty{
	private:
		int iCount;
		DWORD dwUser[8];
	public:
		//General
		void Refresh();
		inline DWORD GetUser(int iIndex){return dwUser[iIndex];};
		inline int GetCount(){return iCount;};
		bool CheckInParty(DWORD dwUserID);
		//Default
		CParty(){iCount = 0;};
};

class CRecvFunction{
	private:
		BYTE bFuncIndex;
		HANDLE hMailSlot;
		HANDLE hThread;
		DWORD dwThread;
		LPVOID lFuncAdr;
		DWORD dwOldAdr;
		bool bState;
		bool bPaused;

		inline static void _StartThread(void* param){((CRecvFunction*)param)->ExecuteMessages();};

		//Message Variables
		int iLastChestX;
		int iLastChestY;
		DWORD dwLastChestID;

	public:
		//General
		void ExecuteMessages();
		BYTE GetIndex();
		inline bool GetState(){return bState;};
		bool StartFunction();
		void StopFunction();
		void Pause(){bPaused=true;};
		void Resume(){bPaused=false;};
		//Message Functions
		void ChestDrop(char* pBuf);
		void ItemDrop(char* pBuf);
		bool PartyRecv(char* pBuf);
		void ChatRecv(char* pBuf);
		void ExpRecv(char* pBuf);
		bool TradeRecv(char* pBuf);
		bool CombatRecv(char* pBuf);
		bool DeadRecv(char* pBuf);
		void MoveRecv(char* pBuf);
		//Message Variables
		bool bOnLootWay;
		//Default
		CRecvFunction(){bFuncIndex = -1; hMailSlot = 0; lFuncAdr = 0; bState = false; bPaused = false; bOnLootWay = false;};
};

class CSkillIcon{
	private:
	DWORD dwBase;
	int iCurrentKind;
	int iCurrentPage;
	int iCurrentSlot;

	public:
	inline DWORD GetBase(){return dwBase;};
	inline int GetKind(){return iCurrentKind;};
	inline int GetPage(){return iCurrentPage;};
	inline int GetSlot(){return iCurrentSlot;};
	void SetSlot(int iKind, int iPage, int iSlot);
	inline DWORD dwSkillNum(){return *(DWORD*)(*(DWORD*)(dwBase + 0x38));};
	inline char* strSkillName(){return (LPSTR)(*(DWORD*)(*(DWORD*)(dwBase + 0x38) + 0x18));};
	inline char* strSkillDesc(){return (LPSTR)(*(DWORD*)(*(DWORD*)(dwBase + 0x38) + 0x28));};
	inline bool isEmpty(){return (dwBase == 0);};

	CSkillIcon(int iKind, int iPage, int iSlot);
	CSkillIcon(){dwBase = 0; iCurrentKind = -1; iCurrentPage = -1; iCurrentSlot = -1;};
};

class CRecv{
	public:
	BYTE bFuncIndex;
	DWORD dwGameFunction;
	bool bState;
	bool bPaused;
	BYTE GetIndex();
	//void ProcessRecv(DWORD DataPack, int& iOffset);
	bool Enable();
	void Disable();

	CRecv(){bFuncIndex = -1; dwGameFunction = 0; bState = false; bPaused = false;}
};

class CTest{
	public:

	float GetPosZ(float fPosX, float fPosY);
	void InitPos(float fPosX, float fPosY, float fPosZ);
	void InitPos2(float fPosX, float fPosY, float fPosZ);
	DWORD GetSkillBase(DWORD dwSkillNum);
	bool CheckSkillAvailable(DWORD dwSkillNum);
	void CommandSit(bool bSit);
	bool CastMagic(int iCastID, int iTargetID);
	DWORD PickChar(int iScreenX, int iScreenY, float &fRealX, float &fRealY, float &fRealZ);

	CTest(){};
};

////////////////////////////////////////
//***** GLOBAL CLASS DEFINITIONS *****//
////////////////////////////////////////

extern CGeneral			_CGeneral;
extern CCharBase		_CCharBase;
extern CPacket			_CPacket;
extern CArea			_CArea;
extern CMath			_CMath;
extern CCast			_CCast;
extern CBag				_CBag;
extern CTrap			_CTrap;
extern CParty			_CParty;
extern CRecvFunction	_CRecvFunction;
extern CTest			_CTest;
extern CRecv			_CRecv;
//---------------------------------------------------------------------------


#endif
