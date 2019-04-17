#if !defined(NETTCP)
#define NETTCP

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Winsock2.h"
#pragma comment(lib,"Ws2_32.lib")

class CBroadCast : public CObject  
{
public:
	void Close();
	BOOL IsConnected();
	static void Unintilize();
	static void Initilize();
	int Receive(char* lpData, int size);
	int Send(char* lpData, int size);

	BOOL CreateBroadCastReceiver(unsigned short port, HWND hWnd, int Msg);
	BOOL CreateBroadCastSender(char* lpstrAddr, unsigned short port);
	CBroadCast();
	virtual ~CBroadCast();

private:
	HWND	m_hWnd;
	SOCKET	m_Socket;
	BOOL	m_bConnected;

	SOCKADDR_IN ReceiverAddr;
	SOCKADDR_IN SenderAddr;
	SOCKADDR_IN ReceiverBind;
};

#endif // !defined(AFX_MULTICAST_H__0F627997_5A90_11D5_B587_00E04C39B036__INCLUDED_)
