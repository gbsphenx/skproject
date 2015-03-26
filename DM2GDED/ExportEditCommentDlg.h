#if !defined(AFX_EXPORTEDITCOMMENTDLG_H__0FAB3415_4B1B_46BE_8610_320A3476B7DF__INCLUDED_)
#define AFX_EXPORTEDITCOMMENTDLG_H__0FAB3415_4B1B_46BE_8610_320A3476B7DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportEditCommentDlg.h : ヘッダー ファイル
//

#include "ExportDATHintDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CExportEditCommentDlg ダイアログ

class CExportEditCommentDlg : public CDialog
{
public:
	// 
	CExportEditCommentDlg(CWnd* pParent = NULL);
	// 
	void Revert(CExportDATComment &rEx, bool fRevert);

// ダイアログ データ
	//{{AFX_DATA(CExportEditCommentDlg)
	enum { IDD = IDD_EDIT_COMMENT };
	CString	m_strAuthor;
	CString	m_strComment;
	CString	m_strDate;
	CString	m_strName;
	CString	m_strPlatform;
	CString	m_strVer;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CExportEditCommentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CExportEditCommentDlg)
	afx_msg void OnButtonToday();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EXPORTEDITCOMMENTDLG_H__0FAB3415_4B1B_46BE_8610_320A3476B7DF__INCLUDED_)
