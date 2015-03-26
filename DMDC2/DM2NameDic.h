// DM2NameDic.h: CDM2NameDic クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2NAMEDIC_H__9FF9616A_DCE3_4BE6_A9F8_37F35C4227E3__INCLUDED_)
#define AFX_DM2NAMEDIC_H__9FF9616A_DCE3_4BE6_A9F8_37F35C4227E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DO2PathStr.h"

enum {
	a1tNo,
	a1tItem,
	a1tMap,
	a1tN,
	a1tMissile,
	a1tCreature,
	a1tHero,
	a1tAIState,
};

enum {
	a2tNo,
};

enum {
	a3tNo,
	a3tLocator,
	a3tStr,
};

enum {
	a3ltNo,
	a3ltTile,
	a3ltTrap,
};

enum {
	ahmt0 = 0,
	ahmtItemType,
	ahmtItemItem,
	ahmtCount,
	ahmtMissile,
	ahmtCreature,
	ahmtHero,
	ahmtOrnate,
	ahmtSw,
	ahmtMessageType,
	ahmtInverse,
	ahmtBeep,
	ahmtDelay,
	ahmtLocator,
	ahmtStr,
};

struct CDM2ActuHint {
	// 
	CString pszName;
	// 
	int a1t;
	// 
	int a2t;
	// 
	int a3t;
	// 
	int a3lt;

	// 
	CDM2ActuHint()
	{
		a1t = a2t = a3t = a3lt = 0;
	}
};

class CDM2NameDic  
{
	// 
	CString strChests[256];
	// 
	CString strClothes[256];
	// 
	CString strMisc[256];
	// 
	CString strPotions[256];
	// 
	CString strScrolls[256];
	// 
	CString strWeapons[256];
	// 
	CString strCreatures[256];
	// 
	CString strHeroes[256];
	// 
	CDM2ActuHint hintActuators[256];
	// 
	CString strMissiles[256];
	// 
	CString strMessages[256];

	// 
	bool LoadTbl(LPCTSTR psz, CString str[256], int nMax = 256);
	// 
	bool LoadTblA(LPCTSTR psz, bool fFloor);

public:
	// 
	bool Load(DO2::CDO2PathStr strWorkDir);
	// 
	static CDM2NameDic &GetInstance();

	// 
	LPCSTR GetName(int i, int x);
	// 
	LPCSTR GetHeroName(int x);
	// 
	const CDM2ActuHint &GetActuHint(int i, BOOL fWall);
	// 
	LPCSTR GetDATDBName(int i, int x);
	// 
	LPCSTR GetCreatureName(int x);
	// 
	LPCSTR GetOrnateName(int x, BOOL fWall);
};

#endif // !defined(AFX_DM2NAMEDIC_H__9FF9616A_DCE3_4BE6_A9F8_37F35C4227E3__INCLUDED_)
