#if !defined(AFX_EDITVFLAGSPG_H__41491803_EE05_453B_8103_CBCF03F7A23E__INCLUDED_)
#define AFX_EDITVFLAGSPG_H__41491803_EE05_453B_8103_CBCF03F7A23E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVFlagsPg.h : �w�b�_�[ �t�@�C��
//

#include "EditVPg.h"
#include "EditVxxx.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVFlagsPg �_�C�A���O

class CEditVFlagsPg : public CEditVPg
{
	// 
	UINT m_nFlags;

	// 
	virtual UINT GetVal()
	{
		return m_nFlags;
	}
	// 
	virtual LPCTSTR GetTemplateName() { return MAKEINTRESOURCE(IDD); }

public:
	// 
	CEditVFlagsPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVPg(vctx, pParent) { }
	// 
	void Construction();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEditVFlagsPg)
	enum { IDD = IDD_EDITV_FLAGS };
	CListBox	m_wndList;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditVFlagsPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// 
	CString m_strHintText[16];

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEditVFlagsPg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CEditVBinaryFlagsPg : public CEditVFlagsPg, public CWordValCmd_Flags, public CEditVxxx
{
public:
	// 
	DECL_WVCT_SPEC(wvctFlags)

	// 
	CEditVBinaryFlagsPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVFlagsPg(vctx, pParent) { }

	// 
	virtual CString Format(WORD x);
	// 
	virtual CString FormatDisp(WORD x);
	// 
	virtual bool Parse(LPCSTR pszText, WORD &x);
	// 
	virtual LPCTSTR GetHintText(int i) { return m_strHintText[i & 15]; }

	// 
	void SetParm1(LPCTSTR psz);

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDITVFLAGSPG_H__41491803_EE05_453B_8103_CBCF03F7A23E__INCLUDED_)
