// DM2OrnateIdxMig.h: CDM2OrnateIdxMig クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2ORNATEIDXMIG_H__7082D348_9822_42B9_8E66_910746CB7978__INCLUDED_)
#define AFX_DM2ORNATEIDXMIG_H__7082D348_9822_42B9_8E66_910746CB7978__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DDATLD.h"

class CDM2OrnateIdxArray
{
	// 
	BYTE bData[16];
	// 
	BYTE nData;

public:
	// 
	CDM2OrnateIdxArray()
	{
		nData = 0;
	}
	// 
	int Add(BYTE x)
	{
		if (nData < 16) {
			int i = Find(x);
			if (i < 0) {
				bData[i = nData] = x;
				nData++;
			}
			return i;
		}
		return -1;
	}
	// 
	BYTE operator [](BYTE x) const
	{
		ASSERT(x < 16);
		return bData[x];
	}
	// 
	BYTE& operator [](BYTE x)
	{
		ASSERT(x < 16);
		return bData[x];
	}
	// 
	BYTE Size() const
	{
		return nData;
	}
	// 
	void Clear()
	{
		nData = 0;
	}
	// 
	int Find(BYTE x) const
	{
		for (BYTE i = 0; i < nData; i++) {
			if (bData[i] == x)
				return i;
		}
		return -1;
	}
	// 
	void RemoveTail()
	{
		if (nData != 0) nData--;
	}
};

class CDM2OrnateIdxMig  
{
public:
	// 
	int m_iDoor[2];
	// 
	CDM2OrnateIdxArray m_floor, m_wall, m_door, m_mons;

	// 
	CDM2OrnateIdxMig()
	{
		m_iDoor[0] = m_iDoor[1] = -1;
	}

	// 
	void LoadFrom(const LevelDesc_t &s);
	// 
	void SaveTo(LevelDesc_t &s) const;

};

#endif // !defined(AFX_DM2ORNATEIDXMIG_H__7082D348_9822_42B9_8E66_910746CB7978__INCLUDED_)
