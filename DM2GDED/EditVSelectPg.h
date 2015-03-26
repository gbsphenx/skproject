#if !defined(AFX_EditVSelectPg_H__92e0d64f_0131_41ab_b50d_4d6a4f1236b5__INCLUDED_)
#define AFX_EditVSelectPg_H__92e0d64f_0131_41ab_b50d_4d6a4f1236b5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVSelectPg.h : �w�b�_�[ �t�@�C��
//

#include "EditVPg.h"
#include "EditVxxx.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVSelectPg �_�C�A���O

class CEditVSelectPg : public CEditVPg, public CWordValCmd_Unsigned, public CEditVxxx
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
	DECL_WVCT_SPEC(wvctSelect)

	// 
	CEditVSelectPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVPg(vctx, pParent) { }
	// 
	void Construction();

	// 
	virtual CString FormatDisp(WORD x);
	// 
	virtual bool Parse(LPCSTR pszText, WORD &x);

	// 
	void SetParm1(LPCTSTR psz);

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEditVSelectPg)
	enum { IDD = IDD_EDITV_SELECT };
	CListCtrl m_wndList;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditVSelectPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// 
	class Sel {
	public:
		// 
		int iVal;
		// 
		CString strText, strImage;
	};
	// 
	CArray<Sel, Sel &> m_arrSels;
	// 
	CImageList m_il;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEditVSelectPg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnListSelectDblClk(NMHDR *pNotifyStruct, LRESULT *result);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EditVSelectPg_H__92e0d64f_0131_41ab_b50d_4d6a4f1236b5__INCLUDED_)
