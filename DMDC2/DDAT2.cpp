// DDAT2.cpp: CDDAT2 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "exp00069.h"
#include "DDAT2.h"
#include "Exp00069Doc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CDDATWork2

void CDDATWork2::OnChangeAtlasBit(const AtlasPos_t &pos)
{
	pNMTo->DispatchDMUpdate(DPM_CHANGED_ATLASBIT, (void *)&pos);
}

void CDDATWork2::OnChangeLevelDesc(UINT iMapAt)
{
	ChangeLevelDesc_t notify;
	notify.iMapAt = iMapAt;

	pNMTo->DispatchDMUpdate(DPM_CHANGED_LEVELDESC, &notify);
}

void CDDATWork2::OnChangeAtlas(UINT iMapAt)
{
	RepaintAtlas_t notify;
	notify.iMapAt = iMapAt;

	pNMTo->DispatchDMUpdate(DPM_CHANGED_ATLAS, (void *)&notify);
}

void CDDATWork2::OnDeleteMap(UINT iMapAt)
{
	DeleteMap_t notify;
	notify.iMapAt = iMapAt;

	pNMTo->DispatchDMUpdate(DPM_DELETED_MAP, (void *)&notify);
}

void CDDATWork2::OnChangeDBRec(const DBIPos_t &pos, UINT nSRf)
{
	ChangeDBRec_t notify;
	notify.pos = pos;
	notify.nSRf = nSRf;

	pNMTo->DispatchDMUpdate(DPM_CHANGED_DBITEM, (void *)&notify);
}

void CDDATWork2::OnInsertMap(UINT iMapAt)
{
	InsertMap_t notify;
	notify.iMapAt = iMapAt;

	pNMTo->DispatchDMUpdate(DPM_INSERTED_MAP, (void *)&notify);
}

void CDDATWork2::OnInsertDBRec(const DBIPos_t &pos)
{
	InsertDBRec_t notify;
	notify.pos = pos;

	pNMTo->DispatchDMUpdate(DPM_INSERTED_DBITEM, (void *)&notify);
}

void CDDATWork2::OnDeleteDBRec(const DBIPos_t &pos)
{
	InsertDBRec_t notify;
	notify.pos = pos;

	pNMTo->DispatchDMUpdate(DPM_DELETED_DBITEM, (void *)&notify);
}

void CDDATWork2::OnChangingDBRec(const DBIPos_t &pos)
{
	ChangeDBRec_t notify;
	notify.pos = pos;

	pNMTo->DispatchDMUpdate(DPM_CHANGING_DBITEM, (void *)&notify);
}

void CDDATWork2::OnLinkedDBRec(const DBIPos_t &pos)
{
	ChangeDBRec_t notify;
	notify.pos = pos;

	pNMTo->DispatchDMUpdate(DPM_LINKED_DBITEM, (void *)&notify);
}
