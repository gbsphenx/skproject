// SelCMDlg.cpp : インプリメンテーション ファイル
//

#include "StdAfx.h"
#include "DM2GDED.h"
#include "SelCMDlg.h"
#include "DM2SkCmdList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelCMDlg ダイアログ


CSelCMDlg::CSelCMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelCMDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelCMDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

	m_iSel = -1;
	m_pTable = g_DM2SkCM;
}


void CSelCMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelCMDlg)
	DDX_Control(pDX, IDC_LIST_CM, m_wndCM);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelCMDlg, CDialog)
	//{{AFX_MSG_MAP(CSelCMDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CM, OnDblclkListCm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelCMDlg メッセージ ハンドラ

BOOL CSelCMDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pics.Create(16, 16, ILC_COLOR | ILC_MASK, 0, 20);

	m_wndCM.SetImageList(&m_pics, LVSIL_SMALL);

	m_wndCM.InsertColumn(0, "Effect", LVCFMT_LEFT, CalcWidthOfUIText(55));

	for (int ci=0; m_pTable[ci].pszName != NULL; ci++) {
		int iIco = m_pics.Add(AfxGetApp()->LoadIcon(m_pTable[ci].iIcoRes));
		int r = m_wndCM.InsertItem(ci, m_pTable[ci].pszName, iIco);
		int iID = m_pTable[ci].i;
		m_wndCM.SetItemData(r, iID);
	}

	m_wndCM.SetExtendedStyle(m_wndCM.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	LVFINDINFO lvfi;
	lvfi.flags = LVFI_PARAM;
	lvfi.lParam = m_iSel;
	int r = m_wndCM.FindItem(&lvfi);
	if (r != -1) {
		m_wndCM.SetItemState(r, 0|LVIS_SELECTED|LVIS_FOCUSED, 0|LVIS_SELECTED|LVIS_FOCUSED);
		m_wndCM.EnsureVisible(r, false);
	}
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CSelCMDlg::OnOK() 
{
	int iSel = m_wndCM.GetNextItem(-1, LVNI_SELECTED);
	if (iSel < 0) {

	} else {
		int r = (int)m_wndCM.GetItemData(iSel);
		m_iSel = r;
		CDialog::OnOK();
	}
	
}

void CSelCMDlg::OnDblclkListCm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int iSel = m_wndCM.GetNextItem(-1, LVNI_SELECTED);
	if (iSel < 0) {
		EndDialog(IDCANCEL);
	} else {
		int r = (int)m_wndCM.GetItemData(iSel);
		m_iSel = r;
		EndDialog(IDOK);
	}
	
	*pResult = 0;
}
