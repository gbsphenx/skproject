#if !defined(AFX_REPLC1DLG_H__4B3837D9_4D79_44D7_B347_8DCCD269A401__INCLUDED_)
#define AFX_REPLC1DLG_H__4B3837D9_4D79_44D7_B347_8DCCD269A401__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReplC1Dlg.h : �w�b�_�[ �t�@�C��
//

#include "GDEntry.h"

/////////////////////////////////////////////////////////////////////////////
// CReplC1Dlg �_�C�A���O

class CReplC1Dlg : public CDialog
{
public:
	// 
	CGDEntry m_from, m_to;

	// 
	CReplC1Dlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CReplC1Dlg)
	enum { IDD = IDD_REPL_C1 };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CReplC1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CReplC1Dlg)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_REPLC1DLG_H__4B3837D9_4D79_44D7_B347_8DCCD269A401__INCLUDED_)
