#if !defined(AFX_IMPORTQUERYDLG_H__61520CCC_4495_42AE_99CA_D8FD6E383AF2__INCLUDED_)
#define AFX_IMPORTQUERYDLG_H__61520CCC_4495_42AE_99CA_D8FD6E383AF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportQueryDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CImportQueryDlg �_�C�A���O

class CImportQueryDlg : public CDialog
{
public:
	// 
	CImportQueryDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	// 
	bool IfOverwrite() { return m_iOverwriteSel == 0; }

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CImportQueryDlg)
	enum { IDD = IDD_QUERY_IMPORT };
	int		m_iOverwriteSel;
	CString	m_strFileIn;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CImportQueryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CImportQueryDlg)
	afx_msg void OnButtonBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_IMPORTQUERYDLG_H__61520CCC_4495_42AE_99CA_D8FD6E383AF2__INCLUDED_)
