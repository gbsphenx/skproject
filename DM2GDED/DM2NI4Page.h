#if !defined(AFX_DM2NI4PAGE_H__7CAFB188_1788_414D_802E_AD0C564DBC26__INCLUDED_)
#define AFX_DM2NI4PAGE_H__7CAFB188_1788_414D_802E_AD0C564DBC26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2NI4Page.h : �w�b�_�[ �t�@�C��
//

#include "DM2NIBPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2NI4Page �_�C�A���O

class CDM2NI4Page : public CDM2NIBPage
{
	// 
	void UpdateVw();
	// 
	DECLARE_DYNCREATE(CDM2NI4Page)

// �R���X�g���N�V����
public:
	CDM2NI4Page();
	~CDM2NI4Page();

protected:
	// 
	virtual int GetStage() { return 3; }

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDM2NI4Page)
	enum { IDD = IDD_PP_NI4 };
		// ���� - ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
		//    ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDM2NI4Page)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDM2NI4Page)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DM2NI4PAGE_H__7CAFB188_1788_414D_802E_AD0C564DBC26__INCLUDED_)
