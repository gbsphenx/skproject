// EdPINoPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdPINoPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdPINoPage プロパティ ページ

IMPLEMENT_DYNCREATE(CEdPINoPage, CEdPIDlg)

CEdPINoPage::CEdPINoPage() : CEdPIDlg(CEdPINoPage::IDD)
{
	//{{AFX_DATA_INIT(CEdPINoPage)
		// メモ - ClassWizard はこの位置にメンバの初期化処理を追加します。
	//}}AFX_DATA_INIT
}

void CEdPINoPage::DoDataExchange(CDataExchange* pDX)
{
	CEdPIDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdPINoPage)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEdPINoPage, CEdPIDlg)
	//{{AFX_MSG_MAP(CEdPINoPage)
		// メモ: ClassWizard はこの位置に DDX および DDV の呼び出しコードを追加します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdPINoPage メッセージ ハンドラ
