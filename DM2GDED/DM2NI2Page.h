#if !defined(AFX_DM2NI2PAGE_H__16C61F09_65C5_495D_B2A7_972869AB9963__INCLUDED_)
#define AFX_DM2NI2PAGE_H__16C61F09_65C5_495D_B2A7_972869AB9963__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2NI2Page.h : ヘッダー ファイル
//

#include "DM2NIBPage.h"
#include "DM2SelColorKeyCB.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2NI2Page ダイアログ

class CDM2NI2Page : public CDM2NIBPage
{
	// 
	bool m_fUse1, m_fUse2;

	// 
	void UpdateVw(bool fRecalc = false);

	// 
	DECLARE_DYNCREATE(CDM2NI2Page)

// コンストラクション
public:
	CDM2NI2Page();
	~CDM2NI2Page();

protected:
	// 
	virtual int GetStage() { return 2; }

// ダイアログ データ
	//{{AFX_DATA(CDM2NI2Page)
	enum { IDD = IDD_PP_NI2 };
	CDM2SelColorKeyCB	m_wndPal2;
	CDM2SelColorKeyCB	m_wndPal1;
	CButton	m_wndCK2;
	CButton	m_wndCK1;
	int		m_iColorKey1;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDM2NI2Page)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDM2NI2Page)
	afx_msg void OnStaticPal1();
	afx_msg void OnStaticPal2();
	//}}AFX_MSG
	afx_msg void OnStaticPal();
	LRESULT OnClickPixel(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRadioColorKey1(UINT nID);
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2NI2PAGE_H__16C61F09_65C5_495D_B2A7_972869AB9963__INCLUDED_)
