#if !defined(AFX_EDPITEXTPAGE_H__6E2E70CE_8113_4D94_AD9A_185CAE8AE526__INCLUDED_)
#define AFX_EDPITEXTPAGE_H__6E2E70CE_8113_4D94_AD9A_185CAE8AE526__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPITextPage.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CEdPITextPage �_�C�A���O

#include "EdPIDlg.h"
#include "DM2OrnatePvw.h"
#include "ExtComboBox.h"

class CEdPITextPage : public CEdPIDlg
{
	// 
	int m_iSel;

	int m_trpt;

	// 
	DECLARE_DYNCREATE(CEdPITextPage)

public:
	// 
	CEdPITextPage();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

	void UpdateUseType();
	void UpdateLargeType();
	void UpdatePoint();

	void ApplyText();
	void ApplyUseType();
	void ApplySel();

	void ValidateLargeType();

	void enableWindows(const UINT *nIDs, bool fEnabled);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdPITextPage)
	enum { IDD = IDD_PI_TEXT };
	CComboBox	m_wndUse;
	CDM2OrnatePvw	m_wndOrnatePvw;
	CExtComboBox	m_wndSel;
	BOOL	m_f0;
	BOOL	m_f1;
	BOOL	m_fB;
	BOOL	m_fC;
	BOOL	m_fD;
	BOOL	m_fE;
	BOOL	m_fF;
	BOOL	m_f2;
	int		m_iMode;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CEdPITextPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdPITextPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboText();
	afx_msg void OnSelchangeComboUse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnCheckN15(UINT nID);
	afx_msg void OnRadioM1(UINT nID);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDPITEXTPAGE_H__6E2E70CE_8113_4D94_AD9A_185CAE8AE526__INCLUDED_)
