#if !defined(AFX_EDPITELE_H__CF1F51EC_0F10_4442_A9BC_492FB6E7FD1F__INCLUDED_)
#define AFX_EDPITELE_H__CF1F51EC_0F10_4442_A9BC_492FB6E7FD1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPITele.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CEdPITelePage �_�C�A���O

#include "EdPIDlg.h"

class CEdPITelePage : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPITelePage)

public:
	// 
	CEdPITelePage();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdPITelePage)
	enum { IDD = IDD_PI_TELE };
	int		m_iRot;
	int		m_iTar;
	BOOL	m_fBeep;
	UINT	m_nMap;
	UINT	m_iX;
	UINT	m_iY;
	int		m_iTF;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CEdPITelePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdPITelePage)
	afx_msg void OnButtonRef();
	afx_msg void OnButtonSelf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDPITELE_H__CF1F51EC_0F10_4442_A9BC_492FB6E7FD1F__INCLUDED_)
