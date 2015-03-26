#if !defined(AFX_EDPIDOORPAGE_H__3A3EBD06_1542_4D9C_98D1_54C129188AC9__INCLUDED_)
#define AFX_EDPIDOORPAGE_H__3A3EBD06_1542_4D9C_98D1_54C129188AC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIDoorPage.h : �w�b�_�[ �t�@�C��
//

#include "EdPIDlg.h"
#include "DM2Ornate1ComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CEdPIDoorPage �_�C�A���O

class CEdPIDoorPage : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPIDoorPage)

public:
	// 
	CEdPIDoorPage();

	// 
	virtual bool IsAcc(UINT nID);
	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdPIDoorPage)
	enum { IDD = IDD_PI_DOOR };
	CDM2Ornate1ComboBox	m_wndDoor;
	CDM2Ornate1ComboBox	m_wndOrnate;
	BOOL	m_fBashOk;
	BOOL	m_fDestroyOk;
	BOOL	m_fDoorBtn;
	BOOL	m_fDoor2;
	BOOL	m_fA;
	BOOL	m_fB;
	BOOL	m_fC;
	BOOL	m_fD;
	BOOL	m_fE;
	BOOL	m_fF;
	BOOL	m_f9;
	BOOL	m_fVert;
	int		m_iOrnate;
	int		m_iDoor;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CEdPIDoorPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdPIDoorPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckDoorType();
	afx_msg void OnSelchangeComboDoor();
	afx_msg void OnSelchangeComboOrnate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDPIDOORPAGE_H__3A3EBD06_1542_4D9C_98D1_54C129188AC9__INCLUDED_)
