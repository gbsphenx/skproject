// DM2NI3Page.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2NI3Page.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2NI3Page プロパティ ページ

IMPLEMENT_DYNCREATE(CDM2NI3Page, CDM2NIBPage)

CDM2NI3Page::CDM2NI3Page() : CDM2NIBPage(CDM2NI3Page::IDD)
{
	//{{AFX_DATA_INIT(CDM2NI3Page)
	m_iKeep = -1;
	//}}AFX_DATA_INIT

	m_iKeep = 0;
}

CDM2NI3Page::~CDM2NI3Page()
{
}

void CDM2NI3Page::DoDataExchange(CDataExchange* pDX)
{
	CDM2NIBPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDM2NI3Page)
	DDX_Radio(pDX, IDC_RADIO_KEEP_PAL, m_iKeep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDM2NI3Page, CDM2NIBPage) // CDM2NIBPage
	//{{AFX_MSG_MAP(CDM2NI3Page)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_KEEP_PAL, IDC_RADIO_OVERWRITE_PAL, OnRadioKeepPal)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2NI3Page メッセージ ハンドラ

BOOL CDM2NI3Page::OnSetActive() 
{
	if (!CDM2NIBPage::OnSetActive()) AfxThrowNotSupportedException();

	UpdateVw();
	
	return TRUE;
}

void CDM2NI3Page::OnRadioKeepPal(UINT nID) 
{
	int iKeep = m_iKeep;
	if (!UpdateData()) return;
	if (iKeep == m_iKeep) return;

	if (!MkCtx()) return;

	CWaitCursor wc;

	m_ctx->fKeepPal = iKeep ? true : false;

	UpdateVw(true);
}

void CDM2NI3Page::UpdateVw(bool f)
{
	if (f) VERIFY(m_ctx->DoStep(3));

	m_frame.m_view.MakeStaged(3);
}
