#if !defined(AFX_EDITDMTILEDLG_H__DAE35316_2398_4735_9CBD_C9899EC92DD7__INCLUDED_)
#define AFX_EDITDMTILEDLG_H__DAE35316_2398_4735_9CBD_C9899EC92DD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditDMTileDlg.h : �w�b�_�[ �t�@�C��
//

#include "DDAT.h"

/////////////////////////////////////////////////////////////////////////////
// CEditDMRootSel �_�C�A���O

class CEditDMRootSel : public CDialog
{
public:
	// 
	RN m_rnNext;
	// 
	void OnOK();

// �R���X�g���N�V����
public:
	CEditDMRootSel(RN rnNext, CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEditDMRootSel)
	enum { IDD = IDD_EDITROOTSEL };
	CComboBox	m_wndNextItem;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditDMRootSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEditDMRootSel)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDITDMTILEDLG_H__DAE35316_2398_4735_9CBD_C9899EC92DD7__INCLUDED_)
