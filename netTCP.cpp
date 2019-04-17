// MultiCast.cpp: implementation of the CMultiCast class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "netTCP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetTCP::CNetTCP()
{
	Initilize();
	m_hWnd = NULL;
	m_bConnected = FALSE;
}

CNetTCP::~CNetTCP()
{
	Unintilize();
}

bool CNetTCP::CreateTCPServer(unsigned short port, HWND hWnd, int Msg)
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

	iLen = sizeof(Serveraddr);

	m_Socket = socket (AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (m_Socket == INVALID_SOCKET){
		AfxMessageBox(_T("WSASocket:%d\n"),WSAGetLastError());
		return FALSE;
    } 

	Serveraddr.sin_family = AF_INET;
	Serveraddr.sin_port = htons(port);
	Serveraddr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(m_Socket,(SOCKADDR *)&Serveraddr,sizeof(Serveraddr));

	listen(m_Socket,5);

	if(Msg != 0)
	{
		ret = WSAAsyncSelect(m_Socket, m_hWnd, Msg, FD_READ | FD_CONNECT | FD_ACCEPT);

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

bool CNetTCP::CreateTCPClient(char* lpstrAddr, unsigned short port)
{
	if(m_bConnected){
		AfxMessageBox(_T("Has Successfully Create the socket!\n"));
		return FALSE;
	}

    // 将字符串地址转换为套接字地址
	int iLen = 0;
	int cbRet = 0;
	int nIP_TTL = 16;//设置IP数据生存期

	iLen = sizeof(Serveraddr);

	m_Socket = socket (AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (m_Socket == INVALID_SOCKET){
		AfxMessageBox(_T("WSASocket is not created!"));
		return FALSE;
    } 
//(char *)(lpstrAddr.GetBuffer(lpstrAddr.GetLength())
	Clientaddr.sin_family = AF_INET;
	Clientaddr.sin_port = htons(port);
	Clientaddr.sin_addr.S_un.S_addr = inet_addr(lpstrAddr);

	int con = connect(m_Socket,(SOCKADDR *)&Clientaddr,sizeof(Clientaddr));
	if(con < 0)
	{
		AfxMessageBox(_T("WSAConnect is failed!"));
		m_bConnected = false;
		return FALSE;
	}

	m_bConnected = TRUE;
	return TRUE;
}

int CNetTCP::Send(char *lpData, int size)
{
	if(m_Socket == INVALID_SOCKET)
		return -1;
	unsigned long nDataSent = 0;

	nDataSent = send(m_Socket,lpData,size,0);
	
	return nDataSent;
}

int CNetTCP::Receive(SOCKET AcceptSocket, char *lpData, int size)
{
	WSABUF wsaRecvBuf;
	wsaRecvBuf.buf = lpData;
	wsaRecvBuf.len = size;
	unsigned long cbRet = 0;

//	m_AcceptSocket = accept(m_Socket,(SOCKADDR *)&Clientaddr,&iLen);

	cbRet = recv(AcceptSocket,lpData,size,0);

	//通知父窗口读数据
	return cbRet;
}

void CNetTCP::Initilize()
{
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 2);
	int ret = WSAStartup(version, &wsaData);
	if(ret != 0)
		TRACE("Initilize Error!\n");
}

void CNetTCP::Unintilize()
{
	if (WSACleanup() != 0){
		TRACE("UnInitilize Error:%d\n", WSAGetLastError());
	}
}

bool CNetTCP::IsConnected()
{
	return m_bConnected;
}

void CNetTCP::Close()
{
	closesocket(m_Socket);
	m_bConnected = FALSE;
}
