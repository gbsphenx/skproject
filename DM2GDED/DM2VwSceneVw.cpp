// DM2VwSceneVw.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2VwSceneVw.h"
#include "DM2GDEDDoc.h"
#include "DM2ImageExch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2VwSceneVw

IMPLEMENT_DYNCREATE(CDM2VwSceneVw, CScrollView)

CDM2VwSceneVw::CDM2VwSceneVw()
{
	m_pSIIVCtx = NULL;
}

CDM2VwSceneVw::~CDM2VwSceneVw()
{
	InitSIIVCtx(NULL);
}


BEGIN_MESSAGE_MAP(CDM2VwSceneVw, CScrollView)
	//{{AFX_MSG_MAP(CDM2VwSceneVw)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_SCENE_NO, ID_SCENE_5, OnScene1)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SCENE_NO, ID_SCENE_5, OnUpdateScene1)
	ON_COMMAND_RANGE(ID_DRIVING_PCAT, ID_DRIVING_PC98, OnDrivingPcat)
	ON_UPDATE_COMMAND_UI_RANGE(ID_DRIVING_PCAT, ID_DRIVING_PC98, OnUpdateDrivingPcat)
	ON_COMMAND_RANGE(ID_EDIT_PASTE_4BPP, ID_EDIT_PASTE_8BPP, OnEditPaste4bpp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2VwSceneVw 描画

void CDM2VwSceneVw::OnDraw(CDC* pDC)
{
	if (!m_pSIIVCtx->GetScrnCtx().RenderScreenTo(*pDC, m_rcScene)) {
		pDC->FillSolidRect(m_rcScene, RGB(0, 0, 128));
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->DrawText("Sorry. ERR", m_rcScene, 0 |DT_CENTER |DT_VCENTER |DT_SINGLELINE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDM2VwSceneVw 診断

#ifdef _DEBUG
void CDM2VwSceneVw::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDM2VwSceneVw::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDM2VwSceneVw メッセージ ハンドラ

void CDM2VwSceneVw::OnScene1(UINT nID) 
{
	int iScene = m_iScene;

	switch (nID) {
	case ID_SCENE_NO: m_iScene = -1; break;
	case ID_SCENE_1: m_iScene = 1; break;
	case ID_SCENE_2: m_iScene = 2; break;
	case ID_SCENE_3: m_iScene = 3; break;
	case ID_SCENE_4: m_iScene = 4; break;
	case ID_SCENE_5: m_iScene = 5; break;
	}

	if (iScene != m_iScene) RefreshScene();
}

void CDM2VwSceneVw::OnUpdateScene1(CCmdUI* pCmdUI) 
{
	bool fMark = false;

	switch (pCmdUI->m_nID) {
	case ID_SCENE_NO: fMark = (m_iScene == -1); break;
	case ID_SCENE_1: fMark = (m_iScene == 1); break;
	case ID_SCENE_2: fMark = (m_iScene == 2); break;
	case ID_SCENE_3: fMark = (m_iScene == 3); break;
	case ID_SCENE_4: fMark = (m_iScene == 4); break;
	case ID_SCENE_5: fMark = (m_iScene == 5); break;
	}

	pCmdUI->SetRadio(fMark);
}

void CDM2VwSceneVw::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	CDM2GDEDDoc *pDoc = static_cast<CDM2GDEDDoc *>(GetDocument());
	CDM2SIIVCtx *pSIIVCtx = new CDM2SIIVCtx(pDoc->GetEdCtx(FALSE));
	InitSIIVCtx(pSIIVCtx);
	pSIIVCtx->Release();

#if 0
	m_iScene = -1;
	m_iDM2SIIVT = DM2SIIVT_PCAT;
#else
	m_iScene = 2;
	m_iDM2SIIVT = DM2SIIVT_PC9801;
#endif

	RefreshSized();
	RefreshScene();

	SetScrollSizes(MM_TEXT, CSize(640, 400));

	ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);

	ResizeParentToFit();
	
}

void CDM2VwSceneVw::OnSIIVCtxUpdate(UINT nHint, void *pHint)
{

}

void CDM2VwSceneVw::RefreshScene(bool fReinit)
{
	if (!m_pSIIVCtx) return;

	if (fReinit) m_pSIIVCtx->GetScrnCtx().InitScreen(m_iDM2SIIVT, (BYTE)m_iScene);

	m_pSIIVCtx->GetScrnCtx().RenderScene();
	//m_pSIIVCtx->GetScrnCtx().DrawImageEntry(m_imgm, CRect(0, 40, 0+224, 40+136), 0, 0, 0, false, 224/2, 40+136/2, 1, 1);
	m_pSIIVCtx->GetScrnCtx().RenderToBM();

	InvalidateRect(m_rcScene);
}

void CDM2VwSceneVw::OnDrivingPcat(UINT nID) 
{
	int iDM2SIIVT = m_iDM2SIIVT;

	switch (nID) {
	case ID_DRIVING_PCAT: m_iDM2SIIVT = DM2SIIVT_PCAT; break;
	case ID_DRIVING_AMIGA: m_iDM2SIIVT = DM2SIIVT_AMIGA; break;
	case ID_DRIVING_PC98: m_iDM2SIIVT = DM2SIIVT_PC9801; break;
	}

	if (m_iDM2SIIVT != iDM2SIIVT) RefreshScene();
}

void CDM2VwSceneVw::OnUpdateDrivingPcat(CCmdUI* pCmdUI) 
{
	bool fMark = false;

	switch (pCmdUI->m_nID) {
	case ID_DRIVING_PCAT: fMark = (m_iDM2SIIVT == DM2SIIVT_PCAT); break;
	case ID_DRIVING_AMIGA: fMark = (m_iDM2SIIVT == DM2SIIVT_AMIGA); break;
	case ID_DRIVING_PC98: fMark = (m_iDM2SIIVT == DM2SIIVT_PC9801); break;
	}

	pCmdUI->SetRadio(fMark);
}

BOOL CDM2VwSceneVw::OnEraseBkgnd(CDC* pDC) 
{
	pDC->SaveDC();
	pDC->ExcludeClipRect(&m_rcScene);
	pDC->FillSolidRect(&m_rcScreen, RGB(0, 0, 0));
	pDC->RestoreDC(-1);
	return TRUE;
	
	return CScrollView::OnEraseBkgnd(pDC);
}

void CDM2VwSceneVw::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	RefreshSized();
	
}

void CDM2VwSceneVw::RefreshSized()
{
	GetClientRect(&m_rcScreen);

	{
		CRect &rc = m_rcScene;
		rc.SetRect(0, 0, 640, 400);
	}
}

void CDM2VwSceneVw::OnEditPaste4bpp(UINT nID) 
{
	bool fUse8bpp = (nID == ID_EDIT_PASTE_8BPP);

	if (OpenClipboard()) {
		HGLOBAL hMem = GetClipboardData(CF_DIB);
		CExtBM bm;
		if (hMem) {
			bm.LoadFrom(hMem);
		}
		CloseClipboard();

		if (bm.m_hObject) {
			CDM2ImageConv cvt;

			if (false
				|| (fUse8bpp && cvt.EncodeBMTo8(bm, m_imgm))
				|| (!fUse8bpp && cvt.EncodeBMTo4(bm, m_imgm))
			) {
				RefreshScene();
			}
		}
	}
}
