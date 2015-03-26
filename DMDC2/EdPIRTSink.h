// EdPIRTSink.h: CEdPIRTSink クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDPIRTSINK_H__7CC088C5_3F67_4581_AC87_D157798DFEFF__INCLUDED_)
#define AFX_EDPIRTSINK_H__7CC088C5_3F67_4581_AC87_D157798DFEFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DDAT.h"
#include "DM2NameResolver.h"

class CEdPIRTSink  
{
public:
	// 
	virtual BYTE GetDB() = NULL;
	// 
	virtual bool GetRecord(DMDBU &rec) = NULL;
	// 
	virtual bool SetRecord(const DMDBU &rec) = NULL;
	// 
	virtual bool TileIsWall() = NULL;
	// 
	virtual CDDAT *GetDDIf() = NULL;
	// 
	virtual bool GetTilePos(AtlasPos_t &ap) = NULL;
	// 
	virtual CDM2OrnateIdxMig *GetOIM() = NULL;
	// 
	virtual DM2NameResolver_t *GetNameRes() = NULL;

};

#endif // !defined(AFX_EDPIRTSINK_H__7CC088C5_3F67_4581_AC87_D157798DFEFF__INCLUDED_)
