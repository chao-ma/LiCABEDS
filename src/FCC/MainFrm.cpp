// MainFrm.cpp : implementation of the CMainFrame class
//
#pragma once

#include "stdafx.h"
#include "FCC.h"

#include "MainFrm.h"
#include "FCCView.h"
#include "NoticeView.h"
#include "PredView.h"
#include "StartView.h"
#include "FCCDoc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern bool bAdaboostRunning;

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_VIEW_PREDICTION, &CMainFrame::OnViewPrediction)
	ON_COMMAND(ID_VIEW_TRAININGERROR, &CMainFrame::OnViewTrainingerror)
	ON_COMMAND(ID_VIEW_STARTUPPAGE, &CMainFrame::OnViewStartuppage)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR, 
	ID_SEPARATOR,// status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	firstrun=true;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetPaneInfo(0,0,SBPS_NOBORDERS   ,300);
	m_wndStatusBar.SetPaneInfo(1,0,SBPS_STRETCH,300);


	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	firstrun=false;
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	m_splitter.CreateStatic(this,2,1);
	//m_splitter.Create(this,2,1,CSize(10,10),pContext);
	m_splitter.CreateView(0,0,RUNTIME_CLASS(CStartView),CSize(0,0),pContext);
	m_splitter.CreateView(1,0,RUNTIME_CLASS(CNoticeView),CSize(0,0),pContext);
	m_splitter.SetRowInfo(0,500,101);
	//m_splitter.SetRowInfo(0,100,0);
	return true;
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	if(cx<100)
		cx=100;
	if(cy<200)
		cy=200;
	CFrameWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(firstrun==false && nType!=SIZE_MINIMIZED)
	{
		CRect rect;
		GetClientRect(rect);
		if(rect.Height()-185>0)
			m_splitter.SetRowInfo(0,rect.Height()-185,1);
		//m_splitter.SetRowInfo(1,140,0);
		//TRACE("on size set row info called\n");
		m_splitter.RecalcLayout();
	}
	
	
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	//CFrameWnd::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x=300;
	lpMMI->ptMinTrackSize.y=300;
}

void CMainFrame::OnViewPrediction()
{
	// TODO: Add your command handler code here
	CDocument* pDoc;
	CView* pView;
	CCreateContext context;
	CRect rect;
	pView=(CView*)m_splitter.GetPane(0,0);
	pView->GetWindowRect(rect);
	if(!pView)
	{
		AfxMessageBox("Unexpected error occurs");
		return;
	}
	pDoc=pView->GetDocument();
	CString title=pDoc->GetTitle();
	if(!pDoc)
	{
		AfxMessageBox("Unexpected error occurs");
		return;
	}
	pDoc->m_bAutoDelete=false;
	m_splitter.GetPane(0,0)->DestroyWindow();
	pDoc->m_bAutoDelete=true;
	context.m_pNewViewClass=RUNTIME_CLASS(CPredView);
	context.m_pCurrentDoc=pDoc;
	context.m_pLastView=NULL;
	context.m_pNewDocTemplate=NULL;
	context.m_pCurrentFrame=NULL;
	m_splitter.CreateView(0,0,RUNTIME_CLASS(CPredView),CSize(rect.Width(),rect.Height()),&context);
	((CView*)m_splitter.GetPane(0,0))->OnInitialUpdate();// it is important to call initial update before create a view expecially a CScrollView
	m_splitter.RecalcLayout();
	m_splitter.GetPane(0,0)->SendMessage(WM_PAINT);
	SetActiveView((CScrollView*)m_splitter.GetPane(0,0));
	pDoc->SetTitle(title);
	((CFCCDoc*)pDoc)->MessageCenter(IDS_MESSAGE_MAKEPREDICTION);
	

}

void CMainFrame::OnViewTrainingerror()
{
	// TODO: Add your command handler code here
	CDocument* pDoc;
	CView* pView;
	CCreateContext context;
	CRect rect;
	pView=(CView*)m_splitter.GetPane(0,0);
	pView->GetWindowRect(rect);
	if(!pView)
	{
		AfxMessageBox("Unexpected error occurs");
		return;
	}
	pDoc=pView->GetDocument();
	CString title=pDoc->GetTitle();
	if(!pDoc)
	{
		AfxMessageBox("Unexpected error occurs");
		return;
	}
	pDoc->m_bAutoDelete=false;
	m_splitter.GetPane(0,0)->DestroyWindow();
	pDoc->m_bAutoDelete=true;
	context.m_pNewViewClass=RUNTIME_CLASS(CFCCView);
	context.m_pCurrentDoc=pDoc;
	context.m_pLastView=NULL;
	context.m_pNewDocTemplate=NULL;
	context.m_pCurrentFrame=NULL;
	m_splitter.CreateView(0,0,RUNTIME_CLASS(CFCCView),CSize(rect.Width(),rect.Height()),&context);
	((CView*)m_splitter.GetPane(0,0))->OnInitialUpdate();// it is important to call initial update before create a view expecially a CScrollView
	m_splitter.RecalcLayout();
	m_splitter.GetPane(0,0)->SendMessage(WM_PAINT);
	SetActiveView((CScrollView*)m_splitter.GetPane(0,0));
	pDoc->SetTitle(title);
}

void CMainFrame::OnViewStartuppage()
{
	// TODO: Add your command handler code here
	CDocument* pDoc;
	CView* pView;
	CCreateContext context;
	CRect rect;
	pView=(CView*)m_splitter.GetPane(0,0);
	pView->GetWindowRect(rect);
	if(!pView)
	{
		AfxMessageBox("Unexpected error occurs");
		return;
	}
	pDoc=pView->GetDocument();
	CString title=pDoc->GetTitle();
	if(!pDoc)
	{
		AfxMessageBox("Unexpected error occurs");
		return;
	}
	pDoc->m_bAutoDelete=false;
	m_splitter.GetPane(0,0)->DestroyWindow();
	pDoc->m_bAutoDelete=true;
	context.m_pNewViewClass=RUNTIME_CLASS(CStartView);
	context.m_pCurrentDoc=pDoc;
	context.m_pLastView=NULL;
	context.m_pNewDocTemplate=NULL;
	context.m_pCurrentFrame=NULL;
	m_splitter.CreateView(0,0,RUNTIME_CLASS(CStartView),CSize(rect.Width(),rect.Height()),&context);
	((CView*)m_splitter.GetPane(0,0))->OnInitialUpdate();// it is important to call initial update before create a view expecially a CScrollView
	m_splitter.RecalcLayout();
	m_splitter.GetPane(0,0)->SendMessage(WM_PAINT);
	SetActiveView((CScrollView*)m_splitter.GetPane(0,0));
	pDoc->SetTitle(title);
}
