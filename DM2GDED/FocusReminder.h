// FocusReminder.h: CFocusReminder クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOCUSREMINDER_H__DBDD42BF_1CB8_4A53_BDFB_1595B3D6D6BA__INCLUDED_)
#define AFX_FOCUSREMINDER_H__DBDD42BF_1CB8_4A53_BDFB_1595B3D6D6BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFocusReminder  
{
public:
	// 
	list<HWND> listHwnd;

	// 
	bool MoveFocusTo(HWND hWnd);
	// 
	HWND FindRecentFocus();
	// 
	void RotateFocus(int x);

};

#endif // !defined(AFX_FOCUSREMINDER_H__DBDD42BF_1CB8_4A53_BDFB_1595B3D6D6BA__INCLUDED_)
