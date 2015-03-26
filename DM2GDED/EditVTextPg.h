#if !defined(AFX_EDITVTEXTPG_H__5A0C4406_FD8C_48D1_B2F0_7CF108F1E16F__INCLUDED_)
#define AFX_EDITVTEXTPG_H__5A0C4406_FD8C_48D1_B2F0_7CF108F1E16F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVTextPg.h : ヘッダー ファイル
//

#include "EditVPg.h"
#include "EditVxxx.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVTextPg ダイアログ

class CEditVTextPg : public CEditVPg, public CWordValCmd_Unsigned, public CEditVxxx
{
public:
	// 
	DECL_WVCT_SPEC(wvctText)

	// 
	CEditVTextPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVPg(vctx, pParent) { }
	// 
	virtual void Construction();
	// 
	virtual UINT GetVal()
	{
		return 0;
	}
	// 
	virtual LPCTSTR GetText() { return m_strText; }
	// 
	virtual LPCTSTR GetTemplateName() { return MAKEINTRESOURCE(IDD); }

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }

	// 
	virtual void OnCancel() {
		if (GetParent() != NULL)
			GetParent()->SendMessage(WM_COMMAND, IDCANCEL);
	}

// ダイアログ データ
	//{{AFX_DATA(CEditVTextPg)
	enum { IDD = IDD_EDITV_TEXT };
	CString	m_strText;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditVTextPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditVTextPg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITVTEXTPG_H__5A0C4406_FD8C_48D1_B2F0_7CF108F1E16F__INCLUDED_)
