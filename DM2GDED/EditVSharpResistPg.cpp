// EditVSharpResistPg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditVSharpResistPg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditVSharpResistPg ダイアログ

void CEditVSharpResistPg::Construction()
{
	//{{AFX_DATA_INIT(CEditVSharpResistPg)
	m_strHB = _T("");
	m_strLB = _T("");
	m_fH3 = FALSE;
	m_fH4 = FALSE;
	m_fH5 = FALSE;
	m_fH6 = FALSE;
	m_fH7 = FALSE;
	m_fL3 = FALSE;
	m_fL4 = FALSE;
	m_fL5 = FALSE;
	m_fL6 = FALSE;
	m_fL7 = FALSE;
	m_iH3 = -1;
	m_iL3 = -1;
	//}}AFX_DATA_INIT
}

void CEditVSharpResistPg::DoDataExchange(CDataExchange* pDX)
{
	CEditVPg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditVSharpResistPg)
	DDX_Text(pDX, IDC_STATIC_HB, m_strHB);
	DDX_Text(pDX, IDC_STATIC_LB, m_strLB);
	DDX_Check(pDX, IDC_CHECK_H3, m_fH3);
	DDX_Check(pDX, IDC_CHECK_H4, m_fH4);
	DDX_Check(pDX, IDC_CHECK_H5, m_fH5);
	DDX_Check(pDX, IDC_CHECK_H6, m_fH6);
	DDX_Check(pDX, IDC_CHECK_H7, m_fH7);
	DDX_Check(pDX, IDC_CHECK_L3, m_fL3);
	DDX_Check(pDX, IDC_CHECK_L4, m_fL4);
	DDX_Check(pDX, IDC_CHECK_L5, m_fL5);
	DDX_Check(pDX, IDC_CHECK_L6, m_fL6);
	DDX_Check(pDX, IDC_CHECK_L7, m_fL7);
	DDX_CBIndex(pDX, IDC_COMBO_H3, m_iH3);
	DDX_CBIndex(pDX, IDC_COMBO_L3, m_iL3);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEditVSharpResistPg, CEditVPg)
	//{{AFX_MSG_MAP(CEditVSharpResistPg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditVSharpResistPg メッセージ ハンドラ

int CEditVSharpResistPg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_iL3 = (m_nInitVal % 256) & 7;
	m_iH3 = (m_nInitVal / 256) & 7;

	m_fL3 = (m_nInitVal & (1 << 3)) ? 1 : 0;
	m_fL4 = (m_nInitVal & (1 << 4)) ? 1 : 0;
	m_fL5 = (m_nInitVal & (1 << 5)) ? 1 : 0;
	m_fL6 = (m_nInitVal & (1 << 6)) ? 1 : 0;
	m_fL7 = (m_nInitVal & (1 << 7)) ? 1 : 0;
	m_fH3 = (m_nInitVal & (1 <<11)) ? 1 : 0;
	m_fH4 = (m_nInitVal & (1 <<12)) ? 1 : 0;
	m_fH5 = (m_nInitVal & (1 <<13)) ? 1 : 0;
	m_fH6 = (m_nInitVal & (1 <<14)) ? 1 : 0;
	m_fH7 = (m_nInitVal & (1 <<15)) ? 1 : 0;

	CString strText;

	strText = GetHintText(0);
	if (strText.IsEmpty())
		strText = "First byte:";
	m_strLB = strText;

	strText = GetHintText(1);
	if (strText.IsEmpty())
		strText = "Second byte:";
	m_strHB = strText;

	if (CEditVPg::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

CString CEditVSharpResistPg::FormatB3(int iTextIdx, LPCTSTR pszDefText, UINT x)
{
	if (x & 8)
		return "";
	LPCTSTR pszText = GetHintText(iTextIdx);
	if (pszText == NULL)
		pszText = pszDefText;
	CString strRet;
	strRet.Format("%s: %d", pszText, x);
	return strRet;
}

CString CEditVSharpResistPg::FormatDisp(WORD x)
{
	UINT iLo = (x >> 0) & 15;
	UINT iHi = (x >> 8) & 15;

	CString strRet, strText;

	strRet = FormatB3(0, "First byte", iLo);

	strText = FormatB3(1, "Second byte", iHi);
	if (!strText.IsEmpty() && !strRet.IsEmpty()) strRet += ", ";
	strRet += strText;

	return strRet;
}

CString CEditVSharpResistPg::Format(WORD x)
{
	return Unsigned_Format(x);
}

bool CEditVSharpResistPg::Parse(LPCSTR pszText, WORD &x)
{
	return Unsigned_Parse(pszText, x);
}

void CEditVSharpResistPg::SetParm1(LPCSTR psz)
{
	LPCSTR pszMark = _tcschr(psz, '/');
	m_str2[0] = (pszMark == NULL) ? psz : CString(psz, pszMark -psz);
	if (pszMark != NULL) {
		psz = pszMark +1;
		m_str2[1] = psz;
	}
}
