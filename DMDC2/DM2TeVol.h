// DM2TeVol.h: CDM2TeVol クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2TEVOL_H__8493B725_3134_4D0E_A207_6A13AFF90FB2__INCLUDED_)
#define AFX_DM2TEVOL_H__8493B725_3134_4D0E_A207_6A13AFF90FB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <comdef.h>

class CDM2TeComm
{
public:
	// 
	IUnknownPtr pAx;
};

class CDM2TeCommTrop
{
public:
	// 
	CString strMes[256];
	// 
	int iEC;

	// 
	bool Enumerate(CDM2TeComm &r);
};

class CDM2TeVol
{
public:
	// 
	map<CString, CDM2TeComm> m;

	// 
	bool Enumerate();

};

#endif // !defined(AFX_DM2TEVOL_H__8493B725_3134_4D0E_A207_6A13AFF90FB2__INCLUDED_)
