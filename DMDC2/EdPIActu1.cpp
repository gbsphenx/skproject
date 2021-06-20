// EdPIActu1.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdPIActu1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdPIActu1Page プロパティ ページ

IMPLEMENT_DYNCREATE(CEdPIActu1Page, CEdPIDlg)

CEdPIActu1Page::CEdPIActu1Page() : CEdPIDlg(CEdPIActu1Page::IDD)
{
	//{{AFX_DATA_INIT(CEdPIActu1Page)
	m_iCreature = -1;
	m_iHero = -1;
	m_iItemName = -1;
	m_iItemType = -1;
	m_iMissile = -1;
	m_iSel = -1;
	m_nTimes = 0;
	m_nLo = 0;
	m_nHi = 0;
	//}}AFX_DATA_INIT
}

void CEdPIActu1Page::DoDataExchange(CDataExchange* pDX)
{
	CEdPIDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdPIActu1Page)
	DDX_Control(pDX, IDC_COMBO_ITEM_NAME, m_wndItemName);
	DDX_Control(pDX, IDC_COMBO_HERO, m_wndHero);
	DDX_Control(pDX, IDC_COMBO_CREATURE, m_wndCreature);
	DDX_Control(pDX, IDC_COMBO_MISSILE, m_wndMissile);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_wndSel);
	DDX_Control(pDX, IDC_SPIN_N, m_wndTimes);
	DDX_Control(pDX, IDC_SPIN_LOARG, m_wndLoArg);
	DDX_Control(pDX, IDC_SPIN_HIARG, m_wndHiArg);
	DDX_CBIndex(pDX, IDC_COMBO_CREATURE, m_iCreature);
	DDX_CBIndex(pDX, IDC_COMBO_HERO, m_iHero);
	DDX_CBIndex(pDX, IDC_COMBO_ITEM_NAME, m_iItemName);
	DDX_CBIndex(pDX, IDC_COMBO_ITEM_TYPE, m_iItemType);
	DDX_CBIndex(pDX, IDC_COMBO_MISSILE, m_iMissile);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_iSel);
	DDX_Text(pDX, IDC_EDIT_N, m_nTimes);
	DDX_Text(pDX, IDC_EDIT_LOARG, m_nLo);
	DDX_Text(pDX, IDC_EDIT_HIARG, m_nHi);
	DDV_MinMaxUInt(pDX, m_nTimes, 0, 511);
	DDV_MinMaxUInt(pDX, m_nLo, 0, 15);
	DDV_MinMaxUInt(pDX, m_nHi, 0, 31);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEdPIActu1Page, CEdPIDlg)
	//{{AFX_MSG_MAP(CEdPIActu1Page)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM_TYPE, OnSelchangeComboItemType)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdPIActu1Page メッセージ ハンドラ

bool CEdPIActu1Page::RevertRecord(DMDBU &rec, bool fRevert)
{
	if (fRevert) {
		m_iSel = (rec.r3.w2) & 127;

		UpdateRecord(rec);
	} else {
		ModifySetVal(rec.r3.w2, 0x007F, m_iSel & 127);

		UINT x = 0;

		switch (m_a1t) {
		case a1tItem:
			{
				if (m_iItemType == 4) {
					x = 511;
				} else {
					x = (m_iItemType & 3) * 0x80 + (m_iItemName & 127);
				}
				ModifySetVal(rec.r3.w2, 0xFF80, x*0x80);

				break;
			}
		case a1tN:
			{
				x = m_nTimes;

				ModifySetVal(rec.r3.w2, 0xFF80, x*0x80);
				break;
			}
		case a1tMissile:
			{
				x = m_iMissile & 63;

				ModifySetVal(rec.r3.w2, 0x0F80, x*0x80);
				break;
			}
		case a1tCreature:
			{
				//x = m_iCreature & 127;
				x = m_iCreature & 255;	// SPX: extension to 255

				ModifySetVal(rec.r3.w2, 0x3F80, x*0x80);
				break;
			}
		case a1tHero:
			{
				x = m_iHero & 127;

				ModifySetVal(rec.r3.w2, 0x3F80, x*0x80);
				break;
			}
		case a1tAIState:
			{
				ModifySetVal(rec.r3.w2, 0x0780, ((m_nLo & 15)     )*0x80);
				ModifySetVal(rec.r3.w2, 0xF800, ((m_nHi & 31) << 4)*0x80);

				break;
			}
		}
	}
	return true;
}

BOOL CEdPIActu1Page::OnInitDialog() 
{
	CEdPIDlg::OnInitDialog();

	int iTileBase = (TileIsWall() ? 128 : 0);
	int i;
	for (i = 0; i < 128; i++) {
		m_wndSel.AddString(CDM2NameDic::GetInstance().GetName(3, i |iTileBase));
	}
	for (i = 0; i < 64; i++) {
		m_wndMissile.AddString(CDM2NameDic::GetInstance().GetDATDBName(0x0D, i));
	}
	for (i = 0; i < 128; i++) {
		m_wndCreature.AddString(CDM2NameDic::GetInstance().GetName(4, i));
		m_wndHero.AddString(CDM2NameDic::GetInstance().GetHeroName(i));
	}

	Active1(-1);

	m_a1t = -1;

	m_wndTimes.SetRange(0, 511);
	m_wndLoArg.SetRange(0, 15);
	m_wndHiArg.SetRange(0, 31);
	
	return FALSE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CEdPIActu1Page::Active1(UINT nID)
{
	CWnd *p = GetDlgItem(IDC_STATIC_ITEM);
	bool fEnabled = false;
	while (p) {
		TCHAR tc[MAX_PATH] = "";
		GetClassName(*p, tc, MAX_PATH);
		if (_tcsicmp(tc, "BUTTON") == 0 && (p->GetStyle() & 0xF) == BS_GROUPBOX) {
			fEnabled = (p->GetDlgCtrlID() == nID);
		}
		p->EnableWindow(fEnabled);

		p = p->GetNextWindow();
	}
}

void CEdPIActu1Page::OnSelchangeComboItemType() 
{
	if (!UpdateData()) return;

	UpdateItemName();
}

void CEdPIActu1Page::UpdateRecord(DMDBU rec)
{
	UINT x = rec.r3.w2 / 0x80;

	const CDM2ActuHint &ah = CDM2NameDic::GetInstance().GetActuHint(m_iSel, TileIsWall());
	m_a1t = ah.a1t;

	switch (m_a1t) {
	case a1tItem:
		{
			if (x == 511) {
				m_iItemType = 4;
				m_iItemName = -1;
			} else {
				m_iItemType = x / 0x80;
				m_iItemName = x & 0x7F;
			}

			Active1(IDC_STATIC_ITEM);
			break;
		}
	case a1tN:
		{
			m_nTimes = x;
			Active1(IDC_STATIC_COUNT);
			break;
		}
	case a1tMissile:
		{
			m_iMissile = x & 63;
			Active1(IDC_STATIC_MISSILE);
			break;
		}
	case a1tCreature:
		{
			//m_iCreature = x & 127;
			m_iCreature = x & 255;	// SPX: extension to 255
			Active1(IDC_STATIC_CREATURE);
			break;
		}
	case a1tHero:
		{
			m_iHero = x & 127;
			Active1(IDC_STATIC_HERO);
			break;
		}
	case a1tAIState:
		{
			m_nLo = x & 15;
			m_nHi = (x >> 4) & 31;
			Active1(IDC_STATIC_CREATURE_AI);
			break;
		}
	default:
		{
			Active1(-1);
			break;
		}
	}

	UpdateItemName();

	UpdateData(FALSE);
}

void CEdPIActu1Page::UpdateState()
{
	DMDBU rec;
	if (GetRecord(rec)) {
		UpdateRecord(rec);
	}
}

void CEdPIActu1Page::OnSelchangeComboType() 
{
	m_iSel = m_wndSel.GetCurSel();

	UpdateState();
}

void CEdPIActu1Page::UpdateItemName()
{
	m_wndItemName.ResetContent();

	if (m_iItemType != 4 && m_iItemType != -1) {
		int t = 0;
		switch (m_iItemType) {
		case 0: t = 5; break;
		case 1: t = 6; break;
		case 2: t = 10; break;
		case 3: t = 8; break;
		}
		for (int i = 0; i < 128; i++) {
			m_wndItemName.AddString(CDM2NameDic::GetInstance().GetName(t, i));
		}
	}
}
