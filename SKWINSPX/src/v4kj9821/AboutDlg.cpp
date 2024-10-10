// AboutDlg.cpp : 実装ファイル
//

#include <stdafx.h>
#include <resource.h>
#include <AboutDlg.h>

// CAboutDlg ダイアログ

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{

}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAboutDlg メッセージ ハンドラ
