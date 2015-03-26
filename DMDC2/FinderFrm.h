#if !defined(AFX_FINDERFRM_H__57F31302_1A7A_4806_A41F_6BA3ACF1673C__INCLUDED_)
#define AFX_FINDERFRM_H__57F31302_1A7A_4806_A41F_6BA3ACF1673C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FinderFrm.h : ヘッダー ファイル
//

#include "FinderVw.h"

/////////////////////////////////////////////////////////////////////////////
// CFinderFrm フレーム

class CFinderFrm : public CFrameWnd
{
public:
	// 
	CFinderVw m_vw;
	CToolBar m_wndToolBar;

	// 
	CFinderFrm();
	virtual ~CFinderFrm();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CFinderFrm)
	protected:
	virtual void PostNcDestroy();
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CFinderFrm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FINDERFRM_H__57F31302_1A7A_4806_A41F_6BA3ACF1673C__INCLUDED_)
