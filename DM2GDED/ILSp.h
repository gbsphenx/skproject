// ILSp.h: CILSp クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ILSP_H__CCA7876E_0292_4CC8_ABC5_A7A76D90ACA2__INCLUDED_)
#define AFX_ILSP_H__CCA7876E_0292_4CC8_ABC5_A7A76D90ACA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GDAT.h"

class CILSp  
{
	// 
	CGDEntry posMask, posTest;

public:
	// 
	CILSp() { }
	// 
	CILSp(CGDEntry posMask, CGDEntry posTest): posMask(posMask), posTest(posTest)
	{

	}
	// 
	void Transform(CGDEntry &pos)
	{
		BYTE *pVal = (BYTE *)pos.c;
		const BYTE *pAnd = (const BYTE *)posMask.c;
		const BYTE *pXor = (const BYTE *)posTest.c;
		for (int i = 0; i < 6; i++) pVal[i] = (pVal[i] & pAnd[i]) | pXor[i];
	}
	// 
	static CILSp &GetInlang(int inlang);
	// 
	static CILSp &GetCnlang(int cnlang);

};

#endif // !defined(AFX_ILSP_H__CCA7876E_0292_4CC8_ABC5_A7A76D90ACA2__INCLUDED_)
