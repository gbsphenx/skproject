#if !defined(AFX_EDPICLOTHPAGE_H__024E572A_C3CC_4A37_B731_C5679721EF39__INCLUDED_)
#define AFX_EDPICLOTHPAGE_H__024E572A_C3CC_4A37_B731_C5679721EF39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIClothPage.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CEdPIClothPage �_�C�A���O

#include "EdPIDlg.h"

class CEdPIClothPage : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPIClothPage)

public:
	// 
	CEdPIClothPage();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdPIClothPage)
	enum { IDD = IDD_PI_CLOTH };
	CComboBox	m_wndSel;
	BOOL	m_fA;
	BOOL	m_fB;
	BOOL	m_fC;
	BOOL	m_fD;
	BOOL	m_fE;
	BOOL	m_fF;
	BOOL	m_f8;
	BOOL	m_f9;
	int		m_iSel;
	BOOL	m_f7;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CEdPIClothPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdPIClothPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDPICLOTHPAGE_H__024E572A_C3CC_4A37_B731_C5679721EF39__INCLUDED_)
