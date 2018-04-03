//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "_frmServer.h"

char* strIPArray[2] = {"213.142.138.205","31.3.251.68"};
int iTargetServerNo = 1;
CSocket CSyncSocket = 0;

bool bSyncServer = false;

long *lBannedIPList;
bool bBanIP = false;
bool bFirstBan = false;
int iIPCount = 0;
int iIPIndex = 0;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmServer *frmServer;

CSocket CMainSocket 		=  	0;
SOCKET  hAcceptedSocket 	=   0;
SOCKET  hAcceptedSocket2 	=   0;
SOCKET  hAcceptedSocket3 	=   0;
long AcceptedTick 			= 	0;
long AcceptedTick2 			= 	0;
long AcceptedTick3 			= 	0;
DWORD dwThread 				= 	0;
DWORD dwThread2 				= 	0;
DWORD dwThread3 				= 	0;
HANDLE hThread 				= 	0;
HANDLE hThread2				= 	0;
HANDLE hThread3 				= 	0;
long lConnectionCount 		=	0;

int iLoginCount				=	0;
int iTodayRegisterCount		=	0;
int iTodayAddCount			=	0;

int iRegisterCount			=	0;
int iAddCount				=	0;
int	iThreadCount			=	0;

CRITICAL_SECTION csProcess1;
CRITICAL_SECTION csProcess2;
CRITICAL_SECTION csProcess3;
CRITICAL_SECTION csProcess4;
CRITICAL_SECTION csProcess5;

std::list<CUser*>				lstUserList;
std::list<CNewConnection*>	  	lstSocket;
std::list<CSyncData*>			lstSync;

//---------------------------------------------------------------------------
__fastcall TfrmServer::TfrmServer(TComponent* Owner)
	: TForm(Owner)
{
	bTryLater 		= false;
	bRegisterPaused = false;
	bCheckPaused 	= false;
	bAddPaused 		= false;
	bLogoutPaused 	= false;
	bBanPaused 		= false;
	bBanTrial		= true;
	bChangePWPaused = false;

	bLogKeys = true;
	bBanDate = false;
	bBanID = false;
	bBanNum = false;
	bFree = false;
	bFreeIP = false;

	bCritical = true;

	bFreeTest	=	false;

}
//---------------------------------------------------------------------------
void __fastcall TfrmServer::chpauseClick(TObject *Sender)
{
	bTryLater = chpause->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TfrmServer::FormShow(TObject *Sender)
{

SQLCon->Open();

if(!SQLCon->Connected){
	MessageBox(this->Handle,"SQL Baglanti hatasi!","Hata",MB_ICONEXCLAMATION);
	Application->Terminate();
}

if(!_CWinsock.GetState()){
	bool bStart = _CWinsock.StartWinsock();
	if(!bStart){
		MessageBox(this->Handle,"Winsock error!","Hata",MB_ICONEXCLAMATION);
		Application->Terminate();
	}
}

CMainSocket = _CWinsock.NewSocket();
if(CMainSocket.GetSocket() == INVALID_SOCKET){
	MessageBox(this->Handle,"Socket error!","Hata",MB_ICONEXCLAMATION);
	Application->Terminate();
}

DWORD dwTrue = 1;
ioctlsocket(CMainSocket.GetSocket(), FIONBIO, (u_long FAR*) &dwTrue);

bSyncServer = chsync->Checked;

/*
/////////////////////////////////////////////////
// paket bitmeden soket kapatmasin :D
struct linger pLinger;
pLinger.l_onoff = 1;	//aktif
pLinger.l_linger = 1;	//3 saniye gecikme yeterli
if( setsockopt(CMainSocket.GetSocket(), SOL_SOCKET, SO_LINGER, (char*)&pLinger, sizeof(pLinger)) )
	MessageBoxA(0,"setsockopt error!",0,0);
/////////////////////////////////////////////////
*/

lstSocket.clear();
lstUserList.clear();
lstSync.clear();

if(!CMainSocket.Server_Bind(iServerPort)){
	MessageBox(this->Handle,"Bind error!","Hata",MB_ICONEXCLAMATION);
	CMainSocket.Close();
	Application->Terminate();
}

if(!CMainSocket.Server_Listen(iMaxListen)){
	MessageBox(this->Handle,"Listen error!","Hata",MB_ICONEXCLAMATION);
	CMainSocket.Close();
	Application->Terminate();
}

LoadDateList();
LoadIDList();
LoadNumList();

tmusercheck->Enabled = true;

char* strCMD = new char[64];

memset(strCMD,0,64);
strcpy(strCMD,"select iUserID from userlist");
SQLData->Close();
SQLData->CommandText = strCMD;
SQLData->Open();

iRegisterCount = SQLData->RecordCount;

char tmp[32];
itoa(iRegisterCount,tmp,10);

lbregistercount->Caption = tmp;

memset(strCMD,0,64);
strcpy(strCMD,"select iUserID from addlist");
SQLData->Close();
SQLData->CommandText = strCMD;
SQLData->Open();

iAddCount = SQLData->RecordCount;
itoa(iAddCount,tmp,10);
lbaddcount->Caption = tmp;

InitializeCriticalSection(&csProcess1);
InitializeCriticalSection(&csProcess2);
InitializeCriticalSection(&csProcess3);
InitializeCriticalSection(&csProcess5);
if(bSyncServer)
	InitializeCriticalSection(&csProcess4);

delete [] strCMD;
hThread = 	CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ServerFunction,NULL,0,&dwThread);
			CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SocketProcess,NULL,0,0);
			if(bSyncServer)
				CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SyncProcess,NULL,0,0);
//hThread2 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ServerFunction2,NULL,0,&dwThread2);
//hThread3 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ServerFunction3,NULL,0,&dwThread3);
}
//---------------------------------------------------------------------------

/*
void ServerFunction(){
int iInfoSize;
DWORD nRecv,nRecvd;
char *pRecv = new char[iRecvSizeMax];	//packet size
char tmp[8];

while(1){
hAcceptedSocket = CMainSocket.Server_Accept();
if(hAcceptedSocket == INVALID_SOCKET){
	Sleep(25);
	continue;
}

CMainSocket.SetAcceptedSocket(hAcceptedSocket);
AcceptedTick = GetTickCount();

lConnectionCount++;
ltoa(lConnectionCount,tmp,10);
frmServer->lbcount->Caption = tmp;

nRecv = 0;
while(!nRecv){
	ioctlsocket(CMainSocket.GetAcceptedSocket(),FIONREAD,&nRecv);
	if(!nRecv && GetTickCount() - AcceptedTick >= 1000)
	{
		CMainSocket.CloseAccepted();
		break;
	}
}

if(!CMainSocket.GetAcceptedSocket())
	continue;

if(nRecv > iRecvSizeMax || nRecv < iRecvSizeMin)
{
	CMainSocket.CloseAccepted();
	continue;
}

nRecvd = 0;
do{
	nRecvd += CMainSocket.Server_Recv(pRecv+nRecvd,nRecv-nRecvd);
} while(nRecvd < nRecv);

int iDataHead = CMainSocket.FindPacketHead(pRecv,nRecvd);
if(iDataHead < (int)sizeof(wPACKETHEAD)){
	CMainSocket.CloseAccepted();
	continue;
}

int iDataTail = CMainSocket.FindPacketTail(pRecv+iDataHead,nRecvd-iDataHead);
if(iDataTail < 1){
	CMainSocket.CloseAccepted();
	continue;
}

int iDataSize = iDataTail;
char* pData = pRecv + iDataHead;

int index(0);
BYTE bPrefix = CMainSocket.GetBYTE(pData,index);	//main prefix!!!
switch(bPrefix){

	case bRecv_LOGIN:
	Recv_Login(pData,index);
	iLoginCount++;
	itoa(iLoginCount,tmp,10);
	frmServer->lblogincount->Caption = tmp;
	break;

	case bRecv_CHECK:
	Recv_Check(pData,index);
	break;

	case bRecv_LOGOUT:
	Recv_Logout(pData,index);
	break;

	case bRecv_BANOTHER:
	Recv_BanOther(pData,index);
	break;

	case bRecv_REGISTER:
	Recv_Register(pData,index);
	break;

	case bRecv_ADD:
	Recv_Add(pData,index);
	break;

	case bRecv_REPORT:
	Recv_Report(pData,index);
	break;

	case bRecv_TRIAL:
	Recv_Trial(pData,index);
	break;

	case bRecv_BANTRIAL:
	Recv_BanTrial(pData,index);
	break;

	default:
	break;
}

CMainSocket.CloseAccepted();
Sleep(25);
}
}
*/

void ServerFunction(){
char tmp[16]; memset(tmp,0,16);
DWORD dwTrue = 1;
CNewConnection *pNew;
long lAddr;
bool bBack = false;

/*
/////////////////////////////////////////////////
// paket bitmeden soket kapatmasin :D
struct linger pLinger;
pLinger.l_onoff = 1;	//aktif
pLinger.l_linger = 1;	//3 saniye gecikme yeterli
/////////////////////////////////////////////////
*/

while(1){
hAcceptedSocket = CMainSocket.Server_Accept();
if(hAcceptedSocket == INVALID_SOCKET)
{
	Sleep(10);
	continue;
}

	bBack = false;
	if(bBanIP)
	{
		if(iIPCount)
		{
			lAddr = CMainSocket.GetSockInfo().sin_addr.S_un.S_addr;
			for (int i = 0; i < iIPCount; i++)
			{
				if( lAddr == lBannedIPList[i])
				{
					bBack = true;
					break;
                }
			}
        }
	}

	if(bBack)
	{
    	closesocket(hAcceptedSocket);
		continue;
	}

EnterCriticalSection(&csProcess2);

//setsockopt(hAcceptedSocket,SOL_SOCKET,SO_LINGER,(char*)&dwTrue,4);

ioctlsocket(hAcceptedSocket, FIONBIO, (u_long FAR*) &dwTrue);
//setsockopt(hAcceptedSocket, SOL_SOCKET, SO_LINGER, (char*)&pLinger, sizeof(pLinger));

pNew = new CNewConnection( hAcceptedSocket, GetTickCount() );
strcpy( pNew->m_strIP, inet_ntoa(CMainSocket.GetSockInfo().sin_addr) );
//CreateThread(0,0,(LPTHREAD_START_ROUTINE)pNew->StartProcess,pNew,0,0);
//CreateThread(0,4096,(LPTHREAD_START_ROUTINE)UserProcess2,pNew,0,0);

//UserProcess2(pNew);

lstSocket.push_back(pNew);

LeaveCriticalSection(&csProcess2);

lConnectionCount++;
ltoa(lConnectionCount,tmp,10);
frmServer->lbcount->Caption = tmp;

Sleep(1);
}
}

//---------------------------------------------------------------------------

void __fastcall TfrmServer::FormClose(TObject *Sender, TCloseAction &Action)
{
	CMainSocket.CloseAccepted();
	CMainSocket.Close();
	ClearOnlineList();
	_CWinsock.CleanWinsock();
}

//---------------------------------------------------------------------------

void __fastcall TfrmServer::chlogkeysClick(TObject *Sender)
{
bLogKeys = chlogkeys->Checked;
}
//---------------------------------------------------------------------------

void WriteLog(char* strFileName, char* strLog){
ofstream fLogFile;
fLogFile.open(strFileName,std::ios_base::out | std::ios_base::app);
if(!fLogFile.is_open())return;
fLogFile << strLog << "\n";
fLogFile.close();
}

void Recv_Login(CNewConnection *pSocket, char* pBuf, int &index){

	CUser* pUser;
	int iUserIndex;
	int iUserIndex_InUse;
	int iLogOutKey_InUse;
	int iLogOutKey;

	SYSTEMTIME pDate;
	SYSTEMTIME pDate2;
	long lCurrentTime;
	int iResult,iLen;
	char* pStrLog;
	char* tmp;
	char* strCMD = NULL;

	itUL it,itEnd;

	int iUserDate,iUserID,iUserDays,iUserExtraDays,iUserKeyNo;
	bool bUserBanned,bUserFreeIP;

	char strID[21];
	char strPW[21];

	memset(strID,0,21);
	memset(strPW,0,21);

	short sBotVersion = CMainSocket.GetWORD(pBuf,index);
	short sSourceID = CMainSocket.GetWORD(pBuf,index);
	long lSockHandle = CMainSocket.GetDWORD(pBuf,index);
	DWORD dwSendAddr = CMainSocket.GetDWORD(pBuf,index);
	DWORD dwHDNo = CMainSocket.GetDWORD(pBuf,index);
	DWORD dwARG = CMainSocket.GetDWORD(pBuf,index);
	BYTE bSendFixAdd = 0;
	lSockHandle--;
	__asm{NOT dwSendAddr};	//////////////
	__asm{NOT lSockHandle};//////////////
	__asm{NOT sSourceID};///////////////
	bSendFixAdd = (BYTE)(dwSendAddr % 10) + 0x53;
	dwHDNo -= dwSendAddr / 10;
	dwARG -= dwSendAddr / 10;
	__asm{NOT bSendFixAdd};

	int iIDLen = CMainSocket.GetBYTE(pBuf,index);
	if(iIDLen <= 20)
		CMainSocket.GetSTR(strID,pBuf,iIDLen,index);
	int iPWLen = CMainSocket.GetBYTE(pBuf,index);
	if(iPWLen <= 20)
		CMainSocket.GetSTR(strPW,pBuf,iPWLen,index);

	if(frmServer->bTryLater){
		iResult = bLogin_TRYLATER;
		goto RETN;
	}

	if(!sSourceID || sSourceID == -1){
		iResult = bLogin_INITERR;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pUser->m_strIP) );
		}

		goto RETN;
	}

	if(!lSockHandle || lSockHandle == -1){
		iResult = bLogin_INITERR;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pUser->m_strIP) );
		}

		goto RETN;
	}

	if(frmServer->bFree){
		iResult = bLogin_SUCCESS;
		goto RETN;
	}

	if(!iIDLen || iIDLen < 4 || iIDLen > 20 || !iPWLen || iPWLen < 4 || iPWLen > 20){
		iResult = bLogin_INVALID;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pUser->m_strIP) );
		}

		goto RETN;
	}

	if(!strlen(strID) || !strlen(strPW))
	{
		iResult = bLogin_INVALID;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pUser->m_strIP) );
		}

		goto RETN;
	}

	if(!CheckValidIDPW(strID) || !CheckValidIDPW(strPW))
	{
		iResult = bLogin_INVALID;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pUser->m_strIP) );
		}

		goto RETN;
	}

	if(sBotVersion != iServerVersion){
		iResult = bLogin_INVALIDV;
		goto RETN;
	}

	strCMD = new char[64];
	memset(strCMD,0,64);

	strcpy(strCMD,"select * from userlist where strID like '");
	strcat(strCMD,strID);
	strcat(strCMD,"' and strPW like '");
	strcat(strCMD,strPW);
	strcat(strCMD,"'");

	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	if(frmServer->SQLData->RecordCount != 1){
		iResult = bLogin_INVALID;
		goto RETN;
	}

	iUserDate = atoi(frmServer->SQLData->FieldByName("iDate")->AsString.c_str());
	iUserID = atoi(frmServer->SQLData->FieldByName("iUserID")->AsString.c_str());
	iUserDays = atoi(frmServer->SQLData->FieldByName("iDays")->AsString.c_str());
	iUserExtraDays = atoi(frmServer->SQLData->FieldByName("iExtraDays")->AsString.c_str());
	iUserKeyNo = atoi(frmServer->SQLData->FieldByName("iKeyNo")->AsString.c_str());
	bUserBanned = (bool)atoi(frmServer->SQLData->FieldByName("bBanned")->AsString.c_str());
	bUserFreeIP = (bool)atoi(frmServer->SQLData->FieldByName("bFreeIP")->AsString.c_str());

	if(bUserBanned){
		iResult = bLogin_BANNED;
		goto RETN;
	}

	if(iUserID == 425 && !frmServer->bFreeTest)	//DENEME KARTI
	{
		GetLocalTime(&pDate2);
		if(pDate2.wHour < 18 || pDate2.wHour >= 21)	//18 - 21
		{
			iResult = bLogin_TRIALERR;
			goto RETN;
		}
	}

	if(frmServer->bBanDate)
	{	if(frmServer->lstdateban->Items->Count)
		{	for (int i = 0; i < frmServer->lstdateban->Items->Count; i++)
			{	if(iUserDate == atol(frmServer->lstdateban->Items->operator [](i).c_str()))
				{	iResult = bLogin_BANNED;
					goto RETN;
				}
			}
		}
	}

	if(frmServer->bBanID)
	{	if(frmServer->lstidban->Items->Count)
		{	for (int i = 0; i < frmServer->lstidban->Items->Count; i++)
			{	if(iUserID == atoi(frmServer->lstidban->Items->operator [](i).c_str()))
				{	iResult = bLogin_BANNED;
					goto RETN;
				}
			}
		}
	}

	if(frmServer->bBanNum)
	{	if(frmServer->lstnoban->Items->Count)
		{	for (int i = 0; i < frmServer->lstnoban->Items->Count; i++)
			{	if(iUserKeyNo == atoi(frmServer->lstnoban->Items->operator [](i).c_str()))
				{	iResult = bLogin_BANNED;
					goto RETN;
				}
			}
		}
	}

	if(iUserDays != 0xFF){	//unlimited
		GetSystemTime(&pDate);

		lCurrentTime = ((pDate.wYear - 2000) * 365) + ((pDate.wMonth - 1) * 30) + pDate.wDay;

		if(lCurrentTime >= (iUserDate + iUserDays + iUserExtraDays)){
			iResult = bLogin_TIMEOUT;
			goto RETN;
		}
	}

	if(!frmServer->bFreeIP && !frmServer->bFree && !bUserFreeIP)
	{
		if(lstUserList.size())
		{
			EnterCriticalSection(&csProcess3);
			it 		= lstUserList.begin();
			itEnd 	= lstUserList.end();

			for (; it != itEnd; it++)
			{
				if((*it)->m_iUserID == iUserID)
				{
					if((*it)->m_bFreeIP == false)
					{
						/*
						if( strcmp((*itUser)->m_strIP,pSocket->m_strIP) > 0 )
						{
							iResult = bLogin_INUSE;
							iUserIndex_InUse = (*itUser)->m_iUserIndex;
							iLogOutKey_InUse = (*itUser)->m_sLogOutKey;
							goto RETN;
						}
						else
						{
						*/
							if( (*it)->m_lHDNo != (long)dwHDNo || (*it)->m_dwARG != dwARG )
							{
								iResult = bLogin_INUSE;
								iUserIndex_InUse = (*it)->m_iUserIndex;
								iLogOutKey_InUse = (*it)->m_sLogOutKey;
								LeaveCriticalSection(&csProcess3);
								goto RETN;
							}
						//}
					}
				}
			}
			LeaveCriticalSection(&csProcess3);
		}
	}

	iResult = bLogin_SUCCESS;

RETN:

	if(iResult == bLogin_SUCCESS){

		iUserIndex = GetNewUserIndex();
		iLogOutKey = GetNewRandomKey();

		if(frmServer->bFree){
			Send_Login(pSocket,iResult,0xFF,iUserIndex,0,sSourceID,lSockHandle,bSendFixAdd,iLogOutKey,dwHDNo,dwARG);
			iUserID = 0;
		}
		else{

			if(iUserDays == 0xFF)	//unlimited key
				Send_Login(pSocket,iResult,0xFF,iUserIndex,iUserID,sSourceID,lSockHandle,bSendFixAdd,iLogOutKey,dwHDNo,dwARG);	//success
			else
				Send_Login(pSocket,iResult,(iUserDate + iUserDays + iUserExtraDays) - lCurrentTime,iUserIndex,iUserID,sSourceID,lSockHandle,bSendFixAdd,iLogOutKey,dwHDNo,dwARG);	//success
		}

		pUser = new CUser(iUserID,iUserIndex,bUserFreeIP,sSourceID,lSockHandle,dwHDNo,iLogOutKey,dwARG);
		strcpy( pUser->m_strIP, pSocket->m_strIP );

		EnterCriticalSection(&csProcess3);
			lstUserList.push_back(pUser);
		LeaveCriticalSection(&csProcess3);
	}
	else
		Send_Login(pSocket,iResult,0,iUserIndex_InUse,iUserID,0,0,0,iLogOutKey_InUse,0,0);

	if(frmServer->bLogKeys){

		GetLocalTime(&pDate);

		pStrLog = new char[128];
		memset(pStrLog,0x00,128);

		tmp = new char[32];
		memset(tmp,0,32);

		itoa(pDate.wDay,tmp,10);
		strcpy(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wMonth,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wYear,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," ");

		itoa(pDate.wHour,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMinute,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wSecond,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMilliseconds,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," --- ");

		switch(iResult){

			case bLogin_INVALID:
			strcat(pStrLog,"ERROR_INVALID: [");
			break;

			case bLogin_SUCCESS:
			if(frmServer->bFree)
				strcat(pStrLog,"SUCCESS (FREE): [ID: ");
			else
				strcat(pStrLog,"SUCCESS: [ID: ");
			itoa(iUserID,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog," Index: ");
			itoa(iUserIndex,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"] - [");
			break;

			case bLogin_INITERR:
			strcat(pStrLog,"ERROR_INIT: [");
			break;

			case bLogin_INVALIDV:
			strcat(pStrLog,"ERROR_VERSION: [");
			break;

			case bLogin_INUSE:
			strcat(pStrLog,"ERROR_INUSE: [ID: ");
			itoa(iUserID,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog," Index_InUse: ");
			itoa(iUserIndex_InUse,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"] - [");
			break;

			case bLogin_BANNED:
			strcat(pStrLog,"ERROR_BANNED: [ID: ");
			itoa(iUserID,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"] - [");
			break;

			case bLogin_TRYLATER:
			strcat(pStrLog,"ERROR_TRYLATER: [");
			break;

			case bLogin_TIMEOUT:
			strcat(pStrLog,"ERROR_TIMEOUT: [ID: ");
			itoa(iUserID,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"] - [");
			break;

			default:
			strcat(pStrLog,"ERROR_UNKNOWN: [");
			break;

		}

		strcat(pStrLog,strID);
		strcat(pStrLog," - ");
		strcat(pStrLog,strPW);
		strcat(pStrLog,"] - ");
		strcat(pStrLog,pSocket->m_strIP);
		WriteLog("login_log.txt",pStrLog);

		delete[] tmp;
		delete[] pStrLog;
	}

	if(strCMD)
		delete[] strCMD;
}

void Send_Login(CNewConnection *pSocket, int iResult, int iRemainingDays, int iUserIndex, int iUserID, int iSourceID, long lSockHandle, BYTE bSendFixAdd,int iLogOutKey,DWORD dwHDNo,DWORD dwARG){
	char pSend[29];
	memset(pSend,0,29);
	int index(0);

	CMainSocket.PutPacketHead(pSend,index);
	CMainSocket.PutBYTE(pSend,bSend_LOGIN,index);
	CMainSocket.PutBYTE(pSend,(BYTE)iResult,index);

	if(iResult == bLogin_SUCCESS){
		CMainSocket.PutBYTE(pSend,bSendByte,index);
		CMainSocket.PutBYTE(pSend,(BYTE)iRemainingDays,index);
	}
	else
		index += 2;

	if(iResult == bLogin_SUCCESS || iResult == bLogin_INUSE){
		CMainSocket.PutWORD(pSend,(WORD)iUserIndex,index);
		CMainSocket.PutWORD(pSend,(WORD)iUserID,index);
		if(iResult == bLogin_SUCCESS){
			CMainSocket.PutWORD(pSend,(WORD)iSourceID,index);
			CMainSocket.PutDWORD(pSend,(DWORD)lSockHandle,index);
			CMainSocket.PutBYTE(pSend,bSendFixAdd,index);
			CMainSocket.PutDWORD(pSend,dwHDNo,index);
			CMainSocket.PutDWORD(pSend,dwARG,index);
		}
		else
			index += 15;

		CMainSocket.PutWORD(pSend,(WORD)iLogOutKey,index);
	}
	else
		index += 21;

	CMainSocket.PutPacketTail(pSend,index);

	CMainSocket.Encrypt(pSend,29);

	int iSent = 0;
	int iRet;
	do
	{
		iRet = send(pSocket->m_hSocket,pSend+iSent,29-iSent,0);
		if(iRet == -1)
			break;
		iSent += iRet;
	}while(iSent < 29);
}

void __fastcall TfrmServer::btncleardatebanClick(TObject *Sender)
{
	lstdateban->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnclearidbanClick(TObject *Sender)
{
	lstidban->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnadddatebanClick(TObject *Sender)
{
	if(txtdateban->Text.Length())
		lstdateban->AddItem(txtdateban->Text,Sender);
}

void __fastcall TfrmServer::btnaddidbanClick(TObject *Sender)
{
	if(txtidban->Text.Length())
		lstidban->AddItem(txtidban->Text,Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::chdatebanClick(TObject *Sender)
{
	bBanDate = chdateban->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::chidbanClick(TObject *Sender)
{
	bBanID = chidban->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnsavedatebanClick(TObject *Sender)
{
	if(!lstdateban->Items->Count)
		return;

	for (int i = 0; i < lstdateban->Items->Count; i++) {
		WriteLog("date_ban_list.txt",lstdateban->Items->operator [](i).c_str());
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnsaveidbanClick(TObject *Sender)
{
	if(!lstidban->Items->Count)
		return;

	for (int i = 0; i < lstidban->Items->Count; i++) {
		WriteLog("id_ban_list.txt",lstidban->Items->operator [](i).c_str());
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnloaddatebanClick(TObject *Sender)
{
	LoadDateList();
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnloadidbanClick(TObject *Sender)
{
	LoadIDList();
}
//---------------------------------------------------------------------------

void LoadDateList(){

	frmServer->lstdateban->Clear();

	char Buffer[33];
	ifstream fDateList ("date_ban_list.txt");
	if(!fDateList.is_open()) return;

	memset(Buffer,0,33);
	bool retLine;

	while(1){
		retLine = fDateList.getline(Buffer,32);
		if(!retLine)break;
		if(!strlen(Buffer)) continue;
		frmServer->lstdateban->AddItem(Buffer,0);
	}
}

void LoadIDList(){

	frmServer->lstidban->Clear();

	char Buffer[33];
	ifstream fIDList ("id_ban_list.txt");
	if(!fIDList.is_open()) return;

	memset(Buffer,0,33);
	bool retLine;

	while(1){
		retLine = fIDList.getline(Buffer,32);
		if(!retLine)break;
		if(!strlen(Buffer)) continue;
		frmServer->lstidban->AddItem(Buffer,0);
	}
}

void LoadNumList(){

	frmServer->lstnoban->Clear();

	char Buffer[33];
	ifstream fNoList ("no_ban_list.txt");
	if(!fNoList.is_open()) return;

	memset(Buffer,0,33);
	bool retLine;

	while(1){
		retLine = fNoList.getline(Buffer,32);
		if(!retLine)break;
		if(!strlen(Buffer)) continue;
		frmServer->lstnoban->AddItem(Buffer,0);
	}
}

void __fastcall TfrmServer::btnclearnobanClick(TObject *Sender)
{
	lstnoban->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnaddnobanClick(TObject *Sender)
{
	if(txtnoban->Text.Length())
		lstnoban->AddItem(txtnoban->Text,Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::chnobanClick(TObject *Sender)
{
	bBanNum = chnoban->Checked;
}
//---------------------------------------------------------------------------


void __fastcall TfrmServer::btnsavenobanClick(TObject *Sender)
{
	if(!lstnoban->Items->Count)
		return;

	for (int i = 0; i < lstnoban->Items->Count; i++) {
		WriteLog("no_ban_list.txt",lstnoban->Items->operator [](i).c_str());
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::tmusercheckTimer(TObject *Sender)
{
	int iCount = lstUserList.size();
	char tmp[8];
	itoa(iCount,tmp,10);
	lbcurrentcount->Caption = tmp;

	if(!iCount)
		return;

	EnterCriticalSection(&csProcess3);

	itUL it 	= lstUserList.begin();
	itUL itEnd 	= lstUserList.end();

	for (; it != itEnd; it++)
	{
		if( !(DWORD)(*it) )
			continue;

		(*it)->m_iCheckInterval++;

		if((*it)->m_iCheckInterval >= iMaxUserInterval)
		{
			delete (*it);	//free yapmayi unutmayalim :D
			lstUserList.erase(it);
		}
	}

	LeaveCriticalSection(&csProcess3);

}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::chfreeClick(TObject *Sender)
{
	bFree = chfree->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::chfreeipClick(TObject *Sender)
{
	bFreeIP = chfreeip->Checked;
}
//---------------------------------------------------------------------------

void Recv_Check(CNewConnection *pSocket, char* pBuf, int &index){

	SYSTEMTIME pDate;
	SYSTEMTIME pDate2;
	char* pStrLog;
	char* tmp;
	int iLen;
	short iUserIndex = CMainSocket.GetWORD(pBuf,index);
	short iUserID = CMainSocket.GetWORD(pBuf,index);

	int iSourceID = 0;
	long lSockHandle = -1;

	itUL it,itEnd;

	int iResult = bCheck_NOLOGIN;

	if(frmServer->bCheckPaused){
		iResult = bCheck_PAUSED;
		goto RETN;
	}

	if(iUserIndex == -1 || iUserID == -1 )
	{
		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}
		goto RETN;
	}

	if(!lstUserList.size())
		goto RETN;	//bCheck_NOLOGIN

	EnterCriticalSection(&csProcess3);

	it 		= lstUserList.begin();
	itEnd 	= lstUserList.end();

	for (; it != itEnd; it++) {
		if((*it)->m_iUserIndex == iUserIndex && (*it)->m_iUserID == iUserID){
			if(frmServer->bFreeIP || frmServer->bFree || (*it)->m_bFreeIP){

				if(iUserID == 425 && !frmServer->bFreeTest)	//DENEME KARTI
				{
					GetLocalTime(&pDate2);
					if(pDate2.wHour < 18 || pDate2.wHour >= 21)	//18 - 21
					{
						iResult = bCheck_TIMEOUT;
						delete (*it);	//free yapmayi unutmayalim :D
						lstUserList.erase(it);
						LeaveCriticalSection(&csProcess3);
						goto RETN;
					}
				}

				(*it)->m_iCheckInterval = 0;
				iResult = bCheck_SUCCESS;
				iSourceID = (*it)->m_sSourceID;
				lSockHandle = (*it)->m_lSockHandle;
			}else{

				//if(strcmp((*itUser)->m_strIP,pSocket->m_strIP) == 0){
					(*it)->m_iCheckInterval = 0;
					iResult = bCheck_SUCCESS;
					iSourceID = (*it)->m_sSourceID;
					lSockHandle = (*it)->m_lSockHandle;
				//}

				//else
				//{
					//iResult = bCheck_WRONGIP;
					//delete (*itUser);	//free yapmayi unutmayalim :D
					//lstUserList.erase(itUser);
				//}
			}
			LeaveCriticalSection(&csProcess3);
			goto RETN;
		}
	}

	LeaveCriticalSection(&csProcess3);

RETN:
	Send_Check(pSocket,iResult,iSourceID,lSockHandle);

	if(frmServer->bLogKeys){
		GetLocalTime(&pDate);

		pStrLog = new char[64];
		memset(pStrLog,0x00,64);

		tmp = new char[32];
		memset(tmp,0,32);

		itoa(pDate.wDay,tmp,10);
		strcpy(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wMonth,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wYear,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," ");

		itoa(pDate.wHour,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMinute,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wSecond,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMilliseconds,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," --- ");

		switch(iResult){

			case bCheck_NOLOGIN:
			strcat(pStrLog,"ERROR_NOLOGIN: [ID: ");
			itoa(iUserID,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog," Index: ");
			itoa(iUserIndex,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"]");
			break;

			case bCheck_SUCCESS:
			if(frmServer->bFree)
				strcat(pStrLog,"SUCCESS (FREE): [ID: ");
			else
				strcat(pStrLog,"SUCCESS: [ID: ");
			itoa(iUserID,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog," Index: ");
			itoa(iUserIndex,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"]");
			break;

			case bCheck_WRONGIP:
			strcat(pStrLog,"ERROR_WRONGIP: [ID: ");
			itoa(iUserID,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog," Index: ");
			itoa(iUserIndex,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"]");
			break;

			case bCheck_PAUSED:
			strcat(pStrLog,"ERROR_PAUSED: [ID: ");
			itoa(iUserID,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog," Index: ");
			itoa(iUserIndex,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"]");
			break;

			default:
			strcat(pStrLog,"ERROR_UNKNOWN: [ID: ");
			itoa(iUserID,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog," Index: ");
			itoa(iUserIndex,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"]");
			break;

		}

		strcat(pStrLog," - ");
		strcat(pStrLog,pSocket->m_strIP);
		WriteLog("check_log.txt",pStrLog);

		delete[] tmp;
		delete[] pStrLog;
	}
}

void Send_Check(CNewConnection *pSocket, int iResult, int iSourceID, long lSockHandle){
	char pSend[13];
	memset(pSend,0,13);
	int index(0);

	CMainSocket.PutPacketHead(pSend,index);
	CMainSocket.PutBYTE(pSend,bSend_CHECK,index);
	CMainSocket.PutBYTE(pSend,(BYTE)iResult,index);

	if(iResult == bCheck_SUCCESS){
		CMainSocket.PutBYTE(pSend,bSendByte,index);
		CMainSocket.PutWORD(pSend,(WORD)iSourceID,index);
		CMainSocket.PutDWORD(pSend,lSockHandle,index);
	}
	else
		index += 7;

	CMainSocket.PutPacketTail(pSend,index);

	CMainSocket.Encrypt(pSend,13);

	int iSent = 0;
	int iRet;
	do
	{
		iRet = send(pSocket->m_hSocket,pSend+iSent,13-iSent,0);
		if(iRet == -1)
			break;
		iSent += iRet;
	}while(iSent < 13);
}

void Recv_Logout(CNewConnection *pSocket, char* pBuf, int &index){

	SYSTEMTIME pDate;
	char* pStrLog;
	char* tmp;
	int iLen;
	short iUserIndex = CMainSocket.GetWORD(pBuf,index);
	short iUserID = CMainSocket.GetWORD(pBuf,index);
	short iLogOutKey = CMainSocket.GetWORD(pBuf,index);
	bool iError = 0;
	bool bFound = false;

	itUL it,itEnd;

	if(frmServer->bLogoutPaused)
		goto RETN;

	if(iUserIndex == -1 || iUserID == -1 )
	{
		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}
		return;
	}

	if( !iLogOutKey )
	{
		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}
		return;
	}

	if(!lstUserList.size())
		return;

	EnterCriticalSection(&csProcess3);
	it 		= lstUserList.begin();
	itEnd 	= lstUserList.end();

	for (; it != itEnd; it++)
	{
		if((*it)->m_iUserIndex == iUserIndex && (*it)->m_iUserID == iUserID)
		{
			bFound = true;
			if( (*it)->m_sLogOutKey == iLogOutKey )
			{
				if(frmServer->bFreeIP || frmServer->bFree || (*it)->m_bFreeIP)
				{
					delete (*it);
					lstUserList.erase(it);
				}
				else
				{
					//if(strcmp((*itUser)->m_strIP,pSocket->m_strIP) == 0)
					//{
						delete (*it);
						lstUserList.erase(it);
					//}
					//else
						//iError = 2;
				}
			}
			else
				iError = 1;
		}
	}

	LeaveCriticalSection(&csProcess3);

	if(!bFound)
		iError = 3;

RETN:

	if(frmServer->bLogKeys){
		GetLocalTime(&pDate);

		pStrLog = new char[64];
		memset(pStrLog,0x00,64);

		tmp = new char[32];
		memset(tmp,0,32);

		itoa(pDate.wDay,tmp,10);
		strcpy(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wMonth,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wYear,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," ");

		itoa(pDate.wHour,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMinute,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wSecond,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMilliseconds,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," --- ");

		if(frmServer->bLogoutPaused)
			strcat(pStrLog,"LOGOUT (PAUSED) [ID: ");
		else
		{
			if(iError)
			{
				if(iError == 1)
					strcat(pStrLog,"WRONG_KEY [ID: ");
				else if(iError == 2)
					strcat(pStrLog,"WRONG_IP [ID: ");
				else if(iError == 3)
					strcat(pStrLog,"NOTFOUND [ID: ");

			}
			else
			{
				if(frmServer->bFree)
					strcat(pStrLog,"LOGOUT (FREE) [ID: ");
				else
					strcat(pStrLog,"LOGOUT [ID: ");
			}
		}
		itoa(iUserID,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," Index: ");
		itoa(iUserIndex,tmp,10);
		strcat(pStrLog,tmp);

		strcat(pStrLog,"] - ");
		strcat(pStrLog,pSocket->m_strIP);
		WriteLog("logout_log.txt",pStrLog);

		delete[] tmp;
		delete[] pStrLog;
	}
}

int GetNewUserIndex(){

	int iNewIndex = 0;
	bool bOK = true;

	if(!lstUserList.size())
		return iNewIndex;

	EnterCriticalSection(&csProcess3);
	itUL it 		= lstUserList.begin();
	itUL itEnd 		= lstUserList.end();

	lstUserList.sort(SortUserList);

	while(1){

		bOK = true;

		for (it; it != itEnd; it++) {
			if((*it)->m_iUserIndex == iNewIndex){
				bOK = false;
				break;
			}
		}

		if(bOK)
			break;

		iNewIndex++;

	}

	LeaveCriticalSection(&csProcess3);

	return iNewIndex;

}

bool SortUserList(CUser* User1, CUser* User2){

	if(User1->m_iUserIndex < User2->m_iUserIndex)
		return true;
	else
		return false;
}

void Recv_BanOther(CNewConnection *pSocket, char* pBuf, int &index){

	SYSTEMTIME pDate;
	char* pStrLog;
	char* tmp;
	int iLen;
	short iUserIndex = CMainSocket.GetWORD(pBuf,index);
	short iUserID = CMainSocket.GetWORD(pBuf,index);
	int iLogOutKey = CMainSocket.GetWORD(pBuf,index);
	bool bFound = false;
	int iError = 0;

	itUL it,itEnd;

	if(frmServer->bBanPaused)
		goto RETN;

	if(iUserIndex == -1 || iUserID == -1)
	{
		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}
		return;
	}

	if( !iLogOutKey )
	{
		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}
		return;
	}

	if(!lstUserList.size())
		return;

	EnterCriticalSection(&csProcess3);
	it 		= lstUserList.begin();
	itEnd 	= lstUserList.end();

	for (; it != itEnd; it++)
	{
		if((*it)->m_iUserIndex == iUserIndex && (*it)->m_iUserID == iUserID)
		{
			bFound = true;
			if( (*it)->m_sLogOutKey == iLogOutKey )
			{
				delete (*it);
				lstUserList.erase(it);
			}
			else
				iError = 1;
		}
	}

	LeaveCriticalSection(&csProcess3);

	if(!bFound)
		iError = 2;

RETN:

	if(frmServer->bLogKeys){
		GetLocalTime(&pDate);

		pStrLog = new char[64];
		memset(pStrLog,0x00,64);

		tmp = new char[32];
		memset(tmp,0,32);

		itoa(pDate.wDay,tmp,10);
		strcpy(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wMonth,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wYear,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," ");

		itoa(pDate.wHour,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMinute,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wSecond,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMilliseconds,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," --- ");

		if(frmServer->bBanPaused)
			strcat(pStrLog,"BANOTHER (PAUSED) [ID: ");
		else
		{
			if(iError)
			{
				if(iError == 1)
					strcat(pStrLog,"WRONG_KEY [ID: ");
				else if(iError == 2)
					strcat(pStrLog,"NOTFOUND [ID: ");
			}
			else
				strcat(pStrLog,"BANOTHER [ID: ");
		}

		itoa(iUserID,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," Index: ");
		itoa(iUserIndex,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,"]");

		strcat(pStrLog,"] - ");
		strcat(pStrLog,pSocket->m_strIP);
		WriteLog("logout_log.txt",pStrLog);

		delete[] tmp;
		delete[] pStrLog;
	}
}

void Recv_Register(CNewConnection *pSocket, char* pBuf, int &index){

	int iResult;

	char* pStrLog;
	char* tmp = new char[32];
	memset(tmp,0,32);
	char* strCMD = NULL;
	int iLen;
	char strKey[21];
	char strID[21];
	char strPW[21];
	int KeyUsed_ID,IDUsed_ID;

	SYSTEMTIME pDate;
	long lCurrentTime;

	int iNewID;

	memset(strKey,0,21);
	memset(strID,0,21);
	memset(strPW,0,21);

	BYTE	bKeyNo 		= 	CMainSocket.GetBYTE(pBuf,index);
	BYTE	bKeyLimit 	= 	CMainSocket.GetBYTE(pBuf,index);
	short	sRand1 		= 	CMainSocket.GetWORD(pBuf,index);
	short	sRand2 		= 	CMainSocket.GetWORD(pBuf,index);
	long	lKey 		= 	CMainSocket.GetDWORD(pBuf,index);
	int		iIDLen		=	CMainSocket.GetBYTE(pBuf,index);
	if(iIDLen <= 20)
		CMainSocket.GetSTR(strID,pBuf,iIDLen,index);
	int		iPWLen		=	CMainSocket.GetBYTE(pBuf,index);
	if(iPWLen <= 20)
		CMainSocket.GetSTR(strPW,pBuf,iPWLen,index);

	ltoa(bKeyNo,tmp,16);
	iLen = strlen(tmp);
	if(iLen < 2){
		memmove(tmp+(2-iLen),tmp,iLen);
		memset(tmp,0x30,2-iLen);
	}
	strcat(strKey,tmp);

	ltoa(bKeyLimit,tmp,16);
	iLen = strlen(tmp);
	if(iLen < 2){
		memmove(tmp+(2-iLen),tmp,iLen);
		memset(tmp,0x30,2-iLen);
	}
	strcat(strKey,tmp);

	ltoa(sRand1,tmp,16);
	iLen = strlen(tmp);
	if(iLen < 4){
		memmove(tmp+(4-iLen),tmp,iLen);
		memset(tmp,0x30,4-iLen);
	}
	strcat(strKey,tmp);

	ltoa(sRand2,tmp,16);
	iLen = strlen(tmp);
	if(iLen < 4){
		memmove(tmp+(4-iLen),tmp,iLen);
		memset(tmp,0x30,4-iLen);
	}
	strcat(strKey,tmp);

	ltoa(lKey,tmp,16);
	iLen = strlen(tmp);
	if(iLen < 8){
		memmove(tmp+(8-iLen),tmp,iLen);
		memset(tmp,0x30,8-iLen);
	}
	strcat(strKey,tmp);

	long lRealKey = ((sRand1 * bKeyNo) + (sRand2 * bKeyLimit)) * (sRand1 + sRand2 + 8095);

	if(frmServer->bRegisterPaused){
		iResult = bRegister_PAUSED;
		goto RETN;
	}

	if(!bKeyNo || !bKeyLimit || !sRand1 || !sRand2 || !lKey){
		iResult = bRegister_INVALID;
		goto RETN;
	}

	/*
	if(sRand1 < 1000 || sRand2 < 1000 || lKey < 2000)
	{
		iResult = bRegister_INVALID;
		goto RETN;
	}
	*/

	if(!iIDLen || iIDLen < 4 || iIDLen > 20){
		iResult = bRegister_IDPWERROR;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}

		goto RETN;
	}

	if(!iPWLen || iIDLen < 4 || iPWLen > 20){
		iResult = bRegister_IDPWERROR;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}

		goto RETN;
	}

	if(lRealKey != lKey){
		iResult = bRegister_INVALID;
		goto RETN;
	}

	if(frmServer->bBanNum)
	{	if(frmServer->lstnoban->Items->Count)
		{	for (int i = 0; i < frmServer->lstnoban->Items->Count; i++)
			{	if(bKeyNo == atoi(frmServer->lstnoban->Items->operator [](i).c_str()))
				{	iResult = bRegister_BANNED;
					goto RETN;
				}
			}
		}
	}

	if( !strlen(strKey) || !strlen(strID) || !strlen(strPW) )
	{
		iResult = bRegister_IDPWERROR;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}

		goto RETN;
	}

	if( !CheckIllegal(strKey) )
	{
		iResult = bRegister_INVALID;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}

		goto RETN;
	}

	if( !CheckValidIDPW(strID) || !CheckValidIDPW(strPW) )
	{
		iResult = bRegister_IDPWERROR;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}

		goto RETN;
    }

	strCMD = new char[64];
	memset(strCMD,0,64);

	strcpy(strCMD,"select iUserID from userlist where strKey like '");
	strcat(strCMD,strKey);
	strcat(strCMD,"'");
	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	if(frmServer->SQLData->RecordCount){
		KeyUsed_ID = atoi(frmServer->SQLData->FieldByName("iUserID")->AsString.c_str());
		iResult = bRegister_KEYUSED;
		goto RETN;
	}

	memset(strCMD,0,64);
	strcpy(strCMD,"select iUserID from addlist where strAddKey like '");
	strcat(strCMD,strKey);
	strcat(strCMD,"'");
	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	if(frmServer->SQLData->RecordCount){
		KeyUsed_ID = atoi(frmServer->SQLData->FieldByName("iUserID")->AsString.c_str());
		iResult = bRegister_KEYUSED;
		goto RETN;
	}

	memset(strCMD,0,64);
	strcpy(strCMD,"select iUserID from userlist where strID like '");
	strcat(strCMD,strID);
	strcat(strCMD,"'");
	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	if(frmServer->SQLData->RecordCount){
		IDUsed_ID = atoi(frmServer->SQLData->FieldByName("iUserID")->AsString.c_str());
		iResult = bRegister_INUSE;
		goto RETN;
	}

	memset(strCMD,0,64);
	strcpy(strCMD,"select iUserID from userlist");
	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	iNewID = frmServer->SQLData->RecordCount;

	GetSystemTime(&pDate);
	lCurrentTime = ((pDate.wYear - 2000) * 365) + ((pDate.wMonth - 1) * 30) + pDate.wDay;

	ltoa(iNewID,tmp,10);

	memset(strCMD,0,64);
	strcpy(strCMD,"insert into userlist values (");
	strcat(strCMD,tmp);
	strcat(strCMD,",'");
	strcat(strCMD,strKey);
	strcat(strCMD,"','");
	strcat(strCMD,strID);
	strcat(strCMD,"','");
	strcat(strCMD,strPW);
	strcat(strCMD,"',");

	ltoa(lCurrentTime,tmp,10);
	strcat(strCMD,tmp);
	strcat(strCMD,",");

	ltoa(bKeyLimit,tmp,10);
	strcat(strCMD,tmp);
	strcat(strCMD,",0,0,");

	ltoa(bKeyNo,tmp,10);
	strcat(strCMD,tmp);
	strcat(strCMD,",0)");

	frmServer->SQLCon->Execute(strCMD);

	iResult = bRegister_SUCCESS;

	iTodayRegisterCount++;
	itoa(iTodayRegisterCount,tmp,10);
	frmServer->lbcurrentregistercount->Caption = tmp;

	iRegisterCount++;
	itoa(iRegisterCount,tmp,10);
	frmServer->lbregistercount->Caption = tmp;

RETN:

	Send_Register(pSocket,iResult,bKeyLimit);

	if(frmServer->bLogKeys){

		GetLocalTime(&pDate);

		pStrLog = new char[128];
		memset(pStrLog,0x00,128);
		memset(tmp,0,32);

		itoa(pDate.wDay,tmp,10);
		strcpy(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wMonth,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wYear,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," ");

		itoa(pDate.wHour,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMinute,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wSecond,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMilliseconds,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," --- ");

		switch(iResult){


			case bRegister_INUSE:
			strcat(pStrLog,"ERROR_ID_USED: [ID: ");
			itoa(IDUsed_ID,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"] - [");
			break;

			case bRegister_SUCCESS:
			strcat(pStrLog,"SUCCESS: [ID: ");
			itoa(iNewID,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"] - [");
			break;

			case bRegister_BANNED:
			strcat(pStrLog,"ERROR_BANNED: [KEYNO: ");
			itoa(bKeyNo,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"] - [");
			break;

			case bRegister_KEYUSED:
			strcat(pStrLog,"ERROR_KEY_USED: [ID: ");
			itoa(KeyUsed_ID,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"] - [");
			break;

			case bRegister_IDPWERROR:
			strcat(pStrLog,"ERROR_IDPW: [");
			break;

			case bRegister_INVALID:
			strcat(pStrLog,"ERROR_INVALID_KEY: [");
			break;

			case bRegister_PAUSED:
			strcat(pStrLog,"ERROR_PAUSED: [");

			default:
			strcat(pStrLog,"ERROR_UNKNOWN: [");
			break;

		}
		strcat(pStrLog,strID);
		strcat(pStrLog," - ");
		strcat(pStrLog,strPW);
		strcat(pStrLog," / ");
		strcat(pStrLog,strKey);

		strcat(pStrLog,"] - ");
		strcat(pStrLog,pSocket->m_strIP);
		WriteLog("register_log.txt",pStrLog);

		delete[] pStrLog;
		}

	delete[] tmp;
	if(strCMD)
		delete[] strCMD;

}

void Send_Register(CNewConnection *pSocket, int iResult, int iDays){

	char pSend[7];
	memset(pSend,0,7);

	int index(0);
	CMainSocket.PutPacketHead(pSend,index);
	CMainSocket.PutBYTE(pSend,bSend_REGISTER,index);
	CMainSocket.PutBYTE(pSend,(BYTE)iResult,index);

	if(iResult == bRegister_SUCCESS)
		CMainSocket.PutBYTE(pSend,(BYTE)iDays,index);
	else
		index++;

	CMainSocket.PutPacketTail(pSend,index);

	CMainSocket.Encrypt(pSend,7);

	int iSent = 0;
	int iRet;
	do
	{
		iRet = send(pSocket->m_hSocket,pSend+iSent,7-iSent,0);
		if(iRet == -1)
			break;
		iSent += iRet;
	}while(iSent < 7);

}

void __fastcall TfrmServer::btnfindnoClick(TObject *Sender)
{
	lstfindno->Clear();

	char* strCMD = new char[64];
	memset(strCMD,0,64);

	strcpy(strCMD,"select iUserID from userlist where iKeyNo = ");
	strcat(strCMD,txtfindno->Text.c_str());

	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	int iFound = frmServer->SQLData->RecordCount;
	if(!iFound){
		delete[] strCMD;
		return;
	}

	for (int i = 0; i < iFound; i++) {
		lstfindno->AddItem(SQLData->Fields->operator [](i)->AsString.c_str(),Sender);
	}

	delete[] strCMD;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnmovefilterClick(TObject *Sender)
{
	if(!lstfindno->Count)
		return;

	for (int i = 0; i < lstfindno->Count; i++) {
		lstidban->AddItem(lstfindno->Items->operator [](i),Sender);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmServer::btnbanallClick(TObject *Sender)
{
	if(!lstfindno->Count)
		return;

	char* strCMD = new char[64];
	memset(strCMD,0,64);

	strcpy(strCMD,"update userlist set bBanned = 1 where iUserID = ");
	int y = strlen(strCMD);

	for (int i = 0; i < lstfindno->Count; i++) {
		strcpy(strCMD + y,lstfindno->Items->operator [](i).c_str());
		frmServer->SQLCon->Execute(strCMD);
	}

	delete[] strCMD;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnfindidClick(TObject *Sender)
{
	char* strCMD = new char[64];
	memset(strCMD,0,64);

	strcpy(strCMD,"select iUserID from userlist where strID like '");
	strcat(strCMD,txtfindid->Text.c_str());
	strcat(strCMD,"'");

	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	if(!SQLData->RecordCount){
		delete[] strCMD;
		return;
	}

	txtfoundid->Text = SQLData->Fields->operator [](0)->AsString.c_str();

	delete[] strCMD;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnlistdateClick(TObject *Sender)
{
	lstfoundids->Clear();
	char* strCMD = new char[64];
	memset(strCMD,0,64);

	strcpy(strCMD,"select iUserID from userlist where iDate = ");
	strcat(strCMD,txtfinddate->Text.c_str());

	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	if(!SQLData->RecordCount){
		delete[] strCMD;
		return;;
	}

	for (int i = 0; i < SQLData->RecordCount; i++) {
		lstfoundids->AddItem(SQLData->Fields->operator [](i)->AsString.c_str(),Sender);
	}

	delete[] strCMD;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnlistbanClick(TObject *Sender)
{
	lstfoundids->Clear();
	char* strCMD = new char[64];
	memset(strCMD,0,64);

	strcpy(strCMD,"select iUserID from userlist where bBanned = 1");

	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	if(!SQLData->RecordCount){
		delete[] strCMD;
		return;
	}

	for (int i = 0; i < SQLData->RecordCount; i++) {
		lstfoundids->AddItem(SQLData->Fields->operator [](i)->AsString.c_str(),Sender);
	}

	delete[] strCMD;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnbanallfoundClick(TObject *Sender)
{
	if(!lstfoundids->Count)
		return;

	char* strCMD = new char[64];
	memset(strCMD,0,64);

	strcpy(strCMD,"update userlist set bBanned = 1 where iUserID = ");
	int y = strlen(strCMD);

	for (int i = 0; i < lstfoundids->Count; i++) {
		strcpy(strCMD + y,lstfoundids->Items->operator [](i).c_str());
		frmServer->SQLCon->Execute(strCMD);
	}

	delete[] strCMD;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btngetidinfoClick(TObject *Sender)
{
	char* strCMD = new char[64];
	memset(strCMD,0,64);

	strcpy(strCMD,"select * from userlist where iUserID = ");
	strcat(strCMD,txtidno->Text.c_str());

	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	if(!SQLData->RecordCount){
		delete[] strCMD;
		return;
	}

	txtidkey->Text = SQLData->FieldByName("strKey")->AsString.c_str();
	txtidid->Text = SQLData->FieldByName("strID")->AsString.c_str();
	txtidpw->Text = SQLData->FieldByName("strPW")->AsString.c_str();
	txtiddays->Text = SQLData->FieldByName("iDays")->AsString.c_str();
	txtiddate->Text = SQLData->FieldByName("iDate")->AsString.c_str();
	txtidextra->Text = SQLData->FieldByName("iExtraDays")->AsString.c_str();
	txtidkeyno->Text = SQLData->FieldByName("iKeyno")->AsString.c_str();
	chidbanned->Checked = (bool)atoi(SQLData->FieldByName("bBanned")->AsString.c_str());
	chidfreeip->Checked = (bool)atoi(SQLData->FieldByName("bFreeIP")->AsString.c_str());

	delete[] strCMD;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnsaveidClick(TObject *Sender)
{
	char* strCMD = new char[128];
	memset(strCMD,0,128);

	strcpy(strCMD,"update userlist set strKey = '");
	strcat(strCMD,txtidkey->Text.c_str());
	strcat(strCMD,"',strID = '");
	strcat(strCMD,txtidid->Text.c_str());
	strcat(strCMD,"',strPW = '");
	strcat(strCMD,txtidpw->Text.c_str());
	strcat(strCMD,"',iDate = ");
	strcat(strCMD,txtiddate->Text.c_str());
	strcat(strCMD,",iDays = ");
	strcat(strCMD,txtiddays->Text.c_str());
	strcat(strCMD,",bBanned = ");
	if(chidbanned->Checked)
		strcat(strCMD,"1");
	else
		strcat(strCMD,"0");
	strcat(strCMD,",iExtraDays = ");
	strcat(strCMD,txtidextra->Text.c_str());
	strcat(strCMD,",iKeyNo = ");
	strcat(strCMD,txtidkeyno->Text.c_str());
	strcat(strCMD,",bFreeIP = ");
	if(chidfreeip->Checked)
		strcat(strCMD,"1");
	else
		strcat(strCMD,"0");

	strcat(strCMD," where iUserID = ");
	strcat(strCMD,txtidno->Text.c_str());

	int iRetn;
	frmServer->SQLCon->Execute(strCMD,iRetn);

	delete[] strCMD;

	if(iRetn)
		MessageBox(this->Handle,"user basariyla guncellendi.","guncelleme",MB_ICONINFORMATION);
	else
		MessageBox(this->Handle,"guncelleme basarisiz!","guncelleme",MB_ICONEXCLAMATION);
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnclearonlinesClick(TObject *Sender)
{
	ClearOnlineList();
}
//---------------------------------------------------------------------------

void ClearOnlineList(){

	if(!lstUserList.size())
		return;

	itUL itUser;

	EnterCriticalSection(&csProcess3);

	for (itUser = lstUserList.begin() ; itUser != lstUserList.end(); itUser++) {
		delete (*itUser);

		lstUserList.erase(itUser);

	}

	lstUserList.clear();

	LeaveCriticalSection(&csProcess3);
}

void Recv_Add(CNewConnection *pSocket, char* pBuf, int &index){

	int iResult;

	char* pStrLog;
	char* tmp = new char[32]; memset(tmp,0,32);
	char* strCMD = NULL;
	int iLen;
	char strKey[21];
	char strID[21];
	char strPW[21];
	int KeyUsed_ID;
	bool bUserBanned;
	int iUserDate,iUserID,iUserDays,iUserExtraDays,iUserKeyNo;

	SYSTEMTIME pDate;
	long lCurrentTime;

	memset(strKey,0,21);
	memset(strID,0,21);
	memset(strPW,0,21);

	BYTE	bKeyNo 		= 	CMainSocket.GetBYTE(pBuf,index);
	BYTE	bKeyLimit 	= 	CMainSocket.GetBYTE(pBuf,index);
	short	sRand1 		= 	CMainSocket.GetWORD(pBuf,index);
	short	sRand2 		= 	CMainSocket.GetWORD(pBuf,index);
	long	lKey 		= 	CMainSocket.GetDWORD(pBuf,index);
	int		iIDLen		=	CMainSocket.GetBYTE(pBuf,index);
	if(iIDLen <= 20)
		CMainSocket.GetSTR(strID,pBuf,iIDLen,index);
	int		iPWLen		=	CMainSocket.GetBYTE(pBuf,index);
	if(iPWLen <= 20)
		CMainSocket.GetSTR(strPW,pBuf,iPWLen,index);

	ltoa(bKeyNo,tmp,16);
	iLen = strlen(tmp);
	if(iLen < 2){
		memmove(tmp+(2-iLen),tmp,iLen);
		memset(tmp,0x30,2-iLen);
	}
	strcat(strKey,tmp);

	ltoa(bKeyLimit,tmp,16);
	iLen = strlen(tmp);
	if(iLen < 2){
		memmove(tmp+(2-iLen),tmp,iLen);
		memset(tmp,0x30,2-iLen);
	}
	strcat(strKey,tmp);

	ltoa(sRand1,tmp,16);
	iLen = strlen(tmp);
	if(iLen < 4){
		memmove(tmp+(4-iLen),tmp,iLen);
		memset(tmp,0x30,4-iLen);
	}
	strcat(strKey,tmp);

	ltoa(sRand2,tmp,16);
	iLen = strlen(tmp);
	if(iLen < 4){
		memmove(tmp+(4-iLen),tmp,iLen);
		memset(tmp,0x30,4-iLen);
	}
	strcat(strKey,tmp);

	ltoa(lKey,tmp,16);
	iLen = strlen(tmp);
	if(iLen < 8){
		memmove(tmp+(8-iLen),tmp,iLen);
		memset(tmp,0x30,8-iLen);
	}
	strcat(strKey,tmp);

	long lRealKey = ((sRand1 * bKeyNo) + (sRand2 * bKeyLimit)) * (sRand1 + sRand2 + 8095);

	if(frmServer->bAddPaused){
		iResult = bAdd_PAUSED;
		goto RETN;
	}

	if(!bKeyNo || !bKeyLimit || !sRand1 || !sRand2 || !lKey){
		iResult = bAdd_KEYINVALID;
		goto RETN;
	}

	/*
	if(sRand1 < 1000 || sRand2 < 1000 || lKey < 2000)
	{
		iResult = bRegister_INVALID;
		goto RETN;
	}
	*/

	if(!iIDLen || iIDLen < 4 || iIDLen > 20){
		iResult = bAdd_IDPWINVALID;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}

		goto RETN;
	}

	if(!iPWLen || iPWLen < 4 || iPWLen > 20){
		iResult = bAdd_IDPWINVALID;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}

		goto RETN;
	}

	if(lRealKey != lKey){
		iResult = bAdd_KEYINVALID;
		goto RETN;
	}

	if(frmServer->bBanNum)
	{	if(frmServer->lstnoban->Items->Count)
		{	for (int i = 0; i < frmServer->lstnoban->Items->Count; i++)
			{	if(bKeyNo == atoi(frmServer->lstnoban->Items->operator [](i).c_str()))
				{	iResult = bAdd_KEYBANNED;
					goto RETN;
				}
			}
		}
	}

	if( !strlen(strKey) || !strlen(strID) || !strlen(strPW) )
	{
		iResult = bAdd_IDPWINVALID;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}

		goto RETN;
	}

	if( !CheckIllegal(strKey) )
	{
		iResult = bAdd_KEYINVALID;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}

		goto RETN;
	}

	if( !CheckValidIDPW(strID) || !CheckValidIDPW(strPW) )
	{
		iResult = bAdd_IDPWINVALID;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}

		goto RETN;
    }

	strCMD = new char[64];

	memset(strCMD,0,64);
	strcpy(strCMD,"select iUserID from userlist where strKey like '");
	strcat(strCMD,strKey);
	strcat(strCMD,"'");
	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	if(frmServer->SQLData->RecordCount){
		KeyUsed_ID = atoi(frmServer->SQLData->FieldByName("iUserID")->AsString.c_str());
		iResult = bAdd_KEYUSED;
		goto RETN;
	}

	memset(strCMD,0,64);
	strcpy(strCMD,"select iUserID from addlist where strAddKey like '");
	strcat(strCMD,strKey);
	strcat(strCMD,"'");
	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	if(frmServer->SQLData->RecordCount){
		KeyUsed_ID = atoi(frmServer->SQLData->FieldByName("iUserID")->AsString.c_str());
		iResult = bAdd_KEYUSED;
		goto RETN;
	}

	memset(strCMD,0,64);
	strcpy(strCMD,"select * from userlist where strID like '");
	strcat(strCMD,strID);
	strcat(strCMD,"' and strPW like '");
	strcat(strCMD,strPW);
	strcat(strCMD,"'");

	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	if(frmServer->SQLData->RecordCount != 1){
		iResult = bAdd_IDPWINVALID;
		goto RETN;
	}

	bUserBanned = (bool)atoi(frmServer->SQLData->FieldByName("bBanned")->AsString.c_str());
	iUserDate = atoi(frmServer->SQLData->FieldByName("iDate")->AsString.c_str());
	iUserID = atoi(frmServer->SQLData->FieldByName("iUserID")->AsString.c_str());
	iUserDays = atoi(frmServer->SQLData->FieldByName("iDays")->AsString.c_str());
	iUserExtraDays = atoi(frmServer->SQLData->FieldByName("iExtraDays")->AsString.c_str());
	iUserKeyNo = atoi(frmServer->SQLData->FieldByName("iKeyNo")->AsString.c_str());

	if(bUserBanned){
		iResult = bAdd_IDBANNED;
		goto RETN;
	}

	if(frmServer->bBanDate)
	{	if(frmServer->lstdateban->Items->Count)
		{	for (int i = 0; i < frmServer->lstdateban->Items->Count; i++)
			{	if(iUserDate == atol(frmServer->lstdateban->Items->operator [](i).c_str()))
				{	iResult = bAdd_IDBANNED;
					goto RETN;
				}
			}
		}
	}

	if(frmServer->bBanID)
	{	if(frmServer->lstidban->Items->Count)
		{	for (int i = 0; i < frmServer->lstidban->Items->Count; i++)
			{	if(iUserID == atoi(frmServer->lstidban->Items->operator [](i).c_str()))
				{	iResult = bAdd_IDBANNED;
					goto RETN;
				}
			}
		}
	}

	if(frmServer->bBanNum)
	{	if(frmServer->lstnoban->Items->Count)
		{	for (int i = 0; i < frmServer->lstnoban->Items->Count; i++)
			{	if(iUserKeyNo == atoi(frmServer->lstnoban->Items->operator [](i).c_str()))
				{	iResult = bAdd_IDBANNED;
					goto RETN;
				}
			}
		}
	}

	if(iUserDays == 0xFF){			//sinirsiz hesaba ek yapilmaz
		iResult = bAdd_NOTIMEOUT;
		goto RETN;
	}

	GetSystemTime(&pDate);
	lCurrentTime = ((pDate.wYear - 2000) * 365) + ((pDate.wMonth - 1) * 30) + pDate.wDay;

	if(lCurrentTime < (iUserDate + iUserDays + iUserExtraDays)){
			iResult = bAdd_NOTIMEOUT;
			goto RETN;
	}

	ltoa(lCurrentTime,tmp,10);

	memset(strCMD,0,64);
	strcpy(strCMD,"update userlist set iDate = ");
	strcat(strCMD,tmp);
	strcat(strCMD,",iDays = ");

	itoa(bKeyLimit,tmp,10);
	strcat(strCMD,tmp);
	strcat(strCMD,",iExtraDays = 0,iKeyNo = ");
	itoa(bKeyNo,tmp,10);
	strcat(strCMD,tmp);
	strcat(strCMD," where iUserID = ");
	itoa(iUserID,tmp,10);
	strcat(strCMD,tmp);

	frmServer->SQLCon->Execute(strCMD);

	memset(strCMD,0,64);
	itoa(iUserID,tmp,10);
	strcpy(strCMD,"insert into addlist values (");
	strcat(strCMD,tmp);
	strcat(strCMD,",'");
	strcat(strCMD,strKey);
	strcat(strCMD,"')");

	frmServer->SQLCon->Execute(strCMD);

	iResult = bAdd_SUCCESS;

	iTodayAddCount++;
	itoa(iTodayAddCount,tmp,10);
	frmServer->lbcurrentaddcount->Caption = tmp;

	iAddCount++;
	itoa(iAddCount,tmp,10);
	frmServer->lbaddcount->Caption = tmp;

RETN:

	Send_Add(pSocket,iResult,bKeyLimit);

	if(frmServer->bLogKeys){

		GetLocalTime(&pDate);

		pStrLog = new char[128];
		memset(pStrLog,0x00,128);
		memset(tmp,0,32);

		itoa(pDate.wDay,tmp,10);
		strcpy(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wMonth,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wYear,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," ");

		itoa(pDate.wHour,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMinute,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wSecond,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMilliseconds,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," --- ");

		switch(iResult){


			case bAdd_IDPWINVALID:
			strcat(pStrLog,"ERROR_IDPWINVALID: [");
			break;

			case bAdd_SUCCESS:
			strcat(pStrLog,"SUCCESS: [ID: ");
			itoa(iUserID,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog," DAYS: ");
			itoa(bKeyLimit,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"] - [");
			break;

			case bAdd_NOTIMEOUT:
			strcat(pStrLog,"ERROR_NOTIMEOUT: [ID: ");
			itoa(iUserID,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"] - [");
			break;

			case bAdd_IDBANNED:
			strcat(pStrLog,"ERROR_IDBANNED [ID: ");
			itoa(iUserID,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"] - [");
			break;

			case bAdd_KEYBANNED:
			strcat(pStrLog,"ERROR_KEYBANNED: [KEYNO: ");
			itoa(bKeyNo,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"] - [");
			break;

			case bAdd_KEYUSED:
			strcat(pStrLog,"ERROR_KEYUSED: [ID: ");
			itoa(KeyUsed_ID,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"] - [");
			break;

			case bAdd_KEYINVALID:
			strcat(pStrLog,"ERROR_KEYINVALID: [");
			break;

			case bAdd_PAUSED:
			strcat(pStrLog,"ERROR_PAUSED: [");
			break;

			default:
			strcat(pStrLog,"ERROR_UNKNOWN: [");
			break;

		}
		strcat(pStrLog,strID);
		strcat(pStrLog," - ");
		strcat(pStrLog,strPW);
		strcat(pStrLog," / ");
		strcat(pStrLog,strKey);

		strcat(pStrLog,"] - ");
		strcat(pStrLog,pSocket->m_strIP);
		WriteLog("add_log.txt",pStrLog);

		delete[] pStrLog;
		}

	delete[] tmp;
	if(strCMD)
		delete[] strCMD;

}

void Send_Add(CNewConnection *pSocket, int iResult, int iDays){

	char pSend[7];
	memset(pSend,0,7);

	int index(0);
	CMainSocket.PutPacketHead(pSend,index);
	CMainSocket.PutBYTE(pSend,bSend_ADD,index);
	CMainSocket.PutBYTE(pSend,(BYTE)iResult,index);

	if(iResult == bAdd_SUCCESS)
		CMainSocket.PutBYTE(pSend,(BYTE)iDays,index);
	else
		index++;

	CMainSocket.PutPacketTail(pSend,index);

	CMainSocket.Encrypt(pSend,7);

	int iSent = 0;
	int iRet;
	do
	{
		iRet = send(pSocket->m_hSocket,pSend+iSent,7-iSent,0);
		if(iRet == -1)
			break;
		iSent += iRet;
	}while(iSent < 7);

}

void __fastcall TfrmServer::chregisterpauseClick(TObject *Sender)
{
	bRegisterPaused = chregisterpause->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::chcheckpauseClick(TObject *Sender)
{
	bCheckPaused = chcheckpause->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::chaddpauseClick(TObject *Sender)
{
	bAddPaused = chaddpause->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::chlogoutpauseClick(TObject *Sender)
{
	bLogoutPaused = chlogoutpause->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::chbanpauseClick(TObject *Sender)
{
	bBanPaused = chbanpause->Checked;
}
//---------------------------------------------------------------------------

void Recv_Report(CNewConnection *pSocket, char* pBuf, int &index){

	short sSenderID = CMainSocket.GetWORD(pBuf,index);
	int	iReportSize = CMainSocket.GetBYTE(pBuf,index);

	if(!iReportSize || iReportSize > 128)
	{

		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}

		return;
	}

	if( sSenderID == -1 )
	{
		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
			return;
		}
	}

	itUL it,itEnd;

	EnterCriticalSection(&csProcess3);

	it 		= lstUserList.begin();
	itEnd 	= lstUserList.end();
	bool bFound = false;

	for (; it != itEnd; it++)
	{
		if( (*it)->m_iUserID == (int)sSenderID )
		{
			bFound = true;
			break;
        }
	}

	if(!bFound)
	{
		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}
		LeaveCriticalSection(&csProcess3);
		return;
	}
	else
	{
		if( GetTickCount() - (*it)->m_iLastReportTick <= 55000 )
		{
			if(bBanIP)
			{
				AddBanIP( inet_addr(pSocket->m_strIP) );
				LeaveCriticalSection(&csProcess3);
				return;
			}
        }
    }

	char* strReport = new char[iReportSize + 1];
	memset(strReport,0,iReportSize+1);

	CMainSocket.GetSTR(strReport,pBuf,iReportSize,index);

	//melih - koruma test :D 29.09.2011 - 16:26
	/*
	if(bBanIP)
	{
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
				AddBanIP( inet_addr(pSocket->m_strIP) );
				LeaveCriticalSection(&csProcess3);
				return;
			}
		}
	}
	*/

	char* pStrLog = new char[256];
	memset(pStrLog,0,256);

	char tmp[8];
	itoa(sSenderID,tmp,10);
	strcpy(pStrLog,"[");
	strcat(pStrLog,tmp);
	strcat(pStrLog,": ");
	strcat(pStrLog,strReport);
	strcat(pStrLog,"] - ");
	strcat(pStrLog,pSocket->m_strIP);

	WriteLog("reports.txt",pStrLog);

	(*it)->m_iLastReportTick = GetTickCount();
	LeaveCriticalSection(&csProcess3);

	delete [] pStrLog;
	delete [] strReport;
}

void __fastcall TfrmServer::btnlistfreeipClick(TObject *Sender)
{
	lstfoundids->Clear();
	char* strCMD = new char[64];
	memset(strCMD,0,64);

	strcpy(strCMD,"select iUserID from userlist where bFreeIP = 1");

	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	if(!SQLData->RecordCount){
		delete[] strCMD;
		return;
	}

	for (int i = 0; i < SQLData->RecordCount; i++) {
		lstfoundids->AddItem(SQLData->Fields->operator [](i)->AsString.c_str(),Sender);
	}

	delete[] strCMD;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnrefreshcountsClick(TObject *Sender)
{
char* strCMD = new char[64];

memset(strCMD,0,64);
strcpy(strCMD,"select iUserID from userlist");
SQLData->Close();
SQLData->CommandText = strCMD;
SQLData->Open();

iRegisterCount = SQLData->RecordCount;

char tmp[32];
itoa(iRegisterCount,tmp,10);

lbregistercount->Caption = tmp;

memset(strCMD,0,64);
strcpy(strCMD,"select iUserID from addlist");
SQLData->Close();
SQLData->CommandText = strCMD;
SQLData->Open();

iAddCount = SQLData->RecordCount;
itoa(iAddCount,tmp,10);
lbaddcount->Caption = tmp;

delete [] strCMD;

iLoginCount = 0;
iTodayRegisterCount = 0;
iTodayAddCount = 0;
lConnectionCount = 0;

lbcount->Caption = "0";
lblogincount->Caption = "0";
lbcurrentregistercount->Caption = "0";
lbcurrentaddcount->Caption = "0";
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnfindonlineClick(TObject *Sender)
{
	int iID = txtonlineid->Text.ToInt();
	int iFoundIndex = -1;

	if(!iID)
		return;

	if(lstUserList.empty())
		return;

	itUL itUser;

	for (itUser = lstUserList.begin() ; itUser != lstUserList.end(); itUser++) {
		if(iID == (*itUser)->m_iUserID){
			iFoundIndex = (*itUser)->m_iUserIndex;
			break;
		}
	}

	if(iFoundIndex == -1){
		lbonlineindex->Caption = "-1";
		return;
	}

	char tmp[16];
	itoa(iFoundIndex,tmp,10);
	lbonlineindex->Caption = tmp;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btndconlineClick(TObject *Sender)
{
	int iIndex = lbonlineindex->Caption.ToInt();
	if(iIndex == -1)
		return;

	if(lstUserList.empty())
		return;

	itUL itUser;

	for (itUser = lstUserList.begin() ; itUser != lstUserList.end(); itUser++) {
		if(iIndex == (*itUser)->m_iUserIndex){
			delete (*itUser);
			EnterCriticalSection(&csProcess3);
				lstUserList.erase(itUser);
			LeaveCriticalSection(&csProcess3);
			break;
		}
	}
}
//---------------------------------------------------------------------------

void Recv_Trial(CNewConnection *pSocket, char* pBuf, int &index){

	CUser* pUser;
	SYSTEMTIME pDate;
	long lCurrentTime;
	int iResult;
	char* pStrLog;
	char* tmp;
	char* strCMD = NULL;
	int iUserDate,iUserIndex;

	short sBotVersion;
	short sSourceID;
	long lSockHandle;

	char strAccountID[32];
	memset(strAccountID,0,32);
	int iIDLen;

	if(frmServer->bBanTrial){
		iResult = bTrial_DISABLED;
		goto RETN;
	}

	sBotVersion = CMainSocket.GetWORD(pBuf,index);
	sSourceID = CMainSocket.GetWORD(pBuf,index);
	lSockHandle = CMainSocket.GetDWORD(pBuf,index);

	if(sBotVersion != iServerVersion){
		iResult = bTrial_INVALIDV;
		goto RETN;
	}

	iIDLen = CMainSocket.GetBYTE(pBuf,index);
	if(iIDLen < 4 || iIDLen >= 32){
		iResult = bTrial_INVALID;
		goto RETN;
	}

	CMainSocket.GetSTR(strAccountID,pBuf,iIDLen,index);

	if( !strlen(strAccountID))
	{
		iResult = bTrial_INVALIDV;
		goto RETN;
	}

	StrDecrypt(strAccountID);

	if( !strlen(strAccountID))
	{
		iResult = bTrial_INVALIDV;
		goto RETN;
	}

	strCMD = new char[64];
	memset(strCMD,0,64);

	strcpy(strCMD,"select * from trialbanlist where strAccountID like '");
	strcat(strCMD,strAccountID);
	strcat(strCMD,"'");

	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	if(frmServer->SQLData->RecordCount > 0)
	{
		iResult = bTrial_BANNED;
		goto RETN;
    }

	memset(strCMD,0,64);

	strcpy(strCMD,"select * from triallist where strAccountID like '");
	strcat(strCMD,strAccountID);
	strcat(strCMD,"'");

	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	if(frmServer->SQLData->RecordCount > 0)
	{
		iUserDate = atoi(frmServer->SQLData->FieldByName("iDate")->AsString.c_str());

		GetSystemTime(&pDate);
		lCurrentTime = ((pDate.wYear - 2000) * 365) + ((pDate.wMonth - 1) * 30) + pDate.wDay;
		if(lCurrentTime - iUserDate >= 3){
			iResult = bTrial_IDUSED;
			goto RETN;
		}
		iResult = bTrial_SUCCESS;
	}
	else
	{
		memset(strCMD,0,64);
		GetSystemTime(&pDate);
		lCurrentTime = ((pDate.wYear - 2000) * 365) + ((pDate.wMonth - 1) * 30) + pDate.wDay;

		iUserDate = lCurrentTime;

		strcpy(strCMD,"insert into triallist values ('");
		strcat(strCMD,strAccountID);

		tmp = new char[32];
		memset(tmp,0,32);

		ltoa(lCurrentTime,tmp,10);
		strcat(strCMD,"',");
		strcat(strCMD,tmp);
		strcat(strCMD,")");

		frmServer->SQLCon->Execute(strCMD);

		delete [] tmp;
		iResult = bTrial_SUCCESS;
	}

RETN:

	if(iResult == bTrial_SUCCESS){

		iUserIndex = GetNewUserIndex();

		Send_Trial(pSocket,iResult,(iUserDate + 3) - lCurrentTime,iUserIndex,0,sSourceID,lSockHandle);

		pUser = new CUser(0,iUserIndex,true,sSourceID,lSockHandle,0,0,0);
		strcpy( pUser->m_strIP, pSocket->m_strIP );
		EnterCriticalSection(&csProcess3);
			lstUserList.push_back(pUser);
		LeaveCriticalSection(&csProcess3);
	}
	else
		Send_Trial(pSocket,iResult,0,0,0,0,0);

	if(frmServer->bLogKeys){

		GetLocalTime(&pDate);

		pStrLog = new char[128];
		memset(pStrLog,0x00,128);

		tmp = new char[32];
		memset(tmp,0,32);

		itoa(pDate.wDay,tmp,10);
		strcpy(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wMonth,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wYear,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," ");

		itoa(pDate.wHour,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMinute,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wSecond,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMilliseconds,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," --- ");

		switch(iResult){

			case bTrial_INVALID:
			strcat(pStrLog,"TRIAL_INVALID: [");
			break;

			case bTrial_SUCCESS:
			strcat(pStrLog,"TRIAL_SUCCESS: [Index:");
			itoa(iUserIndex,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"] - [");
			break;

			case bTrial_IDUSED:
			strcat(pStrLog,"TRIAL_USED: [");
			break;

			case bTrial_DISABLED:
			strcat(pStrLog,"TRIAL_DISABLED: [");
			break;

			default:
			strcat(pStrLog,"ERROR_UNKNOWN: [");
			break;

		}

		strcat(pStrLog,strAccountID);
		strcat(pStrLog,"] - ");
		strcat(pStrLog,pSocket->m_strIP);
		WriteLog("login_log.txt",pStrLog);

		delete[] pStrLog;
		delete[] tmp;
	}

	if(strCMD)
		delete[] strCMD;
}

void __fastcall TfrmServer::chbantrialClick(TObject *Sender)
{
	bBanTrial = chbantrial->Checked;
}
//---------------------------------------------------------------------------

void Send_Trial(CNewConnection *pSocket, int iResult, int iRemainingDays, int iUserIndex, int iUserID, int iSourceID, long lSockHandle){
	char pSend[18];
	memset(pSend,0,18);
	int index(0);

	CMainSocket.PutPacketHead(pSend,index);
	CMainSocket.PutBYTE(pSend,bSend_TRIAL,index);
	CMainSocket.PutBYTE(pSend,(BYTE)iResult,index);

	if(iResult == bTrial_SUCCESS){
		CMainSocket.PutBYTE(pSend,bSendByte,index);
		CMainSocket.PutBYTE(pSend,(BYTE)iRemainingDays,index);

		CMainSocket.PutWORD(pSend,(WORD)iUserIndex,index);
		CMainSocket.PutWORD(pSend,(WORD)iUserID,index);

		CMainSocket.PutWORD(pSend,(WORD)iSourceID,index);
		CMainSocket.PutDWORD(pSend,(DWORD)lSockHandle,index);
	}
	else
		index += 12;

	CMainSocket.PutPacketTail(pSend,index);

	CMainSocket.Encrypt(pSend,18);

	int iSent = 0;
	int iRet;
	do
	{
		iRet = send(pSocket->m_hSocket,pSend+iSent,18-iSent,0);
		if(iRet == -1)
			break;
		iSent += iRet;
	}while(iSent < 18);
}

void Recv_BanTrial(CNewConnection *pSocket, char* pBuf, int &index)
{
	char* pStrLog;
	char* tmp;
	char* strCMD = NULL;
	SYSTEMTIME pDate;

	char pStrID[22];
	memset(pStrID,0,22);

	if(frmServer->bBanTrial)
	{
		return;
	}

	int iLen = CMainSocket.GetBYTE(pBuf,index);
	if(iLen < 22)
	{
		CMainSocket.GetSTR(pStrID,pBuf,iLen,index);

		if( !strlen(pStrID) )
			return;

		StrDecrypt(pStrID);

		if( !strlen(pStrID) )
			return;
	}
	else
	{
		return;
    }

	strCMD = new char[64];
	memset(strCMD,0,64);

	strcpy(strCMD,"select * from trialbanlist where strAccountID like '");
	strcat(strCMD,pStrID);
	strcat(strCMD,"'");

	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	if(frmServer->SQLData->RecordCount == 0)
	{
		memset(strCMD,0,64);
		strcpy(strCMD,"insert into trialbanlist values ('");
		strcat(strCMD,pStrID);
		strcat(strCMD,"')");

		frmServer->SQLCon->Execute(strCMD);
	}

	if(frmServer->bLogKeys){

		GetLocalTime(&pDate);

		pStrLog = new char[128];
		memset(pStrLog,0x00,128);

		tmp = new char[32];
		memset(tmp,0,32);

		itoa(pDate.wDay,tmp,10);
		strcpy(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wMonth,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wYear,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," ");

		itoa(pDate.wHour,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMinute,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wSecond,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMilliseconds,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," --- ");

		strcat(pStrLog,"TRIAL_BANNED: [");

		strcat(pStrLog,pStrID);
		strcat(pStrLog,"] - ");
		strcat(pStrLog,pSocket->m_strIP);
		WriteLog("login_log.txt",pStrLog);

		delete[] tmp;
		delete[] pStrLog;
	}

	if(strCMD)
		delete[] strCMD;
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

void CNewConnection::UserProcess()
{

char tmp[16]; memset(tmp,0,16);
DWORD nRecv = 0,nRecvd = 0;
//EnterCriticalSection(&csProcess1);
char *pRecv = new char[iRecvSizeMax];	//packet size
//LeaveCriticalSection(&csProcess1);
memset(pRecv,0,iRecvSizeMax);

while(!nRecv){
	ioctlsocket(this->m_hSocket,FIONREAD,&nRecv);
	if(!nRecv && GetTickCount() - this->m_lTick >= 5000)
	{
		this->Close();
		delete [] pRecv;
		//CloseSelfHandle();
		delete this;
		return;
	}
}

if(nRecv > iRecvSizeMax || nRecv < iRecvSizeMin)
{
	this->Close();
	delete [] pRecv;
	//CloseSelfHandle();
	delete this;
	return;
}

nRecvd = 0;
int iRet;
do{
	iRet = recv(this->m_hSocket,pRecv+nRecvd,nRecv-nRecvd,0);
	if(iRet == -1)
	{
		this->Close();
		delete [] pRecv;
		//CloseSelfHandle();
		delete this;
		return;
	}
	nRecvd += iRet;
} while(nRecvd < nRecv);

if( !nRecvd )
{
	this->Close();
	delete [] pRecv;
	//CloseSelfHandle();
	delete this;
	return;
}

CMainSocket.Decrypt(pRecv,nRecv);

int iDataHead = CMainSocket.FindPacketHead(pRecv,nRecvd);
if(iDataHead < (int)sizeof(wPACKETHEAD)){
	this->Close();
	delete [] pRecv;
	//CloseSelfHandle();
	delete this;
	return;
}

int iDataTail = CMainSocket.FindPacketTail(pRecv+iDataHead,nRecvd-iDataHead);
if(iDataTail < 1){
	this->Close();
	delete [] pRecv;
	//CloseSelfHandle();
	delete this;
	return;
}

int iDataSize = iDataTail;
char* pData = pRecv + iDataHead;

int index(0);
BYTE bPrefix = CMainSocket.GetBYTE(pData,index);	//main prefix!!!

if(frmServer->bCritical)
	EnterCriticalSection(&csProcess1);

switch(bPrefix){

	case bRecv_LOGIN:
	Recv_Login(this,pData,index);
	iLoginCount++;
	itoa(iLoginCount,tmp,10);
	frmServer->lblogincount->Caption = tmp;
	break;

	case bRecv_CHECK:
	Recv_Check(this,pData,index);
	break;

	case bRecv_LOGOUT:
	Recv_Logout(this,pData,index);
	break;

	case bRecv_BANOTHER:
	Recv_BanOther(this,pData,index);
	break;

	case bRecv_REGISTER:
	Recv_Register(this,pData,index);
	break;

	case bRecv_ADD:
	Recv_Add(this,pData,index);
	break;

	case bRecv_REPORT:
	Recv_Report(this,pData,index);
	break;

	case bRecv_TRIAL:
	Recv_Trial(this,pData,index);
	break;

	case bRecv_BANTRIAL:
	Recv_BanTrial(this,pData,index);
	break;

	case bRecv_CHANGEPW:
	Recv_ChangePW(this,pData,index);
	break;

	case bRecv_TPT:
	Recv_TPT(this,pData,index);
	break;

	default:
	Send_Unknown(this);
	break;
}

if(frmServer->bCritical)
	LeaveCriticalSection(&csProcess1);

delete [] pRecv;
//CloseSelfHandle();
Sleep(1000);
this->Close();
delete this;
}

void UserProcess2(CNewConnection *pUser)
{
char tmp[16]; memset(tmp,0,16);
DWORD nRecv = 0,nRecvd = 0;
//EnterCriticalSection(&csProcess1);
char *pRecv = new char[iRecvSizeMax];	//packet size
//LeaveCriticalSection(&csProcess1);
memset(pRecv,0,iRecvSizeMax);

while(!nRecv){
	ioctlsocket(pUser->m_hSocket,FIONREAD,&nRecv);
	if(!nRecv && GetTickCount() - pUser->m_lTick >= 1000)
	{
		pUser->Close();
		delete [] pRecv;
		//CloseSelfHandle();
		delete pUser;
		return;
	}
}

if(nRecv > iRecvSizeMax || nRecv < iRecvSizeMin)
{
	pUser->Close();
	delete [] pRecv;
	//CloseSelfHandle();
	delete pUser;
	return;
}

nRecvd = 0;
int iRet;
do{
	iRet = recv(pUser->m_hSocket,pRecv+nRecvd,nRecv-nRecvd,0);
	if(iRet == -1)
	{
		pUser->Close();
		delete [] pRecv;
		//CloseSelfHandle();
		delete pUser;
		return;
	}
	nRecvd += iRet;
} while(nRecvd < nRecv);

if( !nRecvd )
{
	pUser->Close();
	delete [] pRecv;
	//CloseSelfHandle();
	delete pUser;
	return;
}

CMainSocket.Decrypt(pRecv,nRecv);

int iDataHead = CMainSocket.FindPacketHead(pRecv,nRecvd);
if(iDataHead < (int)sizeof(wPACKETHEAD)){
	pUser->Close();
	delete [] pRecv;
	//CloseSelfHandle();
	delete pUser;
	return;
}

int iDataTail = CMainSocket.FindPacketTail(pRecv+iDataHead,nRecvd-iDataHead);
if(iDataTail < 1){
	pUser->Close();
	delete [] pRecv;
	//CloseSelfHandle();
	delete pUser;
	return;
}

int iDataSize = iDataTail;
char* pData = pRecv + iDataHead;

int index(0);
BYTE bPrefix = CMainSocket.GetBYTE(pData,index);	//main prefix!!!

if(frmServer->bCritical)
	EnterCriticalSection(&csProcess1);

switch(bPrefix){

	case bRecv_LOGIN:
	Recv_Login(pUser,pData,index);
	iLoginCount++;
	itoa(iLoginCount,tmp,10);
	frmServer->lblogincount->Caption = tmp;
	break;

	case bRecv_CHECK:
	Recv_Check(pUser,pData,index);
	break;

	case bRecv_LOGOUT:
	Recv_Logout(pUser,pData,index);
	break;

	case bRecv_BANOTHER:
	Recv_BanOther(pUser,pData,index);
	break;

	case bRecv_REGISTER:
	Recv_Register(pUser,pData,index);
	break;

	case bRecv_ADD:
	Recv_Add(pUser,pData,index);
	break;

	case bRecv_REPORT:
	Recv_Report(pUser,pData,index);
	break;

	case bRecv_TRIAL:
	Recv_Trial(pUser,pData,index);
	break;

	case bRecv_BANTRIAL:
	Recv_BanTrial(pUser,pData,index);
	break;

	case bRecv_CHANGEPW:
	Recv_ChangePW(pUser,pData,index);
	break;

	case bRecv_TPT:
	Recv_TPT(pUser,pData,index);
	break;

	default:
	Send_Unknown(pUser);
	break;
}

if(frmServer->bCritical)
	LeaveCriticalSection(&csProcess1);

delete[] pRecv;
//CloseSelfHandle();
//Sleep(1000);
pUser->Close();
delete pUser;
}

void Recv_ChangePW(CNewConnection *pSocket,char* pBuf, int &index)
{
	int iResult;
	int iUserID;

	char* pStrLog;
	char* tmp = new char[32]; memset(tmp,0,32);
	char* strCMD = NULL;
	int iLen;
	char strKey[21];
	char strPW[21];
	char strNewPW[21];

	SYSTEMTIME pDate;
	long lCurrentTime;

	memset(strKey,0,21);
	memset(strPW,0,21);
	memset(strNewPW,0,21);

	BYTE	bKeyNo 		= 	CMainSocket.GetBYTE(pBuf,index);
	BYTE	bKeyLimit 	= 	CMainSocket.GetBYTE(pBuf,index);
	short	sRand1 		= 	CMainSocket.GetWORD(pBuf,index);
	short	sRand2 		= 	CMainSocket.GetWORD(pBuf,index);
	long	lKey 		= 	CMainSocket.GetDWORD(pBuf,index);
	int		iPWLen		=	CMainSocket.GetBYTE(pBuf,index);
	if(iPWLen <= 20)
		CMainSocket.GetSTR(strPW,pBuf,iPWLen,index);
	int		iNewPWLen		=	CMainSocket.GetBYTE(pBuf,index);
	if(iNewPWLen <= 20)
		CMainSocket.GetSTR(strNewPW,pBuf,iNewPWLen,index);

	ltoa(bKeyNo,tmp,16);
	iLen = strlen(tmp);
	if(iLen < 2){
		memmove(tmp+(2-iLen),tmp,iLen);
		memset(tmp,0x30,2-iLen);
	}
	strcat(strKey,tmp);

	ltoa(bKeyLimit,tmp,16);
	iLen = strlen(tmp);
	if(iLen < 2){
		memmove(tmp+(2-iLen),tmp,iLen);
		memset(tmp,0x30,2-iLen);
	}
	strcat(strKey,tmp);

	ltoa(sRand1,tmp,16);
	iLen = strlen(tmp);
	if(iLen < 4){
		memmove(tmp+(4-iLen),tmp,iLen);
		memset(tmp,0x30,4-iLen);
	}
	strcat(strKey,tmp);

	ltoa(sRand2,tmp,16);
	iLen = strlen(tmp);
	if(iLen < 4){
		memmove(tmp+(4-iLen),tmp,iLen);
		memset(tmp,0x30,4-iLen);
	}
	strcat(strKey,tmp);

	ltoa(lKey,tmp,16);
	iLen = strlen(tmp);
	if(iLen < 8){
		memmove(tmp+(8-iLen),tmp,iLen);
		memset(tmp,0x30,8-iLen);
	}
	strcat(strKey,tmp);

	if(frmServer->bChangePWPaused){
		iResult = bChange_PAUSED;
		goto RETN;
	}

	if(!iPWLen || iPWLen < 4 || iPWLen > 20){
		iResult = bChange_INVALID;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}

		goto RETN;
	}

	if(!iNewPWLen || iNewPWLen < 4 || iNewPWLen > 20){
		iResult = bChange_INVALID;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}

		goto RETN;
	}

	if( !strlen(strKey) || !strlen(strPW) || !strlen(strNewPW) )
	{
		iResult = bChange_INVALID;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}

		goto RETN;
	}

	if( !CheckIllegal(strKey) )
	{
		iResult = bChange_INVALID;
		goto RETN;
	}

	if( !CheckValidIDPW(strPW) || !CheckValidIDPW(strNewPW) )
	{
		iResult = bChange_INVALID;

		if(bBanIP)
		{
			AddBanIP( inet_addr(pSocket->m_strIP) );
		}

		goto RETN;
	}

	strCMD = new char[64];

	memset(strCMD,0,64);
	strcpy(strCMD,"select * from userlist where strKey like '");
	strcat(strCMD,strKey);
	strcat(strCMD,"' and strPW like '");
	strcat(strCMD,strPW);
	strcat(strCMD,"'");
	frmServer->SQLData->Close();
	frmServer->SQLData->CommandText = strCMD;
	frmServer->SQLData->Open();

	if(!frmServer->SQLData->RecordCount){
		iResult = bChange_INVALID;
		goto RETN;
	}

	iUserID = atoi(frmServer->SQLData->FieldByName("iUserID")->AsString.c_str());

	memset(strCMD,0,64);
	strcpy(strCMD,"update userlist set strPW = '");
	strcat(strCMD,strNewPW);
	strcat(strCMD,"' where iUserID = ");
	itoa(iUserID,tmp,10);
	strcat(strCMD,tmp);

	frmServer->SQLCon->Execute(strCMD);

	iResult = bChange_SUCCESS;

RETN:

	Send_ChangePW(pSocket,iResult);

	if(frmServer->bLogKeys){

		GetLocalTime(&pDate);

		pStrLog = new char[128];
		memset(pStrLog,0x00,128);
		memset(tmp,0,32);

		itoa(pDate.wDay,tmp,10);
		strcpy(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wMonth,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wYear,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," ");

		itoa(pDate.wHour,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMinute,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wSecond,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMilliseconds,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," --- ");

		switch(iResult){

			case bChange_INVALID:
			strcat(pStrLog,"ERROR_INVALID: [");
			break;

			case bChange_SUCCESS:
			strcat(pStrLog,"SUCCESS: [ID: ");
			itoa(iUserID,tmp,10);
			strcat(pStrLog,tmp);
			strcat(pStrLog,"] - [");
			break;

			case bChange_PAUSED:
			strcat(pStrLog,"ERROR_PAUSED: [ID: ");
			break;

			default:
			strcat(pStrLog,"ERROR_UNKNOWN: [");
			break;

		}
		strcat(pStrLog,strPW);
		strcat(pStrLog," - ");
		strcat(pStrLog,strNewPW);
		strcat(pStrLog," / ");
		strcat(pStrLog,strKey);

		strcat(pStrLog,"] - ");
		strcat(pStrLog,pSocket->m_strIP);
		WriteLog("change_log.txt",pStrLog);

		delete[] pStrLog;
		}

	delete[] tmp;
	if(strCMD)
		delete[] strCMD;
}
void __fastcall TfrmServer::chbanchangeClick(TObject *Sender)
{
	bChangePWPaused = chbanchange->Checked;
}
//---------------------------------------------------------------------------

void Send_ChangePW(CNewConnection *pSocket, int iResult){

	char pSend[6];
	memset(pSend,0,6);

	int index(0);
	CMainSocket.PutPacketHead(pSend,index);
	CMainSocket.PutBYTE(pSend,bSend_CHANGEPW,index);
	CMainSocket.PutBYTE(pSend,(BYTE)iResult,index);

	CMainSocket.PutPacketTail(pSend,index);

	CMainSocket.Encrypt(pSend,6);

	int iSent = 0;
	int iRet;
	do
	{
		iRet = send(pSocket->m_hSocket,pSend+iSent,6-iSent,0);
		if(iRet == -1)
			break;
		iSent += iRet;
	}while(iSent < 6);

}

void CloseSelfHandle()
{
	CloseHandle( GetCurrentThread() );
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

void Send_Unknown(CNewConnection *pSocket)
{
	char pSend[5]; memset(pSend,0,5);

	int index(0);
	CMainSocket.PutPacketHead(pSend,index);
	CMainSocket.PutBYTE(pSend,bSend_UNKNOWN,index);
	CMainSocket.PutPacketTail(pSend,index);

	CMainSocket.Encrypt(pSend,5);

	int iSent = 0;
	int iRet;
	do
	{
		iRet = send(pSocket->m_hSocket,pSend+iSent,5-iSent,0);
		if(iRet == -1)
			break;
		iSent += iRet;
	}while(iSent < 5);
}

void __fastcall TfrmServer::chcriticalClick(TObject *Sender)
{
	bCritical = chcritical->Checked;

	if(!bCritical)
		LeaveCriticalSection(&csProcess1);
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::chfreetestClick(TObject *Sender)
{
	bFreeTest = chfreetest->Checked;
}
//---------------------------------------------------------------------------


int	GetNewRandomKey()
{
	srand( GetTickCount() );
	int iRand = rand() % 0xFFFF + 1;

	return iRand;
}

void Recv_TPT(CNewConnection *pSocket,char* pBuf, int &index)
{
	int iUserID = CMainSocket.GetWORD(pBuf,index);
	if(iUserID == -1)
		return;

	char* pStrLog;
	char* tmp;
	SYSTEMTIME pDate;

	if(frmServer->bLogKeys){
		GetLocalTime(&pDate);

		pStrLog = new char[64];
		memset(pStrLog,0x00,64);

		tmp = new char[32];
		memset(tmp,0,32);

		itoa(pDate.wDay,tmp,10);
		strcpy(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wMonth,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,".");
		itoa(pDate.wYear,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," ");

		itoa(pDate.wHour,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMinute,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wSecond,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog,":");
		itoa(pDate.wMilliseconds,tmp,10);
		strcat(pStrLog,tmp);
		strcat(pStrLog," --- ");

		strcat(pStrLog,"TPT [ID: ");
		itoa(iUserID,tmp,10);
		strcat(pStrLog,tmp);

		strcat(pStrLog,"] - ");
		strcat(pStrLog,pSocket->m_strIP);
		WriteLog("tpt_log.txt",pStrLog);

		delete[] tmp;
		delete[] pStrLog;
	}

}

void SocketProcess()
{
	DWORD nRecv = 0;
	DWORD dwTick = 0;

	itSL it, itEnd;

	while(1)
	{
		if(lstSocket.empty() == true)
		{
			Sleep(10);
			continue;
		}

		dwTick = GetTickCount();

		EnterCriticalSection(&csProcess2);
			it 		= lstSocket.begin();
			itEnd 	= lstSocket.end();
		//LeaveCriticalSection(&csProcess2);

		for (; it != itEnd; it++)
		{
			if((*it)->m_bSuccess)
			{
				if(dwTick - (*it)->m_lSuccessTick >= 1000)
				{
					(*it)->Close();
					delete (*it);
					lstSocket.erase(it);
				}
			}
			else
			{
				ioctlsocket((*it)->m_hSocket,FIONREAD,&nRecv);

				if(nRecv)
				{
					UserProcess3((*it),nRecv);

					if((*it)->m_bSuccess == false)
					{
						(*it)->Close();
						delete (*it);
						lstSocket.erase(it);
					}

				}
				else
				{
					if(dwTick - (*it)->m_lTick >= 10000)
					{
						(*it)->Close();

						if(bBanIP)
						{
							AddBanIP( inet_addr( (*it)->m_strIP ) );
						}

						delete (*it);
						lstSocket.erase(it);
					}
				}
			}

			//Sleep(1);
		}
		LeaveCriticalSection(&csProcess2);
		Sleep(25);
	}
}

void UserProcess3(CNewConnection *pUser, int iRecvSize)
{
char tmp[16]; memset(tmp,0,16);
DWORD nRecvd = 0;
//EnterCriticalSection(&csProcess1);
char *pRecv = new char[iRecvSizeMax];	//packet size
//LeaveCriticalSection(&csProcess1);
memset(pRecv,0,iRecvSizeMax);


if(iRecvSize > iRecvSizeMax || iRecvSize < iRecvSizeMin)
{
	delete [] pRecv;
	//CloseSelfHandle();

	if(bBanIP)
	{
		AddBanIP( inet_addr(pUser->m_strIP) );
	}

	return;
}

nRecvd = 0;
int iRet;
do{
	iRet = recv(pUser->m_hSocket,pRecv+nRecvd,iRecvSize-nRecvd,0);
	if(iRet == -1)
	{
		delete [] pRecv;
		//CloseSelfHandle();
		return;
	}
	nRecvd += iRet;
} while((int)nRecvd < iRecvSize);

if( !nRecvd )
{
	delete [] pRecv;
	//CloseSelfHandle();
	return;
}

CMainSocket.Decrypt(pRecv,iRecvSize);

int iDataHead = CMainSocket.FindPacketHead(pRecv,nRecvd);
if( iDataHead == -1 )
{
	if(bBanIP)
	{
		AddBanIP( inet_addr(pUser->m_strIP) );
	}
	delete [] pRecv;
	return;
}

if(iDataHead < (int)sizeof(wPACKETHEAD)){
	delete [] pRecv;

	//CloseSelfHandle();
	return;
}

int iDataTail = CMainSocket.FindPacketTail(pRecv+iDataHead,nRecvd-iDataHead);
if( iDataTail == -1 )
{
	if(bBanIP)
	{
		AddBanIP( inet_addr(pUser->m_strIP) );
	}
	delete [] pRecv;
	return;
}

if(iDataTail < 1){
	delete [] pRecv;
	//CloseSelfHandle();
	return;
}

int iDataSize = iDataTail;
char* pData = pRecv + iDataHead;

int index(0);
BYTE bPrefix = CMainSocket.GetBYTE(pData,index);	//main prefix!!!

if(frmServer->bCritical)
	EnterCriticalSection(&csProcess1);

switch(bPrefix){

	case bRecv_LOGIN:
	Recv_Login(pUser,pData,index);
	iLoginCount++;
	itoa(iLoginCount,tmp,10);
	frmServer->lblogincount->Caption = tmp;
	break;

	case bRecv_CHECK:
	Recv_Check(pUser,pData,index);
	break;

	case bRecv_LOGOUT:
	Recv_Logout(pUser,pData,index);
	break;

	case bRecv_BANOTHER:
	Recv_BanOther(pUser,pData,index);
	break;

	case bRecv_REGISTER:
	Recv_Register(pUser,pData,index);
	break;

	case bRecv_ADD:
	Recv_Add(pUser,pData,index);
	break;

	case bRecv_REPORT:
	Recv_Report(pUser,pData,index);
	break;

	case bRecv_TRIAL:
	Recv_Trial(pUser,pData,index);
	break;

	case bRecv_BANTRIAL:
	Recv_BanTrial(pUser,pData,index);
	break;

	case bRecv_CHANGEPW:
	Recv_ChangePW(pUser,pData,index);
	break;

	case bRecv_TPT:
	Recv_TPT(pUser,pData,index);
	break;

	default:
	//Send_Unknown(pUser);
	if(bBanIP)
	{
		AddBanIP( inet_addr(pUser->m_strIP) );
	}
	else
	{
		Send_Unknown(pUser);
    }
	break;
}

if(frmServer->bCritical)
	LeaveCriticalSection(&csProcess1);

delete[] pRecv;
//CloseSelfHandle();
//Sleep(1000);
pUser->m_bSuccess = true;
pUser->m_lSuccessTick = GetTickCount();
//pUser->Close();
//delete pUser;
}
void __fastcall TfrmServer::opnum1Click(TObject *Sender)
{
	iTargetServerNo = 1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::opnum2Click(TObject *Sender)
{
	iTargetServerNo = 0;
}
//---------------------------------------------------------------------------

void SyncProcess()
{
	itSYL it, itEnd;

	struct linger pLinger;
	pLinger.l_onoff = 1;	//aktif
	pLinger.l_linger = 1;	//

	while(1)
	{
		if( !bSyncServer )
        {
			Sleep(1000);
			continue;
		}

		if( lstSync.empty() == true )
		{
			Sleep(50);
			continue;
		}

		CSyncSocket = _CWinsock.NewSocket();

		if(CSyncSocket.GetSocket() == INVALID_SOCKET)
		{
			Sleep(100);
			continue;
		}

		setsockopt(CSyncSocket.GetSocket(), SOL_SOCKET, SO_LINGER, (char*)&pLinger, sizeof(pLinger));

		if( !CSyncSocket.Client_Connect(strIPArray[iTargetServerNo],iServerPort) )
		{
			Sleep(250);
			continue;
		}

		EnterCriticalSection(&csProcess4);
		it 		= lstSync.begin();
		itEnd 	= lstSync.end();

		for (; it != itEnd; it++)
		{
			if( (*it)->m_iSize > 0 )
        	{
				CSyncSocket.Client_Send( (*it)->m_pData, (*it)->m_iSize );
				delete (*it);
				lstSync.erase(it);
            }
			else
			{
				delete (*it);
				lstSync.erase(it);
			}
		}

		LeaveCriticalSection(&csProcess4);
		CSyncSocket.Close();
		Sleep(50);
	}
}

void __fastcall TfrmServer::chsyncClick(TObject *Sender)
{
	bSyncServer = chsync->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnaddipClick(TObject *Sender)
{

	/*
	if(iIPCount == 10000)
	{
		MessageBox(0,"max limit!",0,0);
		return;
	}
	*/

	if(txtip->Text.Length() == 0)
		return;

	if(iIPIndex == 10000)
		iIPIndex = 0;

	char *IP = txtip->Text.c_str();

	lstip->AddItem(IP,NULL);

	lBannedIPList[iIPIndex] = inet_addr(IP);

	if(iIPCount < 10000)
		iIPCount++;

	iIPIndex++;

}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::chbanipClick(TObject *Sender)
{

	if( !bFirstBan )
	{
		bFirstBan = true;
		lBannedIPList = new long[10000];
	}

bBanIP = chbanip->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnipclearClick(TObject *Sender)
{
lstip->Clear();
iIPCount = 0;
iIPIndex = 0;
}
//---------------------------------------------------------------------------

void AddBanIP(long lIP)
{
	EnterCriticalSection(&csProcess5);

	for (int i = 0; i < iIPCount; i++)
	{
		if( lBannedIPList[i] == lIP )
		{
			LeaveCriticalSection(&csProcess5);
			return;
		}
	}

	if(iIPIndex == 10000)
		iIPIndex = 0;

	lBannedIPList[iIPIndex] = lIP;

	if(iIPCount < 10000)
		iIPCount++;

	iIPIndex++;

	LeaveCriticalSection(&csProcess5);
}

void __fastcall TfrmServer::Button1Click(TObject *Sender)
{
lstip->Clear();
iIPCount = 0;
iIPIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::Button2Click(TObject *Sender)
{
	char tmp[32]; memset(tmp,0,32);
	itoa(iIPCount,tmp,10);
	Label22->Caption = tmp;
}
//---------------------------------------------------------------------------

void __fastcall TfrmServer::btnsaveipClick(TObject *Sender)
{
	if(!iIPCount)
		return;

	if(!txtsaveip->Text.Length())
		return;

	char *IP = txtsaveip->Text.c_str();
	long lIP = inet_addr(IP);

	for (int i = 0; i < iIPCount; i++)
	{
		if( lBannedIPList[i] == lIP )
		{
			lBannedIPList[i] = 0;
			break;
		}
	}
}
//---------------------------------------------------------------------------

