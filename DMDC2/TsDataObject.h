#if !defined(AFX_TSDATAOBJECT_H__A2A7C114_A01A_4725_8667_19ED82DFEEDB__INCLUDED_)
#define AFX_TSDATAOBJECT_H__A2A7C114_A01A_4725_8667_19ED82DFEEDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TsDataObject.h : �w�b�_�[ �t�@�C��
//

#include "ShowcaseData.h"

/////////////////////////////////////////////////////////////////////////////
// CTsDataObject �R�}���h �^�[�Q�b�g

class CTsDataObject : public COleDataObject
{
public:
	// 
	CTsDataObject();

	// 
	bool GetShowcase(ShowcaseData &ref);

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTsDataObject)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTsDataObject)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TSDATAOBJECT_H__A2A7C114_A01A_4725_8667_19ED82DFEEDB__INCLUDED_)
