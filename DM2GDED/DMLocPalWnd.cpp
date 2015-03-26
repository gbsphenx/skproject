// DMLocPalWnd.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DM2GDED.h"
#include "DMLocPalWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

/////////////////////////////////////////////////////////////////////////////
// CDMLocPalWnd

CDMLocPalWnd::CDMLocPalWnd()
{
	for (UINT i = 0; i < 16; i++) m_bPalIdx[i] = 0, m_clr[i] = 0;
}

CDMLocPalWnd::~CDMLocPalWnd()
{

}

BEGIN_MESSAGE_MAP(CDMLocPalWnd, CStatic)
	//{{AFX_MSG_MAP(CDMLocPalWnd)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMLocPalWnd メッセージ ハンドラ

void CDMLocPalWnd::SetItem(UINT iIdx, BYTE iPalIdx, COLORREF clr)
{
	iIdx = iIdx % 16;
	m_bPalIdx[iIdx] = iPalIdx;
	m_clr[iIdx] = clr;
}

BOOL CDMLocPalWnd::GetItem(UINT iIdx, BYTE &iPalIdx, COLORREF &clr)
{
	iIdx = iIdx % 16;
	iPalIdx = m_bPalIdx[iIdx];
	clr = m_clr[iIdx];

	return TRUE;
}

void CDMLocPalWnd::SetReadOnly(BOOL fReadOnly)
{
	m_fReadOnly = fReadOnly ? 1 : 0;
}

BOOL CDMLocPalWnd::IsReadOnly()
{
	return m_fReadOnly;
}

void CDMLocPalWnd::PreSubclassWindow() 
{
	CStatic::PreSubclassWindow();

	m_iIdxSwap[0] = m_iIdxSwap[1] = -1;
	m_fReadOnly = TRUE;

	Resized();
}

BOOL CDMLocPalWnd::OnEraseBkgnd(CDC* pDC) 
{
	CRect rc;
	GetClientRect(&rc);
	UINT i;
	pDC->SaveDC();
	if (IsWindowEnabled())
		for (i = 0; i < 16; i++) {
			pDC->ExcludeClipRect(m_rcDrawItem[i]);
		}
	CBrush br;
	br.CreateStockObject(BLACK_BRUSH);
	pDC->FillRect(&rc, &br);
	pDC->RestoreDC(-1);
	return TRUE;
	
	return CStatic::OnEraseBkgnd(pDC);
}

void CDMLocPalWnd::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	Resized();
	
}

void CDMLocPalWnd::Resized()
{
	CRect rc;
	GetClientRect(&rc);

	for (UINT i = 0; i < 16; i++) {
		CRect &rcItem = m_rcDrawItem[i];
		if (i == 0) {
			rcItem.left = 1;
			rcItem.top = 1;
			rcItem.bottom = rc.bottom - 1;
			rcItem.right = rcItem.left + rcItem.Height();
		} else {
			rcItem.left = m_rcDrawItem[i-1].right + 2;
			rcItem.top = 1;
			rcItem.bottom = rc.bottom - 1;
			rcItem.right = rcItem.left + rcItem.Height();
		}
		m_rcItem[i] = rcItem;
		m_rcItem[i].InflateRect(1, 1);
	}
}

void CDMLocPalWnd::OnPaint() 
{
	CPaintDC dc(this);

	if (!IsWindowEnabled())
		return;

	CFont font;
	font.CreateStockObject(DEFAULT_GUI_FONT);
	dc.SaveDC();
	dc.SelectObject(&font);
	dc.SetBkMode(TRANSPARENT);
	CString strText;
	int iSunken = m_iIdxSwap[1];
	UINT iIdx;
	for (iIdx = 0; iIdx < 16; iIdx++) {
		UINT i = iIdx;
		if (i == m_iIdxSwap[0]) i = m_iIdxSwap[1];
		else if (i == m_iIdxSwap[1]) i = m_iIdxSwap[0];

		strText.Format("%02X", 0U + m_bPalIdx[i]);
		COLORREF clr = m_clr[i];
		COLORREF clrText = m_clr[i] ^ 0xFFFFFF;
		CBrush br(clr);
		dc.FillRect(m_rcDrawItem[iIdx], &br);
		dc.DrawEdge(m_rcDrawItem[iIdx], (iSunken == iIdx) ? BDR_SUNKENOUTER : BDR_RAISEDOUTER, BF_RECT);
		dc.SetTextColor(clrText);
		dc.DrawText(strText, m_rcDrawItem[iIdx], DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	}
	dc.RestoreDC(-1);
}

void CDMLocPalWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_fReadOnly) {
		int iItem = HitTestItemRect(point);
		if (iItem >= 0) {
			InitLoop();
			int iPos = TrackItem(iItem);
			if (iPos >= 0) {
				swap(m_bPalIdx[iPos], m_bPalIdx[iItem]);
				swap(m_clr[iPos], m_clr[iItem]);
				Invalidate();
			}
			return;
		}
	}
	
	CStatic::OnLButtonDown(nFlags, point);
}

int CDMLocPalWnd::HitTestItemRect(CPoint point)
{
	UINT i;
	for (i = 0; i < 16; i++) {
		if (m_rcItem[i].PtInRect(point))
			return i;
	}
	return -1;
}

int CDMLocPalWnd::TrackItem(int iOrgPos)
{
	if (::GetCapture() != NULL)
		return -1;

	m_iIdxSwap[0] = m_iIdxSwap[1] = iOrgPos;

	Invalidate();
	UpdateWindow();

	SetCapture();
	ASSERT(this == GetCapture());

	int iCurPos = -1;

	while (CWnd::GetCapture() == this) {
		MSG wm;
		if (!::GetMessage(&wm, NULL, 0, 0)) {
			AfxPostQuitMessage(wm.wParam);
			break;
		}

		CPoint point;
		int iNewPos = iCurPos;

		switch (wm.message) {
		case WM_LBUTTONUP:
			CancelLoop();
			return iNewPos;
		case WM_MOUSEMOVE:
			point.x = GET_X_LPARAM(wm.lParam);
			point.y = GET_Y_LPARAM(wm.lParam);
			iNewPos = HitTestItemRect(point);
			if (iCurPos != iNewPos) {
				if (iNewPos < 0) {
					m_iIdxSwap[0] = m_iIdxSwap[1] = -1;
				} else {
					m_iIdxSwap[0] = iOrgPos;
					m_iIdxSwap[1] = iNewPos;
				}
				iCurPos = iNewPos;
				Invalidate();
				UpdateWindow();
			}
			break;
		case WM_KEYDOWN:
			if (wm.wParam == VK_ESCAPE) {
				CancelLoop();
				return -1;
			}
			break;
		case WM_RBUTTONDOWN:
			CancelLoop();
			return -1;
		}
	}

	CancelLoop();
	return -1;
}

void CDMLocPalWnd::InitLoop()
{
	m_iIdxSwap[0] = m_iIdxSwap[1] = -1;

	SetFocus();

	CWnd *pParentWnd = GetParent();
	if (pParentWnd)
		pParentWnd->UpdateWindow();
}

void CDMLocPalWnd::CancelLoop()
{
	m_iIdxSwap[0] = m_iIdxSwap[1] = -1;

	Invalidate();
	ReleaseCapture();
}

UINT CDMLocPalWnd::OnNcHitTest(CPoint point) 
{
	UINT r = CStatic::OnNcHitTest(point);
	switch (r) {
	case HTTRANSPARENT:
		return HTCLIENT;
	}
	return r;
}
