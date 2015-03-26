// DMDBBT.cpp: CDMDBBT クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "exp00069.h"
#include "DMDBBT.h"
#include <utility>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace rel_ops;

// CDMDBRBT

int CDMDBRBT::CompareTo(const CDMDBRBT &s) const
{
	if (t < s.t) return 1;
	if (t > s.t) return -1;
	if (pos < s.pos) return 1;
	if (pos > s.pos) return -1;
	if (ap < s.ap) return 1;
	if (ap > s.ap) return -1;
	return 0;
}

// CDMDBRBTMap

void CDMDBRBTMap::remove(const value_type::second_type &value)
{
	iterator
		iterPos = begin(),
		iterEnd = end();
	for (; iterPos != iterEnd; ) {
		if (iterPos->second == value) {
			iterPos = erase(iterPos);
		} else {
			iterPos++;
		}
	}
}

void CDMDBRBTMap::Lookup(DBIPos_t pos, list<DBIPos_t> &t) const
{
	const_iterator
		iterPos = lower_bound(pos),
		iterEnd = upper_bound(pos);
	for (; iterPos != iterEnd; iterPos++) {
		const CDMDBRBT &r = iterPos->second;
		switch (r.t) {
		case CDMDBRBT::tRink:
		case CDMDBRBT::tRinks:
			t.push_back(r.pos);
		}
	}
}

void CDMDBRBTMap::Lookup(DBIPos_t pos, list<AtlasPos_t> &t) const
{
	const_iterator
		iterPos = lower_bound(pos),
		iterEnd = upper_bound(pos);
	for (; iterPos != iterEnd; iterPos++) {
		const CDMDBRBT &r = iterPos->second;
		switch (r.t) {
		case CDMDBRBT::tTile:
			t.push_back(r.ap);
		}
	}
}

void CDMDBRBTMap::Insert(DBIPos_t key, CDMDBRBT value)
{
	insert(value_type(key, value));
}

void CDMDBRBTMap::RemoveValue(CDMDBRBT value)
{
	remove(value);
}
