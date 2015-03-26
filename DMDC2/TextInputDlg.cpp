// TextInputDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "exp00069.h"
#include "TextInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextInputDlg �_�C�A���O


CTextInputDlg::CTextInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextInputDlg)
	m_strText = _T("");
	//}}AFX_DATA_INIT
}


void CTextInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextInputDlg)
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_wndEdit);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextInputDlg, CDialog)
	//{{AFX_MSG_MAP(CTextInputDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextInputDlg ���b�Z�[�W �n���h��

BOOL CTextInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LOGFONT lf;
	ReadFontTemplateFrom(lf, IDS_FONTTEMP_TEXTINPUT, "Courier;;;;;;;;;;;;;1");
	VERIFY(fontMe.CreateFontIndirect(&lf));
	m_wndEdit.SetFont(&fontMe);
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}
