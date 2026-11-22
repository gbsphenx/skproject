//------------------------------------------------------------------------------
// SK-WEATHER
//	What is related to outside ambiance, rain and thunder
//------------------------------------------------------------------------------

//#include <StdAfx.h>	// Required for MVC6 compilation
#include <skcore.h>
#include <skdebug.h>
#include <skglobal.h>

//#if defined(_USE_MFC80) || defined(_USE_MFC60)
//#include "SkWinMFC.h"
//#include "resource.h"
//#endif // defined(_USE_MFC80) || defined(_USE_MFC60)




//^32CB:0AA6
void SkWinCore::WEATHER_QUERY_RAINFALL_IMAGEID(U8 *iRainImageID, U16 *isMirrored)
{
	*isMirrored = (cd.ge.glbRainDirection - _4976_5aa0) & 3;
	// SPX: 0x71 is vertical rain view / 0x6D is side rain view
	U8 iImageID = (*isMirrored == 0 || *isMirrored == 2) ? GFX_ENVIRONMENT_IMG_x71_RAIN_STRAIGHT : GFX_ENVIRONMENT_IMG_x6D_RAIN_SLANTED;
	U16 iRainLevel = C00_RAIN_POWERLEVEL_NONE;	// dx
	if (cd.ge.glbRainIntensity >= RAIN_THRESHOLD_x80_LEVEL_3) {
		iRainLevel = C03_RAIN_POWERLEVEL_STRONG;
	}
	else if (cd.ge.glbRainIntensity >= RAIN_THRESHOLD_x40_LEVEL_2) {
		iRainLevel = C02_RAIN_POWERLEVEL_MEDIUM;
	}
	else if (cd.ge.glbRainIntensity >= RAIN_THRESHOLD_x10_LEVEL_1) {
		iRainLevel = C01_RAIN_POWERLEVEL_SMALL;
	}
	else if (cd.ge.glbRainIntensity > RAIN_THRESHOLD_x00_LEVEL_0 && cd.ge.glbRainIntensity < RAIN_THRESHOLD_x10_LEVEL_1) {
		iRainLevel = C00_RAIN_POWERLEVEL_NONE;
	}
	*iRainImageID = iImageID + U8(iRainLevel);
	return;
}

// SPX: Not originally in V4, added for convienence
U8 SkWinCore::WEATHER_GET_HOUR_OF_DAY()
{
	return RCJ(24,((glbGameTick + cd.ge.glbDayTimeTicksOffset) / C1365_WEATHER_HOUR_TICKS) % C24_WEATHER_DAY_HOURS);
}

//^3DF7:014D
// SPX: _3df7_014d renamed WEATHER_UPDATE
void SkWinCore::WEATHER_UPDATE(U16 aa)	// aa = 1 when called from timer, aa = 0 else
{
	ENTER(46);
	cd.ge.glbEnvironmentIsOutdoor = cd.ge.tlbRainScene[RCJ(5,cd.ge.glbRainSceneType)][0];
	if (aa != 0) {
		if (++cd.ge.glbRainRelated3 > 0x1F) {
			WEATHER_INIT(0);
			return;
		}

		SkD((DLV_DBG_RAIN, "Update Weather >> cnt = %03d + %03d (idx1:%d idx2:%d pw:%d)\n"
			, cd.ge.glbRainLevelForGround, cd.ge.tlbRainRelated[RCJ(128,cd.ge.glbRainRelated2 + cd.ge.glbRainRelated3)]
			, cd.ge.glbRainRelated2, cd.ge.glbRainRelated3, cd.ge.glbRainMultiplicator
			));

		cd.ge.glbStormPrevious = cd.ge.glbRainStormController;
		cd.ge.glbRainStormController += cd.ge.tlbRainRelated[RCJ(128, cd.ge.glbRainRelated2 + cd.ge.glbRainRelated3)] * cd.ge.glbRainMultiplicator;
		cd.ge.glbRainStormController = BETWEEN_VALUE(0, cd.ge.glbRainStormController, 255);
		WEATHER_SET_TIME_EVENT(U32(RAND16(0x100)) + 0x32);	// RAND16(256) + 50
		return;
	}
	if (glbGameTick >= cd.ge.glbRainSpecialNextTick) {
		U8 iDayHour = WEATHER_GET_HOUR_OF_DAY();
		cd.ge.glbWeatherDayHourLight = tblOutsideLightByHour[iDayHour];
		cd.ge.glbRainSpecialNextTick = glbGameTick + C1365_WEATHER_HOUR_TICKS;

		SkD((DLV_DBG_RAIN, "Update Weather >> lightmod = %d / next tick = %d\n"
			, cd.ge.glbWeatherDayHourLight
			, cd.ge.glbRainSpecialNextTick
			));

		if (cd.ge.glbEnvironmentIsOutdoor != 0)
			RECALC_LIGHT_LEVEL();
	}
	X16 bp0a = 0;
	X16 bp10;
	X16 bp12;
	if (cd.ge.glbRainStormController != 0) {
		bp10 = (RAND() & 15) + (0x100 - cd.ge.glbRainStormController);
		bp12 = (cd.ge.glbRainStormController >= 0xCD) ? 0x28 : 7;
		cd.ge.glbRainLevelForSky = U8(cd.ge.glbRainStormController);
		if (cd.ge.glbRainIntensity == 0) {
			cd.ge.glbRainIntensity = (RAND16(bp10) <= 7) ? 1 : 0;
		}
		else {
			cd.ge.glbRainIntensity = U8(cd.ge.glbRainStormController);
		}

		SkD((DLV_DBG_RAIN, "Update Weather >> lvl=%03d / cnt=%03d (v1:%d v2:%d pw:%d) / tick=%d\n"
			, cd.ge.glbRainIntensity
			, cd.ge.glbRainLevelForGround
			, cd.ge.glbRainRelated2, cd.ge.glbRainRelated3, cd.ge.glbRainMultiplicator
			, glbGameTick
			));

		if (cd.ge.glbRainIntensity != 0 && cd.ge.glbRainLevelForGround < 0xff) {
			if (false
				|| (cd.ge.glbRainIntensity >= RAIN_THRESHOLD_x80_LEVEL_3)
				|| (cd.ge.glbRainIntensity >= RAIN_THRESHOLD_x40_LEVEL_2 || (glbGameTick & 1) == 0)
				|| (cd.ge.glbRainIntensity >= RAIN_THRESHOLD_x10_LEVEL_1 || (glbGameTick % 3) == 0)
				|| (U16(glbGameTick & 3) == 0)
			) {
				cd.ge.glbRainLevelForGround++;
			}
		}
		if (cd.ge.glbMapSceneHasThunderImage != 0) {
			bp0a = (RAND16(bp10) <= bp12) ? 1 : 0;
		}
	}
	else {
		if (cd.ge.glbRainLevelForGround > 0 && (glbGameTick % 3) == 0)
			--cd.ge.glbRainLevelForGround;
		bp0a = (RAND16(0x40) == 0) ? 1 : 0;
		cd.ge.glbRainIntensity = 0;
		cd.ge.glbRainLevelForSky = 1;
	}
	if (cd.ge.glbEnvironmentIsOutdoor == 0)
		return;
	if (cd.ge.glbLightningFlash != 0) {
		cd.ge.glbLightningFlash = 0;
		RECALC_LIGHT_LEVEL();
	}
	// This is a table of 5 distant possible elements; first two are for sky, then ground
	DistantEnvironment *xEnvElement = &cd.ge.tblEnvironmentElement[0];	// bp04
	if (cd.ge.glbMapSceneHasCloudSky != 0 && cd.ge.glbRainLevelForSky >= 0x10) {
		if (cd.ge.glbRainLevelForSky >= 0x80) {
			cd.ge.glbRainAmbientLightModifier = 1;
			xEnvElement->envImg = GFX_ENVIRONMENT_IMG_x67_SKY_CLOUDS+2;	// 0x69 Dark clouds
		}
		else if (cd.ge.glbRainLevelForSky >= 0x40) {
			xEnvElement->envImg = GFX_ENVIRONMENT_IMG_x67_SKY_CLOUDS+1;	// 0x68 Medium clouds
		}
		else {
			xEnvElement->envImg = GFX_ENVIRONMENT_IMG_x67_SKY_CLOUDS;		// 0x67 Grey clouds
		}
		if (ENVIRONMENT_GET_CMD_CD_FW(xEnvElement) != 0)
			xEnvElement++;
	}
	if (cd.ge.glbMapSceneHasWetGround != 0 && cd.ge.glbRainLevelForGround >= 0x40) {
		if (cd.ge.glbRainLevelForGround >= 0xc0) {
			xEnvElement->envImg = GFX_ENVIRONMENT_IMG_x6A_WET_GROUND+2;	// 0x6C Big water puddles
		}
		else if (cd.ge.glbRainLevelForGround >= 0x80) {
			xEnvElement->envImg = GFX_ENVIRONMENT_IMG_x6A_WET_GROUND+1;	// 0x6B Medium water puddles
		}
		else {
			xEnvElement->envImg = GFX_ENVIRONMENT_IMG_x6A_WET_GROUND;		// 0x6A Small water puddles
		}
		if (ENVIRONMENT_GET_CMD_CD_FW(xEnvElement) != 0)
			xEnvElement++;
	}
	i16 si;
	i16 di;
//	printf("glbRainStormController = %d (%d / %d) \n", glbRainStormController, 0x3c, 0xb6);
	if (bp0a != 0) {
		bp0a = 0;
		X16 iIsWeakStorm = (cd.ge.glbRainStormController < C182_STORM_MEDIUM_THRESHOLD) ? 1 : 0;	// bp18
		if (iIsWeakStorm != 0) {
			UPDATE_GLOB_VAR(C064_GLOBAL_BYTE_00_STORM_STRONG, 0x00, C06_GLOBAL_VAR_OP_NEW_VALUE);
		}
		X16 bp0e = 0;
		if (cd.ge.glbRainStormController >= C060_STORM_WEAK_THRESHOLD && RAND02() != 0) {
			bp0e = 8;
			// SPX: 0x6C only found in ROOF set
			X16 xLightningRodPosition = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x08_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, GDAT_GFXSET_x6C_THUNDER_POSITION);	// bp16, 0x6C
			i16 iLightningRodPosX = 0;
			i16 iLightningRodPosY = 0;
			// This specific value in original DM2 is at x=10, y=09 (0x0A09), which is the exact position of the Lightning Rod machine.
			if (xLightningRodPosition != 0) {	// If a Lightning Rod position is specified, it will capture the Lightning.
				// SPX: Will animate the Lightning Rod machine
				INVOKE_MESSAGE(iLightningRodPosX = (xLightningRodPosition >> 8), iLightningRodPosY = (xLightningRodPosition & 0xFF), 0, 0, glbGameTick +1);
				// SPX: and get sound of thunder
				QUEUE_NOISE_GEN2(GDAT_CATEGORY_x0D_SPELL_MISSILES, C48_EXPLOSION_THUNDERBOLT, SOUND_STD_EXPLOSION, 0xFE, iLightningRodPosX, iLightningRodPosY, 1, 0x6C, U8(RAND16(0x20)));
			}
			else {
				do {	// lightning_strike
					i8 iStrikePosX = i8(RAND16(C32_MAP_TILE_MAX));	// di
					i8 iStrikePosY = i8(RAND16(C32_MAP_TILE_MAX));	// si
					if (iStrikePosX < 0 || iStrikePosX >= glbCurrentMapWidth || iStrikePosY < 0 || iStrikePosY >= glbCurrentMapHeight)
						continue;
					//printf("Thunder located at :%d %d\n", iStrikePosX, iStrikePosY);
					ExtendedTileInfo xTileInfo;	// bp2e
					SUMMARIZE_STONE_ROOM(&xTileInfo, 0, iStrikePosX, iStrikePosY);
					if (xTileInfo.w0 != 1)
						continue;
					// Check "lightning_strike" as the continue condition seems different
					TELE_inf bp1e;
					if (GET_TELEPORTER_DETAIL(&bp1e, U8(iStrikePosX), U8(iStrikePosY)) != 0)
						continue;
					if (cd.pi.glbPlayerMap == glbCurrentMapIndex && cd.pi.glbPlayerPosX == iStrikePosX 
							&& cd.pi.glbPlayerPosY == iStrikePosY && cd.ge.glbRainStormController < C182_STORM_MEDIUM_THRESHOLD)
						continue;
					// Before a certain game tick time, thunder can not strike on party
					if (glbGameTick < 0x1770)	// 6000 for V4, this is 12000 for V5
						continue;
					//printf("glbRainThunderRandom (static random?)= %d  over global 0x40 = %d\n", glbRainThunderRandom, GET_GLOB_VAR(0x40));
					if (GET_GLOB_VAR(C064_GLOBAL_BYTE_00_STORM_STRONG) <= cd.ge.glbRainThunderRandom)
						continue;
					ObjectID oidCreature = GET_CREATURE_AT(iStrikePosX, iStrikePosY);	// bp14
					AIDefinition *pCreatureStats;	// bp08
					i16 iPlayerDistanceToStrikePos;	// bp0c
					if (oidCreature != OBJECT_NULL) {
						pCreatureStats = QUERY_CREATURE_AI_SPEC_FROM_RECORD(oidCreature);
						if (pCreatureStats->ArmorClass == AI_DEF_ARMOR_MAX)
							continue;
						iPlayerDistanceToStrikePos = CALC_SQUARE_DISTANCE(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, iStrikePosX, iStrikePosY);
						if (iPlayerDistanceToStrikePos > 2) {
							if (iPlayerDistanceToStrikePos > 3)
								continue;
							if (CALC_VECTOR_DIR(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, iStrikePosX, iStrikePosY) != cd.pi.glbPlayerDir)
								continue;
						}
					}
					// Then, if near and seen by player, a THUNDER explosion is created
					CLOUD_EXPLOSION_CREATE(OBJECT_EFFECT_THUNDER, cd.ge.glbRainStormController, iStrikePosX, iStrikePosY, C255_EXPLOSION_AT_CENTER);
					printf("Thunder created @ %02d,%02d\n", iStrikePosX, iStrikePosY);
					if (glbChampionInventory == 0 && cd.pi.glbIsPlayerSleeping == 0)
						break;
					if (iIsWeakStorm != 0)
						break;
					// SPX: glbPlayerPosX = x1, glbPlayerPosY = y1, iStrikePosX = x2, iStrikePosY = y2
					iPlayerDistanceToStrikePos = CALC_SQUARE_DISTANCE(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, iStrikePosX, iStrikePosY);
					if (iPlayerDistanceToStrikePos > 3)
						break;
					if (CALC_VECTOR_DIR(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, iStrikePosX, iStrikePosY) != cd.pi.glbPlayerDir)
						break;
					UPDATE_GLOB_VAR(C065_GLOBAL_BYTE_01_STRIKE_SEEN, 1, C03_GLOBAL_VAR_OP_ADD);
					break;
				} while (--bp0e != 0);
			}
		}
		// show_lightning
		if (bp0e == 0 && RAND01() != 0) {
			xEnvElement->envImg = U8(RAND16(3)) + GFX_ENVIRONMENT_IMG_x64_THUNDER;	// 0x64 = thunder 1 => 0x66 = thunder 3
			if (ENVIRONMENT_GET_CMD_CD_FW(xEnvElement) != 0) {
				bp0a = 1;
				SOMETHING_WITH_SCALED_RECTANGLES(xEnvElement->cmCD, xEnvElement->cmCD + 1, RAND16(100), &xEnvElement->w4, &xEnvElement->w6);
				xEnvElement->cmFW = U8(RAND02());
				xEnvElement++;
			}
		}
		if (bp0a != 0) {
			if (cd.ge.glbThunderJustCast == 0) {
				X16 bp0c;
				if (cd.ge.glbRainStormController == 0) {
					bp0c = RAND16(10) + 5;
				}
				else {
					bp0c = 0x4C - (cd.ge.glbRainStormController / cd.ge.glbRainMultiplicator);
				}
				bp0c = BETWEEN_VALUE(1, bp0c, 15);
				//SPX: Get sound of distant thunder
				QUEUE_NOISE_GEN1(GDAT_CATEGORY_x17_ENVIRONMENT, glbMapGraphicsSet, SOUND_DEFAULT_STORM, 0x19, 0x40, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, bp0c);
				SkD((DLV_DBG_THUNDER, "Thunder at distance %d!\n", bp0c));
				cd.ge.glbThunderJustCast = 1;
			}
			else {
				cd.ge.glbThunderJustCast = 0;
			}
		}
		cd.ge.glbLightningFlash = 1;
		RECALC_LIGHT_LEVEL();
	}
	xEnvElement->envImg = 0xFF;

#if (XDMX_FEATURE_AMBIENT_SOUND == 1)
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

	return;
}


//^3DF7:0037
// SPX: some sort of init, called from read_structure
// _3df7_0037 renamed WEATHER_INIT
void SkWinCore::WEATHER_INIT(X16 iKeepState)
{
	if (iKeepState != 0) {
		cd.ge.glbStormPrevious = 0;
		if (cd.ge.glbRainMultiplicator == 0)
			cd.ge.glbRainMultiplicator = 1;
	}
	else {
		UPDATE_GLOB_VAR(C064_GLOBAL_BYTE_00_STORM_STRONG, 0, C06_GLOBAL_VAR_OP_NEW_VALUE);
		cd.ge.glbRainSpecialNextTick = glbGameTick +5;
		cd.ge.glbRainAmbientLightModifier = 0;
		cd.ge.glbEnvironmentIsOutdoor = 0;
		X32 iTickDelayForRain;	// bp04
		if (cd.ge.glbStartQuickRain != 0) {
			cd.ge.glbRainLevelForGround = 0;
			iTickDelayForRain = RAND16(0x1F4);	// 0x1F4 = 500
			cd.ge.glbRainRelated2 = 3;
			cd.ge.glbRainMultiplicator = 1;
		}
		else {
			iTickDelayForRain = RAND16(0x1F40) +0x1F4;	// RAND16(8000) + 500
			cd.ge.glbRainRelated2 = U8(RAND02());
			cd.ge.glbRainMultiplicator = RAND16(3) +1;
		}
		cd.ge.glbRainLevelForSky = 1;
		cd.ge.glbRainIntensity = 0;
		cd.ge.glbRainStormController = 0;
		cd.ge.glbStormPrevious = 0;
		cd.ge.glbRainDirection = U8(RAND02());
		// SPX: This might be the initial rain timer : NOPE, it must be still called ingame (when there is no rain)
		WEATHER_SET_TIME_EVENT(iTickDelayForRain);
	}
	cd.ge.glbRainThunderRandom = RAND16(3) +2;
	//glbWeatherDayHourLight = tblOutsideLightByHour[RCJ(24,((glbGameTick + _4976_5c24) / 0x555) % 0x18)];
	cd.ge.glbWeatherDayHourLight = tblOutsideLightByHour[WEATHER_GET_HOUR_OF_DAY()];
	cd.ge.glbStartQuickRain = 0;
	return;
}

// SPX: new
void SkWinCore::WEATHER_DEBUG_RESET_STATE(U8 iState)
{
	SkCodeParam::iDebugWeatherState = SkCodeParam::iDebugWeatherState%2;
	if (SkCodeParam::iDebugWeatherState == 0)
	{
		cd.ge.glbRainMultiplicator = 0;
		cd.ge.glbStartQuickRain = 0;
		cd.ge.glbRainDirection = U8(RAND02());
		cd.ge.glbRainLevelForSky = 0;
		cd.ge.glbRainIntensity = 0;
		cd.ge.glbRainStormController = 0;
		cd.ge.glbRainRelated2 = U8(RAND02());
		cd.ge.glbRainAmbientLightModifier = 0;
		UPDATE_GLOB_VAR(C064_GLOBAL_BYTE_00_STORM_STRONG, 0, C06_GLOBAL_VAR_OP_NEW_VALUE);
	}
	else if (SkCodeParam::iDebugWeatherState == 1)
	{
		cd.ge.glbRainMultiplicator = 3;
		cd.ge.glbStartQuickRain = 1;
		cd.ge.glbRainDirection = U8(RAND02());
		cd.ge.glbRainLevelForSky = 4;
		cd.ge.glbRainIntensity = 0xFF;
		cd.ge.glbRainStormController = 255;
		cd.ge.glbRainRelated2 = U8(RAND02());
		cd.ge.glbRainAmbientLightModifier = 0;
		cd.ge.glbRainLevelForGround = 0;
		cd.ge.glbRainSpecialNextTick = glbGameTick + 1;
		UPDATE_GLOB_VAR(C064_GLOBAL_BYTE_00_STORM_STRONG, 100, C06_GLOBAL_VAR_OP_NEW_VALUE);
	}
}


//^3DF7:000A
// SPX: _3df7_000a renamed WEATHER_SET_TIME_EVENT
void SkWinCore::WEATHER_SET_TIME_EVENT(U32 tickDelta)
{
	ENTER(10);

	if (SkCodeParam::bUseDM2ExtendedMode && SkCodeParam::fDebugRainSpeed != 1)
		tickDelta = (U32) ((float) tickDelta / (float) SkCodeParam::fDebugRainSpeed);

	SkD((DLV_DBG_RAIN, "Rain timer duration = %03d (will hit at %d)\n"
		, tickDelta
		, glbGameTick +tickDelta
		));

	Timer tTimerWeather;	// bp0a
	tTimerWeather.SetMap(0);
	tTimerWeather.SetTick(glbGameTick + tickDelta);
	tTimerWeather.TimerType(C84_TIME_EVENT_WEATHER);
	tTimerWeather.actor = TIMER_ACTOR__00;
	QUEUE_TIMER(&tTimerWeather);
	return;
}