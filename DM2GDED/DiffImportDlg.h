#if !defined(AFX_DIFFIMPORTDLG_H__198313CB_61BE_42AD_B3FD_4F4DB0F935A8__INCLUDED_)
#define AFX_DIFFIMPORTDLG_H__198313CB_61BE_42AD_B3FD_4F4DB0F935A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiffImportDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDiffImportDlg �_�C�A���O

class CDiffImportDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CDiffImportDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDiffImportDlg)
	enum { IDD = IDD_DIFF_IMPORT };
	CString	m_strFileDiff;
	CString	m_strFileOrg;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDiffImportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDiffImportDlg)
	afx_msg void OnButtonRefDiff();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DIFFIMPORTDLG_H__198313CB_61BE_42AD_B3FD_4F4DB0F935A8__INCLUDED_)
