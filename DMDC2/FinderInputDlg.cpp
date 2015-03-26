// FinderInputDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "exp00069.h"
#include "FinderInputDlg.h"
#include "DM2NameDic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFinderInputDlg �_�C�A���O


CFinderInputDlg::CFinderInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFinderInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFinderInputDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CFinderInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFinderInputDlg)
	DDX_Control(pDX, IDC_COMBO_SEL, m_wndSel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFinderInputDlg, CDialog)
	//{{AFX_MSG_MAP(CFinderInputDlg)
	ON_CBN_DBLCLK(IDC_COMBO_SEL, OnDblclkComboSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFinderInputDlg ���b�Z�[�W �n���h��

BOOL CFinderInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	static const BYTE cntTy = 7;
	static const BYTE tytbl[cntTy] = {3,4, 5,6,8,9,10};

	for (int ty=0; ty<cntTy; ty++) {
		for (int x=0; x<128; x++) {
			CString strName = CDM2NameDic::GetInstance().GetName(tytbl[ty], x);
			if (strName.IsEmpty() || strName == "-") continue;

			int r = m_wndSel.AddString(strName);
			m_wndSel.SetItemData(r, 0xFEFE0000 | (tytbl[ty] << 8) | x);
		}
	}
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CFinderInputDlg::OnOK() 
{
	m_iFindData = m_wndSel.GetItemData(m_wndSel.GetCurSel());
	
	CDialog::OnOK();
}

void CFinderInputDlg::OnDblclkComboSel() 
{
	OnOK();
	
}
