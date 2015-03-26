// MainFrm.cpp : CMainFrame クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "Exp00069.h"

#include "MainFrm.h"
#include "Exp00069Doc.h"
#include "TriViewNe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_EN_CHANGE(IDC_EDIT_LVCX, OnChangeEditLvcx)
	ON_EN_CHANGE(IDC_EDIT_LVCY, OnChangeEditLvcy)
	ON_WM_QUERYNEWPALETTE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_BTN_CURSOR, ID_BTN_ERASE_TILE_REC, OnBtnCursor)
	ON_UPDATE_COMMAND_UI_RANGE(ID_BTN_CURSOR, ID_BTN_ERASE_TILE_REC, OnUpdateBtnCursor)

#define ON_AUTO_UPDATE_CONTROL_BAR(ID) \
	ON_UPDATE_COMMAND_UI(ID, OnUpdateControlBarMenu) \
	ON_COMMAND_EX(ID, OnBarCheck)

	ON_AUTO_UPDATE_CONTROL_BAR(IDW_DIALOGBAR)
	ON_AUTO_UPDATE_CONTROL_BAR(IDW_EDITLVBAR)
	ON_AUTO_UPDATE_CONTROL_BAR(IDW_DMOBBAR)
	ON_AUTO_UPDATE_CONTROL_BAR(IDW_DMLVSELBAR)
	ON_AUTO_UPDATE_CONTROL_BAR(IDW_DMCIBAR)
	ON_AUTO_UPDATE_CONTROL_BAR(IDW_SHOWCASEBAR)
	ON_AUTO_UPDATE_CONTROL_BAR(IDW_FINDERBAR)

	ON_COMMAND(IDW_DMLVSEL2BAR, OnDMLvSel2BarSelChange)
	ON_COMMAND(IDW_DMMAPSEL2BAR, OnDMMapSel2BarSelChange)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケータ
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの構築/消滅

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_iEditLvTool = 0;
	m_iEditToolF = 0;

	CExp00069Doc *pDocument = static_cast<CExp00069Doc *>(((CCreateContext *)lpCreateStruct->lpCreateParams)->m_pCurrentDoc);

	//---
	if (false
		|| !m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_ALIGN_TOP, CRect(0, 0, 0, 0), AFX_IDW_TOOLBAR)
		|| !m_wndToolBar.LoadToolBar(IDR_MAINFRAME)
	) {
		return -1;
	}
	m_wndToolBar.SetHeight(26);
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle()|CBRS_TOOLTIPS);

	//---
#if 0
	if (false
		|| !m_wndLvDlgBar.Create(this, WS_CHILD|WS_VISIBLE|CBRS_ALIGN_TOP, IDW_DMLVSELBAR)
	) {
		return -1;
	}

	{
		CWnd *pWnd;
		if (pWnd = m_wndLvDlgBar.m_wndLvMetrBar.GetDlgItem(IDC_SPIN_LVCX)) {
			pWnd->SendMessage(UDM_SETRANGE, 0, MAKELONG(1, 32));
		}
		if (pWnd = m_wndLvDlgBar.m_wndLvMetrBar.GetDlgItem(IDC_SPIN_LVCY)) {
			pWnd->SendMessage(UDM_SETRANGE, 0, MAKELONG(1, 32));
		}
	}
#endif

	//---
	if (false
		|| !m_wndEditLvToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_ALIGN_TOP, CRect(0, 0, 0, 0), IDW_EDITLVBAR)
		|| !m_wndEditLvToolBar.LoadToolBar(IDR_EDITLV)
	) {
		return -1;
	}
	m_wndEditLvToolBar.SetHeight(26);
	m_wndEditLvToolBar.SetBarStyle(m_wndEditLvToolBar.GetBarStyle()|CBRS_TOOLTIPS);

	//---
	if (false
		|| !m_wndMapSelBar.Create(this, WS_CHILD|WS_VISIBLE, CBRS_TOP|CBRS_SIZE_DYNAMIC, IDW_DMMAPSEL2BAR)
	) {
		return -1;
	}

	//---
	if (false
		|| !m_wndReBar.Create(this)
		|| !m_wndReBar.AddBar(&m_wndToolBar)
		|| !m_wndReBar.AddBar(&m_wndEditLvToolBar)
//		|| !m_wndReBar.AddBar(&m_wndLvDlgBar)
		|| !m_wndReBar.AddBar(&m_wndMapSelBar)
	) {
		return -1;
	}

	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_STYLE;
	m_wndReBar.GetReBarCtrl().GetBandInfo(1, &rbbi);
	rbbi.fStyle |= RBBS_BREAK;
	m_wndReBar.GetReBarCtrl().SetBandInfo(1, &rbbi);

	m_wndReBar.GetReBarCtrl().GetBandInfo(2, &rbbi);
	rbbi.fStyle |= RBBS_BREAK;
	m_wndReBar.GetReBarCtrl().SetBandInfo(2, &rbbi);

	//---
	if (false
		|| !m_wndStatusBar.Create(this)
		|| !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT))
	) {
		return -1;
	}

	//---
	if (false
		|| !m_wndDMObBar.Create(pDocument->GetEdCtx(FALSE), this, "Object Tree", WS_CHILD|CBRS_SIZE_DYNAMIC, IDW_DMOBBAR)
	) {
		return -1;
	}
	m_wndDMObBar.EnableDocking(CBRS_ALIGN_ANY);

	//---
	if (false
		|| !m_wndShowcaseBar.Create("DRAG'n'DROP Showcase.", this, IDW_SHOWCASEBAR, 0 |CBRS_LEFT |WS_CHILD |WS_VISIBLE |CBRS_SIZE_DYNAMIC)
	) {
		return -1;
	}
	m_wndShowcaseBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndShowcaseBar.m_frame.m_vw.SetDMEdCtx(pDocument->GetEdCtx(false));

	//---
	if (false
		|| !m_wndFinderBar.Create("Finder.", this, IDW_FINDERBAR, 0 |CBRS_BOTTOM |WS_CHILD |WS_VISIBLE |CBRS_SIZE_DYNAMIC)
	) {
		return -1;
	}
	m_wndFinderBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndFinderBar.m_frm.m_vw.SetDMEdCtx(pDocument->GetEdCtx(false));

//	//---
//	if (false
//		|| !m_wndCIBar.Create(pDocument->GetEdCtx(FALSE), this, "Logging", WS_CHILD|WS_VISIBLE|CBRS_SIZE_DYNAMIC, IDW_DMCIBAR)
//	) {
//		return -1;
//	}
//	m_wndCIBar.EnableDocking(CBRS_ORIENT_ANY);

	//---
	if (false
		|| !m_wndLvSelBar.Create(this, WS_CHILD|WS_VISIBLE, CBRS_LEFT|CBRS_SIZE_DYNAMIC, IDW_DMLVSEL2BAR)
	) {
		return -1;
	}
	m_wndLvSelBar.SetCount(16);

	//---

	EnableDocking(CBRS_ALIGN_ANY);

#ifdef _SCB_REPLACE_MINIFRAME
    m_pFloatingFrameClass = RUNTIME_CLASS(CSCBMiniDockFrameWnd);
#endif //_SCB_REPLACE_MINIFRAME

	DockControlBar(&m_wndDMObBar, AFX_IDW_DOCKBAR_RIGHT);
	DockControlBar(&m_wndShowcaseBar, AFX_IDW_DOCKBAR_LEFT);
//	DockControlBar(&m_wndCIBar, AFX_IDW_DOCKBAR_BOTTOM);
	DockControlBar(&m_wndFinderBar, AFX_IDW_DOCKBAR_BOTTOM);

//	m_wndCIBar.ShowWindow(SW_HIDE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メッセージ ハンドラ

void CMainFrame::OnChangeEditLvcx() 
{
//	OnChangeEditLvMetr(1);
}

void CMainFrame::OnChangeEditLvcy() 
{
//	OnChangeEditLvMetr(2);
}

void CMainFrame::OnChangeEditLvMetr(int nMask)
{
	CSize size = GetLvSize();
	UINT i = GetDMLv();
	CExp00069Doc *pDoc;
	if (pDoc = DYNAMIC_DOWNCAST(CExp00069Doc, GetActiveDocument())) {
		CDDAT &ddat = pDoc->GetDDAT();
		LevelDesc_t d;
		HRESULT hr;
		if (SUCCEEDED(hr = ddat.GetMapDescAt(i, d))) {
			CSize sizeIn(d.cyLv, d.cxLv);
			if (!(nMask & 1)) size.cx = sizeIn.cx;
			if (!(nMask & 2)) size.cy = sizeIn.cy;
			if (size != sizeIn) {
				d.cxLv = size.cy;
				d.cyLv = size.cx;
				if (ddat.SetMapDescAt(i, d)) {
					pDoc->GetEdCtx(FALSE)->DispatchDMUpdate(DPM_SELECTDMLV, NULL);
				}
			}
		}
	}
}

void CMainFrame::OnBtnCursor(UINT nID) 
{
	int iEditLvTool = -1;

	switch (nID) {
	case ID_BTN_CURSOR:	iEditLvTool = 0; break;
	case ID_BTN_N1:		iEditLvTool = 1; break;
	case ID_BTN_N2:		iEditLvTool = 2; break;
	case ID_BTN_N3:		iEditLvTool = 3; break;
	case ID_BTN_N4:		iEditLvTool = 4; break;
	case ID_BTN_N5:		iEditLvTool = 5; break;
	case ID_BTN_N6:		iEditLvTool = 6; break;
	case ID_BTN_N7:		iEditLvTool = 7; break;
	case ID_BTN_N8:		iEditLvTool = 8; break;
	case ID_BTN_ERASE_TILE_REC: m_iEditToolF = m_iEditToolF ^ ELVTF_ERASE_TILE_REC; break;
	}

	if (0 <= iEditLvTool) m_iEditLvTool = iEditLvTool, m_iEditToolF &= ~ELVTF_ERASE_TILE_REC;
}

void CMainFrame::OnUpdateBtnCursor(CCmdUI* pCmdUI) 
{
	UINT nID = pCmdUI->m_nID;
	BOOL b = FALSE, r = TRUE;
	switch (nID) {
	case ID_BTN_CURSOR:	b = (m_iEditLvTool == 0); break;
	case ID_BTN_N1:		b = (m_iEditLvTool == 1); break;
	case ID_BTN_N2:		b = (m_iEditLvTool == 2); break;
	case ID_BTN_N3:		b = (m_iEditLvTool == 3); break;
	case ID_BTN_N4:		b = (m_iEditLvTool == 4); break;
	case ID_BTN_N5:		b = (m_iEditLvTool == 5); break;
	case ID_BTN_N6:		b = (m_iEditLvTool == 6); break;
	case ID_BTN_N7:		b = (m_iEditLvTool == 7); break;
	case ID_BTN_N8:		b = (m_iEditLvTool == 8); break;
	case ID_BTN_ERASE_TILE_REC: b = (m_iEditLvTool != 0 && (m_iEditToolF & ELVTF_ERASE_TILE_REC)) ? 1 : 0, r = (m_iEditLvTool != 0); break;
	}
	pCmdUI->SetRadio(b);
	pCmdUI->Enable(r);
}

BOOL CMainFrame::OnQueryNewPalette() 
{
	CWnd *wnd;
	if (wnd = GetActiveView()) {
		return wnd->SendMessage(WM_QUERYNEWPALETTE);
	}
	
	return CFrameWnd::OnQueryNewPalette();
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFrameWnd::OnShowWindow(bShow, nStatus);

	RecalcLayout();
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (!CFrameWnd::OnCreateClient(lpcs, pContext))
		return FALSE;
	return TRUE;
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if (CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	if (nID == ID_EDIT_COPY || nID == ID_EDIT_PASTE || (32600 <= nID && nID < 32700) || nID == ID_VIEW_DEV)
		if (m_wndDMObBar.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;

	return FALSE;
}

void CMainFrame::OnDMLvSelBarSelChange()
{
	ASSERT(FALSE);
}

CWnd *CMainFrame::GetDockedSite(UINT nID)
{
//	switch (nID) {
//	case IDW_DMCIBAR: return &m_wndCIBar;
//	}
	return NULL;
}

BOOL CMainFrame::ShowDockedSite(UINT nID, BOOL fShow)
{
	CWnd *pWnd = GetDockedSite(nID);
	if (!pWnd || !pWnd->IsWindowEnabled()) return FALSE;

	if (pWnd->IsWindowVisible()) return TRUE;
	pWnd->ShowWindow(fShow ? SW_SHOW : SW_HIDE);

	RecalcLayout();
	return TRUE;
}

void CMainFrame::OnDMLvSel2BarSelChange()
{
	int iLv = m_wndLvSelBar.GetPos();

	CInterViewInfluential::Send(GetActiveView(), DPM_SET_VW_LV, &iLv);
}

void CMainFrame::OnDMMapSel2BarSelChange()
{
	int iMap = m_wndMapSelBar.GetPos();

	CInterViewInfluential::Send(GetActiveView(), DPM_SET_VW_MAP, &iMap);
}
