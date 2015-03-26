#if !defined(AFX_EXPORTDATTREEVW_H__18C1AC16_01B1_4C92_98C2_65B104C097D9__INCLUDED_)
#define AFX_EXPORTDATTREEVW_H__18C1AC16_01B1_4C92_98C2_65B104C097D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportDATTreeVw.h : ヘッダー ファイル
//

#include "ExtBM.h"
#include "DTreeUsing.h"

/////////////////////////////////////////////////////////////////////////////
// CExportDATTreeVw ビュー

class CExportDATDoc;
class TSelExpTreeItem;

class CExportDATTreeVw : public CTreeView, protected CDTreeUsing
{
protected:
	// 
	CImageList m_ilState;

	// 
	CExportDATTreeVw();
	// 
	CString FormatClass1(BYTE c1);
	// 
	CString FormatClass2(BYTE c2, BYTE c1);
	// 
	void CreateCheckImage(CExtBM &bm, int cx, int nState);
	// 
	void OnInverseCheckState(HTREEITEM hItem);
	// 
	void OnChangeCheckState(HTREEITEM hItem);
	// 
	int GetCheckState(HTREEITEM hItem)
	{
		int x, y;
		if (GetTreeCtrl().GetItemImage(hItem, x, y)) {
			return x;
		}
		return -1;
	}
	// 
	BOOL SetCheckState(HTREEITEM hItem, int nState, bool fNotify = true)
	{
		if (GetTreeCtrl().SetItemImage(hItem, nState, nState)) {
			if (fNotify)
				OnChangeCheckState(hItem);
			OnAfterSetCheckState(hItem, nState == 1);
			return TRUE;
		}
		return FALSE;
	}
	// 
	void OnAfterInverseCheckState(HTREEITEM hItem);
	// 
	void InheritCheckState(HTREEITEM hItem);
	// 
	void OnActivateItem(HTREEITEM hItem);
	// 
	void CreateTree();
	// 
	void OnAfterSetCheckState(HTREEITEM hItem, bool fCheck);

	// 
	virtual void OnGDUpdate(UINT nHint, void *pHint) { }

	// 
	DECLARE_DYNCREATE(CExportDATTreeVw)

public:
	// 
	CExportDATDoc *GetDocument() { return (CExportDATDoc *)CView::GetDocument(); }

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CExportDATTreeVw)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CExportDATTreeVw();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CExportDATTreeVw)
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReturn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EXPORTDATTREEVW_H__18C1AC16_01B1_4C92_98C2_65B104C097D9__INCLUDED_)
