// FinderFrm.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "FinderFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFinderFrm

CFinderFrm::CFinderFrm()
{
}

CFinderFrm::~CFinderFrm()
{
}


BEGIN_MESSAGE_MAP(CFinderFrm, CFrameWnd)
	//{{AFX_MSG_MAP(CFinderFrm)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFinderFrm メッセージ ハンドラ

void CFinderFrm::PostNcDestroy() 
{

}

BOOL CFinderFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (!m_vw.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0,0,0,0), this, AFX_IDW_PANE_FIRST))
		return false;
	if (!m_wndToolBar.CreateEx(this, 0 |TBSTYLE_FLAT |TBSTYLE_TOOLTIPS, WS_CHILD|WS_VISIBLE|CBRS_ALIGN_LEFT))
		return false;
	if (!m_wndToolBar.LoadToolBar(IDR_FINDER))
		return false;

	return true;
}

BOOL CFinderFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return false;
	//cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return true;
}

void CFinderFrm::OnSize(UINT nType, int cx, int cy) 
{
	bool fHorz = (static_cast<CControlBar *>(GetParent())->GetBarStyle() & CBRS_ORIENT_HORZ) != 0;

	m_wndToolBar.SetBarStyle(
		(m_wndToolBar.GetBarStyle()
		&(~(CBRS_ALIGN_LEFT|CBRS_ALIGN_TOP)))
		|( fHorz ? CBRS_ALIGN_LEFT : CBRS_ALIGN_TOP)
	);

	CFrameWnd::OnSize(nType, cx, cy);
}
