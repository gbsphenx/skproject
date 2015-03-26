// AddImageDlg2.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "AddImageDlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EM_C1 0x01
#define EM_C2 0x02
#define EM_C3 0x04
#define EM_C4 0x08
#define EM_C5 0x10
#define EM_C6 0x20
#define EM_VAL 0x40
#define EM_TEXT 0x80
#define EM_ADD 0x100
#define EM_EDITDATA 0x200
//#define EM_FLAGS 0x400
//#define EM_MSITEMS_A 0x1000
//#define EM_MSITEMS_C 0x2000
//#define EM_B5353 0x4000
//#define EM_KEYITEM 0x8000
//#define EM_PRIZEITEM 0x10000
//#define EM_SKTEXT 0x20000
//#define EM_ICONANIM 0x40000

namespace
{

void AFXAPI DDX_Text_EntryHex02(CDataExchange* pDX, int nIDC, UINT &value)
{
	if (pDX->m_bSaveAndValidate) {
		HWND hWnd = pDX->PrepareEditCtrl(nIDC);
		TCHAR tc[256] = {""};
		GetWindowText(hWnd, tc, 256);
		if (_stscanf(tc, "%x", &value) != 1) {
			AfxMessageBox(AFX_IDP_PARSE_INT);
			pDX->Fail();
		}
	} else {
		TCHAR tc[256] = {""};
		wsprintf(tc, "%02X", value);
		HWND hWnd = pDX->PrepareEditCtrl(nIDC);
		SetWindowText(hWnd, tc);
	}
}

};

/////////////////////////////////////////////////////////////////////////////
// CAddImageDlg2 ダイアログ

CAddImageDlg2::CAddImageDlg2(CWnd* pParent /*=NULL*/)
	: CAnchorDlg(CAddImageDlg2::IDD, pParent)
	, m_vxxx(NULL)
	, m_wndUsingPg(NULL)
{
	Construction();
}

CAddImageDlg2::CAddImageDlg2(CEditVxxx *vxxx, CWnd* pParent /*=NULL*/)
	: CAnchorDlg(CAddImageDlg2::IDD, pParent)
	, m_vxxx(vxxx)
	, m_wndUsingPg(NULL)
{
	Construction();
}


void CAddImageDlg2::DoDataExchange(CDataExchange* pDX)
{
	CAnchorDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddImageDlg2)
	DDX_Control(pDX, IDOK, m_wndOk);
	DDX_Control(pDX, IDC_EDIT_C6, m_wndC6);
	DDX_Control(pDX, IDC_EDIT_C5, m_wndC5);
	DDX_Control(pDX, IDC_EDIT_C4, m_wndC4);
	DDX_Control(pDX, IDC_EDIT_C3, m_wndC3);
	DDX_Control(pDX, IDC_EDIT_C2, m_wndC2);
	DDX_Control(pDX, IDC_EDIT_C1, m_wndC1);
	DDX_Control(pDX, IDC_TAB_INPLACE, m_wndTab);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
		UINT c6[6];
		DDX_Text_EntryHex02(pDX, IDC_EDIT_C1, c6[0]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT_C2, c6[1]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT_C3, c6[2]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT_C4, c6[3]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT_C5, c6[4]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT_C6, c6[5]);
		m_pos = CGDEntry(c6[0], c6[1], c6[2], c6[3], c6[4], c6[5]);
	} else {
		UINT c6[6] = {
			m_pos.c1(),
			m_pos.c2(),
			m_pos.c3(),
			m_pos.c4(),
			m_pos.c5(),
			m_pos.c6(),
		};
		DDX_Text_EntryHex02(pDX, IDC_EDIT_C1, c6[0]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT_C2, c6[1]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT_C3, c6[2]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT_C4, c6[3]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT_C5, c6[4]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT_C6, c6[5]);
	}
}


BEGIN_MESSAGE_MAP(CAddImageDlg2, CAnchorDlg)
	//{{AFX_MSG_MAP(CAddImageDlg2)
	ON_WM_MOVE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddImageDlg2 メッセージ ハンドラ

void CAddImageDlg2::Construction()
{
	//{{AFX_DATA_INIT(CAddImageDlg2)
	//}}AFX_DATA_INIT

	m_nEntryMask = 0;
	m_nVal = 0;
}


void CAddImageDlg2::InitAddText(CGDEntry pos)
{
	m_pos = pos;
	m_pos.c3(5);
	m_pos.c4(0);
	m_pos.c5(0);
	m_pos.c6(0);

	m_nEntryMask = EM_C4|EM_C5|EM_C6|EM_ADD|EM_TEXT|EM_EDITDATA;

	m_strNewCaption.LoadString(IDS_STR_ADD_TEXT);
}

void CAddImageDlg2::InitAdd0B(CGDEntry pos)
{
	m_pos = pos;
	m_pos.c3(0xB);
	m_pos.c4(0);
	m_pos.c5(0);
	m_pos.c6(0);

	m_nEntryMask = EM_C4|EM_C5|EM_C6|EM_ADD|EM_VAL|EM_EDITDATA;

	m_strNewCaption.LoadString(IDS_STR_ADD_0B);
}

void CAddImageDlg2::InitAdd0C(CGDEntry pos)
{
	InitAdd0B(pos);
}

void CAddImageDlg2::InitEditText(CGDEntry pos, LPCSTR pszText)
{
	m_pos = pos;

	m_strText = pszText;

	m_nEntryMask = EM_TEXT|EM_EDITDATA;

	m_strNewCaption.LoadString(IDS_STR_EDIT_TEXT);
}

void CAddImageDlg2::InitEdit0B(CGDEntry pos, UINT nVal)
{
	m_pos = pos;

	m_nVal = nVal;

	m_nEntryMask = EM_VAL|EM_EDITDATA;

	m_strNewCaption.LoadString(IDS_STR_EDIT_0B);
}

void CAddImageDlg2::InitEdit0C(CGDEntry pos, UINT nVal)
{
	InitEdit0B(pos, nVal);
}

void CAddImageDlg2::InitRename(CGDEntry pos)
{
	m_pos = pos;

	m_nEntryMask = EM_C1|EM_C2|EM_C4|EM_C5|EM_C6;

	m_strNewCaption.LoadString(IDS_STR_RENAME_ENTRY);
}

void CAddImageDlg2::InitAddClass2(CGDEntry pos)
{
	m_pos = pos;
	m_pos.c3(0);
	m_pos.c4(0);
	m_pos.c5(0);
	m_pos.c6(0);

	m_nEntryMask = EM_C1|EM_C2|EM_ADD;

	m_strNewCaption.LoadString(IDS_STR_ADD_CLASS_2);
}


BOOL CAddImageDlg2::OnInitDialog() 
{
	CAnchorDlg::OnInitDialog();

	GetWindowRect(&m_rcFrame);

	m_wndC1.EnableWindow(m_nEntryMask & EM_C1);
	m_wndC2.EnableWindow(m_nEntryMask & EM_C2);
	m_wndC3.EnableWindow(m_nEntryMask & EM_C3);
	m_wndC4.EnableWindow(m_nEntryMask & EM_C4);
	m_wndC5.EnableWindow(m_nEntryMask & EM_C5);
	m_wndC6.EnableWindow(m_nEntryMask & EM_C6);

	CString strText;

	strText.LoadString((m_nEntryMask & EM_ADD) ? IDS_STR_BTN_ADD : IDS_STR_BTN_OK); m_wndOk.SetWindowText(strText);

	if (!m_strNewCaption.IsEmpty()) SetWindowText(m_strNewCaption);

	Use(m_vxxx->GetEditValPg(), (m_nEntryMask & EM_TEXT) != 0);

//	if (m_nEntryMask & EM_TEXT) {
//		Use(m_wndPgText, true);
//	} else if (m_nEntryMask & EM_FLAGS) {
//		Use(m_wndPgFlags, false);
//	} else if (m_nEntryMask & EM_PRIZEITEM) {
//		m_wndPgOOFItem.SelectTypePrize(true);
//		Use(m_wndPgOOFItem, false);
//	} else if (m_nEntryMask & EM_KEYITEM) {
//		m_wndPgOOFItem.SelectTypePrize(false);
//		Use(m_wndPgOOFItem, false);
//	} else if (m_nEntryMask & EM_MSITEMS_A) {
//		m_wndPgMSItems.SelectCat(true);
//		Use(m_wndPgMSItems, true);
//	} else if (m_nEntryMask & EM_MSITEMS_C) {
//		m_wndPgMSItems.SelectCat(false);
//		Use(m_wndPgMSItems, true);
//	} else if (m_nEntryMask & EM_B5353) {
//		Use(m_wndPgB1313, false);
//	} else if (m_nEntryMask & EM_SKTEXT) {
//		Use(m_wndPgSKText, true);
//	} else if (m_nEntryMask & EM_VAL) {
//		Use(m_wndPgWV, false);
//	} else if (m_nEntryMask & EM_ICONANIM) {
//		Use(m_wndPgIconAnim, false);
//	}

	CaptureLayout(acrLeft | acrTop);
	SetAnchor(IDOK, acrRight | acrBottom);
	SetAnchor(IDCANCEL, acrRight | acrBottom);
	SetAnchor(IDC_TAB_INPLACE, acrLeft | acrTop | acrRight | acrBottom);
	SetAnchor(m_wndUsingPg->GetDlgCtrlID(), acrLeft | acrTop | acrRight | acrBottom);

	MoveWindow(&m_rcFrame);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CAddImageDlg2::Use(CEditVPg &wndPg, bool fUseText)
{
	wndPg.Construction();

	if (!wndPg.Create(m_strText, m_nVal, this))
		return;

	CRect rc;
	m_wndTab.GetWindowRect(&rc);
	ScreenToClient(&rc);
//	m_wndTab.AdjustRect(FALSE, &rc);

	CRect rcIn;
	wndPg.GetClientRect(&rcIn);

	if (m_nEntryMask & EM_EDITDATA) {
		wndPg.EnableWindow();
		wndPg.ModifyStyleEx(0, WS_EX_CONTROLPARENT, 0);
		wndPg.ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);
		wndPg.SetWindowPos(&m_wndTab, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOOWNERZORDER);
		wndPg.ShowWindow(SW_SHOW);

		GetWindowRect(&m_rcFrame);
		m_rcFrame.right += max(0, rcIn.Width() -rc.Width());
		m_rcFrame.bottom += max(0, rcIn.Height() -rc.Height());
	}
	else {
		wndPg.ShowWindow(SW_HIDE);
	}

	m_wndUsingPg = &wndPg;
}

void CAddImageDlg2::OnOK() 
{
	if (m_wndUsingPg && !m_wndUsingPg->OnApply()) {
		return;
	}
	
	CAnchorDlg::OnOK();
}

void CAddImageDlg2::OnMove(int x, int y) 
{
	CAnchorDlg::OnMove(x, y);

	Moved();
}

void CAddImageDlg2::OnSize(UINT nType, int cx, int cy) 
{
	CAnchorDlg::OnSize(nType, cx, cy);

	Resized();
}
