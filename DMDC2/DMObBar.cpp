// DMObBar.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Exp00069.h"
#include "DMObBar.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDMObBar

BEGIN_MESSAGE_MAP(CDMObBar, CSizingControlBarCF)
	//{{AFX_MSG_MAP(CDMObBar)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_DICTREE, OnViewDictree)
	ON_WM_INITMENUPOPUP()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMObBar メッセージ ハンドラ

void CDMObBar::OnSize(UINT nType, int cx, int cy) 
{
	HWND hWnd;
	if (IsWindow(hWnd = m_wndTree.m_hWnd)) {
		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST);
	}

	baseCDMObBar::OnSize(nType, cx, cy);
}

int CDMObBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (baseCDMObBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	LOGFONT lfNice;
	ReadFontTemplateFrom(lfNice, IDS_FONTTEMP_DMOBBAR, ";;;;;;;;;;;;;;1");
	m_fontNice.DeleteObject();
	if (!m_fontNice.CreateFontIndirect(&lfNice))
		return -1;
	if (0
		|| !m_wndToolBar.CreateEx(this, TBSTYLE_FLAT|TBSTYLE_LIST, WS_CHILD|WS_VISIBLE|CBRS_ALIGN_TOP, CRect(0, 0, 0, 0), AFX_IDW_TOOLBAR)
		|| !m_wndToolBar.LoadToolBar(IDR_OBJEXP)
	) {
		return -1;
	}
	m_wndToolBar.SetOwner(GetParentFrame());
	{
		m_wndToolBar.SetButtonText(0, "Browse All-");
		m_wndToolBar.SetButtonStyle(0, TBSTYLE_AUTOSIZE);
		m_wndToolBar.SetButtonStyle(1, TBSTYLE_AUTOSIZE);
		m_wndToolBar.SetButtonStyle(2, TBSTYLE_AUTOSIZE);
#if 0
		struct Tbl_t {
			LPCTSTR lpszText;
			UINT nID;
		} Tbls[] = {
			"Browse All-", ID_VIEW_DICTREE,
			"Back", ID_VIEW_BROWSE_BACK,
			"Forward", ID_VIEW_BROWSE_FORWARD,
			NULL,
		};
		CToolBarCtrl &r = m_wndToolBar.GetToolBarCtrl();
		while (r.DeleteButton(0));
		r.SetButtonWidth(16, 128);
		r.SetImageList(NULL);
		for (int i=0; Tbls[i].lpszText != NULL; i++) {
			CString tstr = CString(Tbls[i].lpszText) + ' ';
			tstr.SetAt(tstr.GetLength() - 1, 0);
			int iIdx = r.AddStrings(tstr);
			TBBUTTON tbb;
			tbb.iBitmap = i;
			tbb.idCommand = Tbls[i].nID;
			tbb.fsState = TBSTATE_ENABLED;
			tbb.fsStyle = TBSTYLE_AUTOSIZE;
			tbb.dwData = 0;
			tbb.iString = iIdx;
			r.AddButtons(1, &tbb);
		}
#endif
	}

	if (0
		|| !m_browseMenu.LoadMenu(IDR_OBJEXP)
	) {
		return -1;
	}

	return 0;
}

BOOL CDMObBar::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if (nCode == -1)
		ASSERT(TRUE);
	if (baseCDMObBar::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	if (m_wndToolBar.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	if (m_wndTree.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	return FALSE;
}

void CDMObBar::OnViewDictree() 
{
	CToolBarCtrl &r = m_wndToolBar.GetToolBarCtrl();
	CRect rc;
	r.GetItemRect(0, &rc);
	m_wndToolBar.ClientToScreen(&rc);
	CMenu *lpSub = m_browseMenu.GetSubMenu(0);
	if (lpSub) {
		lpSub->TrackPopupMenu(
			TPM_LEFTALIGN|TPM_LEFTBUTTON,
			rc.left,
			rc.bottom,
			this,
			NULL);
	}
}

void CDMObBar::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	baseCDMObBar::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	CMenu *pMenu = pPopupMenu;

	CCmdUI state;
	state.m_pMenu = pMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	state.m_nIndexMax = pMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
	  state.m_nIndex++)
	{
		state.m_nID = pMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // menu separator or invalid cmd - ignore it

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// possibly a popup menu, route to first item of that popup
			state.m_pSubMenu = pMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // first item of popup can't be routed to
			}
			state.DoUpdate(this, FALSE);    // popups are never auto disabled
		}
		else
		{
			// normal menu item
			// Auto enable/disable if frame window has 'm_bAutoMenuEnable'
			//    set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, state.m_nID < 0xF000);
		}

		// adjust for menu deletions and additions
		UINT nCount = pMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}

void CDMObBar::OnDestroy() 
{
	baseCDMObBar::OnDestroy();
	
}
