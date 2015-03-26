// SKTextGridCtrl.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "SKTextGridCtrl.h"
#include "SelCMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//

IMPLEMENT_DYNCREATE(CSKTextCell, CGridCell);

/////////////////////////////////////////////////////////////////////////////
// 

IMPLEMENT_DYNCREATE(CSKTextCMCell, CSKTextCell);

void CSKTextCMCell::OnDblClick(CPoint PointCellRelative)
{

}

BOOL CSKTextCMCell::Edit( int /* nRow */, int /* nCol */, CRect /* rect */, CPoint /* point */, 
                   UINT /* nID */, UINT /* nChar */)
{
	CSelCMDlg wndDlg(CWnd::GetActiveWindow());
	wndDlg.m_iSel = iSel;
	if (skc == skcPA) wndDlg.m_pTable = g_DM2SkPA;

	if (wndDlg.DoModal() == IDOK) {
		iSel = wndDlg.m_iSel;
		
		GetGrid()->Invalidate();
	}
	return true;
}

CString CSKTextCMCell::GetValue() const 
{
	CString tempStr;

	tempStr.Format("%d", iSel);
	return tempStr;
}

void CSKTextCMCell::SetValue(LPCTSTR szText)
{
	iSel = _ttoi(szText);
}

LPCTSTR CSKTextCMCell::GetText() const 
{
	if (skc == skcPA) return CDM2SkPAUtil::GetNameOfId(iSel);

	return CDM2SkCMUtil::GetNameOfId(iSel);
}

/////////////////////////////////////////////////////////////////////////////
// CSKTextGridCtrl

BEGIN_MESSAGE_MAP(CSKTextGridCtrl, CGridCtrl)
	//{{AFX_MSG_MAP(CSKTextGridCtrl)
	ON_WM_CREATE()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSKTextGridCtrl メッセージ ハンドラ

void CSKTextGridCtrl::PreSubclassWindow() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	CGridCtrl::PreSubclassWindow();
}

void CSKTextGridCtrl::CreateList(const CDM2SkCmdList &r, const CDM2SkCmdDesc *pSkDesc)
{
	DeleteAllItems();

	EnableSelection(FALSE);
	EnableTitleTips(FALSE);

	SetColumnCount(3);
	SetRowCount(1);

	SetFixedRowCount(1);

	SetItemText(0, 0, "Parm");
	SetItemText(0, 1, "Description");
	SetItemText(0, 2, "Value");

	for (int i = 0; i < DM2SKCMD_MAX_CMD; i++) {
		int iRow = InsertRow(CDM2SkCmdList::GetName(i));

		SetItemState(iRow, 0, GVIS_READONLY);
		SetItemState(iRow, 1, GVIS_READONLY);

		if (pSkDesc) SetItemText(iRow, 1, pSkDesc->GetDesc(i));

		CRuntimeClass *pRTC = RUNTIME_CLASS(CSKTextCell);
		if (i == skcCM || i == skcPA) pRTC = RUNTIME_CLASS(CSKTextCMCell);

		SetCellType(iRow, 2, pRTC);
		CSKTextCell *pCell = static_cast<CSKTextCell *>(GetCell(iRow, 2));

		CString strVal; strVal.Format("%d", r.nVal[i]);
		pCell->SetValue(strVal);

		if (i == skcPA) static_cast<CSKTextCMCell *>(pCell)->skc = skcPA;
	}

	AutoSizeColumn(0);
	AutoSizeColumn(1);
	AutoSizeColumn(2);

	SetFocusCell(1, 0);
}

void CSKTextGridCtrl::SaveList(CDM2SkCmdList &r)
{
	for (int i = 0; i < DM2SKCMD_MAX_CMD; i++) {
		int iRow = 1 + i;
		CSKTextCell *pCell = static_cast<CSKTextCell *>(GetCell(iRow, 2));
		r.nVal[i] = _ttoi(pCell->GetValue());
	}
}

int CSKTextGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetGridBkColor(GetSysColor(COLOR_WINDOW));
	
	return 0;
}

void CSKTextGridCtrl::OnSysColorChange() 
{
	CGridCtrl::OnSysColorChange();
	
	SetGridBkColor(GetSysColor(COLOR_WINDOW));
	
}
