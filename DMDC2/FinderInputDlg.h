#if !defined(AFX_FINDERINPUTDLG_H__22EEA564_86D4_4A54_834B_D74CD3988FE3__INCLUDED_)
#define AFX_FINDERINPUTDLG_H__22EEA564_86D4_4A54_834B_D74CD3988FE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FinderInputDlg.h : �w�b�_�[ �t�@�C��
//

#include "FinderComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CFinderInputDlg �_�C�A���O

class CFinderInputDlg : public CDialog
{
// �R���X�g���N�V����
public:
	// 
	UINT m_iFindData;

	// 
	CFinderInputDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CFinderInputDlg)
	enum { IDD = IDD_FINDER_INPUT };
	CFinderComboBox	m_wndSel;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CFinderInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CFinderInputDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkComboSel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_FINDERINPUTDLG_H__22EEA564_86D4_4A54_834B_D74CD3988FE3__INCLUDED_)
