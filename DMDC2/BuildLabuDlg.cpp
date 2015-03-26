// BuildLabu.cpp : 実装ファイル
//

#include "StdAfx.h"
#include "Exp00069.h"
#include "BuildLabuDlg.h"


// CBuildLabuDlg ダイアログ

IMPLEMENT_DYNAMIC(CBuildLabuDlg, CDialog)

CBuildLabuDlg::CBuildLabuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBuildLabuDlg::IDD, pParent)
	, m_iMethod(0), m_rcWin(0, 0, 0, 0)
	, m_iRetire(0)
	, m_fTrap(FALSE)
{

}

CBuildLabuDlg::~CBuildLabuDlg()
{
}

void CBuildLabuDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Radio(pDX, IDC_RADIO_KABE_TAOSHI, m_iMethod);
	DDX_Text(pDX, IDC_EDIT_RETIRE, m_iRetire);
	DDV_MinMaxInt(pDX, m_iRetire, 0, 100);
	DDX_Check(pDX, IDC_CHECK_TRAP, m_fTrap);
}


BEGIN_MESSAGE_MAP(CBuildLabuDlg, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CBuildLabuDlg メッセージ ハンドラ

BOOL CBuildLabuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_rcWin.left != 0)
		SetWindowPos(NULL, m_rcWin.left, m_rcWin.top, 0, 0, SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER);

	{
		CSpinButtonCtrl *p = static_cast<CSpinButtonCtrl *>(GetDlgItem(IDC_SPIN_RETIRE));
		p->SetRange(0, 100);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CBuildLabuDlg::OnDestroy()
{
	GetWindowRect(m_rcWin);
	CDialog::OnDestroy();
}
