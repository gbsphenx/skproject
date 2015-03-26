// DMDBRMask.h: CDMDBRMask クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMDBRMASK_H__BF532907_C169_4921_907B_DE3C0B21E5CC__INCLUDED_)
#define AFX_DMDBRMASK_H__BF532907_C169_4921_907B_DE3C0B21E5CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RN.h"

class CDMDBRMask  
{
	// 
	BYTE cMask[16][1024/8];

public:
	// 
	CDMDBRMask()
	{
		ZeroMemory(cMask, sizeof(cMask));
	}
	// 
	void Set(int iDB, int iIdx)
	{
		cMask[iDB & 15][(iIdx / 8) & 127] |= (1 << (iIdx & 7));
	}
	// 
	void Set(RN rn)
	{
		Set(rn.db(), rn.idx());
	}
	// 
	void Reset(int iDB, int iIdx)
	{
		cMask[iDB & 15][(iIdx / 8) & 127] &= ~(1 << (iIdx & 7));
	}
	// 
	void Reset(RN rn)
	{
		Reset(rn.db(), rn.idx());
	}
	// 
	BOOL IsSet(int iDB, int iIdx)
	{
		return (cMask[iDB & 15][(iIdx / 8) & 127] & (1 << (iIdx & 7))) ? 1 : 0;
	}
	// 
	BOOL IsSet(RN rn)
	{
		return IsSet(rn.db(), rn.idx());
	}
};

#endif // !defined(AFX_DMDBRMASK_H__BF532907_C169_4921_907B_DE3C0B21E5CC__INCLUDED_)
