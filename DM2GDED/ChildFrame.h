#if !defined(AFX_CHILDFRAME_H__824EBE4A_D0E7_41FD_AED5_E102871A3E45__INCLUDED_)
#define AFX_CHILDFRAME_H__824EBE4A_D0E7_41FD_AED5_E102871A3E45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildFrame.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CChildFrame �t���[��

class CChildFrame : public CMDIChildWnd
{
	// 
	DECLARE_DYNCREATE(CChildFrame)

protected:
	// 
	CChildFrame();           // ���I�����Ɏg�p�����v���e�N�g �R���X�g���N�^�B

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂�

	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	virtual ~CChildFrame();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CHILDFRAME_H__824EBE4A_D0E7_41FD_AED5_E102871A3E45__INCLUDED_)
