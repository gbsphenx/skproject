
#pragma once

// CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
