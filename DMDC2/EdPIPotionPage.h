#if !defined(AFX_EDPIPOTIONPAGE_H__4DB4A62E_2318_4920_8162_69C8060E4DA8__INCLUDED_)
#define AFX_EDPIPOTIONPAGE_H__4DB4A62E_2318_4920_8162_69C8060E4DA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIPotionPage.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CEdPIPotionPage �_�C�A���O

#include "EdPIDlg.h"

class CEdPIPotionPage : public CEdPIDlg
{
	// 
	DECLARE_DYNCREATE(CEdPIPotionPage)

public:
	// 
	CEdPIPotionPage();

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEdPIPotionPage)
	enum { IDD = IDD_PI_POTION };
	CComboBox	m_wndSel;
	CSliderCtrl	m_wndStr;
	int		m_iSel;
	int		m_iStr;
	BOOL	m_fF;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CEdPIPotionPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEdPIPotionPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDPIPOTIONPAGE_H__4DB4A62E_2318_4920_8162_69C8060E4DA8__INCLUDED_)
