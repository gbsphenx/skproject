#if !defined(AFX_EDPIACTU3PAGE_H__D038A9E7_0EBE_4BDC_B7C7_A05E851AAE63__INCLUDED_)
#define AFX_EDPIACTU3PAGE_H__D038A9E7_0EBE_4BDC_B7C7_A05E851AAE63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIActu3Page.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CEdPIActu3Page �_�C�A���O

#include "EdPIDlg.h"

class CEdPIActu3Page : public CEdPIDlg
{
	// 
	int m_a1t, m_a3t;

	// 
	void Active1(UINT nID);

	// 
	DECLARE_DYNCREATE(CEdPIActu3Page)

public:
	// 
	CEdPIActu3Page();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdPIActu3Page)
	enum { IDD = IDD_PI_ACTU3 };
	CSliderCtrl	m_wndStr;
	CStatic	m_wndStaticMap;
	CEdit	m_wndMap;
	UINT	m_iMap;
	UINT	m_iX;
	UINT	m_iY;
	int		m_nStr;
	int		m_iDir;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CEdPIActu3Page)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdPIActu3Page)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRef();
	afx_msg void OnButtonSelf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDPIACTU3PAGE_H__D038A9E7_0EBE_4BDC_B7C7_A05E851AAE63__INCLUDED_)
