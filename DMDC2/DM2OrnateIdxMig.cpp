// DM2OrnateIdxMig.cpp: CDM2OrnateIdxMig クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "exp00069.h"
#include "DM2OrnateIdxMig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CDM2OrnateIdxMig::LoadFrom(const LevelDesc_t &s)
{
	BYTE i;
	BYTE nShared = s.nSharedOrnateIndices, nWall = s.nWallOrnateIndices, nFloor = s.nFloorOrnateIndices, nMons = s.nCreatureIndices;

	CByteArray v;

	for (i = 0; i < nWall; i++)
		v.Add(s.bWallOrnateIndices[i]);
	for (i = 0; i < nFloor; i++)
		v.Add(s.bFloorOrnateIndices[i]);
	for (i = 0; i < nShared; i++)
		v.Add(s.bSharedOrnateIndices[i]);

	BYTE x = 0;

	*this = CDM2OrnateIdxMig();

	for (i = 0; i < nWall; i++, x++)
		m_wall.Add(v[x]);
	for (i = 0; i < nFloor; i++, x++)
		m_floor.Add(v[x]);
	for (i = 0; i < nShared; i++, x++)
		m_door.Add(v[x]);

	m_iDoor[0] = (s.w2 & 0x0080) ? s.n14_8_B : -1;
	m_iDoor[1] = (s.w2 & 0x0100) ? s.n14_C_F : -1;

	for (i = 0; i < nMons; i++)
		m_mons.Add(s.bCreatureIndices[i]);
}

void CDM2OrnateIdxMig::SaveTo(LevelDesc_t &s) const
{
	BYTE i;
	BYTE nShared = m_door.Size(), nWall = m_wall.Size(), nFloor = m_floor.Size(), nMons = m_mons.Size();

	CByteArray v;

	for (i = 0; i < nWall; i++)
		v.Add(m_wall[i]);
	for (i = 0; i < nFloor; i++)
		v.Add(m_floor[i]);
	for (i = 0; i < nShared; i++)
		v.Add(m_door[i]);

	BYTE x = 0;

	s.nWallOrnateIndices = nWall;
	s.nFloorOrnateIndices = nFloor;
	s.nSharedOrnateIndices = nShared;
	s.nCreatureIndices = nMons;

	for (i = 0; i < nWall; i++, x++)
		s.bWallOrnateIndices[i] = v[x];
	for (i = 0; i < nFloor; i++, x++)
		s.bFloorOrnateIndices[i] = v[x];
	for (i = 0; i < nShared; i++, x++)
		s.bSharedOrnateIndices[i] = v[x];

	if (m_iDoor[0] < 0) {
		s.w2 &=~0x0080;
	} else {
		s.w2 |= 0x0080; s.n14_8_B = m_iDoor[0];
	}

	if (m_iDoor[1] < 0) {
		s.w2 &=~0x0100;
	} else {
		s.w2 |= 0x0100; s.n14_C_F = m_iDoor[1];
	}

	for (i = 0; i < nMons; i++)
		s.bCreatureIndices[i] = m_mons[i];
}
