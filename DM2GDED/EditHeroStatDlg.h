#if !defined(AFX_EDITHEROSTATDLG_H__FD524B30_ECB5_4428_973F_4B2B902FFB14__INCLUDED_)
#define AFX_EDITHEROSTATDLG_H__FD524B30_ECB5_4428_973F_4B2B902FFB14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditHeroStatDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CEditHeroStatDlg �_�C�A���O

class CEditHeroStatDlg : public CDialog
{
	// 
	void revert();
	void commit();

	void setVal(UINT nID, UINT val, UINT minVal, UINT maxVal, UINT step);
	void getValW(UINT nID, void *pw, UINT , UINT , UINT );

public:
	// 
	BYTE m_bData[52];
	bool m_fLE;

public:
	CEditHeroStatDlg(bool fLE, CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEditHeroStatDlg)
	enum { IDD = IDD_EDIT_HEROSTAT };
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditHeroStatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEditHeroStatDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonYukman();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDITHEROSTATDLG_H__FD524B30_ECB5_4428_973F_4B2B902FFB14__INCLUDED_)
