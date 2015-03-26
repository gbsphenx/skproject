#if !defined(AFX_DM2EDPICFRAME_H__CA28D1AC_1232_4E31_AAB8_B2CF837C07DF__INCLUDED_)
#define AFX_DM2EDPICFRAME_H__CA28D1AC_1232_4E31_AAB8_B2CF837C07DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2EdPicFrame.h : �w�b�_�[ �t�@�C��
//

#include "DM2RPtr.h"
#include "GDEdCtx.h"
#include "DM2VwPicVwPal.h"
#include "ToolBarEx.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2EdPicFrame �t���[��

class CDM2EdPicFrame : public CFrameWnd
{
	// 
	typedef map<CDocument *, CDM2EdPicFrame *> DocMap;

	// 
	CDM2SPtr<CGDEdCtx> m_pEdCtx;
	// 
	CDM2VwPicVwPal m_wndVwPal;
	// 
	CToolBarEx m_wndToolBar;

	// 
	static DocMap m_map;

	// 
	DECLARE_DYNAMIC(CDM2EdPicFrame)

public:
	// 
	CDM2EdPicFrame(CGDEdCtx *pEdCtx);
	// 
	CDM2VwPicVwPal &GetVwPal() { return m_wndVwPal; }
	// 
	static CDM2EdPicFrame *CreateNew(CDocument *pDoc, CGDEdCtx *pEdCtx, CGDEntry pos);
	// 
	static void OnCloseDocument(CDocument *pDoc);

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDM2EdPicFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	virtual ~CDM2EdPicFrame();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDM2EdPicFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DM2EDPICFRAME_H__CA28D1AC_1232_4E31_AAB8_B2CF837C07DF__INCLUDED_)
