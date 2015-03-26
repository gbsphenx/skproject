// DM2OrnatePvw.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "DM2OrnatePvw.h"
#include "CompatDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2OrnatePvw

BEGIN_MESSAGE_MAP(CDM2OrnatePvw, CStatic)
	//{{AFX_MSG_MAP(CDM2OrnatePvw)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2OrnatePvw メッセージ ハンドラ

void CDM2OrnatePvw::OnPaint() 
{
	CPaintDC dc(this);

	if (m_iOrnate < 0) return;

	//
	CRect rc;
	GetClientRect(&rc);
	//dc.FillSolidRect(&rc, GetSysColor(COLOR_BTNFACE));
	CPoint pt((rc.right - 128) / 2, (rc.bottom - 128) / 2);
	GetApp()->RenderOrnate64To(&dc, pt.x, pt.y, m_iClass, m_iOrnate, 128);
	//
}

int CDM2OrnatePvw::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_iClass = -1;
	m_iOrnate = -1;
	
	return 0;
}

BOOL CDM2OrnatePvw::OnEraseBkgnd(CDC* pDC) 
{
	pDC->SaveDC();
	CRect rcMax;
	GetClientRect(&rcMax);
	CRect rcMin(rcMax.CenterPoint(), CSize(0,0));
	rcMin.InflateRect(64, 64);
	pDC->FillSolidRect(&rcMin, RGB(64,64,64));
	pDC->ExcludeClipRect(&rcMin);
	pDC->FillSolidRect(&rcMax, RGB(0,0,0));
	pDC->RestoreDC(-1);

	return TRUE;
//	return CStatic::OnEraseBkgnd(pDC);
}
