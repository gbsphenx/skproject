// DMLvSelWnd2.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Exp00069.h"
#include "DMLvSelWnd2.h"
#include "CompatDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDMLvSelWnd2

CDMLvSelWnd2::CDMLvSelWnd2()
{
}

CDMLvSelWnd2::~CDMLvSelWnd2()
{
}


BEGIN_MESSAGE_MAP(CDMLvSelWnd2, CControlBar)
	//{{AFX_MSG_MAP(CDMLvSelWnd2)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDMLvSelWnd2 メッセージ ハンドラ

int CDMLvSelWnd2::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	LOGFONT lfNice;
	ReadFontTemplateFrom(lfNice, IDS_FONTTEMP_DMLVSEL, ";;;;;;;;;;;;;;");
	m_fontNice.DeleteObject();
	if (!m_fontNice.CreateFontIndirect(&lfNice))
		return -1;

	m_fHorz = true;
	m_iOffset = m_nItems = m_nUserItems = m_nBoxItems = 0;
	m_iSel = -1;

	Recalc();

	return 0;
}

void CDMLvSelWnd2::OnDestroy() 
{
	CControlBar::OnDestroy();
	
	m_fontNice.DeleteObject();
	
}

void CDMLvSelWnd2::Recalc()
{
	{
		CCompatDC dc(this);
		CFont *pOld = dc.GetCurrentFont();
		dc.SelectObject(&m_fontNice);
		dc.GetTextMetrics(&m_tm);
		dc.SelectObject(pOld);
	}

	int cx = m_tm.tmAveCharWidth * 3;
	int cy = m_tm.tmHeight + m_tm.tmInternalLeading + m_tm.tmExternalLeading;

	cx = __max(cx, cy);

	m_cxBorder = GetSystemMetrics(SM_CXEDGE);

	m_cxBox = 1 + cx + 1;

	m_sizeBox.cx = 1 + 1 + m_cxBox + 1;
	m_sizeBox.cy = m_sizeBox.cx + 1;

	Resized();
}

void CDMLvSelWnd2::Resized()
{
	GetClientRect(&m_rcWc);
	m_rcBorder = m_rcWc;
	m_rcBorder.DeflateRect(m_cxBorder, m_cxBorder);
	m_rcUse = m_rcBorder;
	m_rcUse.DeflateRect(1, 1);

	Reassign();
}

void CDMLvSelWnd2::Reassign()
{
	CRect rc;
	rc.right = (rc.left = m_rcUse.left) + (m_fHorz ? m_sizeBox.cx : m_sizeBox.cy);
	rc.bottom = (rc.top = m_rcUse.top) + (m_fHorz ? m_sizeBox.cy : m_sizeBox.cx);
	CPoint ptStep(
		m_fHorz ? m_sizeBox.cx : 0,
		m_fHorz ? 0 : m_sizeBox.cx
		);

	int x = 0;
	int cx = m_fHorz ? m_rcUse.Width() : m_rcUse.Height();

	m_nBoxItems = ((cx - 1) / m_sizeBox.cx);
	m_nUserItems = _MAX(0, m_nBoxItems - 4);

	m_iOffset = ValidateOffset(m_iOffset);

	m_list.RemoveAll();

	m_list.AddTail(Obj(otyLL, rc)); rc += ptStep;
	m_list.AddTail(Obj(otyL, rc)); rc += ptStep;

	int i;
	CString str;
	for (i = 0; i < m_nUserItems && m_iOffset+i < m_nItems; i++) {
		str.Format("%d", m_iOffset+i);
		m_list.AddTail(Obj(otyBox, rc, str, m_iOffset + i)); rc += ptStep;
	}

	CPoint pt2(
		m_fHorz ? (m_rcUse.right - 2 * ptStep.x - rc.left - 1) : 0,
		m_fHorz ? 0 : (m_rcUse.bottom - 2 * ptStep.y - rc.top - 1)
		);

	rc += pt2;

	m_list.AddTail(Obj(otyR, rc)); rc += ptStep;
	m_list.AddTail(Obj(otyRR, rc)); rc += ptStep;
}

void CDMLvSelWnd2::OnSize(UINT nType, int cx, int cy) 
{
	CControlBar::OnSize(nType, cx, cy);
	
	Resized();
	
}

void CDMLvSelWnd2::OnPaint() 
{
	CPaintDC dc(this);
	
	CBrush br, br2;
	br.CreateSysColorBrush(COLOR_BTNFACE);
	dc.FillRect(m_rcWc, &br);
	br2.CreateSolidBrush(RGB(0, 0, 0));
	dc.FillRect(m_rcBorder, &br2);
	dc.Draw3dRect(m_rcBorder, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNHIGHLIGHT));

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255,255,255));
	dc.SelectObject(&m_fontNice);

	CBrush brWhite;
	brWhite.CreateStockObject(WHITE_BRUSH);
	POSITION pos = m_list.GetHeadPosition();
	int iIdx = 0;
	while (pos) {
		Obj &r = m_list.GetNext(pos);
		CRect rc = r.rc;
		rc.DeflateRect(1, 1, m_fHorz ? 0 : 1, m_fHorz ? 1 : 0);

		CRect rcFrame = rc;
		dc.FrameRect(&rcFrame, &brWhite);

		bool fSel = false;

		switch (r.oty) {
		case otyL:
		case otyLL:
		case otyRR:
		case otyR:
			{
				LPCTSTR pszMarks = "";
				switch (r.oty) {
				case otyL: pszMarks = m_fHorz ? "<-" : "A|"; break;
				case otyLL: pszMarks = m_fHorz ? "<<" : "AA"; break;
				case otyRR: pszMarks = m_fHorz ? ">>" : "VV"; break;
				case otyR: pszMarks = m_fHorz ? "->" : "|V"; break;
				}
				if (m_fHorz) {
					dc.DrawText(pszMarks, rc, 0 |DT_CENTER |DT_VCENTER |DT_SINGLELINE);
				} else {
					int cy = m_tm.tmHeight;
					rc -= CPoint(0, cy/6);
					dc.DrawText(&pszMarks[0], 1, rc, 0 |DT_CENTER |DT_VCENTER |DT_SINGLELINE);
					rc += CPoint(0, cy/3);
					dc.DrawText(&pszMarks[1], 1, rc, 0 |DT_CENTER |DT_VCENTER |DT_SINGLELINE);
				}
				break;
			}
		case otyBox:
			{
				dc.DrawText(r.strText, rc, 0 |DT_CENTER |DT_VCENTER |DT_SINGLELINE);
				fSel = true;
				break;
			}
		}
		if (fSel) {
			if (r.iIdx == m_iSel) {
				rcFrame.DeflateRect(2, 2);
				dc.InvertRect(rcFrame);
			}
			iIdx++;
		}
	}
}

void CDMLvSelWnd2::OnLButtonDown(UINT nFlags, CPoint point) 
{
	POSITION pos = m_list.GetHeadPosition();
	while (pos) {
		Obj &r = m_list.GetNext(pos);
		if (r.rc.PtInRect(point)) {
			int iOffset = m_iOffset;
			bool fNotify = false;
			switch (r.oty) {
			case otyBox:
				m_iSel = r.iIdx;
				fNotify = true;
				break;
			case otyL:
				iOffset--;
				break;
			case otyLL:
				iOffset -= m_nUserItems;
				break;
			case otyRR:
				iOffset += m_nUserItems;
				break;
			case otyR:
				iOffset++;
				break;
			}
			iOffset = ValidateOffset(iOffset);

			if (iOffset != m_iOffset) {
				m_iOffset = iOffset;
				Reassign();
				Invalidate();
			}
			if (fNotify) {
				Invalidate();
				SendNotifyToParent(0);
			}
			return;
		}
	}
	
	CControlBar::OnLButtonDown(nFlags, point);
}

CSize CDMLvSelWnd2::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	int cy = 2 * m_cxBorder + 2 + m_sizeBox.cy;
	int cx = 2 * m_cxBorder + 2 + 5 * m_sizeBox.cx + 1;
	if (m_fHorz = bHorz ? true : false) {
		if (bStretch)
			return CSize(2046, cy);
		return CSize(cx, cy);
	} else {
		if (bStretch)
			return CSize(cy, 2046);
		return CSize(cy, cx);
	}
}

void CDMLvSelWnd2::SetCount(int n)
{
	m_nItems = _MAX(0, n);
	m_iOffset = ValidateOffset(m_iOffset);
	Reassign();
	Invalidate();
}

int CDMLvSelWnd2::GetPos()
{
	return m_iSel;
}

int CDMLvSelWnd2::GetCount()
{
	return m_nItems;
}

void CDMLvSelWnd2::SetPos(int iPos)
{
	int iSel = (0 <= iPos && iPos < m_nItems) ? iPos : -1;

	if (m_iSel != iSel) {
		m_iSel = iSel;

		if (m_iSel < 0) {
			m_iSel = -1;
		} else {
			if (iSel < m_iOffset || m_iOffset + m_nUserItems <= iSel) {
				int iOffset = ValidateOffset(m_iSel - m_nUserItems / 2);

				if (m_iOffset != iOffset) {
					m_iOffset = iOffset;

					Reassign();
				}
			}
		}

		Invalidate();
	}
}
