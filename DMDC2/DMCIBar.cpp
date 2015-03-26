// DMCIBar.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "DMCIBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDMCIBar

BEGIN_MESSAGE_MAP(CDMCIBar, CSizingControlBarG)
	//{{AFX_MSG_MAP(CDMCIBar)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDMCIBar メッセージ ハンドラ

void CDMCIBar::OnSize(UINT nType, int cx, int cy) 
{
	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST);

	baseCDMCIBar::OnSize(nType, cx, cy);
}
