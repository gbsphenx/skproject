// EdPITele.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdPITele.h"
#include "DMTeleLocFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdPITelePage プロパティ ページ

IMPLEMENT_DYNCREATE(CEdPITelePage, CEdPIDlg)

CEdPITelePage::CEdPITelePage() : CEdPIDlg(CEdPITelePage::IDD)
{
	//{{AFX_DATA_INIT(CEdPITelePage)
	m_iRot = -1;
	m_iTar = -1;
	m_fBeep = FALSE;
	m_nMap = 0;
	m_iX = 0;
	m_iY = 0;
	m_iTF = -1;
	//}}AFX_DATA_INIT
}

void CEdPITelePage::DoDataExchange(CDataExchange* pDX)
{
	CEdPIDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdPITelePage)
	DDX_Radio(pDX, IDC_RADIO_ROT1, m_iRot);
	DDX_Radio(pDX, IDC_RADIO_TAR1, m_iTar);
	DDX_Check(pDX, IDC_CHECK_BEEP, m_fBeep);
	DDX_Text(pDX, IDC_EDIT_MAP, m_nMap);
	DDV_MinMaxUInt(pDX, m_nMap, 0, 255);
	DDX_Text(pDX, IDC_EDIT_X, m_iX);
	DDV_MinMaxUInt(pDX, m_iX, 0, 31);
	DDX_Text(pDX, IDC_EDIT_Y, m_iY);
	DDV_MinMaxUInt(pDX, m_iY, 0, 31);
	DDX_Radio(pDX, IDC_RADIO_V0, m_iTF);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEdPITelePage, CEdPIDlg)
	//{{AFX_MSG_MAP(CEdPITelePage)
	ON_BN_CLICKED(IDC_BUTTON_REF, OnButtonRef)
	ON_BN_CLICKED(IDC_BUTTON_SELF, OnButtonSelf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdPITelePage メッセージ ハンドラ

bool CEdPITelePage::RevertRecord(DMDBU &rec, bool fRevert)
{
	if (fRevert) {
		m_fBeep = (rec.r1.w2 & 0x8000) ? 1 : 0;
		m_nMap = (rec.r1.w4 / 0x0100) & 255;
		m_iX = (rec.r1.w2 / 0x0020) & 31;
		m_iY = (rec.r1.w2) & 31;
		m_iRot = (rec.r1.w2 / 0x0400) & 7;
		m_iTar = (rec.r1.w2 / 0x2000) & 3;
		m_iTF = (rec.r1.w4 & 0x0007);
	} else {
		MaskedModify(rec.r1.w2, 0x8000, m_fBeep);
		ModifySetVal(rec.r1.w4, 0xFF00, 0x0100 * (m_nMap & 255));
		ModifySetVal(rec.r1.w2, 0x03E0, 0x0020 * (m_iX & 31));
		ModifySetVal(rec.r1.w2, 0x001F, 0x0001 * (m_iY & 31));
		ModifySetVal(rec.r1.w2, 0x1C00, 0x0400 * (m_iRot & 7));
		ModifySetVal(rec.r1.w2, 0x6000, 0x2000 * (m_iTar & 3));
		ModifySetVal(rec.r1.w4, 0x0007, (m_iTF & 7));
	}
	return true;
}

void CEdPITelePage::OnButtonRef() 
{
	if (!UpdateData()) return;

	CDMTeleLocFrm frm(GetDDIf(), AtlasPos_t(m_nMap, m_iX, m_iY), true);
	int r = frm.DoModal(this);
	if (r == IDOK) {
		AtlasPos_t ap = frm.m_view.GetAP();
		m_nMap = ap.Z();
		m_iX = ap.X();
		m_iY = ap.Y();
		UpdateData(FALSE);

		Revert(false);
	}
}

void CEdPITelePage::OnButtonSelf() 
{
	if (!UpdateData()) return;

	AtlasPos_t ap;
	if (GetTilePos(ap)) {
		m_iX = ap.X();
		m_iY = ap.Y();
		m_nMap = ap.Z();
		UpdateData(FALSE);

		Revert(false);
	}
}
