// EditImgFlgsDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditImgFlgsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditImgFlgsDlg �_�C�A���O


CEditImgFlgsDlg::CEditImgFlgsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditImgFlgsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditImgFlgsDlg)
	m_nX = 0;
	m_nY = 0;
	m_fUse0c = FALSE;
	//}}AFX_DATA_INIT

	m_f4bpp = true;

	m_nMin = -32;
	m_nMax = +31;
}


void CEditImgFlgsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditImgFlgsDlg)
	DDX_Control(pDX, IDC_EDIT_POS_Y, m_wndPosY);
	DDX_Control(pDX, IDC_EDIT_POS_X, m_wndPosX);
	DDX_Control(pDX, IDC_SPIN_POS_Y, m_wndSpinY);
	DDX_Control(pDX, IDC_SPIN_POS_X, m_wndSpinX);
	DDX_Text(pDX, IDC_EDIT_POS_X, m_nX);
	DDV_MinMaxInt(pDX, m_nX, -128, 127);
	DDX_Text(pDX, IDC_EDIT_POS_Y, m_nY);
	DDV_MinMaxInt(pDX, m_nY, -128, 127);
	DDX_Check(pDX, IDC_CHECK_USE0C, m_fUse0c);
	//}}AFX_DATA_MAP

	DDX_Text(pDX, IDC_EDIT_POS_X, m_nX);
	DDV_MinMaxInt(pDX, m_nX, m_nMin, m_nMax);
	DDX_Text(pDX, IDC_EDIT_POS_Y, m_nY);
	DDV_MinMaxInt(pDX, m_nY, m_nMin, m_nMax);
}


BEGIN_MESSAGE_MAP(CEditImgFlgsDlg, CDialog)
	//{{AFX_MSG_MAP(CEditImgFlgsDlg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditImgFlgsDlg ���b�Z�[�W �n���h��

BOOL CEditImgFlgsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_wndSpinX.SetRange(m_nMin, m_nMax);
	m_wndSpinY.SetRange(m_nMin, m_nMax);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

int CEditImgFlgsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_nMin = -128;
	m_nMax = +127;
	
//	m_nMin = m_f4bpp ? -32 : -128;
//	m_nMax = m_f4bpp ? +31 : +127;
//
//	m_strRangeX = 
//	m_strRangeY = m_f4bpp ? _T("Min = -32, Max = 31") : _T("Min = -128, Max = 127");
//
//	if (m_nY < -31 && m_f4bpp) {
//		m_nX = 0;
//		m_nY = 0;
//	}
	
	return 0;
}
