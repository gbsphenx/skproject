#if !defined(AFX_EDTILEDOORDLG_H__6E67B895_3E4D_4365_9D32_7B3CCABA966F__INCLUDED_)
#define AFX_EDTILEDOORDLG_H__6E67B895_3E4D_4365_9D32_7B3CCABA966F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdTileDoorDlg.h : �w�b�_�[ �t�@�C��
//

#include "EdTileDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CEdTileDoorDlg �_�C�A���O

class CEdTileDoorDlg : public CEdTileDlg
{
// �R���X�g���N�V����
public:
	CEdTileDoorDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdTileDoorDlg)
	enum { IDD = IDD_TILE_DOOR };
	BOOL	m_fSw1;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEdTileDoorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdTileDoorDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCheckSw1(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDTILEDOORDLG_H__6E67B895_3E4D_4365_9D32_7B3CCABA966F__INCLUDED_)
