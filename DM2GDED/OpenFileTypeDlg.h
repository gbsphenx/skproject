#if !defined(AFX_OPENFILETYPEDLG_H__0631E734_BE72_4831_9E11_2917091E69D5__INCLUDED_)
#define AFX_OPENFILETYPEDLG_H__0631E734_BE72_4831_9E11_2917091E69D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenFileTypeDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// COpenFileTypeDlg �_�C�A���O

class COpenFileTypeDlg : public CDialog
{
	// 
	struct OpenTbl
	{	// 
		LPCSTR pszName;
		// 
		BOOL fLE;
		// 
		int iLang;
		// 
		int iPal;
		// 
		int iSnd;
		// 
		int iPicFmt;
		// 
		BOOL fRestrictPal;
	};
	// 
	static OpenTbl m_tblOpen[];

public:
	// 
	COpenFileTypeDlg(LPCTSTR pszFileIn, CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	// 
	UINT GetGDOF();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(COpenFileTypeDlg)
	enum { IDD = IDD_QUERY_FILE_TYPE };
	CComboBox	m_wndPlatform;
	int		m_iLangSel;
	int		m_iPlatformSel;
	CString	m_strFileIn;
	int		m_iSndSel;
	int		m_iPalSel;
	BOOL	m_fRestrictPal;
	int		m_iPicFmtSel;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(COpenFileTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(COpenFileTypeDlg)
	afx_msg void OnSelchangeComboPlatform();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRetry();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_OPENFILETYPEDLG_H__0631E734_BE72_4831_9E11_2917091E69D5__INCLUDED_)
