// DDAT2.h: CDDAT2 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DDAT2_H__C05AB85A_9C06_453A_8278_D8EF587F8582__INCLUDED_)
#define AFX_DDAT2_H__C05AB85A_9C06_453A_8278_D8EF587F8582__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DMEdCtx.h"

class CDDATWork2 : public CDDAT
{
	// 
	CDMEdCtx *pNMTo;

public:
	// 
	CDDATWork2(CDMEdCtx *pNMTo): pNMTo(pNMTo) { }
	// 
	virtual void OnChangeAtlasBit(const AtlasPos_t &pos);
	// 
	virtual void OnChangeLevelDesc(UINT iMapAt);
	// 
	virtual void OnChangeAtlas(UINT iMapAt);
	// 
	virtual void OnDeleteMap(UINT iMapAt);
	// 
	virtual void OnChangeDBRec(const DBIPos_t &pos, UINT nSRf);
	// 
	virtual void OnInsertMap(UINT iMapAt);
	// 
	virtual void OnInsertDBRec(const DBIPos_t &pos);
	// 
	virtual void OnDeleteDBRec(const DBIPos_t &pos);
	// 
	virtual void OnChangingDBRec(const DBIPos_t &pos);
	// 
	virtual void OnLinkedDBRec(const DBIPos_t &pos);

};

#endif // !defined(AFX_DDAT2_H__C05AB85A_9C06_453A_8278_D8EF587F8582__INCLUDED_)
