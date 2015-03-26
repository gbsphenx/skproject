// HexEditCB.h: CHexEditCB クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEXEDITCB_H__79AD08D8_5E4F_4FF5_BB12_2CF5D27F58FD__INCLUDED_)
#define AFX_HEXEDITCB_H__79AD08D8_5E4F_4FF5_BB12_2CF5D27F58FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHexEditCB  
{
public:
	// 
	virtual void OnApplyHexEditCB(DWORD nCookie) = 0;

};

#endif // !defined(AFX_HEXEDITCB_H__79AD08D8_5E4F_4FF5_BB12_2CF5D27F58FD__INCLUDED_)
