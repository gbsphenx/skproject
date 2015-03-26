#if !defined(AFX_EXPORTDATFRAME_H__3CD0C5F2_D37A_4553_B8D9_FD94FBE074EE__INCLUDED_)
#define AFX_EXPORTDATFRAME_H__3CD0C5F2_D37A_4553_B8D9_FD94FBE074EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportDATFrame.h : ヘッダー ファイル
//

#include "FlatSplitterWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CExportDATFrame フレーム

class CExportDATFrame : public CFrameWnd
{
	// 
	CFlatSplitterWnd m_wndSplitter;
	// 
	CToolBar m_wndToolBar;
	// 
	CFrameWnd *m_pMainWnd;
	// 
	CStatusBar m_wndStatusBar;

	// 
	DECLARE_DYNCREATE(CExportDATFrame)

public:
	// 
	CExportDATFrame();
	// 
	void StartModal(CWnd *pParentWnd);

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CExportDATFrame)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CExportDATFrame();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CExportDATFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EXPORTDATFRAME_H__3CD0C5F2_D37A_4553_B8D9_FD94FBE074EE__INCLUDED_)
