// EditDMItemSubDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EditDMItemSubDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditDMItemSubDlg ダイアログ


CEditDMItemSubDlg::CEditDMItemSubDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD)
{
	m_iDB = 0;
	m_bUseAsHex = FALSE;
	//{{AFX_DATA_INIT(CEditDMItemSubDlg)
	//}}AFX_DATA_INIT
}

void CEditDMItemSubDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditDMItemSubDlg)
	//}}AFX_DATA_MAP
	switch (m_iDB) {
	case 0:
		DDX_Text2(pDX, IDC_EDIT_P01, m_ret.m_item.r0.w2);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r0.w2, 0, 65535);
		break;
	case 1:
		DDX_Text2(pDX, IDC_EDIT_P01, m_ret.m_item.r1.w2);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r1.w2, 0, 65535);
		DDX_Text2(pDX, IDC_EDIT_P02, m_ret.m_item.r1.w4);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r1.w4, 0, 65535);
		break;
	case 2:
		DDX_Text2(pDX, IDC_EDIT_P01, m_ret.m_item.r2.w2);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r2.w2, 0, 65535);
		break;
	case 3:
	case 11: case 12: case 13:
		DDX_Text2(pDX, IDC_EDIT_P01, m_ret.m_item.r3.w2);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r3.w2, 0, 65535);
		DDX_Text2(pDX, IDC_EDIT_P02, m_ret.m_item.r3.w4);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r3.w4, 0, 65535);
		DDX_Text2(pDX, IDC_EDIT_P03, m_ret.m_item.r3.w6);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r3.w6, 0, 65535);
		break;
	case 4:
		DDX_Text2(pDX, IDC_EDIT_P01, m_ret.m_item.r4.b4);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r4.b4, 0, 255);
		DDX_Text2(pDX, IDC_EDIT_P02, m_ret.m_item.r4.b5);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r4.b5, 0, 255);
		DDX_Text2(pDX, IDC_EDIT_P03, m_ret.m_item.r4.w6[0]);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r4.w6[0], 0, 65535);
		DDX_Text2(pDX, IDC_EDIT_P04, m_ret.m_item.r4.w6[1]);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r4.w6[1], 0, 65535);
		DDX_Text2(pDX, IDC_EDIT_P05, m_ret.m_item.r4.w6[2]);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r4.w6[2], 0, 65535);
		DDX_Text2(pDX, IDC_EDIT_P06, m_ret.m_item.r4.w6[3]);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r4.w6[3], 0, 65535);
		DDX_Text2(pDX, IDC_EDIT_P07, m_ret.m_item.r4.w14);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r4.w14, 0, 65535);
		DDX_Text2(pDX, IDC_EDIT_P08, m_ret.m_item.r4.w2);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r4.w2, 0, 65535);
		break;
	case 5:
		DDX_Text2(pDX, IDC_EDIT_P01, m_ret.m_item.r5.w2);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r5.w2, 0, 65535);
		break;
	case 6:
		DDX_Text2(pDX, IDC_EDIT_P01, m_ret.m_item.r6.w2);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r6.w2, 0, 65535);
		break;
	case 7:
		DDX_Text2(pDX, IDC_EDIT_P01, m_ret.m_item.r7.w2);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r7.w2, 0, 65535);
		break;
	case 8:
		DDX_Text2(pDX, IDC_EDIT_P01, m_ret.m_item.r8.w2);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r8.w2, 0, 65535);
		break;
	case 9:
		DDX_Text2(pDX, IDC_EDIT_P01, m_ret.m_item.r9.w4);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r9.w4, 0, 65535);
		DDX_Text2(pDX, IDC_EDIT_P02, m_ret.m_item.r9.b6[0]);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r9.b6[0], 0, 255);
		DDX_Text2(pDX, IDC_EDIT_P03, m_ret.m_item.r9.b6[1]);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r9.b6[1], 0, 255);
		DDX_Text2(pDX, IDC_EDIT_P04, m_ret.m_item.r9.w2);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r9.w2, 0, 65535);
		break;
	case 10:
		DDX_Text2(pDX, IDC_EDIT_P01, m_ret.m_item.r10.w2);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r10.w2, 0, 65535);
		break;
	case 14:
		DDX_Text2(pDX, IDC_EDIT_P01, m_ret.m_item.r14.b4[0]);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r14.b4[0], 0, 255);
		DDX_Text2(pDX, IDC_EDIT_P02, m_ret.m_item.r14.b4[1]);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r14.b4[1], 0, 255);
		DDX_Text2(pDX, IDC_EDIT_P03, m_ret.m_item.r14.w6);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r14.w6, 0, 65535);
		DDX_Text2(pDX, IDC_EDIT_P04, m_ret.m_item.r14.w2);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r14.w2, 0, 65535);
		break;
	case 15:
		DDX_Text2(pDX, IDC_EDIT_P01, m_ret.m_item.r15.w2);
		DDV_MinMaxUInt         (pDX, m_ret.m_item.r15.w2, 0, 65535);
		break;
	}
}

BEGIN_MESSAGE_MAP(CEditDMItemSubDlg, CDialog)
	//{{AFX_MSG_MAP(CEditDMItemSubDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditDMItemSubDlg メッセージ ハンドラ

#include <afxpriv.h>

void CEditDMItemSubDlg::_Afx_DDX_TextWithFormat2(CDataExchange* pDX, int nIDC,
	LPCTSTR lpszFormat, UINT nIDPrompt, ...)
{
	va_list pData;
	va_start(pData, nIDPrompt);

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	TCHAR szT[32];
	if (pDX->m_bSaveAndValidate)
	{
		// the following works for %d, %u, %ld, %lu
		::GetWindowText(hWndCtrl, szT, sizeof(szT) / sizeof(TCHAR));
		if (!_AfxSimpleScanf2(szT, lpszFormat, pData))
		{
			AfxMessageBox(nIDPrompt);
			pDX->Fail();        // throws exception
		}
	}
	else
	{
		wvsprintf(szT, lpszFormat, pData);
			// does not support floating point numbers - see dlgfloat.cpp
		AfxSetWindowText(hWndCtrl, szT);
	}

	va_end(pData);
}

BOOL CEditDMItemSubDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (m_bLimitless) {
		CWnd *wnd;
		switch (m_iDB) {
		case 4:
			if (wnd = GetDlgItem(IDC_EDIT_P08)) wnd->SendMessage(EM_SETREADONLY);
			break;
		case 9:
			if (wnd = GetDlgItem(IDC_EDIT_P04)) wnd->SendMessage(EM_SETREADONLY);
			break;
		case 14:
			if (wnd = GetDlgItem(IDC_EDIT_P04)) wnd->SendMessage(EM_SETREADONLY);
			break;
		}
	}

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
