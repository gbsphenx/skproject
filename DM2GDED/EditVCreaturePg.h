#if !defined(AFX_EditVCreaturePg_H__49e31057_4f4a_4751_8faa_cf57b453c3ee__INCLUDED_)
#define AFX_EditVCreaturePg_H__49e31057_4f4a_4751_8faa_cf57b453c3ee__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVCreaturePg.h : �w�b�_�[ �t�@�C��
//

#include "EditVPg.h"
#include "EditVxxx.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CEditVCreaturePg �_�C�A���O

class CEditVCreaturePg : public CEditVPg, public CWordValCmd_Unsigned, public CEditVxxx
{
	// 
	UINT m_nFlags;
	// 
	CBitmap m_bmlast;
	// 
	UINT m_tick;

	// 
	virtual UINT GetVal()
	{
		return m_nFlags;
	}
	// 
	virtual LPCTSTR GetTemplateName() { return MAKEINTRESOURCE(IDD); }

	// 
	void Tick();

public:
	// 
	DECL_WVCT_SPEC(wvctCreature)

	// 
	CEditVCreaturePg(CEditVLower &vctx, CWnd* pParent = NULL): CEditVPg(vctx, pParent) { }
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
	//{{AFX_DATA(CEditVCreaturePg)
	enum { IDD = IDD_EDITV_CREATURE };
	CComboBox m_wndCreatureSel;
	CStatic m_wndPortrait;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditVCreaturePg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEditVCreaturePg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCbnSelchangeComboCreature();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EditVCreaturePg_H__49e31057_4f4a_4751_8faa_cf57b453c3ee__INCLUDED_)
