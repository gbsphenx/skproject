#if !defined(AFX_EDTILETELEDLG_H__E32A01DF_72B0_411D_A504_D8EBF37CC5E2__INCLUDED_)
#define AFX_EDTILETELEDLG_H__E32A01DF_72B0_411D_A504_D8EBF37CC5E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdTileTeleDlg.h : �w�b�_�[ �t�@�C��
//

#include "EdTileDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CEdTileTeleDlg �_�C�A���O

class CEdTileTeleDlg : public CEdTileDlg
{
// �R���X�g���N�V����
public:
	CEdTileTeleDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdTileTeleDlg)
	enum { IDD = IDD_TILE_TELE };
	BOOL	m_fSw1;
	BOOL	m_fSw2;
	BOOL	m_fSw3;
	BOOL	m_fSw4;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEdTileTeleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdTileTeleDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCheckSw1(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDTILETELEDLG_H__E32A01DF_72B0_411D_A504_D8EBF37CC5E2__INCLUDED_)
