#if !defined(AFX_OPENFTQUERYFILEDLG_H__445CEB88_CE7D_41F3_A11D_F02028E98C39__INCLUDED_)
#define AFX_OPENFTQUERYFILEDLG_H__445CEB88_CE7D_41F3_A11D_F02028E98C39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenFTQueryFileDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// COpenFTQueryFileDlg ダイアログ

class COpenFTQueryFileDlg : public CFileDialog
{
public:
	// 
	COpenFTQueryFileDlg(
		BOOL bOpenFileDialog,
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL,
		DWORD dwSize = 0
	);

// ダイアログ データ
	//{{AFX_DATA(COpenFTQueryFileDlg)
	enum { IDD = IDD_QUERY_OF };
	BOOL	m_fUse;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(COpenFTQueryFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(COpenFTQueryFileDlg)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPENFTQUERYFILEDLG_H__445CEB88_CE7D_41F3_A11D_F02028E98C39__INCLUDED_)
