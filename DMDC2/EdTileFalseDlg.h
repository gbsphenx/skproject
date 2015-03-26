#if !defined(AFX_EDTILEFALSEDLG_H__6C639953_2097_44F0_860C_D2C6D0AC74DF__INCLUDED_)
#define AFX_EDTILEFALSEDLG_H__6C639953_2097_44F0_860C_D2C6D0AC74DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdTileFalseDlg.h : ヘッダー ファイル
//

#include "EdTileDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CEdTileFalseDlg ダイアログ

class CEdTileFalseDlg : public CEdTileDlg
{
// コンストラクション
public:
	CEdTileFalseDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CEdTileFalseDlg)
	enum { IDD = IDD_TILE_FALSE };
	BOOL	m_fSw1;
	BOOL	m_fSw2;
	BOOL	m_fSw3;
	BOOL	m_fSw4;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEdTileFalseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdTileFalseDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCheckSw1(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDTILEFALSEDLG_H__6C639953_2097_44F0_860C_D2C6D0AC74DF__INCLUDED_)
