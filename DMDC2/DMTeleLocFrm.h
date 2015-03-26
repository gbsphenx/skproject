#if !defined(AFX_DMTELELOCFRM_H__35FDB0A3_43B0_4066_B966_47CFAF5E64D3__INCLUDED_)
#define AFX_DMTELELOCFRM_H__35FDB0A3_43B0_4066_B966_47CFAF5E64D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMTeleLocFrm.h : �w�b�_�[ �t�@�C��
//

#include "DMTeleLocVw.h"
#include "DDAT.h"

/////////////////////////////////////////////////////////////////////////////
// CDMTeleLocFrm �t���[��

class CDMTeleLocFrm : public CFrameWnd
{
	// 
	CPtrArray m_arrayWndz;
	// 
	int m_iModalStateStack;

public:
	// 
	CDMTeleLocVw m_view;

	// 
	CDMTeleLocFrm(CDDAT *pDDIf, AtlasPos_t ap, bool fMovable): m_view(pDDIf, ap, fMovable)
	{
		m_iModalStateStack = 0;
	}
	// 
	int DoModal(CWnd *pParentWnd = NULL);
	// 
	void EnterModalState();
	// 
	void LeaveModalState();

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDMTeleLocFrm)
	protected:
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDMTeleLocFrm)
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DMTELELOCFRM_H__35FDB0A3_43B0_4066_B966_47CFAF5E64D3__INCLUDED_)
