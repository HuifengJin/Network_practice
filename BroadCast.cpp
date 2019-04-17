#include "stdafx.h"
#include "BroadCast.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBroadCast::CBroadCast()
{
	m_hWnd = NULL;
	m_bConnected = FALSE;
}

CBroadCast::~CBroadCast()
{

}

BOOL CBroadCast::CreateBroadCastReceiver(unsigned short port, HWND hWnd, int Msg)
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

	m_Socket = socket (AF_INET,SOCK_DGRAM,0);
	if (m_Socket == INVALID_SOCKET){
		AfxMessageBox(_T("WSASocket:%d\n"),WSAGetLastError());
		return FALSE;
    } 

	ReceiverBind.sin_family = AF_INET;
	ReceiverBind.sin_port = htons(port);
	ReceiverBind.sin_addr.S_un.S_addr = INADDR_ANY;

	bool bBroadcast = true;
	setsockopt(m_Socket,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));

	bind(m_Socket,(SOCKADDR *)&ReceiverBind,sizeof(ReceiverBind));

	ReceiverAddr.sin_family = AF_INET;
	ReceiverAddr.sin_port = htons(port);
	ReceiverAddr.sin_addr.S_un.S_addr = INADDR_BROADCAST;

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

BOOL CBroadCast::CreateBroadCastSender(char* lpstrAddr, unsigned short port)
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

	SenderAddr.sin_family = AF_INET;
	SenderAddr.sin_port = htons(port);
	SenderAddr.sin_addr.S_un.S_addr = inet_addr(lpstrAddr);

	bool bBroadcast = true;

	setsockopt(m_Socket,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));

	m_bConnected = TRUE;
	return TRUE;
}

int CBroadCast::Send(char *lpData, int size)
{
	if(m_Socket == INVALID_SOCKET)
		return -1;
	unsigned long nDataSent = 0;

	nDataSent = sendto(m_Socket,lpData,size,0,(SOCKADDR *)&SenderAddr,sizeof(SenderAddr));
	
	return nDataSent;
}

int CBroadCast::Receive(char *lpData, int size)
{
	WSABUF wsaRecvBuf;
	wsaRecvBuf.buf = lpData;
	wsaRecvBuf.len = size;
	unsigned long cbRet = 0;
	int ReceiverAddrsize = sizeof(ReceiverAddr);
//	m_AcceptSocket = accept(m_Socket,(SOCKADDR *)&Clientaddr,&iLen);

	cbRet = recvfrom(m_Socket,lpData,size,0,(SOCKADDR *)&ReceiverAddr,&ReceiverAddrsize);

	//通知父窗口读数据
	return cbRet;
}

void CBroadCast::Initilize()
{
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 2);
	int ret = WSAStartup(version, &wsaData);
	if(ret != 0)
		TRACE("Initilize Error!\n");
}

void CBroadCast::Unintilize()
{
	if (WSACleanup() != 0){
		TRACE("UnInitilize Error:%d\n", WSAGetLastError());
	}
}

BOOL CBroadCast::IsConnected()
{
	return m_bConnected;
}

void CBroadCast::Close()
{
	closesocket(m_Socket);
	m_bConnected = FALSE;
}
