#include "stdafx.h"
#include "netUDP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetUDP::CNetUDP()
{
	m_hWnd = NULL;
	m_bConnected = FALSE;
}

CNetUDP::~CNetUDP()
{

}

BOOL CNetUDP::CreateUDPReceiver(unsigned short port, HWND hWnd, int Msg)
{
	if(m_bConnected){
		AfxMessageBox(_T("Has Successfully Create the socket!\n"));
		return FALSE;
	}
	m_hWnd = hWnd;
	int ret;

    // 将字符串地址转换为套接字地址
	int iLen = 0;
	int cbRet = 0;
	int nIP_TTL = 16;//设置IP数据生存期

	iLen = sizeof(Receiveraddr);

	m_Socket = socket (AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if (m_Socket == INVALID_SOCKET){
		AfxMessageBox(_T("WSASocket:%d\n"),WSAGetLastError());
		return FALSE;
    } 

	Senderaddr.sin_family = AF_INET;
	Senderaddr.sin_port = htons(port);
	Senderaddr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(m_Socket,(SOCKADDR *)&Senderaddr,sizeof(Senderaddr));

	if(Msg != 0)
	{
		ret = WSAAsyncSelect(m_Socket, m_hWnd, Msg, //WM_AIR_MOVE_EVENT, 
		 FD_READ);

		if(ret == SOCKET_ERROR)
		{
			AfxMessageBox(_T("WSAAsyncSelect():%d\n"), WSAGetLastError());
			m_bConnected = FALSE;
			return FALSE;
		}
	}

	m_bConnected = TRUE;
	return TRUE;
}

BOOL CNetUDP::CreateUDPSender(unsigned short port,char* lpstrAddr)
{
	if(m_bConnected){
		AfxMessageBox(_T("Has Successfully Create the socket!\n"));
		return FALSE;
	}

    // 将字符串地址转换为套接字地址
	int iLen = 0;
	int cbRet = 0;
	int nIP_TTL = 16;//设置IP数据生存期

	m_Socket = socket (AF_INET,SOCK_DGRAM,NULL);
	if (m_Socket == INVALID_SOCKET){
		AfxMessageBox(_T("WSASocket:%d\n"),WSAGetLastError());
		return FALSE;
    } 

	Receiveraddr.sin_family = AF_INET;
	Receiveraddr.sin_port = htons(port);
	Receiveraddr.sin_addr.S_un.S_addr = inet_addr(lpstrAddr);

	bool bBroadcast = true;

	setsockopt(m_Socket,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));

	m_bConnected = TRUE;
	return TRUE;
}

int CNetUDP::Send(char *lpData, int size)
{
	if(m_Socket == INVALID_SOCKET)
		return -1;
	unsigned long nDataSent = 0;

	nDataSent = sendto(m_Socket,lpData,size,0,(SOCKADDR *)&Receiveraddr,sizeof(Receiveraddr));
	
	return nDataSent;
}

int CNetUDP::Receive(char *lpData, int size)
{
	WSABUF wsaRecvBuf;
	wsaRecvBuf.buf = lpData;
	wsaRecvBuf.len = size;
	unsigned long cbRet = 0;
	int SenderAddrsize = sizeof(Senderaddr);


	cbRet = recvfrom(m_Socket,lpData,size,0,(SOCKADDR *)&Senderaddr,&SenderAddrsize);

	//通知父窗口读数据
	return cbRet;
}

void CNetUDP::Initilize()
{
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 2);
	int ret = WSAStartup(version, &wsaData);
	if(ret != 0)
		TRACE("Initilize Error!\n");
}

void CNetUDP::Unintilize()
{
	if (WSACleanup() != 0){
		TRACE("UnInitilize Error:%d\n", WSAGetLastError());
	}
}

BOOL CNetUDP::IsConnected()
{
	return m_bConnected;
}

void CNetUDP::Close()
{
	closesocket(m_Socket);
	m_bConnected = FALSE;
}
