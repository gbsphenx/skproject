// RuneSelStatic.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "RuneSelStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRuneSelStatic

CRuneSelStatic::CRuneSelStatic()
{
	m_iSel = -1;
}

CRuneSelStatic::~CRuneSelStatic()
{
}


BEGIN_MESSAGE_MAP(CRuneSelStatic, CStatic)
	//{{AFX_MSG_MAP(CRuneSelStatic)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRuneSelStatic メッセージ ハンドラ

int CRuneSelStatic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CRuneSelStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (nFlags & MK_LBUTTON && m_rcBox.PtInRect(point)) {
		int x = (point.x - m_rcBox.left) / 24;
		int y = (point.y - m_rcBox.top) / 24;
		if (0 <= x && x < 6 && 0 <= y && y < 3) {
			SetSel(6*y +x);
			return;
		}
		SetSel(-1);
		return;
	}
	
	CStatic::OnLButtonDown(nFlags, point);
}

void CRuneSelStatic::PreSubclassWindow() 
{
	m_bmRunes.DeleteObject();
	m_bmRunes.LoadBitmap(IDB_RUNES);

	CRect rcOffset(0, 0, 0, 0);
	ClientToScreen(&rcOffset);
	CRect rcFrame;
	GetWindowRect(&rcFrame);
	CRect rcClient;
	GetClientRect(&rcClient);

	m_rcBox = CRect(CPoint(rcOffset.TopLeft() - rcFrame.TopLeft()), rcClient.Size());
	
	CStatic::PreSubclassWindow();
}

void CRuneSelStatic::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	
	CDC dcMem;
	if (dcMem.CreateCompatibleDC(&dc)) {
		CBitmap *pOldBM = dcMem.SelectObject(&m_bmRunes);
		dc.BitBlt(m_rcBox.left, m_rcBox.top, 24*6, 24*3, &dcMem, 0, 0, SRCCOPY);
		dcMem.SelectObject(pOldBM);

		if (m_iSel >= 0 && m_iSel < 18) {
			int x = m_iSel % 6;
			int y = m_iSel / 6;
			dc.InvertRect(CRect(24*x, 24*y, 24*(x+1), 24*(y+1)) + m_rcBox.TopLeft());
		}
	}
}

LRESULT CRuneSelStatic::OnNcHitTest(CPoint point) 
{
	CPoint pt = point;
	ScreenToClient(&pt);

	if (m_rcBox.PtInRect(pt))
		return HTCLIENT;
	
	return CStatic::OnNcHitTest(point);
}
