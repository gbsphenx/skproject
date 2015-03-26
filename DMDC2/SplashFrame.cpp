// SplashFrame.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "SplashFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplashDialogBar

BEGIN_MESSAGE_MAP(CSplashDialogBar, CDialogBar)
	//{{AFX_MSG_MAP(CSplashDialogBar)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplashFrame

CSplashFrame::CSplashFrame()
{
}

CSplashFrame::~CSplashFrame()
{
}


BEGIN_MESSAGE_MAP(CSplashFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CSplashFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplashFrame メッセージ ハンドラ

BOOL CSplashFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CFrameWnd::PreCreateWindow(cs)) return false;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return true;
}

BOOL CSplashFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (!m_vw.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, pContext))
		return false;

	return true;
}

int CSplashFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_bar.Create(this, IDD_SPLASH, 0 |WS_CHILD |WS_VISIBLE |CBRS_BOTTOM, AFX_IDW_DIALOGBAR))
		return -1;

	if (!m_horz.SubclassDlgItem(IDC_STATIC_HINT, &m_bar))
		return -1;
	
	return 0;
}
