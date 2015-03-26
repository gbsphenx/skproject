#if !defined(AFX_EDITVICONANIMPG_H__4DF55E0B_ED44_4E6F_B98E_85F2C6F5B659__INCLUDED_)
#define AFX_EDITVICONANIMPG_H__4DF55E0B_ED44_4E6F_B98E_85F2C6F5B659__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVIconAnimPg.h : ヘッダー ファイル
//

#include "EditVPg.h"
#include "EditVxxx.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVIconAnimPg

class CEditVIconAnimPg : public CEditVPg, public CWordValCmd_Hex, public CEditVxxx
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

	// 
	void SetSectionEnable(UINT nID, bool fEnabled);

public:
	// 
	DECL_WVCT_SPEC(wvctIconAnim)

	// 
	CEditVIconAnimPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVPg(vctx, pParent) { }
	// 
	void Construction();

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }

	//{{AFX_DATA(CEditVIconAnimPg)
	enum { IDD = IDD_EDITV_ICONANIM };
	BOOL	m_fAF4000;
	BOOL	m_fAF8000;
	int		m_nFrameCnt;
	int		m_iASF;
	int		m_nAdvMax;
	int		m_nAdv1;
	//}}AFX_DATA

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditVIconAnimPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CEditVIconAnimPg();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CEditVIconAnimPg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnCheckAny(UINT nID);
	afx_msg void OnRadioAsf0();
	afx_msg void OnCheckAf8000();
	afx_msg void OnCheckAf4000();
	afx_msg void OnSelchangeComboFramecnt();
	afx_msg void OnSelchangeComboMaxframecnt();
	afx_msg void OnSelchangeComboFpa();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITVICONANIMPG_H__4DF55E0B_ED44_4E6F_B98E_85F2C6F5B659__INCLUDED_)
