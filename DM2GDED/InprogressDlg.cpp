// InprogressDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "InprogressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInprogressDlg ダイアログ


CInprogressDlg::CInprogressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInprogressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInprogressDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CInprogressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInprogressDlg)
	DDX_Control(pDX, IDC_PROGRESS_HINT, m_wndHint);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInprogressDlg, CDialog)
	//{{AFX_MSG_MAP(CInprogressDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInprogressDlg メッセージ ハンドラ
