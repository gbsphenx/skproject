// EdPIText2Page.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdPIText2Page.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdPIText2Page プロパティ ページ

IMPLEMENT_DYNCREATE(CEdPIText2Page, CEdPIDlg)

CEdPIText2Page::CEdPIText2Page() : CEdPIDlg(CEdPIText2Page::IDD)
{
	//{{AFX_DATA_INIT(CEdPIText2Page)
	m_iSP = 0;
	//}}AFX_DATA_INIT

	m_iUse = 0;
}

void CEdPIText2Page::DoDataExchange(CDataExchange* pDX)
{
	CEdPIDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdPIText2Page)
	DDX_Control(pDX, IDC_SPIN_SP, m_wndSpinSP);
	DDX_Control(pDX, IDC_EDIT_SP, m_wndEditSP);
	DDX_Control(pDX, IDC_COMBO_USE, m_wndUse);
	DDX_Text(pDX, IDC_EDIT_SP, m_iSP);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEdPIText2Page, CEdPIDlg)
	//{{AFX_MSG_MAP(CEdPIText2Page)
	ON_CBN_SELCHANGE(IDC_COMBO_USE, OnSelchangeComboUse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdPIText2Page メッセージ ハンドラ

bool CEdPIText2Page::RevertRecord(DMDBU &rec, bool fRevert)
{
	WORD &w2 = rec.r2.w2;

	if (fRevert) {
		m_iSP = w2 >> 8;
		m_iUse = w2 & 255;

		isComplex = (w2 & 0x04) ? true : false;

		Validates();
		UpdateUse();
	} else {
		ModifySetVal(w2, 0xFF00, m_iSP << 8);
		ModifySetVal(w2, 0x00FF, m_iUse);
	}
	return true;
}

void CEdPIText2Page::enableWindows(const UINT *nIDs, bool fEnabled)
{
	for (int x=0; nIDs[x] != 0; x++) {
		CWnd *win = GetDlgItem(nIDs[x]);
		if (win == NULL) continue;

		if (win->IsWindowEnabled() != (BOOL)fEnabled) {
			win->EnableWindow(fEnabled);
		}
	}
}

BOOL CEdPIText2Page::OnInitDialog() 
{
	CEdPIDlg::OnInitDialog();
	
	m_wndSpinSP.SetRange(0, 255);

	static const struct {
		WORD mask;
		LPCSTR psz;
	} tbl[] = {
		0x0004,"Creature spawn point",
		0,NULL,
	};
	for (int x=0; x<128; x++) {
		WORD mask = 0x04
			|((x & 0x01) ? 0x01 : 0)
			|((x & 0x02) ? 0x02 : 0)
			|((x & 0x04) ? 0x08 : 0)
			|((x & 0x08) ? 0x10 : 0)
			|((x & 0x10) ? 0x20 : 0)
			|((x & 0x20) ? 0x40 : 0)
			|((x & 0x40) ? 0x80 : 0)
			;
		CString text;
		for (int t=0; tbl[t].psz != NULL; t++) {
			if (mask != tbl[t].mask)
				continue;
			text = tbl[t].psz;
			break;
		}
		if (text.IsEmpty())
			text.Format("x %02X", 0U +mask);
		int r = m_wndUse.AddString(text);
		m_wndUse.SetItemData(r, mask);
	}
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CEdPIText2Page::OnSelchangeComboUse() 
{
	if (!UpdateData())
		return;

	int r = m_wndUse.GetCurSel();
	if (r != -1) {
		m_iUse = m_wndUse.GetItemData(r);
	}

	Validates();
}

void CEdPIText2Page::UpdateUse()
{
	LPARAM lParam = m_iUse;

	{
		int cx = m_wndUse.GetCount();
		for (int x=0; x<cx; x++) {
			if (m_wndUse.GetItemData(x) == lParam) {
				m_wndUse.SetCurSel(x);
				break;
			}
		}
	}
}

void CEdPIText2Page::Validates()
{
	bool fComplex = isComplex;
	bool fSP = (m_iUse == 0x0004);

	static const UINT nID_Use[] = {
		IDC_STATIC_USE_TYPE,
		IDC_COMBO_USE,
		0,
	};
	static const UINT nID_SP[] = {
		IDC_STATIC_SP,
		IDC_EDIT_SP,
		IDC_SPIN_SP,
		0,
	};

	enableWindows(nID_Use, fComplex);
	enableWindows(nID_SP, fComplex && fSP);
}
