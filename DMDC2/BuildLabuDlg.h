
#pragma once

// CBuildLabuDlg �_�C�A���O

class CBuildLabuDlg : public CDialog
{
	DECLARE_DYNAMIC(CBuildLabuDlg)

public:
	CBuildLabuDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CBuildLabuDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_BUILDLABU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL m_iMethod;
	afx_msg void OnDestroy();
	CRect m_rcWin;
	int m_iRetire;
	BOOL m_fTrap;
};
