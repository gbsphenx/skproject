// EditVSSItemPg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditVSSItemPg.h"
#include "DM2RTextToken.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditVSSItemPg ダイアログ


void CEditVSSItemPg::Construction()
{
	//{{AFX_DATA_INIT(CEditVSSItemPg)
	m_iCat = -1;
	m_iItem = -1;
	m_iCnt = 1;
	m_nRand = 0;
	//}}AFX_DATA_INIT
}


void CEditVSSItemPg::DoDataExchange(CDataExchange* pDX)
{
	CEditVPg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditVSSItemPg)
	DDX_Control(pDX, IDC_LIST_ITEM, m_wndList);
	DDX_CBIndex(pDX, IDC_COMBO_CAT, m_iCat);
	DDX_LBIndex(pDX, IDC_LIST_ITEM, m_iItem);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_iCnt);
	DDV_MinMaxUInt(pDX, m_iCnt, 1, 16);
	DDX_Text(pDX, IDC_EDIT_RAND, m_nRand);
	DDV_MinMaxUInt(pDX, m_nRand, 0, 7);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditVSSItemPg, CEditVPg)
	//{{AFX_MSG_MAP(CEditVSSItemPg)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO_CAT, OnSelchangeComboCat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditVSSItemPg メッセージ ハンドラ

int CEditVSSItemPg::tbl4[] = {rttWeapons, rttClothes, rttMisc, rttPotions};

int CEditVSSItemPg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (m_fTypePrize) {
		m_iCat = (m_nInitVal >> 14) & 3;
		m_iInitItem = m_iItem = (m_nInitVal >> 7) & 127;
		m_iCnt = (m_nInitVal >> 0) & 15; m_iCnt++;
		m_nRand = (m_nInitVal >> 4) & 7;
	} else {
		m_iCat = (m_nInitVal >> 7) & 3;
		m_iInitItem = m_iItem = (m_nInitVal >> 0) & 127;
	}

	if (CEditVPg::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

BOOL CEditVSSItemPg::OnInitDialog() 
{
	CEditVPg::OnInitDialog();
	
	OnSel();

	m_wndList.SetCurSel(m_iInitItem);

	EnableItem(IDC_STATIC_CNT, m_fTypePrize);
	EnableItem(IDC_EDIT_COUNT, m_fTypePrize);
	EnableItem(IDC_STATIC_RND, m_fTypePrize);
	EnableItem(IDC_EDIT_RAND, m_fTypePrize);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CEditVSSItemPg::OnSel()
{
	if (!UpdateData()) return;

	m_wndList.SetRedraw(FALSE);

	m_wndList.ResetContent();

	CString strText, strName;
	for (UINT i = 0; i < 128; i++) {
		strName.Empty();
		m_vctx.Get4CatItemName(tbl4[m_iCat % 4], i, strName);

		strText.Format(strName.IsEmpty() ? "%02X" : "%02X - %s", 0U +i, (LPCTSTR)strName);
		m_wndList.AddString(strText);
	}

	m_wndList.SetRedraw(TRUE);
	m_wndList.Invalidate();
}

void CEditVSSItemPg::OnSelchangeComboCat() 
{
	OnSel();
}

//////////////////////////////////////////////////////////////////////
// CEditVSSKeyItemPg

CString CEditVSSKeyItemPg::FormatDisp(WORD x)
{
	CString strRet;
	LPCTSTR psz = "?";
	switch ((x >> 7) & 3) {
	case 0: psz = "Weapons"; break;
	case 1: psz = "Clothes"; break;
	case 2: psz = "Miscellaneous"; break;
	case 3: psz = "Potions"; break;
	}

	CString strName;
	m_vctx.Get4CatItemName(tbl4[(x >> 7) & 3], 0 +((x >> 0) & 127), strName);

	strRet.Format(
		"%s: %s"
		, psz
		, (LPCTSTR)strName
		);
	return strRet;
}

//////////////////////////////////////////////////////////////////////
// CEditVSSPrizeItemPg

CString CEditVSSPrizeItemPg::FormatDisp(WORD x)
{
	CString strRet;
	LPCTSTR psz = "?";
	switch ((x >> 14) & 3) {
	case 0: psz = "Weapons"; break;
	case 1: psz = "Clothes"; break;
	case 2: psz = "Miscellaneous"; break;
	case 3: psz = "Potions"; break;
	}

	CString strName;
	m_vctx.Get4CatItemName(tbl4[(x >> 14) & 3], 0 +((x >> 7) & 127), strName);

	int nMin = 1 +((x >> 0) & 15);
	int nMax = 0 +((x >> 4) & 7);
	strRet.Format(
		"%s: %s, Count: %d (+%d)"
		, psz
		, (LPCTSTR)strName
		, nMin
		, nMax
		);
	return strRet;
}
