#if !defined(AFX_DM2WORLDSHIFTDLG_H__A772733A_C1A3_46D2_8F07_7EEC968F6906__INCLUDED_)
#define AFX_DM2WORLDSHIFTDLG_H__A772733A_C1A3_46D2_8F07_7EEC968F6906__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2WorldShiftDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDM2WorldShiftDlg �_�C�A���O

class CDM2WorldShiftDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CDM2WorldShiftDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDM2WorldShiftDlg)
	enum { IDD = IDD_WORLD_SHIFT };
	CSpinButtonCtrl	m_wndY;
	CSpinButtonCtrl	m_wndX;
	int		m_nX;
	int		m_nY;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDM2WorldShiftDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDM2WorldShiftDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DM2WORLDSHIFTDLG_H__A772733A_C1A3_46D2_8F07_7EEC968F6906__INCLUDED_)
