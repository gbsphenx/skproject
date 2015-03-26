#if !defined(AFX_EDTILESTAIRSDLG_H__82FC7305_9A5A_4F04_A29E_2A2849EF723E__INCLUDED_)
#define AFX_EDTILESTAIRSDLG_H__82FC7305_9A5A_4F04_A29E_2A2849EF723E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdTileStairsDlg.h : �w�b�_�[ �t�@�C��
//

#include "EdTileDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CEdTileStairsDlg �_�C�A���O

class CEdTileStairsDlg : public CEdTileDlg
{
// �R���X�g���N�V����
public:
	CEdTileStairsDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdTileStairsDlg)
	enum { IDD = IDD_TILE_STAIRS };
	BOOL	m_fSw1;
	BOOL	m_fSw2;
	BOOL	m_fSw3;
	BOOL	m_fSw4;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEdTileStairsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdTileStairsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCheckSw1(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDTILESTAIRSDLG_H__82FC7305_9A5A_4F04_A29E_2A2849EF723E__INCLUDED_)
