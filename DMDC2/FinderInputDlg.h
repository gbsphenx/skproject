#if !defined(AFX_FINDERINPUTDLG_H__22EEA564_86D4_4A54_834B_D74CD3988FE3__INCLUDED_)
#define AFX_FINDERINPUTDLG_H__22EEA564_86D4_4A54_834B_D74CD3988FE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FinderInputDlg.h : ヘッダー ファイル
//

#include "FinderComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CFinderInputDlg ダイアログ

class CFinderInputDlg : public CDialog
{
// コンストラクション
public:
	// 
	UINT m_iFindData;

	// 
	CFinderInputDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CFinderInputDlg)
	enum { IDD = IDD_FINDER_INPUT };
	CFinderComboBox	m_wndSel;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CFinderInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CFinderInputDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkComboSel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FINDERINPUTDLG_H__22EEA564_86D4_4A54_834B_D74CD3988FE3__INCLUDED_)
