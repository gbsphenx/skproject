#if !defined(AFX_EDITDMFILEDESCDLG_H__F4785292_4B61_4252_8687_45BE52A40CA9__INCLUDED_)
#define AFX_EDITDMFILEDESCDLG_H__F4785292_4B61_4252_8687_45BE52A40CA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditDMFileDescDlg.h : �w�b�_�[ �t�@�C��
//

#include "DDAT.h"
#include "HexEditCB.h"

/////////////////////////////////////////////////////////////////////////////
// CEditDMFileDescDlg �_�C�A���O

class CEditDMFileDescDlg : public CDialog, CHexEditCB
{
	// 
	void ShowHex(UINT nID, const CByteArray &arr);
	// 
	void TryHexEdit(CWnd *pTar, LPCTSTR lpszName, UINT nID, const CByteArray &arr);
	// 
	void OnApplyHexEditCB(DWORD nCookie);

public:
	// 
	void Revert(const AllMapDesc_t &d);
	// 
	void Commit(AllMapDesc_t &d);

// �R���X�g���N�V����
public:
	CEditDMFileDescDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEditDMFileDescDlg)
	enum { IDD = IDD_EDITDMFILEDESC };
	UINT	m_nRandomGraphicsSeed;
	UINT	m_nMinObjListSize;
	UINT	m_nSPPx;
	UINT	m_nSPPy;
	UINT	m_nMaps;
	int		m_nPlayerDir;
	//}}AFX_DATA
	CByteArray m_arrTextData;

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditDMFileDescDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEditDMFileDescDlg)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDITDMFILEDESCDLG_H__F4785292_4B61_4252_8687_45BE52A40CA9__INCLUDED_)
