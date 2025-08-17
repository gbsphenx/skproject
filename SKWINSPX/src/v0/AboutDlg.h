
#pragma once

// CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
