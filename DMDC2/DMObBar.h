#if !defined(AFX_DMOBBAR_H__F1D6A595_04C0_4DDD_ACF6_B576D8EE6CD6__INCLUDED_)
#define AFX_DMOBBAR_H__F1D6A595_04C0_4DDD_ACF6_B576D8EE6CD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMObBar.h : �w�b�_�[ �t�@�C��
//

#include "DMObTreeView.h"

/////////////////////////////////////////////////////////////////////////////
// CDMObBar �E�B���h�E

typedef CSizingControlBarCF baseCDMObBar;

class CDMObBar : public baseCDMObBar
{
	// 
	CFont m_fontNice;
	// 
	CDMObTreeView m_wndTree;
	// 
	CToolBar m_wndToolBar;
	// 
	CMenu m_browseMenu;

public:
	// 
	CDMObBar(): baseCDMObBar()
	{

	}
	// 
	BOOL Create(CDMEdCtx *pEdCtx, CWnd *lpParent, LPCTSTR lpszWindowText, DWORD dwStyle, UINT nID)
	{
		m_szVert = CSize(250, 250);
		if (!baseCDMObBar::Create(lpszWindowText, lpParent, nID, dwStyle))
			return FALSE;
		if (!m_wndTree.Create(pEdCtx, WS_CHILD|WS_VISIBLE, this, AFX_IDW_PANE_FIRST))
			return FALSE;
		return TRUE;
	}

// �R���X�g���N�V����
public:

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDMObBar)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CDMObBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewDictree();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DMOBBAR_H__F1D6A595_04C0_4DDD_ACF6_B576D8EE6CD6__INCLUDED_)
