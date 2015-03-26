// Exp00069View.cpp : CExp00069View クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "Exp00069.h"

#include "Exp00069Doc.h"
#include "Exp00069View.h"
#include "mainfrm.h"
#include "EditLvDescDlg.h"
#include <math.h>
#include "EditDMFileDescDlg.h"
#include "DMObTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MCF_LDRAG	1LU

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CExp00069View

IMPLEMENT_DYNCREATE(CExp00069View, CView)

BEGIN_MESSAGE_MAP(CExp00069View, CView)
	//{{AFX_MSG_MAP(CExp00069View)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_BTN_B10, OnBtnB10)
	ON_UPDATE_COMMAND_UI(ID_BTN_B10, OnUpdateBtnB10)
	ON_COMMAND(ID_EDIT_LVDESC, OnEditLvdesc)
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_FORCE_HEX_TILE, OnViewForceHexTile)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FORCE_HEX_TILE, OnUpdateViewForceHexTile)
	ON_COMMAND(ID_EDIT_DMFILEDESC, OnEditDmfiledesc)
	ON_COMMAND(ID_EDIT_REPAIR_5THBIT, OnEditRepair5thbit)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDIT_OPTIMIZE, OnEditOptimize)
	ON_COMMAND(ID_EDIT_CHECK_INTEGRITY, OnEditCheckIntegrity)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_VIEW_SIZE1X, ID_VIEW_SIZE4X, OnViewSize1x)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_SIZE1X, ID_VIEW_SIZE4X, OnUpdateViewSize1x)
	ON_COMMAND_RANGE(ID_BTN_B0, ID_BTN_B3, OnBtnB0)
	ON_UPDATE_COMMAND_UI_RANGE(ID_BTN_B0, ID_BTN_B3, OnUpdateBtnB0)
	ON_COMMAND(ID_EDIT_BUILDLABYRINTH, OnEditBuildlabyrinth)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExp00069View クラスの構築/消滅

CExp00069View::CExp00069View()
{
	m_iLv = 0;
	m_pEdCtx = NULL;
}

CExp00069View::~CExp00069View()
{
	ASSERT(m_pEdCtx == NULL);
}

BOOL CExp00069View::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_HSCROLL|WS_VSCROLL;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CExp00069View クラスの描画

void CExp00069View::OnDraw(CDC* dc)
{
	HGDIOBJ hOldFont = dc->SelectObject(m_fontNice.m_hObject);
	HGDIOBJ hOldPen = dc->SelectObject(GetStockObject(BLACK_PEN));
	HGDIOBJ hOldBrush = dc->SelectObject(GetStockObject(BLACK_BRUSH));
	TRY
		DoDraw(*dc);
	CATCH_ALL(e)
	END_CATCH_ALL
	dc->SelectObject(hOldBrush);
	dc->SelectObject(hOldPen);
	dc->SelectObject(hOldFont);
}

void CExp00069View::DoDraw(CDC &dc)
{
	{
		CExp00069Doc* doc = GetDocument();
		ASSERT_VALID(doc);

		CDC dcAn;
		dcAn.CreateCompatibleDC(&dc);
		//HGDIOBJ hOldBitmap = dcAn.SelectObject(NULL);

		DIBits_RealizePalette(dc, m_bmTile[m_iTile], FALSE);

		if (GetDDAT().CntMap() < 1)
			goto SolidFill;

		Atlas_t m;
		HRESULT hr;
		LevelDesc_t d;
		if (FAILED(hr = GetDDAT().GetMapAtlasAt(m_iLv, m)))
			goto SolidFill;
		if (FAILED(hr = GetDDAT().GetMapDescAt(m_iLv, d)))
			goto SolidFill;
		CSize sizeTile = CalcSizeCell();
		CPen aPenJrid(PS_SOLID, 0, (COLORREF)0xffff00);
		dc.SelectObject(aPenJrid.m_hObject);
		dc.SetBkColor((COLORREF)0x000000);
		int jcx = d.cyLv;
		int jcy = d.cxLv;
		int bcx = sizeTile.cx;
		int bcy = sizeTile.cy;

		dc.SetBkMode(TRANSPARENT);
		CString tstrHell;
		for (int vy=m_rcVp.top; vy<jcy; vy++) {
			if (vy < 0 || 32 <= vy) continue;
			for (int vx=m_rcVp.left; vx<jcx; vx++) {
				if (vx < 0 || 32 <= vx) continue;
				int nx = __max(0, __min(jcx-1, m_rcVp.left));
				int ny = __max(0, __min(jcy-1, m_rcVp.top));
				CRect rcItem(
					bcx*(vx-nx+0)+0,
					bcy*(vy-ny+0)+0,
					bcx*(vx-nx+1)+0,
					bcy*(vy-ny+1)+0);

	//				CString tstr;
	//				tstr.Format("%02X", (UINT)m.c[vx][vy]);
	//				dc.Rectangle(rcItem);
	//				dc.SetTextColor((COLORREF)0xffffff);
	//				dc.TextOut(bcx*vx, bcy*vy, tstr);
				BYTE c = m.c[vx][vy];
				BYTE cl = (vx ==      0 ) ? 0x00 : m.c[vx-1][vy];
				BYTE cr = (vx == (jcx-1)) ? 0x00 : m.c[vx+1][vy];

				UINT ix = c & 0xF0;
				UINT iy = ((cl & 0xE0) == 0 && (cr & 0xE0) == 0) ? 0x10 : 0x00;
				//dc.BitBlt(rcItem.left, rcItem.top, 16, 16, &dcAn, ix, iy, SRCCOPY);
				CDIBits &bm = m_bmTile[m_iTile];
				int tx = ix/16*bcx;
				int ty = iy/16*bcy;
				if (bm.IsLoaded() && !m_bForceHexTile) {
					bm.Draw(
						dc, 
						rcItem, 
						CRect(tx, ty, tx+bcx, ty+bcy));
				} else {
					COLORREF clrFore = (COLORREF)0xffffff;
					COLORREF clrBack = (COLORREF)0x000000;
					switch (c & 0xF0) {
					case 0x00:	clrFore = 0x808080; clrBack = 0x000000; break;
					case 0x10:	clrFore = 0x808080; clrBack = 0x000000; break;

					case 0x20:	clrFore = 0x808080; clrBack = 0x800000; break;
					case 0x30:	clrFore = 0x808080; clrBack = 0x800000; break;

					case 0x40:	clrFore = 0x0000ff; clrBack = 0x000000; break;
					case 0x50:	clrFore = 0x0000ff; clrBack = 0x000000; break;

					case 0x60:	clrFore = 0xffff00; clrBack = 0x800000; break;
					case 0x70:	clrFore = 0xffff00; clrBack = 0x800000; break;

					case 0x80:	clrFore = 0x00ffff; clrBack = 0x800000; break;
					case 0x90:	clrFore = 0x00ffff; clrBack = 0x800000; break;

					case 0xA0:	clrFore = 0x808080; clrBack = 0x000000; break;
					case 0xB0:	clrFore = 0x808080; clrBack = 0x000000; break;

					case 0xC0:	clrFore = 0x808080; clrBack = 0x000080; break;
					case 0xD0:	clrFore = 0x808080; clrBack = 0x000080; break;

					case 0xE0:	clrFore = 0x800000; clrBack = 0x000000; break;
					case 0xF0:	clrFore = 0x800000; clrBack = 0x000000; break;
					}
					//dc.SetBkColor((COLORREF)clrBack);
					dc.SetTextColor((COLORREF)clrFore);
					tstrHell.Format("%02X", (UINT)c);
					dc.FillSolidRect(
						rcItem,
						clrBack);
					dc.DrawText(
						tstrHell,
						rcItem,
						DT_CENTER|DT_VCENTER|DT_SINGLELINE
						);
				}

				dc.ExcludeClipRect(rcItem);
			}
		}
	}

SolidFill:
	CRect rcFill;
	GetClientRect(&rcFill);
	dc.FillSolidRect(rcFill, (COLORREF)0x000000);

	//dcAn.SelectObject(hOldBitmap);
}

/////////////////////////////////////////////////////////////////////////////
// CExp00069View クラスの診断

#ifdef _DEBUG
void CExp00069View::AssertValid() const
{
	CView::AssertValid();
}

void CExp00069View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CExp00069Doc* CExp00069View::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CExp00069Doc)));
	return (CExp00069Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CExp00069View クラスのメッセージ ハンドラ

BOOL CExp00069View::OnEraseBkgnd(CDC* dc)
{
	return TRUE;

	CRect rc;
	GetClientRect(&rc);
	dc->FillSolidRect(&rc, (COLORREF)0);
	return TRUE;

	return CView::OnEraseBkgnd(dc);
}

void CExp00069View::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	if (m_pEdCtx)
		m_pEdCtx->RemoveCB(this),
		m_pEdCtx->Release();
	if (m_pEdCtx = GetDocument()->GetEdCtx(TRUE))
		m_pEdCtx->DispatchDMUpdate(DPM_INITVIEW, NULL);
		m_pEdCtx->AddCB(this);

	LOGFONT lfNice;
//	ZeroMemory(&lfNice, sizeof(lfNice));
//	lfNice.lfHeight = 16;
//	lfNice.lfCharSet = SHIFTJIS_CHARSET;
//	lfNice.lfPitchAndFamily = FIXED_PITCH;
//	_tcscpy(lfNice.lfFaceName, "Courier New");
	ReadFontTemplateFrom(lfNice, IDS_FONTTEMP_VIEW, "Courier New;16;;;;;;;;;;;;1");
	m_fontNice.DeleteObject();
	VERIFY(m_fontNice.CreateFontIndirect(&lfNice));
	m_iTile = 0;
	(m_bmTile[0].Load(CExp00069App::GetNeighborFile("CELL1X.DIB")));
	(m_bmTile[1].Load(CExp00069App::GetNeighborFile("CELL2X.DIB")));
	(m_bmTile[2].Load(CExp00069App::GetNeighborFile("CELL4X.DIB")));

	CMainFrameIndir *pIndir;
	if (pIndir = dynamic_cast<CMainFrameIndir *>(GetParentFrame())) {
		pIndir->SetEditLvTool(0);
	}

	m_iLv = 0;
	m_sizeAtlas.cx = m_sizeAtlas.cy = 0;
	m_nMaskCapture = 0;
	m_rcVp.SetRect(0, 0, 0, 0);
	m_bSelTile = FALSE;
	m_bForceHexTile = FALSE;

	OnUpdate(NULL, DPM_RELOAD, NULL);
}

void CExp00069View::OnDMUpdate(UINT nHint, void *pHint)
{
	switch (nHint) {
	case DPM_RELOAD:
	{
		m_bSelTile = FALSE;
		Invalidate();
		CMainFrameIndir *pIndir;
		if (pIndir = dynamic_cast<CMainFrameIndir *>(GetParentFrame())) {
			pIndir->SetDMLv(0);
			pIndir->SetLvCnt(GetDDAT().CntMap());
		}
		SelectedNewMap(); RecalcScroll();
		break;
	}
	case DPM_SELECTDMLV:
	case DPM_DELETED_MAP:
	case DPM_INSERTED_MAP:
	{
		m_bSelTile = FALSE;
		CMainFrameIndir *pIndir;
		if (pIndir = dynamic_cast<CMainFrameIndir *>(GetParentFrame())) {
			UINT nLvs = GetDDAT().CntMap();
			LevelDesc_t d;
			HRESULT hr;
			if (nLvs <= m_iLv || FAILED(hr = GetDDAT().GetMapDescAt(m_iLv, d))) {
				pIndir->SetLvSize(CSize(0, 0));

				m_iLv = 0;
			} else {
				pIndir->SetLvSize(CSize(d.cyLv, d.cxLv));

				m_iLv = __min(nLvs - 1, pIndir->GetDMLv());
			}
			if (nHint != DPM_SELECTDMLV)
				pIndir->SetLvCnt(nLvs);
			Invalidate();
		}
		SelectedNewMap(); RecalcScroll();
		break;
	}
	case DPM_CHANGED_ATLASBIT:
	{
		const AtlasPos_t &pos = *(const AtlasPos_t *)pHint;
		int px = pos.Y();
		int py = pos.X();
		RedrawMapBit(px  , py  );
		break;
	}
	case DPM_CHANGED_LEVELDESC:
	{
		CMainFrameIndir *pIndir;
		if (pIndir = dynamic_cast<CMainFrameIndir *>(GetParentFrame())) {
			m_iLv = pIndir->GetDMLv();
			LevelDesc_t d;
			HRESULT hr;
			VERIFY(SUCCEEDED(hr = GetDDAT().GetMapDescAt(m_iLv, d)));
			pIndir->SetLvSize(CSize(d.cyLv, d.cxLv));
			Invalidate();
		}
	}
	case DPM_CHANGED_ATLAS:
	{
		m_bSelTile = FALSE;

		const ChangeLevelDesc_t &info = *(const ChangeLevelDesc_t *)pHint;
		if (m_iLv == info.iMapAt) {
			SelectedNewMap(); RecalcScroll(); Invalidate();
		}
		break;
	}
	case DPM_SELECTBTLVXY:
	{
		SelectLvXY_t *what = reinterpret_cast<SelectLvXY_t *>(pHint);
		m_aSelPos = what->pos;

		m_bSelTile = TRUE;
		CMainFrameIndir *pIndir;
		if (pIndir = dynamic_cast<CMainFrameIndir *>(GetParentFrame())) {
			pIndir->SetDMLv(m_iLv = m_aSelPos.Z());
			LevelDesc_t d;
			HRESULT hr;
			VERIFY(SUCCEEDED(hr = GetDDAT().GetMapDescAt(m_iLv, d)));
			pIndir->SetLvSize(CSize(d.cyLv, d.cxLv));
			Invalidate();

			m_rcVp -= m_rcVp.TopLeft();
			m_rcVp += CPoint(
				m_aSelPos.Y() - m_rcVp.Width() /2,
				m_aSelPos.X() - m_rcVp.Height()/2);
		}
		SelectedNewMap(); RecalcScroll();

		const nWidth = 60, nMaxWidth = 100;
		m_hlat.cpTar = CalcRectForAtlasPos(m_aSelPos).CenterPoint();
		m_hlat.rcTar = CRect(m_hlat.cpTar.x - nWidth, m_hlat.cpTar.y - nWidth, m_hlat.cpTar.x + nWidth, m_hlat.cpTar.y + nWidth);
		m_hlat.rcInvalid = CRect(m_hlat.cpTar.x - nMaxWidth, m_hlat.cpTar.y - nMaxWidth, m_hlat.cpTar.x + nMaxWidth+1, m_hlat.cpTar.y + nMaxWidth+1);
		m_hlat.nTickStart = GetTickCount();
		m_hlat.bDisable = FALSE;
		SetTimer(0x0101, 50, NULL);
		break;
	}
	}
}

void CExp00069View::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	OnDMUpdate(lHint, pHint);
}

void CExp00069View::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_sizeAtlas.cx != 0 && m_sizeAtlas.cy != 0) {
		CSize size(m_sizeAtlas.cy, m_sizeAtlas.cx);
		CPoint xy = CalcBitPtFrom(point);
		int px = xy.x;
		int py = xy.y;
		if (px >= 0 && px < size.cx && py >= 0 && py < size.cy) {
			if (nFlags & MK_LBUTTON) {
				if (m_nMaskCapture & MCF_LDRAG) {
					if (DoSetMapBit(px, py)) {

					}
				}
			} else if (nFlags & MK_RBUTTON) {

			} else if (nFlags & MK_MBUTTON) {

			} else {
				CString tstr;
				tstr.Format(
					"Lv:%2u (%2u,%2u)", 
					(UINT)m_iLv, 
					(UINT)py, 
					(UINT)px);
				UINT nItems = GetDDAT().CntAtlasAssocAt(AtlasPos_t(m_iLv, py, px));
				if (nItems != (UINT)-1) {
					CString tstrAdd;
					tstrAdd.Format(" Objs:%2u", nItems);
					tstr += tstrAdd;
				}

				CFrameWnd *wnd;
				if (wnd = GetParentFrame()) {
					wnd->SetMessageText(tstr);
				}
			}
			return;
		}
	}
	
	CView::OnMouseMove(nFlags, point);
}

void CExp00069View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	DisableHlat();

	if (m_sizeAtlas.cx != 0 && m_sizeAtlas.cy != 0) {
		CSize size(m_sizeAtlas.cy, m_sizeAtlas.cx);
		CPoint xy = CalcBitPtFrom(point);
		int px = xy.x;
		int py = xy.y;
		if (px >= 0 && px < size.cx && py >= 0 && py < size.cy) {
			if (DoSetMapBit(px, py)) {
				m_nMaskCapture |= MCF_LDRAG;
				SetCapture();
			}
			return;
		}
	}
	
	CView::OnLButtonDown(nFlags, point);
}

void CExp00069View::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_nMaskCapture & MCF_LDRAG) {
		ReleaseCapture();
		m_nMaskCapture&=~MCF_LDRAG;
	}
	
	CView::OnLButtonUp(nFlags, point);
}

BOOL CExp00069View::DoSetMapBit(UINT px, UINT py)
{
	CMainFrameIndir *pIndir;
	HRESULT hr;
	if (pIndir = dynamic_cast<CMainFrameIndir *>(GetParentFrame())) {
		UINT iTool = pIndir->GetEditLvTool();
		if (iTool == 0) {
			SelectLvXY_t what;
			what.pos = AtlasPos_t(m_iLv, py, px);
			GetDocument()->GetEdCtx(FALSE)->DispatchDMUpdate(DPM_SELECTLVXY, (void *)&what);
			m_bSelTile = TRUE;
			m_aSelPos = what.pos;
		} else if (1 <= iTool && iTool <= 8) {
			m_nMaskCapture |= MCF_LDRAG;
			AtlasPos_t pos(m_iLv, py, px);
			AtlasBit_t b;
			if (SUCCEEDED(GetDDAT().GetAtlasBitAt(pos, b))) {
				b.c = 0x20 * (iTool-1) | (b.c & 0x10);
				if (SUCCEEDED(hr = GetDDAT().SetAtlasBitAt(pos, b))) {
					m_bSelTile = FALSE;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CExp00069View::OnQueryNewPalette() 
{
	CClientDC dc(this);
	DIBits_RealizePalette(dc, m_bmTile[m_iTile], FALSE);

	return CView::OnQueryNewPalette();
}

void CExp00069View::OnViewSize1x(UINT nID)
{
	UINT i = m_iTile;
	switch (nID) {
	case ID_VIEW_SIZE1X: i = 0; break;
	case ID_VIEW_SIZE2X: i = 1; break;
	case ID_VIEW_SIZE4X: i = 2; break;
	}
	if (m_iTile != i) {
		m_iTile = i;
		RecalcScroll();
		Invalidate();
	}
}

void CExp00069View::OnUpdateViewSize1x(CCmdUI* pCmdUI) 
{
	UINT i = -1;
	switch (pCmdUI->m_nID) {
	case ID_VIEW_SIZE1X: i = 0; break;
	case ID_VIEW_SIZE2X: i = 1; break;
	case ID_VIEW_SIZE4X: i = 2; break;
	}
	pCmdUI->SetRadio(m_iTile == i);
}

void CExp00069View::RecalcScroll()
{
	CRect rc;
	GetClientRect(&rc);
	CSize size = CalcSizeCell();
	int cx = rc.right / size.cx;
	int cy = rc.bottom/ size.cy;
	cx = __max(1, cx);
	cy = __max(1, cy);
	m_rcVp.right = m_rcVp.left+ cx;
	m_rcVp.bottom= m_rcVp.top + cy;

	ScrollUpdate();

	DisableHlat();
}

void CExp00069View::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int vx = m_rcVp.left, vy = m_rcVp.top;
	switch (nSBCode) {
	case SB_LEFT:			vx = m_rcVpRange.left; break;
	case SB_PAGELEFT:		vx-= m_rcVp.Width(); break;
	case SB_LINELEFT:		vx-= 1; break;
	case SB_LINERIGHT:		vx+= 1; break;
	case SB_PAGERIGHT:		vx+= m_rcVp.Width(); break;
	case SB_RIGHT:			vx = m_rcVpRange.right; break;
	case SB_THUMBTRACK:		vx = nPos; break;
	}
	ScrollSetPos(vx, vy);

	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CExp00069View::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int vx = m_rcVp.left, vy = m_rcVp.top;
	switch (nSBCode) {
	case SB_LEFT:			vy = m_rcVpRange.top; break;
	case SB_PAGELEFT:		vy-= m_rcVp.Height(); break;
	case SB_LINELEFT:		vy-= 1; break;
	case SB_LINERIGHT:		vy+= 1; break;
	case SB_PAGERIGHT:		vy+= m_rcVp.Height(); break;
	case SB_RIGHT:			vy = m_rcVpRange.bottom; break;
	case SB_THUMBTRACK:		vy = nPos; break;
	}
	ScrollSetPos(vx, vy);
	
	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CExp00069View::ScrollSetPos(int x, int y)
{
	x = __min(x, m_rcVpRange.right - m_rcVp.Width());
	x = __max(x, m_rcVpRange.left);
	y = __min(y, m_rcVpRange.bottom - m_rcVp.Height());
	y = __max(y, m_rcVpRange.top);

	SetScrollPos(SB_HORZ, x);
	SetScrollPos(SB_VERT, y);

	m_rcVp = m_rcVp - m_rcVp.TopLeft() + CPoint(x, y);

	Invalidate();

	DisableHlat();
}

void CExp00069View::ScrollUpdate()
{
	SCROLLINFO SI;
	SI.cbSize = sizeof(SI);
	SI.fMask = SIF_PAGE|SIF_RANGE|SIF_POS;

	SI.nMin = m_rcVpRange.left;
	SI.nMax = m_rcVpRange.right;
	SI.nPage = m_rcVp.Width();
	SI.nPos = m_rcVp.left;
	SI.nPos = __min((int)SI.nPos, (int)SI.nMax - (int)SI.nPage);
	SI.nPos = __max((int)SI.nPos, (int)SI.nMin);
	SI.nMax--;
	SetScrollInfo(SB_HORZ, &SI);
	m_rcVp.right -= m_rcVp.left;
	m_rcVp.left = SI.nPos;
	m_rcVp.right += SI.nPos;

	SI.nMin = m_rcVpRange.top;
	SI.nMax = m_rcVpRange.bottom;
	SI.nPage = m_rcVp.Height();
	SI.nPos = m_rcVp.top;
	SI.nPos = __min((int)SI.nPos, (int)SI.nMax - (int)SI.nPage);
	SI.nPos = __max((int)SI.nPos, (int)SI.nMin);
	SI.nMax--;
	SetScrollInfo(SB_VERT, &SI);
	m_rcVp.bottom -= m_rcVp.top;
	m_rcVp.top = SI.nPos;
	m_rcVp.bottom += SI.nPos;
}

void CExp00069View::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	RecalcScroll();
	ScrollUpdate();

	Invalidate();
}

void CExp00069View::OnContextMenu(CWnd* pWnd, CPoint point) 
{

}

void CExp00069View::OnBtnB0(UINT nID) 
{
	if (m_bSelTile) {
		AtlasBit_t o;
		HRESULT hr;
		if (FAILED(hr = GetDDAT().GetAtlasBitAt(m_aSelPos, o)))
			return;
		UINT i = nID - ID_BTN_B0;
		o.c ^= 1 << i;
		if (FAILED(hr = GetDDAT().SetAtlasBitAt(m_aSelPos, o)))
			return;
	}
}

void CExp00069View::OnUpdateBtnB0(CCmdUI* pCmdUI) 
{
	if (m_bSelTile) {
		AtlasBit_t o;
		HRESULT hr;
		if (FAILED(hr = GetDDAT().GetAtlasBitAt(m_aSelPos, o)))
			return;
		UINT i = pCmdUI->m_nID - ID_BTN_B0;
		pCmdUI->SetCheck(((o.c & 0x0F) & (1 << i)) ? 1 : 0);
		pCmdUI->Enable(TRUE);
		return;
	}
	pCmdUI->SetCheck(0);
	pCmdUI->Enable(FALSE);
}

void CExp00069View::OnBtnB10() 
{
	if (m_bSelTile) {
		AtlasBit_t o;
		HRESULT hr;
		if (FAILED(hr = GetDDAT().GetAtlasBitAt(m_aSelPos, o)))
			return;
		o.c ^= 0x10;
		if (FAILED(hr = GetDDAT().SetAtlasBitAt(m_aSelPos, o)))
			return;
	}
}

void CExp00069View::OnUpdateBtnB10(CCmdUI* pCmdUI) 
{
	if (m_bSelTile) {
		AtlasBit_t o;
		HRESULT hr;
		if (FAILED(hr = GetDDAT().GetAtlasBitAt(m_aSelPos, o)))
			return;
		pCmdUI->SetCheck((o.c & 0x10) ? 1 : 0);
		pCmdUI->Enable(TRUE);
		return;
	}
	pCmdUI->SetCheck(0);
	pCmdUI->Enable(FALSE);
}

void CExp00069View::OnEditLvdesc() 
{
	LevelDesc_t d;
	HRESULT hr;
	if (FAILED(hr = GetDDAT().GetMapDescAt(m_iLv, d)))
		return;
	//
	CEditLvDescDlg wndDlg(this);
	wndDlg.Revert(d);
	int r = wndDlg.DoModal();
	if (r != IDOK)
		return;
	wndDlg.Commit(d);
	//
	if (FAILED(hr = GetDDAT().SetMapDescAt(m_iLv, d)))
		return;
	return;
}

void CExp00069View::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent) {
	case 0x0101:
	{
		InvalidateRect(m_hlat.rcInvalid);
		UpdateWindow();

		DWORD nTick = GetTickCount();
		int nDelta = (nTick - m_hlat.nTickStart);
		if (nDelta > 5000) {
			DisableHlat();
		}

		if (!m_hlat.bDisable) {
			CClientDC dc(this);
			static const POINT pt0 = {0, 0};
			const nInside = 20, nOutside = 100;
			double d = nDelta / 5000.0 * 3.1415926535897932384626433832795;
			const LONG axIn = cos(d) * nInside;
			const LONG ayIn = sin(d) * nInside;
			const LONG axOut = cos(d) * nOutside;
			const LONG ayOut = sin(d) * nOutside;
			POINT ptc = m_hlat.cpTar;
			POINT pts[] = {
				ptc.x + (LONG)(axIn),
				ptc.y + (LONG)(ayIn),
				ptc.x + (LONG)(axOut),
				ptc.y + (LONG)(ayOut),

				ptc.x + (LONG)(-axIn),
				ptc.y + (LONG)(-ayIn),
				ptc.x + (LONG)(-axOut),
				ptc.y + (LONG)(-ayOut),

				ptc.x + (LONG)(ayIn),
				ptc.y + (LONG)(-axIn),
				ptc.x + (LONG)(ayOut),
				ptc.y + (LONG)(-axOut),

				ptc.x + (LONG)(-ayIn),
				ptc.y + (LONG)(axIn),
				ptc.x + (LONG)(-ayOut),
				ptc.y + (LONG)(axOut),
			};
			static INT nPolyPoints[] = {
				2, 2, 2, 2
			};

			HGDIOBJ hOldObj = dc.SelectObject(GetStockObject(WHITE_PEN));
			dc.Arc(m_hlat.rcTar, pt0, pt0);
			dc.PolyPolygon(pts, nPolyPoints, 4);
			dc.SelectObject(hOldObj);
		} else {
			KillTimer(0x0101);
		}
		break;
	}
	}

	CView::OnTimer(nIDEvent);
}

void CExp00069View::OnViewForceHexTile() 
{
	m_bForceHexTile = m_bForceHexTile ? 0 : 1;

	Invalidate();
}

void CExp00069View::OnUpdateViewForceHexTile(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bForceHexTile ? 1 : 0);
}

void CExp00069View::OnEditDmfiledesc() 
{
	CDDATUndoLock lock(GetDDAT(), TRUE);

	AllMapDesc_t d;
	HRESULT hr;
	if (FAILED(hr = GetDDAT().GetAllMapDesc(d)))
		return;
	//
	CEditDMFileDescDlg wndDlg(this);
	wndDlg.Revert(d);
	UINT nMaps = (wndDlg.m_nMaps = GetDDAT().CntMap());
	int r = wndDlg.DoModal();
	if (r != IDOK)
		return;
	wndDlg.Commit(d);
	//
	if (FAILED(hr = GetDDAT().SetAllMapDesc(d)))
		return;
	if (nMaps != wndDlg.m_nMaps)
		VERIFY(SUCCEEDED(GetDDAT().SetMapCnt(wndDlg.m_nMaps)));

	lock.Unlock(TRUE);
	return;
}

void CExp00069View::OnEditRepair5thbit() 
{
	CDDATUndoLock lock(GetDDAT(), TRUE);

	HRESULT hr;
	int x, y;
	Atlas_t a;
	LevelDesc_t d;
	AtlasBit_t o;
	for (int iMap = 0, nMaps = GetDDAT().CntMap(); iMap < nMaps; iMap++) {
		if (FAILED(hr = GetDDAT().GetMapAtlasAt(iMap, a)))
			return;
		if (FAILED(hr = GetDDAT().GetMapDescAt(iMap, d)))
			return;
		for (y = 0; y < d.cyLv; y++) {
			for (x = 0; x < d.cxLv; x++) {
				BOOL bRepaired = FALSE;
				o.r = a.r[y][x];
				o.c = a.c[y][x];
				if (o.r == RN_END) {
					o.r = RN_NIL;
				} else if (o.r == RN_NIL && (o.c & 0x10)) {
					o.c &=~0x10;
				} else if (o.r != RN_NIL && !(o.c & 0x10)) {
					o.c |= 0x10;
				} else {
					continue;
				}
				if (FAILED(hr = GetDDAT().SetAtlasBitAt(AtlasPos_t(iMap, x, y), o)))
					return;
			}
		}
	}

	lock.Unlock(TRUE);
}

void CExp00069View::PostNcDestroy() 
{
	if (m_pEdCtx)
		m_pEdCtx->RemoveCB(this),
		m_pEdCtx->Release(),
		m_pEdCtx = NULL;
	
	CView::PostNcDestroy();
}

void CExp00069View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	BOOL fCtrl = (GetKeyState(VK_CONTROL) & 0x8000);
	int nLvDelta = 0;
	switch (nChar) {
	case VK_NEXT:
		if (!fCtrl) nLvDelta = +1;
		break;
	case VK_PRIOR:
		if (!fCtrl) nLvDelta = -1;
		break;
	case VK_HOME:
		if (fCtrl) nLvDelta = -256;
		break;
	case VK_END:
		if (fCtrl) nLvDelta = +256;
		break;
	}
	if (nLvDelta != 0) {
		CMainFrameIndir *pIndir;
		if (pIndir = dynamic_cast<CMainFrameIndir *>(GetParentFrame())) {
			UINT i = pIndir->GetDMLv(), o = i;
			UINT n = pIndir->GetLvCnt();
			i = __max(0, __min((int)i + nLvDelta, (int)n - 1));
			if (i != o) pIndir->SetDMLv(i);
		}
	}
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CExp00069View::OnEditOptimize() 
{
	CWaitCursor wc;

	GetDDAT().Optimize();
	
}

void CExp00069View::OnEditCheckIntegrity() 
{
	CheckIntegrity();
}

bool CExp00069View::CheckIntegrity()
{
	CDDAT *pDDIf = m_pEdCtx->GetDDIf();
	if (!pDDIf) return false;

	CDMCIIf *pDMCIIf = GetDMCIIf();
	if (!pDMCIIf) return false;

	HTREEITEM hRoot = pDMCIIf->AddRoot_CheckIntegrity();
	HTREEITEM hItemDupe = pDMCIIf->AddSite_Memo(hRoot, "DUPLICATED...");
	HTREEITEM hItemCorr = pDMCIIf->AddSite_Memo(hRoot, "CORRUPT RECORD LINK...");
	HTREEITEM hItemLiss = pDMCIIf->AddSite_Memo(hRoot, "LOGICAL ISSUE...");

	typedef multimap<DBIPos_t, AtlasPos_t> RecRefMMap;
	RecRefMMap mmapRecRef;

	UINT iMap, nMaps = pDDIf->CntMap();
	for (iMap = 0; iMap < nMaps; iMap++) {
		LevelDesc_t d;
		Atlas_t g;
		if (FAILED(pDDIf->GetMapDescAt(iMap, d))) return false;
		if (FAILED(pDDIf->GetMapAtlasAt(iMap, g))) return false;

		UINT cx = d.cxLv, cy = d.cyLv;

		for (UINT y = 0; y < cy; y++) {
			for (UINT x = 0; x < cx; x++) {
				if (g.c[y][x] & 0x10) {
					AtlasPos_t pos(iMap, x, y);
					if (!IsThisAsIs(g.r[y][x])) {
						DBIPos_t rpos(g.r[y][x]);
						if (pDDIf->CntDBRecIn(rpos.db()) <= rpos.idx()) {
							pDMCIIf->AddSite_CheckInt(hItemCorr, pos, "UNDEFINED LINK TARGET");
						} else {
							mmapRecRef.insert(RecRefMMap::value_type(rpos, pos));
						}
					} else {
						pDMCIIf->AddSite_CheckInt(hItemCorr, pos, "NO LINK TARGET");
					}
				}
			}
		}
	}

	{
		RecRefMMap::iterator
			iterPos = mmapRecRef.begin(),
			iterEnd = mmapRecRef.end();
		while (iterPos != iterEnd) {
			RecRefMMap::iterator
				iterLwr = mmapRecRef.lower_bound(iterPos->first),
				iterUpr = mmapRecRef.upper_bound(iterPos->first);
			if (distance(iterLwr, iterUpr) > 2) {
				DMDBU rec;
				pDDIf->GetDBRecAt(DBIPos_t(iterPos->first), rec);
				CString strMes;
				MakeHint(RN_END, iterPos->first, &rec, strMes, RN());

				HTREEITEM hItem = pDMCIIf->AddSite_Memo(hItemDupe, strMes);
				for (; iterLwr != iterUpr; iterLwr++) {
					pDMCIIf->AddSite_CheckInt(hItem, iterLwr->second, "");
				}
			}
			iterPos = iterUpr;
		}
	}

	pDMCIIf->PickItUp(hRoot);

	static_cast<CMainFrameDockSite *>(static_cast<CMainFrame *>(GetParentFrame()))->ShowDockedSite(IDW_DMCIBAR, TRUE);

	return true;
}

CDMCIIf *CExp00069View::GetDMCIIf()
{
	CDMCITreeView *pView = static_cast<CDMCITreeView *>(static_cast<CDMCIBar *>(static_cast<CMainFrameDockSite *>(static_cast<CMainFrame *>(GetParentFrame()))->GetDockedSite(IDW_DMCIBAR))->GetDlgItem(AFX_IDW_PANE_FIRST));

	return pView;
}
