#if !defined(AFX_EDPIPOTIONPAGE_H__4DB4A62E_2318_4920_8162_69C8060E4DA8__INCLUDED_)
#define AFX_EDPIPOTIONPAGE_H__4DB4A62E_2318_4920_8162_69C8060E4DA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIPotionPage.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEdPIPotionPage ダイアログ

#include "EdPIDlg.h"

class CEdPIPotionPage : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPIPotionPage)

public:
	// 
	CEdPIPotionPage();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// ダイアログ データ
	//{{AFX_DATA(CEdPIPotionPage)
	enum { IDD = IDD_PI_POTION };
	CComboBox	m_wndSel;
	CSliderCtrl	m_wndStr;
	int		m_iSel;
	int		m_iStr;
	BOOL	m_fF;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CEdPIPotionPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdPIPotionPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDPIPOTIONPAGE_H__4DB4A62E_2318_4920_8162_69C8060E4DA8__INCLUDED_)
