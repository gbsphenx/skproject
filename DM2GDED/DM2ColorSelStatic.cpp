// DM2ColorSelStatic.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2ColorSelStatic.h"
#include "DM2Palo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2ColorSelStatic

CDM2ColorSelStatic::CDM2ColorSelStatic()
{
	m_iSel = -1;

	ZeroMemory(m_rcBox, sizeof(m_rcBox));
}

BEGIN_MESSAGE_MAP(CDM2ColorSelStatic, CStatic)
	//{{AFX_MSG_MAP(CDM2ColorSelStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2ColorSelStatic メッセージ ハンドラ

BOOL CDM2ColorSelStatic::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CDM2ColorSelStatic::OnPaint() 
{
	CPaintDC dc(this);

	dc.SaveDC();

	CRect rc;
	GetClientRect(&rc);

	for (int i = 0; i < 256; i++) {
		COLORREF clr;
		CDM2Palo::GetColor(i, clr);

		dc.FillSolidRect(m_rcBox[i], clr);
		dc.ExcludeClipRect(m_rcBox[i]);
	}

	dc.FillSolidRect(&rc, RGB(0, 0, 0));
	dc.RestoreDC(-1);
}

LRESULT CDM2ColorSelStatic::OnNcHitTest(CPoint point) 
{
	UINT x = CStatic::OnNcHitTest(point);
	if (x == HTTRANSPARENT)
		x = HTCLIENT;

	return x;
}

void CDM2ColorSelStatic::PreSubclassWindow() 
{
	Init();
	
	CStatic::PreSubclassWindow();
}

void CDM2ColorSelStatic::Init()
{
	CRect rc;
	GetClientRect(&rc);
	rc.DeflateRect(1, 1);

	int cxBox = _MAX(1, rc.Width() / 256);

	for (int i = 0; i < 256; i++) {
		m_rcBox[i].SetRect(
			cxBox * (i+0),
			rc.top,
			cxBox * (i+1),
			rc.bottom
			);
	}
	Invalidate();
}

void CDM2ColorSelStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	for (int i = 0; i < 256; i++) {
		if (m_rcBox[i].PtInRect(point)) {
			OnSetSel(i, true);
		}
	}
	
	CStatic::OnLButtonDown(nFlags, point);
}
