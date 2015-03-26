#if !defined(AFX_DM2GDEDVIEW_H__4499420D_A107_4708_8718_BFAD15474D7B__INCLUDED_)
#define AFX_DM2GDEDVIEW_H__4499420D_A107_4708_8718_BFAD15474D7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2GDEDView.h : �w�b�_�[ �t�@�C��
//

#include "DGridCtrl.h"
#include "EditPCMCB.h"
#include "HexEditCB.h"
#include "DTreeDock.h"
#include "FlatSplitterWnd.h"
#include "DM2GDEDSplitterWnd.h"
#include "FocusReminder.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDView �r���[

class CDM2GDEDView : public CView, protected CGDEdCtxCB
{
	// 
	CGDEdCtx *m_pEdCtx;
	// 
	CDGridCtrl *m_pGrid;
	// 
	CDTreeDock m_wndTreeBar;
	// 
	CDM2GDEDSplitterWnd m_wndSplitter;
	// 
	CDTreeDock *m_pVw;
	// 
	CFocusReminder m_fr;

protected:
	// 
	CDM2GDEDView();
	// 
	virtual void OnGDUpdate(UINT nHint, void *pHint);
	// 
	void InitEdCtx(CGDEdCtx *pEdCtx);
	// 
	virtual void OnSelTreeItem(TSelTreeItem &rItem);

	// 
	CGDEdCtx *MkGDEdCtx() { return m_pEdCtx; }

	// 
	void ExportCsv(CString fpcsv, int cls1);

	// 
	DECLARE_DYNCREATE(CDM2GDEDView)

public:
	// 
	CDM2GDEDDoc* GetDocument();

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDM2GDEDView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h���܂����B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	virtual ~CDM2GDEDView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CDM2GDEDView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCtrlTreeview();
	afx_msg void OnCtrlGridview();
	//}}AFX_MSG
	afx_msg BOOL OnNextPane(UINT nID);
	afx_msg void OnUpdateNextPane(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePane();
	afx_msg void OnFileExportCsv();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DM2GDEDVIEW_H__4499420D_A107_4708_8718_BFAD15474D7B__INCLUDED_)
