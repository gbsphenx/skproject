// EditImageFragsDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DM2GDED.h"
#include "EditImageFragsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditImageFragsDlg ダイアログ


CEditImageFragsDlg::CEditImageFragsDlg(UINT iW, UINT iH, CWnd* pParent /*=NULL*/)
	: CDialog(CEditImageFragsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditImageFragsDlg)
	m_fH02 = FALSE;
	m_fH04 = FALSE;
	m_fH08 = FALSE;
	m_fH10 = FALSE;
	m_fH20 = FALSE;
	m_fH40 = FALSE;
	m_fH80 = FALSE;
	m_fW02 = FALSE;
	m_fW04 = FALSE;
	m_fW08 = FALSE;
	m_fW10 = FALSE;
	m_fW20 = FALSE;
	m_fW40 = FALSE;
	m_fW80 = FALSE;
	//}}AFX_DATA_INIT

	m_fH02 = (iH & 0x0200) ? 1 : 0;
	m_fH04 = (iH & 0x0400) ? 1 : 0;
	m_fH08 = (iH & 0x0800) ? 1 : 0;
	m_fH10 = (iH & 0x1000) ? 1 : 0;
	m_fH20 = (iH & 0x2000) ? 1 : 0;
	m_fH40 = (iH & 0x4000) ? 1 : 0;
	m_fH80 = (iH & 0x8000) ? 1 : 0;

	m_fW02 = (iW & 0x0200) ? 1 : 0;
	m_fW04 = (iW & 0x0400) ? 1 : 0;
	m_fW08 = (iW & 0x0800) ? 1 : 0;
	m_fW10 = (iW & 0x1000) ? 1 : 0;
	m_fW20 = (iW & 0x2000) ? 1 : 0;
	m_fW40 = (iW & 0x4000) ? 1 : 0;
	m_fW80 = (iW & 0x8000) ? 1 : 0;
}


void CEditImageFragsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditImageFragsDlg)
	DDX_Check(pDX, IDC_CHECK_H0200, m_fH02);
	DDX_Check(pDX, IDC_CHECK_H0400, m_fH04);
	DDX_Check(pDX, IDC_CHECK_H0800, m_fH08);
	DDX_Check(pDX, IDC_CHECK_H1000, m_fH10);
	DDX_Check(pDX, IDC_CHECK_H2000, m_fH20);
	DDX_Check(pDX, IDC_CHECK_H4000, m_fH40);
	DDX_Check(pDX, IDC_CHECK_H8000, m_fH80);
	DDX_Check(pDX, IDC_CHECK_W0200, m_fW02);
	DDX_Check(pDX, IDC_CHECK_W0400, m_fW04);
	DDX_Check(pDX, IDC_CHECK_W0800, m_fW08);
	DDX_Check(pDX, IDC_CHECK_W1000, m_fW10);
	DDX_Check(pDX, IDC_CHECK_W2000, m_fW20);
	DDX_Check(pDX, IDC_CHECK_W4000, m_fW40);
	DDX_Check(pDX, IDC_CHECK_W8000, m_fW80);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditImageFragsDlg, CDialog)
	//{{AFX_MSG_MAP(CEditImageFragsDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditImageFragsDlg メッセージ ハンドラ
