// EditLvDesc3Dlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EditLvDesc3Dlg.h"
#include "DM2SelOrnateImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditLvDesc3Dlg ダイアログ


CEditLvDesc3Dlg::CEditLvDesc3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditLvDesc3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditLvDesc3Dlg)
	m_iDO = -1;
	m_iDS = -1;
	m_iFO = -1;
	m_iWO = -1;
	m_iScene = -1;
	m_cy = 0;
	m_cx = 0;
	m_shiftX = 0;
	m_shiftY = 0;
	m_iCS = -1;
	m_f1 = FALSE;
	m_f2 = FALSE;
	m_f3 = FALSE;
	m_f4 = FALSE;
	m_f5 = FALSE;
	m_nAltitude = 0;
	m_nDifficulty = 0;
	m_iDoorSel = -1;
	//}}AFX_DATA_INIT

	m_iWO = 0;
	m_iFO = 0;
	m_iDO = 0;
	m_iCS = 0;
	m_iDoorSel = 0;
}


void CEditLvDesc3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditLvDesc3Dlg)
	DDX_Control(pDX, IDC_SPIN_DEPTH, m_wndDepth);
	DDX_Control(pDX, IDC_SPIN_DIFFICULTY, m_wndDifficulty);
	DDX_Control(pDX, IDC_SPIN_HEIGHT, m_wndHeight);
	DDX_Control(pDX, IDC_SPIN_WIDTH, m_wndWidth);
	DDX_Control(pDX, IDC_SPIN_Y, m_wndY);
	DDX_Control(pDX, IDC_SPIN_X, m_wndX);
	DDX_Control(pDX, IDC_COMBO_CS, m_wndCS);
	DDX_Control(pDX, IDC_COMBO_WO, m_wndWO);
	DDX_Control(pDX, IDC_COMBO_FO, m_wndFO);
	DDX_Control(pDX, IDC_COMBO_DS, m_wndDS);
	DDX_Control(pDX, IDC_COMBO_DO, m_wndDO);
	DDX_CBIndex(pDX, IDC_COMBO_DO, m_iDO);
	DDX_CBIndex(pDX, IDC_COMBO_DS, m_iDS);
	DDX_CBIndex(pDX, IDC_COMBO_FO, m_iFO);
	DDX_CBIndex(pDX, IDC_COMBO_WO, m_iWO);
	DDX_CBIndex(pDX, IDC_COMBO_SCENE, m_iScene);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_cy);
	DDV_MinMaxUInt(pDX, m_cy, 1, 32);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_cx);
	DDV_MinMaxUInt(pDX, m_cx, 1, 32);
	DDX_Text(pDX, IDC_EDIT_X, m_shiftX);
	DDV_MinMaxUInt(pDX, m_shiftX, 0, 255);
	DDX_Text(pDX, IDC_EDIT_Y, m_shiftY);
	DDV_MinMaxUInt(pDX, m_shiftY, 0, 255);
	DDX_CBIndex(pDX, IDC_COMBO_CS, m_iCS);
	DDX_Check(pDX, IDC_CHECK_V1, m_f1);
	DDX_Check(pDX, IDC_CHECK_V2, m_f2);
	DDX_Check(pDX, IDC_CHECK_V3, m_f3);
	DDX_Check(pDX, IDC_CHECK_V4, m_f4);
	DDX_Check(pDX, IDC_CHECK_V5, m_f5);
	DDX_Text(pDX, IDC_EDIT_ALTITUDE, m_nAltitude);
	DDV_MinMaxUInt(pDX, m_nAltitude, 0, 63);
	DDX_Text(pDX, IDC_EDIT_DIFFICULTY, m_nDifficulty);
	DDV_MinMaxUInt(pDX, m_nDifficulty, 0, 15);
	DDX_Radio(pDX, IDC_RADIO_DOOR1, m_iDoorSel);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEditLvDesc3Dlg, CDialog)
	//{{AFX_MSG_MAP(CEditLvDesc3Dlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DS, OnSelchangeComboDS)
	ON_CBN_SELCHANGE(IDC_COMBO_DO, OnSelchangeComboDo)
	ON_CBN_SELCHANGE(IDC_COMBO_FO, OnSelchangeComboFo)
	ON_CBN_SELCHANGE(IDC_COMBO_WO, OnSelchangeComboWo)
	ON_CBN_SELCHANGE(IDC_COMBO_CS, OnSelchangeComboCs)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE_WO, OnButtonReplaceWo)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE_FO, OnButtonReplaceFo)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE_DO, OnButtonReplaceDo)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE_CS, OnButtonReplaceCs)
	ON_BN_CLICKED(IDC_BUTTON_RT_WO, OnButtonRtWo)
	ON_BN_CLICKED(IDC_BUTTON_RT_FO, OnButtonRtFo)
	ON_BN_CLICKED(IDC_BUTTON_RT_DO, OnButtonRtDo)
	ON_BN_CLICKED(IDC_BUTTON_RT_CS, OnButtonRtCs)
	ON_BN_CLICKED(IDC_RADIO_DOOR1, OnRadioDoor1)
	ON_BN_CLICKED(IDC_RADIO_DOOR2, OnRadioDoor1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditLvDesc3Dlg メッセージ ハンドラ

BOOL CEditLvDesc3Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CtlRevert(true);
	UpdateData(FALSE);

	m_wndX.SetRange(0, 255);
	m_wndY.SetRange(0, 255);
	m_wndWidth.SetRange(1, 32);
	m_wndHeight.SetRange(1, 32);
	m_wndDepth.SetRange(0, 63);
	m_wndDifficulty.SetRange(0, 15);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

bool CEditLvDesc3Dlg::CtlRevert(bool fRevert)
{
	if (fRevert) {
		m_wndWO.Revert(m_oim.m_wall, true);
		m_wndWO.SelOrnate(true);
		m_wndWO.Freshen();

		m_wndFO.Revert(m_oim.m_floor, true);
		m_wndFO.SelOrnate(false);
		m_wndFO.Freshen();

		m_wndDO.Revert(m_oim.m_door, true);
		m_wndDO.SelDoorOrnate();
		m_wndDO.Freshen();

		m_wndCS.Revert(m_oim.m_mons, true);
		m_wndCS.SelCreature();
		m_wndCS.Freshen();

		m_wndDS.AllDoor();
		m_wndDS.Freshen();

		m_iDS = m_oim.m_iDoor[m_iDoorSel ? 1 : 0] +1;
	} else {

	}
	return true;
}

bool CEditLvDesc3Dlg::Revert(LevelDesc_t &s, bool fRevert)
{
	if (fRevert) {
		m_oim.LoadFrom(s);

		m_shiftX = s.nShiftX;
		m_shiftY = s.nShiftY;
		m_cx = s.cyLv;
		m_cy = s.cxLv;
		m_nAltitude = s.iLv;
		m_nDifficulty = s.n12_C_F;
		m_iScene = s.n14_4_7;
		m_f1 = (s.w2 & 0x0001) ? 1 : 0;
		m_f2 = (s.w2 & 0x0002) ? 1 : 0;
		m_f3 = (s.w2 & 0x0008) ? 1 : 0;
		m_f4 = (s.w2 & 0x0010) ? 1 : 0;
		m_f5 = (s.w2 & 0x0020) ? 1 : 0;
	} else {
		m_oim.SaveTo(s);

		s.nShiftX = m_shiftX;
		s.nShiftY = m_shiftY;
		s.cxLv = m_cy;
		s.cyLv = m_cx;
		s.iLv = m_nAltitude;
		s.n12_C_F = m_nDifficulty;
		s.n14_4_7 = m_iScene;
		ModifyBit(s.w2, 0x0001, m_f1);
		ModifyBit(s.w2, 0x0002, m_f2);
		ModifyBit(s.w2, 0x0008, m_f3);
		ModifyBit(s.w2, 0x0010, m_f4);
		ModifyBit(s.w2, 0x0020, m_f5);
	}
	return true;
}

void CEditLvDesc3Dlg::OnSelchangeComboDS() 
{
	if (!UpdateData()) return;

	(int &)(m_oim.m_iDoor[m_iDoorSel ? 1 : 0]) = m_iDS -1;
}

void CEditLvDesc3Dlg::OnSelchangeComboDo() { OnSel(m_wndDO, 0x0B, m_oim.m_door, m_iDO); }
void CEditLvDesc3Dlg::OnSelchangeComboFo() { OnSel(m_wndFO, 0x0A, m_oim.m_floor, m_iFO); }
void CEditLvDesc3Dlg::OnSelchangeComboWo() { OnSel(m_wndWO, 0x09, m_oim.m_wall, m_iWO); }
void CEditLvDesc3Dlg::OnSelchangeComboCs() { OnSel(m_wndCS, 0x0F, m_oim.m_mons, m_iCS); }

void CEditLvDesc3Dlg::OnButtonReplaceDo() { OnReplace(m_wndDO, 0x0B, m_oim.m_door, m_iDO); }
void CEditLvDesc3Dlg::OnButtonReplaceFo() { OnReplace(m_wndFO, 0x0A, m_oim.m_floor, m_iFO); }
void CEditLvDesc3Dlg::OnButtonReplaceWo() { OnReplace(m_wndWO, 0x09, m_oim.m_wall, m_iWO); }
void CEditLvDesc3Dlg::OnButtonReplaceCs() { OnReplace(m_wndCS, 0x0F, m_oim.m_mons, m_iCS); }

void CEditLvDesc3Dlg::OnButtonRtDo() { OnRT(m_wndDO, m_oim.m_door, m_iDO); }
void CEditLvDesc3Dlg::OnButtonRtFo() { OnRT(m_wndFO, m_oim.m_floor, m_iFO); }
void CEditLvDesc3Dlg::OnButtonRtWo() { OnRT(m_wndWO, m_oim.m_wall, m_iWO); }
void CEditLvDesc3Dlg::OnButtonRtCs() { OnRT(m_wndCS, m_oim.m_mons, m_iCS); }

void CEditLvDesc3Dlg::OnSel(CDM2Ornate1ComboBox &wndBox, int f, CDM2OrnateIdxArray &v, int &i)
{
	int iOld = i;

	if (!UpdateData()) return;

	if (wndBox.IsAddNew1(i)) {
		CDM2SelOrnateImageDlg wndDlg(f, this);
		if (IDOK == wndDlg.DoModal()) {
			int iNew = v.Add(wndDlg.m_iOrnate);
			if (iNew < 0) iNew = 0; else iNew++;
			i = iNew;

			wndBox.Revert(v, true);
			wndBox.Freshen();
		} else {
			i = iOld;
		}
		UpdateData(FALSE);
	}
}

void CEditLvDesc3Dlg::OnReplace(CDM2Ornate1ComboBox &wndBox, int f, CDM2OrnateIdxArray &v, int &i)
{
	if (!UpdateData()) return;

	int x = wndBox.GetOrnateIdx(i);
	if (x < 0) {

	} else {
		bool fDoor = (f & 1) ? true : false;
		bool fWall = (f & 2) ? true : false;
		CDM2SelOrnateImageDlg wndDlg(f, this);
		wndDlg.m_iOrnate = v[x];
		if (IDOK == wndDlg.DoModal()) {
			v[x] = wndDlg.m_iOrnate;

			wndBox.Revert(v, true);
			wndBox.Freshen();
		}
	}
}

void CEditLvDesc3Dlg::OnRT(CDM2Ornate1ComboBox &wndBox, CDM2OrnateIdxArray &v, int &i)
{
	v.RemoveTail();

	wndBox.Revert(v, true);
	wndBox.Freshen();

	if (!UpdateData()) return;

	if (wndBox.GetOrnateIdx(i) < 0) {
		i = 0;

		UpdateData(FALSE);
	}
}

void CEditLvDesc3Dlg::OnRadioDoor1() 
{
	BOOL fDoorType = m_iDoorSel ? 1 : 0;

	if (!UpdateData())
		return;

	(int &)(m_oim.m_iDoor[fDoorType ? 1 : 0]) = m_iDS -1;

	m_iDS = m_oim.m_iDoor[m_iDoorSel ? 1 : 0] +1;

	UpdateData(FALSE);
}
