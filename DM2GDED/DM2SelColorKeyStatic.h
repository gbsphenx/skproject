#if !defined(AFX_DM2SELCOLORKEYSTATIC_H__D790FB4C_93B0_4B15_8D34_E1C1FDE353AB__INCLUDED_)
#define AFX_DM2SELCOLORKEYSTATIC_H__D790FB4C_93B0_4B15_8D34_E1C1FDE353AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2SelColorKeyStatic.h : ヘッダー ファイル
//

#include "DMGHLci.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2SelColorKeyStatic ウィンドウ

class CDM2SelColorKeyStatic : public CStatic
{
	// 
	enum {
		bkNo,
		bkSolid,
		bkLL,
		bkRR,
	};
	// 
	struct Solid {
		// 
		COLORREF clr;
		// 
		UINT f;

		// 
		Solid(): f(0) { }
	};
	// 
	struct Box {
		// 
		CRect rc;
		// 
		int bk;
		// 
		int iSolid;

		// 
		Box(): bk(0), iSolid(-1) { }
	};

	// 
	CArray<Solid, Solid> m_solid;
	// 
	CArray<Box, Box> m_box;
	// 
	int m_iColorSrcKey;
	// 
	int m_iColorKey;
	// 
	CFont m_fontMarlett;
	// 
	int m_iSel;
	// 
	int m_iOffset;
	// 
	int m_nSolidVis;

	// 
	bool Clicked(CPoint pt);

public:
	// 
	CDM2SelColorKeyStatic();
	// 
	void Set4(const DMGHLciPAL &a, int iColorSrcKey, const DMGHLciDM2Pal &DM2Pal);
	// 
	void Set8(int iColorSrcKey, const DMGHLciDM2Pal &DM2Pal);
	// 
	void SetSel(int i, bool fNotify);
	// 
	int GetSel() { return m_iSel; }
	// 
	void Init();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDM2SelColorKeyStatic)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDM2SelColorKeyStatic)
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2SELCOLORKEYSTATIC_H__D790FB4C_93B0_4B15_8D34_E1C1FDE353AB__INCLUDED_)
