#if !defined(AFX_EXPORTEDITCOMMENTDLG_H__0FAB3415_4B1B_46BE_8610_320A3476B7DF__INCLUDED_)
#define AFX_EXPORTEDITCOMMENTDLG_H__0FAB3415_4B1B_46BE_8610_320A3476B7DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportEditCommentDlg.h : �w�b�_�[ �t�@�C��
//

#include "ExportDATHintDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CExportEditCommentDlg �_�C�A���O

class CExportEditCommentDlg : public CDialog
{
public:
	// 
	CExportEditCommentDlg(CWnd* pParent = NULL);
	// 
	void Revert(CExportDATComment &rEx, bool fRevert);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CExportEditCommentDlg)
	enum { IDD = IDD_EDIT_COMMENT };
	CString	m_strAuthor;
	CString	m_strComment;
	CString	m_strDate;
	CString	m_strName;
	CString	m_strPlatform;
	CString	m_strVer;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CExportEditCommentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CExportEditCommentDlg)
	afx_msg void OnButtonToday();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EXPORTEDITCOMMENTDLG_H__0FAB3415_4B1B_46BE_8610_320A3476B7DF__INCLUDED_)
