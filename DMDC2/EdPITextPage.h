#if !defined(AFX_EDPITEXTPAGE_H__6E2E70CE_8113_4D94_AD9A_185CAE8AE526__INCLUDED_)
#define AFX_EDPITEXTPAGE_H__6E2E70CE_8113_4D94_AD9A_185CAE8AE526__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPITextPage.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEdPITextPage ダイアログ

#include "EdPIDlg.h"
#include "DM2OrnatePvw.h"
#include "ExtComboBox.h"

class CEdPITextPage : public CEdPIDlg
{
	// 
	int m_iSel;

	int m_trpt;

	// 
	DECLARE_DYNCREATE(CEdPITextPage)

public:
	// 
	CEdPITextPage();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

	void UpdateUseType();
	void UpdateLargeType();
	void UpdatePoint();

	void ApplyText();
	void ApplyUseType();
	void ApplySel();

	void ValidateLargeType();

	void enableWindows(const UINT *nIDs, bool fEnabled);

// ダイアログ データ
	//{{AFX_DATA(CEdPITextPage)
	enum { IDD = IDD_PI_TEXT };
	CComboBox	m_wndUse;
	CDM2OrnatePvw	m_wndOrnatePvw;
	CExtComboBox	m_wndSel;
	BOOL	m_f0;
	BOOL	m_f1;
	BOOL	m_fB;
	BOOL	m_fC;
	BOOL	m_fD;
	BOOL	m_fE;
	BOOL	m_fF;
	BOOL	m_f2;
	int		m_iMode;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CEdPITextPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdPITextPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboText();
	afx_msg void OnSelchangeComboUse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnCheckN15(UINT nID);
	afx_msg void OnRadioM1(UINT nID);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDPITEXTPAGE_H__6E2E70CE_8113_4D94_AD9A_185CAE8AE526__INCLUDED_)
