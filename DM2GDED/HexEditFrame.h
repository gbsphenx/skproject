#if !defined(AFX_HEXEDITFRAME_H__E9A865E2_D6AC_4305_9393_0130C8CB8A9F__INCLUDED_)
#define AFX_HEXEDITFRAME_H__E9A865E2_D6AC_4305_9393_0130C8CB8A9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HexEditFrame.h : ヘッダー ファイル
//

#include "HexEditCB.h"

class CHexEditView;

/////////////////////////////////////////////////////////////////////////////
// CHexEditFrame フレーム

class CHexEditFrame : public CFrameWnd
{
	// 
	CHexEditView *m_pView;
	// 
	DECLARE_DYNAMIC(CHexEditFrame)

public:
	// 
	CHexEditFrame();
	// 
	virtual ~CHexEditFrame();
	// 
	CHexEditCB *GetHexEditCB();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CHexEditFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CHexEditFrame)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern void OnDeleteHexEditFrame(CHexEditFrame *pWnd);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_HEXEDITFRAME_H__E9A865E2_D6AC_4305_9393_0130C8CB8A9F__INCLUDED_)
