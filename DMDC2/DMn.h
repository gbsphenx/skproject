
#ifndef _DMN_H_
#define _DMN_H_

struct DMnBase {
	// 
	DWORD nType;

	// 
	virtual ~DMnBase() { }
};

struct DMnChangedAtlasBit : DMnBase {
	// 
	AtlasPos_t pos;

	// 
	DMnChangedAtlasBit(const AtlasPos_t &pos)
		: pos(pos)
	{

	}
};

struct DMnChangedAtlas : DMnBase {
	// 
	UINT iMap;

	// 
	DMnChangedAtlas(UINT iMap)
		: iMap(iMap)
	{

	}
};

struct DMnDeletedMap : DMnBase {
	// 
	UINT iMap;

	// 
	DMnDeletedMap(UINT iMap)
		: iMap(iMap)
	{

	}
};

struct DMnChangedRecord : DMnBase {
	// 
	DBIPos_t pos;
	// 
	UINT nSRf;
	// 
	DMDBU sOld;
	// 
	DMDBU sNew;

	// 
	DMnChangedRecord(const DBIPos_t &pos, UINT nSRf, const DMDBU &sOld, const DMDBU &sNew)
		: pos(pos)
		, nSRf(nSRf)
		, sOld(sOld)
		, sNew(sNew)
	{

	}
};

struct DMnInsertedMap : DMnBase {
	// 
	UINT iMap;

	// 
	DMnInsertedMap(UINT iMap)
		: iMap(iMap)
	{

	}
};

struct DMnInsertedRecord : DMnBase {
	// 
	DBIPos_t pos;
	// 
	DMDBU sNew;

	// 
	DMnInsertedRecord(const DBIPos_t &pos, const DMDBU &sNew)
		: pos(pos)
		, sNew(sNew)
	{

	}
};

struct DMnDeletedRecord : DMnBase {
	// 
	DBIPos_t pos;
	// 
	DMDBU sOld;

	// 
	DMnDeletedRecord(const DBIPos_t &pos, const DMDBU &rec)
		: pos(pos)
		, sOld(rec)
	{

	}
};

struct DMnChangedLevelDesc : DMnBase {
	// 
	UINT iMap;

	// 
	DMnChangedLevelDesc(UINT iMap)
		: iMap(iMap)
	{

	}
};

#endif // _DMN_H_
