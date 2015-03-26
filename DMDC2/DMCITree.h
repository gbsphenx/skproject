#if !defined(AFX_DMCITREE_H__80B6D104_772D_45D4_B70D_6256EA4311A1__INCLUDED_)
#define AFX_DMCITREE_H__80B6D104_772D_45D4_B70D_6256EA4311A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMCITree.h : �w�b�_�[ �t�@�C��
//

#include "DMEdCtx.h"

/////////////////////////////////////////////////////////////////////////////
// CDMCITreeView �E�B���h�E

class CDMCIIf
{
public:
	// 
	struct Any;
	// 
	struct Root;
	// 
	struct RootChkInt;
	// 
	struct Site;
	// 
	struct SiteAtlasPos;

	// 
	virtual HTREEITEM AddRoot_CheckIntegrity() = 0;
	// 
	virtual HTREEITEM AddSite_CheckInt(HTREEITEM hParent, AtlasPos_t pos, LPCTSTR pszMessage) = 0;
	// 
	virtual HTREEITEM AddSite_Memo(HTREEITEM hParent, LPCTSTR pszMessage) = 0;
	// 
	virtual BOOL PickItUp(HTREEITEM hItem) = 0;
};

struct CDMCIIf::Any {
	// 
	virtual ~Any() { }
};

struct CDMCIIf::Root : CDMCIIf::Any {

};

struct CDMCIIf::RootChkInt : CDMCIIf::Root {

};

struct CDMCIIf::Site : CDMCIIf::Any {

};

struct CDMCIIf::SiteAtlasPos : CDMCIIf::Site {
	// 
	AtlasPos_t pos;
};

class CDMCITreeView : public CTreeCtrl, public CDMCIIf, CDMEdCtxCB
{
	// 
	CDMEdCtx *m_pEdCtx;
	// 
	CFont m_fontNice;
	// 
	HTREEITEM m_hItemChkInt;

	// 
	BOOL SetItemData(HTREEITEM hItem, Any *pItem);

	// 
	virtual void OnDMUpdate(UINT nHint, void *pHint);

public:
	// 
	CDMCITreeView();
	// 
	BOOL Create(CDMEdCtx *pEdCtx, DWORD style, CWnd *pParentWnd, UINT nID)
	{
		if (CTreeCtrl::Create(style, CRect(0, 0, 0, 0), pParentWnd, nID)) {
			Init(pEdCtx);
			return TRUE;
		}
		return FALSE;
	}
	// 
	void Init(CDMEdCtx *pEdCtx)
	{
		if (m_pEdCtx)
			m_pEdCtx->RemoveCB(this),
			m_pEdCtx->Release();
		if (m_pEdCtx = pEdCtx)
			m_pEdCtx->AddRef(),
			m_pEdCtx->AddCB(this);
	}

	// 
	virtual HTREEITEM AddRoot_CheckIntegrity();
	// 
	virtual HTREEITEM AddSite_CheckInt(HTREEITEM hParent, AtlasPos_t pos, LPCTSTR pszMessage);
	// 
	virtual HTREEITEM AddSite_Memo(HTREEITEM hParent, LPCTSTR pszMessage);
	// 
	virtual BOOL PickItUp(HTREEITEM hItem);

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDMCITreeView)
	protected:
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CDMCITreeView();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CDMCITreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DMCITREE_H__80B6D104_772D_45D4_B70D_6256EA4311A1__INCLUDED_)
