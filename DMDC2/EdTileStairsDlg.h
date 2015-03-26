#if !defined(AFX_EDTILESTAIRSDLG_H__82FC7305_9A5A_4F04_A29E_2A2849EF723E__INCLUDED_)
#define AFX_EDTILESTAIRSDLG_H__82FC7305_9A5A_4F04_A29E_2A2849EF723E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdTileStairsDlg.h : ヘッダー ファイル
//

#include "EdTileDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CEdTileStairsDlg ダイアログ

class CEdTileStairsDlg : public CEdTileDlg
{
// コンストラクション
public:
	CEdTileStairsDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CEdTileStairsDlg)
	enum { IDD = IDD_TILE_STAIRS };
	BOOL	m_fSw1;
	BOOL	m_fSw2;
	BOOL	m_fSw3;
	BOOL	m_fSw4;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEdTileStairsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdTileStairsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCheckSw1(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDTILESTAIRSDLG_H__82FC7305_9A5A_4F04_A29E_2A2849EF723E__INCLUDED_)
