#if !defined(AFX_GETBACKBUTTON_H__FA05EB80_3E6F_4BFB_9684_6182601B82EB__INCLUDED_)
#define AFX_GETBACKBUTTON_H__FA05EB80_3E6F_4BFB_9684_6182601B82EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetBackButton.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CGetBackButton �E�B���h�E

class CGetBackButton : public CWnd
{
	// 
	CBitmap m_ico;
	HCURSOR m_cursor;
	// 
	bool fDirIsUp;
	bool m_fInvert;

// �R���X�g���N�V����
public:
	// 
	CGetBackButton();
	// 
	bool Create(CWnd *pParentWnd);
	// 
	void Show(CWnd *pTar);
	void Hide();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CGetBackButton)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CGetBackButton();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CGetBackButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_GETBACKBUTTON_H__FA05EB80_3E6F_4BFB_9684_6182601B82EB__INCLUDED_)
