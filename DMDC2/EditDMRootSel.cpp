// EditDMTileDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "exp00069.h"
#include "EditDMRootSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditDMRootSel �_�C�A���O


CEditDMRootSel::CEditDMRootSel(RN rnNext, CWnd* pParent /*=NULL*/)
	: CDialog(CEditDMRootSel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditDMRootSel)
	//}}AFX_DATA_INIT
	m_rnNext = rnNext;
}

void CEditDMRootSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditDMRootSel)
	DDX_Control(pDX, IDC_COMBO_NEXT_ITEM, m_wndNextItem);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEditDMRootSel, CDialog)
	//{{AFX_MSG_MAP(CEditDMRootSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditDMRootSel ���b�Z�[�W �n���h��

BOOL CEditDMRootSel::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString tstr;
	tstr.Format("%04X", (UINT)m_rnNext.r.w);
	m_wndNextItem.AddString(tstr);
	m_wndNextItem.SetCurSel(0);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CEditDMRootSel::OnOK()
{
	CString tstr;
	m_wndNextItem.GetWindowText(tstr);
	LPTSTR lpszAfter;
	m_rnNext = RNco((WORD)strtol(tstr, &lpszAfter, 16));

	CDialog::OnOK();
}
