// EditVIconAnimPg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditVIconAnimPg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditVIconAnimPg

void CEditVIconAnimPg::Construction()
{
	//{{AFX_DATA_INIT(CEditVIconAnimPg)
	m_fAF4000 = FALSE;
	m_fAF8000 = FALSE;
	m_nFrameCnt = -1;
	m_iASF = -1;
	m_nAdvMax = -1;
	m_nAdv1 = -1;
	//}}AFX_DATA_INIT
}

CEditVIconAnimPg::~CEditVIconAnimPg()
{

}

BEGIN_MESSAGE_MAP(CEditVIconAnimPg, CEditVPg)
	//{{AFX_MSG_MAP(CEditVIconAnimPg)
	ON_BN_CLICKED(IDC_RADIO_ASF0, OnRadioAsf0)
	ON_BN_CLICKED(IDC_CHECK_AF8000, OnCheckAf8000)
	ON_BN_CLICKED(IDC_CHECK_AF4000, OnCheckAf4000)
	ON_CBN_SELCHANGE(IDC_COMBO_FRAMECNT, OnSelchangeComboFramecnt)
	ON_CBN_SELCHANGE(IDC_COMBO_MAXFRAMECNT, OnSelchangeComboMaxframecnt)
	ON_CBN_SELCHANGE(IDC_COMBO_FPA, OnSelchangeComboFpa)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_ASF2, OnRadioAsf0)
	ON_BN_CLICKED(IDC_RADIO_ASF3, OnRadioAsf0)
	ON_BN_CLICKED(IDC_RADIO_ASF5, OnRadioAsf0)
	ON_BN_CLICKED(IDC_RADIO_ASF6, OnRadioAsf0)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditVIconAnimPg メッセージ ハンドラ

BOOL CEditVIconAnimPg::OnInitDialog() 
{
	CEditVPg::OnInitDialog();

	m_nFlags = m_nInitVal;

	m_fAF4000 = (m_nFlags & 0x4000) ? true : false;
	m_fAF8000 = (m_nFlags & 0x8000) ? true : false;

	m_iASF = -1;
	switch (m_nFlags & 0x0F00) {
		case 0x0000: m_iASF = 0; break;
		case 0x0200: m_iASF = 1; break;
		case 0x0300: m_iASF = 2; break;
		case 0x0500: m_iASF = 3; break;
		case 0x0600: m_iASF = 4; break;
	}

	m_nFrameCnt = -1;
	m_nAdv1 = -1;
	m_nAdvMax = -1;

	switch (m_nFlags & 0x0F00) {
		case 0x0600:
			m_nAdv1 =   (m_nFlags >> 5) &  7;
			m_nAdvMax = (m_nFlags >> 0) & 31;
			break;
		default:
			m_nFrameCnt = (m_nFlags & 0xF);
			break;
	}

	UpdateData(false);

	OnRadioAsf0();

	return true;
}

void CEditVIconAnimPg::DoDataExchange(CDataExchange* pDX) 
{
	CEditVPg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditVIconAnimPg)
	DDX_Check(pDX, IDC_CHECK_AF4000, m_fAF4000);
	DDX_Check(pDX, IDC_CHECK_AF8000, m_fAF8000);
	DDX_CBIndex(pDX, IDC_COMBO_FRAMECNT, m_nFrameCnt);
	DDX_Radio(pDX, IDC_RADIO_ASF0, m_iASF);
	DDX_CBIndex(pDX, IDC_COMBO_MAXFRAMECNT, m_nAdvMax);
	DDX_CBIndex(pDX, IDC_COMBO_FPA, m_nAdv1);
	//}}AFX_DATA_MAP
}

BOOL CEditVIconAnimPg::OnCheckAny(UINT nID)
{
	return true;
}

void CEditVIconAnimPg::OnRadioAsf0() 
{
	UpdateData();

	SetSectionEnable(IDC_STATIC_SIMPLE, m_iASF != 4);
	SetSectionEnable(IDC_STATIC_COMPLEX, m_iASF == 4);

	switch (m_iASF) {
		case 0: m_nFlags &= 0xF0FF; m_nFlags |= 0x0000; break;
		case 1: m_nFlags &= 0xF0FF; m_nFlags |= 0x0200; break;
		case 2: m_nFlags &= 0xF0FF; m_nFlags |= 0x0300; break;
		case 3: m_nFlags &= 0xF0FF; m_nFlags |= 0x0500; break;
		case 4: m_nFlags &= 0xF0FF; m_nFlags |= 0x0600; break;
	}
}

void CEditVIconAnimPg::SetSectionEnable(UINT nID, bool fEnabled)
{
	CWnd *p = GetDlgItem(nID);
	while (p != NULL) {
		p->EnableWindow(fEnabled);

		p = p->GetNextWindow();

		if (p == NULL) break;

		switch (p->GetDlgCtrlID()) {
		case IDC_STATIC_SIMPLE:
		case IDC_STATIC_COMPLEX:
			break;
		}
	}
}

void CEditVIconAnimPg::OnCheckAf8000() 
{
	UpdateData();
	if (m_fAF8000 != 0) {
		m_nFlags |=   0x8000;
	} else {
		m_nFlags &= (~0x8000);
	}
}

void CEditVIconAnimPg::OnCheckAf4000() 
{
	UpdateData();
	if (m_fAF4000 != 0) {
		m_nFlags |=   0x4000;
	} else {
		m_nFlags &= (~0x4000);
	}
}

void CEditVIconAnimPg::OnSelchangeComboFramecnt() 
{
	UpdateData();
	m_nFlags &= 0xFFF0;
	m_nFlags |= (m_nFrameCnt & 15);
}

void CEditVIconAnimPg::OnSelchangeComboMaxframecnt() 
{
	UpdateData();
	m_nFlags &= 0xFFE0;
	m_nFlags |= (m_nAdvMax & 31);
}

void CEditVIconAnimPg::OnSelchangeComboFpa() 
{
	UpdateData();
	m_nFlags &= 0xFF1F;
	m_nFlags |= (m_nAdv1 & 7) << 5;
}
