#if !defined(AFX_DM2GDEDOPINFODLG_H__A97B350C_D5EA_4C1B_A058_069D183E8EA4__INCLUDED_)
#define AFX_DM2GDEDOPINFODLG_H__A97B350C_D5EA_4C1B_A058_069D183E8EA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2GDEDOPInfoDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDOPInfoDlg �_�C�A���O

class CDM2GDEDOPInfoDlg : public CDialog
{

public:
	// 
	CDM2GDEDOPInfoDlg(CWnd* pParent = NULL);
	// 
	void SetGDOF(DWORD nGDOF);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDM2GDEDOPInfoDlg)
	enum { IDD = IDD_OPEN_INFO };
	CString	m_strLE;
	CString	m_strPath;
	CString	m_strPM;
	CString	m_strRP;
	CString	m_strST;
	CString	m_strTL;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDM2GDEDOPInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDM2GDEDOPInfoDlg)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DM2GDEDOPINFODLG_H__A97B350C_D5EA_4C1B_A058_069D183E8EA4__INCLUDED_)
