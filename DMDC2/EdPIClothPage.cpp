// EdPIClothPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdPIClothPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdPIClothPage プロパティ ページ

IMPLEMENT_DYNCREATE(CEdPIClothPage, CEdPIDlg)

CEdPIClothPage::CEdPIClothPage() : CEdPIDlg(CEdPIClothPage::IDD)
{
	//{{AFX_DATA_INIT(CEdPIClothPage)
	m_fA = FALSE;
	m_fB = FALSE;
	m_fC = FALSE;
	m_fD = FALSE;
	m_fE = FALSE;
	m_fF = FALSE;
	m_f8 = FALSE;
	m_f9 = FALSE;
	m_iSel = -1;
	m_f7 = FALSE;
	//}}AFX_DATA_INIT
}

void CEdPIClothPage::DoDataExchange(CDataExchange* pDX)
{
	CEdPIDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdPIClothPage)
	DDX_Control(pDX, IDC_COMBO_CLOTH, m_wndSel);
	DDX_Check(pDX, IDC_CHECK_N10, m_fA);
	DDX_Check(pDX, IDC_CHECK_N11, m_fB);
	DDX_Check(pDX, IDC_CHECK_N12, m_fC);
	DDX_Check(pDX, IDC_CHECK_N13, m_fD);
	DDX_Check(pDX, IDC_CHECK_N14, m_fE);
	DDX_Check(pDX, IDC_CHECK_N15, m_fF);
	DDX_Check(pDX, IDC_CHECK_N8, m_f8);
	DDX_Check(pDX, IDC_CHECK_N9, m_f9);
	DDX_CBIndex(pDX, IDC_COMBO_CLOTH, m_iSel);
	DDX_Check(pDX, IDC_CHECK_N7, m_f7);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEdPIClothPage, CEdPIDlg)
	//{{AFX_MSG_MAP(CEdPIClothPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdPIClothPage メッセージ ハンドラ

bool CEdPIClothPage::RevertRecord(DMDBU &rec, bool fRevert)
{
	if (fRevert) {
		m_iSel = (rec.r6.w2) & 127;
		m_f7 = (rec.r6.w2 & 0x0080) ? 1 : 0;
		m_f8 = (rec.r6.w2 & 0x0100) ? 1 : 0;
		m_f9 = (rec.r6.w2 & 0x0200) ? 1 : 0;
		m_fA = (rec.r6.w2 & 0x0400) ? 1 : 0;
		m_fB = (rec.r6.w2 & 0x0800) ? 1 : 0;
		m_fC = (rec.r6.w2 & 0x1000) ? 1 : 0;
		m_fD = (rec.r6.w2 & 0x2000) ? 1 : 0;
		m_fE = (rec.r6.w2 & 0x4000) ? 1 : 0;
		m_fF = (rec.r6.w2 & 0x8000) ? 1 : 0;
	} else {
		ModifySetVal(rec.r6.w2, 0x007F, m_iSel & 127);
		MaskedModify(rec.r6.w2, 0x0080, m_f7);
		MaskedModify(rec.r6.w2, 0x0100, m_f8);
		MaskedModify(rec.r6.w2, 0x0200, m_f9);
		MaskedModify(rec.r6.w2, 0x0400, m_fA);
		MaskedModify(rec.r6.w2, 0x0800, m_fB);
		MaskedModify(rec.r6.w2, 0x1000, m_fC);
		MaskedModify(rec.r6.w2, 0x2000, m_fD);
		MaskedModify(rec.r6.w2, 0x4000, m_fE);
		MaskedModify(rec.r6.w2, 0x8000, m_fF);
	}
	return true;
}

BOOL CEdPIClothPage::OnInitDialog() 
{
	CEdPIDlg::OnInitDialog();
	
	for (int i = 0; i < 128; i++) {
		m_wndSel.AddString(CDM2NameDic::GetInstance().GetName(6, i));
	}
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
