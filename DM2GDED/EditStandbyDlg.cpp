// EditStandbyDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditStandbyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditStandbyDlg ダイアログ


CEditStandbyDlg::CEditStandbyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditStandbyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditStandbyDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CEditStandbyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditStandbyDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditStandbyDlg, CDialog)
	//{{AFX_MSG_MAP(CEditStandbyDlg)
	ON_BN_CLICKED(IDCLOSE, OnClose)
	ON_BN_CLICKED(IDRETRY, OnRetry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditStandbyDlg メッセージ ハンドラ

void CEditStandbyDlg::OnClose() 
{
	EndDialog(IDCLOSE);
}

void CEditStandbyDlg::OnRetry() 
{
	EndDialog(IDRETRY);
}
