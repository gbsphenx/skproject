// MainFrm.cpp : CMainFrame クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "DM2GDED.h"

#include "MainFrm.h"
#include "DM2GDEDDoc.h"
#include "GridCtrl.h"
#include "DTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_VIEW_TREE_BAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_TREE_BAR, OnBarCheck)
//	ON_COMMAND(ID_NEXT_PANE, OnNextPane)
//	ON_COMMAND(ID_PREV_PANE, OnPrevPane)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケータ
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの構築/消滅

CMainFrame::CMainFrame()
{
	m_dwPromptContext = 101;
}

CMainFrame::~CMainFrame()
{

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//---
	if (false
		|| !m_wndToolBar.Create(this, 0 |WS_CHILD |WS_VISIBLE |CBRS_TOP |CBRS_TOOLTIPS |CBRS_FLYBY |CBRS_SIZE_DYNAMIC)
		|| !m_wndToolBar.LoadToolBar(IDR_MAINFRAME)
	) {
		return -1;
	}

	m_wndToolBar.SetFlatLook();
	m_wndToolBar.SetHeight(GetSystemMetrics(SM_CYSMICON) + 12);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

	//---
	if (false
		|| !m_wndStatusBar.Create(this)
		|| !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT))
	) {
		return -1;
	}

	//---
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	EnableDocking(CBRS_ALIGN_ANY);

#ifdef _SCB_REPLACE_MINIFRAME
    m_pFloatingFrameClass = RUNTIME_CLASS(CSCBMiniDockFrameWnd);
#endif //_SCB_REPLACE_MINIFRAME

	DockControlBar(&m_wndToolBar);
//	DockControlBar(&m_wndTreeBar);

#ifdef _DEBUG
	SetTimer(0x0100, 1000, NULL);
#endif

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: この位置で CREATESTRUCT cs を修正して、Window クラスやスタイルを
	//       修正してください。

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メッセージ ハンドラ

void CMainFrame::OnTimer(UINT nIDEvent) 
{
#if 0
	switch (nIDEvent) {
	case 0x0100:
		{
			CWnd *pWnd;
			CString s;
			if (pWnd = GetFocus()) {
				TCHAR tc[256] = {""};
				GetClassName(*pWnd, tc, 256);
				pWnd->GetWindowText(s); TRACE2("GetFocus(): \"%s\",\"%s\"\n", tc, (LPCSTR)s);
			}
			break;
		}
	}
#endif
	
	CMDIFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnDestroy() 
{
	CMDIFrameWnd::OnDestroy();
	
#ifdef _DEBUG
	KillTimer(0x0100);
#endif
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	switch (nState) {
	case WA_ACTIVE:
		{
			CWnd *pWnd = GetActiveView();
			if (pWnd)
				pWnd->SetFocus();
			break;
		}
	}
}

void CMainFrame::OnNextPane() 
{
//	CWnd *pWnd = GetFocus();
//	if (pWnd->IsKindOf(RUNTIME_CLASS(CGridCtrl))) {
//		m_wndTreeBar.SetFocus();
//		return;
//	}
//	if (pWnd->IsKindOf(RUNTIME_CLASS(CDTreeView))) {
//		CWnd *pWnd = GetActiveView();
//		if (pWnd)
//			pWnd->SetFocus();
//		return;
//	}
}

void CMainFrame::OnPrevPane() 
{
	OnNextPane();
}

BOOL CMainFrame::OnQueryNewPalette() 
{
	CView *pView = GetActiveView();
	if (pView) {
		pView->SendMessage(WM_QUERYNEWPALETTE);
	}
	
	return CMDIFrameWnd::OnQueryNewPalette();
}

void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd) 
{
	CView *pView = GetActiveView();
	if (pView) {
		pView->SendMessage(WM_PALETTECHANGED, (WPARAM)(HWND)*pFocusWnd);
	}
	CMDIFrameWnd::OnPaletteChanged(pFocusWnd);
}

void CMainFrame::OnSysColorChange() 
{
	CMDIFrameWnd::OnSysColorChange();
	
	SendMessageToDescendants(WM_SYSCOLORCHANGE, 0, 0, TRUE, TRUE);
	
}
