#if !defined(AFX_EDTILEFALSEDLG_H__6C639953_2097_44F0_860C_D2C6D0AC74DF__INCLUDED_)
#define AFX_EDTILEFALSEDLG_H__6C639953_2097_44F0_860C_D2C6D0AC74DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdTileFalseDlg.h : �w�b�_�[ �t�@�C��
//

#include "EdTileDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CEdTileFalseDlg �_�C�A���O

class CEdTileFalseDlg : public CEdTileDlg
{
// �R���X�g���N�V����
public:
	CEdTileFalseDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdTileFalseDlg)
	enum { IDD = IDD_TILE_FALSE };
	BOOL	m_fSw1;
	BOOL	m_fSw2;
	BOOL	m_fSw3;
	BOOL	m_fSw4;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEdTileFalseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdTileFalseDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCheckSw1(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDTILEFALSEDLG_H__6C639953_2097_44F0_860C_D2C6D0AC74DF__INCLUDED_)
