// AddImageDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DM2GDED.h"
#include "AddImageDlg.h"
#include "EditPCMDlg.h"

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
#define EM_PCM 0x200
#define EM_FLAGS 0x400

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
// CAddImageDlg ダイアログ

void CAddImageDlg::Construction()
{
	//{{AFX_DATA_INIT(CAddImageDlg)
	m_strVal = _T("");
	m_strText = _T("");
	//}}AFX_DATA_INIT

	m_strVal = "0";
	m_strText = "<00>";
	m_nEntryMask = 0;
	m_pCB = NULL;
	m_nEntryCtxFlag = 0;
	m_fEditFlags = false;
	memset(m_fFlags, 0, sizeof(m_fFlags));
}

CString CAddImageDlg::FormatFlagText(UINT i, LPCTSTR psz)
{
	CString strText, str = psz;

	strText.Format(str.IsEmpty() ? "$%04X" : "$%04X - %s", 0U +i, (LPCTSTR)str);
	return strText;
}

CAddImageDlg::CAddImageDlg(CEditVxxx &vxxx, CWnd* pParent /*=NULL*/)
	: CDialog(CAddImageDlg::IDD, pParent)
	, m_vxxx(vxxx)
{
	Construction();

	switch (m_vxxx.GetOp().GetWvct()) {
	case wvctFlags:
		m_fEditFlags = true;
		break;
	}
}

void CAddImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddImageDlg)
	DDX_Control(pDX, IDC_LIST_FLAGS, m_wndFlags);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_wndEditPCM);
	DDX_Control(pDX, IDOK, m_wndOk);
	DDX_Control(pDX, IDC_EDIT_VAL, m_wndVal);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_wndText);
	DDX_Control(pDX, IDC_EDIT_C6, m_wndC6);
	DDX_Control(pDX, IDC_EDIT_C5, m_wndC5);
	DDX_Control(pDX, IDC_EDIT_C4, m_wndC4);
	DDX_Control(pDX, IDC_EDIT_C3, m_wndC3);
	DDX_Control(pDX, IDC_EDIT_C2, m_wndC2);
	DDX_Control(pDX, IDC_EDIT_C1, m_wndC1);
	DDX_Text(pDX, IDC_EDIT_VAL, m_strVal);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_strText);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
		if (m_fEditFlags) {
			UINT i;
			for (i = 0; i < 16; i++) {
				int r = m_wndFlags.GetSel(i);
				if (r < 0) break;
				m_fFlags[i] = (r > 0);
			}
		} else {
			WORD x;
			if (!m_vxxx.GetOp().Parse(m_strVal, x)) {
				AfxMessageBox(AFX_IDP_PARSE_INT);
				pDX->PrepareEditCtrl(IDC_EDIT_VAL);
				pDX->Fail();
			}
		}

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

		if (m_fEditFlags) {
			UINT i;
			for (i = 0; i < 16; i++) {
				m_wndFlags.SetSel(i, m_fFlags[i]);
			}
			m_wndFlags.SetTopIndex(0);
		}
	}
}

BEGIN_MESSAGE_MAP(CAddImageDlg, CDialog)
	//{{AFX_MSG_MAP(CAddImageDlg)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddImageDlg メッセージ ハンドラ

void CAddImageDlg::InitAddImage(CGDEntry pos)
{
	m_pos = pos;
	m_pos.c3(1);
	m_pos.c4(0);
	m_pos.c5(0);
	m_pos.c6(0);

	m_nEntryMask = EM_C4|EM_C5|EM_C6|EM_ADD;
}

void CAddImageDlg::InitAddText(CGDEntry pos)
{
	m_pos = pos;
	m_pos.c3(5);
	m_pos.c4(0);
	m_pos.c5(0);
	m_pos.c6(0);

	m_nEntryMask = EM_C4|EM_C5|EM_C6|EM_ADD|EM_TEXT;

	m_strNewCaption.LoadString(IDS_STR_ADD_TEXT);
}

void CAddImageDlg::InitAdd0B(CGDEntry pos)
{
	m_pos = pos;
	m_pos.c3(0xB);
	m_pos.c4(0);
	m_pos.c5(0);
	m_pos.c6(0);

	m_nEntryMask = EM_C4|EM_C5|EM_C6|EM_ADD|EM_VAL;

	m_strNewCaption.LoadString(IDS_STR_ADD_0B);
}

void CAddImageDlg::InitAdd0C(CGDEntry pos)
{
	m_pos = pos;
	m_pos.c3(0xC);
	m_pos.c4(0);
	m_pos.c5(0);
	m_pos.c6(0);

	m_nEntryMask = EM_C4|EM_C5|EM_C6|EM_ADD|EM_VAL;

	m_strNewCaption.LoadString(IDS_STR_ADD_0C);
}

void CAddImageDlg::InitEditText(CGDEntry pos, LPCSTR pszText)
{
	m_pos = pos;

	m_nEntryMask = EM_TEXT;
	m_strText = pszText;

	m_strNewCaption.LoadString(IDS_STR_EDIT_TEXT);
}

void CAddImageDlg::InitEdit0B(CGDEntry pos, UINT nVal)
{
	m_pos = pos;

	if (m_fEditFlags) {
		m_nEntryMask = EM_FLAGS;
		for (UINT i = 0; i < 16; i++) m_fFlags[i] = (nVal & (1 << i)) ? true : false;
	} else {
		m_nEntryMask = EM_VAL;
		m_strVal.Format(m_vxxx.GetOp().Format(nVal));
	}

	m_strNewCaption.LoadString(IDS_STR_EDIT_0B);
}

void CAddImageDlg::InitEdit0C(CGDEntry pos, UINT nVal)
{
	m_pos = pos;

	if (m_fEditFlags) {
		m_nEntryMask |= EM_FLAGS;
		for (UINT i = 0; i < 16; i++) m_fFlags[i] = (nVal & (1 << i)) ? true : false;
	} else {
		m_nEntryMask = EM_VAL;
		m_strVal.Format(m_vxxx.GetOp().Format(nVal));
	}

	m_strNewCaption.LoadString(IDS_STR_EDIT_0C);
}

void CAddImageDlg::InitEditPCM(CGDEntry pos, CEditPCMCB *pCB)
{
	m_pos = pos;

	m_nEntryMask = EM_PCM;
	m_pCB = pCB;

	m_strNewCaption.LoadString(IDS_STR_EDIT_PCM);
}

void CAddImageDlg::InitAddPCM(CGDEntry pos, CEditPCMCB *pCB)
{
	m_pos = pos;
	m_pos.c3(0x2);
	m_pos.c4(0);
	m_pos.c5(0);
	m_pos.c6(0);

	m_nEntryMask = EM_C4|EM_C5|EM_C6|EM_PCM|EM_ADD;
	m_pCB = pCB;

	m_strNewCaption.LoadString(IDS_STR_ADD_PCM);
}

void CAddImageDlg::InitAddClass2(CGDEntry pos)
{
	m_pos = pos;
	m_pos.c3(0);
	m_pos.c4(0);
	m_pos.c5(0);
	m_pos.c6(0);

	m_nEntryMask = EM_C1|EM_C2|EM_ADD;

	m_strNewCaption.LoadString(IDS_STR_ADD_CLASS_2);
}

void CAddImageDlg::InitAdd04(CGDEntry pos)
{
	m_pos = pos;
	m_pos.c3(4);
	m_pos.c4(0);
	m_pos.c5(0);
	m_pos.c6(0);

	m_nEntryMask = EM_C4|EM_C5|EM_C6|EM_ADD;

	m_strNewCaption.LoadString(IDS_STR_ADD_RAW);
}

void CAddImageDlg::InitAdd07(CGDEntry pos)
{
	m_pos = pos;
	m_pos.c3(7);
	m_pos.c4(0);
	m_pos.c5(0);
	m_pos.c6(0);

	m_nEntryMask = EM_C4|EM_C5|EM_C6|EM_ADD;

	m_strNewCaption.LoadString(IDS_STR_ADD_RAW);
}

void CAddImageDlg::InitAdd08(CGDEntry pos)
{
	m_pos = pos;
	m_pos.c3(8);
	m_pos.c4(0);
	m_pos.c5(0);
	m_pos.c6(0);

	m_nEntryMask = EM_C4|EM_C5|EM_C6|EM_ADD;

	m_strNewCaption.LoadString(IDS_STR_ADD_RAW);
}

void CAddImageDlg::InitAdd09(CGDEntry pos)
{
	m_pos = pos;
	m_pos.c3(9);
	m_pos.c4(0);
	m_pos.c5(0);
	m_pos.c6(0);

	m_nEntryMask = EM_C4|EM_C5|EM_C6|EM_ADD;

	m_strNewCaption.LoadString(IDS_STR_ADD_RAW);
}

void CAddImageDlg::InitAdd0E(CGDEntry pos)
{
	m_pos = pos;
	m_pos.c3(14);
	m_pos.c4(0);
	m_pos.c5(0);
	m_pos.c6(0);

	m_nEntryMask = EM_C4|EM_C5|EM_C6|EM_ADD;

	m_strNewCaption.LoadString(IDS_STR_ADD_RAW);
}

void CAddImageDlg::InitRename(CGDEntry pos)
{
	m_pos = pos;

	m_nEntryMask = EM_C1|EM_C2|EM_C4|EM_C5|EM_C6;

	m_strNewCaption.LoadString(IDS_STR_RENAME_ENTRY);
}

BOOL CAddImageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strText;

	m_wndC1.EnableWindow(m_nEntryMask & EM_C1);
	m_wndC2.EnableWindow(m_nEntryMask & EM_C2);
	m_wndC3.EnableWindow(m_nEntryMask & EM_C3);
	m_wndC4.EnableWindow(m_nEntryMask & EM_C4);
	m_wndC5.EnableWindow(m_nEntryMask & EM_C5);
	m_wndC6.EnableWindow(m_nEntryMask & EM_C6);
	m_wndVal.EnableWindow(m_nEntryMask & EM_VAL);
	m_wndText.EnableWindow(m_nEntryMask & EM_TEXT);
	m_wndEditPCM.EnableWindow(m_nEntryMask & EM_PCM);
	m_wndFlags.EnableWindow(m_nEntryMask & EM_FLAGS);

	strText.LoadString((m_nEntryMask & EM_ADD) ? IDS_STR_BTN_ADD : IDS_STR_BTN_OK); m_wndOk.SetWindowText(strText);

	if (!m_strNewCaption.IsEmpty()) SetWindowText(m_strNewCaption);

	if (false
		|| m_vxxx.GetOp().GetWvct() == wvctFlags
	) {
		m_wndFlags.ResetContent();
		UINT i;
		for (i = 0; i < 16; i++) {
			m_wndFlags.AddString(FormatFlagText(1 << i, m_vxxx.GetOp().GetHintText(i)));
			m_wndFlags.SetSel(i, m_fFlags[i]);
		}
		m_wndFlags.SetTopIndex(0);
	}
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

UINT CAddImageDlg::GetVal()
{
	if (m_fEditFlags) {
		UINT i, x = 0;
		for (i = 0; i < 16; i++) {
			if (m_fFlags[i]) x |= (1 << i);
		}
		return x;
	} else {
		WORD x;
		if (m_vxxx.GetOp().Parse(m_strVal, x))
			return x;
		return 0;
	}
}

void CAddImageDlg::OnButtonEdit() 
{
	CEditPCMDlg wndDlg(this);
	wndDlg.Init(m_pCB);
	wndDlg.DoModal();
}

HBRUSH CAddImageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd == &m_wndFlags && !m_fEditFlags) {
		hbr = GetSysColorBrush(COLOR_BTNFACE);
	}

	return hbr;
}
