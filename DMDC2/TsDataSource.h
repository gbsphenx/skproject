#if !defined(AFX_TSDATASOURCE_H__A6FD9FC1_2C46_4DEE_AE9D_438AE1567538__INCLUDED_)
#define AFX_TSDATASOURCE_H__A6FD9FC1_2C46_4DEE_AE9D_438AE1567538__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TsDataSource.h : �w�b�_�[ �t�@�C��
//

#include "ShowcaseData.h"

/////////////////////////////////////////////////////////////////////////////
// CTsDataSource �R�}���h �^�[�Q�b�g

class CTsDataSource : public COleDataSource
{
public:
	// 
	static UINT cfShowcase;

	// 
	CTsDataSource();
	// 
	void CacheShowcase(ShowcaseData &ref);

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTsDataSource)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTsDataSource)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TSDATASOURCE_H__A6FD9FC1_2C46_4DEE_AE9D_438AE1567538__INCLUDED_)
