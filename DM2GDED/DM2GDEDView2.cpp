// DM2GDEDView2.cpp : CDM2GDEDView2 クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "DM2GDED.h"

#include "DM2GDEDDoc.h"
#include "DM2GDEDView2.h"
#include "CompatDC.h"
#include "EditImgDlg.h"
#include "AddImageDlg.h"
#include "HexEditFrame.h"

#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDW_GRID 0x0100

namespace
{

CGDEntry g_kSmily(21,0xfe,0x01,0xfe,0x00,0x0);

CHexEditFrame *g_pHexEditWnd = NULL;

};

/////////////////////////////////////////////////////////////////////////////

void OnDeleteHexEditFrame(CHexEditFrame *pWnd)
{
	if (g_pHexEditWnd == pWnd)
		g_pHexEditWnd = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDView2::HexEditCtx

void CDM2GDEDView2::HexEditCtx::OnHexEditApply(CHexEditCB *pSender)
{
	if (pCtx && g_pHexEditWnd && g_pHexEditWnd->GetHexEditCB() == pSender) {
		SizeBuff fv;
		pSender->GetRawData(fv);
		pCtx->GetGDAT().SetRawDataFor(pos, fv);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEntryItemCell

class CEntryItemCell : public CGridCell
{
	// 
	CGDEntry m_pos;
	// 
	DECLARE_DYNCREATE(CEntryItemCell)

public:
	// 
	CEntryItemCell(): CGridCell()
	{
		m_pos = CGDEntry::ERR;
	}
	// 
	void SetPos(const CGDEntry &pos) { m_pos = pos; }
	// 
	const CGDEntry &GetPos() { return m_pos; }
};

IMPLEMENT_DYNCREATE(CEntryItemCell, CGridCell);

/////////////////////////////////////////////////////////////////////////////
// CPortlaitCell

class CPortlaitCell : public CGridCell {
	// 
	CBitmap m_bm;
	// 
	CGDEntry m_pos;
	// 
	DECLARE_DYNCREATE(CPortlaitCell)

public:
	// 
	void Init(HBITMAP hBM, CGDEntry pos)
	{
		m_bm.DeleteObject();
		m_bm.Attach(hBM);
		m_pos = pos;
	}
	// 
	CGDEntry GetEntryPos() { return m_pos; }

protected:
	// 
	virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd)
	{
		if (!CGridCell::Draw(pDC, nRow, nCol, rect, bEraseBkgnd))
			return FALSE;
		BITMAP bm;
		if (m_bm.m_hObject && m_bm.GetBitmap(&bm)) {
			pDC->SaveDC();
			HPALETTE hPal = GetApp()->GetDM2Pal();
			pDC->SelectPalette(CPalette::FromHandle(hPal), FALSE);
			CDC dc;
			dc.CreateCompatibleDC(NULL);
			dc.SelectObject(&m_bm);
			dc.SelectPalette(CPalette::FromHandle(hPal), FALSE);
			rect.DeflateRect(1, 1, 0, 1);
			pDC->IntersectClipRect(&rect);
			pDC->BitBlt(
				rect.left, 
				rect.top,
				bm.bmWidth,
				bm.bmHeight,
				&dc,
				0,
				0,
				SRCCOPY
				);
			pDC->RestoreDC(-1);
		}
		return TRUE;
	}
};

IMPLEMENT_DYNCREATE(CPortlaitCell, CGridCell);

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDView2

IMPLEMENT_DYNCREATE(CDM2GDEDView2, CView)

BEGIN_MESSAGE_MAP(CDM2GDEDView2, CView)
	//{{AFX_MSG_MAP(CDM2GDEDView2)
	ON_WM_SIZE()
	ON_COMMAND(ID_POPUP_EDIT_IMAGE, OnPopupEditImage)
	ON_COMMAND(ID_POPUP_ADD_IMAGE, OnPopupAddImage)
	ON_COMMAND(ID_POPUP_DELETE_ENTRY, OnPopupDeleteEntry)
	ON_COMMAND(ID_POPUP_ADD_TEXT, OnPopupAddText)
	ON_COMMAND(ID_POPUP_ADD_0B, OnPopupAdd0B)
	ON_COMMAND(ID_POPUP_ADD_0C, OnPopupAdd0C)
	ON_COMMAND(ID_POPUP_EDIT_TEXT, OnPopupEditText)
	ON_COMMAND(ID_POPUP_EDIT_0B, OnPopupEdit0B)
	ON_COMMAND(ID_POPUP_EDIT_0C, OnPopupEdit0C)
	ON_COMMAND(ID_POPUP_ADD_PCM, OnPopupAddPcm)
	ON_COMMAND(ID_POPUP_EDIT_PCM, OnPopupEditPcm)
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_NEXT_PANE, OnUpdateNextPane)
	ON_UPDATE_COMMAND_UI(ID_PREV_PANE, OnUpdatePrevPane)
	ON_WM_CONTEXTMENU()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_POPUP_EDIT_RAW, OnPopupEditRaw)
	ON_COMMAND(ID_POPUP_ADD_04, OnPopupAdd04)
	ON_COMMAND(ID_POPUP_ADD_07, OnPopupAdd07)
	ON_COMMAND(ID_POPUP_ADD_09, OnPopupAdd09)
	ON_COMMAND(ID_POPUP_RENAME_ENTRY, OnPopupRenameEntry)
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDW_GRID, OnDblClk)
	ON_NOTIFY(NM_RCLICK, IDW_GRID, OnRclick)
	ON_NOTIFY(GVN_KEYDOWN, IDW_GRID, OnGridKeydown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDView2 クラスの構築/消滅

CDM2GDEDView2::CDM2GDEDView2()
{
	m_pEdCtx = NULL;
}

CDM2GDEDView2::~CDM2GDEDView2()
{
	ASSERT(m_pEdCtx == NULL);
}

BOOL CDM2GDEDView2::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDView2 クラスの描画

void CDM2GDEDView2::OnDraw(CDC* pDC)
{

}

void CDM2GDEDView2::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	if (g_pHexEditWnd)
		g_pHexEditWnd->DestroyWindow();

	if (m_wndGrid.m_hWnd == NULL) {
		m_wndGrid.DestroyWindow();
		m_wndGrid.Create(CRect(0, 0, 0, 0), this, IDW_GRID);
		m_wndGrid.EnableSelection(FALSE);
	}

	m_wndGrid.SetColumnCount(3);
	m_wndGrid.SetRowCount(1);
	m_wndGrid.SetRowCount(2);
	m_wndGrid.SetFixedRowCount(1);

	m_wndGrid.SetItemText(0, 0, "Name");
	m_wndGrid.SetItemText(0, 1, "Type");
	m_wndGrid.SetItemText(0, 2, "Value");
	m_wndGrid.SetCellType(1, 0, RUNTIME_CLASS(CEntryItemCell));

	m_wndGrid.ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	m_wndGrid.SetFocusCell(1, 0);

	m_lower = CGDEntry(0,0,0,0,0,0);

	m_aHexEdCtx = HexEditCtx();

	InitEdCtx(GetDocument()->GetEdCtx(FALSE));

}

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDView2 クラスの診断

#ifdef _DEBUG
void CDM2GDEDView2::AssertValid() const
{
	CView::AssertValid();
}

void CDM2GDEDView2::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDM2GDEDDoc* CDM2GDEDView2::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDM2GDEDDoc)));
	return (CDM2GDEDDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDView2 クラスのメッセージ ハンドラ

void CDM2GDEDView2::OnGDUpdate(UINT nHint, void *pHint)
{
	if (!m_pEdCtx) return;

	switch (nHint) {
	case PM_V_RELOAD:
		break;
	case PM_V_SEL_TREE_ITEM:
		OnSelTreeItem(*(TSelTreeItem *)pHint);
		break;
	case PM_V_SET_OPT:
		OnSetOpt();
		break;
	}
}

void CDM2GDEDView2::UpdateCell(int iRow, CGDAT &core, const CGDEntry *ppos, SIZE &sizeCell, BOOL fUpdate)
{
	CGDEntry pos;
	if (ppos == NULL) {
		CEntryItemCell *pCell = static_cast<CEntryItemCell *>(m_wndGrid.GetCell(iRow, 0));
		ppos = &(pos = pCell->GetPos());

		if (sizeCell.cx == 0) {
			sizeCell.cx = m_wndGrid.GetColumnWidth(2);
			sizeCell.cy = m_wndGrid.GetRowHeight(0);
		}
	}
	CString strText;
	UINT iIdx = core.GetValueAt(*ppos);

	switch (ppos->c3()) {
	case 0x01:
		{
			m_wndGrid.SetCellType(iRow, 2, RUNTIME_CLASS(CPortlaitCell));
			CPortlaitCell *pCell = static_cast<CPortlaitCell *>(m_wndGrid.GetCell(iRow, 2));
			if (pCell) {
				BITMAP bm;
				HBITMAP hBM = core.GetImgAt(iIdx);
				pCell->Init(hBM, *ppos);
				if (GetObject(hBM, sizeof(BITMAP), &bm) == sizeof(BITMAP)) {
					m_wndGrid.SetRowHeight(iRow, __max(sizeCell.cy, bm.bmHeight + 2));
					sizeCell.cx = __max(sizeCell.cx, bm.bmWidth + 2);
				}
			}
			break;
		}
	case 0x02:
		{
			strText.Format(core.IsSnd56() ? "%u ..." : "%u", iIdx);
			m_wndGrid.SetItemText(iRow, 2, strText);
			break;
		}
	case 0x05:
		{
			if (core.GetTextAt(iIdx, strText)) {
				strText.Replace("\n", "\r\n");
				m_wndGrid.SetItemText(iRow, 2, strText);
			}
			break;
		}
	case 0x0B:
	case 0x0C:
		{
			if (m_aEdOpt.m_fShowHex) {
				strText.Format("%02X %02X"
					, 0U+HIBYTE(iIdx)
					, 0U+LOBYTE(iIdx)
					);
			} else {
				strText.Format("%u"
					, iIdx
					);
			}
			m_wndGrid.SetItemText(iRow, 2, strText);
			break;
		}
	case 0x04:
	case 0x06:
	case 0x07:
	case 0x08:
	case 0x09:
	case 0x0E:
		{
			strText.Format("%u ...", iIdx);
			m_wndGrid.SetItemText(iRow, 2, strText);
			break;
		}
	}
	if (fUpdate) {
		m_wndGrid.Invalidate();
	}
}

void CDM2GDEDView2::OnSelTreeItem(TSelTreeItem &rItem)
{
	if (!m_pEdCtx) return;

	CGDAT &core = m_pEdCtx->GetGDAT();

	list<CGDEntry> listEntry;

	m_lower = rItem.lower;

	core.Enum(listEntry, rItem.lower, rItem.upper);

	CSize sizeCell(
		m_wndGrid.GetColumnWidth(2),
		m_wndGrid.GetRowHeight(0)
		);

	m_wndGrid.SetRowCount(1);
	m_wndGrid.SetRowCount(1 + __max(1, listEntry.size()));

	m_wndGrid.SetCellType(1, 0, RUNTIME_CLASS(CEntryItemCell));

	CString strName, strText;
	int nItems = 1;
	while (!listEntry.empty()) {
		const CGDEntry &pos = listEntry.front();

		int iItem = nItems;

		m_wndGrid.SetCellType(iItem, 0, RUNTIME_CLASS(CEntryItemCell));

		CEntryItemCell *pCell = (CEntryItemCell *)STATIC_DOWNCAST(CEntryItemCell, m_wndGrid.GetCell(iItem, 0));

		pCell->SetPos(pos);

		m_wndGrid.SetItemState(iItem, 0, GVIS_READONLY);
		m_wndGrid.SetItemState(iItem, 1, GVIS_READONLY);
		m_wndGrid.SetItemState(iItem, 2, GVIS_READONLY);

		strName.Format("%02X %02X %02X"
			, 0U + pos.c4()
			, 0U + pos.c5()
			, 0U + pos.c6()
			);
		m_wndGrid.SetItemText(iItem, 0, strName);
		m_wndGrid.SetItemText(iItem, 1, pos.GetFriendlyTypeName(pos.c3()));

		UpdateCell(iItem, core, &pos, sizeCell, FALSE);

		nItems++;

		listEntry.pop_front();
	}

	m_wndGrid.SetColumnWidth(2, sizeCell.cx);

	m_wndGrid.SetFocusCell(1, 0);
}

void CDM2GDEDView2::PostNcDestroy() 
{
	InitEdCtx(NULL);
	
	CView::PostNcDestroy();
}

void CDM2GDEDView2::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	RepositionBars(0, 0xffff, IDW_GRID);
	
}

void CDM2GDEDView2::OnDblClkItem(int iCol, int iRow)
{
	CGDAT &core = m_pEdCtx->GetGDAT();

	if (iCol == 2) {
		CEntryItemCell *pCell = (CEntryItemCell *)DYNAMIC_DOWNCAST(CEntryItemCell, m_wndGrid.GetCell(iRow, 0));
		if (pCell) {
			CGDEntry pos = pCell->GetPos();
			switch (pos.c3()) {
			case 0x02:
				{
					if (core.IsSnd56()) {
						UINT iIdx = core.GetValueAt(pos);
						if (core.GetRawDataAt(iIdx, m_fvWav) && CreateRIFFWave(m_fvPlaySound)) {
							PlaySound(
								(LPCSTR)m_fvPlaySound.GetData(),
								NULL,
								SND_ASYNC|SND_MEMORY
								);
						}
					}
				}
			}
		}
	}
}

void CDM2GDEDView2::OnDblClk(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GRIDVIEW *pNM = (NM_GRIDVIEW *)pNMHDR;
	
	OnDblClkItem(pNM->iColumn, pNM->iRow);
}

bool CDM2GDEDView2::CreateWav(SizeBuff &fvRaw, BOOL fLE)
{
	const BYTE *pbRaw = (const BYTE *)m_fvWav.GetData();
	if (m_fvWav.GetSize() < 4)
		return false;
	if (fLE)
	{
		if (pbRaw[0] != 0x77 || pbRaw[1] != 0x2B || pbRaw[2] != 0x08 || pbRaw[3] != 0x01)
			return false;
	}
	else
	{
		if (pbRaw[0] != 0x2B || pbRaw[1] != 0x77 || pbRaw[2] != 0x08 || pbRaw[3] != 0x01)
			return false;
	}
	UI32 nWavLen = m_fvWav.GetSize() - 4;
	UI32 nMaxLength = (8+4)+(8+18)+(8+4)+(8+nWavLen);
	if (!fvRaw.Alloc(nMaxLength))
		return false;
	MemDestIo io((BYTE *)fvRaw.GetData(), (BYTE *)fvRaw.GetData() + fvRaw.GetSize());
	io.WriteDWord(0x46464952U); // "RIFF"
	io.WriteDWord(nMaxLength - 8);
	io.WriteDWord(0x45564157U); // "WAVE"
	io.WriteDWord(0x20746D66U); // "fmt "
	io.WriteDWord(18); // sizeof(WAVEFORMATEX)
	io.WriteWord (1); // WAVEFORMATEX.wFormatTag
	io.WriteWord (1); // WAVEFORMATEX.nChannels
	io.WriteDWord(11025); // WAVEFORMATEX.nSamplesPerSec
	io.WriteDWord(11025); // WAVEFORMATEX.nAvgBytesPerSec
	io.WriteWord (1); // WAVEFORMATEX.nBlockAlign
	io.WriteWord (8); // WAVEFORMATEX.wBitsPerSample
	io.WriteWord (0); // WAVEFORMATEX.cbSize
	io.WriteDWord(0x74636166U); // "fact"
	io.WriteDWord(4);
	io.WriteDWord(nWavLen); // total sample count
	io.WriteDWord(0x61746164U); // "data"
	io.WriteDWord(nWavLen);
	for (UI32 iSample = 0, nSamples = nWavLen; iSample < nSamples; iSample++)
	{
		BYTE x = pbRaw[4 + iSample];
		x = (x + 0x80);
		io.WriteByte(x);
	}
	return true;
}

void CDM2GDEDView2::OnRclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW *pNM = (NM_GRIDVIEW *)pNMHDR;
	
	CPoint point;
	GetCursorPos(&point);
	OnRclickItem(pNM->iColumn, pNM->iRow, point);
}

void CDM2GDEDView2::OnPopupEditImage() 
{
	if (!m_pEdCtx) return;

	CGDAT &core = m_pEdCtx->GetGDAT();

	int iRow = m_rclkSel.row;
	CGridCellBase *pAny = m_wndGrid.GetCell(iRow, m_rclkSel.col);
	CPortlaitCell *pCell = (CPortlaitCell *)DYNAMIC_DOWNCAST(CPortlaitCell, pAny);
	if (pCell) {
		CEditImgDlg wndDlg(core.IsClassic(), core.IsLE(), this);
		I32 iIdx = core.GetValueAt(pCell->GetEntryPos());
		{
			DMGHLiIMG4 img4;
			HBITMAP hBM;
			// EDITIMG
			if (hBM = core.GetImg4At(iIdx, img4)) {
				wndDlg.Init(hBM, img4);
			} else {
				DMGHLiIMG8 img8;
				if (hBM = core.GetImg8At(iIdx, img8)) {
					wndDlg.Init(hBM, img8);
				} else {
					wndDlg.Init();
				}
			}
		}
		int r = wndDlg.DoModal();
		if (r != IDOK)
			return;
		VERIFY(core.SetRawDataFor(pCell->GetEntryPos(), wndDlg.GetNew1()));

		UpdateCell(iRow, m_pEdCtx->GetGDAT(), NULL, CSize(0, 0), TRUE);
		m_wndGrid.Invalidate();
	}
}

void CDM2GDEDView2::OnPopupAddImage() 
{
	OnAddCtx(0x01);
}

void CDM2GDEDView2::UpdateAddEntry(const CGDEntry &pos)
{
	UINT iPos = 1;
	UINT iItem, nItems = m_wndGrid.GetRowCount();
	for (iItem = 1; iItem < nItems; iItem++) {
		CEntryItemCell *pCell = GetEntryItemCellAt(iItem);
		ASSERT(pCell);
		if (!pCell)
			return;
		CGDEntry posCell = pCell->GetPos();
		if (posCell == CGDEntry::ERR) {
			m_wndGrid.DeleteRow(iItem);
			nItems--;
			break;
		}
		if (pos < posCell) {
			break;
		}
	}
	int iRow = m_wndGrid.InsertRow("", iItem);
	if (iRow < 0) {
		if (iItem != nItems)
			return;
		iRow = nItems;
		m_wndGrid.SetRowCount(nItems + 1);
	}
	SetupCell(iRow, m_pEdCtx->GetGDAT(), pos);
	m_wndGrid.SetFocusCell(iRow, 0);
	m_wndGrid.Invalidate();
}

void CDM2GDEDView2::SetupCell(int iItem, CGDAT &core, const CGDEntry &pos)
{
	CString strName;

	m_wndGrid.SetItemState(iItem, 0, GVIS_READONLY);
	m_wndGrid.SetItemState(iItem, 1, GVIS_READONLY);
	m_wndGrid.SetItemState(iItem, 2, GVIS_READONLY);

	strName.Format("%02X %02X %02X"
		, 0U + pos.c4()
		, 0U + pos.c5()
		, 0U + pos.c6()
		);
	m_wndGrid.SetItemText(iItem, 0, strName);
	m_wndGrid.SetItemText(iItem, 1, pos.GetFriendlyTypeName(pos.c3()));

	m_wndGrid.SetCellType(iItem, 0, RUNTIME_CLASS(CEntryItemCell));

	CEntryItemCell *pCell = GetEntryItemCellAt(iItem);

	pCell->SetPos(pos);

	UpdateCell(iItem, core, &pos, CSize(0, 0), TRUE);
}

void CDM2GDEDView2::OnPopupDeleteEntry() 
{
	if (!ChkCtx())
		return;
	int iRow = m_rclkSel.row;
	CEntryItemCell *pCell = GetEntryItemCellAt(iRow);
	if (!pCell)
		return;
	CGDAT &core = m_pEdCtx->GetGDAT();

	int r = AfxMessageBox(IDS_WARN_DELETE_ENTRY, MB_ICONEXCLAMATION|MB_YESNO);
	if (r != IDYES)
		return;

	if (core.DeleteEntry(pCell->GetPos())) {
		DeleteRow(iRow);
	}
}

void CDM2GDEDView2::DeleteRow(int iRow)
{
	CCellID id = m_wndGrid.GetFocusCell();

	m_wndGrid.DeleteRow(iRow);
	m_wndGrid.Invalidate();

	int nRow = m_wndGrid.GetRowCount();
	if (nRow < 2) {
		m_wndGrid.SetRowCount(2);
		m_wndGrid.SetCellType(1, 0, RUNTIME_CLASS(CEntryItemCell));
	}

	id.row = __min(id.row, nRow - 1);
	m_wndGrid.SetFocusCell(id);
}

bool CDM2GDEDView2::ChkCtx()
{
	if (!m_pEdCtx)
		return false;
	return true;
}

CEntryItemCell *CDM2GDEDView2::GetEntryItemCellAt(int iRow)
{
	CEntryItemCell *pCell = (CEntryItemCell *)DYNAMIC_DOWNCAST(CEntryItemCell, m_wndGrid.GetCell(iRow, 0));
	return pCell;
}

void CDM2GDEDView2::OnPopupAddText() 
{
	OnAddCtx(0x05);
}

void CDM2GDEDView2::OnPopupAdd0B() 
{
	OnAddCtx(0x0B);
}

void CDM2GDEDView2::OnPopupAdd0C() 
{
	OnAddCtx(0x0C);
}

void CDM2GDEDView2::OnPopupEditText() 
{
	OnEditCtx(0x05);
}

void CDM2GDEDView2::OnPopupEdit0B() 
{
	OnEditCtx(0x0B);
}

void CDM2GDEDView2::OnPopupEdit0C() 
{
	OnEditCtx(0x0C);
}

void CDM2GDEDView2::OnPopupAddPcm() 
{
	OnAddCtx(0x02);
}

void CDM2GDEDView2::OnPopupEditPcm() 
{
	OnEditCtx(0x02);
}

bool CDM2GDEDView2::CreateRIFFWave(SizeBuff &fv)
{
	if (!ChkCtx())
		return false;
	CGDAT &core = m_pEdCtx->GetGDAT();

	if (CreateWav(fv, core.IsLE())) {
		return true;
	}
	return false;
}

bool CDM2GDEDView2::SetPCM11025(BYTE *pbWav, UINT cbWav)
{
	if (!ChkCtx())
		return false;
	if (!m_fvWav.Alloc(cbWav + 4))
		return false;
	CGDAT &core = m_pEdCtx->GetGDAT();
	BYTE *pbPos = (BYTE *)m_fvWav.GetData();
	BYTE *pbEnd = pbPos + m_fvWav.GetSize();
	memcpy(pbPos, core.IsLE() ? "\x77\x2B\x08\x01" : "\x2B\x77\x08\x01", 4);
	memcpy(pbPos + 4, pbWav, cbWav);
	return true;
}

void CDM2GDEDView2::OnSetFocus(CWnd* pOldWnd) 
{
	m_wndGrid.SetFocus();
}

void CDM2GDEDView2::OnUpdateNextPane(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CDM2GDEDView2::OnUpdatePrevPane(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CDM2GDEDView2::OnEditCtx(BYTE iCtx, int iRow)
{
	if (!ChkCtx())
		return;
	if (iRow < 0)
		iRow = m_rclkSel.row;
	CEntryItemCell *pCell = GetEntryItemCellAt(iRow);
	if (!pCell)
		return;
	CGDAT &core = m_pEdCtx->GetGDAT();
	CGDEntry pos = pCell->GetPos();
	I32 nVal = core.GetValueAt(pos);

	CAddImageDlg wndDlg(m_aEdOpt.m_fShowHex, this);
	switch (iCtx) {
	case 0x02:
		{
			if (!core.GetRawDataAt(nVal, m_fvWav))
				return;
			wndDlg.InitEditPCM(pos, this);
			break;
		}
	case 0x05:
		{
			CString strText;
			if (!core.GetTextAt(core.GetValueAt(pos), strText))
				return;
			strText.Replace("\n", "\r\n");
			wndDlg.InitEditText(pos, strText);
			break;
		}
	case 0x0B:
		{
			wndDlg.InitEdit0B(pos, nVal);
			break;
		}
	case 0x0C:
		{
			wndDlg.InitEdit0C(pos, nVal);
			break;
		}
	case 0x04:
		{
			SizeBuff fv;
			if (!core.GetRawDataAt(nVal, fv))
				return;
			m_aHexEdCtx = HexEditCtx(m_pEdCtx, pos);
			DoHexEdit(fv);
			return;
		}
	}

	while (true) {
		int r = wndDlg.DoModal();
		if (r != IDOK)
			return;
		CGDEntry pos = wndDlg.GetEntryPos();
		BOOL fEditOk = FALSE;

		switch (iCtx) {
		case 0x02:
			{
				fEditOk = core.SetRawDataFor(pos, m_fvWav);
				break;
			}
		case 0x05:
			{
				CString strText = wndDlg.m_strText;
				strText.Replace("\r\n", "\n");
				fEditOk = core.SetTextFor(pos, strText);
				break;
			}
		case 0x0B:
		case 0x0C:
			{
				UI32 nNewVal = wndDlg.GetVal();
				fEditOk = core.SetValueAt(pos, nNewVal, FALSE);
				break;
			}
		}

		if (fEditOk) {
			UpdateCell(iRow, core, &pos, CSize(0, 0), TRUE);
			break;
		} else {
			AfxMessageBox(IDS_ERR_ENTRY_DUPE, MB_ICONEXCLAMATION);
		}
	}
}

void CDM2GDEDView2::OnAddCtx(BYTE iCtx)
{
	if (!ChkCtx())
		return;
	CGDAT &core = m_pEdCtx->GetGDAT();

	I32 iSmily = -1;

	CAddImageDlg wndDlg(m_aEdOpt.m_fShowHex, this);
	switch (iCtx) {
	case 0x01:
		{
			if ((iSmily = core.GetValueAt(g_kSmily)) < 0) {
				AfxMessageBox(IDS_ERR_NO_SMILY, MB_ICONEXCLAMATION);
				return;
			}
			wndDlg.InitAddImage(m_lower);
			break;
		}
	case 0x02:
		{
			m_fvWav.Free();
			wndDlg.InitAddPCM(m_lower, this);
			break;
		}
	case 0x05:
		{
			wndDlg.InitAddText(m_lower);
			break;
		}
	case 0x0B:
		{
			wndDlg.InitAdd0B(m_lower);
			break;
		}
	case 0x0C:
		{
			wndDlg.InitAdd0C(m_lower);
			break;
		}
	case 0x04:
		{
			wndDlg.InitAdd04(m_lower);
			break;
		}
	case 0x07:
		{
			wndDlg.InitAdd07(m_lower);
			break;
		}
	case 0x09:
		{
			wndDlg.InitAdd09(m_lower);
			break;
		}
	case 0x8001:
		{
			break;
		}
	}

	while (true) {
		int r = wndDlg.DoModal();
		if (r != IDOK)
			return;
		CGDEntry pos = wndDlg.GetEntryPos();
		BOOL fAddOk = FALSE;

		switch (iCtx) {
		case 0x01:
			{
				fAddOk = core.AddEntry(pos, iSmily, TRUE);
				break;
			}
		case 0x02:
			{
				fAddOk = core.AddBinEntry(pos, m_fvWav);
				break;
			}
		case 0x05:
			{
				CString strText = wndDlg.m_strText;
				strText.Replace("\r\n", "\n");
				fAddOk = core.AddTextEntry(pos, strText);
				break;
			}
		case 0x0B:
		case 0x0C:
			{
				UINT nVal = wndDlg.GetVal();
				fAddOk = core.AddEntry(pos, nVal, FALSE);
				break;
			}
		case 0x04:
		case 0x07:
		case 0x09:
			{
				fAddOk = core.AddBinEntry(pos, SizeBuff());
				break;
			}
		}

		if (fAddOk) {
			UpdateAddEntry(pos);
			break;
		} else {
			AfxMessageBox(IDS_ERR_ENTRY_DUPE, MB_ICONEXCLAMATION);
		}
	}
}

void CDM2GDEDView2::OnGridKeydown(NMHDR* pNMHDR, LRESULT* pResult)
{
	GV_KEYDOWN *pNM = (GV_KEYDOWN *)pNMHDR;

	switch (pNM->wVKey) {
	case VK_DELETE:
		{
			CCellID id = m_wndGrid.GetFocusCell();
			m_rclkSel.col = id.col;
			m_rclkSel.row = id.row;

			OnPopupDeleteEntry();
			break;
		}
	case VK_INSERT:
		{
			CCellID id = m_wndGrid.GetFocusCell();

			CPoint point(0, 0);
			ClientToScreen(&point);
			OnRclickItem(id.col, id.row, point);
			break;
		}
	case VK_RETURN:
		{
			CCellID id = m_wndGrid.GetFocusCell();

			OnDblClkItem(id.col, id.row);
			break;
		}
	}

	*pResult = 0;
}

void CDM2GDEDView2::OnContextMenu(CWnd* pWnd, CPoint point) 
{
#if 0
	CCellID id = m_wndGrid.GetFocusCell();
	m_rclkSel.col = id.col;
	m_rclkSel.row = id.row;

	if (point.x < 0) {
		point.x = point.y = 0;
		ClientToScreen(&point);
		ShowCellMenu(point, 0);
	}
#endif
}

void CDM2GDEDView2::ShowCellMenu(CPoint point, int iCmd)
{
	CMenu m;
	m.LoadMenu(IDR_CELLCMD);
	CMenu *p = m.GetSubMenu(iCmd);
	if (p) p->TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this, NULL);
}

void CDM2GDEDView2::OnRclickItem(int iCol, int iRow, CPoint point)
{
	if (!ChkCtx())
		return;
	CEntryItemCell *pCell = GetEntryItemCellAt(iRow);

	CGDAT &core = m_pEdCtx->GetGDAT();

	int iPopUp = -1;
	if (iRow > 0) {
		iPopUp = 0;
		if (iCol == 2) {
			if (pCell) {
				switch (pCell->GetPos().c3()) {
				case 0x01:
					iPopUp = 2;
					break;
				case 0x05:
					iPopUp = 3;
					break;
				case 0x0B:
					iPopUp = 4;
					break;
				case 0x0C:
					iPopUp = 5;
					break;
				case 0x02:
					iPopUp = core.IsSnd56() ? 6 : 1;
					break;
				case 0x04:
				case 0x06:
				case 0x07:
				case 0x08:
				case 0x09:
				case 0x0E:
					iPopUp = 7;
					break;
				default:
					iPopUp = 1;
					break;
				}
			}
		}
	}
	if (iPopUp >= 0) {
		m_rclkSel.col = iCol;
		m_rclkSel.row = iRow;

		ShowCellMenu(point, iPopUp);
	}
}

BOOL CDM2GDEDView2::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	
	return CView::OnEraseBkgnd(pDC);
}

void CDM2GDEDView2::OnSetOpt()
{
	CGDEdOpt aEdOpt = m_aEdOpt;
	m_pEdCtx->GetOpts(m_aEdOpt);
	CGDEdOpt &rOld = aEdOpt;
	CGDEdOpt &rNew = m_aEdOpt;

	if (rOld.m_fShowHex != rNew.m_fShowHex) {
		int iRow = 1, nRows = m_wndGrid.GetRowCount();
		for (; iRow < nRows; iRow++) {
			CEntryItemCell *pCell = GetEntryItemCellAt(iRow);
			if (pCell) {
				CGDEntry pos = pCell->GetPos();
				switch (pos.c3()) {
				case 0x0B:
				case 0x0C:
					UpdateCell(iRow, m_pEdCtx->GetGDAT(), &pos, CSize(0, 0), TRUE);
					break;
				}
			}
		}
	}
}

void CDM2GDEDView2::OnPopupEditRaw() 
{
	OnEditCtx(0x04);
}

void CDM2GDEDView2::DoHexEdit(SizeBuff &fv)
{
	if (g_pHexEditWnd)
		g_pHexEditWnd->SendMessage(WM_CLOSE);
	if (g_pHexEditWnd)
		return;

	g_pHexEditWnd = new CHexEditFrame();

	if (g_pHexEditWnd->LoadFrame(IDR_HEXEDIT)) {
		CRect rc;
		GetParentFrame()->GetWindowRect(&rc);
		g_pHexEditWnd->MoveWindow(&rc);

		g_pHexEditWnd->InitialUpdateFrame(NULL, TRUE);
		CHexEditCB *pCtx = g_pHexEditWnd->GetHexEditCB();
		pCtx->SetRawData(fv.GetData(), fv.GetSize());
		pCtx->SetSink(&m_aHexEdCtx);
	}
}

void CDM2GDEDView2::OnPopupAdd04() 
{
	OnAddCtx(0x04);
}

void CDM2GDEDView2::OnPopupAdd07() 
{
	OnAddCtx(0x07);
}

void CDM2GDEDView2::OnPopupAdd09() 
{
	OnAddCtx(0x09);
}

void CDM2GDEDView2::OnPopupRenameEntry() 
{
	RenameEntry(-1);
}

void CDM2GDEDView2::RenameEntry(int iRow)
{
	if (!ChkCtx())
		return;
	if (iRow < 0)
		iRow = m_rclkSel.row;
	CEntryItemCell *pCell = GetEntryItemCellAt(iRow);
	if (!pCell)
		return;
	CGDAT &core = m_pEdCtx->GetGDAT();
	CGDEntry posSrc = pCell->GetPos();

	CAddImageDlg wndDlg(false, this);
	wndDlg.InitRename(posSrc);
	for (; ; ) {
		int r = wndDlg.DoModal();
		if (r != IDOK) return;

		CGDEntry posDst = wndDlg.GetEntryPos();
		if (!core.RenameEntry(posSrc, posDst)) {
			AfxMessageBox(IDS_ERR_ENTRY_DUPE, MB_ICONEXCLAMATION);
			continue;
		}
		DeleteRow(iRow);
		if (IsInSameScheme(posSrc, posDst)) {
			UpdateAddEntry(posDst);
		} else {
			m_pEdCtx->DispatchGDUpdate(PM_V_UPDATE_TREE_ITEM, &posDst);
		}
		break;
	}
}

BOOL CDM2GDEDView2::OnQueryNewPalette() 
{
	TRY
		CClientDC dc(&m_wndGrid);
		HPALETTE hPal = GetApp()->GetDM2Pal();
		UnrealizeObject(hPal);
		dc.SelectPalette(CPalette::FromHandle(hPal), FALSE);
		RealizePalette(dc);
	CATCH_ALL(e)

	END_CATCH_ALL
	
	return CView::OnQueryNewPalette();
}

void CDM2GDEDView2::OnPaletteChanged(CWnd* pFocusWnd) 
{
	CView::OnPaletteChanged(pFocusWnd);

	if (pFocusWnd == &m_wndGrid) return;

	TRY
		CClientDC dc(&m_wndGrid);
		HPALETTE hPal = GetApp()->GetDM2Pal();
		UnrealizeObject(hPal);
		RealizePalette(dc);
	CATCH_ALL(e)

	END_CATCH_ALL
}
