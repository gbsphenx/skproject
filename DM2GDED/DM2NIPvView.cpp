// DM2NIPvView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2NIPvView.h"
#include "CompatDC.h"
#include "DM2ImageExch.h"
#include "PM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2NIPvView

IMPLEMENT_DYNCREATE(CDM2NIPvView, CScrollView)

CDM2NIPvView::CDM2NIPvView()
{
	m_iStage = -1;
	m_ctx = NULL;
}

BEGIN_MESSAGE_MAP(CDM2NIPvView, CScrollView)
	//{{AFX_MSG_MAP(CDM2NIPvView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2NIPvView 描画

void CDM2NIPvView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	if (!MkCtx()) AfxThrowNotSupportedException();

	CSize sizeTotal;
	sizeTotal = m_size = m_ctx->size;
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_ptOrg.x = m_ptOrg.y = 0;
}

void CDM2NIPvView::OnDraw(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	CSize size = rc.Size();

	CPoint pt = GetDeviceScrollPosition();

	CSize sizeMax = GetTotalSize();

	sizeMax.cx = _MAX(sizeMax.cx, size.cx);
	sizeMax.cy = _MAX(sizeMax.cy, size.cy);

	CCompatDC dcMem(pDC);
	pDC->SaveDC();
	CBitmap *pOldBM = dcMem.SelectObject(&m_bm);

	int x = (m_size.cx < size.cx)
		? ((size.cx - m_size.cx) / 2)
		: (pt.x)
		;
	int y = (m_size.cy < size.cy)
		? ((size.cy - m_size.cy) / 2)
		: (pt.y)
		;

	m_ptOrg.x = x;
	m_ptOrg.y = y;

	pDC->BitBlt(x, y, m_size.cx, m_size.cy, &dcMem, 0, 0, SRCCOPY);
	pDC->ExcludeClipRect(CRect(CPoint(x, y), m_size));

	pDC->FillSolidRect(0, 0, sizeMax.cx, sizeMax.cy, RGB(0, 0, 0));

	dcMem.SelectObject(pOldBM);
	pDC->RestoreDC(-1);
}

/////////////////////////////////////////////////////////////////////////////
// CDM2NIPvView 診断

#ifdef _DEBUG
void CDM2NIPvView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDM2NIPvView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDM2NIPvView メッセージ ハンドラ

void CDM2NIPvView::PostNcDestroy() 
{

}

BOOL CDM2NIPvView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
//	
//	return CScrollView::OnEraseBkgnd(pDC);
}

void CDM2NIPvView::MakeStaged(int iStage)
{
	m_bm.DeleteObject();

	if (iStage < 0) return;

	if (false
		|| !MkCtx()
		|| !m_ctx->Enter(iStage)
	) {
		AfxThrowNotSupportedException();
	}

	HBITMAP hbm = NULL;
	if (!CDM2ImageConv().EncodeToBM(m_ctx->imgm[iStage], hbm, m_ctx->DM2Pal)) AfxThrowNotSupportedException();

	m_bm.Attach(hbm);

	Invalidate();
}

void CDM2NIPvView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPoint pt = point + GetDeviceScrollPosition() - m_ptOrg;
	if (true
		&& (UINT)pt.x < m_size.cx
		&& (UINT)pt.y < m_size.cy
	) {
		OnClickPixel(pt);
		return;
	}
	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CDM2NIPvView::OnClickPixel(CPoint point)
{
	ASSERT(GetOwner() != NULL);

	GetOwner()->SendMessage(PM_WM_CLICK_PIXEL, point.x, point.y);
}
