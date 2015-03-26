#if !defined(AFX_RUNESELSTATIC_H__3EB2AEB7_DD97_42FA_A79D_A58565C2CEF7__INCLUDED_)
#define AFX_RUNESELSTATIC_H__3EB2AEB7_DD97_42FA_A79D_A58565C2CEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RuneSelStatic.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CRuneSelStatic �E�B���h�E

class CRuneSelStatic : public CStatic
{
	CBitmap m_bmRunes;
	CRect m_rcBox;
	int m_iSel;

// �R���X�g���N�V����
public:
	CRuneSelStatic();

	void SetSel(int i) {
		m_iSel = i;

		if (m_hWnd != NULL && IsWindow(m_hWnd))
			Invalidate();
	}

	int GetSel() {
		return m_iSel;
	}

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CRuneSelStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CRuneSelStatic();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CRuneSelStatic)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_RUNESELSTATIC_H__3EB2AEB7_DD97_42FA_A79D_A58565C2CEF7__INCLUDED_)
