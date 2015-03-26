#if !defined(AFX_DM2NIPVVIEW_H__E1E3BC13_C290_420A_A284_200896A00340__INCLUDED_)
#define AFX_DM2NIPVVIEW_H__E1E3BC13_C290_420A_A284_200896A00340__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2NIPvView.h : ヘッダー ファイル
//

#include "DM2NICtx.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2NIPvView ビュー

class CDM2NIPvView : public CScrollView
{
	// 
	CDM2NICtx *m_ctx;
	// 
	int m_iStage;
	// 
	CExtBM m_bm;
	// 
	CSize m_size;
	// 
	CPoint m_ptOrg;

	// 
	bool MkCtx() { return m_ctx != NULL; }
	// 
	void OnClickPixel(CPoint point);

	// 
	DECLARE_DYNCREATE(CDM2NIPvView)

public:
	// 
	CDM2NIPvView();
	// 
	bool Create(CDM2NICtx *pContext, CWnd *pParentWnd, DWORD nStyle, UINT nID)
	{
		m_ctx = pContext;
		if (CScrollView::Create(NULL, _T("Pv view"), nStyle, CRect(0, 0, 0, 0), pParentWnd, nID, NULL)) {
			return true;
		}
		return false;
	}
	// 
	void MakeStaged(int iStage);

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDM2NIPvView)
	protected:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	virtual void OnInitialUpdate();     // 構築後、最初の処理。
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDM2NIPvView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2NIPVVIEW_H__E1E3BC13_C290_420A_A284_200896A00340__INCLUDED_)
