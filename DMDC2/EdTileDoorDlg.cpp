// EdTileDoorDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdTileDoorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdTileDoorDlg ダイアログ


CEdTileDoorDlg::CEdTileDoorDlg(CWnd* pParent /*=NULL*/)
	: CEdTileDlg(CEdTileDoorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEdTileDoorDlg)
	m_fSw1 = FALSE;
	//}}AFX_DATA_INIT
}


void CEdTileDoorDlg::DoDataExchange(CDataExchange* pDX)
{
	CEdTileDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdTileDoorDlg)
	DDX_Check(pDX, IDC_CHECK_SW1, m_fSw1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEdTileDoorDlg, CEdTileDlg)
	//{{AFX_MSG_MAP(CEdTileDoorDlg)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK_SW1, IDC_RADIO_OP8, OnCheckSw1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdTileDoorDlg メッセージ ハンドラ

void CEdTileDoorDlg::OnCheckSw1(UINT nID) 
{
	if (m_fSiteLow || m_fSiteUpdating || !UpdateData()) return;

	switch (nID) {
	case IDC_RADIO_OP1: Modify(7, 0); break;
	case IDC_RADIO_OP2: Modify(7, 1); break;
	case IDC_RADIO_OP3: Modify(7, 2); break;
	case IDC_RADIO_OP4: Modify(7, 3); break;
	case IDC_RADIO_OP5: Modify(7, 4); break;
	case IDC_RADIO_OP6: Modify(7, 5); break;
	case IDC_RADIO_OP7: Modify(7, 6); break;
	case IDC_RADIO_OP8: Modify(7, 7); break;
	case IDC_CHECK_SW1: Set(8, m_fSw1); break;
	case IDC_CHECK_SW2: break;
	case IDC_CHECK_SW3: break;
	case IDC_CHECK_SW4: break;
	case IDC_CHECK_SW5: break;
	case IDC_CHECK_SW6: break;
	case IDC_CHECK_SW7: break;
	case IDC_CHECK_SW8: break;
	}
}

BOOL CEdTileDoorDlg::OnInitDialog() 
{
	CEdTileDlg::OnInitDialog();
	
	Cnt(
		m_cTile & 7
		, m_cTile & 8
		, 0
		, 0
		, 0
		, 0
		, 0
		, 0
		, 0
	);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
