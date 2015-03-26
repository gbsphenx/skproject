// MainFrm.h : CMainFrame クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__F7E3EB4A_4FEB_4D79_AF7B_98994EAC3972__INCLUDED_)
#define AFX_MAINFRM_H__F7E3EB4A_4FEB_4D79_AF7B_98994EAC3972__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MkStatusBar.h"
#include "DMObBar.h"
#include "DMCIBar.h"
#include "DMLvSelWnd2.h"
#include "ShowcaseDock.h"
#include "FinderDock.h"

#include "MainFrmIndir.h"

class CMainFrame : public CFrameWnd, public CMainFrameInfluential, public CMainFrameDockSite
{
	// 
	UINT m_iEditLvTool;
	// 
	UINT m_iEditToolF;

	// 
	virtual UINT GetDMLv()
	{
		//return GetUIntFrom1(IDC_EDIT_DMLV);

		//return m_wndLvDlgBar.m_wndLvSel.GetItemPos();

		return m_wndMapSelBar.GetPos();
	}
	// 
	virtual void SetDMLv(UINT n)
	{
		//SetUIntTo1(IDC_EDIT_DMLV, n);

		//m_wndLvDlgBar.m_wndLvSel.SetItemPos(n);

		m_wndMapSelBar.SetPos(n);
	}
	// 
	virtual CSize GetLvSize()
	{
		return CSize(0, 0);
	}
	// 
	virtual void SetLvSize(CSize size)
	{

	}
	// 
	virtual UINT GetLvCnt()
	{
		//return GetUIntFrom1(IDC_EDIT_LVMAX, 0);

		//return m_wndLvDlgBar.m_wndLvSel.GetItemCount();

		return m_wndMapSelBar.GetCount();
	}
	// 
	virtual void SetLvCnt(UINT n)
	{
		//SetUIntTo1(IDC_EDIT_LVMAX, n);

		//CWnd *wnd;
		//if (wnd = m_wndLvDlgBar.GetDlgItem(IDC_SPIN_DMLV)) {
		//	wnd->SendMessage(UDM_SETRANGE, 0, MAKELONG(0, __max(0, (int)n - 1)));
		//	wnd->Invalidate();
		//}

		//m_wndLvDlgBar.m_wndLvSel.SetItemCount(n);

		m_wndMapSelBar.SetCount(n);
	}
	// 
	virtual UINT GetEditLvTool()
	{
		return m_iEditLvTool;
	}
	// 
	virtual void SetEditLvTool(UINT i)
	{
		m_iEditLvTool = i;
		m_wndEditLvToolBar.Invalidate();
	}
	// 
	virtual CWnd *GetDockedSite(UINT nID);
	// 
	virtual BOOL ShowDockedSite(UINT nID, BOOL fShow);
	// 
	virtual UINT GetVwLv()
	{
		return m_wndLvSelBar.GetPos();
	}
	// 
	virtual void SetVwLv(UINT n)
	{
		m_wndLvSelBar.SetPos(n);
	}
	// 
	virtual UINT GetEditToolF()
	{
		return m_iEditToolF;
	}

	// 
	void OnChangeEditLvMetr(int nMask);
	
protected: // シリアライズ機能のみから作成します。
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // コントロール バー用メンバ
	CMkStatusBar m_wndStatusBar;
	CToolBar m_wndToolBar;
	CReBar m_wndReBar;
	CDialogBar m_wndDlgBar;
//	CDMLvBar m_wndLvDlgBar;
	CToolBar m_wndEditLvToolBar;
	CDMObBar m_wndDMObBar;
//	CDMCIBar m_wndCIBar;
	CDMLvSelWnd2 m_wndLvSelBar;
	CDMLvSelWnd2 m_wndMapSelBar;
	CShowcaseDock m_wndShowcaseBar;
	CFinderDock m_wndFinderBar;

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChangeEditLvcx();
	afx_msg void OnChangeEditLvcy();
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	void CMainFrame::OnBtnCursor(UINT nID);
	void CMainFrame::OnUpdateBtnCursor(CCmdUI* pCmdUI);
	void OnDMLvSelBarSelChange();
	void OnDMLvSel2BarSelChange();
	void OnDMMapSel2BarSelChange();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAINFRM_H__F7E3EB4A_4FEB_4D79_AF7B_98994EAC3972__INCLUDED_)
