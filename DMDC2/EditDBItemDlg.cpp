// EditDBItemDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EditDBItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CEditDBItemDlg::m_bRememberedUseHex = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CEditDBItemDlg ダイアログ

CEditDBItemDlg::CEditDBItemDlg(BOOL bLimitless, CWnd* pParent /*=NULL*/)
	: CDialog(CEditDBItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditDBItemDlg)
	m_iAlign = 0;
	//}}AFX_DATA_INIT

	m_bAdd = TRUE; m_bAddLimited = FALSE;
	m_bLimitless = bLimitless;
}

CEditDBItemDlg::CEditDBItemDlg(int iAlign, UINT iDB, const DMDBU &rec, BOOL bLimitless, CWnd* pParent)
	: CDialog(CEditDBItemDlg::IDD, pParent)
{
	m_iAlign = iAlign;

	m_rec = rec;
	m_iEditTab = iDB;
	m_bAdd = FALSE; m_bAddLimited = FALSE;
	m_bLimitless = bLimitless;
}

CEditDBItemDlg::CEditDBItemDlg(int iAlign, UINT iDB, BOOL bLimitless, CWnd* pParent)
	: CDialog(CEditDBItemDlg::IDD, pParent)
{
	m_iAlign = iAlign;

	m_iEditTab = iDB;
	m_bAdd = TRUE; m_bAddLimited = TRUE;
	m_bLimitless = bLimitless;
}

void CEditDBItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditDBItemDlg)
	DDX_Control(pDX, IDC_COMBO_NEXT_ITEM, m_wndNextItem);
	DDX_Control(pDX, IDC_STATIC_ALIGNMENT, m_wndAlignLabel);
	DDX_Control(pDX, IDC_COMBO_ALIGN, m_wndAlign);
	DDX_Control(pDX, IDC_TAB_DBSEL, m_wndTab);
	DDX_CBIndex(pDX, IDC_COMBO_ALIGN, m_iAlign);
	//}}AFX_DATA_MAP

	{
		CString s;
		m_wndNextItem.GetWindowText(s);
		LPTSTR lpsz;
		m_iNextItem = strtol(s, &lpsz, 16);
	}
}

BEGIN_MESSAGE_MAP(CEditDBItemDlg, CDialog)
	//{{AFX_MSG_MAP(CEditDBItemDlg)
	ON_UPDATE_COMMAND_UI(IDOK, OnUpdateOK)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_DBSEL, OnSelChangeTabDbsel)
	ON_BN_CLICKED(IDC_CHECK_HEX, OnCheckHex)
	ON_BN_CLICKED(IDC_CHECK_REMEMBER_HEX, OnCheckRememberHex)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditDBItemDlg メッセージ ハンドラ

BOOL CEditDBItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	static LPCTSTR lpszCate[] = {
		"DOOR",
		"TELE",
		"TEXT",
		"ACTU1",
		"MONS",
		"WEAP",
		"CLOT",
		"SCRO",
		"POTI",
		"CHES",
		"MISC",
		 "ACTU2",
		 "ACTU3",
		 "ACTU4",
		"PROJ",
		"---",
		NULL,
	};

	if (m_bAdd && !m_bAddLimited) {
		for (int i=0; lpszCate[i] != NULL; i++) {
			m_wndTab.InsertItem(i, lpszCate[i]);
		}
	} else {
		m_wndTab.InsertItem(0, lpszCate[m_iEditTab]);
	}

	m_iTab = 0;

	if (m_bAdd) {
		SetDlgItemText(IDOK, "Add");
		SetWindowText("Add an item");
	} else {
		SetWindowText("Edit an item");
	}
	if (m_iAlign < 0) {
		m_wndAlign.EnableWindow(FALSE);
		m_wndAlignLabel.EnableWindow(FALSE);
	}
	if (m_bLimitless) {
		CWnd *wnd;
		if (wnd = GetDlgItem(IDC_STATIC_NEXT_ITEM))
			wnd->EnableWindow(TRUE);
		if (wnd = GetDlgItem(IDC_COMBO_NEXT_ITEM))
			wnd->EnableWindow(TRUE);
	}

	CheckDlgButton(IDC_CHECK_HEX, (m_bUseHex = m_bRememberedUseHex) ? 1 : 0);
	for (int iTab = 0; iTab < 16; iTab++) {
		GetSub(iTab).UseHex(m_bUseHex);
	}

	ProcessTabChange();

	if (!m_bAdd) {
	//	CWnd *wnd;
	//	if (wnd = GetSub(m_iTab).GetWindow(GW_CHILD)) {
	//		while (wnd) {
	//			if (wnd->GetStyle() & WS_TABSTOP) {
	//				wnd->SetFocus();
	//				wnd->SendMessage(EM_SETSEL, 0, -1);
	//				return FALSE;
	//			}
	//			wnd = wnd->GetNextWindow();
	//		}
	//	}
		GetSub(m_iTab).SetFocus();
		GetSub(m_iTab).NextDlgCtrl();

		CString s;
		s.Format("%04X", (UINT)m_rec.r0.next.r.w);
		m_wndNextItem.AddString(s);
		m_wndNextItem.SetCurSel(0);
		return FALSE;
	}

	return TRUE;
}

void CEditDBItemDlg::OnOK() 
{
	CWnd &wnd = GetSub(m_iTab);
	if (!wnd.UpdateData())
		return;
	CDialog::OnOK();
}

void CEditDBItemDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CEditDBItemDlg::OnUpdateOK(CCmdUI* pCmdUI) 
{

}

void CEditDBItemDlg::OnSelChangeTabDbsel(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ProcessTabChange();
	*pResult = 0;
}

void CEditDBItemDlg::ProcessTabChange()
{
	const UINT nIDTemplates[] = {
		IDD_EDITDB00,
		IDD_EDITDB01,
		IDD_EDITDB02,
		IDD_EDITDB03,
		IDD_EDITDB04,
		IDD_EDITDB05,
		IDD_EDITDB06,
		IDD_EDITDB07,
		IDD_EDITDB08,
		IDD_EDITDB09,
		IDD_EDITDB10,
		 IDD_EDITDB03,
		 IDD_EDITDB03,
		 IDD_EDITDB03,
		IDD_EDITDB14,
		IDD_EDITDB15,
	};
	UINT i = m_wndTab.GetCurSel();
	if (m_bAdd && !m_bAddLimited) {
		for (UINT x=0; x<16; x++) {
			if (i != x) {
				CWnd &wnd = GetSub(x);
				if (wnd.m_hWnd && wnd.IsWindowVisible())
					wnd.ShowWindow(SW_HIDE);
			} else {
				CWnd &wnd = GetSub(m_iTab = x);
				if (::IsWindow(wnd.m_hWnd)) {

				} else {
					GetSub(x).InitAsNew(x);
					VERIFY(GetSub(x).Create(nIDTemplates[x], x, m_bLimitless, this));
					CRect rc;
					m_wndTab.GetWindowRect(&rc);
					m_wndTab.AdjustRect(FALSE, &rc);
					ScreenToClient(&rc);
					wnd.MoveWindow(&rc);
				}
				wnd.SetWindowPos(&m_wndTab, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
				wnd.ShowWindow(SW_SHOWNOACTIVATE);
				wnd.Invalidate();

				::EnableWindow(::GetDlgItem(*this, IDOK), GetSub(x).SaveCapable());
			}
		}
	} else {
		CWnd &wnd = GetSub(0);
		if (::IsWindow(wnd.m_hWnd)) {

		} else {
			if (!m_bAddLimited)
				ImportFrom(0, m_iEditTab, m_rec);

			VERIFY(GetSub(0).Create(nIDTemplates[m_iEditTab], m_iEditTab, m_bLimitless, this));
			CRect rc;
			m_wndTab.GetWindowRect(&rc);
			m_wndTab.AdjustRect(FALSE, &rc);
			ScreenToClient(&rc);
			wnd.MoveWindow(&rc);
		}
		wnd.SetWindowPos(&m_wndTab, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		wnd.ShowWindow(SW_SHOWNOACTIVATE);
		wnd.Invalidate();

		::EnableWindow(::GetDlgItem(*this, IDOK), GetSub(0).SaveCapable());
	}
}

void CEditDBItemDlg::ImportFrom(UINT iTab, UINT iDB, const DMDBU &rec)
{
	GetSub(iTab).m_ret.ReadFrom(iDB, rec);
#if 0
	switch (iDB) {
	case 0:
		GetSub(iTab).m_ret.m_item.r0.w2 = ((DMDB0 *)pAny)->w2;
		break;
	case 1:
		GetSub(iTab).m_ret.m_item.r1.w2 = ((DMDB1 *)pAny)->w2;
		GetSub(iTab).m_ret.m_item.r1.w4 = ((DMDB1 *)pAny)->w4;
		break;
	case 2:
		GetSub(iTab).m_ret.m_item.r2.w2 = ((DMDB2 *)pAny)->w2;
		break;
	case 3:
		GetSub(iTab).m_ret.m_item.r3.w2 = ((DMDB3 *)pAny)->w2;
		GetSub(iTab).m_ret.m_item.r3.w4 = ((DMDB3 *)pAny)->w4;
		GetSub(iTab).m_ret.m_item.r3.w6 = ((DMDB3 *)pAny)->w6;
		break;
	case 4:
		GetSub(iTab).m_ret.m_item.r4.b4 = ((DMDB4 *)pAny)->b4;
		GetSub(iTab).m_ret.m_item.r4.b5 = ((DMDB4 *)pAny)->b5;
		GetSub(iTab).m_ret.m_item.r4.w6[0] = ((DMDB4 *)pAny)->w6[0];
		GetSub(iTab).m_ret.m_item.r4.w6[1] = ((DMDB4 *)pAny)->w6[1];
		GetSub(iTab).m_ret.m_item.r4.w6[2] = ((DMDB4 *)pAny)->w6[2];
		GetSub(iTab).m_ret.m_item.r4.w6[3] = ((DMDB4 *)pAny)->w6[3];
		GetSub(iTab).m_ret.m_item.r4.w14 = ((DMDB4 *)pAny)->w14;
		break;
	case 5:
		GetSub(iTab).m_ret.m_item.r5.w2 = ((DMDB5 *)pAny)->w2;
		break;
	case 6:
		GetSub(iTab).m_ret.m_item.r6.w2 = ((DMDB6 *)pAny)->w2;
		break;
	case 7:
		GetSub(iTab).m_ret.m_item.r7.w2 = ((DMDB7 *)pAny)->w2;
		break;
	case 8:
		GetSub(iTab).m_ret.m_item.r8.w2 = ((DMDB8 *)pAny)->w2;
		break;
	case 9:
		GetSub(iTab).m_ret.m_item.r9.w4 = ((DMDB9 *)pAny)->w4;
		GetSub(iTab).m_ret.m_item.r9.b6[0] = ((DMDB9 *)pAny)->b6[0];
		GetSub(iTab).m_ret.m_item.r9.b6[1] = ((DMDB9 *)pAny)->b6[1];
		break;
	case 10:
		GetSub(iTab).m_ret.m_item.r10.w2 = ((DMDB10 *)pAny)->w2;
		break;
	case 11:
		break;
	case 12:
		break;
	case 13:
		break;
	case 14:
		GetSub(iTab).m_ret.m_item.r14.b4[0] = ((DMDB14 *)pAny)->b4[0];
		GetSub(iTab).m_ret.m_item.r14.b4[1] = ((DMDB14 *)pAny)->b4[0];
		GetSub(iTab).m_ret.m_item.r14.w6 = ((DMDB14 *)pAny)->w6;
		break;
	case 15:
		GetSub(iTab).m_ret.m_item.r15.w2 = ((DMDB15 *)pAny)->w2;
		break;
	}
#endif
}

void CEditDBItemDlg::OnCheckHex() 
{
	if (!UpdateData())
		return;
	m_bUseHex = IsDlgButtonChecked(IDC_CHECK_HEX);
	UINT x;
	for (x=0; x<16; x++) {
		CWnd &wnd = GetSub(x);
		if (wnd.m_hWnd && IsWindow(wnd)) {
			if (!GetSub(x).UpdateData()) {
				m_wndTab.SetCurSel(x);
				ProcessTabChange();
				CheckDlgButton(IDC_CHECK_HEX, (m_bUseHex = !m_bUseHex) ? 1 : 0);
				return;
			}
		}
	}
	for (x=0; x<16; x++) {
		CWnd &wnd = GetSub(x);
		if (wnd.m_hWnd && IsWindow(wnd)) {
			VERIFY(GetSub(x).UseHex(m_bUseHex));
		}
	}
}

void CEditDBItemDlg::OnCheckRememberHex() 
{
	m_bRememberUseHex = IsDlgButtonChecked(IDC_CHECK_REMEMBER_HEX);
}

void CEditDBItemDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	if (m_bRememberUseHex)
		m_bRememberedUseHex = m_bUseHex;
}
