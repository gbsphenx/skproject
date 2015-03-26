// DM2VwSceneFrame.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2VwSceneFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2VwSceneFrame

IMPLEMENT_DYNCREATE(CDM2VwSceneFrame, CFrameWnd)

CDM2VwSceneFrame::CDM2VwSceneFrame()
{
}

CDM2VwSceneFrame::~CDM2VwSceneFrame()
{
}

BEGIN_MESSAGE_MAP(CDM2VwSceneFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CDM2VwSceneFrame)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2VwSceneFrame メッセージ ハンドラ

CDM2VwSceneFrame *CDM2VwSceneFrame::CreateNew(CDocument *pDoc)
{
	CDocTemplate *pTemplate = GetApp()->GetVwSceneTemplate();
	CDM2VwSceneFrame *pFrame = static_cast<CDM2VwSceneFrame *>(pTemplate->CreateNewFrame(pDoc, NULL));
	if (pFrame) {
		pTemplate->InitialUpdateFrame(pFrame, pDoc);
	}
	return pFrame;
}
