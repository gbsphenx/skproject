#if !defined(AFX_EDITVSKTEXT_H__4572A445_3F9E_47D7_8882_A9B05E78D1E8__INCLUDED_)
#define AFX_EDITVSKTEXT_H__4572A445_3F9E_47D7_8882_A9B05E78D1E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVSKText.h : �w�b�_�[ �t�@�C��
//

#include "SKTextGridCtrl.h"
#include "EditVPg.h"
#include "EditVxxx.h"
#include "AnchorDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVSKText �_�C�A���O

class CEditVSKText : public CEditVPg, public CWordValCmd_Unsigned, public CEditVxxx, public CAnchorSupport
{
	// 
	CString m_strSKTextRet;
	// 
	CSKTextGridCtrl m_wndGrid;
	// 
	CDM2SkCmdList m_listCmd;

public:
	// 
	DECL_WVCT_SPEC(wvctCommand)

	// 
	CEditVSKText(CEditVLower &vctx, CWnd* pParent = NULL): CEditVPg(vctx, pParent), CAnchorSupport(this) { }
	// 
	virtual void Construction();
	// 
	virtual UINT GetVal()
	{
		return 0;
	}
	// 
	virtual LPCTSTR GetText()
	{
		m_listCmd.strName = m_strName;

		return (m_strSKTextRet = m_listCmd.Mk() + "<00>");
	}
	// 
	virtual LPCTSTR GetTemplateName() { return MAKEINTRESOURCE(IDD); }

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEditVSKText)
	enum { IDD = IDD_EDITV_SKTEXT };
	CHyperLink	m_wndLink;
	CStatic	m_wndPlaceHolder;
	CString	m_strName;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditVSKText)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEditVSKText)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDITVSKTEXT_H__4572A445_3F9E_47D7_8882_A9B05E78D1E8__INCLUDED_)
