// MultiCast.h: interface for the CMultiCast class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTICAST_H__0F627997_5A90_11D5_B587_00E04C39B036__INCLUDED_)
#define AFX_MULTICAST_H__0F627997_5A90_11D5_B587_00E04C39B036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Winsock2.h"
#pragma comment(lib,"Ws2_32.lib")

#define WM_AIR_MOVE_EVENT WM_USER + 1001
#define WM_GROUND_MOVE_EVENT WM_USER + 1002
#define WM_OWNSHIP WM_USER + 1003
#define WM_SIMU_CONTROL WM_USER + 1004
#define WM_EQUIP_STATE WM_USER + 1005

class CMultiCast : public CObject  
{
public:
	void Close();
	BOOL IsConnected();
	static void Unintilize();
	static void Initilize();
	int Receive(char* lpData, int size);
	int Send(char* lpData, int size);
	SOCKET Get_Group_Socket(){ return m_hGroupSocket;};
	BOOL Create(char* lpstrAddr, unsigned short port, HWND hWnd, int Msg);
	CMultiCast();
	virtual ~CMultiCast();

private:
	HWND	m_hWnd;
	SOCKET	m_hSocket;
	SOCKET  m_hGroupSocket;
	BOOL	m_bConnected;

	SOCKADDR_IN addr;
	SOCKADDR_IN srcaddr;
};

#endif // !defined(AFX_MULTICAST_H__0F627997_5A90_11D5_B587_00E04C39B036__INCLUDED_)
