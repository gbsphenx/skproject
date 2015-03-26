// ReplC1Dlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "ReplC1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// anonymous-namespace

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

/////////////////////////////////////////////////////////////////////////////
// CReplC1Dlg ダイアログ

CReplC1Dlg::CReplC1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReplC1Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReplC1Dlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}

void CReplC1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReplC1Dlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
		UINT c6[6];

		DDX_Text_EntryHex02(pDX, IDC_EDIT1, c6[0]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT2, c6[1]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT3, c6[2]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT4, c6[3]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT5, c6[4]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT6, c6[5]);
		m_from = CGDEntry(c6[0], c6[1], c6[2], c6[3], c6[4], c6[5]);

		DDX_Text_EntryHex02(pDX, IDC_EDIT7, c6[0]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT8, c6[1]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT9, c6[2]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT10, c6[3]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT11, c6[4]);
		DDX_Text_EntryHex02(pDX, IDC_EDIT12, c6[5]);
		m_to = CGDEntry(c6[0], c6[1], c6[2], c6[3], c6[4], c6[5]);
	} else {
		{
			UINT c6[6] = {m_from.c1(), m_from.c2(), m_from.c3(), m_from.c4(), m_from.c5(), m_from.c6()};
			DDX_Text_EntryHex02(pDX, IDC_EDIT1, c6[0]);
			DDX_Text_EntryHex02(pDX, IDC_EDIT2, c6[1]);
			DDX_Text_EntryHex02(pDX, IDC_EDIT3, c6[2]);
			DDX_Text_EntryHex02(pDX, IDC_EDIT4, c6[3]);
			DDX_Text_EntryHex02(pDX, IDC_EDIT5, c6[4]);
			DDX_Text_EntryHex02(pDX, IDC_EDIT6, c6[5]);
		}
		{
			UINT c6[6] = {m_to.c1(), m_to.c2(), m_to.c3(), m_to.c4(), m_to.c5(), m_to.c6()};
			DDX_Text_EntryHex02(pDX, IDC_EDIT7, c6[0]);
			DDX_Text_EntryHex02(pDX, IDC_EDIT8, c6[1]);
			DDX_Text_EntryHex02(pDX, IDC_EDIT9, c6[2]);
			DDX_Text_EntryHex02(pDX, IDC_EDIT10, c6[3]);
			DDX_Text_EntryHex02(pDX, IDC_EDIT11, c6[4]);
			DDX_Text_EntryHex02(pDX, IDC_EDIT12, c6[5]);
		}
	}
}

BEGIN_MESSAGE_MAP(CReplC1Dlg, CDialog)
	//{{AFX_MSG_MAP(CReplC1Dlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReplC1Dlg メッセージ ハンドラ
