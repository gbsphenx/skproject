#if !defined(AFX_ADDIMAGEDLG_H__9AA27745_8E3B_4377_8455_597FFD83354C__INCLUDED_)
#define AFX_ADDIMAGEDLG_H__9AA27745_8E3B_4377_8455_597FFD83354C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddImageDlg.h : �w�b�_�[ �t�@�C��
//

#include "GDAT.h"
#include "EditPCMCB.h"
#include "WVCT.h"
#include "EditVxxx.h"

/////////////////////////////////////////////////////////////////////////////
// CAddImageDlg �_�C�A���O

class CAddImageDlg : public CDialog
{
	// 
	CGDEntry m_pos;
	// 
	int m_nEntryMask;
	// 
	CEditPCMCB *m_pCB;
	// 
	BOOL m_nEntryCtxFlag;
	// 
	CString m_strNewCaption;
	// 
	CEditVxxx &m_vxxx;
	// 
	bool m_fEditFlags;
	// 
	bool m_fFlags[16];

	// 
	void Construction();
	// 
	CString FormatFlagText(UINT i, LPCTSTR psz);

public:
	// 
	CAddImageDlg(CWnd* pParent = NULL);
	// 
	CAddImageDlg(CEditVxxx &vxxx, CWnd* pParent = NULL);
	// 
	void InitAddImage(CGDEntry pos);
	// 
	void InitAddText(CGDEntry pos);
	// 
	void InitAdd0B(CGDEntry pos);
	// 
	void InitAdd0C(CGDEntry pos);
	// 
	const CGDEntry &GetEntryPos() { return m_pos; }
	// 
	void InitEditText(CGDEntry pos, LPCSTR pszText);
	// 
	void InitEdit0B(CGDEntry pos, UINT nVal);
	// 
	void InitEdit0C(CGDEntry pos, UINT nVal);
	// 
	UINT GetVal();
	// 
	void InitEditPCM(CGDEntry pos, CEditPCMCB *pCB);
	// 
	void InitAddPCM(CGDEntry pos, CEditPCMCB *pCB);
	// 
	void InitAddClass2(CGDEntry pos);
	// 
	void InitAdd04(CGDEntry pos);
	// 
	void InitAdd07(CGDEntry pos);
	// 
	void InitAdd08(CGDEntry pos);
	// 
	void InitAdd09(CGDEntry pos);
	// 
	void InitAdd0E(CGDEntry pos);
	// 
	void InitRename(CGDEntry pos);

	// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAddImageDlg)
	enum { IDD = IDD_ADD_IMAGE };
	CListBox	m_wndFlags;
	CButton	m_wndEditPCM;
	CButton	m_wndOk;
	CEdit	m_wndVal;
	CEdit	m_wndText;
	CEdit	m_wndC6;
	CEdit	m_wndC5;
	CEdit	m_wndC4;
	CEdit	m_wndC3;
	CEdit	m_wndC2;
	CEdit	m_wndC1;
	CString	m_strVal;
	CString	m_strText;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CAddImageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CAddImageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEdit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_ADDIMAGEDLG_H__9AA27745_8E3B_4377_8455_597FFD83354C__INCLUDED_)
