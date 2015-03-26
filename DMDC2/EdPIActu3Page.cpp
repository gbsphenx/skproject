// EdPIActu3Page.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdPIActu3Page.h"
#include "DMTeleLocFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdPIActu3Page プロパティ ページ

IMPLEMENT_DYNCREATE(CEdPIActu3Page, CEdPIDlg)

CEdPIActu3Page::CEdPIActu3Page() : CEdPIDlg(CEdPIActu3Page::IDD)
{
	//{{AFX_DATA_INIT(CEdPIActu3Page)
	m_iMap = 0;
	m_iX = 0;
	m_iY = 0;
	m_nStr = 0;
	m_iDir = -1;
	//}}AFX_DATA_INIT
}

void CEdPIActu3Page::DoDataExchange(CDataExchange* pDX)
{
	CEdPIDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdPIActu3Page)
	DDX_Control(pDX, IDC_SLIDER_STR, m_wndStr);
	DDX_Control(pDX, IDC_STATIC_MAP, m_wndStaticMap);
	DDX_Control(pDX, IDC_EDIT_MAP, m_wndMap);
	DDX_Text(pDX, IDC_EDIT_MAP, m_iMap);
	DDV_MinMaxUInt(pDX, m_iMap, 0, 255);
	DDX_Text(pDX, IDC_EDIT_X, m_iX);
	DDV_MinMaxUInt(pDX, m_iX, 0, 31);
	DDX_Text(pDX, IDC_EDIT_Y, m_iY);
	DDV_MinMaxUInt(pDX, m_iY, 0, 31);
	DDX_Slider(pDX, IDC_SLIDER_STR, m_nStr);
	DDX_CBIndex(pDX, IDC_COMBO_DIR, m_iDir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEdPIActu3Page, CEdPIDlg)
	//{{AFX_MSG_MAP(CEdPIActu3Page)
	ON_BN_CLICKED(IDC_BUTTON_REF, OnButtonRef)
	ON_BN_CLICKED(IDC_BUTTON_SELF, OnButtonSelf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdPIActu3Page メッセージ ハンドラ

bool CEdPIActu3Page::RevertRecord(DMDBU &rec, bool fRevert)
{
	if (fRevert) {
		const CDM2ActuHint &ah = CDM2NameDic::GetInstance().GetActuHint(rec.r3.w2 & 127, TileIsWall());
		m_a1t = ah.a1t;
		UINT w2 = rec.r3.w2;
		UINT w6 = rec.r3.w6;
		m_iDir = 0;
		m_iX = 0;
		m_iY = 0;
		m_iMap = 0;
		m_nStr = 0;
		switch (m_a3t = ah.a3t) {
		case a3tLocator:
			{
				m_iDir = (w6 / 0x10) & 3;
				m_iX = (w6 / 0x0800) & 31;
				m_iY = (w6 / 0x0040) & 31;
				Active1(IDC_STATIC_LOCATOR);
				m_wndMap.EnableWindow(m_a1t == a1tMap);
				m_wndStaticMap.EnableWindow(m_a1t == a1tMap);
				if (m_a1t == a1tMap) {
					m_iMap = (w2 / 0x80) & 63;
				} else {
					AtlasPos_t ap;
					GetTilePos(ap);
					m_iMap = ap.Z();
				}
				break;
			}
		case a3tStr:
			{
				m_nStr = w6 & 4095;
				Active1(IDC_STATIC_STR);
				break;
			}
		default:
			{
				Active1(-1);
				break;
			}
		}
	} else {
		switch (m_a3t) {
		case a3tLocator:
			{
				ModifySetVal(rec.r3.w6, 0x0030, (m_iDir & 3) * 16);
				ModifySetVal(rec.r3.w6, 0xF800, (m_iX & 31) * 0x0800);
				ModifySetVal(rec.r3.w6, 0x07C0, (m_iY & 31) * 0x0040);
				if (m_a1t == a1tMap) {
					ModifySetVal(rec.r3.w2, 0x1F80, (m_iMap & 63) * 0x80);
				}
				break;
			}
		case a3tStr:
			{
				ModifySetVal(rec.r3.w6, 0x0FFF, m_nStr & 4095);
				break;
			}
		}
	}
	return true;
}

void CEdPIActu3Page::Active1(UINT nID)
{
	CWnd *p = GetDlgItem(IDC_STATIC_LOCATOR);
	bool fEnabled = false;
	while (p) {
		TCHAR tc[MAX_PATH] = "";
		GetClassName(*p, tc, MAX_PATH);
		if (_tcsicmp(tc, "BUTTON") == 0 && (p->GetStyle() & 0xF) == BS_GROUPBOX) {
			fEnabled = (p->GetDlgCtrlID() == nID);
		}
		if (!(p->GetExStyle() & WS_EX_TOPMOST)) {
			p->EnableWindow(fEnabled);
		}

		p = p->GetNextWindow();
	}
}

BOOL CEdPIActu3Page::OnInitDialog() 
{
	CEdPIDlg::OnInitDialog();
	
	Active1(-1);

	m_a1t = m_a3t = -1;

	m_wndStr.SetRange(0, 4095);

	for (int i = 0; i < 6; i++) m_wndStr.SetTic(4096 * i / 6);

	m_wndStr.SetPageSize(4096 / 6);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CEdPIActu3Page::OnButtonRef() 
{
	if (!UpdateData()) return;

	CDMTeleLocFrm frm(GetDDIf(), AtlasPos_t(m_iMap, m_iX, m_iY), m_a1t == a1tMap);
	int r = frm.DoModal(this);
	if (r == IDOK) {
		AtlasPos_t ap = frm.m_view.GetAP();
		m_iMap = ap.Z();
		m_iX = ap.X();
		m_iY = ap.Y();
		UpdateData(FALSE);

		Revert(false);
	}
}

void CEdPIActu3Page::OnButtonSelf() 
{
	if (!UpdateData()) return;

	AtlasPos_t ap;
	if (GetTilePos(ap)) {
		m_iX = ap.X();
		m_iY = ap.Y();
		m_iMap = ap.Z();
		UpdateData(FALSE);

		Revert(false);
	}
}
