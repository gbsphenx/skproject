// EditVxxx.cpp: CEditVxxx クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "EditVxxx.h"
#include "GDEdCtx.h"
#include <afxtempl.h>

#include "resource.h"
#include "EditVSharpResistPg.h"
#include "EditVFlagsPg.h"
#include "EditVMSItemsPg.h"
#include "EditVSSItemPg.h"
#include "EditVWordValPg.h"
#include "EditVTextPg.h"
#include "EditVSKText.h"
#include "EditVIconAnimPg.h"
#include "EditVIOff.h"
#include "EditVPgTemp.h"
#include "EditVRuneSel.h"
#include "EditVSelectPg.h"
#include "EditVCreaturePg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// WVCF DECL

typedef struct _WVCF_TABLE {
	int wvct;
	int c4;
	CEditVxxx *(*pfnCreateVxx)(CEditVLower &);
	LPCSTR pszAlias;
}	_WVCF_TABLE;

#define BEGIN_WVCF_DECL()	\
	_WVCF_TABLE _wvcfDecl[] = {

#define DECL_WVCF1(C4,T2,V)	\
		{ T2::wvct, C4, _EditVxxx_Factory<T2>::Create, #V },

#define DECL_WVCF2(C4,T2,V,A)	\
		{ T2::wvct, C4, _EditVxxx_Factory<T2>::Create, #A },

#define END_WVCF_DECL()			\
		{ -1, NULL, NULL, },	\
	};

template<class T> class _EditVxxx_Factory
{
public:
	// 
	static CEditVxxx *Create(CEditVLower &vl) {
		return new T(vl);
	}
};

//////////////////////////////////////////////////////////////////////
// WVCF table

BEGIN_WVCF_DECL()
	DECL_WVCF1(0x5,CEditVMSItemsAPg,			Items)
	DECL_WVCF1(0x5,CEditVMSItemsCPg,			Creatures)
	DECL_WVCF1(0x5,CEditVSKText,				Command)
	DECL_WVCF1(0x5,CEditVTextPg,				Text)		// be at bottom!

	DECL_WVCF1(0x8,CEditVChampStatPg,			ChampStat)	// at bottom!

	DECL_WVCF1(0xB,CEditVWordValSignedPg,		Signed)
	DECL_WVCF1(0xB,CEditVWordValHexPg,			Hex)
	DECL_WVCF1(0xB,CEditVWordValBinaryPg,		Binary)
	DECL_WVCF1(0xB,CEditVBinaryFlagsPg,			Flags)
	DECL_WVCF1(0xB,CEditVSSKeyItemPg,			KeyItem)
	DECL_WVCF1(0xB,CEditVSSPrizeItemPg,			PrizeItem)
	DECL_WVCF1(0xB,CEditVSharpResistPg,			B5353)
	DECL_WVCF1(0xB,CEditVIconAnimPg,			IconAnim)
	DECL_WVCF1(0xB,CEditVRuneSelPg,				RuneSel)
	DECL_WVCF1(0xB,CEditVSelectPg,				Select)
	DECL_WVCF1(0xB,CEditVCreaturePg,			Creature)

	DECL_WVCF1(0xB,CEditVWordValUnsignedPg,		Unsigned)	// this must be at bottom!

	DECL_WVCF1(0xC,CEditVIOff,					ImageOffset)	// place at bottom!
END_WVCF_DECL()

//////////////////////////////////////////////////////////////////////
// CEditVxxxFactoryImpl

CEditVxxxFactoryImpl::CEditVxxxFactoryImpl(CGDEdCtx *pEdCtx)
: pEdCtx(NULL)
, pEdLower(NULL)
{
	SetEdCtx(pEdCtx);

	pEdLower = new CEditVLower(pEdCtx);
}

CEditVxxxFactoryImpl::~CEditVxxxFactoryImpl()
{
	SetEdCtx(NULL);

	if (pEdLower != NULL) {
		delete pEdLower;
		pEdLower = NULL;
	}

	POSITION pos = staticInstanceMap.GetStartPosition();
	while (pos != NULL) {
		int x;
		CEditVxxx *p;
		staticInstanceMap.GetNextAssoc(pos, x, p);
		delete p;
	}
}

void CEditVxxxFactoryImpl::SetEdCtx(CGDEdCtx *pEdCtx)
{
//	if (pEdCtx != NULL)
//		pEdCtx->AddRef();
//
//	if (this->pEdCtx != NULL)
//		this->pEdCtx->Release();

	this->pEdCtx = pEdCtx;
}

CEditVxxx &CEditVxxxFactoryImpl::GetVxxx(int wvct)
{
	CEditVxxx *vxxx;
	if (staticInstanceMap.Lookup(wvct, vxxx) == false) {
		staticInstanceMap[wvct] = vxxx = CreateVxxx(wvct);
	}
	return *vxxx;
}

CEditVxxx *CEditVxxxFactoryImpl::CreateVxxx(int wvct)
{
	int x = 0;
	while (true) {
		if (false
			|| _wvcfDecl[x].wvct == wvct
			|| _wvcfDecl[x + 1].wvct == -1
		) {
			return _wvcfDecl[x].pfnCreateVxx(*pEdLower);
		}
		x++;
	}
}

int CEditVxxxFactoryImpl::ParseName(LPCSTR psz)
{
	int x = 0;
	while (true) {
		if (false
			|| stricmp(_wvcfDecl[x].pszAlias, psz) == 0
			|| _wvcfDecl[x + 1].wvct == -1
		) {
			return _wvcfDecl[x].wvct;
		}
		x++;
	}
}

int CEditVxxxFactoryImpl::ParseName2(LPCSTR psz, BYTE c3)
{
	int x = 0;
	while (true) {
		if (false
			|| stricmp(_wvcfDecl[x].pszAlias, psz) == 0
			|| _wvcfDecl[x + 1].wvct == -1
			|| (_wvcfDecl[x].c4 == c3 && _wvcfDecl[x +1].c4 != c3)
		) {
			return _wvcfDecl[x].wvct;
		}
		x++;
	}
}
