#if !defined(AFX_EDPIACTU2PAGE_H__4525E0D3_FDE3_406F_8BE3_0322B89869C6__INCLUDED_)
#define AFX_EDPIACTU2PAGE_H__4525E0D3_FDE3_406F_8BE3_0322B89869C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIActu2Page.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEdPIActu2Page ダイアログ

#include "EdPIDlg.h"
#include "DM2Ornate1ComboBox.h"

class CEdPIActu2Page : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPIActu2Page)

public:
	// 
	CEdPIActu2Page();

	// 
	virtual bool IsAcc(UINT nID);
	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// ダイアログ データ
	//{{AFX_DATA(CEdPIActu2Page)
	enum { IDD = IDD_PI_ACTU2 };
	CDM2Ornate1ComboBox	m_wndOrnate;
	CSpinButtonCtrl	m_wndDelay;
	BOOL	m_fBeep;
	BOOL	m_fInverse;
	BOOL	m_fSwIsOn;
	int		m_iOrnate;
	UINT	m_nDelay;
	int		m_iAT;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CEdPIActu2Page)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdPIActu2Page)
	afx_msg void OnSelchangeComboOrnate();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDPIACTU2PAGE_H__4525E0D3_FDE3_406F_8BE3_0322B89869C6__INCLUDED_)
