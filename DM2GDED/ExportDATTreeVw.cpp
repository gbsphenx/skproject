// ExportDATTreeVw.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DM2GDED.h"
#include "ExportDATTreeVw.h"
#include "ExportDATDoc.h"
#include "CompatDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

/////////////////////////////////////////////////////////////////////////////
// CExportDATTreeVw

IMPLEMENT_DYNCREATE(CExportDATTreeVw, CTreeView)

CExportDATTreeVw::CExportDATTreeVw()
{
}

CExportDATTreeVw::~CExportDATTreeVw()
{
}


BEGIN_MESSAGE_MAP(CExportDATTreeVw, CTreeView)
	//{{AFX_MSG_MAP(CExportDATTreeVw)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeyDown)
	ON_NOTIFY_REFLECT(NM_RETURN, OnReturn)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportDATTreeVw 描画

void CExportDATTreeVw::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: この位置に描画用のコードを追加してください
}

/////////////////////////////////////////////////////////////////////////////
// CExportDATTreeVw 診断

#ifdef _DEBUG
void CExportDATTreeVw::AssertValid() const
{
	CTreeView::AssertValid();
}

void CExportDATTreeVw::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CExportDATTreeVw メッセージ ハンドラ

void CExportDATTreeVw::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
	InitEdCtx(GetDocument()->GetEdCtx(FALSE));

	CTreeCtrl &wndTree = GetTreeCtrl();
	{
		int cy = wndTree.GetItemHeight();
		VERIFY(m_ilState.Create(cy, cy, ILC_COLOR|ILC_MASK, 0, 3));
		UINT i;
		for (i = 0; i < 3; i++) {
			CExtBM bm;
			CreateCheckImage(bm, cy, i);
			m_ilState.Add(&bm, RGB(255,255,255));
			bm.Detach();
		}

		wndTree.SetImageList(&m_ilState, TVSIL_NORMAL);
	}

	CreateTree();
}

BOOL CExportDATTreeVw::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= 0
		|TVS_DISABLEDRAGDROP
		|TVS_HASBUTTONS
		|TVS_HASLINES
		|TVS_LINESATROOT
		|TVS_SHOWSELALWAYS
		;
	
	return CTreeView::PreCreateWindow(cs);
}

void CExportDATTreeVw::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DWORD nPos = GetMessagePos();
	TVHITTESTINFO tvht;
	tvht.pt.x = GET_X_LPARAM(nPos);
	tvht.pt.y = GET_Y_LPARAM(nPos);
	GetDesktopWindow()->MapWindowPoints(CWnd::FromHandle(pNMHDR->hwndFrom), &tvht.pt, 1);
	GetTreeCtrl().HitTest(&tvht);
	if (tvht.flags & TVHT_ONITEMICON) {
		GetTreeCtrl().SelectItem(tvht.hItem);
		OnInverseCheckState(tvht.hItem);
	}
	
	*pResult = 0;
}

void CExportDATTreeVw::CreateCheckImage(CExtBM &bm, int cx, int nState)
{
	do {
		if (!bm.Create(CSize(cx, cx), 4, NULL, 0)) break;
		CCompatDC dcMem;
		VERIFY(dcMem.SaveDC() != 0);
		dcMem.SelectObject(&bm);
		//
		dcMem.SetBkMode(OPAQUE);
		dcMem.FillSolidRect(0, 0, cx, cx, RGB(255, 255, 255));
		if (nState == 2) dcMem.FillSolidRect(1, 1, cx-2, cx-2, RGB(192, 192, 192));
		CBrush brBlack;
		brBlack.CreateStockObject(BLACK_BRUSH);
		{ CRect rc(1, 1, cx-1, cx-1); dcMem.FrameRect(&rc, &brBlack); }
		switch (nState) {
		case 1:
		case 2:
			CFont fontMarlett;
			fontMarlett.CreatePointFont(100, "Marlett");
			dcMem.SelectObject(&fontMarlett);
			dcMem.SetBkMode(TRANSPARENT);
			dcMem.DrawText("a", CRect(1, 1, cx-1, cx-1), DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			break;
		}
		//
		VERIFY(dcMem.RestoreDC(-1));
		return;
	} while (false);
	ASSERT(FALSE);
}

CString CExportDATTreeVw::FormatClass1(BYTE v1)
{
	CString str;
	str.Format("%02X %s", 0U+v1, CGDEntry::GetFriendlyClass1Name(v1));
	return str;
}

CString CExportDATTreeVw::FormatClass2(BYTE v2, BYTE v1)
{
	CString str;
	str.Format("%02X %s", 0U+v2, CDTreeUsing::FormatClass2Name(CGDEntry(v1, v2, 0, 0, 0, 0)));
	return str;
}

void CExportDATTreeVw::OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	switch (pTVKeyDown->wVKey) {
	case ' ':
		if (!(GetKeyState(VK_MENU) & 0x8000)) {
			OnInverseCheckState(GetTreeCtrl().GetSelectedItem());
			*pResult = 1;
			return;
		}
		break;
	}
	
	*pResult = 0;
}

void CExportDATTreeVw::OnInverseCheckState(HTREEITEM hItem)
{
	CTreeCtrl &wndTree = GetTreeCtrl();
	int iImage, iSelImage;
	if (!wndTree.GetItemImage(hItem, iImage, iSelImage)) return;
	int iNewImage = (iImage == 0) ? 1 : 0;
	SetCheckState(hItem, iNewImage);
	OnAfterInverseCheckState(hItem);
}

void CExportDATTreeVw::OnChangeCheckState(HTREEITEM hItem)
{
	CTreeCtrl &wndTree = GetTreeCtrl();
	HTREEITEM hParent = wndTree.GetParentItem(hItem);
	if (!hParent) return;
	hItem = wndTree.GetChildItem(hParent);
	int nCnt = 0, nYes = 0;
	while (hItem) {
		nCnt++;
		if (GetCheckState(hItem) == 1) nYes++;
		hItem = wndTree.GetNextSiblingItem(hItem);
	}
	int nState = (nCnt == nYes) ? 1 : ((nYes != 0) ? 2 : 0);
	SetCheckState(hParent, nState);
}

void CExportDATTreeVw::OnAfterInverseCheckState(HTREEITEM hItem)
{
	int nState = GetCheckState(hItem);
	switch (nState) {
	case 0:
	case 1:
		{
			InheritCheckState(hItem);
			break;
		}
	}
}

void CExportDATTreeVw::InheritCheckState(HTREEITEM hItem)
{
	if (hItem == NULL) return;

	int nState = GetCheckState(hItem);
	hItem = GetTreeCtrl().GetChildItem(hItem);
	while (hItem) {
		int nHisState = GetCheckState(hItem);
		if (nHisState != nState) {
			SetCheckState(hItem, nState, false);
			InheritCheckState(hItem);
		}
		hItem = GetTreeCtrl().GetNextSiblingItem(hItem);
	}
}

void CExportDATTreeVw::OnReturn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnActivateItem(GetTreeCtrl().GetSelectedItem());
	
	*pResult = 0;
}

void CExportDATTreeVw::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnActivateItem(GetTreeCtrl().GetSelectedItem());
	
	*pResult = 0;
}

void CExportDATTreeVw::OnActivateItem(HTREEITEM hItem)
{
	DWORD nData = GetTreeCtrl().GetItemData(hItem);
	if (nData & 0x10000) {
		TSelExpTreeItem t;
		t.lower = CGDEntry(LOBYTE(nData), HIBYTE(nData), 0, 0, 0, 0);
		t.upper = CGDEntry(LOBYTE(nData), HIBYTE(nData),-1,-1,-1,-1);
		GetDocument()->UpdateAllViews(this, PM_V_SEL_EXPTREEITEM, reinterpret_cast<CObject *>(&t));
	}
}

void CExportDATTreeVw::CreateTree()
{
	CTreeCtrl &wndTree = GetTreeCtrl();

	wndTree.DeleteAllItems();

	typedef list<CExportDATDoc::NodeKey> NodeList;
	NodeList l;
	GetDocument()->Enum(l);
	NodeList::iterator
		iterPos = l.begin(),
		iterEnd = l.end();
	UINT c1 = -1, c2;
	HTREEITEM hL1 = NULL, hL2;
	for (; iterPos != iterEnd; iterPos++) {
		CExportDATDoc::NodeKey k = *iterPos;
		if (k.c1 != c1) {
			c1 = k.c1;

			hL1 = wndTree.InsertItem(FormatClass1(c1), 0, 0, TVI_ROOT, TVI_LAST);
			wndTree.SetItemData(hL1, 0x00000 | MAKEWORD(c1, 0));
		}

		{
			c2 = k.c2;

			hL2 = wndTree.InsertItem(FormatClass2(c2, c1), 0, 0, hL1, TVI_LAST);
			wndTree.SetItemData(hL2, 0x10000 | MAKEWORD(c1, c2));
		}

		//TRACE2("%02X-%02X\n", 0U+c1, 0U+c2);
	}
}

void CExportDATTreeVw::OnAfterSetCheckState(HTREEITEM hItem, bool fCheck)
{
	CTreeCtrl &wndTree = GetTreeCtrl();

	DWORD nVal = wndTree.GetItemData(hItem);
	if (nVal & 0x10000) {
		GetDocument()->SetCheck(CExportDATDoc::NodeKey(LOBYTE(nVal), HIBYTE(nVal)), fCheck);
	}
}
