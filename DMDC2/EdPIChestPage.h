#if !defined(AFX_EDPICHESTPAGE_H__BB0AFADD_19D2_4B83_B1E0_EFE566ABC89D__INCLUDED_)
#define AFX_EDPICHESTPAGE_H__BB0AFADD_19D2_4B83_B1E0_EFE566ABC89D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIChestPage.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEdPIChestPage ダイアログ

#include "EdPIDlg.h"

class CEdPIChestPage : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPIChestPage)

public:
	// 
	CEdPIChestPage();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// ダイアログ データ
	//{{AFX_DATA(CEdPIChestPage)
	enum { IDD = IDD_PI_CHEST };
	CComboBox	m_wndSel;
	int		m_iSel;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CEdPIChestPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdPIChestPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDPICHESTPAGE_H__BB0AFADD_19D2_4B83_B1E0_EFE566ABC89D__INCLUDED_)
