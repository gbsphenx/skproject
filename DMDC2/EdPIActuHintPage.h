#if !defined(AFX_EDPIACTUHINTPAGE_H__1E8EA11A_BB41_4435_9FC2_99766D35DFF1__INCLUDED_)
#define AFX_EDPIACTUHINTPAGE_H__1E8EA11A_BB41_4435_9FC2_99766D35DFF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIActuHintPage.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CEdPIActuHintPage �_�C�A���O

#include "EdPIDlg.h"
#include "AHRichEditCtrl.h"
#include "ActuHintWebFrm.h"

class CEdPIActuHintPage : public CEdPIDlg
{
	// 
	BYTE m_iActuType;
	// 
	BYTE m_lockWarp;
	// 
	CActuHintWebFrm m_webfrm;
	// 
	BYTE m_iDisplayActuType;

	// 
	void loadHintRtf();

	// 
	DECLARE_DYNCREATE(CEdPIActuHintPage)

public:
	// 
	CEdPIActuHintPage();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

	virtual void AfterRevert();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdPIActuHintPage)
	enum { IDD = IDD_PI_ACTUHINT };
	CButton	m_wndButton;
	CStatic	m_wndArea;
	CComboBox	m_wndSel;
	int		m_iSel;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEdPIActuHintPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdPIActuHintPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboType();
	virtual void OnCancel();
	afx_msg void OnButtonEdit();
	afx_msg void OnContextEditFp();
	afx_msg void OnContextEditNotepad();
	afx_msg void OnButtonSampler();
	//}}AFX_MSG
	LPARAM OnClickAHMT(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDPIACTUHINTPAGE_H__1E8EA11A_BB41_4435_9FC2_99766D35DFF1__INCLUDED_)
