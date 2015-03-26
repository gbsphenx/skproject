#if !defined(AFX_DM2NI1PAGE_H__7D28D688_7188_4F33_A101_9C5A53812D8E__INCLUDED_)
#define AFX_DM2NI1PAGE_H__7D28D688_7188_4F33_A101_9C5A53812D8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2NI1Page.h : ヘッダー ファイル
//

#include "DM2NIBPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2NI1Page ダイアログ

class CDM2NI1Page : public CDM2NIBPage
{
	// 
	DECLARE_DYNCREATE(CDM2NI1Page)

public:
	CDM2NI1Page();
	~CDM2NI1Page();

protected:
	// 
	virtual int GetStage() { return 1; }

// ダイアログ データ
	//{{AFX_DATA(CDM2NI1Page)
	enum { IDD = IDD_PP_NI1 };
	int		m_i4bpp;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDM2NI1Page)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDM2NI1Page)
	//}}AFX_MSG
	afx_msg void OnRadio4bpp(UINT nID);
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2NI1PAGE_H__7D28D688_7188_4F33_A101_9C5A53812D8E__INCLUDED_)
