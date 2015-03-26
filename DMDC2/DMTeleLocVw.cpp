// DMTeleLocVw.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "DMTeleLocVw.h"
#include "CompatDC.h"
#include "BMODID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDMTeleLocVw

BEGIN_MESSAGE_MAP(CDMTeleLocVw, CScrollView)
	//{{AFX_MSG_MAP(CDMTeleLocVw)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMTeleLocVw 描画

void CDMTeleLocVw::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	const UINT cxTile = 24;
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = cxTile*32;
	SetScrollSizes(MM_TEXT, sizeTotal, CSize(cxTile*2, cxTile*2), CSize(cxTile, cxTile));

	m_fNoSel = false;

	ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);

//	ResizeParentToFit(FALSE);

	MarkBuildTS();
}

void CDMTeleLocVw::OnDraw(CDC* pDC)
{
	if (m_fBuildTS) {
		BuildTS();
		m_fBuildTS = false;
	}

	CRect rc;
	GetClientRect(&rc);
	pDC->SaveDC();

	const UINT cxTile = 24;

	CCompatDC dcMem(pDC);

	dcMem.SaveDC();

	COLORREF clr = RGB(127,127,127);

	CRect rcFrame;

	CBMO *pbmoHatch1 = GetApp()->FindBMO(BMODID_HATCH1 * 4);
	CBMO *pbmoHatch2 = GetApp()->FindBMO(BMODID_HATCH2 * 4);

	BYTE x, y, cx = m_size.cx, cy = m_size.cy;
	for (y = 0; y < cy; y++) {
		for (x = 0; x < cx; x++) {
			CRect rcTile;
			rcTile.right = (rcTile.left = cxTile * y) + cxTile;
			rcTile.bottom = (rcTile.top = cxTile * x) + cxTile;

			TileSt &ts = m_ts[y][x];
			BYTE iTile = ts.iTile;
			BOOL fClip = 0;
			CBMO *pbmo = GetApp()->FindBMO((BMODID_TILE0 + iTile) * 4);
			if (pbmo != NULL && iTile != -1) {
				int x1 = pbmo->cx / 24;
				int y1 = pbmo->cy / 24;
				int ox = 24 * (x % x1);
				int oy = 24 * (y % y1);
				StretchDIBits(*pDC, rcTile.left, rcTile.top, cxTile, cxTile, ox, oy, 24, 24, pbmo->pv, pbmo->pbmi, DIB_RGB_COLORS, SRCCOPY);
				fClip = 1;
			}
			if (ts.iInv) {
				if (!fClip) pDC->FillSolidRect(&rcTile, clr);

				if (ts.iInv == 1 && pbmoHatch1 != NULL && pbmoHatch2 != NULL) {
					StretchDIBits(*pDC, rcTile.left, rcTile.top, cxTile, cxTile, 0, 0, 24, 24, pbmoHatch2->pv, pbmoHatch2->pbmi, DIB_RGB_COLORS, SRCAND);
					StretchDIBits(*pDC, rcTile.left, rcTile.top, cxTile, cxTile, 0, 0, 24, 24, pbmoHatch1->pv, pbmoHatch1->pbmi, DIB_RGB_COLORS, SRCPAINT);
				}

				if (ts.iInv == 2) {
					rcFrame = rcTile;
					rcFrame.InflateRect(2, 2);
				}

				fClip = 1;
			}
			if (fClip) {
				pDC->ExcludeClipRect(&rcTile);
			}
		}
	}

	dcMem.RestoreDC(-1);

	CSize size = GetTotalSize();
	CSize sizeBox = rc.Size();

	size.cx = _MAX(sizeBox.cx, size.cx);
	size.cy = _MAX(sizeBox.cy, size.cy);

	rc = CRect(CPoint(0, 0), size);

	pDC->FillSolidRect(rc, clr);
	pDC->RestoreDC(-1);

	CBrush br;
	br.CreateStockObject(WHITE_BRUSH);

	pDC->FrameRect(&rcFrame, &br);
	rcFrame.InflateRect(2, 2);
	pDC->FrameRect(&rcFrame, &br);
}

/////////////////////////////////////////////////////////////////////////////
// CDMTeleLocVw 診断

#ifdef _DEBUG
void CDMTeleLocVw::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDMTeleLocVw::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDMTeleLocVw メッセージ ハンドラ

void CDMTeleLocVw::PostNcDestroy() 
{

}

BOOL CDMTeleLocVw::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CScrollView::PreCreateWindow(cs)) return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return TRUE;
}

BOOL CDMTeleLocVw::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
//	return CScrollView::OnEraseBkgnd(pDC);
}

void CDMTeleLocVw::BuildTS()
{
	BYTE x, y;

	for (y = 0; y < 32; y++)
		for (x = 0; x < 32; x++)
			m_ts[y][x] = TileSt();

	m_size.cx = m_size.cy = 0;

	if (!MkDDIf()) return;

	Atlas_t at;
	HRESULT hr;
	if (FAILED(hr = m_pDDIf->GetMapAtlasAt(m_ap.Z(), at))) return;
	LevelDesc_t s;
	if (FAILED(hr = m_pDDIf->GetMapDescAt(m_ap.Z(), s))) return;
	m_size.cx = s.cxLv;
	m_size.cy = s.cyLv;

	BYTE fx = m_ap.Y(), fy = m_ap.X();

	for (y = 0; y < 32; y++) {
		for (x = 0; x < 32; x++) {
			TileSt &ts = m_ts[y][x];
			BYTE c = at.c[y][x];
			switch (c / 0x20) {
			case 0: // WALL
				ts.iTile = 1;
				break;
			case 1: // FLOOR
			case 3: // STAIRCASE
			case 4: // DOOR
				ts.iTile = 0;
				break;
			case 2: // PITFALL
				ts.iTile = 2;
				break;
				ts.iTile = 0;
			case 5: // TELE
				ts.iTile = 5;
				break;
			case 6: // FALSE
				ts.iTile = 6;
				break;
			case 7: // ?
				break;
			}
			if (!m_fNoSel) {
				ts.iInv = ((y == fx) ? 1 : 0) + ((x == fy) ? 1 : 0);
			}
		}
	}
}

void CDMTeleLocVw::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_RETURN) {
		CWnd *p = GetParent();
		if (p)
			p->EndModalLoop(IDOK);
		return;
	}

	int iDelta = 0;
	switch (nChar) {
	case VK_PRIOR: iDelta = -1; break;
	case VK_NEXT: iDelta = +1; break;
	}

	if (iDelta != 0 && m_fMovable && MkDDIf()) {
		UINT nZ = m_pDDIf->CntMap();
		UINT iZ = m_ap.Z() + iDelta;
		if (iZ < nZ) {
			m_ap = AtlasPos_t(iZ, 0, 0);
		}
		m_fNoSel = true;

		MarkBuildTS();
		Invalidate();
		return;
	}

	int iX = 0, iY = 0;
	switch (nChar) {
	case VK_LEFT: iY = -1; break;
	case VK_RIGHT: iY = 1; break;
	case VK_UP: iX = -1; break;
	case VK_DOWN: iX = 1; break;
	}
	if (iX || iY) {
		iX += m_ap.X();
		iY += m_ap.Y();
		iX = _std_min<int>(+m_size.cx-1, _std_max<int>(0, +iX));
		iY = _std_min<int>(+m_size.cy-1, _std_max<int>(0, +iY));
		m_ap = AtlasPos_t(m_ap.Z(), iX, iY);

		m_fNoSel = false;
		MarkBuildTS();
		Invalidate();
		return;
	}
	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDMTeleLocVw::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (Clicked(point)) return;
	
	CScrollView::OnLButtonDown(nFlags, point);
}

bool CDMTeleLocVw::Clicked(CPoint pt, bool fReally)
{
	pt += GetDeviceScrollPosition();
	UINT x = pt.y / 24;
	UINT y = pt.x / 24;
	if (x < m_size.cx && y < m_size.cy) {
		if (fReally) {
			m_ap = AtlasPos_t(m_ap.Z(), x, y);
			m_fNoSel = false;

			MarkBuildTS();
			Invalidate();
		}
		return true;
	}
	return false;
}

void CDMTeleLocVw::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (Clicked(point, false)) {
		CWnd *p = GetParent();
		if (p)
			p->EndModalLoop(IDOK);
		return;
	}
	
	CScrollView::OnLButtonDblClk(nFlags, point);
}
