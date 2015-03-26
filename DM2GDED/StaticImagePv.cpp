// StaticImagePv.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DM2GDED.h"
#include "StaticImagePv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticImagePv

CStaticImagePv::CStaticImagePv()
{

}

CStaticImagePv::~CStaticImagePv()
{

}

BEGIN_MESSAGE_MAP(CStaticImagePv, CStatic)
	//{{AFX_MSG_MAP(CStaticImagePv)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticImagePv メッセージ ハンドラ

CRect CalcCenter(CRect rc, int cx, int cy)
{
	CPoint pt = rc.CenterPoint();
	rc.SetRect(
		pt.x - (cx + 0) / 2,
		pt.y - (cy + 0) / 2,
		pt.x + (cx + 1) / 2,
		pt.y + (cy + 1) / 2
		);
	return rc;
}

#include "CompatDC.h"

void CStaticImagePv::OnPaint() 
{
	CPaintDC dc(this);

	dc.SaveDC();

	TRY
		BITMAP bm;
		ZeroMemory(&bm, sizeof(bm));
		HBITMAP hBM = GetBitmap();
		GetObject(hBM, sizeof(BITMAP), &bm);

		CRect rc;
		GetClientRect(&rc);
		CRect rcPv = CalcCenter(rc, bm.bmWidth, bm.bmHeight);
		CCompatDC dcMem(&dc);

		dcMem.SelectObject(hBM);

		dc.BitBlt(rcPv.left, rcPv.top, rcPv.Width(), rcPv.Height(), &dcMem, 0, 0, SRCCOPY);
		dc.ExcludeClipRect(rcPv);
		dc.FillSolidRect(&rc, RGB(0, 0, 0));

	CATCH_ALL(e)

	END_CATCH_ALL

	dc.RestoreDC(-1);
}
