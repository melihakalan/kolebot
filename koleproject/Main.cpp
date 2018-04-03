//---------------------------------------------------------------------------

#pragma hdrstop

#include "Main.h"

CGeneral		_CGeneral;
CCharBase		_CCharBase;
CPacket			_CPacket;
CArea			_CArea;
CMath			_CMath;
CCast			_CCast;
CBag			_CBag;
CTrap			_CTrap;
CParty			_CParty;
CRecvFunction	_CRecvFunction;
CTest			_CTest;
CRecv			_CRecv;

//---------------------------------------------------------------------------

#pragma package(smart_init)

//***** CGeneral *****//

bool CGeneral::ConnectServer(char* strIP, int iPort){
LPVOID tmpIP = strIP;
int tmpPort = iPort;
DWORD dwRet = 0;
__asm{
MOV EDX,[KO_SCK2]
MOV ECX,[KO_SOCK]
PUSH tmpPort
PUSH tmpIP
PUSH EDX
MOV EAX,KO_CONN
CALL EAX
MOV dwRet,EAX
}
if(dwRet)
	return false;
return true;
}

void CGeneral::DisconnectServer(){
DWORD CallAddr = KO_DISC;
__asm{
MOV ECX,[KO_SOCK]
CALL [&CallAddr]
MOV ECX,[KO_SOCK+4]
CALL [&CallAddr]
}
}

void CGeneral::PostMsgBox(char* strMsg, BYTE bButton, BYTE bFunction){
if(strMsg == 0)
	return;
int msgLen = strlen(strMsg);
char *alloc = new char[msgLen+96+1];
memset(alloc,0,msgLen+96+1);
memcpy(alloc+96,strMsg,msgLen);
*(DWORD*)(alloc+36) = (DWORD)(alloc+96);
*(DWORD*)(alloc+40) = msgLen;
DWORD Addr1 = (DWORD)(alloc);
DWORD Addr2 = (DWORD)(alloc+32);
DWORD Addr3 = (DWORD)(alloc+64);
__asm{
XOR EAX,EAX
PUSH 0
MOV AL,bFunction
PUSH EAX
MOV AL,bButton
PUSH EAX
PUSH Addr1
PUSH Addr2
PUSH Addr3
MOV EAX,KO_MSBX
CALL EAX
ADD ESP,0x18
}
delete [] alloc;
}

void CGeneral::PostMsgOut(char* strMsg, DWORD dwColor){
if(strMsg == 0)
	return;
int msgLen = strlen(strMsg);
char *alloc = new char[msgLen+32+1];
memset(alloc,0,msgLen+32+1);
memcpy(alloc+32,strMsg,msgLen);
*(DWORD*)(alloc+4) = (DWORD)(alloc+32);
*(DWORD*)(alloc+8) = msgLen;
DWORD Addr1 = (DWORD)(alloc);
__asm{
MOV ECX,[KO_DLGA]
MOV ECX,[ECX+OFF_MSG]
PUSH dwColor
PUSH Addr1
MOV EAX,KO_MSOP
CALL EAX
//ADD ESP,8
}
delete [] alloc;
}

void CGeneral::PassEncrypt(char* strPass, char* pEncBuffer){
DWORD TmpECX = 0x9BF9C8;
LPVOID InBuffer = strPass;
LPVOID OutBuffer = pEncBuffer;
__asm{
	PUSH OutBuffer
	PUSH 1000
	PUSH InBuffer
	LEA ECX,&TmpECX
	MOV EAX,0x854520
	CALL EAX
}
}

//***** CCharBase *****//

bool CCharBase::IsEnemy(){
bool bRet;
DWORD CharBase = this->GetBase();
__asm{
MOV ECX,[KO_CHAR]
PUSH CharBase
MOV EAX,KO_ISEN
CALL EAX
MOV bRet,AL
}
return bRet;
}

//***** CPacket *****//

void CPacket::PSend2(BYTE* pBuf,size_t szBuf){
//melih
DWORD* ptrTHRD = (DWORD*)KO_THRD;
DWORD tmpThread 	= *ptrTHRD;
*ptrTHRD = GetCurrentThreadId();
LPVOID Buffer = pBuf;
int Size = szBuf;
DWORD CallAddr = *(DWORD*)((GetIndex() * 4) + *(DWORD*)KO_SOCK + 0x40064);
__asm{
MOV ECX,[KO_SOCK]
PUSH Size
PUSH Buffer
MOV EAX,CallAddr
CALL EAX
}
*ptrTHRD = tmpThread;
*(BYTE*)KO_PFIX = 0;
AddPCount();
}

__declspec(naked) __stdcall void CPacket::PSend(BYTE* pBuf,size_t szBuf){
	//SendFunction(pBuf,szBuf);	//virtual
	__asm
	{
		POP EAX
		XCHG [ESP],EAX
		XOR EAX,EAX
		PUSH dwSendPointer
		SHL [ESP],1
		POP EAX
		PUSH EAX
		SHL [ESP],2
		ADD EAX,[ESP]
		XCHG [ESP],EAX
		PUSH dwSendPointerAdd
		POP EAX
		NOT EAX
		AND EAX,0xFF
		ADD [ESP],EAX
		MOV bSendOwnCall,1
		RET
	}
}

bool CPacket::PatchGameFunction(){
	DWORD dwCallAddr = (GetIndex() * 4) + *(DWORD*)KO_SOCK + 0x40064;
	dwGameFunction = *(DWORD*)dwCallAddr;
	*(DWORD*)dwCallAddr = (DWORD)&SendFunction;
	return true;
}

void CPacket::RepairGameFunction(){
	if(dwGameFunction == NULL)
		return;

	DWORD dwCallAddr = (GetIndex() * 4) + *(DWORD*)KO_SOCK + 0x40064;
	*(DWORD*)dwCallAddr = dwGameFunction;
}

void CPacket::PatchHooked(){
	DWORD dwCallAddr = (GetIndex() * 4) + *(DWORD*)KO_SOCK + 0x40064;
	dwGameFunction = *(DWORD*)dwCallAddr;

	DWORD dwTemp = *(DWORD*)KO_SOCK + 0x40064;

	for (int i = 0; i < 10; i++)
	{
		*(DWORD*)(dwTemp + (i * 4)) = (DWORD)&HookedSendFunction;
	}
}

BYTE CPacket::GetIndex(){
DWORD Idx = 0;
__asm{
LEA EAX,&Idx
PUSH EAX
PUSH 4
PUSH KO_SDFX
PUSH 2
MOV EAX,KO_INDX
CALL EAX
ADD ESP,0x10
}
bFuncIndex = (BYTE)Idx;
return (BYTE)Idx;
}

//***** CArea *****//

bool CArea::CheckGM(){
DWORD dwTmpBase	= *(DWORD*)(KO_AREA);
DWORD dwStart	= *(DWORD*)(dwTmpBase + 0x3C);
DWORD dwBase;

dwTmpBase = *(DWORD*)(dwStart);

LOOP:
if(dwTmpBase == dwStart)
	goto RETN;

dwBase = *(DWORD*)(dwTmpBase + 0x10);
if(dwBase){
	if(*(DWORD*)(dwBase + KO_AUTH) == 0)
		return true;
}

NEXT:
dwTmpBase = NextUPC(dwTmpBase);
goto LOOP;

RETN:
return false;
}

int CArea::GetUPCCount(){
int iCount = 0;

DWORD dwTmpBase	= *(DWORD*)(KO_AREA);
DWORD dwStart	= *(DWORD*)(dwTmpBase + 0x3C);
DWORD dwBase;

dwTmpBase = *(DWORD*)(dwStart);

LOOP:
if(dwTmpBase == dwStart)
	goto RETN;

dwBase = *(DWORD*)(dwTmpBase + 0x10);
if(dwBase)
	iCount += 1;

NEXT:
dwTmpBase = NextUPC(dwTmpBase);
goto LOOP;

RETN:
return iCount;
}

int CArea::GetNPCCount(){
int iCount = 0;

DWORD dwTmpBase	= *(DWORD*)(KO_AREA);
DWORD dwStart	= *(DWORD*)(dwTmpBase + 0x2C);
DWORD dwBase;

dwTmpBase = *(DWORD*)(dwStart);

LOOP:
if(dwTmpBase == dwStart)
	goto RETN;

dwBase = *(DWORD*)(dwTmpBase + 0x10);
if(dwBase)
	iCount += 1;

NEXT:
dwTmpBase = NextNPC(dwTmpBase);
goto LOOP;

RETN:
return iCount;
}

DWORD CArea::GetUPCBase(WORD wID){
DWORD dwBase;
DWORD dwID = (DWORD)wID;
__asm{
MOV ECX,[KO_AREA]
PUSH 1
PUSH dwID
MOV EAX,KO_FPBS
CALL EAX
MOV dwBase,EAX
}
return dwBase;
}

DWORD CArea::GetNPCBase(WORD wID){
DWORD dwBase;
DWORD dwID = (DWORD)wID;
__asm{
MOV ECX,[KO_AREA]
PUSH 1
PUSH dwID
MOV EAX,KO_FMBS
CALL EAX
MOV dwBase,EAX
}
return dwBase;
}

DWORD CArea::GetCorpseBase(WORD wID){
DWORD dwBase = 0;
DWORD dwID = (DWORD)wID;
__asm{
LEA EAX,&dwID
PUSH EAX
LEA EAX,&dwBase
PUSH EAX
MOV ECX,[KO_AREA]
LEA ECX,[ECX + 0x48]
MOV EAX,KO_NPCB
CALL EAX
}
if(!dwBase)
	return 0;
if(dwBase == *(DWORD*)(*(DWORD*)(KO_AREA) + 0x4C))
	return 0;
return *(DWORD*)(dwBase + 0x10);
}

DWORD CArea::GetAnyBase(WORD wID){
DWORD dwBase;

if(!wID || wID == 0xFFFF)
	return 0;

if(wID < 10000)
	dwBase = GetUPCBase(wID);
else
	dwBase = GetNPCBase(wID);

if(!dwBase)
	dwBase = GetCorpseBase(wID);

return dwBase;
}

DWORD CArea::GetZEnemyBase(){
DWORD dwBase;
__asm{
MOV EAX,[KO_CHAR]
ADD EAX,KO_POSX
MOV ECX,[KO_AREA]
PUSH EAX
PUSH 0
MOV EAX,KO_FPOZ
CALL EAX
MOV dwBase,EAX
}
if(dwBase) dwLastNearstBase = dwBase;
return dwBase;
}

DWORD CArea::GetNPCByName(int iNameCount, char* ptrName[], char* sName, bool bNearst, bool bEnemy, bool bAlive, FLOAT fMaxDist, bool bLoop){
if(!iNameCount) return 0;

SIZE_T iNameLen;				//for single name
SIZE_T* ptrNameLen = NULL;		//for multi names
bool bSingleName = false;

if(ptrName){
	ptrNameLen = new SIZE_T[iNameCount];
	for (int i = 0; i < iNameCount; i++) {
		if(ptrName[i])
			ptrNameLen[i] = strlen(ptrName[i]);
		else
			ptrNameLen[i] = 0;
	}
}
else
{
	if(!sName)	return 0;
	iNameLen = strlen(sName);
	bSingleName = true;
}

DWORD dwTmpBase,dwStart,dwRetBase;
FLOAT fRetDist,pDist;
CCharBase pNPC;
SIZE_T iCurLen;
DWORD TmpVal;

BEGIN:
	dwTmpBase	= *(DWORD*)(KO_AREA);
	dwStart	= *(DWORD*)(dwTmpBase + 0x2C);
	dwRetBase	= 0;
	fRetDist;

	dwTmpBase = *(DWORD*)(dwStart);

LOOP:
	if(dwTmpBase == dwStart){
		if(bLoop && !dwRetBase)
			goto BEGIN;
		goto RETN;
	}

	pNPC = *(DWORD*)(dwTmpBase + 0x10);
	if(!pNPC.GetBase())
		goto NEXT;

	if(bAlive){
		if( !pNPC.IsAlive() )
			goto NEXT;
	}

	if(bEnemy){
		/*TmpVal = pNPC.GetDWORD(0x93C);
		if(TmpVal){
			if(*(BYTE*)(TmpVal + 0xA) == 1 || *(BYTE*)(TmpVal + 0xA) == 3 || *(BYTE*)(TmpVal + 0xA) == 4)
				goto NEXT;
		} */

		if( !pNPC.IsEnemy() )
			goto NEXT;
	}

	pDist = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pNPC.GetFLOAT(KO_POSX),pNPC.GetFLOAT(KO_POSY));

	if(fMaxDist){
		if(pDist > fMaxDist)
			goto NEXT;
	}

	if(pNPC.GetName())
		iCurLen = strlen(pNPC.GetName());
	else
		iCurLen = 0;

	if(bSingleName){
	if(iNameLen != iCurLen)
		goto NEXT;

	if(strcmp(sName,pNPC.GetName()) != 0)
		goto NEXT;

	if(bNearst){
		if(dwRetBase){
			if(pDist < fRetDist){
				dwRetBase = pNPC.GetBase();
				fRetDist = pDist;
			}
		}else{
			dwRetBase = pNPC.GetBase();
			fRetDist = pDist;
		}
	}else{
	dwRetBase = pNPC.GetBase();
	goto RETN;
	}

	}else{
		for (int i = 0; i < iNameCount; i++) {
			if(ptrNameLen[i] != iCurLen)
				continue;

			if(strcmp(ptrName[i],pNPC.GetName()) != 0)
				continue;

				if(bNearst){
					if(dwRetBase){
						if(pDist < fRetDist){
							dwRetBase = pNPC.GetBase();
							fRetDist = pDist;
						}
					}else{
						dwRetBase = pNPC.GetBase();
						fRetDist = pDist;
					}
				}else{
					dwRetBase = pNPC.GetBase();
					goto RETN;
				}

			break;
		}
	}

NEXT:
	dwTmpBase = NextNPC(dwTmpBase);
	goto LOOP;

RETN:
	if(ptrName)
		delete [] ptrNameLen;
	if(dwRetBase)
		dwLastNearstBase = dwRetBase;
	return dwRetBase;
}

DWORD CArea::GetUPCByName(int iNameCount, char* ptrName[], char* sName, bool bAlive, FLOAT fMaxDist, bool bNotInParty,bool bEnemy){
if(!iNameCount) return 0;

SIZE_T iNameLen;				//for single name
SIZE_T* ptrNameLen = NULL;		//for multi names
bool bSingleName = false;

if(ptrName){
	ptrNameLen = new SIZE_T[iNameCount];
	for (int i = 0; i < iNameCount; i++) {
		if(ptrName[i])
			ptrNameLen[i] = strlen(ptrName[i]);
		else
			ptrNameLen[i] = 0;
	}
}
else
{
	if(!sName)	return 0;
	iNameLen = strlen(sName);
	bSingleName = true;
}

DWORD dwTmpBase,dwStart,dwRetBase;
FLOAT fRetDist,pDist;
CCharBase pUPC;
SIZE_T iCurLen;

//party
DWORD dwUser;
int iPartyCount;

BEGIN:
	dwTmpBase	= *(DWORD*)(KO_AREA);
	dwStart	= *(DWORD*)(dwTmpBase + 0x3C);
	dwRetBase	= 0;
	fRetDist;

	dwTmpBase = *(DWORD*)(dwStart);

LOOP:
	if(dwTmpBase == dwStart)
		goto RETN;

	pUPC = *(DWORD*)(dwTmpBase + 0x10);
	if(!pUPC.GetBase())
		goto NEXT;

	if(bAlive){
		if(!pUPC.IsAlive())
			goto NEXT;
	}

	if(bEnemy){
		/*TmpVal = pNPC.GetDWORD(0x93C);
		if(TmpVal){
			if(*(BYTE*)(TmpVal + 0xA) == 1 || *(BYTE*)(TmpVal + 0xA) == 3 || *(BYTE*)(TmpVal + 0xA) == 4)
				goto NEXT;
		} */

		if( !pUPC.IsEnemy() )
			goto NEXT;
	}

	pDist = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pUPC.GetFLOAT(KO_POSX),pUPC.GetFLOAT(KO_POSY));

	if(fMaxDist){
		if(pDist > fMaxDist)
			goto NEXT;
	}

	if(bNotInParty){
		_CParty.Refresh();
		iPartyCount = _CParty.GetCount();
		if(iPartyCount >= 2){
			for (int i = 0; i < iPartyCount; i++) {
				dwUser = _CParty.GetUser(i);
				if(dwUser){
					if(*(DWORD*)(dwUser + KO_PT_ID) == (DWORD) pUPC.GetID())
						goto NEXT;
				}
			}
		}
	}

	if(pUPC.GetName())
		iCurLen = strlen(pUPC.GetName());
	else
    	iCurLen = 0;

	if(bSingleName){
	if(iNameLen != iCurLen)
		goto NEXT;

	if(strcmp(sName,pUPC.GetName()) != 0)
		goto NEXT;

	dwRetBase = pUPC.GetBase();
	goto RETN;

	}else{
		for (int i = 0; i < iNameCount; i++) {
			if(ptrNameLen[i] != iCurLen)
				continue;

			if(strcmp(ptrName[i],pUPC.GetName()) != 0)
				continue;

			dwRetBase = pUPC.GetBase();
			goto RETN;
		}
	}

NEXT:
	dwTmpBase = NextUPC(dwTmpBase);
	goto LOOP;

RETN:
	if(ptrName)
		delete [] ptrNameLen;
	return dwRetBase;
}

DWORD CArea::GetNearstUPC(bool bEnemy,bool bAlive,FLOAT fMaxDist){
DWORD dwTmpBase,dwStart,dwRetBase;
FLOAT fRetDist,pDist;
CCharBase pUPC;
DWORD TmpVal;

BEGIN:
	dwTmpBase	= *(DWORD*)(KO_AREA);
	dwStart	= *(DWORD*)(dwTmpBase + 0x3C);
	dwRetBase	= 0;
	fRetDist;

	dwTmpBase = *(DWORD*)(dwStart);

LOOP:
	if(dwTmpBase == dwStart)
		goto RETN;

	pUPC = *(DWORD*)(dwTmpBase + 0x10);
	if(!pUPC.GetBase())
		goto NEXT;

	if(bAlive){
		if( !pUPC.IsAlive() )
			goto NEXT;
	}

	if(bEnemy){
		/*TmpVal = pNPC.GetDWORD(0x93C);
		if(TmpVal){
			if(*(BYTE*)(TmpVal + 0xA) == 1 || *(BYTE*)(TmpVal + 0xA) == 3 || *(BYTE*)(TmpVal + 0xA) == 4)
				goto NEXT;
		} */

		if( !pUPC.IsEnemy() )
			goto NEXT;
	}

	pDist = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pUPC.GetFLOAT(KO_POSX),pUPC.GetFLOAT(KO_POSY));

	if(fMaxDist){
		if(pDist > fMaxDist)
			goto NEXT;
	}

	if(dwRetBase){
		if(pDist < fRetDist){
			dwRetBase = pUPC.GetBase();
			fRetDist = pDist;
		}
	}else{
		dwRetBase = pUPC.GetBase();
		fRetDist = pDist;
	}

NEXT:
	dwTmpBase = NextUPC(dwTmpBase);
	goto LOOP;

RETN:
	return dwRetBase;
}

void CArea::SelectObject(CCharBase Object){
	DWORD dwBase = Object.GetBase();
	__asm
	{
		MOV ECX,[KO_DLGA]
		PUSH dwBase
		MOV EAX,KO_SETM
		CALL EAX
	}
}

DWORD CArea::GetNearstEnemy(int iLevel, bool bAlive, FLOAT fMaxDist){

	//UPC
	DWORD dwTmpBase	= *(DWORD*)(KO_AREA);
	DWORD dwStart	= *(DWORD*)(dwTmpBase + 0x3C);
	DWORD dwRetBase	= 0;
	FLOAT fRetDist,pDist;
	CCharBase pUPC = 0;
	CCharBase pNPC = 0;

	dwTmpBase = *(DWORD*)(dwStart);

	bool bNext = false;

	while(dwTmpBase != dwStart){

		if(bNext)
			dwTmpBase = NextUPC(dwTmpBase);

		if(dwTmpBase == dwStart)
			continue;	//break

		bNext = true;	//begin

		pUPC = *(DWORD*)(dwTmpBase + 0x10);

		if(pUPC.GetBase() == 0)
			continue;

		if(iLevel){
			if(pUPC.GetDWORD(KO_LEVL) != (DWORD)iLevel)
				continue;
		}

		if(bAlive){
			if(pUPC.IsAlive() == false)
				continue;
		}

		if(pUPC.IsEnemy() == false)
			continue;

		pDist = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pUPC.GetFLOAT(KO_POSX),pUPC.GetFLOAT(KO_POSY));

		if(fMaxDist){
			if(pDist > fMaxDist)
				continue;
		}

		if(dwRetBase){
			if(pDist < fRetDist){
				dwRetBase = pUPC.GetBase();
				fRetDist = pDist;
			}
		}else{
			dwRetBase = pUPC.GetBase();
			fRetDist = pDist;
		}

	}

	//NPC
	bNext = false;	//reset
	dwTmpBase	= *(DWORD*)(KO_AREA);
	dwStart	= *(DWORD*)(dwTmpBase + 0x2C);
	dwTmpBase = *(DWORD*)(dwStart);

	while(dwTmpBase != dwStart){

		if(bNext)
			dwTmpBase = NextNPC(dwTmpBase);

		if(dwTmpBase == dwStart)
			continue;	//break

		bNext = true;	//begin

		pNPC = *(DWORD*)(dwTmpBase + 0x10);

		if(pNPC.GetBase() == 0)
			continue;

		if(iLevel){
			if(pNPC.GetDWORD(KO_LEVL) != (DWORD)iLevel)
				continue;
		}

		if(bAlive){
			if(pNPC.IsAlive() == false)
				continue;
		}

		if(pNPC.IsEnemy() == false)
			continue;

		pDist = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pNPC.GetFLOAT(KO_POSX),pNPC.GetFLOAT(KO_POSY));

		if(fMaxDist){
			if(pDist > fMaxDist)
				continue;
		}

		if(dwRetBase){
			if(pDist < fRetDist){
				dwRetBase = pNPC.GetBase();
				fRetDist = pDist;
			}
		}else{
			dwRetBase = pNPC.GetBase();
			fRetDist = pDist;
		}

	}

	return dwRetBase;
}

void CArea::LootChaosCubes(){

	BYTE Packet[8] = {0x20,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

	DWORD dwTmpBase	= *(DWORD*)(KO_AREA);
	DWORD dwStart	= *(DWORD*)(dwTmpBase + 0x2C);
	FLOAT pDist;
	CCharBase pNPC = 0;

	dwTmpBase = *(DWORD*)(dwStart);

	bool bNext = false;

	while(dwTmpBase != dwStart)
	{
		if(bNext)
			dwTmpBase = NextNPC(dwTmpBase);

		if(dwTmpBase == dwStart)
			continue;	//break

		bNext = true;	//begin

		pNPC = *(DWORD*)(dwTmpBase + 0x10);

		if(pNPC.GetBase() == 0)
			continue;

		if(pNPC.GetDWORD(KO_LEVL) != 100)	//cube level
			continue;

		pDist = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pNPC.GetFLOAT(KO_POSX),pNPC.GetFLOAT(KO_POSY));

		if(pDist >= 3.0)
			continue;

		*(WORD*)(Packet + 2) = pNPC.GetID();
		_CPacket.PSend(Packet,8);
	}


}

DWORD CArea::GetNearstNPCByID(int iIDArray[],int iIDCount, bool bAlive, FLOAT fMaxDist)
{

	if( !iIDCount )
		return 0;

	CCharBase pNPC;
	CCharBase pReturn = 0;
	float fPrevDist = 0, fCurDist = 0;

	float fMyX = _CCharBase.GetFLOAT(KO_POSX);
	float fMyY = _CCharBase.GetFLOAT(KO_POSY);

	for (int i = 0; i < iIDCount; i++)
	{
		if( !iIDArray[i] )
			continue;

		pNPC = _CArea.GetNPCBase( iIDArray[i] );

		if( !pNPC.GetBase() )
			continue;

		if( bAlive )
		{
			if( !pNPC.IsAlive() )
				continue;
		}

		fCurDist = _CMath.fDist(fMyX,fMyY,pNPC.GetFLOAT(KO_POSX),pNPC.GetFLOAT(KO_POSY));

		if( fMaxDist && fCurDist > fMaxDist )
        	continue;

		if( !pReturn.GetBase() )
		{
			pReturn = pNPC.GetBase();
			fPrevDist = fCurDist;
		}
		else
		{
			if( fCurDist < fPrevDist )
			{
				pReturn = pNPC.GetBase();
				fPrevDist = fCurDist;
            }
        }
	}

	return pReturn.GetBase();
}

DWORD CArea::NextNPC(DWORD dwOldBase){
	DWORD dwBase = dwOldBase;
	__asm{
		LEA ECX,&dwBase
		MOV EAX,KO_NXTM
		CALL EAX
	}
	return dwBase;
}

DWORD CArea::NextUPC(DWORD dwOldBase){
	DWORD dwBase = dwOldBase;
	__asm{
		LEA ECX,&dwBase
		MOV EAX,KO_NXTP
		CALL EAX
	}
	return dwBase;
}

//***** CMath *****//

FLOAT CMath::fDist(FLOAT X1, FLOAT Y1, FLOAT X2, FLOAT Y2){
return pow((double)((X2-X1)*(X2-X1)) + (double)((Y2-Y1)*(Y2-Y1)),0.5) / 4;
}
FLOAT CMath::iPerc(int HiVal, int LoVal){

FLOAT fHiVal = (float)HiVal;
FLOAT fLoVal = (float)LoVal;

return (100.0 / (fHiVal / fLoVal));
}

//***** CCast *****//

int CCast::CastCount(){
DWORD TMP = *(DWORD*)(KO_DLGA);
TMP = *(DWORD*)(TMP+OFF_STATEBAR);
TMP = *(DWORD*)(TMP+0x4);
return *(DWORD*)(TMP+0xE4);
}

int CCast::CastNum(int iNo){
DWORD TMP = *(DWORD*)(KO_DLGA);
TMP = *(DWORD*)(TMP+OFF_STATEBAR);
TMP = *(DWORD*)(TMP+0x4);
TMP = *(DWORD*)(TMP+0xE0);
do{
TMP = *(DWORD*)(TMP);
iNo--;
}while(iNo > -1);
TMP = *(DWORD*)(TMP+0x8);
if(TMP)
	return *(DWORD*)(TMP);
return 0;
}

int CCast::MakeCastNum(CCharBase dwBase, char* strTail){
int iClass = dwBase.GetDWORD(KO_CLSS);

char temp[7]; memset(temp,0,7);
itoa(iClass,temp,10);
strcat(temp,strTail);
return atoi(temp);
}

bool CCast::IsCasted(int Num){
int count = CastCount();
if(!count) return false;

for (int i=0; i < count; i++) {
if(CastNum(i)==Num) return true;
}
return false;
}

//***** CInvSlot *****//

CInvSlot::CInvSlot(int iSlot){
if(iSlot == -1){
dwBase = 0;
return;
}
DWORD Tmp = *(DWORD*)(KO_DLGA);
Tmp = *(DWORD*)(Tmp + OFF_INVENTORY);
Tmp = *(DWORD*)(Tmp + (0x148 + (4 * (iSlot + 26))));
dwBase = Tmp;
iCurrentSlot = iSlot;
}

void CInvSlot::SelectSlot(int iSlot){
if(iSlot == -1){
dwBase = 0;
return;
}
DWORD Tmp = *(DWORD*)(KO_DLGA);
Tmp = *(DWORD*)(Tmp + OFF_INVENTORY);
Tmp = *(DWORD*)(Tmp + (0x148 + (4 * (iSlot + 26))));
dwBase = Tmp;
iCurrentSlot = iSlot;
}

void CInvSlot::SelectLeft(){
DWORD Tmp = *(DWORD*)(KO_DLGA);
Tmp = *(DWORD*)(Tmp + OFF_INVENTORY);
Tmp = *(DWORD*)(Tmp + 0x198);
dwBase = Tmp;
iCurrentSlot = -1;
}

void CInvSlot::SelectRight(){
DWORD Tmp = *(DWORD*)(KO_DLGA);
Tmp = *(DWORD*)(Tmp + OFF_INVENTORY);
Tmp = *(DWORD*)(Tmp + 0x190);
dwBase = Tmp;
iCurrentSlot = -1;
}

void CInvSlot::SelectHelmet(){
DWORD Tmp = *(DWORD*)(KO_DLGA);
Tmp = *(DWORD*)(Tmp + OFF_INVENTORY);
Tmp = *(DWORD*)(Tmp + 0x178 + 4);
dwBase = Tmp;
iCurrentSlot = -1;
}

void CInvSlot::SelectPauldron(){
DWORD Tmp = *(DWORD*)(KO_DLGA);
Tmp = *(DWORD*)(Tmp + OFF_INVENTORY);
Tmp = *(DWORD*)(Tmp + 0x178 + 16);
dwBase = Tmp;
iCurrentSlot = -1;
}

void CInvSlot::SelectPads(){
DWORD Tmp = *(DWORD*)(KO_DLGA);
Tmp = *(DWORD*)(Tmp + OFF_INVENTORY);
Tmp = *(DWORD*)(Tmp + 0x178 + 40);
dwBase = Tmp;
iCurrentSlot = -1;
}

void CInvSlot::SelectGloves(){
DWORD Tmp = *(DWORD*)(KO_DLGA);
Tmp = *(DWORD*)(Tmp + OFF_INVENTORY);
Tmp = *(DWORD*)(Tmp + 0x178 + 48);
dwBase = Tmp;
iCurrentSlot = -1;
}

void CInvSlot::SelectBoots(){
DWORD Tmp = *(DWORD*)(KO_DLGA);
Tmp = *(DWORD*)(Tmp + OFF_INVENTORY);
Tmp = *(DWORD*)(Tmp + 0x178 + 52);
dwBase = Tmp;
iCurrentSlot = -1;
}

//***** CInnSlot *****//

CInnSlot::CInnSlot(int iSlot){
if(iSlot == -1){
dwBase = 0;
return;
}
DWORD Tmp = *(DWORD*)(KO_DLGA);
Tmp = *(DWORD*)(Tmp + OFF_WAREHOUSE);
Tmp = *(DWORD*)(Tmp + ((iSlot * 4)+0xDC));
dwBase = Tmp;
iCurrentSlot = iSlot;
}

void CInnSlot::SelectSlot(int iSlot){
if(iSlot == -1){
dwBase = 0;
return;
}
DWORD Tmp = *(DWORD*)(KO_DLGA);
Tmp = *(DWORD*)(Tmp + OFF_WAREHOUSE);
Tmp = *(DWORD*)(Tmp + ((iSlot * 4)+0xDC));
dwBase = Tmp;
iCurrentSlot = iSlot;
}

//***** CBag *****//

int CBag::FindInvItem(int iBegin, int dwNum){
CInvSlot pSlot;
int iRetSlot = -1;

for (int i = iBegin; i < iMaxInventorySlot; i++) {
pSlot.SelectSlot(i);
if(pSlot.IsFree())
	continue;
if(pSlot.dwItemNum() == dwNum){
iRetSlot = i;
break;
}
}

return iRetSlot;
}

int CBag::FindFreeInvSlot(int iBegin){
CInvSlot pSlot;
int iRetSlot = -1;

for (int i = iBegin; i < iMaxInventorySlot; i++) {
pSlot.SelectSlot(i);
if(pSlot.IsFree()){
iRetSlot = i;
break;
}
}

return iRetSlot;
}

DWORD CBag::GetItemBase(DWORD dwItemNum){

	if(!dwItemNum)
		return 0;

	DWORD 	_dwItemNum			=	(dwItemNum / 1000) * 1000;
	DWORD 	_dwItemBase			=	0;
	DWORD 	dwTmp				=	*(DWORD*)(KO_ITPA);

	__asm{
		MOV ESI,dwTmp
		LEA ECX,[ESI+0x14]
		LEA EAX,&_dwItemNum
		PUSH EAX
		LEA EAX,&_dwItemBase
		PUSH EAX
		MOV EAX,KO_ITCA
		CALL EAX
	}

	if(!_dwItemBase)
		return 0;

	if(*(DWORD*)(dwTmp + 0x18) == _dwItemBase)
		return 0;

	_dwItemBase += 0x10;

	return _dwItemBase;
}

DWORD CBag::GetItemExtBase(DWORD dwItemNum, BYTE bExtIndex){

	if(!dwItemNum)
		return 0;

	DWORD 	_dwItemExtNum		= 	dwItemNum % 1000;
	DWORD 	_dwItemExtBase		=   0;
	DWORD	dwTmp				=	0;

	dwTmp = *(DWORD*)( (bExtIndex * 4) + KO_ITPB);

	__asm{
		MOV ESI,dwTmp
		LEA ECX,[ESI+0x14]
		LEA EAX,&_dwItemExtNum
		PUSH EAX
		LEA EAX,&_dwItemExtBase
		PUSH EAX
		MOV EAX,KO_ITCB
		CALL EAX
	}

	if(!_dwItemExtBase)
		return 0;

	if(*(DWORD*)(dwTmp + 0x18) == _dwItemExtBase)
		return 0;

	_dwItemExtBase += 0x10;
	return _dwItemExtBase;
}

int CBag::FindInnItem(int iBegin, int dwNum){
CInnSlot pSlot;
int iRetSlot = -1;

for (int i = iBegin; i < iMaxInnSlot; i++) {
pSlot.SelectSlot(i);
if(pSlot.IsFree())
	continue;
if(pSlot.dwItemNum() == dwNum){
iRetSlot = i;
break;
}
}

return iRetSlot;
}

int CBag::FindFreeInnSlot(int iBegin){
CInnSlot pSlot;
int iRetSlot = -1;

for (int i = iBegin; i < iMaxInnSlot; i++) {
pSlot.SelectSlot(i);
if(pSlot.IsFree()){
iRetSlot = i;
break;
}
}

return iRetSlot;
}

//***** CTrap *****//

bool CTrap::Switch(bool bOnOff){
if(bOnOff){
if(IsSwitched())
	return false;

hThread = CreateThread(0,0,(LPTHREAD_START_ROUTINE)_StartThread,this,0,&dwThread);
bState = true;
}else{
if(!IsSwitched())
	return false;

TerminateThread(hThread,0);
CloseHandle(hThread);
hThread=0;
dwThread=0;
bState = false;
}
return true;
}

void CTrap::Function(){
bool bTrapped;
DWORD WSOCKADR	=	(DWORD) GetProcAddress(GetModuleHandleA("WS2_32.dll"),"send");
DWORD SENDADR	=	*(DWORD*)(KO_SOCK) + (_CPacket.GetIndex() * 4) + 0x40064;
if(!WSOCKADR || !SENDADR)return;

LOOP:
bTrapped=false;
if(*(BYTE*)(KO_CRYP) != TRAP_bCryptByte) bTrapped=true;
if(*(BYTE*)(WSOCKADR) != TRAP_bWSockByte) bTrapped=true;
if(*(DWORD*)(SENDADR) >= TRAP_iMaxSendAdr) bTrapped=true;
if(!bTrapped){
Sleep(TRAP_iTrapDelay);
goto LOOP;
}
TerminateProcess(GetCurrentProcess(),0);
}

//***** CParty *****//

void CParty::Refresh(){
DWORD dwBase = *(DWORD*)(KO_PRTY);
dwBase = *(DWORD*)(dwBase + OFF_PARTY);

int UserCount = *(DWORD*)(dwBase + 0x374);
iCount = UserCount;

for (int n = UserCount; n < 8; n++) {
dwUser[n] = 0;
}

if(!UserCount)
	return;

dwBase = *(DWORD*)(dwBase + 0x370);
DWORD tmpUser;

for (int i = 0; i < UserCount; i++) {
tmpUser = dwBase;

int y = i;
do{
	tmpUser = *(DWORD*)(tmpUser);
	y--;
}while(y > -1);

dwUser[i] = tmpUser;
}

}

bool CParty::CheckInParty(DWORD dwUserID){

	_CParty.Refresh();
	int c = _CParty.GetCount();

	if(c < 2)
		return false;

	DWORD dwUser,dwID;

	for (int i = 0; i < c; i++)
	{
		dwUser = _CParty.GetUser(i);
		if(dwUser)
		{
			dwID = *(DWORD*)(dwUser + KO_PT_ID);
			if(dwID == dwUserID)
				return true;
        }
	}

	return false;
}

//***** CRecvFunction *****//

BYTE CRecvFunction::GetIndex(){
DWORD Idx = 0;
__asm{
LEA EAX,&Idx
PUSH EAX
PUSH 4
PUSH KO_RCFX
PUSH 1
MOV EAX,KO_INDX
CALL EAX
ADD ESP,0x10
}
bFuncIndex = (BYTE)Idx;
return (BYTE)Idx;
}

bool CRecvFunction::StartFunction(){

if(bState)
	return false;

BYTE FuncIdx = GetIndex();
DWORD _KO_RCVA = KO_RCVA + (FuncIdx * 4);
DWORD _KO_RCVB = *(DWORD*)(_KO_RCVA);
dwOldAdr = _KO_RCVB;

HMODULE hKernel32		= GetModuleHandleA("Kernel32.dll");
DWORD adrCreateFile		= (DWORD) GetProcAddress(hKernel32,"CreateFileA");
DWORD adrWriteFile		= (DWORD) GetProcAddress(hKernel32,"WriteFile");
DWORD adrCloseFile		= (DWORD) GetProcAddress(hKernel32,"CloseHandle");

LPVOID FuncAdr = VirtualAlloc(0,1024,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
if(!FuncAdr)return false;

char* RF_strMSName	= "//./mailslot/kojd";
size_t szBasePath = strlen(RF_strMSName);
srand(GetTickCount());
char pid[16];	itoa(rand() % 0xFFFF + 1,pid,10);
size_t szPid	=	strlen(pid);
char *MSlotName = new char[szBasePath+szPid+1];
memset(MSlotName,0,szBasePath+szPid+1);
memcpy(MSlotName,RF_strMSName,szBasePath);
strcat(MSlotName,pid);

HANDLE MSlotHandle = CreateMailslotA(MSlotName,0,0,0);
if((DWORD)(MSlotHandle)==0xFFFFFFFF){
VirtualFree(FuncAdr,0,MEM_RELEASE);
delete [] MSlotName;
return false;
}
hMailSlot	=	MSlotHandle;

DWORD tmpAddr = (DWORD)(FuncAdr)+0x400;
memcpy((LPVOID)(tmpAddr),MSlotName,strlen(MSlotName));

tmpAddr = (DWORD)(FuncAdr)+0x32A;
*(DWORD*)(tmpAddr) = adrCreateFile;

tmpAddr = (DWORD)(FuncAdr)+0x334;
*(DWORD*)(tmpAddr) = adrWriteFile;

tmpAddr = (DWORD)(FuncAdr)+0x33E;
*(DWORD*)(tmpAddr) = adrCloseFile;

tmpAddr = (DWORD)(FuncAdr)+0x208;
*(DWORD*)(tmpAddr) = _KO_RCVB;

tmpAddr = (DWORD)(FuncAdr)+0x212;
*(DWORD*)(tmpAddr) = _KO_RCVA;

tmpAddr = (DWORD)(FuncAdr)+0x21C;
*(DWORD*)(tmpAddr) = (DWORD)(FuncAdr);

BYTE PATCH[]={	0x52,0x89,0x0D,0,0,0,0,
				0x89,0x05,0,0,0,0,
				0x8B,0x4E,0x04,0x89,0x0D,0,0,0,0,
				0x8B,0x56,0x08,0x89,0x15,0,0,0,0,
				0x81,0xF9,0x00,0x10,0x00,0x00,0x7D,0x3E,0x50,0x68,0x80,0x00,0x00,0x00,0x6A,0x04,0x6A,0x00,0x6A,0x01,0x68,0x00,0x00,0x00,0x40,0x68,0,0,0,0,
				0xFF,0x15,0,0,0,0,
				0x83,0xF8,0xFF,0x74,0x1D,0x50,0x6A,0x00,0x54,0xFF,0x35,0,0,0,0,
				0xFF,0x35,0,0,0,0,
				0x50,0xFF,0x15,0,0,0,0,
				0xFF,0x15,0,0,0,0,
				0x8B,0x0D,0,0,0,0,
				0x8B,0x05,0,0,0,0,
				0x5A,0xFF,0x25,0,0,0,0,
};

tmpAddr = (DWORD)(FuncAdr)+0x320;
CopyBytes(PATCH+3,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x3B6;
CopyBytes(PATCH+9,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x1F4;
CopyBytes(PATCH+18,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x1FE;
CopyBytes(PATCH+27,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x400;
CopyBytes(PATCH+57,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x32A;
CopyBytes(PATCH+63,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x1F4;
CopyBytes(PATCH+78,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x1FE;
CopyBytes(PATCH+84,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x334;
CopyBytes(PATCH+91,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x33E;
CopyBytes(PATCH+97,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x320;
CopyBytes(PATCH+103,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x3B6;
CopyBytes(PATCH+109,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x208;
CopyBytes(PATCH+116,tmpAddr);

memcpy(FuncAdr,PATCH,sizeof(PATCH));

*(DWORD*)(_KO_RCVA)=(DWORD)(FuncAdr);

delete [] MSlotName;

lFuncAdr = FuncAdr;
bPaused = false;
bState = true;

//hThread = CreateThread(0,0,(LPTHREAD_START_ROUTINE)_StartThread,this,0,&dwThread);

return true;
}

void CRecvFunction::StopFunction(){
if(!bState)
	return;

DWORD _KO_RCVA = KO_RCVA + (bFuncIndex * 4);
*(DWORD*)(_KO_RCVA) = dwOldAdr;

/*if(hThread){
	TerminateThread(hThread,0);
	CloseHandle(hThread);
}*/

if(lFuncAdr)
	VirtualFree(lFuncAdr,0,MEM_RELEASE);

bState = false;
bPaused = false;
hThread = 0;
dwThread = 0;
CloseHandle(hMailSlot);
hMailSlot = 0;
}

void CRecvFunction::ExecuteMessages(){
DWORD MsgCount			=	0;
DWORD NextMsgSize		=	0;
DWORD lBytesRead		=	0;
char *Buffer;
char *RecvMessage;
BYTE bPacketHeader;

//---//

//LOOP:

while(1){

GetMailslotInfo(hMailSlot,0,&NextMsgSize,&MsgCount,0);
if(!MsgCount)
	break;

Buffer = new char[NextMsgSize+1];
memset(Buffer,0,NextMsgSize+1);
lBytesRead = 0;
ReadFile(hMailSlot,Buffer,NextMsgSize,&lBytesRead,0);
if(lBytesRead){
RecvMessage = new char[lBytesRead+1];
memset(RecvMessage,0,lBytesRead+1);
memcpy(RecvMessage,Buffer,lBytesRead);

bPacketHeader = _CPacket.GetBYTE(RecvMessage);

if(!bPaused){
switch(bPacketHeader){

case 0x10:
if(frmBot->bAutoInvite || frmBot->bChatTP)
	ChatRecv(RecvMessage + 1);
break;

case 0x23:
if(frmBot->bLoot)
	ChestDrop(RecvMessage + 1);
break;

case 0x24:
if(frmBot->bLoot)
	ItemDrop(RecvMessage + 1);
break;

case 0x2F:
if(frmBot->bAutoParty)
	PartyRecv(RecvMessage + 1);
break;

default:
break;

}
}

delete [] RecvMessage;
}
delete [] Buffer;
}

//---//

//Sleep(RF_iExecDelay);
//goto LOOP;
}

void CRecvFunction::ChestDrop(char* pBuf){
	if(frmBot->bLootPaused){
		frmBot->bWaitingNewDrop = false;
		return;
	}

	if(frmBot->bWaitingNewDrop){
		if(GetTickCount() - frmBot->iLastChestTick < 5000)	//skip
			return;
		else{
        	if(frmBot->bMoveToChest){
				delete (_CHEST*)lstChestList.back();
				lstChestList.pop_back();
			}
			frmBot->bWaitingNewDrop = false;
        }
	}

	WORD wCorpseID = _CPacket.GetWORD(pBuf);
	DWORD dwChestID = _CPacket.GetDWORD(pBuf + 2);

	CCharBase pCorpse = _CArea.GetAnyBase(wCorpseID);
	if(!pCorpse.GetBase())	return;

	if(frmBot->bMoveToChest){
		if(_CMath.fDist((float)frmBot->iCenterX,(float)frmBot->iCenterY,pCorpse.GetFLOAT(KO_POSX),pCorpse.GetFLOAT(KO_POSY)) > frmBot->iMaxChestDistance)
			return;
	}

	float fDistance = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pCorpse.GetFLOAT(KO_POSX),pCorpse.GetFLOAT(KO_POSY));

	if(!frmBot->bMoveToChest){
		if(!frmBot->bFarChest && fDistance >= 3.0)
			return;
	}

	char pSendBuf[5];
	_CPacket.PutBYTE(pSendBuf,0x24);
	_CPacket.PutDWORD(pSendBuf+1,dwChestID);
	_CPacket.PSend(pSendBuf,5);

	frmBot->fLastChestDist = fDistance;
	frmBot->dwLastChestID = dwChestID;

	if(frmBot->bMoveToChest){
		_CHEST *pNewChest = new _CHEST(wCorpseID,dwChestID,pCorpse.GetFLOAT(KO_POSX),pCorpse.GetFLOAT(KO_POSY));
		lstChestList.push_back(pNewChest);
	}

	frmBot->bWaitingNewDrop = true;
	frmBot->iLastChestTick = GetTickCount();

}

void CRecvFunction::ItemDrop(char* pBuf){
	if(frmBot->bLootPaused){
		frmBot->bWaitingNewDrop = false;
		return;
	}

	if(!frmBot->bWaitingNewDrop)
		return;

	if(frmBot->bMoveToChest){
		if(lstChestList.empty() || lstChestList.size() <= lstDropList.size())
			return;
	}

	DWORD dwItemID[4];
	int iItemN[4];
	bool bItemFound = false;
	bool bSpecialFound[4] = {0,0,0,0};
	bool bCheckSpecial = false;
	DWORD dwItemBase,dwItemExtBase;
	BYTE bExtType;
	int i;
	int iGrade;

	if(frmBot->bLootUnique || frmBot->bLootPriceLimit || frmBot->bLootUpgrade || frmBot->bLootList)
		bCheckSpecial = true;

	for (i = 0; i < 4; i++) {
		dwItemID[i] = _CPacket.GetDWORD(pBuf + (i * 4) + (i * 2));
		iItemN[i] = _CPacket.GetWORD(pBuf + (i * 4) + (i * 2) + 4);

		if(dwItemID[i]){
			bItemFound = true;

			if(bCheckSpecial){
				if(dwItemID[i] != ITEM_NOAH){
					dwItemBase = _CBag.GetItemBase(dwItemID[i]);
					if(!dwItemBase) continue;

					dwItemExtBase = 0;
					bExtType = 0;

					if(*(BYTE*)(dwItemBase + 0x4)){
						dwItemExtBase = _CBag.GetItemExtBase(dwItemID[i],*(BYTE*)(dwItemBase + 0x4));
						if(!dwItemExtBase) continue;
						bExtType = *(BYTE*)(dwItemExtBase + 0x34);
					}

					if(frmBot->bLootUnique){
						if(bExtType == 4)	//unique
							bSpecialFound[i] = true;
					}

					if(frmBot->bLootUpgrade){
						if(dwItemExtBase){
							if(bExtType != 3){	//craft
								iGrade = *(DWORD*)(dwItemExtBase);
								if((iGrade - ((iGrade / 10) * 10)) >= frmBot->iLootUpgradeLimit)
									bSpecialFound[i] = true;
							}
						}
					}

					if(frmBot->bLootPriceLimit){
						if(dwItemBase){
							if(*(DWORD*)(dwItemBase + 0x50) >= (DWORD)frmBot->iLootPriceLimit)
								bSpecialFound[i] = true;
						}
					}


					if(frmBot->bLootList){
						if(frmBot->iLootCount){
							for (int y = 0; y < frmBot->iLootCount; y++) {
								if(dwItemID[i] == (DWORD)frmBot->lLootID[y]){
									bSpecialFound[i] = true;
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	if(!bItemFound){
		if(frmBot->bMoveToChest){
			delete (_CHEST*)lstChestList.back();
			lstChestList.pop_back();
		}
		frmBot->bWaitingNewDrop = false;
		return;
	}

	if(bCheckSpecial){
		for (i = 0; i < 4; i++) {
			if(!bSpecialFound[i]){
            	if(frmBot->bLootCoins){
					if(dwItemID[i] != ITEM_NOAH)
						dwItemID[i] = 0;
				}
				else
					dwItemID[i] = 0;
			}
		}
	}

	bool bEmpty = true;
	for (i = 0; i < 4; i++) {
		if(dwItemID[i]){
			bEmpty = false;
			break;
		}
	}

	if(bEmpty){
		if(frmBot->bMoveToChest){
			delete (_CHEST*)lstChestList.back();
			lstChestList.pop_back();
		}
		frmBot->bWaitingNewDrop = false;
		return;
	}

	if(frmBot->bMoveToChest){
		_DROP *pNewDrop = new _DROP(dwItemID[0],dwItemID[1],dwItemID[2],dwItemID[3]);
		lstDropList.push_back(pNewDrop);
		frmBot->bWaitingNewDrop = false;
		return;
	}

	char pSend[9];
	_CPacket.PutBYTE(pSend,0x26);
	_CPacket.PutDWORD(pSend+1,frmBot->dwLastChestID);

	for (i = 0; i < 4; i++) {
		if(dwItemID[i]){
			_CPacket.PutDWORD(pSend+5,dwItemID[i]);
			_CPacket.PSend(pSend,9);
		}
	}

	frmBot->bWaitingNewDrop = false;
}

bool CRecvFunction::PartyRecv(char* pBuf){
	if(*(BYTE*)pBuf == 0x02){	//invite
		if(frmBot->bAutoParty || frmBot->bNoParty)
		{
			char pSendBuf[3];
			_CPacket.PutBYTE(pSendBuf,0x2F);
			_CPacket.PutBYTE(pSendBuf+1,0x02);

			if(frmBot->bAutoParty)
			{
				if(dlgParty->bAccept && frmBot->iAddCount)
				{
					char pStrName[32]; memset(pStrName,0,32);
					int iNameLen = *(short*)(pBuf + 3);
					if(iNameLen && iNameLen < 32)
					{
						strcpy(pStrName,(char*)(pBuf + 5));
						for (int i = 0; i < frmBot->iAddCount; i++)
						{
							if( !strcmp(pStrName,frmBot->strAddList[i]) )
							{
								_CPacket.PutBYTE(pSendBuf+2,0x01);
								_CPacket.PSend(pSendBuf,3);
								return true;
                            }
						}
						_CPacket.PutBYTE(pSendBuf+2,0x00);
						_CPacket.PSend(pSendBuf,3);
						return true;
					}
					else
					{
						_CPacket.PutBYTE(pSendBuf+2,0x00);
						_CPacket.PSend(pSendBuf,3);
						return true;
                    }
				}
				else
				{
					_CPacket.PutBYTE(pSendBuf+2,0x01);
					_CPacket.PSend(pSendBuf,3);
					return true;
				}
			}
			if(frmBot->bNoParty)
			{
				_CPacket.PutBYTE(pSendBuf+2,0x00);
				_CPacket.PSend(pSendBuf,3);
				return true;
			}
		}
	}
	else if(*(BYTE*)pBuf == 0x05 && dlgCZ->bStopBotOnNoPT)
	{
		StopBot();
    }
	return false;
}

bool CRecvFunction::TradeRecv(char* pBuf){
	if(*pBuf == 0x01)	//request
	{
		char pSend[3];
		*pSend 			= 0x30;
		*(pSend + 1) 	= 0x02;
		*(pSend + 2) 	= 0x00;
		_CPacket.PSend(pSend,3);
		return true;
	}
	return false;
}

bool CRecvFunction::CombatRecv(char* pBuf){
	if(*pBuf == 0x01)	//request
	{
		char pSend[2];
		*pSend 			= 0x75;
		*(pSend + 1) 	= 0x04;
		_CPacket.PSend(pSend,2);
		return true;
	}
	return false;
}

bool CRecvFunction::DeadRecv(char* pBuf){
	WORD ID = *(WORD*)pBuf;
	if(ID == _CCharBase.GetID())
	{
		char pSend[2];
		*pSend = 0x2F;
		*(pSend + 1) = 0x05;
		_CPacket.PSend(pSend,2);
	}
	return false;
}

void CRecvFunction::MoveRecv(char* pBuf){

	if(frmBot->bBotPaused)
		return;

	DWORD dwCurrentTick = GetTickCount();

	if(dwCurrentTick - frmBot->iLastPTSWTick < 1500)
		return;

	BYTE bSpeed = *(BYTE*)(pBuf + 8);
	if(bSpeed != 0x2D)	//normal (45)
		return;

	DWORD ID = *(WORD*)pBuf;

	if( !_CParty.CheckInParty(ID) )	//not party member
		return;

	CCharBase pUser = _CArea.GetUPCBase(ID);

	if( !pUser.GetBase() )
		return;

	float fMyX = _CCharBase.GetFLOAT(KO_POSX);
	float fMyY = _CCharBase.GetFLOAT(KO_POSY);
	float fUserX = pUser.GetFLOAT(KO_POSX);
	float fUserY = pUser.GetFLOAT(KO_POSY);

	float fDist = _CMath.fDist(fMyX,fMyY,fUserX,fUserY);
	if(fDist > 12.5)
		return;

	DWORD dwSkillNum = _CCast.MakeCastNum(_CCharBase,"010");
	if( !_CTest.CheckSkillAvailable(dwSkillNum) )
		return;

	*(DWORD*)(pBuf_BuffSelf1 + 2) = dwSkillNum;
	*(DWORD*)(pBuf_BuffSelf2 + 2) = dwSkillNum;

	*(WORD*)(pBuf_BuffSelf1 + 8) = pUser.GetID();
	*(WORD*)(pBuf_BuffSelf2 + 8) = pUser.GetID();

	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x0F;
	_CPacket.PSend(pBuf_BuffSelf1,24);
	_CPacket.PSend(pBuf_BuffSelf2,22);
	*(BYTE*)(pBuf_BuffSelf1 + 22) = 0x00;

	*(WORD*)(pBuf_BuffSelf1 + 8) = dwMyID;
	*(WORD*)(pBuf_BuffSelf2 + 8) = dwMyID;

	frmBot->iLastPTSWTick = dwCurrentTick;
}

void CRecvFunction::ChatRecv(char* pBuf){
	if(*(BYTE*)pBuf == 0x01 || *(BYTE*)pBuf == 0x02 || *(BYTE*)pBuf == 0x03){	//all || pm || party

		WORD wUserID = _CPacket.GetWORD(pBuf + 2);

		int iNameLen = _CPacket.GetBYTE(pBuf + 4);
		char *strName = new char[iNameLen + 1];
		memset(strName,0,iNameLen + 1);
		memcpy(strName,pBuf + 5,iNameLen);
		int iChatLen = _CPacket.GetWORD(pBuf + 5 + iNameLen);
		char *strChat = new char[iChatLen + 1];
		memset(strChat,0,iChatLen + 1);
		memcpy(strChat,pBuf + 7 + iNameLen,iChatLen);

		if( iICPartyPass && iChatLen == 4 && atoi(strChat) == iICPartyPass)
		{
			char *pParty = new char[iNameLen + 4];
			_CPacket.PutBYTE(pParty,0x2F);

			if(bICPartyFirstUser)
			{
				_CPacket.PutBYTE(pParty + 1,0x01);
				bICPartyFirstUser = false;
			}
			else
				_CPacket.PutBYTE(pParty + 1,0x03);

			_CPacket.PutWORD(pParty + 2,iNameLen);
			memcpy(pParty + 4, strName, iNameLen);
			_CPacket.PSend(pParty,iNameLen + 4);
			delete [] pParty;
		}

		///////////////////////// BOT KONTROL TEST :D
		if( iChatLen == 11 )
		{
			if( !strcmp(strChat,"asd1238095a") )
			{
				_CTest.CommandSit(true);
				_CTest.CommandSit(false);
            }
		}
		/////////////////////////

		if(frmBot->bAutoInvite){
			if(iChatLen == dlgParty->txtautoadd->Text.Length()){
				if(!strcmp(strChat,dlgParty->txtautoadd->Text.c_str())){
					char *pSend = new char[iNameLen + 4];
					_CPacket.PutBYTE(pSend,0x2F);

					_CParty.Refresh();
					_CPacket.PutBYTE(pSend + 1,0x01);

					if(_CParty.GetCount() >= 2)
						_CPacket.PutBYTE(pSend + 1,0x03);

					_CPacket.PutWORD(pSend + 2,iNameLen);
					memcpy(pSend + 4, strName, iNameLen);
					_CPacket.PSend(pSend,iNameLen + 4);
					delete [] pSend;
				}
			}
		}

		if(*(BYTE*)pBuf == 0x03 && frmBot->bChatTP){
			if(iChatLen == frmBot->txttpchat->Text.Length()){
				if(!strcmp(strChat,frmBot->txttpchat->Text.c_str())){
					if(wUserID != _CCharBase.GetID())
						TeleportUser(wUserID);
				}
			}
		}

		if(*(BYTE*)pBuf == 0x03 && frmBot->bChatBuff){
			if(iChatLen == frmBot->txtchatbuff->Text.Length()){
				if(!strcmp(strChat,frmBot->txtchatbuff->Text.c_str())){
					_CParty.Refresh();
					if(_CParty.GetCount()){
						DWORD dwUser;
						for (int i = 0; i < _CParty.GetCount(); i++) {
							dwUser = _CParty.GetUser(i);
							if(dwUser){
								if(*(WORD*)(dwUser + KO_PT_ID) == wUserID)
									frmBot->iLastUserMaxHP[i] = 0;
									frmBot->bUserWaitingForNewHP[i] = false;
							}
						}
					}
				}
			}
		}

		if(*(BYTE*)pBuf == 0x03 && frmBot->bChatHeal){
			if(iChatLen == frmBot->txtchatheal->Text.Length()){
				if(!strcmp(strChat,frmBot->txtchatheal->Text.c_str())){
					_CParty.Refresh();
					if(_CParty.GetCount()){
						DWORD dwUser;
						for (int i = 0; i < _CParty.GetCount(); i++) {
							dwUser = _CParty.GetUser(i);
							if(dwUser){
								if(*(WORD*)(dwUser + KO_PT_ID) == wUserID)
									frmBot->iPriestHealLockIndex = i;
							}
						}
					}
				}
			}
		}

		delete [] strChat;
		delete [] strName;
	}
}

void CRecvFunction::ExpRecv(char* pBuf){

	long lTotalExp = *(long*)(pBuf + 1);
	signed int iEarned = lTotalExp - (long)_CCharBase.GetDWORD(KO_CRXP);

	if(iEarned < 0)
		iEarned = 0;

	if(!iEarned)
		return;

	long lNeededExp = _CCharBase.GetDWORD(KO_MXXP) - _CCharBase.GetDWORD(KO_CRXP);
	int iNeededKill = lNeededExp / iEarned;

	static long lLastKillTick = 0;

	long lKillTime = GetTickCount() - lLastKillTick;
	if(lKillTime != (long)GetTickCount())
	{
		long lTotalTime = iNeededKill * lKillTime;
		lTotalTime = lTotalTime / 1000;

		long lExpPerHour = (3600 / lTotalTime) * iEarned;

		long lSec = lTotalTime;
		long lMin = 0;
		long lHour = 0;
		long lDay = 0;
		if(lSec >= 60)
        {
			lMin = lSec / 60;
			lSec = lSec % 60;
			if(lMin >= 60)
			{
				lHour = lMin / 60;
				lMin = lMin % 60;
				if(lHour >= 24)
				{
					lDay = lHour / 24;
					lHour = lHour % 24;
                }
            }
		}

		char tmp[32];	memset(tmp,0,32);
		char* pText = new char[64];	memset(pText,0,64);
		strcpy(pText,"Saatlik EXP: [");
		ltoa(lExpPerHour,tmp,10);
		strcat(pText,tmp);
		strcat(pText,"] | Level alinacak: [");

		memset(tmp,0,32);
		ltoa(lDay,tmp,10);
		strcat(pText,tmp);
		strcat(pText," gun, ");

		memset(tmp,0,32);
		ltoa(lHour,tmp,10);
		strcat(pText,tmp);
		strcat(pText," saat, ");

		memset(tmp,0,32);
		ltoa(lMin,tmp,10);
		strcat(pText,tmp);
		strcat(pText," dk, ");

		memset(tmp,0,32);
		ltoa(lSec,tmp,10);
		strcat(pText,tmp);
		strcat(pText," sn.]");

		ModifyMenu(frmBot->hMainMenu,2,MF_BYPOSITION | MF_DISABLED,NULL,pText);
		DrawMenuBar(hGameHWND);
    }

	lLastKillTick = GetTickCount();

}

CSkillIcon::CSkillIcon(int iKind, int iPage, int iSlot){
	if(iKind == -1 || iPage == -1 || iSlot == -1){
		dwBase = 0;
		return;
	}

	DWORD dwTmp = *(DWORD*)(KO_DLGA);
	dwTmp = *(DWORD*)(dwTmp + OFF_SKILLTREE);
	dwTmp = *(DWORD*)(dwTmp + 0x13C + (iKind * 168) + (iPage * 24) + (iSlot * 4));
	dwBase = dwTmp;
	iCurrentKind = iKind;
	iCurrentPage = iPage;
	iCurrentSlot = iSlot;
}

void CSkillIcon::SetSlot(int iKind, int iPage, int iSlot){ //CSkillIcon()
	if(iKind == -1 || iPage == -1 || iSlot == -1){
		dwBase = 0;
		return;
	}

	DWORD dwTmp = *(DWORD*)(KO_DLGA);
	dwTmp = *(DWORD*)(dwTmp + OFF_SKILLTREE);
	dwTmp = *(DWORD*)(dwTmp + 0x13C + (iKind * 168) + (iPage * 24) + (iSlot * 4));
	dwBase = dwTmp;
	iCurrentKind = iKind;
	iCurrentPage = iPage;
	iCurrentSlot = iSlot;
}

BYTE CRecv::GetIndex(){
	DWORD Idx = 0;
	__asm{
		LEA EAX,&Idx
		PUSH EAX
		PUSH 4
		PUSH KO_RCFX
		PUSH 1
		MOV EAX,KO_INDX
		CALL EAX
		ADD ESP,0x10
	}
	bFuncIndex = (BYTE)Idx;
	return (BYTE)Idx;
}

bool CRecv::Enable(){

	if(bState == true)
		return true;

	BYTE bFuncIdx = GetIndex();
	DWORD _KO_RCVA = KO_RCVA + (bFuncIdx * 4);
	DWORD _KO_RCVB = *(DWORD*)(_KO_RCVA);
	dwGameFunction = _KO_RCVB;

	*(DWORD*)_KO_RCVA = (DWORD)&ProcessRecv;
	bState = true;
	//bPaused = false;

	return true;
}

void CRecv::Disable(){

	if(bState == false)
		return;

	DWORD _KO_RCVA = KO_RCVA + (bFuncIndex * 4);
	*(DWORD*)_KO_RCVA = dwGameFunction;
	bState = false;
	//bPaused = false;
}

float CTest::GetPosZ(float fPosX, float fPosY)
{
	float fRetZ = 0;
	__asm{
		PUSH 1
		PUSH fPosY
		PUSH fPosX
		MOV EAX,[KO_LOCA]
		MOV ECX,[EAX+0x1C]
		MOV EAX,[ECX]
		CALL [EAX+0x34]
		FSTP &fRetZ
	}
	return fRetZ;
}

void CTest::InitPos(float fPosX, float fPosY, float fPosZ)
{
	char pNewPos[12];	// 4*3 = 12 bytes memory allocation.

	*(FLOAT*)(pNewPos) 		= fPosX;
	*(FLOAT*)(pNewPos + 4) 	= fPosZ;
	*(FLOAT*)(pNewPos + 8) 	= fPosY;

	__asm
	{
		LEA EAX,pNewPos
		PUSH EAX
		MOV ECX,[KO_DLGA]
		MOV EAX,KO_IPOS
		CALL EAX
	}

}

void CTest::InitPos2(float fPosX, float fPosY, float fPosZ)
{
	char pNewPos[12];	// 4*3 = 12 bytes memory allocation.

	*(FLOAT*)(pNewPos) 		= fPosX;
	*(FLOAT*)(pNewPos + 4) 	= fPosZ;
	*(FLOAT*)(pNewPos + 8) 	= fPosY;

	__asm
	{
		PUSH 1
		LEA EAX,pNewPos
		PUSH EAX
		MOV ECX,[KO_CHAR]
		MOV EAX,[ECX]
		CALL [EAX+0x20]
	}

}

DWORD CTest::GetSkillBase(DWORD dwSkillNum){
	if(!dwSkillNum)
		return 0;

	DWORD dwSkillBase = 0;

	__asm{
		LEA EAX,&dwSkillNum
		PUSH EAX
		LEA EAX,&dwSkillBase
		PUSH EAX
		MOV ECX,[KO_PSFN]
		LEA ECX,[ECX + 0x14]
		MOV EAX,KO_CSFN
		CALL EAX
	}

	if(dwSkillBase){
		if(*(DWORD*)(*(DWORD*)(KO_PSFN) + 0x18) == dwSkillBase)
			return 0;
	}

	return dwSkillBase + 0x10;
}

bool CTest::CheckSkillAvailable(DWORD dwSkillNum){
	if(!dwSkillNum)
		return false;

	bool bRet = false;

	__asm{
		PUSH dwSkillNum
		MOV EAX,KO_CHKS
		MOV ECX,[KO_DLGA]
		MOV ECX,[ECX + OFF_SKILLTREE]
		CALL EAX
		MOV bRet,AL
	}

	return bRet;
}

void CTest::CommandSit(bool bSit){
	DWORD byOperation = 0;
	byOperation = (DWORD)bSit;
	__asm{
		MOV ECX,[KO_DLGA]
		PUSH 1
		PUSH byOperation
		PUSH 0
		MOV EAX,KO_SITD
		CALL EAX
	}
}

bool CTest::CastMagic(int iCastID, int iTargetID)
{
	bool bRet;
	DWORD dwCastBase = _CTest.GetSkillBase(iCastID);

	if( !dwCastBase )
		return false;

	DWORD dwTargetID = (DWORD)iTargetID;

	__asm
	{
		MOV ECX,[KO_DLGA]
		MOV ECX,[ECX + 0x3DC]
		PUSH dwCastBase
		PUSH dwTargetID
		PUSH KO_CAST
		POP EAX
		CALL EAX
		MOV bRet,AL
	}

	return bRet;
}

DWORD CTest::PickChar(int iScreenX, int iScreenY, float &fRealX, float &fRealY, float &fRealZ)
{
	DWORD dwRet = 0;
	DWORD dwID = 0;

	char pPos[12];

	__asm
	{
		LEA EAX,&pPos
		PUSH EAX
		LEA EAX,&dwID
		PUSH EAX
		PUSH iScreenY
		PUSH iScreenX
		MOV ECX,[KO_AREA]
		MOV EAX,KO_PICK
		CALL EAX
		MOV dwRet,EAX
	}

	if(dwRet)
	{
		fRealX = *(float*)(pPos);
		fRealZ = *(float*)(pPos + 4);
		fRealY = *(float*)(pPos + 8);
	}
	else
	{
		fRealX = 0;
		fRealY = 0;
		fRealZ = 0;
	}

	return dwRet;
}
