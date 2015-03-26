// DMDBBT.h: CDMDBBT クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMDBBT_H__F04C0A85_E6F9_4D11_BAC0_8F5FA91B13A6__INCLUDED_)
#define AFX_DMDBBT_H__F04C0A85_E6F9_4D11_BAC0_8F5FA91B13A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DDATDef.h"

struct CDMDBRBT
{
public:
	// 
	enum {
		tRink,
		tRinks,
		tTile,
	};
	// 
	int t;
	// 
	DBIPos_t pos;
	// 
	AtlasPos_t ap;

	// 
	CDMDBRBT()
		: t(-1)
	{

	}
	// 
	CDMDBRBT(DBIPos_t pos, bool fRink)
		: t(fRink ? tRink : tRinks)
		, pos(pos)
	{
		
	}
	// 
	CDMDBRBT(AtlasPos_t ap)
		: t(tTile)
		, ap(ap)
	{

	}

	// 
	int CompareTo(const CDMDBRBT &s) const;
	// 
	bool operator ==(const CDMDBRBT &s) const
	{
		return CompareTo(s) == 0;
	}
};

class CDMDBRBTMap : multimap<DBIPos_t, CDMDBRBT>
{
	// 
	void remove(const value_type::second_type &value);

public:
	// 
	void Lookup(DBIPos_t pos, list<DBIPos_t> &t) const;
	// 
	void Lookup(DBIPos_t pos, list<AtlasPos_t> &t) const;
	// 
	void Insert(DBIPos_t key, CDMDBRBT value);
	// 
	void RemoveValue(CDMDBRBT value);
	// 
	void Clear() { clear(); }

};

#endif // !defined(AFX_DMDBBT_H__F04C0A85_E6F9_4D11_BAC0_8F5FA91B13A6__INCLUDED_)
