#if !defined(AFX_EDITVFLAGSPG_H__41491803_EE05_453B_8103_CBCF03F7A23E__INCLUDED_)
#define AFX_EDITVFLAGSPG_H__41491803_EE05_453B_8103_CBCF03F7A23E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVFlagsPg.h : ヘッダー ファイル
//

#include "EditVPg.h"
#include "EditVxxx.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVFlagsPg ダイアログ

class CEditVFlagsPg : public CEditVPg
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
	CEditVFlagsPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVPg(vctx, pParent) { }
	// 
	void Construction();

// ダイアログ データ
	//{{AFX_DATA(CEditVFlagsPg)
	enum { IDD = IDD_EDITV_FLAGS };
	CListBox	m_wndList;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditVFlagsPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 
	CString m_strHintText[16];

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditVFlagsPg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CEditVBinaryFlagsPg : public CEditVFlagsPg, public CWordValCmd_Flags, public CEditVxxx
{
public:
	// 
	DECL_WVCT_SPEC(wvctFlags)

	// 
	CEditVBinaryFlagsPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVFlagsPg(vctx, pParent) { }

	// 
	virtual CString Format(WORD x);
	// 
	virtual CString FormatDisp(WORD x);
	// 
	virtual bool Parse(LPCSTR pszText, WORD &x);
	// 
	virtual LPCTSTR GetHintText(int i) { return m_strHintText[i & 15]; }

	// 
	void SetParm1(LPCTSTR psz);

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITVFLAGSPG_H__41491803_EE05_453B_8103_CBCF03F7A23E__INCLUDED_)
