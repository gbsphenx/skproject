// InprogressDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "dm2gded.h"
#include "InprogressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInprogressDlg �_�C�A���O


CInprogressDlg::CInprogressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInprogressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInprogressDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CInprogressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInprogressDlg)
	DDX_Control(pDX, IDC_PROGRESS_HINT, m_wndHint);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInprogressDlg, CDialog)
	//{{AFX_MSG_MAP(CInprogressDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInprogressDlg ���b�Z�[�W �n���h��
