// EdPIScrollPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdPIScrollPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdPIScrollPage プロパティ ページ

IMPLEMENT_DYNCREATE(CEdPIScrollPage, CEdPIDlg)

CEdPIScrollPage::CEdPIScrollPage() : CEdPIDlg(CEdPIScrollPage::IDD)
{
	//{{AFX_DATA_INIT(CEdPIScrollPage)
	m_iUse = -1;
	//}}AFX_DATA_INIT
}

void CEdPIScrollPage::DoDataExchange(CDataExchange* pDX)
{
	CEdPIDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdPIScrollPage)
	DDX_Control(pDX, IDC_COMBO_TEXT, m_wndSel);
	DDX_Radio(pDX, IDC_RADIOM1, m_iUse);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEdPIScrollPage, CEdPIDlg)
	//{{AFX_MSG_MAP(CEdPIScrollPage)
	ON_BN_CLICKED(IDC_RADIOM1, OnRadioM1)
	ON_BN_CLICKED(IDC_RADIOM2, OnRadioM1)
	ON_CBN_SELCHANGE(IDC_COMBO_TEXT, OnSelchangeComboText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdPIScrollPage メッセージ ハンドラ

bool CEdPIScrollPage::RevertRecord(DMDBU &rec, bool fRevert)
{
	if (fRevert) {
		m_iLowSel = (rec.r7.w2 & 0x03FF);
		m_iSel = (rec.r7.w2 >> 10);

		m_fExttext = (m_iSel > 1);
		m_iUse = (m_fExttext ? 1 : 0);
		UpdateUse();
	} else {
		ModifySetVal(rec.r7.w2, 0x03FF, m_iLowSel);
		ModifySetVal(rec.r7.w2, 0xFC00, m_iSel << 10);
	}
	return true;
}

BOOL CEdPIScrollPage::OnInitDialog() 
{
	CEdPIDlg::OnInitDialog();
	
	m_wndSel.SetCount(256);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CEdPIScrollPage::UpdateUse()
{
	DM2NameResolver_t *nameRes = GetNameRes();
	if (nameRes == NULL)
		return;

	const int cx = 256;
	if (m_fExttext) {
		for (int x=0; x<cx; x++) {
			m_wndSel.ai[x].suffix.Empty();
			m_wndSel.ai[x].text = nameRes->GetExttextStr(x);
			m_wndSel.ai[x].lp = x;
		}

		m_wndSel.Sort();
		m_wndSel.SetCurSel(m_wndSel.FindIntData(m_iSel));
	} else {
		CWordArray vec;
		nameRes->EnumTextRecords(vec);

		const UINT cxVec = vec.GetSize();

		for (int x=0; x<cx; x++) {
			WORD t=0;
			m_wndSel.ai[x].suffix.Empty();
			if (x < cxVec) {
				t = vec[x];
				m_wndSel.ai[x].text = nameRes->GetIntScrollStr(t);
				m_wndSel.ai[x].lp = t;
			} else {
				m_wndSel.ai[x].text.Empty();
				m_wndSel.ai[x].lp = 0;
			}
		}

		m_wndSel.Sort();
		m_wndSel.SetCurSel(m_wndSel.FindIntData(m_iLowSel));
	}
}

void CEdPIScrollPage::OnRadioM1() 
{
	if (!UpdateData())
		return;

	if (m_iUse == 0) { // int
		m_iSel = 0;
	} else { // ext

	}

	m_fExttext = (m_iUse == 0) ? false : true;

	UpdateUse();
}

void CEdPIScrollPage::OnSelchangeComboText() 
{
	LPARAM lp = m_wndSel.GetIntData(m_wndSel.GetCurSel());

	if (m_fExttext) {
		m_iSel = lp;
	} else {
		m_iLowSel = lp;
	}
}
