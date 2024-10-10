#include <StdAfx.h>	// Required for MVC6 compilation
#include <KCORE.H>
#include <SkDebug.h>

#if defined(_USE_MFC80) || defined(_USE_MFC60)
//#include "SkWinMFC.h"
//#include "resource.h"
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)




//^32CB:0AA6
void SkWinCore::QUERY_RAINFALL_PARAM(U8 *cls4, U16 *isMirrored)
{
	//^32CB:0AA6
	ENTER(0);
	//^32CB:0AA9
	*isMirrored = (glbRainDirection - _4976_5aa0) & 3;
	//^32CB:0ABB
	// SPX: 0x71 is vertical rain view / 0x6D is side rain view
	U8 cl = (*isMirrored == 0 || *isMirrored == 2) ? GFX_ENVIRONMENT_IMG_RAIN_STRAIGHT : GFX_ENVIRONMENT_IMG_RAIN_SLANTED;
	//^32CB:0ACF
	U16 rainLevel = 0;	// dx
	//^32CB:0AD1
	if (glbRainStrength >= RAIN_THRESHOLD_LEVEL_3) {
		//^32CB:0AD8
		rainLevel = 3;
	}
	//^32CB:0ADD
	else if (glbRainStrength >= RAIN_THRESHOLD_LEVEL_2) {
		//^32CB:0AE4
		rainLevel = 2;
	}
	//^32CB:0AE9
	else if (glbRainStrength >= RAIN_THRESHOLD_LEVEL_1) {
		//^32CB:0AF0
		rainLevel = 1;
	}
	//^32CB:0AF5
	else if (glbRainStrength > RAIN_THRESHOLD_LEVEL_0 && glbRainStrength < RAIN_THRESHOLD_LEVEL_1) {
		//^32CB:0B03
		rainLevel = 0;
	}
	//^32CB:0B05
	*cls4 = cl + U8(rainLevel);
	//^32CB:0B0F
	return;
}


//^3DF7:014D
// SPX: _3df7_014d renamed UPDATE_WEATHER
void SkWinCore::UPDATE_WEATHER(U16 aa)	// aa = 1 when called from timer, aa = 0 else
{
	//^3DF7:014D
	ENTER(46);
	//^3DF7:0153
	glbRainFlagSomething = tlbRainScene[RCJ(5,glbRainSceneType)][0];
	if (aa != 0) {
		//^3DF7:0167
		if (++glbRainRelated3 > 0x1f) {
			//^3DF7:0172
			_3df7_0037(0);
			return;
		}

		SkD((DLV_DBG_RAIN, "Update Weather >> cnt = %03d + %03d (idx1:%d idx2:%d pw:%d)\n"
			, glbRainLevelForGround, tlbRainRelated[RCJ(128,glbRainRelated2 + glbRainRelated3)]
			, glbRainRelated2, glbRainRelated3, glbRainMultiplicator
			));

		//^3DF7:017C
		_4976_5c28 = glbRainStormController;
		glbRainStormController += tlbRainRelated[RCJ(128,glbRainRelated2 + glbRainRelated3)] * glbRainMultiplicator;
		glbRainStormController = BETWEEN_VALUE(0, glbRainStormController, 255);
		SET_TIMER_WEATHER(U32(RAND16(0x100)) +0x32);	// RAND16(256) + 50
		return;
	}
	//^3DF7:01D5
	if (glbGameTick >= glbRainSpecialNextTick) {
		//^3DF7:01EA
		glbRainSomeLightMod = _4976_4764[RCJ(24,((glbGameTick + _4976_5c24) / 0x555) % 0x18)];
		glbRainSpecialNextTick = glbGameTick +0x555;

		SkD((DLV_DBG_RAIN, "Update Weather >> lightmod = %d / next tick = %d\n"
			, glbRainSomeLightMod
			, glbRainSpecialNextTick
			));

		if (glbRainFlagSomething != 0)
			RECALC_LIGHT_LEVEL();
	}
	//^3DF7:023B
	X16 bp0a = 0;
	X16 bp10;
	X16 bp12;
	if (glbRainStormController != 0) {
		//^3DF7:024A
		bp10 = (RAND() & 15) + (0x100 - glbRainStormController);
		bp12 = (glbRainStormController >= 0xcd) ? 0x28 : 7;
		glbRainLevelForSky = U8(glbRainStormController);
		if (glbRainStrength == 0)
		{
			//^3DF7:027E
			glbRainStrength = (RAND16(bp10) <= 7) ? 1 : 0;
		}
		else
		{
			glbRainStrength = U8(glbRainStormController);
		}

		SkD((DLV_DBG_RAIN, "Update Weather >> lvl=%03d / cnt=%03d (v1:%d v2:%d pw:%d) / tick=%d\n"
			, glbRainStrength
			, glbRainLevelForGround
			, glbRainRelated2, glbRainRelated3, glbRainMultiplicator
			, glbGameTick
			));

		//^3DF7:029B
		if (glbRainStrength != 0 && glbRainLevelForGround < 0xff) {
			//^3DF7:02A9
			if (false
				|| (glbRainStrength >= RAIN_THRESHOLD_LEVEL_3)
				|| (glbRainStrength >= RAIN_THRESHOLD_LEVEL_2 || (glbGameTick & 1) == 0)
				|| (glbRainStrength >= RAIN_THRESHOLD_LEVEL_1 || (glbGameTick % 3) == 0)
				|| (U16(glbGameTick & 3) == 0)
			) {
				//^3DF7:02EF
				glbRainLevelForGround++;
			}
		}
		//^3DF7:02F3
		if (glbRainHasThunderImage != 0) {
			bp0a = (RAND16(bp10) <= bp12) ? 1 : 0;
		}
	}
	else {
		//^3DF7:0314
		if (glbRainLevelForGround > 0 && (glbGameTick % 3) == 0)
			//^3DF7:0330
			--glbRainLevelForGround;
		bp0a = (RAND16(0x40) == 0) ? 1 : 0;
		glbRainStrength = 0;
		glbRainLevelForSky = 1;
	}
	//^3DF7:0354
	if (glbRainFlagSomething == 0)
		return;
	if (glbRecalcLightIndicator != 0) {
		glbRecalcLightIndicator = 0;
		RECALC_LIGHT_LEVEL();
	}
	DistantEnvironment *bp04 = &_4976_5c2a[0];
	if (glbRainHasCloudSky != 0 && glbRainLevelForSky >= 0x10) {
		//^3DF7:0388
		if (glbRainLevelForSky >= 0x80) {
			glbRainAmbientLightModifier = 1;
			bp04->envImg = GFX_ENVIRONMENT_IMG_SKY_CLOUDS+2;	// 0x69 Dark clouds
		}
		else if (glbRainLevelForSky >= 0x40) {
			//^3DF7:03A4
			bp04->envImg = GFX_ENVIRONMENT_IMG_SKY_CLOUDS+1;	// 0x68 Medium clouds
		}
		else {
			//^3DF7:03AD
			bp04->envImg = GFX_ENVIRONMENT_IMG_SKY_CLOUDS;	// 0x67 Grey clouds
		}
		//^3DF7:03B4
		if (RETRIEVE_ENVIRONMENT_CMD_CD_FW(bp04) != 0)
			//^3DF7:03C5
			bp04++;
	}
	//^3DF7:03C9
	if (glbRainHasWetGround != 0 && glbRainLevelForGround >= 0x40) {
		if (glbRainLevelForGround >= 0xc0) {
			//^3DF7:03DE
			bp04->envImg = GFX_ENVIRONMENT_IMG_WET_GROUND+2;	// 0x6C Big water puddles
		}
		else if (glbRainLevelForGround >= 0x80) {
			//^3DF7:03EE
			bp04->envImg = GFX_ENVIRONMENT_IMG_WET_GROUND+1;	// 0x6B Medium water puddles
		}
		else {
			//^3DF7:03F7
			bp04->envImg = GFX_ENVIRONMENT_IMG_WET_GROUND;	// 0x6A Small water puddles
		}
		//^3DF7:03FE
		if (RETRIEVE_ENVIRONMENT_CMD_CD_FW(bp04) != 0)
			//^3DF7:040F
			bp04++;
	}
	//^3DF7:0413
	i16 si;
	i16 di;
//	printf("glbRainStormController = %d (%d / %d) \n", glbRainStormController, 0x3c, 0xb6);
	if (bp0a != 0) {
		//^3DF7:041C
		bp0a = 0;
		X16 bp18 = (glbRainStormController < 0xb6) ? 1 : 0;
		if (bp18 != 0) {
			UPDATE_GLOB_VAR(0x40, 0x0, 0x6);
		}
		//^3DF7:0447
		X16 bp0e = 0;
		if (glbRainStormController >= 0x3c && RAND02() != 0) {
			//^3DF7:0462
			bp0e = 8;
			// SPX: 0x6C only found in ROOF set
			X16 bp16 = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, GDAT_GFXSET_THUNDER_POSITION);	// 0x6C
			if (bp16 != 0) {
				//^3DF7:0480
				// SPX: Will animate the Lightning Rod machine
				INVOKE_MESSAGE(di = (bp16 >> 8), si = (bp16 & 0xff), 0, 0, glbGameTick +1);
				// SPX: and get sound of thunder
				QUEUE_NOISE_GEN2(GDAT_CATEGORY_SPELL_MISSILES, missileThunderBolt, SOUND_STD_EXPLOSION, 0xfe, di, si, 1, 0x6c, U8(RAND16(0x20)));
			}
			else {
				do {
					//^3DF7:04D4
					di = RAND16(0x20);
					si = RAND16(0x20);
					if (di < 0 || di >= glbCurrentMapWidth || si < 0 || si >= glbCurrentMapHeight)
						continue;
					//printf("Thunder located at :%d %d\n", di, si);
					//^3DF7:0508
					ExtendedTileInfo bp2e;
					SUMMARIZE_STONE_ROOM(&bp2e, 0, di, si);
					if (bp2e.w0 != 1)
						continue;
					//^3DF7:0522
					TELE_inf bp1e;
					if (GET_TELEPORTER_DETAIL(&bp1e, U8(di), U8(si)) != 0)
						continue;
					//^3DF7:0538
					if (glbPlayerMap == glbCurrentMapIndex && glbPlayerPosX == di && glbPlayerPosY == si && glbRainStormController < 0xb6)
						continue;
					//^3DF7:0558
					if (glbGameTick < 0x1770)
						continue;
					//^3DF7:056F
					//printf("glbRainThunderRandom (static random?)= %d  over global 0x40 = %d\n", glbRainThunderRandom, GET_GLOB_VAR(0x40));
					if (GET_GLOB_VAR(0x40) <= glbRainThunderRandom)
						continue;
					//^3DF7:0580
					ObjectID bp14 = GET_CREATURE_AT(di, si);
					AIDefinition *bp08;
					i16 bp0c;
					if (bp14 != OBJECT_NULL) {
						//^3DF7:0591
						bp08 = QUERY_CREATURE_AI_SPEC_FROM_RECORD(bp14);
						if (bp08->ArmorClass == AI_DEF_ARMOR_MAX)
							continue;
						//^3DF7:05AD
						bp0c = CALC_SQUARE_DISTANCE(glbPlayerPosX, glbPlayerPosY, di, si);
						if (bp0c > 2) {
							if (bp0c > 3)
								continue;
							if (CALC_VECTOR_DIR(glbPlayerPosX, glbPlayerPosY, di, si) != glbPlayerDir)
								continue;
						}
					}
					//^3DF7:05E8
					CREATE_CLOUD(OBJECT_EFFECT_THUNDER, glbRainStormController, di, si, 255);
					if (glbChampionInventory == 0 && glbIsPlayerSleeping == 0)
						break;
					//^3DF7:0609
					if (bp18 != 0)
						break;
					//^3DF7:060F
					// SPX: glbPlayerPosX = x1, glbPlayerPosY = y1, di = x2, si = y2
					bp0c = CALC_SQUARE_DISTANCE(glbPlayerPosX, glbPlayerPosY, di, si);
					if (bp0c > 3)
						break;
					//^3DF7:062A
					if (CALC_VECTOR_DIR(glbPlayerPosX, glbPlayerPosY, di, si) != glbPlayerDir)
						break;
					//^3DF7:0642
					UPDATE_GLOB_VAR(0x41, 1, 3);
					break;
					//^3DF7:0652
				} while (--bp0e != 0);
			}
		}
		//^3DF7:065A
		if (bp0e == 0 && RAND01() != 0) {
			//^3DF7:0669
			bp04->envImg = U8(RAND16(3)) + GFX_ENVIRONMENT_IMG_THUNDER;	// 0x64 = thunder 1 => 0x66 = thunder 3
			if (RETRIEVE_ENVIRONMENT_CMD_CD_FW(bp04) != 0) {
				bp0a = 1;
				_098d_04c7(bp04->cmCD, bp04->cmCD +1, RAND16(0x64), &bp04->w4, &bp04->w6);
				bp04->cmFW = U8(RAND02());
				bp04++;
			}
		}
		//^3DF7:06D1
		if (bp0a != 0) {
			if (glbThunderJustCast == 0) {
				X16 bp0c;
				if (glbRainStormController == 0) {
					bp0c = RAND16(10) +5;
				}
				else {
					bp0c = 0x4c - (glbRainStormController / glbRainMultiplicator);
				}
				//^3DF7:070A
				bp0c = BETWEEN_VALUE(1, bp0c, 15);
				//SPX: Get sound of distant thunder
				QUEUE_NOISE_GEN1(GDAT_CATEGORY_ENVIRONMENT, glbMapGraphicsSet, SOUND_DEFAULT_STORM, 0x19, 0x40, glbPlayerPosX, glbPlayerPosY, bp0c);
				SkD((DLV_DBG_THUNDER, "Thunder at distance %d!\n", bp0c));
				glbThunderJustCast = 1;
			}
			else {
				//^3DF7:0743
				glbThunderJustCast = 0;
			}
		}
		//^3DF7:0749
		glbRecalcLightIndicator = 1;
		RECALC_LIGHT_LEVEL();
	}
	//^3DF7:0754
	bp04->envImg = 0xff;

#if (DM2_EXTENDED_MODE == 1)
	{	// Activate ambient sound, checking global variable then issuing a first Ambient Sound timer that will regenerate itself.
		if (glbXAmbientSoundActivated != 1)
		{
			glbXAmbientSoundActivated = 1;
			Timer xTimerRef;
			xTimerRef.SetMap(glbCurrentMapIndex);
			xTimerRef.SetTick(glbGameTick + 1);
			xTimerRef.TimerType(ttyAmbientSound);
			xTimerRef.actor = TIMER_ACTOR__00;
			xTimerRef.value = 0;
			QUEUE_TIMER(&xTimerRef);
		}
	}
#endif

	//^3DF7:075B
	return;
}


