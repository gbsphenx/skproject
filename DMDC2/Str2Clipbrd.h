// Str2Clipbrd.h: CStr2Clipbrd クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STR2CLIPBRD_H__0C246754_94F0_432E_AB4E_5F7E325011A9__INCLUDED_)
#define AFX_STR2CLIPBRD_H__0C246754_94F0_432E_AB4E_5F7E325011A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStr2Clipbrd  
{
public:
	// 
	BOOL SetClipboardTextData(LPCSTR pszText);
	// 
	BOOL GetClipboardTextData(CString &strText);
	// 
	BOOL SetClipboardTextData2(LPCSTR pszText);
	// 
	BOOL GetClipboardTextData2(CString &strText);

};

#endif // !defined(AFX_STR2CLIPBRD_H__0C246754_94F0_432E_AB4E_5F7E325011A9__INCLUDED_)
