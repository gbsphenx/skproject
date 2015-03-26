// DGridCtrl.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DM2GDED.h"
#include "DGridCtrl.h"
#include "CompatDC.h"
#include "DM2GDEDDoc.h"
#include "AddImageDlg.h"
#include "HexEditFrame.h"
#include "WVCT.h"
#include "AddImageDlg2.h"
#include "DM2RawContConv.h"
#include "FM.h"
#include "DM2EdPicFrame.h"
#include "DM2NIPS.h"
#include "DM2NI1Page.h"
#include "DM2NI2Page.h"
#include "DM2NI3Page.h"
#include "DM2NIMan.h"
#include "InprogressDlg.h"
#include "DM2ImageComp.h"
#include "EditImgFlgsDlg.h"
#include "DM2NI4Page.h"
#include "OSP.h"
#include "EditStandbyDlg.h"
#include "EditHeroStatDlg.h"
#include "DM1ImageComp.h"
#include "GridCellCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLUMN_NAME	0
#define COLUMN_TYPE	1
#define COLUMN_DESC	2
#define COLUMN_VAL	3

const DWORD nDefSaveFlags = 0
	|OFN_ENABLESIZING
	|OFN_EXPLORER
	|OFN_HIDEREADONLY
	|OFN_LONGNAMES
	|OFN_OVERWRITEPROMPT
	|OFN_PATHMUSTEXIST
	;
const DWORD nDefOpenFlags = 0
	|OFN_ENABLESIZING
	|OFN_EXPLORER
	|OFN_FILEMUSTEXIST
	|OFN_HIDEREADONLY
	|OFN_LONGNAMES
	|OFN_PATHMUSTEXIST
	;

using namespace OSP;

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
// CDGridCtrl_PH

class CDGridCtrl_PH : public CDM2ImageComp::ProgressHint
{
	// 
	CInprogressDlg wndDlg;

public:
	// 
	CDGridCtrl_PH()
	{

	}
	// 
	~CDGridCtrl_PH()
	{
		wndDlg.Close();
	}
	// 
	void Create(CWnd *p)
	{
		wndDlg.DoModeless(p);
	}

	// 
	virtual void Set_Max(int iMax)
	{
		wndDlg.SetHintMinMax(0, iMax);
	}
	// 
	virtual void Set_Pos(int iPos)
	{
		wndDlg.SetHintPos(iPos);
	}

};

/////////////////////////////////////////////////////////////////////////////
// CDCellEntryItem

class CDCellEntryItem : public CGridCell
{
	// 
	DECLARE_DYNCREATE(CDCellEntryItem)

public:
	// 
	CDCellEntryItem(): m_pos(CGDEntry::ERR), m_fDynSel(false)
	{

	}
	// 
	void SetPos(const CGDEntry &pos) { m_pos = pos; }
	// 
	void SetCol(int col) { m_col = col; }
	// 
	const CGDEntry &GetPos() { return m_pos; }
	// 
	void SetDynSel(bool f) { m_fDynSel = f; }
	// 
	bool IsDynSel() const { return m_fDynSel; }
	// 
	virtual void operator =(const CDCellEntryItem& cell)
	{
		ASSERT(FALSE);
	}
	// 
	CDGridCtrl *GetGrid()
	{
		CDGridCtrl *pGrid = static_cast<CDGridCtrl *>(CGridCell::GetGrid());
		ASSERT_VALID(pGrid);
		return pGrid;
	}
	// 
	enum {
		addImage,
		addText,
		addWordValue,
		addImageOffset,
		add04, 
		add07, 
		add08, 
		add09, 
		add0E,
		editPureText,
		editRawWordValue,
		editRawImageOffset,
		openThruTempFile,
		playSound,
		editRaw,
		editText,
		editWordValue,
		editImageOffset,
		renameEntry,
		deleteEntry,
		importRawFromFile,
		exportRawToFile,
		copyWavToClipboard,
		pasteWavFromClipboard,
		importWavFromFile,
		exportWavToFile,
		copyBitmapToClipboard,
		pasteBitmapFromClipboard,
		importBitmapFromFile,
		exportBitmapToFile,
		editImageFlags,
		editThruTempFile,
		addSound,
		menuItemEndsHere,
		septor,
		editAsRaw,
		editChampStat,
		copySeledItems,
		pasteItems,
		mark4x4ToImage,
	};
	// 
	void OnRClick()
	{
#if 1
		struct TblMenuItem {
			int key;
			UINT nID;
			LPCSTR psz;
		};
		static const TblMenuItem tblItems[] = {
			{addImage, ID_POPUP_ADD_IMAGE, "Add &Image..."},
			{addText, ID_POPUP_ADD_TEXT, "Add &Text..."},
			{addSound, ID_POPUP_ADD_PCM, "Add &Sound..."},
			{addWordValue, ID_POPUP_ADD_0B, "Add Word value..."},
			{addImageOffset, ID_POPUP_ADD_0C, "Add Image offset..."},
			{add04, ID_POPUP_ADD_04, "Add 04 (raw)..."},
			{add07, ID_POPUP_ADD_07, "Add 07 (raw)..."},
			{add08, ID_POPUP_ADD_08, "Add 08 (raw)..."},
			{add09, ID_POPUP_ADD_09, "Add 09 (raw)..."},
			{add0E, ID_POPUP_ADD_0E, "Add 0E (raw)..."},
			{editPureText, ID_POPUP_EDIT_RAW_TEXT, "Edit Pure text..."},
			{editRawWordValue, ID_POPUP_EDIT_RAW_0B, "Edit Raw Word value ..."},
			{editRawImageOffset, ID_POPUP_EDIT_RAW_0C, "Edit Raw Image offset..."},
			{openThruTempFile, ID_POPUP_EDIT_IMAGE, "Open thru temp file"},
			{playSound, ID_POPUP_PLAY_ENTRY, "Play Sound"},
			{editRaw, ID_POPUP_EDIT_RAW, "Edit Raw..."},
			{editText, ID_POPUP_EDIT_TEXT, "Edit Text..."},
			{editWordValue, ID_POPUP_EDIT_0B, "Edit Word value..."},
			{editImageOffset, ID_POPUP_EDIT_0C, "Edit Image offset..."},
			{renameEntry, ID_POPUP_RENAME_ENTRY, "&Rename Entry"},
			{deleteEntry, ID_POPUP_DELETE_ENTRY, "&Delete Entry"},
			{importRawFromFile, ID_POPUP_IMPORT_RAW, "Import raw from file..."},
			{exportRawToFile, ID_POPUP_EXPORT_RAW, "Export raw to file..."},
			{copyWavToClipboard, ID_POPUP_COPY_PCM, "Copy wav to Clipboard"},
			{pasteWavFromClipboard, ID_POPUP_PASTE_PCM, "Paste wav from Clipboard"},
			{importWavFromFile, ID_POPUP_IMPORT_WAV, "Import wav from file..."},
			{exportWavToFile, ID_POPUP_EXPORT_WAV, "Export wav to file..."},
			{copyBitmapToClipboard, ID_POPUP_COPY_BMP, "Copy bitmap to Clipboard"},
			{pasteBitmapFromClipboard, ID_POPUP_PASTE_BMP, "Paste bitmap from Clipboard..."},
			{importBitmapFromFile, ID_POPUP_IMPORT_BMP, "Import bitmap from file..."},
			{exportBitmapToFile, ID_POPUP_EXPORT_BMP, "Export bitmap to file..."},
			{editImageFlags, ID_POPUP_EDIT_IMAGE_IPF, "Edit image flags..."},
			{editThruTempFile, ID_POPUP_EDIT_IMAGE_EXTERNAL, "Edit thru temp file"},
			{editAsRaw, ID_POPUP_EDIT_RAW, "Edit as Raw..."},
			{editChampStat, ID_POPUP_EDIT_CHAMP_STAT, "Edit Champion stat..."},
			{copySeledItems, ID_POPUP_COPY_SELED, "Copy selected items"},
			{pasteItems, ID_POPUP_PASTE_ITEMS, "Paste items"},
			{mark4x4ToImage, ID_POPUP_MARK4X4_TO_IMAGE, "Mark 4x4 to image"},
			{menuItemEndsHere, -1, NULL},
		};
		CUIntArray vec;
		CEditVxxx &vxxx = GetGrid()->GetVxxx(GetPos());
		switch (GetEntryType()) {
		case 0x3: // music
		case 0x4: // rect
		case 0x6: // raw
		case 0x7: // raw
		case 0x8: // raw
		case 0x9: // raw
		case 0xD: // raw
		case 0xE: // raw
			if (vxxx.GetOp().GetWvct() == wvctChampStat) {
				vec.Add(editChampStat);
				vec.Add(septor); // ---
			}
			vec.Add(editRaw);
			vec.Add(septor); // ---
			vec.Add(importRawFromFile);
			vec.Add(exportRawToFile);
			vec.Add(septor); // ---
			break;
		case 0x1: // image
			vec.Add(openThruTempFile);
			vec.Add(editThruTempFile);
			//vec.Add(mark4x4ToImage);
			vec.Add(editRaw);
			vec.Add(septor); // ---
			vec.Add(editImageFlags);
			vec.Add(septor); // ---
			vec.Add(copyBitmapToClipboard);
			vec.Add(pasteBitmapFromClipboard);
			vec.Add(importBitmapFromFile);
			vec.Add(exportBitmapToFile);
			vec.Add(septor); // ---
			vec.Add(importRawFromFile);
			vec.Add(exportRawToFile);
			vec.Add(septor); // ---
			break;
		case 0x2: // sound
			if (GetGrid()->IsSndOk()) vec.Add(playSound);
			vec.Add(editRaw);
			vec.Add(septor); // ---
			vec.Add(copyWavToClipboard);
			vec.Add(pasteWavFromClipboard);
			vec.Add(importWavFromFile);
			vec.Add(exportWavToFile);
			vec.Add(septor); // ---
			vec.Add(importRawFromFile);
			vec.Add(exportRawToFile);
			vec.Add(septor); // ---
			break;
		case 0x5: // text
			vec.Add(editText);
			vec.Add(editPureText);
			vec.Add(editRaw);
			vec.Add(septor); // ---
			vec.Add(importRawFromFile);
			vec.Add(exportRawToFile);
			vec.Add(septor); // ---
			break;
		case 0xB: // word value
			vec.Add(editWordValue);
			vec.Add(editRawWordValue);
			vec.Add(septor); // ---
			break;
		case 0xC: // image offset
			vec.Add(editImageOffset);
			vec.Add(editRawImageOffset);
			vec.Add(septor); // ---
			break;
		}

		vec.Add(renameEntry);
		vec.Add(septor); // ---
		vec.Add(deleteEntry);
		//vec.Add(septor); // ---
		//vec.Add(copySeledItems);
		//vec.Add(pasteItems);
		vec.Add(septor); // ---
		vec.Add(addImage);
		vec.Add(addText);
		vec.Add(addSound);
		vec.Add(addWordValue);
		vec.Add(addImageOffset);
		vec.Add(add04);
		vec.Add(add07);
		vec.Add(add08);
		vec.Add(add09);
		vec.Add(add0E);

		CMenu m;
		m.CreatePopupMenu();
		for (UINT x=0; x<static_cast<UINT>(vec.GetSize()); x++) {
			if (vec[x] == menuItemEndsHere)
				break;
			else if (vec[x] == septor) {
				m.AppendMenu(MF_ENABLED|MF_SEPARATOR);
			}
			else {
				for (int t=0; tblItems[t].key != menuItemEndsHere; t++) {
					if (tblItems[t].key == vec[x]) {
						m.AppendMenu(MF_ENABLED|MF_STRING, tblItems[t].nID, tblItems[t].psz);
						break;
					}
				}
			}
		}

		CPoint pt;
		GetCursorPos(&pt);
		m.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, pt.x, pt.y, GetGrid());

#else
//		struct TblAdd {
//			UINT nID;
//			LPCSTR pszText;
//		};
//		struct TblEdit {
//			UINT nFlags;
//			UINT nID;
//			LPCSTR pszText;
//		};
//		static const TblAdd tblAdd[] = {
//			{ID_POPUP_ADD_IMAGE, "Add &Image..."},
//			{ID_POPUP_ADD_TEXT, "Add &Text..."},
//			{ID_POPUP_ADD_PCM, "Add &Sound..."},
//			{ID_POPUP_ADD_0B, "Add Word value..."},
//			{ID_POPUP_ADD_0C, "Add Image offset..."},
//			{ID_POPUP_ADD_04, "Add 04 (raw)..."},
//			{ID_POPUP_ADD_07, "Add 07 (raw)..."},
//			{ID_POPUP_ADD_08, "Add 08 (raw)..."},
//			{ID_POPUP_ADD_09, "Add 09 (raw)..."},
//			{ID_POPUP_ADD_0E, "Add 0E (raw)..."},
//			{0},
//		};
//		static const TblEdit tblEditLowLevel[] = {
//			{0,0},
//			{4,0},
//			{2,0},
//			{1,0},
//			{1,0},
//			{0|1,ID_POPUP_EDIT_RAW_TEXT, "Edit Pure text..."},
//			{1,0},
//			{1,0},
//			{1,0},
//			{1,0},
//			{0,0},
//			{0,ID_POPUP_EDIT_RAW_0B, "Edit Raw Word value..."},
//			{0,ID_POPUP_EDIT_RAW_0C, "Edit Raw Image offset..."},
//			{1,0},
//			{1,0},
//		};
//		static const TblEdit tblEditPCAT[] = {
//			{0,0},
//			{4|1,ID_POPUP_EDIT_IMAGE, "Open thru temp file"},
//			{2|1,ID_POPUP_PLAY_ENTRY, "Play Sound"},
//			{1,ID_POPUP_EDIT_RAW, "Edit Raw..."},
//			{1,ID_POPUP_EDIT_RAW, "Edit Raw..."},
//			{0|1,ID_POPUP_EDIT_TEXT, "Edit Text..."},
//			{1,ID_POPUP_EDIT_RAW, "Edit Raw..."},
//			{1,ID_POPUP_EDIT_RAW, "Edit Raw..."},
//			{1,ID_POPUP_EDIT_RAW, "Edit Raw..."},
//			{1,ID_POPUP_EDIT_RAW, "Edit Raw..."},
//			{0,0},
//			{0,ID_POPUP_EDIT_0B, "Edit Word value..."},
//			{0,ID_POPUP_EDIT_0C, "Edit Image offset..."},
//			{1,ID_POPUP_EDIT_RAW, "Edit Raw..."},
//			{1,ID_POPUP_EDIT_RAW, "Edit Raw..."},
//		};
//		static const TblEdit tblEditPC98[] = {
//			{0,0},
//			{4|1,ID_POPUP_EDIT_IMAGE, "Open thru temp file"},
//			{1,ID_POPUP_EDIT_PCM, "Edit Raw..."},
//			{1,ID_POPUP_EDIT_RAW, "Edit Raw..."},
//			{1,ID_POPUP_EDIT_RAW, "Edit Raw..."},
//			{0|1,ID_POPUP_EDIT_TEXT, "Edit Text..."},
//			{1,ID_POPUP_EDIT_RAW, "Edit Raw..."},
//			{1,ID_POPUP_EDIT_RAW, "Edit Raw..."},
//			{1,ID_POPUP_EDIT_RAW, "Edit Raw..."},
//			{1,ID_POPUP_EDIT_RAW, "Edit Raw..."},
//			{0,0},
//			{0,ID_POPUP_EDIT_0B, "Edit Word value..."},
//			{0,ID_POPUP_EDIT_0C, "Edit Image offset..."},
//			{1,ID_POPUP_EDIT_RAW, "Edit Raw..."},
//			{1,ID_POPUP_EDIT_RAW, "Edit Raw..."},
//		};
//		static const TblAdd tblMisc[] = {
//			{ID_POPUP_RENAME_ENTRY, "&Rename Entry"}, // 0
//			{ID_POPUP_DELETE_ENTRY, "&Delete Entry"}, // 1
//			{ID_POPUP_IMPORT_RAW, "Import raw from file..."}, // 2
//			{ID_POPUP_EXPORT_RAW, "Export raw to file..."}, // 3
//			{ID_POPUP_COPY_PCM, "Copy wav to Clipboard"}, // 4
//			{ID_POPUP_PASTE_PCM, "Paste wav from Clipboard"}, // 5
//			{ID_POPUP_IMPORT_WAV, "Import wav from file..."}, // 6
//			{ID_POPUP_EXPORT_WAV, "Export wav to file..."}, // 7
//			{ID_POPUP_COPY_BMP, "Copy bitmap to Clipboard"}, // 8
//			{ID_POPUP_PASTE_BMP, "Paste bitmap from Clipboard..."}, // 9
//			{ID_POPUP_IMPORT_BMP, "Import bitmap from file..."}, // 10
//			{ID_POPUP_EXPORT_BMP, "Export bitmap to file..."}, // 11
//			{ID_POPUP_EDIT_IMAGE_IPF, "Edit image flags..."}, // 12
//			{ID_POPUP_EDIT_IMAGE_EXTERNAL, "Edit thru temp file"}, // 13
//			{ID_POPUP_EDIT_RAW, "Edit as Raw"}, // 14
//		};
//		const TblEdit *tblEdit = GetGrid()->IsSndOk()
//			? tblEditPCAT
//			: tblEditPC98;
//
//		if (GetGrid()->IsRO()) return;
//
//		CMenu m;
//		m.CreatePopupMenu();
//		BYTE c3 = GetEntryType();
//		if (c3 <= 15 && tblEdit[c3].nID != 0) {
//			UINT i = c3;
//			//if (i == 1) m.AppendMenu(MF_ENABLED|MF_STRING, ID_POPUP_VIEW_IMAGE, "&View Image...");
//
//			m.AppendMenu(MF_ENABLED|MF_STRING, tblEdit[i].nID, tblEdit[i].pszText);
//
//			if (tblEditLowLevel[c3].nID != 0) {
//				UINT i = c3;
//				m.AppendMenu(MF_ENABLED|MF_STRING, tblEditLowLevel[i].nID, tblEditLowLevel[i].pszText);
//			}
//
//			if (!(tblEdit[c3].nFlags & 4)) m.AppendMenu(MF_ENABLED|MF_SEPARATOR);
//		}
//		if (c3 != 0xFF) {
//			if (tblEdit[c3].nFlags & 4) {
//				m.AppendMenu(MF_ENABLED|MF_STRING, tblMisc[13].nID, tblMisc[13].pszText);
//				m.AppendMenu(MF_ENABLED|MF_STRING, tblMisc[14].nID, tblMisc[14].pszText); // EDIT AS RAW
//				m.AppendMenu(MF_ENABLED|MF_SEPARATOR);
//				m.AppendMenu(MF_ENABLED|MF_STRING, tblMisc[12].nID, tblMisc[12].pszText);
//				m.AppendMenu(MF_ENABLED|MF_SEPARATOR);
//				m.AppendMenu(MF_ENABLED|MF_STRING, tblMisc[ 8].nID, tblMisc[ 8].pszText);
//				m.AppendMenu(MF_ENABLED|MF_STRING, tblMisc[ 9].nID, tblMisc[ 9].pszText);
//				m.AppendMenu(MF_ENABLED|MF_STRING, tblMisc[10].nID, tblMisc[10].pszText);
//				m.AppendMenu(MF_ENABLED|MF_STRING, tblMisc[11].nID, tblMisc[11].pszText);
//				m.AppendMenu(MF_ENABLED|MF_SEPARATOR);
//			}
//			if (tblEdit[c3].nFlags & 2) {
//				m.AppendMenu(MF_ENABLED|MF_STRING, tblMisc[4].nID, tblMisc[4].pszText);
//				m.AppendMenu(MF_ENABLED|MF_STRING, tblMisc[5].nID, tblMisc[5].pszText);
//				m.AppendMenu(MF_ENABLED|MF_STRING, tblMisc[6].nID, tblMisc[6].pszText);
//				m.AppendMenu(MF_ENABLED|MF_STRING, tblMisc[7].nID, tblMisc[7].pszText);
//				m.AppendMenu(MF_ENABLED|MF_STRING, tblMisc[3].nID, tblMisc[3].pszText);
//				m.AppendMenu(MF_ENABLED|MF_SEPARATOR);
//			}
//			if (tblEdit[c3].nFlags & 1) {
//				m.AppendMenu(MF_ENABLED|MF_STRING, tblMisc[2].nID, tblMisc[2].pszText);
//				m.AppendMenu(MF_ENABLED|MF_STRING, tblMisc[3].nID, tblMisc[3].pszText);
//				m.AppendMenu(MF_ENABLED|MF_SEPARATOR);
//			}
//			m.AppendMenu(MF_ENABLED|MF_STRING, tblMisc[0].nID, tblMisc[0].pszText);
//			m.AppendMenu(MF_ENABLED|MF_SEPARATOR);
//			m.AppendMenu(MF_ENABLED|MF_STRING, tblMisc[1].nID, tblMisc[1].pszText);
//			m.AppendMenu(MF_ENABLED|MF_SEPARATOR);
//		}
//		UINT i;
//		for (i = 0; tblAdd[i].nID != 0; i++) {
//			m.AppendMenu(MF_ENABLED|MF_STRING, tblAdd[i].nID, tblAdd[i].pszText);
//		}
//
//		CPoint pt;
//		GetCursorPos(&pt);
//		m.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, pt.x, pt.y, GetGrid());
#endif
	}
	// 
	void OnDblClick()
	{
		if (GetGrid()->IsRO()) return;

		GetGrid()->ActivateEntry(m_pos, m_col);
	}

protected:
	// 
	CGDEntry m_pos;
	// 
	int m_col;
	// 
	bool m_fDynSel;

	// 
	void OnRClick(CPoint PointCellRelative)
	{
		OnRClick();
	}
	// 
	void OnDblClick(CPoint PointCellRelative)
	{
		OnDblClick();
	}
	// 
	BYTE GetEntryType()
	{
		return m_pos.c3();
	}

};

IMPLEMENT_DYNCREATE(CDCellEntryItem, CGridCell);

/////////////////////////////////////////////////////////////////////////////
// CDCellImageGetImgBase

class CDCellImageGetImgBase : public CRCI
{
public:
	// 
	virtual HBITMAP GetImg() = NULL;
};

/////////////////////////////////////////////////////////////////////////////
// CDCellImageGetImgImpl

class CDCellImageGetImgImpl : public CDCellImageGetImgBase
{
	// 
	CGDEdCtx &m_obj;
	// 
	CGDEntry m_pos;

public:
	// 
	CDCellImageGetImgImpl(CGDEdCtx &obj, const CGDEntry &pos): m_obj(obj), m_pos(pos)
	{
		m_obj.AddRef();
	}
	// 
	~CDCellImageGetImgImpl()
	{
		m_obj.Release();
	}
	// 
	virtual HBITMAP GetImg()
	{
		return m_obj.GetGDAT().GetImgAt(m_pos);
	}
};

/////////////////////////////////////////////////////////////////////////////
// CDCellImage

class CDCellImage : public CDCellEntryItem
{
	// 
	CDCellImageGetImgBase *m_pGetImg;
	// 
	CExtBM m_bm;

	// 
	DECLARE_DYNCREATE(CDCellImage)

public:
	// 
	CDCellImage()
	{
		m_pGetImg = NULL;
	}
	// 
	~CDCellImage()
	{
		if (m_pGetImg)
			m_pGetImg->Release();
	}
	// 
	void SetGetImage(CDCellImageGetImgBase *pGetImg)
	{
		if (m_pGetImg)
			m_pGetImg->Release();
		if (m_pGetImg = pGetImg)
			m_pGetImg->AddRef();

		m_bm.DeleteObject();
	}
	// 
	CSize GetImageSize()
	{
		if (MakeImageReady()) {
			return m_bm.GetSize();
		}
		return CSize(0, 0);
	}

protected:
	// 
	bool MakeImageReady()
	{
		if ((HBITMAP)m_bm == NULL) {
			m_bm.Attach(m_pGetImg->GetImg());
			if ((HBITMAP)m_bm == NULL) {
				return false;
			}
		}
		return true;
	}
	// 
	virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd)
	{
		if (!CGridCell::Draw(pDC, nRow, nCol, rect, bEraseBkgnd))
			return FALSE;
		if (MakeImageReady()) {
			CPalette *pPal = &m_bm.GetPalette();
			CSize size = m_bm.GetSize();

			CCompatDC dcMem(pDC);
			dcMem.SaveDC();
			dcMem.SelectObject(&m_bm);
			dcMem.SelectPalette(pPal, FALSE);

			pDC->SaveDC();
			pDC->SelectPalette(pPal, FALSE);

			rect.DeflateRect(1, 1, 0, 1);
			pDC->IntersectClipRect(&rect);
			pDC->BitBlt(
				rect.left, 
				rect.top,
				size.cx,
				size.cy,
				&dcMem,
				0,
				0,
				SRCCOPY
				);

			pDC->RestoreDC(-1);
			dcMem.RestoreDC(-1);
		}
		return TRUE;
	}
};

IMPLEMENT_DYNCREATE(CDCellImage, CDCellEntryItem);

/////////////////////////////////////////////////////////////////////////////
// CDCellChk

class CDCellChk : public CDCellEntryItem
{
    DECLARE_DYNCREATE(CDCellChk)

public:
    CDCellChk() {
		m_bChecked = false;
	}

public:
	BOOL SetCheck(BOOL bChecked = TRUE) {
		BOOL bTemp = m_bChecked;
		m_bChecked = bChecked;
		if (!m_Rect.IsRectEmpty())
			GetGrid()->InvalidateRect(m_Rect);

		return bTemp;
	}

	BOOL GetCheck() {
		return m_bChecked;
	}

// Operations
	virtual CSize GetCellExtent(CDC* pDC) {
		int nWidth = GetSystemMetrics(SM_CYMENUCHECK) + 2*GetMargin();
		return CGridCell::GetCellExtent(pDC) + CSize(nWidth, nWidth);
	}

    virtual void OnClick( CPoint PointCellRelative) {
		// PointCellRelative is relative to the topleft of the cell. Convert to client coords
		PointCellRelative += m_Rect.TopLeft();

		// GetCheckPlacement returns the checkbox dimensions in client coords. Only check/
		// uncheck if the user clicked in the box
		if (GetCheckPlacement().PtInRect(PointCellRelative))
		{
			m_bChecked = !m_bChecked;
			GetGrid()->InvalidateRect(m_Rect);
		}
	}

    virtual BOOL GetTextRect( LPRECT pRect) {
		BOOL bResult = CGridCell::GetTextRect(pRect);
		if (bResult)
		{
			int nWidth = GetSystemMetrics(SM_CYMENUCHECK) + 2*GetMargin();
			pRect->left += nWidth;
			if (pRect->left > pRect->right)
				pRect->left = pRect->right;
		}
		return bResult;
	}

	BOOL Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar) {
		m_bChecked = !m_bChecked;
		if (!m_Rect.IsRectEmpty())
			GetGrid()->InvalidateRect(m_Rect);

		return false;
	}

protected:
	CRect GetCheckPlacement() {
		int nWidth = GetSystemMetrics(SM_CYMENUCHECK) + GetSystemMetrics(SM_CXEDGE) * 2;
		CRect place = m_Rect + CSize(GetMargin(), 0);
		place.right = place.left + nWidth;
		place.bottom = place.top + nWidth;

		int offy = (m_Rect.Height() -place.Height()) / 2;
		place.OffsetRect(0, offy);

		return place;
	}

    virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE) {
	    BOOL bResult = CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);

#ifndef _WIN32_WCE
		int nSavedDC = pDC->SaveDC();
		CRect clipRect = rect;
		pDC->IntersectClipRect(&clipRect);

		// Store the cell's dimensions for later
		m_Rect = rect;

		CRect CheckRect = GetCheckPlacement();
		rect.left = CheckRect.right;

		// enough room to draw?
		// if (CheckRect.Width() < rect.Width() && CheckRect.Height() < rect.Height()) {

		// Do the draw 

		pDC->DrawFrameControl(GetCheckPlacement(), DFC_BUTTON, 
			(m_bChecked)? DFCS_BUTTONCHECK | DFCS_CHECKED : DFCS_BUTTONCHECK);

		// }

		pDC->RestoreDC(nSavedDC);
#endif
	    return bResult;
	}

protected:
    BOOL  m_bChecked;
    CRect m_Rect;

};

IMPLEMENT_DYNCREATE(CDCellChk, CDCellEntryItem);

/////////////////////////////////////////////////////////////////////////////
// CDGridCtrl::HexEditCtx

void CDGridCtrl::HexEditCtx::OnHexEditApply(CHexEditCB *pSender)
{
	if (pCtx && g_pHexEditWnd && g_pHexEditWnd->GetHexEditCB() == pSender) {
		SizeBuff fv;
		pSender->GetRawData(fv);
		pCtx->GetGDAT().SetRawDataFor(pos, fv);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDGridCtrl

CDGridCtrl::CDGridCtrl()
{
	m_pEdCtx = NULL;
}

CDGridCtrl::~CDGridCtrl()
{
	InitEdCtx(NULL);
}

IMPLEMENT_DYNCREATE(CDGridCtrl, CGridCtrl)

BEGIN_MESSAGE_MAP(CDGridCtrl, CGridCtrl)
	//{{AFX_MSG_MAP(CDGridCtrl)
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_COMMAND(ID_POPUP_ADD_04, OnContextAdd04)
	ON_COMMAND(ID_POPUP_ADD_07, OnContextAdd07)
	ON_COMMAND(ID_POPUP_ADD_09, OnContextAdd09)
	ON_COMMAND(ID_POPUP_ADD_0B, OnContextAdd0b)
	ON_COMMAND(ID_POPUP_ADD_0C, OnContextAdd0c)
	ON_COMMAND(ID_POPUP_ADD_IMAGE, OnContextAddImage)
	ON_COMMAND(ID_POPUP_ADD_PCM, OnContextAddPcm)
	ON_COMMAND(ID_POPUP_ADD_TEXT, OnContextAddText)
	ON_COMMAND(ID_POPUP_DELETE_ENTRY, OnContextDeleteEntry)
	ON_COMMAND(ID_POPUP_RENAME_ENTRY, OnContextRenameEntry)
	ON_COMMAND(ID_POPUP_EDIT_0B, OnContextEdit0b)
	ON_COMMAND(ID_POPUP_EDIT_0C, OnContextEdit0c)
	ON_COMMAND(ID_POPUP_EDIT_IMAGE, OnContextEditImage)
	ON_COMMAND(ID_POPUP_EDIT_PCM, OnContextEditPcm)
	ON_COMMAND(ID_POPUP_EDIT_RAW, OnContextEditRaw)
	ON_COMMAND(ID_POPUP_EDIT_TEXT, OnContextEditText)
	ON_WM_SYSCOLORCHANGE()
	ON_COMMAND(ID_POPUP_IMPORT_RAW, OnPopupImportRaw)
	ON_COMMAND(ID_POPUP_EXPORT_RAW, OnPopupExportRaw)
	ON_COMMAND(ID_POPUP_EXPORT_WAV, OnPopupExportWav)
	ON_COMMAND(ID_POPUP_IMPORT_WAV, OnPopupImportWav)
	ON_COMMAND(ID_POPUP_COPY_PCM, OnPopupCopyPcm)
	ON_COMMAND(ID_POPUP_PASTE_PCM, OnPopupPastePcm)
	ON_COMMAND(ID_POPUP_PLAY_ENTRY, OnPopupPlayEntry)
	ON_COMMAND(ID_POPUP_VIEW_IMAGE, OnPopupViewImage)
	ON_COMMAND(ID_POPUP_EXPORT_BMP, OnPopupExportBmp)
	ON_COMMAND(ID_POPUP_IMPORT_BMP, OnPopupImportBmp)
	ON_COMMAND(ID_POPUP_PASTE_BMP, OnPopupPasteBmp)
	ON_COMMAND(ID_POPUP_COPY_BMP, OnPopupCopyBmp)
	ON_COMMAND(ID_POPUP_EDIT_IMAGE_IPF, OnPopupEditImageIPF)
	ON_COMMAND(ID_POPUP_ADD_08, OnContextAdd08)
	ON_COMMAND(ID_POPUP_ADD_0E, OnContextAdd0e)
	ON_COMMAND(ID_POPUP_EDIT_IMAGE_EXTERNAL, OnContextEditImageExternal)
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_POPUP_EDIT_RAW_TEXT, OnContextEditRawText)
	ON_COMMAND(ID_POPUP_EDIT_RAW_0B, OnContextEditRaw0B)
	ON_COMMAND(ID_POPUP_EDIT_RAW_0C, OnContextEditRaw0C)
	ON_COMMAND(ID_POPUP_EDIT_CHAMP_STAT, OnPopupEditChampStat)
	ON_COMMAND(ID_POPUP_COPY_SELED, OnPopupCopySeled)
	ON_COMMAND(ID_POPUP_PASTE_ITEMS, OnPopupPasteItems)
	ON_COMMAND(ID_POPUP_MARK4X4_TO_IMAGE, OnPopupMark4x4ToImage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDGridCtrl メッセージ ハンドラ

void CDGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//	SendKeyDownToParent(m_idCurrentCell.row, m_idCurrentCell.col, nChar);

	switch (nChar) {
	case VK_DELETE:
		{
			DeleteEntryItem();
			break;
		}
	case VK_INSERT:
		{
			CCellID ci = GetFocusCell();
			CDCellEntryItem *pItem;
			if (pItem = GetEntryItem(Conv2Entry(ci.row), COLUMN_NAME))
				pItem->OnRClick();
			break;
		}
	case VK_RETURN:
		{
			CCellID ci = GetFocusCell();
			OnDblClkCell(ci.row, ci.col);
			break;
		}
	}
	
	CGridCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

LRESULT CDGridCtrl::SendKeyDownToParent(int nRow, int nCol, WORD wVKey) const
{
    if (!IsWindow(m_hWnd))
        return 0;

    GV_KEYDOWN nmgv;
    nmgv.iRow         = nRow;
    nmgv.iColumn      = nCol;
    nmgv.hdr.hwndFrom = m_hWnd;
    nmgv.hdr.idFrom   = GetDlgCtrlID();
    nmgv.hdr.code     = GVN_KEYDOWN;
	nmgv.wVKey        = wVKey;

    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        return pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
    else
        return 0;
}

int CDGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CreateList(CGDEntry::ERR, CGDEntry::ERR, TRUE);

	SetGridBkColor(GetSysColor(COLOR_WINDOW));
	
	return 0;
}

void CDGridCtrl::CreateList(CGDEntry lower, CGDEntry upper, BOOL fRO)
{
	SetRedraw(FALSE);

	SetRowCount(1);
	//DeleteAllItems();

	SetDefaultCellType(RUNTIME_CLASS(CDCellEntryItem));

	SetColumnCount(4);
	SetEntryItemCount(0);

	SetItemText(0, COLUMN_NAME, "Name");
	SetItemText(0, COLUMN_TYPE, "Type");
	SetItemText(0, COLUMN_VAL, "Value");
	SetItemText(0, COLUMN_DESC, "Description");

	m_fRO = fRO;

	do {
		if (!m_pEdCtx) break;

		m_pEdCtx->GetOpts(m_aEdOpt);

		CGDAT &core = GetGDAT();
		list<CGDEntry> listKey;
		core.Enum(listKey, m_lower = lower, m_upper = upper);

		UINT iEntry, nEntries = listKey.size();
		if (!nEntries) break;

		SetEntryItemCount(nEntries);

		list<CGDEntry>::iterator
			iterPos = listKey.begin(),
			iterEnd = listKey.end();
		CString strName;
		m_ci = CellProcessing();
		for (iEntry = 0; iterPos != iterEnd; iEntry++, iterPos++) {
			UpdateEntryContents(iEntry, &*iterPos);
		}
		ReflectCellProcessing();
		SetRedraw(TRUE, TRUE); Invalidate();
		ResetFocusCell(CCellID(1, 0));
		return;
	} while(false);

	SetEntryItemCount(1);
	MakeEntryContentsEmpty(0);

	SetRedraw(TRUE, TRUE); Invalidate();
	ResetFocusCell(CCellID(1, 0));
	return;
}

void CDGridCtrl::FormatEntryName(const CGDEntry &pos, CString &strVal)
{
	strVal.Format("%02X %02X %02X"
		, 0U+pos.c4()
		, 0U+pos.c5()
		, 0U+pos.c6()
		);
}

void CDGridCtrl::FormatEntryType(const CGDEntry &pos, CString &strVal)
{
	strVal = CGDEntry::GetFriendlyTypeName(pos.c3());
}

CGDAT &CDGridCtrl::GetGDAT()
{
	if (!m_pEdCtx) { ASSERT(FALSE); AfxThrowNotSupportedException(); }
	return m_pEdCtx->GetGDAT();
}

void CDGridCtrl::FormatEntryContents(const CGDEntry &pos, CString &strVal, const CDM2DescItem *pItem)
{
	ASSERT(m_pEdCtx);

	BYTE c3 = pos.c3();

	CGDAT &core = GetGDAT();

	UINT nVal = core.GetValueAt(pos);

	strVal.Empty();

	switch (c3) {
	case 0x02:
		{
			if (core.IsSndPCM()) {
				strVal.Format("%u ...", nVal);
			} else {
				int nLen = core.GetSizeOfRawData(nVal);

				strVal.Format((nLen < 0) ? "%u ..." : "%u ...  (%d bytes)", nVal, nLen);
			}
			break;
		}
	case 0x05:
		{
			core.GetTextAt(nVal, strVal);
			break;
		}
	case 0x0B:
		{
			if (pItem == NULL) {
				strVal = m_pEdCtx->GetVxxxFactory().GetVxxx(m_aEdOpt.m_wvct).GetOp().FormatDisp(nVal);
			} else {
				strVal = pItem->GetVxxx().GetOp().FormatDisp(nVal);
			}
			break;
		}
	case 0x0C:
		{
			int x = (nVal >> 8) & 255;
			int y = (nVal >> 0) & 255;
			if (127 < x) x -= 256;
			if (127 < y) y -= 256;
			strVal.Format("Image offset. x=%4d, y=%4d"
				, x
				, y
				);
			break;
		}
	case 0x03:
	case 0x04:
	case 0x06:
	case 0x07: // ANYRAW
	case 0x08:
	case 0x09:
	case 0x0E:
	case 0x0D:
		{
			int nLen = core.GetSizeOfRawData(nVal);

			strVal.Format((nLen < 0) ? "%u ..." : "%u ...  (%d bytes)", nVal, nLen);
			break;
		}
	}
}

void CDGridCtrl::OnGDUpdate(UINT nHint, void *pHint)
{
	ASSERT(m_pEdCtx);

	switch (nHint) {
	case PM_V_RELOAD:
		CreateList(CGDEntry::ERR, CGDEntry::ERR, TRUE);
		break;
	case PM_V_SET_OPT:
		OnSetOpt();
		break;
	case PM_V_REFRESH:
		UpdateAllEntryContents();
		break;
	case PM_V_UPDATE_ENTRY:
		OnUpdateEntryItem(*(CGDEntry *)pHint);
		break;
	}
}

void CDGridCtrl::OnSetOpt()
{
	ASSERT(m_pEdCtx);

	CGDEdOpt aEdOpt = m_aEdOpt;
	m_pEdCtx->GetOpts(m_aEdOpt);

	if (false
		|| aEdOpt.m_cnlang != m_aEdOpt.m_cnlang
		|| aEdOpt.m_inlang != m_aEdOpt.m_inlang
		|| aEdOpt.m_wvct != m_aEdOpt.m_wvct
	) {
		CWaitCursor wc;
		UpdateAllEntryContents();
	}
}

void CDGridCtrl::UpdateAllEntryContents()
{
	if (!m_pEdCtx) return;

	CCellID id = GetFocusCell();
	m_ci = CellProcessing();
	UINT iEntry = 0, nEntries = GetEntryItemCount();
	for (; iEntry < nEntries; iEntry++) {
		UpdateEntryContents(iEntry, NULL);
	}
	ReflectCellProcessing();
	Invalidate();
	ResetFocusCell(id);
}

void CDGridCtrl::ReflectCellProcessing()
{
	int nWidth = GetColumnWidth(COLUMN_VAL);
	int nWidthNew = __max(nWidth, 3 + m_ci.nWidth);
	SetColumnWidth(COLUMN_VAL, nWidthNew);

	if (nWidth != nWidthNew) { Invalidate(); }
}

void CDGridCtrl::OnContextAdd04() { OnAddCtx(0x04); }
void CDGridCtrl::OnContextAdd08() { OnAddCtx(0x08); }
void CDGridCtrl::OnContextAdd07() { OnAddCtx(0x07); }
void CDGridCtrl::OnContextAdd09() { OnAddCtx(0x09); }
void CDGridCtrl::OnContextAdd0b() { OnAddCtx(0x0B); }
void CDGridCtrl::OnContextAdd0c() { OnAddCtx(0x0C); }
void CDGridCtrl::OnContextAdd0e() { OnAddCtx(0x0E); }
void CDGridCtrl::OnContextAddImage() { OnAddCtx(0x01); }
void CDGridCtrl::OnContextAddPcm() { OnAddCtx(0x02); }
void CDGridCtrl::OnContextAddText() { OnAddCtx(0x05); }

void CDGridCtrl::OnContextEdit0b() { OnEditCtx2(0x0B); }
void CDGridCtrl::OnContextEdit0c() { OnEditCtx2(0x0C); }
void CDGridCtrl::OnContextEditPcm() { OnEditCtx(0x02); }
void CDGridCtrl::OnContextEditRaw() { OnEditCtx(0x04); }
void CDGridCtrl::OnContextEditText() { OnEditCtx2(0x05); }

void CDGridCtrl::OnContextEditImage() { OnOpenImageExternally(false); }
void CDGridCtrl::OnContextEditImageExternal() { OnOpenImageExternally(true); }

void CDGridCtrl::OnContextEditRawText() { OnEditCtx2(0x05, true); }
void CDGridCtrl::OnContextEditRaw0B() { OnEditCtx2(0x0B, true); }
void CDGridCtrl::OnContextEditRaw0C() { OnEditCtx2(0x0C, true); }

void CDGridCtrl::OnPopupMark4x4ToImage() {
	CCellID ci = GetFocusCell();
	CGDEntry pos = GetEntryItemID(Conv2Entry(ci.row));

	CGDAT &core = m_pEdCtx->GetGDAT();

	core.Mark4x4ToImage(pos);
}

void CDGridCtrl::OnContextDeleteEntry() 
{
	if (!m_pEdCtx) return;

	DeleteEntryItem();
}

void CDGridCtrl::OnContextRenameEntry() 
{
	if (!m_pEdCtx) return;

	RenameEntryItem();
}

void CDGridCtrl::OnAddCtx(BYTE c3)
{
	if (!m_pEdCtx) return;

	CCellID ci = GetFocusCell();
	CGDEntry lower = m_lower;
	lower = CGDEntry(lower.c1(), lower.c2(), c3, 0, 0, 0);

	CGDAT &core = GetGDAT();

	I32 iSmily = -1;

	CAddImageDlg wndDlg(m_pEdCtx->GetVxxxFactory().GetVxxx(m_aEdOpt.m_wvct), this);
	switch (c3) {
	case 0x01:
		{
			if ((iSmily = core.GetValueAt(g_kSmily)) < 0) {
				AfxMessageBox(IDS_ERR_NO_SMILY, MB_ICONEXCLAMATION);
				return;
			}
			wndDlg.InitAddImage(lower);
			break;
		}
	case 0x02:
		{
			m_fvWav.Free();
			wndDlg.InitAddPCM(lower, this);
			break;
		}
	case 0x05:
		{
			wndDlg.InitAddText(lower);
			break;
		}
	case 0x0B:
		{
			wndDlg.InitAdd0B(lower);
			break;
		}
	case 0x0C:
		{
			wndDlg.InitAdd0C(lower);
			break;
		}
	case 0x04:
		{
			wndDlg.InitAdd04(lower);
			break;
		}
	case 0x07: // ANYRAW
		{
			wndDlg.InitAdd07(lower);
			break;
		}
	case 0x08:
		{
			wndDlg.InitAdd08(lower);
			break;
		}
	case 0x09:
		{
			wndDlg.InitAdd09(lower);
			break;
		}
	case 0x0E:
		{
			wndDlg.InitAdd0E(lower);
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

		switch (c3) {
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
		case 0x07: // ANYRAW
		case 0x08:
		case 0x09:
		case 0x0E:
			{
				fAddOk = core.AddBinEntry(pos, SizeBuff());
				break;
			}
		}

		if (fAddOk) {
			OnAddEntryItem(pos);
			UpdateDynSelEntries();
			ResetFocusCell(ci);
			break;
		} else {
			AfxMessageBox(IDS_ERR_ENTRY_DUPE, MB_ICONEXCLAMATION);
		}
	}
}

void CDGridCtrl::OnEditCtx2(BYTE c3, bool editRaw)
{
	if (!m_pEdCtx) return;

	CCellID ci = GetFocusCell();
	UINT iEntry = Conv2Entry(ci.row);
	CGDEntry pos = GetEntryItemID(iEntry);

	CGDAT &core = GetGDAT();
	I32 nVal = core.GetValueAt(pos);

	CEditVxxx &vxxx = editRaw ? GetRawVxxx(pos) : GetVxxx(pos);

	CAddImageDlg2 wndDlg(&vxxx, this);
	switch (c3) {
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
	case 0x05:
		{
			CString strText;
			if (!core.GetTextAt(core.GetValueAt(pos), strText))
				return;
			strText.Replace("\n", "\r\n");
			wndDlg.InitEditText(pos, strText);
			break;
		}
	}

	while (true) {
		int r = wndDlg.DoModal();
		if (r != IDOK)
			return;
		CGDEntry pos = wndDlg.GetEntryPos();
		BOOL fEditOk = FALSE;

		switch (c3) {
		case 0x05:
			{
				CString strText = wndDlg.GetText();
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
			UpdateSingleEntryContents(iEntry, &pos);
			UpdateDynSelEntries();
			ResetFocusCell(ci);
			break;
		} else {
			AfxMessageBox(IDS_ERR_ENTRY_DUPE, MB_ICONEXCLAMATION);
		}
	}
}

void CDGridCtrl::OnEditCtx(BYTE c3)
{
	if (!m_pEdCtx) return;

	CCellID ci = GetFocusCell();
	UINT iEntry = Conv2Entry(ci.row);
	CGDEntry pos = GetEntryItemID(iEntry);

	CGDAT &core = GetGDAT();
	I32 nVal = core.GetValueAt(pos);

	CEditVxxx &vxxx = GetVxxx(pos); // CWordValCmdCreator::GetOp(m_aEdOpt.m_wvct)

	if (c3 == 2 && !core.IsSndPCM()) c3 = 0x04;

	CAddImageDlg wndDlg(vxxx, this);
	switch (c3) {
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

		switch (c3) {
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
			UpdateSingleEntryContents(iEntry, &pos);
			UpdateDynSelEntries();
			ResetFocusCell(ci);
			break;
		} else {
			AfxMessageBox(IDS_ERR_ENTRY_DUPE, MB_ICONEXCLAMATION);
		}
	}
}

void CDGridCtrl::UpdateDynSelEntries()
{
	int iEntry;
	for (iEntry = 0; iEntry < GetEntryItemCount(); iEntry++) {
		CDCellEntryItem *pItem = GetEntryGenericContents(iEntry);
		if (pItem != NULL && pItem->IsDynSel()) {
			UpdateSingleEntryContents(iEntry);
		}
	}
}

bool CDGridCtrl::CreateRIFFWave(SizeBuff &fv)
{
	ASSERT(m_pEdCtx);

	CGDAT &core = GetGDAT();

	if (CreateWav(fv, core.IsLE())) {
		return true;
	}
	return false;
}

bool CDGridCtrl::SetPCM11025(BYTE *pbWav, UINT cbWav)
{
	if (!m_fvWav.Alloc(cbWav + 4))
		return false;
	CGDAT &core = GetGDAT();
	BYTE *pbPos = (BYTE *)m_fvWav.GetData();
	BYTE *pbEnd = pbPos + m_fvWav.GetSize();
	memcpy(pbPos, core.IsLE() ? "\x77\x2B\x08\x01" : "\x2B\x77\x08\x01", 4);
	memcpy(pbPos + 4, pbWav, cbWav);
	return true;
}

bool CDGridCtrl::CreateWav(SizeBuff &fvRaw, BOOL fLE)
{
	CGDAT &core = GetGDAT();
	CDM2RCConv cc(core.IsLE(), core.GetGDRSDT());
	return cc.EncodeRIFFWave((const BYTE *)m_fvWav.GetData(), m_fvWav.GetSize(), CDM2RCConvMIfSizeBuff(fvRaw));
}

void CDGridCtrl::DoHexEdit(SizeBuff &fv)
{
	if (g_pHexEditWnd)
		g_pHexEditWnd->SendMessage(WM_CLOSE);
	if (g_pHexEditWnd)
		return;

	g_pHexEditWnd = new CHexEditFrame();

	if (g_pHexEditWnd->LoadFrame(IDR_HEXEDIT)) {
		CRect rc;
		GetTopLevelFrame()->GetWindowRect(&rc);
		g_pHexEditWnd->MoveWindow(&rc);

		g_pHexEditWnd->InitialUpdateFrame(NULL, TRUE);
		CHexEditCB *pCtx = g_pHexEditWnd->GetHexEditCB();
		pCtx->SetRawData(fv.GetData(), fv.GetSize());
		pCtx->SetSink(&m_aHexEdCtx);
	}
}

void CDGridCtrl::ResetFocusCell(CCellID ci)
{
	SetFocusCell(0, 0);
	SetFocusCell(ci);
}

void CDGridCtrl::OnAddEntryItem(CGDEntry pos)
{
	ASSERT(m_pEdCtx);

	int iEntry, nEntries = GetEntryItemCount();
	for (iEntry = 0; iEntry < nEntries; iEntry++) {
		CGDEntry posVw = GetEntryItemID(iEntry);
		if (pos < posVw) break;
	}
	if (iEntry >= nEntries) {
		SetRowCount(GetRowCount() + 1);
	} else {
		InsertRow("", 1 + iEntry);
	}
	UpdateSingleEntryContents(iEntry, &pos);
}

void CDGridCtrl::OnDeleteEntryItem(CGDEntry pos)
{
	ASSERT(m_pEdCtx);

	CCellID ci = GetFocusCell();
	int iEntry, nEntries = GetEntryItemCount();
	for (iEntry = 0; iEntry < nEntries; iEntry++) {
		CGDEntry posVw = GetEntryItemID(iEntry);
		if (pos == posVw) {
			UINT iRow = Conv2Row(iEntry);
			DeleteRow(iRow);
			if (GetEntryItemCount() == 0) {
				SetEntryItemCount(1);
				MakeEntryContentsEmpty(0);
			}
			Refresh();
			ci.row = __min(ci.row, GetRowCount() - 1);
			ResetFocusCell(ci);
			break;
		}
		if (pos < posVw) break;
	}
}

void CDGridCtrl::OnUpdateEntryItem(CGDEntry pos)
{
	ASSERT(m_pEdCtx);

	int iEntry, nEntries = GetEntryItemCount();
	for (iEntry = 0; iEntry < nEntries; iEntry++) {
		CGDEntry posVw = GetEntryItemID(iEntry);
		if (pos == posVw) break;
	}
	if (iEntry != nEntries) {
		UpdateSingleEntryContents(iEntry, &pos);
	}
}

int CDGridCtrl::GetEntryItemCount()
{
	return GetRowCount() - 1;
}

CGDEntry CDGridCtrl::GetEntryItemID(UINT iEntry)
{
	CDCellEntryItem *pItem = GetEntryGenericContents(iEntry);
	if (pItem) {
		return pItem->GetPos();
	}
	return CGDEntry::ERR;
}

void CDGridCtrl::RenameEntryItem()
{
	if (!m_pEdCtx || IsRO()) return;

	CCellID ci = GetFocusCell();
	CGDAT &core = GetGDAT();
	CGDEntry posSrc = GetEntryItemID(Conv2Entry(ci.row));

	CAddImageDlg2 wndDlg(&m_pEdCtx->GetVxxxFactory().GetVxxx(-1), this);
	wndDlg.InitRename(posSrc);
	for (; ; ) {
		int r = wndDlg.DoModal();
		if (r != IDOK) return;

		CGDEntry posDst = wndDlg.GetEntryPos();
		if (!core.RenameEntry(posSrc, posDst)) {
			AfxMessageBox(IDS_ERR_ENTRY_DUPE, MB_ICONEXCLAMATION);
			continue;
		}
		OnDeleteEntryItem(posSrc);
		if (IsInSameVw(posSrc, posDst)) {
			OnAddEntryItem(posDst);
		} else {
			m_pEdCtx->DispatchGDUpdate(PM_V_UPDATE_TREE_ITEM, &posDst);
		}
		break;
	}
}

void CDGridCtrl::OnViewImage2()
{
	CCellID ci = GetFocusCell();
	CGDEntry pos = GetEntryItemID(Conv2Entry(ci.row));

	CFrameWnd *pFrame;
	if (pFrame = GetParentFrame()) {
		CView *pVw;
		if (pVw = pFrame->GetActiveView()) {
			CDM2GDEDDoc *pDoc;
			if (pDoc = DYNAMIC_DOWNCAST(CDM2GDEDDoc, pVw->GetDocument())) {
				CDM2EdPicFrame *pFrame = CDM2EdPicFrame::CreateNew(pDoc, pDoc->GetEdCtx(FALSE), pos);
			}
		}
	}
}

void CDGridCtrl::OnEditImage2()
{
	OnViewImage2();
	return;

	CCellID ci = GetFocusCell();
	CGDEntry pos = GetEntryItemID(Conv2Entry(ci.row));

//	CFrameWnd *pFrame;
//	if (pFrame = GetParentFrame()) {
//		CView *pVw;
//		if (pVw = pFrame->GetActiveView()) {
//			CDM2GDEDDoc *pDoc;
//			if (pDoc = DYNAMIC_DOWNCAST(CDM2GDEDDoc, pVw->GetDocument())) {
//				CDM2VwSceneFrame *pFrame = CDM2VwSceneFrame::CreateNew(pDoc);
//			}
//		}
//	}
}

void CDGridCtrl::OnEditImage()
{
#if 0
//	ASSERT(m_pEdCtx);
//
//	CGDAT &core = GetGDAT();
//
//	CCellID ci = GetFocusCell();
//	UINT iEntry = Conv2Entry(ci.row);
//	CGDEntry posSrc = GetEntryItemID(iEntry);
//	if (posSrc != CGDEntry::ERR) {
//		CEditImgDlg wndDlg(core.IsClassic(), core.IsLE(), this);
//		{
//			DMGHLiIMGM imgm;
//			HBITMAP hBM;
//			// EDITIMG
//			if (hBM = core.GetImgAt(posSrc, imgm)) {
//				wndDlg.Init(hBM, imgm);
//			} else {
//				wndDlg.Init();
//			}
//		}
//		int r = wndDlg.DoModal();
//		if (r != IDOK)
//			return;
//		VERIFY(core.SetRawDataFor(posSrc, wndDlg.GetNew1()));
//
//		UpdateSingleEntryContents(iEntry, &posSrc);
//	}
#endif
}

void CDGridCtrl::InvalidateAfterRow(UINT iRow)
{
	CRect rc;
	if (!GetCellRect(iRow, 0, &rc)) {
		iRow = GetRowCount() - 1;
		if (!GetCellRect(iRow, 0, &rc))
			return;
		rc.top = rc.bottom;
	}
	CRect rcMax;
	GetClientRect(&rcMax);
	rc.right = __max(rc.right, rcMax.right);
	rc.bottom = __max(rc.bottom, rcMax.bottom);
	InvalidateRect(&rc);
}

void CDGridCtrl::SetEntryItemCount(UINT nEntries)
{
	SetRowCount(1 + nEntries);
	SetFixedRowCount(1);
}

void CDGridCtrl::UpdateEntryContents(UINT iEntry, const CGDEntry *pposNew)
{
	UINT iRow = Conv2Row(iEntry);

	SetItemState(iRow, COLUMN_NAME, GVIS_READONLY);
	SetItemState(iRow, COLUMN_TYPE, GVIS_READONLY);
	SetItemState(iRow, COLUMN_VAL, GVIS_READONLY);
	SetItemState(iRow, COLUMN_DESC, GVIS_READONLY);

	CDCellEntryItem *pItem;
	if (!(pItem = GetEntryGenericContents(iEntry)))
		return;
	CGDEntry pos;
	if (pposNew) {
		pos = *pposNew;
		GetEntryItem(iEntry, COLUMN_NAME)->SetPos(pos);
		GetEntryItem(iEntry, COLUMN_TYPE)->SetPos(pos);
		GetEntryItem(iEntry, COLUMN_VAL)->SetPos(pos);
		GetEntryItem(iEntry, COLUMN_DESC)->SetPos(pos);

		GetEntryItem(iEntry, COLUMN_NAME)->SetCol(COLUMN_NAME);
		GetEntryItem(iEntry, COLUMN_TYPE)->SetCol(COLUMN_TYPE);
		GetEntryItem(iEntry, COLUMN_VAL)->SetCol(COLUMN_VAL);
		GetEntryItem(iEntry, COLUMN_DESC)->SetCol(COLUMN_DESC);
	} else {
		pos = pItem->GetPos();

		if (pos == CGDEntry::ERR) return;
	}

	CString strText;

	FormatEntryName(pos, strText);
	SetItemText(iRow, COLUMN_NAME, strText);

	FormatEntryType(pos, strText);
	SetItemText(iRow, COLUMN_TYPE, strText);

	const CDM2DescItem *pDesc;
	if (pDesc = m_pEdCtx->GetDescriptCtx().FindItem2(pos, m_pEdCtx->GetGDAT())) {
		SetItemText(iRow, COLUMN_DESC, pDesc->GetText());

		pItem->SetDynSel(pDesc->HasProg());
	}
	else {
		SetItemText(iRow, COLUMN_DESC, "");
	}

	BYTE c3 = pos.c3();

	switch (c3) {
	case 0x01:
		{
			SetCellType(iRow, COLUMN_VAL, RUNTIME_CLASS(CDCellImage));
			CDCellImage *pItemImg = GetEntryImageContents(iEntry);
			CDCellImageGetImgImpl *pGetImg;
			pItemImg->SetGetImage(pGetImg = new CDCellImageGetImgImpl(*m_pEdCtx, pos));
			pItemImg->SetPos(pos);
			pGetImg->Release();

			CSize size = pItemImg->GetImageSize();
			int nHeight = GetRowHeight(iRow);
			nHeight = __max(nHeight, 2 + size.cy);
			SetRowHeight(iRow, nHeight);
			m_ci.nWidth = __max(m_ci.nWidth, size.cx);
			break;
		}
	case 0x02:

	case 0x05:

	case 0x0B:
	case 0x0C:

	case 0x03:

	case 0x04:
	case 0x06:
	case 0x07: // ANYRAW
	case 0x08:
	case 0x09:
	case 0x0E:
	case 0x0D:
		{
			FormatEntryContents(pos, strText, pDesc);
			SetItemText(iRow, COLUMN_VAL, strText);

			CSize size = GetTextExtent(iRow, COLUMN_VAL, strText);
			m_ci.nWidth = __max(m_ci.nWidth, size.cx);
			break;
		}
	}
}

CDCellEntryItem *CDGridCtrl::GetEntryItem(UINT iEntry, UINT iCol)
{
	CDCellEntryItem *pCell = static_cast<CDCellEntryItem *>(GetCell(Conv2Row(iEntry), iCol));
	ASSERT_VALID(pCell);
	return pCell;
}

CDCellEntryItem *CDGridCtrl::GetEntryGenericContents(UINT iEntry)
{
	return GetEntryItem(iEntry, COLUMN_VAL);
}

CDCellImage *CDGridCtrl::GetEntryImageContents(UINT iEntry)
{
	CDCellImage *pCell = static_cast<CDCellImage *>(GetEntryGenericContents(iEntry));
	ASSERT_VALID(pCell);
	return pCell;
}

void CDGridCtrl::UpdateSingleEntryContents(UINT iEntry, const CGDEntry *pposNew)
{
	m_ci = CellProcessing();
	UpdateEntryContents(iEntry, pposNew);
	ReflectCellProcessing();

	InvalidateAfterRow(Conv2Row(iEntry));
}

void CDGridCtrl::DeleteEntryItem()
{
	if (!m_pEdCtx || IsRO()) return;

	CCellID ci = GetFocusCell();
	CGDEntry posSrc = GetEntryItemID(Conv2Entry(ci.row));

	if (posSrc == CGDEntry::ERR) return;

	CGDAT &core = GetGDAT();

	int r = AfxMessageBox(IDS_WARN_DELETE_ENTRY, MB_ICONEXCLAMATION|MB_YESNO);
	if (r != IDYES)
		return;

	if (core.DeleteEntry(posSrc)) {
		OnDeleteEntryItem(posSrc);
	}
}

void CDGridCtrl::MakeEntryContentsEmpty(UINT iEntry)
{
	UINT iRow = Conv2Row(iEntry);

	SetItemState(iRow, COLUMN_NAME, GVIS_READONLY);
	SetItemState(iRow, COLUMN_TYPE, GVIS_READONLY);
	SetItemState(iRow, COLUMN_VAL, GVIS_READONLY);
	SetItemState(iRow, COLUMN_DESC, GVIS_READONLY);
}

void CDGridCtrl::OnDblClkCell(int nRow, int nCol)
{
	if (!m_pEdCtx) return;

	ActivateEntry(GetEntryItemID(Conv2Entry(nRow)), nCol);
}

void CDGridCtrl::ActivateEntry(const CGDEntry &pos, int nCol)
{
	CGDAT &core = GetGDAT();

	if (nCol == COLUMN_NAME) {
		OnContextRenameEntry();
		return;
	}

	CEditVxxx &vxxx = GetVxxx(pos);

	if (vxxx.GetOp().GetWvct() == wvctChampStat) {
		OnPopupEditChampStat();
		return;
	}

	switch (pos.c3()) {
	case 0x01:
		{
			OnOpenImageExternally(false); //OnViewImage2();
			break;
		}
	case 0x05:
	case 0x0B:
	case 0x0C:
		{
			OnEditCtx2(pos.c3());
			break;
		}
	case 0x03:
	case 0x04:
	case 0x06:
	case 0x07: // ANYRAW
	case 0x08:
	case 0x09:
	case 0x0E:
	case 0x0D:
		{
			OnEditCtx(0x04);
			break;
		}
	case 0x02:
		{
			if (core.IsSndPCM()) {
				UINT iIdx = core.GetValueAt(pos);
				if (core.GetRawDataAt(iIdx, m_fvWav) && CreateRIFFWave(m_fvPlaySound)) {
					PlaySound(
						(LPCSTR)m_fvPlaySound.GetData(),
						NULL,
						SND_ASYNC|SND_MEMORY
						);
				}
			} else {
				OnEditCtx(0x04);
			}
			break;
		}
	}
}

CEditVxxx &CDGridCtrl::GetRawVxxx(const CGDEntry &pos)
{
	switch (pos.c3()) {
	case 0x05:
		return m_pEdCtx->GetVxxxFactory().GetVxxx(wvctText);
	}
	return m_pEdCtx->GetVxxxFactory().GetVxxx(m_aEdOpt.m_wvct);
}

CEditVxxx &CDGridCtrl::GetVxxx(const CGDEntry &pos)
{
	const CDM2DescItem *pItem;
	if (pItem = m_pEdCtx->GetDescriptCtx().FindItem2(pos, GetGDAT()))
		return pItem->GetVxxx();
	if (pos.c3() == 0x05) {
		// text
		return m_pEdCtx->GetVxxxFactory().GetVxxx(wvctText);
	}
	if (pos.c3() == 0x0c) {
		// text
		return m_pEdCtx->GetVxxxFactory().GetVxxx(wvctImageOffset);
	}
	return m_pEdCtx->GetVxxxFactory().GetVxxx(m_aEdOpt.m_wvct);
}

void CDGridCtrl::OnSysColorChange() 
{
	CGridCtrl::OnSysColorChange();
	
	SetGridBkColor(GetSysColor(COLOR_WINDOW));
}

void CDGridCtrl::OnPopupImportRaw() 
{
	ExportEntryItem(true, false, true);
}

void CDGridCtrl::OnPopupExportRaw() 
{
	ExportEntryItem(true, true, true);
}

void CDGridCtrl::ExportEntryItem(bool fViaFile, bool fExport, bool fForceRaw)
{
	if (!m_pEdCtx || IsRO()) return;

	CCellID ci = GetFocusCell();
	CGDAT &core = GetGDAT();
	int iRow = ci.row;
	CGDEntry posSrc = GetEntryItemID(Conv2Entry(iRow));

	I32 iIdx = core.GetValueAt(posSrc);
	if (iIdx < 0) return;

	LPCSTR pszExt = "bin";
	LPCSTR pszFilter = "bin (*.bin)|*.bin|all (*.*)|*.*|";
	BYTE c3 = posSrc.c3();
	if (!fForceRaw) {
		switch (c3) {
		case 1: pszExt = "bmp"; pszFilter = "*.bmp|*.bmp||"; break;
		case 2:
			c3 = 4;
			if (core.IsSndPCM())
				pszExt = "wav", c3 = 2, pszFilter = "*.wav|*.wav||";
			break;
		case 3: pszExt = "hmp", pszFilter = "*.hmp|*.hmp||"; break;
		}
	}

	CString strName;
	strName.Format("%04d.%s", iIdx, pszExt);

	CString strPath;

	TRY
		if (fViaFile) {
			if (fExport) {
				CFileDialog wndDlg(FALSE, pszExt, strName, nDefSaveFlags, pszFilter, this);
				int r = wndDlg.DoModal();
				if (r != IDOK) return;
				strPath = wndDlg.GetPathName();
			} else {
				CFileDialog wndDlg(TRUE, pszExt, NULL, nDefOpenFlags, pszFilter, this);
				int r = wndDlg.DoModal();
				if (r != IDOK) return;
				strPath = wndDlg.GetPathName();
			}
		}

		CDM2RCConv cc(core.IsLE(), core.GetGDRSDT());
		SizeBuff fv, fvNew;

		UINT cf = (c3 == 1) ? CF_DIB : CF_WAVE;
		UINT cf2 = (c3 == 1) ? CF_DIB : CF_RIFF;

		if (fExport) {
			if (!fForceRaw && c3 == 1) {
				HBITMAP hBM = core.GetImgAt(posSrc);
				if (!hBM) AfxThrowResourceException();

				CExtBM bm;
				bm.Attach(hBM);
				if (!bm.SaveTo(fvNew, fViaFile)) AfxThrowResourceException();
			} else {
				if (!core.GetRawDataAt(iIdx, fv)) AfxThrowResourceException();

				if (!fForceRaw && c3 == 2) {
					if (!cc.EncodeRIFFWave((BYTE *)fv.GetData(), fv.GetSize(), CDM2RCConvMIfSizeBuff(fvNew)))
						AfxThrowResourceException();
				} else {
					fvNew = fv;
				}
			}

			if (fViaFile) {
				CFile fileInto(strPath, CFile::modeWrite|CFile::modeCreate|CFile::shareDenyWrite);
				fileInto.Write(fvNew.GetData(), fvNew.GetSize());
				fileInto.Close();
			} else {
				CSharedFile f;
				f.Write(fvNew.GetData(), fvNew.GetSize());

				if (OpenClipboard()) {
					EmptyClipboard();
					SetClipboardData(cf, f.Detach());
					CloseClipboard();
				}
			}
			return;
		} else {
			if (fViaFile) {
				CFile fileIn(strPath, CFile::modeRead|CFile::shareDenyWrite);
				UINT cbIn = static_cast<UINT>(fileIn.GetLength());
				if (!fv.Alloc(cbIn) || fileIn.Read(fv.GetData(), cbIn) != cbIn) {
					AfxThrowResourceException();
				}
			} else {
				HANDLE hMem = NULL;
				if (OpenClipboard()) {
					hMem = GetClipboardData(cf);
					if (hMem == NULL)
						hMem = GetClipboardData(cf2);

					if (hMem) {
						BYTE *pbWav = (BYTE *)GlobalLock(hMem);
						DWORD cbWav = GlobalSize(hMem);

						fv.Alloc(cbWav, pbWav);

						GlobalUnlock(hMem);
					}
					VERIFY(CloseClipboard());
				}
				if (hMem == NULL) AfxThrowResourceException();
			}

			if (!fForceRaw && c3 == 1) {
				CExtBM bm;
				if (!bm.LoadFrom(fv.GetData(), fv.GetSize(), fViaFile)) {
					AfxMessageBox(IDS_ERR_FAILED_PASTE_IMAGE, 0 |MB_ICONEXCLAMATION);
					AfxThrowResourceException();
				}
				OnEditImageInternally(posSrc, bm);
			} else {
				if (!fForceRaw && c3 == 2) {
					if (!cc.DecodeRIFFWave((BYTE *)fv.GetData(), fv.GetSize(), CDM2RCConvMIfSizeBuff(fvNew)))
						AfxThrowResourceException();
				} else {
					fvNew = fv;
				}

				if (!core.SetRawDataFor(posSrc, fvNew)) {
					AfxThrowResourceException();
				}
			}

			UpdateSingleEntryContents(Conv2Entry(iRow));
			return;
		}
	CATCH_ALL(e)

	END_CATCH_ALL
}

void CDGridCtrl::OnPopupExportWav() 
{
	ExportEntryItem(true, true);
}

void CDGridCtrl::OnPopupImportWav() 
{
	ExportEntryItem(true, false);
}

void CDGridCtrl::OnPopupCopyPcm() 
{
	ExportEntryItem(false, true);
}

void CDGridCtrl::OnPopupPastePcm() 
{
	ExportEntryItem(false, false);
}

void CDGridCtrl::OnPopupPlayEntry() 
{
	if (!m_pEdCtx) return;

	ActivateEntry(GetEntryItemID(Conv2Entry(GetFocusCell().row)), GetFocusCell().col);
}

void CDGridCtrl::OnPopupViewImage() 
{
	OnViewImage2();
}

void CDGridCtrl::OnPopupExportBmp() 
{
	ExportEntryItem(true, true);
}

void CDGridCtrl::OnPopupImportBmp() 
{
	ExportEntryItem(true, false);
}

void CDGridCtrl::OnPopupPasteBmp() 
{
	ExportEntryItem(false, false);
}

void CDGridCtrl::OnPopupCopyBmp() 
{
	ExportEntryItem(false, true);
}

void CDGridCtrl::OnPopupEditImageIPF() 
{
	if (!m_pEdCtx || IsRO()) return;

	CCellID ci = GetFocusCell();
	CGDAT &core = GetGDAT();
	CGDEntry posSrc = GetEntryItemID(Conv2Entry(ci.row));

	DMGHLiPicIPF ipf;
	if (!core.GetPicIPF(posSrc, ipf)) return;

	CEditImgFlgsDlg wndDlg(this);

	wndDlg.m_nX = ipf.x;
	wndDlg.m_nY = ipf.y;
	wndDlg.m_fUse0c = ipf.fUse0c;

	int r = wndDlg.DoModal();
	if (r == IDOK) {
		ipf.x = wndDlg.GetX();
		ipf.y = wndDlg.GetY();

		if (!core.SetPicIPF(posSrc, ipf)) return;
	}
}

void CDGridCtrl::OnOpenImageExternally(bool fEdit)
{
	CCellID ci = GetFocusCell();
	CGDEntry pos = GetEntryItemID(Conv2Entry(ci.row));

	CGDAT &core = m_pEdCtx->GetGDAT();

	do {
		CExtBM bm;
		bm.Attach(core.GetImgAt(pos));

		if (bm.m_hObject == NULL) return;

		CString strPath = OSP_GetTempFile(".bmp");

		GetApp()->DeleteOnClose(strPath);

		{
			if (!bm.SaveToFile(strPath)) return;

			bm.DeleteObject();

			while (true) {
				HINSTANCE hInst = ShellExecute(GetActiveWindow()->GetSafeHwnd(), NULL, strPath, NULL, NULL, SW_SHOW);

				if ((DWORD)hInst < 32) break;

				if (fEdit) {
					CEditStandbyDlg wndDlg(this);
					int r = wndDlg.DoModal();
					if (r == IDOK) {
						bm.Attach((HBITMAP)::LoadImage(NULL, strPath, IMAGE_BITMAP, 0, 0, 0 |LR_CREATEDIBSECTION |LR_LOADFROMFILE |LR_VGACOLOR));

						if (bm.m_hObject) {
							TRY
								OnEditImageInternally(pos, bm);
							CATCH_ALL(e)

							END_CATCH_ALL
						} else {
							AfxMessageBox(IDS_ERR_FAILED_IMPORT_TEMP_BMP, 0 |MB_ICONEXCLAMATION);
						}
					}
					if (r == IDRETRY) {
						continue;
					}
				}
				return;
			}
			return;
		}

		AfxMessageBox(IDS_ERR_FAILED_OPEN_TEMP_BMP, 0 |MB_ICONEXCLAMATION);
		return;

	} while(false);

	AfxMessageBox(IDS_ERR_FAILED_EXPORT_TEMP_BMP, 0 |MB_ICONEXCLAMATION);
	return;
}

void CDGridCtrl::OnEditImageInternally(CGDEntry posSrc, CExtBM &bmSrc)
{
	CGDAT &core = m_pEdCtx->GetGDAT();

	SizeBuff fvNew;

	bool fLE = core.IsLE();
	UI32 nGDRIPT = core.GetGDRIPT();
	bool fIsFMTowns = core.IsFMTowns();

	DMGHLiPicIPF ipf;
	VERIFY(core.GetPicIPF(posSrc, ipf));

	bool fForce8 = false;

	CDM2NICtx ctx;
	ctx.fForce4 = !core.IsC8U8Allowed();
	ctx.fUse8 = !ctx.fForce4;
	ctx.DM2Pal = core.GetDM2Pal();
	CDM2NIMan man(m_pEdCtx->GetGDAT());
	VERIFY(man.Load(posSrc, ctx));

	if (256 <= ctx.iColorSrcKey || 256 <= ctx.iColorSrcKey2) {
		AfxMessageBox(IDS_ERR_COLORKEY_TOO_LARGE, 0 |MB_ICONEXCLAMATION);
		AfxThrowNotSupportedException();
	}
	if (16 <= ctx.iColorSrcKey || 16 <= ctx.iColorSrcKey2) {
		if (ctx.fForce4) {
			AfxMessageBox(IDS_ERR_COLORKEY_TOO_LARGE, 0 |MB_ICONEXCLAMATION);
			AfxThrowNotSupportedException();
		}
		fForce8 = true;
	}

	CExtBM &bm = ctx.bm;
	bm.Attach(bmSrc.Detach());
	if (!bm.m_hObject)
		AfxThrowResourceException();
	//if (!bm.LoadFrom(fv.GetData(), fv.GetSize(), fViaFile)) {
	//	AfxMessageBox(IDS_ERR_FAILED_PASTE_IMAGE, 0 |MB_ICONEXCLAMATION);
	//	AfxThrowResourceException();
	//}
	if (!ctx.Enter(0))
		AfxThrowResourceException();

	CDM2NIPS wndSheet(ctx, this);
	CDM2NI1Page wndPg1;
	CDM2NI2Page wndPg2;
	CDM2NI3Page wndPg3;
	CDM2NI4Page wndPg4;
	if (!ctx.fForce4 && !fForce8) wndSheet.AddPage(&wndPg1);
	if (0 <= ctx.iColorSrcKey) wndSheet.AddPage(&wndPg2);
	if (ctx.ps == ctx.psShould) wndSheet.AddPage(&wndPg3);

	if (wndSheet.GetPageCount() == 0) wndSheet.AddPage(&wndPg4);

	int r = wndSheet.DoModal();
	if (r != ID_WIZFINISH) AfxThrowNotSupportedException();

	VERIFY(ctx.Enter(3));

	DMGHLiIMGM &imgm = ctx.imgm[3];

	imgm.ipf = ipf;

	bool fRet = false;
	bool fRawPal = false;

	SizeBuff fvRawPal;
	CGDEntry posRawPal = posSrc;

	CDM2ImageComp ic(fLE);

	CWaitCursor wc;

	if (imgm.fIs8bpp) {
		CDGridCtrl_PH ph;
		ph.Create(this);

		CDM2ImageComp::DM2BMPImg8 img8(imgm);
		fRet = ic.MakeC8(img8, fvNew, ph);
	} else {
		CDM2ImageComp::DM2BMPImg4 img4(imgm);
		CDM2ImageComp::DM2Pal4Loc16 a16;
		CDM2ImageComp::DM2Pal4Loc6 a6;
		CDM2ImageComp::IdxScatter ais;
		ais.MakeFrom(img4);
		a16.MakeFrom(imgm.cPal);
		a6.MakeFrom(ais, a16);
		switch (nGDRIPT) {
		case GDRIPT_IMAGE:
			{
				fRet = ic.MakeC4(img4, a16, a6, fvNew);
				break;
			}
		case GDRIPT_RAW:
			{
				fRawPal = fvRawPal.Alloc(16, a16.iColorIdx);
				posRawPal.c3(0x0D);
			}
		case GDRIPT_SCENE:
			{
				if (fIsFMTowns) {
					CDM1ImageComp icv1(fLE);
					fRet = icv1.MakeI1(CDM1Pic(img4), fvNew);
				}
				else {
					fRet = ic.MakeC4(img4, a6, fvNew);
				}
				break;
			}
		}
		switch (nGDRIPT) {
		case GDRIPT_IMAGE:
		case GDRIPT_SCENE:
			{
				if (ctx.fKeepPal) break;
				if (ctx.posColorPal != CGDEntry::ERR && (fRawPal = fvRawPal.Alloc(64))) {
					UI8 *p = fvRawPal.GetData();
					for (int i = 0; i < 16; i++, p += 4)
						p[0] = i,
						p[1] = GetRValue(imgm.cPal.p.f.color[i]),
						p[2] = GetGValue(imgm.cPal.p.f.color[i]),
						p[3] = GetBValue(imgm.cPal.p.f.color[i]);
					posRawPal = ctx.posColorPal;
				}
				break;
			}
		}
	}

	if (!fRet) AfxThrowNotSupportedException();

	if (false
		|| !core.SetRawDataFor(posSrc, fvNew)
		|| (fRawPal && !core.MakeBinEntry(posRawPal, fvRawPal))
	) {
		AfxThrowResourceException();
	}
}

void CDGridCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CGridCtrl::OnSetFocus(pOldWnd);
	
	CCmdTarget *p;
	if (p = GetParentFrame()) {
		p->OnCmdMsg(ID_UPDATE_CURRENT_PANE, 0, NULL, NULL);
	}
	
}

void CDGridCtrl::OnPopupEditChampStat() 
{
	CCellID ci = GetFocusCell();
	UINT iEntry = Conv2Entry(ci.row);
	CGDEntry pos = GetEntryItemID(iEntry);

	CGDAT &core = m_pEdCtx->GetGDAT();
	I32 nVal = core.GetValueAt(pos);

	SizeBuff fvNew;

	bool fLE = core.IsLE();

	if (core.GetRawDataAt(nVal, fvNew)) {
		CEditHeroStatDlg wndDlg(core.IsLE(), this);
		size_t size = __min(sizeof(wndDlg.m_bData), static_cast<size_t>(fvNew.GetSize()));
		memcpy(wndDlg.m_bData, fvNew.GetData(), size);
		if (wndDlg.DoModal() == IDOK) {
			memcpy(fvNew.GetData(), wndDlg.m_bData, size);
			core.SetRawDataFor(pos, fvNew);
		}
	}
}

void CDGridCtrl::OnPopupCopySeled() 
{
	CString buff = ";;;; DM2GDED io format\r\n";

	CGDAT &core = m_pEdCtx->GetGDAT();

	for (int iEntry=0; iEntry<GetEntryItemCount(); iEntry++) {
		if (!IsEntryItemChecked(iEntry)) continue;

		CGDEntry pos = GetEntryItemID(iEntry);

		switch (pos.c3()) {
		case 0x0B: case 0x0C:
			{
				CString temp;

				temp.Format("(addWordVal #x%02X #x%02X #x%02X #x%02X #x%02X #x%02X #x%04X)\r\n"
					, 0U +pos.c1()
					, 0U +pos.c2()
					, 0U +pos.c3()
					, 0U +pos.c3()
					, 0U +pos.c5()
					, 0U +pos.c6()
					, 0U +core.GetValueAt(pos)
					);
				buff += temp;
				break;
			}
		case 0x00: case 0x03: case 0x04: case 0x06: case 0x07: case 0x08: case 0x09: case 0x0D: case 0x0E:
			{
				SizeBuff fv;
				if (core.GetRawDataAt(core.GetValueAt(pos), fv)) {
					CString buffRawData;
					const UI8 *pb = fv.GetData();
					for (int x=0; x<fv.GetSize(); x++) {
						char c[100];
						_snprintf(c, sizeof(c), "%02X", 0U +pb[x]);
						buffRawData += c;
						//if (0 != x && (x & 31) == 0) buffRawData += "\r\n";
					}

					CString temp;
					temp.Format("(addRawData #x%02X #x%02X #x%02X #x%02X #x%02X #x%02X \"%s\")\r\n"
						, 0U +pos.c1()
						, 0U +pos.c2()
						, 0U +pos.c3()
						, 0U +pos.c3()
						, 0U +pos.c5()
						, 0U +pos.c6()
						, (LPCTSTR)buffRawData
						);

					buff += temp;
				}
				break;
			}
		case 0x01:
			{
				SizeBuff fv;
				DMGHLiIMGM pic;
				if (core.GetImgAt(pos, pic)) {
					CString strPal;
					if (!pic.fIs8bpp) {

					}
					CString strPic;
					CString temp;
					temp.Format("(addPicData #x%02X #x%02X #x%02X #x%02X #x%02X #x%02X %d %d %d \"%s\" \"%s\")\r\n"
						, 0U +pos.c1()
						, 0U +pos.c2()
						, 0U +pos.c3()
						, 0U +pos.c3()
						, 0U +pos.c5()
						, 0U +pos.c6()
						, 0U +pic.cx
						, 0U +pic.cy
						, 0U +(pic.fIs8bpp ? 8 : 4)
						, (LPCTSTR)strPal
						, (LPCTSTR)strPic
						);
					buff += temp;
				}
				break;
			}
		}
	}

	{
		CSharedFile f;
		f.Write(buff, (buff.GetLength() +1) * sizeof(TCHAR));

		if (OpenClipboard()) {
			EmptyClipboard();
			SetClipboardData(CF_TEXT, f.Detach());
			CloseClipboard();
		}
	}
}

void CDGridCtrl::OnPopupPasteItems() 
{
}

bool CDGridCtrl::IsEntryItemChecked(UINT iEntry)
{
	return false;
}
