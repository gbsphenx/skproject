#if !defined(AFX_DMLVMETRBAR_H__849826E0_29C0_4E22_9466_AF15BDE00B21__INCLUDED_)
#define AFX_DMLVMETRBAR_H__849826E0_29C0_4E22_9466_AF15BDE00B21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMLvMetrBar.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDMLvMetrBar �E�B���h�E

class CDMLvMetrBar : public CDialogBar
{
	// 
	CFont m_fontUl;

// �R���X�g���N�V����
public:
	CDMLvMetrBar();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDMLvMetrBar)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CDMLvMetrBar();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CDMLvMetrBar)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	virtual LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DMLVMETRBAR_H__849826E0_29C0_4E22_9466_AF15BDE00B21__INCLUDED_)
