#if !defined(AFX_EDITVWVPG_H__2E7A460E_08BB_4454_9F08_F13FEEC9C35E__INCLUDED_)
#define AFX_EDITVWVPG_H__2E7A460E_08BB_4454_9F08_F13FEEC9C35E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVWVPg.h : ヘッダー ファイル
//

#include "EditVPg.h"
#include "EditVxxx.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVWordValPg ダイアログ

class CEditVWordValPg : public CEditVPg
{
	// 
	DECL_WVCT_PRE()

	// 
	int m_wvct;
	// 
	int m_nVal;
	// 
	int m_sel;

	// 
	virtual CEditVxxx &GetVxxx() = NULL;
	// 
	CWordValCmd &GetCur();

public:
	// 
	CEditVWordValPg(CEditVLower &vctx, int sel, CWnd* pParent = NULL): CEditVPg(vctx, pParent), m_sel(sel) { }
	// 
	virtual void Construction();
	// 
	virtual UINT GetVal()
	{
		return (WORD)m_nVal;
	}
	// 
	virtual LPCTSTR GetTemplateName() { return MAKEINTRESOURCE(IDD); }

// ダイアログ データ
	//{{AFX_DATA(CEditVWordValPg)
	enum { IDD = IDD_EDITV_WV };
	int		m_iSel;
	CString	m_strVal;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditVWordValPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditVWordValPg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRadioD10();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CEditVWordValSignedPg : public CEditVWordValPg, public CWordValCmd_Signed, public CEditVxxx
{
public:
	// 
	DECL_WVCT_SPEC(wvctSigned)

	// 
	CEditVWordValSignedPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVWordValPg(vctx, 0, pParent) { }

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }
	// 
	virtual CEditVxxx &GetVxxx() { return *this; }
};

class CEditVWordValUnsignedPg : public CEditVWordValPg, public CWordValCmd_Unsigned, public CEditVxxx
{
public:
	// 
	DECL_WVCT_SPEC(wvctUnsigned)

	// 
	CEditVWordValUnsignedPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVWordValPg(vctx, 1, pParent) { }

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }
	// 
	virtual CEditVxxx &GetVxxx() { return *this; }
};

class CEditVWordValHexPg : public CEditVWordValPg, public CWordValCmd_Hex, public CEditVxxx
{
public:
	// 
	DECL_WVCT_SPEC(wvctHex)

	// 
	CEditVWordValHexPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVWordValPg(vctx, 2, pParent) { }

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }
	// 
	virtual CEditVxxx &GetVxxx() { return *this; }
};

class CEditVWordValBinaryPg : public CEditVWordValPg, public CWordValCmd_Flags, public CEditVxxx
{
public:
	// 
	DECL_WVCT_SPEC(wvctBinary)

	// 
	CEditVWordValBinaryPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVWordValPg(vctx, 3, pParent) { }

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }
	// 
	virtual CEditVxxx &GetVxxx() { return *this; }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITVWVPG_H__2E7A460E_08BB_4454_9F08_F13FEEC9C35E__INCLUDED_)
