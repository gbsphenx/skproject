#if !defined(AFX_EditVSSItemPg_H__E5BECE4B_E5F6_4EB7_94F5_EFCA389F74FE__INCLUDED_)
#define AFX_EditVSSItemPg_H__E5BECE4B_E5F6_4EB7_94F5_EFCA389F74FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVSSItemPg.h : �w�b�_�[ �t�@�C��
//

#include "EditVPg.h"
#include "EditVxxx.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVSSItemPg �_�C�A���O

class CEditVSSItemPg : public CEditVPg
{
	// 
	int m_iInitItem;
	// 0=Key-hole item, 1=Prize item
	bool m_fTypePrize;

	// 
	void OnSel();
	// 
	void EnableItem(UINT nID, BOOL f)
	{
		CWnd *pWnd;
		if (pWnd = GetDlgItem(nID)) {
			pWnd->ShowWindow(f ? SW_SHOW : SW_HIDE);
			pWnd->EnableWindow(f);
		}
	}

public:
	// 
	CEditVSSItemPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVPg(vctx, pParent) { }
	// 
	virtual void Construction();
	// 
	virtual UINT GetVal()
	{
		if (m_fTypePrize) {
			UINT r = 0
				|(((m_iCat) & 3) << 14)
				|(((m_iItem) & 127) << 7)
				|(((m_iCnt -1) & 15) << 0)
				|(((m_nRand) & 7) << 4)
				;
			return r;
		}
		UINT r = 0
			|(((m_iCat) & 3) << 7)
			|(((m_iItem) & 127) << 0)
			;
		return r;
	}
	// 
	virtual LPCTSTR GetTemplateName() { return MAKEINTRESOURCE(IDD); }
	// 
	void SelectTypePrize(bool fUse)
	{
		ASSERT(!IsWindow(m_hWnd));

		m_fTypePrize = fUse;
	}

protected:
	// 
	static int tbl4[];

public:

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEditVSSItemPg)
	enum { IDD = IDD_EDITV_OOFITEM };
	CListBox	m_wndList;
	int		m_iCat;
	int		m_iItem;
	UINT	m_iCnt;
	UINT	m_nRand;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditVSSItemPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEditVSSItemPg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboCat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CEditVSSKeyItemPg : public CEditVSSItemPg, public CWordValCmd_Unsigned, public CEditVxxx
{
public:
	// 
	DECL_WVCT_SPEC(wvctKeyItem)

	// 
	CEditVSSKeyItemPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVSSItemPg(vctx, pParent)
	{
		SelectTypePrize(false);
	}

	// 
	virtual CString FormatDisp(WORD x);

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }
};

class CEditVSSPrizeItemPg : public CEditVSSItemPg, public CWordValCmd_Unsigned, public CEditVxxx
{
public:
	// 
	DECL_WVCT_SPEC(wvctPrizeItem)

	// 
	CEditVSSPrizeItemPg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVSSItemPg(vctx, pParent)
	{
		SelectTypePrize(true);
	}

	// 
	virtual CString FormatDisp(WORD x);

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EditVSSItemPg_H__E5BECE4B_E5F6_4EB7_94F5_EFCA389F74FE__INCLUDED_)
