// DMCITree.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "DMCITree.h"
#include "Exp00069Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDMCITreeView

CDMCITreeView::CDMCITreeView()
{
	m_pEdCtx = NULL;
}

CDMCITreeView::~CDMCITreeView()
{
	ASSERT(m_pEdCtx == NULL);
}

BEGIN_MESSAGE_MAP(CDMCITreeView, CTreeCtrl)
	//{{AFX_MSG_MAP(CDMCITreeView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnDeleteItem)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMCITreeView メッセージ ハンドラ

void CDMCITreeView::PostNcDestroy() 
{
	Init(NULL);
	
	CTreeCtrl::PostNcDestroy();
}

BOOL CDMCITreeView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_SHOWSELALWAYS;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	
	return CTreeCtrl::PreCreateWindow(cs);
}

int CDMCITreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	LOGFONT lfNice;
	ReadFontTemplateFrom(lfNice, IDS_FONTTEMP_DMCITREEVIEW, "Courier;10;;;;;;;;;;;;1");
	m_fontNice.DeleteObject();
	m_fontNice.CreateFontIndirect(&lfNice);

	SetBkColor(RGB(0, 0, 0));
	SetTextColor(RGB(255, 255, 255));
	SetFont(&m_fontNice);

	m_hItemChkInt = NULL;
	
	return 0;
}

HTREEITEM CDMCITreeView::AddRoot_CheckIntegrity()
{
	if (m_hItemChkInt) DeleteItem(m_hItemChkInt), m_hItemChkInt = NULL;

	HTREEITEM hItem = InsertItem("INTEGRITY CHECKER REPORT");
	SetItemData(hItem, new RootChkInt());
	return m_hItemChkInt = hItem;
}

HTREEITEM CDMCITreeView::AddSite_CheckInt(HTREEITEM hParent, AtlasPos_t pos, LPCTSTR pszMessage)
{
	CString strMes; strMes.Format("MAP:%3u(%2u,%2u) %s"
		, 0U + pos.Z()
		, 0U + pos.X()
		, 0U + pos.Y()
		, pszMessage
		);
	HTREEITEM hItem = InsertItem(strMes, hParent);
	SiteAtlasPos *p = new SiteAtlasPos();
	p->pos = pos;
	SetItemData(hItem, p);
	return hItem;
}

HTREEITEM CDMCITreeView::AddSite_Memo(HTREEITEM hParent, LPCTSTR pszMessage)
{
	HTREEITEM hItem = InsertItem(pszMessage, hParent);
	SetItemData(hItem, new Site());
	return hItem;
}

void CDMCITreeView::OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	Any *pItem = reinterpret_cast<Any *>(pNMTreeView->itemOld.lParam);
	if (pItem) {
		delete pItem;
	}
	
	*pResult = 0;
}

BOOL CDMCITreeView::SetItemData(HTREEITEM hItem, Any *pItem)
{
	return CTreeCtrl::SetItemData(hItem, (DWORD)pItem);
}

void CDMCITreeView::OnDMUpdate(UINT nHint, void *pHint)
{
	switch (nHint) {
	case DPM_INITVIEW:
		{
			DeleteAllItems();
			break;
		}
	}
}

void CDMCITreeView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hItem = GetNextItem(NULL, TVGN_CARET);
	if (hItem) {
		Any *pAny = reinterpret_cast<Any *>(GetItemData(hItem));
		if (pAny) {
			{
				SiteAtlasPos *p = dynamic_cast<SiteAtlasPos *>(pAny);
				if (p) {
					SelectLvXY_t what;
					what.pos = p->pos;
					m_pEdCtx->DispatchDMUpdate(DPM_SELECTBTLVXY, &what);
					m_pEdCtx->DispatchDMUpdate(DPM_SELECTLVXY, &what);
				}
			}
		}
	}
	
	*pResult = 0;
}

BOOL CDMCITreeView::PickItUp(HTREEITEM hItem)
{
	Expand(hItem, TVE_EXPAND);
	Select(hItem, TVGN_CARET);
	Select(hItem, TVGN_FIRSTVISIBLE);
	return TRUE;
}
