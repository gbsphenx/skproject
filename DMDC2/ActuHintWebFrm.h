#if !defined(AFX_ACTUHINTWEBFRM_H__DC05ADC8_C96D_40F8_8F10_64DF05836AAC__INCLUDED_)
#define AFX_ACTUHINTWEBFRM_H__DC05ADC8_C96D_40F8_8F10_64DF05836AAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActuHintWebFrm.h : �w�b�_�[ �t�@�C��
//

#include "ActuHintWebVw.h"

/////////////////////////////////////////////////////////////////////////////
// CActuHintWebFrm �t���[��

class CActuHintWebFrm : public CFrameWnd
{
public:
	// 
	CActuHintWebVw m_vw;

	// 
	CActuHintWebFrm();
	virtual ~CActuHintWebFrm();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CActuHintWebFrm)
	protected:
	virtual void PostNcDestroy();
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CActuHintWebFrm)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_ACTUHINTWEBFRM_H__DC05ADC8_C96D_40F8_8F10_64DF05836AAC__INCLUDED_)
