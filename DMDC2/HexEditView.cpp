// HexEditView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "HexEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHexEditView

IMPLEMENT_DYNCREATE(CHexEditView, CView)

CHexEditView::CHexEditView()
{
	m_cb = NULL;
}

CHexEditView::~CHexEditView()
{

}

BEGIN_MESSAGE_MAP(CHexEditView, CView)
	//{{AFX_MSG_MAP(CHexEditView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_COMMAND(ID_FILE_APPLY, OnFileApply)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHexEditView 描画

void CHexEditView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	DoDraw(*pDC);
}

static const BYTE g_cHex256Tbl[] = {
	"000102030405060708090A0B0C0D0E0F"
	"101112131415161718191A1B1C1D1E1F"
	"202122232425262728292A2B2C2D2E2F"
	"303132333435363738393A3B3C3D3E3F"
	"404142434445464748494A4B4C4D4E4F"
	"505152535455565758595A5B5C5D5E5F"
	"606162636465666768696A6B6C6D6E6F"
	"707172737475767778797A7B7C7D7E7F"
	"808182838485868788898A8B8C8D8E8F"
	"909192939495969798999A9B9C9D9E9F"
	"A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
	"B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
	"C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
	"D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
	"E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
	"F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF"
};

void CHexEditView::DoDraw(CDC &dc)
{
	CRect rcPaint = static_cast<CPaintDC &>(dc).m_ps.rcPaint;

	const int cxText = zeroTo1(m_tmText.tmAveCharWidth);
	const int cyText = zeroTo1(m_tmText.tmInternalLeading + m_tmText.tmHeight + m_tmText.tmExternalLeading);
	int y0 = m_tmText.tmInternalLeading + m_tmText.tmAscent;
	dc.SaveDC();
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextAlign(TA_LEFT|TA_BASELINE);
	HGDIOBJ hOldFont = dc.SelectObject(m_fontText.m_hObject);
	//
	int tx = -m_rcVp.left * cxText;
	// 上段をば
	dc.SetTextColor((COLORREF)0xffffff);
	dc.TextOut(tx, y0, " ADDRESS   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F   0123456789ABCDEF");
	y0 += cyText;
	// 左段をば
	dc.SetTextColor((COLORREF)0xffffff);
	dc.SetBkColor((COLORREF)0xffffff);
	int nLb = __max(1, rcPaint.Height() / cyText + 1);
	CString tstrAddr;
	const BYTE *pbPos = m_arrData.GetData();
	const BYTE *pbEnd = pbPos + m_arrData.GetSize();
	const BYTE *pbHorz = pbPos + m_iOffset, *pbWalk1, *pbWalk2;
	UINT iOffset = m_iOffset;
	int nWalk1, nWalk2, nLastWalk = 1;
	int x0 = 11 * cxText;
	for (int iLb = 0; iLb < nLb; iLb++) {
		UINT iOff = pbHorz - pbPos;
		pbWalk1 = pbWalk2 = pbHorz;
		nWalk1 = nWalk2 = __min(16, (pbWalk1 < pbEnd) ? pbEnd - pbWalk1 : 0);

		if (!((nWalk1 != 0) || (nWalk1 == 0 && nLastWalk == 16))) break;

		tstrAddr.Format(" %08X", (UINT)(iOffset));
		dc.TextOut(tx, y0, tstrAddr);

		if (nWalk1 == 0) break;

		nLastWalk = nWalk1;

		BOOL bSelMask[16];
		BOOL bSel = FALSE;

		TCHAR tcHex[48];
		tcHex[0] = 0;
		int iWalk1;
		for (iWalk1 = 0; iWalk1 < nWalk1; iWalk1++, pbWalk1++) {
			tcHex[iWalk1*3+0] = g_cHex256Tbl[(UINT)*pbWalk1*2+0];
			tcHex[iWalk1*3+1] = g_cHex256Tbl[(UINT)*pbWalk1*2+1];
			tcHex[iWalk1*3+2] = ' ';

			UINT iMin = __min(m_iCursor, m_iSelFrom);
			UINT iMax = __max(m_iCursor, m_iSelFrom);
			UINT iCur = iOff + iWalk1;
			bSelMask[iWalk1] = (iMin <= iCur) && (iCur < iMax);
		}
		if (iWalk1 != 0) tcHex[iWalk1*3-1] = 0;
		dc.TextOut(tx + x0, y0, tcHex, strlen(tcHex));

		TCHAR tcAsc[17];
		int iWalk2;
		for (iWalk2 = 0; iWalk2 < nWalk2; iWalk2++, pbWalk2++) {
			if (isprint(*pbWalk2)) {
				tcAsc[iWalk2] = *pbWalk2;
			} else {
				tcAsc[iWalk2] = '.';
			}
		}
		tcAsc[iWalk2] = 0;
		dc.TextOut(tx + x0 + cxText * 50, y0, tcAsc);

		int iMask;
		for (iMask = 0; iMask < nWalk1; iMask++) {
			if (bSelMask[iMask]) {
				int iMask2;
				for (iMask2 = iMask; iMask2 < nWalk1 && bSelMask[iMask2]; iMask2++);
				CRect rcInv;
				rcInv.left = tx + cxText * (11 + 3 * iMask);
				rcInv.top = m_rcWork.top + cyText * iLb;
				rcInv.right = tx + cxText * (11 + 3 * iMask2 - 1);
				rcInv.bottom = rcInv.top + cyText;
				rcInv.DeflateRect(0, m_tmText.tmInternalLeading, 0, m_tmText.tmExternalLeading);
				dc.InvertRect(rcInv);

				rcInv.left = tx + x0 + cxText * (50 + iMask);
				rcInv.top = m_rcWork.top + cyText * iLb;
				rcInv.right = tx + x0 + cxText * (50 + iMask2);
				rcInv.bottom = rcInv.top + cyText;
				rcInv.DeflateRect(0, m_tmText.tmInternalLeading, 0, m_tmText.tmExternalLeading);
				dc.InvertRect(rcInv);

				iMask = iMask2;
			}
		}

		y0 += cyText;
		iOffset += 16;
		pbHorz += 16;
	}
	//
	dc.SelectObject(hOldFont);
	dc.RestoreDC(-1);
}

/////////////////////////////////////////////////////////////////////////////
// CHexEditView 診断

#ifdef _DEBUG
void CHexEditView::AssertValid() const
{
	CView::AssertValid();
}

void CHexEditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHexEditView メッセージ ハンドラ

void CHexEditView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	LOGFONT lfText;
	ReadFontTemplateFrom(lfText, IDS_FONTTEMP_DMHEXEDIT, "Courier New;16;8;;;;;;;1;;;2;1");
	m_fontText.DeleteObject();
	m_fontText.CreateFontIndirect(&lfText);

	m_iOffset = 0;
	m_arrData.RemoveAll();
	m_iCursor = m_iSelFrom = 0;
	m_bCaretState = FALSE;
	m_iLaterBit = 0;
	m_bOverwrite = FALSE;
	m_bChrPane = 0;

	{
		ZeroMemory(&m_tmText, sizeof(m_tmText));
		CClientDC dc(this);
		HGDIOBJ hOldObj = dc.SelectObject(m_fontText.m_hObject);
		dc.GetTextMetrics(&m_tmText);
		dc.SelectObject(hOldObj);
	}

	m_rcVp.SetRectEmpty();
	m_rcVpRange.SetRect(0, 0, 77, 0);

	BinaryResize();
	Resize();
	RefreshCaretPos();
}

BOOL CHexEditView::OnEraseBkgnd(CDC* pDC) 
{
	CDC &dc = *pDC;

	int i = dc.SaveDC();
	dc.FillSolidRect(m_rcBarTop, RGB(0, 0, 128));
	dc.ExcludeClipRect(m_rcBarTop);
	dc.FillSolidRect(m_rcBarLeft, RGB(0, 128, 128));
	dc.ExcludeClipRect(m_rcBarLeft);
	dc.FillSolidRect(m_rcWork, RGB(0, 0, 0));
	dc.RestoreDC(i);
	return 1;

	return CView::OnEraseBkgnd(pDC);
}

void CHexEditView::ActivateCaret(BOOL bShow)
{
	if (bShow) {
		if (m_bCaretState)
			DestroyCaret();

		CreateSolidCaret(
			m_bOverwrite ? (m_tmText.tmAveCharWidth - 1) : 2, 
			m_tmText.tmHeight);
		ShowCaret();
	} else {
		DestroyCaret();
	}
	if (m_bCaretState = bShow) {
		RefreshCaretPos();
	}
}

void CHexEditView::RefreshCaretPos()
{
	int iOff = (int)(m_iCursor - m_iOffset);
	int x = iOff & 15;
	int y = iOff / 16;

	if (m_rcVp.Height() != 0 && (y < 0 || y >= m_rcVp.Height())) {
		ActivateCaret(FALSE);
		return;
	} else {
		if (!m_bCaretState)
			ActivateCaret(TRUE);
	}

	int nHeight = m_tmText.tmHeight + m_tmText.tmInternalLeading + m_tmText.tmExternalLeading;
	int nWidth = m_tmText.tmAveCharWidth;
	int nLead = m_tmText.tmInternalLeading;
	CPoint point;
	if (m_bChrPane) {
		point.x = nWidth * (51 + x);
	} else {
		point.x = nWidth * (1 + x * 3 + m_iLaterBit);
	}
	point.x+= (10 - m_rcVp.left) * nWidth;
	point.y = m_rcWork.top + nHeight * (y) + nLead;

	SetCaretPos(point);
}

void CHexEditView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	Resize();
}

void CHexEditView::Resize()
{
	CRect rc;
	GetClientRect(&rc);

	for (int i=0; i<2; i++) {
		int tx = -m_rcVp.left * m_tmText.tmAveCharWidth;
		int nHeight = m_tmText.tmHeight + m_tmText.tmInternalLeading + m_tmText.tmExternalLeading;
		int nWidth = m_tmText.tmAveCharWidth;
		m_rcBarTop.SetRect(
			rc.left,
			rc.top,
			rc.right,
			rc.top + nHeight
			);
		m_rcBarLeft.SetRect(
			rc.left + tx,
			m_rcBarTop.top,
			rc.left + tx + nWidth * 10,
			rc.bottom
			);
		m_rcBarLeft.left = __max(0, m_rcBarLeft.left);
		m_rcBarLeft.right = __max(0, m_rcBarLeft.right);
		m_rcWork.SetRect(
			m_rcBarLeft.right,
			m_rcBarTop.bottom,
			rc.right,
			rc.bottom
			);

		RecalcVisibleRange();
		RecalcScroll();
	}

	RefreshCaretPos();
}

void CHexEditView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar) {
	case VK_DELETE:
		if (KillSelected()) {
			m_iLaterBit = 0;
			BinaryResize();
			RefreshCaretPos();
		} else if (m_iCursor + 1 <= GetDataSize()) {
			m_arrData.RemoveAt(m_iCursor);
			BinaryResize();

			if (m_iLaterBit) {
				m_iLaterBit = 0;
				RefreshCaretPos();
			}
		}
		return;
	case VK_BACK:
		if (KillSelected()) {
			m_iLaterBit = 0;
			BinaryResize();
			RefreshCaretPos();
		} else if (m_iCursor != 0 && m_iCursor <= GetDataSize()) {
			m_arrData.RemoveAt(m_iCursor - 1);
			BinaryResize();

			m_iCursor--;
			m_iSelFrom = m_iCursor;
			m_iLaterBit = 0;
			RefreshCaretPos();
		}
		return;
	case VK_TAB:
		m_bChrPane = m_bChrPane ? 0 : 1;
		m_iLaterBit = 0;
		RefreshCaretPos();
		SetViewportVisible();
		return;
	case VK_INSERT:
		m_bOverwrite = m_bOverwrite ? 0 : 1;
		ActivateCaret(TRUE);
		return;
	}
	int iSelRange = m_iCursor - m_iSelFrom;
	BOOL bShift = (GetKeyState(VK_SHIFT) & 0x8000) ? 1 : 0;
	BOOL bControl = (GetKeyState(VK_CONTROL) & 0x8000) ? 1 : 0;
	int iOffset = m_iCursor;
	int iLaterBit = m_iLaterBit;
	BOOL bOther = FALSE;
	switch (nChar) {
	case VK_UP:
		iOffset -= 16; iLaterBit = 0;
		break;
	case VK_DOWN:
		iOffset += 16; iLaterBit = 0;
		break;
	case VK_LEFT:
		iOffset--;
		iLaterBit = 0;
		break;
	case VK_RIGHT:
		iOffset++;
		iLaterBit = 0;
		break;
	case VK_HOME:
		if (bControl)
				iOffset = 0;
		else	iOffset = iOffset & (~15);
		iLaterBit = 0;
		break;
	case VK_END:
		if (bControl)
				iOffset = INT_MAX;
		else	iOffset = iOffset | 15;
		iLaterBit = 0;
		break;
	case VK_PRIOR:
		iOffset -= 16 * m_rcVp.Height();
		break;
	case VK_NEXT:
		iOffset += 16 * m_rcVp.Height();
		break;
	case 'A':
		if (bControl) {
			m_iCursor = 0;
			m_iSelFrom = GetDataSize();
			InvalidateRect(m_rcWork);
			SetViewportVisible();
			RefreshCaretPos();
			return;
		}
	default:
		bOther = TRUE;
	}
	if (!bOther) {
		iOffset = __min(iOffset, (int)GetDataSize());
		iOffset = __max(iOffset, 0);
		if (!bShift)
			m_iSelFrom = iOffset;
		int iNewSelRange = iOffset - m_iSelFrom;
		if (iSelRange != iNewSelRange) {
			InvalidateRect(m_rcWork);
		}
		if (m_iCursor != iOffset || m_iLaterBit != iLaterBit) {
			m_iCursor = iOffset;
			m_iLaterBit = iLaterBit;
			SetViewportVisible();
			RefreshCaretPos();
		}
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CHexEditView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	BOOL bControl = (GetKeyState(VK_CONTROL) & 0x8000) ? 1 : 0;
	if (bControl) {
		return;
	}
	if (m_bChrPane) {
		switch (nChar) {
		case VK_TAB:
		case VK_BACK:
			return;
		}

		int w = 1;
		BYTE c = nChar;
		BOOL bKilled = KillSelected();
		if (GetDataSize() <= m_iCursor) {
			m_arrData.Add(c);
			m_iCursor++;
		} else {
			if (m_bOverwrite && !bKilled) {
				m_arrData.SetAt(m_iCursor, c);
				m_iCursor++;
			} else {
				m_arrData.InsertAt(m_iCursor, c);
				m_iCursor++;
			}
		}
		if (w) {
			m_iSelFrom = m_iCursor;
			BinaryResize();
			SetViewportVisible();
			RefreshCaretPos();
		}
		return;
	}
	int v = -1;
	switch (nChar) {
	case '0': v=0x0; break;
	case '1': v=0x1; break;
	case '2': v=0x2; break;
	case '3': v=0x3; break;
	case '4': v=0x4; break;
	case '5': v=0x5; break;
	case '6': v=0x6; break;
	case '7': v=0x7; break;
	case '8': v=0x8; break;
	case '9': v=0x9; break;
	case 'A': case 'a': v=0xA; break;
	case 'B': case 'b': v=0xB; break;
	case 'C': case 'c': v=0xC; break;
	case 'D': case 'd': v=0xD; break;
	case 'E': case 'e': v=0xE; break;
	case 'F': case 'f': v=0xF; break;
	}
	if (v != -1) {
		int w = 0;
		BOOL bKilled = KillSelected();
		if (GetDataSize() <= m_iCursor) {
			m_arrData.Add(v << 4);
			m_iLaterBit = 1; w = 1;
		} else {
			BYTE &c = m_arrData[m_iCursor];
			if (m_bOverwrite && !bKilled) {
				if (m_iLaterBit) {
					c = (c & 0xF0) | v;
					m_iLaterBit = 0;
					m_iCursor++;
				} else {
					c = (c & 0x0F) |(v << 4);
					m_iLaterBit = 1;
				}
			} else {
				if (m_iLaterBit) {
					c = (c & 0xF0) | v;
					m_iLaterBit = 0;
					m_iCursor++;
				} else {
					m_arrData.InsertAt(m_iCursor, v << 4);
					m_iLaterBit = 1;
				}
			}
			w++;
		}
		if (w) {
			m_iSelFrom = m_iCursor;
			BinaryResize();
			SetViewportVisible();
			RefreshCaretPos();
		}
	}
	
	CView::OnChar(nChar, nRepCnt, nFlags);
}

void CHexEditView::OnFileApply() 
{
	if (m_cb)
		m_cb->OnApplyHexEditCB(m_nCookie);
}

void CHexEditView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	LONG x = m_rcVp.left, y = m_rcVp.top;
	switch (nSBCode) {
	case SB_LEFT:
		x = m_rcVpRange.left;
		break;
	case SB_LINELEFT:
		x--;
		break;
	case SB_PAGELEFT:
		x -= m_rcVp.Width();
		break;
	case SB_PAGERIGHT:
		x += m_rcVp.Width();
		break;
	case SB_LINERIGHT:
		x++;
		break;
	case SB_RIGHT:
		x = m_rcVpRange.right;
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		x = nPos;
		break;
	}
	SetScroll(x, y);

	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CHexEditView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	LONG x = m_rcVp.left, y = m_rcVp.top;
	switch (nSBCode) {
	case SB_LEFT:
		y = m_rcVpRange.top;
		break;
	case SB_LINELEFT:
		y--;
		break;
	case SB_PAGELEFT:
		y -= m_rcVp.Height();
		break;
	case SB_PAGERIGHT:
		y += m_rcVp.Height();
		break;
	case SB_LINERIGHT:
		y++;
		break;
	case SB_RIGHT:
		y = m_rcVpRange.bottom;
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		y = nPos;
		break;
	}
	SetScroll(x, y);

	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CHexEditView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= WS_HSCROLL|WS_VSCROLL;
	
	return CView::PreCreateWindow(cs);
}

void CHexEditView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_iCursor != m_iSelFrom);
}

void CHexEditView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	OnUpdateEditCopy(pCmdUI);
}

void CHexEditView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CHexEditView::OnEditCopy() 
{
	CString tstr, tstrTemp;
	UINT iMin = __min(m_iCursor, m_iSelFrom);
	UINT iMax = __max(m_iCursor, m_iSelFrom);
	int i = 0;
	for (; iMin != iMax; iMin++, i++) {
		tstrTemp.Format((i == 0) ? "%02X" : " %02X", (UINT)m_arrData[iMin]);
		tstr += tstrTemp;
	}
	CSharedFile f;
	f.Write((LPCTSTR)tstr, tstr.GetLength());
	f.Write("\0", 1);
	if (OpenClipboard()) {
		SetClipboardData(CF_TEXT, f.Detach());
		CloseClipboard();
	}
}

void CHexEditView::OnEditCut() 
{
	OnEditCopy();

	if (KillSelected()) {
		BinaryResize();
		RefreshCaretPos();
	}
}

void CHexEditView::OnEditPaste() 
{
	CString tstr;
	if (OpenClipboard()) {
		HANDLE hMem = GetClipboardData(CF_TEXT);
		if (hMem) {
			UINT cb = GlobalSize(hMem);
			LPTSTR lpsz = tstr.GetBuffer(cb + 1);
			memcpy(lpsz, GlobalLock(hMem), cb);
			VERIFY(GlobalUnlock(hMem));
			lpsz[cb] = 0;
			tstr.ReleaseBuffer(-1);
		}
		CloseClipboard();
		if (!tstr.IsEmpty()) {
			CByteArray arr;
			LPCTSTR lpszPos = tstr;
			LPCTSTR lpszEnd = _tcschr(tstr, 0);
			while (lpszPos + 2 <= lpszEnd) {
				if (isxdigit(lpszPos[0]) && isxdigit(lpszPos[1]) && (isspace(lpszPos[2]) || lpszPos[2] == 0)) {
					char c[3];
					c[0] = lpszPos[0];
					c[1] = lpszPos[1];
					c[2] = 0;
					char *pc;
					UINT i = strtol(c, &pc, 16);
					arr.Add(i);
				} else {
					break;
				}
				lpszPos += 3;
			}
			KillSelected();
			int iPaste, nPastes;
			for (iPaste = 0, nPastes = arr.GetSize(); iPaste < nPastes; iPaste++) {
				m_arrData.InsertAt(m_iCursor, arr[iPaste]);
				m_iCursor++;
			}
			if (nPastes) {
				m_iSelFrom = m_iCursor;
				BinaryResize();
				RefreshCaretPos();
			}
		}
	}
}
