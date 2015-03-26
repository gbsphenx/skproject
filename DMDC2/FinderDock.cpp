// FinderDock.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "FinderDock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFinderDock

CFinderDock::CFinderDock()
{
	const UINT cx = 81;
	m_szHorz = CSize(cx, cx);
	m_szVert = CSize(cx, cx);
}

CFinderDock::~CFinderDock()
{

}

BEGIN_MESSAGE_MAP(CFinderDock, CSizingControlBarCF)
	//{{AFX_MSG_MAP(CFinderDock)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFinderDock メッセージ ハンドラ

int CFinderDock::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBarCF::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_frm.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this))
		return -1;
	m_frm.SetDlgCtrlID(0x0100);
	m_frm.SetActiveView(&m_frm.m_vw);
	
	return 0;
}

void CFinderDock::OnSize(UINT nType, int cx, int cy) 
{
	CSizingControlBarCF::OnSize(nType, cx, cy);
	
	RepositionBars(0, 0xffff, 0x0100);
	
}
