
#include "StdAfx.h"
#include "DM2ScrollRec.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void DM2ScrollRec_t::Decode(WORD w2)
{
	WORD w = w2 / 0x400;

	if (w > 1) {
		fExt = true; text1 = "text in GRAPHICS.dat";
		index = w;
	} else {
		fExt = false; text1 = "text in DUNGEON.dat";
		index = w2 % 0x400;
	}
}
