// DM2GDEDDoc.h : CDM2GDEDDoc �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2GDEDDOC_H__FF9AAACF_C3D2_413A_827F_9DE8A1E723F7__INCLUDED_)
#define AFX_DM2GDEDDOC_H__FF9AAACF_C3D2_413A_827F_9DE8A1E723F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GDEdCtx.h"
#include "PM.h"

struct TSelTreeItem {
	// 
	CGDEntry lower, upper;
};

class CDM2GDEDDoc : public CDocument, protected COnIdle
{
	// 
	CGDEdCtx *m_pEdCtx;
	// 
	CGDEdOpt m_aEdOpt;
	// 
	UINT m_nGDRSLT;

public:
	// 
	CGDEdCtx *GetEdCtx(BOOL fAddRef)
	{
		if (m_pEdCtx && fAddRef)
			m_pEdCtx->AddRef();

		return m_pEdCtx;
	}
	// 
	virtual BOOL IsModified();
	// 
	virtual void SetModifiedFlag(BOOL bModified = TRUE);

protected:
	// 
	CDM2GDEDDoc();
	// 
	virtual BOOL OnIdleNotify();

	// 
	DECLARE_DYNCREATE(CDM2GDEDDoc)

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

//�I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDM2GDEDDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CDM2GDEDDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CDM2GDEDDoc)
	afx_msg void OnFileExport();
	afx_msg void OnFileImport();
	afx_msg void OnOptReloadRules();
	afx_msg void OnFileExportDiff();
	afx_msg void OnFileImportDiff();
	afx_msg void OnFileDisplayOPI();
	afx_msg void OnTest();
	//}}AFX_MSG
	afx_msg void OnOptMisc(UINT nID);
	afx_msg void OnUpdateOptMisc(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
public:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DM2GDEDDOC_H__FF9AAACF_C3D2_413A_827F_9DE8A1E723F7__INCLUDED_)
