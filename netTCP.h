

#ifndef MYNETTCP
#define MYNETTCP

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Winsock2.h"
#pragma comment(lib,"Ws2_32.lib")

class CNetTCP : public CObject  
{
public:
	void Close();
	bool IsConnected();
	static void Unintilize();
	static void Initilize();
	SOCKET getSocket(){ return m_Socket;};
	int Receive(SOCKET AcceptSocket, char* lpData, int size);
	int Send(char* lpData, int size);

	bool CreateTCPServer(unsigned short port, HWND hWnd, int Msg);
	bool CreateTCPClient(char* lpstrAddr, unsigned short port);
	CNetTCP();
	virtual ~CNetTCP();

private:
	HWND	m_hWnd;
	SOCKET	m_Socket;
	bool	m_bConnected;

	SOCKADDR_IN Serveraddr;
	SOCKADDR_IN Clientaddr;
};

#endif // !defined(AFX_MULTICAST_H__0F627997_5A90_11D5_B587_00E04C39B036__INCLUDED_)
