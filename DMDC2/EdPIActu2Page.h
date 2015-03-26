#if !defined(AFX_EDPIACTU2PAGE_H__4525E0D3_FDE3_406F_8BE3_0322B89869C6__INCLUDED_)
#define AFX_EDPIACTU2PAGE_H__4525E0D3_FDE3_406F_8BE3_0322B89869C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIActu2Page.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CEdPIActu2Page �_�C�A���O

#include "EdPIDlg.h"
#include "DM2Ornate1ComboBox.h"

class CEdPIActu2Page : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPIActu2Page)

public:
	// 
	CEdPIActu2Page();

	// 
	virtual bool IsAcc(UINT nID);
	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdPIActu2Page)
	enum { IDD = IDD_PI_ACTU2 };
	CDM2Ornate1ComboBox	m_wndOrnate;
	CSpinButtonCtrl	m_wndDelay;
	BOOL	m_fBeep;
	BOOL	m_fInverse;
	BOOL	m_fSwIsOn;
	int		m_iOrnate;
	UINT	m_nDelay;
	int		m_iAT;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CEdPIActu2Page)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdPIActu2Page)
	afx_msg void OnSelchangeComboOrnate();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDPIACTU2PAGE_H__4525E0D3_FDE3_406F_8BE3_0322B89869C6__INCLUDED_)
