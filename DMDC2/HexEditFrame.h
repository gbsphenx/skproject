#if !defined(AFX_HEXEDITFRAME_H__6AADE43C_DAF9_4F0B_8A42_7672E66846CE__INCLUDED_)
#define AFX_HEXEDITFRAME_H__6AADE43C_DAF9_4F0B_8A42_7672E66846CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HexEditFrame.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CHexEditFrame �t���[��

class CHexEditFrame : public CFrameWnd
{
	// 
	HWND m_hWndModalTar;
public:
	// 
	static CHexEditFrame *Create1(CWnd *pParentWnd);
	// 
	int StayModal()
	{
		return RunModalLoop(MLF_SHOWONIDLE);
	}

	DECLARE_DYNCREATE(CHexEditFrame)
protected:
	CHexEditFrame();           // ���I�����Ɏg�p�����v���e�N�g �R���X�g���N�^�B

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CHexEditFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	virtual ~CHexEditFrame();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CHexEditFrame)
	afx_msg void OnAppClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_HEXEDITFRAME_H__6AADE43C_DAF9_4F0B_8A42_7672E66846CE__INCLUDED_)
