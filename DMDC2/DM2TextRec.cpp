
#include "StdAfx.h"
#include "DM2TextRec.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void DM2TextRec_t::Decode(WORD w2)
{
	trpt = trptNo;

	index = 0;

	if (w2 & 4) {
		// complex extension
		text1 = "Complex";
	} else if (w2 & 2) {
		// simple extension
		switch (w2 & 0xF800) {
		case 0x9800:
		case 0x0800:
			trpt = trptCreature;
			break;
		case 0x7000:
			trpt = trptTextg;
			break;
		default:
			trpt = trptOrnate;
			break;
		}

		static const struct Table {
			WORD mask;
			LPCSTR psz;
		} tbl[] = {
			0x7000,"Text message (GRAPHICS.dat)",
			0x0000,"Still image (An alcove)",
			0x1000,"Animate image (Water basin)",
			0x2000,"Holder (Torch holder)",
			0x3000,"Ladder enabled",
			0x4000,"Recharge (Blue gem crop)",
			0x9800,"Creature spawn on trigger",
			0x6800,"2-state switch (Window)",
			0x5000,"Bush",
			0x3800,"Ladder disabled",
			0x8000,"Teleporter with flag",
			0x7800,"Teleporter",

			0x0800,"Creature transformer",
			-1,NULL,
		};

		WORD mask = w2 & 0xF800;

		text1.Format("x %04X", mask);
		for (int x=0; tbl[x].psz != NULL; x++) {
			if (tbl[x].mask == mask) {
				text1 = tbl[x].psz;
				break;
			}
		}

		index = (w2 >> 3) & 0xFF;
	} else {
		// simple text
		text1 = "Text message (DUNGEON.dat)";

		trpt = trptTexti;

		index = (w2 >> 3);
	}
}
