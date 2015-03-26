#if !defined(AFX_IMPORTQUERYDLG_H__61520CCC_4495_42AE_99CA_D8FD6E383AF2__INCLUDED_)
#define AFX_IMPORTQUERYDLG_H__61520CCC_4495_42AE_99CA_D8FD6E383AF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportQueryDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CImportQueryDlg ダイアログ

class CImportQueryDlg : public CDialog
{
public:
	// 
	CImportQueryDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	// 
	bool IfOverwrite() { return m_iOverwriteSel == 0; }

// ダイアログ データ
	//{{AFX_DATA(CImportQueryDlg)
	enum { IDD = IDD_QUERY_IMPORT };
	int		m_iOverwriteSel;
	CString	m_strFileIn;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CImportQueryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CImportQueryDlg)
	afx_msg void OnButtonBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_IMPORTQUERYDLG_H__61520CCC_4495_42AE_99CA_D8FD6E383AF2__INCLUDED_)
