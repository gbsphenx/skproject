// DM2NIMan.h: CDM2NIMan クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2NIMAN_H__F7BC95AF_AB33_4408_B875_BE5B77B65DFD__INCLUDED_)
#define AFX_DM2NIMAN_H__F7BC95AF_AB33_4408_B875_BE5B77B65DFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GDAT.h"
#include "DM2NICtx.h"

class CDM2NIMan  
{
	// 
	CGDAT &core;

	// 
	int GetOrnateSceneKey(CGDEntry pos, bool fColorKey1)
	{
		int c4 = -1;
		if (fColorKey1) {
			switch (pos.c1()) {
			case 0x09:
			case 0x0A:
			case 0x0F:
				c4 = 0x04; break;
			}
		} else {
			switch (pos.c1()) {
			case 0x09:
			case 0x0A:
				c4 = 0x11; break;
			case 0x0E:
				c4 = 0x12; break;
			}
		}

		if (c4 < 0) return -1;

		pos.c3(0x0B);
		pos.c4(c4);
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
	CDM2NIMan(CGDAT &core): core(core)
	{

	}
	// 
	bool Load(CGDEntry pos, CDM2NICtx &r);

};

#endif // !defined(AFX_DM2NIMAN_H__F7BC95AF_AB33_4408_B875_BE5B77B65DFD__INCLUDED_)
