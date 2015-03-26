// EdPIPotionPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdPIPotionPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdPIPotionPage プロパティ ページ

IMPLEMENT_DYNCREATE(CEdPIPotionPage, CEdPIDlg)

CEdPIPotionPage::CEdPIPotionPage() : CEdPIDlg(CEdPIPotionPage::IDD)
{
	//{{AFX_DATA_INIT(CEdPIPotionPage)
	m_iSel = -1;
	m_iStr = 0;
	m_fF = FALSE;
	//}}AFX_DATA_INIT
}

void CEdPIPotionPage::DoDataExchange(CDataExchange* pDX)
{
	CEdPIDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdPIPotionPage)
	DDX_Control(pDX, IDC_COMBO_POTION, m_wndSel);
	DDX_Control(pDX, IDC_SLIDER_STR, m_wndStr);
	DDX_CBIndex(pDX, IDC_COMBO_POTION, m_iSel);
	DDX_Slider(pDX, IDC_SLIDER_STR, m_iStr);
	DDX_Check(pDX, IDC_CHECK_N15, m_fF);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEdPIPotionPage, CEdPIDlg)
	//{{AFX_MSG_MAP(CEdPIPotionPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdPIPotionPage メッセージ ハンドラ

bool CEdPIPotionPage::RevertRecord(DMDBU &rec, bool fRevert)
{
	if (fRevert) {
		m_iSel = (rec.r8.w2 / 0x0100) & 127;
		m_iStr = (rec.r8.w2) & 255;
		m_fF = (rec.r8.w2 & 0x8000) ? 1 : 0;
	} else {
		MaskedModify(rec.r8.w2, 0x8000, m_fF);
		ModifySetVal(rec.r8.w2, 0x00FF, m_iStr & 255);
		ModifySetVal(rec.r8.w2, 0x7F00, 0x0100 * (m_iSel & 127));
	}
	return true;
}

BOOL CEdPIPotionPage::OnInitDialog() 
{
	CEdPIDlg::OnInitDialog();
	
	m_wndStr.SetRange(0, 255, TRUE);

	m_wndStr.SetTic(0);
	m_wndStr.SetTic(0x14);
	m_wndStr.SetTic(0x1E);
	m_wndStr.SetTic(0x24);
	m_wndStr.SetTic(0xC8);
	m_wndStr.SetTic(0xF0);
	m_wndStr.SetTic(255);

	for (int i = 0; i < 127; i++) {
		m_wndSel.AddString(CDM2NameDic::GetInstance().GetName(8, i));
	}
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
