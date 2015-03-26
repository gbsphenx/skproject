// EdPIWeapon.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdPIWeapon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdPIWeaponPage プロパティ ページ

IMPLEMENT_DYNCREATE(CEdPIWeaponPage, CEdPIDlg)

CEdPIWeaponPage::CEdPIWeaponPage() : CEdPIDlg(CEdPIWeaponPage::IDD)
{
	//{{AFX_DATA_INIT(CEdPIWeaponPage)
	m_fE = FALSE;
	m_fF = FALSE;
	m_f8 = FALSE;
	m_f9 = FALSE;
	m_nCharges = -1;
	m_iSel = -1;
	m_f7 = FALSE;
	//}}AFX_DATA_INIT
}

void CEdPIWeaponPage::DoDataExchange(CDataExchange* pDX)
{
	CEdPIDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdPIWeaponPage)
	DDX_Control(pDX, IDC_COMBO_WEAPON, m_wndSel);
	DDX_Check(pDX, IDC_CHECK_N14, m_fE);
	DDX_Check(pDX, IDC_CHECK_N15, m_fF);
	DDX_Check(pDX, IDC_CHECK_N8, m_f8);
	DDX_Check(pDX, IDC_CHECK_N9, m_f9);
	DDX_CBIndex(pDX, IDC_COMBO_N_CHARGES, m_nCharges);
	DDX_CBIndex(pDX, IDC_COMBO_WEAPON, m_iSel);
	DDX_Check(pDX, IDC_CHECK_N7, m_f7);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEdPIWeaponPage, CEdPIDlg)
	//{{AFX_MSG_MAP(CEdPIWeaponPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdPIWeaponPage メッセージ ハンドラ

bool CEdPIWeaponPage::RevertRecord(DMDBU &rec, bool fRevert)
{
	if (fRevert) {
		m_f7 = (rec.r5.w2 & 0x0080) ? 1 : 0;
		m_f8 = (rec.r5.w2 & 0x0100) ? 1 : 0;
		m_f9 = (rec.r5.w2 & 0x0200) ? 1 : 0;
		m_fE = (rec.r5.w2 & 0x4000) ? 1 : 0;
		m_fF = (rec.r5.w2 & 0x8000) ? 1 : 0;
		m_nCharges = (rec.r5.w2 / 0x0400) & 15;
		m_iSel = rec.r5.w2 & 127;
	} else {
		MaskedModify(rec.r5.w2, 0x0080, m_f7);
		MaskedModify(rec.r5.w2, 0x0100, m_f8);
		MaskedModify(rec.r5.w2, 0x0200, m_f9);
		MaskedModify(rec.r5.w2, 0x4000, m_fE);
		MaskedModify(rec.r5.w2, 0x8000, m_fF);
		ModifySetVal(rec.r5.w2, 0x3C00, 0x0400 * (m_nCharges & 15));
		ModifySetVal(rec.r5.w2, 0x007F, m_iSel & 127);
	}
	return true;
}

BOOL CEdPIWeaponPage::OnInitDialog() 
{
	CEdPIDlg::OnInitDialog();
	
	for (int i = 0; i < 127; i++) {
		m_wndSel.AddString(CDM2NameDic::GetInstance().GetName(5, i));
	}
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
