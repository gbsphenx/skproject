#if !defined(AFX_GETBACKBUTTON_H__FA05EB80_3E6F_4BFB_9684_6182601B82EB__INCLUDED_)
#define AFX_GETBACKBUTTON_H__FA05EB80_3E6F_4BFB_9684_6182601B82EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetBackButton.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CGetBackButton ウィンドウ

class CGetBackButton : public CWnd
{
	// 
	CBitmap m_ico;
	HCURSOR m_cursor;
	// 
	bool fDirIsUp;
	bool m_fInvert;

// コンストラクション
public:
	// 
	CGetBackButton();
	// 
	bool Create(CWnd *pParentWnd);
	// 
	void Show(CWnd *pTar);
	void Hide();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CGetBackButton)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CGetBackButton();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CGetBackButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_GETBACKBUTTON_H__FA05EB80_3E6F_4BFB_9684_6182601B82EB__INCLUDED_)
