#if !defined(AFX_EDPIMISCPAGE_H__9C8402AC_7252_46B8_B978_CDDAB64986DE__INCLUDED_)
#define AFX_EDPIMISCPAGE_H__9C8402AC_7252_46B8_B978_CDDAB64986DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIMiscPage.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CEdPIMiscPage �_�C�A���O

#include "EdPIDlg.h"

class CEdPIMiscPage : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPIMiscPage)

public:
	// 
	CEdPIMiscPage();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdPIMiscPage)
	enum { IDD = IDD_PI_MISC };
	CComboBox	m_wndSel;
	BOOL	m_fA;
	BOOL	m_fB;
	BOOL	m_fC;
	BOOL	m_fD;
	BOOL	m_f8;
	BOOL	m_f9;
	int		m_iSel;
	int		m_iState;
	BOOL	m_f7;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CEdPIMiscPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdPIMiscPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDPIMISCPAGE_H__9C8402AC_7252_46B8_B978_CDDAB64986DE__INCLUDED_)
