#if !defined(AFX_EDTILEPITDLG_H__B01326E0_248E_4A15_8D9F_F5382F8EFD16__INCLUDED_)
#define AFX_EDTILEPITDLG_H__B01326E0_248E_4A15_8D9F_F5382F8EFD16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdTilePitDlg.h : �w�b�_�[ �t�@�C��
//

#include "EdTileDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CEdTilePitDlg �_�C�A���O

class CEdTilePitDlg : public CEdTileDlg
{
// �R���X�g���N�V����
public:
	CEdTilePitDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdTilePitDlg)
	enum { IDD = IDD_TILE_PIT };
	BOOL	m_fSw1;
	BOOL	m_fSw2;
	BOOL	m_fSw3;
	BOOL	m_fSw4;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEdTilePitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdTilePitDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCheckSw1(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDTILEPITDLG_H__B01326E0_248E_4A15_8D9F_F5382F8EFD16__INCLUDED_)
