#if !defined(AFX_EDITDMTILEDLG_H__DAE35316_2398_4735_9CBD_C9899EC92DD7__INCLUDED_)
#define AFX_EDITDMTILEDLG_H__DAE35316_2398_4735_9CBD_C9899EC92DD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditDMTileDlg.h : ヘッダー ファイル
//

#include "DDAT.h"

/////////////////////////////////////////////////////////////////////////////
// CEditDMRootSel ダイアログ

class CEditDMRootSel : public CDialog
{
public:
	// 
	RN m_rnNext;
	// 
	void OnOK();

// コンストラクション
public:
	CEditDMRootSel(RN rnNext, CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CEditDMRootSel)
	enum { IDD = IDD_EDITROOTSEL };
	CComboBox	m_wndNextItem;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditDMRootSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditDMRootSel)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITDMTILEDLG_H__DAE35316_2398_4735_9CBD_C9899EC92DD7__INCLUDED_)
