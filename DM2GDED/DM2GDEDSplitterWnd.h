#if !defined(AFX_DM2GDEDSPLITTERWND_H__F1E1EB09_5D27_4557_BF42_8E20B5F4558B__INCLUDED_)
#define AFX_DM2GDEDSPLITTERWND_H__F1E1EB09_5D27_4557_BF42_8E20B5F4558B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2GDEDSplitterWnd.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDSplitterWnd �����t���[��

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "FlatSplitterWnd.h"

class CDM2GDEDSplitterWnd : public CFlatSplitterWnd
{
	// 
	int m_iX, m_iY;

public:
	// 
	CDM2GDEDSplitterWnd();
	// 
	CWnd* GetActivePane(int* pRow = NULL, int* pCol = NULL);
	// 
	void SetActivePane(int row, int col, CWnd* pWnd = NULL);

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDM2GDEDSplitterWnd)
	protected:
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDM2GDEDSplitterWnd)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DM2GDEDSPLITTERWND_H__F1E1EB09_5D27_4557_BF42_8E20B5F4558B__INCLUDED_)
