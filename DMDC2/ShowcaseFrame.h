#if !defined(AFX_SHOWCASEFRAME_H__0BB3B7C9_C747_4A75_A027_629F3AE5012B__INCLUDED_)
#define AFX_SHOWCASEFRAME_H__0BB3B7C9_C747_4A75_A027_629F3AE5012B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowcaseFrame.h : ヘッダー ファイル
//

#include "ShowcaseLv.h"

/////////////////////////////////////////////////////////////////////////////
// CShowcaseFrame フレーム

class CShowcaseFrame : public CFrameWnd
{
public:
	// 
	CShowcaseLv m_vw;
	// 
	CToolBar m_wndToolBar;

	// 
	CShowcaseFrame();
	// 
	virtual ~CShowcaseFrame();

	// 
	virtual void PostNcDestroy() { }

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CShowcaseFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CShowcaseFrame)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SHOWCASEFRAME_H__0BB3B7C9_C747_4A75_A027_629F3AE5012B__INCLUDED_)
