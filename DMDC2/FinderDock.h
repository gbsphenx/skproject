#if !defined(AFX_FINDERDOCK_H__72AD5D3F_2017_402C_96B6_72EA0926C2B4__INCLUDED_)
#define AFX_FINDERDOCK_H__72AD5D3F_2017_402C_96B6_72EA0926C2B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FinderDock.h : �w�b�_�[ �t�@�C��
//

#include "FinderFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CFinderDock �E�B���h�E

class CFinderDock : public CSizingControlBarCF
{
public:
	// 
	CFinderFrm m_frm;

	// 
	CFinderDock();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CFinderDock)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CFinderDock();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CFinderDock)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_FINDERDOCK_H__72AD5D3F_2017_402C_96B6_72EA0926C2B4__INCLUDED_)
