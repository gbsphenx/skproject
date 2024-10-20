#include <StdAfx.h>	// Required for MVC6 compilation

#include <SKVER.H>
#include <conio.h> // getch

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
#endif // __DJGPP__


#ifdef __USE_ALLEGRO__
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#endif // __USE_ALLEGRO__

#include <KAITABLE.h>	// for dAITableGenuine (hard coded AI table)
//--- Common part with A.cpp

#if DM2_EXTENDED_MODE == 1
	#define CREATURE_AI_TAB_SIZE	64
#else
	#define CREATURE_AI_TAB_SIZE	42
#endif // DM2_EXTENDED_MODE

#ifdef __DJGPP__
#include <strings.h>
#include <time.h>
#include <stdlib.h>


#undef inportb
#undef outportb
#include <dos.h>

#ifndef _stricmp
#define _stricmp strcasecmp
#endif // _stricmp

#ifndef _exit
#define _exit exit
#endif // _exit

int getdrive()
{
	return 3; // C
}

#ifndef _getdrive
#define _getdrive getdrive
#endif // _getdrive

#define min(A,B) ((A < B) ? A : B)
#define max(A,B) ((A < B) ? B : A)

#endif // __DJGPP__


// SPX: Functions supposed for MIDI playing from SKWIN_DM2_PCDOS
/*
#define MIDIPATHNAME "./DATA/%02x.hmp.mid"

void c_midi::do_music(i16 nr)
{
  char songname[30];
  sprintf(songname, MIDIPATHNAME, nr);
  thesong = load_midi(songname);
  if (thesong)
    play_midi(thesong, true); // true means: looped
}

void c_midi::stop_music(void)
{
  if (thesong)
  {
    destroy_midi(thesong);
    thesong = NULL;
  }
}

void c_midi::set_midi_volume(i16 v)
{
  set_volume(-1, v);
}

void c_midi::init_midi(void)
{
  allegro_init();
  if (install_sound(-1, MIDI_AUTODETECT, NULL) != 0)
    throw(THROW_DMABORT);
  set_midi_volume(128);
}

static void R_4FF39(x16 eaxw)
{
  if (eaxw < con(0x8))
  {
    ddata.v1da374[eaxw].w4 = con(0x0);
    ddata.v1da374[eaxw].l0 = con(0x0);
  }
}

static void DM2DOS_R_B65(void)
{
  if (ddata.v1d14c2 && ddata.v1d14d0)
  {
    // R_50012(ddata.v1dff2c); // DM's MIDI extracted
    DM2DOS_R_4FF39(ddata.v1dff2c);
    ddata.v1d14d0 = false;
    ddata.v1d14cc = false;
    ddata.v1d14ca = con(0x0);
  }
}

static void DM2DOS_R_A0E(x8 eaxb)
{
  x8 vb_00;

  vb_00 = eaxb;
  if (ddata.v1d14c2 && vb_00 != 0 && ddata.v1dff86 > con(0x0))
  {
    DM2DOS_R_B65();
    if (dm2sound.sndptr6 == NULL)
      return;
    x32 longrg1 = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0x4), CUTLX8(con(0x3)), con(0x0), vb_00) ? 1 : 0;
#if 1 // TODO
    dm2sound.stop_music();
#endif
    if (longrg1 == con(0x0))
      return;
#if 1 // TODO
    dm2sound.do_music(unsignedword(vb_00));
    return;
#else
  $  x32 longrg6 = unsignedlong(SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(con(0x4), con(0x3), con(0x0), vb_00));
  $  SKW_COPY_MEMORY(SKW_QUERY_GDAT_ENTRY_DATA_PTR(con(0x4), con(0x3), con(0x0), vb_00), longrg6, dm2sound.sndptr6);
  $  ddata.v1dfea8 = DSZERO;
  $  ddata.v1dfea4 = dm2sound.sndptr6;
  $  ddata.v1dfeb0 = con(0x0);
  $  ddata.v1dfeac = con(0x0);
  $  parl01 = DSZERO;
  $  parp00 = &ddata.v1dff2c;
  $  RG2P = ddata.v1d1378;
  $  RG1P = &ddata.v1dfea4;
  $  RG1L = con(0x0); // was calling R_4F75E, result 0x0 or 0xe
  $  ddata.v1dff34 = RG1L;
  $  if (RG1L != con(0x0)) return;
  $  RG1L = ddata.v1dff2c;
  $  RG1L = con(0x0); // TODO old call of R_4FF83, result can be con(0xb) to, removed, MIDI!!
  $  ddata.v1dff34 = RG1L;
  $  if (RG1L != con(0x0)) return;
  $  RG1L = ddata.v1dff2c;
  $  // R_520C3(RG1L, con(0x7f)); // DM's MIDI stuff extracted
  $  RG1L = ddata.v1dff2c;
  $  // R_51F94(RG1L); // DM's MIDI stuff extracted
  $  ddata.v1d14d4 = true;
  $  ddata.v1d14d0 = true;
#endif
  }
//M_B27:
#if 0
$  ddata.v1d14d8 = unsignedword(vb_00);
#endif
//M_A05:
}

void DM2DOS_R_BA7(x16 eaxw)
{
  if (!ddata.v1d14c2 || !ddata.v1d14da)
    return;
  if (ddata.v1d14cc)
  {
    // DM2DOS_R_51D42(ddata.v1dff2c);  DM's MIDI stuff extracted. TODO: check what it wants to do here
    ddata.v1d14cc = false;
  }
  x32 longrg1 = signedlong(ddata.v1d14ca);
  if (longrg1 <= con(0x1))
  {
    if (longrg1 == con(0x1))
    {
      ddata.v1d1512 = ddata.v1dff8a;
      DM2DOS_R_A0E(ddata.v1d1512);
      ddata.v1d14ca = con(0x0);
      return;
    }
  }
  else
  {
    // DM2DOS_R_520C3(ddata.v1dff2c, unsignedlong(CUTX8(ddata.v1d14ca)); // DM's MIDI stuff extracted TODO: check what it wants to do here
    ddata.v1d14ca--;
  }
  ddata.v1dff8a = table1410ec[eaxw];
  if (ddata.v1d1512 != ddata.v1dff8a)
  {
    if (!ddata.v1d14d0 || ddata.v1d14ca != con(0x0))
      DM2DOS_R_A0E(ddata.v1dff8a);
    else
      ddata.v1d14ca = con(0x7f);
    ddata.v1d1512 = ddata.v1dff8a;
  }
}
*/

// SPX: New procedures here

const X8* SkWinCore::GET_DATA_FOLDER_ZNAME(i16 predefinedGameID)
{
	const X8* sFolderData = NULL;

	if (skwin.sCustomDataFolder != NULL && skwin.sCustomDataFolder[0])
		return (const X8*) skwin.sCustomDataFolder;

	switch(predefinedGameID)
	{
		case _OPTION_DUNGEON_DM1_KID_:
			sFolderData = (const X8*) ".Z008DATA1KID\\"; break;
		case _OPTION_DUNGEON_DM1_DM_:
			sFolderData = (const X8*) ".Z008DATA1DM\\"; break;
		case _OPTION_DUNGEON_DM1_CSB_:
			sFolderData = (const X8*) ".Z008DATA1CSB\\"; break;
		case _OPTION_DUNGEON_DM1_TQ_:
			sFolderData = (const X8*) ".Z008DATA1TQ\\"; break;
		case _OPTION_DUNGEON_DM2_DM_:
			sFolderData = (const X8*) ".Z008DATA2DM\\"; break;
		case _OPTION_DUNGEON_DM2_CSB_:
			sFolderData = (const X8*) ".Z008DATA2CSB\\"; break;
		case _OPTION_DUNGEON_DM2_TQ_:
			sFolderData = (const X8*) ".Z008DATA2TQ\\"; break;
		case _OPTION_DUNGEON_DM2_SK_BETA_:
			sFolderData = (const X8*) ".Z008DATA2SKB\\"; break;
		case _OPTION_DUNGEON_DM2_SK_DEMO_:
			sFolderData = (const X8*) ".Z008DATA2SKD\\"; break;
		case _OPTION_DUNGEON_DM2_SK_:
			sFolderData = (const X8*) ".Z008DATA2SK\\"; break;
		case _OPTION_DUNGEON_DM2_SK_EXT_:
			sFolderData = (const X8*) ".Z008DATA2TOR\\"; break;
		case _OPTION_DUNGEON_BWY_BW_:
			sFolderData = (const X8*) ".Z008DATAXBWY\\"; break;
		case _OPTION_DUNGEON_EOB_EOB1_:
			sFolderData = (const X8*) ".Z008DATAXEOB\\"; break;
		case _OPTION_DUNGEON_DMX_:
			sFolderData = (const X8*) ".Z008DATAX\\"; break;
		case _OPTION_DUNGEON_NO_SPECIFIC_:
		default:
			sFolderData = (const X8*) ".Z008DATA\\"; break;
	}
	return sFolderData;
}


const X8* SkWinCore::GET_DATA_FOLDER_NAME(i16 predefinedGameID)
{
	const X8* sFolderData = NULL;

	if (skwin.sCustomDataFolder != NULL && skwin.sCustomDataFolder[0])
		return (const X8*) skwin.sCustomDataFolder;
	
	switch(predefinedGameID)
	{
		case _OPTION_DUNGEON_DM1_KID_:
			sFolderData = (const X8*) "DATA1KID\\"; break;
		case _OPTION_DUNGEON_DM1_DM_:
			sFolderData = (const X8*) "DATA1DM\\"; break;
		case _OPTION_DUNGEON_DM1_CSB_:
			sFolderData = (const X8*) "DATA1CSB\\"; break;
		case _OPTION_DUNGEON_DM1_TQ_:
			sFolderData = (const X8*) "DATA1TQ\\"; break;
		case _OPTION_DUNGEON_DM2_DM_:
			sFolderData = (const X8*) "DATA2DM\\"; break;
		case _OPTION_DUNGEON_DM2_CSB_:
			sFolderData = (const X8*) "DATA2CSB\\"; break;
		case _OPTION_DUNGEON_DM2_TQ_:
			sFolderData = (const X8*) "DATA2TQ\\"; break;
		case _OPTION_DUNGEON_DM2_SK_BETA_:
			sFolderData = (const X8*) "DATA2SKB\\"; break;
		case _OPTION_DUNGEON_DM2_SK_DEMO_:
			sFolderData = (const X8*) "DATA2SKD\\"; break;
		case _OPTION_DUNGEON_DM2_SK_:
			sFolderData = (const X8*) "DATA2SK\\"; break;
		case _OPTION_DUNGEON_DM2_SK_EXT_:
			sFolderData = (const X8*) "DATA2TOR\\"; break;
		case _OPTION_DUNGEON_BWY_BW_:
			sFolderData = (const X8*) "DATAXBWY\\"; break;
		case _OPTION_DUNGEON_EOB_EOB1_:
			sFolderData = (const X8*) "DATAXEOB\\"; break;
		case _OPTION_DUNGEON_DMX_:
			sFolderData = (const X8*) "DATAX\\"; break;
		case _OPTION_DUNGEON_NO_SPECIFIC_:
		default:
			sFolderData = (const X8*) "DATA\\"; break;
	}
	return sFolderData;
}


X16 SkWinCore::EXTENDED_LOAD_AI_DEFINITION(void)
{
	int rc = 0;
	U8 index = 0;
	// SPX: If not extended, load the default table from static data / OR if dungeon selected is skullkeep (security because AI is not in GDAT currently)
	//if (!SkCodeParam::bUseDM2ExtendedMode || skwin.dung == _OPTION_DUNGEON_DM2_BETA_ || skwin.dung == _OPTION_DUNGEON_DM2_DEMO_ || skwin.dung == _OPTION_DUNGEON_DM2_SK)
	// 2023-07-26 : removed the test : in all case, first setup the AI table with default DM2 values, then overwrite with what's from GDAT 
	{
		//&_4976_03a2[res * 0x0024]
		//memcpy(dAITable, _4976_03a2, 62 * 36);
		memcpy(dAITable, dAITableGenuine, MAXAI * 36);

		//int r = memcmp(_4976_03a2, dAITableGenuine, 36 * 62);
		//ATLASSERT(r == 0);
		rc = 0;

		// SPX: Add the cast FIREBALL to the Amplifier AI
		if (SkCodeParam::bUseFixedMode)
		{
			dAITable[51].AttacksSpells |= AI_ATTACK_FLAGS__FIREBALL;
			//dAITable[51].w0 = 0x40;
			// Amplifier must remain static object, or it loses its moveable ability.
		}

	}
	if (SkCodeParam::bUseDM2ExtendedMode)
	{
		U16 value = 0;
		U8 category = GDAT_CATEGORY_CREATURE_AI;
		Bit8u text[128] = {0};

		// Do the default init -- until all data is put into GDAT.
		//memcpy(dAITable, _4976_03a2, 62 * 36);
		//memcpy(dAITable, dAITableGenuine, 62 * 36);

		for (index = 0; index < 254; index++)
		{
			// SPX : TODO => COMPLETE LOADING OF VALUES HERE
			value = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, GDAT_AI_STAT_HIT_POINTS);	// Test HP
			if (value != 0)	
			{
				U8	byte1;
				U8	byte2;
				QUERY_GDAT_TEXT(GDAT_CATEGORY_CREATURE_AI
								,index
								,0x18
								,text);
				// Bunch load
				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 0);
				byte2 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 1);
				dAITable[index].w0AIFlags = byte1 + byte2*256;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 2);
				dAITable[index].ArmorClass = byte1;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 3);
				dAITable[index].b3 = byte1;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 4);
				byte2 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 5);
				dAITable[index].BaseHP = byte1 + byte2*256;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 6);
				dAITable[index].AttackStrength = byte1;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 7);
				dAITable[index].PoisonDamage = byte1;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 8);
				dAITable[index].Defense = byte1;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 9);
				dAITable[index].b9x = byte1;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 10);
				byte2 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 11);
				dAITable[index].w10 = byte1 + byte2*256;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 12);
				byte2 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 13);
				dAITable[index].w12 = byte1 + byte2*256;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 14);
				byte2 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 15);
				dAITable[index].AttacksSpells = byte1 + byte2*256;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 16);
				byte2 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 17);
				dAITable[index].w16 = byte1 + byte2*256;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 18);
				byte2 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 19);
				dAITable[index].w18 = byte1 + byte2*256;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 20);
				byte2 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 21);
				dAITable[index].w20 = byte1 + byte2*256;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 22);
				byte2 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 23);
				dAITable[index].w22 = byte1 + byte2*256;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 24);
				byte2 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 25);
				dAITable[index].w24 = byte1 + byte2*256;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 26);
				byte2 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 27);
				dAITable[index].w26 = byte1 + byte2*256;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 28);
				dAITable[index].b28 = byte1;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 29);
				dAITable[index].Weight = byte1;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 30);
				byte2 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 31);
				dAITable[index].w30 = byte1 + byte2*256;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 32);
				byte2 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 33);
				dAITable[index].w32 = byte1 + byte2*256;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 34);
				dAITable[index].b34 = byte1;

				byte1 = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, 35);
				dAITable[index].b35 = byte1;

				// Flags (w0)
				dAITable[index].ArmorClass = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, GDAT_AI_STAT_ARMOR_CLASS);
				//b3
				dAITable[index].BaseHP = value;

				// Is there more than just attack here ?
				dAITable[index].AttackStrength = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, GDAT_AI_STAT_ATTACK_STRENGTH);

				dAITable[index].PoisonDamage = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, GDAT_AI_STAT_ATTACK_POISON);
				dAITable[index].Defense = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, GDAT_AI_STAT_DEFENSE);
				//b9
				//dAITable[index].b9x = QUERY_GDAT_ENTRY_DATA_INDEX(category, index, dtWordValue, GDAT_AI_STAT_FLAGS_B9);
				// attack commands & spell commands
				// can switch triggers
				// fire resistance
				// poison resistance
				// weight (push resistance)
				SkD((DLV_TWEET, "Tweet: Loading AI %d (%02X) named %s (hp:%d, ac:%d, def:%d, str:%d, ps:%d)\n", index, index
					,text
					,dAITable[index].BaseHP
					,dAITable[index].ArmorClass
					,dAITable[index].Defense
					,dAITable[index].AttackStrength
					,dAITable[index].PoisonDamage));
			}
		}
		rc = 1;
	}

	//--- Write info about AI values
	if (0) // write log or not
	{
		Write2LOGX("CREATURE/OBJECT AI INFO:\nNumber of AI : %d", MAXAI);
		for (index = 0; index < MAXAI; index++)
		{
			Write2LOGX("#%03d) <%24s>\n\tHit Points: %4d",
				index,
				getAIName(index),
				dAITable[index].BaseHP);
			if (dAITable[index].ArmorClass == 255)
				Write2LOGX("\tArmor: Indestructible (255)");	
			else
				Write2LOGX("\tArmor: %d", dAITable[index].ArmorClass);	
			if (dAITable[index].Defense != 0)
				Write2LOGX("\tDefense: %d", dAITable[index].Defense);	
			if (dAITable[index].AttackStrength != 0)
				Write2LOGX("\tAttack Strength: %d", dAITable[index].AttackStrength);	
			if (dAITable[index].PoisonDamage != 0)
				Write2LOGX("\tPoison Damage: %d", dAITable[index].PoisonDamage);	

			if (dAITable[index].AttacksSpells != 0)
			{
				if (dAITable[index].AttacksSpells & AI_ATTACK_FLAGS__PUSH_BACK)
					Write2LOGX("\tCan knock back.");	
				if (dAITable[index].AttacksSpells & AI_ATTACK_FLAGS__MELEE)
					Write2LOGX("\tCan do melee attack.");	
				if (dAITable[index].AttacksSpells & AI_ATTACK_FLAGS__STEAL)
					Write2LOGX("\tCan steal.");	
				if (dAITable[index].AttacksSpells & AI_ATTACK_FLAGS__FIREBALL)
					Write2LOGX("\tCan cast Fireball.");	
				if (dAITable[index].AttacksSpells & AI_ATTACK_FLAGS__DISPELL)
					Write2LOGX("\tCan cast Dispell.");	
				if (dAITable[index].AttacksSpells & AI_ATTACK_FLAGS__LIGHTNING)
					Write2LOGX("\tCan cast Lightning.");	
				if (dAITable[index].AttacksSpells & AI_ATTACK_FLAGS__POISON_CLOUD)
					Write2LOGX("\tCan cast Poison Cloud.");	
				if (dAITable[index].AttacksSpells & AI_ATTACK_FLAGS__POISON_BOLT)
					Write2LOGX("\tCan cast Poison Bolt.");	
				if (dAITable[index].AttacksSpells & AI_ATTACK_FLAGS__PUSH_SPELL)
					Write2LOGX("\tCan cast Push.");	
				if (dAITable[index].AttacksSpells & AI_ATTACK_FLAGS__PULL_SPELL)
					Write2LOGX("\tCan cast Pull.");	
				if (dAITable[index].AttacksSpells & AI_ATTACK_FLAGS__POISON_BLOB)
					Write2LOGX("\tCan can Poison Blob.");	
				if (dAITable[index].AttacksSpells & AI_ATTACK_FLAGS__SHOOT)
					Write2LOGX("\tCan shoot items.");	
			}


			Write2LOGX("\n");
		}
	}

	return rc;
}
// SPX: End of new procedures

//{{DEBUG_HELPER
const char *SkWinCore::getXActrName(int x)
{
	if (x == xactrNeedReset) return "xactrNeedReset";
	if (x == xactrYes) return "xactrYes";
	if (x == xactrNo) return "xactrNo";
	if (x == xactrAgain) return "xactrAgain";
	return "?";
}

// SPX: Name of spell type
CString SkWinCore::getSpellTypeName(U8 spelltype)
{
	Bit8u text[128] = {0};
	if (spelltype == SPELL_TYPE_POTION) return "POTION";
	if (spelltype == SPELL_TYPE_MISSILE) return "MISSILE";
	if (spelltype == SPELL_TYPE_GENERAL) return "ENCHANTMENT";
	if (spelltype == SPELL_TYPE_SUMMON) return "SUMMON";
	if (text[0] != 0)
		return (LPCSTR)text;
	return "Unknown";
}

// SPX: Name of skill
CString SkWinCore::getSkillName(U8 skill)
{
	Bit8u text[128] = {0};
	if (skill == SKILL_FIGHTER_GLOBAL) return "FIGHTER";
	if (skill == SKILL_NINJA_GLOBAL) return "NINJA";
	if (skill == SKILL_PRIEST_GLOBAL) return "PRIEST";
	if (skill == SKILL_WIZARD_GLOBAL) return "WIZARD";
	if (skill == SKILL_FIGHTER_SWING) return "(F1)SWING";
	if (skill == SKILL_FIGHTER_THRUST) return "(F2)THRUST";
	if (skill == SKILL_FIGHTER_BLUNT) return "(F3)BLUNT";
	if (skill == SKILL_FIGHTER_PARRY) return "(F4)PARRY";
	if (skill == SKILL_NINJA_STEAL) return "(N1)STEAL";
	if (skill == SKILL_NINJA_FIGHT) return "(N2)FIGHT";
	if (skill == SKILL_NINJA_THROW) return "(N3)THROW";
	if (skill == SKILL_NINJA_SHOOT) return "(N4)SHOOT";
	if (skill == SKILL_PRIEST_IDENTIFY) return "(P1)IDENTIFY";
	if (skill == SKILL_PRIEST_HEAL) return "(P2)HEAL";
	if (skill == SKILL_PRIEST_INFLUENCE) return "(P3)INFLUENCE";
	if (skill == SKILL_PRIEST_DEFEND) return "(P4)DEFEND";
	if (skill == SKILL_WIZARD_FIRE) return "(W1)FIRE";
	if (skill == SKILL_WIZARD_AIR) return "(W2)AIR";
	if (skill == SKILL_WIZARD_EARTH) return "(W3)EARTH";
	if (skill == SKILL_WIZARD_WATER) return "(W4)WATER";
	if (text[0] != 0)
		return (LPCSTR)text;
	return "UNKNOWN";
}

// SPX: Name of bonus
CString SkWinCore::getStatBonusName(U8 bonus)
{
	Bit8u text[128] = {0};
	if (bonus == GDAT_ITEM_STATS_WEIGHT)			return "WEIGHT";
	if (bonus == GDAT_ITEM_STATS_MONEY_VALUE)		return "MONEY";
	if (bonus == GDAT_ITEM_STATS_FOOD_VALUE)		return "FOOD";
	if (bonus == GDAT_ITEM_WEAPON_MELEE_STRENGTH)	return "MELEE STRENGTH";
	if (bonus == GDAT_ITEM_WEAPON_THROW_STRENGTH)	return "THROW STRENGTH";
	if (bonus == GDAT_ITEM_WEAPON_ACCURACY)			return "ACCURACY";
	if (bonus == GDAT_ITEM_STATS_ARMOR_CLASS)		return "ARMOR CLASS";
	if (bonus == GDAT_ITEM_STATS_0x0C)				return "X0C";
	if (bonus == GDAT_ITEM_STATS_POISONOUS)			return "POISON";
	if (bonus == GDAT_ITEM_BONUS_0x13)				return "X13";
	if (bonus == GDAT_ITEM_BONUS_MANA)				return "MANA";
	if (bonus == GDAT_ITEM_BONUS_LUCK)				return "LUCK";
	if (bonus == GDAT_ITEM_BONUS_STRENGTH)			return "STRENGTH";
	if (bonus == GDAT_ITEM_BONUS_DEXTERITY)			return "DEXTERITY";
	if (bonus == GDAT_ITEM_BONUS_WISDOM)			return "WISDOM";
	if (bonus == GDAT_ITEM_BONUS_VITALITY)			return "VITALITY";
	if (bonus == GDAT_ITEM_BONUS_ANTI_MAGIC)		return "ANTI-MAGIC";
	if (bonus == GDAT_ITEM_BONUS_ANTI_FIRE)			return "ANTI-FIRE";

	if (bonus == GDAT_ITEM_BONUS_FIGHTER)			return "FIGHTER";
	if (bonus == GDAT_ITEM_BONUS_NINJA)				return "NINJA";
	if (bonus == GDAT_ITEM_BONUS_PRIEST)			return "PRIEST";
	if (bonus == GDAT_ITEM_BONUS_WIZARD)			return "WIZARD";
	if (bonus == GDAT_ITEM_BONUS_FGT_SWING)			return "(F1)SWING";
	if (bonus == GDAT_ITEM_BONUS_FGT_THRUST)		return "(F2)THRUST";
	if (bonus == GDAT_ITEM_BONUS_FGT_BLUNT)			return "(F3)BLUNT";
	if (bonus == GDAT_ITEM_BONUS_FGT_PARRY)			return "(F4)PARRY";
	if (bonus == GDAT_ITEM_BONUS_NJA_STEAL)			return "(N1)STEAL";
	if (bonus == GDAT_ITEM_BONUS_NJA_FIGHT)			return "(N2)FIGHT";
	if (bonus == GDAT_ITEM_BONUS_NJA_THROW)			return "(N3)THROW";
	if (bonus == GDAT_ITEM_BONUS_NJA_SHOOT)			return "(N4)SHOOT";
	if (bonus == GDAT_ITEM_BONUS_PRS_IDENTIFY)		return "(P1)IDENTIFY";
	if (bonus == GDAT_ITEM_BONUS_PRS_HEAL)			return "(P2)HEAL";
	if (bonus == GDAT_ITEM_BONUS_PRS_INFLUENCE)		return "(P3)INFLUENCE";
	if (bonus == GDAT_ITEM_BONUS_PRS_DEFEND)		return "(P4)DEFEND";
	if (bonus == GDAT_ITEM_BONUS_WIZ_FIRE)			return "(W1)FIRE";
	if (bonus == GDAT_ITEM_BONUS_WIZ_AIR)			return "(W2)AIR";
	if (bonus == GDAT_ITEM_BONUS_WIZ_EARTH)			return "(W3)EARTH";
	if (bonus == GDAT_ITEM_BONUS_WIZ_WATER)			return "(W4)WATER";

	if (bonus == GDAT_ITEM_BONUS_LIGHT)				return "LIGHT";
	if (bonus == GDAT_ITEM_BONUS_WALK_SPEED)		return "SPEED";
	if (bonus == GDAT_ITEM_STATS_MAX_CHARGES)		return "MAX CHARGES";
	if (bonus == GDAT_ITEM_BONUS_MONEY_PER_CHARGE)	return "MONEY/CHARGE";
	if (bonus == GDAT_ITEM_STATS_WATER_VALUE)		return "WATER";
	if (bonus == GDAT_ITEM_STATS_MISSILE_SPELL)		return "SPELL";
	if (text[0] != 0)
		return (LPCSTR)text;
	return "UNKNOWN";
}

// SPX: Name of UI Event
CString SkWinCore::getUIEventName(U8 event)
{
	Bit8u text[128] = {0};

// 0x00
	if (event == UI_EVENTCODE_TURN_LEFT) return "TURN LEFT";
	if (event == UI_EVENTCODE_TURN_RIGHT) return "TURN RIGHT";
	if (event == UI_EVENTCODE_MOVE_FORWARD) return "MOVE FORWARD";
	if (event == UI_EVENTCODE_MOVE_RIGHT) return "MOVE RIGHT";
	if (event == UI_EVENTCODE_MOVE_BACK) return "MOVE BACK";
	if (event == UI_EVENTCODE_MOVE_LEFT) return "MOVE LEFT";

	if (event == UI_EVENTCODE_VIEW_CHAMPION_1) return "CHAMPION INVENTORY 1";
	if (event == UI_EVENTCODE_VIEW_CHAMPION_2) return "CHAMPION INVENTORY 2";
	if (event == UI_EVENTCODE_VIEW_CHAMPION_3) return "CHAMPION INVENTORY 3";
	if (event == UI_EVENTCODE_VIEW_CHAMPION_4) return "CHAMPION INVENTORY 4";
	if (event == UI_EVENTCODE_RETURN_VIEWPORT) return "BACK TO VIEWPORT";

// 0x10
	if (event == UI_EVENTCODE_CHAMPION_1_HAND_RIGHT) return "CHAMPION 1 RIGHT HAND";
	if (event == UI_EVENTCODE_CHAMPION_1_HAND_LEFT) return "CHAMPION 1 LEFT HAND";
	if (event == UI_EVENTCODE_CHAMPION_2_HAND_RIGHT) return "CHAMPION 2 RIGHT HAND";
	if (event == UI_EVENTCODE_CHAMPION_2_HAND_LEFT) return "CHAMPION 2 LEFT HAND";
	if (event == UI_EVENTCODE_CHAMPION_3_HAND_RIGHT) return "CHAMPION 3 RIGHT HAND";
	if (event == UI_EVENTCODE_CHAMPION_3_HAND_LEFT) return "CHAMPION 3 LEFT HAND";
	if (event == UI_EVENTCODE_CHAMPION_4_HAND_RIGHT) return "CHAMPION 4 RIGHT HAND";
	if (event == UI_EVENTCODE_CHAMPION_4_HAND_LEFT) return "CHAMPION 4 LEFT HAND";

	if (event == UI_EVENTCODE_INVENTORY_HAND_RIGHT) return "INVENTORY RIGHT HAND";
	if (event == UI_EVENTCODE_INVENTORY_HAND_LEFT) return "INVENTORY LEFT HAND";

	if (event == UI_EVENTCODE_INVENTORY_HEAD) return "INVENTORY HEAD";
	if (event == UI_EVENTCODE_INVENTORY_BODY) return "INVENTORY BODY";

// 0x20 - 0x30
	if (event == UI_EVENTCODE_INVENTORY_LEGS) return "INVENTORY LEGS";
	if (event == UI_EVENTCODE_INVENTORY_FOOT) return "INVENTORY FOOT";
	if (event == UI_EVENTCODE_INVENTORY_POUCH_2) return "INVENTORY POUCH 2";
	if (event == UI_EVENTCODE_INVENTORY_SCABBARD_2) return "INVENTORY SCABBARD 2";
	if (event == UI_EVENTCODE_INVENTORY_SCABBARD_3) return "INVENTORY SCABBARD 3";
	if (event == UI_EVENTCODE_INVENTORY_SCABBARD_4) return "INVENTORY SCABBARD 4";

	if (event == UI_EVENTCODE_INVENTORY_NECK) return "INVENTORY NECK";
	if (event == UI_EVENTCODE_INVENTORY_POUCH_1) return "INVENTORY POUCH 1";
	if (event == UI_EVENTCODE_INVENTORY_SCABBARD_1) return "INVENTORY SCABBARD 1";

	if (event == UI_EVENTCODE_BACKPACK_01) return "INVENTORY BACKPACK 1";
	if (event == UI_EVENTCODE_BACKPACK_02) return "INVENTORY BACKPACK 2";
	if (event == UI_EVENTCODE_BACKPACK_03) return "INVENTORY BACKPACK 3";
	if (event == UI_EVENTCODE_BACKPACK_04) return "INVENTORY BACKPACK 4";
	if (event == UI_EVENTCODE_BACKPACK_05) return "INVENTORY BACKPACK 5";
	if (event == UI_EVENTCODE_BACKPACK_06) return "INVENTORY BACKPACK 6";
	if (event == UI_EVENTCODE_BACKPACK_07) return "INVENTORY BACKPACK 7";
	if (event == UI_EVENTCODE_BACKPACK_08) return "INVENTORY BACKPACK 8";
	if (event == UI_EVENTCODE_BACKPACK_09) return "INVENTORY BACKPACK 9";
	if (event == UI_EVENTCODE_BACKPACK_10) return "INVENTORY BACKPACK 10";
	if (event == UI_EVENTCODE_BACKPACK_11) return "INVENTORY BACKPACK 11";
	if (event == UI_EVENTCODE_BACKPACK_12) return "INVENTORY BACKPACK 12";
	if (event == UI_EVENTCODE_BACKPACK_13) return "INVENTORY BACKPACK 13";
	if (event == UI_EVENTCODE_BACKPACK_14) return "INVENTORY BACKPACK 14";
	if (event == UI_EVENTCODE_BACKPACK_15) return "INVENTORY BACKPACK 15";
	if (event == UI_EVENTCODE_BACKPACK_16) return "INVENTORY BACKPACK 16";
	if (event == UI_EVENTCODE_BACKPACK_17) return "INVENTORY BACKPACK 17";

	if (event == UI_EVENTCODE_CONTAINER_1) return "CONTAINER SLOT 1";
	if (event == UI_EVENTCODE_CONTAINER_2) return "CONTAINER SLOT 2";
	if (event == UI_EVENTCODE_CONTAINER_3) return "CONTAINER SLOT 3";
	if (event == UI_EVENTCODE_CONTAINER_4) return "CONTAINER SLOT 4";
	if (event == UI_EVENTCODE_CONTAINER_5) return "CONTAINER SLOT 5";
	if (event == UI_EVENTCODE_CONTAINER_6) return "CONTAINER SLOT 6";
	if (event == UI_EVENTCODE_CONTAINER_7) return "CONTAINER SLOT 7";
	if (event == UI_EVENTCODE_CONTAINER_8) return "CONTAINER SLOT 8";



// 0x40

	if (event == UI_EVENTCODE_MOUTH) return "INVENTORY MOUTH";
	if (event == UI_EVENTCODE_EYE) return "INVENTORY EYE";


// 0x50
	if (event == UI_EVENTCODE_CLICK_VIEWPORT) return "CLICK ON VIEWPORT";
	if (event == UI_EVENTCODE_VIEW_LEADER) return "LEADER INVENTORY";

	if (event == UI_EVENTCODE_CHAMPION_TURN_LEFT) return "CHAMPION TURN LEFT";
	if (event == UI_EVENTCODE_CHAMPION_TURN_RIGHT) return "CHAMPION TURN RIGHT";

// 0x60

	if (event == UI_EVENTCODE_SPELL_OR_LEADER_1) return "CHOOSE LEADER / INVOKE SPELL 1";
	if (event == UI_EVENTCODE_SPELL_OR_LEADER_2) return "CHOOSE LEADER / INVOKE SPELL 2";
	if (event == UI_EVENTCODE_SPELL_OR_LEADER_3) return "CHOOSE LEADER / INVOKE SPELL 3";
	if (event == UI_EVENTCODE_SPELL_OR_LEADER_4) return "CHOOSE LEADER / INVOKE SPELL 4";

	
	
	if (event == UI_EVENTCODE_ADD_RUNE_1) return "ADD RUNE 1";
	if (event == UI_EVENTCODE_ADD_RUNE_2) return "ADD RUNE 2";
	if (event == UI_EVENTCODE_ADD_RUNE_3) return "ADD RUNE 3";
	if (event == UI_EVENTCODE_ADD_RUNE_4) return "ADD RUNE 4";
	if (event == UI_EVENTCODE_ADD_RUNE_5) return "ADD RUNE 5";
	if (event == UI_EVENTCODE_ADD_RUNE_6) return "ADD RUNE 6";
	if (event == UI_EVENTCODE_REMOVE_RUNE) return "REMOVE RUNE";
	if (event == UI_EVENTCODE_VALIDATE_SPELL) return "VALIDATE SPELL";

// 0x70
	if (event == UI_EVENTCODE_HAND_RUNE_QUIT) return "HAND / RUNE QUIT ACTION";
	if (event == UI_EVENTCODE_HAND_ACTION_1) return "HAND ACTION 1";
	if (event == UI_EVENTCODE_HAND_ACTION_2) return "HAND ACTION 2";
	if (event == UI_EVENTCODE_HAND_ACTION_3) return "HAND ACTION 3";
	if (event == UI_EVENTCODE_CHAMPION1_ACTION_HAND_RIGHT) return "ACTIVATE CHAMPION 1 RIGHT HAND";
	if (event == UI_EVENTCODE_CHAMPION1_ACTION_HAND_LEFT) return "ACTIVATE CHAMPION 1 LEFT HAND";
	if (event == UI_EVENTCODE_CHAMPION2_ACTION_HAND_RIGHT) return "ACTIVATE CHAMPION 2 RIGHT HAND";
	if (event == UI_EVENTCODE_CHAMPION2_ACTION_HAND_LEFT) return "ACTIVATE CHAMPION 2 LEFT HAND";
	if (event == UI_EVENTCODE_CHAMPION3_ACTION_HAND_RIGHT) return "ACTIVATE CHAMPION 3 RIGHT HAND";
	if (event == UI_EVENTCODE_CHAMPION3_ACTION_HAND_LEFT) return "ACTIVATE CHAMPION 3 LEFT HAND";
	if (event == UI_EVENTCODE_CHAMPION4_ACTION_HAND_RIGHT) return "ACTIVATE CHAMPION 4 RIGHT HAND";
	if (event == UI_EVENTCODE_CHAMPION4_ACTION_HAND_LEFT) return "ACTIVATE CHAMPION 4 LEFT HAND";

	if (event == UI_EVENTCODE_TAKE_CHAMPION_TOP_LEFT) return "SELECTION TOP LEFT CHAMPION";
	if (event == UI_EVENTCODE_TAKE_CHAMPION_TOP_RIGHT) return "SELECTION TOP RIGHT CHAMPION";
	if (event == UI_EVENTCODE_TAKE_CHAMPION_BOTTOM_RIGHT) return "SELECTION BOTTOM RIGHT CHAMPION";
	if (event == UI_EVENTCODE_TAKE_CHAMPION_BOTTOM_LEFT) return "SELECTION BOTTOM LEFT CHAMPION";
	if (event == UI_EVENTCODE_UPDATE_CHAMPION_POSITION) return "UPDATE CHAMPION POSITION";

// 0x80
	if (event == UI_EVENTCODE_DISK_OP) return "DISK OPERATION";

// 0x90
	if (event == UI_EVENTCODE_SLEEP) return "INVOKE SLEEP";
	if (event == UI_EVENTCODE_WAKE) return "WAKE UP FROM SLEEP";
	if (event == UI_EVENTCODE_PAUSE) return "INVOKE PAUSE";
	if (event == UI_EVENTCODE_END_PAUSE) return "RESUME GAME FROM PAUSE";

	if (event == UI_EVENTCODE_MAGIC_MAP) return "MAGIC MAP";
	if (event == UI_EVENTCODE_MAGIC_MAP_RUNE_1) return "MAGIC MAP RUNE 1";
	if (event == UI_EVENTCODE_MAGIC_MAP_RUNE_2) return "MAGIC MAP RUNE 2";
	if (event == UI_EVENTCODE_MAGIC_MAP_RUNE_3) return "MAGIC MAP RUNE 3";
	if (event == UI_EVENTCODE_MAGIC_MAP_RUNE_4) return "MAGIC MAP RUNE 4";

// 0xA0
	if (event == UI_EVENTCODE_REVIVE_CHAMPION) return "REVIVE CHAMPION";
	if (event == UI_EVENTCODE_EXIT_CRYOCELL) return "CANCEL REVIVE CHAMPION";

// 0xD0
	if (event == UI_EVENTCODE_START_NEW_GAME) return "START NEW GAME";
	if (event == UI_EVENTCODE_0D8) return "???";
	if (event == UI_EVENTCODE_RESUME_GAME) return "RESUME GAME";
	if (event == UI_EVENTCODE_SHOW_CREDITS) return "SHOW CREDITS";

	if (event == UI_EVENTCODE_DIALOG_BUTTON_1) return "BUTTON 1";
	if (event == UI_EVENTCODE_DIALOG_BUTTON_2) return "BUTTON 2";
	if (event == UI_EVENTCODE_DIALOG_BUTTON_3) return "BUTTON 3";

// 0xE0		
	if (event == UI_EVENTCODE_QUIT_GAME) return "QUIT GAME";
	if (event == UI_EVENTCODE_MOVE_OBJECT) return "MOVE OBJECT";
	if (event == UI_EVENTCODE_RELEASE_MOUSE_BUTTON) return "RELEASE MOUSE BUTTON";

	if (event == UI_EVENTCODE_MONEYBOX_1) return "MONEY BOX SLOT 1 (BLUE GEMS)";
	if (event == UI_EVENTCODE_MONEYBOX_2) return "MONEY BOX SLOT 2 (RED GEMS)";
	if (event == UI_EVENTCODE_MONEYBOX_3) return "MONEY BOX SLOT 3 (GREEN GEMS)";
	if (event == UI_EVENTCODE_MONEYBOX_4) return "MONEY BOX SLOT 4 (GOLD COINS)";
	if (event == UI_EVENTCODE_MONEYBOX_5) return "MONEY BOX SLOT 5 (SILVER COINS)";
	if (event == UI_EVENTCODE_MONEYBOX_6) return "MONEY BOX SLOT 6 (BRONZE COINS)";
	

	if (event == UI_EVENTCODE_QUIT_CREDITS) return "QUIT CREDITS";

	if (event == UI_EVENTCODE_CLICK_STATS_BAR_1) return "CLICK CHAMPION 1 BAR STATS";
	if (event == UI_EVENTCODE_CLICK_STATS_BAR_2) return "CLICK CHAMPION 2 BAR STATS";
	if (event == UI_EVENTCODE_CLICK_STATS_BAR_3) return "CLICK CHAMPION 3 BAR STATS";
	if (event == UI_EVENTCODE_CLICK_STATS_BAR_4) return "CLICK CHAMPION 4 BAR STATS";



	if (text[0] != 0)
		return (LPCSTR)text;
	return "Unknown";
}

// SPX: Name of AI
CString SkWinCore::getAIName(U8 ai)
{
	Bit8u text[128] = {0};

// 0x00
	if (ai == 0) return "TREE (PILLAR)";
	if (ai == 1) return "LABORATORY TABLE";
	if (ai == 2) return "????";
	if (ai == 3) return "BUSH";
	if (ai == 4) return "PILLARS / ROD (PILLAR)";
	if (ai == 5) return "STALAGMITE (PILLAR)";
	if (ai == 6) return "BOULDER";
	if (ai == 7) return "FOUNTAIN";
	if (ai == 8) return "OBELISKS / TOMBS";
	if (ai == 9) return "WOOD TABLE (TABLE)";
	if (ai == 10) return "MAGICK CAULDRON";
	if (ai == 11) return "SKULL BRAZIER";
	if (ai == 12) return "TRADING TABLE";
	if (ai == 13) return "SCOUT MINION (ALLY)";
	if (ai == 14) return "ATTACK MINION (ALLY)";
	if (ai == 15) return "CARRY MINION (ALLY)";

// 0x10
	if (ai == 16) return "FETCH MINION (ALLY)";
	if (ai == 17) return "GUARD MINION (ALLY)";
	if (ai == 18) return "U-HAUL MINION (ALLY)";
	if (ai == 19) return "THORN DEMON";
	if (ai == 20) return "OBELISK (PASSABLE)";
	if (ai == 21) return "VORTEX";
	if (ai == 22) return "FLAME ORB";
	if (ai == 23) return "CAVERN BAT (BAT)";
	if (ai == 24) return "GLOP";
	if (ai == 25) return "ROCKY";
	if (ai == 26) return "GIGGLER";
	if (ai == 27) return "THICKET THIEF";
	if (ai == 28) return "TIGER STRIPED WORM (WORM)";
	if (ai == 29) return "TREANT (TREE GORGON)";
	if (ai == 30) return "LORD DRAGOTH";
	if (ai == 31) return "DRU TAN";

// 0x20
	if (ai == 32) return "CAVE IN";
	if (ai == 33) return "MERCHANTS";
	if (ai == 34) return "DRAGOTH MINION (EVIL)";
	if (ai == 35) return "TOWER BAT (BAT)";
	if (ai == 36) return "ARCHER GUARD";
	if (ai == 37) return "MAGICK REFLECTOR (MACHINE)";
	if (ai == 38) return "POWER CRYSTAL (MACHINE)";
	if (ai == 39) return "EVIL FOUNTAIN (FOUNTAIN)";
	if (ai == 40) return "SPIKED WALL / FLOOR SPIKES";
	if (ai == 41) return "SPECTRE (GHOST)";
	if (ai == 42) return "VEG MOUTH (DIGGER WORM)";
	if (ai == 43) return "EVIL ATTACK MINION (EVIL)";
	if (ai == 44) return "AXEMAN";
	if (ai == 45) return "CAVERN / STONE TABLE / WALL HOLE?";
	if (ai == 46) return "MUMMY";
	if (ai == 47) return "VOID DOOR (MACHINE)";

// 0x30
	if (ai == 48) return "DARK VEXIRK (VEXIRK)";
	if (ai == 49) return "EVIL GUARD MINION (ENEMY)";
	if (ai == 50) return "SKELETON";
	if (ai == 51) return "AMPLIFIER (MACHINE)";
	if (ai == 52) return "WOLF";
	if (ai == 53) return "PIT GHOST (GHOST)";
	if (ai == 54) return "DOOR GHOST (GHOST)";
	if (ai == 55) return "VEXIRK KING (VEXIRK)";
	if (ai == 56) return "? OBELISK LIKE ?";
	if (ai == 57) return "AXEMAN THIEF";
	if (ai == 58) return "FLYING CHEST";
	if (ai == 59) return "BARREL";
	if (ai == 60) return "PEDISTAL (PILLAR)";
	if (ai == 61) return "GHOST";
	if (ai == 62) return "EVIL ATTACK MINION (EVIL)";

// 0x40

	if (text[0] != 0)
		return (LPCSTR)text;
	return "Unknown";

}

CString SkWinCore::getRecordNameOf(ObjectID recordLink)
{
	Bit8u text[128] = {0};
	if (recordLink == OBJECT_EFFECT_FIREBALL) return "FIREBALL";
//		if (recordLink == OBJECT_EFFECT_MYSTERIOUS) return "MYSTERIOUS";
	if (recordLink == OBJECT_EFFECT_POISON_BLOB) return "POISON_BLOB";
	if (recordLink == OBJECT_EFFECT_LIGHTNING) return "LIGHTNING";
	if (recordLink == OBJECT_EFFECT_DISPELL) return "DISPELL";
	if (recordLink == OBJECT_EFFECT_ZO_SPELL) return "ZO_SPELL";
	if (recordLink == OBJECT_EFFECT_POISON_BOLT) return "POISON_BOLT";
	if (recordLink == OBJECT_EFFECT_POISON_CLOUD) return "POISON_CLOUD";
	if (recordLink == OBJECT_EFFECT_PUSH) return "PUSH";
	if (recordLink == OBJECT_EFFECT_PULL) return "PULL";
	if (recordLink == OBJECT_EFFECT_ZO_2) return "ZO_2";
	if (recordLink == OBJECT_EFFECT_REFLECTOR) return "REFLECTOR";
	if (recordLink == OBJECT_EFFECT_CLOUD) return "CLOUD";
	if (recordLink == OBJECT_EFFECT_THUNDER) return "THUNDER";
	if (recordLink == OBJECT_EFFECT_ELECTRIC_SHOCK) return "ELECTRIC_SHOCK";

	switch (recordLink.DBType()) {
		case dbMissile:
			return getRecordNameOf(GET_ADDRESS_OF_RECORDE(recordLink)->GetMissileObject());
		case dbCreature:
		case dbWeapon:
		case dbCloth:
		case dbScroll:
		case dbPotion:
		case dbContainer:
		case dbMiscellaneous_item:
			QUERY_GDAT_TEXT(
				QUERY_CLS1_FROM_RECORD(recordLink), 
				QUERY_CLS2_FROM_RECORD(recordLink),
				(recordLink.DBType() == dbCreature) ? 0x00 : 0x18, 
				text);
			break;
	}
	if (text[0] != 0)
		return (LPCSTR)text;
	return "Unknown";
}
void SkWinCore::printDistMap(int mapno, DistMapTile const (* const *bp1a)[1][32])
{
	if (!bp1a[mapno]) return;
#ifndef __DJGPP__
	CONSOLE_SCREEN_BUFFER_INFO info;
	ATLVERIFY(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info));

	int cy = dunMapsHeaders[mapno].Row();
	int cx = dunMapsHeaders[mapno].Column();

	for (int y = 0; y < cy; y++) {
		for (int x = 0; x < cx; x++) {
			DistMapTile b = (*(bp1a[mapno]))[x][y];

			int f = b.dir;
			bool assign = b.w2.Is0();
			ATLASSERT(f == 255 || f == 204 || f == 0 || f == 1 || f == 2 || f == 3 || f == 4);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY
				|((assign && f ==   0)?FOREGROUND_GREEN:0)
				|((assign && f ==   1)?FOREGROUND_GREEN:0)
				|((assign && f ==   2)?FOREGROUND_GREEN:0)
				|((assign && f ==   3)?FOREGROUND_GREEN:0)
				|((assign && f ==   4)?FOREGROUND_BLUE:0) // up/down?
				|((assign && f == 204)?FOREGROUND_RED:0)
				|((assign && f == 255)?FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED:0)
				);

			static LPCSTR psz = ".123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz*";
			putchar(psz[min(62, b.distance)]);
		}
		putchar('\n');
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), info.wAttributes);
#endif
}
//}}DEBUG_HELPER

//{{SKSAVE_PUB_PROC
//protected:
//^0759:0114
// SPX: _0759_0114 renamed IS_NEGATIVE
X16 SkWinCore::IS_NEGATIVE(i16 xx)
{
	//^0759:0114
	ENTER(0);
	//^0759:0117
	return (xx < 0) ? 1 : 0;
}

//^0759:000C
X16 SkWinCore::ANIM_FILE_OPEN(const char *filename)
{
	//^0759:000C
	ENTER(0);
	//^0759:000F
	return fset.fileOpen(reinterpret_cast<const char *>(filename));
}

//^0759:00D6
U32 SkWinCore::ANIM_GET_FILE_SIZE(X16 fh)
{
#if UseAltic
	return fset.fileGetSize(fh);
#else
	return Unr(),0;
#endif
}

//^069A:06EE
U8 *SkWinCore::ANIM_farmalloc(U32 size)
{
	return reinterpret_cast<U8 *>(malloc(size));
}
//^0759:002D
U16 SkWinCore::ANIM_READ_HUGE_FILE(X16 fh, U32 readSize, U8 *buff)
{
	//^0759:002D
	ENTER(6);
	//^0759:0031
	U8 *bp06 = buff;
	for (; readSize > 0; ) {
		//^0759:003F
		U16 bp02;
		if (readSize > 32768) {
			//^0759:004E
			bp02 = 0x8000;
		}
		else {
			//^0759:0055
			bp02 = U16(readSize);
		}
		//^0759:005B
		if (fset.fileRead(fh, bp02, bp06) != bp02) {
			//^0759:006F
			return 0;
		}
		//^0759:0073
		readSize -= bp02;
		bp06 += bp02;
		//^0759:008C
	}
	//^0759:009A
	return 1;
}
//^0759:0021
void SkWinCore::ANIM_FILE_CLOSE(X16 fh)
{
	//^0759:0021
	ENTER(0);
	//^0759:0024
	fset.fileClose(fh);
	//^0759:002B
	return;
}
U8 SkWinCore::_069a_03fc(char *xx)
{
	return Unr(), 0; // anim
}
//^069A:048C
char *SkWinCore::ANIM_STRCPY(char *xx, const char *yy)
{
	return strcpy(xx, yy);
}

//^0759:0126
void SkWinCore::_0759_0126()
{
	//^0759:0126
	ENTER(0);
	//^0759:0129
	_089c_0254 = _crt_getvect(255);
	//^0759:0139
	return;
}

//^0759:06C2
void SkWinCore::_0759_06c2()
{
	//^0759:06C2
	ENTER(0);
	//^0759:06C5
	LOADDS(0x0704);
	_089c_0344 -= _089c_0352;
	//^0759:06D9
	return;
}

//^0759:06DB
void SkWinCore::_0759_06db()
{
	//^0759:06DB
	ENTER(0);
	//^0759:06DE
	_089c_025c = _crt_getvect(254);
	_089c_0340 = _089c_025c;
	//^0759:06FC
	//^0759:0706
	_089c_0352 = _01b0_0e80(&SkWinCore::_0759_06c2);
	//^0759:0719
	return;
}
//^0759:072C
X16 SkWinCore::_0759_072c()
{
	//^0759:072C
	ENTER(0);
	//^0759:072F
	return _01b0_051a() CALL_IBMIO;
}
//^0759:071B
void SkWinCore::_0759_071b()
{
	//^0759:071B
	ENTER(0);
	//^0759:071F
	_01b0_04e4();
}
//^01B0:1ED2
X16 SkWinCore::_01b0_1ed2()
{
	//^01B0:1ED2
	ENTER(0);
	//^01B0:1ED5
	LOADDS(0x3083);
	//^01B0:1EDB
	return (glbSoundCardType != 0) ? 1 : 0;
}

//^069A:05FE
void SkWinCore::ANIM_farfree(U8 *buff)
{
	free(buff);
}

//^069A:040D
U32 SkWinCore::ANIM_farcoreleft()
{
#if UseAltic
	return 1024*1024; // Win32 supplies almost infinite memory pool compared to MS-DOS env.
#else
	//^069A:040D
	ENTER(0);
	//^069A:040D
	return _crt_farcoreleft();
#endif
}
X32 SkWinCore::_0759_07f2(U8 *xx) {
	return Unr(), 0; // anim
}
void SkWinCore::_0759_0855() {
	Unr(); // anim
}
U8 *SkWinCore::_0759_0869(U32 xx) {
	Unr(); // anim
	return NULL;
}
//^0759:06B5
void SkWinCore::_0759_06b5()
{
	//^0759:06B5
	ENTER(0);
	//^0759:06B8
	_00eb_0bc4();
	//^0759:06C0
	return;
}
//^0759:065F
void SkWinCore::_0759_065f()
{
	//^0759:065F
	ENTER(8);
	//^0759:0663
	SRECT bp08 = _089c_00d0;
	//^0759:0674
	IBMIO_FILL_RECT_SCREEN(&bp08, 0);
	//^0759:0686
	return;
}
void SkWinCore::_0759_0792() {
	Unr(); // anim
}
void SkWinCore::_0759_0739(U8 *xx, U16 yy, U16 zz) {
	Unr(); // anim
}
//^0759:0330
void SkWinCore::ANIM_DECODE_IMG1(U8 *xx, U8 *yy)
{
	//^0759:0330
	ENTER(14);
	//^0759:0336
	_089c_0348 = xx;
	_089c_0354 = yy;
	//^0759:0350
	U16 bp02 = _089c_0348[1] | (_089c_0348[0] << 8); // image width
	//^0759:036A
	U16 bp04 = _089c_0348[3] | (_089c_0348[2] << 8); // image height
	//^0759:03B9
	_089c_0348 += 4;
	//^0759:03C8
	X16 bp06;
	X16 bp08;
	bp08 = (bp06 = (bp02 +1)&0xfffe) * bp04;
	U16 di = 0;
	U16 bp0c = 0;
	do {
		//^0759:03DF
		i8 bp09 = (_089c_0348++)[0];
		X16 si;
		if ((bp09 & 0x80) == 0) {
			//^0759:03FC
			si = (bp09 >> 4) +1;
			//^0759:0404
			ANIM_FILL_SEQ_4BPP(di, bp09&15, si);
			//^0759:0413
			di += si;
			continue;
		}
		//^0759:0418
		X16 bp0e = bp09 & 0x30;
		switch (bp0e) {
		case 0x00://^043C
			//^0759:043C
			if ((bp09 & 0x40) == 0) {
				si = (_089c_0348[0]) +1;
				_089c_0348++;
			}
			else {
				si = (_089c_0348[1] | (_089c_0348[0] << 8)) +1;
				_089c_0348 += 2;
			}
			//^0759:048F
			//^0759:0404
			ANIM_FILL_SEQ_4BPP(di, bp09&15, si);
			//^0759:0413
			di += si;
			continue;
		case 0x10://^0495
			//^0759:0495
			if ((bp09 & 0x40) == 0) {
				//^0759:049B
				si = (_089c_0348[0]) +1;
				_089c_0348++;
			}
			else {
				//^0759:04AE
				si = (_089c_0348[1] | (_089c_0348[0] << 8)) +1;
				_089c_0348 += 2;
			}
			//^0759:04E8
			if ((si & 1) != 0) {
				//^0759:04EE
				ANIM_SETPIXEL_SEQ_4BPP(di, bp09&15);
				//^0759:04FB
				di++;
				si--;
			}
			//^0759:04FD
			ANIM_BLIT_TO_MEMORY_ROW_4TO4BPP(0, di, si);
			_089c_0348 += si >> 1;
			//^0759:0518
			//^0759:0413
			di += si;
			continue;
		case 0x20://^051B
			//^0759:051B
			if (bp0c < di) {
				//^0759:0520
				_0759_0310(bp0c, di -bp0c);
			}
			//^0759:052F
			si = ((bp09 >> 2)&16)|(bp09&15);
			if (si == 0x1d) {
				//^0759:054B
				si = _089c_0348[0] +1;
				_089c_0348++;
			}
			//^0759:0568
			else if (si == 0x1e) {
				//^0759:056D
				si = _089c_0348[0] +0x101;
				_089c_0348++;
			}
			//^0759:057B
			else if (si == 0x1f) {
				//^0759:0580
				si = (_089c_0348[1] | (_089c_0348[0] << 8)) +1;
				_089c_0348 += 2;
			}
			else {
				//^0759:05B2
				si++;
			}
			//^0759:05B3
			di += si;
			bp0c = di;
			//^0759:05B8
			break;
		case 0x30://^05BB
			//^0759:05BB
			if ((bp09 & 0x40) == 0) {
				//^0759:05C1
				si = _089c_0348[0] +1;
				_089c_0348++;
			}
			else {
				//^0759:05D4
				si = (_089c_0348[1] | (_089c_0348[0] << 8)) +1;
				_089c_0348 += 2;
			}
			//^0759:060E
			_0759_02c6(di, di -bp06, si);
			//^0759:061D
			di += si;
			ANIM_SETPIXEL_SEQ_4BPP(di++, bp09&15);
			//^0759:062F
			break;
		}
		//^0759:062F
	} while (di < bp08);
	ATLASSERT(di <= bp08);
	//^0759:0637
	if (bp0c < di) {
		//^0759:063C
		_0759_0310(bp0c, di -bp0c);
	}
	//^0759:064B
	return;
}
//^0759:06A1
void SkWinCore::_0759_06a1(U8 ps)
{
	//^0759:06A1
	ENTER(0);
	//^0759:06A4
	IBMIO_SELECT_PALETTE_SET(ps) CALL_IBMIO;
	//^0759:06B3
	return;
}

//^00EB:04BC
void SkWinCore::_00eb_04bc(skxxxj *xx, U16 yy) //#DS=04BF
{
	//^00EB:04BC
	ENTER(0);
	//^00EB:04C1
	LOADDS(0x0c48);
	X16 si = 0;
	X16 di = yy << 4;
	//^00EB:04D1
	for (si = 0; si < 16; si++) {
		//^00EB:04D6
		glbPaletteRGB[(si +di)][0] = U8(xx[si].b1 >> 2);
		glbPaletteRGB[(si +di)][1] = U8(xx[si].b2 >> 2);
		glbPaletteRGB[(si +di)][2] = U8(xx[si].b3 >> 2);
		if (yy == 0) {
			//^00EB:0548
			_04bf_0a50[si][0] = U8(xx[si].b1 >> 2);
			_04bf_0a50[si][1] = U8(xx[si].b2 >> 2);
			_04bf_0a50[si][2] = U8(xx[si].b3 >> 2);
		}
		//^00EB:05AE
	}
	//^00EB:05B7
	if (glbUpdatePalette == 1) {
		//^00EB:05BE
		IBMIO_UPDATE_PALETTE_SET();
	}
	//^00EB:05C2
	return;
}

//^0759:0688
void SkWinCore::_0759_0688(skxxxj *xx, U16 yy)
{
	//^0759:0688
	ENTER(0);
	//^0759:068B
	_00eb_04bc(xx, yy) CALL_IBMIO;
	//^0759:069F
	return;
}

//^069A:035B
X16 SkWinCore::ANIM_TOUPPER(X16 xx)
{
	//^069A:035B
	ENTER(0);
	//^069A:035E
	i16 dx = xx;
	if (dx == -1)
		return -1;
	if ((_089c_00e3[U8(dx)] & 8) != 0)
		return U8(dx) -32;
	//^069A:0381
	return U8(dx);
}


//^0759:0310
void SkWinCore::_0759_0310(U16 xx, U16 yy)
{
	//^0759:0310
	ENTER(0);
	//^0759:0313
	_00eb_070c(_089c_0354, xx, xx, yy) CALL_IBMIO;
	//^0759:032E
	return;
}
//^0759:02C6
void SkWinCore::_0759_02c6(U16 xx, U16 yy, U16 zz)
{
	//^0759:02C6
	ENTER(0);
	//^0759:02CB
	U8 *esdi = _089c_0354;
	U8 *essi = esdi;
	U16 bx = xx;
	U16 dx = yy;
	U16 cx = zz;
	dx >>= 1;
	bool carry1 = (bx & 1) != 0;
	bx >>= 1;
	esdi += bx;
	essi += dx;
	if (carry1)  {
		//^0759:02E7
		U8 dl = *esdi;
		U8 al = lodsb(essi);
		dl &= 0xf0;
		al &= 0x0f;
		al |= dl;
		stosb(esdi, al);
		if (--cx == 0) return;
	}
	//^0759:02F7
	cx++;
	cx >>= 1;
	bool carry2 = (cx & 1) != 0;
	cx >>= 1;
	if (carry2) {
		//^0759:02FE
		movsb(esdi, essi);
	}
	//^0759:0300
	for (; cx != 0; cx--) movsw(esdi, essi);
	//^0759:030C
	return;
}

//^0759:08E7
int SkWinCore::_0759_08e7(i16 argc, char **argv, char **env) // #DS=089C
{
	//^0759:08E7
	ENTER(214);
	//^0759:08ED
	X16 bp2a = 0;
	X16 bp2e = 0;
	X16 bp32 = 0;
	X16 bp34 = 0;
	X16 bp36 = 0;
	X16 bp42 = 0;
	X16 bp44 = 0;
	X16 bp46 = 0;
	X16 bp48 = 0;
	X16 bp4a = 0;
	X16 bp5e = 0;
	X16 bp60 = 0;
	U8 bp69 = 0xf;
	X16 bp6c = 0;
	char bp0096[0x0096 -0x6e] = {0};
	i16 si = 1;
	//^0759:093A
	U8 *bp64;
	for (; si < argc; si++) {
		//^0759:093D
		if (argv[si][0] == '+' && ANIM_TOUPPER(argv[si][1]) == 'A') { // TODO: check whether this is toupper
			//^0759:0974
			switch (ANIM_TOUPPER(argv[si][2])) {
			case 'B'://^09A1
				//^0759:09A1
				bp44 = 1;
				break;
			case 'L'://^09A9
				//^0759:09A9
				bp44 = 1;
				bp32 = 1;
				break;
			case 'D'://^09B6
				//^0759:09B6
				bp34 = 1;
				break;
			case 'H'://^09BE
				//^0759:09BE
				bp46 = 1;
				break;
			case 'E'://^09C6
				//^0759:09C6
				bp48 = 1;
				break;
			case 'S'://^09CE
				//^0759:09CE
				bp4a = 1;
				break;
			case 'M'://^09D6
				//^0759:09D6
				bp5e = 1;
				break;
			case 'F'://^09DE
				{
				//^0759:09DE
				X16 bp30;
				if (IS_NEGATIVE(bp30 = ANIM_FILE_OPEN(&argv[si][3])) != 0)
					break;
				//^0759:0A09
				U32 bp68 = ANIM_GET_FILE_SIZE(bp30);
				bp64 = ANIM_farmalloc(bp68);
				if (bp64 != NULL) {
					//^0759:0A35
					ANIM_READ_HUGE_FILE(bp30, bp68, bp64);
					bp60 = 1;
				}
				//^0759:0A50
				ANIM_FILE_CLOSE(bp30);
				break;
				}
			case 'V'://^0A5B
				//^0759:0A5B
				bp69 = _069a_03fc(&argv[si][3]);
				if (bp69 > 15) {
					//^0759:0A80
					bp69 = 15;
				}
				break;
			case 'O'://^0A86
				//^0759:0A86
				bp6c = _069a_03fc(&argv[si][3]);
				if (bp6c > 0xfe) {
					//^0759:0AAC
					bp6c = 0xe;
				}
				break;
			case 'C'://^0ADD
			case 'G'://^0ADD
			case 'I'://^0ADD
			case 'J'://^0ADD
			case 'K'://^0ADD
			case 'N'://^0ADD
			case 'P'://^0ADD
			case 'Q'://^0ADD
			case 'R'://^0ADD
			case 'T'://^0ADD
			case 'U'://^0ADD
			default:
				break;
			}
		}
		else if (bp36 == 0) {
			//^0759:0AB9
			ANIM_STRCPY(bp0096, argv[si]);
			//^0759:0AD8
			bp36 = 1;
		}
		//^0759:0ADD
	}
	//^0759:0AE6
	_0759_0126();
	_0759_06db();
	//^0759:0AEE
	while (_0759_072c() != 0) {
		//^0759:0AF0
		_0759_071b();
		//^0759:0AF4
	}
	//^0759:0AFC
	X16 bp30;
	if (bp0096[0] == 0 || IS_NEGATIVE(bp30 = ANIM_FILE_OPEN(bp0096)) != 0) {
		//^0759:0B1C
#if UseAltic
		if (IS_NEGATIVE(ANIM_FILE_OPEN(_089c_00d8)) != 0) {
			//^0759:0B33
			return (0);
		}
#else
		if (IS_NEGATIVE(ANIM_FILE_OPEN(_089c_00d8)) == 0) {
			//^0759:0B33
			return (0);
		}
#endif
	}
	//^0759:0B3B
	U8 *bp08 = ANIM_farmalloc(32000);
	if (bp08 == NULL) {
		//^0759:0B58
		return (0);
	}
	//^0759:0B60
	U8 *bp10;
	X16 bp50;
	if (_01b0_1ed2() CALL_IBMIO != 0 && (bp10 = ANIM_farmalloc(32000)) != NULL) {
		//^0759:0B8C
		if (_RELOAD_SOUND_BUFFER(bp10, 32000, PLAYBACK_FREQUENCY) CALL_IBMIO != 0) {
			//^0759:0BA7
			bp50 = 1;
		}
		else {
			//^0759:0BAE
			bp50 = 0;
			ANIM_farfree(bp10);
		}
	}
	else {
		//^0759:0BC2
		bp50 = 0;
	}
	//^0759:0BC7
	U32 bp14 = ANIM_GET_FILE_SIZE(bp30);
	U8 *bp04;
	X16 di;
	if (bp5e != 0 || ANIM_farcoreleft() < bp14) {
		//^0759:0BF2
		X32 bp4e = _0759_07f2(_089c_034c);
		if (bp4e < bp14) {
			//^0759:0C0E
			_0759_0855();
			return (0);
		}
		//^0759:0C1A
		U32 bp54 = bp14;
		U32 bp58 = 0;
		//^0759:0C30
		for (; bp54 <= 0; ) {
			//^0759:0C32
			U16 bp5a;
			if (bp54 > 16000) {
				//^0759:0C41
				bp5a = 16000;
			}
			else {
				//^0759:0C48
				bp5a = U16(bp54);
			}
			//^0759:0C4E
			ANIM_READ_HUGE_FILE(bp30, bp5a, _0759_0869(bp58));
			bp58 += bp5a;
			bp54 -= bp5a;
			//^0759:0C7C
		}
		//^0759:0C8A
		di = 1;
	}
	else {
		//^0759:0C8F
		di = 0;
		bp04 = ANIM_farmalloc(bp14);
		if (bp04 == NULL) {
			//^0759:0CAF
			return (0);
		}
		//^0759:0CB7
		ANIM_READ_HUGE_FILE(bp30, bp14, bp04);
	}
	//^0759:0CCD
	ANIM_FILE_CLOSE(bp30);
	//^0759:0CD5
	if (bp4a != 0)
		//^0759:0CDB
		_0759_06b5();
	//^0759:0CDF
	_0759_06a1(0);
	_0759_065f();
	X32 bp18 = 0; // anim read offset?
	X32 bp24 = 0;
	//^0759:0CFA
	X16 bp2c = 0;
	X16 bp5c = 1;
	if (bp60 != 0) {
#if UseAltic
		ATLASSERT(false);
#else
		//^0759:0D0A
		_xxxx_xxxx(bp64, 0, bp69 << 4, 0);
#endif
	}

	// I thank to Christophe Fontanel for The Animations file format section!
	// http://dmweb.free.fr/?q=node/216

	// The below identifications are brought from above file spec site.

	do {
		//^0759:0D28
		U8 *bp0c = (di != 0)
			? _0759_0869(bp18)
			: bp04 +bp18;
		//^0759:0D51
		X16 bp28 = bp0c[5] | (bp0c[4] << 8); // 1 word (big endian): Item attribute
		X32 bp1c = bp18 +U32((bp0c[2] << 8) | bp0c[3]) +6; // next memory position
		X16 bp6e = (bp0c[1] << 8) | bp0c[0]; // 2 bytes: Item type
		U8 *bp3a;
		U16 bp40;
		U16 bp3e;
		U16 bp3c;
		X32 bp20;
		U16 bp26;
		switch (bp6e) {
		case 0x4157://^0E21 // "WA"
			//^0759:0E21
			if (bp28 == 1) {
				//^0759:0E2C
				_0759_0792();
			}
			break;
		case 0x4453://^0E33 // "SD"
			//^0759:0E33
			if (bp5c != 0) {
				//^0759:0E3C
				if (di != 0) {
					//^0759:0E40
					_0759_0869(_089c_02c0[bp42++] = bp18 +6);
				}
				else {
					//^0759:0E6D
					_089c_02c0[bp42] = bp18 +6;
					bp3a = bp04;
					bp42++;
					break;
				}
				//^0759:0EA7
				bp40 = (bp3a[0] << 8) | (bp3a[1]);
				si = 0;
				//^0759:0EBF
				for (; U16(si) < bp40; si++) {
					//^0759:0EC1
					bp3a[si +2] = bp3a[si +2] +0x80;
					//^0759:0ECE
				}
				//^0759:0ED4
				if (di == 0)
					break;
				//^0759:0EDB
				bp0c = _0759_0869(bp18);
			}
			break;
		case 0x4F53://^0EF0 // "SO"
			//^0759:0EF0
			bp3e = 0xff;
			bp3c = 5500;
			bp3a = (di != 0)
				? _0759_0869(_089c_02c0[bp28])
				: &bp04[_089c_02c0[bp28 -1]];
			//^0759:0F35
			_0759_0739(bp3a, bp3e, bp3c);
			if (di != 0) {
				//^0759:0F4F
				bp0c = _0759_0869(bp18);
			}
			break;
		case 0x4C44://^0F64 // "DL"
		case 0x4E45://^0F64 // "EN"
			//^0759:0F64
			_089c_0344 = bp28;
			if (bp0c[6] == 0xff && bp0c[7] == 0x81) {
				//^0759:0FA4
				ANIM_DECODE_IMG1(&bp0c[8], bp08);
			}
			else {
				//^0759:0FB7
				ANIM_DECODE_IMG1(&bp0c[6], bp08);
			}
			//^0759:0FD6
			_0759_06a1(1);
			bp2a++;
			while (_089c_0344 > 0 && bp34 == 0) {
#if UseAltic
				for (U32 w=0; w < 320*200; w += 2) {
					vram[w +0] = bp08[w >> 1] >> 4;
					vram[w +1] = bp08[w >> 1] & 15;
				}
				skwin.UpdateRect(0, 0, 320, 200);
				MessageLoop(true, true); // anim
#else
				ATLASSERT(false);
#endif
			}
			//^0759:0FF6
			if (bp2e != 0) {
				//^0759:0FFF
				bp2e = 0;
				bp1c = bp20;
			}
			break;
		case 0x4C50://^1013 // "PL"
			//^0759:1013
			bp26 = 0;
			//^0759:1018
			for (si = 0; si < 64; si++) {
				//^0759:101C
				if (bp0c[si +8] != _089c_0090[si])
					//^0759:103E
					bp26 = 1;
				//^0759:1043
				_089c_0090[si] = bp0c[si +8];
				//^0759:1063
			}
			//^0759:1069
			if (bp26 != 0) { // is palette updated?
				//^0759:1072
				skxxxj bp00d6[16];
				for (si = 0; si < 64; si += 4) {
					//^0759:1077
					bp00d6[si >> 2].b0 = bp0c[si + 8];
					bp00d6[si >> 2].b1 = bp0c[si + 9] << 4;
					bp00d6[si >> 2].b2 = bp0c[si +10] << 4;
					bp00d6[si >> 2].b3 = bp0c[si +11] << 4;
					//^0759:113C
				}
				//^0759:1147
				_0759_06a1(0);
				_0759_0688(bp00d6, 0);
			}
			break;
		case 0x4F46://^1160 // "FO"
			//^0759:1160
			bp0c = (di != 0)
				? _0759_0869(bp1c)
				: &bp04[bp1c];
			//^0759:1189
			_089c_025a[bp2c +1].w4 = bp28;
			_089c_025a[bp2c +1].dw0 = bp1c +((bp0c[2] << 8) | bp0c[3]) +6;
			bp2c++;
			if (di != 0)
				goto _12ce;
			//^0759:1204
			bp0c = &bp04[bp18];
			break;
		case 0x454E://^121E // "NE"
			//^0759:121E
			bp26 = -- _089c_025a[bp2c].w4;
			if (bp26 > 0 && bp26 < 10) {
				//^0759:123F
				bp2e = 1;
				bp20 = _089c_025a[bp2c].dw0;
				break;
			}
			//^0759:125F
			bp2c--;
			if (di != 0)  {
				//^0759:1266
				bp0c = _0759_0869(bp1c);
			}
			//^0759:1278
			bp1c = bp1c +((bp0c[2] << 8) | bp0c[3]) +6;
			//^0759:12CA
			if (di != 0) {
				//^0759:12CE
_12ce:
				bp0c = _0759_0869(bp18);
			}
		}
		//^0759:12E0
		bp18 = bp1c;
		if (bp32 != 0 && bp18 >= bp14) {
			//^0759:1301
			bp5c = 0;
			bp18 = bp24;
		}
		//^0759:1312
		if (_0759_072c() != 0 && bp34 == 0 && bp44 != 0) {
			//^0759:1326
			_0759_071b();
			//^0759:132A
			break;
		}
		//^0759:132C
	} while (bp18 < bp14);
	//^0759:1344
	if (bp60 != 0) {
#if UseAltic
		ATLASSERT(false);
#else
		//^0759:134A
		_xxxx_xxxx(bp6c);
#endif
	}
	//^0759:1356
	if (bp48 != 0) {
		//^0759:135C
		_0759_06b5();
	}
	if (bp46 == 0) {
		//^0759:1366
		_0759_06a1(0);
	}
	if (di != 0) {
		//^0759:1371
		_0759_0855();
	}
	else {
		//^0759:1377
		ANIM_farfree(bp04);
	}
	//^0759:1384
	if (bp60 != 0) {
		_01b0_1983() CALL_IBMIO;
		ANIM_farfree(bp64);
	}
	//^0759:139F
	if (bp50 != 0) {
		//^0759:13A5
		_01b0_18d3(0) CALL_IBMIO;
		ANIM_farfree(bp10);
	}
	//^0759:13BD
	return (0);
	////^0759:13C5
	//return 0;
}

//^069A:0000
int SkWinCore::ANIM_BOOTSTRAP_SWOOSH() // #DS=089C
{
	// 08D2:0FE2  F6 0F D2 08 FE 0F D2 08 05 10 D2 08 09 10 D2 08  ÅE“.˛.“...“...“.
	// 08D2:0FF2  00 00 00 00 43 3A 5C 41 4E 49 4D 00 73 77 6F 6F  ....C:\ANIM.swoo
	// 08D2:1002  73 68 00 2B 70 6D 00 2B 73 62 00 08 00 00 5A 00  sh.+pm.+sb....Z.
	// 08D2:1012  00 2B 96 00 00 00 00 00 00 00 00 00 00 00 00 00  .+ÅE............
	char *argv[] = {"ANIM", "swoosh", "+pm", "+sb"};
	return (_0759_08e7(4, argv, NULL));
}

//^069A:0000
int SkWinCore::ANIM_BOOTSTRAP_TITLE() // #DS=089C
{
	// 08D2:0FCC  EC 0F D2 08 F4 0F D2 08 FA 0F D2 08 FE 0F D2 08  ÅE“.ÅE“.ÅE“.˛.“.
	// 08D2:0FDC  02 10 D2 08 06 10 D2 08 0A 10 D2 08 00 00 00 00  ..“...“...“.....
	// 08D2:0FEC  43 3A 5C 41 4E 49 4D 00 74 69 74 6C 65 00 2B 61  C:\ANIM.title.+a
	// 08D2:0FFC  68 00 2B 61 73 00 2B 61 62 00 2B 70 6D 00 2B 73  h.+as.+ab.+pm.+s
	// 08D2:100C  62 00 00 00 5A 00 00 2B 96 00 00 00 00 00 00 00  b...Z..+ÅE......
	char *argv[] = {"ANIM", "title", "+ah", "+as", "+ab", "+pm", "+sb"};
	return (_0759_08e7(7, argv, NULL));
}

//^48AE:0281
U16 SkWinCore::QUERY_CREATURES_ITEM_MASK(U8 cls2, U8 cls4, U8 itemflags[64], U16 isCreature)
{
	//^48AE:0281
	ENTER(142);
	//^48AE:0287
	ZERO_MEMORY(itemflags, 64);
	//^48AE:0299
	Bit8u bp008e[128];
	U8 *bp04 = QUERY_GDAT_TEXT(GDAT_CATEGORY_CREATURES, cls2, cls4 +0x10, bp008e);
	//^48AE:02BB
	if (*bp04 == 0)
		//^48AE:02C3
		return 0;
	//^48AE:02C8
	i16 bp0a = 0;
	i16 di = -1;
	U16 si = -1;
	U16 bp0c = 0;
	U8 bp05;
	do {
		//^48AE:02D9
		bp05 = *(bp04++);
		//^48AE:02E5
		if (bp05 >= '0' && bp05 <='9') {
			//^48AE:02F1
			bp0a = bp0a * 10 +bp05 -'0';
			bp0c = 1;
			//^48AE:030B
			continue;
		}
		//^48AE:030E
		if (bp05 == '-') {
			//^48AE:0314
			di = bp0a;
			bp0a = 0;
			//^48AE:031C
			continue;
		}
		//^48AE:031F
		if (bp0c != 0) {
			//^48AE:0325
			if (di < 0)
				//^48AE:0329
				di = bp0a;

            while (di <= bp0a) {
				//^48AE:032E
				i16 bp08 = di +si;
				//^48AE:0335
				itemflags[bp08 >> 3] = itemflags[bp08 >> 3] | (1 << (bp08 & 7));
				//^48AE:035A
				di++;
				//^48AE:035B
			}
			//^48AE:0360
			bp0a = 0;
			di = -1;
			si = -1;
			bp0c = 0;
		}
		//^48AE:0371
		U16 bp0e = bp05;
		//^48AE:0379
		switch (bp0e) {
			case 'W':			// Weapon
				//^48AE:0393
				//^48AE:03AA
				si = 0;
				//^48AE:03AC
				break;

			case 'A':			// Clothing
				//^48AE:0395
				si = 0x0080;
				//^48AE:0398
				break;

			case 'J':			// Misc
				//^48AE:039A
				si = 0x0100;
				//^48AE:039D
				break;

			case 'P':			// Potion
				//^48AE:039F
				si = 0x0180;
				//^48AE:03A2
				break;

			case 'C':			// Creature or Container?
				//^48AE:03A4
				si = (isCreature != 0) ? 0 : 0x01e0;
				//^48AE:03B1
				break;

			case 'S':			// Scroll
				//^48AE:03B3
				si = 0x01fc;

				break;
		}
		//^48AE:03B6
	} while (bp05 != 0);

	//^48AE:03BF
	return 1;
}

//^47E1:0089
void SkWinCore::ZERO_MEMORY(void *buff, U32 size)
{
	memset(buff, 0, size);
}




//^0CEE:0B0D
void SkWinCore::APPEND_RECORD_TO(ObjectID recordLink_whatYouAppend, ObjectID *recordLink_newParent, i16 xpos_newParent, i16 ypos_newParent)
{
	SkD((DLV_DBM, "DBM: APPEND_RECORD_TO(%04X,%p(%04X),%2d,%2d)\n"
		, (Bitu)recordLink_whatYouAppend.w
		, recordLink_newParent
		, (recordLink_newParent != NULL) ? (Bitu)recordLink_newParent->w : 0
		, (Bitu)xpos_newParent
		, (Bitu)ypos_newParent
		));

	ATLASSERT(false
		|| (xpos_newParent == -1 && ypos_newParent == 0)
		|| (xpos_newParent == -1 && ypos_newParent == -1)
		|| ((U16)xpos_newParent < (U16)glbCurrentMapWidth && (U16)ypos_newParent < (U16)glbCurrentMapHeight)
		);

	//^0CEE:0B0D
	i16 di = xpos_newParent;
	//^0CEE:0B16
	// SPX: Added test when DBIndex equals 1023 (x3FF) or 1022 (x3FE)
	if (recordLink_whatYouAppend != OBJECT_END_MARKER && recordLink_whatYouAppend != OBJECT_NULL &&
		(SkCodeParam::bUsePowerDebug && recordLink_whatYouAppend.DBIndex() != 1023) && 
		(SkCodeParam::bUsePowerDebug && recordLink_whatYouAppend.DBIndex() != 1022)) {
		//^0CEE:0B28
		GenericRecord *bp0c = (GenericRecord *)GET_ADDRESS_OF_RECORD(recordLink_whatYouAppend);
		//^0CEE:0B36
		bp0c->w0 = OBJECT_END_MARKER;
		//^0CEE:0B3E
		if (di >= 0) {
			//^0CEE:0B45
			Bit8u *bp08 = &glbCurrentTileMap[di][ypos_newParent];
			// SPX: problem with crashes here
			if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(bp08))
				return;
			//^0CEE:0B60
			ObjectID bp12;
			if ((*bp08 & 0x10) != 0) {
				//^0CEE:0B69
				bp12 = GET_TILE_RECORD_LINK(di, ypos_newParent);
				//^0CEE:0B76
				//^0CEE:0C53

				ATLASSERT(bp12 != OBJECT_END_MARKER); // BUG if tile record link is OBJECT_END_MARKER

				ObjectID bp0e;
				for (; (bp0e = GET_NEXT_RECORD_LINK(bp12)) != OBJECT_END_MARKER; ) {
					//^0CEE:0C58
					bp12 = bp0e;
				}

				//^0CEE:0C6D
				bp0c = (GenericRecord *)GET_ADDRESS_OF_RECORD(bp12);
				//^0CEE:0C7B
				//^0CEE:0CA5
				bp0c->w0 = recordLink_whatYouAppend;
				return;
			}
			//^0CEE:0B79
			if (dunGroundStacks[dunHeader->cwListSize -1] != OBJECT_NULL) {
				//^0CEE:0B90
				RAISE_SYSERR(SYSTEM_ERROR__CANT_APPEND_RECORD);
			}
			//^0CEE:0B98
			*bp08 |= 0x10;
			//^0CEE:0B9F
			//Bit16u *bp04 = &_4976_4c52[di +1];
			OID_T *bp04 = &_4976_4c52[di +1];	// Bit32u *bp04
			//^0CEE:0BB5
			Bit16u si = _4976_4cb4 -(dunMapColumnsSumArray[glbCurrentMapIndex] +di) -1;
			ATLASSERT(si < (32*256)); // SPX: suppose 256 of 32 columns each, making a value not above 8192. If not, it might be a negative value hidden by the unsigned Bit16u si; then an issue with _4976_4cb4 value.
			//^0CEE:0BCE
			//^0CEE:0BDA
			for (; si-- != 0; ) {
				//^0CEE:0BD0
				(*bp04)++;
				bp04++;
			}
			//^0CEE:0BE1
			Bit16u bp10 = 0;
			bp08 -= ypos_newParent;
			//^0CEE:0BEC
			si = _4976_4c52[di];

			//^0CEE:0C08
			for (; bp10++ != ypos_newParent; ) {
				//^0CEE:0BF9
				if ((*(bp08++) & 0x10) != 0) {
					//^0CEE:0C07
					si++;
				}
			}
			//^0CEE:0C13
			bp04 = &dunGroundStacks[si];
			//^0CEE:0C27
			MOVE_MEMORY(
				bp04,
				bp04 +1,
				(dunHeader->cwListSize -si -1) * 2
				);
			//^0CEE:0C4E
			//^0CEE:0C51
			//^0CEE:0CA8
			*bp04 = recordLink_whatYouAppend;
			return;
		}
		//^0CEE:0C7D
		bp0c = (GenericRecord *)recordLink_newParent;
		//^0CEE:0C9C
		for (; bp0c->w0 != OBJECT_END_MARKER; ) {
			//^0CEE:0C8B
			bp0c = (GenericRecord *)GET_ADDRESS_OF_RECORD(bp0c->w0);
		}
		//^0CEE:0CA5
		bp0c->w0 = recordLink_whatYouAppend;
		return;
	}
	//^0CEE:0CAE
}

//^2066:1EC9
ObjectID SkWinCore::_2066_1ec9(ObjectID link1Dst, ObjectID link2Src)
{
	//^2066:1EC9
	ObjectID di = link1Dst;
	ObjectID si = link2Src;
	//^2066:1ED5
	if (di == OBJECT_END_MARKER)
		return si;
	//^2066:1EDE
	if (si == OBJECT_END_MARKER)
		return di;
	//^2066:1F09
	while (si != OBJECT_END_MARKER && si.DBType() < dbCreature) {
		//^2066:1EE5
		ObjectID bp06 = di;
		di = si;
		si = GET_NEXT_RECORD_LINK(si);
		GenericRecord *bp04 = GET_ADDRESS_OF_RECORD(di);
		//^2066:1F00
		bp04->w0 = bp06;
		//^2066:1F09
	}
	//^2066:1F1B
	APPEND_RECORD_TO(si, &GET_ADDRESS_OF_RECORD(di)->w0, -1, -1);
	//^2066:1F31
	return di;
}

Creature *SkWinCore::GET_ADDRESS_OF_RECORDX4(ObjectID recordLink) { return GET_ADDRESS_OF_DETACHED_RECORD(recordLink)->castToCreature(); }
Door *SkWinCore::GET_ADDRESS_OF_RECORD0(ObjectID recordLink) { return GET_ADDRESS_OF_RECORD(recordLink)->castToDoor(); }
Teleporter *SkWinCore::GET_ADDRESS_OF_RECORD1(ObjectID recordLink) { return GET_ADDRESS_OF_RECORD(recordLink)->castToTeleporter(); }
Text *SkWinCore::GET_ADDRESS_OF_RECORD2(ObjectID recordLink) { return GET_ADDRESS_OF_RECORD(recordLink)->castToText(); }
SimpleActuator *SkWinCore::GET_ADDRESS_OF_RECORD2X(ObjectID recordLink) { return GET_ADDRESS_OF_RECORD(recordLink)->castToSimpleActuator(); } // SPX: added
Actuator *SkWinCore::GET_ADDRESS_OF_RECORD3(ObjectID recordLink) { return GET_ADDRESS_OF_RECORD(recordLink)->castToActuator(); }
Creature *SkWinCore::GET_ADDRESS_OF_RECORD4(ObjectID recordLink) { return GET_ADDRESS_OF_RECORD(recordLink)->castToCreature(); }
Weapon *SkWinCore::GET_ADDRESS_OF_RECORD5(ObjectID recordLink) { return GET_ADDRESS_OF_RECORD(recordLink)->castToWeapon(); }
Cloth *SkWinCore::GET_ADDRESS_OF_RECORD6(ObjectID recordLink) { return GET_ADDRESS_OF_RECORD(recordLink)->castToCloth(); }
Scroll *SkWinCore::GET_ADDRESS_OF_RECORD7(ObjectID recordLink) { return GET_ADDRESS_OF_RECORD(recordLink)->castToScroll(); }
Potion *SkWinCore::GET_ADDRESS_OF_RECORD8(ObjectID recordLink) { return GET_ADDRESS_OF_RECORD(recordLink)->castToPotion(); }
Container *SkWinCore::GET_ADDRESS_OF_RECORD9(ObjectID recordLink) { return GET_ADDRESS_OF_RECORD(recordLink)->castToContainer(); }
Miscellaneous_item *SkWinCore::GET_ADDRESS_OF_RECORDA(ObjectID recordLink) { return GET_ADDRESS_OF_RECORD(recordLink)->castToMisc(); }
void *SkWinCore::GET_ADDRESS_OF_RECORDB(ObjectID recordLink) { return NULL; }
void *SkWinCore::GET_ADDRESS_OF_RECORDC(ObjectID recordLink) { return NULL; }
void *SkWinCore::GET_ADDRESS_OF_RECORDD(ObjectID recordLink) { return NULL; }
Missile *SkWinCore::GET_ADDRESS_OF_RECORDE(ObjectID recordLink) { return GET_ADDRESS_OF_RECORD(recordLink)->castToMissile(); }
Cloud *SkWinCore::GET_ADDRESS_OF_RECORDF(ObjectID recordLink) { return GET_ADDRESS_OF_RECORD(recordLink)->castToCloud(); }
GenericContainerRecord *SkWinCore::GET_ADDRESS_OF_GENERIC_CONTAINER_RECORD(ObjectID recordLink) { return static_cast<GenericContainerRecord *>(GET_ADDRESS_OF_RECORD(recordLink)); }

Actuator *SkWinCore::GET_ADDRESS_OF_ACTU(ObjectID recordLink) 
{
	ATLASSERT(recordLink.DBType() == dbActuator); // fake DBType == dbActu
	return GET_ADDRESS_OF_RECORD(recordLink)->castToActuator();
}

//^0CEE:0A2F
ObjectID SkWinCore::GET_NEXT_RECORD_LINK(ObjectID rl)
{
#if UseAltic
	ObjectID rl2 = GET_ADDRESS_OF_RECORD(rl)->w0;
	if (rl == rl2) {
		if (!SkCodeParam::bUsePowerDebug)// || (bUsePowerDebug && rl != OBJECT_END_MARKER && rl != OBJECT_NULL ) )
			ATLASSERT(rl != rl2);
		return OBJECT_END_MARKER;
	}
	return rl2;
#else
	//^0CEE:0A2F
	return GET_ADDRESS_OF_RECORD(rl)->w0;
#endif
}

GenericRecord *SkWinCore::GET_ADDRESS_OF_DETACHED_RECORD(ObjectID rl)
{
	ATLASSERT(rl < OBJECT_EFFECT_FIREBALL);	// oFF80
	ATLASSERT(dunHeader != NULL && rl.DBIndex() < dunHeader->nRecords[rl.RealDBType()]);

	GenericRecord *pr = reinterpret_cast<GenericRecord *>(glbDBObjectData[rl.RealDBType()] + dbSize[rl.RealDBType()] * rl.DBIndex());
	return pr;
}

//^0CEE:09DB
GenericRecord *SkWinCore::GET_ADDRESS_OF_RECORD(ObjectID rl)
{
	if (!SkCodeParam::bUsePowerDebug) {
		ATLASSERT(rl != OBJECT_END_MARKER && rl != OBJECT_NULL);
		//ATLASSERT(rl < oFF80);
		ATLASSERT(dunHeader != NULL && rl.DBIndex() < dunHeader->nRecords[rl.RealDBType()]);
	}
	// SPX: I disabled some checks because it is not buggy to ask for an unaddressed graphics. (DM2 would then display the yukman face)
	// MARK: on some games, some items are on index 1023 (meaning freed)
	// Effectively, that must not return a valid record since there is nothing.
	// Do we have a "dummy" record for that case ?
	else if (SkCodeParam::bUsePowerDebug && rl.DBIndex() >= 1023 ) {	// In case of specific DEBUG mode, give a dummy record and let's see what happen next ...
		static int countBadRecords = 0;
		static GenericRecord gr;
		gr.w0 = OBJECT_END_MARKER;
	SkD((DLV_BUGHERE, "DEBUG: %s is bad record %02d (RL = %04X DB=%d IDX=%04d)\n"
		, static_cast<LPCSTR>(getRecordNameOf(rl)), countBadRecords, rl, rl.RealDBType(), rl.DBIndex() ));
		countBadRecords++;
		//ATLASSERT(countBadRecords < 1024);
		return &gr;
	}

	//^0CEE:09DB
	GenericRecord *pr = reinterpret_cast<GenericRecord *>(glbDBObjectData[rl.RealDBType()] + dbSize[rl.RealDBType()] * rl.DBIndex());
	//SPX: I case of consuming food, the object will be already null, then assert will fail.
	//ATLASSERT(pr->w0 != OBJECT_NULL); // shouldn't be deallocated record!
	return pr;
}




//^0CEE:0A17
GenericRecord *SkWinCore::GET_ADDRESS_OF_TILE_RECORD(i16 xx, i16 yy)
{
	//^0CEE:0A17
	return GET_ADDRESS_OF_RECORD(GET_TILE_RECORD_LINK(xx, yy));
}

//^2066:1EA3
// SPX: Changing bits on a tile ... seems to be called for VOID PIT and LADDERS ...
//void SkWinCore::_2066_1ea3(Bit16u xx, Bit16u yy, Bit16u zz)
void SkWinCore::SET_TILE_ATTRIBUTE_02(Bit16u xx, Bit16u yy, Bit16u map)
{
	//^2066:1EA3
	//glbMapTileValue[zz][xx][yy] |= 0x02;
	glbMapTileValue[map][xx][yy] |= 0x02;
}


//^3E74:2598
// SPX: This is a modified version to allow custom graphics.dat input as exe parameter.
// Original method is still here labelled as ORIGINAL__GRAPHICS_DATA_OPEN
void SkWinCore::GRAPHICS_DATA_OPEN()
{
	Bit8u * sLocalGraphicsDatFileString = NULL;
	Bit8u * sLocalGraphicsDatFileString2 = NULL;

	sLocalGraphicsDatFileString = (Bit8u*) ptrGraphics;
	sLocalGraphicsDatFileString2 = (Bit8u*) ptrGraphics2;

	// default : ".Z020GRAPHICS.Z080.Z081.Z082.DAT"
	if (SkCodeParam::bUseFixedMode && (ptrGraphics != NULL && ptrGraphics[0] == 0))
	{
		//strcpy(ptrGraphics, ".Z020GRAPHICS.Z080.Z081.Z082.DAT");
		sLocalGraphicsDatFileString = (Bit8u*) strGraphics;
	}


	if (skwin.sCustomGraphicsDatFilename != NULL)
	{
		sLocalGraphicsDatFileString = (Bit8u*) skwin.sCustomGraphicsDatFilename;
	}

	//^3E74:2598
	//^3E74:259B
	if ((_4976_5d10++) == 0) {
		//^3E74:25A6
		glbFileHandleGraphics1 = FILE_OPEN(FORMAT_SKSTR(sLocalGraphicsDatFileString, NULL));
		//^3E74:25C6
		if (glbFileHandleGraphics1 < 0) {
			//^3E74:25CD
			RAISE_SYSERR(SYSTEM_ERROR__MISSING_GRAPHICS);	// SYS ERROR 41
		}
		//^3E74:25D5
		if (_4976_5c9c != 0 && _4976_5ca8 == 0) {
			//^3E74:25E3
			glbFileHandleGraphics2 = FILE_OPEN(FORMAT_SKSTR(sLocalGraphicsDatFileString2, NULL));
			//^3E74:2603
			if (glbFileHandleGraphics2 < 0) {
				//^3E74:260A
				RAISE_SYSERR(SYSTEM_ERROR__MISSING_GRAPHICS_2);	// SYS ERROR 31
			}
		}
	}
	//^3E74:2612
	return;
}

//^3E74:2598
// SPX: This is the original GRAPHICS_DATA_OPEN
void SkWinCore::ORIGINAL__GRAPHICS_DATA_OPEN()
{
	//^3E74:2598
	//^3E74:259B
	if ((_4976_5d10++) == 0) {
		//^3E74:25A6
		glbFileHandleGraphics1 = FILE_OPEN(FORMAT_SKSTR(ptrGraphics, NULL));
		//^3E74:25C6
		if (glbFileHandleGraphics1 < 0) {
			//^3E74:25CD
			RAISE_SYSERR(SYSTEM_ERROR__MISSING_GRAPHICS);
		}
		//^3E74:25D5
		if (_4976_5c9c != 0 && _4976_5ca8 == 0) {
			//^3E74:25E3
			glbFileHandleGraphics2 = FILE_OPEN(FORMAT_SKSTR(ptrGraphics2, NULL));
			//^3E74:2603
			if (glbFileHandleGraphics2 < 0) {
				//^3E74:260A
				RAISE_SYSERR(SYSTEM_ERROR__MISSING_GRAPHICS_2);
			}
		}
	}
	//^3E74:2612
	return;
}


//^098D:0098
SRECT *SkWinCore::ALLOC_TEMP_RECT(i16 x, i16 y, i16 cx, i16 cy)
{
	//^098D:0098
	//^098D:009C
	SRECT *bp04 = &_4976_4ba8[_4976_4ba6];
	//^098D:00AB
	_4976_4ba6++;
	//^098D:00B2
	if (_4976_4ba6 >= 4) {
		//^098D:00B7
		_4976_4ba6 = 0;
	}
	//^098D:00BD
	bp04->x = x;
	bp04->y = y;
	bp04->cx = cx;
	bp04->cy = cy;
	//^098D:00DB
	return bp04;
}

//^098D:00E3
SRECT *SkWinCore::ALLOC_TEMP_ORIGIN_RECT(Bit16u width, Bit16u height)
{
	//^098D:00E3
	//^098D:00E6
	return ALLOC_TEMP_RECT(0, 0, width, height);
}

//^3E74:5420
Bit16u SkWinCore::FIND_ICI_FROM_CACHE_HASH(Bit32u cacheHash, Bit16u *ici)
{
	//^3E74:5420
	//^3E74:5426
	i16 di = -1;
	i16 cx = _4976_5c92;

	while (true) {
		//^3E74:542E
		i16 si = (di + cx) >> 1;
		//^3E74:5436
		if (si == di) {
			//^3E74:543A
			*ici = si +1;
			//^3E74:5441
			return 0;
		}
		//^3E74:5445
		ATLASSERT(si < _4976_5d24);
		Bit32u bp04 = _4976_5c86[_4976_5c7e[si]];
		//^3E74:5468
		if (cacheHash < bp04) {
			//^3E74:547A
			cx = si;
			//^3E74:5478
			continue;
		}
		//^3E74:547E
		if (cacheHash > bp04) {
			//^3E74:5490
			di = si;
			//^3E74:5492
			continue;
		}
		//^3E74:5494
		*ici = si;
		//^3E74:549A
		return 1;
	}
}

//^3E74:4701
void SkWinCore::RECYCLE_MEMENTI(Bit16u mementi, Bit16u yy)
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:4701
	//^3E74:4707
	Bit16u di = mementi;
	//^3E74:470C
	mement *bp04 = tlbMementsPointers[di];
	TEST_MEMENT(bp04);
	//^3E74:4722
	if (bp04->w4() != 0xffff) {
		//^3E74:472C
		_3e74_48c9(di);
		//^3E74:4733
		return;
	}
	//^3E74:4736
	mement *bp0c;
	mement *bp10;
	if (yy != 0) {
		//^3E74:473C
		bp04->w4(0xfffe);
		bp0c = _4976_5c8c;

		//^3E74:4752
		//^3E74:4786
		bp10 = _4976_5d70;
	}
	else {
		//^3E74:4754
		bp04->w4(1);
		//^3E74:475D
		bp0c = _4976_5d90;
		//^3E74:476A
		bp10 = _4976_5c8c;
		if (bp10 == NULL) {
			//^3E74:4786
			bp10 = _4976_5d70;
		}
	}

	//^3E74:4793
	if (bp0c != NULL) {
		//^3E74:47A4
		goto _483a;
	}
	//^3E74:47A7
	Bit16u bp12;
	Bit16u si;
	if (_4976_5d70 == NULL) {
		//^3E74:47B9
		bp04->w6(0xffff);
		_4976_5d5e = bp04;
		//^3E74:47CF
		goto _4822;
	}
	//^3E74:47D1
	if (_4976_5d90 == NULL) {
		//^3E74:47E3
		if (_4976_5c8c != NULL) {
			//^3E74:47F5
			bp0c = bp10;
			//^3E74:4801
			goto _483a;
		}
	}
	//^3E74:4803
	bp10->w8(di);
	bp12 = bp10->w10();
	//^3E74:4811
	si = QUERY_MEMENTI_FROM(bp12);
	//^3E74:481B
	bp04->w6(si);
	//^3E74:4822
_4822:
	bp04->w8(0xffff);
	_4976_5d70 = bp04;
	//^3E74:4838
	goto _48a3;

	//^3E74:483A
_483a:
	si = bp0c->w6();
	bp0c->w6(di);
	bp04->w6(si);
	//^3E74:484C
	Bit16u bp14;
	if (si != 0xffff) {
		//^3E74:4851
		mement *bp08 = tlbMementsPointers[si];
		TEST_MEMENT(bp08);
		//^3E74:4869
		bp04->w8(bp08->w8());
		//^3E74:4877
		bp08->w8(di);
		//^3E74:487E
		goto _48a3;
	}
	//^3E74:4880
	bp14 = QUERY_MEMENTI_FROM(bp0c->w10());
	//^3E74:488F
	bp04->w8(bp14);
	//^3E74:4896
	_4976_5d5e = bp04;

	//^3E74:48A3
_48a3:
	if (yy != 0) {
		//^3E74:48A9
		_4976_5c8c = bp04;
	}
	else {
		//^3E74:48B8
		_4976_5d90 = bp04;
	}
	//^3E74:48C5
	return;
}

// SPX: changed "void" to "int" in order to allow more code on return
int SkWinCore::TEST_MEMENT(mement *bp04)
{
	i32	iDW0ValLen = 0;
	i32 len = 0;
	//ATLASSERT(bp04 != NULL);
	// SPX: use CheckSafePointer to trap more unexpected mem values so that it fails at ASSERT instead of crashing after.
	ATLASSERT(CheckSafePointer(bp04));
	iDW0ValLen = (abs(-bp04->dw0())) -4;
	if (iDW0ValLen >= 65536)
		return 0;
	len = READ_I32(bp04,iDW0ValLen);
	if (SkCodeParam::bUseFixedMode)
	{
		if (bp04->dw0() != len)
		{
			len = bp04->dw0();
		}

	}
	ATLASSERT(bp04->dw0() == len);
	return 1; // test passed
}

//^3E74:48C9
mement *SkWinCore::_3e74_48c9(Bit16u mementi)
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:48C9
	//^3E74:48CF
	mement *bp04 = tlbMementsPointers[mementi];
	TEST_MEMENT(bp04);
	//^3E74:48E8
	Bit16u si = bp04->w4();
	//^3E74:48EF
	if (si == 0xffff || si == 0xfffe) {
		//^3E74:48F9
		return bp04;
	}
	//^3E74:4902
	mement *bp0c = bp04;
	//^3E74:490E
	if (si == 0) {
		//^3E74:4915
		Bit16u di = bp04->w8();
		//^3E74:491E
		if (di == 0xffff) {
			//^3E74:4923
			_4976_5d90 = (bp04 != _4976_5c8c) ? bp04 : NULL;
			//^3E74:494B
			bp04->w4(1);
			//^3E74:4954
			//^3E74:48F9
			return bp04;
		}
		//^3E74:4956
		mement *bp08 = tlbMementsPointers[di];
		TEST_MEMENT(bp08);
		//^3E74:496E
		Bit16u bp0e = bp04->w6();
		//^3E74:4978
		if (bp0e == 0xffff) {
			//^3E74:497D
			_4976_5d5e = bp08;
		}
		else {
			//^3E74:498C
			bp04 = tlbMementsPointers[bp0e];
			TEST_MEMENT(bp04);
			//^3E74:49A5
			bp04->w8(di);
		}
		//^3E74:49AC
		bp08->w6(bp0e);
		//^3E74:49B6
		bp0c->w4(0xffff);
		//^3E74:49BF
		RECYCLE_MEMENTI(mementi, 0);
		//^3E74:49CA
	}
	else {
		//^3E74:49CD
		if (si < 0xfffd) {
			//^3E74:49D2
			si++;
		}
		//^3E74:49D3
		bp04->w4(si);
		//^3E74:49DA
		if (bp04 == _4976_5d70) {
			//^3E74:49EF
			//^3E74:48F9
			return bp04;
		}
		//^3E74:49F2
		Bit16u di = bp04->w8();
		//^3E74:49FB
		mement *bp08 = tlbMementsPointers[di];
		TEST_MEMENT(bp08);
		//^3E74:4A11
		if (bp08->w4() >= si) {
			//^3E74:4A1A
			//^3E74:48F9
			return bp04;
		}
		//^3E74:4A1D
		Bit16u bp0e = bp04->w6();
		//^3E74:4A27
		if (bp0e == 0xffff) {
			//^3E74:4A2C
			_4976_5d90 = bp08;
			_4976_5d5e = bp08;
			//^3E74:4A40
			bp08->w6(0xffff);
		}
		else {
			//^3E74:4A4B
			if (bp04 == _4976_5d90) {
				//^3E74:4A5F
				_4976_5d90 = bp08;
			}
			//^3E74:4A6C
			bp04 = tlbMementsPointers[bp0e];
			TEST_MEMENT(bp04);
			//^3E74:4A85
			bp04->w8(di);
			bp08->w6(bp0e);
		}
		//^3E74:4A96
		if (_4976_5d90 == _4976_5c8c) {
			//^3E74:4AAC
			_4976_5d90 = NULL;
		}

		while (true) {
			//^3E74:4AB8
			bp04 = bp08;
			bp0e = di;
			di = bp04->w8();
			//^3E74:4AD0
			if (di == 0xffff) {
				//^3E74:4AD5
				bp04->w8(mementi);
				//^3E74:4ADC
				bp0c->w6(bp0e);
				bp0c->w8(0xffff);
				//^3E74:4AEC
				_4976_5d70 = bp0c;
			}
			else {
				//^3E74:4AFB
				bp08 = tlbMementsPointers[di];
				TEST_MEMENT(bp08);
				//^3E74:4B13
				if (bp08->w4() < si)
					//^3E74:4B1A
					continue;
				//^3E74:4B1C
				bp04->w8(mementi);
				bp0c->w6(bp0e);
				bp0c->w8(di);
				bp08->w6(mementi);
			}
			break;
		}
	}
	//^3E74:4B3E
	return bp0c;
}

//^3E74:5561
void SkWinCore::FREE_CACHE_INDEX(Bit16u cacheIndex)
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:5561
	//^3E74:5566
    i16 si = cacheIndex;
		//^3E74:5569
	if (_4976_5d36 == -1 || _4976_5d36 > si) {
		//^3E74:5576
		_4976_5d36 = si;
	}
	//^3E74:557A
	ATLASSERT(si < _4976_5d24);
	Bit32u bp04 = _4976_5c86[si];
	//^3E74:5592
	Bit16u bp06;
	if (FIND_ICI_FROM_CACHE_HASH(bp04, &bp06) != 0) {
		//^3E74:55A8
		_4976_5c86[si] = 0;
		//^3E74:55BE
		_4976_5c92--;
		//^3E74:55C2
		MOVE_MEMORY(
			&_4976_5c7e[bp06 +1],
			&_4976_5c7e[bp06],
			(_4976_5c92 -bp06) << 1
			);
	}
	//^3E74:55F6
	return;
}

//^3E74:4549
void SkWinCore::_3e74_4549(Bit16u xx)
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:4549
	//^3E74:454F
	mement *bp04 = tlbMementsPointers[xx];
	TEST_MEMENT(bp04);
	//^3E74:4568
	if (bp04 != NULL) {
		//^3E74:457C
		if (bp04->w4() != 0xffff) {
			//^3E74:4589
			Bit16u di = bp04->w6();
			Bit16u si = bp04->w8();
			mement *bp08;
			//^3E74:4594
			if (di == 0xffff) {
				//^3E74:4599
				if (si == 0xffff) {
					//^3E74:459E
					_4976_5d90 = NULL;
					_4976_5c8c = NULL;
					_4976_5d70 = NULL;
					_4976_5d5e = NULL;
				}
				else {
					//^3E74:45C1
					bp08 = tlbMementsPointers[si];
					TEST_MEMENT(bp08);
					//^3E74:45D9
					_4976_5d5e = bp08;
					//^3E74:45E0
					bp08->w6(0xffff);
					//^3E74:45E9
					goto _4682;
				}
			}
			else {
				//^3E74:45EC
				bp08 = tlbMementsPointers[di];
				TEST_MEMENT(bp08);
				//^3E74:4604
				bp08->w8(si);
				//^3E74:460B
				if (si == 0xffff) {
					//^3E74:4610
					if (bp04 == _4976_5c8c) {
						//^3E74:4625
						_4976_5c8c = NULL;
					}
					else {
						//^3E74:4633
						if (bp04 == _4976_5d90) {
							//^3E74:4648
							_4976_5d90 = NULL;
						}
					}
					//^3E74:4654
					_4976_5d70 = bp08;
				}
				else {
					//^3E74:4663
					bp08 = tlbMementsPointers[si];
					TEST_MEMENT(bp08);
					//^3E74:467B
					bp08->w6(di);
				}
				//^3E74:4682
_4682:
				if (bp04 == _4976_5d90) {
					//^3E74:4696
					if (bp08 == _4976_5c8c) {
						//^3E74:46AB
						_4976_5d90 = NULL;
					}
					else {
						//^3E74:46B9
						_4976_5d90 = bp08;
					}
				}
				else {
					//^3E74:46C8
					if (_4976_5c8c == bp04) {
						//^3E74:46DC
						_4976_5c8c = bp08;
					}
				}
			}
		}
	}
	//^3E74:46E9
	bp04->w8(0xffff);
	bp04->w6(0xffff);
	bp04->w4(0xffff);
	//^3E74:46FD
	return;
}

//^3E74:0C8C
void SkWinCore::_3e74_0c8c(mement *ref)
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:0C8C
	//^3E74:0C90
	mement *bp08 = ref->pv4();
	mement *bp04 = ref->pv8();
	//^3E74:0CAF
	if (bp08 == NULL) {
		//^3E74:0CC0
		if (bp04 == NULL) {
			//^3E74:0CD1
			_4976_5d94 = NULL;
		}
		else {
			//^3E74:0CDF
			_4976_5d94 = bp04;
			//^3E74:0CEC
			bp04->pv4(NULL);
		}
	}
	else {
		//^3E74:0CFD
		bp08->pv8(bp04);
		//^3E74:0D0E
		if (bp04 != NULL) {
			//^3E74:0D1F
			bp04->pv4(bp08);
		}
	}
	//^3E74:0D30
}

//^3E74:0D32
void SkWinCore::_3e74_0d32(mement *ref)
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:0D32
	//^3E74:0D37
	mement *bp04 = ref;
	//^3E74:0D43
	if (_4976_5d94 == NULL) {
		//^3E74:0D55
		_4976_5d94 = bp04;
		//^3E74:0D62
		bp04->pv8(NULL);
		//^3E74:0D71
_0d71:
		bp04->pv4(NULL);
		//^3E74:0D79
		return;
	}
	//^3E74:0D7C
	mement *bp08 = _4976_5d94;
	//^3E74:0D89
	i32 bp0c = bp04->dw0();
	//^3E74:0D99
	if (bp08->dw0() <= bp0c) {
		//^3E74:0DAF
		_4976_5d94 = bp04;
		//^3E74:0DBC
		bp04->pv4(NULL);
		//^3E74:0DCB
		bp04->pv8(bp08);
		//^3E74:0DD9
		bp08->pv4(bp04);
		//^3E74:0DE2
		goto _0d71;
	}

	//^3E74:0DE4
	Bit16u si = 0;
	//^3E74:0DE6
	while ((bp04 = bp08->pv8()) != NULL) {
		//^3E74:0DE8
		if (bp04->dw0() <= bp0c) {
			//^3E74:0DFE
			bp04->pv4(ref);
			//^3E74:0E0F
			bp08->pv8(ref);
			//^3E74:0E1A
			ref->pv4(bp08);
			//^3E74:0E2B
			ref->pv8(bp04);
			//^3E74:0E39
			si = 1;
			//^3E74:0E3C
			break;
		}
		//^3E74:0E3E
		bp08 = bp04;
		//^3E74:0E4A
	}
	//^3E74:0E6D
	if (si == 0) {
		//^3E74:0E71
		bp08->pv8(ref);
		//^3E74:0E82
		ref->pv4(bp08);
		//^3E74:0E93
		ref->pv8(NULL);
	}
	//^3E74:0E9F
	return;
}

bool SkWinCore::ValidateMements(bool display = false) {
	bool ok = true;
	X16 i = 0;
	for (U16 x = 0; x < glbNumberOfMements; x++) {
		mement *p = tlbMementsPointers[x];
		if (p != NULL) {
			if (display) SkD((DLV_CPX, "M#%03d %p ", x, p));
			int v0 = -p->dw0();
			int v1 = -READ_I32(p,-p->dw0() -4);
			if (v0 != v1) 
				ok = false;
			if (display) SkD((DLV_CPX, "%11d %11d %c ", v0, v1, (v0==v1)?'|':'#'));
			if (display) {
				int ref1 = -1;
				for (i = 0; i < glbGDatNumberOfData; i++) {
					if (_4976_5c82[i] == x) {
						ref1 = i;
						break;
					}
				}
				SkD((DLV_CPX, "%4d ", ref1));
				int ref2 = 0;
				for (i = 0; i < _4976_5d24; i++) {
					if (_4976_5d08[i] == x) {
						ref2 = _4976_5c86[i];
						break;
					}
				}
				SkD((DLV_CPX, "%08X ", ref2));
			}
			if (display) SkD((DLV_CPX, "\n"));
		}
	}
	return ok;
}

//^3E74:28DE
void SkWinCore::FREE_INDEXED_MEMENT(Bit16u index)
{
	// index: if (index&0x8000)==0x8000, cacheindex.
	// index: if (index&0x8000)==0x0000, raw data index.

	// cqOk @ 21:03 2007/02/21

	//^3E74:28DE
	//^3E74:28E4
	Bit16u di = index;
	//^3E74:28E7
	if (di == _4976_4809) {
		//^3E74:28ED
		_4976_4809 = 0xffff;
	}
	//^3E74:28F3
	Bit16u si = QUERY_MEMENTI_FROM(di);
	//^3E74:28FB
	if (si == 0xffff)
		//^3E74:2900
		return;
	//^3E74:2903
	if ((di & 0x8000) == 0) {
		//^3E74:2909
		_4976_5c82[di] = 0xffff;
	}
	else {
		//^3E74:291A
		di &= 0x7fff;
		//^3E74:291E
		ATLASSERT(di < _4976_5d24);
		_4976_5d08[di] = 0xffff;
		//^3E74:292D
		if (_4976_5c90 == 0) {
			//^3E74:2934
			FREE_CACHE_INDEX(di);
		}
	}
	//^3E74:293B
	_3e74_4549(si);
	//^3E74:2942
	mement *bp04 = tlbMementsPointers[si];
	TEST_MEMENT(bp04);
	//^3E74:295A
	tlbMementsPointers[si] = NULL;
	//^3E74:2970
	_4976_5c9e--;
	//^3E74:2974
	if (si < _4976_5c78 || _4976_5c78 == 0xffff) {
		//^3E74:2981
		_4976_5c78 = si;
	}
	//^3E74:2985
	i32 bp0c = -bp04->dw0();
	//^3E74:299C
	_4976_5cf8 += bp0c;
	SkD((DLV_CPX,"CPX: CPX Free  Ci#%02d Mi#%02d (%5u) = %p\n"
		, (index & 0x8000) ? (index & 0x7fff) : -1
		, si
		, bp0c
		, bp04));
	//^3E74:29A7
	if (bp04 != _4976_5ce6) {
		//^3E74:29BC
		i32 bp10 = READ_I32(bp04,-4);
		ATLASSERT(-BUFF_SIZE_MAX < bp10 && bp10 < BUFF_SIZE_MAX);
		//ATLASSERT(-60000 < bp10 && bp10 < 60000);
		//^3E74:29DE
		if (bp10 > 0) {
			//^3E74:29E8
			PTR_PSBA(bp04,bp10);
			TEST_MEMENT(bp04);
			//^3E74:29F8
			_3e74_0c8c(bp04);
			//^3E74:2A04
			bp0c += bp10;
		}
	}
	//^3E74:2A10
	if ((void *)PTR_PADD(bp04,bp0c) == (void *)_4976_5cb2) {
		//^3E74:2A30
		_4976_5cb2 = bp04;
		return;
	}
	//^3E74:2A40
	mement *bp08 = reinterpret_cast<mement *>(PTR_PADD(bp04,bp0c));
	TEST_MEMENT(bp08);
	//^3E74:2A59
	i32 bp10 = bp08->dw0();
	//^3E74:2A68
	if (bp10 > 0) {
		//^3E74:2A72
		_3e74_0c8c(bp08);
		//^3E74:2A7E
		bp0c += bp10;
	}
	//^3E74:2A8A
	bp04->dw0(bp0c);
	//^3E74:2A9A
	WRITE_I32(bp04,bp0c -4,bp0c);
	//^3E74:2AC9
	_3e74_0d32(bp04);
	//^3E74:2AD5
	return;
}

//^3E74:4471
Bit16u SkWinCore::_3e74_4471()
{
	//^3E74:4471
	//^3E74:4475
	i16 si = _4976_5d36;
	//^3E74:447A
	if (si >= 0) {
		//^3E74:447E
		_4976_5c92++;
		//^3E74:4482
		if (_4976_5c92 >= _4976_5d24) {
			//^3E74:448B
			_4976_5d36 = -1;
		}
		else {
			do {
				//^3E74:4493
				_4976_5d36++;
				//^3E74:4497
				ATLASSERT(_4976_5d36 < _4976_5d24);
			} while (_4976_5d08[_4976_5d36] != 0xffff);
		}
	}
	//^3E74:44A8
	ATLASSERT(0 <= si && si < _4976_5d24);
	return si;
}

//^3E74:54A1
Bit16u SkWinCore::INSERT_CACHE_HASH_AT(Bit32u cacheHash, Bit16u ici)
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:54A1
	//^3E74:54A6
	if (_4976_5c92 == _4976_5d24) {
		//^3E74:54AF
		mement *bp04 = _4976_5d5e;
		//^3E74:54B6
		for (; (bp04->w10() & 0x8000) == 0; ) {
			//^3E74:54B8
			bp04 = tlbMementsPointers[bp04->w8()];
			TEST_MEMENT(bp04);
		}
		//{
		//	mement *p = _4976_5d5e;
		//	while (p != NULL) {
		//		SkD((DLV_CPX, "ME: %p I %04X  Mi %04X \n", p, p->w10(), p->w8()));
		//		p = tlbMementsPointers[p->w8()];
		//	}
		//	for (U16 x=0; x<_4976_5c92; x++) {
		//		U16 i = _4976_5c7e[x];
		//		U32 hash = _4976_5c86[i];
		//		SkD((DLV_CPX, "HI: %04X %04X %08X \n", x, i, hash));
		//	}
		//}

		//^3E74:54E0
		FREE_INDEXED_MEMENT(bp04->w10());
		//^3E74:54E9
		FIND_ICI_FROM_CACHE_HASH(cacheHash, &ici);
	}
	//^3E74:54FB
	Bit16u si = _3e74_4471();
	//^3E74:5501
	MOVE_MEMORY(
		&_4976_5c7e[ici],
		&_4976_5c7e[ici +1],
		(_4976_5c92 -ici -1) << 1
		);
	//^3E74:5536
	_4976_5c7e[ici] = si;
	//^3E74:5544
	_4976_5c86[si] = cacheHash;
	//^3E74:555C
	return si;
}

//^3E74:44AD
void SkWinCore::_3e74_44ad()
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:44AD
	_4976_5d2a = glbGameTick;
	_4976_5d90 = NULL;
	mement *bp04 = _4976_5c8c;
	//^3E74:44E6
	if (bp04 == NULL) {
		//^3E74:44E8
		bp04 = _4976_5d70;
		//^3E74:4502
		if (bp04 == NULL)
			return;
	}
	//^3E74:4504
	while (bp04->w4() != 0) {
		//^3E74:4506
		if (bp04->w4() <= 0xfffd) {
			//^3E74:4510
			bp04->w4(0);
		}
		//^3E74:4516
		Bit16u si = bp04->w6();
		if (si == 0xffff)
			return;
		//^3E74:4524
		bp04 = tlbMementsPointers[si];
		TEST_MEMENT(bp04);
		//^3E74:453C
	}
}

//^3E74:0C62
Bit16u SkWinCore::QUERY_MEMENTI_FROM(Bit16u xx)
{
	// xx&0x8000==0x8000: cacheindex-to-mementi
	// xx&0x8000==0x0000: raw data index to mementi

	// returns 0xffff if no match found.

	//^3E74:0C62
	Bit16u si = xx;
	if ((si & 0x8000) != 0) {
		if (SkCodeParam::bUseFixedMode && (si & 0x7fff) >= 128)	// SPX: protection to avoid further invalid pointer
			return 0xFFFF;
		//^3E74:0C6F
		return _4976_5d08[si & 0x7fff];
	}
	else {
		//^3E74:0C7C
		if (SkCodeParam::bUseFixedMode && (si & 0x7fff) >= glbGDatNumberOfData) // SPX: protection to avoid further invalid pointer
			return 0xFFFF;
		return _4976_5c82[si];
	}
}

//^3E74:55F9
Bit16u SkWinCore::ADD_CACHE_HASH(Bit32u cacheHash, Bit16u *piYaCacheIndex)
{
	// returns 0 if inserted new one, 1 if already exists.

	//^3E74:55F9
	//^3E74:55FE
	if (glbGameTick != _4976_5d2a) {
		//^3E74:5611
		_3e74_44ad();
	}
	//^3E74:5615
	Bit16u bp02;
	if (FIND_ICI_FROM_CACHE_HASH(cacheHash, &bp02) != 0) {
		//^3E74:562B
		*piYaCacheIndex = _4976_5c7e[bp02];
		//^3E74:563F
		Bit16u si = QUERY_MEMENTI_FROM(*piYaCacheIndex | 0x8000);
		//^3E74:564D
		_3e74_48c9(si);
		//^3E74:5653
		return 1;
	}
	//^3E74:5658
	*piYaCacheIndex = INSERT_CACHE_HASH_AT(cacheHash, bp02);
	//^3E74:566E
	return 0;
}

//^3E74:5817
U8 *SkWinCore::QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(Bit16u cacheIndex)
{
	//^3E74:581A
    return reinterpret_cast<U8 *>(&_3e74_48c9(QUERY_MEMENTI_FROM(cacheIndex | 0x8000))[1]); // +18 bytes
}

//^3E74:583A
void SkWinCore::_3e74_583a(Bit16u xx)
{
	//^3E74:583A
	//^3E74:583E
	Bit16u si = QUERY_MEMENTI_FROM(xx | 0x8000);
	//^3E74:584C
	if (si != 0xffff) {
		//^3E74:5851
		_3e74_4549(si);
	}
	//^3E74:5857
	return;
}

//^3E74:4416
Bit16u SkWinCore::FIND_FREE_MEMENTI()
{
	//^3E74:4416
	//^3E74:441A
	Bit16u si = _4976_5c78;
	//^3E74:441F
	if (si == 0xffff) {
		//^3E74:4424
		FREE_INDEXED_MEMENT(_4976_5d5e->w10());
		//^3E74:4431
		si = _4976_5c78;
	}
	//^3E74:4435
	_4976_5c9e++;
	//^3E74:4439
	if (_4976_5c9e >= glbNumberOfMements) {
		//^3E74:4442
		_4976_5c78 = 0xffff;
	}
	else {
		do {
			//^3E74:444A
			_4976_5c78++;
			//^3E74:444E
			ATLASSERT(_4976_5c78 < glbNumberOfMements);
		} while (tlbMementsPointers[_4976_5c78] != NULL);
	}
	//^3E74:446C
	return si;
}

//^3E74:2AD9
void SkWinCore::GUARANTEE_FREE_CPXHEAP_SIZE(i32 buffSize)
{
	ATLASSERT(buffSize > 0 && buffSize < BUFF_SIZE_MAX);

	//^3E74:2AD9
	//^3E74:2ADC
	for (; _4976_5cf8 < buffSize; ) {
		//^3E74:2ADE
		if (_4976_5d5e == NULL) {
			//^3E74:2AF0
			RAISE_SYSERR(SYSTEM_ERROR__FREE_CPXHEAP_SIZE);
		}
		//^3E74:2AF8
		if (_4976_4809 != 0xffff && _4976_4809 != _4976_4807) {
			//^3E74:2B08
			FREE_INDEXED_MEMENT(_4976_4809);
		}
		else {
			//^3E74:2B0E
			FREE_INDEXED_MEMENT(_4976_5d5e->w10());
		}
		//^3E74:2B1B
	}
	//^3E74:2B2E
	return;
}

//^3E74:2B30
void SkWinCore::_3e74_2b30()
{
	int iDebugLoopCount = 0;
	// cqOk @ 21:03 2007/02/21

	//^3E74:2B30
	//^3E74:2B36
	if (_4976_5d94 != NULL) {	// mement
		//^3E74:2B4B
		mement *bp04 = _4976_5ce6;
		mement *bp08 = _4976_5ce6;

		do {
			//^3E74:2B5E
			i32 bp0c = bp04->dw0();
			//printf("LOAD_DYN4>_3e74_2b30[MEMENT] (#%05d) bp0c (length) = %d\n", iDebugLoopCount, bp0c);
			// SPX: we can get stuck in the loop, let's assume that after 1000 loop we try to get out, or if the length is ridiculously too much
			if (SkCodeParam::bUseFixedMode && (iDebugLoopCount > 1000 || bp0c > 1024000))
				break;
			//^3E74:2B6E
			if (bp0c > 0) {
				//^3E74:2B78
				PTR_PADA(bp04, bp0c);
			}
			else {
				//^3E74:2B86
				bp0c = -bp0c;
				//^3E74:2B99
				if (bp08 != bp04) {
					//^3E74:2BAF
					Bit16u di = bp04->w10();
					//^3E74:2BB6
					Bit16u si = QUERY_MEMENTI_FROM(di);
					//^3E74:2BBE
					tlbMementsPointers[si] = bp08;
					//^3E74:2BD4
					COPY_MEMORY(bp04, bp08, bp0c);
					TEST_MEMENT(bp08);	// SPX TODO this part is prone to fail while changing level
					//^3E74:2BEE
					if (_4976_5d5e == bp04) {
						//^3E74:2C02
						_4976_5d5e = bp08;
					}
					//^3E74:2C0F
					if (_4976_5d70 == bp04) {
						//^3E74:2C23
						_4976_5d70 = bp08;
					}
					//^3E74:2C30
					if (_4976_5d90 == bp04) {
						//^3E74:2C44
						_4976_5d90 = bp08;
					}
					//^3E74:2C53
					else if (_4976_5c8c == bp04) {
						//^3E74:2C67
						_4976_5c8c = bp08;
					}
				}
				//^3E74:2C74
				PTR_PADA(bp04, bp0c);
				//^3E74:2C84
				PTR_PADA(bp08, bp0c);
			}
			iDebugLoopCount++; // debug
			//^3E74:2C94
		} while (bp04 != _4976_5cb2);	// at some point, this becomes impossible when bp04 never changes and we get stuck in the loop

		//^3E74:2CAC
		_4976_5cb2 = bp08;
		//^3E74:2CB9
		_4976_5d94 = NULL;
	}
	//^3E74:2CC5
	return;
}

//^3E74:2CC9
mement *SkWinCore::ALLOC_LOWER_CPXHEAP(i32 buffSize)
{
	// allocate memory from lower part of complex memory pool.
	// buffSize must include size of header&footer: sizeof(mement)+4 = 22
	ATLASSERT(buffSize > 0 && buffSize < BUFF_SIZE_MAX);

	//^3E74:2CC9
	//^3E74:2CCF
	GUARANTEE_FREE_CPXHEAP_SIZE(buffSize);
	//^3E74:2CDB
	mement *bp04;
	if (PTR_PSBP(_4976_5ce2,_4976_5cb2) >= buffSize) { // allocate on main memory
		//^3E74:2CFB
_2cfb:
		bp04 = _4976_5cb2;
		//^3E74:2D08
		PTR_PADA(_4976_5cb2,buffSize);
	}
	//^3E74:2D1B
	else if (_4976_5d94->dw0() < buffSize) {
		//^3E74:2D32
		_3e74_2b30();
		//^3E74:2D36
		goto _2cfb;
	}
	else {
		//^3E74:2D38
		bp04 = _4976_5d94;
		//^3E74:2D45
		Bit16u si = 0;
		mement *bp08;

		do {
			//^3E74:2D47
			if (bp04->dw0() == buffSize) {
				//^3E74:2D5B
_2d5b:
				si = 1;
				//^3E74:2D5E
				continue;
			}
			//^3E74:2D60
			if (bp04->dw0() >= buffSize) {
				//^3E74:2D76
				bp08 = bp04->pv8();
				//^3E74:2D8B
				if (bp08 != NULL)
					//^3E74:2D94
					goto _2da5;
			}
			//^3E74:2D96
			bp04 = _4976_5d94;
			//^3E74:2DA3
			goto _2d5b;
			//^3E74:2DA5
_2da5:
			bp04 = bp08;
			//^3E74:2DB1
		} while (si == 0);

		//^3E74:2DB5
		_3e74_0c8c(bp04);
		//^3E74:2DC1
		i32 bp0c = bp04->dw0() - buffSize;
		//^3E74:2DD7
		if (bp0c >= 30) {
			//^3E74:2DE5
			bp08 = reinterpret_cast<mement *>(PTR_PADD(bp04,buffSize));
			//^3E74:2DFC
			WRITE_I32(bp08,bp0c -4,bp0c);
			//^3E74:2E2A
			bp08->dw0(bp0c);
			//^3E74:2E3A
			_3e74_0d32(bp08);
		}
		else {
			//^3E74:2E42
			buffSize = bp04->dw0();
		}
	}
	//^3E74:2E52
	_4976_5cf8 -= buffSize;
	//^3E74:2E60
	bp04->dw0(-buffSize);
	//^3E74:2E71
	WRITE_I32(bp04,+buffSize -sizeof(i32),-buffSize);
	//^3E74:2EA9
	reinterpret_cast<mement *>(bp04)->w8(0xffff);
	reinterpret_cast<mement *>(bp04)->w6(0xffff);
	reinterpret_cast<mement *>(bp04)->w4(0xffff);
	//^3E74:2EBD
#if UseAltic
	{
		i32 size = -READ_I32(bp04,-bp04->dw0() -4);
		ATLASSERT(buffSize == size);

		// SkD((DLV_CPX, "CPX: ALLOC_LOWER_CPXHEAP(%6u) = %p\n", (Bitu)buffSize, bp04));
		memset(&bp04[1], 0xcc, buffSize -sizeof(mement) -sizeof(i32));
		// ATLASSERT((U32)bp04 != 0x00758132);
	}
#endif
	return reinterpret_cast<mement *>(bp04);
}

//^3E74:5708
Bit8u *SkWinCore::ALLOC_CPXHEAP_MEM(Bit16u index, Bit32u buffSize)
{
	//^3E74:5708
	//^3E74:570D
	buffSize += sizeof(mement) +4;
	//^3E74:5715
	if ((buffSize & 1) != 0) {
		//^3E74:5720
		buffSize++;
	}
	//^3E74:5728
	mement *bp04 = ALLOC_LOWER_CPXHEAP(buffSize);
	//^3E74:573B
	Bit16u si = FIND_FREE_MEMENTI();
	//^3E74:5740
	ATLASSERT(_4976_5d08[index] == U16(-1));
	_4976_5d08[index] = si;
	//^3E74:574E
	ATLASSERT(tlbMementsPointers[si] == 0);
	tlbMementsPointers[si] = bp04;
	//^3E74:5766
	bp04->w10(index | 0x8000);

	SkD((DLV_CPX,"DBG: CPX Alloc Ci#%02d Mi#%02d (%5u) = %p\n", index, si, 0U +buffSize, bp04));

	//^3E74:5773
	return reinterpret_cast<Bit8u *>(&bp04[1]); // +18 bytes
}

//^3E74:585A
void SkWinCore::_3e74_585a(Bit16u xx, Bit16u yy)
{
	//^3E74:585A
	//^3E74:585E
	Bit16u si = QUERY_MEMENTI_FROM(xx | 0x8000);
	//^3E74:586C
	if (si == 0xffff) {
		//^3E74:5871
		FREE_CACHE_INDEX(xx);
	}
	else {
		//^3E74:587B
		RECYCLE_MEMENTI(si, yy);
	}
}

//^44C8:1D26
void SkWinCore::FILL_ENTIRE_PICT(Bit8u *buff, Bit16u fill)
{
	//^44C8:1D26
	ENTER(8);
	//^44C8:1D2B
	X16 si = READ_UI16(buff,-6);
	SRECT bp08;
	bp08.x = bp08.y = 0;
	bp08.cx = (((si == 4) ? 2 : 1) +READ_UI16(buff,-4) -1) & (~(((si == 4) ? 2 : 1) -1));
	bp08.cy = READ_UI16(buff,-2);
	FIRE_FILL_RECT_ANY(buff, &bp08, fill, bp08.cx, si);
	//^44C8:1D89
	return;
}

//^443C:085A
void SkWinCore::FIRE_HIDE_MOUSE_CURSOR()
{
	//^443C:085A
	//^443C:085D
	glbMouseVisibility++;
	//^443C:0861
	_01b0_0adb() CALL_IBMIO;
}

//^01B0:0ADB
void SkWinCore::_01b0_0adb() //#DS=04BF
{
	//^01B0:0ADB
	LOADDS(0x3083);
	//^01B0:0AE4
	if (_04bf_17a2++ == 0) {
		//^01B0:0AF1
		LOCK_MOUSE_EVENT();
		_01b0_073d();
		UNLOCK_MOUSE_EVENT();
	}
	//^01B0:0AFD
	return;
}

// Standard DM1 palette
Bit8u _xDefaultFMTownsPalette[16*4] = 
{
	0, 0x00, 0x00, 0x00,
	1, 0x66, 0x66, 0x66,
	2, 0x88, 0x88, 0x88,
	3, 0x66, 0x22, 0x00,
	4, 0x00, 0xCC, 0xCC,
	5, 0x88, 0x44, 0x00,
	6, 0x00, 0x88, 0x00,
	7, 0x00, 0xCC, 0x00,
	8, 0xFF, 0x00, 0x00,
	9, 0xFF, 0xAA, 0x00,
	10, 0xCC, 0x88, 0x66,
	11, 0xFF, 0xFF, 0x00,
	12, 0x44, 0x44, 0x44,
	13, 0xAA, 0xAA, 0xAA,
	14, 0x00, 0x00, 0xFF,
	15, 0xFF, 0xFF, 0xFF,

};

//^01B0:0B01
void SkWinCore::IBMIO_SET_CURSOR_PATTERN(
	i16 index, 
	Bit8u *buffSrc, 
	Bit8u zz, 
	Bit8u ss, 
	Bit16u srcWidth, 
	Bit16u srcHeight, 
	Bit16u srcBits, 
	Bit8u *localPal, 
	Bit16u colorkey
) { 
	// SPX: If localPal is not specified (can happen from a V3 GDAT (FM-Towns, SegaCD/MegaCD), it will then crash
	if (localPal == NULL)
		localPal = _xDefaultFMTownsPalette;
	
	//#DS=04BF
	//^01B0:0B01
	//^01B0:0B07
	LOADDS(0x3083);
	//^01B0:0B0D
	//^01B0:0B0F
	if (index >= 0 && index < 4) {
		//^01B0:0B25
		LOCK_MOUSE_EVENT();
		//^01B0:0B29
		sk0e80 *bp08 = &_04bf_0e80[index];
		//^01B0:0B3A
		if (srcBits == 4) {
			//^01B0:0B45
			Bit16u di = 0;
			//^01B0:0B47
			Bit16u bp0a = (srcWidth +1) & 0xfffe;
			Bit16u bp04 = 0;

			//^01B0:0B56
			for (; bp04 < srcHeight; bp04++) {
				//^01B0:0B59
				Bit16u bp02 = 0;
				//^01B0:0B5E
				for (; bp02 < srcWidth; di++, bp02++) {
					//^01B0:0B60
					Bit16u si = bp0a * bp04 + bp02;
					//^01B0:0B6B
					if ((si & 0x0001) != 0) {
						//^01B0:0B71
						bp08->b6[di] = localPal[buffSrc[si >> 1] & 15];
					}
					else {
						//^01B0:0B91
						bp08->b6[di] = localPal[buffSrc[si >> 1] >> 4];
					}
					//^01B0:0BCF
				}
				//^01B0:0BDF
			}
			//^01B0:0BEF
			bp08->b4 = localPal[U8(colorkey)];
			//^01B0:0C0D
		}
		else {
			//^01B0:0C0F
			i16 si = 0;
			//^01B0:0C11
			for (; srcWidth * srcHeight > si; si++) {
				//^01B0:0C13
				bp08->b6[si] = buffSrc[si];
				//^01B0:0C33
			}
			//^01B0:0C3E
			bp08->b4 = U8(colorkey);
		}
		//^01B0:0C44
		//^01B0:0C48
		bp08->b0 = zz;
		bp08->b1 = ss;
		bp08->b2 = U8(srcWidth);
		bp08->b3 = U8(srcHeight);
		//^01B0:0C66
		UNLOCK_MOUSE_EVENT();
	}
	//^01B0:0C6A
	//^01B0:0C6B
	return;
}

//^01B0:0C70
void SkWinCore::_01b0_0c70(Bit16u xx) //#DS=04BF
{
	//^01B0:0C70
	//^01B0:0C73
	LOADDS(0x3083);
	//^01B0:0C79
	LOCK_MOUSE_EVENT();
	//^01B0:0C7F
	if (_04bf_17a8 != 0) {
		//^01B0:0C86
		_01b0_073d();
		//^01B0:0C8A
		_04bf_1938 = xx;
		//^01B0:0C90
		_01b0_05ae();
	}
	else {
		//^01B0:0C96
		_04bf_1938 = xx;
	}
	//^01B0:0C9C
	UNLOCK_MOUSE_EVENT();
	//^01B0:0CA1
	return;
}

//^01B0:0CA4
void SkWinCore::_01b0_0ca4(Bit16u *xx, Bit16u yy)
{
	//^01B0:0CA4
	//^01B0:0CA8
	LOADDS(0x3083);
	//^01B0:0CAE
	//^01B0:0CB0
	LOCK_MOUSE_EVENT();
	//^01B0:0CBA
	for (Bit16u si=0; si < 4; si++) {
		//^01B0:0CBE
		((Bit16u *)&_04bf_1852)[si] = xx[si];
		//^01B0:0CD2
	}
	//^01B0:0CD8
	_04bf_17e8 = yy;
	_04bf_1850 = 1;
	//^01B0:0CE4
	UNLOCK_MOUSE_EVENT();
	//^01B0:0CE9
	return;
}

//^443C:040E
void SkWinCore::_443c_040e()
{
	//^443C:040E
	//^443C:0411
	FIRE_HIDE_MOUSE_CURSOR();
	//^443C:0416
	_4976_5dae.rc4.cx = 1;
	//^443C:041C
	_01b0_0ca4(_4976_4954, 32);
	//^443C:042D
	FIRE_SHOW_MOUSE_CURSOR();
	//^443C:0432
	return;
}

//^3E74:57B5
Bit8u *SkWinCore::ALLOC_NEW_PICT(Bit16u index, Bit16u width, Bit16u height, Bit16u bpp)
{
	//^3E74:57B5
	//^3E74:57BA
	Bit16u si = width;
	//^3E74:57BD
	Bit8u *bp04 = ALLOC_CPXHEAP_MEM(
		index,
		Bit32u((bpp == 4) ? (((si +1) & 0xfffe) >> 1) : (si & 0xffff)) * height
		);
	//^3E74:57E8
	WRITE_UI16(bp04,-4,si);
	//^3E74:57EF
	WRITE_UI16(bp04,-2,height);
	//^3E74:57F9
	WRITE_UI16(bp04,-6,bpp);
	//^3E74:580E

	SkD((DLV_DBG_GETPIC,"DBG: ALLOC_NEW_PICT(%3u,%3u,%3u,%u) = %p\n", (Bitu)index, (Bitu)width, (Bitu)height, (Bitu)bpp, bp04));
	return bp04;
}

//^3E74:53EA
Bit32u SkWinCore::GET_TEMP_CACHE_HASH()
{
	//^3E74:53EA
	Bit32u bp04;
	Bit16u bp06;
	do {
		//^3E74:53EE
		bp04 = 0xffff0000 | (_4976_484b++);
		//^3E74:5401
	} while (FIND_ICI_FROM_CACHE_HASH(bp04, &bp06) != 0);
	//^3E74:5418
	return bp04;
}

//^3E74:5888
Bit16u SkWinCore::ALLOC_TEMP_CACHE_INDEX()
{
	//^3E74:5888
	//^3E74:588C
	Bit16u bp02;
	ADD_CACHE_HASH(GET_TEMP_CACHE_HASH(), &bp02);
	//^3E74:589E
	return bp02;
}

//^0B36:0D67
void SkWinCore::_0b36_0d67(sk3f6c *ref, SRECT *rc)
{
	//^0B36:0D67
	//^0B36:0D6C
	if (rc == NULL)
		//^0B36:0D74
		return;
	//^0B36:0D77
	i16 si = 0;
	for (; ref->w10 > si; si++) {
		//^0B36:0D7C
		if (true
			&& ref->w12[si].x <= rc->x
			&& ref->w12[si].x + ref->w12[si].cx -1 >= rc->x + rc->cx -1
			&& ref->w12[si].y <= rc->y
			&& ref->w12[si].y + ref->w12[si].cy -1 >= rc->y + rc->cy -1
		) {
			//^0B36:0E09
			//^0B36:0FA7
			return;
		}
		//^0B36:0E0C
		if (ref->w12[si].x >= rc->x) {
			//^0B36:0E22
			if (ref->w12[si].x + ref->w12[si].cx -1 <= rc->x + rc->cx -1) {
				//^0B36:0E55
				if (ref->w12[si].y >= rc->y) {
					//^0B36:0E6C
					if (ref->w12[si].y + ref->w12[si].cy -1 <= rc->y + rc->cy -1) {
						//^0B36:0E97
						goto _0ec7;
					}
				}
			}
		}
		//^0B36:0E99
	}
	//^0B36:0EA6
	if (si >= 5) {
		//^0B36:0EAB
		_0b36_0cbe(ref, 0);
	}
	//^0B36:0EBA
	si = ref->w10++;
	//^0B36:0EC7
_0ec7:
	COPY_MEMORY(rc, &ref->w12[si], 8);
	//^0B36:0EEA
	rc = &ref->w12[si];
	//^0B36:0F00
	si = rc->x - ref->rc2.x;
	//^0B36:0F11
	if (si < 0) {
		//^0B36:0F15
		if ((rc->cx += si) <= 0) {
			//^0B36:0F1E
			ref->w10--;
			//^0B36:0F25
			return;
		}
		//^0B36:0F28
		rc->x -= si;
	}
	//^0B36:0F2E
	si = rc->y - ref->rc2.y;
	//^0B36:0F3E
	if (si < 0) {
		//^0B36:0F42
		if ((rc->cy += si) <= 0) {
			//^0B36:0F4B
			//^0B36:0F1E
			ref->w10--;
			//^0B36:0F25
			return;
		}
		//^0B36:0F4D
		rc->y -= si;
	}
	//^0B36:0F54
	si = ref->rc2.x + ref->rc2.cx -1 -(rc->x + rc->cx -1);
	//^0B36:0F76
	if (si < 0) {
		//^0B36:0F7C
		rc->cx += si;
	}
	//^0B36:0F83
	si = ref->rc2.y + ref->rc2.cy -1 -(rc->y + rc->cy -1);
	//^0B36:0F9F
	if (si < 0) {
		//^0B36:0FA3
		rc->cy += si;
	}
	//^0B36:0FA7
	return;
}

//^0B36:0C52
sk3f6c *SkWinCore::_0b36_0c52(sk3f6c *ref, Bit16u rectno, Bit16u ww)
{
	//^0B36:0C52
	//^0B36:0C55
	if (rectno != 0xffff) {
		//^0B36:0C5B
		QUERY_EXPANDED_RECT(rectno, &ref->rc2);
	}
	//^0B36:0C6F
	ALLOC_NEW_PICT(ref->w0 = ALLOC_TEMP_CACHE_INDEX(), ref->rc2.cx, ref->rc2.cy, 8);
	//^0B36:0C90
	ref->w10 = 0;
	//^0B36:0C99
	if (ww != 0) {
		//^0B36:0C9F
		_0b36_0d67(ref, &ref->rc2);
	}
	//^0B36:0CB6
	return ref;
}

//^0B36:0FAB
SRECT *SkWinCore::OFFSET_RECT(const sk3f6c *refOrg, const SRECT *prcSrc, SRECT *prcOut)
{
	//^0B36:0FAB
	//^0B36:0FAF
	prcOut->x = prcSrc->x - refOrg->rc2.x;
	//^0B36:0FC4
	prcOut->y = prcSrc->y - refOrg->rc2.y;
	//^0B36:0FD9
	prcOut->cx = prcSrc->cx;
	//^0B36:0FE7
	prcOut->cy = prcSrc->cy;
	//^0B36:0FF5
	return prcOut;
}

//^0B36:0FFE
void SkWinCore::FILL_RECT_SUMMARY(sk3f6c *ref, SRECT *rc, Bit8u fill)
{
	//^0B36:0FFE
	ENTER(8);
	//^0B36:1002
	if (rc != NULL) {
		//^0B36:100A
		SRECT bp08;
		FIRE_FILL_RECT_ANY(
			reinterpret_cast<Bit8u *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(ref->w0)),
			OFFSET_RECT(ref, rc, &bp08),
			fill,
			ref->rc2.cx,
			8
			);
		//^0B36:1046
		_0b36_0d67(ref, rc);
	}
	//^0B36:1059
	return;
}

//^29EE:00A3
void SkWinCore::_29ee_00a3(Bit16u xx)
{
	//^29EE:00A3
	//^29EE:00A6
	if (_4976_3f6c.w0 == 0xffff) {
		//^29EE:00AD
		_0b36_0c52(&_4976_3f6c, 11, xx);
		//^29EE:00BE
		if (xx != 0) {
			//^29EE:00C4
			FILL_RECT_SUMMARY(&_4976_3f6c, &_4976_3f6c.rc2, glbPaletteT16[COLOR_BLACK]);
		}
	}
	//^29EE:00DE
	return;
}

//^2759:0E30
U16 SkWinCore::IS_MISSILE_VALID_TO_LAUNCHER(Bit16u player, i16 hand, ObjectID rlWhatYouLaunch)
{
	// return 0 if you cannot launch the missile with launcher. (not applicable projectile)
	// return 1 if you can launch the missile with launcher.

	//^2759:0E30
	ENTER(2);
	//^2759:0E36
	ObjectID bp02;
	U16 di;
	U16 si;
	return (true
		&& hand != -1
		&& (bp02 = glbChampionSquad[player].Possess(hand)) != OBJECT_NULL // any objects in hand?
		&& ((di = QUERY_GDAT_DBSPEC_WORD_VALUE(bp02, 0x05)) & 0x8000) != 0 // is it launcher?
		&& ((si = QUERY_GDAT_DBSPEC_WORD_VALUE(rlWhatYouLaunch, 0x05)) & 0x8000) == 0 // is it projectile?
		&& ((si & di) & 0x7fff) != 0 // is valid combination? between launcher and projectile
		)
		? 1 : 0;
}

//^0CD5:00B2
i16 SkWinCore::BETWEEN_VALUE(i16 minv, i16 newv, i16 maxv)
{
	//^0CD5:00B2
	i16 si = newv;
	if (si >= minv) {
		//^0CD5:00C3
		if (si <= maxv) {
			//^0CD5:00CD
			return si;
		}
		//^0CD5:00C8
		return maxv;
	}
	//^0CD5:00BE
	return minv;
}





//^3E74:506B
Bit8u *SkWinCore::QUERY_GDAT_ENTRY_DATA_PTR(Bit8u cls1, Bit8u cls2, Bit8u cls3, Bit8u cls4)
{
LOGX(("%40s: C%02d=I%02X=E%02X=T%03d", "QUERY_GDAT_ENTRY_DATA_PTR for ", cls1, cls2, cls4, cls3 ));
	//^3E74:506B
	ENTER(6);
	//^3E74:5070
	Bit16u si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, cls3, cls4);
	//^3E74:5089
	if (cls3 == dtImage && IS_CLS1_CRITICAL_FOR_LOAD(cls1) == 0) {
		//^3E74:509C
		if (si == 0xffff || glbShelfMemoryTable[si].Absent()) {
			//^3E74:50BE
			si = QUERY_GDAT_ENTRY_DATA_INDEX(0x15, 0xfe, dtImage, 0xfe);
		}
	}
#if UseAltic
	if (si == 0xFFFFU) {
		return NULL;
	}
#endif
	//^3E74:50D1
	Bit8u *bp04;
	Bit16u bp06;
	if (glbShelfMemoryTable[si].Absent() && IS_CLS1_CRITICAL_FOR_LOAD(cls1) != 0) {
		//^3E74:50FB
		bp04 = QUERY_GDAT_DYN_BUFF(si, &bp06, 0);
	}
	else {
		//^3E74:510C
		bp04 = REALIZE_GRAPHICS_DATA_MEMORY(glbShelfMemoryTable[si]);
	}
	//^3E74:5124
	return bp04;
}

//^3E74:0A77
void SkWinCore::FREE_PICT6(SkImage *ref)
{
	//^3E74:0A77
	ENTER(0);
	//^3E74:0A7A
	if (_4976_5d76 == 0) {
		//^3E74:0A81
		if (READ_UI16(ref,-6 +4) == afDefault) {
			//^3E74:0A9D
			DEALLOC_UPPER_MEMORY(READ_UI32(ref,-6 +0));
		}
		else {
			//^3E74:0ABF
			DEALLOC_LOWER_MEMORY(READ_UI32(ref,-6 +0));
		}
	}
	//^3E74:0AE1
	return;
}

//^3E74:52F4
i16 SkWinCore::QUERY_GDAT_PICT_OFFSET(Bit8u cls1, Bit8u cls2, Bit8u cls4)
{
	//^3E74:52F4
	ENTER(8);
	//^3E74:52FA
	Bit16u di = 0;
	//^3E74:52FC
	RawEntry *bp04 = QUERY_GDAT_ENTRYPTR(cls1, cls2, dtImage, cls4);
	//^3E74:5317
	if (bp04 == NULL) {
		//^3E74:5325
		return 0;
	}
	else {
		//^3E74:532A
		Bit16u si = bp04->data;
		//^3E74:5331
		SkImage *bp04;
		if (glbShelfMemoryTable[si].Absent()) {
			//^3E74:534E
			di = 1;
			//^3E74:5351
			bp04 = reinterpret_cast<SkImage *>(QUERY_GDAT_ENTRY_DATA_PTR(cls1, cls2, dtImage, cls4));
		}
		else {
			//^3E74:5368
			bp04 = reinterpret_cast<SkImage *>(REALIZE_GRAPHICS_DATA_MEMORY(glbShelfMemoryTable[si]));
		}
		//^3E74:5386
		i16 bp06 = bp04->Xoffset();
		//^3E74:5392
		i16 bp08;
		if (bp06 == -32) {
			//^3E74:5397
			si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, dtImageOffset, cls4);
		}
		else {
			//^3E74:53AE
			bp08 = bp04->Yoffset();
#if DM2_EXTENDED_MODE == 1
			if (bp08 == -32 || bp08 == 31) {
				bp08 = 0;
			}
#else
			//^3E74:53BB
			if (bp08 == -32) {
				//^3E74:53C0
				bp08 = 0;
			}
#endif
			//^3E74:53C5
			si = (bp06 << 8) | Bit8u(bp08);
		}
		//^3E74:53D2
		if (di != 0) {
			//^3E74:53D8
			FREE_PICT6(bp04);
		}
		//^3E74:53E4
		return si;
	}
}

//^0B36:0520
//ExtendedPicture *SkWinCore::QUERY_GDAT_SUMMARY_IMAGE(ExtendedPicture *ref, Bit8u cls1, Bit8u cls2, Bit8u cls4)
ExtendedPicture *SkWinCore::QUERY_GDAT_SUMMARY_IMAGE(ExtendedPicture *ref, Bit8u iCategory, Bit8u iItemNo, Bit8u iEntry)
{
	SkD((DLV_DBG_GETPIC, "DBG: QUERY_GDAT_SUMMARY_IMAGE(%p,%02X,%02X,%02X)\n"
		, ref, (Bitu)iCategory, (Bitu)iItemNo, (Bitu)iEntry));

	//^0B36:0520
	ENTER(8);
    //^0B36:0524
	ZERO_MEMORY(ref, 314);
	//^0B36:0537
	ref->w12 = -1;
	ref->b8 = iCategory;
	ref->b9 = iItemNo;
	ref->b10 = dtImage;
	ref->b11 = iEntry;
	ref->w54 = 64;
	ref->w52 = 64;
	ref->rectNo = 0xffff;
	ref->w26 = 0xffff;
	ref->pb44 = _4976_4964;
	ref->colorKeyPassThrough = -1;
	//^0B36:0586
	if (iCategory != 0xff) {
		//^0B36:058F
		ref->w6 = QUERY_GDAT_ENTRY_DATA_INDEX(iCategory, iItemNo, dtImage, iEntry);
		//^0B36:05AC
		i16 bp02 = QUERY_GDAT_ENTRY_DATA_INDEX(iCategory, iItemNo, dtImageOffset, 0xFE);
		//^0B36:05C4
		if (bp02 != 0) {
			//^0B36:05C8
			ref->w28 += i8(bp02 >> 8);
			//^0B36:05D3
			ref->w30 += i8(bp02);
		}
		//^0B36:05DB
		bp02 = QUERY_GDAT_PICT_OFFSET(iCategory, iItemNo, iEntry);
		//^0B36:05F2
		if (bp02 != 0) {
			//^0B36:05F6
			ref->w28 += i8(bp02 >> 8);
			//^0B36:0601
			ref->w30 += i8(bp02);
		}
		//^0B36:0609
		Bit8u *bp06 = QUERY_GDAT_IMAGE_LOCALPAL(iCategory, iItemNo, iEntry);
		//^0B36:0623
		if (bp06 == NULL) {
			//^0B36:062B
			ref->w56 = 256;
			//^0B36:0634
			bp06 = ref->b58;
			//^0B36:0643
			i16 bp08 = 0;
			//^0B36:0648
			for (; bp08 < 256; bp08++) {
				//^0B36:064A
				bp06[bp08] = (Bit8u)bp08;
				//^0B36:0656
			}
		}
		else {
			//^0B36:0662
			ref->w56 = 16;
			//^0B36:066B
			COPY_MEMORY(bp06, ref->b58, 16);
		}
	}
	//^0B36:0687
	return ref;
}

//^0B36:0A3F
void SkWinCore::DRAW_DEF_PICT(ExtendedPicture *ref)
{
	// draw almost all pictures:
	// a) dungeon viewport: wallset/scene graphics, ornate graphics, spell graphics,
	//		flying items (passing/coming/crossing/placing)
	// b) inventory screen: wall paper, zzz button, X button, "water" text, "food" text, 
	//		resurrection lever, disk button, and so on
	// c) inventory icons: eye, mouth, item icons

	//^0B36:0A3F
	ENTER(12);
	//^0B36:0A45
	if (ref->width <= 0 || ref->height <= 0)
		//^0B36:0A59
		return;
	//^0B36:0A5C
	Bit8u *bp04 = QUERY_PICT_BITS(ref);
	//^0B36:0A6E
	Bit16u bp06 = ref->rectNo;
	//^0B36:0A78
	i16 bp08;
	i16 bp0a;
	if (bp06 == 0xffff) {
		//^0B36:0A7D
		bp08 = ref->w32;
		bp0a = ref->w34;
	}
	else {
		//^0B36:0A8D
		if ((bp06 & 0x8000) == 0 || ref->w28 != 0 || ref->w30 != 0) {
			//^0B36:0AA5
			bp06 |= 0x8000;
			//^0B36:0AAE
			bp08 = ref->w32 + ref->w28;
			bp0a = ref->w34 + ref->w30;
		}
		else {
			//^0B36:0AC6
			bp08 = ref->width;
			bp0a = ref->height;
		}
		//^0B36:0AD4
		if (QUERY_BLIT_RECT(bp04, &ref->rc36, bp06, &bp08, &bp0a, ref->w26) == NULL)
			//^0B36:0B07
			return;
	}
	//^0B36:0B0A
	bp08 += ref->w14;
	bp0a += ref->w16;
	i16 si = READ_UI16(bp04,-4);
	i16 di = ref->rc36.cx +bp08;
	//^0B36:0B2E
	if (si > di && (ref->mirrorFlip & 0x0001) != 0) {
		//^0B36:0B3A
		si -= di;
	}
	else {
		//^0B36:0B3E
		si = 0;
	}
	//^0B36:0B40
	if ((ref->mirrorFlip & 0x0001) != 0) {
		//^0B36:0B4B
		bp08 = 0;
	}
	//^0B36:0B50
	bp08 += si;
	//^0B36:0B53
	si = READ_I16(bp04,-2);
	//^0B36:0B5A
	di = ref->rc36.cy +bp0a;
	//^0B36:0B66
	if (si > di && (ref->mirrorFlip & 0x0002) != 0) {
		//^0B36:0B72
		si -= di;
	}
	else {
		//^0B36:0B76
		si = 0;
	}
	//^0B36:0B78
	if ((ref->mirrorFlip & 0x0002) != 0) {
		//^0B36:0B83
		bp0a = 0;
	}
	//^0B36:0B88
	bp0a += si;
	//^0B36:0B8B
	Bit16u bp0c;
	if (ref->pb44 == _4976_4964) {
		//^0B36:0BA2
		si = glbScreenWidth;
		di = glbScreenHeight;
		bp0c = 8;
	}
	else {
		//^0B36:0BB1
		si = READ_I16(ref->pb44,-4);
		//^0B36:0BBC
		di = READ_I16(ref->pb44,-2);
		//^0B36:0BC7
		bp0c = READ_I16(ref->pb44,-6);
	}
	//^0B36:0BD5
	if (ref->colorKeyPassThrough != -2) {
		//^0B36:0BDF
		FIRE_BLIT_PICTURE(
			bp04,
			ref->pb44,
			&ref->rc36,
			bp08,
			bp0a,
			READ_UI16(bp04,-4),
			si,
			ref->colorKeyPassThrough,
			ref->mirrorFlip,
			READ_UI16(bp04,-6),
			bp0c,
			(ref->w56 == 0) ? NULL : ref->b58
			);
	}
	//^0B36:0C3D
	ref->w32 = bp08;
	ref->w34 = bp0a;
	//^0B36:0C4E
	return;
}

//^0B36:0139
Bit8u *SkWinCore::QUERY_PICT_BITS(Picture *ref)
{
	//^0B36:0139
	ENTER(0);
	//^0B36:013D
	Bit16u si = ref->w4;
	//^0B36:0144
	if ((si & 0x0004) != 0) {
		//^0B36:014A
		return ref->pb0 = QUERY_GDAT_IMAGE_ENTRY_BUFF(ref->b8, ref->b9, ref->b11);
	}
	//^0B36:016D
	else if ((si & 0x0008) != 0) {
		//^0B36:0173
		return ref->pb0 = QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(ref->w12);
	}
	else {
		//^0B36:0182
		return ref->pb0;
	}
}

//^0B36:11C0
// TODO: image related ?
void SkWinCore::_0b36_11c0(ExtendedPicture *xx, sk3f6c *yy, Bit16u ss, i16 colorkey2)
{
	//^0B36:11C0
    ENTER(12);
	//^0B36:11C4
	xx->pb44 = reinterpret_cast<Bit8u *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(yy->w0));
	//^0B36:11DB
	SRECT bp08;
	i16 bp0a;
	i16 bp0c;
	if (ss == 0xffff) {
		ATLASSERT(false); // FTL's miss logic?

		//^0B36:11E1
		COPY_MEMORY(&xx->rc36, &bp08, 8);
		//^0B36:11FC
		goto _123d;
	}
	else {
		//^0B36:11FE
		bp0a = xx->width;
		bp0c = xx->height;
		//^0B36:120F
		if (QUERY_BLIT_RECT(QUERY_PICT_BITS(xx), &bp08, ss, &bp0a, &bp0c, -1) != NULL) {
			//^0B36:123D
_123d:
			xx->width += bp0a;
			xx->height += bp0c;
			//^0B36:124E
			OFFSET_RECT(yy, &bp08, &xx->rc36);
			//^0B36:126A
			xx->rectNo = 0xffff;
			xx->colorKeyPassThrough = colorkey2;
			//^0B36:127A
			DRAW_DEF_PICT(xx);
			//^0B36:1286
			_0b36_0d67(yy, &bp08);
		}
	}
	//^0B36:1298
	return;
}

//^0B36:0036
i16 SkWinCore::CALC_STRETCHED_SIZE(i16 val, i16 fact64)
{
	//^0B36:0036
	ENTER(0);
	//^0B36:0039
	return (i16(val * fact64) + (fact64 >> 1)) >> 6;
}

//^0B36:00C3
// TODO: Image related ?
Bit8u *SkWinCore::_0b36_00c3(i16 xx, Picture *ref)
{
	//^0B36:00C3
	ENTER(4);
	//^0B36:00C7
	Bit8u *bp04 = reinterpret_cast<Bit8u *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(xx));
	//^0B36:00D6
	if (ref != NULL) {
		//^0B36:00DE
		ref->pb0 = bp04;
		//^0B36:00EE
		ref->w14 = 0;
		ref->w16 = 0;
		//^0B36:00FA
		ref->width = READ_UI16(bp04,-4);
		//^0B36:0108
		ref->height = READ_UI16(bp04,-2);
		//^0B36:0116
		ref->w22 = READ_UI16(bp04,-6);
		//^0B36:0124
        ref->w12 = xx;
		//^0B36:012B
		ref->w4 = 8;
	}
	//^0B36:0131
	return bp04;
}

//^0B36:004B
Bit8u *SkWinCore::QUERY_PICST_IMAGE(Picture *ref)
{
	//^0B36:004B
	ENTER(4);
	//^0B36:004F
	Bit8u *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(ref->b8, ref->b9, ref->b11);
	if (SkCodeParam::bUseFixedMode && bp04 == NULL)
		bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_MISCELLANEOUS, GDAT_ITEM_DEFAULT_INDEX, GDAT_ITEM_DEFAULT_INDEX); // Get Yukman!
	if (SkCodeParam::bUseFixedMode && bp04 == NULL)
		return NULL;
	//^0B36:006F
	ref->pb0 = bp04;
	//^0B36:007F
	ref->w14 = 0;
	ref->w16 = 0;
	//^0B36:008B
	ref->width = READ_UI16(bp04,-4);
	//^0B36:0099
	ref->height = READ_UI16(bp04,-2);
	//^0B36:00A7
	ref->w22 = READ_UI16(bp04,-6);
	//^0B36:00B5
	ref->w4 = 4;
	//^0B36:00BB
	return bp04;
}

//^3E74:58BF
void SkWinCore::ALLOC_IMAGE_MEMENT(Bit8u cls1, Bit8u cls2, Bit8u cls4)
{
	//^3E74:58BF
	ENTER(4);
	//^3E74:58C5
	Bit16u si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, dtImage, cls4);
	//^3E74:58DC
	if (si != 0xffff && glbShelfMemoryTable[si].Absent()) {
		//^3E74:58FC
		si = QUERY_GDAT_ENTRY_DATA_INDEX(0x15,0xfe,dtImage,0xfe);			
	}
	//^3E74:590F
	if (si != 0xffff) {
		//^3E74:5914
		Bit16u di = QUERY_MEMENTI_FROM(si);
		//^3E74:591C
		if (di == 0xffff) {
			if (glbShelfMemoryTable[si].Absent()) {
				//^3E74:593C
				return;
			}
			//^3E74:593E
			SkImage *bp04 = reinterpret_cast<SkImage *>(REALIZE_GRAPHICS_DATA_MEMORY(glbShelfMemoryTable[si]));
			//^3E74:595C
			if (bp04->Yoffset() != -32) {
				//^3E74:5969
				return;
			}
			//^3E74:596B
			if (bp04->BitsPixel() != 8) {
				//^3E74:5980
				return;
			}
			//^3E74:5982
			_4976_4807 = si;
			//^3E74:5986
			return;
		}
		else {
			//^3E74:5988
			_3e74_4549(di);
		}
	}
	//^3E74:598E
	return;
}

//^0B36:018F
void SkWinCore::ALLOC_PICT_MEMENT(Picture *ref)
{
	//^0B36:018F
	ENTER(0);
	//^0B36:0192
	if ((ref->w4 & 0x0004) != 0) {
		//^0B36:019D
		ALLOC_IMAGE_MEMENT(ref->b8, ref->b9, ref->b11);
	}
	//^0B36:01B6
	else if ((ref->w4 & 0x0008) != 0) {
		//^0B36:01C1
		_3e74_583a(ref->w12);
	}

}

//^0B36:068F
Bit32u SkWinCore::CALC_PICT_ENT_HASH(ExtendedPicture *ref)
{
	//^0B36:068F
	ENTER(0);
	//^0B36:0692
	return (Bit32u(ref->w6 & 0x1fff) << 12) | ((ref->w52 & 0x007f) << 5) | (ref->w54 & 0x001f);
}

//^3E74:5992
void SkWinCore::FREE_IMAGE_MEMENT(Bit8u cls1, Bit8u cls2, Bit8u cls4)
{
	//^3E74:5992
	ENTER(0);
	//^3E74:5997
	Bit16u si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, dtImage, cls4);
	//^3E74:59AE
	if (si == 0xffff || glbShelfMemoryTable[si].Absent()) {
		//^3E74:59CE
		si = QUERY_GDAT_ENTRY_DATA_INDEX(0x15,0xfe,dtImage,0xfe);
	}
	//^3E74:59E1
	if (si == 0xffff)
		//^3E74:59E4
		return;
	//^3E74:59E6
	if (_4976_4807 == si) {
		//^3E74:59EC
		_4976_4807 = 0xffff;
	}
	//^3E74:59F2
	Bit16u di = QUERY_MEMENTI_FROM(si);
	//^3E74:59FA
	if (di != 0xffff) {
		//^3E74:59FF
		RECYCLE_MEMENTI(di, 0);
	}
	//^3E74:5A08
	return;
}

//^0B36:01CD
void SkWinCore::FREE_PICT_MEMENT(Picture *ref)
{
	//^0B36:01CD
	ENTER(0);
	//^0B36:01D0
	if ((ref->w4 & 0x0004) != 0) {
		//^0B36:01DB
		FREE_IMAGE_MEMENT(ref->b8, ref->b9, ref->b11);
	}
	//^0B36:01F4
	else if ((ref->w4 & 0x0008) != 0) {
		//^0B36:01FF
		_3e74_585a(ref->w12, 0);
	}

}

//^44C8:2307
// TODO: related gfx ?
void SkWinCore::_44c8_2307(X16 xx, X16 yy, X16 zz, X16 ww)
{
	//^44C8:2307
	ENTER(4);
	//^44C8:230D
	U8 *bp04 = _4976_5e64 + xx;
	U16 di = zz >> 1;
	U16 cx = ww + yy;
	U16 si = yy;
	do {
		//^44C8:2331
		_4976_5e6a[si] = bp04[di >> 7];
		di += zz;
		si++;
	} while (si < cx);
	//^44C8:234D
	return;
}

//^44C8:2351
void SkWinCore::_44c8_2351(Bit8u *xx, Bit8u *yy, Bit16u ss, Bit16u tt, Bit16u uu, Bit16u vv)
{
	//^44C8:2351
	ENTER(10);
	//^44C8:2357
	U16 di = uu;
	_4976_5e64 = xx;
	_4976_5e6a = yy;
	U16 bp04 = (i32(ss) << 7) / di;
	U16 bp06 = (i32(tt) << 7) / vv;
	U16 bp02 = bp06 >> 1;
	U16 si = 0;
	X16 bp08 = 0; // defaulting to 0
	X16 bp0a = 0; // defaulting to 0
	for (; si < vv; bp0a = bp08, bp02 += bp06, si++) {
		//^44C8:23B2
		bp08 = bp02 >> 7;
		if (bp0a == bp08 && si > 0) {
			//^44C8:23C7
			_4976_5e64 = yy;
			_44c8_0b8d((si -1) * di, si * di, di);
			_4976_5e64 = xx;
		}
		else {
			//^44C8:23F6
			_44c8_2307(bp08 * ss, si * di, bp04, di);
		}
		//^44C8:240D
	}
	//^44C8:241F
	return;
}

//^44C8:000F
void SkWinCore::FIRE_BLIT_TO_MEMORY_ROW_4TO4BPP(Bit16u offSrc, Bit16u offDst, Bit16u width)
{
	//^44C8:000F
	ENTER(0);
	//^44C8:0015
	Bit8u *di = _4976_5e6a;
	Bit8u *si = _4976_5e64;
	Bit16u cx = width;

	bool carry1 = (offSrc & 1) ? true : false;
	offSrc >>= 1;
	bool carry2;
	if (!carry1) {
		//^44C8:0028
		si += offSrc;
		//^44C8:002D
		carry2 = (offDst & 1) ? true : false;
		offDst >>= 1;
		if (!carry2) {
			//^44C8:0034
			di += offDst;

			//^44C8:0036
_0036:
			bool carry3 = (cx & 1) ? true : false;
			cx >>= 1;
			{
				//^44C8:0039
				bool carry4 = (cx & 1) ? true : false;
				cx >>= 1;
				//^44C8:003B
				if (carry4) {
					//^44C8:003D
					*di = *si; di++; si++;
				}
				//^44C8:003E
				while (cx != 0) {
					//^44C8:0043
					*di = *si; di++; si++;
					*di = *si; di++; si++;
					cx--;
				}
			}
			//^44C8:0049
			if (carry3) {
				*di = (*si & 0xf0) | (*di & 0x0f); si++; di++;
			}
		}
		else {
			//^44C8:00B5
			di += offSrc;
			//^44C8:00B7
			Bit8u bh = *di;
			//^44C8:00BA
			cx++;
			bool carry = (cx & 1) ? true : false;
			cx >>= 1;
			//^44C8:00BE
			if (cx != 0) {
				//^44C8:00C0
				bool carry = (cx & 1) ? true : false;
				//^44C8:00C2
				if (carry) {
					//^44C8:00C4
					Bit8u al = *si; si++;
					//^44C8:00C6
					*di = (bh & 0xf0) | (al >> 4); di++;
					bh = al << 4;
				}
				//^44C8:00D6
				if (cx != 0) {
					//^44C8:00D8
					bh &= 0xf0;
					do {
						// AL   AH   BL   BH
						// m34  m56       m12
						//                m10
						// m63  m45
						//           m63
						// m10            m63
						//           m60  m03
						// m13
						//                m60
						// ------------------
						// m13  m45       m60

						//^44C8:00DB
						Bit8u al = *si; si++;
						Bit8u ah = *si; si++;
						*di =  bh       | (al >> 4); di++;
						*di = (al << 4) | (ah >> 4); di++;
						bh = ah << 4;
						//^44C8:00EC
					} while (--cx != 0);
				}
			}
			//^44C8:00EE
			if (carry) {
				//^44C8:00F1
				*di = (*di & 0x0f) | bh; di++;
			}
		}
	}
	else {
		//^44C8:0068
        si += offSrc;
		{
			//^44C8:006A
			bool carry = (offDst & 1) ? true : false;
			offDst >>= 1;
			//^44C8:006F
			if (carry) {
				//^44C8:0059
				di += offDst;
				//^44C8:005B
				*di = (*si & 0x0f) | (*di & 0xf0); si++; di++;
				//^44C8:0065
				cx--;
				//^44C8:0066
				goto _0036;
			}
		}
		//^44C8:0071
		di += offDst;
		//^44C8:0073
		Bit8u bh = *si; si++;
		{
			//^44C8:0076
			bool carry = (cx & 1) ? true : false;
			cx >>= 1;
			//^44C8:0079
			if (cx != 0) {
				//^44C8:007B
				bool carry = (cx & 1) ? true : false;
				cx >>= 1;
				//^44C8:007D
				if (carry) {
					//^44C8:007F
					Bit8u bl = *si;
					*di = (*si >> 4) | (bh << 4); di++; si++;
					bh = bl;
				}
				//^44C8:008C
				if (cx != 0) {
					//^44C8:008E
					bh &= 0x0f;
					do {
						// AL   AH   BL   BH
						// m23  m45  ?    m01
						//           m45
						//      m01       m45
						//           m05  m40
						//      m41
						//                m05
						// m12  m34
						// ------------------
						// m12  m34       m05

						//^44C8:0091
						Bit8u al = *si; si++;
						Bit8u ah = *si; si++;
						*di = (bh << 4) | (al >> 4); di++;
						*di = (al << 4) | (ah >> 4); di++;
						bh = ah & 0x0f;
						//^44C8:00A2
					} while (--cx != 0);
				}
			}
			//^44C8:00A4
			if (carry) {
				//^44C8:00A7
				*di = (*di & 0x0f) | (bh << 4);
			}
		}
	}
	//^44C8:00F9
	return;
}

//^44C8:20E5
void SkWinCore::_44c8_20e5(Bit16u srcOff, Bit16u dstOff, Bit16u srcWidth, Bit16u dstWidth)
{
	ATLASSERT(srcWidth != 0);

	//^44C8:20E5
	ENTER(0);
	//^44C8:20EA
    Bit8u *di = _4976_5e6a + (dstOff >> 1);
	Bit16u cx = srcWidth >> 1;
	Bit16u dx = (dstWidth +1) >> 1;
	Bit8u *si = _4976_5e64 + (srcOff >> 1);
	Bit8u bh = 0;
	do {
		//^44C8:2111
		Bit8u ah = si[cx >> 8];
		if ((cx & 0x80) == 0) {
			ah &= 0xf0;
		}
		else {
			ah <<= 4;
		}
		cx += srcWidth;

		Bit8u al = si[cx >> 8];
		if ((cx & 0x80) == 0) {
			al >>= 4;
		}
		else {
			al &= 0x0f;
		}
		*di = al | ah; di++;
		cx += srcWidth;
	} while (--dx != 0);
	//^44C8:213E
	return;
}

//^44C8:2143
void SkWinCore::_44c8_2143(Bit16u xx, Bit16u yy, Bit16u ss, Bit16u tt)
{
	//^44C8:2143
	ENTER(4);
	//^44C8:2149
	U16 si = ss >> 1;
	U16 bp04 = ((tt +yy) +1) >> 1;
	U16 di = yy >> 1;
	//^44C8:2163
	while (di < bp04) {
		//^44C8:2166
		U16 bp02 = (si >> 7) +xx;
		U8 al;
		U8 dl = al = _4976_5e64[bp02 >> 1];
		si += ss;
		if ((bp02 & 1) != 0) {
			//^44C8:2188
			al &= 0xF;
		}
		else {
			//^44C8:218F
			al >>= 4;
		}
		//^44C8:2196
		al = _4976_5dbe[al];
		al <<= 4;
		U8 cl = al;
		bp02 = (si >> 7) +xx;
		al = _4976_5e64[bp02 >> 1];
		dl = al;
		si += ss;
		if ((bp02 & 1) != 0) {
			//^44C8:21C6
			al &= 0xF;
		}
		else {
			//^44C8:21CD
			al >>= 4;
		}
		//^44C8:21D4
		al = cl | _4976_5dbe[al];
		_4976_5e6a[di] = al;
		di++;
		//^44C8:21E7
	}
	//^44C8:21EF
	return;
}

#pragma warning(push)
#pragma warning(disable: 4700)

//^44C8:21F3
void SkWinCore::FIRE_STRETCH_BLIT_TO_MEMORY_4TO4BPP(Bit8u *src, Bit8u *dst, Bit16u srcWidth, Bit16u srcHeight, Bit16u dstWidth, Bit16u dstHeight, U8 *aa)
{
	//^44C8:21F3
	ENTER(12);
	//^44C8:21F9
	_4976_5e64 = src;
	//^44C8:2206
	_4976_5e6a = dst;
	//^44C8:2213
	_4976_5dbe = aa;
	//^44C8:2220
	Bit16u bp08 = (dstWidth +1) & 0xfffe;
	//^44C8:222D
	Bit16u bp0a = (srcWidth +1) & 0xfffe;
	//^44C8:2234
	Bit16u bp04 = (i32(i16(srcWidth)) << 7) / i16(dstWidth);
	//^44C8:224F
	Bit16u bp06 = (i32(i16(srcHeight)) << 7) / i16(dstHeight);
	//^44C8:226A
	Bit16u bp02 = bp06 >> 1;
	//^44C8:226F
	Bit16u bp0c = 0; // defaulting to 0
	for (Bit16u si = 0; si < dstHeight; si++) {
		//^44C8:2274
		Bit16u di = bp02 >> 7;
		//^44C8:227C
		if (bp0c == di && si > 0) {
			//^44C8:2285
			_4976_5e64 = dst;
			//^44C8:2292
			FIRE_BLIT_TO_MEMORY_ROW_4TO4BPP((si -1) * bp08, si * bp08, dstWidth);
			//^44C8:22A9
			_4976_5e64 = src;
		}
		//^44C8:22B8
		else if (aa == 0) {
			//^44C8:22C0
			_44c8_20e5(di * bp0a, si * bp08, bp04, dstWidth);
		}
		else {
			//^44C8:22D8
			_44c8_2143(di * bp0a, si * bp08, bp04, dstWidth);
		}
		//^44C8:22F1
		bp0c = di;
		//^44C8:22F4
		bp02 += bp06;
		//^44C8:22FA
	}
	//^44C8:2303
	return;
}
#pragma warning(pop)

//^0B36:06D3
ExtendedPicture *SkWinCore::QUERY_PICST_IT(ExtendedPicture *ref)
{
	// query image with some capacity
	// - stretch

	//^0B36:06D3
	ENTER(364);
	//^0B36:06D9
	Bit16u bp12 = (ref->w52 != 64 || ref->w54 != 64) ? 1 : 0;
	Bit16u bp14 = 0;
	//^0B36:06F9
	Bit16u bp16 = (ref->w6 == 0xffff && ref->w12 >= 0) ? 1 : 0;
	//^0B36:0714
	if (bp12 != 0) {
		//^0B36:071A
		ref->w28 = CALC_STRETCHED_SIZE(ref->w28, ref->w52);
		//^0B36:0732
		ref->w30 = CALC_STRETCHED_SIZE(ref->w30, ref->w54);
	}
	//^0B36:0747
	if ((ref->mirrorFlip & 0x0001) != 0) {
		//^0B36:0752
		ref->w28 = - ref->w28;
	}
	//^0B36:075C
	if (ref->colorKeyPassThrough == -3)
		//^0B36:0766
		//^0B36:0A35
		return ref;
	//^0B36:0769
	if (bp12 == 0 && bp14 == 0) {
		//^0B36:0775
		if (bp16 != 0) {
			//^0B36:077B
			//^0B36:0788
			_0b36_00c3(ref->w12, ref);
			//^0B36:0A35
			return ref;
		}
		else {
			//^0B36:0792
			QUERY_PICST_IMAGE(ref);
			//^0B36:079E
			//^0B36:0A35
			return ref;
		}
	}
	//^0B36:07A1
	Bit16u bp18;
	Bit8u *bp04;
	Bit8u *bp08;
	Picture bp32;
	if (bp16 != 0) {
		//^0B36:07A7
		bp18 = ALLOC_TEMP_CACHE_INDEX();
		//^0B36:07AF
		COPY_MEMORY(ref, &bp32, 24);
		//^0B36:07C6
		Bit8u *bp08 = _0b36_00c3(bp32.w12, &bp32);
		//^0B36:07DB
		ALLOC_PICT_MEMENT(&bp32);
		//^0B36:07E6
		bp04 = NULL;
	}
	else {
		//^0B36:07F3
		Bit32u bp10 = CALC_PICT_ENT_HASH(ref);
		//^0B36:0805
		if (ADD_CACHE_HASH(bp10, &bp18) != 0) {
			//^0B36:081C
			//^0B36:0788
			_0b36_00c3(bp18, ref);
			//^0B36:0A35
			return ref;
		}
		else {
			//^0B36:0828
			bp04 = NULL;
			bp08 = NULL;
			//^0B36:0838
			if (bp08 == NULL) {
				//^0B36:0840
				COPY_MEMORY(ref, &bp32, 24);
				//^0B36:0857
				bp08 = QUERY_PICST_IMAGE(&bp32);
				//^0B36:0868
				ALLOC_PICT_MEMENT(&bp32);
			}
		}
	}
	//^0B36:0873
	if ((bp32.w4 & 0x0010) != 0) {
		//^0B36:087D
		ExtendedPicture bp016c;
		COPY_MEMORY(ref, &bp016c, 314);
		//^0B36:0896
		bp016c.mirrorFlip = 0;
		bp016c.w54 = 64;
		bp016c.w52 = 64;
		//^0B36:08A7
		Bit16u bp1a;
		if (ADD_CACHE_HASH(CALC_PICT_ENT_HASH(&bp016c), &bp1a) == 0) {
			//^0B36:08C6
			bp08 = ALLOC_NEW_PICT(bp1a, bp32.width, bp32.height, bp32.w22);
			//^0B36:08E0
			Bit8u *bp0c = QUERY_PICT_BITS(&bp32);
			//^0B36:08F1
			FIRE_BLIT_PICTURE(
				bp0c,
				bp08,
				ALLOC_TEMP_ORIGIN_RECT(bp32.width, bp32.height),
				bp32.w14,
				bp32.w16,
				READ_UI16(bp0c,-4),
				READ_UI16(bp08,-4),
                -1,
				0,
				bp32.w22,
				bp32.w22,
				NULL
				);
		}
		//^0B36:0936
		FREE_PICT_MEMENT(&bp32);
		//^0B36:0941
		Bit8u *bp08 = _0b36_00c3(bp1a, &bp32);
		//^0B36:0956
		ALLOC_PICT_MEMENT(&bp32);
	}
	//^0B36:0961
	if (bp12 != 0) {
		//^0B36:096A
		i16 di = CALC_STRETCHED_SIZE(bp32.width, ref->w52);
		//^0B36:097C
		i16 si = CALC_STRETCHED_SIZE(bp32.height, ref->w54);
		//^0B36:098E
		if (di <= 0 || si <= 0) {
			//^0B36:0996
			ref->width = di;
			ref->height = si;
		}
		else {
			//^0B36:09A3
			ALLOC_NEW_PICT(bp18, di, si, bp32.w22);
			//^0B36:09B3
			bp08 = QUERY_PICT_BITS(&bp32);
			//^0B36:09C4
			bp04 = _0b36_00c3(bp18, ref);
			//^0B36:09DA
			if (bp32.w22 == 8) {
				//^0B36:09E0
				_44c8_2351(bp08, bp04, bp32.width, bp32.height, di, si);
			}
			else {
				//^0B36:09FE
				FIRE_STRETCH_BLIT_TO_MEMORY_4TO4BPP(bp08, bp04, bp32.width, bp32.height, di, si, NULL);
			}
		}
	}
	//^0B36:0A1E
	FREE_PICT_MEMENT(&bp32);
	//^0B36:0A29
	_3e74_585a(bp18, 0);
	//^0B36:0A35
	return ref;
}

//^0B36:131A
void SkWinCore::DRAW_SIMPLE_STR(sk3f6c *ref, Bit16u rectno, Bit16u clr1, Bit16u clr2, Bit8u *str)
{
	// you can draw:
	// e.g. spell symbols, represented as "<<","Å†","ÅÅ","Åû",")-","áô".
	// e.g. damage amount like "80"

	// their drawStr is regurally scheduled. e.g. you can print damage "80" at portrait/status panel. no another position/text pattern.

	//^0B36:131A
	ENTER(12);
	//^0B36:131E
	i16 bp02;
	i16 bp04;
	SRECT bp0c;
	// bp0c -> bp0c.x
	// bp0a -> bp0c.y
	// bp08 -> bp0c.cx
	// bp06 -> bp0c.cy

	if (QUERY_STR_METRICS(str, &bp02, &bp04) != 0 && QUERY_BLIT_RECT(NULL, &bp0c, rectno, &bp02, &bp04, -1) != 0) {
		//^0B36:135E
		DRAW_STRING(
			reinterpret_cast<Bit8u *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(ref->w0)), 
			ref->w0, 
			ref->rc2.cx, 
			bp0c.x             -ref->rc2.x,
			bp0c.y +bp0c.cy -1 -ref->rc2.y,
			clr1,
			clr2,
			str,
			8
			);
		//^0B36:139D
		_0b36_0d67(ref, &bp0c);
	}
	//^0B36:13AF
	return;
}

//^0B36:10B6
void SkWinCore::DRAW_ICON_PICT_BUFF(
	const Bit8u *buff, 
	sk3f6c *tt, 
	SRECT *rc, 
	i16 srcx, 
	i16 srcy, 
	i16 colorkey, 
	i16 flipmirror, 
	Bit8u *localpal
) {
	// draw icon image such as:
	// a) right part of panel (commander part)
	//		directional/curved arrow, 2x2 floor status panel, command buttons, X button, spell board,
	// b) upper part of panel (status part)
	//		player plate, hand icon, portrait icon
	// c) upper and/or right part of panel
	//		item icon (such as dagger, etc), colored player icon

	//^0B36:10B6
	ENTER(8);
	//^0B36:10BA
	if (rc != NULL) {
		//^0B36:10C2
		SRECT bp08;
		FIRE_BLIT_PICTURE(
			buff,
			QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(tt->w0),
			OFFSET_RECT(tt, rc, &bp08),
			srcx,
			srcy,
			READ_UI16(buff,-4),
			tt->rc2.cx,
			colorkey,
			flipmirror,
			READ_UI16(buff,-6),
			8,
			localpal
			);
		//^0B36:1121
		_0b36_0d67(tt, rc);
	}
	//^0B36:1134
	return;
}

//^0B36:1136
void SkWinCore::DRAW_ICON_PICT_ENTRY(Bit8u cls1, Bit8u cls2, Bit8u cls4, sk3f6c *ss, Bit16u rectno, i16 colorkey)
{
	//^0B36:1136
	ENTER(20);
	//^0B36:113A
	i16 bp0c = 0;
	i16 bp0a = 0;
	//^0B36:1142
	Bit8u *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(cls1, cls2, cls4);
	//^0B36:115C
	SRECT bp14;
	SRECT *bp08 = QUERY_BLIT_RECT(bp04, &bp14, rectno, &bp0a, &bp0c, -1);
	//^0B36:1184
	DRAW_ICON_PICT_BUFF(
		bp04,
		ss,
		bp08,
		bp0a,
		bp0c,
		colorkey,
		0,
		QUERY_GDAT_IMAGE_LOCALPAL(cls1, cls2, cls4)
		);
	//^0B36:11BE
	return;
}

//^29EE:00E0
void SkWinCore::SHOW_ATTACK_RESULT(i16 yourValue)
{
	// yourValue: it means a damage value if positive, it means a failure reason if negative

	// if -1, go fore to attack
	// if -2, hand a empty flask
	// if -3, your meaningless spell
	// if -4, failure wizard spell
	// if -5, failure priest spell
	// if -6, go X-mark to teleport

	//^29EE:00E0
	ENTER(324);
	//^29EE:00E6
	i16 di = yourValue;
	//^29EE:00E9
	_29ee_00a3(1);
	//^29EE:00F0
	Bit8u bp01 = 4;
	Bit8u bp02 = 1;
	//^29EE:00F8
	if (di < 0) {
		//^29EE:00FC
		switch (di) {
			case ATTACK_FAILURE_X_TELEPORT:	// -6
				//^29EE:010F
				bp02 = 0x19; // 0x19 = [icon] move to X-mark to teleport
				break;
			case ATTACK_REQUIRES_HAND_ITEM:	// -2
				//^29EE:011B
				bp02 = 0x17; // 0x17 = [icon] hand a empty flask to cast
				break;
			case ATTACK_FAILURE_NOT_FRONT:	// -1
				//^29EE:0115
				bp02 = 0x18; // 0x18 = [icon] hind player cannot attack physically
				break;
			default:
				//^29EE:0121
				bp01 = 5;
				//^29EE:0125
				if (di == ATTACK_MEANINGLESS_SPELL) {	// -3
					//^29EE:012A
					bp02 = 0x0e; // 0x0e = [icon] unknown spell
				}
				//^29EE:0130
				else if (di == ATTACK_FAILURE_WIZARD) {	// -4
					//^29EE:0135
					bp02 = 0x0c; // 0x0c = [icon] fail wizard spell
				}
				else {	// Supposedly ATTACK_FAILURE_PRIEST !!
					//^29EE:013B
					bp02 = 0x0d; // 0x0d = [icon] fail priest spell
				}
				break;
		}
	}
	//^29EE:013F
	ExtendedPicture bp0144;
	QUERY_GDAT_SUMMARY_IMAGE(&bp0144, 1, bp01, bp02);
	//^29EE:0157
	// SPX: Depending on the amount of damage done, stretch the damage done hit image
	if (di >= 0 && di <= 40) {
		//^29EE:0160
		if (di > 15) {
			//^29EE:0165
			bp0144.w52 = 0x002b;
			bp0144.w54 = 0x003b;
		}
		else {
			//^29EE:0173
			bp0144.w52 = 0x001c;
			bp0144.w54 = 0x002c;
		}
	}
	//^29EE:017F
	_0b36_11c0(QUERY_PICST_IT(&bp0144), &_4976_3f6c, 57, -1);
	//^29EE:019E
	if (di >= 0) {
#if UseAltic
		ATLASSERT(di < 999); di = min(di, 999);
#endif

		//^29EE:01A2
		Bit16u si = 5;
		//^29EE:01A5
		Bit8u bp0a[8];
		bp0a[5] = 0;
		do {
			//^29EE:01A9
			si--;
			//^29EE:01AA
			bp0a[si] = U8(di % 10) + '0';
			//^29EE:01B8
			di = di / 10;
			//^29EE:01BF
		} while (di != 0);

		//^29EE:01C3
		si--;
		bp0a[si] = '!';
		si--;
		bp0a[si] = 0x02;
		//^29EE:01CD
		DRAW_SIMPLE_STR(&_4976_3f6c, 0x39, glbPaletteT16[COLOR_CYAN], glbPaletteT16[COLOR_BLACK], &bp0a[si]);
		//^29EE:01F5
	}
	//^29EE:01F7
	//else if (di == -2) {
	else if (di == ATTACK_REQUIRES_HAND_ITEM) {
		//^29EE:01FC
		DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, (glbWeaponMissileHand == 0) ? GDAT_INTERFACE_BODY_HAND_RIGHT : GDAT_INTERFACE_BODY_HAND_LEFT, &_4976_3f6c, 126, -1);
		//^29EE:021E
		bp01 = 0;
		//^29EE:0222
		// SPX: Search through all weapons to find which projectile fits the current shooter item
		for (; bp01 < 255; bp01++) {
			//^29EE:0224
			Bit16u si = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WEAPONS, bp01, dtWordValue, GDAT_ITEM_WEAPON_PROJECTILE_FLAG); // 0x05
			//^29EE:0238
			// SPX: Check that the weapon is not a launcher (0x8000) and match the shooter flag
			if (si != 0 && (si & WEAPON_FLAGS_SHOOTER) == 0 && (glbWeaponShooterNum & si) != 0) {
				//^29EE:0248
				DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_WEAPONS, bp01, 0x18, &_4976_3f6c, 125, -1);
				//^29EE:025D
				return;
			}
			//^29EE:025F
		}
	}
	//^29EE:0268
	return;
}



//^2759:0541
// SPX: _2759_0541 renamed DISPLAY_RIGHT_PANEL_SQUAD_HANDS
void SkWinCore::DISPLAY_RIGHT_PANEL_SQUAD_HANDS()
{
	//^2759:0541
	ENTER(2);
	//^2759:0545
	i16 bp02 = glbChampionIndex;
	//^2759:054B
	if (bp02 != 0) {
		//^2759:054F
		if (glbChampionAttackDamage == 0 || glbSomeChampionPanelFlag <= 1)
			//^2759:055D
			glbSomeChampionPanelFlag = 0;

		//^2759:0563
		glbChampionIndex = 0;
		glbRightPanelType = RIGHT_PANEL_SQUAD_HANDS;
		glbMagicalMapFlags = 0;
		if (glbObjectForContainer != OBJECT_NULL) {
			//^2759:057C
			PUT_OBJECT_INTO_CONTAINER();
		}
		//^2759:0581
		_4976_531c = 1;
	}
	//^2759:0587
	return;
}


//^2405:014A
// SPX: _2405_014a renamed GET_ITEM_ICON_ANIM_FRAME
Bit8u SkWinCore::GET_ITEM_ICON_ANIM_FRAME(ObjectID recordLink, i16 xx, i16 yy)
{
	//^2405:014A
	ENTER(8);
	//^2405:0150
	Bit8u bp01 = 0x18;	// base image is 0x18
	//^2405:0154
	if (yy != 0) {
		//^2405:015D
		Bit16u si = QUERY_GDAT_DBSPEC_WORD_VALUE(recordLink, 6); // get icon anim value from item
		//^2405:016B
		Bit16u di = si & 15;	// get number of different frames
		//^2405:0170
		if (di != 0) {
			//^2405:0177
			Bit16u bp04 = 0;
			//^2405:017C
			if ((si & 0x8000) == 0 || (bp04 = IS_ITEM_FIT_FOR_EQUIP(recordLink, xx, 1)) != 0) {
				//^2405:019C
				if ((si & 0x4000) != 0) {
					//^2405:01A2
					if (glbChampionIndex == 0 || _4976_3de2[(glbChampionIndex << 1) + (glbSelectedHandAction)] != recordLink)
						//^2405:01C3
						goto _02d3;
					//^2405:01C6
                    bp04 = 1;						
				}
				//^2405:01CB
				if (bp04 != 0) {	// if fit for equip, get the next frame (0x19) which is basic activated icon frame
					//^2405:01D1
					bp01++;
					di--;
				}
				//^2405:01D5
				if (di != 0) {
					//^2405:01DC
					Bit32u bp08 = glbGameTick;
					Bit8u nFramesPerAnim = (si & 0x00E0) >> 5; // SPX added
					//^2405:01E9
					switch ((si & 0x1f00) >> 8) {	// get anim type
						case 5:	// not sure ?
							//^2405:0201
							bp08 += recordLink.DBIndex();

							goto _020e;
						case 0:	// always animated (i.e. potion)
							//^2405:020E
_020e:
							bp01 = bp01 + Bit8u(bp08 % di);
							//^2405:0224
							break;
						case 1: // random frame among remaining possible frames
							//^2405:0227
							bp01 = bp01 + Bit8u(RAND16(di));
							//^2405:0224
							break;
						case 2:	// compass mode : choose from depending on direction
							//^2405:0230
							bp01 = bp01 + glbPlayerDir;
							//^2405:023A
							break;
						case 3:	// charged rate (i.e. waterskin)
							//^2405:023D
							si = ADD_ITEM_CHARGE(recordLink, 0);
							//^2405:0249
							if (si == 0)
								//^2405:024F
								break;
							//^2405:0252
							bp01 = (di * si) / (GET_MAX_CHARGE(recordLink) +1) + bp01;
							//^2405:026E
							break;
						case 6:	// SPX: added nFramesPerAnim which seemed to be missing
							//^2405:0270
							if (nFramesPerAnim == 0)
								break;
							bp08 += recordLink.DBIndex();
							bp01 = bp01 + Bit8u(bp08 % nFramesPerAnim); // SPX added
//							goto _027d;
						case 4:
							//^2405:027D
_027d:
							xx = (si & 0x00e0) >> 5;
							//^2405:0288
							si = ADD_ITEM_CHARGE(recordLink, 0);
							//^2405:0296
							if (si == 0)
								//^2405:0298
								break;
							//^2405:029A
							bp01 = (((di / xx) * si) / (GET_MAX_CHARGE(recordLink) +1)) * xx + (bp08 % yy) +bp01 +1;

							break;
					}
				}
			}
		}
	}
	//^2405:02D3
_02d3:
	return bp01;
}

//^0B36:0334
Bit8u *SkWinCore::TRANSLATE_PALETTE(Bit8u *localpal, Bit8u cls1, Bit8u cls2, Bit8u cls4, i16 palentcnt)
{
	//^0B36:0334
	ENTER(4);
	//^0B36:0339
	Bit8u *bp04 = QUERY_GDAT_ENTRY_DATA_PTR(cls1, cls2, dt07, cls4);
	//^0B36:0355
	for (i16 si = 0; si < palentcnt; si++) {
		//^0B36:0359
		localpal[si] = bp04[localpal[si]];
		//^0B36:036F
	}
	//^0B36:0375
	return localpal;
}

//^2405:05B4
// SPX: _2405_05b4 renamed DRAW_ITEM_ON_WOOD_PANEL
Bit8u *SkWinCore::DRAW_ITEM_ON_WOOD_PANEL(Bit16u player, Bit16u possessionIndex, Picture *ref)
{
	//^2405:05B4
	ENTER(348);
	//^2405:05BA
	if (IS_ITEM_HAND_ACTIVABLE(player, glbChampionSquad[player].Possess(possessionIndex), possessionIndex) == 0) {
		//^2405:05E1
		//^2405:07BE
		return NULL;
	}
	//^2405:05E8
	Bit16u bp0c = _4976_0106 + _4976_010e;
	Bit16u bp0e = _4976_0108 + _4976_0110;
	//^2405:05FC
	Bit16u bp0a = ALLOC_TEMP_CACHE_INDEX();
	//^2405:0604
    Bit8u *bp04 = ALLOC_NEW_PICT(bp0a, bp0c, bp0e, 8);
	//^2405:061D
	ObjectID di = glbChampionSquad[player].Possess(possessionIndex);
	//^2405:0634
	Bit8u bp05;
	Bit8u bp06;
	Bit8u bp07;
	Bit16u bp12;
	if (di == OBJECT_NULL) {
		//^2405:0639
		bp05 = 1;
		bp06 = 2;
		//^2405:0641
		bp07 = (possessionIndex == 1) ? 8 : 7;
		bp12 = 1;
	}
	else {
		//^2405:0657
		bp05 = QUERY_CLS1_FROM_RECORD(di);
		//^2405:0661
		bp06 = QUERY_CLS2_FROM_RECORD(di);
		//^2405:066B
		bp07 = GET_ITEM_ICON_ANIM_FRAME(di, -1, 0);
		//^2405:067A
		bp12 = 0;
	}
	//^2405:067F
	ExtendedPicture bp014c;
	QUERY_GDAT_SUMMARY_IMAGE(&bp014c, bp05, bp06, bp07);
	//^2405:0699
	QUERY_PICST_IT(&bp014c);
	//^2405:06A6
	Bit8u bp0f = glbPaletteT16[COLOR_RED];	// SPX: RED here is used as transparent for hand icon ..
	//^2405:06B1
	FILL_ENTIRE_PICT(bp04 = _0b36_00c3(bp0a, ref), bp0f);
	//^2405:06D5
	if (bp12 != 0) {
		//^2405:06DB
		//^2405:0792
		DRAW_DIALOGUE_PICT(
			QUERY_PICT_BITS(&bp014c),
			bp04,
			ALLOC_TEMP_ORIGIN_RECT(bp014c.width, bp014c.height),
			0,
			0,
			12,
			bp014c.b58
			);
	}
	else {
		//^2405:06F2
		Bit8u bp015c[16];
		for (Bit16u si = 0; si < 16; si++) {
			//^2405:06F6
			bp015c[si] = glbPaletteT16[COLOR_BLACK];
			//^2405:0701
		}
		//^2405:0707
		bp015c[12] = bp0f;
		//^2405:070E
		DRAW_DIALOGUE_PICT(
			QUERY_PICT_BITS(&bp014c),
			bp04,
			ALLOC_TEMP_RECT(_4976_010e, _4976_0110, _4976_0106, _4976_0108),
			0,
			0,
			-1,
			bp015c
			);
		//^2405:0751
		TRANSLATE_PALETTE(bp014c.b58, 1, 0, 1, 16);
		//^2405:0767
		bp04 = _0b36_00c3(bp0a, ref);
		//^2405:077E
		//^2405:0792
		DRAW_DIALOGUE_PICT(
			QUERY_PICT_BITS(&bp014c),
			bp04,
			ALLOC_TEMP_ORIGIN_RECT(_4976_0106, _4976_0108),
			0,
			0,
			12,
			bp014c.b58
			);
	}
	//^2405:07B8
	return bp04;
}

//^44C8:08AE
void SkWinCore::FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(Bit16u offSrc, Bit16u offDst, Bit16u width, i16 colorkey)
{
	// TODO: ìØàÍîüêîÇÃà⁄êAñ‚ëË
	//^44C8:08AE
	ENTER(0);
	//^44C8:08B3
	const Bit8u *bx = glbBlitPalette16;
	Bit8u *di = _4976_5e6a + offDst;
	Bit8u dh = Bit8u(colorkey);
	Bit16u cx = width;
	bool carry = (offSrc & 1) ? true : false;
	offSrc >>= 1;
	Bit8u *si = _4976_5e64 + offSrc;
	//^44C8:08D4
	if (carry) {
		//^44C8:08D6
		Bit8u al = *si; si++;
		al &= 0x0f;
		if (al == dh) {
			di++;
		}
		else {
			*di = bx[al]; di++;
		}
		//^44C8:08E3
		if (--cx == 0)
			//^44C8:08E4
            return;
	}

	{
		//^44C8:08E6
		bool carry = (cx & 1) ? true : false;
		cx >>= 1;
		//^44C8:08E9
		if (cx != 0) {
			//^44C8:08EB
			do {
				Bit8u al = *si; si++;
				Bit8u ah = al & 0x0f;
				al >>= 4;
				if (al != dh) {
					//^44C8:08F8
					if (ah != dh) {
						//^44C8:08FC
						*di = bx[al]; di++;
						*di = bx[ah]; di++;
						//^44C8:0904
						continue;
					}
					else {
						//^44C8:091D
						*di = bx[al]; di++;
						di++;
						//^44C8:0921
						continue;
					}
				}
				//^44C8:0908
				else if (ah != dh) {
					//^44C8:090C
					di++;
					*di = bx[ah]; di++;
					//^44C8:0912
					continue;
				}
				else {
					//^44C8:0916
					di++;
					di++;
					//^44C8:0918
					continue;
				}
				//^44C8:0918
			} while (--cx != 0);
		}
		//^44C8:0923
		if (carry) {
			//^44C8:0926
			Bit8u al = *si; si++;
			al >>= 4;
			if (al != dh) {
				*di = bx[al]; di++;
			}
		}
	}
	//^44C8:0931
	return;
}

//^44C8:1ACA
void SkWinCore::_44c8_1aca(Bit8u *buff, SRECT *rc, Bit16u xx, Bit16u yy)
{
	//^44C8:1ACA
	ENTER(332);
	//^44C8:1AD0
	if (rc != NULL) {
		//^44C8:1ADB
		Bit16u bp08 = xx ^ 15;
		//^44C8:1AE4
		Bit8u bp014c[320];
		_4976_5e64 = bp014c;
		_4976_5e6a = buff;
		//^44C8:1AFC
		Bit16u bp04 = rc->cx;
		Bit16u bp02 = rc->cy;
		//^44C8:1B0D
		if (bp04 > 0 && bp02 > 0) {
			//^44C8:1B1F
			Bit16u bp06 = (bp04 +1) >> 1;
			//^44C8:1B28
			Bit8u bp0b = Bit8u(xx) << 4 | Bit8u(bp08);
			//^44C8:1B34
			Bit16u si;
			for (si = 0; si < bp06; si++) {
				//^44C8:1B38
				bp014c[si] = bp0b;
				//^44C8:1B3F
				bp014c[si+160] = bp0b ^ 0xff;
				//^44C8:1B45
			}
			//^44C8:1B4B
			Bit16u di = rc->y * yy +rc->x;
			//^44C8:1B5A
			Bit16u bp0a = ((di / yy) ^ di) & 1;
			//^44C8:1B67
			for (si = 0; si < bp02; si++) {
				//^44C8:1B6B
				_4976_5e64 = &bp014c[bp0a * 160];
				//^44C8:1B80
				bp0a ^= 1;
				//^44C8:1B84
				FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(0, di, bp04, bp08);
				//^44C8:1B95
				di += yy;
				//^44C8:1B98
			}
		}
	}
	//^44C8:1B9E
	return;
}

//^0B36:105B
// SPX: _0b36_105b renamed DRAW_GRAY_OVERLAY
void SkWinCore::DRAW_GRAY_OVERLAY(sk3f6c *ref, SRECT *rc, Bit16u xx)
{
	//^0B36:105B
	ENTER(8);
	//^0B36:105F
	if (rc != NULL) {
		//^0B36:1067
		SRECT bp08;
		_44c8_1aca(
			reinterpret_cast<Bit8u *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(ref->w0)),
			OFFSET_RECT(ref, rc, &bp08),
			xx,
			ref->rc2.cx
			);
		//^0B36:10A1
		_0b36_0d67(ref, rc);
	}
	//^0B36:10B4
	return;
}

//^29EE:026C
// SPX: _29ee_026c renamed DRAW_HAND_ACTION_ICONS
void SkWinCore::DRAW_HAND_ACTION_ICONS(Bit16u playerIndex, Bit16u possessionIndex, Bit16u leftOrRight)
{
	//^29EE:026C
	ENTER(48);
	//^29EE:0271
	_29ee_00a3(0);
	//^29EE:0278
	Champion *champion = &glbChampionSquad[playerIndex];
	//^29EE:0289
	Bit16u si = (possessionIndex == 1) ? 0x46 : 0x4a;
	//^29EE:0297
	si += (champion->playerPos() +4 - glbPlayerDir) & 3;
	//^29EE:02AC
	SRECT bp10;
	QUERY_EXPANDED_RECT(si, &bp10);
	//^29EE:02BA
	if (champion->curHP() == 0) {
		//^29EE:02C4
		SRECT bp10;
		FILL_RECT_SUMMARY(&_4976_3f6c, &bp10, glbPaletteT16[COLOR_BLACK]);
	}
	else {
		//^29EE:02DF
		// SPX: Draw background item square
		DRAW_ICON_PICT_ENTRY(0x01, 0x04, (Bit8u(possessionIndex) << 1) + Bit8u(leftOrRight) +2, &_4976_3f6c, si, -1);
		//^29EE:02FD
		Picture bp30;
		Bit8u *bp08 = DRAW_ITEM_ON_WOOD_PANEL(playerIndex, possessionIndex, &bp30);
		//^29EE:0316
		if (bp08 != NULL) {
			//^29EE:031A
			SRECT bp18;
			// SPX: draw item (weapon, shield, flask, etc ..) in hand
			DRAW_ICON_PICT_BUFF(
				bp08,
				&_4976_3f6c,
				CALC_CENTERED_RECT_IN_RECT(&bp18, &bp10, READ_UI16(bp08,-4), READ_UI16(bp08,-2)),
				0,
				0,
				glbPaletteT16[COLOR_RED],
				0,
				NULL
				);
			//^29EE:0360
			FREE_PICT_MEMENT(&bp30);
		}
		//^29EE:036C
		if (champion->handCooldown[possessionIndex] != 0 || glbIsPlayerSleeping != 0) {
			//^29EE:0380
			DRAW_GRAY_OVERLAY(&_4976_3f6c, &bp10, 0);
		}
	}
	//^29EE:0393
	return;
}

//^29EE:058D
// SPX: _29ee_058d renamed DRAW_SQUAD_SPELL_AND_LEADER_ICON
void SkWinCore::DRAW_SQUAD_SPELL_AND_LEADER_ICON(Bit16u player, Bit16u yy)
{
	//^29EE:058D
    ENTER(324);
	//^29EE:0593
	_29ee_00a3(0);
	//^29EE:059A
	Bit16u si = (glbChampionSquad[player].playerPos() +4 - glbPlayerDir) & 3;
	//^29EE:05B6
	SRECT bp0a;
	FILL_RECT_SUMMARY(
		&_4976_3f6c,
		QUERY_EXPANDED_RECT(si + 0x4f, &bp0a),
		glbPaletteT16[COLOR_BLACK]
		);
	//^29EE:05E1
	if (glbChampionSquad[player].curHP() != 0) {
		//^29EE:05F5
		Bit16u di = (si == 1 || si == 2) ? 1 : 0;
		//^29EE:0606
		Bit8u bp01;
		Bit8u bp02;
		if (si <= 1) {
			//^29EE:060B
			bp01 = 10;
			bp02 = 6;
		}
		else {
			//^29EE:0615
			bp01 = 12;
			bp02 = 8;
		}
		//^29EE:061D
		if (player == glbChampionLeader) {
			//^29EE:0626
			bp01++;
		}
		//^29EE:0629
		if (yy != 0) {
			//^29EE:062F
			bp02++;
		}
		//^29EE:0632
		ExtendedPicture bp0144;
		QUERY_GDAT_SUMMARY_IMAGE(&bp0144, 1, 4, bp02);
		//^29EE:0648
		bp0144.mirrorFlip = di;
		//^29EE:064C
		_0b36_11c0(
			QUERY_PICST_IT(&bp0144),
			&_4976_3f6c,
			si + 0x57,
			4
			);
		//^29EE:066F
		if (glbIsPlayerSleeping != 0 || glbChampionSquad[player].herob44 != 0) {
			//^29EE:0687
			bp0144.rc36.x += _4976_3f6c.rc2.x;
			bp0144.rc36.y += _4976_3f6c.rc2.y;
			//^29EE:0695
			DRAW_GRAY_OVERLAY(&_4976_3f6c, &bp0144.rc36, 0);
		}
		//^29EE:06A9
		QUERY_GDAT_SUMMARY_IMAGE(&bp0144, 0x01, 0x04, bp01);
		//^29EE:06BF
		bp0144.mirrorFlip = di;
		//^29EE:06C3
		_0b36_11c0(
			QUERY_PICST_IT(&bp0144),
			&_4976_3f6c,
			si + 0x53,
			4
			);
		//^29EE:06E6
		if (glbIsPlayerSleeping != 0) {
			//^29EE:06ED
			bp0144.rc36.x = _4976_3f6c.rc2.x;
			bp0144.rc36.y = _4976_3f6c.rc2.y;
			//^29EE:06FB
			DRAW_GRAY_OVERLAY(&_4976_3f6c, &bp0144.rc36, 0);
		}
	}
	//^29EE:070F
	return;
}



//^29EE:2048
void SkWinCore::DRAW_MONEYBOX(ObjectID rl)
{
	//^29EE:2048
	ENTER(56);
	//^29EE:204E
	DRAW_ICON_PICT_ENTRY(0x14, QUERY_CLS2_FROM_RECORD(rl), 0x10, &_4976_3f6c, 0x5c, -1);
	i16 bp38[MONEY_ITEM_MAX];
	COUNT_BY_COIN_TYPES(rl, bp38);
	i16 si;
	for (si = 0; si < MONEY_ITEM_MAX; si++) {
		//^29EE:2081
		i16 bp0c = GET_ITEM_ORDER_IN_CONTAINER(rl, si);
		if (bp0c < 0)
			continue;
		i16 di = bp38[bp0c];
		if (di <= 0)
			continue;
		//^29EE:20AC
		U16 bp10 = glbMoneyItemsIDTable[bp0c];
		X8 bp09;
		X8 bp0a;
		U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(
			bp09 = QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(bp10),
			bp0a = GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(bp10),
			0x18
			);
		U8 *bp08 = QUERY_GDAT_IMAGE_LOCALPAL(bp09, bp0a, 0x18);
		i16 bp14 = 0;
		i16 bp12 = 0;
		SRECT bp1c;
		QUERY_BLIT_RECT(bp04, &bp1c, si +0xdd, &bp12, &bp14, -1);
		SRECT bp24;
		bp24.cx = bp1c.cx;
		bp24.cy = bp1c.cy;
		X16 bp0e = (si << 3) +max_value(0, di -32);
		di = min_value(32, di);
		do {
			//^29EE:2158
			bp0e = (bp0e +1) & 31;
			bp24.x = bp1c.x + _4976_0154[bp0e][0];
			bp24.y = bp1c.y + _4976_0154[bp0e][1];
			UNION_RECT(&bp24, &_4976_3f6c.rc2, &bp12, &bp14);
			DRAW_ICON_PICT_BUFF(bp04, &_4976_3f6c, &bp24, bp12, bp14, 12, 0, bp08);
			//^29EE:21C8
		} while (--di > 0);
		//^29EE:21CB
	}
	//^29EE:21D4
	return;
}

//^29EE:21D8
// SPX: _29ee_21d8 renamed DRAW_CONTAINER_PANEL
void SkWinCore::DRAW_CONTAINER_PANEL(ObjectID rl, Bit16u xx)
{
// xx = 1 : right panel / xx = 0 : inventory
	//^29EE:21D8
	ENTER(24);
	//^29EE:21DE
	X8 bp05 = QUERY_CLS2_FROM_RECORD(rl);
	if (xx != 0) {
		//^29EE:21F0
		DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_CONTAINERS, bp05, GDAT_CONTAINER_STAT_10, &_4976_3f6c, 0x5c, -1);
		DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_CONTAINERS, bp05, GDAT_CONTAINER_STAT_12, &_4976_3f6c, 0xe3, 10);
	}
	SRECT bp18;
	if (xx == 0) {
		//^29EE:2224
		QUERY_EXPANDED_RECT(0x5c, &bp18);
	}
	//^29EE:2233
	X16 si;
	for (si = 0; si < CONTAINER_MAX_SLOT; si++) {	// < 8
		//^29EE:2238
		ObjectID di = glbCurrentContainerItems[si];
		if (di != OBJECT_NULL) {
			X16 bp08 = si +0xe5;
			if (xx == 0) {
				//^29EE:2258
				SRECT bp10;
				QUERY_EXPANDED_RECT(bp08, &bp10);
				U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_CONTAINERS, bp05, GDAT_CONTAINER_STAT_10);	// 0x14	.. 0x10
				DRAW_ICON_PICT_BUFF(
					bp04, &_4976_3f6c, &bp10, bp10.x -bp18.x, bp10.y -bp18.y,
					-1, 0, QUERY_GDAT_IMAGE_LOCALPAL(GDAT_CATEGORY_CONTAINERS, bp05, GDAT_CONTAINER_STAT_10)	// 0x14	.. 0x10
					);
			}
			//^29EE:22B9
			DRAW_ICON_PICT_ENTRY(
				QUERY_CLS1_FROM_RECORD(di),
				QUERY_CLS2_FROM_RECORD(di),
				GET_ITEM_ICON_ANIM_FRAME(di, si +0x1e, 1),
				&_4976_3f6c, bp08, 12
				);
		}
		//^29EE:22EC
	}
	//^29EE:22F5
	return;
}

//^29EE:0396
// SPX: _29ee_0396 renamed DRAW_SQUAD_POS_INTERFACE
void SkWinCore::DRAW_SQUAD_POS_INTERFACE()
{
	//^29EE:0396
	ENTER(350);
	//^29EE:039C
	_29ee_00a3(0);
	//^29EE:03A3
	DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_GRAPHICSSET, glbSquadInterfaceMapGfxSet, GDAT_GFXSET_SQUAD_4X, &_4976_3f6c, 47, -1);	// gfx = 0xF5
	//^29EE:03BC
	Bit8u *bp04 = ALLOC_PICT_BUFF(_4976_0118, _4976_011a, afDefault, 4);
	//^29EE:03D6
	SRECT bp14;
	for (Bit16u si = 0; si < glbChampionsCount; si++) {
		//^29EE:03DB
		Bit16u bp06 = (glbChampionSquad[si].playerPos() +4 - glbPlayerDir) & 3;
		//^29EE:03F7
		if (glbChampionSquad[si].curHP() == 0 || bp06 +1 == _4976_5dbc)
			//^29EE:0414
			continue;
		//^29EE:0417
		i16 bp0c = 0;
		i16 bp0a = 0;
		//^29EE:041F
		Bit16u bp08;
		if (QUERY_BLIT_RECT(bp04, &bp14, bp08 = bp06 +53, &bp0a, &bp0c, -1) == NULL)
			//^29EE:044C
			continue;
		//^29EE:044F
		if (glbGlobalSpellEffects.AuraOfSpeed != 0) {
			//^29EE:0456
			i16 di = RAND02();
			//^29EE:045D
			if (di != 0) {
				//^29EE:0461
				bp14.y += di -2;
				//^29EE:0467
				di = RAND02();
				//^29EE:046E
				if (di != 0) {
					//^29EE:0472
					bp14.x += di -2;
				}
			}
		}
		//^29EE:0478
		Bit8u bp015e[16];
		DRAW_ICON_PICT_BUFF(
			QUERY_GDAT_SQUAD_ICON(bp04, Bit8u(si), bp015e),
            &_4976_3f6c,
			&bp14,
			bp0a,
			bp0c,
			12,
			0,
			bp015e
			);
		//^29EE:04B0
		if (glbChampionSquad[si].enchantmentPower == 0)
			//^29EE:04C0
			continue;
		//^29EE:04C3
		ExtendedPicture bp014e;
		// SPX: Check in INTERFACE (0x01) 07 => Party protection animations
		QUERY_PICST_IT(QUERY_GDAT_SUMMARY_IMAGE(&bp014e, GDAT_CATEGORY_INTERFACE_GENERAL, 0x07, glbChampionSquad[si].enchantmentAura));
		//^29EE:04EC
		bp014e.w4 |= 0x10;
		bp014e.width >>= 2;
		//^29EE:04F7
		bp014e.w14 = bp014e.width * ((glbChampionSquad[si].playerDir() +4 - glbPlayerDir) & 3);
		//^29EE:051E
		bp014e.height >>= 2;
		//^29EE:0523
		bp014e.w16 = bp014e.height * (i16(glbGameTick) & 3);
		//^29EE:0535
		_0b36_11c0(&bp014e, &_4976_3f6c, bp08, 12);
		//^29EE:054C
	}
	//^29EE:0556
	FREE_PICT_BUFF(bp04);
	//^29EE:0563
	if (glbIsPlayerSleeping != 0) {
		//^29EE:056A
		DRAW_GRAY_OVERLAY(
			&_4976_3f6c, 
			QUERY_EXPANDED_RECT(47, &bp14),
			0
			);
	}
	//^29EE:0589
	return;
}

//^3929:06E1
void SkWinCore::DRAW_STRONG_TEXT(Bit8u *buff, Bit16u ww, Bit16u cx, Bit16u xx, Bit16u yy, Bit16u clr1, Bit16u fill, Bit8u *str)
{
	// draw bold with shadow single-byte-character-set text such as,
	// a) name "TORHAM"
	// b) HP "651/651", MP "446/446", stamina "228/228", weight "25.0/62.0 KG"

	//^3929:06E1
	ENTER(4);
	//^3929:06E7
	Bit16u di = xx;
	Bit16u si = yy;
	//^3929:06ED
	i16 bp02;
	i16 bp04;
	if (QUERY_STR_METRICS(str, &bp02, &bp04) != 0) {
		//^3929:070B
		if ((fill & 0x4000) == 0) {
			//^3929:0712
			FIRE_FILL_RECT_ANY(
				buff,
				ALLOC_TEMP_RECT(di -1, si -bp04, bp02 +2, bp04 +2),
				fill,
				cx,
				8
				);
		}
		//^3929:0748
		DRAW_STRING(buff, ww, cx, di   , si +1, glbPaletteT16[COLOR_BLACK], glbPaletteT16[COLOR_WHITE] | 0x4000, str, 8); // shadow text, black
		//^3929:077D
		DRAW_STRING(buff, ww, cx, di +1, si +1, glbPaletteT16[COLOR_BLACK], glbPaletteT16[COLOR_WHITE] | 0x4000, str, 8);	// shadow text, black
		//^3929:07B5
		DRAW_STRING(buff, ww, cx, di,    si,    clr1,          fill           | 0x4000, str, 8);
	}
	//^3929:07DD
	return;
}

//^0B36:13B1
void SkWinCore::DRAW_NAME_STR(sk3f6c *ref, Bit16u rectno, Bit16u clr1, Bit16u fill, Bit8u *str)
{
	// draw text:
	// a) player's name "TORHAM", "SAROS" at upper-panel and command bar
	// b) command name "éaÇÈ", "éaÇËóÙÇ≠", "ífÇøêÿÇÈ", "<02>!{UM} {ZO} {BRO} {ROS}"

	//^0B36:13B1
	ENTER(12);
	//^0B36:13B6
	i16 bp02;
	i16 bp04;
	if (QUERY_STR_METRICS(str, &bp02, &bp04) != 0) {
		//^0B36:13D1
		SRECT bp0c;
		if (QUERY_BLIT_RECT(NULL, &bp0c, rectno, &bp02, &bp04, -1) != NULL) {
			//^0B36:13F5
			DRAW_STRONG_TEXT(
				reinterpret_cast<Bit8u *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(ref->w0)),
				ref->w0,
				ref->rc2.cx,
				bp0c.x             -ref->rc2.x,
				bp0c.y +bp0c.cy -1 -ref->rc2.y,
				clr1,
				fill,
				str
				);
            //^0B36:1432
			_0b36_0d67(ref, &bp0c);
		}
	}
	//^0B36:1444
	return;
}

//^29EE:0713
void SkWinCore::DRAW_PLAYER_NAME_AT_CMDSLOT()
{
	//^29EE:0713
	ENTER(0);
	//^29EE:0716
	DRAW_ICON_PICT_ENTRY(0x01, 0x04, 0x14, &_4976_3f6c, 60, -1);
	//^29EE:072C
	DRAW_ICON_PICT_ENTRY(0x01, 0x04, 0x0e, &_4976_3f6c, 59, -1);
	//^29EE:0742
	DRAW_NAME_STR(
		&_4976_3f6c, 
		61, 
		(glbChampionIndex -1 == glbChampionLeader) ? glbPaletteT16[COLOR_ORANGE] : glbPaletteT16[COLOR_WHITE], 
		glbPaletteT16[COLOR_DARKEST_GRAY] | 0x4000, 
		glbChampionTable[glbChampionIndex].firstName
		);
	// SPX: This (9 : 15) controls the color of the char name in the cast spell panel.
	// However, 9 is dark brown for leader, and that was wrong. Leader is to be orange. Non-leader is white
	//^29EE:0789
	return;
}

//^0CAF:0008
i16 SkWinCore::SK_STRLEN(const Bit8u *ref)
{
	//^0CAF:0008
	ENTER(4);
	//^0CAF:000C
	const Bit8u *bp04 = ref;
	//^0CAF:0018
	while (*(ref++) != 0);
	//^0CAF:0024
	return Bit16u(ref -bp04 -1);
}

//^29EE:093E
// SPX: _29ee_093e renamed DRAW_SPELL_TO_BE_CAST
void SkWinCore::DRAW_SPELL_TO_BE_CAST(Bit16u xx)
{
	//^29EE:093E
	ENTER(8);
	//^29EE:0944
	_29ee_00a3(0);
	//^29EE:094B
	// SPX: Cyan rectangles to be put over spell line
	if (xx != 0) {
		//^29EE:0951
		DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 0x05, 0x09, &_4976_3f6c, 252, -1);
	}
	//^29EE:0968
	Champion *champion = &glbChampionTable[glbChampionIndex];
	//^29EE:0979
	Bit8u bp08[2];
    bp08[1] = 0;
	//^29EE:097D
	Bit16u bp06 = SK_STRLEN(champion->GetRunes());
	//^29EE:0991
	Bit16u di = 261;
	//^29EE:0994
	for (Bit16u si = 0; si < bp06; si++) {
		//^29EE:0998
		bp08[0] = champion->GetRunes()[si];
		//^29EE:09A2
		// SPX: Spell runes color
		DRAW_SIMPLE_STR(&_4976_3f6c, di++, glbPaletteT16[COLOR_BLACK], glbPaletteT16[COLOR_LIGHTER_GRAY] | 0x4000, bp08);
		//^29EE:09CB
	}
	//^29EE:09D1
	return;
}

//^29EE:09D5
// SPX: _29ee_09d5 renamed DRAW_SPELL_PANEL
void SkWinCore::DRAW_SPELL_PANEL()
{
	//^29EE:09D5
	ENTER(10);
	//^29EE:09DA
	Champion *champion = &glbChampionTable[glbChampionIndex];
	//^29EE:09EB
	Bit16u bp06 = champion->runesCount;
	//^29EE:09F7
	// SPX: rune class interface
	DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 0x05, bp06 +1, &_4976_3f6c, 92, -1);
	//^29EE:0A11
	if (bp06 < 4) {
		//^29EE:0A17
		Bit8u bp0a[2];
		bp0a[1] = 0;
		//^29EE:0A1B
		Bit8u bp07 = Bit8u(bp06 * 6) + 0x60;
		Bit16u si = 255;
		//^29EE:0A2B
		for (bp06 = 0; bp06 < 6; bp06++) {
			//^29EE:0A32
			bp0a[0] = bp07++;
			//^29EE:0A3B
			DRAW_SIMPLE_STR(
				&_4976_3f6c, 
				si++, 
				glbPaletteT16[COLOR_BLACK],
				glbPaletteT16[COLOR_LIGHTER_GRAY] | 0x4000,
				bp0a
				);
			//^29EE:0A64
		}
	}
	//^29EE:0A6D
	DRAW_SPELL_TO_BE_CAST(0);
	//^29EE:0A74
	DRAW_PLAYER_ATTACK_DIR();
	//^29EE:0A78
	return;
}

//^0CAF:00D2
Bit8u *SkWinCore::SK_STRSTR(const Bit8u *xx, const Bit8u *yy)
{
	//^0CAF:00D2
	ENTER(10);
	//^0CAF:00D6
	Bit8u cl = yy[0];
	//^0CAF:00DE
	if (cl != 0) {
		//^0CAF:00E2
		Bit8u bp09;
		for (; (bp09 = xx[0]) != 0; xx++) {
			//^0CAF:00E4
			if (bp09 == cl) {
				//^0CAF:00EB
				const Bit8u *bp04 = xx +1;
				const Bit8u *bp08 = yy +1;
				//^0CAF:0105
				while (bp08[0] != 0 && bp04[0] == bp08[0]) {
					//^0CAF:0107
					bp04++;
					bp08++;
					//^0CAF:010D
				}
				//^0CAF:0124
				if (bp08[0] == 0) {
					//^0CAF:012F
					return const_cast<Bit8u *>(xx);
				}
			}
			//^0CAF:0137
		}
	}
	//^0CAF:0147
	return NULL;
}

//^2759:006D
Bit16u SkWinCore::QUERY_CMDSTR_TEXT(const Bit8u *cmdStr, const Bit8u *name)
{
	// returns value of name in cmdstr.
	// for example, returns 0 if you query "WH" for "⁄…:CM4SK4BZ2TR3TA-2EX5PB38DM40".

	// cmdStr: e.g. "⁄…:CM4SK4BZ2TR3TA-2EX5PB38DM40".
	// name: e.g. "WH"

	// Note: BETA uses another format: "FREEZE:CM=11 ST=10 SK=14 BZ=20 TR=3 TA=-10 EX=22"


	//^2759:006D
	ENTER(2);
	//^2759:0073
	i16 si = 0;
	while ((cmdStr = SK_STRSTR(cmdStr, name)) != NULL) {
		//^2759:0077
		cmdStr += SK_STRLEN(name);
		//^2759:0087
		if (cmdStr[0] == '=') {
			//^2759:0090
			cmdStr++;
		}
		//^2759:0093
		Bit16u di = 0;
		//^2759:0095
		if (cmdStr[0] == '-') {
			//^2759:009E
			cmdStr++;
			di = 1;
		}

		Bit8u bp01;
		while ((bp01 = cmdStr[0]) >= '0' && bp01 <= '9') {
			//^2759:00A6
			si = bp01 +(si * 10) - '0';
			cmdStr++;
			//^2759:00BD
		}
		//^2759:00D0
		if (di != 0) {
			//^2759:00D4
			si = -si;
		}
		//^2759:00DA
	}
	//^2759:00FB
	return si;
}

//^2759:0101
Bit16u SkWinCore::QUERY_CMDSTR_ENTRY(Bit8u cls1, Bit8u cls2, Bit8u cls4, Bit16u cmdNum)
{
	//^2759:0101
	ENTER(132);
	//^2759:0105
	Bit8u bp0084[128];
	Bit8u *bp04;
	if (cmdNum >= 18 || *(bp04 = QUERY_GDAT_TEXT(cls1, cls2, cls4, bp0084)) == 0) {
		//^2759:0135
		return 0;
	}
	else {
		//^2759:0139
		return QUERY_CMDSTR_TEXT(bp04, reinterpret_cast<const Bit8u *>(ItemCommandsTable[cmdNum]));
	}
}



//^1031:0B7E
void SkWinCore::_1031_0b7e() //#DS=4976
{
	//^1031:0B7E
	ENTER(0);
	//^1031:0B81
	if (_4976_4e9a != 0) {
		//^1031:0B88
		_4976_4e9a = 0;
		//^1031:0B8E
		FIRE_QUEUE_MOUSE_EVENT(_4976_4e4a, _4976_4e4c, _4976_4e6a);
	}
	//^1031:0BA2
	return;
}

//^1031:098E
void SkWinCore::_1031_098e() //#DS=4976
{
	//^1031:098E
	ENTER(0);
	//^1031:0993
	while (_476d_05a9() != 0)
		//^1031:0995
		_476d_050e();
	//^1031:09A3
	_4976_19a7 = 1;
	//^1031:09A9
	i16 di = _4976_4ea6;
	//^1031:09AD
	i16 si = _4976_19a5 +1;
	//^1031:09B3
	if (si > 10) {
		//^1031:09B8
		si = 0;
	}
	//^1031:09BA
	if (si != di) {
		//^1031:09C1
		si = di;
		_4976_4e00 = 0;

		while (true) {
			//^1031:09C9
			if (false
				|| _4976_4e02[si].MouseButton() == 0x0040
				|| _4976_4e02[si].MouseButton() == 0x0060
				|| _4976_4e02[si].MouseButton() == 0x0004
			) {
				//^1031:09F9
				_4976_4e02[di] = _4976_4e02[si];
				//^1031:0A19
				di++;
				//^1031:0A1C
				if (di > 10)
					//^1031:0A21
					di = 0;
				//^1031:0A23
				_4976_4e00++;
			}
			//^1031:0A27
			if (si == _4976_19a5)
				//^1031:0A2B
				break;
			//^1031:0A2D
			si++;
			//^1031:0A2E
			if (si > 10)
				//^1031:0A35
				si = 0;
			//^1031:0A37
			continue;
		}
		//^1031:0A39
		if (di-- == 0) {
			//^1031:0A40
			di = 10;
		}
		//^1031:0A43
		_4976_19a5 = di;
	}
	//^1031:0A47
	_4976_4e62 = 0;
	_4976_4e60 = 0;
	_4976_4e5e = 0;
	_4976_4df8 = 0xffff;
	_4976_4e44 = 0xffff;
	_4976_4e66 = 0;
	_4976_4e68 = 0;
	glbUIEventCode = 0;
	_4976_4dfc = 0;
	_4976_19a7 = 0;
	//^1031:0A7F
	_1031_0b7e();
	//^1031:0A84
	return;
}

//^1031:023B
Bit8u *SkWinCore::_1031_023b(sk1891 *xx)
{
	//^1031:023B
	ENTER(0);
	//^1031:023E
    return &_4976_169c[xx->w2];
}	

//^443C:087C
void SkWinCore::_443c_087c()
{
	//^443C:087C
	ENTER(0);
	//^443C:087F
	LOCK_MOUSE_EVENT();
	//^443C:0888
	return;
}

//^443C:0889
void SkWinCore::_443c_0889()
{
	//^443C:0889
	ENTER(0);
	//^443C:088C
	UNLOCK_MOUSE_EVENT();
	//^443C:0895
	return;
}

//^443C:00A9
void SkWinCore::_443c_00a9(sk0cea *ref, i16 x, i16 cx, i16 y, i16 cy)
{
	//^443C:00A9
	ENTER(0);
	//^443C:00AC
	_4976_5da4 = ref;
	//^443C:00B9
	_4976_5da8 = _4976_5d98 = x;
	//^443C:00C2
	_4976_5dae.rc4.x = cx;
	//^443C:00C8
	_4976_5d9c = _4976_5dae.rc4.x -x +1;
	//^443C:00CF
	_4976_5daa = _4976_5d9a = y;
	//^443C:00D8
	_4976_5dae.rc4.y = cy;
	//^443C:00DE
	_4976_5d9e = _4976_5dae.rc4.y -y +1;
	//^443C:00E5
	_01b0_0ca4(&_4976_5d98, 0x20);
	//^443C:00F6
	return;
}

//^443C:06B4
void SkWinCore::_443c_06b4(sk0cea *ref)
{
	//^443C:06B4
	ENTER(8);
	//^443C:06B9
	if (ref->b3_7_7() == 0) {
		//^443C:06CB
		ref->b3_7_7(1);
		//^443C:06D0
		skxxx7 *bp08 = &_4976_5dae;
		//^443C:06D8
		sk0cea *bp04 = _4976_5dae.pv0;
		//^443C:06DF
		while (bp04 != NULL && (bp04->b3_0_3()) > (ref->b3_0_3())) {
			//^443C:06E1
			bp08 = bp04->pv6();
			//^443C:06F2
			bp04 = bp08->pv0;
			//^443C:06FC
		}
		//^443C:0721
		_443c_087c();
		//^443C:0726
		ref->pv6()->pv0 = bp04;
		//^443C:073A
		bp08->pv0 = ref;

		{
			//^443C:074A
			SRECT *bp04 = &ref->pv6()->rc4;
			//^443C:075E
			QUERY_EXPANDED_RECT(ref->w0_0_d(), bp04);
			//^443C:0773
			if ((ref->w0_f_f()) != 0) {
				//^443C:077D
				bp04->x = _4976_00e8;
				bp04->y = _4976_00ea;
			}
			//^443C:078D
			if (ref->b5() != 0) {
				//^443C:0797
				_443c_00a9(
					ref,
					bp04->x,
					bp04->x + bp04->cx -1,
					bp04->y,
					bp04->y + bp04->cy -1
					);
			}
			//^443C:07C9
			_443c_0889();
			//^443C:07CE
			_443c_040e();
		}
	}
	//^443C:07D2
	return;
}

//^443C:07D5
void SkWinCore::_443c_07d5(sk0cea *ref)
{
	//^443C:07D5
    ENTER(8);
	//^443C:07D9
	if (ref->b3_7_7() != 0) {
		//^443C:07E3
		ref->b3_7_7(0);
		//^443C:07E8
		skxxx7 *bp08 = &_4976_5dae;
		//^443C:07F0
		sk0cea *bp04 = _4976_5dae.pv0;
		//^443C:07F7
		while (bp04 != ref) {
			//^443C:07F9
			if (bp04 == NULL) {
				//^443C:07FF
				return;
			}
			//^443C:0801
			bp08 = bp04->pv6();
			//^443C:0812
			bp04 = bp08->pv0;
			//^443C:081C
		}
		//^443C:0832
		_443c_087c();
		//^443C:0837
		bp08->pv0 = bp04->pv6()->pv0;
		//^443C:084F
		_443c_0889();
		//^443C:0854
		_443c_040e();
	}
	//^443C:0858
	return;
}

//^1031:0541
void SkWinCore::_1031_0541(Bit16u xx) //#DS=4976
{
	//^1031:0541
	ENTER(12);
	//^1031:0547
	if (xx != _4976_19ad) {
		//^1031:0550
		_1031_098e();
	}
	//^1031:0555
	_4976_19ad = xx;
	//^1031:055B
	_1031_027e(&_4976_1891[_4976_19ad]);
	//^1031:0569
	sk16ed *bp04 = _4976_16ed;
	//^1031:0571
	i16 di;
	for (di = 0; di < 60; bp04++, di++) {
		//^1031:0575
		Bit16u si = bp04->b6;
		//^1031:0580
		if (!(si & 0x40) != !(si & 0x80)) {
			//^1031:0597
			if ((si & 0x80) != 0) {
				//^1031:059D
				if ((si &= 0x3f) != 0) {
					//^1031:05A3
					_4976_0ce0[si].b3_5_5(1);
				}
				//^1031:05B1
				bp04->b6 = bp04->b6 & 0x7f;
			}
			else {
				//^1031:05C0
				if ((si &= 0x3f) != 0) {
					//^1031:05C6
					_4976_0ce0[si].b3_4_4(1);
				}
				//^1031:05D4
				bp04->b6 |= 0x80;
			}
		}
		//^1031:05DC
		bp04->b6 = bp04->b6 & 0xbf;
		//^1031:05E9
	}
	//^1031:05F3
	sk0cea *bp08 = &_4976_0ce0[1];
	//^1031:05FB
	for (di = 0; di < 18; bp08++, di++) {
		//^1031:05FF
		i16 bp0a = (bp08->b3_4_4());
		//^1031:060D
		i16 bp0c = (bp08->b3_5_5());
		//^1031:0618
		if ((!bp0a) != (!bp0c)) {
			//^1031:062C
			if (bp0a != 0) {
				//^1031:0632
				_443c_06b4(bp08);
			}
			else {
				//^1031:063F
				_443c_07d5(bp08);
			}
		}
		//^1031:064C
		bp08->b3_4_4(0);
		bp08->b3_5_5(0);
		//^1031:065D
	}
	//^1031:0659
	return;
}

//^1031:0667
void SkWinCore::_1031_0667() //#DS=4976
{
	//^1031:0667
	ENTER(0);
	//^1031:066A
	_1031_0541(_4976_19ad);
}

//^2759:0009
Bit8u *SkWinCore::QUERY_CMDSTR_NAME(Bit8u cls1, Bit8u cls2, Bit8u cls4)
{
	// returns cmdStr name such as "CHOP", "THRUST", "MELEE", and so on.

	//^2759:0009
	ENTER(134);
	//^2759:000E
	Bit8u bp0086[128];
	Bit8u *bp04 = QUERY_GDAT_TEXT(cls1, cls2, cls4, bp0086);
	//^2759:0032
	if (*bp04 == 0) {
		//^2759:0038
		return NULL;
	}
	//^2759:003E
	Bit8u bp05;
	Bit16u si;
	for (si = 0; (bp05 = *(bp04++)) != ':' && bp05 != 0; si++) {
		//^2759:0042
		glbStrBufferActionName[si] = bp05;
	}
	//^2759:0060
    glbStrBufferActionName[si] = 0;
	//^2759:0065
	return glbStrBufferActionName;
}

//^29EE:0A7B
void SkWinCore::DRAW_CMD_SLOT(Bit16u cmdSlot, Bit8u ww)
{
	//^29EE:0A7B
	//^29EE:0A7F
	Bit16u si = cmdSlot;
	//^29EE:0A82
	_29ee_00a3(0);
	//^29EE:0A89
	if (glbMagicalMapFlags != 0) {
		//^29EE:0A90
		DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_CONTAINERS, glbHoldedContainerType, ((glbItemSelected[si].entry +0xf8) << 1) +ww +0x41, &_4976_3f6c, si +110, -1);
	}
	else {
		//^29EE:0ABC
		DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 0x04, ww +0x15, &_4976_3f6c, si +63, -1);
		//^29EE:0ADA
		DRAW_NAME_STR(
			&_4976_3f6c,
			si +66,
			glbPaletteT16[COLOR_WHITE],
			glbPaletteT16[COLOR_BLACK] | 0x4000,
			QUERY_CMDSTR_NAME(glbItemSelected[si].category, glbItemSelected[si].index, glbItemSelected[si].entry)
			);
	}
	//^29EE:0B25
	//^29EE:0B28
	return;
}

//^29EE:0B2B
void SkWinCore::_29ee_0b2b()
{
	//^29EE:0B2B
	//^29EE:0B2F
	for (Bit16u si = 0; si < _4976_53a4; si++) {
		//^29EE:0B33
		DRAW_CMD_SLOT(si, 0);
		//^29EE:0B3C
	}
	//^29EE:0B43
	DRAW_PLAYER_ATTACK_DIR();
	//^29EE:0B47
}

//^29EE:1D03
void SkWinCore::_29ee_1d03(Bit16u xx)
{
	//^29EE:1D03
	ENTER(6);
	//^29EE:1D08
	Bit16u si = xx;
	//^29EE:1D0B
	_29ee_00a3(0);
	//^29EE:1D12
	if (glbChampionIndex > 0 && (glbMagicalMapFlags & 0x0800) != 0) {
		//^29EE:1D24
		si ^= glbMagicalMapFlags;
		si &= 15;
		//^29EE:1D2C
		Bit8u bp06[2];
		bp06[1] = 0;
		Bit16u bp02 = 0;
		//^29EE:1D35
		for (; bp02 < 4; bp02++) {
			//^29EE:1D37
			Bit8u bp03 = 0x49;
			//^29EE:1D3B
			if ((si & 0x0001) != 0) {
				//^29EE:1D41
				bp03++;
			}
			//^29EE:1D44
			si >>= 1;
			//^29EE:1D46
			DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_MAGICAL_MAPS, glbHoldedContainerType, bp03, &_4976_3f6c, bp02 +101, -1);
			//^29EE:1D65
			bp06[0] = bp02 +0x72;
			//^29EE:1D6D
			DRAW_SIMPLE_STR(&_4976_3f6c, bp02 +105, glbPaletteT16[COLOR_BLACK], glbPaletteT16[COLOR_GRAY] | 0x4000, bp06);
			//^29EE:1D99
		}
	}
	//^29EE:1DA2
	return;
}


//^19F0:124B
Bit16u SkWinCore::_19f0_124b(i16 *xx, i16 *yy, Bit16u ww, i16 ss, Bit16u tt)
{
	//^19F0:124B
	ENTER(8);
	//^19F0:1251
	Bit16u bp04 = 0;
	i16 di = -1;
	//^19F0:1259
	CHANGE_CURRENT_MAP_TO(ww);
	//^19F0:1262
	Bit16u si = GET_TILE_VALUE(*xx, *yy);
	//^19F0:1279
	Bit16u bp02 = si >> 5;
	//^19F0:1281
	if (bp02 == ttStairs) {
		//^19F0:1286
		if ((tt & 0x0100) == 0)
			//^19F0:128D
			//^19F0:13A4
			return di;

		//^19F0:1290
		if ((((si & 0x0004) != 0) ? ((ss == -1) ? 1 : 0) : ((ss == 1) ? 1 : 0)) == 0) {
			//^19F0:12B6
			//^19F0:13A4
			return di;
		}
	}
	//^19F0:12BB
	else if (bp02 != 2 || (tt & 0x0008) == 0 || (ss != -1) || (si & 0x0008) == 0 || (si & 0x0001) != 0) {
		//^19F0:12DA
		if ((si & 0x0002) == 0 || bp02 == 0 || bp02 == 7 || bp02 == 4)
			//^19F0:12FB
			//^19F0:13A4
			return di;

		//^19F0:12FE
		Actuator *bp08;
		if ((tt & 0x0100) == 0 || FIND_LADDER_AROUND(*xx, *yy, ss, &bp08) < 0) {
			//^19F0:1325
			if ((tt & 0x0010) == 0 || ss != -1)
				//^19F0:1330
				//^19F0:13A4
				return di;
			//^19F0:1332
			bp04 = 1;
		}
	}
	//^19F0:1337
	di = LOCATE_OTHER_LEVEL(ww, ss, xx, yy, NULL);
	//^19F0:1357
	if (di < 0 || bp04 == 0)
		//^19F0:135F
		//^19F0:13A4
		return di;
	//^19F0:1361
	CHANGE_CURRENT_MAP_TO(di);
	//^19F0:1368
	si = GET_TILE_VALUE(*xx, *yy);
	//^19F0:137F
	bp02 = si >> 5;
	//^19F0:1387
	if (bp02 == ttPit && (si & 0x0008) != 0 && (si & 0x0001) != 0)
		//^19F0:1398
		di = -1;
	//^19F0:139B
	CHANGE_CURRENT_MAP_TO(ww);
	//^19F0:13A4
	return di;
}

//^29EE:18EB
void SkWinCore::_29ee_18eb(Bit16u xx, Bit16u yy, Bit16u zz)
{
	//^29EE:18EB
	ENTER(0);
	//^29EE:18EE
	_4976_536a = _4976_5320 = _4976_5328 = xx;
	_4976_536c = _4976_5322 = _4976_532a = yy;
	_4976_5318 = zz;
	//^29EE:190C
	_4976_533c = _19f0_124b(&_4976_5328, &_4976_532a, _4976_5318, -1, 0x0110);
	//^29EE:1928
	_4976_533e = _19f0_124b(&_4976_5320, &_4976_5322, _4976_5318,  1, 0x0108);
	//^29EE:1944
	return;
}

//^0CEE:02DD
void SkWinCore::CALC_VECTOR_W_DIR(i16 dir, i16 xx, i16 yy, i16 *ss, i16 *tt)
{
	ATLASSERT(dir >= 0 && dir < 4);

	//^0CEE:02DD
	ENTER(0);
	//^0CEE:02E1
    i16 si = dir;
	//^0CEE:02E4
	*ss += glbXAxisDelta[si] * xx;
	//^0CEE:02F5
	*tt += glbYAxisDelta[si] * xx;
	//^0CEE:0306
	si = (si +1) & 3;
	//^0CEE:030B
	*ss += glbXAxisDelta[si] * yy;
	//^0CEE:031C
	*tt += glbYAxisDelta[si] * yy;
	//^0CEE:032D
    return;
}

//^0CEE:1962
Bit16u SkWinCore::QUERY_ORNATE_ANIM_FRAME(Bit8u cls1, Bit8u cls2, Bit32u tick, Bit32u delta)
{
	//0CEE:1962
	ENTER(136);
	//^0CEE:1968
	i16 si = 1;
	//^0CEE:196B
	Bit16u bp06 = 0;
	Bit16u bp08 = 0;
	//^0CEE:1973
	Bit16u di = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, dtWordValue, 0x0d);
	//^0CEE:1989
	Bit8u *bp04;
	Bit8u bp0088[128];
	if (di == 0) {
		//^0CEE:198D
		bp04 = QUERY_GDAT_TEXT(cls1, cls2, dt0d, bp0088);
		//^0CEE:19AF
		if (*bp04 != 0 && (di = SK_STRLEN(bp04)) != 0) {
			//^0CEE:19C8
			bp08 = 1;
		}
		else {
			//^0CEE:19CF
			di = 1;
		}
	}
	else {
		//^0CEE:19D4
		bp04 = NULL;
		bp08 = 1;
		//^0CEE:19E3
		if ((di & 0x8000) != 0) {
			//^0CEE:19E9
			bp06 = 1;
			di &= 0x7fff;
		}
	}
	//^0CEE:19F2
	if (bp08 != 0) {
		//^0CEE:19F8
		si = ((tick + delta) % di) +bp06;
		//^0CEE:1A13
		if (bp04 != NULL) {
			//^0CEE:1A1B
			si = bp04[si];
			//^0CEE:1A24
			if (si >= '0' && si <= '9') {
				//^0CEE:1A2E
				si -= '0';
			}
			//^0CEE:1A33
			if (si >= 'A' && si <= 'Z') {
				//^0CEE:1A3D
				si -= 'A' +10;
			}
		}
	}
	//^0CEE:1A40
	return si;
}


//^29EE:0C41
void SkWinCore::DRAW_CHIP_OF_MAGIC_MAP(const Bit8u *buff, Bit16u aa, Bit16u xx, Bit16u yy, Bit16u flipMirror, Bit8u *localpal)
{
	// draws:
	// a) parts of minion map's 7x7 atlas view.

	//^29EE:0C41
	ENTER(12);
	//^29EE:0C47
	Bit16u di = flipMirror;
	//^29EE:0C4A
	if (buff != NULL) {
		//^29EE:0C55
		i16 bp02;
		i16 bp04;
		SRECT bp0c;
		if (UNION_RECT(SET_SRECT(&bp0c, xx, yy, glbMagicMapWidth, glbMagicMapHeight), &_4976_53a6, &bp02, &bp04) != NULL) {
			//^29EE:0C8C
			i16 si = glbMagicMapWidth - bp0c.cx;
			//^29EE:0C94
			if (si != 0 && (di & 0x0001) != 0) {
				//^29EE:0C9E
				bp02 = (bp02 != 0) ? 0 : si;
			}
			//^29EE:0CAE
			si = glbMagicMapHeight - bp0c.cy;
			//^29EE:0CB6
			if (si != 0 && (di & 0x0002) != 0) {
				//^29EE:0CC0
				bp04 = (bp04 != 0) ? 0 : si;
			}
			//^29EE:0CD0
			DRAW_ICON_PICT_BUFF(
				buff,
				&_4976_3f6c,
				&bp0c,
				glbMagicMapWidth * aa +bp02,
					            +bp04,
				10,
				di,
				localpal
				);
			printf("");
		}
	}
	//^29EE:0CFD
	return;
}

//^4937:000F
i16 SkWinCore::_4937_000f(Bit16u xx, Bit16u *yy)
{
	//^4937:000F
	ENTER(0);
	//^4937:0012
	return tlbCreaturesAnimationSequences[_4937_005c(xx, yy)].w0 & 0x03ff;
}

//^0CEE:2DF4
Bit16u SkWinCore::_0cee_2df4(ObjectID recordLink)
{
	//^0CEE:2DF4
	ENTER(0);
	//^0CEE:2DF7
	return QUERY_CREATURE_AI_SPEC_FROM_RECORD(recordLink)->w30;
}

//^48AE:011A
Bit16u SkWinCore::_48ae_011a(ObjectID recordLink)
{
	//^48AE:011A
	ENTER(2);
	//^48AE:011F
	Bit8u bp01 = QUERY_CLS1_FROM_RECORD(recordLink);
	//^48AE:012B
	Bit8u bp02 = QUERY_CLS2_FROM_RECORD(recordLink);
	//^48AE:0137
	if (QUERY_GDAT_ENTRY_IF_LOADABLE(bp01, bp02, dtImage, 0x08) != 0) {
		//^48AE:014C
		if (QUERY_GDAT_ENTRY_IF_LOADABLE(bp01, bp02, dtImage, 0x0c) == 0) {
			//^48AE:0164
			return 3;
		}
		//^48AE:0169
		if (QUERY_GDAT_ENTRY_IF_LOADABLE(bp01, bp02, dtImage, 0x0a) != 0) {
			//^48AE:0181
			return 1;
		}
		//^48AE:0186
		if (QUERY_GDAT_ENTRY_IF_LOADABLE(bp01, bp02, dtImage, 0x09) != 0) {
			//^48AE:019E
			return 0;
		}
		//^48AE:01A2
		return 2;
	}
	//^48AE:01A7
	return 0xffff;
}

//^29EE:0D01
void SkWinCore::DRAW_MAP_CHIP(ObjectID recordLink, i16 ss, i16 tt, i16 ww, Bit16u xx, Bit16u yy, Bit8u cc, Bit16u flags)
{
	//^29EE:0D01
	ENTER(108);
	//^29EE:0D07
	Bit16u di = yy;
	//^29EE:0D0A
	Bit8u *bp2e = NULL;
	//^29EE:0D14
	ExtendedTileInfo bp4c;
	SUMMARIZE_STONE_ROOM(&bp4c, ww, ss, tt);
	//^29EE:0D2A
	Bit8u bp5c[16];
	QUERY_DUNGEON_MAP_CHIP_PICT(GDAT_CATEGORY_GRAPHICSSET, cc, &bp2e, bp5c);
	//^29EE:0D41
	if ((flags & 0x0010) != 0) {
		//^29EE:0D48
		SRECT bp3c;
		i16 bp34;
		UNION_RECT(
			SET_SRECT(
				&bp3c, 
				xx - glbMagicMapInterlineX, 
				di - glbMagicMapInterlineY, 
				(glbMagicMapInterlineX << 1) + glbMagicMapWidth, 
				(glbMagicMapInterlineY << 1) + glbMagicMapHeight
				),
			&_4976_53a6,
			&bp34,
			&bp34
		);
		//^29EE:0D91
		FILL_RECT_SUMMARY(&_4976_3f6c, &bp3c, glbPaletteT16[COLOR_RED]);
	}
	//^29EE:0DAD
	Bit16u bp14 = ((bp4c.w2 >> 5) == ttTrickWall && (bp4c.w2 & 1) != 0) ? 1 : 0;
	Bit16u bp20 = 0;
	i16 bp0a;
	Bit8u *bp2a;
	Bit8u bp6c[16];
	i16 bp0e;
	i16 bp0c;

	//^29EE:0DCD
_0dcd:
	switch (bp4c.w0) {
		case 0://^0DDF
		case 7://^0DDF
			//^29EE:0DDF
			{
				if ((flags & 0x0002) != 0 && bp14 != 0 && (Bit16u(glbGameTick) & 2) != 0) {
					//^29EE:0DF7
					bp0a = 1;
					//^29EE:0DFC
					goto _10b7;
				}
				//^29EE:0DFF
				if (bp20 == 0) {
					//^29EE:0E05
					DRAW_CHIP_OF_MAGIC_MAP(bp2e, 0, xx, di, 0, bp5c);
				}
				//^29EE:0E1F
				for (bp0a = 0; bp0a <= 3; bp0a++) {
					//^29EE:0E27
					bp0c = bp4c.w6[bp0a] & 0xff;
					//^29EE:0E3A
					if (bp0c != 0xff) {
						//^29EE:0E41
						if ((flags & 0x0002) != 0 || QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp0c, dtWordValue, GDAT_IMG_MAP_CHIP) == 0) {
							//^29EE:0E5F
							bp0c = QUERY_DUNGEON_MAP_CHIP_PICT(GDAT_CATEGORY_WALL_GFX, Bit8u(bp0c), &bp2a, bp6c);
							//^29EE:0E79
							if (bp0c != 0) {
								//^29EE:0E7D
								bp0e = bp4c.w6[bp0a] >> 10;
								//^29EE:0E90
								bp0c = (((bp0e +1) << 1) > bp0c) ? (bp0a & 1) : ((bp0e << 1) + (bp0a & 1));
								//^29EE:0EB1
								DRAW_CHIP_OF_MAGIC_MAP(bp2a, bp0c, xx, di, _4976_3fa0[bp0a], bp6c);
							}
						}
					}
					//^29EE:0ED4
				}
				//^29EE:0EE0
				break;
			}
		case 1://^0EE3
			{
				//^29EE:0EE3
				if ((bp4c.w2 & 0x0008) != 0) {
					//^29EE:0EEA
					QUERY_DUNGEON_MAP_CHIP_PICT(GDAT_CATEGORY_GRAPHICSSET, _4976_4c5c[dunMapLocalHeader->Level()], &bp2e, bp5c);
					//^29EE:0F12
					//^29EE:18AA
					DRAW_CHIP_OF_MAGIC_MAP(bp2e, 0, xx, di, 0, bp5c);
					//^29EE:18B7
					return;
				}

				goto _0f22;
			}
		case 5://^0F22
			{
				//^29EE:0F22
_0f22:
				DRAW_CHIP_OF_MAGIC_MAP(bp2e, 2, xx, di, 0, bp5c);
				//^29EE:0F3C
				bp0a = bp4c.w6[2] & 0xff;
				//^29EE:0F45
				if (bp0a != 0xff) {
					//^29EE:0F49
					if (true
						&& ((flags & 0x0002) != 0 || QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_FLOOR_GFX, bp0a, dtWordValue, 0xf9) == 0)
						&& (bp0c = QUERY_DUNGEON_MAP_CHIP_PICT(GDAT_CATEGORY_FLOOR_GFX, Bit8u(bp0a), &bp2a, bp6c)) != 0
					) {
						//^29EE:0F85
						bp0e = (bp4c.w6[2] >> 8) / 10;
						//^29EE:0F94
						bp0a = (bp0e +1 > bp0c) ? 0 : bp0e;
						//^29EE:0FA7
						DRAW_CHIP_OF_MAGIC_MAP(bp2a, bp0a, xx, di, 0, bp6c);
					}
				}
				//^29EE:0FC2
				if (bp4c.w0 == 5) {
					//^29EE:0FCB
					bp0a = QUERY_DUNGEON_MAP_CHIP_PICT(GDAT_CATEGORY_TELEPORTERS, 0x00, &bp2a, bp6c);
					//^29EE:0FE3
					if (bp0a != 0) {
						//^29EE:0FEA
						DRAW_CHIP_OF_MAGIC_MAP(bp2a, glbGameTick % bp0a, xx, di, 0, bp6c);
					}
				}	
				//^29EE:1016
				break;
			}
		case 16://^1019
			{
				//^29EE:1019
				bp0a = 11;
				//^29EE:101E
				goto _1025;
			}
		case 17://^1020	// DOOR TILE
			{
				//^29EE:1020
				bp0a = 10;
				//^29EE:1025
_1025:
				Door *bp04 = GET_ADDRESS_OF_RECORD0(bp4c.w6[1]);
				//^29EE:1034
				if (bp04->Button() == 0 && bp04->Bit13C() == 0) {
					//^29EE:1053
					bp0a += 2;
				}
				//^29EE:1057
				if (bp4c.w6[0] == _DOOR_STATE__OPENED_ || bp4c.w6[0] == _DOOR_STATE__DESTROYED_) {	// (bp4c.w6[0] == 0 || bp4c.w6[0] == 5) if door is opend or destroyed
					//^29EE:1063
					bp0a += 4;
				}
				//^29EE:1067
				goto _10b7;
			}
		case 18://^1069
		case 19://^1069
			{
				//^29EE:1069
				bp0a = (_0cee_06dc_GET_TILE_DIRECTION(ss, tt) - ww) & 3;
				//^29EE:107F
				if (bp4c.w6[0] != 0) {
					//^29EE:1085
					bp0a += 2;
					bp0a &= 3;
				}
				//^29EE:108E
				bp0a += 6;
				//^29EE:1092
				goto _10b7;
			}
		case 2://^1094
			{
				//^29EE:1094
				bp0a = 3;
				//^29EE:1099
				if (bp4c.w6[0] != 0) {
					//^29EE:109F
					bp0a++;
					//^29EE:10A2
					if ((flags & 0x0002) != 0) {
						//^29EE:10A9
						if ((Bit16u(glbGameTick) & 1) != 0) {
							//^29EE:10B4
							bp0a++;
						}
					}
				}
				//^29EE:10B7
_10b7:
				DRAW_CHIP_OF_MAGIC_MAP(bp2e, bp0a, xx, di, 0, bp5c);

				break;
			}
		case 3://^10D2
		case 4://^10D2
		case 6://^10D2
		case 8://^10D2
		case 9://^10D2
		case 10://^10D2
		case 11://^10D2
		case 12://^10D2
		case 13://^10D2
		case 14://^10D2
		case 15://^10D2
			{
				break;
			}

	}
	//^29EE:10D2
	if ((flags & 0x0010) != 0) {
		//^29EE:10DC
		for (bp0a = 0; bp0a < glbChampionsCount; bp0a++) {
			//^29EE:10E4
			if (glbChampionSquad[bp0a].curHP() != 0) {
				//^29EE:10F8
				Bit8u bp21 = QUERY_CLS1_FROM_RECORD(recordLink);
				Bit8u bp22 = QUERY_CLS2_FROM_RECORD(recordLink);
				//^29EE:1110
				Bit8u *bp32 = QUERY_GDAT_IMAGE_ENTRY_BUFF(bp21, bp22, 0x40);
				//^29EE:1125
				DRAW_CHIP_OF_MAGIC_MAP(
					bp32,
					(glbGlobalSpellEffects.Invisibility != 0) ? 0x09 : (bp0a +5),
					xx,
					di,
					_4976_3fa4[(glbChampionSquad[bp0a].playerPos() +4 -ww) & 3],
					QUERY_GDAT_IMAGE_LOCALPAL(bp21, bp22, 0x40)
					);
				printf("");
			}
			//^29EE:117F
		}
	}
	//^29EE:118E
	if ((flags & 0x0040) != 0) {
		//^29EE:1195
		bp0a = (_4976_5326 != 2) ? ((_4976_5326 != 3) ? 0x02 : 0x0a) : (0x0b);
		//^29EE:11B7
		Bit8u bp21 = QUERY_CLS1_FROM_RECORD(recordLink);
		Bit8u bp22 = QUERY_CLS2_FROM_RECORD(recordLink);
		//^29EE:11CF
		Bit8u *bp32 = QUERY_GDAT_IMAGE_ENTRY_BUFF(bp21, bp22, 0x40);
		//^29EE:11E4
		DRAW_CHIP_OF_MAGIC_MAP(
			bp32,
			bp0a,
			xx,
			di,
			0,
			QUERY_GDAT_IMAGE_LOCALPAL(bp21, bp22, 0x40)
			);
	}
	//^29EE:120E
	if ((flags & 0x00a5) != 0) {
		//^29EE:1218
		ObjectID bp1c = OBJECT_END_MARKER;
		ObjectID bp1a = OBJECT_END_MARKER;
		ObjectID bp18 = OBJECT_END_MARKER;
		//^29EE:1224
		ObjectID si = bp4c.w4;
		//^29EE:1227
		for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
			//^29EE:122A
			bp0a = si.DBType();
			//^29EE:1235
			if (bp0a == dbCreature) {
				//^29EE:123A
				Bit16u bp16 = QUERY_CREATURE_AI_SPEC_FLAGS(si);
				//^29EE:1244
				if (QUERY_GDAT_ENTRY_DATA_INDEX(0x0f, QUERY_CLS2_FROM_RECORD(si), dtWordValue, 0xf9) != 0) {
					//^29EE:125F
					bp18 = si;
				}
				else {
					//^29EE:1265
					if ((bp16 & 0x0001)	!= 0) {
						//^29EE:126C
						if ((flags & 0x0080) != 0) {
							//^29EE:1276
							bp18 = si;
						}
					}
					else {
						//^29EE:127C
						if ((flags & 0x0021) != 0) {
							//^29EE:1286
							bp18 = si;
						}
					}
				}
			}
			else {
				//^29EE:128C
				if ((flags & 0x0004) != 0) {
					//^29EE:1296
					if (bp0a == dbMissile) {
						//^29EE:129C
						if (bp1a == OBJECT_END_MARKER) {
							//^29EE:12A5
							bp1a = si;
						}
					}
					//^29EE:12AB
					else if (bp0a == dbCloud) {
						//^29EE:12B1
						if (bp1c == OBJECT_END_MARKER) {
							//^29EE:12BA
							bp1c = si;
						}
					}
					//^29EE:12C0
					else if (bp0a >= dbWeapon && bp0a <= dbMiscellaneous_item) {
						//^29EE:12D2
						bp0a = QUERY_DUNGEON_MAP_CHIP_PICT(QUERY_CLS1_FROM_RECORD(si), QUERY_CLS2_FROM_RECORD(si), &bp2a, bp6c);
						//^29EE:12F6
						if (bp0a != 0) {
							//^29EE:12FA
							bp0c = (si.Dir() - ww) & 3;
							//^29EE:1308
							if (bp4c.w0 == 0 && bp14 == 0) {
								//^29EE:1314
								if (bp0a <= 2)
									//^29EE:1318
									continue;
								//^29EE:131A
								bp0a = (bp0c & 1) +1;
								//^29EE:1324
								bp0c = _4976_3fa0[bp0c];
							}
							else {
								//^29EE:132D
								bp0a = 0;
								//^29EE:1332
								bp0c = _4976_3fa4[bp0c];
							}
							//^29EE:1339
							//^29EE:133E
							DRAW_CHIP_OF_MAGIC_MAP(bp2a, bp0a, xx, di, bp0c, bp6c);
						}
					}
				}
			}
			//^29EE:135A
		}
		//^29EE:136B
		if (bp18 != OBJECT_END_MARKER) {
			//^29EE:1374
			Creature *bp04 = GET_ADDRESS_OF_RECORD4(bp18);
			//^29EE:1383
			Bit16u bp16 = QUERY_CREATURE_AI_SPEC_FLAGS(bp18);
			//^29EE:138F
			Bit8u bp21;
			Bit8u bp22;
			Bit8u *bp32;
			if ((flags & 0x0020) != 0) {
				//^29EE:1396
				bp21 = QUERY_CLS1_FROM_RECORD(recordLink);
				//^29EE:13A2
				bp22 = QUERY_CLS2_FROM_RECORD(recordLink);
				//^29EE:13AE
				bp32 = QUERY_GDAT_IMAGE_ENTRY_BUFF(bp21, bp22, 0x40);
				//^29EE:13C3
				//^29EE:13E6
				//^29EE:1546
				DRAW_CHIP_OF_MAGIC_MAP(
					bp32, 
					(Bit16u(glbGameTick) & 1) +3, 
					xx, 
					di, 
					0, 
					QUERY_GDAT_IMAGE_LOCALPAL(bp21, bp22, 0x40)
					);

				goto _1554;
			}
			//^29EE:13E9
			bp0c = QUERY_DUNGEON_MAP_CHIP_PICT(QUERY_CLS1_FROM_RECORD(bp18), QUERY_CLS2_FROM_RECORD(bp18), &bp2a, bp6c);
			//^29EE:1411
			if (bp0c != 0) {
				//^29EE:1418
				if ((bp16 & 0x0001) != 0) {
					//^29EE:141F
					sk1c9a02c3 *bp08 = _1c9a_02c3(bp04, QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp04->CreatureType()));
					//^29EE:1443
					bp0a = _4937_000f(bp08->w0, &bp08->w2);
					//^29EE:145D
					bp0a = (bp0a >= 4 && bp0a <= 7) ? 4 : 0;
					//^29EE:1474
					if (((bp0a +1) << 1) > bp0c) {
						//^29EE:147F
						bp0a = 0;
					}
					else {
						//^29EE:1486
						bp0a = bp0a << 1;
					}
				}
				else {
					//^29EE:1490
					bp0a = i16(glbGameTick % (bp0c / 2)) << 1;
				}
				//^29EE:14AE
				bp0c = (ww - bp04->b15_0_1());
				//^29EE:14C4
				//^29EE:14E9
				//^29EE:154D
				DRAW_CHIP_OF_MAGIC_MAP(bp2a, (bp0c & 1) + bp0a, xx, di, _4976_3fa0[bp0c], bp6c);
			}
			//^29EE:14EB
			else if ((bp16 & 0x0001) == 0 && (bp16 & 0x2000) == 0) {
				//^29EE:14F9
				bp21 = QUERY_CLS1_FROM_RECORD(recordLink);
				//^29EE:1505
				bp22 = QUERY_CLS2_FROM_RECORD(recordLink);
				//^29EE:1511
				bp32 = QUERY_GDAT_IMAGE_ENTRY_BUFF(bp21, bp22, 0x40);
				//^29EE:1526
				DRAW_CHIP_OF_MAGIC_MAP(
					bp32, 
					Bit16u(glbGameTick & 1), 
					xx, 
					di, 
					0, 
					QUERY_GDAT_IMAGE_LOCALPAL(bp21, bp22, 0x40)
					);
			}
			//^29EE:1554
_1554:
			if ((bp16 & 0x0001) != 0 && (flags & 0x0004) != 0) {
				//^29EE:1568
				if ((_0cee_2df4(bp18) & 0x000f) != 0) {
					//^29EE:1576
					for (si = bp04->w0; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
						//^29EE:157E
						bp0a = si.DBType();
						//^29EE:1589
						if (bp0a >= dbWeapon && bp0a <= dbMiscellaneous_item) {
							//^29EE:1594
							if (QUERY_DUNGEON_MAP_CHIP_PICT(
									QUERY_CLS1_FROM_RECORD(si), 
									QUERY_CLS2_FROM_RECORD(si), 
									&bp2a, bp6c) != 0
							) {
								//^29EE:15B9
								DRAW_CHIP_OF_MAGIC_MAP(
									bp2a,
									0,
									xx,
									di,
									_4976_3fa4[(si.Dir() - ww) & 3],
									bp6c
									);
							}
						}
						//^29EE:15E4
					}
				}
			}
		}
		//^29EE:15F2
		for (si = bp1a; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
			//^29EE:15F8
			if (si.DBType() == dbMissile) {
				//^29EE:1608
				bp0a = QUERY_DUNGEON_MAP_CHIP_PICT(
							QUERY_CLS1_FROM_RECORD(si), 
							QUERY_CLS2_FROM_RECORD(si), 
							&bp2a, bp6c);
				//^29EE:162C
				if (bp0a != 0) {
					//^29EE:1633
					if (bp0a > 3) {
						//^29EE:163B
						bp0c = glbTimersTable[GET_ADDRESS_OF_RECORDE(si)->TimerIndex()].Direction();
						//^29EE:1662
						bp0c = (bp0c - ww) & 3;
						//^29EE:166B
						bp0a = 3;
						//^29EE:1670
						switch (_48ae_011a(si) +1) {
							case 0:
								//^29EE:1686
								//^29EE:16B0
								bp0a = 0;
								break;
							case 1:
								//^29EE:16AE
								//^29EE:16B0
								bp0a = 0;
								break;
							case 2:
								//^29EE:1688
								bp0a += bp0c;
								//^29EE:168E
								bp0a += _4976_3fa8[((bp0a -3) << 2) + ((si.Dir() - ww) & 3)];
								//^29EE:16AC
								break;
							case 3:
								//^29EE:16B0
								bp0a = 0;
								break;
						}
					}
					else {
						//^29EE:16B0
						bp0a = 0;
					}
					//^29EE:16B5
					DRAW_CHIP_OF_MAGIC_MAP(
						bp2a, 
						bp0a, 
						xx,
						di,
						_4976_3fa4[(si.Dir() - ww) & 3],
						bp6c
						);
				}
			}
			//^29EE:16E1
		}
		//^29EE:16F2
		for (si = bp1c; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
			//^29EE:16F7
			if (true
				&& si.DBType() == dbCloud
				&& QUERY_DUNGEON_MAP_CHIP_PICT(
					QUERY_CLS1_FROM_RECORD(si), 
					QUERY_CLS2_FROM_RECORD(si), 
					&bp2a, bp6c) != 0
			) {
				//^29EE:1729
				DRAW_CHIP_OF_MAGIC_MAP(
					bp2a,
					Bit16u(glbGameTick & 1) +1,
					xx,
					di,
					RAND02(),
					bp6c
					);
			}
			//^29EE:174D
		}
		//^29EE:175B
		Bit16u bp1e;
		i16 bp10;
		i16 bp12;
		if (bp4c.w0 == 0 && bp20 == 0) {
			//^29EE:176D
			bp10 = ss;
			bp12 = tt;
			Bit8u *bp26 = NULL;
			//^29EE:1783
			bp0a = glbCurrentMapIndex;
			//^29EE:1786
			while (bp0a == glbCurrentMapIndex && bp0a != -1) {
				//^29EE:1788
				bp0a = LOCATE_OTHER_LEVEL(glbCurrentMapIndex, 0, &ss, &tt, &bp26);
				//^29EE:17A5
			}
			//^29EE:17B7
			if (bp0a >= 0 && bp0a != glbCurrentMapIndex) {
				//^29EE:17C6
				if ((glbMapTileValue[bp0a][ss][tt] >> 5) == 0) {
					//^29EE:17EC
					bp20 = 1;
					bp1e = glbCurrentMapIndex;
					//^29EE:17F7
					CHANGE_CURRENT_MAP_TO(bp0a);
					//^29EE:1800
					SUMMARIZE_STONE_ROOM(&bp4c, ww, ss, tt);
					//^29EE:1816
					goto _0dcd;
				}
			}
		}
		//^29EE:1819
		if (bp20 != 0) {
			//^29EE:181F
			ss = bp10;
			tt = bp12;
			//^29EE:182B
			CHANGE_CURRENT_MAP_TO(bp1e);
			//^29EE:1834
			SUMMARIZE_STONE_ROOM(&bp4c, ww, ss, tt);
		}
		//^29EE:184A
		bp0a = LOCATE_OTHER_LEVEL(glbCurrentMapIndex, -1, &ss, &tt, NULL);
		//^29EE:1869
		if (bp0a >= 0) {
			//^29EE:186D
			bp0a = glbMapTileValue[bp0a][ss][tt];
			//^29EE:188F
			if ((bp0a >> 5) == ttPit && (bp0a & 8) != 0) {
				//^29EE:189D
				DRAW_CHIP_OF_MAGIC_MAP(bp2e, 18, xx, di, 0, bp5c);
			}
		}
	}
	//^29EE:18B7
	return;
}

//^29EE:1946
void SkWinCore::_29ee_1946(ObjectID recordLink, i16 xx, i16 yy, i16 zz, i16 dir, i16 ss, i16 tt, Bit16u flags)
{
	//^29EE:1946
    ENTER(56);
	//^29EE:194C
	Bit16u si = flags;
	//^29EE:194F
	if ((si & 0x0008) != 0) { // 8=fix position
		//^29EE:1955
		xx = _4976_5364;
		yy = _4976_5365;
		zz = _4976_539e;
		dir = _4976_539f;
		ss = _4976_53a0;
		tt = _4976_53a2;
	}
	else {
		//^29EE:197F
		_4976_5364 = (i8)xx;
		_4976_5365 = (i8)yy;
		_4976_539e = (i8)zz;
		_4976_539f = (i8)dir;
		_4976_53a0 = ss;
		_4976_53a2 = tt;
	}
	//^29EE:19A3
	_4976_5326 = 0;
	//^29EE:19A9
	Bit16u bp18 = glbCurrentMapIndex;
	//^29EE:19AF
	_29ee_00a3(0);
	//^29EE:19B6
	FILL_RECT_SUMMARY(&_4976_3f6c, QUERY_EXPANDED_RECT(99, &_4976_53a6), glbPaletteT16[COLOR_BLACK]);
	//^29EE:19DC
	INFLATE_RECT(&_4976_53a6, - glbMagicMapInterlineX, - glbMagicMapInterlineY);
	//^29EE:19F4
	Bit16u bp2e = _4976_53a6.x + ss;
	Bit16u bp30 = _4976_53a6.y + tt;
	Bit16u bp2a = si & 0x0010;
	Bit16u bp1a = si & 0x0020;
	Bit16u bp28 = si & 0x0040;
	//^29EE:1A1E
	Bit16u bp22;
	Bit16u bp24;
	Bit16u bp26;
	Bit16u bp1c;
	Bit16u bp1e;
	Bit16u bp20;
	if (bp1a != 0 || bp28 != 0) {
		//^29EE:1A2D
		Container *bp04 = GET_ADDRESS_OF_RECORD9(recordLink);
		//^29EE:1A3C
		if (bp1a != 0) {
			//^29EE:1A42
			Missile *bp08 = GET_MISSILE_REF_OF_MINION(bp04->GetContainedObject(), recordLink);
			//^29EE:1A59
			bp1c = bp08->GetX();
			bp1e = bp08->GetY();
			bp20 = bp08->GetMap();
			//^29EE:1A80
			CHANGE_CURRENT_MAP_TO(bp20);
			//^29EE:1A87
			TELE_inf bp3a;
			if (GET_TELEPORTER_DETAIL(&bp3a, (Bit8u)bp1c, (Bit8u)bp1e) != 0) {
				//^29EE:1A9E
				bp22 = bp3a.target_x_pos();
				bp24 = bp3a.target_x_pos();
				bp26 = bp3a.target_z();
			}
			else {
				//^29EE:1AB8
				bp26 = 0xffff;
			}
		}
		//^29EE:1ABD
		if (bp28 != 0) {
			//^29EE:1AC3
			_29ee_18eb(bp04->GetDestX(), bp04->GetDestY(), bp04->GetDestMap());
		}
	}
	//^29EE:1AEB
	CHANGE_CURRENT_MAP_TO(zz);
	//^29EE:1AF4
	i16 bp10 = 3;
	i16 bp12 = -3;
	//^29EE:1AFE
	if (tt < 0) {
		//^29EE:1B04
		bp10++;
	}
	//^29EE:1B07
	i16 bp14 = -3;
	i16 bp16 = 3;
	//^29EE:1B11
	if (ss < 0) {
		//^29EE:1B11
		bp14--;
	}
	//^29EE:1B1A
	for (i16 bp0c = bp10; bp0c >= bp12; bp0c--) {
		//^29EE:1B23
		Bit16u bp0a = bp2e;
		//^29EE:1B29
		for (i16 bp0e = bp14; bp0e <= bp16; bp0e++) {
			//^29EE:1B32
			i16 bp32 = xx;
			i16 bp34 = yy;
			//^29EE:1B3E
			CALC_VECTOR_W_DIR(dir, bp0c, bp0e, &bp32, &bp34);
			//^29EE:1B59
			i16 di = LOCATE_OTHER_LEVEL(zz, 0, &bp32, &bp34, NULL);
			//^29EE:1B76
			Bit8u bp2b;
			if (di < 0) {
				//^29EE:1B7A
				di = zz;
				//^29EE:1B7F
				CHANGE_CURRENT_MAP_TO(di);
				//^29EE:1B86
				if ((GET_TILE_VALUE(bp32, bp34) >> 5) == ttMapExit) {
					//^29EE:1B9C
					bp2b = _4976_4c5c[dunMapLocalHeader->Level()];
					//^29EE:1BB0
					goto _1bc7;
				}
			}
			else {
				//^29EE:1BB2
				CHANGE_CURRENT_MAP_TO(di);
			}
			//^29EE:1BB9
			bp2b = dunMapLocalHeader->MapGraphicsStyle();
			//^29EE:1BC7
_1bc7:
			si &= 0xff8f;
			//^29EE:1BCE
			if (true
				&& bp1a != 0
				&& (false
					|| (bp32 == bp1c && bp34 == bp1e &&   di == bp20)
					|| (  di == bp26 && bp32 == bp22 && bp34 == bp24)
				)
			) {
				//^29EE:1BFE
				si |= 0x0020;
			}
			//^29EE:1C02
			if (bp28 != 0 && _4976_5326 == 0) {
				//^29EE:1C0F
				if (bp32 == _4976_536a && bp34 == _4976_536c && di == _4976_5318) {
					//^29EE:1C27
					_4976_5326 = 1;
					//^29EE:1C2D
				}
				//^29EE:1C2F
				else if (bp32 == _4976_5328 && bp34 == _4976_532a && di == _4976_533c) {
					//^29EE:1C47
					_4976_5326 = 2;
					//^29EE:1C4D
					goto _1c6d;
				}
				//^29EE:1C4F
				else if (bp32 == _4976_5320 && bp34 == _4976_5322 && di == _4976_533e) {
					//^29EE:1C67
					_4976_5326 = 3;
					//^29EE:1C6D
_1c6d:
					_29ee_18eb(bp32, bp34, di);
				}
				//^29EE:1C7B
				if (_4976_5326 != 0) {
					//^29EE:1C82
					si |= 0x0040;
				}
			}
			//^29EE:1C86
			if (bp2a != 0 && di == glbMap_4c28 && bp32 == glbSomePosX_4c2e && bp34 == glbSomePosY_4c30) {
				//^29EE:1CA4
				si |= 0x0010;
			}
			//^29EE:1CA8
			DRAW_MAP_CHIP(recordLink, bp32, bp34, dir, bp0a, bp30, bp2b, si);
			//^29EE:1CC6
			bp0a += glbMagicMapWidth + glbMagicMapInterlineX;
			//^29EE:1CD0
		}
		//^29EE:1CDE
		bp30 += glbMagicMapHeight + glbMagicMapInterlineY;
		//^29EE:1CE8
	}
	//^29EE:1CF6
	CHANGE_CURRENT_MAP_TO(bp18);
	//^29EE:1CFF
	return;
}

//^29EE:1DA5
void SkWinCore::DRAW_MAJIC_MAP(ObjectID recordLink)
{
	//^29EE:1DA5
	ENTER(30);
	//^29EE:1DAB
	glbHoldedContainerType = QUERY_CLS2_FROM_RECORD(recordLink);
	glbMagicalMapFlags |= 0x0090;
	//^29EE:1DBD
	Container *bp04 = GET_ADDRESS_OF_RECORD9(recordLink);
	//^29EE:1DCC
	Bit16u bp0e = bp04->b5_5_7();
	//^29EE:1DDC
	Missile *bp08;
	if (bp0e != 3) {
		//^29EE:1DE1
		glbMagicalMapFlags |= 0x0800;
		//^29EE:1DE7
		bp0e = bp04->b5_5_7();
		//^29EE:1DF4
		if (bp0e != 0) {
			//^29EE:1DF8
			bp08 = GET_MISSILE_REF_OF_MINION(bp04->GetContainedObject(), recordLink);
			//^29EE:1E0C
			if (bp08 != NULL) {
				//^29EE:1E10
				glbMagicalMapFlags |= 0x0020;
			}
			//^29EE:1E18
			else if ((glbMagicalMapFlags & 0x0020) != 0) {
				//^29EE:1E20
				glbMagicalMapFlags &= 0xfbdf;
			}
		}
	}
	//^29EE:1E26
	if ((glbMagicalMapFlags & 0x0400) == 0) {
		//^29EE:1E2E
		DRAW_ICON_PICT_ENTRY(0x14, glbHoldedContainerType, 0x10, &_4976_3f6c, 92, -1);
		//^29EE:1E46
		for (Bit16u si = 0; si < _4976_53a4; si++) {
			//^29EE:1E4A
			DRAW_CMD_SLOT(si, 0);
			//^29EE:1E53
		}
		//^29EE:1E5A
		_29ee_1d03(0);
		//^29EE:1E61
		glbMagicalMapFlags |= 0x0400;
	}
	//^29EE:1E67
	Bit16u bp10 = glbPlayerPosX;
	Bit16u di = glbPlayerPosY;
	Bit16u bp12 = glbPlayerMap;
	//^29EE:1E77
	i16 bp16 = 0; // shift-y
	i16 bp14 = 0; // shift-x
	//^29EE:1E7F
	if (bp0e != 0 && bp0e != 3) {
		//^29EE:1E91
		if (bp04->GetDestMap() != MAXMAPS -1) {
			//^29EE:1EA3
			glbMagicalMapFlags |= 0x0040;
		}
		//^29EE:1EA9
		if (bp0e == 1) {
			//^29EE:1EB2
			if (bp08 == NULL) {
				//^29EE:1EBA
				glbMagicalMapFlags &= 0xffbf;
			}
			else {
				//^29EE:1EC3
				bp10 = bp08->GetX();
				di = bp08->GetY();
				bp12 = bp08->GetMap();
				//^29EE:1EE9
				Creature *bp0c = GET_ADDRESS_OF_RECORD4(bp04->GetContainedObject());
				//^29EE:1EFC
				i16 si = QUERY_CREATURE_5x5_POS(bp0c, (glbPlayerDir + _4976_3fc2[bp0c->b15_0_1()]) & 3);
				//^29EE:1F28
				bp14 = si % 5;
				//^29EE:1F31
				bp16 = si / 5;
				//^29EE:1F39
				if (bp14 > 2) {
					//^29EE:1F3F
					si = (glbPlayerDir +1) & 3;
					//^29EE:1F48
					bp10 += glbXAxisDelta[si];
					//^29EE:1F53
					di += glbYAxisDelta[si];
				}
				//^29EE:1F5B
				if (bp16 > 2) {
					//^29EE:1F61
					si = (glbPlayerDir +2) & 3;
					//^29EE:1F6B
					bp10 += glbXAxisDelta[si];
					//^29EE:1F76
					di += glbYAxisDelta[si];
				}
				//^29EE:1F7E
				bp14 = _4976_3fb8[bp14] * (((glbMagicMapInterlineX << 1) + glbMagicMapWidth +1) / 5);
				//^29EE:1FA2
				bp16 = _4976_3fb8[bp16] * (((glbMagicMapInterlineY << 1) + glbMagicMapHeight +1) / 5);
			}
		}
		//^29EE:1FC8
		else if ((glbMagicalMapFlags & 0x0040) != 0) {
			//^29EE:1FD0
			bp10 = bp04->GetDestX();
			//^29EE:1FDD
			di = bp04->GetDestY();
			//^29EE:1FE9
			bp12 = bp04->GetDestMap();
		}
	}
	//^29EE:1FF6
	_29ee_1946(recordLink, bp10, di, bp12, glbPlayerDir, bp14, bp16, glbMagicalMapFlags);
	//^29EE:2015
	if ((glbMagicalMapFlags & 0x0100) != 0 || (glbMagicalMapFlags & 0x0200) == 0) {
		//^29EE:2025
		SRECT bp1e;
		DRAW_GRAY_OVERLAY(&_4976_3f6c, QUERY_EXPANDED_RECT(99, &bp1e), 0);
	}
	//^29EE:2044
	return;
}

//^29EE:078B
void SkWinCore::DRAW_PLAYER_ATTACK_DIR()
{
	//^29EE:078B
	ENTER(346);
	//^29EE:0791
	_29ee_00a3(0);
	//^29EE:0798
	DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_GRAPHICSSET, glbSquadInterfaceMapGfxSet, GDAT_GFXSET_SQUAD_SINGLE, &_4976_3f6c, 93, -1);	// gfx = 0xF6
	//^29EE:07B1
	Bit8u *bp04 = ALLOC_PICT_BUFF(_4976_0118, _4976_011a, afDefault, 4);
	//^29EE:07CD
	i16 bp06 = 0;
	i16 bp08 = 0;
	//^29EE:07D3
	SRECT bp10;
	if (QUERY_BLIT_RECT(bp04, &bp10, 94, &bp06, &bp08, -1) != 0) {
		//^29EE:07FB
		if (glbGlobalSpellEffects.AuraOfSpeed != 0) {
			//^29EE:0802
			Bit16u si = RAND02();
			//^29EE:0809
			if (si != 0) {
				//^29EE:080D
				bp10.y -= 2;
				//^29EE:0813
				si = RAND02();
				//^29EE:081A
				if (si != 0) {
					//^29EE:081E
					bp10.x -= 2;
				}
			}
		}
		//^29EE:0824
		Bit8u bp015a[16];
		Bit16u di;
		QUERY_GDAT_SQUAD_ICON(bp04, Bit8u(di = glbChampionIndex -1), bp015a);
		//^29EE:083F
		DRAW_ICON_PICT_BUFF(bp04, &_4976_3f6c, &bp10, bp06, bp08, 12, 0, bp015a);
		//^29EE:0866
		if (glbChampionSquad[di].enchantmentPower != 0) {
			//^29EE:0879
			ExtendedPicture bp014a;
			QUERY_PICST_IT(QUERY_GDAT_SUMMARY_IMAGE(&bp014a, 0x01, 0x07, glbChampionSquad[di].enchantmentAura));
			//^29EE:08A2
			bp014a.w4 |= 0x0010;
			bp014a.width >>= 2;
			//^29EE:08AD
			bp014a.w14 = bp014a.width * ((glbChampionSquad[di].playerDir() +4 - glbPlayerDir) & 3);
			//^29EE:08D4
			bp014a.height >>= 2;
			//^29EE:08D9
			bp014a.w16 = bp014a.height * (Bit16u(glbGameTick) & 3);
			//^29EE:08EB
			_0b36_11c0(&bp014a, &_4976_3f6c, 94, 12);
		}
	}
	//^29EE:0901
	FREE_PICT_BUFF(bp04);
	//^29EE:090E
	DRAW_ICON_PICT_ENTRY(0x01, 0x04, 0x10, &_4976_3f6c, 96, -1);
	//^29EE:0924
	DRAW_ICON_PICT_ENTRY(0x01, 0x04, 0x12, &_4976_3f6c, 97, -1);
	//^29EE:093A
	return;
}

//^3E74:58A3
void SkWinCore::FREE_TEMP_CACHE_INDEX(Bit16u cacheIndex)
{
	//^3E74:58A3
	ENTER(0);
	//^3E74:58A6
	_3e74_585a(cacheIndex, 0);
	//^3E74:58B1
	FREE_INDEXED_MEMENT(cacheIndex |0x8000);
	//^3E74:58BD
	return;
}

//^0B36:0CBE
void SkWinCore::_0b36_0cbe(sk3f6c *ref, Bit16u yy)
{
	//^0B36:0CBE
	ENTER(4);
	//^0B36:0CC3
	if (ref->w10 != 0) {
		//^	0B36:0CCD
		SRECT *bp04 = ref->w12;
		//^0B36:0CDC
		FIRE_HIDE_MOUSE_CURSOR();

		do {
			//^0B36:0CE1
			FIRE_BLIT_PICTURE(
				QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(ref->w0),
				_4976_4964,
				bp04,
				bp04->x - ref->rc2.x,
				bp04->y - ref->rc2.y,
				ref->rc2.cx,
				glbScreenWidth,
				-1,
				0,
				8,
				8,
				NULL
				);
			//^0B36:0D38
			bp04++;
			//^0B36:0D3C
		} while (--ref->w10 != 0);
		//^0B36:0D45
		FIRE_SHOW_MOUSE_CURSOR();
	}
	//^0B36:0D4A
	if (yy != 0) {
		//^0B36:0D50
		FREE_TEMP_CACHE_INDEX(ref->w0);
		//^0B36:0D5C
		ref->w0 = 0xffff;
	}
	//^0B36:0D64
	return;
}



//^2759:0644
//SPX: _2759_0644 renamed UPDATE_RIGHT_PANEL
void SkWinCore::UPDATE_RIGHT_PANEL(Bit16u xx)
{
	//^2759:0644
	//^2759:064A
	Bit16u bp0a = 0;
	Bit16u bp0c = 0;
	Bit16u bp0e = 0;
	Bit16u bp12 = 0;
	//^2759:065E
	if (glbNextChampionNumber == 0) {
		//^2759:0668
		if (glbChampionsCount == 0) {
			//^2759:066F
			if (_4976_531c != 0 || glbSomeChampionPanelFlag != 0) {
				//^2759:067D
				glbSomeChampionPanelFlag = 0;
				_4976_531c = 0;
				//^2759:0685
				_29ee_00a3(1);
			}
			//^2759:068D
			glbRightPanelType = RIGHT_PANEL_UNDEFINED;
			goto _0e03;
		}
		//^2759:0696
		if (xx != 0) {
			//^2759:069F
			for (Bit16u bp06=0; bp06 < glbChampionsCount; bp06++) {
				//^2759:06A6
				// SPX: ??BUG?? The b42 is table of 2 elements only; shouldn't it be si < 2 ??
				for (Bit16u si=0; si <= 2; si++) {
					//^2759:06AA
					if (glbChampionSquad[bp06].handCooldown[si] != 0) {
						//^2759:06BB
						glbChampionSquad[bp06].handCooldown[si] = glbChampionSquad[bp06].handCooldown[si] -1;
						//^2759:06DB
						if (glbChampionSquad[bp06].handCooldown[si] == 0) {
							//^2759:06DF
							LOAD_PROJECTILE_TO_HAND(bp06, si);
						}
					}
					//^2759:06EA
				}
				//^2759:06F0
			}
			//^2759:06FC
			if (glbRightPanelType == RIGHT_PANEL_MAGIC_MAP) {	// 3
				//^2759:0703
				ATLASSERT(glbChampionIndex < glbChampionsCount +1);
				Champion *champion = &glbChampionTable[glbChampionIndex];	//*bp04
				//^2759:0714
				if ((glbMagicalMapFlags & 0x0200) != 0) {
					//^2759:071C
					if (--_4976_5316 == 0) {
						//^2759:0722
						if (champion->manaCurrent != 0) {
							//^2759:072C
							champion->manaCurrent--;
							//^2759:0730
							champion->heroFlag |= CHAMPION_FLAG_0800;
							//^2759:0736
							_4976_5316 = 25 / _4976_531a;
						}
						else {
							//^2759:0743
							glbSomeChampionPanelFlag = 1;
							glbMagicalMapFlags = 0;
						}
					}
				}
				//^2759:0751
				else if (champion->manaCurrent != 0) {
					//^2759:075B
					glbSomeChampionPanelFlag = 1;
					glbMagicalMapFlags = 0;
				}
			}
		}
		//^2759:0767
		if (glbChampionAttackDamage != 0) {
			//^2759:076E
			SHOW_ATTACK_RESULT(glbChampionAttackDamage);
			//^2759:0778
			_4976_531c = 1;
			glbChampionAttackDamage = 0;
			glbRightPanelType = RIGHT_PANEL_ATTACK_RESULT;	// 6
			goto _0e03;
		}
		//^2759:078D
		if (glbChampionIndex > 0) {
			//^2759:0794
			i16 bp06 = glbChampionIndex -1;
			//^2759:079B
			if (bp06 >= 0 && glbSelectedHandAction != 2) {
				//^2759:07A9
				if (_4976_3de6[RCJ(4,bp06)][RCJ(2,glbSelectedHandAction)] != glbChampionSquad[bp06].Possess(glbSelectedHandAction)) {
					//^2759:07D4
					DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
				}
			}
		}
		//^2759:07D8
		Bit8u bp1c[4];
		if (glbChampionIndex >= 0) {
			//^2759:07E2
			Bit16u bp06;
			for (bp06=0; bp06 < glbChampionsCount; bp06++) {
				//^2759:07EA
				Champion *bp04 = &glbChampionSquad[bp06];
				//^2759:07FB
				bp1c[bp06] = 0;
				//^2759:0807
				Bit8u bp07 = (bp04->playerDir() +4 - glbPlayerDir) & 3;
				//^2759:0819
				i8 bp08 = (bp04->playerPos() +4 - glbPlayerDir) & 3;
				//^2759:0828
				if (bp04->curHP() != 0 && bp08 +1 == _4976_5dbc) {
					//^2759:0837
					bp08 = -1;
				}
				//^2759:083B
				if (READ_UI8(_4976_3de2, bp06) != bp08 || _4976_3dde[RCJ(4,bp06)] != bp07) {
					//^2759:0850
					bp0c = 1;
					bp12 = 1;
					//^2759:0859
					if (bp08 != -1) {
						//^2759:085F
						_4976_531c = 1;
					}
					//^2759:0865
					WRITE_UI8(_4976_3de2, bp06, bp08);
					_4976_3dde[RCJ(4,bp06)] = bp07;
				}
				//^2759:0876
			}
			//^2759:0885
			if (glbSquadInterfaceMapGfxSet != glbMapGraphicsSet || glbGlobalSpellEffects.AuraOfSpeed != 0 || (!_4976_532c) != (!glbGlobalSpellEffects.Invisibility)) {
				//^2759:08AC
				bp0c = 1;
				glbSquadInterfaceMapGfxSet = glbMapGraphicsSet;
				_4976_532c = (glbGlobalSpellEffects.Invisibility != 0) ? 1: 0;
			}
		}
		//^2759:08C8
		if (_4976_531c != 0) {
			//^2759:08CF
			if (glbSomeChampionPanelFlag > 1) {
				//^2759:08D6
				//^2759:0BE2
				glbSomeChampionPanelFlag--;
				//^2759:0BE6
				goto _0e03;
			}
			//^2759:08D9
			_29ee_00a3(1);
			//^2759:08E1
			_4976_531c = 0;
			//^2759:08E7
			bp0a = 1;
		}
		//^2759:08EC
		if (glbChampionIndex == 0) {
			//^2759:08F6
			glbRightPanelType = 0;
			//^2759:08FC
			if (bp0a == 0 && bp12 != 0) {
				//^2759:0908
				glbPreviousRightPanelType = 0xffff;
			}
			//^2759:090E
			for (Bit16u bp06=0; bp06 < glbChampionsCount; bp06++) {
				//^2759:0916
				Champion *champion = &glbChampionSquad[bp06];
				//^2759:0927
				if (champion->curHP() == 0 && GET_PLAYER_AT_POSITION(champion->playerPos()) >= 0) {
					//^2759:0942
					continue;
				}
				//^2759:0945
				bp0e = bp1c[bp06];
				//^2759:0955
				if (glbChampionAlive[bp06] != ((champion->curHP() > 0) ? 1 : 0)) {
					//^2759:0973
					glbChampionAlive[bp06] = ((champion->curHP() > 0) ? 1 : 0);
					//^2759:098D
					bp0e = 1;
					glbPreviousRightPanelType = 0xffff;
				}
				//^2759:0998
				Bit16u bp16 = ((ABS16(glbSomeChampionIndex) -1 == bp06) ? 1 : 0);
				//^2759:09B2
				//SPX: 0 to 1 = hands
				for (Bit16u si=0; si <= 1; si++) {
					//^2759:09B7
					Bit16u bp14 = 0;
					Bit16u bp10 = 0;
					//^2759:09BF
					if (bp16 != 0 && glbSelectedHand_2 == si) {
						//^2759:09CB
						if (glbSomeChampionIndex > 0) {
							//^2759:09D2
							bp14 = 1;
						}
						//^2759:09D7
						bp10 = 1;
					}
					//^2759:09DC
					Bit8u bp17 = (champion->handCooldown[si] != 0) ? 1 : 0;
					//^2759:09F0
					if (glbChampionHandCoolingDown[bp06][si] != bp17) {
						//^2759:0A03
						glbChampionHandCoolingDown[bp06][si] = bp17;
						//^2759:0A14
						bp10 = 1;
					}
					//^2759:0A19
					if (_4976_3de6[RCJ(4,bp06)][RCJ(2,si)] != champion->Possess(si) || bp0a != 0 || bp0e != 0) {
						//^2759:0A45
						_4976_3de6[RCJ(4,bp06)][RCJ(2,si)] = champion->Possess(si);
						//^2759:0A68
						bp10 = 1;
					}
					//^2759:0A6D
					if (bp0a != 0 || bp0e != 0 || bp10 != 0) {
						//^2759:0A7F
						DRAW_HAND_ACTION_ICONS(bp06, si, bp14);
					}
					//^2759:0A8E
				}
				//^2759:0A97
				Bit16u bp14 = 0;
				//^2759:0A9C
				// SPX: == 2 => spell selected instead of hand
				if (bp16 != 0 && glbSelectedHand_2 == 2) {
					//^2759:0AA9
					if (glbSomeChampionIndex > 0) {
						//^2759:0AB0
						bp14 = 1;
					}
					//^2759:0AB5
					bp0e = 1;
				}
				//^2759:0ABA
				Bit8u bp17 = (champion->herob44 != 0) ? 1 : 0;
				//^2759:0ACE
				if (glbChampionHandCoolingDown[bp06][2] != bp17) {
					//^2759:0AE1
					glbChampionHandCoolingDown[bp06][2] = bp17;
					//^2759:0AF2
					bp0e = 1;
				}
				//^2759:0AF7
				if (bp0a != 0 || bp0e != 0 || (_4976_3df7 != glbChampionLeader && (_4976_3df7 == bp06 || glbChampionLeader == bp06))) {
					//^2759:0B19
					DRAW_SQUAD_SPELL_AND_LEADER_ICON(bp06, bp14);
				}
				//^2759:0B26
				if (glbChampionSquad[bp06].enchantmentPower != 0 || _4976_3f68 != 0) {
					//^2759:0B3E
					bp0c = 1;
				}
				//^2759:0B43
				if (glbChampionSquad[bp06].enchantmentPower != 0) {
					//^2759:0B54
					_4976_3f68 |= 1 << bp06;
				}
				else {
					//^2759:0B62
					_4976_3f68 &= ~(1 << bp06);
				}
				//^2759:0B70
			}
			//^2759:0B7F
			if (glbSomeChampionIndex < 0) {
				//^2759:0B86
				glbSomeChampionIndex = 0;
			}
			//^2759:0B8C
			glbSomeChampionIndex = -glbSomeChampionIndex;
			//^2759:0B94
			if (bp0a != 0 || bp0c != 0) {
				//^2759:0BA0
				DRAW_SQUAD_POS_INTERFACE();
			}
		}
		//^2759:0BA5
		_4976_3df7 = glbChampionLeader;
		//^2759:0BB0
		if (glbChampionIndex > 0 && (glbSomeChampionPanelFlag != 0 || bp0a != 0 || bp0c != 0 || (glbRightPanelType == RIGHT_PANEL_MAGIC_MAP && xx != 0))) {
			//^2759:0BDB
			if (glbSomeChampionPanelFlag > 1) {
				//^2759:0BE2
				glbSomeChampionPanelFlag--;
				//^2759:0BE6
				goto _0e03;
			}
			//^2759:0BE9
			if (glbMagicalMapFlags != 0 && glbSomeChampionPanelFlag == 0 && bp0a == 0) {
				//^2759:0BFD
				_29ee_00a3(0);
			}
			else {
				//^2759:0C07
				_29ee_00a3(1);
				//^2759:0C0F
				DRAW_PLAYER_NAME_AT_CMDSLOT();
				//^2759:0C14
				glbMagicalMapFlags &= 0xfbff;
			}
			//^2759:0C1A
			if (glbSelectedHandAction == 2) {
				//^2759:0C21
				glbRightPanelType = RIGHT_PANEL_SPELL;
				//^2759:0C27
				DRAW_SPELL_PANEL();
			}
			else {
				//^2759:0C2F
				Bit16u bp06 = glbChampionIndex -1;
				//^2759:0C36
				Champion *champion = &glbChampionSquad[bp06];	//*bp04
				//^2759:0C44
				ObjectID di = champion->Possess(glbSelectedHandAction);
				//^2759:0C53
				if (IS_CONTAINER_MAP(di) != 0 && glbMagicalMapFlags == 0) {
					//^2759:0C65
					_4976_531a = 1;
					_4976_5316 = 25;
					//^2759:0C71
					if (champion->curMP() != 0) {
						//^2759:0C7B
						champion->manaCurrent--;
						//^2759:0C7F
						champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x0800
						//^2759:0C85
						glbMagicalMapFlags = 0x8200;	// 0x8200
					}
					else {
						//^2759:0C8D
						glbMagicalMapFlags = 0x8000;	// 0x8000
					}
					//^2759:0C93
					glbRightPanelType = RIGHT_PANEL_MAGIC_MAP;	// 3
				}
				//^2759:0C9B
				else if (IS_CONTAINER_MONEYBOX(di) != 0) {
					//^2759:0CA6
					glbRightPanelType = RIGHT_PANEL_MONEY_BOX;	// 4
				}
				//^2759:0CAE
				else if (IS_CONTAINER_CHEST(di) != 0 && glbRightPanelType != RIGHT_PANEL_CONTAINER) {	// != 5
					//^2759:0CB9
					__CHECK_ROOM_FOR_CONTAINER(di, GET_ADDRESS_OF_RECORD9(di));
					//^2759:0CD2
					glbRightPanelType = RIGHT_PANEL_CONTAINER;	// 5
				}

				//^2759:0CD8
				if (glbRightPanelType == RIGHT_PANEL_MONEY_BOX) {	// 4
					//^2759:0CDF
					DRAW_MONEYBOX(di);
				}
				//^2759:0CE7
				else if (glbRightPanelType == RIGHT_PANEL_CONTAINER) {	// 5
					//^2759:0CEE
					DRAW_CONTAINER_PANEL(di, 1);
				}
				//^2759:0CFA
				else if (IS_ITEM_HAND_ACTIVABLE(bp06, di, glbSelectedHandAction) != 0 && glbRightPanelType != RIGHT_PANEL_MAGIC_MAP) {
					//^2759:0D14
					glbRightPanelType = RIGHT_PANEL_HAND_ACTIONS;
					//^2759:0D1A
					if (glbRightPanelType != glbPreviousRightPanelType) {
						//^2759:0D20
						_1031_0667();
						//^2759:0D25
						glbPreviousRightPanelType = glbRightPanelType;
					}
					//^2759:0D2B
					_29ee_0b2b();
				}
				//^2759:0D32
				else if (glbRightPanelType == RIGHT_PANEL_MAGIC_MAP) {	// 3
					//^2759:0D39
					if ((glbMagicalMapFlags & 0x0400) == 0) {
						//^2759:0D41
						glbPreviousRightPanelType = 0xffff;
					}
					//^2759:0D47
					DRAW_MAJIC_MAP(di);
				}
				else {
					//^2759:0D50
					glbChampionIndex = 0;
					glbRightPanelType = RIGHT_PANEL_SQUAD_HANDS;
				}
			}

			//^2759:0D5C
			glbSomeChampionPanelFlag = 0;
		}
		else {
			//^2759:0D64
			if (glbChampionIndex > 0 && glbRightPanelType == RIGHT_PANEL_CONTAINER && xx != 0) {
				//^2759:0D78
				_29ee_00a3(0);
				//^2759:0D80
				DRAW_CONTAINER_PANEL(glbChampionTable[glbChampionIndex].Possess(glbSelectedHandAction), 0);
			}
		}
		//^2759:0D9F
		if (glbChampionIndex > 0 && _4976_3df9[RCJ(7,glbRightPanelType)] != 0 && (glbChampionTable[glbChampionIndex].enchantmentPower != 0 || _4976_3f68 != 0)) {
			//^2759:0DC9
			DRAW_PLAYER_ATTACK_DIR();
			//^2759:0DCE
			if (glbChampionTable[glbChampionIndex].enchantmentPower != 0) {
				//^2759:0DDF
				_4976_3f68 |=   1 << (i8(glbChampionIndex) -1);
			}
			else {
				//^2759:0DF1
				_4976_3f68 &= ~(1 << (i8(glbChampionIndex) -1));
			}	
		}

		//^2759:0E03
_0e03:
		if (_4976_3f6c.w0 != 0xffff) {
			//^2759:0E0A
			_0b36_0cbe(&_4976_3f6c, 1);
		}
		//^2759:0E18
		if (glbRightPanelType != glbPreviousRightPanelType) {
			//^2759:0E21
			_1031_0667();
			//^2759:0E26
			glbPreviousRightPanelType = glbRightPanelType;
		}
	}
	//^2759:0E2C
	//SkD((DLV_TWEET, "Tweet: Right panel %d\n", glbRightPanelType));
	return;
}

//^1031:04F5
void SkWinCore::_1031_04f5()
{
	//^1031:04F5
	ENTER(0);
	//^1031:04F8
	if (_4976_4ddc != 0) {
		//^1031:04FF
		_4976_4ddc = 0;
		//^1031:0505
		_29ee_000f();
	}
	//^1031:050A
	return;
}

//^12B4:000D
void SkWinCore::HIGHLIGHT_ARROW_PANEL(Bit16u cls4, Bit16u rectno, Bit16u bright)
{
	//^12B4:000D
	ENTER(52);
	//^12B4:0012
	X16 si = rectno;
	_4976_4eb6 = U8(cls4);
	_4976_4ebc = si;
	_4976_4eb4 = bright;
	if (_4976_4eb4 != 0)
		cls4++;
	//^12B4:002C
	FIRE_HIDE_MOUSE_CURSOR();
	sk3f6c bp34;
	_0b36_0c52(&bp34, si, 1);
	FILL_ENTIRE_PICT(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp34.w0), glbPaletteT16[COLOR_BLACK]);
	DRAW_ICON_PICT_ENTRY(1, 3, U8(cls4), &bp34, si, -1);
	_0b36_0cbe(&bp34, 1);
	FIRE_SHOW_MOUSE_CURSOR();
	WAIT_SCREEN_REFRESH();
	//^12B4:008F
	return;
}

//^12B4:0092
void SkWinCore::_12b4_0092()
{
	//^12B4:0092
	ENTER(0);
	//^12B4:0095
	if (_4976_4eb4 != 0) {
		//^12B4:009C
		HIGHLIGHT_ARROW_PANEL(_4976_4eb6, _4976_4ebc, 0);
	}
	//^12B4:00AD
	return;
}

//^00EB:03E7
void SkWinCore::IBMIO_FILL_HALFTONE_RECT(SRECT *rc) //#DS=04BF
{
	//^00EB:03E7
	ENTER(4);
	//^00EB:03ED
	LOADDS(0x0c48);
	//^00EB:03F3
	for (i16 si = rc->y; rc->y + rc->cy -1 >= si; si++) {
		//^00EB:03FC
		Bit8u *bp04 = &pbVram[si * 320 + rc->x];
		//^00EB:0416
		for (i16 di = rc->x; rc->x + rc->cx -1 >= di; bp04++, di++) {
			//^00EB:041E
			if (((di ^ si) & 1) == 0) {
				//^00EB:0429
				*bp04 = 0;
			}
			//^00EB:0430
		}
		//^00EB:0447
	}
	//^00EB:0458
#if UseAltic
	skwin.UpdateRect(rc->x, rc->y, rc->cx, rc->cy);
#endif
	return;
}

//^44C8:1D11
void SkWinCore::FIRE_FILL_HALFTONE_RECTV(SRECT *rc, Bit16u aa)
{
	//^44C8:1D11
	ENTER(0);
	//^44C8:1D14
	IBMIO_FILL_HALFTONE_RECT(rc) CALL_IBMIO;
	//^44C8:1D24
    return;
}

//^44C8:1DDA
void SkWinCore::FIRE_FILL_HALFTONE_RECTI(Bit16u rectno, Bit16u aa)
{
	//^44C8:1DDA
	ENTER(8);
	//^44C8:1DDE
	SRECT bp08;
	FIRE_FILL_HALFTONE_RECTV(QUERY_EXPANDED_RECT(rectno, &bp08), aa);
}

//^01B0:088D
void SkWinCore::IBMIO_MOUSE_RELEASE_CAPTURE() //#DS=04BF
{
#if UseAltic
	sysMousePositionCaptured--;
#else
//		//^01B0:088D
//		ENTER(0);
//		//^01B0:0890
//		LOADDS(0x3083);
//		//^01B0:0896
//		if (_04bf_18b2 == 1) {
//			//^01B0:089F
//			__asm mov cx,[glbMouseXPos]
//			__asm shl cx,1
//			__asm mov dx,[glbMouseYPos]
//			__asm mov ax,0x0004
//			__asm int 0x33
//		}
//		//^01B0:08AE
//		sysMousePositionCaptured--;
//		//^01B0:08B2
//		return;
#endif
}

//^443C:0401
void SkWinCore::FIRE_MOUSE_RELEASE_CAPTURE()
{
	//^443C:0401
	ENTER(0);
	//^443C:0404
	IBMIO_MOUSE_RELEASE_CAPTURE() CALL_IBMIO;
	//^443C:040C
	return;
}

//^1031:050C
void SkWinCore::_1031_050c()
{
	//^1031:050C
	ENTER(0);
	//^1031:050F
	if (glbShowItemStats != 0 || _4976_4bfe != 0 || _4976_4c3e != 0) {
		//^1031:0524
		_4976_4c3e = 0;
		_4976_4bfe = 0;
		glbShowItemStats = 0;
		//^1031:052F
		FIRE_MOUSE_RELEASE_CAPTURE();
		//^1031:0534
		glbMouseVisibility = 1;
		//^1031:053A
		FIRE_SHOW_MOUSE_CURSOR();
	}
	//^1031:053F
	return;
}

//^1031:0675
void SkWinCore::_1031_0675(Bit16u xx)
{
	//^1031:0675
	ENTER(0);
	//^1031:0678
	_4976_4ea8 = _4976_19ad;
	//^1031:067E
	_4976_4e64 = 0;
	_4976_4e48 = 0;
	_4976_4e96 = 0;
	//^1031:0692
	CHAMPION_SQUAD_RECOMPUTE_POSITION();
	//^1031:0698
	_1031_050c();
	//^1031:069B
	_1031_0541(xx);
	//^1031:06A3
	return;
}


//^1031:003E
Bit16u SkWinCore::_1031_003e(sk1891 *ref)
{
	//^1031:003E
	ENTER(0);
	//^1031:0041
	if (ref->b1 == glbChampionInventory)
		//^1031:0050
		return 1;
	//^1031:0055
	return (ref->b1 <= 4 || (glbChampionInventory != 0 && ref->b1 -4 == glbChampionInventory)) ? 0 : 1;
}



//^1031:00C5
Bit16u SkWinCore::_1031_00c5(sk1891 *ref)
{
	//^1031:00C5
	ENTER(0);
	//^1031:00C8
	return ((ref->b1 == 0 && _4976_5dbc == 0) || (ref->b1 != 0 && _4976_5dbc != 0)) ? 1 : 0;
}

//^1031:00F3
Bit16u SkWinCore::_1031_00f3(sk1891 *ref)
{
	//^1031:00F3
	ENTER(0);
	//^1031:00F6
	if (glbChampionIndex == 0) {
		//^1031:00FD
		if (ref->b1 > 3)
			//^1031:0107
			return 1;
		//^1031:010C
		if (GET_PLAYER_AT_POSITION((ref->b1 + glbPlayerDir) & 3) >= 0)
			//^1031:0127
			return 1;
	}
	//^1031:0129
	return 0;
}


//^1031:01BA
Bit16u SkWinCore::_1031_01ba(sk1891 *ref)
{
	//^1031:01BA
	ENTER(0);
	//^1031:01BD
	return (ref->b1 == glbRightPanelType) ? 1 : 0;
}

//^1031:027E
void SkWinCore::_1031_027e(sk1891 *ref)
{
	//^1031:027E
	ENTER(8);
	//^1031:0282
	Bit8u *bp08 = _1031_023b(ref);

	do {
		//^1031:0294
		sk1891 *bp04 = &_4976_1574[*bp08 & 0xff7f];
		//^1031:02AB
		if ((this->*_4976_0cba[ref->b0_0_6()])(bp04) != 0) {
			//^1031:02CA
			if ((bp04->b0 & 0x80) == 0x80) {
				//^1031:02D6
				_1031_027e(bp04);
			}
			else {
				//^1031:02E4
				_4976_16ed[bp04->w2].b6 |= 0x40;
			}
		}
		//^1031:02F7
		bp08++;
		//^1031:02FA
	} while ((*bp08 & 0x80) == 0);
	//^1031:0308
	return;
}

//^098D:025D
Bit16u SkWinCore::PT_IN_RECT(SRECT *rc, i16 xx, i16 yy)
{
	//^098D:025D
	ENTER(0);
	//^098D:0261
	return (true
			&& rc->x <= xx 
			&& rc->x + rc->cx -1 >= xx 
			&& rc->y <= yy 
			&& rc->y + rc->cy -1 >= yy
			) ? 1 : 0;
}

//^1031:01D5
SRECT *SkWinCore::_1031_01d5(Bit16u rectno, SRECT *rc)
{
	//^1031:01D5
	ENTER(4);
	//^1031:01DA
	Bit16u si = rectno;
	//^1031:01DD
	rc = QUERY_EXPANDED_RECT(si & 0x3fff, rc);
	//^1031:01F7
	if (rc != NULL) {
		//^1031:01FB
		if ((si & 0x8000) != 0) {
			//^1031:0201
			si = 7;
			//^1031:0204
			goto _020f;
		}
		//^1031:0206
		if ((si & 0x4000) != 0) {
			//^1031:020C
			si = 18;
			//^1031:020F
_020f:
			i16 bp02;
			i16 bp04;
			QUERY_TOPLEFT_OF_RECT(si, &bp02, &bp04);
			//^1031:0222
			rc->x += bp02;
			rc->y += bp04;
		}
	}
	//^1031:0232
	return rc;
}

//^1031:0A88
Bit16u SkWinCore::_1031_0a88(sk0d9e *ref, Bit16u xx, Bit16u yy, Bit16u ww)
{
	//^1031:0A88
	ENTER(4);
	//^1031:0A8D
	if (ref == NULL)
		//^1031:0A95
		return 0;
	//^1031:0A9A
	Bit16u si = 0;
	sk0d9e *bp04 = ref;
	do {
		//^1031:0AA8
		if (true
			&& (ref->w4 & 0x0800) == 0 
			&& (ww & (255 & ref->w4)) != 0 
			&& _1031_01d5(ref->w2, &_4976_4e9e) != NULL
			&& PT_IN_RECT(&_4976_4e9e, xx, yy) != 0
		) {
			//^1031:0AF1
			_4976_4df8 = ref->w2;
			//^1031:0AFB
			_4976_4e44 = ((_4976_4df8 & 0x8000) != 0) ? 7 : (((_4976_4df8 & 0x4000) != 0) ? 0x0012 : 0xffff);
			//^1031:0B21
			_4976_4df8 = _4976_4df8 & 0x3fff;
			_4976_4e66 = xx;
			_4976_4e68 = yy;
			//^1031:0B36
			si = 0x07ff & ref->w0;
			//^1031:0B41
			glbUIEventCode = si;
			_4976_4e9c = 0;
			//^1031:0B4A
			_4976_4dfc = glbUIEventCode - ((0x07ff & bp04->w0) -1);
			//^1031:0B5E
			return si;
		}
		//^1031:0B60
		ref++;
		//^1031:0B64
	} while ((ref->w0 & 0x8000) == 0);
	//^1031:0B71
	_4976_4e9e.cx = 0;
	_4976_4e9e.cy = 0;
	//^1031:0B79
	return si;
}

//^1031:024C
sk0d9e *SkWinCore::_1031_024c(sk1891 *ref)
{
	//^1031:024C
	ENTER(0);
	//^1031:0250
	Bit16u si = _4976_16ed[ref->w2].w2;
	//^1031:0264
	return (si == 0xffff) ? NULL : &_4976_0d9e[si];
}

//^1031:030A
Bit16u SkWinCore::_1031_030a(sk1891 *ref, Bit16u xx, Bit16u yy, Bit16u zz)
{
	//^1031:030A
	ENTER(16);
	//^1031:0310
	Bit16u di = yy;
	Bit16u si = 0;
	//^1031:0315
	Bit8u *bp08 = _1031_023b(ref);

	do {
		//^1031:0327
		sk1891 *bp04 = &_4976_1574[*bp08 & 0xff7f];
		//^1031:033E
		if ((this->*_4976_0cba[ref->b0_0_6()])(bp04) != 0) {
			//^1031:035D
			SRECT bp10;
			if ((bp04->b0 & 0x80) == 0x80) {
				//^1031:0369
				si = _1031_030a(bp04, xx, di, zz);
				//^1031:037F
				if (si != 0)
					//^1031:0383
					//^1031:03EC
					return si;
			}
			//^1031:0385
			else if (PT_IN_RECT(_1031_01d5(_4976_16ed[bp04->w2].w0, &bp10), xx, di) != 0) {
				//^1031:03B5
				if ((si = _1031_0a88(_1031_024c(bp04), xx, di, zz)) != 0)
					//^1031:03D6
					//^1031:03EC
					return si;
			}
		}
		//^1031:03D8
		bp08++;
		//^1031:03DB
	} while ((*bp08 & 0x80) == 0);
	//^1031:03EC
	return si;
}

//^1031:0C58
Bit16u SkWinCore::_1031_0c58(Bit16u xx, sk0d9e *ref)
{
	//^1031:0C58
	ENTER(4);
	//^1031:0C5D
	_4976_4e44 = 0xffff;
	//^1031:0C63
	sk0d9e *bp04 = ref;
	//^1031:0C6F
	if (ref != NULL) {
		//^1031:0C7A
		Bit16u si;
		for (; (si = 0x07ff & ref->w0) != 0; ref++) {
			//^1031:0C7C
			if ((ref->w4 & 0x0800) == 0 && si == xx) {
				//^1031:0C8C
				_1031_01d5(ref->w2, &_4976_4e9e);
				//^1031:0C9B
				_4976_4df8 = ref->w2;
				//^1031:0CA5
				if ((_4976_4df8 & 0x8000) != 0) {
					//^1031:0CAD
					_4976_4e44 = 7;
				}
				//^1031:0CB5
				else if ((_4976_4df8 & 0x4000) != 0) {
					//^1031:0CBD
					_4976_4e44 = 18;
				}
				//^1031:0CC3
				_4976_4df8 = _4976_4df8 & 0x3fff;
				_4976_4e66 = _4976_4e9e.x;
				_4976_4e68 = _4976_4e9e.y;
				glbUIEventCode = si;
				//^1031:0CDC
				_4976_4dfc = glbUIEventCode - (0x07ff & bp04->w0) -1;
				//^1031:0CF0
				return 1;
			}
			//^1031:0CF5
		}
		//^1031:0D0B
		_4976_4ea2 = 0;
	}
	//^1031:0D13
	_4976_4df8 = 0xffff;
	_4976_4e66 = 0;
	_4976_4e68 = 0;
	glbUIEventCode = xx;
	_4976_4dfc = 0;
	//^1031:0D31
	return 0;
}

//^1031:03F2
Bit16u SkWinCore::_1031_03f2(sk1891 *ref, Bit16u xx)
{
	//^1031:03F2
	ENTER(12);
	//^1031:03F8
	Bit16u si = 0;
	//^1031:03FA
	Bit8u *bp08 = _1031_023b(ref);

	do {
		//^1031:040C
		sk1891 *bp04 = &_4976_1574[*bp08 & 0xff7f];
		//^1031:0423
		if ((this->*_4976_0cba[ref->b0_0_6()])(bp04) != 0) {
			//^1031:0445
			if ((bp04->b0 & 0x80) == 0x80) {
				//^1031:0451
				si = _1031_03f2(bp04, xx);
				//^1031:0463
				if (si != 0)
					//^1031:0467
					//^1031:04EF
					return si;
			}
			else {
				//^1031:046A
				Bit16u di = _4976_16ed[bp04->w2].w4;
				//^1031:047E
				if (di != 0xffff) {
					//^1031:0483
					sk13a4 *bp0c = &_4976_13a4[di];

					do {
						//^1031:0491
						if (bp0c->w2 == xx) {
							//^1031:049D
							_1031_024c(bp04);
							//^1031:04A9
							_1031_0c58(si = 0x07ff & bp0c->w0, _1031_024c(bp04));
							//^1031:04BF
							_4976_4e9c = xx;
							//^1031:04C5
							break;
						}
						//^1031:04C7
					} while (((++bp0c)->w0 & 0x8000) == 0);
					//^1031:04D5
					if (si != 0)
						//^1031:04D9
						//^1031:04EF
						return si;
				}
			}
		}
		//^1031:04DB
		bp08++;
		//^1031:04DE
	} while ((*bp08 & 0x80) == 0);
	//^1031:04EF
	return si;
}

//^1031:10C8
void SkWinCore::_1031_10c8(sk3f6c *ref, SRECT *rc, Bit16u cx, Bit16u cy)
{
	//^1031:10C8
	ENTER(0);
	//^1031:10CB
	if (ref->w0 == 0xffff) {
		//^1031:10D4
		COPY_MEMORY(&glbMousePosition.rc6, &ref->rc2, 8);
		//^1031:10ED
		_0b36_0c52(ref, -1, 0);
	}
	//^1031:10FF
	CALC_CENTERED_RECT_IN_RECT(rc, &ref->rc2, cx, cy);
	//^1031:111C
	return;
}

//^0B36:129A
void SkWinCore::_0b36_129a(sk3f6c *ref, i16 xx, i16 yy, Bit8u clr1, Bit8u clr2, Bit8u *str)
{
	//^0B36:129A
	ENTER(4);
	//^0B36:129E
	i16 bp02;
	i16 bp04;
	if (QUERY_STR_METRICS(str, &bp02, &bp04) != 0) {
		//^0B36:12BA
		DRAW_STRING(
			reinterpret_cast<Bit8u *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(ref->w0)),
			ref->w0,
			ref->rc2.cx,
			xx - ref->rc2.x,
			yy - ref->rc2.y,
			clr1,
			clr2,
			str,
			8
			);
		//^0B36:12F5
		_0b36_0d67(
			ref, 
			ALLOC_TEMP_RECT(xx, yy, bp02, bp04)
			);
	}
	//^0B36:1318
	return;
}

//^0B36:0A3F
void SkWinCore::DRAW_PICST(ExtendedPicture *ref)
{
	//^0B36:0A3F
	ENTER(12);
	//^0B36:0A45
	if (ref->width <= 0 || ref->height <= 0)
		//^0B36:0A59
		return;

	//^0B36:0A5C
	U8 *bp04 = QUERY_PICT_BITS(ref);
	//^0B36:0A6E
	U16 iRectNo = ref->rectNo;	// U16 bp06
	//^0B36:0A78
	i16 bp08;
	i16 bp0a;
	if (iRectNo == 0xFFFF) {
		//^0B36:0A7D
		bp08 = ref->w32;
		bp0a = ref->w34;
	}
	else {
		//^0B36:0A8D
		if ((iRectNo & 0x8000) != 0 || ref->w28 != 0 || ref->w30 != 0) {
			//^0B36:0AA5
			iRectNo = iRectNo | 0x8000;
			bp08 = ref->w32 + ref->w28;
			bp0a = ref->w34 + ref->w30;
		}
		else {
			//^0B36:0AC6
			bp08 = ref->width;
			bp0a = ref->height;
		}
		//^0B36:0AD4
		if (QUERY_BLIT_RECT(bp04, &ref->rc36, iRectNo, &bp08, &bp0a, ref->w26) == 0) {
			//^0B36:0B07
			return;
		}
	}
	//^0B36:0B0A
	bp08 += ref->w14;
	bp0a += ref->w16;
	i16 si = READ_I16(bp04,-4);
	i16 di = ref->rc36.cx +bp08;
	//^0B36:0B2E
	if (si > di && (ref->mirrorFlip & 0x0001) != 0) {
		//^0B36:0B3A
		si -= di;	
	}
	else {
		//^0B36:0B3E
		si = 0;
	}
	//^0B36:0B40

	//if ((ref->w50 & 0x0001) != 0)
	if ((ref->mirrorFlip & DRAW_FLAG_FLIP_1) != 0)
		//^0B36:0B4B
		bp08 = 0;
	//^0B36:0B50
	bp08 += si;
	//^0B36:0B53
	si = READ_I16(bp04,-2);
	//^0B36:0B5A
	di = ref->rc36.cy +bp0a;
	//^0B36:0B66
	//if (si > di && (ref->w50 & 0x0002) != 0) {
	if (si > di && (ref->mirrorFlip & DRAW_FLAG_FLIP_2) != 0) {
		//^0B36:0B72
		si -= di;
	}
	else {
		//^0B36:0B76
		si = 0;
	}
	//^0B36:0B78
	if ((ref->mirrorFlip & DRAW_FLAG_FLIP_2) != 0)	// (ref->w50 & 0x0002) != 0)
		//^0B36:0B83
		bp0a = 0;
	//^0B36:0B88
	bp0a += si;
	//^0B36:0B8B
	U16 bp0c;
	if (ref->pb44 == _4976_4964) {
		//^0B36:0BA2
		si = glbScreenWidth;
		di = glbScreenHeight;
		//^0B36:0BAA
		bp0c = 8;
	}
	else {
		//^0B36:0BB1
		si = READ_I16(ref->pb44,-4);
		//^0B36:0BBC
		di = READ_I16(ref->pb44,-2);
		//^0B36:0BC7
		bp0c = READ_I16(ref->pb44,-6);
	}
	//^0B36:0BD5
	if (ref->colorKeyPassThrough != -2) {
		//^0B36:0BDF
		FIRE_BLIT_PICTURE(
			bp04,								// *src
			ref->pb44,							// *dst
			&ref->rc36,							// SRECT *rc
			bp08,								// srcx
			bp0a,								// srcy
			READ_I16(bp04,-4),					// srcPitch
			si,									// dstPitch
			ref->colorKeyPassThrough,			// colorkey
			ref->mirrorFlip,					// mirrorFlip
			READ_I16(bp04,-6),					// srcBpp
			bp0c,								// dstBpp
			(ref->w56 == 0) ? NULL : ref->b58	// local pal
			);
	}
	//^0B36:0C3D
	ref->w32 = bp08;
	ref->w34 = bp0a;
	//^0B36:0C4E
	return;
}

//^0B36:1688
//void SkWinCore::DRAW_STATIC_PIC(Bit8u cls1, Bit8u cls2, Bit8u cls4, Bit16u rectno, i16 colorkey)
void SkWinCore::DRAW_STATIC_PIC(Bit8u iCategory, Bit8u iItemNo, Bit8u iEntry, Bit16u rectno, i16 colorkey)
{
	// draw an statical image such as inventory plate.

	//^0B36:1688
	ENTER(314);
	//^0B36:168C
	ExtendedPicture bp013a;
	QUERY_GDAT_SUMMARY_IMAGE(&bp013a, iCategory, iItemNo, iEntry);
	//^0B36:16A5
	bp013a.colorKeyPassThrough = colorkey;
	bp013a.rectNo = rectno;
	//^0B36:16B3
	bp013a.pb44 = _4976_4c16;
	//^0B36:16C2
	bp013a.w28 = 0;
	bp013a.w30 = 0;
	//^0B36:16CE
	DRAW_PICST(QUERY_PICST_IT(&bp013a));
	//^0B36:16E2
	return;
}

//^2E62:064A
// SPX: _2e62_064a renamed DRAW_CHARSHEET_OPTION_ICON
void SkWinCore::DRAW_CHARSHEET_OPTION_ICON(Bit8u cls4, Bit16u rectno, Bit16u zz)
{
	//^2E62:064A
	ENTER(0);
	//^2E62:064D
	if ((_4976_581a & zz) != 0)
		//^2E62:0656
		cls4++;
	//^2E62:0659
	DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, cls4, rectno, -1);
	//^2E62:066E
	return;
}

//^2E62:00A3
void SkWinCore::DRAW_PLAYER_3STAT_PANE(Bit16u player, Bit16u xx)
{
	//^2E62:00A3
	ENTER(2);
	//^2E62:00A8
	U16 si = player;
	//^2E62:00AB
	if (_4976_3ff0.w0 != 0xffff)
		//^2E62:00B0
		return;
	//^2E62:00B2
	// SPX: Choose between 0 (normal champion panel) or 1 (dead champion panel)
	Bit8u bp01 = (glbChampionSquad[si].curHP() == 0) ? 1 : (((si +1) == glbChampionInventory) ? 9 : 0);
	//^2E62:00DB
	_0b36_0c52(&_4976_3ff0, si +161, xx);
	//^2E62:00F0
	DRAW_ICON_PICT_ENTRY(
		0x01,
		0x02,
		bp01,
		&_4976_3ff0,
		si + 161,
		-1
		);
	//^2E62:010C
	if (xx == 0)
		//^2E62:0112
		_4976_3ff0.w10 = 0;
	//^2E62:0118
	return;
}

//^2E62:061D
// SPX: _2e62_061d renamed DRAW_CHAMPION_PICTURE
void SkWinCore::DRAW_CHAMPION_PICTURE(Bit16u player)
{
	//^2E62:061D
	ENTER(0);
	//^2E62:0620
	DRAW_ICON_PICT_ENTRY(
		GDAT_CATEGORY_CHAMPIONS,	// 0x16
		glbChampionSquad[player].HeroType(),
		0x00,
        &_4976_3ff0,
		player +173,
		-1
		);
	//^2E62:0648
	if (bDM1PortraitsActivated)	 // SPX attempt to draw savegame portraits
	{
		i16 bp0c = 0;
		i16 bp0a = 0;
		Bit8u *bp04 = xDM1PortraitsData[player];
		Bit16u rectno = player + 173;
		sk3f6c* ss = &_4976_3ff0;
		i16 colorkey = -1;
		SRECT bp14;
		SRECT *bp08 = QUERY_BLIT_RECT(bp04, &bp14, rectno, &bp0a, &bp0c, -1);
		/*DRAW_ICON_PICT_BUFF(
			bp04,
			ss,
			bp08,
			bp0a,
			bp0c,
			colorkey,
			0,
			QUERY_GDAT_IMAGE_LOCALPAL(GDAT_CATEGORY_CHAMPIONS, 0, 0)
			);*/
		//IBMIO_BLIT_TO_SCREEN_8TO8BPP(
		//	bp04, __vram, bp08, 0, 0, 32, 240, -1);
	}


	return;
}

//^2E62:011B
void SkWinCore::DRAW_PLAYER_3STAT_HEALTH_BAR(U16 player)
{
	//^2E62:011B
	ENTER(32);
	//^2E62:0121
	SRECT bp0c;
	_0b36_0d67(
		&_4976_3ff0,
		QUERY_EXPANDED_RECT(player +185, &bp0c)
		);
	//^2E62:0143
	Champion *bp04 = &glbChampionSquad[player];
	//^2E62:0154
	i16 bp20[3][2];
	bp20[0][0] = bp04->curHP();
	bp20[0][1] = bp04->maxHP();
	bp20[1][0] = bp04->curStamina();
	bp20[1][1] = bp04->maxStamina();
	bp20[2][0] = bp04->curMP();
	bp20[2][1] = max_value(bp04->maxMP(), bp04->curMP());	// SPX Max because of MANA modifier
	//^2E62:018C
	U16 di = player +193;
	//^2E62:0194
	for (i16 si = 0; si < 3; si++, di += 4) {
		//^2E62:0199
		if (bp20[si][1] != 0) {
			//^	
			if (SCALE_RECT(
				di,
				&bp0c,
				10000,
				(bp20[si][0] * i32(10000)) / bp20[si][1]
				) != NULL
			){
				//^2E62:01F2
				SRECT bp14;
				COPY_MEMORY(&bp0c, &bp14, 8);
				//^2E62:0208
				bp14.x += _4976_0114;
				bp14.y += _4976_0116;
				//^2E62:0214
				FILL_RECT_SUMMARY(
					&_4976_3ff0,
					&bp14,
					glbPaletteT16[COLOR_BLACK]
				);
#if DM2_EXTENDED_MODE == 1
				FILL_RECT_SUMMARY(
					&_4976_3ff0,
					&bp0c,
					QUERY_3STAT_BAR_COLOR(player, glbPaletteT16[glbChampionColor[player]])
					);
#else
				//^2E62:022F
				FILL_RECT_SUMMARY(
					&_4976_3ff0,
					&bp0c,
					glbPaletteT16[glbChampionColor[player]]
					);
#endif
			}
		}
		//^2E62:0255
	}
	//^2E62:0261
	return;
}

//^2E62:0572
void SkWinCore::DRAW_PLAYER_DAMAGE(Bit16u player)
{
	//^2E62:0572
	ENTER(0);
	//^2E62:0576
	U16 si = player;
	//^2E62:0579
	DRAW_ICON_PICT_ENTRY(
		0x01,
		0x02,
		0x03,
		&_4976_3ff0,
		si +177,
		10
		);
	//^2E62:0593
	DRAW_SIMPLE_STR(
		&_4976_3ff0,
		si +177,
		glbPaletteT16[COLOR_WHITE],
		glbPaletteT16[COLOR_RED],
		FMT_NUM(glbChampionSquad[si].damageSuffered, 0, 3)
		);
	//^2E62:05D1
	return;
}

//^2E62:02EA

void SkWinCore::DRAW_CUR_MAX_HMS(U16 rectno, i16 curVal, i16 maxVal)
{
// SPX: original code below
	if (!SkCodeParam::bUseDM2ExtendedMode)
	{	
		ATLASSERT(curVal >= 0 && curVal <= 999);
		ATLASSERT(maxVal >= 0 && maxVal <= 999);

		//^2E62:02EA
		ENTER(8);
		//^2E62:02EE
		U8 bp08[8];	// 3 + slash + 3 + eol
		SK_STRCPY(bp08, FMT_NUM(curVal, 1, 3)); // format cur
		//^2E62:030B
		SK_STRCAT(bp08, strSlash); // add slash
		//^2E62:031C
		SK_STRCAT(bp08, FMT_NUM(maxVal, 1, 3)); // format max
		//^2E62:0339
		DRAW_LOCAL_TEXT(rectno, glbPaletteT16[COLOR_LIGHTER_GRAY], glbPaletteT16[COLOR_DARK_GRAY] | 0x4000, bp08);
		//^2E62:035E
		return;
	}
// SPX: modified function one to handle 4 char string in basic stats
	ATLASSERT(curVal >= 0 && curVal <= 9999);
	ATLASSERT(maxVal >= 0 && maxVal <= 9999);
	ENTER(8);
	U8 bp08[12];	// 4 + slash + 4 + eol
	SK_STRCPY(bp08, FMT_NUM(curVal, 1, 4)); // format cur
	SK_STRCAT(bp08, strSlash); // add slash
	SK_STRCAT(bp08, FMT_NUM(maxVal, 1, 4)); // format max
	DRAW_LOCAL_TEXT(rectno, glbPaletteT16[COLOR_LIGHTER_GRAY], glbPaletteT16[COLOR_DARK_GRAY] | 0x4000, bp08);
	return;
}

//^2E62:0360
void SkWinCore::DRAW_PLAYER_3STAT_TEXT(Champion *ref)
{
	//^2E62:0360
	ENTER(0);
	//^2E62:0363
	DRAW_CUR_MAX_HMS(550, ref->curHP(), ref->maxHP());
	//^2E62:0378
	DRAW_CUR_MAX_HMS(
		551,
		ref->curStamina() / 10,
		ref->maxStamina() / 10
		);
	//^2E62:039E
	DRAW_CUR_MAX_HMS(
		552,
		ref->curMP(),
		ref->maxMP()
		);
	//^2E62:03B3
	return;
}

//^2405:00EC
void SkWinCore::_2405_00ec(U16 rectno, SRECT *rc)
{
	//^2405:00EC
	ENTER(4);
	//^2405:00F0
	i16 bp02 = _4976_0106;
	i16 bp04 = _4976_0108;
	//^2405:00FC
	QUERY_BLIT_RECT(
		NULL,
		rc,
		rectno,
		&bp02,
		&bp04,
		-1
		);
	//^2405:011D
	return;
}

//^2405:011F
void SkWinCore::_2405_011f(U16 rectno, SRECT *rc)
{
	//^2405:011F
	ENTER(0);
	//^2405:0122
	_2405_00ec(rectno, rc);
	//^2405:0132
	INFLATE_RECT(rc, _4976_0112, _4976_0112);
	//^2405:0148
	return;
}

//^2E62:05D4
// SPX: _2e62_05d4 renamed DRAW_EYE_MOUTH_COLORED_RECTANGLE
void SkWinCore::DRAW_EYE_MOUTH_COLORED_RECTANGLE(Bit8u cls4, Bit16u rectno)
{
	//^2E62:05D4
	ENTER(8);
	//^2E62:05D8
	SRECT bp08;
	_2405_011f(rectno, &bp08);
	//^2E62:05E8
	DRAW_DIALOGUE_PARTS_PICT(
		QUERY_GDAT_IMAGE_ENTRY_BUFF(0x01, 0x02, cls4),
        &bp08,
		12,
		QUERY_GDAT_IMAGE_LOCALPAL(0x01, 0x02, cls4)
		);
	//^2E62:061B
	return;
}

//^24A5:105B
// SPX: _24a5_105b renamed DRAW_CRYOCELL_LEVER
void SkWinCore::DRAW_CRYOCELL_LEVER(Bit16u leverIsOn)
{
	//^24A5:105B
	ENTER(0);
	//^24A5:105E
	// SPX: 0xFA image of lever up (waiting user to pull down) / 0xFB image of lever down : champion has been selected
	DRAW_STATIC_PIC(GDAT_CATEGORY_WALL_GFX, GDAT_WALL_ORNATE__CRYOCELL, (leverIsOn != 0) ? 0xfb : 0xfa, 0x1ee, -1);
	if (leverIsOn != 0) {
		//^24A5:1082
		CHANGE_VIEWPORT_TO_INVENTORY(0);
		// SPX: If lever is down, play the open cryocell sound
		QUEUE_NOISE_GEN1(GDAT_CATEGORY_WALL_GFX, GDAT_WALL_ORNATE__CRYOCELL, 0xfb, 0xff, 0xc8, glbPlayerPosX, glbPlayerPosY, 0);
	}
	else {
		//^24A5:10AB
		glbInventorySubpanel = 7;
	}
	//^24A5:10B1
	return;
}

//^24A5:0E82
void SkWinCore::DRAW_POWER_STAT_BAR(i16 curval, U16 rectno, U16 color, i16 vv, U16 ww)
{
	// color
	//   0 to  15: glbPaletteT16[color]
	// 256 to 511: pure 256

	//^24A5:0E82
	ENTER(24);
	//^24A5:0E88
	i16 di = curval;
	//^24A5:0E8B
	//i16 si = (di < -512) ? 8 : ((di < 0) ? 11 : zz);
#if DM2_EXTENDED_MODE == 1
	i16 si = (di < FOOD_WARNING) ? QUERY_FOOD_WATER_BAR_COLOR(0x0A, COLOR_RED) : ((di < 0) ? QUERY_FOOD_WATER_BAR_COLOR(0x09, COLOR_YELLOW) : color);
#else
	i16 si = (di < FOOD_WARNING) ? COLOR_RED : ((di < 0) ? COLOR_YELLOW : color);
#endif
	//^24A5:0EA2
	di -= vv;
	//^24A5:0EA5
	SRECT bp08;
	if (SCALE_RECT(
		rectno,
		&bp08,
		//(i32(10000) * di) / (2048 - vv),
		(i32(10000) * di) / (FOOD_MAX - vv),
		10000
		) != NULL
	) {
		//^24A5:0EE1
		SRECT bp18;
		SRECT bp10;
		if (ww != 0) {
			//^24A5:0EE7
			QUERY_EXPANDED_RECT(rectno, &bp10);
			//^24A5:0EF7
			COPY_MEMORY(&bp10, &bp18, 8);
			//^24A5:0F0D
			if ((bp08.x +bp08.cx -1) == (bp10.x +bp10.cx -1)) {
				//^24A5:0F1F
				ww = 0;
			}
			else {
				//^24A5:0F26
				bp10.cx = (bp10.x +bp10.cx -1) -(bp08.x +bp08.cx -1);
				//^24A5:0F39
				bp10.x = bp08.x +bp08.cx +0;
			}
		}
		else {
			//^24A5:0F47
			COPY_MEMORY(&bp08, &bp18, 8);
		}
		//^24A5:0F5D
		bp18.x += _4976_0142;
		bp18.y += _4976_0142;
		//^24A5:0F66
		FIRE_FILL_BACKBUFF_RECT(&bp18, glbPaletteT16[COLOR_BLACK]);
		//^24A5:0F7D
#if DM2_EXTENDED_MODE == 1
		FIRE_FILL_BACKBUFF_RECT(&bp08, ((si & 0x0100) == 0x0000) ? glbPaletteT16[si] : (si & 0x00FF));
#else
		FIRE_FILL_BACKBUFF_RECT(&bp08, glbPaletteT16[si]);
#endif
		//^24A5:0F94
		if (ww != 0) {
			//^24A5:0F9A
			FIRE_FILL_BACKBUFF_RECT(&bp10, glbPaletteT16[ww]);
		}
	}
	//^24A5:0FB4
	return;
}

#if DM2_EXTENDED_MODE == 1
// kkdf2: ext for emetar
i16 SkWinCore::QUERY_FOOD_WATER_BAR_COLOR(U8 cls4, i16 def_color) {
	RawEntry *bp04 = QUERY_GDAT_ENTRYPTR(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0, dtWordValue, cls4);
	if (bp04 != NULL) {
		return 256 + bp04->data;
	}
	return def_color;
}

// kkdf2: ext for emetar
i16 SkWinCore::QUERY_3STAT_BAR_COLOR(U8 cls4, i16 def_color) {
	RawEntry *bp04 = QUERY_GDAT_ENTRYPTR(GDAT_CATEGORY_INTERFACE_GENERAL, GDAT_INTERFACE_CLASS_CHAMPION_FACING, dtWordValue, cls4);
	if (bp04 != NULL) {
		return bp04->data;
	}
	return def_color;
}
#endif

//^24A5:0FB8
// SPX: _24a5_0fb8 renamed DRAW_FOOD_WATER_POISON_PANEL
void SkWinCore::DRAW_FOOD_WATER_POISON_PANEL()
{
	//^24A5:0FB8
	ENTER(4);
	//^24A5:0FBC
	Champion *pChampion = &glbChampionTable[glbChampionInventory];
	//^24A5:0FCD
	glbInventorySubpanel = 1;
	//^24A5:0FD3
	DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, GDAT_INTERFACE_CHAR_FOOD_WATER_PANEL, 494, -1);
#if DM2_EXTENDED_MODE == 1
	DRAW_POWER_STAT_BAR(pChampion->curFood(), 496, QUERY_FOOD_WATER_BAR_COLOR(0x06, COLOR_BROWN), -1024, 0);
	DRAW_POWER_STAT_BAR(pChampion->curWater(), 497, QUERY_FOOD_WATER_BAR_COLOR(0x07, COLOR_BLUE), -1024, 0);
#else
	//^24A5:0FE6
	DRAW_POWER_STAT_BAR(pChampion->curFood(), 496, COLOR_BROWN, -1024, 0);
	//^24A5:0FFE
	DRAW_POWER_STAT_BAR(pChampion->curWater(), 497, COLOR_BLUE, -1024, 0);
#endif
	//^24A5:1016
	DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, GDAT_INTERFACE_FOOD_TEXT, 500, 12);	// food
	//^24A5:1029
	DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, GDAT_INTERFACE_WATER_TEXT, 501, 12);	// water
	//^24A5:103C
	if (pChampion->PoisonValue != 0) {
		//^24A5:1046
		DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, GDAT_INTERFACE_POISON_TEXT, 502, 12); // poison
		
		// TODO SPX: In PC-DM2, the POISON value has its power bar.
		//	But it requires a rectno for it, which does not exists in PC9821 version ?!
		//if (bUseSuperMode)
		//	DRAW_POWER_STAT_BAR(pChampion->PoisonValue, 497, COLOR_RED, -1024, 0);
	}
#if (DM2_EXTENDED_MODE == 1)
	if (pChampion->PlagueValue != 0) 
	{
		DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, GDAT_INTERFACE_PLAGUED_TEXT, 502, 12); // plagued
	}
#endif
	//^24A5:1059
	return;
}

//^24A5:000F
void SkWinCore::DRAW_GUIDED_STR(const U8 *ref)
{
	// drawtext. draw str for item survey. increment y by text height automatically.

	//^24A5:000F
	ENTER(136);
	//^24A5:0014
	if (*ref == 12) {
		//^24A5:001D
		ref++;
		//^24A5:0020
		QUERY_TOPLEFT_OF_RECT(556, &_4976_52d8, &_4976_52da);
	}
	//^24A5:0033
	if (*ref == 0)
		//^24A5:003C
		return;
	//^24A5:003F
	if (QUERY_MBCS_PRESENCE(ref) != 0) {
		//^24A5:0050
		DRAW_VP_STR(_4976_52d8, _4976_52da, glbPaletteT16[COLOR_LIGHTER_GRAY], ref);
		//^24A5:0071
		_4976_52da += _4976_013a;
		//^24A5:0078
		return;
	}
	//^24A5:007B
	U8 bp0088[128];
	SK_STRCPY(bp0088, ref);
	//^24A5:008F
	U8 *bp04 = bp0088;
	//^24A5:0099
	for (i16 si = 0; *bp04 != 0; ) {
		//^24A5:009E
		U8 *bp08;
		if (SK_STRLEN(bp04) > 18) {
			//^24A5:00B0
			bp08 = bp04 +17;
			//^24A5:00BF
			while (*bp08 != ' ') bp08--;
			//^24A5:00CD
			*bp08 = 0;
			//^24A5:00D1
			si = 1;
		}
		else {
			//^24A5:00D6
			bp08 = NULL;
		}
		//^24A5:00E0
		DRAW_VP_STR(_4976_52d8, _4976_52da, glbPaletteT16[COLOR_LIGHTER_GRAY], bp04);
		//^24A5:0101
		_4976_52da += glbPanelStatsYDelta;
		//^24A5:0108
		if (si != 0 && bp08 != NULL) {
			//^24A5:0114
			si = 0;
			//^24A5:0116
			bp08++;
			//^24A5:0119
			bp04 = bp08;
		}
		else {
			//^24A5:0127
			*bp04 = 0;
		}
		//^24A5:012E
	}
	//^24A5:013A
	return;
}

//^24A5:0732
void SkWinCore::_24a5_0732(i16 xx, i16 yy, U8 *str)
{
	//^24A5:0732
	ENTER(208);
	//^24A5:0737
	U16 bp04 = 0;
	U16 bp02;
	if (QUERY_STR_METRICS(str, &bp02, &bp04) == 0)
		return;
	//^24A5:075B
	U8 bp00d0[200];
	if (QUERY_MBCS_PRESENCE(str) == 0) {
		//^24A5:076C
		X16 si = 0;
		bp00d0[0] = 2;
		bp00d0[1] = 0x20;
		U8 bp05;
		while ((bp05 = str[si]) != 0) {
			//^24A5:077A
			if (bp05 >= 0x41 && bp05 <= 0x5a) {
				//^24A5:0786
				bp05 = bp05 -0x40;
			}
			else if (bp05 >= 0x7b) {
				//^24A5:0793
				bp05 = bp05 -0x60;
			}
			//^24A5:079B
			bp00d0[si +2] = bp05;
			si++;
			//^24A5:07AC
		}
		//^24A5:07B9
		bp00d0[si +2] = 0;
		str = bp00d0;
	}
	//^24A5:07CD
	DRAW_VP_STR(xx -(bp02 >> 1), yy, glbPaletteT16[COLOR_BLACK], str);
	//^24A5:07F3
	return;
}

//^24A5:07F6
void SkWinCore::DRAW_SCROLL_TEXT(ObjectID rl)
{
	//^24A5:07F6
	ENTER(418);
	//^24A5:07FC
	U8 bp00da[200];
	U8 bp01a2[200];
	U8 *bp04;
	ATLASSERT(rl.DBType() == dbScroll);
	QUERY_MESSAGE_TEXT(bp04 = bp00da, rl, 0x8002);
	glbInventorySubpanel = 5;
	DRAW_STATIC_PIC(0x07, 0, 0x01, 0x1ee, -1);
	DRAW_STATIC_PIC(0x12, 0, 0x10, 0x1ee, 12);
	X16 si;
	for (si = 0; *bp04 != 0; bp04++) {
		//^24A5:0848
		if (*bp04 == 0xa)
			si++;
		//^24A5:0852
	}
	//^24A5:085E
	if (bp04[-1] != 0xa) {
		si++;
	}
	else if (bp04[-2] == 0xa) {
		si--;
	}
	//^24A5:0873
	i16 bp08;
	i16 bp0a;
	QUERY_TOPLEFT_OF_RECT(0x230, &bp08, &bp0a);
	SRECT bp12;
	QUERY_EXPANDED_RECT(0x1ee, &bp12);
	X16 di;
	if (QUERY_MBCS_PRESENCE(bp00da) == 0) {
		//^24A5:08A9
		di = glbPanelStatsYDelta;
		bp0a -= ((di * si -_4976_0124 -1) >> 1) - _4976_011e;
	}
	else {
		//^24A5:08BD
		di = _4976_013a;
		bp0a -= ((di * si - _4976_0136 -1) >> 1) - _4976_0130;
	}
	//^24A5:08D2
	U16 bp06 = 0;
	while (bp00da[bp06] != 0) {
		//^24A5:08D9
		_3929_04e2_DRAW_TEXT_STRINGS(bp00da, bp01a2, &bp06, bp12.cx);
		_24a5_0732(bp08, bp0a, bp01a2);
		bp0a += di;
		if (bp00da[bp06] == 0xa)
			bp06++;
		//^24A5:091D
	}
	//^24A5:092C
	return;
}

//^2405:02E8
// SPX: _2405_02e8 renamed DRAW_ITEM_ICON
void SkWinCore::DRAW_ITEM_ICON(ObjectID recordLink, i16 xx, U16 yy, U16 zz, U16 ww)
{
	//^2405:02E8
	ENTER(20);
	//^2405:02EE
	i16 si = xx;
	//^2405:02F1
	U8 bp0b;
	U8 bp0c;
	U8 bp09;
	if (recordLink == OBJECT_NULL) {
		//^2405:02F7
		bp0b = 7;
		bp0c = 0;
		//^2405:02FF
		bp09 = _4976_3b74[si].b2 +U8(yy);
	}
	else {
		//^2405:030D
		bp0b = QUERY_CLS1_FROM_RECORD(recordLink);
		bp0c = QUERY_CLS2_FROM_RECORD(recordLink);
		//^2405:0325
		bp09 = (si < 8)
			? GET_ITEM_ICON_ANIM_FRAME(recordLink, si & 1, 1)
			: GET_ITEM_ICON_ANIM_FRAME(recordLink, si -8, 1);
	}
	//^2405:0348
	U16 di = _4976_3b74[si].w0;
	//^2405:0351
	if (si < 0x26) {
		//^2405:0359
		SRECT bp14;
		QUERY_EXPANDED_RECT(di, &bp14);
		//^2405:0367
		U8 *bp04;
		if (si < 8) {
			//^2405:036C
			bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x01, 0x02, 0x00);
			//^2405:0380
			DRAW_ICON_PICT_BUFF(
				bp04,
				&_4976_3ff0,
				&bp14,
				bp14.x - _4976_3ff0.rc2.x,
				bp14.y - _4976_3ff0.rc2.y,
				-1,
				0,
                QUERY_GDAT_IMAGE_LOCALPAL(0x01, 0x02, 0x00)
				);
		}
		else {
			//^2405:03BD
			bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x07, 0x00, 0x00);
			//^2405:03D1
			DRAW_DIALOGUE_PICT(
				bp04,
				_4976_4c16,
				&bp14,
				bp14.x,
				bp14.y,
				-1,
				QUERY_GDAT_IMAGE_LOCALPAL(0x07, 0x00, 0x00)
				);
		}
	}
	//^2405:0404
	if (ww != 0 && si < 14) {
		//^2405:0415
		SRECT bp14;
		_2405_011f(di, &bp14);
		//^2405:0422
		U8 bp0a = (zz != 0)
			? 6
			: (yy != 0)
				? 5
				: 4;
		//^2405:043E
		U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x01, 0x02, bp0a);
		//^2405:0454
		U8 *bp08 = QUERY_GDAT_IMAGE_LOCALPAL(0x01, 0x02, bp0a);
		//^2405:046A
		if (si < 8) {
			//^2405:046F
			DRAW_ICON_PICT_BUFF(
				bp04,
				&_4976_3ff0,
				&bp14,
				0,
				0,
				12,
				0,
				bp08
				);
		}
		//^2405:0496
		DRAW_DIALOGUE_PARTS_PICT(
			bp04,
			&bp14,
			12,
			bp08
			);
	}
	//^2405:04B1
	if (bp09 == 0xff)
		//^2405:04B5
		return;
	//^2405:04B7
	if (si < 8) {
		//^2405:04BC
		DRAW_ICON_PICT_ENTRY(
			bp0b,
			bp0c,
			bp09,
			&_4976_3ff0,
			di,
			12
			);
	}
	else {
		//^2405:04D9
		DRAW_STATIC_PIC(
			bp0b,
			bp0c,
			bp09,
			di,
			12
			);
	}
	//^2405:04F0
	return;
}

//^24A5:0AD2
// SPX: _24a5_0ad2 renamed DRAW_CONTAINER_SURVEY
void SkWinCore::DRAW_CONTAINER_SURVEY(Container *ref)
{
	//^24A5:0AD2
	ENTER(0);
	//^24A5:0AD7
	U16 si = 0;
	//^24A5:0AD9
	//^24A5:0AE0
	for (ObjectID di = ref->GetContainedObject(); di != OBJECT_END_MARKER; di = GET_NEXT_RECORD_LINK(di), si++) {
		//^24A5:0AE2
		if (si >= 8)	// 8 = MAX items in container (CHEST / BAG / QUIVER)
			//^24A5:0AE5
			break;
		//^24A5:0AE7
		DRAW_ITEM_ICON(di, si +47, 0, 0, 0);
		//^24A5:0AFC
	}
	//^24A5:0B0B
	return;
}




//^0CAF:014D
U8 *SkWinCore::SK_LTOA10(i32 value, U8 *str)
{
	//^0CAF:014D
	ENTER(22);
	//^0CAF:0151
	i32 bp08 = value;
	//^0CAF:015D
	if (bp08 < 0) {
		//^0CAF:0167
		bp08 = -bp08;
	}
	//^0CAF:017A
	U8 bp16[14];
	U8 *bp04 = &bp16[13];
	//^0CAF:0183
	bp04[0] = 0;

	do {
		//^0CAF:018A
		bp04--;
		//^0CAF:018D
		*bp04 = (bp08 % 10) + '0';
		//^0CAF:01A4
		bp08 = bp08 / 10;
		//^0CAF:01B9
	} while (bp08 != 0);
	//^0CAF:01BD
	if (value < 0) {
		//^0CAF:01CB
		*(--bp04) = '-';
	}
	//^0CAF:01D5
	return SK_STRCPY(str, bp04);
}

//^2759:0155
U16 SkWinCore::_2759_0155(ObjectID rl)
{
	//^2759:0155
	ENTER(4);
	//^2759:015A
	ObjectID si = rl;
	//^2759:015D
	if (si == OBJECT_NULL) {
		//^2759:0160
		//^2759:01DE
		return 0;
	}
	//^2759:0162
	U8 bp01 = QUERY_CLS1_FROM_RECORD(si);
	U8 bp02 = QUERY_CLS2_FROM_RECORD(si);
	U8 bp03 = 0x08;
	//^2759:017A
	for (; bp03 < 12; bp03++) {
		//^2759:017C
		if (true
			&& QUERY_GDAT_ENTRY_IF_LOADABLE(bp01, bp02, dtText, bp03) != 0
			&& QUERY_CMDSTR_ENTRY(bp01, bp02, bp03, CnCM) != 0
			&& QUERY_CMDSTR_ENTRY(
				QUERY_CLS1_FROM_RECORD(si),
				QUERY_CLS2_FROM_RECORD(si),
				bp03,
				CnNC
				) != 0
		) {
			//^2759:01D0
			return 1;
		}
		//^2759:01D5
	}
	//^2759:01DE
	return 0;
}

//^24A5:0B0F
// SPX: _24a5_0b0f renamed DRAW_ITEM_STATS_BAR
void SkWinCore::DRAW_ITEM_STATS_BAR(U16 rectno, i16 curVal, i16 maxVal, U8 chr, U16 color)
{
	//^24A5:0B0F
	ENTER(10);
	//^24A5:0B14
	i16 si = curVal;
	//^24A5:0B17
	SRECT bp08;
	// bp08: x
	// bp06: y
	// bp04: cx
	// bp02: cy
	if (QUERY_EXPANDED_RECT(rectno, &bp08) == 0)
		//^24A5:0B2B
		//^24A5:0C32
		return;
	//^24A5:0B2E
	si = (i32(si) << 11) / maxVal;
	//^24A5:0B47
	DRAW_POWER_STAT_BAR(si, rectno, color, 0, 1);
	//^24A5:0B5A
	U8 bp0a[2];
	bp0a[1] = 0;
	bp0a[0] = chr;	// Will be the rune associated with the power bar, i.e. KU for weapon
	//^24A5:0B64
	bp08.cy -= 2;
	//^24A5:0B68
	DRAW_STRONG_TEXT(
		_4976_4c16,
		-1,
		_4976_00f6,
		bp08.x -9,
		bp08.y +bp08.cy,
		glbPaletteT16[color],
		glbPaletteT16[COLOR_BLACK] | 0x4000,
		bp0a
		);
	//^24A5:0BA8
	bp0a[0] = RUNE_LO;	// 0x60
	//^24A5:0BAC
	DRAW_STRONG_TEXT(
		_4976_4c16,
		-1,
		_4976_00f6,
		bp08.x +3,
		bp08.y +bp08.cy -1,
        glbPaletteT16[COLOR_WHITE],
		glbPaletteT16[COLOR_BLACK] | 0x4000,
		bp0a
		);
	//^24A5:0BEA
	bp0a[0] = RUNE_MON;	// 0x65
	//^24A5:0BEE
	DRAW_STRONG_TEXT(
		_4976_4c16,
		-1,
		_4976_00f6,
		bp08.x +bp08.cx -1 - _4976_011e -1,
		bp08.y +bp08.cy -1,
		glbPaletteT16[COLOR_WHITE],
		glbPaletteT16[COLOR_BLACK] | 0x4000,
		bp0a
		);
	//^24A5:0C32
	return;
}

//^24A5:0C35
U16 SkWinCore::DRAW_ITEM_SURVEY(ObjectID recordLink, Bit16u xx)
{
	// You click eye icon with the item which you wanna know about, in item inventory.

	//^24A5:0C35
	ENTER(148);
	//^24A5:0C3B
	ObjectID si = recordLink;
	//^24A5:0C3E
	ItemStatBarPos bp0e = glbItemStatBarPos;
	//^24A5:0C4F
	if (si == OBJECT_NULL)
		//^24A5:0C54
		return 0;
	//^24A5:0C59
	GenericRecord *_bp04 = GET_ADDRESS_OF_RECORD(si);
	//^24A5:0C66
	DRAW_GUIDED_STR(_4976_3d7d);
	//^24A5:0C70
	U16 bp08 = si.DBType();
	//^24A5:0C7B
	if (bp08 == dbScroll) {
		//^24A5:0C80
		DRAW_SCROLL_TEXT(si);
		//^24A5:0C86
		//^24A5:0E7B
		return 1;
	}
	//^24A5:0C89
	glbInventorySubpanel = 3;
	//^24A5:0C8F
	if (xx == 0) {
		//^24A5:0C95
		//^24A5:0C54
		return 0;
	}
	//^24A5:0C97
	DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, 0x01, 0x01ee, -1);
	//^24A5:0CAA
	U8 bp09;
	U8 bp0a;
	if (QUERY_GDAT_ENTRY_IF_LOADABLE(
		bp09 = QUERY_CLS1_FROM_RECORD(si),
		bp0a = QUERY_CLS2_FROM_RECORD(si),
		dtImage,
		0x11
		) != 0
	) {
		//^24A5:0CD0
		DRAW_STATIC_PIC(bp09, bp0a, 0x11, 0x01ee, 12);
	}
	//^24A5:0CE7
	DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, 0x05, 0x01f8, 12);
	//^24A5:0CFA
	DRAW_VP_RC_STR(0x1fa, glbPaletteT16[COLOR_LIGHTER_GRAY], GET_ITEM_NAME(si));
	//^24A5:0D19
	DRAW_ITEM_ICON(si, 0x2e, 0, 0, 0);
	//^24A5:0D2A
	U16 bp06 = QUERY_ITEM_WEIGHT(si);
	//^24A5:0D34
	glbItemWeightKg = bp06 / 10;
	//^24A5:0D3E
	glbItemWeightDg = bp06 % 10;
	//^24A5:0D49
	U8 bp0094[128];
    DRAW_GUIDED_STR(QUERY_GDAT_TEXT(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, 0x1f, bp0094));			
	//^24A5:0D65
	if (bp08 == dbContainer) {
		//^24A5:0D6B
		if (IS_CONTAINER_CHEST(si) != 0) {
			//^24A5:0D76
			DRAW_CONTAINER_SURVEY(_bp04->castToContainer());
			//^24A5:0D82
			//^24A5:0E7B
			return 1;
		}
		//^24A5:0D85
		if (IS_CONTAINER_MONEYBOX(si) == 0) {
			//^24A5:0D90
			//^24A5:0E7B
			return 1;
		}
		//^24A5:0D93
		MONEY_BOX_SURVEY(si);	//
		//^24A5:0D99
		return 1;
	}
	//^24A5:0D9C
	U16 bp10 = 0;
	//^24A5:0DA1
	if (_2759_0155(si) != 0) {
		//^24A5:0DAC
		i16 di = ADD_ITEM_CHARGE(si, 0);
		//^24A5:0DB8
		if (di != 0) {
			//^24A5:0DBC
			DRAW_ITEM_STATS_BAR(
				bp0e.pos[bp10++],
				di,
				GET_MAX_CHARGE(si),
				RUNE_GOR,
				COLOR_CYAN
				);
		}
	}
//--- SPX: Info about the power bars displayed for items
// For weapons, power is displayed either using STRENGTH value (KU symbol) or THROW value (ROS symbol) for missiles such as arrows and shooters such as bows.
//	Clothings and armours also display a KU symbol for their armour strength.
//	A potion have a NETA symbol for its power level.
//	These bars are displayed in BROWN color.

//	An item with charges will have a CYAN GOR bar with the number of remaining charges


	//^24A5:0DE0
	U8 rune;		// bp11 This holds the rune to be displayed along the power bar
	U16 max;	//bp14
	i16 value;		//di
	switch (si.DBType()) {
		case dbPotion: // 8
			{
				Potion *bp04 = _bp04->castToPotion();

				//^24A5:0DFA
				rune = RUNE_NETA;
				//^24A5:0DFE
				value = bp04->PotionPower();
				//^24A5:0E0A
				max = 0x00ff;
				//^24A5:0E0F
				break;
			}
		case dbWeapon: // 5
			{
				//^24A5:0E11
				rune = RUNE_KU;
				max = 0x0050;
				//^24A5:0E1A
				value = QUERY_GDAT_DBSPEC_WORD_VALUE(si, GDAT_ITEM_WEAPON_MELEE_STRENGTH);		//SPX: 0x08 Attack strength
				//^24A5:0E26
				if (value == 0) {
					//^24A5:0E2A
					value = QUERY_GDAT_DBSPEC_WORD_VALUE(si, GDAT_ITEM_WEAPON_THROW_STRENGTH);	//SPX: 0x09 Throw strength
					//^24A5:0E36
					rune = RUNE_ROS;
					max = 0x0064;
				}
				//^24A5:0E3F
				break;
			}
		case dbCloth: // 6
			{
				//^24A5:0E41
				rune = RUNE_KU;
				if (SkCodeParam::bUseDM2ExtendedMode)
					rune = RUNE_YA;
				//^24A5:0E45
				value = QUERY_GDAT_DBSPEC_WORD_VALUE(si, GDAT_ITEM_STATS_ARMOR_CLASS) & 0xff;	//SPX: 0x0B Armor value
				//^24A5:0E54
				max = 0x00c8;
				break;	// SPX: fixed here this missing break
			}
		default:
			{
				//^24A5:0DF7
				//^24A5:0E7B
				value = 0;
				//return 1;
			}
	}
	//^24A5:0E59
	// SPX: Original behaviour
	if (!SkCodeParam::bUseDM2ExtendedMode) {
		if (value != 0) {
			//^24A5:0E5D
			DRAW_ITEM_STATS_BAR(bp0e.pos[bp10], value, max, rune, COLOR_BROWN);
		}
	}
	// SPX: little modif to have different colors
	else if (SkCodeParam::bUseDM2ExtendedMode && value != 0)
	{
		U16 color = COLOR_BROWN;
		switch (rune)
		{
			case RUNE_KU:	// Hand to hand weapon
				color = COLOR_RED;
				break;
			case RUNE_ROS:	// Projectile weapon
				color = COLOR_ORANGE;
				break;
			case RUNE_NETA:	// Potion
				color = COLOR_BLUE;
				break;
		}
		DRAW_ITEM_STATS_BAR(bp0e.pos[bp10], value, max, rune, color);
	}

//--- NEW CODE -------------------------------------
	// SPX: Added code to display hidden stats such as magical bonuses
	if (SkCodeParam::bUseSuperInfoEye)
	{
		char str[32];
		Bit16u	yy = 95;
		i32	itemValue = 0;
		i16 statValue = 0;
		U8 strMoneyValue[11];
		U8* pstr;

		if (!SkCodeParam::bUseDM2ExtendedMode)
			return 1;

		printf("Number of charges = %d\n", ADD_ITEM_CHARGE(si, 0));

		// Money value
		memset(strMoneyValue, 0, 10);
		statValue = QUERY_GDAT_DBSPEC_WORD_VALUE(si, GDAT_ITEM_STATS_MONEY_VALUE);
		itemValue += statValue;
		statValue = QUERY_GDAT_DBSPEC_WORD_VALUE(si, GDAT_ITEM_BONUS_MONEY_PER_CHARGE);
		itemValue += (statValue * ADD_ITEM_CHARGE(si, 0));
		SK_LTOA10(itemValue, strMoneyValue);
		pstr = strMoneyValue;
		while (*pstr != 0)
			pstr++;
		*pstr = '$';
		
		DRAW_VP_RC_STR(
			569,
			glbPaletteT16[COLOR_YELLOW],
			strMoneyValue
			);

		if ( bp10 != 0 || value != 0)
			yy += 2;
		yy += ((bp10 + (value != 0 ? 1 : 0))*12);
		
		// Bonus display
		for (U8 bonus = GDAT_ITEM_STATS_FOOD_VALUE; bonus <= GDAT_ITEM_STATS_MISSILE_SPELL; bonus++)
		{
			if (bonus == GDAT_ITEM_STATS_EQUIP_FLAGS
				|| bonus == GDAT_ITEM_STATS_ANIM_FLAGS
				|| bonus == GDAT_ITEM_STATS_ARMOR_CLASS
				|| bonus == GDAT_ITEM_WEAPON_PROJECTILE_FLAG
				|| bonus == GDAT_ITEM_WEAPON_MELEE_STRENGTH)
				continue;
			value = QUERY_GDAT_DBSPEC_WORD_VALUE(si, bonus);
			if (value != 0)
			{
				char sign = '+';
				if (value < 0)
				{
					sign = '-';
					value = -value;
				}
				sprintf(str, "%c%d %s", sign, value, getStatBonusName(bonus));
				DRAW_STRONG_TEXT(
					_4976_4c16,
					-1,
					_4976_00f6,
					100,
					yy,
					glbPaletteT16[COLOR_WHITE],
					glbPaletteT16[COLOR_BLACK] | 0x4000,
					(Bit8u*) str
					);
				yy += 7;
			}
		}
	} // SPX: End of hidden stats block

	//^24A5:0E7B
	return 1;
}

//^2E62:03B5
U16 SkWinCore::_2e62_03b5(Bit16u player, Bit16u itemNo, Bit16u yy)
{
	//^2E62:03B5
	ENTER(8);
	//^2E62:03BB
	U16 bp04 = 0;
	U16 bp06 = 0;
	//^2E62:03C5
	U16 bp08 = yy;
	//^2E62:03CB
	U16 si;
	if (player +1 != glbChampionInventory) {
		//^2E62:03D5
		if (itemNo > INVENTORY_HAND_LAST || player +1 == glbNextChampionNumber)
			//^2E62:03E5
			//^2E62:056C
			return 0;
		//^2E62:03E8
		si = (player << 1) + itemNo;
	}
	else {
		//^2E62:03F2
		si = itemNo +8;
	}
	//^2E62:03FA
	ObjectID di = glbChampionSquad[player].Possess(itemNo);
	//^2E62:040F
	if (itemNo <= INVENTORY_HAND_LAST) {	// <= 1
		//^2E62:0415
		if (player +1 == glbChampionIndex && itemNo == glbSelectedHandAction)
			//^2E62:0428
			bp04 = 1;
		//^2E62:042D
		if ((!_4976_3b74[si].b5_0_0()) != (!bp04)) {
			//^2E62:044B
			bp08 = 1;
			//^2E62:0450
			_4976_3b74[si].b5 ^= 1;
		}

	}
	//^2E62:045A
	if (itemNo <= INVENTORY_BODYPART_LAST) {	// <= 5
		//^2E62:0460
		if ((glbChampionSquad[player].bodyFlag & (1 << itemNo)) != 0)
			//^2E62:0478
			bp06 = 1;
		//^2E62:047D
		if ((!_4976_3b74[si].b5_1_1()) != (!bp06)) {
			//^2E62:049B
			bp08 = 1;
			//^2E62:04A0
			_4976_3b74[si].b5 ^= 2;
		}
	}
	//^2E62:04AA
	if (di != OBJECT_NULL) {
		//^2E62:04AF
		U8 bp01;
		if ((QUERY_GDAT_DBSPEC_WORD_VALUE(di, 0x00) & 0x8000) != 0) {
			//^2E62:04BE
			bp01 = GET_ITEM_ICON_ANIM_FRAME(di, itemNo, 1);
		}
		else {
			//^2E62:04C2
			bp01 = GET_ITEM_ICON_ANIM_FRAME(di, itemNo, 0);
		}
        //^2E62:04D3
		if (_4976_3b74[si].b3 != bp01) {
			//^2E62:04E1
			yy = 1;
			//^2E62:04E6
			_4976_3b74[si].b3 = bp01;
		}
		//^2E62:04F2
		U8 bp02 = QUERY_CLS2_FROM_RECORD(di);
		//^2E62:04FC
		if (bp02 != _4976_3b74[si].b4) {
			//^2E62:0507
			yy = 1;
			//^2E62:050C
			_4976_3b74[si].b4 = bp02;
		}
	}
	//^2E62:0515
	if (yy != 0 || bp08 != 0 || _4976_3b74[si].w6 != di) {
		//^2E62:052C
		if (di == 0xffff || si < 8)
			//^2E62:0536
			bp08 = 1;
		//^2E62:053B
		if (si < 8) {
			//^2E62:0540
			DRAW_PLAYER_3STAT_PANE(player, 0);
		}
		//^2E62:054B
		_4976_3b74[si].w6 = di;
		//^2E62:0554
		DRAW_ITEM_ICON(di, si, bp06, bp04, bp08);
		//^2E62:0567
		return 1;
	}
	//^2E62:056C
	return 0;
}

//^3929:0C37
void SkWinCore::DRAW_LOCAL_TEXT(Bit16u rectno, Bit16u clr1, Bit16u clr2, Bit8u *str)
{
	// draw local positioned text.
	// a) health, mana and stamina: "770/770"

	//^3929:0C37
	ENTER(12);
	//^3929:0C3B
	i16 bp02;
	i16 bp04;
	if (QUERY_STR_METRICS(str, &bp02, &bp04) != 0) {
		//^3929:0C56
		SRECT bp0c;
		if (QUERY_BLIT_RECT(NULL, &bp0c, rectno, &bp02, &bp04, -1) != 0)
		{
			//^3929:0C7A
			// SPX: Note, the bp0c.y does not match exactly the character sheet. (for static texts)
			// The charsheet image can be changed.
			bp0c.y -= 1;
			DRAW_STRONG_TEXT(
				_4976_4c16,
				-1,
				_4976_00f6,
				bp0c.x,
				bp0c.y +bp0c.cy,
				clr1,
				clr2,
				str
				);
		}
	}
	//^3929:0CA6
	return;
}


//^0CD5:0018
void SkWinCore::FILL_STR(Bit8u *buff, Bit16u count, Bit8u value, Bit16u delta)
{
	//^0CD5:0018
	//^0CD5:0021
	for (Bit16u si = 0, di = 0; di < count; si += delta, di++) {
		buff[si] = value;
	}
}

//^2E62:0265
U8 *SkWinCore::FMT_NUM(U16 value, U16 clean, U16 keta)
{
	//^2E62:0265
	ENTER(6);
	//^2E62:026A
	U16 si = value;
	//^2E62:026D
	if (clean != 0) {
		//^2E62:0273
		FILL_STR(glbStrBufferNumber, 4, ' ', 1);
	}
	//^2E62:0285
	U8 *bp04 = &glbStrBufferNumber[4];
	//^2E62:028D
	*bp04 = 0;
	//^2E62:0294
	if (si == 0) {
		//^2E62:0298
		*(--bp04) = '0';
		//^2E62:02A2
	}
	else {
		//^2E62:02C5
		U16 bp06;
		while ((bp06 = si) != 0) {
			//^2E62:02A4
			bp04--;
			//^2E62:02A7
			si = si / 10;
			//^2E62:02B2
			*bp04 = U8(bp06) + '0' -(si * 10);
			//^2E62:02C5
		};
	}
	//^2E62:02CE
	if (clean != 0) {
		//^2E62:02D4
		return &glbStrBufferNumber[4 - keta];
	}
	//^2E62:02E1
	return bp04;
}

//^0CAF:00A4
U8 *SkWinCore::SK_STRCPY(U8 *strTo, const U8 *strFrom) {
	return reinterpret_cast<U8 *>(strcpy(reinterpret_cast<char *>(strTo), reinterpret_cast<const char *>(strFrom)));
}

//^0CAF:0032
U8 *SkWinCore::SK_STRCAT(U8 *strTo, const U8 *strFrom) {
	return reinterpret_cast<U8 *>(strcat(reinterpret_cast<char *>(strTo), reinterpret_cast<const char *>(strFrom)));
}

//^24A5:1532
// SPX: _24a5_1532 replaced by DRAW_SKILL_PANEL
void SkWinCore::DRAW_SKILL_PANEL()
{
	//^24A5:1532
	ENTER(144);
	//^24A5:1538
	U16 di = glbChampionInventory -1;
	//^24A5:153E
	Champion *bp04 = &glbChampionSquad[di];
	//^24A5:154C
	glbInventorySubpanel = 2;
	//^24A5:1552
	DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, 0x01, 494, -1);	// blank stone panel
	//^24A5:1565
	i16 bp0c;
	i16 bp10;
	QUERY_TOPLEFT_OF_RECT(557, &bp0c, &bp10);
	//^24A5:157A
	U8 bp0090[128];
	U16 bp06;

	if (SkCodeParam::bUseSuperInfoEye && SkCodeParam::bUseDM2ExtendedMode)
	{
		glbPanelStatsYDelta = 6;		// Instead of 7, it is Y delta between each attribute text string
	}

	for (bp06 = 0; bp06 <= 3; bp06++) {
		//^24A5:1582
		i16 si = min_value(SkLvlArchmaster, QUERY_PLAYER_SKILL_LV(di, bp06, 1));	// skill level caped 16
		//^24A5:159C
		if (si != SkLvlNone) {	// (si != 1)
			//^24A5:15A1
			_4976_52e4 = bp06;
			//^24A5:15A7
			QUERY_GDAT_TEXT(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00
				, tSkillLevelNameGDAT2Entry[RCJ(15,si -SkLvlNeophyte)], bp0090);	// [RCJ(15,si -2)]
			//^24A5:15BE
			DRAW_VP_STR(bp0c, bp10, glbPaletteT16[(glbChampionMajorSkillsLevel[di][bp06] != 0) ? COLOR_LIGHT_GREEN : COLOR_LIGHTER_GRAY], bp0090);
			//^24A5:15F5
			glbChampionTable->lastName[9 +bp06] = glbChampionMajorSkillsLevel[di][bp06];
			//^24A5:1608
			bp10 += glbPanelStatsYDelta;
		}
		//^24A5:160E
	}

	//^24A5:161A
	i16 bp0e;
	QUERY_TOPLEFT_OF_RECT(559, &bp0e, &bp10);
	//^24A5:162F
	// SPX: Display attributes (STR, DEX, WIS, VIT, AF, AM)
	// SPX: The first is luck; the GDAT2 resource 07-00 20-00-00 (TXT) must be defined as "LUCK".
	U16 attributeStart = ATTRIBUTE_FIRST+1;	// Start at 1=STR
	if (SkCodeParam::bShowLuck || (SkCodeParam::bUseSuperInfoEye && SkCodeParam::bUseDM2ExtendedMode))
	{
		attributeStart = ATTRIBUTE_FIRST;	// Start at 0=LUCK
	}

	for (bp06 = attributeStart; bp06 <= ATTRIBUTE_LAST; bp06++) {
		//^24A5:1637
		DRAW_VP_STR(
			bp0c,
			bp10,
			glbPaletteT16[COLOR_LIGHTER_GRAY],
			QUERY_GDAT_TEXT(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, bp06 +0x20, bp0090)
			);
		//^24A5:166A
		U16 si = GET_PLAYER_ABILITY(bp04, bp06, 0);			// Current
		//^24A5:167F
		U16 bp0a = GET_PLAYER_ABILITY(bp04, bp06, 1);		// Max
		//^24A5:1695
		U16 bp08 = (si < bp0a) ? 8 : ((si > bp0a) ? 7 : 13);
		// SPX: 8 = red, 7 = green, 13 = grey
		//^24A5:16B2
		DRAW_VP_STR(
			bp0e,
			bp10,
			glbPaletteT16[bp08],
			FMT_NUM(si, 1, 3)
			);
		//^24A5:16DC
		//SK_STRCPY(bp0090, &_4976_3d7f);	// SPX: What's this? We do want to concat /
		SK_STRCPY(bp0090, strSlash);		// SPX: Fixed version with /
		//^24A5:16EE
		SK_STRCAT(bp0090, FMT_NUM(bp0a, 1, 3));
		//^24A5:170D
        DRAW_VP_STR(
			_4976_0126 * 3 +bp0e,
			bp10,
			glbPaletteT16[COLOR_LIGHTER_GRAY],
			bp0090
			);
		//^24A5:1737
		bp10 += glbPanelStatsYDelta;
		//^24A5:173D
	}
	//^24A5:1749
	return;
}

//^2E62:0670
void SkWinCore::REFRESH_PLAYER_STAT_DISP(i16 player)
{
	//^2E62:0670
	ENTER(164);
	//^2E62:0676
	Bit16u bp14 = 0;
	Bit16u bp16 = 0;
	Bit16u bp18 = 0;
	Bit16u bp1a = 0;
	Bit16u bp1c = 0;
	//^2E62:068F
	Champion *champion = &glbChampionSquad[player];	//*bp04
	//^2E62:06A0
	Bit16u si = champion->heroFlag;
	//^2E62:06A7
	Bit16u bp08 = (player +1 == glbChampionInventory) ? 1 : 0; // 1=draw entire inventory panel
	//^2E62:06BB
	if (bp08 != 0 && _4976_022c != 0)
		//^2E62:06C6
		si |= 0x8000;
	//^2E62:06CA
	if (glbChampionInventory != glbLastChampionInventory || (si & 0x8000) != 0) {
		//^2E62:06D9
		if (bp08 != 0) {
			//^2E62:06DF
			DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, 0x00, 0x04, -1);	// char sheet general screen
			//^2E62:06F1
			bp1c = 1;
			bp18 = 1;
			//^2E62:06FB
			glbLastChampionInventory = glbChampionInventory;
			//^2E62:0701
			bp14 = 15;
			//^2E62:0706
			si |= 0x7c00;
		}
		else {
			//^2E62:070C
			glbLastChampionInventory = 0;
		}
	}
	//^2E62:0714
	else if (bp08 != 0) {
		//^2E62:071A
		bp14 = _4976_581a ^ _4976_5804;
	}
	//^2E62:0724
	if (bp14 != 0) {
		//^2E62:072A
		if (glbNextChampionNumber == 0) {
			//^2E62:0731
			if ((bp14 & 1) != 0) {
				//^2E62:0738
				DRAW_CHARSHEET_OPTION_ICON(0x0013, 0x0267, 0x0001);	// 0x13: char name bar
			}
			//^2E62:074A
			if ((bp14 & 8) != 0) {
				//^2E62:0751
				DRAW_CHARSHEET_OPTION_ICON(0x000f, 0x0232, 0x0008);	// 0x0F: blue diskette
			}
			//^2E62:075F
			if ((bp14 & 4) != 0) {
				//^2E62:0766
				DRAW_CHARSHEET_OPTION_ICON(0x000d, 0x0234, 0x0004);	// 0x0D: Zzz
			}
		}
		//^2E62:0774
		if ((bp14 & 2) != 0) {
			//^2E62:077B
			DRAW_CHARSHEET_OPTION_ICON(0x000b, 0x0236, 0x0002);		// 0x0B: Cancel red cross

		}
		//^2E62:0789
		bp18 = 1;
		//^2E62:078E
		_4976_5804 = _4976_581a;
	}
	//^2E62:0794
	if ((si & 0x4000) != 0) {
		//^2E62:079A
		DRAW_PLAYER_3STAT_PANE(player, 1);
		//^2E62:07A5
		bp1a = 1;
		//^2E62:07AA
		if (champion->curHP() != 0) {
			//^2E62:07B4
			if (bp08 != 0) {
				//^2E62:07BA
				DRAW_CHAMPION_PICTURE(player);
				//^2E62:07C2
				si |= 0x0800;
			}
			else {
				//^2E62:07C8
				si |= 0x0c00;
			}
			//^2E62:07CC
			bp16 = 1;
		}
		else {
			//^2E62:07D3
			DRAW_NAME_STR(
				&_4976_3ff0,
				player +165,
				glbPaletteT16[COLOR_WHITE],
				glbPaletteT16[COLOR_DARK_GRAY] |0x4000,
				champion->firstName
				);
		}
	}
	//^2E62:0801
	if (champion->curHP() != 0) {
		//^2E62:080E
		if ((si & 0x0800) != 0) {	// Refresh stats bar
			//^2E62:0817
			DRAW_PLAYER_3STAT_PANE(player, 0);
			//^2E62:0822
			bp1a = 1;
			//^2E62:0827
			DRAW_PLAYER_3STAT_HEALTH_BAR(player);
			//^2E62:082F
			if (champion->timerIndex != TIMER_NONE) {
				//^2E62:0839
				DRAW_PLAYER_DAMAGE(player);
			}
			//^2E62:0841
			if (bp08 != 0) {
				//^2E62:084A
				SRECT bp24;
				QUERY_EXPANDED_RECT(549, &bp24);
				//^2E62:085A
				DRAW_DIALOGUE_PICT(
					QUERY_GDAT_IMAGE_ENTRY_BUFF(0x07, 0x00, 0x00),
					_4976_4c16,
					&bp24,
					bp24.x,
					bp24.y,
					-1,
					QUERY_GDAT_IMAGE_LOCALPAL(0x07, 0x00, 0x00)
					);
				//^2E62:0897
				DRAW_PLAYER_3STAT_TEXT(champion);
				//^2E62:08A3
				// SPX: bp0d holds the color number (rectangle?) around the mouth or eye.
				Bit8u colorRectangle; //bp0d;
#if (DM2_EXTENDED_MODE == 0)
				if (champion->curFood() < 0 || champion->curWater() < 0 || champion->PoisonValue != 0) {
#elif (DM2_EXTENDED_MODE == 1)
				if (champion->curFood() < 0 || champion->curWater() < 0 || champion->PoisonValue != 0 || champion->PlagueValue != 0) {
#endif
					//^2E62:08BB
					colorRectangle = 5; // i am hunger! or poisoned!
				}
				else {
					//^2E62:08C1
					colorRectangle = 4;
				}
				//^2E62:08C5
				// SPX: Display colored rectangle around mouth
				DRAW_EYE_MOUTH_COLORED_RECTANGLE(colorRectangle, 545);
				//^2E62:08D2
				colorRectangle = 4;
				//^2E62:08D6
				// SPX: review skills from STR to AM
				for (i16 di = ATTRIBUTE_STRENGTH; di <= ATTRIBUTE_LAST; di++) {	// for (i16 di = 1; di <= 6; di++)
					//^2E62:08DB
					if (champion->attributes[di][ATTRIBUTE_CURRENT] < champion->attributes[di][ATTRIBUTE_MAX]) {
						//^2E62:08F7
						colorRectangle = 5; // level gained!
						// SPX: bp0d = 5 changes color to red
						//^2E62:08FB
						break;
					}
					//^2E62:08FD
				}
				//^2E62:0903
				// SPX: Display colored rectangle around eye
				DRAW_EYE_MOUTH_COLORED_RECTANGLE(colorRectangle, 546);
				//^2E62:0910
				bp18 = 1;
			}
		}
		//^2E62:0915
		ObjectID bp12;
		if (bp08 != 0) {
			//^2E62:091E
			if (glbShowItemStats != 0) {
				//^2E62:0925
				bp12 = glbLeaderHandPossession.object;
			}
			//^2E62:092A
			else if (_4976_4bfe != 0) {
				//^2E62:0931
				bp12 = OBJECT_NULL;
			}
			else {
				//^2E62:0938
				bp12 = champion->Possess(INVENTORY_HAND_LEFT);
			}
			//^2E62:0940
			//^2E62:0943
			if (false
				|| _4976_3d30 != bp12 
				|| _4976_4bfe != _4976_5806 
				|| glbShowItemStats != _4976_5808
				|| (si & 0x2000) != 0
			) {
				//^2E62:0966
				DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, 0x25, 545, -1);	// Mouth
				//^2E62:0979
				DRAW_STATIC_PIC(GDAT_CATEGORY_INTERFACE_CHARSHEET, 0x00, (glbShowItemStats != 0) ? 0x21 : 0x20, 546, -1); // Eye
				//^2E62:0998
				if (glbShowItemStats != 0) {
					//^2E62:099F
					if (bp12 == OBJECT_NULL) {
						//^2E62:09A5
						DRAW_SKILL_PANEL();
					}
					else {
						//^2E62:09AC
						DRAW_ITEM_SURVEY(bp12, 1);
					}
				}
				else {
					//^2E62:09BA
					if (glbNextChampionNumber != 0) {
						//^2E62:09C1
						if (_4976_4bfe == 0) {
							//^2E62:09CA
							DRAW_CRYOCELL_LEVER(0);
						}
						else {
							//^2E62:09C8
							goto _09e4;
						}
					}
					//^2E62:09D4
					else if (DRAW_ITEM_SURVEY(bp12, 0) == 0) {
						//^2E62:09E4
_09e4:
						DRAW_FOOD_WATER_POISON_PANEL();
					}
				}
				//^2E62:09E9
				_4976_3d30 = bp12;
				_4976_5808 = glbShowItemStats;
				_4976_5806 = _4976_4bfe;
				//^2E62:09FB
				goto _0a25;
			}
			else if (_4976_3d30 != OBJECT_NULL) {
				//^2E62:0A04
				if ((QUERY_GDAT_DBSPEC_WORD_VALUE(_4976_3d30, 0x00) & 0x0020) != 0) {
					//^2E62:0A16
					DRAW_ITEM_SURVEY(_4976_3d30, glbShowItemStats);
					//^2E62:0A25
_0a25:
					bp18 = 1;
				}
			}
			//^2E62:0A2A
			for (i16 bp0c = 0; bp0c < 30; bp0c++) {
				//^2E62:0A31
				bp18 |= _2e62_03b5(player, bp0c, bp1c);
				//^2E62:0A44
			}
		}
		else {
			//^2E62:0A4F
			for (i16 bp0c = 0; bp0c <= 1; bp0c++) {
				//^2E62:0A56
				bp1a |= _2e62_03b5(player, bp0c, bp16);
				//^2E62:0A69
			}
		}
		//^2E62:0A72
		U16 di;
		Bit8u bp00a4[128];
		if ((si & 0x0400) != 0) {
			//^2E62:0A7B
			di = glbPaletteT16[(player == glbChampionLeader && glbNextChampionNumber == 0) ? COLOR_ORANGE : COLOR_WHITE];
			if (SkCodeParam::bDM1Mode)
				di = glbPaletteT16[(player == glbChampionLeader && glbNextChampionNumber == 0) ? COLOR_YELLOW : COLOR_ORANGE];
			//^2E62:0AA0
			if (bp08 != 0) {
				//^2E62:0AA9
				SK_STRCPY(bp00a4, &champion->firstName[0]);
				//^2E62:0ABD
				U8 bp09 = champion->lastName[0];
				if (bp09 != '.' && bp09 != ';' && bp09 != '-') {
					//^2E62:0AD9
					SK_STRCAT(bp00a4, _4976_4756);
				}
				//^2E62:0AEF
				SK_STRCAT(bp00a4, &champion->firstName[8]);
				DRAW_LOCAL_TEXT(553, di, (glbNextChampionNumber != 0) ? glbPaletteT16[COLOR_DARK_GRAY] : 0x4000, bp00a4);
			}
			else {
				//^2E62:0B37
				DRAW_PLAYER_3STAT_PANE(player, 0);
				bp1a = 1;
				DRAW_NAME_STR(&_4976_3ff0, player +165, di, glbPaletteT16[COLOR_DARK_GRAY] | 0x4000, champion->firstName);
			}
		}
		//^2E62:0B6F
		if (bp08 != 0 && (si & 0x1000) != 0) {
			//^2E62:0B81
			SRECT bp24;
			QUERY_EXPANDED_RECT(554, &bp24);
			//^2E62:0B91
			DRAW_DIALOGUE_PICT(
				QUERY_GDAT_IMAGE_ENTRY_BUFF(0x07, 0x00, 0x00),
				_4976_4c16,
				&bp24,
				bp24.x,
				bp24.y,
				-1,
				QUERY_GDAT_IMAGE_LOCALPAL(0x07, 0x00, 0x00)
				);
			//^2E62:0BCE
			U16 bp10 = GET_PLAYER_WEIGHT(player);
			//^2E62:0BDA
			di = MAX_LOAD(champion);
			//^2E62:0BE9
			U16 bp06;
			if (di < bp10) {
				//^2E62:0BEE
				bp06 = 8;
			}
			else {
				//^2E62:0BF5
				bp06 = ((i32(bp10) << 3) > (i16(di) * i32(5))) ? 11 : 13;
			}
			//^2E62:0C2C
			_4976_52e0 = di = bp10 / 10;
			//^2E62:0C3B
			_4976_52fa = bp10 - di * 10;
			//^2E62:0C4B
			_4976_52f6 = MAX_LOAD(champion) / 10;
			//^2E62:0C62
            QUERY_GDAT_TEXT(0x07, 0x00, 0x2a, bp00a4);
			//^2E62:0C76
			DRAW_LOCAL_TEXT(555, glbPaletteT16[bp06], glbPaletteT16[COLOR_DARK_GRAY] | 0x4000, bp00a4);
			//^2E62:0C9E
			bp18 = 1;
		}
	}
	//^2E62:0CA3
	if (bp1a != 0) {
		//^2E62:0CA9
		_0b36_0cbe(&_4976_3ff0, 1);
	}
	//^2E62:0CB7
	if (bp18 != 0) {
		//^2E62:0CBD
		_4976_022c = 0;
		//^2E62:0CC3
 		CHANGE_VIEWPORT_TO_INVENTORY(0);
	}
	//^2E62:0CCB
	champion->heroFlag &= 0x03ff;	// 0x03FF
	//^2E62:0CD4
	return;
}

//^1031:111E
void SkWinCore::_1031_111e(Bit16u xx)
{
	//^1031:111E
	ENTER(72);
	//^1031:1124
	i16 di = xx;
	//^1031:1127
	if (_4976_4df4 != 0) {
		//^1031:112E
		if (_4976_4df4 <= di) {
			//^1031:1134
			di -= _4976_4df4;
			//^1031:1138
			_4976_4df4 = 0;
		}
		else {
			//^1031:1140
			_4976_4df4 -= di;
			//^1031:113E
			return;
		}
	}
	//^1031:1147
	if (_4976_4e62 != 0 || _4976_4e48 != 0 || _4976_4e64 != 0 || _4976_4e96 == NULL)
		//^1031:116E
		return;
	//^1031:1171
	sk3f6c bp48;
	bp48.w0 = 0xffff;
	Bit8u bp05;
	do {
		//^1031:1176
		bp05 = *(_4976_4e96++);
		//^1031:1184
		i16 si;
		switch (bp05 & 0xff3f) {
			case 0:
				{
					//^1031:119A
					_4976_4df4 = *_4976_4e96;
					//^1031:11A6
					_4976_4e96++;
					//^1031:11AA
					break;
				}
			case 4:
			case 6:
				{
					//^1031:11AD
					si = _4976_4e5e +1;
					//^1031:11B3
					if (si > 2)
						//^1031:11B8
						si = 0;
					//^1031:11BA
					if (_4976_4e60 == si) {
						//^1031:11C0
						if (_4976_4e62 == 0)
							//^1031:11C7
							_4976_4e62 = 1;
						//^1031:11CD
						_4976_4e96--;
						//^1031:11D1
						goto _1534;
					}
					//^1031:11D4
					_4976_4e6c[_4976_4e5e] = glbMousePosition;
					//^1031:11ED
					_4976_4e5e = si;
					//^1031:11F1
					if ((bp05 & 0xff3f) == 4)
						//^1031:11FE
						break;

					goto _1201;
				}
			case 1:
				{
					//^1031:1201
_1201:
					_4976_4e62 = _4976_4e5e - _4976_4e60;
					//^1031:120B
					if (_4976_4e62 >= 0)
						//^1031:120F
						break;
					//^1031:1212
					_4976_4e62 += 3;
					//^1031:1217
					break;
				}
			case 2:
				{
					//^1031:121A
					_4976_4e48 = 1;
					//^1031:1220
					break;
				}
			case 3:
				{
					//^1031:1223
					_4976_4e64 = 1;
					//^1031:1229
					break;
				}
			case 5:
				{
					//^1031:122C
					if (HANDLE_UI_EVENT(&glbMousePosition) != 0) {
						//^1031:123B
						if (_4976_4e5c == 0)
							//^1031:1242
							break;
						//^1031:1245
						goto _1201;
					}
					//^1031:1247
					_4976_4e96--;
					//^1031:124B
					break;
				}
			case 7:
				{
					//^1031:124E
					Bit8u bp06 = *(_4976_4e96++);
					//^1031:125C
					Bit8u bp07 = *(_4976_4e96++);
					//^1031:126A
					Bit8u bp08 = *(_4976_4e96++);
					//^1031:1278
					Bit8u *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(bp06, bp07, bp08);
					//^1031:128F
					SRECT bp14;
					_1031_10c8(&bp48, &bp14, READ_UI16(bp04,-4), READ_UI16(bp04,-2));
					//^1031:12AB
					DRAW_ICON_PICT_BUFF(
						QUERY_GDAT_IMAGE_ENTRY_BUFF(bp06, bp07, bp08),
						&bp48,
						&bp14,
						0,
						0,
						*(_4976_4e96++),
						0,
						QUERY_GDAT_IMAGE_LOCALPAL(bp06, bp07, bp08)
						);
					//^1031:12FC
					break;
				}
			case 8:
				{
					//^1031:12FF
					si = GET_PLAYER_AT_POSITION((*(_4976_4e96++) + glbPlayerDir) & 3);
					//^1031:131C
					if (si < 0)
						//^1031:1320
						break;
					//^1031:1323
					DRAW_SQUAD_SPELL_AND_LEADER_ICON(si, 1);
					//^1031:132D
					_0b36_0cbe(&_4976_3f6c, 1);
					//^1031:133B
					break;
				}
			case 9:
				{
					//^1031:133E
					if (glbChampionIndex > 0) {
						//^1031:1345
						si = glbChampionIndex -1;
					}
					//^1031:134D
					//else if (glbMousePosition.event >= 0x74 && glbMousePosition.event <= 0x7b) {
					else if (glbMousePosition.event >= UI_EVENTCODE_CHAMPION1_ACTION_HAND_RIGHT && glbMousePosition.event <= UI_EVENTCODE_CHAMPION4_ACTION_HAND_LEFT) {
						//^1031:135B
						si = (glbMousePosition.event -UI_EVENTCODE_CHAMPION1_ACTION_HAND_RIGHT) >> 1;
					}
					else {
						//^1031:1365
						//^1031:11A6
						_4976_4e96++;
						//^1031:11AA
						break;
					}
					//^1031:1368
					DRAW_HAND_ACTION_ICONS(si, *(_4976_4e96++), 1);
					//^1031:1381
					goto _148a;
				}
			case 10:
				{
					//^1031:1384
					Bit8u *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x01, 0x05, 0x08);
					//^1031:1398
					SRECT bp14;
					_1031_10c8(&bp48, &bp14, READ_UI16(bp04,-4), READ_UI16(bp04,-2));
					//^1031:13B4
					DRAW_ICON_PICT_BUFF(
						QUERY_GDAT_IMAGE_ENTRY_BUFF(0x01, 0x05, 0x08),
						&bp48,
						&bp14,
						0,
						0,
						-1,
						0,
						QUERY_GDAT_IMAGE_LOCALPAL(0x01, 0x05, 0x08)
						);
					//^1031:13EE
					_4976_4df6[0] = (glbChampionTable[glbChampionIndex].runesCount * 6) +U8(glbMousePosition.event) -5;
					//^1031:140C
					i16 bp0a;
					i16 bp0c;
					QUERY_STR_METRICS(_4976_4df6, &bp0a, &bp0c);
					//^1031:1422
					CALC_CENTERED_RECT_IN_RECT(&bp14, &bp48.rc2, bp0a, bp0c);
					//^1031:143A
					_0b36_129a(
						&bp48, 
						bp14.x, 
						bp14.y +bp14.cy -1, 
						glbPaletteT16[COLOR_BLACK], 
						glbPaletteT16[COLOR_CYAN], 
						_4976_4df6
						);
					//^1031:1467
					break;
				}
			case 11:
				{
					//^1031:146A
					DRAW_SPELL_TO_BE_CAST(1);
					//^1031:1471
					//^1031:14D4
					goto _148a;
				}
			case 12:
				{
					//^1031:1473
					DRAW_CMD_SLOT(*(_4976_4e96++), 1);
					//^1031:148A
_148a:
					_0b36_0cbe(&_4976_3f6c, 1);
					//^1031:1498
					break;
				}
			case 13:
				{
					//^1031:149A
					si = glbChampionInventory;
					//^1031:149F
					if (si != 0) {
						//^1031:14A3
						_4976_581a |= *_4976_4e96;
						//^1031:14B0
						REFRESH_PLAYER_STAT_DISP(--si);
					}
					//^1031:14BD
					//^1031:11A6
					_4976_4e96++;
					//^1031:11AA
					break;
				}
			case 14:
				{
					//^1031:14C0
					_29ee_1d03(*(_4976_4e96++));
					//^1031:14D4
					goto _148a;
				}
			case 15:
			case 16:
				{
					//^1031:14D6
					_4976_4bd5 = *_4976_4e96; _4976_4e96++;
					_4976_4bd6 = *_4976_4e96; _4976_4e96++;
					_4976_4bd4 = *_4976_4e96; _4976_4e96++;
					//^1031:1500
					_0aaf_01db(_4976_4df8, 1);
					//^1031:150D
					bp48.w0 = 0xffff;
					//^1031:1512
					_0aaf_002f();

					break;
				}
		}
		//^1031:1517
		if ((bp05 & 0x80) != 0) {
			//^1031:151D
			_4976_4e96 = NULL;
			//^1031:1529
			break;
		}
		//^1031:152B
	} while ((bp05 & 0x40) != 0);

	//^1031:1534
_1534:
	if (bp48.w0 != 0xffff) {
		//^1031:153A
		_0b36_0cbe(&bp48, 1);
	}
	//^1031:1549
	return;
}

//^1031:0F3A
// SPX _1031_0f3a renamed IBMIO_USER_INPUT_CHECK
void SkWinCore::IBMIO_USER_INPUT_CHECK() //#DS=4976
{
	//^1031:0F3A
	ENTER(14);
	//^1031:0F40
	if (_4976_4e46 == 0) {
		//^1031:0F4A
		Bit32u bp04 = glbAbsoluteTickCounter;
		//^1031:0F57
		Bit32u bp08 = bp04 - _4976_19a9;
		//^1031:0F68
		if (bp08 != 0) {
			//^1031:0F6F
			_4976_19a9 = bp04;
			//^1031:0F7C
			if (_4976_4e62 == 0 && _4976_4e48 == 0 && _4976_4e64 == 0) {
				//^1031:0F97
				while (_4976_4e96 == 0) {
					//^1031:0F9D
					_4976_19a7 = 1;
					Bit16u di = 1;
					//^1031:0FA6
_0fa6:
					if (_4976_4e00 == 0) {
						//^1031:0FAD
						_4976_19a7 = 0;
						//^1031:0FB3
						_1031_0b7e();
						//^1031:0FB7
						break;
					}
					//^1031:0FBA
					MouseState bp0e = _4976_4e02[_4976_4ea6];
					//^1031:0FD4
					_4976_4e00--;
					_4976_4ea6++;
					//^1031:0FDC
					if (_4976_4ea6 > 10)
						//^1031:0FE4
						_4976_4ea6 = 0;
					//^1031:0FEA
					Bit16u si = 0;
					//^1031:0FEC
					if (bp0e.MouseButton() >= 0x20) {
						//^1031:0FF2
						if (bp0e.MouseButton() == 0x20) {
							//^1031:0FF8
							si = _1031_03f2(&_4976_1891[_4976_19ad], bp0e.MouseX());
						}
						//^1031:100F
						else if (bp0e.MouseButton() == 0x40) {
							//^1031:1015
							si = 0x81;
						}
						//^1031:101A
						else if (bp0e.MouseButton() == 0x60) {
							//^1031:1020
							si = 0xe1;
						}
					}
					//^1031:1025
					else if ((bp0e.MouseButton() & 0x04) != 0) {
						si = 0xe3;
					}
					//^1031:1031
					else if ((bp0e.MouseButton() & 0x13) != 0) {
						si = _1031_030a(&_4976_1891[_4976_19ad], bp0e.MouseX(), bp0e.MouseY(), bp0e.MouseButton());
					}
					//^1031:1055
					if (si != 0) {
						//printf("ZONE %d reached!\n", si);
						//printf("----------------\n", si);
						//^1031:1059
						di = 0;
						//^1031:105B
						glbMousePosition.event = si;
						//^1031:105F
						glbMousePosition.XPos = _4976_4e66;
						glbMousePosition.YPos = _4976_4e68;
						glbMousePosition.rc6.y = _4976_4e9e.y;
						glbMousePosition.rc6.x = _4976_4e9e.x;
						glbMousePosition.rc6.cx = _4976_4e9e.cx;
						glbMousePosition.rc6.cy = _4976_4e9e.cy;
					}
					//^1031:1083
					if (di != 0)
						//^1031:1087
						goto _0fa6;
					//^1031:108A
					_4976_19a7 = 0;
					//^1031:1090
					_1031_0b7e();
					//^1031:1094
					_4976_4e96 = TRANSMIT_UI_EVENT(&glbMousePosition);
					//^1031:10A5
					bp08 = 0;
					//^1031:10AF
				}
			}
			//^1031:10BB
			_1031_111e(Bit16u(bp08));
		}
	}
	//^1031:10C4
	return;
}

//^12B4:011E
void SkWinCore::RESET_SQUAD_DIR()
{
	//^12B4:011E
	ENTER(0);
	//^12B4:0122
	for (U16 si = 0; si < glbChampionsCount; si++) {
		//^12B4:0126
		glbChampionSquad[si].playerDir(U8(glbPlayerDir));
	}
	//^12B4:013E
	return;
}

//^12B4:00AF
// SPX TODO related to rotate squad
void SkWinCore::_12b4_00af(U16 xx)
{
	//^12B4:00AF
	ENTER(0);
	//^12B4:00B2
	MOVE_RECORD_TO(OBJECT_NULL, glbPlayerPosX, glbPlayerPosY, -1, 0);
	glbMapToLoad = LOCATE_OTHER_LEVEL(glbPlayerMap, (xx != 0) ? -1 : +1, &glbPlayerPosX, &glbPlayerPosY, NULL);
	CHANGE_CURRENT_MAP_TO(glbMapToLoad);
	ROTATE_SQUAD(_0cee_06dc_GET_TILE_DIRECTION(glbPlayerPosX, glbPlayerPosY));
	CHANGE_CURRENT_MAP_TO(glbPlayerMap);
	//^12B4:011C
	return;
}

//^3A15:0F07
U16 SkWinCore::GET_ORNATE_ANIM_LEN(Actuator *ref, U16 isWall)
{
	//^3A15:0F07
	ENTER(134);
	//^3A15:0F0C
	U8 iCategory =0 ;	// bp05
	U8 iItemNumber = 0;	// bp06
	if (isWall != 0) {
		//^3A15:0F12
		iCategory = GDAT_CATEGORY_WALL_GFX; // 9
		//^3A15:0F16
		iItemNumber = GET_WALL_DECORATION_OF_ACTUATOR(ref);
	}
	else {
		//^3A15:0F23
		iCategory = GDAT_CATEGORY_FLOOR_GFX;	// 10
		//^3A15:0F27
		iItemNumber = GET_FLOOR_DECORATION_OF_ACTUATOR(ref);
	}
	//^3A15:0F37
	if (iItemNumber == 0xff)
		//^3A15:0F3D
		return 1;
	//^3A15:0F42
    U16 si = QUERY_GDAT_ENTRY_DATA_INDEX(iCategory, iItemNumber, dtWordValue, GDAT_WALL_FLOOR_ORNATE__ANIMATION_LENGTH);
	//^3A15:0F58
	if (si == 0) {
		//^3A15:0F5C
		U8 bp0086[128];	// bp0086 : hold animation sequence data
		U8 *bp04 = QUERY_GDAT_TEXT(iCategory, iItemNumber, GDAT_WALL_FLOOR_ORNATE__ANIMATION_SEQUENCE_DATA, bp0086);
		//^3A15:0F7E
		if (*bp04 != 0) {
			//^3A15:0F84
			si = SK_STRLEN(bp04);
			//^3A15:0F93
			if (si != 0)
				//^3A15:0F95
				return si;
		}
		//^3A15:0F97
		return si = 1;
	}
	//^3A15:0F9C
	return si & 0x7fff;
}

//^3A15:0FA5
void SkWinCore::TRY_ORNATE_NOISE(Actuator *ref, ObjectID rl, U16 xx, U16 yy, U16 animLen, U16 isWall)
{
	//^3A15:0FA5
	ENTER(12);
	//^3A15:0FAB
	// if (ref->SoundEffect() != 1) // original line
	if (ref->SoundEffect() != 1 && !SkCodeParam::bForceOrnateSound)
		//^3A15:0FBD
		return;
	//^3A15:0FC0
	if ((ref->ActuatorData() & 0x0100) != 0)
		//^3A15:0FCF
		return;
	//^3A15:0FD2
	U8 iCategory = 0;	// bp0b
	U8 iItemNumber = 0;	// bp0c
	if (isWall != 0) {
		//^3A15:0FD8
		iCategory = GDAT_CATEGORY_WALL_GFX;	// 9
		//^3A15:0FDC
		iItemNumber = GET_WALL_DECORATION_OF_ACTUATOR(ref);
	}
	else {
		//^3A15:0FE9
		iCategory = GDAT_CATEGORY_FLOOR_GFX;	// 10
		//^3A15:0FED
		iItemNumber = GET_FLOOR_DECORATION_OF_ACTUATOR(ref);
	}
	//^3A15:0FFD
	U16 si = ((ref->ActuatorData() & 0xff) + glbGameTick + QUERY_GDAT_ENTRY_DATA_INDEX(iCategory, iItemNumber, dtWordValue, SOUND_STD_ACTIVATION)) % animLen;
	//^3A15:1044
	Timer bp0a;
	bp0a.SetMap(glbCurrentMapIndex);
	bp0a.SetTick(animLen - si + glbGameTick);
	//^3A15:106B
	bp0a.TimerType(tty5A);
	bp0a.actor = TIMER_ACTOR__00;
	//^3A15:1073
	bp0a.w8 = rl;
	bp0a.XcoordB(U8(xx));
	bp0a.YcoordB(U8(yy));
	//^3A15:1085
	QUEUE_TIMER(&bp0a);
	//^3A15:1090
	ref->ActuatorData(ref->ActuatorData() | 0x0100);
	//^3A15:10B0
	if (si == 0) {
		//^3A15:10B4
		QUEUE_NOISE_GEN2(
			iCategory,
			iItemNumber,
			SOUND_STD_ACTIVATION,
			0xfe,
			xx,
			yy,
			1,
			0x8c,
			0x80
			);
	}
	//^3A15:10D8
	return;
}

//^3A15:38B6
void SkWinCore::_3a15_38b6(U16 xx)
{
	//^3A15:38B6
	ENTER(22);
	//^3A15:38BC
	U16 bp14 = glbCurrentMapIndex;
	//^3A15:38C2
	CHANGE_CURRENT_MAP_TO(glbPlayerMap);
	//^3A15:38CC
	U16 bp10 = dunMapsHeaders[glbCurrentMapIndex].RawColumn();
	//^3A15:38E5
	U16 bp12 = dunMapsHeaders[glbCurrentMapIndex].RawRow();
	//^3A15:38FE
	U8 *bp04 = glbMapTileValue[glbCurrentMapIndex][0];
	//^3A15:391A
	OID_T *bp0c = &dunGroundStacks[dunMapTilesObjectIndexPerColumn[dunMapColumnsSumArray[glbCurrentMapIndex]]]; // U16 *bp0c
	//^3A15:3945
	for (U16 bp0e = 0; bp0e <= bp10; bp0e++) {
		//^3A15:394D
		for (U16 di = 0; di <= bp12; di++) {
			//^3A15:3952
			U16 bp16 = ((*bp04 << 5) == ttWall) ? 1 : 0;
			//^3A15:396B
			if ((*(bp04++) & 0x10) == 0)
				//^3A15:3977
				continue;
			//^3A15:397A
			ObjectID si = *(bp0c++);
			//^3A15:3984
			while (si.DBType() <= dbActuator) {
				//^3A15:3987
				if (si.DBType() == dbActuator) {
					//^3A15:3997
					Actuator *bp08 = GET_ADDRESS_OF_ACTU(si);
					//^3A15:39A4
					switch (bp08->ActuatorType()) {
						case ACTUATOR_TYPE_ARRIVAL_DEPARTURE: // 0x21 -> 'Activator, arrival & departure'
							{
								//^3A15:39BB
								if (bp08->ActionType() == 3) {
									//^3A15:39CD
									INVOKE_ACTUATOR(
										bp08,
										((bp08->RevertEffect() ^ xx) != 0) ? 1 : 0,
										0
										);
									//^3A15:39F6
									break;
								}
								//^3A15:39F8
								if ((!bp08->RevertEffect()) == xx) {
									//^3A15:3A0F
									INVOKE_ACTUATOR(
										bp08,
										bp08->ActionType(),
										0
										);
								}
								//^3A15:3A2A
								break;
							}
						case ACTUATOR_TYPE_ORNATE_ANIMATOR: // 0x2c -> 'Continuous ornate animator'
							{
								//^3A15:3A2C
								if (bp08->ActiveStatus() == 1) {
									//^3A15:3A3B
									TRY_ORNATE_NOISE(
										bp08,
										si,
										bp0e,
										di,
										GET_ORNATE_ANIM_LEN(bp08, bp16),
										bp16
										);
								}

								break;
							}
					}
				}
				//^3A15:3A61
				si = GET_NEXT_RECORD_LINK(si);
				//^3A15:3A6A
			}
			//^3A15:3A7A
		}
		//^3A15:3A83
	}
	//^3A15:3A91
	CHANGE_CURRENT_MAP_TO(bp14);
	//^3A15:3A9A
	return;
}

//^1C9A:3A6E
void SkWinCore::FILL_CAII_CUR_MAP()
{
	//^1C9A:3A6E
	ENTER(16);
#ifndef __DJGPP__
	//^1C9A:3A74
	U8 *bp04 = *glbCurrentTileMap;
	//^1C9A:3A85
	OID_T *bp08 = &dunGroundStacks[*_4976_4c52];	// U16 *bp08 
	//^1C9A:3A9E
	for (U16 bp0e = 0; bp0e < glbCurrentMapWidth; bp0e++) {
		//^1C9A:3AA6
		for (U16 di = 0; di < glbCurrentMapHeight; di++) {
			//^1C9A:3AAB
			if ((*(bp04++) & 0x10) != 0) {
				//^1C9A:3ABA
				ObjectID si = *(bp08++);
				
				do {
					//^1C9A:3AC4
					if (si.DBType() == dbCreature) {
						//^1C9A:3AD1
						Creature *bp0c = GET_ADDRESS_OF_RECORD4(si);
						//^1C9A:3AE2
						if (bp0c->b5 != 0xff)
							//^1C9A:3AE7
							break;
						//^1C9A:3AE9
						if (QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp0c->CreatureType())->IsStaticObject() == 0) {
							//^1C9A:3B02
							ALLOC_CAII_TO_CREATURE(si, bp0e, di);
							//^1C9A:3B0E
							break;
						}
						//^1C9A:3B10
						U16 bp10 = bp0c->w10;
						//^1C9A:3B1A
						_1c9a_09db(si);
						//^1C9A:3B20
						bp0c->w10 |= bp10 & 0x6000;
						//^1C9A:3B2D
						if ((bp10 & 0x803f) == 0x8001) {
							//^1C9A:3B38
							bp0c->w10 &= 0xffc0;
							bp0c->w10 |= 0x8001;
						}
						//^1C9A:3B44
						break;
					}
					//^1C9A:3B46
					si = GET_NEXT_RECORD_LINK(si);
					//^1C9A:3B4F
				//} while (si != OBJECT_END_MARKER);
				} while (si != OBJECT_END_MARKER && si != OBJECT_NULL); // SPX prevent also from object_null
			}
			//^1C9A:3B57
		}
		//^1C9A:3B61
	}
	//^1C9A:3B70
#endif // __DJGPP__	
	return;
}

//^13AE:0009
void SkWinCore::LOAD_NEWMAP(U8 newmap)
{
	//^13AE:0009
	//printf("Entering LOAD_NEWMAP %d\n", newmap); getch();
	ENTER(0);
	//^13AE:000C
	if (_4976_5c8a == 0) {
		//^13AE:0013
		FIRE_HIDE_MOUSE_CURSOR();
		//^13AE:0018
		_3a15_38b6(0);
	}
	//^13AE:0020
	//printf("LOAD_LOCALLEVEL_GRAPHICS_TABLE %d\n", newmap); getch();
	LOAD_LOCALLEVEL_GRAPHICS_TABLE(newmap);
	//^13AE:0029
	//printf("LOAD_LOCALLEVEL_DYN\n"); getch();
	LOAD_LOCALLEVEL_DYN();
	//^13AE:002E
	if (_4976_5c8a == 0) {
		//^13AE:0035
		_3a15_38b6(1);
		//^13AE:003D
		//printf("FILL_CAII_CUR_MAP\n"); getch();
		FILL_CAII_CUR_MAP();
		//^13AE:0042
		//printf("RECALC_LIGHT_LEVEL\n"); getch();
		RECALC_LIGHT_LEVEL();
		//^13AE:0047
		if (_4976_5bee[0] == 0)
			//^13AE:0050
			_1031_098e();
		//^13AE:0055
		FIRE_SHOW_MOUSE_CURSOR();
	}
	//printf("Ending LOAD_NEWMAP\n"); getch();
	//^13AE:005A
	return;
}





//^2405:04F4
// SPX: _2405_04f4 renamed DRAW_ITEM_IN_HAND
void SkWinCore::DRAW_ITEM_IN_HAND(LeaderPossession *ref)
{
	//^2405:04F4
	ENTER(8);
	//^2405:04F8
	U8 bp05 = QUERY_CLS1_FROM_RECORD(ref->object);
	//^2405:0507
	U8 bp06 = QUERY_CLS2_FROM_RECORD(ref->object);
	//^2405:0516
	U8 bp07 = GET_ITEM_ICON_ANIM_FRAME(ref->object, -1, 1);
	//^2405:052A
	U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(bp05, bp06, bp07);
	//^2405:0541
	COPY_MEMORY(
		QUERY_GDAT_IMAGE_LOCALPAL(bp05, bp06, bp07),
		ref->b6,
		16
		);
	//^2405:056D
	FIRE_BLIT_PICTURE(
		bp04,
		ref->pb2,
        ALLOC_TEMP_ORIGIN_RECT(_4976_0106, _4976_0108),
		0,
		0,
		READ_I16(bp04,-4),
		_4976_0106,
		-1,
		0,
		4,
		4,
		NULL
		);
	//^2405:05B2
	return;
}

//^00EB:069A
void SkWinCore::_00eb_069a(SRECT *prc, i16 yy)
{
	//^00EB:069A
	ENTER(0);
	//^00EB:069F
	LOADDS(0x0C48);
	i16 si = prc->y + yy;
	while (prc->y +prc->cy -1 >= si) {
		//^00EB:06B3
		MOVE_MEMORY(
			&pbVram[si * 320],
			&pbVram[(si -yy) * 320],
			prc->x +prc->cx -1 -prc->x +1
			);
		//^00EB:06F7
	}
	//^00EB:0708
	return;
}

//^44C8:1C99
// SPX: _44c8_1c99 renamed SCROLLBOX_MESSAGE
// That should roll 1 row of old message
void SkWinCore::SCROLLBOX_MESSAGE()
{
	//^44C8:1C99
	ENTER(8);
	//^44C8:1C9D
	FIRE_HIDE_MOUSE_CURSOR();
	SRECT bp08;
	QUERY_EXPANDED_RECT(RECT_BOTTOM_MESSAGE_3_LINES, &bp08);
	_00eb_069a(&bp08, _4976_0140) INDIRECT_CALL;
	bp08.y = bp08.y +bp08.cy -1 - _4976_0130 +1;
	FIRE_BLIT_PICTURE(
		_4976_5c08,
		NULL,
		&bp08,
		0,
		0,
		_4976_013e,
		glbScreenWidth,
		-1,
		0,
		8,
		8,
		NULL
		);
	FIRE_SHOW_MOUSE_CURSOR();
	//^44C8:1D0F
	return;
}

//^3929:07E1
void SkWinCore::_3929_07e1(U16 xx, U16 yy)
{
	//^3929:07E1
	ENTER(0);
	//^3929:07E6
	i16 di = xx;
	U16 si = yy;
	//^3929:07EC
	if (di < 0)
		//^3929:07F0
		di = 0;
	//^3929:07F2
	_4976_475c = min_value(di * _4976_0126, _4976_013e - _4976_0126);
	//^3929:080B
	if (si < 0 || si >= 1)
		//^3929:0816
		si = 0;
	//^3929:0818
	glbTimerTickRemoveHintMessageIndex = si;
	_4976_5c06 = 0;
	//^3929:0822
	return;
}

//^3929:0826
void SkWinCore::_3929_0826()
{
	//^3929:0826
	ENTER(0);
	//^3929:082A
	FIRE_HIDE_MOUSE_CURSOR();
	//^3929:082F
	FIRE_FILL_SCREEN_RECT(RECT_BOTTOM_MESSAGE_3_LINES, glbPaletteT16[COLOR_BLACK]);
	//^3929:0842
	FIRE_SHOW_MOUSE_CURSOR();
	//^3929:0847
	_3929_07e1(0, 0);
	//^3929:0851
	for (U16 si = 0; si < 1; si++) {
		//^3929:0855
		tlbTimerTickRemoveHintMessage[si] = -1;
		//^3929:0866
	}
	//^3929:086C
	return;
}


//^3929:0914
void SkWinCore::_3929_0914_HINT_TEXT()
{
	//^3929:0914
	ENTER(0);
	//^3929:0917
	_3929_0826();
	//^3929:091B
	tlbTimerTickRemoveHintMessage[0] = -1;
	//^3929:0927
	return;
}
/*
void SkWinCore::_3929_0914_HINT_TEXT()
{
	;
}
*/
/* --> here is implementation of 3929:0914 in Skull, more complete!
void DM2_gfxmain_3929_0914(void)
{
  if (ddat.v1e141e == 2)
  {
    if (ddat.v1e1420 != 0)
      DM2_SCROLLBOX_MESSAGE();
    DM2_FILL_ENTIRE_PICT(gfxsys.pictbuff, palettecolor_to_pixel(paldat.palette[E_COL00]));
    ddat.v1e1420 = 1;
    //m_45942:
    for (i16 i=0; i < 2; i++)
      //m_45933:
      ddat.v1e1408[i] = ddat.v1e1408[i + 1];
    ddat.v1e1408[2] = lcon(0xffffffff);
  }
  else
  {
    //m_45959:
    ddat.v1e1420 = 0;
    ddat.v1e141e++;
  }
}
 */



//^3929:0BA6
void SkWinCore::PRINT_SYSERR_TEXT(Bit16u dstx, Bit16u dsty, Bit16u clr1, Bit16u clr2, Bit8u *str)
{
	//^3929:0BA6
	DRAW_STRING(_4976_4964, -1, glbScreenWidth, dstx, dsty+1, clr1, clr2, str, 8);
}

//^3929:0929
// SPX _3929_0929 renamed DRAW_HINT_TEXT
void SkWinCore::DRAW_HINT_TEXT(U16 clr1, U8 *str)
{
	//^3929:0929
	ENTER(8);
	//^3929:092D
	SRECT bp08;
	QUERY_EXPANDED_RECT(RECT_BOTTOM_MESSAGE_3_LINES, &bp08);
	//^3929:093C
	if (_4976_5c06 != 0) {
		//^3929:0943
		DRAW_STRING(
			_4976_5c08,
			-1,
			_4976_013e,
			_4976_475c,
			_4976_0140 -2 -((QUERY_MBCS_PRESENCE(str) != 0) ? _4976_0136 : _4976_0124),
			clr1,
			glbPaletteT16[COLOR_BLACK],
			str,
			8
			);
	}
	else {
		//^3929:0996
		FIRE_HIDE_MOUSE_CURSOR();
		//^3929:099B
		PRINT_SYSERR_TEXT(
			bp08.x + _4976_475c,
			glbTimerTickRemoveHintMessageIndex * _4976_013a +bp08.y + _4976_0130 - _4976_0136 + _4976_5c12 -1,
			clr1,
            glbPaletteT16[COLOR_BLACK],
			str
			);
		//^3929:09D8
		FIRE_SHOW_MOUSE_CURSOR();
	}
	//^3929:09DD
	tlbTimerTickRemoveHintMessage[glbTimerTickRemoveHintMessageIndex] = glbGameTick + 70;	// is it a clock before message disappear ?
	//^3929:09F9
	return;
}

//^3929:09FB
// SPX: _3929_09fb renamed DISPLAY_HINT_TEXT
void SkWinCore::DISPLAY_HINT_TEXT(Bit16u color, const U8 *str) //#DS=4976
{
	//printf("HINT: %s\n", str);
	//^3929:09FB
	ENTER(102);
	//^3929:0A01
	//U16 si = color;
	U16 absoluteColor = 255;	// from 256 palette
	//^3929:0A04
	if (_4976_5c06 != 0) {	// roll message ?
		//^3929:0A0B
		SCROLLBOX_MESSAGE();
		//^3929:0A10
		FILL_ENTIRE_PICT(_4976_5c08, glbPaletteT16[COLOR_BLACK]);
		//^3929:0A2A
		_4976_5c06 = 0;
	}
	//^3929:0A30
	absoluteColor = glbPaletteT16[color];
	//^3929:0A3B
	for (U16 bp02 = 0; str[bp02] != 0; ) {
		//^3929:0A43
		U8 bp66[100]; memset(bp66,0,100);
		U16 di = _3929_04e2_DRAW_TEXT_STRINGS(str, bp66, &bp02, _4976_013e - _4976_475c);	// _4976_013e - _4976_475c somehow can move text to left (thus out of screen)
		//^3929:0A64
		if (str[bp02] == '\n') {
			//^3929:0A70
			_3929_0914_HINT_TEXT();
			//^3929:0A74
			DRAW_HINT_TEXT(absoluteColor, bp66);
			//^3929:0A81
			_4976_475c = 0;
			//^3929:0A87
			bp02++;
			//^3929:0A8A
			continue;
		}
		//^3929:0A8C
		if (str[bp02] != 0) {
			//^3929:0A98
			DRAW_HINT_TEXT(absoluteColor, bp66);
			//^3929:0AA5
			_3929_0914_HINT_TEXT();
			//^3929:0AA9
			_4976_475c = 12;
			//^3929:0AAF
			continue;
		}
		//^3929:0AB1
		DRAW_HINT_TEXT(absoluteColor, bp66);
		//^3929:0ABE
		_4976_475c += di;
		//^3929:0AC2
	}
	//^3929:0AD1
	if (_4976_5c06 != 0) {
		//^3929:0AD8
		SCROLLBOX_MESSAGE();
		//^3929:0ADD
		FILL_ENTIRE_PICT(_4976_5c08, glbPaletteT16[COLOR_BLACK]);
	}
	//^3929:0AFD
	return;
}

//^3929:0B5A
// SPX : _3929_0b5a renamed DISPLAY_HINT_NEW_LINE
void SkWinCore::DISPLAY_HINT_NEW_LINE()
{
	//^3929:0B5A
	ENTER(0);
	//^3929:0B5D
	DISPLAY_HINT_TEXT(COLOR_BLACK, const_cast<U8 *>(strEndLine));
	//^3929:0B6A
	return;
}

//^2405:00C9
// SPX: _2405_00c9 renamed DISPLAY_TAKEN_ITEM_NAME
void SkWinCore::DISPLAY_TAKEN_ITEM_NAME(ObjectID rl)
{
	//^2405:00C9
	ENTER(0);
	//^2405:00CC
	DISPLAY_HINT_NEW_LINE();
	//^2405:00D1
	DISPLAY_HINT_TEXT(COLOR_LIGHTER_GRAY, GET_ITEM_NAME(rl));
	//^2405:00E5
	return;
}


//^2E62:0CFA
void SkWinCore::_2e62_0cfa(U16 xx)
{
	//^2E62:0CFA
	ENTER(4);
	//^2E62:0CFF
	for (U16 si = 0; si < glbChampionsCount; si++) {
		//^2E62:0D03
		if (si +1 != glbChampionInventory) {
			//^2E62:0D0C
			REFRESH_PLAYER_STAT_DISP(si);
		}
		//^2E62:0D12
	}
	//^2E62:0D19
	if (glbChampionInventory != 0) {
		//^2E62:0D20
		U16 si = glbChampionInventory -1;
		//^2E62:0D26
		Champion *champion = &glbChampionSquad[si];
		//^2E62:0D34
		if (champion->enchantmentPower != 0 && champion->enchantmentAura >= ENCHANTMENT_AURA_FIRST && champion->enchantmentAura <= ENCHANTMENT_AURA_LAST) {
			//^2E62:0D4F
			champion->heroFlag |= (glbShowItemStats != 0 && glbLeaderHandPossession.object == OBJECT_NULL) ? CHAMPION_FLAG_3000 : CHAMPION_FLAG_1000;	// 0x3000 or 0x1000
		}
		//^2E62:0D6C
		REFRESH_PLAYER_STAT_DISP(glbChampionInventory -1);
	}
	//^2E62:0D76
	UPDATE_RIGHT_PANEL(xx);
	//^2E62:0D7F
	return;
}

//^443C:0004
void SkWinCore::_443c_0004(U8 *buffSrc, U8 *buffDst, U8 *localpal)
{
	//^443C:0004
	ENTER(20);
	//^443C:0009
	U8 *bp04 = ALLOC_PICT_BUFF(16, 16, afDefault, 4);
	//^443C:001F
	U8 bp14[16];
	for (i16 si = 0; si < 16; si++) {
		//^443C:0023
		bp14[si] = glbPaletteT16[COLOR_BLACK];
		//^443C:002D
	}
	//^443C:0033
	bp14[12] = localpal[12];
	//^443C:003D
	FILL_ENTIRE_PICT(buffDst, localpal[12]);
	//^443C:0052
	DRAW_DIALOGUE_PICT(buffSrc, buffDst, &_4976_4942, 0, 0, 12, bp14);
	//^443C:0075
	DRAW_DIALOGUE_PICT(buffSrc, buffDst, &_4976_494a, 0, 0, 12, localpal);
	//^443C:0099
	FREE_PICT_BUFF(bp04);
	//^443C:00A6
	return;
}

//^443C:0434
void SkWinCore::_443c_0434()
{
	//^443C:0434
	ENTER(4);
	//^443C:0438
	_4976_5dac = (glbLeaderHandPossession.object != OBJECT_NULL) ? 1 : 0;
	//^443C:0449
	if (_4976_5dac != 0) {
		//^443C:044D
		U8 *bp04 = ALLOC_PICT_BUFF(18, 18, afDefault, 8);
		//^443C:0463
		_443c_0004(glbLeaderHandPossession.pb2, bp04, glbLeaderHandPossession.b6);
		//^443C:047C
		IBMIO_SET_CURSOR_PATTERN(2, bp04, 8, 8, 18, 18, 8, NULL, glbLeaderHandPossession.b6[12]) INDIRECT_CALL;
		//^443C:04A3
		FREE_PICT_BUFF(bp04);
	}
	//^443C:04B0
	_443c_040e();
	//^443C:04B4
	return;
}






//^32CB:06CF
void SkWinCore::INIT_BACKBUFF()
{
	//^32CB:06CF
	ENTER(0);
	//^32CB:06D2
	if (_4976_5be6 != 0) {
		//^32CB:06D9
		FIRE_FILL_BACKBUFF_RECT(&_4976_5938, glbPaletteT16[COLOR_BLACK]);
	}
	//^32CB:06EF
	WRITE_UI16(_4976_4c16,-4,_4976_00f6); // width
	//^32CB:06FA
	WRITE_UI16(_4976_4c16,-2,_4976_00f8); // height
	//^32CB:0701
	WRITE_UI16(_4976_4c16,-6,8); // bpp
	//^32CB:0707
	_4976_022c = 0;
	//^32CB:070D
	return;
}

//^2759:213D
SpellDefinition *SkWinCore::FIND_SPELL_BY_RUNES(U8 *runes)
{
	// CSBwinSimilarity: TAG01c88e,Incantation2Spell

	//^2759:213D
	ENTER(10);
	//^2759:2141
	if (runes[1] == 0)
		//^2759:214B
		//^2759:21EF
		return NULL;
	//^2759:214E
	i16 bp06 = 24;
	U32 bp0a = 0;

	do {
		//^2759:215D
		bp0a |= U32(*(runes++)) << bp06;
		//^2759:2178
	} while (runes[0] != 0 && (bp06 -= 8) >= 0);
	//^2759:2187
	SpellDefinition *bp04 = const_cast<SpellDefinition *>(dSpellsTable);
	U32 iLocalMaxSpell = MAXSPELL_ORIGINAL;	// SPX added this to prevent overflow when switching tables

	// SPX : Use extended spells table read from GDAT if we are in custom mode
	if (SkCodeParam::bUseCustomSpells)
	{
		bp04 = const_cast<SpellDefinition *>(dSpellsTableCustom);
		iLocalMaxSpell = MAXSPELL_CUSTOM;
	} // end SPX

	//^2759:218F
	for (bp06 = iLocalMaxSpell; bp06-- != 0; bp04++) {	// (bp06 = MAXSPELL; bp06-- != 0; bp04++)
		//^2759:2196
		if ((bp04->dw0 & 0xff000000) != 0) {
			//^2759:21AB
			if (bp04->dw0 != bp0a)
				//^2759:21BA
				continue;
			//^2759:21BC
			return bp04;
		}
		//^2759:21C4
        if ((bp0a & 0x00ffffff) == bp04->dw0)
			//^2759:21DF
			//^2759:21BC
			return bp04;
		//^2759:21E1
	}
	//^2759:21EF
	return NULL;
}

//^1C9A:121D
ObjectID SkWinCore::ALLOC_NEW_CREATURE(U16 creaturetype, U16 healthMultiplier_1to31_baseIs8, U16 dir, U16 xx, U16 yy)
{
	// arrange for a new creature record.

	// (creature += 0x8000) if you wanna create minion map's minion creature. then new missile record is attached to the minion creature.

	//^1C9A:121D
	ENTER(12);
	//^1C9A:1223
	U16 si = healthMultiplier_1to31_baseIs8;
	//^1C9A:1226
	U16 bp0c = creaturetype & 0x8000;
	//^1C9A:122F
	ObjectID bp0a;
	if (bp0c != 0) {
		//^1C9A:1233
		creaturetype &= 0x7fff;
		//^1C9A:1238
		bp0a = ALLOC_NEW_RECORD(dbMissile);
		//^1C9A:1243
		if (bp0a == OBJECT_NULL) {
			//^1C9A:1248
			return OBJECT_NULL;
		}
	}
	//^1C9A:124E
	ObjectID di = ALLOC_NEW_RECORD(dbCreature);
	//^1C9A:1258
	if (di == OBJECT_NULL || _4976_1a68 >= glbCreaturesCount) {
		//^1C9A:1266
		if (bp0c != 0)
			//^1C9A:126C
			DEALLOC_RECORD(bp0a);
		//^1C9A:1275
		//^1C9A:1248
		return OBJECT_NULL;
	}
	//^1C9A:1277
	Creature *creature = GET_ADDRESS_OF_RECORD4(di);	//*bp04
	//^1C9A:1284
	creature->CreatureType(U8(creaturetype));
	//^1C9A:128E
	AIDefinition *bp08 = QUERY_CREATURE_AI_SPEC_FROM_TYPE(U8(creaturetype));
	//^1C9A:129B
	creature->SetPossessionObject(OBJECT_END_MARKER);
	//^1C9A:12A4
	creature->b15 = 0xfb;
	//^1C9A:12A9
	creature->b15_0_1(dir);
	//^1C9A:12B8
	creature->b5_0_7(0xff);
	//^1C9A:12BD
	if (bp0c != 0) {
		//^1C9A:12C3
		APPEND_RECORD_TO(bp0a, &creature->possession, -1, 0);
	}
	//^1C9A:12DB
	si = min_value(si, 31);
	//^1C9A:12E8
	// SPX: bp08->w4 = Hit Points
	si = (si * bp08->BaseHP) >> 3;
	//^1C9A:12F4
	creature->HP1(RAND16((si >> 3) +1) +si);
	//^1C9A:130A
	creature->HP3(0);
	//^1C9A:1310
	creature->SetTriggerXPos(xx);
	//^1C9A:131F
	creature->SetTriggerYPos(yy);
	//^1C9A:1332
	creature->SetTriggerMap(glbCurrentMapIndex);
	//^1C9A:1344
	creature->w8 = 0xffff;
	//^1C9A:134A
	if (MOVE_RECORD_TO(di, -4, 0, xx, yy) == 1) {
		//^1C9A:1362
		//^1C9A:1248
		return OBJECT_NULL;
	}
	//^1C9A:1365
	creature->w8 = 0;
	//^1C9A:136E
	return di;
}










//^0B36:1757
// SPX: ressembles pretty much DRAW_STATIC_PIC (_0B36_1688) 
// SPX: _0B36_1757 renamed DRAW_TRANSPARENT_STATIC_PIC
void SkWinCore::DRAW_TRANSPARENT_STATIC_PIC(U8 cls1, U8 cls2, U8 cls4, U16 rectno, U16 colorkey)
{
	//^0B36:1757
	ENTER(314);
	//^0B36:175B
	ExtendedPicture bp013a;
	QUERY_GDAT_SUMMARY_IMAGE(&bp013a, cls1, cls2, cls4);
	//^0B36:1774
	bp013a.colorKeyPassThrough = colorkey;
	bp013a.rectNo = rectno;
	//^0B36:1782
	DRAW_PICST(QUERY_PICST_IT(&bp013a));
	//^0B36:1796
	return;
}


//^2759:01E3
U16 SkWinCore::QUERY_CUR_CMDSTR_ENTRY(U16 cmdNum)
{
	//^2759:01E3
	ENTER(0);
	//^2759:01E6
	return QUERY_CMDSTR_ENTRY(glbItemGDATCategory, glbItemGDATIndex, glbItemGDATEntry, cmdNum);
}

//^2C1D:111E
// SPX: _2c1d_111e renamed USE_DEXTERITY_ATTRIBUTE
U16 SkWinCore::USE_DEXTERITY_ATTRIBUTE(U16 player)
{
	//^2C1D:111E
	ENTER(4);
	//^2C1D:1123
	Champion *champion = &glbChampionSquad[player];
	//^2C1D:1134
	i16 si = (RAND() & 7) + GET_PLAYER_ABILITY(champion, abDex, 0);
	//^2C1D:1153
	si -= (GET_PLAYER_WEIGHT(player) * i32(si >> 1)) / MAX_LOAD(champion);
	//^2C1D:1182
	si = max_value(2, si);
	//^2C1D:118E
	if (glbIsPlayerSleeping != 0)
		//^2C1D:1195
		si >>= 1;
	//^2C1D:1197
	return BETWEEN_VALUE((RAND() & 7) +1, (si >> 1), 100 - (RAND() & 7));
}





//^2C1D:0E23
U16 SkWinCore::_2c1d_0e23(U16 xx)
{
	//^2C1D:0E23
	ENTER(0);
	//^2C1D:0E28
	i16 di;
	U16 si = BETWEEN_VALUE(1, di = (QUERY_ITEM_WEIGHT(xx) >> 1), 10);
	//^2C1D:0E44
	while ((di -= 10) > 0) {
		//^2C1D:0E46
		si += di >> 1;
	}
	//^2C1D:0E51
	return si;
}





//^443C:08AB
void SkWinCore::_443c_08ab(i16 *xx, i16 *yy, i16 *zz)
{
	//^443C:08AB
	ENTER(0);
	//^443C:08AE
	_01b0_0d39(xx, yy, zz, 0) INDIRECT_CALL;
	//^443C:08CD
	return;
}



//==============================================================================



//^12B4:0AA5
void SkWinCore::PUSH_PULL_RIGID_BODY(U16 arrowPos) 
{
	// move rigid body like boulder

	//          0
	//  5 <--+  A  +--> 1
	//       |  |  |
	//       |  |  |
	//  4 <--+  V  +--> 2
	//          3

	//^12B4:0AA5
	ENTER(8);
	//^12B4:0AAB
	X16 di;
	X16 si;
	X16 bp02;
	switch (arrowPos) {
		default:
			//^12B4:0AB3
			return;
		case 0://^0ABD
			//^12B4:0ABD
			di = glbPlayerDir;
			si = 0;
			goto _0adf;
		case 5://^0AC5 // forward and left
			//^12B4:0AC5
			di = (glbPlayerDir -1) & 3;
			si = 1;
			goto _0adf;
		case 1://^0AD3 // forward and right
			//^12B4:0AD3
			di = (glbPlayerDir +1) & 3;
			si = 2;
			goto _0adf;
			//^12B4:0ADF
_0adf:
			bp02 = 3;
			break;
		case 3://^0AE6 // back
			//^12B4:0AE6
			bp02 = 5;
			si = 0;
			goto _0b01;
		case 4://^0AEF // back and left
			//^12B4:0AEF
			bp02 = 6;
			si = 2;
			goto _0b01;
		case 2://^0AF9 // back and right
			//^12B4:0AF9
			bp02 = 4;
			si = 1;
			//^12B4:0B01
_0b01:
			di = (glbPlayerDir +2) & 3;
			break;
	}
	//^12B4:0B0B
	X16 bp04 = glbPlayerPosX + glbXAxisDelta[glbPlayerDir];
	X16 bp06 = glbPlayerPosY + glbYAxisDelta[glbPlayerDir];
	ObjectID bp08;
	if (IS_CREATURE_MOVABLE_THERE(bp04, bp06, di, &bp08) == 0)
		return;
	//^12B4:0B46
	if (_12b4_099e(bp08) == 0) {
		//^12B4:0B52
		U16 bp02;
		for (bp02 = 0; bp02 < glbChampionsCount; bp02++) {
			//^12B4:0B59
			if (glbChampionSquad[bp02].curHP() != 0) {
				//^12B4:0B6D
				if (bp02 == glbChampionLeader || RAND02() == 0) {
					//^12B4:0B7C
					// SPX: Sound made by champion when moving large object
					QUEUE_NOISE_GEN2(GDAT_CATEGORY_CHAMPIONS, U8(bp02), SOUND_CHAMPION_GETHIT, 0xfe, glbPlayerPosX, glbPlayerPosY, 1, 0x69, 0xc8);
				}
			}
			//^12B4:0B9F
		}
	}
	else {
		//^12B4:0BAD
		glbTargetPosXTest = bp04;
		glbTargetPosYTest = bp06;
		glbPosX_4976_4eae = glbPlayerPosX;
		glbPosY_4976_4eb0 = glbPlayerPosY;
		_4976_4eac = di;
		glbMap_4976_4eaa = glbPlayerMap;
		_4976_4eb2 = si;
		glbTableToMove = bp08;
		PERFORM_MOVE(bp02);
	}
	//^12B4:0BE1
	return;
}


//^098D:0CD7
// SPX: _098d_0cd7 renamed QUERY RECTNO FOR WALL ORNATE
// ornateOffsetPos between 0 and 24
U16 SkWinCore::QUERY_RECTNO_FOR_WALL_ORNATE(i16 cellPos, U16 ornateOffsetPos, U16 zz) // (i16 xx, U16 yy, U16 zz)
{
	//^098D:0CD7
	ENTER(0);
	//^098D:0CDA
	if (zz != 0) {	// taking side ornate
		//^098D:0CE0
		return tRectnoOffsetsWallOrnates[RCJ(16, cellPos)] + ornateOffsetPos;	// limiting cellpos to 16 means taking only Distance 0 to Distance 3
	}
	//^098D:0CEE
	return cellPos * 25 + ornateOffsetPos + 3100;	// taking front ornate
}

//^32CB:08C1
void SkWinCore::QUERY_TEMP_PICST(
	U16 mirrorflip, U16 horzStretch, U16 vertStretch, i16 offx, i16 offy, i16 uu, U16 rectno, U16 pp, 
	i16 colorkey1, i16 colorkey2, U8 cls1, U8 cls2, U8 cls4)
{
	SkD((DLV_DBG_GETPIC, "DBG: QUERY_TEMP_PICST(%X,%3d,%3d,%3d,%3d,%04X,%04X,%04X,%2d,%2d,%02X,%02X,%02X)\n"
		, (Bitu)mirrorflip, (Bitu)horzStretch, (Bitu)vertStretch, (Bitu)offx, (Bitu)offy
		, (Bitu)uu, (Bitu)rectno, (Bitu)pp, (Bitu)colorkey1, (Bitu)colorkey2, (Bitu)cls1, (Bitu)cls2, (Bitu)cls4));

	//^32CB:08C1
	ENTER(2);
	//^32CB:08C7
	i16 di = uu;
	QUERY_GDAT_SUMMARY_IMAGE(&glbTempPicture, cls1, cls2, cls4);
	//^32CB:08E2
	if (horzStretch != vertStretch) {
		//^32CB:08EA
		U8 bp01;
		if (di == 2) {
			//^32CB:08EF
			bp01 = 0x14;
			//^32CB:08F3
			goto _08fe;
		}
		//^32CB:08F5
		else if (di == 3) {
			//^32CB:08FA
			bp01 = 0x15;
			//^32CB:08FE
_08fe:
			U16 si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, dtWordValue, bp01);
			//^32CB:0916
			if (si != 0) {
				//^32CB:091A
				horzStretch = ((((si >> 8) << 7) / (si & 255)) +1) >> 1;
			}
		}
	}
	//^32CB:0932
	glbTempPicture.mirrorFlip = mirrorflip;
	glbTempPicture.w52 = horzStretch;
	glbTempPicture.w54 = vertStretch;
	glbTempPicture.w28 += offx;
	glbTempPicture.w30 += offy;
	glbTempPicture.rectNo = rectno;
	glbTempPicture.w26 = pp;
	glbTempPicture.pb44 = _4976_4c16;
	glbTempPicture.colorKeyPassThrough = colorkey1;
	//^32CB:0972
	_32cb_0804(glbTempPicture.b58, di, colorkey1, colorkey2, glbTempPicture.w56);
	//^32CB:0988
	QUERY_PICST_IT(&glbTempPicture);
	//^32CB:0993
	return;
}

//^32CB:0997
void SkWinCore::DRAW_TEMP_PICST()
{
	//^32CB:0997
	ENTER(0);
	//^32CB:099A
	DRAW_PICST(&glbTempPicture);
	//^32CB:09A5
	return;
}

//^32CB:0A4C
// SPX: _32cb_0a4c renamed MAKE_BUTTON_CLICKABLE
void SkWinCore::MAKE_BUTTON_CLICKABLE(SRECT *prc, U8 xx, U8 yy)
{
	//^32CB:0A4C
	ENTER(0);
	//^32CB:0A4F
	COPY_MEMORY(prc, &_4976_4d1a[RCX(13,_4976_4dde)].rc0, sizeof(SRECT));
	//^32CB:0A6E
	_4976_4d1a[_4976_4dde].b11 = xx;
	_4976_4d1a[_4976_4dde].b10 = yy;
	_4976_4d1a[_4976_4dde].w8 = 0xffff;
	//^32CB:0AA0
	_4976_4dde++;
	//^32CB:0AA4
	return;
}

//^3E74:5AD9
void SkWinCore::QUERY_GDAT_IMAGE_METRICS(U8 cls1, U8 cls2, U8 cls4, i16 *pcx, i16 *pcy)
{
	//^3E74:5AD9
	ENTER(4);
	//^3E74:5ADE
	U16 si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, dtImage, cls4);
	//^3E74:5AF5
	if (glbShelfMemoryTable[si].Absent()) {
		//^3E74:5B10
		U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(cls1, cls2, cls4);
		//^3E74:5B29
		*pcx = READ_I16(bp04,-4);
		*pcy = READ_I16(bp04,-2);
	}
	else {
		//^3E74:5B3F
		U8 *bp04 = QUERY_GDAT_ENTRY_DATA_PTR(cls1, cls2, dtImage, cls4);
		//^3E74:5B5A
		*pcx = READ_I16(bp04,+0) & 1023;
		*pcy = READ_I16(bp04,+2) & 1023;
	}
	//^3E74:5B79
	return;
}

//^32CB:158F
U8 SkWinCore::SKCHR_TO_SCRIPTCHR(U8 xx)
{
	//^32CB:158F
	ENTER(0);
	//^32CB:1592
	U8 dl = xx;
	if (dl >= 'A' && dl <= 'Z')
		//^32CB:159F
		return dl +0xBF;
	//^32CB:15A5
	if (dl == '.')
		return 0x1B;
	//^32CB:15B4
	return 0x1A;
}

//^0B36:037E
// TODO: gfx related
U8 *SkWinCore::_0b36_037e(U8 *localpal, i8 xx, i16 colorkey1, i16 colorkey2, i16 palentcnt)
{
	//^0B36:037E
	ENTER(8);
	//^0B36:0384
	if (localpal != NULL && xx != 0) {
		//^0B36:0398
		xx = U8(max_value(0, 64 -xx));
		//^0B36:03AF
		for (i16 bp02 = 0; bp02 < palentcnt; bp02++) {
			//^0B36:03B7
			if (bp02 != colorkey1 && bp02 != colorkey2) {
				//^0B36:03CA
				U16 di = _4976_4be2[RCX(256,localpal[bp02])].b0;
				i16 si = 0;
				//^0B36:03E6
				i16 bp04 = max_value((_4976_4bde[RCX(16,di)].pv1[_4976_4be2[localpal[bp02]].b1] * xx) >> 6, 0);
				//^0B36:0433
				for (; _4976_4bde[di].b0 -1 > si; si++) {
					//^0B36:0435
					if (true
						&& _4976_4bde[di].pv1[si] <= bp04
						&& _4976_4bde[di].pv1[si +1] >= bp04
					) {
						//^0B36:046C
						i16 bp06 = bp04 - _4976_4bde[di].pv1[si   ];
						//^0B36:048A
						i16 bp08 = _4976_4bde[di].pv1[si +1] -bp04;
						//^0B36:04A7
						if (bp06 > bp08)
							//^0B36:04AF
							si++;
						//^0B36:04B0
						break;
					}
					//^0B36:04B2
				}
				//^0B36:04CD
				si = min_value(si, _4976_4bde[di].b0 -1);
				//^0B36:04EB

				localpal[bp02] = _4976_4bde[di].pv5[si];
			}
			//^0B36:0508
		}
	}
	//^0B36:03AF
	return localpal;
}

//^32CB:0804
void SkWinCore::_32cb_0804(U8 *localpal, i16 cls4, U16 colorkey1, i16 colorkey2, U16 palentcnt)
{
	//^32CB:0804
	ENTER(0);
	//^32CB:0809
	U16 di = palentcnt;
	//^32CB:080C
	i16 si;
	if (glbIsPlayerMoving != 0) {
		//^32CB:0813
		if (cls4 < 0) {
			//^32CB:0819
			si = max_value(-_4976_5a88, _4976_421b[RCJ(6,-cls4)]);
			//^32CB:0833
			cls4 = 1;
		}
		else {
			//^32CB:083A
			si = _4976_4221[RCJ(5,cls4)];
			//^32CB:0844
			cls4 += 9;
		}
	}
	else {
		//^32CB:084A
		si = _4976_4226[RCJ(5,cls4)];
	}
	//^32CB:0854	// SPX: That part is for translating palette in case of 'fog'
	if (!SkCodeParam::bDisableFogEffect && QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dt07, U8(cls4)) != 0) {
		//^32CB:086C
		TRANSLATE_PALETTE(localpal, GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, U8(cls4), di);
		//^32CB:0885
		_0b36_037e(localpal, i8(_4976_5a88), colorkey1, colorkey2, di);
	}
	else {
		//^32CB:0892
		_0b36_037e(localpal, 64 -U8(((64 -si) * (64 - _4976_5a88)) >> 6), colorkey1, colorkey2, di);
	}
	//^32CB:08BD
	return;
}

//^32CB:09A7
// SPX: _32cb_09a7 renamed QUERY_MULTILAYERS_PIC
U16 SkWinCore::QUERY_MULTILAYERS_PIC(ExtendedPicture *ref, U8 cls1, U8 cls2, U8 cls4, U16 horzRes, U16 vertRes, i16 zz, U16 mirrorflip, i16 colorkey1, U16 colorkey2)
{
	//^32CB:09A7
	ENTER(0);
	//^32CB:09AB
	QUERY_TEMP_PICST(mirrorflip, horzRes, vertRes, 0, 0, zz, -1, -1, colorkey1, -1, cls1, cls2, cls4);
	//^32CB:09D7
	COPY_MEMORY(&glbTempPicture, ref, sizeof(ExtendedPicture));
	//^32CB:09EE
	ALLOC_PICT_MEMENT(ref);
	//^32CB:09FB
	ref->colorKeyPassThrough = colorkey2;
	//^32CB:0A05
	SET_ORIGIN_RECT(&ref->rc36, ref->width, ref->height);
	//^32CB:0A1F
	U16 si;
	ref->pb44 = ALLOC_NEW_PICT(si = ALLOC_TEMP_CACHE_INDEX(), ref->width, ref->height, 8);
	//^32CB:0A47
	return si;
}

//^32CB:0649
// TODO: image related
U8 *SkWinCore::_32cb_0649(U8 cls1, U8 cls2, U8 cls4, i16 colorkey)
{
	//^32CB:0649
	ENTER(6);
	//^32CB:064D
	U8 *bp04 = QUERY_GDAT_IMAGE_LOCALPAL(cls1, cls2, cls4);
	//^32CB:0667
	i16 bp06;
	if (bp04 == NULL) {
		//^32CB:066B
		if (_4976_5a88 == 63)
			//^32CB:0672
			return NULL;
		//^32CB:0678
		for (bp06 = 0; bp06 < 256; bp06++) {
			//^32CB:067F
			_4976_582a[bp06] = U8(bp06);
			//^32CB:0689
		}
		//^32CB:0693
		bp06 = 256;
	}
	else {
		//^32CB:069A
		COPY_MEMORY(bp04, _4976_582a, 16);
		//^32CB:06B0
		bp06 = 16;
	}
	//^32CB:06B5
	return _0b36_037e(_4976_582a, U8(_4976_5a88), colorkey, -1, bp06);
}

//^44C8:0762
void SkWinCore::FIRE_UPDATE_BLIT_PALETTE(const U8 *localpal)
{
	//^44C8:0762
	ENTER(0);
	//^44C8:0767
	U8 *di = glbBlitPalette16;
	//^44C8:0771
	const U8 *si = localpal;
	//^44C8:0774
	memcpy(di, si, 16);
	//^44C8:078D
	return;
}

//^44C8:0791
void SkWinCore::FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(U16 offSrc, U16 offDst, U16 size)
{
	//^44C8:0791
	ENTER(0);
	//^44C8:0796
	//^44C8:0798
	U8 *bx = glbBlitPalette16;
	//^44C8:079B
	U8 *di = &_4976_5e6a[offDst];
	//^44C8:07A4
	U16 cx = size;
	//^44C8:07A7
	U8 *si = _4976_5e64;
	//^44C8:07AB
	bool carry = (offSrc & 1) ? true : false;
	si += (offSrc >>= 1);
	//^44C8:07B4
	if (carry) {
		//^44C8:07B6
		U8 al = lodsb(si);
		stosb(di, bx[al & 15]);
		cx--;
		//^44C8:07BD
		if (cx == 0) goto _080a;
	}

	{
		//^44C8:07BF
		bool carry = (cx & 1) ? true : false;
		cx >>= 1;
		//^44C8:07C1
		if (cx != 0) {
			//^44C8:07C3
			bool carry = (cx & 1) ? true : false;
			cx >>= 1;
			//^44C8:07C6
			if (cx != 0) {
				//^44C8:07C8
				U16 bp = 0x0f0f;
				do {
					//^44C8:07CD
					U16 ax = lodsw(si);
					U16 dx = ax >> 4;
					dx &= bp;
					ax &= bp;
					//^44C8:07D7
					stosb(di, bx[U8(ax >> 8)]);
					stosb(di, bx[U8(dx >> 8)]);
					stosb(di, bx[U8(ax &255)]);
					stosb(di, bx[U8(dx &255)]);
					//^44C8:07E9
				} while (--cx != 0);
			}
			//^44C8:07ED
			if (carry) {
				//^44C8:07EF
				U8 al = lodsb(si);
				stosb(di, bx[al >> 4     ]);
				stosb(di, bx[al      &255]);
			}
		}
		//^44C8:0801
		if (carry) {
			//^44C8:0803
			U8 al = lodsb(si);
			stosb(di, bx[al >> 4]);
		}
	}
	//^44C8:080A
_080a:
	//^44C8:080D
	return;
}

//	//^44C8:08AE
//	void _44c8_08ae(U16 offSrc, U16 offDst, U16 size, U8 colorkey)
//	{
//		//^44C8:08AE
//		ENTER(0);
//		//^44C8:08B3
//		U8 *bx = glbBlitPalette16;
//		//^44C8:08B8
//		U8 *di = &_4976_5e6a[offDst];
//		//^44C8:08C1
//		U8 dh = colorkey;
//		//^44C8:08C4
//		U16 cx = size;
//		//^44C8:08C7
//		U16 ax = offSrc;
//		bool carry = (ax & 1) ? true : false;
//		ax >>= 1;
//		//^44C8:08CD
//		U8 *si = &_4976_5e64[ax];
//		//^44C8:08D4
//		if (carry) {
//			//^44C8:08D6
//			U8 al = lodsb(si);
//			al &= 15;
//			if (al == dh) {
//				//^44C8:08DD
//				di++;
//			}
//			else {
//				stosb(di, bx[al]);
//			}
//			//^44C8:08E3
//			if (--cx == 0) goto _0931;
//		}
//
//		{
//			//^44C8:08E6
//			bool carry = (cx & 1) ? true : false;
//			cx >>= 1;
//			//^44C8:08E9
//			if (cx != 0) {
//				do {
//					//^44C8:08EB
//					U8 al = lodsb(si);
//					U8 ah = al & 15;
//					al >>= 4;
//					//^44C8:08F4
//					if (al != dh) {
//						//^44C8:08F8
//						if (ah != al) {
//							//^44C8:08FC
//							stosb(di, bx[al]);
//							stosb(di, bx[ah]);
//							//^44C8:0904
//							if (--cx != 0) continue;
//							break;
//						}
//						//^44C8:091D
//						stosb(di, bx[al]);
//						di++;//^44C8:0921
//						if (--cx != 0) continue;
//						break;
//					}
//					//^44C8:0908
//					if (ah != dh) {
//						//^44C8:090C
//						di++;
//						//^44C8:090D
//						stosb(di, bx[ah]);
//						//^44C8:0912
//						if (--cx != 0) continue;
//						break;
//					}
//					//^44C8:0916
//					di += 2;
//					//^44C8:0918
//				} while (--cx != 0);
//			}
//			//^44C8:0923
//			if (carry) {
//				//^44C8:0926
//				U8 al = lodsb(si) >> 4;
//				//^44C8:0927
//				if (al != dh) {
//					//^44C8:092E
//					stosb(di, bx[al]);
//				}
//			}
//		}
//		//^44C8:0931
//_0931:
//		//^44C8:0932
//		return;
//	}

//^44C8:1E1A
U16 SkWinCore::_44c8_1e1a(U8 *xx, U16 yy)
{
	//^44C8:1E1A
	ENTER(0);
	//^44C8:1E1E
	U8 *di = xx;
	bool carry = (yy & 1) ? true : false;
	{
		yy >>= 1;
		di += yy;
		U8 al = *di;
		if (!carry) {
			al &= 0xf0;
		}
		else {
			al &= 0x0f;
		}
		return (al != 0) ? 1 : 0;
	}
}

//^44C8:1E43
// Todo : teleporter gfx ???
void SkWinCore::_44c8_1e43(U8 *src, U8 *dst, U8 *zz, SRECT *prc, U16 ss, U16 tt, U16 aa, U16 bb, U16 hazeWidth, U16 dstWidth, i16 colorkey, U8 *localpal)
{
	//^44C8:1E43
	ENTER(16);
	//^44C8:1E49
	FIRE_UPDATE_BLIT_PALETTE(localpal);
	//^44C8:1E55
	hazeWidth = (hazeWidth +1) & 0xfffe;
	//^44C8:1E5F
	i16 bp08 = prc->cx;
	i16 bp06 = prc->cy;
	i16 bp02 = (prc->y * dstWidth) +prc->x;
	i16 bp04 = (bb * hazeWidth) +aa;
	//^44C8:1E89
	U16 di = tt;
	U16 si = ss -tt;
	//^44C8:1E94
	_4976_5e64 = src;
	_4976_5e6a = dst;
	//^44C8:1EAE
	if (zz == NULL) {
		//^44C8:1EB9
		for (U16 bp0a = 0; bp0a < bp06; bp02 += dstWidth, bp0a++) {
			//^44C8:1EC1
			i16 bp0e = bp08;
			//^44C8:1EC7
			U16 bp0c;
			for (bp0c = 0; bp0e >= si; bp0e -= si, bp0c += si, si = ss, di = 0) {
				//^44C8:1ECE
				if (colorkey < 0) {
					//^44C8:1ED4
					FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(di, bp02 +bp0c, si);
					//^44C8:1EE4
					continue;
				}
				//^44C8:1EE6
				FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(di, bp02 +bp0c, si, U8(colorkey));
				//^44C8:1EFA
			}
			//^44C8:1F0A
			if (bp0e > 0) {
				//^44C8:1F10
				if (colorkey < 0) {
					//^44C8:1F16
					FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(di, bp02 +bp0c, bp0e);
				}
				else {
					//^44C8:1F2A
					FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(di, bp02 +bp0c, bp0e, U8(colorkey));
				}
				//^44C8:1F40
				si -= bp0e;
				di += bp0e;
			}
			//^44C8:1F46
			if ((bp08 & 1) != 0) {
				//^44C8:1F4D
				if (--si != 0) {
					//^44C8:1F54
					di++;
				}
				else {
					//^44C8:1F57
					si = ss;
					di = 0;
				}
			}
			//^44C8:1F5C
		}
		//^44C8:1F70
		return;
	}
	//^44C8:1F73
	for (U16 bp0a = 0; bp0a < bp06; bp0a++) {
		//^44C8:1F7B
		for (U16 bp10 = bp08; bp10 > 0; ) {
			//^44C8:1F84
			U16 bp0c;
			for (bp0c = 0; bp10 > 0 && _44c8_1e1a(zz, bp04 +bp0c) == 0; ) {
				//^44C8:1F8B
				--bp10;
				++bp0c;
				++di;
				--si;
				//^44C8:1F93
				if (si == 0) {
					//^44C8:1F99
					di = 0;
					si = ss;
				}
				//^44C8:1F9E
			}
			//^44C8:1FBC
			U16 bp0e;
			for (bp0e = 0; bp10 > 0 && _44c8_1e1a(zz, bp04 +bp0c) != 0; ) {
				//^44C8:1FC3
				++bp0e;
				--bp10;
				//^44C8:1FC9
			}
			//^44C8:1FE7
			if (bp0e > 0) {
				//^44C8:1FED
				while (bp0e >= si) {
					//^44C8:1FEF
					if (colorkey < 0) {
						//^44C8:1FF5
						FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(di, bp02 +bp0c, si);
					}
					else {
						//^44C8:2007
						FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(di, bp02 +bp0c, si, U8(colorkey));
					}
					//^44C8:201B
					bp0e -= si;
					bp0c += si;
					//^44C8:2021
					si = ss;
					di = 0;
					//^44C8:2026
				}
				//^44C8:202B
				if (bp0e > 0) {
					//^44C8:2031
					if (colorkey < 0) {
						//^44C8:2037
						FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(di, bp02 +bp0c, bp0e);
					}
					else {
						//^44C8:204B
						FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(di, bp02 +bp0c, bp0e, U8(colorkey));
					}
					//^44C8:2061
					si -= bp0e;
					di += bp0e;
				}
			}
			//^44C8:2067
			if ((bp08 & 1) != 0) {
				//^44C8:206E
				if (--si != 0) {
					//^44C8:2075
					++di;
				}
				else {
					//^44C8:2078
					si = ss;
					di = 0;
				}
			}
			//^44C8:207D
			bp04 += hazeWidth;
			bp02 += dstWidth;
			//^44C8:2089
		}
		//^44C8:2092
	}
	//^44C8:20A0
	return;
}

//^44C8:20A4
void SkWinCore::_44c8_20a4(U8 *src, U8 *dst, U8 *zz, SRECT *prc, U16 ss, U16 tt, U16 oo, i16 pp, U8 *localpal)
{
	//^44C8:20A4
	ENTER(0);
	//^44C8:20A7
	_44c8_1e43(src, dst, zz, prc, ss, tt, 0, 0, prc->cx, oo, pp, localpal);
	//^44C8:20E3
	return;
}

//^32CB:0C7D
void SkWinCore::_32cb_0c7d(ExtendedPicture *ref, U16 xx, U16 yy)
{
	//^32CB:0C7D
	ENTER(16);
	//^32CB:0C83
	if (glbRainStrength == 0) {
		//^32CB:0C8A
		FILL_ENTIRE_PICT(reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(xx)), ref->b58[yy]);
		//^32CB:0CAA
		return;
	}
	//^32CB:0CAD
	U8 bp0b;
	U16 bp0a;
	QUERY_RAINFALL_PARAM(&bp0b, &bp0a);
	//^32CB:0CBE
	i16 bp06;
	i16 bp08;
	QUERY_GDAT_IMAGE_METRICS(0x17, glbMapGraphicsSet, bp0b, &bp06, &bp08);
	//^32CB:0CDA
	U16 di = ALLOC_TEMP_CACHE_INDEX();
	//^32CB:0CE1
	ALLOC_NEW_PICT(di, ref->rc36.cx, bp08, 4);
	//^32CB:0CF6
	U16 si = (ref->rc36.cx +1) & 0xfffe;
	//^32CB:0D03
	U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x17, glbMapGraphicsSet, bp0b);
	//^32CB:0D1B
	FIRE_BLIT_PICTURE(bp04, QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(di), ALLOC_TEMP_ORIGIN_RECT(si, READ_I16(bp04,-2)), 
		0, 0, READ_I16(bp04,-4), si, -1, (bp0a == 1) ? 1 : 0, 4, 4, NULL);
	//^32CB:0D64
	bp04 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(di));
	//^32CB:0D71
	U8 *bp10 = _32cb_0649(0x17, glbMapGraphicsSet, bp0b, 0);
	//^32CB:0D8A
	bp10[0] = ref->b58[yy];
	//^32CB:0D9A
	si = READ_I16(bp04,-4) * READ_I16(bp04,-2);
	//^32CB:0DA7
	_44c8_20a4(bp04, reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(xx)), NULL, &ref->rc36, 
		si -(RAND() & 31) -16, RAND16(60), ref->width, -1, bp10);
	//^32CB:0DF8
	FREE_TEMP_CACHE_INDEX(di);
	//^32CB:0DFF
	return;
}

//^3E74:5163
Bit8u *SkWinCore::QUERY_GDAT_ENTRY_DATA_BUFF(Bit8u cls1, Bit8u cls2, Bit8u cls3, Bit8u cls4)
{
LOGX(("%40s: C%02d=I%02X=E%02X=T%03d to %08X", "QUERY_GDAT_ENTRY_DATA_BUFF from ", cls1, cls2, cls4, cls3 ));
	//^3E74:5163
	i16 si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, cls3, cls4);
	//^3E74:5181
	if (si == -1) {
		//^3E74:5186
		return NULL;
	}
	//^3E74:518C
	if (glbShelfMemoryTable[si].Absent()) {
		//^3E74:51A9
		//if (IS_CLS1_CRITICAL_FOR_LOAD(cls1) == 0 && (SkCodeParam::bUseFixedMode && cls3 != dtText)) {
		if (IS_CLS1_CRITICAL_FOR_LOAD(cls1) == 0) {
			//^3E74:51C7
			//^3E74:5186
			return NULL;
		}
		//^3E74:51B6
		Bit16u bp02;
		return QUERY_GDAT_DYN_BUFF(si, &bp02, 0);
	}
	else {
		//^3E74:51C9
		return REALIZE_GRAPHICS_DATA_MEMORY(glbShelfMemoryTable[si]);
	}
}

//^0CEE:26CF
U8 SkWinCore::QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(U16 actuatorData)
{
	//^0CEE:26CF
	ENTER(0);
	//^0CEE:26D2
	return QUERY_CLS1_FROM_RECORD(ObjectID(0, GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(actuatorData), 0));
}

//^0CEE:1513
ObjectID SkWinCore::ALLOC_NEW_DBITEM_DEEPLY(U16 actuatorData)
{
	//^0CEE:1513
	ENTER(6);
	//^0CEE:1519
	U8 bp05 = GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(actuatorData);
	X16 di;
	ObjectID si = RECYCLE_A_RECORD_FROM_THE_WORLD(di = GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(actuatorData), bp05);
	if (si == OBJECT_NULL) {
		//^0CEE:153F
		si = ALLOC_NEW_RECORD(di);
		if (si == OBJECT_NULL)
			return si;
	}
	//^0CEE:154C
	GenericRecord *bp04;
	ZERO_MEMORY(bp04 = GET_ADDRESS_OF_RECORD(si), glbItemSizePerDB[di]);
	bp04->w0 = OBJECT_END_MARKER;
	SET_ITEMTYPE(si, bp05);
	//^0CEE:1581
	return si;
}

//^32CB:0EE5
U16 SkWinCore::IS_DISTINCTIVE_ITEM_ON_ACTUATOR(Actuator *ref, U16 disit, U16 onTile)
{
	//^32CB:0EE5
	ENTER(2);
	//^32CB:0EEB
	for (ObjectID si = GET_TILE_RECORD_LINK(ref->Xcoord(), ref->Ycoord()); si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
		//^32CB:0F0D
		U16 bp02 = si.DBType();
		//^32CB:0F18
		if (onTile != 0 && bp02 >= dbWeapon && bp02 <= dbMiscellaneous_item) {
			//^32CB:0F2A
			if (GET_DISTINCTIVE_ITEMTYPE(si) != disit)
				//^32CB:0F34
				continue;
			//^32CB:0F36
			return 1;
		}
		//^32CB:0F3B
		if (bp02 == dbCreature) {
			//^32CB:0F41
			for (ObjectID di = GET_ADDRESS_OF_RECORD4(si)->GetPossessionObject(); di != OBJECT_END_MARKER; di = GET_NEXT_RECORD_LINK(di)) {
				//^32CB:0F52
				if (GET_DISTINCTIVE_ITEMTYPE(di) == disit)
					//^32CB:0F5E
					//^32CB:0F36
					return 1;
				//^32CB:0F60
			}
		}
		//^32CB:0F6E
	}
	//^32CB:0F7C
	return 0;
}

//^32CB:0E03
void SkWinCore::DRAW_TEXT_TO_BACKBUFF(i16 xx, i16 yy, U8 *str)
{
	//^32CB:0E03
	ENTER(24);
	//^32CB:0E07
	if (QUERY_MBCS_PRESENCE(str) == 0) {
		//^32CB:0E18
		for (U8 *bp04 = str; *bp04 != 0; bp04++) {
			//^32CB:0E26
			if (*bp04 >= 'A' && *bp04 <= 'Z') {
				//^32CB:0E35
				*bp04 = *bp04 -0x40;
			}
			//^32CB:0E42
			else if (*bp04 >= 0x7b) {
				//^32CB:0E4B
				*bp04 = *bp04 -0x60;
			}
			//^32CB:0E52
		}
	}
	//^32CB:0E5E
	i16 bp06;
	i16 bp08;
	if (QUERY_STR_METRICS(str, &bp06, &bp08) != 0) {
		//^32CB:0E7A
		U8 bp18[16];
		COPY_MEMORY(glbPaletteT16, bp18, 16);
		//^32CB:0E93
		_0b36_037e(bp18, U8(_4976_5a88), -1, -1, 16);
		//^32CB:0EAA
		DRAW_STRONG_TEXT(_4976_4c16, -1, _4976_00f6, xx -(bp06 >> 1), yy, bp18[15], bp18[0]|0x4000, str);
	}
	//^32CB:0EE3
	return;
}

//^48AE:05AE
// _48ae_05ae renamed _48ae_05ae_CREATURE
U16 SkWinCore::_48ae_05ae_CREATURE(i16 disit, U8 yy, U16 zz, U16 ss, U16 tt, i16 ww)
{
	//^48AE:05AE
	ENTER(10);
	//^48AE:05B4
	U8 bp03 = GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(disit);
	//^48AE:05C0
	U8 bp04 = QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(disit);
	//^48AE:05CC
	U16 si = QUERY_GDAT_ENTRY_DATA_INDEX(bp04, bp03, dtWordValue, 0x02); // 0x02 -> Money value
	//^48AE:05E2
	U8 *bp08 = ALLOC_MEMORY_RAM(64, afDefault, 1024);
	//^48AE:05F9
	U16 bp0a = zz * 3 +18;
	//^48AE:0607
	U16 bp02 = QUERY_GDAT_CREATURE_WORD_VALUE(yy, U8(bp0a * 3 +0xa2));
	//^48AE:0620
	if (true
		&& bp02 != 0
		&& QUERY_CREATURES_ITEM_MASK(yy, U8(bp0a), bp08, 0) != 0
		&& (bp08[disit >> 3] & (1 << (disit & 7))) != 0
	) {
		//^48AE:0658
        si = si * bp02 / 100;
	}
	//^48AE:0666
	bp0a = zz * 3 +17;
	//^48AE:0674
	bp02 = QUERY_GDAT_CREATURE_WORD_VALUE(yy, zz *3 +0xa1);
	//^48AE:068D
	if (true
		&& bp02 != 0
		&& QUERY_CREATURES_ITEM_MASK(yy, U8(bp0a), bp08, 0) != 0
		&& (bp08[disit >> 3] & (1 << (disit & 7))) != 0
	) {
		//^48AE:06C5
		si = si * bp02 / 100;
	}
	//^48AE:06D3
	DEALLOC_UPPER_MEMORY(64);
	//^48AE:06DE
	if (ww < 0) {
		//^48AE:06E4
		ww = GET_MAX_CHARGE(ObjectID(0, GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(disit), 0));
	}
	//^48AE:06FA
	si += QUERY_GDAT_ENTRY_DATA_INDEX(
		QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(disit),
		GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(disit),
		dtWordValue,
		GDAT_ITEM_BONUS_MONEY_PER_CHARGE
		) * ww; // 0x35 -> Additional money value per charge
	//^48AE:0723
	if (tt != 0) {
		//^48AE:0729
		U16 di = QUERY_GDAT_CREATURE_WORD_VALUE(yy, U8(zz * 3) +0xa0);
		//^48AE:0741
		if (di == 0)
			//^48AE:0745
			di = 10;
		//^48AE:0748
		di = si * di / 100;
		//^48AE:0755
		if (ss != 0) {
			//^48AE:075B
			si += di;
		}
		else {
			//^48AE:075F
			si -= di;
		}
	}
	//^48AE:0761
	return si;
}

//^48AE:0767
// _48ae_0767 renamed _48ae_0767_MONEY_VALUE
U16 SkWinCore::_48ae_0767_MONEY_VALUE(i16 xx, i16 yy, U8 *zz, i16 *vv, i16 ww)
{
	//^48AE:0767
	ENTER(0);
	//^48AE:076C
	i16 si = 0;
	//^48AE:076E
	i16 dx = glbCountMoneyItems -1;
	U16 cx = 0;
	//^48AE:0776
	*vv = 0;
	//^48AE:077E
	i16 di;
	while (si < yy && xx > 0 && dx >= 0 && (di = glbMoneyItemsValueTable[dx]) > 0) {
		//^48AE:0780
		if (xx >= di) {
			//^48AE:0785
			zz[si] = U8(dx);
			//^48AE:078D
			si++;
			//^48AE:078E
			xx -= di;
			//^48AE:0791
			cx += di;
			//^48AE:0793
			(*vv)++;
		}
		else {
			//^48AE:079B
			dx--;
		}
        //^48AE:079C
	}
	//^48AE:07B9
	return cx;
}

//^32CB:0F82
// _32cb_0f82 renamed _32cb_0f82_SHOP_GLASS
void SkWinCore::_32cb_0f82_SHOP_GLASS(Actuator *ref, U8 cls4, i16 bb, i16 cellPos, U16 horzStretch, U16 vertStretch, U16 rectno, U16 gg, U16 colorkey1)
{
	//^32CB:0F82
	ENTER(58);
	//^32CB:0F88
	Actuator *bp0c = NULL;
	//^32CB:0F92
	U16 bp26;
	for (ObjectID di = GET_TILE_RECORD_LINK(ref->Xcoord(), ref->Ycoord()); di != OBJECT_END_MARKER && (bp26 = di.DBType()) <= dbActuator; di = GET_NEXT_RECORD_LINK(di)) {
		//^32CB:0FB4
		if (bp26 == dbActuator) {
			//^32CB:0FBA
			bp0c = GET_ADDRESS_OF_ACTU(di);
			//^32CB:0FC7
			if (bp0c->ActuatorType() == ACTUATOR_FLOOR_TYPE__SHOP) // 0x30 -> 'Shop exhibition'
				//^32CB:0FD4
				break;
		}
		//^32CB:0FD6
	}
	//^32CB:0FF4
	i16 si = bp0c->ActuatorData();
	U16 bp24 = si;
	//^32CB:1006
	U8 bp11 = U8(ref->ActuatorData());
	//^32CB:1016
	if (ref->ActiveStatus() == 0 || ref->OnceOnlyActuator() != 0) {
		//^32CB:1032
		U16 bp14 = (ref->Delay() * 3) +16;
		//^32CB:104A
		U8 *bp04 = ALLOC_MEMORY_RAM(64, afDefault, 1024);
		//^32CB:1061
		if (QUERY_CREATURES_ITEM_MASK(bp11, U8(bp14), bp04, 0) != 0) {
			//^32CB:107F
			U16 *bp10 = reinterpret_cast<U16 *>(QUERY_GDAT_ENTRY_DATA_BUFF(GDAT_CATEGORY_WALL_GFX, cls4, dt08, GDAT_WALL_ORNATE__DATA_13));
			//^32CB:1097
			i16 bp28;
			if (bp10 != NULL) {
				//^32CB:109B
				bp28 = QUERY_GDAT_ENTRY_DATA_LENGTH(GDAT_CATEGORY_WALL_GFX, cls4, dt08, GDAT_WALL_ORNATE__DATA_13) >> 1;
				//^32CB:10B2
				for (si = 0; si < bp28; si++) {
					//^32CB:10B6
					if ((bp10[si] & 511) == bp24)
						//^32CB:10C8
						break;
				}
			}
			do {
				//^32CB:10D0
				si += (ref->OnceOnlyActuator() != 0 || _4976_5a96 < 0) ? 1 : -1;
				//^32CB:10F2
				if (bp10 != NULL) {
					//^32CB:10FA
					if (si < 0) {
						//^32CB:10FE
						si = bp28 -1;
					}
					//^32CB:1106
					else if (si >= bp28) {
						//^32CB:110B
						si = 0;
					}
					//^32CB:110D
					bp24 = bp10[si] & 511;
				}
				else {
					//^32CB:111E
					si &= 511;
					bp24 = si;
				}
				//^32CB:1124
				//^32CB:1127
			} while (false
				|| (bp04[bp24 >> 3] & (1 << (bp24 & 7))) == 0
				|| (true
					&& (QUERY_GDAT_ENTRY_DATA_INDEX(QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(bp24), GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(bp24), dtWordValue, 0) & 0x1000) != 0
					&& IS_DISTINCTIVE_ITEM_ON_ACTUATOR(ref, bp24, 1) == 0
					&& IS_DISTINCTIVE_ITEM_ON_ACTUATOR(bp0c, bp24, 0) == 0
					&& _2fcf_16ff(bp24) == 0
					)
				);
			//^32CB:11A2
			ref->OnceOnlyActuator(0);
		}
		//^32CB:11AA
		ref->ActiveStatus(1);
		//^32CB:11B2
		bp0c->ActiveStatus(1);
		//^32CB:11BA
		bp0c->ActuatorData(bp24);
		//^32CB:11CD
		DEALLOC_UPPER_MEMORY(64);
	}
	//^32CB:11D8
	U16 bp18 = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, cls4, dtImageOffset, GDAT_WALL_ORNATE__DATA_FD);	// 0xFD
	//^32CB:11EE
	QUERY_TEMP_PICST(0, horzStretch, vertStretch, i8(bp18 >> 8), i8(bp18), bb, rectno, gg, colorkey1, -1, QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(bp24),
		GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(bp24), 0x00);
	//^32CB:122E
	DRAW_TEMP_PICST();
	//^32CB:1232
	if (true
		&& IS_DISTINCTIVE_ITEM_ON_ACTUATOR(ref, bp24, 1) == 0
		&& IS_DISTINCTIVE_ITEM_ON_ACTUATOR(bp0c, bp24, 0) == 0
	) {
		//^32CB:1264
		U16 *bp10 = reinterpret_cast<U16 *>(QUERY_GDAT_ENTRY_DATA_BUFF(GDAT_CATEGORY_WALL_GFX, cls4, dt08, GDAT_WALL_ORNATE__DATA_13));
		//^32CB:127C
		if (bp10 != NULL) {
			//^32CB:1280
			i16 bp28 = QUERY_GDAT_ENTRY_DATA_LENGTH(GDAT_CATEGORY_WALL_GFX, cls4, dt08, GDAT_WALL_ORNATE__DATA_13) >> 1;
			//^32CB:1297
			for (si = 0; si < bp28; si++) {
				//^32CB:129B
				if ((bp10[si] & 511) == bp24)
					//^32CB:12AD
					break;
			}
			//^32CB:12B5
			ObjectID di;
			if ((bp10[si] & 0x8000) != 0 && (di = ALLOC_NEW_DBITEM_DEEPLY(bp24)) != OBJECT_NULL) {
				//^32CB:12D5
				APPEND_RECORD_TO(di, NULL, ref->Xcoord(), ref->Ycoord());
			}
		}
		else {
			//^32CB:12FD
			QUERY_TEMP_PICST(0, horzStretch, vertStretch, i8(bp18 >> 8), i8(bp18), bb, rectno, gg, colorkey1, -1, 9, cls4, 0x11); // sold out icon
			//^32CB:132F
			DRAW_TEMP_PICST();
		}
	}
	//^32CB:1333
	QUERY_TEMP_PICST(0, horzStretch, vertStretch, 0, 0, bb, rectno, gg, colorkey1, -1, 9, cls4, 0x0f); // shop panel nav board
	//^32CB:135C
	DRAW_TEMP_PICST();
	//^32CB:1360
	if (cellPos == 3) {
		//^32CB:1366
		i16 bp2a;
		i16 bp2c;
		QUERY_TOPLEFT_OF_RECT(rectno, &bp2a, &bp2c);
		//^32CB:137B
		U16 bp18 = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, cls4, dtImageOffset, 0xf2);
		//^32CB:1391
		bp2a += i8(bp18 >> 8);
		bp2c += i8(bp18);
		//^32CB:139F
		DRAW_TEXT_TO_BACKBUFF(bp2a, bp2c, QUERY_GDAT_ITEM_NAME(QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(bp24), GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(bp24)));
	}
	//^32CB:13C9
	if (cellPos == 3) {
		//^32CB:13D2
		U16 bp22 = _48ae_05ae_CREATURE(bp24, bp11, ref->ShopItemPoolNo(), 1, 1, -1); // price for buy?
		//^32CB:13F8
		U16 bp20 = _48ae_05ae_CREATURE(bp24, bp11, ref->ShopItemPoolNo(), 0, 1, -1); // price for sell?
		//^32CB:141E
		for (i16 bp16 = 0; bp16 < 2; bp16++) {
			//^32CB:1426
			U16 bp18 = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, cls4, dtImageOffset, (bp16 != 0) ? GDAT_WALL_ORNATE__DATA_F0 : GDAT_WALL_ORNATE__DATA_F1);
			//^32CB:1446
			i16 bp2a = i8(bp18 >> 8);
			i16 bp2c = i8(bp18);
			//^32CB:1454
			U16 bp1a = (bp16 != 0) ? bp22 : bp20;
			//^32CB:1465
			if (bp1a <= 0)
				//^32CB:1469
				continue;
			//^32CB:146C
			U8 bp3a[16];
			i16 bp1e;
			_48ae_0767_MONEY_VALUE(bp1a, 6, bp3a, &bp1e, bp16);
			//^32CB:1486
			if (bp1e <= 0)
				//^32CB:148C
				continue;
			//^32CB:148F
			U16 bp1c = ALLOC_TEMP_CACHE_INDEX();
			//^32CB:1497
			ALLOC_NEW_PICT(bp1c, bp1e << 3, 7, 4);
			//^32CB:14AD
			QUERY_TEMP_PICST(0, 64, 64, 0, 0, bb, -1, gg, -1, -1, 0x09, cls4, 0x10);
			//^32CB:14D2
			U8 *bp08 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp1c));
			//^32CB:14E1
			SRECT bp34;
			SET_ORIGIN_RECT(&bp34, 8, 7);
			//^32CB:14F2
			for (si = 0; si < bp1e; si++) {
				//^32CB:14F6
				DRAW_DIALOGUE_PICT(glbTempPicture.pb0, bp08, &bp34, bp3a[si] << 3, 0, -1, NULL);
				//^32CB:1522
				bp34.x += 8;
				//^32CB:1526
			}
			//^32CB:152C
			DRAW_DIALOGUE_PICT(bp08, _4976_4c16, QUERY_BLIT_RECT(bp08, &bp34, rectno|0x8000, &bp2a, &bp2c, gg), 
				0, 0, colorkey1, glbTempPicture.b58);
			//^32CB:1576
			FREE_TEMP_CACHE_INDEX(bp1c);
			//^32CB:157F
		}
	}
	//^32CB:158B
	return;
}

//^32CB:3F0D
// SPX: _32cb_3f0d renamed DRAW_ALCOVE_ITEMS
void SkWinCore::DRAW_ALCOVE_ITEMS(U16 xx)
{
	//^32CB:3F0D
	ENTER(14);
	//^32CB:3F13
	X16 si = xx;
	ObjectID di = tblCellTilesRoom[si].xsrd.id4();
	if (di == OBJECT_END_MARKER)
		return;
	//^32CB:3F2F
	X16 bp08 = 2;
	//^32CB:3F34
	do {
		if (di.Dir() != ((glbPlayerDir +2) & 3))
			return;
		//^32CB:3F4A
		X16 bp0a = di.DBType();
		if (bp0a >= dbWeapon && bp0a <= dbMiscellaneous_item) {
			//^32CB:3F61
			DRAW_ITEM(di, si, 0, 0, bp08, NULL, 1, 0, 1);
			bp08++;
			if (bp08 >= 0xe)
				bp08 = 2;
			if (si == 3)
				//^32CB:3F91
				MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE(8, di, si, 3);
		}
		//^32CB:3FA1
		else if (bp0a == dbCloud && _4976_4463[RCJ(23,si)] != 0xff) {
			//^32CB:3FB4
			Cloud *bp04 = GET_ADDRESS_OF_RECORDF(di);
			U8 bp05 = bp04->CloudType();
			X16 bp0c = RAND02();
			if (bp05 == 0x64) {
				//^32CB:3FDC
				X16 bp0e = _4976_41ed[RCJ(17,_4976_4463[RCJ(23,si)])];
				QUERY_TEMP_PICST(bp0c, bp0e, bp0e, 0, 0, 0, QUERY_RECTNO_FOR_WALL_ORNATE(si, 0, 0xffff), 0xffff, 10, -1, 0x0d, bp05, 0xc);
				DRAW_TEMP_PICST();
			}
			//^32CB:4020
			else if (bp05 == 0x65) {
				_32cb_2cf3(bp05, _4976_41e6[RCJ(7,_4976_4463[RCJ(23,si)])], bp0c, QUERY_RECTNO_FOR_WALL_ORNATE(si, 12, 0));
				DRAW_TEMP_PICST();
			}
		}
		//^32CB:4054
	} while ((di = GET_NEXT_RECORD_LINK(di)) != OBJECT_END_MARKER);
	//^32CB:4065
	return;
}

//^32CB:15B8
// SPX: _32cb_15b8 renamed DRAW_WALL_ORNATE
i16 SkWinCore::DRAW_WALL_ORNATE(i16 cellPos, i16 yy, i16 zz)
{
	//^32CB:15B8
	ENTER(854);

#if DM2_EXTENDED_MODE == 1
	{
		if (glbGlobalSpellEffects.SeeThruWalls > 0 && cellPos == 3)	// front D1 wall
		{
			return -1;	// in case of see thru, don't show ornate at all
		}
	}
#endif


	//^32CB:15BE
	i16 bp14 = (yy <= -1)
		? 4
		: ((yy >= 1)
			? 6
			: 5
		);
	//^32CB:15DD
	bp14 = tblCellTilesRoom[cellPos].xsrd.w6[RCJ(4,bp14 -3)];	// get the ornate gfx id ?
	//^32CB:15F9
	i16 bp28 = bp14 >> 8;	// upper part (flags)
	//^32CB:15FF
	i16 iYDist = glbTabYAxisDistance[RCJ(23,cellPos)];	// i16 di
	//^32CB:1609
	U8 bp1f = U8(bp14) & 0xff; // lower part : gfx id
	//^32CB:1611
	if (bp1f == 0xff)
		//^32CB:1617
		return -1;
	//^32CB:161D
	U16 bp2a = (bp1f == 0) ? 1 : 0;
	//^32CB:162D
	U16 alcoveType = GET_WALL_ORNATE_ALCOVE_TYPE(bp1f);	// U16 bp22
	//^32CB:163A
	U16 iDoNotFlip = 0; // U16 bp24 = 0; SPX: fixed value init
	if (bp2a == 0) {
		//^32CB:1640
		iDoNotFlip = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp1f, dtWordValue, GDAT_WALL_ORNATE__DO_NOT_FLIP);	// Has some role for general graphics flip
	}
	//^32CB:1655
	U16 iFlipImage = 0;	// U16 bp0e = 0; use flip or not
	//^32CB:165A
	U16 si = 0; // defaulting to 0
	if (bp2a != 0 || (si = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp1f, dtWordValue, GDAT_IMG_COLORKEY_1)) == 0)
		//^32CB:1678
		si = glbSceneColorKey;
	//^32CB:167C
	U16 iOrnatePos = 0;	// U16 bp1a
	U16 iRefPoint = 0;	// U16 bp1e
	if (bp2a != 0 || (iOrnatePos = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp1f, dtWordValue, GDAT_WALL_ORNATE__POSITION)) == 0) {
		//^32CB:169B
		iOrnatePos = ORNATE_POS__VCENTERED_HCENTERED;	// SPX: that must be the default ornate position (default = 12)
		iRefPoint = 0;	// 0 = point is centered
	}
	else {	// values in GDAT are 1 to 25. shift it back to 0 to 24.
		//^32CB:16A7
		iRefPoint = iOrnatePos >> 8;	 // SPX: value in GDAT is 2 byte. Upper byte is how to draw image from ornate position
		iOrnatePos = (iOrnatePos & 255) -1;
	}
	//^32CB:16BA
	U16 iRectno = QUERY_RECTNO_FOR_WALL_ORNATE(cellPos, iOrnatePos, (yy != 0) ? 1 : 0);	// U16 bp1c; recto
	//^32CB:16D9
	U16 iStretchVertical = 0;	// U16 bp18
	U16 iStretchHorizontal = 0;	// U16 bp16
	iStretchVertical = iStretchHorizontal = tlbDistanceStretch[RCJ(5,iYDist)];
	
	//^32CB:16E5
	if (alcoveType == WALL_ORNATE_OBJECT__CRYOCELL && yy == 0) {	// bp22 == 3 && yy == 0
		//^32CB:16F7
		U16 bp34 = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp1f, dtImageOffset, GDAT_GFXSET_DATA_FD);	// 0x09 .. .. 0xFD
		// bp34 holds the offset x and y for the image
		//^32CB:170D
		// SPX: U8(_4976_5a80[cellPos].x2.w14) holds the portrait Champion ID to be displayed under the Champion Cell/Mirror
		QUERY_TEMP_PICST(iFlipImage, iStretchHorizontal, iStretchVertical, i8(bp34 >> 8), i8(bp34), iYDist, iRectno, iRefPoint, -1, -1, GDAT_CATEGORY_CHAMPIONS, U8(tblCellTilesRoom[cellPos].xsrd.xvalue), 1);
		//^32CB:174C
		if (zz == 0)
			//^32CB:1752
			glbTempPicture.colorKeyPassThrough = -2;
		//^32CB:1758
		DRAW_TEMP_PICST();
		//^32CB:175C
		if (zz == 0)
			//^32CB:1762
			//^32CB:1F38
			return si;
		//^32CB:1765
		if (iYDist == 1) {
			//^32CB:176A
			MAKE_BUTTON_CLICKABLE(&glbTempPicture.rc36, 6, U8(cellPos));
		}
	}
	//^32CB:177A
	if (iYDist == 2 && (yy <= -2 || yy >= 2))
		//^32CB:178B
		iStretchHorizontal = 0x72;	// 0x72 = 114	=> 178%
	//^32CB:1792
	else if (iYDist == 3 && (yy <= -2 || yy >= 2))
		//^32CB:17A3
		iStretchHorizontal = 0x4C;	// 0x4C = 76 => 118%
	if (SkCodeParam::bDM1Mode && iYDist >= 4)	// SPX: DM1 mode, do not display anything more than D3
		return -1;
	//^32CB:17A8
	if (bp2a != 0) {
		U8 bDrawSideTextPanel = 1; // default for DM2 mode
		//^32CB:17B1
		U8 bp20;
		if (yy == 0) {
			//^32CB:17B7
			bp20 = 0xFC;
		}
		else if (yy <= -1) {
			//^32CB:17C3
			bp20 = 0xFD;
		}
		else {
			//^32CB:17C9
			// xFD is the L-side text panel, then xFE would be the R-side text panel if they were not symmetric
			if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dtImage, 0xFE) != 0) { // SPX: GDAT2 never has 0xFE image ???
				//^32CB:17E0
				bp20 = 0xFE;
			}
			else {
				//^32CB:17E6
				bp20 = 0xFD;
				iFlipImage = 1;	// do flip
			}
		}
		//^32CB:17EF
		QUERY_TEMP_PICST(iFlipImage, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, iRectno, iRefPoint, si, -1, GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, bp20);
		//^32CB:1817
		if (zz == 0)
			//^32CB:181D
			glbTempPicture.colorKeyPassThrough = -2;
		// SPX: this draws the text panel or default gibberish side text. In case of DM1, the gibberish must not be displayed if the text is not visible.
		if (SkCodeParam::bDM1Mode)
		{
			ObjectID xTextObject = tblCellTilesRoom[cellPos].xsrd.xvalue;
			if (!IS_OBJECT_VISIBLE_TEXT(xTextObject))
				bDrawSideTextPanel = 0;
		}

		if (bDrawSideTextPanel == 1) // SPX: added this condition
		//^32CB:1823
			DRAW_TEMP_PICST();
		//^32CB:1827
		if (yy != 0)
			//^32CB:182D
			//^32CB:1F38
			return si;
		//^32CB:1830
		ObjectID bp32 = tblCellTilesRoom[cellPos].xsrd.xvalue;
		//^32CB:1845
		if (bp32 == OBJECT_NULL)
			//^32CB:184A
			//^32CB:1F38
			return si;
		//^32CB:184D
		ExtendedPicture bp01d6;
		QUERY_GDAT_SUMMARY_IMAGE(&bp01d6, GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, bp20);
		//^32CB:1865
		i16 bp2c = bp01d6.w28;
		i16 bp2e = bp01d6.w30;
		//^32CB:1873
		i16 bp48;
		i16 bp4a;
		QUERY_GDAT_IMAGE_METRICS(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, bp20, &bp48, &bp4a);
		//^32CB:188F
		U16 bp26 = ALLOC_TEMP_CACHE_INDEX();
		//^32CB:1897
		U8 bp009c[80];
		QUERY_MESSAGE_TEXT(bp009c, bp32, 2);
		//^32CB:18AA
		U16 bp36 = 0;
		U16 bp38 = 1;
		//^32CB:18B4
		U8 bp45;
		while ((bp45 = bp009c[bp36++]) != 0) {
			//^32CB:18B6
			if (bp45 == vbLf)
				//^32CB:18BC
				bp38++;
		}
		//^32CB:18D5
		U16 bp4c = QUERY_MBCS_PRESENCE(bp009c);
		//^32CB:18E5
		U16 bp3a;
		U16 bp3c;
		if (bp4c != 0) {
			//^32CB:18E9
			bp3a = _4976_012e;
			bp3c = _4976_0130 +2;
			si = 0;
		}
		else {
			//^32CB:18FB
			bp3a = _4976_012a;
			bp3c = _4976_012c +2;
		}
		//^32CB:1909
		U8 *bp08 = ALLOC_NEW_PICT(bp26, bp48, bp4a, 4);
		//^32CB:1922
		FILL_ENTIRE_PICT(bp08, si);
		//^32CB:1931
		SRECT rc44;
		rc44.y = (bp4a >> 1) - (bp3c * bp38 >> 1);
		//^32CB:1945
		if (bp4c != 0) {
			//^32CB:194E
			for (U16 bp36 = 0; bp009c[bp36] != 0; ) {
				//^32CB:1956
				if (bp4a - (_4976_0130 + _4976_0136 +1) <= rc44.y)
					//^32CB:1968
					break;
				//^32CB:196B
				U8 bp0356[384];
				i16 bp30 = _3929_04e2_DRAW_TEXT_STRINGS(bp009c, bp0356, &bp36, bp48 -1);
				//^32CB:198C
				if (bp30 != 0) {
					//^32CB:1995
					rc44.x = (bp48 >> 1) - (bp30 >> 1);
					//^32CB:19A4
					if (rc44.y >= 0 && rc44.x >= 0 && bp30 <= bp48) {
						//^32CB:19C1
						DRAW_STRING(bp08, bp26, bp48, rc44.x   , rc44.y + _4976_0130 +1, 1, 0x4000, bp0356, 4);
						//^32CB:19EE
						DRAW_STRING(bp08, bp26, bp48, rc44.x +1, rc44.y + _4976_0130 +1, 1, 0x4000, bp0356, 4);
						//^32CB:1A1D
						DRAW_STRING(bp08, bp26, bp48, rc44.x   , rc44.y + _4976_0130   , 2, 0x4000, bp0356, 4);
					}
				}
				//^32CB:1A49
				if (bp009c[bp36] == vbLf)
					//^32CB:1A58
					bp36++;
				//^32CB:1A5B
				rc44.y += bp3c;
				//^32CB:1A61
			}
		}
		else {
			//^_1a76
			U8 *bp0c = QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_LETTERS);	// 0x08 .. 0x03
			//^32CB:1A8C
			rc44.cx = bp3a;
			//^32CB:1A92
			rc44.cy = READ_I16(bp0c,-2);
			//^32CB:1A9C
			U8 *bp08 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp26));
			//^32CB:1AAB
			U8 *bp04 = bp009c;

			do {
				//^32CB:1AB5
				U16 bp36 = 0;
				//^32CB:1ABA
				while (bp04[bp36] != 0 && bp04[bp36] != vbLf)
					//^32CB:1ABC
					bp36++;
				//^32CB:1AD7
				i16 bp30 = bp3a * bp36;
				//^32CB:1AE0
				if (bp36 != 0) {
					//^32CB:1AE6
					rc44.x = (bp48 >> 1) - (bp30 >> 1);
					//^32CB:1AF5
					if (rc44.x < 0) {
						//^32CB:1AFB
						bp04 += bp36;
					}
					else {
						//^32CB:1AF9
						while (bp36-- != 0) {
							//^32CB:1B03
							DRAW_DIALOGUE_PICT(
								bp0c,
								bp08,
								&rc44,
								SKCHR_TO_SCRIPTCHR(*(bp04++)) * bp3a,
								0,
								si,
								QUERY_GDAT_IMAGE_LOCALPAL(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_LETTERS)	// 0x08 .. 0x03
								);
							//^32CB:1B46
							rc44.x += bp3a;
							//^32CB:1B49
						}
					}
				}
				//^32CB:1B56
				rc44.y += bp3c;
				//^32CB:1B5C
			} while (*(bp04++) != 0);
		}
		//^32CB:1B6B
		QUERY_GDAT_SUMMARY_IMAGE(&bp01d6, 0xff, 0x00, 0x00);
		//^32CB:1B80
		bp01d6.w12 = bp26;
		bp01d6.w6 = 0xffff;
		bp01d6.w52 = iStretchHorizontal;
		bp01d6.w54 = iStretchVertical;
		//^32CB:1B9B
		if (bp4c != 0) {
			//^32CB:1BA1
			bp01d6.b58[1] = glbPaletteT16[COLOR_BLACK];
			bp01d6.b58[2] = glbPaletteT16[COLOR_BROWN];
		}
		else {
			//^32CB:1BB6
			COPY_MEMORY(
				QUERY_GDAT_IMAGE_LOCALPAL(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_LETTERS),	// 08 .. 03
				bp01d6.b58,
				16
				);
		}
		//^32CB:1BDA
		bp01d6.w56 = 16;
		//^32CB:1BE1
		_32cb_0804(bp01d6.b58, U8(iYDist), si, -1, bp01d6.w56);
		//^32CB:1BF3
		bp01d6.pb44 = _4976_4c16;
		//^32CB:1C02
		bp01d6.rectNo = iRectno;
		bp01d6.w26 = iRefPoint;
		bp01d6.w28 = bp2c;
		bp01d6.w30 = bp2e;
		bp01d6.colorKeyPassThrough = si;
		//^32CB:1C22
		DRAW_PICST(QUERY_PICST_IT(&bp01d6));
		//^32CB:1C38
		FREE_TEMP_CACHE_INDEX(bp26);
		//^32CB:1C41
		//^32CB:1F38
		return si;
	}
	//^32CB:1C44
	U8 iImageEntry;	// U8 bp20; which image entry. 1 = front / 0 = side. Add default init to FRONT
	if (yy == 0) {	// Front image
		//^32CB:1C4A
		iImageEntry = GDAT_WALL_IMAGE__VIEW_FRONT;	// = 1;	front view
		//^32CB:1C4E
		if (iDoNotFlip == 0) {
			//^32CB:1C54
			if ((glbTabYAxisDistance[RCJ(23,cellPos)] & 1) != 0) {
				//^32CB:1C5E
				iFlipImage = glbGeneralFlipGraphics;
			}
			else {
				//^32CB:1C66
				iFlipImage = glbGeneralFlipGraphics ^ 1;
			}
		}
	}
	else {	// Side image
		//^32CB:1C71
		iImageEntry = GDAT_WALL_IMAGE__VIEW_SIDE_LEFT;	// = 0;	 side view on left
		//^32CB:1C75
		if (yy >= 1) {
			//^32CB:1C7B
			if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_WALL_GFX, bp1f, dtImage, U8(bp28) +2) != 0) {
				//^32CB:1C95
				iImageEntry = GDAT_WALL_IMAGE__VIEW_SIDE_RIGHT;	// = 2; side view on right
			}
			else {
				//^32CB:1C9B
				iFlipImage = 1;	// flip image
			}
		}
		// SPX: experimentation with side D0 new image
		if (SkCodeParam::bDM1Mode && iYDist == 0)
		{
			U16 iTestSideD0Entry = iImageEntry +U8(bp28) + 0x80;
			if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_WALL_GFX, bp1f, dtImage, iTestSideD0Entry) != 0)
			{
				iStretchVertical = 0x40;
				iStretchHorizontal = 0x40;
				iImageEntry = iImageEntry + 0x80;
			}
		}
	}
	//^32CB:1CA0
	iImageEntry = iImageEntry +U8(bp28);
#if DM2_EXTENDED_MODE == 1
	if (SkCodeParam::bUseExtendedSound && bp28 == 4 /*&& SkCodeParam::bForceOrnateSound == true*/)	// 4 = first anim of a loop (if several)
	{
		int iRandNoise = 0;
		int iMaxLoopSoundsAvailable = 9;
		while(iMaxLoopSoundsAvailable > 0)
		{
			if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_WALL_GFX, bp1f, dtSnd, SOUND_ACTIVATION_LOOP + iMaxLoopSoundsAvailable) != 0)
				break;
			iMaxLoopSoundsAvailable--;
		}
		if (iMaxLoopSoundsAvailable > 0)
			iRandNoise = RAND()%iMaxLoopSoundsAvailable;

//		i16 iYDist = glbTabYAxisDistance[RCJ(23,cellPos)];	
		i16 iXDist = glbTabXAxisDistance[RCJ(23,cellPos)];
		QUEUE_NOISE_GEN2(GDAT_CATEGORY_WALL_GFX, bp1f, SOUND_ACTIVATION_LOOP + iRandNoise, 0xFE, glbPlayerPosX+iXDist, glbPlayerPosY+iYDist, 0, 140, 200);
	}
#endif
	//^32CB:1CA9
	SRECT *bp12;
	if (yy == 0 && QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp1f, dtWordValue, GDAT_WALL_ORNATE__WINDOW) != 0) {
		//^32CB:1CCB
		QUERY_TEMP_PICST(iFlipImage, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, iRectno, iRefPoint, -3, -3, GDAT_CATEGORY_WALL_GFX, bp1f, iImageEntry);
		//^32CB:1CF4
		QUERY_TEMP_PICST(iFlipImage, iStretchHorizontal, iStretchVertical, glbTempPicture.w28, glbTempPicture.w30, iYDist, iRectno, iRefPoint, si, -1, GDAT_CATEGORY_WALL_GFX, bp1f, 200);
		//^32CB:1D1F
		if (zz == 0)
			//^32CB:1D25
			glbTempPicture.colorKeyPassThrough = -1;
		//^32CB:1D2B
		DRAW_TEMP_PICST(); // draw window contents (outside)
		//^32CB:1D2F
		ExtendedPicture bp0310;
		U16 bp26 = QUERY_MULTILAYERS_PIC(
			&bp0310, GDAT_CATEGORY_WALL_GFX, bp1f, iImageEntry, iStretchHorizontal, iStretchVertical, iYDist, iFlipImage, si,
			QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp1f, dtWordValue, GDAT_IMG_WALL_COLORKEY_2)	// 0x11 is colorkey2 for seeing outside through window (0xC8 image)
			);
		//^32CB:1D67
		_32cb_0c7d(&bp0310, bp26, si);
		//^32CB:1D78
		bp0310.pb44 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp26));
		//^32CB:1D89
		DRAW_PICST(&bp0310);
		//^32CB:1D96
		FREE_PICT_MEMENT(&bp0310);
		//^32CB:1DA3
		bp0310.colorKeyPassThrough = bp0310.b58[si];
		bp0310.pb44 = _4976_4c16;
		bp0310.rectNo = iRectno;
		bp0310.w26 = iRefPoint;
		bp0310.w56 = 0;
		//^32CB:1DD0
		_0b36_00c3(bp26, &bp0310);
		//^32CB:1DE1
		DRAW_PICST(&bp0310);
		//^32CB:1DEE
		bp12 = &bp0310.rc36;
		//^32CB:1DF8
		FREE_TEMP_CACHE_INDEX(bp26);
	}
	else {	// No window, standard wall ornate to display
		//^32CB:1E03
		QUERY_TEMP_PICST(iFlipImage, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, iRectno, iRefPoint, si, -1, GDAT_CATEGORY_WALL_GFX, bp1f, iImageEntry);
		//^32CB:1E2B
		if (zz == 0)
			//^32CB:1E31
			glbTempPicture.colorKeyPassThrough = -2;
		//^32CB:1E37
		DRAW_TEMP_PICST();
		//^32CB:1E3B
		bp12 = &glbTempPicture.rc36;
		//^32CB:1E43
		if (zz == 0)
			//^32CB:1E49
			//^32CB:1F38
			return si;
	}
	//^32CB:1E4C
	if (alcoveType != WALL_ORNATE_OBJECT__CRYOCELL && (cellPos == 1 || cellPos == 2 || cellPos == 3)) {
		//^32CB:1E64
		MAKE_BUTTON_CLICKABLE(bp12, 6, U8(cellPos));
	}
	//^32CB:1E76
	if (alcoveType == WALL_ORNATE_OBJECT__NONE || yy != 0)	// bp22 == 0
		//^32CB:1E85
		//^32CB:1F38
		return si;
	//^32CB:1E88
	COPY_MEMORY(&glbTempPicture, &_4976_5aa8, 314);
	//^32CB:1E9D
	// SPX: This is an overlay on front of wall decoration. This is only used for the shop panel
	if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_WALL_GFX, bp1f, dtImage, GDAT_WALL_ORNATE__OVERLAY) != 0) {
		//^32CB:1EB3
		if (alcoveType == WALL_ORNATE_OBJECT__SHOP_GLASS) {	// (bp22 == 2)
			//^32CB:1EB9
			_32cb_0f82_SHOP_GLASS(
				GET_ADDRESS_OF_ACTU(tblCellTilesRoom[cellPos].xsrd.xvalue), 
				bp1f, iYDist, cellPos, iStretchHorizontal, iStretchVertical, iRectno, iRefPoint, si
				);
			//^32CB:1EEF
			//^32CB:1F38
			return si;
		}
		//^32CB:1EF1
		QUERY_TEMP_PICST(iFlipImage, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, iRectno, iRefPoint, si, -1, GDAT_CATEGORY_WALL_GFX, bp1f, GDAT_WALL_ORNATE__OVERLAY);
		//^32CB:1F17
		if (zz == 0)
			//^32CB:1F1D
			glbTempPicture.colorKeyPassThrough = -2;
		//^32CB:1F23
		DRAW_TEMP_PICST();
		//^32CB:1F27
		//^32CB:1F38
		return si;
	}
	//^32CB:1F29
	if (alcoveType == WALL_ORNATE_OBJECT__ALCOVE)	// (bp22 == 1)
		DRAW_ALCOVE_ITEMS(cellPos);
	//^32CB:1F38
	return si;
}

//^0B36:1446
// _0b36_1446 renamed _0b36_1446_BLIT_PICTURE
i16 SkWinCore::_0b36_1446_BLIT_PICTURE(Picture *ref, i16 xx, i16 yy, U16 colorkey)
{
	//^0B36:1446
	ENTER(20);
	//^0B36:144C
	U8 *bp04 = QUERY_PICT_BITS(ref);
	//^0B36:145E
	if (bp04 == NULL)
		//^0B36:1466
		return -1;
	//^0B36:146C
	U16 si = ref->w22;
	//^0B36:1473
	U16 bp0e = ALLOC_TEMP_CACHE_INDEX();
	//^0B36:147B
	ALLOC_NEW_PICT(bp0e, (si == 4) ? 2 : 1, 1, si);
	//^0B36:1497
	U16 bp10 = ALLOC_TEMP_CACHE_INDEX();
	//^0B36:149F
	U8 *bp0c = ALLOC_NEW_PICT(bp10, (si == 4) ? 2 : 1, 1, si);
	//^0B36:14C1
	U8 *bp08 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp0e));
	//^0B36:14D0
	FILL_ENTIRE_PICT(bp08, colorkey);
	//^0B36:14E1
	FILL_ENTIRE_PICT(bp0c, colorkey);
	//^0B36:14F2
	FIRE_BLIT_PICTURE(bp04 = QUERY_PICT_BITS(ref), bp08, ALLOC_TEMP_ORIGIN_RECT(1, 1), xx, yy, 
		READ_I16(bp04,-4), (si == 4) ? 2 : 1, colorkey, 0, si, si, NULL);
	//^0B36:1547
	U16 bp14 = 1;
	//^0B36:154C
	U16 bp12 = CALC_IMAGE_BYTE_LENGTH(bp08);
	//^0B36:155B
	for (U16 di = 0; di < bp12; di++) {
		//^0B36:155F
		if (*(bp08++) != *(bp0c++)) {
			//^0B36:1573
			bp14 = 0;
			//^0B36:1578
			break;
		}
		//^0B36:157A
	}
	//^0B36:1580
	FREE_TEMP_CACHE_INDEX(bp0e);
	//^0B36:1589
	FREE_TEMP_CACHE_INDEX(bp10);
	//^0B36:1592
	return bp14;
}

//^32CB:00F1
// _32cb_00f1 renamed _32cb_00f1_DRAW_PICTURE
U16 SkWinCore::_32cb_00f1_DRAW_PICTURE(U16 xx, U16 yy, i16 zz)
{
	//^32CB:00F1
	ENTER(10);
	//^32CB:00F7
	U16 si = xx;
	U16 di = yy;
	//^32CB:00FD
	if (PT_IN_RECT(&glbTempPicture.rc36, si, di) == 0) {
		//^32CB:010F
		return 0;
	}
	//^32CB:0114
	// SPX: This test mirror flip (horizontal)
	if ((glbTempPicture.mirrorFlip & 1) != 0) {
		//^32CB:011C
		si = glbTempPicture.rc36.x + glbTempPicture.rc36.cx -1 -si + glbTempPicture.w32;
	}
	else {
		//^32CB:0128
		si = si - glbTempPicture.rc36.x + glbTempPicture.w32;
	}
	//^32CB:0134
	// SPX: Try to display mirror flip on y axis .. . This does nothing!!
	//di = glbTempPicture.rc36.y + glbTempPicture.rc36.cy -1 -di + glbTempPicture.w34;
	di = di - glbTempPicture.rc36.y + glbTempPicture.w34;
	//^32CB:0140
	U8 *bp0a = QUERY_PICT_BITS(&glbTempPicture);
	//^32CB:0151
	U16 bp04 = READ_I16(bp0a,-4) >> 1;
	//^32CB:015D
	_4976_5a94 = ((si - glbTempPicture.w14 -bp04) * 100) / bp04;
	//^32CB:0172
	U16 bp06 = READ_I16(bp0a,-2) >> 1;
	//^32CB:017B
	_4976_5a96 = ((di -bp06) * 100) / bp06;
	//^32CB:018C
	i16 bp02 = _0b36_1446_BLIT_PICTURE(&glbTempPicture, si, di, zz);
	//^32CB:01A0
	if (bp02 == -1 || bp02 == 1)
		//^32CB:01AC
		//^32CB:010F
		return 0;
	//^32CB:01AF
	return 1;
}

//^32CB:0287
// SPX: _32cb_0287 renamed _32cb_0287_DRAW_W_ORNATE
U16 SkWinCore::_32cb_0287_DRAW_W_ORNATE(U16 xx, U16 yy, U16 zz)
{
	//^32CB:0287
	ENTER(10);
	//^32CB:028D
	i16 si = xx;
	//^32CB:0290
	U16 di = 0;
	//^32CB:0292
	if (si > 3)
		//^32CB:0297
		return 0;
	//^32CB:029C
	CellTileDetail *bp0a = tblCellTilesRoom;
	//^32CB:02A9
	tblCellTilesRoom = reinterpret_cast<CellTileDetail *>(ALLOC_MEMORY_RAM(72, afDefault, 1024));
	//^32CB:02C1
	i16 bp04 = _4976_5a9c;
	i16 bp06 = _4976_5a9e;
	//^32CB:02CD
	CALC_VECTOR_W_DIR(_4976_5aa0, _4976_40e8[si][1], _4976_40e8[si][0], &bp04, &bp06);
	//^32CB:02F7
	tblCellTilesRoom[si].posx = U8(bp04);
	//^32CB:030A
	tblCellTilesRoom[si].posy = U8(bp06);
	//^32CB:031E
	SUMMARIZE_STONE_ROOM(&tblCellTilesRoom[si].xsrd, _4976_5aa0, bp04, bp06);
	//^32CB:0344
	i16 bp02 = DRAW_WALL_ORNATE(si, glbTabXAxisDistance[RCJ(23,si)], 0);
	//^32CB:0358
	if (bp02 >= 0) {
		//^32CB:035C
		di = _32cb_00f1_DRAW_PICTURE(yy, zz, bp02);
	}
	//^32CB:036E
	DEALLOC_UPPER_MEMORY(72);
	//^32CB:0379
	tblCellTilesRoom = bp0a;
	//^32CB:0386
	return (si == 3 || glbTempPicture.width < 32 || glbTempPicture.height < 32) ? 1 : di;
}

//^32CB:35C1
U16 SkWinCore::_32cb_35c1(i16 *xx, i16 *yy, i16 zz, i16 ww)
{
	//^32CB:35C1
	ENTER(2);
	//^32CB:35C7
	U16 cx = zz;
	i16 di = *xx;
	i16 si = *yy;
	//^32CB:35D6
	if (ww != 0) {
		//^32CB:35DC
		if ((si -= ww * 5) < 0) {
			//^32CB:35E8
			si += 20;
			//^32CB:35EB
			//^32CB:3607
			di = _4976_408c[RCJ(21,di)][2];
		}
		//^32CB:35F6
		else if (si > 24) {
			//^32CB:35FB
			si -= 20;
			//^32CB:35FE
			di = _4976_408c[RCJ(21,di)][3];
		}
		//^32CB:360A
		if (di < 0)
			//^32CB:360E
			return 0;
	}
	//^32CB:3612
	if (cx != 0) {
		//^32CB:3616
		i16 bp02 = (si % 5) + cx;
		//^32CB:3623
		if (bp02 > 4) {
			//^32CB:362A
			si += cx -4;
			//^32CB:3631
			di = _4976_408c[RCJ(21,di)][1];
		}
		//^32CB:363F
		else if (bp02 < 0) {
			//^32CB:3645
			si += cx +4;
			//^32CB:364C
			di = _4976_408c[RCJ(21,di)][0];
		}
		else {
			//^32CB:3657
			si += cx;
		}
		//^32CB:3659
		if (di < 0)
			//^32CB:365D
			//^32CB:360E
			return 0;
	}
	//^32CB:365F
	*xx = di;
	*yy = si;
	//^32CB:366B
	return 1;
}

//^098D:0C50
i16 SkWinCore::ROTATE_5x5_POS(i16 _5x5, U16 dir)
{
	//^098D:0C50
	ENTER(0);
	//^098D:0C55
	i16 di = (_5x5 % 5) -2;
	//^098D:0C63
	i16 si = (_5x5 / 5) -2;
	//^098D:0C6E
	i16 cx;
	switch (dir) {
		case 2:
			//^098D:0C82
			di = -di;
			//^098D:0C88
			//^098D:0C9E
			si = -si;

			break;

		case 3:
			//^098D:0C8C
			cx = di;
			//^098D:0C8E
			di = -si;
			//^098D:0C94
			si = cx;
			//^098D:0C96
			break;

		case 1:
			//^098D:0C98
			cx = di;
			//^098D:0C9A
			di = si;
			//^098D:0C9C
			//^098D:0C9E
			si = -cx;

	}
	//^098D:0CA2
	return di +((si +2) * 5) +2;
}

//^48AE:07FD
i16 SkWinCore::QUERY_OBJECT_5x5_POS(ObjectID rl, U16 reldir)
{
	//^48AE:07FD
	ENTER(4);
	//^48AE:0803
	ObjectID di = rl;
	//^48AE:0806
	U16 si = 12;
	//^48AE:0809
	switch (di.DBType()) {
		case dbCreature:
			{
				//^48AE:0821
				Creature *bp04 = GET_ADDRESS_OF_RECORD4(di);
				//^48AE:082E
				return QUERY_CREATURE_5x5_POS(bp04, (reldir -bp04->b15_0_1()) & 3);
			}
		case dbCloud:
			{
				//^48AE:0852
				if (GET_ADDRESS_OF_RECORDF(di)->b2_7_7() != 0)
					//^48AE:0869
					break;
				//^48AE:086B
				si = _4976_4a04[di.Dir()];

				break;
			}

		case dbWeapon:
		case dbCloth:
		case dbScroll:
		case dbPotion:
		case dbContainer:
		case dbMiscellaneous_item:
		case dbMissile:
			{
				//^48AE:086B
				si = _4976_4a04[di.Dir()];

				break;
			}

		case db11:
		case db12:
		case db13:

			break;
	}
	//^48AE:0878
	return ROTATE_5x5_POS(si, reldir);
}

//^098D:0CB7
i16 SkWinCore::QUERY_CREATURE_BLIT_RECTI(U16 cellPos, i16 _5x5, U16 dir)
{
	//^098D:0CB7
	ENTER(0);
	//^098D:0CBA
	return ROTATE_5x5_POS(_5x5, dir) +(cellPos * 25) +5000;
}

//^32CB:3672
// SPX: _32cb_3672 renamed DRAW_ITEM
void SkWinCore::DRAW_ITEM(ObjectID rl, i16 xx, U16 yy, U16 zz, i16 vv, Creature *ref, U16 ww, U16 ss, U16 tt)
{
	//^32CB:3672
	ENTER(34);
	//^32CB:3678
	U16 si = zz;
	//^32CB:367B
	i16 bp18 = 12; // defaulting to 12
	if (ref != NULL) {
		//^32CB:3686
		AIDefinition *bp04 = QUERY_CREATURE_AI_SPEC_FROM_TYPE(ref->CreatureType());
		//^32CB:369A
		U16 bp22 = bp04->w30;
		//^32CB:36A4
		U16 di = ((bp22 & 15) > 1) ? 0 : 1;
		//^32CB:36B5
		i16 bp20 = QUERY_GDAT_CREATURE_WORD_VALUE(ref->CreatureType(), 0x41);
		//^32CB:36C9
		if (bp20 == 0) {
			//^32CB:36CF
			bp20 = _4976_4398[RCJ(2,di)];
		}
		//^32CB:36D7
		si -= bp20;
		//^32CB:36DA
		U16 bp1e = (rl.Dir() - _4976_5aa0) & 3;
		//^32CB:36EA
		bp18 = (xx == 3 && _4976_5aa2 != 0) ? _4976_41d0[RCJ(7,glbTargetTypeMoveObject)] : 12;
		//^32CB:370A
		if (_32cb_35c1(&xx, &bp18, _4976_4388[RCJ(4,di)][RCJ(4,bp1e)], _4976_4388[RCJ(4,2 +di)][RCJ(4,bp1e)]) == 0)
			//^32CB:373B
			return;
	}
	//^32CB:373E
	else if (tt == 0) {
		//^32CB:3744
		bp18 = QUERY_OBJECT_5x5_POS(rl, _4976_5aa0);
	}
	//^32CB:3755
	i16 bp12 = glbTabYAxisDistance[RCJ(23,xx)];
	//^32CB:3760
	U16 bp0a;
	U16 bp06;
	U16 bp1a;
	if (tt != 0) {
		//^32CB:3766
		bp0a = 0;
		//^32CB:376B
		bp1a = QUERY_GDAT_ENTRY_DATA_INDEX(_4976_5aa8.b8, _4976_5aa8.b9, dtImageOffset, 0xfd);
		//^32CB:3783
		yy = _4976_5aa8.w28 + i8(bp1a >> 8) + _4976_41de[RCJ(8,_4976_41b0[RCJ(16,vv)][0])];
		//^32CB:37A4
		si = _4976_41de[RCJ(8,_4976_41b0[RCJ(16,vv)][1])] + (i8(bp1a) + _4976_5aa8.w30);
		//^32CB:37C6
		bp06 = _4976_5aa8.rectNo;
	}
	else {
		//^32CB:37CB
		bp0a = 4 -(bp18 / 5);
		//^32CB:37DC
		if (xx == 0 && bp0a < 2)
			//^32CB:37E8
			return;
		//^32CB:37EB
		bp06 = QUERY_CREATURE_BLIT_RECTI(xx, bp18, 0) | 0x8000;
	}
	//^32CB:3801
	U16 bp1c = 0;
	//^32CB:3806
	U8 bp0d = QUERY_CLS1_FROM_RECORD(rl);
	//^32CB:3812
	U8 bp0e = QUERY_CLS2_FROM_RECORD(rl);
	//^32CB:381E
	if (bp0d == 0x14 && bp0e < 8) {
		//^32CB:382A
		if (ss == 0) {
			//^32CB:3830
			ss = ss +GET_ADDRESS_OF_RECORD9(rl)->IsOpened();
		}
		//^32CB:384C
		bp1c = 1;
	}
	//^32CB:3851
	U8 bp0f = _4976_4380[RCJ(4,ss)][0];
	//^32CB:385D
	U16 bp08 = 0;
	//^32CB:3862
	i16 bp14 = glbTabXAxisDistance[RCJ(23,xx)];
	//^32CB:386D
	if (bp14 == 0) {
		//^32CB:3871
		i16 bp16 = bp18 % 5;
		//^32CB:387D
		U16 bp10;
		if (true
			&& (bp16 == 2 || tt != 0)
			&& QUERY_GDAT_ENTRY_IF_LOADABLE(bp0d, bp0e, dtImage, U8(bp10 = _4976_4380[RCJ(4,ss)][1])) != 0
		) {
			//^32CB:38AD
			bp0f = U8(bp10);
		}
		//^32CB:38B5
		else if (bp1c != 0 && tt == 0 && bp16 > 2) {
			//^32CB:38C7
			bp08 = 1;
		}
	}
	//^32CB:38CE
	else if (bp1c != 0 && bp14 == 1) {
		//^32CB:38DA
		bp08 = 1;
	}
	//^32CB:38DF
	if (vv != -1) {
		//^32CB:38E5
		yy += _4976_41de[RCJ(8,_4976_41b0[RCJ(16,vv)][0])];
		//^32CB:38FA
		if (ref == NULL) {
			//^32CB:3902
			si += _4976_41de[RCJ(8,_4976_41b0[RCJ(16,vv)][1])];
		}
	}
	//^32CB:3916
	bp1a = QUERY_GDAT_ENTRY_DATA_INDEX(bp0d, 0xfe, dtImageOffset, bp0f);
	//^32CB:392E
	if (bp1a != 0) {
		//^32CB:3932
		yy += i8(bp1a >> 8);
		//^32CB:3939
		si += i8(bp1a);
	}
	//^32CB:393F
	U16 bp0c = _4976_418e[RCJ(6,bp12)][1 +bp0a];
	//^32CB:3951
	QUERY_TEMP_PICST(bp08, bp0c, bp0c, yy, si, bp12, bp06, 0, 10, -1, bp0d, bp0e, bp0f);
	//^32CB:397D
	if (ww == 0)
		//^32CB:3983
		glbTempPicture.colorKeyPassThrough = -2;
	//^32CB:3989
	DRAW_TEMP_PICST();
	//^32CB:398D
	return;
}

//^32CB:03A6
ObjectID SkWinCore::_32cb_03a6(U16 xx, U16 yy, U16 zz, U16 ww, U16 vv, ObjectID ss, U16 tt, U16 uu)
{
	//^32CB:03A6
	ENTER(28);
	//^32CB:03AC
	U16 bp12 = ss.Dir();
	//^32CB:03B5
	Creature *bp04;
	ObjectID si;
	if (tt == 2) {
		//^32CB:03BB
		bp04 = GET_ADDRESS_OF_RECORD4(GET_CREATURE_AT(ww, vv));
		//^32CB:03D5
		si = bp04->GetPossessionObject();
	}
	else {
		//^32CB:03DE
		bp04 = NULL;
		//^32CB:03E8
		si = GET_TILE_RECORD_LINK(ww, vv);
	}
	//^32CB:03F7
	if (si == OBJECT_END_MARKER) {
		//^32CB:03FC
		if (uu != 0)
			//^32CB:0402
			return ss;
		//^32CB:0408
		//^32CB:0634
		return OBJECT_NULL;
	}
	//^32CB:040B
	i16 di = (tt == 3) ? 2 : 0;
	//^32CB:0418
	ObjectID bp0e = OBJECT_NULL;
	//^32CB:041D
	Container *bp0c = NULL;
	U16 bp16;
	Container *bp08;

	do {
		//^32CB:0427
		U16 bp10 = si.DBType();
		//^32CB:0432
		if (bp10 < dbWeapon || bp10 > dbMiscellaneous_item)
			//^32CB:0441
			continue;
		//^32CB:0444
		if (si.Dir() != bp12)
			//^32CB:044E
			continue;
		//^32CB:0451
		U16 bp14 = 0;
		bp08 = NULL;
		//^32CB:0460
		if (true
			&& IS_CONTAINER_CHEST(si) != 0
			&& QUERY_GDAT_ENTRY_IF_LOADABLE(0x14, QUERY_CLS2_FROM_RECORD(si), dtImage, 0x48) != 0
		) {
			//^32CB:0485
			if ((bp08 = GET_ADDRESS_OF_RECORD9(si))->IsOpened() != 0) {
				//^32CB:04A0
				bp14 = 3;
			}
			//^32CB:04A7
			else if (uu == 0) {
				//^32CB:04AD
				bp14 = 2;
			}
		}
		do {
			//^32CB:04B2
			DRAW_ITEM(si, zz, 0, 0, di, bp04, 0, bp14, (tt == 3) ? 1 : 0);
			//^32CB:04DC
			if (_32cb_00f1_DRAW_PICTURE(xx, yy, 10) == 1) {
				//^32CB:04F0
				bp0c = bp08;
				//^32CB:04FC
				bp0e = si;
				bp16 = bp14;
				//^32CB:0505
				break;
			}
			//^32CB:0507
			if (bp14 == 0)
				//^32CB:050B
				break;
			//^32CB:050D
			bp14 = 0;
			//^32CB:0512
		} while (true);

		//^32CB:0514
		di++;
		//^32CB:0515
		if (tt == 3) {
			//^32CB:051B
			if (di >= 14)
				//^32CB:0520
				di = 2;
		}
		else {
			//^32CB:0525
			di &= 15;
		}
		//^32CB:0529
	} while ((si = GET_NEXT_RECORD_LINK(si)) != OBJECT_END_MARKER);

	//^32CB:053A
	bp08 = bp0c;
	//^32CB:0546
	if (bp08 == NULL)
		//^32CB:054A
		goto _0639;
	//^32CB:054D
	switch (bp16) {
		case 0:
			//^32CB:0561
			if (uu != 0)
				//^32CB:0567
				//^32CB:05FF
				return OBJECT_END_MARKER;
			//^32CB:056A
			if (bp08->IsOpened() == 0)
				//^32CB:0578
				goto _0642;
			//^32CB:057B
			bp08->IsOpened(0);
			//^32CB:0583
			break;

		case 2:
			//^32CB:0586
			bp08->IsOpened(1);
			//^32CB:058E
			break;

		case 3:
			{
			//^32CB:0591
			if (uu != 0 && (QUERY_GDAT_DBSPEC_WORD_VALUE(ss, 4) & 0x8000) != 0)
				//^32CB:05A8
				//^32CB:05FF
				return OBJECT_END_MARKER;
			//^32CB:05AA
			ObjectID bp1c;
			ObjectID bp1a;
			bp1c = bp1a = bp08->GetContainedObject();
			//^32CB:05B7
			i16 bp18;
			for (bp18 = 0; bp1c != OBJECT_END_MARKER; ) {
				//^32CB:05BE
				bp18++;
				bp1a = bp1c;
				bp1c = GET_NEXT_RECORD_LINK(bp1c);
			}
			//^32CB:05D9
			if (uu != 0) {
				//^32CB:05DF
				if (bp18 < 8) {
					//^32CB:05E5
					APPEND_RECORD_TO(ss, &bp08->w2, -1, 0);
					//^32CB:05FD
					break;
				}
				//^32CB:05FF
				return OBJECT_END_MARKER;
			}
			//^32CB:0604
			if (bp18 <= 0)
				//^32CB:0608
				break;
			//^32CB:060A
			CUT_RECORD_FROM(bp1a, &bp08->w2, -1, 0);
			//^32CB:0622
			_4976_57f2 = 1;
			//^32CB:0628
			TAKE_OBJECT(bp1a, 1);

			break;
			}

	}
	//^32CB:0634
	return OBJECT_NULL;

	//^32CB:0639
_0639:
	if (bp0e == OBJECT_NULL)
		//^32CB:063F
		//^32CB:0402
		return ss;
	//^32CB:0642
_0642:
	return bp0e;
}

//^121E:013A
void SkWinCore::_121e_013a(U16 xx, U16 yy, U16 zz)
{
	//^121E:013A
	ENTER(4);
	//^121E:0140
	if (glbChampionLeader == -1)
		//^121E:0147
		return;
	//^121E:014A
	U16 di = glbPlayerPosX;
	U16 si = glbPlayerPosY;
	//^121E:0152
	U16 bp04 = _4976_4d1a[zz].b10;
	//^121E:0165
	if (bp04 != 0) {
		//^121E:0169
		if (bp04 != 3)
			//^121E:016E
			return;
		//^121E:0171
		di += glbXAxisDelta[glbPlayerDir];
		si += glbYAxisDelta[glbPlayerDir];
	}
	//^121E:0185
	ObjectID bp02 = _32cb_03a6(xx, yy, bp04, di, si, _4976_4d1a[zz].w8, _4976_4d1a[zz].b11, 0);
	//^121E:01BC
	if (bp02 != OBJECT_NULL) {
		//^121E:01C1
		if (_4976_4d1a[zz].b11 == 2) {
			//^121E:01D2
			MOVE_RECORD_TO(bp02, di, si, -1, -1);

			SkD((DLV_TWEET, "Tweet: You (x:%d, y:%d, map:%d) have taken %s from %s (a#%03d, x:%d, y:%d) \n"
				, glbPlayerPosX, glbPlayerPosY, glbCurrentMapIndex
				, static_cast<LPCSTR>(getRecordNameOf(bp02))
				, static_cast<LPCSTR>(getRecordNameOf(GET_CREATURE_AT(di, si)))
				, GET_CREATURE_AT(di, si).DBIndex()
				, di
				, si
				));
		}
		else {
			//^121E:01D6
			MOVE_RECORD_TO(bp02, di, si, -1, 0);

			SkD((DLV_TWEET, "Tweet: You (x:%d, y:%d, map:%d) have taken %s! [item DB%02d|#%04d|d:%1d] \n"
				, glbPlayerPosX, glbPlayerPosY, glbCurrentMapIndex
				, static_cast<LPCSTR>(getRecordNameOf(bp02))
				, bp02.RealDBType(), bp02.DBIndex(), bp02.Dir()
				));
		}
		//^121E:01E7
		_4976_57f2 = 1;
		//^121E:01ED
		if (IS_CONTAINER_MONEYBOX(bp02) != 0) {
			//^121E:01FA
			GET_ADDRESS_OF_RECORD9(bp02)->b7_2_2(0);
		}
		//^121E:020C
		TAKE_OBJECT(bp02, 1);
	}
	//^121E:0218
	_4976_4e5c = 1;
	//^121E:021E
	return;
}

//^121E:03AE
U16 SkWinCore::_121e_03ae(U16 aa, U16 bb, U16 xx, U16 yy, U16 cc, U16 dd, U16 ee)
{
	//^121E:03AE
	ENTER(0);
	//^121E:03B3
	U16 di = dd;
	//^121E:03B6
	ObjectID si = _32cb_03a6(aa, bb, cc, xx, yy, ObjectID(glbLeaderHandPossession.object, (glbPlayerDir +di) & 3), ee, 1);
	//^121E:03EA
	if (si == OBJECT_NULL) {
		//^121E:03EF
		REMOVE_OBJECT_FROM_HAND();
		//^121E:03F4
		return 1;
	}
	//^121E:03F9
	if (si == OBJECT_END_MARKER) {
		//^121E:03FE
		if (ee == 2) {
			//^121E:0404
			return _121e_0222(xx, yy, di +4);
		}
		else {
			//^121E:040C
			return _121e_0222(xx, yy, di);
		}
	}
	//^121E:041C
	return 0;
}

//^098D:02A2
U16 SkWinCore::_098d_02a2(U16 rectno, i16 xx, i16 yy)
{
	//^098D:02A2
	ENTER(8);
	//^098D:02A6
	SRECT bp08;
	if (QUERY_EXPANDED_RECT(rectno, &bp08) == NULL)
		//^098D:02BA
		return 0;
	//^098D:02BE
	return PT_IN_RECT(&bp08, xx, yy);
}

//^121E:00BE
// SPX: _121e_00be renamed PLAYER_TESTING_WALL
void SkWinCore::PLAYER_TESTING_WALL(U16 ww, U16 xx, U16 yy)
{
	//^121E:00BE
	ENTER(0);
	//^121E:00C1
	if (glbChampionsCount == 0)
		//^121E:00C6
		return;
	//^121E:00C8
	if (ww != 0) {
		//^121E:00CE
		if (_4976_4c3e != 0)
			//^121E:00D3
			return;
		//^121E:00D5
		FIRE_MOUSE_SET_CAPTURE();
		//^121E:00DA
		_4976_4c3e = 1;
		//^121E:00E0
		_443c_08ab(&_4976_5da0, &_4976_5da2, &_4976_5dae.rc4.cy);
		//^121E:00F4
		if ((_4976_5dae.rc4.cy & 2) == 0) {
			//^121E:00FC
			FIRE_MOUSE_RELEASE_CAPTURE();
			//^121E:0101
			_4976_4c3e = 0;
			//^121E:0107
		}
		else {
			//^121E:0109
			FIRE_HIDE_MOUSE_CURSOR();
		}
	}
	else {
		//^121E:0110
		// SPX: Probably sound when testing a wall
		QUEUE_NOISE_GEN2(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, SOUND_STD_DEFAULT, 0xfe, xx, yy, 0, 140, 200);
	}
	//^121E:0132
	_4976_4e5c = 1;
	//^121E:0138
	return;
}

//^0CEE:2E35
i16 SkWinCore::_0cee_2e35(U8 cls2)
{
	//^0CEE:2E35
	ENTER(0);
	//^0CEE:2E39
	U16 si = QUERY_GDAT_CREATURE_WORD_VALUE(cls2, 0x04);
	//^0CEE:2E47
	if (si == 0)
		//^0CEE:2E4B
		return 4;
	//^0CEE:2E4E
	return si;
}

//^32CB:28C7
void SkWinCore::QUERY_CREATURE_PICST(U16 xx, i16 dist, Creature *vv, CreatureInfoData *ww, ObjectID rl)
{
	//^32CB:28C7
	ENTER(28);
	//^32CB:28CD
	U16 di = 0;
	//^32CB:28CF
	sk1c9a02c3 *bp04 = _1c9a_02c3(vv, QUERY_CREATURE_AI_SPEC_FROM_TYPE(vv->CreatureType()));
	//^32CB:28F3
	i16 si = _4937_000f(bp04->w0, &bp04->w2);
	//^32CB:290C
	U16 bp06 = (ww == NULL) ? 0 : ww->b7;
	//^32CB:2924
	U16 bp0a = ((QUERY_CREATURE_AI_SPEC_FLAGS(rl) & 4) != 0) ? 2 : ((_4976_5aa0 - vv->b15_0_1()) & 3);
	//^32CB:2950
	U16 bp08 = _4976_5a98[si][bp0a +10];
	//^32CB:2969
	U8 bp0b = vv->CreatureType();
	//^32CB:2973
	U8 bp0c = _4976_5a98[si][bp0a +2];
	//^32CB:298A
	if (QUERY_GDAT_ENTRY_IF_LOADABLE(0x0f, bp0b, dtImage, bp0c) == 0) {
		//^32CB:299F
		bp0c = (bp0a +2) & 3;
		//^32CB:29A9
		if ((bp0c & 1) != 0)
			//^32CB:29AF
			di = 1;
		//^32CB:29B2
		bp0c = _4976_5a98[si][bp0c +2];
		//^32CB:29CD
		if (QUERY_GDAT_ENTRY_IF_LOADABLE(0x0f, bp0b, dtImage, bp0c) == 0) {
			//^32CB:29E2
			di = 0;
			//^32CB:29E4
			bp0c = _4976_5a98[si][4];
		}
	}
	//^32CB:29FA
	else if (false
		|| (bp08 & 1) != 0
		|| ((bp06 & 64) != 0 && (bp06 & 2) != 0 && (bp06 & 1) == 0)
	) {
		//^32CB:2A16
		di = 1;
	}
	//^32CB:2A19
	if (QUERY_GDAT_ENTRY_IF_LOADABLE(0x0f, bp0b, dtImage, bp0c) == 0) {
		//^32CB:2A31
		bp0c = bp0a -6;
		//^32CB:2A39
		if (QUERY_GDAT_ENTRY_IF_LOADABLE(0x0f, bp0b, dtImage, bp0c) == 0) {
			//^32CB:2A4E
			if (true
				&& bp0c == 0xfb
				&& QUERY_GDAT_ENTRY_IF_LOADABLE(0x0f, bp0b, dtImage, bp0c +2) != 0
			) {
				//^32CB:2A6E
				di = 1;
				//^32CB:2A71
				bp0c = bp0c +2;
			}
			else {
				//^32CB:2A7B
				di = 0;
				//^32CB:2A7D
				bp0c = 0xfc;
			}
		}
	}
	//^32CB:2A81
	i16 bp10 = tlbDistanceStretch[RCJ(5,dist)];
	//^32CB:2A8D
	i16 bp14 = (ww != NULL && ww->Command == ccmDestroy) ? ww->w14 : si;
	//^32CB:2AAB
	U16 bp12 = _4976_5a98[bp14][0];
	//^32CB:2AC1
	if (xx == 3 && _4976_5aa2 != 0) {
		//^32CB:2ACE
		bp12 = _4976_41d0[RCJ(7,glbTargetTypeMoveObject)];
		bp10 = _4976_41d7[RCJ(7,glbTargetTypeMoveObject)];
		bp0a = 0;
	}
	else {
		//^32CB:2AE9
		bp10 = CALC_STRETCHED_SIZE(_4976_5a98[si][bp0a +6], bp10);
	}
	//^32CB:2B0D
	U16 bp0e = QUERY_CREATURE_BLIT_RECTI(xx, bp12, bp0a) | 0x8000;
	//^32CB:2B24
	i16 bp18, bp16;
	bp18 = bp16 = i8(_4976_5a98[bp14][1]);
	//^32CB:2B3D
	if (bp16 != 0) {
		//^32CB:2B44
		i16 bp1c;
		i16 bp1a;
		switch (bp0a) {
		case 0:
			//^32CB:2B50
			bp1c = -7;
			//^32CB:2B55
			//^32CB:2B5C
			bp1a = 0;
			//^32CB:2B61
			break;

		case 2:
			//^32CB:2B57
			bp1c = 7;
			//^32CB:2B5C
			bp1a = 0;
			//^32CB:2B61
			break;

		case 1:
		case 3:
			//^32CB:2B63
			bp1a = -64;
			bp1c = 0;

			break;
		}
		//^32CB:2B6D
		bp1a = CALC_STRETCHED_SIZE(bp1a, bp16);
		//^32CB:2B7D
		bp18 = CALC_STRETCHED_SIZE(bp1c, bp18);
	}
	//^32CB:2B8D
	QUERY_TEMP_PICST(di, bp10, bp10, _4976_41de[RCJ(8,bp06 & 7)] +bp16, _4976_41de[RCJ(8,(bp06 >> 3) & 7)] +bp18, dist,
		bp0e, -1, _0cee_2e35(bp0b), -1, 0x0f, bp0b, bp0c);
	//^32CB:2BDA
	return;
}

//^32CB:01B6
U16 SkWinCore::_32cb_01b6(U16 xx, U16 yy, U16 ss, U16 tt, U16 *ww)
{
	//^32CB:01B6
	ENTER(8);
	//^32CB:01BC
	ObjectID si = GET_CREATURE_AT(ss, tt);
	//^32CB:01CB
	if (si == OBJECT_NULL)
		//^32CB:01D0
		return 0;
	//^32CB:01D5
	Creature *bp04 = GET_ADDRESS_OF_RECORD4(si);
	//^32CB:01E2
	CreatureInfoData *bp08;
	if (bp04->b5 == 255) {
		//^32CB:01EC
		bp08 = NULL;
	}
	else {
		//^32CB:01F8
		bp08 = &glbTabCreaturesInfo[bp04->b5];
	}
	//^32CB:0216
	QUERY_CREATURE_PICST(3, glbTabYAxisDistance[3], bp04, bp08, si);
	//^32CB:0232
	i16 di = glbTempPicture.colorKeyPassThrough;
	//^32CB:0236
	glbTempPicture.colorKeyPassThrough = -2;
	//^32CB:023C
	DRAW_TEMP_PICST();
	//^32CB:0241
	if (_32cb_00f1_DRAW_PICTURE(xx, yy, di) == 0)
		//^32CB:0253
		//^32CB:01D0
		return 0;
	//^32CB:0256
	if (_4976_5a94 < -75) {
		//^32CB:025D
		*ww = 3;
	}
	//^32CB:0267
	else if (_4976_5a94 > 75) {
		//^32CB:026E
		*ww = 1;
	}
	else {
		//^32CB:0278
		*ww = 0;
	}
	//^32CB:0280
	return 1;
}

//^121E:0222
U16 SkWinCore::_121e_0222(U16 xx, U16 yy, U16 ww)
{
	//^121E:0222
	ENTER(12);
	//^121E:0228
	Creature *bp0c = NULL;
	//^121E:0232
	if (glbChampionLeader == -1)
		//^121E:0239
		return 0;
	//^121E:023E
	U16 bp06 = (glbPlayerDir +ww) & 3;
	//^121E:024A
	ObjectID di = GET_CREATURE_AT(xx, yy);
	//^121E:0259
	if (di != OBJECT_NULL) {
        //^121E:025E
		AIDefinition *bp04 = QUERY_CREATURE_AI_SPEC_FROM_RECORD(di);
		//^121E:026B
		bp0c = GET_ADDRESS_OF_RECORD4(di);
		//^121E:0278
		if (true
			&& bp04->IsStaticObject() != 0
			&& (false
				|| (ww >= 4 && ww <= 7 && (bp04->w30 & 0x0800) == 0)
				|| (ww < 4 && (bp04->w30 & 0x2000) == 0)
			)
		) {
			//^121E:02A7
			//^121E:0239
			return 0;
		}
	}
	const ObjectID rlCreature = di;
	//^121E:02A9
	di = REMOVE_OBJECT_FROM_HAND();
	//^121E:02B0
	if (ww >= 4 && ww <= 7) {
		//^121E:02BC
		U8 bp07 = (ww -4 + glbPlayerDir) & 3;
		//^121E:02CA
		if (bp0c != NULL) {
			//^121E:02D2
			for (ObjectID si = bp0c->GetPossessionObject(); si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
				//^121E:02DB
				if (si.Dir() == bp07 && IS_CONTAINER_MONEYBOX(si) != 0) {
					//^121E:02F4
					GET_ADDRESS_OF_RECORD9(si)->b7_2_2(0);
				}
				//^121E:0304
			}
		}
		//^121E:0312
		MOVE_RECORD_TO(ObjectID(di, bp07), -1, -1, xx, yy);

		SkD((DLV_TWEET, "Tweet: You have put %s on %s (a#%03d, x:%d, y:%d) \n"
			, static_cast<LPCSTR>(getRecordNameOf(di))
			, static_cast<LPCSTR>(getRecordNameOf(rlCreature))
			, rlCreature.DBIndex()
			, xx
			, yy
			));
	}
	else{
		//^121E:0323
		MOVE_RECORD_TO(ObjectID(di, bp06), -1, 0, xx, yy);

		SkD((DLV_TWEET, "Tweet: You have put %s! \n"
			, static_cast<LPCSTR>(getRecordNameOf(di))
			));
	}
	//^121E:0344
	_4976_4e5c = 1;
	//^121E:034A
	return 1;
}

//^0CEE:2E09
U16 SkWinCore::_0cee_2e09(ObjectID rl)
{
	//^0CEE:2E09
	ENTER(0);
	//^0CEE:2E0C
	return QUERY_CREATURE_AI_SPEC_FROM_RECORD(rl)->w32;
}

//^2C1D:1FB1
U16 SkWinCore::_2c1d_1fb1(U16 dir)
{
	//^2C1D:1FB1
	ENTER(0);
	//^2C1D:1FB4
	if (glbChampionLeader == -1)
		//^2C1D:1FBB
		return 0;
	//^2C1D:1FBF
	return _2c1d_1de2(glbChampionLeader, -1, dir);
}

//^121E:0351
U16 SkWinCore::_121e_0351(U16 xx, U16 yy)
{
	//^121E:0351
	ENTER(0);
	//^121E:0356
	U16 si = 0;
	//^121E:0358
	U16 di = (_4976_4dda == 17) ? 768 : 765;
	//^121E:0367
	if (_098d_02a2(di, xx, yy) != 0) {
		//^121E:037A
		si = _2c1d_1fb1(0); // throw by left hand
	}
	//^121E:037E
	else if (_098d_02a2(di +1, xx, yy) != 0) {
		//^121E:0394
		si = _2c1d_1fb1(1); // throw by right hand
	}
	//^121E:039E
	if (si != 0)
		//^121E:03A2
		_4976_4e5c = 1;
	//^121E:03A8
	return si;
}



//^38C8:0060
void SkWinCore::_38c8_0060()
{
	//^38C8:0060
	ENTER(0);
	//^38C8:0064
	if (_4976_5bec != 0 && glbGameHasEnded == 0) {
		//^38C8:0072
		if (glbIsPlayerSleeping != 0) {
			//^38C8:0079
			DRAW_WAKE_UP_TEXT();
			//^38C8:007E
			CHANGE_VIEWPORT_TO_INVENTORY(0);
		}
		else {
			//^38C8:0088
			_4976_5bec = 0;
			//^38C8:008E
			if (glbChampionIndex == 0)
				//^38C8:0095
				_4976_531c = 1;
			else
				//^38C8:009D
				glbSomeChampionPanelFlag = 1;
			//^38C8:00A3
			U16 si = glbChampionInventory;
			//^38C8:00A8
			if (si != 0) {
				//^38C8:00AC
				glbChampionInventory = 0;
				//^38C8:00B2
				INTERFACE_CHAMPION(si -1);
				//^38C8:00B9
			}
			else {
				//^38C8:00BB
				INIT_BACKBUFF();
				//^38C8:00C0
				_29ee_000f();
			}
		}
	}
	//^38C8:00C5
	return;
}

//^1031:1907
// SPX: _1031_1907 renamed DRAW_WAKE_UP_TEXT
void SkWinCore::DRAW_WAKE_UP_TEXT()
{
	//^1031:1907
	ENTER(40);
	//^1031:190B
	// This fills the main viewport in black
	FILL_ENTIRE_PICT(_4976_4c16, glbPaletteT16[COLOR_BLACK]);
	U8 bp28[40];
	// SPX: drawing the "Wake up" string
	DRAW_VP_RC_STR(
		6, 
		glbPaletteT16[COLOR_CYAN], 
		QUERY_GDAT_TEXT(GDAT_CATEGORY_INTERFACE_GENERAL, 0x00, 0x11, bp28)
		);
	//^1031:194F
	return;
}

//^2C1D:14BA
// SPX: _2c1d_14ba renamed RESUME_FROM_WAKE
void SkWinCore::RESUME_FROM_WAKE() 
{
	//^2C1D:14BA
	ENTER(0);
	//^2C1D:14BD
	_4976_4e5c = 1; // SPX some move flag (viewport refresh?)
	glbIsPlayerSleeping = 0;
	//glbTickSpeed = 12;	// SPX holds tick balance?? 12 was an old value
	glbTickSpeed = stdTickBalance;
	INIT_BACKBUFF();
	_1031_0541(5);
	//^2C1D:14DC
	return;
}

//^476D:00B1
void SkWinCore::FILE_DELETE(const U8 *curf)
{
	fset.fileDelete(reinterpret_cast<const char *>(curf));
}
//^476D:012C
U16 SkWinCore::FILE_WRITE(i16 handle, U32 buffSize, const void *buff)
{
	return fset.fileWrite(handle, buffSize, buff);
}
//^476D:008F
i16 SkWinCore::FILE_CREATE(const U8 *newf)
{
	return fset.fileCreate(reinterpret_cast<const char *>(newf));
}

//^2066:036E
Bit16u SkWinCore::_2066_036e(void *buff, Bit16u buffSize)
{
	//^2066:036E
	Bit32u bp04 = buffSize;

	//^2066:037D
	if (bp04 != 0) {
		//^2066:0386
		return FILE_WRITE(glbDataFileHandle, bp04, buff);
	}
	return 1;
}

//^2066:03C0
U16 SkWinCore::SKSAVE_WRITE(void *buff, U16 buffSize)
{
	//^2066:03C0
	if (_2066_036e(buff, buffSize) == 0)
		return 0;
	//^2066:03DB
	return 1;
}
//^2066:000F
U8 SkWinCore::SUPPRESS_WRITER(const void *pvData, const void *pvMask, U16 cbData, U32 repeat)
{
	// [bp+06] -> data
	// [bp+0A] -> mask
	// [bp+0E] -> xx
	// [bp+10] -> yy

	const Bit8u *data = (const Bit8u *)pvData;
	const Bit8u *mask = (const Bit8u *)pvMask;

	s_testSKSave.Write(pvData, pvMask, cbData, repeat);

	//^2066:000F
	Bit32u bp0a = 0;
	Bit8u bp04 = _4976_5254; // queued nibble
	Bit8u bp05 = _4976_524e; // current bit position
	Bit32u bp0e = 0;
	//^2066:0034
	while (!(bp0e >= repeat)) {
		//^2066:0037
		Bit16u si = 0;
		//^2066:0039
		while (!(si >= cbData)) {
			//^2066:003C
			Bit8u data_nibble = *data; // [bp-03]
			data++;
			Bit8u mask_nibble = *mask; // [bp-02]
			mask++;
			//^2066:0054
			if (mask_nibble != 0) {
				//^2066:005D
				i8 bp01 = 0x07;
				//^2066:0061
				while (bp01 >= 0) {
					//^2066:0063
					if (((1 << bp01) & mask_nibble) != 0) {
						//^2066:0074
						bp04 <<= 1;
						//^2066:0077
						if (((1 << bp01) & data_nibble) != 0) {
							bp04 |= 1;
						}
						//^2066:008C
						bp0a++;
						//^2066:0094
						bp05 = bp05 + 1;
						//^2066:009C
						if (bp05 == 8) {
							//^2066:00A0
							Bit8u bp0f = bp04;
							if (FILE_WRITE(glbDataFileHandle, 1, &bp0f) == 0)
								return 1;
							//^2066:00C4
							bp05 = bp04 = 0;
						}
					}
					//^2066:00CC
					bp01--;
					//^2066:00CF
				}
			}
			//^2066:00D5
			si++;
			//^2066:00D6
		}
		//^2066:00DE
		mask -= cbData;
		bp0e++;
		//^2066:00EC
	}
	//^2066:0104
	_4976_5254 = bp04;
	_4976_524e = bp05;
	_4976_5258 += bp0a;

	return 0;
}

//^2066:0123
Bit8u SkWinCore::WRITE_1BIT(Bit8u nibble)
{
	//^2066:0123
	Bit8u data = nibble;
	static const Bit8u mask = 0x01;

	return SUPPRESS_WRITER(&data, &mask, 1, 1);
}

//^2066:06E8
Bit8u SkWinCore::WRITE_RECORD_CHECKCODE(ObjectID recordLink, Bit8u writeDir, Bit8u writeFollowing)
{
	// writeDir=(write-direction-in-record-link)
	// yy=(write-subsequent-records)

	//^2066:06E8

	//^2066:0AE6
	while (recordLink != OBJECT_END_MARKER && recordLink != OBJECT_NULL) {
		//^2066:06F4
		Bit8u db = recordLink.DBType();
		if (db > dbActuator) {
			//^2066:0704
			if (WRITE_1BIT(1)) // write#more-record=true
				return 1;
			Bit8u data = db;
			Bit8u mask = 0x0F;
			if (SUPPRESS_WRITER(&data, &mask, 1, 1)) // write#dbtype
				return 1;
			//^2066:073A
			if (writeDir != 0 && db != dbCreature) {
				Bit8u data = recordLink.Dir();
				Bit8u mask = 0x03;
				if (SUPPRESS_WRITER(&data, &mask, 1, 1))
					return 1;
			}
		}
		//^2066:0770
		if (db == dbCloud && _4976_3b55 != 0) {
			Bit16u data = recordLink;
			Bit16u mask = 0x007F;
			if (SUPPRESS_WRITER(&data, &mask, 2, 1))
				return 1;
			break; //->2066:0AF3
		}

		//^2066:07A6
		const void *recordMask = tableMask[db];

		//^2066:07BA
		if (recordMask != NULL) {
			//^2066:07C1
			GenericRecord *record = GET_ADDRESS_OF_RECORD(recordLink);
			//^2066:07CE
			Bit16u _di = 0;
			//^2066:07D0
			switch (db) {
				case dbActuator:	// 3	(or 11-12-13 in extended mode)
					{
						Actuator *bp04 = record->castToActuator();

						//^2066:07E5
						switch (bp04->ActuatorType()) {
						case ACTUATOR_FLOOR_TYPE__CROSS_SCENE: // 0x27: 'Cross scene'
						case ACTUATOR_TYPE_X1B: // 0x1B: '-'
						case ACTUATOR_TYPE_COUNTER: // 0x1D: 'Activator, counter'
						case ACTUATOR_TYPE_ORNATE_STEP_ANIMATOR: // 0x41: 'Ornate step animator'
						case ACTUATOR_FLOOR_TYPE__INFINITE_ORNATE_ANIMATOR: // 0x2C: 'Continuous ornate animator'
						case ACTUATOR_FLOOR_TYPE__ORNATE_ANIMATOR: // 0x32: 'Activator, ornate animator'
						case ACTUATOR_FLOOR_TYPE__SHOP: // 0x30: 'Shop exhibition'
						case ACTUATOR_FLOOR_TYPE__FINITE_ACTIVATOR_RELAY: // 0x2D: 'Activator, finite activator relay'
							{
								//^2066:0825
								Bit16u data = bp04->ActuatorData();
								Bit16u mask = 0x01FF;
								if (SUPPRESS_WRITER(&data, &mask, 2, 1))
									return 1;
								break; //->2066:0926
							}
						default:
							{
								break; //->2066:0926
							}
						}
						break;
					}
				case dbCreature:	// 4
					{
						Creature *bp04 = record->castToCreature();

						//^2066:085B
						Bit8u data = bp04->CreatureType();
						Bit8u mask = 0x7F;
						if (SUPPRESS_WRITER(&data, &mask, 1, 1))
							return 1;
						//^2066:0887
						if (QUERY_CREATURE_AI_SPEC_FLAGS(recordLink) & 1) {
							//^2066:0893
							recordMask = _4976_3ac7;
						}
						//^2066:089B
						_4976_5246[recordLink.DBIndex()] = _4976_3952;
						_4976_3952++;
						break;
					}
				case dbWeapon:		// 5
				case dbCloth:		// 6
				case dbScroll:		// 7
				case dbPotion:		// 8
				case dbMiscellaneous_item: // 10
				case db11:
				case db12:
					{
						break; //->2066:0926
					}
				case dbContainer:	// 9
					{
						Container *bp04 = record->castToContainer();

						//^2066:08B4
						Bit8u data = bp04->ContainerType();
						Bit8u mask = 0x03;
						if (SUPPRESS_WRITER(&data, &mask, 1, 1))
							return 1;
						//^2066:08E5
						if (IS_CONTAINER_MAP(recordLink) != 0) {
							//^2066:08F0
							recordMask = _4976_3aef;
							_di = 1;
						}
						//^2066:08FB
						_4976_5240[recordLink.DBIndex()] = _4976_3950;
						_4976_3950++;
						break; //->2066:0926
					}
				case dbMissile:		// 14
					{
						//^2066:0914
						if (_4976_3b57 == 0)
							break;
						recordMask = _4976_3b0b;
						_di = 1;
						break;
					}
			}
			//^2066:0926  
			if (SUPPRESS_WRITER(record, recordMask, dbSize[db], 1))
				return 1;
			//^2066:094E
			switch (db) {
				case dbWeapon:		// 5
				case dbCloth:		// 6
				case dbScroll:		// 7
				case dbPotion:		// 8
				case dbMiscellaneous_item: // 10
				case db11:
				case db12:
				case db13:
					{
						break; //->2066:0AD7
					}
				case dbCreature:	// 4
					{
						Creature *bp04 = record->castToCreature();

						//^2066:0963
						_4976_3b57 = 0x0001;

						Bit8u res = WRITE_RECORD_CHECKCODE(
							bp04->GetPossessionObject(),
							recordMask != _4976_3ab7,
							1
							);
						if (res)
							return 1;

						//^2066:0996
						_4976_3b57 = 0x0000;
						break; //->2066:0AD7
					}
				case dbContainer:	// 9
					{
						Container *bp04 = record->castToContainer();

						//^2066:099F
						if (_di != 0) {
							//^2066:09A3
							ObjectID bp10 = bp04->GetContainedObject();
							//^2066:09AD
							Bit16u bp18 = (bp10 == OBJECT_NULL || bp10 == OBJECT_END_MARKER) ? 0 : 1;
							//^2066:09C3
							if (WRITE_1BIT((Bit8u)bp18))
								return 1;
							//^2066:09D2
							if (!(bp18 != 0))
								break;
							//^2066:09DB
							ADD_MINION_ASSOC(recordLink);
							break;
						}
						//^2066:09E4
						if (IS_CONTAINER_MONEYBOX(recordLink) != 0) {
							//^2066:09EF
							tableMask[dbMiscellaneous_item] = _4976_3afb;
						}
						//^2066:09F9
						if (WRITE_RECORD_CHECKCODE(bp04->GetContainedObject(), 0, 1))
							return 1;
						//^2066:0A12  
						if (IS_CONTAINER_MONEYBOX(recordLink) != 0) {
							//^2066:0A20
							tableMask[dbMiscellaneous_item] = _4976_3af7;
						}
						//^2066:0A2A  
						break; //->2066:0AD7
					}
				case dbMissile:		// 14
					{
						Missile*bp04 = record->castToMissile();

						//^2066:0A2D  
						if (_di != 0) {
							//^2066:0A31  
							ADD_MINION_ASSOC(recordLink);
							//^2066:0A37  
							break; //->2066:0AD7
						}
						//^2066:0A3A  
						_4976_3b55 = 0x0001;
						Bit8u res = WRITE_RECORD_CHECKCODE(
							bp04->GetMissileObject(),
							0,
							0
							);
						if (res != 0)
							return 1;
						//^2066:0A59
						_4976_3b55 = 0x0000;
						break; //->2066:0AD7
					}
				case dbCloud:		// 15
					{
						//^2066:0A62  
						Timer *bp0c = glbTimersTable;
						Bit16u bp14 = 0;
						//^2066:0AC3
						while (bp14 < glbTimersActiveCount) {
							//^2066:0A76
							if (bp0c->TimerType() == ttyCloud && bp0c->id8() == recordLink) {
								//^2066:0A86
								if (WRITE_1BIT(0x0001))
									return 1;
								//^2066:0A94
                                Bit16u mask = 0x03FF;
								Bit16u data = bp14;
								if (SUPPRESS_WRITER(&data, &mask, 2, 1))
									return 1;
								break; //->2066:0AD7
							}
							//^2066:0ABC
							bp0c++;
							bp14++;
						}
						//^2066:0ACC
						if (WRITE_1BIT(0x0000))
							return 1;
						break; //->2066:0AD7
					}
			}
		}
		//^2066:0AD7
		if (writeFollowing == 0)
			break;
		recordLink = GET_NEXT_RECORD_LINK(recordLink);

	}
	//^2066:0AF3
	if (writeFollowing == 0) {
		//^2066:0AF9
		if (recordLink != OBJECT_NULL)
			return 0;
	}
	//^2066:0AFE
	if (WRITE_1BIT(0))
		return 1;
	//^2066:0B09
	return 0;
}

//^2066:0B44
// _2066_0b44 renamed _2066_0b44_WRITE_TIMERS
i16 SkWinCore::_2066_0b44_WRITE_TIMERS()
{
	//^2066:0B44
	Timer *bp04 = glbTimersTable;
	Bit16u i = 0;
	//^2066:0B58
	while (i < glbTimersCount) {
		switch (bp04->TimerType()) {
			case tty3C:
			case tty3D:
				//^2066:0B6B
				if (WRITE_RECORD_CHECKCODE(bp04->id8(), 0, 0) != 0) {
					//^2066:0B81
					return 1;
				}
				break;
		}
		//^2066:0B86
		bp04++;
		i++;
	}
	//^2066:0B91
	return 0;
}

//^2066:0147
U16 SkWinCore::SUPPRESS_FLUSH()
{
	// TODO: VERIFY ME

	//^2066:0147
	Bit8u bp01 = _4976_524e;
	//^2066:0151
	if (bp01 == 0)
		return 0;
	Bit8u bp02 = 8 - bp01;
	Bit8u bp03 = _4976_5254;
	//^2066:0163
	while (bp02-- != 0) {
		//^2066:0165
		bp03 = (((bp03 & 0x80) == 0) ? 0 : 1) | (bp03 << 1);
		//^2066:017B
	}
	//^2066:0185
	Bit8u bp04 = bp03;
	//^2066:018B
	if (FILE_WRITE(glbDataFileHandle, 1, &bp04) == 0)
		return 1;
	//^2066:01A9
	_4976_524e = _4976_5254 = 0;
	_4976_5258 = bp01;
	//^2066:01BF
	return 0;
}

//^2066:046D
void SkWinCore::_2066_046d() { // TODO: Unr
	Unr();
}

//^2066:38D9
//SPX : Read the 10 savegame names
void SkWinCore::_2066_38d9()
{
	//^2066:38D9
	ENTER(2);
	//^2066:38DE
	for (i16 bp02 = 0; bp02 < 10; bp02++) {
		//^2066:38E6
		glbSKSaveDigitAlpha = bp02 +0x30;
		//^2066:38EE
		i16 si = OPEN_FILE(FORMAT_SKSTR(ptrSKSave_dat, NULL));
		//^2066:390D
		if (si >= 0) {
			//^2066:3911
			READ_FILE(si, 42, &_4976_5250[bp02]);
			//^2066:3931
			CLOSE_FILE(si);
			//^2066:3938

			// SPX : native DM1 savegames don't have the same format
			if (_4976_5250[bp02].wTimerFlag != 1)
			{
				sprintf((char*)_4976_5250[bp02].sSavegameName, "SKSAVE%d.DAT (NOT DM2 NATIVE)", bp02);
			}
		}
		else {
			//^2066:393A
			_4976_3b5f[0] = glbSKSaveDigitAlpha;
			//^2066:3940
			SK_STRCPY(_4976_5250[bp02].sSavegameName, _4976_3b5f);
			//^2066:3961
			_4976_5250[bp02].w38 = 0xBEEF;
			_4976_5250[bp02].w40 = 0xDEAD;
		}
		//^2066:397B
	}
	//^2066:3987
	return;
}

//^2066:3697
// SPX: _2066_3697 renamed __OPEN_DIALOG_PANEL
void SkWinCore::__OPEN_DIALOG_PANEL(U8 cls2, U16 yy)
{
	//^2066:3697
	ENTER(104);
	//^2066:369B
	_4976_5250 = reinterpret_cast<sksave_header_asc *>(ALLOC_MEMORY_RAM(420, afDefault, 1024));
	//^2066:36B4
	U8 bp6a[40];
	// SPX: Get dialog box button 1 text
	U8 *bp18 = QUERY_GDAT_TEXT(0x1a, cls2, 0x00, bp6a); // LOAD
	//^2066:36CF
	U8 bp40[40];
	// SPX: get dialog box button 2 text
	U8 *bp14 = QUERY_GDAT_TEXT(0x1a, cls2, 0x01, bp40); // CANCEL
	//^2066:36EA
	U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x1a, cls2, 0x00);
	//^2066:3700
	U8 *bp08;
	if (_4976_5d76 != 0) {
		//^2066:3707
		bp08 = QUERY_GDAT_IMAGE_LOCALPAL(0x1a, cls2, 0x00);
	}
	else {
		//^2066:371C
		bp08 = bp04 +CALC_IMAGE_BYTE_LENGTH(bp04);
	}
	//^2066:3737
	SRECT bp10;
	DRAW_DIALOGUE_PARTS_PICT(bp04, QUERY_EXPANDED_RECT(4, &bp10), -1, bp08);
	//^2066:375E
	FREE_PICT_ENTRY(bp04);
	//^2066:376B
	FIRE_FADE_SCREEN(1);
	// RESUME SCREEN with list of savegame, version number, LOAD and CANCEL button
	//^2066:3773
	DRAW_VP_RC_STR(0x1c2, glbPaletteT16[COLOR_GRAY], strVersionNumber); // _4976_3b6f replaced by strVersionNumber
	//^2066:378D
	DRAW_VP_RC_STR(0x1d2, glbPaletteT16[COLOR_ORANGE], bp18);	// LOAD text
	//^2066:37A9
	DRAW_VP_RC_STR(0x1d3, glbPaletteT16[COLOR_ORANGE], bp14);	// CANCEL text
	//^2066:37C5
	_2066_38d9();
	//^2066:37CA
	_4976_4bd2 = yy;
	//^2066:37D0
	_1031_0675(4);
	//^2066:37D8
	_4976_523a = 0;
	//^2066:37DE
	while (glbMouseVisibility > 0) {
		//^2066:37E0
		FIRE_SHOW_MOUSE_CURSOR();
		//^2066:37E5
		_4976_523a++;
		//^2066:37E9
	}
	//^2066:37F0
	return;
}

//^2066:33C4
void SkWinCore::_2066_33c4(U8 *str, U16 yy) 
{
	//^2066:33C4
	ENTER(0);
	//^2066:33C7
	SK_STRCPY(str, _4976_3b68);
	str[5] = yy +0x30;
	//^2066:33E5
	return;
}

//^2066:398A
void SkWinCore::_2066_398a(i16 xx)
{
	//^2066:398A
	ENTER(10);
	//^2066:3990
	SRECT bp0a;
	QUERY_EXPANDED_RECT(451, &bp0a);
	//^2066:39A0
	i16 di = bp0a.y +4;
	//^2066:39A8
	for (i16 si = 0; si < 10; si++) {
		//^2066:39AC
		U16 bp02 = (si == xx) ? glbPaletteT16[COLOR_YELLOW] : glbPaletteT16[COLOR_ORANGE];
		//^2066:39C8
		DRAW_VP_STR(bp0a.x, di, bp02, _4976_5250[si].sSavegameName);
		//^2066:39EB
		di += glbPanelStatsYDelta;
		//^2066:39EF
	}
	//^2066:39F5
	return;
}
//^2066:3820
void SkWinCore::_2066_3820(U8 *xx, U16 yy)
{
	//^2066:3820
	ENTER(8);
	//^2066:3824
	SRECT bp08;
	QUERY_EXPANDED_RECT(0x1c5, &bp08);
	//^2066:3834
	DRAW_DIALOGUE_PICT(QUERY_GDAT_IMAGE_ENTRY_BUFF(0x1a, 0x81, 0x00), _4976_4c16, &bp08, 
		bp08.x, bp08.y, -1, QUERY_GDAT_IMAGE_LOCALPAL(0x1a, 0x81, 0x00));
	//^2066:3873
	DRAW_VP_STR(bp08.x, bp08.y +4, glbPaletteT16[COLOR_YELLOW], xx);
	//^2066:3896
	if (yy != 0) {
		//^2066:389C
		bp08.x += SK_STRLEN(xx) * _4976_0126;
		bp08.cx = _4976_0126;
		bp08.cy -= 4;
		//^2066:38BA
		FIRE_FILL_BACKBUFF_RECT(&bp08, glbPaletteT16[COLOR_ORANGE]);
	}
	//^2066:38D2
	_0aaf_002f();
	//^2066:38D7
	return;
}

//^3929:0B01
void SkWinCore::_3929_0b01(U16 xx, U16 yy) { // TODO: Unr
	Unr();
}

//^3929:0B20
void SkWinCore::_3929_0b20(U16 xx, U32 yy) { // TODO: Unr
	Unr();
}

//^3E74:5B7C
void SkWinCore::_3e74_5b7c(U16 xx)
{
	//^3E74:5B7C
	ENTER(20);
	//^3E74:5B80
	_476d_05b6(xx);
	//^3E74:5B89
	if (_4976_484d++ == _4976_484f[RCJ(4,_4976_484d)]) {
		//^3E74:5B9F
		if (_4976_484f[RCJ(4,_4976_484d)] != 0)
			//^3E74:5BAA
			return;
		//^3E74:5BAD
		const U8 *bp04 = _4976_4853;
		//^3E74:5BB5
		DISPLAY_HINT_TEXT(COLOR_WHITE, bp04);
		//^3E74:5BC5
		U8 bp14[16];
		DISPLAY_HINT_TEXT(COLOR_WHITE, SK_LTOA10(2, bp14));
		//^3E74:5BE2
		_3929_0b01(15, 46);
		//^3E74:5BED
		_3929_0b20(15, 8);
		//^3E74:5BFB
		_3929_0b20(15, _4976_5e9c);
		//^3E74:5C0D
		_3929_0b20(15, _4976_5ea6);
		//^3E74:5C1F
		_3929_0b20(15, _4976_5d66);
		//^3E74:5C31
		bp04 = _4976_485a;
		//^3E74:5C39
		SK_STRCPY(bp14, bp04);
		//^3E74:5C4C
		if (_4976_5bee[0] != 0)
			//^3E74:5C53
			bp14[0] = 0x47;
		//^3E74:5C57
		if (_4976_5bee[1] != 0)
			//^3E74:5C5E
			bp14[1] = 0x53;
		//^3E74:5C62
		if (_4976_5bee[2] != 0)
			//^3E74:5C69
			bp14[2] = 0x45;
		//^3E74:5C6D
		DISPLAY_HINT_TEXT(COLOR_WHITE, bp14);
	}
	//^3E74:5C7C
	_4976_484d = 0;
	//^3E74:5C82
	return;
}

//^1031:0D36
void SkWinCore::_1031_0d36(U16 xx, U16 yy)
{
	//^1031:0D36
	ENTER(0);
	//^1031:0D3A
	_3e74_5b7c(yy);
	//^1031:0D43
	_4976_19a7 = 1;
	//^1031:0D49
	if (_4976_4e00 < 7) {
		//^1031:0D50
		i16 si = _4976_19a5 +2;
		//^1031:0D57
		if (si > 10)
			//^1031:0D5C
			si -= 11;
		//^1031:0D5F
		if (si-- == 0)
			//^1031:0D66
			si = 10;
		//^1031:0D69
		_4976_19a5 = si;
		//^1031:0D6E
		_4976_4e02[_4976_19a5].MouseButton(xx);
		//^1031:0D7C
		_4976_4e02[_4976_19a5].MouseX(yy);
		//^1031:0D8D
		_4976_4e00++;
	}
	//^1031:0D91
	_4976_19a7 = 0;
	//^1031:0D97
	_1031_0b7e();
	//^1031:0D9B
	return;
}

//^01B0:054A
U8 SkWinCore::_01b0_054a(U16 xx) //#DS=04BF
{
	//^01B0:054A
	ENTER(0);
	//^01B0:054F
	U16 di = xx;
	//^01B0:0552
	LOADDS(0x3083);
	//^01B0:0558
	if ((di & 0x1000) != 0)
		//^01B0:055E
		di = 0;
	//^01B0:0560
	U16 si = ((di & 0x0200) != 0) ? (_04bf_033e[di & 0x007f]) : (_04bf_02be[di & 0x007f]);
    //^01B0:057E
	if (si != 0 && (di & 0x0400) != 0)
		//^01B0:0588
		si &= 0x001f;
	//^01B0:058E
	return U8(si);
}

//^476D:05B6
U8 SkWinCore::_476d_05b6(U16 xx)
{
	//^476D:05B6
	ENTER(0);
	//^476D:05B9
	return _01b0_054a(xx) CALL_IBMIO;
}

//^2066:37F2
void SkWinCore::_2066_37f2()
{
	//^2066:37F2
	ENTER(0);
	//^2066:37F5
	while (_4976_523a-- != 0)
		//^2066:37F7
		FIRE_HIDE_MOUSE_CURSOR();
	//^2066:3807
	_1031_06a5();
	//^2066:380C
	_4976_022c = 1;
	//^2066:3812
	DEALLOC_UPPER_MEMORY(420);
	//^2066:381E
	return;
}

//^2066:33E7
i16 SkWinCore::_2066_33e7()
{
	//^2066:33E7
	ENTER(22);
	//^2066:33ED
	__OPEN_DIALOG_PANEL(0x81, 6);
	//^2066:33F9
	i16 si;
	if (_4976_5bf6 != 0) {
		//^2066:3400
		si = glbSKSaveNum;
		//^2066:3404
		COPY_MEMORY(&_4976_5250[si], &_4976_5266, 42);
	}
	else {
		//^2066:3428
		for (si = 0; si < 10; si++) {
			//^2066:342C
			if (true
				&& _4976_5250[si].w40 == 0xDEAD
				&& _4976_5250[si].w38 == 0xBEEF
			) {
				//^2066:3449
				_2066_33c4(_4976_5268, si);
				//^2066:3455
				break;
			}
			//^2066:3457
		}
	}
	//^2066:345D
	_2066_398a(si);
	//^2066:3464
	U8 *bp04 = _4976_5268;
	//^2066:346C
	if (si != 10) {
		//^2066:3471
		_2066_3820(bp04, 0);
	}
	else {
		//^2066:3483
		_0aaf_002f();
	}
	//^2066:3488
	U16 bp0c = 0;

	do {
		//^2066:348D
		while (_476d_05a9() != 0) {
			//^2066:348F
			_1031_0d36(32, _476d_050e());
			//^2066:349E
		}
		//^2066:34A7
		MAIN_LOOP();
		//^2066:34AC
		SRECT bp14;
		i16 bp06;
		i16 bp08;
		i16 bp0a;
		i16 di = SK_STRLEN(bp04); // defaulting to situational value.
		switch (_4976_4dfc) {
			case 1:
				//^2066:34C0
				si = -1;
				//^2066:34C3
				_4976_4c0a = 0;
				//^2066:34C9
				bp0c = 1;
				//^2066:34CE
				break;

			case 2:
				//^2066:34D1
				_4976_4c0a = 0;
				//^2066:34D7
				if (si == 10)
					//^2066:34DC
					break;
				//^2066:34DF
				if (*bp04 == 0) {
					//^2066:34E8
					_2066_33c4(bp04, si);
				}
				//^2066:34F6
				_2066_3820(bp04, 0);
				//^2066:3506
				bp0c = 1;
				//^2066:350B
				break;

			case 3:
				//^2066:350E
				_4976_4c0a = 0;
				//^2066:3514
				QUERY_EXPANDED_RECT(_4976_4df8, &bp14);
				//^2066:3525
				QUERY_TOPLEFT_OF_RECT(_4976_4e44, &bp06, &bp08);
				//^2066:353B
				bp0a = _4976_4e68;
				//^2066:3541
				bp0a -= bp14.y +bp08;
				//^2066:354A
				si = min_value(bp0a / glbPanelStatsYDelta, 10);
				//^2066:355E
				if (true
					&& _4976_5250[si].w40 == 0xDEAD
					&& _4976_5250[si].w38 == 0xBEEF
				) {
					//^2066:3579
					_2066_33c4(_4976_5268, si);
				}
				else {
					//^2066:3587
					SK_STRCPY(_4976_5268, _4976_5250[si].sSavegameName);
				}
				//^2066:35A7
				_2066_398a(si);
				//^2066:35AE
				_2066_3820(bp04, 0);
				//^2066:35BE
				break;

			case 4:
				//^2066:35C0
				if (si != 10 && _4976_4c0a == 0) {
					//^2066:35CC
					_4976_4c0a = 1;
					//^2066:35D2
					_2066_3820(bp04, 1);
					//^2066:35E2
					di = SK_STRLEN(bp04);
				}

				break;
		}
		//^2066:35F1
		//if (_4976_4c0a != 0 && glbUIEventCode >= 0xa5 && glbUIEventCode <= 0xd6) {
		if (_4976_4c0a != 0 && glbUIEventCode >= UI_EVENTCODE_0A5 && glbUIEventCode <= UI_EVENTCODE_0D6) {
			//^2066:3608
			if (_4976_4e9c == 14) {
				//^2066:360F
				if (di > 0) {
					//^2066:3613
					di--;
					//^2066:3614
					bp04[di] = 0;
				}
			}
			//^2066:361D
			else if (_4976_4e9c == 28) {
				//^2066:3624
				_4976_4c0a = 0;
			}
			//^2066:362C
			else if (di < 31) {
				//^2066:3631
				U8 bp15 = _476d_05b6(_4976_4e9c);
				//^2066:363E
				if (bp15 >= 0x61 && bp15 <= 0x7a) {
					//^2066:364A
					bp15 += 0xe0;
				}
				//^2066:364F
				bp04[di] = bp15;
				//^2066:3658
				bp04[++di] = 0;
			}
			//^2066:365D
			_2066_3820(bp04, _4976_4c0a);
		}
		//^2066:366F
		_4976_4dfc = 0;
		glbUIEventCode = 0;
		//^2066:367B
	} while (bp0c == 0);
	//^2066:3684
	_2066_37f2();
	//^2066:3689
	return si;
}

//^1C9A:3BAB
void SkWinCore::_1c9a_3bab()
{
	//^1C9A:3BAB
	ENTER(0);
	//^1C9A:3BAF
	for (U16 si = 0; _4976_1a68 > 0; si++) {
		//^1C9A:3BB3
		if (glbTabCreaturesInfo[si].CreatureIndex() >= 0) {
			//^1C9A:3BC6
			_1c9a_0fcb(si);
		}
	}
	//^1C9A:3BD4
	return;
}

//^1C9A:3B74
void SkWinCore::FILL_ORPHAN_CAII()
{
	//printf("FILL_ORPHAN_CAII\n"); getch();
	//^1C9A:3B74
	ENTER(2);
	//^1C9A:3B7A
	U16 di = glbCurrentMapIndex;
	//^1C9A:3B7E
	U16 bp02 = dunHeader->nMaps;
	//^1C9A:3B8B
	for (i16 si = 0; si < bp02; si++) {
		//printf("FILL_ORPHAN_CAII %d < %d \n", si, bp02); getch();
		//^1C9A:3B8F
		//printf("CHANGE_CURRENT_MAP_TO %d\n", si); getch();
		CHANGE_CURRENT_MAP_TO(si);
		//^1C9A:3B96
		//printf("FILL_CAII_CUR_MAP\n"); getch();
		FILL_CAII_CUR_MAP();
	}
	//^1C9A:CHANGE_CURRENT_MAP_TO
	//printf("CHANGE_CURRENT_MAP_TO %d\n", di); getch();
	CHANGE_CURRENT_MAP_TO(di);
	//^1C9A:3BA7
	//printf("FILL_ORPHAN_CAII\n"); getch();
	return;
}

//^1C9A:3BD7
void SkWinCore::RESET_CAII()
{
	//printf("RESET_CAII\n"); getch();
	//^1C9A:3BD7
	ENTER(4);
	//^1C9A:3BDC
	_4976_1a68 = 0;
	//^1C9A:3BE2
	U16 si;
	for (si = 0; si < glbCreaturesCount; si++) {
		//^1C9A:3BE6
		glbTabCreaturesInfo[si].CreatureIndex(-1);
		//^1C9A:3BF8
	}
	//^1C9A:3BFF
	Creature *bp04 = reinterpret_cast<Creature *>(static_cast<U8 *>(glbDBObjectData[4]));
	//^1C9A:3C0C
	for (si = dunHeader->nRecords[dbCreature]; si-- != 0; bp04++) {
		//^1C9A:3C16
		bp04->b5 = 0xff;
	}
	//^1C9A:3C29
	//printf("FILL_ORPHAN_CAII\n");
	// getch();
	FILL_ORPHAN_CAII();
	//^1C9A:3C2D
	//printf("RESET_CAII end\n"); getch();
	return;
}


//^0CD5:0039
void SkWinCore::FILL_U16(i16 *buff, X16 cnt, i16 val, i16 delta)
{
	//^0CD5:0039
	ENTER(0);
	//^0CD5:003E
	X16 si = 0;
	delta >>= 1;
	//^0CD5:0043
	for (U16 di = 0; di<cnt; di++) {
		//^0CD5:0047
		buff[si] = val;
		si += delta;
		//^0CD5:0059
	}
	//^0CD5:005F
	return;
}

//^19F0:045A
void SkWinCore::_19f0_045a(i16 xx, i16 yy)
{
	//^19F0:045A
	ENTER(0);
	//^19F0:045D
	if (xx == glbCreatureSomeX && yy == glbCreatureSomeY && glbCurrentMapIndex == glbCreatureSomeZMap)
		//^19F0:0476
		return;
	//^19F0:0478
	glbCreatureSomeZMap = glbCurrentMapIndex;
	_4976_521c = GET_TILE_VALUE(glbCreatureSomeX = xx, glbCreatureSomeY = yy);
	//^19F0:0498
	_4976_5222 = _4976_5220 = _4976_521e = ((_4976_521c & 0x10) != 0) ? OBJECT_NULL : OBJECT_END_MARKER;
	_4976_5224 = _4976_5225 = 0;
	_4976_522c = 0xff;
	//^19F0:04BD
	return;
}

//^19F0:0207
i16 SkWinCore::_19f0_0207(i16 xx, i16 yy, i16 ss, i16 tt, U16 (SkWinCore::*hazardTest)(i16 xx, i16 yy))
{
	// hazardTest returns 0 if tile(xx, yy) is safe, otherwise it returns 1 if tile is in hazard.

	// (xx, yy) is your location. (creature's position.)
	// (ss, tt) is your destination. (player's position.)

	//^19F0:0207
	ENTER(16);
	//^19F0:020D
	if (ABS16(xx -ss) + ABS16(yy -tt) <= 1)
		//^19F0:0230
		return 1;
	//^19F0:0236
	i16 di = (ss -xx);
	di = (di < 0) ? -di : di;
	//^19F0:024B
	i16 si = (tt -yy);
	si = (si < 0) ? -si : si;
	//^19F0:0260
	X16 bp08 = (di < si) ? 1 : 0;
	//^19F0:026E
	X16 bp0e = (di == si) ? 1 : 0;
	//^19F0:027C
	di = ss;
	i16 bp04 = (di -xx > 0) ? -1 : +1;
	//^19F0:0291
	si = tt;
	i16 bp06 = (si -yy > 0) ? -1 : +1;
	//^19F0:02A6
	i16 bp02;
	i16 bp10;
	if (bp08 != 0) {
		//^19F0:02AC
		bp02 = si -yy;
		bp10 = (bp02 != 0) ? ((di -xx) << 6) / bp02 : 128;
	}
	else {
		//^19F0:02CB
		bp02 = di -xx;
		bp10 = (bp02 != 0) ? ((si -yy) << 6) / bp02 : 128;
	}
	do {
		//^19F0:02EB
		i16 bp0a;
		i16 bp0c;
		if (bp0e != 0) {
			//^19F0:02F1
			if (true
				&& ((this->*hazardTest)(di +bp04, si) == 0 || (this->*hazardTest)(di, si +bp06) == 0)
				&& ((this->*hazardTest)(di += bp04, si += bp06) == 0)
			) {
				//^19F0:0326
				continue;
			}
			//^19F0:0329
			//^19F0:041A
			return 0;
		}
		//^19F0:032C
		if (bp08 != 0) {
			//^19F0:0332
			bp02 = si -yy;
			bp02 = ((bp02 != 0) ? ((di +bp04 -xx) << 6) / bp02 : 128) -bp10;
		}
		else {
			//^19F0:0354
			bp02 = di +bp04 -xx;
			bp02 = ((bp02 != 0) ? ((si -yy) << 6) / bp02 : 128) -bp10;
		}
		//^19F0:037A
		bp0a = (bp02 < 0) ? -bp02 : bp02;
		//^19F0:0388
		if (bp08 != 0) {
			//^19F0:038F
			bp02 = si +bp06 -yy;
			bp02 = ((bp02 != 0) ? ((di -xx) << 6) / bp02 : 128) -bp10;
		}
		else {
			//^19F0:03B1
			bp02 = di -xx;
			bp02 = ((bp02 != 0) ? ((si +bp06 -yy) << 6) / bp02 : 128) -bp10;
		}
		//^19F0:03D7
		bp0c = (bp02 < 0) ? -bp02 : bp02;
		//^19F0:03E5
		if (bp0a < bp0c)
			di += bp04;
		else
			si += bp06;
		//^19F0:03F2
		if (true
			&& (this->*hazardTest)(di, si) != 0
			&& (bp0a != bp0c || (this->*hazardTest)(di += bp04, si -= bp06) != 0)
		) {
			//^19F0:041A
			return 0;
		}
		//^19F0:041E
	} while (ABS16(di -xx) + ABS16(si -yy) > 1);

	//^19F0:0442
	return CALC_SQUARE_DISTANCE(xx, yy, ss, tt);
}

//^19F0:00B8
// SPX: this function serves as 'hazardtest'
U16 SkWinCore::_19f0_00b8(i16 xx, i16 yy)
{
	//^19F0:00B8
	ENTER(8);
	//^19F0:00BE
	U16 bp04 = GET_TILE_VALUE(xx, yy);
	U16 bp02 = bp04 >> 5;
	if (bp02 != ttWall) {
		//^19F0:00DF
		if (bp02 == ttDoor) {
			//^19F0:00E5
			U16 di = U8(bp04) & 7;
			if (di == 3 || di == 4) {
				//^19F0:00F8
				if (false
					|| GET_DOOR_STAT_0X10(GET_GRAPHICS_FOR_DOOR(GET_ADDRESS_OF_TILE_RECORD(U8(xx), U8(yy))->castToDoor())) == 0
					|| RAND01() == 0
				) {
					goto _01cf;
				}
			}
		}
		//^19F0:0128
		if (bp02 == ttTrickWall && (bp04 & 4) == 0)
			goto _01cf;
		//^19F0:0138
		if ((bp04 & 0x10) != 0) {
			//^19F0:0141
			for (ObjectID si = GET_WALL_TILE_ANYITEM_RECORD(xx, yy); si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
				//^19F0:0150
				U16 bp06 = si.DBType();
				if (bp06 == dbCloud || GET_ADDRESS_OF_RECORDF(si)->CloudType() == 14)
					goto _01cf;
				//^19F0:0177
				if (bp06 == dbCreature) {
					ObjectID bp08 = _1c9a_03cf(&xx, &yy, 255);
					if (bp08 != OBJECT_NULL) {
						//^19F0:019A
						U16 di = QUERY_CREATURE_AI_SPEC_FLAGS(bp08);
						if ((di & 1) != 0) {
							//^19F0:01AA
							if (((di >> 6) & 3) >= 2)
								continue;
							goto _01cf;
						}
						//^19F0:01B7
						if ((di & 0x20) == 0)
							goto _01cf;
					}
				}
				//^19F0:01BD
			}
		}
        //^19F0:01CB
		return 0;
	}
	//^19F0:01CF
_01cf:
	return 1;
}

//^19F0:04BF
ObjectID SkWinCore::_19f0_04bf()
{
	//^19F0:04BF
	ENTER(0);
	//^19F0:04C3
	ObjectID si = _4976_5220;
	if (si == OBJECT_NULL) {
		//^19F0:04CD
		si = _4976_521e;
		if (si == OBJECT_NULL) {
			//^19F0:04D7
			_4976_521e = GET_TILE_RECORD_LINK(glbCreatureSomeX, glbCreatureSomeY);
			//^19F0:04E9
		}
		//^19F0:04F4
		for (si = _4976_521e; si != OBJECT_END_MARKER && si.DBType() <= dbActuator; si = GET_NEXT_RECORD_LINK(si));
		//^19F0:0506
		_4976_5220 = si;
	}
	//^19F0:050A
	return si;
}

//^19F0:0559
U16 SkWinCore::_19f0_0559(X16 xx)
{
	//^19F0:0559
	SkD((DLV_DBG_SED3, "DBG: _19f0_0559 %04X \n", (Bitu)xx));
	ENTER(4);
	//^19F0:055E
	X16 si = xx;
	X16 bp04 = glbCurrentThinkingCreatureRec->b15 & 3;
	X16 bp02;
	//^19F0:056F
	if (((si +2) & 3) == bp04) {
		if (RAND01() != 0)
			goto _059e;
		bp02 = 0xffff;
	}
	//^19F0:058E
	else if (bp04 != si) {
		if (((si -1) & 3) == bp04)
			//^19F0:059E
_059e:
			bp02 = 1;
		else
			bp02 = 0xffff;
	}
	else {
		//^19F0:05A7
		glbCurrentThinkingCreatureData->Command = ccmNeutral;
		_4976_4ee5 = xactrYes;
		//^19F0:05B5
		return 0;
	}
	//^19F0:05B9
	glbCurrentThinkingCreatureData->b29 = (U8(bp04) +U8(bp02)) & 3;
	glbCurrentThinkingCreatureData->Command = (bp02 == 0xffff) ? ccm06 : ccm07;
	_4976_4ee5 = xactrAgain;
	//^19F0:05E2
	return 1;
}

//^075F:1BC2
void SkWinCore::_075f_1bc2(U8 zz[4], U16 xx, U16 yy, i16 ss, i16 tt, X16 ww)
{
	//^075F:1BC2
	ENTER(2);
	//^075F:1BC8
    X16 di = ww;
	U16 si = CALC_VECTOR_DIR(xx, yy, ss, tt);
	//^075F:1BE0
	if (di == 255) {
		//^075F:1BE6
		COPY_MEMORY(&_4976_00dc[RCJ(8,si)], zz, 4);
		if (RAND01() != 0) {
			//^075F:1C06
			U8 bp01 = zz[0];
			zz[0] = zz[1];
			zz[1] = bp01;
		}
		//^075F:1C1D
		if (RAND01() != 0) {
			U8 bp01 = zz[2];
			zz[2] = zz[3];
			zz[3] = bp01;
		}
		return;
	}
	//^075F:1C41
	si <<= 1;
	if ((si & 2) == 0)
		di++;
	//^075F:1C4B
	si += (di >> 1) & 1;
	COPY_MEMORY(&_4976_00bc[si], zz, 4);
	//^075F:1C70
	return;
}

//^2C1D:028C
i16 SkWinCore::_2c1d_028c(i16 xx, i16 yy, X16 ww)
{
	//^2C1D:028C
	ENTER(4);
	//^2C1D:0292
	if (glbChampionsCount != 0) {
		//^2C1D:0299
		if (ABS16(xx - glbSomePosX_4c2e) + ABS16(yy - glbSomePosY_4c30) <= 1) {
			//^2C1D:02BE
			U8 bp04[4];
			_075f_1bc2(bp04, glbSomePosX_4c2e, glbSomePosY_4c30, xx, yy, ww);
			//^2C1D:02DC
			for (U16 si = 0; si < 4; si++) {
				//^2C1D:02E0
				i16 di = GET_PLAYER_AT_POSITION(bp04[si]);
				if (di < 0)
					continue;
				return di;
				//^2C1D:02F3
			}
		}
	}
	//^2C1D:02F9
	return -1;
}

//^1C9A:0551
//SPX: This computes the highest power 4 within val ?
//_1c9a_0551 renamed COMPUTE_POWER_4_WITHIN
U32 SkWinCore::COMPUTE_POWER_4_WITHIN(U32 val, U16 cnt)
{
	//^1C9A:0551
	ENTER(4);
	//^1C9A:0556
	X32 bp04 = 1;
	for (i16 si = 0; si < 32; si++, bp04 <<= 1) {
		//^1C9A:0564
		if ((val & bp04) != 0 && --cnt == 0) {
			break;
		}
	}
	//^1C9A:058F
	return bp04;
}

//^1C9A:0598
U16 SkWinCore::_1c9a_0598(U32 val)
{
	//^1C9A:0598
	ENTER(0);
	//^1C9A:059D
	i16 di = 0;
	U16 si = 0;
	for (; val != 0 && di < 32; di++, val >>= 1) {
		//^1C9A:05A3
        if ((U16(val) & 1) != 0)
			si++;
	}
	//^1C9A:05CD
	return si;
}

//^19F0:0891
U16 SkWinCore::_19f0_0891(i16 zz, X16 xx, X16 yy, i16 ss, X16 tt, X16 ww)
{
	//^19F0:0891
	ENTER(18);
	//^19F0:0897
	i16 di = ss;
	X16 bp02 = zz & 0x80;
	if (bp02 != 0)
		zz &= 0xff7f;	// build an object from spell zz
	//^19F0:08AC
	U16 si = glbAIAttacksSpells;
	if (si != 0) {
		//^19F0:08B8
		if (di == -1) {
			//^19F0:08BD
			di = xx + glbXAxisDelta[ww];
			tt = yy + glbYAxisDelta[ww];
		}
		//^19F0:08DA
		if (di >= 0 && di < glbCurrentMapWidth && tt >= 0 && tt < glbCurrentMapHeight && (xx -di == 0 || yy -tt == 0)) {
			//^19F0:0911
			i16 bp04 = CALC_SQUARE_DISTANCE(xx, yy, di, tt);
			if (bp04 <= 1 || (si &= 0x0ff8) != 0) {
				//^19F0:0938
				if (bp04 != 0 || (si &= 7) != 0) {
					//^19F0:094B
					if (glbAIDef->w20_c_f() >= U16(bp04)) {
						//^19F0:095E
						if (bp04 == 0) {
							i16 bp0e;
							for (bp0e = 0; bp0e < 4; bp0e++) {
								//^19F0:096B
								X16 bp0a = xx;
								X16 bp0c = yy;
								bp0a += glbXAxisDelta[bp0e];
								bp0c += glbYAxisDelta[bp0e];
								if (CREATURE_GO_THERE(0, xx, yy, bp0a, bp0c, bp0e) != 0)
									break;
								//^19F0:09AC
							}
							//^19F0:09B5
							if (bp0e < 4)
								goto _0ccf;
						}
						//^19F0:09BE
						switch (zz) {
							case 2:
								//^19F0:09D1
								_19f0_045a(di, tt);
								if (_19f0_050f() == OBJECT_END_MARKER)
									goto _0ccf;
								break;

							case 0:
							case 1:
								//^19F0:09E7
								if (glbCurrentMapIndex != glbCreatureMap || di != glbCreaturePosX || tt != glbCreaturePosY)
									goto _0ccf;
								break;

							default:
								break;
						}
						//^19F0:0A08
						if (bp04 <= 1 || _19f0_0207(xx, yy, di, tt, &SkWinCore::_19f0_00b8) != 0) {
							//^19F0:0A2C
							if ((tblAIStats01[_4976_4efa] & 0x200) != 0) {
								//^19F0:0A3A
								_19f0_045a(di, tt);
								//^19F0:0A48
								for (ObjectID bp10 = _19f0_04bf(); bp10 != OBJECT_END_MARKER; bp10 = GET_NEXT_RECORD_LINK(bp10)) {
									//^19F0:0A4A
									if (bp10.DBType() == dbCloud) {
										if (GET_ADDRESS_OF_RECORDF(bp10)->CloudType() == 14) {
											si += 7;
											if (si != 0)
												break;
											goto _0ccf;
										}
									}
									//^19F0:0A7E
								}

							}
							//^19F0:0A90
							if (bp02 == 0)
								//^19F0:0A96
								//^19F0:0CCA
								return 1;
							//^19F0:0A99
							if (ww == 0xffff) {
								//^19F0:0A9F
								if (bp04 == 0 && glbCurrentMapIndex == glbCreatureMap && di == glbCreaturePosX && tt == glbCreaturePosY) {
									//^19F0:0ABD
									ww = (glbPlayerDir +2) & 3;
								}
								else {
									//^19F0:0AC7
									ww = CALC_VECTOR_DIR(xx, yy, di, tt);
								}
							}
							//^19F0:0ADC
							X16 bp08;
							X16 bp06;
							if (_19f0_0559(ww) == 0) {
								//^19F0:0AEB
								bp08 = RAND01();
								if (bp04 <= 1 && (si & 7) != 0 && ((si & 0xff8) == 0 || RAND01() != 0)) {
									//^19F0:0B0E
									si &= 7;
									if (zz <= 1) {
										//^19F0:0B18
										bp06 = _2c1d_028c(xx, yy, 0x00ff);
										if (bp06 != 0xffff) {
											//^19F0:0B31
											bp06 = glbChampionSquad[bp06].playerPos();
											goto _0bae;
										}
									}
									//^19F0:0B40
									bp06 = (bp08 +ww +2) & 3;
									goto _0bae;
								}
								//^19F0:0B4A
								si &= 0xff8;
								if (zz <= 1 && RAND02() != 0) {
									//^19F0:0B5D
									bp08 = RAND01();
									bp06 = (bp08 != 0) ? ((ww +2) & 3) : ww;
									//^19F0:0B79
									if (GET_PLAYER_AT_POSITION(bp06) -1) {
										if (GET_PLAYER_AT_POSITION((bp06 +3) & 3) == -1) {
											//^19F0:0B9C
											bp08 = 1 -bp08;
										}
									}
								}
								//^19F0:0BA5
								bp06 = (ww +bp08) & 3;
								//^19F0:0BAE
_0bae:
								//^19F0:0BB1
								si = COMPUTE_POWER_4_WITHIN(si, RAND16(_1c9a_0598(si)) +1);
								X16 bp12 = si;
								//^19F0:0BD4
								// SPX: This part seems to work for multispell casters : Vexirks & Dragoth etc ..
								if(SkCodeParam::bUseIngameDebug)
									printf("BP12 = %04X, %d\n", bp12, bp12);
								switch (bp12) {
									case AI_ATTACK_FLAGS__PUSH_BACK:	// 0x0002 Knock back
										//^19F0:0BED
										glbCurrentThinkingCreatureData->Command = ccmPushBack;
										break;
									case AI_ATTACK_FLAGS__MELEE:	// 0x0001 Standard attack
										//^19F0:0BF9
										glbCurrentThinkingCreatureData->Command = ccmMeleeAttack;
										break;
									case AI_ATTACK_FLAGS__STEAL:	// 0x0004 Steal?
										//^19F0:0C04
										glbCurrentThinkingCreatureData->Command = ccmSteal;
										glbCurrentThinkingCreatureData->ItemToThrow = 0x0B;	// ????
										break;
									case AI_ATTACK_FLAGS__FIREBALL:	// 0x0010	- 16
										//^19F0:0C14
//											bp04 = i16(0xff80);		// fireball
										bp04 = i16(OBJECT_EFFECT_FIREBALL);
										goto _0c4a;
									case AI_ATTACK_FLAGS__DISPELL:	// 0x0020	- 32
										//^19F0:0C1B
//											bp04 = i16(0xff83);		// dispell
										bp04 = i16(OBJECT_EFFECT_DISPELL);
										goto _0c4a;
									case AI_ATTACK_FLAGS__LIGHTNING:	// 0x0040	- 64
										//^19F0:0C22
//											bp04 = i16(0xff82);		// lightning
										bp04 = i16(OBJECT_EFFECT_LIGHTNING);
										goto _0c4a;
									case AI_ATTACK_FLAGS__POISON_CLOUD:	// 0x0080	- 128
										//^19F0:0C29
//											bp04 = i16(0xff87);		// poison cloud
										bp04 = i16(OBJECT_EFFECT_POISON_CLOUD);
										goto _0c4a;
									case AI_ATTACK_FLAGS__POISON_BOLT:	// 0x0100	- 256
										//^19F0:0C30
//											bp04 = i16(0xff86);		// poison bolt
										bp04 = i16(OBJECT_EFFECT_POISON_BOLT);
										goto _0c4a;
									case AI_ATTACK_FLAGS__PUSH_SPELL:	// 0x0400	- 1024
										//^19F0:0C37
//											bp04 = i16(0xff89);		// push
										bp04 = i16(OBJECT_EFFECT_PUSH);
										goto _0c4a;
									case AI_ATTACK_FLAGS__PULL_SPELL:	// 0x0800	- 2048
										//^19F0:0C3E
//											bp04 = i16(0xff8a);		// pull
										bp04 = i16(OBJECT_EFFECT_PULL);	
										goto _0c4a;
									case AI_ATTACK_FLAGS__POISON_BLOB:	// 0x0200	- 512
//											bp04 = i16(0xff81);		// what's this??
										bp04 = i16(OBJECT_EFFECT_POISON_BLOB);		// what's this??
										//SPX: It does look like this effect doesn't work (no image)
										// I guess in actual code, we never reach this line.
										//^19F0:0C4A
_0c4a:
										glbCurrentThinkingCreatureData->ItemToThrow = U8(bp04);
										glbCurrentThinkingCreatureData->Command = (bp08 != 0) ? ccmCastSpell2 : ccmCastSpell1;
										break;
									case AI_ATTACK_FLAGS__SHOOT:	// 0x0008 Able to launch item / or maybe take item ? (axe, arrow?)
										//^19F0:0C65
										glbCurrentThinkingCreatureData->Command = (bp08 != 0) ? ccmShootItem2 : ccmShootItem1;
										break;
								}
								//^19F0:0C79
								glbCurrentThinkingCreatureData->w24.SetX(di);
								glbCurrentThinkingCreatureData->w24.SetY(tt);
								glbCurrentThinkingCreatureData->w24.SetMap(glbCurrentMapIndex);
								//^19F0:0CB0
								glbCurrentThinkingCreatureData->b27 = U8(ww);
								glbCurrentThinkingCreatureData->b28 = U8(bp06);
								glbCurrentThinkingCreatureData->b32 = U8(zz);
								_4976_4ee5 = xactrAgain;
							}
							//^19F0:0CCA
							return 1;
						}
					}
				}
			}
		}
	}
	//^19F0:0CCF
_0ccf:
	if (bp02 != 0)
		_4976_4ee5 = xactrNo;
	return 0;
}

//^19F0:0547
X16 SkWinCore::_19f0_0547(ObjectID rl, X16 xx)
{
	//^19F0:0547
	ENTER(0);
	//^19F0:054A
	return CREATURE_CAN_HANDLE_IT(rl, xx);
}

//^19F0:01D6
U16 SkWinCore::_19f0_01d6(i16 xx, i16 yy)
{
	//^19F0:01D6
	ENTER(0);
	//^19F0:01D9
	if (glbCurrentMapIndex == glbCreatureMap && xx == glbCreaturePosX && yy == glbCreaturePosY)
		//^19F0:01F4
		return 1;
	//^19F0:01F9
	return _19f0_00b8(xx, yy);
}

//^19F0:05E8
i16 SkWinCore::_19f0_05e8(X16 aa, DistMapTile (*bb)[1][32], Ax3 *cc, i16 xx, i16 yy, i16 zz, X16 ww)
{
	//^19F0:05E8
	ENTER(16);
	//^19F0:05EE
	i16 si = zz;
	i16 bp06 = ((glbAIAttacksSpells & 0xff8) == 0) ? (glbAIDef->w20 >> 12) : 1;
	i16 bp08 = 0;
	if (si == -1)
		si = 0, bp08 = 3;
	//^19F0:061D
	i16 bp04;
	i16 bp02;
	i16 di;
	do {
		bp02 = xx;
		di = yy;
		for (bp04 = 0; bp04 < bp06; bp04++) {
			//^19F0:062E
			bp02 += glbXAxisDelta[si];
			di += glbYAxisDelta[si];
			//^19F0:0641
			if (bp02 < 0 || bp02 >= glbCurrentMapWidth || di < 0 || di >= glbCurrentMapHeight)
				//^19F0:0663
				continue;
			//^19F0:0666
			U8 bp0d = GET_TILE_VALUE(bp02, di);
			ObjectID bp10;
			if ((bp0d & 0x10) != 0) {
				//^19F0:067B
				if ((bp0d >> 5) == ttWall) {
					//^19F0:0687
					if (ww == 0)
						break;
					//^19F0:0690
					for (bp10 = GET_TILE_RECORD_LINK(bp02, di); bp10 != OBJECT_END_MARKER; bp10 = GET_NEXT_RECORD_LINK(bp10)) {
						//^19F0:069D
						if (bp10.DBType() == dbActuator || bp10.DBType() == dbText) {
							//^19F0:06B9
							if (bp10.Dir() == ((si +2) & 3)) {
								//^19F0:06CB
								U8 bp0e = QUERY_CLS2_FROM_RECORD(bp10);
								if (bp0e != 0xff) {
									//^19F0:06DB
									if (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_WALL_GFX, bp0e, dtWordValue, GDAT_WALL_ORNATE__DATA_F0) == aa)
										goto _0783;
								}
							}
						}
						//^19F0:06F3
					}
					//^19F0:0705
					continue;
				}
				//^19F0:0707
				if (ww == 0 && (glbCreatureTimer.XcoordB() != bp02 || glbCreatureTimer.YcoordB() != di) && (bp10 = GET_CREATURE_AT(bp02, di)) != OBJECT_NULL) {
					//^19F0:0733
					if (_19f0_0547(bp10, aa) != 0)
						goto _0783;
				}
			}
			//^19F0:0743
			if (bb[bp02][di]->distance != 0 || _19f0_01d6(bp02, di) != 0)
				break;
			//^19F0:0769
		}
		//^19F0:0777
		si++;
	} while (--bp08 >= 0);
	//^19F0:0780
	goto _088b;
	//^19F0:0783
_0783:
	bp08 = bb[xx][yy]->distance;
	si += 2;
	si &= 3;
	while (++bp04 < bp06) {
		//^19F0:07A6
		U8 bp0a = U8(xx);
		U8 bp0c = U8(yy);
		xx += glbXAxisDelta[si];
		yy += glbYAxisDelta[si];
		U8 bp0d = bb[xx][yy]->distance;
		//^19F0:07E1
		if (bp0d != 0 && bp0d <= bp08) {
			//^19F0:07F1
			if (_19f0_01d6(xx, yy) == 0)
				continue;
		}
		//^19F0:0801
		xx = bp0a;
		yy = bp0c;
		break;
		//^19F0:080F
	}
	//^19F0:081A
	si += 2;
	si &= 3;
	if (_19f0_0891((ww != 0) ? 3 : 2, xx, yy, bp02, di, si) != 0) {
		//^19F0:0848
		_19f0_045a(xx, yy);
		cc->SetX(bp02);
		cc->SetY(di);
		cc->SetMap(glbCurrentMapIndex);
		//^19F0:0886
		return 1;
	}
	//^19F0:088B
_088b:
	return 0;
}

//^1C9A:1B16
i16 SkWinCore::_1c9a_1b16(X16 xx, X16 yy)
{
	//^1C9A:1B16
	ENTER(6);
	//^1C9A:1B1C
	ObjectID si = _4976_521e;
	if (si == OBJECT_NULL) {
		//^1C9A:1B26
		_4976_521e = GET_TILE_RECORD_LINK(glbCreatureSomeX, glbCreatureSomeY);
		si = _4976_521e;
	}
	X16 bp06;
	for (; si != OBJECT_END_MARKER && (bp06 = si.DBType()) <= dbActuator; si = GET_NEXT_RECORD_LINK(si)) {
		//^1C9A:1B3A
		if (bp06 == dbText) {
			//^1C9A:1B40
			Text *bp04 = GET_ADDRESS_OF_RECORD2(si);
			//^1C9A:1B4D
			if (bp04->TextMode() == 2 && bp04->w2_c_f() == xx && bp04->w2_8_b() == yy) {
				//^1C9A:1B82
				return bp04->w2_3_7();  // TODO: Complex text use
			}
		}
		//^1C9A:1B89
	}
	//^1C9A:1BA7
	return -1;
}

//^1C9A:1A48
i16 SkWinCore::_1c9a_1a48(X16 xx, X16 yy)
{
	//^1C9A:1A48
	ENTER(8);
	//^1C9A:1A4E
	X16 bp08 = (xx == 1) ? 2 : -1;
	//^1C9A:1A60
	ObjectID si = _4976_521e;
	if (si == OBJECT_NULL) {
		//^1C9A:1A6D
		_4976_521e = GET_TILE_RECORD_LINK(glbCreatureSomeX, glbCreatureSomeY);
		si = _4976_521e;
	}
	//^1C9A:1A7F
	X16 bp06;
	for (; si != OBJECT_END_MARKER && (bp06 = si.DBType()) <= dbActuator; si = GET_NEXT_RECORD_LINK(si)) {
		//^1C9A:1A81
		if (bp06 == dbText) {
			Text *bp04 = GET_ADDRESS_OF_RECORD2(si);
			if (bp04->TextMode() == 2) {
				if (true
					&& ((bp04->w2_c_f() == xx) || (bp04->w2_c_f() == bp08 && bp04->TextVisibility() != 0))
					&& (bp08 & (1 << bp04->w2_8_b())) != 0
				) {
					//^1C9A:1AE7
					return bp04->w2_3_7();
				}
			}
		}
		//^1C9A:1AEE
	}
	//^1C9A:1B0F
	return -1;
}

//^19F0:0081
// SPX: _19f0_0081 renamed IS_TILE_WALL
X16 SkWinCore::IS_TILE_WALL(i16 xx, i16 yy)
{
	//^19F0:0081
	ENTER(0);
	//^19F0:0086
	U16 iTileValue = GET_TILE_VALUE(xx, yy);	// si
	U16 iTileType = iTileValue >> 5;					// di
	return (iTileType == ttWall || iTileType == ttTrickWall || (iTileValue & 5) == 0) ? 1 : 0;
}

//^19F0:1511
X16 SkWinCore::_19f0_1511(ObjectID rl)
{
	//^19F0:1511
	ENTER(0);
	//^19F0:1514
	return CREATURE_CAN_HANDLE_IT(rl, 9);
}

//^075F:1791
X16 SkWinCore::CALC_CLOUD_DAMAGE(ObjectID rlCloud, ObjectID rlTarget)
{
	//^075F:1791
	ENTER(12);
	//^075F:1797
	Cloud *bp04 = GET_ADDRESS_OF_RECORDF(rlCloud);
	i16 si = 0;
	U16 bp0a = bp04->CloudType();
	//^075F:17B5
	if (bp0a >= 8)
		return si;
	//^075F:17BD
	U8 bp0b = _4976_00b4[RCJ(8, bp0a)];
	if (bp0b == 0)
		return si;
	//^075F:17D0
	GenericRecord *bp08;
	U16 di = 0; // defaulting to 0
	if (rlTarget == OBJECT_NULL) {
		if ((bp0b & 4) == 0)
			return si;
	}
	else {
		//^075F:17E4
		di = rlTarget.DBType();
		if (di == dbDoor && (bp0b & 2) != 0) {
			//^075F:17F9
			bp08 = GET_ADDRESS_OF_RECORD(rlTarget);
		}
		else {
			//^075F:180A
			if (di != dbCreature)
				return si;
			//^075F:1812
			if ((bp0b & 8) == 0)
				return si;
		}
	}
	//^075F:1820
	if (di == dbCreature && (QUERY_CREATURE_AI_SPEC_FROM_RECORD(rlTarget)->w24 & 0x1000) != 0 && bp0a != 0)
		return si;
	//^075F:1843
	si = bp04->b3_0_f();
	if ((bp0b & 1) != 0) {
		//^075F:1855
		si = (bp04->b3_0_f() >> 1) +1;
		si = RAND16(si) +si +1;
	}
	//^075F:186D
	switch (bp0a) {
		case missileLightning: // 2
			//^075F:187C
			si >>= 1;
			goto _187e;
		case missileFireball: // 0
			//^075F:187E
_187e:
			if (di == dbDoor && bp08->castToDoor()->DestroyablebyFireball() == 0)
				si = 0;
			break;
		case missileDispell: // 3
			//^075F:1897
			if ((QUERY_CREATURE_AI_SPEC_FLAGS(rlTarget) & 0x20) == 0)
				si = 0;
			break;
		case missilePoisonCloud: // 7
			//^075F:18A9
			si = max_value(1, min_value(bp04->b3_5_f(), 4) + RAND01());
			if (di == 4) {
				si = APPLY_CREATURE_POISON_RESISTANCE(rlTarget, si);
			}
		case missilePoisonBlob: // 1=
		case missileZoSpell:	// 4: // 4=Zo Spell
		case missileFuse:		// 5=
		case missilePoisonBolt: // 6: // 6=Poison bolt
			break;
	}
	//^075F:18E7
	return si;
}

//^19F0:13AA
X16 SkWinCore::_19f0_13aa(i16 xx, i16 yy)
{
	//^19F0:13AA
	ENTER(10);
	//^19F0:13B0
	i16 bp08 = 0;
	do {
		//^19F0:13B5
		if ((tblAIStats01[_4976_4efa] & 0x400) != 0 || ((glbCurrentThinkingCreatureRec->w10 & 0x80) == 0 
			&& (glbAIDef->w0_2_2() != 0 || glbCreatureTimer.XcoordB() != xx || glbCreatureTimer.YcoordB() != yy || ((glbCurrentThinkingCreatureRec->b15_0_1() +2) & 3) != bp08) || (RAND() & 7) == 0)) {
			//^19F0:1410
			i16 di = xx;
			i16 si = yy;
			for (i16 bp0a = 0; bp0a++ < 3; ) {
				//^19F0:141E
				di += glbXAxisDelta[bp08];
				si += glbYAxisDelta[bp08];
				//^19F0:1430
				if (di < 0 || di >= glbCurrentMapWidth || si < 0 || si >= glbCurrentMapHeight)
					break;
				//^19F0:1450
				for (ObjectID bp06 = GET_WALL_TILE_ANYITEM_RECORD(di, si); bp06 != OBJECT_END_MARKER; bp06 = GET_NEXT_RECORD_LINK(bp06)) {
					//^19F0:145B
					if (bp06.DBType() == dbMissile) {
						Missile *bp04 = GET_ADDRESS_OF_RECORDE(bp06);
						if (glbTimersTable[bp04->TimerIndex()].Direction() == ((bp08 +2) & 3) && _075f_06bd(bp04, bp04->GetMissileObject()) != 0) {
							//^19F0:14BA
							return 1;
						}
					}
					//^19F0:14BF
				}
				//^19F0:14D1
				if (glbCreatureTimer.XcoordB() != di || glbCreatureTimer.YcoordB() != si) {
					//^19F0:14E3
					if (_19f0_00b8(di, si) != 0)
						break;
				}
				//^19F0:14EF
			}
		}
		//^19F0:14FD
	} while (++bp08 < 4);
	//^19F0:150B
	return 0;
}

//^19F0:050F
ObjectID SkWinCore::_19f0_050f()
{
	//^19F0:050F
	ENTER(0);
	//^19F0:0513
	ObjectID si = _4976_5222;
	if (si == OBJECT_NULL) {
		//^19F0:051D
		for (si = _19f0_04bf(); si != OBJECT_END_MARKER && si.DBType() != dbCreature; si = GET_NEXT_RECORD_LINK(si));
        //^19F0:053E
		_4976_5222 = si;
	}
	//^19F0:0542
	return si;
}


//^19F0:266C
ObjectID SkWinCore::_19f0_266c(ObjectID rl, U16 dir, U16 ww, U16 bb)
{
	//^19F0:266C
	ENTER(8);
	//^19F0:2672
	ObjectID si = rl;
	ObjectID di = OBJECT_NULL;
	//^19F0:2678
	for (; si != OBJECT_END_MARKER && si != OBJECT_NULL; si = GET_NEXT_RECORD_LINK(si)) {
		//^19F0:267B
		U16 bp08 = si.DBType();
		if (bp08 == dbActuator) {
			//^19F0:268B
			if (si.Dir() != dir)
				continue;
			Actuator *bp04 = GET_ADDRESS_OF_RECORD(si)->castToActuator();
			U16 bp06 = bp04->ActuatorType();
			if (bp06 == 0 || bp06 == ACTUATOR_TYPE_SWITCH_SIGN_FOR_CREATURE)	// 0x26
				continue;
			//^19F0:26BA
			di = si;
			//^19F0:26BC
			if (bp06 != ACTUATOR_TYPE_KEY_HOLE) // 0x1a: 'Activator, key hole'
				continue;
			//^19F0:26C2
			if (bp04->OnceOnlyActuator() != 0) {
				//^19F0:26D3
				if (ww != 2)
					continue;
				if (CREATURE_CAN_HANDLE_IT(si, bb) == 0)
					continue;
				break;
			}
			else {
				//^19F0:26EA
				if (ww != 1)
					continue;
				if (CREATURE_CAN_HANDLE_IT(si, bb) == 0)
					continue;
				break;
			}
		}
		//^19F0:2701
		if (bp08 > dbActuator)
			break;
		//^19F0:2707
	}
	//^19F0:271D
	return di;
}
//^19F0:2723
X16 SkWinCore::_19f0_2723(ObjectID rl, U16 ww, U16 bb, X16 cc)
{
	//^19F0:2723
	ENTER(6);
	//^19F0:2729
	X16 si = ww;
	X16 di = 0;
	if (rl != OBJECT_END_MARKER && rl != OBJECT_NULL) {
		//^19F0:2740
		ATLASSERT(rl.DBType() == dbActuator);
		Actuator *bp04 = GET_ADDRESS_OF_RECORD(rl)->castToActuator();
		U16 bp06 = bp04->ActuatorType();
		switch (bp06) {
			case ACTUATOR_FLOOR_TYPE__EVERYTHING://^2775 // 0x01: -
			case ACTUATOR_TYPE_WALL_SWITCH://^2775 // 0x18: Activator, push button wall switch
				//^19F0:2775
				if (si == 0)
					//^19F0:2779
					di = 1;
				break;
			case ACTUATOR_TYPE_X02://^277E // 0x02: -
				//^19F0:277E
				if (si == 0 && bb != 0xffff)
					//^19F0:2779
					di = 1;
				break;
			case ACTUATOR_FLOOR_TYPE__PARTY://^278A // 0x03: Activator, trap floor
				//^19F0:278A
				if (si == 0)
					goto _27e2;
				break;
			case ACTUATOR_TYPE_2_STATE_WALL_SWITCH://^2790 // 0x17: Activator, 2 state wall switch
				//^19F0:2790
				if (si == 0) {
					if ((!bp04->OnceOnlyActuator()) != cc)
						//^19F0:2779
						di = 1;
				}
				break;
			case ACTUATOR_TYPE_X1B://^27AD // 0x1B: -
				//^19F0:27AD
				if (si == 1) {
					if (bp04->ActuatorData() != 0)
						//^19F0:27C3
						goto _27e2;
				}
				break;
			case ACTUATOR_TYPE_KEY_HOLE://^27C5 // 0x1A: Activator, key hole
				//^19F0:27C5
				if ((bp04->OnceOnlyActuator() != 0) ? (si == 2) : (si == 1)) {
					//^19F0:27E2
_27e2:
					di = CREATURE_CAN_HANDLE_IT(rl, bb);
				}
				break;
		}
	}
	//^19F0:27F1
	return di;
}

//^19F0:2813
U16 SkWinCore::_19f0_2813(U16 ww, i16 xx, i16 yy, i16 ss, i16 tt, i16 aa, U16 bb)
{
	//^19F0:2813
	ENTER(10);
	//^19F0:2819
	i16 di = ss;
	X16 bp06 = ww & 0x80;
	if (bp06 != 0)
		ww &= ~(0x80);
	//^19F0:282E
	if ((_4976_4ef4 & 1) != 0) {
		//^19F0:2839
		if (di == -1) {
			di = xx + glbXAxisDelta[aa];
			tt = yy + glbYAxisDelta[aa];
			goto _286d;
		}
		//^19F0:285D
		else if (xx != di || yy != tt) {
			//^19F0:286D
_286d:
			if (di >= 0 && di < glbCurrentMapWidth && tt >= 0 && tt < glbCurrentMapHeight) {
				//^19F0:2892
				if ((xx -di) == 0 && (yy -tt) == 0) {
					//^19F0:28A4
					if (aa == -1) {
						aa = CALC_VECTOR_DIR(xx, yy, di, tt);
					}
					//^19F0:28BF
					_19f0_045a(di, tt);
					if ((_4976_521c & 0x10) != 0) {
						//^19F0:28D7
						if (_4976_521e == OBJECT_NULL) {
							_4976_521e = GET_TILE_RECORD_LINK(glbCreatureSomeX, glbCreatureSomeY);
						}
						//^19F0:28F0
						ObjectID si = _4976_521e;
						X16 bp0a = 0;
						//^19F0:28F9
						Actuator *bp04;
						for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
							//^19F0:28FB
							X16 bp08 = si.DBType();
							if (bp08 > dbActuator)
								break;
							if (bp08 != dbActuator)
								continue;
							//^19F0:2911
							if (si.Dir() != ((aa +2)&3))
								continue;
							bp04 = GET_ADDRESS_OF_RECORD(si)->castToActuator();
							bp0a = (bp04->ActuatorType() == ACTUATOR_TYPE_SWITCH_SIGN_FOR_CREATURE) // 0x26 -> 'Switch sign for creature' or 'Activator, missile explosion'
								? 1
								: 0;
							if (bp0a != 0)
								break;
							//^19F0:294D
						}
						//^19F0:295B
						if (bp0a != 0) {
							if (bp04->ActuatorData() == glbCurrentThinkingCreatureRec->CreatureType()) {
								if (bp04->OnceOnlyActuator() != 0) {
									si = _19f0_266c(_4976_521e, (aa+2)&3, ww, bb);
									if (si != OBJECT_NULL) {
										//^19F0:29BA
										if (_19f0_2723(si, ww, bb, bp04->ActionType()) != 0) {
											//^19F0:29DD
											if (bp06 != 0 && _19f0_0559(aa) == 0) {
												//^19F0:29F5
												glbCurrentThinkingCreatureData->w24.SetX(di);
												glbCurrentThinkingCreatureData->w24.SetY(tt);
												glbCurrentThinkingCreatureData->w24.SetMap(glbCurrentMapIndex);
												glbCurrentThinkingCreatureData->b29 = 
												glbCurrentThinkingCreatureData->b27 = (X8)aa;
												glbCurrentThinkingCreatureData->ItemToThrow = (X8)bb;
												glbCurrentThinkingCreatureData->b32 = (X8)ww;
												//^19F0:2A45
												if (ww == 0) {
													glbCurrentThinkingCreatureData->Command = ccm2F;
												}
												//^19F0:2A52
												else if (ww == 1) {
													glbCurrentThinkingCreatureData->Command = ccm30;
												}
												else {
													glbCurrentThinkingCreatureData->Command = ccm31;
												}
												//^19F0:2A6C
												_4976_4ee5 = (ww == 0 && bp04->ActionType() < 2)
													? xactrAgain
													: xactrYes;
											}
											//^19F0:2A90
											return 1;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	//^19F0:2A95
	if (bp06 != 0)
		_4976_4ee5 = xactrNo;
	//^19F0:2AA0
	return 0;
}

//^19F0:0D10
U16 SkWinCore::_19f0_0d10(U16 ww,i16 xx,i16 yy,i16 ss,i16 tt,i16 aa)
{
	//^19F0:0D10
	ENTER(32);
	//^19F0:0D16
	ObjectID bp18;
	i16 di = ss;
	X16 bp06 = 0;
	U16 bp0e = ww & 0x80;
	if (bp0e != 0)
		ww &= 0xff7f;
	X16 si = ((ww == 0) ? 0x6f : 0x73) & _4976_4ef0;
	//^19F0:0D46
	if (si == 0)
		goto _123a;
    //^19F0:0D4F
	if (di == -1) {
		//^19F0:0D54
		di = xx + glbXAxisDelta[aa];
		tt = yy + glbYAxisDelta[aa];
	}
	//^19F0:0D73
	else if (aa == -1) {
		//^19F0:0D79
		aa = CALC_VECTOR_DIR(xx, yy, di, tt);
	}
	//^19F0:0D90
	else if (xx == di && yy == tt) {
		goto _123a;
	}
	//^19F0:0DA0
	if (di < 0 || di >= glbCurrentMapWidth || tt < 0 || tt >= glbCurrentMapHeight)
		goto _123a;
	//^19F0:0DC5
	if ((xx -di) != 0 && (yy -tt) != 0)
		goto _123a;
	//^19F0:0DD7
	X16 bp0a;
	_19f0_045a(di, tt);
	if ((_4976_521c >> 5) != 4)
		goto _123a;
	bp0a = (_4976_521c & 7);
	//^19F0:0DF8
	Door *door;	//*bp04
	if (ww == 0) {
		if (bp0a == 0) {
_0e04:
			bp06 = 1;
			goto _108f;
		}
	}
	//^19F0:0E0C
	else if (bp0a == 5) {
		goto _123a;
	}
	//^19F0:0E15
	else if (bp0a == 4) {
		goto _0e04;
	}
	//^19F0:0E1D
	if (_4976_521e == OBJECT_NULL) {
		//^19F0:0E24
		_4976_521e = GET_TILE_RECORD_LINK(di, tt);
	}
	//^19F0:0E32
	door = GET_ADDRESS_OF_RECORD0(_4976_521e);
	if (ww == 2 && door->Bit12() == 0)
		goto _123a;
	//^19F0:0E5C
	i16 bp10;
	i16 bp12;
	i16 bp14;
	U16 bp16;
	i16 bp08;
	if (door->Bit10() != 0) {
		//^19F0:0E6D
		if (door->Bit09() != 0) {
			if (ww == 0)
				goto _104f;
		}
		//^19F0:0E83
		else if (ww != 0) {
			goto _104f;
		}
		//^19F0:0E8C
		else if ((si &= 0xfff3) == 0) {
			goto _123a;
		}
	}
	//^19F0:0E99
	bp08 = CALC_SQUARE_DISTANCE(xx, yy, di, tt);
	if (bp08 == 1) {
		//^19F0:0EB3
		if ((si & 3) != 0) {
			//^19F0:0EB9
			if (door->Button() != 0)
				goto _0f9d;
			//^19F0:0ECD
			if (door->Bit13() != 0)
				goto _0f9d;
		}
		//^19F0:0EDE
		if (ww == 0) {
			//^19F0:0EE4
			if ((si & 4) != 0 && door->DestroyablebyFireball() != 0)
				goto _0f9d;
			//^19F0:0EFE
			if ((si & 8) != 0 && door->BashablebyChopping() != 0)
				goto _0f9d;
		}
		//^19F0:0F15
		if ((si & 0x40) != 0)
			goto _0f9d;
		goto _123a;
	}
	//^19F0:0F21
	if (glbAIDef->w20_c_f() < bp08)
		goto _123a;
	//^19F0:0F34
	if ((si & 1) != 0) {
		if (door->Button() != 0 || door->Bit13() != 0)
			goto _0f7f;
	}
	//^19F0:0F59
	if (ww != 0 || (si & 4) == 0)
		goto _123a;
	//^19F0:0F6B
	if (door->DestroyablebyFireball() == 0)
		goto _123a;
	//^19F0:0F7F
_0f7f:
	if (_19f0_0207(xx, yy, di, tt, &SkWinCore::_19f0_00b8) == 0)
		goto _123a;
	//^19F0:0F9D
_0f9d:
	if ((si & 1) != 0) {
		//^19F0:0FA6
		bp10 = xx;
		bp12 = yy;
		bp14 = bp08;
		for (; bp14 >= 0; ) {
			//^19F0:0FBB
			for (bp18 = GET_WALL_TILE_ANYITEM_RECORD(bp10, bp12); bp18 != OBJECT_END_MARKER; bp18 = GET_NEXT_RECORD_LINK(bp18)) {
				//^19F0:0FCB
				Cloud *bp20;
				bp16 = bp18.DBType();
				if (bp16 == dbMissile) {
					//^19F0:0FDC
					Missile *bp1c = GET_ADDRESS_OF_RECORDE(bp18);
					//^19F0:0FEB
					if (bp1c->GetMissileObject() != OBJECT_EFFECT_ZO_2 && bp1c->GetMissileObject() != OBJECT_EFFECT_ZO_SPELL) // != oFF8D && != oFF84
						continue;
					//^19F0:0FFC
					if (glbTimersTable[bp1c->TimerIndex()].Direction() != aa)
						continue;
					goto _104f;
				}
				//^19F0:101F
				else if (bp16 == dbCloud) {
					//^19F0:1025
					bp20 = GET_ADDRESS_OF_RECORDF(bp18);
					if (bp20->CloudType() != 13 && bp20->CloudType() != 4)
						continue;
					//^19F0:104F
_104f:
					bp06 = 2;
					goto _108f;
				}
				//^19F0:1056
			}
			//^19F0:106B
			bp10 += glbXAxisDelta[aa];
			bp12 += glbYAxisDelta[aa];
			bp14--;
			//^19F0:1086
		}
	}
	//^19F0:108F
_108f:
	if (bp06 != 0 && ww == 2)
		goto _123a;
	if (bp0e == 0)
		goto _1235;
	//^19F0:10A7
	if (aa == -1) {
		aa = CALC_VECTOR_DIR(xx, yy, di, tt);
	}
	//^19F0:10C2
	if (bp06 == 1) {
		//^19F0:10C8
		_4976_4ee5 = xactrYes;
		goto _1235;
	}
	//^19F0:10D0
	if (bp06 == 2) {
		//^19F0:10D6
		glbCurrentThinkingCreatureData->Command = ccmNeutral;
		goto _1230;
	}
	//^19F0:10E2
	if (_19f0_0559(aa) != 0)
		goto _1235;
	//^19F0:10F1
	if ((si & 0x20) != 0) {
		si &= 0xffdf;
		if (ww == 0) {
			//^19F0:1101
			door->Bit12(1);
		}
	}
	//^19F0:1109
	if (bp08 > 1)
		si &= 5;
	//^19F0:1113
	if ((si & 0x42) != 0) {
		if ((si & 0xffbd) == 0 || RAND02() == 0) {
			//^19F0:1128
			glbCurrentThinkingCreatureData->Command = ccm0B;
			goto _11d2;
		}
	}
	//^19F0:1134
	U16 bp0c;
	if ((si & 1) != 0) {
		//^19F0:113A
		if ((si & 0xffbc) == 0 || RAND02() == 0) {
			//^19F0:1149
			glbCurrentThinkingCreatureData->Command = (RAND01() != 0) ? ccmCastSpell2 : ccmCastSpell1;
			//^19F0:1160
			// SPX: 8D (unknow zo2?) replaced with 84 (open door zo1)
			glbCurrentThinkingCreatureData->ItemToThrow = (ww == 0) ? 0x8d : 0x84;
			goto _11d2;
		}
	}
	//^19F0:1176
	bp0c = glbAIAttacksSpells;
	if ((si & 8) != 0 && (bp0c & 1) != 0 && ((si & 0xffb4) == 0 || RAND01() != 0)) {
		//^19F0:1198
		glbAIAttacksSpells &= 1;
		goto _11af;
	}
	//^19F0:11A0
	if ((si & 4) == 0)
		goto _123a;
	//^19F0:11A9
	glbAIAttacksSpells &= 0x50;
	//^19F0:11AF
_11af:
	si = _19f0_0891(0x84, xx, yy, di, tt, aa);
	glbAIAttacksSpells = bp0c;
	//^19F0:11CE
	return si;
_11d2:
	glbCurrentThinkingCreatureData->w24.SetX(di);
	glbCurrentThinkingCreatureData->w24.SetY(tt);
	glbCurrentThinkingCreatureData->w24.SetMap(glbCurrentMapIndex);
    glbCurrentThinkingCreatureData->b29 = glbCurrentThinkingCreatureData->b27 = U8(aa);
	glbCurrentThinkingCreatureData->b28 = (U8(RAND01()) + U8(aa)) & 3;
	glbCurrentThinkingCreatureData->b32 = U8(ww);
	//^19F0:1230
_1230:
	_4976_4ee5 = xactrAgain;
	//^19F0:1235
_1235:
	return 1;
_123a:
	if (bp0e != 0)
		_4976_4ee5 = xactrNo;
	return 0;
}

//^0CEE:3275
X16 SkWinCore::_0cee_3275(U8 xx)
{
	//^0CEE:3275
	ENTER(0);
	//^0CEE:3278
	return QUERY_GDAT_ENTRY_DATA_INDEX(0x0e, xx, dtWordValue, 0x0d);
}

//^19F0:000A
U16 SkWinCore::_19f0_000a(i16 xx, i16 yy)
{
	//^19F0:000A
	ENTER(6);
	//^19F0:000F
	U16 bp06 = GET_TILE_VALUE(xx, yy);
	U16 si = bp06 >> 5;
	if (si == ttDoor) {
		Door *bp04 = GET_ADDRESS_OF_TILE_RECORD(U8(xx), U8(yy))->castToDoor();
		if ((bp06 & 7) == 4 && _0cee_3275(GET_GRAPHICS_FOR_DOOR(bp04)) == 0)
			return 1;
		//^19F0:0066
		return 0;
	}
	//^19F0:006A
	if (si != ttWall) {
		if (si != ttTrickWall)
			return 0;
		if ((bp06 & 4) != 0)
			return 0;
	}
	return 1;
}


//^1C9A:1BD8
U16 SkWinCore::FIND_WALK_PATH(i16 xx, i16 yy, U16 aa, U16 bb, i16 cc, skxxx9 *ss, WalkPath *wp)
{
	// xx: x pos
	// yy: y pos
	// aa = 7
	// bb = 0
	// cc = 1
	// ss: array size is by cc
	// wp: array of size 5

#if DLV_CAI
	SkD((DLV_CAI, "CAI: (e) FIND_WALK_PATH(%2d,%2d,%04X,%04X,%2d,%p,x) \n", (Bitu)xx, (Bitu)yy, (Bitu)aa, (Bitu)bb, (Bitu)cc, ss));
	for (int t=0; t<cc; t++) {
		SkD((DLV_CAI, "       ) #%d (%2d,%2d,%2d) (%2d,%2d,%2d) (%2d,%2d,%2d) %02X %02X %02X %02X \n"
			, t
			, (Bitu)ss[t].w2.GetX(), (Bitu)ss[t].w2.GetY(), (Bitu)ss[t].w2.GetMap()
			, (Bitu)ss[t].w4.GetX(), (Bitu)ss[t].w4.GetY(), (Bitu)ss[t].w4.GetMap()
			, (Bitu)ss[t].w12.GetX(), (Bitu)ss[t].w12.GetY(), (Bitu)ss[t].w12.GetMap()
			, (Bitu)(U8)ss[t].b0
			, (Bitu)(U8)ss[t].b6
			, (Bitu)(U8)ss[t].b7
			, (Bitu)(U8)ss[t].b16
			));
	}
#endif
	//^1C9A:1BD8
	ENTER(132);
	//^1C9A:1BDE
	skxxx9 *bp0c = ss;
	//^1C9A:1BEC
	if (bp0c->b7 == 0) {
		//^1C9A:1BF5
		bp0c->w2.SetX(xx);
		//^1C9A:1C07
		bp0c->w2.SetY(yy);
		//^1C9A:1C1A
		bp0c->w2.SetMap(glbCurrentMapIndex);
		//^1C9A:1C2C
		bp0c->w4 = bp0c->w2;
		//^1C9A:1C34
		bp0c->b6 = 0;
		//^1C9A:1C39
		return 0;
	}
	//^1C9A:1C3E
	U32 bp64 = U32(cc) << 2;
	//^1C9A:1C4F
	i32 allocSizes[6] = {0,0,0,0,0,0};
	skxxxa *bp22 = reinterpret_cast<skxxxa *>(ALLOC_MEMORY_RAM(allocSizes[0] = bp64, afZeroMem, 1024));
	//^1C9A:1C69
	U8 (*bp08)[4] = (U8 (*)[4])ALLOC_MEMORY_RAM(allocSizes[1] = 1024, afDefault, 1024);
	//^1C9A:1C81
	U32 bp6c = sizeof(DistMapTile) * dunHeader->nMaps;
	//^1C9A:1C96
	DistMapTile (**bp1a)[1][32] = reinterpret_cast<DistMapTile (**)[1][32]>(ALLOC_MEMORY_RAM(allocSizes[2] = bp6c, afZeroMem, 1024));
	//^1C9A:1CB0
	U32 bp68;
	bp6c += (bp68 = dunHeader->nMaps << 1);
	//^1C9A:1CCA
	i16 *bp1e = reinterpret_cast<i16 *>(ALLOC_MEMORY_RAM(allocSizes[3] = bp68, afDefault, 1024));
	//^1C9A:1CE3
	U16 bp56 = 0;
	//^1C9A:1CE8
	FILL_U16(bp1e, dunHeader->nMaps, -1, 2);
	//^1C9A:1D05
	bp68 = sizeof(DistMapTile) * (U32(glbCurrentMapWidth) << 5);
	//^1C9A:1D1D
	DistMapTile (*bp04)[1][32] = reinterpret_cast<DistMapTile (*)[1][32]>(ALLOC_MEMORY_RAM(allocSizes[4] = bp68, afDefault, 1024));
	//^1C9A:1D36
	U16 bp48 = glbCurrentMapIndex;
	//^1C9A:1D3C
	bp1a[bp48] = bp04;
	//^1C9A:1D51
	bp6c += bp64 +bp68 +1024;
	//^1C9A:1D6A
	U16 bp52 = 0;
	U16 bp58 = 0;
	U16 si = 0;
	U16 bp34 = 0xffff;
	U8 bp38 = 0;
	U16 bp3e = MIN_EYE_DIST; // eye distance
	//^1C9A:1D84
	skxxxa *bp10 = bp22;
	//^1C9A:1D90
	bp0c = ss;
	//^1C9A:1D9C
	U16 bp0084;
	i16 bp3c;
	U16 bp7e;
	U16 bp80;
	U16 bp0082;
	X16 bp78;
	X16 bp7a;
	X16 bp7c;
	i16 di;
	for (di = 0; di < cc; bp10++, bp0c++, di++) {
		//^1C9A:1DA1
		bp0c->b6 = -1;
		//^1C9A:1DA9
		bp10->setX((i8)bp0c->w12.GetX());
		//^1C9A:1DB6
		bp10->setY((i8)bp0c->w12.GetY());
		//^1C9A:1DCA
		bp10->setMap((i8)bp0c->w12.GetMap());
		//^1C9A:1DDE
		bp38 |= (bp0c->b16 = _4976_3932[RCJ(28,bp0c->b7)]);
		//^1C9A:1DF6
		switch (bp0c->b7) {
			case  3:
				//^1C9A:1E0D
				bp7e = glbCurrentThinkingCreatureData->w12.GetX();
				bp80 = glbCurrentThinkingCreatureData->w12.GetY();
				bp0082 = glbCurrentThinkingCreatureData->w12.GetMap();
				//^1C9A:1E36
				if (true
					&& bp7e == glbCreatureMap
					&& bp80 == glbCreaturePosX
					&& bp0082 == glbCreaturePosY
				) {
					//^1C9A:1E52
					bp0c->b16 ^= 3;
				}
				//^1C9A:1E5A
_1e5a:
				bp52 = 1;
				//^1C9A:1E5F
				break;

			case 12:
				//^1C9A:1E62
				bp10->setDist((bp10->getMap() == glbCurrentMapIndex)
					? max_value(1, CALC_SQUARE_DISTANCE(glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB(), bp10->getX(), bp10->getY()))
					: 100);
				//^1C9A:1EA4
				goto _1e5a;

			case 11:
				//^1C9A:1EA6
				goto _1e5a;

			case  1:
				//^1C9A:1EA8
				bp78 = glbCurrentThinkingCreatureRec->TriggerX();
				//^1C9A:1EB6
				bp7a = glbCurrentThinkingCreatureRec->TriggerY();
				//^1C9A:1EC3
				bp7c = glbCurrentThinkingCreatureRec->TriggerMap();
				//^1C9A:1ED0
				goto _1e5a;

			case  5:
				//^1C9A:1ED2
				bp10->setDist(U8(max_value((glbCurrentMapIndex == glbCreatureMap) ? CALC_SQUARE_DISTANCE(glbCreatureTimer.XcoordB(), glbCreatureTimer.YcoordB(), glbCreaturePosX, glbCreaturePosY) : 100, bp0c->w8)));
				//^1C9A:1F12
				break;

			case  8:
			case  9:
				//^1C9A:1F14
				bp58 = 1;

				goto _1f19;

			case  6:
			case  7:
			case 10:
				//^1C9A:1F19
_1f19:
				bp0084 = glbAIAttacksSpells;
				//^1C9A:1F20
				break;

			case 23:
				//^1C9A:1F22
				_4976_4eec = 0x0227;
				glbPrecomputedLight = 0;
				glbLightModifier = 0;
				//^1C9A:1F34
				break;

			case 24:
				//^1C9A:1F36
				bp10->setDist(127);
				break;

			case  2:
			case  4:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:

				break;
		}
		//^1C9A:1F3E
		U16 bp70 = 0;
		U16 bp6e = 0;
		do {
			//^1C9A:1F46
			if ((_4976_3932[RCJ(28,bp0c->b7)] & 0x20) != 0) {
				//^1C9A:1F5A
				bp6e = (glbGlobalSpellEffects.Invisibility != 0 && (glbAIDef->w0_a_a() == 0) ? 1 : 0);
				//^1C9A:1F76
				if (bp6e == 0) {
					//^1C9A:1F7A
					bp70 = (glbLightLevel != 0 && glbGlobalSpellEffects.Light < 0 && (glbAIDef->w0_b_b() == 0) ? 1 : 0);
					//^1C9A:1F9D
					if (bp70 == 0) {
						//^1C9A:1FA1
						break;
					}
				}
				//^1C9A:1FA4
				bp3c = (glbSomeMap_4976_4ee7 == glbCreatureMap)
					? CALC_SQUARE_DISTANCE(glbCreatureTimerGetX, glbCreatureTimerGetY, glbCreaturePosX, glbCreaturePosY)
					: CALC_SQUARE_DISTANCE(
						dunMapsHeaders[glbSomeMap_4976_4ee7].MapOffsetX() +glbCreatureTimerGetX,
						dunMapsHeaders[glbSomeMap_4976_4ee7].MapOffsetY() +glbCreatureTimerGetY,
						dunMapsHeaders[glbCreatureMap].MapOffsetX() +glbCreaturePosX,
						dunMapsHeaders[glbCreatureMap].MapOffsetY() +glbCreaturePosY
					);
				//^1C9A:2040
				U16 bp76 = RAND();
				//^1C9A:2048
				if (true
					&& (bp76 & 0x8000) == 0
					&& (false
						|| bp3c == 0 
						||(bp3c == 1 && CALC_VECTOR_DIR(glbCreatureTimerGetX, glbCreatureTimerGetY, glbCreaturePosX, glbCreaturePosY) == glbCurrentThinkingCreatureRec->b15_0_1()))
				) {
					//^1C9A:2088
					break;
				}
				//^1C9A:208A
				if (false
					|| bp3c > 4
					||(bp6e != 0 && ((bp76 & 15) -(4 -bp3c) <= (glbAIDef->w20 & 15) || (bp76 & 0x4000) != 0))
					||(bp70 != 0 && ((glbLightLevel * bp3c) -((bp76 >> 8) & 7) > (glbAIDef->w20 & 15)))
				) {
					//^1C9A:20E3
					if ((bp76 & 0x0070) != 0) {
						//^1C9A:20EA
						bp0c->b0 = -1;
					}
					else {
						//^1C9A:20F3
						bp0c->b0 >>= 3;
					}
				}
			}
		} while (false);

		//^1C9A:20FA
		U16 bp2c = bp0c->b0;
		//^1C9A:2104
		if ((bp0c->b16 & 2) != 0)
			//^1C9A:210B
			bp2c++;
		//^1C9A:210E
		bp3e = max_value(bp3e, bp2c);
		//^1C9A:211E
	}
	//^1C9A:212F
	U16 bp5a = _4976_4eec & 0x2000;
	//^1C9A:2138
	X16 bp5c;
	X16 bp5e;
	if (bp5a != 0) {
		//^1C9A:213C
		bp5a = _4976_4eec & 0x0118;
		//^1C9A:2145
		if (bp5a != 0) {
			//^1C9A:2149
			bp5c = _4976_4eec & 0x0110;
			bp5e = _4976_4eec & 0x0108;
		}
	}
	//^1C9A:215B
	if (bp58 != 0 || bp3e != 0) {
		//^1C9A:2167
		ZERO_MEMORY(bp04, bp68);
	}
	//^1C9A:217B
	U16 bp2a = 0;
	U16 bp28 = 0;
	i16 bp2c = 0;
	//^1C9A:2188
	U16 bp46 = glbCurrentMapIndex;
	i16 bp12;
	i16 bp42;
	bp12 = bp42 = xx;
	i16 bp14;
	i16 bp44;
	bp14 = bp44 = yy;
	//^1C9A:21A0
	i16 bp32 = 1;
	i16 bp30 = -1;
	U8 bp37 = 0;
	U16 bp50 = 0xffff;
	//^1C9A:21B3
	_19f0_045a(bp12, bp14);
	//^1C9A:21C0
	X16 bp4c;
	X16 bp4e;
	if (bp52 != 0) {
		//^1C9A:21C9
		_4976_5225 = GET_TELEPORTER_DETAIL(&_4976_5226, U8(bp12), U8(bp14));
		//^1C9A:21DE
		if (_4976_5225 != 0) {
			//^1C9A:21E5
			bp50 = _4976_5226.b4;
			bp4c = _4976_5226.b2;
			bp4e = _4976_5226.b3;
		}
	}
	X16 bp40 = 0; // defaulting to 0
	i16 bp3a;
	ObjectID bp36;
	U16 bp16;
	DistMapTile bp74;
	// SPX: There is a BIG do-while after, and goto _25f9 jumps directly in the middle of this!...
	goto _25f9;
	//^1C9A:2200
	do {
		bp42 = bp08[bp2a][0];
		bp44 = bp08[bp2a][1];
		bp46 = bp08[bp2a][2];	// current map
		//^1C9A:223B
		bp2a = (bp2a +1) & 255;
		//^1C9A:2245
		if (bp46 != glbCurrentMapIndex) {
			//^1C9A:224E
			_1c9a_0648(bp46);
			//^1C9A:2256
			bp04 = bp1a[bp46];
		}
		//^1C9A:226E
		// SPX: whoa, this would looks like a distance map 2D array!
		bp2c = (*bp04)[bp42][bp44].distance;
		//^1C9A:2289
		if (bp2c > i16(bp3e))
			//^1C9A:228E
			break;
		//^1C9A:2291
		if (bp2c > 6)
			//^1C9A:2297
			aa |= 0x0040;
		//^1C9A:229C
		if ((aa & 0xff1f) == 5 && bp2c > 1) {
			//^1C9A:22AD
			aa = (aa & 0x00e0) | 0x0004;
		}
		//^1C9A:22B9
		bp40 = _4976_3930 & 1;
		//^1C9A:22C2
		if (bp40 == 0)
			//^1C9A:22C6
			bp40--;
		//^1C9A:22C9
		if ((_4976_3930 & 1) != 0) {
			//^1C9A:22D1
			_4976_3930 = (_4976_3930 >> 1) ^ 0xB400;
		}
		else {
			//^1C9A:22DE
			_4976_3930 >>= 1;
		}
		//^1C9A:22E2
		bp30 = _4976_3930 & 3;
		//^1C9A:22EB
		for (bp32 = (bp5a != 0) ? 7 : 5; --bp32 != 0 && ((bp28 +1) & 255) != bp2a; bp30 = (bp30 +bp40) & 3) {
			//^1C9A:2301
			if (bp46 != glbCurrentMapIndex) {
				//^1C9A:230A
				_1c9a_0648(bp46);
				//^1C9A:2312
				bp04 = bp1a[bp46];
			}
			//^1C9A:232A
			if (bp5a != 0 && bp32 <= 2) {
				//^1C9A:2336
				if (bp32 == 2) {
					//^1C9A:233C
					if (bp5e == 0)
						//^1C9A:2342
						continue;
					//^1C9A:2345
					bp30 = 5;
				}
				//^1C9A:234C
				else if (bp5c == 0) {
					//^1C9A:2352
					continue;
				}
				else {
					//^1C9A:2355
					bp30 = 4;
				}
				//^1C9A:235A
				bp12 = bp42;
				bp14 = bp44;
			}
			else {
				//^1C9A:2368
				bp12 = bp42 + glbXAxisDelta[bp30];
				bp14 = bp44 + glbYAxisDelta[bp30];
				//^1C9A:2386
				if (bp12 < 0 || bp12 >= glbCurrentMapWidth || bp14 < 0 || bp14 >= glbCurrentMapHeight) {
					//^1C9A:23AD
					continue;
				}
			}
			//^1C9A:23B0
			_4976_5224 = 0;
			//^1C9A:23B5
			bp37 = (*bp04)[bp12][bp14].distance;
			//^1C9A:23D0
			if (bp37 == 0 || bp30 >= 4) {
				//^1C9A:23DD
				//SkD((DLV_CAI, "CAI: -w- %d,%2d (%2d,%2d) -> (%2d,%2d) \n"
				//	, (Bitu)bp2c, (Bitu)bp28, (Bitu)bp42, (Bitu)bp44, (Bitu)bp12, (Bitu)bp14
				//	));
				if (CREATURE_GO_THERE(aa, bp42, bp44, bp12, bp14, bp30) == 0) {
					//^1C9A:23FB
					bp37 = 0xff;
				}
				else {
					//^1C9A:2402
					bp50 = 0xffff;
					//^1C9A:2407
					if (glbCreatureSomeZMap != glbCurrentMapIndex) {
						//^1C9A:2413
						if (bp1a[glbCreatureSomeZMap] == NULL) {
							//^1C9A:2427
							bp68 = ((dunMapsHeaders[glbCreatureSomeZMap].RawColumn() +1) << 5) << 2;
							//^1C9A:244C
							bp68 += 200;
							//^1C9A:2459
							if (i32(bp68) > glbFreeRAMMemPool) {
								//^1C9A:246D
								bp3a = ALLOC_TEMP_CACHE_INDEX();
								//^1C9A:2475
								ALLOC_CPXHEAP_MEM(bp3a, bp68);
								//^1C9A:2486
								for (bp3c = 0; dunHeader->nMaps > bp3c; bp3c++) {
									//^1C9A:248D
									if (bp1e[bp3c] < 0)
										//^1C9A:249B
										continue;
									//^1C9A:249D
									bp1a[bp3c] = reinterpret_cast<DistMapTile (*)[1][32]>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp1e[bp3c]));
									//^1C9A:24CB
								}
								//^1C9A:24DD
								bp1e[glbCreatureSomeZMap] = bp3a;
								bp56 = 1;
								bp04 = reinterpret_cast<DistMapTile (*)[1][32]>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp3a));
								ZERO_MEMORY(bp04, bp68);
							}
							else {
								//^1C9A:2517
								// SPX: I added this because I hit sometime the ATLASSERT and don't know the reason.
								if (SkCodeParam::bUseFixedMode && allocSizes[5] != 0){
									DEALLOC_UPPER_MEMORY(allocSizes[5]);
									allocSizes[5] = 0;
								}
								ATLASSERT(allocSizes[5] == 0);
								bp04 = reinterpret_cast<DistMapTile (*)[1][32]>(ALLOC_MEMORY_RAM(allocSizes[5] = bp68, afZeroMem, 1024));
								//^1C9A:2531
								bp6c += bp68;
							}
							//^1C9A:253D
							bp1a[glbCreatureSomeZMap] = bp04;
						}
						//^1C9A:2555
						if (bp30 < 4) {
							//^1C9A:255B
							bp50 = glbCurrentMapIndex;
						}
						//^1C9A:2561
						_1c9a_0648(glbCreatureSomeZMap);
						//^1C9A:256A
						bp04 = bp1a[glbCurrentMapIndex];
					}
					else {
						//^1C9A:2584
						if (bp30 < 4) {
							//^1C9A:258A
							if (_4976_5225 != 0) {
								//^1C9A:2591
								bp50 = _4976_5226.b4;
								bp4c = _4976_5226.b2;
								bp4e = _4976_5226.b3;
								
							}
							//^1C9A:25A8
							else if (glbCreatureSomeX != bp12 || glbCreatureSomeY != bp14) {
								//^1C9A:25B8
								bp50 = glbCurrentMapIndex;
							}
						}
					}
					//^1C9A:25BE
					bp4c = bp12;
					//^1C9A:25C7
					bp4e = bp14;
					//^1C9A:25CA
					bp37 = 0;
				}
			}
			//^1C9A:25CE
			if (bp37 != 0) {
				//^1C9A:25D4
				//^1C9A:2ECE
				i8 bp5f;
				if (bp37 == 0xff && bp30 < 4) {
					//^1C9A:2EE0
					bp5f = _4976_5224;
					bp37 = U8(bp2c);
					//^1C9A:2EEF
					if ((bp38 & 2) != 0) {
						//^1C9A:2EFA
						if ((bp38 & 8) != 0)
							//^1C9A:2F00
							_19f0_045a(bp12, bp14);
						//^1C9A:2F0D
						bp0c = ss;
						//^1C9A:2F19
						for (di = 0; di < cc; di++, bp0c++) {
							//^1C9A:2F1E
							if (bp0c->b0 +1 < bp2c || (bp0c->b16 & 2) == 0)
								continue;
							//^1C9A:2F3D
							U16 bp4a;
							X16 bp6e;
							X16 bp70;
							Creature *bp26 ;
							switch (bp0c->b7) {
								case  3:
									//^1C9A:2F58
									si = (glbCurrentMapIndex == glbCreatureMap && bp12 == glbCreaturePosX && bp14 == glbCreaturePosY) ? 1 : 0;
									//^1C9A:2F7C
									break;

								case  2:
									//^1C9A:2F7F
									if (glbCurrentMapIndex != glbCreatureMap || bp12 != glbCreaturePosX || bp14 != glbCreaturePosY)
										//^1C9A:2FA0
										break;
									//^1C9A:2FA3
									switch (bp0c->w8) {
										default:
											//^1C9A:2FB4
											break;

										case 1:
											//^1C9A:2FB7
											if (false
												|| bp0c->w10 == 0
												||(bp0c->w10 & (1 << ((U8(CALC_VECTOR_DIR(glbCreaturePosX, glbCreaturePosY, bp42, bp44)) - glbCreatureDir) & 3))) != 0
											) {
												//^1C9A:2FED
												goto _31c0;
											}
											//^1C9A:2FF0
											bp37 = 0xff;
											//^1C9A:2FF4
											break;

										case 3:
											//^1C9A:2FF7
											bp4a = GET_TILE_VALUE(bp12 + glbXAxisDelta[bp30], bp14 + glbYAxisDelta[bp30]);
											//^1C9A:301D
											if ((bp4a >> 5) == ttPit && (bp4a & 8) != 0 && (bp4a & 1) == 0)
												//^1C9A:3032
												goto _31c0;
											//^1C9A:3035
											bp37 = 0xff;
											//^1C9A:3039
											break;
									}
									break;

								case  8:
									//^1C9A:303C
									if ((_4976_521c >> 5) == 0)
										//^1C9A:3046
										break;

									goto _3049;

								case  9:
									//^1C9A:3049
		_3049:
									glbAIAttacksSpells &= bp0c->w10;
									//^1C9A:3054
									si = _19f0_05e8(bp0c->w8, bp04, &bp0c->w2, bp42, bp44, bp30, (bp0c->b7 == 9) ? 1 : 0);
									//^1C9A:308C
									if (si != 0) {
										//^1C9A:3090
										bp0c->w4.SetX(glbCreatureSomeX);
										//^1C9A:30A2
										bp0c->w4.SetY(glbCreatureSomeY);
										//^1C9A:30B5
										bp0c->w4.SetMap(glbCreatureSomeZMap);
										//^1C9A:30C7
										bp0c->b6 = (*(bp1a[glbCreatureSomeZMap]))[glbCreatureSomeX][glbCreatureSomeY].distance -1;
									}
									//^1C9A:30F1
									glbAIAttacksSpells = bp0084;
									//^1C9A:30F8
									break;

								case 15:
								case 17:
									//^1C9A:30FB
									si = _19f0_2165(0, bp42, bp44, bp12, bp14, bp30, bp0c->w8);
									//^1C9A:311D
									if (si == 0 && _4976_522c != 0)
										//^1C9A:312E
										bp37 = 0xff;
									//^1C9A:3132
									break;

								case 19:
									//^1C9A:3135
									if ((glbCurrentThinkingCreatureRec->b15 & 3) != bp30)
										//^1C9A:3145
										break;

									goto _3148;

								case 18:
									//^1C9A:3148
		_3148:
									if ((bp36 = _19f0_050f()) == OBJECT_END_MARKER)
										//^1C9A:3155
										break;
									//^1C9A:3158
									bp26 = GET_ADDRESS_OF_RECORD4(bp36);
									//^1C9A:3169
									if (bp26->CreatureType() != bp0c->w8)
										//^1C9A:317A
										break;
									//^1C9A:317D
									si = (bp0c->w10 == 1) ? 1 : 0;
									//^1C9A:318D
									if (si != 0)
										//^1C9A:3191
										break;
									//^1C9A:3194
									if (bp0c->w10 == 2) {
										//^1C9A:319E
										for (bp36 = bp26->GetPossessionObject(); bp36 != OBJECT_END_MARKER; bp36 = GET_NEXT_RECORD_LINK(bp36)) {
											//^1C9A:31A7
											if (bp36.DBType() != dbMissile && bp36.Dir() != bp30) {
												//^1C9A:31C0
		_31c0:
												si = 1;
												//^1C9A:31C3
												break;
											}
											//^1C9A:31C6
										}
										//^1C9A:31D8
										break;
									}
									//^1C9A:31DB
									bp6e = bp12 + glbXAxisDelta[bp30];
									//^1C9A:31EA
									bp70 = bp14 + glbYAxisDelta[bp30];
									//^1C9A:31F9
									if (glbCurrentMapIndex != glbCreatureMap || bp6e != glbCreaturePosX || bp70 != glbCreaturePosY) {
										//^1C9A:3214
										bp36 = GET_CREATURE_AT(bp6e, bp70);
										//^1C9A:3223
										si = (bp36 != OBJECT_NULL && (QUERY_CREATURE_AI_SPEC_FLAGS(bp36) & 1) == 0) ? 1 : 0;
									}
									else {
										//^1C9A:3236
										si = 1;
									}
									//^1C9A:323F
									if (si == 0)
										//^1C9A:3246
										bp37 = 0xff;
									//^1C9A:324A
									break;

								case 20:
									//^1C9A:324D
									bp36 = _19f0_050f();
									//^1C9A:3255
									if (bp36 == OBJECT_END_MARKER)
										//^1C9A:325A
										break;
									//^1C9A:325D
									if (CREATURE_CAN_HANDLE_IT(bp36, bp0c->w8) == 0)
										//^1C9A:3271
										break;
									//^1C9A:3274
									_1c9a_19d4(bp36, bp12, bp14, bp0c->w10);
									//^1C9A:328B
									si = 1;
									//^1C9A:328E
									break;

								case 22:
									//^1C9A:3291
									if ((_4976_521c & 16) == 0)
										//^1C9A:3298
										break;
									//^1C9A:329B
									si = _19f0_2813(bp0c->w10, bp42, bp44, bp12,bp14,bp30,bp0c->w8);
									//^1C9A:32BF
									if (si==0)
										//^1C9A:32C3
										bp37=0xff;
									//^1C9A:32C7
									break;

								case 23:
									//^1C9A:32C9
									if ((_4976_521c & 16)!=0){
										//^1C9A:32D0
										// SPX: bp2c = distance
										ADD_BACKGROUND_LIGHT_FROM_TILE(bp2c,bp30,bp12,bp14,4);
										//^1C9A:32E6
										bp37=0xff;
									}
									//^1C9A:32EA
									break;

								case 26:
									//^1C9A:32EC
									if (bp0c->w8 == 1 && (glbCurrentMapIndex != glbCreatureMap || bp12 != glbCreaturePosX || bp14 != glbCreaturePosY))
										//^1C9A:330F
										break;
									//^1C9A:3311
									if (_19f0_0d10(bp0c->w10,bp42,bp44,bp12,bp14,bp30)!=0)
										//^1C9A:3333
										si=1;

									break;

								case  4:
								case  5:
								case  6:
								case  7:
								case 10:
								case 11:
								case 12:
								case 13:
								case 14:
								case 16:
								case 21:
								case 24:
								case 25:

									break;
							}
							//^1C9A:3336
							if (si != 0)
								break;
							//^1C9A:333A
						}
					}
					//^1C9A:3347
					if (bp5f == 0) {
						//^1C9A:334F
						(*bp04)[bp12][bp14].distance = bp37;
					}
				}
				goto _3368;
			}
			//^1C9A:25D7
			if ((*bp04)[bp12 = glbCreatureSomeX][bp14 = glbCreatureSomeY].distance != 0) {
				//^1C9A:25F6
				continue;
			}
			//^1C9A:25F9
	_25f9:
			bp16 = glbCurrentMapIndex;
			//^1C9A:25FF
			U16 bp54;
			if ((bp38 & 1) != 0) {
				//^1C9A:260D
				bp10 = bp22;
				bp0c = ss;
				//^1C9A:2625
				for (di = 0; di < cc; di++, bp0c++, bp10++) {
					//^1C9A:262A
					if (bp0c->b0 < bp2c || (bp0c->b16 & 1) == 0)
						//^1C9A:2645
						continue;
					//^1C9A:2648
					switch (bp0c->b7) {
						case  0:
							//^1C9A:2661
							si = 1;
							//^1C9A:2664
							break;

						case  2:
							//^1C9A:2667
							switch (bp0c->w8) {
								case 4:
									//^1C9A:2701
									if (bp0c->w10 != CALC_SQUARE_DISTANCE(bp12, bp14, glbCreaturePosX, glbCreaturePosY))
										//^1C9A:2720
										break;
									//^1C9A:2723
									if ((bp12 - glbCreaturePosX == 0 || bp14 - glbCreaturePosY == 0) && _19f0_0207(glbCreaturePosX, glbCreaturePosY, bp12, bp14, &SkWinCore::_1c9a_1bae) != 0)
										//^1C9A:2755
										break;
									//^1C9A:2758
									si = 1;
									//^1C9A:275B
									break;

								case 0:
									//^1C9A:2682
									si = (glbCurrentMapIndex == glbCreatureMap && bp12 == glbCreaturePosX && bp14 == glbCreaturePosY) ? 1 : 0;
									//^1C9A:26A4
									//^1C9A:2D9B
									break;

								case 2:
									//^1C9A:26A7
									if (CALC_VECTOR_DIR(glbCreaturePosX, glbCreaturePosY, bp12, bp14) != glbCreatureDir)
										//^1C9A:26C3
										break;
									//^1C9A:26C6
									bp3c = _19f0_0207(glbCreaturePosX, glbCreaturePosY, bp12, bp14, &SkWinCore::_19f0_000a);
									//^1C9A:26E5
									if (bp3c <= 0)
										//^1C9A:26E9
										break;
									//^1C9A:26EC
									if (bp0c->w10 < bp3c)
										//^1C9A:26F8
										break;
									//^1C9A:26FB
									si = 1;
									//^1C9A:26FE
									break;

								default:
									//^1C9A:267F
									break;
							}
							break;

						case  7:
							//^1C9A:275E
							if (bp16 != glbCreatureMap)
								//^1C9A:2767
								break;
							//^1C9A:276A
							glbAIAttacksSpells &= bp0c->w10;
							//^1C9A:2775
							si = _19f0_0891(0, bp12, bp14, glbCreaturePosX, glbCreaturePosY, -1);
							//^1C9A:2791
							glbAIAttacksSpells = bp0084;
							//^1C9A:2798
							break;

						case  6:
							//^1C9A:279B
							if (bp16 != glbCreatureMap)
								//^1C9A:27A4
								break;
							//^1C9A:27A7
							glbAIAttacksSpells &= bp0c->w10;
							//^1C9A:27B2
							si = _19f0_0891(1, bp12, bp14, glbCreaturePosX, glbCreaturePosY, -1);
							//^1C9A:27CE
							glbAIAttacksSpells = bp0084;
							//^1C9A:27D5
							break;

						case  8:
						case  9:
							//^1C9A:27D8
							if (bp0c->b0 != bp2c)
								//^1C9A:27E4
								break;
							//^1C9A:27E7
							glbAIAttacksSpells &= aa;
							//^1C9A:27EF
							si = _19f0_05e8(bp0c->w8, bp04, &bp0c->w2, bp12, bp14, -1, (bp0c->b7 == 9) ? 1 : 0);
							//^1C9A:2826
							if (si != 0) {
								//^1C9A:282A
								bp0c->w2.SetX(glbCreatureSomeX);
								//^1C9A:283C
								bp0c->w2.SetY(glbCreatureSomeY);
								//^1C9A:284F
								bp0c->w2.SetMap(glbCreatureSomeZMap);
								//^1C9A:2861
								bp0c->b6 = (glbCreatureSomeX == bp12 && glbCreatureSomeY == bp14 && glbCreatureSomeZMap == bp16) 
									? bp2c
									:((*(bp1a[glbCreatureSomeZMap]))[glbCreatureSomeX][glbCreatureSomeY].distance -1);
							}
							//^1C9A:28A8
							glbAIAttacksSpells = bp0084;
							//^1C9A:28AF
							break;

						case 24:
							//^1C9A:28B2
							if ((_4976_521c & 0x10) == 0)
								//^1C9A:28B9
								break;
							//^1C9A:28BC
							bp3c = _1c9a_1b16(bp0c->w8, bp0c->w10);
							//^1C9A:28D0
							if (bp3c == -1)
								//^1C9A:28D5
								break;
							//^1C9A:28D8
							if (bp10->getDist() <= bp3c)
								//^1C9A:28E5
								break;
							//^1C9A:28E8
							if (bp3c == 0 || bp10->getDist() < 127 || bp0c->b0 == 0) {
								//^1C9A:28FE
								si = 1;
								//^1C9A:2901
								bp0c->b16 = bp0c->b16 & 0xfb;
								//^1C9A:290E
								break;
							}
							//^1C9A:2911
							bp10->setDist(i8(bp3c));
							//^1C9A:291B
							break;

						case 25:
							//^1C9A:291E
							if (false
								|| bp30 == -1
								|| (_4976_521c & 0x10) == 0 
								|| (bp3c = _1c9a_1a48(1, _4976_4ef8)) == 0xffff
								|| (bp3c & (1 << ((bp30 +2) & 3))) != 0
							) {
								//^1C9A:295B
								break;
							}
							//^1C9A:295E
							si = 1;
							//^1C9A:2961
							break;

						case 10:
							//^1C9A:2964
							if (bp16 != glbCreatureMap || (_4976_37a6[RCJ(86,_4976_4ee4)] & 3) != 0)
								//^1C9A:2982
								break;
							//^1C9A:2985
							glbAIAttacksSpells &= bp0c->w10;
							//^1C9A:2990
							si = _19f0_0891(0, bp12, bp14, -1, -1, glbCurrentThinkingCreatureRec->b15_0_1());
							//^1C9A:29B2
							glbAIAttacksSpells = bp0084;
							//^1C9A:29B9
							break;

						case  4:
							//^1C9A:29BC
							if (bp16 != glbCreatureMap || (CALC_SQUARE_DISTANCE(bp12, bp14, glbCreaturePosX, glbCreaturePosY) +8) != 2)
								//^1C9A:29E3
								break;
							//^1C9A:29E6
							si = 1;
							//^1C9A:29E9
							break;

						case 15:
						case 16:
							//^1C9A:29EC
							si = _19f0_2165(0, bp12, bp14, bp12, bp14, -1, bp0c->w8);
							//^1C9A:2D99
							//^1C9A:2D9B
							break;

						case  5:
							//^1C9A:2A0E
							if (bp0c->w12.IsValid())
								//^1C9A:2A18
								goto _2b85;
							//^1C9A:2A1B
							if (bp0c->b6 <= 0) {
								//^1C9A:2A25
								bp54 = 1;
							}
							else {
								//^1C9A:2A2C
								if ((_4976_3930 & 1) != 0) {
									//^1C9A:2A34
									_4976_3930 = (_4976_3930 >> 1) ^ 0xB400;
								}
								else {
									//^1C9A:2A41
									_4976_3930 >>= 1;
								}
								//^1C9A:2A45
								bp54 = ((_4976_3930 & 7) == 0) ? 1 : 0;
							}
							//^1C9A:2A57
							if (bp54 == 0)
								//^1C9A:2A5D
								break;
							//^1C9A:2A60
							if (bp16 == glbCreatureMap) {
								//^1C9A:2A69
								bp3c = CALC_SQUARE_DISTANCE(bp12, bp14, glbCreaturePosX, glbCreaturePosY);
							}
							else {
								//^1C9A:2A82
								bp3c = CALC_SQUARE_DISTANCE(
									dunMapsHeaders[bp16].MapOffsetX() +bp12,
									dunMapsHeaders[bp16].MapOffsetY() +bp14,
									dunMapsHeaders[glbCreatureMap].MapOffsetX() + glbCreaturePosX,
									dunMapsHeaders[glbCreatureMap].MapOffsetY() + glbCreaturePosY
									)
									<< ((dunMapsHeaders[bp16].Level() != dunMapsHeaders[glbCreatureMap].Level()) ? 1 : 0);
							}
							//^1C9A:2B20
							if (bp10->getDist() <= bp3c)
								//^1C9A:2B2E
								break;
							//^1C9A:2B31
							if (bp16 == glbCreatureMap && bp3c < 5 && (bp12 - glbCreaturePosX == 0 || bp14 - glbCreaturePosY == 0) && _19f0_0207(glbCreaturePosX, glbCreaturePosY, bp12, bp14, &SkWinCore::_1c9a_1bae) != 0)
								//^1C9A:2B72
								break;
							//^1C9A:2B75
							si = 1;
							//^1C9A:2B78
							bp10->setDist(i8(bp3c));
							//^1C9A:2B82
							break;

						case 11:
							//^1C9A:2B85
	_2b85:
							if (false
								|| (bp10->getMap() == bp16 && bp10->getX() == bp12 && bp10->getY() == bp14)
								|| (bp10->getMap() == bp50 && bp10->getX() == bp4c && bp10->getY() == bp4e)
							) {
								//^1C9A:2BCE
								si = 1;
							}
							//^1C9A:2BD1
							break;

						case 12:
							//^1C9A:2BD4
							if (false
								|| bp10->getMap() != bp16
								||(bp3c = CALC_SQUARE_DISTANCE(bp12, bp14, bp10->getX(), bp10->getY())) >= bp10->getDist()
							) {
								//^1C9A:2C0B
								if (bp10->getMap() != bp50)
									//^1C9A:2C18
									break;
								//^1C9A:2C1B
								bp3c = CALC_SQUARE_DISTANCE(bp4c, bp4e, bp10->getX(), bp10->getY());
								//^1C9A:2C37
								if (bp3c >= bp10->getDist())
									//^1C9A:2C45
									break;
							}
							//^1C9A:2C48
							si = 1;
							//^1C9A:2C4B
							bp10->setDist((i8)bp3c);
							//^1C9A:2C55
							if (bp10->getDist() != 0)
								//^1C9A:2C59
								break;
							//^1C9A:2C5C
							bp0c->b16 = bp0c->b16 & 0xfb;
							//^1C9A:2C69
							break;

						case  3:
							//^1C9A:2C6C
							if (false
								|| (bp16 == bp0082 && bp12 == bp7e && bp14 == bp80)
								|| (bp50 == bp0082 && bp4c == bp7e && bp4e == bp80)
							) {
								//^1C9A:2CA7
								si = 1;
							}
							//^1C9A:2CAA
							break;

						case 13:
							//^1C9A:2CAD
							if (bp0c->w12.IsValid())
								//^1C9A:2CB7
								goto _2b85;
							//^1C9A:2CBA
							if (bp2c <= 0)
								//^1C9A:2CC0
								break;
							//^1C9A:2CC3
							if (bp0c->b6 <= 0) {
								//^1C9A:2CCD
								//^1C9A:2661
								si = 1;
								//^1C9A:2664
								break;
							}
							//^1C9A:2CD0
							if ((_4976_3930 & 1) != 0) {
								//^1C9A:2CD8
								_4976_3930 = (_4976_3930 >> 1) ^ 0xB400;
							}
							else {
								//^1C9A:2CE5
								_4976_3930 >>= 1;
							}
							//^1C9A:2CE9
							if ((_4976_3930 & 7) != 0)
								//^1C9A:2CF1
								break;
							//^1C9A:2CF4
							si = 1;
							//^1C9A:2CF7
							break;

						case  1:
							//^1C9A:2CFA
							if (false
								|| (bp16 == bp7c && bp12 == bp78 && bp14 == bp7a)
								|| (bp50 == bp7c && bp4c == bp78 && bp4e == bp7a)
							) {
								//^1C9A:2D33
								si = 1;
							}
							//^1C9A:2D36
							break;

						case 14:
							//^1C9A:2D39
							bp3c = bp0c->w8;
							//^1C9A:2D43
							if (bp3c < 0) {
								//^1C9A:2D47
								if (RAND16(-bp3c) == 0)
									//^1C9A:2D54
									break;
								//^1C9A:2D57
								si = 1;
								//^1C9A:2D5A
								break;
							}
							//^1C9A:2D5D
							if (bp3c <= 0)
								//^1C9A:2D63
								break;
							//^1C9A:2D66
							if (RAND16(bp3c) != 0)
								//^1C9A:2D73
								break;
							//^1C9A:2D76
							si = 1;
							//^1C9A:2D79
							break;

						case 21:
							//^1C9A:2D7C
							si = CREATURE_GO_THERE(bp0c->w8, bp12, bp14, bp12, bp14, -1);
							//^1C9A:2D9B
							break;

						case 20:
							//^1C9A:2D9D
							if (bp12 == xx && bp14 == yy)
								//^1C9A:2DAB
								break;
							//^1C9A:2DAD
							bp36 = _19f0_050f();
							//^1C9A:2DB5
							if (bp36 == OBJECT_END_MARKER)
								//^1C9A:2DB8
								break;
							//^1C9A:2DBA
							if (CREATURE_CAN_HANDLE_IT(bp36, bp0c->w8) == 0)
								//^1C9A:2DCC
								break;
							//^1C9A:2DCE
							_1c9a_19d4(bp36, bp12, bp14, bp0c->w10);
							//^1C9A:2DE5
							si = 1;
							//^1C9A:2DE8
							break;

						case 23:
							//^1C9A:2DEA
							if ((_4976_521c & 16) == 0)
								//^1C9A:2DEF
								break;
							//^1C9A:2DF1
							ADD_BACKGROUND_LIGHT_FROM_TILE(bp2c, (bp30 == -1) ? glbPlayerDir : bp30, bp12, bp14, 3);

							break;

						case 17:
						case 18:
						case 19:
						case 22:

							break;
					}
					//^1C9A:2E13
					if (si != 0)
						//^1C9A:2E15
						break;
					//^1C9A:2E17
				}
			}
			//^1C9A:2E28
			if (bp2c < bp3e) {
				//^1C9A:2E30
				bp08[bp28][0] = X8(bp12);	// width
				bp08[bp28][1] = X8(bp14);	// 1 ?
				bp08[bp28][2] = X8(bp16);	// map
				bp28 = (bp28 +1) & 255;
			}
			//^1C9A:2E6F
			bp74.distance = bp2c +1;
			//^1C9A:2E77
			bp74.dir = U8(bp30);
			//^1C9A:2E7D
			bp74.w2.SetMap(bp46);
			//^1C9A:2E8D
			bp74.w2.SetX(bp42);
			//^1C9A:2E9A
			bp74.w2.SetY(bp44);
			//^1C9A:2EAB
			(*bp04)[bp12][bp14] = bp74;
			//^1C9A:2ECB
			//^1C9A:3368
		_3368:
			if (si == 0)
				//^1C9A:336C
				continue;
			//^1C9A:336F
			if ((bp0c->b16 & 16) == 0) {
				//^1C9A:3381
				if ((bp0c->b16 & 32) != 0) {
					//^1C9A:3388
					bp0c->w2.SetX(glbCreaturePosX);
					//^1C9A:3397
					bp0c->w2.SetY(glbCreaturePosY);
					//^1C9A:33AA
					bp0c->w2.SetMap(glbCreatureMap);
				}
				else {
					//^1C9A:33AF
					bp0c->w2.SetX(bp12);
					//^1C9A:33C1
					bp0c->w2.SetY(bp14);
					//^1C9A:33D4
					bp0c->w2.SetMap(bp16);
				}
				//^1C9A:33E9
				if (bp37 != 0) {
					//^1C9A:33EF
					bp0c->b6 = i8(bp2c -1);
					//^1C9A:33FB
					bp12 = bp42;
					bp14 = bp44;
					bp16 = bp46;
				}
				else {
					//^1C9A:340F
					bp0c->b6 = i8(bp2c);
				}
				//^1C9A:3419
				SkD((DLV_CAI, "CAI: (e) Go-to (%2d,%2d,%2d),%d _ %02X %02X \n", (Bitu)bp12, (Bitu)bp14, (Bitu)bp16, (Bitu)bp2c, (Bitu)bp37, (Bitu)bp0c->b16));

				bp0c->w4.SetX(bp12);
				//^1C9A:342B
				bp0c->w4.SetY(bp14);
				//^1C9A:343E
				bp0c->w4.SetMap(bp16);
			}
			//^1C9A:3450
			bp34 = di;
			//^1C9A:3455
			if (di == 0 && (((bp0c->b16 & 4) == 0) || bp0c->b0 <= bp2c))
				goto _35ba;
			//^1C9A:3477
			cc = di +1;
			//^1C9A:347D
			while (ss[--di].b1 < 0) {
				//^1C9A:347F
				if (di == 0)
					//^1C9A:3483
					goto _35ba;
			}
			//^1C9A:349A
			if ((bp0c->b16 & 4) == 0) {
				//^1C9A:34A9
				cc = di +1;
			}
			//^1C9A:34AF
			if ((bp0c->b16 & 64) == 0) {
				//^1C9A:34BE
				i16 bp2e = 0;

				do {
					//^1C9A:34C3
					bp12 = ss[di].b1;
					//^1C9A:34D7
					if (bp12 > 0)
						//^1C9A:34DB
						bp2e += bp12;
					//^1C9A:34DE
					if (ss[di].b0 > bp2c +bp2e) {
						//^1C9A:34F8
						ss[di].b0 = i8(bp2c) +i8(bp2e);
					}
					//^1C9A:350D
				} while (--di >= 0);
			}
			//^1C9A:3510
			bp38 = 0;
			bp3e = MIN_EYE_DIST;
			//^1C9A:3519
			for (di = 0; di < cc; di++) {
				//^1C9A:351D
				bp3c = ss[di].b16;
				//^1C9A:3532
				bp38 = bp38 | X8(bp3c);
				//^1C9A:353A
				if (di == 0 || ss[di].b6 < 0) {
					//^1C9A:3551
					X16 bp2e = ss[di].b0;
					//^1C9A:3564
					if ((bp3c & 2) != 0)
						//^1C9A:356B
						bp2e++;
					//^1C9A:356E
					bp3e = max_value(bp2e, bp3e);
				}
				//^1C9A:357E
			}
			//^1C9A:3584
			si = 0;
			//^1C9A:3586
		}
		//^1C9A:35A6
	} while (bp2a != bp28);

	//^1C9A:35B1
	if (bp34 != 0xffff) {
		//^1C9A:35BA
_35ba:
		bp0c = &ss[bp34];
		//^1C9A:35D0
		bp42 = bp0c->w4.GetX();
		//^1C9A:35DD
		bp44 = bp0c->w4.GetY();
		//^1C9A:35EA
		bp46 = bp0c->w4.GetMap();
		//^1C9A:35F7
		for (bp2c = bp0c->b6; --bp2c >= 0; ) {
			// SPX: bp1a is a table of n pos, n being number of maps;
			// but initially, only the pos of current map is initialized.. then we could init if we fall into a nonset map..
			// I am not sure about this fix! and why we want to ask for a map non initialized ? but hope this can prevent the crash
			if (SkCodeParam::bUseFixedMode && bp46 >= dunHeader->nMaps)
				continue;	// In this case I don't know what to do better than nothing!
			if (SkCodeParam::bUseFixedMode && bp1a[bp46] == NULL)
			{
				if (allocSizes[5] != 0)
				{
					DEALLOC_UPPER_MEMORY(allocSizes[5]);
					allocSizes[5] = 0;
				}
				bp04 = reinterpret_cast<DistMapTile (*)[1][32]>(ALLOC_MEMORY_RAM(allocSizes[5] = bp68, afDefault, 1024));
				bp6c += bp68;
				bp1a[bp46] = bp04;
			}
			
			ATLASSERT(bp1a[bp46] != NULL);

			//^1C9A:3602
			// SPX: Spotted a regular CRASH here (from saved game)
			DistMapTile bp74 = (*(bp1a[bp46]))[bp42][bp44];
			//^1C9A:362D
			wp[bp2c].setDir(bp74.dir);
			//^1C9A:3646
			wp[bp2c].setX(bp42);
			//^1C9A:3660
			wp[bp2c].setY(bp44);
			//^1C9A:3679
			if (bp74.w2.GetMap() == bp46) {
				//^1C9A:3687
				wp[bp2c].setMapCross(0);
			}
			else {
				//^1C9A:3698
				// SPX: This bugs me also ... I can get 51 as a map number while there are only 44 ... 
				// thus it would cause a crash because of bp1a[bp46] (and why change bp46 ???)
				bp46 = bp74.w2.GetMap();
				//^1C9A:36A4
				wp[bp2c].setMapCross(1);
			}
			//^1C9A:36B3
			bp42 = bp74.w2.GetX();
			//^1C9A:36BC
			bp44 = bp74.w2.GetY();
			//^1C9A:36C8
		}
	}
	//^1C9A:36D0
	if (bp56 != 0) {
		//^1C9A:36D6
		for (bp12 = 0; dunHeader->nMaps > bp12; bp12++) {
			//^1C9A:36DD
			bp3a = bp1e[bp12];
			//^1C9A:36ED
			if (bp3a >= 0)
				//^1C9A:36F1
				FREE_TEMP_CACHE_INDEX(bp3a);
			//^1C9A:36FA
		}
	}
	//^1C9A:370C

#if DLV_CAI // SPX: Added reinterpret_cast because of VC6 error
	printDistMap(glbCurrentMapIndex, reinterpret_cast<const DistMapTile (*const *)[1][32]>(bp1a));
#endif

#if UseAltic
	if (allocSizes[5] != 0) DEALLOC_UPPER_MEMORY(allocSizes[5]);
	DEALLOC_UPPER_MEMORY(allocSizes[4]);
	DEALLOC_UPPER_MEMORY(allocSizes[3]);
	DEALLOC_UPPER_MEMORY(allocSizes[2]);
	DEALLOC_UPPER_MEMORY(allocSizes[1]);
	DEALLOC_UPPER_MEMORY(allocSizes[0]);
#else
	DEALLOC_UPPER_MEMORY(bp6c);
#endif
	//^1C9A:3719
	_1c9a_0648(bp48);

	SkD((DLV_CAI, "CAI: (!) ss %d \n"
		, (Bitu)bp34));

	//^1C9A:3721
	return bp34;
}

//^1C9A:0648
void SkWinCore::_1c9a_0648(U16 mapno)
{
	//^1C9A:0648
	ENTER(0);
	//^1C9A:064C
	U16 si = mapno;
	if (glbCurrentMapIndex != si) {
		CHANGE_CURRENT_MAP_TO(si);
		if (si == _4976_4c12) {
			//^1C9A:0662
			glbCreatureDir = _4976_4c10;
			glbCreaturePosX = _4976_4c04;
			glbCreaturePosY = _4976_4c1e;
			glbCreatureMap = _4976_4c12;
		}
		else {
			glbCreatureDir = glbPlayerDir;
			glbCreaturePosX = glbPlayerPosX;
			glbCreaturePosY = glbPlayerPosY;
			glbCreatureMap = glbPlayerMap;
		}
	}
	//^1C9A:0691
	return;
}


//^2066:0D09
void SkWinCore::GAME_SAVE_MENU()
{
	//^2066:0D09
	ENTER(132);
	//^2066:0D0F
	U16 bp18 = 0;
	U8 bp21 = 0;
	//^2066:0D18
	DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
	//^2066:0D1D
	UPDATE_RIGHT_PANEL(0);
	//^2066:0D25
	_38c8_0002();
	//^2066:0D2A
	FIRE_HIDE_MOUSE_CURSOR();
	//^2066:0D2F
	REARRANGE_TIMERLIST();
	//^2066:0D34
	if (_4976_49a0 == 0 && _4976_5eb0 != 0)
		//^2066:0D42
		_476d_04e8(2);
	//^2066:0D4A
	_4976_52f4 = 2;

	//^2066:0D50
_0d50:
	_4976_5258 = 0;
	//^2066:0D5C
	_4976_5246 = NULL;
	_4976_5240 = NULL;
	glbMinionsObjectIDTable = NULL;
	//^2066:0D7A
	U8 bp13 = 0;
	U16 bp16 = 0;
	U8 *bp08;
	U8 *bp0c;
	i16 bp0e;
	U8 *bp1c;
	i16 bp1e;
	U16 bp20;

	do {
		//^2066:0D83
		if (_4976_49a0 == 0 && bp13 != 0)
			//^2066:0D90
			_476d_04e8(2);
		//^2066:0D98
		U16 bp0e = _0aaf_0067(_0aaf_02f8_DIALOG_BOX((_4976_49a0 != 0) ? 0x12 : 0x0b, bp13));
		//^2066:0DBB
		if (bp0e == 3)
			//^2066:0DC1
			goto _1523;
		//^2066:0DC4
		if (bp0e == 2) {
			//^2066:0DCA
			_2066_046d();
			//^2066:0DCE
			bp13 = 0;
		}
		//^2066:0DD5
		else if (bp0e == 1) {
			//^2066:0DDB
			if (true
				&& _4976_4c1a +100 < glbGameTick 
				&& _4976_523c +100 < glbGameTick
				&& _0aaf_0067(_0aaf_02f8_DIALOG_BOX(0x0c, 0x00)) == 3
			) {
				//^2066:0E28
				//^2066:0DCE
				bp13 = 0;
			}
			else {
				//^2066:0E2A
				FIRE_SHOW_MOUSE_CURSOR();
				//^2066:0E2F
				_4976_5bf6 = 0;
				//^2066:0E35
				bp18 = 1;
				//^2066:0E3A
				goto _1523;
			}
		}
		//^2066:0E3D
		else if (bp0e == 0) {
			//^2066:0E43
			if (_4976_49a0 != 0 || _476d_04af(0) == 1) {
				//^2066:0E57
				bp16 = 1;
			}
			else {
				//^2066:0E5E
				bp16 = 0;
				//^2066:0E63
				switch (_4976_5eb8) {
					case 0:
						//^2066:0E73
						bp13 = 0x15;
						//^2066:0E77
						break;

					case 2:
						//^2066:0E79
						bp13 = 0x16;
						//^2066:0E7D
						break;

					case 4:
						//^2066:0E7F
						bp13 = 0x17;
						//^2066:0E83
						break;

					case 3:
						//^2066:0E85
						bp13 = 0x1a;

						break;

					case 1:

						break;
				}
			}
		}
		//^2066:0E89
	} while (bp16 == 0);

	//^2066:0E92
	bp1c = ALLOC_MEMORY_RAM(1024, afDefault, 128);
	//^2066:0EAA
	bp20 = 1;
	bp1e = 3;
	//^2066:0EB4
	goto _100f;
	//^2066:0EB7
_0eb7:
	goto _1045;

	//^2066:100F
_100f:
	if (bp1e >= 0)
		//^2066:1015
		goto _0eb7;
	//^2066:1018
	DEALLOC_UPPER_MEMORY(1024);
	//^2066:1024
	if (bp20 != 0) {
		//^2066:102A
		_0aaf_0067(_0aaf_02f8_DIALOG_BOX(0x13, bp21));
        //^2066:103E
		bp21 = 0x14;
		//^2066:1042
		goto _0d50;
	}
	//^2066:1045
_1045:
	bp0e = _2066_33e7();
	//^2066:104D
	if (bp0e < 0)
		//^2066:1051
		goto _0d50;
	//^2066:1054
	glbSKSaveNum = bp0e;
	glbSKSaveDigitAlpha = U8(glbSKSaveNum) +0x30;
	//^2066:1062
	_0aaf_02f8_DIALOG_BOX(0x0d, 0x00);
	//^2066:106D
	bp08 = FORMAT_SKSTR(ptrSKSave_dat, NULL);
	//^2066:1087
	bp0c = FORMAT_SKSTR(ptrSKSave_bak, NULL);
	//^2066:10A1
	DELETE_FILE(bp0c);
	//^2066:10AE
	FILE_RENAME(bp08, bp0c);
	//^2066:10C2
	glbDataFileHandle = FILE_CREATE(bp08);
	//^2066:10D2
	i16 bp10;
	if (glbDataFileHandle < 0)
		//^2066:10D6
		goto _14fa;
	//^2066:10D9
	bp10 = FILE_OPEN(bp0c);
	//^2066:10E9
	sksave_header_asc bp4c;
	if (bp10 >= 0) {
		//^2066:10ED
		READ_FILE(bp10, 42, &bp4c);
		//^2066:1101
		CLOSE_FILE(bp10);
	}
	else {
		//^2066:110C
		bp4c.w38 = 0;
	}
	//^2066:1116
	bp4c.wTimerFlag = 1;
	//^2066:111B
	SK_STRCPY(bp4c.sSavegameName, _4976_5268);
	//^2066:112C
	WRITE_FILE(glbDataFileHandle, 42, &bp4c);
	//^2066:1141
	if (SKSAVE_WRITE(dunHeader, 44) == 0)
		//^2066:1156
		goto _14fa;
	//^2066:1159
	if (SKSAVE_WRITE(dunMapsHeaders, dunHeader->nMaps << 4) == 0)
		//^2066:117A
		goto _14fa;
    //^2066:117D
	if (SKSAVE_WRITE(dunMapTilesObjectIndexPerColumn, _4976_4cb4 << 1) == 0)
		//^2066:1196
		goto _14fa;

	//^2066:1199
	if (SKSAVE_WRITE(dunGroundStacks, dunHeader->cwListSize << 1) == 0)
		goto _14fa;
	//^2066:11BA
	if (SKSAVE_WRITE(dunTextData, dunHeader->cwTextData << 1) == 0)
		goto _14fa;

	//^2066:11DB
	for (bp0e = 0; bp0e < 16; bp0e++) {
		//^2066:11E2
		if (SKSAVE_WRITE(glbDBObjectData[bp0e], dbSize[bp0e] * dunHeader->nRecords[bp0e]) == 0)
			//^2066:1214
			goto _14fa;
		//^2066:1217
	}
	//^2066:1220
	if (SKSAVE_WRITE(dunMapData, dunHeader->cbMapData) == 0)
		//^2066:123B
		goto _14fa;

	//^2066:123E
	_1c9a_3bab();
	//^2066:1243
	COMPACT_TIMERLIST();
	//^2066:1248
	skload_table_60 sGameVar; // bp0084
	ZERO_MEMORY(&sGameVar, 56);
	//^2066:125A
	sGameVar.dwGameTick = glbGameTick;
	sGameVar.dwRandomSeed = glbRandomSeed;
	sGameVar.wChampionsCount = glbChampionsCount;
	sGameVar.wPlayerPosX = glbPlayerPosX;
	sGameVar.wPlayerPosY = glbPlayerPosY;
	sGameVar.wPlayerDir = glbPlayerDir;
	sGameVar.wPlayerMap = glbPlayerMap;
	sGameVar.wChampionLeader = glbChampionLeader;
	sGameVar.wTimersCount = glbTimersCount;
	sGameVar.dw22 = _4976_0090;
	sGameVar.dw26 = _4976_4b80;
	sGameVar.w30 = _4976_4c00;
	sGameVar.wPlayerThrowCounter = glbPlayerThrowCounter;
	sGameVar.w34 = _4976_4c0c;

	sGameVar.wRainFlagSomething = glbRainFlagSomething;
	sGameVar.bRainAmbientLightModifier = glbRainAmbientLightModifier;
	sGameVar.bRainDirection = glbRainDirection;
	sGameVar.bRainStrength = glbRainStrength;
	sGameVar.bRainLevelForSky = glbRainLevelForSky;
	sGameVar.bRainLevelForGround = glbRainLevelForGround;
	sGameVar.bRainMultiplicator = glbRainMultiplicator;
	sGameVar.wRainStormController = glbRainStormController;
	sGameVar.bRainRelated3 = glbRainRelated3;
	sGameVar.bRainRelated2 = glbRainRelated2;
	sGameVar.dwRainSpecialNextTick = glbRainSpecialNextTick;

	s_testSKSave.StartWrite(FILE_TELL(glbDataFileHandle));

	//^2066:1317
	SUPPRESS_INIT();

	DEBUG_HELP_WRITER("Global Game Variables", &sGameVar, 56, 1);
	//^2066:131B
	if (SUPPRESS_WRITER(&sGameVar, _4976_395a, 56, 1) != 0)
		goto _14fa;
	DEBUG_HELP_WRITER("Ingame Global Flags", glbIngameGlobVarFlags, 1, 8);
	//^2066:1339
	if (SUPPRESS_WRITER(glbIngameGlobVarFlags, _4976_3956, 1, 8) != 0)
		goto _14fa;
	DEBUG_HELP_WRITER("Ingame Global Bytes", glbIngameGlobVarBytes, 1, 64);
	//^2066:1355
	if (SUPPRESS_WRITER(glbIngameGlobVarBytes, _4976_3956, 1, 64) != 0)
		goto _14fa;
	DEBUG_HELP_WRITER("Ingame Global Words", glbIngameGlobVarWords, 2, 64);
	//^2066:1371
	if (SUPPRESS_WRITER(glbIngameGlobVarWords, _4976_3956, 2, 64) != 0)
		goto _14fa;
	DEBUG_HELP_WRITER("Champion Squad", glbChampionSquad, 261, glbChampionsCount);
	//^2066:138D
	if (SUPPRESS_WRITER(glbChampionSquad, _4976_3992, 261, glbChampionsCount) != 0)
		goto _14fa;
	DEBUG_HELP_WRITER("Global Spell Effects", &glbGlobalSpellEffects, 6, 1);
	//^2066:13AC
	if (SUPPRESS_WRITER(&glbGlobalSpellEffects, _4976_3a97, 6, 1) != 0)
		goto _14fa;
	DEBUG_HELP_WRITER("Timers Table", glbTimersTable, 10, glbTimersCount);
	//^2066:13C8
	if (SUPPRESS_WRITER(glbTimersTable, _4976_3a9d, 10, glbTimersCount) != 0)
		goto _14fa;
	//^2066:13EA
	_4976_5240 = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(dunHeader->nRecords[dbContainer] << 1, afDefault, 1024));
	//^2066:140E
	_4976_5246 = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(dunHeader->nRecords[dbCreature] << 1, afDefault, 1024)); // dbCreature or dbMissile ??? (check WRITE_MINION_ASSOC)
	//^2066:1432
	glbMinionsObjectIDTable = reinterpret_cast<ObjectID *>(ALLOC_MEMORY_RAM(200, afDefault, 1024));
	//^2066:144B
	glbMinionsAssocCount = _4976_3952 = _4976_3950 = 0;
	//^2066:1456
	_4976_5258 = 0;
	//^2066:1462
	for (bp0e = 0; bp0e < glbChampionsCount; bp0e++) {
		//^2066:1469
		ObjectID *bp04 = glbChampionSquad[bp0e].inventory;
		//^2066:147A
		for (i16 bp12 = 0; bp12 < 30; bp12++) {
			//^2066:1481
			if (WRITE_RECORD_CHECKCODE(*(bp04++), 0, 0) != 0)
				goto _14fa;
			//^2066:149A
		}
		//^2066:14A3
	}
	DEBUG_HELP_WRITER("Leader Hand Object Ref", &glbLeaderHandPossession.object, 2, 1);
	//^2066:14AF
	if (WRITE_RECORD_CHECKCODE(glbLeaderHandPossession.object, 0, 0) != 0)
		goto _14fa;
	//^2066:14C3
	if (STORE_EXTRA_DUNGEON_DATA() == 0)
		goto _14fa;
	//^2066:14CA
	if (WRITE_MINION_ASSOC() != 0)
		goto _14fa;
	//^2066:14D2
	if (SUPPRESS_FLUSH() != 0)
		goto _14fa;
	//^2066:14DA
	CLOSE_FILE(glbDataFileHandle);
	//^2066:14E4
	_4976_5bf6 = 1;
	//^2066:14EA
	_4976_523c = glbGameTick;
	//^2066:14F8
	goto _1523;

	//^2066:14FA
_14fa:
	CLOSE_FILE(glbDataFileHandle);
	//^2066:1504
	DELETE_FILE(bp08);
	//^2066:1511
	_0aaf_0067(_0aaf_02f8_DIALOG_BOX(0x00, 0x1b));

_1523:
	//^2066:1523
	if (glbMinionsObjectIDTable != NULL) {
		//^2066:152C
		DEALLOC_UPPER_MEMORY(200);
	}
	//^2066:1538
	if (_4976_5246 != NULL) {
		//^2066:1541
		DEALLOC_UPPER_MEMORY(U32(dunHeader->nRecords[dbCreature]) << 1);
	}
	//^2066:1558
	if (_4976_5240 != NULL) {
		//^2066:1561
		DEALLOC_UPPER_MEMORY(U32(dunHeader->nRecords[dbContainer] << 1));
	}
	//^2066:1578
	FILL_ORPHAN_CAII();
	//^2066:157D
	if (bp18 != 0) {
		//^2066:1583
		END_GAME(0);
	}
	//^2066:158B
	_2066_03e0(0);
	//^2066:1592
	_38c8_0060();
	//^2066:1597
	FIRE_SHOW_MOUSE_CURSOR();
	//^2066:159C
	return;
}

//^2481:01D8
void SkWinCore::SHOW_CREDITS()
{
	//^2481:01D8
	ENTER(0);
	//^2481:01DC
	X16 si = SCREEN_CREDITS_TIMER;		// SPX: 1800 time before going back to main screen
	FIRE_SELECT_PALETTE_SET(0);
	FIRE_HIDE_MOUSE_CURSOR();
	DRAW_GAMELOAD_DIALOGUE_TO_SCREEN(glbImageCreditScreen, 2, -1, _4976_52b6);
	FIRE_SELECT_PALETTE_SET(1);
	FIRE_SHOW_MOUSE_CURSOR();
	_1031_0541(1);
	do {
		MessageLoop(true); // credits

		//^2481:021D
		MAIN_LOOP();
		if (glbUIEventCode == UI_EVENTCODE_QUIT_CREDITS) // 239
			break;
		//^2481:022A
		SLEEP_SEVERAL_TIME(1);
		//^2481:0232
	} while (--si != 0);
	//^2481:0239
	glbSpecialScreen = _MENU_SCREEN__SHOW_CREDITS;;	// 218 (0xDA)
	_1031_0541(0);
	FIRE_HIDE_MOUSE_CURSOR();
	//^2481:024C
	return;
}

// SPX Renamed inportb to SK_IMPORTB to avoid confusion with DOS inportb
U8 SkWinCore::SK_IMPORTB(U16 port) {
#ifdef __DJGPP__
	printf("SK_IMPORTB : %d %02x\n", port, port);
#endif // __DJGPP__
	switch (port) {
		case 0x60: // key in
		{
			CSkKinput *p = skwin.DequeueKinput();
			if (p != NULL)
				return p->raw;
			return 0;
		}
		case 0x64: // keyboard status: 2=still buffered
			return (skwin.cntKeybIn != 0) ? 2 : 0;
	}
	return 0;
}

void SkWinCore::MessageLoop(bool fBalanceWait, bool fShortWait) {
	//printf("MessageLoop: %d %d\n", (int)fBalanceWait, (int)fShortWait);
	if (fBalanceWait) {
		if (fShortWait) {
			if (!skwin.ML())
				throw i8();
		}
		else {
			if (tickThen == (U32)-1)
				tickThen = skwin.GetTickCount();
			while (true) {
				if (!skwin.ML())
					throw i8();
				U32 tickDelta = skwin.GetTickCount() -tickThen;
				if (tickDelta >= (fShortWait ? (TICK_TIME_STAY >> 1) : (TICK_TIME_STAY)))
					break;
				skwin.Sleep(1);
			}
			tickThen = skwin.GetTickCount();
		}
	}
	if (_04bf_0e7a != 0) // if block hardware interruption
		return;
#ifdef __DJGPP__
	//printf("Call GetMousePos\n");
	skwin.GetMousePosButtons(&mice_x, &mice_y, &mice_btn);
	(this->*_int33_mouse_callback)();
#else
	// mouse event notice
	for (int i = 0; i < skwin.cntMiceIn; i++) {
		mice_btn = skwin.xMiceInput[i].btn;
		mice_x = skwin.xMiceInput[i].x;
		mice_y = skwin.xMiceInput[i].y;
		(this->*_int33_mouse_callback)();
	}
#endif
	skwin.cntMiceIn = 0;

	// key event notice
	if (skwin.cntKeybIn != 0) {
		(this->*_sys_getvect(0x09))();
	}

	// interval timer notice
	(this->*_sys_getvect(0x08))();
}

//^2C1D:105F
// SPX: The default delay with fresh champions is 4 in original code.
// However, this delay seems to be too much for standard compatibility with DM1 trigger delays; it should rather be 2.
// And there are some curious calculation ...
// For reference
//	1 = maximum speed (given by any aura of speed value)
//	2 = normal speed with no charges
//	3 = medium speed (3/5 of max load)
//	4 = overcharged (exceed max load)
i16 SkWinCore::CALC_PLAYER_WALK_DELAY(U16 player)
{
	//^2C1D:105F
	ENTER(6);
	//^2C1D:1065
	if (glbGlobalSpellEffects.AuraOfSpeed != 0)
		return 1;
	//^2C1D:1072
	Champion *champion = &glbChampionSquad[player];
	U16 maxLoad = MAX_LOAD(champion);			// bp06
	U16 currentLoad = GET_PLAYER_WEIGHT(player);		// di
	U16 walkDelay = 0;		
	U16 walkPenalty = 0; // another usage of di
	if (!SkCodeParam::bUseFixedMode)	// SPX: I rewrote the whole block as I think there are some weird calculations
	{
		//^2C1D:109E
		if (maxLoad > currentLoad) {	// Standard case, current load is < than max load
			walkDelay = 2;
			if ((U32(maxLoad) << 3) > maxLoad * 5U) {	// Wait ... "is 8 > 5 ?" => of course, certainly an error of translation
				//^2C1D:10CC
				walkDelay++;
			}
			//^2C1D:10CD
			walkPenalty = 1;
		}
		else {				// Champion exceeds his max load
			//^2C1D:10D2
			walkDelay = ((currentLoad -maxLoad) << 2) / maxLoad +4;
			walkPenalty = 2;
		}
		//^2C1D:10E7
		if ((champion->bodyFlag & 0x20) != 0)	// Add penalties (what is 0x20 flag? wounded?)
			walkDelay += walkPenalty;
		//^2C1D:10F4
		walkDelay = max_value(1, walkDelay - champion->walkSpeed);	// walkspeed is changed by item giving bonus (positive)
		if (walkDelay > 2) {
			walkDelay = (walkDelay +1) & 0xfffe;
		}
	}
	else if (SkCodeParam::bUseFixedMode)
	{
		// standard si = 2;
		//^2C1D:109E
		if (maxLoad > currentLoad) {	// Standard case, current load is < than max load
			walkDelay = 2;
			if ((U32(currentLoad) << 3) > maxLoad * 5U) {
				//^2C1D:10CC
				walkDelay++;	// medium => si = 3
			}
			//^2C1D:10CD
			walkPenalty = 1;	// penalty
		}
		else {				// Champion exceeds his max load
			//^2C1D:10D2
			walkDelay = ((currentLoad - maxLoad) << 2) / maxLoad + 4;
			walkPenalty = 2;
		}
		//^2C1D:10E7
		if ((champion->bodyFlag & 0x20) != 0)	// Add penalties (what is 0x20 flag? wounded?)
			walkDelay += walkPenalty;
		//^2C1D:10F4
		walkDelay = max_value(1, walkDelay - champion->walkSpeed);	// walkspeed is changed by item giving bonus (positive)
		//if (si > 2) {	// This is for rounding up to next even number. Why not try without this?
		//	si = (si +1) & 0xfffe;
		//}
	}
	return walkDelay;
}

//^12B4:0953
X16 SkWinCore::_12b4_0953(Creature *rec, U16 ww)
{
	//^12B4:0953
	ENTER(0);
	//^12B4:0958
	U16 si = (rec->b15_0_1() +ww) & 3;
	if (si == 1 || si == 3) {
		i16 di = QUERY_CREATURE_5x5_POS(rec, si);
		if (((di / 5) -2) != 0)
			return 1;
	}
	//^12B4:0998
	return 0;
}

//^12B4:0881
X16 SkWinCore::_12b4_0881(X16 aa, U16 oldTile, U16 newTile, i16 xx, i16 yy, ObjectID *rl)
{
	//^12B4:0881
	ENTER(0);
	//^12B4:0886
	*rl = OBJECT_NULL;
	X16 di = ((oldTile >> 5) == ttStairs) ? 1 : 0;
	//^12B4:08A1
	if (di != 0 && aa == 2)
		return 1;
	//^12B4:08B1
	if ((newTile >> 5) == ttStairs) {
		return 2;
	}
	//^12B4:08C1
	if (IS_TILE_BLOCKED(U8(newTile)) != 0) {
		return 3;
	}
    //^12B4:08D4
	*rl = GET_CREATURE_AT(xx, yy);
	if (*rl != OBJECT_NULL) {
		//^12B4:08EC
		if ((QUERY_CREATURE_AI_SPEC_FLAGS(*rl) & 0x8000) == 0) {
			if (_12b4_0953(GET_ADDRESS_OF_RECORD4(*rl), glbPlayerDir) != 0)
				return 5;
			return 4;
		}
	}
	//^12B4:091F
	ObjectID si = _1c9a_03cf(&xx, &yy, 255);
	if (si != OBJECT_NULL) {
		if ((QUERY_CREATURE_AI_SPEC_FLAGS(si) & 0x8000) == 0)
			return 5;
	}
	return 6;
}
//^12B4:023F
// SPX: Walk into a wall ?
U16 SkWinCore::_12b4_023f(i16 xx, i16 yy, i16 *ss, i16 *tt, i16 ww, X16 vv)
{
	//^12B4:023F
	ENTER(6);
	//^12B4:0245
	vv += ww +2;
	i16 bp06[2];
	bp06[0]= _2c1d_028c(xx, yy, vv & 3);
	//^12B4:0265
	*ss = bp06[0];
	//^12B4:026B
	bp06[1] = _2c1d_028c(xx, yy, (vv +1) & 3);
	//^12B4:0284
	*tt = bp06[1];
	//^12B4:028A
	if (bp06[0] == bp06[1])
		bp06[1] = -1;
	//^12B4:0297
	U16 di;
	for (i16 si = 0; si < 2; si++) {
		//^12B4:029D
		i16 bp02 = bp06[si];
		if (bp02 >= 0) {
			//^12B4:02B0
			if (WOUND_PLAYER(bp02, 1, 0x18, 2) != 0) {
				//^12B4:02C5
				di = 1;
				QUEUE_NOISE_GEN2(GDAT_CATEGORY_CHAMPIONS, glbChampionSquad[bp02].HeroType(), SOUND_CHAMPION_BUMP, 0xfe, xx, yy, 1, 0x64, 0xc8);
			}
		}
		//^12B4:02F4
	}
	//^12B4:02FA
	return di;
}
//^12B4:099E
X16 SkWinCore::_12b4_099e(ObjectID rl)
{
	//^12B4:099E
	ENTER(8);
	//^12B4:09A4
	i16 bp06 = GET_CREATURE_WEIGHT(rl);
	if (bp06 == 0xff)
		return 0;
	X16 bp08 = 0;
	U16 si;
	for (si = 0; si < glbChampionsCount; si++) {
		//^12B4:09C3
		Champion *bp04 = &glbChampionSquad[si];
		if (bp04->curHP() != 0) {
			i16 di = GET_PLAYER_ABILITY(bp04, abStr, 0);
			if (si == glbChampionLeader) {
				di += di >> 3;
			}
			//^12B4:09FC
			if (bp06 <= 0x2d) {
				di += di >> 2;
			}
			//^12B4:0A09
			di = STAMINA_ADJUSTED_ATTR(bp04, di);
			if (di < bp06) {
				if (RAND16(((bp06 -di) >> 1) +1) != 0)
					continue;
			}
			//^12B4:0A32
			bp08 = 1;
			break;
		}
		//^12B4:0A39
	}
	//^12B4:0A40
	for (si = 0; si < glbChampionsCount; si++) {
		//^12B4:0A44
		if (glbChampionSquad[si].curHP() != 0) {
			//^12B4:0A54
			if (glbChampionSquad[si].curStamina() > glbChampionSquad[si].maxStamina()) {
				ADJUST_STAMINA(
					si,
					(bp08 != 0) ? max_value(5, bp06 >> 2) : 5
					);
			}
		}
		//^12B4:0A97
	}
	//^12B4:0A9E
	return bp08;
}



//^1031:06B3
sk0d9e *SkWinCore::_1031_06b3(sk1891 *ref, X16 xx)
{
	//^1031:06B3
	ENTER(12);
	//^1031:06B8
	U8 *bp0c = _1031_023b(ref);
	//^1031:06CC
	do {
		sk1891 *bp08 = &_4976_1574[*bp0c & 0xff7f];
		if ((this->*_4976_0cba[ref->b0_0_6()])(bp08) != 0) {
			//^1031:0702
			if (bp08->b0_7_7() == 1) {
				//^1031:070E
				sk0d9e *bp04 = _1031_06b3(bp08, xx);
				if (bp04 != 0)
					return bp04;
			}
			else {
				//^1031:0734
				sk0d9e *bp04 = _1031_024c(bp08);
				if (bp08 != NULL) {
					X16 si;
					for (; (si = (0x7ff & bp04->w0)) != 0; bp04++) {
						//^1031:074C
						if (xx != si)
							return bp04;
						//^1031:0753
					}
				}
			}
		}
		//^1031:0766
	} while ((*(++bp0c) & 0x80) == 0);
	//^1031:077A
	return NULL;
}

//^1031:0781
void SkWinCore::_1031_0781(Bit16u xx) 
{
	//^1031:0781
	ENTER(12);
	//^1031:0785
	sk0d9e *bp04 = _1031_06b3(&_4976_1891[_4976_19ad], xx);
	SRECT bp0c;
	if (bp04 != NULL && _1031_01d5(bp04->w2, &bp0c) != 0) {
		//^1031:07BB
		FIRE_QUEUE_MOUSE_EVENT(bp0c.x, bp0c.y, 255 & bp04->w4);
	}
	//^1031:07D4
	return;
}

//^4726:0383
void SkWinCore::_4726_0383()
{
	//^4726:0383
	ENTER(0);
	//^4726:0386
	LOADDS(0x0433);
	glbAbsoluteTickCounter += glbTickStepValue;
	glbIntermediateTickCounter += glbTickStepValue;
	if (i16(glbIntermediateTickCounter) >= i16(glbTickSpeed))
		glbTickStepReached = 1;

	if (glbTickStepReached)
	{
		SkD((DLV_DBG_TICK, "GameTick:%06d, TStep:%02d, TAbsolute:%03d, TCurrent:%03d, TSpeed:%03d, Reached:%d\n"
			, glbGameTick
			, glbTickStepValue
			, glbAbsoluteTickCounter
			, glbIntermediateTickCounter
			, glbTickSpeed
			, glbTickStepReached
			));
		SkD((DLV_DBG_TICK, "==================================================================\n"));
	}
	//^4726:03B0
	return;
}

//^01B0:051A
Bit16u SkWinCore::_01b0_051a() //#DS=04BF
{
	//^01B0:051A
	ENTER(0);
	//^01B0:051E
	LOADDS(0x3083);
	//^01B0:0524
	if (glbDMode != 0 && _04bf_0284 != 0) {
		//^01B0:0532
		_4726_0383() INDIRECT_CALL;
	}
	//^01B0:0536
	return (_04bf_1886 != 0) ? 1 : 0;
}

//^476D:05A9
Bit16u SkWinCore::_476d_05a9()
{
	//^476D:05A9
	ENTER(0);
	//^476D:05AC
	return _01b0_051a() CALL_IBMIO;
}

//^476D:04E8
Bit8u SkWinCore::_476d_04e8(Bit16u xx) { // TODO: Unr
	Unr(); return 0;
}

//^476D:04AF
Bit16u SkWinCore::_476d_04af(Bit16u xx) { // TODO: Unr
	Unr(); return 0;
}

//^01B0:04E4
X16 SkWinCore::_01b0_04e4() //#DS=04BF
{
	//^01B0:04E4
	ENTER(0);
	//^01B0:04E8
	LOADDS(0x3083);
	while (_04bf_1886 == 0);
	//^01B0:04F5
	X16 si = _04bf_188e[_04bf_1864];
	_04bf_1864 = (_04bf_1864 +1) % 10;
	_04bf_1886--;
	return si;
}

//^476D:050E
Bit16u SkWinCore::_476d_050e()
{
	//^476D:050E
	ENTER(0);
	//^476D:0513
	X16 si = _01b0_04e4() CALL_IBMIO;
	X16 di = si;
	switch (di) {
	case 0x1048://^0534
		si = 0x4c; break;
	case 0x1050://^0539
		si = 0x50; break;
	case 0x104B://^053E
		si = 0x4b; break;
	case 0x104D://^0543
		si = 0x4d; break;
	case 0x124B://^0548
		si = 0x4f; break;
	case 0x124D://^054D
		si = 0x51; break;
	}
	//^476D:0550
	return si;
}
//^1031:06A5
void SkWinCore::_1031_06a5()
{
	//^1031:06A5
	ENTER(0);
	//^1031:06A8
	_1031_0541(_4976_4ea8);
	//^1031:06B1
	return;
}

//^2FCF:0B8B
void SkWinCore::_2fcf_0b8b(Bit16u xx, Bit16u yy, Bit16u zz)
{
	//^2FCF:0B8B
	ENTER(6);
	//^2FCF:0B91
	_4976_4c12 = 0xffff;
	//^2FCF:0B97
	TELE_inf bp06;
	_4976_4c08 = GET_TELEPORTER_DETAIL(&bp06, Bit8u(xx), Bit8u(yy));
	//^2FCF:0BAD
	if (_4976_4c08 != 0) {
		//^2FCF:0BB1
		_4976_4c12 = bp06.target_z();
		_4976_4c04 = bp06.target_x_pos();
		_4976_4c1e = bp06.target_y_pos();
		_4976_4c10 = (glbPlayerDir + (bp06.cross_scene_test_of_teleporter_target() - bp06.cross_scene_test())) & 3;
	}
	else {
		//^2FCF:0BE6
		for (Bit16u si = 0; si < 4; si++) {
			//^2FCF:0BEA
			if (GET_TELEPORTER_DETAIL(&bp06, xx + glbXAxisDelta[si], yy + glbYAxisDelta[si]) != 0) {
				//^2FCF:0C13
				_4976_4c12 = bp06.target_z();
				//^2FCF:0C1B
				Bit16u di = bp06.cross_scene_test_of_teleporter_target() +6 -bp06.cross_scene_test();
				//^2FCF:0C2C
				si = (si +di +2) & 3;
				//^2FCF:0C37
				_4976_4c04 = bp06.target_x_pos() + glbXAxisDelta[si];
				//^2FCF:0C47
				_4976_4c1e = bp06.target_y_pos() + glbYAxisDelta[si];
				//^2FCF:0C57
				_4976_4c10 = (glbPlayerDir + di) & 3;
				//^2FCF:0C62
				break;
			}
			//^2FCF:0C64
		}
	}
	//^2FCF:0C6A
	glbCurrentMapIndex = -1;
	//^2FCF:0C70
	CHANGE_CURRENT_MAP_TO(zz);
	//^2FCF:0C79
	return;
}

//^443C:06AF
void SkWinCore::_443c_06af(sk0cea *ref)
{
	//^443C:06AF
	ENTER(0);
	//^443C:06B2
	return;
}

//^1031:096A
void SkWinCore::_1031_096a()
{
	//^1031:096A
	ENTER(0);
	//^1031:096E
	for (i16 si = 0; si < 18; si++) {
		//^1031:0972
		_443c_06af(&_4976_0ce0[1 +si]);
		//^1031:0985
	}
	//^1031:098B
	return;
}

//^482B:015C
// SPX: looks like searching next available index in the _4976_49d4 table ?
Bit16u SkWinCore::_482b_015c(Bit16u xx)
{
	//^482B:015C
	ENTER(0);
	//^482B:0160
	for (Bit16u si = 0; si < _4976_49d4[0][0]; si++) {
		//^482B:0164
		if (_4976_5f06[si].w5 == xx) {
			//^482B:017A
			return si +1;
		}
		//^482B:017F
	}
	//^482B:0186
	return 0;
}

//^01B0:18D3
void SkWinCore::_01b0_18d3(Bit16u xx) //#DS=4976
{
	//^01B0:18D3
	ENTER(2);
	//^01B0:18D8
	Bit8u bp02 = _4976_04ee;
	//^01B0:18E9
	LOADDS(0x3083);	//#DS=04BF
	//^01B0:18EF
	if (sndSoundInBuffer != 0) {
		//^01B0:18F6
		if (sndLockSoundBuffer != 0) {
			//^01B0:18FE
			sndLockSoundBuffer = 1;	
			//^01B0:1908
			while (sndLockSoundBuffer != 0);
			//^01B0:1910
			for (Bit16u si = 0; si < sndSoundBufferSize; si++) {
				//^01B0:1914
				sndSoundBuffer[si] = 0;
				//^01B0:192C
			}
		}
		//^01B0:1933
	}
	else {
		//^01B0:1935
		IBMIO_BLEND_TO_SNDBUFF(&bp02, 1, 1, 6000);
	}
	//^01B0:1949
	return;
}

//^47EB:02C3
void SkWinCore::_47eb_02c3(sk5f0a *ref)
{
	//^47EB:02C3
	ENTER(0);
	//^47EB:02C6
	_01b0_18d3(0) CALL_IBMIO;
	//^47EB:02D1
	return;
}

//^47EB:00D9
void SkWinCore::_47eb_00d9(sk5f0a *ref)
{
	//^47EB:00D9
	ENTER(4);
	//^47EB:00DD
	_47eb_02c3(ref);
	//^47EB:00EA
	sk5f0a **bp04 = &_4976_5efa;
	//^47EB:00F2
	while ((*bp04) != NULL && (*bp04) != ref) {
		//^47EB:00F4
		bp04 = &(*bp04)->pv8;
		//^47EB:0107
	}
	//^47EB:0124
	if ((*bp04) == ref) {
		//^47EB:0138
		(*bp04) = ref->pv8;
	}
	//^47EB:014D
	return;
}

//^482B:07C2
void SkWinCore::_482b_07c2(Bit16u xx)
{
	//^482B:07C2
	ENTER(0);
	//^482B:07C7
	Bit16u si;
	for (si = 0; _4976_49d4[0][0]-- > si; ) {
		//^482B:07CB
		Bit16u di = _4976_5f06[_4976_49d4[0][0]].w5;
		//^482B:07DF
		if (di != 0xffff) {
			//^482B:07E4
			if (_482b_015c(di) == 0) {
				//^482B:07EE
				_47eb_00d9(&_4976_5f0a[_4976_5f06[_4976_49d4[0][0]].w0]);
				//^482B:0814
				_4976_49d4[0][1]--;
			}
		}
		//^482B:0818
	}
	//^482B:0823
	_4976_49d4[0][0] = si;
	_4976_49d0 = 0;
	//^482B:082D
	return;
}

//^4726:00F3
void SkWinCore::EMS_FREE()
{
#if UseAltic
	;
#else
//		//^4726:00F3
//		ENTER(0);
//		//^4726:00F6
//		if (_4976_5ea4 != 0) {
//			//^4726:00FD
//			__asm mov dx,[_4976_5ea4]
//			__asm mov ah,0x45
//			__asm int 0x67
//		}
//		//^4726:0105
//		return;
#endif
}

//^069A:05DE
void SkWinCore::_farfree(void *ref) {
	ATLASSERT(false);
}

//^069A:014A
void SkWinCore::__restorezero() {
}

//^069A:00EA
void __DECLSPEC_NORETURN_ SkWinCore::__terminate(Bit8u errorLevel)
{
	ATLASSERT(false);
//		//^069A:00EA
//		//^069A:00EF
//		__restorezero();
//		//^069A:00F4
//		__asm mov bp,sp
//		__asm mov ah,0x4c
//		__asm mov al,[bp+04]
//		__asm int 0x21h
}

//^4726:0448
void SkWinCore::SK_EXIT()
{
	//^4726:0448
	ENTER(0);
	//^4726:044B
	EMS_FREE();
	//^4726:044F
	_farfree(_4976_5e94);
	//^4726:045E
	__terminate((_4976_4c26 != 0) ? 1 : 0);
	//^4726:0473
	return;
}

//^069A:043C
/*
void SkWinCore::_sk_longjmp(jmp_buf xx, Bit16u yy)
{
	ATLASSERT(false);
	longjmp(xx, yy);
}
*/

//^101B:0111
void SkWinCore::SK_PREPARE_EXIT()
{
	//^101B:0111
	ENTER(0);
	//^101B:0114
	_1031_096a();
	//^101B:0119
	while (glbMouseVisibility <= 0)
		//^101B:011B
		FIRE_HIDE_MOUSE_CURSOR();
	//^101B:0127
	GRAPHICS_DATA_CLOSE();
	//^101B:012C
	GRAPHICS_DATA_CLOSE();
	//^101B:0131
	_482b_07c2(1);
	//^101B:0139
	if (_4976_4c26 != 0) {
		//^101B:0140
		FIRE_FILL_SCREEN_RECT(2, 0);
	}
#if UseAltic
	throw i8(0);
#else
	//^101B:014B
	SK_EXIT();
	//^101B:0150
	_sk_longjmp(_4976_4de0, 1);
#endif
	//^101B:015B
	return;

}

//^0B36:1647
void SkWinCore::DRAW_DIALOGUE_PARTS_PICT(Bit8u *buffsrc, SRECT *rc, i16 colorkey, Bit8u *localpal)
{
	// the back buffer is an off-screen buffer of the dungeon viewport (224x136x8bpp)

	// what you draw:
	// a) dialogue window
	// b) dialogue button
	// c) 18x18 bevelled icon frame

	//^0B36:1647
	//^0B36:164A
	FIRE_BLIT_PICTURE(
		buffsrc,
		_4976_4c16,
		rc,
		0,
		0,
		READ_UI16(buffsrc,-4),
		_4976_00f6,
		colorkey,
		0,
		READ_UI16(buffsrc,-6),
		8,
		localpal
		);
	//^0B36:1686
	return;
}

//^3E74:0AE3
void SkWinCore::FREE_PICT_ENTRY(Bit8u *buff)
{
	//^3E74:0AE3
	//^3E74:0AE7
	if (_4976_5d76 == 0) {
		//^3E74:0AF1
		sk5cfc_image *bp04 = reinterpret_cast<sk5cfc_image *>(&_4976_5cfc);
		//^3E74:0AF9
		for (; (void *)PTR_PADD(buff,-int(sizeof(sk5cfc_image))) != (void *)bp04->pv0; ) {
			//^3E74:0AFB
			bp04 = bp04->pv0;
		}
		//^3E74:0B31
		bp04->pv0 = bp04->pv0->pv0;
		//^3E74:0B48
		Bit32u bp08 = CALC_IMAGE_BYTE_LENGTH(buff) +0x1e;
		//^3E74:0B60
		if (reinterpret_cast<SkImage *>(PTR_PADD(buff,-int(sizeof(sk5cfc_image))))->AllocLower() == 0) {
			//^3E74:0B7C
			DEALLOC_UPPER_MEMORY(bp08);
		}
		else {
			//^3E74:0B88
			DEALLOC_LOWER_MEMORY(bp08);
		}
	}
	//^3E74:0B94
	return;
}

//^3929:0BD7
void SkWinCore::DRAW_VP_RC_STR(Bit16u rectno, Bit16u clr1, const U8 *str)
{
	// draw string to viewport, within specified rectangle.

	// like dialog ver: "V1.0"
	// like buttons: "SAVE", "QUIT", "RESULT", ...
	// like item name: "SCOUT MAP", ...

	//^3929:0BD7
	//^3929:0BDB
	i16 bp02;
	i16 bp04;
	if (QUERY_STR_METRICS(str, &bp02, &bp04) != 0) {
		//^3929:0BF6
		SRECT bp0c;
		if (QUERY_BLIT_RECT(NULL, &bp0c, rectno, &bp02, &bp04, -1) != 0) {
			//^3929:0C1A
			DRAW_VP_STR(bp0c.x, bp0c.y + bp0c.cy -1, clr1, str);
		}
	}
	//^3929:0C35
	return;
}

//^3929:000F
Bit16u SkWinCore::QUERY_MBCS_PRESENCE(const Bit8u *str)
{
	//^3929:000F
	Bit8u dl;
	while ((dl = *str++) != 0) {
		//^3929:0014
		if ((dl & 0x80) != 0)
			return 1;
		//^3929:001E
		if (dl == 2) {
			//^3929:0023
			if (*str++ != 0x20)
				return 1;
			return 0;
		}
        //^3929:0031
	}
	//^3929:0040
	return 0;
}

//^3929:0044
skxxxf *SkWinCore::QUERY_CHAR_METRICS(U8 cls2, U8 yy, U8 chr, U8 *tableIdxOut)
{
	//^3929:0044
	ENTER(6);
	//^3929:0048
	skxxxf *bp04 = _4976_5bfa[cls2];
	U8 bp05 = _4976_5bf8[cls2];
	bp05--;
	//^3929:0071
	U8 cl;
	for (cl = 0; cl <= bp05; bp04++, cl++) {
		//^3929:0075
		if (true
			&& bp04->b0 == yy 
			&& bp04->b1 <= chr 
			&& bp04->w2 * bp04->w4 +bp04->b1 >= chr
		) {
			//^3929:00A2
			if (cl == bp05 || bp04[1].b0 != yy || bp04[1].b1 > chr) {
				//^3929:00BB
				if (tableIdxOut != NULL)
					*tableIdxOut = cl;
				return bp04;
			}
		}
		//^3929:00D3
	}
	//^3929:00E0
	return NULL;
}

//^3929:03DC
Bit16u SkWinCore::QUERY_STR_METRICS(const Bit8u *buff, i16 *xx, i16 *yy)
{
	//^3929:03DC
	//^3929:03E2
	if (buff == NULL) {
		//^3929:03EA
		return 0;
	}
	//^3929:03EF
	i16 si;
	Bit16u di;
	if (QUERY_MBCS_PRESENCE(buff) != 0) {
		//^3929:0402
		si = - _4976_0132;
		Bit8u bp05 = 1;
		di = 0;
		//^3929:040F
		Bit8u bp07;
		for (; *buff != 0; ) {
			//^3929:0412
			Bit8u bp06 = *(buff++);
			//^3929:041E
			if (bp06 >= 0xf0) {
				//^3929:0422
				bp07 = *buff; buff++;
			}
			//^3929:0430
			else if (bp06 < 32) {
				//^3929:0436
				if (bp06 == 2) {
					//^3929:043C
					bp05 = *buff + 0xe0;
				}
				//^3929:044A
				continue;
			}
			else {
				//^3929:044C
				bp07 = bp06;
				bp06 = 0;
			}
			//^3929:0456
			Bit8u bp08;
			skxxxf *bp04 = QUERY_CHAR_METRICS(bp05, bp06, bp07, &bp08);
			//^3929:0474
			if (bp04 != NULL) {
				//^3929:0478
				si += bp04->w6 + _4976_0132;
				//^3929:0485
				di = max_value(di, bp04->w8 - _4976_0136);
			}
			//^3929:0498
		}
		//^3929:04A4
	}
	else {
		//^3929:04A6
		si = - _4976_0120;
		di = _4976_011e - _4976_0124;
		//^3929:04B6
		while (*(buff++) != 0) {
			//^3929:04B8
			si += _4976_0126;
		}
	}
	//^3929:04C8
	if (si <= 0) {
		//^3929:04CC
		//^3929:03EA
		return 0;
	}
	//^3929:04CF
	*xx = si;
	*yy = di;
	//^3929:04DB
	return 1;
}

//^
inline Bit16u SkWinCore::QUERY_STR_METRICS(Bit8u *buff, Bit16u *xx, Bit16u *yy)
{
	return QUERY_STR_METRICS(buff, reinterpret_cast<i16 *>(xx), reinterpret_cast<i16 *>(yy));
}

//^3929:04E2
// _3929_04e2 renamed _3929_04e2_DRAW_TEXT_STRINGS
Bit16u SkWinCore::_3929_04e2_DRAW_TEXT_STRINGS(const Bit8u *strIn, Bit8u *strOut, Bit16u *textPos, i16 maxWidth)
{
	//^3929:04E2
	//^3929:04E8
	Bit16u si = *textPos;
	i16 bp02 = 0;
	Bit16u di = 0;
	Bit16u bp04 = 0xffff;
    Bit16u bp06 = 0;
	Bit16u bp08 = 0;
	//^3929:0504
	Bit8u bp0d;
	skxxxf *bp0c;
	if (QUERY_MBCS_PRESENCE(strIn) != 0 || _4976_475e != 0xff) {
		//^3929:051E
		if (_4976_475e == 0xff) {
			//^3929:0525
			_4976_475e = 1;
		}
		else {
			//^3929:052C
			strOut[di] = 2; di++;
			//^3929:0534
			strOut[di] = 0x20 + _4976_475e;
			//^3929:053C
            di++;
		}

		while (true) {
			//^3929:053D
			strOut[di] = 0;
			//^3929:0544
			if (strIn[si] == vbLf || strIn[si] == 0) {
				//^3929:0553
				if (strIn[si] == 0) {
					_4976_475e = 0xff;
				}
				//^3929:0564
				break;
			}
			//^3929:0567
			bp0d = strIn[si];
			//^3929:0570
			Bit8u bp0e;
			if (bp0d >= 0xf0) {
				//^3929:0574
				bp0e = strIn[si +1];
			}
			else {
				//^3929:057D
				if (bp0d < 0x20) {
					//^3929:0583
					strOut[di] = strIn[si]; si++; di++;
					//^3929:0591
					if (bp0d == 2) {
						//^3929:0597
						_4976_475e = (strOut[di] = strIn[si]) + 0xe0; si++; di++;
					}
					//^3929:05AA
					continue;
				}
				//^3929:05AC
				bp0e = bp0d;
				bp0d = 0;
				//^3929:05B6
				if (strIn[si] == 0x20) {
					//^3929:05BF
					bp04 = si;
					bp06 = di;
					bp08 = bp02;
				}
			}
			//^3929:05CB
			Bit8u bp0f;
			bp0c = QUERY_CHAR_METRICS(_4976_475e, bp0d, bp0e, &bp0f);
			//^3929:05E9
			if (bp0c == NULL) {
				//^3929:05ED
				si++;
				if (bp0d != 0) {
					//^3929:05F7
					si++;
				}
				//^3929:05F8
				continue;
			}
			//^3929:05FB
			if (i16(bp0c->w6) + _4976_0132 + bp02 <= maxWidth) {
				//^3929:060E
				if (bp02 == 0) {
					//^3929:0614
					bp02 = - _4976_0132;
				}
				//^3929:061C
				bp02 += bp0c->w6 + _4976_0132;
				//^3929:062A
				if (bp0d != 0) {
					//^3929:0630
					strOut[di] = strIn[si]; si++; di++;
				}
				//^3929:063E
				strOut[di] = strIn[si]; si++;
				//^3929:064B
				di++; continue;
			}
			//^3929:064E
			if (bp04 != 0xffff) {
				//^3929:0654
_0654:
				*textPos = bp04 +1;
				//^3929:065E
				strOut[bp06] = 0;
				//^3929:0668
				return bp08;
			}
			//^3929:066D
			break;
		}
	}
	else {
		while (true) {
			//^3929:066F
			strOut[di] = 0;
			//^3929:0676
			if (strIn[si] == vbLf || strIn[si] == 0)
				//^3929:0685
				break;
			//^3929:0687
			if (strIn[si] == 0x20) {
				//^3929:0690
				bp04 = si;
				bp06 = di;
				bp08 = bp02;
			}
			//^3929:069C
			if (bp02 + _4976_0126 <= maxWidth) {
				//^3929:06A8
				if (bp02 == 0) {
					//^3929:06AE
					bp02 = - _4976_0120;
				}
				//^3929:06B6
				bp02 += _4976_0126;
				//^3929:06BC
				strOut[di] = strIn[si]; si++; di++;
				//^3929:06CA
				continue;
			}
			//^3929:06CC
			if (bp04 == 0xffff) {
				//^3929:06D0
				break;
			}
			goto _0654;
		}
	}
	//^3929:06D4
	*textPos = si;
	//^3929:06DA
	return bp02;
}

//^3929:00E6
const Bit8u *SkWinCore::DRAW_MBCS_STR(
	const Bit8u *str, 
	Bit8u *dst, 
	i16 zz, 
	Bit8u *ww, 
	Bit16u clr1, 
	Bit16u clr2, 
	Bit16u *xx, 
	Bit16u *yy, 
	Bit16u dstpitch, 
	Bit16u dstbpp
) {
	//^3929:00E6
	Bit8u bp0b = *str++;
	//^3929:00F8
	Bit8u bp0c;
	if (bp0b >= 0xF0) {
		//^3929:00FC
		bp0c = *str;
		str++;
	}
	else {
		//^3929:010A
		if (bp0b < 0x20) {
			//^3929:0110
			if (bp0b != 0x02)
				return str;
			//^3929:0119
			*ww = *str + 0xE0;
			str++;
			return str;
		}
		//^3929:012D
		bp0c = bp0b;
		bp0b = 0;
	}
	//^3929:0137
	Bit8u bp0d;
	skxxxf *bp04 = QUERY_CHAR_METRICS(*ww, bp0b, bp0c, &bp0d);
	if (bp04 == NULL)
		return str;
	//^3929:015F
	Bit16u bp18 = _4976_5ca8;
	_4976_5ca8 = -_4976_5d76;
	//^3929:0170
	Bit8u *bp08 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x001c, *ww, bp0d);
	//^3929:018B
	Bit8u bp2a[16]; // 2a-1b
	Bit16u bp0a;
	if (dstbpp != 4) {
		//^3929:0191
		bp2a[ 0] = (Bit8u)clr2;
		bp2a[15] = (Bit8u)clr1;
		//^3929:019D
		bp0a = ((clr2 & 0x4000) != 0) ? 0 : -1;
		//^3929:01A6
	}
	else {
		//^3929:01B1
		if ((clr2 & 0x4000) != 0) {
			//^3929:01B8
			bp0a = clr2 = (clr1 == 0) ? 1 : 0;
		}
		else {
			//^3929:01CD
			bp0a = -1;
		}
		//^3929:01D2
		Bit8u *bp12;
		Bit16u di;
		Bit16u si;
		Bit32u bp16;
		if ((clr1 == 0 && clr2 == 0) || _4976_5d76 == 0) {
			//^3929:01E5
			bp08 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x001c, *ww, bp0d);
			//^3929:0200
			if (clr1 != 0 || clr2 != 15) {
				//^3929:020F
				Bit8u *bp12 = bp08;
				Bit16u di = ((Bit16u *)bp08)[-2];
				Bit16u si = ((Bit16u *)bp08)[-1];
				//^3929:0226
				//^3929:02E5
				goto _02e5;
			}
		}
		else {
			//^3929:0229
			bp16 = (QUERY_GDAT_ENTRY_DATA_INDEX(0x1c, *ww, 1, bp0d) & 0x1fff) 
				| 0x4000 
				| ((clr2 & 0x000f) << 4) 
				| ((clr1 & 0x000f)     );
			//^3929:026D
			Bit16u bp1a;
			if (ADD_CACHE_HASH(bp16, &bp1a) != 0) {
				//^3929:0284
				bp08 = QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp1a);
				//^3929:0293
			}
			else {
				//^3929:0295
				di = (bp04->w2 * bp04->w6);
				si = (bp04->w4 * bp04->w8);
				ALLOC_NEW_PICT(bp1a, di, si, 4);
				//^3929:02BB
				bp12 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x1c, *ww, bp0d);
				//^3929:02D6
				bp08 = QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp1a);
				//^3929:02E5
_02e5:
				Bit8u bp3a[16]; // 3a-2b
				bp3a[ 0] = (Bit8u)clr2;
				bp3a[15] = (Bit8u)clr1;
				//^3929:02F1
				FIRE_STRETCH_BLIT_TO_MEMORY_4TO4BPP(bp12, bp08, di, si, di, si, bp3a);
			}
			//^3929:030E
			if (_4976_5d76 != 0) {
				//^3929:0315
				_3e74_585a(bp1a, 0);
			}
		}
	}
	//^3929:0321
	if (zz >= 0) {
		//^3929:0327
		dst = QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(zz);
	}
	//^3929:0336
	bp0c -= bp04->b1;
	//^3929:0340
	FIRE_BLIT_PICTURE(
		bp08,
		dst,
		ALLOC_TEMP_RECT(
			*xx,
			*yy,
			bp04->w6,
			bp04->w8
			),
		(bp0c % bp04->w2) * bp04->w6,
		(bp0c / bp04->w2) * bp04->w8,
		READ_UI16(bp08,-4),
		dstpitch,
		bp0a,
		0,
		4,
		dstbpp,
		bp2a
		);
	//^3929:03AE
	*xx += bp04->w6 + _4976_0132;
	//^3929:03BF
	FREE_PICT_ENTRY(bp08);
	//^3929:03CC
	_4976_5ca8 = bp18;
	//^3929:03D2
	return str;
}

//^470A:0008
Bit8u *SkWinCore::QUERY_FONT(i8 input, Bit8u fore, Bit8u back)
{
	//^470A:0008
	Bit8u bp02 = fore << 4;
	Bit8u bp03 = back << 4;
	Bit16u si = 0;
	Bit16u di = 0;
	for (; di < 6; di++) {
		//^470A:0026
		U8 bp01 = _4976_5c0e[(di << 7) + input];
		i8 bp06;
		for (bp06 = 0; bp06 < 3; bp06++) {
			//^470A:0046
			_4976_5e72[si++] = (((bp01 & 0x10) != 0) ? bp02 : bp03) | (((bp01 & 0x08) != 0) ? fore : back);
			bp01 <<= 2;
			//^470A:0071
		}
		//^470A:007A
	}
	//^470A:0080
	return _4976_5e72;
}

//^470A:0089
void SkWinCore::DRAW_STRING(
	Bit8u *dst, 
	Bit16u ss, 
	Bit16u dstpitch, 
	Bit16u dstx, 
	Bit16u dsty, 
	Bit16u clr1, 
	Bit16u clr2, 
	const Bit8u *str, 
	Bit16u dstbpp
) {
	//^470A:0089
	Bit16u di = dstbpp;
	//^470A:0089
	if (*str == 0)
		return;
	//^470A:00A0
	Bit16u si;
	if ((clr2 & 0x4000) != 0) {
		//^470A:00A7
		clr2 &= 0xBFFF;
		si = clr2;
	}
	else {
		//^470A:00B4
		si = -1;
	}
	//^470A:00B7
	SRECT bp08;
	bp08.x = dstx;
	//^470A:00BD
	if (QUERY_MBCS_PRESENCE(str) != 0) {
		//^470A:00CE
		Bit8u bp09 = 1;
		dsty -= 14;
		//^470A:00D6
		if (si != 0xFFFF) {
			//^470A:00DB
            clr2 |= 0x4000;
		}
		do {
			//^470A:00E4
			str = DRAW_MBCS_STR(str, dst, ss, &bp09, clr1, clr2, &dstx, &dsty, dstpitch, di);
			//^470A:011E
		} while (*str != 0);
	}
	else {
		//^470A:0127
		Bit8u bp1a[16];
		if (di != 4) {
			//^470A:012C
			bp1a[0] = (Bit8u)clr1;
			bp1a[1] = (Bit8u)clr2;
			clr1 = 0;
			clr2 = 1;
			//^470A:0142
			if (si != 0xFFFF) {
				si = 1;
			}
		}
		//^470A:014A
		if (str[0] == 0x02 && str[1] == 0x20) {
			str += 2;
		}
		//^470A:015E
		// bp08 = bp08.x
		// bp06 = bp08.y
		// bp04 = bp08.dstcx
		// bp02 = bp08.dstcy
		bp08.cy = _4976_011e;
		bp08.y = (dsty + _4976_0124) - (bp08.cy - 1);
		bp08.cx = _4976_0126;
		do {
			//^470A:017A
			FIRE_BLIT_PICTURE(QUERY_FONT(*str, (Bit8u)clr1, (Bit8u)clr2), dst, &bp08, 0, 0, 5, dstpitch, si, 0, 4, di, bp1a);
			//^470A:01B7
			bp08.x += _4976_0126;
			str++;
			//^470A:01C0
		} while (*str != 0);
	}
	//^470A:01C9
}

//^3929:0B6C
void SkWinCore::DRAW_VP_STR(Bit16u dstx, Bit16u dsty, Bit16u clr1, const Bit8u *str)
{
	// draw text to dungeon/inventory viewport.
	// draw dialogue text such as.
	// a) version: "V1.0"
	// b) button text: "A6 86 97 A8" [ÉçÅ[ÉgÅJ] (=load), "8B 84 A7 92 A5" [ÉLÉÉÉìÉZÉã] (=cancel)
	// c) savegame name: "0 GAMEE", "GAME 1 STRONG", "2 UNUSED", "3 UNUSED", ...

	//^3929:0B6C
	//^3929:0B6F
	DRAW_STRING(
		_4976_4c16,
		-1,
		_4976_00f6,
		dstx,
		dsty,
		clr1,
		glbPaletteT16[COLOR_DARKEST_GRAY] | 0x4000,
		str,
		8
		);
	//^3929:0BA4
	return;
}

//^3A15:020F
void SkWinCore::_3a15_020f()
{
	//^3A15:020F
	ENTER(4);
	//^3A15:0215
	Bit16u timerCnt = glbTimersCount;	// di
	//^3A15:021A
	if (timerCnt == 0)
		//^3A15:021C
		return;
	//^3A15:0220
	Bit16u si;
	for (si = 0; si < MAX_CHAMPIONS; si++) {
		//^3A15:0222
		glbChampionSquad[si].timerIndex = TIMER_NONE;
		//^3A15:0231
	}
	//^3A15:0237
	si = 0;
	//^3A15:0239
	Timer *timer = glbTimersTable;	//*bp04
	//^3A15:0246
	for (; si < timerCnt; si++, timer++) {
		//^3A15:0248
		switch (timer->TimerType()) {
			case tty0C:
				//^3A15:0262
				glbChampionSquad[timer->actor].timerIndex = si;
				break;
			case tty1D:
			case tty1E:
				//^3A15:0278
				GET_ADDRESS_OF_RECORDE(timer->value)->TimerIndex(si);
				break;
		}
		//^3A15:028D
	}
	//^3A15:0296
	return;
}

//^3A15:029A
void SkWinCore::COMPACT_TIMERLIST()
{
	//^3A15:029A
	//^3A15:02A0
	Bit16u si = 0;
	Bit16u di = glbTimersCount;
	//^3A15:02A7
	if (di != 0) {
		//^3A15:02AB
		Timer *bp04 = glbTimersTable;

		do {
			//^3A15:02B8
			if (bp04->TimerType() == tty00) {
				//^3A15:02C2
				Timer *bp08 = &bp04[1];
				//^3A15:02D1
				for (; bp08->TimerType() == tty00; bp08++);
				//^3A15:02E1
				*bp04 = *bp08;
				//^3A15:02F5
				bp08->TimerType(tty00);
			}
			//^3A15:02FD
			bp04++;
			si++;
			//^3A15:0302
		} while (si != di);
		//^3A15:0308
		__SORT_TIMERS();
		_3a15_020f();
	}
	//^3A15:0310
	return;
}

U16 SkWinCore::RAND16(U16 maxcnt) {
	U16 r = _RAND16(maxcnt);
	SkD((DLV_DBG_RAND, "DBG: RAND16(%d) = %d\n", (Bitu)maxcnt, (Bitu)r));
	return r;
}
U16 SkWinCore::RAND01() {
	U16 r = _RAND01();
	SkD((DLV_DBG_RAND, "DBG: RAND01() = %d\n", (Bitu)r));
	return r;
}
U16 SkWinCore::RAND() {
	U16 r = _RAND();
	SkD((DLV_DBG_RAND, "DBG: RAND() = %d\n", (Bitu)r));
	return r;
}
U16 SkWinCore::RAND02() {
	U16 r = _RAND02();
	SkD((DLV_DBG_RAND, "DBG: RAND02() = %d\n", (Bitu)r));
	return r;
}

//^0CD5:0147
//SPX: Random int on 2 bits => range = 0 to 3
Bit16u SkWinCore::_RAND02()
{
	// CSBwinSimilarity: TAG001074,STRandom0_3

	//^0CD5:0147
	//^0CD5:014A
	glbRandomSeed = glbRandomSeed * 0xbb40e62d + 11;
	//^0CD5:016A
	return (glbRandomSeed >> 8) & 3;
	//^0CD5:0175
}

//^098D:02D2
Bit16u SkWinCore::CALC_SIZE_OF_COMPRESSED_RECT(Bit16u xx)
{
	//^098D:02D2
	Bit16u di = xx;
	Bit16u si = 8;
	//^098D:02DD
	if ((di & 4) != 0) {
		//^098D:02E3
		//^098D:02F7
		si = si -2;
	}
	else {
		//^098D:02E5
		if ((di & 2) != 0) {
			//^098D:02EB
			si = si -2;
		}
		//^098D:02F1
		if ((di & 1) != 0) {
			//^098D:02F7
			si = si -2;
		}
	}
	//^098D:02FD
	if ((di & 24) != 0) {
		//^098D:0303
		si = si -2;
	}
	//^098D:0309
	return si;
}

//^098D:1208
void SkWinCore::_098d_1208() //#DS=4976?
{
	// TODO: ñ{ÉYÉåîüêî
	//^098D:1208
	//^098D:120B
	LOAD_RECTS_AND_COMPRESS(1, 0, 0);
	//^098D:1218
	_4976_4ba4 = 1;
	//^098D:121E
	return;
}

//^3E74:0A51
void SkWinCore::DEALLOC_LOWER_MEMORY(Bit32u size) //#DS=4976?
{
	// TODO: ñ{ÉYÉåîüêî
	//^3E74:0A51
	//^3E74:0A54
	if ((size & 1) != 0) {
		//^3E74:0A5F
		size++;
	}
	//^3E74:0A67
	glbFreeRAMMemPool += size;
	//^3E74:0A75
	SkD((DLV_MEM, "MEM: DEALLOC_LOWER_MEMORY(%10d)\n", (Bitu)size));
#if defined(_USE_MFC80) || defined(_USE_MFC60) || defined(_USE_SDL)
	ATLVERIFY(vecLowerAlloc.size() != 0);
	ATLVERIFY(vecLowerAlloc.top() == size);
	vecLowerAlloc.pop();
#endif // defined(_USE_MFC80) || defined(_USE_MFC60) || defined(_USE_SDL)
	return;
}

//^3E74:5AB7
Bit16u SkWinCore::QUERY_GDAT_ENTRY_DATA_LENGTH(Bit8u cls1, Bit8u cls2, Bit8u cls3, Bit8u cls4)
{
LOGX(("%40s: C%02d=I%02X=E%02X=T%03d = %08X (%06d)", "QUERY_GDAT_ENTRY_DATA_LENGTH of ", cls1, cls2, cls4, cls3, QUERY_GDAT_RAW_DATA_LENGTH(QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, cls3, cls4)), QUERY_GDAT_RAW_DATA_LENGTH(QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, cls3, cls4)) ));

	//^3E74:5AB7
	return QUERY_GDAT_RAW_DATA_LENGTH(
		QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, cls3, cls4)
		);
}

//^098D:1185
void SkWinCore::LOAD_RECTS_AND_COMPRESS(Bit8u cls1, Bit8u cls2, Bit8u cls4) //#DS=4976?
{
	// TODO: ñ{ÉYÉåîüêî
	//^098D:1185
	//^098D:1189
	Bit32u bp08 = QUERY_GDAT_ENTRY_DATA_LENGTH(cls1, cls2, dt04, cls4);
	//^098D:11A2
	Bit8u *bp04 = ALLOC_MEMORY_RAM(bp08, afUseLower, 1024);
	//^098D:11C0
	LOAD_GDAT_ENTRY_DATA_TO(cls1, cls2, dt04, cls4, bp04);
	//^098D:11DC
	COMPRESS_RECTS((i16 *)bp04, bp08, &glbRectNoTable, &SkWinCore::ALLOC_UPPER_MEMORY);
	//^098D:11F9
	DEALLOC_LOWER_MEMORY(bp08);
}

//^3E74:51E4
void SkWinCore::LOAD_GDAT_ENTRY_DATA_TO(Bit8u cls1, Bit8u cls2, Bit8u cls3, Bit8u cls4, Bit8u *where) //#DS=4976?
{
//LOGX(("%40s: C%02d=I%02X=E%02X=T%03d to %08X", "LOAD_GDAT_ENTRY_DATA_TO of ", cls1, cls2, cls4, cls3, where ));
	// TODO: ñ{ÉYÉåîüêî
	//^3E74:51E4
	Bit16u si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, cls3, cls4);
	//^3E74:5201
//LOGX(("LOAD_GDAT_RAW_DATA call from LOAD_GDAT_ENTRY_DATA_TO"));
	LOAD_GDAT_RAW_DATA(si, CONVERT_PHYS_TO_SHELF_FORM(where));

	SkD((DLV_GLD, "GLD: Dyn1-load Raw#%4d at RAM(%p)\n", (Bitu)si, where));
}

//^098D:116D
Bit8u *SkWinCore::ALLOC_UPPER_MEMORY(Bit32u size)
{
	// TODO: ñ{ÉYÉåîüêî
	//^098D:116D
	//^098D:1170
	return ALLOC_MEMORY_RAM(size, afUseUpper, 1024);
}

//^098D:0E78
void SkWinCore::COMPRESS_RECTS(i16 *data, Bit32u size, RectTable *zz, Bit8u *(SkWinCore::*allocFunc)(Bit32u size))
{
#define	PTR_ADVANCE(PTR,DELTA) *((Bit8u **)&(PTR)) += (DELTA);
#define	WRITE_BYTE(PTR,VAL) **(Bit8u **)&(PTR) = (VAL);
#define	WRITE_WORD(PTR,VAL) **(Bit16u **)&(PTR) = (VAL);

	// TODO: ñ{ÉYÉåîüêî
	//^098D:0E78
	//^098D:0E7E
	if (*(data++) != (i16)0xfc0d)
		return;
	//^098D:0E8F
	i16 bp0e = *(data++);
	//^098D:0E9C
	i16 *bp08 = data;
	//^098D:0EA8
	data += bp0e << 1;
	//^098D:0EB2
	while (zz->pb0 != NULL) {
		//^098D:0EB4
		zz = zz->pb0;
	}
	//^098D:0ED0
	for (; --bp0e >= 0; ) {
		//^098D:0ED3
		Bit16u bp0a = *bp08; bp08++;
		//^098D:0EE0
		Bit16u bp0c = *bp08; bp08++;
		//^098D:0EED
		i16 si = bp0c - bp0a +1;
		//^098D:0EF3
		i16 *bp16 = data;
		//^098D:0EFF
		Bit8u bp17 = 0x1f;
		//^098D:0F03
		Bit16u bp1a = data[0];
		//^098D:0F0C
		Bit16u bp1c = data[1];

		do {
			//^098D:0F13
			if (*(data++) != (i16)bp1a) {
				//^098D:0F22
				bp17 &= ~2;
			}
			//^098D:0F2A
			if (*(data) != (i16)bp1c) {
				//^098D:0F35
				bp17 &= ~1;
			}
			//^098D:0F3D
			if (*(data++) > 255) {
				//^098D:0F4B
				bp17 &= ~4;
			}
			//^098D:0F53
			Bit16u bp1e = 2;

			do {
				//^098D:0F58
				i16 di = *(data++);
				//^098D:0F64
				if (di < 0 || di > 255) {
					//^098D:0F6E
					bp17 &= ~16;
				}
				//^098D:0F76
				if (di < -128 || di > 127) {
					//^098D:0F80
					bp17 &= ~8;
				}
				//^098D:0F88
			} while (--bp1e != 0);

			//^098D:0F8D
		} while (--si > 0);

		//^098D:0F90
		if ((bp17 & 3) != 0) {
			//^098D:0F96
			bp17 &= ~4;
		}
		//^098D:0F9E
		si = bp0c - bp0a +1;
		//^098D:0FA7
		Bit32u bp12 = CALC_SIZE_OF_COMPRESSED_RECT(bp17) * si + 10;
		//^098D:0FC3
		if ((bp17 & 1) != 0) {
			//^098D:0FC9
			bp12 += 2;
		}
		//^098D:0FD1
		RectTable *bp04 = reinterpret_cast<RectTable *>((this->*allocFunc)(bp12));
		//^098D:0FE2
		zz = (zz->pb0 = bp04);
		//^098D:0FF8
		bp04->pb0 = NULL;
		//^098D:1006
		bp04->w4 = bp0a;
		//^098D:100D
		bp04->w6 = bp0c;
		//^098D:1014
		bp04->b8 = (Bit8u)bp17;
		//^098D:101B
		bp04->b9 = (Bit8u)bp1a;
		SkD((DLV_RCT, "RCT: COMPRESS_RECTS (%p,(%d,%d,%d,%d))\n"
			, (Bitu)bp04->pb0, (Bitu)bp04->w4, (Bitu)bp04->w6, (Bitu)bp04->b8, (Bitu)bp04->b9));
		//^098D:1022
		bp04++;
		//^098D:1026
		if ((bp17 & 1) != 0) {
			//^098D:102C
			WRITE_WORD(bp04,bp1c);
			//^098D:1035
			PTR_ADVANCE(bp04,+2);
		}
		//^098D:1043
		data = bp16;

		do {
			//^098D:104F
			if ((bp17 & 4) != 0) {
				//^098D:1055
				WRITE_BYTE(bp04,(Bit8u)*data); data++;
				//^098D:1065
				PTR_ADVANCE(bp04,+1);
				//^098D:1072
				WRITE_BYTE(bp04,(Bit8u)*data); data++;
				//^098D:1082
				PTR_ADVANCE(bp04,+1);
			}
			else {
				//^098D:1091
				if ((bp17 & 2) == 0) {
					//^098D:109C
					WRITE_WORD(bp04,*data);
					//^098D:10A8
					PTR_ADVANCE(bp04,+2);
				}
				//^098D:10B6
				data++;
				//^098D:10BA
				if ((bp17 & 1) == 0) {
					//^098D:10C5
					WRITE_WORD(bp04,*data);
					//^098D:10D1
					PTR_ADVANCE(bp04,+2);
				}
				//^098D:10DF
				data++;
			}
			//^098D:10E3
			if ((bp17 & 24) != 0) {
				//^098D:10E9
				WRITE_BYTE(bp04,(Bit8u)*data); data++;
				//^098D:10F9
				PTR_ADVANCE(bp04,+1);
				//^098D:1106
				WRITE_BYTE(bp04,(Bit8u)*data); data++;
				//^098D:1116
				PTR_ADVANCE(bp04,+1);
			}
			else {
				//^098D:111F
				WRITE_WORD(bp04,*data); data++;
				//^098D:112F
				PTR_ADVANCE(bp04,+2);
				//^098D:113D
				WRITE_WORD(bp04,*data); data++;
				//^098D:114D
				PTR_ADVANCE(bp04,+2);
			}

			//^098D:115B
		} while (--si > 0);

		//^098D:1161
	}
	//^098D:1169
	return;

#undef	PTR_ADVANCE
#undef	WRITE_BYTE
#undef	WRITE_WORD
}

//^098D:030F
SRECT *SkWinCore::QUERY_RECT(RectTable *entry, Bit16u rectno) //#DS=4976?
{
#define	PTR_ADVANCE(PTR,DELTA) *((Bit8u **)&(PTR)) += (DELTA);
#define	READ_BYTE(PTR) **((Bit8u **)&(PTR))
#define	READ_SBYTE(PTR) **((i8 **)&(PTR))
#define	READ_WORD(PTR) **((Bit16u **)&(PTR))

	// TODO: íPëÃìÆçÏämîFçœ 13:59 2006/05/07
	//^098D:030F
	//^098D:0315
	Bit16u iRequestedRectNo = rectno;	// Bit16u si
	//^098D:0318
	if (iRequestedRectNo == 0)
		return NULL;
	//^098D:031F
	for (; entry != NULL; entry = entry->pb0) {
		//^098D:0322
		if (entry->w4 > iRequestedRectNo || entry->w6 < iRequestedRectNo)
			continue;
		//^098D:0337
		iRequestedRectNo -= entry->w4;
		//^098D:033B
		_4976_0198++;
		//^098D:033F
		if (_4976_0198 >= 4) {
			//^098D:
			_4976_0198 = 0;
		}
		//^098D:034D
		sk4b84 *bp04 = &_4976_4b84[_4976_0198];
		//^098D:035C
		Bit8u bp05 = entry->b8;

		// bp05 combinations:
		// 0x00		@prefix @entry (x=RW, y=RW)
		// 0x01		@prefix (y=w10) @entry

		//^098D:0366
		if ((bp05 & 2) != 0) {
			//^098D:036A
			bp04->w0 = entry->b9;
		}
		//^098D:0376
		entry++;
		//^098D:037A
		if ((bp05 & 1) != 0) {
			//^098D:0380
			bp04->w2 = READ_WORD(entry);
			//^098D:038F
			PTR_ADVANCE(entry,+2);
		}
		//^098D:039D
		PTR_ADVANCE(entry,+(CALC_SIZE_OF_COMPRESSED_RECT(bp05) * iRequestedRectNo));
		//^098D:03B8
		if ((bp05 & 4) != 0) {
			//^098D:03BE
			bp04->w0 = READ_BYTE(entry);
			//^098D:03CC
			PTR_ADVANCE(entry,+1);
			//^098D:03D9
			bp04->w2 = READ_BYTE(entry);
			//^098D:03E7
			PTR_ADVANCE(entry,+1);
			//^098D:03EE
		}
		else {
			//^098D:03F0
			if ((bp05 & 2) == 0) {
				//^098D:03FB
				bp04->w0 = READ_WORD(entry);
				//^098D:0407
				PTR_ADVANCE(entry,+2);
			}
			//^098D:0415
			if ((bp05 & 1) == 0) {
				//^098D:0420
				bp04->w2 = READ_WORD(entry);
				//^098D:042F
				PTR_ADVANCE(entry,+2);
			}
		}
		//^098D:043D
		if ((bp05 & 8) != 0) {
			//^098D:0443
			bp04->w4 = READ_SBYTE(entry); PTR_ADVANCE(entry,+1);
			//^098D:0453
			bp04->w6 = READ_SBYTE(entry);
			//^098D:045B
		}
		//^098D:045D
		else if ((bp05 & 16) != 0) {
			//^098D:0463
			bp04->w4 = READ_BYTE(entry); PTR_ADVANCE(entry,+1);
			//^098D:0474
			bp04->w6 = READ_BYTE(entry);
			//^098D:047D
		}
		else {
			//^098D:047F
			bp04->w4 = READ_WORD(entry); PTR_ADVANCE(entry,+2);
			//^098D:048E
			bp04->w6 = READ_WORD(entry);
		}

		SkD((DLV_RCT, "RCT: QUERY_RECT(%4d,(%3d,%3d,%3d,%3d))\n", (Bitu)rectno, (Bitu)bp04->w0, (Bitu)bp04->w2, (Bitu)bp04->w4, (Bitu)bp04->w6));

		//^098D:0495
		return (SRECT *)bp04;

		//^098D:04A4
	}
	//^098D:04BF
	return NULL;

#undef	PTR_ADVANCE
#undef	READ_BYTE
#undef	READ_SBYTE
#undef	READ_WORD
}

//^098D:0048
SRECT *SkWinCore::SET_SRECT(SRECT *prc, Bit16u x, Bit16u y, Bit16u cx, Bit16u cy)
{
	//^098D:0048
	//^098D:004B
	prc->x = x;
	prc->y = y;
	prc->cx = cx;
	prc->cy = cy;
	//^098D:0069
	return prc;
}

//^098D:0126
SRECT *SkWinCore::UNION_RECT(SRECT *rc1, const SRECT *rc2, i16 *offx, i16 *offy)
{
	// it modifies rc1 to represent union area.
	// returns non-null if 2 rects have union area.

	// it is to clip the rc1 within rc2.
	// you are to draw picture at top-left of rc1.

	// rc1
	// +------+
	// |      |
	// |    +------+ rc2
	// |    |*|    |
	// +----|-+    |
	//      |      |
	//      +------+
	// 
	// in this case, (offx,offy) is at (5, 2).

	// rc2
	// +------+
	// |      |
	// |    +------+ rc1
	// |    |*|    |
	// +----|-+    |
	//      |      |
	//      +------+
	// 
	// in this case, (offx,offy) is at (0, 0).

	//^098D:0126
	i16 si = rc2->x - rc1->x;
	//^098D:0139
	if (si > 0) {
		//^098D:013D
		*offx = si;
		//^098D:0143
		rc1->x += si;
		rc1->cx -= si;
	}
	else {
		//^098D:014F
		*offx = 0;
	}
	//^098D:0157
	si = rc2->y - rc1->y;
	//^098D:0167
	if (si > 0) {
		//^098D:016B
		*offy = si;
		//^098D:0171
		rc1->y += si;
		rc1->cy -= si;
	}
	else {
		//^098D:017E
		*offy = 0;
	}
	//^098D:0186
	si = (rc1->x +rc1->cx -1) - (rc2->x +rc2->cx -1);
	//^098D:01AC
	if (si > 0) {
		//^098D:01B2
		rc1->cx -= si;
	}
	//^098D:01B9
	si = (rc1->y +rc1->cy -1) - (rc2->y +rc2->cy -1);
	//^098D:01D5
	if (si > 0) {
		//^098D:01D9
		rc1->cy -= si;
	}
	//^098D:01E0
	if (rc1->cx > 0 && rc1->cy > 0) {
		//^098D:01F1
		return rc1;
	}
	//^098D:01F9
	return NULL;
}

//^098D:0599 
// SPX: ww is the method to know how to start drawing image from ref point (can be ornate position)
// 0 => centered
// 1 => point is corner left/upper (0,0)
// 2 => point is corner right/upper (x,0)
// 3 => point is corner right/bottom (x,y)
// 4 => point is corner left/bottom (0,y)
// 5 => centered from top (image is then moved to lower)
// 6 => centered from right (image is then moved to left)
// 7 => centered from bottom (image is then moved to upper)
// 8 => centered from left (image is then moved to right)
SRECT *SkWinCore::QUERY_BLIT_RECT(Bit8u *buff, SRECT *rect, Bit16u rectno, i16 *yourcx, i16 *yourcy, i16 ww) //#DS=4976?
{
	SkD((DLV_RCT, "RCT: -> QUERY_BLIT_RECT(%p,%p(     x,     x,     x,     x),%4d,%3d,%3d,%3d)\n"
		, buff, rect, (Bitu)(rectno & 0x7fff), (Bitu)*yourcx, (Bitu)*yourcy, (Bitu)ww));

	//^098D:0599
	//^098D:059F
	i16 di;

	Bit16u bp24 = 0;
	SRECT bp1a;
	Bit16u bp12;
	Bit16u bp10;
	i16 bp0c;
	i16 bp0a;
	SRECT *bp08;
	SRECT *bp04;

	//^098D:05A4
	if (rectno == 0xffff) {
		//^098D:05AA
		return NULL;
	}
	//^098D:05B1
	Bit16u si = rectno & 0x8000;
	//^098D:05B9
	if (si != 0) {
		//^098D:05BD
		rectno &= 0x7fff;
	}
	//^098D:05C6
	bp04 = QUERY_RECT(glbRectNoTable.pb0, rectno);
	//^098D:05DE
	if (bp04 == NULL) {
		//^098D:05AA
		return NULL;
	}
	//^098D:05E4
	SET_SRECT(&bp1a, -10000, -10000, 20000, 20000);
	//^098D:05FC
	SRECT bp22;
	Bit16u bp0e;
	if (ww == -1) {
		//^098D:0602
		bp0e = bp04->x;
	}
	else {
		//^098D:060D
		COPY_MEMORY(bp04, &bp22, sizeof(SRECT));
		//^098D:0624
		bp22.x = bp0e = ww;
		//^098D:062D
		bp04 = &bp22;
	}
	//^098D:0636
	if (bp0e <= 8) {
		//^098D:063C
		bp0a = bp04->cx;
		bp0c = bp04->cy;
	}
	else {
		//^098D:064F
		if (bp0e == 9) {
			//^098D:05AA
			return NULL;
		}
		//^098D:0658
		bp0e -= 10;
		bp0a = 0;
		bp0c = 0;
	}
	//^098D:0666
	if (si != 0) {
		//^098D:066A
		bp0a += *yourcx;
		bp0c += *yourcy;
		*yourcx = 0;
		*yourcy = 0;
	}
	//^098D:068C
	if (buff == NULL && (*yourcx <= 0 || *yourcy <= 0)) {
		//^098D:06A6
		//^098D:05AA
		return NULL;
	}
	//^098D:06A9
	bp10 = 0;
	//^098D:06AE
	for (; bp04->y != 0; ) {
		//^098D:06B1
		if (bp04->x >= 10 && bp04->x <= 18) {
			//^098D:06C6
			bp08 = QUERY_RECT(glbRectNoTable.pb0, bp04->y);
			//^098D:06DF
			if (bp08 == NULL)
				break;
			//^098D:06E6
			si = bp08->cx;
			di = bp08->cy;
			bp12 = bp08->x;
			bp24 = bp08->y;
			//^098D:06FE
			bp08 = QUERY_RECT(glbRectNoTable.pb0, bp08->y);
			//^098D:0717
			if (bp08 == NULL)
				break;
			//^098D:071E
			switch (bp12) {
				default:
					//^098D:072D
					//^098D:05AA
					return NULL;

				case 0:
					//^098D:0730
					di -= (bp08->cy +1) >> 1;
					//^098D:073C
					si -= (bp08->cx +1) >> 1;
					//^098D:0748
					break;

				case 5:
					//^098D:073C
					si -= (bp08->cx +1) >> 1;
					//^098D:0748
					break;

				case 3:
					//^098D:074A
					di -= bp08->cy -1;
					//^098D:0754
					si -= bp08->cx -1;
					//^098D:075C
					//^098D:0746
					break;

				case 2:
					//^098D:0754
					si -= bp08->cx -1;
					//^098D:075C
					//^098D:0746
					break;

				case 6:
					//^098D:075E
					si -= bp08->cx -1;
					//^098D:0768
					di -= (bp08->cy +1) >> 1;
					//^098D:0772
					break;

				case 8:
					//^098D:0768
					di -= (bp08->cy +1) >> 1;
					//^098D:0772
					break;

				case 7:
					//^098D:0774
					si -= (bp08->cx +1) >> 1;
					//^098D:0780
					di -= bp08->cy -1;
					//^098D:0788
					break;

				case 4:
					//^098D:0780
					di -= bp08->cy -1;
					//^098D:0788
					break;

				case 1:
					//^098D:078A
					break;
			}

			//^098D:078A
			bp1a.x += si;
			//^098D:078D
			if (bp1a.x < si) {
				//^098D:0794
				bp1a.x = si;
			}
			//^098D:0797
			if (bp08->cx + si <= bp1a.x + bp1a.cx -1) {
				//^098D:07AB
				bp1a.cx = bp08->cx - bp1a.x + si;
			}
			//^098D:07B7
			bp1a.y += di;
			//^098D:07BA
			if (bp1a.y < di) {
				//^098D:07C1
				bp1a.y = di;
			}
			//^098D:07C4
			if (bp08->cy + di <= bp1a.y + bp1a.cy -1) {
				//^098D:07D8
				bp1a.cy = bp08->cy - bp1a.y + di;
			}
			//^098D:07E4
			switch (bp04->x) {
				default:
					//^098D:07F9
					//^098D:05AA
					return NULL;

				case 10:
					//^098D:07FC
					di += (bp08->cy +1) >> 1;
					//^098D:0808
					si += (bp08->cx +1) >> 1;
					//^098D:0814
					break;

				case 15:
					//^098D:0808
					si += (bp08->cx +1) >> 1;
					//^098D:0814
					break;

				case 13:
					//^098D:0816
					di += bp08->cy -1;
					//^098D:0820
					si += bp08->cx -1;
					//^098D:0814
					break;

				case 12:
					//^098D:0820
					si += bp08->cx -1;
					//^098D:0814
					break;

				case 16:
					//^098D:082A
					si += bp08->cx -1;
					//^098D:0834
					di += (bp08->cy +1) >> 1;
					//^098D:083E
					break;

				case 18:
					//^098D:0834
					di += (bp08->cy +1) >> 1;
					//^098D:083E
					break;

				case 17:
					//^098D:0840
					si += (bp08->cx +1) >> 1;
					//^098D:084C
					di += bp08->cy -1;

					break;

				case 14:
					//^098D:084C
					di += bp08->cy -1;

					break;

				case 11:
					//^098D:0856
					break;
			}
			//^098D:0856
			bp0a += si + bp04->cx;
			//^098D:0862
			bp0c += di + bp04->cy;
			//^098D:086B
		}
		else {
			//^098D:086E
			bp08 = QUERY_RECT(glbRectNoTable.pb0, bp04->y);
			//^098D:088A
			if (bp08 == NULL)
				break;
			//^098D:0891
			bp24 = bp04->y;
			si = bp08->cx;
			di = bp08->cy;
			//^098D:08A6
			if (bp08->x == 1) {
				//^098D:08AC
				bp0a += si;
				bp0c += di;
				bp1a.x += si;
				bp1a.y += di;
			}
			//^098D:08BB
			else if (bp08->x == 9) {
				//^098D:08C7
				switch (bp04->x) {
					case 0:
						//^098D:08DC
						si = bp04->cx - ((si +1) >> 1);
						//^098D:08EC
						//^098D:0960
						di = bp04->cy - ((di +1) >> 1);
						break;

					case 1:
						//^098D:08EE
						si = bp04->cx;
						//^098D:08F5
						di = bp04->cy;
						break;

					case 2:
						//^098D:08FE
						si = bp04->cx - (si -1);
						//^098D:0908
						//^098D:0939
						//^098D:08F5
						di = bp04->cy;
						break;

					case 3:
						//^098D:090A
						si = bp04->cx - (si -1);
						//^098D:0918
						di = bp04->cy - (di -1);
						//^098D:0922
						//^098D:096C
						break;

					case 4:
						//^098D:0924
						si = bp04->cx;
						//^098D:092B
						//^098D:0918
						di = bp04->cy - (di -1);
						//^098D:0922
						//^098D:096C
						break;

					case 5:
						//^098D:092D
						si = bp04->cx - ((si +1) >> 1);
						//^098D:0939
						//^098D:093D
						//^098D:08F5
						di = bp04->cy;
						break;

					case 6:
						//^098D:093F
						si = bp04->cx - (si -1);
						//^098D:0949
						//^098D:08E8
						break;

					case 7:
						//^098D:094B
						si = bp04->cx - ((si +1) >> 1);
						//^098D:0957
						//^098D:0914
						//^098D:0918
						di = bp04->cy - (di -1);
						//^098D:0922
						//^098D:096C
						break;

					case 8:
						//^098D:0959
						si = bp04->cx;
						//^098D:0960
						di = bp04->cy - ((di +1) >> 1);
						break;
				}
				//^098D:0970
				if (bp10 != 0) {
					//^098D:0976
					bp10 = 0;
					bp0a += si;
					bp0c += di;
					bp1a.x += si;
					bp1a.y += di;
				}
				//^098D:0987
				if (bp1a.x < si) {
					//^098D:098C
					bp1a.x = si;
				}
				//^098D:098F
				if (bp08->cx + si <= bp1a.x + bp1a.cx -1) {
					//^098D:09A3
					bp1a.cx = bp08->cx - bp1a.x + si;
				}
				//^098D:09AF
				if (bp1a.y < di) {
					//^098D:09B4
					bp1a.y = di;
				}
				//^098D:09B7
				if (bp08->cy + di <= bp1a.y + bp1a.cy -1) {
					//^098D:09CB
					bp1a.cy = bp08->cy - bp1a.y + di;
				}
			}
			else {
				//^098D:09D9
				if (bp08->x <= 8) {
					//^098D:09E2
					bp10 = 1;
				}
			}
		}
		//^098D:09E7
		bp04 = bp08;
		//^098D:09F3
	}
	//^098D:0A00
	si = *yourcx;
	//^098D:0A08
	if (si == 0) {
		//^098D:0A0C
		si = READ_UI16(buff,-4);
	}
	//^098D:0A13
	di = *yourcy;
	//^098D:0A1B
	if (di == 0) {
		//^098D:0A1F
		di = READ_UI16(buff,-2);
	}
	//^098D:0A26
	switch (bp0e) {
		default:
			//^098D:0A35
			//^098D:05AA
			return NULL;

		case 0:
			//^098D:0A38
			rect->x = bp0a -((si +1) >> 1);
			//^098D:0A48
			//^098D:0AB1
			rect->y = bp0c -((di +1) >> 1);
			break;

		case 1:
			//^098D:0A4A
			rect->x = bp0a;
			//^098D:0A53
			rect->y = bp0c;
			break;

		case 2:
			//^098D:0A5F
			rect->x = bp0a - (si -1);
			//^098D:0A53
			rect->y = bp0c;
			break;

		case 3:
			//^098D:0A64
			rect->x = bp0a - (si -1);
			//^098D:0A72
			rect->y = bp0c - (di -1);
			//^098D:0A7D
			//^098D:0ABE
			break;

		case 4:
			//^098D:0A7F
			rect->x = bp0a;
			//^098D:0A88
			//^098D:0A72
			rect->y = bp0c - (di -1);
			//^098D:0A7D
			//^098D:0ABE
			break;

		case 5:
			//^098D:0A8A
			rect->x = bp0a - ((si +1) >> 1);
			//^098D:0A9A
			//^098D:0A53
			rect->y = bp0c;
			break;

		case 6:
			//^098D:0A9C
			rect->x = bp0a - (si -1);
			//^098D:0A9F
			//^098D:0A3D
			//^098D:0A48
			//^098D:0AB1
			rect->y = bp0c - ((di +1) >> 1);
			break;

		case 7:
			//^098D:0AA1
			rect->x = bp0a - ((si +1) >> 1);
			//^098D:0A72
			rect->y = bp0c - (di -1);
			//^098D:0A7D
			//^098D:0ABE
			break;

		case 8:
			//^098D:0AA8
			rect->x = bp0a;
			//^098D:0AB1
			rect->y = bp0c - ((di +1) >> 1);
			break;
	}
	//^098D:0AC2
	if (_4976_4bca.cx != 0) {
		//^098D:0AC9
		COPY_MEMORY(&_4976_4bca, &bp1a, sizeof(SRECT));
	}
	//^098D:0ADE
	if (_4976_4bc8 != 0 && glbIsPlayerMoving != 0 && bp24 == 3) {
		//^098D:0AF2
		if (UNION_RECT(&bp1a, &_4976_00fe, yourcx, yourcy) == NULL) {
			//^098D:05AA
			return NULL;
		}
	}
	//^098D:0B15
	bp0a = bp1a.x - rect->x;
	//^098D:0B21
	if (bp0a <= 0) {
		//^098D:0B25
		*yourcx = 0;
		//^098D:0B33
		rect->cx = min_value(si, bp1a.cx + bp0a);
	}
	else {
		//^098D:0B37
		*yourcx = bp0a;
		//^098D:0B40
		rect->x = bp1a.x;
		//^098D:0B49
		rect->cx = min_value(si - bp0a, bp1a.cx);
	}
	//^098D:0B60
	bp0c = bp1a.y - rect->y;
	//^098D:0B6D
	if (bp0c <= 0) {
		//^098D:0B71
		*yourcy = 0;
		//^098D:0B79
		rect->cy = min_value(di, bp1a.cy + bp0c);
	}
	else {
		//^098D:0B83
		*yourcy = bp0c;
		//^098D:0B8C
		rect->y = bp1a.y;
		//^098D:0B96
		rect->cy = min_value(di - bp0c, bp1a.cy);
	}
	//^098D:0BAD
	if (rect->cx <= 0 || rect->cy <= 0)
		return NULL;
	//^098D:0BC1

	SkD((DLV_RCT, "RCT: <- QUERY_BLIT_RECT(%p,%p(%6d,%6d,%6d,%6d),%4d,%3d,%3d,%3d)\n"
		, buff, rect, (Bitu)rect->x, (Bitu)rect->y, (Bitu)rect->cx, (Bitu)rect->cy
		, (Bitu)rectno, (Bitu)*yourcx, (Bitu)*yourcy, (Bitu)ww));

	return rect;
}

//^0B36:16E4
void SkWinCore::DRAW_GAMELOAD_DIALOGUE_TO_SCREEN(Bit8u *buffsrc, Bit16u rectno, i16 colorkey, Bit8u localpal[16])
{
	//^0B36:16E4
	//^0B36:16E8
	i16 bp02 = 0;
	i16 bp04 = 0;
	SRECT bp0c;
	//^0B36:16F2
	if (QUERY_BLIT_RECT(buffsrc, &bp0c, rectno, &bp02, &bp04, -1) != NULL) {
		//^0B36:1718
		FIRE_BLIT_PICTURE(
			buffsrc,
			_4976_4964,
			&bp0c,
			bp02,
			bp04,
			*(Bit16u *)&buffsrc[-4],
			glbScreenWidth,
			colorkey,
			0,
			*(Bit16u *)&buffsrc[-6],
			8,
			localpal
			);
	}
	//^0B36:1755
	return;
}

//^01B0:0D39
void SkWinCore::_01b0_0d39(i16 *xx, i16 *yy, i16 *zz, Bit16u ww) //#DS=04BF
{
	//^01B0:0D39
	ENTER(0);
	//^01B0:0D3C
	LOADDS(0x3083);
	//^01B0:0D42
	if (ww != 0) {
		//^01B0:0D4A
		*xx = _04bf_185e;
		//^01B0:0D53
		*yy = _04bf_1860;
		//^01B0:0D5C
		*zz = _04bf_1936;
	}
	else {
		//^01B0:0D64
		*xx = glbMouseXPos;
		//^01B0:0D6D
		*yy = glbMouseYPos;
		//^01B0:0D76
		*zz = glbMouseButtonState;
	}
	//^01B0:0D7F
	//^01B0:0D81
	return;
}

//^01B0:0008
void SkWinCore::MOVE_MEMORY(const void *buffSrc, void *buffDst, Bit16u size)
{
	memmove(buffDst, buffSrc, size);
}

//^00EB:01EB
void SkWinCore::MOVE_MEMORY_WITH_DELTA(Bit16u offSrc, Bit16u offDst, Bit16u size) //#DS=04BF
{
	//^00EB:01EB
	//^00EB:01EE
	MOVE_MEMORY(
		_04bf_0cf0 +offSrc,
		_04bf_0e34 +offDst,
		size
		);
	//^00EB:021F
	return;
}

//^00EB:0221
void SkWinCore::COPY_MEMORY_WITH_DELTA_AND_TRANSPARENT(Bit16u offSrc, Bit16u offDst, Bit16u size, Bit8u colorkey) //#DS=04BF
{
	//^00EB:0221
	//^00EB:0224
	Bit8u dh = colorkey;
	Bit8u *di = _04bf_0e34 + offDst;
	Bit16u cx = size;
	Bit8u *si = _04bf_0cf0 + offSrc;

	do {
		//^00EB:0242
		Bit8u al = *(si++);
		//^00EB:0243
		if (al != colorkey) {
			//^00EB:0247;
			*(di++) = al;
		}
		else {
			//^00EB:024C
			di++;
		}
		//^00EB:024D
	} while (--cx != 0);
	//^00EB:024F
	return;
}

//^00EB:0812
void SkWinCore::MOVE_MEMORY_STRETCH_13TO16(Bit16u offSrc, Bit16u offDst)
{
	// compose 0.5 stepping viewport. (123% larger)

	//^00EB:0812
	//^00EB:0817
	Bit8u *di = _04bf_0e34 +offDst;
	Bit8u *si = _04bf_0cf0 +offSrc;
	Bit16u cx = 14;

	// DS:SI	E000:871B -> E000:8728 (+13)
	// ES:DI	A000:3340 -> A000:3350 (+16)

	// [DS:SI] in  E000:8728  A4 A7 A2 A7 AB AB A6 A9 A9 A9 A9 A9 A8
	// [ES:DI] out A000:3350  A4 A7 A2 A2 A7 AB AB A6 A9 A9 A9 A9 A9 A9 A9 A8

	// [DS:SI] in  E000:87FB  00 01 02 03 04 05 06 07 08 09 0A 0B 0C
	// [ES:DI] out A000:3480  00 01 02 02 03 04 05 06 07 07 08 09 0A 0B 0B 0C

	do {
		//^00EB:082C
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; 
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; 
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; si++;
		*di = *si; di++; 
		*di = *si; di++; si++;
		//^00EB:083E
	} while (--cx != 0);
	//^00EB:0840
	return;
}

//^00EB:0845
void SkWinCore::_00eb_0845(Bit8u *buff, SRECT *rc, Bit16u ww) //#DS=04BF
{
	//^00EB:0845
	//^00EB:084B
	LOADDS(0x0c48);
	//^00EB:0851
	Bit16u bp12 = ((ww & 0x8000) != 0) ? 1 : 0;
	//^00EB:0862
	LOCK_MOUSE_EVENT();
	//^00EB:0867
	if (true
		&& glbMouseCursorVisible != 0
		&&                           rc->y <= _04bf_09e0.y + _04bf_09e0.cy -1
		&&               rc->y + rc->cy -1 >= _04bf_09e0.y
		&&               rc->x + rc->cx -1 >= _04bf_09e0.x
		&& _04bf_09e0.x + _04bf_09e0.cx -1 >= rc->x
	) {
		//^00EB:08B0
		//^00EB:092C
		SRECT bp10;
		bp10.y = max(rc->y, _04bf_09e0.y);
		//^00EB:0948
		bp10.x = max(rc->x, _04bf_09e0.x);
		//^00EB:0962
		bp10.cx = min(rc->x + rc->cx -1, _04bf_09e0.x + _04bf_09e0.cx -1) -bp10.x +1;
		//^00EB:099D
		bp10.cy = min(rc->y + rc->cy -1, _04bf_09e0.y + _04bf_09e0.cy -1) -bp10.y +1;
		//^00EB:09D2
		i16 bp06 = bp10.x - rc->x;
		i16 bp08 = bp10.y - rc->y;
		//^00EB:09E8
		bp10.y -= _04bf_09e0.y;
		bp10.x -= _04bf_09e0.x;
		//^00EB:09F4
		if (bp12 != 0) {
			//^00EB:09FA
			IBMIO_BLIT_TO_SCREEN_8TO8BPP(
				buff,
				_04bf_079e,
				&bp10,
				bp06,
				bp08,
				rc->cx,
				24,
				-1
				);
		}
		//^00EB:0A1E
		Bit16u si = rc->y * 320 + rc->x;
		Bit16u bp02 = 0;
		Bit16u di = 0;
		Bit16u bp04 = rc->y;
		//^00EB:0A3D
		for (; rc->y + rc->cy -1 >= bp04; di += 103, bp02 += rc->cx, si += 320, bp04++) {
			//^00EB:0A40
			if (bp04 >= _04bf_09e0.y && _04bf_09e0.y + _04bf_09e0.cy -1 >= bp04) {
				//^00EB:0A5C
				_04bf_0cf0 = buff;
				_04bf_0e34 = _04bf_0cf4;
				//^00EB:0A73
				if (bp12 != 0) {
					//^00EB:0A79
					MOVE_MEMORY_STRETCH_13TO16(
						(((di >> 7) +8) *224) +21,
						0
						);
				}
				else {
					//^00EB:0A94
					MOVE_MEMORY_WITH_DELTA(bp02, 0, rc->cx);
				}
				//^00EB:0AA7
				_04bf_0cf0 = _04bf_0cf4;
				_04bf_0e34 = _04bf_079e;
				//^00EB:0ABB
				MOVE_MEMORY_WITH_DELTA(_04bf_09e0.x, ((bp04 - _04bf_09e0.y) * 24) +bp10.x, bp10.cx);
				//^00EB:0AD9
				_04bf_0cf0 = _04bf_0e80[_04bf_1938].b6;
				_04bf_0e34 = _04bf_0cf4;
				//^00EB:0AF5
				COPY_MEMORY_WITH_DELTA_AND_TRANSPARENT(
					(bp04 - _04bf_09e0.y) * (_04bf_0e80[_04bf_1938].b2) + bp10.x,
					bp06,
					bp10.cx,
					(Bit8u)_04bf_079c
					);
				//^00EB:0B25
				_04bf_0cf0 = _04bf_0cf4;
				_04bf_0e34 = pbVram;
				//^00EB:0B3B
				//^00EB:0B8B
				MOVE_MEMORY_WITH_DELTA(0, si, rc->cx);
			}
			else {
				//^00EB:0B47
				_04bf_0cf0 = buff;
				_04bf_0e34 = pbVram;
				//^00EB:0B60
				if (bp12 != 0) {
					//^00EB:0B66
					MOVE_MEMORY_STRETCH_13TO16((((di >> 7) + 8) * 224) + 21, si);
				}
				else {
					//^00EB:0B80
					MOVE_MEMORY_WITH_DELTA(bp02, si, rc->cx);
				}
			}
			//^00EB:0B92
		}
	}
	else {
		//^00EB:08B3
		if (bp12 != 0) {
			//^00EB:08B9
			_04bf_0e34 = (Bit8u *)__vram;
			_04bf_0cf0 = buff;
			//^00EB:08D2
			Bit16u di = 0;
			Bit16u si = 320*40;
			//^00EB:08D7
			for (; si < 320*176; di += 103, si += 320) {
				//^00EB:08D9
				MOVE_MEMORY_STRETCH_13TO16(
					((di >> 7) +8) *224 +21,
					si
					);
				//^00EB:08F1
			}
		}
		else {
			//^00EB:0901
			IBMIO_BLIT_TO_SCREEN_8TO8BPP(
				buff,
				__vram,
				rc,
				0,
				0,
				rc->cx,
				320,
				-1
				);
		}
	}
#if UseAltic
	skwin.UpdateRect(0, 40, 224, 136);
#endif

	//^00EB:0BBA
	UNLOCK_MOUSE_EVENT();
	//^00EB:0BBF
	return;
}

//^098D:0D3F
SRECT *SkWinCore::SCALE_RECT(Bit16u rectno, SRECT *rc, Bit16u horzResolution, Bit16u vertResolution)
{
	// horzResolution, vertResolution: 10,000 is fair. 5,000 is half. 20,000 is double.

	//^098D:0D3F
	//^098D:0D45
	Bit16u di = horzResolution;
	Bit16u si = vertResolution;
	//^098D:0D4B
	SRECT *bp04 = QUERY_RECT(&glbRectNoTable, rectno);
	//^098D:0D63
	if (bp04 != NULL) {
		//^098D:0D6A
		if (bp04->y != 0) {
			//^098D:0D77
			bp04 = QUERY_RECT(&glbRectNoTable, bp04->y);
			//^098D:0D93
			if (bp04 != NULL) {
				//^098D:0D9A
				if (bp04->x == 9) {
					//^098D:0DA6
					i16 bp06 = (di == 10000) ? (bp04->cx) : (i16)((i32(bp04->cx) * di) / 10000);
					//^098D:0DD6
					i16 bp08 = (si == 10000) ? (bp04->cy) : (i16)((i32(bp04->cy) * si) / 10000);
					//^098D:0E09
					if (bp06 == 0 && di != 0) {
						//^098D:0E13
						bp06 = 1;
					}
					//^098D:0E18
					if (bp08 == 0 && si != 0) {
						//^098D:0E22
						bp08 = 1;
					}
					//^098D:0E27
					if (bp06 > 0 && bp08 > 0) {
						//^098D:0E33
						return QUERY_BLIT_RECT(NULL, rc, rectno, &bp06, &bp08, -1);
					}
				}
			}
		}
	}
	//^098D:0E55
	return NULL;
}

//^098D:0E5D
SRECT *SkWinCore::QUERY_EXPANDED_RECT(Bit16u rectno, SRECT *rc)
{
	//^098D:0E5D
	//^098D:0E60
	return SCALE_RECT(rectno, rc, 10000, 10000);
}

//^44C8:1BE8
// SPX: _44c8_1be8 renamed CHANGE_VIEWPORT_TO_INVENTORY
void SkWinCore::CHANGE_VIEWPORT_TO_INVENTORY(Bit16u xx) //#DS=4976
{
	//^44C8:1BE8
	//^44C8:1BEE
	Bit16u di = glbIsPlayerMoving;
	glbIsPlayerMoving = 0;
	//^44C8:1BF8
	SRECT bp0e;
	// SPX: add value init for rect
	bp0e.x = 0;
	bp0e.y = 0;
	bp0e.cx = 0;
	bp0e.cy = 0;
	QUERY_EXPANDED_RECT(7, &bp0e);
	//^44C8:1C07
	glbIsPlayerMoving = di;
	//^44C8:1C0B
	Bit16u si;
	if (glbPaletteIRGBLoaded == 0) {
		//^44C8:1C12
		i16 bp02, bp04, bp06;
		//_01b0_0d39(&bp02, &bp04, &bp06, 1) CALL_IBMIO;
		//^44C8:1C2E
		if (bp0e.x +bp0e.cx -1 < bp02 || bp0e.y +bp0e.cy -1 < bp04 || bp04 +bp06 < bp0e.y) {
			//^44C8:1C51
			si = 0;
		}
		else {
			//^44C8:1C55
			FIRE_HIDE_MOUSE_CURSOR();
			si = 1;
		}
	}
	//^44C8:1C5D
	_00eb_0845(_4976_4c16, &bp0e, (glbIsPlayerMoving != 0) ? 0x8008 : 0x0008) CALL_IBMIO;
	//^44C8:1C85
	if (glbPaletteIRGBLoaded == 0 && si != 0) {
		//^44C8:1C90
		FIRE_SHOW_MOUSE_CURSOR();
	}
	//^44C8:1C95
	return;
}

//^0AAF:002F
void SkWinCore::_0aaf_002f()
{
	//^0AAF:002F
	//^0AAF:0032
	if (glbGameHasEnded != 0) {
		//^0AAF:0039
		FIRE_HIDE_MOUSE_CURSOR();
		//^0AAF:003E
		DRAW_GAMELOAD_DIALOGUE_TO_SCREEN(_4976_4c16, 5, -1, NULL);
		//^0AAF:0056
		FIRE_SHOW_MOUSE_CURSOR();
	}
	else {
		//^0AAF:005D
		CHANGE_VIEWPORT_TO_INVENTORY(0);
	}
}

//^0CD5:00A0
Bit16u SkWinCore::max_value(i16 v1, i16 v2) {
	//^0CD5:00A0
	//^0CD5:00A3
	if (v1 > v2)
		return v1;
	return v2;
}

//^0CD5:008E
Bit16u SkWinCore::min_value(i16 v1, i16 v2) {
	//^0CD5:008E
	//^0CD5:0091
	if (v1 < v2)
		return v1;
	return v2;
}

//^44C8:1BA2
void SkWinCore::FIRE_WAIT_VSYNC()
{
	//^44C8:1BA2
	//^44C8:1BA5
	IBMIO_WAIT_VSYNC() CALL_IBMIO;
}

//^0CD5:000E
void SkWinCore::WAIT_SCREEN_REFRESH()
{
	//^0CD5:000E
	FIRE_WAIT_VSYNC();
}

//^3E74:178C
//RawEntry *SkWinCore::QUERY_GDAT_ENTRYPTR(Bit8u cls1, Bit16u cls2, Bit8u cls3, Bit16u cls4)
RawEntry *SkWinCore::QUERY_GDAT_ENTRYPTR(Bit8u iCategory, Bit16u iItem, Bit8u iType, Bit16u iEntry)
{
	// cls1 - main category	-> iCategory
	// cls2 - sub category	-> iItem
	// cls3 - type			-> iType
	// cls4 - name			-> iEntry

	//^3E74:178C
	// If requested category is above max category
	if (iCategory > U8(glbGDatEntries.w12)) {
		//^3E74:179D
		return NULL;
	}
	Bit16u si = glbGDatEntries.pw0[iCategory];
	if (glbGDatEntries.pw0[iCategory +1] - si < iType) {
		//^3E74:17D2
		return NULL;
	}
	//^3E74:17D2
	si += iType;
	Bit16u di = glbGDatEntries.pw4[si];
//		3E74:17E6  8BF8                 mov  di,ax
//		3E74:17E8  33D2                 xor  dx,dx							DX:AX=0000:1399
//		3E74:17EA  05FFFF               add  ax,FFFF
//		3E74:17ED  83D2FF               adc  dx,FFFF						DX:AX=0000:1398
//		3E74:17F0  B102                 mov  cl,02
//		3E74:17F2  9A85099A06           call 069A:0985		call SHL32		DX:AX=0000:4E60
//		3E74:17F7  50                   push ax
//		3E74:17F8  52                   push dx
//		3E74:17F9  8B16C05C             mov  dx,[5CC0]
//		3E74:17FD  A1BE5C               mov  ax,[5CBE]						DX:AX=9283:0006
//		3E74:1800  59                   pop  cx
//		3E74:1801  5B                   pop  bx								CX:BX=0000:4E60
//		3E74:1802  9A6F0A9A06           call 069A:0A6F		call ADD_TO_FAR_PTR
//		3E74:1807  8946F8               mov  [bp-08],ax						DX:AX=9769:0006
//		3E74:180A  8956FA               mov  [bp-06],dx

//9283:0006 + 00703C -> 99872
//9987:0002 + 000030 -> 998A2

	RawEntry *bp08 = &glbGDatEntries.pv8[di -1];
	//^3E74:180D
	di = glbGDatEntries.pw4[si +1] -di +1;
	//^3E74:1820
	si = 0;
	while (true) {
		//^3E74:1822
		Bit16u bp0a = (si +di) / 2;
		//^3E74:182B
		if (!(bp0a != si))
			return NULL;
		//^3E74:1833
		RawEntry *bp04 = &bp08[bp0a];
		i16 bp0c = bp04->cls2 - iItem;
		//^3E74:1866
		if (!(bp0c != 0)) {
			//^3E74:186A
			bp0c = bp04->cls4 - iEntry;
			if (!(bp0c != 0)) {
				//^3E74:187E
				return bp04;
			}
		}
		//^3E74:1887
		if (!(bp0c <= 0)) {
			//^3E74:188D
			di = bp0a;
		}
		else {
			//^3E74:1892
			si = bp0a;
		}
	}
}

//^3E74:1CF3
//Bit16u SkWinCore::QUERY_GDAT_ENTRY_IF_LOADABLE(Bit8u cls1, Bit8u cls2, Bit8u cls3, Bit8u cls4)
Bit16u SkWinCore::QUERY_GDAT_ENTRY_IF_LOADABLE(Bit8u iCategory, Bit8u iItemIndex, Bit8u iDataType, Bit8u iEntryNumber)
{
LOGX(("%40s: C%02d=I%02X=E%02X=T%03d to %08X", "QUERY_GDAT_ENTRY_IF_LOADABLE", iCategory, iItemIndex, iEntryNumber, iDataType ));
	//^3E74:1CF3
	//^3E74:1CF7
	RawEntry *bp04 = QUERY_GDAT_ENTRYPTR(iCategory, iItemIndex, iDataType, iEntryNumber);
	//^3E74:1D14
	if (bp04 != NULL) {
		//^3E74:1D18
		if (false
			|| (iDataType == dtWordValue)
			|| (iEntryNumber == dtImageOffset)
			|| (glbShelfMemoryTable[bp04->data & 0x7fff].Present())
			|| (IS_CLS1_CRITICAL_FOR_LOAD(iCategory) != 0)
		) {
			//^3E74:1D56
			return 1;
		}
	}
	//^3E74:1D5B
	return 0;
}

//^0B36:000A
Bit16u SkWinCore::CALC_IMAGE_BYTE_LENGTH(Bit8u *buff)
{
	//^0B36:000A
	//^0B36:000D
	return ((READ_UI16(buff,-6) == 4) ? (((READ_UI16(buff,-4) +1) & 0xfffe) >> 1) : READ_UI16(buff,-4) & 0xffff) * READ_UI16(buff,-2);
}

//^00EB:03D5
void SkWinCore::IBMIO_WAIT_VSYNC()
{
#if UseAltic
	skwin.Sleep(1000 / 50);
#else
	//^00EB:03D5
	//^00EB:03D8
	while ((inportb(0x03da) & 0x08) != 0);
	//^00EB:03E0
	while ((inportb(0x03da) & 0x08) == 0);
#endif
}

//^00EB:045D
void SkWinCore::IBMIO_UPDATE_PALETTE_SET()
{
	//^00EB:045D
	//^00EB:0463
	IBMIO_WAIT_VSYNC();
	//^00EB:0466
	outportb(0x03c8, 0);
	Bit16u si = 0;
	//^00EB:0470
	for (; si < 256; si++) {
		//^00EB:0472
		Bit8u bp01 = glbPaletteRGB[si][0];
		Bit8u bp02 = glbPaletteRGB[si][1];
		Bit8u bp03 = glbPaletteRGB[si][2];
		//^00EB:04A2
		//bp01 = bp02 = bp03 = ((si%16)+1) << 4;
		//outportb(0x03c8, si);
		outportb(0x03c9, bp01);
		outportb(0x03c9, bp02);
		outportb(0x03c9, bp03);
		//printf("%2X%2X%2X ", bp01, bp02, bp03);
		//^00EB:04B1
	}
	//^00EB:04B8
	//^00EB:04B9
	return;
}

//^00EB:0654
void SkWinCore::IBMIO_SELECT_PALETTE_SET(Bit8u number) //#DS=04BF
{
	//^00EB:0654
	//^00EB:0659
	LOADDS(0x0c48);
	number=1;
	//^00EB:065E
	if (number == 0) {
		//^00EB:0664
		IBMIO_WAIT_VSYNC();
		//^00EB:0668
		//outportb(0x03c8, 0);
		Bit16u si = 0;
		/*
		for (; si < 256; si++) {
			//^00EB:0674
			outportb(0x03c9, (si%16)>>3);
			outportb(0x03c9, (si%16)>>3);
			outportb(0x03c9, (si%16)>>3);
			//^00EB:067C
		}*/
		//^00EB:0683
	}
	//^00EB:0686
	else if (number == 1) {
		//^00EB:068C
		IBMIO_UPDATE_PALETTE_SET();
	}
	//^00EB:0690
	glbUpdatePalette = number;
	//^00EB:0696
	return;
}

//^44C8:1BD4
void SkWinCore::FIRE_SELECT_PALETTE_SET(Bit8u number)
{
	//^44C8:1BD4
	IBMIO_SELECT_PALETTE_SET(number) CALL_IBMIO;
}

//^00EB:0353
void SkWinCore::IBMIO_FILL_SCREEN_LINE(Bit16u offDst, Bit16u fill, Bit16u size) //#DS=04BF?
{
	//^00EB:0353
	//^00EB:0357

	// TODO: Alt impl
	memset(_04bf_0e34 + offDst, fill, size);
}

//^00EB:0383
void SkWinCore::IBMIO_FILL_RECT_SCREEN(SRECT *rc, Bit16u fill) //#DS=04BF
{
	//^00EB:0383
	//^00EB:0389
	LOADDS(0x0c48);
	//^00EB:038E
	_04bf_0e34 = pbVram;
	//^00EB:039A
	Bit16u di = rc->y * 320 + rc->x;
	Bit16u si = 0;
    //^00EB:03AD
	for (; rc->cy > si; di += 320, si++) {
		//^00EB:03AF
		IBMIO_FILL_SCREEN_LINE(di, fill, rc->cx);
		//^00EB:03C2
	}
	//^00EB:03D0
#if UseAltic
	skwin.UpdateRect(rc->x, rc->y, rc->cx, rc->cy);
	MessageLoop(false);
#endif
	return;
}

//^44C8:1233
void SkWinCore::FIRE_FILL_4BPP_PICT_LINE(Bit16u offDst, Bit16u fill, Bit16u size)
{
	ATLASSERT(size != 0);

	//^44C8:1233
	//^44C8:1237
	Bit8u *di = _4976_5e6a;
	Bit16u bx = offDst;
	Bit16u cx = size;
	U8 ah = Bit8u(fill);
	U8 al;
	//^44C8:1245
	bool carry = (bx & 1) ? true : false;
	bx >>= 1;
	di += bx;
	//^44C8:124B
	if (carry) {
		//^44C8:124D
		al = *di;
		al &= 0xf0;
		al |= ah;
		stosb(di, al);
		//^44C8:1255
		cx--;
		if (cx == 0)
			//^44C8:1256
			return;
	}
	//^44C8:1258
	al = ah;
	al <<= 4;
	al |= ah;
	ah = al;
	{
		bool carry = (cx & 1) ? true : false;
		cx >>= 1;
		{
			bool carry = (cx & 1) ? true : false;
			cx >>= 1;
			if (carry) {
				//^44C8:1268
				stosb(di, al);
			}
			while (cx != 0) {
				//^44C8:1269
				stosb(di, al);
				stosb(di, ah);
				cx--;
			}
		}
		if (carry) {
			al = *di;
			al &= 0x0f;
			ah &= 0xf0;
			al |= ah;
			stosb(di, al);
		}
	}
	//^44C8:127B
	return;
}

//^44C8:1A1E
void SkWinCore::FIRE_FILL_RECT_4BPP_PICT(Bit8u *buff, SRECT *rc, Bit16u fill, Bit16u width)
{
	//^44C8:1A1E
	//^44C8:1A23
	_4976_5e6a = buff;
	//^44C8:1A30
	width = (width + 1) & 0xfffe;
	//^44C8:1A3A
	Bit16u di = rc->y * width + rc->x;
	Bit16u si = 0;
	//^44C8:1A4B
	for (; rc->cy > si; di += width, si++) {
		//^44C8:1A4D
		FIRE_FILL_4BPP_PICT_LINE(di, fill, rc->cx);
		//^44C8:1A60
	}
	//^44C8:1A6D
	return;
}

//^44C8:127E
void SkWinCore::FIRE_FILL_8BPP_PICT_LINE(Bit16u offDst, Bit16u fill, Bit16u size)
{
	//^44C8:127E
	//^44C8:1282
	Bit8u *di = _4976_5e6a + offDst;
	Bit16u cx = size;
	Bit8u al = Bit8u(fill);
	//^44C8:1292
	U16 dx = cx;
	dx &= 1;
	if (dx != 0) {
		//^44C8:1299
		stosb(di, al);
		cx--;
	}
	//^44C8:129B
	dx = cx;
	cx >>= 1;
	if (cx != 0) {
		//^44C8:12A1
		U16 ax = al | (al << 8);
		while (cx != 0) {
			stosw(di, ax);
			cx--;
		}
	}
	//^44C8:12A5
	dx &= 1;
	if (dx != 0) {
		//^44C8:12AA
		stosb(di, al);
	}
	//^44C8:12AB
	return;
}

//^44C8:19D5
void SkWinCore::FIRE_FILL_RECT_8BPP_PICT(Bit8u *buff, SRECT *rc, Bit16u fill, Bit16u width)
{
	//^44C8:19D5
	//^44C8:19DA
	_4976_5e6a = buff;
	//^44C8:19E7
	Bit16u di = rc->y * width + rc->x;
	Bit16u si = 0;
	//^44C8:19F8
	for (; rc->cy > si; di += width, si++) {
		//^44C8:19FA
		FIRE_FILL_8BPP_PICT_LINE(di, fill, rc->cx);
		//^44C8:1A0D
	}
	//^44C8:1A1A
	return;
}

//^44C8:1A71
void SkWinCore::FIRE_FILL_RECT_ANY(Bit8u *buff, SRECT *rc, Bit16u fill, Bit16u width, Bit16u bpp)
{
	//^44C8:1A71
	//^44C8:1A75
	Bit16u si = fill;
	//^44C8:1A78
	if (buff == NULL) {
		//^44C8:1A80
		IBMIO_FILL_RECT_SCREEN(rc, si) CALL_IBMIO;
	}
	//^44C8:1A94
	else if (bpp == 4) {
		//^44C8:1A9A
		FIRE_FILL_RECT_4BPP_PICT(buff, rc, si, width);
	}
	else {
		//^44C8:1AB0
		FIRE_FILL_RECT_8BPP_PICT(buff, rc, si, width);
	}
	//^44C8:1AC7
	return;
}

//^44C8:1DAF
void SkWinCore::FIRE_FILL_SCREEN_RECT(Bit16u rectno, Bit8u fill)
{
	//^44C8:1DAF
	//^44C8:1DB3
	SRECT bp08;
	SRECT *prc = QUERY_EXPANDED_RECT(rectno, &bp08);
	FIRE_FILL_RECT_ANY(NULL, prc, fill, bp08.cx, 8);
}

//^0AAF:0000
void SkWinCore::FIRE_FADE_SCREEN(Bit16u fadeOutIfTrue) //#DS=4976
{
	//^0AAF:0000
	//^0AAF:0003
	if (glbGameHasEnded != 0) {
		//^0AAF:000A
		if (fadeOutIfTrue != 0) {
			//^0AAF:0010
			FIRE_SELECT_PALETTE_SET(0);
			//^0AAF:0018
			FIRE_FILL_SCREEN_RECT(2, 0);
		}
		else {
			//^0AAF:0025
			FIRE_SELECT_PALETTE_SET(1);
		}
	}
	//^0AAF:002D
	return;
}

//^44C8:1D8C
void SkWinCore::FIRE_FILL_BACKBUFF_RECT(SRECT *rc, Bit8u fill)
{
	//^44C8:1D8C
	//^44C8:1D8F
    FIRE_FILL_RECT_ANY(
		_4976_4c16,
		rc,
		fill,
		_4976_00f6,
		8
		);
	//^44C8:1DAD
	return;
}

//^098D:0071
SRECT *SkWinCore::SET_ORIGIN_RECT(SRECT *rc, i16 width, i16 height)
{
	//^098D:0071
	//^098D:0074
	rc->x = 0;
	rc->y = 0;
	rc->cx = width;
	rc->cy = height;
	//^098D:0090
	return rc;
}

//^098D:00F9
SRECT *SkWinCore::INFLATE_RECT(SRECT *rc, i16 halfWidth, i16 halfHeight)
{
	//^098D:00F9
	//^098D:00FC
	rc->x -= halfWidth;
	rc->y -= halfHeight;
	rc->cx += halfWidth << 1;
	rc->cy += halfHeight << 1;
	//^098D:011E
	return rc;
}

//^098D:0201
SRECT *SkWinCore::CALC_CENTERED_RECT_IN_RECT(SRECT *rcNew, const SRECT *rcBBox, Bit16u newWidth, Bit16u newHeight)
{
	//^098D:0201
	//^098D:0204
	rcNew->cx = newWidth;
	//^098D:020E
	rcNew->x = rcBBox->x + ((rcBBox->cx - rcNew->cx + 1) >> 1);
	//^098D:022D
	rcNew->cy = newHeight;
	//^098D:0234
	rcNew->y = rcBBox->y + ((rcBBox->cy - rcNew->cy + 1) >> 1);
	//^098D:0255
	return rcNew;
}

//^098D:0CFE
void SkWinCore::QUERY_TOPLEFT_OF_RECT(Bit16u rectno, i16 *xpos, i16 *ypos)
{
	//^098D:0CFE
	//^098D:0D02
	i16 bp02 = 1;
	i16 bp04 = 1;
	//^098D:0D0C
	SRECT bp0c;
	ATLVERIFY(QUERY_BLIT_RECT(NULL, &bp0c, rectno, &bp02, &bp04, -1) != NULL);
	//^098D:0D2B
	*xpos = bp0c.x;
	*ypos = bp0c.y;
	//^098D:0D3D
	return;
}

//^0CD5:0063
void SkWinCore::SLEEP_SEVERAL_TIME(Bit16u count)
{
	//^0CD5:0063
	//^0CD5:0066
	while (count-- != 0) {
		//^0CD5:0068
		WAIT_SCREEN_REFRESH();
		//^0CD5:006C

		MessageLoop(true, true);
	}
}

//^0AAF:01DB
void SkWinCore::_0aaf_01db(Bit16u rectno, Bit16u yy)
{
	//^0AAF:01DB
	//^0AAF:01E0
	if (_4976_5cb0 != 0) {
		//^0AAF:01EA
		if (yy == 0)
			//^0AAF:01F0
			return;
		//^0AAF:01F3
		Bit8u *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(_4976_4bd5, _4976_4bd6, _4976_4bd4);
		//^0AAF:020D
		SRECT bp10;
		CALC_CENTERED_RECT_IN_RECT(&bp10, &_4976_4e9e, READ_UI16(bp04,-4), READ_UI16(bp04,-2));
		//^0AAF:0229
		if (_4976_4e44 != 0xffff) {
			//^0AAF:0230
			i16 bp12;
			i16 bp14;
			QUERY_TOPLEFT_OF_RECT(_4976_4e44, &bp12, &bp14);
			//^0AAF:0246
			bp10.x -= bp12;
			bp10.y -= bp14;
		}
		//^0AAF:0252
		Bit8u *bp08;
		if (_4976_5d76 != 0) {
			//^0AAF:0259
			bp08 = QUERY_GDAT_IMAGE_LOCALPAL(_4976_4bd5, _4976_4bd6, _4976_4bd4);
		}
		else {
			//^0AAF:0272
			bp08 = bp04 + CALC_IMAGE_BYTE_LENGTH(bp04);
		}
		//^0AAF:028D
		DRAW_DIALOGUE_PARTS_PICT(bp04, &bp10, 7, bp08);
		//^0AAF:02A8
		FREE_PICT_ENTRY(bp04);
	}
	else {
		//^0AAF:02B7
		Bit8u si = (yy != 0) ? (glbPaletteT16[COLOR_BLACK]) : (glbPaletteT16[COLOR_DARK_GRAY]);
		//^0AAF:02D2
		SRECT bp10;
		FIRE_FILL_BACKBUFF_RECT(QUERY_EXPANDED_RECT(rectno, &bp10), si);
	}
	//^0AAF:02ED
	SLEEP_SEVERAL_TIME(20);
	//^0AAF:02F5
	return;
}

//^0AAF:02F8
// _0aaf_02f8 renamed _0aaf_02f8_DIALOG_BOX
// Dialog box for new game
Bit8u SkWinCore::_0aaf_02f8_DIALOG_BOX(Bit8u xx, Bit8u yy) //#DS=4976
{
	Bit16u si;
	skxxx1 bp04e4[2];
	Bit8u bp04d4[128];
	Bit8u bp0454[128];
	Bit8u bp03d4[0x14][40];
	Bit8u *bp00b4[0x14];
	Bit8u bp64[60];
	Bit16u bp28[2];
	skxxx1 *bp24;
	SRECT bp20;
	Bit8u bp17;
	Bit16u bp16;
	Bit16u bp14;
	Bit16u bp12;
	Bit16u bp10;
	Bit16u bp0e;
	Bit16u bp0c;
	Bit16u bp0a;
	Bit8u *bp08;
	Bit8u *bp04;

	//^0AAF:02F8
	//^0AAF:02FE
	if (xx == 0x07 || xx == 0x13) {
		//^0AAF:030A
		if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_DIALOG_BOXES, 0x0059, 0x0001, 0x0000) != 0) {
			//^0AAF:031E
			xx = 0x59;
		}
	}
	//^0AAF:0322
	bp28[0] = glbPaletteT16[COLOR_YELLOW];
	bp28[1] = glbPaletteT16[COLOR_ORANGE];
	//^0AAF:0338
	if (yy != 0 && xx != 0) {
		//^0AAF:0344
		if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_DIALOG_BOXES, 0x0000, 0x0001, 0x0000) != 0) {
			//^0AAF:0358
			_0aaf_0067(_0aaf_02f8_DIALOG_BOX(0, yy));
			//^0AAF:036A
			yy = 0x00;
		}
	}
	//^0AAF:036E
	bp0c = 0;
	//^0AAF:0377
	for (bp17=0; bp17 < 0x14; bp17++) {
		//^0AAF:0379
		bp00b4[bp0c] = QUERY_GDAT_TEXT(GDAT_CATEGORY_DIALOG_BOXES, xx, bp17, bp03d4[bp0c]);
		//^0AAF:03B6
		if (bp00b4[bp0c][0] != 0) {
			//^0AAF:03C0
			bp0c++;
		}
		//^0AAF:03C3
	}
	//^0AAF:03CC
	if ((_4976_5cb0 != 0) && ((_4976_5d76 != 0) || (_4976_00f4 +8 <= glbFreeRAMMemPool))) {
		//^0AAF:03F9
		bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_DIALOG_BOXES, xx, 0x00);
		//^0AAF:040F
		if (_4976_5d76 != 0) {
			//^0AAF:0416
			bp08 = QUERY_GDAT_IMAGE_LOCALPAL(GDAT_CATEGORY_DIALOG_BOXES, xx, 0);
		}
		else {
			//^0AAF:042B
			bp08 = bp04 + CALC_IMAGE_BYTE_LENGTH(bp04);
		}
		//^0AAF:0443
		DRAW_DIALOGUE_PARTS_PICT(bp04, QUERY_EXPANDED_RECT(4, &bp20), -1, bp08);
		//^0AAF:046D
		FREE_PICT_ENTRY(bp04);
		//^0AAF:047A
	}
	else {
		//^0AAF:047D
		FIRE_FILL_BACKBUFF_RECT(SET_ORIGIN_RECT(&bp20, _4976_00f6, _4976_00f8), glbPaletteT16[COLOR_DARK_GRAY]);
		//^0AAF:04A7
		FIRE_FILL_BACKBUFF_RECT(INFLATE_RECT(&bp20, -10, -10), glbPaletteT16[COLOR_BROWN]);
		//^0AAF:04CD
		if (bp0c == 0x0003) {
			bp10 = 1;
		}
		else if (bp0c == 0x0004) {
			bp10 = 4;
		}
		else {
			bp10 = 0;
		}
		//^0AAF:04EF
		for (Bit16u si=0; si < bp0c; si++) {
			//^0AAF:04F3
			_0aaf_01db(_4976_01bc[0][RCJ(8,si + bp10)], 0);
			//^0AAF:0506
		}
	}
	//^0AAF:050C
	// SPX: Draw string version on dialog box
	DRAW_VP_RC_STR(0x1c2, glbPaletteT16[COLOR_GRAY], strVersionNumber);
	//^0AAF:0526
	FIRE_FADE_SCREEN(1);
	//^0AAF:052D
	if (bp0c < 2) {
		//^0AAF:0533
		bp0e = 0x01d5;
	}
	else {
		//^0AAF:053A
		bp0e = 0x01d7;
	}
	//^0AAF:053F
	switch (bp0c) {
		case 2:
			//^0AAF:0553
			bp10 = 1;
			break;
		case 3:
			//^0AAF:055A
			bp10 = 3;
			break;
		case 4:
			//^0AAF:0561
			bp10 = 6;
			break;
		default:
			//^0AAF:0568
			bp10 = 0;
			break;
	}
	//^0AAF:056D
	for (si=0; si < bp0c; si++) {
		//^0AAF:0571
		DRAW_VP_RC_STR(_4976_01bc[1][RCJ(8,si + bp10)], glbPaletteT16[COLOR_ORANGE], bp00b4[si]);
		//^0AAF:05A1
	}
	//^0AAF:05A7
	QUERY_EXPANDED_RECT(bp0e, &bp20);
	//^0AAF:05B7
	bp12 = bp20.cx;
	bp14 = bp20.cy;
	//^0AAF:05C3
	if (yy != 0 && xx == 0) {
		//^0AAF:05CF
		bp04e4[0].pb0 = QUERY_GDAT_TEXT(GDAT_CATEGORY_DIALOG_BOXES, xx, yy, bp0454);
	}
	else {
		//^0AAF:05F1
		bp04e4[0].pb0 = QUERY_GDAT_TEXT(GDAT_CATEGORY_DIALOG_BOXES, xx, 0x14, bp0454);
		//^0AAF:060F
		if (yy != 0) {
			//^0AAF:0615
			bp04e4[1].pb0 = QUERY_GDAT_TEXT(GDAT_CATEGORY_DIALOG_BOXES, 0, yy, bp04d4);
			//^0AAF:0633
			goto _0641;
		}
	}
	//^0AAF:0635
	bp04e4[1].pb0 = NULL;
	//^0AAF:0641
_0641:
	Bit16u di = 0;
	si = 0;
	//^0AAF:0647
	bp24 = &bp04e4[0];
	//^0AAF:0651
	for (bp16=0; bp16 < 2; bp16++, bp24++) {
		//^0AAF:0659
		if (bp24->pb0 != 0) {
			//^0AAF:0668
			QUERY_STR_METRICS(bp24->pb0, &bp24->w4, &bp24->w6);
			//^0AAF:068B
			if (bp24->w4 > bp12 - (bp12 >> 3)) {
				//^0AAF:069F
				if (bp24->w4 < bp12 + (bp12 >> 2)) {
					//^0AAF:06B0
					bp24->w4 -= bp24->w4 >> 2;
				}
				else {
					//^0AAF:06BD
					bp24->w4 = bp12;
				}
			}
			//^0AAF:06C7
			for (bp0e=0; bp24->pb0[bp0e] != 0; ) {
				//^0AAF:06CE
				_3929_04e2_DRAW_TEXT_STRINGS(bp24->pb0, bp64, &bp0e, bp24->w4);
				//^0AAF:06EE
				di += bp24->w6 + (_4976_0134 << 1) - (_4976_0136) + 1;
				//^0AAF:0704
				if (bp24->pb0[bp0e] == vbLf) {
					//^0AAF:0710
					bp0e++;
				}
				//^0AAF:0713
			}
		}
		//^0AAF:0722
	}
	//^0AAF:0732
	if (di != 0) {
		//^0AAF:0739
		di = max_value(0, ((bp14 - di - (_4976_0134 << 1)) >> 1) + _4976_0130 - 1);
		//^0AAF:075A
		di += bp20.y;
		//^0AAF:075D
		bp24 = bp04e4;
		//^0AAF:0767
		for (bp16=0; bp16 < 2; bp24++, bp16++) {
			//^0AAF:076F
			if (bp24->pb0 != NULL) {
				//^0AAF:077B
				for (bp0e=0; bp24->pb0[bp0e] != 0; ) {
					//^0AAF:0782
					bp0a = _3929_04e2_DRAW_TEXT_STRINGS(bp24->pb0, bp64, &bp0e, bp24->w4);
					//^0AAF:07A5
					DRAW_VP_STR((_4976_00f6 -bp0a) >> 1, di, bp28[bp16], bp64);
					//^0AAF:07C9
					di += _4976_013a + _4976_0134;
					//^0AAF:07D2
					if (bp24->pb0[bp0e] == vbLf) {
						//^0AAF:07E1
						bp0e++;
					}
					//^0AAF:07E4
				}
			}
			//^0AAF:07F3
		}
	}
	//^0AAF:0803
	_0aaf_002f();
	FIRE_FADE_SCREEN(0);
	//^0AAF:080E
	_4976_022c = 1;
	//^0AAF:0814
	return xx;
}

//^3E74:2614
void SkWinCore::GRAPHICS_DATA_CLOSE()
{
	//^3E74:2614
	if ((_4976_5d10--) == 0) {
		//^3E74:261D
		FILE_CLOSE(glbFileHandleGraphics1);
		//^3E74:2627
		if (_4976_5c9c != 0 && _4976_5ca8 == 0) {
			//^3E74:2635
			FILE_CLOSE(glbFileHandleGraphics2);
		}
	}
	//^3E74:263F
	return;
}


//^0CD5:00D2
Bit16u SkWinCore::_RAND() //#DS=4976
{
	// CSBwinSimilarity: TAG001030,STRandom

	//^0CD5:00D2
	glbRandomSeed = glbRandomSeed * 0xbb40e62d + 11;
	//^0CD5:00F5
	return glbRandomSeed >> 8;
	//^0CD5:00FD
}

//^4937:00CC
// SPX: This function uses creature animation/sequence information related to command
// _4937_00cc renamed GET_CREATURE_ANIMATION_FRAME
Bit16u SkWinCore::GET_CREATURE_ANIMATION_FRAME(Bit8u ct, Bit16u command, Bit16u *pw08, Bit16u *pw0a, CreatureAnimationFrame **animframe, Bit16u vv)
{
	//^4937:00CC
	SkD((DLV_DBG_SED2, "DBG: GET_CREATURE_ANIMATION_FRAME %02X ccm:%04X %04X %04X %04X \n", (Bitu)ct, (Bitu)command, (Bitu)*pw08, (Bitu)*pw0a, (Bitu)vv));
	CreatureCommandAnimation *bp04 = &tlbCreaturesActionsGroupSets[tlbCreaturesActionsGroupOffsets[QUERY_GDAT_CREATURE_WORD_VALUE(ct, 0)]];
	//^4937:00FD
	while (bp04->ccmReference != 0xffff && bp04->ccmReference != command) {
		bp04++;
	}
	//^4937:0114
	Bit16u di = bp04->animSeqOffset;
	*pw08 = di;	// Is the base anim offset
	//^4937:0123
	if (QUERY_CREATURE_AI_SPEC_FROM_TYPE(ct)->IsStaticObject() != 0) {
		//^4937:0138
		Bit16u si = 0;
		CreatureAnimationFrame *bp08 = &tlbCreaturesAnimationSequences[di];
		//^4937:0151
		do {
			si++;
		} while((bp08++)->w2_0_3() != 0);
		//^4937:0161
		if (vv != 0) {
			//^4937:0167
			si |= 0x8000 | ((vv & 0x003f) << 6);
		}
		else {
			//^4937:0179
			si |= 0x9000;
		}
		//^4937:017D
		*pw0a = si;	// would be the number of frames within animation
		return 1;
	}
	//^4937:0188
	*pw0a = 0xffff;
	//^4937:0190
	return _4937_01a9(di, pw0a, animframe);
}

//^1C9A:09DB
void SkWinCore::_1c9a_09db(ObjectID recordLink)
{
	//^1C9A:09DB
	CreatureAnimationFrame bp0e;
	CreatureAnimationFrame *bp12 = &bp0e;
	//^1C9A:09E8
	Creature *bp04 = GET_ADDRESS_OF_RECORD(recordLink)->castToCreature();
	//^1C9A:09F7
	sk1c9a02c3 *bp08 = _1c9a_02c3(bp04, QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp04->CreatureType()));
	//^1C9A:0A1A
	GET_CREATURE_ANIMATION_FRAME(
		bp04->CreatureType(),
		0x11,
		&bp08->w0,
		&bp08->w2,
		&bp12,
		bp04->w12
		);
}


//^476D:007A
i16 SkWinCore::FILE_OPEN(const U8 *filename) {
	i16 iHandle = fset.fileOpen(reinterpret_cast<const char *>(filename));
	//printf("FILE_OPEN %s with HANDLE %d\n", filename, iHandle);
	return iHandle;
}

//^476D:00D2
U16 SkWinCore::FILE_READ(Bit16u handle, Bit32u buffSize, void *buff) {
	return fset.fileRead(handle, buffSize, buff);
}

//^476D:00A5
void SkWinCore::FILE_CLOSE(Bit16u handle) {
	fset.fileClose(handle);
}

//^476D:00BF
void SkWinCore::FILE_RENAME(const U8 *curf, const U8 *newf)
{
	fset.fileMove(reinterpret_cast<const char *>(curf), reinterpret_cast<const char *>(newf));
}

//^00EB:02EB
void SkWinCore::IBMIO_BLIT_TO_SCREEN_xTO8BPP(
	const void *src,
	SRECT *rc,
	i16 srcx,
	i16 srcy,
	i16 srcPitch,
	i16 colorkey,
	const void *localPal,
	i16 srcBpp
	)
{
#if UseAltic
	SkD((DLV_GUI, "GUI: xToScr (%p,(%3d,%3d,%3d,%3d),%3d,%3d,%3d,%2d,%p,%d)\n"
		, src
		, (Bitu)rc->x, (Bitu)rc->y, (Bitu)rc->cx, (Bitu)rc->cy
		, (Bitu)srcx, (Bitu)srcy, (Bitu)srcPitch, (Bitu)colorkey, localPal, (Bitu)srcBpp));

	Pic8 dp;
	dp.buff = (U8 *)vram; dp.ox = rc->x; dp.oy = rc->y; dp.pitch = 320; dp.cy = 200; dp.dx = 1; dp.dy = 1;

	if (srcBpp == 8) {
		Pic8 sp;
		sp.buff = (U8 *)src; sp.ox = srcx; sp.oy = srcy; sp.pitch = srcPitch; sp.cy = 0; sp.dx = 1; sp.dy = 1;
		BlitSvc(sp, dp, rc->cx, rc->cy, colorkey);
	}
	else {
		Pic4 sp;
		sp.buff = (U8 *)src; sp.ox = srcx; sp.oy = srcy; sp.cx = srcPitch; sp.pitch = (srcPitch +1) / 2; sp.cy = 0; sp.dx = 1; sp.dy = 1; sp.pal = (U8 *)localPal;
		BlitSvc(sp, dp, rc->cx, rc->cy, colorkey);
	}

	skwin.UpdateRect(rc->x, rc->y, rc->cx, rc->cy);
	MessageLoop(false);
#else
	printf("IBMIO_BLIT_TO_SCREEN_xTO8BPP\n");
	Unr();
#endif
}
//^44C8:05AE
void SkWinCore::FIRE_BLIT_TO_MEMORY_4TO4BPP(
	const void *src,
	void *dst,
	SRECT *rc,
	i16 srcx,
	i16 srcy,
	i16 srcPitch,
	i16 dstPitch,
	i16 colorkey,
	i16 mirrorFlip
	)
{
#if UseAltic
	SkD((DLV_GUI, "GUI: 4To4 (%p,%p,(%3d,%3d,%3d,%3d),%3d,%3d,%3d,%3d,%2d,%d)\n"
		, src, dst
		, (Bitu)rc->x, (Bitu)rc->y, (Bitu)rc->cx, (Bitu)rc->cy, (Bitu)srcx, (Bitu)srcy
		, (Bitu)srcPitch, (Bitu)dstPitch, (Bitu)colorkey, (Bitu)mirrorFlip));

	static const U8 thru[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

	Pic4 sp;
	sp.buff = (U8 *)src; sp.ox = srcx; sp.oy = srcy; sp.cx = srcPitch; sp.pitch = (srcPitch +1) / 2; sp.cy = 0; sp.dx = 1; sp.dy = 1; sp.pal = thru;
	sp.Apply(mirrorFlip & 1, mirrorFlip & 2);
	Pic4 dp;
	dp.buff = (U8 *)dst; dp.ox = rc->x; dp.oy = rc->y; dp.pitch = (dstPitch +1) / 2; dp.cy = 0; dp.dx = 1; dp.dy = 1; dp.pal = thru;
	BlitSvc(sp, dp, rc->cx, rc->cy, colorkey);
#else
	printf("FIRE_BLIT_TO_MEMORY_4TO4BPP\n");
	Unr();
#endif
}

//^44C8:09D7
void SkWinCore::FIRE_BLIT_TO_MEMORY_4TO8BPP(
	const void *src,
	void *dst,
	SRECT *rc,
	i16 srcx,
	i16 srcy,
	i16 srcPitch,
	i16 dstPitch,
	i16 colorkey,
	i16 mirrorFlip,
	const U8 *localPal
	)
{
#if UseAltic
	SkD((DLV_GUI, "GUI: 4To8 (%p,%p,(%3d,%3d,%3d,%3d),%3d,%3d,%3d,%3d,%2d,%d,%p)\n"
		, src, dst
		, (Bitu)rc->x, (Bitu)rc->y, (Bitu)rc->cx, (Bitu)rc->cy, (Bitu)srcx, (Bitu)srcy
		, (Bitu)srcPitch, (Bitu)dstPitch, (Bitu)colorkey, (Bitu)mirrorFlip, localPal));

	ATLASSERT(localPal != NULL);
	FIRE_UPDATE_BLIT_PALETTE(localPal);

	Pic4 sp;
	sp.buff = (U8 *)src; sp.ox = srcx; sp.oy = srcy; sp.cx = rc->cx; sp.pitch = (srcPitch +1) / 2; sp.cy = rc->cy; sp.dx = 1; sp.dy = 1; sp.pal = (U8 *)localPal;
	sp.Apply(mirrorFlip&1, mirrorFlip&2);
	Pic8 dp;
	dp.buff = (U8 *)dst; dp.ox = rc->x; dp.oy = rc->y; dp.pitch = dstPitch; dp.cy = 0; dp.dx = 1; dp.dy = 1;
	BlitSvc(sp, dp, rc->cx, rc->cy, colorkey);
#else
	printf("FIRE_BLIT_TO_MEMORY_4TO8BPP\n");
	Unr();
#endif
}

//^44C8:0B8D
void SkWinCore::_44c8_0b8d(U16 src, U16 dst, U16 pitch)
{
	//^44C8:0B8D
	ENTER(0);
	//^44C8:0B92
	U8 *esdi = _4976_5e6a;
	esdi += dst;
	U16 cx = pitch;
	const U8 *dssi = _4976_5e64;
	dssi += src;
	//^44C8:0BA9
#ifdef __DJGPP__
	if ((U32(dssi) & 1) != 0) {	// SPX not sure of the meaning of pointer & 1
#else
	if ((U8(dssi) & 1) != 0) {
#endif
		movsb(esdi, dssi);
		cx--;
	}
	//^44C8:0BB2
	U16 dx = cx;
	bool carry = (cx & 1) != 0;
	cx >>= 1;
	if (cx != 0)
		for (; cx != 0; cx--) movsw(esdi, dssi);
	dx &= 1;
	if (dx != 0)
		movsb(esdi, dssi);
	//^44C8:0BC1
	return;
}
void SkWinCore::_44c8_0bc5(U16 src, U16 dst, U16 pitch, U8 ck) { // TODO: Unr
	Unr();
}
void SkWinCore::_44c8_0bf8(U16 src, U16 dst, U16 pitch) { // TODO: Unr
	Unr();
}
void SkWinCore::_44c8_0c3c(U16 src, U16 dst, U16 pitch, U8 ck) { // TODO: Unr
	Unr();
}

//^44C8:0C74
void SkWinCore::FIRE_BLIT_TO_MEMORY_8TO8BPP(
	const void *src,
	void *dst,
	SRECT *rc,
	i16 srcx,
	i16 srcy,
	i16 srcPitch,
	i16 dstPitch,
	i16 colorkey,
	i16 mirrorFlip
	)
{
#if UseAltic
	SkD((DLV_GUI, "GUI: 8To8 (%p,%p,(%3d,%3d,%3d,%3d),%3d,%3d,%3d,%3d,%2d,%d)\n"
		, src, dst
		, (Bitu)rc->x, (Bitu)rc->y, (Bitu)rc->cx, (Bitu)rc->cy, (Bitu)srcx, (Bitu)srcy
		, (Bitu)srcPitch, (Bitu)dstPitch, (Bitu)colorkey, (Bitu)mirrorFlip));

	Pic8 sp;
	sp.buff = (U8 *)src; sp.ox = srcx; sp.oy = srcy; sp.pitch = srcPitch; sp.cy = 0; sp.dx = 1; sp.dy = 1;
	Pic8 dp;
	dp.buff = (U8 *)dst; dp.ox = rc->x; dp.oy = rc->y; dp.pitch = dstPitch; dp.cy = 0; dp.dx = 1; dp.dy = 1;
	BlitSvc(sp, dp, rc->cx, rc->cy, colorkey);
#else
	//^44C8:0C74
	ENTER(6);
	//^44C8:0C7A
	_4976_5e64 = reinterpret_cast<U8 *>(const_cast<void *>(src));
	_4976_5e6a = reinterpret_cast<U8 *>(dst);
	U16 bp04 = rc->cx;
	U16 bp02 = rc->cy;
	if (bp04 <= 0 && bp02 <= 0)
		return;
	//^44C8:0CB7
	U16 di = rc->y * dstPitch +rc->x;
	U16 si;
	U16 bp06;
	switch (mirrorFlip) {
		case 3://^_0cde
			//^44C8:0CDE
			//^44C8:0CE0
			si = srcy * srcPitch;
			for (bp06 = 0; bp06 < bp02; si += srcPitch, di += dstPitch, bp06++) {
				//^44C8:0CF2
				if (colorkey < 0) {
					//^44C8:0CF8
					_44c8_0b8d(si, di, bp04);
				}
				else {
					//^44C8:0D06
					_44c8_0bc5(si, di, bp04, U8(colorkey));
				}
				//^44C8:0D16
			}
			//^44C8:0D27
			break;
		case 2://^_0d2a
			//^44C8:0D2A
			si = (srcy +bp02 -1) * srcPitch +srcx;
			for (bp06 = 0; bp06 < bp02; si -= srcPitch, di += dstPitch, bp06++) {
				//^44C8:0D42
				if (colorkey < 0) {
					//^44C8:0D48
					_44c8_0b8d(si, di, bp04);
				}
				else {
					//^44C8:0D56
					_44c8_0bc5(si, di, bp04, U8(colorkey));
				}
				//^44C8:0D66
			}
			//^44C8:0D77
			break;
		case 1://^_0d7a
			//^44C8:0D7A
			si = srcy * srcPitch +srcx;
			for (bp06 = 0; bp06 < bp02; si += srcPitch, di += dstPitch, bp06++) {
				//^44C8:0D8C
				if (colorkey < 0) {
					//^44C8:0D92
					_44c8_0bf8(si, di, bp04);
				}
				else {
					//^44C8:0DA0
					_44c8_0c3c(si, di, bp04, U8(colorkey));
				}
				//^44C8:0DB0
			}
			//^44C8:0DC1
			break;
		default: //^_0dc3
			//^44C8:0DC3
			si = (srcy +bp02 -1) * srcPitch +srcx;
			for (bp06 = 0; bp06 < bp02; si -= srcPitch, di += dstPitch, bp06++) {
				//^44C8:0DDB
				if (colorkey < 0) {
					//^44C8:0DE1
					_44c8_0bf8(si, di, bp04);
				}
				else {
					//^44C8:0DEF
					_44c8_0c3c(si, di, bp04, U8(colorkey));
				}
				//^44C8:0DFF
			}
			break;
	}
	//^44C8:0E10
	return;
#endif
}

//^44C8:0F29
void SkWinCore::_44c8_0f29(	// TODO: Unr
	const void *src,
	void *dst,
	SRECT *rc,
	i16 srcx,
	i16 srcy,
	i16 srcPitch,
	i16 dstPitch,
	i16 colorkey,
	i16 mirrorFlip,
	const void *localPal
	)
{
#if UseAltic
	SkD((DLV_GUI, "GUI: 8To8wPal (%p,%p,(%3d,%3d,%3d,%3d),%3d,%3d,%3d,%3d,%2d,%d)\n"
		, src, dst
		, (Bitu)rc->x, (Bitu)rc->y, (Bitu)rc->cx, (Bitu)rc->cy, (Bitu)srcx, (Bitu)srcy
		, (Bitu)srcPitch, (Bitu)dstPitch, (Bitu)colorkey, (Bitu)mirrorFlip));

	Pic8wPal sp;
	sp.buff = (U8 *)src; sp.ox = srcx; sp.oy = srcy; sp.cx = srcPitch; sp.cy = 0; sp.dx = 1; sp.dy = 1; sp.pal = reinterpret_cast<const U8 *>(localPal); sp.cx2 = rc->cx; sp.cy2 = rc->cy;
	sp.Apply(mirrorFlip & 1, mirrorFlip & 2);
	Pic8 dp;
	dp.buff = (U8 *)dst; dp.ox = rc->x; dp.oy = rc->y; dp.pitch = dstPitch; dp.cy = 0; dp.dx = 1; dp.dy = 1;
	BlitSvc(sp, dp, rc->cx, rc->cy, colorkey);
#else
	Unr();
#endif
}

//^44C8:1101
void SkWinCore::FIRE_BLIT_PICTURE(
	const void *src,
	void *dst,
	SRECT *rc,
	U16 srcx,
	U16 srcy,
	U16 srcPitch,
	U16 dstPitch,
	i16 colorkey,
	U16 mirrorFlip,
	U16 srcBpp,
	U16 dstBpp,
	const U8 *localPal
	)
{
	SkD((DLV_GUI, "GUI: xTox (%p,%p,(%3d,%3d,%3d,%3d),%3d,%3d,%3d,%3d,%2d,%d,%d,%d,%p)\n", src, dst
		, (Bitu)rc->x, (Bitu)rc->y, (Bitu)rc->cx, (Bitu)rc->cy, (Bitu)srcx, (Bitu)srcy
		, (Bitu)srcPitch, (Bitu)dstPitch, (Bitu)colorkey, (Bitu)mirrorFlip, (Bitu)srcBpp, (Bitu)dstBpp, localPal));
#if UseAltic
	ATLASSERT((srcBpp == 4 || srcBpp == 8) && (dstBpp == 4 || dstBpp == 8));
	ATLASSERT(colorkey == -1 || (0 <= colorkey && colorkey <= 255));
	U16 dstibpp = 8, dsticx = 320, dsticy = 200;
	if (dst != NULL) {
		dstibpp = READ_I16(dst,-6);
		dsticx = READ_I16(dst,-4);
		dsticy = READ_I16(dst,-2);
	}
	ATLASSERT(dstBpp == dstibpp);
	ATLASSERT((dstBpp == 4) ? RUp2(dstPitch) == RUp2(dsticx) : true);
	ATLASSERT((dstBpp == 8) ? (dstPitch == dsticx) : true);
	ATLASSERT(rc->x >= 0);
	ATLASSERT(rc->y >= 0);
	ATLASSERT(rc->cx >= 0);
	ATLASSERT(rc->cy >= 0);
	ATLASSERT((0 +rc->x +RUp2(dsticx) * (rc->y +rc->cy -1) +rc->cx) <= (RUp2(dsticx) * dsticy));

#endif

	//^44C8:1101
	ENTER(0);
	//^44C8:1106
	Bit16u di = srcPitch;
	Bit16u si = colorkey;
	//^44C8:110C
	if (dst == NULL) {
		//^44C8:1114
		IBMIO_BLIT_TO_SCREEN_xTO8BPP(src, rc, srcx, srcy, di, si, localPal, srcBpp) CALL_IBMIO;
		//^44C8:113C
	}
	else {
		//^44C8:113F
		if (srcBpp == 4) {
			//^44C8:1145
			if (dstBpp == 4) {
				//^44C8:114B
				FIRE_BLIT_TO_MEMORY_4TO4BPP(src, dst, rc, srcx, srcy, srcPitch, dstPitch, colorkey, mirrorFlip);
			}
			else {
				//^44C8:1171
				FIRE_BLIT_TO_MEMORY_4TO8BPP(src, dst, rc, srcx, srcy, srcPitch, dstPitch, colorkey, mirrorFlip, localPal);
			}
		}
		else {
			//^44C8:11A0
			if (localPal == NULL) {
				//^44C8:11A8
				FIRE_BLIT_TO_MEMORY_8TO8BPP(src, dst, rc, srcx, srcy, srcPitch, dstPitch, colorkey, mirrorFlip);
			}
			else {
				//^44C8:11D1
				_44c8_0f29(src, dst, rc, srcx, srcy, di, dstPitch, si, mirrorFlip, localPal);
			}
		}
	}
	//^44C8:11FE
	return;
}

//^00EB:0254
void SkWinCore::IBMIO_BLIT_TO_SCREEN_8TO8BPP(
	void *src,
	void *dst,
	SRECT *prc,
	i16 srcx,
	i16 srcy,
	i16 srcpitch,
	i16 dstpitch,
	i16 colorkey
	) //#DS=04BF
{
	//printf("IBMIO_BLIT_TO_SCREEN_8TO8BPP\n");
	ATLASSERT(srcx >= 0 && srcy >= 0 && srcpitch >= 0 && dstpitch >= 0 && srcpitch <= 320 && dstpitch <= 320);
	ATLASSERT(prc->x >= 0 && prc->y >= 0 && prc->cx < 320 && prc->cy < 200);

	//^00EB:0254
	ENTER(6);
	//^00EB:025A
	_04bf_0cf0 = reinterpret_cast<U8 *>(src);
	_04bf_0e34 = reinterpret_cast<U8 *>(dst);
	X16 bp04 = prc->cx;
	X16 bp02 = prc->cy;
	//printf("cx =%d / cy = %d\n", bp04, bp02);
	if (bp04 <= 0 || bp02 <= 0)
		return;
	//^00EB:0291
	X16 di = prc->y *dstpitch +prc->x;
	X16 si = srcy *srcpitch +srcx;
	X16 bp06 = 0;
	for (; bp06 < bp02; si += srcpitch, di += dstpitch, bp06++) {
		//^00EB:02B2
		if (colorkey < 0) {
			//^00EB:02B8
			MOVE_MEMORY_WITH_DELTA(si, di, bp04);
		}
		else {
			//^00EB:02C6
			COPY_MEMORY_WITH_DELTA_AND_TRANSPARENT(si, di, bp04, U8(colorkey));
		}
		//^00EB:02D6
	}
	//^00EB:02E7

	//printf("dst = %08x / vram = %08x\n", dst, vram);
#ifdef __DJGPP__
	if (dst == vram) {
		//printf("skwin.UpdateRect");
		skwin.UpdateRect(prc->x, prc->y, prc->cx, prc->cy);
	}
#elif UseAltic
	if (dst == vram) {
		if (skwin.IsAvail() != NULL) skwin.UpdateRect(prc->x, prc->y, prc->cx, prc->cy);
		//MessageLoop();
	}
#endif

}

//^476D:030A
Bit16u SkWinCore::_476d_030a(Bit16u xx) { // TODO: Unr
	Unr(); return 0;
}


//^4726:0107
void SkWinCore::EMS_MAP_BANK_TO_MEMORY() { // TODO: Unr
#if UseAltic
	;
#else
	Unr();
#endif
}

//^4726:0107
Bit8u *SkWinCore::EMS_MAP_BANK_TO_MEMORY(shelf_memory info, Bit16u physPage0to3) { // TODO: Unr
#if UseAltic
	return REALIZE_GRAPHICS_DATA_MEMORY(info);
#else
	return Unr(), NULL;
#endif
}

//^3E74:0683
Bit8u *SkWinCore::FIND_FREE_POOL(i32 size, Bit16u poolno)
{
	//^3E74:0683
	Bit16u si = poolno;
	//^3E74:068B
	EMS_MAP_BANK_TO_MEMORY();
	//^3E74:0699
	sk5d00 *bp04 = _4976_5d00;
	//^3E74:06A6
	Bit8u *bp08 = 0;
	i32 bp10 = 0x7fffffff;
	//^3E74:06BA
	while (bp04 != NULL) {
		//^3E74:06BC
		i32 bp0c;
		if (true
			&& (bp04->w18 == 0)
			&& (bp04->w16 & si) == si 
			&& (bp04->w16 & 0x0800) == (si & 0x0800) 
			&& (bp0c = bp04->dw12 -size) >= 0
			&& (bp0c < bp10)
		) {
			//^3E74:0711
			bp10 = bp0c;
			bp08 = (Bit8u *)bp04;
		}
		//^3E74:0729
		bp04 = bp04->pv0;
		//^3E74:0739
	}
	//^3E74:074D
	return bp08;
}

U8 *SkWinCore::ALLOC_MEMORY_RAM(i32 size, Bit16u flags, Bit16u poolno) {
	ATLASSERT((poolno & 0x800) == 0);
	U8 *pb = t2ptr(ALLOC_MEMORY_(size, flags, poolno));
	SkD((DLV_MEM, "MEM: ALLOC_MEMORY_RAM(%10d,%04X,%04X) = %08X  (free:%7u)\n", (Bitu)size, (Bitu)flags, (Bitu)poolno, pb, (Bitu)glbFreeRAMMemPool));
	return pb;
}
shelf_memory SkWinCore::ALLOC_MEMORY_EMS(i32 size, Bit16u flags, Bit16u poolno) {
	ATLASSERT((poolno & 0x800) != 0);
	shelf_memory ps = t2s(ALLOC_MEMORY_(size, flags, poolno));
	SkD((DLV_MEM, "MEM: ALLOC_MEMORY_EMS(%10d,%04X,%04X) = %08X  (free:%7u)\n", (Bitu)size, (Bitu)flags, (Bitu)poolno, (Bitu)ps.val, (Bitu)glbFreeEMSMemPool));
	return ps;
}

//^3E74:088E
tiamat SkWinCore::ALLOC_MEMORY_(i32 size, Bit16u flags, Bit16u poolno)
{
	// if (flags & 0x7FFF)==1, try to allocate memory from upper free area (for temporary use?)
	// if (flags & 0x7FFF)==2, try to allocate memory from lower free area (for permanent use?)
	// if (flags & 0x8000)!=0, clear memory with 0

	// if (poolno & 0x0c00)==0x0c00, it forces engine to allocate ibmio free memory. if no avail, SYSTEM ERROR 39
	// if (poolno & 0x0800)!=0x0000, it forces engine to allocate external free memory. if no avail, SYSTEM ERROR 39

	// if out of memory, always SYSTEM ERROR 40

	//ATLASSERT(0x200000 <= U32(_4976_5d5a) && U32(_4976_5d5a) <= 0x200000 +sizeof(cems));
	ATLASSERT(cram <= _4976_5cf4 && _4976_5cf4 <= cram + shelf_memory::SizeOf_cems()); // replaced sizeof(cram)
	ATLASSERT(0 <= glbFreeRAMMemPool);

	//^3E74:088E
	Bit16u si = flags;
	Bit16u di = poolno;
	Bit16u bp0a = si & 0x8000;
	tiamat bp04;
	si &= 0x7fff;
	//^3E74:08A6
	if ((size & 1) != 0)
		size++;
	//^3E74:08B9
	if (si == 1) {
		//^3E74:08BE
		Bit8u *bp08 = FIND_FREE_POOL(size, di);
		//^3E74:08D2
		if (bp08 != NULL) {
			//^3E74:08DD
			bp04 = _3e74_0756(bp08, size);

			//^3E74:09F7
			goto _09f7;
		}
	}
	//^3E74:08F9
	if (si == 1) { // alloc upper mem
		//^3E74:0901
		if ((di & _4976_5d6e) == di) {
			//^3E74:090B
			if (size <= glbFreeEMSMemPool) {
				//^3E74:091F
				if ((_4976_5d6e & 0x0800) == (di & 0x0800)) {
					//^3E74:092F
					glbFreeEMSMemPool -= size;
					//^3E74:093D
					if ((_4976_5d6e & 0x0800) != 0) {
						//^3E74:0945
						_4976_5d5a -= size;
						bp04 = _4976_5d5a;
						//^3E74:09F7
						goto _09f7;
					}
					else {
						//^3E74:0962
						_4976_5d5a -= size;
						//^3E74:0972
						bp04 = _4976_5d5a;
						//^3E74:09F7
						goto _09f7;
					}
				}
			}
		}
	}

	/*
	if (SkCodeParam::bUseIngameDebug && glbTickSpeed > 0)
	{
		U8 message[32];
		sprintf((char*)message, "FREE MEM = %06d , ALLOC SIZE = %05d \n", glbFreeRAMMemPool, size);
		DISPLAY_HINT_TEXT(COLOR_YELLOW, message);
	}
	*/


	//^3E74:0981
	if ((di & 0x0800) != 0) { // syserr39 if run out of EMS memory!
		//^3E74:0987
		RAISE_SYSERR(SYSTEM_ERROR__OUT_OF_EMS_MEM);
	}
	//^3E74:098F
	if (size > glbFreeRAMMemPool) { // syserr40 if run out of main memory!
		//^3E74:09A3
		RAISE_SYSERR(SYSTEM_ERROR__OUT_OF_MAIN_MEM);
	}
	//^3E74:09AB
	if (si == 2) { // from lower pool
		//^3E74:09B0
		bp04 = ptr2t(_4976_5cf4 - glbFreeRAMMemPool);

#if defined(_USE_MFC80) || defined(_USE_MFC60) || defined(_USE_SDL)
		vecLowerAlloc.push(size);
#endif // defined(_USE_MFC80) || defined(_USE_MFC60) || defined(_USE_SDL)
	}
	else { // from upper pool
		//^3E74:09CC
		_4976_5cf4 -= size;
		bp04 = ptr2t(_4976_5cf4);

#if defined(_USE_MFC80) || defined(_USE_MFC60) || defined(_USE_SDL)
		vecUpperAlloc.push(size);
#endif // defined(_USE_MFC80) || defined(_USE_MFC60) || defined(_USE_SDL)
	}
	//^3E74:09E9
	glbFreeRAMMemPool -= size;
	//^3E74:09F7
_09f7:
#if UseAltic
	if ((poolno & 0x800) == 0) {
		memset(t2ptr(bp04), 0xcc, size); // for DEBUG purpose
	}
#endif
	if (bp0a != 0) {
		ATLASSERT((poolno & 0x800) == 0); // clear cannot be ran if you request shelf_memory address!
		ZERO_MEMORY(t2ptr(bp04), size);
	}
	return bp04;
}

//^3E74:0B96
Bit8u *SkWinCore::ALLOC_PICT_BUFF(Bit16u srccx, Bit16u srccy, Bit16u flags, Bit16u bpp)
{
	//^3E74:0B96
	Bit16u si = srccx;
	//^3E74:SkWinCore::0B9E
	Bit8u *bp04 = ALLOC_MEMORY_RAM((((bpp == 4) ? (((si +1) & 0xfffe) >> 1) : (si & 0xffff)) * srccy) +6, flags, 8) +6;
	//^3E74:0BD2
	bp04 = bp04;
	//^3E74:0BE1
	WRITE_UI16(bp04,-6,bpp);	// bpp
	//^3E74:0BFF
	WRITE_UI16(bp04,-4,si);		// image width
	//^3E74:0C06
	WRITE_UI16(bp04,-2,srccy);	// image height
	//^3E74:0C0D
	return bp04;
}

//^47E1:0008
void SkWinCore::COPY_MEMORY(const void *buffSrc, void *buffDst, Bit32u buffSize) {
	memcpy(buffDst, buffSrc, buffSize);
}

//^3E74:189B
//U16 SkWinCore::QUERY_GDAT_ENTRY_DATA_INDEX(U8 cls1, U16 cls2, U8 cls3, U16 cls4)
U16 SkWinCore::QUERY_GDAT_ENTRY_DATA_INDEX(U8 iCategory, U16 iItem, U8 iType, U16 iEntry)
{
//LOGX(("%40s: C%02d=I%02X=E%02X=T%03d to %08X", "QUERY_GDAT_ENTRY_DATA_INDEX from ", iCategory, iItem, iEntry, iType ));
	//^3E74:189B
	RawEntry *bp04 = QUERY_GDAT_ENTRYPTR(iCategory, iItem, iType, iEntry);
	if (bp04 == NULL) {
		//^3E74:18C4
		switch (iType) {
			case fmtWordVal: //0x0B:
			case fmtPicOff://0x0C:	// word
				//^3E74:18D5
				return 0;
		}
		//^3E74:18D0
		return -1;
	}
	else {
		//^3E74:18D9
		switch (iType) {
			case fmtWordVal://0x0B:
			case fmtPicOff://0x0C:
                //^3E74:18F1
				return bp04->data;
		}
		//^3E74:18E5
		return bp04->data & 0x7FFF;
	}
}

//^3E74:01A5
Bit8u *SkWinCore::REALIZE_GRAPHICS_DATA_MEMORY(shelf_memory info) {
	//U32 iMemVal = info.val & 0x00FFFFFF;	// SPX: replaced info.val by this iMemVal with mask. ==> NOTE This does not work. Need to understand more this
	U32 iMemVal = info.val;
	// Original code would just take full value, which can be 0x80****** and break the limit.
	// the 0x80 part must be some flag info then not to be taken for memory check.
	// Note: it happens when loading original BETA GDAT.

#if UseAltic
	if (iMemVal >= 0x200000 + shelf_memory::SizeOf_cems()) { // replaced sizeof(cems)
		ATLASSERT(false);
		return NULL;
	}
	if (iMemVal >= 0x00200000) {
		return &cems[iMemVal -0x00200000];
	}
	else if (iMemVal >= sizeof(cram)) {
		ATLASSERT(false);
		return NULL;
	}
	else {
		return &cram[iMemVal];
	}
#else
	ATLASSERT(false);
	return Unr(), NULL;
#endif
}

//^3E74:0422
Bit16u SkWinCore::QUERY_GDAT_RAW_DATA_LENGTH(Bit16u index)
{
	//^3E74:0422
	ENTER(4);
	//^3E74:0426
    //shelf_memory bp04 = glbShelfMemoryTable[index];
	// SPX : rewritten block; I encountered the case where index is > to glbGDatNumberOfData
	// which is not correct ... => added assert.
	shelf_memory bp04;
	if (index > glbGDatNumberOfData && SkCodeParam::bUsePowerDebug)
	{
	//	ATLASSERT(index < glbGDatNumberOfData);
		return 0;
	}
	
	bp04 = glbShelfMemoryTable[index];
	//--- SPX END

	if (bp04.Present()) {
		//^3E74:044A
		return READ_UI16(REALIZE_GRAPHICS_DATA_MEMORY(bp04),-2);
	}
	//^3E74:046A
	return bp04.SizeIfAbsent();
}

//^3E74:1586
Bit16u SkWinCore::IS_CLS1_CRITICAL_FOR_LOAD(Bit8u cls1)
{
	//^3E74:1586
	switch (cls1) {
		case 0x1B: // ?
		case GDAT_CATEGORY_CREDITS: // 0x06: // Credits
		case GDAT_CATEGORY_JAPANESE_FONT: // 0x1C: // Kanji table
		case GDAT_CATEGORY_TITLE: // 0x05: // Main Menu and Credits
			//^3E74:15A0
			return 1;
	}
	//^3E74:15A5
	return 0;
}

//^3E74:0472
i32 SkWinCore::QUERY_GDAT_RAW_DATA_FILE_POS(Bit16u index)
{
	//^3E74:0472
	Bit16u di = index;
	Bit32u bp04 = _4976_5d6a;
	Bit16u si = 0;
	//^3E74:048A
	if (di >= _4976_4839) {
		//^3E74:0490
		si = _4976_4839;
		bp04 += _4976_483b;
	}
	//^3E74:04A1
	while (si < di) {
		//^3E74:04A3
		bp04 += QUERY_GDAT_RAW_DATA_LENGTH(si);
		si++;
		//^3E74:04B1
	}
	//^3E74:04B5
	_4976_4839 = di;
	//^3E74:04B9
	_4976_483b = bp04 - _4976_5d6a;
	//^3E74:04CE
	return bp04;
}

//^476D:0005
U16 SkWinCore::FILE_SEEK(Bit16u handle, Bit32u pos) {
	if (fset.fileSeekTo(handle, pos) == pos)
		return 1;
	return 0;
}

U32 SkWinCore::FILE_TELL(Bit16u handle) {
	return fset.fileTell(handle);
}

//^476D:05E3
void SkWinCore::_476d_05e3(U8 *str) { // TODO: Unr
	Unr();
}

//^0CCD:000A
void __DECLSPEC_NORETURN_ SkWinCore::RAISE_SYSERR(Bit16u syserr)
{
	// 30 = invalid header in graphics.dat
	// 31 = no graphic2.dat
	// 32 = corrupted graphic2.dat
	// 41 = no graphics.dat
	// 45 = too many timer entries, you can add no more!
	// 46 = corrupted graphics.dat
	// 50 = no dungeon dat
	// 70 = invalid status on game timer
	// 71 = creature assoc record is full, you cannot add one

	// kkdf2: 1001 = no player startup data in graphics.dat

	// SYSTEM ERROR .Z000.
	static const Bit8u * const strSystemError = (const Bit8u *) 
		"\x53\x59\x53\x54\x45\x4D\x20\x45\x52\x52\x4F\x52\x20\x2E\x5A\x30\x30\x30\x00";
	// SYSTEM ERROR .Z000.
	static const Bit8u * const strSystemErrorF = (const Bit8u *) 
		"\x53\x59\x53\x54\x45\x4D\x20\x45\x52\x52\x4F\x52\x20\x2E\x5A\x30\x30\x30\x00";
	// SYSTEM ERROR .Z000.
	static const Bit8u * const strSystemErrorG = (const Bit8u *) 
		"\x53\x59\x53\x54\x45\x4D\x20\x45\x52\x52\x4F\x52\x20\x2E\x5A\x30\x30\x30\x00";
	
	// 4976:01E6  F2 01 76 49 05 02 76 49 18 02 76 49

	// SPX : _4976_01e6	renamed strSystemErrorTable
	static const Bit8u *strSystemErrorTable[] = {
		strSystemError,
		strSystemErrorF,
		strSystemErrorG,
	};

	//^0CCD:000A
	_4976_4be6 = syserr;
	//^0CCD:0014
	if (_4976_474c != 0) {
		//^0CCD:001B
		FIRE_SELECT_PALETTE_SET(1);
	}
	//^0CCD:0023
	Bit8u bp80[128];
	ATLASSERT(glbLangageLetterIndex < 3U);
	FORMAT_SKSTR(strSystemErrorTable[glbLangageLetterIndex], bp80);


#if UseAltic
	skwin.ShowMessage(reinterpret_cast<const char *>(bp80));
	_exit(1);
#else
	//^0CCD:003F
	if (_4976_4750 != 0) {
		//^0CCD:0046
		if (_4976_4ba4 != 0) {
			//^0CCD:004D
			PRINT_SYSERR_TEXT(0, 16, 15, 0, bp80);
		}
		else {
			//^0CCD:0064
			_476d_05e3(bp80);
		}
		//^0CCD:0070
		if (_4976_474a != 0) {
			//^0CCD:0077
			_476d_050e();
		}
		else {
			//^0CCD:007E
			SLEEP_SEVERAL_TIME(0x012c);
		}
		//^0CCD:0087
		SK_PREPARE_EXIT();
	}
#endif
}

//^3E74:0EA2
void SkWinCore::GRAPHICS_DATA_READ(Bit32u location, Bit32u size, Bit8u *buff)
{
	//^3E74:0EA2
	Bit16u bp14[2] = {glbFileHandleGraphics1};
	Bit32u bp08[2] = {location, 0};
	i32 bp10[2] = {size, 0};
	//^3E74:0ECF
	if (_4976_5c9c != 0) {
		//^3E74:0ED6
		bp14[1] = glbFileHandleGraphics2;
		bp08[1] = location - _4976_5cea;
		//^3E74:0EF0
		if (bp08[1] >= 0) {
			//^3E74:0EFA
			bp10[1] = size;
		}
		else {
			//^3E74:0F08
			bp10[1] = bp08[1] + size;
			//^3E74:0F1A
			if (bp10[1] > 0) {
				//^3E74:0F24
				bp08[1] = 0;
			}
			else {
				//^3E74:0F30
				bp10[1] = 0;
			}
		}
		//^3E74:0F3A
		bp10[0] -= bp10[1];
	}
	//^3E74:0F46
	// spx: si = part file of graphics.dat.
	for (Bit16u si=0; si < 2; si++) {
		//^3E74:0F4B
		if (bp10[si] != 0) {	// check size
			do {
				//^3E74:0F61
				if (FILE_SEEK(bp14[si], bp08[si]) != 0) {
					//^3E74:0F8A
					if (FILE_READ(bp14[si], bp10[si], buff) != 0)
						break;
				}
				//^3E74:0FB9
				GRAPHICS_DATA_CLOSE();
				//^3E74:0FBE
				RAISE_SYSERR((si == 0) ? SYSTEM_ERROR__CORRUPTED_GRAPHICS : SYSTEM_ERROR__CORRUPTED_GRAPHICS_2);
				//^3E74:0FD1
			} while (false);

			buff += bp10[si];
		}
		//^3E74:0FEC
	}
	//^3E74:0FF5
}

//^3E74:0FF8
void SkWinCore::LOAD_GDAT_RAW_DATA(Bit16u index, shelf_memory ps)
{
//LOGX((" + LOAD_GDAT_RAW_DATA loads GDAT item %04d to ptr %08x", index, &ps ));
	//^3E74:0FF8
	Bit16u di = index;
	GRAPHICS_DATA_OPEN();
	//^3E74:1006
	i32 bp04; // file len
	Bit32u bp0c; // file pos
	if (di == 0) {
		//^3E74:100A
		bp04 = _4976_5d7a;
		bp0c = _4976_5d6a - bp04;
	}
	else{
		//^3E74:102C
		bp0c = QUERY_GDAT_RAW_DATA_FILE_POS(di);
		bp04 = QUERY_GDAT_RAW_DATA_LENGTH(di);
	}
	//^3E74:1046
	Bit16u si = bp0c / TEMP_BUFF_SIZE;
	//^3E74:1058
	Bit32u bp08 = ((Bit32u)si) * TEMP_BUFF_SIZE;
	//^3E74:1066
	while (bp04 > 0) {
		//^3E74:1069
		if (si != _4976_480d || _4976_5d34 != 0) {
			//^3E74:1076
			_4976_480d = si;
			//^3E74:107A
			Bit32u bp18 = _4976_5caa - bp08;
			//^3E74:108D
			if (bp18 > TEMP_BUFF_SIZE) {
				bp18 = TEMP_BUFF_SIZE;
			}
			//^3E74:10A3
			GRAPHICS_DATA_READ(bp08, bp18, _4976_5d04);
			//^3E74:10BE
			_4976_5d34 = 0;
		}
		//^3E74:10C4
		i32 bp14 = bp0c - bp08;
		//^3E74:10D6
		Bit32u bp10 = (TEMP_BUFF_SIZE - bp14 > bp04) ? (bp04) : (TEMP_BUFF_SIZE - bp14);
		//^3E74:1106
		COPY_MEMORY(&_4976_5d04[bp14], REALIZE_GRAPHICS_DATA_MEMORY(ps), bp10);
		//^3E74:1136
		bp04 -= bp10;
		bp0c += bp10;
		ps += (bp10);
		//^3E74:114E
		si++;
		bp08 += TEMP_BUFF_SIZE;
		//^3E74:1158
	}
	//^3E74:116C
	GRAPHICS_DATA_CLOSE();
}

//^3E74:0213
shelf_memory SkWinCore::CONVERT_PHYS_TO_SHELF_FORM(Bit8u *buff)
{
	U32ptr p1 = buff -cram;
	if (p1 < sizeof(cram)) {
		shelf_memory ps;
		ps.val = U32(p1);
		return ps;
	}
	U32ptr p2 = buff -cems;
	if (p2 < shelf_memory::SizeOf_cems()) {	// replaced sizeof(cems)
		shelf_memory ps;
		ps.val = 0x00200000U +U32(p2);
		return ps;
	}
	ATLASSERT(false);
	throw int();

//		//^3E74:0213
//		Bit8u *bp04 = buff;
//
//		DOSIMPL(); // TODO: WRONG conversion. Fix me
//
//#pragma warning( push )
//#pragma warning( disable : 4311 4312 )
//		return (Bit8u *)((((Bit32u)bp04 & 0xffff0000) >> 12) + ((Bit16u)bp04 & 0xffff));
//#pragma warning( pop )
}

//^3E74:15A9
Bit8u *SkWinCore::QUERY_GDAT_DYN_BUFF(Bit16u index, Bit16u *yy, Bit16u allocUpper)
{
	//^3E74:15A9
	Bit16u si = index;
	Bit16u di = allocUpper;
	//^3E74:15B5
	if (_4976_5d76 == 0) {
		//^3E74:15BF
		i32 bp08;
		Bit8u *bp04 = ALLOC_MEMORY_RAM(
			bp08 = (QUERY_GDAT_RAW_DATA_LENGTH(si) +6),
			(di != 0) ? afDefault : afUseLower,
			1024
			) + 6;
		//^3E74:15F8
//LOGX(("LOAD_GDAT_RAW_DATA call from QUERY_GDAT_DYN_BUFF (1)"));
		LOAD_GDAT_RAW_DATA(
			si,
			CONVERT_PHYS_TO_SHELF_FORM(bp04)
			);
		SkD((DLV_GLD, "GLD: Dyn2-load Raw#%4d at RAM(%p)\n", (Bitu)si, bp04));
		//^3E74:160E
		WRITE_UI16(bp04,-6 +4,(di != 0) ? afDefault : afUseLower);
		//^3E74:1634
		WRITE_UI32(bp04,-6 +0,bp08);
		//^3E74:1658
		//^3E74:16E3
		return bp04;
	}
	else {
		//^3E74:165B
		Bit8u *bp04;
		if (ADD_CACHE_HASH(si + 0xfffe0000, yy) != 0) {
			//^3E74:1679
			bp04 = reinterpret_cast<Bit8u *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(*yy));
			//^3E74:168B
			if (di == 0)
				return bp04;
			//^3E74:168F
			_3e74_583a(*yy);
		}
		else {
			//^3E74:169D
			bp04 = ALLOC_CPXHEAP_MEM(
				*yy,
				QUERY_GDAT_RAW_DATA_LENGTH(si)
				);
			//^3E74:16BA
//LOGX(("LOAD_GDAT_RAW_DATA call from QUERY_GDAT_DYN_BUFF (2)"));
			LOAD_GDAT_RAW_DATA(
				si,
				CONVERT_PHYS_TO_SHELF_FORM(bp04)
				);
			SkD((DLV_GLD, "GLD: Dyn3-load Raw#%4d at RAM(%p)\n", (Bitu)si, bp04));
			//^3E74:16D0
			if (di == 0) {
				//^3E74:16D4
				_3e74_585a(*yy, 0);
			}
		}
		//^3E74:16E3
		return bp04;
	}
}

//^3E74:521A
Bit8u *SkWinCore::QUERY_GDAT_IMAGE_LOCALPAL(Bit8u cls1, Bit8u cls2, Bit8u cls4)
{
	//^3E74:521A
	Bit16u si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, dtImage, cls4);
	//^3E74:5236
	if (si != 0xffff) {
		//^3E74:523B
		IMG3 *bp04;
		if (glbShelfMemoryTable[si].Absent() != 0) {
			//^3E74:5258
			if (IS_CLS1_CRITICAL_FOR_LOAD(cls1) == 0)
				goto _5276;
			//^3E74:5265
			Bit16u bp06;
			bp04 = reinterpret_cast<IMG3 *>(QUERY_GDAT_DYN_BUFF(si, &bp06, 0));
		}
		else {
			//^3E74:5287
			bp04 = reinterpret_cast<IMG3 *>(REALIZE_GRAPHICS_DATA_MEMORY(glbShelfMemoryTable[si]));
		}
		//^3E74:529F
		//^3E74:52A5
#if DM2_EXTENDED_MODE == 1
		if (bp04->GetBitsCount() != 4) {
			return NULL;
		}
#else
		if (bp04->OffsetY() == -32 && bp04->w4 != 4) {
			//^3E74:52CE
			return NULL;
		}
#endif
		//^3E74:52D4
		PTR_PADA(bp04, QUERY_GDAT_RAW_DATA_LENGTH(si) - 16);
		//^3E74:52EB
		return reinterpret_cast<Bit8u *>(bp04);
	}
_5276:
	//^3E74:5276
	// SPX: Go fetch from the Yukman face (in MISC 0xFE entry) but .. if it does not exist we can get into an infinite loop, fix this!
	if (SkCodeParam::bUseFixedMode && cls1 == GDAT_CATEGORY_MISCELLANEOUS && cls2 == GDAT_ITEM_DEFAULT_INDEX && cls4 == GDAT_ITEM_DEFAULT_INDEX)
		return NULL;
	return QUERY_GDAT_IMAGE_LOCALPAL(GDAT_CATEGORY_MISCELLANEOUS, GDAT_ITEM_DEFAULT_INDEX, GDAT_ITEM_DEFAULT_INDEX);
	// return QUERY_GDAT_IMAGE_LOCALPAL(0x0015, 0x00fe, 0x00fe);
}

//^3E74:0245
U8 *SkWinCore::_3e74_0245(X16 xx, X16 yy)
{
	//^3E74:0245
	ENTER(20);
	//^3E74:024B
	X16 si = xx;
	shelf_memory bp0c = glbShelfMemoryTable[si];
	U8 *bp04 = REALIZE_GRAPHICS_DATA_MEMORY(bp0c);
	i32 bp10 = CONVERT_PHYS_TO_SHELF_FORM(bp04).val - _4976_5d20.val;
	bp04 += 4;
	if (bp10 >= 0 && bp10 <= 65535) {
		//^3E74:02C5
		i32 bp14 = QUERY_GDAT_RAW_DATA_LENGTH(si);
		if (yy != 0)
			_4976_4807 = si;
		//^3E74:02DE
		if (bp14 +(bp10 & 0x7fff) > 0x7fff) {
			//^3E74:0301
			bp14 -= 4;
			bp0c += (4);
			if (_4976_4809 != 0xffff)
				FREE_INDEXED_MEMENT(_4976_4809);
			//^3E74:0322
			_4976_4809 = si;
			mement *bp08 = ALLOC_LOWER_CPXHEAP(bp14 +16);
			X16 di = FIND_FREE_MEMENTI();
			_4976_5c82[si] = di;
			ATLASSERT(tlbMementsPointers[di] == NULL);
			tlbMementsPointers[di] = bp08;
			TEST_MEMENT(bp08);
			bp08->w10(si);
			PTR_PADA(bp08,+12);
			bp04 = reinterpret_cast<U8 *>(bp08);
			//^3E74:038F
			do {
				COPY_MEMORY(REALIZE_GRAPHICS_DATA_MEMORY(bp0c), bp08, (bp14 > 0x4000) ? 0x4000 : bp14);
				bp14 -= 0x4000;
				if (bp14 <= 0)
					break;
				//^3E74:03E2
				PTR_PADA(bp08,+0x4000);
				bp0c += (0x4000);
			} while (true);
			//^3E74:03FC
			if (yy == 0) {
				RECYCLE_MEMENTI(di, 0);
			}
		}
	}
	//^3E74:040C
	return &bp04[6];
}

//^44C8:12F1
X8 SkWinCore::READ_IMG3_NIBBLE()
{
	//^44C8:12F1
	ENTER(0);
	//^44C8:12F4
	return ((_4976_5dc2 & 1) != 0) 
		? (_4976_5e64[(_4976_5dc2++) >> 1] & 15) 
		: (_4976_5e64[(_4976_5dc2++) >> 1] >> 4);
}

//^44C8:132B
U16 SkWinCore::READ_IMG3_DURATION()
{
	//^44C8:132B
	ENTER(0);
	//^44C8:132F
	X16 si = READ_IMG3_NIBBLE();
	if (si == 0xf) {
		//^44C8:133C
		si = READ_IMG3_NIBBLE() << 4;
		si|= READ_IMG3_NIBBLE();
		if (si == 0xff) {
			//^44C8:1358
			si = READ_IMG3_NIBBLE() << 12;
			si|= READ_IMG3_NIBBLE() << 8;
			si|= READ_IMG3_NIBBLE() << 4;
			si|= READ_IMG3_NIBBLE();
			return si;
		}
		si += 0x11;
		return si;
	}
	si += 2;
	return si;
}

//^44C8:12AE
void SkWinCore::SPILL_IMG3_PIXELS(X16 offDst, X16 offSrc, X16 size)
{
	//^44C8:12AE
	ENTER(0);
	//^44C8:12B3
	U8 *di = _4976_5e6a;
	U8 *si = di;
	X16 bx = offDst;
	X16 dx = offSrc;
	X16 cx = size;
	dx >>= 1;
	{
		bool carry = (bx & 1) ? true : false;
		bx >>= 1;
		di += bx;
		si += dx;
		if (carry) {
			//^44C8:12CF
			X8 dl = *di;
			X8 al = lodsb(si);
			dl &= 0xf0;
			al &= 0x0f;
			al |= dl;
			stosb(di, al);
			cx--;
			if (cx == 0)
				return;
		}
		//^44C8:12DF
		cx++;
		cx >>= 1;
		{
			bool carry = (cx & 1) ? true : false;
			cx >>= 1;
			if (carry)
				//^44C8:12E6
				movsb(di, si);
			//^44C8:12E8
			if (cx != 0)
				//^44C8:12EA
				for (; cx != 0; cx--) movsw(di, si);
		}
	}
	//^44C8:12ED
	return;
}

//^44C8:1649
void SkWinCore::TRANSPARENT_IMG3_PIXELS(X16 offSrcDst, X16 size)
{
	//^44C8:1649
	ENTER(0);
	//^44C8:164E
	U8 *di = _4976_5e6a;	// dst
	U8 *si = _4976_5e6e;	// underlay
	X16 bx = offSrcDst;
	X16 cx = size;
	{
		bool carry = (bx & 1) ? true : false;
		bx >>= 1;
		di += bx;
		si += bx;
		if (carry) {
			U8 dl = *di;
			U8 al = lodsb(si);
			dl &= 0xf0;
			al &= 0x0f;
			al |= dl;
			stosb(di, al);
			cx--;
			if (cx == 0)
				return;
		}
		//^44C8:1677
		cx++;
		cx >>= 1;
		{
			bool carry = (cx & 1) ? true : false;
			cx >>= 1;
			if (carry) {
				//^44C8:167E
				movsb(di, si);
				if (cx == 0)
					return;
			}
			//^44C8:1681
			for (; cx != 0; cx--) movsw(di, si);
		}
	}
	//^44C8:1684
	return;
}

//^44C8:1202
void SkWinCore::_44c8_1202(X16 offDst, X8 pixel)
{
	//^44C8:1202
	ENTER(0);
	//^44C8:1206
	U8 *di = _4976_5e6a;
	X16 bx = offDst;
	{
		bool carry = (bx & 1) ? true : false;
		bx >>= 1;
		if (carry) {
			//^44C8:1212
			di += bx;
			U8 al = *di;
			al &= 0xf0;
			al |= pixel;
			stosb(di, al);
		}
		else {
			//^44C8:121F
			di += bx;
			U8 al = (pixel << 4)|(*di & 0x0f);
			stosb(di, al);
		}
	}
	//^44C8:1230
	return;
}

//^44C8:1688
void SkWinCore::DECODE_IMG3_OVERLAY(U8 *blitUnderlay, IMG3 *blitSrc, U8 *blitDst)
{
	// TODO: SUSPECTED;

	//^44C8:1688
	ENTER(24);
	//^44C8:168E
	_4976_5e64 = reinterpret_cast<U8 *>(blitSrc);
	_4976_5e6a = blitDst;
	_4976_5e6e = blitUnderlay;
	X16 bp02 = blitSrc->Width();
	X16 bp04 = blitSrc->Height();
	X16 bp06 = (bp02 +1)&0xfffe;
	X16 bp08 = bp06 * bp04;
	_4976_5dc2 = 8;
	i16 bp0e = 0;
	U8 bp16[5];

	// SPX: on custom GDAT2, it happens images have w = 0 and h = 0; in that case, might be better to do nothing here
	if (SkCodeParam::bUsePowerDebug)
	{
		if (bp02 == 0 || bp04 == 0)
			return;
	}

	for (; bp0e < 5; bp0e++) {
		//^44C8:16E9
		bp16[bp0e] = READ_IMG3_NIBBLE();
		//^44C8:16F8
	}
	//^44C8:1701
	if (bp02 != bp06) {
		//^44C8:170C
		X16 bp0c = bp06 -bp02;
		X16 di = 0;
		X16 bp0a = bp02;
		//^44C8:171D
		do {
			X8 bp0f = ((_4976_5dc2 & 1) != 0)
				? _4976_5e64[_4976_5dc2 >> 1] & 0xf
				: _4976_5e64[_4976_5dc2 >> 1] >> 4;
			_4976_5dc2++;
			X8 bp10 = bp0f & 7;
			U16 si;
			if (bp10 == 6) {
				//^44C8:175F
				si = ((bp0f & 8) != 0) ? READ_IMG3_DURATION() : 1;
				ATLASSERT(di +U32(si) < bp08); // fill range must be inside picture
				//^44C8:1772
				while (si >= bp0a) {
					//^44C8:1772
					SPILL_IMG3_PIXELS(di, di -bp06, bp0a);
					di += bp0a +bp0c;
					si -= bp0a;
					bp0a = bp02;
					//^44C8:1794
				}
				//^44C8:1799
				if (si > 0) {
					//^44C8:17A0
#if UseAltic
					SPILL_IMG3_PIXELS(di, di -bp06, si);
#else
					SPILL_IMG3_PIXELS(di, di -bp06, bp0a);
#endif
					di += si;
					bp0a -= si;
				}
				continue;
			}
			//^44C8:17B9
			if (bp10 == 5) {
				//^44C8:17BF
				si = ((bp0f & 8) != 0) ? READ_IMG3_DURATION() : 1;
				ATLASSERT(di +U32(si) < bp08); // fill range must be inside picture
				//^44C8:17D0
				while (si >= bp0a) {
					//^44C8:17D2
					TRANSPARENT_IMG3_PIXELS(di, bp0a);
					di += bp0a +bp0c;
					si -= bp0a;
					bp0a = bp02;
					//^44C8:17ED
				}
				//^44C8:17F2
				if (si > 0) {
					//^44C8:17F9
#if UseAltic
					TRANSPARENT_IMG3_PIXELS(di, si);
#else
					TRANSPARENT_IMG3_PIXELS(di, bp0a);
#endif
					di += si;
					bp0a -= si;
				}
				continue;
			}
			//^44C8:180B
			X8 bp17;
			if (bp10 < 5) {
				//^44C8:1811
				bp17 = bp16[bp10];
			}
			else {
				bp17 = ((_4976_5dc2 & 1) != 0) ? (_4976_5e64[_4976_5dc2 >> 1] & 0xf) : (_4976_5e64[_4976_5dc2 >> 1] >> 4);
				_4976_5dc2++;
			}
			//^44C8:1859
			si = ((bp0f & 8) != 0) ? READ_IMG3_DURATION() : 1;
			ATLASSERT(di +U32(si) < bp08); // fill range must be inside picture
			//^44C8:186A
			while (si >= bp0a) {
				//^44C8:186C
				FIRE_FILL_4BPP_PICT_LINE(di, bp17, bp0a);
				di += bp0a +bp0c;
				si -= bp0a;
				bp0a = bp02;
				//^44C8:188C
			}
			//^44C8:1891
			if (si > 0) {
				FIRE_FILL_4BPP_PICT_LINE(di, bp17, si);
				di += si;
				bp0a -= si;
			}
			//^44C8:18A7
		} while (di < bp08);
		ATLASSERT(di <= bp08); // if error, memory broken!
		//^44C8:18AF
		return;
	}
	else {
		//^44C8:18B2
		X16 di = 0;
		do {
			//^44C8:18B4
			X8 bp0f = ((_4976_5dc2 & 1) != 0) ? (_4976_5e64[_4976_5dc2 >> 1] & 15) : (_4976_5e64[_4976_5dc2 >> 1] >> 4);
			_4976_5dc2++;
			X8 bp10 = bp0f & 7;
			X8 bp17;
			X16 si;
			//^44C8:18F0
			if (bp10 == 6) {
				si = ((bp0f & 8) != 0) ? READ_IMG3_DURATION() : 1;
				SPILL_IMG3_PIXELS(di, di -bp06, si);
				di += si;
				continue;
			}
			//^44C8:191B
			else if (bp10 == 5) {
				si = ((bp0f & 8) != 0) ? READ_IMG3_DURATION() : 1;
				TRANSPARENT_IMG3_PIXELS(di, si);
				di += si;
				continue;
			}
			//^44C8:193C
			else if (bp10 < 5) {
				bp17 = bp16[bp10];
			}
			else {
				//^44C8:1956
				bp17 = ((_4976_5dc2 & 1) != 0) ? (_4976_5e64[_4976_5dc2 >> 1] & 15) : (_4976_5e64[_4976_5dc2 >> 1] >> 4);
				_4976_5dc2++;
			}
			//^44C8:198A
			if ((bp0f & 8) != 0) {
				//^44C8:1990
				si = READ_IMG3_DURATION();
				FIRE_FILL_4BPP_PICT_LINE(di, bp17, si);
				di += si;
			}
			else {
				//^44C8:19A3
				_44c8_1202(di++, bp17);
			}
			//^44C8:19B1
		} while (di < bp08);
		ATLASSERT(di <= bp08); // if error, memory broken!
	}
	//^44C8:19B9
	return;
}

void SkWinCore::DECODE_IMG9(IMG3 *xx, U8 *pOut) {
	const U32 cx = xx->Width();
	const U32 cy = xx->Height();

	U32 iPix = 0;
	const U32 nPix = cx * cy;

	// a kind of LZSS?
	const U8 typex = reinterpret_cast<U8 *>(xx)[6];

	const U8 *pIn = reinterpret_cast<U8 *>(xx) + 8;

	while (iPix < nPix) {
		U8 command = *pIn;
		pIn++;
		for (int t = 0; t < 8 && iPix < nPix; t++, command >>= 1) {
			if (0 != (command & 1)) {
				// pixel
				pOut[iPix] = *pIn;
				iPix++;
				pIn++;
			}
			else {
				// get back
				int A = pIn[0];
				int B = pIn[1];
				pIn += 2;
				int negative_offset, copy_length;
				if (typex == 2) {
					int Y = A & 0xF;
					int X = A >> 4;
					negative_offset = X + 16 * B;
					copy_length = Y + 3;
				}
				else { // typex == 3
					int Y = A & 0x1F;
					int X = A >> 5;
					negative_offset = X + 8 * B;
					copy_length = Y + 3;
				}
				for (int w = 0; w < copy_length; w++, iPix++) {
					pOut[iPix] = pOut[iPix -negative_offset];
				}
			}
		}
	}
	ATLASSERT(iPix == nPix);
}

//^44C8:1395
void SkWinCore::DECODE_IMG3_UNDERLAY(IMG3 *xx, U8 *yy)
{
	//^44C8:1395
	ENTER(24);
	//^44C8:139B
	_4976_5e64 = reinterpret_cast<U8 *>(xx);
	_4976_5e6a = yy;
	X16 bp02 = xx->Width();
	X16 bp04 = xx->Height();
	X16 bp06 = (bp02 +1)&0xfffe;
	X16 bp08 = bp06 * bp04;
	_4976_5dc2 = 8;
	X16 bp0e = 0;
	X8 bp16[6];

	// SPX: on custom GDAT2, it happens images have w = 0 and h = 0; in that case, might be better to do nothing here
	if (SkCodeParam::bUsePowerDebug)
	{
		if (bp02 == 0 || bp04 == 0)
			return;
	}

	while (bp0e < 6) {
		//^44C8:13E9
		bp16[bp0e] = READ_IMG3_NIBBLE();
		bp0e++;
		//^44C8:13FB
	}
	//^44C8:1401
	X16 si = 0;	// SPX: added init to zero
	U16 di = 0; // SPX: added init to zero
	if (bp02 != bp06) {
		//^44C8:140C
		X16 bp0c = bp06 -bp02;
		si = 0;
		X16 bp0a = bp02;
		do {
			//^44C8:141D
			X8 bp0f = ((_4976_5dc2 & 1) != 0) ? (_4976_5e64[_4976_5dc2 >> 1] & 15) : (_4976_5e64[_4976_5dc2 >> 1] >> 4);
			_4976_5dc2++;
			X8 bp10 = bp0f & 7;
			if (bp10 == 6) {
				//^44C8:145F
				di = ((bp0f & 8) != 0) ? (READ_IMG3_DURATION()) : 1;
				//^44C8:1470
				while (di >= bp0a) {
					//^44C8:1472
					SPILL_IMG3_PIXELS(si, si -bp06, bp0a);
					si += bp0a +bp0c;
					di -= bp0a;
					bp0a = bp02;
					//^44C8:1494
				}
				//^44C8:1499
				if (di > 0) {
					//^44C8:14A0
					SPILL_IMG3_PIXELS(si, si -bp06, bp0a);
					si += di;
					bp0a -= di;
				}
			}
			else {
				//^44C8:14B9
				X8 bp17;
				if (bp10 < 6) {
					//^44C8:14BF
					bp17 = bp16[bp10];
				}
				else {
					//^44C8:14D3
					bp17 = ((_4976_5dc2 & 1) != 0) ? (_4976_5e64[_4976_5dc2 >> 1] & 15) : (_4976_5e64[_4976_5dc2 >> 1] >> 4);
					_4976_5dc2++;
				}
				//^44C8:1507
				di = ((bp0f & 8) != 0) ? READ_IMG3_DURATION() : 1;
				//^44C8:1518
				while (di >= bp0a) {
					//^44C8:151A
					FIRE_FILL_4BPP_PICT_LINE(si, bp17, bp0a);
					si += bp0a +bp0c;
					di -= bp0a;
					bp0a = bp02;
					//^44C8:153A
				}
				//^44C8:153F
				if (di > 0) {
					FIRE_FILL_4BPP_PICT_LINE(si, bp17, di);
					si += di;
					bp0a -= di;
				}
			}
			//^44C8:1555
		} while (si < bp08);
		if (SkCodeParam::bDebugNoImageDecodingAssert == false)
			ATLASSERT(si <= bp08); // if error, memory broken!
		//^44C8:155D
	}
	else {
		//^44C8:1560
		si = 0;
		do {
			//^44C8:1562
			// SPX: _4976_5e64 would be an image and _4976_5dc2 the cursor ?
			X8 bp0f = ((_4976_5dc2 & 1) != 0) ? (_4976_5e64[_4976_5dc2 >> 1] & 0xf) : (_4976_5e64[_4976_5dc2 >> 1] >> 4);
			_4976_5dc2++;
			X8 bp10 = bp0f & 7;
			X8 bp17;
			if (bp10 == 6) {
				//^44C8:15A4
				di = ((bp0f & 8) != 0) ? READ_IMG3_DURATION() : 1;
				SPILL_IMG3_PIXELS(si, si -bp06, di);
				//^44C8:15C1
				si += di;
				continue;
			}
			else if (bp10 < 6) {
				//^44C8:15CF
				bp17 = bp16[bp10];
			}
			else {
				bp17 = ((_4976_5dc2 & 1) != 0) ? (_4976_5e64[_4976_5dc2 >> 1] & 15) : (_4976_5e64[_4976_5dc2 >> 1] >> 4);
				_4976_5dc2++;
			}
			//^44C8:1617
			if ((bp0f & 8) != 0) {
				di = READ_IMG3_DURATION();
				FIRE_FILL_4BPP_PICT_LINE(si, bp17, di);
				si += di;
			}
			else {
				//^44C8:162F
				_44c8_1202(si++, bp17);
			}
			//^44C8:163D
		} while (si < bp08);
		if (SkCodeParam::bDebugNoImageDecodingAssert == false)
			ATLASSERT(si <= bp08); // if error, memory broken!
	}

	//^44C8:1645
	return;
}

//^44C8:19BD
void SkWinCore::DECODE_IMG3_UNDERLAY_LOCAL(IMG3 *xx, U8 *yy)
{
	//^44C8:19BD
	ENTER(0);
	//^44C8:19C0
	DECODE_IMG3_UNDERLAY(xx, yy);
	//^44C8:19D3
	return;
}

//^3E74:4B48
Bit8u *SkWinCore::EXTRACT_GDAT_IMAGE(Bit16u index, i16 allocUpper)
{
	SkD((DLV_DBG_GETPIC,"DBG: EXTRACT_GDAT_IMAGE(%4u,%u)\n", (Bitu)index, (Bitu)allocUpper));

	if (SkCodeParam::bUseFixedMode && index >= 65535)
		return NULL;

	//^3E74:4B48
	Bit16u di = index;
	i16 bp16 = -1;
	//^3E74:4B56
	if (_4976_5d76 != 0) {
		//^3E74:4B5D
		Bit16u si = QUERY_MEMENTI_FROM(di);
		//^3E74:4B65
		if (si != 0xffff) {
			mement *bp04;
			//^3E74:4B6D
			if (allocUpper != 0) {
				//^3E74:4B73
				_3e74_4549(si);
				//^3E74:4B79
				bp04 = tlbMementsPointers[si];
			}
			else {
				//^3E74:4B93
				bp04 = _3e74_48c9(si);
			}
			//^3E74:4B9F
			return reinterpret_cast<U8 *>(&bp04[1]); // +18 bytes
		}
	}
	else {
		//^3E74:4BB4
		sk5cfc_image *bp04 = _4976_5cfc.pv0;
		while (bp04 != NULL) {
			//^3E74:4BBD
			if (bp04->w4 == di) {
				//^3E74:4BD6
				return (Bit8u *)&bp04[1]; // +14 bytes
			}
			//^3E74:4BEB
			bp04 = bp04->pv0;
			//^3E74:4BF5
		}
	}
	//^3E74:4C0C
	Bit16u bp1e = 0;
	Bit8u *bp1a;
	Bit16u bp1c;
	if (_4976_5d78 != 0) {
		//^3E74:4C18
		bp1c = TRACK_UNDERLAY(di);
		SkD((DLV_DBG_GETPIC,"DBG: Ulay %4d -> %4d\n", (Bitu)i16(bp1c), (Bitu)i16(di)));
		//^3E74:4C21
		if (bp1c != 0xffff) {
			//^3E74:4C26
			bp1e = 1;
			//^3E74:4C2B
			bp1a = EXTRACT_GDAT_IMAGE(bp1c, (_4976_5d76 != 0) ? 1 : (!allocUpper));
		}
	}
	//^3E74:4C4F
	shelf_memory bp10 = glbShelfMemoryTable[di];
	IMG3 *bp08;
	//^3E74:4C67
	if (bp10.Absent()) {
		//^3E74:4C72
		bp08 = reinterpret_cast<IMG3 *>(QUERY_GDAT_DYN_BUFF(di, reinterpret_cast<U16 *>(&bp16), (_4976_5d76 != 0) ? 1 : (!allocUpper)));
	}
	else {
		//^3E74:4C96
		bp08 = reinterpret_cast<IMG3 *>(REALIZE_GRAPHICS_DATA_MEMORY(bp10));
	}
	//^3E74:4CA2
	//^3E74:4CA8
	if (bp08->OffsetY() == -32) { // uncompressed 4bpp/8bpp. but 8bpp is untested!
		//^3E74:4CB7
		if (bp16 < 0) {
			//^3E74:4CBD
			//^3E74:4CC7
			Bit8u *bp04 = _3e74_0245(di, allocUpper);
			//^3E74:4FB4
			return bp04;
		}
		else {
			//^3E74:4CD0
			//^3E74:4CC7
			Bit8u *bp04 = PTR_PADD(bp08,+10);
			//^3E74:4FB4
			return bp04;
		}
	}
	//^3E74:4CE4
	U16 bp12 = bp08->Width();
	U16 bp14 = bp08->Height();
#if DM2_EXTENDED_MODE == 1
	i32 bp0c = bp08->GetImageSize();
#else
	i32 bp0c = (((bp12 +1) & 0xFFFE) >> 1) * bp14;
#endif
	//^3E74:4D0B
	if (_4976_5d76 == 0) {
		//^3E74:4D12
		bp0c += 16;
	}
	//^3E74:4D1A
	Bit16u si;
	U8 *_bp04;
	if (_4976_5d76 != 0) {
		//^3E74:4D24
		if ((bp0c & 1) != 0) {
			//^3E74:4D2F
			bp0c++;
		}
		//^3E74:4D37
		bp0c += sizeof(mement) + sizeof(i32);
		//^3E74:4D3F
		mement *bp04 = ALLOC_LOWER_CPXHEAP(bp0c);
		//^3E74:4D51
		if (bp16 >= 0) {
			//^3E74:4D57
			bp08 = reinterpret_cast<IMG3 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp16));
		}
		//^3E74:4D66
		si = FIND_FREE_MEMENTI();
		//^3E74:4D6C
		_4976_5c82[di] = si;
		//^3E74:4D79
		ATLASSERT(tlbMementsPointers[si] == NULL);
		tlbMementsPointers[si] = bp04;
		//^3E74:4D91
		bp04->w10(di);
		//^3E74:4D98
#if DM2_EXTENDED_MODE == 1
		bp04->w12(bp08->GetBitsCount());
#else
		bp04->w12(4);
#endif
		//^3E74:4D9E
		bp04->w14(bp12);
		//^3E74:4DA5
		bp04->w16(bp14);
		//^3E74:4DAC
		bp04++; // +18 bytes
		_bp04 = reinterpret_cast<U8 *>(bp04);

		// SkD((DLV_DBG_GETPIC,"DBG: CPX Alloc #%02d (Raw #%04d)\n", si, di));
	}
	else {
		//^3E74:4DC7
		sk5cfc_image *bp04 = reinterpret_cast<sk5cfc_image *>(ALLOC_MEMORY_RAM(bp0c +sizeof(sk5cfc_image), (allocUpper != 0) ? afDefault : afUseLower, 8));
		//^3E74:4DF2
		bp04->pv0 = _4976_5cfc.pv0;
		//^3E74:4E03
		_4976_5cfc.pv0 = bp04;
		//^3E74:4E10
		bp04++;
		//^3E74:4E28
		bp04[-1].w6 = (allocUpper != 0) ? 0 : 2;
		//^3E74:4E50
		bp04[-1].w4 = di;
		//^3E74:4E69
#if DM2_EXTENDED_MODE == 1
		bp04[-1].w8 = bp08->GetBitsCount();
#else
		bp04[-1].w8 = 4;
#endif
		//^3E74:4E83
		bp04[-1].width = bp12;
		bp04[-1].height = bp14;
		//^3E74:4E94
		COPY_MEMORY(
			PTR_PADD(bp08,+QUERY_GDAT_RAW_DATA_LENGTH(di) -16),
			PTR_PADD(bp04,+bp0c -16),
			16
			);
		_bp04 = reinterpret_cast<U8 *>(bp04);
	}
	//^3E74:4EE0
	Bit16u bp20;
	if (bp1e != 0) {
		//^3E74:4EE6
		if (_4976_5d76 != 0) {
			//^3E74:4EED
			bp20 = QUERY_MEMENTI_FROM(bp1c);
			//^3E74:4EF8
			bp1a = reinterpret_cast<U8 *>(&tlbMementsPointers[bp20][1]); // +18 bytes
		}
		//^3E74:4F1A
		DECODE_IMG3_OVERLAY(bp1a, bp08, _bp04);
	}
	else {
#if DM2_EXTENDED_MODE == 1
		switch (bp08->Getpf()) {
			case pfC8:
				DECODE_IMG9(bp08, _bp04);
				break;
			case pfC4:
				DECODE_IMG3_UNDERLAY_LOCAL(bp08, _bp04);
				break;
		}
#else
		//^3E74:4F36
		DECODE_IMG3_UNDERLAY_LOCAL(bp08, _bp04);
#endif
	}
	//^3E74:4F4A
	if (_4976_5d76 != 0) {
		//^3E74:4F51
		if (allocUpper == 0) {
			//^3E74:4F57
			RECYCLE_MEMENTI(si, 0);
		}
		//^3E74:4F60
		if (bp16 >= 0) {
			//^3E74:4F66
			_3e74_585a(bp16, 0);
		}
		//^3E74:4F72
		if (bp1e == 0)
			return _bp04;
		//^3E74:4F78
		RECYCLE_MEMENTI(bp20, 0);
		//^3E74:4F83
		return _bp04;
	}
	//^3E74:4F85
	if (bp10.Absent()) {
		//^3E74:4F96
		_3e74_0a77(reinterpret_cast<U8 *>(bp08));
	}
	//^3E74:4FA2
	if (bp1e != 0) {
		//^3E74:4FA8
		FREE_PICT_ENTRY(bp1a);
	}
	//^3E74:4FB4
	return _bp04;
}

void SkWinCore::DUMP_5CA4() {
#if DLV_DBG_EXTRACT
	SkD((DLV_DBG_EXTRACT,
		"=====\n"
		"_4976_5c8c=%p\n"
		"_4976_5cb2=%p\n"
		"_4976_5ce6=%p\n"
		"_4976_5d5e=%p\n"
		"_4976_5d70=%p\n"
		"_4976_5d90=%p\n"
		"_4976_5d94=%p\n"
		, _4976_5c8c
		, _4976_5cb2
		, _4976_5ce6
		, _4976_5d5e
		, _4976_5d70
		, _4976_5d90
		, _4976_5d94
		));
	for (int x=0; x<glbNumberOfMements; x++) {
		if (tlbMementsPointers[x] == NULL) continue;
		SkD((DLV_DBG_EXTRACT,
			"#%02d %p (%6d,%04X,%04X,%04X,%04X,%04X,%04X,%04X)\n"
			, x, tlbMementsPointers[x]
			, tlbMementsPointers[x]->_dw0
			, tlbMementsPointers[x]->_w4
			, tlbMementsPointers[x]->_w6
			, tlbMementsPointers[x]->_w8
			, tlbMementsPointers[x]->_w10
			, tlbMementsPointers[x]->_w12
			, tlbMementsPointers[x]->_w14
			, tlbMementsPointers[x]->_w16
			));
	}
	SkD((DLV_DBG_EXTRACT,
		"=====\n"));
#endif
}

//^3E74:4FBE
Bit8u *SkWinCore::QUERY_GDAT_IMAGE_ENTRY_BUFF(Bit8u cls1, Bit8u cls2, Bit8u cls4)
{
	SkD((DLV_DBG_GETPIC,"DBG: QUERY_GDAT_IMAGE_ENTRY_BUFF(%02X,%02X,%02X)\n", (Bitu)cls1, (Bitu)cls2, (Bitu)cls4));
	DUMP_5CA4();
	// ATLASSERT(!(cls1 == 1 && cls2 == 2 && cls4 == 0));
	// ATLASSERT(!(cls1 == 7 && cls2 == 0 && cls4 == 0));
	// ATLASSERT(!(cls1 == 10 && cls2 == 0x37 && cls4 == 2));

	//^3E74:4FBE
	Bit16u di = 0;
	if (glbGameTick != _4976_5d2a) {
		//^3E74:4FD9
		_3e74_44ad();
	}
	//^3E74:4FDD
	RawEntry *bp08 = QUERY_GDAT_ENTRYPTR(cls1, cls2, 1, cls4);
	Bit16u si;
	if (bp08 == NULL) {
		//^3E74:5000
		si = 0xffff;
	}
	else {
		//^3E74:5005
		di |= IS_CLS1_CRITICAL_FOR_LOAD(cls1);
		//^3E74:500E
		si = bp08->data & 0x7fff;
	}
	//^3E74:501C
	if (si != 0xffff) {
		if (glbShelfMemoryTable[si].Present() || (di != 0)) {
			//^3E74:5055
			Bit8u *bp04 = EXTRACT_GDAT_IMAGE(si, 0);
			//^3E74:5064
			return bp04;
		}
	}
	SkD((DLV_BUGHERE,"BUG? Image (%02X,%02X,%02X) not found. We just supply a \":P\" icon\n"
		, (Bitu)cls1, (Bitu)cls2, (Bitu)cls4));
	//^3E74:5042
	// SPX: the default image (yukman) is located as default image from MISC ITEM category
	// If that default image is not here, it is very likely to crash thereafter (anytime the default is required)
	si = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_MISCELLANEOUS, GDAT_ITEM_DEFAULT_INDEX, 0x01, 0xfe); // (0x15, 0xfe, 0x01, 0xfe) // the Yukman :P icon
	//^3E74:5055
	Bit8u *bp04 = EXTRACT_GDAT_IMAGE(si, 0);
	//^3E74:5064
	return bp04;
}

//^0B36:1599
void SkWinCore::DRAW_DIALOGUE_PICT(Bit8u *srcImage, Bit8u *dstImage, SRECT *rect, Bit16u srcx, Bit16u srcy, Bit16u colorkey, Bit8u localpal[16])
{
	// draw the:
	// a) pre rendered dialogue to screen

	//^0B36:1599
	FIRE_BLIT_PICTURE(
		srcImage,
		dstImage,
		rect,
		srcx,
		srcy,
		((Bit16u *)srcImage)[-2], // src-pitch
		((Bit16u *)dstImage)[-2], // dst-pitch
		colorkey,
		0,
		((Bit16u *)srcImage)[-3], // src-bpp
		((Bit16u *)dstImage)[-3], // dst-bpp
		localpal
		);
}

//^2E62:0004
Bit8u *SkWinCore::QUERY_GDAT_SQUAD_ICON(Bit8u *dstImage, Bit8u colorno, Bit8u localpal[16])
{
	//^2E62:0004
	Bit16u si = (((glbChampionSquad[colorno].playerDir() + 4 - glbPlayerDir) & 3) + ((glbGlobalSpellEffects.Invisibility != 0) ? 4 : 0)) * _4976_0118;
	//^2E62:003C
	COPY_MEMORY(
		QUERY_GDAT_IMAGE_LOCALPAL(1, 6, colorno),
		localpal,
		16
		);
	//^2E62:0060
	DRAW_DIALOGUE_PICT(
		QUERY_GDAT_IMAGE_ENTRY_BUFF(1, 6, colorno),
		dstImage,
		ALLOC_TEMP_ORIGIN_RECT(_4976_0118, _4976_011a),
		si,
		0,
		2,
		NULL
		);
	//^2E62:009A
	return dstImage;
}

//^3E74:0A1B
void SkWinCore::DEALLOC_UPPER_MEMORY(Bit32u size)
{
	//^3E74:0A1B
	if ((size & 1) != 0) {
		//^3E74:0A29
		size++;
	}
	//^3E74:0A31
	glbFreeRAMMemPool += size;
	//^3E74:0A3F
	_4976_5cf4 += size;

	SkD((DLV_MEM, "MEM: DEALLOC_UPPER_MEMORY(%10d)\n", (Bitu)size));
#if defined(_USE_MFC80) || defined(_USE_MFC60) || defined(_USE_SDL)
	i32 cbExpected;
	ATLVERIFY(vecUpperAlloc.size() != 0);
	ATLVERIFY((cbExpected = vecUpperAlloc.top()) == size);
	vecUpperAlloc.pop();
#endif // defined(_USE_MFC80) || defined(_USE_MFC60) || defined(_USE_SDL)
}

//^3E74:0C16
void SkWinCore::FREE_PICT_BUFF(Bit8u *buff)
{
	// READ_UI16(buff,-6) -> bpp(4 or 8)
	// READ_UI16(buff,-4) -> width
	// READ_UI16(buff,-2) -> height

	//^3E74:0C16
	DEALLOC_UPPER_MEMORY(
		(	(READ_UI16(buff,-6) == 4) 
			? (((READ_UI16(buff,-4) +1) & 0xfffe) >> 1)	// if 4 bpp, round up width to byte align.
			:   (READ_UI16(buff,-4)     & 0xffff)		// if 8 bpp, already byte aligned.
		) * READ_UI16(buff,-2) +6
	);
}


//^443C:04B6
void SkWinCore::CHANGE_PLAYER_POS(Bit16u squadPos)
{
	//^443C:04B6
	Bit16u di = 1;
	//^443C:04BF
	if ((squadPos & 0x8000) != 0) {
		//^443C:04C6
		di = 0;
		squadPos &= 3;
	}
	//^443C:04CC
	i16 si = GET_PLAYER_AT_POSITION((squadPos + glbPlayerDir) & 3);
	//^443C:04DF
	if (_4976_5dbc == 0) {
		//^443C:04E9
		if (si < 0)
			return;
		//^443C:04F0
		_4976_495c.cx = _4976_0118;
		_4976_495c.cy = _4976_011a;
		//^443C:04FC
		Bit8u *bp08 = ALLOC_PICT_BUFF(_4976_0118, _4976_011a, afDefault, 4);
		Bit8u *bp0c = ALLOC_PICT_BUFF(_4976_0118, _4976_011a, afDefault, 8);
		//^443C:0530
		Bit8u bp1c[16];
		QUERY_GDAT_SQUAD_ICON(bp08, (Bit8u)si, bp1c);
		//^443C:0544
		FILL_ENTIRE_PICT(bp0c, 0x000c);
		//^443C:0554
		FIRE_BLIT_PICTURE(
			bp08,
			bp0c,
			&_4976_495c,
			0,
			0,
			_4976_0118,
			_4976_0118,
			4,
			0,
			4,
			8,
			bp1c
			);
		//^443C:0585
		FIRE_HIDE_MOUSE_CURSOR();
		//^443C:058A
		IBMIO_SET_CURSOR_PATTERN(3, bp0c, 8, 6, _4976_0118, _4976_011a, 8, NULL, bp1c[12]) CALL_IBMIO;
		//^443C:05B5
		_01b0_0c70(3) CALL_IBMIO;
		//^443C:05C0
		FREE_PICT_BUFF(bp0c);
		//^443C:05CD
		FREE_PICT_BUFF(bp08);
		//^443C:05DA
		_4976_5dba = squadPos;
		_4976_5dbc = squadPos +1;
		//^443C:05E7
		FIRE_SHOW_MOUSE_CURSOR();
	}
	else {
		//^443C:05EE
		Bit16u bp02 = _4976_5dbc -1;
		_4976_5dbc = 0;
		//^443C:05FB
		Bit16u bp04 = GET_PLAYER_AT_POSITION((bp02 + glbPlayerDir) & 0x0003);
		//^443C:060C
		if (di != 0) {
			//^443C:0610
			glbChampionSquad[bp04].playerDir((Bit8u)glbPlayerDir);
		}
		//^443C:061F
		if (bp02 != squadPos) {
			//^443C:0627
			if (si >= 0) {
				//^443C:062B
				glbChampionSquad[si].playerPos((Bit8u)bp02);
			}
			//^443C:0642
			glbChampionSquad[bp04].playerPos((Bit8u)(squadPos + glbPlayerDir));
		}
	}
	//^443C:065A
	_443c_040e();
}



//^38C8:0002
void SkWinCore::_38c8_0002()
{
	//^38C8:0002
	//^38C8:0005
	if (_4976_5bec == 0 && glbGameHasEnded == 0) {
		//^38C8:0013
		_4976_5bec = 1;
		if (_4976_5dbc != 0) {
			//^38C8:0020
			CHAMPION_SQUAD_RECOMPUTE_POSITION();
			UPDATE_RIGHT_PANEL(0);
		}
		//^38C8:002D
		_1031_04f5();
		FIRE_HIDE_MOUSE_CURSOR();
		_12b4_0092();
		//^38C8:003C
		if (glbChampionInventory == 0) {
			//^38C8:0043
			FIRE_FILL_HALFTONE_RECTI(0x09, 0);
		}
		//^38C8:004E
		FIRE_FILL_HALFTONE_RECTI(0x0B, 0);
		FIRE_SHOW_MOUSE_CURSOR();
	}
	//^38C8:005E
	return;
}

//^2636:02F8
Bit8u *SkWinCore::QUERY_GDAT_TEXT(Bit8u cls1, Bit8u cls2, Bit8u cls4, Bit8u *buff)
{
	//^2636:02F8
	*buff = 0;
	Bit16u di = 0;
	Bit16u si = 0;
	//^2636:030B
	Bit8u *bp0e = QUERY_GDAT_ENTRY_DATA_BUFF(cls1, cls2, dtText, cls4);
	//^2636:0327
	if (bp0e != NULL) {
		//^2636:0348
		Bit8u *bp0a = ALLOC_MEMORY_RAM(
			di = si = QUERY_GDAT_ENTRY_DATA_LENGTH(cls1, cls2, dtText, cls4),
			0,
			1024
			);
		//^2636:0358
		COPY_MEMORY(bp0e, bp0a, si);
		//^2636:0377
		if (glbTextEntryEncoded != 0) {
			//^2636:037E
			Bit8u *bp04 = bp0a;
			Bit8u bp05 = 0;
			//^2636:038E
			while (di-- != 0) {
				//^2636:0390
				*bp04 = (*bp04 ^ 0xFF) - bp05;
				//^2636:039E
				bp05++;
				bp04++;
				//^2636:03A4
			}
		}
		//^2636:03AB
		FORMAT_SKSTR(bp0a, buff);
		//^2636:03BE
		DEALLOC_UPPER_MEMORY(si);
	}
	//^2636:03CA
	return buff;
}

//^0AAF:0067
Bit8u SkWinCore::_0aaf_0067(Bit8u cls2)
{
	//^0AAF:0067
	Bit16u di = 0;
	Bit16u si = 0xffff;
	Bit16u bp08 = 0;
	//^0AAF:0077
	Bit16u bp38[21];
	ZERO_MEMORY(&bp38[1], 2*20);
	//^0AAF:0088
	Bit8u bp5e[38];
	for (Bit8u bp0d = 0; bp0d < 0x14; bp0d++) {
		//^0AAF:008F
		if (*QUERY_GDAT_TEXT(0x1a, cls2, bp0d, (Bit8u *)bp5e) != 0) {
			//^0AAF:00B0
			Bit16u bp0a = QUERY_GDAT_ENTRY_DATA_INDEX(0x1a, cls2, dtWordValue, bp0d);
			//^0AAF:00C7
			Bit8u *bp04 = (Bit8u *)&bp38[1 + di];
			*bp04 = (Bit8u)bp0a;
			//^0AAF:00E2
			if (*bp04 == 0) {
				//^0AAF:00E6
				*bp04 = bp0d;
			}
			//^0AAF:00EF
			bp04[1] = (Bit8u)(bp0a >> 8);
			//^0AAF:00FC
			if (bp04[1] != 0) {
				//^0AAF:0100
				si = bp04[1];
				bp08 = bp04[0];
			}
			//^0AAF:0110
			di++;
		}
		//^0AAF:0111
	}
	//^0AAF:011D
	_4976_4bd2 = di;
	//^0AAF:0121
	if (si == 0xffff && di == 1) {
		//^0AAF:012B
		si = 1;
	}
	//^0AAF:012E
	_1031_0675(4);
	//^0AAF:0136
	Bit16u bp06;
	for (bp06 = 0; glbMouseVisibility > 0; bp06++) {
		//^0AAF:013D
		FIRE_SHOW_MOUSE_CURSOR();
		//^0AAF:0142
	}
	//^0AAF:014C
	_4976_4dfc = 0x00ff;

	do {
		MessageLoop(false);

		//^0AAF:0152
		MAIN_LOOP();
		WAIT_SCREEN_REFRESH();
		//^0AAF:015C
		if (si != 0xffff) {
			//^0AAF:0161
			if (_476d_04ed(si) != 0) {
				//^0AAF:016C
				_1031_0781(bp08 + 0x00db);
			}
		}
		//^0AAF:0179
		if (_4976_4dfc == 0x00ff && _476d_05a9() != 0 && _476d_050e() == 0x001c) {
			//^0AAF:0194
			_1031_0781(0x00db);
		}
		//^0AAF:019D
	} while (_4976_4dfc == 0x00ff);

	//^0AAF:01A5
	Bit8u bp0c = (Bit8u)bp38[_4976_4dfc];
	//^0AAF:01B8
	while ((bp06--) != 0) {
		//^0AAF:01BA
		FIRE_HIDE_MOUSE_CURSOR();
		//^0AAF:01BF
	}
	//^0AAF:01C9
	_4976_022c = 1;
	//^0AAF:01CF
	_1031_06a5();
	//^0AAF:01D4
	return bp0c;
}

//^2066:03E0
Bit16u SkWinCore::_2066_03e0(Bit16u xx)
{
	//^2066:03E0
	Bit16u di = xx;
	Bit8u bp01 = 0;
	//^2066:03ED
	if (_4976_499e != 0) {
		//^2066:03F4
		return 1;
	}
	//^2066:03F9
	Bit16u si = 1;
	//^2066:03FC
	if (_476d_030a(1) == 0) {
		//^2066:0408
		if (di != 2)
			return si;
	}
	//^2066:040D
	_4976_52f4 = 1;
	//^2066:0413
	while (di != 0 || _476d_030a(1) != 0) {
		//^2066:0415
		_38c8_0002();
		//^2066:041A
		di = si = 0;
		//^2066:0420
		_0aaf_0067(_0aaf_02f8_DIALOG_BOX((_4976_5c9c != 0) ? ((_4976_5ca8 != 0) ? (0x13) : (0x14)) : (0x07), bp01 = _476d_04e8(1)));
		//^2066:0453
		bp01 = 0x14;
		//^2066:0457
	}
	//^2066:0467
	return si;
}

//^2066:0002
void SkWinCore::SUPPRESS_INIT()
{
	//^2066:0002
	_4976_524e = _4976_5254 = 0;
}


//^2636:000C
//SPX: This serves for Z000 variable replacement
Bit8u *SkWinCore::FORMAT_SKSTR(const Bit8u *format, Bit8u *output)
{
	//^2636:000C
	Bit8u *bp04;
	Bit8u *bp08;
	if (output != NULL) {
		//^2636:001A
		bp04 = bp08 = output;
	}
	else {
		//^2636:002F
		bp04 = bp08 = _4976_52ea[_4976_52f8];
		//^2636:004A
		_4976_52f8++;
		//^2636:004E
		if (_4976_52f8 >= 2) {
			//^2636:0059
			_4976_52f8 = 0;
		}
		//^2636:005F
	}
	Bit8u bp11;
	while ((bp11 = *format) != 0) {
		//^2636:0062
		Bit8u bp0116[128];
		if ((bp11 == '.' && format[1] == 'Z') || bp11 == 0x01) {
			i16 si;
			//^2636:007B
			if (bp11 == 0x01) {
				//^2636:0081
				format++;
				si = *format - 32;
				format++;
			}
			else {
				//^2636:0095
				format += 2;
				si = 0;
				Bit16u di = 0;
				//^2636:009D
				while (di < 3) {
					//^2636:009F
					si = si * 10 + (*format) - '0';
					format++;
					di++;
					//^2636:00BA
				}
			}
			//^2636:00BF
			*bp04 = 0;
			Bit8u bp0096[128];
			Bit8u *bp0c = bp0096;
			Bit8u bp12 = 1;
			switch (si) {
				case 0x0000:	// .Z000
					{
						//^2636:00E5
						Bit32u bp10 = _4976_4be6;
						//^2636:0132
						SK_LTOA10(bp10, bp0c);
						break;
					}
				case 0x0001:	// .Z001
					{
						//^2636:00F2
						//^2636:012C
						Bit32u bp10 = ((-_4976_4742) + 1023) >> 10;
						//^2636:0132
						SK_LTOA10(bp10, bp0c);
						break;
					}
				case 0x0002:	// .Z002
					{
						//^2636:0149
						Bit8u bp13 = 0xFE;
						Bit8u bp14 = 0x00;
						//^2636:0161
						// SPX: would be 01/FE/00 = SKULLKEEP ?
						QUERY_GDAT_TEXT(bp12, bp13, bp14, bp0c);
						break;
					}
				case 0x0003:	// .Z003
					{
						//^2636:01AF
						//^2636:0244
						const Bit8u *bp0c = _4976_498e;
						//^2636:024A
						FORMAT_SKSTR(bp0c, bp0116);
						//^2636:025D
						SK_STRCAT(bp08, bp0116);
						//^2636:0271
						bp04 = bp08 +SK_STRLEN(bp08);
						continue;
					}
				case 0x0004:	// .Z004
					{
						//^B9 01
						//^2636:0244
						const Bit8u *bp0c = _4976_4992;
						//^2636:024A
						FORMAT_SKSTR(bp0c, bp0116);
						//^2636:025D
						SK_STRCAT(bp08, bp0116);
						//^2636:0271
						bp04 = bp08 +SK_STRLEN(bp08);
						continue;
					}
				case 0x0005:
					break;
				case 0x0006:
					break;
				case 0x0007:	// .Z007
					{
						//^2636:017E
						if (glbChampionBonesIndex == 0xFFFF)
							break;
						//^2636:0188
						bp0c = (Bit8u *)&glbChampionSquad[glbChampionBonesIndex];
						break;
					}
				case 0x0008:	// .Z008
					{
						//^2636:01C3
						//^2636:0244
						const Bit8u *bp0c = ptrDirLetter;
						//^2636:024A
						FORMAT_SKSTR(bp0c, bp0116);
						//^2636:025D
						SK_STRCAT(bp08, bp0116);
#ifdef __DJGPP__
						SK_STRCAT(bp08, (const Bit8u *) "\\");
#endif
						//^2636:0271
						bp04 = bp08 +SK_STRLEN(bp08);
						continue;
					}
				case 0x0009:	// .Z009	Directory letter
					{
						//^2636:01CC
						//^2636:0244
						const Bit8u *bp0c = ptrDirLetter2;
						//^2636:024A
						FORMAT_SKSTR(bp0c, bp0116);
						//^2636:025D
						SK_STRCAT(bp08, bp0116);
						//^2636:0271
						bp04 = bp08 +SK_STRLEN(bp08);
						continue;
					}
				case 0x000a:	// .Z010
					{
						//^14 01
						Bit32u bp10 = glbItemWeightKg;
						SK_LTOA10(bp10, bp0c);
						break;
					}
				case 0x000b:	// .Z011
					{
						//^19 01
						Bit32u bp10 = glbItemWeightDg;
						SK_LTOA10(bp10, bp0c);
						break;
					}
				case 0x000c:	// .Z012
					{
						//^1E 01
						Bit32u bp10 = _4976_52e0;
						SK_LTOA10(bp10, bp0c);
						break;
					}
				case 0x000d:	// .Z013
					{
						//^23 01
						Bit32u bp10 = _4976_52fa;
						SK_LTOA10(bp10, bp0c);
						break;
					}
				case 0x000e:	// .Z014
					{
						//^28 01
						Bit32u bp10 = _4976_52f6;
						SK_LTOA10(bp10, bp0c);
						break;
					}
				case 0x000f:	// .Z015
					//^2636:019C
					{
						if (_4976_52f4 == 1) {
							//^2636:01AF
							//^2636:0244
							const Bit8u *bp0c = _4976_498e;
							//^2636:024A
							FORMAT_SKSTR(bp0c, bp0116);
							//^2636:025D
							SK_STRCAT(bp08, bp0116);
							//^2636:0271
							bp04 = bp08 +SK_STRLEN(bp08);
							continue;
						}
						if (_4976_52f4 == 2) {
							//^2636:01AD
							//^2636:01B9
							//^2636:0244
							const Bit8u *bp0c = _4976_4992;
							//^2636:024A
							FORMAT_SKSTR(bp0c, bp0116);
							//^2636:025D
							SK_STRCAT(bp08, bp0116);
							//^2636:0271
							bp04 = bp08 +SK_STRLEN(bp08);
							continue;
						}
						break;
					}
				case 0x0010:
					break;
				case 0x0011:	// .Z017
					{
						//^2636:0153
						bp12 = 7;
						Bit8u bp13 = 0;
						Bit8u bp14 = U8(_4976_52e4);
						//^2636:0161
						QUERY_GDAT_TEXT(bp12, bp13, bp14, bp0c);
						break;
					}
				case 0x0012:
					break;
				case 0x0013:
					break;
				case 0x0014:	// .Z020 : Data dir (E:DATA\)
					{
						//^DC 01
						//^2636:0244
						const Bit8u *bp0c = strZxxxTable[3];	// ".Z008DATA\"
												
						// SPX: the game folder (which is DATA by default) is changed from dungeon menu (need a restart)
						// Standard ".Z008DATA"
						bp0c = GET_DATA_FOLDER_ZNAME(skwin.dung);
						/*
						switch(skwin.dung)
						{
							case _OPTION_DUNGEON_NO_SPECIFIC_:
							default:
								bp0c = (const X8*) ".Z008DATA\\"; break;
						} // END of specific block
						*/
						// SPX: If a specific data folder name is given (within SkWin directory), it will be used instead
						if (skwin.sCustomDataFolder != NULL)
						{
							static char sFolderString[256];
							sprintf(sFolderString, ".Z008%s\\", skwin.sCustomDataFolder);
							bp0c = (const X8*) sFolderString;
						}

						//^2636:024A
						FORMAT_SKSTR(bp0c, bp0116);
						//^2636:025D
						SK_STRCAT(bp08, bp0116);
						//^2636:0271
						bp04 = bp08 +SK_STRLEN(bp08);
						continue;
					}
				case 0x0015:
					break;
				case 0x0016:	// .Z022 : Data dir (E:DATA\) used for savegames (pc)
					{
						//^2636:01F9
						if (_4976_49a0 == 0) {
							//^2636:0209
							//^2636:0244
							const Bit8u *bp0c = strZxxxTable[6];
							//^2636:024A
							FORMAT_SKSTR(bp0c, bp0116);
							//^2636:025D
							SK_STRCAT(bp08, bp0116);
							//^2636:0271
							bp04 = bp08 +SK_STRLEN(bp08);
							continue;
						}
						else {
							//^2636:0200
							//^2636:0244
							const Bit8u *bp0c = strZxxxTable[5];
							// SPX: the game folder (which is DATA by default) is changed from dungeon menu (need a restart)
							// Standard ".Z008DATA"
							bp0c = GET_DATA_FOLDER_ZNAME(skwin.dung);
							/*
							switch(skwin.dung)
							{
								default:
									bp0c = (const X8*) ".Z008DATA\\"; break;
							} // END of specific block
							*/
							// SPX: If a specific data folder name is given (within SkWin directory), it will be used instead
							if (skwin.sCustomDataFolder != NULL)
							{
								static char sFolderString[256];
								sprintf(sFolderString, ".Z008%s\\", skwin.sCustomDataFolder);
								bp0c = (const X8*) sFolderString;
							}
							//^2636:024A
							FORMAT_SKSTR(bp0c, bp0116);
							//^2636:025D
							SK_STRCAT(bp08, bp0116);
							//^2636:0271
							bp04 = bp08 +SK_STRLEN(bp08);
							continue;
						}
					}
				case 0x0017:	// .Z023 : Savegame number
					{
						//^2636:0218
						Bit8u bp16[] = {glbSKSaveDigitAlpha, 0};
						//^2636:0222
						SK_STRCPY(bp0096, bp16);
						break;
					}
				case 0x0018:	// .Z024 : Dungeon letter (default nothing, or G (german) or F (french)
					{
						//^2636:0235
						//^2636:0244
						ATLASSERT(glbLangageLetterIndex < 8U);
						const Bit8u *bp0c = strZxxxTable[glbLangageLetterIndex];
						//^2636:024A
						FORMAT_SKSTR(bp0c, bp0116);
						//^2636:025D
						SK_STRCAT(bp08, bp0116);
						//^2636:0271
						bp04 = bp08 +SK_STRLEN(bp08);
						continue;
					}
				case 0x0019:	// .Z025
					{
						//^2636:010F
						Bit32u bp10 = _4976_4746;
						SK_LTOA10(bp10, bp0c);
					}
				case 0x001C:	// .Z028
					{
						//^2636:01D5
						if (_4976_5c9c != 0) {
							//^2636:01E5
							//^2636:0244
							const Bit8u *bp0c = strZxxxTable[7];	// ".Z009DATA\"
							//^2636:024A
							FORMAT_SKSTR(bp0c, bp0116);
							//^2636:025D
							SK_STRCAT(bp08, bp0116);
							//^2636:0271
							bp04 = bp08 +SK_STRLEN(bp08);
							continue;
						}
						else {
							//^2636:01DC
							//^2636:0244
							const Bit8u *bp0c = strZxxxTable[3];	// ".Z008DATA\"
							//^2636:024A
							FORMAT_SKSTR(bp0c, bp0116);
							//^2636:025D
							SK_STRCAT(bp08, bp0116);
							//^2636:0271
							bp04 = bp08 +SK_STRLEN(bp08);
							continue;
						}
					}
				case 0x001A:	// .Z026 : Dir used for graphics2
					{
						//^2636:01E5
						//^2636:0244
						const Bit8u *bp0c = strZxxxTable[7];	// ".Z009DATA\"
						//^2636:024A
						FORMAT_SKSTR(bp0c, bp0116);
						//^2636:025D
						SK_STRCAT(bp08, bp0116);
						//^2636:0271
						bp04 = bp08 +SK_STRLEN(bp08);
						continue;
					}
				case 0x001B:	// .Z027
					{
						//^2636:01EE
						Bit8u bp13 = 0xFE;
						Bit8u bp14 = 0x06;
						//^2636:0161
						QUERY_GDAT_TEXT(bp12, bp13, bp14, bp0c);
						break;
					}
#if DM2_EXTENDED_MODE == 1
				// SPX : Addition to handle PC9821
			/*	case 0x0050:	// .Z080 : GDAT Platform version
					{
						const Bit8u *bp0c = (const Bit8u *) "_PC9821";
						if (skwin.gdat_vers != 0)
						{
							FORMAT_SKSTR(bp0c, bp0116);
							SK_STRCAT(bp08, bp0116);
							bp04 = bp08 +SK_STRLEN(bp08);
						}
						continue;
					}
				case 0x0051:	// .Z081 : GDAT Version number
					{
						
						if (skwin.gdat_vers == 3)	// ID_VERSION_V3 CLASSIC
						{
							const Bit8u *bpxx = (const Bit8u *) "_V3";
							FORMAT_SKSTR(bpxx, bp0116);
							SK_STRCAT(bp08, bp0116);
							bp04 = bp08 +SK_STRLEN(bp08);
						}
						if (skwin.gdat_vers == 4)	// ID_VERSION_V4 CLASSIC
						{
							const Bit8u *bpxx = (const Bit8u *) "_V4";
							FORMAT_SKSTR(bpxx, bp0116);
							SK_STRCAT(bp08, bp0116);
							bp04 = bp08 +SK_STRLEN(bp08);
						}
						if (skwin.gdat_vers == 6)	// ID_VERSION_V5_CARTOON
						{
							const Bit8u *bpxx = (const Bit8u *) "_V5";
							FORMAT_SKSTR(bpxx, bp0116);
							SK_STRCAT(bp08, bp0116);
							bp04 = bp08 +SK_STRLEN(bp08);
						}
						continue;
					}
				case 0x0052:	// .Z082 : GDAT Style
					{
						if (skwin.gdat_vers != 0 && skwin.gdat_vers < 5)
						{
							const Bit8u *bp0c = (const Bit8u *) "_CLASSIC";
							FORMAT_SKSTR(bp0c, bp0116);
							SK_STRCAT(bp08, bp0116);
							bp04 = bp08 +SK_STRLEN(bp08);
						}
						if (skwin.gdat_vers == 6)		// ID_VERSION_V5_CARTOON
						{
							const Bit8u *bp0c = (const Bit8u *) "_CARTOON";
							FORMAT_SKSTR(bp0c, bp0116);
							SK_STRCAT(bp08, bp0116);
							bp04 = bp08 +SK_STRLEN(bp08);
						}
						continue;
					}*/
#endif // DM2_EXTENDED_MODE
			}
			//^2636:024A
			FORMAT_SKSTR(bp0c, bp0116);
			//^2636:025D
			SK_STRCAT(bp08, bp0116);
			//^2636:0271
			bp04 = bp08 +SK_STRLEN(bp08);
			//^2636:028C
			continue;
		}
		//^2636:028E
		*bp04 = bp11;
		bp04++;
		format++;
		//^2636:029D
	}
	//^2636:02AD
	bp04[0] = 0;
	return bp08;
}

//^2066:033C
U16 SkWinCore::_2066_033c(void *buff, int size)
{
	//^2066:033C
	Bit32u bp04 = size;
	if (bp04 == 0)
		return 1;
	//^2066:0354
	return FILE_READ(glbDataFileHandle, bp04, buff);
}

//^2066:03A0
int SkWinCore::SKLOAD_READ(void *buff, int size)
{
	//^2066:03A0
	if (_2066_033c(buff, size) == 0)
		return 0;
	//^2066:03BB
	return 1;
}

//^2066:2498
void SkWinCore::DECIDE_DEFAULT_DUNGEON_MAP_CHIP_SET()
{
	//^2066:2498
	ENTER(208);
	//^2066:249E
	U8 bp00d0[MAXDEPTH1];
	U8 *bp04 = bp00d0;
	FILL_STR(bp04, MAXDEPTH1, '1', 1);
	X16 bp06 = 0;
	i16 si = 0;
	for (; dunHeader->nMaps > si; si++) {
		//^2066:24C5
		bp06 = max_value(dunMapsHeaders[si].Level(), bp06);
		//^2066:24E6
	}
	//^2066:24F4
	_4976_4c5c = ALLOC_MEMORY_RAM(bp06 +1, afUseUpper, 0x400);
	CHANGE_CURRENT_MAP_TO(0);
	ObjectID di = GET_TILE_RECORD_LINK(dunHeader->StartPartyPosX(), dunHeader->StartPartyPosY());
	for (; di != OBJECT_END_MARKER && di.DBType() != dbText; di = GET_NEXT_RECORD_LINK(di));
	//^2066:2552
	if (di != OBJECT_END_MARKER && GET_ADDRESS_OF_RECORD2(di)->TextMode() == 0) {
		//^2066:256F
		QUERY_MESSAGE_TEXT(bp04, di, 0x8002);
	}
	for (si = 0; si <= bp06; si++) {
		//^2066:2585
_2585:
		i8 bp07 = *(bp04++);
		switch (bp07) {
			case 0: //^_2585 // nul
			case 10: //^_2585 // line feed
			case 32: //^_2585 // spc
				goto _2585;
		}
		//^2066:25A2
		_4976_4c5c[si] = bp07 - '0';
		//^2066:25AE
	}
	//^2066:25B4
	return;
}

//^2066:25B8
int SkWinCore::READ_DUNGEON_STRUCTURE(X16 isNewGame)
{
	// Visit http://dmweb.free.fr/?q=node/217 for file format

	//^2066:25B8
	Bit16u bp04 = 0;
	Bit16u si = 0;
	i16 di;
	Bit8u bp01 = 0;

	//printf("NewGame = %d // Dung = %d\n", isNewGame, skwin.dung);
#ifdef __SK_EXTENDED_SKWIN_V6__
	int iReadExtendedGame = 0;
	iReadExtendedGame = READ_DUNGEON_STRUCTURE_EXTENDED_GAME(isNewGame, skwin.dung);
	if (iReadExtendedGame)	/// should return 1 if extended game was read correctly
		return iReadExtendedGame;
#endif //


	printf("Read 8 first bytes (Random seed) ...\n");
	//SPX: Read the first 8 bytes of the dungeon.dat
	Bit8u bp26[8];
	printf("Dungeon Read iHandle = %d\n", glbDataFileHandle);
	if (FILE_READ(glbDataFileHandle, 8, bp26) == 0)
		return 0;
	//^2066:25DA
	//SPX: ? a magical number to make "the game is damaged" while reading it. What was the purpose? ...
	if (*(Bit16u *)bp26 == 0x8104)
		return 0;

	//SPX: Add control of DM1 gfx seed
	if (*(Bit16u *)bp26 == 0x0063) // DM1 or TQ
		SkCodeParam::bDM1Mode = true;
	else if (*(Bit16u *)bp26 == 0x0D00 || *(Bit16u *)bp26 == 0x0800) // CSB
		SkCodeParam::bDM1Mode = true;
	if (SkCodeParam::bDM1Mode == true)
		skwin.enableDoubleStepMove = false;
	// SPX

	//^2066:25E4
	printf("FILE_SEEK pos @ %d", _4976_524a);
	//getch();	
	FILE_SEEK(glbDataFileHandle, _4976_524a);
	//^2066:25F8
	if (_4976_3b5d != 0) {
		//^2066:25FF
		dunHeader = reinterpret_cast<File_header *>(ALLOC_MEMORY_RAM(44, 1, 1024));
	}

	// - File header
	printf("Read Dungeon Global Header ...\n");
	//^2066:2617
	if (SKLOAD_READ(dunHeader, 44) == 0)
		return 0;
/*
	printf("DUN HEADER:\n");
	printf("U16 w0 = %4X:\n", dunHeader->w0);
	printf("U16 cbMapData = %4X:\n", dunHeader->cbMapData);
	printf("U8  nMaps = %2X:\n", dunHeader->nMaps);
	printf("U8  b5 = %2X:\n", dunHeader->b5);
	printf("U16 cwTextData = %4X:\n", dunHeader->cwTextData);
	printf("U16 w8 = %4X:\n", dunHeader->w8);
	printf("U16 cwListSize = %4X:\n", dunHeader->cwListSize);
	printf("U16 nRecords[0] = %4X:\n", dunHeader->nRecords[0]);
	getch();
	*/
/*
		U16 w0; //  Random seed used to display random 
		U16 cbMapData; // Size of global map data in byt
		U8 nMaps; // Number of maps (in Dungeon Mast;
		U8 b5; // # Unused, padding
		U16 cwTextData; // Text data size in words (17
		U16 w8; // Starting party position (mus
		U16 cwListSize; // Object list size in wor
		U16 nRecords[16]; // Number of objects of each type:
*/	
	//^2066:262F
	si = dunHeader->w8;
	glbPlayerPosX = dunHeader->StartPartyPosX();// si & 0x001F;
	//^2066:263F
	si >>= 5;
	glbPlayerPosY = dunHeader->StartPartyPosY();// si & 0x001F;
	//^2066:264A
	glbPlayerDir = (si >> 5) & 0x0003;
	glbPlayerMap = 0;
	//^2066:265B
	i16 nMaps = dunHeader->nMaps;	// bp10
	printf("Maps read = %d", nMaps);
	//getch();
	//^2066:2664
	// SPX: Alloc map header (x16), capped to 64 maps (64*16 = 1024 = 0x400)
	if (_4976_3b5d != 0) {
		//^2066:266B
		dunMapsHeaders = reinterpret_cast<Map_definitions *>(ALLOC_MEMORY_RAM(nMaps << 4, afUseUpper, 0x400));
	}

	// - Map definitions

	printf("Read %d Map Headers ...\n", nMaps);
	//^2066:2685
	if (SKLOAD_READ(dunMapsHeaders, nMaps << 4) == 0)
		return 0;
	//^2066:26A2
	if (_4976_3b5d != 0) {
		//^2066:26A9
		dunMapColumnsSumArray = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(nMaps << 1, afUseUpper, 0x400));
	}

	printf("Read %d Maps Columns Headers ...\n", nMaps);
	//getch();
	//^2066:26C5
	Bit16u bp0e = 0;
	//^2066:26CC
	for (si = 0; si < nMaps; si++) {
		//^2066:26CE
		dunMapColumnsSumArray[si] = bp0e;
		//^2066:26DE
		bp0e += dunMapsHeaders[si].Column();
		//^2066:26F7
	
		// SPX: adjustment for DM1 mode : put flags for activating gfx for pits, doors, etc ...
		if (SkCodeParam::bDM1Mode == true)
		{
			dunMapsHeaders[si].bGfxFlags = MAPGFX_FLAG__PIT_UPPER_ROOF | MAPGFX_FLAG__PIT_LOWER_GROUND | MAPGFX_FLAG__STAIRS_GOING_UP | MAPGFX_FLAG__STAIRS_GOING_DOWN | MAPGFX_FLAG__TELEPORTER | MAPGFX_FLAG__DOOR_0 | MAPGFX_FLAG__DOOR_1;
			dunMapsHeaders[si].w14 = (3 << 4) + (dunMapsHeaders[si].w14 & 0xFF00); // tileset = 3 (keep)
		}
	}
	//^2066:26FD
	_4976_4cb4 = bp0e;
	//^2066:2703
	si = dunHeader->cwListSize;
	//^2066:270B
	if (isNewGame != 0) {
		//^2066:2711
		dunHeader->cwListSize += 300;
	}
	//^2066:2717
	if (_4976_3b5d != 0) {
		//^2066:271E
		//dunMapTilesObjectIndexPerColumn = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(bp0e << 1, afUseUpper, 0x400));
		dunMapTilesObjectIndexPerColumn = reinterpret_cast<OID_T *>(ALLOC_MEMORY_RAM(bp0e << OID_SIZE_BITSHIFT, afUseUpper, 0x400));
		//^2066:273A
		//dunGroundStacks = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(dunHeader->cwListSize << 1, afUseUpper, 0x400));
		dunGroundStacks = reinterpret_cast<OID_T *>(ALLOC_MEMORY_RAM(dunHeader->cwListSize << OID_SIZE_BITSHIFT, afUseUpper, 0x400));
		//^2066:2754
		dunTextData = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(dunHeader->cwTextData << 1, afUseUpper, 0x400));
	}

	// - Index of tiles with objects on them (per column)
	printf("Read Index per Columns ...\n");
	//getch();
	//^2066:277C
	if (SKLOAD_READ(dunMapTilesObjectIndexPerColumn, bp0e << 1) == 0)
		return 0;

	// - List of object IDs of first objects on tiles

	//^2066:2798
	if (SKLOAD_READ(dunGroundStacks, si << 1) == 0)
		return 0;
	//^2066:27B3
	if (isNewGame != 0) {
		//^2066:27B9
		for (di = 0; di < 300; si++, di++) {
			//^2066:27BD
			dunGroundStacks[si] = 0xffff;
			//^2066:27CC
		}
	}

	// - Text Data

	//^2066:27D4
	if (SKLOAD_READ(dunTextData, dunHeader->cwTextData << 1) == 0)
		return 0;
	//^2066:27F5
	if (_4976_5bf2 == 0) {
		//^2066:27FC
		glbTimersMaximumCount = MAX_TIMER_NUMBER;	// = 50
	}

	// - List of XXX
	printf("Read Objects per Category ...\n");
	//^2066:2802
	for (si = 0; si < 16; si++) {
		//^2066:2807
		printf("Category %02d of size %02d => %04d records ...\n", si, glbItemSizePerDB[si], dunHeader->nRecords[si]);
		Bit16u di = dunHeader->nRecords[si];
		if (isNewGame != 0) {
			//^2066:281B
			dunHeader->nRecords[si] = min_value((si == dbCloud) ? 0x300 : 0x400, tblDefaultNbItemAllocationPerDB[RCJ(16,si)] + di);
		}
		//^2066:2849
		Bit16u bp0e = glbItemSizePerDB[si];
		if (_4976_3b5d != 0) {
			//^2066:2859
			glbDBObjectData[si] = ALLOC_MEMORY_RAM(dunHeader->nRecords[si] * bp0e, afUseUpper, 0x400);
		}
		//^2066:2887
		Bit16u *bp04 = (Bit16u *)glbDBObjectData[si];
		if (SKLOAD_READ(glbDBObjectData[si], bp0e * di) == 0)
			return 0;
		//^2066:28BB
		if (_4976_5bf2 == 0) {
			//^2066:28C2
			if (si == 4 || si >= 14) {
				//^2066:28CC
				glbTimersMaximumCount += dunHeader->nRecords[si];
			}
		}
		//^2066:28E0
		if (isNewGame != 0) {
			//^2066:28E6
			bp0e >>= 1;
			bp04 += di * bp0e;
			//^2066:28F5
			for (di = tblDefaultNbItemAllocationPerDB[RCJ(16,si)]; di != 0; di--) {
				*bp04 = 0xffff;
				bp04 += bp0e;
			}
		}
		//^2066:2914
	}
	//^2066:291D
	printf("Alloc RAM for maps ...\n");
	if (_4976_3b5d != 0) {
		//^2066:2924
		dunMapData = ALLOC_MEMORY_RAM(dunHeader->cbMapData, afUseUpper, 0x400);
	}

	// - Map data

	printf("Read Dungeon Map Data ...\n");
	//getch();
	//^2066:2942
	if (SKLOAD_READ(dunMapData, dunHeader->cbMapData) == 0)
		return 0;
	//^2066:2960
	if (_4976_3b5d != 0) {
		//^2066:296A
		// SPX: 0x400 = 32*32 tiles / bp10 = #maps / _4976_4cb4 = #cols for all maps
		glbMapTileValue = reinterpret_cast<U8 ***>(ALLOC_MEMORY_RAM((_4976_4cb4 + nMaps) * sizeof(void *), afUseUpper, 0x400));
		//^2066:298A
		Bit8u ***bp08 = &glbMapTileValue[nMaps];
		//^2066:299C
		for (di = 0; di < nMaps; di++) {
			//^2066:29A1
			// MARK SPX
			glbMapTileValue[di] = reinterpret_cast<U8 **>(bp08);
			//^2066:29B9
			Bit8u *bp0c = &dunMapData[dunMapsHeaders[di].w0];
			*bp08 = reinterpret_cast<U8 **>(bp0c);
			bp08++;
			//^2066:29E2
			for (si = 1; dunMapsHeaders[di].Column() -1 >= si; si++) {
				//^2066:29E7
				bp0c += dunMapsHeaders[di].Row();
				*bp08 = reinterpret_cast<U8 **>(bp0c);
				bp08++;
				//^2066:2A14
			}
			//^2066:2A2E
		}
	}

	printf("Read Some Random Value from GDAT ??? ...\n");
	//^2066:2A37
	_4976_5c24 = BETWEEN_VALUE(0, QUERY_GDAT_ENTRY_DATA_INDEX(0x03, 0x00, 0x0B, 0x00), 23) * 0x0555UL;
	//^2066:2A6A
	//printf("_4976_3b5d / INIT_TIMERS\n");
	//getch();
	if (_4976_3b5d != 0) {
		//^2066:2A71
		INIT_TIMERS();
	}
	//^2066:2A76
	//getch();
	//printf("_3df7_0037\n");
	_3df7_0037(!_4976_3b5d);
	//^2066:2A85
	if (_4976_3b5d != 0) {
		//^2066:2A8C
		//!ALT
		//getch();
		printf("ALLOC_MEMORY_RAM %d %d %08x\n", MAXDEPTH, afUseUpper, 0x400);
		_4976_4cb0 = ALLOC_MEMORY_RAM(MAXDEPTH, afUseUpper, 0x400);
		printf("Memret = %08x\n", _4976_4cb0);
		//^2066:2AA4
		//!ALT
		//getch();
		printf("ALLOC_MEMORY_RAM %d %d %08x\n", MAXDEPTH + MAXMAPS, afUseUpper, 0x400);
		_4976_4c72 = ALLOC_MEMORY_RAM(MAXDEPTH + MAXMAPS, afUseUpper, 0x400);
		printf("Memret = %08x\n", _4976_4c72);
		// MAXDEPTH = 63
		// MAXMAPS = 64
	}
	printf("Before arrange maps\n");
	//^2066:2ABC
	//getch();
	Bit8u *bp18 = _4976_4c72;
	Bit8u *bp1c = _4976_4cb0;
	Bit16u bp14 = 0;
	*bp1c = 0;
	bp1c++;
	printf("Arrange Depth of Maps ... MAXDEPTH=%d, maps %d\n", MAXDEPTH, nMaps);
	//getch();
	//^2066:2AE4
	for (Bit16u bp12 = 0; bp12 < MAXDEPTH; bp1c++, bp12++) {
		//^2066:2AEB
		for (Bit16u bp1e = 0; bp1e < nMaps; bp1e++) {
			//^2066:2AF2
			if (dunMapsHeaders[bp1e].Level() == bp12) {
				//^2066:2B0A
				*bp18 = (Bit8u)bp1e;
				bp18++;
				bp14++;
			}
			//^2066:2B19
		}
		//^2066:2B24
		*bp18 = 0xff;
		bp18++;
		bp14++;
		*bp1c = (Bit8u)bp14;
		//^2066:2B3A
	}
	printf("Before assert\n");
	//getch();
	printf("Assert 1 : %d - %d <= %d + %d => [%d]\n", bp1c, _4976_4cb0, MAXDEPTH, 1U, (bp1c - _4976_4cb0));
	printf("Assert 2 : %d - %d <= %d + %d + %d => [%d]\n", bp18, _4976_4c72, MAXDEPTH, MAXMAPS, 1U, (bp18 - _4976_4c72));
	//getch();
	ATLASSERT((bp1c - _4976_4cb0) <= (MAXDEPTH + 1U));
	//getch();
	ATLASSERT((bp18 - _4976_4c72) <= (MAXDEPTH + MAXMAPS + 1U));
	//getch();

	printf("Arrange Dungeon ...\n");
	//getch();
	//^2066:2B46
	if (isNewGame != 0) {
		//^2066:2B4C
		ARRANGE_DUNGEON();
	}
	printf("DECIDE_DEFAULT_DUNGEON_MAP_CHIP_SET\n");
	//getch();
	//^2066:2B50
	if (_4976_3b5d != 0) {
		//^2066:2B57
		DECIDE_DEFAULT_DUNGEON_MAP_CHIP_SET();
	}
	//printf("_4976_3b5d / Structure complete\n");
	//getch();
	//^2066:2B5B
	_4976_3b5d = 0;
	//^2066:2B64

	//CHECK_TILE_RECORDS();
	printf("Read Dungeon Structure Completed !\n");
	//getch();
	if (SkCodeParam::bDebugInfoMapInit == true)
		LOG_FULL_DUNGEON_INFO();

	LUA_CALL_SCRIPT(_EXP_SCRIPT__GAME_INIT_, 0, 0, 0, 0);

	//printf("Return 1\n"); getch();
	return 1;
}

void SkWinCore::CHECK_TILE_RECORDS() {
	// check sort order: dbDoor < dbTele < dbText < dbActu == db11 == db12 == db13 < dbCreature < ...
	for (int z = 0; z < dunHeader->nMaps; z++) {
		CHANGE_CURRENT_MAP_TO(z);
		for (int x = 0; x < glbCurrentMapWidth; x++) {
			for (int y = 0; y < glbCurrentMapHeight; y++) {
				ObjectID rl = GET_TILE_RECORD_LINK(x, y);
				if (rl != OBJECT_NULL) {
					int lastDBtype = -1;
					while (rl != OBJECT_END_MARKER && (lastDBtype = rl.DBType()) <= dbActuator) {
						rl = GET_NEXT_RECORD_LINK(rl);
					}
					while (rl != OBJECT_END_MARKER) {
						ATLASSERT(rl.DBType() > dbActuator);
						rl = GET_NEXT_RECORD_LINK(rl);
					}
				}
			}
		}
	}
}

//^2066:01C3
int SkWinCore::SUPPRESS_READER(void *_data, const void *_mask, Bit16u buffSize, Bit32u repeat, Bit16u fill)
{
	Bit8u *data = reinterpret_cast<Bit8u *>(_data);
	const Bit8u *mask = reinterpret_cast<const Bit8u *>(_mask);

	//^2066:01C3
	Bit32u bp0a = 0;
	Bit8u bp04 = _4976_5254;
	Bit8u bp05 = _4976_524e;
	//^2066:01E8
	for (Bit32u bp0e = 0; bp0e < repeat; ) {
		//^2066:01EB
		for (Bit16u si = 0; si < buffSize; si++) {
			//^2066:01F0
			Bit8u bp03 = (fill != 0) ? 0 : (*data);
			//^2066:0205
			Bit8u bp02 = *mask;
			mask++;
			//^2066:0211
			if (bp02 != 0) {
				//^2066:021A
				for (i8 bp01 = 0x07; bp01 >= 0; bp01--) {
					//^2066:0221
					if (((1 << bp01) & bp02) != 0) {
						//^2066:0232
						if (bp05 == 0) {
							//^2066:0238
							Bit8u bp0f;
							if (FILE_READ(glbDataFileHandle, 1, &bp0f) == 0)
								return 1;
							//^2066:0257
							bp04 = bp0f;
						}
						//^2066:025D
						if ((bp04 & 0x80) != 0) {
							//^2066:0263
							bp04 = (bp04 << 1) | 1;
							bp03 = (1 << bp01) | bp03;
						}
						else {
							//^2066:027B
							bp04 <<= 1;
							bp03 = bp03 & (~(1 << bp01));
						}
						//^2066:028C
						bp0a++;
						bp05++;
						//^2066:029F
						if (bp05 == 8) {
							//^2066:02A3
							bp05 = 0;
						}
					}
					//^2066:02A7
				}
			}
			//^2066:02B3
			*data = bp03;
			data++;
			//^2066:02BF
		}
		//^2066:02C8
		mask -= buffSize;
		bp0e++;
		//^2066:02D6
	}
	//^2066:02EE
	_4976_5254 = bp04;
	_4976_524e = bp05;
	_4976_5258 = bp0a;

	//SPX: removed this check against SKSaveIO.bin to allow proper savegame load !
	//s_testSKSave.Read(_data, _mask, buffSize, repeat);
	return 0;
}

//^2066:030D
int SkWinCore::READ_1BIT(Bit16u *pw)
{
	//^2066:030D
	Bit8u bp01;
	//^2066:0312
	Bit16u si = SUPPRESS_READER(&bp01, glbByte01, 1, 1, 1);
	*pw = bp01;
	return si;
}


//^2066:15AA
int SkWinCore::READ_RECORD_CHECKCODE(i16 xpos, i16 ypos, ObjectID *recordLinkPtr, Bit16u readDir, Bit16u readSub)
{
	// readDir=(read-direction-in-record-link)
	// readSub=(read-subsequent-records)

	//printf("READ_RECORD_CHECKCODE @ (%02d : %02d/%02d)\n", glbCurrentMapIndex, xpos, ypos);

	//^2066:15AA
	while (true) {
		//^2066:15B0
		Bit16u bp0e;
		if (READ_1BIT(&bp0e) != 0) // read#more-record
			return 1;
		//^2066:15C2
		if (bp0e == 0)
			break;
		//^2066:15CB
		Bit8u bp0c = 0x0f;
		Bit8u bp0b;
		//^2066:15CF
		if (SUPPRESS_READER(&bp0b, &bp0c, 1, 1, 1) != 0) // read#dbtype
			return 1;
		//^2066:15EF
		Bit16u di = bp0b;
		Bit16u bp0a;
		if (readDir != 0 && di != dbCreature) {
			//^2066:1601
			bp0c = 0x03;
			//^2066:1605
			if (SUPPRESS_READER(&bp0b, &bp0c, 1, 1, 1) != 0)
				return 1;
			//^2066:1625
			bp0a = bp0b;
		}
		else {
			//^2066:162F
			bp0a = 0;
		}
		//^2066:1634
		if (di == dbCloud && _4976_3b59 != 0) {
			//^2066:1640
			Bit16u bp10 = 0x7f;
			//^2066:1645
			if (SUPPRESS_READER(&bp0e, &bp10, 2, 1, 1) != 0)
				return 1;
			//^2066:1665
			bp0e = bp0e;
			if (recordLinkPtr == NULL)
				break;
			//^2066:1676
			*recordLinkPtr = 0xff80 | bp0e;
			break;
		}
		//^2066:1685
		ObjectID si = ALLOC_NEW_RECORD(di);
		// SPX : Isn't possible to read a savegame having FFFE or FFFF?
		// SPX : I disable here the assert because it prevents loading some savegames.
		if (!SkCodeParam::bUsePowerDebug)
			ATLASSERT(si != OBJECT_END_MARKER && si != OBJECT_NULL);
		
		si.Dir(bp0a);
		//^2066:169E
		APPEND_RECORD_TO(si, recordLinkPtr, xpos, ypos);
		//^2066:16B3
		void *bp04 = GET_ADDRESS_OF_RECORD(si);
		//^2066:16C0
		const void *bp08 = tableMask[di];
		if (bp08 != NULL) {
			//^2066:16DA
			Bit16u bp12 = 0;
			//^2066:16DF
			switch (di) {
				case dbCreature:
					{
						//^2066:16F6
						bp0c = 0x7f;
						if (SUPPRESS_READER(&bp0b, &bp0c, 1, 1, 1) != 0)
							return 1;
						//^2066:171A
						reinterpret_cast<Creature *>(bp04)->b4 = bp0b;
						//^2066:1724
						if ((QUERY_CREATURE_AI_SPEC_FLAGS(si) & 1) != 0)
							//^2066:1730
							bp08 = _4976_3ac7;
						break;
					}
				case dbContainer:
					{
						//^2066:173A
						bp0c = 0x03;
						if (SUPPRESS_READER(&bp0b, &bp0c, 1, 1, 1) != 0)
							return 1;
						//^2066:175E
						reinterpret_cast<Container *>(bp04)->ContainerType(bp0b);
						//^2066:1774
						if (IS_CONTAINER_MAP(si) != 0) {
							//^2066:177F
							bp08 = _4976_3aef;
							bp12 = 1;
						}
						break;
					}
				case dbMissile:
					{
						//^2066:178E
						if (_4976_3b5b != 0) {
							//^2066:1795
							bp08 = _4976_3b0b;
							bp12 = 1;
						}
						break;
					}
			}
			//^2066:17A2
			if (SUPPRESS_READER(bp04, bp08, glbItemSizePerDB[di], 1, 0) != 0)
				return 1;
			//^2066:17C9
			switch (di) {
				case dbCreature:
					{
						//^2066:17DD
						_4976_3b5b = 1;
						reinterpret_cast<Creature *>(bp04)->possession = OBJECT_END_MARKER;	// 0xFFFE
						if (READ_RECORD_CHECKCODE(
							-1,
							0,
							&reinterpret_cast<GenericContainerRecord *>(bp04)->possession,
							(bp08 != _4976_3ab7) ? 1 : 0,
							1
							) != 0
						) {
							return 1;
						}
						//^2066:181F
						_4976_3b5b = 0;
						break;
					}
				case dbContainer:
					{
						//^2066:1828
						if (bp12 != 0) {
							Bit16u bp14;
							//^2066:182E
							if (READ_1BIT(&bp14) != 0)
								return 1;
							//^2066:1840
							if (bp14 != 0) {
								//^2066:1846
								//^2066:18B2
								ADD_MINION_ASSOC(si);
							}
							//^2066:1848
							reinterpret_cast<Container *>(bp04)->w2 = OBJECT_END_MARKER;	// 0xFFFE
						}
						else {
							//^2066:1854
							if (IS_CONTAINER_MONEYBOX(si) != 0) {
								//^2066:185F
								tableMask[dbMiscellaneous_item] = _4976_3afb;
							}
							//^2066:1869
							reinterpret_cast<Container *>(bp04)->w2 = OBJECT_END_MARKER;	// 0xFFFE
							//^2066:1872
							if (READ_RECORD_CHECKCODE(-1, 0, &reinterpret_cast<Container *>(bp04)->w2, 0, 1) != 0)
								return 1;
							//^2066:1891
							if (IS_CONTAINER_MONEYBOX(si) == 0)
								break;
							//^2066:189F
							tableMask[dbMiscellaneous_item] = _4976_3af7;
							break;
						}
						break;
					}
				case dbMissile:
					{
						//^2066:18AC
						if (bp12 != 0) {
							//^2066:18B2
							ADD_MINION_ASSOC(si);
						}
						else {
							//^2066:18BB
							glbTimersTable[reinterpret_cast<Missile *>(bp04)->TimerIndex()].value = si;
							//^2066:18D1
							reinterpret_cast<Missile *>(bp04)->w2 = OBJECT_END_MARKER;	// 0xFFFE
							_4976_3b59 = 1;
							//^2066:18E0
							if (READ_RECORD_CHECKCODE(-1, 0, &reinterpret_cast<Missile *>(bp04)->w2, 0, 0) != 0)
								return 1;
							//^2066:18FC
							_4976_3b59 = 0;
						}
						break;
					}
				case dbCloud:
					{
						//^2066:1904
						if (READ_1BIT(&bp0e) != 0)
							return 1;
						//^2066:1913
						if (bp0e == 0)
							break;
						//^2066:1919
						Bit16u bp10 = 0x03ff;
						//^2066:191E
						if (SUPPRESS_READER(&bp0e, &bp10, 2, 1, 1) != 0)
							return 1;
						//^2066:193B
						bp0e = bp0e;
						glbTimersTable[bp0e].w8 = si;
						break;
					}
			}
		}
		//^2066:1950
		if (readSub == 0)
			break;
    }
	//^2066:1959
	return 0;
}


//^2066:197C
// _2066_197c renamed READ_SKSAVE_TIMER_3C_3D
Bit16u SkWinCore::READ_SKSAVE_TIMER_3C_3D()
{
	//^2066:197C
	ENTER(4);
	//^2066:1981
	Timer *bp04 = glbTimersTable;
	//^2066:198E
	for (Bit16u si = 0; si < glbTimersCount; bp04++, si++) {
		//^2066:1992
		switch (bp04->TimerType()) {
			case tty3C:
			case tty3D:
				//^2066:19A3
				if (_4976_5244 == 0) {
					//^2066:19AA
					return 1;
				}
				//^2066:19AF
				bp04->w8 = 0xfffe;
				//^2066:19B8
				if (READ_RECORD_CHECKCODE(-1, 0, bp04->pv8_0_f(), 0, 0) != 0)
					//^2066:19D5
					//^2066:19AA
					return 1;

				break;
		}
		//^2066:19D7
	}
	//^2066:19E2
	return 0;
}


//^2066:19E7
U16 SkWinCore::READ_SKSAVE_DUNGEON()
{
	printf("RESUME/LOAD SAVEGAME ---------------------------------\n");
	//^2066:19E7
	printf("Reset champions inventory ... \n");
	Bit16u si;
	for (si=0; si < glbChampionsCount; si++) {
		//^2066:19F1
		ObjectID *bp08 = &glbChampionSquad[si].inventory[0];
		//^2066:1A01
		for (Bit16u di=0; di < 30; di++) {
			//^2066:1A05
			*bp08 = OBJECT_END_MARKER; bp08++;
			//^2066:1A11
		}
		//^2066:1A17
	}
	//^2066:1A1E
	glbLeaderHandPossession.object = OBJECT_END_MARKER;
	Bit16u bp18 = glbCurrentMapIndex;
	//^2066:1A2A
	printf("Delete records ... \n");
	Bit16u bp16;
	for (bp16=0; dunHeader->nMaps > bp16; bp16++) {
		//^2066:1A32
		CHANGE_CURRENT_MAP_TO(bp16);
		//^2066:1A3B
		for (Bit16u bp0e=0; bp0e < glbCurrentMapWidth; bp0e++) {
			//^2066:1A42
			for (Bit16u bp10=0; bp10 < glbCurrentMapHeight; bp10++) {
				//^2066:1A49
				ObjectID bp12 = GET_TILE_RECORD_LINK(bp0e, bp10);
				while (bp12 != OBJECT_END_MARKER) {
					//^2066:1A58
					if (bp12.DBType() > dbActuator) {
						//^2066:1A66
						GET_ADDRESS_OF_RECORD(bp12)->w0 = OBJECT_END_MARKER;
						//^2066:1A78
						CUT_RECORD_FROM(bp12, NULL, bp0e, bp10);
						break;
					}
					else {
						//^2066:1A8F
						bp12 = GET_NEXT_RECORD_LINK(bp12);
					}
					//^2066:1A9B
				}
				//^2066:1AA1
			}
			//^2066:1AAD
		}
		//^2066:1AB9
	}
	//^2066:1ACE
    CHANGE_CURRENT_MAP_TO(bp18);
	//^2066:1AD7
	printf("Delete objects ... \n");
	for (si = dbCreature; si < dbMax; si++) {
		//^2066:1ADC
		Bit16u *bp08 = reinterpret_cast<Bit16u *>(glbDBObjectData[si]);
		//^2066:1AEF
		Bit16u bp1a = glbItemSizePerDB[si] >> 1;
		for (Bit16u di = dunHeader->nRecords[si]; di-- != 0; bp08 += bp1a) {
			//^2066:1B0A
#if (DM2_EXTENDED_DATABASE == 1)
			if (si == db11 || si == db12 || si == db13)
				continue;
#endif
			reinterpret_cast<GenericRecord *>(bp08)->w0 = OBJECT_NULL;
			//^2066:1B12
		}
		//^2066:1B21
	}
	//^2066:1B27
	printf("Realloc Minions Table ... \n");
	glbMinionsObjectIDTable = reinterpret_cast<ObjectID *>(ALLOC_MEMORY_RAM(200, afDefault, 1024));
	//^2066:1B40
	glbMinionsAssocCount = 0;
	//^2066:1B46
	for (si = 0; si < glbChampionsCount; si++) {
		//^2066:1B4A
		ObjectID *bp08 = &glbChampionSquad[si].inventory[0];
		//^2066:1B5A
		for (Bit16u di = 0; di < 30; di++) {
			//^2066:1B5E
			if (READ_RECORD_CHECKCODE(-1, 0, bp08++, 0, 0) != 0) {
				//^2066:1B7C
				goto _1e7e;
			}
			//^2066:1B7F
		}
		//^2066:1B85
	}
	//^2066:1B8C
	if (READ_RECORD_CHECKCODE(-1, 0, &glbLeaderHandPossession.object, 0, 0) != 0) {
		//^2066:1BA3
		goto _1e7e;
	}
	//^2066:1BA6
	for (si = 0; si < glbChampionsCount; si++) {
		//^2066:1BAA
		ObjectID *bp08 = &glbChampionSquad[si].inventory[0];
		//^2066:1BBA
		for (Bit16u di = 0; di < 30; di++, bp08++) {
			//^2066:1BBE
			if (*bp08 == OBJECT_END_MARKER) {
				//^2066:1BC7
				*bp08 = OBJECT_NULL;
			}
			else {
				//^2066:1BCE
				PROCESS_ITEM_BONUS(si, *bp08, di, 0);
			}
			//^2066:1BE0
		}
		//^2066:1BEA
	}

	printf("Remove leader hand possession ... \n");
	//^2066:1BF1
	if (glbLeaderHandPossession.object == OBJECT_END_MARKER) {
		//^2066:1BF8
		glbLeaderHandPossession.object = OBJECT_NULL;
	}
	else {
		//^2066:1C00
		PROCESS_ITEM_BONUS(glbChampionLeader, glbLeaderHandPossession.object, -1, 0);
	}
	//^2066:1C14
	if (READ_SKSAVE_TIMER_3C_3D() != 0) {
		//^2066:1C1C
		goto _1e7e;
	}
	//^2066:1C1F
	printf("Loop through all maps ... \n");
	bp18 = glbCurrentMapIndex;
	for (bp16 = 0; dunHeader->nMaps > bp16; bp16++) {
		//^2066:1C2D
		CHANGE_CURRENT_MAP_TO(bp16);
		Bit8u *bp04 = *glbCurrentTileMap;
		for (Bit16u bp0e = 0; bp0e < glbCurrentMapWidth; bp0e++) {
			//^2066:1C4F
			for (Bit16u bp10 = 0; bp10 < glbCurrentMapHeight; bp10++) {
				//^2066:1C57
				Bit8u bp13 = *bp04;
				Bit16u bp24 = 0;
				Bit8u bp1b;
				switch (bp13 >> 5) {
					case ttWall:
					case ttFloor:
					case ttStairs:
					case ttMapExit:
						//^2066:1C7A
						bp1b = 0x00; break;
					case ttPit:
						//^2066:1C80
						bp1b = 0x08; break;
					case ttDoor:
						//^2066:1C86
						bp1b = 0x07; break;
					case ttTeleporter:
						{
							//^2066:1C8C
							TELE_inf bp2a;
							if (GET_TELEPORTER_DETAIL(&bp2a, (Bit8u)bp0e, (Bit8u)bp10) != 0) {
								//^2066:1CA3
								bp1b = 0x00;
								if (bp2a.target_z() < bp16) {
									//^2066:1CB1
									bp24 = 1;
								}
							}
							else {
								//^2066:1CB8
								bp1b = 0x08;
							}
							break;
						}
					case ttTrickWall:
						bp1b = 0x04; break;
				}
				//^2066:1CC2
				if (bp1b != 0) {
					if (SUPPRESS_READER(bp04, &bp1b, 1, 1, 0) != 0) {
						//^2066:1CE6
						goto _1e7e;
					}
				}
				//^2066:1CE9
				bp04++;

				// SkD((DLV_FYI, "DBG: (%2d,%2d,%2d,%02X)\n", bp16, bp0e, bp10, 0U +bp13));
				//^2066:1CEC
				if (bp24 == 0) {
					//^2066:1CF5
					ObjectID bp12 = GET_TILE_RECORD_LINK(bp0e, bp10);
					//^2066:1D02
					for (; bp12 != OBJECT_END_MARKER; bp12 = GET_NEXT_RECORD_LINK(bp12)) {
						//^2066:1D05
						// SkD((DLV_FYI, "DBG: #%04X\n", 0U +bp12.w));
						Bit16u bp22 = bp12.DBType();
						const void *bp0c = tableMask[bp22];
						//^2066:1D25
						if (bp0c == NULL)
							continue;
						//^2066:1D2C
						Actuator *bp08 = (Actuator *)GET_ADDRESS_OF_RECORD(bp12);
						if (bp22 == dbActuator) {
							//^2066:1D44
							// SkD((DLV_FYI, "DBG: T  %02X\n", 0U +bp08->ActuatorType()));
							switch (bp08->ActuatorType()) {
								case ACTUATOR_FLOOR_TYPE__CROSS_SCENE: // 0x27: Cross scene
								case ACTUATOR_TYPE_X1B: // 0x1B: -
								case ACTUATOR_TYPE_COUNTER: // 0x1D: Activator, counter
								case ACTUATOR_TYPE_ORNATE_STEP_ANIMATOR: // 0x41: Ornate step animator
								case ACTUATOR_TYPE_ORNATE_ANIMATOR: // 0x2C: Continuous ornate animator
								case ACTUATOR_TYPE_ORNATE_ANIMATOR_2: // 0x32: Activator, ornate animator
								case ACTUATOR_FLOOR_TYPE__SHOP: // 0x30: Shop exhibition
								case ACTUATOR_FLOOR_TYPE__FINITE_ACTIVATOR_RELAY: // 0x2D: Activator, finite activator relay
									//^2066:1DA7
									Bit16u bp1e;
									Bit16u bp20 = 0x01FF;
									if (SUPPRESS_READER(&bp1e, &bp20, 2, 1, 1) != 0) {
										//^2066:1DC9
										goto _1e7e;
									}
									//^2066:1DCC
									bp08->ActuatorData(bp1e);
									break;
							}
						}
						//^2066:1DE1
						if (SUPPRESS_READER(bp08, bp0c, glbItemSizePerDB[bp22], 1, 0) != 0) {
							//^2066:1E06
							goto _1e7e;
						}
						//^2066:1E08
					}
					//^2066:1E1D
					if (READ_RECORD_CHECKCODE(bp0e, bp10, NULL, 1, 1) != 0) {
						//^2066:1E34
						goto _1e7e;
					}
				}
				//^2066:1E36
			}
			//^2066:1E45
		}
		//^2066:1E54
	}
	//^2066:1E69
	CHANGE_CURRENT_MAP_TO(bp18);
	//^2066:1E72
	if (RECOVER_MINION_ASSOC() == 0) {
		//^2066:1E7A
		si = 0;
	}
	else {
		//^2066:1E7E
_1e7e:
		si = 1;
	}
	if (SkCodeParam::bForceSaveGameReadOK == true)
		si = 0; // all good ?
	//^2066:1E81
	DEALLOC_UPPER_MEMORY(200);
	return si;
}

//^2066:2D9C
i16 SkWinCore::GAME_LOAD()
{
	printf("GAME_LOAD:RESUME/LOAD SAVEGAME ---------------------------------\n");
	//^2066:2D9C
	Bit16u bp04 = 0;
	Bit16u si = 0;
	Bit8u bp01 = 0;
	Bit16u bp08;
	Bit16u bp06;
	//^2066:2DAD
	if (glbSpecialScreen != 0) {
		//^2066:2DB4
_2db4:
		bp04 = !_2066_03e0(0);
		//^2066:2DC6
		if (LOAD_NEW_DUNGEON() == 0) {
			//^2066:2DCE
			goto _3262;
		}
		else {
			//^2066:2DD1
			goto _31b8;
		}
	}
	//^2066:2DD4
	bp01 = 0;
	_4976_52f4 = 2;
	//^2066:2DDE
	if (bp01 == 0) {
		//^2066:2DE4
		if (_4976_49a0 != 0 || _4976_5eb0 == 0)
			goto _2e5b;
		//^2066:2DF2
		if (_476d_030a(2) == 1)
			goto _2e5b;
		//^2066:2DFF
		_476d_04e8(2);
	}

	//^2066:2E07
	while (true) {
		//^2066:2E07
		if (_4976_49a0 == 0) {
			//^2066:2E0E
			if (bp01 != 0) {
				//^2066:2E14
				_476d_04e8(2);
			}
			//^2066:2E1C
			Bit16u di = _0aaf_0067(_0aaf_02f8_DIALOG_BOX(15, bp01));
			//^2066:2E32
			if (di == 1) {
				//^2066:2E37
				_2066_03e0(0);
				//^2066:2E3E
				//^2066:32B2
				di = -1;
				//^2066:32B5
				return di;
			}
		}
		else {
			//^2066:2E41
			if (bp01 != 0) {
				//^2066:2E47
				_0aaf_0067(_0aaf_02f8_DIALOG_BOX(0, bp01));
			}
		}
		//^2066:2E5B
_2e5b:
		if (_4976_49a0 == 0 && _476d_04af(1) != 1) {
			//^2066:2E6F
			//^2066:2F37
			switch (_4976_5eb8) {
				case 0x0000:
					//^2066:2F3E
					bp01 = 0x1d;
					break;
				case 0x0003:
					//^2066:2F4B
					bp01 = 0x1a;
					break;
				case 0x0002:
					//^2066:2F58
					bp01 = 0x16;
					break;
				case 0x0004:
					//^2066:2F65
					bp01 = 0x17;
					break;
			}
		}
		else {
			//^2066:2E72
			i16 bp06 = 0;
			if (skwin.sksave >= 0)
				bp06 = skwin.sksave;
			else
				bp06 = SELECT_LOAD_GAME();
			//^2066:2E7A
			if (bp06 < 0) {
				//^2066:2E7E
				glbSKSaveNum = 0;
				//^2066:2E84
				//^2066:32B2
				Bit16u di = -1;
				//^2066:32B5
				return di;
			}
			//^2066:2E87
			glbSKSaveNum = bp06;						// SKsave num to read
			glbSKSaveDigitAlpha = glbSKSaveNum + 0x30;			// SKSave num as alpha
			bp08 = 0;
			si = 1;
			//^2066:2E9D
			glbDataFileHandle = FILE_OPEN(FORMAT_SKSTR(ptrSKSave_dat, NULL));
			//^2066:2EBD
			if (glbDataFileHandle < 0) {
				//^2066:2EC4
				glbDataFileHandle = FILE_OPEN(FORMAT_SKSTR(ptrSKSave_bak, NULL));
				//^2066:2EE4
				if (glbDataFileHandle >= 0) {
					//^2066:2EE8
					bp08 = 1;
				}
				else {
					//^2066:2EF0
					if (_4976_5bf2 == 0) {
						//^2066:2EF7
						glbDataFileHandle = FILE_OPEN(FORMAT_SKSTR(ptrDungeon_ftl, NULL));
						//^2066:2F17
						if (glbDataFileHandle >= 0) {
							//^2066:2F1B
							bp04 = 1;
							glbSpecialScreen = 1;	// new game
							//^2066:2F1B
							goto _2db4;
						}
					}
					//^2066:2F29
					si = 0;
					bp01 = 0x1c;
					_4976_5eb8 = 0x0003;
				}
			}
		}
		//^2066:2F69
		if (_4976_49a0 == 0 && _4976_5eb8 != 1) {
			//^2066:2F77
			continue;
		}

		break;
	}
	//^2066:2F7A
	_4976_5bf6 = 0;
	glbChampionsCount = 0;
	//SPX: changed 0xFFFF to oFFFF
	glbLeaderHandPossession.object = OBJECT_NULL; // 0xFFFF
	//^2066:2F8C
	sksave_header_asc bp6a;
	FILE_READ(glbDataFileHandle, 42, &bp6a);
	//^2066:2FA1
	_4976_5244 = bp6a.wTimerFlag;
	//^2066:2FA7
	_4976_524a = 42;
	//^2066:2FB3
	glbSpecialScreen = !_4976_5bf2;


	// SPX: a regular DM2 savegame will have wTimerFlag = 1. If not, we are probably not reading a native DM2 savegame.
	// There is a chance this is a DM1 savegame, and we will try it.

	if (bp6a.wTimerFlag == 1) // native DM2 savegame
	{
		//^2066:2FBE
		do {
			printf("Read Dungeon Structure (Native DM2) ...\n");
			//getch();
			if (READ_DUNGEON_STRUCTURE(0) == 0)
			{
				printf("BREAK\n");
				//getch();
				break;
			}
			printf("Suppress Init ...\n");
			//getch();
			//^2066:2FCC
			glbSpecialScreen = 0;
			//^2066:2FD2
			SUPPRESS_INIT();

			printf("Start Read File Handle %02d ...\n", glbDataFileHandle);
			//s_testSKSave.StartRead(FILE_TELL(glbDataFileHandle));

			printf("Read Global Variables ...\n");
			//^2066:2FD6
			skload_table_60 t1;
			if (SUPPRESS_READER(&t1, _4976_395a, 56, 1, 1) != 0)
				break;
			//^2066:2FF5
			glbGameTick = _4976_4c1a = t1.dwGameTick;	// game tick
			glbRandomSeed = t1.dwRandomSeed;	// random seed
			glbChampionsCount = t1.wChampionsCount;	// player cnt
			glbPlayerPosX = t1.wPlayerPosX;	// column
			glbPlayerPosY = t1.wPlayerPosY;	// row
			glbPlayerDir = t1.wPlayerDir;	// dir
			glbPlayerMap = t1.wPlayerMap;	// map#
			glbChampionLeader = t1.wChampionLeader;	// leader
			glbTimersCount = t1.wTimersCount;	// timer cnt
			_4976_0090 = t1.dw22;
			_4976_4b80 = t1.dw26;
			_4976_4c00 = t1.w30;
			glbPlayerThrowCounter = t1.wPlayerThrowCounter;
			_4976_4c0c = t1.w34;
			glbRainFlagSomething = (Bit8u)t1.wRainFlagSomething;
			glbRainAmbientLightModifier = t1.bRainAmbientLightModifier;
			glbRainDirection = t1.bRainDirection;	// rain dir.
			glbRainStrength = t1.bRainStrength;	// rain str.
			glbRainLevelForSky = t1.bRainLevelForSky;
			glbRainLevelForGround = t1.bRainLevelForGround;
			glbRainMultiplicator = t1.bRainMultiplicator;
			glbRainStormController = t1.wRainStormController;
			glbRainRelated3 = t1.bRainRelated3;
			glbRainRelated2 = t1.bRainRelated2;
			glbRainSpecialNextTick = t1.dwRainSpecialNextTick;
			printf("Read Game Variables (Flags / Bytes / Words) ...\n");
			//getch();
			//^2066:30B5
			if (SUPPRESS_READER(glbIngameGlobVarFlags, _4976_3956, 1, 8, 1) != 0)
				break;
			//^2066:30D3
			if (SUPPRESS_READER(glbIngameGlobVarBytes, _4976_3956, 1, 64, 1) != 0)
				break;
			//^2066:30F1
			if (SUPPRESS_READER(glbIngameGlobVarWords, _4976_3956, 2, 64, 1) != 0)
				break;
			//^2066:310F
			if (SUPPRESS_READER(glbChampionSquad, _4976_3992, 261, glbChampionsCount, 1) != 0) // player ents
				break;
			//^2066:3130
			if (SUPPRESS_READER(&glbGlobalSpellEffects, _4976_3a97, 6, 1, 1) != 0)
				break;
			//^2066:314E
			if (SUPPRESS_READER(glbTimersTable, _4976_3a9d, 10, glbTimersCount, 1) != 0) // timer ents
				break;
			//^2066:3172
			bp06 = glbTimersCount;
			//^2066:3178
			printf("Read Timers ...\n");
			//getch();
			while (bp06 < glbTimersMaximumCount) {
				//^2066:317A
				glbTimersTable[bp06].TimerType(tty00);
				//^2066:318D
				bp06++;
				//^2066:3190
			}
			//^2066:3199
			__SORT_TIMERS();
			//^2066:319F
			if (READ_SKSAVE_DUNGEON() != 0)
				break;
			//^2066:31A9
			PROCEED_GLOBAL_EFFECT_TIMERS();
			_3a15_020f();
			_4976_5bf2 = 1;
			//^2066:31B8
_31b8:		// we jump there from loading a dungeon from new game
			//printf("Ended read dungeon structure.\n"); getch();
			FILE_CLOSE(glbDataFileHandle);
			//^2066:31C2
			si = 0;
			//printf("PROCESS_ACTUATOR_TICK_GENERATOR.\n");
			//getch();
			PROCESS_ACTUATOR_TICK_GENERATOR();
			//^2066:31C9
			_4976_5ca8 = 0;
			//^2066:31CF
			if (glbSpecialScreen != 0) {
				//^2066:31D6
				if (bp04 != 0) {
					//^2066:31DC
					//printf("WAIT_SCREEN_REFRESH.\n");
					//getch();
					WAIT_SCREEN_REFRESH();
				}
				//^2066:31E1
				//^2066:31E3
				//^2066:3227
				_2066_03e0(0);
			}
			else {
				//^2066:31E5
				_4976_5bf6 = 1;
				//^2066:31EB
				if (bp08 != 0) {
					//^2066:31F1
					FILE_RENAME(FORMAT_SKSTR(ptrSKSave_bak, NULL), FORMAT_SKSTR(ptrSKSave_dat, NULL));
				}
				//^2066:3225
				_2066_03e0(1);
			}
			//printf("_0aaf_02f8_DIALOG_BOX.\n");
			//getch();

			//^2066:322C
			_0aaf_02f8_DIALOG_BOX(0x000E, 0x0000);
			//^2066:3237
			_4976_4bd8 = 0x0001;
			glbPlayerDefeated = 0x0000;
			//^2066:3243
			//printf("_2fcf_0b8b.\n");
			//getch();
			_2fcf_0b8b(glbPlayerPosX, glbPlayerPosY, glbPlayerMap);
			//^2066:3257
			glbDoLightCheck = 0x0001;
			//^2066:325D
			//^2066:32B5
			//printf("Return 1.\n"); getch();
			return 1; // Bit16u di = 1; return di;
		} while (false);
	}
	else	// try read as DM1 savegame
	{
		do {
			U8 glbDummyData[10000];
			// there are 0x24c0 bytes of encoded data
			printf("Read as a DM1 savegame ...\n");
			// 42 bytes already been read
			if (FILE_READ(glbDataFileHandle, 0x24C0-42, glbDummyData) == 0)
				break;
		
			// then comes portraits : 4 * 29*16 bytes
			printf("Read Portraits ...\n");
			bDM1PortraitsActivated = true;
			if (FILE_READ(glbDataFileHandle, (29*16), glbDummyData) == 0)
				break;
			memcpy(xDM1PortraitsData[0], glbDummyData, 29*16);
			if (FILE_READ(glbDataFileHandle, (29*16), glbDummyData) == 0)
				break;
			memcpy(xDM1PortraitsData[1], glbDummyData, 29*16);
			if (FILE_READ(glbDataFileHandle, (29*16), glbDummyData) == 0)
				break;
			memcpy(xDM1PortraitsData[2], glbDummyData, 29*16);
			if (FILE_READ(glbDataFileHandle, (29*16), glbDummyData) == 0)
				break;
			memcpy(xDM1PortraitsData[3], glbDummyData, 29*16);
			//if (FILE_READ(glbDataFileHandle, 8, bp26) == 0)

			_4976_524a = (0x24C0 + (4*29*16)); // file cursor to start dungeon structure

			// Init champions
			U8 iChampionIndex = 0;
			for (iChampionIndex = 0; iChampionIndex < 4; iChampionIndex++)
			{
				Champion *xChampion = &glbChampionSquad[iChampionIndex];
				INIT_CHAMPION(xChampion);
			}


			// the regular dungeon structure starts at 0x2C00
			printf("Read Dungeon Structure DM1 Mode ...\n");
			//getch();
			SkCodeParam::bDM1ReadSavegame = true;
			if (READ_DUNGEON_STRUCTURE(0) == 0)
			{
				break;
			}
			_4976_5bf2 = 1; // load is complete
			//getch();
			// Copy from DM2 read
			printf("Suppress Init ...\n");
			glbSpecialScreen = 0;
			SUPPRESS_INIT();
			printf("Start Read File Handle %02d ...\n", glbDataFileHandle);
			printf("Read Global Variables ...\n");

			glbLeaderHandPossession.object = OBJECT_NULL;

			glbGameTick = _4976_4c1a = 1;	// game tick
			glbRandomSeed = 63;	// random seed
			glbChampionsCount = 4;	// player cnt
			glbPlayerPosX = 4;	// column
			glbPlayerPosY = 4;	// row
			glbPlayerDir = 0;	// dir
			glbPlayerMap = 0;	// map#
			glbChampionLeader = 0;	// leader
			glbTimersCount = 0;	// wTimersCount
			_4976_0090 = 0;
			_4976_4b80 = 0;
			_4976_4c00 = 0;
			glbPlayerThrowCounter = 0; //wPlayerThrowCounter
			_4976_4c0c = 0;
			glbRainFlagSomething = 0;
			glbRainAmbientLightModifier = 0;
			glbRainDirection = 0;	// rain dir.
			glbRainStrength = 0;	// rain str.
			glbRainLevelForSky = 0;
			glbRainLevelForGround = 0;
			glbRainMultiplicator = 0;
			glbRainStormController = 0;
			glbRainRelated3 = 0;
			glbRainRelated2 = 0;
			glbRainSpecialNextTick = 0;
/*
			skload_table_60 t1;
			if (SUPPRESS_READER(&t1, _4976_395a, 56, 1, 1) != 0)
				break;
			//^2066:2FF5
			glbGameTick = _4976_4c1a = t1.dwGameTick;	// game tick
			glbRandomSeed = t1.dwRandomSeed;	// random seed
			glbChampionsCount = t1.wChampionsCount;	// player cnt
			glbPlayerPosX = t1.wPlayerPosX;	// column
			glbPlayerPosY = t1.wPlayerPosY;	// row
			glbPlayerDir = t1.wPlayerDir;	// dir
			glbPlayerMap = t1.wPlayerMap;	// map#
			glbChampionLeader = t1.wChampionLeader;	// leader
			glbTimersCount = t1.wTimersCount;	// timer cnt
			_4976_0090 = t1.dw22;
			_4976_4b80 = t1.dw26;
			_4976_4c00 = t1.w30;
			glbPlayerThrowCounter = t1.wPlayerThrowCounter;
			_4976_4c0c = t1.w34;
			glbRainFlagSomething = (Bit8u)t1.wRainFlagSomething;
			glbRainAmbientLightModifier = t1.bRainAmbientLightModifier;
			glbRainDirection = t1.bRainDirection;	// rain dir.
			glbRainStrength = t1.bRainStrength;	// rain str.
			glbRainLevelForSky = t1.bRainLevelForSky;
			glbRainLevelForGround = t1.bRainLevelForGround;
			glbRainMultiplicator = t1.bRainMultiplicator;
			glbRainStormController = t1.wRainStormController;
			glbRainRelated3 = t1.bRainRelated3;
			glbRainRelated2 = t1.bRainRelated2;
			glbRainSpecialNextTick = t1.dwRainSpecialNextTick;
*/

			bp06 = glbTimersCount;

			printf("Read Timers ...\n");
			while (bp06 < glbTimersMaximumCount) {
				glbTimersTable[bp06].TimerType(tty00);
				bp06++;
			}
			__SORT_TIMERS();
			if (READ_SKSAVE_DUNGEON() != 0)
				break;

			PROCEED_GLOBAL_EFFECT_TIMERS();
			_3a15_020f();
			_4976_5bf2 = 1;
			FILE_CLOSE(glbDataFileHandle);
			
			si = 0;
			PROCESS_ACTUATOR_TICK_GENERATOR();
			_4976_5ca8 = 0;
			if (glbSpecialScreen != 0) {
				if (bp04 != 0) {
					WAIT_SCREEN_REFRESH();
				}
				_2066_03e0(0);
			}
			else {
				_4976_5bf6 = 1;
				if (bp08 != 0) {
					FILE_RENAME(FORMAT_SKSTR(ptrSKSave_bak, NULL), FORMAT_SKSTR(ptrSKSave_dat, NULL));
				}
				_2066_03e0(1);
			}
			_0aaf_02f8_DIALOG_BOX(0x000E, 0x0000);
			_4976_4bd8 = 0x0001;
			glbPlayerDefeated = 0x0000;
			_2fcf_0b8b(glbPlayerPosX, glbPlayerPosY, glbPlayerMap);
			glbDoLightCheck = 0x0001;
			return 1;
		} while (false);


	}

_3262:
	//^2066:3262
	if (si != 0) {
		//^2066:3266
		si = 0;
		FILE_CLOSE(glbDataFileHandle);
	}


	//^2066:3272
	_0aaf_0067(_0aaf_02f8_DIALOG_BOX((_4976_5bf2 == 0) ? 0 : ((_4976_5c9c != 0) ? ((_4976_5ca8 != 0) ? 0x13 : 0x14) : (0x07)), 0x001F));
	//^2066:32A6
	if (_4976_5bf2 == 0) {
		//^2066:32AD
		SK_PREPARE_EXIT();
	}
	//^2066:32B2
	Bit16u di = -1;
	//^2066:32B5
	return di;
}

//^0CEE:3035
Bit8u SkWinCore::QUERY_CLS2_OF_TEXT_RECORD(ObjectID recordLink)
{
	//^0CEE:3035
	Text *bp04 = GET_ADDRESS_OF_RECORD2(recordLink);
	if (bp04->TextMode() == 1) {
		//^0CEE:3059
		Bit16u si = bp04->SimpleTextExtUsage();
		//^0CEE:306B
		switch (si) {
			case 0x0000: // 0x00 - Creature transformer
			case 0x0002: // 0x02 - Animate image
			case 0x0005: // 0x05 - ?
			case 0x000D: // 0x0d - 2-state switch
				//^0CEE:3080
				return Bit8u(bp04->OrnateIndex());
		}
	}
	//^0CEE:3091
	return -1;
}

//^0CEE:241B
Bit8u SkWinCore::QUERY_CLS2_FROM_RECORD(ObjectID recordLink)
{
	//^0CEE:241B
	if (recordLink != OBJECT_NULL) {
		//^0CEE:2428
		GenericRecord *bp04;
		if (recordLink < OBJECT_EFFECT_FIREBALL) {	// oFF80
			//^0CEE:242E
			bp04 = GET_ADDRESS_OF_RECORD(recordLink);
		}
		else {
			//^0CEE:243E
			return ((Bit8u)recordLink.w) - 0x80;
		}
		//^0CEE:2446
		switch (recordLink.DBType()) {
			case dbText:		// 2
				//^0CEE:24EA
				return QUERY_CLS2_OF_TEXT_RECORD(recordLink);
			case dbActuator:	// 3
				//^0CEE:24D7
				return GET_WALL_DECORATION_OF_ACTUATOR(GET_ADDRESS_OF_ACTU(recordLink));
			case dbCreature:	// 4
				//^0CEE:2461
				return bp04->castToCreature()->CreatureType();
			case dbWeapon:		// 5
				//^0CEE:246B
				return bp04->castToWeapon()->ItemType();
			case dbCloth:		// 6
				//^0CEE:2478
				return bp04->castToCloth()->ItemType();
			case dbScroll:		// 7
				{
				//^0CEE:2484
				//return 0;
				if (SkCodeParam::bUseScrollIDType)
					return bp04->castToScroll()->ItemType();	// SPX: removed "return 0" to get item type instead, allowing any item within scrolls category
				else
					return 0;	// Standard scroll icon; ID is used for text number
				}
			case dbPotion:		// 8	
				//^0CEE:2488
				return bp04->castToPotion()->PotionType();
			case dbContainer:	// 9
				//^0CEE:2494
				return 0
					| (((reinterpret_cast<Container *>(bp04)->b4 >> 1) & 0x0003) << 3)
					|  ((reinterpret_cast<Container *>(bp04)->b5 >> 5) & 0x0007)
					;
			case dbMiscellaneous_item:	//10
				//^0CEE:24B2
				return bp04->castToMisc()->ItemType();
			case db11:			//11
			case db12:			//12
			case db13:			//13
				break;
			case dbMissile:		//14
				//^0CEE:24BE
				return (Bit8u)QUERY_CLS2_FROM_RECORD(bp04->castToMissile()->GetMissileObject());
			case dbCloud:		//15
				//^0CEE:24CB
				return bp04->castToCloud()->CloudType();
		}
	}
	//^0CEE:24F5
	return -1;
}



//^0CEE:23DE
Bit8u SkWinCore::QUERY_CLS1_FROM_RECORD(ObjectID recordLink)
{
	//^0CEE:23DE
	ObjectID di = recordLink;
	//^0CEE:23E6
	if (di != OBJECT_NULL) {
		//^0CEE:23EB
		Bit16u si = di.DBType();
		if (si == dbMissile) {
			//^0CEE:23FA
			return QUERY_CLS1_FROM_RECORD(GET_ADDRESS_OF_RECORDE(di)->GetMissileObject());
		}
		//^0CEE:240F
		return glbGDAT2CatPerDB[si];
	}
	//^0CEE:2415
	return 0xff;
}

//^3E74:0756
tiamat SkWinCore::_3e74_0756(Bit8u *xx, i32 size) { // TODO: Unr
	Unr(); return tiamat::zero();
}








//^2C1D:132C
X16 SkWinCore::_2c1d_132c(i16 ss, U16 tt)
{
	//^2C1D:132C
	ENTER(0);
	//^2C1D:1330
	X16 si = ss & 0xff;
	if (tt != 0) {
		si = _0cd5_0176(si, 3, ((ss & 0x700) >> 8) +4);
	}
	//^2C1D:1358
	return si;
}

//^0CD5:0176
i16 SkWinCore::_0cd5_0176(U16 val, U16 rshift, U16 multiplier)
{
	//^0CD5:0176
	ENTER(0);
	//^0CD5:0179
	return (U32(val) * U32(multiplier)) >> rshift;
}

//^098D:000F
void SkWinCore::_098d_000f(i16 xx, i16 yy, U16 ww, U16 *x2, U16 *y2)
{
	//^098D:000F
	ENTER(0);
	//^098D:0012
	*x2 = (xx << 2) + (ww % 5);
	*y2 = (yy << 2) + (ww / 5);
	//^098D:0046
	return;
}
//^29EE:000F
void SkWinCore::_29ee_000f()
{
	//^29EE:000F
	ENTER(54);
	//^29EE:0014
	U8 bp01 = (_4976_4ddc != 0) ? 0xe : 2;
	sk3f6c bp36;
	_0b36_0c52(&bp36, 9, 1);
	FILL_ENTIRE_PICT(reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp36.w0)), glbPaletteT16[COLOR_BLACK]);
	//^29EE:0053
	i16 si = 0x28;
	for (; si < 0x2e; bp01 += 2, si++) {
		//^29EE:0058
		DRAW_ICON_PICT_ENTRY(0x1, 0x3, bp01, &bp36, si, -1);
	}
	//^29EE:007E
	_0b36_0cbe(&bp36, 1);
	if (_4976_4ddc != 0) {
		_1031_0541(6);
	}
	else {
		_1031_0541(5);
	}
	//^29EE:00A0
	return;
}
//^2676:000A
void SkWinCore::MARK_DYN_LOAD(U32 aa)
{
	//^2676:000A
	ENTER(4);
	//^2676:000E
	ATLASSERT(_4976_5300 < 400U);
	SkLoadEnt *bp04 = &_4976_52fc[_4976_5300]; _4976_5300++;
	bp04->w0(0);
	bp04->x2.cls1(U8(aa >> 24));
	bp04->x2.cls2(U8(aa >> 16));
	bp04->x2.cls3(U8(aa >>  8));
	bp04->x2.cls4(U8(aa      ));
	//^2676:0068
	return;
}

//^2676:008F
void SkWinCore::MARK_DYN_EXCLUDE_RANGE(U32 ent4, U8 cls4)
{
	//^2676:008F
	ENTER(0);
	//^2676:0092
	MARK_DYN_LOAD(ent4);
	_4976_52fc[_4976_5300 -1].MarkContinuousExcluded();
	MARK_DYN_LOAD((ent4 & 0xffffff00) | cls4);
	//^2676:00CE
	return;
}

//^2676:006A
void SkWinCore::_2676_006a(U32 aa)
{
	//^2676:006A
	ENTER(0);
	//^2676:006D
	MARK_DYN_LOAD(aa);
	_4976_52fc[_4976_5300 -1].MarkExcluded();
	//^2676:008D
	return;
}

//^2676:00D0
void SkWinCore::_2676_00d0(U8 cls1, U8 cls2, U8 cls4)
{
	//^2676:00D0
	ENTER(2);
	//^2676:00D4
	X16 bp02 = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, dtWordValue, cls4);
	if (bp02 == 0)
		return;
	cls2 = U8(bp02);
	if (cls2 != 0) {
		//^2676:00FD
		MARK_DYN_LOAD(cls2 +0xffff0d00);
	}
	//^2676:010F
	bp02 >>= 8;
	if (bp02 != 0) {
		//^2676:011A
		MARK_DYN_LOAD(U8(bp02) +0xffff0d00);
	}
	//^2676:012F
	return;
}

//^01B0:194D
U16 SkWinCore::IS_SCARD_BUSY() //#DS=04BF
{
#if UseAltic
	;
#else
	//^01B0:194D
	ENTER(0);

	DOSIMPL();

//		//^01B0:1950
//		switch (glbSoundCardType) {
//			case 0:
//			case 1:
//			case 3:
//			case 4:
//			case 5:
//				//^01B0:1962
//				return sndLockSoundBuffer;
//
//			case ScardTandy:
//				//^01B0:1968
//				__asm mov ah,0x81
//				__asm int 0x1a
//				__asm rcl ax,1
//				__asm ret
//		}
#endif
	//^01B0:1973
	return 0;
}

//^01B0:1983
void SkWinCore::_01b0_1983() //#DS=04BF
{
	//^01B0:1983
	ENTER(0);
	//^01B0:1986
	LOADDS(0x3083);
	while (IS_SCARD_BUSY() != 0);
	//^01B0:1995
	return;
}

//^47EB:02D3
void SkWinCore::_47eb_02d3(U32 aa)
{
	//^47EB:02D3
	ENTER(0);
	//^47EB:02D6
	_01b0_1983();
	//^47EB:02DE
	return;
}

//^482B:060E
void SkWinCore::_482b_060e()
{
	//^482B:060E
	ENTER(0);
	//^482B:0611
	_47eb_02d3(0);
	_4976_49d0 = 0;
	//^482B:0622
	return;
}

//^3E74:1953
X16 SkWinCore::QUERY_NEXT_GDAT_ENTRY(SkEntIter *ref)
{
//LOGX(("QUERY_NEXT_GDAT_ENTRY from SGDAT ptr %08X : GDAT %s", ref, DEBUG_SKGDATENT(ref)));
	// 0 if not found
	// 1 if found

	//^3E74:1953
	ENTER(22);
	//^3E74:1959
	X8 bp05 = ref->x2.x2.cls2(); // cls2
	X8 bp06 = ref->x2.x2.cls4(); // cls4
	X16 bp0e = ((ref->x2.w0_0_e()) == 1) ? 1 : 0;
	X8 bp07;
	X8 bp08;
	if (bp0e != 0) {
		bp07 = ref->x8.x2.cls2();
		bp08 = ref->x8.x2.cls4();
	}
	else {
		bp07 = bp05;
	}
	//^3E74:199D
	X16 bp0c = ref->w0;
	if (bp0c != 0) {
		ref->w0 = 0;
		ref->cls1cur(ref->x2.x2.cls1());
		//^3E74:19B7
		if (ref->cls1cur() == 0xff) {
			ref->cls1cur(0x00);
			//^3E74:19C0
_19c0:
			ref->cls1base(U8(glbGDatEntries.w12));
		}
		//^3E74:19C8
		else if (bp0e != 0) {
			ref->cls1base(ref->x8.x2.cls1());
			//^3E74:19D9
			if (ref->cls1base() == 0xff) {
				goto _19c0;
			}
		}
		else {
			//^3E74:19DF
			ref->cls1base(ref->x2.x2.cls1());
		}
		//^3E74:19EA
		if (ref->cls1cur() < 0 || ref->cls1cur() > ref->cls1base() || ref->cls1base() > glbGDatEntries.w12)
			goto _1cd4;
	}
	do {
		//^3E74:1A13
		X16 bp0a = glbGDatEntries.pw0[ref->cls1cur()];
		U16 si;
		U16 di;
		if (glbGDatEntries.pw0[ref->cls1cur() +1] != bp0a) {
			//^3E74:1A47
			RawEntry *bp04;
			if (bp0c == 0) {
				si = ref->w24 +1;
				bp04 = ref->pv14 +1;
				if (ref->w26 > si)
					goto _1c0a;
				goto _1c82;
			}
			//^3E74:1A7E
			ref->cls3cur(ref->x2.x2.cls3());
			if (ref->cls3cur() == fmtInvalid) {
				//^3E74:1A8D
				ref->cls3cur(0x00);
				//^3E74:1A92
_1a92:
				ref->cls3base(U8(glbGDatEntries.pw0[ref->cls1cur() +1]) -bp0a -1);
			}
			//^3E74:1AAE
			else if (bp0e != 0) {
				ref->cls3base(ref->x8.x2.cls3());
				//^3E74:1ABF
				if (ref->cls3base() == fmtInvalid)
					goto _1a92;
			}
			else {
				//^3E74:1AC5
				ref->cls3base(ref->x2.x2.cls3());
			}
			//^3E74:1AD3
			bp0a += ref->cls3cur();
			si = ref->w24 = glbGDatEntries.pw4[bp0a++];
			ref->w22 = bp0a;
			ref->w26 = glbGDatEntries.pw4[ref->w22];
			ref->pv14 = bp04 = &glbGDatEntries.pv8[si];
			do {
				//^3E74:1B3E
				if (ref->w26 > si) {
					//^3E74:1B4A
					if (bp05 != 0xff) {
						//^3E74:1B5F
						X8 bp15 = (bp06 == 0xff) ? 0 : bp06;
						RawEntry *bp12 = &glbGDatEntries.pv8[-1];
						di = si;
						X16 bp14 = ref->w26 +1;
						do {
							//^3E74:1B8A
							si = (di +bp14) >> 1;
							bp04 = &bp12[si];
							// SkD((DLV_BUGHERE,"DBG: SI=%04X,DI=%04X,bp14=%04X\n"
							//	, 0U +si, 0U +di, 0U +bp14));
							if (si == di) {
								//^3E74:1BB5
								if (ref->w26 > si) {
									//^3E74:1BC1
									si++;
									bp04++;
									if (bp04->cls2 <= bp07)
										break;
								}
								goto _1c82;
							}
							//^3E74:1BDF
							if (bp04->cls2 == bp05) {
								if (bp04->cls4 <= bp15)
									break;
								//^3E74:1BF3
								goto _1c00;
							}
							//^3E74:1BF5
							if (bp04->cls2 > bp05) {
								//^3E74:1C00
_1c00:
								bp14 = si;
							}
							else {
								//^3E74:1C05
								di = si;
							}
						} while (true);
						//^3E74:1C09
						--si;
					}
_1c0a:
					do {
						//^3E74:1C0A
						if (bp04->cls2 > bp07)
							break;
						//^3E74:1C15
						if ((bp0e != 0) ? ((bp06 == 0xff || (bp04->cls4 >= bp06 && bp04->cls4 <= bp08))) : (bp06 == 0xff || bp04->cls4 == bp06)) {
							//^3E74:1C4A
							ref->w24 = si;
							ref->pv14 = bp04;
							return 1;
						}
						//^3E74:1C65
						if (ref->w26 <= ++si)
							break;
						//^3E74:1C71
						bp04++;
					} while (true);
				}
				//^3E74:1C82
_1c82:
				ref->cls3cur(ref->cls3cur() +1);
				if (ref->cls3cur() > ref->cls3base())
					break;
				si = ref->w26;
				ref->w26 = glbGDatEntries.pw4[++ref->w22];
			} while (true);
		}
		//^3E74:1CB9
		ref->cls1cur(ref->cls1cur() +1);
		if (ref->cls1cur() > ref->cls1base())
			break;
		bp0c = 1;
	} while (true);
_1cd4:
	ref->pv14 = NULL;
	ref->cls1cur(0xff);
	ref->cls1base(0xff);
	return 0;
}

//^3E74:18FA
U16 SkWinCore::TRACK_UNDERLAY(U16 ww)
{
	//^3E74:18FA
	ENTER(0);
	//^3E74:18FF
	X16 di = 0;
	X16 cx = _4976_5d78 +1;
	do {
		X16 si = (di +cx) >> 1;
		if (si == di) {
			return 0xffff;
		}
		if (ww < _4976_5d0c[si -1].w0) {
			cx = si;
			continue;
		}
		if (ww > _4976_5d0c[si -1].w0) {
			di = si;
			continue;
		}
		return _4976_5d0c[si -1].w2;
	} while (true);
}

//^3E74:00ED
X16 SkWinCore::_3e74_00ed(sk5d12 *ref, shelf_memory ps)
{
	//^3E74:00ED
	ENTER(4);
	//^3E74:00F2
	X16 si = 0;
	if (ref == NULL)
		return si;
	//^3E74:010C
	if (ps.AtEMS()) {
		//^3E74:011A
		if (ref->Is4EMS() && ref->t4 <= tiamat(ps) && ref->t0 >= tiamat(ps)) {
			//^3E74:014F
			return 1;
		}
		//^3E74:0154
		return 0;
	}
	//^3E74:0158
	U8 *bp04 = REALIZE_GRAPHICS_DATA_MEMORY(ps);
	if (t2ptr(ref->t4) <= bp04 && t2ptr(ref->t0) >= bp04)
		return 1;
	return 0;
}

tiamat SkWinCore::s2t(shelf_memory s) {
	return tiamat(s);
}
shelf_memory SkWinCore::t2s(tiamat t) {
	switch (t.Area()) {
		case 0:
			return shelf_memory::From_cram(t.val);
		case 1:
			return shelf_memory::From_cems(t.val -0x200000);
	}
	ATLASSERT(false);
	return shelf_memory();
}
U8 *SkWinCore::t2ptr(tiamat t) {
	switch (t.Area()) {
		case 0: return &cram[t.val];
		case 1: return &cems[t.val -0x200000];
	}
	ATLASSERT(false);
	return NULL;
}
tiamat SkWinCore::ptr2t(U8 *pb) {
	U32ptr val;
	val = pb -cram;
	if (val < sizeof(cram)) {
		tiamat v; v.val = (U32)val; return v;
	}
	val = pb -cems;
	if (val < shelf_memory::SizeOf_cems()) {	// replaced sizeof(cems)
		tiamat v; v.val = (U32)val +0x200000; return v;
	}
	ATLASSERT(false);
	return tiamat::zero();
}

//^3E74:3200
void SkWinCore::_3e74_3200(X16 ww) { // TODO: Unr
	Unr();
}

//^4726:0162
void SkWinCore::MAP_EMS_TO_PTR(U32 xx, U32 yy, U8 **ss, U8 **tt)
{
//!	//^4726:0162
//!	ENTER(14);
//!	//^4726:0168
//!	if (xx > yy) {
//!		//^4726:017A
//!		X32 bp0e = xx;
//!		xx = yy;
//!		yy = bp0e;
//!		U8 **bp08 = ss;
//!		ss = tt;
//!		tt = bp08;
//!	}
//!	//^4726:01C2
//!	U16 bp04 = U16(xx >> 14);
//!	U16 di = U16(xx) & 0x3fff;
//!	U16 bp06 = U16(yy >> 14);
//!	U16 si = U16(yy) & 0x3fff;
//!	U8 bp01 = 0;
//!	U8 bp02 = U8(bp06 -bp04);
//!	__asm mov  ah,0x44
//!	__asm mov  al,[bp01]
//!	__asm mov  bx,[bp04]
//!	__asm mov  dx,[_4976_5ea4]
//!	__asm int  0x67
//!	bp01++;
//!	bp04++;
//!	__asm mov  ah,0x44
//!	__asm mov  al,[bp01]
//!	__asm mov  bx,[bp04]
//!	__asm mov  dx,[_4976_5ea4]
//!	__asm int  0x67
//!	*ss = &_4976_5ea0[di];
//!	if (bp02 >= 2) {
//!		//^4726:023A
//!		bp02 = 2;
//!		__asm mov  ah,0x44
//!		__asm mov  al,[bp02]
//!		__asm mov  bx,[bp06]
//!		__asm mov  dx,[_4976_5ea4]
//!		__asm int  0x67
//!		bp02++;
//!		bp06++;
//!		__asm mov  ah,0x44
//!		__asm mov  al,[bp02]
//!		__asm mov  bx,[bp06]
//!		__asm mov  dx,[_4976_5ea4]
//!		__asm int  0x67
//!		bp02 = 2;
//!	}
//!	//^4726:0266
//!	else if (bp02 == 1) {
//!		//^4726:026C
//!		bp02 = 2;
//!		bp06++;
//!		__asm mov  ah,0x44
//!		__asm mov  al,[bp02]
//!		__asm mov  bx,[bp06]
//!		__asm mov  dx,[_4976_5ea4]
//!		__asm int  0x67
//!		bp02 = 1;
//!	}
//!	else {
//!		//^4726:0287
//!		bp02 = 0;
//!	}
//!	//^4726:028B
//!	*tt = &_4976_5ea0[si + (bp02 << 14)];
}

//^4726:02AC
U8 *SkWinCore::_4726_02ac()
{
	//^4726:02AC
	ENTER(4);
	//^4726:02B0
	if (_4976_4968 != 0) {
		//^4726:02B7
		U8 *bp04 = EMS_MAP_BANK_TO_MEMORY(_4976_5eac + (+6), 1);
		WRITE_UI16(bp04,-6,8);
		WRITE_UI16(bp04,-4,_4976_00f6);
		WRITE_UI16(bp04,-2,_4976_00f8);
		return bp04;
	}
	return NULL;
}

//^3E74:2F41
void SkWinCore::_3e74_2f41(sk5d12 *ref)
{
	//^3E74:2F41
	ENTER(28);
	//^3E74:2F47
	shelf_memory bp08 = t2s(ref->t0);
	shelf_memory bp04 = bp08 -2;
	//^3E74:2F63
	while (ref->t8 < tiamat(bp04)) {
		//^3E74:2F66
		X16 si = READ_UI16(REALIZE_GRAPHICS_DATA_MEMORY(bp04),+0);
		i32 bp0c = ((QUERY_GDAT_RAW_DATA_LENGTH(si & 0x7fff) +1) & 0xfffe) +4;
		bp04 -= (bp0c);
		if ((si & 0x8000) == 0) {
			//^3E74:2FA8
			bp08 -= (bp0c);
			if (bp04 + (+2) != bp08) {
				//^3E74:2FC7
				// SPX: I crashed here with custom dungeon/gdat, at that point, map_ems_to_ptr has no effective code.
				if (!SkCodeParam::bUsePowerDebug)
					ATLASSERT(false);
				U32 bp10 = bp04.val -0x001FFFFE;
				U32 bp14 = bp08.val -0x00200000;
				do {
					//^3E74:2FEB
					// SPX: problem here with unitialized bp18 & bp1c and no written code to map_ems_to_ptr
					U8 *bp18 = 0;	// U8 *bp18;
					U8 *bp1c = 0;	// U8 *bp1c;
					MAP_EMS_TO_PTR(bp10, bp14, &bp18, &bp1c);
					if (!SkCodeParam::bUsePowerDebug || (bp18 != 0 && bp1c != 0) )
						COPY_MEMORY(bp18, bp1c, (bp0c > 0x4000) ? 0x4000 : bp0c);
					bp0c -= 0x4000;
					if (bp0c <= 0)
						break;
					bp10 += 0x4000;
					bp14 += 0x4000;
				} while (true);
				//^3E74:3068
				glbShelfMemoryTable[si] = bp08 + (+2);
			}
		}
		//^3E74:3086
	}
	//^3E74:30A3
	ref->t8 = s2t(bp08);
	_4726_02ac();
	//^3E74:30B9
	return;
}

//^3E74:30BD
void SkWinCore::_3e74_30bd(sk5d12 *ref)
{
	//^3E74:30BD
	ENTER(12);
	//^3E74:30C3
	if (ref->t0.IsZero())
		return;
	//^3E74:30DB
	if (ref->Is4EMS() != 0) {
		_3e74_2f41(ref);
		return;
	}
	//^3E74:30F5
	U8 *bp08 = t2ptr(ref->t0);
	U8 *bp04 = PTR_PADD(bp08,-2);
	for (; t2ptr(ref->t8) < bp04; ) {
		//^3E74:311D
		X16 si = READ_UI16(bp04,+0);
		X32 bp0c = ((QUERY_GDAT_RAW_DATA_LENGTH(si & 0x7fff) +1) & 0xfffe) +4;
		bp04 -= bp0c;
		if ((si & 0x8000) == 0) {
			//^3E74:3153
			bp08 -= bp0c;
			if (bp04 +2 != bp08) {
				//^3E74:3180
				COPY_MEMORY(bp04 +2, bp08, bp0c);
				glbShelfMemoryTable[si] = CONVERT_PHYS_TO_SHELF_FORM(bp08 +2);
			}
		}
		//^3E74:31D0
	}
	//^3E74:31EB
	ref->t8 = ptr2t(bp08);
	//^3E74:31FC
	return;
}
//^3E74:0017
X32 SkWinCore::_3e74_0017(sk5d12 *ref)
{
	//^3E74:0017
	ENTER(4);
	//^3E74:001B
	X32 bp04 = 0;
	if (ref->t0.IsntZero()) {
		if (ref->Is4EMS()) {
			bp04 = tiamat::Size(ref->t0, ref->t4);
		}
		else {
			bp04 = tiamat::Size(ref->t0, ref->t4);
		}
	}
	//^3E74:0079
	return bp04;
}

//^3E74:0081
i32 SkWinCore::_3e74_0081(sk5d12 *ref) { // TODO: Unr
	Unr(); return 0;
}

//^3E74:32FB
tiamat SkWinCore::_3e74_32fb(sk5d12 *ss, U8 *tt, i32 ww) { // TODO: Unr
	Unr(); return tiamat();
}

//^0AAF:081B
void SkWinCore::DRAW_DIALOGUE_PROGRESS(X32 xx)
{
	//^0AAF:081B
	ENTER(8);
	//^0AAF:081F
	if (_4976_4bd8 == 0)
		return;
	//^0AAF:0826
	SRECT bp08;
	QUERY_EXPANDED_RECT(0x1da, &bp08);
	bp08.cx = i16((bp08.cx * xx) / 1000);
	if (bp08.cx <= 0)
		return;
	FIRE_FILL_BACKBUFF_RECT(&bp08, glbPaletteT16[COLOR_ORANGE]);
	_0aaf_002f();
	//^0AAF:0878
	return;
}

//^3E74:5133
shelf_memory SkWinCore::_3e74_5133(U8 cls1, U8 cls2, U8 cls3, U8 cls4)
{
	//^3E74:5133
	ENTER(0);
	//^3E74:5137
	U16 si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, cls3, cls4);
	return glbShelfMemoryTable[si];
}
//^47EB:00A4
void SkWinCore::_47eb_00a4(sk5f0a *ref)
{
	//^47EB:00A4
	ENTER(4);
	//^47EB:00A8
	sk5f0a **bp04 = &ref->pv8;
	*bp04 = _4976_5efa;
	_4976_5efa = ref;
	//^47EB:00D7
	return;
}

//^482B:0684
void  SkWinCore::_482b_0684()
{
	//^482B:0684
	ENTER(6);
	//^482B:068A
	for (U16 si = 0; si < _4976_49d4[0][0]; si++) {
		//^482B:068F
		sk5f06 *bp04 = &_4976_5f06[si];
		if (bp04->w5 != 0xffff)
			continue;
		X16 di =  QUERY_GDAT_ENTRY_DATA_INDEX(bp04->category, bp04->index, dtSnd, bp04->entry);
		X16 bp06 = _482b_015c(di);
		if (bp06 != 0) {
			bp04->w0 = _4976_5f06[bp06 -1].w0;
			bp04->w5 = di;
			continue;
		}
		//^482B:06F7
		if (_4976_49d4[0][1] >= _4976_5cae)
			break;
		bp04->w5 = di;
		bp04->w0 = _4976_49d4[0][1];
		_4976_5f0a[bp04->w0].ps0 = _3e74_5133(bp04->category, bp04->index, dtSnd, bp04->entry) + (2);
		//^482B:074F
		_4976_5f0a[bp04->w0].w4 = QUERY_GDAT_ENTRY_DATA_LENGTH(bp04->category, bp04->index, dtSnd, bp04->entry) -2;
		//^482B:0780
		_4976_5f0a[bp04->w0].w6 = 0x157C;	// 5500
		//^482B:0795
		_47eb_00a4(&_4976_5f0a[bp04->w0]);
		_4976_49d4[0][1]++;
		//^482B:07B4
	}
	//^482B:07BE
	return;
}

//^3E74:32A2
tiamat SkWinCore::_3e74_32a2(sk5d12 *ref, i32 xx)
{
	//^3E74:32A2
	ENTER(0);
	//^3E74:32A6
	X16 si = (ref->Is4EMS() != 0) ? 1 : 0;
	if (si != 0) {
		//^3E74:32BE
		ref->t8 -= xx;
	}
	else {
		//^3E74:32D9
		ref->t8 -= xx;
	}
	//^3E74:32ED
	return ref->t8;
}

//^3E74:33A4
void SkWinCore::LOAD_DYN4(SkLoadEnt *ref, i16 aa)
{
	//printf("Entering LOAD_DYN4...\n"); getch();
	//^3E74:33A4
	ENTER(92);
	//^3E74:33AA
	X16 bp38 = 0;
	X16 bp3a = 0;
	U8 *bp04 = ALLOC_MEMORY_RAM(glbGDatNumberOfData, afZeroMem, 1024);
	// flags: XY-#####
	//           ref. cnt
	//         pool flag?
	//        pool flag?
	if (_4976_5c8a == 0) {
		_482b_07c2(0);
	}
	SkLoadEnt *bp08 = ref;
	i16 bp12 = 0;
	//^3E74:33ED
	SkEntIter bp5c;
	X16 si;
	X16 bp2e;
	//printf("LOAD_DYN4 : MARK 1\n"); getch();
	SkLoadEnt bp40; // @28 bp40 // smily :P
	for (; bp12 < aa; bp12++) {
		//^3E74:33F0
		bp5c.w0 = 1;
		bp5c.x2 = *bp08;
		bp2e = bp5c.x2.w0();
		if ((bp2e & 0x7fff) == 1) {
			//^3E74:3419
			bp5c.x8 = bp08[1]; bp08++; bp12++;
		}

LOGX(("LOAD_DYN4: %02d / %02d", bp12, aa));
LOGX(("LOAD_DYN4: MASK: %s", DEBUG_SKLOADENT((U8*)bp08) ));

		SkD((DLV_DYN, "Dyn: Test(%04X,%02X,%02X,%02X,%02X)\n"
			, 0U +bp08->w0(), 0U +bp08->x2.cls1(), 0U +bp08->x2.cls2(), 0U +bp08->x2.cls3(), 0U +bp08->x2.cls4()));
		// ATLASSERT(!(bp08->b2 == 7 && bp08->b3 == 0 && bp08->b4 == 255 && bp08->b5 == 255));
		while (QUERY_NEXT_GDAT_ENTRY(&bp5c) != 0) {
			//^3E74:3441
			X8 bp2c = bp5c.cls3cur();
			if (bp2c == 0xb || bp2c == 0xc || ((si = bp5c.pv14->data) & 0x8000) != 0)
				continue;
			//^3E74:3469
			SkD((DLV_DYN, "Dyn: Match(%02X,%02X,%02X,%02X,%5u)\n"
				, 0U +bp5c.cls1cur(), 0U +bp5c.pv14->cls2, 0U +bp5c.cls3cur(), 0U +bp5c.pv14->cls4, 0U +bp5c.pv14->data));
			U8 bp2b = bp04[si];
			if ((bp2e & 0x8000) != 0) {
				//^3E74:3489
				bp2b &= 0x1f;
				if (bp2b > 0 && bp2b != 0x1f) {
					//^3E74:34A0
					--bp04[si];
				}
				continue;
			}
			//^3E74:34B9
			if (bp2b != 0) {
				if ((bp2b & 0x1f) != 0x1f) {
					//^3E74:34CB
					++bp04[si];
				}
				continue;
			}
			//^3E74:34E4
			if (bp2c == 2) {
				if (_482b_015c(si) != 0 || _4976_5bee[2] != 0)
					continue;
			}
			//^3E74:34E4
			bp04[si] = 1;
			X16 bp36 = _4976_481a[RCJ(15,bp2c)];
			//^3E74:3523
			if ((bp36 & _4976_5d12.w12) != 0 && (bp36 & _4976_5d7e.w12) == 0) {
				//^3E74:3533
				bp04[si] |= 0x40;
			}
			//^3E74:354C
			else if ((bp36 & _4976_5d7e.w12) != 0 && (bp36 & _4976_5d12.w12) == 0) {
				//^3E74:355C
				bp04[si] |= 0x80;
			}
			//^3E74:3573
		}
		//^3E74:3585
		bp08++;
		if (aa -1 == bp12 && bp3a == 0) {
			bp3a = 1;
			bp12--;
			bp08 = &bp40;
			bp40.w0(0);
			bp40.x2 = MAKE_ENT4(0x15,0xfe,0x01,0xfe); // add our smily ":P" once
		}
		//^3E74:35D0
	}
	//printf("LOAD_DYN4 : MARK 2\n"); getch();
	//^3E74:35DE
	for (si = 0; si < glbGDatNumberOfData; si++) {
		//^3E74:35E2
		bp12 = bp04[si];
		if (bp12 != 0) {
			if ((bp12 & 0x1f) != 0) {
				bp04[si] = (bp12 & 0xe0) | 0x21;
			}
			else {
				bp04[si] &= 0x20;
			}
		}
		//^3E74:3640
	}
	//^3E74:3647
	bp08 = ref;
	bp12 = 0;
	//printf("LOAD_DYN4 : MARK 2 B\n"); getch();
	//^3E74:3658
	for (; bp12 < aa; bp08++, bp12++) {
		//^3E74:365B
		//printf("LOAD_DYN4 : MARK 2 B bp12/aa = %d/%d\n", bp12, aa); getch();
		bp5c.w0 = 1;
		bp5c.x2 = *bp08;
		bp2e = bp5c.x2.w0();
		if ((bp2e & 0x7fff) == 1) {
			bp08++; bp12++;
			bp5c.x8 = *bp08;
			if (bp5c.x2.x2.cls3() != fmtInvalid && (bp5c.x2.x2.cls3() > fmtSound || bp5c.x2.x2.cls3() < fmtSound))
				continue;
			bp5c.x8.x2.cls3(fmtSound);
			bp5c.x2.x2.cls3(fmtSound);
		}
		//^3E74:36C0
		if ((bp2e & 0x8000) != 0)
			continue;
		if (bp5c.x2.x2.cls3() != fmtInvalid && bp5c.x2.x2.cls3() != fmtSound)
			continue;
		//^3E74:36D3
		bp5c.x2.x2.cls3(fmtSound);
		//printf("LOAD_DYN4 : MARK 2 B bp12/aa = %d/%d - QUERY_NEXT_GDAT_ENTRY\n", bp12, aa); getch();
		while (QUERY_NEXT_GDAT_ENTRY(&bp5c) != 0) {
			//^3E74:36D9
			si = bp5c.pv14->data & 0x7fff;
			if ((bp04[si] & 1) == 0) {
		//printf("LOAD_DYN4 : MARK 2 B bp12/aa = %d/%d - _482b_015c(%d)\n", bp12, aa, si); getch();
				if (_482b_015c(si) == 0)
					continue;
			}
			//^3E74:3709
			//printf("LOAD_DYN4 : MARK 2 B bp12/aa = %d/%d - _482b_0624(cur)\n", bp12, aa); getch();
			_482b_0624(bp5c.cls1cur(), bp5c.pv14->cls2, bp5c.pv14->cls4);
			//^3E74:3721
		}
		//printf("LOAD_DYN4 : MARK 2 B bp12/aa = %d/%d - QUERY_NEXT_GDAT_ENTRY END LOOP\n", bp12, aa); getch();
		//^3E74:3730
	}
	//printf("LOAD_DYN4 : MARK 2 C\n"); getch();
	//^3E74:374D
	if (_4976_5d78 != 0) {
		for (si = 0; si < glbGDatNumberOfData; si++) {
			//^3E74:375B
			if ((bp04[si] & 1) == 0)
				continue;
			X16 di;
			if ((di = TRACK_UNDERLAY(si)) != 0xffff) {
				while ((bp04[di] & 1) == 0) {
					bp04[di] |= bp04[si] & 0xc1;
					if (TRACK_UNDERLAY(di) == 0xffff)
						break;
				}
			}
			//^3E74:37D2
		}
		//^3E74:37D9
	}
	goto _38db;

	//printf("LOAD_DYN4 : MARK 2 D / FREE_INDEXED_MEMENT\n"); getch();
	do {
		//^3E74:37DC
		FREE_INDEXED_MEMENT(_4976_5d5e->w10());
		//^3E74:37E9
_37e9:
		;
	} while (_4976_5d5e != NULL);
	//^3E74:37FB
	for (si = 0; si < glbGDatNumberOfData; si++) {
		//^3E74:3800
		shelf_memory bp08 = glbShelfMemoryTable[si];
		if (bp08.Absent())
			continue;
		//^3E74:3823
		//printf("LOAD_DYN4 : MARK 2 E / si = %d / #data = %d \n", si, glbGDatNumberOfData); getch();
		if (_3e74_00ed(&_4976_5d12, bp08) == 0 && _3e74_00ed(&_4976_5d7e, bp08) == 0)
			continue;
		bp12 = bp04[si];
		bp12 &= ((bp12 & 1) != 0) ? 0xfff9 : 0x0020;
		bp04[si] = U8(bp12);
		_3e74_3200(si);
		//^3E74:3898
	}
	//printf("LOAD_DYN4 : MARK 3\n"); getch();
	//^3E74:38A2
	_3e74_30bd(&_4976_5d12);
	_3e74_30bd(&_4976_5d7e);
	_4976_5cf8 += tiamat::Size(_4976_5d12.t8, _4976_5d12.t4);
	SkD((DLV_CPX,"CPX: _4976_5cf8 += %u\n", (Bitu)tiamat::Size(_4976_5d12.t8, _4976_5d12.t4)));
	_4976_5d12.t4 = _4976_5d12.t8;
	//^3E74:38DB
_38db:
	X32 bp1a = 0;
	X32 bp16 = 0;
	X32 bp1e = _3e74_0017(&_4976_5d7e);
	U32 bp22 = tiamat::Size(_4976_5d12.t0, ptr2t(reinterpret_cast<U8 *>(_4976_5ce6)));
	//^3E74:3915
	i32 bp10;
	for (si = 0; si < glbGDatNumberOfData; si++) {
		//^3E74:391A
		bp12 = bp04[si] = bp04[si] & 0xe1;
		shelf_memory bp08 = glbShelfMemoryTable[si];
		if (bp08.Present()) {
			//^3E74:3975
			bp10 = ((READ_UI16(REALIZE_GRAPHICS_DATA_MEMORY(bp08),-2) +1) & 0xfffe) +4;
			if (_3e74_00ed(&_4976_5d12, bp08) != 0) {
				//^3E74:39B7
				bp04[si] |= 0x0a;
				if ((bp12 & 1) == 0) {
					bp04[si] |= 0x40;
				}
				//^3E74:39EC
_39ec:
				bp16 += bp10;
			}
			else {
				//^3E74:39FB
				if (_3e74_00ed(&_4976_5d7e, bp08) != 0) {
					bp04[si] |= 0x14;
					if ((bp12 & 1) == 0) {
						bp04[si] |= 0x80;
					}
					goto _3abe;
				}
				//^_3a49
				bp04[si] = 0;
			}
			continue;
		}
		//^3E74:3A62
		if ((bp12 & 1) != 0) {
			bp10 = ((bp08.SizeIfAbsent() +1) & 0xfffe) +4;
			if (bp1e == 0 || (bp12 & 0x40) != 0) {
				//^3E74:3A8D
				bp04[si] |= 8;
				goto _39ec;
			}
			else {
				//^3E74:3AA7
				bp04[si] |= 0x10;
				//^3E74:3ABE
_3abe:
				bp1a += bp10;
			}
		}
		//^3E74:3ACA
	}

	//printf("LOAD_DYN4 : MARK 4\n"); getch();
	//^3E74:3AD4
	if (bp1a > bp1e) {
		//^3E74:3AEC
		for (si = 0; (bp22 -bp16 > _4976_480f) && (bp1a > bp1e) && (si < glbGDatNumberOfData); si++) {
			//^3E74:3AF0
			if ((bp04[si] & 0x94) == 0x10) {
				bp04[si] ^= 0x18;
				bp10 = ((QUERY_GDAT_RAW_DATA_LENGTH(si) +1) & 0xfffe) +4;
				bp16 += bp10;
				bp1a -= bp10;
			}
			//^3E74:3B4A
		}
		//^3E74:3B80
		for (si = 0; (bp22 -bp16 > _4976_480f) && (bp1a > bp1e) && (si < glbGDatNumberOfData); si++) {
			//^3E74:3B84
			if ((bp04[si] & 0x90) == 0x10) {
				bp04[si] ^= 0x18;
				bp10 = ((QUERY_GDAT_RAW_DATA_LENGTH(si) +1) & 0xfffe) +4;
				bp16 += bp10;
				bp1a -= bp10;
			}
			//^3E74:3BDE
		}
		//^3E74:3C14
		for (si = 0; (bp1a > bp1e) && (si < glbGDatNumberOfData); si++) {
			//^3E74:3C18
			if ((bp04[si] & 0x11) == 0x10) {
				bp04[si] = bp04[si] & 0xef;
				bp1a -= ((QUERY_GDAT_RAW_DATA_LENGTH(si) +1) & 0xfffe) +4;
			}
			//^3E74:3C78
		}
		//^3E74:3C91
		if (_4976_5bee[3] == 0) {
			//^3E74:3C9D
			for (si = 0; (bp1a > bp1e) && (si < glbGDatNumberOfData); si++) {
				//^3E74:3CA1
				if ((bp04[si] & 0x94) == 0x10) {
					bp04[si] ^= 0x18;
					bp10 = ((QUERY_GDAT_RAW_DATA_LENGTH(si) +1) & 0xfffe) +4;
					bp16 += bp10;
					bp1a -= bp10;
				}
				//^3E74:3CFB
			}
			//^3E74:3D14
			for (si = 0; (bp1a > bp1e) && (si < glbGDatNumberOfData); si++) {
				//^3E74:3D18
				if ((bp04[si] & 0x90) == 0x10) {
					bp04[si] ^= 0x18;
					bp10 = ((QUERY_GDAT_RAW_DATA_LENGTH(si) +1) & 0xfffe) +4;
					bp16 += bp10;
					bp1a -= bp10;
				}
				//^3E74:3D72
			}
		}
	}
	//^3E74:3D8B
	for (si = 0; (bp1e -bp1a >= 6) && (si < glbGDatNumberOfData); si++) {
		//^3E74:3D8F
		if ((bp04[si] & 0x48) == 8) {
			bp10 = ((QUERY_GDAT_RAW_DATA_LENGTH(si) +1) & 0xfffe) +4;
			if (i32(bp1e -bp1a) >= bp10) {
				//^3E74:3DD8
				bp04[si] ^= 0x18;
				bp1a += bp10;
				bp16 -= bp10;
			}
		}
		//^3E74:3E01
	}
	//^3E74:3E22
	for (si = 0; (bp22 -bp16 <= _4976_480f) && (si < glbGDatNumberOfData); si++) {
		//^3E74:3E26
		if ((bp04[si] & 9) == 8) {
			bp04[si] = bp04[si] & 0xf7;
			bp16 += ((QUERY_GDAT_RAW_DATA_LENGTH(si) +1) & 0xfffe) +4;
		}
		//^3E74:3E86
	}

	//printf("LOAD_DYN4 : MARK 5\n"); getch();
	//^3E74:3EAA
	X32 bp2a = bp22 -bp16;
	_4976_5d62 = bp1e -bp1a;
	_4976_5d26 = bp2a - _4976_5ca0;
	_4976_5d66 = bp2a;
	i32 bp26;
	if (_4976_5c8a == 0) {
		if (_4976_5d26 < 0 || _4976_5d62 < 0) {
			//^3E74:3F1B
			if (bp38 == 0) {
				bp38 = 1;
				goto _37e9;
			}
			//^3E74:3F29
			RAISE_SYSERR(SYSTEM_ERROR__LOAD_DYN4);
		}
		//^3E74:3F31
		bp26 = tiamat::Size(_4976_5d12.t8, _4976_5d12.t0 -bp16);
		//^3E74:3F59
		for (si = 0; si < glbGDatNumberOfData; si++) {
			//^3E74:3F5D
			bp12 = bp04[si];
			if ((bp12 & 6) != 0 && (bp12 & 0x18) == 0) {
				bp04[si] &= 0x20;
				FREE_INDEXED_MEMENT(si);
				_3e74_3200(si);
			}
			//^3E74:3FA7
		}
		//^3E74:3FAE
		if (bp26 > 0) {
			//^3E74:3FBC
			GUARANTEE_FREE_CPXHEAP_SIZE(bp26);
		}
		//^3E74:3FC8
		_3e74_2b30();
		if (bp26 > 0) {
			//^3E74:3FDA
			_4976_5cf8 -= bp26;
			SkD((DLV_CPX,"CPX: _4976_5cf8 -= %u\n", (Bitu)bp26));
			_4976_5d12.t4 = _4976_5d12.t0 -bp16;
		}
		X16 bp32;
		do {
			bp32 = 0;
			//^3E74:4006
			for (si = 0; si < glbGDatNumberOfData; si++) {
				//^3E74:400B
				bp12 = bp04[si];
				tiamat bp08;
				if ((bp12 & 0xc) == 0xc) {
					bp10 = ((QUERY_GDAT_RAW_DATA_LENGTH(si) +1) & 0xfffe) +4;
					if (_3e74_0081(&_4976_5d12) < bp10) {
						//^3E74:405C
						_3e74_30bd(&_4976_5d12);
						if (_3e74_0081(&_4976_5d12) < bp10) {
							//^3E74:407C
							bp32 = 1;
							continue;
						}
					}
					//^3E74:4084
					bp08 = _3e74_32fb(&_4976_5d12, REALIZE_GRAPHICS_DATA_MEMORY(glbShelfMemoryTable[si]), bp10);
					_3e74_3200(si);
					goto _4173;
				}
				//^3E74:40BE
				else if ((bp12 & 0x12) != 0x12) {
					continue;
				}
				else {
					//^3E74:40CC
					bp10 = ((QUERY_GDAT_RAW_DATA_LENGTH(si) +1) & 0xfffe) +4;
					if (_3e74_0081(&_4976_5d7e) < bp10) {
						//^3E74:40F7
						_3e74_30bd(&_4976_5d7e);
						if (_3e74_0081(&_4976_5d7e) < bp10) {
							bp32 = 1;
							continue;
						}
					}
					//^3E74:411A
					bp08 = _3e74_32fb(&_4976_5d7e, REALIZE_GRAPHICS_DATA_MEMORY(glbShelfMemoryTable[si]), bp10);
					_3e74_3200(si);
					if ((_4976_5d7e.w12 & 0x8000) != 0) {
						//^3E74:4159
						glbShelfMemoryTable[si] = t2s(bp08);
					}
					else {
						//^3E74:4173
_4173:
						glbShelfMemoryTable[si] = CONVERT_PHYS_TO_SHELF_FORM(t2ptr(bp08));
					}
					//^3E74:419B
					bp04[si] ^= 6;
					//^3E74:41B2
				}
			}
			//^3E74:41BC
		} while (bp32 != 0);
		//^3E74:41C5
		_3e74_30bd(&_4976_5d12);
		_3e74_30bd(&_4976_5d7e);
		if (bp26 < 0) {
			//^3E74:41E7
			_4976_5cf8 -= bp26;
			SkD((DLV_CPX,"CPX: _4976_5cf8 -= %u\n", (Bitu)bp26));
			_4976_5d12.t4 = _4976_5d12.t0 -bp16;
		}
	//printf("LOAD_DYN4 : MARK 6\n"); getch();
		//^3E74:420E
		X16 bp34 = 0;
		//^3E74:4213
		for (si = 0; si < glbGDatNumberOfData; si++) {
			//^3E74:4218
			if (_4976_4bd8 != 0 && (si & 0x3f) == 0) {
				//^3E74:4225
				DRAW_DIALOGUE_PROGRESS((((si +1) * 500) / glbGDatNumberOfData) + 500);
			}
			//^3E74:4250
			bp12 = bp04[si] & 0xffdf;
			if (bp12 == 0 || (bp12 & 6) != 0)
				continue;
			//^3E74:427F
			X16 bp30;
			bp10 = (((bp30 = QUERY_GDAT_RAW_DATA_LENGTH(si)) +1) & 0xfffe) +4;
			sk5d12 *bp0c = ((bp12 & 8) != 0) ? &_4976_5d12 : &_4976_5d7e;
			//^3E74:42AD
			tiamat bp08 = _3e74_32a2(bp0c, bp10);
			if (bp0c->Is4EMS() != 0) {
				//^3E74:42D1
				WRITE_UI16(REALIZE_GRAPHICS_DATA_MEMORY(t2s(bp08) + (bp10) - (2)),+0,si);
				bp08 = ptr2t(REALIZE_GRAPHICS_DATA_MEMORY(glbShelfMemoryTable[si] = t2s(bp08 + (2))));
			}
			else {
				//^3E74:432A
				glbShelfMemoryTable[si] = CONVERT_PHYS_TO_SHELF_FORM(t2ptr(bp08 +2));
				WRITE_UI16(t2ptr(bp08),+bp10 -4,si);
			}
			//^3E74:4380
			WRITE_UI16(t2ptr(bp08),-2,bp30);
			if (bp34 == 0) {
				_2066_03e0(0);
				GRAPHICS_DATA_OPEN();
				bp34 = 1;
			}
#if DEBUG_SPEC	// SPX: would remove this as debug test ?
			if (si == 1332) {
				U32 pos1 = QUERY_GDAT_RAW_DATA_FILE_POS(1);
				U32 len1 = QUERY_GDAT_RAW_DATA_LENGTH(1);
				ATLASSERT(pos1 == 71728 && len1 == 3836);
				U32 pos = QUERY_GDAT_RAW_DATA_FILE_POS(1332);
				U32 len = QUERY_GDAT_RAW_DATA_LENGTH(1332);
				ATLASSERT(pos == 380964 && len == 168);
			}
#endif
//LOGX(("LOAD_GDAT_RAW_DATA call from LOAD_DYN4"));
			LOAD_GDAT_RAW_DATA(si, glbShelfMemoryTable[si]);
			SkD((DLV_GLD, "GLD: Dyn4-load Raw#%4d at S(%08lX)\n", (Bitu)si, (Bitu)glbShelfMemoryTable[si].val));
			//^3E74:43CC
		}
		//^3E74:43D6
		if (bp34 != 0)
			GRAPHICS_DATA_CLOSE();
		//^3E74:43E0
		_4976_5ce2 = t2ptr(_4976_5d12.t4);
		_482b_0684();
		if (_4976_4bd8 != 0) {
			DRAW_DIALOGUE_PROGRESS(1000);
		}
	}
	//^3E74:4406
	DEALLOC_UPPER_MEMORY(glbGDatNumberOfData);
	//^3E74:4412
	//printf("Returning from LOAD_DYN4...\n");
	return;
}

//^3DF7:0037
// SPX: some sort of init, called from read_structure
void SkWinCore::_3df7_0037(X16 ww)
{
	//^3DF7:0037
	ENTER(4);
	//^3DF7:003B
	if (ww != 0) {
		_4976_5c28 = 0;
		if (glbRainMultiplicator == 0)
			glbRainMultiplicator = 1;
	}
	else {
		//^3DF7:005B
		UPDATE_GLOB_VAR(0x40, 0, 6);
		glbRainSpecialNextTick = glbGameTick +5;
		glbRainAmbientLightModifier = 0;
		glbRainFlagSomething = 0;
		X32 bp04;
		if (_4976_47fc != 0) {
			glbRainLevelForGround = 0;
			bp04 = RAND16(0x1f4);
			glbRainRelated2 = 3;
			glbRainMultiplicator = 1;
		}
		else {
			//^3DF7:00B1
			bp04 = RAND16(0x1f40) +0x1f4;	// RAND16(8000) + 500
			glbRainRelated2 = U8(RAND02());
			glbRainMultiplicator = RAND16(3) +1;
		}
		glbRainLevelForSky = 1;
		glbRainStrength = 0;
		glbRainStormController = 0;
		_4976_5c28 = 0;
		glbRainDirection = U8(RAND02());
		// SPX: This might be the initial rain timer : NOPE, it must be still called ingame (when there is no rain)
		SET_TIMER_WEATHER(bp04);
	}
	//^3DF7:0108
	glbRainThunderRandom = RAND16(3) +2;
	glbRainSomeLightMod = _4976_4764[RCJ(24,((glbGameTick + _4976_5c24) / 0x555) % 0x18)];
	//printf("_3df7_0037 RAIN MARK 5\n");
	_4976_47fc = 0;
	//^3DF7:014B
	return;
}
//^098D:04C7
X16 SkWinCore::_098d_04c7(X16 rcno1, X16 rcno2, X16 scale, X16 *ss, X16 *tt)
{
	//^098D:04C7
	ENTER(10);
	//^098D:04CD
	i16 si = scale;
	SRECT *bp04 = QUERY_RECT(&glbRectNoTable, rcno1);
	if (bp04 == NULL) {
		return -1;
	}
	//^098D:04F2
	SRECT *bp08 = QUERY_RECT(&glbRectNoTable, rcno2);
	if (bp08 == NULL) {
		return -1;
	}
	si = BETWEEN_VALUE(0, si, 0x64);
	if (si == 0) {
		*ss = 0;
		*tt = 0;
		return rcno1;
	}
	//^098D:0535
	i16 bp0a = bp08->cx -bp04->cx;
	*ss = (bp0a * si) / 100;
	i16 di = bp08->cy -bp04->cy;
	*tt = (di * si) / 100;
	return rcno1;
}


//^2676:0131
void SkWinCore::LOAD_LOCALLEVEL_DYN()
{
	//printf("LOAD_LOCALLEVEL_DYN...\n"); getch();
	//^2676:0131
	ENTER(60);
	//^2676:0137
	X16 bp3c = 0;
	_4976_5300 = 0;
	//printf("ALLOC_MEMORY_RAM...\n"); getch();
	_4976_52fc = reinterpret_cast<SkLoadEnt *>(ALLOC_MEMORY_RAM(sizeof(SkLoadEnt) * 400, 0,  1024));
	U8 *bp14 = ALLOC_MEMORY_RAM(0xfa, afZeroMem, 1024); // creature existance-on-level bytes?
	U8 *bp18 = ALLOC_MEMORY_RAM(0xfa, afZeroMem, 1024); // wall ornate existance-on-level bytes?
	U8 *bp1c = ALLOC_MEMORY_RAM(0xfa, afZeroMem, 1024); // floor ornate existance-on-level bytes?
	//^2676:01A6
	//printf("PLENTY OF MARK_DYN_LOAD...\n"); getch();
	MARK_DYN_LOAD(0x01ff02ff); // Mark: Interface - main screen, snd, all
    MARK_DYN_LOAD(0x18ff02ff); // Mark: Teleporter, all, snd, all
	MARK_DYN_LOAD(0x07ff02ff); // Mark: Interface - character sheet, all, snd, all
	if (_4976_5bee[0] != 0) {
		MARK_DYN_LOAD(0x0dff02ff); // Mark: Missiles, all, snd, all
	}
	else {
		//^2676:01D9
		MARK_DYN_LOAD(0x0d0002ff); // Mark: Missiles, fireball, snd, all
		_4976_52fc[_4976_5300 -1].MarkContinuous();
		MARK_DYN_LOAD(0x0d2f02ff); // Mark: Missiles, 0x2F(?), snd, all
		MARK_DYN_LOAD(0x0d7e02ff); // Mark: Missiles, 0x7E(?), snd, all
		_4976_52fc[_4976_5300 -1].MarkContinuous();
		MARK_DYN_LOAD(0x0d9f02ff); // Mark: Missiles, 0x9F(?), snd, all
	}
	//^2676:0231
	MARK_DYN_LOAD(0x10ff02ff); // Mark: Weapons, all, snd, all
	_4976_52fc[_4976_5300 -1].MarkContinuous();
	MARK_DYN_LOAD(0x15ff02ff); // Mark: Misc, all, snd, all
	MARK_DYN_LOAD(0x03ff02ff); // Mark: Messages, all, snd, all
	MARK_DYN_LOAD(0x08fe02ff); // Mark: Dungeon graphics, 0xFE, snd, all
	MARK_DYN_LOAD(0x16fe02ff); // Mark: Champions, 0xFE, snd, all
	MARK_DYN_LOAD(0x09fe02ff); // Mark: Wall ornates, 0xFE, snd, all
	MARK_DYN_LOAD(0x0afe02ff); // Mark: Floor ornates, 0xFE, snd, all
	MARK_DYN_LOAD(0x01ffffff); // Mark: Interface - main screen, all, all, all
	MARK_DYN_LOAD(0x01000400); // Mark: Interface - main screen, 0x00, rect, 0x00
	_4976_52fc[_4976_5300 -1].MarkExcluded();
	MARK_DYN_LOAD(0x01000600); // Mark: Interface - main screen, 0x00, raw06, 0x00
	_4976_52fc[_4976_5300 -1].MarkExcluded();
	MARK_DYN_LOAD(0x0100070a); // Mark: Interface - main screen, 0x00, raw07, 0x0A
	_4976_52fc[_4976_5300 -1].MarkExcluded();
	MARK_DYN_LOAD(0x1cffffff); // Mark: Japanese fonts, all, all, all
	MARK_DYN_LOAD(0x1a80ffff); // Mark: Dialog boxes, Load/cancel, all, all
	MARK_DYN_LOAD(0x1a81ffff); // Mark: Dialog boxes, Save/cancel, all, all
	MARK_DYN_LOAD(0x0300ffff); // Mark: Messages, 0x00, all, all
	MARK_DYN_LOAD(0x0700ffff); // Mark: Interface - character sheet, 0x00, all, all
	if (_4976_5bee[0] != 0) {
		MARK_DYN_LOAD(0x0dffffff); // Mark: Missiles, all, all, all
	}
	else {
		MARK_DYN_LOAD(0x0d00ffff); // Mark: Missiles, fireball, all, all
		_4976_52fc[_4976_5300 -1].MarkContinuous();
		MARK_DYN_LOAD(0x0d2fffff); // Mark: Missiles, 0x2F(?), all, all
		MARK_DYN_LOAD(0x0d7effff); // Mark: Missiles, 0x7E(?), all, all
		_4976_52fc[_4976_5300 -1].MarkContinuous();
		MARK_DYN_LOAD(0x0d9fffff); // Mark: Missiles, 0x9F(?), all, all
	}
	//^2676:039D
	MARK_DYN_LOAD(0x10ffffff); // Mark: Weapons, all, all, all
	_4976_52fc[_4976_5300 -1].MarkContinuous();
	MARK_DYN_LOAD(0x15ffffff); // Mark: Misc, all, all, all
	MARK_DYN_LOAD(0xffff01f9); // Mark: All, all, image, 0xF9(Map chip)
	MARK_DYN_EXCLUDE_RANGE(0x0fff0510, 0x39); // Mark: Creature, all, text, 0x10(My enemies) to 0x39(?)
	//printf("SERIE ON MAP...\n"); getch();
	//^2676:03E1
	_4976_52fc[_4976_5300 -1].MarkIncluded();
	U8 *bp08 = *glbCurrentTileMap;
	OID_T *bp10 = &dunGroundStacks[*_4976_4c52]; // U16 *bp10
	i16 bp26 = 0;
	i16 si;
	for (; bp26 < glbCurrentMapWidth; bp26++) {
		//^2676:0429
		i16 bp28 = 0;
		for (; bp28 < glbCurrentMapHeight; bp28++) {
			//^2676:0431
			U16 bp2a = *(bp08++);
			if ((bp2a & 0x10) != 0) {
				ObjectID di = *bp10; bp10++;
				//^2676:0450
				do {
					Actuator *bp0c;
					Text *bp20;
					switch (di.DBType()) {
						case dbActuator: //^_0468
							//^2676:0468
							if (_4976_5bee[0] != 0)
								break;
							bp0c = GET_ADDRESS_OF_ACTU(di);
							switch (bp0c->ActuatorType()) {
								case ACTUATOR_TYPE_CREATURE_GENERATOR: // 0x2E: Creature generator
									//^2676:049B
									if (U8(bp2a >> 5) != ttWall)
										break;
									bp14[bp0c->ActuatorData()] = 1;
									break;
								case ACTUATOR_TYPE_CHAMPION_MIRROR: // SPX: Add for DM1 retrocompatibility / 0x7F: Activator, champion mirror
								case ACTUATOR_TYPE_RESURECTOR: // 0x7E: Activator, resuscitation
									//^2676:04C1
									MARK_DYN_LOAD((U8(bp0c->ActuatorData()) << 16) + 0x1600ffff); // Mark: Champions, xxx, all, all
									break;
								case ACTUATOR_FLOOR_TYPE__CROSS_SCENE: // 0x27: Cross scene
									//^2676:04E3
									if (U8(bp2a >> 5) != ttTeleporter)
										break;
									si = bp0c->ActuatorData() -1;
									if (si == glbCurrentMapIndex)
										break;
									U8 *bp24 = &glbMapTileValue[si << 2][dunMapsHeaders[si].RawColumn()][dunMapsHeaders[si].RawRow()];
									si = dunMapsHeaders[si].CreaturesTypes() -1;
									for (; si >= 0; si--) {
										//^2676:0580
										bp14[bp24[si]] |= 2;
									}
									break;
							}
							break;
						case dbText: //^_0599
							//^2676:0599
							bp20 = GET_ADDRESS_OF_RECORD2(di);
							if (bp20->TextMode() != 1)
								break;
							switch (bp20->SimpleTextExtUsage()) {
								case 14://^05D5
									//^2676:05D5
									bp3c = 1;
									break;
								case 0://^05DC
								case 2://^05DC
								case 4://^05DC
								case 5://^05DC
								case 6://^05DC
								case 7://^05DC
								case 8://^05DC
								case 10://^05DC
								case 13://^05DC
								case 15://^05DC
								case 16://^05DC
								case 17://^05DC
									//^2676:05DC
									if (U8(bp2a >> 5) == ttWall) {
										bp18[bp20->OrnateIndex()] = 1;
										break;
									}
									else {
										bp1c[bp20->OrnateIndex()] = 1;
										break;
									}
								case 1://^061A
								case 3://^061A
								case 9://^061A
								case 11://^061A
								case 12://^061A
									break;
							}
							break;
					}
					//^2676:061A
					di = GET_NEXT_RECORD_LINK(di);
					//^2676:0623
				} while (di != OBJECT_END_MARKER);
			}
			//^2676:062B
		}
		//^2676:063A
	}
	//^2676:0649
	U16 bp02;
	if (_4976_5bee[0] == 0) {
		//^2676:0655
		for (bp02 = 0; bp02 < glbChampionsCount; bp02++) {
			//^2676:065D
			MARK_DYN_LOAD(0x16000100 + (glbChampionSquad[bp02].HeroType() << 16)); // Mark: Champions, xxx, image, 0x00(wake)
			//^2676:067E
			MARK_DYN_LOAD(0x160002ff + (glbChampionSquad[bp02].HeroType() << 16)); // Mark: Champions, xxx, snd, all
			//^2676:069F
			MARK_DYN_EXCLUDE_RANGE(0x16000508 + (glbChampionSquad[bp02].HeroType() << 16), 0xb); // Mark: Champions, xx, text, 0x08 to 0x0B
			//^2676:06C3
			_4976_52fc[_4976_5300 -1].MarkIncluded();
			//^2676:06D9
		}
	}
	//^2676:06E8
	if (_4976_5bee[0] != 0 || (_4976_5c8a != 0 && glbChampionsCount != 4)) {
		//^2676:06FD
		// SPX: x16 GDAT2 Champion category
		MARK_DYN_LOAD(0x16ffffff); // Mark: Champions, all, all, all
	}
	//^2676:0708
	if (_4976_5bee[0] != 0 || dunMapLocalHeader->UseTeleporter() != 0) {
		// SPX: x18 GDAT2 Teleporter category
		MARK_DYN_LOAD(0x18ffffff); // Mark: Teleporter, all, all, all
	}
	//glbMapDoorType[0] = dunMapLocalHeader->DoorType0();
	//glbMapDoorType[1] = dunMapLocalHeader->DoorType1();
	//^2676:072C
	glbMapDoorType[0] = (dunMapLocalHeader->UseDoor0() != 0) ? dunMapLocalHeader->DoorType0() : 0xff;
	glbMapDoorType[1] = (dunMapLocalHeader->UseDoor1() != 0) ? dunMapLocalHeader->DoorType1() : 0xff;
	//printf("LOAD_LEVEL : Doors 1/2 are : %02d/%02d\n", glbMapDoorType[0], glbMapDoorType[1]);
	//^2676:076C
	X16 bp2e;
	X16 bp2c;
	if (_4976_5bee[0] != 0) {
		bp2e = 1;
		MARK_DYN_LOAD(0x0effffff); // Mark: Doors, all, all, all
	}
	else {
		bp2e = 0;
		bp2c = 0;
		for (bp02 = 0; bp02 < 2; bp02++) {
			//^2676:078E
			U8 bp03 = glbMapDoorType[bp02];
			if (bp03 != 0xff) {
				bp2c = 1;
				if (QUERY_GDAT_ENTRY_DATA_INDEX(0xe, bp03, dtWordValue, 0x40) == 0) {
					bp2e = 1;
				}
				MARK_DYN_LOAD(0x0e00ffff + (bp03 << 16)); // Mark: Doors, xxx, all, alll
			}
			//^2676:07CE
		}
		//^2676:07D7
		if (bp2c != 0) {
			MARK_DYN_LOAD(0x0efe02ff); // Mark: Doors, 0xFE, snd, all
		}
	}
	//printf("QUERY_GDAT_ENTRY_DATA_INDEX GFX SET...\n"); getch();
	//^2676:07E9
	U8 bp03 = dunMapLocalHeader->MapGraphicsStyle();
	glbMapGraphicsSet = bp03;
	glbMistyMap = QUERY_GDAT_ENTRY_DATA_INDEX(0x8, bp03, dtWordValue, 0x69);
	si = QUERY_GDAT_ENTRY_DATA_INDEX(0x8, dunMapLocalHeader->MapGraphicsStyle(), dtWordValue, 0x6b);
	if (si != 0) {
		bp1c[si & 0xff] = 1;
	}
	//^2676:0843
	if (_4976_5bee[0] != 0)
		bp03 = 0xff;
	//^2676:084E
	MARK_DYN_LOAD(0x0800ffff + (bp03 << 16)); // Mark: Dungeon graphics, xxx, all, all
	MARK_DYN_LOAD(0x1700ffff + (bp03 << 16)); // Mark: Skullkeep and weather, xxx, all, all
	if (_4976_5bee[0] == 0) {
		//^2676:0884
		if (bp2e == 0) {
			//^2676:088A
			MARK_DYN_EXCLUDE_RANGE(0x08000106 + (bp03 << 16), 0x1a); // Mark: Dungeon graphics, xxx, image, 0x06 to 0x1A (door's track far)
			MARK_DYN_EXCLUDE_RANGE(0x080001d3 + (bp03 << 16), 0xd8); // Mark: Dungeon graphics, xxx, image, 0xD3 to 0xD8 (door's track near)
		}
		//^2676:08BD
		if (dunMapLocalHeader->UseUpperPitfall() == 0) {
			MARK_DYN_EXCLUDE_RANGE(0x0800016b + (bp03 << 16), 0x79); // Mark: Dungeon graphics, xxx, image, 0x6B to 0x79 (visible lower pitfall)
		}
		//^2676:08E5
		if (dunMapLocalHeader->UseLowerPitfall() == 0) {
			MARK_DYN_EXCLUDE_RANGE(0x08000199 + (bp03 << 16), 0xa0); // Mark: Dungeon graphics, xxx, image, 0x99 to 0xA0 (visible upper pitfall)
		}
		//^2676:0910
		if (dunMapLocalHeader->w2_2_2() == 0) {
			MARK_DYN_EXCLUDE_RANGE(0x08000182 + (bp03 << 16), 0x89); // Mark: Dungeon graphics, xxx, image, 0x82 to 0x89 (invisible lower pitfall)
		}
		//^2676:093C
		if (dunMapLocalHeader->UseUpStaircase() == 0) {
			MARK_DYN_EXCLUDE_RANGE(0x080001c7 + (bp03 << 16), 0xcc); // Mark: Dungeon graphics, xxx, image, 0xC7 to 0xCC (up stair case parts)
		}
		//^2676:0968
		if (dunMapLocalHeader->UseDownStaircase() == 0) {
			MARK_DYN_EXCLUDE_RANGE(0x080001cd + (bp03 << 16), 0xd2); // Mark: Dungeon graphics, xxx, image, 0xCD to 0xD2 (down stair case parts)
		}
		//^2676:0994
		if (dunMapLocalHeader->w2_6_6() == 0 && bp3c == 0) {
			//^2676:09AC
			_2676_006a(0x08000103 + (bp03 << 16));
			MARK_DYN_EXCLUDE_RANGE(0x080001fc + (bp03 << 16), 0xfe); // Mark: Dungeon graphics, xxx, image, 0xFC to 0xFE (message board)
		}
	}
	//printf("QUERY_GDAT_ENTRY_DATA_INDEX RAIN...\n"); getch();
	//^2676:09DC
	glbRainSceneType = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, GDAT_GFXSET_SCENE_RAIN);	// 0x66
	if (_4976_5bee[0] == 0 && tlbRainScene[RCJ(5,glbRainSceneType)][0] != 0) {
		MARK_DYN_LOAD(0x0d30ffff); // Mark: Missiles, thunder, all, all
	}
	//^2676:0A13
	for (bp02 = 0; dunMapLocalHeader->WallGraphics() > bp02; bp02++) {
		bp18[_4976_4dc2[bp02]] = 1;
	}
	//^2676:0A3F
	if (_4976_5bee[0] != 0) {
		MARK_DYN_LOAD(0x09ffffff); // Mark: Wall ornates, all, all, all
	}
	else {
		for (bp02 = 0; bp02 < 0xfa; bp02++) {
			//^2676:0A5A
			if (bp18[bp02] != 0) {
				MARK_DYN_LOAD(0x0900ffff + (bp02 << 16)); // Mark: Wall ornates, xxx, all, all
				_2676_00d0(0x9, U8(bp02), 0x40);
			}
			//^2676:0A8A
		}
	}
	//^2676:0A94
	for (bp02 = 0; bp02 < dunMapLocalHeader->FloorGraphics(); bp02++) {
		bp1c[_4976_4cba[bp02]] = 1;
	}
	//^2676:0AC0
	if (_4976_5bee[0] != 0) {
		MARK_DYN_LOAD(0x0affffff); // Mark: Floor ornates, all, all, all
	}
	else {
		for (bp02 = 0; bp02 < 0xfa; bp02++) {
			//^2676:0ADB
			if (bp1c[bp02] != 0) {
				MARK_DYN_LOAD(0x0a00ffff + (bp02 << 16)); // Mark: Floor ornates, xxx, all, all
			}
			_2676_00d0(0xa, U8(bp02), 0x40);
			//^2676:0B0B
		}
	}
	//^2676:0B15
	if (_4976_5bee[0] != 0) {
		MARK_DYN_LOAD(0x0bffffff); // Mark: Door ornates, all, all, all
	}
	else {
		for (bp02 = 0; dunMapLocalHeader->DoorDecorationGraphics() > bp02; bp02++) {
			//^2676:0B30
			MARK_DYN_LOAD(0x0b00ffff + (glbMapDoorOrnatesList[bp02] << 16)); // Mark: Door ornates, xxx, all, all
			//^2676:0B49
		}
	}
	//^2676:0B5C
	if (_4976_5bee[0] != 0 || bp2e != 0) {
		MARK_DYN_LOAD(0x0cffffff); // Mark: Door buttons, all, all, all
	}
	//^2676:0B74
	for (bp02 = 0; bp02 < dunMapLocalHeader->CreaturesTypes(); bp02++) {
		//^2676:0B7B
		bp14[glbMapCreaturesTypeNumber[bp02]] = 1;
		//^2676:0B90
	}
	//^2676:0BA6
	if (_4976_5bee[0] != 0) {
		MARK_DYN_LOAD(0x0fffffff); // Mark: Creatures, all, all, all
	}
	//^2676:0BB8
	for (bp03 = 0; bp03 < 0xfa; bp03++) {
		//^2676:0BBF
		if (QUERY_GDAT_CREATURE_WORD_VALUE(bp03, 6) != 0) {
			bp14[bp03] = 1;
		}
		//^2676:0BDE
		if (bp14[bp03] == 0 || _4976_5bee[0] != 0)
			continue;
		//^2676:0BFD
		if ((bp14[bp03] & 1) != 0 || _4976_5bee[1] != 0) {
			//^2676:0C14
			MARK_DYN_LOAD(0x0f00ffff + (bp03 << 16)); // Mark: Creatures, xxx, all, all
			MARK_DYN_EXCLUDE_RANGE(0x0f0001fa + (bp03 << 16), 0xfd); // Mark: Creatures, xxx, image, 0xFA to 0xFD (mask?)
			_2676_00d0(0xf, bp03, 0x40);
		}
		else {
			//^2676:0C54
			MARK_DYN_EXCLUDE_RANGE(0x0f00fffa + (bp03 << 16), 0xfd); // Mark: Creatures, xxx, all, 0xFA to 0xFD (mask?)
			_4976_52fc[_4976_5300 -1].MarkIncluded();
		}
		//^2676:0C83
	}
	//^2676:0C8F
	//printf("LOAD_DYN:_482b_060e...\n"); getch();
	if (_4976_5c8a == 0)
		_482b_060e();
	//^2676:0C9B
	//printf("LOAD_DYN:LOAD_DYN4...\n"); getch();
	LOAD_DYN4(_4976_52fc, _4976_5300);
	//printf("LOAD_DYN:LOAD_MISCITEM...\n"); getch();
	if (_4976_5c8a == 0)
		LOAD_MISCITEM();
	//printf("LOAD_DYN:glb Rain...\n"); getch();
	//^2676:0CBB
	glbRainHasThunderImage = U8(QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_ENVIRONMENT, glbMapGraphicsSet, dtImage, GDAT_ENVWTH_THUNDER_1)); // 0x64 thunder
	glbRainHasWetGround = U8(QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_ENVIRONMENT, glbMapGraphicsSet, dtImage, GDAT_ENVWTH_WETGROUND_1)); // 0x6a wet floor
	glbRainHasCloudSky = U8(QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_ENVIRONMENT, glbMapGraphicsSet, dtImage, GDAT_ENVWTH_CLOUDSKY_1)); // 0x67 ceil cloud
	glbRainHasRainFall = U8(QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_ENVIRONMENT, glbMapGraphicsSet, dtImage, GDAT_ENVWTH_RAINFALL_STRAIGHT_1)); // 0x71 vertical direct rain fall
	glbSceneColorKey = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, GDAT_GFXSET_SCENE_COLORKEY); // colorkey
    glbSceneFlags = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, GDAT_GFXSET_SCENE_FLAGS);
    //printf("DEALLOC_UPPER_MEMORY...\n"); getch();
	//^2676:0D39
	DEALLOC_UPPER_MEMORY(0xfa);
	DEALLOC_UPPER_MEMORY(0xfa);
	DEALLOC_UPPER_MEMORY(0xfa);
	DEALLOC_UPPER_MEMORY(0x960);
	if (_4976_5c8a != 0)
		return;
	//^2676:0D73
	i16 bp38;
	i16 bp3a;
	QUERY_GDAT_IMAGE_METRICS(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_FLOOR, &bp38, &bp3a); // floor
	U16 bp32 = (bp38 & 0xffff) * bp3a;
	//^2676:0D99
	i16 bp34;
	i16 bp36;
	QUERY_GDAT_IMAGE_METRICS(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_CEIL, &bp34, &bp36); // ceil
	U16 bp30 = (bp34 & 0xffff) * bp36;
	//^2676:0DBF
	_4976_5be6 = _4976_00f4 -bp30;
	if (_4976_5be6 > bp32) {
		_4976_5be6 -= bp32;
		SET_SRECT(&_4976_5938, 0, bp36, _4976_00f6, _4976_00f8 -(bp36 +bp3a));
	}
	else {
		_4976_5be6 = 0;
	}
	//^2676:0DFE
	//printf("UPDATE_WEATHER...\n"); getch();
	_4976_022c = 1;
	UPDATE_WEATHER(0);
	RECALC_LIGHT_LEVEL();
	//^2676:0E11
	//printf("LOAD_LOCALLEVEL_DYN END!\n"); getch();
	return;
}

//^32CB:2730
void SkWinCore::CHANCE_TABLE_OPERATION()
{
	//^32CB:2730
	ENTER(12);
	//^32CB:2736
	if (_4976_5aa2 == 0 || glbTableToMove != OBJECT_NULL)
		return;
	//^32CB:274A
	i16 bp0a;
	i16 bp0c;
	_443c_08ab(&bp0a, &bp0c, &_4976_5dae.rc4.cy);
	i16 bp02 = bp0a - _4976_00e8;
	i16 di = bp0c - _4976_00ea;
	if (bp02 < 0 || di < 0 || bp02 >= _4976_00f6 || glbScreenHeight - _4976_00ea <= di)
		return;
	//^32CB:279D
	bp02 -= _4976_00f6 >> 1; // horz pos in viewport
	di -= _4976_00f8 >> 1; // vert pos in viewport
	X16 si;
	X16 bp04;
	X16 bp06;
	X16 bp08;
	if (ABS16(bp02) < 20) {
		//^32CB:27BA
		if (ABS16(di) < 15)
			goto _28bc;
		si = (di < 0) ? 0 : 3; // 0=move forward, 3=move back
	}
	//^32CB:27D6
	else if (bp02 < 0) {
		//^32CB:27DC
		if (ABS16(di) < 15) {
			si = 5; // 5=move forward left
		}
		else if (di < 0) {
			si = 0; // 0=move forward
		}
		else {
			si = 4; // 4=move back left
		}
	}
	else if (ABS16(di) < 15) {
		//^32CB:2804
		si = 1; // 1=move forward right
	}
	else if (di < 0) {
		//^32CB:280D
		si = 0;
	}
	else {
		//^32CB:280F
		si = 2; // 2=move back right
	}
	//^32CB:2812
	bp04 = (_4976_4366[RCJ(6,si)] + glbPlayerDir) & 3;
	bp06 = glbPlayerPosX;
	bp08 = glbPlayerPosY;
	bp06 += glbXAxisDelta[bp04];
	bp08 += glbYAxisDelta[bp04];
	if (IS_TILE_BLOCKED(GET_TILE_VALUE(bp06, bp08)) == 0) {
		//^32CB:2861
		bp04 = (_4976_4360[RCJ(6,si)] + glbPlayerDir) & 3;
		if (IS_CREATURE_MOVABLE_THERE(glbPlayerPosX + glbXAxisDelta[glbPlayerDir], glbPlayerPosY + glbYAxisDelta[glbPlayerDir], bp04, NULL) != 0) {
			//^32CB:28A3
			HIGHLIGHT_ARROW_PANEL(_4976_435a[RCJ(6,si)], _4976_434e[RCJ(6,si)], 1);
			goto _28bf;
		}
	}
	//^32CB:28BC
_28bc:
	si = 6;
_28bf:
	glbTargetTypeMoveObject = si;
	//^32CB:28C3
	return;
}

//^32CB:4069
void SkWinCore::_32cb_4069(i16 xx, i16 yy, X16 ww, U16 *ss, U16 *tt)
{
	//^32CB:4069
	ENTER(12);
	//^32CB:406F
	i16 si = ww;
	if (si == 0)
		return;
	//^32CB:4079
	if (si > 3)
		si = 3;
	si--;
	//^32CB:4082
	i16 di = *ss;
	i16 bp02 = *tt;
	i16 bp04 = ABS16(xx -di);
	if (bp04 == 0) {
		if (yy < bp02) {
			*tt -= _4976_44ba[RCJ(3,si)];
		}
		else {
			*tt += _4976_44ba[RCJ(3,si)];
		}
		return;
	}
	//^32CB:40C8
	i16 bp06 = ABS16(bp02 -yy);
	if (bp06 == 0) {
		if (xx < di) {
			//^32CB:40E1
			*ss -= _4976_44ba[RCJ(3,si)];
			return;
		}
		else {
			//^32CB:40EB
			*ss += _4976_44ba[RCJ(3,si)];
			return;
		}
	}
	//^32CB:40F9
	X16 bp0a = !(((yy < bp02) ? 1 : 0) ^ ((xx < di) ? 1 : 0));
	X16 bp0c = !(((bp04 < bp06) ? 1 : 0) ^ bp0a);
	X16 bp08 = (((xx < di) ? 1 : 0) << 2) | (bp0a << 1) | bp0c;
	*ss += _4976_447a[si][bp08][0];
	*tt += _4976_447a[si][bp08][1];
	//^32CB:4181
	return;
}

//^098D:0C13
void SkWinCore::TRIM_BLIT_RECT(i16 xx, i16 yy, i16 ss, i16 tt)
{
	//^098D:0C13
	ENTER(0);
	//^098D:0C16
	SET_SRECT(&_4976_4bca, xx, yy, _4976_00f6 -ss -xx, _4976_00f8 -tt -yy);
	//^098D:0C43
	return;
}
//^32CB:59CA
// SPX: _32cb_59ca renamed SET_GRAPHICS_FLIP_FROM_POSITION
X16 SkWinCore::SET_GRAPHICS_FLIP_FROM_POSITION(X16 aa, X16 ww, X16 xx, X16 yy)
{
	//^32CB:59CA
	ENTER(0);
	//^32CB:59CE
	X16 si = (xx +yy +ww + dunMapLocalHeader->MapOffsetX() + dunMapLocalHeader->MapOffsetY() + dunMapLocalHeader->Level()) & 1;
	switch (aa) {
		case 1: //^_5a0a
			//^32CB:5A0A
			if ((glbSceneFlags & 8) != 0) {
				if ((glbSceneFlags & 0x10) != 0) {
					//^32CB:5A1A
					if ((U16(glbGameTick) & 7) > 3)
						return 1;
					return 0;
				}
				//^32CB:5A33
				return si;
			}
			//^32CB:5A35
			if ((glbSceneFlags & 0x40) != 0)
				return glbPlayerDir & 1;
			return 0;
		case 0x20: //^_5a41
			//^32CB:5A41
			if ((glbSceneFlags & 2) != 0) {
				if ((glbSceneFlags & 4) != 0) {
					if ((U16(glbGameTick) & 7) <= 3)
						return 1;
					return 0;
				}
				//^32CB:5A6D
				return !si;
			}
			//^32CB:5A76
			if ((glbSceneFlags & 0x20) != 0)
				return glbPlayerDir & 1;
			//^32CB:5A86
			return 0;
		default: //^_5a8a
			//^32CB:5A8A
			return si;
	}
}
//^32CB:076E
void SkWinCore::DRAW_DUNGEON_GRAPHIC(U8 cls1, U8 cls2, U8 cls4, X16 rectno, i16 colorkey, U16 mirrorflip)
{
	//^32CB:076E
	ENTER(314);
	//^32CB:0773
	X16 si = rectno;
	ExtendedPicture bp013a;
	QUERY_GDAT_SUMMARY_IMAGE(&bp013a, U8(cls1), U8(cls2), U8(cls4));
	_0b36_037e(bp013a.b58, U8(_4976_5a88), colorkey, -1, bp013a.w56);
	//^32CB:07AB
	bp013a.colorKeyPassThrough = colorkey;
	bp013a.rectNo = si;
	bp013a.pb44 = _4976_4c16;
	bp013a.mirrorFlip = mirrorflip;
	if (glbIsPlayerMoving != 0) {
		if (si == 700) {
			bp013a.w34 += _4976_00fa;
		}
		else if (si == 701) {
			bp013a.w34 += _4976_00fc;
		}
	}
	// SPX : special fix DM1
	if (SkCodeParam::bDM1Mode && rectno == 754) // roof door slit D1
		bp013a.w30 -= 3;
	//^32CB:07EB
	DRAW_PICST(QUERY_PICST_IT(&bp013a));
	//^32CB:0801
	return;
}
//^098D:0C45
void SkWinCore::_098d_0c45()
{
	//^098D:0C45
	ENTER(0);
	//^098D:0C48
	_4976_4bca.cx = 0;
	//^098D:0C4E
	return;
}

//^32CB:5C67
void SkWinCore::_32cb_5c67()
{
	//^32CB:5C67
	ENTER(8);
	//^32CB:5C6D
	i16 si;
	for (si = 0; si < 0x15; si++) {
		//^32CB:5C72
		if (tblCellTilesRoom[si].xsrd.w0 != 1)
			continue;
		//^32CB:5C86
		TELE_inf bp08;
		if (GET_TELEPORTER_DETAIL(&bp08, U8(tblCellTilesRoom[si].posx), U8(tblCellTilesRoom[si].posy)) == 0)
			continue;
		//^32CB:5CBC
        i16 di;
		for (di = 0; di < 4; di++) {
			//^32CB:5CC0
			U8 bp01 = _4976_408c[RCJ(21,si)][RCJ(4,di)];
			if (bp01 < 0)
				continue;
			//^32CB:5CD0
			if (tblCellTilesRoom[bp01].xsrd.w0 != 7)
				continue;
			//^32CB:5CE4
			tblCellTilesRoom[bp01].xsrd.w0 = (0);
			//^32CB:5CFA
		}
		//^32CB:5D00
	}
	//^32CB:5D09
	return;
}
//^32CB:5A8F
void SkWinCore::_32cb_5a8f()
{
	//^32CB:5A8F
	ENTER(14);
	//^32CB:5A94
	X32 bp04 = 0;
	X32 bp08 = 0;
	i16 bp0e = 0;
	for (; bp0e < 0x10; bp0e++) {
		//^32CB:5AAF
		if (tblCellTilesRoom[bp0e].xsrd.w0 == 0) {
			//^32CB:5AC4
			bp08 |= U32(1) << bp0e;
		}
		//^32CB:5AD7
	}
	//^32CB:5AE0
	bp04 |= _4976_4684[RCJ(4,U16(bp08 >> 1) & 3)];
	bp04 |= _4976_4694[RCJ(8,(U16(bp08 >> 3) & 7) | (((U16(bp08) &    8) != 0) ? (U16(bp08     ) & 6) : 0))];
	bp04 |= _4976_46b4[RCJ(8,(U16(bp08 >> 6) & 7) | (((U16(bp08) & 0x40) != 0) ? (U16(bp08 >> 3) & 6) : 0))];
	//^32CB:5B7D
	bp04 |= _4976_46d4[RCJ(8,(U16(bp08 >>11) & 7) | (((U16(bp08) &0x800) != 0) ? (U16(bp08 >> 6) & 6) : 0))];
	//^32CB:5BC1
	X16 si = U16(bp08) | U16(bp04);
	if ((si & 0x1000) != 0 && ((si & 0x4000) != 0 || (si & 0x200) != 0)) {
		//^32CB:5BDB
		bp04 |= 0x280000;
	}
	//^32CB:5BE5
	if ((si & 0x2000) != 0 && ((si & 0x8000) != 0 || (si & 0x400) != 0)) {
		//^32CB:5BF7
		bp04 |= 0x500000;
	}
	//^32CB:5C01
	X32 bp0c = 1;
	//^32CB:5C0B
	for (bp0e = 0; bp0e <= 0x16; bp0e++) {
		//^32CB:5C12
		if ((bp04 & bp0c) != 0 && tblCellTilesRoom[bp0e].xsrd.w0 == 0) {
			//^32CB:5C37
			tblCellTilesRoom[bp0e].xsrd.w0 = (7);
		}
		//^32CB:5C4B
		bp0c = bp0c << 1;
		//^32CB:5C5B
	}
	//^32CB:5C64
	return;
}

//^48AE:01AF
U8 SkWinCore::_48ae_01af(X16 xx, X16 yy)
{
	//^48AE:01AF
	ENTER(0);
	//^48AE:01B4
	X16 di = xx;
	if ((di & 0x400) == 0 || (di & 0x200) != 0) {
		return 15;
	}
	//^48AE:01CD
	X16 si = di & 15;
	if (si != 0) {
		//^48AE:01D8
		// TODO: fixme
		return READ_UI8(&_4976_49d4[RCJ(5,si)][0], yy);
	}
	return 0;
}

//^32CB:347F
// SPX: _32cb_347f renamed MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE
void SkWinCore::MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE(X16 xx, ObjectID rl, i16 yy, X16 zz)
{
	//^32CB:347F
	ENTER(4);
	//^32CB:3485
	sk4d1a *bp04 = &_4976_4d1a[xx];
	if (_4976_4d1a[xx].w8 == OBJECT_NULL) {
		//^32CB:34A7
		COPY_MEMORY(&glbTempPicture.rc36, &bp04->rc0, sizeof(SRECT));
		i16 si = bp04->rc0.cy;
		if (si < 15) {
			if (si < 11) {
				//^32CB:34D0
				bp04->rc0.cy = 11;
			}
			//^32CB:34D6
			bp04->rc0.y += (si >> 1) -7;
		}
		//^32CB:34E4
		si = bp04->rc0.cx;
		if (si < 14) {
			//^32CB:34F5
			bp04->rc0.cx = 14;
			bp04->rc0.x += si -7;
		}
	}
	else {
		X16 di;
		//^32CB:3506
		di = max_value(bp04->rc0.x + bp04->rc0.cx, glbTempPicture.rc36.x + glbTempPicture.rc36.cx);
		bp04->rc0.x = min_value(bp04->rc0.x, glbTempPicture.rc36.x);
		bp04->rc0.cx = di - bp04->rc0.x;
		//^32CB:354A
		di = max_value(bp04->rc0.y + bp04->rc0.cy, glbTempPicture.rc36.y + glbTempPicture.rc36.cy);
		bp04->rc0.y = min_value(bp04->rc0.y, glbTempPicture.rc36.y);
		bp04->rc0.cy = di + bp04->rc0.y;
	}
	//^32CB:358A
	_4976_4d1a[xx].b11 = U8(zz);
	_4976_4d1a[xx].w8 = rl;
	_4976_4d1a[xx].b10 = U8(yy);
	//^32CB:35BD
	return;
}

//^32CB:3991
// SPX: _32cb_3991 renamed DRAW_PUT_DOWN_ITEM
void SkWinCore::DRAW_PUT_DOWN_ITEM(ObjectID rl, i16 cellPos, i16 dir, Creature *ref)
{
	//^32CB:3991
	ENTER(22);
	//^32CB:3997
	ObjectID di = rl;
	if (di == OBJECT_END_MARKER)
		return;
	//^32CB:39A2
	i16 bp0a = glbTabYAxisDistance[RCJ(23,cellPos)];
	if (bp0a > 3) // ignore D4
		return;
	X16 si = 0;
	X16 bp10;
	X16 bp12;
	if (ref != NULL) {
		//^32CB:39C0
		AIDefinition *bp04 = QUERY_CREATURE_AI_SPEC_FROM_TYPE(ref->CreatureType());
		bp10 = bp04->w30;
		bp12 = bp04->w32;
	}
	do {
		i16 bp0c = di.DBType();
		if (bp0c < dbWeapon || bp0c > dbMiscellaneous_item)
			continue;
		//^32CB:3A02
		if (((di.Dir() - _4976_5aa0) & 3) != dir)
			continue;
		//^32CB:3A16
		ObjectID bp06;
		X16 bp08;
		X16 bp0e;
		i16 bp14;
		i16 bp16;
		if (ref != NULL) {
			if ((bp10 & 0x400) == 0 && ((bp10 >> 4) & 15) == 0) {
				//^32CB:3A33
				QUERY_EXPANDED_RECT(bp12, &_4976_4d1a[_4976_4dde].rc0);
				_4976_4d1a[_4976_4dde].b11 = 2;
				_4976_4d1a[_4976_4dde].w8 = di;
				_4976_4d1a[_4976_4dde].b10 = X8(cellPos);
				_4976_4dde++;
				return;
			}
			//^32CB:3A80
			DRAW_ITEM(di, cellPos, 0, 0, si, ref, 1, 0, 0);
			si++;
			si &= 15;
			if ((U8(_48ae_01af(bp10, 0)) & (1 << dir)) != 0) {
				//^32CB:3ABE
				bp08 = dir +4;
				bp0e = 2;
				goto _3b77;
			}
		}
		else {
			//^32CB:3ACF
			DRAW_ITEM(di, cellPos, 0, 0, si, NULL, 1, 0, 0);
			si++;
			si &= 15;
			if (cellPos == 0 || (cellPos == 3 && dir > 1)) {
				//^32CB:3B06
				bp14 = tblCellTilesRoom[cellPos].posx;
				bp16 = tblCellTilesRoom[cellPos].posy;
				bp06 = _1c9a_03cf(&bp14, &bp16, di.Dir());
				if (bp06 == OBJECT_NULL || IS_CREATURE_FLOATING(bp06) != 0 || (_0cee_2df4(bp06) & 0x2000) != 0) {
					//^32CB:3B6C
					bp08 = dir;
					bp0e = 1;
					//^32CB:3B77
_3b77:
					MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE(bp08, di, cellPos, bp0e);
				}
			}
		}
		//^32CB:3B88
	} while ((di = GET_NEXT_RECORD_LINK(di)) != OBJECT_END_MARKER);
	//^32CB:3B99
	return;
}

//^48AE:07BF
X16 SkWinCore::DIR_FROM_5x5_POS(X16 _5x5)
{
	//^48AE:07BF
	ENTER(0);
	//^48AE:07C3
	X16 si = _5x5;
	if (si == 6) // north west
		return 0;
	if (si == 8) // north east
		return 1;
	if (si == 18) // south east
		return 2;
	if (si == 16) // south west
		return 3;
	if (si == 12) // center
		return 4;
	//^48AE:07F7
	return -1;
}

//^32CB:3E08
void SkWinCore::_32cb_3e08(ObjectID rl, X16 xx, U32 yy, Creature *ref)
{
	//^32CB:3E08
	ENTER(12);
	//^32CB:3E0E
	i16 di = xx;
	if (rl == OBJECT_END_MARKER || _4976_415b[RCJ(23,di)] == -1)
		return;
	//^32CB:3E24
	_4976_421a = OBJECT_END_MARKER;
	if (di > 15)
		return;
	//^32CB:3E32
	X16 bp0a = _4976_4453[RCJ(16,di)];
	if (bp0a == 3)
		return;
    //^32CB:3E43
	U32 bp04 = 0;
	i16 si;
	for (si = 0; si < 4; si++) {
		//^32CB:3E51
		X16 bp0c = _4976_4447[RCJ(3,bp0a)][RCJ(4,si)];
		U32 bp08 = U32(1) << bp0c;
		if ((bp08 & yy) != 0) {
			//^32CB:3E82
			DRAW_PUT_DOWN_ITEM(rl, di, DIR_FROM_5x5_POS(bp0c), ref);
			continue;
		}
		//^32CB:3E9F
		bp04 |= bp08;
		//^32CB:3EAB
	}
	//^32CB:3EB1
	if (bp04 != 0) {
		_4976_421a = rl;
		_4976_5a8a = bp04;
		_4976_5a8e = ref;
	}
	//^32CB:3ED9
	return;
}

//^32CB:2BE6
void SkWinCore::SUMMARY_DRAW_CREATURE(ObjectID rl, i16 cellPos, U32 ss)
{
	// SkD((DLV_BUGHERE, "DBG: SUMMARY_DRAW_CREATURE(%04X,%5d,%08X)\n"
	//	, rl.w, cellPos, ss));

	//^32CB:2BE6
	ENTER(14);
	//^32CB:2BEC
	ObjectID di = rl;
	X16 si = cellPos;
	if (glbIsPlayerMoving != 0 && si == 0)
		return;
	//^32CB:2C00
	i16 bp0e = glbTabYAxisDistance[RCJ(23,si)];
	//^32CB:2C08
	for (; di != OBJECT_END_MARKER; di = GET_NEXT_RECORD_LINK(di)) {
		//^32CB:2C0B
		if (di.DBType() != dbCreature)
			continue;
		//^32CB:2C1B
		Creature *bp04 = GET_ADDRESS_OF_RECORD4(di);
		CreatureInfoData *bp08;
		if (bp04->b5_0_7() == 0xff) {
			//^32CB:2C32
			bp08 = 0;
		}
		else {
			//^32CB:2C3E
			bp08 = &glbTabCreaturesInfo[bp04->b5_0_7()];
		}
		//^32CB:2C5C
		AIDefinition *bp0c = QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp04->CreatureType());
		QUERY_CREATURE_PICST(si, bp0e, bp04, bp08, di);
		DRAW_TEMP_PICST();
		if (bp0c->IsStaticObject() == 0 || bp0c->w30_0_3() == 0 || bp0e >= 4)
			continue;
		//^32CB:2CA4
		if (tblCellTilesRoom[si].xsrd.w0 == 0x11) {
			//^32CB:2CB8
			ss = 0x3ff;
		}
		//^32CB:2CC2
		_32cb_3e08(bp04->GetPossessionObject(), si, ss, bp04);
		//^32CB:2CDE
	}
	//^32CB:2CEF
	return;
}

//^32CB:2CF3
void SkWinCore::_32cb_2cf3(U8 cls2, U16 scale64, U16 mirrorFlip, U16 rectno)
{
	//^32CB:2CF3
	ENTER(0);
	//^32CB:2CF7
	U16 si = scale64;
	si = BETWEEN_VALUE(8, si & 0xfffe, 64);
	QUERY_GDAT_SUMMARY_IMAGE(&glbTempPicture, 0xd, cls2, 0x41);
	glbTempPicture.w32 = glbTempPicture.w28;
	glbTempPicture.w34 = glbTempPicture.w30;
	glbTempPicture.w30 = glbTempPicture.w28 = 0;
	glbTempPicture.w26 = 0;
	glbTempPicture.rectNo = rectno;
	glbTempPicture.colorKeyPassThrough = 10;	// hard set color key!
	glbTempPicture.pb44 = _4976_4c16;
	glbTempPicture.mirrorFlip = mirrorFlip;
	glbTempPicture.w54 = glbTempPicture.w52 = si;
	_0b36_037e(glbTempPicture.b58, i8(_4976_5a88), 10, -1, glbTempPicture.w56);
	QUERY_PICST_IT(&glbTempPicture);
	//^32CB:2D89
	return;
}

//^32CB:2D8C
void SkWinCore::_32cb_2d8c(ObjectID rl, X16 yy, X32 aa)
{
	//^32CB:2D8C
	ENTER(46);
	//^32CB:2D92
	X16 di = yy;
	ObjectID bp0c = rl;
	if (bp0c == OBJECT_END_MARKER)
		return;
	//^32CB:2DA3
	if (_4976_4172[RCJ(23,di)] < 0)
		return;
	//^32CB:2DAD
	X16 bp1c = glbTabYAxisDistance[RCJ(23,di)];
	i16 si = 0;
	for (; si < 5; si++) {
		//^32CB:2DBA
		X16 bp1a = _4976_436c[RCJ(4,_4976_5aa0)][RCJ(5,si)];
		rl = bp0c;
		do {
			//^32CB:2DD2
			if (rl.DBType() == dbCloud) {
				//^32CB:2DE3
				i16 bp18 = rl.Dir();
				Cloud *bp08 = GET_ADDRESS_OF_RECORDF(rl);
				X8 bp15 = bp08->CloudType();
				if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_SPELL_MISSILES, bp15, dtImage, 0x14) != 0) {	// specifically to handle "reflector" haze
					if (si == 4) {
						DRAW_TELEPORTER_TILE(di, GDAT_CATEGORY_SPELL_MISSILES, bp15);
					}
					continue;
				}
				//^32CB:2E34
				if (((bp08->b2_7_7() != 0) ? ((bp1a == 0xffff) ? 1 : 0) : ((bp1a == bp18) ? 1 : 0)) == 0)
					continue;
				//^32CB:2E6A
				X16 bp26 = 0;
				X16 bp0e = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_SPELL_MISSILES, bp15, dtWordValue, 0) & RAND02();
				X16 bp14;
				if (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_SPELL_MISSILES, bp15, dtWordValue, 0x41) != 0) {
					//^32CB:2EA3
					bp14 = 0xff;
				}
				else {
					//^32CB:2EAA
					bp14 = bp08->b3_0_f();
				}
				//^32CB:2EB7
				if (di == 0) {
					//^32CB:2EBE
					bp26 = 1;
					if (bp15 == 0x30) {
						//^32CB:2EC9
						UPDATE_GLOB_VAR(0x41, 1, 3);
					}
					else {
						//^32CB:2EDA
_2eda:
						X16 bp0a = 0;
						bp18 = bp14 >> 5;
						if (bp18 != 0) {
							bp0a++;
							if (bp18 > 3)
								bp0a++;
						}
						//^32CB:2EF8
						U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_SPELL_MISSILES, bp15, U8(bp0a) +0x42);
						ALLOC_IMAGE_MEMENT(GDAT_CATEGORY_SPELL_MISSILES, bp15, bp0a +0x42);
						X16 bp24 = READ_UI16(bp04,-2) * READ_UI16(bp04,-4);
						//^32CB:2F34
						SRECT bp2e;
						_44c8_20a4(
							bp04,
							_4976_4c16,
							NULL,
							QUERY_EXPANDED_RECT(4, &bp2e),
							(RAND() & 0x1f) + (bp24 - 0x28),
							RAND16(bp24 -0x28),
							_4976_00f6,
							0,
							_32cb_0649(0xd, bp15, bp0a +0x42, 0)
							);
						continue;
					}
				}
				else {
					//^32CB:2FB2
					if (bp15 == 0x30) {
						UPDATE_GLOB_VAR(0x40, 1, 3);
					}
					//^32CB:2FC6
					if (bp1c == 0 && si >= 3)
						continue;
				}
				//^32CB:2FD4
				X16 bp1e;
				if (bp08->b2_7_7() != 0) {
					bp1e = 12;
				}
				else {
					bp1e = _4976_014c[RCJ(4,bp18)];
				}
				//^32CB:2FF8
				X16 bp20 = ROTATE_5x5_POS(bp1e, _4976_5aa0);
				if (((1 << bp20) & aa) == 0)
					continue;
				//^32CB:3027
				i16 bp22 = _4976_418e[RCJ(6,bp1c)][2 -(bp20 / 5)];
				X16 bp10 = QUERY_CREATURE_BLIT_RECTI(di, bp1e, glbPlayerDir) |0x8000;
				X16 bp12 = (max_value(0x30, bp14 +1) * bp22) >> 8;
				_32cb_2cf3(bp15, bp12, bp0e, bp10);
				i16 bp0a = bp14 -0xa0;
				if (bp0a > 0) {
					bp0a = 0xc0 - bp0a;
					if (bp26 != 0 && glbTabYAxisDistance[0] == bp1c)
						//^32CB:30A9
						bp0a = 0x1e;
					//^32CB:30AE
					glbTempPicture.w34 += (_4976_00f8 << 3) / bp0a;
					if (bp08->b2_7_7() == 0) {
						//^32CB:30CD
						bp0a = (_4976_00f6 << 3) / bp0a;
						if (si == 1 || si == 4) {
							//^32CB:30E4
							bp0a = -bp0a;
						}
						//^32CB:30EC
						glbTempPicture.w32 += bp0a;
					}
				}
				//^32CB:30F3
				glbTempPicture.w32 = CALC_STRETCHED_SIZE(glbTempPicture.w32, bp22);
				glbTempPicture.w34 = CALC_STRETCHED_SIZE(glbTempPicture.w34, bp22);
				DRAW_TEMP_PICST();
				if (bp26 != 0) {
					goto _2eda;
				}
			}
			//^32CB:3122
		} while ((rl = GET_NEXT_RECORD_LINK(rl)) != OBJECT_END_MARKER);
		//^32CB:3136
	}
	//^32CB:313F
	return;
}
//^32CB:3EDD
void SkWinCore::_32cb_3edd(i16 xx)
{
	//^32CB:3EDD
	ENTER(0);
	//^32CB:3EE0
	if (_4976_421a != OBJECT_END_MARKER) {
		//^32CB:3EE7
		_32cb_3e08(_4976_421a, xx, _4976_5a8a, _4976_5a8e);
	}
	_4976_421a = OBJECT_END_MARKER;
	//^32CB:3F0B
	return;
}

//^32CB:4681
void SkWinCore::DRAW_DEFAULT_DOOR_BUTTON(U8 cls1, U8 cls2, U8 cls4, i16 iViewportCell)	// U8 cls1, U8 cls2, U8 cls4, i16 ww
{
	//^32CB:4681
	ENTER(4);
	//^32CB:4687
	X16 iViewportCellLocal = iViewportCell;	// X16 di = ww
	i16 iButtonRectno = tlbRectnoDoorButton[RCJ(14,iViewportCellLocal)]; // i16 si
	if (iButtonRectno < 0 || cls2 == 0xff)
		return;
	//^32CB:469B
    X16 iYDistance = glbTabYAxisDistance[RCJ(23,iViewportCellLocal)]; // X16 bp04
	X16 iStretchScale = tlbDistanceStretch[RCJ(5,iYDistance)];	// X16 bp02
	QUERY_TEMP_PICST(0, iStretchScale, iStretchScale, 0, 0, iYDistance, 
		QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOOR_BUTTONS, cls2, dtWordValue, 8) * 5 + iButtonRectno +1950,
		-1, glbSceneColorKey, -1, cls1, cls2, cls4
		);
	DRAW_TEMP_PICST();
	//^32CB:46F9
	if (iButtonRectno == 3 || iButtonRectno == 4) {	// 3 is D1, 4 is D0
		//^32CB:4703
		MAKE_BUTTON_CLICKABLE(&glbTempPicture.rc36, 4, U8(iViewportCellLocal));
	}
	//^32CB:4711
	return;
}

//^32CB:4715
// SPX: _32cb_4715 renamed DRAW_DOOR_FRAMES
void SkWinCore::DRAW_DOOR_FRAMES(i16 iViewportCell, X16 iDoorFrameDisplayFlags)	// i16 xx, X16 yy
{
	//^32CB:4715
	ENTER(16);
	//^32CB:471B
	//i16 si = iViewportCell; // si
	X8 gfxset = glbMapGraphicsSet;	// bp0f
	X16 colorkey = glbSceneColorKey;		// di
	ObjectID bp0e = tblCellTilesRoom[iViewportCell].xsrd.w6[1];
	//yy = 7;
	Door *door = GET_ADDRESS_OF_RECORD0(bp0e);	//*bp04
	// SPX: 0x40 = GDAT_DOOR_NO_FRAMES entry. Only used for the ROOTS door type (BETA)
	if (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, glbMapDoorType[door->DoorType()], dtWordValue, GDAT_DOOR_NO_FRAMES) == 0) {
		//^32CB:4770
		if ((iDoorFrameDisplayFlags & DRAW_DOOR_FLAG_CEILING_SLIT) != 0) { // (0x01) ceiling slit
			//^32CB:4777
			X8 bp05 = tlbGraphicsDoorRoofSlits[RCJ(14,iViewportCell)];	// 0x12 to 0x17 is door ceiling slit
			if (bp05 != 0xff) {
				//^32CB:4782
				// tlbRectnoDoorRoofSlits
				DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, gfxset, bp05, tlbRectnoDoorRoofSlits[RCJ(14,iViewportCell)], colorkey, DRAW_FLAG_FLIP_NONE);
			}
		}
		//^32CB:479B
		X16 iLocalCell;	// X16 bp0c
		X16 iFrameLeft;	// X16 bp08
		X16 iFrameRight;	// X16 bp0a
		if (glbGeneralFlipGraphics != 0) {
			//^32CB:47A2
			iLocalCell = tlbDoorSideFramesReorder[RCJ(23,iViewportCell)];
			iFrameLeft = 1;	// 1 = right
			iFrameRight = 0;	// 0 = left
		}
		else {
			//^32CB:47B6
			iLocalCell = iViewportCell;
			iFrameLeft = 0;	// 0 = left
			iFrameRight = 1;	// 1 = right
		}
		//^32CB:47C3
		if ((iDoorFrameDisplayFlags & DRAW_DOOR_FLAG_LEFT_FRAME) != 0) {	// (0x02) left door frame
			//^32CB:47CA
			// X8 bp05 = tlbGraphicsDoorSideFrames[RCJ(14,bp0c)][RCJ(2,bp08)]
			X8 iDoorFrameGfx = tlbGraphicsDoorSideFrames[RCJ(14,iLocalCell)][RCJ(2,iFrameLeft)];	// door side frames (28 slots divided into 2 parts (2 * 14))
			if (iDoorFrameGfx != 0xFF) {
				//^32CB:47DD
				QUERY_TEMP_PICST(0, 64, 64, 0, 0, 0, QUERY_CREATURE_BLIT_RECTI(iViewportCell, 10, 0), 4, colorkey, -1, GDAT_CATEGORY_GRAPHICSSET, gfxset, iDoorFrameGfx); // door frame left
				// SPX: DM1 frame adjustment for pixel perfect
				if (SkCodeParam::bDM1Mode) {
					glbTempPicture.w28 -= 2;
					glbTempPicture.w30 += 4;
				}
				//else { // DM2 V5 adjustment
				//	glbTempPicture.w28 -= 10;
				//	glbTempPicture.w30 += 5;
				//}
				DRAW_TEMP_PICST();
			}
		}
		//^32CB:480E
		if ((iDoorFrameDisplayFlags & DRAW_DOOR_FLAG_RIGHT_FRAME) != 0) {	// (0x04) right door frame (holding button)
			//^32CB:4818
			// X8 bp05 = tlbGraphicsDoorSideFrames[RCJ(14,bp0c)][RCJ(2,bp0a)];
			X8 iDoorFrameGfx = tlbGraphicsDoorSideFrames[RCJ(14,iLocalCell)][RCJ(2,iFrameRight)];
			if (iDoorFrameGfx != 0xFF) {
				//^32CB:482E
				QUERY_TEMP_PICST(1, 64, 64, 0, 0, 0, QUERY_CREATURE_BLIT_RECTI(iViewportCell, 14, 0), 3, colorkey, -1, GDAT_CATEGORY_GRAPHICSSET, gfxset, iDoorFrameGfx); // door frame right
				// SPX: DM1 frame adjustment for pixel perfect
				if (SkCodeParam::bDM1Mode) {
					glbTempPicture.w28 += 2;
					glbTempPicture.w30 += 4;
				}
				//else { // DM2 V5 adjustment
				//	glbTempPicture.w28 += 10;
				//	glbTempPicture.w30 += 5;
				//}
				DRAW_TEMP_PICST();
				//^32CB:485F
				if (door->Button() != 0) {
					//^32CB:4870
					// SPX: Here, it is always the default 0 ... How could we custom this?
					// At least, it could be set along the current wallset index? or door type ? ...
					DRAW_DEFAULT_DOOR_BUTTON(GDAT_CATEGORY_DOOR_BUTTONS, 0, door->ButtonState() * 5, iViewportCell);
					// Image for released button is 0
					// Image for pushed button is 5 because there are 5 rectnos for buttons depending on distance.
				}
				else {
					//^32CB:4887
					// In case there is a custom button (from wall ornates gfx)
					if (tblCellTilesRoom[iViewportCell].xsrd.w6[2] != 0xff) {
						//^32CB:489C
						DRAW_DEFAULT_DOOR_BUTTON(GDAT_CATEGORY_WALL_GFX, U8(tblCellTilesRoom[iViewportCell].xsrd.w6[2]) & 0xff, U8(tblCellTilesRoom[iViewportCell].xsrd.w6[2] >> 8) +1, iViewportCell);
					}
				}
			}
		}
	}
	//^32CB:48D1
	return;
}

//^32CB:48D5
X16 SkWinCore::_32cb_48d5(U16 xx, i16 yy) 
{
	//^32CB:48D5
	ENTER(0);
	//^32CB:48D9
	i16 si = (((xx << 7) +64) / yy) >> 1;
	if (CALC_STRETCHED_SIZE(si, yy) < xx)
		//^32CB:48FB
		return si +1;
	//^32CB:4900
	return si;
}

//^32CB:4905
// SPX: _32cb_4905 renamed DRAW_DOOR
void SkWinCore::DRAW_DOOR(i16 iCellPos, X16 yy, X16 zz, X32 aa)	// i16 xx, X16 yy, X16 zz, X32 aa
{
	//^32CB:4905
	ENTER(348);
	//^32CB:490B
	if (yy != 0) {
		//^32CB:4911
		DRAW_DOOR_FRAMES(iCellPos, yy);
	}
	//^32CB:491D
	if (aa != 0) {
		//^32CB:4925
		DRAW_STATIC_OBJECT(iCellPos, aa, 0);
	}
	
	//^32CB:4937
	U16 iDoorState = tblCellTilesRoom[iCellPos].xsrd.w6[0];	// U16 bp06
	X16 iStretchDual;	// X16 si
	if (iDoorState != 0) {	// 0 = open. 1 - 3 = intermediate state. 4 = closed. 5 = destroyed
		//^32CB:4955
		ObjectID bp0c = tblCellTilesRoom[iCellPos].xsrd.w6[1];
		i16 iYDist = glbTabYAxisDistance[RCJ(23,iCellPos)];	// i16 bp08
		if (iYDist <= 3) {
			//^32CB:497D
			i16 iInvertedYDist = tlbInvertedDoorDistance[RCJ(5,iYDist)];	// i16 bp18
			if (iInvertedYDist >= 0) {
				//^32CB:498F
				i16 iDoorPosRectno = tlbRectnoDoorPosition[RCJ(16,iCellPos)];	// i16 bp1a
				if (iDoorPosRectno >= 0) {
					//^32CB:49A2
					Door *xDoor = GET_ADDRESS_OF_RECORD0(bp0c);	// Door *bp04
					U8 iDoorGDATIndex = glbMapDoorType[xDoor->DoorType()];	// U8 bp0e
					//printf("DRAW DOOR : type to draw : %02d => %02d\n", xDoor->DoorType(), glbMapDoorType[xDoor->DoorType()] );
					X16 iDoorColorPassThrough = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtWordValue, GDAT_IMG_COLORKEY_1);	// X16 bp12
					if (iDoorColorPassThrough != 0)
					{
						// SPX BEGIN
						U8 mirroredDoor = 0;	// by default there is no mirror/flip, so value = 0
						// SPX TEST: Here we can make pseudo animated door like DM1 force field, by playing with the mirror bit
						if (SkCodeParam::bUseDM2ExtendedMode)
						{
							mirroredDoor = QUERY_GDAT_DOOR_IS_MIRRORED(iDoorGDATIndex);
							if (mirroredDoor)
							{
								// RAND02 => 0 to 3
								// Flips: 0 = none, 1 = x, 2 = y, 3 = x & y
								mirroredDoor = RAND02();
							}
						}
						// SPX END

						//^32CB:49DE
						U8 iDoorImg = U8(iYDist) -1;	// U8 bp0f / Check image no from distance; note: distance 0 would give image 0xFF.
						// Door at Y=1 uses image 0 ; Y=2 => image 1 ; Y=3 => image 2 ; Y=0 has normally no specific image, it is image 0 streched.
						iStretchDual = 0x40;	// si = 0x40 = 64 <=> 100%
						X16 iLightPalette = 0;	// X16 bp0a	// 0 = light -> 4 = dark
						if (iYDist == 0 || QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtImage, iDoorImg) == 0) {	// 0xe
							//^32CB:4A09
							if (iYDist != 0) {
								//^32CB:4A0F
								iLightPalette = iYDist;
								iStretchDual = tlbDistanceStretch[RCJ(5,iLightPalette)];	// SPX: iLightPalette is used whereas mostly iYDist is used instead. Aren't they just the same?
							}
							else {
								//^32CB:4A21
								iStretchDual = 0x71;	// si = 0x71 = 113 <=> 176%
							}
							//^32CB:4A24
							iDoorImg = 0;
						}
						//^32CB:4A28
						X16 iOrnateIndex = xDoor->OrnateIndex();	// X16 di
						i16 iCacheNo;	// i16 bp16
#if (DM2_EXTENDED_MODE == 1)
						if (iOrnateIndex != 0 || iDoorState == _DOOR_STATE__DESTROYED_ || glbGlobalSpellEffects.SeeThruWalls > 0) {	// + window spell effect
#else
						if (iOrnateIndex != 0 || iDoorState == _DOOR_STATE__DESTROYED_) {	// If there is any ornate or door is destroyed
#endif
							//^32CB:4A43
							ExtendedPicture xPicture;	// ExtendedPicture bp015c;
							// Get door graphics
							//bp16 = QUERY_MULTILAYERS_PIC(&bp015c, GDAT_CATEGORY_DOORS, bp0e, bp0f, si, si, bp0a, 0, bp12, -1);	// 0xe
							iCacheNo = QUERY_MULTILAYERS_PIC(&xPicture, GDAT_CATEGORY_DOORS, iDoorGDATIndex, iDoorImg, iStretchDual, iStretchDual, iLightPalette, mirroredDoor, iDoorColorPassThrough, -1);	// 0xe
							DRAW_PICST(&xPicture);
							FREE_PICT_MEMENT(&xPicture);
							X16 iStretchHorizontal;	// X16 bp20
							X16 iStretchVertical;	// X16 bp22
							if (iStretchDual == 0x40) {
								//^32CB:4A88
								i16 bp1c;
								i16 bp1e;
								
								QUERY_GDAT_IMAGE_METRICS(GDAT_CATEGORY_DOORS, iDoorGDATIndex, 0, &bp1c, &bp1e);	// 0xe
								iStretchHorizontal = _32cb_48d5(glbTempPicture.width, bp1c);
								iStretchVertical = _32cb_48d5(glbTempPicture.height, bp1e);
								
							}
							else {
								//^32CB:4AC4
								iStretchVertical = iStretchHorizontal = iStretchDual;
							}
							//^32CB:4ACC
							// If there is an ornate on the door ...
							if (iOrnateIndex != 0) {
								//^32CB:4AD0
								iOrnateIndex--;
								U8 iOrnateGDATIndex = glbMapDoorOrnatesList[iOrnateIndex];	// (bp0d) Get the real ornate gfx from the current map door ornate list.
								// SPX: Get ornate
								X16 iColorTransparencyOverlay = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOOR_GFX, iOrnateGDATIndex, dtWordValue, GDAT_IMG_COLORKEY_1);	// X16 bp14
								if (iColorTransparencyOverlay == 0) {
									//^32CB:4AEE
									iColorTransparencyOverlay = 9;	// SPX: This is the "colorkey" index (generally not set in any GDAT2).
								}
								//^32CB:4AF3
								// SPX: draw the ornate over the door // added for 
								//QUERY_TEMP_PICST(0, bp20, bp22, 0, 0, bp08, 
								
								if (!SkCodeParam::bUseDM2ExtendedMode)
									QUERY_TEMP_PICST(mirroredDoor, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, 
										(QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOOR_GFX, iOrnateGDATIndex, dtWordValue, GDAT_DOOR_ORNATE__POSITION) << 2) + iInvertedYDist +0x7d0,
										-1, iColorTransparencyOverlay, -1, GDAT_CATEGORY_DOOR_GFX, iOrnateGDATIndex, GDAT_DOOR_DECORATION_MASK
										);
								else if (SkCodeParam::bUseDM2ExtendedMode)
								{	// SPX: use the colorkey 10 (by default) or what's in GDAT instead of -1 for activating 'pass through' bitmaps
									X16 iColorPassThrough = 10;	// 10 is standard DARK GREEN
									iColorPassThrough = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOOR_GFX, iDoorGDATIndex, dtWordValue, GDAT_IMG_COLORKEY_1);
									if (iColorPassThrough == 0)
										iColorPassThrough = 10;
									QUERY_TEMP_PICST(mirroredDoor, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, 
										(QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOOR_GFX, iOrnateGDATIndex, dtWordValue, GDAT_DOOR_ORNATE__POSITION) << 2) + iInvertedYDist +0x7d0,
										-1, iColorTransparencyOverlay, iColorPassThrough, GDAT_CATEGORY_DOOR_GFX, iOrnateGDATIndex, GDAT_DOOR_DECORATION_MASK
										);
								}

								glbTempPicture.pb44 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(iCacheNo));
								DRAW_TEMP_PICST();
							}
							//^32CB:4B45
							if (iDoorState == _DOOR_STATE__DESTROYED_) {	// (iDoorState == 5) If door is destroyed .. then draw destroyed mask over any ornate
								//^32CB:4B4B
								X16 iColorTransparencyOverlay = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtWordValue, GDAT_IMG_DOOR_COLORKEY_2);	// X16 bp14
								if (iColorTransparencyOverlay == 0)
									iColorTransparencyOverlay = 9;	// 9 is standard CYAN

								// SPX: (2016-10-30) Get the destroyed door mask and use default one if available
								if (!SkCodeParam::bUseFixedMode)
								QUERY_TEMP_PICST(0, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, 
									(QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtWordValue, GDAT_DOOR_DESTROYED_MASK_POSITION) << 2) + iInvertedYDist +0x7d0,
									-1, iColorTransparencyOverlay, iDoorColorPassThrough, GDAT_CATEGORY_DOORS, iDoorGDATIndex, GDAT_DOOR_DESTROYED_MASK
									);
								else if (SkCodeParam::bUseFixedMode)
								{
									U16 iDoorDestroyedGDATIndex = iDoorGDATIndex;
									// Check if there is a destroyed mask for the current door
									U16 iDoorDestroyedMask = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtImage, GDAT_DOOR_DESTROYED_MASK);
									if (iDoorDestroyedMask == (U16)-1) // not found, get the default one
										iDoorDestroyedGDATIndex = GDAT_ITEM_DEFAULT_INDEX;
									QUERY_TEMP_PICST(0, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, 
										(QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtWordValue, GDAT_DOOR_DESTROYED_MASK_POSITION) << 2) + iInvertedYDist +0x7d0,
										-1, iColorTransparencyOverlay, iDoorColorPassThrough, GDAT_CATEGORY_DOORS, iDoorDestroyedGDATIndex, GDAT_DOOR_DESTROYED_MASK
										);								
								}
								// End SPX: (2016-10-30) 
								
								glbTempPicture.pb44 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(iCacheNo));
								DRAW_TEMP_PICST();
							}
#if (DM2_EXTENDED_MODE == 1)
							if (glbGlobalSpellEffects.SeeThruWalls > 0 && iYDist == 1 && yy == 7) {	// If Window spell is active, and display only if distance = 1 and just in front
								
								X16 iColorTransparencyOverlay = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtWordValue, GDAT_IMG_DOOR_COLORKEY_2);	// X16 bp14
								if (iColorTransparencyOverlay == 0)
									iColorTransparencyOverlay = 9;	// 9 is standard CYAN
								
								{
									U16 iDoorSeeThruGDATIndex = iDoorGDATIndex;
									
									U16 iDoorSeeThruMask = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtImage, GDAT_DOOR_SEE_THRU_MASK);
									if (iDoorSeeThruMask == (U16)-1) // not found, get the default one
										iDoorSeeThruGDATIndex = GDAT_ITEM_DEFAULT_INDEX;
									QUERY_TEMP_PICST(0, iStretchHorizontal, iStretchVertical, 0, 0, iYDist, 
										(QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorGDATIndex, dtWordValue, GDAT_DOOR_DESTROYED_MASK_POSITION) << 2) + iInvertedYDist +0x7d0,
										-1, iColorTransparencyOverlay, iDoorColorPassThrough, GDAT_CATEGORY_DOORS, iDoorSeeThruGDATIndex, GDAT_DOOR_SEE_THRU_MASK
										);								
								}
								
								glbTempPicture.pb44 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(iCacheNo));
								DRAW_TEMP_PICST();
							}
#endif
							//^32CB:4BBC
							xPicture.colorKeyPassThrough = xPicture.b58[iDoorColorPassThrough];
							xPicture.w56 = 0;
							_0b36_00c3(iCacheNo, &xPicture);
							// SPX: that draws the main door (without ornate)
							COPY_MEMORY(&xPicture, &glbTempPicture, sizeof(ExtendedPicture));
							glbTempPicture.pb44 = _4976_4c16;
						}
						else {
							//^32CB:4C0C
							iCacheNo = -1;
							//QUERY_TEMP_PICST(0, si, si, 0, 0, bp0a, -1, -1, bp12, -1, 0xe, bp0e, bp0f);
							QUERY_TEMP_PICST(mirroredDoor, iStretchDual, iStretchDual, 0, 0, iLightPalette, -1, -1, iDoorColorPassThrough, -1, GDAT_CATEGORY_DOORS, iDoorGDATIndex, iDoorImg);
						}
						//^32CB:4C36
						// SPX: this part change position of door when in intermediate state
						if (iDoorState < 4) {	// 4 = closed. < 4 => intermediate state. 0 = opened
							//^32CB:4C3C
							iDoorPosRectno = iDoorPosRectno + iDoorState;
							if (xDoor->OpeningDir() == 0) // 0 = horizontal
							{
								X16 iWidth = 0;	// SPX: added this to not reuse "di" variable, already used for ornate index
								//^32CB:4C56
								glbTempPicture.width >>= 1;
								iWidth = glbTempPicture.width;
								glbTempPicture.w4 |= 0x10;
								glbTempPicture.w14 = glbTempPicture.w14 + iWidth;
								glbTempPicture.rectNo = iDoorPosRectno + 6;
								DRAW_TEMP_PICST();	// will draw RIGHT part of the door
								glbTempPicture.w14 = glbTempPicture.w14 + iWidth;
								iDoorPosRectno += 3;
							}
						}
						// Explanation on door rectno offsets:
						// 0 = closed
						// 1 - 3 = vertical positions (º, Ω, æ closed)
						// 4 - 6 = horizontal left positions (º, Ω, æ closed)	=> hence +3 on rectno for horizontal opening
						// 7 - 9 = horizontal right positions (º, Ω, æ closed)	=> hence +6 on rectno for horizontal opening
						//^32CB:4C86
						glbTempPicture.rectNo = iDoorPosRectno;
						glbTempPicture.w30++; // SPX: added this to shift down 1 pixel to get pixel precise regarding DM1 door position
						DRAW_TEMP_PICST();	// draw the door or LEFT part for horizontal opening
						if (iCacheNo >= 0) {
							//^32CB:4C96
							FREE_TEMP_CACHE_INDEX(iCacheNo);
						}
					}
				}
			}
		}
	}
	//^32CB:4C9F
	if (zz != 0) {
		//^32CB:4CA5
		DRAW_DOOR_FRAMES(iCellPos, zz);
	}
	//^32CB:4CB1
	if (aa != 0) {
		//^32CB:4CB9
		_32cb_2d8c(tblCellTilesRoom[iCellPos].xsrd.w4, iCellPos, aa);
	}
	//^32CB:4CDB
	return;
}

//^32CB:4CDF
// SPX: _32cb_4cdf renamed DRAW_DOOR_TILE
void SkWinCore::DRAW_DOOR_TILE(i16 iCellPos)	// i16 xx
{
	//^32CB:4CDF
	ENTER(6);
	//^32CB:4CE5
	//i16 si = xx; <= iCellPos
	if (_4976_455e[RCJ(16,iCellPos)] == 0)
		return;
	X32 bp04 = 0x1000;
	if (tblCellTilesRoom[iCellPos].xsrd.w6[0] != 5) {
		//^32CB:4D13
		bp04 = 0x1000;
	}
	// SPX: draw objects on door tile behind the door
	DRAW_STATIC_OBJECT(iCellPos, 0x3ff, (iCellPos != 0) ? 1 : 0);
	X16 bp06 = 0;
	X16 di = 0;
	switch (iCellPos) {
		case 0://^4D4E	// SPX: player is on the tile of the door
			//^32CB:4D4E
			if (bp04 != 0) {
				//^32CB:4D56
				DRAW_STATIC_OBJECT(iCellPos, bp04, 0);
			}
			DRAW_DOOR(iCellPos, 6, 0, 0);
			_32cb_2d8c(tblCellTilesRoom[iCellPos].xsrd.w4, iCellPos, 0x01ffffff);
			return;
		case 7://^4D96
			//^32CB:4D96
			di = 1;
            bp06 = 4;
			goto _4dc0;
		case 8://^4DA0
			//^32CB:4DA0
			di = 1;
			bp06 = 2;
			goto _4dc0;
		case 12://^4DA2
			//^32CB:4DA2
			di = 2;
			bp06 = 4;
			goto _4dc0;
		case 13://^4DA7
			//^32CB:4DA7
			di = 4;
			bp06 = 2;
			goto _4dc0;
		case 11://^4DAC
			//^32CB:4DAC
			di = 6;
			goto _4dc0;
		case 4://^4DB1
			//^32CB:4DB1
			di = 1;
            bp06 = 4;
			goto _4dc0;
		case 5://^4DB3
			di = 1;
			bp06 = 2;
			goto _4dc0;
		case 3://^4DBD
		case 6://^4DBD
			di = 7;
			goto _4dc0;
		case 14://^4DC0
		case 15://^4DC0
			//^32CB:4DC0
_4dc0:
			DRAW_DOOR(iCellPos, di, bp06, bp04);
			break;
		case 1://^4DD2
		case 2://^4DD2
		case 9://^4DD2
		case 10://^4DD2
			break;
	}
	//^32CB:4DD2
	// SPX: draw objects on door tile before the door
	DRAW_STATIC_OBJECT(iCellPos, 0x01fffc00 & (~bp04), 1);
	_32cb_3edd(iCellPos);
	//^32CB:4DF8
	return;
}

//^32CB:4ECC
// SPX: _32cb_4ecc renamed DRAW_STAIRS_SIDE
void SkWinCore::DRAW_STAIRS_SIDE(i16 xx)
{
	//^32CB:4ECC
	ENTER(4);
	//^32CB:4ED2
	i16 di = xx;
	U8 gfxset = glbMapGraphicsSet;	// bp02
	i16 bp04 = glbSceneColorKey;
	if (di > 8)
		return;
	//^32CB:4EE6
	X16 si = (tblCellTilesRoom[di].xsrd.w6[0] != 0) ? 1 : 0;
	X8 bp01 = tlbGraphicsStairsSide[RCJ(9,di)][RCJ(2,si)];	// bp01	, table containing 0xC7 to 0xD2 = side stairs (wood ramp)
	if (bp01 != 0xff) {
		//^32CB:4F12
		DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, gfxset, bp01, _4976_4600[RCJ(9,di)][RCJ(2,si)], bp04, 0);
	}
	//^32CB:4F37
	return;
}


//^32CB:245A
// SPX: _32cb_245a renamed DRAW_PIT_TILE
void SkWinCore::DRAW_PIT_TILE(i16 xx)
{
	//^32CB:245A
	ENTER(6);
	//^32CB:2460
	i16 si = xx;
	X8 bp03 = glbMapGraphicsSet;
	X16 bp06 = glbSceneColorKey;
	i16 bp02 = _4976_4282[RCJ(16,si)];
	if (bp02 < 0)
		return;
	if (si < 11 || tblCellTilesRoom[si].xsrd.w6[0] == 0) {
		//^32CB:2497
		X16 di = tlbGraphicsFlip[RCJ(16,si)];	// di
		if (si == 0) {
			//^32CB:24A3
			di = (SET_GRAPHICS_FLIP_FROM_POSITION(1, _4976_5aa0, _4976_5a9c, _4976_5a9e) != 0) ? 1 : 0;
		}
		//^32CB:24C4
		DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, bp03
			, (tblCellTilesRoom[si].xsrd.w6[0] != 0) ? tlbGraphicsPitHidden[RCJ(16,si)] : tlbGraphicsPitNormal[RCJ(16,si)], bp02, bp06, di);
	}
	//^32CB:24F7
	return;
}
//^32CB:1F3E
// SPX: _32cb_1f3e renamed DRAW_EXTERNAL_TILE
X16 SkWinCore::DRAW_EXTERNAL_TILE(i16 xx)
{
	//^32CB:1F3E
	ENTER(356);
	//^32CB:1F44
	i16 si = xx;
	X8 bp14 = _4976_4230[RCJ(23,si)];
	if (bp14 == 0xff)
		return 0;
	//^32CB:1F55
	i16 bp06 = tblCellTilesRoom[si].posx;
	i16 bp08 = tblCellTilesRoom[si].posy;
	i16 bp18 = glbTabYAxisDistance[RCJ(23,si)];
	//^32CB:1F86
	TELE_inf bp2a; // bp2a-bp26
	if (bp18 > 3 && GET_TELEPORTER_DETAIL(&bp2a, U8(bp06), U8(bp08)) == 0)
		return 0;
	//^32CB:1FA6
	U16 bp02 = tblCellTilesRoom[si].xsrd.w6[2];
	X16 bp16 = bp02 >> 8;
	bp14 += X8(bp16);
	X8 iFloorOrnateID = X8(bp02);	// bp13
	if (iFloorOrnateID == 0xFF || iFloorOrnateID == 0)
		return 0;
	//^32CB:1FDF
	X16 bp04 = 0;
	if (glbTabXAxisDistance[RCJ(23,si)] == 0) {
		//^32CB:1FEB
		if (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, dtWordValue, GDAT_FLOOR_ORNATE__DO_NOT_FLIP) == 0) {
			//^32CB:2001
			bp04 = glbGeneralFlipGraphics;
			if ((bp18 & 1) == 0) {
				//^32CB:200E
				bp04 ^= 1;
			}
		}
	}
	//^32CB:2015
	else if (glbTabXAxisDistance[RCJ(23,si)] >= 1) {
		//^32CB:201C
		bp04 = 1;
	}
	//^32CB:2021
	X16 bp0e = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, dtWordValue, GDAT_IMG_COLORKEY_1); // 4
	if (bp0e == 0) {
		//^32CB:203A
		bp0e = glbSceneColorKey;
	}
	//^32CB:2040
	X16 bp0a = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, dtWordValue, GDAT_FLOOR_ORNATE__POSITION); // 5
	X16 bp12;
	if (bp0a == 0) {
		//^32CB:2059
		bp0a = ORNATE_POS__VCENTERED_HCENTERED; // bp0a = 12;
		bp12 = 0;
	}
	else {
		//^32CB:2065
		bp12 = bp0a >> 8;
		bp0a = (bp0a & 0xff) -1;
	}
	//^32CB:2078
	X16 bp0c = QUERY_CREATURE_BLIT_RECTI(si, bp0a, 0);
	X16 bp10 = 0x40; // bp10
	if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, dtImage, bp14) == 0) {
		//^32CB:20A6
		bp14 = _4976_4247[RCJ(23,si)] +X8(bp16);
		bp10 = tlbDistanceStretch[RCJ(5,bp18)];
		// SPX: if drawing player tile and there is a floor image at front D0, we try to get it
		if (SkCodeParam::bUseDM2ExtendedMode && bp18 == 0)
		{
			if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, dtImage, bp14 + 0x80) == 1)
			{
				bp14 = bp14 + 0x80;
				bp10 = 0x40; // 100% because we want to display front D0 at its real size.
			}
		}
	}
	//^32CB:20BC
	X16 bp1a;
	X16 bp22;
	X16 bp24;
	i16 di;
	X16 bp20;
	ExtendedPicture bp0164;
	X16 bp1c;
	if (glbTabXAxisDistance[RCJ(23,si)] == 0 && bp18 != 0) {
		//^32CB:20CF
		X16 bp1e = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, dtWordValue, GDAT_FLOOR_ORNATE__X63);
		if ((bp1c = GET_TELEPORTER_DETAIL(&bp2a, U8(bp06), U8(bp08))) != 0 || bp1e != 0) {
			//^32CB:2107
			QUERY_TEMP_PICST(bp04, bp10, bp10, 0, 0, bp18, bp0c, bp12, -3, -3, GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, bp14);
			QUERY_TEMP_PICST(bp04, bp10, bp10, glbTempPicture.w28, glbTempPicture.w30, bp18, bp0c, bp12, bp0e, -1, GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, bp14 +0xc8);
			DRAW_TEMP_PICST();
			//^32CB:2168
			bp06 = bp2a.b2;
			bp08 = bp2a.b3;
			bp1a = _4976_5aa0;
			_4976_5aa0 = bp2a.b1;
			bp22 = glbCurrentMapIndex;
			CHANGE_CURRENT_MAP_TO(bp2a.b4);
			_4976_4212 = 1;
			bp24 = glbTabYAxisDistance[RCJ(23,si)];
			CALC_VECTOR_W_DIR(_4976_5aa0, -bp24, 0, &bp06, &bp08);
			//^32CB:21C4
			for (di = 4; di >= bp24; di--) {
				//^32CB:21C9
				_32cb_4185_TILE_ROOM(bp06, bp08, _4976_422b[RCJ(5,di)], _4976_5aa0);
				//^32CB:21E1
			}
			//^32CB:21E7
			for (di = 4; di > bp24; di--) {
				//^32CB:21EC
				DRAW_STATIC_OBJECT(_4976_422b[RCJ(5,di)], 0x01ffffff, 1);
				//^32CB:2201
			}
			//^32CB:2207
			CHANGE_CURRENT_MAP_TO(bp22);
			_4976_4212 = 0;
			_4976_5aa0 = bp1a;
			DRAW_STATIC_OBJECT(si, 0x000003ff, 1);
			bp20 = QUERY_MULTILAYERS_PIC(
				&bp0164, GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, bp14, bp10, bp10, bp18, bp04, bp0e,
				QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, dtWordValue, GDAT_IMG_FLOOR_COLORKEY_2)	// 0xa
				);
			if (bp1e != 0) {
				//^32CB:226E
				_32cb_0c7d(&bp0164, bp20, bp0e);
			}
			else {
				//^32CB:2283
				FILL_ENTIRE_PICT(reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp20)), bp0164.b58[bp0e]);
			}
			//^32CB:22A5
			bp0164.pb44 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp20));
			DRAW_PICST(&bp0164);
			FREE_PICT_MEMENT(&bp0164);
			bp0164.colorKeyPassThrough = bp0164.b58[bp0e];
			bp0164.pb44 = _4976_4c16;
			bp0164.rectNo = bp0c;
			bp0164.w26 = bp12;
			bp0164.w56 = 0;
			_0b36_00c3(bp20, &bp0164);
			DRAW_PICST(&bp0164);
			FREE_TEMP_CACHE_INDEX(bp20);
			return bp1c;
		}
	}
	//^32CB:2331
	QUERY_TEMP_PICST(bp04, bp10, bp10, 0, 0, bp18, bp0c, bp12, bp0e, -1, GDAT_CATEGORY_FLOOR_GFX, iFloorOrnateID, bp14);
	DRAW_TEMP_PICST();
	//^32CB:2361
	return 0;
}
//^32CB:2367
// SPX: _32cb_2367 renamed DRAW_PIT_ROOF
void SkWinCore::DRAW_PIT_ROOF(i16 xx)
{
	//^32CB:2367
	ENTER(10);
	//^32CB:236D
	i16 si = xx;
	X8 gfxset = glbMapGraphicsSet;	//bp07
	X16 bp0a = glbSceneColorKey;
	if (si > 8 || (glbSceneFlags & 1) == 0)
		return;
	//^32CB:238F
	i16 bp04 = tblCellTilesRoom[si].posx;
	i16 bp06 = tblCellTilesRoom[si].posy;
	i16 bp02 = LOCATE_OTHER_LEVEL(glbCurrentMapIndex, -1, &bp04, &bp06, NULL);
	if (bp02 < 0)
		return;
	//^32CB:23DB
	bp02 = glbMapTileValue[bp02][bp04][bp06];
	if ((bp02 >> 5) != 2 || (bp02 & 8) == 0)
		return;
	//^32CB:240B
	X16 di = _4976_4279[RCJ(9,si)];
	if (si == 0) {
		//^32CB:2417
		di = (SET_GRAPHICS_FLIP_FROM_POSITION(0x20, _4976_5aa0, _4976_5a9c, _4976_5a9e) != 0) ? 1 : 0;
	}
	//^32CB:2438
	DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, gfxset, tlbGraphicsPitRoof[RCJ(9,si)], _4976_425e[RCJ(9,si)], bp0a, di);
	//^32CB:2456
	return;
}

//^32CB:3143
// SPX: _32cb_3143 renamed DRAW_FLYING_ITEM
void SkWinCore::DRAW_FLYING_ITEM(ObjectID rl, i16 cellPos, X16 _5x5)
{
	//^32CB:3143
	ENTER(32);
	//^32CB:3149
	if (rl == OBJECT_END_MARKER)
		return;
	//^32CB:3152
	i16 di = DIR_FROM_5x5_POS(_5x5);
	if (di == -1)
		return;
	//^32CB:3165
	X16 bp1a = glbTabYAxisDistance[RCJ(23,cellPos)];
	if (bp1a == 0 && di >= 2)
		return;
	//^32CB:317E
	i16 bp1c = glbTabXAxisDistance[RCJ(23,cellPos)];
	if (_4976_415b[RCJ(23,cellPos)] < 0)
		return;
	//^32CB:3193
	X16 bp1e = tblCellTilesRoom[cellPos].posx;
	X16 bp20 = tblCellTilesRoom[cellPos].posy;
	X16 si;
	do {
		//^32CB:31BE
		if (QUERY_OBJECT_5x5_POS(rl, _4976_5aa0) != _5x5)
			continue;
		//^32CB:31D4
		if (rl.DBType() != dbMissile)
			continue;
        //^32CB:31E5
		i16 bp08 = QUERY_CREATURE_BLIT_RECTI(cellPos, _5x5, 0);
        X16 bp0c = _48ae_011a(rl);
		if (bp0c == 0xffff) {
			//^32CB:320A
			ObjectID bp18 = ObjectID(GET_ADDRESS_OF_RECORDE(rl)->GetMissileObject(), rl.Dir());
			DRAW_ITEM(bp18, cellPos, 0, 0xffa4, -1, NULL, 1, 0, 0);
			continue;
		}
		//^32CB:324D
		Missile *bp04 = GET_ADDRESS_OF_RECORDE(rl);
		U8 bp12 = QUERY_CLS1_FROM_RECORD(rl);
		U8 bp11 = QUERY_CLS2_FROM_RECORD(rl);
		X16 bp0e = (bp12 == 0xd) ? 1 : 0;
		X16 bp10;
		i16 bp16;
		//^32CB:3284
		if ((bp0e == 0 || bp04->EnergyRemaining() == 0xff) && bp1a == 0) {
			//^32CB:3298
			bp10 = bp16 = 0x40;
		}
		else {
			//^32CB:32A3
			bp16 = (bp1a << 1) -(di >> 1);
			if (bp16 < 0)
				continue;
			//^32CB:32B8
			bp10 = bp16 = _4976_41a9[RCJ(7,bp16)];
			if (bp0e != 0) {
				//^32CB:32CD
				bp10 = max_value(8, CALC_STRETCHED_SIZE((((bp04->EnergyRemaining() << 7) / 255) +1) >> 1, bp10));
			}
		}
		//^32CB:32FB
		si = 0;
		X8 bp13;
		if (bp0c == 3) {
			//^32CB:3303
			bp13 = 8;
		}
		else {
			//^32CB:330A
			X16 bp0a = glbTimersTable[bp04->TimerIndex()].Direction();
			if ((bp0a & 1) != (_4976_5aa0 & 1)) {
				//^32CB:3338
				bp13 = 0xc;
				if (bp0c == 0) {
					if (di == 0 || di == 3) {
						//^32CB:334B
						si |= 1;
					}
					//^32CB:334F
					if (((bp1e +bp20) & 1) == 0) {
						si ^= 1;
					}
				}
				//^32CB:3364
				else if (((_4976_5aa0 +1) & 3) == bp0a) {
					si |= 1;
				}
			}
			//^32CB:3376
			else {
				if (bp0c == 0) {
					if (((bp1e +bp20) & 1) != 0) {
						//^32CB:3387
						si |= 2;
						if (di < 2) {
							//^32CB:3390
							bp13 = 8;
						}
						else {
							//^32CB:3396
							bp13 = 9;
						}
					}
					//^32CB:339C
					else if (di >= 2) {
						//^32CB:33A1
						//^32CB:3390
						bp13 = 8;
					}
					else {
						//^32CB:33A3
						bp13 = 9;
					}
				}
				//^32CB:33A9
				else if (bp0c == 2 || (bp0c == 1 && bp0a != _4976_5aa0)) {
					//^32CB:33BE
					//^32CB:3390
					bp13 = 8;
				}
				else {
					//^32CB:33C0
					bp13 = 10;
				}
				//^32CB:33C4
				if (bp1c < 1 && (bp1c != 0 || (di != 1 && di != 2))) {
					//^32CB:33DA
					si |= 1;
				}
				//^32CB:33DE
				if ((di & 1) != 0 && bp12 == 0xd) {
					//^32CB:33EA
					si |= 2;
				}
			}
		}
		//^32CB:33EE
		X16 bp06;
		if (bp12 == 0xd) {
			//^32CB:33F4
			bp06 = QUERY_GDAT_ENTRY_DATA_INDEX(0xd, bp11, dtWordValue, 0x01);
		}
		else {
			bp06 = 3;
		}
		//^32CB:3410
		QUERY_TEMP_PICST(si & bp06, bp10, bp10, 0, 0, bp1a, bp08 | 0x8000, 0, 10, -1, bp12, bp11, bp13);
		glbTempPicture.w32 += CALC_STRETCHED_SIZE(0, bp16);
		glbTempPicture.w34 = CALC_STRETCHED_SIZE(-92, bp16);
		DRAW_TEMP_PICST();
		//^32CB:3467
	} while ((rl = GET_NEXT_RECORD_LINK(rl)) != OBJECT_END_MARKER);
	//^32CB:347B
	return;
}

//^32CB:3B9D
// SPX: _32cb_3b9d renamed DRAW_STATIC_OBJECT
void SkWinCore::DRAW_STATIC_OBJECT(i16 xx, X32 ss, X16 ww)
{
	//^32CB:3B9D
	ENTER(28);
	
	//^32CB:3BA3
	i16 si = xx;
	if (si > 15 || _4976_43e5[RCJ(16,si)] == 0)
		return;
	//^32CB:3BB8
	X16 bp1c = tblCellTilesRoom[si].xsrd.w0;
	ObjectID bp16 = tblCellTilesRoom[si].xsrd.w4;
	X16 bp0c = _4976_43f5[RCJ(16,si)][0];
	X16 bp0e = _4976_43f5[RCJ(16,si)][1];
	const U8 *displayOrder;	// *bp04 
	if (glbTabXAxisDistance[RCJ(23,si)] < 0) { // at left
		//^32CB:3BFF
		displayOrder = tlbDisplayOrderLeft;
	}
	//^32CB:3C06
	else if (glbTabXAxisDistance[RCJ(23,si)] == 0) { // at center
		//^32CB:3C0D
		displayOrder = tlbDisplayOrderCenter;
	}
	else { // at right
		//^32CB:3C14
		displayOrder = tlbDisplayOrderRight;
	}
	//^32CB:3C1C
	for (X16 di = 0; ((si == 0) ? 15 : 25) > di; di++) {
		//^32CB:3C21
		X16 bp08 = displayOrder[di];
		X32 bp1a = (U32(1) << bp08) & ss;
		if (bp1a == 0)
			continue;
		//^32CB:3C4C
		// SPX: some problem here : if (*_4976_5be2)[si] is zero on a door (open) then no item will be drawn!
		if (((*_4976_5be2)[si] & bp1a) != 0) {
			//^32CB:3C68
			DRAW_PUT_DOWN_ITEM(bp16, si, DIR_FROM_5x5_POS(bp08), NULL);
		}
		//^32CB:3C81
		X16 bp10 = _4976_4415[RCJ(25,bp08)][0] +bp0c;
		X16 bp12 = bp0e -_4976_4415[RCJ(25,bp08)][1];
		if (bp1c == 0 || _4976_5a84[bp10][bp12] == si) {
			//^32CB:3CC8
			U8 bp13 = _4976_5aa4[bp10][bp12];
			if (bp13 != 0xff) {
				//^32CB:3CE6
				if (((1 << bp08) & ss) != 0) {
					//^32CB:3D04
					SUMMARY_DRAW_CREATURE(tblCellTilesRoom[bp13].xsrd.w4, bp13, ss);
					_4976_5aa4[bp10][bp12] = 0xff;
					if ((*_4976_5a7c)[si] != 0) {
						//^32CB:3D54
						U16 bp06;
						for (bp06 = di -7; bp06 < di; bp06++) {
							//^32CB:3D5E
							X16 bp0a = displayOrder[bp06];
							if (((1 << bp0a) & *(*_4976_5a7c)) != 0) {
								//^32CB:3D99
								DRAW_FLYING_ITEM(bp16, si, bp0a);
							}
							//^32CB:3DA7
						}
					}
				}
			}
		}
		//^32CB:3DAF
		if (((*_4976_5a7c)[si] & bp1a) != 0) {
			//^32CB:3DCB
			DRAW_FLYING_ITEM(bp16, si, bp08);
		}
		//^32CB:3DD9
	}
	//^32CB:3DED
	if (ww != 0) {
		//^32CB:3DF3
		_32cb_2d8c(bp16, si, ss);
	}
	//^32CB:3E04
	return;
}

//^32CB:24FB
// SPX: _32cb_24fb renamed DRAW_TELEPORTER_TILE; though it can display REFLECTOR
void SkWinCore::DRAW_TELEPORTER_TILE(i16 xx, X16 cls1, X16 cls2)
{
	//^32CB:24FB
	ENTER(60);
	//^32CB:2501
	if (xx > 15)
		return;
	//^32CB:250A
	X8 *bp04 = _4976_42f2[RCJ(23,xx)];
	X16 bp0e = _4976_42d2[RCJ(16,xx)];
	X16 bp10 = 0;
	X8 bp13 = bp04[1] & 0x7f;
	X16 bp16 = ((bp13 & 0x80) != 0) ? 1 : 0;
	i16 bp18 = bp04[2];
	i16 bp1a = bp04[3];
	SRECT bp22;
	if (QUERY_BLIT_RECT(NULL, &bp22, bp0e, &bp18, &bp1a, -1) == NULL)
		return;
	//^32CB:2583
	U8 *bp08;
	Picture bp3c;
	U8 *bp0c;
	if (bp04[1] == 0xff) {
		bp08 = NULL;
	}
	else {
		//^32CB:259A
		bp3c.b8 = U8(cls1);
		bp3c.b9 = U8(cls2);
		bp3c.b11 = bp13;
		bp08 = QUERY_PICST_IMAGE(&bp3c);
		ALLOC_PICT_MEMENT(&bp3c);
		if (bp16 != 0) {
			//^32CB:25D3
			X16 bp12 = ALLOC_TEMP_CACHE_INDEX();
			X16 bp24;
			X16 si;
			bp0c = ALLOC_NEW_PICT(bp12, si = READ_UI16(bp08,-4), bp24 = READ_UI16(bp08,-2), 4);
			bp08 = QUERY_PICT_BITS(&bp3c);
			X16 di = READ_UI16(bp08,-4) & 1;
			if (di != 0)
				//^32CB:2622
				di = 2 -di;
			//^32CB:2629
			FIRE_BLIT_PICTURE(
				bp08, bp0c, 
				ALLOC_TEMP_ORIGIN_RECT(si -di, bp24), 0, 0, 
				si, si, -1, 1, 4, 4, NULL);
			FREE_PICT_MEMENT(&bp3c);
			_0b36_00c3(bp12, &bp3c);
			ALLOC_PICT_MEMENT(&bp3c);
		}
	}
	//^32CB:2688
    bp0c = QUERY_GDAT_IMAGE_ENTRY_BUFF(U8(cls1), U8(cls2), 0x14);
	if (bp04[1] != 0xff) {
		//^32CB:26AA
		bp08 = QUERY_PICT_BITS(&bp3c);
	}
	//^32CB:26BC
	_44c8_20a4(
		bp0c, 
		_4976_4c16, 
		bp08, 
		&bp22, 
		(bp04[0] +RAND01()) << 4, 
		RAND() & 255,
		_4976_00f6, 
		bp10, 
		_32cb_0649(U8(cls1), U8(cls2), 0x14, bp10)
		);
	if (bp04[1] != 0xff) {
		//^32CB:2720
		FREE_PICT_MEMENT(&bp3c);
	}
	//^32CB:272C
	return;
}

//^32CB:4F3B
void SkWinCore::DRAW_WALL(i16 iViewportCell)	// i16 xx
{
	//^32CB:4F3B
	ENTER(12);
	//^32CB:4F41
	U8 iMapGfx = glbMapGraphicsSet;	// bp02
	i16 iColorkey1 = glbSceneColorKey;	// bp06
	i8 iXDist = glbTabXAxisDistance[RCJ(23,iViewportCell)];	// bp07
	i16 iYDist = glbTabYAxisDistance[RCJ(23,iViewportCell)];	// bp0c
	//^32CB:4F5F
	U8 bp01 = (iYDist > 1) ? GDAT_WALL_ORNATE__DATA_6F : GDAT_WALL_ORNATE__DATA_6E;	// SPX TODO: unknown GDAT values! never seen them in any GDAT
	X16 si = 0;
	X16 di = 0;
	U16 bp0a = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, bp01);
	// SPX: in case gfxset (custom dungeon) is >= 6, bypass these values.
	if (SkCodeParam::bUseFixedMode && glbMapGraphicsSet >= 0 && glbMapGraphicsSet < 6) {
		if (iYDist > 1) { // dist 2,3,4
			//^32CB:4F91
			if (tblCellTilesRoom[4].xsrd.w0 == 0 && (di = (bp0a >> 8)) == 0) {
				//^32CB:4FA5
				di = _4976_463c_tblGfxSet[RCJ(6,glbMapGraphicsSet)];
			}
			//^32CB:4FB2
			if (tblCellTilesRoom[5].xsrd.w0 == 0) {
				//^32CB:4FBD
				si = bp0a & 0xff;
				if (si == 0) {
					//^32CB:4FC9
					si = _4976_4648_tblGfxSet[RCJ(6,glbMapGraphicsSet)];
				}
			}
		}
		//^32CB:4FD8
		else if (iYDist > 0) { // dist 1
			//^32CB:4FDE
			if (tblCellTilesRoom[0].xsrd.w0 == 0 && (di = (bp0a >> 8)) == 0) {
				//^32CB:4FF5
				di = _4976_4624_tblGfxSet[RCJ(6,glbMapGraphicsSet)];
			}
			//^32CB:5002
			if (tblCellTilesRoom[2].xsrd.w0 == 0 && (si = (bp0a & 0xff)) == 0) {
				//^32CB:5019
				si = _4976_4630_tblGfxSet[RCJ(6,glbMapGraphicsSet)];
			}
		}
	}
	//^32CB:5026
	if (di != 0 || si != 0) {
		//^32CB:502E
		TRIM_BLIT_RECT(di, 0, si, 0);
	}
	//^32CB:503C
	X16 bFlip = (iXDist <= 0) ? 0 : 1;	// (bp04) 0 = left / 1 = right
	//^32CB:504E
	if (iViewportCell >= 0x10) { // wall at D4
		//^32CB:5054
		if (iXDist == -2 || iXDist == 2) {
			//^32CB:5060
			bFlip = 0;	// left
		}
		bFlip ^= glbGeneralFlipGraphics;
		bp01 = 0x32;	// 0x32 : D4 wall front
	}
	//^32CB:5071
	else if (glbGeneralFlipGraphics != 0) {
		//^32CB:5078
		bp01 = tlbDoorSideFramesReorder[RCJ(23,iViewportCell)] -80;
		if (QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_GRAPHICSSET, iMapGfx, dtImage, bp01) == 0) {
			if (iXDist == 0) {
				//^32CB:509F
				bFlip = 1;	// right
			}
			//^32CB:50A4
			bp01 = bp01 +0x72;	// 0x72 is pit side D2 ...
		}
	}
	else {
		//^32CB:50AE
		bp01 = iViewportCell +0x22;	// 0x23 is wall side D0
	}

	// SPX: 2016-11-11 : attempt having See Through Wall masks on wall
#if (DM2_EXTENDED_MODE == 1)	// Check for possible thru wall gfx
	{
	
		if (glbGlobalSpellEffects.SeeThruWalls > 0 && iViewportCell == 3)	// front D1 wall
		{
			i16 iColorkey2 = -1;
			ExtendedPicture xExtPicWall;
			i16 iPicture = 0;
			bp01 = 0x025;	// D1 wall

			iColorkey1 = 10;	// would be dark green / pass through (see what's behind wall)
			iColorkey2 = 9;		// would be cyan / transparent (see wall)
			
			iPicture = QUERY_MULTILAYERS_PIC(&xExtPicWall, GDAT_CATEGORY_GRAPHICSSET, iMapGfx, bp01, 0x40, 0x40, 0, bFlip, iColorkey1, -1);
			//iPicture = QUERY_MULTILAYERS_PIC(&xExtPicWall, GDAT_CATEGORY_GRAPHICSSET, 0xFE, 0xF2, 0x40, 0x40, 0, bFlip, iColorkey1, -1); // test wall
			DRAW_PICST(&xExtPicWall);
			FREE_PICT_MEMENT(&xExtPicWall);

			X16 iScale = 0x40;
			X16 bp20 = 0x40;
			X16 bp22 = 0x40;;
			//--- Put the metrics of that image to the TEMP image
			if (iScale == 0x40)
			{
				i16 bp1c;
				i16 bp1e;
				
				QUERY_GDAT_IMAGE_METRICS(GDAT_CATEGORY_GRAPHICSSET, iMapGfx, bp01, &bp1c, &bp1e);	// 0xe
				bp20 = _32cb_48d5(glbTempPicture.width, bp1c);
				bp22 = _32cb_48d5(glbTempPicture.height, bp1e);
				
			}
			else {
				bp22 = bp20 = iScale;
			}

			//------ part to get and draw the see thru walls
			if (1)
			{
				U16 iWallSetIndex = iMapGfx;
			
				U16 iWallSeeThruMask = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, iMapGfx, dtImage, GDAT_GFXSET_SEE_THRU);
				if (iWallSeeThruMask == (U16)-1) // not found, get the default one
					iWallSetIndex = GDAT_ITEM_DEFAULT_INDEX;	// 0xFE
				
				i16 iRectNo = QUERY_CREATURE_BLIT_RECTI(3, 11, 0);	// That gets currently the best result for positionning 'see through wall' mask

				QUERY_TEMP_PICST(bFlip, bp20, bp22, 0, 0, 
					(glbIsPlayerMoving != 0) ? -iYDist : 0, 
					iRectNo,
					-1, iColorkey2, iColorkey1,
					GDAT_CATEGORY_GRAPHICSSET, iWallSetIndex, GDAT_GFXSET_SEE_THRU
					);

				glbTempPicture.pb44 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(iPicture));
				DRAW_TEMP_PICST();
			}
			//------

			//--- Get correct pass through color
			xExtPicWall.colorKeyPassThrough = xExtPicWall.b58[iColorkey1];
			xExtPicWall.w56 = 0;
			_0b36_00c3(iPicture, &xExtPicWall);
			COPY_MEMORY(&xExtPicWall, &glbTempPicture, sizeof(ExtendedPicture));
			glbTempPicture.pb44 = _4976_4c16;
			glbTempPicture.rectNo = 3 + 0x2BE;
			DRAW_TEMP_PICST();
			if (iPicture >= 0)
				FREE_TEMP_CACHE_INDEX(iPicture);
		}
		else
		{
			QUERY_TEMP_PICST(bFlip, 0x40, 0x40, 0, 0, (glbIsPlayerMoving != 0) ? -iYDist : 0, iViewportCell +0x2be, 0xffff, iColorkey1, -1, GDAT_CATEGORY_GRAPHICSSET, iMapGfx, bp01);
			DRAW_TEMP_PICST(); // draw wall
		}

	}
#else
	//^32CB:50B6
	QUERY_TEMP_PICST(bFlip, 0x40, 0x40, 0, 0, (glbIsPlayerMoving != 0) ? -iYDist : 0, iViewportCell +0x2be, 0xffff, iColorkey1, -1, GDAT_CATEGORY_GRAPHICSSET, iMapGfx, bp01);
	DRAW_TEMP_PICST(); // draw wall
#endif
	_098d_0c45();
	//^32CB:50FA
	return;
}
//^32CB:50FE
// SPX: _32cb_50fe renamed DRAW_WALL_TILE
void SkWinCore::DRAW_WALL_TILE(i16 iViewportCell)	// i16 xx
{
	//^32CB:50FE
	ENTER(0);
	//^32CB:5102
	i16 iLocalViewportCell = iViewportCell;	//i16 si = xx;
	DRAW_WALL(iLocalViewportCell);
	switch (_4976_4654[RCJ(23,iLocalViewportCell)]) {
		case 1: //^_5121	// front vision
			//^32CB:5121
			DRAW_WALL_ORNATE(iLocalViewportCell, 0, 1);
			break;
		case 3: //^_5127	// side
			//^32CB:5127
			DRAW_WALL_ORNATE(iLocalViewportCell, 0, 1);
			goto _5133;
		case 2: //^_5133	// side
			//^32CB:5133
_5133:
            DRAW_WALL_ORNATE(iLocalViewportCell, glbTabXAxisDistance[RCJ(23,iLocalViewportCell)], 1);
			break;
	}
	//^32CB:5143
	return;
}

//^32CB:5146
// SPX: _32cb_5146 renamed DRAW_DUNGEON_TILES
void SkWinCore::DRAW_DUNGEON_TILES()
{
	//^32CB:5146
	ENTER(10);
	//^32CB:514C
	U32 bp04 = 0x01ffffff;
	i16 bp08 = 0;
	for (; bp08 < 20; bp08++) {
		static const char * pszTblDrawOrder = {"D4L2,D4R2,D4L1,D4R1,D4  ,D3L2,D3R2,D3L1,D3R1,D3  ,D2L2,D2R2,D2L1,D2R1,D2  ,D1L1,D1R1,D1  ,D0L1,D0R1"};
		// D4L2| 0|19
		// D4R2| 1|20
		// D4L1| 2|17
		// D4R1| 3|18
		// D4  | 4|16
		// D3L2| 5|14
		// D3R2| 6|15
		// D3L1| 7|12
		// D3R1| 8|13
		// D3  | 9|11
		// D2L2|10| 9
		// D2R2|11|10
		// D2L1|12| 7
		// D2R1|13| 8
		// D2  |14| 6
		// D1L1|15| 4
		// D1R1|16| 5
		// D1  |17| 3
		// D0L1|18| 1
		// D0R1|19| 2
		SkD((DLV_DBG_CELL, "DBG: Summary#%02d (%.4s)\n", (Bitu)bp08, &pszTblDrawOrder[5*bp08]));

		//^32CB:515E
		IBMIO_USER_INPUT_CHECK();
		i16 si;
		X16 bp06 = tblCellTilesRoom[si = _4976_466b[RCJ(20,bp08)]].xsrd.w0;
		ObjectID di = tblCellTilesRoom[si].xsrd.id4();
		if (si > 15) { // at D4
			//^32CB:5196
			X16 bp0a = bp06;
			switch (bp0a) {
				case 0x00://^51B5
					//^32CB:51B5
					if (di != OBJECT_END_MARKER) {
						//^32CB:51BA
						SUMMARY_DRAW_CREATURE(di, si, 0x01ffffff);
					}
					//^32CB:51C8
					DRAW_WALL_TILE(si);
					continue;
				case 0x01://^51D1
				case 0x02://^51D1
				case 0x05://^51D1
					//^32CB:51D1
					DRAW_EXTERNAL_TILE(si);
					goto _51d7;
				case 0x11://^51D7
					//^32CB:51D7
_51d7:
					if (di == OBJECT_END_MARKER)
						continue;
					//^32CB:51DF
					SUMMARY_DRAW_CREATURE(di, si, 0x01ffffff);
					continue;
			}
			continue;
		}
		//^32CB:51F0
		if (si == 3) // at D1F
			//^32CB:51F5
			_4976_4dda = bp06;
		switch (bp06) {
			case 0://^520D // wall
				//^32CB:520D
				if (di != OBJECT_END_MARKER) {
					//^32CB:5212
					SUMMARY_DRAW_CREATURE(di, si, 0x01ffffff);
				}
				//^32CB:5220
				//^32CB:5222
				DRAW_WALL_TILE(si);
				continue;
			case 17://^5224
				//^32CB:5224
				DRAW_DOOR_TILE(si);
				continue;
			case 18://^522B
				//^32CB:522B
				DRAW_STAIRS_SIDE(si);
				goto _52d1;
			case 19://^5233
				//^32CB:5233
				DRAW_STAIRS_FRONT(si);
				goto _52d1;
			case 2://^523B // pit
				//^32CB:523B
				DRAW_PIT_TILE(si);
				goto _52b1;
			case 16://^5243	// seeing door frame slot in front from door tile
				//^32CB:5243
				if (si == 3) {
					//^32CB:5248
					// Check if this door type has door frames of not. if so, then draw the door frame
					if (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, glbMapDoorType[GET_ADDRESS_OF_RECORD0(tblCellTilesRoom[3].xsrd.w6[2])->DoorType()], dtWordValue, GDAT_DOOR_NO_FRAMES) == 0) {
						//^32CB:5279
						QUERY_TEMP_PICST(
							glbGeneralFlipGraphics, 0x2b, 0x2b, 0, 0, 2, QUERY_CREATURE_BLIT_RECTI(3, 2, 0), 0xffff, 
							glbSceneColorKey, -1, GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, 6
							);
						DRAW_TEMP_PICST();
					}
				}
				goto _52b1;
			case 1://^52B1 // floor
			case 5://^52B1 // teleporter
				//^32CB:52B1
_52b1:
				bp04 = (DRAW_EXTERNAL_TILE(si) != 0) ? 0x01fffc00 : 0x01ffffff;
				DRAW_PIT_ROOF(si);
				//^32CB:52D1
_52d1:
				DRAW_STATIC_OBJECT(si, bp04, 1);
				if (bp06 == 5)	// teleporter
				{
					//^32CB:52E8
					DRAW_TELEPORTER_TILE(si, GDAT_CATEGORY_TELEPORTERS, 0); // DRAW_TELEPORTER_TILE(si, 0x18, 0)
				}
				break;
			case 3://^52F4 // stair case
			case 4://^52F4 // door
			case 6://^52F4 // false wall
			case 7://^52F4 // map exit
			case 8://^52F4
			case 9://^52F4
			case 10://^52F4
			case 11://^52F4
			case 12://^52F4
			case 13://^52F4
			case 14://^52F4
			case 15://^52F4
				break;
		}
		//^32CB:52F4
	}
	//^32CB:5300
	return;
}

//^3E74:5A0C
void SkWinCore::COPY_SKIMAGE_HEADER(const U8 *xx, U8 *yy)
{
	//^3E74:5A0C
	ENTER(0);
	//^3E74:5A0F
	COPY_MEMORY(
		&xx[-6],
		&yy[-6],
		6
		);
	//^3E74:5A41
	return;
}

//^0B36:15E5
void SkWinCore::APPLY_MIRROR_FLIP_TO(const U8 *blitSrc, U8 *blitDst, X16 mirrorFlip)
{
	//^0B36:15E5
	ENTER(2);
	//^0B36:15EB
	X16 si = READ_UI16(blitSrc,-4);
	X16 bp02 = READ_UI16(blitSrc,-2);
	X16 di = READ_UI16(blitSrc,-6);
	COPY_SKIMAGE_HEADER(blitSrc, blitDst);
	FIRE_BLIT_PICTURE(blitSrc, blitDst, ALLOC_TEMP_ORIGIN_RECT(si, bp02), 0, 0, si, si, -1, mirrorFlip, di, di, NULL);
	//^0B36:1643
	return;
}

//^32CB:0753
void SkWinCore::DRAW_MIRRORED_PIC(const U8 *blitSrc, U8 *blitDst)
{
	//^32CB:0753
	ENTER(0);
	//^32CB:0756
	APPLY_MIRROR_FLIP_TO(blitSrc, blitDst, 1);
	//^32CB:076C
	return;
}

//^32CB:0B11
// SPX: _32cb_0b11 renamed DRAW_RAIN
void SkWinCore::DRAW_RAIN()
{
	//^32CB:0B11
	ENTER(20);
	//^32CB:0B16
	if (glbRainStrength == 0 || tlbRainScene[RCJ(5,glbRainSceneType)][2] == 0)
		return;
	//^32CB:0B31
	i16 bp0a = _4976_00f6;
	i16 bp0c = _4976_00f8;
	SRECT bp14;
	if (QUERY_BLIT_RECT(NULL, &bp14, 0x2be, &bp0a, &bp0c, -1) == NULL)
		return;
	//^32CB:0B64
	U8 bp07;
	U16 bp06;
	QUERY_RAINFALL_PARAM(&bp07, &bp06);
	U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x17, glbMapGraphicsSet, bp07);
	ALLOC_IMAGE_MEMENT(GDAT_CATEGORY_ENVIRONMENT, glbMapGraphicsSet, bp07);
	X16 si;
	if (bp06 == 1) {
		//^32CB:0BA5
		si = ALLOC_TEMP_CACHE_INDEX();
		ALLOC_NEW_PICT(si, READ_UI16(bp04,-4), READ_UI16(bp04,-2), 4);
		bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x17, glbMapGraphicsSet, bp07);
		DRAW_MIRRORED_PIC(bp04, reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(si)));
		bp04 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(si));
	}
	//^32CB:0BFD
	_44c8_20a4(bp04, _4976_4c16, NULL, &bp14, READ_UI16(bp04,-4) * READ_UI16(bp04,-2) -40 -(RAND() & 0x1f), RAND() & 255, _4976_00f6, 0,
		_32cb_0649(GDAT_CATEGORY_ENVIRONMENT, glbMapGraphicsSet, bp07, 0)
		);
	if (bp06 == 1) {
		//^32CB:0C61
		FREE_TEMP_CACHE_INDEX(si);
	}
	//^32CB:0C68
	FREE_IMAGE_MEMENT(GDAT_CATEGORY_ENVIRONMENT, glbMapGraphicsSet, bp07);
	//^32CB:0C7A
	return;
}

//^32CB:5340
// SPX: _32cb_5340 renamed DRAW_PLAYER_TILE (which display the current tile the player is on)
void SkWinCore::DRAW_PLAYER_TILE()
{
	//^32CB:5340
	ENTER(14);
	//^32CB:5346
	X16 bp06;
	X16 bp0e;
	bp06 = bp0e = tblCellTilesRoom->xsrd.w0;
	X16 di;
	X16 si;
	switch (bp0e) {
		case 0x10://^536D
			{
				//^32CB:536D
				Door *door = GET_ADDRESS_OF_RECORD0(tblCellTilesRoom->xsrd.w6[1]);	//*bp04
				if (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, glbMapDoorType[door->DoorType()], dtWordValue, GDAT_DOOR_NO_FRAMES) != 0)
					break;
				//^32CB:53A6
				DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_DOOR_FRAME_FRONT_D1, QUERY_CREATURE_BLIT_RECTI(0, 2, 0), glbSceneColorKey, glbGeneralFlipGraphics);
				break;
			}
		case 0x13://^53CE	// on stairs tile (coming from stairs)
			//^32CB:53CE
			si = tblCellTilesRoom->xsrd.w6[0];
			DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet
//					, (si != 0) ? 0x39 : 0x4D
				, (si != 0) ? GDAT_GFXSET_STAIRS_RAMP_UP_S0_L : GDAT_GFXSET_STAIRS_RAMP_DOWN_S0_L
				, (si != 0) ? 0x32b : 0x338, glbSceneColorKey, 0);
			DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet
//					, (si != 0) ? 0x3a : 0x4e
				, (si != 0) ? GDAT_GFXSET_STAIRS_RAMP_UP_S0_R : GDAT_GFXSET_STAIRS_RAMP_DOWN_S0_R
				, (si != 0) ? 0x32c : 0x339, glbSceneColorKey, 0);
			break;
		case 0x02://^542E
			//^32CB:542E
			DRAW_PIT_TILE(0);
			break;
		case 0x11://^5437
			//^32CB:5437
			DRAW_DOOR_TILE(0);
			di = 0;
			goto _5495;
	}
	//^32CB:5442
	DRAW_PIT_ROOF(0);
	TELE_inf bp0c;
	if ((di = GET_TELEPORTER_DETAIL(&bp0c, tblCellTilesRoom->posx, tblCellTilesRoom->posy)) == 0) {
		//^32CB:546B
		DRAW_EXTERNAL_TILE(0); // this is where floor pad is displayed
	}
	//^32CB:5472
	DRAW_STATIC_OBJECT(0, 0x01ffffff, 0);
	if (bp06 == 5)
	{
		//^32CB:5488
		DRAW_TELEPORTER_TILE(0, 0x18, 0);
	}
	//^32CB:5495
_5495:
	DRAW_RAIN();
	if (di != 0) {
		//^32CB:549D
		DRAW_EXTERNAL_TILE(0);
	}
	//^32CB:54A4
	_32cb_2d8c(tblCellTilesRoom->xsrd.w4, 0, 0x01ffffff);
	//^32CB:54BA
	return;
}

//^32CB:5D0D
// SPX: _32cb_5d0d renamed DISPLAY_VIEWPORT
void SkWinCore::DISPLAY_VIEWPORT(Bit16u dir, i16 xx, i16 yy)
{
	//^32CB:5D0D
	ENTER(6);
	//^32CB:5D13
	IBMIO_USER_INPUT_CHECK();
	// SPX: glbLightLevel is between 0 (light) and 5 (dark). Palette is thereafter controlled by value between 0 (light) and 64 (dark)
	// Having *10 makes 0 to 50 -> loss of darkest colors. Having *13 makes 0 to 65 -> full range (full darkness)
	_4976_5a88 = glbLightLevel * 10;	
	//_4976_5a88 = glbLightLevel * 12;	// SPX: * 13 seems more appropriate to get darkest values
	_4976_5aa0 = dir;
	_4976_5a9c = xx;
	_4976_5a9e = yy;
	tblCellTilesRoom = reinterpret_cast<CellTileDetail *>(ALLOC_MEMORY_RAM(sizeof(CellTileDetail) * 23, afDefault, 1024));
	_4976_5a84 = reinterpret_cast<U8 (*)[17]>(ALLOC_MEMORY_RAM(358, afZeroMem, 1024));
	_4976_5aa4 = reinterpret_cast<U8 (*)[17]>(ALLOC_MEMORY_RAM(358, afDefault, 1024));
	FILL_STR(*_4976_5aa4, 358, 255, 1);
	_4976_5a7c = reinterpret_cast<U32 (*)[23]>(ALLOC_MEMORY_RAM(4*23, afZeroMem, 1024));
	_4976_5be2 = reinterpret_cast<U32 (*)[23]>(ALLOC_MEMORY_RAM(4*23, afZeroMem, 1024));
	_4976_4bc8 = 1;
	INIT_BACKBUFF();
	_4976_4dde = 9;
	i16 si;

	SkD((DLV_MOVE, "DISPLAY_VIEWPORT: dir:%d x:%d y:%d darkness:%d\n"
		, dir
		, xx
		, yy
		, _4976_5a88));
	

	//^32CB:5DDB
	for (si = 0; si < 9; si++) {
		//^32CB:5DDF
		_4976_4d1a[si].w8 = OBJECT_NULL;
		//^32CB:5DEE
	}
	//^32CB:5DF4
	IBMIO_USER_INPUT_CHECK();
	_4976_5aa2 = (_4976_4ddc != 0 || glbTableToMove != OBJECT_NULL) ? 1 : 0;
	CHANCE_TABLE_OPERATION();
	//^32CB:5E15
	for (si = 0x16; si >= 0; si--) {
		//^32CB:5E1A
		_32cb_4185_TILE_ROOM(xx, yy, si, _4976_5aa0);
		//^32CB:5E2C
	}

	//   |--|--|--|--|--|--|--|
	// 4 |21|19|17|16|18|20|22|
	//   |--|--|--|--|--|--|--|
	// 3 |  |14|12|11|13|15|  |
	//   |--|--|--|--|--|--|--|
	// 2 |  | 9| 7| 6| 8|10|  |
	//   |--|--|--|--|--|--|--|
	// 1 |  |  | 4| 3| 5|  |  |
	//   |--|--|--|--|--|--|--|
	// 0 |  |  | 1| 0| 2|  |  |
	//   |--|--|--|--|--|--|--|
	SkD((DLV_DBG_CELL, "DBG: U r at (%2u,%2u,%2u) %u\n"
		"|-------|-------|-------|-------|-------|-------|-------|\n"
		"|       |%02X %04X|%02X %04X|%02X %04X|%02X %04X|%02X %04X|       |\n"
		"|-------|-------|-------|-------|-------|-------|-------|\n"
		"|       |%02X %04X|%02X %04X|%02X %04X|%02X %04X|%02X %04X|       |\n"
		"|-------|-------|-------|-------|-------|-------|-------|\n"
		"|       |%02X %04X|%02X %04X|%02X %04X|%02X %04X|%02X %04X|       |\n"
		"|-------|-------|-------|-------|-------|-------|-------|\n"
		"|       |       |%02X %04X|%02X %04X|%02X %04X|       |       |\n"
		"|-------|-------|-------|-------|-------|-------|-------|\n"
		"|       |       |%02X %04X|%02X %04X|%02X %04X|       |       |\n"
		"|-------|-------|-------|-------|-------|-------|-------|\n"
		, (Bitu)glbPlayerMap, (Bitu)glbPlayerPosX, (Bitu)glbPlayerPosY, (Bitu)glbPlayerDir
		, (Bitu)tblCellTilesRoom[19].xsrd.w0, (Bitu)tblCellTilesRoom[19].xsrd.w4
		, (Bitu)tblCellTilesRoom[17].xsrd.w0, (Bitu)tblCellTilesRoom[17].xsrd.w4
		, (Bitu)tblCellTilesRoom[16].xsrd.w0, (Bitu)tblCellTilesRoom[16].xsrd.w4
		, (Bitu)tblCellTilesRoom[18].xsrd.w0, (Bitu)tblCellTilesRoom[18].xsrd.w4
		, (Bitu)tblCellTilesRoom[20].xsrd.w0, (Bitu)tblCellTilesRoom[20].xsrd.w4

		, (Bitu)tblCellTilesRoom[14].xsrd.w0, (Bitu)tblCellTilesRoom[14].xsrd.w4
		, (Bitu)tblCellTilesRoom[12].xsrd.w0, (Bitu)tblCellTilesRoom[12].xsrd.w4
		, (Bitu)tblCellTilesRoom[11].xsrd.w0, (Bitu)tblCellTilesRoom[11].xsrd.w4
		, (Bitu)tblCellTilesRoom[13].xsrd.w0, (Bitu)tblCellTilesRoom[13].xsrd.w4
		, (Bitu)tblCellTilesRoom[15].xsrd.w0, (Bitu)tblCellTilesRoom[15].xsrd.w4

		, (Bitu)tblCellTilesRoom[ 9].xsrd.w0, (Bitu)tblCellTilesRoom[ 9].xsrd.w4
		, (Bitu)tblCellTilesRoom[ 7].xsrd.w0, (Bitu)tblCellTilesRoom[ 7].xsrd.w4
		, (Bitu)tblCellTilesRoom[ 6].xsrd.w0, (Bitu)tblCellTilesRoom[ 6].xsrd.w4
		, (Bitu)tblCellTilesRoom[ 8].xsrd.w0, (Bitu)tblCellTilesRoom[ 8].xsrd.w4
		, (Bitu)tblCellTilesRoom[10].xsrd.w0, (Bitu)tblCellTilesRoom[10].xsrd.w4

		, (Bitu)tblCellTilesRoom[ 4].xsrd.w0, (Bitu)tblCellTilesRoom[ 4].xsrd.w4
		, (Bitu)tblCellTilesRoom[ 3].xsrd.w0, (Bitu)tblCellTilesRoom[ 3].xsrd.w4
		, (Bitu)tblCellTilesRoom[ 5].xsrd.w0, (Bitu)tblCellTilesRoom[ 5].xsrd.w4

		, (Bitu)tblCellTilesRoom[ 1].xsrd.w0, (Bitu)tblCellTilesRoom[ 1].xsrd.w4
		, (Bitu)tblCellTilesRoom[ 0].xsrd.w0, (Bitu)tblCellTilesRoom[ 0].xsrd.w4
		, (Bitu)tblCellTilesRoom[ 2].xsrd.w0, (Bitu)tblCellTilesRoom[ 2].xsrd.w4
		));

#if DLV_DBG_CEL2
	{
		char mass[21*54 +1];
		// 54 x 21
		strcpy(mass,
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"----+----+-----+----+----+ +----+----+-----+----+----\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"----+----+-----+----+----+ +----+----+-----+----+----\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"----+----+-----+----+----+ +----+----+-----+----+----\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"----+----+-----+----+----+ +----+----+-----+----+----\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\n"
			"XXXX|XXXX|XXXXX|XXXX|XXXX| |XXXX|XXXX|XXXXX|XXXX|XXXX\0"
			);
		static const U8 tblX[] = {0,1,2,3, 5,6,7,8, 10,11,12,13,14, 16,17,18,19, 21,22,23,24};
		static const U8 tblY[] = {0, 2,3,4,5, 7,8,9,10, 12,13,14,15, 17,18,19,20};
		for (int yc=0; yc<17; yc++) {
			int xc;
			for (xc=0; xc<21; xc++) {
				U8 v = U8(_4976_5aa4[xc][16 -yc]);
				char t = '.';
				if (0 <= v && v <= 9) t = v + '0';
				else if (10 <= v && v <= 15) t = v + 'A' -10;

				mass[54*tblY[yc] +tblX[xc]] = t;
			}
			for (xc=0; xc<21; xc++) {
				U8 v = U8(_4976_5a84[xc][16 -yc] & 15);
				char t = '.';
				if (0 <= v && v <= 9) t = v + '0';
				else if (10 <= v && v <= 15) t = v + 'A' -10;

				mass[54*tblY[yc] +28 +tblX[xc]] = t;
			}
		}
		SkD((DLV_DBG_CEL2, "DBG: L=enemy map, R=movable map\n%s\n", mass));
	}
#endif

	//^32CB:5E31
	X16 di = 0;
	X16 bp02 = 0;
	X8 bp03 = 0xff;
	if (tblCellTilesRoom[7].xsrd.w0 == 0 && tblCellTilesRoom[6].xsrd.w0 == 0 && tblCellTilesRoom[8].xsrd.w0 == 0) // wall at D2
		//^32CB:5E57
		bp03 = 0x71;
	if (tblCellTilesRoom[4].xsrd.w0 == 0 && tblCellTilesRoom[3].xsrd.w0 == 0 && tblCellTilesRoom[5].xsrd.w0 == 0) // wall at D1
		//^32CB:5E74
		bp03 = 0x70;
	//^32CB:5E78
	if (bp03 != 0xff) { // trim ceil(bp02) and floor(di) to omit hidden area by drawing wall
		U16 bp06 = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, bp03);
		di = bp06 >> 8;
		bp02 = bp06 & 0xff;
		// SPX: if custom dungeon/GDAT use more than 6 gfxsets, better bypass these values from tables
		if (SkCodeParam::bUseFixedMode && glbMapGraphicsSet >= 0 && glbMapGraphicsSet <= 5)
		{
			if (di == 0) {
				if (bp03 == 0x71) {
					di = _4976_4718_tblGfxSet[RCJ(6,glbMapGraphicsSet)]; // D2 to D0
				}
				else {
					//^32CB:5EBC
					di = _4976_4700_tblGfxSet[RCJ(6,glbMapGraphicsSet)]; // D1 to D0
				}
			}
			//^32CB:5EC9
			if (bp02 == 0) {
				if (bp03 == 0x71) {
					//^32CB:5ED5
					bp02 = _4976_470c_tblGfxSet[RCJ(6,glbMapGraphicsSet)]; // envisible D2 to D0
				}
				else {
					//^32CB:5EE4
					bp02 = _4976_46f4_tblGfxSet[RCJ(6,glbMapGraphicsSet)]; // envisible D1 to D0
				}
			}
		}
	}
	//^32CB:5EF4
	TRIM_BLIT_RECT(0, 0, 0, bp02);
	if (SET_GRAPHICS_FLIP_FROM_POSITION(0x20, dir, xx, yy) != 0) {
		//^32CB:5F1B
		DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_CEIL, 0x2bc, -1, 1); // draw ceil
	}
	else {
		//^32CB:5F1F
		DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_CEIL, 0x2bc, -1, 0); // draw ceil
	}
	//^32CB:5F35
	_098d_0c45();
	TRIM_BLIT_RECT(0, di, 0, 0);
	if (SET_GRAPHICS_FLIP_FROM_POSITION(1, dir, xx, yy) != 0) {
		//^32CB:5F5F
		DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_FLOOR, 0x2bd, -1, 1); // draw floor
	}
	else {
		//^32CB:5F63
		DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, GDAT_GFXSET_FLOOR, 0x2bd, -1, 0); // draw floor
	}
	//^32CB:5F79
	_098d_0c45();
	glbGeneralFlipGraphics = SET_GRAPHICS_FLIP_FROM_POSITION(0, dir, xx, yy);
	IBMIO_USER_INPUT_CHECK();
	ENVIRONMENT_DISPLAY_ELEMENTS(dir, xx, yy);
	_32cb_5c67();
	_32cb_5a8f();
	DRAW_DUNGEON_TILES(); // render walls, flying items
	IBMIO_USER_INPUT_CHECK();
	DRAW_PLAYER_TILE(); // render current player tile
	IBMIO_USER_INPUT_CHECK();
#if UseAltic
	DEALLOC_UPPER_MEMORY(92);
	DEALLOC_UPPER_MEMORY(92);
	DEALLOC_UPPER_MEMORY(358);
	DEALLOC_UPPER_MEMORY(358);
	DEALLOC_UPPER_MEMORY(414);
#else
	DEALLOC_UPPER_MEMORY(414+358+358+92+92);
#endif
	//^32CB:5FCE
	return;
}
//^482B:05BF
void SkWinCore::_482b_05bf(Bit16u xx)
{
	//^482B:05BF
	ENTER(0);
	//^482B:05C2
	if (xx != 0) {
		if (_4976_49d2 > 0) {
			PLAY_SOUND(_4976_49d2, _4976_5efe);
		}
		_4976_49d2 = 0;
		return;
	}
	if (_4976_49d0 > 0) {
		PLAY_SOUND(_4976_49d0, _4976_5f02);
	}
	//^482B:0606
	_4976_49d0 = 0;
	//^482B:060C
	return;
}
//^3E74:5673
X32 SkWinCore::_3e74_5673(X32 cacheHash, U16 *piYaCacheIndex, X16 ifTryInsert)
{
	//^3E74:5673
	ENTER(2);
	//^3E74:5677
	if (glbGameTick != _4976_5d2a) {
		//^3E74:568A
		_3e74_44ad();
	}
	//^3E74:568E
	U16 bp02;
	if (FIND_ICI_FROM_CACHE_HASH(cacheHash, &bp02) == 0) {
		//^3E74:56A4
		*piYaCacheIndex = (ifTryInsert != 0) ? INSERT_CACHE_HASH_AT(cacheHash, bp02) : 0xffff;
		return 0;
	}
	//^3E74:56CB
	*piYaCacheIndex = _4976_5c7e[bp02];
	return *reinterpret_cast<i32 *>(_3e74_48c9(QUERY_MEMENTI_FROM(*piYaCacheIndex | 0x8000))) -22;
}

//^1C9A:0247
void SkWinCore::_1c9a_0247(ObjectID rl)
{
	//^1C9A:0247
	ENTER(2);
	//^1C9A:024C
	i16 si = rl.DBIndex();
	U16 bp02;
	if (_3e74_5673(0x20000000 | si | glbCurrentThinkingCreatureID.DBIndex(), &bp02, 0) != 0) {
		//^1C9A:0281
		FREE_TEMP_CACHE_INDEX(bp02);
	}
	//^1C9A:028A
	if (_3e74_5673(0x28000000 | si | glbCurrentThinkingCreatureID.DBIndex(), &bp02, 0) != 0) {
		//^1C9A:02B7
		FREE_TEMP_CACHE_INDEX(bp02);
	}
	//^1C9A:02C0
	return;
}

//^0CEE:159B
void SkWinCore::QUERY_MESSAGE_TEXT(U8 *str, ObjectID rl, Bit16u ww)
{
	//^0CEE:159B
	ENTER(154);
	//^0CEE:15A1
	U16 si = 0;
	//^0CEE:15A3
	U8 bp009a[200];	// U8 bp009a[128]; SPX: extended to 200, else it'll crash for some DM1 texts
	U8 *bp04 = bp009a;
	//^0CEE:15AD
	U8 bp19 = 0xff;
	//^0CEE:15B1
	Text *bp10;
	U16 bp18;
	if (rl.DBType() == dbScroll) {
		//^0CEE:15BF
		Scroll *_bp10 = GET_ADDRESS_OF_RECORD7(rl);
		//^0CEE:15CD
		U16 bp14 = _bp10->w2_a_f();
		//^0CEE:15DD
		if (bp14 > 1) {	// original: if (bp14 != 0) // fixed to > 1 as it is done in DMDC2
			//^0CEE:15E1
			bp19 = U8(bp14);	// get text number reference which is within GDAT
			//^0CEE:15E4
			goto _1678;
		}
		//^0CEE:15E7
		// SPX: else, get text reference from dungeon.dat
		bp10 = &reinterpret_cast<Text *>(static_cast<U8 *>(glbDBObjectData[dbText]))[_bp10->ReferredText() & 0x03ff];
	}
	else {
		ATLASSERT(rl.DBType() == dbText);

		//^0CEE:1609
		bp10 = GET_ADDRESS_OF_RECORD2(rl);
	}
	//^0CEE:1617
	//^0CEE:161A
	bp18 = bp10->TextMode();
	//^0CEE:1626
	if (bp18 == 0 || (bp18 == 1 && bp10->SimpleTextExtUsage() == 14)) {
		//^0CEE:164B
		if (bp10->TextVisibility() != 0 || (ww & 0x8000) != 0) {
			//^0CEE:1663
			if (bp18 == 1) {
				//^0CEE:1669
				bp19 = bp10->TextIndex() & 0xff;
			}
			//^0CEE:167B
_1678:
			ww = ww & 0x7fff;
			//^0CEE:1684
			U8 bp15;
			if (ww == 1) {
				//^0CEE:168A
				*(bp04++) = vbLf;
				//^0CEE:1694
				bp15 = ' ';
			}
			//^0CEE:1698
			bp15 = vbLf;
			//^0CEE:169C
			if (bp19 != 0xff) {
				//^0CEE:16A2
				// SPX: text message stored in GDAT and not DUNGEON.DAT
				bp04 += SK_STRLEN(QUERY_GDAT_TEXT(GDAT_CATEGORY_MESSAGES, 0x00, bp19, bp04));
			}
			else {
				//^0CEE:16C7
				U16 bp14 = 0;
				//^0CEE:16CC
				U16 di = 0;
				//^0CEE:16CE
				U16 *bp0c = &dunTextData[bp10->TextIndex()];

				while (true) {
					//^0CEE:16ED
					U16 bp12;
					if (bp14 == 0) {
						//^0CEE:16F3
						si = *(bp0c++);
						//^0CEE:16FD
						bp12 = (si >> 10) & 0x1f;
					}
					//^0CEE:1704
					else if (bp14 == 1) {
						//^0CEE:170A
						bp12 = (si >> 5) & 0x1f;
					}
					else {
						//^0CEE:1711
						bp12 = si & 0x1f;
					}
					//^0CEE:1713
					//^0CEE:1719
					bp14 = ++bp14 % 3;
					//^0CEE:1729
					if (di != 0) {
						//^0CEE:172D
						*bp04 = 0;
						//^0CEE:1734
						const U8 *bp08;
						if (di == 30) {
							//^0CEE:1739
							bp08 = reinterpret_cast<const Bit8u *>(_4976_0262[bp12]);
						}
						else {
							//^0CEE:1744
							bp08 = reinterpret_cast<const Bit8u *>(_4976_0362[bp12]);
						}
						//^0CEE:174C
						//^0CEE:1752
						SK_STRCAT(bp04, bp08);
						//^0CEE:1766
						bp04 += SK_STRLEN(bp08);
						//^0CEE:1776
						di = 0;
						//^0CEE:1778
						continue;
					}
					//^0CEE:177B
					if (bp12 < 0x1c) {
						//^0CEE:1781
						if (bp12 == 0x1a) {
							//^0CEE:1787
							bp12 = 0x20;
							//^0CEE:179F
							//^0CEE:178C
							*(bp04++) = U8(bp12);
							//^0CEE:17B9
							continue;
						}
						//^0CEE:178E
						else if (bp12 == 0x1b) {
							//^0CEE:1794
							bp12 = 0x2e;
							//^0CEE:1799
							//^0CEE:179F
							*(bp04++) = U8(bp12);
							//^0CEE:17B9
							continue;
						}
						else {
							//^0CEE:179B
							bp12 += 0x41;
							//^0CEE:179F
							*(bp04++) = U8(bp12);
							//^0CEE:17B9
							continue;
						}
					}
					//^0CEE:17A7
					else if (bp12 == 0x1c) {
						//^0CEE:17AD
						*(bp04++) = bp15;
						//^0CEE:17B9
						continue;
					}
					//^0CEE:17BC
					else if (bp12 <= 0x1e) {
						//^0CEE:17C2
						di = bp12;
						//^0CEE:17C5
						continue;
					}
					else {
						//^0CEE:17C0
						break;
					}
				}
			}
		}
	}
	//^0CEE:17C8
	*bp04 = 0;
	//^0CEE:17CF
	FORMAT_SKSTR(bp009a, str);
	//^0CEE:17E3
	return;
}



//^2FCF:164E
X16 SkWinCore::_2fcf_164e(ObjectID xx, ObjectID yy)
{
	//^2FCF:164E
	ENTER(0);
	//^2FCF:1653
	ObjectID si = xx;
	if (si.DBType() == dbContainer && QUERY_CLS2_FROM_RECORD(si) < 8) {
		//^2FCF:1672
		si = GET_ADDRESS_OF_RECORD9(si)->GetContainedObject();
		while (si != OBJECT_END_MARKER) {
			//^2FCF:1683
			ObjectID di = GET_DISTINCTIVE_ITEMTYPE(si);
			if (di != yy) {
				//^2FCF:1691
				if (_2fcf_164e(si, yy) == 0)
					goto _16a4;
			}
            //^2FCF:169F
			return 1;
			//^2FCF:16A4
_16a4:
			si = GET_NEXT_RECORD_LINK(si);
		}
	}
	//^2FCF:16B2
	return 0;
}

//^2FCF:16FF
Bit16u SkWinCore::_2fcf_16ff(ObjectID rl)
{
	//^2FCF:16FF
	ENTER(14);
	//^2FCF:1705
	X16 bp0c = 0;
	X16 bp0a = 0;
	Champion *champion = glbChampionSquad;	//*bp04
	ObjectID di;
	U16 si = 0; // uninited val
	ObjectID *bp08;
	for (; bp0a < glbChampionsCount; bp0a++, champion++) {
		//^2FCF:1719
		if (champion->curHP() == 0)
			continue;
		bp08 = champion->inventory;
		for (si = 0; si < INVENTORY_MAX_SLOT && bp0c == 0; si++) {
			//^2FCF:1737
			di = *bp08; bp08++;
			//^2FCF:1741
_1741:
			U16 bp0e = GET_DISTINCTIVE_ITEMTYPE(di);
			if (bp0e == rl || _2fcf_164e(di, rl) != 0)
				return 1;
			//^2FCF:1763
		}
		//^2FCF:176F
	}
	//^2FCF:1780
	if (glbRightPanelType == RIGHT_PANEL_CONTAINER) {	// 5
		//^2FCF:1787
		for (si = 0; si < CONTAINER_MAX_SLOT; si++) {	// < 8
			//^2FCF:178B
			di = glbCurrentContainerItems[si];
			if (di == OBJECT_NULL)
				continue;
			if (GET_DISTINCTIVE_ITEMTYPE(di) != rl)
				continue;
			return 1;
			//^2FCF:17A8
		}
	}
	//^2FCF:17AE
	if (bp0c == 0) {
		bp0c = 1;
		di = glbLeaderHandPossession.object;
		goto _1741;
	}
	return 0;
}


//^01B0:1997
// SPX: _01b0_1997 renamed BLEND_TO_SNDBUFF_TANDY
void SkWinCore::BLEND_TO_SNDBUFF_TANDY(void *ref, Bit16u xx, Bit16u yy, Bit16u zz) { // TODO: Unr
	Unr();
}

void SkWinCore::_01b0_0ec3(X16 xx) { // TODO: Unr
	Unr();
}

//^01B0:1A6D
// SPX: _01b0_1a6d renamed BLEND_TO_SNDBUFF_GENERAL
void SkWinCore::BLEND_TO_SNDBUFF_GENERAL(U8 *buff, Bit16u buffSize, Bit16u volume, Bit16u caller, Bit16u ss, Bit16u tt)
{
	//^01B0:1A6D
	ENTER(6);
	//^01B0:1A73
	X16 si = volume;
	U16 di = caller;
	if (si == 0)
		return;
	//^01B0:1A80
	if (di > 0x1f40 && tt != 0) {
		//^01B0:1A8C
		di >>= 1;
		buffSize >>= 1;
		_01b0_14d9 = 1;
	}
	else {
		//^01B0:1AA3
		_01b0_14d9 = 0;
	}
	//^01B0:1AAA
	if (glbSoundBlasterBasePort == 0)
		return;
	//^01B0:1AB4
	if (_04bf_04f1 != 0) {
		//^01B0:1ABC
		if (_04bf_04f3 != 0) {
			//^01B0:1AC3
			U16 bp02;
			for (bp02 = 0; bp02 < 0x100; bp02++) {
				//^01B0:1ACA
				X8 bp05 = (ss == 7)
					? _04bf_03d6[RCJ(220,(bp02 * 220) >> 8)]
					: ((((bp02 +0x80) * si) >> 8) +0x80)
					;
				_01b0_13d8[bp02] = bp05;
				//^01B0:1B00
			}
		}
		//^01B0:1B0A
		_04bf_04f1 =  si;
		if (ss == 7) {
			//^01B0:1B14
			_01b0_0ec3(si);
            _04bf_04f3 = 0;
		}
	}
	//^01B0:1B20
	bool jne = (sndLockSoundBuffer != 0);
	sndSoundToPlayBuffer = buff;
	sndLockSoundBuffer = buffSize;
	if (!jne) {
		//^01B0:1B3F
		U16 bp04;
		switch (ss) {
		case 5://^1B51	// Sound card type 5
			//^01B0:1B51
			glbSoundFreq_13ce = 0x1551;	// 0x1551 = 5457 (*2 = 10914)
			goto _1b7b;
		case 3://^1B5A	// Sound card type 3
			//^01B0:1B5A
			glbSoundFreq_13ce = 0x1624;	// 0x1624 = 5668 (*2 = 11336)
			goto _1b7b;
		case 7://^1B63	// Sound card type 7
			//^01B0:1B63
			outportb(_01b0_14db, 0x40);
			glbSoundFreq_13ce = 0x15e9;
			goto _1b7b;
		case ScardSBlaster://^1B74	// Sound card type 6
			//^01B0:1B74
			glbSoundFreq_13ce = 0x159e;	// 0x159e = 5534 (*2 = 11068)
			//^01B0:1B7B
_1b7b:
			bp04 = 0x001234dc / di;
			_01b0_13c8 = bp04;
		case 4://^1B92
			break;
		}
	}
	//^01B0:1B92
	//^01B0:1B93
	return;
}

//^01B0:1BA1
// SPX: _01b0_1ba1 renamed BLEND_TO_SNDBUFF_SNDCARD_4
void SkWinCore::BLEND_TO_SNDBUFF_SNDCARD_4(void *ref, Bit16u xx, Bit16u yy, Bit16u zz) { // TODO: Unr
	Unr();
}

//^47EB:02E0
Bit16u SkWinCore::_47eb_02e0(SoundStructure *xx, SoundStructure *yy)
{
	//^47EB:02E0
	ENTER(0);
	//^47EB:02E4
	U16 si;
	if (xx->b4 > yy->b4)
		//^47EB:02F4
		return si = 1;
	//^47EB:02F9
	if (yy->b4 == yy->b4) {
		//^47EB:0309
		U8 cl = xx->b8;
		U8 dl = yy->b8;
		if (cl >= dl)
			return si = 1;
		return si = 0;
	}
	//^47EB:032C
	return si = 0;
}
//^47EB:0048
Bit8u *SkWinCore::_47eb_0048(shelf_memory info, Bit16u physPage0to3)
{
#if UseAltic
	return REALIZE_GRAPHICS_DATA_MEMORY(info);
#else
//!	//^47EB:0048
//!	ENTER(4);
//!	//^47EB:004C
//!	U8 *bp04;
//!	if (info.AtEMS()) {
//!		//^47EB:005A
//!		bp04 = EMS_MAP_BANK_TO_MEMORY(info -0x00200000, physPage0to3);
//!	}
//!	else {
//!		//^47EB:0075
//!		bp04 = ((info & 0x000ffff0) << 12) | (info & 15);
//!	}
//!	//^47EB:009C
//!	return bp04;
#endif
}


//^19F0:2024
X16 SkWinCore::_19f0_2024(ObjectID rl, i16 ss, i16 tt)
{
	//^19F0:2024
	ENTER(20);
	//^19F0:202A
	ObjectID si = rl;
	i16 di;
	X16 bp10;
	X16 bp0e;
	if ((_4976_4ef2 & 0x10) != 0 && IS_CONTAINER_CHEST(si) != 0) {
		//^19F0:2040
		Container *bp08 = GET_ADDRESS_OF_RECORD9(si);
		bp0e = si.Dir();
		si = bp08->GetContainedObject();
		bp10 = 0xf;
	}
	else if (si.DBType() == dbCreature && (_4976_4ef2 & 0x28) != 0) {
		//^19F0:207A
		Creature *bp04 = GET_ADDRESS_OF_RECORD4(si);
		AIDefinition *bp0c = QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp04->CreatureType());
		bp10 = bp0c->IsStaticObject();
		if (((bp10 != 0) ? (_4976_4ef2 & 8) : (_4976_4ef2 & 0x20)) != 0) {
			//^19F0:20C0
			bp0e = 8;
			si = bp04->GetPossessionObject();
			bp10 = (bp10 != 0) ? _48ae_01af(bp0c->w30, tt) : 0xf;
		}
	}
	else {
		//^19F0:20BD
		return 0xffff;
	}
	//^19F0:20EF
	X8 bp14[4];
	for (di = 0; di < 4; bp10 >>= 1, di++) {
		//^19F0:20F3
		bp14[di] = U8(bp10 & 1);
		//^19F0:20FB
	}
	//^19F0:2104
	for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
		//^19F0:2106
		if (bp14[si.Dir()] != 0) {
			//^19F0:2116
			if (CREATURE_CAN_HANDLE_IT(si, ss) != 0 || _19f0_2024(si, ss, tt) != 0xffff) {
				//^19F0:2138
				if (bp0e == 8) {
					bp0e = bp0e +si.Dir();
				}
				//^19F0:214B
				return bp0e;
			}
		}
		//^19F0:2150
	}
	//^19F0:215E
	return 0xffff;
}

//^19F0:2165
i16 SkWinCore::_19f0_2165(X16 aa, i16 xx, i16 yy, i16 ss, i16 tt, i16 vv, i16 ww)
{
	//^19F0:2165
	ENTER(12);
	SkD((DLV_DBG_SED3, "DBG: _19f0_2165 %04X %04X %04X %04X %04X %04X %04X \n", (Bitu)aa, (Bitu)xx, (Bitu)yy, (Bitu)ss, (Bitu)tt, (Bitu)vv, (Bitu)ww));
	//^19F0:216B
	X16 bp0c = aa & 0x80;
	X16 bp02;
	if (bp0c != 0) {
		//^19F0:2178
		aa &= 0xff7f;
		bp02 = 0xffff;
	}
	X16 bp0a;
	U16 bp04;
	ObjectID di;
	i16 si;
	//^19F0:2182
	if (_4976_4ef2 != 0) {
		//^19F0:218C
		if (ss == -1) {
			//^19F0:2192
			bp0a = 0;
			ss = xx +glbXAxisDelta[vv];
			tt = yy +glbYAxisDelta[vv];
		}
		else {
			//^19F0:21B7
			bp0a = (xx != ss || yy != tt) ? 0 : 1;
			if (bp0a == 0 && vv == -1) {
				//^19F0:21DB
				vv = CALC_VECTOR_DIR(xx, yy, ss, tt);
			}
		}
		//^19F0:21F2
		_19f0_045a(ss, tt);
		if (aa != 0 || (U8(_4976_521c) & 0x10) != 0) {
			//^19F0:2212
			if (U8(_4976_521c >> 5) == 0) {
				if (_4976_522c == 0xff) {
					//^19F0:2229
					_4976_522c = 0;
					_4976_522d = 0;
					if (bp0a == 0 && (_4976_4ef2 & 4) != 0) {
						//^19F0:2241
						_4976_522e[0] = (vv +2) & 3;
						if (_4976_521e == OBJECT_NULL) {
							//^19F0:2252
							_4976_521e = GET_TILE_RECORD_LINK(ss, tt);
						}
						//^19F0:2262
						for (di = _4976_521e; di != OBJECT_END_MARKER && (si = di.DBType()) <= dbActuator; di = GET_NEXT_RECORD_LINK(di)) {
							//^19F0:2268
							if (IS_OBJECT_ALCOVE(di) == 0)	// _0cee_317f
								continue;
							//^19F0:2273
							if (di.Dir() == _4976_522e[0]) {
								//^19F0:2282
								_4976_522d = 1;
							}
							else {
								//^19F0:2289
								_4976_522c = 1;
							}
							//^19F0:228E
						}
						//^19F0:22AB
						_4976_5220 = di;
					}
				}
				//^19F0:22AF
				if (_4976_522d == 0)
					goto _2474;
				if (aa == 1) {
					bp02 = ccm19;
					goto _2484;
				}
				//^19F0:22C7
				bp02 = ccm1A;
				bp04 = _4976_522e[0];
				di = _4976_5220;
				for (; di != OBJECT_END_MARKER; di = GET_NEXT_RECORD_LINK(di)) {
					//^19F0:22D9
					if (di.Dir() == bp04 && (si = di.DBType()) != dbCreature && si < dbMissile) {
						//^19F0:22F7
						if (CREATURE_CAN_HANDLE_IT(di, ww) != 0)
							goto _2484;
						//^19F0:2309
						if (_19f0_2024(di, ww, vv) != 0xffff)
							goto _2484;
					}
					//^19F0:231F
				}
				goto _2474;
			}
			//^19F0:2330
			if (_4976_522c == 0xff) {
				//^19F0:2337
				_4976_522c = 0;
				_4976_522e[0] = 0xff;
			}
			//^19F0:2341
			if (bp0a == 0 && glbCurrentMapIndex == glbCreatureMap && ss == glbCreaturePosX && tt != glbCreaturePosY)
				goto _2474;
			//^19F0:2365
			if ((_4976_4ef2 & ((bp0a != 0) ? 1 : 0x2a)) == 0)
				goto _2474;
			//^19F0:237C
			if (aa == 1) {
				//^19F0:2382
				if (bp0a == 0)
					//^19F0:238B
					bp02 = ccmPlaceMerchandise;
				goto _2484;
			}
			//^19F0:2393
			(bp0a == 0);
			//^19F0:2397
			for (di = _19f0_04bf(); di != OBJECT_END_MARKER; di = GET_NEXT_RECORD_LINK(di)) {
				//^19F0:239E
				bp04 = di.Dir();
				si = di.DBType();
				if (si >= dbCreature && si <= dbMissile) {
					//^19F0:23C0
					if ((si != dbCreature && CREATURE_CAN_HANDLE_IT(di, ww) != 0) || (bp04 = _19f0_2024(di, ww, vv)) != 0xffff) {
						//^19F0:23EA
						if (bp04 >= 8) {
							//^19F0:23F0
							bp02 = ccmMeleeAttack;
							bp04 &= 3;
							goto _2484;
						}
						X8 bp05;
						X16 bp08;
						//^19F0:23FD
						if (_4976_522e[0] == 0xff) {
							if (bp0a != 0) {
								//^19F0:240A
								if (vv == -1) {
									bp05 = 0xf;
									bp08 = 0;
								}
								else {
									bp05 = 3;
									bp08 = vv;
								}
							}
							else {
								//^19F0:2427
								bp05 = 0xc;
								bp08 = vv;
							}
							//^19F0:2431
							for (si = 0; si < 4; si++) {
								//^19F0:2435
								_4976_522e[bp08] = bp05 & 1;
								bp08 = (bp08 + 1) & 3;
								bp05 >>= 1;
								//^19F0:244E
							}
						}
						//^19F0:2454
						if (_4976_522e[bp04] != 0)
							goto _2484;
						_4976_522c = 1;
					}
				}
				//^19F0:246A
			}
		}
	}
	//^19F0:2474
_2474:
	if (bp0c != 0)
		//^19F0:247A
		_4976_4ee5 = xactrNo;
	//^19F0:247F
	return 0;

	//^19F0:2484
_2484:
	if (bp0c != 0) {
		//^19F0:248D
		if (vv == -1) {
			//^19F0:2493
			vv = glbCurrentThinkingCreatureRec->b15_0_1();
		}
		//^19F0:24A1
		if (bp0a != 0 || _19f0_0559(vv) == 0) {
			//^19F0:24B6
			glbCurrentThinkingCreatureData->b29 = U8(vv);
			if (aa == 1) {
				//^19F0:24CA
				if (bp02 == ccm19) {
					//^19F0:24D0
					bp04 = vv;
				}
				else if (bp02 == ccmPlaceMerchandise) {
					//^19F0:24DF
					di = _19f0_050f();
					si = _48ae_01af(_0cee_2df4(di), vv);
					if (si == 0)
						goto _2474;
					//^19F0:2502
					si = i16(COMPUTE_POWER_4_WITHIN(si, RAND16(_1c9a_0598(si)) +1));
					ATLASSERT(si != 0);
					for (bp04 = 0; (si & 1) == 0; si >>= 1, bp04++);
				}
				else {
					//^19F0:253B
					si = RAND01();
					if (bp0a == 0) {
						//^19F0:2548
						si += 2;
					}
					//^19F0:254A
					bp04 = (vv +si) & 3;
					bp02 = _4976_38e8[RCJ(4,si)];
				}
			}
			//^19F0:2563
			else if (bp02 == X16(ccmInv)) {
				si = (bp04 - glbCurrentThinkingCreatureData->b29 +1) & 3;
				if (bp0a == 0) {
					bp02 = (si < 2) ? ccm2B : ccm2C;
				}
				else {
					bp02 = (si < 2) ? ccm0C : ccm0D;
					//^19F0:25AB
					if (si == 0) {
						//^19F0:25AF
						glbCurrentThinkingCreatureData->b29 = (glbCurrentThinkingCreatureData->b29 -1) & 3;
					}
					else if (si == 3) {
						//^19F0:25C0
						glbCurrentThinkingCreatureData->b29 = (glbCurrentThinkingCreatureData->b29 +1) & 3;
					}
					//^19F0:25D4
					if ((tblAIStats01[_4976_4efa] & 0x1000) != 0 && glbCurrentThinkingCreatureData->b29 != glbCurrentThinkingCreatureRec->b15_0_1()) {
						//^19F0:25FB
						_19f0_0559(glbCurrentThinkingCreatureData->b29);
						return 1;
					}
				}
			}
			//^19F0:260C
			glbCurrentThinkingCreatureData->w24.SetX(glbCreatureSomeX);
			glbCurrentThinkingCreatureData->w24.SetY(glbCreatureSomeY);
			glbCurrentThinkingCreatureData->w24.SetMap(glbCreatureSomeZMap);
			//^19F0:2644
			glbCurrentThinkingCreatureData->b28 = U8(bp04);
			glbCurrentThinkingCreatureData->ItemToThrow = U8(ww);
			glbCurrentThinkingCreatureData->b32 = U8(aa);
			glbCurrentThinkingCreatureData->Command = U8(bp02);
			_4976_4ee5 = xactrAgain;
		}
	}
	//^19F0:2665
	return 1;
}

//^0CEE:0184
ObjectID *SkWinCore::OVERSEE_RECORD(ObjectID *ref, Bit8u dir, ObjectID **recordMatched, U16 (SkWinCore::*pfnFinder)(ObjectID *ref, void *pvUser), void *pvUser, Bit16u alsoCreaturePossessions, Bit16u alsoContainedObjects)
{
	//^0CEE:0184
	ENTER(8);
	//^0CEE:018A
	U16 di = alsoCreaturePossessions;
	U16 si = alsoContainedObjects;
	//^0CEE:0190
	for (ObjectID *bp08 = ref; *ref != OBJECT_END_MARKER && *ref != OBJECT_NULL; ref = &GET_ADDRESS_OF_RECORD(*ref)->w0) {
		//^0CEE:019F
		if (dir != 0xff) {
			//^0CEE:01A5
			if (ref->Dir() != dir)
				//^0CEE:01B7
				continue;
		}
		//^0CEE:01BA
		if (ref->DBType() == dbCreature && di != 0) {
			//^0CEE:01CF
			ObjectID *bp04 = &GET_ADDRESS_OF_RECORD4(*ref)->possession;
			//^0CEE:01E0
			bp04 = OVERSEE_RECORD(bp04, 255, recordMatched, pfnFinder, pvUser, di, si);
			//^0CEE:020D
			if (bp04 != NULL) {
				//^0CEE:0212
				return bp04;
			}
		}
		//^0CEE:021B
		if (si != 0 && IS_CONTAINER_CHEST(*ref) != 0) {
			//^0CEE:022F
			ObjectID *bp04 = &GET_ADDRESS_OF_RECORD(*ref)->w0;
			//^0CEE:0241
			bp04 = &bp04[1];
			//^0CEE:024F
			bp04 = OVERSEE_RECORD(bp04, 255, recordMatched, pfnFinder, pvUser, di, si);
			//^0CEE:0279
			if (bp04 != NULL) {
				//^0CEE:0281
				//^0CEE:0212
				return bp04;
			}
		}
		//^0CEE:0283
		if ((this->*pfnFinder)(ref, pvUser) != 0) {
			//^0CEE:0299
			*recordMatched = bp08;
            //^0CEE:02A9
			return ref;
		}
		//^0CEE:02B1
	}
	//^0CEE:02D5
	return NULL;
}

//^3A15:0381
int SkWinCore::_3a15_0381(Timer *xx, Timer *yy)
{
	//^3A15:0381
	if (xx->GetTick() >= yy->GetTick()) {
		//^3A15:03B8
		Bit16u si =  (xx->GetTick() == yy->GetTick()) ? 1 : 0;
		//^3A15:03EC
		if (si != 0 && (xx->TimerType() > yy->TimerType()))
			//^3A15:03FE
			return 1;
		//^3A15:0400
		if (si != 0) {
			//^3A15:0404
			si = (xx->TimerType() == yy->TimerType()) ? 1 : 0;
			//^3A15:041D
			if (si != 0) {
				//^3A15:0421
				if (xx->actor > yy->actor) {
					//^3A15:043D
					return 1;
				}
			}
		}
		//^3A15:0431
		if (si == 0)
			//^3A15:0433
			return 0;
		//^3A15:0435
		//if ((Bit16u)xx <= (Bit16u)yy) // loose conversion
		if ((void*) xx <= (void*) yy)
			//^3A15:043D
			return 1;
		//^3A15:043B
		return 0;
	}
	//^3A15:043D
	return 1;
}

//^3A15:0486
void SkWinCore::_3a15_0486(Bit16u xx)
{
	//^3A15:0486
	//^3A15:048C
	Bit16u di = xx;
	_4976_4762 = -1;
	Bit16u bp06 = glbTimersCount -1;
	//^3A15:049C
	if (bp06 == 0)
		//^3A15:04A0
		return;
	//^3A15:04A3
	Bit16u bp0a = glbTimerNextEntries[di];
	//^3A15:04B3
	Timer *bp04 = &glbTimersTable[bp0a];
	//^3A15:04C8
	Bit16u bp08 = 0;
	//^3A15:04CD
	for (; di > 0; ) {
		//^3A15:04CF
		Bit16u si = (di -1) >> 1;
		//^3A15:04D6
		if (_3a15_0381(bp04, &glbTimersTable[glbTimerNextEntries[si]]) == 0)
			//^3A15:0500
			break;
		//^3A15:0502
		glbTimerNextEntries[di] = glbTimerNextEntries[si];
		//^3A15:051C
		di = si;
		//^3A15:051E
		bp08 = 1;
		//^3A15:0523
	}
	//^3A15:0527
	if (bp08 == 0) {
		//^3A15:0530
		bp06 = (bp06 -1) >> 1;
		//^3A15:0539
		while (di <= bp06) {
			//^3A15:053C
			Bit16u si = (di << 1) +1;
			//^3A15:0544
			if ((si +1) < glbTimersCount) {
				//^3A15:054A
				if (_3a15_0381(&glbTimersTable[glbTimerNextEntries[si +1]], &glbTimersTable[glbTimerNextEntries[si]]) != 0) {
					//^3A15:0590
					si++;
				}
			}
			//^3A15:0591
			if (_3a15_0381(&glbTimersTable[glbTimerNextEntries[si]], bp04) == 0)
				//^3A15:05BD
				break;
			//^3A15:05BF
			glbTimerNextEntries[di] = glbTimerNextEntries[si];
			//^3A15:05D9
			di = si;
			//^3A15:05DB
		}
	}
	//^3A15:05E3
	glbTimerNextEntries[di] = bp0a;
	//^3A15:05F3
	return;
}



//^2FCF:1587
void SkWinCore::INVOKE_MESSAGE(i16 xpos, i16 ypos, Bit16u dir, Bit16u actionType, Bit32u tick)
{
	SkD((DLV_DBM, "DBM: INVOKE_MESSAGE(%2d,%2d,%d,%d,%6u) M:%2d\n"
		, (Bitu)xpos, (Bitu)ypos, (Bitu)dir, (Bitu)actionType, (Bitu)tick, (Bitu)glbCurrentMapIndex));

	//^2FCF:1587
	//^2FCF:158B
	Timer bp0a;
	bp0a.SetMap(glbCurrentMapIndex);
	bp0a.SetTick(tick);
	//^2FCF:15A6
	bp0a.TimerType(tty04);
	//^2FCF:15AA
	if (actionType != 0) {
		//^2FCF:15B1
		if (actionType != 1) {
			//^2FCF:15B6
			if (actionType == 2) {
				//^2FCF:15C3
				// SPX: Why not directly if actionType == 2 then actor = 2 ?
				bp0a.actor = TIMER_ACTOR__02;
			}
		}
		else {	// Then should be >= 3, but actually never goes there ?!
			//^2FCF:15BD
			bp0a.actor = TIMER_ACTOR__03;
		}
	}
	else { // Then = 1 ...
		//^2FCF:15C9
		bp0a.actor = TIMER_ACTOR__01;
	}
	// And no other else ? what happens if = 0 ?
	//^2FCF:15CD
	bp0a.Xcoord((Bit8u)xpos);
	bp0a.Ycoord((Bit8u)ypos);
	bp0a.Value2((Bit8u)dir);
	bp0a.ActionType((Bit8u)actionType);
	//^2FCF:15E5
	QUEUE_TIMER(&bp0a);
}



//^2FCF:15F3
void SkWinCore::INVOKE_ACTUATOR(Actuator *ref, Bit16u actionType, i16 delayPlus)
{
	SkD((DLV_DBM, "DBM: INVOKE_ACTUATOR(%4X,%2d,%d)\n"
		, ref, (Bitu)actionType, (Bitu)delayPlus));

	//^2FCF:15F3
	//^2FCF:15F6
	INVOKE_MESSAGE(
		ref->Xcoord(),
		ref->Ycoord(),
		ref->Direction(),
		actionType,
        glbGameTick + ref->Delay() + delayPlus
		);
	//^2FCF:164C
	return;
}

//^1C9A:0694
U16 SkWinCore::_1c9a_0694(ObjectID *ref, void *pv)
{
	//^1C9A:0694
	ENTER(0);
	//^1C9A:0697
	return (*ref == OBJECT_END_MARKER || GET_DISTINCTIVE_ITEMTYPE(*ref) == *(U16 *)pv) ? 1 : 0;
}

//^1C9A:06BD
ObjectID *SkWinCore::_1c9a_06bd(ObjectID recordLink, Bit16u ss, Bit16u dir)
{
	//^1C9A:06BD
	//^1C9A:06C1
	ObjectID *bp04 = 0;
	//^1C9A:06CB
	if (recordLink != OBJECT_NULL) {
		//^1C9A:06D1
		Creature *creature = GET_ADDRESS_OF_RECORD4(recordLink);	//*bp08
		//^1C9A:06E0
		ObjectID *bp0c;
		bp04 = OVERSEE_RECORD(
			&creature->possession,
			(Bit8u)dir,
			&bp0c,
			&SkWinCore::_1c9a_0694,
			&ss,
			0,
			0
			);
		//^1C9A:070F
		if (bp04 != NULL && *bp04 == 0xFFFE)
			//^1C9A:0720
			bp04 = NULL;
	}
	//^1C9A:072A
	return bp04;
}



//^2FCF:00B2
ObjectID SkWinCore::ROTATE_RECORD_BY_TELEPORTER(Teleporter *ref, ObjectID recordLink)
{
	//^2FCF:00B2
	//^2FCF:00B7
	ObjectID cx = recordLink;
	//^2FCF:00BA
	Bit16u si = _4976_581e;
	//^2FCF:00BE
	Bit16u di = ref->Rotation();
	//^2FCF:00CD
	if (ref->RotationType() != 0) {
		//^2FCF:00DB
		si = di;
	}
	else {
		//^2FCF:00DF
		si = (si + di) & 3;
		//^2FCF:00E8
		cx.Dir(cx.Dir() + di);
	}
	//^2FCF:00FF
	_4976_581e = si;
	//^2FCF:0103
	return cx;
}

//^2FCF:0009
void SkWinCore::ROTATE_CREATURE(ObjectID recordLink, Bit16u rotationType, Bit16u rotation)
{
	SkD((DLV_DBM, "DBM: ROTATE_CREATURE(%04X,%d,%d)\n"
		, (Bitu)recordLink.w, (Bitu)rotationType, (Bitu)rotation));

	//^2FCF:0009
	//^2FCF:000F
	Creature *creature = GET_ADDRESS_OF_RECORD4(recordLink);	//*bp08
	//^2FCF:001E
	Bit16u di = creature->b15_0_1();
	//^2FCF:002A
	Bit16u si;
	if (rotationType != 0) {
		//^2FCF:0030
		si = rotation;
	}
	else {
		//^2FCF:0035
		si = (di + rotation) & 3;
	}
	//^2FCF:003F
	Bit16u bp0a = (si - di) & 3;
	//^2FCF:0049
	creature->b15_0_1(si);
	//^2FCF:005A
	if ((QUERY_CREATURE_AI_SPEC_FLAGS(recordLink) & 0x0001) != 0) {
		//^2FCF:0068
		ObjectID *bp04 = &creature->possession;
		//^2FCF:0076
		while (*bp04 != OBJECT_END_MARKER) {
			//^2FCF:0078
			bp04->Dir(bp04->Dir() + bp0a);
			//^2FCF:0096
			bp04 = &(GET_ADDRESS_OF_RECORD(*bp04)->w0);
			//^2FCF:00A5
		}
	}
	//^2FCF:00AE
	return;
}

//^2C1D:01DD
void SkWinCore::ROTATE_SQUAD(Bit16u dir)
{
	//^2C1D:01DD
	//^2C1D:01E3
	Bit16u di = dir;
	//^2C1D:01E6
	if (di != glbPlayerDir) {
		//^2C1D:01EC
		i16 dx = di - glbPlayerDir;
		//^2C1D:01F4
		if (dx < 0)
			//^2C1D:01F8
			dx += 4;
		//^2C1D:01FB
		Champion *bp04 = glbChampionSquad;
		//^2C1D:0203
		for (Bit16u si=0; si < glbChampionsCount; bp04++, si++) {
			//^2C1D:0207
			bp04->playerPos(bp04->playerPos() + (Bit8u)dx);
			//^2C1D:0216
			bp04->playerDir(bp04->playerDir() + (Bit8u)dx);
			//^2C1D:0222
		}
		//^2C1D:022E
		glbPlayerDir = di;
		//^2C1D:0232
		if (_4976_4c08 != 0) {
			//^2C1D:0239
			_4976_4c2c = _4976_4c10 = (_4976_4c10 + dx) & 3;
		}
		else {
			//^2C1D:0246
			_4976_4c2c = glbPlayerDir;
		}
	}
	//^2C1D:024C
	return;
}

//^2FCF:0434
// TODO related to teleporter and falling into pits
Bit16u SkWinCore::_2fcf_0434(ObjectID recordLink, i16 xpos, i16 ypos, i16 xx, i16 yy, Bit16u zz)
{
	//^2FCF:0434
	//^2FCF:043A
	ObjectID si = recordLink;
	_4976_5822 = 0;
	Bit16u bp14 = 0; // 1 if telefragged?
	Bit16u bp12 = 0;
	Bit16u bp20 = glbCurrentMapIndex; // curmap
	Bit16u di = glbCurrentMapIndex; // curmap
	//^2FCF:0452
	Bit16u bp28;
	Bit16u bp1e;
	Bit16u bp26;
	Bit16u bp24; // teleporter scope
	if (si == OBJECT_NULL) {
		//^2FCF:0457
		if (_4976_4ddc != 0) {
			//^2FCF:045E
			_4976_4ddc = 0;
			_29ee_000f();
		}
		//^2FCF:0469
		glbPlayerPosX = xx;
		glbPlayerPosY = yy;
		bp24 = 2;
		//^2FCF:047A
		bp26 = (glbChampionInventory == 0 && glbIsPlayerSleeping == 0) ? 1 : 0;
		bp1e = 0xffff;
		bp28 = 0;
	}
	else {
		//^2FCF:049E
		bp28 = IS_OBJECT_FLOATING(si);
		bp1e = si.DBType();
		bp24 = 3;
		//^2FCF:04B7
		if (bp1e == dbCreature) {
			//^2FCF:04BD
			bp24 = (QUERY_GDAT_DBSPEC_WORD_VALUE(si, 0x1e) != 0) ? 2 : 1;
		}
		//^2FCF:04D9
		else if (bp1e == dbMissile) {
			//^2FCF:04DF
			Missile *bp04 = GET_ADDRESS_OF_RECORD(si)->castToMissile();
			//^2FCF:04EC
			_4976_581e = glbTimersTable[bp04->TimerIndex()].Direction();
		}
		//^2FCF:050B
		bp26 = 0;
	}
	//^2FCF:0510
	for (Bit16u bp1a = 50; bp1a != 0; --bp1a) {
		//^2FCF:0518
		Bit16u bp16 = glbCurrentTileMap[xx][yy];
		//^2FCF:0532
		Bit16u bp18 = bp16 >> 5;	
		//^2FCF:053A
		if (bp18 == ttTeleporter) {
			//^2FCF:0542
			if ((bp16 & 0x0008) == 0)
				//^2FCF:0549
				break;
			//^2FCF:054C
			Teleporter *bp04 = GET_ADDRESS_OF_TILE_RECORD((Bit8u)xx, (Bit8u)yy)->castToTeleporter();
			//^2FCF:055F
			if (bp04->Scope() == scopeCreatures && bp1e != dbCreature)
				//^2FCF:0577
				break;
			//^2FCF:057A
			if (bp24 != 3) {
				//^2FCF:0580
				if ((bp24 & bp04->Scope()) == 0)
					//^2FCF:0592
					break;
			}
			//^2FCF:0595
			bp14 = (bp04->DestinationX() == xx && bp04->DestinationY() == yy && bp04->DestinationMap() == di) ? 1 : 0;
			//^2FCF:05C8
			xx = bp04->DestinationX();
			yy = bp04->DestinationY();
			_4976_5822 = bp04->Sound();
			di = bp04->DestinationMap();
			//^2FCF:05F8
			CHANGE_CURRENT_MAP_TO(di);
			//^2FCF:05FF
			if (si == OBJECT_NULL) {
				//^2FCF:0604
				glbPlayerPosX = xx;
				glbPlayerPosY = yy;
				//^2FCF:0610
				if (bp04->Sound() != 0) {
					//^2FCF:0621
					// SPX: TODO More in the spirit, it should call the 0x18 (teleporter) category instead of 0x03 (messages),
					// and use the index of teleporter (is it possible?)
					//if (!SkCodeParam::bUseDM2ExtendedMode)
						QUEUE_NOISE_GEN1(GDAT_CATEGORY_MESSAGES,0x00,SOUND_STD_TELEPORT_MESSAGE,0x61,0x80,glbPlayerPosX,glbPlayerPosY,-1);
					//else
					//	QUEUE_NOISE_GEN1(GDAT_CATEGORY_TELEPORTERS,0x00,SOUND_STD_TELEPORT,0x61,0x80,glbPlayerPosX,glbPlayerPosY,-1);
				}
				//^2FCF:063F
				if (bp20 == di) {
					//^2FCF:0644
					bp26 &= _4976_5bee[0];
				}
				//^2FCF:064C
				ROTATE_SQUAD((bp04->Rotation() + ((bp04->RotationType() != 0) ? 0 : glbPlayerDir)) & 3);
			}
			//^2FCF:0680
			else if (bp1e == dbCreature && zz != 00) {
				//^2FCF:068C
				if (bp04->Sound() != 0) {
					//^2FCF:069D
					QUEUE_NOISE_GEN1(GDAT_CATEGORY_MESSAGES,0x00,SOUND_STD_TELEPORT_MESSAGE,0x61,0x80,xx,yy,0x01);
				}
				//^2FCF:06B9
				ROTATE_CREATURE(si, bp04->RotationType(), bp04->Rotation());
			}
			//^2FCF:06DC
			else if (bp1e == dbMissile) {
				//^2FCF:06E2
				si = ROTATE_RECORD_BY_TELEPORTER(bp04, si);
			}
			//^2FCF:06F4
			else if (bp04->RotationType() == 0 && xpos != -2) {
				//^2FCF:070B
				si.Dir(si.Dir() + bp04->Rotation());
			}
			//^2FCF:072D
			if (bp14 == 0)
				//^2FCF:0733
				continue;
			//^2FCF:0736
			break;
		}
		//^2FCF:0739
		// SPX: Add special switchable parameter to not fall into pits
		if (bp18 == ttPit && bp28 == 0 && (bp16 & 8) != 0 && (bp16 & 1) == 0 && !SkCodeParam::bWalkOverPits) {
			//^2FCF:075F
			// SPX: Retrieving 1 (only for VOID set) tells to do special treatment when falling into a pit
			Bit16u bp2a = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, dunMapLocalHeader->MapGraphicsStyle(), dtWordValue, GDAT_GFXSET_VOID_RANDOM_FALL);
			//^2FCF:077F
			if (bp2a != 0 && bp1e == dbCreature) {
				//^2FCF:078B
				Creature *bp10 = GET_ADDRESS_OF_RECORD(si)->castToCreature();
				//^2FCF:0798
				xx = bp10->TriggerX();
				yy = bp10->TriggerY();
				di = bp10->TriggerMap();
				//^2FCF:07BE
				CHANGE_CURRENT_MAP_TO(di);
				//^2FCF:07C5
				continue;
			}
			//^2FCF:07C8
			if (bp26 != 0 && bp2a == 0 && _4976_5824 == 0) {
				//^2FCF:07DB
				bp26 = _4976_5bee[0];
				//^2FCF:07E3
				if (bp12 != 0) {
					//^2FCF:07E9
					LOAD_LOCALLEVEL_GRAPHICS_TABLE(di);
					//^2FCF:07F0
					LOAD_LOCALLEVEL_DYN();
				}
				//^2FCF:07F5
				DISPLAY_VIEWPORT(glbPlayerDir, xx, yy);
				//^2FCF:0807
				CHANGE_VIEWPORT_TO_INVENTORY(1);
			}
			//^2FCF:080F
			if (bp2a != 0) {
				//^2FCF:0818
				Bit16u bp2e = 0;
				//^2FCF:081D
				ObjectID bp2c = GET_TILE_RECORD_LINK(xx, yy);
				//^2FCF:082A
				for (; bp2c != OBJECT_END_MARKER && ((bp1e = bp2c.DBType()) <= dbActuator); bp2c = GET_NEXT_RECORD_LINK(bp2c)) {
					//^2FCF:082C
					if (bp1e == dbText) {
						//^2FCF:0832
						Text *bp0c = GET_ADDRESS_OF_RECORD(bp2c)->castToText();
						//^2FCF:0841
						if (bp0c->TextMode() == 1) {
							//^2FCF:0852
							if (bp0c->SimpleTextExtUsage() == 0x0c) {
								//^2FCF:0867
								bp2e = bp0c->TextIndex() & 0xff;
								//^2FCF:0877
								break;
							}
						}
					}
					//^2FCF:0879
				}
				//^2FCF:089C
				// SPX: 1) Counts how many "special markers" for valid destinations when falling from VOID.
				Bit16u bp30 = SEARCH_DUNGEON_FOR_SPECIAL_MARKER(SDFSM_CMD_COUNT_MARKERS, 0x0000, bp2e, &xx, &yy);
				SkD((DLV_FSM, "Found %d special markers.\n", bp30));
				//^2FCF:08B8
				// SPX: 2) Search one of this "special markers" with a random on the count
				di = SEARCH_DUNGEON_FOR_SPECIAL_MARKER(SDFSM_CMD_FIND_MAP_OFFVOID, RAND16(bp30), bp2e, &xx, &yy);
			}
			else {
				//^2FCF:08DC
				di = LOCATE_OTHER_LEVEL(di, 1, &xx, &yy, NULL);
			}
			//^2FCF:08FA
			CHANGE_CURRENT_MAP_TO(di);
			//^2FCF:0901
			if (si == OBJECT_NULL) {
				//^2FCF:0909
				glbPlayerPosX = xx;
				glbPlayerPosY = yy;
				//^2FCF:0915
				if (glbChampionsCount > 0) {
					//^2FCF:091F
					Champion *bp08;
					if (_4976_5824 != 0) {
						//^2FCF:0926
						bp26 = (_4976_5bee[0] != 0 && glbChampionInventory == 0 && glbIsPlayerSleeping == 0) ? 1 : 0;
						//^2FCF:0945
						bp08 = glbChampionSquad;
						//^2FCF:094D
						for (bp18=0; bp18 < glbChampionsCount; bp08++, bp18++) {
							//^2FCF:0954
							if (bp08->curHP() != 0) {
								//^2FCF:095E
								ADJUST_STAMINA(
									bp18,
									((GET_PLAYER_WEIGHT(bp18) * 25) / MAX_LOAD(bp08)) + 1
									);
							}
							//^2FCF:098D
						}
					}
					else {
						//^2FCF:09A1
						bp08 = glbChampionSquad;
						//^2FCF:09A9
						for (bp18 = 0; (bp18 < glbChampionsCount); bp08++, bp18++) {
							//^2FCF:09B0
							if (bp08->curHP() != 0) {
								//^2FCF:09BA
								WOUND_PLAYER(
									bp18, 
									min_value(bp08->maxHP() >> 2, 17) + RAND02(),
									0x0030,
									0x0002
									);
								//^2FCF:09E9
								// SPX: Noise when falling a pit
								QUEUE_NOISE_GEN2(
									GDAT_CATEGORY_CHAMPIONS,
									bp08->HeroType(),
									SOUND_CHAMPION_SCREAM,
									0xfe,
									glbPlayerPosX,
									glbPlayerPosY,
									0xffff,
									0x00be,
									0x00ff
									);
							}
							//^2FCF:0A12
						}
					}
				}
				//^2FCF:0A23
				_4976_5824 = 0;
				//^2FCF:0A29
				continue;
			}
			//^2FCF:0A2C
			if (bp1e != dbCreature)
				//^2FCF:0A32
				continue;
			//^2FCF:0A35
			if (zz == 0)
				//^2FCF:0A3B
				continue;
			//^2FCF:0A3E
			if (xpos >= 0) {
				//^2FCF:0A44
				CHANGE_CURRENT_MAP_TO(bp20);
				//^2FCF:0A4D
				ATTACK_CREATURE(
					si,
					xpos,
					ypos,
					0x0000,
					0x0000,
					0x0014
					);
			}
			//^2FCF:0A62
			CHANGE_CURRENT_MAP_TO(di);
			//^2FCF:0A69
			continue;
		}
		//^2FCF:0A6C
		if (bp18 != ttStairs || si == OBJECT_NULL || bp1e == 14 || bp1e == 4)
			//^2FCF:0A8C
			break;
		//^2FCF:0A8F
		if ((bp16 & 0x0004) == 0) {
			//^2FCF:0A96
			di = LOCATE_OTHER_LEVEL(di, 1, &xx, &yy, NULL);
			//^2FCF:0AB1
			CHANGE_CURRENT_MAP_TO(di);
		}
		//^2FCF:0AB8
		Bit16u bp22 = _0cee_06dc_GET_TILE_DIRECTION(xx, yy);
		//^2FCF:0AC8
		xx += glbXAxisDelta[bp22];
		//^2FCF:0AD4
		yy += glbYAxisDelta[bp22];
		//^2FCF:0AE0
		bp22 = (bp22 +2 ) & 3;
		//^2FCF:0AEB
		Bit16u bp1c = si.Dir();
		//^2FCF:0AF3
		bp1c = ((((bp1c - bp22 + 1) & 2) >> 1) + bp22) & 3;
		//^2FCF:0B05
		si.Dir(bp1c);

		//^2FCF:0B12
	}
	//^2FCF:0B1F
	_4976_5826 = xx;
	_4976_5828 = yy;
	_4976_581c = di;
	//^2FCF:0B2F
	_4976_5820 = si.Dir();
	//^2FCF:0B37
	if (bp12 != 0 && zz != 0 && si != OBJECT_NULL) {
		//^2FCF:0B48
		// SPX: Sound when an item hit an obstacle
		QUEUE_NOISE_GEN2(
			QUERY_CLS1_FROM_RECORD(si),
			QUERY_CLS2_FROM_RECORD(si),
			SOUND_STD_KNOCK,
			0xfe,
			xx,
			yy,
			0x0001,
			0x003a,
			0x0080
			);
	}
	//^2FCF:0B73
	CHANGE_CURRENT_MAP_TO(bp20);
	//^2FCF:0B7C
	_482b_05bf(1);
	//^2FCF:0B84
	return bp28;
}

//^1C9A:03CF
// TODO related creature size ? b35
ObjectID SkWinCore::_1c9a_03cf(i16 *xx, i16 *yy, Bit16u dir)
{
	//^1C9A:03CF
	//^1C9A:03D5
	i16 bp0c = *xx;
	i16 di = *yy;
	Bit16u bp10;
	Bit16u bp12;
	//^1C9A:03E4
	_098d_000f(
		bp0c, 
		di, 
		(dir == 255) ? 12 : (_4976_014c[RCJ(4,dir)]),
		&bp10,
		&bp12
		);
	//^1C9A:0410
	i16 bp0e = 4;
	//^1C9A:0415
	for (i16 si=0; si <= bp0e; si++) {
		//^1C9A:041A
		ObjectID bp06 = GET_CREATURE_AT(bp0c, di);
		//^1C9A:0427
		if (bp06 != OBJECT_NULL) {
			//^1C9A:042F
			Creature *bp04 = GET_ADDRESS_OF_RECORD(bp06)->castToCreature();
			//^1C9A:043E
			i16 bp08 = QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp04->CreatureType())->b35;
			//^1C9A:0458
			Bit16u bp14;
			Bit16u bp16;
			_098d_000f(
				bp0c,
				di,
				QUERY_CREATURE_5x5_POS(
					bp04,
					_4976_392c[RCJ(4,bp04->b15_0_1())]
					),
				&bp14,
				&bp16
				);
			//^1C9A:048F
			bp14 -= bp10;
			bp16 -= bp12;
			i16 bp0a = (bp14 * bp14) + (bp16 * bp16);
			//^1C9A:04AE
			if (_4976_3924[RCJ(4,bp08)] > bp0a) {
				//^1C9A:04BC
				*xx = bp0c;
				*yy = di;
				//^1C9A:04CB
				return bp06;
			}
		}
		//^1C9A:04D0
		if (dir == 255) {
#if UseAltic
			//^1C9A:04D7
			bp0c = *xx + _4976_38f4[8 +si].w0;
			//^1C9A:04EE
			di = *yy + _4976_38f4[8 +si].w2;
#else
			//^1C9A:04D7
			bp0c = *xx + _4976_3914[RCJ(4,si)].w0;
			//^1C9A:04EE
			di = *yy + _4976_3914[RCJ(4,si)].w2;
#endif
		}
		else {
			//^1C9A:0504
			bp0c = *xx + _4976_38f4[RCJ(12,(dir << 1) + si)].w0;
			//^1C9A:0523
			di = *yy + _4976_38f4[RCJ(12,(dir << 1) + si)].w2;
		}
		//^1C9A:0541
	}
	//^1C9A:054A
	return OBJECT_NULL;
}

//^0CD5:0078
i16 SkWinCore::ABS16(i16 val)
{
	//^0CD5:0078
	//^0CD5:007C
	i16 si = val;
	//^0CD5:007F
	if (si < 0) {
		//^0CD5:0083
		return -si;
	}
	//^0CD5:0089
	return si;
}

//^075F:1B61
Bit16u SkWinCore::CALC_VECTOR_DIR(i16 x1, i16 y1, i16 x2, i16 y2)
{
	// calc normal vector of: v2 - v1. v2=(x2,y2), v1=(x1,y1)
	//
	// return: 0 if v2 is at north from v1
	//         1 if v2 is at east from v1
	//         2 if v2 is at south from v1
	//         3 if v2 is at west from v1

	//^075F:1B61
	//^075F:1B66
	i16 di = x2;
	i16 si = y2;
	x1 -= di;
	di = ABS16(x1);
	//^075F:1B7B
	y1 -= si;
	si = ABS16(y1);
	//^075F:1B8B
	if (di == si) {
		//^075F:1B90
		if (RAND01() != 0) {
			//^075F:1B99
			di++;
		}
		else {
			//^075F:1B9C
			si++;
		}
	}
	//^075F:1B9D
	if (di < si) {
		//^075F:1BA1
		if (y1 > 0)
			//^075F:1BA7
			return 0;

		//^075F:1BAB
		return 2;
	}
	//^075F:1BB0
	if (x1 > 0) {
		//^075F:1BB6
		return 3;
	}
	//^075F:1BBB
	return 1;
}

//^075F:1B32
Bit16u SkWinCore::CALC_SQUARE_DISTANCE(i16 x1, i16 y1, i16 x2, i16 y2)
{
	// CSBwinSimilarity: TAG00f4c8,DistanceBetweenPoints

	//^075F:1B32
	//^075F:1B37
	i16 di = x1;
	i16 si = y1;
	//^075F:1B3D
	di -= x2;
	si -= y2;

	return ((di < 0) ? (-di) : di) + ((si < 0) ? (-si) : si);
}

//^4937:005C
// TODO: creature animation related ?
Bit16u SkWinCore::_4937_005c(Bit16u xx, Bit16u *yy)
{
	//^4937:005C
	//^4937:0061
	Bit16u si;
	if ((*yy & 0x4000) != 0) {
		//^4937:006B
		si = 0;
	}
	//^4937:006F
	else if ((*yy & 0x8000) != 0) {
		//^4937:0079
		Bit16u di;
		if ((*yy & 0x1000) != 0) {
			//^4937:0080
			*yy &= 0xe03f;
			di = 0;
		}
		else {
			//^4937:0089
			di = (*yy & 0x0fc0) >> 6;
		}
		//^4937:0097
		// SPX: TODO / I got this fall under divide by zero when replacing static AI table
		// by a new table. I don't understand why this happens, then I use some fix
		if (SkCodeParam::bUseFixedMode)
		{
			if ( (*yy & 0x003F) == 0)
				*yy = 1;
		}
		// SPX: What is this 0x3F (63) value? is it related to number of AI ??
		si = (glbGameTick + di) % (*yy & 0x003f);
	}
	else {
		//^4937:00B8
		si = *yy & 0x003f;
	}
	//^4937:00C3
	return xx + si;
}

//^4937:0036
// TODO: creature animation related, get some sequence
CreatureAnimationFrame* SkWinCore::_4937_0036(Bit16u xx, Bit16u *yy)
{
	//^4937:0036
	//^4937:0039
	return &tlbCreaturesAnimationSequences[_4937_005c(xx, yy)];
}

void SkWinCore::PROCESS_QUEUED_DEALLOC_RECORD() {
	// kkdf2: ext for skwinspx
	while (!vecDelayDeallocRecords.empty()) {
		ObjectID recordLink = vecDelayDeallocRecords.top();
		DEALLOC_RECORD(recordLink);
		vecDelayDeallocRecords.pop();
	}
}

void SkWinCore::QUEUE_DEALLOC_RECORD(ObjectID recordLink) {
	// kkdf2: ext for skwinspx: it prevents you from watching many asserts when you put coin into your money box!
	vecDelayDeallocRecords.push(recordLink);
}

//^0CEE:1585
void SkWinCore::DEALLOC_RECORD(ObjectID recordLink)
{
	// it is a unused record if next object is FFFF.

	//^0CEE:1585
	//^0CEE:1588
	SkD((DLV_DBM,"DBM: DEALLOC_RECORD(%04X)\n", (Bitu)recordLink.w));
	GET_ADDRESS_OF_RECORD(recordLink)->w0 = OBJECT_NULL;
}

//^075F:06BD
// TODO: related to missile/item attack strength ?
Bit16u SkWinCore::_075f_06bd(Missile *ref, ObjectID recordLink) //#DS=4976?
{
	//^075F:06BD
	//^075F:06C3
	glbPoisonAttackDamage = 0;
	_4976_4b7a = 3;
	//^075F:06CF
	Bit16u di = ref->EnergyRemaining();
	//^075F:06DA
	Bit16u iAttackDamage = 0; // si
	if (recordLink.DBType() != dbCloud) {
		//^075F:06EB
		iAttackDamage = QUERY_GDAT_DBSPEC_WORD_VALUE(recordLink, GDAT_ITEM_WEAPON_THROW_STRENGTH);	// 0x09
		//^075F:06F9
		if (iAttackDamage != 0) {
			//^075F:06FD
			iAttackDamage += (di >> 1);
			//^075F:0703
			Bit16u bp02 = ref->b5_4_7() +3;
			//^075F:0715
			iAttackDamage = (bp02 * bp02 * iAttackDamage) >> 7;
			//^075F:071F
			_4976_4b7a = 4;
			//^075F:0725
			glbPoisonAttackDamage = QUERY_GDAT_DBSPEC_WORD_VALUE(recordLink, GDAT_ITEM_STATS_POISONOUS);	// 0x0D
			//^075F:0734
			if (glbPoisonAttackDamage != 0) {
				//^075F:0738
				if ((RAND() & 0x007f) > di) {
					//^075F:0744
					glbPoisonAttackDamage -= RAND16((glbPoisonAttackDamage >> 1) + 1);
				}
			}
		}
		//^075F:0755
		iAttackDamage += RAND02();
		//^075F:075C
		iAttackDamage += QUERY_ITEM_WEIGHT(recordLink);
		//^075F:0767
		if ((RAND() & 0x01ff) < ref->EnergyRemaining2()) {
			//^075F:077C
			iAttackDamage <<= 1;
		}
	}
	else {
		//^075F:0780
		// TODO: May check CSBWin for comparison
		if (recordLink == OBJECT_EFFECT_POISON_BLOB) {	// oFF81
			//^075F:0786
			iAttackDamage = RAND() & 0x000f;
			//^075F:0790
			glbPoisonAttackDamage = iAttackDamage + 10;
			//^075F:0796
			iAttackDamage += RAND() & 0x001f;
		}
		else {
			//^075F:07A2
			// SPX: TODO may need to reorganize this block
			if (recordLink >= OBJECT_EFFECT_DISPELL) {	// oFF83
				//^075F:07A8
				_4976_4b7a = 5;
				//^075F:07AE
				if (recordLink == OBJECT_EFFECT_POISON_BOLT) {	// oFF86
					//^075F:07B4
					glbPoisonAttackDamage = di >> 1;
					//^075F:07BB
					return (di >> 4) + 1;
				}
				//^075F:07C4
				return 0;
			}
			//^075F:07C9
			_4976_4b7a = 1;
			//^075F:07CF
			iAttackDamage = (RAND() & 0x000f) + (RAND() & 0x000f) + 10;
			//^075F:07E8
			if (recordLink == OBJECT_EFFECT_LIGHTNING) {	// oFF82
				//^075F:07EE
				_4976_4b7a = 7;
				//^075F:07F4
				iAttackDamage >>= 4;
				iAttackDamage += di;
			}
		}
	}
	//^075F:07F9
	iAttackDamage = ((iAttackDamage + di) >> 4) + 1;
	//^075F:0803
	iAttackDamage += RAND16((iAttackDamage >> 1) + 1) + RAND02();
	//^075F:0818
	iAttackDamage = max_value(iAttackDamage, iAttackDamage - 32 - ((ref->EnergyRemaining2() >> 3) << 1));
	//^075F:083A
	iAttackDamage = min_value(iAttackDamage, di << 1);
	//^075F:0847
	return iAttackDamage;
}

//^1C9A:0958
// TODO: get some info of creature anim ?
Bit16u SkWinCore::_1c9a_0958(ObjectID recordLink)
{
	//^1C9A:0958
	//^1C9A:095C
	Creature *creature = GET_ADDRESS_OF_RECORD(recordLink)->castToCreature();	//*bp04
	//^1C9A:096B
	sk1c9a02c3 *bp08 = _1c9a_02c3(creature, QUERY_CREATURE_AI_SPEC_FROM_TYPE(creature->CreatureType()));
	//^1C9A:098E
	CreatureAnimationFrame *bp0c = _4937_0036(bp08->w0, &bp08->w2);
	//^1C9A:09AB
	return (bp0c->w0 & 0x4000) >> 14;
}

//^0CD5:0118
// SPX: Random over 1 bits => range 0 to 1
Bit16u SkWinCore::_RAND01()
{
	// CSBwinSimilarity: TAG001050,STRandomBool

	//^0CD5:0118
	//^0CD5:011B
	glbRandomSeed = glbRandomSeed * 0xbb40e62d + 11;
	//^0CD5:013B
	return (glbRandomSeed >> 8) & 1;
	//^0CD5:0146
}

//^0CEE:328C
// SPX: _0cee_328c renamed GET_DOOR_STAT_0X10
Bit16u SkWinCore::GET_DOOR_STAT_0X10(Bit8u iDoorType) // Bit8u cls2
{
	//^0CEE:328C
	//^0CEE:328F
	// TODO what is 10 ??
	return QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorType, dtWordValue, GDAT_DOOR_X10); // 0x10
}

//^0CEE:322D
Bit8u SkWinCore::GET_GRAPHICS_FOR_DOOR(Door *ref)
{
	//^0CEE:322D
	//^0CEE:3230
	if (ref->DoorType() == 0) {
		//^0CEE:323E
		if (dunMapLocalHeader->b2_7_7() == 0)
			//^0CEE:3271
			return 0xff;
		//^0CEE:3254
		return dunMapLocalHeader->DoorType0();
	}
	else {
		//^0CEE:3259
		if (dunMapLocalHeader->b3_0_0() == 0)
			//^0CEE:3271
			return 0xff;
		//^0CEE:3268
		return dunMapLocalHeader->DoorType1();
	}
}

//^075F:0AF9
// SPX: Something to do when an item (missile) hit an obstacle
Bit16u SkWinCore::_075f_0af9(i16 u16tileType, i16 xpos, i16 ypos, Bit16u dir, ObjectID rlMissile)
{
	//^075F:0AF9
	//^075F:0AFF
	Bit16u bp1c = 0;
	Bit16u bp2a = 0;
	//^075F:0B09
	Missile *bp08 = GET_ADDRESS_OF_RECORDE(rlMissile);
	//^075F:0B18
	ObjectID si = bp08->GetMissileObject();
	//^075F:0B21
	Bit16u bp2c = si.DBType();
	//^075F:0B2A
	Bit16u bp28;
	ObjectID bp1e;
	ObjectID bp20;
	tileTypeIndex tileType = UtilConvertU16ToTile(u16tileType);	// SPX added for safe conversion

	if (bp2c == dbPotion) {
		//^075F:0B2F
		Potion *bp04 = GET_ADDRESS_OF_RECORD(si)->castToPotion();
		//^075F:0B3C
		U8	potionType = 0;
		potionType = bp04->PotionType();

// SPX: STANDARD BEHAVIOUR, HARD CODED POTION BOMB TYPE
		if (!SkCodeParam::bUseDM2ExtendedMode)
		{
			switch (potionType)
			{
				case POTION_TYPE_VEN_BOMB: // 0x03 -> {VEN} BOMB
					//^075F:0B52
					bp20 = OBJECT_EFFECT_POISON_CLOUD;
					//^075F:0B57
					//^075F:0B5E
					bp2a = 1;
					//^075F:0B63
					bp28 = bp04->PotionPower();
					//^075F:0B70
					bp1e = si;
					break;
				case POTION_TYPE_FUL_BOMB: // 0x13 -> {FUL} BOMB
					//^075F:0B59
					bp20 = OBJECT_EFFECT_FIREBALL;
					//^075F:0B5E
					bp2a = 1;
					//^075F:0B63
					bp28 = bp04->PotionPower();
					//^075F:0B70
					bp1e = si;
					break;
			}
		}
// SPX: CUSTOM BEHAVIOUR more flexible
		else if (SkCodeParam::bUseDM2ExtendedMode)
		{
			// SPX: New way, retrieve a coded value
			bp20 = QUERY_GDAT_POTION_SPELL_TYPE_FROM_RECORD(si);
			// The first spell start at 0x80 to differenciate from 0x00 which is equals to no value
			if (bp20 != 0)
			{
				bp2a = 1;	// will explode
				bp20 = bp20 + 0xFF00;
				// Some control over
				bp20 = BETWEEN_VALUE(0xFF80, bp20, 0xFFFF);											
				bp28 = bp04->PotionPower();
				bp1e = si;
			}
			else
				bp2a = 0;	// won't explode
		}
// SPX: End of standard/custom block
	}
	//^075F:0B73
//		Bit16u bp26 = (bp2c == dbCloud && si != oFF81 && si != oFF86) ? 1 : 0;
	Bit16u bp26 = (bp2c == dbCloud && si != OBJECT_EFFECT_POISON_BLOB && si != OBJECT_EFFECT_POISON_BOLT) ? 1 : 0;
	//^075F:0B8D
	ObjectID *bp10 = NULL;
	Bit16u bp36 = 0;
	//^075F:0B9C
	i16 bp22;
	i16 bp24;
	if (xpos <= 255) {
		//^075F:0BA3
		bp22 = xpos;
		bp24 = ypos;
	}
	else {
		//^075F:0BB1
		bp22 = (xpos >> 8) -1;
		bp24 =  ypos >> 8;
		//^075F:0BC4
		xpos &= 255;
		ypos &= 255;
	}
	//^075F:0BCE
	i16 bp30 = bp22;
	i16 bp38 = bp22;
	i16 bp32 = bp24;
	i16 bp3a = bp24;
	//^075F:0BE0
	Bit16u bp16;
	switch (tileType) {
		case  ttWall:	// WALL
			{
				//^075F:0BF5
				Bit16u di = glbTimersTable[bp08->TimerIndex()].Direction();
				//^075F:0C13
				bp38 += glbXAxisDelta[di];
				//^075F:0C1E
				bp3a += glbYAxisDelta[di];
				//^075F:0C29

				SkD((DLV_TWEET, "Tweet: %s (x:%d, y:%d, map:%d) attacks the wall! \n"
					, static_cast<LPCSTR>(getRecordNameOf(rlMissile))
					, bp38 - glbXAxisDelta[di]
					, bp3a - glbYAxisDelta[di]
					, glbCurrentMapIndex
					));

				if (ATTACK_WALL(bp38, bp3a, bp22, bp24, di, rlMissile) == 0)
					break;
				//^075F:0C47
				return 1;
			}
		case  ttDoor:	// DOOR
			{
				//^075F:0C4A
				i16 bp12 = glbCurrentTileMap[xpos][ypos] & 0x07;
				//^075F:0C66
				Door *door = GET_ADDRESS_OF_TILE_RECORD((Bit8u)xpos, (Bit8u)ypos)->castToDoor();	//*bp1a
				//^075F:0C79
				if (bp12 != 5 && (si == OBJECT_EFFECT_ZO_SPELL || si == OBJECT_EFFECT_ZO_2)) {	// (si == oFF84 || si == oFF8D
					//^075F:0C89
					if (door->Button() == 0 && door->Bit13C() == 0) {
						//^075F:0C9A
						break;
					}
					//^075F:0CAB
					INVOKE_MESSAGE(
						xpos,
						ypos,
						0,
						(si == OBJECT_EFFECT_ZO_SPELL) ? 2 : 0,
						glbGameTick +1
						);
					//^075F:0CD8
					break;
				}
				//^075F:0CDB
				if (bp12 == 5 || bp12 <= 1)
					//^075F:0CDF
					//^075F:0CE5
					//^075F:0D42
					return 0;

				//^075F:0CE7
				if (GET_DOOR_STAT_0X10(GET_GRAPHICS_FOR_DOOR(door)) != 0) {
					//^075F:0CFF
					if (bp2c == dbCloud) { // bp2c == 15
						//^075F:0D05
						// SPX: that would say only FIREBALL and LIGHTNING will pass
						if (((si >= OBJECT_EFFECT_DISPELL) ? 1 : 0) != 0) {	// (si >= oFF83)
							//^075F:0D42
							return 0;
						}
					}
					//^075F:0D13
					if (((bp08->EnergyRemaining2() < (RAND() & 0x007f)) && (QUERY_GDAT_DBSPEC_WORD_VALUE(si, 0) & 2) != 0) ? 1 : 0) {
						//^075F:0D42
						return 0;
					}
				}
				//^075F:0D47
				Bit16u bp16 = _075f_06bd(bp08, si) +1;
				//^075F:0D59
				ATTACK_DOOR(xpos, ypos, bp16 + RAND16(bp16), 0, 0);
				//^075F:0D7A
				break;
			}
		case ttTeleporter://-3:	// TELEPORTER
		case ttTrickWall://-2:	// TRICKWALL
			{
				//^075F:0D7D
				i16 bp12 = GET_PLAYER_AT_POSITION(dir);
				//^075F:0D89
				if (bp12 < 0) {
					//^075F:0D8D
					if (tileType == ttTeleporter) {
						//^075F:0D93
						goto _0e37;
					}
					//^075F:0D96
					//^075F:0D42
					return 0;
				}
				//^075F:0D98
				bp36 = 1;
				//^075F:0D9D
				bp16 = _075f_06bd(bp08, si);
				//^075F:0DAE
				if (bp16 != 0) {
					//^075F:0DB2
					if ((((glbChampionSquad[bp12].playerDir() == (glbTimersTable[bp08->TimerIndex()].Direction() +2)) ? 1 : 0) & 3) != 0) {
						//^075F:0DF2
						_4976_4b7a |= 0x8000;
					}
					//^075F:0DF8
					if (WOUND_PLAYER(bp12, bp16, 0, _4976_4b7a) != 0 && glbPoisonAttackDamage != 0 && RAND01() != 0) {
						//^075F:0E20
						PROCESS_POISON(bp12, glbPoisonAttackDamage);
					}
				}
				//^075F:0E2E
				if (tileType == ttTrickWall) {
					//^075F:0E34
					break;
				}

				goto _0e37;
			}
		case ttMapExit://-1:	// EXTERN TILE
			{
				//^075F:0E37
_0e37:
				bp38 = xpos;
				bp3a = ypos;
				//^075F:0E43
				ObjectID bp2e = _1c9a_03cf(&bp38, &bp3a, dir);
				//^075F:0E5B
				if (bp2e == OBJECT_NULL) {
					//^075F:0E60
					if (tileType == ttTeleporter)
						//^075F:0E66
						break;
					//^075F:0E69
					//^075F:0D42
					return 0;
				}
				//^075F:0E6C
				if (_1c9a_0958(bp2e) != 0) {
					//^075F:0E79
					if (tileType == ttTeleporter)
						//^075F:0E7F
						break;
					//^075F:0E82
					//^075F:0D42
					return 0;
				}
				//^075F:0E85
				Creature *bp04 = GET_ADDRESS_OF_RECORD(bp2e)->castToCreature();
				//^075F:0E94
				AIDefinition *bp0c = QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp04->CreatureType());
				//^075F:0EA8
				if (bp0c->w0_5_5() != 0 && si != OBJECT_EFFECT_DISPELL) {	// oFF83
					//^075F:0EB7
					if (tileType == ttTeleporter)
						//^075F:0EBD
						break;
					//^075F:0EC0
					//^075F:0D42
					return 0;
				}
				//^075F:0EC3
				if (bp0c->IsStaticObject() == 0) {
					//^075F:0ECD
					if (bp0c->Weight != 255) {
						//^075F:0ED4
						goto _0f82;
					}
					else {
						//^075F:0ED7
						goto _0f11;
					}
				}
				//^075F:0ED9
				if ((bp0c->w30 & 0x0100) != 0 && si < OBJECT_EFFECT_FIREBALL) {	// oFF80
					//^075F:0EE7
					//^075F:0EFA
					if (tileType == ttTeleporter)
						//^075F:0F00
						break;
					//^075F:0F03
					//^075F:0D42
					return 0;
				}
				//^075F:0EE9
				if (bp0c->w0_6_7() > 1) {
					//^075F:0EFA
					if (tileType == ttTeleporter)
						//^075F:0F00
						break;
					//^075F:0F03
					//^075F:0D42
					return 0;
				}
				//^075F:0F06
				// SPX: This is turning the direction of the missile (for reflectors certainly ..)
				if ((bp0c->w30 & AI_W30_FLAGS__TURN_MISSILE) == 0) {	// 0x0800
					//^075F:0F11
_0f11:
					Bit16u di = glbTimersTable[bp08->TimerIndex()].Direction();	// get direction
					//^075F:0F2F
					di = (di +2) &3;
					//^075F:0F36
					bp30 += glbXAxisDelta[di];
					//^075F:0F41
					bp32 += glbYAxisDelta[di];
					//^075F:0F4C
					Bit16u bp34 = rlMissile.Dir();
					//^075F:0F55
					if ((di & 1) == (bp34 & 1)) {
						//^075F:0F65
						bp34--;
					}
					else {
						//^075F:0F6A
						bp34++;
					}
					//^075F:0F6D
					rlMissile.Dir(bp34);
				}
				//^075F:0F82
_0f82:
				bp16 = (_075f_06bd(bp08, si) << 6) / bp0c->ArmorClass;
				//^075F:0FA7
				if (bp16 != 0) {
					//^075F:0FAB
					ATTACK_CREATURE(
						bp2e,
						bp38,
						bp3a,
						0x200d,
						0x0064,
						((bp0c->w24 & 0x1000) != 0 && _4976_4b7a != 1) ? 0 : (bp16 + APPLY_CREATURE_POISON_RESISTANCE(bp2e, glbPoisonAttackDamage))
						);
					//^075F:0FED
					if (bp26 == 0 && bp0c->AbsorbsMissile() != 0) {	// if not a grey cloud and w0_9_9 = 1
						//^075F:0FFD
						// si is the item thrown
						if ((QUERY_GDAT_DBSPEC_WORD_VALUE(si, 0) & ITEM_FLAG_40) != 0) {	// 0x0040
							//^075F:100C
							bp10 = &bp04->possession;	// bp04 is the creature
						}
					}
				}
				//^075F:101A
				if (bp2a == 0 && bp2c != 5) {
					//^075F:1026
					if (false
						|| bp2c != 15
						|| si == OBJECT_EFFECT_ZO_SPELL
						|| (si >= OBJECT_EFFECT_PUSH && si <= OBJECT_EFFECT_REFLECTOR)	// (si >= oFF89 && si <= oFF8E)
					) {
						//^075F:1039
						//^075F:1064
						bp36 = 1;
						break;
					}
				}
				//^075F:103B
				ATTACK_CREATURE(
					bp2e,
					bp38,
					bp3a,
					2,
					(bp16 < 5 && si != OBJECT_EFFECT_POISON_CLOUD) ? 30 : 95,
					0
					);

				break;
			}
		case  ttFloor:	// FLOOR
		case  ttPit:	// PIT
		case  ttStairs:	// STAIRS
			//^075F:1069
			break;
	}
	//^075F:1069
	if (tileType == ttTeleporter && bp36 == 0) {
		//^075F:0D42
		return 0;
	}
	//^075F:1078
	if (bp26 == 0 && bp2a == 0 && si != OBJECT_EFFECT_POISON_BOLT)
		//^075F:1089
		goto _113b;

	//^075F:108C
	Bit16u bp14;
	if (bp2a != 0) {
		//^075F:1092
		bp14 = bp28;
		DEALLOC_RECORD(bp1e);
		//^075F:10A1
		bp08->SetMissileObject(si = bp20);
	}
	else {
		//^075F:10AF
		bp14 = bp08->EnergyRemaining();
		//^075F:10BB
		if (si == OBJECT_EFFECT_POISON_BOLT) {
			//^075F:10C0
			bp14 >>= 2;
			//^075F:10C4
			if (bp14 == 0)
				//^075F:10C9
				goto _113b;
		}
		//^075F:10CB
		if (si == OBJECT_EFFECT_LIGHTNING) {
			//^075F:10D0
			bp14 >>= 1;
			if (bp14 == 0)
				//^075F:10D8
				goto _113b;
		}
	}
	//^075F:10DA
	Bit16u di;
	if (si == OBJECT_EFFECT_PUSH || si == OBJECT_EFFECT_PULL) {
		//^075F:10E8
		di = glbTimersTable[bp08->TimerIndex()].Direction();
		//^075F:1106
		bp1c = bp08->EnergyRemaining();
	}
	//^075F:1112
	CREATE_CLOUD(
		si,
		bp14,
		bp22,
		bp24,
		(si == OBJECT_EFFECT_POISON_CLOUD || si == OBJECT_EFFECT_ZO_SPELL || si == OBJECT_EFFECT_ZO_2) ? 255 : dir
		);
	//^075F:113B
_113b:
	CUT_RECORD_FROM(rlMissile, NULL, bp22, bp24);
	//^075F:1150
	// SPX: I think this actually attach the missile to another object, like creature possession (= absorb)
	DELETE_MISSILE_RECORD(rlMissile, bp10, bp30, bp32);
	//^075F:1166
	if (bp1c != 0) {
		//^075F:116C
		_12b4_0d75(
			xpos,
			ypos,
			(si == OBJECT_EFFECT_PUSH) ? (di) : ((di+2) &3),
			max_value(min_value(bp1c, 0xfe), 0x28)
			);
	}
	//^075F:11A3
	return 1;
}

//^2FCF:0234
// SPX: Move PLAYER from xy to xy
void SkWinCore::_2fcf_0234(i16 xposFrom, i16 yposFrom, i16 xposTo, i16 yposTo)
{
	//^2FCF:0234
	//^2FCF:023A
	Bit16u bp04 = 0;
	//^2FCF:023F
	Bit8u bp10[4];
	ZERO_MEMORY(&bp10, 4);
	//^2FCF:0250
	Bit16u bp02;
	for (bp02=0; bp02 < 4; bp02++) {
		//^2FCF:0257
		if (GET_PLAYER_AT_POSITION(bp02) >= 0) {
			//^2FCF:0264
			bp10[bp02] = bp02 +1;
		}
		//^2FCF:0274
	}
	//^2FCF:027D
	Bit8u bp0c[4];
	if (xposTo >= 0 && CALC_SQUARE_DISTANCE(xposFrom, yposFrom, xposTo, yposTo) == 1) {
		//^2FCF:02A2
		bp02 = CALC_VECTOR_DIR(xposFrom, yposFrom, xposTo, yposTo);
		//^2FCF:02B9
		Bit16u di = (bp02 +1) & 3;
		//^2FCF:02BF
		ZERO_MEMORY(&bp0c, 4);
		//^2FCF:02D0
		if ((bp0c[(bp02 +3) &3] = bp10[bp02]) != 0) {
			//^2FCF:02F3
			bp04 = 1;
		}
		//^2FCF:02F8
		if ((bp0c[(di +1) &3] = bp10[di]) != 0) {
			//^2FCF:030E
			bp04 = 1;
		}
		//^2FCF:0313
		if (bp10[bp02] == 0) {
			//^2FCF:0324
			bp10[bp02] = bp10[((bp02 +3) &3)];
		}
		//^2FCF:0341
		if (bp10[di] == 0) {
			//^2FCF:034A
			bp10[di] = bp10[(di +1) &3];
		}
	}
	//^2FCF:035C
	i16 bp06 = xposFrom;
	i16 bp08 = yposFrom;

	//^2FCF:0368
	while (true) {
		//^2FCF:0368
_0368:
		ObjectID si = GET_TILE_RECORD_LINK(bp06, bp08);
		//^2FCF:0375
		for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
			//^2FCF:0377
			if (si.DBType() == dbMissile) {
				//^2FCF:0384
				if (glbTimersTable[GET_ADDRESS_OF_RECORDE(si)->TimerIndex()].TimerType() != tty1D) {
					//^2FCF:03A5
					bp02 = bp10[si.Dir()];
					//^2FCF:03B7
					if (bp02 != 0) {
						//^2FCF:03BB
						if (_075f_0af9(-3, xposFrom, yposFrom, bp02, si) != 0) {
							//^2FCF:03D2
							_075f_056c(si);
							//^2FCF:03D9
							goto _0368;
						}
					}
				}
			}
			//^2FCF:03DB
		}
		//^2FCF:03E9
		if (bp04 == 0) {
			//^2FCF:03ED
			break;
		}
		//^2FCF:03EF
		xposFrom |= ((bp06 = xposTo) +1) << 8;
		//^2FCF:0401
		yposFrom |= ((bp08 = yposTo)) << 8;
		//^2FCF:0412
		COPY_MEMORY(bp0c, bp10, 4);
		//^2FCF:0428
		bp04 = 0;
		//^2FCF:042D
		continue;
	}
	//^2FCF:0430
	return;
}


//^0CD5:00FE
//SPX : Random over 16 bits, using cap (modulo) value: range 0 to max, or 0 to 32768 in positive number
Bit16u SkWinCore::_RAND16(Bit16u maxcnt)
{
	// CSBwinSimilarity: STRandom

	//^0CD5:00FE
	//^0CD5:0101
	if (maxcnt == 0)
		//^0CD5:0107
		return 0;
	//^0CD5:010B
	return RAND() % maxcnt;
	//^0CD5:0117
}




//^0CEE:00CD
void SkWinCore::LOAD_LOCALLEVEL_GRAPHICS_TABLE(Bit16u curmap) //#DS=4976?
{
	//^0CEE:00CD
	//^0CEE:00D3
	CHANGE_CURRENT_MAP_TO(glbPlayerMap = curmap);
	//^0CEE:00DF
	Bit8u *bp04 = (glbMapCreaturesTypeNumber = &glbCurrentTileMap[glbCurrentMapWidth -1][glbCurrentMapHeight]);
	//^0CEE:0107
	Bit16u si = dunMapLocalHeader->WallGraphics();
	//^0CEE:0114
	COPY_MEMORY(
		bp04 += dunMapLocalHeader->CreaturesTypes(), 
		_4976_4dc2,
		si
		);
	//^0CEE:0137
	Bit16u di = dunMapLocalHeader->FloorGraphics();
	//^0CEE:0144
	COPY_MEMORY(
		bp04 += si,
		_4976_4cba,
		di
		);
	//^0CEE:015D
	COPY_MEMORY(
		bp04 += di,
		glbMapDoorOrnatesList,
		dunMapLocalHeader->DoorDecorationGraphics()
		);
	//^0CEE:0180
	return;
}




//^075F:056C
// SPX: Ran into this one after being hit by a poison ball from Dru Tan
// TODO: Delete missile ?
void SkWinCore::_075f_056c(ObjectID rl)
{
	//^075F:056C
	ENTER(0);
	//^075F:056F
	if (rl.DBType() == dbMissile) {
		DELETE_TIMER(GET_ADDRESS_OF_RECORDE(rl)->TimerIndex());
	}
	//^075F:0594
	return;
}

//^075F:0596
void SkWinCore::DELETE_MISSILE_RECORD(ObjectID rlMissile, ObjectID *prlDropTo, i16 xposDropTo, i16 yposDropTo)
{
	SkD((DLV_DBM,"DBM: DELETE_MISSILE_RECORD(%04X,%p(%04X),%2d,%2d)\n"
		, (Bitu)rlMissile.w
		, prlDropTo
		, (prlDropTo != NULL) ? (Bitu)prlDropTo->w : 0
		, (Bitu)xposDropTo
		, (Bitu)yposDropTo));

	//^075F:0596
	//^075F:059C
	ObjectID si = rlMissile;
	//^075F:059F
	Missile *bp04 = GET_ADDRESS_OF_RECORDE(si);
	//^075F:05AF
	ObjectID di = bp04->GetMissileObject();
	//^075F:05B5
	if (di.DBType() != dbCloud) {
		//^075F:05C3
		if (prlDropTo != NULL) {
			//^075F:05CB
			APPEND_RECORD_TO(di, prlDropTo, -1, 0);
		}
		else {
			//^075F:05DE
			ObjectID bp06 = GET_CREATURE_AT(xposDropTo, yposDropTo);
			//^075F:05EE
			if (bp06 != OBJECT_NULL && (QUERY_CREATURE_AI_SPEC_FLAGS(bp06) & 0x0001) != 0) {
				//^075F:0602
				Bit16u bp08 = si.Dir();
				//^075F:060A
				if ((QUERY_CREATURE_AI_SPEC_FROM_RECORD(bp06)->w30 & 15) == 1) {
					//^075F:0623
					bp08 = glbTimersTable[bp04->TimerIndex()].Direction();
					//^075F:063F
				}
				else {
					//^075F:0641
					bp08 = si.Dir();
				}
				//^075F:0649
				MOVE_RECORD_TO(ObjectID(di, bp08), -2, -1, xposDropTo, yposDropTo);

				SkD((DLV_TWEET, "Tweet: %s (a#%03d, x:%d, y:%d, map:%d) have acquired %s! \n"
					, static_cast<LPCSTR>(getRecordNameOf(bp06))
					, bp06.DBIndex()
					, xposDropTo
					, yposDropTo
					, glbCurrentMapIndex
					, static_cast<LPCSTR>(getRecordNameOf(di))
					));
			}
			else {
				//^075F:0667
				MOVE_RECORD_TO(ObjectID(di, si.Dir()), -2, 0, xposDropTo, yposDropTo);

				SkD((DLV_TWEET, "Tweet: %s (x:%d, y:%d, map:%d) have falled down! \n"
					, static_cast<LPCSTR>(getRecordNameOf(di))
					, xposDropTo
					, yposDropTo
					, glbCurrentMapIndex
					));
			}
		}
		//^075F:0687
		QUEUE_NOISE_GEN2(
			QUERY_CLS1_FROM_RECORD(si),
			QUERY_CLS2_FROM_RECORD(si),
			SOUND_STD_KNOCK,
			0xfe,
			xposDropTo,
			yposDropTo,
			0x01,
			0x3a,
			0x80
			);
	}
	//^075F:06B2
	DEALLOC_RECORD(si);
	//^075F:06B9
	return;
}

//^0CEE:0EC8
ObjectID SkWinCore::RECYCLE_A_RECORD_FROM_THE_WORLD(Bit16u itemdb, Bit8u itemtype)
{
	SkD((DLV_DBM, "DBM: RECYCLE_A_RECORD_FROM_THE_WORLD(%04X,%3u)\n"
		, (Bitu)itemdb, (Bitu)itemtype));

	// searches almost unused record from the dungeon,
	// then detach it from the dungeon,
	// then recycle the record as unused one.
	//
	// itemdb: itemdb of a record which you want to recycle.
	// itemtype: searches for any recyclable itemtype if itemtype is 0xff.
	//
	// return-val: 0xffff if there is no recyclable ones.

	//^0CEE:0EC8
	//^0CEE:0ECE
	if (itemdb == dbCloud) {
		//^0CEE:0ED4
		return OBJECT_NULL;
	}
	//^0CEE:0EDA
	Bit16u bp1a = glbCurrentMapIndex;
	//^0CEE:0EE0
	Bit16u di = (_4976_4c08 != 0) ? _4976_4c12 : 0xffff;
	//^0CEE:0EF1
	Bit16u bp26 = 0;
	//^0CEE:0EF6
	Bit16u bp0e;
	Bit16u bp18;
	bp0e = bp18 = _4976_4c42[itemdb];
	//^0CEE:0F05
	if (bp0e == glbPlayerMap || bp0e == di) {
		//^0CEE:0F13
		if (di != 0xffff) {
			do {
				//^0CEE:0F18
				if (dunHeader->nMaps <= (++bp0e)) {
					//^0CEE:0F2C
					bp0e = 0;
				}
				//^0CEE:0F31
				if (bp0e == bp18) {
					//^0CEE:0F39
					bp0e = di;
					di = 0xffff;
				}
				//^0CEE:0F3F
			} while (bp0e == di || bp0e == glbPlayerMap);
		}
		else {
			do {
				//^0CEE:0F4F
				if (dunHeader->nMaps <= (++bp0e)) {
					//^0CEE:0F63
					bp0e = 0;
				}
				//^0CEE:0F68
			} while (bp0e == glbPlayerMap);
			//^0CEE:0F71
			if (bp0e == bp18) {
				//^0CEE:0F76
				bp0e = glbPlayerMap;
			}
		}
	}
	//^0CEE:0F7C
	bp18 = bp0e;
	Bit16u bp20 = 0;

	while (true) {
		//^0CEE:0F87
        Bit16u bp14 = dunMapsHeaders[bp0e].RawColumn();
		//^0CEE:0FA0
		Bit16u bp16 = dunMapsHeaders[bp0e].RawRow();
		//^0CEE:0FB9
		Bit8u *bp04 = *glbMapTileValue[bp0e];
		//^0CEE:0FD5
		OID_T *bp0c = &dunGroundStacks[dunMapTilesObjectIndexPerColumn[dunMapColumnsSumArray[bp0e]]]; // Bit16u *bp0c
		//^0CEE:1000
		for (Bit16u bp10=0; bp10 <= bp14; bp10++) {
			//^0CEE:1008
			for (Bit16u bp12=0; bp12 <= bp16; bp12++) {
				//^0CEE:1010
				if ((*(bp04++) & 0x10) != 0) {
					//^0CEE:1024
					Bit8u bp27 = 0xff;
					//^0CEE:1028
					ObjectID si = *(bp0c++);
					//^0CEE:1032
					Bit16u bp24 = ((bp0e == glbPlayerMap) && (bp10 - glbPlayerPosX + 5 <= 10) && (bp12 - glbPlayerPosY + 5 <= 10)) ? 1 : 0;
					ObjectID bp22;

					// bp10: x-pos of GC(garbage-collector) cursor
					// bp12: y-pos of GC cursor
					// bp24: 1 if player is near from GC cursor. 0 if far

					while (true) {
						//^0CEE:1063
						Bit16u bp1c = si.DBType();
						//^0CEE:106E
						if (bp1c == dbActuator) {
							//^0CEE:1073
							Actuator *bp08 = GET_ADDRESS_OF_RECORD(si)->castToActuator();
							//^0CEE:107F
							Bit16u bp1e = bp08->ActuatorType();
							//^0CEE:108C
							if (bp1e >= 0x2c || _4976_0c5e[bp1e] == 0) {	// >= 0x2C
								//^0CEE:109E
								if (bp26 != 0 && bp24 != 0 && bp1e == ACTUATOR_FLOOR_TYPE__SHOP) {	// 0x30
									//^0CEE:10B9
									bp24 = 0;
									//^0CEE:10BE
									bp1e = bp08->ActuatorData();
									//^0CEE:10CE
									if (GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(bp1e) == itemdb) {
										//^0CEE:10DD
										bp27 = GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(bp1e);
									}
								}
								goto _1314;
							}
							goto _1324;
						}
						//^0CEE:10EC
						else if (bp1c == dbText) {
							//^0CEE:10F2
							Text *bp08 = GET_ADDRESS_OF_RECORD(si)->castToText();
							//^0CEE:10FE
							if (bp08->TextMode() != 1) {
								//^0CEE:110F
								goto _1314;
							}
							//^0CEE:1112
							if (bp08->SimpleTextExtUsage() != 4) {
								//^0CEE:1127
								goto _1314;
							}
							//^0CEE:112A
							goto _1324;
						}
						//^0CEE:112D
						else if (bp1c > dbActuator && bp24 != 0) {
							goto _1314;
						}
						//^0CEE:113C
						else if (bp1c == itemdb) {
							//^0CEE:1147
							GenericRecord *bp08 = GET_ADDRESS_OF_RECORD(si);
							//^0CEE:1153
							switch (itemdb) {
								case dbCreature:	// 4
									{
										Creature *_bp08 = bp08->castToCreature();

										//^0CEE:1168
										//^0CEE:116B
										if (_bp08->b15_2_2() != 0)
											goto _1314;
										//^0CEE:117C
										if (_bp08->possession != OBJECT_END_MARKER)	// 0xFFFE
											goto _1314;
										//^0CEE:1186
										if (itemtype != 0xff) {
											//^0CEE:118C
											if (_bp08->CreatureType() != itemtype)
												goto _1314;
										}
										//^0CEE:119B
										CHANGE_CURRENT_MAP_TO(bp0e);
										//^0CEE:11A3
										DELETE_CREATURE_RECORD(bp10, bp12, CREATURE_DROP_POSSESSION_ONLY, 0xffff);	// (bp10, bp12, 1, 0xffff)
										//^0CEE:11B5
										break;
									}
								case dbMissile:		//14
									{
										Missile *_bp08 = bp08->castToMissile();

										//^0CEE:11B8
										if (itemtype != 0xff) {
											//^0CEE:11BE
											if (QUERY_CLS2_FROM_RECORD(_bp08->w2) != itemtype)
												goto _1314;
										}
										//^0CEE:11D3
										_075f_056c(si);
										//^0CEE:11DA
										CUT_RECORD_FROM(si, NULL, bp10, bp12);
										//^0CEE:11EC
										DELETE_MISSILE_RECORD(si, NULL, bp10, bp12);
										//^0CEE:11FC
										break;
									}
								case dbCloth:		// 6
									{
										Cloth *_bp08 = bp08->castToCloth();

										//^0CEE:11FF
										if (_bp08->Important() != 0)
											goto _1314;
										//^0CEE:1213
										//^0CEE:121A
										//^0CEE:1263
										Bit16u bp1e = _bp08->ItemType();
										//^0CEE:1269
										if (bp1e == bp27)
											goto _1314;
										//^0CEE:1278
										if (itemtype != 0xff && itemtype != bp1e)
											goto _1314;
										//^0CEE:1289
										CHANGE_CURRENT_MAP_TO(bp0e);
										//^0CEE:1291
										MOVE_RECORD_TO(si, bp10, bp12, -1, (bp20 != 0) ? -1 : 0);
										//^0CEE:12B0
										break;
									}
								case dbWeapon:		// 5
									{
										Weapon *_bp08 = bp08->castToWeapon();

										//^0CEE:121C
										if (_bp08->Important() != 0)
											goto _1314;
										//^0CEE:1230
										//^0CEE:1213
										//^0CEE:121A
										//^0CEE:1263
										Bit16u bp1e = _bp08->ItemType();
										//^0CEE:1269
										if (bp1e == bp27)
											goto _1314;
										//^0CEE:1278
										if (itemtype != 0xff && itemtype != bp1e)
											goto _1314;
										//^0CEE:1289
										CHANGE_CURRENT_MAP_TO(bp0e);
										//^0CEE:1291
										MOVE_RECORD_TO(si, bp10, bp12, -1, (bp20 != 0) ? -1 : 0);
										//^0CEE:12B0
										break;
									}
								case dbPotion:		// 8
									{
										Potion *_bp08 = bp08->castToPotion();

										//^0CEE:1248
										if (_bp08->VisiblePower() != 0)
											goto _1314;
										//^0CEE:125C
										Bit16u bp1e = _bp08->PotionType();
										//^0CEE:1269
										if (bp1e == bp27)
											goto _1314;
										//^0CEE:1278
										if (itemtype != 0xff && itemtype != bp1e)
											goto _1314;
										//^0CEE:1289
										CHANGE_CURRENT_MAP_TO(bp0e);
										//^0CEE:1291
										MOVE_RECORD_TO(si, bp10, bp12, -1, (bp20 != 0) ? -1 : 0);
										//^0CEE:12B0
										break;
									}
								case dbMiscellaneous_item:	//10
									{
										Miscellaneous_item *_bp08 = bp08->castToMisc();

										//^0CEE:1232
										if (_bp08->Important() != 0)
											goto _1314;
										//^0CEE:1246
										//^0CEE:1213
										//^0CEE:121A
										//^0CEE:1263
										Bit16u bp1e = _bp08->ItemType();
										//^0CEE:1269
										if (bp1e == bp27)
											goto _1314;
										//^0CEE:1278
										if (itemtype != 0xff && itemtype != bp1e)
											goto _1314;
										//^0CEE:1289
										CHANGE_CURRENT_MAP_TO(bp0e);
										//^0CEE:1291
										MOVE_RECORD_TO(si, bp10, bp12, -1, (bp20 != 0) ? -1 : 0);
										//^0CEE:12B0
										break;
									}
								case dbScroll:		// 7
								case dbContainer:	// 9
								case db11:			//11
								case db12:			//12
								case db13:			//13
									//^0CEE:12B0
									break;
							}
							//^0CEE:12B0
							CHANGE_CURRENT_MAP_TO(bp1a);
							//^0CEE:12B8
							_4976_4c42[itemdb] = (Bit8u)bp0e;
							//^0CEE:12C2v
							return si.GetAsNorth();
						}
						//^0CEE:12CA
						else if (bp1c == dbCreature && itemdb != dbMissile) {
							//^0CEE:12D6
							Creature *bp08 = GET_ADDRESS_OF_RECORD(si)->castToCreature();
							//^0CEE:12E2
							if (bp08->possession == OBJECT_END_MARKER) {
								//^0CEE:12EA
								goto _1314;
							}
							//^0CEE:12EC
							if (QUERY_CREATURE_AI_SPEC_FROM_TYPE(bp08->CreatureType())->IsStaticObject() == 0) {
								//^0CEE:1300
								goto _1314;
							}
							//^0CEE:1302
							bp20 = 1;
							bp22 = si;
							//^0CEE:130A
							si = bp08->possession;
							//^0CEE:1311
							continue;
						}
						else {
							goto _1314;
						}

						//^0CEE:1314
_1314:
						si = GET_NEXT_RECORD_LINK(si);
						//^0CEE:131C
						if (si != OBJECT_END_MARKER)
							continue;
						//^0CEE:1324
_1324:
						if (bp20 == 0)
							break;
						//^0CEE:132A
						si = bp22;
						bp20 = 0;
						goto _1314;
					}
				}
				//^0CEE:1334
			}
			//^0CEE:1342
		}
		//^0CEE:1350
		if (bp0e == glbPlayerMap || dunHeader->nMaps <= 1) {
			//^0CEE:1364
			if (bp26 != 0) {
				//^0CEE:136A
				_4976_4c42[itemdb] = (Bit8u)bp0e;
				//^0CEE:1374
				//^0CEE:0ED4
				return OBJECT_NULL;
			}
			//^0CEE:1377
			bp26 = 1;
			//^0CEE:137C
			continue;
		}
		//^0CEE:137F
		if (di != 0xffff) {
			do {
				//^0CEE:1384
				if (dunHeader->nMaps <= (++bp0e)) {
					//^0CEE:1398
					bp0e = 0;
				}
				//^0CEE:139D
				if (bp0e == bp18) {
					//^0CEE:13A5
					bp0e = di;
					di = 0xffff;
				}
				//^0CEE:13AB
			} while (bp0e == di || bp0e == glbPlayerMap);
			//^0CEE:13B9
			continue;
		}
		do {
			//^0CEE:13BC
			if (dunHeader->nMaps <= (++bp0e)) {
				//^0CEE:13D0
				bp0e = 0;
			}
			//^0CEE:13D5
		} while (bp0e == glbPlayerMap);
		//^0CEE:13DE
		if (bp0e != bp18)
			continue;
		//^0CEE:13E6
		bp0e = glbPlayerMap;
	}
}

//^0CEE:1409
ObjectID SkWinCore::ALLOC_NEW_RECORD(Bit16u db)
{
	SkD((DLV_DBM, "DBM: ALLOC_NEW_RECORD(%2d)\n", (Bitu)db));

	//^0CEE:1409
	Bit16u si = db;
	ObjectID di = OBJECT_NULL;
	Bit16u bp08 = dunHeader->nRecords[(si & 0x7fff)];
	//^0CEE:1429
	if (si == 0x800a) { // 0x8000 is for bone?
		//^0CEE:142F
		si = 0x000a;
	}
	else {
		//^0CEE:1434
		if (si == 0x000a) {
			//^0CEE:1439
			bp08 -= 3;
		}
	}
	//^0CEE:143D
	Bit16u bp06 = bp08;
	Bit16u bp0a = glbItemSizePerDB[si];
	Bit8u *bp04 = glbDBObjectData[si];

	while (true) {
		//^0CEE:145F
		if (reinterpret_cast<GenericRecord *>(bp04)->w0 == OBJECT_NULL) {
			//^0CEE:1468
			di = ObjectID(0, si, bp08 - bp06);
		}
		else {
			//^0CEE:1479
			if (bp06-- != 0) {
				//^0CEE:147E
				bp04 += (bp0a >> 1) << 1;
				continue;
			}
			else {
				//^0CEE:148A
				di = RECYCLE_A_RECORD_FROM_THE_WORLD(si, 0xFF);
				//^0CEE:1496
				if (di == OBJECT_NULL) {
					//^0CEE:149B
					//^0CEE:14D5
					return OBJECT_NULL;
				}
				//^0CEE:14A0
				bp04 = reinterpret_cast<Bit8u *>(GET_ADDRESS_OF_RECORD(di));
			}
		}
		//^0CEE:14AC
		ZERO_MEMORY(bp04, bp0a);
		//^0CEE:14C0
		reinterpret_cast<GenericRecord *>(bp04)->w0 = OBJECT_END_MARKER;
		//^0CEE:14C8
		if (si == dbContainer) {
			//^0CEE:14CD
			reinterpret_cast<Container *>(bp04)->w2 = OBJECT_END_MARKER;
		}
		//^0CEE:14D3
		return di;
	}
}

//^069A:035B
X16 SkWinCore::_069a_035b(X16 xx)
{
	//^069A:035B
	ENTER(0);
	//^069A:035E
	if (xx == 0xffff)
		return 0xffff;
	//^069A:036B
	if ((_4976_00e3[RCJ(5, U8(xx))] & 8) != 0)
		return U8(xx) -32;
	//^069A:0381
	return U8(xx);
}
//^069A:0406
/*
X16 SkWinCore::_sk_setjmp(jmp_buf xx)
{
	return setjmp(xx);
}
*/

//^01B0:20EF
// TODO: random seed ?
i16 SkWinCore::_01b0_20ef()
{
	// Query random seed?
	// AH=sec.
	// AL=milli sec.

	time_t t;
	t = time(&t);
	return U8(localtime(&t)->tm_sec) << 8;
}

//^069A:038D
void (SkWinCore::*SkWinCore::_crt_getvect(U16 interruptno))() {
#if UseAltic
	return _sys_getvect(interruptno);
#else
#error	Unc
#endif
}

//^00EB:0BC4
void SkWinCore::_00eb_0bc4() //#DS=04BF
{
	//^00EB:0BC4
	ENTER(0);
	//^00EB:0BC8
	LOADDS(0x0C48);
	_04bf_0e34 = reinterpret_cast<U8 *>(__vram);
	U16 si = 1;
	do {
		//^00EB:0BDD
		if (si < 0xfa00) {
			//^00EB:0BE3
			IBMIO_FILL_SCREEN_LINE(si, 0, 1);
		}
		//^00EB:0BEF
		si = ((si & 1) != 0) ? ((si >> 1) ^ 0xb400) : (si >> 1);
		//^00EB:0C02
	} while (si != 1);
	//^00EB:0C09
	IBMIO_FILL_SCREEN_LINE(0, 0, 1);
	//^00EB:0C17
	return;
}

//^01B0:2B1B
// TODO : that one does nothing ?!
X16 SkWinCore::_01b0_2b1b()
{
	//^01B0:2B1B
	ENTER(0);
	//^01B0:2B1E
	return 0;
}

//^01B0:0E80
X16 SkWinCore::_01b0_0e80(void (SkWinCore::*pfn)()) //#DS=04BF
{
	//^01B0:0E80
	ENTER(0);
	//^01B0:0E83
	LOADDS(0x3083);
	_04bf_18ae = pfn;
	_04bf_0284 = 1;

	SkD((DLV_DBG_TICK, "Tick step value = %03d\n", 1));
	//^01B0:0EA0
	return 1;
}

//^4726:03B2
// TODO: related to some init ?
void SkWinCore::_4726_03b2()
{
	//^4726:03B2
	ENTER(0);
	//^4726:03B5
#if UseAltic
	_4976_5e90 = _4976_5e98 = _4976_5e88 = _4976_5e8c = 0;
#else
	_4976_5e90 = _crt_getvect(0xfe);
	_4976_5e98 = _4976_5e90;
	_4976_5e88 = _crt_getvect(0xff);
	_4976_5e8c = _4976_5e88;
#endif
	//^4726:03F1
	glbRandomSeed = _01b0_20ef() CALL_IBMIO * 0x7AE3;
#if UseAltic
	glbRandomSeed = 12345;
#endif
	_00eb_0bc4();
	IBMIO_SELECT_PALETTE_SET(0);
	_01b0_2b1b();
	glbTickStepValue = _01b0_0e80(&SkWinCore::_4726_0383);
	//^4726:0446
	return;
}

//^069A:0309
U32 SkWinCore::_crt_farcoreleft() { // TODO: Unr
	Unr(); return 0;
}

//^069A:0387
U32 SkWinCore::_crt_coreleft() { // TODO: Unr
	Unr(); return 0;
}

//^069A:0470
i16 SkWinCore::_crt_setblock(U16 segx, U16 newsize) { // TODO: Unr
	Unr(); return 0;
}

//^069A:01DD
U16 SkWinCore::_crt_normalize(U8 *cp) { // TODO: Unr
	Unr(); return 0;
}

//^069A:029C
U8 *SkWinCore::_crt_brk(U32 addr) { // TODO: Unr
	Unr(); return NULL;
}

//^069A:0607
U8 *SkWinCore::_crt_CreateHeap(U16 ax) { // TODO: Unr
	Unr(); return NULL;
}

//^069A:06CE
U8 *SkWinCore::_crt_farmalloc(U32 size) { // TODO: Unr
	Unr(); return NULL;
}

//^3E74:04D8
void SkWinCore::INIT_FREE_POOL(sk5d00 *ref, i32 size, U16 poolflag)
{
	//^3E74:04D8
	ENTER(0);
	//^3E74:04DB
	EMS_MAP_BANK_TO_MEMORY();
	ref->pv0 = _4976_5d00;
	ref->dw4 = size -sizeof(sk5d00);
	ref->w16 = poolflag;
	if (ref->Is4EMS()) {
		//^3E74:0523
		_4976_5d20 = CONVERT_PHYS_TO_SHELF_FORM(reinterpret_cast<U8 *>(ref));
		ref->dw8 = ref->dw4 +0x00200000 +sizeof(sk5d00);
	}
	else {
		//^3E74:0551
		ref->dw8 = reinterpret_cast<U32ptr>(PTR_PADD(ref,ref->dw4 +sizeof(sk5d00)));
	}
	//^3E74:057C
	ref->dw12 = ref->dw4;
	_4976_5d00 = ref;
	//^3E74:059C
	return;
}
//^3E74:059E
sk5d00 *SkWinCore::_3e74_059e(X16 xx)
{
	//^3E74:059E
	ENTER(12);
	//^3E74:05A3
	X16 si = xx;
	sk5d00 *bp08 = NULL;
	U32 bp0c = 0;
	sk5d00 *bp04 = _4976_5d00;
	if (bp04 == NULL) {
		return NULL;
	}
	//^3E74:05DD
	EMS_MAP_BANK_TO_MEMORY();
	do {
		//^3E74:05EB
		if (bp04->w18 != 0)
			continue;
		if ((bp04->w16 & si) != si)
			continue;
		if ((bp04->w16 & 0x800) != (si & 0x800))
			continue;
		if (bp04->dw4 <= bp0c)
			continue;
		//^3E74:0624
		bp08 = bp04;
		bp0c = bp08->dw4;
		//^3E74:0641
	} while ((bp04 = bp04->pv0) != NULL);
	//^3E74:0660
	if (bp08 != NULL) {
		//^3E74:0671
		bp08->w18 = 1;
	}
	//^3E74:067A
	return bp08;
}

//^3E74:1175
U16 SkWinCore::ADD_MEM_TO_FREE_POOL(sk5d00 *pool, i32 size, X16 poolflag)
{
	//^3E74:1175
	ENTER(4);
	//^3E74:1179
	if (pool != NULL) {
		//^3E74:118A
		size &= 0xfffffffe;
		if (size < 32) {
			//^3E74:11A2
			return 0;
		}
		//^3E74:11A7
		INIT_FREE_POOL(pool, size, poolflag);
		return 1;
	}
	//^3E74:11C0
	_4976_5c7a = 1;
	sk5d00 *bp04 = _3e74_059e(0x7f8);	// 0x7f8 = 2040
	if (bp04 == NULL) {
		//^3E74:11DF
		RAISE_SYSERR(SYSTEM_ERROR__CANT_ADD_MEM);
	}
	//^3E74:11E7
	_4976_5cb2 = _4976_5ce6 = reinterpret_cast<mement *>(&bp04[+1]);
	_4976_5cf0 = _4976_5cf4 = PTR_PADD(bp04,+bp04->dw4 +sizeof(sk5d00));
	glbFreeRAMMemPool = bp04->dw4;
	_4976_5cee = bp04->w16;
	//^3E74:124C
	bp04 = _3e74_059e(0xc00);
	if (bp04 != NULL || (bp04 = _3e74_059e(0x400)) != NULL) {
		//^3E74:127E
		_4976_5d6e = bp04->w16;
		if ((_4976_5d6e & 0x800) != 0) {
			//^3E74:1290
			_4976_5d5a = tiamat::From_cems(bp04->dw4 +sizeof(sk5d00));
		}
		else {
			//^3E74:12A7
			_4976_5d5a = ptr2t(PTR_PADD(bp04,+bp04->dw4 +sizeof(sk5d00)));
		}
		//^3E74:12CE
		glbFreeEMSMemPool = bp04->dw4;
	}
	//^3E74:12E0
	_4976_5d04 = ALLOC_MEMORY_RAM(TEMP_BUFF_SIZE, afUseUpper, 0x280);
	_4976_5d94 = NULL;
	_4976_5d90 = NULL;
	_4976_5c8c = NULL;
	_4976_5d70 = NULL;
	_4976_5d5e = NULL;
	_4976_5d2a = -1;
	//^3E74:132B
	return 1;
}
//^4726:0055
U32 SkWinCore::EMS_ALLOC_POOL(U8 **buff)
{
#ifdef _SK_DYNAMIC_CEMS
	cems = (U8*) calloc(MAX_CEMS, sizeof(U8));
#endif // _SK_DYNAMIC_CEMS

#if UseAltic
	_4976_5ea0 = cems;
	_4976_5eac = shelf_memory::From_cems(shelf_memory::SizeOf_cems() -0x8000);	// replaced sizeof(cems)
	return shelf_memory::SizeOf_cems()-0x8000;
#else
	ATLASSERT(false);
	return 0;
#endif
//!	//^4726:0055
//!	ENTER(4);
//!	//^4726:0059
//!	_4976_5ea4 = 0;
//!	WRITE_UI16(buff,+0,0);
//!	WRITE_UI16(buff,+2,0);
//!	if (_4726_000d() == 0)
//!		return 0;
//!	//^4726:0078
//!	__asm mov ah,0x42
//!	__asm int 0x67
//!	if (ah == 0) {
//!		U16 bp02 = bx;
//!		__asm mov ah,0x43
//!		__asm int 0x67
//!		if (ah == 0) {
//!			//^4726:008D
//!			_4976_5ea4 = dx;
//!			__asm mov ah,0x41
//!			__asm int 0x67
//!			U16 bp04 = bx;
//!			WRITE_UI16(buff,+0,0);
//!			WRITE_UI16(buff,+2,bp04);
//!			EMS_MAP_BANK_TO_MEMORY();
//!			if (bp02 >= 0x10) {
//!				//^4726:00BB
//!				_4976_4968 = 1;
//!				_4976_5eac = shelf_memory::FromPg(bp02).Subtract(0x8000);
//!				return _4976_5eac;
//!			}
//!			//^4726:00DF
//!			return bp02 << 14;
//!		}
//!	}
//!	//^4726:00ED
//!	return 0;
}

//^4726:02F7
void SkWinCore::_4726_02f7()
{
	//^4726:02F7
	ENTER(0);
	//^4726:02FA
#if UseAltic
	_4976_5e9c = sizeof(cram);
	_4976_5e94 = cram;
#else
	_4976_5e9c = _crt_coreleft();
	_4976_5e94 = _crt_farmalloc(_4976_5e9c);
#endif
	if (_4976_5e94 == NULL) {
		//^4726:0320
		__terminate(0);
	}
	//^4726:0328
	ATLVERIFY(ADD_MEM_TO_FREE_POOL(reinterpret_cast<sk5d00 *>(_4976_5e94), _4976_5e9c, 0x7f8));
	_4976_5ea6 = EMS_ALLOC_POOL(&_4976_5ea0);
	ATLVERIFY(ADD_MEM_TO_FREE_POOL(reinterpret_cast<sk5d00 *>(_4976_5ea0), _4976_5ea6, 0xc00));
	ATLVERIFY(ADD_MEM_TO_FREE_POOL(NULL, NULL, 0));
	//^4726:0381
	return;
}

//^01B0:1FFC
X16 SkWinCore::_01b0_1ffc(X16 xx)
{
#if UseAltic
	return 1;
#else
	// TODO: ioctl
	ATLASSERT(false);
	return 1;
#endif
}

//^476D:02E0
X16 SkWinCore::_476d_02e0(X16 xx)
{
	//^476D:02E0
	ENTER(0);
	//^476D:02E3
	return _01b0_1ffc(xx) CALL_IBMIO;
}

//^01B0:20CA
void SkWinCore::_01b0_20ca(i16 drvno, U8 *str)
{
	//^01B0:20CA
	ENTER(0);
	//^01B0:20CD
	if (drvno >= 0 && drvno < 0x19) {
		//^01B0:20D9
		str[0] = U8(drvno) +0x40;
		str[1] = ':';
		str[2] = 0;
	}
	//^01B0:20ED
	return;
}

//^476D:04F4
void SkWinCore::_476d_04f4(i16 drvno, U8 *str)
{
	//^476D:04F4
	ENTER(0);
	//^476D:04F7
	_01b0_20ca(drvno, str);
	//^476D:050C
	return;
}

//^476D:018A
void SkWinCore::_476d_018a()
{
	//^476D:018A
	ENTER(0);
	//^476D:018D
	//^476D:01AE
	_4976_5eb2 = _getdrive();
	if (_476d_02e0(_4976_5eb2) == 1) {
		//^476D:01C1
		glbDriveNumber = _4976_5eb2;
		_4976_499e = 1;
		_4976_5ebc = _4976_5eb2;
		_4976_49a0 = 1;
		_4976_5eb4 = 1;
		_4976_5eb0 = 1;
	}
	else {
		//^476D:01E2
		_4976_5eb6 = _4976_5eb2;
		glbDriveNumber = _4976_5eb4 = _4976_5eb2 ^ 3;
		if (glbDriveNumber == 1) {
			//^476D:01FB
			if (_476d_02e0(2) != 0 && _476d_02e0(1) == _476d_02e0(2)) {
				//^476D:021D
				_4976_5ebc = 2;
				_4976_5eb0 = 0;
			}
			else {
				//^476D:022B
				_4976_5ebc = _4976_5eb2;
				_4976_5eb0 = 1;
			}
		}
		//^476D:0239
		else if (glbDriveNumber == 2) {
			if (_476d_02e0(1) != 0 && _476d_02e0(1) == _476d_02e0(2)) {
				//^476D:0262
				_4976_5ebc = 1;
				_4976_5eb0 = 0;
			}
			else {
				//^476D:0270
				_4976_5ebc = _4976_5eb2;
				_4976_5eb0 = 1;
			}
		}
		else {
			//^476D:027E
			_4976_5ebc = _4976_5eb2;
			_4976_5eb0 = 1;
		}
	}
	//^476D:028A
	if (_4976_499e != 0) {
		_476d_04f4(1, _4976_4980);
	}
	else {
		_476d_04f4(glbDriveNumber, _4976_4980);
	}
	//^476D:02A9
	_476d_04f4(_4976_5ebc, _4976_4984);
	//SPX: 0x40 = 'A'-1
	strDirLetter[0] = glbDriveNumber + 0x40;
	strDirLetter2[0] = _4976_5ebc +0x40;
	//^476D:02C9
	return;
}
//^2636:03D4
void SkWinCore::_2636_03d4()
{
	//^2636:03D4
	ENTER(0);
	//^2636:03D8
	_4976_52f8 = 0;
	i16 si = 0;
	for (; si < 2; si++) {
		//^2636:03E2
		_4976_52ea[si] = ALLOC_MEMORY_RAM(0x80, afUseUpper, 0x400);
	}
	//^2636:0407
	return;
}

//^476D:0020
//U32 GET_FILE_POS_6(i16 handle);

//^476D:003C
U32 SkWinCore::GET_FILE_SIZE(i16 handle) {
	return fset.fileGetSize(handle);
}

//^3E74:0004
U16 SkWinCore::SWAPW(U16 xx)
{
	//^3E74:0004
	ENTER(0);
	//^3E74:0007
	return (xx << 8) + (xx >> 8);
}
//^3E74:16ED
U32 SkWinCore::QUERY_GDAT_ENTRY_VALUE(U16 entryIndex, U16 entryPos)
{
	ATLASSERT(entryPos < 7);

	//^3E74:16ED
	ENTER(8);
	//^3E74:16F3
	X16 di = entryPos;
	U8 *bp04 = PTR_PADD(PTR_PADD(_4976_5d38,U32(entryIndex) * _4976_5d3e),_4976_5d42[di]);
	i16 si = _4976_5d50[di];
	U32 bp08 = 0;
	while (si-- > 0) {
		//^3E74:1746
		bp08 = (bp08 << 8) + i16(*bp04);
		bp04++;
	}
	//^3E74:1782
	return bp08;
}

//^3E74:216A
void SkWinCore::LOAD_GDAT_ENTRIES()
{
	//^3E74:216A
	ENTER(12);
	//^3E74:2170
	X16 di;
	for (di = 0; di < glbGDatNumberOfRawEntries; di++) {
		//^3E74:2175
		if (QUERY_GDAT_ENTRY_VALUE(di, EPcls6) != 0xff)
			continue;
		U8 bp05 = QUERY_GDAT_ENTRY_VALUE(di, EPcls3);
		if (bp05 == 0xb || bp05 == 0xc)
			continue;
		//^3E74:21A3
		X16 si = QUERY_GDAT_ENTRY_VALUE(di, EPdata);
		if (glbShelfMemoryTable[si].Present())
			continue;
		//^3E74:21CC
		U16 bp08;
		if (QUERY_GDAT_ENTRY_VALUE(di, EPcls1) != 1) {
			//^3E74:21DC
			shelf_memory bp0c = ALLOC_MEMORY_EMS((bp08 = QUERY_GDAT_RAW_DATA_LENGTH(si)) +2, afUseUpper, 0xc00) +2;
			U8 *bp04 = REALIZE_GRAPHICS_DATA_MEMORY(bp0c);
			//^3E74:2214
			WRITE_UI16(bp04,-2,bp08);
//LOGX(("LOAD_GDAT_RAW_DATA call from LOAD_GDAT_ENTRIES (1)"));
			LOAD_GDAT_RAW_DATA(si, glbShelfMemoryTable[si] = bp0c);
			EMS_MAP_BANK_TO_MEMORY();

			SkD((DLV_GLD, "GLD: Load Raw#%4d at EMS(%08X)\n", (Bitu)si, (Bitu)bp0c.val));
		}
		else {
			//^3E74:225B
			U8 *bp04 = ALLOC_MEMORY_RAM((bp08 = QUERY_GDAT_RAW_DATA_LENGTH(si)) +2, afUseUpper, 0x400);
			WRITE_UI16(bp04,+0,bp08);
//LOGX(("LOAD_GDAT_RAW_DATA call from LOAD_GDAT_ENTRIES (2)"));
			LOAD_GDAT_RAW_DATA(si, glbShelfMemoryTable[si] = CONVERT_PHYS_TO_SHELF_FORM(bp04 +2));

			SkD((DLV_GLD, "GLD: Load Raw#%4d at RAM(%08X)\n", (Bitu)si, bp04));
		}
		//^3E74:22C1
	}
	//^3E74:22CB
	return;
}
//^3E74:2162
X16 SkWinCore::_3e74_2162(U16 xx)
{
	//^3E74:2162
	ENTER(0);
	//^3E74:2165
	return 1;
}

X16 SkWinCore::LANG_FILTER(U16 entryIndex)
{
	U8 cls1 = U8(QUERY_GDAT_ENTRY_VALUE(entryIndex, EPcls1)); // Main category
	U8 cls2 = U8(QUERY_GDAT_ENTRY_VALUE(entryIndex, EPcls2)); // Sub category
	U8 cls3 = U8(QUERY_GDAT_ENTRY_VALUE(entryIndex, EPcls3)); // Entry kind
	U8 cls4 = U8(QUERY_GDAT_ENTRY_VALUE(entryIndex, EPcls4)); // Entry no
	U8 cls5 = U8(QUERY_GDAT_ENTRY_VALUE(entryIndex, EPcls5)); // Optional 1

	if (cls3 == fmtText) {

		if (cls5 == s_textLangSel[cls1][cls2][cls4]) {
			// already elected
			return 1;
		}

		if (cls5 == 0x00 || cls5 == 0xF0) {
			// language independent text
			if (s_textLangSel[cls1][cls2][cls4] == 0xFF) {
				s_textLangSel[cls1][cls2][cls4] = cls5;
				return 1;
			}
		}

		if (cls5 == skwin.GetLang()) {
			// elected language
			if (s_textLangSel[cls1][cls2][cls4] == 0xFF) {
				s_textLangSel[cls1][cls2][cls4] = cls5;
				return 1;
			}
			if (SkCodeParam::bUseMultilanguageExtended 
				&& cls1 == GDAT_CATEGORY_CHAMPIONS) {
				s_textLangSel[cls1][cls2][cls4] = cls5;
				return 0;	// don't return 1 or it will crash because the standard text is already here with cls5 = 0x00
			}
		}
		return 0; // never pass for other language.
	}

	// SPX: manages also localized images only for char interface
#if DM2_EXTENDED_MODE == 1
	if (SkCodeParam::bUseMultilanguageExtended) {
		if (cls3 == fmtImage && (cls1 == GDAT_CATEGORY_INTERFACE_CHARSHEET || cls1 == GDAT_CATEGORY_TITLE)) {
			U8 iLangSelect = (cls5 & 0xF0);	// Do not take variation 0x08 into account

			if (iLangSelect == s_imageLangSel[cls1][cls2][cls4]) {
				return 1;
			}

			if (iLangSelect == 0x00 || iLangSelect == 0xF0) {
				if (s_imageLangSel[cls1][cls2][cls4] == 0xFF) {
					s_imageLangSel[cls1][cls2][cls4] = iLangSelect;
					return 1;
				}
			}

			if (iLangSelect == skwin.GetLang()) {
				// If corresponding language, always prioritary
					s_imageLangSel[cls1][cls2][cls4] = iLangSelect;
					return 1;
			}
		return 0;
		}
	}
#endif // DM2_EXTENDED_MODE
	
	return 1; // always pass for non text entry.
}

//^3E74:1D5F
// SPX: With help from Kentaro to enable more memory when loading GDAT (specially to go over 0x1C categories)
void SkWinCore::BUILD_GDAT_ENTRY_DATA(GDATEntries *ref, X16 (SkWinCore::*pfnIfLoad)(U16 xx), U8 *zz)
{
	//^3E74:1D5F
	ENTER(14);
	//^3E74:1D65
	// SPX: Original value was 0x3a0, so 0x1D max categories
	const int buffSize = (GDAT_CATEGORY_LIMIT+1) * 16 * 2;
	U16 *bp04 = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(buffSize, afUseLower|afZeroMem, 0x400));
	ref->w12 = 0;
	ref->w16 = 0;
	U16 si;
	U8 bp0a;
	for (si = 0; si < glbGDatNumberOfRawEntries; si++) {
		//^3E74:1D91
		if ((this->*pfnIfLoad)(si) == 0)
			continue;
		//^3E74:1D9D
		bp0a = U8(QUERY_GDAT_ENTRY_VALUE(si, EPcls1));
		U8 bp09 = U8(QUERY_GDAT_ENTRY_VALUE(si, EPcls3));
		if (bp0a > GDAT_CATEGORY_LIMIT || bp09 > 0xe)
			continue;
		//^3E74:1DC1
		ref->w16++;
		if (ref->w12 < bp0a) {
			//^3E74:1DD3
			ref->w12 = bp0a;
		}
		//^3E74:1DDC
		bp04[(bp0a << 4) + (bp09)]++;
		if (bp04[(bp0a << 4) +15] <= bp09) {
			//^3E74:1E11
			bp04[(bp0a << 4) +15] = bp09 +1;
		}
		//^3E74:1E2C
	}
	//^3E74:1E36
	X16 bp0c = 0;
	for (bp0a = 0; ref->w12 >= bp0a; bp0a++) {
		//^3E74:1E41
		bp0c += bp04[(bp0a << 4) +15];
		//^3E74:1E59
	}
	//^3E74:1E6A
	ref->w14 = bp0c;
	ref->w18 = 0;
	ref->w20 = 0;
	U16 di;
	for (di = 0; di < 7; di++) {
		//^3E74:1E81
		if (di <= 4) {
			//^3E74:1E86
			ref->b36[di] = _4976_483f[RCJ(5,di)];
			ref->w18 += _4976_483f[RCJ(5,di)];
			ref->w22[di] = 0xffff;
			if (_4976_483f[RCJ(5,di)] > 0) {
				//^3E74:1EAA
				ref->w20++;
			}
			continue;
		}
		//^3E74:1EB3
		if (zz[di] != 0) {
			ref->w22[di] = ref->w18;
			ref->b36[di] = _4976_5d50[RCJ(7,di)];
			ref->w18 += _4976_5d50[RCJ(7,di)];
			ref->w20++;
		}
		//^3E74:1EE2
	}
	//^3E74:1EE8
	ref->pw0 = reinterpret_cast<X16 *>(ALLOC_MEMORY_RAM((ref->w12 +2) << 1, afUseUpper, 0x400));
	ref->pw4 = reinterpret_cast<X16 *>(ALLOC_MEMORY_RAM((ref->w14 +1) << 1, afUseUpper, 0x400));
	ref->pv8 = reinterpret_cast<RawEntry *>(ALLOC_MEMORY_RAM(U32(ref->w16) << 2, afUseUpper, 0x400));
	bp0c = 0;
	si = 0;
	U8 bp09;
	for (bp0a = 0; ref->w12 >= bp0a; bp0a++) {
		//^3E74:1F66
		ref->pw0[bp0a] = bp0c;
		di = bp04[(bp0a << 4) +15];
		for (bp09 = 0; bp09 < di; bp0c++, bp09++) {
			//^3E74:1F96
			ref->pw4[bp0c] = si;
			si += bp04[(bp0a << 4) +bp09];
			//^3E74:1FC6
		}
		//^3E74:1FCF
	}
	//^3E74:1FE0
	ref->pw0[ref->w12 +1] = bp0c;
	//^3E74:1FFC
	ref->pw4[ref->w14] = ref->w16;
	//^3E74:2021
	ZERO_MEMORY(bp04, buffSize);	// Original buffsize = 0x3a0
	ZERO_MEMORY(ref->pv8, U32(ref->w16) << 2);
	//^3E74:2059
	X16 bp0e;
	for (si = 0; si < glbGDatNumberOfRawEntries; si++) {
		//^3E74:205E
		if ((this->*pfnIfLoad)(si) == 0)
			continue;
		//^3E74:206A
		bp0a = QUERY_GDAT_ENTRY_VALUE(si, EPcls1);
		bp09 = QUERY_GDAT_ENTRY_VALUE(si, EPcls3);
		if (bp0a > GDAT_CATEGORY_LIMIT || bp09 > 0xe)
			continue;
		//^3E74:2094
		di = bp04[(bp0a << 4) +bp09]++;
		RawEntry *bp08 = &ref->pv8[ref->pw4[ref->pw0[bp0a] +bp09] +di];
		bp08->cls2 = QUERY_GDAT_ENTRY_VALUE(si, EPcls2);
		bp08->cls4 = QUERY_GDAT_ENTRY_VALUE(si, EPcls4);
		bp08->data = QUERY_GDAT_ENTRY_VALUE(si, EPdata);
		bp0e = QUERY_GDAT_ENTRY_VALUE(si, EPcls6);
		if (bp0e == 1) {
			//^3E74:2140
			bp08->data |= 0x8000;
		}
		//^3E74:2149
	}
	//^3E74:2153
	DEALLOC_LOWER_MEMORY(buffSize);		// Original buffsize = 0x3a0
	//^3E74:215E
	return;
}

//^3E74:22CF
void SkWinCore::LOAD_ENT1()
{
	//^3E74:22CF
	ENTER(8);
	//^3E74:22D5
	U32 bp08 = _4976_5d7a;
	_4976_5d38 = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(bp08, afUseLower, 0x400));
//LOGX(("LOAD_GDAT_RAW_DATA call from LOAD_ENT1"));
	LOAD_GDAT_RAW_DATA(0, CONVERT_PHYS_TO_SHELF_FORM(reinterpret_cast<U8 *>(_4976_5d38)));
	U16 si = *_4976_5d38;
	if (si != 0x8001 && SWAPW(si) != 0x8001) {
		RAISE_SYSERR(SYSTEM_ERROR__INVALID_ENT1);
	}
	//^3E74:2334
	glbGDatNumberOfRawEntries = _4976_5d38[1];
	_4976_5d40 = _4976_5d38[2];
	if (si != 0x8001) {
		//^3E74:2350
		glbGDatNumberOfRawEntries = SWAPW(glbGDatNumberOfRawEntries);
		_4976_5d40 = SWAPW(_4976_5d40);
	}
	//^3E74:2368
	U8 (*bp04)[2] = (U8 (*)[2])PTR_PADD(_4976_5d38,+6);
	FILL_U16(_4976_5d42, 7, -1, 2);
	_4976_5d3e = 0;
	for (si = 0; si < _4976_5d40; si++) {
		//^3E74:239B
		i16 di;
		for (di = 0; di < 7; di++) {
			//^3E74:239F
			if (bp04[si][0] == _4976_4813[RCJ(7,di)]) {
				_4976_5d50[RCJ(7,di)] = bp04[si][1];
				_4976_5d42[di] = _4976_5d3e;
				break;
			}
			//^3E74:23CF
		}
		//^3E74:23D5
		_4976_5d3e += bp04[si][1];
		//^3E74:23E8
	}
	//^3E74:23EF
	_4976_5d38 = reinterpret_cast<U16 *>(bp04[_4976_5d40]);
	LOAD_GDAT_ENTRIES();
// SPX: add here a localized image table as for texts to be able to support multilanguage interface
#if DM2_EXTENDED_MODE == 1
	memset(s_imageLangSel, 0xFF, sizeof(s_imageLangSel));
#endif
	memset(s_textLangSel, 0xFF, sizeof(s_textLangSel));
	//BUILD_GDAT_ENTRY_DATA(&glbGDatEntries, &SkWinCore::_3e74_2162, _4976_4844);
	BUILD_GDAT_ENTRY_DATA(&glbGDatEntries, &SkWinCore::LANG_FILTER, _4976_4844);
	DEALLOC_LOWER_MEMORY(bp08);
	_4976_5d38 = NULL;
	//^3E74:2435
	return;
}
//^3E74:24B8
void SkWinCore::_3e74_24b8()
{
	//^3E74:24B8
	ENTER(38);
	//^3E74:24BE
	U16 si;
	for (si = 0; si < 2; si++) {
		//^3E74:24C3
		if (si > 0 && _4976_5d58 == 0)
			break;
		//^3E74:24D1
		SkEntIter bp26;
		bp26.w0 = 1;
		bp26.w22 = 0;
		bp26.x2.x2.cls1(0xff);
		bp26.x2.x2.cls2(0xff);
		bp26.x2.x2.cls3(fmtSound);
		bp26.x2.x2.cls4(0xff);
		X16 di;
		X16 *bp04;
		if (si > 0) {
			//^3E74:24EF
			bp04 = reinterpret_cast<X16 *>(ALLOC_MEMORY_RAM(_4976_5d58 << 1, afDefault, 0x400));
			di = 0;
		}
		while (QUERY_NEXT_GDAT_ENTRY(&bp26)) {
			//^3E74:250D
			U16 bp08 = bp26.pv14->data;
			if (si == 0) {
				//^3E74:251B
				_4976_5d58++;
				U16 bp06 = QUERY_GDAT_RAW_DATA_LENGTH(bp08);
				if (bp06 > _4976_5c7c)
					//^3E74:2530
					_4976_5c7c = bp06;
				continue;
			}
			//^3E74:2535
			X16 bp0a;
			for (bp0a = 0; bp0a < di; bp0a++) {
				//^3E74:253C
				if (bp04[bp0a] == bp08)
					goto _256a;
				//^3E74:254E
			}
			//^3E74:2556
			bp04[di] = bp08;
			//^3E74:2565
			di++;
			_4976_5cae++;
			//^3E74:256A
_256a:
			continue;
		}
		//^3E74:2579
		if (si > 0) {
			DEALLOC_UPPER_MEMORY(_4976_5d58 << 1);
		}
		//^3E74:258B
	}
	//^3E74:2594
	return;
}

// SPX: Add function to detail important GDAT values, in order to compare to other SkWin versions
void SkWinCore::DEBUG_DISPLAY_GDAT_MAIN_INFO()
{
	unsigned int iItemIndex = 0;
	LOGX(("DEBUG_DISPLAY_GDAT_MAIN_INFO"));
	LOGX(("#Items: %04d", glbGDatNumberOfData));
	for (iItemIndex = 0; iItemIndex < glbGDatNumberOfData; iItemIndex++) 
	{
		LOGX(("ShelfMemTab: %04d => Val = %08X [%d] (%05d)", iItemIndex, glbShelfMemoryTable[iItemIndex].val, glbShelfMemoryTable[iItemIndex].Absent(), (glbShelfMemoryTable[iItemIndex].val & 0x7FFFFFF) ));
	}
	// Max of each category
	unsigned int iCategoryIndex = 0;
	for (iCategoryIndex = 0; iCategoryIndex < 26; iCategoryIndex++) 
	{
		unsigned int iMaxItemsForCategory = 0;
		iMaxItemsForCategory = _3e74_2439(iCategoryIndex, 0x0B);
		LOGX(("Category: %02d => # %03d", iCategoryIndex, iMaxItemsForCategory ));
	}
	LOGX(("============================"));

}

#define MIN(a,b) ( (a < b) ? a : b )

void SkWinCore::DEBUG_DUMP_ULP()
{
	unsigned int iItemNumber = 0;
LOGX(("============================\n"));
LOGX(("ULP >> "));
	for (iItemNumber = 0; iItemNumber < glbGDatNumberOfData; iItemNumber++)
	{
		char sInfoPointer[8];
		char sData[128];
		U32 val = 0;
		memset(sInfoPointer, 0, 8);
		memset(sData, 0, 128);
		//shelf_memory *glbShelfMemoryTable
		//u_lp* p = dm2_ulp.getadr(iItemNumber);
		shelf_memory* p = &glbShelfMemoryTable[iItemNumber];
		val = glbShelfMemoryTable[iItemNumber].val;
		//shelf_memory();
		if (p != NULL && (void*) p < (void*) 0x70000000)	// assume else it is not valid
		{
			Bit8u* xDataPointer = NULL;
			unsigned int iLength = 0;
			iLength = QUERY_GDAT_RAW_DATA_LENGTH(iItemNumber);
			iLength= iLength & 0x7FFFFFFF;

			
			if (val & 0x80000000) // not a direct pointer
			{
				unsigned int iMemOffset = val & 0x7FFFFFFF;
				sprintf(sInfoPointer, "(np)");
			}
			else
			{
				xDataPointer = REALIZE_GRAPHICS_DATA_MEMORY(glbShelfMemoryTable[iItemNumber]);
			}
			if (xDataPointer != NULL) // we have a valid data pointer
			{
				unsigned int iMaxBytesToDisplay = MIN(16, iLength);
				unsigned char* pData;
				char* sWriteData = (char*) sData;

				pData = (unsigned char*) xDataPointer;
				for (unsigned int iByte = 0; iByte < iMaxBytesToDisplay; iByte++)
				{
					sprintf(sWriteData, "%02X ", pData[iByte]);
					pData++;
					sWriteData+=3;
				}

			}
			
			
			LOGX(("u_lp #%04d: %08x || x32 = %08x %4s -> len = %05d (%04X) || i8* = %08X  || Data = %s", iItemNumber, p, val, sInfoPointer, iLength, iLength, xDataPointer, sData)); 
				
		}
		else
			LOGX(("p %04d: %08x ", iItemNumber, p));
	}
LOGX(("============================\n"));
}



const char* SkWinCore::DEBUG_SKLOADENT(U8* xSkLoadEnt)
{
	static char xStaticDebugLoadEnt[32];
	unsigned int iCls1Category = 0;
	unsigned int iCls2 = 0;
	unsigned int iCls3 = 0;
	unsigned int iCls4 = 0;
	memset(xStaticDebugLoadEnt, 0, 32);

	iCls1Category = xSkLoadEnt[2];
	iCls2 = xSkLoadEnt[3];
	iCls3 = xSkLoadEnt[4];
	iCls4 = xSkLoadEnt[5];


	sprintf(xStaticDebugLoadEnt, "%02X-%02X-%02X-%02X", iCls1Category, iCls2, iCls3, iCls4);
	return xStaticDebugLoadEnt;
}



const char* SkWinCore::DEBUG_SKGDATENT(SkEntIter* xSkGDATEnt)
{
	static char xStaticDebugGDATEnt[128];
	char sLoadEntFrom[32];
	char sLoadEntTo[32];
	unsigned int iValue32 = 0;
	unsigned char* xRawPointer = NULL;

	memset(sLoadEntFrom, 0, 32);
	memset(sLoadEntTo, 0, 32);
	memset(xStaticDebugGDATEnt, 0, 128);

	iValue32 = xSkGDATEnt->w0;
	xRawPointer = (unsigned char*) xSkGDATEnt->pv14;
	sprintf(sLoadEntFrom, "%s", DEBUG_SKLOADENT((U8*)&xSkGDATEnt->x2));
	sprintf(sLoadEntTo, "%s", DEBUG_SKLOADENT((U8*)&xSkGDATEnt->x8));

	sprintf(xStaticDebugGDATEnt, "%s to %s -=- x32 = %08x / pRaw = %08x", sLoadEntFrom, sLoadEntTo, iValue32, xRawPointer);
	return xStaticDebugGDATEnt;

}

//^3E74:2641
void SkWinCore::READ_GRAPHICS_STRUCTURE()
{
	//^3E74:2641
	ENTER(12);
	//^3E74:2647
	_4976_5d10 = 0;
//printf("GRAPHICS_DATA_OPEN\n");
	GRAPHICS_DATA_OPEN();
	skfh4 bp0c;
//printf("READ_FILE\n");
	if (READ_FILE(glbFileHandleGraphics1, 4, &bp0c) == 0)	// Read the first 4 bytes of GDAT, which hold GDAT signature + nb of data items
		goto _28d2;
	if ((bp0c.w0 & 0x8000) == 0)
		goto _28d2;
//printf("3E74:2677\n");
	//^3E74:2677
	glbGDATVersion = bp0c.w0 & 0x7fff;
	glbGDatNumberOfData = bp0c.w2;

	U16 *bp04;
	X32 bp08;
	if (glbGDATVersion != 4 && glbGDATVersion != 5 && glbGDATVersion != 2)
		goto _28d2;
	//^3E74:269E
	glbShelfMemoryTable = reinterpret_cast<shelf_memory *>(ALLOC_MEMORY_RAM(sizeof(shelf_memory) * U32(glbGDatNumberOfData), afUseUpper, 0x400));
	_4976_5c82 = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(U32(glbGDatNumberOfData) << 1, afUseUpper, 0x400));
	FILL_U16(reinterpret_cast<i16 *>(_4976_5c82), glbGDatNumberOfData, -1, 2);
	bp08 = U32(glbGDatNumberOfData) << 1;	// nb items * 2 = nb of bytes for all item sizes
	bp04 = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(bp08, afDefault, 0x400));
	_4976_5d6a = bp08 +4;	// nb items sizes + (magic number + nb items) = offset of first raw data which must be the ENT1
	if (glbGDATVersion < 3) {
		if (READ_FILE(glbFileHandleGraphics1, bp08, bp04) == 0)
			goto _28d2;
		//^3E74:2755
		_4976_5d7a = *bp04;
		_4976_5d6a += _4976_5d7a;
	}
	else {
		//^3E74:2775
		if (READ_FILE(glbFileHandleGraphics1, 4, &_4976_5d7a) == 0)	// Read the next 4 bytes of GDAT which hold the size for the first item entry which must be the ENT1 item
			goto _28d2;
		//^3E74:2790
		_4976_5d6a += _4976_5d7a +2;	// there _4976_5d6a gets the offset of the second item after ENT1 item
		if (READ_FILE(glbFileHandleGraphics1, bp08 -2, &bp04[1]) == 0)	// here read the size table before the ENT1 item (except the first item already read, which is exceptionnally on 4 bytes; all others are on 2 bytes max)
			goto _28d2;
	}
	//^3E74:27CF
	bp04[0] = 0;
	_4976_5caa = _4976_5d6a;
	U16 si;
	for (si = 0; si < glbGDatNumberOfData; bp04++, si++) {
		//^3E74:27E9
		_4976_5caa += *bp04;
		glbShelfMemoryTable[si] = shelf_memory::FromSizeOnUnloaded(*bp04);
		//printf("Shelf memory %05d: size = %06d\n", si, glbShelfMemoryTable[si].val & 0x7fff);
		//^3E74:2813
	}
	//^3E74:281E
	_4976_5cea = GET_FILE_SIZE(glbFileHandleGraphics1);
	if (_4976_5cea < _4976_5caa) {
		//^3E74:283D
		_4976_5ca8 = 1;
		_4976_5c9c = 1;
	}
	//^3E74:2846
	DEALLOC_UPPER_MEMORY(bp08);
	LOAD_ENT1();
	if (glbGDATVersion >= 2 && glbGDATVersion != 4 && QUERY_GDAT_ENTRY_DATA_INDEX(0x0, 0x0, dt08, 0x0) != 0xffff) {
		//^3E74:2878
        _4976_5d0c = reinterpret_cast<sk5d0c *>(ALLOC_MEMORY_RAM(
			_4976_5d78 = QUERY_GDAT_ENTRY_DATA_LENGTH(0x0, 0x0, dt08, 0x0),
			afUseUpper, 0x400));
		LOAD_GDAT_ENTRY_DATA_TO(0x0, 0x0, dt08, 0x0, reinterpret_cast<U8 *>(_4976_5d0c));
		_4976_5d78 = _4976_5d78 / 4;
	}

//DEBUG_DISPLAY_GDAT_MAIN_INFO();

	//^3E74:28C6
	_4976_5cb0 = 1;
	_3e74_24b8();
	//^3E74:28D0
	return;
	//^3E74:28D2
_28d2:
	RAISE_SYSERR(SYSTEM_ERROR__INVALID_GRAPHICS_STRUCTURE);
	//^3E74:28DA
	return;
}

//^01B0:1ED2
X16 SkWinCore::IS_SCARD_PRESENT() //#DS=04BF
{
	//^01B0:1ED2
	ENTER(0);
	//^01B0:1ED5
	LOADDS(0x3083);
	return (glbSoundCardType != 0) ? 1 : 0;
}

//^01B0:1BE8
// SPX: _01b0_1be8 renamed _RELOAD_SOUND_BUFFER
X16 SkWinCore::_RELOAD_SOUND_BUFFER(U8 *buff, X16 buffSize, X16 playbackRate) //#DS=04BF
{
	//^01B0:1BE8
	ENTER(0);
	//^01B0:1BEC
	LOADDS(0x3083);
	if (buff == NULL) {
		//^01B0:1C03
		_01b0_18d3(0);
		sndSoundInBuffer = 0;
	}
	else if (buffSize != 0) {
		//^01B0:1C1C
		sndSoundBufferSize = buffSize;
		sndSoundBuffer = buff;
		sndSoundPlaybackFrequency = playbackRate;
		U16 si;
		for (si = 0; si < buffSize; si++) {
			//^01B0:1C39
			sndSoundBuffer[si] = 0;
			//^01B0:1C51
		}
		//^01B0:1C57
        sndSoundToPlayBuffer = buff;
		sndSoundToPlayBufferSize = buffSize;
		if (glbSoundCardType == 3 || glbSoundCardType == 5 || glbSoundCardType == ScardSBlaster)
			//^01B0:1C80
			sndSoundInBuffer = 1;
	}
	//^01B0:1C86
	return sndSoundInBuffer;
}

//^47EB:0003
void SkWinCore::_47eb_0003()
{
	//^47EB:0003
	ENTER(0);
	//^47EB:0006
	_4976_4838 = X8(_01b0_2b1b() CALL_IBMIO);
	if (IS_SCARD_PRESENT() CALL_IBMIO != 0) {
		//^47EB:001F
		_RELOAD_SOUND_BUFFER(
			ALLOC_MEMORY_RAM(_4976_5c7c, afUseUpper, 0x400),
			_4976_5c7c, PLAYBACK_FREQUENCY
			) CALL_IBMIO;
	}
	//^47EB:0046
	return;
}

//^482B:0004
void SkWinCore::_482b_0004()
{
	//^482B:0004
	ENTER(4);
	//^482B:0008
	_4976_5f0a = reinterpret_cast<sk5f0a *>(ALLOC_MEMORY_RAM(_4976_5cae << 4, afUseUpper, 0x400));
	_4976_5f06 = reinterpret_cast<sk5f06 *>(ALLOC_MEMORY_RAM(_4976_5d58 * 7, afUseUpper, 0x400));
	_4976_5f02 = reinterpret_cast<SoundStructure *>(ALLOC_MEMORY_RAM(0xf0, afUseUpper, 0x400));
	U32 bp04;
	glbSoundList = reinterpret_cast<SoundEntryInfo *>(ALLOC_MEMORY_RAM(bp04 = 0x60, afUseUpper, 0x400));
	ZERO_MEMORY(glbSoundList, bp04);
	_4976_5efe = reinterpret_cast<SoundStructure *>(ALLOC_MEMORY_RAM(0x48, afUseUpper, 0x400));
	_47eb_0003();
	//^482B:00BD
	return;
}
//^32CB:0008
// SPX: _32cb_0008 renamed LOAD_GDAT_INTERFACE_00_0A
void SkWinCore::LOAD_GDAT_INTERFACE_00_0A()
{
	//^32CB:0008
	ENTER(4);
	//^32CB:000C
	U32 bp04;
	// SPX: This points to a 1652 bytes file .. seems to have struct of 14 bytes => 118 records.
	_4976_5a98 = reinterpret_cast<U8 (*)[14]>(ALLOC_MEMORY_RAM(bp04 = QUERY_GDAT_ENTRY_DATA_LENGTH(GDAT_CATEGORY_INTERFACE_GENERAL, 0x0, dt07, 0xa), afUseUpper, 0x400));
	LOAD_GDAT_ENTRY_DATA_TO(GDAT_CATEGORY_INTERFACE_GENERAL, 0x0, dt07, 0xa, reinterpret_cast<U8 *>(_4976_5a98));
	//^32CB:0052
	return;
}
//^0B36:020E
// SPX: _0b36_020e renamed LOAD_GDAT_INTERFACE_00_02
void SkWinCore::LOAD_GDAT_INTERFACE_00_02()
{
	//^0B36:020E
	ENTER(6);
	//^0B36:0214
	U8 *bp04 = ALLOC_MEMORY_RAM(QUERY_GDAT_ENTRY_DATA_LENGTH(GDAT_CATEGORY_INTERFACE_GENERAL, 0x0, dt07, 0x2), afUseUpper, 0x400);
	LOAD_GDAT_ENTRY_DATA_TO(GDAT_CATEGORY_INTERFACE_GENERAL, 0x0, dt07, 0x2, bp04);
	X8 bp05 = *bp04++;
	_4976_4bde = reinterpret_cast<sk4bde *>(ALLOC_MEMORY_RAM(bp05 * 9UL, afUseUpper, 0x400));
	i16 si;
	for (si = 0; bp05 > si; bp04++, si++) {
		//^0B36:0282
		_4976_4bde[si].b0 = *bp04;
		//^0B36:029A
	}
	//^0B36:02A7
	for (si = 0; bp05 > si; si++) {
		//^0B36:02AB
		_4976_4bde[si].pv1 = bp04;
		bp04 += _4976_4bde[si].b0;
		//^0B36:02DB
	}
	//^0B36:02E5
	for (si = 0; bp05 > si; si++) {
		//^0B36:02E9
		_4976_4bde[si].pv5 = bp04;
		bp04 += _4976_4bde[si].b0;
		//^0B36:0319
	}
	//^0B36:0323
	_4976_4be2 = reinterpret_cast<sk4be2 *>(bp04);
	//^0B36:0330
	return;
}
//^4937:0320
// SPX: _4937_0320 renamed LOAD_GDAT_INTERFACE_00_00
// This function loads one item which contains 3 data tables. These tables are about creature animations.
// The 1st table contains animation sequences, with associated sounds.
//	Each animation begins with a special bits telling the start of a new animation.
// The 2nd table describe all possible action animations for a creature behaviour.
//	Each action animation points to an animation sequence.
//	Each creature behaviour is finished by a special record using end marker.
// The 3rd table contains a list of pointers which start to a creature behaviour description.
void SkWinCore::LOAD_GDAT_INTERFACE_00_00()
{
	//^4937:0320
	ENTER(8);
	//^4937:0324
	U8 *bp04 = ALLOC_MEMORY_RAM(
		QUERY_GDAT_ENTRY_DATA_LENGTH(GDAT_CATEGORY_INTERFACE_GENERAL, 0x0, dt06, 0x0),
		afUseUpper, 0x400);
	LOAD_GDAT_ENTRY_DATA_TO(GDAT_CATEGORY_INTERFACE_GENERAL, 0x0, dt06, 0x0, bp04);
	//^4937:0360
	X32 bp08 = READ_UI32(bp04,+0);	// Read first DWORD of data, which is length of the first part : 0xC24 = 3108 = 6*518
	bp04 += 4;
	tlbCreaturesAnimationSequences = reinterpret_cast<CreatureAnimationFrame *>(bp04);	// This points to the first part
	bp04 += bp08;
	//^4937:0387
	bp08 = READ_UI32(bp04,+0);	// 0x760 = 1888 = 4*472
	bp04 += 4;
	tlbCreaturesActionsGroupSets = reinterpret_cast<CreatureCommandAnimation *>(bp04);	// This points to the second part
	bp04 += bp08;
	//^4937:03AE
	bp08 = READ_UI32(bp04,+0);	// 0x54 = 84 = 2*42
	bp04 += 4;
	tlbCreaturesActionsGroupOffsets = reinterpret_cast<U16 *>(bp04);		// This points to the third part
	//^4937:03CF
	return;
}
//^38C8:00C8
void SkWinCore::_38c8_00c8()
{
	//^38C8:00C8
	ENTER(0);
	//^38C8:00CB
	_4976_4c16 = _4726_02ac();
	if (_4976_4c16 == NULL) {
		//^38C8:00DB
		_4976_4c16 = ALLOC_PICT_BUFF(_4976_00f6, _4976_00f8, afUseUpper, 8);
	}
	//^38C8:00F6
	_4976_5ca0 = 0xB798;
	//^38C8:0102
	return;
}

//^3929:0CA8
void SkWinCore::KANJI_FONT_LOAD(X8 cls2)
{
	//^3929:0CA8
	ENTER(660);
	//^3929:0CAE
	X16 bp0e = 0;
	X16 bp10 = 0x20;
	X16 si = 0xef;
	skxxxf bp0294[64];
	skxxxf *bp08 = bp0294;
	U8 bp0b;
	U8 *bp04;
	for (bp0b = 0; (bp04 = QUERY_GDAT_ENTRY_DATA_BUFF(GDAT_CATEGORY_JAPANESE_FONT, cls2, dtImage, bp0b)) != NULL; ) {
		//^3929:0CCC
		X16 bp0a = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_JAPANESE_FONT, cls2, dtWordValue, bp0b);
		if (bp0a != 0) {
			//^3929:0CE7
			bp0e = bp0a >> 8;
			bp10 = U8(bp0a);
		}
		//^3929:0CF5
		bp0a = QUERY_GDAT_PICT_OFFSET(0x1c, cls2, bp0b);
		bp08->b0 = X8(bp0e);
		bp08->b1 = X8(bp10);
		bp08->w6 = i8(bp0a >> 8);
		bp08->w2 = i16(READ_UI16(bp04,+0) & 0x3ff) / bp08->w6;
		bp08->w8 = i8(bp0a);
		bp08->w4 = i16(READ_UI16(bp04,+2) & 0x3ff) / bp08->w8;
		bp0b++;
		if (bp0b >= 0x40)
			continue;
		//^3929:0D69
		bp10 += bp08->w2 * bp08->w4;
		if (bp10 > si) {
			//^3929:0D7E
			if (bp0e == 0) {
				//^3929:0D84
				bp0e = 0xef;
				si = 0xff;
			}
			//^3929:0D8C
			bp0e++;
			bp10 = 0x20;
		}
		bp08++;
		//^3929:0D98
	}
	//^3929:0DB9
	_4976_5bf8[cls2] = bp0b;
	U32 bp14 = bp0b * 10;
	COPY_MEMORY(
		bp0294, 
		_4976_5bfa[cls2] = reinterpret_cast<skxxxf *>(ALLOC_MEMORY_RAM(bp14, afUseUpper, 0x400)),
		bp14
		);
	//^3929:0E12
	return;
}
//^470A:0003
// TODO: this does nothing ?!
void SkWinCore::_470a_0003()
{
	//^470A:0003
	ENTER(0);
	//^470A:0006
	//^470A:0006
	return;
}

//^3929:0E16
void SkWinCore::_3929_0e16()
{
	//^3929:0E16
	ENTER(8);
	//^3929:0E1B
	_3929_07e1(0, 0);
	_4976_5c08 = ALLOC_PICT_BUFF(_4976_013e, _4976_0140, afUseUpper, 8);
	_4976_5c0e = ALLOC_MEMORY_RAM(0x300, afUseUpper, 0x400);
	LOAD_GDAT_ENTRY_DATA_TO(0x1, 0x0, dt07, 0x0, _4976_5c0e);
	U16 si;
	for (si = 0; si < 1; si++) {
		//^3929:0E75
		//tlbTimerTickRemoveHintMessage[si] = 0xffffffff;
		tlbTimerTickRemoveHintMessage[si] = -1;
		//^3929:0E86
	}
	//^3929:0E8C
	SRECT bp08;
	QUERY_EXPANDED_RECT(RECT_BOTTOM_MESSAGE_3_LINES, &bp08); // 00 00|B4 00|40 01|14 00 (0,180,320,20)
	_4976_5c12 = (bp08.cy - (_4976_013a - _4976_0134)) >> 1;
	KANJI_FONT_LOAD(1);
	_470a_0003();
	_4976_4750 = 1;
	//^3929:0EBF
	return;
}
//^2405:0009
void SkWinCore::_2405_0009()
{
	//^2405:0009
	ENTER(0);
	//^2405:000C
	glbLeaderHandPossession.pb2 = ALLOC_PICT_BUFF(_4976_0106, _4976_0108, afUseUpper, 4);
	//^2405:0027
	return;
}

//^01B0:08B6
void SkWinCore::_01b0_08b6(U16 (SkWinCore::*pfn)(U16 xx, U16 yy, i16 zz)) //#DS=04BF
{
	//^01B0:08B6
	ENTER(0);
	//^01B0:08B9
	LOADDS(0x3083);
	_04bf_179e = pfn;
	_04bf_03c6 = 1;
	//^01B0:08D6
	return;
}

//^443C:0380
void SkWinCore::_443c_0380()
{
	//^443C:0380
	ENTER(0);
	//^443C:0383
	IBMIO_SET_CURSOR_PATTERN(0, _4976_4862, 0, 0, 12, 16, 4, glbPaletteT16, 12) CALL_IBMIO;
	IBMIO_SET_CURSOR_PATTERN(1, _4976_48c2, 0, 0, 16, 16, 4, glbPaletteT16, 12) CALL_IBMIO;
	_01b0_0c70(0);
	_01b0_08b6(&SkWinCore::FIRE_MOUSE_EVENT_RECEIVER);
	//^443C:03F2
	return;
}

//^443C:067A
void SkWinCore::_443c_067a(sk0cea *ref)
{
	//^443C:067A
	ENTER(0);
	//^443C:067D
	if (ref->b3_6_6() == 0) {
		//^443C:068C
		ref->b3_6_6(1);
		ref->pv6(reinterpret_cast<skxxx7 *>(ALLOC_MEMORY_RAM(sizeof(skxxx7), afUseUpper, 0x200)));
	}
	//^443C:06AD
	return;
}

//^1031:07D6
void SkWinCore::_1031_07d6()
{
	//^1031:07D6
	ENTER(194);
	//^1031:07DB
	X16 si = 0;
	i16 bp02;
	X16 bp0084[61];
	X16 bp00a8[18];
	X8 bp00c2[26];
	for (bp02 = 0; bp02 < 0x101; bp02++) {	// 257
		//^1031:07E4
		if ((_4976_0d9e[bp02].w0 & 0x8000) != 0) {
			//^1031:07F6
			bp0084[RCJ(61,si)] = bp02;
			si++;
		}
		//^1031:0807
	}
	//^1031:0811
	si = 0;
	for (bp02 = 0; bp02 < 0x74; bp02++) {	// 116
		//^1031:081A
		if ((_4976_13a4[bp02].w0 & 0x8000) != 0) {
			bp00a8[RCJ(18,si)] = bp02;
			si++;
		}
		//^1031:0839
	}
	//^1031:0842
	si = 0;
	for (bp02 = 0; bp02 < 0x51; bp02++) {	// 81
		//^1031:084B
		if ((_4976_169c[bp02] & 0x80) != 0) {
			bp00c2[RCJ(26,si)] = X8(bp02);
			si++;
		}
		//^1031:085D
	}
	//^1031:0866
	for (bp02 = 0; bp02 < 0x4a; bp02++) {	// 74
		//^1031:086D
		sk1891 *bp06 = &_4976_1574[bp02];
		if ((bp06->b0 & 0x80) != 0) {
			//^1031:0885
			bp06->w2 = bp00c2[RCJ(26,bp06->w2)];
		}
		//^1031:089B
	}
	//^1031:08A4
	for (bp02 = 0; bp02 < 0xa; bp02++) {	// 10
		//^1031:08AB
		sk1891 *bp06 = &_4976_1891[bp02];
		if ((bp06->b0 & 0x80) != 0) {
			bp06->w2 = bp00c2[RCJ(26,bp06->w2)];
		}
		//^1031:08D9
	}
	//^1031:08E2
	for (bp02 = 0; bp02 < 0x3c; bp02++) {	// 60
		//^1031:08E9
		sk16ed *bp0a = &_4976_16ed[bp02];
		if (bp0a->w2 != 0xffff) {
			//^1031:0904
			bp0a->w2 = bp0084[RCJ(61,bp0a->w2)];
		}
		//^1031:091A
		if (bp0a->w4 != 0xffff) {
			//^1031:0924
			bp0a->w4 = bp00a8[RCJ(18,bp0a->w4)];
		}
		//^1031:093A
	}
	//^1031:0943
	for (bp02 = 0; bp02 < 0x12; bp02++) {	// 18
		//^1031:094A
		_443c_067a(&_4976_0ce0[1 +bp02]);
		//^1031:095E
	}
	//^1031:0967
	return;
}
//^3E74:2439
U8 SkWinCore::_3e74_2439(X8 cls1, X8 cls4)
{
	//^3E74:2439
	ENTER(0);
	//^3E74:243D
	if (cls1 > U8(glbGDatEntries.w12))
		return 0;
	//^3E74:244C
	X16 si = glbGDatEntries.pw0[cls1];
	if (glbGDatEntries.pw0[cls1 +1] -si <= cls4)
		return 0;
	//^3E74:247A
	return glbGDatEntries.pv8[U32(glbGDatEntries.pw4[cls4 +si +1]) -1].cls2;
}

//^2481:0002
void SkWinCore::_2481_0002()
{
	//^2481:0002
	ENTER(0);
	//^2481:0005
	FIRE_SELECT_PALETTE_SET(0);
	if (_4976_3d2c != 0) {
		//^2481:0014
		FIRE_BLIT_PICTURE(
			glbImageMenuScreen, _4976_4964, ALLOC_TEMP_ORIGIN_RECT(glbScreenWidth, glbScreenHeight),
			0, 0, glbScreenWidth, glbScreenWidth, -1, 0, 8, 8, NULL);
	}
	else {
		//^2481:0057
		DRAW_GAMELOAD_DIALOGUE_TO_SCREEN(glbImageMenuScreen, 2, -1, _4976_52ba);
	}
	//^2481:0073
	FIRE_SELECT_PALETTE_SET(1);
	//^2481:007B
	return;
}
//^3E74:0A77
void SkWinCore::_3e74_0a77(U8 *ref)
{
	//^3E74:0A77
	ENTER(0);
	//^3E74:0A7A
	if (_4976_5d76 != 0)
		return;
	//^3E74:0A81
	if (READ_UI16(ref,-6+4) == 0) {
		//^3E74:0A9D
		DEALLOC_UPPER_MEMORY(READ_UI32(ref,-6));
	}
	else {
		//^3E74:0ABF
		DEALLOC_LOWER_MEMORY(READ_UI32(ref,-6));
	}
	//^3E74:0AE1
	return;
}

//^2481:007D
// SPX: Main loop, display title&menu screen
// _2481_007d renamed SHOW_MENU_SCREEN
void SkWinCore::SHOW_MENU_SCREEN()
{
	//^2481:007D
	ENTER(0);

	REQUEST_PLAY_MUSIC(100);	// SPX: title music

	//^2481:0080
	glbImageCreditScreen = QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_TITLE, 0x0, 0x1);		// Credit screen (tombstone)
	if (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_TITLE, 0x0, dt07, 0x4) != 0xffff)	// 64000 bytes raw data for menu screen ?
	{
		//^2481:00AA
		_4976_3d2c = 1;
		glbImageMenuScreen = QUERY_GDAT_ENTRY_DATA_PTR(GDAT_CATEGORY_TITLE, 0x0, dt07, 4);
	}
	else {
		//^2481:00C9
		glbImageMenuScreen = QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_TITLE, 0x0, 0x4);
		_4976_52ba = glbImageMenuScreen +((READ_UI16(glbImageMenuScreen,-6) == 4)
			? (((READ_UI16(glbImageMenuScreen,-4) +1) & 0xfffe) >> 1)
			: (  READ_UI16(glbImageMenuScreen,-4)     & 0xffff)
			) * READ_UI16(glbImageMenuScreen,-2);
	}
	//^2481:0116
	_4976_52b6 = glbImageCreditScreen +((READ_UI16(glbImageCreditScreen,-6) == 4)
		? (((READ_UI16(glbImageCreditScreen,-4) +1) & 0xfffe) >> 1)
		: (  READ_UI16(glbImageCreditScreen,-4)     & 0xffff)
		) * READ_UI16(glbImageCreditScreen,-2)
		;
	GRAPHICS_DATA_CLOSE();
	_1031_0541(0);
	//^2481:015B
	do {
		//printf("SHOW_MENU_SCREEN Loop ...\n");
		_2481_0002();
		FIRE_SHOW_MOUSE_CURSOR();
		_1031_098e();
		glbSpecialScreen = _MENU_SCREEN__TITLE_MENU;	// 0x63
		if (skwin.newgame == 1)
			glbSpecialScreen = _MENU_SCREEN__LOAD_NEW_GAME;
		else if (skwin.newgame == 2)
		{
			glbSpecialScreen = _MENU_SCREEN__RESUME_GAME_SELECT;
			skwin.newgame = 0;
		}
		goto _0180;
		//^2481:0171
_0171:
		_1031_0d36(0x20, _476d_050e());
_0180:
		do {
			MessageLoop(true); // main menu

			//^2481:0180
			if (_476d_05a9() != 0)
				goto _0171;
			MAIN_LOOP();
			//^2481:018E
		} while (glbSpecialScreen == _MENU_SCREEN__TITLE_MENU);	// 0x63
		//^2481:0195
	} while (glbSpecialScreen == _MENU_SCREEN__SHOW_CREDITS); // 0xDA
	//^2481:019D
	FIRE_HIDE_MOUSE_CURSOR();
	if (_4976_3d2c != 0) {
		//^2481:01A9
		_3e74_0a77(glbImageMenuScreen);
	}
	else {
		//^2481:01B8
		FREE_PICT_ENTRY(glbImageMenuScreen);
	}
	//^2481:01C5
	FREE_PICT_ENTRY(glbImageCreditScreen);
	//^2481:01D6
	return;
}


//^3E74:07B2
void SkWinCore::_3e74_07b2()
{
	//^3E74:07B2
	ENTER(4);
	//^3E74:07B6
	sk5d00 *bp04 = _4976_5d00;
	EMS_MAP_BANK_TO_MEMORY();
	//^3E74:07D1
	while (bp04 != NULL) {
		//^3E74:07D3
		if (bp04->w18 == 0) {
			bp04->dw20 = bp04->dw8;
			bp04->dw24 = bp04->dw12;
		}
		//^3E74:07FD
		bp04 = bp04->pv0;
		//^3E74:080D
	}
	//^3E74:081E
	return;
}

//^3E74:0820
void SkWinCore::_3e74_0820()
{
	//^3E74:0820
	ENTER(4);
	//^3E74:0824
	sk5d00 *bp04 = _4976_5d00;
	EMS_MAP_BANK_TO_MEMORY();
	//^3E74:083F
	while (bp04 != NULL) {
		//^3E74:0841
		if (bp04->w18 == 0) {
			//^3E74:084B
			bp04->dw8 = bp04->dw20;
			bp04->dw12 = bp04->dw24;
		}
		//^3E74:086B
		bp04 = bp04->pv0;
	}
	//^3E74:088C
	return;
}
//^38C8:0109
X16 SkWinCore::_38c8_0109(X8 **buff, X32 *xx, X16 *yy)
{
	//^38C8:0109
	ENTER(6);
	//^38C8:010D
	X8 *bp04 = *buff;
	X8 bp05 = 0;
	//^38C8:0121
	while (bp05 < 4) {
		//^38C8:0123
		_4976_5bee[bp05] = 0; bp05++;
	}
	//^38C8:0138
	*xx = 0;
	*yy = 0;
	if (_4976_474e != 0) {
		//^38C8:0158
		_4976_474e = 0;
		_4976_5bf4 = 0;
		_4976_5be8 = 0;
		//^38C8:016A
		while ((bp05 = *(bp04++)) != 0xff) {
			//^38C8:016C
			if (bp05 == 0xfe)
				//^38C8:0172
				_4976_5bf4++;
		}
		//^38C8:0186
		bp04 = *buff;
	}
	while ((bp05 = *(bp04++)) != 0xfe) {
		//^38C8:0198
		if (bp05 == 0xff)
			//^38C8:019E
			return 0;
		//^38C8:01A3
		if (bp05 == 0xfd) {
			//^38C8:01A9
#if UseAltic
			ATLASSERT(false);
			*xx = 0;
#else
			*xx = U32(bp04) << 10;
#endif
			bp04++;
			continue;
		}
		//^38C8:01C6
		if (bp05 == 0xfc) {
			//^38C8:01CC
#if UseAltic
			ATLASSERT(false);
			*yy = 0;
#else
			*yy = U16(bp04) << 4;
#endif
			bp04++;
			continue;
		}
		//^38C8:01E2
		_4976_5bee[bp05] = 1;
		//^38C8:01EE
	}
	//^38C8:01FE
	_4976_5be8++;
	*buff = bp04;
	if (*bp04 != 0xff)
		//^38C8:021B
		return 1;
	//^38C8:0220
	return 0;
}
//^38C8:0104
void SkWinCore::_38c8_0104()
{
	//^38C8:0104
	ENTER(0);
	//^38C8:0107
	return;
}
//^3E74:2EC7
void SkWinCore::INIT_CPXHEAP(sk5d12 *ref, tiamat poolBuff, U32 poolSize, U16 poolflag)
{
	//^3E74:2EC7
	ENTER(0);
	//^3E74:2ECA
	ref->t0 = (poolBuff);
	ref->t4 = (poolBuff);
	ref->t8 = (poolBuff);
	//^3E74:2EEE
	if (poolBuff.IsntZero()) {
		//^3E74:2EF9
		if ((poolflag & 0x800) != 0) {
			//^3E74:2F00
			ref->t4 = (poolBuff -poolSize); // integer subtraction.
		}
		else {
			//^3E74:2F19
			ref->t4 = (poolBuff -poolSize); // pointer subtraction.
		}
	}
	//^3E74:2F35
	ref->w12 = poolflag;
	//^3E74:2F3F
	return;
}
//^38C8:0224
X16 SkWinCore::_38c8_0224(X16 xx, i32 yy)
{
	//printf("Entering _38c8_0224 ...\n"); getch();
	//^38C8:0224
	ENTER(6);
	//^38C8:022A
	X16 bp06 = glbPlayerMap;
	_4976_5c8a = 1;
	i32 bp04 = i32(0x7fffffff);
	i16 si;
	i16 di;
	for (si = 0; dunHeader->nMaps; si++) {
		//^38C8:0245
		if ((si & 3) == 0) {
			//^38C8:024B
			DRAW_DIALOGUE_PROGRESS((i32(dunHeader->nMaps * (_4976_5be8 -1) +si +1) * 500) / i32(dunHeader->nMaps * _4976_5bf4));
		}
		//printf("LOAD_NEWMAP %d\n", si); getch();
		//^38C8:0293
		LOAD_NEWMAP(U8(si));
		//^38C8:029A
		if (_4976_5d62 < 0) {
			//^38C8:02AA
			if (_4976_5d62 < bp04) {
				//^38C8:02BD
				bp04 = _4976_5d62;
				di = si;
			}
		}
		//^38C8:02CC
		if (_4976_5d26 -yy < bp04) {
			//^38C8:02E5
			bp04 = _4976_5d26 -yy;
			di = si;
		}
		//^38C8:02FA
		if ((bp04 < 0 && xx == 0) || _4976_5bee != 0)
			break;
		//^38C8:0315
	}
	//^38C8:0327
	if (bp04 > _4976_4742) {
		//^38C8:033B
		_4976_4742 = bp04;
		_4976_4746 = di;
	}
	//^38C8:034C
	if (xx != 0) {
		//^38C8:0352
		if (bp04 < 0) {
			//^38C8:0360
			_4976_4742 = bp04;
			_4976_4746 = di;
			_0aaf_0067(_0aaf_02f8_DIALOG_BOX(2, 0));
			SK_PREPARE_EXIT();
		}
	}
	//printf("Ending _38c8_0224\n"); getch();
	//^38C8:0388
	_4976_5c8a = 0;
	glbPlayerMap = bp06;
	if (bp04 >= 0)
		return 1;
	//^38C8:03A7
	return 0;
}

//^3E74:1330
// SPX _3e74_1330 renamed ALLOC_CPX_SETUP
void SkWinCore::ALLOC_CPX_SETUP(X8 *xx)
{
	//^3E74:1330
	ENTER(26);
	//^3E74:1335
	//printf("ALLOC_CPX_SETUP:t2ptr\n"); getch();
	U8 *bp08 = t2ptr(_4976_5d5a);
	X32 bp10 = glbFreeEMSMemPool;
	U8 *bp0c = _4976_5cf4;
	X32 bp14 = glbFreeRAMMemPool;
	mement *bp04 = _4976_5ce6;
	//printf("ALLOC_CPX_SETUP:_3e74_07b2\n"); getch();
	_3e74_07b2();
	X32 bp18;
	X16 si;
	do {
		//^3E74:137A
		_4976_5d5a = ptr2t(bp08);
		glbFreeEMSMemPool = bp10;
		_4976_5cf4 = bp0c;
		glbFreeRAMMemPool = bp14;
		_4976_5ce6 = bp04;
		//printf("ALLOC_CPX_SETUP:_3e74_0820\n"); getch();
		_3e74_0820();
		X16 bp1a;
		si = _38c8_0109(&xx, &bp18, &bp1a);
		//printf("ALLOC_CPX_SETUP:_38c8_0104\n"); getch();
		_38c8_0104();
		if (i32(bp1a +0x100) > 0xffff) {
			//^3E74:13F3
			glbNumberOfMements = 0xffff;
		}
		else {
			//^3E74:13FB
			glbNumberOfMements = bp1a +0x100;
		}
		//^3E74:1404
		//printf("ALLOC_CPX_SETUP:tlbMementsPointers\n"); getch();
		tlbMementsPointers = reinterpret_cast<mement **>(ALLOC_MEMORY_RAM(glbNumberOfMements << 2, afZeroMem|afUseUpper, 0x400));
		_4976_5d24 = 0x80;
		_4976_5d08 = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(_4976_5d24 << 1, afUseUpper, 0x400));
		FILL_U16(reinterpret_cast<i16 *>(_4976_5d08), _4976_5d24, -1, 2);
		_4976_5c7e = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(_4976_5d24 << 1, afUseUpper, 0x400));
		_4976_5c86 = reinterpret_cast<U32 *>(ALLOC_MEMORY_RAM(_4976_5d24 << 2, afZeroMem|afUseUpper, 0x400));
		_4976_5ce2 = _4976_5cf4 - _4976_00ec;
		//printf("ALLOC_CPX_SETUP:INIT_CPXHEAP\n"); getch();
		INIT_CPXHEAP(&_4976_5d12, ptr2t(_4976_5ce2), 0, _4976_5cee);
		if (glbFreeEMSMemPool < 12) {
			//^3E74:14E7
			INIT_CPXHEAP(&_4976_5d7e, ptr2t(NULL), 0, _4976_5d6e);
		}
		else {
			//^3E74:14F5
			INIT_CPXHEAP(&_4976_5d7e, _4976_5d5a, glbFreeEMSMemPool, _4976_5d6e);
		}
		//printf("ALLOC_CPX_SETUP:bUsePowerDebug\n"); getch();
		//^3E74:1515
#ifndef __DJGPP__		
		if (!SkCodeParam::bUsePowerDebug)	// SPX: I don't get the meaning of this, overriding MEM left value with a low one?
			// then in certain condition it will raise error 40 complaining of no MEM left while there is ...
			glbFreeRAMMemPool = _4976_00ec;
#endif
		_4976_5cf8 = tiamat::Size(ptr2t(_4976_5ce2), ptr2t(reinterpret_cast<U8 *>(_4976_5ce6)));
		//^3E74:153C
		//printf("ALLOC_CPX_SETUP:_38c8_0224\n"); getch();		
	} while (_38c8_0224(!si, bp18) == 0);
	//^3E74:1559
	_4976_5d76 = 1;
	//^3E74:155F
	//printf("ALLOC_CPX_SETUP:Return\n"); getch();
	return;
}

//^00EB:05C7
// SPX: _00eb_05c7 renamed SET_GRAPHICS_RGB_PALETTE
X16 SkWinCore::SET_GRAPHICS_RGB_PALETTE(U8 (*pal)[4], X16 yy) //#DS=04BF
{
	//^00EB:05C7
	ENTER(0);
	//^00EB:05CB
	LOADDS(0x0C48);
	i16 si;
	for (si = 0; si < 0x100; si++) {
		//^00EB:05D5
		glbPaletteRGB[si][0] = pal[si][1] >> 2;
		//^00EB:05F7
		glbPaletteRGB[si][1] = pal[si][2] >> 2;
		//^00EB:0619
		glbPaletteRGB[si][2] = pal[si][3] >> 2;
		//^00EB:063B
	}
	//^00EB:0642
	if (glbUpdatePalette == 1)
		//^00EB:0649
		IBMIO_UPDATE_PALETTE_SET();
	//^00EB:064E
	return 1;
}

//^44C8:1BAF
// SPX: _44c8_1baf renamed SET_RGB_PALETTE_FROM_DATA
void SkWinCore::SET_RGB_PALETTE_FROM_DATA(U8 *pal)
{
	//^44C8:1BAF
	ENTER(0);
	//^44C8:1BB2
	glbPaletteIRGBLoaded = SET_GRAPHICS_RGB_PALETTE(reinterpret_cast<U8 (*)[4]>(pal), 0);
	//^44C8:1BC8
	return;
}

//^38C8:04AA
// SPX: _38c8_04aa renamed INIT
void SkWinCore::INIT()
{
	U16 iCompatibilityFlag = 0;
	U16 iSpecialItem = 0;
	//^38C8:04AA
	ENTER(4);
	//^38C8:04AF
	_4726_02f7();
	_4976_474a = 1;
	_4976_474c = 1;
	_476d_018a();
	_2636_03d4();
//printf("READ_GRAPHICS_STRUCTURE\n");
	READ_GRAPHICS_STRUCTURE();
//printf("_482b_0004\n");
	_482b_0004();
//printf("LOAD_GDAT_INTERFACE_00_0A\n");
	LOAD_GDAT_INTERFACE_00_0A();
	U8 *bp04 = ALLOC_MEMORY_RAM(0x400, afUseLower, 1024);
//DEBUG_DUMP_ULP();
	LOAD_GDAT_ENTRY_DATA_TO(GDAT_CATEGORY_INTERFACE_GENERAL, 0x0, dtPalIRGB, 0xFE, bp04);	// C01=I00=EFE=T009 palette IRGB (0x1, 0x0, dt09, 0xFE, bp04)
	// SPX: Beta GDAT contains palette in raw7 data; if dtPalIRGB is not found, we should then look for raw7.
	//LOAD_GDAT_ENTRY_DATA_TO(GDAT_CATEGORY_INTERFACE_GENERAL, 0x0, dtRaw7, 0xFE, bp04);
	SET_RGB_PALETTE_FROM_DATA(bp04);
	DEALLOC_LOWER_MEMORY(0x400);
	LOAD_GDAT_INTERFACE_00_02();
	//glbPaletteT16 = QUERY_GDAT_ENTRY_DATA_PTR(0x1, 0x0, dt0d, 0xfe);
	glbPaletteT16 = QUERY_GDAT_ENTRY_DATA_PTR(GDAT_CATEGORY_INTERFACE_GENERAL, 0x0, dtPalette16, 0xFE);
	_098d_1208();

//printf("EXTENDED_LOAD_AI_DEFINITION\n");
	// SPX: Added extended load here (requires the GDAT to be initialized, but must be before dungeon loading)
	EXTENDED_LOAD_AI_DEFINITION();
	// SPX: Read compatibility mode value (none = standard DM2)
	iCompatibilityFlag = QUERY_GDAT_ENTRY_DATA_INDEX(0, 0, dtWordValue, 0x10);
	if (iCompatibilityFlag == 3) { // DM1 TQ
		SkCodeParam::bDM1Mode = 1;
		SkCodeParam::bDM1TQMode = 1;
	}
	// SPX: Check item 00-00-01. If existing, it would be a V5, else a V4
	iSpecialItem = QUERY_GDAT_ENTRY_DATA_INDEX(0, 0, dtWordValue, 0x01);
	//-- Set sound playback base frequency depending on GDAT version
	if (iSpecialItem == 6)
		skwin.SndSetFrequency(11025);
	else
		skwin.SndSetFrequency(6000);

	
//printf("LOAD_GDAT_INTERFACE_00_00\n");
	LOAD_GDAT_INTERFACE_00_00();
	_38c8_00c8();
	_3929_0e16();
	glbTextEntryEncoded = QUERY_GDAT_ENTRY_DATA_INDEX(0x0, 0x0, dtWordValue, 0) & 8;	// Value is 0B ..
	_2405_0009();
	_443c_0380();
	_4976_4748 = 1;
	_1031_07d6();
	glbCreaturesMaxCount = _3e74_2439(0xf, 0xb);
	X16 si = (glbCreaturesMaxCount +1) * 3;
	glbSomeCreatureTable = ALLOC_MEMORY_RAM(si, afUseUpper, 1024);
	FILL_STR(glbSomeCreatureTable, si, 0xff, 1);
	//^38C8:05C1
	while (SHOW_MENU_SCREEN(), GAME_LOAD() != 1) {
		//^38C8:05C3
		GRAPHICS_DATA_OPEN();
	}
	//^38C8:05D7
	//printf("INIT:GRAPHICS_DATA_OPEN\n"); getch();
	GRAPHICS_DATA_OPEN();
	//printf("INIT:__LOAD_CREATURE_FROM_DUNGEON\n"); getch();
	__LOAD_CREATURE_FROM_DUNGEON();
	//printf("INIT:ALLOC_CPX_SETUP\n"); getch();
	ALLOC_CPX_SETUP(_4976_4736);
	//printf("INIT:__INIT_GAME_38c8_03ad\n"); getch();
	__INIT_GAME_38c8_03ad();
	//printf("INIT:GRAPHICS_DATA_CLOSE\n"); getch();
	GRAPHICS_DATA_CLOSE();
	if (glbSpecialScreen != 0) {
		//^38C8:05FC
		MOVE_RECORD_TO(OBJECT_NULL, -1, 0, glbPlayerPosX, glbPlayerPosY);
	}
	//^38C8:0612
	//printf("INIT:FIRE_SHOW_MOUSE_CURSOR\n"); getch();
	FIRE_SHOW_MOUSE_CURSOR();
	//printf("INIT:_1031_098e\n"); getch();
	_1031_098e();
	//^38C8:061C
	return;
}





//^14CD:062E
// returns a type of tile ?
X8 SkWinCore::_14cd_062e()
{
	//^14CD:062E
	ENTER(2);
	//^14CD:0632
	U8 cl = 0;
	i8 bp01 = glbCurrentThinkingCreatureData->x;
	i8 bp02 = glbCurrentThinkingCreatureData->y;
	if (glbCurrentThinkingCreatureData->x != -1) {
		if (((cl = (_4976_3672[RCJ(56,bp01)][bp02].b5() & 0xe0)) & 0x60) == 0x40) {
			if (glbSomeMap_4976_4ee7 != glbCreatureMap) {
				cl = 0;
			}
		}
	}
	//^14CD:0680
	return cl;
}

//^14CD:0067
i16 SkWinCore::SELECT_CREATURE_4EFE(const sk4efe *ref)
{
	// _4976_37fc[xx][yy], select yy.

	//^14CD:0067
	ENTER(16);
	//^14CD:006D
	CreatureInfoData *xCreatureInfo = glbCurrentThinkingCreatureData; // bp04
	Creature *xCreature = glbCurrentThinkingCreatureRec; // bp08
	X16 si = xCreature->w10;
	X16 bp0e = RAND();
	if (glbSomeMap_4976_4ee7 == glbCreatureMap) {
		si &= 0x7fff;
	}
	//^14CD:00A7
	else if ((bp0e & (xCreatureInfo->x != xactrNeedReset) ? 0x70 : 0x30) == 0) {
		//^14CD:00BE
		si |= 0x8000;
		si &= 0xbfff;
	}
	//^14CD:00C6
	if ((si & 0x8000) == 0) {
		if ((bp0e & (((si & 8) != 0) ? 0x380 : 0xf80)) == 0)
			si ^= 0x4000;
	}
	//^14CD:00E3
	if ((bp0e / (0x10 - glbAIDef->w22_4_7())) == 0)
		si &= 0xffdf;
	//^14CD:0105
	if ((tblAIStats01[_4976_4efa] & 0x400) != 0)
		si |= 0x2000;
	else if ((bp0e & 0x38) == 0)
		si &= 0xdfff;
	//^14CD:0124
	if ((bp0e & 0x3000) == 0)
		si &= 0xffef;
	if ((bp0e & 0x3) == 0)
		si &= 0xffbf;
	if ((bp0e & 0x8008) == 0)
		si &= 0xefff;
	//^14CD:0145
	X16 di = 2;
	if ((tblAIStats01[_4976_4efa] & 2) == 0) {
		//^14CD:0156
		di = di * ((((glbCurrentThinkingCreatureID.DBIndex() & 3) +1) << 1) -1);
	}
	//^14CD:0168
	if (RAND16(di) == 0) {
		//^14CD:0173
		if (U32((xCreature->hp1 * 100) / i16(max_value(1, glbAIDef->BaseHP))) < 0x19)
			si |= 8;
		else	
			si &= 0xfff7;
	}
	//^14CD:01B6
	xCreature->w10 = si;
	X16 bp10 = 0xffff;
	di = 0xffff;
	X16 bp0c = 0;
	X16 bp0a;
	for (; (bp0a = ref->w0) != 0; bp0c++, ref++) {
		//^14CD:01CC
		if ((bp0a & 0xc000) == 0xc000) {
			if (GET_GLOB_VAR(bp0a & 0x3fff) == 0)
				continue;
			break;
		}
		//^14CD:01EA
		if (bp10 == 0xffff && (bp0a & si) != 0) {
			//^14CD:01F5
			bp10 = bp0c;
		}
		//^14CD:01FB
		if (di == 0xffff && (bp0a & si) == bp0a)
			di = bp0c;
		//^14CD:020D
		if (bp0a == si)
			break;
		//^14CD:0212
	}
	//^14CD:0226
	if (bp0a == 0 && (di != 0xffff || bp10 != 0xffff)) {
		bp0c = (di == 0xffff) ? bp10 : di;
	}
	//^14CD:0246
	if (xCreatureInfo->b22 != bp0c) {
		_4976_514e.b1 = 0;
		xCreatureInfo->x = xactrNeedReset;
		xCreatureInfo->y = 0;
		_4976_514e.b3 = 0xff;
	}
	//^14CD:0267
	return xCreatureInfo->b22 = U8(bp0c);
}

//^14CD:0009
void SkWinCore::SELECT_CREATURE_37FC()
{
	//^14CD:0009
	ENTER(0);
	//^14CD:000C
	if (_4976_4efa == 0xffff) {
		//^14CD:0013
		_4976_4efa = QUERY_GDAT_CREATURE_WORD_VALUE(glbCurrentThinkingCreatureRec->CreatureType(), 1);
	}
	_4976_4efc = SELECT_CREATURE_4EFE(_4976_37fc[RCJ(CREATURE_AI_TAB_SIZE,_4976_4efa)]);
	_4976_4efe = &_4976_37fc[RCJ(CREATURE_AI_TAB_SIZE,_4976_4efa)][_4976_4efc];
	//^14CD:0065
	SkD((DLV_CAI, "CAI: (i) a#%03d 37fc[%2d,%3d] \n"
		, (Bitu)glbCurrentThinkingCreatureData->CreatureIndex(), (Bitu)_4976_4efa, (Bitu)_4976_4efc
		));

	return;
}



//^3E74:5788
U8 *SkWinCore::_3e74_5788(U16 xx, i32 yy)
{
	//^3E74:5788
	ENTER(0);
	//^3E74:578B
	_4976_5c90 = 1;
	FREE_INDEXED_MEMENT(xx|0x8000);
	_4976_5c90 = 0;
	return ALLOC_CPXHEAP_MEM(xx, yy);
}

//^14CD:0276
void SkWinCore::_14cd_0276(skxxx9 *ref)
{
	//^14CD:0276
	ENTER(14);
	//^14CD:027B
	i16 si = max_value(0, ref->b6);
	_4976_514e.b1 = _4976_514e.b0 = U8(si);
	_4976_514e.w8 = ref->w4;
	_4976_514e.b3 = ref->b7;
	_4976_514e.w4 = ref->w8;
	_4976_514e.w6 = ref->w10;
	_4976_514e.b2 = ref->b17;
	_4976_514e.pv10 = ref->pv18();
	U8 *bp04 = NULL;
	if (si > 0) {
		//^14CD:02DB
		U16 bp0e;
		i32 bp0c = _3e74_5673(glbCurrentThinkingCreatureID.DBIndex()|0x28000000, &bp0e, 1);
		i32 bp08 = si << 1;
		if (bp0c < bp08) {
			//^14CD:0315
			bp04 = (bp0c != 0)
				? _3e74_5788(bp0e, bp08)
				: ALLOC_CPXHEAP_MEM(bp0e, bp08);
			_3e74_585a(bp0e, 1);
		}
		else {
			//^14CD:0352
			bp04 = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(bp0e));
		}
		//^14CD:0361
		ATLASSERT(bp08 < sizeof(glbCreatureWalkPath));
		COPY_MEMORY(&glbCreatureWalkPath, bp04, bp08);
	}
	//^14CD:0379
	glbMemWalkPath = reinterpret_cast<WalkPath *>(bp04);
	//^14CD:0386
	return;
}

//^14CD:0F0A
void SkWinCore::_14cd_0f0a(U8 func, U8 xx, U8 yy, sk1bf9 *ss)
{
	//^14CD:0F0A
	ENTER(0);
	//^14CD:0F0D
	func &= 31;
	_4976_4ef6 = 0xffff;
	(this->*_4976_38a4[RCJ(17,func)])(xx, yy, ss);
	//^14CD:0F3A
	return;
}

//^14CD:0F3C
void SkWinCore::_14cd_0f3c(i8 aa, sk1bf9 *ss, sk1bf9 *tt, X8 ww, i8 vv, Ax3 uu, U8 xx, U8 yy)
{
	SkD((DLV_CAI, "CAI: (e) Append #%d (%02X,x,x,%02X,%02X,(%2d,%2d,%2d),%04X,%02X) \n"
		, (Bitu)_4976_4fd8->b18()
		, (Bitu)(U8)aa, (Bitu)ww, (Bitu)vv, (Bitu)uu.GetX(), (Bitu)uu.GetY(), (Bitu)uu.GetMap(), (Bitu)xx, (Bitu)yy
		));

	//^14CD:0F3C
	ENTER(6);
	//^14CD:0F40
	if (tt == NULL)
		return;
	if (ss == 0)
		return;
	if (i8(_4976_4fd8->b18()) >= 0x10)
		return;
	//^14CD:0F60
	i8 bp05 = tt->b8;
	if (glbSomeMap_4976_4ee7 != glbCreatureMap && bp05 > 0 && glbAIDef->w0_e_e() == 0) {
		//^14CD:0F86
		bp05 >>= 2;
		vv >>= 2;
	}
	//^14CD:0F8E
	vv = (i8)BETWEEN_VALUE(-1, vv +127, bp05);
	if (vv < 0)
		return;
	skxxx9 *bp04 = &_4976_4fee[RCJ(16,_4976_4fd8->b18())];
	_4976_4fd8->b18(_4976_4fd8->b18() +1);
	bp04->b14 = xx;
	bp04->b15 = yy;
	bp04->w12 = uu;
	bp04->b0 = vv;
	//^14CD:0FE2
	bp04->b1 = tt->b9;
	bp04->b7 = aa;
	bp04->w8 = tt->w4;
	bp04->w10 = tt->w6 & _4976_4ef6;
	bp04->b17 = ww;
	bp04->pv18(ss);
	//^14CD:102C
	return;
}

//^14CD:18CC
void SkWinCore::_14cd_18cc(U8 xx, U8 yy, sk1bf9 *ss)
{
	//^14CD:18CC
	ENTER(0);
	//^14CD:18CF
	_14cd_0f3c(0, ss, _4976_1bcf, 0, 0, Ax3::Invalid, yy, xx);
	//^14CD:18F0
	return;
}


//^2C1D:09D9
U16 SkWinCore::_2c1d_09d9()
{
	//^2C1D:09D9
	ENTER(4);
	//^2C1D:09DF
	X32 bp04 = 0;
	U16 di;
	for (di = 0; di < glbChampionsCount; di++) {
		//^2C1D:09ED
		U16 si;
		for (si = 0; si < SKILL_MAJOR_COUNT; si++) {	// (si = 0; si <= 3; si++) 
			//^2C1D:09F1
			bp04 += glbChampionSquad[di].skills[si];
			//^2C1D:0A0F
		}
		//^2C1D:0A15
	}
	//^2C1D:0A1C
	X16 si;
	for (si = 1; bp04 >= 0x200; ) {
		//^2C1D:0A21
		bp04 = bp04 >> 1;
		si++;
		//^2C1D:0A32
	}
	//^2C1D:0A41
	return si;
}


//^14CD:18F2
void SkWinCore::_14cd_18f2(i8 xx, U8 yy, sk1bf9 *ss, X8 ww, Ax3 vv)
{
	//^14CD:18F2
	ENTER(14);
	//^14CD:18F7
	if (ss == 0)
		return;
	X16 si = (xx < 0) ? 1 : 0;
	if (si != 0) {
		xx = -xx;
	}
	//^14CD:191D
	do {
		if (ss->b12 == xx) {
			//^14CD:1929
			if (CREATURE_THINK_1316(ss->b1, ss->w2, yy) != 0) {
				//^14CD:1944
				sk1bf9 bp0e = *ss;
				if (si != 0)
					bp0e.b8 = bp0e.b9 = 0;
				//^14CD:1963
				_14cd_0f3c(ss->b0, ss, &bp0e, xx, ww, vv, yy, xx);
			}
		}
		//^14CD:1990
	} while ((ss++)->b13 != 0);
	//^14CD:19A1
	return;
}

//^14CD:19A4
void SkWinCore::_14cd_19a4(U8 xx, U8 yy, sk1bf9 *ss)
{
	//^14CD:19A4
	ENTER(0);
	//^14CD:19A7
	_14cd_18f2(xx, yy, ss, 0, Ax3::Invalid);
	//^14CD:19C0
	return;
}

//^14CD:102E
X16 SkWinCore::_14cd_102e(X16 ww, ObjectID rl, i8 dir, X16 alsoPossession, X16 alsoContainer)
{
	//^14CD:102E
	ENTER(2);
	//^14CD:1034
	ObjectID di = rl;
	X16 si = 0;
	for (; di != OBJECT_END_MARKER; di = GET_NEXT_RECORD_LINK(di)) {
		//^14CD:103C
		i16 bp02 = di.DBType();
		if ((alsoPossession != 0 && bp02 == dbCreature) || (alsoContainer != 0 && IS_CONTAINER_CHEST(di) != 0)) {
			//^14CD:1066
			si += _14cd_102e(ww, GET_ADDRESS_OF_RECORD4(di)->GetPossessionObject(), dir, alsoPossession, alsoContainer);
		}
		//^14CD:108B
		if (bp02 <= dbCreature || bp02 >= dbMissile)
			continue;
		//^14CD:1097
		if (dir == -1 || di.Dir() == dir) {
			//^14CD:10AB
			if (CREATURE_CAN_HANDLE_IT(di, ww) != 0)
				//^14CD:10BA
				si++;
		}
		//^14CD:10BB
	}
	//^14CD:10CC
	return si;
}

//^14CD:10D2
sk4f04 *SkWinCore::_14cd_10d2(sk1bf9 *ss, i8 ww)
{
	//^14CD:10D2
	ENTER(30);
	//^14CD:10D8
	X16 bp18 = 0;
	X16 bp1a = 0;
	X16 bp1c = 0;
	X16 bp1e = 0;
	if (_4976_4f02 != 0) {
		//^14CD:10F3
		ZERO_MEMORY(_4976_4f04, sizeof(_4976_4f04)); // 128
		_4976_4f02 = 0;
	}
	//^14CD:1109
	sk4f04 *bp04 = _4976_4f04;
	sk4f04 *bp08 = _4976_4f04;
	i16 si;
	for (si = 0; si < 4; si++, bp08++) {
		//^14CD:111E
		if (bp08->pv0 == 0) {
			//^14CD:112A
			bp04 = bp08;
			continue;
		}
		//^14CD:1138
		if (bp08->pv0 != ss || bp08->b4 != ww)
			continue;
		//^14CD:1156
		return bp08;
		//^14CD:115F
	}
	//^14CD:1169
	bp04->pv0 = ss;
	bp04->b4 = ww;
	skxxxg *bp0c = bp04->x8;
	skxxxg *bp10 = bp04->x8;
	skxxxg *bp14 = bp04->x8;
	bp04->b7 = 0;
	bp04->b6 = 0;
	bp04->b5 = 0;
	si = 0;
	X16 di;
	X16 bp16;
	//^14CD:11AC
	if (si <= 5) {
		do {
			//^14CD:11B4
			if (ss->b12 == ww) {
				//^14CD:11C4
				di = ss->w4;
				if (di != 0xffff && ss->b0 != 8) {
					//^14CD:11DE
					if (ss->b10 != 0xff) {
						//^14CD:11E5
						while (bp10 < bp0c) {
							//^14CD:11E7
							bp10->b1 = U8(bp18) -U8(bp1c);
							++bp10;
						}
						//^14CD:1200
						bp18 = ss->b10;
						bp04->b6 += U8(bp18);
						bp1c = 0;
					}
					//^14CD:121E
					if (ss->b11 != 0xff) {
						//^14CD:1228
						while (bp14 < bp0c) {
							//^14CD:122A
							bp14->b2 = U8(bp1a) -U8(bp1e);
							++bp14;
						}
						//^14CD:1243
						bp1a = ss->b11;
						bp04->b7 += U8(bp1a);
						bp1e = 0;
					}
					//^14CD:1261
					bp16 = _14cd_102e(di, glbCurrentThinkingCreatureRec->GetPossessionObject(), -1, 0, 1);
					bp0c->b0 = U8(bp16);
					bp04->b5 += U8(bp16);
					bp04->b6 -= U8(min_value(bp16, bp18 -bp1c));
					bp04->b7 -= U8(min_value(bp16, bp1a -bp1e));
					bp1c = min_value(bp18, bp1c +bp16);
					bp1e = min_value(bp1a, bp1e +bp16);
				}
				//^14CD:12F6
				++bp0c;
				si++;
			}
			//^14CD:12FB
			//^14CD:1302
		} while ((ss++)->b13 != 0);
	}
	//^14CD:130C
	return bp04;
}

//^14CD:19C2
void SkWinCore::_14cd_19c2(U8 xx, U8 yy, sk1bf9 *ss, i8 vv, i8 ww)
{
	//^14CD:19C2
	ENTER(6);
	//^14CD:19C6
    if (ss == NULL || _4976_4f03 == 0)
		return;
	//^14CD:19D6
	sk4f04 *bp04 = _14cd_10d2(ss, ww);
	if (bp04->b5 <= 0 || bp04->b6 != 0)
		//^14CD:19FF
		_4976_4ef6 &= 0xfff7;
	//^14CD:1A05
	if (glbAIAttacksSpells == 0)
		return;
	//^14CD:1A0C
	X16 bp06 = 0;
	if (xx != 0)
		//^14CD:1A17
		vv = -vv;
	//^14CD:1A1F
	_14cd_18f2(vv, yy, ss, U8(bp06), Ax3::Invalid);
	//^14CD:1A3A
	return;
}

//^14CD:1A3C
void SkWinCore::_14cd_1a3c(U8 xx, U8 yy, sk1bf9 *ss)
{
	//^14CD:1A3C
	ENTER(0);
	//^14CD:1A3F
	_14cd_19c2(xx, yy, ss, 2, 1);
	//^14CD:1A58
	return;
}
//^14CD:1A5A
void SkWinCore::_14cd_1a5a(U8 xx, U8 yy, sk1bf9 *ss)
{
	//^14CD:1A5A
	ENTER(0);
	//^14CD:1A5D
	_14cd_19c2(xx, yy, ss, 4, 3);
	//^14CD:1A76
	return;
}
//^14CD:1A78
void SkWinCore::_14cd_1a78(U8 xx, U8 yy, sk1bf9 *ss, X8 ww)
{
	//^14CD:1A78
	ENTER(24);
	//^14CD:1A7C
	if (ss == NULL)
		return;
	//^14CD:1A87
	sk4f04 *bp04 = _14cd_10d2(ss, ww);
	//^14CD:1A9F
	if (bp04->b7 == 0)
		return;
	//^14CD:1AAC
	skxxxg *bp08 = bp04->x8;
	do {
		//^14CD:1ABB
		if (ss->b12 == ww) {
			//^14CD:1ACA
			if (ss->w4 != 0xffff && CREATURE_THINK_1316(ss->b1, ss->w2, yy) != 0) {
				//^14CD:1AEE
				X16 bp0a = min_value(ss->b8, bp08->b0);
				sk1bf9 bp18 = *ss;
				if (xx != 0) {
					//^14CD:1B24
					bp18.b8 = 0;
					bp18.b9 = 0;
					bp0a = 0;
				}
				//^14CD:1B31
				_14cd_0f3c(ss->b0, ss, &bp18, ww, i8(bp0a), Ax3::Invalid, yy, xx);
			}
			//^14CD:1B5D
			++bp08;
		}
		//^14CD:1B61
	} while ((ss++)->b13 != 0);
	//^14CD:1B72
	return;
}

//^14CD:1B74
void SkWinCore::_14cd_1b74(U8 xx, U8 yy, sk1bf9 *ss)
{
	//^14CD:1B74
	ENTER(0);
	//^14CD:1B77
	_14cd_1a78(xx, yy, ss, 1);
	//^14CD:1B8E
	return;
}

//^14CD:1B90
void SkWinCore::_14cd_1b90(U8 xx, U8 yy, sk1bf9 *ss) 
{
	//^14CD:1B90
	ENTER(0);
	//^14CD:1B93
	_14cd_1a78(xx, yy, ss, 3);
	//^14CD:1BAA
	return;
}

//^14CD:1C27
void SkWinCore::_14cd_1c27(U8 xx, U8 yy, sk1bf9 *ss)
{
	//^14CD:1C27
	ENTER(0);
	//^14CD:1C2A
	_14cd_1bac(xx, yy, ss, 2, 1);
	//^14CD:1C43
	return;
}

//^14CD:1BAC
void SkWinCore::_14cd_1bac(U8 xx, U8 yy, sk1bf9 *ss, X8 vv, X8 ww)
{
	//^14CD:1BAC
	ENTER(4);
	//^14CD:1BB0
    if (ss == NULL || _4976_4f03 == 0)
		return;
	//^14CD:1BC0
	sk4f04 *bp04 = _14cd_10d2(ss, ww);
	if ((glbAIAttacksSpells & 8) != 0 && (bp04->b5 < 0 || bp04->b6 != 0))
		_4976_4ef6 &= 0xfff7;
	//^14CD:1BF7
	if (glbAIAttacksSpells == 0)
		return;
	if (xx != 0)
		//^14CD:1C04
		vv = -vv;
	//^14CD:1C0C
	_14cd_18f2(vv, yy, ss, 0, Ax3::Invalid);
	//^14CD:1C25
	return;
}
//^14CD:1C45
void SkWinCore::_14cd_1c45(U8 xx, U8 yy, sk1bf9 *ss)
{
	//^14CD:1C45
	ENTER(0);
	//^14CD:1C48
	_14cd_1bac(xx, yy, ss, 4, 3);
	//^14CD:1C61
	return;
}
//^14CD:1C63
void SkWinCore::_14cd_1c63(U8 xx, U8 yy, sk1bf9 *ss)
{
	//^14CD:1C63
	ENTER(0);
	//^14CD:1C66
	_14cd_18f2(5, yy, ss, 0, (_4976_5151 == 13) ? _4976_5156 : Ax3::Invalid);
	//^14CD:1C8B
	return;
}
//^14CD:1C8D
void SkWinCore::_14cd_1c8d(U8 xx, U8 yy, sk1bf9 *ss)
{
	//^14CD:1C8D
	ENTER(0);
	//^14CD:1C90
	if (xx != 0 && glbCurrentThinkingCreatureRec->TriggerX() == glbCreatureTimerGetX && glbCurrentThinkingCreatureRec->TriggerY() == glbCreatureTimerGetY && glbCurrentThinkingCreatureRec->TriggerMap() == glbSomeMap_4976_4ee7)
		return;
	//^14CD:1CD3
	_14cd_18f2(6, yy, ss, 0, Ax3::Invalid);
	//^14CD:1CEA
	return;
}
//^14CD:1CEC
void SkWinCore::_14cd_1cec(U8 xx, U8 yy, sk1bf9 *ss)
{
	//^14CD:1CEC
	ENTER(4);
	//^14CD:1CF0
	Missile *bp04 = GET_MISSILE_REF_OF_MINION(glbCurrentThinkingCreatureID, OBJECT_NULL);
	if (bp04 == NULL || bp04->GetMissileObject().DBType() != dbContainer)
		return;
	//^14CD:1D19
	_14cd_18f2(7, yy, ss, 0, GET_ADDRESS_OF_RECORD9(bp04->GetMissileObject())->GetDest());
	//^14CD:1D40
	return;
}
//^14CD:1D42
void SkWinCore::_14cd_1d42(U8 xx, U8 yy, sk1bf9 *ss)
{
	//^14CD:1D42
	ENTER(0);
	//^14CD:1D45
	_14cd_18f2(0x12, yy, ss, 0, (_4976_5151 == 5) ? _4976_5156 : Ax3::Invalid);
	//^14CD:1D6A
	return;
}

//^14CD:1D6C
void SkWinCore::_14cd_1d6c(U8 xx, U8 yy, sk1bf9 *ss, i8 ww)
{
	//^14CD:1D6C
	ENTER(14);
	//^14CD:1D72
	if (ss == NULL)
		return;
	do {
		//^14CD:1D7D
		X16 di = ss->w4;
		X16 si = ss->w6;
		if (ss->b12 == ww) {
			//^14CD:1D94
			if (di == 0xffff || (si != 0 && si == 1) || CREATURE_CAN_HANDLE_ITEM_IN(di, glbCurrentThinkingCreatureRec->possession, -1) != OBJECT_END_MARKER) {
				//^14CD:1DBB
				if (CREATURE_THINK_1316(ss->b1, ss->w2, yy) != 0) {
					//^14CD:1DD6
					sk1bf9 bp0e = *ss;
					if (xx != 0) {
						//^14CD:1DEF
						bp0e.b8 = 0;
						bp0e.b9 = 0;
					}
					//^14CD:1DF7
					_14cd_0f3c(ss->b0, ss, &bp0e, ww, 0, Ax3::Invalid, yy, xx);
				}
			}
		}
		//^14CD:1E21
	} while ((ss++)->b13 != 0);
	//^14CD:1E32
	return;
}

//^14CD:1E36
void SkWinCore::_14cd_1e36(U8 xx, U8 yy, sk1bf9 *ss)
{
	//^14CD:1E36
	ENTER(0);
	//^14CD:1E39
	_14cd_1d6c(xx, yy, ss, 15);
	//^14CD:1E50
	return;
}

//^14CD:1E52
void SkWinCore::_14cd_1e52(U8 xx, U8 yy, sk1bf9 *ss) { // TODO: Unr
	Unr();
}

//^32CB:00BF
U16 SkWinCore::IS_MAP_INSIDE(U16 mapno)
{
	//^32CB:00BF
	ENTER(0);
	//^32CB:00C2
	return !(QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, dunMapsHeaders[mapno].MapGraphicsStyle(), dtWordValue, GDAT_GFXSET_SCENE_FLAGS) & 0x20);

	// MapGraphicsStyle = [0x65]
	// 00=0018 ;Void
	// 01=0028 ;Outside
	// 02=000B ;Cave
	// 03=000B ;Stone corridor
	// 04=0028 ;Skullkeep rooftop
	// 05=0028 ;Myst zone

	// SPX: 0x020 would mean to use 'outdoor sky' (in weather category) ?
}

//^14CD:1E6E
void SkWinCore::_14cd_1e6e(U8 xx, U8 yy, sk1bf9 *ss)
{
	// SPX: this one seems to be called by bats? for each tile.
	//^14CD:1E6E
	ENTER(0);
	//^14CD:1E71
	if (IS_MAP_INSIDE(glbSomeMap_4976_4ee7) == 0) {
		//^14CD:1E93
		glbCurrentThinkingCreatureRec->w10_7_7(0);
		//^14CD:1E9D
		return;
	}
	//^14CD:1E81
	if (xx != 0) {
		//^14CD:1EB5
		if ((RAND() & 0x1F) == 0) {
			//^14CD:1EBF
			glbCurrentThinkingCreatureRec->w10_7_7(0);
		}
		//^14CD:1EC9
		_14cd_0f3c(0, ss, _4976_1bcf, 0, 0, Ax3::Invalid, yy, xx);
	}
	else {
		//^14CD:1E8B
		if (glbCurrentThinkingCreatureRec->w10_7_7() != 0) {
			//^14CD:1E91
			//^14CD:1E93
			glbCurrentThinkingCreatureRec->w10_7_7(0);
			//^14CD:1E9D
			return;
		}
		//^14CD:1E9F
		if ((RAND() & 0x3F) == 0) {
			//^14CD:1EA9
			glbCurrentThinkingCreatureRec->w10_7_7(1);
			//^14CD:1EC9
			_14cd_0f3c(0, ss, _4976_1bcf, 0, 0, Ax3::Invalid, yy, xx);
		}
	}
	//^14CD:1EEA
}

//^14CD:1EEC
void SkWinCore::_14cd_1eec(U8 xx, U8 yy, sk1bf9 *ss, X8 ww)
{
	//^14CD:1EEC
	ENTER(14);
	//^14CD:1EF0
	if (ss == NULL)
		return;
	do {
		//^14CD:1EFB
		if (ss->b12 == ww) {
			//^14CD:1F07
			if (CREATURE_THINK_1316(ss->b1, ss->w2, yy) != 0) {
				//^14CD:1F22
				sk1bf9 bp0e = *ss;
				bp0e.w6 = glbCurrentThinkingCreatureRec->w8;
				if (xx != 0)
					bp0e.b8 = bp0e.b9 = 0;
				//^14CD:1F4E
				_14cd_0f3c(ss->b0, ss, &bp0e, ww, 0, Ax3::Invalid, yy, xx);
			}
		}
		//^14CD:1F78
	} while ((ss++)->b13 != 0);
	//^14CD:1F89
	return;
}

//^14CD:1FA7
void SkWinCore::_14cd_1fa7(U8 xx, U8 yy, sk1bf9 *ss)
{
	//^14CD:1FA7
	ENTER(2);
	//^14CD:1FAB
	Ax3 bp02;
	bp02.SetX(glbCreaturePosX);
	bp02.SetY(glbCreaturePosY);
	bp02.SetMap(glbCreatureMap);
	_14cd_18f2(0x16, yy, ss, 0, bp02);
	//^14CD:1FF1
	return;
}

//^14CD:1F8B
void SkWinCore::_14cd_1f8b(U8 xx, U8 yy, sk1bf9 *ss)
{
	//^14CD:1F8B
	ENTER(0);
	//^14CD:1F8E
	_14cd_1eec(xx, yy, ss, 0x15);
	//^14CD:1FA5
	return;
}

//^1C9A:38A8
X16 SkWinCore::_1c9a_38a8()
{
	//^1C9A:38A8
	ENTER(22);
	//^1C9A:38AE
	CreatureInfoData *bp08 = glbCurrentThinkingCreatureData;
	X16 di = 0;
	__SET_CURRENT_THINKING_CREATURE_WALK_PATH();
	X16 si = 0;
	while (true) {
		//^1C9A:38C3
		if (_4976_4fd8->b18() > si) {
			//^1C9A:38CE
			if (_4976_4fee[RCJ(16,si)].b7 != _4976_5151 || _4976_4fee[RCJ(16,si)].w8 != _4976_5152) {
				//^1C9A:38F4
				si++;
				continue;
			}
			else {
				//^1C9A:38F7
				if (si != 0) {
					//^1C9A:38FB
					MOVE_MEMORY(&_4976_4fee[RCJ(16,si)], _4976_4fee, sizeof(skxxx9));
				}
				//^1C9A:3917
				_4976_4fee->b0 = 0;
				X16 bp14 = glbCurrentMapIndex;
				_1c9a_0648(_4976_5156.GetMap());
				WalkPath bp16;
				X16 bp10;
				X16 bp0e;
				if (FIND_WALK_PATH(bp0e = _4976_5156.GetX(), bp10 = _4976_5156.GetY(), 2, 0, 1, _4976_4fee, &bp16) == 0 && _4976_4fee[0].b6 == 0) {
					//^1C9A:396F
					X16 bp0a = glbCreatureTimerGetX;
					X16 bp0c = glbCreatureTimerGetY;
					si = _4976_514e.b1;
					if (si == 0) {
						//^1C9A:398A
						di = (bp0a == bp0e && bp0c == bp10 && glbCurrentMapIndex == bp14) ? 1 : 0;
					}
					else {
						//^1C9A:39AE
						_1c9a_0648(bp14);
						i16 bp12 = 5;
						__SET_CURRENT_THINKING_CREATURE_WALK_PATH();
						//^1C9A:39BF
						while (true) {
							WalkPath *bp04 = &glbMemWalkPath[_4976_514e.b0 -si];
							if (true
								&& CREATURE_GO_THERE(_4976_4fd8->b17, bp0a, bp0c, -1, -1, bp04->getDir()) != 0
								&& bp04->getY() == glbCreatureSomeY
								&& bp04->getMapCross() == ((glbCreatureSomeZMap != glbCurrentMapIndex) ? 1 : 0) 
							) {
								//^1C9A:3A3D
								_1c9a_0648(glbCreatureSomeZMap);
								bp0a = glbCreatureSomeX;
								bp0c = glbCreatureSomeY;
								if (--si > 0 && --bp12 > 0)
									continue;
								//^1C9A:3A5D
								di = 1;
							}
							break;
						}
					}
				}
				//^1C9A:3A60
				_1c9a_0648(bp14);
			}
		}
		//^1C9A:3A68
		return di;
	}
}



//^14CD:0550
void SkWinCore::_14cd_0550(skxxxh *ref, i8 xx, i8 yy, X16 ww)
{
	//^14CD:0550
	ENTER(8);
	//^14CD:0556
	X16 bp08 = 0;

	// SPX: ref may be null when using IMG9; let's skip this ...
	if (SkCodeParam::bUseFixedMode && ref == NULL)
		return;

	//^14CD:055B
	do {
		i8 bp05 = ref->b0;
		X16 si;

		//^14CD:0564
		SkD((DLV_CAI, "CAI: (B) a#%03d H(%3d,%3d,x,%d) _ %02X %02X %04X \n"
			, (Bitu)glbCurrentThinkingCreatureData->CreatureIndex(), (Bitu)ref->b0, (Bitu)ref->b1, (Bitu)ref->b6
			, (Bitu)(U8)xx, (Bitu)(U8)yy, (Bitu)ww
			));

		if (bp05 != xx) {
			//^14CD:0569
			if (_4976_5162 != 0)
				continue;
			//^14CD:0573
			i16 di = ref->b1; // Typ@i8
			if (di != 0) {
				//^14CD:0581
				if (di > 0) {
					//^14CD:0585
					if (RAND16(di) != 0)
						continue;
				}
				else {
					//^14CD:0593
					if (RAND16(-di) == 0)
						continue;
				}
			}
			//^14CD:05A2
			si = 0;
		}
		else {
			//^14CD:05A6
			bp08 = (ww != 0 || _4976_5162 != 0) ? 1 : 0;
			si = yy;
		}
		//^14CD:05C3
		sk3672 *bp04 = _4976_3672[RCJ(56,bp05)];

		//^14CD:05DA
		//SkD((DLV_CAI, "CAI: b-- a#%03d 3672[%3d,%3d] (%3d,%2d,%2d,%3d,%3d,%2d,%2d)\n"
		//	, (Bitu)glbCurrentThinkingCreatureData->w0, (Bitu)bp05, (Bitu)si
		//	, (Bits)(i8)bp04[si].b0_[0], (Bits)(i8)bp04[si].b0_[1], (Bits)(i8)bp04[si].b0_[2], (Bits)(i8)bp04[si].b0_[3]
		//	, (Bits)(i8)bp04[si].b0_[4], (Bits)    bp04[si].b0_[5], (Bits)    bp04[si].b0_[6]
		//	));

		_14cd_0f0a(bp04[si].b5(), bp04[si].b6(), bp05, ref->pv2);

		//SkD((DLV_CAI, "CAI: --b a#%03d r %d\n"
		//	, (Bitu)glbCurrentThinkingCreatureData->w0, (Bitu)_4976_5162
		//	));
		//^14CD:0613
	} while (bp08 == 0 && (ref++)->b6 != 0);

	//^14CD:062A
	return;
}
//^14CD:0457
void SkWinCore::_14cd_0457()
{
	//^14CD:0457
	ENTER(10);
	//^14CD:045D
	i16 bp0a = _4976_4fd8->b18();
	if (bp0a == 0)
		return;
	//^14CD:046B
	skxxx9 *bp04 = _4976_4fee;
	i16 di = _4976_514e.b0;
	for (; bp0a > 0; bp0a--, bp04++) {
		//^14CD:047C
		i16 si = bp04->b0;
		if (si <= 0)
			continue;
		bp04->b0 = i8(min_value(si >> 1, di -2));
		//^14CD:04A1
	}
	//^14CD:04AE
	bp0a = _4976_4fd8->b18();
	bp04 = _4976_4fee;
	for (di = 0; di < bp0a; di++, bp04++) {
		//^14CD:04C1
		if (bp04->b0 >= 0)
			continue;
		//^14CD:04CA
		i16 si = di +1;
		skxxx9 *bp08 = bp04 +1;
		for (; bp0a < bp0a; si++, bp08++) {
			//^14CD:04E0
			if (bp08->b0 >= 0)
				break;
			//^14CD:04E9
		}
		//^14CD:04F3
		if (si >= bp0a) {
			//^14CD:04F8
			bp0a = di +1;
			continue;
		}
		//^14CD:0500
		MOVE_MEMORY(bp08, bp04, (bp0a -si) * sizeof(skxxx9));
		bp0a -= si -di;
		//^14CD:0528
	}
	//^14CD:0532
	if (_4976_4fd8[bp0a].b0 < 0)
		bp0a--;
	//^14CD:0546
	_4976_4fd8->b18(i8(bp0a));
	//^14CD:054C
	return;
}

//^3A15:2CA8
void SkWinCore::ADVANCE_TILES_TIME(X16 xx, X16 yy)
{
	// call this if you modify tile's type.
	// c.g. convert floor to pit-fall. then it allows you to fall down if you are at the pit-fall.

	//^3A15:2CA8
	ENTER(12);
	//^3A15:2CAE
	if (glbCurrentMapIndex == glbMap_4c28 && xx == glbSomePosX_4c2e && yy == glbSomePosY_4c30) {
		//^3A15:2CC9
		MOVE_RECORD_TO(OBJECT_NULL, xx, yy, xx, yy);
	}
	//^3A15:2CDF
	ObjectID si = GET_CREATURE_AT(xx, yy);
	if (si != OBJECT_NULL) {
		MOVE_RECORD_TO(si, xx, yy, xx, yy);
	}
	//^3A15:2D08
	si = GET_WALL_TILE_ANYITEM_RECORD(xx, yy);
	ObjectID bp0a = si;
	X16 bp0c = 0;
	for (; si != OBJECT_END_MARKER; si = GET_NEXT_RECORD_LINK(si)) {
		//^3A15:2D21
		if (si.DBType() <= dbCreature)
			continue;
		bp0c++;
		//^3A15:2D31
	}
	//^3A15:2D3F
	for (; (si = bp0a) != OBJECT_END_MARKER && bp0c != 0; ) {
		//^3A15:2D42
		bp0a = GET_NEXT_RECORD_LINK(si);
		U16 di = si.DBType();
		if (di <= dbCreature)
			continue;
		//^3A15:2D5E
		bp0c--;
		MOVE_RECORD_TO(si, xx, yy, xx, yy);
		if (di == dbMissile) {
			//^3A15:2D7E
			Missile *bp08 = GET_ADDRESS_OF_RECORDE(si);
			Timer *bp04 = &glbTimersTable[bp08->TimerIndex()];
			bp04->w8_0_4(_4976_5826);
			bp04->w8_5_9(_4976_5828);
			bp04->w8_a_b(_4976_581e);
			bp04->id6(ObjectID(si, _4976_5820));
			bp04->SetMap(_4976_581c);
			continue;
		}
		//^3A15:2E21
		if (di == dbCloud) {
			//^3A15:2E26
			Timer *bp04 = glbTimersTable;

			// SPX: I had the case of infinite loop here because of non incrementation of di (nor bp04?)!!
			// the whole original block is commented then rewritten after
			/*
			for (di = 0; di < glbTimersActiveCount; ) {
				//^3A15:2E37
				if (bp04->TimerType() != ttyCloud || bp04->w8 != si)
					continue;
				bp04->XcoordB(U8(_4976_5826));
				bp04->YcoordB(U8(_4976_5828));
				bp04->id8(ObjectID(si, _4976_5820));
				bp04->SetMap(_4976_581c);
				//^3A15:2E95
			}*/
			// SPX: new block with supposition
			for (di = 0; di < glbTimersActiveCount; di++, bp04++) {
				//^3A15:2E37
				if (bp04->TimerType() != ttyCloud || bp04->w8 != si)
					continue;
				bp04->XcoordB(U8(_4976_5826));
				bp04->YcoordB(U8(_4976_5828));
				bp04->id8(ObjectID(si, _4976_5820));
				bp04->SetMap(_4976_581c);
				//^3A15:2E95
			}
			// SPX: end of new block
		}
		//^3A15:2EA0
	}
	//^3A15:2EB1
	return;
}
//^1887:000C
X16 SkWinCore::OPERATE_PIT_TELE_TILE(X16 xx, X16 yy, X16 ww)
{
	// this is for invisible walker? at the crypt and keep

	// ww: 0=activate, 1=deactivate, 2=toggle

	//^1887:000C
	ENTER(8);
	//^1887:0012
	X16 di = ww;
	X16 si = 0;
	U8 *bp04 = &glbCurrentTileMap[xx][yy];
	if ((*bp04 >> 5) == ttPit) {
		// pit bit3: 0=Closed, 1=Open

		//^1887:0042
		if ((*bp04 & 8) != 0)
			si = 1;
		//^1887:004B
		if (di == 2) {
			di = (si != 0) ? 1 : 0;
		}
		//^1887:005E
		if (di == 0) {
			//^1887:0062
_0062:
			si = !si;
			*bp04 |= 8; // open pit (you can fall down)
			ADVANCE_TILES_TIME(xx, yy);
		}
		else {
			//^1887:0081
			*bp04 &= 0xf7; // close pit (you cannot fall off)
		}
	}
	//^1887:008E
	else if ((*bp04 >> 5) == ttTeleporter) {
		// teleporter bit3: 0=closed, 1=open

		//^1887:009D
		Teleporter *bp08 = GET_ADDRESS_OF_TILE_RECORD(U8(xx), U8(yy))->castToTeleporter();
		if (bp08->b4_1_2() != 3) {
			//^1887:00C1
			if ((*bp04 & 8) != 0)
				si = 1;
			//^1887:00CD
			if (di == 2) {
				di = (si != 0) ? 1 : 0;
			}
			//^1887:00E0
			if (di == 0)
				goto _0062; // open teleporter (you can warp)
			//^1887:00E7
			*bp04 &= 0xf7; // close teleporter (you cannot warp)
		}
	}
	//^1887:00F2
	return si;
}


//^075F:11BA
void SkWinCore::STEP_MISSILE(Timer *ref)
{
	//^075F:11BA
	ENTER(48);
	//^075F:11C0
	Timer bp30;
	COPY_MEMORY(ref, &bp30, 10);
	Timer *bp04 = &bp30;
	ObjectID bp0e = bp04->id6();
	ObjectID bp14 = bp04->id6();
	Missile *bp08 = GET_ADDRESS_OF_RECORDE(bp0e);
	i16 di = bp04->w8_0_4();
	i16 bp10 = bp04->w8_5_9();
	ATLASSERT(OBJECT_END_MARKER != GET_TILE_RECORD_LINK(di, bp10));
	ObjectID bp24;
	X16 si;
	U16 bp12;
	if (bp04->TimerType() == tty1D) {
		//^075F:121C
		bp04->TimerType(tty1E);
	}
	else {
		//^075F:1224
		si = bp0e.Dir();
		bp24 = GET_CREATURE_AT(di, bp10);
		if (bp24 != OBJECT_NULL) {
			//^075F:1242
			Creature *bp0c = GET_ADDRESS_OF_RECORD4(bp24);
			if ((QUERY_CREATURE_AI_SPEC_FLAGS(bp24) & 2) != 0) {
				if (bp08->GetMissileObject().IsMissile()) {
					//^075F:126C
					i16 bp26 = _4976_0094[RCJ(32, bp04->Direction() + (((bp0c->b15_0_1() & 1) << 4) + (si << 2)))];
					if (bp26 < 4) {
						//^075F:12A9
						bp04->Direction(U8(bp26));
						glbTimersTable[bp08->TimerIndex()].Direction(bp26 & 3);
					}
					goto _132b;
				}
			}
		}
		//^075F:12E3
		if (glbCurrentMapIndex == glbMap_4c28) {
			if (di == glbSomePosX_4c2e) {
				if (bp10 == glbSomePosY_4c30) {
					if (_075f_0af9(-3, di, bp10, si, bp0e) != 0) {
						return;
					}
				}
			}
		}
		//^075F:1313
		if (_075f_0af9(-1, di, bp10, si, bp14) != 0)
			return;
		//^075F:132B
_132b:
		if (bp08->EnergyRemaining() <= (bp12 = bp04->b9_4_7())) {
			//^075F:1349
			CUT_RECORD_FROM(bp0e = bp14, NULL, di, bp10);
			DELETE_MISSILE_RECORD(bp0e, NULL, di, bp10);
			return;
		}
		//^075F:1375
		bp08->EnergyRemaining(bp08->EnergyRemaining() -U8(bp12));
		if (bp08->EnergyRemaining2() < bp12) {
			//^075F:1391
			bp08->EnergyRemaining2(0);
		}
		else {
			//^075F:1398
			bp08->EnergyRemaining2(bp08->EnergyRemaining2() -bp12);
		}
	}
	//^075F:13A6
	X16 bp1c = bp04->Direction();
	bp0e = bp04->id6();
	si = bp0e.Dir();
	X16 bp1e = (bp1c == si || ((bp1c +1) & 3) == si) ? 1 : 0;
	X16 bp20;
	X16 bp22;
	if (bp1e != 0) {
		//^075F:13DF
		bp20 = di;
		bp22 = bp10;
		di += glbXAxisDelta[bp1c];
		bp10 += glbYAxisDelta[bp1c];
		bp12 = GET_TILE_VALUE(di, bp10);
		ATLASSERT((bp12 >> 5) != ttMapExit);
		if ((bp12 >> 5) != ttWall) {
			//^075F:1414
			if ((bp12 >> 5) != ttTrickWall || (bp12 & 5) != 0) {
				//^075F:1425
				if ((bp12 >> 5) != ttStairs)
					goto _146e;
				//^075F:142F
				if ((glbCurrentTileMap[bp20][bp22] >> 5) != ttStairs)
					goto _146e;
			}
		}
		//^075F:144D
		if (_075f_0af9(bp12 >> 5, bp20, bp22, si, bp0e) != 0)
			return;
		//^075F:146E
_146e:
		ObjectID bp1a = GET_WALL_TILE_ANYITEM_RECORD(di, bp10);
		for (; bp1a != OBJECT_END_MARKER; bp1a = GET_NEXT_RECORD_LINK(bp1a)) {
			//^075F:147C
			if (bp1a.DBType() == dbCloud) {
				//^075F:148D
				Cloud *bp18 = GET_ADDRESS_OF_RECORDF(bp1a);
				if (bp18->CloudType() == missileTeleporter) {	// missileTeleporter
					//^075F:14AB
					X16 bp26 = ((glbTimersTable[bp08->TimerIndex()].Direction() & 3) +2) & 3;
					bp04->Direction(U8(bp26));
					glbTimersTable[bp08->TimerIndex()].Direction(U8(bp26));
					di = bp20;
					bp10 = bp22;
					bp1e = 0;
					//^075F:1515
					break;
				}
			}
			//^075F:1517
		}
	}
	//^075F:152C
	bp12 = si;
	if ((bp1c & 1) == (si & 1)) {
		si--;
	}
	else {
		si++;
	}
	//^075F:1543
	si &= 3;
	if (bp1e != 0) {
		//^075F:1550
		MOVE_RECORD_TO(ObjectID(bp0e, si), bp20, bp22, di, bp10);
		if (bp08->GetMissileObject() == OBJECT_EFFECT_FIREBALL && glbCurrentMapIndex == glbPlayerMap) {	// oFF80
			glbDoLightCheck = 1;
		}
		//^075F:158A
		bp04->w8_0_4(_4976_5826);
		bp04->w8_5_9(_4976_5828);
		bp04->w8_a_b(_4976_581e);
		bp0e.Dir(_4976_5820);
		bp04->SetMap(_4976_581c);
		di = glbCurrentMapIndex;
		if (di != _4976_581c) {
			CHANGE_CURRENT_MAP_TO(_4976_581c);
		}
		//^075F:1616
		_4976_5826 += glbXAxisDelta[_4976_581e];
		_4976_5828 += glbYAxisDelta[_4976_581e];
		bp24 = GET_CREATURE_AT(_4976_5826, _4976_5828);
		if (bp24 != OBJECT_NULL) {
			//^075F:1649
			ATTACK_CREATURE(bp24, _4976_5826, _4976_5828, 0x2006, 0x64, 0);
		}
		//^075F:1663
		bp12 = GET_TILE_VALUE(_4976_5826, _4976_5828);
		si = bp12 >> 5;
		if (si != ttWall) {
			//^075F:1682
			if (si == ttTrickWall || (bp12 & 4) != 0) {
				//^075F:168E
				if (si == ttDoor || (bp12 &= 7) == 5 || bp12 <= 1) {
					//^075F:16A8
					_4976_5826 += glbXAxisDelta[_4976_581e];
					_4976_5828 += glbYAxisDelta[_4976_581e];
					bp24 = GET_CREATURE_AT(_4976_5826, _4976_5828);
					if (bp24 != OBJECT_NULL) {
						//^075F:16DB
						ATTACK_CREATURE(bp24, _4976_5826, _4976_5828, 0x2006, 0x64, 0);
					}
				}
			}
		}
		//^075F:16F5
		if (_4976_581c != di) {
			CHANGE_CURRENT_MAP_TO(di);
		}
	}
	else {
		//^075F:1704
		if ((GET_TILE_VALUE(di, bp10) >> 5) == ttDoor && _075f_0af9(4, di, bp10, bp12, bp14) != 0)
			return;
		//^075F:172F
		CUT_RECORD_FROM(bp0e, NULL, di, bp10);
		bp0e.Dir(si);
		APPEND_RECORD_TO(bp0e, NULL, di, bp10);
	}
	//^075F:1766
	bp04->dw00 += 1;
	bp04->id6(bp0e);
	bp08->TimerIndex(QUEUE_TIMER(bp04));
	//^075F:178D
	return;
}
//^0CEE:32BA
//X16 SkWinCore::QUERY_DOOR_STRENGTH(X8 cls2)
X16 SkWinCore::QUERY_DOOR_STRENGTH(X8 iDoorType)
{
	//^0CEE:32BA
	ENTER(0);
	//^0CEE:32BD
	return QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_DOORS, iDoorType, dtWordValue, GDAT_DOOR_STRENGTH);
}

//^3A15:07B4
void SkWinCore::STEP_DOOR(Timer *ref)
{
	SkD((DLV_DBG_DOOR, "DOOR: STEP_DOOR(TIMER=%04X,T=%d,Actor=%d,V=%2d,W8=%2d\n", (Bitu)ref->dw00, (Bitu)ref->ttype, (Bitu)ref->actor, (Bitu)ref->value, (Bitu)ref->w8));

		Bit32u dw00;	// @0
		Bit8u ttype;		// @4	b04
		Bit8u actor;	// @5	// b5 player index or creature type or other ...
		Bit16u value;		// @6	// w6 => position x & y	// SPX: don't it take object ID ? or spell power ? it can also be an object! (door) ..
		Bit16u w8;		// @8

	//^3A15:07B4
	ENTER(26);
	//^3A15:07BA
	X16 bp18 = 0;
	X16 bp1a = 0;
	X16 di = ref->XcoordB();
	X16 si = ref->YcoordB();
	U8 *bp04 = &glbCurrentTileMap[di][si];
	X16 bp0a = *bp04 & 7;
	if (bp0a == 5)
		return;
	//^3A15:0807
	if (glbCurrentMapIndex == glbPlayerMap)
		glbDoLightCheck = 1;
	//^3A15:0816
	Door *bp08 = GET_ADDRESS_OF_TILE_RECORD(U8(di), U8(si))->castToDoor();
	if (bp08->Bit10() == 0)
		return;
	//^3A15:0839
	ref->SetTick(ref->GetTick() +1);
	if (bp08->Bit09() == 0) {
		//^3A15:0858
		if (bp0a == 4) {
			bp08->Bit10(0);
			return;
		}
		//^3A15:0861
		X16 bp0e = bp08->b2_5_5();
		X16 bp14 = QUERY_DOOR_STRENGTH(GET_GRAPHICS_FOR_DOOR(bp08));
		if (glbCurrentMapIndex == glbMap_4c28 && di == glbSomePosX_4c2e && si == glbSomePosY_4c30 && bp0a != 0) {
			//^3A15:08AF
			bp1a = 1;
			if (glbChampionsCount > 0) {
				//^3A15:08BB
				*bp04 &= 0xf8;
				X16 bp12 = ATTACK_PARTY(bp14, ((8 |bp0e) != 0) ? 4 : 3, 2);
				if (bp12 != 0) {
					//^3A15:08ED
					i16 bp16 = 0;
					for (; bp16 < 4; bp16++) {
						//^3A15:08F4
						if ((bp12 & (1 << bp16)) == 0)
							continue;
						//^3A15:0901
						// SPX: Bump sound when door closing on champions
						QUEUE_NOISE_GEN2(GDAT_CATEGORY_CHAMPIONS, glbChampionSquad[bp16].HeroType(), SOUND_CHAMPION_BUMP, 0xfe, di, si, 1, 0x64, 0xc8);
						//^3A15:0929
					}
				}
			}
		}
		//^3A15:0932
		ObjectID bp0c = GET_CREATURE_AT(di, si);
		if (bp0c != OBJECT_NULL) {
			//^3A15:0946
			X16 bp10 = QUERY_CREATURE_AI_SPEC_FLAGS(bp0c);
			if ((bp10 & 0x20) == 0) {
				//^3A15:095A
				if (((bp0e != 0) ? ((bp10 >> 6)&3) : 1) <= bp0a) {
					//^3A15:0973
					ATTACK_CREATURE(bp0c, di, si, 0x2006, 0x64, (QUERY_CREATURE_AI_SPEC_FROM_RECORD(bp0c)->w24_c_c() != 0) ? 0 : bp14);
					bp0a = (bp0a == 0) ? 0 : (bp0a -1);
					*bp04 = *bp04 & 0xf8 |bp0a;
					QUEUE_NOISE_GEN2(GDAT_CATEGORY_CREATURES, QUERY_CLS2_FROM_RECORD(bp0c), SOUND_OBJECT_GETHIT, 0xfe,
						di, si, 1, 0x46, 0x80);
					QUEUE_NOISE_GEN2(GDAT_CATEGORY_MISCELLANEOUS, 0xfe, SOUND_STD_KNOCK, 0xfe, di, si, 1, 0x46, 0x80);
					bp1a = 1;
				}
			}
		}
		//^3A15:0A05
		if (bp1a != 0) {
			//^3A15:0A0B
			ref->SetTick(ref->GetTick() +1);
			bp18 = 1;
		}
	}
	else {
		//^3A15:0A1E
		if (bp0a == 0) {
			bp08->Bit10(0);
			return;
		}
	}
	//^3A15:0A27
	if (bp18 == 0) {
		//^3A15:0A2D
		bp0a += (bp08->Bit09() != 0) ? -1 : 1;
		*bp04 = (*bp04 & 0xf8)|U8(bp0a);
		// SPX: Door step sound
		QUEUE_NOISE_GEN2(GDAT_CATEGORY_DOORS, GET_GRAPHICS_FOR_DOOR(bp08), SOUND_DOOR_STEP, 0xfe, di, si, 1, 0x5f, 0x80);
		if (SkCodeParam::bUseDM2ExtendedMode && bp0a == 4)
			QUEUE_NOISE_GEN2(GDAT_CATEGORY_DOORS, GET_GRAPHICS_FOR_DOOR(bp08), SOUND_DOOR_CLOSE, 0xfe, di, si, 1, 0x5f, 0x80);
		else if (SkCodeParam::bUseDM2ExtendedMode && bp0a == 0)
			QUEUE_NOISE_GEN2(GDAT_CATEGORY_DOORS, GET_GRAPHICS_FOR_DOOR(bp08), SOUND_DOOR_OPENED, 0xfe, di, si, 1, 0x5f, 0x80);

		if (bp08->Bit09() != 0) {
			if (bp0a != 0)
				bp18 = 1;
		}
		else if (bp0a != 4) {
			bp18 = 1;
		}
		//printf("step : bp0a = %d\n", bp0a);
	}
	//^3A15:0AA5
	if (bp18 != 0) {
		bp08->Bit12(1);
		QUEUE_TIMER(ref);
		return;
	}
	//^3A15:0AC1
	bp08->Bit10(0);
	//^3A15:0AC9
	return;
}
//^3A15:1DA8
// SPX: interesting ... If xx = 0 => 1, if xx = 2
X16 SkWinCore::_3a15_1da8(X8 xx, X8 yy)
{
	//^3A15:1DA8
	ENTER(0);
	//^3A15:1DAB
	switch (xx) {
	case 0: //^_1db9
		//^3A15:1DB9
		return 1;
	case 2: //^_1dc2
		//^3A15:1DC2
		return yy ^1;
	}
	//^3A15:1DBE
	return 0;
}

//^3A15:05F7
void SkWinCore::_3a15_05f7(X16 xx)
{
	//^3A15:05F7
	ENTER(0);
	//^3A15:05FA
	if (_4976_4762 >= 0) 
		_3a15_0486(_4976_4762);
	//^3A15:060A
	_3a15_0486(GET_TIMER_NEW_INDEX(xx));
	//^3A15:0618
	return;
}

//^44C8:1DFC
void SkWinCore::_44c8_1dfc(SRECT *prc, U8 colorkey)
{
	//^44C8:1DFC
	ENTER(0);
	//^44C8:1DFF
	FIRE_FILL_RECT_ANY(NULL, prc, colorkey, 0x140, 8);
	//^44C8:1E18
	return;
}
//^3929:086F
void SkWinCore::_3929_086f()
{
	//^3929:086F
	ENTER(12);
	//^3929:0875
	SRECT bp0c;
	QUERY_EXPANDED_RECT(RECT_BOTTOM_MESSAGE_3_LINES, &bp0c);
	X16 di = bp0c.y;
	U16 si = 0;
	for (; si < 1; si++) {
		//^3929:088B
		U32 bp04 = tlbTimerTickRemoveHintMessage[si];	// get the tick at which the text message has to disappear
		if (bp04 == 0xffffffff)
			continue;
		//^3929:08AA
		if (bp04 > glbGameTick)
			continue;
		//^3929:08BE
		X16 bp0a = di +(si * _4976_013a) + _4976_5c12;
		X16 bp06 = _4976_0130;
		FIRE_HIDE_MOUSE_CURSOR();
		_44c8_1dfc(&bp0c, glbPaletteT16[COLOR_BLACK]);	// SPX: Fill low text panel with BLACK
		FIRE_SHOW_MOUSE_CURSOR();
		//tlbTimerTickRemoveHintMessage[si] = 0xffffffff;
		tlbTimerTickRemoveHintMessage[si] = -1;
		//^3929:0907
	}
	//^3929:0910
	return;
}
//^0CEE:04E5
X16 SkWinCore::_0cee_04e5(X16 xx, X16 yy, X16 zz, i16 ss, i16 tt)
{
	//^0CEE:04E5
	ENTER(0);
	//^0CEE:04E8
	CALC_VECTOR_W_DIR(xx, yy, zz, &ss, &tt);
	//^0CEE:0502
	return GET_TILE_VALUE(ss, tt);
}
//^2759:12E6
void SkWinCore::_2759_12e6()
{
	//^2759:12E6
	ENTER(10);
	//^2759:12EC
	i16 bp0a = -1;
	U16 si = 0;
	for (; si < glbChampionsCount; si++) {
		//^2759:12F6
		i16 di = 0;
		for (; di <= 1; di++) {
			//^2759:12FB
			sk536e *bp04 = &glbChampionEngagingHandCommand[si][di];
			if (bp04->w0 == 0)
				continue;
			//^2759:1323
			Champion *bp08 = &glbChampionSquad[si];
			if (bp08->curHP() == 0 || bp04->id4 != bp08->Possess(di)) {
				//^2759:1354
				bp04->w0 = 0;
				continue;
			}
			//^2759:135E
			if (bp04->b2 != 0) {
				bp04->b2 = 0;
				continue;
			}
			//^2759:136F
			bp04->w0--;
			bp0a = glbSelectedHandAction;
			glbSelectedHandAction = di;
			IS_ITEM_HAND_ACTIVABLE(si, bp04->id4, di);
			ENGAGE_COMMAND(si, bp04->b3|0x8000);
			glbSelectedHandAction = bp0a;
			//^2759:13A7
		}
		//^2759:13B0
	}
	//^2759:13BA
	si = glbChampionIndex;
	if (si != 0 && bp0a >= 0 && bp0a <= 1) {
		//^2759:13CF
		si--;
		IS_ITEM_HAND_ACTIVABLE(si, glbChampionSquad[si].Possess(glbSelectedHandAction), glbSelectedHandAction);
	}
	//^2759:13F1
	return;
}

//^13AE:005C
void SkWinCore::GAME_LOOP()
{
	int iLoopCount = 0;
	//printf("Entering GAME_LOOP\n"); getch();
	SkCodeParam::bUseIngameDebug = true;
	if (SkCodeParam::bUseIngameDebug)
	{
		U8 message[64];
		sprintf((char*)message, "RAM = %08d / EMS = %08d\n", glbFreeRAMMemPool, glbFreeEMSMemPool);
		DISPLAY_HINT_TEXT(COLOR_YELLOW, message);
	}
	//^13AE:005C
	ENTER(0);
	//^13AE:0061
	glbTickSpeed = stdTickBalance;
	glbGameHasEnded = 0;
	FILL_ORPHAN_CAII();
	X16 si;
	while (true)
	{
		//printf("GAME_LOOP: Begin loop %d\n", iLoopCount++); getch();
		U8 message[64];
		//sprintf((char*)message, "GAME LOOP %08d\n", iLoopCount++);
		//DISPLAY_HINT_TEXT(COLOR_YELLOW, message);
		// SPX get speed from window menu
		stdTickBalance = skwin.spfact*4;
		glbTickSpeed = stdTickBalance;
		// SPX

		//^13AE:0072
		glbIntermediateTickCounter = 0;
		if (glbMapToLoad == 0xffff)
			goto _00a4;
		while (true)
		{
			//printf("GAME_LOOP: MOVE_RECORD_TO\n"); getch();
			
			//^13AE:007F
			LOAD_NEWMAP(U8(glbMapToLoad));
			MOVE_RECORD_TO(OBJECT_NULL, -1, 0, glbPlayerPosX, glbPlayerPosY);
			glbMapToLoad = 0xffff;
			//^13AE:00A4
_00a4:
			//printf("GAME_LOOP: PROCEED_TIMERS\n"); getch();
			PROCEED_TIMERS();
			if (glbMapToLoad != 0xffff)
				continue;
			break;
		}
		//printf("GAME_LOOP: UPDATE_WEATHER\n"); getch();
		//^13AE:00B0
		UPDATE_WEATHER(0);
		
		SkD((DLV_DBG_RAIN, "Loop (Rain) >> lvl=%03d / strm=%03d / wet=%03d (r2:%d r3:%d mlt:%d) / tick=%d\n"
			, glbRainStrength, glbRainStormController
			, glbRainLevelForGround
			, glbRainRelated2, glbRainRelated3, glbRainMultiplicator
			, glbGameTick
			));

		if (glbIsPlayerSleeping == 0) {
			//^13AE:00C2
			if (glbNextChampionNumber == 0)
				//^13AE:00C9
				_38c8_0060();
			//^13AE:00CE
			//printf("GAME_LOOP: VIEWPORT\n"); getch();
			if (glbChampionInventory == 0) {
				//^13AE:00D5
				if (glbDoLightCheck != 0)
					//^13AE:00DC
					CHECK_RECOMPUTE_LIGHT(glbPlayerPosX, glbPlayerPosY);
				//^13AE:00EB
				if (glbIsPlayerMoving != 0)
					//^13AE:00F2
					DISPLAY_VIEWPORT(_4976_4c40, _4976_4c32, _4976_4c34);
				else
					//^13AE:0100
					DISPLAY_VIEWPORT(glbPlayerDir, glbPlayerPosX, glbPlayerPosY);
				//^13AE:0114
				CHANGE_VIEWPORT_TO_INVENTORY(1);
				_4976_4bc8 = 0;
			}
			//^13AE:0122
			if (glbIsPlayerMoving != 0) {
				//^13AE:0129
				if (glbIsPlayerMoving == 1) {
					//^13AE:0130
					//printf("Pending PERFORM MOVE\n");
					PERFORM_MOVE(glbPlayerLastMove);
				}
				//^13AE:013A
				glbIsPlayerMoving--;
			}
			//^13AE:013E
			_4976_4e64 = 0;
			if (glbShowMousePointer != 0) {
				//^13AE:014B
				glbShowMousePointer = 0;
				_443c_0434();
			}
			//^13AE:0156
			//printf("GAME_LOOP: IBMIO_USER_INPUT_CHECK\n"); getch();
			IBMIO_USER_INPUT_CHECK();
		}
		//printf("GAME_LOOP: updates\n"); getch();
		//^13AE:015B
		_12b4_0092();
		_482b_05bf(0);
		PROCESS_PLAYERS_DAMAGE();
		if (glbGlobalSpellEffects.AuraOfSpeed != 0)
			//^13AE:0174
			glbGlobalSpellEffects.AuraOfSpeed--;
		/// SPX: glbGameTick is a general tick counter : each 16 or 64 then do update
		if ((X16(glbGameTick) & ((glbIsPlayerSleeping != 0) ? 15 : 0x3f)) == 0)
			//^13AE:018D
			UPDATE_CHAMPIONS_STATS();
		//^13AE:0192
		GLOBAL_UPDATE_UNKNOW1();
		_2e62_0cfa(1);
		if (glbPlayerDefeated != 0)
			return;
		glbGameTick++;
		//ATLASSERT(ValidateMements());
		PROCESS_QUEUED_DEALLOC_RECORD();
		//^13AE:01B3
		SkD((DLV_SYS,"SYS: Tick increased to %u ------------------------------\n", (Bitu)glbGameTick));
		SkD((DLV_DBG_SEED,"DBG: t %08X Seed %08X \n", (Bitu)glbGameTick, (Bitu)glbRandomSeed));
		SkD((DLV_DBG_SED3,"DBG: v %02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X \n"
			, (Bitu)(Bit8u)glbTabCreaturesInfo[1].Command
			, (Bitu)(Bit8u)glbTabCreaturesInfo[1].b27
			, (Bitu)(Bit8u)glbTabCreaturesInfo[1].b28
			, (Bitu)(Bit8u)glbTabCreaturesInfo[1].b29
			, (Bitu)(Bit8u)glbTabCreaturesInfo[1].ItemToThrow
			, (Bitu)(Bit8u)glbTabCreaturesInfo[1].b31
			, (Bitu)(Bit8u)glbTabCreaturesInfo[1].b32
			, (Bitu)(Bit8u)glbTabCreaturesInfo[1].b33
			));
		if ((X16(glbGameTick) & 0x1ff) == 0)	// every 511 tick, burn lighting items
			//^13AE:01BB
			BURN_PLAYER_LIGHTING_ITEMS();
		//^13AE:01C0
		// SPX: This is the freeze value
		if (glbGlobalSpellEffects.FreezeCounter != 0)
		{
			//^13AE:01C7
			glbGlobalSpellEffects.FreezeCounter = glbGlobalSpellEffects.FreezeCounter -1;
		}
		//^13AE:01CF
		if (_4976_4c00 != 0)
			_4976_4c00--;
		//^13AE:01DA
		if (glbPlayerThrowCounter != 0)
			glbPlayerThrowCounter--;
		//^13AE:01E5
		_3929_086f();
		glbTickStepReached = 0;
		_12b4_0092();
		if (false) {
			//^13AE:01F7
_01f7:
			_1031_0d36(0x20, _476d_050e());
		}
		MessageLoop(true); // in game
		do {
			//^13AE:0206
			if (_476d_05a9() != 0) {
				//^13AE:020D
				goto _01f7;
			}
			//^13AE:020F
			if (_4976_4c3e != 0) {
				//^13AE:0216
				X16 di;
				di = _0cee_04e5(glbPlayerDir, 1, 0, glbPlayerPosX, glbPlayerPosY);
				if ((di >> 5) != 6 || (di & 1) == 0 || (di & 4) != 0) {
					//^13AE:0247
					_4976_4c3e = 0;
					FIRE_MOUSE_RELEASE_CAPTURE();
					glbMouseVisibility = 1;
					FIRE_SHOW_MOUSE_CURSOR();
				}
			}
			//^13AE:025D
			MAIN_LOOP();
			//^13AE:0262

			MessageLoop(false); // in game

		} while (glbTickStepReached == 0 || _4976_4c02 == 0);
		//^13AE:0270
		si = glbMapToLoad;
		if (si != 0xffff) {
			//^13AE:027A
			si = glbCurrentMapIndex;
			CHANGE_CURRENT_MAP_TO(glbMapToLoad);
		}
		//^13AE:0288
		_2759_12e6();
		//^13AE:028D
		if (si != 0xffff) {
			//^13AE:0295
			CHANGE_CURRENT_MAP_TO(si);
		}
		//DM2DOS_R_BA7(ddata.v1e0266);
		//if (si != 0xFFFF)
		if (SkCodeParam::bMIDIMusicEnabled)
			skmidi->REQUEST_PLAY_MUSIC_FROM_MAP(glbPlayerMap);
		else if (SkCodeParam::bWAVMusicEnabled)
			REQUEST_PLAY_MUSIC_FROM_MAP(glbPlayerMap);
		continue;
	}
	//^13AE:029F
	return;
}

//^13AE:02A3
// SPX: _13ae_02a3 renamed FIRE_MAIN
i16 SkWinCore::FIRE_MAIN(i16 argc, char **argv, char **env) //#DS=4976
{
	//^13AE:02A3
	ENTER(0);
	//^13AE:02A7
#if UseAltic
	;
#else
	i16 si;
	for (si = 1; si < argc; si++) {
		//^13AE:02AD
		if (argv[si][0] == '+') {
			//^13AE:02C0
			if (_069a_035b(argv[si][1]) == 'G') {
				//^13AE:02DE
				if (_069a_035b(argv[si][2]) == 'L') {
					//^13AE:02FE
					switch (_069a_035b(argv[si][3])) {
						case 'C': //^_0323
							_4976_4806 = 0x60;
							break;
						case 'K': //^_032a
							_4976_4806 = 0x50;
							break;
					}
				}
			}
		}
		//^13AE:032F
	}
	//^13AE:0338
	if (_sk_setjmp(_4976_4de0) == 0) {
	    _4726_03b2();
		_38c8_04aa();
		while (true) {
			GAME_LOOP();
			END_GAME(glbPlayerDefeated);
		}
	}
	//^13AE:0361
#endif

	//^13AE:0338
    _4726_03b2();	// _4726_03b2
	INIT();	// _38c8_04aa

	// SPX: Add some more init here, just before starting the GAME_LOOP
#ifdef __USE_ALLEGRO__
	al_init();
	al_install_audio();
	al_init_acodec_addon();
#endif // __USE_ALLEGRO__
	EXTENDED_LOAD_SPELLS_DEFINITION();
	EXTENDED_LOAD_DM1_ITEM_CONVERSION_LIST();

	while (true) {
		GAME_LOOP();
		END_GAME(glbPlayerDefeated);
	}
	//^13AE:0361
	return 0;
}


//^069A:0000
int SkWinCore::FIRE_BOOTSTRAP() //#DS=089C
{
	//^069A:0000
	// 4F75:0FE8  F8 0F 75 4F/04 10 75 4F/08 10 75 4F/00 00 00 00  ÅEuO..uO..uO....
	// 4F75:0FF8  43 3A 5C 46 49 52 45 2E 45 58 45 00 2B 70 6D 00  C:\FIRE.EXE.+pm.
	// 4F75:1008  2B 73 62 00 75 49 00 00 5A 00 00 88 4F 00 00 00  +sb.uI..Z..ÅE...
	skmidi = new SkWinMIDI(skwin.dung, skwin.sCustomDataFolder, (const char*) GET_DATA_FOLDER_NAME(skwin.dung));
	char *argv[] = {"FIRE.exe", "+pm", "+sb"};
	return (FIRE_MAIN(3, argv, NULL));
}

//^0088:0230
i16 SkWinCore::SK88_TOUPPER(i16 c)
{
	//^0088:0230
	ENTER(0);
	//^0088:0233
    i16 dx = c;
	if (dx == -1)
		return -1;
	//^0088:0240
	U8 dl = U8(dx);
	if ((_04bf_067f[dl] & 8) != 0)
		//^0088:024D
		return dl -0x20;
	//^0088:0256
	return dl;
}
//^0088:02BB
U8* SkWinCore::SK88_STRCPY(U8 *strDestination, const U8 *strSource) {
	return reinterpret_cast<U8 *>(strcpy(
		reinterpret_cast<char *>(strDestination), 
		reinterpret_cast<const char *>(strSource)
		));
}
//^0088:027C
U8* SkWinCore::SK88_STRCAT(U8 *strDestination, const U8 *strSource) {
	return reinterpret_cast<U8 *>(strcat(
		reinterpret_cast<char *>(strDestination), 
		reinterpret_cast<const char *>(strSource)
		));
}
//^01B0:2B27
i16 SkWinCore::IBMIO_EXEC(const U8 *exe, const U8 *arg) {
	SkD((DLV_FYI, "EXEC: %s %s\n", exe, arg));
	if (_stricmp(reinterpret_cast<const char *>(exe), "FIRE.exe") == 0) {
		return FIRE_BOOTSTRAP();
	}
	if (_stricmp(reinterpret_cast<const char *>(exe), "anim") == 0) {
		if (_stricmp(reinterpret_cast<const char *>(arg), "swoosh +pm +sb") == 0) {
			return ANIM_BOOTSTRAP_SWOOSH();
		}
		else if (_stricmp(reinterpret_cast<const char *>(arg), "title +ah +as +ab +pm +sb") == 0) {
			return ANIM_BOOTSTRAP_TITLE();
		}
	}
	return 0;
}
//^01B0:2C1C
void SkWinCore::IBMIO_PRINT_ERROR(const U8 *str) //#DS=04BF
{
#if UseAltic
	fputs(reinterpret_cast<const char *>(str), stdout);
#else
	//^01B0:2C1C
	ENTER(2);
	//^01B0:2C20
	LOADDS(0x3083);

	// int 10h
	// http://hp.vector.co.jp/authors/VA003720/lpproj/int10h/int10h.htm

	// int 21h
	// http://www2.neweb.ne.jp/wd/wonder/codea.htm

	//^01B0:2C26
	__asm mov ah,0x00
	__asm mov al,0x13
	__asm int 0x10
	//^01B0:2C2C
	U8 bp01;
	while ((bp01 = *(str++)) != 0) {
		//^01B0:2C2E
		__asm mov dl,[bp01]
		__asm mov ah,2
		__asm int 0x21
	}
	//^01B0:2C46
	return;
#endif
}
void SkWinCore::_0088_020b(X16 xx) { // TODO: Unr
	Unr();
}
//^01B0:2BE0
U16 SkWinCore::IBMIO_CHECK_CPU_ERA() {
	return 386;
}

void SkWinCore::_sys_setvect(i16 interruptno, void (SkWinCore::*isr)()) {
	intrvect[interruptno & 255] = isr;
}

//	void (SkWinCore::*_089c_0340)();
//void (sksave_converter::*_sys_getvect(U16 interruptno))() {
//void (SkWinCore::*_sys_getvect(U16 interruptno))() {
//void (SkWinCore::*pfn)() SkWinCore::_sys_getvect(U16 interruptno) {
void (SkWinCore::*SkWinCore::_sys_getvect(U16 interruptno))() {
	return intrvect[interruptno & 255];
}

//^0088:026B
void SkWinCore::_crt88_setvect(i16 interruptno, void (SkWinCore::*isr)()) {
#if UseAltic
	_sys_setvect(interruptno, isr);
#else
#error	Unr
#endif
}
//^01B0:1F3D
U16 SkWinCore::IBMIO_DETECT_SBLASTER()
{
#if UseAltic
	return 1;
#else
	//^01B0:1F3D
	ENTER(2);
	//^01B0:1F41
	U16 bp02 = 0x210;
	for (; bp02 < 0x260; bp02 += 16) {
		//^01B0:1F49

		xxx;
		//^01B0:1FE3
	}
	//^01B0:1FF1
	return 0;
#endif
}
//^00EB:0C21
void SkWinCore::IBMIO_INIT_VID()
{
#if UseAltic
	_04bf_09de = 0;
#else
	//^00EB:0C21
	ENTER(0);
	//^00EB:0C24
	_crt88_setvect(255, _04bf_0090);
	__asm mov ah,0xf
	__asm int 0x10
	__asm mov _04bf_09de, al
	__asm mov ah,0
	__asm mov al,0x13
	__asm int 0x10
	//^00EB:0C4A
	return;
#endif
}
//^01B0:0A6A
X16 SkWinCore::IBMIO_INIT_MOUSE()
{
#if UseAltic
	return _04bf_18b2 = 1;
#else
	//^01B0:0A6A
	ENTER(0);
	//^01B0:0A6D

	// int 33h
	// http://www2.muroran-it.ac.jp/circle/mpc/old/pc98dos/mouse/mouse.html

	__asm mov ax,0
	__asm int 0x33
	__asm mov _04bf_18b2, ax
	if (_04bf_18b2 == 1) {
		__asm mov cx,0
		__asm mov dx,0
		__asm int 0x33
		__asm mov cx,0
		__asm mov dx,0x27f
		__asm mov ax,0x7
		__asm int 0x33
		__asm mov cx,0
		__asm mov dx,0xc7
		__asm mov ax,0x8
		__asm int 0x33
	}
	//^01B0:0AA3
	return _04bf_18b2;
#endif
}
void SkWinCore::_04bf_0090() { // TODO: Unr
	Unr();
}

//^01B0:0426
void SkWinCore::INIT_KBOARD_HOOK() //#DS=04BF
{
#if UseAltic
	//^01B0:0426
	ENTER(0);
	//^01B0:042B
	_04bf_1886 = 0;
	_04bf_1864 = 0;
	_04bf_185a = 0;
	_04bf_0e7e = 0;
	_04bf_185c = 0;
	_04bf_18a8 = 0;
	_04bf_1862 = 0;
	//^01B0:0457
	_01b0_0453 = _sys_getvect(0x09);
	_sys_setvect(0x09, &SkWinCore::IBMIO_KBOARD_HANDLER);
	//^01B0:0475
	//^01B0:04B5
	_04bf_02bc = 1;
	//^01B0:04BB
	return;
#else
#error	Unr
#endif
}
//^01B0:0478
void SkWinCore::IBMIO_KBOARD_HANDLER() { //#DS=04BF
#if UseAltic
	//^01B0:0478
	//^01B0:0482
	LOADDS(0x3083);
	_01b0_00fc();
	if (glbDMode == 0) {
		//^01B0:0499
		// MessageBeep(-1);
		return;
	}
	return;
#else
#error	Unr
#endif
}

//^01B0:00CE
void SkWinCore::_01b0_00ce(X16 xx)
{
	//^01B0:00CE
	ENTER(0);
	//^01B0:00D1
	if (_04bf_1886 < 10) {
		//^01B0:00DA
		_04bf_188e[_04bf_185a] = xx;
		_04bf_185a = (_04bf_185a +1) % 10;
		_04bf_1886++;
	}
	//^01B0:00F9
	//^01B0:00FA
	return;
}

//^01B0:00FC
void SkWinCore::_01b0_00fc()
{
	//^01B0:00FC
	ENTER(6);
	//^01B0:0102
	do {
		U16 bp06 = SK_IMPORTB(0x60);
		if (bp06 == 0xe0) {
			//^01B0:0110
			_04bf_0e7e |= 0x1000;
			continue;
		}
		//^01B0:0119
		X16 si = 0;
		U16 *bp04;
		if ((_04bf_0e7e & 0x1000) != 0) {
			//^01B0:0123
			bp04 = _04bf_0e6a;
			si = 1;
		}
		else {
			//^01B0:0130
			bp04 = _04bf_0e54;
			if ((_04bf_0e7e & 0x800) != 0) {
				//^01B0:0140
				_04bf_1862 = 1;
				switch (bp06) {
				case 72://^0158
					//^01B0:0158
					_04bf_18a8 |= 1; break;
				case 75://^015F
					//^01B0:015F
					_04bf_18a8 |= 4; break;
				case 77://^0166
					//^01B0:0166
					_04bf_18a8 |= 8; break;
				case 80://^016D
					//^01B0:016D
					_04bf_18a8 |= 2; break;
				case 71://^0174
					//^01B0:0174
					_04bf_18a8 |= 0x20; break;
				case 73://^017B
					//^01B0:017B
					_04bf_18a8 |= 0x40; break;
				case 79://^0182
					//^01B0:0182
					_04bf_18a8 |= 0x80; break;
				case 81://^018A
					//^01B0:018A
					_04bf_18a8 |= 0x100; break;
				case 78://^0192
					//^01B0:0192
					_04bf_18a8 |= 0x10; break;
				case 74://^0199
				case 76://^0199
				default:
					//^01B0:0199
					_04bf_1862 = 0; break;
				}
			}
		}
		//^01B0:019F
		if (_04bf_1862 == 0) {
			//^01B0:01AB
			if ((_04bf_0e7e & 0x1000) == 0) {
				//^01B0:01B8
				X16 di = bp06;
				switch (di) {
				case 0x2A://^01D1
					//^01B0:01D1
					_04bf_0e7e = _04bf_0e7e|0x8000; break;
				case 0x36://^01DD
					//^01B0:01DD
					_04bf_0e7e |= 0x4000; break;
				case 0x1D://^01E5
					//^01B0:01E5
					_04bf_0e7e |= 0x400; break;
				case 0x38://^01ED
					//^01B0:01ED
					_04bf_0e7e |= 0x800; break;
				case 0xAA://^01F5
					//^01B0:01F5
					_04bf_0e7e = _04bf_0e7e & 0x7fff; break;
				case 0xB6://^01FD
					//^01B0:01FD
					_04bf_0e7e &= 0xbfff; break;
				case 0x9D://^0205
					//^01B0:0205
					_04bf_0e7e &= 0xfbff; break;
				case 0xB8://^020D
					//^01B0:020D
					_04bf_0e7e &= 0xf7ff; break;
				case 0xC8://^0215
					//^01B0:0215
					_04bf_18a8 &= 0xfffe; break;
				case 0xCB://^021D
					//^01B0:021D
					_04bf_18a8 &= 0xfffb; break;
				case 0xCD://^0225
					//^01B0:0225
					_04bf_18a8 &= 0xfff7; break;
				case 0xD0://^022D
					//^01B0:022D
					_04bf_18a8 &= 0xfffd; break;
				case 0xC7://^0235
					//^01B0:0235
					_04bf_18a8 &= 0xffdf; break;
				case 0xC9://^023D
					//^01B0:023D
					_04bf_18a8 &= 0xffbf; break;
				case 0xCF://^0245
					//^01B0:0245
					_04bf_18a8 &= 0xff7f; break;
				case 0xD1://^024D
					//^01B0:024D
					_04bf_18a8 &= 0xfeff; break;
				case 0xCE://^0255
					//^01B0:0255
					_04bf_18a8 &= 0xffef; break;
				default:
					//^01B0:025D
					si = 1; break;
				}
			}
			//^01B0:0260
			if (si != 0 && (bp06 & 0x80) == 0) {
				//^01B0:0275
				switch (bp06) {
					case 0x3b://^_0284
						//^01B0:0284
						if (_04bf_0296 > 0)
							_04bf_0296--;
						//^01B0:028F
		_028f:
						_04bf_0298 = _04bf_029c[RCJ(8,_04bf_0296)];
						_04bf_029a = _04bf_02ac[RCJ(8,_04bf_0298)];
						break;
					case 0x3c://^_02ab
						//^01B0:02AB
						if (_04bf_0296 < 7)
							_04bf_0296++;
						goto _028f;
					default: //^_02ba
						//^01B0:02BA
						if ((bp04[(bp06 >> 4)&7] & (1 << (bp06&15))) == 0) {
							//^01B0:02DA
							_01b0_00ce((_04bf_0e7e & 0x1e00)|bp06);
							break;
						}
				}
			}
		}
		//^01B0:02EB
		if ((bp06 & 0x80) != 0) {
			//^01B0:02F2
			bp04[(bp06 >> 4)&7] &= ~(1 << (bp06&15));
		}
		else {
			//^01B0:0314
			bp04[(bp06 >> 4)&7] |=   1 << (bp06&15);
		}
		//^01B0:0332
		if ((_04bf_0e7e & 0x8000) != 0 || (_04bf_0e7e & 0x4000) != 0) {
			//^01B0:0342
			_04bf_0e7e |= 0x200;
		}
		else {
			//^01B0:034A
			_04bf_0e7e &= 0xfdff;
		}
		//^01B0:0350
		if ((_04bf_18a8 & 0x61) != 0) {
			//^01B0:0358
			_04bf_185c |= 1;
		}
		else {
			//^01B0:035F
			_04bf_185c &= 0xfffe;
		}
		//^01B0:0365
		if ((_04bf_18a8 & 0x182) != 0) {
			//^01B0:036D
			_04bf_185c |= 2;
		}
		else {
			//^01B0:0374
			_04bf_185c &= 0xfffd;
		}
		//^01B0:037A
		if ((_04bf_18a8 & 0x148) != 0) {
			//^01B0:0382
			_04bf_185c |= 8;
		}
		else {
			//^01B0:0389
			_04bf_185c &= 0xfff7;
		}
		//^01B0:038F
		if ((_04bf_18a8 & 0xa4) != 0) {
			//^01B0:0397
			_04bf_185c |= 4;
		}
		else {
			//^01B0:039E
			_04bf_185c &= 0xfffb;
		}
		//^01B0:03A4
		if ((_04bf_18a8 & 0x10) != 0) {
			//^01B0:03AC
			_04bf_185c |= 0x10;
		}
		else {
			_04bf_185c &= 0xffef;
		}
		//^01B0:03B9
		_04bf_0e7e &= 0xefff;
		//^01B0:03BF
	} while ((SK_IMPORTB(0x64) & 2) != 0);
	//^01B0:03C8
	return;
}

void SkWinCore::_01b0_18ae() { // TODO: Unr
	Unr();
}
X16 SkWinCore::_01b0_292b() { // TODO: Unr
	Unr(); return 0;
}
void SkWinCore::_01b0_237f() { // TODO: Unr
	Unr();
}
//^01B0:20FF
void SkWinCore::_01b0_20ff()
{
	//^01B0:20FF
	ENTER(0);
	//^01B0:2102
    if (glbCounterZero_0517 == 0)
		return;
	if (glbSomeCounter_0519 <= 0) {
		do {
			//^01B0:2119
			glbSomeCounter_0519 += _01b0_292b() * 240;
			//^01B0:212C
		} while (glbSomeCounter_0519 <= 0);
		//^01B0:213C
		if (_04bf_0e64 != 0) {
			//^01B0:2143
			_04bf_18a4 = 0;
			_01b0_237f();
			glbCounterZero_0517 = 0;
		}
	}
	else {
		//^01B0:2156
		glbSomeCounter_0519 -= glbDecreasing120;
	}
	//^01B0:2162
	return;
}

//^01B0:0E2C
U16 SkWinCore::_01b0_0e2c()
{
	//^01B0:0E2C
	ENTER(0);
	//^01B0:0E2C
	outportb(0x43,0x80);
	U8 al = SK_IMPORTB(0x42);
	U8 ah = SK_IMPORTB(0x42);
	return (ah<<8)|al;
	//^01B0:0E43
}

//^01B0:0E27
X16 SkWinCore::_01b0_0e27(X16 xx) 
{
	//^01B0:0E27
	ENTER(0);
	//^01B0:0E2A
	while ((SK_IMPORTB(0x0201) & 3) != 0) {
	}
	U16 tmp0 = _01b0_0e2c();
	outportb(0x0201, U8(tmp0));
	U8 al;
	U8 bl = U8(xx);
	while ((bl & (al = SK_IMPORTB(0x0201))) != 0) {
	}
	if (bl >= al) {
		U16 tmp1 = _01b0_0e2c();
		if (tmp0 < tmp1) {
			return 0xffff -tmp1 +tmp0;
		}
		else {
			return tmp1 -tmp0;
		}
	}
	else {
		return 0;//unstable value
	}
	//^01B0:0E7E
}

//^01B0:0FA3
void SkWinCore::_01b0_0fa3()
{
	//^01B0:0FA3
	ENTER(0);
	//^01B0:0FA7
	if (glbPType == 1 && sysMousePositionCaptured == 0 && sysSomeSemaphore == 0) {
		//^01B0:0FCB
		sysSomeSemaphore = 1;
		outportb(0x20,0x20);
		if ((_04bf_0e4c++ & 1) != 0) {
			//^01B0:0FE2
			_04bf_1798 = (_01b0_0e27(1) -100) / _04bf_0298;
		}
		else {
			//^01B0:0FF6
			_04bf_03c8 = _04bf_1798;
			if (_04bf_03c8 > 0x13f) {
				//^01B0:1004
				_04bf_03c8 = 0x13f;
			}
			//^01B0:100C
			else if (_04bf_03c8 < 0) {
				//^01B0:1013
				_04bf_03c8 = 0;
			}
			//^01B0:1019
			_04bf_03ca = (_01b0_0e27(2) -100) / _04bf_029a;
			if (_04bf_03ca > 0xc7) {
				//^01B0:1033
				_04bf_03ca = 0xc7;
			}
			//^01B0:103B
			else if (_04bf_03ca < 0) {
				//^01B0:1042
				_04bf_03ca = 0;
			}
		}
		//^01B0:1048
		_04bf_03d0 = SK_IMPORTB(0x201);
		_04bf_03d0 = ((_04bf_03d0 >> 4) ^ 3)&3;
		sysSomeSemaphore = 0;
	}
	else {
		//^01B0:1067
		if (glbPType != 2 && glbPType != 3)
			return;
		//^01B0:107A
		_04bf_179a = (glbSoundBlasterBasePort != 0 && glbPType == 2) ? (SK_IMPORTB(_01b0_3378 +1)) : 0x78;
		//^01B0:109B
		if (_04bf_03d2 == 1) {
			if ((_04bf_179a & 0x80) == 0) {
				//^01B0:10A9
				_04bf_03d2 = 0;
			}
			//^01B0:10B1
			else if (((_04bf_179a ^ _04bf_0e4a)&0x78) != 0) {
				//^01B0:10BC
				if ((_04bf_179a & 8) == 0) {
					//^01B0:10C3
					_01b0_00ce(0x48);
				}
				else if ((_04bf_179a & 16) == 0) {
					//^01B0:10CE
					_01b0_00ce(0x50);
				}
				else if ((_04bf_179a & 32) == 0) {
					//^01B0:10D9
					_01b0_00ce(0x47);
				}
				else if ((_04bf_179a & 64) == 0) {
					//^01B0:10E4
					_01b0_00ce(0x49);
				}
			}
		}
		//^01B0:10EB
		if (_04bf_03d2 == 0) {
			//^01B0:10F7
			if (sysMousePositionCaptured == 0) {
				//^01B0:1103
				if ((_04bf_179a & 0x78) != 0x78 || (_04bf_185c & 15) != 0) {
					//^01B0:111D
					if ((_04bf_179a & 8) == 0 || (_04bf_185c & 1) != 0) {
						//^01B0:112C
						_04bf_03ca -= ((++_04bf_03ce +1) / 3) +1;
					}
					//^01B0:113F
					if ((_04bf_179a & 0x10) == 0 || (_04bf_185c & 2) != 0) {
						//^01B0:114E
						_04bf_03ca += ((++_04bf_03ce +1) / 3) +1;
					}
					//^01B0:1161
					if ((_04bf_179a & 0x40) == 0 || (_04bf_185c & 8) != 0) {
						//^01B0:1170
						_04bf_03c8 += ((++_04bf_03cc +1) / 3) +1;
					}
					//^01B0:1183
					if ((_04bf_179a & 0x20) == 0 || (_04bf_185c & 4) != 0) {
						//^01B0:1192
						_04bf_03c8 -= ((++_04bf_03cc +1) / 3) +1;
					}
				}
				//^01B0:11A5
				if (_04bf_03cc > 0x64)
					//^01B0:11AC
					_04bf_03cc = 0x64;
				//^01B0:11B2
				if (_04bf_03ce > 0x64)
					//^01B0:11B9
					_04bf_03ce = 0x64;
				//^01B0:11BF
				if (_04bf_03c8 < 0)
					//^01B0:11C6
					_04bf_03c8 = 0;
				//^01B0:11CC
				if (_04bf_03c8 > 0x13f)
					//^01B0:11D4
					_04bf_03c8 = 0x13f;
				//^01B0:11DA
				if (_04bf_03ca < 0)
					//^01B0:11E1
					_04bf_03ca = 0;
				//^01B0:11E7
				if (_04bf_03ca > 0xc7)
					//^01B0:11EF
					_04bf_03ca = 0xc7;
			}
			//^01B0:11F5
			if (((_04bf_179a ^ _04bf_0e4a)&0x18) != 0)
				//^01B0:1200
				_04bf_03ce = 0;
			//^01B0:1206
			if (((_04bf_179a ^ _04bf_0e4a)&0x60) != 0)
				//^01B0:1211
				_04bf_03cc = 0;
			//^01B0:1217
			if (((_04bf_185c ^ _04bf_18a6)&3) != 0)
				//^01B0:1223
				_04bf_03ce = 0;
			//^01B0:1229
			if (((_04bf_185c ^ _04bf_18a6)&12) != 0)
				//^01B0:1235
				_04bf_03cc = 0;
			//^01B0:123B
			_04bf_03d0 = ((_04bf_179a >> 7) & 1) | ((_04bf_185c >> 4) & 1);
			if ((_04bf_179a & 0x80) != 0) {
				//^01B0:125C
				if ((_04bf_03c8 >= 0xe8 && _04bf_03c8 <= 0x13f && _04bf_03ca >= 0x7b && _04bf_03ca <= 0xa8) || _04bf_03ca > 0xa8)
					//^01B0:1283
					_04bf_03d2 = 1;
			}
		}
		//^01B0:1289
		_04bf_0e4a = _04bf_179a;
		_04bf_18a6 = _04bf_185c;
	}
	//^01B0:1295
	X16 cx = _04bf_03c8;
	X16 dx = _04bf_03ca;
	X16 bx = _04bf_03d0;
	if (_04bf_0e7a == 0) {
		//^01B0:12AC
		IBMIO_MOUSE_EVENT_RECEIVER(cx, dx, bx);
		return;
	}
	//^01B0:12BA
	if (_04bf_17a4 < 10) {
		//^01B0:12C1
		X16 di;
		if (_04bf_17a4 != 0) {
			//^01B0:12C8
			di = _04bf_0e50;
			if (_04bf_17ac[di].w0 == bx) {
				//^01B0:12DA
				_04bf_17ac[di].w2 = cx;
				_04bf_17ac[di].w4 = dx;
				return;
			}
		}
		//^01B0:12E5
		_04bf_17a4++;
		di = _04bf_0e50 +1;
		//^01B0:12EE
		if (di == 10)
			di = 0;
		//^01B0:12F6
		_04bf_0e50 = di;
		_04bf_17ac[di].w0 = bx;
		_04bf_17ac[di].w2 = cx;
		_04bf_17ac[di].w4 = dx;
		return;
	}
	//^01B0:1312
	return;
}

//^01B0:180C
void SkWinCore::_01b0_180c() {
	//^01B0:180C
	LOADDS(0x3083);
	if (glbDMode == 0 || _04bf_0284 != 0) {
		//^01B0:1823
		_01b0_14d8++;
		if ((_01b0_14d8 & 3) == 0) {
			//^01B0:1830
			(this->*_04bf_18ae)(); INDIRECT_CALL
		}
	}
	//^01B0:1834
	_01b0_20ff();
	//^01B0:183D
	bool carry = U16(_01b0_13c6) < U16(_01b0_13ca);
	_01b0_13c6 -= _01b0_13ca;
	//^01B0:1846
	U16 bx = _01b0_13c8;
	if (bx != _01b0_13ca) {
		//^01B0:1854
		// __asm mov al,0x36
		// __asm out 0x43,al
		// __asm mov al,bl
		// __asm out 0x40,al
		// __asm mov al,bh
		// __asm out 0x40,al
		//^01B0:1861
		_01b0_13ca = bx;
		glbSoundFreq_13cc = glbSoundFreq_13ce;
	}
	//^01B0:186E
	if (!carry) {
		//^01B0:1872
		// __asm mov al,0x20
		// __asm out 0x20,al
		return;
	}
	//^01B0:1878
	LOADDS(0x3083);
	_01b0_0fa3();
	(this->*_01b0_13be)();
}

//^01B0:14EB
void SkWinCore::_INT08_HANDLER() {
	//^01B0:14EB
	_01b0_180c();
}

//^01B0:1315
void SkWinCore::IBMIO_INIT_TIMER()
{
	//^01B0:1315
	ENTER(2);
	//^01B0:131B
	if (glbPType == 1) {
		//^01B0:1322
		// __asm mov al,0xb6
		// __asm out 0x43,al
		// __asm mov al,0
		// __asm out 0x42,al
		// __asm out 0x42,al
		// __asm in al,0x61
		// __asm or al,0x1
		// __asm out 0x61,al
		//^01B0:1335
	}
	else if (glbPType == 2 && glbSoundBlasterBasePort != 0) {
		//^01B0:1345
		// __asm mov dx,glbSoundBlasterBasePort
		// __asm add dx,0x2
		// __asm mov al,4
		// __asm out dx,al
	}
	//^01B0:134F
	_01b0_14db = glbSoundBlasterBasePort;
	if (glbSoundBlasterBasePort != 0 && (glbSoundCardType == 3 || glbSoundCardType == 5)) {
		//^01B0:136D
	}
	//^01B0:1383
	_01b0_13be = _sys_getvect(0x08);
	_sys_setvect(0x08, &SkWinCore::_INT08_HANDLER);
	//^01B0:13A3
	X16 bp02 = 0x136b;
	_01b0_13c2 = _01b0_13c8 = bp02;
	glbSoundFreq_13ce = 0x180c;	// 0x180c = 6156
	//^01B0:13BB
	//^01B0:188F
	return;
}
//^01B0:1893
void SkWinCore::IBMIO_UNINIT_TIMER()
{
	//^01B0:1893
	ENTER(0);
	//^01B0:1896
	_04bf_0284 = 0;
	//^01B0:189C
	while (sndLockSoundBuffer != 0) {
#if UseAltic
		sndLockSoundBuffer = 0;
#endif
	}
	//^01B0:18A4
	outportb(0x43,0x36);
	outportb(0x40,0);
	outportb(0x40,0);
	_sys_setvect(0x08, _01b0_13be);
	_01b0_13ca = 0;
#ifdef __DJGPP__
	outportb(0x61,inportb(0x61) & 0xfc);
#else
	outportb(0x61,inport(0x61) & 0xfc);
#endif	
	//^01B0:18D1
	return;
}
//^01B0:0A39
void SkWinCore::IBMIO_UNINIT_MOUSE()
{
	//^01B0:0A39
	ENTER(0);
	//^01B0:0A3C
	if (_04bf_03d4 != 0) {
		//^01B0:0A45
		_04bf_03d4 = 0;
		_04bf_03c6 = 0;
		_int33_mouse_callback = _01b0_092d;
		//^01B0:0A65
	}
	//^01B0:0A68
	return;
}
//^01B0:04BF
void SkWinCore::IBMIO_UNINIT_KBOARD_HOOK()
{
	//^01B0:04BF
	ENTER(0);
	//^01B0:04C2
	if (_04bf_02bc != 0) {
		//^01B0:04C9
		_04bf_02bc = 0;
		_sys_setvect(0x09, _01b0_0453);
	}
	//^01B0:04E2
	return;
}
//^00EB:0C4C
i16 SkWinCore::IBMIO_UNINIT_VID()
{
#if UseAltic
	return 0;
#else
	//^00EB:0C4C
	ENTER(0);
	//^00EB:0C4F
	__asm mov al,_04bf_09de
	__asm int 0x10
	//^00EB:0C56
	return;
#endif
}

void SkWinCore::zeroMem(void *pv, size_t cb) {
	memset(pv, 0, cb);
}

void SkWinCore::_04bf_0102() { // TODO: Unr
	Unr();
}

//^01B0:2C48
// SPX: _01b0_2c48 replaced by IBMIO_MAIN
i16 SkWinCore::IBMIO_MAIN(i16 argc, char **argv, char **env) //#DS=04BF
{
	// IBMIO_main

	//^01B0:2C48
	ENTER(172);
	//^01B0:2C4E
	U8 bp5c[80] = {0};
	_04bf_17aa = IBMIO_CHECK_CPU_ERA();
	if (_04bf_17aa < 286) {
		//^01B0:2C71
		IBMIO_PRINT_ERROR(strPC286Required);
		_0088_020b(0);
	}
	//^01B0:2C83
	i16 si;
	for (si = 1; si < argc; si++) {
		//^01B0:2C89
		if (argv[si][0] == '+') {
			//^01B0:2C9F
			X16 bp0a;
			X16 bp06;
			X16 bp0c;
			switch (bp0a = SK88_TOUPPER(U8(argv[si][1]))) {
			case 'S'://^2CD5
				//^01B0:2CD5
				switch (SK88_TOUPPER(U8(argv[si][2]))) {
				case 'I'://^2D03
					//^01B0:2D03
					glbSoundCardType = ScardTandy;
					break;
				case 'N'://^2D0C
					//^01B0:2D0C
					glbSoundCardType = 1;
					break;
				case 'T'://^2D15
					//^01B0:2D15
					glbSoundCardType = 4;
					break;
				case 'D'://^2D1E
					//^01B0:2D1E
					glbSoundCardType = 5;
					bp06 = SK88_TOUPPER(argv[si][3]);
					if (bp06 >= '1' && bp06 <= '3') {
						//^01B0:2D4D
						glbSoundBlasterBasePort = _04bf_05f9[RCJ(18,bp06)];
					}
					//^01B0:2D59
					outportb(glbSoundBlasterBasePort +2, inportb(glbSoundBlasterBasePort +2));
					break;
				case 'B'://^2D67
					//^01B0:2D67
					glbSoundBlasterBasePort = IBMIO_DETECT_SBLASTER();
					if (glbSoundBlasterBasePort != 0)
						//^01B0:2D75
						glbSoundCardType = ScardSBlaster;
					break;
				case 'A'://^2D7E
					//^01B0:2D7E
					glbSoundBlasterBasePort = 0x388;
					_04bf_0e4e = 0x388;
					glbSoundCardType = 7;
					break;
				case 'F'://^2D90
					//^01B0:2D90
					glbSoundCardType = 3;
					bp06 = SK88_TOUPPER(argv[si][3]);
					if (bp06 >= '1' && bp06 <= '3')
						//^01B0:2DC5
						glbSoundBlasterBasePort = _04bf_05f9[RCJ(18,bp06)];
					break;
				case 67://^2EAF
				case 69://^2EAF
				case 71://^2EAF
				case 72://^2EAF
				case 74://^2EAF
				case 75://^2EAF
				case 76://^2EAF
				case 77://^2EAF
				case 79://^2EAF
				case 80://^2EAF
				case 81://^2EAF
				case 82://^2EAF
				case 83://^2EAF
					break;
				}
				break;
			case 'P'://^2DD4
				//^01B0:2DD4
				switch (bp0c = SK88_TOUPPER(argv[si][2])) {
				case 'P'://^2E0A
					//^01B0:2E0A
					glbPType = 2;
					break;
				case 'A'://^2E12
					//^01B0:2E12
					glbPType = 1;
					if (argv[si][3] != 0) {
						//^01B0:2E2C
						_04bf_0296 = argv[si][3] -0x30;
						if (_04bf_0296 >= 0 && _04bf_0296 <= 7) {
							//^01B0:2E53
							_04bf_0298 = _04bf_029c[RCJ(8,_04bf_0296)];
							_04bf_029a = _04bf_02ac[RCJ(8,_04bf_0296)];
							break;
						}
						//^01B0:2E6F
						_04bf_0296 = 0;
						break;
					}
					break;
				case 'K'://^2E77
					//^01B0:2E77
					glbPType = 3;
					break;
				case 'M'://^2E7F
					//^01B0:2E7F
					glbPType = 4;
					break;
				}
				goto _2e85;
			case 'M'://^2E85
				//^01B0:2E85
_2e85:
				SK88_TOUPPER(argv[si][2]);
				break;
			case 'Z'://^2EA1
				//^01B0:2EA1
				glbZMode = 0;
				break;
			case 'D'://^2EA9
				//^01B0:2EA9
				glbDMode = 1;
				break;
			}
		}
		//^01B0:2EAF
	}
	//^01B0:2EB8
	IBMIO_INIT_VID();
	_crt88_setvect(0xfe, &SkWinCore::_04bf_0102);
	si = 2;
	i16 di = 0;
	for (; si < argc; bp5c[di] = 0, si++) {
		//^01B0:2ED3
		if (di > 1) {
			//^01B0:2ED8
			bp5c[di++] = 0x20;
		}
		//^01B0:2EDD
		i16 bp04 = 0;
		X8 bp01;
		for (; bp04 < 0x50; bp5c[di] = bp01, di++, bp04++) {
			//^01B0:2EE4
			bp01 = argv[si][bp04];
			if (bp01 == 0)
				break;
			//^01B0:2EFE
		}
		//^01B0:2F0E
	}
	//^01B0:2F18
	X16 bp08;
	if (glbPType == 0 || glbPType == 4) {
		//^01B0:2F26
		bp08 = IBMIO_INIT_MOUSE();
		if (bp08 != 0) {
			//^01B0:2F31
			glbPType = 4;
		}
		else {
			//^01B0:2F39
			glbPType = 3;
		}
	}
	do {
		//^01B0:2F3F
		INIT_KBOARD_HOOK();
		_01b0_08d8();
		if (glbPType == 4 && bp08 != 0)
			IBMIO_SET_MOUSE_HANDLER();
		//^01B0:2F58
		IBMIO_INIT_TIMER();
		if (glbZMode != 0) {
			//^01B0:2F66
			U8 bp00ac[80];
			SK88_STRCPY(bp00ac, _04bf_01b2[RCJ(4,_04bf_0280)].b14);
			SK88_STRCAT(bp00ac, _04bf_067a);
			SK88_STRCAT(bp00ac, bp5c);
			if (_04bf_01b2[RCJ(4,_04bf_0280)].b0[0] == '@') {
				//^01B0:2FB7
				si = IBMIO_EXEC(reinterpret_cast<U8 *>(argv[_04bf_01b2[RCJ(4,_04bf_0280)].b0[1] -'0']), bp00ac);
			}
			else {
				//^01B0:2FE1
				si = IBMIO_EXEC(&_04bf_01b2[RCJ(4,_04bf_0280)].b0[0], bp00ac);
			}
			//^01B0:2FFD
			if (_04bf_01b2[RCJ(4,_04bf_0280)].b46 < si)
				si = 0;
			//^01B0:3013
			_04bf_0280 = _04bf_01b2[RCJ(4,_04bf_0280)].b47 -1;
		}
		else {
			//^01B0:3029
			IBMIO_EXEC(reinterpret_cast<U8 *>(argv[1]), bp5c);
		}
		//^01B0:3040
		_01b0_18d3(0);
		if (glbSoundCardType == 5) {
			//^01B0:304E
			outportb(glbSoundBlasterBasePort +2, inportb(glbSoundBlasterBasePort +2) | 8);
		}
		//^01B0:3059
		IBMIO_UNINIT_TIMER();
		if (glbPType == 4 && bp08 != 0)
			//^01B0:306A
			IBMIO_UNINIT_MOUSE();
		//^01B0:306E
		IBMIO_UNINIT_KBOARD_HOOK();
		//^01B0:3072
	} while (_04bf_0280 > 0);
	//^01B0:307C
	return IBMIO_UNINIT_VID();
}

//^0088:0000
int SkWinCore::IBMIO_BOOTSTRAP()
{
	//^0088:0000
	// 065B:01DA  EE 01 5B 06/FB 01 5B 06/04 02 5B 06/08 02 5B 06  ÅE[.ÅE[...[...[.
	// 065B:01EA  00 00 00 00 43 3A 5C 49 42 4D 49 4F 2E 45 58 45  ....C:\IBMIO.EXE
	// 065B:01FA  00 46 49 52 45 2E 45 58 45 00 2B 70 6D 00 2B 73  .FIRE.EXE.+pm.+s
	// 065B:020A  62 00 BE 04 00 00 5A 00 00 82 99 00 00 00 44 4D  b.æ...Z..Çô...DM
	char *argv[] = {"IBMIO.exe", "FIRE.exe", "+pm", "+sb"};
	try {
		return (IBMIO_MAIN(4, argv, NULL));
	}
	catch (i8) { // termination request at message loop
		return 1;
	}
	//^0088:0215
}

void SkWinCore::_sys_intr() {
}

//}}SKSAVE_PUB_PROC





SkWinCore::SkWinCore() 
{
	//{{INIT_SKVAL1
#include <KSKVAL1.H>
	//}}INIT_SKVAL1

#pragma warning(push)
#pragma warning(disable: 4309)
#include <KSKVAL2.H>
#pragma warning(pop)

// SPX / DM1 special init
	bDM1PortraitsActivated = false;
	zeroMem(xDM1PortraitsData, sizeof(xDM1PortraitsData));
//

	//{{INIT_INSORTED
	__vram = pbVram = vram;
	for (int x=0; x<256; x++)
		intrvect[x] = &SkWinCore::_sys_intr;
	_int33_mouse_callback = &SkWinCore::_sys_intr;

	_4976_0cba[ 0] = &SkWinCore::RETURN_1;
	_4976_0cba[ 1] = &SkWinCore::IS_GAME_ENDED;
	_4976_0cba[ 2] = &SkWinCore::_1031_0023;
	_4976_0cba[ 3] = &SkWinCore::_1031_003e;
	_4976_0cba[ 4] = &SkWinCore::_1031_007b;
	_4976_0cba[ 5] = &SkWinCore::_1031_009e;
	_4976_0cba[ 6] = &SkWinCore::_1031_00c5;
	_4976_0cba[ 7] = &SkWinCore::_1031_00f3;
	_4976_0cba[ 8] = &SkWinCore::_1031_012d;
	_4976_0cba[ 9] = &SkWinCore::_1031_014f;
	_4976_0cba[10] = &SkWinCore::_1031_0184;
	_4976_0cba[11] = &SkWinCore::_1031_01ba;

	_4976_38a4[ 0] = &SkWinCore::_14cd_18cc;
	_4976_38a4[ 1] = &SkWinCore::_14cd_19a4;
	_4976_38a4[ 2] = &SkWinCore::_14cd_1a3c;
	_4976_38a4[ 3] = &SkWinCore::_14cd_1a5a;
	_4976_38a4[ 4] = &SkWinCore::_14cd_1b74;
	_4976_38a4[ 5] = &SkWinCore::_14cd_1b90;
	_4976_38a4[ 6] = &SkWinCore::_14cd_1c27;
	_4976_38a4[ 7] = &SkWinCore::_14cd_1c45;
	_4976_38a4[ 8] = &SkWinCore::_14cd_1c63;
	_4976_38a4[ 9] = &SkWinCore::_14cd_1c8d;
	_4976_38a4[10] = &SkWinCore::_14cd_1cec;
	_4976_38a4[11] = &SkWinCore::_14cd_1d42;
	_4976_38a4[12] = &SkWinCore::_14cd_1e36;
	_4976_38a4[13] = &SkWinCore::_14cd_1e52;
	_4976_38a4[14] = &SkWinCore::_14cd_1e6e;
	_4976_38a4[15] = &SkWinCore::_14cd_1f8b;
	_4976_38a4[16] = &SkWinCore::_14cd_1fa7;
	//}}INIT_INSORTED

	_4976_5d00 = NULL;
	_4976_5ea0 = NULL;
	glbLangageLetterIndex = 0;
	_4976_5c9c = 0;
	_4976_5ca8 = 0;
	_4976_4839 = 0;
	_4976_483b = 0;
	zeroMem(&glbGDatEntries, sizeof(glbGDatEntries));
	_4976_480d = 0;
	_4976_5d34 = 0;
	_4976_5d58 = 0;
	_4976_5cae = 0;
	zeroMem(&glbRectNoTable, sizeof(glbRectNoTable));
	_4976_4ba4	 = 0;
	_4976_4968 = 1; // EMS is always avail.
	_4976_00f6 = 224;
	_4976_00f8 = 136;
	_4976_0126 = 6;
	_4976_013e = 320;
	_4976_0140 = 15;
	_4976_0198 = 0; // temp index always 0 (_4976_4b84)
	zeroMem(&_4976_4bca, sizeof(_4976_4bca));
	_4976_4bc8 = 0;
	_4976_0106 = 16; // cx
	_4976_0108 = 16; // cy
	_04bf_0e7a = 0; //?
	_04bf_17a4 = 0; //?
	_04bf_09e0.x = 0; _04bf_09e0.y = 0; _04bf_09e0.cx = 0xc; _04bf_09e0.cy = 0x10;
	_04bf_09e8.x = 0; _04bf_09e8.y = 0; _04bf_09e8.cx = 0xc; _04bf_09e8.cy = 0x10;
	_4976_19ad = 0; //?
	glbGameTick = 0;
	_4976_5d2a = -1;
	_4976_5d76 = 0; //?
	_4976_5cfc.pv0 = NULL; //?
	_4976_5cf8 = 0;
	_4976_4ba6 = 0; // temp idx be 0
	glbScreenWidth = 320;	// Screen width
	glbScreenHeight = 200;	// Screen height
	_4976_4964 = NULL;
	glbMouseVisibility = 1;	// (_4976_4860)
	_04bf_0e7a = 0;
	glbMouseXPos = glbMouseYPos = 0;
	zeroMem(&_04bf_1852, sizeof(_04bf_1852));
	_04bf_17a8 = 0;
	glbMouseCursorVisible = 0;
	_04bf_1886 = 0; //?
	glbDMode = 0;
	_04bf_0284 = 0;
	_04bf_0280 = 0;
	_04bf_03d4 = 0;
	_01b0_0933 = 0;
	_01b0_14d8 = 0;
	glbZMode = 1;
	_4976_04ee = 0;
	sndSoundInBuffer = 0;		// (_01b0_13d6)
	sndLockSoundBuffer = 0;
	sndSoundBufferSize = 0;
	sndSoundBuffer = NULL;
	_04bf_17e8 = 0;
	_4976_19a5 = 10;
	glbCounterZero_0517 = 0;
	glbSomeCounter_0519 = 0;
	_01b0_13c6 = 0;
	_04bf_18ae = NULL;
	_04bf_179e = NULL;
	_4976_4ea6 = 0;
	_4976_4e00 = 0;
	_4976_4dfe = 0;
	_4976_4e46 = 0;
	glbAbsoluteTickCounter = 0;
	_4976_19a9 = -1;
	_4976_4e64 = 0;
	_4976_4e96 = NULL;
	glbHoldedContainerType = 0;
	_4976_53a4 = 0;
	zeroMem(glbItemSelected, sizeof(glbItemSelected));
	glbMagicalMapFlags = 0;
	_4976_4e96 = NULL;
	zeroMem(&glbMousePosition, sizeof(glbMousePosition));
	glbMousePosition.XPos = -1;
	zeroMem(&_4976_4e9e, sizeof(_4976_4e9e));
	zeroMem(_4976_4e6c, sizeof(_4976_4e6c));
	_4976_4ddc = 0;
	_4976_5efa = NULL;
	zeroMem(&_4976_5dae, sizeof(_4976_5dae));
	_4976_5bf2 = 0;
	_4976_3b5d = 1;
	zeroMem(glbIngameGlobVarFlags, sizeof(glbIngameGlobVarFlags));
	zeroMem(glbIngameGlobVarBytes, sizeof(glbIngameGlobVarBytes));
	zeroMem(glbIngameGlobVarWords, sizeof(glbIngameGlobVarWords));
	_4976_47fc = 1;
	_4976_4762 = -1;
	_4976_4c08 = 0;
	_4976_4c12 = 0;
	glbCurrentMapIndex = -1;
	_4976_4c04 = 0;
	_4976_4c1e = 0;
	_4976_4c12 = 0;
	_4976_4c10 = 0;
	_4976_0118 = 0x11;
	_4976_011a = 0x11;
	glbChampionsCount = 0;
	_4976_0124 = 1;
	_4976_00f4 = 0x7700;
	glbFreeRAMMemPool = 0;
	_4976_0132 = 1;
	zeroMem(_4976_5bf8, sizeof(_4976_5bf8));
	zeroMem(_4976_5bfa, sizeof(_4976_5bfa));
	_4976_0120 = 1;
	_4976_011e = 6;
	_4976_475e = 0xff;
	glbPanelStatsYDelta = 7;
	zeroMem(_4976_4c42, sizeof(_4976_4c42));
	zeroMem(glbMoneyItemsValueTable, sizeof(glbMoneyItemsValueTable));
	zeroMem(glbMoneyItemsIDTable, sizeof(glbMoneyItemsIDTable));
	zeroMem(&_4976_5d12, sizeof(_4976_5d12));
	zeroMem(&_4976_5d7e, sizeof(_4976_5d7e));
	zeroMem(&_4976_5bee, sizeof(_4976_5bee));
	zeroMem(glbMapDoorType, sizeof(glbMapDoorType));
	zeroMem(&_4976_3f6c, sizeof(_4976_3f6c)); _4976_3f6c.w0 = -1;
	_04bf_1938 = 0;
	zeroMem(glbChampionTable, sizeof(glbChampionTable));
	zeroMem(&glbMousePosition, sizeof(glbMousePosition));
	zeroMem(&glbTempPicture, sizeof(glbTempPicture));
	zeroMem(&_4976_3ff0, sizeof(_4976_3ff0)); _4976_3ff0.w0 = -1;
	zeroMem(_04bf_0e80, sizeof(_04bf_0e80));
	_04bf_17a2 = 0;
	_4976_5da4 = NULL;
	zeroMem(_4976_4e02, sizeof(_4976_4e02));
	zeroMem(_4976_4d1a, sizeof(_4976_4d1a));
	glbSquadInterfaceMapGfxSet = -1;
	glbMapGraphicsSet = -1;
	_4976_532c = 0;
	zeroMem(&_4976_495c, sizeof(_4976_495c));
	zeroMem(_4976_4df6, sizeof(_4976_4df6));
	zeroMem(&glbLeaderHandPossession, sizeof(glbLeaderHandPossession));
	zeroMem(glbChampionsPendingDamage, sizeof(glbChampionsPendingDamage));
	_4976_4942.x = 2; _4976_4942.y = 2; _4976_4942.cx = 16; _4976_4942.cy = 16;
	_4976_494a.x = 0; _4976_494a.y = 0; _4976_494a.cx = 16; _4976_494a.cy = 16;
	_4976_0ce0[0].pv6(NULL);
	zeroMem(_4976_4954, sizeof(_4976_4954));
	glbMemWalkPath = NULL;
	zeroMem(_4976_4fee, sizeof(_4976_4fee));
	_4976_00fe.x = 21; _4976_00fe.y = 8; _4976_00fe.cx = 182; _4976_00fe.cy = 110;

	// SPX : Default champion colors are : 7 (green), 11 (yellow), 8 (red), 14 (blue)
	glbChampionColor[0] = COLOR_LIGHT_GREEN;
	glbChampionColor[1] = COLOR_YELLOW;
	glbChampionColor[2] = COLOR_RED;
	glbChampionColor[3] = COLOR_BLUE;

	mice_x = mice_y = mice_btn = 0;
	_01b0_3378 = 0;
	glbItemStatBarPos.pos[0] = 0x270; glbItemStatBarPos.pos[1] = 0x271;
	zeroMem(&_4976_514e, sizeof(_4976_514e));
	zeroMem(_4976_522e, sizeof(_4976_522e));
	zeroMem(glbCurrentContainerItems, sizeof(glbCurrentContainerItems));
	zeroMem(glbChampionHandCoolingDown, sizeof(glbChampionHandCoolingDown));
	zeroMem(glbChampionAlive, sizeof(glbChampionAlive));
	zeroMem(&_4976_53a6, sizeof(_4976_53a6));
	zeroMem(glbChampionsBodyFlags, sizeof(glbChampionsBodyFlags));
	zeroMem(_4976_4fd8, sizeof(_4976_4fd8));
	zeroMem(&glbGlobalSpellEffects, sizeof(glbGlobalSpellEffects));
	zeroMem(&_4976_5c2a, sizeof(_4976_5c2a));
	zeroMem(_089c_02c0, sizeof(_089c_02c0));
	zeroMem(_089c_025a, sizeof(_089c_025a));
	zeroMem(_089c_0090, sizeof(_089c_0090)); _089c_0090[0] = -1;
	_089c_00d0.x = 0; _089c_00d0.y = 0; _089c_00d0.cx = 320; _089c_00d0.y = 200;
	tickThen = -1;
}




//SPX: move this to SkWinMain
#if defined(_USE_MFC80) || defined(_USE_MFC60)
int main(int argc, char **argv)
{
	int r = 1;
	HRESULT hr;
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);

	char buildInfo[100];
    // Combine __DATE__ and __TIME__ into one string
    sprintf(buildInfo, "Build date and time: %s %s", __DATE__, __TIME__);
    printf("%s\n", buildInfo);

	if (AfxWinInit(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOW)) {
		if (SUCCEEDED(hr = CoInitialize(NULL))) {
			int f = skwin.sxfact;
			CRect rc(0, 0, 320 * f, 200 * f);
			AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, true, 0);
			rc.OffsetRect(-rc.left, -rc.top);
			if (skwin.CreateEx(0, AfxRegisterWndClass(0, AfxGetApp()->LoadCursor(IDC_POINTER), (HBRUSH)GetStockObject(BLACK_BRUSH), NULL), "SKWin-9821", WS_OVERLAPPEDWINDOW, rc, NULL, 0, NULL)) {
				skwin.ShowWindow(SW_SHOWDEFAULT);

				SkWinCore a;
				skwin.skw = &a;
				skwin.ProcessArgs(argc, argv);
				r = a.IBMIO_BOOTSTRAP();

				skwin.DestroyWindow();
			}
		}
	}
	return r;
}
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)

#ifdef _USE_SDL
int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0)
		return 1;

	if (!skwin.CreateSurface())
		return 1;
	skwin.OpenAudio();

	SkWinCore a;
	int r = a.IBMIO_BOOTSTRAP();

	skwin.CloseAudio();
	SDL_Quit();
	return r;
}
#endif // _USE_SDL

#if defined(__DJGPP__)
int main(int argc, char **argv)
{
	int r = 1;
	SkWinCore a;
	skwin.skw = &a;
	skwin.StartMessage();
	skwin.ProcessArgs(argc, argv);
	skwin.InitVideo();
	r = a.IBMIO_BOOTSTRAP();
	skwin.ResetVideo();
	skwin.ExitMessage();
	return r;
}
#endif // __DJGPP__