// EdPIDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "exp00069.h"
#include "EdPIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL AFXAPI AfxIsDescendant(HWND hWndParent, HWND hWndChild);

/////////////////////////////////////////////////////////////////////////////
// CEdPIDlg

IMPLEMENT_DYNAMIC(CEdPIDlg, CDialog)

BEGIN_MESSAGE_MAP(CEdPIDlg, CDialog)
	//{{AFX_MSG_MAP(CEdPIDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdPIDlg メッセージ ハンドラ

BOOL CEdPIDlg::OnKillActive()
{
	if (!UpdateData()) return FALSE;

	if (!Revert(false)) return FALSE;

	return TRUE;
//	return CPropertyPage::OnKillActive();
}

BOOL CEdPIDlg::OnSetActive()
{
	if (!UpdateData()) return FALSE;

	if (!Revert(true)) return FALSE;

	UpdateData(FALSE);

	AfterRevert();

	return TRUE;
//	return CPropertyPage::OnSetActive();
}

bool CEdPIDlg::Revert(bool fRevert)
{
	DMDBU rec;
	if (!GetRecord(rec)) return false;

	if (fRevert) {
		return RevertRecord(rec, true);
	} else {
		return RevertRecord(rec, false) && SetRecord(rec);
	}
}

LRESULT CEdPIDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	LRESULT r = CDialog::WindowProc(message, wParam, lParam);

	if (message == WM_COMMAND) {
		TCHAR tc[MAX_PATH] = "";
		GetClassName((HWND)lParam, tc, MAX_PATH);

		UINT nCode = HIWORD(wParam);

		if (false
			|| (_tcsicmp(tc, "BUTTON") == 0 && (nCode == BN_CLICKED))
			|| (_tcsicmp(tc, "COMBOBOX") == 0 && (nCode == CBN_SELCHANGE))
		) {
			UINT nID = LOWORD(wParam);
			if (IsAcc(nID)) {
				LockedSendNotifyToParent(0);
			}
		}
		if (false
			|| (_tcsicmp(tc, "EDIT") == 0 && (nCode == EN_KILLFOCUS))
		) {
			UINT nID = LOWORD(wParam);
			HWND hWndUnk = ::GetFocus();
			if (IsAcc(nID) && hWndUnk && AfxIsDescendant(*this, hWndUnk)) {
				LockedSendNotifyToParent(0);
			}
		}

	}
	if (message == WM_HSCROLL) {
		TCHAR tc[MAX_PATH] = "";
		GetClassName((HWND)lParam, tc, MAX_PATH);

		if (false
			|| (_tcsicmp(tc, TRACKBAR_CLASS) == 0)
		) {
			LockedSendNotifyToParent(0);
		}
	}

	return r;
}
