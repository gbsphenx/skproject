// DM2SelColorKeyStatic.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2SelColorKeyStatic.h"
#include "DM2Palo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2SelColorKeyStatic

CDM2SelColorKeyStatic::CDM2SelColorKeyStatic()
{
	m_iSel = -1;
	m_iOffset = 0;
}

BEGIN_MESSAGE_MAP(CDM2SelColorKeyStatic, CStatic)
	//{{AFX_MSG_MAP(CDM2SelColorKeyStatic)
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2SelColorKeyStatic メッセージ ハンドラ

void CDM2SelColorKeyStatic::Set4(const DMGHLciPAL &r, int iColorSrcKey, const DMGHLciDM2Pal &DM2Pal)
{
	m_solid.RemoveAll();

	for (int i = 0; i < 16; i++) {
		Solid a;

		a.clr = r.p.f.color[i];
		a.f = (i == iColorSrcKey) ? 1 : 0;

		if (i == iColorSrcKey) m_iSel = i;

		m_solid.Add(a);
	}
}

void CDM2SelColorKeyStatic::Set8(int iColorSrcKey, const DMGHLciDM2Pal &DM2Pal)
{
	m_solid.RemoveAll();

	for (int i = 0; i < 256; i++) {
		Solid a;

		a.clr = DM2Pal.clrs[i];
		a.f = (i == iColorSrcKey) ? 1 : 0;

		if (i == iColorSrcKey) m_iSel = i;

		m_solid.Add(a);
	}
}

void CDM2SelColorKeyStatic::Init()
{
	CRect rc;
	GetClientRect(&rc);

	CRect rc1 = rc;

	rc1.right = rc.bottom - rc.top + rc.left;

	int nBox = rc.Width() / (rc1.Height() -1);
	int nSolid = m_solid.GetSize();

	bool fAdd = nBox < nSolid;

	int iSolid = 0;

	m_box.RemoveAll();

	m_nSolidVis = 0;

	for (int i = 0; i < nBox; i++) {
		Box a;
		a.rc = rc1;
		a.bk = bkNo;

		if (fAdd) {
			if (i == 0) {
				a.bk = bkLL;
			} else if (i == nBox - 1) {
				a.bk = bkRR;
			}
		}

		if (a.bk == bkNo) {
			m_nSolidVis++;

			if (m_iOffset + iSolid < nSolid) {
				a.bk = bkSolid;
				a.iSolid = m_iOffset + iSolid;

				iSolid++;
			}
		}

		m_box.Add(a);

		rc1.OffsetRect(rc1.Width() -1, 0);
	}
}

LRESULT CDM2SelColorKeyStatic::OnNcHitTest(CPoint point) 
{
	UINT x = CStatic::OnNcHitTest(point);
	if (x == HTTRANSPARENT)
		x = HTCLIENT;

	return x;
}

void CDM2SelColorKeyStatic::OnPaint() 
{
	CPaintDC dc(this);

	if (m_fontMarlett.m_hObject == NULL) {
		m_fontMarlett.CreatePointFont(120, _T("Marlett"), &dc);
	}

	{
		CRect rc;
		GetClientRect(&rc);

		dc.FillSolidRect(&rc, RGB(0, 0, 0));
	}

	CBrush br;
	br.CreateStockObject(WHITE_BRUSH);

	CFont *pOldFont = dc.SelectObject(&m_fontMarlett);

	int nBox = m_box.GetSize();

	dc.SelectStockObject(DEFAULT_GUI_FONT);

	for (int i = 0; i < nBox; i++) {
		Box &r = m_box[i];

		if (r.rc.Width() == 0) continue;

		CRect rc = r.rc;

		rc.DeflateRect(1, 1);

		switch (r.bk) {
		case bkLL:
		case bkRR:
			{
				dc.FrameRect(&rc, &br);

				rc.DeflateRect(2, 2);

				LPCTSTR psz = (r.bk == bkLL) ? "<" : ">";
				CPoint pt = rc.CenterPoint();
				dc.SetBkMode(TRANSPARENT);
				dc.SetTextColor(RGB(255, 255, 255));
				rc.OffsetRect(-2, 0); dc.DrawText(psz, &rc, 0 |DT_SINGLELINE |DT_VCENTER |DT_CENTER);
				rc.OffsetRect(+4, 0); dc.DrawText(psz, &rc, 0 |DT_SINGLELINE |DT_VCENTER |DT_CENTER);

				dc.SetBkMode(OPAQUE);
				break;
			}
		case bkSolid:
			{
				Solid &s = m_solid[r.iSolid];

				if (s.f & 1) dc.FrameRect(&rc, &br);

				rc.DeflateRect(2, 2);

				dc.FillSolidRect(&rc, s.clr);

				if (r.iSolid == m_iSel) {
					int x = dc.GetROP2();
					dc.SetROP2(R2_NOT);
					dc.MoveTo(rc.left, rc.top);
					dc.LineTo(rc.right, rc.bottom);
					dc.MoveTo(rc.right -1, rc.top);
					dc.LineTo(rc.left -1, rc.bottom);
					dc.SetROP2(x);
				}
				break;
			}
		}
	}

	dc.SelectObject(pOldFont);
}

void CDM2SelColorKeyStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (Clicked(point)) return;
	
	CStatic::OnLButtonDown(nFlags, point);
}

void CDM2SelColorKeyStatic::SetSel(int i, bool fNotify)
{
	if (m_iSel == i) return;

	m_iSel = i;

	Invalidate();

	if (fNotify) {
		ASSERT(GetParent() != NULL);

		GetParent()->PostMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), 0), (LPARAM)(HWND)*this);
	}
}

void CDM2SelColorKeyStatic::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (Clicked(point)) return;
	
	CStatic::OnLButtonDblClk(nFlags, point);
}

bool CDM2SelColorKeyStatic::Clicked(CPoint point)
{
	int nBox = m_box.GetSize();
	for (int i = 0; i < nBox; i++) {
		const Box &r = m_box[i];
		if (r.rc.PtInRect(point)) {
			int iOffset = m_iOffset;
			switch (r.bk) {
			case bkLL:
				m_iOffset = _MAX(0, m_iOffset -m_nSolidVis);
				break;
			case bkRR:
				m_iOffset = _MAX(0, _MIN(m_solid.GetSize() -m_nSolidVis, m_iOffset +m_nSolidVis));
				break;
			case bkSolid:
				SetSel(r.iSolid, true);
				break;
			}
			if (iOffset != m_iOffset)
				Init();
			Invalidate();
			return true;
		}
	}
	return false;
}
