#if !defined(AFX_EDPISCROLLPAGE_H__80F1FE2E_DDBB_4B93_AFC5_D2150A075394__INCLUDED_)
#define AFX_EDPISCROLLPAGE_H__80F1FE2E_DDBB_4B93_AFC5_D2150A075394__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIScrollPage.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEdPIScrollPage ダイアログ

#include "EdPIDlg.h"
#include "ExtComboBox.h"

class CEdPIScrollPage : public CEdPIDlg
{
	// 
	UINT m_iLowSel;		// internal DUNGEON.dat
	UINT m_iSel;		// external GRAPHICS.dat
	bool m_fExttext;

	// 
	DECLARE_DYNCREATE(CEdPIScrollPage)

public:
	// 
	CEdPIScrollPage();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

	void UpdateUse();

// ダイアログ データ
	//{{AFX_DATA(CEdPIScrollPage)
	enum { IDD = IDD_PI_SCROLL };
	CExtComboBox	m_wndSel;
	int		m_iUse;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CEdPIScrollPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdPIScrollPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioM1();
	afx_msg void OnSelchangeComboText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDPISCROLLPAGE_H__80F1FE2E_DDBB_4B93_AFC5_D2150A075394__INCLUDED_)
