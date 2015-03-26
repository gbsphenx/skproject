// EdPIActu2Page.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdPIActu2Page.h"
#include "DM2SelOrnateImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdPIActu2Page プロパティ ページ

IMPLEMENT_DYNCREATE(CEdPIActu2Page, CEdPIDlg)

CEdPIActu2Page::CEdPIActu2Page() : CEdPIDlg(CEdPIActu2Page::IDD)
{
	//{{AFX_DATA_INIT(CEdPIActu2Page)
	m_fBeep = FALSE;
	m_fInverse = FALSE;
	m_fSwIsOn = FALSE;
	m_iOrnate = -1;
	m_nDelay = 0;
	m_iAT = -1;
	//}}AFX_DATA_INIT
}

void CEdPIActu2Page::DoDataExchange(CDataExchange* pDX)
{
	CEdPIDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdPIActu2Page)
	DDX_Control(pDX, IDC_COMBO_ORNATE, m_wndOrnate);
	DDX_Control(pDX, IDC_SPIN_DELAY, m_wndDelay);
	DDX_Check(pDX, IDC_CHECK_BEEP, m_fBeep);
	DDX_Check(pDX, IDC_CHECK_INVERSE, m_fInverse);
	DDX_Check(pDX, IDC_CHECK_SW_IS_ON, m_fSwIsOn);
	DDX_CBIndex(pDX, IDC_COMBO_ORNATE, m_iOrnate);
	DDX_Text(pDX, IDC_EDIT_DELAY, m_nDelay);
	DDV_MinMaxUInt(pDX, m_nDelay, 0, 31);
	DDX_Radio(pDX, IDC_RADIO_AT_ON, m_iAT);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEdPIActu2Page, CEdPIDlg)
	//{{AFX_MSG_MAP(CEdPIActu2Page)
	ON_CBN_SELCHANGE(IDC_COMBO_ORNATE, OnSelchangeComboOrnate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdPIActu2Page メッセージ ハンドラ

bool CEdPIActu2Page::RevertRecord(DMDBU &rec, bool fRevert)
{
	if (fRevert) {
		m_fSwIsOn = (rec.r3.w4 & 0x0004) ? 1 : 0;
		m_iAT = (rec.r3.w4 / 0x0008) & 3;
		m_fInverse = (rec.r3.w4 & 0x0020) ? 1 : 0;
		m_fBeep = (rec.r3.w4 & 0x0040) ? 1 : 0;
		m_nDelay = (rec.r3.w4 / 0x0080) & 31;
		m_iOrnate = (rec.r3.w4 / 0x1000) & 15;

		if (MkOrnateIdxMig()) {
			m_wndOrnate.SelOrnate(TileIsWall());
			m_wndOrnate.Revert(TileIsWall() ? m_oim->m_wall : m_oim->m_floor, true);
			m_wndOrnate.Freshen();

			if (m_wndOrnate.GetOrnateIdx(m_iOrnate) < 0) m_iOrnate = 0;
		}
	} else {
		MaskedModify(rec.r9.w4, 0x0004, m_fSwIsOn);
		ModifySetVal(rec.r9.w4, 0x0018, 0x0008 * (m_iAT & 3));
		MaskedModify(rec.r9.w4, 0x0020, m_fInverse);
		MaskedModify(rec.r9.w4, 0x0040, m_fBeep);
		ModifySetVal(rec.r9.w4, 0x0F80, 0x0080 * (m_nDelay & 31));
		ModifySetVal(rec.r9.w4, 0xF000, 0x1000 * (m_iOrnate & 15));
	}
	return true;
}

void CEdPIActu2Page::OnSelchangeComboOrnate() 
{
	int iSel = m_iOrnate;

	if (!UpdateData()) return;

	if (MkOrnateIdxMig()) {
		if (m_wndOrnate.IsAddNew1(m_iOrnate)) {
			CDM2SelOrnateImageDlg wndDlg(false, TileIsWall(), this);
			int r = wndDlg.DoModal();
			if (r == IDOK) {
				CDM2OrnateIdxArray &oia = TileIsWall() ? m_oim->m_wall : m_oim->m_floor;
				int iNew = oia.Add(wndDlg.m_iOrnate);
				if (iNew < 0) iNew = 0; else iNew++;
				m_wndOrnate.Revert(oia, true);
				m_wndOrnate.Freshen();
				m_iOrnate = iNew;
			} else {
				m_iOrnate = iSel;
			}

			UpdateData(FALSE);
		}

		Revert(false);
	}
}

bool CEdPIActu2Page::IsAcc(UINT nID)
{
	switch (nID) {
	case IDC_COMBO_ORNATE:
		return false;
	}
	return true;
}

BOOL CEdPIActu2Page::OnInitDialog() 
{
	CEdPIDlg::OnInitDialog();
	
	m_wndDelay.SetRange(0, 31);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
