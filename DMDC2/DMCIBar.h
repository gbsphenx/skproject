#if !defined(AFX_DMCIBAR_H__4ACE1777_B585_4A3A_BDE8_B63846A43B0F__INCLUDED_)
#define AFX_DMCIBAR_H__4ACE1777_B585_4A3A_BDE8_B63846A43B0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMCIBar.h : �w�b�_�[ �t�@�C��
//

#include "DMCITree.h"

/////////////////////////////////////////////////////////////////////////////
// CDMCIBar �E�B���h�E

typedef CSizingControlBarG baseCDMCIBar;

class CDMCIBar : public baseCDMCIBar
{
	// 
	CDMCITreeView m_wndTree;

public:
	// 
	BOOL Create(CDMEdCtx *pEdCtx, CWnd *lpParent, LPCTSTR lpszWindowText, DWORD dwStyle, UINT nID)
	{
		m_szHorz = m_szVert = CSize(150, 150);
		m_szFloat = CSize(300, 400);
		if (!baseCDMCIBar::Create(lpszWindowText, lpParent, nID, dwStyle))
			return FALSE;
		if (!m_wndTree.Create(pEdCtx, WS_CHILD|WS_VISIBLE, this, AFX_IDW_PANE_FIRST))
			return FALSE;
		return TRUE;
	}

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDMCIBar)
	//}}AFX_VIRTUAL

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CDMCIBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DMCIBAR_H__4ACE1777_B585_4A3A_BDE8_B63846A43B0F__INCLUDED_)
