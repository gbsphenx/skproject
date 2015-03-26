// SplashVw.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "SplashVw.h"
#include "CompatDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplashVw

CSplashVw::CSplashVw()
{
}

CSplashVw::~CSplashVw()
{
}


BEGIN_MESSAGE_MAP(CSplashVw, CScrollView)
	//{{AFX_MSG_MAP(CSplashVw)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplashVw 描画

void CSplashVw::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal(m_bmp.bmWidth, m_bmp.bmHeight);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit(true);

	CRect rc;
	GetParentFrame()->GetWindowRect(&rc);
	int x = (GetSystemMetrics(SM_CXFULLSCREEN) -rc.Width()) /2;
	int y = (GetSystemMetrics(SM_CYFULLSCREEN) -rc.Height()) /2;
	GetParentFrame()->MoveWindow(x, y, rc.Width(), rc.Height());
}

void CSplashVw::OnDraw(CDC* pDC)
{
	CCompatDC dcMem(this);

	CGdiObject *pOldBM = dcMem.SelectObject(&m_bm);
	pDC->BitBlt(0, 0, m_bmp.bmWidth, m_bmp.bmHeight, &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(pOldBM);

}

/////////////////////////////////////////////////////////////////////////////
// CSplashVw 診断

#ifdef _DEBUG
void CSplashVw::AssertValid() const
{
	CScrollView::AssertValid();
}

void CSplashVw::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSplashVw メッセージ ハンドラ

int CSplashVw::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_bm.LoadBitmap(IDB_SPLASH)) return -1;

	m_bm.GetBitmap(&m_bmp);
	
	return 0;
}

BOOL CSplashVw::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CScrollView::PreCreateWindow(cs)) return false;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return true;
}
