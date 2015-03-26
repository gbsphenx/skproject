#if !defined(AFX_DIFFEXPORTDLG_H__E137BEF2_A54A_4A61_83D2_E7094EB8D7F0__INCLUDED_)
#define AFX_DIFFEXPORTDLG_H__E137BEF2_A54A_4A61_83D2_E7094EB8D7F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiffExportDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDiffExportDlg ダイアログ

class CDiffExportDlg : public CDialog
{
// コンストラクション
public:
	CDiffExportDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDiffExportDlg)
	enum { IDD = IDD_DIFF_EXPORT };
	CString	m_strFileMod;
	CString	m_strFileOrg;
	CString	m_strOutput;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDiffExportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDiffExportDlg)
	afx_msg void OnButtonRefOrg();
	afx_msg void OnButtonRefOut();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DIFFEXPORTDLG_H__E137BEF2_A54A_4A61_83D2_E7094EB8D7F0__INCLUDED_)
