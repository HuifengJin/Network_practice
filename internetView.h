
// internetView.h : CinternetView ��Ľӿ�
//

#pragma once

#include <cstring>

#include "BroadCast.h"
#include "MultiCast.h"
#include "netTCP.h"
#include "netUDP.h"
#include "BroadCast.h"
#include "MultiCast.h"

#define WM_TCP_MSG WM_USER+11
#define WM_UDP_MSG WM_USER+22
#define WM_BC_MSG WM_USER+33
#define WM_MC_MSG WM_USER+44

typedef struct stru_studentInfo
{
	int xuehao;
	char name[20];
}studentInfo;

enum netType
{
	netTCPServer,
	netTCPClient,
	netUDPReceiver,
	netUDPSender,
	netBCReceiver,
	netBCSender,
	netMCReceiver,
	netMCSender
};

class CinternetView : public CView
{
protected: // �������л�����
	CinternetView();
	DECLARE_DYNCREATE(CinternetView)

// ����
public:
	CinternetDoc* GetDocument() const;

// ����
public:
	CString  msgString;
	studentInfo m_studinfo;
	
	netType m_netType;
	CNetTCP m_serverST,m_clientST;
	CNetUDP m_receiveUDP,m_sendUDP;
	CBroadCast m_receiveBC,m_sendBC;
	CMultiCast m_receiveMC,m_sendMC;

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CinternetView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPointTcpServer();
	afx_msg void OnPointTcpClient();
	afx_msg void OnPointUdpSend();
	afx_msg void OnPointUdpReceive();
	afx_msg void OnBroadcastSend();
	afx_msg void OnBroadcastReceive();
	afx_msg void OnMulticastSend();
	afx_msg void OnMulticastReceive();
	afx_msg long OnTCPReceive(WPARAM wParam, LPARAM lParam);
	afx_msg long OnUDPReceive(WPARAM wParam, LPARAM lParam);
	afx_msg long OnBCReceive(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMCReceive(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNetSend();
	afx_msg void OnNetClose();
};

#ifndef _DEBUG  // internetView.cpp �еĵ��԰汾
inline CinternetDoc* CinternetView::GetDocument() const
   { return reinterpret_cast<CinternetDoc*>(m_pDocument); }
#endif

