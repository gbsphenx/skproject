#if !defined(AFX_EDITVIOFF_H__9BFDBB8E_4B1D_4B78_88D6_F35EEE9E52CC__INCLUDED_)
#define AFX_EDITVIOFF_H__9BFDBB8E_4B1D_4B78_88D6_F35EEE9E52CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVIOff.h : �w�b�_�[ �t�@�C��
//

#include "EditVPg.h"
#include "EditVxxx.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVIOff �_�C�A���O

class CEditVIOff : public CEditVPg, public CWordValCmd_Hex, public CEditVxxx
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
	DECL_WVCT_SPEC(wvctImageOffset)

	// 
	CEditVIOff(CEditVLower &vctx, CWnd* pParent = NULL): CEditVPg(vctx, pParent) { }
	// 
	void Construction();

	// 
	virtual CEditVPg &GetEditValPg() { return *this; }
	// 
	virtual CWordValCmd &GetOp() { return *this; }

	virtual ~CEditVIOff();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEditVIOff)
	enum { IDD = IDD_EDITV_IOFF };
	CEdit	m_wndEditY;
	CEdit	m_wndEditX;
	CSpinButtonCtrl	m_wndSpinY;
	CSpinButtonCtrl	m_wndSpinX;
	int		m_offx;
	int		m_offy;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CEditVIOff)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEditVIOff)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditPosX();
	afx_msg void OnChangeEditPosY();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDITVIOFF_H__9BFDBB8E_4B1D_4B78_88D6_F35EEE9E52CC__INCLUDED_)
