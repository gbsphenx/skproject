#if !defined(AFX_DM2NIPVFRAME_H__3486CC2B_C1F9_40EF_8464_76AD6567B6EB__INCLUDED_)
#define AFX_DM2NIPVFRAME_H__3486CC2B_C1F9_40EF_8464_76AD6567B6EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2NIPvFrame.h : ヘッダー ファイル
//

#include "DM2NIPvView.h"
#include "DM2NICtx.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2NIPvFrame フレーム

class CDM2NIPvFrame : public CFrameWnd
{
public:
	// 
	CDM2NIPvView m_view;

	// 
	CDM2NIPvFrame();
	// 
	bool Create(CDM2NICtx *pContext, CWnd *pParentWnd, DWORD nStyle, CRect rc, UINT nID)
	{
		if (CFrameWnd::Create(NULL, _T("Pv frame"), nStyle, rc, pParentWnd, NULL, 0, NULL)) {
			if (m_view.Create(pContext, this, 0 |WS_VISIBLE |WS_CHILD, AFX_IDW_PANE_FIRST)) {
				InitialUpdateFrame(NULL, TRUE);
				RecalcLayout();
				return true;
			}
		}
		return false;
	}

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDM2NIPvFrame)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDM2NIPvFrame)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DM2NIPVFRAME_H__3486CC2B_C1F9_40EF_8464_76AD6567B6EB__INCLUDED_)
