#if !defined(AFX_FINDERFRM_H__57F31302_1A7A_4806_A41F_6BA3ACF1673C__INCLUDED_)
#define AFX_FINDERFRM_H__57F31302_1A7A_4806_A41F_6BA3ACF1673C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FinderFrm.h : �w�b�_�[ �t�@�C��
//

#include "FinderVw.h"

/////////////////////////////////////////////////////////////////////////////
// CFinderFrm �t���[��

class CFinderFrm : public CFrameWnd
{
public:
	// 
	CFinderVw m_vw;
	CToolBar m_wndToolBar;

	// 
	CFinderFrm();
	virtual ~CFinderFrm();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CFinderFrm)
	protected:
	virtual void PostNcDestroy();
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CFinderFrm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_FINDERFRM_H__57F31302_1A7A_4806_A41F_6BA3ACF1673C__INCLUDED_)
