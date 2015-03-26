#if !defined(AFX_DM2SELORNATEIMAGE_H__07F43807_FB82_4302_8384_E8D4E0FFF4B6__INCLUDED_)
#define AFX_DM2SELORNATEIMAGE_H__07F43807_FB82_4302_8384_E8D4E0FFF4B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2SelOrnateImage.h : �w�b�_�[ �t�@�C��
//

#include "DM2Ornate1ComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2SelOrnateImageDlg �_�C�A���O

class CDM2SelOrnateImageDlg : public CDialog
{
	// 
	int m_t;

	// 
	void MkTree();

public:
	// 
	enum {
		tWallOrnates = 0x09,
		tFloorOrnates = 0x0A,
		tDoorOrnates = 0x0B,
		tDoors = 0x0E,
		tCreatures = 0x0F,
	};

	// 
	CDM2SelOrnateImageDlg(int t, CWnd* pParent = NULL);
	// 
	CDM2SelOrnateImageDlg(bool fDoor, bool fWall, CWnd* pParent = NULL);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDM2SelOrnateImageDlg)
	enum { IDD = IDD_SEL_ORNATE };
	CTreeCtrl	m_wndTree;
	CDM2Ornate1ComboBox	m_wndOrnate;
	int		m_iOrnate;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDM2SelOrnateImageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDM2SelOrnateImageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTreeClasses(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DM2SELORNATEIMAGE_H__07F43807_FB82_4302_8384_E8D4E0FFF4B6__INCLUDED_)
