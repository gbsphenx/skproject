#if !defined(AFX_SELCMDLG_H__41B887C6_7817_42EF_97EC_82E56A06D484__INCLUDED_)
#define AFX_SELCMDLG_H__41B887C6_7817_42EF_97EC_82E56A06D484__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelCMDlg.h : ヘッダー ファイル
//

#include "DM2SkCmdList.h"

/////////////////////////////////////////////////////////////////////////////
// CSelCMDlg ダイアログ

class CSelCMDlg : public CDialog
{
	// 
	CImageList m_pics;

public:
	// 
	int m_iSel;
	// 
	const CDM2SkCM *m_pTable;

// コンストラクション
public:
	CSelCMDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSelCMDlg)
	enum { IDD = IDD_SEL_CM };
	CListCtrl	m_wndCM;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSelCMDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSelCMDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkListCm(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SELCMDLG_H__41B887C6_7817_42EF_97EC_82E56A06D484__INCLUDED_)
