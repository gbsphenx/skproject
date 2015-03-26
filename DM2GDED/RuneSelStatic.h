#if !defined(AFX_RUNESELSTATIC_H__3EB2AEB7_DD97_42FA_A79D_A58565C2CEF7__INCLUDED_)
#define AFX_RUNESELSTATIC_H__3EB2AEB7_DD97_42FA_A79D_A58565C2CEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RuneSelStatic.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRuneSelStatic ウィンドウ

class CRuneSelStatic : public CStatic
{
	CBitmap m_bmRunes;
	CRect m_rcBox;
	int m_iSel;

// コンストラクション
public:
	CRuneSelStatic();

	void SetSel(int i) {
		m_iSel = i;

		if (m_hWnd != NULL && IsWindow(m_hWnd))
			Invalidate();
	}

	int GetSel() {
		return m_iSel;
	}

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRuneSelStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CRuneSelStatic();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CRuneSelStatic)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RUNESELSTATIC_H__3EB2AEB7_DD97_42FA_A79D_A58565C2CEF7__INCLUDED_)
