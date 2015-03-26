// GetBackButton.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "GetBackButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetBackButton

CGetBackButton::CGetBackButton()
{
	VERIFY(m_ico.LoadBitmap(IDB_GETBACK));

	m_cursor = AfxGetApp()->LoadCursor(IDC_H_POINT);
	m_fInvert = false;
}

CGetBackButton::~CGetBackButton()
{

}

BEGIN_MESSAGE_MAP(CGetBackButton, CWnd)
	//{{AFX_MSG_MAP(CGetBackButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_ENABLE()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetBackButton メッセージ ハンドラ

void CGetBackButton::OnPaint() 
{
	CPaintDC dc(this);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	HGDIOBJ prev = dcMem.SelectObject(m_ico);
	dc.BitBlt(0, 0, 16, 16, &dcMem, fDirIsUp ? 16 : 0, 0, m_fInvert ? NOTSRCCOPY : SRCCOPY);
	dcMem.SelectObject(prev);
}

void CGetBackButton::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	CWnd *p = GetOwner();
	if (p != NULL) {
		p->SendMessage(WM_COMMAND, ID_BUTTON_GET_BACK);
		ShowWindow(SW_HIDE);
	}
	return;
}

bool CGetBackButton::Create(CWnd *pParentWnd)
{
	if (CWnd::CreateEx(0, AfxRegisterWndClass(0), NULL, WS_POPUP, CRect(0,0,16,16), pParentWnd, 0))
		return true;
	return false;
}

void CGetBackButton::Hide()
{
	if (m_hWnd == NULL) return;

	ShowWindow(SW_HIDE);
}

void CGetBackButton::Show(CWnd *pTar)
{
//	SetParent(pTar->GetParent());

	CRect rc;
	pTar->GetWindowRect(&rc);

	//GetParent()->ScreenToClient(&rc);

	TCHAR tc[256] = {""};
	GetClassName(*pTar, tc, 256);

	if (_tcsicmp(tc, "BUTTON") != 0) {
		rc.top = rc.bottom;
		rc.right = rc.left +16;
		rc.bottom = rc.top +16;

		fDirIsUp = true;
	}
	else {
		int y = (rc.top + rc.bottom) / 2 -8;

		rc.left = rc.right;
		rc.top = y;
		rc.right = rc.left +16;
		rc.bottom = rc.top +16;

		fDirIsUp = false;
	}

	SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOOWNERZORDER|SWP_SHOWWINDOW|SWP_NOZORDER);

	SetTimer(0x0101, 300, NULL); m_fInvert = true;
}

void CGetBackButton::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	CWnd::OnNcLButtonDown(nHitTest, point);
}

void CGetBackButton::OnEnable(BOOL bEnable) 
{
	CWnd::OnEnable(bEnable);
	
	if (!bEnable) EnableWindow(true);
	
}

BOOL CGetBackButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest != HTERROR) {
		SetCursor(m_cursor);
		return true;
	}
	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CGetBackButton::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0x0101) {
		KillTimer(nIDEvent);
		m_fInvert = false;
		Invalidate();
	}
	
	CWnd::OnTimer(nIDEvent);
}
