// DM2EdPicFrame.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2EdPicFrame.h"
#include "DM2EdPicView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDM2EdPicFrame::DocMap CDM2EdPicFrame::m_map;

/////////////////////////////////////////////////////////////////////////////
// CDM2EdPicFrame

IMPLEMENT_DYNAMIC(CDM2EdPicFrame, CFrameWnd)

CDM2EdPicFrame::CDM2EdPicFrame(CGDEdCtx *pEdCtx): m_pEdCtx(pEdCtx)
{

}

CDM2EdPicFrame::~CDM2EdPicFrame()
{

}

BEGIN_MESSAGE_MAP(CDM2EdPicFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CDM2EdPicFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2EdPicFrame メッセージ ハンドラ

BOOL CDM2EdPicFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	CDM2EdPicView *pView = new CDM2EdPicView(m_pEdCtx);

	if (pView->Create(
		NULL,
		NULL,
		AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0),
		this,
		AFX_IDW_PANE_FIRST
		)
	) {
		return TRUE;
	}
	return FALSE;
}

int CDM2EdPicFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (false
		|| !m_wndVwPal.Create(WS_EX_CLIENTEDGE, 0 |WS_CHILD |WS_VISIBLE, CBRS_ALIGN_BOTTOM, this, 0x0100)
	) {
		return -1;
	}

	if (false
		|| !m_wndToolBar.Create(this, 0 |WS_CHILD |WS_VISIBLE |CBRS_TOP |CBRS_TOOLTIPS |CBRS_FLYBY |CBRS_SIZE_DYNAMIC)
		|| !m_wndToolBar.LoadToolBar(IDR_EDPIC)
	) {
		return -1;
	}
	m_wndToolBar.SetFlatLook();
	m_wndToolBar.SetHeight(GetSystemMetrics(SM_CYSMICON) + 12);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
	
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

CDM2EdPicFrame *CDM2EdPicFrame::CreateNew(CDocument *pDoc, CGDEdCtx *pEdCtx, CGDEntry pos)
{
	bool fMakeVis = false;
	CDM2EdPicFrame *pFrame;
	DocMap::iterator
		iterPos = m_map.find(pDoc),
		iterEnd = m_map.end();
	if (iterPos == iterEnd) {
		pFrame = new CDM2EdPicFrame(pEdCtx);
		pFrame->LoadFrame(IDR_EDPIC);
		fMakeVis = true;
	} else {
		pFrame = iterPos->second;
	}
	if (pFrame) {
		if (fMakeVis) {
			pFrame->InitialUpdateFrame(NULL, 0);
		}
		CDM2EdPicView *pVw = static_cast<CDM2EdPicView *>(pFrame->GetActiveView());
		if (pVw) {
			pVw->Select(pos);
		}

		if (fMakeVis) {
			pFrame->InitialUpdateFrame(NULL, 1);
			m_map[pDoc] = pFrame;
		} else {
			pFrame->SetActiveWindow();
		}
		return pFrame;
	}
	return NULL;
}

void CDM2EdPicFrame::OnCloseDocument(CDocument *pDoc)
{
	DocMap::iterator
		iterPos = m_map.find(pDoc),
		iterEnd = m_map.end();
	if (iterPos != iterEnd) {
		CDM2EdPicFrame *p = iterPos->second;
		m_map.erase(iterPos);

		p->DestroyWindow();
	}
}

void CDM2EdPicFrame::PostNcDestroy() 
{
	DocMap::iterator
		iterPos = m_map.begin(),
		iterEnd = m_map.end();
	while (iterPos != iterEnd) {
		if (this == iterPos->second) {
			iterPos = m_map.erase(iterPos);
		} else {
			iterPos++;
		}
	}
	
	CFrameWnd::PostNcDestroy();
}

void CDM2EdPicFrame::OnClose() 
{
	CDM2EdPicView *pVw = static_cast<CDM2EdPicView *>(GetActiveView());
	if (pVw) {
		if (!pVw->DoSave(true)) return;
	}
	
	CFrameWnd::OnClose();
}
