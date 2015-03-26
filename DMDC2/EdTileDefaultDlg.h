#if !defined(AFX_EDTILEDEFAULT_H__E6B3DE1B_1AF3_4954_968B_76990EE1C877__INCLUDED_)
#define AFX_EDTILEDEFAULT_H__E6B3DE1B_1AF3_4954_968B_76990EE1C877__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdTileDefault.h : �w�b�_�[ �t�@�C��
//

#include "EdTileDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CEdTileDefaultDlg �_�C�A���O

class CEdTileDefaultDlg : public CEdTileDlg
{
// �R���X�g���N�V����
public:
	CEdTileDefaultDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdTileDefaultDlg)
	enum { IDD = IDD_TILE_NO };
	BOOL	m_fSw1;
	BOOL	m_fSw2;
	BOOL	m_fSw3;
	BOOL	m_fSw4;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEdTileDefaultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL
	afx_msg void OnCheckSw1(UINT nID);

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdTileDefaultDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDTILEDEFAULT_H__E6B3DE1B_1AF3_4954_968B_76990EE1C877__INCLUDED_)
