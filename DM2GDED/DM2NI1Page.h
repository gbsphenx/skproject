#if !defined(AFX_DM2NI1PAGE_H__7D28D688_7188_4F33_A101_9C5A53812D8E__INCLUDED_)
#define AFX_DM2NI1PAGE_H__7D28D688_7188_4F33_A101_9C5A53812D8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2NI1Page.h : �w�b�_�[ �t�@�C��
//

#include "DM2NIBPage.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2NI1Page �_�C�A���O

class CDM2NI1Page : public CDM2NIBPage
{
	// 
	DECLARE_DYNCREATE(CDM2NI1Page)

public:
	CDM2NI1Page();
	~CDM2NI1Page();

protected:
	// 
	virtual int GetStage() { return 1; }

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDM2NI1Page)
	enum { IDD = IDD_PP_NI1 };
	int		m_i4bpp;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDM2NI1Page)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDM2NI1Page)
	//}}AFX_MSG
	afx_msg void OnRadio4bpp(UINT nID);
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DM2NI1PAGE_H__7D28D688_7188_4F33_A101_9C5A53812D8E__INCLUDED_)
