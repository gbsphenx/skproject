// EdPICreaturePage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdPICreaturePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdPICreaturePage プロパティ ページ

IMPLEMENT_DYNCREATE(CEdPICreaturePage, CEdPIDlg)

CEdPICreaturePage::CEdPICreaturePage() : CEdPIDlg(CEdPICreaturePage::IDD)
{
	//{{AFX_DATA_INIT(CEdPICreaturePage)
	m_iSel = -1;
	//}}AFX_DATA_INIT
}

void CEdPICreaturePage::DoDataExchange(CDataExchange* pDX)
{
	CEdPIDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdPICreaturePage)
	DDX_Control(pDX, IDC_COMBO_CREATURE, m_wndSel);
	DDX_CBIndex(pDX, IDC_COMBO_CREATURE, m_iSel);
	DDX_Control(pDX, IDC_COMBO_DIR, m_wndDir);
	DDX_CBIndex(pDX, IDC_COMBO_DIR, m_iDir);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEdPICreaturePage, CEdPIDlg)
	//{{AFX_MSG_MAP(CEdPICreaturePage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdPICreaturePage メッセージ ハンドラ

bool CEdPICreaturePage::RevertRecord(DMDBU &rec, bool fRevert)
{
	if (fRevert) {
		m_iSel = (rec.r4.b4) & 127;
		m_iDir = (rec.r4.w14 >> 8) & 3;
	} else {
		ModifySetVal(rec.r4.b4, 0x7F, m_iSel & 127);
		ModifySetVal(rec.r4.w14, 0x0300, m_iDir << 8);
	}
	return true;
}

BOOL CEdPICreaturePage::OnInitDialog() 
{
	CEdPIDlg::OnInitDialog();
	
	for (int i = 0; i < 127; i++) {
		m_wndSel.AddString(CDM2NameDic::GetInstance().GetName(4, i));
	}

	m_wndDir.AddString(_T("N"));
	m_wndDir.AddString(_T("E"));
	m_wndDir.AddString(_T("S"));
	m_wndDir.AddString(_T("W"));
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
