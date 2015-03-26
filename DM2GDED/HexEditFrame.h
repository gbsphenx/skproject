#if !defined(AFX_HEXEDITFRAME_H__E9A865E2_D6AC_4305_9393_0130C8CB8A9F__INCLUDED_)
#define AFX_HEXEDITFRAME_H__E9A865E2_D6AC_4305_9393_0130C8CB8A9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HexEditFrame.h : �w�b�_�[ �t�@�C��
//

#include "HexEditCB.h"

class CHexEditView;

/////////////////////////////////////////////////////////////////////////////
// CHexEditFrame �t���[��

class CHexEditFrame : public CFrameWnd
{
	// 
	CHexEditView *m_pView;
	// 
	DECLARE_DYNAMIC(CHexEditFrame)

public:
	// 
	CHexEditFrame();
	// 
	virtual ~CHexEditFrame();
	// 
	CHexEditCB *GetHexEditCB();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CHexEditFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CHexEditFrame)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern void OnDeleteHexEditFrame(CHexEditFrame *pWnd);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_HEXEDITFRAME_H__E9A865E2_D6AC_4305_9393_0130C8CB8A9F__INCLUDED_)
