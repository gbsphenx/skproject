// HexEditFrame.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DM2GDED.h"
#include "HexEditFrame.h"

#include "HexEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHexEditFrame

IMPLEMENT_DYNAMIC(CHexEditFrame, CFrameWnd)

CHexEditFrame::CHexEditFrame()
{
	m_pView = NULL;
}

CHexEditFrame::~CHexEditFrame()
{

}

BEGIN_MESSAGE_MAP(CHexEditFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CHexEditFrame)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHexEditFrame メッセージ ハンドラ

BOOL CHexEditFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	m_pView = new CHexEditView();
	if (!m_pView->Create(this, AFX_IDW_PANE_FIRST))
		return FALSE;
	return TRUE;
}

CHexEditCB *CHexEditFrame::GetHexEditCB()
{
	return m_pView;
}

void CHexEditFrame::PostNcDestroy() 
{
	m_pView = NULL;

	OnDeleteHexEditFrame(this);
	
	CFrameWnd::PostNcDestroy();
}

void CHexEditFrame::OnClose() 
{
	if (m_pView && !m_pView->SaveModified()) {
		return;
	}
	
	CFrameWnd::OnClose();
}
