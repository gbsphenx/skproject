#if !defined(AFX_EDPICHESTPAGE_H__BB0AFADD_19D2_4B83_B1E0_EFE566ABC89D__INCLUDED_)
#define AFX_EDPICHESTPAGE_H__BB0AFADD_19D2_4B83_B1E0_EFE566ABC89D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIChestPage.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CEdPIChestPage �_�C�A���O

#include "EdPIDlg.h"

class CEdPIChestPage : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPIChestPage)

public:
	// 
	CEdPIChestPage();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdPIChestPage)
	enum { IDD = IDD_PI_CHEST };
	CComboBox	m_wndSel;
	int		m_iSel;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CEdPIChestPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdPIChestPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDPICHESTPAGE_H__BB0AFADD_19D2_4B83_B1E0_EFE566ABC89D__INCLUDED_)
