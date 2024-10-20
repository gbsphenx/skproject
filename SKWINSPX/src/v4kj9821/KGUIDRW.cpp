#include <StdAfx.h>	// Required for MVC6 compilation

#include <SKVER.H>
#include <KCORE.H>

//^32CB:4E1C
// SPX: _32cb_4e1c renamed DRAW_STAIRS_FRONT
void SkWinCore::DRAW_STAIRS_FRONT(i16 xx)
{
	//^32CB:4E1C
	ENTER(4);
	//^32CB:4E22
	U8 gfxset = glbMapGraphicsSet;	// bp04
	U16 colorkey = glbSceneColorKey;		// di
	X16 si = (tblCellTilesRoom[xx].xsrd.w6[0] != 0) ? 1 : 0;
	i16 bp02 = _4976_45ae[RCJ(16,xx)][RCJ(2,si)];
	if (bp02 < 0)
		return;
	//^32CB:4E5F
	X8 bp03 = tlbGraphicsStairsFront1[RCJ(16,xx)][RCJ(2,si)];
	if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_GRAPHICSSET, gfxset, dtImage, bp03) != 0) {
		//^32CB:4E80
		DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, gfxset, bp03, bp02, colorkey, 0);
		return;
	}
	//^32CB:4E99
	QUERY_TEMP_PICST(1, 64, 64, 0, 0, 0, bp02, -1, colorkey, -1, 8, gfxset, tlbGraphicsStairsFront2[RCJ(16,xx)][RCJ(2,si)]);
	DRAW_TEMP_PICST();
	//^32CB:4EC8
	return;
}