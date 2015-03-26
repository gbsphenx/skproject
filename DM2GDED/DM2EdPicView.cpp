// DM2EdPicView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2EdPicView.h"
#include "DM2ImageExch.h"
#include "DM2CompAdv.h"
#include "DM2EdPicFrame.h"
#include "DM2ImageComp.h"
#include "PM.h"
#include "EditImgFlgsDlg.h"
#include "DM2Palo.h"
#include "InprogressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2EdPicView_PH

class CDM2EdPicView_PH : public CDM2ImageComp::ProgressHint
{
	// 
	CInprogressDlg wndDlg;

public:
	// 
	CDM2EdPicView_PH()
	{

	}
	// 
	~CDM2EdPicView_PH()
	{
		wndDlg.Close();
	}
	// 
	void Create(CWnd *p)
	{
		wndDlg.DoModeless(p);
	}

	// 
	virtual void Set_Max(int iMax)
	{
		wndDlg.SetHintMinMax(0, iMax);
	}
	// 
	virtual void Set_Pos(int iPos)
	{
		wndDlg.SetHintPos(iPos);
	}

};

/////////////////////////////////////////////////////////////////////////////
// CDM2EdPicView

IMPLEMENT_DYNAMIC(CDM2EdPicView, CScrollView)

CDM2EdPicView::CDM2EdPicView(CGDEdCtx *pEdCtx): m_pEdCtx(pEdCtx)
{
	m_pSIIVCtx.Attach(new CDM2SIIVCtx(m_pEdCtx));

	m_nDM2SIIVT = DM2SIIVT_PCAT;
	m_nScene = -1;
	m_nTarScene = -1;
	m_fEditing = false;
	m_fTransparent = true;
	m_iKeyIdx = -1;
	m_fConv4bpp = false;
	m_ts = piptsScene;
	m_fPalConvMaster = false;
	m_fOverrideSceneKey = false;

	m_iStage1 = m_iStage2 = 0;

	m_rct.m_nStyle = 0 |CRectTracker::dottedLine;

	m_nInit = 0;
}

CDM2EdPicView::~CDM2EdPicView()
{

}

BEGIN_MESSAGE_MAP(CDM2EdPicView, CScrollView)
	//{{AFX_MSG_MAP(CDM2EdPicView)
	ON_COMMAND(ID_FILE_CLOSE_VIEW, OnFileCloseView)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE_4BPP, OnEditPaste4bpp)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_4BPP, OnUpdateEditPaste4bpp)
	ON_COMMAND(ID_EDIT_PASTE_8BPP, OnEditPaste8bpp)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_8BPP, OnUpdateEditPaste8bpp)
	ON_COMMAND(ID_FILE_MODE_PREVIEW, OnFileModePreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_MODE_PREVIEW, OnUpdateFileModePreview)
	ON_COMMAND(ID_FILE_MODE_EDIT, OnFileModeEdit)
	ON_UPDATE_COMMAND_UI(ID_FILE_MODE_EDIT, OnUpdateFileModeEdit)
	ON_COMMAND(ID_MOVE_BACK, OnMoveBack)
	ON_UPDATE_COMMAND_UI(ID_MOVE_BACK, OnUpdateMoveBack)
	ON_COMMAND(ID_MOVE_FRONT, OnMoveFront)
	ON_UPDATE_COMMAND_UI(ID_MOVE_FRONT, OnUpdateMoveFront)
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_FILE_SAVE_PIC, OnFileSavePic)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_PIC, OnUpdateFileSavePic)
	ON_COMMAND(ID_EDIT_IMAGE_FLGS, OnEditImageFlgs)
	ON_UPDATE_COMMAND_UI(ID_EDIT_IMAGE_FLGS, OnUpdateEditImageFlgs)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_SCENE_NO, ID_DRIVING_PC98, OnSceneNo)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SCENE_NO, ID_DRIVING_PC98, OnUpdateSceneNo)
	ON_COMMAND_RANGE(ID_VIEW_ENABLE_TRANSPARENT, ID_4BPP_PALCONV_SLAVE, OnViewEnableTransparent)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_ENABLE_TRANSPARENT, ID_4BPP_PALCONV_SLAVE, OnUpdateViewEnableTransparent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2EdPicView 描画

void CDM2EdPicView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(640, 400));

	//OnReloadImage();

	ResizeParentToFit(FALSE);

	OnResize();

	if (m_nInit == 0) {
		CGDAT &core = m_pEdCtx->GetGDAT();
		UI32 nGDRIPT = core.GetGDRIPT();

		m_fBestMatch = (nGDRIPT == GDRIPT_SCENE);
		m_fMkAltPal = (nGDRIPT == GDRIPT_RAW);
		m_fUse8bpp = !core.IsC8U8Allowed();
	}

	m_nInit++;
}

void CDM2EdPicView::OnDraw(CDC* pDC)
{
	pDC->SelectStockObject(DEFAULT_GUI_FONT);
	pDC->SetTextColor(RGB(255,255,255));

	bool fRenderOk = m_pSIIVCtx->GetScrnCtx().RenderScreenTo(*pDC, m_rcPic);

	if (!fRenderOk) {
		pDC->FillSolidRect(&m_rcPic, RGB(0, 0, 0));
		pDC->DrawText("Sorry ERR...", m_rcPic, 0 |DT_VCENTER |DT_CENTER |DT_SINGLELINE);
	} else {
		if (m_fEditing) {
			int cx = m_sizeImage.cx*2;
			int cy = m_sizeImage.cy*2;
			CPoint point = m_rcPic.CenterPoint();
			CRect rc(point.x -(cx+1)/2, point.y -(cy+1)/2, point.x +cx/2, point.y +cy/2);
			m_rct.m_rect = rc;
			m_rct.Draw(pDC);
		}
	}

#if 0
	LPCSTR pszImageState = "-";

	CString str[3];
	str[0].Format("Render Status: %s (Image: %s)"
		, fRenderOk ? "OK" : "ERR"
		, IsOrgImage8bpp() ? "8-bpp" : "4-bpp"
		);
	str[1].Format("Image Status: %s"
		, pszImageState
		);

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(192,192,192));

	CRect rc;
	GetClientRect(&rc);
	rc.bottom = rc.top +tm.tmInternalLeading +tm.tmExternalLeading +tm.tmHeight;
	for (int i = 0; i < 2; i++) {
		pDC->DrawText(str[i], &rc, 0 |DT_LEFT|DT_TOP|DT_NOPREFIX);
		rc += CPoint(0, rc.Height());
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CDM2EdPicView 診断

#ifdef _DEBUG
void CDM2EdPicView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDM2EdPicView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDM2EdPicView メッセージ ハンドラ

void CDM2EdPicView::Select(CGDEntry pos, BOOL fAddHist)
{
	if (fAddHist) {
		if (m_listNext.size() >= 1) m_listBack.push_front(GetCurPos());

		m_listNext.clear();
		m_listNext.push_back(pos);
	}

	m_fEditing = false;

	OnFreshScene();
	OnReloadImage();
}

bool CDM2EdPicView::SelectSR(int iDir, bool fTest)
{
	if (m_fEditing) return false;

	if (iDir > 0 && m_listNext.size() >= 2) {
		if (fTest) return true;

		m_listBack.push_front(m_listNext.front());
		m_listNext.pop_front();
	} else if (iDir < 0 && m_listBack.size() >= 1) {
		if (fTest) return true;

		m_listNext.push_front(m_listBack.front());
		m_listBack.pop_front();
	} else {
		return false;
	}
	Select(m_listNext.front(), false);
	return true;
}

void CDM2EdPicView::OnSceneNo(UINT nID) 
{
	switch (nID) {
	case ID_DRIVING_PCAT:   m_nDM2SIIVT = DM2SIIVT_PCAT; break;
	case ID_DRIVING_AMIGA:  m_nDM2SIIVT = DM2SIIVT_AMIGA; break;
	case ID_DRIVING_PC98:   m_nDM2SIIVT = DM2SIIVT_PC9801; break;
	case ID_SCENE_FORCE_PAL_NO: m_nTarScene =-1; break;
	case ID_SCENE_FORCE_PAL_0:  m_nTarScene = 0; break;
	case ID_SCENE_FORCE_PAL_1:  m_nTarScene = 1; break;
	case ID_SCENE_FORCE_PAL_2:  m_nTarScene = 2; break;
	case ID_SCENE_FORCE_PAL_3:  m_nTarScene = 3; break;
	case ID_SCENE_FORCE_PAL_4:  m_nTarScene = 4; break;
	case ID_SCENE_FORCE_PAL_5:  m_nTarScene = 5; break;
	case ID_SCENE_NO: m_nScene =-1; break;
	case ID_SCENE_0:  m_nScene = 0; break;
	case ID_SCENE_1:  m_nScene = 1; break;
	case ID_SCENE_2:  m_nScene = 2; break;
	case ID_SCENE_3:  m_nScene = 3; break;
	case ID_SCENE_4:  m_nScene = 4; break;
	case ID_SCENE_5:  m_nScene = 5; break;
	case ID_SCENE_FORCE_SCENE_KEY: m_fOverrideSceneKey = !m_fOverrideSceneKey; break;
	}

	OnFreshScene();
}

void CDM2EdPicView::OnUpdateSceneNo(CCmdUI* pCmdUI) 
{
	bool fSceneSelable = IsSceneSelable();
	bool fVwSceneSelable = m_fEditing && (m_ts == piptsScene);

	bool fRadio = false;
	bool fEnabled = true;
	switch (pCmdUI->m_nID) {
	case ID_DRIVING_PCAT:   fRadio = (m_nDM2SIIVT == DM2SIIVT_PCAT); break;
	case ID_DRIVING_AMIGA:  fRadio = (m_nDM2SIIVT == DM2SIIVT_AMIGA); break;
	case ID_DRIVING_PC98:   fRadio = (m_nDM2SIIVT == DM2SIIVT_PC9801); break;
	case ID_SCENE_FORCE_PAL_NO: fRadio = fSceneSelable && (m_nTarScene ==-1); fEnabled = fSceneSelable; break;
	case ID_SCENE_FORCE_PAL_0:  fRadio = fSceneSelable && (m_nTarScene == 0); fEnabled = fSceneSelable; break;
	case ID_SCENE_FORCE_PAL_1:  fRadio = fSceneSelable && (m_nTarScene == 1); fEnabled = fSceneSelable; break;
	case ID_SCENE_FORCE_PAL_2:  fRadio = fSceneSelable && (m_nTarScene == 2); fEnabled = fSceneSelable; break;
	case ID_SCENE_FORCE_PAL_3:  fRadio = fSceneSelable && (m_nTarScene == 3); fEnabled = fSceneSelable; break;
	case ID_SCENE_FORCE_PAL_4:  fRadio = fSceneSelable && (m_nTarScene == 4); fEnabled = fSceneSelable; break;
	case ID_SCENE_FORCE_PAL_5:  fRadio = fSceneSelable && (m_nTarScene == 5); fEnabled = fSceneSelable; break;
	case ID_SCENE_NO: fRadio = fVwSceneSelable && (m_nScene ==-1); fEnabled = fVwSceneSelable; break;
	case ID_SCENE_0:  fRadio = fVwSceneSelable && (m_nScene == 0); fEnabled = fVwSceneSelable; break;
	case ID_SCENE_1:  fRadio = fVwSceneSelable && (m_nScene == 1); fEnabled = fVwSceneSelable; break;
	case ID_SCENE_2:  fRadio = fVwSceneSelable && (m_nScene == 2); fEnabled = fVwSceneSelable; break;
	case ID_SCENE_3:  fRadio = fVwSceneSelable && (m_nScene == 3); fEnabled = fVwSceneSelable; break;
	case ID_SCENE_4:  fRadio = fVwSceneSelable && (m_nScene == 4); fEnabled = fVwSceneSelable; break;
	case ID_SCENE_5:  fRadio = fVwSceneSelable && (m_nScene == 5); fEnabled = fVwSceneSelable; break;
	case ID_SCENE_FORCE_SCENE_KEY: fRadio = m_fOverrideSceneKey && fVwSceneSelable; fEnabled = fVwSceneSelable; break;
	}
	pCmdUI->SetRadio(fRadio);
	pCmdUI->Enable(fEnabled);
}

void CDM2EdPicView::OnFileCloseView() 
{
	GetParentFrame()->SendMessage(WM_CLOSE);
}

BOOL CDM2EdPicView::OnEraseBkgnd(CDC* pDC) 
{
	CRect rc;
	GetClientRect(&rc);
	pDC->SaveDC();
	pDC->ExcludeClipRect(&m_rcPic);
	pDC->FillSolidRect(&rc, RGB(0, 0, 0));
	pDC->RestoreDC(-1);
	return TRUE;
}

void CDM2EdPicView::OnEditCopy() 
{
	CDM2ImageConv z;
	HBITMAP hbm = NULL;
	if (m_imgs[m_iStage1].fIs8bpp) {
		z.EncodeToBM8(m_imgs[m_iStage1], hbm, m_pEdCtx->GetGDAT().GetDM2Pal());
	} else {
		z.EncodeToBM4(m_imgs[m_iStage1], hbm, m_pEdCtx->GetGDAT().GetDM2Pal());
	}
	bool fOk = false;
	CExtBM bm;
	bm.Attach(hbm);
	if (OpenClipboard()) {
		HGLOBAL hMem = NULL;
		if (EmptyClipboard()) {
			if (bm.SaveTo(hMem)) {
				if (SetClipboardData(CF_DIB, hMem)) {
					fOk = true;
				}
			}
		}
		CloseClipboard();
	}
	if (!fOk) {
		AfxMessageBox(IDS_ERR_FAILED_COPY_IMAGE, 0 |MB_ICONEXCLAMATION);
	}
}

void CDM2EdPicView::OnEditPaste4bpp() 
{
	while (true) {
		if (!OpenClipboard())
			break;
		HGLOBAL hMem = GetClipboardData(CF_DIB);
		CExtBM bm;
		if (hMem) {
			bm.LoadFrom(hMem);
		}
		CloseClipboard();

		if (!bm.m_hObject)
			break;

		CDM2ImageConv cvt;

		if (!cvt.EncodeBMTo8(bm, m_imgs[0], m_pEdCtx->GetGDAT().GetDM2Pal()))
			break;

		m_iStage1 = m_iStage2 = 0;
		m_fConv4bpp = true;
		m_fTweakTp = false;
		m_fEditing = true;

		OnFreshImage();
		OnRedrawImage();
		return;
	}
	AfxMessageBox(IDS_ERR_FAILED_PASTE_IMAGE, 0 |MB_ICONEXCLAMATION);
}

void CDM2EdPicView::OnEditPaste8bpp() 
{
	while (true) {
		if (!OpenClipboard())
			break;
		HGLOBAL hMem = GetClipboardData(CF_DIB);
		CExtBM bm;
		if (hMem) {
			bm.LoadFrom(hMem);
		}
		CloseClipboard();

		if (!bm.m_hObject)
			break;

		CDM2ImageConv cvt;

		if (!cvt.EncodeBMTo8(bm, m_imgs[0], m_pEdCtx->GetGDAT().GetDM2Pal()))
			break;

		m_iStage1 = m_iStage2 = 0;
		m_fConv4bpp = false;
		m_fTweakTp = false;
		m_fEditing = true;

		OnFreshImage();
		OnRedrawImage();
		return;
	}
	AfxMessageBox(IDS_ERR_FAILED_PASTE_IMAGE, 0 |MB_ICONEXCLAMATION);
}

void CDM2EdPicView::OnFileModePreview() 
{
	if (!m_fEditing) return;

	m_fEditing = false;

	OnReloadImage();
}

void CDM2EdPicView::OnFileModeEdit() 
{
	if (m_fEditing) return;

	m_fEditing = true;
	m_fConv4bpp = !m_imgs[0].fIs8bpp;

	OnFreshScene();
}

void CDM2EdPicView::OnUpdateEditPaste4bpp(CCmdUI* pCmdUI) { }
void CDM2EdPicView::OnUpdateEditPaste8bpp(CCmdUI* pCmdUI) {	pCmdUI->Enable(m_fUse8bpp); }
void CDM2EdPicView::OnUpdateFileModePreview(CCmdUI* pCmdUI) { pCmdUI->SetRadio(!m_fEditing); }
void CDM2EdPicView::OnUpdateFileModeEdit(CCmdUI* pCmdUI) { pCmdUI->SetRadio(m_fEditing); }
void CDM2EdPicView::OnUpdateMoveBack(CCmdUI* pCmdUI) { pCmdUI->Enable(SelectSR(-1, true)); }
void CDM2EdPicView::OnUpdateMoveFront(CCmdUI* pCmdUI) { pCmdUI->Enable(SelectSR(+1, true)); }

void CDM2EdPicView::OnMoveBack() 
{
	SelectSR(-1);
}

void CDM2EdPicView::OnMoveFront() 
{
	SelectSR(+1);
}

void CDM2EdPicView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	OnResize();
	
}

void CDM2EdPicView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_fEditing) {
		CPoint pt = GetDeviceScrollPosition() + point;
		if (m_rcPicIn.PtInRect(pt) && m_iKeyIdx >= 0) {
			CString strMsg;
			AfxFormatString1(strMsg, IDS_WARN_SET_NEW_COLORKEY, "");
			int r = MessageBox(strMsg, 0, 0 |MB_YESNO |MB_ICONEXCLAMATION);
			if (r == IDYES) {
				pt -= m_rcPicIn.TopLeft();
				pt.x /= 2;
				pt.y /= 2;
				SetNewColorKeyAt(pt);
			}
			return;
		}
	}
	
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CDM2EdPicView::SetNewColorKeyAt(CPoint pt)
{
	DMGHLiIMGM &rSrc = m_imgs[m_iStage1];
	m_iKeyFrom = m_iKeyIdx;
	m_iKeyTo = rSrc.GetPixel(pt.x, pt.y);
	m_fTweakTp = true;

	OnRedrawImage();
}

void CDM2EdPicView::OnViewEnableTransparent(UINT nID) 
{
	switch (nID) {
	case ID_VIEW_ENABLE_TRANSPARENT:
		m_fTransparent = !m_fTransparent;
		OnRedrawImage();
		break;
	case ID_4BPP_MK_INDEPENDENT_PALETTE:
		m_fMkAltPal = !m_fMkAltPal;
		break;
	case ID_4BPP_BEST_MATCH:
		m_fBestMatch = true;
		OnRedrawImage();
		break;
	case ID_4BPP_BEST_QUALITY:
		m_fBestMatch = false;
		OnRedrawImage();
		break;
	case ID_8BPP_ENABLED:
		m_fUse8bpp = !m_fUse8bpp;
		break;
	case ID_4BPP_PALETTE_DUNGEON:
		m_ts = piptsScene;
		OnFreshScene();
		break;
	case ID_4BPP_PALETTE_INTERFACE:
		m_ts = piptsIf;
		OnFreshScene();
		break;
	case ID_4BPP_PALETTE_COLOR_PAL:
		m_ts = piptsColorPal;
		OnFreshScene();
		break;
	case ID_4BPP_PALCONV_SLAVE:
		m_fPalConvMaster = false;
		OnRedrawImage();
		break;
	case ID_4BPP_PALCONV_MASTER:
		m_fPalConvMaster = true;
		OnRedrawImage();
		break;
	}
}

void CDM2EdPicView::OnUpdateViewEnableTransparent(CCmdUI* pCmdUI) 
{
	switch (pCmdUI->m_nID) {
	case ID_VIEW_ENABLE_TRANSPARENT:
		pCmdUI->SetCheck(m_fTransparent);
		break;
	case ID_4BPP_MK_INDEPENDENT_PALETTE:
		pCmdUI->Enable(m_fEditing);
		pCmdUI->SetCheck(m_fMkAltPal);
		break;
	case ID_4BPP_BEST_MATCH:
		pCmdUI->Enable(m_fEditing);
		pCmdUI->SetRadio(m_fBestMatch);
		break;
	case ID_4BPP_BEST_QUALITY:
		pCmdUI->Enable(m_fEditing);
		pCmdUI->SetRadio(!m_fBestMatch);
		break;
	case ID_8BPP_ENABLED:
		pCmdUI->SetCheck(m_fUse8bpp);
		break;
	case ID_4BPP_PALETTE_DUNGEON:
		pCmdUI->Enable(m_fEditing && m_fBestMatch);
		pCmdUI->SetRadio(m_fBestMatch && (m_ts == piptsScene));
		break;
	case ID_4BPP_PALETTE_INTERFACE:
		pCmdUI->Enable(m_fEditing && m_fBestMatch);
		pCmdUI->SetRadio(m_fBestMatch && (m_ts == piptsIf));
		break;
	case ID_4BPP_PALETTE_COLOR_PAL:
		pCmdUI->Enable(m_fEditing && m_fBestMatch && (m_tsOrg == piptsColorPal));
		pCmdUI->SetRadio(m_fBestMatch && (m_ts == piptsColorPal));
		break;
	case ID_4BPP_PALCONV_SLAVE:
		pCmdUI->Enable(m_fEditing && m_fBestMatch && (m_ts == piptsColorPal));
		pCmdUI->SetRadio(!m_fPalConvMaster);
		break;
	case ID_4BPP_PALCONV_MASTER:
		pCmdUI->Enable(m_fEditing && m_fBestMatch && (m_ts == piptsColorPal));
		pCmdUI->SetRadio(m_fPalConvMaster);
		break;
	}
}

void CDM2EdPicView::OnReloadImage()
{
	CGDAT &core = m_pEdCtx->GetGDAT();

	m_imgs[0].cx = 0;

	CExtBM bm;
	bm.Attach(core.GetImgAt(GetCurPos(), m_imgs[0]));

	m_fx = m_imgs[0].ipf.x;
	m_fy = m_imgs[0].ipf.y;

	CFrameWnd *pWnd;
	if (pWnd = GetTopLevelFrame()) {
		CGDEntry pos = GetCurPos();
		CString strTitle;
		strTitle.Format("Image %02X-%02X-%02X-%02X-%02X-%02X"
			, 0U +pos.c1()
			, 0U +pos.c2()
			, 0U +pos.c3()
			, 0U +pos.c4()
			, 0U +pos.c5()
			, 0U +pos.c6()
			);
		pWnd->SetTitle(strTitle);
		pWnd->UpdateFrameTitleForDocument("DM2GDED");
	}

	CDM2VwPalAdvisor sa(m_pEdCtx->GetGDAT());
	CDM2VwPalAdvisory a = sa.Match(GetCurPos(), m_nScene);

	m_iStage1 = m_iStage2 = 0;
	m_fConv4bpp = !m_imgs[0].fIs8bpp;
	m_fTweakTp = false;
	m_ts = m_tsOrg = a.ts;
	m_posColorPal = a.pos;

	OnFreshImage();
	OnFreshScene();
}

void CDM2EdPicView::OnFreshScene()
{
	CDM2VwPalAdvisor sa(m_pEdCtx->GetGDAT());

	if (!m_fEditing || !m_fOverrideSceneKey) {
		CDM2VwPalAdvisory a = sa.Match(GetCurPos(), m_nScene);
		m_iKeyIdx = a.iSceneKey;
	} else {
		CDM2VwPalAdvisory a = sa.Match(CGDEntry::ERR, m_nScene);
		m_iKeyIdx = a.iSceneKey;
	}

	CGDAT &core = m_pEdCtx->GetGDAT();

	if (m_ts == piptsColorPal) {
		CDM2VwPalAdvisory a = sa.Match(GetCurPos(), m_nScene);

		COLORREF clr[16];
		RGBQUAD rq[16];
		core.ReadRawColorPalette(a.pos, 0, clr);

		for (int i = 0; i < 16; i++) CDM2Palo::Conv(clr[i], rq[i]);

		m_pSIIVCtx->GetScrnCtx().InitScreen(m_nDM2SIIVT, rq, core.GetDM2Pal());
	} else {
		m_pSIIVCtx->GetScrnCtx().InitScreen(m_nDM2SIIVT, m_nScene, core.GetDM2Pal());
	}

	OnRedrawImage();
}

void CDM2EdPicView::OnFreshImage()
{
	m_sizeImage.cx = m_imgs[0].cx;
	m_sizeImage.cy = m_imgs[0].cy;

	int cx = m_sizeImage.cx*2;
	int cy = m_sizeImage.cy*2;
	CPoint pt = m_rcPic.CenterPoint();
	m_rcPicIn.SetRect(pt.x -(cx+1)/2, pt.y -(cy+1)/2, pt.x +cx/2, pt.y +cy/2);
}

void CDM2EdPicView::OnResize()
{
	CRect rc;
	GetClientRect(&rc);
	m_rcPic.left = 0;
	m_rcPic.right = 640;
	if (640 <= rc.Width()) {
		m_rcPic += CPoint((rc.Width() - 640) / 2, 0);
	}
	m_rcPic.top = 0;
	m_rcPic.bottom = 400;
	if (400 <= rc.Height()) {
		m_rcPic += CPoint(0, (rc.Height() - 400) / 2);
	}
}

void CDM2EdPicView::OnRedrawImage()
{
	GetVwPal().Reinit(!m_fConv4bpp, m_iKeyIdx);

	if (m_fConv4bpp && m_imgs[0].cx != 0) {
		CDM2ImageConv ic;

		m_imgs[1].cx = 0;

		CGDAT &core = m_pEdCtx->GetGDAT();

		bool fOk = false;
		bool fReplacePal = false;
		bool fRestrict = false;
		COLORREF clr[16], clrScene[16];
		DMGHLciPAL cPal;

		if (!IsOrgImage8bpp()) GetVwPal().SetPal(0, m_imgs[0].cPal.p.f.color);

		if (!m_fEditing) {
			fOk = core.GetScenePalette(m_nScene, 0, clr);
		} else {
			switch (m_ts) {
			case piptsScene:
				fOk = fRestrict = core.GetScenePalette(m_nTarScene, 0, clr);
				break;
			case piptsIf:
				fOk = fRestrict = core.GetInterfacePalette(clr);
				break;
			case piptsColorPal:
				fOk = fRestrict = core.ReadRawColorPalette(m_posColorPal, 0, clr);
				if (m_fPalConvMaster) fRestrict = false;
				break;
			}
		}

		if (fOk && fRestrict) GetVwPal().SetPal(1, clr);

		if (core.GetScenePalette(m_nScene, 0, clrScene)) {
			fReplacePal = CDM2ImageConv().CCS4(clrScene, cPal, core.GetDM2Pal());
		}

		if (m_fBestMatch) {
			if (fOk && fRestrict) {
				ic.CCS4(m_imgs[0], m_imgs[1], clr, core.GetDM2Pal());
			} else {
				ic.CCS4(m_imgs[0], m_imgs[1], core.GetDM2Pal());
			}
			if (fReplacePal) m_imgs[1].cPal = cPal;
		} else {
			ic.CCS4(m_imgs[0], m_imgs[1], core.GetDM2Pal());
		}

		GetVwPal().SetPal(2, m_imgs[1].cPal.p.f.color);
		//GetVwPal().SetKey(2, (m_iKeyTo < 0) ? m_iKeyIdx : m_iKeyTo);

		m_iStage1 = m_iStage2 = 1;
	}

	GetVwPal().Invalidate();

	if (m_fTweakTp && m_imgs[m_iStage1].cx != 0) {
		m_iStage2 = m_iStage1 + 1;

		DMGHLiIMGM &rSrc = m_imgs[m_iStage1];
		DMGHLiIMGM &rDst = m_imgs[m_iStage2];
		rDst.cx = rSrc.cx;
		rDst.cy = rSrc.cy;
		rDst.fIs8bpp = rSrc.fIs8bpp;
		rDst.cPal = rSrc.cPal;
		int cx = rSrc.cx;
		int cy = rSrc.cy;
		if (!rDst.fIs8bpp) {
			for (int y = 0; y < cy; y++) {
				for (int x = 0; x < cx; x++) {
					UI8 p = rSrc.GetPixel(x, y);
					if (p == m_iKeyFrom) p = m_iKeyTo;
					else if (p == m_iKeyTo) p = m_iKeyFrom;
					rDst.SetPixel(x, y, p);
				}
			}

			swap(rDst.cPal.p.f.index[m_iKeyFrom], rDst.cPal.p.f.index[m_iKeyTo]);
			swap(rDst.cPal.p.f.color[m_iKeyFrom], rDst.cPal.p.f.color[m_iKeyTo]);

			GetVwPal().SetPal(2, rDst.cPal.p.f.color);
		} else {
			BYTE bKeyTo = (BYTE)m_iKeyFrom;
			if ((bKeyTo & 0x0F) > 8) bKeyTo--; else bKeyTo++;
			for (int y = 0; y < cy; y++) {
				for (int x = 0; x < cx; x++) {
					UI8 p = rSrc.GetPixel(x, y);
					if (p == m_iKeyTo) p = m_iKeyFrom;
					else if (p == m_iKeyFrom) p = bKeyTo;
					rDst.SetPixel(x, y, p);
				}
			}
		}
	}

	m_pSIIVCtx->GetScrnCtx().ClearScreen(0);
	m_pSIIVCtx->GetScrnCtx().DrawImageEntry(
		GetFinalImg(),
		CDM2SIIVScrnCtx::lpsMC,
		CRect(0, 0, 320, 200),
		m_fTransparent ? m_iKeyIdx : -1,
		0,
		0,
		m_ts == piptsIf,
		160,
		100,
		1,
		1
		);
	m_pSIIVCtx->GetScrnCtx().RenderToBM();

	Invalidate();
}

CDM2VwPicVwPal &CDM2EdPicView::GetVwPal()
{
	return static_cast<CDM2EdPicFrame *>(GetParentFrame())->GetVwPal();
}

void CDM2EdPicView::OnFileSavePic() 
{
	VERIFY(SaveNow());
}

void CDM2EdPicView::OnUpdateFileSavePic(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_fEditing);
}

bool CDM2EdPicView::DoSave(bool fQuery)
{
	if (!GetModified()) return true;

	CString str;
	AfxFormatString1(str, IDS_STR_SAVE_MOD, "");
	int r = MessageBox(str, NULL, 0 |MB_YESNOCANCEL |MB_ICONEXCLAMATION);
	if (r == IDYES) {
		SaveNow();
		return true;
	}
	if (r == IDNO) {
		return true;
	}
	return false;
}

bool CDM2EdPicView::SaveNow()
{
	CGDAT &core = m_pEdCtx->GetGDAT();
	bool fLE = core.IsLE();

	SizeBuff fv;
	CDM2ImageComp a(fLE);
	if (GetFinalImg().fIs8bpp) {
		CDM2EdPicView_PH ph;
		ph.Create(this);

		DMGHLiIMGM &imgm = GetFinalImg();
		imgm.ipf.x = m_fx;
		imgm.ipf.y = m_fy;
		CDM2ImageComp::DM2BMPImg8 aBmp8(imgm);
		if (a.MakeC8(aBmp8, fv, ph)) {
			if (core.SetRawDataFor(GetCurPos(), fv)) {
				UpdateEntry(GetCurPos());
				return true;
			} else {
				ASSERT(FALSE);
			}
		}
	} else {
		DMGHLiIMGM &imgm = GetFinalImg();
		imgm.ipf.x = m_fx;
		imgm.ipf.y = m_fy;
		CDM2ImageComp::DM2BMPImg4 aBmp4(imgm);
		CDM2ImageComp::DM2Pal4Loc16 aPal16;
		CDM2ImageComp::DM2Pal4Loc6 aPal6;
		CDM2ImageComp::IdxScatter aScatter;
		aScatter.MakeFrom(aBmp4);
		aPal16.MakeFrom(GetFinalImg().cPal);
		aPal6.MakeFrom(aScatter, aPal16);
		while (true) {
			if (m_fBestMatch || m_fMkAltPal) {
				if (!a.MakeC4(aBmp4, aPal6, fv)) break;
			} else {
				if (!a.MakeC4(aBmp4, aPal16, aPal6, fv)) break;
			}
			if (core.SetRawDataFor(GetCurPos(), fv)) {
				if (m_fMkAltPal) {
					SizeBuff fv;
					if (!fv.Alloc(16, GetFinalImg().cPal.p.f.index)) break;
					CGDEntry pos = GetCurPos();
					pos.c3(0x0D);
					pos.c5(0x00);
					pos.c6(0x00);
					if (!core.AddBinEntry(pos, fv) && !core.SetRawDataFor(pos, fv)) break;
					UpdateEntry(pos);
				}
				if (m_fPalConvMaster) {
					SizeBuff fv;
					if (!fv.Alloc(64)) break;
					UI8 *pb = fv.GetData();
					COLORREF clr;
					for (int i = 0; i < 16; i++, pb += 4)
						clr = GetFinalImg().cPal.p.f.color[i],
						pb[0] = i,
						pb[1] = GetRValue(clr),
						pb[2] = GetGValue(clr),
						pb[3] = GetBValue(clr);
					CGDEntry pos = GetCurPos();
					pos.c3(0x09);
					pos.c5(0x04);
					pos.c6(0x00);
					if (!core.AddBinEntry(pos, fv) && !core.SetRawDataFor(pos, fv)) break;
					UpdateEntry(pos);
				}
				UpdateEntry(GetCurPos());
				return true;
			}
			break;
		}
	}
	return false;
}

void CDM2EdPicView::UpdateEntry(CGDEntry pos)
{
	m_pEdCtx->DispatchGDUpdate(PM_V_UPDATE_ENTRY, &pos);
}

void CDM2EdPicView::OnEditImageFlgs() 
{
	CEditImgFlgsDlg wndDlg(this);
	wndDlg.m_nX = m_fx;
	wndDlg.m_nY = m_fy;
	int r = wndDlg.DoModal();
	if (r == IDOK) {
		m_fx = wndDlg.GetX();
		m_fy = wndDlg.GetY();
	}
}

void CDM2EdPicView::OnUpdateEditImageFlgs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_fEditing);
}
