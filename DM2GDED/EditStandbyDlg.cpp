// EditStandbyDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditStandbyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditStandbyDlg �_�C�A���O


CEditStandbyDlg::CEditStandbyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditStandbyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditStandbyDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CEditStandbyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditStandbyDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditStandbyDlg, CDialog)
	//{{AFX_MSG_MAP(CEditStandbyDlg)
	ON_BN_CLICKED(IDCLOSE, OnClose)
	ON_BN_CLICKED(IDRETRY, OnRetry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditStandbyDlg ���b�Z�[�W �n���h��

void CEditStandbyDlg::OnClose() 
{
	EndDialog(IDCLOSE);
}

void CEditStandbyDlg::OnRetry() 
{
	EndDialog(IDRETRY);
}
