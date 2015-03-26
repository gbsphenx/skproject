#if !defined(AFX_EDPIWEAPON_H__32E08F6C_9659_4B6B_ACF0_4C981B978CF1__INCLUDED_)
#define AFX_EDPIWEAPON_H__32E08F6C_9659_4B6B_ACF0_4C981B978CF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIWeapon.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEdPIWeaponPage ダイアログ

#include "EdPIDlg.h"

class CEdPIWeaponPage : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPIWeaponPage)

public:
	// 
	CEdPIWeaponPage();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// ダイアログ データ
	//{{AFX_DATA(CEdPIWeaponPage)
	enum { IDD = IDD_PI_WEAPON };
	CComboBox	m_wndSel;
	BOOL	m_fE;
	BOOL	m_fF;
	BOOL	m_f8;
	BOOL	m_f9;
	int		m_nCharges;
	int		m_iSel;
	BOOL	m_f7;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CEdPIWeaponPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdPIWeaponPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDPIWEAPON_H__32E08F6C_9659_4B6B_ACF0_4C981B978CF1__INCLUDED_)
