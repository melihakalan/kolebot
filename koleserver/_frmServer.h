//---------------------------------------------------------------------------

#ifndef _frmServerH
#define _frmServerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <stdio.h>
#include <ExtCtrls.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>

#include "CSocket.h"
#include <list>

void AddBanIP(long lIP);

extern long* lBannedIPList;
extern bool bBanIP;
extern bool bFirstBan;
extern int iIPCount;
extern int iIPIndex;

void StrDecrypt(char* pStr);

extern char* strIPArray[2];
extern int iTargetServerNo;
extern CSocket CSyncSocket;

extern bool bSyncServer;

const short iServerVersion = 1105	;

const int iMaxUserInterval = 1760;	//

const int iServerPort = 3113;
const int iMaxListen = 500;

const int iKeySize = 10;
const int iRecvSizeMax = 256;
const int iRecvSizeMin = 9;
const BYTE bSendByte = 0x31; //skill packet prefix.

const BYTE bRecv_LOGIN 		= 0x01;	//unutma 0x01 xd
const BYTE bRecv_CHECK 		= 0x02;
const BYTE bRecv_LOGOUT    	= 0x03;
const BYTE bRecv_BANOTHER 	= 0x04;
const BYTE bRecv_REGISTER 	= 0x05;
const BYTE bRecv_ADD		= 0x06;
const BYTE bRecv_REPORT	 	= 0x07;
const BYTE bRecv_TRIAL	 	= 0x08;
const BYTE bRecv_BANTRIAL	= 0x09;
const BYTE bRecv_CHANGEPW	= 0x0A;
const BYTE bRecv_TPT		= 0x0B;

const BYTE bCheck_NOLOGIN 	= 	0x00;
const BYTE bCheck_SUCCESS 	= 	0x01;
const BYTE bCheck_PAUSED 	= 	0x02;
const BYTE bCheck_TIMEOUT 	= 	0xFE;
const BYTE bCheck_WRONGIP 	= 	0xFF;

const BYTE bLogin_INVALID 	= 	0x00;
const BYTE bLogin_SUCCESS 	= 	0x01;
const BYTE bLogin_TRIALERR 	= 	0x02;
const BYTE bLogin_INITERR	=	0xFA;
const BYTE bLogin_INVALIDV	=	0xFB;
const BYTE bLogin_INUSE 	= 	0xFC;
const BYTE bLogin_BANNED 	=  	0xFD;
const BYTE bLogin_TRYLATER 	= 	0xFE;
const BYTE bLogin_TIMEOUT 	= 	0xFF;

const BYTE bSend_LOGIN		=	0x01;	// 0x01
const BYTE bSend_CHECK		=	0x02;
const BYTE bSend_REGISTER 	= 	0x05;
const BYTE bSend_ADD	 	= 	0x06;
const BYTE bSend_TRIAL	 	= 	0x08;
const BYTE bSend_CHANGEPW	= 	0x0A;
const BYTE bSend_UNKNOWN  	=	0xFF;

const BYTE bTrial_INVALID	=	0x00;
const BYTE bTrial_SUCCESS	=	0x01;
const BYTE bTrial_IDUSED	=	0x02;
const BYTE bTrial_BANNED	=	0xFD;
const BYTE bTrial_INVALIDV	=	0xFE;
const BYTE bTrial_DISABLED	=	0xFF;

const BYTE bRegister_INUSE		=	0x00;
const BYTE bRegister_SUCCESS	=	0x01;
const BYTE bRegister_PAUSED		=	0x02;
const BYTE bRegister_BANNED		=	0xFC;
const BYTE bRegister_KEYUSED	=	0xFD;
const BYTE bRegister_IDPWERROR	=	0xFE;
const BYTE bRegister_INVALID	=	0xFF;

const BYTE bAdd_IDPWINVALID		=	0x00;
const BYTE bAdd_SUCCESS			=	0x01;
const BYTE bAdd_PAUSED			=	0x02;
const BYTE bAdd_NOTIMEOUT		=	0xFB;
const BYTE bAdd_IDBANNED		=	0xFC;
const BYTE bAdd_KEYBANNED		=	0xFD;
const BYTE bAdd_KEYUSED			=	0xFE;
const BYTE bAdd_KEYINVALID		=	0xFF;

const BYTE bChange_INVALID		=	0x00;
const BYTE bChange_SUCCESS		=	0x01;
const BYTE bChange_PAUSED		=	0xFF;

void ServerFunction();
void ServerFunction2();
void ServerFunction3();
void WriteLog(char* strFileName, char* strLog);

class CNewConnection;

bool CheckValidIDPW(char* pStr);
bool CheckIllegal(char* pStr);

void Recv_Login(CNewConnection *pSocket, char* pBuf, int &index);
void Recv_Check(CNewConnection *pSocket,char* pBuf, int &index);
void Recv_Logout(CNewConnection *pSocket,char* pBuf, int &index);
void Recv_BanOther(CNewConnection *pSocket,char* pBuf, int &index);
void Recv_Register(CNewConnection *pSocket,char* pBuf, int &index);
void Recv_Add(CNewConnection *pSocket,char* pBuf, int &index);
void Recv_Report(CNewConnection *pSocket,char* pBuf, int &index);
void Recv_Trial(CNewConnection *pSocket,char* pBuf, int &index);
void Recv_BanTrial(CNewConnection *pSocket,char* pBuf, int &index);
void Recv_ChangePW(CNewConnection *pSocket,char* pBuf, int &index);
void Recv_TPT(CNewConnection *pSocket,char* pBuf, int &index);

void Send_Login(CNewConnection *pSocket,int iResult, int iRemainingDays, int iUserIndex, int iUserID, int iSourceID, long lSockHandle,BYTE bSendFixAdd,int iLogOutKey,DWORD dwHDNo,DWORD dwARG);
void Send_Check(CNewConnection *pSocket,int iResult, int iSourceID, long lSockHandle);
void Send_Register(CNewConnection *pSocket,int iResult,int iDays);
void Send_Add(CNewConnection *pSocket,int iResult,int iDays);
void Send_Trial(CNewConnection *pSocket,int iResult, int iRemainingDays, int iUserIndex, int iUserID, int iSourceID, long lSockHandle);
void Send_ChangePW(CNewConnection *pSocket,int iResult);
void Send_Unknown(CNewConnection *pSocket);

void LoadDateList();
void LoadIDList();
void LoadNumList();
void ClearOnlineList();

void CloseSelfHandle();

extern CSocket CMainSocket;
extern SOCKET  hAcceptedSocket;
extern long AcceptedTick;
extern DWORD dwThread;
extern HANDLE hThread;
extern long lConnectionCount;

extern	int iLoginCount;
extern	int iTodayRegisterCount;
extern	int iTodayAddCount;
extern	int iRegisterCount;
extern	int iAddCount;
extern 	int	iThreadCount;

extern CRITICAL_SECTION csProcess1;
extern CRITICAL_SECTION csProcess2;
extern CRITICAL_SECTION csProcess3;
extern CRITICAL_SECTION csProcess4;
extern CRITICAL_SECTION csProcess5;

class CUser
{
public:
	//in_addr		m_inIP;
	int			m_iCheckInterval;
	int			m_iUserID;
	int			m_iUserIndex;
	bool		m_bFreeIP;
	int			m_sSourceID;
	long		m_lSockHandle;
	long		m_lHDNo;
	int			m_sLogOutKey;
	DWORD		m_dwARG;
	int			m_iLastReportTick;
	char		m_strIP[16];

	CUser(int iUserID, int iUserIndex, bool bFreeIP, int sSourceID, long lSockHandle, long lHDNo,int sLogOutKey,DWORD dwARG)
	{
		//m_inIP = in_IP;
		m_iCheckInterval = 0;
		m_iUserID = iUserID;
		m_iUserIndex = iUserIndex;
		m_bFreeIP = bFreeIP;
		m_sSourceID = sSourceID;
		m_lSockHandle = lSockHandle;
		m_lHDNo = lHDNo;
		m_sLogOutKey = sLogOutKey;
		m_dwARG = dwARG;
		m_iLastReportTick = 0;
		memset(m_strIP,0,16);
		//strcpy(m_strIP,inet_ntoa(m_inIP));
	}

	CUser(){
		//m_inIP.s_addr = 0;
		m_iCheckInterval = 0;
		m_iUserID = 0;
		m_iUserIndex = -1;
		m_bFreeIP = false;
		m_sSourceID = -1;
		m_lSockHandle = -1;
		m_lHDNo = 0;
		m_sLogOutKey = 0;
		m_dwARG = 0;
		m_iLastReportTick = 0;
		memset(m_strIP,0,16);
	}
};

class CNewConnection
{
	public:
	SOCKET					m_hSocket;
	//struct sockaddr_in 		m_SocketInfo;
	long					m_lTick;
	bool					m_bSuccess;
	long					m_lSuccessTick;
	char					m_strIP[16];

	static void StartProcess(void* param){((CNewConnection*)param)->UserProcess();}
	void UserProcess();
	void Close()
	{
		closesocket(m_hSocket);
		m_hSocket = NULL;
	}

	CNewConnection(SOCKET hSocket, long lTick)
	{
		m_hSocket = hSocket;
		//m_SocketInfo = SocketInfo;
		m_lTick = lTick;
		m_bSuccess = false;
		m_lSuccessTick = 0;
		memset(m_strIP,0,16);
	}

	CNewConnection()
	{
		m_hSocket = 0;
		m_lTick = 0;
		m_bSuccess = false;
		m_lSuccessTick = 0;
		memset(m_strIP,0,16);
	}


};

class CSyncData
{
	public:
	char* m_pData;
	int m_iSize;

	CSyncData()
	{
		m_pData = NULL;
		m_iSize = 0;
	}

	CSyncData(char* pData, int iSize)
	{
		m_pData = pData;
		m_iSize = iSize;
	}

	~CSyncData()
	{
		delete [] m_pData;
		m_iSize = 0;
    }
};

	extern std::list<CUser*> 						lstUserList;
	typedef std::list<CUser*>::iterator				itUL;
	//std::list<CUser*>::iterator				itUser;

	extern std::list<CNewConnection*>				lstSocket;
	typedef std::list<CNewConnection*>::iterator   	itSL;
	//std::list<CNewConnection*>::iterator   	itSocket;

	extern std::list<CSyncData*>					lstSync;
	typedef std::list<CSyncData*>::iterator   		itSYL;

	int GetNewUserIndex();
	int	GetNewRandomKey();
	bool SortUserList(CUser* User1, CUser* User2);

	void UserProcess2(CNewConnection *pUser);
	void UserProcess3(CNewConnection *pUser, int iRecvSize);
	void SocketProcess();
	void SyncProcess();

//---------------------------------------------------------------------------
class TfrmServer : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *grpMain;
	TCheckBox *chpause;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *lbcount;
	TCheckBox *chlogkeys;
	TGroupBox *grpbandate;
	TListBox *lstdateban;
	TLabel *Label3;
	TEdit *txtdateban;
	TButton *btnadddateban;
	TButton *btnloaddateban;
	TButton *btnsavedateban;
	TButton *btncleardateban;
	TCheckBox *chdateban;
	TGroupBox *grpbantime;
	TLabel *Label4;
	TListBox *lstidban;
	TButton *btnloadidban;
	TButton *btnsaveidban;
	TButton *btnclearidban;
	TCheckBox *chidban;
	TEdit *txtidban;
	TButton *btnaddidban;
	TGroupBox *grpbanno;
	TLabel *Label5;
	TListBox *lstnoban;
	TButton *btnloadnoban;
	TButton *btnsavenoban;
	TButton *btnclearnoban;
	TCheckBox *chnoban;
	TEdit *txtnoban;
	TButton *btnaddnoban;
	TLabel *Label6;
	TLabel *lbcurrentcount;
	TTimer *tmusercheck;
	TCheckBox *chfree;
	TCheckBox *chfreeip;
	TADOConnection *SQLCon;
	TADODataSet *SQLData;
	TLabel *Label7;
	TLabel *lblogincount;
	TGroupBox *grpidoperations;
	TLabel *Label8;
	TEdit *txtidno;
	TButton *btngetidinfo;
	TLabel *Label9;
	TEdit *txtidid;
	TEdit *txtidpw;
	TLabel *Label10;
	TEdit *txtiddays;
	TLabel *Label11;
	TEdit *txtiddate;
	TLabel *Label12;
	TEdit *txtidkey;
	TLabel *Label13;
	TEdit *txtidextra;
	TEdit *txtidkeyno;
	TLabel *Label14;
	TCheckBox *chidbanned;
	TButton *btnsaveid;
	TGroupBox *grpfindno;
	TListBox *lstfindno;
	TEdit *txtfindno;
	TButton *btnfindno;
	TButton *btnmovefilter;
	TButton *btnbanall;
	TGroupBox *grpfindid;
	TEdit *txtfindid;
	TLabel *Label15;
	TButton *btnfindid;
	TEdit *txtfoundid;
	TListBox *lstfoundids;
	TEdit *txtfinddate;
	TButton *btnlistdate;
	TButton *btnlistban;
	TButton *btnbanallfound;
	TButton *btnclearonlines;
	TCheckBox *chaddpause;
	TCheckBox *chregisterpause;
	TCheckBox *chcheckpause;
	TCheckBox *chlogoutpause;
	TCheckBox *chbanpause;
	TLabel *Label16;
	TLabel *lbcurrentregistercount;
	TLabel *Label18;
	TLabel *lbcurrentaddcount;
	TLabel *Label17;
	TLabel *lbregistercount;
	TLabel *Label20;
	TLabel *lbaddcount;
	TCheckBox *chidfreeip;
	TButton *btnlistfreeip;
	TButton *btnrefreshcounts;
	TGroupBox *grponline;
	TLabel *Label19;
	TEdit *txtonlineid;
	TButton *btnfindonline;
	TLabel *Label21;
	TLabel *lbonlineindex;
	TButton *btndconline;
	TCheckBox *chbantrial;
	TCheckBox *chbanchange;
	TCheckBox *chcritical;
	TCheckBox *chfreetest;
	TLabel *lbthread;
	TRadioButton *opnum1;
	TRadioButton *opnum2;
	TCheckBox *chsync;
	TListBox *lstip;
	TEdit *txtip;
	TButton *btnaddip;
	TButton *btnipclear;
	TCheckBox *chbanip;
	TButton *Button1;
	TButton *Button2;
	TLabel *Label22;
	TButton *btnsaveip;
	TEdit *txtsaveip;
	void __fastcall chpauseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall chlogkeysClick(TObject *Sender);
	void __fastcall btncleardatebanClick(TObject *Sender);
	void __fastcall btnclearidbanClick(TObject *Sender);
	void __fastcall btnadddatebanClick(TObject *Sender);
	void __fastcall btnaddidbanClick(TObject *Sender);
	void __fastcall chdatebanClick(TObject *Sender);
	void __fastcall chidbanClick(TObject *Sender);
	void __fastcall btnsavedatebanClick(TObject *Sender);
	void __fastcall btnsaveidbanClick(TObject *Sender);
	void __fastcall btnloaddatebanClick(TObject *Sender);
	void __fastcall btnloadidbanClick(TObject *Sender);
	void __fastcall btnclearnobanClick(TObject *Sender);
	void __fastcall btnaddnobanClick(TObject *Sender);
	void __fastcall chnobanClick(TObject *Sender);
	void __fastcall btnsavenobanClick(TObject *Sender);
	void __fastcall tmusercheckTimer(TObject *Sender);
	void __fastcall chfreeClick(TObject *Sender);
	void __fastcall chfreeipClick(TObject *Sender);
	void __fastcall btnfindnoClick(TObject *Sender);
	void __fastcall btnmovefilterClick(TObject *Sender);
	void __fastcall btnbanallClick(TObject *Sender);
	void __fastcall btnfindidClick(TObject *Sender);
	void __fastcall btnlistdateClick(TObject *Sender);
	void __fastcall btnlistbanClick(TObject *Sender);
	void __fastcall btnbanallfoundClick(TObject *Sender);
	void __fastcall btngetidinfoClick(TObject *Sender);
	void __fastcall btnsaveidClick(TObject *Sender);
	void __fastcall btnclearonlinesClick(TObject *Sender);
	void __fastcall chregisterpauseClick(TObject *Sender);
	void __fastcall chcheckpauseClick(TObject *Sender);
	void __fastcall chaddpauseClick(TObject *Sender);
	void __fastcall chlogoutpauseClick(TObject *Sender);
	void __fastcall chbanpauseClick(TObject *Sender);
	void __fastcall btnlistfreeipClick(TObject *Sender);
	void __fastcall btnrefreshcountsClick(TObject *Sender);
	void __fastcall btnfindonlineClick(TObject *Sender);
	void __fastcall btndconlineClick(TObject *Sender);
	void __fastcall chbantrialClick(TObject *Sender);
	void __fastcall chbanchangeClick(TObject *Sender);
	void __fastcall chcriticalClick(TObject *Sender);
	void __fastcall chfreetestClick(TObject *Sender);
	void __fastcall opnum1Click(TObject *Sender);
	void __fastcall opnum2Click(TObject *Sender);
	void __fastcall chsyncClick(TObject *Sender);
	void __fastcall btnaddipClick(TObject *Sender);
	void __fastcall chbanipClick(TObject *Sender);
	void __fastcall btnipclearClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall btnsaveipClick(TObject *Sender);
private:	// User declarations
public:		// User declarations

	bool bTryLater;
	bool bRegisterPaused;
	bool bCheckPaused;
	bool bAddPaused;
	bool bLogoutPaused;
	bool bBanPaused;
	bool bBanTrial;
	bool bChangePWPaused;

	bool bLogKeys;
	bool bBanDate;
	bool bBanID;
	bool bBanNum;
	bool bFree;
	bool bFreeIP;

	bool bFreeTest;

	bool bCritical;
	__fastcall TfrmServer(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmServer *frmServer;
//---------------------------------------------------------------------------
#endif
