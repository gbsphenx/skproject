#if !defined(AFX_DMCIBAR_H__4ACE1777_B585_4A3A_BDE8_B63846A43B0F__INCLUDED_)
#define AFX_DMCIBAR_H__4ACE1777_B585_4A3A_BDE8_B63846A43B0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMCIBar.h : ヘッダー ファイル
//

#include "DMCITree.h"

/////////////////////////////////////////////////////////////////////////////
// CDMCIBar ウィンドウ

typedef CSizingControlBarG baseCDMCIBar;

class CDMCIBar : public baseCDMCIBar
{
	// 
	CDMCITreeView m_wndTree;

public:
	// 
	BOOL Create(CDMEdCtx *pEdCtx, CWnd *lpParent, LPCTSTR lpszWindowText, DWORD dwStyle, UINT nID)
	{
		m_szHorz = m_szVert = CSize(150, 150);
		m_szFloat = CSize(300, 400);
		if (!baseCDMCIBar::Create(lpszWindowText, lpParent, nID, dwStyle))
			return FALSE;
		if (!m_wndTree.Create(pEdCtx, WS_CHILD|WS_VISIBLE, this, AFX_IDW_PANE_FIRST))
			return FALSE;
		return TRUE;
	}

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDMCIBar)
	//}}AFX_VIRTUAL

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDMCIBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DMCIBAR_H__4ACE1777_B585_4A3A_BDE8_B63846A43B0F__INCLUDED_)
