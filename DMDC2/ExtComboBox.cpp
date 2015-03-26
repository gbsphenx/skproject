// ExtComboBox.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "ExtComboBox.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtComboBox::DATA

bool CExtComboBox::DATA::operator <(const DATA &r) const
{
	int res = text.CompareNoCase(r.text);

	if (  text.IsEmpty()) res += 2;
	if (r.text.IsEmpty()) res -= 2;

	if (res < 0)
		return true;
	if (res > 0)
		return false;

	res = suffix.CompareNoCase(r.suffix);
	if (res < 0)
		return true;
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CExtComboBox

CExtComboBox::CExtComboBox()
{

}

CExtComboBox::~CExtComboBox()
{

}

BEGIN_MESSAGE_MAP(CExtComboBox, CComboBox)
	//{{AFX_MSG_MAP(CExtComboBox)
	ON_WM_CHARTOITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtComboBox メッセージ ハンドラ

void CExtComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if (pDC == NULL) return;

	CRect rc = lpDrawItemStruct->rcItem;

	int clrBack = (lpDrawItemStruct->itemState & ODS_SELECTED) ? COLOR_HIGHLIGHT : COLOR_WINDOW;
	int clrText = (lpDrawItemStruct->itemState & ODS_SELECTED) ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT;

	if (!IsWindowEnabled())
		clrBack = COLOR_WINDOW,
		clrText = COLOR_3DFACE;

	COLORREF crBack = GetSysColor(clrBack);
	COLORREF crText = GetSysColor(clrText);

	DATA &ai = this->ai[lpDrawItemStruct->itemID & 255];

	CString text = ai.text + ai.suffix;

	CRect rctext = rc;
	rctext.DeflateRect(2, 0, 2, 0);

	pDC->FillSolidRect(rc, GetSysColor(COLOR_WINDOW));

	pDC->SetBkColor(crBack);
	pDC->SetTextColor(crText);

	pDC->DrawText(text, rctext, 0 |DT_LEFT |DT_VCENTER |DT_SINGLELINE);

	if (lpDrawItemStruct->itemState & ODS_FOCUS) {
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->SelectStockObject(BLACK_PEN);
		pDC->Rectangle(rc);
	}
}

void CExtComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{

}

void CExtComboBox::PreSubclassWindow() 
{
	CComboBox::PreSubclassWindow();

	LOGFONT lfMe;
	ReadFontTemplateFrom(lfMe, IDS_FONTTEMP_EXTTEXT, ";;;;;;;;;;;;;1");
	fontMe.DeleteObject();
	fontMe.CreateFontIndirect(&lfMe);

	{
		CClientDC clientDC(this);
		HGDIOBJ hOLD_font = clientDC.SelectObject(fontMe);
		clientDC.GetTextMetrics(&tm);
		clientDC.SelectObject(hOLD_font);
	}

	SetItemHeight(-1, tm.tmHeight + 2);
}

int CExtComboBox::OnCharToItem(UINT nChar, CListBox* pListBox, UINT nIndex) 
{
	ASSERT(nIndex == 0);

	while (nIndex < 256) {
		const CString &text = ai[nIndex].text;

		if (0 < text.GetLength()) {
			if (tolower(nChar) == tolower(text.GetAt(0)))
				return nIndex;
		}

		nIndex++;
	}
	return -2;
}

int CExtComboBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	return -1;
}

int CExtComboBox::FindIntData(LPARAM lp)
{
	for (int x=0; x<256; x++) {
		if (ai[x].lp == lp)
			return x;
	}
	return -1;
}

void CExtComboBox::Sort()
{
	std::sort(&ai[0], &ai[255]);
}

void CExtComboBox::Reset()
{
	for (int x=0; x<256; x++) {
		ai[x].lp = x;
		ai[x].text.Format("%03u", x);
		ai[x].suffix.Empty();
	}
}

void CExtComboBox::SetCount(int cx)
{
	while (cx < GetCount()) {
		DeleteString(GetCount() -1);
	}
	while (cx > GetCount()) {
		AddString("");
	}
}
