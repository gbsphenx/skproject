#if !defined(AFX_DMLOCPALWND_H__1D833DC9_B9B2_4EE7_8B93_7E93D028B9E8__INCLUDED_)
#define AFX_DMLOCPALWND_H__1D833DC9_B9B2_4EE7_8B93_7E93D028B9E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMLocPalWnd.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDMLocPalWnd ウィンドウ

class CDMLocPalWnd : public CStatic
{
	// 
	BYTE m_bPalIdx[16];
	// 
	COLORREF m_clr[16];
	// 
	CRect m_rcItem[16];
	// 
	CRect m_rcDrawItem[16];
	// 
	BOOL m_fReadOnly;
	// 
	int m_iIdxSwap[2];

	// 
	void Resized();
	// 
	int HitTestItemRect(CPoint point);

public:
	// 
	CDMLocPalWnd();
	// 
	void SetItem(UINT iIdx, BYTE iPalIdx, COLORREF clr);
	// 
	void SetReadOnly(BOOL fReadOnly);
	// 
	BOOL GetItem(UINT iIdx, BYTE &iPalIdx, COLORREF &clr);
	// 
	BOOL IsReadOnly();
	// 
	int TrackItem(int iOldPos);
	// 
	void InitLoop();
	// 
	void CancelLoop();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDMLocPalWnd)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CDMLocPalWnd();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDMLocPalWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DMLOCPALWND_H__1D833DC9_B9B2_4EE7_8B93_7E93D028B9E8__INCLUDED_)
