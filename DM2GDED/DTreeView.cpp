// DTreeView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DM2GDED.h"
#include "DTreeView.h"
#include "DM2GDEDDoc.h"
#include "AddImageDlg.h"
#include "AddImageDlg2.h"
#include "ReplC1Dlg.h"
#include "OSP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace OSP;

/////////////////////////////////////////////////////////////////////////////
// CDTreeView

CDTreeView::CDTreeView()
{

}

CDTreeView::~CDTreeView()
{
	InitEdCtx(NULL);
}

BEGIN_MESSAGE_MAP(CDTreeView, CTreeCtrl)
	//{{AFX_MSG_MAP(CDTreeView)
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnDeleteItem)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(ID_POPUP_ADD_C0, OnPopupAddC0)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(NM_RETURN, OnReturn)
	ON_WM_GETDLGCODE()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelChanged)
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_POPUP_REPLICATE_C1, OnPopupReplicateC1)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_POPUP_EXPORT_THIS, &CDTreeView::OnPopupExportThis)
	ON_COMMAND(ID_POPUP_DELETE_THIS, &CDTreeView::OnPopupDeleteThis)
	ON_COMMAND(ID_POPUP_IMPORT_FROM, &CDTreeView::OnPopupImportFrom)
	ON_COMMAND(ID_POPUP_COPY_THIS, &CDTreeView::OnPopupCopyThis)
	ON_COMMAND(ID_POPUP_PASTE_FROM, &CDTreeView::OnPopupPasteFrom)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(CDTreeView, CTreeCtrl)

/////////////////////////////////////////////////////////////////////////////
// CDTreeView メッセージ ハンドラ

void CDTreeView::OnGDUpdate(UINT nHint, void *pHint)
{
	switch (nHint) {
	case PM_V_RELOAD:
		OnReload();
		break;
	case PM_V_UPDATE_TREE_ITEM:
		{
			CGDEntry *pos = (CGDEntry *)pHint;
			HTREEITEM hItem = FindClass1(*pos, TRUE);
			if (hItem) {
				hItem = FindClass2(hItem, *pos, TRUE);
			}
			break;
		}
	case PM_V_SET_OPT:
		OnSetOpt();
		break;
	case PM_V_UPDATE_ENTRY:
		OnUpdateEntry(*(CGDEntry *)pHint);
		break;
	}
}

void CDTreeView::OnReload()
{
	DeleteAllItems();

	if (!m_pEdCtx) return;

	CGDAT &core = m_pEdCtx->GetGDAT();

	m_nGDRSLT = core.GetGDRSLT();

	HTREEITEM hLv0 = NULL, hLv1 = NULL;
	CGDEntry pos = core.GetHeadPos(), prior = CGDEntry::ERR;
	CString strText;
	while (pos != CGDEntry::ERR) {
		bool fForce = false;

		if (pos.c1() != prior.c1()) {
			strText.Format("%02X %s", 
				0U + pos.c1(),
				(LPCSTR)CGDEntry::GetFriendlyClass1Name(pos.c1())
				);

			fForce = true;
			hLv0 = InsertItem(strText);
			SetItemData(hLv0, new Class1(pos.c1()));
		}

		if (pos.c2() != prior.c2() || fForce) {
			strText.Format("%02X %s", 0U + pos.c2(), (LPCTSTR)FormatClass2Name(pos));

			fForce = true;
			hLv1 = InsertItem(strText, hLv0);
			SetItemData(hLv1, new Class2(pos.c1(), pos.c2()));
		}

		prior = pos;
		pos = core.GetNextPos(pos);
	}
}

void CDTreeView::OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	Root *pRoot = reinterpret_cast<Root *>(pNMTreeView->itemOld.lParam);
	if (pRoot) {
		delete pRoot;
	}
	
	*pResult = 0;
}

BOOL CDTreeView::SetItemData(HTREEITEM hItem, Root *pAny)
{
	return CTreeCtrl::SetItemData(hItem, (DWORD)pAny);
}

void CDTreeView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hItem = GetRClickItem();
	OnDblclkItem(hItem);

	*pResult = 0;
}

void CDTreeView::OnDblclkItem(HTREEITEM hItem)
{
	if (!m_pEdCtx) return;

	Class2 *p2;
	if (hItem && (p2 = GetItemAsClass2(hItem))) {
		TSelTreeItem more;
		more.lower = p2->lower;
		more.upper = p2->upper;

		m_pEdCtx->DispatchGDUpdate(PM_V_SEL_TREE_ITEM, &more);
	}
}

int CDTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(lf));
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	lf.lfHeight = MulDiv(lf.lfHeight, 3, 2);
	lf.lfCharSet = FIXED_PITCH;
	m_font.DeleteObject();
	m_font.CreateFontIndirect(&lf);

	SetFont(&m_font);
	
	return 0;
}

void CDTreeView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CPoint point;
	GetCursorPos(&point);

	SendMessage(WM_CONTEXTMENU, reinterpret_cast<WPARAM>(GetSafeHwnd()), MAKELONG(point.x, point.y));
	
	*pResult = 0;
}

void CDTreeView::OnPopupAddC0() 
{
	CGDEntry pos(0,0,0,0,0,0);
	{
		HTREEITEM hItem = GetNextItem(NULL, TVGN_CARET);
		Class1 *p1 = GetItemAsClass1(hItem);
		if (p1) {
			pos = p1->lower;
		} else {
			Class2 *p2 = GetItemAsClass2(hItem);
			if (p2) {
				pos = p2->lower;
			}
		}
	}

	CAddImageDlg2 wndDlg(m_pEdCtx->GetVxxxFactory().CreateVxxx(-1), this);
	wndDlg.InitAddClass2(pos);
	int r = wndDlg.DoModal();
	if (r != IDOK)
		return;
	pos = wndDlg.GetEntryPos();

	HTREEITEM hItem = FindClass1(pos, TRUE);
	if (hItem) {
		hItem = FindClass2(hItem, pos, TRUE);
		if (hItem) {
			Select(hItem, TVGN_CARET);
		}
	}
}

HTREEITEM CDTreeView::FindClass1(const CGDEntry &pos, BOOL fAdd)
{
	HTREEITEM hParent = NULL;
	HTREEITEM hItem = GetRootItem();
	HTREEITEM hAfter = TVI_FIRST;
	while (hItem) {
		Class1 *p1 = GetItemAsClass1(hItem);
		if (p1) {
			if (p1->lower.c1() == pos.c1()) {
				return hItem;
			}
			if (pos.c1() < p1->lower.c1())
				break;
		}

		hAfter = hItem;
		hItem = GetNextSiblingItem(hItem);
	}
	if (!fAdd)
		return NULL;
	CString strText;
	strText.Format("%02X %s", 
		0U + pos.c1(),
		(LPCSTR)CGDEntry::GetFriendlyClass1Name(pos.c1())
		);
	HTREEITEM hNew = InsertItem(strText, hParent, hAfter);
	SetItemData(hNew, new Class1(pos.c1())); 
	return hNew;
}

HTREEITEM CDTreeView::FindClass2(HTREEITEM hParent, const CGDEntry &pos, BOOL fAdd)
{
	HTREEITEM hItem = GetChildItem(hParent);
	HTREEITEM hAfter = TVI_FIRST;
	while (hItem) {
		Class2 *p2 = GetItemAsClass2(hItem);
		if (p2) {
			if (p2->lower.c2() == pos.c2()) {
				return hItem;
			}
			if (pos.c2() < p2->lower.c2())
				break;
		}

		hAfter = hItem;
		hItem = GetNextSiblingItem(hItem);
	}
	if (!fAdd)
		return NULL;
	CString strText;
	strText.Format("%02X", 0U + pos.c2());
	HTREEITEM hNew = InsertItem(strText, hParent, hAfter);
	SetItemData(hNew, new Class2(pos.c1(), pos.c2())); 
	return hNew;
}

void CDTreeView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (point.x < 0) {
		DWORD pos = GetMessagePos();
		point.x = GET_X_LPARAM(pos);
		point.y = GET_Y_LPARAM(pos);
		//ClientToScreen(&point);

		ShowContextMenu(point, GetSelectedItem());
	}
	else {
		ShowContextMenu(point, GetRClickItem());
	}
}

void CDTreeView::ShowContextMenu(CPoint point, HTREEITEM hItem)
{
	CMenu m;
	m.LoadMenu(IDR_TREECLS);
	CMenu *p = m.GetSubMenu((GetItemAsClass2(m_hItemRClk = hItem) != NULL) ? 1 : 0);
	p->TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CDTreeView::OnReturn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	GetParentFrame()->OnCmdMsg(ID_NEXT_PANE, 0, NULL, NULL);

//	GetParentFrame()->OnCmdMsg(ID_CTRL_GRIDVIEW, 0, NULL, NULL);

//	HTREEITEM hItem = GetRClickItem();
//	OnDblclkItem(hItem);
//	
	*pResult = TRUE;
}

UINT CDTreeView::OnGetDlgCode() 
{
	return CTreeCtrl::OnGetDlgCode() | DLGC_WANTALLKEYS;
}

void CDTreeView::OnSetOpt()
{
	if (!m_pEdCtx) return;

	CGDEdOpt aOptNew;
	m_pEdCtx->GetOpts(aOptNew);

	if (false
		|| m_aOpt.m_inlang != aOptNew.m_inlang
		|| m_aOpt.m_cnlang != aOptNew.m_cnlang
	) {
		m_ilspCnlang = CILSp::GetCnlang(aOptNew.m_cnlang);
		m_ilspInlang = CILSp::GetInlang(aOptNew.m_inlang);

		m_aOpt.m_inlang = aOptNew.m_inlang;
		m_aOpt.m_cnlang = aOptNew.m_cnlang;

		CWaitCursor wc;
		OnUpdateTreeItemName();
	}
}

void CDTreeView::OnUpdateTreeItemName()
{
	CString strText;

	HTREEITEM hLv1 = GetRootItem();
	while (hLv1) {
		HTREEITEM hLv2 = GetChildItem(hLv1);
		while (hLv2) {
			//
			Class2 *p2 = GetItemAsClass2(hLv2);
			if (p2) {
				CGDEntry pos = p2->lower;
				strText.Format("%02X %s", 0U + pos.c2(), (LPCTSTR)FormatClass2Name(pos));

				SetItemText(hLv2, strText);
			}
			//
			hLv2 = GetNextSiblingItem(hLv2);
		}
		hLv1 = GetNextSiblingItem(hLv1);
	}
}

void CDTreeView::OnUpdateEntry(const CGDEntry &pos)
{
	HTREEITEM hItem = FindClass1(pos, TRUE);
	if (hItem) {
		CGDAT &core = m_pEdCtx->GetGDAT();
		CGDEntry posLo = core.GetLowerPos(CGDEntry(pos.c1(), pos.c2(), 0, 0, 0, 0));
		if (posLo.c1() == pos.c1() && posLo.c2() == pos.c2()) {
			hItem = FindClass2(hItem, pos, TRUE);
			if (hItem) {
				Class2 *p2 = GetItemAsClass2(hItem);
				if (p2) {
					CString strText;
					CGDEntry pos = p2->lower;
					strText.Format("%02X %s", 0U + pos.c2(), (LPCTSTR)FormatClass2Name(pos));

					SetItemText(hItem, strText);
				}
			}
		}
		else {
			hItem = FindClass2(hItem, pos, FALSE);
			if (hItem) {
				DeleteItem(hItem);
			}
		}
	}
}

void CDTreeView::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	OnDblclkItem(hItem);
	
	*pResult = 0;
}

void CDTreeView::OnSetFocus(CWnd* pOldWnd) 
{
	CTreeCtrl::OnSetFocus(pOldWnd);
	
	CCmdTarget *p;
	if (p = GetParentFrame()) {
		p->OnCmdMsg(ID_UPDATE_CURRENT_PANE, 0, NULL, NULL);
	}
	
}

void CDTreeView::OnPopupReplicateC1() 
{
	Class2 *p2 = GetItemAsClass2(m_hItemRClk);
	if (p2 != NULL) {
		CGDEntry pos = p2->lower;

		CReplC1Dlg wndDlg;
		wndDlg.m_to = wndDlg.m_from = CGDEntry(pos.c1(), pos.c2(), 0, 0, 0, 0);

		CGDAT &core = m_pEdCtx->GetGDAT();

		int maxy = 0;
		for (int y=0xFD; y>0; y--) {
			if (core.CountEnts(CGDEntry(pos.c1(), y, 0,0,0,0), CGDEntry(pos.c1(), y, 0xFF,0xFF,0xFF,0xFF)) != 0)
				maxy = max(maxy, y);
		}
		if (maxy != 0)
			wndDlg.m_to.c2(maxy +1);

		if (wndDlg.DoModal() == IDOK) {
			core.ReplicateSubclass(wndDlg.m_from, wndDlg.m_to);
		}
	}
}

void CDTreeView::OnPopupDeleteThis()
{
	HTREEITEM hItem;
	Class2 *p2 = GetItemAsClass2(hItem = m_hItemRClk);
	if (p2 != NULL) {
		if (AfxMessageBox(IDS_WARN_DELETE_THIS, MB_OKCANCEL | MB_ICONEXCLAMATION) != IDOK)
			return;

		CGDEntry pos = p2->lower;

		CGDAT &core = m_pEdCtx->GetGDAT();

		core.DeleteSubclass(pos.c1(), pos.c2());
	}
}

HTREEITEM CDTreeView::GetRClickItem() {
	DWORD pos = GetMessagePos();
	CPoint pt(GET_X_LPARAM(pos), GET_Y_LPARAM(pos));
	ScreenToClient(&pt);
	UINT uFlags = 0;
	return HitTest(pt, &uFlags);
}

void CDTreeView::OnPopupExportThis()
{
	HTREEITEM hItem;
	Class2 *p2 = GetItemAsClass2(hItem = m_hItemRClk);
	if (p2 == NULL)
		return;

	CGDEntry pos = p2->lower;

	CGDAT &core = m_pEdCtx->GetGDAT();

	CGDAT new1;

	CString strText;
	strText.Format("%s %02X %s"
		, (LPCSTR)CGDEntry::GetFriendlyClass1Name(pos.c1())
		, 0U + pos.c2()
		, (LPCTSTR)FormatClass2Name(pos)
		);
	strText.Trim();
	strText += _T(".dat");

	CFileDialog wndDlg(false, _T("dat"), strText, OFN_ENABLESIZING|OFN_EXPLORER|OFN_LONGNAMES|OFN_PATHMUSTEXIST, _T("*.dat|*.dat||"));
	if (wndDlg.DoModal() != IDOK)
		return;

	if (new1.Copyfrm(core, p2->lower, p2->upper, true)) {
		if (new1.SaveTo(wndDlg.GetPathName())) {
			AfxMessageBox(IDS_STR_EXPORT_DONE, MB_ICONINFORMATION);
		}
		else AfxMessageBox(IDS_ERR_EXPORT, MB_ICONEXCLAMATION);
	}
	else AfxMessageBox(IDS_ERR_EXPORT, MB_ICONEXCLAMATION);
}

void CDTreeView::OnPopupImportFrom()
{
	CFileDialog wndDlg(true, _T("dat"), NULL, OFN_ENABLESIZING|OFN_EXPLORER|OFN_LONGNAMES|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST, _T("*.dat|*.dat||"));
	if (wndDlg.DoModal() != IDOK)
		return;

	CGDAT &core = m_pEdCtx->GetGDAT();

	CGDAT new1;
	if (new1.LoadFrom(wndDlg.GetPathName(), core.GetGDOF())) {
		CGDEntry first = new1.GetLowerPos(CGDEntry::Min);
		if (core.Copyfrm(new1, CGDEntry::Min, CGDEntry::Max, false)) {
			HTREEITEM hItem = FindClass1(first, false);
			if (hItem) {
				hItem = FindClass2(hItem, first, false);
				if (hItem) {
					Select(hItem, TVGN_CARET);
				}
			}

			AfxMessageBox(IDS_STR_IMPORT_DONE, MB_ICONINFORMATION);
		}
		else AfxMessageBox(IDS_ERR_IMPORT, MB_ICONEXCLAMATION);
	}
	else AfxMessageBox(IDS_ERR_IMPORT, MB_ICONEXCLAMATION);
}

static const UINT CF_GDAT = RegisterClipboardFormat(_T("GRAPHICS_DAT"));

class CGMUt {
public:
	static HGLOBAL LoadFrom(LPCTSTR psz) {
		CFile f;
		if (f.Open(psz, CFile::modeRead)) {
			DWORD cb = (DWORD)min(1024*1024UL, f.GetLength());
			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, cb);
			if (hMem != NULL) {
				PVOID pv = GlobalLock(hMem);
				if (pv != NULL) {
					if (f.Read(pv, cb) == cb) {
						GlobalUnlock(hMem);
						return hMem;
					}
					GlobalUnlock(hMem);
				}
				GlobalFree(hMem);
			}
		}
		return NULL;
	}

	static bool SaveTo(LPCTSTR psz, HGLOBAL hMem) {
		CFile f;
		if (f.Open(psz, CFile::modeCreate|CFile::modeReadWrite)) {
			DWORD cb = GlobalSize(hMem);
			PVOID pv = GlobalLock(hMem);
			if (pv != NULL) {
				f.Write(pv, cb);
				f.SetLength(f.GetPosition());
				GlobalUnlock(pv);
				return true;
			}
		}
		return false;
	}
};

void CDTreeView::OnPopupCopyThis()
{
	HTREEITEM hItem;
	Class2 *p2 = GetItemAsClass2(hItem = m_hItemRClk);
	if (p2 == NULL)
		return;

	CGDEntry pos = p2->lower;

	CGDAT &core = m_pEdCtx->GetGDAT();

	CGDAT new1;

	CString fp = OSP_GetTempFile(_T(".dat"), _T("dun"));

	if (new1.Copyfrm(core, p2->lower, p2->upper, true)) {
		if (new1.SaveTo(fp)) {
			if (OpenClipboard()) {
				HGLOBAL hMem = CGMUt::LoadFrom(fp);
				SetClipboardData(CF_GDAT, hMem);
				CloseClipboard();
			}
			else AfxMessageBox(IDS_ERR_COPY_DAT, MB_ICONEXCLAMATION);
		}
		else AfxMessageBox(IDS_ERR_COPY_DAT, MB_ICONEXCLAMATION);
	}
	else AfxMessageBox(IDS_ERR_COPY_DAT, MB_ICONEXCLAMATION);

	DeleteFile(fp);
}

void CDTreeView::OnPopupPasteFrom()
{
	CString fp = OSP_GetTempFile(_T(".dat"), _T("dun"));

	if (OpenClipboard()) {
		HANDLE hMem = GetClipboardData(CF_GDAT);
		if (CGMUt::SaveTo(fp, hMem)) {

			CGDAT &core = m_pEdCtx->GetGDAT();

			CGDAT new1;
			if (new1.LoadFrom(fp, core.GetGDOF())) {
				CGDEntry first = new1.GetLowerPos(CGDEntry::Min);
				if (core.Copyfrm(new1, CGDEntry::Min, CGDEntry::Max, false)) {
					HTREEITEM hItem = FindClass1(first, false);
					if (hItem) {
						hItem = FindClass2(hItem, first, false);
						if (hItem) {
							Select(hItem, TVGN_CARET);
						}
					}
				}
				else AfxMessageBox(IDS_ERR_PASTE_DAT, MB_ICONEXCLAMATION);
			}
			else AfxMessageBox(IDS_ERR_PASTE_DAT, MB_ICONEXCLAMATION);

		}
		CloseClipboard();
	}

	DeleteFile(fp);
}
