// DM2NI1Page.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2NI1Page.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2NI1Page プロパティ ページ

IMPLEMENT_DYNCREATE(CDM2NI1Page, CDM2NIBPage)

CDM2NI1Page::CDM2NI1Page() : CDM2NIBPage(CDM2NI1Page::IDD)
{
	//{{AFX_DATA_INIT(CDM2NI1Page)
	m_i4bpp = -1;
	//}}AFX_DATA_INIT

	m_i4bpp = 1;
}

CDM2NI1Page::~CDM2NI1Page()
{
}

void CDM2NI1Page::DoDataExchange(CDataExchange* pDX)
{
	CDM2NIBPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDM2NI1Page)
	DDX_Radio(pDX, IDC_RADIO_4BPP, m_i4bpp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDM2NI1Page, CDM2NIBPage)
	//{{AFX_MSG_MAP(CDM2NI1Page)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_4BPP, IDC_RADIO_8BPP, OnRadio4bpp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2NI1Page メッセージ ハンドラ

BOOL CDM2NI1Page::OnSetActive() 
{
	if (!CDM2NIBPage::OnSetActive()) return false;

	m_frame.m_view.MakeStaged(1);

	return true;
}

void CDM2NI1Page::OnRadio4bpp(UINT nID) 
{
	if (!MkCtx()) return;

	int i4bpp = m_i4bpp;
	if (!UpdateData()) return;
	if (i4bpp == m_i4bpp) return;

	m_ctx->fUse8 = !m_ctx->fForce4 && m_i4bpp != 0;

	CWaitCursor wc;

	VERIFY(m_ctx->DoStep(1));

	m_frame.m_view.MakeStaged(1);
}
