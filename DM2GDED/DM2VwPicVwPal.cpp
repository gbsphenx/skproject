// DM2VwPicVwPal.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2VwPicVwPal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2VwPicVwPal

CDM2VwPicVwPal::CDM2VwPicVwPal()
{
	for (int i = 0; i < 3; i++) m_fPalAvail[i] = false, m_iKeys[i] = -1;
}

CDM2VwPicVwPal::~CDM2VwPicVwPal()
{

}

BEGIN_MESSAGE_MAP(CDM2VwPicVwPal, CControlBar)
	//{{AFX_MSG_MAP(CDM2VwPicVwPal)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2VwPicVwPal メッセージ ハンドラ

void CDM2VwPicVwPal::OnPaint() 
{
	CPaintDC dc(this);

	RenderContext a;
	Render(&dc, a);

}

int CDM2VwPicVwPal::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	OnResize();
	
	return 0;
}

void CDM2VwPicVwPal::OnResize()
{
	CClientDC dc(this);
	dc.SelectStockObject(DEFAULT_GUI_FONT);
	dc.GetTextMetrics(&m_tm);

	m_yTextLead = m_tm.tmInternalLeading;
	m_cyText = m_tm.tmHeight +m_tm.tmInternalLeading +m_tm.tmExternalLeading;
	m_cyBox = 20;
}

void CDM2VwPicVwPal::RenderPal(CDC *pDC, RenderContext &r, int i)
{
	int &ty = r.ty;

	LPCSTR pszText = "?";
	switch (i) {
	case 0: pszText = "Original Image Palette"; break;
	case 1: pszText = "Restrict Palette"; break;
	case 2: pszText = "Final Image Palette"; break;
	}

	if (pDC) pDC->TextOut(0, ty +m_yTextLead, pszText);
	ty += m_cyText;

	CBrush brWhite;
	brWhite.CreateStockObject(WHITE_BRUSH);
	CBrush brGray;
	brGray.CreateStockObject(GRAY_BRUSH);

	int x;
	for (x = 0; x < 16; x++) {
		CRect rc(
			m_cyBox *x +0,
			ty,
			m_cyBox *x +m_cyBox,
			ty +m_cyBox
			);
		CRect rc2(rc);
		rc2.DeflateRect(2, 2);
		if (pDC) pDC->FrameRect(&rc2, (x == m_iKeys[i]) ? &brWhite : &brGray);
		rc2.DeflateRect(2, 2);
		if (pDC) pDC->FillSolidRect(&rc2, m_clrPal[i][x]);
	}

	ty += m_cyBox;
}

void CDM2VwPicVwPal::Render(CDC *pDC, RenderContext &r)
{
	if (pDC) pDC->SelectStockObject(DEFAULT_GUI_FONT);

	if (pDC) pDC->SetBkMode(TRANSPARENT);
	if (pDC) pDC->SetTextColor(RGB(255,255,255));

	bool fEnabled = IsWindowEnabled() ? true : false;

	int &ty = r.ty;

	if (pDC) pDC->TextOut(0, ty +m_yTextLead, fEnabled ? "Palette Enabled (4-bit Image)" : "Palette Disabled (8-bit Image)");
	ty += m_cyText;

	RenderPal((fEnabled && m_fPalAvail[0]) ? pDC : NULL, r, 0);
	RenderPal((fEnabled && m_fPalAvail[1]) ? pDC : NULL, r, 1);
	RenderPal((fEnabled && m_fPalAvail[2]) ? pDC : NULL, r, 2);
}
