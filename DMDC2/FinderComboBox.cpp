// FinderComboBox.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "exp00069.h"
#include "FinderComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFinderComboBox

CFinderComboBox::CFinderComboBox()
{
}

CFinderComboBox::~CFinderComboBox()
{
}


BEGIN_MESSAGE_MAP(CFinderComboBox, CComboBox)
	//{{AFX_MSG_MAP(CFinderComboBox)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFinderComboBox ���b�Z�[�W �n���h��

int CFinderComboBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	// TODO: �w�肳�ꂽ���ڂ̃\�[�g�̏��������߂邽�߂̃R�[�h��ǉ����Ă�������
	// return -1 = ���� 2 �̑O�ɍ��� 1 ���\�[�g
	// return 0 = ���� 1 �ƍ��� 2 �𓯎��Ƀ\�[�g
	// return 1 = ���� 2 �̌�ō��� 1 ���\�[�g

	return 0;
}
