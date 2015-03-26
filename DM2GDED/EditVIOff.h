#if !defined(AFX_EDITVIOFF_H__9BFDBB8E_4B1D_4B78_88D6_F35EEE9E52CC__INCLUDED_)
#define AFX_EDITVIOFF_H__9BFDBB8E_4B1D_4B78_88D6_F35EEE9E52CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVIOff.h : ヘッダー ファイル
//

#include "EditVPg.h"
#include "EditVxxx.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVIOff ダイアログ

class CEditVIOff : public CEditVPg, public CWordValCmd_Hex, public CEditVxxx
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
	DECL_WVCT_SPEC(wvctImageOffset)

	// 
	CEditVIOff(CEditVLower &vctx, CWnd* pParent = NULL): CEditVPg(vctx, pParent) { }
	// 
	void Construction();

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }

	virtual ~CEditVIOff();

// ダイアログ データ
	//{{AFX_DATA(CEditVIOff)
	enum { IDD = IDD_EDITV_IOFF };
	CEdit	m_wndEditY;
	CEdit	m_wndEditX;
	CSpinButtonCtrl	m_wndSpinY;
	CSpinButtonCtrl	m_wndSpinX;
	int		m_offx;
	int		m_offy;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CEditVIOff)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditVIOff)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditPosX();
	afx_msg void OnChangeEditPosY();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITVIOFF_H__9BFDBB8E_4B1D_4B78_88D6_F35EEE9E52CC__INCLUDED_)
