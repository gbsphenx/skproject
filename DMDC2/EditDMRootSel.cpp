// EditDMTileDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EditDMRootSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditDMRootSel ダイアログ


CEditDMRootSel::CEditDMRootSel(RN rnNext, CWnd* pParent /*=NULL*/)
	: CDialog(CEditDMRootSel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditDMRootSel)
	//}}AFX_DATA_INIT
	m_rnNext = rnNext;
}

void CEditDMRootSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditDMRootSel)
	DDX_Control(pDX, IDC_COMBO_NEXT_ITEM, m_wndNextItem);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEditDMRootSel, CDialog)
	//{{AFX_MSG_MAP(CEditDMRootSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditDMRootSel メッセージ ハンドラ

BOOL CEditDMRootSel::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString tstr;
	tstr.Format("%04X", (UINT)m_rnNext.r.w);
	m_wndNextItem.AddString(tstr);
	m_wndNextItem.SetCurSel(0);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CEditDMRootSel::OnOK()
{
	CString tstr;
	m_wndNextItem.GetWindowText(tstr);
	LPTSTR lpszAfter;
	m_rnNext = RNco((WORD)strtol(tstr, &lpszAfter, 16));

	CDialog::OnOK();
}
