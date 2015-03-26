// EditHeroStatDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "EditHeroStatDlg.h"
#include "EndianUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditHeroStatDlg ダイアログ


CEditHeroStatDlg::CEditHeroStatDlg(bool fLE, CWnd* pParent /*=NULL*/)
	: CDialog(CEditHeroStatDlg::IDD, pParent)
	, m_fLE(fLE)
{
	//{{AFX_DATA_INIT(CEditHeroStatDlg)
	//}}AFX_DATA_INIT
}


void CEditHeroStatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditHeroStatDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditHeroStatDlg, CDialog)
	//{{AFX_MSG_MAP(CEditHeroStatDlg)
	ON_BN_CLICKED(IDC_BUTTON_YUKMAN, OnButtonYukman)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditHeroStatDlg メッセージ ハンドラ

void CEditHeroStatDlg::setVal(UINT nID, UINT val, UINT minVal, UINT maxVal, UINT step) {
	CWnd *pEdit = static_cast<CEdit *>(GetDlgItem(nID));
	if (pEdit != NULL) {
		CString str;
		str.Format("%u", val);
		pEdit->SetWindowText(str);

		CSpinButtonCtrl *pSpin = static_cast<CSpinButtonCtrl *>(pEdit->GetNextWindow());
		if (pSpin != NULL) {
			pSpin->SetRange(minVal, maxVal);
		}
	}
}

void CEditHeroStatDlg::getValW(UINT nID, void *pw, UINT , UINT , UINT ) {
	CWnd *pEdit = static_cast<CEdit *>(GetDlgItem(nID));
	if (pEdit != NULL) {
		CString str;
		pEdit->GetWindowText(str);
		LPTSTR pszEnd = NULL;
		int w = _tcstol(str, &pszEnd, 10);
		if (pszEnd != NULL && *pszEnd == 0) {
			writeew(pw, w, m_fLE);
		}
	}
}

#define rw(P) readew(P, m_fLE)

void CEditHeroStatDlg::revert() {
	setVal(IDC_EDIT1 , rw(&m_bData[ 0]), 0, 999, 1); //hp
	setVal(IDC_EDIT2 , rw(&m_bData[ 2]), 0,9999,10); //stamina
	setVal(IDC_EDIT3 , rw(&m_bData[ 4]), 0, 999, 1); //mp
	setVal(IDC_EDIT4 , rw(&m_bData[ 6]), 0,  99, 1); //luck
	setVal(IDC_EDIT5 , rw(&m_bData[ 8]), 0,  99, 1); //str
	setVal(IDC_EDIT6 , rw(&m_bData[10]), 0,  99, 1); //dex
	setVal(IDC_EDIT7 , rw(&m_bData[12]), 0,  99, 1); //wis
	setVal(IDC_EDIT8 , rw(&m_bData[14]), 0,  99, 1); //vit
	setVal(IDC_EDIT9 , rw(&m_bData[16]), 0,  99, 1); //anti-magic
	setVal(IDC_EDIT10, rw(&m_bData[18]), 0,  99, 1); //anti-fire

	setVal(IDC_EDITF1, rw(&m_bData[20]), 0,  99, 1); //fighter
	setVal(IDC_EDITF2, rw(&m_bData[22]), 0,  99, 1); //fighter
	setVal(IDC_EDITF3, rw(&m_bData[24]), 0,  99, 1); //fighter
	setVal(IDC_EDITF4, rw(&m_bData[26]), 0,  99, 1); //fighter

	setVal(IDC_EDITN1, rw(&m_bData[28]), 0,  99, 1); //ninja
	setVal(IDC_EDITN2, rw(&m_bData[30]), 0,  99, 1); //ninja
	setVal(IDC_EDITN3, rw(&m_bData[32]), 0,  99, 1); //ninja
	setVal(IDC_EDITN4, rw(&m_bData[34]), 0,  99, 1); //ninja

	setVal(IDC_EDITW1, rw(&m_bData[36]), 0,  99, 1); //wizard
	setVal(IDC_EDITW2, rw(&m_bData[38]), 0,  99, 1); //wizard
	setVal(IDC_EDITW3, rw(&m_bData[40]), 0,  99, 1); //wizard
	setVal(IDC_EDITW4, rw(&m_bData[42]), 0,  99, 1); //wizard

	setVal(IDC_EDITP1, rw(&m_bData[42]), 0,  99, 1); //priest
	setVal(IDC_EDITP2, rw(&m_bData[44]), 0,  99, 1); //priest
	setVal(IDC_EDITP3, rw(&m_bData[46]), 0,  99, 1); //priest
	setVal(IDC_EDITP4, rw(&m_bData[48]), 0,  99, 1); //priest
}

void CEditHeroStatDlg::commit() {
	getValW(IDC_EDIT1 , (&m_bData[ 0]), 0, 999, 1); //hp
	getValW(IDC_EDIT2 , (&m_bData[ 2]), 0,9999,10); //stamina
	getValW(IDC_EDIT3 , (&m_bData[ 4]), 0, 999, 1); //mp
	getValW(IDC_EDIT4 , (&m_bData[ 6]), 0,  99, 1); //luck
	getValW(IDC_EDIT5 , (&m_bData[ 8]), 0,  99, 1); //str
	getValW(IDC_EDIT6 , (&m_bData[10]), 0,  99, 1); //dex
	getValW(IDC_EDIT7 , (&m_bData[12]), 0,  99, 1); //wis
	getValW(IDC_EDIT8 , (&m_bData[14]), 0,  99, 1); //vit
	getValW(IDC_EDIT9 , (&m_bData[16]), 0,  99, 1); //anti-magic
	getValW(IDC_EDIT10, (&m_bData[18]), 0,  99, 1); //anti-fire

	getValW(IDC_EDITF1, (&m_bData[20]), 0,  99, 1); //fighter
	getValW(IDC_EDITF2, (&m_bData[22]), 0,  99, 1); //fighter
	getValW(IDC_EDITF3, (&m_bData[24]), 0,  99, 1); //fighter
	getValW(IDC_EDITF4, (&m_bData[26]), 0,  99, 1); //fighter

	getValW(IDC_EDITN1, (&m_bData[28]), 0,  99, 1); //ninja
	getValW(IDC_EDITN2, (&m_bData[30]), 0,  99, 1); //ninja
	getValW(IDC_EDITN3, (&m_bData[32]), 0,  99, 1); //ninja
	getValW(IDC_EDITN4, (&m_bData[34]), 0,  99, 1); //ninja

	getValW(IDC_EDITW1, (&m_bData[36]), 0,  99, 1); //wizard
	getValW(IDC_EDITW2, (&m_bData[38]), 0,  99, 1); //wizard
	getValW(IDC_EDITW3, (&m_bData[40]), 0,  99, 1); //wizard
	getValW(IDC_EDITW4, (&m_bData[42]), 0,  99, 1); //wizard

	getValW(IDC_EDITP1, (&m_bData[42]), 0,  99, 1); //priest
	getValW(IDC_EDITP2, (&m_bData[44]), 0,  99, 1); //priest
	getValW(IDC_EDITP3, (&m_bData[46]), 0,  99, 1); //priest
	getValW(IDC_EDITP4, (&m_bData[48]), 0,  99, 1); //priest
}

BOOL CEditHeroStatDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	revert();
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CEditHeroStatDlg::OnOK() 
{
	commit();
	
	CDialog::OnOK();
}

void CEditHeroStatDlg::OnButtonYukman() 
{
	bool fLE = m_fLE;
	
	m_fLE = true;
	
	memcpy(m_bData, 
		"\xE7\x03\x28\x23\x20\x03\x50\x00\x50\x00\x50\x00\x50\x00\x50\x00"
		"\x50\x00\x50\x00\x0F\x00\x0F\x00\x0F\x00\x0F\x00\x0F\x00\x0F\x00"
		"\x0F\x00\x0F\x00\x0F\x00\x0F\x00\x0F\x00\x0F\x00\x0F\x00\x0F\x00"
		"\x0F\x00\x03\x00"
		, 52);

	revert();

	m_fLE = fLE;
}
