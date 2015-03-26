// DMTeleLocFrm.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "DMTeleLocFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL AFXAPI AfxIsDescendant(HWND hWndParent, HWND hWndChild);

/////////////////////////////////////////////////////////////////////////////
// CDMTeleLocFrm

BEGIN_MESSAGE_MAP(CDMTeleLocFrm, CFrameWnd)
	//{{AFX_MSG_MAP(CDMTeleLocFrm)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMTeleLocFrm メッセージ ハンドラ

void CDMTeleLocFrm::PostNcDestroy() 
{

}

int CDMTeleLocFrm::DoModal(CWnd *pParentWnd)
{
	pParentWnd = pParentWnd->GetTopLevelParent();

	if (LoadFrame(IDR_DMTELE, 0 |WS_OVERLAPPED |WS_SYSMENU |WS_CAPTION |WS_THICKFRAME |WS_MINIMIZEBOX, pParentWnd)) {
		CRect rc;
		if (::GetWindowRect(pParentWnd->GetTopLevelParent()->GetSafeHwnd(), &rc))
			SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), 0 |SWP_NOZORDER);
		InitialUpdateFrame(NULL, TRUE);
		EnterModalState();
		int nRet = RunModalLoop();
		LeaveModalState();
		DestroyWindow();
		return nRet;
	}
	return -1;
}

void CDMTeleLocFrm::OnClose() 
{
	EndModalLoop(IDCANCEL);
//	
//	CFrameWnd::OnClose();
}

BOOL CDMTeleLocFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.hMenu = NULL;
	
	BOOL fRet = CFrameWnd::PreCreateWindow(cs);

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return fRet;
}

BOOL CDMTeleLocFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	const UINT nID = AFX_IDW_PANE_FIRST;

	if (!m_view.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, nID, pContext))
		return FALSE;

	return TRUE;
//	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CDMTeleLocFrm::EnterModalState()
{
	m_iModalStateStack++;

	if (m_iModalStateStack != 1) return;

	HWND hWnd = ::GetWindow(::GetDesktopWindow(), GW_CHILD);

	while (hWnd != NULL) {
		if (true
			&& ::IsWindowEnabled(hWnd)
			&& CWnd::FromHandlePermanent(hWnd)
			&& hWnd != *this
		) {
			::EnableWindow(hWnd, FALSE);

			if (hWnd == ::GetActiveWindow()) {
				m_arrayWndz.InsertAt(0, hWnd);
			} else {
				m_arrayWndz.Add(hWnd);
			}
		}

		hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
	}
}

void CDMTeleLocFrm::LeaveModalState()
{
	if (m_iModalStateStack == 0) return;

	m_iModalStateStack--;

	if (m_iModalStateStack != 0) return;

	HWND *phPos = (HWND *)m_arrayWndz.GetData();
	HWND *phEnd = phPos + m_arrayWndz.GetSize();

	HWND hWndTop = (phPos != NULL) ? *phPos : NULL;

	for (; phPos != phEnd; phPos++) ::EnableWindow(*phPos, TRUE);

	m_arrayWndz.RemoveAll();

	::SetWindowPos(hWndTop, *this, 0, 0, 0, 0, 0 |SWP_NOMOVE |SWP_NOSIZE);
}

int CDMTeleLocFrm::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CDMTeleLocFrm::OnDestroy() 
{

}
