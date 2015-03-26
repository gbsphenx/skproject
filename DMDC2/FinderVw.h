#if !defined(AFX_FINDERVW_H__4CB663DF_9FE3_492E_AF86_2E8EF589F2D5__INCLUDED_)
#define AFX_FINDERVW_H__4CB663DF_9FE3_492E_AF86_2E8EF589F2D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FinderVw.h : ヘッダー ファイル
//

#include "DMEdCtx.h"

/////////////////////////////////////////////////////////////////////////////
// CFinderVw ビュー

class CFinderVw : public CListView
{
public:
	// 
	CDMEdCtx *pEdCtx;
	CFont font;
	bool isDblClk;

	//
	CFinderVw();
	virtual ~CFinderVw();

	// 
	void SetDMEdCtx(CDMEdCtx *p)
	{
		if (pEdCtx != NULL)
			pEdCtx->Release();
		if (pEdCtx = p)
			pEdCtx->AddRef();
	}

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CFinderVw)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CFinderVw)
	afx_msg void OnFinderFind();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFinderOneclick();
	afx_msg void OnUpdateFinderOneclick(CCmdUI* pCmdUI);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FINDERVW_H__4CB663DF_9FE3_492E_AF86_2E8EF589F2D5__INCLUDED_)
