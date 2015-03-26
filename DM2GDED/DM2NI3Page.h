#if !defined(AFX_DM2NI3PAGE_H__368761BB_6E29_454F_9505_43B377EE472F__INCLUDED_)
#define AFX_DM2NI3PAGE_H__368761BB_6E29_454F_9505_43B377EE472F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2NI3Page.h : �w�b�_�[ �t�@�C��
//

#include "DM2NIBPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2NI3Page �_�C�A���O

class CDM2NI3Page : public CDM2NIBPage
{
	// 
	void UpdateVw(bool fRecalc = false);
	// 
	DECLARE_DYNCREATE(CDM2NI3Page)

// �R���X�g���N�V����
public:
	CDM2NI3Page();
	~CDM2NI3Page();

protected:
	// 
	virtual int GetStage() { return 3; }

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDM2NI3Page)
	enum { IDD = IDD_PP_NI3 };
	int		m_iKeep;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDM2NI3Page)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDM2NI3Page)
	//}}AFX_MSG
	afx_msg void OnRadioKeepPal(UINT nID);
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DM2NI3PAGE_H__368761BB_6E29_454F_9505_43B377EE472F__INCLUDED_)
