#if !defined(AFX_SHOWCASEFRAME_H__0BB3B7C9_C747_4A75_A027_629F3AE5012B__INCLUDED_)
#define AFX_SHOWCASEFRAME_H__0BB3B7C9_C747_4A75_A027_629F3AE5012B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowcaseFrame.h : �w�b�_�[ �t�@�C��
//

#include "ShowcaseLv.h"

/////////////////////////////////////////////////////////////////////////////
// CShowcaseFrame �t���[��

class CShowcaseFrame : public CFrameWnd
{
public:
	// 
	CShowcaseLv m_vw;
	// 
	CToolBar m_wndToolBar;

	// 
	CShowcaseFrame();
	// 
	virtual ~CShowcaseFrame();

	// 
	virtual void PostNcDestroy() { }

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CShowcaseFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CShowcaseFrame)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SHOWCASEFRAME_H__0BB3B7C9_C747_4A75_A027_629F3AE5012B__INCLUDED_)
