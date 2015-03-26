#if !defined(AFX_EDITDBITEMDLG_H__56D31052_EE37_4BA7_BEE4_928CFDFB4910__INCLUDED_)
#define AFX_EDITDBITEMDLG_H__56D31052_EE37_4BA7_BEE4_928CFDFB4910__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditDBItemDlg.h : �w�b�_�[ �t�@�C��
//

#include "EditDMItemSubDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CEditDBItemDlg �_�C�A���O

class CEditDBItemDlg : public CDialog
{
	// 
	BOOL m_bAdd, m_bAddLimited;
	// 
	UINT m_iTab;
	// 
	CEditDMItemSubDlg m_wndSub[16];
	// 
	DMDBU m_rec;
	// 
	UINT m_iEditTab;
	// 
	static BOOL m_bRememberedUseHex;
	// 
	BOOL m_bUseHex;
	// 
	BOOL m_bRememberUseHex;
	// 
	BOOL m_bLimitless;

	// 
	CEditDMItemSubDlg &GetSub(UINT i)
	{
		ASSERT(i <= 15);
		i = __min(i, 15);
		return m_wndSub[i];
	}
	// 
	void ProcessTabChange();
	// 
	void ImportFrom(UINT iTab, UINT iDB, const DMDBU &rec);
	// 
	bool saveAsRec(DMDBU &rec, bool fNew)
	{
		if (m_bAdd && !m_bAddLimited)
			return GetSub(m_iTab).m_ret.SaveAs(m_iTab, rec, fNew);
		return GetSub(0).m_ret.SaveAs(m_iEditTab, rec, fNew);
	}

public:
	// 
	UINT GetLastTabSel()
	{
		return m_iTab;
	}
	// 
	CEditDBItemExchanger &GetSubParam()
	{
		return GetSub(m_iTab).m_ret;
	}
	// 
	bool SaveAsRec(DMDBU &rec, bool fNew)
	{
		if (saveAsRec(rec, fNew)) {
			if (m_bLimitless)
				rec.r0.next.r.w = m_iNextItem;
			return true;
		}
		return false;
	}
	// 
	CEditDBItemDlg(int iAlign, UINT iDB, const DMDBU &rec, BOOL bLimitless, CWnd* pParent = NULL);
	// 
	CEditDBItemDlg(int iAlign, UINT iDB, BOOL bLimitless, CWnd* pParent = NULL);

// �R���X�g���N�V����
public:
	CEditDBItemDlg(BOOL bLimitless, CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEditDBItemDlg)
	enum { IDD = IDD_EDITDBITEM };
	CComboBox	m_wndNextItem;
	CStatic	m_wndAlignLabel;
	CComboBox	m_wndAlign;
	CTabCtrl	m_wndTab;
	int		m_iAlign;
	//}}AFX_DATA
	UINT m_iNextItem;

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditDBItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEditDBItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnUpdateOK(CCmdUI* pCmdUI);
	afx_msg void OnSelChangeTabDbsel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckHex();
	afx_msg void OnCheckRememberHex();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDITDBITEMDLG_H__56D31052_EE37_4BA7_BEE4_928CFDFB4910__INCLUDED_)
