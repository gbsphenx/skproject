#if !defined(AFX_DM2VWSCENEVW_H__1EC36C15_95EE_44C6_92C0_62E8D5F69B77__INCLUDED_)
#define AFX_DM2VWSCENEVW_H__1EC36C15_95EE_44C6_92C0_62E8D5F69B77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2VwSceneVw.h : �w�b�_�[ �t�@�C��
//

#include "DM2SIIVCtx.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2VwSceneVw �r���[

class CDM2VwSceneVw : public CScrollView, protected CDM2SIIVCtxCB
{
protected:
	// 
	int m_iScene;
	// 
	int m_iDM2SIIVT;
	// 
	CDM2SIIVCtx *m_pSIIVCtx;
	// 
	CRect m_rcScreen;
	// 
	CRect m_rcScene;
	// 
	DMGHLiIMGM m_imgm;

	// 
	CDM2VwSceneVw();

	// 
	void InitSIIVCtx(CDM2SIIVCtx *pSIIVCtx)
	{
		if (m_pSIIVCtx)
			m_pSIIVCtx->RemoveCB(this),
			m_pSIIVCtx->Release();
		if (m_pSIIVCtx = pSIIVCtx)
			m_pSIIVCtx->AddCB(this),
			m_pSIIVCtx->AddRef();
	}
	// 
	virtual void OnSIIVCtxUpdate(UINT nHint, void *pHint);
	// 
	void RefreshScene(bool fReinit = true);
	// 
	void RefreshSized();

	// 
	DECLARE_DYNCREATE(CDM2VwSceneVw)

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDM2VwSceneVw)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h���܂����B
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	virtual ~CDM2VwSceneVw();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CDM2VwSceneVw)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnScene1(UINT nID);
	afx_msg void OnUpdateScene1(CCmdUI* pCmdUI);
	afx_msg void OnDrivingPcat(UINT nID);
	afx_msg void OnUpdateDrivingPcat(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste4bpp(UINT nID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DM2VWSCENEVW_H__1EC36C15_95EE_44C6_92C0_62E8D5F69B77__INCLUDED_)
