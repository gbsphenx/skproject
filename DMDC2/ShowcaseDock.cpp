// ShowcaseDock.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "ShowcaseDock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowcaseDock

CShowcaseDock::CShowcaseDock()
{
}

CShowcaseDock::~CShowcaseDock()
{
}

BEGIN_MESSAGE_MAP(CShowcaseDock, CSizingControlBarCF)
	//{{AFX_MSG_MAP(CShowcaseDock)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CShowcaseDock メッセージ ハンドラ

int CShowcaseDock::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBarCF::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_frame.LoadFrame(IDR_SHOWCASE, 0 |WS_VISIBLE |WS_CHILD, this, NULL))
		return -1;

	m_frame.SetDlgCtrlID(0x0100);
	m_frame.SetActiveView(&m_frame.m_vw);
	
	return 0;
}

void CShowcaseDock::OnSize(UINT nType, int cx, int cy) 
{
	CSizingControlBarCF::OnSize(nType, cx, cy);
	
	RepositionBars(0, 0xffff, 0x0100);
}
