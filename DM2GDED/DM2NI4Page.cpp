// DM2NI4Page.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2NI4Page.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2NI4Page �v���p�e�B �y�[�W

IMPLEMENT_DYNCREATE(CDM2NI4Page, CDM2NIBPage)

CDM2NI4Page::CDM2NI4Page() : CDM2NIBPage(CDM2NI4Page::IDD)
{
	//{{AFX_DATA_INIT(CDM2NI4Page)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�̏�����������ǉ����܂��B
	//}}AFX_DATA_INIT
}

CDM2NI4Page::~CDM2NI4Page()
{
}

void CDM2NI4Page::DoDataExchange(CDataExchange* pDX)
{
	CDM2NIBPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDM2NI4Page)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDM2NI4Page, CDM2NIBPage) // CDM2NIBPage
	//{{AFX_MSG_MAP(CDM2NI4Page)
		// ����: ClassWizard �͂��̈ʒu�� DDX ����� DDV �̌Ăяo���R�[�h��ǉ����܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2NI4Page ���b�Z�[�W �n���h��

BOOL CDM2NI4Page::OnSetActive() 
{
	if (!CDM2NIBPage::OnSetActive()) AfxThrowNotSupportedException();

	UpdateVw();
	
	return TRUE;
}

void CDM2NI4Page::UpdateVw()
{
	m_frame.m_view.MakeStaged(3);
}
