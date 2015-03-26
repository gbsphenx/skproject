// ShowcaseLv.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "ShowcaseLv.h"
#include "DM2NameDic.h"
#include "TsDataSource.h"
#include "CompatDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowcaseLv

CShowcaseLv::CShowcaseLv()
{
	pEdCtx = NULL;
	cDir = 0;

	fRebuildTree = true;
}

CShowcaseLv::~CShowcaseLv()
{
	SetDMEdCtx(NULL);
}

BEGIN_MESSAGE_MAP(CShowcaseLv, CTreeView)
	//{{AFX_MSG_MAP(CShowcaseLv)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBeginDrag)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_DIR_N, ID_DIR_W, OnDirN)
	ON_UPDATE_COMMAND_UI_RANGE(ID_DIR_N, ID_DIR_W, OnUpdateDirN)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowcaseLv 描画

void CShowcaseLv::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: この位置に描画用のコードを追加してください
}

/////////////////////////////////////////////////////////////////////////////
// CShowcaseLv 診断

#ifdef _DEBUG
void CShowcaseLv::AssertValid() const
{
	CTreeView::AssertValid();
}

void CShowcaseLv::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShowcaseLv メッセージ ハンドラ

#define LC GetTreeCtrl()

void CShowcaseLv::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
	LC.SetBkColor(RGB(0, 0, 0));
	LC.SetTextColor(RGB(255, 255, 255));
//	LC.SetTextBkColor(RGB(0, 0, 0));

	if (fRebuildTree) {
		fRebuildTree = false;

		LC.DeleteAllItems();

		cPicList.DeleteImageList();
		cPicList.Create(18, 18, ILC_COLOR24 | ILC_MASK, 0, 100);

		cPicList.Add(AfxGetApp()->LoadIcon(IDI_INVMARK));

		LC.SetImageList(&cPicList, TVSIL_NORMAL);

		struct Table {
			int t;
			LPCTSTR pszCase;
		} tbl[] = {
			-1,"Specials",
			0,"Items, creatures",
			NULL,
		};
		static const BYTE cntTy = 6;
		static const BYTE tytbl[cntTy] = {5,6,8,9,10,4};

		for (int t=0; t<2; t++) {
			HTREEITEM hiRoot = LC.InsertItem(tbl[t].pszCase);
			if (tbl[t].t == -1) {
				HTREEITEM hiShow;
				LC.SetItemData(hiShow = LC.InsertItem("Message panel", 0, 0, hiRoot), 0xFF00);
				LC.SetItemData(hiShow = LC.InsertItem("Scroll", 0, 0, hiRoot), 0xFF01);
			}
			else {
				CExtBM bm;
				bm.Create(CSize(18, 18), 24, NULL);
				CCompatDC dcMem;
				HGDIOBJ prev = dcMem.SelectObject(bm);
				CBrush br(RGB(64,64,64));

				for (int ty=0; ty<cntTy; ty++) {
					for (int x=0; x<128; x++) {
						CString strName = CDM2NameDic::GetInstance().GetName(tytbl[ty], x);
						if (strName.IsEmpty()) continue;

						dcMem.FillRect(CRect(0,0,18,18), &br);
						switch (tytbl[ty]) {
						case 4:
							GetApp()->RenderOrnate64To(&dcMem, 1, 1, 0x0F, x, 16);
							break;
						default:
							GetApp()->RenderItem16To(&dcMem, 1, 1, tytbl[ty], x);
							break;
						}
						dcMem.SelectObject(prev);
						int iPic = cPicList.Add(&bm, RGB(1,1,1));
						prev = dcMem.SelectObject(bm);

						HTREEITEM hiShow = LC.InsertItem(strName, iPic, iPic, hiRoot);
						LC.SetItemData(hiShow, 0xFEFE0000 | (tytbl[ty] << 8) | x);
					}
				}
				dcMem.SelectObject(prev);
				dcMem.DeleteDC();

				LC.SortChildren(hiRoot);
			}
		}
	}
}

void CShowcaseLv::OnDMUpdate(UINT nHint, void *pHint)
{

}

BOOL CShowcaseLv::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CTreeView::PreCreateWindow(cs))
		return false;

	cs.style |= 0
		|TVS_LINESATROOT
		|TVS_HASLINES
		|TVS_HASBUTTONS
		;
	return true;
}

void CShowcaseLv::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	///
	DWORD value = pNMTreeView->itemNew.lParam;
	if (value != 0) {
		CTsDataSource *ts = new CTsDataSource();
		ShowcaseData sc;
		sc.cDir = cDir;
		sc.t = value >> 8;
		sc.tid = value & 0xff;
		ts->CacheShowcase(sc);
		if (ts->DoDragDrop() == DROPEFFECT_NONE) {
			delete ts;
		}
	}
	///
	*pResult = 0;
}

void CShowcaseLv::OnDirN(UINT nID) 
{
	switch (nID) {
	case ID_DIR_N: cDir = 0; break;
	case ID_DIR_E: cDir = 1; break;
	case ID_DIR_S: cDir = 2; break;
	case ID_DIR_W: cDir = 3; break;
	}
}

void CShowcaseLv::OnUpdateDirN(CCmdUI* pCmdUI) 
{
	bool f = false;

	switch (pCmdUI->m_nID) {
	case ID_DIR_N: f = (cDir == 0); break;
	case ID_DIR_E: f = (cDir == 1); break;
	case ID_DIR_S: f = (cDir == 2); break;
	case ID_DIR_W: f = (cDir == 3); break;
	}

	pCmdUI->SetRadio(f);
}
