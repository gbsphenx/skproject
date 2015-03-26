// DM2RTextToken.h: CDM2RTextToken クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2RTEXTTOKEN_H__48EBF06C_0F51_4D15_80CF_B56540119A8E__INCLUDED_)
#define AFX_DM2RTEXTTOKEN_H__48EBF06C_0F51_4D15_80CF_B56540119A8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

enum {
	rttWeapons = 0,
	rttClothes,
	rttMisc,
	rttPotions,
	rttChests,
	rttScrolls,

	rttCreatures = rttChests,
};

struct DM2RTTable {
	BYTE cls;
	BYTE clsa;
	WORD valFrom, valTo;
	LPCTSTR pszClass;
	BYTE rtt;
};

extern const DM2RTTable _RTTableWide[], _RTTableCreature[];

class CDM2RTTokenList
{
public:
	// 
	BYTE al[64];
	// 
	bool forCreature;

	// 
	CDM2RTTokenList(bool forCreature = false): forCreature(forCreature) { }

	// 
	bool Parse(LPCTSTR pszText);
	// 
	CString ToStr() const;
};

class CDM2ItemSelList
{
public:
	// 
	bool forCreature;
	// 
	std::vector<UINT> sels;

	// 
	CDM2ItemSelList(bool forCreature = false): forCreature(forCreature) { }

	// 
	bool Parse(LPCTSTR pszText);
	// 
	CString ToStr() const;
};

#endif // !defined(AFX_DM2RTEXTTOKEN_H__48EBF06C_0F51_4D15_80CF_B56540119A8E__INCLUDED_)
