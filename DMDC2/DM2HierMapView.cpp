// DM2HierMapView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "DM2HierMapView.h"
#include "Exp00069Doc.h"
#include "CompatDC.h"
#include "DM2EditTilePropDlg.h"
#include <math.h>
#include "EditDM2FileDescDlg.h"
#include "EditLvDesc3Dlg.h"
#include "CurDir.h"
#include "HUGHUG.h"
#include "BMODID.h"
#include "TsDataSource.h"
#include "TsDataObject.h"
#include "TextInputDlg.h"
#include "DMTextEdit.h"
#include "BuildLabuDlg.h"
#include "Maze.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

#define EPS 1E-5

#define D2R(X) ((X) / 180.0 * 3.14)

const DWORD nDefSaveFlags = 0
	|OFN_ENABLESIZING
	|OFN_EXPLORER
	|OFN_HIDEREADONLY
	|OFN_LONGNAMES
	|OFN_OVERWRITEPROMPT
	|OFN_PATHMUSTEXIST
	;

/////////////////////////////////////////////////////////////////////////////
// CDM2HierTileEd

BOOL CDM2HierTileEd::DoSiteModal(BYTE cTile, CRect rectMin, CRect rectMax, CWnd *pParentWnd)
{
	Destroy();

	CEdTileDlg *p = NULL;
	switch (cTile / 0x20) {
	case 0:
	case 1:
	case 7:
	default:p = &m_wndDefault; break;
	case 2: p = &m_wndPit; break;
	case 3: p = &m_wndStairs; break;
	case 4: p = &m_wndDoor; break;
	case 5: p = &m_wndTele; break;
	case 6: p = &m_wndFalse; break;
	}
	if (p != NULL) {
		if (p->Create(cTile, pParentWnd)) {
			CRect rc;
			p->GetWindowRect(&rc);
			CRect rcNew;
			if (rc.Width() <= rectMax.right - rectMin.right) {
				rcNew.left = rectMin.right;
			} else {
				rcNew.left = rectMin.left - rc.Width();
			}
			if (rc.Height() <= rectMax.bottom - rectMin.bottom) {
				rcNew.top = rectMin.bottom;
			} else {
				rcNew.top = rectMin.top - rc.Height();
			}

			p->SetWindowPos(NULL, rcNew.left, rcNew.top, 0, 0, 0 |SWP_NOSIZE |SWP_NOZORDER |SWP_SHOWWINDOW |SWP_NOOWNERZORDER);
			p->SetFocus();
			p->SetDlgCtrlID(0x0100);
			p->m_fSiteHook = TRUE;

			m_pSite = p;
			return TRUE;
		}
	}
	return FALSE;
}

BYTE CDM2HierTileEd::GetTileNewVal()
{
	if (m_pSite)
		return m_pSite->m_cTileNew;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDM2HierMapView

IMPLEMENT_DYNCREATE(CDM2HierMapView, CScrollView)

CDM2HierMapView::CDM2HierMapView()
{
	m_pEdCtx = NULL;
	m_iLv = 6;
	m_fDispTileVal = false;
	m_hMoveCursor = AfxGetApp()->LoadCursor(IDC_H_MOVE);
	m_fSpcMove = false;
	m_fSnapTL = false;
}

CDM2HierMapView::~CDM2HierMapView()
{
	InitEdCtx(NULL);
}

BEGIN_MESSAGE_MAP(CDM2HierMapView, CScrollView)
	//{{AFX_MSG_MAP(CDM2HierMapView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_VIEW_DISPLAY_TILE_VAL, OnViewDisplayTileVal)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DISPLAY_TILE_VAL, OnUpdateViewDisplayTileVal)
	ON_COMMAND(ID_EDIT_DMFILEDESC, OnEditDmFileDesc)
	ON_COMMAND(ID_EDIT_LVDESC, OnEditLvDesc)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LVDESC, OnUpdateEditLvDesc)
	ON_COMMAND(ID_EDIT_ADD_MAP, OnEditAddMap)
	ON_COMMAND(ID_EDIT_OPTIMIZE, OnEditOptimize)
	ON_COMMAND(ID_EDIT_MK_TOPVW, OnEditMkTopvw)
	ON_COMMAND(ID_EDIT_OPTIMIZE_FLAT, OnEditOptimizeFlat)
	ON_WM_KEYUP()
	ON_COMMAND(ID_VIEW_SNAP_TO_TL, OnViewSnapToTL)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SNAP_TO_TL, OnUpdateViewSnapToTL)
	//}}AFX_MSG_MAP
	ON_COMMAND(0x0100, OnCommand0100)
	ON_COMMAND_RANGE(ID_VIEW_TLV0, ID_VIEW_TL_MAX, OnViewTlv0)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_TLV0, ID_VIEW_TL_MAX, OnUpdateViewTlv0)
	ON_COMMAND_RANGE(ID_EDIT_ROTX_P, ID_EDIT_ROTY_N, OnEditRotyN)
	ON_UPDATE_COMMAND_UI_RANGE(ID_EDIT_ROTX_P, ID_EDIT_ROTY_N, OnUpdateEditRotyN)
	ON_COMMAND_EX(ID_OPT_RELOAD_ORNATES, OnReloadOrnates)
	ON_COMMAND(ID_EDIT_BUILDLABYRINTH, OnEditBuildlabyrinth)
	ON_COMMAND_RANGE(ID_OPT_ASSETS_DM2, ID_OPT_ASSETS_DM2TELOS, OnOptAssets)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPT_ASSETS_DM2, ID_OPT_ASSETS_DM2TELOS, OnUpdateOptAssets)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2HierMapView 描画

void CDM2HierMapView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal(24*287, 24*287);
	SetScrollSizes(MM_TEXT, sizeTotal, sizeDefault, CSize(24, 24));

	InitEdCtx(static_cast<CExp00069Doc *>(GetDocument())->GetEdCtx(FALSE));

	m_rct.m_nStyle = 0 |CRectTracker::dottedLine |CRectTracker::resizeInside;
	m_rct.m_nHandleSize = 5;
	m_rct.m_sizeMin = CSize(24, 24);

	m_iLevelSt = -1;
	m_ptCurSel = CPoint(-1, -1);
	m_fNoTriViewMessage = false;
	m_fBuildTS = true;

	m_offsc.Create24Screen();
	m_offsc.Clear();

	ZeroMemory(m_cVram1, sizeof(m_cVram1));
	ZeroMemory(m_cVram2, sizeof(m_cVram2));

	m_dropTar.Revoke();
	m_dropTar.Register(this);
	m_ptDnDcur = CPoint(-1, -1);

	OnReview();
}

void CDM2HierMapView::OnDraw(CDC* pDC)
{
	if (m_fBuildTS) {
		BuildTS();
		m_fBuildTS = false;
	}

	if ((UINT)m_listLS.GetSize() <= (UINT)m_iLevelSt) {
		m_iLevelSt = -1;
	}

	CRect rcSize;
	GetClientRect(&rcSize);

	const CPoint pt = GetDeviceScrollPosition();

	UINT ctx = (rcSize.Width() +23) / 24 + 1;
	UINT cty = (rcSize.Height() +23) / 24 + 1;
	UINT ox = pt.x / 24;
	UINT oy = pt.y / 24;

	int btx = m_bthl.pt.x;
	int bty = m_bthl.pt.y;

	pDC->SaveDC();

	CRect rcHi;
	if (m_iLevelSt >= 0) {
		LevelSt &rLS = m_listLS[m_iLevelSt];
		rcHi = rLS.rc;
	}

	CRect rcBT(0, 0, 0, 0);

	CBMO *pbmoHatch1 = GetApp()->FindBMO(BMODID_HATCH1 * 4);
	CBMO *pbmoHatch2 = GetApp()->FindBMO(BMODID_HATCH2 * 4);
	CBMO *pbmoTS = GetApp()->FindBMO(BMODID_TS * 4);
	CBMO *pbmoZero = GetApp()->FindBMO(BMODID_BLACK * 4);

#if 0
	{
		CBMO *pbmo = GetApp()->FindBMO(4*(BMODID_TILE0 +0));

		FILE *f = fopen("\\96x192.raw", "wb");
		if (f) {
			fwrite(pbmo->pv, 96*192, 1, f);
			fclose(f);
		}
	}
#endif

#define VRAM1_TILE_MASK		0x1F
#define VRAM1_CLIP_MASK		0x20
#define VRAM1_HATCH1_MASK	0x40
#define VRAM1_HATCH2_MASK	0x80

	int csc[2] = {0, 0};

	UINT x, y;
	for (y = 0; y < cty; y++) {
		for (x = 0; x < ctx; x++) {
			UINT tx = ox + x;
			UINT ty = oy + y;
			if (tx < 287 && ty < 287) {
				TileSt &rTS = m_aTS[ty][tx];

				WORD cVram1 = 0;
				DWORD cVram2 = 0;

				int vrx = min(66, x);
				int vry = min(50, y);

				BYTE iTile = rTS.nTile;
				if (iTile < 7) {
					cVram1 = iTile +(8*rTS.iZone);

					if (rTS.nZ != m_iLv) {
						cVram1 |= VRAM1_HATCH2_MASK;
					} else if (IsHidMasked(rTS.iMap) || (rTS.iMap != m_iLevelSt && rcHi.PtInRect(CPoint(tx, ty)))) {
						cVram1 |= VRAM1_HATCH2_MASK | VRAM1_HATCH1_MASK;
					}
					cVram1 |= VRAM1_CLIP_MASK;
				}

				DWORD iT4w = rTS.iT4w;
				if (iT4w != 0 && rTS.nZ == m_iLv) {
					if (iT4w & 0x0001)
						iT4w = (m_iLevelSt < 0) ? (0x0001) : (iT4w & (~0x0001));

					cVram2 = iT4w;
				}

				CRect rcTar;
				rcTar.right = (rcTar.left = 24*x) + 24;
				rcTar.bottom = (rcTar.top = 24*y) + 24;

				if (tx == btx && ty == bty) {
					rcBT = rcTar;
				}

				if (false
					|| m_cVram1[y][x] != cVram1
					|| m_cVram2[y][x] != cVram2
				) {
					if (cVram1 & VRAM1_CLIP_MASK) {
						//
						CBMO *pbmo = GetApp()->FindBMO(4*(BMODID_TILE0 +8*(1+rTS.iZone) +iTile));
						if (pbmo == NULL)
							pbmo = GetApp()->FindBMO(4*(BMODID_TILE0 +iTile));
						if (pbmo != NULL) {
							int ax = pbmo->cx / 24;
							int ay = pbmo->cy / 24;
							int bx = tx % ax;
							int by = ty % ay;
							m_offsc.FlipMe(rcTar.left, rcTar.top, *pbmo, 24*bx, 24*by, 24, 24, SRCCOPY);
							if ((cVram1 & VRAM1_HATCH2_MASK) && pbmoHatch2 != NULL) {
								m_offsc.FlipMe(rcTar.left, rcTar.top, *pbmoHatch2, 0, 0, 24, 24, SRCAND);
							}
							if ((cVram1 & VRAM1_HATCH1_MASK) && pbmoHatch1 != NULL) {
								m_offsc.FlipMe(rcTar.left, rcTar.top, *pbmoHatch1, 0, 0, 24, 24, SRCPAINT);
							}
						}

						static const DWORD nTbl[32] = {
								  0x01,       0x02,       0x04,       0x08, 
								  0x10,       0x20,       0x40,       0x80,
								0x0100,     0x0200,     0x0400,     0x0800,
								0x1000,     0x2000,     0x4000,     0x8000,
							0x00010000, 0x00020000, 0x00040000, 0x00080000, 
							0x00100000, 0x00200000, 0x00400000, 0x00800000,
							0x01000000, 0x02000000, 0x04000000, 0x08000000,
							0x10000000, 0x20000000, 0x40000000, 0x80000000,
						};
						static const int nXTbl[32] = {
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,
						};
						static const int nYTbl[32] = {
							24*0,24*1,24*2,24*3,24*4,24*5,24*6,24*7,24*8,24*9,24*10,24*11,24*12,24*13,24*14,24*15,
							24*0,24*1,24*2,24*3,24*4,24*5,24*6,24*7,24*8,24*9,24*10,24*11,24*12,24*13,24*14,24*15,
						};

						if (pbmoTS != NULL) {
							for (int x = 0; x < 32; x++) {
								if (cVram2 & nTbl[x]) {
									m_offsc.FlipMe(rcTar.left, rcTar.top, *pbmoTS, nXTbl[x] +24, nYTbl[x], 24, 24, SRCAND);
									m_offsc.FlipMe(rcTar.left, rcTar.top, *pbmoTS, nXTbl[x]  +0, nYTbl[x], 24, 24, SRCPAINT);
								}
							}
						}
						//
					} else {
						m_offsc.FlipMe(rcTar.left, rcTar.top, *pbmoZero, 0, 0, 24, 24, SRCCOPY);
					}

					m_cVram1[y][x] = cVram1;
					m_cVram2[y][x] = cVram2;

					csc[0]++;
				} else {
					csc[1]++;
				}
			}
		}
	}

//	TRACE1("csc:%5.1f\n", 100.f *csc[0] / (0.f +csc[0] +csc[1]));

	CRect rcMax;
	pDC->GetClipBox(&rcMax);

	pDC->IntersectClipRect(&rcMax);

	m_offsc.FlipTo(*pDC, 
		24*ox,
		24*oy,
		24*ctx,
		24*cty,
		0,
		0
		);

	pDC->RestoreDC(-1);

	if (!rcBT.IsRectNull()) {
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->SelectStockObject(WHITE_PEN);

		rcBT.OffsetRect(24*ox, 24*oy);

		CRect rcAt = rcBT;

		rcBT.InflateRect(16, 16);
		pDC->Ellipse(rcBT);
		rcBT.InflateRect(16, 16);
		pDC->Ellipse(rcBT);

		const long cxLong = 100;

		CRect rcBox(0, 0, 24*287, 24*287);
		CPoint pt0 = rcAt.CenterPoint();
		// L->C
		pDC->MoveTo(_MAX(rcBox.left, pt0.x - cxLong), pt0.y);
		pDC->LineTo(rcAt.left, pt0.y);
		// T->C
		pDC->MoveTo(pt0.x, _MAX(rcBox.top, pt0.y - cxLong));
		pDC->LineTo(pt0.x, rcAt.top);
		// C->R
		pDC->MoveTo(rcAt.right, pt0.y);
		pDC->LineTo(_MIN(rcBox.right, pt0.x + cxLong), pt0.y);
		// C->B
		pDC->MoveTo(pt0.x, rcAt.bottom);
		pDC->LineTo(pt0.x, _MIN(rcBox.bottom, pt0.y + cxLong));
	}

	pDC->SaveDC();
	pDC->SetBkMode(TRANSPARENT);

	{
		CPen a1(PS_SOLID, 0, RGB(255,255,255));
		CPen a2(PS_SOLID, 0, RGB(255,255,  0));
		CPen a3(PS_SOLID, 0, RGB(255,  0,  0));

		CPen *aps[] = {&a1, &a2, &a3, &a1};

		CDMTarTrack::Sentry::iterator
			iterPos = m_tarTrack.m_sentry.begin(),
			iterEnd = m_tarTrack.m_sentry.end();
		for (; iterPos != iterEnd; iterPos++) {
			AtlasPos_t ap = iterPos->first.T();
			if (ap.Z() < m_listLS.GetSize()) {
				LevelSt &rSrc = m_listLS[ap.Z()];
				CDMTarTrack::Tar t = iterPos->second;
				if (t.ap.Z() < m_listLS.GetSize()) {
					LevelSt &rDest = m_listLS[t.ap.Z()];
					int x1 = rSrc.rc.left +ap.Y();
					int y1 = rSrc.rc.top +ap.X();
					int x2 = rDest.rc.left +t.ap.Y();
					int y2 = rDest.rc.top +t.ap.X();
					bool fGoodSrc = (m_aTS[y1][x1].nZ == rSrc.nZ);
					bool fGoodDst = (m_aTS[y2][x2].nZ == rDest.nZ);

					CPoint pt1(24*x1 +12, 24*y1 +12);
					CPoint pt2(24*x2 +12, 24*y2 +12);

					pDC->SelectObject(aps[t.ttt % 4]);
					pDC->SetTextColor(RGB(255, 255, 0));

					CString strSrc; strSrc.Format("%u", 0U +rSrc.nZ);
					CString strDst; strDst.Format("%u", 0U +rDest.nZ);
					DrawArrow(pDC, pt1, pt2, 1
						, fGoodSrc ? NULL : (LPCTSTR)strSrc
						, fGoodDst ? NULL : (LPCTSTR)strDst
						);
				}
			}
		}

		pDC->SelectStockObject(WHITE_PEN);
	}

	VERIFY(pDC->RestoreDC(-1));

	if (m_iLevelSt >= 0) {
		m_rct.Draw(pDC);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDM2HierMapView 診断

#ifdef _DEBUG
void CDM2HierMapView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDM2HierMapView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDM2HierMapView メッセージ ハンドラ

BOOL CDM2HierMapView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	
	return CScrollView::OnEraseBkgnd(pDC);
}

void CDM2HierMapView::BuildTS()
{
	if (MkDDIf() == NULL) return;

	UINT x, y;

	for (y = 0; y < 287; y++)
		for (x = 0; x < 287; x++)
			m_aTS[y][x] = TileSt();

	CDDAT &rDD = *m_pEdCtx->GetDDIf();
	UINT iMap, nMaps = rDD.CntMap();
	m_listLS.RemoveAll();
	for (iMap = 0; iMap < nMaps; iMap++) {
		HRESULT hr;
		LevelDesc_t aLevel;
		if (FAILED(hr = rDD.GetMapDescAt(iMap, aLevel))) continue;
		Atlas_t aAtlas;
		if (FAILED(hr = rDD.GetMapAtlasAt(iMap, aAtlas))) continue;

		UINT x, y, cx = aLevel.cxLv, cy = aLevel.cyLv;

		BYTE z = aLevel.iLv;

		LevelSt aLS;
		aLS.rc.right = (aLS.rc.left = aLevel.nShiftX) + cy;
		aLS.rc.bottom = (aLS.rc.top = aLevel.nShiftY) + cx;
		aLS.nZ = z;
		m_listLS.Add(aLS);

		if (m_iLv > z) continue;

		for (y = 0; y < cy; y++) {
			for (x = 0; x < cx; x++) {
				int tx = aLevel.nShiftX + y;
				int ty = aLevel.nShiftY + x;
				TileSt &rTS = m_aTS[ty][tx];
				if (rTS.iMap == m_iLevelSt || rTS.nZ < z) continue;
				BYTE c = aAtlas.c[y][x];
				rTS.iT4w = CalcT4w(aAtlas.r[y][x], aAtlas.c[y][x], CDMDBRMask()) | ((rTS.iT4w && rTS.nZ == z) ? 0x0001 : 0x0000);
				BYTE iTile = 0xFF;
				switch (c / 0x20) {
				case 0: // WALL
					iTile =	1;
					break;
				case 1: // FLOOR
					iTile = 0;
					break;
				case 2: // PITFALL
					iTile = 2;
					break;
				case 3: // STAIRCASE
					iTile = ((c & 4) != 0) ? 4 : 3;
					// Bit 3: Direction ... '0' Down, '1' Up
					break;
				case 4: // DOOR
					iTile = 0;
					break;
				case 5: // TELE
					iTile = 5;
					break;
				case 6: // ILLUSION
					iTile = 6;
					break;
				case 7: // MASK
					break;
				}
				if (iTile < 7) {
					rTS.nTile = iTile;
					rTS.nZ = z;
					rTS.iMap = iMap;
					rTS.iZone = aLevel.n14_4_7;
				}
			}
		}
	}
}

void CDM2HierMapView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	BOOL fShift = nFlags & MK_SHIFT;
	BOOL fCtrl = nFlags & MK_CONTROL;
	BOOL fAlt = GetKeyState(VK_MENU) < 0;

	if (Deselect()) return;

	if (!fCtrl && fShift && !fAlt) {
		if (SelectLevelStAt(GetDeviceScrollPosition() + point)) {
			Invalidate();
		}
		return;
	} else if (fCtrl && !fShift && !fAlt) {
		if (!MkDDIf()) return;
		CPoint pt = GetDeviceScrollPosition() + point;
		int r = m_rct.HitTest(pt);
		AtlasPos_t ap;
		if (r == CRectTracker::hitMiddle && CalcAPFromClient(point, ap)) {
			AtlasBit_t ab;
			HRESULT hr;
			if (SUCCEEDED(hr = m_pDDIf->GetAtlasBitAt(ap, ab))) {
				CRect rcMax;
				GetClientRect(&rcMax);
				CPoint pnt = pt;
				pnt.x = pnt.x / 24 * 24;
				pnt.y = pnt.y / 24 * 24;
				pnt -= GetDeviceScrollPosition();
				CRect rcMin(pnt.x, pnt.y, pnt.x+24, pnt.y+24);
				if (m_tileEd.DoSiteModal(ab.c, rcMin, &rcMax, this)) {
					m_apSite = ap;
				}
			}
		}
		return;
	} else {
		CPoint pt = GetDeviceScrollPosition() + point;
		int r = m_rct.HitTest(pt);
		if (r == CRectTracker::hitMiddle && !fAlt) {
			SetPntTile(pt - m_rct.m_rect.TopLeft());
		} else {
			BOOL fUpdate = m_rct.Track(this, pt, FALSE, NULL);
			if (fUpdate) {
				if (MkDDIf()) {
					LevelDesc_t a;
					HRESULT hr;
					VERIFY(SUCCEEDED(hr = m_pDDIf->GetMapDescAt(m_iLevelSt, a)));
					a.cyLv = m_rct.m_sizeNew.cx;
					a.cxLv = m_rct.m_sizeNew.cy;
					a.nShiftX = m_rct.m_ptNew.x;
					a.nShiftY = m_rct.m_ptNew.y;
					VERIFY(SUCCEEDED(hr = m_pDDIf->SetMapDescAt(m_iLevelSt, a)));
				}
			}
			MarkBuildTS();
			Invalidate();
			return;
		}
	}
	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CDM2HierMapView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (nFlags & MK_SHIFT) {
		CPoint pt = GetDeviceScrollPosition() + point;

		if (SelectLevelStAt(pt)) {
			Invalidate();
			return;
		}
	} else if (MkDDIf()) {
		AtlasPos_t ap;
		if (CalcAPFromClient(point, ap)) {
			CDM2EditTilePropDlg wndDlg(this);
			if (wndDlg.Init(m_pDDIf, ap)) {
				wndDlg.DoModal();
			}
		}
	}
	
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CDM2HierMapView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (nFlags & MK_SHIFT) {

	}
	else if (m_fSpcMove) {
		if (m_fSpcAltMove) {
			CPoint ptCur;
			GetCursorPos(&ptCur);
			CPoint ptDelta = m_ptMoveFirstCursor - ptCur;
			ptDelta.x = ptDelta.x *3 /2;
			ptDelta.y = ptDelta.y *3 /2;
			SetCursorPos(m_ptMoveFirstCursor.x, m_ptMoveFirstCursor.y);

			ScrollToPosition(GetScrollPosition() + ptDelta);
			UpdateWindow();
		}
		else {
			CPoint ptCur;
			GetCursorPos(&ptCur);
			CPoint ptDelta = m_ptMoveFirstCursor - ptCur;

			ScrollToPosition(m_ptMoveFirstPos + ptDelta);
		}
	}
	else {
		CPoint pt = GetDeviceScrollPosition() + point;

		int r = m_rct.HitTest(pt);
		if (r == CRectTracker::hitMiddle) {
			if (nFlags & MK_LBUTTON) {
				SetPntTile(pt - m_rct.m_rect.TopLeft(), true);
				return;
			}
		}
	}
	
	CScrollView::OnMouseMove(nFlags, point);
}

bool CDM2HierMapView::SelectLevelStAt(CPoint point)
{
	CPoint ptSel(point.x /24, point.y /24);
	int iLevelSt = m_iLevelSt;
	if (m_ptCurSel != ptSel) {
		multimap<BYTE, BYTE> m;
		UINT i;
		for (i = 0; i < m_listLS.GetSize(); i++) {
			LevelSt &rLS = m_listLS[i];
			if (rLS.nZ == m_iLv && rLS.PtInRect(ptSel.x, ptSel.y)) {
				m.insert(pair<BYTE, BYTE>(rLS.nZ, i));
			}
		}
		multimap<BYTE, BYTE>::iterator
			iterPos = m.begin(),
			iterEnd = m.end();
		m_listLSIdx.RemoveAll();
		for (; iterPos != iterEnd; iterPos++) {
			m_listLSIdx.Add(iterPos->second);
		}
		if (m_listLSIdx.GetSize() != 0) {
			for (i = 0; i < m_listLSIdx.GetSize() && m_listLSIdx[i] != m_iLevelSt; i++);
			if (i != m_listLSIdx.GetSize() && m_iLevelSt >= 0) i++;
			i = i % m_listLSIdx.GetSize();
			iLevelSt = m_listLSIdx[i];
		} else {
			iLevelSt = -1;
		}
		m_iLSIdx = 0;
		m_ptCurSel = ptSel;
	} else {
		if (m_listLSIdx.GetSize() != 0) {
			m_iLSIdx = (m_iLSIdx + 1) % m_listLSIdx.GetSize();
			iLevelSt = m_listLSIdx[m_iLSIdx];
		} else {
			iLevelSt = -1;
		}
	}
	return SelectLevel(iLevelSt);
}

bool CDM2HierMapView::SelectLevel(UINT iLevelSt, bool fMoveToCenter)
{
	if (m_iLevelSt == iLevelSt || m_listLS.GetSize() <= iLevelSt) return false;

	m_iLevelSt = iLevelSt;

	LevelSt &rLS = m_listLS[m_iLevelSt];
	m_iLv = rLS.nZ;

	MarkBuildTS();

	if (fMoveToCenter) {
		LevelSt &rLS = m_listLS[m_iLevelSt];

		if (m_fSnapTL) {
			// snap to top-left
			CRect rc;
			GetClientRect(rc);
			CPoint pt = rLS.rc.TopLeft();
			pt.x = 24 * _std_max<int>(0, (int)pt.x);
			pt.y = 24 * _std_max<int>(0, (int)pt.y);
			ScrollToPosition(pt);
		}
		else {
			// move to center
			CRect rc;
			GetClientRect(rc);
			CPoint pt = rLS.rc.CenterPoint();
			pt.x = 24 * _std_max<LONG>(0, pt.x - (rc.Width() +0) / 24 / 2);
			pt.y = 24 * _std_max<LONG>(0, pt.y - (rc.Height() +0) / 24 / 2);
			ScrollToPosition(pt);
		}
		Invalidate();
	}

	if (MkMainFrmIf()) {
		m_fNoTriViewMessage = true;
		m_pMainFrmIf->SetVwLv(m_iLv);
		m_fNoTriViewMessage = false;

		m_pMainFrmIf->SetDMLv(m_iLevelSt);
	}

	if (m_iLevelSt >= 0) {
		LevelSt &rLS = m_listLS[m_iLevelSt];
		CRect rc(rLS.rc);
		m_rct.m_rect = CRect(24*rc.left, 24*rc.top, 24*rc.right, 24*rc.bottom);
		m_rct.m_rect.DeflateRect(1, 1);
	}

	return true;
}

void CDM2HierMapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	bool ctrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;

	switch (nChar) {
	case VK_ESCAPE:
		Deselect();
		if (m_iLevelSt != -1) {
			m_iLevelSt = -1;
			m_ptCurSel = CPoint(-1, -1);
			Invalidate();

			if (MkMainFrmIf()) {
				m_pMainFrmIf->SetDMLv(-1);
			}
		}
		break;
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
		if (GetKeyState(VK_CONTROL) < 0) {
			int dx = 0, dy = 0;
			switch (nChar) {
			case VK_UP: dy = -1; break;
			case VK_DOWN: dy = 1; break;
			case VK_LEFT: dx = -1; break;
			case VK_RIGHT: dx = 1; break;
			}
			ScrollTileXY(dx, dy);
			return;
		}
		break;
	case VK_SPACE:
		if (!m_fSpcMove) {
			m_fSpcMove = true;
			m_fSpcAltMove = (GetKeyState(VK_CONTROL) & 0x8000) ? true : false;
			GetCursorPos(&m_ptMoveFirstCursor);
			m_ptMoveFirstPos = GetScrollPosition();
		}
		break;
	case VK_NEXT:
	case VK_PRIOR:
		if (!ctrl) break;
		if (MkMainFrmIf()) {
			Deselect();
			int iLv = _cpp_min(15, _cpp_max(0, m_iLv + ((nChar == VK_NEXT) ? +1 : -1)));
			if (iLv != m_iLv) {
				m_pMainFrmIf->SetVwLv(m_iLv = iLv);
				OnKeyDown(VK_ESCAPE, 0, 0);
				MarkBuildTS();
				Invalidate();
			}
		}
		break;
	}
	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CDM2HierMapView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_iLevelSt >= 0 && m_rct.SetCursor(this, nHitTest)) {
		return TRUE;
	}
	if (nHitTest != HTERROR && m_fSpcMove) {
		SetCursor(m_hMoveCursor);
		return true;
	}
	
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void CDM2HierMapView::SetPntTile(CPoint point, bool fNoTileSel)
{
	if (m_iLevelSt < 0) return;
	if (!MkMainFrmIf()) return;
	CPoint pt = point;
	int x = pt.x / 24;
	int y = pt.y / 24;

	UINT iTool = m_iEditLvTool;
	if (iTool == 0) {
		AtlasPos_t ap(m_iLevelSt, y, x);

		if (!fNoTileSel) {
			m_pEdCtx->DispatchDMUpdate(DPM_SELECTLVXY, &ap);

			if (MkDDIf()) {
				m_tarTrack.Track(*m_pDDIf, ap, m_ttlv);
				Invalidate();
			}
		}
	} else if (MkDDIf()) {
		iTool--;

		CDDAT &rDD = GetDDIf();
		AtlasBit_t ab;
		AtlasPos_t ap(m_iLevelSt, y, x);
		rDD.GetAtlasBitAt(ap, ab);
		BYTE c = ab.c;
		c = (c & 0x10) | (0x20 * iTool);
		bool fNeedUpdate = false;
		if (m_fEraseTileRec) {
			if (ab.c & 0x10 || ab.r != RN_NIL) {
				ab.c &= ~0x10;
				ab.r = RN_NIL;
				fNeedUpdate = true;
			}
		}
		if (ab.c != c) {
			CDDATUndoLock lock(rDD, TRUE);
			if (RemoveTileSpec(ab)) {
				ab.c = c;
				if (AddTileSpec(ab)) {
					fNeedUpdate = true;

					lock.Unlock(TRUE);
				}
			}
		}
		if (ab.r == RN_NIL && (ab.c & 0x10)) ab.c &= ~0x10, fNeedUpdate = true;
		else if (ab.r != RN_NIL && !(ab.c & 0x10)) ab.c |= 0x10, fNeedUpdate = true;

		if (fNeedUpdate) {
			rDD.SetAtlasBitAt(ap, ab);

			UpdatePntTile(CPoint(x, y));
		}
	}
}

void CDM2HierMapView::UpdatePntTile(CPoint point)
{
	if (m_iLevelSt < 0) return;
	LevelSt &rLS = m_listLS[m_iLevelSt];
	CRect rc = rLS.rc;
	CRect rcInval(rc.left*24, rc.top*24, rc.right*24, rc.bottom*24);
	MarkBuildTS();
	InvalidateRect(rcInval);
}

void CDM2HierMapView::OnDMUpdate(UINT nHint, void *pHint)
{
	switch (nHint) {
	case DPM_CHANGED_ATLASBIT:
		{
			AtlasPos_t ap = *(AtlasPos_t *)pHint;
			UpdatePntTile(ap);
			break;
		}
	case DPM_EDITTILE_LVXY:
		{
			SelectLvXY_t *pAny = (SelectLvXY_t *)pHint;

			CDM2EditTilePropDlg wndDlg(this);
			if (wndDlg.Init(m_pDDIf, pAny->pos)) {
				wndDlg.DoModal();
			}
			break;
		}
	case DPM_SELECTBTLVXY:
		{
			SelectLvXY_t *pAny = (SelectLvXY_t *)pHint;
			BTTile(pAny->pos);
			break;
		}
	case DPM_CHANGED_LEVELDESC:
	case DPM_CHANGED_ATLAS:
	case DPM_DELETED_MAP:
	case DPM_CHANGED_DBITEM:
	case DPM_INSERTED_MAP:
	case DPM_INSERTED_DBITEM:
	case DPM_DELETED_DBITEM:
	case DPM_CHANGING_DBITEM:
	case DPM_LINKED_DBITEM:
		{
			MarkBuildTS();
			Invalidate();
			break;
		}
	}
}

void CDM2HierMapView::OnTriViewMessage(UINT nHint, void *pHint)
{
	if (m_fNoTriViewMessage) return;

	switch (nHint) {
	case DPM_SET_VW_LV:
		{
			int iLv = *(int *)pHint;
			if (iLv < 0) break;
			m_iLv = iLv;
			m_ptCurSel = CPoint(-1, -1);
			m_iLevelSt = -1;
			MarkBuildTS();
			Invalidate();
			break;
		}
	case DPM_SET_VW_MAP:
		{
			int iMap = *(int *)pHint;
			if (iMap < 0) break;
			SelectLevel(iMap, true);
			break;
		}
	}
}

void CDM2HierMapView::UpdatePntTile(AtlasPos_t ap)
{
	MarkBuildTS();
	Invalidate();
}

bool CDM2HierMapView::CalcAPFromClient(CPoint pt, AtlasPos_t &ap)
{
	if (m_iLevelSt < 0) return false;
	CPoint pto = m_listLS[m_iLevelSt].rc.TopLeft();
	pto.x *= 24;
	pto.y *= 24;
	pt = pt + GetDeviceScrollPosition() - pto;
	int x = pt.x / 24;
	int y = pt.y / 24;
	ap = AtlasPos_t(m_iLevelSt, y, x);
	return true;
}

DWORD CDM2HierMapView::CalcT4w(RN rn, BYTE c, CDMDBRMask &mask)
{
	DWORD x = 0;
	HRESULT hr;
	static const WORD wTblText[] = {0x0040, 0x0080, 0x0100, 0x0200, 0x0400};
	static const WORD wTblActu[] = {0x0800, 0x1000, 0x2000, 0x4000, 0x8000};
	static const WORD wTblItem[] = {0x0004, 0x0008, 0x0010, 0x0020};
	static const WORD wTblCreature[] = {0x0002};
	static const DWORD nTblDoor[] = {
		0x01000000, 0x02000000,
		0x00400000, 0x00800000,
		0x00100000, 0x00200000,
		0x00040000, 0x00080000,
		0x00010000, 0x00020000,
	};
	BYTE t = c / 0x20;
	bool fWall = CDDAT::IsTileWall(c);
	if (m_fDispTileVal && t != 4) {
		if (c & 1) x |= 0x04000000;
		if (c & 2) x |= 0x08000000;
		if (c & 4) x |= 0x10000000;
		if (c & 8) x |= 0x20000000;
	}
	while (rn != RN_NIL && rn != RN_END) {
		if (mask.IsSet(rn))
			break;
		mask.Set(rn);
		DMDBU rec;
		if (FAILED(hr = m_pDDIf->GetDBRecAt(DBIPos_t(rn), rec)))
			break;
		switch (rn.db()) {
		case  0:
			if ((c & 0xE0) == 0x80) {
				int iHorz = (c & 8) ? 1 : 0;
				BYTE f = c & 7;
				if (f <= 4) x |= nTblDoor[2 * f + iHorz];
			}
			break;
		case  2:
			//if ((rec.r2.w2 & 0x0007) == 0x0002) x |= (fWall) ? wTblText[rn.room()] : wTblText[4];
			x |= (fWall) ? wTblText[rn.room()] : wTblText[4];
			break;
		case  3:
		case 11: case 12: case 13:
			x |= fWall ? wTblActu[rn.room()] : wTblActu[4];
			break;
		case  4:
			x |= wTblCreature[0];
			break;
		case  5:
		case  6:
		case  7:
		case  8:
		case  9:
		case 10:
			x |= wTblItem[rn.room()];
			break;
		}
		switch (rn.db()) {
		case 4: case 9: case 14:
			//x |= CalcT4w(rec.r4.stolen, c, mask);
			break;
		}
		rn = rec.r0.next;
	}
	return x;
}

BOOL CDM2HierMapView::ScrollTileXY(int dx, int dy)
{
	CPoint point = GetScrollPosition();

	int n;
	CSize s[3];
	GetDeviceScrollSizes(n, s[0], s[1], s[2]);

	point.x += s[2].cx * dx;
	point.y += s[2].cy * dy;

	ScrollToPosition(point);

	return TRUE;
}

void CDM2HierMapView::MarkBuildTS()
{
//	if (m_fBuildTS) TRACE0("MarkBuildTS AGAIN\n");

	m_fBuildTS = true;
}

BOOL CDM2HierMapView::RemoveTileSpec(AtlasBit_t &ab)
{
	RN rn = ab.r;
	BYTE t = ab.c / 0x20;
	if (rn == RN_NIL) return TRUE;
	RN rno;
	HRESULT hr;
	DMDBU rec, reco;
	for (int i = 0; rn != RN_END; i++) {
		if (false
			|| (t == 4 && rn.db() == 0)
			|| (t == 5 && rn.db() == 1)
		) {
			if (FAILED(hr = m_pDDIf->GetDBRecAt(DBIPos_t(rn), rec)))
				return FALSE;
			if (i == 0) {
				ab.r = rec.r0.next;
				if (ab.r == RN_END)
					ab.r = RN_NIL;
			} else {
				reco.r0.next = rec.r0.next;
				if (FAILED(hr = m_pDDIf->GetDBRecAt(DBIPos_t(rno), reco)))
					return FALSE;
			}
		}
		rno = rn;
		if (FAILED(hr = m_pDDIf->GetDBRecAt(DBIPos_t(rno), reco)))
			return FALSE;
		rn = reco.r0.next;
	}
	return TRUE;
}

BOOL CDM2HierMapView::AddTileSpec(AtlasBit_t &ab)
{
	BYTE t = ab.c / 0x20;
	switch (t) {
	case 4:
	case 5:
		{
			DMDBU rec;
			ZeroMemory(&rec, sizeof(rec));
			rec.r0.next = RN_END;
			if (ab.r != RN_NIL)
				rec.r0.next = ab.r;
			DBIPos_t pos;
			HRESULT hr;
			if (FAILED(hr = m_pDDIf->AddDBRecTo((t == 4) ? 0 : 1, rec, pos)))
				return FALSE;
			ab.r = RNco(pos.db(), pos.idx(), 0);
			return TRUE;
		}
	default:
		{
			return TRUE;
		}
	}
}

void CDM2HierMapView::OnCommand0100()
{
	BYTE c = m_tileEd.GetTileNewVal();

	if (!MkDDIf()) return;

	AtlasBit_t ab;
	HRESULT hr;
	if (SUCCEEDED(hr = m_pDDIf->GetAtlasBitAt(m_apSite, ab))) {
		if (ab.c != c) {
			ab.c = c;
			VERIFY(SUCCEEDED(hr = m_pDDIf->SetAtlasBitAt(m_apSite, ab)));
		}
	}
}

void CDM2HierMapView::OnViewDisplayTileVal() 
{
	m_fDispTileVal = !m_fDispTileVal;

	MarkBuildTS();
	Invalidate();
}

void CDM2HierMapView::OnUpdateViewDisplayTileVal(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_fDispTileVal);
}

void CDM2HierMapView::DrawArrow(CDC *pDC, CPoint p1, CPoint p2, int iArrow, LPCTSTR pszSrcText, LPCTSTR pszDestText)
{
	CPoint pt12 = p2 - p1;

	double x = pt12.x;
	double y = pt12.y;
	double v = sqrt((x * x) + (y * y));
	if (v < EPS) return;
	x /= v;
	y /= v;

	CPoint pt2 = p2;

	if (iArrow == 0) {
		pDC->MoveTo(p1);
		pDC->LineTo(p2);
	} else if (iArrow == 1) {
		p2.x -= (int)(x * 8);
		p2.y -= (int)(y * 8);

		pDC->MoveTo(p1);
		pDC->LineTo(p2);

		CPoint p3a(
			(int)((x * cos(D2R(150)) - y * sin(D2R(150)))*11),
			(int)((x * sin(D2R(150)) + y * cos(D2R(150)))*11)
			);
		CPoint p3b(
			(int)((x * cos(D2R(-150)) - y * sin(D2R(-150)))*11),
			(int)((x * sin(D2R(-150)) + y * cos(D2R(-150)))*11)
			);

		pDC->MoveTo(p2);
		pDC->LineTo(p2 +p3a);
		pDC->MoveTo(p2);
		pDC->LineTo(p2 +p3b);
	} else if (iArrow == 2) {
		pDC->MoveTo(p1);
		pDC->LineTo(p2);

		CRect rc(p2, CSize(0, 0));
		rc.InflateRect(5, 5);
	}

	CPoint ptLeft;
	{
		ptLeft.x = int(x * 15);
		ptLeft.y = int(y * 15);
	}

	if (pszDestText) {
		CRect rc(pt2 + ptLeft, CSize(0, 0));
		rc.InflateRect(20, 20);
		pDC->DrawText(pszDestText, &rc, 0 |DT_SINGLELINE |DT_CENTER |DT_VCENTER);
	}
	if (pszSrcText) {
		CRect rc(p1 + (-ptLeft), CSize(0, 0));
		rc.InflateRect(20, 20);
		pDC->DrawText(pszSrcText, &rc, 0 |DT_SINGLELINE |DT_CENTER |DT_VCENTER);
	}
}

void CDM2HierMapView::BTTile(AtlasPos_t ap)
{
	BYTE z = ap.Z();
	if (m_listLS.GetSize() <= z) return;

	LevelSt &rLS = m_listLS[z];
	CPoint pt = rLS.rc.TopLeft();
	CRect rc;
	GetClientRect(&rc);
	int x1 = rc.Width() / 24;
	int y1 = rc.Height() / 24;
	int ox = _std_max<int>(0, (pt.x + ap.Y() - x1/2));
	int oy = _std_max<int>(0, (pt.y + ap.X() - y1/2));
	m_bthl.pt = CPoint(pt.x + ap.Y(), pt.y + ap.X());
	pt.x = 24*ox;
	pt.y = 24*oy;
	ScrollToPosition(pt);
	SelectLevel(z);
	Invalidate();
}

bool CDM2HierMapView::Deselect()
{
	bool fRedraw = false;

	m_tileEd.Destroy();

	if (m_tarTrack.HasAny()) fRedraw = true;
	m_tarTrack.Clear();

	if (m_bthl.HasAny()) fRedraw = true;
	m_bthl = BTHL();

	if (fRedraw) Invalidate();

	return false;
}

void CDM2HierMapView::OnEditDmFileDesc() 
{
	if (!MkDDIf()) return;

	CEditDM2FileDescDlg wndDlg(m_pDDIf, this);

	int nOldMaps = m_pDDIf->CntMap();

	AllMapDesc_t a;
	HRESULT hr;
	if (FAILED(hr = m_pDDIf->GetAllMapDesc(a))) return;
	wndDlg.Revert(a, true);
	wndDlg.m_nCntMap = nOldMaps;
	if (IDOK == wndDlg.DoModal()) {
		wndDlg.Revert(a, false);
		int nNewMaps = wndDlg.m_nCntMap;
		CDDATUndoLock lock(*m_pDDIf, TRUE);
		VERIFY(SUCCEEDED(hr = m_pDDIf->SetAllMapDesc(a)));

		UINT i;
		int nAdd = nNewMaps - nOldMaps;
		while (nAdd < 0) nAdd++, nOldMaps--, m_pDDIf->DeleteMapAt(nOldMaps);
		while (nAdd > 0) nAdd--, m_pDDIf->AddEmptyMap(i);

		lock.Unlock(TRUE);
		OnReview();
	}
}

void CDM2HierMapView::OnEditLvDesc() 
{
	if (!MkDDIf()) return;
	if (m_iLevelSt < 0) return;

	CEditLvDesc3Dlg wndDlg(this);

	LevelDesc_t s;
	HRESULT hr;
	if (FAILED(hr = m_pDDIf->GetMapDescAt(m_iLevelSt, s))) return;
	wndDlg.Revert(s, true);
	if (IDOK == wndDlg.DoModal()) {
		wndDlg.Revert(s, false);
		VERIFY(SUCCEEDED(hr = m_pDDIf->SetMapDescAt(m_iLevelSt, s)));

		m_iLv = s.iLv;
		OnReview();
		MarkBuildTS();
	}
}

void CDM2HierMapView::OnUpdateEditLvDesc(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(0 <= m_iLevelSt);
}

void CDM2HierMapView::OnEditAddMap() 
{
	if (!MkDDIf()) return;

	CDDATUndoLock lock(*m_pDDIf, TRUE);

	CRect rc;
	GetClientRect(&rc);

	int cx = rc.Width() / 24;
	int cy = rc.Height() / 24;

	CPoint pt = GetDeviceScrollPosition();

	int x = pt.x / 24;
	int y = pt.y / 24;

	int cxMax = _MIN(cx, _MIN(cy, 20));

	HRESULT hr;
	UINT iMap;
	if (FAILED(hr = m_pDDIf->AddEmptyMap(iMap))) return;
	LevelDesc_t s;
	if (FAILED(hr = m_pDDIf->GetMapDescAt(iMap, s))) return;
	s.cxLv = s.cyLv = cxMax;
	s.nShiftX = x + (cx - cxMax) / 2;
	s.nShiftY = y + (cy - cxMax) / 2;
	s.n14_4_7 = 3;
	s.iLv = m_iLv;
	if (FAILED(hr = m_pDDIf->SetMapDescAt(iMap, s))) return;
#if 0
	Atlas_t a;
	if (FAILED(hr = m_pDDIf->GetMapAtlasAt(iMap, a))) return;
	for (int y1 = 0; y1 < cxMax; y1++) 
		for (int x1 = 0; x1 < cxMax; x1++)
			a.c[y1][x1] = (y1 == 0 || x1 == 0 || y1 == cxMax - 1 || x1 == cxMax - 1) ? 0x00 : 0x20;
	if (FAILED(hr = m_pDDIf->SetMapAtlasAt(iMap, a))) return;
#endif

	lock.Unlock(TRUE);

	OnReview();

	BuildTS();
	SelectLevel(iMap);
}

void CDM2HierMapView::OnReview()
{
	if (!MkMainFrmIf()) return;
	if (!MkDDIf()) return;

	m_pMainFrmIf->SetVwLv(m_iLv);
	m_pMainFrmIf->SetLvCnt(m_pDDIf->CntMap());
}

void CDM2HierMapView::OnViewTlv0(UINT nID) 
{
	switch (nID) {
	case ID_VIEW_TLV0: m_ttlv.nDepth = 0; break;
	case ID_VIEW_TLV1: m_ttlv.nDepth = 1; break;
//	case ID_VIEW_TLV2: m_ttlv.nDepth = 2; break;
//	case ID_VIEW_TLV3: m_ttlv.nDepth = 3; break;
//	case ID_VIEW_TLVX: m_ttlv.nDepth = INT_MAX; break;
//	case ID_VIEW_TI_NORM: m_ttlv.fNorm = !m_ttlv.fNorm; break;
//	case ID_VIEW_TI_BACKTRACE: m_ttlv.fBack = !m_ttlv.fBack; break;
	case ID_VIEW_TL_DEEPBACK: m_ttlv.fDeepBack = !m_ttlv.fDeepBack; break;
	}
}

void CDM2HierMapView::OnUpdateViewTlv0(CCmdUI* pCmdUI) 
{
	bool fRadio = false, fCheck = false;
	switch (pCmdUI->m_nID) {
	case ID_VIEW_TLV0: fRadio = (m_ttlv.nDepth == 0); break;
	case ID_VIEW_TLV1: fRadio = (m_ttlv.nDepth == 1); break;
//	case ID_VIEW_TLV2: fRadio = (m_ttlv.nDepth == 2); break;
//	case ID_VIEW_TLV3: fRadio = (m_ttlv.nDepth == 3); break;
//	case ID_VIEW_TLVX: fRadio = (m_ttlv.nDepth >= 4); break;
//	case ID_VIEW_TI_NORM: fCheck = m_ttlv.fNorm ? true : false; break;
//	case ID_VIEW_TI_BACKTRACE: fCheck = m_ttlv.fBack ? true : false; break;
	case ID_VIEW_TL_DEEPBACK: fCheck = m_ttlv.fDeepBack ? true : false; break;
	}
	if (fRadio) {
		pCmdUI->SetRadio(true);
	} else if (fCheck) {
		pCmdUI->SetCheck(true);
	} else {
		pCmdUI->SetCheck(false);
	}
}

void CDM2HierMapView::OnEditOptimize() 
{
	if (!MkDDIf()) return;

	int r = AfxMessageBox(IDS_WARN_OPTS, 0 |MB_YESNO |MB_ICONEXCLAMATION);
	if (r != IDYES) return;

	CWaitCursor wc;
	HRESULT hr;
	VERIFY(SUCCEEDED(hr = m_pDDIf->Optimize()));

	if (FAILED(hr)) {
		AfxMessageBox(IDS_ERR_OPT_FLATTEN_OOR, 0 |MB_ICONEXCLAMATION);
	} else {
		AfxMessageBox(IDS_OK_OPTS, 0 |MB_ICONINFORMATION);
	}
}

void CDM2HierMapView::OnEditMkTopvw() 
{
	if (!MkDDIf()) return;

	TRY
		CExtBM bmScene[16];
		CExtBM bmMapChip[2];

		CWaitCursor wc;
		CCurDir wd;
		SetCurrentDirectory(GetApp()->GetWorkDir());

		bool fScenes[16];

		{
			LPCTSTR pszScene[16] = {
				"_Scene0.dib", "_Scene1.dib", "_Scene2.dib", "_Scene3.dib",
				"_Scene4.dib", "_Scene5.dib", "_Scene6.dib", "_Scene7.dib",
				"_Scene8.dib", "_Scene9.dib", "_SceneA.dib", "_SceneB.dib",
				"_SceneC.dib", "_SceneD.dib", "_SceneE.dib", "_SceneF.dib",
			};
			for (int i = 0; i < 16; i++) fScenes[i] = bmScene[i].Attach((HBITMAP)::LoadImage(NULL, pszScene[i], IMAGE_BITMAP, 0, 0, 0 |LR_CREATEDIBSECTION |LR_LOADFROMFILE |LR_VGACOLOR)) ? true : false;

			CHUGHUG HH(GetApp()->GetWorkDir());

			VERIFY(bmMapChip[0].Attach((HBITMAP)::LoadImage(NULL, HH.Extraction("_MapChip_0.rle"), IMAGE_BITMAP, 0, 0, 0 |LR_CREATEDIBSECTION |LR_LOADFROMFILE |LR_VGACOLOR)));
			VERIFY(bmMapChip[1].Attach((HBITMAP)::LoadImage(NULL, HH.Extraction("_MapChip_1.rle"), IMAGE_BITMAP, 0, 0, 0 |LR_CREATEDIBSECTION |LR_LOADFROMFILE |LR_VGACOLOR)));
		}

		CExtBM bmMap;

		int xMax = 0;
		int yMax = 0;
		int xMin = INT_MAX;
		int yMin = INT_MAX;

		UINT iMap, nMaps = m_pDDIf->CntMap();
		HRESULT hr;
		BTileSt ts[289][289];
		for (iMap = 0; iMap < nMaps; iMap++) {
			LevelDesc_t s;
			VERIFY(SUCCEEDED(hr = m_pDDIf->GetMapDescAt(iMap, s)));
			if (s.iLv != m_iLv) continue;
			int tx = s.nShiftX;
			int ty = s.nShiftY;
			int cx = s.cyLv;
			int cy = s.cxLv;
			xMin = _MIN(xMin, tx);
			yMin = _MIN(yMin, ty);
			xMax = _MAX(xMax, tx + cx);
			yMax = _MAX(yMax, ty + cy);
		}

		int cxMap = xMax - xMin + 2;
		int cyMap = yMax - yMin + 2;

		if (!bmMap.Create(CSize(8*cxMap, 8*cyMap), 24, NULL)) return;

		CCompatDC dcMap;
		CGdiObject *pOldMapBM = dcMap.SelectObject(&bmMap);
		CCompatDC dcScene;
		CGdiObject *pOldSceneBM = dcScene.GetCurrentBitmap();
		CCompatDC dcMapChip;
		CGdiObject *pOldMapChipBM = dcMapChip.SelectObject(&bmMapChip[0]);
		CCompatDC dcMapChipMask;
		CGdiObject *pOldMapChipMaskBM = dcMapChipMask.SelectObject(&bmMapChip[1]);

		for (iMap = 0; iMap < nMaps; iMap++) {
			LevelDesc_t s;
			VERIFY(SUCCEEDED(hr = m_pDDIf->GetMapDescAt(iMap, s)));
			if (s.iLv != m_iLv) continue;
			int tx = s.nShiftX;
			int ty = s.nShiftY;
			int cx = s.cxLv;
			int cy = s.cyLv;

			BYTE iArray[45];
			char iWallOrnate = 0;
			char iFloorOrnate = s.nWallOrnateIndices;
			{
				UINT i, x = 0;
				for (i = 0; i < s.nWallOrnateIndices; i++, x++) iArray[x] = s.bWallOrnateIndices[i];
				for (i = 0; i < s.nFloorOrnateIndices; i++, x++) iArray[x] = s.bFloorOrnateIndices[i];
				for (i = 0; i < s.nSharedOrnateIndices; i++, x++) iArray[x] = s.bSharedOrnateIndices[i];
				for (; x < sizeof(iArray); x++) iArray[x] = -1;
			}

			Atlas_t a;
			VERIFY(SUCCEEDED(hr = m_pDDIf->GetMapAtlasAt(iMap, a)));

			char iScene = s.n14_4_7 & 15;

			dcScene.SelectObject(&bmScene[iScene]);

			static const int nTblComp[4][3][2] = {
				{{-1,-1},{ 0,-1},{+1,-1},}, // North
				{{+1,-1},{+1, 0},{+1,+1},}, // East
				{{-1,+1},{ 0,+1},{+1,+1},}, // South
				{{-1,-1},{-1, 0},{-1,+1},}, // West
			};

			for (UINT y = 0; y < cx; y++) {
				bool fVertMin = (y == 0);
				bool fVertMax = (y == cx - 1);
				for (UINT x = 0; x < cy; x++) {
					bool fHorzMin = (x == 0);
					bool fHorzMax = (x == cy - 1);

					int px = 1 +tx +x -xMin;
					int py = 1 +ty +y -yMin;
					int vx = 8 * px;
					int vy = 8 * py;
					BTileSt &rts = ts[py][px];

					if (0 <= rts.iMap) continue;

					bool fWestBlank = (x != 0 && a.c[x][y - 1] / 0x20 != 0);
					bool fNorthBlank = (y != 0 && a.c[x - 1][y] / 0x20 != 0);

					BYTE c = a.c[x][y];
					BYTE t = c / 0x20;
					RN rn = a.r[x][y];

					int v = -1;

					switch (t) {
					case 0: v = 0; break;
					case 1: v = 2; break;
					case 2: v = (c & 8) ? ((c & 4) ? 4 : 3) : 2; break;
					case 3:
						{
							if (c & 4) {
								// Up
								if (c & 8) {
									// North-South
									v = fNorthBlank ? 2 : 0;
								} else {
									// West-East
									v = fWestBlank ? 1 : 3;
								}
							} else {
								// Down
								if (c & 8) {
									// North-South
									v = fNorthBlank ? 0 : 2;
								} else {
									// West-East
									v = fWestBlank ? 3 : 1;
								}
							}
							v += 6;
							break;
						}
					case 4:
						{
							bool fOpen = (c & 7) == 0;
							bool fVert = (c & 8) == 0;
							bool fSw = VwMap_DoorSw(rn);
							v = 0
								|(fOpen ? 4 : 0)
								|(fSw ? 0 : 2)
								|(fVert ? 1 : 0)
								;
							v += 10;
							break;
						}
					case 5:
						{
							bool fShow = VwMap_TeleF(rn);
							v = fShow ? 2 : -1;
							break;
						}
					case 6: v = (c & 4) ? 2 : ((c & 1) ? 1 : 0); break;
					}

					if (0 <= v) {
						if (fScenes[iScene]) {
							dcMap.BitBlt(vx, vy, 7, 7, &dcScene, 7 * v, 0, SRCCOPY);

							switch (t) {
							case 0:
							case 1:
							case 2:
							case 5:
								{
									BYTE iOrnate[8] = {-1,-1,-1,-1,-1,-1,-1,-1};
									bool fWall = (t == 0);
									BYTE iCreature = -1;
									VwMap_ScanOrnate(rn, &iArray[fWall ? iWallOrnate : iFloorOrnate], iOrnate, iOrnate +4, fWall);

									int i;
									for (i = 0; i < 4; i++) {
										BYTE x = iOrnate[i];
										if (x == 0xFF) continue;

										int spx = (x % 32) * 16 + ((i & 1) ? 8 : 0);
										int spy = (x / 32) * 8 + (fWall ? 0 : 64);

										Blit7x7(dcMap, vx, vy, dcMapChip, spx, spy, dcMapChipMask, spx, spy, 16 + ((i & 2) ? 12 : 0));
									}
									for (i = 0; i < 4; i++) {
										BYTE x = iOrnate[4 +i];
										if (x == 0xFF) continue;

										int spx = (x % 32) * 16;
										int spy = (x / 32) * 8 + 128;

										Blit7x7(dcMap, vx, vy, dcMapChip, spx, spy, dcMapChipMask, spx, spy, i + 16);
									}
									break;
								}
							}

							if (t != 0 && (fVertMin || fVertMax || fHorzMin || fHorzMax)) {
								for (int iDis = 0; iDis < 4; iDis++) {
									if (iDis == 0 && !fVertMin) continue;
									if (iDis == 1 && !fHorzMax) continue;
									if (iDis == 2 && !fVertMax) continue;
									if (iDis == 3 && !fHorzMin) continue;
									for (int iTri = 0; iTri < 3; iTri++) {
										int mx = nTblComp[iDis][iTri][0];
										int my = nTblComp[iDis][iTri][1];
										BTileSt &rts = ts[py +my][px +mx];
										if (rts.iMap < 0) {
											int tvx = vx + 8 * mx;
											int tvy = vy + 8 * my;
											dcMap.BitBlt(tvx, tvy, 7, 7, &dcScene, 7 * 0, 0, SRCCOPY);
										}
									}
								}
							}
						}

						rts.iMap = iMap;
					}
				}
			}
		}

		dcMap.SelectObject(pOldMapBM);
		dcScene.SelectObject(pOldSceneBM);
		dcMapChip.SelectObject(pOldMapChipBM);
		dcMapChipMask.SelectObject(pOldMapChipMaskBM);

		CString strF; strF.Format("World top-view(Lv%02u).bmp", m_iLv);

		CFileDialog wndDlg(FALSE, _T("bmp"), strF, nDefSaveFlags, _T("BMP (*.bmp)|*.bmp|"), this);

		if (wndDlg.DoModal() == IDOK) {
			if (bmMap.SaveToFile(wndDlg.GetPathName())) {
				if (AfxMessageBox(IDS_ASK_WOULD_YOU_OPEN, MB_YESNO) == IDYES) {
					ShellExecute(*this, NULL, wndDlg.GetPathName(), NULL, NULL, SW_SHOW);
				}
			}
		}
	CATCH_ALL(e)

	END_CATCH_ALL
}

bool CDM2HierMapView::VwMap_DoorSw(RN rn)
{
	HRESULT hr;
	while (rn != RN_END) {
		ASSERT(rn != RN_NIL);
		DMDBU rec;
		if (FAILED(hr = m_pDDIf->GetDBRecAt(DBIPos_t(rn), rec)))
			return false;
		if (rn.db() == 0) {
			return (rec.r0.w2 & (1 << 6)) != 0;
		}
		rn = rec.r0.next;
	}
	return false;
}

bool CDM2HierMapView::VwMap_TeleF(RN rn)
{
	HRESULT hr;
	while (rn != RN_END) {
		ASSERT(rn != RN_NIL);
		DMDBU rec;
		if (FAILED(hr = m_pDDIf->GetDBRecAt(DBIPos_t(rn), rec)))
			return true;
		if (rn.db() == 1) {
			return (rec.r1.w4 & 0xFF) != 1;
		}
		rn = rec.r1.next;
	}
	return true;
}

bool CDM2HierMapView::VwMap_ScanOrnate(RN rn, const BYTE iArray[15], BYTE iOrnate[4], BYTE iCreature[4], bool fWall)
{
	if (rn == RN_NIL) return false;
	HRESULT hr;
	while (rn != RN_END) {
		ASSERT(rn != RN_NIL);
		DMDBU rec;
		if (FAILED(hr = m_pDDIf->GetDBRecAt(DBIPos_t(rn), rec)))
			return true;
		switch (rn.db()) {
		case 2:
			{
				UINT x = 0
					|(rec.r2.w2 & 7)
					|(rec.r2.w2 >> 8)
					;
				switch (x | 1) {
				case 3:
				case 19:
				case 33:
				case 41:
				case 49:
				case 51:
				case 57:
				case 59:
				case 65:
				case 105:
					iOrnate[fWall ? rn.room() : 0] = (rec.r2.w2 / 8) & 0xFF;
					break;
				}
				break;
			}
		case 3:
		case 11: case 12: case 13:
			{
				BYTE i = rec.r3.w4 / 0x1000;
				if (i != 0) {
					iOrnate[fWall ? rn.room() : 0] = iArray[i -1];
				}
				break;
			}
		case 4:
			{
				if (!fWall) {
					iCreature[(rec.r4.w14 / 0x0100) & 3] = rec.r4.b4;
				}
				break;
			}
		}
		rn = rec.r3.next;
	}
	return true;
}

void CDM2HierMapView::Blit7x7(CDC &dcTo, int tx, int ty, CDC &dcFrom, int sx, int sy, CDC &dcMask, int mx, int my, int f)
{
	for (int py = 0; py < 7; py++) {
		for (int px = 0; px < 7; px++) {
			int tpx = (f & 4) ? (6 - px) : px;
			int tpy = (f & 8) ? (6 - py) : py;
			COLORREF clr1 = dcFrom.GetPixel(sx+tpx, sy+tpy);
			COLORREF clr2 = dcMask.GetPixel(sx+tpx, my+tpy);
			if (f & 16) {
				if (clr2 == 0) continue;
			} else {
				if (clr2 != 0) continue;
			}
			int wx = px;
			int wy = py;
			switch (f & 3) {
			case 1: wx = 6 - py, wy =     px; break;
			case 2: wx = 6 - px, wy = 6 - py; break;
			case 3: wx =     py, wy = 6 - px; break;
			}
			dcTo.SetPixel(tx+wx, ty+wy, clr1);
		}
	}
}

void CDM2HierMapView::OnEditOptimizeFlat() 
{
	if (!MkDDIf()) return;

	int r = AfxMessageBox(IDS_WARN_OPTS_FLATTEN, 0 |MB_OKCANCEL |MB_ICONEXCLAMATION);
	if (r != IDOK) return;

	CWaitCursor wc;
	HRESULT hr = m_pDDIf->OptimizeFlatten();

	switch (hr) {
	case S_OK:
		AfxMessageBox(IDS_OK_OPTS, 0 |MB_ICONINFORMATION);
		return;
	case DMERR_NO_MORE_SPACE:
		AfxMessageBox(IDS_ERR_OPT_FLATTEN, 0 |MB_ICONEXCLAMATION);
		return;
	case DMERR_DBREC_OUT_OF_RANGE:
		AfxMessageBox(IDS_ERR_OPT_FLATTEN_OOR, 0 |MB_ICONEXCLAMATION);
		return;
	default:
		AfxMessageBox(IDS_ERR_OPT_FLATTEN_GEN, 0 |MB_ICONEXCLAMATION);
		return;
	}
}

void CDM2HierMapView::OnEditRotyN(UINT nID) 
{
	if (!MkDDIf()) return;
	if (m_iLevelSt < 0) return;

	int vx = 0, vy = 0;
	switch (nID) {
	case ID_EDIT_ROTX_N: vy = -1; break;
	case ID_EDIT_ROTX_P: vy = +1; break;
	case ID_EDIT_ROTY_N: vx = -1; break;
	case ID_EDIT_ROTY_P: vx = +1; break;
	}

	m_pDDIf->RotateMapAtlas(m_iLevelSt, vx, vy);
	MarkBuildTS();
}

void CDM2HierMapView::OnUpdateEditRotyN(CCmdUI* pCmdUI) 
{
	if (!MkDDIf()) return;

	bool fEnabled = false;
	switch (pCmdUI->m_nID) {
	case ID_EDIT_ROTX_N:
	case ID_EDIT_ROTX_P:
	case ID_EDIT_ROTY_N:
	case ID_EDIT_ROTY_P:
		{
			CDMEdCtxAccessibility a;
			m_pEdCtx->GetAccessibility(a);
			fEnabled = (a.bLimitless && 0 <= m_iLevelSt) ? true : false;
			break;
		}
	}
	pCmdUI->Enable(fEnabled);
}

BOOL CDM2HierMapView::OnReloadOrnates(UINT nID)
{
	m_offsc.Create24Screen();
	m_offsc.Clear();

	ZeroMemory(m_cVram1, sizeof(m_cVram1));
	ZeroMemory(m_cVram2, sizeof(m_cVram2));

	return false;
}

DROPEFFECT CDM2HierMapView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	return OnDragOver(pDataObject, dwKeyState, point);
}

void CDM2HierMapView::OnDragLeave()
{
	CPoint pto = GetDeviceScrollPosition();
	CRect rc = CRect(CPoint(m_ptDnDcur.x*24-pto.x, m_ptDnDcur.y*24-pto.y), CSize(24,24));
	InvalidateRect(rc);
	m_ptDnDcur = CPoint(-1, -1);
}

BOOL CDM2HierMapView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	if (pDataObject->IsDataAvailable(CTsDataSource::cfShowcase) && IsTileInSel(point) != NULL) {
		CTsDataObject obj;
		obj.Attach(pDataObject->Detach());
		ShowcaseData sc;
		if (obj.GetShowcase(sc)) {
			if (!AddShowcaseEntityTo(AtlasPos_t(m_tileInSel.lv, m_tileInSel.y, m_tileInSel.x), sc))
				AfxMessageBox(IDS_ERR_DBFULL, 0 |MB_ICONEXCLAMATION);
			return true;
		}
	}
	return false;
}

DROPEFFECT CDM2HierMapView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	if (pDataObject->IsDataAvailable(CTsDataSource::cfShowcase) && IsTileInSel(point) != NULL) {
		return DROPEFFECT_COPY;
	}

	return DROPEFFECT_NONE;
}

CDM2HierMapView::TileInSel *CDM2HierMapView::IsTileInSel(POINT point)
{
	if (m_iLevelSt < 0)
		return NULL;

	CPoint pto = GetDeviceScrollPosition();
	CPoint pt = pto + point;
	CPoint ptOldDnD = m_ptDnDcur;
	int r = m_rct.HitTest(pt);
	if (r == CRectTracker::hitMiddle) {
		UINT tx = pt.x / 24;
		UINT ty = pt.y / 24;
		TileSt &ts = m_aTS[ty % 287][tx % 287];
		if (m_iLevelSt == ts.iMap) {
			if (tx != m_ptDnDcur.x || ty != m_ptDnDcur.y) {
				CRect rc(CPoint(tx*24-pto.x, ty*24-pto.y), CSize(24, 24));
				CClientDC dc(this);
				CBrush br;
				br.CreateSolidBrush(RGB(255, 255, 0));
				dc.FrameRect(rc, &br);
				rc.DeflateRect(1, 1);
				dc.FrameRect(rc, &br);

				rc = CRect(CPoint(ptOldDnD.x*24-pto.x, ptOldDnD.y*24-pto.y), CSize(24,24));
				InvalidateRect(rc);

				m_ptDnDcur.x = tx;
				m_ptDnDcur.y = ty;
			}

			m_tileInSel.lv = m_iLevelSt;
			m_tileInSel.x = (pt.x - m_rct.m_rect.left) / 24;
			m_tileInSel.y = (pt.y - m_rct.m_rect.top) / 24;

			return &m_tileInSel;
		}
		if (ptOldDnD.x != -1) {
			CRect rc = CRect(CPoint(ptOldDnD.x*24-pto.x, ptOldDnD.y*24-pto.y), CSize(24,24));
			InvalidateRect(rc);
			m_ptDnDcur = CPoint(-1, -1);
		}
	}
	return NULL;
}

bool CDM2HierMapView::AddShowcaseEntityTo(AtlasPos_t pos, ShowcaseData sc)
{
	HRESULT hr;
	CDDAT &rDD = *m_pEdCtx->GetDDIf();

	CDDATUndoLock lock(rDD, TRUE);
	//
	BYTE t = sc.t;
	BYTE tid = sc.tid;

	int textPos = 0;

	if (t == 0xff) {
		CTextInputDlg wndDlg;
		if (IDOK != wndDlg.DoModal())
			return true;
		wndDlg.m_strText.Replace("\r\n", "\n");
		AllMapDesc_t a;
		if (FAILED(hr = rDD.GetAllMapDesc(a)))
			return false;
		//
		DMTextEdit_t texte;
		texte.Load(a.fvTextData);

		textPos = texte.AddStr(wndDlg.m_strText);

		if (!texte.Save(a.fvTextData))
			return false;
		//
		VERIFY(SUCCEEDED(hr = rDD.SetAllMapDesc(a)));

		if (tid == 0) {
			t = 2;
		} else {
			t = 7;
		}
	}

	switch (t) {
	case 4: case 5: case 6: case 8: case 9: case 10: case 2: case 7:
		DMDBU rec;
		ZeroMemory(&rec, sizeof(rec));
		rec.r5.next = RN_END;
		rec.r5.w2 = tid;

		switch (t) {
		case 4:
			rec.r4.b4 = tid;
			rec.r4.stolen = RN_END;
			rec.r4.b5 = 0xff;
			rec.r4.w6[0] = 100;
			rec.r4.w14 = 0x400;
			break;
		case 5: case 6: case 10:
			rec.r5.w2 = tid + 0x400 * 5;
			break;
		case 8:
			rec.r8.w2 = (tid << 8) | 0x7f;
			break;
		case 9:
			rec.r9.w4 = CExp00069App::ID2CHES(tid);
			rec.r9.stolen = RN_END;
			break;
		case 2:
			rec.r2.w2 = 1 | (textPos << 3);
			break;
		case 7:
			{
				DMDBU rectext;
				rectext.r2.next = RN_END;
				rectext.r2.w2 = 1 | (textPos << 3);
				DBIPos_t rpos;
				if (FAILED(hr = rDD.AddDBRecTo(2, rectext, rpos)))
					return false;

				rec.r7.w2 = rpos.idx();
			}
		}

		DBIPos_t rpos;
		if (FAILED(hr = rDD.AddDBRecTo(t, rec, rpos)))
			return false;

		RN rnlast = RNco(rpos.db(), rpos.idx(), sc.cDir);

		AtlasBit_t ab;
		if (FAILED(hr = rDD.GetAtlasBitAt(pos, ab)))
			return false;
		if (ab.r == RN_NIL) {
			ab.c |= 0x10;
			ab.r = rnlast;
			VERIFY(SUCCEEDED(hr = rDD.SetAtlasBitAt(pos, ab)));
		}
		else {
			RN rn = ab.r;
			while (true) {
				DMDBU rec2;
				if (FAILED(hr = rDD.GetDBRecAt(DBIPos_t(rn), rec2)))
					return false;
				if (rec2.r0.next == RN_END) {
					rec2.r0.next = rnlast;
					VERIFY(SUCCEEDED(hr = rDD.SetDBRecAt(DBIPos_t(rn), rec2)));
					break;
				}
				rn = rec2.r0.next;
			}
		}
		break;
	}
	//
	lock.Unlock(true);
	return true;
}

void CDM2HierMapView::OnOptSettle() 
{
}

void CDM2HierMapView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar) {
	case VK_SPACE:
		m_fSpcMove = false;
		break;
	}
	
	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CDM2HierMapView::OnViewSnapToTL() 
{
	m_fSnapTL = !m_fSnapTL;
}

void CDM2HierMapView::OnUpdateViewSnapToTL(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_fSnapTL);
}

void CDM2HierMapView::OnEditBuildlabyrinth()
{
	HRESULT hr;
	if (m_iLevelSt < 0)
		return;
	srand(time(NULL));
	CBuildLabuDlg wndDlg(this);
	while (true) {
		if (wndDlg.DoModal() != IDOK)
			return;
		if (!MkDDIf())
			return;
		CDDAT &d = *m_pDDIf;
		LevelDesc_t a;
		if (FAILED(hr = d.GetMapDescAt(m_iLevelSt, a)))
			return;
		Atlas_t g;
		if (FAILED(hr = d.GetMapAtlasAt(m_iLevelSt, g)))
			return;
		Maze m(a.cyLv/2, a.cxLv/2);
		switch (wndDlg.m_iMethod) {
			case 0:
				m.KabeTaoshi();
				break;
			case 1:
				m.Anahori(wndDlg.m_iRetire);
				break;
		}
		for (int y=0; y<a.cxLv; y++) {
			for (int x=0; x<a.cyLv; x++) {
				g.c[y][x] = m.Get(x, y) ? 0x00 : 0x20;
			}
		}
		int men = a.cyLv * 1 * a.cxLv;
		for (int c=0; c<men/10 ; c++) {
			int x = rand() % (a.cyLv/2);
			int y = rand() % (a.cxLv/2);
			DBIPos_t pos;
			DMDBU rec;
			rec.r3.next = RN_END;
			rec.r3.w2 = 0x08;
			rec.r3.w4 = 0<<12;
			rec.r3.w6 = rand() % 1000;
			if (S_OK == (hr = d.AddDBRecToEmpty(dbActuator, rec, pos))) {
				g.r[y][x] = RNco(dbActuator, pos.idx(), 0);
			}
		}
		d.SetMapAtlasAt(m_iLevelSt, g);
	}
}

void CDM2HierMapView::OnOptAssets(UINT nID) {
	switch (nID) {
		case ID_OPT_ASSETS_DM2: GetApp()->SetAssets("DM2"); break;
		case ID_OPT_ASSETS_DM2BETA: GetApp()->SetAssets("DM2Beta"); break;
		case ID_OPT_ASSETS_DM2TELOS: GetApp()->SetAssets("DM2TELOS"); break;
	}
}

void CDM2HierMapView::OnUpdateOptAssets(CCmdUI *p) {
	switch (p->m_nID) {
		case ID_OPT_ASSETS_DM2: p->SetRadio(GetApp()->GetAssets() == "DM2"); break;
		case ID_OPT_ASSETS_DM2BETA: p->SetRadio(GetApp()->GetAssets() == "DM2Beta"); break;
		case ID_OPT_ASSETS_DM2TELOS: p->SetRadio(GetApp()->GetAssets() == "DM2TELOS"); break;
	}
}
