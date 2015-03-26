#if !defined(AFX_OPENFTQUERYFILEDLG_H__445CEB88_CE7D_41F3_A11D_F02028E98C39__INCLUDED_)
#define AFX_OPENFTQUERYFILEDLG_H__445CEB88_CE7D_41F3_A11D_F02028E98C39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenFTQueryFileDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// COpenFTQueryFileDlg �_�C�A���O

class COpenFTQueryFileDlg : public CFileDialog
{
public:
	// 
	COpenFTQueryFileDlg(
		BOOL bOpenFileDialog,
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL,
		DWORD dwSize = 0
	);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(COpenFTQueryFileDlg)
	enum { IDD = IDD_QUERY_OF };
	BOOL	m_fUse;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(COpenFTQueryFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(COpenFTQueryFileDlg)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_OPENFTQUERYFILEDLG_H__445CEB88_CE7D_41F3_A11D_F02028E98C39__INCLUDED_)
