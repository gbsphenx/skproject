// EditVMSItemsPg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditVMSItemsPg.h"
#include "DM2RTextToken.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditVMSItemsPg ダイアログ


void CEditVMSItemsPg::Construction()
{
	//{{AFX_DATA_INIT(CEditVMSItemsPg)
	//}}AFX_DATA_INIT
}


void CEditVMSItemsPg::DoDataExchange(CDataExchange* pDX)
{
	CEditVPg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditVMSItemsPg)
	DDX_Control(pDX, IDC_LIST_SEL, m_wndSel);
	DDX_Control(pDX, IDC_LIST_ALL, m_wndAll);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
		Commit();
		m_strText = m_tl.ToStr();
	}
}


BEGIN_MESSAGE_MAP(CEditVMSItemsPg, CEditVPg)
	//{{AFX_MSG_MAP(CEditVMSItemsPg)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_SEL, OnGetdispinfoListSel)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_SEL, OnLvnBeginDragListSel)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_ALL, OnLvnBeginDragListAll)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_SEL, OnLvnKeyDownListSel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditVMSItemsPg メッセージ ハンドラ

void CEditVMSItemsPg::AddItems(int mask)
{
	LVITEM lvi;
	ZeroMemory(&lvi, sizeof(lvi));
	lvi.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE;
	lvi.pszText = LPSTR_TEXTCALLBACK;

	const DM2RTTable *ptOurs = m_fUseCatAll ? _RTTableWide : _RTTableCreature;

	{
		const DM2RTTable *pt = ptOurs;

		for (int c=0; pt->cls != 0; c++, pt++) {
			if (!(mask & (1 << c))) continue;
			const WORD x0 = pt->valFrom;
			const WORD x1 = pt->valTo;
			WORD x = x0;
			while (x <= x1) {
				if (m_fUseCatAll ? (!m_vctx.Get6CatItemName(pt->rtt, x -x0, m_strDispTemp)) : (!m_vctx.Get1CatItemName(pt->rtt, x -x0, m_strDispTemp))) {
					m_strDispTemp.Format("%s%02X", pt->pszClass, 0U +x -x0);
				}
				LPARAM lp = 0
					|(c << 12) // 12-15: pt[index]
					|(x -x0)   //  0-11: index
					|(x << 16) // 16-31: distinctive value
					;
				lvi.iItem = m_wndSel.GetItemCount();
				lvi.lParam = lp;
				lvi.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(m_strDispTemp));
				lvi.iImage = c;
				int r = m_wndAll.InsertItem(&lvi);
				ASSERT(x < 512);
				x++;
			}
		}
	}

	class Ut {
	public:
		static const DM2RTTable *Find(const DM2RTTable *pt, UINT v) {
			while (pt->cls != 0) {
				if (pt->valFrom <= v && v <= pt->valTo)
					return pt;
				++pt;
			}
			return NULL;
		}
	};

	{
		size_t t;
		for (t = 0; t < m_tl.sels.size(); t++) {
			UINT x = m_tl.sels[t];
			const DM2RTTable *pt = Ut::Find(ptOurs, x);
			if (pt != NULL) {
				UINT x0 = pt->valFrom;
				int c = pt -ptOurs;

				if (m_fUseCatAll ? (!m_vctx.Get6CatItemName(pt->rtt, x -x0, m_strDispTemp)) : (!m_vctx.Get1CatItemName(pt->rtt, x -x0, m_strDispTemp))) {
					m_strDispTemp.Format("%s%02X", pt->pszClass, 0U +x -x0);
				}
				LPARAM lp = 0
					|(c << 12) // 12-15: pt[index]
					|(x -x0)   //  0-11: index
					|(x << 16) // 16-31: distinctive value
					;
				lvi.iItem = m_wndSel.GetItemCount();
				lvi.lParam = lp;
				lvi.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(m_strDispTemp));
				lvi.iImage = c;
				int r = m_wndSel.InsertItem(&lvi);
			}
		}
	}
}

BOOL CEditVMSItemsPg::OnInitDialog() 
{
	CEditVPg::OnInitDialog();
	
	CaptureLayout(acrLeft | acrTop);
	SetAnchor(IDC_LIST_SEL, acrLeft | acrTop | acrRight | acrBottom);
	SetAnchor(IDC_LIST_ALL, acrTop | acrRight | acrBottom);
	SetAnchor(IDC_STATIC_HLP1, acrLeft | acrBottom);
	SetAnchor(IDC_STATIC_HLP2, acrLeft | acrBottom);
	SetAnchor(IDC_STATIC_HLP3, acrLeft | acrBottom);

	m_tl.forCreature = !m_fUseCatAll;
	m_tl.Parse(m_strInitText);

	m_il.DeleteImageList();

	VERIFY(m_il.Create(16, 16, ILC_COLOR4|ILC_MASK, 0, 7));

	if (m_fUseCatAll) {
		m_il.Add(AfxGetApp()->LoadIcon(IDI_GRPW));
		m_il.Add(AfxGetApp()->LoadIcon(IDI_GRPA));
		m_il.Add(AfxGetApp()->LoadIcon(IDI_GRPJ));
		m_il.Add(AfxGetApp()->LoadIcon(IDI_GRPP));
		m_il.Add(AfxGetApp()->LoadIcon(IDI_GRPC));
		m_il.Add(AfxGetApp()->LoadIcon(IDI_GRPS));
	}
	else {
		m_il.Add(AfxGetApp()->LoadIcon(IDI_GRPM));
	}

	m_wndSel.SetImageList(&m_il, LVSIL_SMALL);
	m_wndAll.SetImageList(&m_il, LVSIL_SMALL);

	AddItems();

	m_wndSel.InsertColumn(0, _T("Item"), LVCFMT_LEFT);
	m_wndSel.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_wndSel.SetExtendedStyle(m_wndSel.GetExtendedStyle() |LVS_EX_FULLROWSELECT);

	VERIFY(m_drop2sel.Register(&m_wndSel));

	m_wndAll.InsertColumn(0, _T("Item"), LVCFMT_LEFT);
	m_wndAll.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_wndAll.SetExtendedStyle(m_wndSel.GetExtendedStyle() |LVS_EX_FULLROWSELECT);

	VERIFY(m_drop2all.Register(&m_wndAll));

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

int CEditVMSItemsPg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditVPg::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CEditVMSItemsPg::OnSize(UINT nType, int cx, int cy) 
{
	CEditVPg::OnSize(nType, cx, cy);
	
	Resized();
	
}

void CEditVMSItemsPg::OnMove(int x, int y) 
{
	CEditVPg::OnMove(x, y);
	
	Moved();
	
}

void CEditVMSItemsPg::OnGetdispinfoListSel(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	*pResult = 0;
}

void CEditVMSItemsPg::Commit()
{
	m_tl.sels.clear();

	int x = 0, cx = m_wndSel.GetItemCount();

	for (; x < cx; x++) {
		DWORD val = m_wndSel.GetItemData(x) >> 16;
		m_tl.sels.push_back(val);
	}
}

void CEditVMSItemsPg::OnLvnBeginDragListSel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (m_wndSel.GetSelectedCount() != 0) {
		SourceMe src;
		src.SetText("sel");
		src.DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE|DROPEFFECT_SCROLL);
	}

	*pResult = 0;
}

void CEditVMSItemsPg::OnLvnBeginDragListAll(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (m_wndAll.GetSelectedCount() != 0) {
		SourceMe src;
		src.SetText("all");
		src.DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_SCROLL);
	}

	*pResult = 0;
}

void CEditVMSItemsPg::OnDropItems(CString strFrm, bool fCopy, int iInsertAt, DropHere &dropTo)
{
	if (iInsertAt < 0)
		iInsertAt = m_wndSel.GetItemCount();

	TCHAR tc[1000] = {0};
	LVITEM lvi;
	ZeroMemory(&lvi, sizeof(lvi));
	lvi.mask = LVIF_IMAGE | LVIF_TEXT | LVIF_PARAM;
	lvi.pszText = tc;
	lvi.cchTextMax = 1000;

	if (strFrm == "sel") {
		int pos = -1;

		while ((pos = m_wndSel.GetNextItem(pos, LVNI_SELECTED)) >= 0) {
			lvi.iItem = pos;

			if (m_wndSel.GetItem(&lvi)) {
				if (!fCopy) {
					m_wndSel.DeleteItem(pos);

					if (iInsertAt > pos)
						--iInsertAt;

					--pos;
				}

				if (&dropTo == &m_drop2sel) {
					lvi.iItem = iInsertAt;
					int at = m_wndSel.InsertItem(&lvi);
					if (at >= 0) {
						iInsertAt = at +1;

						if (pos >= at)
							++pos;
					}
				}
			}
			else {
				break;
			}
		}
	}
	else if (strFrm == "all") {
		int i = -1;

		while (true) {
			int pos = m_wndAll.GetNextItem(i, LVNI_SELECTED);
			if (pos < 0)
				break;

			lvi.iItem = pos;

			if (m_wndAll.GetItem(&lvi)) {
				if (&dropTo == &m_drop2sel) {
					lvi.iItem = iInsertAt;
					int at = m_wndSel.InsertItem(&lvi);
					if (at >= 0) {
						iInsertAt = at +1;
					}
				}
			}

			i = pos;
		}
	}
}

void CEditVMSItemsPg::OnLvnKeyDownListSel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDown = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	if (pLVKeyDown->wVKey == VK_DELETE) {
		int i;
		while ((i = m_wndSel.GetNextItem(-1, LVIS_SELECTED)) >= 0) {
			m_wndSel.DeleteItem(i);
		}
	}
	*pResult = 0;
}
