#if !defined(AFX_SPLASHFRAME_H__2885231F_FB90_428B_AF58_9E950F755AB8__INCLUDED_)
#define AFX_SPLASHFRAME_H__2885231F_FB90_428B_AF58_9E950F755AB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplashFrame.h : �w�b�_�[ �t�@�C��
//

#include "SplashVw.h"
#include "HintOfStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CSplashFrame �t���[��

class CSplashDialogBar : public CDialogBar
{
public:
	// 
	BOOL OnEraseBkgnd(CDC* pDC)
	{
		CRect rc;
		GetClientRect(&rc);
		pDC->FillSolidRect(&rc, RGB(0, 0, 0));

		return true;
	}
	// 
	HBRUSH OnCtlColor(
		CDC* pDC,
		CWnd* pWnd,
		UINT nCtlColor 
	)
	{
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255,255,206));
		return (HBRUSH)GetStockObject(BLACK_BRUSH);
	}

	// 
	DECLARE_MESSAGE_MAP()
};

class CSplashFrame : public CFrameWnd
{
public:
	// 
	CSplashVw m_vw;
	// 
	CSplashDialogBar m_bar;
	// 
	CHintOfStatic m_horz;

	//
	CSplashFrame();
	// 
	virtual ~CSplashFrame();
	// 
	void PostNcDestroy() { }


// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSplashFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSplashFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SPLASHFRAME_H__2885231F_FB90_428B_AF58_9E950F755AB8__INCLUDED_)
