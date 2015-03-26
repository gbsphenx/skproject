// EdPIDoorPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdPIDoorPage.h"
#include "DM2SelOrnateImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdPIDoorPage プロパティ ページ

IMPLEMENT_DYNCREATE(CEdPIDoorPage, CEdPIDlg)

CEdPIDoorPage::CEdPIDoorPage() : CEdPIDlg(CEdPIDoorPage::IDD)
{
	//{{AFX_DATA_INIT(CEdPIDoorPage)
	m_fBashOk = FALSE;
	m_fDestroyOk = FALSE;
	m_fDoorBtn = FALSE;
	m_fDoor2 = FALSE;
	m_fA = FALSE;
	m_fB = FALSE;
	m_fC = FALSE;
	m_fD = FALSE;
	m_fE = FALSE;
	m_fF = FALSE;
	m_f9 = FALSE;
	m_fVert = FALSE;
	m_iOrnate = -1;
	m_iDoor = -1;
	//}}AFX_DATA_INIT
}

void CEdPIDoorPage::DoDataExchange(CDataExchange* pDX)
{
	CEdPIDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdPIDoorPage)
	DDX_Control(pDX, IDC_COMBO_DOOR, m_wndDoor);
	DDX_Control(pDX, IDC_COMBO_ORNATE, m_wndOrnate);
	DDX_Check(pDX, IDC_CHECK_BASH_OK, m_fBashOk);
	DDX_Check(pDX, IDC_CHECK_DESTROY_OK, m_fDestroyOk);
	DDX_Check(pDX, IDC_CHECK_DOOR_BUTTON, m_fDoorBtn);
	DDX_Check(pDX, IDC_CHECK_DOOR_TYPE, m_fDoor2);
	DDX_Check(pDX, IDC_CHECK_N10, m_fA);
	DDX_Check(pDX, IDC_CHECK_N11, m_fB);
	DDX_Check(pDX, IDC_CHECK_N12, m_fC);
	DDX_Check(pDX, IDC_CHECK_N13, m_fD);
	DDX_Check(pDX, IDC_CHECK_N14, m_fE);
	DDX_Check(pDX, IDC_CHECK_N15, m_fF);
	DDX_Check(pDX, IDC_CHECK_N9, m_f9);
	DDX_Check(pDX, IDC_CHECK_VERT, m_fVert);
	DDX_CBIndex(pDX, IDC_COMBO_ORNATE, m_iOrnate);
	DDX_CBIndex(pDX, IDC_COMBO_DOOR, m_iDoor);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEdPIDoorPage, CEdPIDlg)
	//{{AFX_MSG_MAP(CEdPIDoorPage)
	ON_BN_CLICKED(IDC_CHECK_DOOR_TYPE, OnCheckDoorType)
	ON_CBN_SELCHANGE(IDC_COMBO_DOOR, OnSelchangeComboDoor)
	ON_CBN_SELCHANGE(IDC_COMBO_ORNATE, OnSelchangeComboOrnate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdPIDoorPage メッセージ ハンドラ

bool CEdPIDoorPage::RevertRecord(DMDBU &rec, bool fRevert)
{
	if (fRevert) {
		m_fDoor2 = (rec.r0.w2 & 0x0001) ? 1 : 0;
		m_iOrnate = (rec.r0.w2 / 0x0002) & 15;
//		m_f4 = (rec.r0.w2 & 0x0010) ? 1 : 0;
		m_fVert = (rec.r0.w2 & 0x0020) ? 1 : 0;
		m_fDoorBtn = (rec.r0.w2 & 0x0040) ? 1 : 0;
		m_fDestroyOk = (rec.r0.w2 & 0x0080) ? 1 : 0;
		m_fBashOk = (rec.r0.w2 & 0x0100) ? 1 : 0;
		m_f9 = (rec.r0.w2 & 0x0200) ? 1 : 0;
		m_fA = (rec.r0.w2 & 0x0400) ? 1 : 0;
		m_fB = (rec.r0.w2 & 0x0800) ? 1 : 0;
		m_fC = (rec.r0.w2 & 0x1000) ? 1 : 0;
		m_fD = (rec.r0.w2 & 0x2000) ? 1 : 0;
		m_fE = (rec.r0.w2 & 0x4000) ? 1 : 0;
		m_fF = (rec.r0.w2 & 0x8000) ? 1 : 0;

		if (MkOrnateIdxMig()) {
			m_wndOrnate.Revert(m_oim->m_door, true);
			m_wndOrnate.SelDoorOrnate();
			m_wndOrnate.Freshen();

			if (m_wndOrnate.GetOrnateIdx(m_iOrnate) < 0) m_iOrnate = 0;

			m_iDoor = 1 + m_oim->m_iDoor[m_fDoor2 ? 1 : 0];
		}
	} else {
		MaskedModify(rec.r0.w2, 0x0001, m_fDoor2);
		ModifySetVal(rec.r0.w2, 0x000E, (m_iOrnate & 15) * 2);
//		MaskedModify(rec.r0.w2, 0x0010, m_f4);
		MaskedModify(rec.r0.w2, 0x0020, m_fVert);
		MaskedModify(rec.r0.w2, 0x0040, m_fDoorBtn);
		MaskedModify(rec.r0.w2, 0x0080, m_fDestroyOk);
		MaskedModify(rec.r0.w2, 0x0100, m_fBashOk);
		MaskedModify(rec.r0.w2, 0x0200, m_f9);
		MaskedModify(rec.r0.w2, 0x0400, m_fA);
		MaskedModify(rec.r0.w2, 0x0800, m_fB);
		MaskedModify(rec.r0.w2, 0x1000, m_fC);
		MaskedModify(rec.r0.w2, 0x2000, m_fD);
		MaskedModify(rec.r0.w2, 0x4000, m_fE);
		MaskedModify(rec.r0.w2, 0x8000, m_fF);
	}
	return true;
}

BOOL CEdPIDoorPage::OnInitDialog() 
{
	CEdPIDlg::OnInitDialog();
	
	m_wndDoor.AllDoor();
	m_wndDoor.Freshen();
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CEdPIDoorPage::OnCheckDoorType() 
{
	if (!MkOrnateIdxMig()) return;
	if (!UpdateData()) return;

	m_iDoor = 1 + m_oim->m_iDoor[m_fDoor2 ? 1 : 0];

	UpdateData(FALSE);
}

void CEdPIDoorPage::OnSelchangeComboDoor() 
{
	if (!MkOrnateIdxMig()) return;
	if (!UpdateData()) return;

	int iSel = m_iDoor;
	int iDoor = iSel - 1;

	m_oim->m_iDoor[m_fDoor2 ? 1 : 0] = iDoor;
}

bool CEdPIDoorPage::IsAcc(UINT nID)
{
	switch (nID) {
	case IDC_COMBO_ORNATE:
	case IDC_COMBO_DOOR:
		return false;
	}
	return true;
}

void CEdPIDoorPage::OnSelchangeComboOrnate() 
{
	int iSel = m_iOrnate;

	if (!MkOrnateIdxMig()) return;
	if (!UpdateData()) return;

	if (m_wndOrnate.IsAddNew1(m_iOrnate)) {
		CDM2SelOrnateImageDlg wndDlg(true, true, this);
		int r = wndDlg.DoModal();
		if (r == IDOK) {
			CDM2OrnateIdxArray &oia = m_oim->m_door;
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
