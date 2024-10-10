//--- SkWinCore2 -------------------------------------
//SPX: This part of code will hold more easily added code for
// - Retrocompatibility with DM1
// - New code for expanding DM2

#include <StdAfx.h>	// Required for MVC6 compilation

#include <SkVer.h>

#include <SkParam.h>
#include <SkDebug.h>
#include <SkGlobal.h>
#include <KCORE.H>
#include <SkMIDI.h>
#include <SkLUA.h>

//--- Common part with A.cpp
using namespace DMEncyclopaedia;
using namespace DM2Internal;
using namespace kkBitBlt;

#if defined(_USE_MFC80) || defined(_USE_MFC60)
#include <SkMFC.h>
#include <resource.h>
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)


#ifdef _USE_SDL
#include <SkSDL.h>
#endif // _USE_SDL
#ifdef __DJGPP__
#include <SkDOS.h>
#include <stdlib.h> // rand note: putting stdlib here and not right after stdafx prevents a bunch of conflicts with min/max macros
#endif // __DJGPP__


#include <KAITABLE.h>	// for dAITableGenuine (hard coded AI table)

//==============================================================================

void SkWinCore::READ_DUNGEON_STRUCTURE_EXTENDED_GAME(X16 isNewGame, int iDungeonMode)
{
	if (isNewGame == 1 && skwin.dung == _OPTION_DUNGEON_DMX_)
		return DUNGEON_CREATE_BLANK();
	if (isNewGame == 1 && skwin.dung == _OPTION_DUNGEON_BWY_BW_)
		return READ_DUNGEON_STRUCTURE_BW(isNewGame);
	else if (isNewGame == 1 && skwin.dung == _OPTION_DUNGEON_EOB_EOB1_)
		return READ_DUNGEON_STRUCTURE_EOB(isNewGame);
		//return INIT_BLANK_DUNGEON(isNewGame);
}

//==============================================================================

// This disable all hands and magic for some time; that allow some new type of attacks from creature
void SkWinCore::STUN_CHAMPION(U16 player, U16 stunvalue)
{
	Champion *champion = &glbChampionSquad[player];
	U16 cooldown = 0;
	U8 hand = 0;

	for (hand = 0; hand < 2; hand++)
	{
		cooldown = champion->handCooldown[hand];
		cooldown += stunvalue;
		if (cooldown >= 256)
			cooldown = 255;
		champion->handCooldown[hand] = cooldown;
	}

}

////////////////////////////////////////////////////////////////////////////////

// SPX: New function CURE_PLAGUE, similar to CURE_POISON
void SkWinCore::CURE_PLAGUE(U16 player)
{
	i16 di = player;
	if (di != -1) {
		U16 si = 0;
		Timer *bp04 = glbTimersTable;
		for (; si < glbTimersActiveCount; bp04++, si++) {
			if (bp04->TimerType() == ttyPlague) {
				if (bp04->actor == di) {
					DELETE_TIMER(si);
				}
			}
		}
		glbChampionSquad[di].PlagueValue = 0;
	}
	return;
}


// SPX: Custom, code added, similar to POISON
void SkWinCore::PROCESS_PLAGUE(i16 player, Bit16u counters)
{
	
	if (SkCodeParam::bUseIngameDebug)
	{
		U8 message[64];
		sprintf((char*)message, "PLAYER %d HAS %d PLAGUE COUNTERS.\n", player, counters);
		DISPLAY_HINT_TEXT(glbChampionColor[player], message);
	}
	X16 si = counters;
	if (player == -1)
		return;
	if (player +1 == glbNextChampionNumber)
		return;
	Champion *champion = &glbChampionSquad[player];
	WOUND_PLAYER(player, 1, 0, 0);
	ADJUST_STAMINA(player, max_value(1, si << 4));
	champion->curWater(champion->curWater() -100);
	if (champion->curWater() < WATER_MIN)
		champion->curWater(WATER_MIN);


	champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x800
	champion->heroFlag |= CHAMPION_FLAG_2000;	// 0x2000
	si--;
	if (si == 0)
		return;
	champion->PlagueValue++;

	// Create a new timer
	Timer newtimer;
	newtimer.TimerType(ttyPlague);
	newtimer.actor = (U8)player;
	newtimer.SetMap(glbPlayerMap);
	newtimer.SetTick(glbGameTick +0x24);
	newtimer.value = si;
	QUEUE_TIMER(&newtimer);
	return;
}














////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////







