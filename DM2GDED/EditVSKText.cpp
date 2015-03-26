// EditVSKText.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditVSKText.h"
#include "GDEdCtx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDW_GRID 0x0100

/////////////////////////////////////////////////////////////////////////////
// CEditVSKText ダイアログ


void CEditVSKText::Construction()
{
	//{{AFX_DATA_INIT(CEditVSKText)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CEditVSKText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditVSKText)
	DDX_Control(pDX, IDC_STATIC_SEE_POST, m_wndLink);
	DDX_Control(pDX, IDC_STATIC_GRIDCTRL, m_wndPlaceHolder);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
		if (m_wndGrid.m_hWnd)
			m_wndGrid.SaveList(m_listCmd);
	}
}


BEGIN_MESSAGE_MAP(CEditVSKText, CDialog)
	//{{AFX_MSG_MAP(CEditVSKText)
	ON_WM_CREATE()
	ON_WM_MOVE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditVSKText メッセージ ハンドラ

BOOL CEditVSKText::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CDM2SkCmdDesc *pSkCmdDesc = NULL;
	if (m_vctx.GetEdCtx(FALSE))
		pSkCmdDesc = &m_vctx.GetEdCtx(FALSE)->GetSkCmdDesc();

	CRect rc;
	m_wndPlaceHolder.GetWindowRect(rc);
	ScreenToClient(&rc);
	m_wndGrid.Create(rc, this, IDW_GRID);
	m_wndGrid.CreateList(m_listCmd, pSkCmdDesc);

	m_wndLink.SetURL(LoadString(IDS_URL_SUULE_DIGGING));

	CAnchorSupport::CaptureLayout(acrLeft|acrTop);
	CAnchorSupport::SetAnchor(IDW_GRID, acrLeft|acrTop|acrRight|acrBottom);
	CAnchorSupport::SetAnchor(IDC_STATIC_THX1, acrLeft|acrBottom);
	CAnchorSupport::SetAnchor(IDC_STATIC_SEE_POST, acrLeft|acrBottom);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

int CEditVSKText::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_listCmd.Parse(m_strInitText);
	m_strName = m_listCmd.strName;

	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CEditVSKText::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	CAnchorSupport::Moved();
	
}

void CEditVSKText::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CAnchorSupport::Resized();
	
}
