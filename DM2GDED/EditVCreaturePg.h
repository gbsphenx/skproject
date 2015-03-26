#if !defined(AFX_EditVCreaturePg_H__49e31057_4f4a_4751_8faa_cf57b453c3ee__INCLUDED_)
#define AFX_EditVCreaturePg_H__49e31057_4f4a_4751_8faa_cf57b453c3ee__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVCreaturePg.h : ヘッダー ファイル
//

#include "EditVPg.h"
#include "EditVxxx.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVCreaturePg ダイアログ

class CEditVCreaturePg : public CEditVPg, public CWordValCmd_Unsigned, public CEditVxxx
{
	// 
	UINT m_nFlags;
	// 
	CBitmap m_bmlast;
	// 
	UINT m_tick;

	// 
	virtual UINT GetVal()
	{
		return m_nFlags;
	}
	// 
	virtual LPCTSTR GetTemplateName() { return MAKEINTRESOURCE(IDD); }

	// 
	void Tick();

public:
	// 
	DECL_WVCT_SPEC(wvctCreature)

	// 
	CEditVCreaturePg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVPg(vctx, pParent) { }
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
	//{{AFX_DATA(CEditVCreaturePg)
	enum { IDD = IDD_EDITV_CREATURE };
	CComboBox m_wndCreatureSel;
	CStatic m_wndPortrait;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditVCreaturePg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditVCreaturePg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCbnSelchangeComboCreature();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EditVCreaturePg_H__49e31057_4f4a_4751_8faa_cf57b453c3ee__INCLUDED_)
