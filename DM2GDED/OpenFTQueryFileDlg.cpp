// OpenFTQueryFileDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "OpenFTQueryFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenFTQueryFileDlg ダイアログ

COpenFTQueryFileDlg::COpenFTQueryFileDlg(
	BOOL bOpenFileDialog,
	LPCTSTR lpszDefExt,
	LPCTSTR lpszFileName,
	DWORD dwFlags,
	LPCTSTR lpszFilter,
	CWnd* pParentWnd,
	DWORD dwSize
)
: CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	//{{AFX_DATA_INIT(COpenFTQueryFileDlg)
	m_fUse = FALSE;
	//}}AFX_DATA_INIT

	if (bOpenFileDialog) {
		m_ofn.Flags |= OFN_ENABLETEMPLATE;
		m_ofn.hInstance = AfxGetInstanceHandle();
		m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD);

		m_fUse = GetApp()->GetUseDetective();
	}
}

void COpenFTQueryFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenFTQueryFileDlg)
	DDX_Check(pDX, IDC_CHECK_USE_DETECTIVE, m_fUse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenFTQueryFileDlg, CFileDialog)
	//{{AFX_MSG_MAP(COpenFTQueryFileDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenFTQueryFileDlg メッセージ ハンドラ

void COpenFTQueryFileDlg::OnDestroy() 
{
	CFileDialog::OnDestroy();
	
	UpdateData();
	
}
