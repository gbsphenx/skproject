#if !defined(AFX_DIFFEXPORTDLG_H__E137BEF2_A54A_4A61_83D2_E7094EB8D7F0__INCLUDED_)
#define AFX_DIFFEXPORTDLG_H__E137BEF2_A54A_4A61_83D2_E7094EB8D7F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiffExportDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDiffExportDlg �_�C�A���O

class CDiffExportDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CDiffExportDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDiffExportDlg)
	enum { IDD = IDD_DIFF_EXPORT };
	CString	m_strFileMod;
	CString	m_strFileOrg;
	CString	m_strOutput;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDiffExportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDiffExportDlg)
	afx_msg void OnButtonRefOrg();
	afx_msg void OnButtonRefOut();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DIFFEXPORTDLG_H__E137BEF2_A54A_4A61_83D2_E7094EB8D7F0__INCLUDED_)
