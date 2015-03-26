#if !defined(AFX_DTREEDOCK_H__DD91369C_D688_48C8_BE7E_85CC791B6FC4__INCLUDED_)
#define AFX_DTREEDOCK_H__DD91369C_D688_48C8_BE7E_85CC791B6FC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DTreeDock.h : ヘッダー ファイル
//

#include "DTreeView.h"

/////////////////////////////////////////////////////////////////////////////
// CDTreeDock ウィンドウ

//typedef CSizingControlBarG baseDTreeDock;
typedef CWnd baseDTreeDock;

class CDTreeDock : public baseDTreeDock
{
	// 
	CDTreeView m_wndView;

	// 
	DECLARE_DYNCREATE(CDTreeDock);

public:
	// 
	CDTreeDock();
	// 
	BOOL Create(LPCTSTR pszWindowName, CWnd *pParentWnd, UINT nID, DWORD style)
	{
//		if (baseDTreeDock::Create(pszWindowName, pParentWnd, nID, style)) {
		if (CWnd::Create(NULL, pszWindowName, style, CRect(0, 0, 0, 0), pParentWnd, nID)) {
			return TRUE;
		}
		return FALSE;
	}
	// 
	void Init(CGDEdCtx *pEdCtx)
	{
		m_wndView.InitEdCtx(pEdCtx);
	}

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDTreeDock)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CDTreeDock();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDTreeDock)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DTREEDOCK_H__DD91369C_D688_48C8_BE7E_85CC791B6FC4__INCLUDED_)
