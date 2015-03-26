// DM2CompAdv.h: CDM2CompAdv クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2COMPADV_H__6CD624E6_4DB9_4A8A_9508_1380397AC49A__INCLUDED_)
#define AFX_DM2COMPADV_H__6CD624E6_4DB9_4A8A_9508_1380397AC49A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GDAT.h"
#include "DM2PIPDescList.h"
#include "GDEntry.h"

struct CDM2VwPalAdvisory
{
	// 
	CDM2PIPTS ts;
	// 
	int iSceneKey;
	// 
	CGDEntry pos;
	// 
	bool fOrnateKey;

	// 
	CDM2VwPalAdvisory()
	{
		ts = piptsNo;
		iSceneKey = -1;
		fOrnateKey = false;
	}
};

class CDM2VwPalAdvisor  
{
	// 
	CGDAT &core;

	// 
	int GetOrnateSceneKey(CGDEntry pos)
	{
		if (pos.c1() != 0x09 && pos.c1() != 0x0A && pos.c1() != 0x0F) return -1;

		pos.c3(0x0B);
		pos.c4(0x04);
		pos.c5(0);
		pos.c6(0);
		return core.GetValueAt(pos);
	}
	// 
	int GetSceneKey(BYTE nScene)
	{
		CGDEntry pos(0x08, nScene, 0x0B, 0x64, 0x00, 0x00);
		return core.GetValueAt(pos);
	}

public:
	// 
	CDM2VwPalAdvisor(CGDAT &core): core(core)
	{

	}
	// 
	CDM2VwPalAdvisory Match(CGDEntry pos, int nScene = -1);

};

#endif // !defined(AFX_DM2COMPADV_H__6CD624E6_4DB9_4A8A_9508_1380397AC49A__INCLUDED_)
