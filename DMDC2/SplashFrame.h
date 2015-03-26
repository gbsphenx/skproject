#if !defined(AFX_SPLASHFRAME_H__2885231F_FB90_428B_AF58_9E950F755AB8__INCLUDED_)
#define AFX_SPLASHFRAME_H__2885231F_FB90_428B_AF58_9E950F755AB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplashFrame.h : ヘッダー ファイル
//

#include "SplashVw.h"
#include "HintOfStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CSplashFrame フレーム

class CSplashDialogBar : public CDialogBar
{
public:
	// 
	BOOL OnEraseBkgnd(CDC* pDC)
	{
		CRect rc;
		GetClientRect(&rc);
		pDC->FillSolidRect(&rc, RGB(0, 0, 0));

		return true;
	}
	// 
	HBRUSH OnCtlColor(
		CDC* pDC,
		CWnd* pWnd,
		UINT nCtlColor 
	)
	{
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255,255,206));
		return (HBRUSH)GetStockObject(BLACK_BRUSH);
	}

	// 
	DECLARE_MESSAGE_MAP()
};

class CSplashFrame : public CFrameWnd
{
public:
	// 
	CSplashVw m_vw;
	// 
	CSplashDialogBar m_bar;
	// 
	CHintOfStatic m_horz;

	//
	CSplashFrame();
	// 
	virtual ~CSplashFrame();
	// 
	void PostNcDestroy() { }


// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSplashFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSplashFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SPLASHFRAME_H__2885231F_FB90_428B_AF58_9E950F755AB8__INCLUDED_)
