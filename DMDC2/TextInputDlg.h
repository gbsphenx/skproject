#if !defined(AFX_TEXTINPUTDLG_H__09934EBF_458F_45FB_B31F_5C25879DA9AB__INCLUDED_)
#define AFX_TEXTINPUTDLG_H__09934EBF_458F_45FB_B31F_5C25879DA9AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextInputDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CTextInputDlg �_�C�A���O

class CTextInputDlg : public CDialog
{
	CFont fontMe;

// �R���X�g���N�V����
public:
	CTextInputDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CTextInputDlg)
	enum { IDD = IDD_TEXTINPUT };
	CEdit	m_wndEdit;
	CString	m_strText;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTextInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTextInputDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TEXTINPUTDLG_H__09934EBF_458F_45FB_B31F_5C25879DA9AB__INCLUDED_)
