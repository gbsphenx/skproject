#if !defined(AFX_EditVRuneSel_H__76BC8909_4957_481D_B107_E112180DC71E__INCLUDED_)
#define AFX_EditVRuneSel_H__76BC8909_4957_481D_B107_E112180DC71E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVRuneSel.h : ヘッダー ファイル
//

#include "EditVPg.h"
#include "EditVxxx.h"
#include "RuneSelStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVRuneSelPg ダイアログ

class CEditVRuneSelPg : public CEditVPg, public CWordValCmd, public CEditVxxx
{
public:
	// 
	DECL_WVCT_SPEC(wvctRuneSel)

	// 
	CEditVRuneSelPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVPg(vctx, pParent) { }
	// 
	virtual void Construction();
	// 
	virtual UINT GetVal()
	{
		int v = m_wndRuneSel.GetSel();
		if (v < 0)
			return 0;
		return v +0x66;
	}
	// 
	virtual LPCTSTR GetTemplateName() { return MAKEINTRESOURCE(IDD); }

	// 
	virtual CString Format(WORD x);
	// 
	virtual CString FormatDisp(WORD x);
	// 
	virtual bool Parse(LPCSTR pszText, WORD &x);

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }

	// 
	virtual LPCTSTR GetHintText(int i)
	{
		return NULL;
	}
	// 
	virtual void SetParm1(LPCSTR psz);

// ダイアログ データ
	//{{AFX_DATA(CEditVRuneSelPg)
	enum { IDD = IDD_EDITV_RUNE };
	CRuneSelStatic	m_wndRuneSel;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditVRuneSelPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 
	CString FormatB3(int iTextIdx, LPCTSTR pszDefText, UINT x);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditVRuneSelPg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRuneSelDblClk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EditVRuneSel_H__76BC8909_4957_481D_B107_E112180DC71E__INCLUDED_)
