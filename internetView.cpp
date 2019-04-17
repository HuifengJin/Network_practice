
// internetView.cpp : CinternetView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "internet.h"
#endif

#include "internetDoc.h"
#include "internetView.h"
#include <WinSock2.h>
#include <WinSock.h>
#pragma comment(lib,"ws2_32.lib")
//#include "point_tcpip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CinternetView

IMPLEMENT_DYNCREATE(CinternetView, CView)

BEGIN_MESSAGE_MAP(CinternetView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CinternetView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_POINT_TCP_SERVER, &CinternetView::OnPointTcpServer)
	ON_COMMAND(ID_POINT_TCP_CLIENT, &CinternetView::OnPointTcpClient)
	ON_COMMAND(ID_POINT_UDP_SEND, &CinternetView::OnPointUdpSend)
	ON_COMMAND(ID_POINT_UDP_RECEIVE, &CinternetView::OnPointUdpReceive)
	ON_COMMAND(ID_BROADCAST_SEND, &CinternetView::OnBroadcastSend)
	ON_COMMAND(ID_BROADCAST_RECEIVE, &CinternetView::OnBroadcastReceive)
	ON_COMMAND(ID_MULTICAST_SEND, &CinternetView::OnMulticastSend)
	ON_COMMAND(ID_MULTICAST_RECEIVE, &CinternetView::OnMulticastReceive)
	ON_MESSAGE(WM_TCP_MSG, OnTCPReceive) 
	ON_MESSAGE(WM_UDP_MSG, OnUDPReceive) 
	ON_MESSAGE(WM_BC_MSG, OnBCReceive) 
	ON_MESSAGE(WM_MC_MSG, OnMCReceive) 
	ON_COMMAND(IDC_NET_SEND, &CinternetView::OnNetSend)
	ON_COMMAND(IDC_NET_CLOSE, &CinternetView::OnNetClose)
END_MESSAGE_MAP()

// CinternetView 构造/析构

CinternetView::CinternetView()
{
	// TODO: 在此处添加构造代码

}

CinternetView::~CinternetView()
{
}

BOOL CinternetView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CinternetView 绘制

void CinternetView::OnDraw(CDC* pDC)
{
	CinternetDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	pDC->TextOutW(10,10,msgString.GetBuffer(msgString.GetLength()));


}


// CinternetView 打印


void CinternetView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CinternetView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CinternetView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CinternetView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CinternetView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CinternetView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CinternetView 诊断

#ifdef _DEBUG
void CinternetView::AssertValid() const
{
	CView::AssertValid();
}

void CinternetView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CinternetDoc* CinternetView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CinternetDoc)));
	return (CinternetDoc*)m_pDocument;
}
#endif //_DEBUG


// CinternetView 消息处理程序

void CinternetView::OnPointTcpServer()
{
	bool status = false;

	m_netType = netTCPServer;

	status = m_serverST.CreateTCPServer(5150, this->m_hWnd, WM_TCP_MSG);

	if(!status)
		AfxMessageBox(_T("Create TCP server failed!"));
}

void CinternetView::OnPointTcpClient()
{
	bool status = false;

	m_netType = netTCPClient;

	status = m_clientST.CreateTCPClient("127.0.0.1",5150);

	if(!status)
		AfxMessageBox(_T("Create TCP client failed!"));
}

void CinternetView::OnPointUdpSend()
{
	// TODO: 在此添加命令处理程序代码


	bool status = false;

	m_netType = netUDPSender;

	status = m_sendUDP.CreateUDPSender(5150,"127.0.0.1");

	if(!status)
		AfxMessageBox(_T("Create UDP client failed!"));
}


void CinternetView::OnPointUdpReceive()
{
	// TODO: 在此添加命令处理程序代码


	bool status = false;

	m_netType = netUDPReceiver;

	status = m_receiveUDP.CreateUDPReceiver(5150, this->m_hWnd, WM_UDP_MSG);

	if(!status)
		AfxMessageBox(_T("Create UDP server failed!"));
}


void CinternetView::OnBroadcastSend()
{
	// TODO: 在此添加命令处理程序代码

	bool status = false;

	m_netType = netBCSender;

	status = m_sendBC.CreateBroadCastSender("255.255.255.255",5150);

	if(!status)
		AfxMessageBox(_T("Create BC client failed!"));
}


void CinternetView::OnBroadcastReceive()
{
	// TODO: 在此添加命令处理程序代码

	bool status = false;

	m_netType = netBCReceiver;

	status = m_receiveBC.CreateBroadCastReceiver(5150, this->m_hWnd, WM_BC_MSG);

	if(!status)
		AfxMessageBox(_T("Create BC server failed!"));
}


void CinternetView::OnMulticastSend()
{
	// TODO: 在此添加命令处理程序代码

	bool status = false;

	m_netType = netMCSender;

	status = m_sendMC.Create("225.0.1.20",3200,this->m_hWnd, WM_MC_MSG);

	if(!status)
		AfxMessageBox(_T("Create MC client failed!"));

}


void CinternetView::OnMulticastReceive()
{
	// TODO: 在此添加命令处理程序代码

	bool status = false;

	m_netType = netMCReceiver;

	status = m_receiveMC.Create("225.0.1.20",3200,this->m_hWnd, WM_MC_MSG);

	if(!status)
		AfxMessageBox(_T("Create MC server failed!"));

}

long CinternetView::OnTCPReceive(WPARAM wParam, LPARAM lParam)
{
	int i;
	int rsize=0;

	SOCKET EventSocket = (SOCKET)wParam;

	switch(WSAGETSELECTEVENT(lParam))
	{
		case FD_ACCEPT:
			SOCKADDR addr;
			int len;
			SOCKET temp;
			temp = accept(m_serverST.getSocket(),(SOCKADDR*)&addr,&len);
			break;
		case FD_READ:
		//	AfxMessageBox(_T("received data!"));
			rsize =  m_serverST.Receive(EventSocket,(char *)&m_studinfo,sizeof(studentInfo));
			if(rsize > 0)
			{
				char buf[50];
				sprintf(buf,"学号：%d， 姓名：%s",m_studinfo.xuehao,m_studinfo.name);
				msgString = buf;
				Invalidate();
			}
			break;
		default:
			break;
	}
	return 0L;
}

long CinternetView::OnUDPReceive(WPARAM wParam, LPARAM lParam)
{
	int i;
	int rsize=0;
	//MessageBox(_T("received!"));
	switch(WSAGETSELECTEVENT(lParam))
	{
		case FD_READ:
			rsize =  m_receiveUDP.Receive((char *)&m_studinfo,sizeof(studentInfo));
			if(rsize > 0)
			{
				char buf[50];
				sprintf(buf,"学号：%d， 姓名：%s",m_studinfo.xuehao,m_studinfo.name);
				msgString = buf;
				Invalidate();
			}
			break;
		default:
			break;
	}
	return 0L;
}

long CinternetView::OnBCReceive(WPARAM wParam, LPARAM lParam)
{
	int i;
	int rsize=0;
	MessageBox(_T("received!"));
	switch(WSAGETSELECTEVENT(lParam))
	{
		case FD_READ:
			MessageBox(_T("received again!"));
			rsize =  m_receiveBC.Receive((char *)&m_studinfo,sizeof(studentInfo));
			if(rsize > 0)
			{
				MessageBox(_T("received data!"));
				char buf[50];
				sprintf(buf,"学号：%d， 姓名：%s",m_studinfo.xuehao,m_studinfo.name);
				msgString = buf;
				Invalidate();
			}
			break;
		default:
			break;
	}
	return 0L;
}

long CinternetView::OnMCReceive(WPARAM wParam, LPARAM lParam)
{
	int i;
	int rsize=0;
	MessageBox(_T("received!"));
	switch(WSAGETSELECTEVENT(lParam))
	{
		case FD_READ:
			MessageBox(_T("received again!"));
			rsize =  m_receiveMC.Receive((char *)&m_studinfo,sizeof(studentInfo));
			if(rsize > 0)
			{
				MessageBox(_T("received data!"));
				char buf[50];
				sprintf(buf,"学号：%d， 姓名：%s",m_studinfo.xuehao,m_studinfo.name);
				msgString = buf;
				Invalidate();
			}
			break;
		default:
			break;
	}
	return 0L;
}

void CinternetView::OnNetSend()
{
	int rsize = 0;
	// TODO: 在此添加命令处理程序代码
	switch(m_netType)
	{
		case netTCPClient:
			m_studinfo.xuehao = 1;
			sprintf(m_studinfo.name,"chenzebin");
			rsize = m_clientST.Send((char *)&m_studinfo,sizeof(studentInfo));
			if(rsize > 0)
			{
				char buf[50];
				sprintf(buf,"学号：%d， 姓名：%s",m_studinfo.xuehao,m_studinfo.name);
				msgString = buf;
				Invalidate();
			}
			break;
		case netUDPSender:
			m_studinfo.xuehao = 2;
			sprintf(m_studinfo.name,"hansiyi");
			rsize = m_sendUDP.Send((char *)&m_studinfo,sizeof(studentInfo));
			if(rsize > 0)
			{
				char buf[50];
				sprintf(buf,"学号：%d， 姓名：%s",m_studinfo.xuehao,m_studinfo.name);
				msgString = buf;
				Invalidate();
			}
			break;
		case netBCSender:
			m_studinfo.xuehao = 3;
			sprintf(m_studinfo.name,"huangyilan");
			rsize = m_sendBC.Send((char *)&m_studinfo,sizeof(studentInfo));
			if(rsize > 0)
			{
				char buf[50];
				sprintf(buf,"学号：%d， 姓名：%s",m_studinfo.xuehao,m_studinfo.name);
				msgString = buf;
				Invalidate();
			}
			break;
		case netMCSender:
			m_studinfo.xuehao = 4;
			sprintf(m_studinfo.name,"jinhuifeng");
			rsize = m_sendMC.Send((char *)&m_studinfo,sizeof(studentInfo));
			if(rsize > 0)
			{
				char buf[50];
				sprintf(buf,"学号：%d， 姓名：%s",m_studinfo.xuehao,m_studinfo.name);
				msgString = buf;
				Invalidate();
			}
			break;
		default:
			break;
	}
}


void CinternetView::OnNetClose()
{
	// TODO: 在此添加命令处理程序代码
	switch(m_netType)
	{
		case netTCPServer:
			m_serverST.Close();
			break;
		case netTCPClient:
			m_clientST.Close();
			break;
		case netUDPReceiver:
			m_receiveUDP.Close();
			break;
		case netUDPSender:
			m_sendUDP.Close();
			break;
		case netBCReceiver:
			m_receiveBC.Close();
			break;
		case netBCSender:
			m_sendBC.Close();
			break;
		case netMCReceiver:
			m_receiveMC.Close();
			break;
		case netMCSender:
			m_sendMC.Close();
			break;
		default:
			break;
	}
}
