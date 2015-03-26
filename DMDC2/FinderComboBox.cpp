// FinderComboBox.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "FinderComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFinderComboBox

CFinderComboBox::CFinderComboBox()
{
}

CFinderComboBox::~CFinderComboBox()
{
}


BEGIN_MESSAGE_MAP(CFinderComboBox, CComboBox)
	//{{AFX_MSG_MAP(CFinderComboBox)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFinderComboBox メッセージ ハンドラ

int CFinderComboBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	// TODO: 指定された項目のソートの順序を決めるためのコードを追加してください
	// return -1 = 項目 2 の前に項目 1 をソート
	// return 0 = 項目 1 と項目 2 を同時にソート
	// return 1 = 項目 2 の後で項目 1 をソート

	return 0;
}
