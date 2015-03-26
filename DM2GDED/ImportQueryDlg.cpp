// ImportQueryDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "ImportQueryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportQueryDlg ダイアログ


CImportQueryDlg::CImportQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportQueryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportQueryDlg)
	m_iOverwriteSel = 0;
	m_strFileIn = _T("");
	//}}AFX_DATA_INIT
}


void CImportQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportQueryDlg)
	DDX_CBIndex(pDX, IDC_COMBO_OVERWRITE, m_iOverwriteSel);
	DDX_Text(pDX, IDC_EDIT_FILE_IN, m_strFileIn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportQueryDlg, CDialog)
	//{{AFX_MSG_MAP(CImportQueryDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportQueryDlg メッセージ ハンドラ

void CImportQueryDlg::OnButtonBrowse() 
{
	if (!UpdateData()) return;

	const DWORD nFlags = 0 |OFN_ENABLESIZING |OFN_EXPLORER |OFN_FILEMUSTEXIST |OFN_HIDEREADONLY |OFN_LONGNAMES |OFN_PATHMUSTEXIST |OFN_READONLY;
	CFileDialog wndDlg(TRUE, "ini", m_strFileIn, nFlags, LoadString(IDS_STR_FILTER_INI), GetActiveWindow());
	int r = wndDlg.DoModal();
	if (r == IDOK) {
		m_strFileIn = wndDlg.GetPathName();
		UpdateData(FALSE);
	}
}
