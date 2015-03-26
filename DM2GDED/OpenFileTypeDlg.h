#if !defined(AFX_OPENFILETYPEDLG_H__0631E734_BE72_4831_9E11_2917091E69D5__INCLUDED_)
#define AFX_OPENFILETYPEDLG_H__0631E734_BE72_4831_9E11_2917091E69D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenFileTypeDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// COpenFileTypeDlg ダイアログ

class COpenFileTypeDlg : public CDialog
{
	// 
	struct OpenTbl
	{	// 
		LPCSTR pszName;
		// 
		BOOL fLE;
		// 
		int iLang;
		// 
		int iPal;
		// 
		int iSnd;
		// 
		int iPicFmt;
		// 
		BOOL fRestrictPal;
	};
	// 
	static OpenTbl m_tblOpen[];

public:
	// 
	COpenFileTypeDlg(LPCTSTR pszFileIn, CWnd* pParent = NULL);   // 標準のコンストラクタ
	// 
	UINT GetGDOF();

// ダイアログ データ
	//{{AFX_DATA(COpenFileTypeDlg)
	enum { IDD = IDD_QUERY_FILE_TYPE };
	CComboBox	m_wndPlatform;
	int		m_iLangSel;
	int		m_iPlatformSel;
	CString	m_strFileIn;
	int		m_iSndSel;
	int		m_iPalSel;
	BOOL	m_fRestrictPal;
	int		m_iPicFmtSel;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(COpenFileTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(COpenFileTypeDlg)
	afx_msg void OnSelchangeComboPlatform();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRetry();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPENFILETYPEDLG_H__0631E734_BE72_4831_9E11_2917091E69D5__INCLUDED_)
