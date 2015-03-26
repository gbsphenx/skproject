#if !defined(AFX_IMPORTRESDLG_H__D17BE755_B98B_44B0_ACCC_7CD17BF7F383__INCLUDED_)
#define AFX_IMPORTRESDLG_H__D17BE755_B98B_44B0_ACCC_7CD17BF7F383__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportResDlg.h : �w�b�_�[ �t�@�C��
//

#include "GDAT.h"

class CImportResRec
{
public:
	// 
	CString strText;

	// 
	CImportResRec() { }
	// 
	CImportResRec(LPCTSTR psz): strText(psz) { }
};

class CImportResRecList : public list<CImportResRec>
{
	// 
	void appendText(LPCTSTR pszText);

public:
	// 
	void ResAddSubject(LPCTSTR pszText);
	// 
	void ResAddText(LPCTSTR pszText);
	// 
	void ResAddEntryImpRes(const CGDEntry &pos, BOOL fRes);
	// 
	void ResAddUnsup8();
	// 
	void ResAddKillEntry(const CGDEntry &pos, bool fFromDAT);
	// 
	CString ResToStr();
};

/////////////////////////////////////////////////////////////////////////////
// CImportResDlg �_�C�A���O

class CImportResDlg : public CDialog, public CImportResRecList
{

public:
	// 
	CImportResDlg(CWnd* pParent = NULL);
	// 
	int DoModal(LPCTSTR pszRes);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CImportResDlg)
	enum { IDD = IDD_RES_IMPORT };
	CString	m_strLog;
	CString	m_strRes;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CImportResDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CImportResDlg)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_IMPORTRESDLG_H__D17BE755_B98B_44B0_ACCC_7CD17BF7F383__INCLUDED_)
