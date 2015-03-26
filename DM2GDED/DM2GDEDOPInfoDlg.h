#if !defined(AFX_DM2GDEDOPINFODLG_H__A97B350C_D5EA_4C1B_A058_069D183E8EA4__INCLUDED_)
#define AFX_DM2GDEDOPINFODLG_H__A97B350C_D5EA_4C1B_A058_069D183E8EA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2GDEDOPInfoDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDOPInfoDlg ダイアログ

class CDM2GDEDOPInfoDlg : public CDialog
{

public:
	// 
	CDM2GDEDOPInfoDlg(CWnd* pParent = NULL);
	// 
	void SetGDOF(DWORD nGDOF);

// ダイアログ データ
	//{{AFX_DATA(CDM2GDEDOPInfoDlg)
	enum { IDD = IDD_OPEN_INFO };
	CString	m_strLE;
	CString	m_strPath;
	CString	m_strPM;
	CString	m_strRP;
	CString	m_strST;
	CString	m_strTL;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDM2GDEDOPInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDM2GDEDOPInfoDlg)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2GDEDOPINFODLG_H__A97B350C_D5EA_4C1B_A058_069D183E8EA4__INCLUDED_)
