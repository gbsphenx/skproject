// DMObTreeView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Exp00069.h"
#include "Exp00069Doc.h"
#include "DMObTreeView.h"
#include "MainFrm.h"
#include "EditDBItemDlg.h"
#include <algorithm>
#include "EditDMRootSel.h"
#include "DMObTreeData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDMObTreeView::DBITEMINFO CDMObTreeView::m_aDBII[] = {
	ID_POPUP_ALL_DB0, 0x0,1,"All DOOR",
	ID_POPUP_ALL_DB1, 0x1,1,"All TELE",
	ID_POPUP_ALL_DB2, 0x2,1,"All TEXT",
	ID_POPUP_ALL_DB3, 0x3,1,"All ACTU1",
	ID_POPUP_ALL_DB4, 0x4,1,"All MONS",
	ID_POPUP_ALL_DB5, 0x5,1,"All WEAP",
	ID_POPUP_ALL_DB6, 0x6,1,"All CLOT",
	ID_POPUP_ALL_DB7, 0x7,1,"All SCRO",
	ID_POPUP_ALL_DB8, 0x8,1,"All POTI",
	ID_POPUP_ALL_DB9, 0x9,1,"All CHES",
	ID_POPUP_ALL_DB10,0xa,1,"All MISC",
	ID_POPUP_ALL_DB11,0xb,1,"All ACTU2",
	ID_POPUP_ALL_DB12,0xc,1,"All ACTU3",
	ID_POPUP_ALL_DB13,0xd,1,"All ACTU4",
	ID_POPUP_ALL_DB14,0xe,1,"All PROJ",
	ID_POPUP_ALL_DB15,0xf,0,"---",
	0,0,0,NULL,
};

namespace
{

BOOL SetClipboardTextData(LPCSTR pszText)
{
	int n = strlen(pszText);
	LPSTR psz = (LPSTR)GlobalAlloc(GMEM_FIXED, n+1);
	if (psz != NULL) {
		strcpy(psz, pszText);
		SetClipboardData(CF_TEXT, (HANDLE)psz);
		return TRUE;
	}
	return FALSE;
}

BOOL GetClipboardTextData(CString &strText)
{
	HGLOBAL hMem = (HGLOBAL)GetClipboardData(CF_TEXT);
	LPVOID pvMem;
	if (pvMem = GlobalLock(hMem)) {
		DWORD cbMem = GlobalSize(hMem);
		strText = CString((LPCSTR)pvMem, cbMem);
		GlobalUnlock(hMem);
		return true;
	}
	return false;
}

LPCTSTR MakeNameFromDb(UINT iDB)
{
	LPCTSTR lpszName = "----";
	switch (iDB) {
	case 0: lpszName = "DOOR"; break;
	case 1: lpszName = "TELE"; break;
	case 2: lpszName = "TEXT"; break;
	case 3: lpszName = "ACTU1"; break;
	case 4: lpszName = "MONS"; break;
	case 5: lpszName = "WEAP"; break;
	case 6: lpszName = "CLOT"; break;
	case 7: lpszName = "SCRO"; break;
	case 8: lpszName = "POTI"; break;
	case 9: lpszName = "CHES"; break;
	case 10:lpszName = "MISC"; break;
	case 11:  lpszName = "ACTU2"; break;
	case 12:  lpszName = "ACTU3"; break;
	case 13:  lpszName = "ACTU4"; break;
	case 14:lpszName = "PROJ"; break;
	case 15:lpszName = "CLOU"; break;
	}
	return lpszName;
}

void FormatTileText(CString &tstrText, const AtlasPos_t &pos)
{
	tstrText.Format("Tile - Lv:%3u (%2u,%2u)", (UINT)pos.Z(), (UINT)pos.Y(), (UINT)pos.X());
}

void FormatBTDbText(CString &tstrText, const DBIPos_t &pos)
{
	tstrText.Format(
		"%s: #%04u",
		MakeNameFromDb(pos.db()), (UINT)pos.idx());
}

};

BOOL MakeHint(RN k, DBIPos_t pos, const DMDBU *pRec, CString &tstr, RN &kSub)
{
	kSub = RN_NIL;
	CString tstrDir, tstrDb, tstrHint;
	CString tstrIdx;
	tstrIdx.Format("#%04u ", (UINT)pos.idx());
	switch (k.r.w) {
	case 0xff80:
		tstrHint.Format("%04X", (UINT)k.r.w);
		tstr = tstrIdx + tstrHint;
		return TRUE;
	}
	if (k == RN_NIL) {
		tstrDir = "- ";
	} else if (k == RN_END) {
		tstrDir = "! ";
	} else {
		switch (k.room()) {
		case 0: tstrDir = "N "; break;
		case 1: tstrDir = "E "; break;
		case 2: tstrDir = "S "; break;
		case 3: tstrDir = "W "; break;
		}
	}
	tstrDb = MakeNameFromDb(pos.db());
	tstrDb+= ":";
	if (!pRec) {
		tstrHint = "!BAD INDEX!";
	} else {
		switch (pos.db()) {
		case 0:
			tstrHint.Format(
				"%04X"
				, (UINT)pRec->r0.w2
				);
			break;
		case 1:
			tstrHint.Format(
				"%04X %04lX"
				, (UINT)pRec->r1.w2
				, (UINT)pRec->r1.w4
				);
			break;
		case 2:
			tstrHint.Format(
				"%04X"
				, (UINT)pRec->r2.w2
				);
			break;
		case 3: case 11: case 12: case 13:
			tstrHint.Format(
				"%04X %04X %04X"
				, (UINT)pRec->r3.w2
				, (UINT)pRec->r3.w4
				, (UINT)pRec->r3.w6
				);
			break;
		case 4:
			tstrHint.Format(
				"%02X %02X {%04X %04X %04X %04X} %04X"
				, (UINT)pRec->r4.b4
				, (UINT)pRec->r4.b5
				, (UINT)pRec->r4.w6[0]
				, (UINT)pRec->r4.w6[1]
				, (UINT)pRec->r4.w6[2]
				, (UINT)pRec->r4.w6[3]
				, (UINT)pRec->r4.w14
				);
			kSub = pRec->r4.stolen;
			break;
		case 5:
			tstrHint.Format(
				"%04X"
				, (UINT)pRec->r5.w2
				);
			break;
		case 6:
			tstrHint.Format(
				"%04X"
				, (UINT)pRec->r6.w2
				);
			break;
		case 7:
			tstrHint.Format(
				"%04X"
				, (UINT)pRec->r7.w2
				);
			break;
		case 8:
			tstrHint.Format(
				"%04X"
				, (UINT)pRec->r8.w2
				);
			break;
		case 9:
			tstrHint.Format(
				"%04X {%02X %02X}"
				, (UINT)pRec->r9.w4
				, (UINT)pRec->r9.b6[0]
				, (UINT)pRec->r9.b6[1]
				);
			kSub = pRec->r9.stolen;
			break;
		case 10:
			tstrHint.Format(
				"%04X"
				, (UINT)pRec->r10.w2
				);
			break;
		case 14:
			tstrHint.Format(
				"{%02X %02X} %04X"
				, (UINT)pRec->r14.b4[0]
				, (UINT)pRec->r14.b4[1]
				, (UINT)pRec->r14.w6
				);
			kSub = pRec->r14.stolen;
			break;
		case 15:
			tstrHint.Format(
				"%04X"
				, (UINT)pRec->r15.w2
				);
			break;
		default:
			tstrHint.Empty();
			break;
		}
	}
	tstr = tstrIdx + tstrDir + tstrDb + tstrHint;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDMObTreeView

IMPLEMENT_DYNAMIC(CDMObTreeView, CTreeCtrl)

CDMObTreeView::CDMObTreeView()
{
	m_pEdCtx = NULL;
}

CDMObTreeView::~CDMObTreeView()
{
	ASSERT(m_pEdCtx == NULL);
}

BEGIN_MESSAGE_MAP(CDMObTreeView, CTreeCtrl)
	//{{AFX_MSG_MAP(CDMObTreeView)
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnDeleteItem)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_NOTIFY_REFLECT(TVN_SETDISPINFO, OnSetDispInfo)
	ON_COMMAND(ID_POPUP_INSERT_ITEM_FIRST, OnPopupInsertItemFirst)
	ON_COMMAND(ID_POPUP_INSERT_ITEM_LAST, OnPopupInsertItemLast)
	ON_COMMAND(ID_POPUP_EDIT_ITEM_HERE, OnPopupEditItemHere)
	ON_COMMAND(ID_POPUP_INSERT_ITEM_BEFORE, OnPopupInsertItemBefore)
	ON_COMMAND(ID_POPUP_INSERT_ITEM_AFTER, OnPopupInsertItemAfter)
	ON_COMMAND(ID_POPUP_DELETE_ITEM_HERE, OnPopupDeleteItemHere)
	ON_COMMAND(ID_POPUP_INSERT_SUBITEM_FIRST, OnPopupInsertSubitemAfter)
	ON_COMMAND(ID_POPUP_INSERT_SUBITEM_LAST, OnPopupInsertSubitemBefore)
	ON_COMMAND(ID_POPUP_INSERT_ITEM_AFTER_APPEND, OnPopupInsertItemAfterAppend)
	ON_COMMAND(ID_POPUP_INSERT_ITEM_BEFORE_APPEND, OnPopupInsertItemBeforeAppend)
	ON_COMMAND(ID_POPUP_INSERT_ITEM_FIRST_APPEND, OnPopupInsertItemFirstAppend)
	ON_COMMAND(ID_POPUP_INSERT_ITEM_LAST_APPEND, OnPopupInsertItemLastAppend)
	ON_COMMAND(ID_POPUP_INSERT_SUBITEM_FIRST_APPEND, OnPopupInsertSubitemFirstAppend)
	ON_COMMAND(ID_POPUP_INSERT_SUBITEM_LAST_APPEND, OnPopupInsertSubitemLastAppend)
	ON_COMMAND(ID_VIEW_BROWSE_BACK, OnViewBrowseBack)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BROWSE_BACK, OnUpdateViewBrowseBack)
	ON_COMMAND(ID_VIEW_BROWSE_FORWARD, OnViewBrowseForward)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BROWSE_FORWARD, OnUpdateViewBrowseForward)
	ON_COMMAND(ID_POPUP_BT_ITEM_HERE, OnPopupBtItemHere)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_POPUP_EXCEPT_ITEM, OnPopupDeleteItemHere)
	ON_COMMAND(ID_VIEW_DEV, OnViewDev)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_POPUP_ALL_DB0, ID_POPUP_ALL_DB15, OnPopupAllDb0)
	ON_UPDATE_COMMAND_UI_RANGE(ID_POPUP_ALL_DB0, ID_POPUP_ALL_DB15, OnUpdatePopupAllDb0)
	ON_UPDATE_COMMAND_UI_RANGE(ID_BTN_B0, ID_BTN_B3, OnUpdateNoCheckNoEnable)
	ON_UPDATE_COMMAND_UI(ID_BTN_B10, OnUpdateNoCheckNoEnable)
//	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
//	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMObTreeView 診断

#ifdef _DEBUG
void CDMObTreeView::AssertValid() const
{
	CTreeCtrl::AssertValid();
}

void CDMObTreeView::Dump(CDumpContext& dc) const
{
	CTreeCtrl::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDMObTreeView メッセージ ハンドラ

void CDMObTreeView::Init(CDMEdCtx *pEdCtx)
{
	if (m_pEdCtx)
		m_pEdCtx->RemoveCB(this),
		m_pEdCtx->Release();
	if (m_pEdCtx = pEdCtx)
		m_pEdCtx->AddCB(this),
		m_pEdCtx->AddRef();

	InitView();
}

BOOL CDMObTreeView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= 0
		|TVS_LINESATROOT
		|TVS_HASBUTTONS
		|TVS_DISABLEDRAGDROP
		|TVS_HASLINES
		;
	cs.dwExStyle |= 0
		|WS_EX_CLIENTEDGE;
	
	return CTreeCtrl::PreCreateWindow(cs);
}

void CDMObTreeView::OnDMUpdate(UINT nHint, void *pHint)
{
	switch (nHint) {
	case DPM_SELECTLVXY:
	{
		SelectLvXY_t *what = reinterpret_cast<SelectLvXY_t *>(pHint);
		BrowseMap(what->pos, BHF_UPDATE);
		break;
	}
	case DPM_CHANGED_DBITEM:
	{
		ChangeDBRec_t *what = reinterpret_cast<ChangeDBRec_t *>(pHint);
		UPDATEREASON ri;
		ri.iReason = UR_CHANGE_DBITEM;
		ri.pos = what->pos;
		ri.nSRf = what->nSRf;
		UpdateItemsIn(ri);
		break;
	}
	case DPM_INSERTED_DBITEM:
	{
		InsertDBRec_t *what = reinterpret_cast<InsertDBRec_t *>(pHint);
		UPDATEREASON ri;
		ri.iReason = UR_INSERT_DBITEM;
		ri.pos = what->pos;
		UpdateItemsIn(ri);
		break;
	}
	case DPM_CHANGED_ATLASBIT:
	{
		AtlasPos_t pos = *reinterpret_cast<AtlasPos_t *>(pHint);
		if (GetLastHist().iWhat == BE_SEL && GetLastHist().pos == pos) {
			BrowseAgain();
		}
		break;
	}
	case DPM_CHANGED_CONF1:
	{
		UpdateConf1();
		break;
	}
	case DPM_LINKED_DBITEM:
	{
		ChangeDBRec_t *what = reinterpret_cast<ChangeDBRec_t *>(pHint);
		UPDATEREASON ri;
		ri.iReason = UR_CHANGE_DBITEM;
		ri.pos = what->pos;
		ri.nSRf = 0;
		UpdateItemsIn(ri);
		break;
	}
	case DPM_INITVIEW:
	{
		InitView();
		break;
	}
	}
}

void CDMObTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	OnDMUpdate(lHint, pHint);
}

void CDMObTreeView::ReadChainInto(HTREEITEM h, HTREEITEM hAfter, DBIPos_t pos, int nDepth)
{
	if (nDepth >= 16) {
		HTREEITEM ti = InsertItem("!STUCK DETECTED!", h);
		Expand(h, TVE_EXPAND);
		return;
	}

	while (pos != DBIPos_t::END && pos != DBIPos_t::NIL) {
		hAfter = WalkAndInsertPersistItem(h, hAfter, pos, 1, nDepth + 1);
	//	HRESULT hr;
	//	DMDBX *pAny = NULL;
	//	RN ks(pos.r.w);
	//	RN k;
	//	if (ks == RNff80) {
	//		k = ks;
	//	} else {
	//		if (FAILED(hr = GetDDAT().GetDBRecAt(pos, pAny))) break;
	//		int nLinked;
	//		if (FAILED(hr = GetDDAT().BTFromDBRec(pos, k, nLinked))) break;
	//		if (nLinked < 1) k = RN_NIL;
	//		if (nLinked > 1) k = RN_END;
	//	}
	//	RN kSub;
	//	//
	//	HTREEITEM ti;
	//	CString tstr;
	//	if (MakeHint(k, pos, pAny, tstr, kSub)) {
	//		ti = InsertPersistItem(tstr, h, hAfter, pos);

	//		if (kSub != RN_NIL) {
	//			ReadChainInto(ti, TVI_LAST, DBIPos_t(kSub), nDepth + 1);
	//		}
	//	}
	//	//
	//	if (!pAny) break;
	//	pos = DBIPos_t(pAny->next);
	}
	Expand(h, TVE_EXPAND);
}

void CDMObTreeView::OnPopupAllDb0(UINT nID) 
{
	const DBITEMINFO &dbii = GetDbItemInfo(nID);
	int i = dbii.bReadOk;
	if (i) {
		BrowseDb(dbii.iRealm, BHF_UPDATE);
	}
}

void CDMObTreeView::OnUpdatePopupAllDb0(CCmdUI* pCmdUI) 
{
	int i = GetDbItemInfo(pCmdUI->m_nID).bReadOk;
	pCmdUI->Enable(i ? TRUE : FALSE);
}

void CDMObTreeView::OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	if (pNMTreeView) {
		LPARAM lParam = pNMTreeView->itemOld.lParam;
		if (lParam) {
			delete reinterpret_cast<ANYITEM *>(lParam);
		}
	}
	*pResult = 0;
}

void CDMObTreeView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_hRclick = GetDropHilightItem();
	if (!m_hRclick) m_hRclick = GetSelectedItem();

	if (m_hRclick) {
		ANYITEM *pAny = reinterpret_cast<ANYITEM *>(GetItemData(m_hRclick));
		ROOTITEM *pRoot = GetAssocRootAt(m_hRclick);
		ROOTITEM *pRootItem = dynamic_cast<ROOTITEM *>(pAny);
		if (pAny) {
			int iSubMenu = -1;
			CPoint pt;
			GetCursorPos(&pt);

			// 0 E - 辞書表示・ROOT
			// 0 D - 
			// 1 E - 選択表示・ROOT
			// 1 D - 
			// 2 E - 辞書表示・非再帰アイテム
			// 2 D - 
			// 3 E - 選択表示・非再帰アイテム
			// 3 D - 
			// 4 E - 辞書表示・再帰アイテム
			// 4 D - 
			// 5 E - 選択表示・再帰アイテム
			// 5 D - 
			if (pRoot) {
				BOOL bRootDic = dynamic_cast<ROOTDICITEM *>(pRoot) ? 1 : 0;
				BOOL bRootSel = dynamic_cast<ROOTSELITEM *>(pRoot) ? 1 : 0;
				BOOL bRootBT  = dynamic_cast<ROOTBTITEM *>(pRoot) ? 1 : 0;
				BOOL bRootDev = dynamic_cast<ROOTDEVITEM *>(pRoot) ? 1 : 0;
				BOOL bIsRoot = pRootItem ? 1 : 0;
				BOOL bAbleToSub = FALSE;
				BOOL bBTIsDb;
				DEFPERSISTITEM *pItem;
				if (pItem = dynamic_cast<DEFPERSISTITEM *>(pAny)) {
					switch (pItem->pos.db()) {
					case 4: case 9: case 14:
						bAbleToSub = TRUE;
					}
				}
				DEFBTITEM *pItemBT;
				if (pItemBT = dynamic_cast<DEFBTITEM *>(pAny)) {
					bBTIsDb = pItemBT->bIsDb ? 1 : 0;
				}
				int iDetailed = IsLimitless() ? 1 : 0;
				if (bIsRoot) {
					if (bRootDic)
						iSubMenu = 0 + iDetailed;
					else if (bRootSel)
						iSubMenu = 2 + iDetailed;
					else if (bRootBT)
						;
					else if (bRootDev)
						iSubMenu = -1;
					else
						ASSERT(FALSE);
				} else {
					if (bRootDic) {
						if (bAbleToSub)
							iSubMenu = 8 + iDetailed;
						else
							iSubMenu = 4 + iDetailed;
					} else if (bRootSel) {
						if (bAbleToSub)
							iSubMenu =10 + iDetailed;
						else
							iSubMenu = 6 + iDetailed;
					} else if (bRootBT) {
						if (bBTIsDb)
							iSubMenu =12;
						else
							iSubMenu =13;
					} else if (bRootDev) {
						iSubMenu = 8; // 強制割付
					} else {
						ASSERT(FALSE);
					}
				}
			}

			if (iSubMenu >= 0) {
				CMenu *pSubMenu;
				if (pSubMenu = m_aMenu.GetSubMenu(iSubMenu)) {
					pSubMenu->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this, NULL);
				}
			}
		}
	}
	*pResult = 0;
}

HTREEITEM CDMObTreeView::InsertRootSel(AtlasPos_t pos)
{
	/*
	HTREEITEM h = GetChildItem(TVGN_ROOT);
	for (; h != NULL; ) {
		HTREEITEM hh = h;
		h = GetNextItem(h, TVGN_NEXT);
		if (m_tiSel != hh) {
			DeleteItem(hh);
		}
	}
	*/
	DeleteAllItems();
	m_tiDic = m_tiSel = m_tiBT = NULL;

	CString tstrText;
	FormatTileText(tstrText, pos);
	TVINSERTSTRUCT tvis;
	ZeroMemory(&tvis, sizeof(tvis));
	ROOTITEM *pAdd = new ROOTSELITEM(pos.Z(), pos.X(), pos.Y());
	tvis.hParent = TVI_ROOT;
	tvis.hInsertAfter = TVI_LAST;
	tvis.item.mask = TVIF_PARAM|TVIF_TEXT;
	tvis.item.cchTextMax = strlen(tvis.item.pszText = tstrText.LockBuffer());
	tvis.item.lParam = (LPARAM)pAdd;
	m_tiSel = InsertItem(&tvis);
	tstrText.UnlockBuffer();
	if (!m_tiSel)
		delete pAdd;
	return m_tiSel;
}

HTREEITEM CDMObTreeView::InsertRootDic(UINT i, LPCTSTR lpszText)
{
	DeleteAllItems();
	m_tiDic = m_tiSel = m_tiBT = NULL;

	CString tstrText;
	tstrText.Format("%s", lpszText);
	TVINSERTSTRUCT tvis;
	ZeroMemory(&tvis, sizeof(tvis));
	ROOTITEM *pAdd = new ROOTDICITEM(i);
	tvis.hParent = TVI_ROOT;
	tvis.hInsertAfter = TVI_LAST;
	tvis.item.mask = TVIF_PARAM|TVIF_TEXT;
	tvis.item.cchTextMax = strlen(tvis.item.pszText = tstrText.LockBuffer());
	tvis.item.lParam = (LPARAM)pAdd;
	m_tiDic = InsertItem(&tvis);
	tstrText.UnlockBuffer();
	if (!m_tiDic)
		delete pAdd;
	return m_tiDic;
}

HTREEITEM CDMObTreeView::InsertRootBT(DBIPos_t pos)
{
	DeleteAllItems();
	m_tiDic = m_tiSel = m_tiBT = NULL;

	CString tstrText;
	tstrText.Format(
		"BT - %s:%04X",
		MakeNameFromDb(pos.db()), (UINT)pos.idx());
	TVINSERTSTRUCT tvis;
	ZeroMemory(&tvis, sizeof(tvis));
	ROOTITEM *pAdd = new ROOTBTITEM(pos);
	tvis.hParent = TVI_ROOT;
	tvis.hInsertAfter = TVI_LAST;
	tvis.item.mask = TVIF_PARAM|TVIF_TEXT;
	tvis.item.cchTextMax = strlen(tvis.item.pszText = tstrText.LockBuffer());
	tvis.item.lParam = (LPARAM)pAdd;
	m_tiBT = InsertItem(&tvis);
	tstrText.UnlockBuffer();
	if (!m_tiBT)
		delete pAdd;
	return m_tiBT;
}

HTREEITEM CDMObTreeView::InsertBTItem(LPCTSTR lpszText, HTREEITEM hParent, HTREEITEM hInsertAfter, DBIPos_t posTar)
{
	ANYITEM *pAny = new DEFBTITEM(posTar);
	TVINSERTSTRUCT tvis;
	ZeroMemory(&tvis, sizeof(tvis));
	tvis.hParent = hParent;
	tvis.hInsertAfter = hInsertAfter;
	tvis.item.mask = TVIF_PARAM|TVIF_TEXT;
	tvis.item.cchTextMax = strlen(tvis.item.pszText = (LPTSTR)lpszText);
	tvis.item.lParam = (LPARAM)pAny;
	HTREEITEM h = InsertItem(&tvis);
	if (!h)
		delete pAny;
	return h;
}

HTREEITEM CDMObTreeView::InsertBTItem(LPCTSTR lpszText, HTREEITEM hParent, HTREEITEM hInsertAfter, AtlasPos_t posTar)
{
	ANYITEM *pAny = new DEFBTITEM(posTar);
	TVINSERTSTRUCT tvis;
	ZeroMemory(&tvis, sizeof(tvis));
	tvis.hParent = hParent;
	tvis.hInsertAfter = hInsertAfter;
	tvis.item.mask = TVIF_PARAM|TVIF_TEXT;
	tvis.item.cchTextMax = strlen(tvis.item.pszText = (LPTSTR)lpszText);
	tvis.item.lParam = (LPARAM)pAny;
	HTREEITEM h = InsertItem(&tvis);
	if (!h)
		delete pAny;
	return h;
}

HTREEITEM CDMObTreeView::InsertRootDev(LPCTSTR lpszText)
{
	DeleteAllItems();
	m_tiDic = m_tiSel = m_tiBT = NULL;

	CString tstrText = lpszText;
	TVINSERTSTRUCT tvis;
	ZeroMemory(&tvis, sizeof(tvis));
	ROOTITEM *pAdd = new ROOTDEVITEM();
	tvis.hParent = TVI_ROOT;
	tvis.hInsertAfter = TVI_LAST;
	tvis.item.mask = TVIF_PARAM|TVIF_TEXT;
	tvis.item.cchTextMax = strlen(tvis.item.pszText = tstrText.LockBuffer());
	tvis.item.lParam = (LPARAM)pAdd;
	m_tiBT = InsertItem(&tvis);
	tstrText.UnlockBuffer();
	if (!m_tiBT)
		delete pAdd;
	return m_tiBT;
}

HTREEITEM CDMObTreeView::InsertPersistItem(LPCTSTR lpszText, HTREEITEM hParent, HTREEITEM hInsertAfter, DBIPos_t pos)
{
	ANYITEM *pAny = new DEFPERSISTITEM(pos);
	TVINSERTSTRUCT tvis;
	ZeroMemory(&tvis, sizeof(tvis));
	tvis.hParent = hParent;
	tvis.hInsertAfter = hInsertAfter;
	tvis.item.mask = TVIF_PARAM|TVIF_TEXT;
	tvis.item.cchTextMax = strlen(tvis.item.pszText = (LPTSTR)lpszText);
	tvis.item.lParam = (LPARAM)pAny;
	HTREEITEM h = InsertItem(&tvis);
	if (!h)
		delete pAny;
	return h;
}

BOOL CDMObTreeView::ModifyPersistItem(LPCTSTR lpszText, HTREEITEM hItem, DBIPos_t pos)
{
	ANYITEM *pAnyExist;
	TVITEM tvi;
	tvi.mask = TVIF_PARAM;
	tvi.hItem = hItem;
	if (!GetItem(&tvi)) {
		return FALSE;
	}
	if (pAnyExist = reinterpret_cast<ANYITEM *>(tvi.lParam)) {
		delete pAnyExist;
	}
	CString sText = lpszText;
	ANYITEM *pAny = new DEFPERSISTITEM(pos);
	tvi.mask |= TVIF_PARAM|TVIF_TEXT;
	tvi.cchTextMax = _tcslen(tvi.pszText = sText.LockBuffer());
	tvi.lParam = (LPARAM)pAny;
	if (!SetItem(&tvi)) {
		delete pAny;
		return FALSE;
	}
	sText.UnlockBuffer();
	HTREEITEM hSub;
	while (hSub = GetChildItem(hItem)) {
		DeleteItem(hSub);
	}
	return TRUE;
}

void CDMObTreeView::OnSetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
//	if (pTVDispInfo && pTVDispInfo->item.mask & TVIF_STATE && (pTVDispInfo->item.state & TVIS_SELECTED)) {
//		m_hRclick = pTVDispInfo->item.hItem;
//	}
	
	*pResult = 0;
}

void CDMObTreeView::OnPopupInsertItemFirst()
{
	// ID_POPUP_INSERT_ITEM_FIRST
	AddNewItemAt(IAA_SUB_FIRST, m_hRclick);
}

void CDMObTreeView::OnPopupInsertItemLast() 
{
	// ID_POPUP_INSERT_ITEM_LAST
	AddNewItemAt(IAA_SUB_LAST, m_hRclick);
}

void CDMObTreeView::OnPopupInsertItemBefore() 
{
	// ID_POPUP_INSERT_ITEM_BEFORE
	AddNewItemAt(IAA_SIB_PRIOR, m_hRclick);
}

void CDMObTreeView::OnPopupInsertItemAfter() 
{
	// ID_POPUP_INSERT_ITEM_AFTER
	AddNewItemAt(IAA_SIB_NEXT, m_hRclick);
}

void CDMObTreeView::OnPopupInsertSubitemAfter() 
{
	AddNewItemAt(IAA_SUB_FIRST, m_hRclick);
}

void CDMObTreeView::OnPopupInsertSubitemBefore() 
{
	AddNewItemAt(IAA_SUB_LAST, m_hRclick);
}

void CDMObTreeView::OnPopupInsertItemAfterAppend() 
{
	AddNewItemAt(IAA_SIB_NEXT|IAF_APPEND, m_hRclick);
}

void CDMObTreeView::OnPopupInsertItemBeforeAppend() 
{
	AddNewItemAt(IAA_SIB_PRIOR|IAF_APPEND, m_hRclick);
}

void CDMObTreeView::OnPopupInsertItemFirstAppend() 
{
	AddNewItemAt(IAA_SUB_FIRST|IAF_APPEND, m_hRclick);
}

void CDMObTreeView::OnPopupInsertItemLastAppend() 
{
	AddNewItemAt(IAA_SUB_LAST|IAF_APPEND, m_hRclick);
}

void CDMObTreeView::OnPopupInsertSubitemFirstAppend() 
{
	AddNewItemAt(IAA_SUB_FIRST|IAF_APPEND, m_hRclick);
}

void CDMObTreeView::OnPopupInsertSubitemLastAppend() 
{
	AddNewItemAt(IAA_SUB_LAST|IAF_APPEND, m_hRclick);
}

void CDMObTreeView::OnPopupDeleteItemHere() 
{
	DEFPERSISTITEM *pItem;
	if (pItem = dynamic_cast<DEFPERSISTITEM *>(GetAnyAssocAt(m_hRclick))) {
		CDDATUndoLock lock(GetDDAT(), TRUE);

		HRESULT hr;
		RN rnNext;
		DBIPos_t posDel = pItem->pos;
		{
			DMDBU rec;
			if (FAILED(hr = GetDDAT().GetDBRecAt(posDel, rec)))
				return;
			rnNext = rec.r0.next;
		}
		HTREEITEM hPrior = GetPrevSiblingItem(m_hRclick);
		HTREEITEM hParent;
		BOOL bDeleted = FALSE;
		if (hPrior) {
			DEFPERSISTITEM *pPersistItem;
			if (pPersistItem = dynamic_cast<DEFPERSISTITEM *>(GetAnyAssocAt(hPrior))) {
				DBIPos_t pos = pPersistItem->pos;
				DMDBU recDet;
				if (FAILED(hr = GetDDAT().GetDetachedDBRecAt(pos, recDet)))
					return;
				recDet.r0.next = rnNext;
				if (FAILED(hr = GetDDAT().SetDBRecAt(pos, recDet)))
					return;
			}
		} else if (hParent = GetParentItem(m_hRclick)) {
			ANYITEM *pAnyItem = GetAnyAssocAt(hParent);
			DEFPERSISTITEM *pPersistItem;
			ROOTSELITEM *pRootSel;
			ROOTDICITEM *pRootDic;
			if (pPersistItem = dynamic_cast<DEFPERSISTITEM *>(pAnyItem)) {
				DBIPos_t pos = pPersistItem->pos;
				DMDBU recDet;
				if (FAILED(hr = GetDDAT().GetDetachedDBRecAt(pos, recDet)))
					return;
				recDet.r4.stolen = rnNext;
				if (FAILED(hr = GetDDAT().SetDBRecAt(pos, recDet)))
					return;
			} else if (pRootSel = dynamic_cast<ROOTSELITEM *>(pAnyItem)) {
				if (rnNext == RN_END)
					rnNext = RN_NIL;
				AtlasPos_t pos(pRootSel->lv, pRootSel->x, pRootSel->y);
				AtlasBit_t o;
				if (FAILED(hr = GetDDAT().GetAtlasBitAt(pos, o)))
					return;
				o.r = rnNext;
				if (FAILED(hr = GetDDAT().SetAtlasBitAt(pos, o)))
					return;
			} else if (pRootDic = dynamic_cast<ROOTDICITEM *>(pAnyItem)) {
				GetDDAT().RemoveDBRecAt(pItem->pos);
				bDeleted = TRUE;
			}
		}
		if (!bDeleted) {
			if (FAILED(hr = GetDDAT().RemoveDBRecAssoc(posDel, FALSE, TRUE)))
				return;
		}
		lock.Unlock(TRUE);
	}
}

BOOL CDMObTreeView::AddNewItemAt(UINT nIAA, HTREEITEM hAssoc)
{
	ROOTITEM *pRoot = GetAssocRootAt(hAssoc);
	ROOTDICITEM *pRootDic;
	if (pRootDic = dynamic_cast<ROOTDICITEM *>(pRoot)) {
		return AddNewDicItemAt(pRootDic, nIAA, hAssoc);
	}
	ANYITEM *pAny = GetAnyAssocAt(hAssoc);
	if (pAny) {
		return AddNewSelItemAt(pAny, nIAA, hAssoc);
	}
	return FALSE;
}

BOOL CDMObTreeView::AddNewSelItemAt(ANYITEM *pItem, UINT nIAA, HTREEITEM hAssoc)
{
	if (!hAssoc)
		return FALSE;
	UINT nIAF = nIAA & IAF_MASK;
	nIAA = nIAA & IAA_MASK;
	DEFPERSISTITEM *pPersistItem = dynamic_cast<DEFPERSISTITEM *>(GetAnyAssocAt(hAssoc));
	CEditDBItemDlg wndDlg(FALSE, this);
	while (true) {
		int r = wndDlg.DoModal();
		if (r != IDOK)
			return FALSE;
		CDDATUndoLock lock(GetDDAT(), TRUE);
		UINT iDB = wndDlg.GetLastTabSel();
		UINT iAlign = wndDlg.m_iAlign;
		DMDBU rec;
		wndDlg.SaveAsRec(rec, true);
		rec.r0.next = RN_END;
		HRESULT hr;
		DBIPos_t posNew;
		if (nIAF & IAF_APPEND) {
			posNew.idx(1024);
			if (FAILED(hr = GetDDAT().AddDBRecTo(iDB, rec, posNew)))
				return FALSE;
		} else {
			if (FAILED(hr = GetDDAT().AddDBRecToEmpty(iDB, rec, posNew)))
				return FALSE;
		}
		RN rnNewObj = RNco(posNew.db(), posNew.idx(), iAlign);
		int iWhich = -1;
		DBIPos_t posDBIAt;
		AtlasPos_t posAtlas;
		switch (nIAA) {
		case IAA_SUB_FIRST:
		case IAA_SUB_LAST:
			{
				ROOTSELITEM *pRootSel;
				if (pRootSel = dynamic_cast<ROOTSELITEM *>(pItem)) {
					posAtlas = AtlasPos_t(pRootSel->lv, pRootSel->x, pRootSel->y);
					iWhich = 0;

					HTREEITEM hTar;
					if (nIAA == IAA_SUB_LAST && (hTar = GetChildItem(hAssoc))) {
						for (; ; ) {
							HTREEITEM h = hTar;
							h = GetNextSiblingItem(h);
							if (!h) break;
							hTar = h;
						}
						DEFPERSISTITEM *pItem;
						if (pItem = dynamic_cast<DEFPERSISTITEM *>(GetAnyAssocAt(hTar))) {
							posDBIAt = pItem->pos;
							iWhich = 1;
						}
					}
				} else if (pPersistItem) {
					posDBIAt = pPersistItem->pos;
					iWhich = 2;

					HTREEITEM hTar;
					if (nIAA == IAA_SUB_LAST) {
						if (hTar = GetChildItem(hAssoc)) {
							for (; ; ) {
								HTREEITEM h = hTar;
								h = GetNextSiblingItem(h);
								if (!h) break;
								hTar = h;
							}
							DEFPERSISTITEM *pItem;
							if (pItem = dynamic_cast<DEFPERSISTITEM *>(GetAnyAssocAt(hTar))) {
								posDBIAt = pItem->pos;
								iWhich = 1;
							}
						} else {
							
						}
					}
				}
				break;
			}
		case IAA_SIB_PRIOR:
			{
				HTREEITEM hPrior = GetPrevSiblingItem(hAssoc);
				if (hPrior) {
					ANYITEM *pItem = GetAnyAssocAt(hPrior);
					DEFPERSISTITEM *pPersist = dynamic_cast<DEFPERSISTITEM *>(pItem);
					if (pPersist) {
						posDBIAt = pPersist->pos;
						iWhich = 1;
					}
				} else {
					HTREEITEM hParent = GetParentItem(hAssoc);
					ANYITEM *pItem = GetAnyAssocAt(hParent);
					ROOTSELITEM *pRootSel;
					if (pRootSel = dynamic_cast<ROOTSELITEM *>(pItem)) {
						posAtlas = AtlasPos_t(pRootSel->lv, pRootSel->x, pRootSel->y);
						iWhich = 0;
					}
					DEFPERSISTITEM *pPersist = dynamic_cast<DEFPERSISTITEM *>(pItem);
					if (pPersist) {
						posDBIAt = pPersist->pos;
						iWhich = 2;
					}
				}
				break;
			}
		case IAA_SIB_NEXT:
			if (pPersistItem) {
				posDBIAt = pPersistItem->pos;
				iWhich = 1;
			}
			break;
		}
		switch (iWhich) {
		case 0:
			{
				AtlasBit_t o;
				HRESULT hr;
				if (FAILED(hr = GetDDAT().GetAtlasBitAt(posAtlas, o)))
					return FALSE;
				RN rnPriorObj = o.r;
				o.c|= 0x10;
				o.r = rnNewObj;
				if (FAILED(hr = GetDDAT().SetAtlasBitAt(posAtlas, o)))
					return FALSE;
				if (rnPriorObj != RN_NIL) {
					DMDBU recDet;
					if (FAILED(hr = GetDDAT().GetDetachedDBRecAt(posNew, recDet)))
						return FALSE;
					recDet.r0.next = rnPriorObj;
					if (FAILED(hr = GetDDAT().SetDBRecAt(posNew, recDet)))
						return FALSE;
				}
				break;
			}
		case 1:
		case 2:
			{
				DMDBU recDet;
				if (FAILED(hr = GetDDAT().GetDetachedDBRecAt(posDBIAt, recDet)))
					return FALSE;
				RN rnPriorObj;
				if (iWhich == 1) {
					rnPriorObj = recDet.r0.next;
					recDet.r0.next = rnNewObj;
				} else {
					rnPriorObj = recDet.r4.stolen;
					recDet.r4.stolen = rnNewObj;
				}
				if (FAILED(hr = GetDDAT().SetDBRecAt(posDBIAt, recDet)))
					return FALSE;
				if (rnPriorObj != RN_NIL) {
					DMDBU recDet;
					if (FAILED(hr = GetDDAT().GetDetachedDBRecAt(posNew, recDet)))
						return FALSE;
					recDet.r0.next = rnPriorObj;
					if (FAILED(hr = GetDDAT().SetDBRecAt(posNew, recDet)))
						return FALSE;
				}
				break;
			}

		}
		if (FAILED(hr))
			break;
		lock.Unlock(TRUE);
		return TRUE;
	}
	return FALSE;
}

BOOL CDMObTreeView::AddNewDicItemAt(ROOTDICITEM *pRoot, UINT nIAA, HTREEITEM hAssoc)
{
	nIAA = nIAA & IAA_MASK;
	DEFPERSISTITEM *pPersistItem = dynamic_cast<DEFPERSISTITEM *>(GetAnyAssocAt(hAssoc));
	UINT iDB = pRoot->db;
	CEditDBItemDlg wndDlg(-1, iDB, m_bLimitless, this);
	while (true) {
		int r = wndDlg.DoModal();
		if (r != IDOK)
			return FALSE;
		DMDBU rec;
		wndDlg.SaveAsRec(rec, true);
		RN k = RNco(iDB, 0, wndDlg.m_iAlign);
		r = IDABORT;
		HRESULT hr = E_FAIL;
		UINT nMax = GetDDAT().CntDBRecIn(iDB);
		UINT iIdxAt = pPersistItem ? (pPersistItem->pos.idx()) : nMax;
		switch (nIAA) {
		case IAA_SUB_FIRST:
			k.idx(0);
			break;
		case IAA_SUB_LAST:
			k.idx(nMax);
			break;
		case IAA_SIB_PRIOR:
			k.idx(iIdxAt);
			break;
		case IAA_SIB_NEXT:
			k.idx(iIdxAt + 1);
			break;
		}
		CWaitCursor wc;
		hr = GetDDAT().InsertDBRecAt(DBIPos_t(k), rec);
		if (FAILED(hr))
			break;
		return TRUE;
	}
	return FALSE;
}

CDMObTreeView::ROOTITEM *CDMObTreeView::GetAssocRootAt(HTREEITEM h)
{
	ROOTITEM *pItem = NULL;
	while (h) {
		LPARAM lParam = GetItemData(h);
		if (lParam && (pItem = dynamic_cast<ROOTITEM *>(reinterpret_cast<ANYITEM *>(lParam)))) {
			break;
		}
		h = GetParentItem(h);
	}
	return pItem;
}

void CDMObTreeView::BrowseDb(UINT iDB, UINT iBHF)
{
	_ASSERTE(iDB < 16);
	const DBITEMINFO &dbii = m_aDBII[iDB];

	CWaitCursor wc;

	DeleteAllItems();
	HTREEITEM h = InsertRootDic(dbii.iRealm, dbii.lpszRealm);
	UpdateWindow();

	CString tstr;
	for (UINT i=0, n=GetDDAT().CntDBRecIn(iDB); i<n; i++) {
		DBIPos_t pos(iDB, i);
		DMDBU rec;
		HRESULT hr;
		VERIFY(SUCCEEDED(hr = GetDDAT().GetDBRecAt(pos, rec)));
		BOOL fExist = SUCCEEDED(hr);
		RN k, k2;
		int nLinked;
		if (FAILED(hr = GetDDAT().BTFromDBRec(pos, k, nLinked))) {
			k = RN_NIL;
		} else if (nLinked > 1) {
			k = RN_END;
		}
		if (!MakeHint(k, pos, fExist ? &rec : NULL, tstr, k2)) break;
		InsertPersistItem(tstr, h, TVI_LAST, pos);
	}
	Expand(h, TVE_EXPAND);

	if (iBHF & BHF_UPDATE) {
		BROWSEHIST bh;
		bh.iWhat = BE_DB; bh.iDB = iDB;
		m_hlForward.clear();
		m_hlBack.push_front(bh);
	}
}

void CDMObTreeView::BrowseMap(AtlasPos_t pos, UINT iBHF)
{
	HRESULT hr;
	AtlasBit_t one;
	VERIFY(SUCCEEDED(hr = GetDDAT().GetAtlasBitAt(pos, one)));

	HTREEITEM h = InsertRootSel(pos);
	ReadChainInto(h, TVI_LAST, DBIPos_t(one.r));

	if (iBHF & BHF_UPDATE) {
		BROWSEHIST bh;
		bh.iWhat = BE_SEL; bh.pos = pos;
		m_hlForward.clear();
		m_hlBack.push_front(bh);
	}
}

void CDMObTreeView::BrowseBT(DBIPos_t pos, UINT iBHF)
{
	CWaitCursor wc;

	DeleteAllItems();
	HTREEITEM h = InsertRootBT(pos);
	UpdateWindow();

	HRESULT hr;
	list<AtlasPos_t> la;
	hr = GetDDAT().BTFromDBRec(pos, la);
	while (!la.empty()) {
		CString tstrText;
		FormatTileText(tstrText, la.front());
		InsertBTItem(tstrText, h, TVI_LAST, la.front());

		la.pop_front();
	}
	list<DBIPos_t> ld;
	hr = GetDDAT().BTFromDBRec(pos, ld);
	while (!ld.empty()) {
		CString tstrText;
		FormatBTDbText(tstrText, ld.front());
		HTREEITEM h2 = InsertBTItem(tstrText, h, TVI_LAST, ld.front());

		la.clear();
		WalkAndFindOriginalReferer(ld.front(), la, 0);
		if (la.empty()) {
			InsertItem("!NOT ON MAP!", h2, TVI_LAST);
		} else {
			do {
				CString tstrText;
				FormatTileText(tstrText, la.front());
				InsertBTItem(tstrText, h2, TVI_LAST, la.front());

				la.pop_front();
			} while (!la.empty());
		}

		Expand(h2, TVE_EXPAND);

		ld.pop_front();
	}

	Expand(h, TVE_EXPAND);

	if (iBHF & BHF_UPDATE) {
		BROWSEHIST bh;
		bh.iWhat = BE_BT; bh.posBT = pos;
		m_hlForward.clear();
		m_hlBack.push_front(bh);
	}
}

CDMObTreeView::ANYITEM *CDMObTreeView::GetAnyAssocAt(HTREEITEM h)
{
	if (!h)
		return NULL;
	LPARAM lParam = GetItemData(h);
	ANYITEM *pItem = NULL;
	if (lParam)
		pItem = reinterpret_cast<ANYITEM *>(lParam);
	return pItem;
}

void CDMObTreeView::OnPopupEditItemHere() 
{
	DEFPERSISTITEM *pItem;
	ROOTSELITEM *pRootSel;
	if (pItem = dynamic_cast<DEFPERSISTITEM *>(GetAnyAssocAt(m_hRclick))) {
		CDDATUndoLock lock(GetDDAT(), TRUE);

		AtlasPos_t pos0;
		DBIPos_t pos1;
		int iWhich = -1;

		DMDBU rec;
		int iAlign = -1;
		HRESULT hr;
		if (FAILED(hr = GetDDAT().GetDBRecAt(pItem->pos, rec)))
			return;

		HTREEITEM hParent = GetParentItem(m_hRclick);
		ROOTDICITEM *pRootDic;
		if (pRootDic = dynamic_cast<ROOTDICITEM *>(GetAnyAssocAt(hParent))) {

		} else {
			HTREEITEM hPrior = GetPrevSiblingItem(m_hRclick);
			if (hPrior) {
				DEFPERSISTITEM *pPersistItem = dynamic_cast<DEFPERSISTITEM *>(GetAnyAssocAt(hPrior));
				if (pPersistItem) {
					DMDBU recIt;
					if (FAILED(hr = GetDDAT().GetDBRecAt(pPersistItem->pos, recIt)))
						return;
					iAlign = recIt.r0.next.room();

					iWhich = 1; pos1 = pPersistItem->pos;
				}
			} else {
				ANYITEM *pItem = GetAnyAssocAt(hParent);
				DEFPERSISTITEM *pPersistItem;
				ROOTSELITEM *pRootSel;
				if (pPersistItem = dynamic_cast<DEFPERSISTITEM *>(pItem)) {
					DMDBU recIt;
					if (FAILED(hr = GetDDAT().GetDBRecAt(pPersistItem->pos, recIt)))
						return;
					iAlign = recIt.r4.stolen.room();

					iWhich = 2; pos1 = pPersistItem->pos;
				} else if (pRootSel = dynamic_cast<ROOTSELITEM *>(pItem)) {
					iWhich = 0; pos0 = AtlasPos_t(pRootSel->lv, pRootSel->x, pRootSel->y);

					AtlasBit_t o;
					if (FAILED(hr = GetDDAT().GetAtlasBitAt(pos0, o)))
						return;
					iAlign = o.r.room();
				}
			}
		}
		{
			CEditDBItemDlg wndDlg(iAlign, pItem->pos.db(), rec, m_bLimitless, this);
			int r = wndDlg.DoModal();
			if (r != IDOK)
				return;
			DMDBU recNew;
			wndDlg.SaveAsRec(recNew, true);
			if (!m_bLimitless) {
				recNew.r0.next = rec.r0.next;
				switch (pItem->pos.db()) {
				case 4:
				case 9:
				case 14:
					recNew.r4.stolen = rec.r4.stolen; break;
				}
			}
			hr = GetDDAT().SetDBRecAt(pItem->pos, recNew);
			iAlign = wndDlg.m_iAlign;
			if (iAlign != -1) {
				switch (iWhich) {
				case 0:
					{
						AtlasBit_t o;
						if (FAILED(hr = GetDDAT().GetAtlasBitAt(pos0, o)))
							return;
						o.r.room(iAlign);
						if (FAILED(hr = GetDDAT().SetAtlasBitAt(pos0, o)))
							return;
						break;
					}
				case 1:
				case 2:
					{
						DMDBU recDet;
						if (FAILED(hr = GetDDAT().GetDetachedDBRecAt(pos1, recDet)))
							return;
						if (iWhich == 1) {
							recDet.r0.next.room(iAlign);
						} else {
							recDet.r4.stolen.room(iAlign);
						}
						if (FAILED(hr = GetDDAT().SetDBRecAt(pos1, recDet)))
							return;
						break;
					}
				}
			}
		}
		lock.Unlock(TRUE);
		return;
	} else if (pRootSel = dynamic_cast<ROOTSELITEM *>(GetAnyAssocAt(m_hRclick))) {
		AtlasPos_t pos(pRootSel->lv, pRootSel->x, pRootSel->y);
		HRESULT hr;
		AtlasBit_t o;
		if (SUCCEEDED(hr = GetDDAT().GetAtlasBitAt(pos, o))) {
			CEditDMRootSel wndDlg(o.r, this);
			int r = wndDlg.DoModal();
			if (r == IDOK) {
				o.r = wndDlg.m_rnNext;
				VERIFY(SUCCEEDED(hr = GetDDAT().SetAtlasBitAt(pos, o)));
			}
		}
	}
}

void CDMObTreeView::UpdateItemsIn(UPDATEREASON &rReason)
{
	HTREEITEM h1 = GetRootItem();
	while (h1) {
		if (m_tiDic == h1 || m_tiSel == h1) {
			UpdateDescentItemsIn(h1, rReason);
		}
		h1 = GetNextSiblingItem(h1);
	}
}

void CDMObTreeView::UpdateDescentItemsIn(HTREEITEM h1, UPDATEREASON &rReason)
{
	ANYITEM *pRootAny = GetAnyAssocAt(h1);
	UINT nFlags =
		(dynamic_cast<ROOTDICITEM *>(pRootAny) ? 0 : 1);
	UINT nSRf = rReason.nSRf;
	if (rReason.iReason == UR_CHANGE_DBITEM) {
		if (!(nFlags & 1)) {
			HTREEITEM h2 = GetChildItem(h1);
			while (h2) {
				ANYITEM *pAny = GetAnyAssocAt(h2);
				DEFPERSISTITEM *pItem;
				if (pItem = dynamic_cast<DEFPERSISTITEM *>(pAny)) {
					if (pItem->pos == rReason.pos) {
						DBIPos_t pos = pItem->pos;
						h2 = WalkAndInsertPersistItem(h1, h2, pos, nFlags|2, 1);
						break;
					}
				}
				h2 = GetNextSiblingItem(h2);
			}
		} else {
			HTREEITEM h2 = GetChildItem(h1);
			while (h2) {
				ANYITEM *pAny = GetAnyAssocAt(h2);
				DEFPERSISTITEM *pItem;
				if (pItem = dynamic_cast<DEFPERSISTITEM *>(pAny)) {
					if (pItem->pos == rReason.pos) {
						DBIPos_t pos = pItem->pos;
						h2 = WalkAndInsertPersistItem(h1, h2, pos, nFlags|2, 1);
						if (nSRf & (DDSRF_NEW_NEXT)) {
							h2 = GetNextSiblingItem(h2);
							if (!h2) h2 = TVI_LAST;
							while (pos != DBIPos_t::END && pos != DBIPos_t::NIL) {
								if (h2 != TVI_LAST) {
									h2 = WalkAndInsertPersistItem(h1, h2, pos, nFlags|2, 1);
									h2 = GetNextSiblingItem(h2);
									if (!h2) h2 = TVI_LAST;
								} else {
									WalkAndInsertPersistItem(h1, h2, pos, nFlags, 1);
								}
							}
							while (h2 != NULL && h2 != TVI_LAST) {
								HTREEITEM h = h2;
								h2 = GetNextSiblingItem(h2);
								DeleteItem(h);
							}
						}
						break;
					} else {
						UpdateDescentItemsIn(h2, rReason);
					}
				}
				h2 = GetNextSiblingItem(h2);
			}
		}
	} else if (rReason.iReason == UR_INSERT_DBITEM) {
		ROOTDICITEM *pRoot;
		UINT iDB = rReason.pos.db();
		if ((pRoot = dynamic_cast<ROOTDICITEM *>(pRootAny)) && (iDB == pRoot->db)) {
			HTREEITEM h2 = GetChildItem(h1);
			while (h2) {
				ANYITEM *pAny = GetAnyAssocAt(h2);
				DEFPERSISTITEM *pItem;
				if (pItem = dynamic_cast<DEFPERSISTITEM *>(pAny)) {
					if (rReason.pos.idx() <= pItem->pos.idx()) {
						break;
					}
				}
				h2 = GetNextSiblingItem(h2);
			}
			UINT iIdx = rReason.pos.idx(), iIdxTo = GetDDAT().CntDBRecIn(iDB);
			if (h2 == NULL)
				h2 = TVI_LAST;
			for (; iIdx != iIdxTo; iIdx++) {
				DBIPos_t pos(iDB, iIdx);
				if (h2 == TVI_LAST) {
					WalkAndInsertPersistItem(h1, h2, pos, nFlags, 1);
				} else {
					h2 = WalkAndInsertPersistItem(h1, h2, pos, nFlags|2, 1);
					h2 = GetNextSiblingItem(h2);
					if (!h2) h2 = TVI_LAST;
				}
			}
			while (h2 != NULL && h2 != TVI_LAST) {
				HTREEITEM h = h2;
				h2 = GetNextSiblingItem(h2);
				DeleteItem(h);
			}
		}
	}
}

HTREEITEM CDMObTreeView::WalkAndInsertPersistItem(HTREEITEM hParent, HTREEITEM hInsertAfter, DBIPos_t &pos, UINT nFlags, int nDepth)
{
	try {
		HRESULT hr;
		DMDBU *pRec = NULL;
		DMDBU rec;
		RN ks = RNco(pos.r.w);
		RN k;
		if (ks == RNff80) {
			k = ks;
		} else {
			if (FAILED(hr = GetDDAT().GetDBRecAt(pos, rec))) throw int();
			pRec = &rec;
			int nLinked;
			if (FAILED(hr = GetDDAT().BTFromDBRec(pos, k, nLinked))) throw int();
			if (nLinked < 1) k = RN_NIL;
			if (nLinked > 1) k = RN_END;
		}
		RN kSub;
		//
		HTREEITEM ti = NULL;
		CString tstr;
		if (MakeHint(k, pos, pRec, tstr, kSub)) {
			if (nFlags & 2) {
				ti = ModifyPersistItem(tstr, hInsertAfter, pos) ? hInsertAfter : NULL;
			} else {
				ti = InsertPersistItem(tstr, hParent, hInsertAfter, pos);
			}

			if (kSub != RN_NIL && (nFlags & 1)) {
				ReadChainInto(ti, TVI_LAST, DBIPos_t(kSub), nDepth + 1);
			}
		}
		//
		if (!pRec) throw int();
		pos = DBIPos_t(rec.r0.next);
		return ti;
	} catch (int) {

	}
	pos = DBIPos_t(RN_NIL);
	return NULL;
}

BOOL CDMObTreeView::ModifyNextLinkOfItem(HTREEITEM hItem, int iDelta, RN rn, RN &rnFollow)
{
	if (hItem == NULL)
		return FALSE;
	HTREEITEM hTar;
	int iSub = 0;
	if (iDelta == 0) {
		hTar = hItem;
	} else if (iDelta < 0) {
		hTar = GetPrevSiblingItem(hItem);
		if (hTar == NULL) {
			hTar = GetParentItem(hItem);
			iSub = 1;
		}
	} else if (iDelta > 0) {
		hTar = GetNextSiblingItem(hItem);
		if (hTar == NULL)
			return FALSE;
	}
	ANYITEM *pItem = GetAnyAssocAt(hTar);
	ROOTSELITEM *pRootSel;
	if (pRootSel = dynamic_cast<ROOTSELITEM *>(pItem)) {
		_ASSERTE(iSub != 0);
		AtlasPos_t pos(pRootSel->lv, pRootSel->x, pRootSel->y);
		AtlasBit_t o;
		HRESULT hr;
		if (FAILED(hr = GetDDAT().GetAtlasBitAt(pos, o)))
			return FALSE;
		rnFollow = o.r;
		o.r = rn;
		if (FAILED(hr = GetDDAT().SetAtlasBitAt(pos, o)))
			return FALSE;
		return TRUE;
	}
	DEFPERSISTITEM *pPersistItem;
	if (pPersistItem = dynamic_cast<DEFPERSISTITEM *>(pItem)) {
		if (iSub == 0) {
			HRESULT hr;
			DMDBU rec;
			if (FAILED(hr = GetDDAT().GetDetachedDBRecAt(pPersistItem->pos, rec)))
				return FALSE;
			rnFollow = rec.r0.next;
			rec.r0.next = rn;
			hr = GetDDAT().SetDBRecAt(pPersistItem->pos, rec);
			if (FAILED(hr))
				return FALSE;
			return TRUE;
		} else {
			switch (pPersistItem->pos.db()) {
			case 4: case 9: case 14:
				{
					HRESULT hr;
					DMDBU rec;
					if (FAILED(hr = GetDDAT().GetDetachedDBRecAt(pPersistItem->pos, rec)))
						return FALSE;
					rnFollow = rec.r4.stolen;
					rec.r4.stolen = rn;
					hr = GetDDAT().SetDBRecAt(pPersistItem->pos, rec);
					if (FAILED(hr))
						return FALSE;
				}
				return TRUE;
			default:
				return FALSE;
			}
		}
	}
	return FALSE;
}

void CDMObTreeView::OnUpdateNoCheckNoEnable(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(0);
	pCmdUI->Enable(FALSE);
}

void CDMObTreeView::OnViewBrowseBack() 
{
	_ASSERTE(!m_hlBack.empty());

	m_hlForward.push_front(m_hlBack.front());
	m_hlBack.pop_front();

	BrowseAgain();
}

void CDMObTreeView::OnUpdateViewBrowseBack(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_hlBack.size() >= 2);
}

void CDMObTreeView::OnViewBrowseForward() 
{
	_ASSERTE(!m_hlForward.empty());

	m_hlBack.push_front(m_hlForward.front());
	m_hlForward.pop_front();

	BrowseAgain();
}

void CDMObTreeView::OnUpdateViewBrowseForward(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_hlForward.empty());
}

void CDMObTreeView::BrowseAgain()
{
	BROWSEHIST bh = GetLastHist();
	switch (bh.iWhat) {
	case BE_SEL:
		BrowseMap(bh.pos, 0);
		break;
	case BE_DB:
		BrowseDb(bh.iDB, 0);
		break;
	case BE_BT:
		BrowseBT(bh.posBT, 0);
		break;
	case BE_NO:
		MakeNotSelected();
		break;
	case BE_DEV:
		BrowseDevView(0);
		break;
	default:
		ASSERT(FALSE);
	}
}

void CDMObTreeView::MakeNotSelected()
{
	DeleteAllItems();
	InsertItem("Not Selected");

	m_tiSel = m_tiDic = m_tiBT = m_hRclick = NULL;
}

void CDMObTreeView::OnPopupBtItemHere() 
{
	ANYITEM *pAny = GetAnyAssocAt(m_hRclick);
	DEFPERSISTITEM *pItem;
	if (pItem = dynamic_cast<DEFPERSISTITEM *>(pAny)) {
		BrowseBT(pItem->pos, BHF_UPDATE);
		return;
	}
	DEFBTITEM *pItemBT;
	if (pItemBT = dynamic_cast<DEFBTITEM *>(pAny)) {
		if (pItemBT->bIsDb) {
			BrowseBT(pItemBT->posDb, BHF_UPDATE);
		} else {
			AtlasPos_t posAt = pItemBT->posAt;
			BrowseMap(posAt, BHF_UPDATE);

			SelectLvXY_t a;
			a.pos = posAt;
			m_pEdCtx->DispatchDMUpdate(DPM_SELECTBTLVXY, &a);
		}
		return;
	}
}

BOOL CDMObTreeView::WalkAndFindOriginalReferer(const DBIPos_t &pos, list<AtlasPos_t> &referer, int iDepth)
{
	if (iDepth >= 96 || IsThisAsIs(RNco(pos.r.w)))
		return FALSE;
	HRESULT hr;
	list<AtlasPos_t> la;
	hr = GetDDAT().BTFromDBRec(pos, la);
	while (!la.empty()) {
		if (find(referer.begin(), referer.end(), la.front()) == referer.end()) {
			referer.push_back(la.front());
		}

		la.pop_front();
	}
	list<DBIPos_t> ld;
	hr = GetDDAT().BTFromDBRec(pos, ld);
	while (!ld.empty()) {
		WalkAndFindOriginalReferer(ld.front(), referer, iDepth + 1);

		ld.pop_front();
	}
	return TRUE;
}

void CDMObTreeView::PostNcDestroy() 
{
	if (m_pEdCtx)
		m_pEdCtx->RemoveCB(this),
		m_pEdCtx->Release(),
		m_pEdCtx = NULL;
	
	CTreeCtrl::PostNcDestroy();
}

void CDMObTreeView::InitView()
{
	LOGFONT lfNice;
	ReadFontTemplateFrom(lfNice, IDS_FONTTEMP_DMOBTREEVIEW, "Courier;9;;;;;;;;;;;;1");
//	ZeroMemory(&lfNice, sizeof(lfNice));
//	lfNice.lfHeight = 8;
//	lfNice.lfPitchAndFamily = FIXED_PITCH;
//	lfNice.lfQuality = PROOF_QUALITY;
//	_tcscpy(lfNice.lfFaceName, "Courier");
	m_fontNice.DeleteObject();
	m_fontNice.CreateFontIndirect(&lfNice);

	SetBkColor(0x000000);
	SetTextColor(0xffffff);
	SetFont(&m_fontNice);

	MakeNotSelected();

	m_aMenu.DestroyMenu();
	m_aMenu.LoadMenu(IDR_OBJVIEW);

	UpdateConf1();

	m_hlBack.clear();
	m_hlForward.clear();

	BROWSEHIST bh;
	bh.iWhat = BE_NO;
	m_hlBack.push_back(bh);
}

void CDMObTreeView::UpdateConf1()
{
	CDMEdCtxAccessibility conf;
	m_pEdCtx->GetAccessibility(conf);

	m_bLimitless = conf.bLimitless;
}

void CDMObTreeView::OnEditCopy() 
{
	HTREEITEM hItem = GetSelectedItem();
	HTREEITEM hPrev = GetPrevSiblingItem(hItem);
	HRESULT hr;
	DMDBU rec;
	RN rn = RN_NIL;
	if (hPrev) {
		DEFPERSISTITEM *pItem = dynamic_cast<DEFPERSISTITEM *>(GetAnyAssocAt(hPrev));
		if (pItem) {
			if (SUCCEEDED(hr = m_pEdCtx->GetDDIf()->GetDBRecAt(pItem->pos, rec))) {
				rn = rec.r0.next;
			}
		}
	} else {
		HTREEITEM hParent = GetParentItem(hItem);
		DEFPERSISTITEM *pItem = dynamic_cast<DEFPERSISTITEM *>(GetAnyAssocAt(hParent));
		if (pItem) {
			if (SUCCEEDED(hr = m_pEdCtx->GetDDIf()->GetDBRecAt(pItem->pos, rec))) {
				rn = rec.r4.stolen;
			}
		} else {
			ROOTSELITEM *pItem = dynamic_cast<ROOTSELITEM *>(GetAnyAssocAt(hParent));
			if (pItem) {
				AtlasBit_t o;
				if (SUCCEEDED(hr = m_pEdCtx->GetDDIf()->GetAtlasBitAt(AtlasPos_t(pItem->lv, pItem->x, pItem->y), o))) {
					rn = o.r;
				}
			}
		}
	}
	if (rn == RN_NIL) return;
	CDMObTreeData z;
	z.Create(*m_pEdCtx->GetDDIf(), rn);
	CString strText = z.Mk();
	if (OpenClipboard()) {
		if (EmptyClipboard()) {
			SetClipboardTextData(strText);
		}
		CloseClipboard();
	}
}

void CDMObTreeView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	bool fEnabled = false;
	HTREEITEM hSel = GetSelectedItem();
	ANYITEM *pAny = GetAnyAssocAt(hSel);
	fEnabled = (dynamic_cast<DEFPERSISTITEM *>(pAny) != NULL);

	pCmdUI->Enable(fEnabled);
}

void CDMObTreeView::OnEditPaste() 
{
	bool fOk = false;
	if (OpenClipboard()) {
		CString strText;
		if (GetClipboardTextData(strText)) {
			CDMObTreeData z;
			if (z.CreateFromText(strText)) {
				//z.InsertTo();
			}
		}
		CloseClipboard();
	}
}

void CDMObTreeView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	bool fEnabled = false;
	if (OpenClipboard()) {
		fEnabled = IsClipboardFormatAvailable(CF_TEXT) ? true : false;
		CloseClipboard();
	}
}

void CDMObTreeView::OnViewDev() 
{
	BrowseDevView(BHF_UPDATE);
}

void CDMObTreeView::BrowseDevView(UINT iBHF)
{
	CWaitCursor wc;

	DeleteAllItems();
	HTREEITEM h = InsertRootDev(_T("Dev Explorer"));
	UpdateWindow();

	UINT i, n = GetDDAT().CntDBRecIn(2);

	CString tstr;

	for (i = 0; i < n; i++) {
		DBIPos_t pos(2, i);
		DMDBU rec;
		HRESULT hr;
		VERIFY(SUCCEEDED(hr = GetDDAT().GetDBRecAt(pos, rec)));

		WORD w2 = rec.r2.w2;

		tstr.Format(
			"#%04d %04X %c%c%c%c%c--------%c%c%c"
			, i, 0U +w2
			, (w2 & (1<<15)) ? '1' : '0'
			, (w2 & (1<<14)) ? '1' : '0'
			, (w2 & (1<<13)) ? '1' : '0'
			, (w2 & (1<<12)) ? '1' : '0'
			, (w2 & (1<<11)) ? '1' : '0'
			, (w2 & (1<< 2)) ? '1' : '0'
			, (w2 & (1<< 1)) ? '1' : '0'
			, (w2 & (1<< 0)) ? '1' : '0'
			);

		if (!(w2 & 4)) {
			if (false
				|| (w2 & 1)
			) {
				InsertPersistItem(tstr, h, TVI_LAST, pos);
			}
		}
	}

	Expand(h, TVE_EXPAND);

	if (iBHF & BHF_UPDATE) {
		BROWSEHIST bh;
		bh.iWhat = BE_DEV;
		m_hlForward.clear();
		m_hlBack.push_front(bh);
	}
}
