#if !defined(AFX_EDPIACTU3PAGE_H__D038A9E7_0EBE_4BDC_B7C7_A05E851AAE63__INCLUDED_)
#define AFX_EDPIACTU3PAGE_H__D038A9E7_0EBE_4BDC_B7C7_A05E851AAE63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIActu3Page.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEdPIActu3Page ダイアログ

#include "EdPIDlg.h"

class CEdPIActu3Page : public CEdPIDlg
{
	// 
	int m_a1t, m_a3t;

	// 
	void Active1(UINT nID);

	// 
	DECLARE_DYNCREATE(CEdPIActu3Page)

public:
	// 
	CEdPIActu3Page();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// ダイアログ データ
	//{{AFX_DATA(CEdPIActu3Page)
	enum { IDD = IDD_PI_ACTU3 };
	CSliderCtrl	m_wndStr;
	CStatic	m_wndStaticMap;
	CEdit	m_wndMap;
	UINT	m_iMap;
	UINT	m_iX;
	UINT	m_iY;
	int		m_nStr;
	int		m_iDir;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CEdPIActu3Page)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdPIActu3Page)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRef();
	afx_msg void OnButtonSelf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDPIACTU3PAGE_H__D038A9E7_0EBE_4BDC_B7C7_A05E851AAE63__INCLUDED_)
