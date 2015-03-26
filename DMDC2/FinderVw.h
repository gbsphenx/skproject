#if !defined(AFX_FINDERVW_H__4CB663DF_9FE3_492E_AF86_2E8EF589F2D5__INCLUDED_)
#define AFX_FINDERVW_H__4CB663DF_9FE3_492E_AF86_2E8EF589F2D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FinderVw.h : �w�b�_�[ �t�@�C��
//

#include "DMEdCtx.h"

/////////////////////////////////////////////////////////////////////////////
// CFinderVw �r���[

class CFinderVw : public CListView
{
public:
	// 
	CDMEdCtx *pEdCtx;
	CFont font;
	bool isDblClk;

	//
	CFinderVw();
	virtual ~CFinderVw();

	// 
	void SetDMEdCtx(CDMEdCtx *p)
	{
		if (pEdCtx != NULL)
			pEdCtx->Release();
		if (pEdCtx = p)
			pEdCtx->AddRef();
	}

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CFinderVw)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h���܂����B
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CFinderVw)
	afx_msg void OnFinderFind();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFinderOneclick();
	afx_msg void OnUpdateFinderOneclick(CCmdUI* pCmdUI);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_FINDERVW_H__4CB663DF_9FE3_492E_AF86_2E8EF589F2D5__INCLUDED_)
