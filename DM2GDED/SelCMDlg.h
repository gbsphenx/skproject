#if !defined(AFX_SELCMDLG_H__41B887C6_7817_42EF_97EC_82E56A06D484__INCLUDED_)
#define AFX_SELCMDLG_H__41B887C6_7817_42EF_97EC_82E56A06D484__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelCMDlg.h : �w�b�_�[ �t�@�C��
//

#include "DM2SkCmdList.h"

/////////////////////////////////////////////////////////////////////////////
// CSelCMDlg �_�C�A���O

class CSelCMDlg : public CDialog
{
	// 
	CImageList m_pics;

public:
	// 
	int m_iSel;
	// 
	const CDM2SkCM *m_pTable;

// �R���X�g���N�V����
public:
	CSelCMDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSelCMDlg)
	enum { IDD = IDD_SEL_CM };
	CListCtrl	m_wndCM;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSelCMDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSelCMDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkListCm(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SELCMDLG_H__41B887C6_7817_42EF_97EC_82E56A06D484__INCLUDED_)
