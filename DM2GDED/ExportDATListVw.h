#if !defined(AFX_EXPORTDATLISTVW_H__82057182_8829_430A_910C_7F4806C022B0__INCLUDED_)
#define AFX_EXPORTDATLISTVW_H__82057182_8829_430A_910C_7F4806C022B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportDATListVw.h : �w�b�_�[ �t�@�C��
//

#include "DGridCtrl.h"

class CExportDATDoc;
class TSelExpTreeItem;

/////////////////////////////////////////////////////////////////////////////
// CExportDATListVw �r���[

class CExportDATListVw : public CView
{
protected:
	// 
	CDGridCtrl m_wndGrid;
	// 
	CGDEdCtx *m_pEdCtx;

	// 
	CExportDATListVw();
	// 
	void OnSelectItem(TSelExpTreeItem &rAny);
	// 
	void InitEdCtx(CGDEdCtx *pEdCtx);
	// 
	CGDAT &GetGDAT();

	// 
	DECLARE_DYNCREATE(CExportDATListVw)

public:
	// 
	CExportDATDoc *GetDocument();

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CExportDATListVw)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h���܂����B
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	virtual ~CExportDATListVw();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CExportDATListVw)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EXPORTDATLISTVW_H__82057182_8829_430A_910C_7F4806C022B0__INCLUDED_)
