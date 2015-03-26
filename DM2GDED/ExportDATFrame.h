#if !defined(AFX_EXPORTDATFRAME_H__3CD0C5F2_D37A_4553_B8D9_FD94FBE074EE__INCLUDED_)
#define AFX_EXPORTDATFRAME_H__3CD0C5F2_D37A_4553_B8D9_FD94FBE074EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportDATFrame.h : �w�b�_�[ �t�@�C��
//

#include "FlatSplitterWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CExportDATFrame �t���[��

class CExportDATFrame : public CFrameWnd
{
	// 
	CFlatSplitterWnd m_wndSplitter;
	// 
	CToolBar m_wndToolBar;
	// 
	CFrameWnd *m_pMainWnd;
	// 
	CStatusBar m_wndStatusBar;

	// 
	DECLARE_DYNCREATE(CExportDATFrame)

public:
	// 
	CExportDATFrame();
	// 
	void StartModal(CWnd *pParentWnd);

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CExportDATFrame)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	virtual ~CExportDATFrame();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CExportDATFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EXPORTDATFRAME_H__3CD0C5F2_D37A_4553_B8D9_FD94FBE074EE__INCLUDED_)
