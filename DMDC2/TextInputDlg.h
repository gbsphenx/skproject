#if !defined(AFX_TEXTINPUTDLG_H__09934EBF_458F_45FB_B31F_5C25879DA9AB__INCLUDED_)
#define AFX_TEXTINPUTDLG_H__09934EBF_458F_45FB_B31F_5C25879DA9AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextInputDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CTextInputDlg ダイアログ

class CTextInputDlg : public CDialog
{
	CFont fontMe;

// コンストラクション
public:
	CTextInputDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CTextInputDlg)
	enum { IDD = IDD_TEXTINPUT };
	CEdit	m_wndEdit;
	CString	m_strText;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTextInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTextInputDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TEXTINPUTDLG_H__09934EBF_458F_45FB_B31F_5C25879DA9AB__INCLUDED_)
