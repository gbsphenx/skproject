// ChildFrame.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "ChildFrame.h"
#include "DM2GDEDDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}


BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame メッセージ ハンドラ

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
//	MDICREATESTRUCT* lpmcs;
//	lpmcs = (MDICREATESTRUCT*)lpCreateStruct->lpCreateParams;
//	CCreateContext* pContext = (CCreateContext*)lpmcs->lParam;
//
//	CGDEdCtx *pEdCtx = static_cast<CDM2GDEDDoc *>(pContext->m_pCurrentDoc)->GetEdCtx(FALSE);
//
//	if (false
//		|| !m_wndSplitter.CreateStatic(this, 1, 2, 0 |WS_CHILD |WS_VISIBLE, AFX_IDW_DIALOGBAR)
//		|| !m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CDTreeDock), CSize(200, 200), pContext)
//		|| !m_wndSplitter.CreateView(0, 1, pContext->m_pNewViewClass, CSize(200, 200), pContext)
//	) {
//		return -1;
//	}
//
//	m_pVw = static_cast<CDTreeDock *>(m_wndSplitter.GetPane(0, 0));
//
//	m_pVw->Init(pEdCtx);

//	if (false
//		|| !m_wndTreeBar.Create("Tree", this, AFX_IDW_DIALOGBAR, WS_CHILD |WS_VISIBLE |CBRS_LEFT |CBRS_SIZE_DYNAMIC)
//	) {
//		return -1;
//	}

	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

//	m_wndView->EnableDocking(CBRS_ORIENT_VERT);

//	EnableDocking(CBRS_ALIGN_ANY);

//	DockControlBar(&m_wndTreeBar);

	return 0;
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CMDIChildWnd::PreCreateWindow(cs);
}

void CChildFrame::ActivateFrame(int nCmdShow) 
{
	nCmdShow = SW_SHOWMAXIMIZED;
	
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (!CMDIChildWnd::OnCreateClient(lpcs, pContext))
		return FALSE;

	CWnd *pVw = GetDlgItem(AFX_IDW_PANE_FIRST);
	if (pVw && pVw->IsKindOf(RUNTIME_CLASS(CView))) {
		SetActiveView((CView *)pVw);
	}

	return TRUE;
}
