// EditVWordValPg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditVWordValPg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditVWordValPg ダイアログ

void CEditVWordValPg::Construction()
{
	//{{AFX_DATA_INIT(CEditVWordValPg)
	m_iSel = -1;
	m_strVal = _T("");
	//}}AFX_DATA_INIT
}

void CEditVWordValPg::DoDataExchange(CDataExchange* pDX)
{
	CEditVPg::DoDataExchange(pDX);

	if (pDX->m_bSaveAndValidate) {

	} else {
		m_strVal = GetCur().Format(m_nVal);
	}

	CWordValCmd &form = GetCur();

	//{{AFX_DATA_MAP(CEditVWordValPg)
	DDX_Radio(pDX, IDC_RADIO_D10S, m_iSel);
	DDX_Text(pDX, IDC_EDIT_VAL, m_strVal);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
		pDX->PrepareEditCtrl(IDC_EDIT_VAL);
		WORD x;
		if (!form.Parse(m_strVal, x)) {
			AfxMessageBox(IDS_ERR_ILLEGAL_INT);
			pDX->Fail();
		}
		m_nVal = (__int16)x;
	}
}

BEGIN_MESSAGE_MAP(CEditVWordValPg, CEditVPg)
	//{{AFX_MSG_MAP(CEditVWordValPg)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_RADIO_D10S, OnRadioD10)
	ON_BN_CLICKED(IDC_RADIO_D10U, OnRadioD10)
	ON_BN_CLICKED(IDC_RADIO_D16, OnRadioD10)
	ON_BN_CLICKED(IDC_RADIO_D2, OnRadioD10)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditVWordValPg メッセージ ハンドラ

CWordValCmd_Signed _wvSigned;
CWordValCmd_Unsigned _wvUnsigned;
CWordValCmd_Hex _wvHex;
CWordValCmd_Flags _wvFlags;

CWordValCmd &CEditVWordValPg::GetCur()
{
	switch (m_iSel) {
	case 0: return _wvSigned;
	case 1: return _wvUnsigned;
	case 2: return _wvHex;
	case 3: return _wvFlags;
	default:return _wvUnsigned;
	}
}

int CEditVWordValPg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_nVal = m_nInitVal;

	switch (m_wvct = GetWvct()) {
	case wvctSigned: m_iSel = 0; break;
	case wvctUnsigned: default: m_iSel = 1; break;
	case wvctHex: m_iSel = 2; break;
	case wvctFlags: m_iSel = 3; break;
	}
	
	m_iSel = m_sel;

	if (CEditVPg::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CEditVWordValPg::OnRadioD10() 
{
	if (!UpdateData()) return;

	switch (m_iSel) {
	case 0: m_wvct = wvctSigned; break;
	case 1: m_wvct = wvctUnsigned; break;
	case 2: m_wvct = wvctHex; break;
	case 3: m_wvct = wvctFlags; break;
	}

	UpdateData(FALSE);
}

BOOL CEditVWordValPg::OnInitDialog() 
{
	CEditVPg::OnInitDialog();
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
