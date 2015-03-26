#if !defined(AFX_EDPICREATUREPAGE_H__FD25C40D_AC7B_4AF1_9A45_B3C5BD3DC8A3__INCLUDED_)
#define AFX_EDPICREATUREPAGE_H__FD25C40D_AC7B_4AF1_9A45_B3C5BD3DC8A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPICreaturePage.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEdPICreaturePage ダイアログ

#include "EdPIDlg.h"

class CEdPICreaturePage : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPICreaturePage)

public:
	// 
	CEdPICreaturePage();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// ダイアログ データ
	//{{AFX_DATA(CEdPICreaturePage)
	enum { IDD = IDD_PI_CREATURE };
	CComboBox	m_wndSel;
	CComboBox	m_wndDir;
	int		m_iSel;
	int		m_iDir;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CEdPICreaturePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdPICreaturePage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDPICREATUREPAGE_H__FD25C40D_AC7B_4AF1_9A45_B3C5BD3DC8A3__INCLUDED_)
