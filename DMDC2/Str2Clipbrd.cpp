// Str2Clipbrd.cpp: CStr2Clipbrd クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "exp00069.h"
#include "Str2Clipbrd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CStr2Clipbrd

BOOL CStr2Clipbrd::SetClipboardTextData(LPCSTR pszText)
{
	int n = strlen(pszText);
	LPSTR psz = (LPSTR)GlobalAlloc(GMEM_FIXED, n+1);
	if (psz != NULL) {
		strcpy(psz, pszText);
		SetClipboardData(CF_TEXT, (HANDLE)psz);
		return TRUE;
	}
	return FALSE;
}

BOOL CStr2Clipbrd::GetClipboardTextData(CString &strText)
{
	HGLOBAL hMem = (HGLOBAL)GetClipboardData(CF_TEXT);
	LPVOID pvMem;
	if (pvMem = GlobalLock(hMem)) {
		DWORD cbMem = GlobalSize(hMem);
		strText = CString((LPCSTR)pvMem, cbMem);
		GlobalUnlock(hMem);
		return true;
	}
	return false;
}

BOOL CStr2Clipbrd::SetClipboardTextData2(LPCSTR pszText)
{
	BOOL fRet = FALSE;
	if (OpenClipboard(AfxGetMainWnd()->GetSafeHwnd())) {
		EmptyClipboard();
		fRet = SetClipboardTextData(pszText);
		CloseClipboard();
	}
	return fRet;
}

BOOL CStr2Clipbrd::GetClipboardTextData2(CString &strText)
{
	BOOL fRet = FALSE;
	if (OpenClipboard(AfxGetMainWnd()->GetSafeHwnd())) {
		fRet = GetClipboardTextData(strText);
		CloseClipboard();
	}
	return fRet;
}
