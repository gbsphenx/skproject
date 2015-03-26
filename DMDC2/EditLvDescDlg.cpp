// EditLvDescDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EditLvDescDlg.h"
#include "HexEditFrame.h"
#include "HexEditView.h"
#include "NamedWndBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditLvDescDlg ダイアログ

CEditLvDescDlg::CEditLvDescDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditLvDescDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditLvDescDlg)
	m_str10_0_3 = _T("");
	m_str10_8_B = _T("");
	m_iAltitude = 0;
	m_cxMap = 0;
	m_cyMap = 0;
	m_strMonsters = _T("");
	m_xShift = 0;
	m_yShift = 0;
	m_w2 = 0;
	m_w4 = 0;
	m_strWallDeco = _T("");
	m_n10_4_7 = 0;
	m_n10_C_F = 0;
	m_n12_8_B = 0;
	m_n12_C_F = 0;
	m_n14_0_3 = 0;
	m_n14_4_7 = 0;
	m_n14_8_B = 0;
	m_n14_C_F = 0;
	//}}AFX_DATA_INIT
}

void CEditLvDescDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

//	ArrayTrans(pDX, IDC_EDIT_10_0_3, m_str10_0_3, m_arr10_0_3, FALSE);
//	ArrayTrans(pDX, IDC_EDIT_10_8_B, m_str10_8_B, m_arr10_8_B, FALSE);
//	ArrayTrans(pDX, IDC_EDIT_MONSTERS, m_strMonsters, m_arrMonsters, FALSE);
//	ArrayTrans(pDX, IDC_EDIT_WALL_DECO, m_strWallDeco, m_arrWallDeco, FALSE);
	//{{AFX_DATA_MAP(CEditLvDescDlg)
	DDX_Text(pDX, IDC_EDIT_10_0_3, m_str10_0_3);
	DDX_Text(pDX, IDC_EDIT_10_8_B, m_str10_8_B);
	DDX_Text(pDX, IDC_EDIT_ALTITUDE, m_iAltitude);
	DDV_MinMaxUInt(pDX, m_iAltitude, 0, 63);
	DDX_Text(pDX, IDC_EDIT_MAP_HEIGHT, m_cxMap);
	DDV_MinMaxUInt(pDX, m_cxMap, 1, 32);
	DDX_Text(pDX, IDC_EDIT_MAP_WIDTH, m_cyMap);
	DDV_MinMaxUInt(pDX, m_cyMap, 1, 32);
	DDX_Text(pDX, IDC_EDIT_MONSTERS, m_strMonsters);
	DDX_Text(pDX, IDC_EDIT_SHIFTX, m_xShift);
	DDV_MinMaxUInt(pDX, m_xShift, 0, 255);
	DDX_Text(pDX, IDC_EDIT_SHIFTY, m_yShift);
	DDV_MinMaxUInt(pDX, m_yShift, 0, 255);
	DDX_Text(pDX, IDC_EDIT_W2, m_w2);
	DDV_MinMaxUInt(pDX, m_w2, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_W4, m_w4);
	DDV_MinMaxUInt(pDX, m_w4, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_WALL_DECO, m_strWallDeco);
	DDX_Text(pDX, IDC_EDIT_10_4_7, m_n10_4_7);
	DDV_MinMaxUInt(pDX, m_n10_4_7, 0, 15);
	DDX_Text(pDX, IDC_EDIT_10_C_F, m_n10_C_F);
	DDV_MinMaxUInt(pDX, m_n10_C_F, 0, 15);
	DDX_Text(pDX, IDC_EDIT_12_8_B, m_n12_8_B);
	DDV_MinMaxUInt(pDX, m_n12_8_B, 0, 15);
	DDX_Text(pDX, IDC_EDIT_12_C_F, m_n12_C_F);
	DDV_MinMaxUInt(pDX, m_n12_C_F, 0, 15);
	DDX_Text(pDX, IDC_EDIT_14_0_3, m_n14_0_3);
	DDV_MinMaxUInt(pDX, m_n14_0_3, 0, 15);
	DDX_Text(pDX, IDC_EDIT_14_4_7, m_n14_4_7);
	DDV_MinMaxUInt(pDX, m_n14_4_7, 0, 15);
	DDX_Text(pDX, IDC_EDIT_14_8_B, m_n14_8_B);
	DDV_MinMaxUInt(pDX, m_n14_8_B, 0, 15);
	DDX_Text(pDX, IDC_EDIT_14_C_F, m_n14_C_F);
	DDV_MinMaxUInt(pDX, m_n14_C_F, 0, 15);
	//}}AFX_DATA_MAP
	if (!pDX->m_bSaveAndValidate) {
		ShowHex(IDC_EDIT_10_0_3, m_arr10_0_3);
		ShowHex(IDC_EDIT_10_8_B, m_arr10_8_B);
		ShowHex(IDC_EDIT_MONSTERS, m_arrMonsters);
		ShowHex(IDC_EDIT_WALL_DECO, m_arrWallDeco);
	}
//	ArrayTrans(pDX, IDC_EDIT_10_0_3, m_str10_0_3, m_arr10_0_3, TRUE);
//	ArrayTrans(pDX, IDC_EDIT_10_8_B, m_str10_8_B, m_arr10_8_B, TRUE);
//	ArrayTrans(pDX, IDC_EDIT_MONSTERS, m_strMonsters, m_arrMonsters, TRUE);
//	ArrayTrans(pDX, IDC_EDIT_WALL_DECO, m_strWallDeco, m_arrWallDeco, TRUE);
}

BEGIN_MESSAGE_MAP(CEditLvDescDlg, CDialog)
	//{{AFX_MSG_MAP(CEditLvDescDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditLvDescDlg メッセージ ハンドラ

void CEditLvDescDlg::ArrayTrans(CDataExchange *pDX, UINT nID, CString &tstr, CByteArray &arr, BOOL bSaveToArray)
{
	if (bSaveToArray && !pDX->m_bSaveAndValidate)
		return;
	if (!bSaveToArray && pDX->m_bSaveAndValidate)
		return;
	pDX->PrepareEditCtrl(nID);
	if (bSaveToArray) {
		LPCTSTR lpszPos = tstr;
		LPCTSTR lpszEnd = _tcschr(lpszPos, 0);
		arr.RemoveAll();
		while (lpszPos+2 <= lpszEnd) {
			if (!isxdigit((BYTE)lpszPos[0]) || !isxdigit((BYTE)lpszPos[1]) || (lpszPos[2] != ' ' && lpszPos[2] != 0)) {
				AfxMessageBox(IDP_PARSE_XDIGIT);
				pDX->Fail();
			}
			LPTSTR lpszFin;
			UINT i = (BYTE)_tcstol(lpszPos, &lpszFin, 16);
			arr.Add(i);
			lpszPos += 3;
		}
	} else {
		LPTSTR lpszTo = tstr.GetBuffer(arr.GetSize() * 3);
		const BYTE *pbPos = arr.GetData();
		const BYTE *pbEnd = pbPos + arr.GetSize();
		static char tbl[] = {"0123456789ABCDEF"};
		int i = 0;
		while (pbPos != pbEnd) {
			lpszTo[i*3 +0] = tbl[(*pbPos) >> 4];
			lpszTo[i*3 +1] = tbl[(*pbPos) & 15];
			lpszTo[i*3 +2] = ' ';
			pbPos++;
			i++;
		}
		lpszTo[i*3 -1] = 0;
		tstr.ReleaseBuffer();
	}
}

void CEditLvDescDlg::Revert(const LevelDesc_t &s)
{
	m_arr10_0_3.SetSize(s.nWallOrnateIndices); memcpy(m_arr10_0_3.GetData(), s.bWallOrnateIndices, s.nWallOrnateIndices);
	m_arr10_8_B.SetSize(s.nFloorOrnateIndices); memcpy(m_arr10_8_B.GetData(), s.bFloorOrnateIndices, s.nFloorOrnateIndices);
	m_arrMonsters.SetSize(s.nCreatureIndices); memcpy(m_arrMonsters.GetData(), s.bCreatureIndices, s.nCreatureIndices);
	m_arrWallDeco.SetSize(s.nSharedOrnateIndices); memcpy(m_arrWallDeco.GetData(), s.bSharedOrnateIndices, s.nSharedOrnateIndices);

	m_iAltitude = s.iLv;
	m_cxMap = s.cxLv;
	m_cyMap = s.cyLv;
	m_xShift = s.nShiftX;
	m_yShift = s.nShiftY;
	m_w2 = s.w2;
	m_w4 = s.w4;
	m_n10_4_7 = s.n10_4_7;
	m_n10_C_F = s.n10_C_F;
	m_n12_8_B = s.n12_8_B;
	m_n12_C_F = s.n12_C_F;
	m_n14_0_3 = s.n14_0_3;
	m_n14_4_7 = s.n14_4_7;
	m_n14_8_B = s.n14_8_B;
	m_n14_C_F = s.n14_C_F;
}

void CEditLvDescDlg::Commit(LevelDesc_t &s)
{
	s.nWallOrnateIndices = __min(15U, m_arr10_0_3.GetSize());
	s.nFloorOrnateIndices = __min(15U, m_arr10_8_B.GetSize());
	s.nCreatureIndices = __min(15U, m_arrMonsters.GetSize());
	s.nSharedOrnateIndices = __min(15U, m_arrWallDeco.GetSize());

	memcpy(s.bWallOrnateIndices, m_arr10_0_3.GetData(), s.nWallOrnateIndices);
	memcpy(s.bFloorOrnateIndices, m_arr10_8_B.GetData(), s.nFloorOrnateIndices);
	memcpy(s.bCreatureIndices, m_arrMonsters.GetData(), s.nCreatureIndices);
	memcpy(s.bSharedOrnateIndices, m_arrWallDeco.GetData(), s.nSharedOrnateIndices);

	s.iLv = m_iAltitude;
	s.cxLv = m_cxMap;
	s.cyLv = m_cyMap;
	s.nShiftX = m_xShift;
	s.nShiftY = m_yShift;
	s.w2 = m_w2;
	s.w4 = m_w4;
	s.n10_4_7 = m_n10_4_7;
	s.n10_C_F = m_n10_C_F;
	s.n12_8_B = m_n12_8_B;
	s.n12_C_F = m_n12_C_F;
	s.n14_0_3 = m_n14_0_3;
	s.n14_4_7 = m_n14_4_7;
	s.n14_8_B = m_n14_8_B;
	s.n14_C_F = m_n14_C_F;
}

BOOL CEditLvDescDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_F2) {
			CWnd *pActive = GetFocus();
			switch (pActive->GetDlgCtrlID()) {
			case IDC_EDIT_10_0_3:
				TryHexEdit(pActive, "10_0_3", 0x0100, m_arr10_0_3);
				break;
			case IDC_EDIT_10_8_B:
				TryHexEdit(pActive, "10_8_B", 0x0101, m_arr10_8_B);
				break;
			case IDC_EDIT_MONSTERS:
				TryHexEdit(pActive, "MONSTERS", 0x0102, m_arrMonsters);
				break;
			case IDC_EDIT_WALL_DECO:
				TryHexEdit(pActive, "WD", 0x0103, m_arrWallDeco);
				break;
			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CEditLvDescDlg::TryHexEdit(CWnd *pTar, LPCTSTR lpszName, UINT nID, const CByteArray &arr)
{
	CWnd *pExist;
	if (pExist = g_boxWnd.FindNameFromBox("LVDESC", lpszName)) {
		pExist->SetActiveWindow();
		pExist->ShowWindow(SW_RESTORE);
		return;
	}
	CHexEditFrame *pFrame;
	if (!(pFrame = CHexEditFrame::Create1(this)))
		return;
	CHexEditView *pView;
	if (pView = dynamic_cast<CHexEditView *>(pFrame->GetActiveView())) {
		pView->SetBinaryData(arr, this, nID);
	}
	g_boxWnd.Add2Box(pFrame, this, "LVDESC", lpszName);
}

void CEditLvDescDlg::ShowHex(UINT nID, const CByteArray &arr)
{
	TCHAR tcText[51];
	tcText[0] = 0;
	LPTSTR lpszTo = tcText;
	const BYTE *pbPos = arr.GetData();
	const BYTE *pbEnd = pbPos + arr.GetSize();
	static char tbl[] = {"0123456789ABCDEF"};
	int i = 0, n = 16;
	if (n != 0) {
		while (pbPos != pbEnd && n != 0) {
			lpszTo[0] = tbl[(*pbPos) >> 4];
			lpszTo[1] = tbl[(*pbPos) & 15];
			lpszTo[2] = ' ';
			lpszTo += 3; pbPos++;
			i++; n--;
		}
		if (pbPos != pbEnd) {
			lpszTo[-1] = lpszTo[0] = lpszTo[1] = '.'; lpszTo[2] = 0;
			lpszTo += 4;
		} else {
			lpszTo[-1] = 0;
		}
	}
	SetDlgItemText(nID, tcText);
}

void CEditLvDescDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	g_boxWnd.DeleteAffliationAndReleaseFromBox("LVDESC");
}

void CEditLvDescDlg::OnApplyHexEditCB(DWORD nCookie)
{
	if (!UpdateData())
		return;
	LPCTSTR lpszName = NULL;
	CByteArray *parr = NULL;
	switch (nCookie) {
	case 0x0100:
		lpszName = "10_0_3";
		parr = &m_arr10_0_3;
		break;
	case 0x0101:
		lpszName = "10_8_B";
		parr = &m_arr10_8_B;
		break;
	case 0x0102:
		lpszName = "MONSTERS";
		parr = &m_arrMonsters;
		break;
	case 0x0103:
		lpszName = "WD";
		parr = &m_arrWallDeco;
		break;
	}
	CHexEditFrame *pFrame = static_cast<CHexEditFrame *>(g_boxWnd.FindNameFromBox("LVDESC", lpszName));
	if (pFrame) {
		CHexEditView *pView = static_cast<CHexEditView* >(pFrame->GetActiveView());
		if (pView) {
			pView->GetBinaryData(*parr);
			UpdateData(FALSE);
		}
	}
}
