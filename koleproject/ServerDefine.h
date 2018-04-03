const int iServerCheckInterval 	= 	160;
const int iPutKeyInterval 		=  	166;

const BYTE bRecv_LOGIN 		= 0x01;	// 0x01
const BYTE bRecv_CHECK 		= 0x02;
const BYTE bRecv_REGISTER 	= 0x05;
const BYTE bRecv_ADD		= 0x06;
const BYTE bRecv_TRIAL		= 0x08;
const BYTE bRecv_CHANGEPW	= 0x0A;
const BYTE bRecv_UNKNOWN	= 0xFF;

const BYTE bCheck_NOLOGIN 	= 	0x00;
const BYTE bCheck_SUCCESS 	= 	0x01;
const BYTE bCheck_PAUSED 	= 	0x02;
const BYTE bCheck_TIMEOUT 	= 	0xFE;
const BYTE bCheck_WRONGIP 	= 	0xFF;

const BYTE bLogin_INVALID 	= 	0x00;
const BYTE bLogin_SUCCESS 	= 	0x01;
const BYTE bLogin_TRIALERR 	= 	0x02;
const BYTE bLogin_INITERR	=	0xFA;
const BYTE bLogin_INVALIDV	= 	0xFB;
const BYTE bLogin_INUSE 	= 	0xFC;
const BYTE bLogin_BANNED 	=  	0xFD;
const BYTE bLogin_TRYLATER 	= 	0xFE;
const BYTE bLogin_TIMEOUT 	= 	0xFF;

const BYTE bSend_LOGIN		=	0x01;	// 0x01
const BYTE bSend_CHECK		=	0x02;
const BYTE bSend_LOGOUT 	= 	0x03;
const BYTE bSend_BANOTHER 	= 	0x04;
const BYTE bSend_REGISTER 	= 	0x05;
const BYTE bSend_ADD	 	= 	0x06;
const BYTE bSend_REPORT	 	= 	0x07;
const BYTE bSend_TRIAL	 	= 	0x08;
const BYTE bSend_BANTRIAL	= 	0x09;
const BYTE bSend_CHANGEPW	= 	0x0A;
const BYTE bSend_TPT		= 	0x0B;

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

const int iKeySize = 10;

const int iCheckPacketSize = 9;
const int iLogoutPacketSize = 11;
const int iBanOtherPacketSize = 11;

const int iRecvSizeCheck = 13;
const int iRecvSizeLogin = 29;
const int iRecvSizeRegister = 7;
const int iRecvSizeAdd = 7;
const int iRecvSizeChange = 6;

const int iServerPort = 3113;
