#if !defined(AFX_EDITSTANDBYDLG_H__E51D7BFB_F8F2_4A3F_8B9C_7CAFA1D07992__INCLUDED_)
#define AFX_EDITSTANDBYDLG_H__E51D7BFB_F8F2_4A3F_8B9C_7CAFA1D07992__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditStandbyDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CEditStandbyDlg �_�C�A���O

class CEditStandbyDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CEditStandbyDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEditStandbyDlg)
	enum { IDD = IDD_WAIT_EDIT };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditStandbyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEditStandbyDlg)
	afx_msg void OnClose();
	afx_msg void OnRetry();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDITSTANDBYDLG_H__E51D7BFB_F8F2_4A3F_8B9C_7CAFA1D07992__INCLUDED_)
