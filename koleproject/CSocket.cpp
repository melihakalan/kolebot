//---------------------------------------------------------------------------


#pragma hdrstop

#include "CSocket.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CWinsock _CWinsock;

DWORD dwSendCryptKey = 0;
DWORD dwRecvCryptKey = 0;
// buraya bisey ekleme!!!
int		iTotalCryption		=	0;	//+4
int		iTotalConnection	=	0;	//+8
int		iTotalDecryption	=	0;	//+12

SOCKET CWinsock::NewSocket()
{
	if(!m_bWSState)	return INVALID_SOCKET;

	SOCKET sRet = socket(AF_INET, SOCK_STREAM,0);

	if(sRet != INVALID_SOCKET)
	{
		struct linger pLinger;
		pLinger.l_onoff = 1;	//aktif
		pLinger.l_linger = 1;	//3 saniye gecikme yeterli
		setsockopt(sRet, SOL_SOCKET, SO_LINGER, (char*)&pLinger, sizeof(pLinger));
	}

	return sRet;
}

bool CSocket::Client_Connect(char* strIP, int iPort)
{
m_SocketInfo.sin_family = AF_INET;
m_SocketInfo.sin_port = htons(iPort);

struct hostent *sockIP;
sockIP = gethostbyname(strIP);

m_SocketInfo.sin_addr = *((struct in_addr *)sockIP->h_addr);
memset(&(m_SocketInfo.sin_zero), 0,0) ;

bool bRet = (bool)!connect(m_hSocket, (struct sockaddr *)&m_SocketInfo,sizeof(struct sockaddr) );
DWORD dwPtr = (DWORD)&dwRecvCryptKey;	// +8
dwPtr += 8;
*(DWORD*)dwPtr += (int)bRet;
return bRet;
}

bool CSocket::Server_Bind(int iPort)
{
m_SocketInfo.sin_family = AF_INET;
m_SocketInfo.sin_port = htons(iPort);
m_SocketInfo.sin_addr.s_addr = htons(INADDR_ANY);
memset(&(m_SocketInfo.sin_zero), 0,0) ;

return (bool)!bind(m_hSocket,(sockaddr*)&m_SocketInfo , sizeof(sockaddr_in) );
}

bool CSocket::Client_Send(char* pBuf,int iSize)
{
if(!pBuf || !iSize || iSize >= 256)
	return false;

dwSendCryptKey = iSize;

this->Encrypt(pBuf,iSize);

//int StartTick = GetTickCount();

int nSent(0);
do
{
nSent += send(m_hSocket,pBuf+nSent,iSize-nSent,0);
if(nSent == -1)
	return false;
//if(!nSent)
//	break;
//if(GetTickCount() - StartTick >= 1000)
//	break;
} while(nSent < iSize);
return (bool)nSent;
}

bool CSocket::Server_Send(char* pBuf,int iSize)
{
if(!pBuf)
	return false;

//int StartTick = GetTickCount();

int nSent(0);
do
{
nSent += send(m_hAcceptedSocket,pBuf+nSent,iSize-nSent,0);
if(nSent == -1)
	return false;
//if(!nSent)
//	break;
//if(GetTickCount() - StartTick >= 1000)
//	break;
} while(nSent < iSize);
return (bool)nSent;
}

int CSocket::Client_Recv(char* pBuf,int iSize)
{
if(!pBuf)
	return false;

int iRet = recv(m_hSocket,pBuf,iSize,0);
if(iRet == iSize)
{
	dwRecvCryptKey = iSize;
	this->Decrypt(pBuf,iSize);
}
return iRet;
}

int CSocket::Server_Recv(char* pBuf,int iSize)
{
if(!pBuf)
	return false;

return recv(m_hAcceptedSocket,pBuf,iSize,0);
}

int CSocket::FindPacketHead(char* pBuf, int MaxLen)
{
	int i(0);
	while(i < MaxLen)
	{
		if(*(WORD*)(pBuf+i) == wPACKETHEAD)
			return i + sizeof(wPACKETHEAD);
		i++;
	}
	return -1;
}

int CSocket::FindPacketTail(char* pBuf, int MaxLen)
{
	int i(0);
	while(i < MaxLen)
	{
		if(*(WORD*)(pBuf+i) == wPACKETTAIL)
			return i;
		i++;
	}
	return -1;
}

void CSocket::Encrypt(char* pBuf, int iSize)
{
	if(!pBuf || !iSize || iSize >= 256)
		return;

	DWORD bKey = dwSendCryptKey;

	DWORD bVal;
	DWORD bNew;

	for (int i = 0; i < iSize; i++)
	{
		bVal = (DWORD)*(BYTE*)(pBuf + i);
		__asm
		{
			PUSH bVal
			POP bNew
			PUSH bKey
			POP EAX
			XOR bNew,EAX
			NOT bNew
			AND bNew,0xFF
		}
		*(BYTE*)(pBuf + i) = (BYTE)bNew;
	}
	DWORD dwPtr = (DWORD)&dwRecvCryptKey;	// +4
	dwPtr += 4;
	*(DWORD*)dwPtr += iSize;
}

void CSocket::Decrypt(char* pBuf, int iSize)
{
	if(!pBuf || !iSize || iSize >= 256)
		return;

	DWORD bKey = dwRecvCryptKey;

	DWORD bVal;
	DWORD bNew;

	for (int i = 0; i < iSize; i++)
	{
		bVal = (DWORD)*(BYTE*)(pBuf + i);
		__asm
		{
			PUSH bVal
			POP bNew
			NOT bNew
			AND bNew,0xFF
			PUSH bKey
			POP EAX
			XOR bNew,EAX
		}
		*(BYTE*)(pBuf + i) = (BYTE)bNew;
	}
	DWORD dwPtr = (DWORD)&dwRecvCryptKey;	// +4
	dwPtr += 12;
	*(DWORD*)dwPtr += iSize;
}





