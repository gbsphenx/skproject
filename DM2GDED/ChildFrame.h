#if !defined(AFX_CHILDFRAME_H__824EBE4A_D0E7_41FD_AED5_E102871A3E45__INCLUDED_)
#define AFX_CHILDFRAME_H__824EBE4A_D0E7_41FD_AED5_E102871A3E45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildFrame.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CChildFrame フレーム

class CChildFrame : public CMDIChildWnd
{
	// 
	DECLARE_DYNCREATE(CChildFrame)

protected:
	// 
	CChildFrame();           // 動的生成に使用されるプロテクト コンストラクタ。

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します

	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CChildFrame();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CHILDFRAME_H__824EBE4A_D0E7_41FD_AED5_E102871A3E45__INCLUDED_)
