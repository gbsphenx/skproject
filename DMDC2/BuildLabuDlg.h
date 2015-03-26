
#pragma once

// CBuildLabuDlg ダイアログ

class CBuildLabuDlg : public CDialog
{
	DECLARE_DYNAMIC(CBuildLabuDlg)

public:
	CBuildLabuDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CBuildLabuDlg();

// ダイアログ データ
	enum { IDD = IDD_BUILDLABU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL m_iMethod;
	afx_msg void OnDestroy();
	CRect m_rcWin;
	int m_iRetire;
	BOOL m_fTrap;
};
