#if !defined(AFX_DM2NIBPAGE_H__9AB5491D_2CD3_4DB7_B121_6546363F3A5F__INCLUDED_)
#define AFX_DM2NIBPAGE_H__9AB5491D_2CD3_4DB7_B121_6546363F3A5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2NIBPage.h : �w�b�_�[ �t�@�C��
//

#include "DM2NICtx.h"
#include "DM2NIPvFrame.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2NIBPage �E�B���h�E

class CDM2NIBPage : public CPropertyPage
{
	// 
	DECLARE_DYNAMIC(CDM2NIBPage)

public:
	// 
	explicit CDM2NIBPage(UINT nIDTemplate, UINT nIDCaption = 0): CPropertyPage(nIDTemplate, nIDCaption)
	{
		m_ctx = NULL;
	}

protected:
	// 
	CDM2NICtx *m_ctx;
	// 
	CPropertySheet *m_ppsh;
	// 
	CDM2NIPvFrame m_frame;

	// 
	bool MkCtx();
	// 
	bool MkPSH();
	// 
	bool MkEmbeddedCtl();

	// 
	virtual int GetStage() = NULL;

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDM2NIBPage)
	public:
	virtual BOOL OnSetActive();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CDM2NIBPage)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DM2NIBPAGE_H__9AB5491D_2CD3_4DB7_B121_6546363F3A5F__INCLUDED_)
