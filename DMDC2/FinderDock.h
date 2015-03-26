#if !defined(AFX_FINDERDOCK_H__72AD5D3F_2017_402C_96B6_72EA0926C2B4__INCLUDED_)
#define AFX_FINDERDOCK_H__72AD5D3F_2017_402C_96B6_72EA0926C2B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FinderDock.h : ヘッダー ファイル
//

#include "FinderFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CFinderDock ウィンドウ

class CFinderDock : public CSizingControlBarCF
{
public:
	// 
	CFinderFrm m_frm;

	// 
	CFinderDock();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CFinderDock)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CFinderDock();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CFinderDock)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FINDERDOCK_H__72AD5D3F_2017_402C_96B6_72EA0926C2B4__INCLUDED_)
