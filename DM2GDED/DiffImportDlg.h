#if !defined(AFX_DIFFIMPORTDLG_H__198313CB_61BE_42AD_B3FD_4F4DB0F935A8__INCLUDED_)
#define AFX_DIFFIMPORTDLG_H__198313CB_61BE_42AD_B3FD_4F4DB0F935A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiffImportDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDiffImportDlg ダイアログ

class CDiffImportDlg : public CDialog
{
// コンストラクション
public:
	CDiffImportDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDiffImportDlg)
	enum { IDD = IDD_DIFF_IMPORT };
	CString	m_strFileDiff;
	CString	m_strFileOrg;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDiffImportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDiffImportDlg)
	afx_msg void OnButtonRefDiff();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DIFFIMPORTDLG_H__198313CB_61BE_42AD_B3FD_4F4DB0F935A8__INCLUDED_)
