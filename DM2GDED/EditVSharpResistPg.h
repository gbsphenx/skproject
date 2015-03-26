#if !defined(AFX_EDITVB1313PG_H__E9B27B79_3E40_4C4A_ADFE_6D8494531BDB__INCLUDED_)
#define AFX_EDITVB1313PG_H__E9B27B79_3E40_4C4A_ADFE_6D8494531BDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVB1313Pg.h : �w�b�_�[ �t�@�C��
//

#include "EditVPg.h"
#include "EditVxxx.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVSharpResistPg �_�C�A���O

class CEditVSharpResistPg : public CEditVPg, public CWordValCmd, public CEditVxxx
{
	// 
	CString m_str2[2];

public:
	// 
	DECL_WVCT_SPEC(wvctSharpResist)

	// 
	CEditVSharpResistPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVPg(vctx, pParent) { }
	// 
	virtual void Construction();
	// 
	virtual UINT GetVal()
	{
		return 0
			|((m_iL3 & 7))
			|((m_fL3 ? 1 : 0) << 3)
			|((m_fL4 ? 1 : 0) << 4)
			|((m_fL5 ? 1 : 0) << 5)
			|((m_fL6 ? 1 : 0) << 6)
			|((m_fL7 ? 1 : 0) << 7)
			|((m_iH3 & 7) << 8)
			|((m_fH3 ? 1 : 0) << 11)
			|((m_fH4 ? 1 : 0) << 12)
			|((m_fH5 ? 1 : 0) << 13)
			|((m_fH6 ? 1 : 0) << 14)
			|((m_fH7 ? 1 : 0) << 15)
			;
	}
	// 
	virtual LPCTSTR GetTemplateName() { return MAKEINTRESOURCE(IDD); }

	// 
	virtual CString Format(WORD x);
	// 
	virtual CString FormatDisp(WORD x);
	// 
	virtual bool Parse(LPCSTR pszText, WORD &x);

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }

	// 
	virtual LPCTSTR GetHintText(int i)
	{
		if (i == 0) return m_str2[0];
		if (i == 1) return m_str2[1];
		return NULL;
	}
	// 
	virtual void SetParm1(LPCSTR psz);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEditVSharpResistPg)
	enum { IDD = IDD_EDITV_B1313 };
	CString	m_strHB;
	CString	m_strLB;
	BOOL	m_fH3;
	BOOL	m_fH4;
	BOOL	m_fH5;
	BOOL	m_fH6;
	BOOL	m_fH7;
	BOOL	m_fL3;
	BOOL	m_fL4;
	BOOL	m_fL5;
	BOOL	m_fL6;
	BOOL	m_fL7;
	int		m_iH3;
	int		m_iL3;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditVSharpResistPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// 
	CString FormatB3(int iTextIdx, LPCTSTR pszDefText, UINT x);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEditVSharpResistPg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDITVB1313PG_H__E9B27B79_3E40_4C4A_ADFE_6D8494531BDB__INCLUDED_)
