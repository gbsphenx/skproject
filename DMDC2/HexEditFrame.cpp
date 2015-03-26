// HexEditFrame.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "HexEditFrame.h"
#include "NamedWndBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHexEditFrame

IMPLEMENT_DYNCREATE(CHexEditFrame, CFrameWnd)

CHexEditFrame::CHexEditFrame()
{
	m_hWndModalTar = NULL;
}

CHexEditFrame::~CHexEditFrame()
{

}

BEGIN_MESSAGE_MAP(CHexEditFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CHexEditFrame)
	ON_COMMAND(ID_APP_CLOSE, OnAppClose)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHexEditFrame メッセージ ハンドラ

#include "HexEditDoc.h"
#include "HexEditView.h"

CHexEditFrame *CHexEditFrame::Create1(CWnd *pParentWnd)
{
	CObject *pDoc;
	CObject *pFrame;

	CCreateContext C;
	C.m_pCurrentDoc = (CDocument *)(pDoc = RUNTIME_CLASS(CHexEditDoc)->CreateObject());
	C.m_pCurrentFrame = (CFrameWnd *)(pFrame = RUNTIME_CLASS(CHexEditFrame)->CreateObject());
	C.m_pLastView = NULL;
	C.m_pNewDocTemplate = NULL;
	C.m_pNewViewClass = RUNTIME_CLASS(CHexEditView);

	if (C.m_pCurrentDoc && C.m_pCurrentFrame && C.m_pNewViewClass) {
		DWORD dwStyle = WS_POPUPWINDOW|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX;
		if (C.m_pCurrentFrame->LoadFrame(IDR_DMHEXEDIT, dwStyle, pParentWnd, &C)) {
			if (C.m_pCurrentDoc->OnNewDocument()) {
				C.m_pCurrentFrame->InitialUpdateFrame(C.m_pCurrentDoc, TRUE);
				if (AfxGetMainWnd() && !AfxGetMainWnd()->IsIconic()) {
					CRect rc;
					AfxGetMainWnd()->GetWindowRect(rc);
					C.m_pCurrentFrame->MoveWindow(rc);
				}
				return static_cast<CHexEditFrame *>(C.m_pCurrentFrame);
			}
		}
		C.m_pCurrentFrame->DestroyWindow();
		return NULL;
	}
	if (pDoc) delete pDoc;
	if (pFrame) delete pFrame;
	return NULL;
}

void CHexEditFrame::OnAppClose() 
{
	OnClose();
}

int CHexEditFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//m_hWndModalTar = lpCreateStruct->hwndParent;

	if (m_hWndModalTar)
		::EnableWindow(m_hWndModalTar, FALSE);
	
	return 0;
}

void CHexEditFrame::OnDestroy() 
{
	g_boxWnd.DeleteMeFromBox(this);

	CFrameWnd::OnDestroy();
}

void CHexEditFrame::OnClose() 
{
	EndModalLoop(IDCANCEL);

	if (m_hWndModalTar) {
		::EnableWindow(m_hWndModalTar, TRUE);
		m_hWndModalTar = NULL;
	}

	CFrameWnd::OnClose();
}

BOOL CHexEditFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.dwExStyle |= WS_EX_DLGMODALFRAME;

	return CFrameWnd::PreCreateWindow(cs);
}

void CHexEditFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
}
