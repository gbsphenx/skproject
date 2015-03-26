// DM2TextEncoder.h: CDM2TextEncoder クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2TEXTENCODER_H__1A3C875F_1173_4714_8F94_865F04D92EAE__INCLUDED_)
#define AFX_DM2TEXTENCODER_H__1A3C875F_1173_4714_8F94_865F04D92EAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SizeVuff.h"

#define DM2TET_EN 0
#define DM2TET_EN_PLAIN 1
#define DM2TET_JA1 2
#define DM2TET_JA2 3

class CDM2TextEncoder
{
public:
	// 
	virtual bool Encode(LPCSTR pszPos, LPCSTR pszEnd, SizeBuff &fv) = NULL;
	// 
	virtual bool Decode(LPCSTR pszPos, LPCSTR pszEnd, CString &strText) = NULL;

};

CDM2TextEncoder *GetDM2TextEncoder(int nDM2TET);

#endif // !defined(AFX_DM2TEXTENCODER_H__1A3C875F_1173_4714_8F94_865F04D92EAE__INCLUDED_)
