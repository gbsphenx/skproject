// DM2WorldShiftDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "DM2WorldShiftDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2WorldShiftDlg ダイアログ


CDM2WorldShiftDlg::CDM2WorldShiftDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDM2WorldShiftDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDM2WorldShiftDlg)
	m_nX = 0;
	m_nY = 0;
	//}}AFX_DATA_INIT
}


void CDM2WorldShiftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDM2WorldShiftDlg)
	DDX_Control(pDX, IDC_SPIN_Y, m_wndY);
	DDX_Control(pDX, IDC_SPIN_X, m_wndX);
	DDX_Text(pDX, IDC_EDIT_X, m_nX);
	DDV_MinMaxInt(pDX, m_nX, -255, 255);
	DDX_Text(pDX, IDC_EDIT_Y, m_nY);
	DDV_MinMaxInt(pDX, m_nY, -255, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDM2WorldShiftDlg, CDialog)
	//{{AFX_MSG_MAP(CDM2WorldShiftDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2WorldShiftDlg メッセージ ハンドラ

BOOL CDM2WorldShiftDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_wndX.SetRange(-255, 255);
	m_wndY.SetRange(-255, 255);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
