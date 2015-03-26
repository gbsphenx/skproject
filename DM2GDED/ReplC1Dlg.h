#if !defined(AFX_REPLC1DLG_H__4B3837D9_4D79_44D7_B347_8DCCD269A401__INCLUDED_)
#define AFX_REPLC1DLG_H__4B3837D9_4D79_44D7_B347_8DCCD269A401__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReplC1Dlg.h : ヘッダー ファイル
//

#include "GDEntry.h"

/////////////////////////////////////////////////////////////////////////////
// CReplC1Dlg ダイアログ

class CReplC1Dlg : public CDialog
{
public:
	// 
	CGDEntry m_from, m_to;

	// 
	CReplC1Dlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CReplC1Dlg)
	enum { IDD = IDD_REPL_C1 };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CReplC1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CReplC1Dlg)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_REPLC1DLG_H__4B3837D9_4D79_44D7_B347_8DCCD269A401__INCLUDED_)
