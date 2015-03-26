#if !defined(AFX_EDTILEPITDLG_H__B01326E0_248E_4A15_8D9F_F5382F8EFD16__INCLUDED_)
#define AFX_EDTILEPITDLG_H__B01326E0_248E_4A15_8D9F_F5382F8EFD16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdTilePitDlg.h : ヘッダー ファイル
//

#include "EdTileDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CEdTilePitDlg ダイアログ

class CEdTilePitDlg : public CEdTileDlg
{
// コンストラクション
public:
	CEdTilePitDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CEdTilePitDlg)
	enum { IDD = IDD_TILE_PIT };
	BOOL	m_fSw1;
	BOOL	m_fSw2;
	BOOL	m_fSw3;
	BOOL	m_fSw4;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEdTilePitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdTilePitDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCheckSw1(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDTILEPITDLG_H__B01326E0_248E_4A15_8D9F_F5382F8EFD16__INCLUDED_)
