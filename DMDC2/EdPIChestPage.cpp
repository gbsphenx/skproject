// EdPIChestPage.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdPIChestPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdPIChestPage �v���p�e�B �y�[�W

IMPLEMENT_DYNCREATE(CEdPIChestPage, CEdPIDlg)

CEdPIChestPage::CEdPIChestPage() : CEdPIDlg(CEdPIChestPage::IDD)
{
	//{{AFX_DATA_INIT(CEdPIChestPage)
	m_iSel = -1;
	//}}AFX_DATA_INIT
}

void CEdPIChestPage::DoDataExchange(CDataExchange* pDX)
{
	CEdPIDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdPIChestPage)
	DDX_Control(pDX, IDC_COMBO_CHEST, m_wndSel);
	DDX_CBIndex(pDX, IDC_COMBO_CHEST, m_iSel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEdPIChestPage, CEdPIDlg)
	//{{AFX_MSG_MAP(CEdPIChestPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdPIChestPage ���b�Z�[�W �n���h��

bool CEdPIChestPage::RevertRecord(DMDBU &rec, bool fRevert)
{
	if (fRevert) {
		m_iSel = CExp00069App::CHES2ID(rec.r9.w4);
	} else {
		ModifySetVal(rec.r9.w4, 0xE006, CExp00069App::ID2CHES(m_iSel));
	}
	return true;
}

BOOL CEdPIChestPage::OnInitDialog() 
{
	CEdPIDlg::OnInitDialog();
	
	for (int i = 0; i < 32; i++) {
		m_wndSel.AddString(CDM2NameDic::GetInstance().GetName(9, i));
	}
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}
