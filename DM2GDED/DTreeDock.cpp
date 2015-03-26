// DTreeDock.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DM2GDED.h"
#include "DTreeDock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDTreeDock

CDTreeDock::CDTreeDock(): baseDTreeDock()
{

}

CDTreeDock::~CDTreeDock()
{

}

BEGIN_MESSAGE_MAP(CDTreeDock, baseDTreeDock)
	//{{AFX_MSG_MAP(CDTreeDock)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(CDTreeDock, CControlBar);

/////////////////////////////////////////////////////////////////////////////
// CDTreeDock メッセージ ハンドラ

int CDTreeDock::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (baseDTreeDock::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_wndView.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP |TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_SHOWSELALWAYS, CRect(0, 0, 0, 0), this, 0x0100))
		return -1;
	m_wndView.ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	
	return 0;
}

void CDTreeDock::OnSize(UINT nType, int cx, int cy) 
{
	baseDTreeDock::OnSize(nType, cx, cy);
	
	RepositionBars(0, 0xffff, 0x0100);
	
}

void CDTreeDock::OnSetFocus(CWnd* pOldWnd) 
{
	m_wndView.SetFocus();
}

void CDTreeDock::PostNcDestroy() 
{
	CWnd::PostNcDestroy();

	delete this;
}
