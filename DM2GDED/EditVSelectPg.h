#if !defined(AFX_EditVSelectPg_H__92e0d64f_0131_41ab_b50d_4d6a4f1236b5__INCLUDED_)
#define AFX_EditVSelectPg_H__92e0d64f_0131_41ab_b50d_4d6a4f1236b5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVSelectPg.h : ヘッダー ファイル
//

#include "EditVPg.h"
#include "EditVxxx.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVSelectPg ダイアログ

class CEditVSelectPg : public CEditVPg, public CWordValCmd_Unsigned, public CEditVxxx
{
	// 
	UINT m_nFlags;

	// 
	virtual UINT GetVal()
	{
		return m_nFlags;
	}
	// 
	virtual LPCTSTR GetTemplateName() { return MAKEINTRESOURCE(IDD); }

public:
	// 
	DECL_WVCT_SPEC(wvctSelect)

	// 
	CEditVSelectPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVPg(vctx, pParent) { }
	// 
	void Construction();

	// 
	virtual CString FormatDisp(WORD x);
	// 
	virtual bool Parse(LPCSTR pszText, WORD &x);

	// 
	void SetParm1(LPCTSTR psz);

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }

// ダイアログ データ
	//{{AFX_DATA(CEditVSelectPg)
	enum { IDD = IDD_EDITV_SELECT };
	CListCtrl m_wndList;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditVSelectPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 
	class Sel {
	public:
		// 
		int iVal;
		// 
		CString strText, strImage;
	};
	// 
	CArray<Sel, Sel &> m_arrSels;
	// 
	CImageList m_il;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditVSelectPg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnListSelectDblClk(NMHDR *pNotifyStruct, LRESULT *result);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EditVSelectPg_H__92e0d64f_0131_41ab_b50d_4d6a4f1236b5__INCLUDED_)
