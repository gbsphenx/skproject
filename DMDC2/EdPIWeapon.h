#if !defined(AFX_EDPIWEAPON_H__32E08F6C_9659_4B6B_ACF0_4C981B978CF1__INCLUDED_)
#define AFX_EDPIWEAPON_H__32E08F6C_9659_4B6B_ACF0_4C981B978CF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIWeapon.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CEdPIWeaponPage �_�C�A���O

#include "EdPIDlg.h"

class CEdPIWeaponPage : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPIWeaponPage)

public:
	// 
	CEdPIWeaponPage();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdPIWeaponPage)
	enum { IDD = IDD_PI_WEAPON };
	CComboBox	m_wndSel;
	BOOL	m_fE;
	BOOL	m_fF;
	BOOL	m_f8;
	BOOL	m_f9;
	int		m_nCharges;
	int		m_iSel;
	BOOL	m_f7;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CEdPIWeaponPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdPIWeaponPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDPIWEAPON_H__32E08F6C_9659_4B6B_ACF0_4C981B978CF1__INCLUDED_)
