#if !defined(AFX_IMPORTRESDLG_H__D17BE755_B98B_44B0_ACCC_7CD17BF7F383__INCLUDED_)
#define AFX_IMPORTRESDLG_H__D17BE755_B98B_44B0_ACCC_7CD17BF7F383__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportResDlg.h : ヘッダー ファイル
//

#include "GDAT.h"

class CImportResRec
{
public:
	// 
	CString strText;

	// 
	CImportResRec() { }
	// 
	CImportResRec(LPCTSTR psz): strText(psz) { }
};

class CImportResRecList : public list<CImportResRec>
{
	// 
	void appendText(LPCTSTR pszText);

public:
	// 
	void ResAddSubject(LPCTSTR pszText);
	// 
	void ResAddText(LPCTSTR pszText);
	// 
	void ResAddEntryImpRes(const CGDEntry &pos, BOOL fRes);
	// 
	void ResAddUnsup8();
	// 
	void ResAddKillEntry(const CGDEntry &pos, bool fFromDAT);
	// 
	CString ResToStr();
};

/////////////////////////////////////////////////////////////////////////////
// CImportResDlg ダイアログ

class CImportResDlg : public CDialog, public CImportResRecList
{

public:
	// 
	CImportResDlg(CWnd* pParent = NULL);
	// 
	int DoModal(LPCTSTR pszRes);

// ダイアログ データ
	//{{AFX_DATA(CImportResDlg)
	enum { IDD = IDD_RES_IMPORT };
	CString	m_strLog;
	CString	m_strRes;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CImportResDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CImportResDlg)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_IMPORTRESDLG_H__D17BE755_B98B_44B0_ACCC_7CD17BF7F383__INCLUDED_)
