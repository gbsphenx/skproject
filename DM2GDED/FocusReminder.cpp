// FocusReminder.cpp: CFocusReminder クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "FocusReminder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL AFXAPI AfxIsDescendant(HWND hWndParent, HWND hWndChild);

bool CFocusReminder::MoveFocusTo(HWND hWnd)
{
	UINT n = listHwnd.size();

	while (n != 0) {
		HWND hWndParent = listHwnd.front();
		if (AfxIsDescendant(hWndParent, hWnd)) break;
		listHwnd.push_back(listHwnd.front());
		listHwnd.pop_front();
		n--;
	}

	if (n != 0)
		return true;
	return false;
}

HWND CFocusReminder::FindRecentFocus()
{
	if (listHwnd.size() != 0) {
		return listHwnd.front();
	}
	return NULL;
}

void CFocusReminder::RotateFocus(int x)
{
	if (listHwnd.size() != 0) {
		for (; x > 0; x--) listHwnd.push_back(listHwnd.front()), listHwnd.pop_front();
		for (; x < 0; x++) listHwnd.push_front(listHwnd.back()), listHwnd.pop_back();
	}
}
