#if !defined(AFX_SHOWCASEDOCK_H__CA11594C_B9C4_4F7F_BEB0_4153849FE0E7__INCLUDED_)
#define AFX_SHOWCASEDOCK_H__CA11594C_B9C4_4F7F_BEB0_4153849FE0E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowcaseDock.h : �w�b�_�[ �t�@�C��
//

#include "scbarcf.h"
#include "ShowcaseFrame.h"

/////////////////////////////////////////////////////////////////////////////
// CShowcaseDock �E�B���h�E

class CShowcaseDock : public CSizingControlBarCF
{
public:
	// 
	CShowcaseFrame m_frame;

	// 
	CShowcaseDock();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CShowcaseDock)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CShowcaseDock();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CShowcaseDock)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SHOWCASEDOCK_H__CA11594C_B9C4_4F7F_BEB0_4153849FE0E7__INCLUDED_)
