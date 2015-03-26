// EdPIMiscPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdPIMiscPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdPIMiscPage プロパティ ページ

IMPLEMENT_DYNCREATE(CEdPIMiscPage, CEdPIDlg)

CEdPIMiscPage::CEdPIMiscPage() : CEdPIDlg(CEdPIMiscPage::IDD)
{
	//{{AFX_DATA_INIT(CEdPIMiscPage)
	m_fA = FALSE;
	m_fB = FALSE;
	m_fC = FALSE;
	m_fD = FALSE;
	m_f8 = FALSE;
	m_f9 = FALSE;
	m_iSel = -1;
	m_iState = -1;
	m_f7 = FALSE;
	//}}AFX_DATA_INIT
}

void CEdPIMiscPage::DoDataExchange(CDataExchange* pDX)
{
	CEdPIDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdPIMiscPage)
	DDX_Control(pDX, IDC_COMBO_MISC, m_wndSel);
	DDX_Check(pDX, IDC_CHECK_N10, m_fA);
	DDX_Check(pDX, IDC_CHECK_N11, m_fB);
	DDX_Check(pDX, IDC_CHECK_N12, m_fC);
	DDX_Check(pDX, IDC_CHECK_N13, m_fD);
	DDX_Check(pDX, IDC_CHECK_N8, m_f8);
	DDX_Check(pDX, IDC_CHECK_N9, m_f9);
	DDX_CBIndex(pDX, IDC_COMBO_MISC, m_iSel);
	DDX_CBIndex(pDX, IDC_COMBO_STATE, m_iState);
	DDX_Check(pDX, IDC_CHECK_N7, m_f7);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEdPIMiscPage, CEdPIDlg)
	//{{AFX_MSG_MAP(CEdPIMiscPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdPIMiscPage メッセージ ハンドラ

bool CEdPIMiscPage::RevertRecord(DMDBU &rec, bool fRevert)
{
	if (fRevert) {
		m_iSel = (rec.r10.w2) & 127;
		m_iState = (rec.r10.w2 / 0x4000) & 3;
		m_f7 = (rec.r10.w2 & 0x0080) ? 1 : 0;
		m_f8 = (rec.r10.w2 & 0x0100) ? 1 : 0;
		m_f9 = (rec.r10.w2 & 0x0200) ? 1 : 0;
		m_fA = (rec.r10.w2 & 0x0400) ? 1 : 0;
		m_fB = (rec.r10.w2 & 0x0800) ? 1 : 0;
		m_fC = (rec.r10.w2 & 0x1000) ? 1 : 0;
		m_fD = (rec.r10.w2 & 0x2000) ? 1 : 0;
	} else {
		ModifySetVal(rec.r10.w2, 0x007F, m_iSel & 127);
		ModifySetVal(rec.r10.w2, 0xC000, 0x4000 * (m_iState & 3));
		MaskedModify(rec.r10.w2, 0x0080, m_f7);
		MaskedModify(rec.r10.w2, 0x0100, m_f8);
		MaskedModify(rec.r10.w2, 0x0200, m_f9);
		MaskedModify(rec.r10.w2, 0x0400, m_fA);
		MaskedModify(rec.r10.w2, 0x0800, m_fB);
		MaskedModify(rec.r10.w2, 0x1000, m_fC);
		MaskedModify(rec.r10.w2, 0x2000, m_fD);
	}
	return true;
}

BOOL CEdPIMiscPage::OnInitDialog() 
{
	CEdPIDlg::OnInitDialog();
	
	for (int i = 0; i < 127; i++) {
		m_wndSel.AddString(CDM2NameDic::GetInstance().GetName(10, i));
	}
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
