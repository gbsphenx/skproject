// DM2Ornate1ComboBox.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "DM2Ornate1ComboBox.h"
#include "CompatDC.h"
#include "DM2NameDic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2Ornate1ComboBox

CDM2Ornate1ComboBox::CDM2Ornate1ComboBox()
{

}

BEGIN_MESSAGE_MAP(CDM2Ornate1ComboBox, CComboBox)
	//{{AFX_MSG_MAP(CDM2Ornate1ComboBox)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2Ornate1ComboBox メッセージ ハンドラ

void CDM2Ornate1ComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rc = lpDrawItemStruct->rcItem;
	UINT i = lpDrawItemStruct->itemID;
	int z = -3;
	if (i < m_v.GetSize()) z = (int)m_v[i];

	if (pDC == NULL) return;

	BOOL fSel = (lpDrawItemStruct->itemState & ODS_SELECTED);

	COLORREF clrBack = GetSysColor(fSel ? COLOR_HIGHLIGHT : COLOR_WINDOW);
	COLORREF clrText = GetSysColor(fSel ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT);

	CBrush br;
	br.CreateSolidBrush(clrBack);
	pDC->FillRect(&rc, &br);

	CRect rcBox = rc;
	rcBox.DeflateRect(1, 1);

	pDC->SetBkColor(clrBack);
	pDC->SetTextColor(clrText);

	if (z == -1) {
		pDC->DrawText(_T("(No)"), &rcBox, 0 |DT_LEFT |DT_VCENTER |DT_SINGLELINE);
	} else if (z == -2) {
		pDC->DrawText(_T("(Add new one...)"), &rcBox, 0 |DT_LEFT |DT_VCENTER |DT_SINGLELINE);
	} else if (z == -3) {
		pDC->DrawText(_T("?"), &rcBox, 0 |DT_LEFT |DT_VCENTER |DT_SINGLELINE);
	} else  {
		BYTE x;
		CString str;
		if (m_fAll) {
			x = z;
			str.Format("&H%02X:", 0 +z);
		} else {
			x = m_oia[z];
			str.Format("%1X:", 1 +z);
		}

		CRect rcText = rcBox;

		pDC->DrawText(str, &rcBox, 0 |DT_LEFT |DT_VCENTER |DT_SINGLELINE |DT_NOPREFIX);
		CSize size = pDC->GetTextExtent(str);
		rcBox.left += size.cx + 1;

		BYTE k = m_t;

		GetApp()->RenderOrnate64To(pDC, rcBox.left, rcBox.top, k, x, 64);

		if (m_fAddText) {
			rcText.top += 64 + 1;

			CString strName = CDM2NameDic::GetInstance().GetDATDBName(k, x);

			pDC->SetTextColor(fSel ? (clrBack ^ 0xFFFFFF) : clrText);
			pDC->DrawText(strName, rcText, 0 |DT_LEFT |DT_VCENTER |DT_SINGLELINE |DT_NOPREFIX);
		}
	}
}

void CDM2Ornate1ComboBox::CtlInit()
{
	UINT nOld = GetCount();
	UINT nNew = m_v.GetSize();

	int i, n = nNew - nOld;

	CString str;

	CCompatDC dcMem(this);
	TEXTMETRIC tm;
	dcMem.SelectStockObject(DEFAULT_GUI_FONT);
	dcMem.GetTextMetrics(&tm);

	if (m_fAddText) {
		m_cy = 64 + 3 + tm.tmHeight + tm.tmInternalLeading + tm.tmExternalLeading;
	} else {
		m_cy = 66;
	}

	SetItemHeight(-1, m_cy);
	SetItemHeight(0, m_cy);
	for (; n < 0; n++) DeleteString(GetCount() - 1);
	for (; n > 0; n--) str.Format("#%03u", 0U +GetCount()), i = AddString(str);
}

void CDM2Ornate1ComboBox::Reset()
{
	m_v.RemoveAll();

	UINT i;

	if (m_fHasNoop) m_v.Add((UINT)-1);

	if (m_fAll) {
		for (i = 0; i < 256; i++) m_v.Add(i);
	} else {
		UINT n = _MIN(0U +m_oia.Size(), m_nMaxOrnate);

		for (i = 0; i < n; i++) m_v.Add(i);

		if (m_fHasAddNew1 && i < m_nMaxOrnate) m_v.Add((UINT)-2);
	}
}

void CDM2Ornate1ComboBox::Revert(CDM2OrnateIdxArray &oia, bool fRevert)
{
	if (fRevert) {
		m_oia = oia;
	} else {
		oia = m_oia;
	}
}

bool CDM2Ornate1ComboBox::IsAddNew1(int iSel)
{
	UINT i = iSel;
	if (m_v.GetSize() <= i) return false;

	return m_v[i] == -2;
}

int CDM2Ornate1ComboBox::GetOrnateIdx(int iSel)
{
	UINT i = iSel;
	if (m_v.GetSize() <= i || m_v[i] < 0) return -1;

	return m_v[i];
}

void CDM2Ornate1ComboBox::Freshen()
{
	Reset();
	CtlInit();
	Invalidate();
}

void CDM2Ornate1ComboBox::Sel(int t)
{
	m_fHasNoop = true;
	m_fHasAddNew1 = true;
//	m_nMaxOrnate = (t == 0x0B) ? 7 : 15;
	m_nMaxOrnate = 15;
	m_fAll = false;
	m_t = t;
	m_fAddText = (t == 0x0B || t == 0x0E) ? false : true;
}

void CDM2Ornate1ComboBox::All(int t)
{
	m_fHasNoop = false;
	m_fHasAddNew1 = false;
	m_nMaxOrnate = 0;
	m_fAll = true;
	m_t = t;
	m_fAddText = (t == 0x0B || t == 0x0E) ? false : true;
}
