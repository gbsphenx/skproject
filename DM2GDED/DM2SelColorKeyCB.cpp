// DM2SelColorKeyCB.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DM2GDED.h"
#include "DM2SelColorKeyCB.h"


// CDM2SelColorKeyCB

IMPLEMENT_DYNAMIC(CDM2SelColorKeyCB, CComboBox)

CDM2SelColorKeyCB::CDM2SelColorKeyCB()
{

}

CDM2SelColorKeyCB::~CDM2SelColorKeyCB()
{

}

BEGIN_MESSAGE_MAP(CDM2SelColorKeyCB, CComboBox)
END_MESSAGE_MAP()

// CDM2SelColorKeyCB メッセージ ハンドラ

// 
void CDM2SelColorKeyCB::Set4(const DMGHLciPAL &a, int iColorSrcKey, const DMGHLciDM2Pal &DM2Pal) {
	ResetContent();
	CString s;
	for (int x=0; x<16; x++) {
		s.Format(_T("%d"), x);
		m_clrs[x] = a.p.f.color[x];
		AddString(s);
	}

	if (iColorSrcKey < 0)
		iColorSrcKey = 0;
	SetCurSel(iColorSrcKey);
}

// 
void CDM2SelColorKeyCB::Set8(int iColorSrcKey, const DMGHLciDM2Pal &DM2Pal) {
	ResetContent();
	CString s;
	for (int x=0; x<256; x++) {
		s.Format(_T("%d"), x);
		m_clrs[x] = DM2Pal.clrs[x];
		AddString(s);
	}

	if (iColorSrcKey < 0)
		iColorSrcKey = 0;
	SetCurSel(iColorSrcKey);
}

// 
void CDM2SelColorKeyCB::Init() {

}

void CDM2SelColorKeyCB::DrawItem(LPDRAWITEMSTRUCT p) {
	CDC *pDC = CDC::FromHandle(p->hDC);
	if (pDC == NULL)
		return;
	CBrush br(GetSysColor((ODS_SELECTED & p->itemState) ? COLOR_HIGHLIGHT : COLOR_WINDOW));
	pDC->FillRect(&p->rcItem, &br);
	pDC->SelectStockObject(DEFAULT_GUI_FONT);
	int x = p->itemID;
	int bkMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF clrFore = pDC->SetTextColor(GetSysColor((ODS_SELECTED & p->itemState) ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));
	CSize size = pDC->GetTextExtent(_T("1234567890"), 10);
	int cx1 = size.cx / 10;
	{
		CString s; s.Format(_T("%d"), x);
		CRect rc1(
			p->rcItem.left + 1,
			p->rcItem.top + 1,
			p->rcItem.left + cx1 * 4,
			p->rcItem.bottom - 1
			);
		pDC->DrawText(s, rc1, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
	}
	{
		CRect rc2(
			p->rcItem.left + cx1 * 4 + 5,
			p->rcItem.top + 2,
			p->rcItem.right - 5,
			p->rcItem.bottom - 2
			);
		CBrush brc(m_clrs[x]);
		pDC->FillRect(&rc2, &brc);
	}

	pDC->SetBkMode(bkMode);
	pDC->SetTextColor(clrFore);
}
