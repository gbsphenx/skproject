#if !defined(AFX_EDITVPG_H__995AC981_EAC0_4EA7_AEC4_34267D9AD5E4__INCLUDED_)
#define AFX_EDITVPG_H__995AC981_EAC0_4EA7_AEC4_34267D9AD5E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVPg.h : �w�b�_�[ �t�@�C��
//

#include "EditVCtx.h"
#include "WVCT.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVPg �E�B���h�E

class CEditVPg : public CDialog
{
public:
	// 
	CEditVPg(CEditVLower &vctx, CWnd* pParent = NULL);
	// 
	BOOL Create(LPCSTR pszInitText, UINT nVal, CWnd *pParent)
	{
		m_nInitVal = nVal;
		m_strInitText = pszInitText;

		if (CDialog::Create(GetTemplateName(), pParent)) {
			return TRUE;
		}
		return FALSE;
	}
	// 
	void OnCancel() { }
	// 
	bool OnApply()
	{
		if (!UpdateData()) {
			return false;
		}
		return true;
	}
	// 
	void OnOK()
	{
		if (GetParent() != NULL)
			GetParent()->OnCmdMsg(IDOK, 0, NULL, NULL);
	}

	// 
	virtual void Construction() { }
	// 
	virtual UINT GetVal() = NULL;
	// 
	virtual LPCTSTR GetText() { return m_strInitText; }
	// 
	virtual LPCTSTR GetTemplateName() = NULL;

protected:
	// 
	CEditVLower &m_vctx;
	// 
	UINT m_nInitVal;
	// 
	CString m_strInitText;

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CEditVPg)
	//}}AFX_VIRTUAL

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CEditVPg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDITVPG_H__995AC981_EAC0_4EA7_AEC4_34267D9AD5E4__INCLUDED_)
