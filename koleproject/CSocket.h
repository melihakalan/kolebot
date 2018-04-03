//---------------------------------------------------------------------------

#ifndef CSocketH
#define CSocketH

#include "winsock.h"

const WORD wPACKETHEAD	=	0x6F6B;	//ko
const WORD wPACKETTAIL	=	0x6A64;	//jd

extern DWORD dwSendCryptKey;
extern DWORD dwRecvCryptKey;
// buraya bisey ekleme!!!
extern int		iTotalCryption;	//+4
extern int		iTotalConnection;	//+8
extern int		iTotalDecryption;	//+12

class CWinsock
{
private:
	WSADATA m_WSData;
	bool	m_bWSState;
public:

	inline bool	GetState()
	{
		return m_bWSState;
	}

	inline bool	StartWinsock()
	{
		m_bWSState = (bool)!WSAStartup(MAKEWORD(2,0),&m_WSData);
		return m_bWSState;
	}

	SOCKET	NewSocket();

	inline void	CleanWinsock()
	{
		WSACleanup();
		m_bWSState = false;
    }

	CWinsock(){m_bWSState = false;};
};

class CSocket
{
private:
	SOCKET					m_hSocket;
	SOCKET					m_hAcceptedSocket;
	struct sockaddr_in 		m_SocketInfo;
public:
	inline void	SetSocket(SOCKET hSocket){m_hSocket = hSocket;};
	inline void	SetAcceptedSocket(SOCKET hSocket){m_hAcceptedSocket = hSocket;};
	inline SOCKET	GetSocket(){return m_hSocket;};
	inline SOCKET	GetAcceptedSocket(){return m_hAcceptedSocket;};
	inline struct sockaddr_in GetSockInfo(){return m_SocketInfo;};

	bool	Client_Connect(char* strIP, int iPort);
	bool	Server_Bind(int iPort);

	inline bool	Server_Listen(int iMaxListen)
	{
		if(listen(m_hSocket,iMaxListen) == SOCKET_ERROR)
			return false;
		return true;
	}

	inline SOCKET	Server_Accept()
	{
		int iInfoSize = sizeof(m_SocketInfo);
		return accept(m_hSocket,(sockaddr*)&m_SocketInfo,&iInfoSize);
	}

	bool	Client_Send(char* pBuf,int iSize);
	bool	Server_Send(char* pBuf,int iSize);
	int		Client_Recv(char* pBuf,int iSize);
	int		Server_Recv(char* pBuf,int iSize);

	void	Encrypt(char* pBuf, int iSize);
	void	Decrypt(char* pBuf, int iSize);

	int		FindPacketHead(char* pBuf, int MaxLen);
	int		FindPacketTail(char* pBuf, int MaxLen);

	inline void	PutPacketHead(char* pBuf, int& Index){*(WORD*)(pBuf+Index)=wPACKETHEAD; Index+=2;};
	inline void PutPacketTail(char* pBuf, int& Index){*(WORD*)(pBuf+Index)=wPACKETTAIL; Index+=2;};

	inline void PutBYTE(char* pBuf,BYTE bVal,int& Index){*(pBuf+Index) = bVal; Index++;};
	inline void PutWORD(char* pBuf,WORD wVal,int& Index){*(WORD*)(pBuf+Index) = wVal; Index+=2;};
	inline void PutDWORD(char* pBuf,DWORD dwVal,int& Index){*(DWORD*)(pBuf+Index) = dwVal; Index+=4;};
	inline void PutSTR(char* pBuf,char* pStr,int iSize,int& Index){memcpy(pBuf+Index,pStr,iSize); Index+=iSize;};

	inline BYTE GetBYTE(char* pBuf,int& Index){Index++; return *(BYTE*)(pBuf+Index-1);};
	inline WORD GetWORD(char* pBuf,int& Index){Index+=2; return *(WORD*)(pBuf+Index-2);};
	inline DWORD GetDWORD(char* pBuf,int& Index){Index+=4; return *(DWORD*)(pBuf+Index-4);};
	inline void GetSTR(char* dBuf,char* sBuf,int iSize,int& Index){memcpy(dBuf,sBuf+Index,iSize); Index+=iSize;};

	inline void	Close()
	{
		closesocket(m_hSocket);
		m_hSocket = 0;
	}

	inline void	CloseAccepted()
	{
		closesocket(m_hAcceptedSocket);
		m_hAcceptedSocket = 0;
    }

	CSocket(SOCKET hSocket)
	{
		m_hSocket = hSocket;
		m_hAcceptedSocket = 0;
	};

	CSocket()
	{
		m_hSocket = 0;
		m_hAcceptedSocket = 0;
	};
};

extern CWinsock _CWinsock;

#endif
