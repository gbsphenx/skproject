// DMRecFmt.h: CDMRecFmt クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMRECFMT_H__D08D118B_B4B1_4012_993F_13EC98FA119F__INCLUDED_)
#define AFX_DMRECFMT_H__D08D118B_B4B1_4012_993F_13EC98FA119F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DDAT.h"

class CDMRecFmt  
{
	// 
	static LPCSTR pszNameTbl[2][16];

public:
	// 
	static CString Format(BYTE iDB, BYTE iDir, DMDBU rec, bool actu1);
	// 
	static LPCSTR GetTblName(BYTE i, bool actu1);

};

#endif // !defined(AFX_DMRECFMT_H__D08D118B_B4B1_4012_993F_13EC98FA119F__INCLUDED_)
