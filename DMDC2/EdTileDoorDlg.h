#if !defined(AFX_EDTILEDOORDLG_H__6E67B895_3E4D_4365_9D32_7B3CCABA966F__INCLUDED_)
#define AFX_EDTILEDOORDLG_H__6E67B895_3E4D_4365_9D32_7B3CCABA966F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdTileDoorDlg.h : ヘッダー ファイル
//

#include "EdTileDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CEdTileDoorDlg ダイアログ

class CEdTileDoorDlg : public CEdTileDlg
{
// コンストラクション
public:
	CEdTileDoorDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CEdTileDoorDlg)
	enum { IDD = IDD_TILE_DOOR };
	BOOL	m_fSw1;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEdTileDoorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdTileDoorDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCheckSw1(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDTILEDOORDLG_H__6E67B895_3E4D_4365_9D32_7B3CCABA966F__INCLUDED_)
