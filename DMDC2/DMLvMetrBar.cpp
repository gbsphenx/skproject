// DMLvMetrBar.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "DMLvMetrBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDMLvMetrBar

CDMLvMetrBar::CDMLvMetrBar()
{
}

CDMLvMetrBar::~CDMLvMetrBar()
{
}

BEGIN_MESSAGE_MAP(CDMLvMetrBar, CDialogBar)
	//{{AFX_MSG_MAP(CDMLvMetrBar)
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDMLvMetrBar メッセージ ハンドラ

HBRUSH CDMLvMetrBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialogBar::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC) {
		switch (pWnd->GetDlgCtrlID()) {
		case IDC_STATIC_LVDESC_APPLY:
		case IDC_STATIC_LVDESC_RESET:
			pDC->SetTextColor((COLORREF)0xff0000);
			break;
		}
	}
	
	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;
}

int CDMLvMetrBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

LRESULT CDMLvMetrBar::HandleInitDialog(WPARAM wParam, LPARAM lParam) 
{
	CDialogBar::HandleInitDialog(wParam, lParam);

	LOGFONT lfUl;
	ZeroMemory(&lfUl, sizeof(lfUl));
	CFont *pFont = GetFont();
	if (pFont)
		pFont->GetLogFont(&lfUl);
	lfUl.lfUnderline = TRUE;
	m_fontUl.DeleteObject();
	m_fontUl.CreateFontIndirect(&lfUl);

	CWnd *wnd;
	if (wnd = GetDlgItem(IDC_STATIC_LVDESC_APPLY)) {
		wnd->SetFont(&m_fontUl);
	}
	if (wnd = GetDlgItem(IDC_STATIC_LVDESC_RESET)) {
		wnd->SetFont(&m_fontUl);
	}

	return TRUE;
}
