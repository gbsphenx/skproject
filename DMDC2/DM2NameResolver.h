// DM2NameResolver.h: DM2NameResolver クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2NAMERESOLVER_H__ECE1025F_BA68_40E3_BAC0_45CD49842568__INCLUDED_)
#define AFX_DM2NAMERESOLVER_H__ECE1025F_BA68_40E3_BAC0_45CD49842568__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DDAT.h"
#include "DMTextEdit.h"

class DM2Nameo
{
public:
	// 
	CString text[3];
};

class DM2NameResolver_t  
{
public:
	// 
	DMTextEdit_t texte;
	// 
	CDDAT *pDDIf;

	// 
	DM2NameResolver_t()
	{
		pDDIf = NULL;
	}

	// 
	bool GetStr(BYTE iDB, bool isWall, const DMDBU &rec, DM2Nameo &ref);
	// 
	CString GetInttextStr(UINT x);
	// 
	CString GetExttextStr(UINT x);
	// 
	CString GetIntScrollStr(UINT x);
	// 
	void EnumTextRecords(CWordArray &vec);

};

#endif // !defined(AFX_DM2NAMERESOLVER_H__ECE1025F_BA68_40E3_BAC0_45CD49842568__INCLUDED_)
