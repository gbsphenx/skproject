#if !defined(AFX_EDPINOPAGE_H__8C198EBC_7DA2_46CE_9BE4_9803B7D3EB2C__INCLUDED_)
#define AFX_EDPINOPAGE_H__8C198EBC_7DA2_46CE_9BE4_9803B7D3EB2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPINoPage.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CEdPINoPage �_�C�A���O

#include "EdPIDlg.h"

class CEdPINoPage : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPINoPage)

public:
	// 
	CEdPINoPage();

	// 
	virtual bool Revert(bool)
	{
		return true;
	}
	// 
	virtual bool RevertRecord(DMDBU &, bool)
	{
		return true;
	}

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdPINoPage)
	enum { IDD = IDD_PI_NO };
		// ���� - ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
		//    ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CEdPINoPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdPINoPage)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDPINOPAGE_H__8C198EBC_7DA2_46CE_9BE4_9803B7D3EB2C__INCLUDED_)
