#if !defined(AFX_EXPORTDATLISTVW_H__82057182_8829_430A_910C_7F4806C022B0__INCLUDED_)
#define AFX_EXPORTDATLISTVW_H__82057182_8829_430A_910C_7F4806C022B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportDATListVw.h : ヘッダー ファイル
//

#include "DGridCtrl.h"

class CExportDATDoc;
class TSelExpTreeItem;

/////////////////////////////////////////////////////////////////////////////
// CExportDATListVw ビュー

class CExportDATListVw : public CView
{
protected:
	// 
	CDGridCtrl m_wndGrid;
	// 
	CGDEdCtx *m_pEdCtx;

	// 
	CExportDATListVw();
	// 
	void OnSelectItem(TSelExpTreeItem &rAny);
	// 
	void InitEdCtx(CGDEdCtx *pEdCtx);
	// 
	CGDAT &GetGDAT();

	// 
	DECLARE_DYNCREATE(CExportDATListVw)

public:
	// 
	CExportDATDoc *GetDocument();

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CExportDATListVw)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CExportDATListVw();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CExportDATListVw)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EXPORTDATLISTVW_H__82057182_8829_430A_910C_7F4806C022B0__INCLUDED_)
