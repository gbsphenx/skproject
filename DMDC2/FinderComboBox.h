#if !defined(AFX_FINDERCOMBOBOX_H__6EB81188_FD62_4B4A_8F45_9F7EED9B4B03__INCLUDED_)
#define AFX_FINDERCOMBOBOX_H__6EB81188_FD62_4B4A_8F45_9F7EED9B4B03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FinderComboBox.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CFinderComboBox �E�B���h�E

class CFinderComboBox : public CComboBox
{
// �R���X�g���N�V����
public:
	CFinderComboBox();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CFinderComboBox)
	public:
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CFinderComboBox();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CFinderComboBox)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_FINDERCOMBOBOX_H__6EB81188_FD62_4B4A_8F45_9F7EED9B4B03__INCLUDED_)
