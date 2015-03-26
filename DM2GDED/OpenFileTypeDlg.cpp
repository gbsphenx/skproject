// OpenFileTypeDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DM2GDED.h"
#include "OpenFileTypeDlg.h"
#include "GDAT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COpenFileTypeDlg::OpenTbl COpenFileTypeDlg::m_tblOpen[] ={
	//                   LE, L, P, S, F, R
	{"DM2 PC",            1, 0, 0, 1, 1, 0},
	{"DM2 PC v0.9 Beta",  1, 1, 1, 2 ,2, 1},
	{"DM2 Macintosh",     0, 0, 0, 1 ,1, 0},
	{"DM2 Amiga",         0, 0, 0, 1 ,0, 1},
	{"DM2 Sega",          0, 0, 2, 3 ,2, 1},
	{"DM2 PC98",          1, 2, 0, 0 ,0, 0},
	{"DM2 PS/V",          1, 3, 0, 4, 0, 0},
	{"DM2 Macintosh JP",  0, 3, 0, 3 ,1, 1},
	{"DM2 FM-Towns",      1, 3, 2, 5 ,3, 1},
	{NULL,                0, 0, 0, 0, 0, 0},
};

// (L)anguage
// 0 Latin
// 1 Latin (Plain)
// 2 PC98xx Latin & Japanese
// 3 PS/V or Mac JP or FM-Towns, Latin & Japanese

// (P)alette
// 0 From image
// 1 From raw palette
// 2 From scene palette

// (S)ound format
// 0 Unknown
// 1 SND5/SND6
// 2 SND3
// 3 SND7
// 4 SND7 LE
// 5 SND10

// Picture (F)ormat
// 0 C4
// 1 C4C8
// 2 Sega
// 3 FM-Towns

// (R)estrict image palette
// 0 no
// 1 yes

/////////////////////////////////////////////////////////////////////////////
// COpenFileTypeDlg ダイアログ

COpenFileTypeDlg::COpenFileTypeDlg(LPCTSTR pszFileIn, CWnd* pParent /*=NULL*/)
	: CDialog(COpenFileTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenFileTypeDlg)
	m_iLangSel = -1;
	m_iPlatformSel = -1;
	m_strFileIn = _T("");
	m_iSndSel = -1;
	m_iPalSel = -1;
	m_fRestrictPal = FALSE;
	m_iPicFmtSel = -1;
	//}}AFX_DATA_INIT

	m_strFileIn = pszFileIn;
}


void COpenFileTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenFileTypeDlg)
	DDX_Control(pDX, IDC_COMBO_PLATFORM, m_wndPlatform);
	DDX_CBIndex(pDX, IDC_COMBO_LANG, m_iLangSel);
	DDX_CBIndex(pDX, IDC_COMBO_PLATFORM, m_iPlatformSel);
	DDX_Text(pDX, IDC_EDIT_FILEIN, m_strFileIn);
	DDX_CBIndex(pDX, IDC_COMBO_SND, m_iSndSel);
	DDX_CBIndex(pDX, IDC_COMBO_PAL, m_iPalSel);
	DDX_Check(pDX, IDC_CHECK_FREEDOM_PALETTE, m_fRestrictPal);
	DDX_CBIndex(pDX, IDC_COMBO_PICFMT, m_iPicFmtSel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenFileTypeDlg, CDialog)
	//{{AFX_MSG_MAP(COpenFileTypeDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_PLATFORM, OnSelchangeComboPlatform)
	ON_BN_CLICKED(IDRETRY, OnRetry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenFileTypeDlg メッセージ ハンドラ

void COpenFileTypeDlg::OnSelchangeComboPlatform() 
{
	if (!UpdateData()) return;

	int i = m_iPlatformSel;
	OpenTbl *pTbl = m_tblOpen;
	while (pTbl->pszName != NULL && i != 0) i--, pTbl++;

//	m_iEndianSel = pTbl->fLE ? 0 : 1;
	m_iLangSel = pTbl->iLang;
//	m_fTraditional = pTbl->fTraditional;
	m_iSndSel = pTbl->iSnd;
//	m_fPalIndependent = pTbl->fPaletteIndependent;
	m_iPalSel = pTbl->iPal;
	m_iPicFmtSel = pTbl->iPicFmt;
	m_fRestrictPal = pTbl->fRestrictPal;

	UpdateData(FALSE);
	
}

UINT COpenFileTypeDlg::GetGDOF()
{
	return 0
//		|((m_iEndianSel == 0) ? GDFF_LITTLE_ENDIAN : 0)
		|((m_iLangSel == 0) ? GDFF_LANG_EN_ENC : 0)
		|((m_iLangSel == 1) ? GDFF_LANG_EN : 0)
		|((m_iLangSel == 2) ? GDFF_LANG_JP1_ENC : 0)
		|((m_iLangSel == 3) ? GDFF_LANG_JP2_ENC : 0)
		|((m_iSndSel == 1) ? GDFF_SND5SND6 : 0)
		|((m_iSndSel == 2) ? GDFF_SND3 : 0)
		|((m_iSndSel == 3) ? GDFF_SND7 : 0)
		|((m_iSndSel == 4) ? GDFF_SNDA : 0)
		|((m_iSndSel == 5) ? GDFF_SND9 : 0)
		|((m_iPalSel == 0) ? GDFF_PAL_IMAGE : 0)
		|((m_iPalSel == 1) ? GDFF_PAL_RAW : 0)
		|((m_iPalSel == 2) ? GDFF_PAL_SCENE : 0)
		|((m_fRestrictPal) ? GDFF_RESTRICT_PAL : 0)
		|((m_iPicFmtSel == 0) ? GDFF_PICFMT_C4 : 0)
		|((m_iPicFmtSel == 1) ? GDFF_PICFMT_C4C8 : 0)
		|((m_iPicFmtSel == 2) ? GDFF_PICFMT_SEGA : 0)
		|((m_iPicFmtSel == 3) ? GDFF_PICFMT_FMTOWNS : 0)
		;
}

void COpenFileTypeDlg::OnOK() 
{
	if (!UpdateData()) return;

	if (GetACP() != 932 && m_iLangSel == 2) {
		int r = AfxMessageBox(IDS_WARN_NOT_CP932, MB_RETRYCANCEL|MB_ICONEXCLAMATION);
		if (r != IDRETRY) return;
	}
	
	CDialog::OnOK();
}

BOOL COpenFileTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UINT i;
	for (i = 0; m_tblOpen[i].pszName != NULL; i++) {
		m_wndPlatform.AddString(m_tblOpen[i].pszName);
	}
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void COpenFileTypeDlg::OnRetry() 
{
	EndDialog(IDRETRY);
}
