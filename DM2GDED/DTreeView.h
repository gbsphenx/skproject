#if !defined(AFX_DTREEVIEW_H__885FC714_CA2D_4CC1_9D12_C7040C2AFA37__INCLUDED_)
#define AFX_DTREEVIEW_H__885FC714_CA2D_4CC1_9D12_C7040C2AFA37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DTreeView.h : ヘッダー ファイル
//

#include "GDEdCtx.h"
#include "DTreeUsing.h"
#include "ILSp.h"

/////////////////////////////////////////////////////////////////////////////
// CDTreeView ウィンドウ

class CDTreeView : public CTreeCtrl, protected CDTreeUsing
{
	// 
	struct Root {
		// 
		virtual ~Root() { }
	};
	// 
	struct Class1 : Root {
		// 
		CGDEntry lower, upper;

		// 
		Class1(UINT c1)
			: lower(c1, 0, 0, 0, 0, 0)
			, upper(c1,-1,-1,-1,-1,-1)
		{

		}
	};
	// 
	struct Class2 : Root {
		// 
		CGDEntry lower, upper;

		// 
		Class2(UINT c1, UINT c2)
			: lower(c1,c2, 0, 0, 0, 0)
			, upper(c1,c2,-1,-1,-1,-1)
		{

		}
	};

	// 
	CFont m_font;

public:
	// 
	CDTreeView();
	// 
	virtual ~CDTreeView();
	// 
	void InitEdCtx(CGDEdCtx *pEdCtx)
	{
		CDTreeUsing::InitEdCtx(pEdCtx);
	}
	// 
	BOOL SetItemData(HTREEITEM hItem, Root *pAny);
	// 
	Root *GetItemAsRoot(HTREEITEM hItem)
	{
		if (!hItem) return NULL;
		return reinterpret_cast<Root *>(GetItemData(hItem));
	}
	// 
	Class1 *GetItemAsClass1(HTREEITEM hItem)
	{
		return dynamic_cast<Class1 *>(GetItemAsRoot(hItem));
	}
	// 
	Class2 *GetItemAsClass2(HTREEITEM hItem)
	{
		return dynamic_cast<Class2 *>(GetItemAsRoot(hItem));
	}

protected:
	// 
	CGDEdOpt m_aOpt;
	// 
	list<CGDEntry> m_listAddPended;
	// 
	UINT m_nGDRSLT;
	// 
	HTREEITEM m_hItemRClk;

	// 
	virtual void OnGDUpdate(UINT nHint, void *pHint);
	// 
	virtual void OnReload();
	// 
	virtual void OnDblclkItem(HTREEITEM hItem);
	// 
	HTREEITEM FindClass1(const CGDEntry &pos, BOOL fAdd);
	// 
	HTREEITEM FindClass2(HTREEITEM hParent, const CGDEntry &pos, BOOL fAdd);
	// 
	void ShowContextMenu(CPoint point, HTREEITEM hItem);
	// 
	void OnSetOpt();
	// 
	void OnUpdateTreeItemName();
	// 
	void OnUpdateEntry(const CGDEntry &pos);
	// 
	HTREEITEM GetRClickItem();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDTreeView)
	//}}AFX_VIRTUAL

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDTreeView)
	afx_msg void OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPopupAddC0();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnReturn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnPopupReplicateC1();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	DECLARE_DYNCREATE(CDTreeView)
public:
	afx_msg void OnPopupExportThis();
	afx_msg void OnPopupDeleteThis();
	afx_msg void OnPopupImportFrom();
	afx_msg void OnPopupCopyThis();
	afx_msg void OnPopupPasteFrom();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DTREEVIEW_H__885FC714_CA2D_4CC1_9D12_C7040C2AFA37__INCLUDED_)
