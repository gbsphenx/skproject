// DM2GDEDOPInfoDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2GDEDOPInfoDlg.h"
#include "GDATDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDOPInfoDlg ダイアログ


CDM2GDEDOPInfoDlg::CDM2GDEDOPInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDM2GDEDOPInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDM2GDEDOPInfoDlg)
	m_strLE = _T("");
	m_strPath = _T("");
	m_strPM = _T("");
	m_strRP = _T("");
	m_strST = _T("");
	m_strTL = _T("");
	//}}AFX_DATA_INIT
}


void CDM2GDEDOPInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDM2GDEDOPInfoDlg)
	DDX_Text(pDX, IDC_EDIT_LE, m_strLE);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strPath);
	DDX_Text(pDX, IDC_EDIT_PM, m_strPM);
	DDX_Text(pDX, IDC_EDIT_RP, m_strRP);
	DDX_Text(pDX, IDC_EDIT_ST, m_strST);
	DDX_Text(pDX, IDC_EDIT_TL, m_strTL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDM2GDEDOPInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CDM2GDEDOPInfoDlg)
	ON_BN_CLICKED(IDCLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDOPInfoDlg メッセージ ハンドラ

void CDM2GDEDOPInfoDlg::SetGDOF(DWORD nGDOF)
{
	m_strTL = "?";
	switch (nGDOF & GDFF_LANG_MASK) {
	case GDFF_LANG_EN_ENC: m_strTL = "Latin (Encoded Format)"; break;
	case GDFF_LANG_EN: m_strTL = "Latin (Plain Format)"; break;
	case GDFF_LANG_JP1_ENC: m_strTL = "Latin & Japanese for PC98 (Encoded Format)"; break;
	case GDFF_LANG_JP2_ENC: m_strTL = "Latin & Japanese for PS/V;Mac JP;FM-Towns (Encoded Format)"; break;
	}
	m_strLE = (nGDOF & GDFF_LITTLE_ENDIAN) ? "Little Endian" : "Big Endian";
	m_strST = "?";
	switch (nGDOF & GDFF_SND_MASK) {
	case GDFF_SND5SND6: m_strST = "SND5/SND6"; break;
	case GDFF_SND3: m_strST = "SND3"; break;
	case GDFF_SND7: m_strST = "SND7"; break;
	case GDFF_SNDA: m_strST = "SNDA"; break;
	case GDFF_SND9: m_strST = "SND9"; break;
	case 0: m_strST = "Unknown"; break;
	}
	switch (nGDOF & GDFF_PAL_MASK) {
	case GDFF_PAL_IMAGE: m_strPM = "Internal (Embedded)"; break;
	case GDFF_PAL_RAW: m_strPM = "External (Entry Level Relation)"; break;
	case GDFF_PAL_SCENE: m_strPM = "No (Decided by Interface or Scene)"; break;
	}
	m_strRP = (nGDOF & GDFF_RESTRICT_PAL) ? "Yes. Subjected by Interface/Scene Palette" : "No. Free";
}

void CDM2GDEDOPInfoDlg::OnClose() 
{
	EndDialog(IDCLOSE);
	
}
