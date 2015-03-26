// ShowcaseFrame.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "ShowcaseFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowcaseFrame

CShowcaseFrame::CShowcaseFrame()
{
}

CShowcaseFrame::~CShowcaseFrame()
{
}


BEGIN_MESSAGE_MAP(CShowcaseFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CShowcaseFrame)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowcaseFrame メッセージ ハンドラ

BOOL CShowcaseFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (!m_vw.Create(NULL, NULL, 0 |WS_CHILD |WS_VISIBLE, CRect(0, 0, 0, 0), this, 0x0100, pContext))
		return false;
	if (!m_wndToolBar.CreateEx(this, 0 |TBSTYLE_FLAT |TBSTYLE_TOOLTIPS))
		return false;
	if (!m_wndToolBar.LoadToolBar(IDR_SHOWCASE))
		return false;

	return true;
}

void CShowcaseFrame::OnSize(UINT nType, int cx, int cy) 
{
	RepositionBars(0, 0xffff, 0x0100);
}

BOOL CShowcaseFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.hMenu = NULL;
	
	return CFrameWnd::PreCreateWindow(cs);
}
