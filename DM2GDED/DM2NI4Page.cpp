// DM2NI4Page.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2NI4Page.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2NI4Page プロパティ ページ

IMPLEMENT_DYNCREATE(CDM2NI4Page, CDM2NIBPage)

CDM2NI4Page::CDM2NI4Page() : CDM2NIBPage(CDM2NI4Page::IDD)
{
	//{{AFX_DATA_INIT(CDM2NI4Page)
		// メモ - ClassWizard はこの位置にメンバの初期化処理を追加します。
	//}}AFX_DATA_INIT
}

CDM2NI4Page::~CDM2NI4Page()
{
}

void CDM2NI4Page::DoDataExchange(CDataExchange* pDX)
{
	CDM2NIBPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDM2NI4Page)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDM2NI4Page, CDM2NIBPage) // CDM2NIBPage
	//{{AFX_MSG_MAP(CDM2NI4Page)
		// メモ: ClassWizard はこの位置に DDX および DDV の呼び出しコードを追加します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2NI4Page メッセージ ハンドラ

BOOL CDM2NI4Page::OnSetActive() 
{
	if (!CDM2NIBPage::OnSetActive()) AfxThrowNotSupportedException();

	UpdateVw();
	
	return TRUE;
}

void CDM2NI4Page::UpdateVw()
{
	m_frame.m_view.MakeStaged(3);
}
