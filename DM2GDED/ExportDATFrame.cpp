// ExportDATFrame.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DM2GDED.h"
#include "ExportDATFrame.h"
#include "ExportDATListVw.h"
#include "ExportDATTreeVw.h"
#include "PaneFrameVw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportDATFrame

IMPLEMENT_DYNCREATE(CExportDATFrame, CFrameWnd)

CExportDATFrame::CExportDATFrame()
{
	m_pMainWnd = NULL;
}

CExportDATFrame::~CExportDATFrame()
{

}

BEGIN_MESSAGE_MAP(CExportDATFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CExportDATFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
};

/////////////////////////////////////////////////////////////////////////////
// CExportDATFrame メッセージ ハンドラ

BOOL CExportDATFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CPaneFrameVw), CSize(200, 200), pContext))
		return FALSE;
	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CPaneFrameVw), CSize(200, 200), pContext))
		return FALSE;
	{
		CWnd *pView = m_wndSplitter.GetPane(0, 0);
		pView->SetWindowText("Class Tree");
		CView *pNew = (CView *)RUNTIME_CLASS(CExportDATTreeVw)->CreateObject();
		pNew->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), pView, AFX_IDW_PANE_FIRST, pContext);
	}

	{
		CWnd *pView = m_wndSplitter.GetPane(0, 1);
		pView->SetWindowText("Entry View");
		CView *pNew = (CView *)RUNTIME_CLASS(CExportDATListVw)->CreateObject();
		pNew->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), pView, AFX_IDW_PANE_FIRST, pContext);
	}
	
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

int CExportDATFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (false
		|| !m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC)
		|| !m_wndToolBar.LoadToolBar(IDR_EXPORTDATFRAME)
	) {
		return -1;
	}

	if (false
		|| !m_wndStatusBar.Create(this)
		|| !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT))
	) {
		return -1;
	}

	m_wndToolBar.SetHeight(GetSystemMetrics(SM_CYSMICON) + 12);
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

//	EnableDocking(CBRS_ALIGN_ANY);
	
	return 0;
}

void CExportDATFrame::StartModal(CWnd *pParentWnd)
{
	if (m_pMainWnd = dynamic_cast<CFrameWnd *>(pParentWnd)) {
		m_pMainWnd->BeginModalState();
	}
}

void CExportDATFrame::OnDestroy() 
{
	if (m_pMainWnd) {
		m_pMainWnd->EndModalState();
		m_pMainWnd = NULL;
	}

	CFrameWnd::OnDestroy();
}

void CExportDATFrame::OnClose() 
{
	if (m_pMainWnd) {
		m_pMainWnd->SetWindowPos(this, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE);
		m_pMainWnd->SetActiveWindow();
	}

	CFrameWnd::OnClose();
}

BOOL CExportDATFrame::DestroyWindow() 
{
	if (m_pMainWnd) {
		m_pMainWnd->EndModalState();
		m_pMainWnd = NULL;
	}

	return CFrameWnd::DestroyWindow();
}

void CExportDATFrame::PostNcDestroy() 
{
	CFrameWnd::PostNcDestroy();
}
