#if !defined(AFX_DM2NI3PAGE_H__368761BB_6E29_454F_9505_43B377EE472F__INCLUDED_)
#define AFX_DM2NI3PAGE_H__368761BB_6E29_454F_9505_43B377EE472F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2NI3Page.h : ヘッダー ファイル
//

#include "DM2NIBPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2NI3Page ダイアログ

class CDM2NI3Page : public CDM2NIBPage
{
	// 
	void UpdateVw(bool fRecalc = false);
	// 
	DECLARE_DYNCREATE(CDM2NI3Page)

// コンストラクション
public:
	CDM2NI3Page();
	~CDM2NI3Page();

protected:
	// 
	virtual int GetStage() { return 3; }

// ダイアログ データ
	//{{AFX_DATA(CDM2NI3Page)
	enum { IDD = IDD_PP_NI3 };
	int		m_iKeep;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDM2NI3Page)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDM2NI3Page)
	//}}AFX_MSG
	afx_msg void OnRadioKeepPal(UINT nID);
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2NI3PAGE_H__368761BB_6E29_454F_9505_43B377EE472F__INCLUDED_)
