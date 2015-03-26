#if !defined(AFX_DM2ORNATEPVW_H__3AC92914_879F_409A_BFAF_66D62066C2D1__INCLUDED_)
#define AFX_DM2ORNATEPVW_H__3AC92914_879F_409A_BFAF_66D62066C2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2OrnatePvw.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDM2OrnatePvw ウィンドウ

class CDM2OrnatePvw : public CStatic
{
	// 
	int m_iClass;
	// 
	int m_iOrnate;

public:
	// 
	void SetOrnate(int i, bool fWall)
	{
		m_iOrnate = i;
		m_iClass = fWall ? 0x09 : 0x0A;

		Invalidate();
	}
	// 
	void SetOrnate_wc(int i, int cls)
	{
		m_iOrnate = i;
		m_iClass = cls;

		Invalidate();
	}

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDM2OrnatePvw)
	//}}AFX_VIRTUAL

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDM2OrnatePvw)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2ORNATEPVW_H__3AC92914_879F_409A_BFAF_66D62066C2D1__INCLUDED_)
