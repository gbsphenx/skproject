#if !defined(AFX_EDITIMGFLGSDLG_H__DAEC97EB_59D0_4718_BF19_D42E6DC46DF3__INCLUDED_)
#define AFX_EDITIMGFLGSDLG_H__DAEC97EB_59D0_4718_BF19_D42E6DC46DF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditImgFlgsDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CEditImgFlgsDlg �_�C�A���O

class CEditImgFlgsDlg : public CDialog
{
	// 
	int m_nMin, m_nMax;

public:
	// 
	bool m_f4bpp;

	// 
	CEditImgFlgsDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	// 
	int GetX()
	{
		return m_nX;
	}
	// 
	int GetY()
	{
		return m_nY;
	}

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEditImgFlgsDlg)
	enum { IDD = IDD_EDIT_IMG_FLG };
	CEdit	m_wndPosY;
	CEdit	m_wndPosX;
	CSpinButtonCtrl	m_wndSpinY;
	CSpinButtonCtrl	m_wndSpinX;
	int		m_nX;
	int		m_nY;
	BOOL	m_fUse0c;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditImgFlgsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEditImgFlgsDlg)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDITIMGFLGSDLG_H__DAEC97EB_59D0_4718_BF19_D42E6DC46DF3__INCLUDED_)
