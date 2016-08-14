// EdTileDefault.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdTileDefaultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdTileDefaultDlg �_�C�A���O


CEdTileDefaultDlg::CEdTileDefaultDlg(CWnd* pParent /*=NULL*/)
	: CEdTileDlg(CEdTileDefaultDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEdTileDefaultDlg)
	m_fSw1 = FALSE;
	m_fSw2 = FALSE;
	m_fSw3 = FALSE;
	m_fSw4 = FALSE;
	//}}AFX_DATA_INIT
}


void CEdTileDefaultDlg::DoDataExchange(CDataExchange* pDX)
{
	CEdTileDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdTileDefaultDlg)
	DDX_Check(pDX, IDC_CHECK_SW1, m_fSw1);
	DDX_Check(pDX, IDC_CHECK_SW2, m_fSw2);
	DDX_Check(pDX, IDC_CHECK_SW3, m_fSw3);
	DDX_Check(pDX, IDC_CHECK_SW4, m_fSw4);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEdTileDefaultDlg, CEdTileDlg)
	//{{AFX_MSG_MAP(CEdTileDefaultDlg)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK_SW1, IDC_RADIO_OP8, OnCheckSw1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdTileDefaultDlg ���b�Z�[�W �n���h��

BOOL CEdTileDefaultDlg::OnInitDialog() 
{
	CEdTileDlg::OnInitDialog();
	
	Cnt(
		-1
		, m_cTile & 1
		, m_cTile & 2
		, m_cTile & 4
		, m_cTile & 8
		, 0
		, 0
		, 0
		, 0
	);
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CEdTileDefaultDlg::OnCheckSw1(UINT nID) 
{
	if (m_fSiteLow || m_fSiteUpdating || !UpdateData()) return;

	switch (nID) {
	case IDC_RADIO_OP1: break;
	case IDC_RADIO_OP2: break;
	case IDC_RADIO_OP3: break;
	case IDC_RADIO_OP4: break;
	case IDC_RADIO_OP5: break;
	case IDC_RADIO_OP6: break;
	case IDC_RADIO_OP7: break;
	case IDC_RADIO_OP8: break;
	case IDC_CHECK_SW1: Set(1, m_fSw1); break;
	case IDC_CHECK_SW2: Set(2, m_fSw2); break;
	case IDC_CHECK_SW3: Set(4, m_fSw3); break;
	case IDC_CHECK_SW4: Set(8, m_fSw4); break;
	case IDC_CHECK_SW5: break;
	case IDC_CHECK_SW6: break;
	case IDC_CHECK_SW7: break;
	case IDC_CHECK_SW8: break;
	}
}