#if !defined(AFX_EDITDM2FILEDESCDLG_H__3A449B8D_459D_4AC8_B7AF_63C6653B8B37__INCLUDED_)
#define AFX_EDITDM2FILEDESCDLG_H__3A449B8D_459D_4AC8_B7AF_63C6653B8B37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditDM2FileDescDlg.h : �w�b�_�[ �t�@�C��
//

#include "DDAT.h"
#include "DMTextEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CEditDM2FileDescDlg �_�C�A���O

class CEditDM2FileDescDlg : public CDialog
{
	// 
	CDDAT *m_pDDIf;
	// 
	DMTextEdit_t m_texte;
	// 
	CFont m_fontNice;

public:
	// 
	CEditDM2FileDescDlg(CDDAT *pDDIf, CWnd* pParent = NULL);
	// 
	void Revert(AllMapDesc_t &a, bool fRevert)
	{
		if (fRevert) {
			m_iDir = a.nStartDir;
			m_iX = a.nStartX;
			m_iY = a.nStartY;
			m_iSeed = a.nRandomGraphicsSeed;

			m_texte.Load(a.fvTextData);
		} else {
			a.nStartDir = m_iDir;
			a.nStartX = m_iX;
			a.nStartY = m_iY;
			a.nRandomGraphicsSeed = m_iSeed;
		}
	}

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEditDM2FileDescDlg)
	enum { IDD = IDD_EDITDMFILEDESC2 };
	CListCtrl	m_wndList;
	CSpinButtonCtrl	m_wndSpin;
	int		m_iDir;
	UINT	m_iSeed;
	UINT	m_iX;
	UINT	m_iY;
	UINT	m_nCntMap;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditDM2FileDescDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEditDM2FileDescDlg)
	afx_msg void OnButtonRef();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDITDM2FILEDESCDLG_H__3A449B8D_459D_4AC8_B7AF_63C6653B8B37__INCLUDED_)
