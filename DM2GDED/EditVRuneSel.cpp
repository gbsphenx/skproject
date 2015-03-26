// EditVRuneSel.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditVRuneSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditVRuneSelPg ダイアログ

void CEditVRuneSelPg::Construction()
{
	//{{AFX_DATA_INIT(CEditVRuneSelPg)
	//}}AFX_DATA_INIT
}

void CEditVRuneSelPg::DoDataExchange(CDataExchange* pDX)
{
	CEditVPg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditVRuneSelPg)
	DDX_Control(pDX, IDC_RUNE_SEL, m_wndRuneSel);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEditVRuneSelPg, CEditVPg)
	//{{AFX_MSG_MAP(CEditVRuneSelPg)
	ON_WM_CREATE()
	ON_CONTROL(STN_DBLCLK, IDC_RUNE_SEL, OnRuneSelDblClk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditVRuneSelPg メッセージ ハンドラ

int CEditVRuneSelPg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_wndRuneSel.SetSel(m_nInitVal -0x66);

	if (CEditVPg::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

CString CEditVRuneSelPg::FormatDisp(WORD x)
{
	switch (x) {
		case 0x66: return "YA";
		case 0x67: return "VI";
		case 0x68: return "OH";
		case 0x69: return "FUL";
		case 0x6A: return "DES";
		case 0x6B: return "ZO";

		case 0x6C: return "VEN";
		case 0x6D: return "EW";
		case 0x6E: return "KATH";
		case 0x6F: return "IR";
		case 0x70: return "BRO";
		case 0x71: return "GOR";

		case 0x72: return "KU";
		case 0x73: return "ROS";
		case 0x74: return "DAIN";
		case 0x75: return "NETA";
		case 0x76: return "RA";
		case 0x77: return "SAR";

		case 0: return "(none)";
	}
	return "?";
}

CString CEditVRuneSelPg::Format(WORD x)
{
	return Unsigned_Format(x);
}

bool CEditVRuneSelPg::Parse(LPCSTR pszText, WORD &x)
{
	return Unsigned_Parse(pszText, x);
}

void CEditVRuneSelPg::SetParm1(LPCSTR psz)
{

}

void CEditVRuneSelPg::OnRuneSelDblClk() 
{
	OnOK();
}
