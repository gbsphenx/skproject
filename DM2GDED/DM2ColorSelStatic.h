#if !defined(AFX_DM2COLORSELSTATIC_H__0239B240_019C_4461_B762_D470B4546249__INCLUDED_)
#define AFX_DM2COLORSELSTATIC_H__0239B240_019C_4461_B762_D470B4546249__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2ColorSelStatic.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDM2ColorSelStatic �E�B���h�E

class CDM2ColorSelStatic : public CStatic
{
	// 
	int m_iSel;
	// 
	CRect m_rcBox[256];

	// 
	void OnSetSel(int iSel, bool fNotify)
	{
		m_iSel = iSel;

		ASSERT(GetParent() != NULL);

		GetParent()->PostMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), STN_DBLCLK), (LPARAM)(HWND)*this);
	}

public:
	// 
	CDM2ColorSelStatic();
	// 
	void SetSel(int iSel)
	{
		m_iSel = iSel;
		Invalidate();
	}
	// 
	int GetSel()
	{
		return m_iSel;
	}
	// 
	void Init();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDM2ColorSelStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CDM2ColorSelStatic)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DM2COLORSELSTATIC_H__0239B240_019C_4461_B762_D470B4546249__INCLUDED_)
