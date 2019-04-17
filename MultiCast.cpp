// MultiCast.cpp: implementation of the CMultiCast class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MultiCast.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultiCast::CMultiCast()
{
	Initilize();
	m_hWnd = NULL;
	m_bConnected = FALSE;
}

CMultiCast::~CMultiCast()
{
	Unintilize();
}

BOOL CMultiCast::Create(char* lpstrAddr, unsigned short port, HWND hWnd, int Msg)
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
	BOOL bFlag;
	iLen = sizeof(addr);

	m_hSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 
		WSA_FLAG_OVERLAPPED | WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF);
	if (m_hSocket == INVALID_SOCKET){
		AfxMessageBox(_T("WSASocket:%d\n"),WSAGetLastError());
		return FALSE;
    } 

	bFlag = TRUE; // 设置套接字为可重用端口地址
	ret = setsockopt(m_hSocket,	SOL_SOCKET,	SO_REUSEADDR, (char *)&bFlag,
				sizeof (bFlag)); 

	if(ret == SOCKET_ERROR){
		AfxMessageBox(_T("setsockopt:%d"),WSAGetLastError());
		return FALSE;
    } 
    // 将套接字绑扎到用户指定端口及默认的接口
    addr.sin_family = AF_INET;
    addr.sin_port = htons (port);
    addr.sin_addr.s_addr = INADDR_ANY;//inet_addr("169.254.245.57");
	ret = bind (m_hSocket, (struct sockaddr FAR *)&addr, 
		sizeof(struct sockaddr));

    if (ret == SOCKET_ERROR){
		TRACE("bind: %d", WSAGetLastError());
		return FALSE;
    }
    // 设置多址广播数据报传播范围（TTL）
	ret = WSAIoctl(m_hSocket, SIO_MULTICAST_SCOPE, &nIP_TTL, 
		sizeof (nIP_TTL), NULL, 0, (unsigned long*)&cbRet, NULL, NULL);           		 
    if (ret == SOCKET_ERROR){
		AfxMessageBox(_T("SAIoctl(SIO_MULTICAST_SCOPE):%d\n"),WSAGetLastError());
		return FALSE;
	}

	//允许或禁止组播数据帧回送；
	int bLoopback=FALSE; 
	WSAIoctl(m_hSocket,SIO_MULTIPOINT_LOOPBACK,&bLoopback,
		sizeof(bLoopback),NULL,0,(unsigned long*)&cbRet,NULL,NULL); 
		
	//设置组播地址
/*	char* chAddr = lpstrAddr.GetBuffer(0);
	ret = WSAStringToAddress((LPWSTR)lpstrAddr, AF_INET, 
		NULL, (LPSOCKADDR)&srcaddr, &iLen);          		
	if(ret == SOCKET_ERROR){
		m_bConnected = FALSE;
		AfxMessageBox(_T("WSAStringToAddress:%d\n",WSAGetLastError()));
		return FALSE;
	} */

	srcaddr.sin_family = AF_INET;
	srcaddr.sin_addr.S_un.S_addr = inet_addr(lpstrAddr);
	srcaddr.sin_port = htons (port);

//	WSABUF wsaCalleeData;
	m_hGroupSocket = WSAJoinLeaf(m_hSocket,	(LPSOCKADDR)&srcaddr,	
		sizeof(srcaddr), NULL, NULL, NULL, NULL, JL_BOTH);				 

    if (m_hGroupSocket == INVALID_SOCKET){
		m_bConnected = FALSE;
		AfxMessageBox(_T("WSAJoinLeaf():%d\n"), WSAGetLastError());
		return FALSE;
    } 

	if(Msg != 0)
	{
	ret = WSAAsyncSelect(m_hGroupSocket, m_hWnd, Msg, //WM_AIR_MOVE_EVENT, 
		FD_WRITE | FD_READ | FD_QOS | FD_GROUP_QOS | FD_CONNECT);

	if(ret == SOCKET_ERROR){
		AfxMessageBox(_T("WSAAsyncSelect():%d\n"), WSAGetLastError());
		m_bConnected = FALSE;
		return FALSE;
	}
	}
	m_bConnected = TRUE;
	return TRUE;
}

int CMultiCast::Send(char *lpData, int size)
{
	if(m_hSocket == INVALID_SOCKET)
		return -1;
	unsigned long nDataSent = 0;
	int nReturnCode;
	WSABUF wsaBuf;
	wsaBuf.len = size;
	wsaBuf.buf = lpData;
	nReturnCode = WSASendTo(m_hSocket, &wsaBuf, 1, &nDataSent, 
			0, (LPSOCKADDR)&srcaddr, sizeof(SOCKADDR), NULL, NULL);
	if(nReturnCode == SOCKET_ERROR){
		TRACE0("Error in send packets to the network!\n");
		return -1;
	}
	
	return nDataSent;
}

int CMultiCast::Receive(char *lpData, int size)
{
	int nReturnCode ;
	WSABUF wsaRecvBuf;
	wsaRecvBuf.buf = lpData;
	wsaRecvBuf.len = size;
	unsigned long cbRet = 0;

	int	iLen = sizeof (srcaddr);
	int	dFlag = 0;
	
	nReturnCode = WSARecvFrom(m_hGroupSocket, &wsaRecvBuf, 1,
		&cbRet, (unsigned long*)&dFlag, (struct sockaddr *)&srcaddr,
		&iLen, NULL, NULL); 
	if(nReturnCode == SOCKET_ERROR){
		if(WSAEWOULDBLOCK == WSAGetLastError())
			TRACE("Socket will block!\n");
		else{
			if(WSAGetLastError() == WSA_IO_PENDING)
				TRACE("IO is pending!\n");
			else
				return -1;
		}
		TRACE("RecvFrom Error:%d\n", WSAGetLastError());
	}
	//本次操作没有接收到数据
	if(cbRet == 0)
		return 0;

	//通知父窗口读数据
	return cbRet;
}

void CMultiCast::Initilize()
{
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 2);
	int ret = WSAStartup(version, &wsaData);
	if(ret != 0)
		TRACE("Initilize Error!\n");
}

void CMultiCast::Unintilize()
{
	if (WSACleanup() != 0){
		TRACE("UnInitilize Error:%d\n", WSAGetLastError());
	}
}

BOOL CMultiCast::IsConnected()
{
	return m_bConnected;
}

void CMultiCast::Close()
{
	closesocket(m_hSocket);
	closesocket(m_hGroupSocket);
	m_bConnected = FALSE;
}
