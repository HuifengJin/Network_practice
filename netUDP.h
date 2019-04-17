#if !defined(NETUDP)
#define NETUDP

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Winsock2.h"
#pragma comment(lib,"Ws2_32.lib")

class CNetUDP : public CObject  
{
public:
	void Close();
	BOOL IsConnected();
	static void Unintilize();
	static void Initilize();
	int Receive(char* lpData, int size);
	int Send(char* lpData, int size);

	BOOL CreateUDPReceiver(unsigned short port, HWND hWnd, int Msg);
	BOOL CreateUDPSender(unsigned short port,char* lpstrAddr);
	CNetUDP();
	virtual ~CNetUDP();

private:
	HWND	m_hWnd;
	SOCKET	m_Socket;
	BOOL	m_bConnected;

	SOCKADDR_IN Receiveraddr;
	SOCKADDR_IN Senderaddr;
};

#endif // !defined(AFX_MULTICAST_H__0F627997_5A90_11D5_B587_00E04C39B036__INCLUDED_)