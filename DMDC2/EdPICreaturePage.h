#if !defined(AFX_EDPICREATUREPAGE_H__FD25C40D_AC7B_4AF1_9A45_B3C5BD3DC8A3__INCLUDED_)
#define AFX_EDPICREATUREPAGE_H__FD25C40D_AC7B_4AF1_9A45_B3C5BD3DC8A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPICreaturePage.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CEdPICreaturePage �_�C�A���O

#include "EdPIDlg.h"

class CEdPICreaturePage : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPICreaturePage)

public:
	// 
	CEdPICreaturePage();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdPICreaturePage)
	enum { IDD = IDD_PI_CREATURE };
	CComboBox	m_wndSel;
	CComboBox	m_wndDir;
	int		m_iSel;
	int		m_iDir;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CEdPICreaturePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdPICreaturePage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDPICREATUREPAGE_H__FD25C40D_AC7B_4AF1_9A45_B3C5BD3DC8A3__INCLUDED_)
