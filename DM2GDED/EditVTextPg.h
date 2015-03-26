#if !defined(AFX_EDITVTEXTPG_H__5A0C4406_FD8C_48D1_B2F0_7CF108F1E16F__INCLUDED_)
#define AFX_EDITVTEXTPG_H__5A0C4406_FD8C_48D1_B2F0_7CF108F1E16F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVTextPg.h : �w�b�_�[ �t�@�C��
//

#include "EditVPg.h"
#include "EditVxxx.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVTextPg �_�C�A���O

class CEditVTextPg : public CEditVPg, public CWordValCmd_Unsigned, public CEditVxxx
{
public:
	// 
	DECL_WVCT_SPEC(wvctText)

	// 
	CEditVTextPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVPg(vctx, pParent) { }
	// 
	virtual void Construction();
	// 
	virtual UINT GetVal()
	{
		return 0;
	}
	// 
	virtual LPCTSTR GetText() { return m_strText; }
	// 
	virtual LPCTSTR GetTemplateName() { return MAKEINTRESOURCE(IDD); }

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }

	// 
	virtual void OnCancel() {
		if (GetParent() != NULL)
			GetParent()->SendMessage(WM_COMMAND, IDCANCEL);
	}

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEditVTextPg)
	enum { IDD = IDD_EDITV_TEXT };
	CString	m_strText;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditVTextPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEditVTextPg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDITVTEXTPG_H__5A0C4406_FD8C_48D1_B2F0_7CF108F1E16F__INCLUDED_)
