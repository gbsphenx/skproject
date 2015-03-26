// EdTileDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdTileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdTileDlg

BEGIN_MESSAGE_MAP(CEdTileDlg, CDialog)
	//{{AFX_MSG_MAP(CEdTileDlg)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdTileDlg メッセージ ハンドラ

void CEdTileDlg::Cnt(
	int i,
	BOOL f1, BOOL f2, BOOL f3, BOOL f4, BOOL f5, BOOL f6, BOOL f7, BOOL f8
)
{
	ASSERT(!m_fSiteUpdating);

	m_fSiteUpdating = TRUE;

	CWnd *p;
	for (int x = 0; x < 8; x++) {
		if ((p = GetDlgItem(IDC_RADIO_OP1 +x)) != NULL && (i == x))
			p->SendMessage(BM_SETCHECK, (i == x) ? BST_CHECKED : BST_UNCHECKED);
	}
	BOOL fTbl[] = {
		f1, f2, f3, f4, f5, f6, f7, f8
	};
	for (int f = 0; f < 8; f++) {
		if (p = GetDlgItem(IDC_CHECK_SW1 +f))
			p->SendMessage(BM_SETCHECK, fTbl[f] ? BST_CHECKED : BST_UNCHECKED);
	}

	m_fSiteUpdating = FALSE;
}

void CEdTileDlg::OnOK()
{
	m_cTileNew = m_cTile;

	SendNotifyToParent(0);

	DestroyWindow();
}

void CEdTileDlg::OnCancel()
{
	DestroyWindow();
}

BOOL CEdTileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_fSiteLow = false;

	GetApp()->RegisterIdleCB(this);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

int CEdTileDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_fSiteLow = true;
	m_fSiteUpdating = false;
	m_fSiteHook = false;

	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CEdTileDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	GetApp()->RevokeIdleCB(this);

	CWnd *p = GetParent();
	if (p) p->SetFocus();
	
}

BOOL CEdTileDlg::OnIdleNotify()
{
	if (m_fSiteHook) {
		CWnd *p = GetFocus(), *q = p;
		while (q && q != this) q = q->GetParent();
		if (q == NULL && GetParent() != p) {
			DestroyWindow();
		}
	}

	return FALSE;
}
