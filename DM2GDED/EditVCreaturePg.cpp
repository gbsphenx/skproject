// EditVCreaturePg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditVCreaturePg.h"
#include "DM2RTextToken.h"
#include "GDEdCtx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_CREATURES (100U)

/////////////////////////////////////////////////////////////////////////////
// CEditVCreaturePg ダイアログ


void CEditVCreaturePg::Construction()
{
	//{{AFX_DATA_INIT(CEditVCreaturePg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CEditVCreaturePg::DoDataExchange(CDataExchange* pDX)
{
	CEditVPg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditVCreaturePg)
	DDX_Control(pDX, IDC_COMBO_CREATURE, m_wndCreatureSel);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
		int i = m_wndCreatureSel.GetCurSel();
		if (i >= 0 && i < 256) {
			m_nFlags = i;
		}
	} else {
		if (m_nFlags < 256) {
			m_wndCreatureSel.SetCurSel(m_nFlags);
		}
	}
	DDX_Control(pDX, IDC_PORTRAIT, m_wndPortrait);
}


BEGIN_MESSAGE_MAP(CEditVCreaturePg, CEditVPg)
	//{{AFX_MSG_MAP(CEditVCreaturePg)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO_CREATURE, OnCbnSelchangeComboCreature)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditVCreaturePg メッセージ ハンドラ

int CEditVCreaturePg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditVPg::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

BOOL CEditVCreaturePg::OnInitDialog() 
{
	CEditVPg::OnInitDialog();
	
	for (int i = 0; i < MAX_CREATURES; i++) {
		CString str;
		if (!m_vctx.Get1CatItemName(rttCreatures, i, str))
			str.Format(" (%u)", i);

		m_wndCreatureSel.AddString(str);
	}

	m_nFlags = m_nInitVal;

	if (m_nInitVal < MAX_CREATURES)
		m_wndCreatureSel.SetCurSel(m_nInitVal);

	//SetTimer(0x0100, 333, NULL);

	m_tick = 0;

	Tick();

	UpdateData(FALSE);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
// CEditVCreaturePg メッセージ ハンドラ

CString CEditVCreaturePg::FormatDisp(WORD x)
{
	CString str;

	if (!m_vctx.Get1CatItemName(rttCreatures, x, str))
		str.Format(" (%u)", x);

	return str;
}

bool CEditVCreaturePg::Parse(LPCSTR pszText, WORD &x)
{
	LPSTR pszTerm;
	x = (WORD)strtol(pszText, &pszTerm, 10);
	return (*pszTerm == 0);
}

void CEditVCreaturePg::SetParm1(LPCTSTR psz)
{

}

void CEditVCreaturePg::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == 0x0100) {
		KillTimer(0x0100);
		Tick();
		SetTimer(0x0100, 333, NULL);
		return;
	}
	CEditVPg::OnTimer(nIDEvent);
}

void CEditVCreaturePg::OnCbnSelchangeComboCreature()
{
	m_tick = 0;

	Tick();
}

void CEditVCreaturePg::Tick()
{
	static const int tbl[] = {0x12, 0x2, 0x6, 0xA, 0xE};

	CGDEdCtx *pCtx = m_vctx.GetEdCtx();
	if (pCtx == NULL)
		return;

	CGDAT &core = pCtx->GetGDAT();
	int c2 = m_wndCreatureSel.GetCurSel();
	m_bmlast.DeleteObject();
	CGDEntry pos(0x0F, c2, 0x01, tbl[m_tick % 5], 0, 0);
	HBITMAP hbm = NULL;
	if (core.FindNearestEntry(pos))
		hbm = core.GetImgAt(pos);

	if (hbm != NULL) {
		m_bmlast.Attach(hbm);

		m_wndPortrait.SetBitmap(hbm);

		if (!m_wndPortrait.IsWindowVisible())
			m_wndPortrait.ShowWindow(SW_SHOW);
	}
	else if (m_tick == 0) {
		if (m_wndPortrait.IsWindowVisible())
			m_wndPortrait.ShowWindow(SW_HIDE);
	}

	m_tick++;
}
