#if !defined(AFX_EDPIACTU1_H__CD4E5D81_AAC4_46F0_88EB_B6AD2F332320__INCLUDED_)
#define AFX_EDPIACTU1_H__CD4E5D81_AAC4_46F0_88EB_B6AD2F332320__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIActu1.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEdPIActu1Page ダイアログ

#include "EdPIDlg.h"

class CEdPIActu1Page : public CEdPIDlg
{
	// 
	int m_a1t;

	// 
	void Active1(UINT nID);

	// 
	void UpdateRecord(DMDBU rec);
	// 
	void UpdateState();
	// 
	void UpdateItemName();

	// 
	DECLARE_DYNCREATE(CEdPIActu1Page)

public:
	// 
	CEdPIActu1Page();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// ダイアログ データ
	//{{AFX_DATA(CEdPIActu1Page)
	enum { IDD = IDD_PI_ACTU1 };
	CComboBox	m_wndItemName;
	CComboBox	m_wndHero;
	CComboBox	m_wndCreature;
	CComboBox	m_wndMissile;
	CComboBox	m_wndSel;
	CSpinButtonCtrl	m_wndTimes;
	int		m_iCreature;
	int		m_iHero;
	int		m_iItemName;
	int		m_iItemType;
	int		m_iMissile;
	int		m_iSel;
	UINT	m_nTimes;
	UINT	m_nLo;
	UINT	m_nHi;
	CSpinButtonCtrl	m_wndLoArg;
	CSpinButtonCtrl	m_wndHiArg;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CEdPIActu1Page)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdPIActu1Page)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboItemType();
	afx_msg void OnSelchangeComboType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDPIACTU1_H__CD4E5D81_AAC4_46F0_88EB_B6AD2F332320__INCLUDED_)
