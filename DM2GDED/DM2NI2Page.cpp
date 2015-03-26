// DM2NI2Page.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2NI2Page.h"
#include "PM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2NI2Page プロパティ ページ

IMPLEMENT_DYNCREATE(CDM2NI2Page, CDM2NIBPage)

CDM2NI2Page::CDM2NI2Page() : CDM2NIBPage(CDM2NI2Page::IDD)
{
	//{{AFX_DATA_INIT(CDM2NI2Page)
	m_iColorKey1 = -1;
	//}}AFX_DATA_INIT

	m_iColorKey1 = 0;
}

CDM2NI2Page::~CDM2NI2Page()
{
}

void CDM2NI2Page::DoDataExchange(CDataExchange* pDX)
{
	CDM2NIBPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDM2NI2Page)
	DDX_Control(pDX, IDC_COMBO_PAL2, m_wndPal2);
	DDX_Control(pDX, IDC_COMBO_PAL1, m_wndPal1);
	DDX_Control(pDX, IDC_RADIO_COLOR_KEY2, m_wndCK2);
	DDX_Control(pDX, IDC_RADIO_COLOR_KEY1, m_wndCK1);
	DDX_Radio(pDX, IDC_RADIO_COLOR_KEY1, m_iColorKey1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDM2NI2Page, CDM2NIBPage) // CDM2NIBPage
	//{{AFX_MSG_MAP(CDM2NI2Page)
	ON_CONTROL(CBN_SELCHANGE, IDC_COMBO_PAL1, OnStaticPal1)
	ON_CONTROL(CBN_SELCHANGE, IDC_COMBO_PAL2, OnStaticPal2)
	//}}AFX_MSG_MAP
//	ON_STN_DBLCLK(IDC_STATIC_PAL, OnStaticPal)
	ON_MESSAGE(PM_WM_CLICK_PIXEL, OnClickPixel)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_COLOR_KEY1, IDC_RADIO_COLOR_KEY2, OnRadioColorKey1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2NI2Page メッセージ ハンドラ

void CDM2NI2Page::OnStaticPal() 
{
	if (!MkCtx()) return;

//	m_ctx->iColorAltKey = m_wndStatic.GetSel();

	UpdateVw(true);
}

BOOL CDM2NI2Page::OnSetActive() 
{
	if (!CDM2NIBPage::OnSetActive()) AfxThrowNotSupportedException();

	UpdateVw();

	if (!MkCtx()) return FALSE;

	DMGHLiIMGM &imgm = m_ctx->imgm[1];
	bool fIs8 = imgm.fIs8bpp;
	m_fUse1 = (0 <= m_ctx->iColorSrcKey);
	m_fUse2 = m_fUse1 && (0 <= m_ctx->iColorSrcKey2);

	m_wndPal1.EnableWindow(m_fUse1 && true);
	m_wndPal2.EnableWindow(false);

	m_wndCK1.EnableWindow(m_fUse1 && m_fUse2);
	m_wndCK2.EnableWindow(m_fUse1 && m_fUse2);

	if (m_fUse1) if (fIs8) m_wndPal1.Set8(m_ctx->iColorSrcKey, m_ctx->DM2Pal); else m_wndPal1.Set4(imgm.cPal, m_ctx->iColorSrcKey, m_ctx->DM2Pal);
	if (m_fUse2) if (fIs8) m_wndPal2.Set8(m_ctx->iColorSrcKey2, m_ctx->DM2Pal); else m_wndPal2.Set4(imgm.cPal, m_ctx->iColorSrcKey2, m_ctx->DM2Pal);

	m_wndPal1.Init();
	m_wndPal2.Init();

	return TRUE;
}

void CDM2NI2Page::UpdateVw(bool f)
{
	if (f) VERIFY(m_ctx->DoStep(2));

	m_frame.m_view.MakeStaged(2);
}

LRESULT CDM2NI2Page::OnClickPixel(WPARAM wParam, LPARAM lParam)
{
	if (!MkCtx()) return 0;

	BYTE c = m_ctx->imgm[1].GetPixel(wParam, lParam);

	if (m_iColorKey1 == 0) {
		m_ctx->iColorKey = c;
		m_wndPal1.SetCurSel(c);
	} else {
		m_ctx->iColorKey2 = c;
		m_wndPal2.SetCurSel(c);
	}

	UpdateVw(true);

	return 0;
}

void CDM2NI2Page::OnRadioColorKey1(UINT nID) 
{
	int iSel = m_iColorKey1;
	if (!UpdateData()) return;
	if (iSel == m_iColorKey1) return;

	m_wndPal1.EnableWindow(m_fUse1 && (m_iColorKey1 == 0));
	m_wndPal2.EnableWindow(m_fUse2 && (m_iColorKey1 == 1));

	m_wndPal1.Invalidate();
	m_wndPal2.Invalidate();
}

void CDM2NI2Page::OnStaticPal1() 
{
	if (!MkCtx() || !m_fUse1) return;

	m_ctx->iColorKey = m_wndPal1.GetCurSel();

	UpdateVw(true);
}

void CDM2NI2Page::OnStaticPal2() 
{
	if (!MkCtx() || !m_fUse2) return;

	m_ctx->iColorKey2 = m_wndPal2.GetCurSel();

	UpdateVw(true);
}
