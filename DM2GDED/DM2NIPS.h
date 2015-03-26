#if !defined(AFX_DM2NIPS_H__AB990AE9_BC7E_4EB2_86F5_99D39BC6550F__INCLUDED_)
#define AFX_DM2NIPS_H__AB990AE9_BC7E_4EB2_86F5_99D39BC6550F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2NIPS.h : �w�b�_�[ �t�@�C��
//

#include "DM2NICtx.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2NIPS

class CDM2NIPS : public CPropertySheet
{
	// 
	CDM2NICtx &m_ctx;

	// 
	DECLARE_DYNAMIC(CDM2NIPS)

public:
	// 
	CDM2NIPS(CDM2NICtx &rContext, CWnd* pParentWnd = NULL)
		: CPropertySheet(_T("Edit image"), pParentWnd, 0)
		, m_ctx(rContext)
	{
		SetWizardMode();
	}

	// 
	CDM2NICtx *GetCtx() { return &m_ctx; }

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDM2NIPS)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CDM2NIPS)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DM2NIPS_H__AB990AE9_BC7E_4EB2_86F5_99D39BC6550F__INCLUDED_)
