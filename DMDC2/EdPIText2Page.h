#if !defined(AFX_EDPITEXT2PAGE_H__859118A0_6D51_44A6_B66E_2609132BD03A__INCLUDED_)
#define AFX_EDPITEXT2PAGE_H__859118A0_6D51_44A6_B66E_2609132BD03A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIText2Page.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEdPIText2Page ダイアログ

#include "EdPIDlg.h"
#include "DM2OrnatePvw.h"

class CEdPIText2Page : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPIText2Page)

// コンストラクション
public:
	// 
	CEdPIText2Page();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// ダイアログ データ
	//{{AFX_DATA(CEdPIText2Page)
	enum { IDD = IDD_PI_TEXT2 };
	CSpinButtonCtrl	m_wndSpinSP;
	CEdit	m_wndEditSP;
	CComboBox	m_wndUse;
	UINT	m_iSP;
	//}}AFX_DATA

protected:
	// 
	bool isComplex;
	int m_iUse;

	// 
	void enableWindows(const UINT *nIDs, bool fEnabled);

	void Validates();
	void UpdateUse();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CEdPIText2Page)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdPIText2Page)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboUse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDPITEXT2PAGE_H__859118A0_6D51_44A6_B66E_2609132BD03A__INCLUDED_)
