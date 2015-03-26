#if !defined(AFX_DM2GDEDSPLITTERWND_H__F1E1EB09_5D27_4557_BF42_8E20B5F4558B__INCLUDED_)
#define AFX_DM2GDEDSPLITTERWND_H__F1E1EB09_5D27_4557_BF42_8E20B5F4558B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2GDEDSplitterWnd.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDSplitterWnd 分割フレーム

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "FlatSplitterWnd.h"

class CDM2GDEDSplitterWnd : public CFlatSplitterWnd
{
	// 
	int m_iX, m_iY;

public:
	// 
	CDM2GDEDSplitterWnd();
	// 
	CWnd* GetActivePane(int* pRow = NULL, int* pCol = NULL);
	// 
	void SetActivePane(int row, int col, CWnd* pWnd = NULL);

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDM2GDEDSplitterWnd)
	protected:
	//}}AFX_VIRTUAL

// インプリメンテーション
public:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDM2GDEDSplitterWnd)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2GDEDSPLITTERWND_H__F1E1EB09_5D27_4557_BF42_8E20B5F4558B__INCLUDED_)
