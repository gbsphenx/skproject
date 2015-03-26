// EdPITextPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdPITextPage.h"
#include "DM2TextRec.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdPITextPage プロパティ ページ

IMPLEMENT_DYNCREATE(CEdPITextPage, CEdPIDlg)

CEdPITextPage::CEdPITextPage() : CEdPIDlg(CEdPITextPage::IDD)
{
	//{{AFX_DATA_INIT(CEdPITextPage)
	m_f0 = FALSE;
	m_f1 = FALSE;
	m_fB = FALSE;
	m_fC = FALSE;
	m_fD = FALSE;
	m_fE = FALSE;
	m_fF = FALSE;
	m_f2 = FALSE;
	m_iMode = -1;
	//}}AFX_DATA_INIT

	m_iSel = -1;
}

void CEdPITextPage::DoDataExchange(CDataExchange* pDX)
{
	CEdPIDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdPITextPage)
	DDX_Control(pDX, IDC_COMBO_USE, m_wndUse);
	DDX_Control(pDX, IDC_STATIC_ORNATE_PREVIEW, m_wndOrnatePvw);
	DDX_Control(pDX, IDC_COMBO_TEXT, m_wndSel);
	DDX_Check(pDX, IDC_CHECK_N0, m_f0);
	DDX_Check(pDX, IDC_CHECK_N1, m_f1);
	DDX_Check(pDX, IDC_CHECK_N11, m_fB);
	DDX_Check(pDX, IDC_CHECK_N12, m_fC);
	DDX_Check(pDX, IDC_CHECK_N13, m_fD);
	DDX_Check(pDX, IDC_CHECK_N14, m_fE);
	DDX_Check(pDX, IDC_CHECK_N15, m_fF);
	DDX_Check(pDX, IDC_CHECK_N2, m_f2);
	DDX_Radio(pDX, IDC_RADIOM1, m_iMode);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEdPITextPage, CEdPIDlg)
	//{{AFX_MSG_MAP(CEdPITextPage)
	ON_CBN_SELCHANGE(IDC_COMBO_TEXT, OnSelchangeComboText)
	ON_CBN_SELCHANGE(IDC_COMBO_USE, OnSelchangeComboUse)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_RADIOM1, IDC_RADIOM2, OnRadioM1)
	ON_COMMAND_RANGE(IDC_CHECK_N0, IDC_CHECK_N15, OnCheckN15)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdPITextPage メッセージ ハンドラ

bool CEdPITextPage::RevertRecord(DMDBU &rec, bool fRevert)
{
	if (fRevert) {
		m_f0 = (rec.r2.w2 & 0x0001) ? 1 : 0;
		m_f1 = (rec.r2.w2 & 0x0002) ? 1 : 0;
		m_f2 = (rec.r2.w2 & 0x0004) ? 1 : 0;
		m_fB = (rec.r2.w2 & 0x0800) ? 1 : 0;
		m_fC = (rec.r2.w2 & 0x1000) ? 1 : 0;
		m_fD = (rec.r2.w2 & 0x2000) ? 1 : 0;
		m_fE = (rec.r2.w2 & 0x4000) ? 1 : 0;
		m_fF = (rec.r2.w2 & 0x8000) ? 1 : 0;
		m_iSel = (rec.r2.w2 / 0x0008) & 255;
		m_iMode = (m_f1 ? 0 : 1);

		ApplyText();
		ApplyUseType();
		ApplySel();
		UpdateUseType();
		UpdateLargeType();
		UpdatePoint();
	} else {
		MaskedModify(rec.r2.w2, 0x0001, m_f0);
		MaskedModify(rec.r2.w2, 0x0002, m_f1);
		MaskedModify(rec.r2.w2, 0x0004, m_f2);
		MaskedModify(rec.r2.w2, 0x0800, m_fB);
		MaskedModify(rec.r2.w2, 0x1000, m_fC);
		MaskedModify(rec.r2.w2, 0x2000, m_fD);
		MaskedModify(rec.r2.w2, 0x4000, m_fE);
		MaskedModify(rec.r2.w2, 0x8000, m_fF);
		ModifySetVal(rec.r2.w2, 0x07F8, 0x0008 * (m_iSel & 255));
	}
	return true;
}

BOOL CEdPITextPage::OnInitDialog() 
{
	CEdPIDlg::OnInitDialog();
	
	{
		static const struct Table {
			int mask;
			LPCSTR psz;
		} tbl[] = {
		//	0x0001,"Text message (DUNGEON.dat)",
			0x7000,"Text message (GRAPHICS.dat)",
			0x0000,"Still image (An alcove)",
			0x1000,"Animate image (Water basin)",
			0x2000,"Holder (Torch holder)",
			0x3000,"Ladder enabled",
			0x4000,"Recharge (Blue gem crop)",
			0x9800,"Creature spawn on trigger",
			0x6800,"2-state switch (Window)",
			0x5000,"Bush",
			0x3800,"Ladder disabled",
			0x8000,"Teleporter with flag",
			0x7800,"Teleporter",

			0x0800,"Creature transformer",
			-1,NULL,
		};

		for (int x = 0; tbl[x].psz != NULL; x++) {
			int mask = tbl[x].mask;

			int r = m_wndUse.AddString(tbl[x].psz);
			m_wndUse.SetItemData(r, mask);
		}

		for (int t = 0; t<32; t++) {
			int o = 0
				|((t & 0x01) ? 0x0800 : 0)
				|((t & 0x02) ? 0x1000 : 0)
				|((t & 0x04) ? 0x2000 : 0)
				|((t & 0x08) ? 0x4000 : 0)
				|((t & 0x10) ? 0x8000 : 0)
				;
			CString text;
			int r;
			switch (o) {
			case 0x7000:
			case 0x0000:
			case 0x1000:
			case 0x2000:
			case 0x3000:
			case 0x4000:
			case 0x9800:
			case 0x6800:
			case 0x5000:
			case 0x3800:

			case 0x0800:
				break;
			default:
				text.Format("x %04X", o);
				r = m_wndUse.AddString(text);
				m_wndUse.SetItemData(r, o);
			}
		}
	}

	m_wndSel.SetCount(256);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CEdPITextPage::OnSelchangeComboText() 
{
	if (!UpdateData())
		return;

	m_iSel = m_wndSel.GetIntData(m_wndSel.GetCurSel());

	switch (m_trpt) {
	case trptTexti:
		m_fB = (m_iSel & 0x0100) ? 1 : 0;
		m_fC = (m_iSel & 0x0200) ? 1 : 0;
		m_fD = (m_iSel & 0x0400) ? 1 : 0;
		m_fE = (m_iSel & 0x0800) ? 1 : 0;
		m_fF = (m_iSel & 0x1000) ? 1 : 0;
		UpdateData(false);
		break;
	}

	ApplyText();
}

void CEdPITextPage::ApplyText()
{
	DMDBU rec;
	RevertRecord(rec, false);

	int k1 = -1;

	DM2TextRec_t tr;
	tr.Decode(rec.r2.w2);

	switch (tr.trpt) {
	case trptCreature: k1 = 0x0F; break;
	case trptOrnate:   k1 = TileIsWall() ? 0x09 : 0x0A; break;
	}

	m_wndOrnatePvw.SetOrnate_wc(tr.index, k1);
}

void CEdPITextPage::OnSelchangeComboUse() 
{
	struct Table {
		UINT mask;
		BOOL *f;
	} tbl[] = {
	//	0x0001,&m_f0,
	//	0x0002,&m_f1,
	//	0x0004,&m_f2,
		0x0800,&m_fB,
		0x1000,&m_fC,
		0x2000,&m_fD,
		0x4000,&m_fE,
		0x8000,&m_fF,
		0,0,
	};

	if (!UpdateData())
		return;

	int r = m_wndUse.GetCurSel();
	if (r != -1) {
		DWORD xxx = m_wndUse.GetItemData(r);

		for (int x=0; tbl[x].mask != 0; x++) {
			*tbl[x].f = (xxx & tbl[x].mask) ? true : false;
		}
	}

	UpdateData(false);

	ApplyText();
	ApplyUseType();
}

void CEdPITextPage::OnCheckN15(UINT nID) 
{
	if (!UpdateData())
		return;

	UpdatePoint();
	UpdateUseType();
	UpdateLargeType();
}

void CEdPITextPage::UpdateUseType()
{
	struct Table {
		UINT mask;
		BOOL f;
	} tbl[] = {
	//	0x0001,m_f0,
	//	0x0002,m_f1,
	//	0x0004,m_f2,
		0x0800,m_fB,
		0x1000,m_fC,
		0x2000,m_fD,
		0x4000,m_fE,
		0x8000,m_fF,
		0,0,
	};

	LPARAM lParam = 0;

	for (int x=0; tbl[x].mask != 0; x++) {
		if (tbl[x].f)
			lParam |= tbl[x].mask;
	}

	{
		int cx = m_wndUse.GetCount();
		for (int x=0; x<cx; x++) {
			if (m_wndUse.GetItemData(x) == lParam) {
				m_wndUse.SetCurSel(x);
				break;
			}
		}
	}
}

void CEdPITextPage::UpdatePoint()
{

}

void CEdPITextPage::UpdateLargeType()
{
	ValidateLargeType();
}

void CEdPITextPage::ValidateLargeType()
{
	int xxx = 0 + (m_f1 ? 2 : 0) + (m_f2 ? 4 : 0);

	bool fUse = (xxx == 2);
	bool fComplex = (xxx & 4) != 0;

	static const UINT nID_Text[] = {
		IDC_STATIC_TEXT,
		IDC_COMBO_TEXT,
		0,
	};
	static const UINT nID_Use[] = {
		IDC_STATIC_USE_TYPE,
		IDC_COMBO_USE,
		IDC_CHECK_N11,
		IDC_CHECK_N12,
		IDC_CHECK_N13,
		IDC_CHECK_N14,
		IDC_CHECK_N15,
		0,
	};
	static const UINT nID_Mode[] = {
		IDC_CHECK_N0,
		IDC_RADIOM1,
		IDC_RADIOM2,
		0,
	};

	enableWindows(nID_Text, !fComplex);
	enableWindows(nID_Use, !fComplex && fUse);
	enableWindows(nID_Mode, !fComplex);
}

void CEdPITextPage::enableWindows(const UINT *nIDs, bool fEnabled)
{
	for (int x=0; nIDs[x] != 0; x++) {
		CWnd *win = GetDlgItem(nIDs[x]);
		if (win == NULL) continue;

		if (win->IsWindowEnabled() != (BOOL)fEnabled) {
			win->EnableWindow(fEnabled);
		}
	}
}

void CEdPITextPage::OnRadioM1(UINT nID) 
{
	if (!UpdateData())
		return;

	m_f1 = (nID == IDC_RADIOM1);

	UpdateData(false);

	ApplyUseType();
	ValidateLargeType();
}

void CEdPITextPage::ApplyUseType()
{
	DM2NameResolver_t *pNameRes = GetNameRes();
	if (pNameRes == NULL) {
		m_wndSel.Reset();
		return;
	}

	DMDBU rec;
	RevertRecord(rec, false);

	DM2TextRec_t tr;
	tr.Decode(rec.r2.w2);

	switch (m_trpt = tr.trpt) {
	case trptTexti:
		{
			CWordArray vec;
			pNameRes->texte.EnumKey(vec);

			const int cx = 256, cxVec = vec.GetSize();

			for (int x=0; x<cx; x++) {
				CExtComboBox::DATA &ai = m_wndSel.ai[x];
				ai.suffix.Empty();
				if (x < cxVec) {
					ai.lp = vec[x];
					ai.text = pNameRes->GetInttextStr(vec[x]);
				} else {
					ai.lp = 0;
					ai.text = "";
				}
			}

			if (m_fB) m_iSel |= 0x0100;
			if (m_fC) m_iSel |= 0x0200;
			if (m_fD) m_iSel |= 0x0400;
			if (m_fE) m_iSel |= 0x0800;
			if (m_fF) m_iSel |= 0x1000;
			break;
		}
	case trptTextg:
	case trptCreature:
	case trptOrnate:
		{
			int cx = 256;
			CString text;
			for (int x=0; x<cx; x++) {
				switch (tr.trpt) {
				default:
					ASSERT(false);
					break;
				case trptTextg:
					text = pNameRes->GetExttextStr(x);
					break;
				case trptCreature:
					text = CDM2NameDic::GetInstance().GetCreatureName(x);
					break;
				case trptOrnate:
					text = CDM2NameDic::GetInstance().GetOrnateName(x, TileIsWall());
					break;
				}

				text.Replace("\r\n", "\n");
				text.Replace('\r', '/');
				text.Replace('\n', '/');

				m_wndSel.ai[x].text = text;
				m_wndSel.ai[x].suffix.Format(" (%02X)", x);
				m_wndSel.ai[x].lp = x;
			}
			m_wndSel.Sort();
			break;
		}
	default:
		{
			m_wndSel.Reset();
			break;
		}
	}

	ApplySel();

	m_wndSel.Invalidate();
}

void CEdPITextPage::ApplySel()
{
	m_wndSel.SetCurSel(m_wndSel.FindIntData(m_iSel));
}
