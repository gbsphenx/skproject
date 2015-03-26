#if !defined(AFX_DMTELELOCVW_H__5B0EA40B_4BE7_4FEF_8766_666917A720DC__INCLUDED_)
#define AFX_DMTELELOCVW_H__5B0EA40B_4BE7_4FEF_8766_666917A720DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMTeleLocVw.h : ヘッダー ファイル
//

#include "DDAT.h"

/////////////////////////////////////////////////////////////////////////////
// CDMTeleLocVw ビュー

class CDMTeleLocVw : public CScrollView
{
	// 
	struct TileSt {
		// 
		BYTE iTile;
		// 
		int iInv;

		// 
		TileSt()
		{
			iTile = -1;
			iInv = 0;
		}
	};

	// 
	CDDAT *m_pDDIf;
	// 
	AtlasPos_t m_ap;
	// 
	bool m_fMovable;
	// 
	bool m_fBuildTS;
	// 
	CSize m_size;
	// 
	TileSt m_ts[32][32];
	// 
	bool m_fNoSel;

	// 
	bool MkDDIf()
	{
		if (m_pDDIf != NULL)
			return true;
		return false;
	}
	// 
	void MarkBuildTS()
	{
		m_fBuildTS = true;
	}
	// 
	void BuildTS();
	// 
	bool Clicked(CPoint pt, bool fReally = true);

public:
	// 
	CDMTeleLocVw(CDDAT *pDDIf, AtlasPos_t ap, bool fMovable)
	{
		m_pDDIf = pDDIf;
		m_ap = ap;
		m_fMovable = fMovable;
		m_fBuildTS = false;
	}
	// 
	AtlasPos_t GetAP() const { return m_ap; }

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDMTeleLocVw)
	protected:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	virtual void OnInitialUpdate();     // 構築後、最初の処理。
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDMTeleLocVw)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DMTELELOCVW_H__5B0EA40B_4BE7_4FEF_8766_666917A720DC__INCLUDED_)
