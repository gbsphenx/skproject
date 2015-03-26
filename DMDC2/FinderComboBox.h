#if !defined(AFX_FINDERCOMBOBOX_H__6EB81188_FD62_4B4A_8F45_9F7EED9B4B03__INCLUDED_)
#define AFX_FINDERCOMBOBOX_H__6EB81188_FD62_4B4A_8F45_9F7EED9B4B03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FinderComboBox.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CFinderComboBox ウィンドウ

class CFinderComboBox : public CComboBox
{
// コンストラクション
public:
	CFinderComboBox();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CFinderComboBox)
	public:
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CFinderComboBox();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CFinderComboBox)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FINDERCOMBOBOX_H__6EB81188_FD62_4B4A_8F45_9F7EED9B4B03__INCLUDED_)
