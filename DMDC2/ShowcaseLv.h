#if !defined(AFX_SHOWCASELV_H__5C463945_2EE0_49C6_9E63_95D57920AF17__INCLUDED_)
#define AFX_SHOWCASELV_H__5C463945_2EE0_49C6_9E63_95D57920AF17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowcaseLv.h : ヘッダー ファイル
//

#include "DMEdCtx.h"

/////////////////////////////////////////////////////////////////////////////
// CShowcaseLv ビュー

class CShowcaseLv : public CTreeView, public CDMEdCtxCB
{
	// 
	CDMEdCtx *pEdCtx;
	// 
	CImageList cPicList;
	// 
	BYTE cDir;
	// 
	bool fRebuildTree;

	// 
	virtual void OnDMUpdate(UINT nHint, void *pHint);

public:
	// 
	CShowcaseLv();
	// 
	virtual ~CShowcaseLv();

	// 
	void SetDMEdCtx(CDMEdCtx *p)
	{
		if (pEdCtx != NULL)
			pEdCtx->RemoveCB(this),
			pEdCtx->Release();
		if (pEdCtx = p)
			pEdCtx->AddRef(),
			pEdCtx->AddCB(this);
	}

	// 
	virtual void PostNcDestroy() { }

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CShowcaseLv)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CShowcaseLv)
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDirN(UINT nID);
	afx_msg void OnUpdateDirN(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SHOWCASELV_H__5C463945_2EE0_49C6_9E63_95D57920AF17__INCLUDED_)
