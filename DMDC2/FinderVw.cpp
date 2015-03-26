// FinderVw.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "FinderVw.h"
#include "FinderInputDlg.h"
#include "PM.h"
#include "Exp00069Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFinderVw

CFinderVw::CFinderVw()
{
	pEdCtx = NULL;
	isDblClk = false;
}

CFinderVw::~CFinderVw()
{
	SetDMEdCtx(NULL);
}


BEGIN_MESSAGE_MAP(CFinderVw, CListView)
	//{{AFX_MSG_MAP(CFinderVw)
	ON_COMMAND(ID_FINDER_FIND, OnFinderFind)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_COMMAND(ID_FINDER_ONECLICK, OnFinderOneclick)
	ON_UPDATE_COMMAND_UI(ID_FINDER_ONECLICK, OnUpdateFinderOneclick)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFinderVw 描画

void CFinderVw::OnDraw(CDC* pDC)
{

}

/////////////////////////////////////////////////////////////////////////////
// CFinderVw 診断

/////////////////////////////////////////////////////////////////////////////
// CFinderVw メッセージ ハンドラ

void CFinderVw::PostNcDestroy() 
{

}

BOOL CFinderVw::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CListView::PreCreateWindow(cs))
		return false;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style &=~(LVS_TYPEMASK);
	cs.style |= (LVS_LIST|LVS_SINGLESEL|LVS_SHOWSELALWAYS|LVS_ALIGNTOP);
	return true;
}

void CFinderVw::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	GetListCtrl().SetBkColor(RGB(0,0,0));
	GetListCtrl().SetTextColor(RGB(255,255,255));
	GetListCtrl().SetTextBkColor(RGB(0,0,0));

	GetListCtrl().DeleteAllItems();

	LOGFONT lfNice;
	ReadFontTemplateFrom(lfNice, IDS_FONTTEMP_FINDERVW, "Courier;10;;;;;;;;;;;;1");
	font.DeleteObject();
	font.CreateFontIndirect(&lfNice);

	SetFont(&font);
	
}

void CFinderVw::OnFinderFind() 
{
	if (pEdCtx == NULL)
		return;
	CFinderInputDlg wndDlg;
	if (wndDlg.DoModal() != IDOK)
		return;
	UINT iFD = wndDlg.m_iFindData;
	if ((iFD & 0xffff0000) != 0xfefe0000)
		return;
	BYTE db = (BYTE)(iFD >> 8);
	BYTE index = (BYTE)(iFD & 255);
	CDDAT *pDDIf = pEdCtx->GetDDIf();

	GetListCtrl().DeleteAllItems();

	std::set<RN> setFootnote;

	for (UINT tz=0; tz<(pDDIf->CntMap()); tz++) {
		Atlas_t at;
		HRESULT hr;
		if (FAILED(hr = pDDIf->GetMapAtlasAt(tz, at)))
			continue;
		for (UINT y=0; y<32; y++) {
			for (UINT x=0; x<32; x++) {
				if ((at.c[y][x] & 0x10) == 0)
					continue;
				RN rn = at.r[y][x];
				std::list<RN> listWalk;
				listWalk.push_back(rn);
				DMDBU rec;
				while (!listWalk.empty()) {
					rn = listWalk.front();
					if (setFootnote.find(rn) == setFootnote.end()) {
						setFootnote.insert(rn);
						for (; rn != RN_NIL && rn != RN_END; rn = rec.r0.next) {
							if (FAILED(hr = pDDIf->GetDBRecAt(DBIPos_t(rn), rec)))
								break;
							switch (rn.db()) {
							case 4: case 9:
								if (rec.r4.stolen != RN_END)
									listWalk.push_back(rec.r4.stolen);
								break;
							}
							if (rn.db() != db)
								continue;
							switch (db) {
							case 4: // MONS
								if (rec.r4.b4 != index)
									continue;
								break;
							case 5: // WEAP
							case 6: // CLOT
							case 10: // MISC
								if ((rec.r5.w2 & 0x7f) != index)
									continue;
								break;
							case 3: // ACTU
								if ((rec.r3.w2 & 0x7f) != index)
									continue;
								break;
							case 8: // POTI
								if (((rec.r8.w2 >> 8)&0x7f) != index)
									continue;
								break;
							case 9: // CHES
								if (CExp00069App::CHES2ID(rec.r9.w4) != index)
									continue;
								break;
							case 7: // SCRO
								break;
							default:
								ASSERT(false);
								break;
							}
							CString text; text.Format("%2d,(%2d,%2d)", 0U+tz, 0U+y, 0U+x);
							int r = GetListCtrl().InsertItem(INT_MAX, text);
							GetListCtrl().SetItemData(r, AtlasPos_t(tz, x, y).k.i);
							goto _EndFind;
						}
					}
					listWalk.pop_front();
				}
_EndFind:
				continue;
			}
		}
	}
}

void CFinderVw::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int r = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if (r >= 0) {
		AtlasPos_t ap;
		ap.k.i = GetListCtrl().GetItemData(r);

		SelectLvXY_t a;
		a.pos = ap;
		pEdCtx->DispatchDMUpdate(!isDblClk ? DPM_EDITTILE_LVXY : DPM_SELECTBTLVXY, &a);
	}
	
	*pResult = 0;
}

void CFinderVw::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (!isDblClk) {
		int r = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
		if (r >= 0) {
			AtlasPos_t ap;
			ap.k.i = GetListCtrl().GetItemData(r);

			SelectLvXY_t a;
			a.pos = ap;
			pEdCtx->DispatchDMUpdate(DPM_SELECTBTLVXY, &a);
		}
		
		*pResult = 0;
	}
	
	*pResult = 0;
}

void CFinderVw::OnFinderOneclick() 
{
	isDblClk = !isDblClk;
}

void CFinderVw::OnUpdateFinderOneclick(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(isDblClk);
}
