// DiffImportDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DiffImportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const DWORD nDefOpenFlags = 0
	|OFN_ENABLESIZING
	|OFN_EXPLORER
	|OFN_FILEMUSTEXIST
	|OFN_HIDEREADONLY
	|OFN_LONGNAMES
	|OFN_PATHMUSTEXIST
	;

/////////////////////////////////////////////////////////////////////////////
// CDiffImportDlg ダイアログ


CDiffImportDlg::CDiffImportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDiffImportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiffImportDlg)
	m_strFileDiff = _T("");
	m_strFileOrg = _T("");
	//}}AFX_DATA_INIT
}


void CDiffImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiffImportDlg)
	DDX_Text(pDX, IDC_EDIT_FILE_DIFF, m_strFileDiff);
	DDX_Text(pDX, IDC_EDIT_FILE_ORG, m_strFileOrg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiffImportDlg, CDialog)
	//{{AFX_MSG_MAP(CDiffImportDlg)
	ON_BN_CLICKED(IDC_BUTTON_REF_DIFF, OnButtonRefDiff)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiffImportDlg メッセージ ハンドラ

void CDiffImportDlg::OnButtonRefDiff() 
{
	if (!UpdateData()) return;

	CFileDialog wndDlg(TRUE, _T("ini"), m_strFileDiff, nDefOpenFlags, "Diff.ini (*.ini)|*.ini||", this);

	if (wndDlg.DoModal() == IDOK) {
		m_strFileDiff = wndDlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CDiffImportDlg::OnOK() 
{
	int r = AfxMessageBox(IDS_WARN_IMPORT_DIFF, 0 |MB_ICONEXCLAMATION |MB_RETRYCANCEL);
	if (r != IDRETRY) return;
	
	CDialog::OnOK();
}
