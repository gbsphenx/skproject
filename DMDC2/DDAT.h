// DDAT.h: CDDAT クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DDAT_H__E4EC41E3_0CC4_44ED_A0A6_B39E22F8623B__INCLUDED_)
#define AFX_DDAT_H__E4EC41E3_0CC4_44ED_A0A6_B39E22F8623B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "RN.h"
#include "DMDBRMask.h"
#include "DDATUndo.h"
#include "DM2OrnateIdxMig.h"
#include "DDATLD.h"
#include "DDATDef.h"
#include "DMDBBT.h"
#include "DDATx.h"
#include "DMn.h"
#include "DM2RPtr.h"

#define MakeDMErrHR(X) MAKE_HRESULT(1,FACILITY_ITF,X)
#define DMERR_NO_MORE_SPACE			MakeDMErrHR(0x0001)
#define DMERR_INVALID_MAP_POS		MakeDMErrHR(0x0002)
#define DMERR_NO_ITEM				MakeDMErrHR(0x0003)
#define DMERR_DBREC_OUT_OF_RANGE	MakeDMErrHR(0x0004)
#define DMERR_INVALID_PARAM			MakeDMErrHR(0x0005)
#define DMERR_MAP_OUT_OF_RANGE		MakeDMErrHR(0x0006)
#define DMERR_MAP_CNT_AT_MAX		MakeDMErrHR(0x0006)
#define DMERR_ATLAS_RANGE_OUT_OF_RANGE	MakeDMErrHR(0x0006)
#define DMERR_STUCK						MakeDMErrHR(0x0007)
#define DMERR_INVALID_MAP_SIZE			MakeDMErrHR(0x0008)

#define DDSRF_NEW_NEXT		0x0001
#define DDSRF_NEW_STOLEN	0x0002

#define DDAT_MAX_MAPS (255U) // (64U)
#define DDAT_MAX_RECS (1024U)

enum dbIndex {
	dbDoor = 0,				// 0
	dbTeleporter,			// 1
	dbText,					// 2
	dbActuator,				// 3
	dbCreature,				// 4
	dbWeapon,				// 5
	dbCloth,				// 6
	dbScroll,				// 7
	dbPotion,				// 8
	dbContainer,			// 9
	dbMiscellaneous_item,	//10
	db11,
	db12,
	db13,
	dbMissile,				//14
	dbCloud,				//15
	dbMax = 16,
};

enum tileTypeIndex {
	ttWall = 0,	// 0
	ttFloor,	// 1
	ttPit,		// 2
	ttStairs,	// 3
	ttDoor,		// 4
	ttTeleporter,	// 5
	ttTrickWall,	// 6
	ttMapExit,		// 7
};

using namespace std;

class CDDAT : public CDDATUndoList
{
	// 
	struct Map_t
	{
		// 
		LevelDesc_t aDesc;
		// 
		Atlas_t aAtlas;
	};
	// 
	struct WalkRecord_t
	{
		// 
		CDMDBRMask &mask;
		// 
		CDMDBRMask maskLoc;

		// 
		WalkRecord_t(CDMDBRMask &mask): mask(mask) { }
	};

	// 
	DMDBS m_sRec[DDAT_MAX_RECS];
	// 
	UINT m_nRec[16];
	// 
	Map_t m_aMaps[DDAT_MAX_MAPS];
	// 
	UINT m_nMaps;
	// 
	typedef multimap<DBIPos_t, DBIPos_t> BTDB2DBMap;
	// 
	typedef multimap<DBIPos_t, AtlasPos_t> BTDB2AtlasMap;
	// 
//	BTDB2DBMap m_mmapDB2DB;
	// 
//	BTDB2AtlasMap m_mmapDB2Atlas;
	// 
	CDMDBRBTMap m_bt;
	// 
	AllMapDesc_t m_aAllDesc;
	// 
	BOOL m_fModified;
	// 
	CDMDBRMask m_maskOccupied;
	// 
	bool m_fNocs;

	// 
	bool loadFrom(CDDATx::CSerializedReader &fLoad, BOOL fLE, bool fDM2_EXTENDED_MAP);
	// 
	void deleteContents();
	// 
	HRESULT addDBRecTo(UINT iDB, const DMDBU &recAny, DBIPos_t &posNew);
	// 
	HRESULT addEmptyMap(UINT &iMapAt);
	// 
	HRESULT setMapDescAt(UINT iMap, const LevelDesc_t &desc, UINT nUIF);
	// 
	HRESULT setMapAtlasAt(UINT iMap, const Atlas_t &atlas, UINT nUIF);
	// 
	BOOL insertToDBRecAnd_(UINT iDB, UINT &iInsertBefore);
	// 
	static UINT sizeInDBRec(UINT iDB);
	// 
	HRESULT saveUndo_SetDBRecAt(const DBIPos_t &pos, const DMDBU &rec);
	// 
	HRESULT saveUndo_DeleteDBRecAt(const DBIPos_t &pos);
	// 
	HRESULT getMapAt(UINT iMap, Map_t *&ppMap);
	// 
	HRESULT saveUndo_SetAtlasBitAt(const AtlasPos_t &p, AtlasBit_t &bit);
	// 
	HRESULT saveUndo_SetAtlas(UINT iMap, Atlas_t &at);
	// 
	HRESULT saveUndo_SetLevelDesc(UINT iMap, LevelDesc_t &ld);
	// 
	HRESULT insertEmptyMapAt(UINT &iMapAt, UINT nUIF);
	// 
	HRESULT saveUndo_DeleteMap(UINT iMap);
	// 
	HRESULT getDBRecAt(const DBIPos_t &pos, DMDBU &rec) const;
	// 
	HRESULT getMapAt(UINT iMap, const Map_t *&ppMap) const;
	// 
	HRESULT cntAtlasAssoc(RN rn, UINT &n, UINT nDepth) const;
	// 
	HRESULT setAtlasBitAt(const AtlasPos_t &pos, const AtlasBit_t &one, UINT nUIF);
	// 
	HRESULT deleteMapAt(UINT iMapAt, UINT nUIF);
	// 
	HRESULT deleteMapAt_(UINT iMapAt);
	// 
	HRESULT saveUndo_InsertMapAt(UINT iMap, Map_t &m);
	// 
	HRESULT removeMapAssoc(UINT iMapAt, Map_t &m);
	// 
	HRESULT removeDBRecAssoc(DBIPos_t pos, BOOL bDeleteNext, BOOL bDeleteSub);
	// 
	HRESULT removeDBRecAssoc(RN rn, BOOL bDeleteNext, BOOL bDeleteSub);
	// 
	void intersectAtlasBitAndDBRec(const AtlasPos_t &pos, RN rn, BOOL fNext, BOOL bSet);
	// 
	void intersectDBRecAndDBRec(const DBIPos_t &p, const DMDBU &rec, BOOL bSet);
	// 
	void intersectDBRecAndDBRec(const DBIPos_t &p, const DBIPos_t &p2, BOOL fNext, BOOL bSet);
	// 
	HRESULT setDBRecAt(const DBIPos_t &pos, const DMDBU &rec, UINT nUIF);
	// 
	bool saveTo(CDDATx::CSerializedWriter &fInto, BOOL fLE, BOOL fAddCheckSum, bool fDM2_EXTENDED_MAP);
	// 
	HRESULT saveUndo_SetAllMapDesc(const AllMapDesc_t &s);
	// 
	HRESULT setAllMapDesc(const AllMapDesc_t &s);
	// 
	HRESULT insertDBRecAt(const DBIPos_t &posIdx, const DMDBU &rec);
	// 
	HRESULT addDBRecToEmpty(UINT iDB, const DMDBU &rec, DBIPos_t &posNew);
	// 
	HRESULT removeDBRecFrom(UINT iDB, UINT iRemoveAt);
	// 
	HRESULT setMapCnt(UINT nMaps);
	// 
	void getRecAt(UINT db, UINT idx, DMDBU &rec) const;
	// 
	void setRecAt(UINT db, UINT idx, const DMDBU &rec);
	// 
	HRESULT removeTailDBRec(UINT iDB);
	// 
	virtual void Undo_OnUndo(CDDATUndoListItem *p);
	// 
	virtual void Undo_OnUndoStacked();
	// 
	HRESULT updateOrnate(UINT iMap, const CDM2OrnateIdxMig &oim);
	// 
	CString computeDM1Text(UINT i);

	// 
	HRESULT optimize();
	// 
	HRESULT trackTarget(OptimizeTester_t &tester, RN rn);
	// 
	HRESULT optimizeFlatten();
	// 
	HRESULT rotateMapAtlas(UINT iMap, int vx, int vy);
	// 
	HRESULT copyRecords(RN rn, RN &rn2, WalkRecord_t &wr);
	// 
	HRESULT worldShift(char vx, char vy);
	// 
	HRESULT walkRecords(RN rn, WalkRecord_t &wr);
	// 
	void walkTest(CString &str, RN rn, int iIndent);
	// 
	void map_insertAt(UINT i);
	// 
	void map_removeAt(UINT i);

public:
	// 
	CDDAT();
	// 
	virtual ~CDDAT();
	// 
	virtual bool LoadFrom(CDDATx::CSerializedReader &fLoad, BOOL fLE, bool fDM2_EXTENDED_MAP);
	// 
	bool SaveTo(CDDATx::CSerializedWriter &fInto, BOOL fLE, BOOL fAddCheckSum, bool fDM2_EXTENDED_MAP);
	// 
	void DeleteContents();
	// 
	void DeleteUndoHistory();
	// 
	BOOL IsUndoable();
	// 
	BOOL DoUndo();
	// 
	void SetSuppression(bool fNocs) { m_fNocs = fNocs; }

	// 
	BOOL GetModified() const;
	// 
	void SetModified(BOOL fModified = TRUE);

	// 
	HRESULT AddEmptyMap(UINT &iMapAt);
	// 
	HRESULT InsertEmptyMapAt(UINT iMapBefore);
	// 
	HRESULT SetMapDescAt(UINT iMap, const LevelDesc_t &desc);
	// 
	HRESULT GetMapDescAt(UINT iMap, LevelDesc_t &desc) const;
	// 
	HRESULT SetMapAtlasAt(UINT iMap, const Atlas_t &atlas);
	// 
	HRESULT GetMapAtlasAt(UINT iMap, Atlas_t &atlas) const;
	// 
	HRESULT SetAtlasBitAt(const AtlasPos_t &pos, const AtlasBit_t &one);
	// 
	HRESULT GetAtlasBitAt(const AtlasPos_t &pos, AtlasBit_t &one) const;
	// 
	UINT CntMap() const;
	// 
	UINT CntAtlasAssocAt(const AtlasPos_t &pos) const;
	// 
	HRESULT DeleteMapAt(UINT iMapAt);

	// 
	HRESULT AddDBRecTo(UINT iDB, const DMDBU &rec, DBIPos_t &posNew);
	// 
	HRESULT InsertDBRecAt(const DBIPos_t &posIdx, const DMDBU &rec);
	// 
	HRESULT DeleteDBRecFrom(const DBIPos_t &posIdx);
	// 
	HRESULT RemoveDBRecAssoc(const DBIPos_t &posIdx, BOOL bDeleteNext, BOOL bDeleteSub);
	// 
	HRESULT GetDBRecAt(const DBIPos_t &pos, DMDBU &rec) const;
	// 
	HRESULT SetDBRecAt(const DBIPos_t &pos, const DMDBU &rec);
	// 
	UINT CntDBRecIn(UINT iDB) const;
	// 
	HRESULT BTFromDBRec(const DBIPos_t &pos, std::list<AtlasPos_t> &l) const;
	// 
	HRESULT BTFromDBRec(const DBIPos_t &pos, std::list<DBIPos_t> &l) const;
	// 
	HRESULT BTFromDBRec(const DBIPos_t &pos, RN &rn, int &nLinked) const;
	// 
	HRESULT SetAllMapDesc(const AllMapDesc_t &amd);
	// 
	HRESULT GetAllMapDesc(AllMapDesc_t &amd);
	// 
	HRESULT GetDetachedDBRecAt(const DBIPos_t &pos, DMDBU &rec) const;
	// 
	void Dev();
	// 
	HRESULT AddDBRecToEmpty(UINT iDB, const DMDBU &rec, DBIPos_t &posNew);
	// 
	HRESULT RemoveDBRecAt(const DBIPos_t &posIdx);
	// 
	HRESULT SetMapCnt(UINT nMaps);
	// 
	HRESULT Optimize();
	// 
	HRESULT OptimizeFlatten();
	// 
	HRESULT ScanOrnate(UINT iMap, CDM2OrnateIdxMig &oim);
	// 
	HRESULT UpdateOrnate(UINT iMap, const CDM2OrnateIdxMig &oim);
	// 
	HRESULT RotateMapAtlas(UINT iMap, int vx, int vy);
	// 
	HRESULT WorldShift(char vx, char vy);
	// 
	HRESULT Move2actu2();
	// 
	HRESULT CheckAllLevelDesc();

	// 
	static bool IsTileWall(BYTE c);

protected:
	// 
	void AddDMn(DMnBase *p)
	{
		delete p;
	}
	// 
	virtual void OnChangeAtlasBit(const AtlasPos_t &pos)
	{
		AddDMn(new DMnChangedAtlasBit(pos));
	}
	// 
	virtual void OnChangeLevelDesc(UINT iMapAt)
	{
		AddDMn(new DMnChangedLevelDesc(iMapAt));
	}
	// 
	virtual void OnChangeAtlas(UINT iMapAt)
	{
		AddDMn(new DMnChangedAtlas(iMapAt));
	}
	// 
	virtual void OnDeleteMap(UINT iMapAt)
	{
		AddDMn(new DMnDeletedMap(iMapAt));
	}
	// 
	virtual void OnChangeDBRec(const DBIPos_t &pos, UINT nSRf, const DMDBU &sOld, const DMDBU &sNew)
	{
		AddDMn(new DMnChangedRecord(pos, nSRf, sOld, sNew));
	}
	// 
	virtual void OnInsertMap(UINT iMapAt)
	{
		AddDMn(new DMnInsertedMap(iMapAt));
	}
	// 
	virtual void OnInsertDBRec(const DBIPos_t &pos, const DMDBU &rec)
	{
		AddDMn(new DMnInsertedRecord(pos, rec));
	}
	// 
	virtual void OnDeleteDBRec(const DBIPos_t &pos, const DMDBU &rec)
	{
		AddDMn(new DMnDeletedRecord(pos, rec));
	}
};

class CDDATWork : public CDDAT
{
	// 
	CDocument *m_lpNMTo;
	// 
	BOOL m_bStayUpdate;
	// 
	inline bool ifStayUpdate() { return m_bStayUpdate ? true : false; }
	// 
	void setModified() { if (m_lpNMTo) m_lpNMTo->SetModifiedFlag(); }
public:
	// 
	CDDATWork(CDocument *lpNMTo): m_lpNMTo(lpNMTo), m_bStayUpdate(FALSE) { }
	// 
	void StayUpdate(BOOL bLock = TRUE) { m_bStayUpdate = bLock; }
	// 
	virtual bool LoadFrom(CDDATx::CSerializedReader &fLoad, BOOL fLE, bool fDM2_EXTENDED_MAP);
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

#endif // !defined(AFX_DDAT_H__E4EC41E3_0CC4_44ED_A0A6_B39E22F8623B__INCLUDED_)
