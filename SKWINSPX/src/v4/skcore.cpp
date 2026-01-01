//------------------------------------------------------------------------------
// SK-CORE
//	Main base core code for running DM2
//------------------------------------------------------------------------------

#include <StdAfx.h>	// Required for MVC6 compilation

#include <skver.h>
//#include <conio.h> // getch

#include <skparam.h>
#include <skdebug.h>
#include <skglobal.h>
#include <skcore.h>
#include <skmidi.h>
#include <sklua.h>
#include <skcnsole.h>

#include <stdlib.h>
#include <string.h>

//--- Common part with A.cpp
using namespace DMEncyclopaedia;
using namespace DM2Internal;
using namespace kkBitBlt;

//#if defined(_USE_MFC80) || defined(_USE_MFC60)
//#include <SKMFC.h>
//#include <resource.h>
//#endif // defined(_USE_MFC80) || defined(_USE_MFC60)


//#ifdef _USE_SDL
//#include <SKSDL.h>
//#endif // _USE_SDL
//#if defined(__DJGPP__) || defined(__MINGW__)
//#include <SKDOS.h>
//#include <unistd.h>
//#endif // __DJGPP__


#ifdef __USE_ALLEGRO__
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#endif // __USE_ALLEGRO__

#include <kaitable.h>	// for dAITableGenuine (hard coded AI table)
//--- Common part with A.cpp
/*
#if DM2_EXTENDED_MODE == 1
	#define CREATURE_AI_TAB_SIZE	64
#else
	#define CREATURE_AI_TAB_SIZE	42
#endif // DM2_EXTENDED_MODE
*/

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


#if defined(__MINGW__) || defined(__LINUX__)
#include <strings.h>
#include <time.h>
#include <stdlib.h>
#undef inportb
#undef outportb
#define outportb(PORT,VAL)
#define inportb(PORT,VAL)
#ifndef _stricmp
#define _stricmp strcasecmp
#endif // _stricmp

#ifndef _exit
#define _exit exit
#endif // _exit

int getdrive() { return 3; }

#ifndef _getdrive
#define _getdrive getdrive
#endif // _getdrive

#define min(A,B) ((A < B) ? A : B)
#define max(A,B) ((A < B) ? B : A)

#endif // __MINGW__



// SPX: New procedures here

const X8* SkWinCore::GET_DATA_FOLDER_ZNAME()
{
	const X8* sFolderData = NULL;

	if (skWinApp->sCustomDataFolder != NULL && skWinApp->sCustomDataFolder[0])
		return (const X8*) skWinApp->sCustomDataFolder;
#ifdef __LINUX__
	sFolderData = (const X8*) ".Z008DATA/";
#else
	sFolderData = (const X8*) ".Z008DATA\\";
#endif
	return sFolderData;
}


const X8* SkWinCore::GET_DATA_FOLDER_NAME()
{
	const X8* sFolderData = NULL;

	if (skWinApp->sCustomDataFolder != NULL && skWinApp->sCustomDataFolder[0])
		return (const X8*) skWinApp->sCustomDataFolder;
#ifdef __LINUX__
	sFolderData = (const X8*) "DATA/";
#else
	sFolderData = (const X8*) "DATA\\";
#endif
	return sFolderData;
}


X16 SkWinCore::EXTENDED_LOAD_AI_DEFINITION(void)
{
	int rc = 0;
	U8 index = 0;
	// SPX: If not extended, load the default table from static data / OR if dungeon selected is skullkeep (security because AI is not in GDAT currently)
	// 2023-07-26 : removed the test : in all case, first setup the AI table with default DM2 values, then overwrite with what's from GDAT if any
	{
		//&_4976_03a2[res * 0x0024]
		memcpy(dAITable, dAITableGenuine, 62 * 36);

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
	if (0)
	//if (SkCodeParam::bUseDM2ExtendedMode)
	{
		U16 value = 0;
		U8 category = GDAT_CATEGORY_CREATURE_AI;
		U8 text[128] = {0};

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
				dAITable[index].wc30 = byte1 + byte2*256;

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
	static X8 text[128] = {0};
	if (spelltype == SPELL_TYPE_POTION) return "POTION";
	if (spelltype == SPELL_TYPE_MISSILE) return "MISSILE";
	if (spelltype == SPELL_TYPE_GENERAL) return "ENCHANTMENT";
	if (spelltype == SPELL_TYPE_SUMMON) return "SUMMON";
	if (text[0] != 0)
		return (LPCSTR)text;
	return "Unknown";
}

// SPX: Name of object database 
CString SkWinCore::getDatabaseCategoryTypeName(U8 dbtype)
{
	static X8 text[128] = {0};
	if (dbtype == DB_CATEGORY_DOOR) return "DB-DOOR";
	if (dbtype == DB_CATEGORY_TELEPORTER) return "DB-TELEPORTER";
	if (dbtype == DB_CATEGORY_TEXT || dbtype == DB_CATEGORY_SIMPLE_ACTUATOR) return "DB-TEXT-SIMPLE-ACTUATOR";	// both are dbtype == 2 ... this is just for knowledge readability
	if (dbtype == DB_CATEGORY_ACTUATOR) return "DB-ACTUATOR-SENSOR";
	if (dbtype == DB_CATEGORY_CREATURE) return "DB-CREATURE-OBJECT";
	if (dbtype == DB_CATEGORY_WEAPON) return "DB-WEAPON";
	if (dbtype == DB_CATEGORY_CLOTHING) return "DB-CLOTHING";
	if (dbtype == DB_CATEGORY_SCROLL) return "DB-SCROLL";
	if (dbtype == DB_CATEGORY_POTION) return "DB-POTION";
	if (dbtype == DB_CATEGORY_CONTAINER) return "DB-CONTAINER-MAP";
	if (dbtype == DB_CATEGORY_MISC_ITEM) return "DB-MISC-ITEM";
	if (dbtype == DB_CATEGORY_MISSILE) return "DB-MISSILE";
	if (dbtype == DB_CATEGORY_CLOUD) return "DB-CLOUD";
	if (text[0] != 0)
		return (LPCSTR)text;
	return "Unknown";

}

// SPX: Name of skill
CString SkWinCore::getSkillName(U8 skill)
{
	static X8 text[128] = {0};
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
	static X8 text[128] = {0};
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

// SPX: Name of creature standard sound
CString SkWinCore::getCreatureSoundName(U8 cs)
{
	static X8 text[128] = {0};
	if (cs == 0xFF)			return "NONE";
	if (cs == 0)			return "WALK/MOVE";
	if (cs == 1)			return "MOVE/TURN";
	if (cs == 2)			return "SHOOT";
	if (cs == 3)			return "REFLECTOR";
	if (cs == 4)			return "JUMP";
	if (cs == 5)			return "JUMP END";
	if (cs == 6)			return "YELL 1";
	if (cs == 7)			return "ATTACK";
	if (cs == 8)			return "STEAL / PUT / PICK";
	if (cs == 9)			return "9";
	if (cs == 0x0A)			return "A";
	if (cs == 0x0B)			return "B";
	if (cs == 0x0C)			return "C";
	if (cs == 0x0D)			return "D";
	if (cs == 0x0E)			return "YELL 2";
	if (cs == 0x0F)			return "GROWL";
	if (cs == 0x10)			return "SPAWN";
	if (cs == 0x11)			return "DEATH";
	if (cs == 0x12)			return "ATTACK 2";
	if (cs == 0x8D)			return "GET HIT";
	if (text[0] != 0)
		return (LPCSTR)text;
	return "UNKNOWN";
}

// SPX: Name of creature command
CString SkWinCore::getCreatureCommandName(U8 ccm)
{
	static X8 text[128] = {0};
	sprintf((char*)text, "CCM-%02X", ccm);	// default
	if (ccm == ccm00_Neutral)		return "NONE";
	if (ccm == ccm01)			return "WALK/MOVE x01";
	if (ccm == ccm02)			return "WALK/MOVE x02";
	if (ccm == ccm03)			return "TURN LEFT + MOVE ? x03";	// right also ????
	if (ccm == ccm04)			return "TURN RIGHT + MOVE x04";
	if (ccm == ccm05_Jump)			return "JUMP";
	if (ccm == ccm06)			return "TURN LEFT ? x06";
	if (ccm == ccm07)			return "TURN RIGHT ? x07";
	if (ccm == ccm08_MeleeAttack)	return "MELEE ATTACK";
	if (ccm == ccm09)			return "WALK x09";
	if (ccm == ccm0A_Steal)		return "STEAL";
	if (ccm == ccm0B)			return "ATTACK x0??";
	if (ccm == ccm0C)			return "x0C";
	if (ccm == ccm0D)			return "x0D";
	if (ccm == ccm0E_ShootItem1)	return "THROW ITEM x0E";
	if (ccm == ccm0F_ShootItem2)	return "THROW ITEM x0F";
	if (ccm == ccm10)			return "ATTACK x10?";
	if (ccm == ccm11_Spawn)		return "SPAWN";
	if (ccm == ccm12)			return "UNUSED x12";
	if (ccm == ccm13_Destroy)		return "DEATH";
	if (ccm == ccm14)			return "x14";
	if (ccm == ccm15)			return "x15";
	if (ccm == ccm16)			return "x16";

	if (ccm == ccm17_PlaceMerchandise)			return "PUT MERCHANDISE x17";
	if (ccm == ccm18_TakeMerchandise)			return "TAKE MERCHANDISE x18";
	if (ccm == ccm19)			return "GRAB ARROW ? x19";
	if (ccm == ccm1A_PickItem)			return "PICK ARROW ? x1A";
	if (ccm == ccm1B)			return "STATE 0 / REFUSEx1B";
	if (ccm == ccm1C)			return "STATE 1 / ACCEPT / YELL x1C";
	if (ccm == ccm1D)			return "STATE 2 / BLINK x1D";
	if (ccm == ccm1E)			return "STATE 3 / SORRY x1E";
	if (ccm == ccm1F)			return "MERCHANT SORRY x1F";

	if (ccm == ccm20)			return "MERCHANT THINK ? x20";
	if (ccm == ccm21)			return "x21";
	if (ccm == ccm22)			return "x22";
	if (ccm == ccm23)			return "GO TO SLEEP x23";
	if (ccm == ccm24)			return "SLEEP x24";
	if (ccm == ccm25)			return "AWAKE x25";
	if (ccm == ccm26_PushBack)		return "PUSH BACK";
	if (ccm == ccm27_CastSpell1)	return "CAST SPELL 3";
	if (ccm == ccm28_CastSpell2)	return "CAST SPELL 2";		

	if (ccm == ccm29)			return "TAKE BONE x29";
	if (ccm == ccm2A)			return "PUT BONE x2A";
	if (ccm == ccm2B_PickItem)			return "PICK ITEM THEN MOVE ? x2B";
	if (ccm == ccm2C_PickItem)			return "PICK ITEM THEN MOVE ? x2C";
	if (ccm == ccm2D)			return "TAKE ? x2D";
	if (ccm == ccm2E)			return "TAKE ? x2E";
	if (ccm == ccm2F_ActivateWallSwitch)			return "SWITCH BUTTON ? x2F";
	if (ccm == ccm30_ActivateWallSwitch)			return "SWITCH BUTTON ? x30";
	if (ccm == ccm31_ActivateWallSwitch)			return "SWITCH BUTTON ? x31";	

	if (ccm == ccm32)			return "13E4:0AEC 1 x32";
	if (ccm == ccm33)			return "13E4:0AEC 2 x33";
	if (ccm == ccm34)			return "13E4:0AEC 3 x34";
	if (ccm == ccm35)			return "MINION UP x35";
	if (ccm == ccm36)			return "MINION COMING FROM TOP x36";
	if (ccm == ccm38)			return "MINION UP x37";
	if (ccm == ccm37)			return "MINION COMING FROM TOP x38";
	if (ccm == ccm39)			return "MINION UP x39";
	if (ccm == ccm3A)			return "MINION COMING FROM TOP x3A";	

	if (ccm == ccm3B_Transform1)	return "TRANSFORM 1 x3B";
	if (ccm == ccm3C_Transform2)	return "TRANSFORM 2 x3C";
	if (ccm == ccm3D_Explode)		return "SUMMON 1 / EXPLODE 1 x3D";
	if (ccm == ccm3E)			return "EXPLODE 2 x3E";
	if (ccm == ccm3F)			return "EXPLODE 3 x3F";
	if (ccm == ccm40)			return "EXPLODE 4 x40";
	//if (ccm == ccm41)			return "x41";
	if (ccm == ccm42)			return "x42";
//	if (ccm == ccm43)			return "x43";	
//	if (ccm == ccm44)			return "x44";
//	if (ccm == ccm45)			return "x45";
//	if (ccm == ccm46)			return "x46";
//	if (ccm == ccm47)			return "x47";
//	if (ccm == ccm48)			return "x48";
//	if (ccm == ccm49)			return "x49";

//		ccm42 = 0x42,	// Hello 1 (rocky) ?
//		ccm52 = 0x52,	// Hello 2 (rocky) ?
	if (ccm == ccm52)				return "ROCKY HELLO 2 ? x52";
	if (ccm == ccm55_CastSpell1or2)	return "SPELL 1 OR 2 ? x55";
	if (ccm == ccmFF_Idle)				return "IDLE";	

	if (text[0] != 0)
		return (LPCSTR)text;
	return "UNKNOWN";
}

CString SkWinCore::getSystemErrorMessage(U8 iSysErrCode)
{
	static X8 sSysErrMessage[128] = {0};

	if (iSysErrCode == SYSTEM_ERROR__INVALID_ENT1)
		return "INVALID HEADER IN GRAPHICS.DAT (ENT1)";
	if (iSysErrCode == SYSTEM_ERROR__MISSING_GRAPHICS_2)
		return "MISSING GRAPHICS2.DAT FILE";
	if (iSysErrCode == SYSTEM_ERROR__CORRUPTED_GRAPHICS_2)
		return "CORRUPTED GRAPHICS2.DAT FILE";

	if (iSysErrCode == SYSTEM_ERROR__OUT_OF_EMS_MEM)
		return "OUT OF EMS MEMORY";
	if (iSysErrCode == SYSTEM_ERROR__OUT_OF_MAIN_MEM)
		return "OUT OF MAIN MEMORY";

	if (iSysErrCode == SYSTEM_ERROR__MISSING_GRAPHICS)
		return "MISSING GRAPHICS.DAT FILE";
	if (iSysErrCode == SYSTEM_ERROR__INVALID_GRAPHICS_STRUCTURE)
		return "INVALID GRAPHICS STRUCTURE";
	if (iSysErrCode == SYSTEM_ERROR__FREE_CPXHEAP_SIZE)
		return "ISSUE IN FREE CPXHEAP SIZE";
	if (iSysErrCode == SYSTEM_ERROR__LOAD_DYN4)
		return "ISSUE IN LOAD_DYN4 FUNCTION";
	if (iSysErrCode == SYSTEM_ERROR__TIMER_MAX_REACHED)
		return "TIMER MAX REACHED";
	if (iSysErrCode == SYSTEM_ERROR__CORRUPTED_GRAPHICS)
		return "CORRUPTED GRAPHICS.DAT FILE";


	if (iSysErrCode == SYSTEM_ERROR__CANT_ADD_MEM)
		return "CAN'T ADD MORE MEMORY";
	if (iSysErrCode == SYSTEM_ERROR__MISSING_DUNGEON)
		return "MISSING DUNGEON.DAT FILE";
	if (iSysErrCode == SYSTEM_ERROR__TIMER_BEYOND_MAX)
		return "MAX GAME TIMER/EVEN REACHED";
	if (iSysErrCode == SYSTEM_ERROR__CREATURE_ASSOC_FULL)
		return "MAX RECORD FOR CREATURE ASSOC";
	if (iSysErrCode == SYSTEM_ERROR__CANT_APPEND_RECORD)
		return "ERROR APPENDING RECORD";
	if (iSysErrCode == SYSTEM_ERROR__NO_PLAYER_DATA)
		return "MISSING PLAYER DATA IN GRAPHICS.DAT";

	if (sSysErrMessage[0] != 0)
		return (LPCSTR)sSysErrMessage;
	return "???";
}

// SPX: Name of UI Event
CString SkWinCore::getUIEventName(U8 event)
{
	static X8 text[128] = {0};

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
	static X8 text[128] = {0};

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
	static X8 text[128] = {0};
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
#if !defined(__DJGPP__) && !defined(__MINGW__) && !defined(__LINUX__)
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
X16 SkWinCore::IS_NEGATIVE(i16 iValue)
{
	ENTER(0);
	return (iValue < 0) ? 1 : 0;
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


//^48AE:0281
U16 SkWinCore::QUERY_CREATURES_ITEM_MASK(U8 cls2, U8 cls4, U8 itemflags[64], U16 isCreature)
{
	//^48AE:0281
	ENTER(142);
	//^48AE:0287
	ZERO_MEMORY(itemflags, 64);
	//^48AE:0299
	U8 bp008e[128];
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
// There is also ZeroMem !!
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
			U8 *bp08 = &glbCurrentTileMap[di][ypos_newParent];
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

				if (!SkCodeParam::bUsePowerDebug)	// Normally, a tile with x10 must have a record link
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
			//U16 *bp04 = &_4976_4c52[di +1];
			OID_T *bp04 = &_4976_4c52[di +1];	// Bit32u *bp04
			//^0CEE:0BB5
			U16 si = _4976_4cb4 -(dunMapColumnsSumArray[glbCurrentMapIndex] +di) -1;
			ATLASSERT(si < (32*256)); // SPX: suppose 256 of 32 columns each, making a value not above 8192. If not, it might be a negative value hidden by the unsigned U16 si; then an issue with _4976_4cb4 value.
			//^0CEE:0BCE
			//^0CEE:0BDA
			for (; si-- != 0; ) {
				//^0CEE:0BD0
				(*bp04)++;
				bp04++;
			}
			//^0CEE:0BE1
			U16 bp10 = 0;
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
	if (SkCodeParam::bUsePowerDebug) {	// SPX: do not try to get next object if current recordlink is already a null object
		if (rl == OBJECT_END_MARKER || rl == OBJECT_NULL)
			return OBJECT_END_MARKER;
	}
	ObjectID rlNext = GET_ADDRESS_OF_RECORD(rl)->w0;
	if (rl == rlNext) {
		if (!SkCodeParam::bUsePowerDebug)// || (bUsePowerDebug && rl != OBJECT_END_MARKER && rl != OBJECT_NULL ) )
			ATLASSERT(rl != rlNext);
		return OBJECT_END_MARKER;
	}
	return rlNext;
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
#ifndef __MINGW__
	SkD((DLV_BUGHERE, "DEBUG: %s is bad record %02d (RL = %04X DB=%d IDX=%04d)\n"
		, static_cast<LPCSTR>(getRecordNameOf(rl)), countBadRecords, rl, rl.RealDBType(), rl.DBIndex() ));
#endif // __MINGW__
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
//void SkWinCore::_2066_1ea3(U16 xx, U16 yy, U16 zz)
void SkWinCore::SET_TILE_ATTRIBUTE_02(U16 xx, U16 yy, U16 map)
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
	U8 * sLocalGraphicsDatFileString = NULL;
	U8 * sLocalGraphicsDatFileString2 = NULL;

	sLocalGraphicsDatFileString = (U8*) ptrGraphics;
	sLocalGraphicsDatFileString2 = (U8*) ptrGraphics2;

	//SkD((DLV_DBG_INIT, "GRAPHICS_DATA_OPEN\n"));

	// default : ".Z020GRAPHICS.Z080.Z081.Z082.DAT"
	if (SkCodeParam::bUseFixedMode && (ptrGraphics != NULL && ptrGraphics[0] == 0))
	{
		//strcpy(ptrGraphics, ".Z020GRAPHICS.Z080.Z081.Z082.DAT");
		sLocalGraphicsDatFileString = (U8*) strGraphics;
	}


	if (skWinApp->sCustomGraphicsDatFilename != NULL)
	{
		sLocalGraphicsDatFileString = (X8*) skWinApp->sCustomGraphicsDatFilename;
	}

	if ((glbGDatOpenCloseFlag++) == 0) {
		glbFileHandleGraphics1 = FILE_OPEN(FORMAT_SKSTR(sLocalGraphicsDatFileString, NULL));
		if (glbFileHandleGraphics1 < 0) {
			printf("GDAT1: Searching for file %s\n", FORMAT_SKSTR(sLocalGraphicsDatFileString, NULL));
			RAISE_SYSERR(SYSTEM_ERROR__MISSING_GRAPHICS);	// SYS ERROR 41
		}
		if (_4976_5c9c != 0 && _4976_5ca8 == 0) {
			glbFileHandleGraphics2 = FILE_OPEN(FORMAT_SKSTR(sLocalGraphicsDatFileString2, NULL));
			if (glbFileHandleGraphics2 < 0) {
				printf("GDAT2: Searching for file %s\n", FORMAT_SKSTR(sLocalGraphicsDatFileString, NULL));
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
	if ((glbGDatOpenCloseFlag++) == 0) {
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
SRECT *SkWinCore::ALLOC_TEMP_ORIGIN_RECT(U16 iWidth, U16 iHeight)
{
	return ALLOC_TEMP_RECT(0, 0, iWidth, iHeight);
}





//^44C8:1D26
void SkWinCore::FILL_ENTIRE_PICT(U8 *buff, U16 fill)
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


//^3E74:57B5
U8 *SkWinCore::ALLOC_NEW_PICT(U16 index, U16 width, U16 height, U16 bpp)
{
	//^3E74:57B5
	//^3E74:57BA
	U16 si = width;
	//^3E74:57BD
	U8 *bp04 = ALLOC_CPXHEAP_MEM(
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
sk3f6c *SkWinCore::_0b36_0c52(sk3f6c *ref, U16 rectno, U16 ww)
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
	prcOut->x = prcSrc->x - refOrg->rc2.x;
	prcOut->y = prcSrc->y - refOrg->rc2.y;
	prcOut->cx = prcSrc->cx;
	prcOut->cy = prcSrc->cy;
	return prcOut;
}

//^0B36:0FFE
void SkWinCore::FILL_RECT_SUMMARY(sk3f6c *ref, SRECT *rc, U8 fill)
{
	//^0B36:0FFE
	ENTER(8);
	//^0B36:1002
	if (rc != NULL) {
		//^0B36:100A
		SRECT bp08;
		FIRE_FILL_RECT_ANY(
			reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(ref->w0)),
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
void SkWinCore::_29ee_00a3(U16 xx)
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
U16 SkWinCore::IS_MISSILE_VALID_TO_LAUNCHER(U16 player, i16 hand, ObjectID rlWhatYouLaunch)
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
U8 *SkWinCore::QUERY_GDAT_ENTRY_DATA_PTR(U8 cls1, U8 cls2, U8 cls3, U8 cls4)
{
LOGX(("%40s: C%02d=I%02X=E%02X=T%03d", "QUERY_GDAT_ENTRY_DATA_PTR for ", cls1, cls2, cls4, cls3 ));
	//^3E74:506B
	ENTER(6);
	//^3E74:5070
	U16 si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, cls3, cls4);
	//^3E74:5089
	if (cls3 == dtImage && IS_CLS1_CRITICAL_FOR_LOAD(cls1) == 0) {
		//^3E74:509C
		if (si == 0xffff || glbShelfMemoryTable[si].Absent()) {
			//^3E74:50BE
			si = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_MISCELLANEOUS, GDAT_ITEM_DEFAULT_INDEX, dtImage, 0xFE);	// yukman face
		}
	}
#if UseAltic
	if (si == 0xFFFFU) {
		return NULL;
	}
#endif
	//^3E74:50D1
	U8 *bp04;
	U16 bp06;
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




//^2405:014A
// SPX: _2405_014a renamed GET_ITEM_ICON_ANIM_FRAME
U8 SkWinCore::GET_ITEM_ICON_ANIM_FRAME(ObjectID recordLink, i16 xx, i16 yy)
{
	//^2405:014A
	ENTER(8);
	//^2405:0150
	U8 bp01 = 0x18;	// base image is 0x18
	//^2405:0154
	if (yy != 0) {
		//^2405:015D
		U16 si = QUERY_GDAT_DBSPEC_WORD_VALUE(recordLink, 6); // get icon anim value from item
		//^2405:016B
		U16 di = si & 15;	// get number of different frames
		//^2405:0170
		if (di != 0) {
			//^2405:0177
			U16 bp04 = 0;
			//^2405:017C
			if ((si & 0x8000) == 0 || (bp04 = IS_ITEM_FIT_FOR_EQUIP(recordLink, xx, 1)) != 0) {
				//^2405:019C
				if ((si & 0x4000) != 0) {
					//^2405:01A2
					if (cd.pi.glbChampionIndex == 0 || _4976_3de2[(cd.pi.glbChampionIndex << 1) + (glbSelectedHandAction)] != recordLink)
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
					U8 nFramesPerAnim = (si & 0x00E0) >> 5; // SPX added
					//^2405:01E9
					switch ((si & 0x1f00) >> 8) {	// get anim type
						case 5:	// not sure ?
							//^2405:0201
							bp08 += recordLink.DBIndex();

							goto _020e;
						case 0:	// always animated (i.e. potion)
							//^2405:020E
_020e:
							bp01 = bp01 + U8(bp08 % di);
							//^2405:0224
							break;
						case 1: // random frame among remaining possible frames
							//^2405:0227
							bp01 = bp01 + U8(RAND16(di));
							//^2405:0224
							break;
						case 2:	// compass mode : choose from depending on direction
							//^2405:0230
							bp01 = bp01 + cd.pi.glbPlayerDir;
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
							bp01 = bp01 + U8(bp08 % nFramesPerAnim); // SPX added
							goto _027d;
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







//^1031:023B
U8 *SkWinCore::_1031_023b(sk1891 *xx)
{
	//^1031:023B
	ENTER(0);
	//^1031:023E
    return &_4976_169c[xx->w2];
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
		_443c_087c_LOCK_MOUSE();
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
				_443c_00a9_MOUSE(
					ref,
					bp04->x,
					bp04->x + bp04->cx -1,
					bp04->y,
					bp04->y + bp04->cy -1
					);
			}
			_443c_0889_UNLOCK_MOUSE();
			_443c_040e_MOUSE();
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
		_443c_087c_LOCK_MOUSE();
		bp08->pv0 = bp04->pv6()->pv0;
		_443c_0889_UNLOCK_MOUSE();
		_443c_040e_MOUSE();
	}
	//^443C:0858
	return;
}

//^1031:0541
void SkWinCore::_1031_0541(U16 xx) //#DS=4976
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
		U16 si = bp04->b6;
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
U8 *SkWinCore::QUERY_CMDSTR_NAME(U8 cls1, U8 cls2, U8 cls4)
{
	// returns cmdStr name such as "CHOP", "THRUST", "MELEE", and so on.

	//^2759:0009
	ENTER(134);
	//^2759:000E
	U8 bp0086[128];
	U8 *bp04 = QUERY_GDAT_TEXT(cls1, cls2, cls4, bp0086);
	//^2759:0032
	if (*bp04 == 0) {
		//^2759:0038
		return NULL;
	}
	//^2759:003E
	U8 bp05;
	U16 si;
	for (si = 0; (bp05 = *(bp04++)) != ':' && bp05 != 0; si++) {
		//^2759:0042
		glbStrBufferActionName[si] = bp05;
	}
	//^2759:0060
    glbStrBufferActionName[si] = 0;
	//^2759:0065
	return glbStrBufferActionName;
}


//^19F0:124B
U16 SkWinCore::_19f0_124b(i16 *xx, i16 *yy, U16 ww, i16 ss, U16 tt)
{
	//^19F0:124B
	ENTER(8);
	//^19F0:1251
	U16 bp04 = 0;
	i16 di = -1;
	//^19F0:1259
	CHANGE_CURRENT_MAP_TO(ww);
	//^19F0:1262
	U16 si = GET_TILE_VALUE(*xx, *yy);
	//^19F0:1279
	U16 bp02 = si >> 5;
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
void SkWinCore::_29ee_18eb(U16 xx, U16 yy, U16 zz)
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
U16 SkWinCore::QUERY_ORNATE_ANIM_FRAME(U8 cls1, U8 cls2, Bit32u tick, Bit32u delta)
{
	//0CEE:1962
	ENTER(136);
	//^0CEE:1968
	i16 si = 1;
	//^0CEE:196B
	U16 bp06 = 0;
	U16 bp08 = 0;
	//^0CEE:1973
	U16 di = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, dtWordValue, 0x0d);
	//^0CEE:1989
	U8 *bp04;
	U8 bp0088[128];
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




//^48AE:011A
U16 SkWinCore::_48ae_011a(ObjectID recordLink)
{
	//^48AE:011A
	ENTER(2);
	//^48AE:011F
	U8 bp01 = QUERY_CLS1_FROM_RECORD(recordLink);
	//^48AE:012B
	U8 bp02 = QUERY_CLS2_FROM_RECORD(recordLink);
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


//^29EE:1946
void SkWinCore::_29ee_1946(ObjectID recordLink, i16 xx, i16 yy, i16 zz, i16 dir, i16 ss, i16 tt, U16 flags)
{
	//^29EE:1946
    ENTER(56);
	//^29EE:194C
	U16 si = flags;
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
	U16 bp18 = glbCurrentMapIndex;
	//^29EE:19AF
	_29ee_00a3(0);
	//^29EE:19B6
	FILL_RECT_SUMMARY(&_4976_3f6c, QUERY_EXPANDED_RECT(99, &_4976_53a6), glbPaletteT16[COLOR_BLACK]);
	//^29EE:19DC
	INFLATE_RECT(&_4976_53a6, - glbMagicMapInterlineX, - glbMagicMapInterlineY);
	//^29EE:19F4
	U16 bp2e = _4976_53a6.x + ss;
	U16 bp30 = _4976_53a6.y + tt;
	U16 bp2a = si & 0x0010;
	U16 bp1a = si & 0x0020;
	U16 bp28 = si & 0x0040;
	//^29EE:1A1E
	U16 bp22;
	U16 bp24;
	U16 bp26;
	U16 bp1c;
	U16 bp1e;
	U16 bp20;
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
			if (GET_TELEPORTER_DETAIL(&bp3a, (U8)bp1c, (U8)bp1e) != 0) {
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
		U16 bp0a = bp2e;
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
			U8 bp2b;
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


//^0B36:0CBE
void SkWinCore::_0b36_0cbe(sk3f6c *ref, U16 yy)
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
void SkWinCore::UPDATE_RIGHT_PANEL(U16 xx)
{
	//^2759:0644
	//^2759:064A
	U16 bp0a = 0;
	U16 bp0c = 0;
	U16 bp0e = 0;
	U16 bp12 = 0;
	//^2759:065E
	if (cd.pi.glbNextChampionNumber == 0) {
		//^2759:0668
		if (cd.pi.glbChampionsCount == 0) {
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
			for (U16 bp06=0; bp06 < cd.pi.glbChampionsCount; bp06++) {
				//^2759:06A6
				// SPX: ??BUG?? The b42 is table of 2 elements only; shouldn't it be si < 2 ??
				for (U16 si=0; si <= 2; si++) {
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
				ATLASSERT(cd.pi.glbChampionIndex < cd.pi.glbChampionsCount + 1);
				Champion *champion = &glbChampionTable[cd.pi.glbChampionIndex];	//*bp04
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
		if (cd.pi.glbChampionIndex > 0) {
			//^2759:0794
			i16 bp06 = cd.pi.glbChampionIndex -1;
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
		U8 bp1c[4];
		if (cd.pi.glbChampionIndex >= 0) {
			//^2759:07E2
			U16 bp06;
			for (bp06=0; bp06 < cd.pi.glbChampionsCount; bp06++) {
				//^2759:07EA
				Champion *bp04 = &glbChampionSquad[bp06];
				//^2759:07FB
				bp1c[bp06] = 0;
				//^2759:0807
				U8 bp07 = (bp04->playerDir() +4 - cd.pi.glbPlayerDir) & 3;
				//^2759:0819
				i8 bp08 = (bp04->playerPos() +4 - cd.pi.glbPlayerDir) & 3;
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
		if (cd.pi.glbChampionIndex == 0) {
			//^2759:08F6
			glbRightPanelType = 0;
			//^2759:08FC
			if (bp0a == 0 && bp12 != 0) {
				//^2759:0908
				glbPreviousRightPanelType = 0xffff;
			}
			//^2759:090E
			for (U16 bp06=0; bp06 < cd.pi.glbChampionsCount; bp06++) {
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
				U16 bp16 = ((ABS16(glbSomeChampionIndex) -1 == bp06) ? 1 : 0);
				//^2759:09B2
				//SPX: 0 to 1 = hands
				for (U16 si=0; si <= 1; si++) {
					//^2759:09B7
					U16 bp14 = 0;
					U16 bp10 = 0;
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
					U8 bp17 = (champion->handCooldown[si] != 0) ? 1 : 0;
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
				U16 bp14 = 0;
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
				U8 bp17 = (champion->herob44 != 0) ? 1 : 0;
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
		if (cd.pi.glbChampionIndex > 0 && (glbSomeChampionPanelFlag != 0 || bp0a != 0 || bp0c != 0 || (glbRightPanelType == RIGHT_PANEL_MAGIC_MAP && xx != 0))) {
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
				U16 bp06 = cd.pi.glbChampionIndex -1;
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
					cd.pi.glbChampionIndex = 0;
					glbRightPanelType = RIGHT_PANEL_SQUAD_HANDS;
				}
			}

			//^2759:0D5C
			glbSomeChampionPanelFlag = 0;
		}
		else {
			//^2759:0D64
			if (cd.pi.glbChampionIndex > 0 && glbRightPanelType == RIGHT_PANEL_CONTAINER && xx != 0) {
				//^2759:0D78
				_29ee_00a3(0);
				//^2759:0D80
				DRAW_CONTAINER_PANEL(glbChampionTable[cd.pi.glbChampionIndex].Possess(glbSelectedHandAction), 0);
			}
		}
		//^2759:0D9F
		if (cd.pi.glbChampionIndex > 0 && _4976_3df9[RCJ(7,glbRightPanelType)] != 0 && (glbChampionTable[cd.pi.glbChampionIndex].enchantmentPower != 0 || _4976_3f68 != 0)) {
			//^2759:0DC9
			DRAW_PLAYER_ATTACK_DIR();
			//^2759:0DCE
			if (glbChampionTable[cd.pi.glbChampionIndex].enchantmentPower != 0) {
				//^2759:0DDF
				_4976_3f68 |=   1 << (i8(cd.pi.glbChampionIndex) -1);
			}
			else {
				//^2759:0DF1
				_4976_3f68 &= ~(1 << (i8(cd.pi.glbChampionIndex) -1));
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
// _1031_04f5 renamed CLEAR_TRY_PUSH_PULL_OBJECT
void SkWinCore::CLEAR_TRY_PUSH_PULL_OBJECT()
{
	ENTER(0);
	if (glbTryPushPullObject != 0) {
		glbTryPushPullObject = 0;
		DRAW_ARROW_PANEL();
	}
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
void SkWinCore::_1031_0675(U16 xx)
{
	//^1031:0675
	ENTER(0);
	//^1031:0678
	_4976_4ea8 = _4976_19ad;
	//^1031:067E
	_4976_4e64 = 0;
	_4976_4e48 = 0;
	glbPtrTransmittedUIEvent = 0;
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
// _1031_003e renamed _1031_003e
U16 SkWinCore::_1031_003e_PFN12_03(sk1891 *ref)
{
	ENTER(0);
	if (ref->b1 == glbChampionInventory)
		return 1;
	return (ref->b1 <= 4 || (glbChampionInventory != 0 && ref->b1 -4 == glbChampionInventory)) ? 0 : 1;
}



//^1031:00C5
// _1031_00c5 renamed _1031_00c5_PFN12_06
U16 SkWinCore::_1031_00c5_PFN12_06(sk1891 *ref)
{
	ENTER(0);
	return ((ref->b1 == 0 && _4976_5dbc == 0) || (ref->b1 != 0 && _4976_5dbc != 0)) ? 1 : 0;
}

//^1031:00F3
// _1031_00f3 renamed _1031_00f3_PFN12_07
U16 SkWinCore::_1031_00f3_PFN12_07(sk1891 *ref)
{
	ENTER(0);
	if (cd.pi.glbChampionIndex == 0) {
		if (ref->b1 > 3)
			return 1;
		if (GET_PLAYER_AT_POSITION((ref->b1 + cd.pi.glbPlayerDir) & 3) >= 0)
			return 1;
	}
	return 0;
}


//^1031:01BA
// _1031_01ba renamed _1031_01ba_PFN12_11
U16 SkWinCore::_1031_01ba_PFN12_11(sk1891 *ref)
{
	ENTER(0);
	return (ref->b1 == glbRightPanelType) ? 1 : 0;
}

//^1031:027E
// _1031_027e renamed _1031_027e
void SkWinCore::_1031_027e(sk1891 *ref)
{
	//^1031:027E
	ENTER(8);
	//^1031:0282
	U8 *bp08 = _1031_023b(ref);

	do {
		//^1031:0294
		sk1891 *bp04 = &_4976_1574[*bp08 & 0xff7f];
		//^1031:02AB
		if ((this->*_4976_0cba_PFN_12[ref->b0_0_6()])(bp04) != 0) {
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
U16 SkWinCore::PT_IN_RECT(SRECT *rc, i16 xx, i16 yy)
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
SRECT *SkWinCore::_1031_01d5(U16 rectno, SRECT *rc)
{
	//^1031:01D5
	ENTER(4);
	//^1031:01DA
	U16 si = rectno;
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
U16 SkWinCore::_1031_0a88(sk0d9e *ref, U16 xx, U16 yy, U16 ww)
{
	//^1031:0A88
	ENTER(4);
	//^1031:0A8D
	if (ref == NULL)
		//^1031:0A95
		return 0;
	//^1031:0A9A
	U16 si = 0;
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
	U16 si = _4976_16ed[ref->w2].w2;
	//^1031:0264
	return (si == 0xffff) ? NULL : &_4976_0d9e[si];
}

//^1031:030A
U16 SkWinCore::_1031_030a(sk1891 *ref, U16 xx, U16 yy, U16 zz)
{
	//^1031:030A
	ENTER(16);
	//^1031:0310
	U16 di = yy;
	U16 si = 0;
	//^1031:0315
	U8 *bp08 = _1031_023b(ref);

	do {
		//^1031:0327
		sk1891 *bp04 = &_4976_1574[*bp08 & 0xff7f];
		//^1031:033E
		if ((this->*_4976_0cba_PFN_12[ref->b0_0_6()])(bp04) != 0) {
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
U16 SkWinCore::_1031_0c58(U16 xx, sk0d9e *ref)
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
		U16 si;
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
U16 SkWinCore::_1031_03f2(sk1891 *ref, U16 xx)
{
	//^1031:03F2
	ENTER(12);
	//^1031:03F8
	U16 si = 0;
	//^1031:03FA
	U8 *bp08 = _1031_023b(ref);

	do {
		//^1031:040C
		sk1891 *bp04 = &_4976_1574[*bp08 & 0xff7f];
		//^1031:0423
		if ((this->*_4976_0cba_PFN_12[ref->b0_0_6()])(bp04) != 0) {
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
				U16 di = _4976_16ed[bp04->w2].w4;
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
void SkWinCore::_1031_10c8(sk3f6c *ref, SRECT *rc, U16 cx, U16 cy)
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


//^2405:00EC
// _2405_00ec renamed _2405_00ec_RECT
void SkWinCore::_2405_00ec_RECT(U16 rectno, SRECT *rc)
{
	ENTER(4);
	i16 bp02 = glbRectX_0106;
	i16 bp04 = glbRectY_0108;
	QUERY_BLIT_RECT(
		NULL,
		rc,
		rectno,
		&bp02,
		&bp04,
		-1
		);
	return;
}

//^2405:011F
// _2405_011f renamed _2405_011f_RECT
void SkWinCore::_2405_011f_RECT(U16 rectno, SRECT *rc)
{
	ENTER(0);
	_2405_00ec_RECT(rectno, rc);
	INFLATE_RECT(rc, _4976_0112, _4976_0112);
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
			&& QUERY_CMDSTR_ENTRY(bp01, bp02, bp03, CnCM_Command) != 0
			&& QUERY_CMDSTR_ENTRY(
				QUERY_CLS1_FROM_RECORD(si),
				QUERY_CLS2_FROM_RECORD(si),
				bp03,
				CnNC_NumberCharge
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
	CHANGE_CURRENT_MAP_TO(cd.pi.glbPlayerMap);
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
//#ifndef __DJGPP__
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
					if (si.DBType() == dbCreature && !SkCodeParam::bDebugNoCreatures) {
						//^1C9A:3AD1
						Creature* xCreature = GET_ADDRESS_OF_RECORD4(si); // bp0c
						if (xCreature->iID != 0xFF)
							break;
						if (QUERY_CREATURE_AI_SPEC_FROM_TYPE(xCreature->CreatureType())->IsStaticObject() == 0) {
							ALLOC_CAII_TO_CREATURE(si, bp0e, di);
							//^1C9A:3B0E
							break;
						}
						U16 bp10 = xCreature->iAnimFrame;
						CREATURE_SET_ANIM_FRAME(si);
						xCreature->iAnimFrame |= bp10 & 0x6000;
						if ((bp10 & 0x803f) == 0x8001) {
							xCreature->iAnimFrame &= 0xffc0;
							xCreature->iAnimFrame |= 0x8001;
						}
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
//#endif // __DJGPP__	
	return;
}

//^13AE:0009
void SkWinCore::LOAD_NEWMAP(U8 newmap)
{
	//^13AE:0009
	SkD((DLV_DBG_INIT, "LOAD_NEWMAP (%d)\n", newmap));
	ENTER(0);
	//^13AE:000C
	if (_4976_5c8a == 0) {
		//^13AE:0013
		FIRE_HIDE_MOUSE_CURSOR();
		//^13AE:0018
		_3a15_38b6(0);
	}
	//^13AE:0020
	SkD((DLV_DBG_INIT, "LOAD_NEWMAP: LOAD_LOCALLEVEL_GRAPHICS_TABLE\n"));
	LOAD_LOCALLEVEL_GRAPHICS_TABLE(newmap);
	SkD((DLV_DBG_INIT, "LOAD_NEWMAP: LOAD_LOCALLEVEL_DYN\n"));
	LOAD_LOCALLEVEL_DYN();
	if (_4976_5c8a == 0) {
		SkD((DLV_DBG_INIT, "LOAD_NEWMAP: _3a15_38b6(1)\n"));
		_3a15_38b6(1);
		SkD((DLV_DBG_INIT, "LOAD_NEWMAP: FILL_CAII_CUR_MAP\n"));
		FILL_CAII_CUR_MAP();
		SkD((DLV_DBG_INIT, "LOAD_NEWMAP: RECALC_LIGHT_LEVEL\n"));
		RECALC_LIGHT_LEVEL();
		if (_4976_5bee[0] == 0)
			_1031_098e();
		FIRE_SHOW_MOUSE_CURSOR();
	}
	SkD((DLV_DBG_INIT, "LOAD_NEWMAP: Complete\n"));
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
	VIDEO_MEM_MOVE(&bp08, _4976_0140) INDIRECT_CALL;
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
	if (runes[1] == 0)
		return NULL;
	i16 iBitShifter = 24; // bp06
	i16 iSpellIndex = 0; // bp06 (was used for 2 different things)
	U32 bp0a = 0;

	do {
		bp0a |= U32(*(runes++)) << iBitShifter;
	} while (runes[0] != 0 && (iBitShifter -= 8) >= 0);
	
	SpellDefinition *xSpell = const_cast<SpellDefinition *>(dSpellsTable);	// bp04
	U32 iLocalMaxSpell = MAXSPELL_ORIGINAL;	// SPX added this to prevent overflow when switching tables

	// SPX : Use extended spells table read from GDAT if we are in custom mode
	if (SkCodeParam::bUseCustomSpells)
	{
		xSpell = const_cast<SpellDefinition *>(dSpellsTableCustom);
		iLocalMaxSpell = MAXSPELL_CUSTOM;
	} // end SPX

	for (iSpellIndex = iLocalMaxSpell; iSpellIndex-- != 0; xSpell++) {	// (bp06 = MAXSPELL; bp06-- != 0; bp04++)
		if ((xSpell->symbols & 0xFF000000) != 0) {
			if (xSpell->symbols != bp0a)
				continue;
			return xSpell;
		}
	    if ((bp0a & 0x00FFFFFF) == xSpell->symbols)
			return xSpell;
	}
	return NULL;
}









//^2759:01E3
U16 SkWinCore::QUERY_CUR_CMDSTR_ENTRY(U16 cmdNum)
{
	//^2759:01E3
	ENTER(0);
	//^2759:01E6
	return QUERY_CMDSTR_ENTRY(glbItemGDATCategory, glbItemGDATIndex, glbItemGDATEntry, cmdNum);
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
			di = cd.pi.glbPlayerDir;
			si = 0;
			goto _0adf;
		case 5://^0AC5 // forward and left
			//^12B4:0AC5
			di = (cd.pi.glbPlayerDir -1) & 3;
			si = 1;
			goto _0adf;
		case 1://^0AD3 // forward and right
			//^12B4:0AD3
			di = (cd.pi.glbPlayerDir +1) & 3;
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
			di = (cd.pi.glbPlayerDir + 2) & 3;
			break;
	}
	//^12B4:0B0B
	X16 bp04 = cd.pi.glbPlayerPosX + glbXAxisDelta[cd.pi.glbPlayerDir];
	X16 bp06 = cd.pi.glbPlayerPosY + glbYAxisDelta[cd.pi.glbPlayerDir];
	ObjectID bp08;
	if (IS_CREATURE_MOVABLE_THERE(bp04, bp06, di, &bp08) == 0)
		return;
	//^12B4:0B46
	if (_12b4_099e(bp08) == 0) {
		//^12B4:0B52
		U16 bp02;
		for (bp02 = 0; bp02 < cd.pi.glbChampionsCount; bp02++) {
			//^12B4:0B59
			if (glbChampionSquad[bp02].curHP() != 0) {
				//^12B4:0B6D
				if (bp02 == glbChampionLeader || RAND02() == 0) {
					//^12B4:0B7C
					// SPX: Sound made by champion when moving large object
					QUEUE_NOISE_GEN2(GDAT_CATEGORY_CHAMPIONS, U8(bp02), SOUND_CHAMPION_GETHIT, 0xFE, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 1, 0x69, 0xc8);
				}
			}
			//^12B4:0B9F
		}
	}
	else {
		//^12B4:0BAD
		glbTargetPosXTest = bp04;
		glbTargetPosYTest = bp06;
		glbPosX_4976_4eae = cd.pi.glbPlayerPosX;
		glbPosY_4976_4eb0 = cd.pi.glbPlayerPosY;
		_4976_4eac = di;
		glbMap_4976_4eaa = cd.pi.glbPlayerMap;
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

//^32CB:0A4C
// SPX: _32cb_0a4c renamed MAKE_BUTTON_CLICKABLE
void SkWinCore::MAKE_BUTTON_CLICKABLE(SRECT *prc, U8 xx, U8 yy)
{
	ENTER(0);
	COPY_MEMORY(prc, &tblViewportClickableRectangles[RCX(13,glbViewportClickableRectCount)].rc0, sizeof(SRECT));
	tblViewportClickableRectangles[glbViewportClickableRectCount].b11 = xx;
	tblViewportClickableRectangles[glbViewportClickableRectCount].b10 = yy;
	tblViewportClickableRectangles[glbViewportClickableRectCount].w8 = 0xFFFF;
	glbViewportClickableRectCount++;
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


// SPX: Added to bypass the critical for load / used for experimental, not guaranteed
U8 *SkWinCore::QUERY_GDAT_ENTRY_DATA_BUFF_FORCE(U8 cls1, U8 cls2, U8 cls3, U8 cls4)
{
LOGX(("%40s: C%02d=I%02X=E%02X=T%03d to %08X", "QUERY_GDAT_ENTRY_DATA_BUFF_FORCE from ", cls1, cls2, cls4, cls3 ));
	i16 si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, cls3, cls4);
	if (si == -1) {
		return NULL;
	}
	if (glbShelfMemoryTable[si].Absent()) {
		U16 bp02;
		return QUERY_GDAT_DYN_BUFF(si, &bp02, 0);
	}
	else {
		return REALIZE_GRAPHICS_DATA_MEMORY(glbShelfMemoryTable[si]);
	}
}


//^3E74:5163
U8 *SkWinCore::QUERY_GDAT_ENTRY_DATA_BUFF(U8 cls1, U8 cls2, U8 cls3, U8 cls4)
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
		U16 bp02;
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
	ENTER(0);
	return QUERY_CLS1_FROM_RECORD(ObjectID(0, GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(actuatorData), 0));
}

//^0CEE:1513
ObjectID SkWinCore::ALLOC_NEW_DBITEM_DEEPLY(U16 actuatorData)
{
	ENTER(6);
	U8 iType = GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(actuatorData); // bp05
	X16 iDBCat; // di
	ObjectID oObjRec = RECYCLE_A_RECORD_FROM_THE_WORLD(iDBCat = GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(actuatorData), iType); // si
	if (oObjRec == OBJECT_NULL) {
		oObjRec = ALLOC_NEW_RECORD(iDBCat);
		if (oObjRec == OBJECT_NULL)
			return oObjRec;
	}
	GenericRecord *oGenRecord; // bp04
	ZERO_MEMORY(oGenRecord = GET_ADDRESS_OF_RECORD(oObjRec), glbItemSizePerDB[iDBCat]);
	oGenRecord->w0 = OBJECT_END_MARKER;
	SET_ITEMTYPE(oObjRec, iType);
	return oObjRec;
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
	U16 di = cd.pi.glbPlayerPosX;
	U16 si = cd.pi.glbPlayerPosY;
	//^121E:0152
	U16 bp04 = tblViewportClickableRectangles[zz].b10;
	//^121E:0165
	if (bp04 != 0) {
		//^121E:0169
		if (bp04 != 3)
			//^121E:016E
			return;
		//^121E:0171
		di += glbXAxisDelta[cd.pi.glbPlayerDir];
		si += glbYAxisDelta[cd.pi.glbPlayerDir];
	}
	//^121E:0185
	ObjectID bp02 = _32cb_03a6(xx, yy, bp04, di, si, tblViewportClickableRectangles[zz].w8, tblViewportClickableRectangles[zz].b11, 0);
	//^121E:01BC
	if (bp02 != OBJECT_NULL) {
		//^121E:01C1
		if (tblViewportClickableRectangles[zz].b11 == 2) {
			//^121E:01D2
			MOVE_RECORD_TO(bp02, di, si, -1, -1);

			SkD((DLV_TWEET, "Tweet: You (x:%d, y:%d, map:%d) have taken %s from %s (a#%03d, x:%d, y:%d) \n"
				, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, glbCurrentMapIndex
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
				, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, glbCurrentMapIndex
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
	cd.gg.glbRefreshViewport = 1;
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
	ObjectID si = _32cb_03a6(aa, bb, cc, xx, yy, ObjectID(cd.pi.glbLeaderHandPossession.object, (cd.pi.glbPlayerDir + di) & 3), ee, 1);
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
			return CREATURE_121e_0222(xx, yy, di +4);
		}
		else {
			//^121E:040C
			return CREATURE_121e_0222(xx, yy, di);
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
	if (cd.pi.glbChampionsCount == 0)
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
		MOUSE_STATE_443c_08ab(&_4976_5da0, &_4976_5da2, &_4976_5dae.rc4.cy);
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
	cd.gg.glbRefreshViewport = 1;
	//^121E:0138
	return;
}





//^38C8:0060
void SkWinCore::_38c8_0060()
{
	//^38C8:0060
	ENTER(0);
	//^38C8:0064
	if (_4976_5bec != 0 && cd.gg.glbGameHasEnded == 0) {
		//^38C8:0072
		if (cd.pi.glbIsPlayerSleeping != 0) {
			//^38C8:0079
			DRAW_WAKE_UP_TEXT();
			//^38C8:007E
			CHANGE_VIEWPORT_TO_INVENTORY(0);
		}
		else {
			//^38C8:0088
			_4976_5bec = 0;
			//^38C8:008E
			if (cd.pi.glbChampionIndex == 0)
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
				DRAW_ARROW_PANEL();
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
		QUERY_GDAT_TEXT(GDAT_CATEGORY_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_BASE_DATA, 0x11, bp28)
		);
	//^1031:194F
	return;
}

//^2C1D:14BA
// SPX: _2c1d_14ba renamed RESUME_FROM_WAKE
void SkWinCore::RESUME_FROM_WAKE() 
{
	ENTER(0);
	cd.gg.glbRefreshViewport = 1; // SPX some move flag (viewport refresh?)
	cd.pi.glbIsPlayerSleeping = 0;
	//glbTickSpeed = 12;	// SPX holds tick balance?? 12 was an old value
	glbTickSpeed = stdTickBalance;
	INIT_BACKBUFF();
	_1031_0541(5);
	return;
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
		while (IS_THERE_KEY_INPUT_2() != 0) {
			_1031_0d36_KEYBOARD(32, SPECIAL_UI_KEY_TRANSFORMATION());
		}
		MAIN_LOOP();
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
				U8 bp15 = _476d_05b6_KEYBOARD(_4976_4e9c);
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
	ENTER(4);
	_4976_1a68 = 0;
	U16 iCreatureIndex;
	for (iCreatureIndex = 0; iCreatureIndex < glbCreaturesCount; iCreatureIndex++) {
		glbTabCreaturesInfo[iCreatureIndex].CreatureIndex(-1);
	}
	Creature *xCreature = reinterpret_cast<Creature *>(static_cast<U8 *>(glbDBObjectData[4]));
	for (iCreatureIndex = dunHeader->nRecords[dbCreature]; iCreatureIndex-- != 0; xCreature++) {
		xCreature->iID = 0xFF;
	}
//	if (SkCodeParam::bDebugNoCreatures)
//		return;
	FILL_ORPHAN_CAII();
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
					|| GET_DOOR_STAT_10(GET_GRAPHICS_FOR_DOOR(GET_ADDRESS_OF_TILE_RECORD(U8(xx), U8(yy))->castToDoor())) == 0
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
	if (cd.pi.glbChampionsCount != 0) {
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
							if ((tblAIStats01[glbCreatureAIStatIndex] & 0x200) != 0) {
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
									ww = (cd.pi.glbPlayerDir + 2) & 3;
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
//								if(SkCodeParam::bUseIngameDebug)
//									printf("BP12 = %04X, %d\n", bp12, bp12);
								switch (bp12) {
									case AI_ATTACK_FLAGS__PUSH_BACK:	// 0x0002 Knock back
										glbCurrentThinkingCreatureData->Command = ccm26_PushBack;
										break;
									case AI_ATTACK_FLAGS__MELEE:	// 0x0001 Standard attack
										glbCurrentThinkingCreatureData->Command = ccm08_MeleeAttack;
										break;
									case AI_ATTACK_FLAGS__STEAL:	// 0x0004 Steal?
										glbCurrentThinkingCreatureData->Command = ccm0A_Steal;
										glbCurrentThinkingCreatureData->ItemToThrow = 0x0B;	// ????
										break;
									case AI_ATTACK_FLAGS__FIREBALL:	// 0x0010	- 16
//											bp04 = i16(0xff80);		// fireball
										bp04 = i16(OBJECT_EFFECT_FIREBALL.w);
										goto _0c4a;
									case AI_ATTACK_FLAGS__DISPELL:	// 0x0020	- 32
//											bp04 = i16(0xff83);		// dispell
										bp04 = i16(OBJECT_EFFECT_DISPELL.w);
										goto _0c4a;
									case AI_ATTACK_FLAGS__LIGHTNING:	// 0x0040	- 64
//											bp04 = i16(0xff82);		// lightning
										bp04 = i16(OBJECT_EFFECT_LIGHTNING.w);
										goto _0c4a;
									case AI_ATTACK_FLAGS__POISON_CLOUD:	// 0x0080	- 128
//											bp04 = i16(0xff87);		// poison cloud
										bp04 = i16(OBJECT_EFFECT_POISON_CLOUD.w);
										goto _0c4a;
									case AI_ATTACK_FLAGS__POISON_BOLT:	// 0x0100	- 256
//											bp04 = i16(0xff86);		// poison bolt
										bp04 = i16(OBJECT_EFFECT_POISON_BOLT.w);
										goto _0c4a;
									case AI_ATTACK_FLAGS__PUSH_SPELL:	// 0x0400	- 1024
//											bp04 = i16(0xff89);		// push
										bp04 = i16(OBJECT_EFFECT_PUSH.w);
										goto _0c4a;
									case AI_ATTACK_FLAGS__PULL_SPELL:	// 0x0800	- 2048
//											bp04 = i16(0xff8a);		// pull
										bp04 = i16(OBJECT_EFFECT_PULL.w);	
										goto _0c4a;
									case AI_ATTACK_FLAGS__POISON_BLOB:	// 0x0200	- 512
//											bp04 = i16(0xff81);		// what's this??
										bp04 = i16(OBJECT_EFFECT_POISON_BLOB.w);
										//SPX: It does look like this effect doesn't work (no image)
										// I guess in actual code, we never reach this line.
_0c4a:
										glbCurrentThinkingCreatureData->ItemToThrow = U8(bp04);
										glbCurrentThinkingCreatureData->Command = (bp08 != 0) ? ccm28_CastSpell2 : ccm27_CastSpell1;
										break;
									case AI_ATTACK_FLAGS__SHOOT:	// 0x0008 Able to launch item / or maybe take item ? (axe, arrow?)
										glbCurrentThinkingCreatureData->Command = (bp08 != 0) ? ccm0F_ShootItem2 : ccm0E_ShootItem1;
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
// _19f0_2813 renamed CREATURE_19f0_2813
U16 SkWinCore::CREATURE_19f0_2813(U16 ww, i16 xx, i16 yy, i16 ss, i16 tt, i16 aa, U16 bb)
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
						if (glbCreatureSomeFirstObjectOnTile == OBJECT_NULL) {
							glbCreatureSomeFirstObjectOnTile = GET_TILE_RECORD_LINK(glbCreatureSomeX, glbCreatureSomeY);
						}
						//^19F0:28F0
						ObjectID si = glbCreatureSomeFirstObjectOnTile;
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
									si = _19f0_266c(glbCreatureSomeFirstObjectOnTile, (aa+2)&3, ww, bb);
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
												if (ww == 0) {
													glbCurrentThinkingCreatureData->Command = ccm2F_ActivateWallSwitch;
												}
												else if (ww == 1) {
													glbCurrentThinkingCreatureData->Command = ccm30_ActivateWallSwitch;
												}
												else {
													glbCurrentThinkingCreatureData->Command = ccm31_ActivateWallSwitch;
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
	if (glbCreatureSomeFirstObjectOnTile == OBJECT_NULL) {
		glbCreatureSomeFirstObjectOnTile = GET_TILE_RECORD_LINK(di, tt);
	}
	//^19F0:0E32
	door = GET_ADDRESS_OF_RECORD0(glbCreatureSomeFirstObjectOnTile);
	if (ww == 2 && door->DoorBit12() == 0)
		goto _123a;
	//^19F0:0E5C
	i16 bp10;
	i16 bp12;
	i16 bp14;
	U16 bp16;
	i16 bp08;
	if (door->DoorBit10() != 0) {
		//^19F0:0E6D
		if (door->DoorBit09() != 0) {
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
			if (door->DoorBit13() != 0)
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
		if (door->Button() != 0 || door->DoorBit13() != 0)
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
	if (bp06 == 2) {
		glbCurrentThinkingCreatureData->Command = ccm00_Neutral;
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
			door->DoorBit12(1);
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
			glbCurrentThinkingCreatureData->Command = (RAND01() != 0) ? ccm28_CastSpell2 : ccm27_CastSpell1;
			// SPX: 8D (unknow zo2?) replaced with 84 (open door zo1)
			glbCurrentThinkingCreatureData->ItemToThrow = (ww == 0) ? 0x8D : 0x84;	// zo spell 2 or zo spell 1
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
		if ((bp06 & 7) == 4 && GET_DOOR_STAT_0D(GET_GRAPHICS_FOR_DOOR(bp04)) == 0)
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
									if ((_4976_521c & 16) == 0)
										break;
									si = CREATURE_19f0_2813(bp0c->w10, bp42, bp44, bp12,bp14,bp30,bp0c->w8);
									if (si == 0)
										bp37 = 0xFF;
									//^1C9A:32C7
									break;

								case 23:
									//^1C9A:32C9
									if ((_4976_521c & 16)!=0){
										// SPX: bp2c = distance
										ADD_BACKGROUND_LIGHT_FROM_TILE(bp2c,bp30,bp12,bp14,4);
										bp37 = 0xFF;
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
							bp3c = CREATURE_1c9a_1b16(bp0c->w8, bp0c->w10);
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
								|| (bp3c = CREATURE_CHECK__1c9a_1a48(1, glbCreatureStat07)) == 0xffff
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
							if (bp16 != glbCreatureMap || (tlbCreatureCommandsFlags[RCJ(MAX_CREATURE_COMMANDS,glbCreatureCommandThinking)] & 3) != 0)
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
							ADD_BACKGROUND_LIGHT_FROM_TILE(bp2c, (bp30 == -1) ? cd.pi.glbPlayerDir : bp30, bp12, bp14, 3);

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
	ENTER(0);
	U16 iMapNo = mapno; // si
	if (glbCurrentMapIndex != iMapNo) {
		CHANGE_CURRENT_MAP_TO(iMapNo);
		if (iMapNo == glbMap_4976_4c12) {
			glbCreatureDir = glbDir_4976_4c10;
			glbCreaturePosX = glbSomePosX_4976_4c04;
			glbCreaturePosY = glbSomePosY_4976_4c1e;
			glbCreatureMap = glbMap_4976_4c12;
		}
		else {
			glbCreatureDir = cd.pi.glbPlayerDir;
			glbCreaturePosX = cd.pi.glbPlayerPosX;
			glbCreaturePosY = cd.pi.glbPlayerPosY;
			glbCreatureMap = cd.pi.glbPlayerMap;
		}
	}
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
	sGameVar.wChampionsCount = cd.pi.glbChampionsCount;
	sGameVar.wPlayerPosX = cd.pi.glbPlayerPosX;
	sGameVar.wPlayerPosY = cd.pi.glbPlayerPosY;
	sGameVar.wPlayerDir = cd.pi.glbPlayerDir;
	sGameVar.wPlayerMap = cd.pi.glbPlayerMap;
	sGameVar.wChampionLeader = glbChampionLeader;
	sGameVar.wTimersCount = glbTimersCount;
	sGameVar.dw22 = _4976_0090;
	sGameVar.dw26 = glbGameTick_4b80;
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
	DEBUG_HELP_WRITER("Champion Squad", glbChampionSquad, 261, cd.pi.glbChampionsCount);
	//^2066:138D
	if (SUPPRESS_WRITER(glbChampionSquad, _4976_3992, 261, cd.pi.glbChampionsCount) != 0)
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
	for (bp0e = 0; bp0e < cd.pi.glbChampionsCount; bp0e++) {
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
	DEBUG_HELP_WRITER("Leader Hand Object Ref", &cd.pi.glbLeaderHandPossession.object, 2, 1);
	//^2066:14AF
	if (WRITE_RECORD_CHECKCODE(cd.pi.glbLeaderHandPossession.object, 0, 0) != 0)
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



void SkWinCore::MessageLoop(bool bBalanceWait, bool bShortWait) {

	static UINT iInternalMLCount = 0;

	SkD((DLV_DBG_INIT, "MessageLoop %06d: %d %d | tickThen = %d\n", iInternalMLCount, (int)bBalanceWait, (int)bShortWait, tickThen)); iInternalMLCount++;
	if (bBalanceWait) {
		if (bShortWait) {
			if (!SkCodeParam::bEngineNoDisplay && !skWinApp->skwin_ML()) {	// SPX: added param to allow continue if in "no display" mode
				//SkD((DLV_DBG_INIT, "throw i8 [1]\n"));
				throw i8();
			}
		}
		else {
			if (tickThen == (U32)-1)
				tickThen = skWinApp->skwin_GetTickCount();
			while (true) {
				if (!SkCodeParam::bEngineNoDisplay && !skWinApp->skwin_ML()) { // SPX: added param to allow continue if in "no display" mode
					//SkD((DLV_DBG_INIT, "throw i8 [2]\n"));
					throw i8();
				}
				U32 tickDelta = skWinApp->skwin_GetTickCount() - tickThen;
				SkD((DLV_DBG_INIT, "MessageWhile : tickThen = %d / delta = %d / wait = %d / stay = %d\n", tickThen, tickDelta, bShortWait, TICK_TIME_STAY));
				if (tickDelta >= (bShortWait ? (TICK_TIME_STAY >> 1) : (TICK_TIME_STAY)))
					break;
				SkD((DLV_DBG_INIT, "MessageWhile : skwin_Sleep\n"));
				skWinApp->skwin_Sleep(10); // wait 1 millisec
			}
			SkD((DLV_DBG_INIT, "MessageWhile : tickThen\n"));
			tickThen = skWinApp->skwin_GetTickCount();
		}
	}

	if (_04bf_0e7a != 0) // if block hardware interruption
		return;
	skWinApp->skwin_Sleep(1);
//#if defined(__DJGPP__) || defined(__LINUX__)
	//printf("Call GetMousePos\n");
	skWinApp->GetMousePosButtons(&cd.mk.mice_x, &cd.mk.mice_y, &cd.mk.mice_btn);
	(this->*_int33_mouse_callback)();
//#else
	// mouse event notice
	for (int i = 0; i < skWinApp->cntMiceIn; i++) {
		cd.mk.mice_btn = skWinApp->xMiceInput[i].btn;
		cd.mk.mice_x = skWinApp->xMiceInput[i].x;
		cd.mk.mice_y = skWinApp->xMiceInput[i].y;
		(this->*_int33_mouse_callback)();
	}
//#endif
	skWinApp->cntMiceIn = 0;

//#if defined (__DJGPP__)
	//printf("Calling poll ");
	//skwin.PollKeyboard();
//#endif
	//if (SkCodeParam::bEngineNoDisplay)
	//	Sleep(500);

	// key event notice
	if (skWinApp->cntKeybIn != 0) {
		(this->*_sys_getvect(0x09))();
	}

	// interval timer notice
	(this->*_sys_getvect(0x08))();
	SkD((DLV_DBG_INIT, "End of MessageLoop\n"));
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
// _12b4_0881
X16 SkWinCore::_12b4_0881_CHECK_MOVE_BETWEEN_TILES(X16 aa, U16 oldTile, U16 newTile, i16 xx, i16 yy, ObjectID *rl)
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
			if (_12b4_0953(GET_ADDRESS_OF_RECORD4(*rl), cd.pi.glbPlayerDir) != 0)
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

// SPX: new => check between tiles + interwalls
X16 SkWinCore::CHECK_MOVE_BETWEEN_TILES_AND_INTERWALLS(X16 aa, U16 iInterwall, U16 oldTile, U16 newTile, i16 xx, i16 yy, ObjectID *rl)
{
	X16 iCheckResult = 0;
	if (iInterwall == 1 )//|| iInterwall == 3)
		return 3;	// consider tile is blocked (as wall)
	iCheckResult = _12b4_0881_CHECK_MOVE_BETWEEN_TILES(aa, oldTile, newTile, xx, yy, rl);
	return iCheckResult;
}


//^12B4:023F
// SPX: Walk into a wall ?
// _12b4_023f renamed _12b4_023f_WOUND_RUNNING_INTO_CREATURE
U16 SkWinCore::_12b4_023f_WOUND_RUNNING_INTO_CREATURE(i16 xx, i16 yy, i16 *ss, i16 *tt, i16 ww, X16 vv)
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
				QUEUE_NOISE_GEN2(GDAT_CATEGORY_CHAMPIONS, glbChampionSquad[bp02].HeroType(), SOUND_CHAMPION_BUMP, 0xFE, xx, yy, 1, 0x64, 0xC8);
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
	for (si = 0; si < cd.pi.glbChampionsCount; si++) {
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
	for (si = 0; si < cd.pi.glbChampionsCount; si++) {
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
		if ((this->*_4976_0cba_PFN_12[ref->b0_0_6()])(bp08) != 0) {
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



//^4726:0383
// _4726_0383 renamed TICK_STEP_CHECK
void SkWinCore::TICK_STEP_CHECK()
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


//^476D:04E8
U8 SkWinCore::_476d_04e8(U16 xx) { // TODO: Unr
	Unr(); return 0;
}

//^476D:04AF
U16 SkWinCore::_476d_04af(U16 xx) { // TODO: Unr
	Unr(); return 0;
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
void SkWinCore::_2fcf_0b8b(U16 xx, U16 yy, U16 zz)
{
	//^2FCF:0B8B
	ENTER(6);
	//^2FCF:0B91
	glbMap_4976_4c12 = 0xffff;
	//^2FCF:0B97
	TELE_inf bp06;
	_4976_4c08 = GET_TELEPORTER_DETAIL(&bp06, U8(xx), U8(yy));
	//^2FCF:0BAD
	if (_4976_4c08 != 0) {
		//^2FCF:0BB1
		glbMap_4976_4c12 = bp06.target_z();
		glbSomePosX_4976_4c04 = bp06.target_x_pos();
		glbSomePosY_4976_4c1e = bp06.target_y_pos();
		glbDir_4976_4c10 = (cd.pi.glbPlayerDir + (bp06.cross_scene_test_of_teleporter_target() - bp06.cross_scene_test())) & 3;
	}
	else {
		//^2FCF:0BE6
		for (U16 si = 0; si < 4; si++) {
			//^2FCF:0BEA
			if (GET_TELEPORTER_DETAIL(&bp06, xx + glbXAxisDelta[si], yy + glbYAxisDelta[si]) != 0) {
				//^2FCF:0C13
				glbMap_4976_4c12 = bp06.target_z();
				//^2FCF:0C1B
				U16 di = bp06.cross_scene_test_of_teleporter_target() +6 -bp06.cross_scene_test();
				//^2FCF:0C2C
				si = (si +di +2) & 3;
				//^2FCF:0C37
				glbSomePosX_4976_4c04 = bp06.target_x_pos() + glbXAxisDelta[si];
				//^2FCF:0C47
				glbSomePosY_4976_4c1e = bp06.target_y_pos() + glbYAxisDelta[si];
				//^2FCF:0C57
				glbDir_4976_4c10 = (cd.pi.glbPlayerDir + di) & 3;
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
U16 SkWinCore::_482b_015c(U16 xx)
{
	//^482B:015C
	ENTER(0);
	//^482B:0160
	for (U16 si = 0; si < _4976_49d4[0][0]; si++) {
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



//^482B:07C2
void SkWinCore::_482b_07c2(U16 xx)
{
	//^482B:07C2
	ENTER(0);
	//^482B:07C7
	U16 si;
	for (si = 0; _4976_49d4[0][0]-- > si; ) {
		//^482B:07CB
		U16 di = _4976_5f06[_4976_49d4[0][0]].w5;
		//^482B:07DF
		if (di != 0xffff) {
			//^482B:07E4
			if (_482b_015c(di) == 0) {
				//^482B:07EE
				_47eb_00d9_AUDIO(&_4976_5f0a[_4976_5f06[_4976_49d4[0][0]].w0]);
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
void __DECLSPEC_NORETURN_ SkWinCore::__terminate(U8 errorLevel)
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
	ENTER(0);
	EMS_FREE();
	_farfree(glbCRAMData);
	__terminate((_4976_4c26 != 0) ? 1 : 0);
	return;
}

//^069A:043C
/*
void SkWinCore::_sk_longjmp(jmp_buf xx, U16 yy)
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


//^3929:000F
U16 SkWinCore::QUERY_MBCS_PRESENCE(const U8 *str)
{
	//^3929:000F
	U8 dl;
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
U16 SkWinCore::QUERY_STR_METRICS(const U8 *buff, i16 *xx, i16 *yy)
{
	//^3929:03DC
	//^3929:03E2
	if (buff == NULL) {
		//^3929:03EA
		return 0;
	}
	//^3929:03EF
	i16 si;
	U16 di;
	if (QUERY_MBCS_PRESENCE(buff) != 0) {
		//^3929:0402
		si = - _4976_0132;
		U8 bp05 = 1;
		di = 0;
		//^3929:040F
		U8 bp07;
		for (; *buff != 0; ) {
			//^3929:0412
			U8 bp06 = *(buff++);
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
			U8 bp08;
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
//inline U16 SkWinCore::QUERY_STR_METRICS(U8 *buff, U16 *xx, U16 *yy)
U16 SkWinCore::QUERY_STR_METRICS_U16(U8 *buff, U16 *xx, U16 *yy)
{
	return QUERY_STR_METRICS(buff, reinterpret_cast<i16 *>(xx), reinterpret_cast<i16 *>(yy));
}

//^3A15:020F
void SkWinCore::_3a15_020f()
{
	//^3A15:020F
	ENTER(4);
	//^3A15:0215
	U16 timerCnt = glbTimersCount;	// di
	//^3A15:021A
	if (timerCnt == 0)
		//^3A15:021C
		return;
	//^3A15:0220
	U16 si;
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
	U16 si = 0;
	U16 di = glbTimersCount;
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
U16 SkWinCore::_RAND02()
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
U16 SkWinCore::CALC_SIZE_OF_COMPRESSED_RECT(U16 xx)
{
	//^098D:02D2
	U16 di = xx;
	U16 si = 8;
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
	// TODO: 本ズレ函数
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
	// TODO: 本ズレ函数
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
U16 SkWinCore::QUERY_GDAT_ENTRY_DATA_LENGTH(U8 cls1, U8 cls2, U8 cls3, U8 cls4)
{
LOGX(("%40s: C%02d=I%02X=E%02X=T%03d = %08X (%06d)", "QUERY_GDAT_ENTRY_DATA_LENGTH of ", cls1, cls2, cls4, cls3, QUERY_GDAT_RAW_DATA_LENGTH(QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, cls3, cls4)), QUERY_GDAT_RAW_DATA_LENGTH(QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, cls3, cls4)) ));

	//^3E74:5AB7
	return QUERY_GDAT_RAW_DATA_LENGTH(
		QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, cls3, cls4)
		);
}

//^098D:1185
void SkWinCore::LOAD_RECTS_AND_COMPRESS(U8 cls1, U8 cls2, U8 cls4) //#DS=4976?
{
	// TODO: 本ズレ函数
	//^098D:1185
	//^098D:1189
	Bit32u bp08 = QUERY_GDAT_ENTRY_DATA_LENGTH(cls1, cls2, dt04, cls4);
	//^098D:11A2
	U8 *bp04 = ALLOC_MEMORY_RAM(bp08, afUseLower, 1024);
	//^098D:11C0
	LOAD_GDAT_ENTRY_DATA_TO(cls1, cls2, dt04, cls4, bp04);
	//^098D:11DC
	COMPRESS_RECTS((i16 *)bp04, bp08, &glbRectNoTable, &SkWinCore::ALLOC_UPPER_MEMORY);
	//^098D:11F9
	DEALLOC_LOWER_MEMORY(bp08);
}

//^3E74:51E4
void SkWinCore::LOAD_GDAT_ENTRY_DATA_TO(U8 cls1, U8 cls2, U8 cls3, U8 cls4, U8 *where) //#DS=4976?
{
//LOGX(("%40s: C%02d=I%02X=E%02X=T%03d to %08X", "LOAD_GDAT_ENTRY_DATA_TO of ", cls1, cls2, cls4, cls3, where ));
	// TODO: 本ズレ函数
	//^3E74:51E4
	U16 si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, cls3, cls4);
	//^3E74:5201
//LOGX(("LOAD_GDAT_RAW_DATA call from LOAD_GDAT_ENTRY_DATA_TO"));
	LOAD_GDAT_RAW_DATA(si, CONVERT_PHYS_TO_SHELF_FORM(where));

	SkD((DLV_GLD, "GLD: Dyn1-load Raw#%4d at RAM(%p)\n", (Bitu)si, where));
}

//^098D:116D
U8 *SkWinCore::ALLOC_UPPER_MEMORY(U32 size)
{
	// TODO: 本ズレ函数
	//^098D:116D
	//^098D:1170
	return ALLOC_MEMORY_RAM(size, afUseUpper, 1024);
}

//^098D:0E78
void SkWinCore::COMPRESS_RECTS(i16 *data, Bit32u size, RectTable *zz, U8 *(SkWinCore::*allocFunc)(Bit32u size))
{
#define	PTR_ADVANCE(PTR,DELTA) *((U8 **)&(PTR)) += (DELTA);
#define	WRITE_BYTE(PTR,VAL) **(U8 **)&(PTR) = (VAL);
#define	WRITE_WORD(PTR,VAL) **(U16 **)&(PTR) = (VAL);

	// TODO: 本ズレ函数
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
		U16 bp0a = *bp08; bp08++;
		//^098D:0EE0
		U16 bp0c = *bp08; bp08++;
		//^098D:0EED
		i16 si = bp0c - bp0a +1;
		//^098D:0EF3
		i16 *bp16 = data;
		//^098D:0EFF
		U8 bp17 = 0x1f;
		//^098D:0F03
		U16 bp1a = data[0];
		//^098D:0F0C
		U16 bp1c = data[1];

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
			U16 bp1e = 2;

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
		si = bp0c - bp0a +1;
		//Bit32u bp12 = CALC_SIZE_OF_COMPRESSED_RECT(bp17) * si + 10; //win32
		// zel: Use sizeof(RectTable) instead of 10 for 64 bits compatibility ! (hard to find bug !)
		Bit32u bp12 = CALC_SIZE_OF_COMPRESSED_RECT(bp17) * si + sizeof(RectTable);

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
		bp04->b8 = (U8)bp17;
		//^098D:101B
		bp04->b9 = (U8)bp1a;
		SkD((DLV_RCT, "RCT: COMPRESS_RECTS (%p,(%d,%d,%d,%d))\n"
			, (RectTable*)bp04->pb0, (U16)bp04->w4, (U16)bp04->w6, (U8)bp04->b8, (U8)bp04->b9));
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
				WRITE_BYTE(bp04,(U8)*data); data++;
				//^098D:1065
				PTR_ADVANCE(bp04,+1);
				//^098D:1072
				WRITE_BYTE(bp04,(U8)*data); data++;
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
				WRITE_BYTE(bp04,(U8)*data); data++;
				//^098D:10F9
				PTR_ADVANCE(bp04,+1);
				//^098D:1106
				WRITE_BYTE(bp04,(U8)*data); data++;
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
SRECT *SkWinCore::QUERY_RECT(RectTable *entry, U16 rectno) //#DS=4976?
{
#define	PTR_ADVANCE(PTR,DELTA) *((U8 **)&(PTR)) += (DELTA);
#define	READ_BYTE(PTR) **((U8 **)&(PTR))
#define	READ_SBYTE(PTR) **((i8 **)&(PTR))
#define	READ_WORD(PTR) **((U16 **)&(PTR))

	// TODO: 単体動作確認済 13:59 2006/05/07
	//^098D:030F
	//^098D:0315
	U16 iRequestedRectNo = rectno;	// U16 si
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
		U8 bp05 = entry->b8;

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
SRECT *SkWinCore::SET_SRECT(SRECT *prc, U16 x, U16 y, U16 cx, U16 cy)
{
	prc->x = x;
	prc->y = y;
	prc->cx = cx;
	prc->cy = cy;
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


//^00EB:0845
void SkWinCore::_00eb_0845(U8 *buff, SRECT *rc, U16 ww) //#DS=04BF
{
	//^00EB:0845
	//^00EB:084B
	LOADDS(0x0c48);
	//^00EB:0851
	U16 bp12 = ((ww & 0x8000) != 0) ? 1 : 0;
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
		U16 si = rc->y * 320 + rc->x;
		U16 bp02 = 0;
		U16 di = 0;
		U16 bp04 = rc->y;
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
					(U8)_04bf_079c
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
			_04bf_0e34 = (U8 *)__vram;
			_04bf_0cf0 = buff;
			//^00EB:08D2
			U16 di = 0;
			U16 si = 320*40;
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
	//skwin.UpdateRect(0, 40, 224, 136);
	skWinApp->renderScreen(vram, 0, 40, 224, 136);
#endif

	//^00EB:0BBA
	UNLOCK_MOUSE_EVENT();
	//^00EB:0BBF
	return;
}

//^44C8:1BE8
// SPX: _44c8_1be8 renamed CHANGE_VIEWPORT_TO_INVENTORY
void SkWinCore::CHANGE_VIEWPORT_TO_INVENTORY(U16 xx) //#DS=4976
{
	//^44C8:1BE8
	//^44C8:1BEE
	U16 di = cd.pi.glbIsPlayerMoving;
	cd.pi.glbIsPlayerMoving = 0;
	//^44C8:1BF8
	SRECT bp0e;
	// SPX: add value init for rect
	bp0e.x = 0;
	bp0e.y = 0;
	bp0e.cx = 0;
	bp0e.cy = 0;
	QUERY_EXPANDED_RECT(7, &bp0e);
	//^44C8:1C07
	cd.pi.glbIsPlayerMoving = di;
	//^44C8:1C0B
	U16 si;
	if (glbPaletteIRGBLoaded == 0) {
		//^44C8:1C12
		i16 bp02, bp04, bp06;
		MOUSE_STATE_01b0_0d39(&bp02, &bp04, &bp06, 1) CALL_IBMIO;
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
	_00eb_0845(_4976_4c16, &bp0e, (cd.pi.glbIsPlayerMoving != 0) ? 0x8008 : 0x0008) CALL_IBMIO;
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
	if (cd.gg.glbGameHasEnded != 0) {
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
U16 SkWinCore::max_value(i16 v1, i16 v2) {
	//^0CD5:00A0
	//^0CD5:00A3
	if (v1 > v2)
		return v1;
	return v2;
}

//^0CD5:008E
U16 SkWinCore::min_value(i16 v1, i16 v2) {
	//^0CD5:008E
	//^0CD5:0091
	if (v1 < v2)
		return v1;
	return v2;
}



//^3E74:178C
//RawEntry *SkWinCore::QUERY_GDAT_ENTRYPTR(U8 cls1, U16 cls2, U8 cls3, U16 cls4)
RawEntry *SkWinCore::QUERY_GDAT_ENTRYPTR(U8 iGDatCategory, U16 iGDatItemId, U8 iGDatEntryType, U16 iGDatEntryId)
{
	// cls1 - main category	-> iCategory
	// cls2 - sub category	-> iItem
	// cls3 - type			-> iType
	// cls4 - name			-> iEntry

	// If requested category is above max category
	if (iGDatCategory > U8(glbGDatEntries.iTotalClass1)) {
		return NULL;
	}
	U16 si = glbGDatEntries.pw0[iGDatCategory];	// si
	if (glbGDatEntries.pw0[iGDatCategory + 1] - si < iGDatEntryType) {
		return NULL;
	}
	//^3E74:17D2
	si += iGDatEntryType;
	U16 di = glbGDatEntries.pw4[si];	// di
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

	RawEntry *bp08 = &glbGDatEntries.pv8[di - 1];
	//^3E74:180D
	// SPX: replaced di / si with boundary
	U16 iLowerBound = 0;
	U16 iUpperBound = 0;
	iUpperBound = glbGDatEntries.pw4[si + 1] - di + 1;
	//^3E74:1820
	iLowerBound = 0;
	while (true) {
		U16 iMidIndex = (iLowerBound + iUpperBound) / 2;	// bp0a
		if (!(iMidIndex != iLowerBound))
			return NULL;
		RawEntry* xRawEnt = &bp08[iMidIndex];	//*bp04
		i16 iSearchDirection = xRawEnt->cls2 - iGDatItemId;	// bp0c
		if (!(iSearchDirection != 0)) {
			iSearchDirection = xRawEnt->cls4 - iGDatEntryId;
			if (!(iSearchDirection != 0)) {
				return xRawEnt;
			}
		}
		if (!(iSearchDirection <= 0)) {
			iUpperBound = iMidIndex;
		}
		else {
			iLowerBound = iMidIndex;
		}
	}
}

//^3E74:1CF3
//U16 SkWinCore::QUERY_GDAT_ENTRY_IF_LOADABLE(U8 cls1, U8 cls2, U8 cls3, U8 cls4)
U16 SkWinCore::QUERY_GDAT_ENTRY_IF_LOADABLE(U8 iCategory, U8 iItemIndex, U8 iDataType, U8 iEntryNumber)
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
U16 SkWinCore::CALC_IMAGE_BYTE_LENGTH(U8 *buff)
{
	//^0B36:000A
	//^0B36:000D
	return ((READ_UI16(buff,-6) == 4) ? (((READ_UI16(buff,-4) +1) & 0xfffe) >> 1) : READ_UI16(buff,-4) & 0xffff) * READ_UI16(buff,-2);
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
SRECT *SkWinCore::CALC_CENTERED_RECT_IN_RECT(SRECT *rcNew, const SRECT *rcBBox, U16 newWidth, U16 newHeight)
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
void SkWinCore::QUERY_TOPLEFT_OF_RECT(U16 rectno, i16 *xpos, i16 *ypos)
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
void SkWinCore::SLEEP_SEVERAL_TIME(U16 count)
{
	while (count-- != 0) {
		WAIT_SCREEN_REFRESH();
		MessageLoop(true, true);
	}
}

//^0AAF:01DB
void SkWinCore::_0aaf_01db(U16 rectno, U16 yy)
{
	//^0AAF:01DB
	//^0AAF:01E0
	if (_4976_5cb0 != 0) {
		//^0AAF:01EA
		if (yy == 0)
			//^0AAF:01F0
			return;
		//^0AAF:01F3
		U8 *bp04 = QUERY_GDAT_IMAGE_ENTRY_BUFF(_4976_4bd5, _4976_4bd6, _4976_4bd4);
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
		U8 *bp08;
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
		U8 si = (yy != 0) ? (glbPaletteT16[COLOR_BLACK]) : (glbPaletteT16[COLOR_DARK_GRAY]);
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
U8 SkWinCore::_0aaf_02f8_DIALOG_BOX(U8 xx, U8 yy) //#DS=4976
{
	U16 si;
	skxxx1 bp04e4[2];
	U8 bp04d4[128];
	U8 bp0454[128];
	U8 bp03d4[0x14][40];
	U8 *bp00b4[0x14];	// panel buttons texts
	U8 bp64[60];
	U16 bp28[2];
	skxxx1 *bp24;
	SRECT bp20;
	U8 bp17;
	U16 bp16;
	U16 bp14;
	U16 bp12;
	U16 bp10;
	U16 bp0e;
	U16 bp0c;
	U16 bp0a;
	U8 *bp08;
	U8 *bp04;

	// SPX clean init
	for (bp17 = 0; bp17 < 0x14; bp17++) {
		bp00b4[bp17] = NULL;
	}

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
		for (U16 si=0; si < bp0c; si++) {
			//^0AAF:04F3
			_0aaf_01db(_4976_01bc[0][RCJ(8,si + bp10)], 0);
			//^0AAF:0506
		}
	}
	//^0AAF:050C
	// SPX: Draw string version on dialog box
	int iColorVersion = COLOR_GRAY;
	int iColorButton = COLOR_ORANGE;
	if (SkCodeParam::bDM2V5Mode) {
		iColorVersion = COLOR_YELLOW;
		iColorButton = COLOR_YELLOW;
	}
	DRAW_VP_RC_STR(0x1c2, glbPaletteT16[iColorVersion], strVersionNumber);
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
	for (si=0; si < bp0c; si++) { // display button texts
		//^0AAF:0571
		DRAW_VP_RC_STR(_4976_01bc[1][RCJ(8,si + bp10)], glbPaletteT16[iColorButton], bp00b4[si]);
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
	U16 di = 0;
	si = 0;
	//^0AAF:0647
	bp24 = &bp04e4[0];
	//^0AAF:0651
	for (bp16=0; bp16 < 2; bp16++, bp24++) {
		//^0AAF:0659
		if (bp24->pb0 != 0) {
			//^0AAF:0668
			QUERY_STR_METRICS_U16(bp24->pb0, &bp24->w4, &bp24->w6);
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
	if ((glbGDatOpenCloseFlag--) == 0) {
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
U16 SkWinCore::_RAND() //#DS=4976
{
	// CSBwinSimilarity: TAG001030,STRandom

	//^0CD5:00D2
	glbRandomSeed = glbRandomSeed * 0xbb40e62d + 11;
	//^0CD5:00F5
	return glbRandomSeed >> 8;
	//^0CD5:00FD
}




//^476D:007A
i16 SkWinCore::FILE_OPEN(const U8 *filename) {
	i16 iHandle = fset.fileOpen(reinterpret_cast<const char *>(filename));
	SkD((DLV_FIL, "FILE_OPEN %s with HANDLE %d\n", filename, iHandle));
	return iHandle;
}

//^476D:00D2
U16 SkWinCore::FILE_READ(U16 handle, Bit32u buffSize, void *buff) {
	return fset.fileRead(handle, buffSize, buff);
}

//^476D:00A5
void SkWinCore::FILE_CLOSE(U16 handle) {
	fset.fileClose(handle);
}

//^476D:00BF
void SkWinCore::FILE_RENAME(const U8 *curf, const U8 *newf)
{
	fset.fileMove(reinterpret_cast<const char *>(curf), reinterpret_cast<const char *>(newf));
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
#if defined(__DJGPP__) || defined(__MINGW__)
	if ((U32(dssi) & 1) != 0) {	// SPX not sure of the meaning of pointer & 1
#elif defined(__LINUX__)
	if ((U8((*dssi)%0x100) & 1) != 0) {
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




//^476D:030A
U16 SkWinCore::_476d_030a(U16 xx) { // TODO: Unr
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
U8 *SkWinCore::EMS_MAP_BANK_TO_MEMORY(shelf_memory info, U16 physPage0to3) { // TODO: Unr
#if UseAltic
	return REALIZE_GRAPHICS_DATA_MEMORY(info);
#else
	return Unr(), NULL;
#endif
}

//^3E74:0683
U8 *SkWinCore::FIND_FREE_POOL(U32 size, U16 poolno)
{
	//^3E74:0683
	U16 si = poolno;
	//^3E74:068B
	EMS_MAP_BANK_TO_MEMORY();
	//^3E74:0699
	sk5d00 *bp04 = _4976_5d00;
	//^3E74:06A6
	U8 *bp08 = 0;
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
			bp08 = (U8 *)bp04;
		}
		//^3E74:0729
		bp04 = bp04->pv0;
		//^3E74:0739
	}
	//^3E74:074D
	return bp08;
}

U8 *SkWinCore::ALLOC_MEMORY_RAM(U32 size, U16 flags, U16 poolno) {
	ATLASSERT((poolno & 0x800) == 0);
	U8 *pb = t2ptr(ALLOC_MEMORY_(size, flags, poolno));
	SkD((DLV_MEM, "MEM: ALLOC_MEMORY_RAM(%10d,%04X,%04X) = %08X  (free:%7u)\n", (Bitu)size, (Bitu)flags, (Bitu)poolno, pb, (Bitu)glbFreeRAMMemPool));
	return pb;
}
shelf_memory SkWinCore::ALLOC_MEMORY_EMS(U32 size, U16 flags, U16 poolno) {
	ATLASSERT((poolno & 0x800) != 0);
	shelf_memory ps = t2s(ALLOC_MEMORY_(size, flags, poolno));
	SkD((DLV_MEM, "MEM: ALLOC_MEMORY_EMS(%10d,%04X,%04X) = %08X  (free:%7u)\n", (Bitu)size, (Bitu)flags, (Bitu)poolno, (Bitu)ps.val, (Bitu)glbFreeEMSMemPool));
	return ps;
}

//^3E74:088E
tiamat SkWinCore::ALLOC_MEMORY_(U32 size, U16 flags, U16 poolno)
{
	// if (flags & 0x7FFF)==1, try to allocate memory from upper free area (for temporary use?)
	// if (flags & 0x7FFF)==2, try to allocate memory from lower free area (for permanent use?)
	// if (flags & 0x8000)!=0, clear memory with 0

	// if (poolno & 0x0c00)==0x0c00, it forces engine to allocate ibmio free memory. if no avail, SYSTEM ERROR 39
	// if (poolno & 0x0800)!=0x0000, it forces engine to allocate external free memory. if no avail, SYSTEM ERROR 39

	// if out of memory, always SYSTEM ERROR 40

	//ATLASSERT(0x200000 <= U32(_4976_5d5a) && U32(_4976_5d5a) <= 0x200000 +sizeof(cems));
	//ATLASSERT(cram <= _4976_5cf4 && _4976_5cf4 <= cram + shelf_memory::SizeOf_cems()); // replaced sizeof(cram)
	SkD((DLV_MEM, "MEM: ALLOC_MEMORY_: CRAM / _4976_5cf4 / CRAM+S(CEMS) = 1)%08X / 2)%08X / 2-1) %08X (%05d) / 3)%08X / 3-1) %08X (%06d)\n",
		xCRAM, _4976_5cf4, _4976_5cf4-xCRAM, _4976_5cf4-xCRAM, (xCRAM + shelf_memory::SizeOf_cems()), shelf_memory::SizeOf_cems(), shelf_memory::SizeOf_cems() ));
	ATLASSERT(xCRAM <= _4976_5cf4 && _4976_5cf4 <= xCRAM + shelf_memory::SizeOf_cems()); // replaced sizeof(cram)
	ATLASSERT(0 <= glbFreeRAMMemPool);

	//^3E74:088E
	U16 si = flags;
	U16 di = poolno;
	U16 bp0a = si & 0x8000;
	tiamat bp04;
	si &= 0x7fff;
	//^3E74:08A6
	if ((size & 1) != 0)
		size++;
	//^3E74:08B9
	if (si == 1) {
		//^3E74:08BE
		U8 *bp08 = FIND_FREE_POOL(size, di);
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
					/* ZEL commented
					if ((_4976_5d6e & 0x0800) != 0) {
						_4976_5d5a -= size;
						bp04 = _4976_5d5a;
						goto _09f7;
					}
					else {
						_4976_5d5a -= size;
						bp04 = _4976_5d5a;
						goto _09f7;
					}*/
					// ZEL
					{
						_4976_5d5a -= size;
						bp04 = _4976_5d5a;
						goto _09f7;
					}
					// ZEL
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

//#if defined(_USE_MFC80) || defined(_USE_MFC60) || defined(_USE_SDL) || defined(__DJGPP__) || defined(__MINGW__) || defined(__LINUX__)
		vecLowerAlloc.push(size);
//#endif // defined(_USE_MFC80) || defined(_USE_MFC60) || defined(_USE_SDL)
	}
	else { // from upper pool
		//^3E74:09CC
		_4976_5cf4 -= size;
		bp04 = ptr2t(_4976_5cf4);

//#if defined(_USE_MFC80) || defined(_USE_MFC60) || defined(_USE_SDL) || defined(__DJGPP__) || defined(__MINGW__) || defined(__LINUX__)
		vecUpperAlloc.push(size);
//#endif // defined(_USE_MFC80) || defined(_USE_MFC60) || defined(_USE_SDL)
	}
	//^3E74:09E9
	SkD((DLV_MEM, "MEM: %d - %d => %d\n", glbFreeRAMMemPool, size, (glbFreeRAMMemPool - size)));
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
U8 *SkWinCore::ALLOC_PICT_BUFF(U16 srccx, U16 srccy, U16 flags, U16 bpp)
{
	//^3E74:0B96
	U16 si = srccx;
	//^3E74:SkWinCore::0B9E
	U8 *bp04 = ALLOC_MEMORY_RAM((((bpp == 4) ? (((si +1) & 0xfffe) >> 1) : (si & 0xffff)) * srccy) +6, flags, 8) +6;
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
U8 *SkWinCore::REALIZE_GRAPHICS_DATA_MEMORY(shelf_memory info) {
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
		return &xCEMS[iMemVal - 0x00200000];
	}
	//else if (iMemVal >= sizeof(xCRAM)) {
	else if (iMemVal >= skWinApp->iSizeCRAM) {
		ATLASSERT(false);
		return NULL;
	}
	else {
		return &xCRAM[iMemVal];
	}
#else
	ATLASSERT(false);
	return Unr(), NULL;
#endif
}

//^3E74:0422
U16 SkWinCore::QUERY_GDAT_RAW_DATA_LENGTH(U16 index)
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
U16 SkWinCore::IS_CLS1_CRITICAL_FOR_LOAD(U8 cls1)
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
i32 SkWinCore::QUERY_GDAT_RAW_DATA_FILE_POS(U16 index)
{
	//^3E74:0472
	U16 di = index;
	Bit32u bp04 = _4976_5d6a;
	U16 si = 0;
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
U16 SkWinCore::FILE_SEEK(U16 handle, Bit32u pos) {
	if (fset.fileSeekTo(handle, pos) == pos)
		return 1;
	return 0;
}

U32 SkWinCore::FILE_TELL(U16 handle) {
	return fset.fileTell(handle);
}

//^476D:05E3
void SkWinCore::_476d_05e3(U8 *str) { // TODO: Unr
	Unr();
}

//^0CCD:000A
void __DECLSPEC_NORETURN_ SkWinCore::RAISE_SYSERR(U16 syserr)
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
	static const U8 * const strSystemError = (const U8 *) 
		"\x53\x59\x53\x54\x45\x4D\x20\x45\x52\x52\x4F\x52\x20\x2E\x5A\x30\x30\x30\x00";
	// SYSTEM ERROR .Z000.
	static const U8 * const strSystemErrorF = (const U8 *) 
		"\x53\x59\x53\x54\x45\x4D\x20\x45\x52\x52\x4F\x52\x20\x2E\x5A\x30\x30\x30\x00";
	// SYSTEM ERROR .Z000.
	static const U8 * const strSystemErrorG = (const U8 *) 
		"\x53\x59\x53\x54\x45\x4D\x20\x45\x52\x52\x4F\x52\x20\x2E\x5A\x30\x30\x30\x00";
	
	// 4976:01E6  F2 01 76 49 05 02 76 49 18 02 76 49

	// SPX : _4976_01e6	renamed strSystemErrorTable
	static const U8 *strSystemErrorTable[] = {
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
	U8 bp80[128];
	ATLASSERT(glbLangageLetterIndex < 3U);
	FORMAT_SKSTR(strSystemErrorTable[glbLangageLetterIndex], bp80);

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_RED, BLACK);
	printf("%s : %s\n", reinterpret_cast<const char *>(bp80), getSystemErrorMessage(syserr));

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_YELLOW, BLACK);
	printf("DATA FOLDER   = %s\n", GET_DATA_FOLDER_NAME());
	skWinApp->LogParams();


#if UseAltic
	skWinApp->skwin_ShowMessage(reinterpret_cast<const char *>(bp80));
	_exit(1);
#else
	if (_4976_4750 != 0) {
		if (_4976_4ba4 != 0) {
			PRINT_SYSERR_TEXT(0, 16, 15, 0, bp80);
		}
		else {
			_476d_05e3(bp80);
		}
		if (_4976_474a != 0) {
			SPECIAL_UI_KEY_TRANSFORMATION();
		}
		else {
			SLEEP_SEVERAL_TIME(0x012C);
		}
		SK_PREPARE_EXIT();
	}
#endif
}

//^3E74:0EA2
void SkWinCore::GRAPHICS_DATA_READ(U32 location, U32 size, X8 *buff)
{
	//^3E74:0EA2
	U16 bp14[2] = {(U16)glbFileHandleGraphics1};
	U32 bp08[2] = {location, 0};
	i32 bp10[2] = {(i32)size, 0};
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
	for (U16 si=0; si < 2; si++) {
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
void SkWinCore::LOAD_GDAT_RAW_DATA(U16 index, shelf_memory ps)
{
//LOGX((" + LOAD_GDAT_RAW_DATA loads GDAT item %04d to ptr %08x", index, &ps ));
	//^3E74:0FF8
	U16 di = index;
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
	U16 si = bp0c / TEMP_BUFF_SIZE;
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
shelf_memory SkWinCore::CONVERT_PHYS_TO_SHELF_FORM(U8 *buff)
{
	U32ptr p1 = buff - xCRAM;
	//if (p1 < sizeof(cram)) {
	if (p1 < skWinApp->iSizeCRAM) {
		shelf_memory ps;
		ps.val = U32(p1);
		return ps;
	}
	U32ptr p2 = buff - xCEMS;
	if (p2 < shelf_memory::SizeOf_cems()) {	// replaced sizeof(cems)
		shelf_memory ps;
		ps.val = 0x00200000U +U32(p2);
		return ps;
	}
	ATLASSERT(false);
	throw int();

//		//^3E74:0213
//		U8 *bp04 = buff;
//
//		DOSIMPL(); // TODO: WRONG conversion. Fix me
//
//#pragma warning( push )
//#pragma warning( disable : 4311 4312 )
//		return (U8 *)((((Bit32u)bp04 & 0xffff0000) >> 12) + ((U16)bp04 & 0xffff));
//#pragma warning( pop )
}

//^3E74:15A9
U8 *SkWinCore::QUERY_GDAT_DYN_BUFF(U16 iDataIndex, U16 *yy, U16 allocUpper)
{
	U16 iLocalDataIndex = iDataIndex;	// si
	U16 iAllocFlags = allocUpper;	// di
	if (_4976_5d76 == 0) {
		i32 bp08;
		X8* xData = ALLOC_MEMORY_RAM(	// bp04
			bp08 = (QUERY_GDAT_RAW_DATA_LENGTH(iLocalDataIndex) +6),
			(iAllocFlags != 0) ? afDefault : afUseLower,
			1024
			) + 6;
//LOGX(("LOAD_GDAT_RAW_DATA call from QUERY_GDAT_DYN_BUFF (1)"));
		LOAD_GDAT_RAW_DATA(
			iLocalDataIndex,
			CONVERT_PHYS_TO_SHELF_FORM(xData)
			);
		SkD((DLV_GLD, "GLD: Dyn2-load Raw#%4d at RAM(%p)\n", (Bitu)iLocalDataIndex, xData));
		WRITE_UI16(xData,-6 +4,(iAllocFlags != 0) ? afDefault : afUseLower);
		WRITE_UI32(xData,-6 +0,bp08);
		return xData;
	}
	else {
		//^3E74:165B
		X8* xData; // bp04
		if (ADD_CACHE_HASH(iLocalDataIndex + 0xFFFE0000, yy) != 0) {
			//^3E74:1679
			xData = reinterpret_cast<U8 *>(QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(*yy));
			//^3E74:168B
			if (iAllocFlags == 0)
				return xData;
			//^3E74:168F
			_3e74_583a_MEMENTI(*yy);
		}
		else {
			//^3E74:169D
			xData = ALLOC_CPXHEAP_MEM(
				*yy,
				QUERY_GDAT_RAW_DATA_LENGTH(iLocalDataIndex)
				);
			//^3E74:16BA
//LOGX(("LOAD_GDAT_RAW_DATA call from QUERY_GDAT_DYN_BUFF (2)"));
			LOAD_GDAT_RAW_DATA(
				iLocalDataIndex,
				CONVERT_PHYS_TO_SHELF_FORM(xData)
				);
			SkD((DLV_GLD, "GLD: Dyn3-load Raw#%4d at RAM(%p)\n", (Bitu)iLocalDataIndex, xData));
			//^3E74:16D0
			if (iAllocFlags == 0) {
				//^3E74:16D4
				_3e74_585a_CACHE(*yy, 0);
			}
		}
		return xData;
	}
}

//^3E74:521A
U8 *SkWinCore::QUERY_GDAT_IMAGE_LOCALPAL(U8 cls1, U8 cls2, U8 cls4)
{
	//^3E74:521A
	U16 si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, dtImage, cls4);
	//^3E74:5236
	if (si != 0xffff) {
		//^3E74:523B
		IMG3 *bp04;
		if (glbShelfMemoryTable[si].Absent() != 0) {
			//^3E74:5258
			if (IS_CLS1_CRITICAL_FOR_LOAD(cls1) == 0)
				goto _5276;
			//^3E74:5265
			U16 bp06;
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
		return reinterpret_cast<U8 *>(bp04);
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
U8 *SkWinCore::EXTRACT_GDAT_IMAGE(U16 index, i16 allocUpper)
{
	SkD((DLV_DBG_GETPIC,"DBG: EXTRACT_GDAT_IMAGE(%4u,%u)\n", (Bitu)index, (Bitu)allocUpper));

	if (SkCodeParam::bUseFixedMode && index >= 65535)
		return NULL;

	//^3E74:4B48
	U16 di = index;
	i16 bp16 = -1;
	//^3E74:4B56
	if (_4976_5d76 != 0) {
		//^3E74:4B5D
		U16 si = QUERY_MEMENTI_FROM(di);
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
				bp04 = _3e74_48c9_MEMENT(si);
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
				return (U8 *)&bp04[1]; // +14 bytes
			}
			//^3E74:4BEB
			bp04 = bp04->pv0;
			//^3E74:4BF5
		}
	}
	//^3E74:4C0C
	U16 bp1e = 0;
	U8 *bp1a;
	U16 bp1c;
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
			U8 *bp04 = _3e74_0245(di, allocUpper);
			//^3E74:4FB4
			return bp04;
		}
		else {
			//^3E74:4CD0
			//^3E74:4CC7
			U8 *bp04 = PTR_PADD(bp08,+10);
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
	U16 si;
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
	U16 bp20;
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
			_3e74_585a_CACHE(bp16, 0);
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
		DEALLOC_BIGPOOL_STRUCT_BEFORE(reinterpret_cast<U8 *>(bp08));
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
U8* SkWinCore::QUERY_GDAT_IMAGE_ENTRY_BUFF(U8 cls1, U8 cls2, U8 cls4)
{
	SkD((DLV_DBG_GETPIC,"DBG: QUERY_GDAT_IMAGE_ENTRY_BUFF(%02X,%02X,%02X)\n", (Bitu)cls1, (Bitu)cls2, (Bitu)cls4));
	DUMP_5CA4();
	// ATLASSERT(!(cls1 == 1 && cls2 == 2 && cls4 == 0));
	// ATLASSERT(!(cls1 == 7 && cls2 == 0 && cls4 == 0));
	// ATLASSERT(!(cls1 == 10 && cls2 == 0x37 && cls4 == 2));

	//^3E74:4FBE
	U16 iCriticalLoad = 0;
	U8* xImageBuffer = NULL;

	if (glbGameTick != _4976_5d2a) {
		_3e74_44ad();
	}
	RawEntry* pEntry = QUERY_GDAT_ENTRYPTR(cls1, cls2, 1, cls4);
	U16 iGDATItemID;
	if (pEntry == NULL) {
		iGDATItemID = 0xFFFF;
	}
	else {
		iCriticalLoad |= IS_CLS1_CRITICAL_FOR_LOAD(cls1);
		iGDATItemID = pEntry->data & 0x7FFF;
	}
	if (iGDATItemID != 0xFFFF) {
		if (glbShelfMemoryTable[iGDATItemID].Present() || (iCriticalLoad != 0)) {
			xImageBuffer = EXTRACT_GDAT_IMAGE(iGDATItemID, 0);
			return xImageBuffer;
		}
	}
	SkD((DLV_BUGHERE,"BUG? Image (%02X,%02X,%02X) not found. We just supply a \":P\" icon\n"
		, (Bitu)cls1, (Bitu)cls2, (Bitu)cls4));
	
	// SPX: the default image (yukman) is located as default image from MISC ITEM category
	// If that default image is not here, it is very likely to crash thereafter (anytime the default is required)

	iGDATItemID = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_MISCELLANEOUS, GDAT_ITEM_DEFAULT_INDEX, 0x01, 0xFE); // (0x15, 0xfe, 0x01, 0xfe) // the Yukman :P icon
	xImageBuffer = EXTRACT_GDAT_IMAGE(iGDATItemID, 0);
	return xImageBuffer;
}


//^2E62:0004
U8 *SkWinCore::QUERY_GDAT_SQUAD_ICON(U8 *dstImage, U8 colorno, U8 localpal[16])
{
	//^2E62:0004
	U16 si = (((glbChampionSquad[colorno].playerDir() + 4 - cd.pi.glbPlayerDir) & 3) + ((glbGlobalSpellEffects.Invisibility != 0) ? 4 : 0)) * _4976_0118;
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


//^443C:04B6
void SkWinCore::CHANGE_PLAYER_POS(U16 squadPos)
{
	//^443C:04B6
	U16 di = 1;
	//^443C:04BF
	if ((squadPos & 0x8000) != 0) {
		//^443C:04C6
		di = 0;
		squadPos &= 3;
	}
	//^443C:04CC
	i16 si = GET_PLAYER_AT_POSITION((squadPos + cd.pi.glbPlayerDir) & 3);
	//^443C:04DF
	if (_4976_5dbc == 0) {
		//^443C:04E9
		if (si < 0)
			return;
		//^443C:04F0
		_4976_495c.cx = _4976_0118;
		_4976_495c.cy = _4976_011a;
		//^443C:04FC
		U8 *bp08 = ALLOC_PICT_BUFF(_4976_0118, _4976_011a, afDefault, 4);
		U8 *bp0c = ALLOC_PICT_BUFF(_4976_0118, _4976_011a, afDefault, 8);
		//^443C:0530
		U8 bp1c[16];
		QUERY_GDAT_SQUAD_ICON(bp08, (U8)si, bp1c);
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
		_01b0_0c70_MOUSE(3) CALL_IBMIO;
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
		U16 bp02 = _4976_5dbc -1;
		_4976_5dbc = 0;
		//^443C:05FB
		U16 bp04 = GET_PLAYER_AT_POSITION((bp02 + cd.pi.glbPlayerDir) & 0x0003);
		//^443C:060C
		if (di != 0) {
			//^443C:0610
			glbChampionSquad[bp04].playerDir((U8)cd.pi.glbPlayerDir);
		}
		//^443C:061F
		if (bp02 != squadPos) {
			//^443C:0627
			if (si >= 0) {
				//^443C:062B
				glbChampionSquad[si].playerPos((U8)bp02);
			}
			//^443C:0642
			glbChampionSquad[bp04].playerPos((U8)(squadPos + cd.pi.glbPlayerDir));
		}
	}
	//^443C:065A
	_443c_040e_MOUSE();
}



//^38C8:0002
void SkWinCore::_38c8_0002()
{
	//^38C8:0002
	//^38C8:0005
	if (_4976_5bec == 0 && cd.gg.glbGameHasEnded == 0) {
		//^38C8:0013
		_4976_5bec = 1;
		if (_4976_5dbc != 0) {
			CHAMPION_SQUAD_RECOMPUTE_POSITION();
			UPDATE_RIGHT_PANEL(0);
		}
		CLEAR_TRY_PUSH_PULL_OBJECT();
		FIRE_HIDE_MOUSE_CURSOR();
		CHOOSE_HIGHLIGHT_ARROW_PANEL();
		if (glbChampionInventory == 0) {
			FIRE_FILL_HALFTONE_RECTI(0x09, 0);
		}
		FIRE_FILL_HALFTONE_RECTI(0x0B, 0);
		FIRE_SHOW_MOUSE_CURSOR();
	}
	return;
}

//^2636:02F8
U8 *SkWinCore::QUERY_GDAT_TEXT(U8 cls1, U8 cls2, U8 cls4, U8 *buff)
{
	//^2636:02F8
	*buff = 0;
	U16 di = 0;
	U16 si = 0;
	//^2636:030B
	U8 *bp0e = QUERY_GDAT_ENTRY_DATA_BUFF(cls1, cls2, dtText, cls4);
	//^2636:0327
	if (bp0e != NULL) {
		//^2636:0348
		U8 *bp0a = ALLOC_MEMORY_RAM(
			di = si = QUERY_GDAT_ENTRY_DATA_LENGTH(cls1, cls2, dtText, cls4),
			0,
			1024
			);
		//^2636:0358
		COPY_MEMORY(bp0e, bp0a, si);
		//^2636:0377
		if (glbTextEntryEncoded != 0) {
			//^2636:037E
			U8 *bp04 = bp0a;
			U8 bp05 = 0;
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
U8 SkWinCore::_0aaf_0067(U8 cls2)
{
	//^0AAF:0067
	U16 di = 0;
	U16 si = 0xffff;
	U16 bp08 = 0;
	//^0AAF:0077
	U16 bp38[21];
	ZERO_MEMORY(&bp38[1], 2*20);
	//^0AAF:0088
	U8 bp5e[38];
	for (U8 bp0d = 0; bp0d < 0x14; bp0d++) {
		//^0AAF:008F
		if (*QUERY_GDAT_TEXT(0x1a, cls2, bp0d, (U8 *)bp5e) != 0) {
			//^0AAF:00B0
			U16 bp0a = QUERY_GDAT_ENTRY_DATA_INDEX(0x1a, cls2, dtWordValue, bp0d);
			//^0AAF:00C7
			U8 *bp04 = (U8 *)&bp38[1 + di];
			*bp04 = (U8)bp0a;
			//^0AAF:00E2
			if (*bp04 == 0) {
				//^0AAF:00E6
				*bp04 = bp0d;
			}
			//^0AAF:00EF
			bp04[1] = (U8)(bp0a >> 8);
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
	U16 bp06;
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
			if (_476d_04ed_DOES_NOTHING(si) != 0) {
				//^0AAF:016C
				_1031_0781(bp08 + 0x00db);
			}
		}
		if (_4976_4dfc == 0x00FF && IS_THERE_KEY_INPUT_2() != 0 && SPECIAL_UI_KEY_TRANSFORMATION() == 0x001C) {
			_1031_0781(0x00DB);
		}
		//^0AAF:019D
	} while (_4976_4dfc == 0x00FF);

	//^0AAF:01A5
	U8 bp0c = (U8)bp38[_4976_4dfc];
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
U16 SkWinCore::_2066_03e0(U16 xx)
{
	//^2066:03E0
	U16 di = xx;
	U8 bp01 = 0;
	//^2066:03ED
	if (_4976_499e != 0) {
		//^2066:03F4
		return 1;
	}
	//^2066:03F9
	U16 si = 1;
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
U8 *SkWinCore::FORMAT_SKSTR(const U8 *format, U8 *output)
{
	//^2636:000C
	U8 *bp04;
	U8 *bp08;
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
	U8 bp11;
	while ((bp11 = *format) != 0) {
		//^2636:0062
		U8 bp0116[128];
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
				U16 di = 0;
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
			U8 bp0096[128];
			U8 *bp0c = bp0096;
			U8 bp12 = 1;
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
						U8 bp13 = 0xFE;
						U8 bp14 = 0x00;
						//^2636:0161
						// SPX: would be 01/FE/00 = SKULLKEEP ?
						QUERY_GDAT_TEXT(bp12, bp13, bp14, bp0c);
						break;
					}
				case 0x0003:	// .Z003
					{
						//^2636:01AF
						//^2636:0244
						const U8 *bp0c = _4976_498e;
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
						const U8 *bp0c = _4976_4992;
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
						bp0c = (U8 *)&glbChampionSquad[glbChampionBonesIndex];
						break;
					}
				case 0x0008:	// .Z008
					{
#if defined (__LINUX__) || defined (__MINGW__)	// for Linux and MinGW we don't want to suffix by C:\ (or C:) and fail on the relative filepath
						continue;
#else
						const U8 *bp0c = ptrDirLetter;
						FORMAT_SKSTR(bp0c, bp0116);
						SK_STRCAT(bp08, bp0116);
#if defined (__DJGPP__) || defined(__MINGW__)
						SK_STRCAT(bp08, (const U8 *) "\\");
#endif
						bp04 = bp08 +SK_STRLEN(bp08);
						continue;
#endif
					}
				case 0x0009:	// .Z009	Directory letter
					{
						//^2636:01CC
						//^2636:0244
						const U8 *bp0c = ptrDirLetter2;
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
							const U8 *bp0c = _4976_498e;
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
							const U8 *bp0c = _4976_4992;
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
						U8 bp13 = 0;
						U8 bp14 = U8(_4976_52e4);
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
						const U8 *bp0c = strZxxxTable[3];	// ".Z008DATA\"
												
						// SPX: the game folder (which is DATA by default) is changed from dungeon menu (need a restart)
						// Standard ".Z008DATA"
						bp0c = GET_DATA_FOLDER_ZNAME();
						/*
						switch(skwin.dung)
						{
							case _OPTION_DUNGEON_NO_SPECIFIC_:
							default:
								bp0c = (const X8*) ".Z008DATA\\"; break;
						} // END of specific block
						*/
						// SPX: If a specific data folder name is given (within SkWin directory), it will be used instead
						if (skWinApp->sCustomDataFolder != NULL)
						{
							static char sFolderString[256];
#ifdef __LINUX__
							sprintf(sFolderString, ".Z008%s/", skWinApp->sCustomDataFolder);
#else
							sprintf(sFolderString, ".Z008%s\\", skWinApp->sCustomDataFolder);
#endif
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
							const U8 *bp0c = strZxxxTable[6];
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
							const U8 *bp0c = strZxxxTable[5];
							// SPX: the game folder (which is DATA by default) is changed from dungeon menu (need a restart)
							// Standard ".Z008DATA"
							bp0c = GET_DATA_FOLDER_ZNAME();
							/*
							switch(skwin.dung)
							{
								default:
									bp0c = (const X8*) ".Z008DATA\\"; break;
							} // END of specific block
							*/
							// SPX: If a specific data folder name is given (within SkWin directory), it will be used instead
							if (skWinApp->sCustomDataFolder != NULL)
							{
								static char sFolderString[256];
#ifdef __LINUX__
								sprintf(sFolderString, ".Z008%s/", skWinApp->sCustomDataFolder);
#else
								sprintf(sFolderString, ".Z008%s\\", skWinApp->sCustomDataFolder);
#endif
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
						U8 bp16[] = {glbSKSaveDigitAlpha, 0};
						//^2636:0222
						SK_STRCPY(bp0096, bp16);
						break;
					}
				case 0x0018:	// .Z024 : Dungeon letter (default nothing, or G (german) or F (french)
					{
						//^2636:0235
						//^2636:0244
						ATLASSERT(glbLangageLetterIndex < 8U);
						const U8 *bp0c = strZxxxTable[glbLangageLetterIndex];
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
							const U8 *bp0c = strZxxxTable[7];	// ".Z009DATA\"
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
							const U8 *bp0c = strZxxxTable[3];	// ".Z008DATA\"
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
						const U8 *bp0c = strZxxxTable[7];	// ".Z009DATA\"
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
						U8 bp13 = 0xFE;
						U8 bp14 = 0x06;
						//^2636:0161
						QUERY_GDAT_TEXT(bp12, bp13, bp14, bp0c);
						break;
					}
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

	SkD((DLV_DBG_GAME_LOAD, "DECIDE_DEFAULT_DUNGEON_MAP_CHIP_SET\n"));

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

int SkWinCore::AUTO_DETECT_ENDIANNESS(File_header* xDunHeader)
{
	if (xDunHeader != NULL)
	{
		int iDatabaseCategoryIndex = 0;
		int iHowManyCatAbove = 0;
		for (iDatabaseCategoryIndex = 0; iDatabaseCategoryIndex < 16; iDatabaseCategoryIndex++)
		{
			if (xDunHeader->nRecords[iDatabaseCategoryIndex] > 1024)
				iHowManyCatAbove++;
		}
		if (iHowManyCatAbove > 0)
			return 1;
	}
	return 0;
}


//^2066:25B8
int SkWinCore::READ_DUNGEON_STRUCTURE(X16 isNewGame)
{
	// Visit http://dmweb.free.fr/?q=node/217 for file format

	//^2066:25B8
	U16 bp04 = 0;
	U16 si = 0;
	i16 di;
	U8 bp01 = 0;
	U8 iSwapEndian = 0;	// SPX: added to handle non-PC dungeons in big endian
	U8 iDatabaseCatIndex = 0;

	if (isNewGame == 1 && SkCodeParam::bTQMode)
		return READ_DUNGEON_STRUCTURE_TQ(isNewGame);

#ifdef __SK_EXTENDED_SKWIN_V6__
	int iReadExtendedGame = 0;
	iReadExtendedGame = READ_DUNGEON_STRUCTURE_EXTENDED_GAME(isNewGame, skWinApp->dung);
	if (iReadExtendedGame)	/// should return 1 if extended game was read correctly
		return iReadExtendedGame;
#endif //


	SkD((DLV_DBG_GAME_LOAD, "Read 8 first bytes (Random seed) ...\n"));
	//SPX: Read the first 8 bytes of the dungeon.dat
	U8 bp26[8];
	SkD((DLV_DBG_GAME_LOAD, "Dungeon Read iHandle = %d\n", glbDataFileHandle));
	if (FILE_READ(glbDataFileHandle, 8, bp26) == 0)
		return 0;

	if (*(U16 *)bp26 == 0x8104)	// x8104 is for compressed dungeon data. DM2 is not handling it.
		return 0; // returns with "the game is damaged"

	//^2066:25E4
	SkD((DLV_DBG_GAME_LOAD, "FILE_SEEK pos @ %d\n", _4976_524a));
	//getch();	
	FILE_SEEK(glbDataFileHandle, _4976_524a);
	//^2066:25F8
	if (_4976_3b5d != 0) {
		//^2066:25FF
		dunHeader = reinterpret_cast<File_header *>(ALLOC_MEMORY_RAM(44, 1, 1024));
	}

	// - File header
	SkD((DLV_DBG_GAME_LOAD, "Read Dungeon Global Header ...\n"));
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

	// SPX: at this point, perform an autodetect to check if the dungeon read is in big endian rather than expected little endian (PC)
	iSwapEndian = AUTO_DETECT_ENDIANNESS(dunHeader);
	if (iSwapEndian)
	{
		dunHeader->w0 = SWAPW(dunHeader->w0);
		dunHeader->cbMapData = SWAPW(dunHeader->cbMapData);
		dunHeader->cwTextData = SWAPW(dunHeader->cwTextData);
		dunHeader->w8 = SWAPW(dunHeader->w8);
		dunHeader->cwListSize = SWAPW(dunHeader->cwListSize);

		for (iDatabaseCatIndex = 0; iDatabaseCatIndex < 16; iDatabaseCatIndex++)
			dunHeader->nRecords[iDatabaseCatIndex] = SWAPW(dunHeader->nRecords[iDatabaseCatIndex]);
	}

	//SPX: Add control of DM1 gfx seed
	if (dunHeader->w0 == 0x0063) // DM1 or TQ
		SkCodeParam::bDM1Mode = true;
	else if (dunHeader->w0 == 0x0D00 || dunHeader->w0 == 0x0800) // CSB
		SkCodeParam::bDM1Mode = true;
	if (SkCodeParam::bDM1Mode == true) {
		SkCodeParam::bEnableDoubleStepMove = false;
		SkCodeParam::bGFXFixModeDM1 = true;
		SkCodeParam::bAutoDefaultMaxLight = true;
	}
	// SPX

	//^2066:262F
	si = dunHeader->w8;
	cd.pi.glbPlayerPosX = dunHeader->StartPartyPosX();// si & 0x001F;
	//^2066:263F
	si >>= 5;
	cd.pi.glbPlayerPosY = dunHeader->StartPartyPosY();// si & 0x001F;
	//^2066:264A
	cd.pi.glbPlayerDir = (si >> 5) & 0x0003;
	cd.pi.glbPlayerMap = 0;
	//^2066:265B
	i16 nMaps = dunHeader->nMaps;	// bp10
	SkD((DLV_DBG_GAME_LOAD, "Maps read = %d", nMaps));
	
	//^2066:2664
	// SPX: Alloc map header (x16), capped to 64 maps (64*16 = 1024 = 0x400)
	if (_4976_3b5d != 0) {
		//^2066:266B
		dunMapsHeaders = reinterpret_cast<Map_definitions *>(ALLOC_MEMORY_RAM(nMaps << 4, afUseUpper, 0x400));
	}

	// - Map definitions

	SkD((DLV_DBG_GAME_LOAD, "Read %d Map Headers ...\n", nMaps));
	//^2066:2685
	if (SKLOAD_READ(dunMapsHeaders, nMaps << 4) == 0)
		return 0;
	if (iSwapEndian)
	{
		U8 iMapIndex = 0;
		for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++)
		{
			U16* pMapHeaderData = (U16*) &dunMapsHeaders[iMapIndex];	// (Map_definitions*)
			
			*pMapHeaderData = SWAPW(*pMapHeaderData);	pMapHeaderData++;
			*pMapHeaderData = SWAPW(*pMapHeaderData);	pMapHeaderData++;
			*pMapHeaderData = SWAPW(*pMapHeaderData);	pMapHeaderData+=2;
			*pMapHeaderData = SWAPW(*pMapHeaderData);	pMapHeaderData++;
			*pMapHeaderData = SWAPW(*pMapHeaderData);	pMapHeaderData++;
			*pMapHeaderData = SWAPW(*pMapHeaderData);	pMapHeaderData++;
			*pMapHeaderData = SWAPW(*pMapHeaderData);	pMapHeaderData++;
		}
	}





	//^2066:26A2
	if (_4976_3b5d != 0) {
		//^2066:26A9
		dunMapColumnsSumArray = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(nMaps << 1, afUseUpper, 0x400));
	}

	SkD((DLV_DBG_GAME_LOAD, "Read %d Maps Columns Headers ...\n", nMaps));

	//^2066:26C5
	U16 bp0e = 0;	// total map column
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

		if (SkCodeParam::bDebugTileset)
			dunMapsHeaders[si].w14 = (0x0F << 4) + (dunMapsHeaders[si].w14 & 0xFF00);
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
	SkD((DLV_DBG_GAME_LOAD, "Read Index per Columns ...\n"));
	//^2066:277C
	if (SKLOAD_READ(dunMapTilesObjectIndexPerColumn, bp0e << 1) == 0)
		return 0;
	if (iSwapEndian)
		SWAPWPX(dunMapTilesObjectIndexPerColumn, bp0e);
	// - List of object IDs of first objects on tiles

	//^2066:2798
	if (SKLOAD_READ(dunGroundStacks, si << 1) == 0)
		return 0;
	//^2066:27B3
	if (isNewGame != 0) {
		//^2066:27B9
		for (di = 0; di < 300; si++, di++) {
			//^2066:27BD
			dunGroundStacks[si] = OBJECT_NULL; // 0xFFFF
			//^2066:27CC
		}
	}
	if (iSwapEndian)
		SWAPWPX(dunGroundStacks, si);

	// - Text Data

	//^2066:27D4
	if (SKLOAD_READ(dunTextData, dunHeader->cwTextData << 1) == 0)
		return 0;
	if (iSwapEndian)
		SWAPWPX(dunTextData, dunHeader->cwTextData);

	//^2066:27F5
	if (_4976_5bf2 == 0) {
		//^2066:27FC
		glbTimersMaximumCount = MAX_TIMER_NUMBER;	// = 50
	}

	// - List of XXX
	SkD((DLV_DBG_GAME_LOAD, "Read Objects per Category ...\n"));
	//^2066:2802
	for (si = 0; si < 16; si++) {
		//^2066:2807
		SkD((DLV_DBG_GAME_LOAD, "Category %02d of size %02d => %04d records ...\n", si, glbItemSizePerDB[si], dunHeader->nRecords[si]));
		U16 di = dunHeader->nRecords[si];
		if (isNewGame != 0) {
			//^2066:281B
			dunHeader->nRecords[si] = min_value((si == dbCloud) ? 0x300 : 0x400, tblDefaultNbItemAllocationPerDB[RCJ(16,si)] + di);
		}
		//^2066:2849
		U16 bp0e = glbItemSizePerDB[si];
		if (_4976_3b5d != 0) {
			//^2066:2859
			glbDBObjectData[si] = ALLOC_MEMORY_RAM(dunHeader->nRecords[si] * bp0e, afUseUpper, 0x400);
		}
		//^2066:2887
		U16 *bp04 = (U16 *)glbDBObjectData[si];
		if (SKLOAD_READ(glbDBObjectData[si], bp0e * di) == 0)
			return 0;
		if (iSwapEndian)
			SWAP_OBJECTDATA((U16*)glbDBObjectData[si], si, di);

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
	SkD((DLV_DBG_GAME_LOAD, "Alloc RAM for maps ...\n"));
	if (_4976_3b5d != 0) {
		//^2066:2924
		dunMapData = ALLOC_MEMORY_RAM(dunHeader->cbMapData, afUseUpper, 0x400);
	}

	// - Map data

	SkD((DLV_DBG_GAME_LOAD, "Read Dungeon Map Data ...\n"));

	if (SKLOAD_READ(dunMapData, dunHeader->cbMapData) == 0)
		return 0;

	if (_4976_3b5d != 0) {
		U16 iMapIndex = 0;
		U16 iColCount = 0;
		// SPX: 0x400 = 32*32 tiles / bp10 = #maps / _4976_4cb4 = #cols for all maps
		glbMapTileValue = reinterpret_cast<U8 ***>(ALLOC_MEMORY_RAM((_4976_4cb4 + nMaps) * sizeof(void *), afUseUpper, 0x400));
		//^2066:298A
		U8 ***pTile = &glbMapTileValue[nMaps];	// bp08
		//^2066:299C
		for (iMapIndex = 0; iMapIndex < nMaps; iMapIndex++) {
			// MARK SPX
			glbMapTileValue[iMapIndex] = reinterpret_cast<U8 **>(pTile);
			U8 *pTileFromMap = &dunMapData[dunMapsHeaders[iMapIndex].w0];	// w0 = map offset
			*pTile = reinterpret_cast<U8 **>(pTileFromMap);
			pTile++;
			for (iColCount = 1; dunMapsHeaders[iMapIndex].Column() -1 >= iColCount; iColCount++) {
				pTileFromMap += dunMapsHeaders[iMapIndex].Row();
				*pTile = reinterpret_cast<U8 **>(pTileFromMap);
				pTile++;
			}
		}
	}

	SkD((DLV_DBG_GAME_LOAD, "Read Some Random Value from GDAT ??? ...\n"));
	//^2066:2A37
	_4976_5c24 = BETWEEN_VALUE(0, QUERY_GDAT_ENTRY_DATA_INDEX(0x03, 0x00, 0x0B, 0x00), 23) * 0x0555UL;
	//^2066:2A6A

	if (_4976_3b5d != 0) {
		//^2066:2A71
		INIT_TIMERS();
	}
	//^2066:2A76

	_3df7_0037(!_4976_3b5d);
	//^2066:2A85
	if (_4976_3b5d != 0) {
		//^2066:2A8C
		//!ALT

		SkD((DLV_DBG_GAME_LOAD, "ALLOC_MEMORY_RAM %d %d %08x\n", MAXDEPTH, afUseUpper, 0x400));
		_4976_4cb0 = ALLOC_MEMORY_RAM(MAXDEPTH, afUseUpper, 0x400);
		SkD((DLV_DBG_GAME_LOAD, "Memret = %08x\n", _4976_4cb0));
		//^2066:2AA4
		//!ALT

		SkD((DLV_DBG_GAME_LOAD, "ALLOC_MEMORY_RAM %d %d %08x\n", MAXDEPTH + MAXMAPS, afUseUpper, 0x400));
		_4976_4c72 = ALLOC_MEMORY_RAM(MAXDEPTH + MAXMAPS, afUseUpper, 0x400);
		SkD((DLV_DBG_GAME_LOAD, "Memret = %08x\n", _4976_4c72));
		// MAXDEPTH = 63
		// MAXMAPS = 64
	}
	
	//^2066:2ABC
	X8 *bp18 = _4976_4c72;
	X8 *bp1c = _4976_4cb0;
	U16 bp14 = 0;
	*bp1c = 0;
	bp1c++;
	SkD((DLV_DBG_GAME_LOAD, "Arrange Depth of Maps ... MAXDEPTH=%d, maps %d\n", MAXDEPTH, nMaps));
	//getch();
	//^2066:2AE4
	for (UINT bp12 = 0; bp12 < MAXDEPTH; bp1c++, bp12++) {
		for (UINT bp1e = 0; bp1e < nMaps; bp1e++) {
			if (dunMapsHeaders[bp1e].Level() == bp12) {
				*bp18 = (U8)bp1e;
				bp18++;
				bp14++;
			}
		}
		*bp18 = 0xff;
		bp18++;
		bp14++;
		*bp1c = (U8)bp14;
	}
	SkD((DLV_DBG_GAME_LOAD, "Assert 1 : %08x - %08x <= %d + %d => [%d]\n", bp1c, _4976_4cb0, MAXDEPTH, 1U, (bp1c - _4976_4cb0)));
	SkD((DLV_DBG_GAME_LOAD, "Assert 2 : %08x - %08x <= %d + %d + %d => [%d]\n", bp18, _4976_4c72, MAXDEPTH, MAXMAPS, 1U, (bp18 - _4976_4c72)));
	ATLASSERT((bp1c - _4976_4cb0) <= (MAXDEPTH + 1U));
	ATLASSERT((bp18 - _4976_4c72) <= (MAXDEPTH + MAXMAPS + 1U));

	// SPX: Special -> remove creatures from dungeon
	/*if (SkCodeParam::bDebugNoCreatures) {
		UINT iCreatureIndex = 0;
		U16 iCreatureRecords = dunHeader->nRecords[DB_CATEGORY_CREATURE];
		Creature* xCreature = NULL;
		xCreature = (Creature*) glbDBObjectData[DB_CATEGORY_CREATURE];
		// header = 0
		for (iCreatureIndex = 0; iCreatureIndex < iCreatureRecords; iCreatureIndex++) {
			xCreature->w0 = 0xFFFF;	// no creature
			xCreature->hp1 = 0;
			xCreature->iID = 0;
			xCreature++;
		}
		//dunHeader->nRecords[DB_CATEGORY_CREATURE] = 0;
		// should go through all maps to remove creature references
	}*/


	SkD((DLV_DBG_GAME_LOAD, "After ASSERTS. isNewGame = %d\n", isNewGame));
	if (isNewGame != 0) {
		ARRANGE_DUNGEON();
	}
	if (_4976_3b5d != 0) {
		DECIDE_DEFAULT_DUNGEON_MAP_CHIP_SET();
	}
	_4976_3b5d = 0;

	//CHECK_TILE_RECORDS();
	SkD((DLV_DBG_GAME_LOAD, "Read Dungeon Structure Completed !\n"));
	if (SkCodeParam::bDebugInfoMapInit == true)
		LOG_FULL_DUNGEON_INFO();

	LUA_CALL_SCRIPT(_EXP_SCRIPT__GAME_INIT_, 0, 0, 0, 0);

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
int SkWinCore::SUPPRESS_READER(void *_data, const void *_mask, U16 buffSize, Bit32u repeat, U16 fill)
{
	U8 *data = reinterpret_cast<U8 *>(_data);
	const U8 *mask = reinterpret_cast<const U8 *>(_mask);

	//^2066:01C3
	Bit32u bp0a = 0;
	U8 bp04 = _4976_5254;
	U8 bp05 = _4976_524e;
	//^2066:01E8
	for (Bit32u bp0e = 0; bp0e < repeat; ) {
		//^2066:01EB
		for (U16 si = 0; si < buffSize; si++) {
			//^2066:01F0
			U8 bp03 = (fill != 0) ? 0 : (*data);
			//^2066:0205
			U8 bp02 = *mask;
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
							U8 bp0f;
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
int SkWinCore::READ_1BIT(U16 *pw)
{
	//^2066:030D
	U8 bp01;
	//^2066:0312
	U16 si = SUPPRESS_READER(&bp01, glbByte01, 1, 1, 1);
	*pw = bp01;
	return si;
}


//^2066:15AA
int SkWinCore::READ_RECORD_CHECKCODE(i16 xpos, i16 ypos, ObjectID *recordLinkPtr, U16 readDir, U16 readSub)
{
	// readDir=(read-direction-in-record-link)
	// readSub=(read-subsequent-records)

	//printf("READ_RECORD_CHECKCODE @ (%02d : %02d/%02d)\n", glbCurrentMapIndex, xpos, ypos);

	//^2066:15AA
	while (true) {
		//^2066:15B0
		U16 bp0e;
		if (READ_1BIT(&bp0e) != 0) // read#more-record
			return 1;
		//^2066:15C2
		if (bp0e == 0)
			break;
		//^2066:15CB
		U8 bp0c = 0x0f;
		U8 bp0b;
		//^2066:15CF
		if (SUPPRESS_READER(&bp0b, &bp0c, 1, 1, 1) != 0) // read#dbtype
			return 1;
		//^2066:15EF
		U16 di = bp0b;
		U16 bp0a;
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
			U16 bp10 = 0x7f;
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
			U16 bp12 = 0;
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
							U16 bp14;
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
						U16 bp10 = 0x03ff;
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
U16 SkWinCore::READ_SKSAVE_TIMER_3C_3D()
{
	//^2066:197C
	ENTER(4);
	//^2066:1981
	Timer *bp04 = glbTimersTable;
	//^2066:198E
	for (U16 si = 0; si < glbTimersCount; bp04++, si++) {
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
	U16 iReadSaveResult = 0;	// si (at end part)
	SkD((DLV_DBG_GAME_LOAD, "RESUME/LOAD SAVEGAME ---------------------------------\n"));

	SkD((DLV_DBG_GAME_LOAD, "Reset champions inventory ... \n"));
	U16 iChampionIndex;	// si
	for (iChampionIndex = 0; iChampionIndex < cd.pi.glbChampionsCount; iChampionIndex++) {
		ObjectID* xInvObject = &glbChampionSquad[iChampionIndex].inventory[0];	// bp08
		for (U16 iInvSlot = 0; iInvSlot < 30; iInvSlot++) {
			*xInvObject = OBJECT_END_MARKER; xInvObject++;
			//*xInvObject = OBJECT_NULL; xInvObject++;	// SPX: changed default to NULL else Yuks are drawn if the rest of savegame is not properly read (i.e. DM1)
		}
	}

	//cd.pi.glbLeaderHandPossession.object = OBJECT_END_MARKER;
	cd.pi.glbLeaderHandPossession.object = OBJECT_NULL;	// SPX: Same here


	U16 bp18 = glbCurrentMapIndex;
	//^2066:1A2A
	SkD((DLV_DBG_GAME_LOAD, "Delete records ... \n"));
	U16 bp16;
	for (bp16=0; dunHeader->nMaps > bp16; bp16++) {
		//^2066:1A32
		CHANGE_CURRENT_MAP_TO(bp16);
		//^2066:1A3B
		for (U16 bp0e=0; bp0e < glbCurrentMapWidth; bp0e++) {
			//^2066:1A42
			for (U16 bp10=0; bp10 < glbCurrentMapHeight; bp10++) {
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
    CHANGE_CURRENT_MAP_TO(bp18);

	SkD((DLV_DBG_GAME_LOAD, "Delete objects ... \n"));
	U16 iCategoryDB = 0;
	for (iCategoryDB = dbCreature; iCategoryDB < dbMax; iCategoryDB++) {
		U16* pObjectData = reinterpret_cast<U16 *>(glbDBObjectData[iCategoryDB]); // bp08
		U16 iSizeStep = glbItemSizePerDB[iCategoryDB] >> 1;	// bp1a; / 2 because we are using U16 here
		for (U16 iRecordNo = dunHeader->nRecords[iCategoryDB]; iRecordNo-- != 0; pObjectData += iSizeStep) {	// di
#if (DM2_EXTENDED_DATABASE == 1)
			if (iCategoryDB == db11 || iCategoryDB == db12 || iCategoryDB == db13)
				continue;
#endif
			reinterpret_cast<GenericRecord *>(pObjectData)->w0 = OBJECT_NULL;
		}
	}

	//^2066:1B27
	SkD((DLV_DBG_GAME_LOAD, "Realloc Minions Table ... \n"));
	glbMinionsObjectIDTable = reinterpret_cast<ObjectID *>(ALLOC_MEMORY_RAM(200, afDefault, 1024));
	glbMinionsAssocCount = 0;

	if (!SkCodeParam::bDM1Mode)
	
	for (iChampionIndex = 0; iChampionIndex < cd.pi.glbChampionsCount; iChampionIndex++) {
		ObjectID* xInvObject = &glbChampionSquad[iChampionIndex].inventory[0]; // bp08
		for (U16 iInvSlot = 0; iInvSlot < 30; iInvSlot++) {	// di
			if (READ_RECORD_CHECKCODE(-1, 0, xInvObject++, 0, 0) != 0) {
				goto _1e7e;
			}
		}
	}

	if (READ_RECORD_CHECKCODE(-1, 0, &cd.pi.glbLeaderHandPossession.object, 0, 0) != 0) {
		goto _1e7e;
	}
	//^2066:1BA6
	for (iChampionIndex = 0; iChampionIndex < cd.pi.glbChampionsCount; iChampionIndex++) {
		ObjectID* xInvObject = &glbChampionSquad[iChampionIndex].inventory[0];	// bp08
		for (U16 iInvSlot = 0; iInvSlot < 30; iInvSlot++, xInvObject++) {	// di
			if (*xInvObject == OBJECT_END_MARKER) {
				*xInvObject = OBJECT_NULL;
			}
			else {
				PROCESS_ITEM_BONUS(iChampionIndex, *xInvObject, iInvSlot, 0);
			}
		}
	}

	if (cd.pi.glbLeaderHandPossession.object == OBJECT_END_MARKER) {
		cd.pi.glbLeaderHandPossession.object = OBJECT_NULL;
	}
	else {
		PROCESS_ITEM_BONUS(glbChampionLeader, cd.pi.glbLeaderHandPossession.object, -1, 0);
	}
	//^2066:1C14
	if (READ_SKSAVE_TIMER_3C_3D() != 0) {
		//^2066:1C1C
		goto _1e7e;
	}
	//^2066:1C1F
	SkD((DLV_DBG_GAME_LOAD, "Loop through all maps ... \n"));
	bp18 = glbCurrentMapIndex;
	for (bp16 = 0; dunHeader->nMaps > bp16; bp16++) {
		//^2066:1C2D
		CHANGE_CURRENT_MAP_TO(bp16);
		U8 *bp04 = *glbCurrentTileMap;
		for (U16 bp0e = 0; bp0e < glbCurrentMapWidth; bp0e++) {
			//^2066:1C4F
			for (U16 bp10 = 0; bp10 < glbCurrentMapHeight; bp10++) {
				//^2066:1C57
				U8 bp13 = *bp04;
				U16 bp24 = 0;
				U8 bp1b;
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
							if (GET_TELEPORTER_DETAIL(&bp2a, (U8)bp0e, (U8)bp10) != 0) {
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
						U16 bp22 = bp12.DBType();
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
									U16 bp1e;
									U16 bp20 = 0x01FF;
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
	CHANGE_CURRENT_MAP_TO(bp18);
	if (RECOVER_MINION_ASSOC() == 0) {
		iReadSaveResult = 0;
	}
	else {
_1e7e:
		iReadSaveResult = 1;
	}
	if (SkCodeParam::bForceSaveGameReadOK == true)
		iReadSaveResult = 0; // all good ?

	DEALLOC_UPPER_MEMORY(200);
	return iReadSaveResult;
}

UINT SkWinCore::IS_ZERO_ARRAY(X8* xDataBuffer, UINT iDataLength)
{
    const U32* pBuffer32 = (const U32*) xDataBuffer;
    UINT iBlocks = iDataLength / sizeof(U32);

    for (UINT i = 0; i < iBlocks; i++) {
        if (pBuffer32[i] != 0) {
            return 0;
        }
    }
	return 1;
}

//^2066:2D9C
i16 SkWinCore::GAME_LOAD()
{
	SkD((SkCodeParam::bEngineNoDisplay||DLV_DBG_GAME_LOAD, "GAME_LOAD:RESUME/LOAD SAVEGAME ---------------------------------\n"));
	//^2066:2D9C
	U16 bp04 = 0;
	U16 si = 0;
	U8 bp01 = 0;
	U16 bp08;
	U16 bp06;
	//^2066:2DAD
	if (cd.mo.glbSpecialScreen != _MENU_SCREEN__RESUME_GAME_SELECT) {
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
			U16 di = _0aaf_0067(_0aaf_02f8_DIALOG_BOX(15, bp01));
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
			if (SkCodeParam::bOptionResumeSaveGame >= 0)
				bp06 = SkCodeParam::bOptionResumeSaveGame;
			else
				bp06 = SELECT_LOAD_GAME();
			//^2066:2E7A
			if (bp06 < 0) {
				//^2066:2E7E
				glbSKSaveNum = 0;
				//^2066:2E84
				//^2066:32B2
				U16 di = -1;
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
							cd.mo.glbSpecialScreen = _MENU_SCREEN__LOAD_NEW_GAME;	// new game
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
	cd.pi.glbChampionsCount = 0;
	//SPX: changed 0xFFFF to oFFFF
	cd.pi.glbLeaderHandPossession.object = OBJECT_NULL; // 0xFFFF
	//^2066:2F8C
	sksave_header_asc bp6a;
	FILE_READ(glbDataFileHandle, 42, &bp6a);
	//^2066:2FA1
	_4976_5244 = bp6a.wTimerFlag;
	//^2066:2FA7
	_4976_524a = 42;
	//^2066:2FB3
	cd.mo.glbSpecialScreen = !_4976_5bf2;


	// SPX: a regular DM2 savegame will have wTimerFlag = 1. If not, we are probably not reading a native DM2 savegame.
	// There is a chance this is a DM1 savegame, and we will try it.

	if (bp6a.wTimerFlag == 1) // native DM2 savegame
	{
		//^2066:2FBE
		do {
			SkD((DLV_DBG_GAME_LOAD, "Read Dungeon Structure (Native DM2) ...\n"));
			
			if (READ_DUNGEON_STRUCTURE(0) == 0)
			{
				printf("BREAK\n");
				break;
			}
			//^2066:2FCC
			cd.mo.glbSpecialScreen = _MENU_SCREEN__RESUME_GAME_SELECT;
			//^2066:2FD2
			SUPPRESS_INIT();

			SkD((DLV_DBG_GAME_LOAD, "Start Read File Handle %02d ...\n", glbDataFileHandle));
			//s_testSKSave.StartRead(FILE_TELL(glbDataFileHandle));

			SkD((DLV_DBG_GAME_LOAD, "Read Global Variables ...\n"));
			//^2066:2FD6
			skload_table_60 t1;
			if (SUPPRESS_READER(&t1, _4976_395a, 56, 1, 1) != 0)
				break;
			//^2066:2FF5
			glbGameTick = _4976_4c1a = t1.dwGameTick;	// game tick
			glbRandomSeed = t1.dwRandomSeed;	// random seed
			cd.pi.glbChampionsCount = t1.wChampionsCount;	// player cnt
			cd.pi.glbPlayerPosX = t1.wPlayerPosX;	// column
			cd.pi.glbPlayerPosY = t1.wPlayerPosY;	// row
			cd.pi.glbPlayerDir = t1.wPlayerDir;	// dir
			cd.pi.glbPlayerMap = t1.wPlayerMap;	// map#
			glbChampionLeader = t1.wChampionLeader;	// leader
			glbTimersCount = t1.wTimersCount;	// timer cnt
			_4976_0090 = t1.dw22;
			glbGameTick_4b80 = t1.dw26;
			_4976_4c00 = t1.w30;
			glbPlayerThrowCounter = t1.wPlayerThrowCounter;
			_4976_4c0c = t1.w34;
			glbRainFlagSomething = (U8)t1.wRainFlagSomething;
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
			SkD((DLV_DBG_GAME_LOAD, "Read Game Variables (Flags / Bytes / Words) ...\n"));
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
			if (SUPPRESS_READER(glbChampionSquad, _4976_3992, 261, cd.pi.glbChampionsCount, 1) != 0) // player ents
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
			SkD((DLV_DBG_GAME_LOAD, "Read Timers ...\n"));
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
			if (cd.mo.glbSpecialScreen != _MENU_SCREEN__RESUME_GAME_SELECT) {
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

			//^2066:322C
			_0aaf_02f8_DIALOG_BOX(0x000E, 0x0000);
			//^2066:3237
			_4976_4bd8 = 0x0001;
			cd.pi.glbPlayerDefeated = 0x0000;
			//^2066:3243
			_2fcf_0b8b(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerMap);
			//^2066:3257
			glbDoLightCheck = 0x0001;
			//^2066:325D
			//^2066:32B5
			return 1; // U16 di = 1; return di;
		} while (false);
	}
	else	// try read as DM1 savegame
	{
		do {
			U8 iChampionIndex = 0;
			U8 glbDummyData[10000];
			// there are 0x24C0 bytes of encoded data
			SkD((DLV_DBG_GAME_LOAD, "Read as a DM1 savegame ...\n"));
			// 42 bytes already been read
			if (FILE_READ(glbDataFileHandle, 0x24C0-42, glbDummyData) == 0)
				break;
		
			// 0x24C0 (9408 bytes) of data, what to put into it ? 
			// An Amiga save looks to have 0x2924 = 10532 bytes
			// CSBWin tells there are 1 block of 512, then a 2nd block of data,
			// then block of items, heros, timers, timers queued
			// some global variables and that may be all ? ...
			// CSB hero would be around 284 without portraits and fillers
			// 284 * 4 = 1136
			// +50 fill * 4 = +200
			// timers should be around 10 or 12, maybe having 50 at first ? so 50 * 10 = 500, not much

			// then comes portraits : 4 * 29*16 bytes
			SkD((DLV_DBG_GAME_LOAD, "Read Portraits ...\n"));
			// copy portrait data at +6 because we need some meta-info before
			if (FILE_READ(glbDataFileHandle, (29*16), glbDummyData) == 0)
				break;
			memcpy(cd.dm1.xDM1PortraitsData[0]+6, glbDummyData, 29*16);

			if (FILE_READ(glbDataFileHandle, (29*16), glbDummyData) == 0)
				break;
			memcpy(cd.dm1.xDM1PortraitsData[1]+6, glbDummyData, 29*16);
			if (FILE_READ(glbDataFileHandle, (29*16), glbDummyData) == 0)
				break;
			memcpy(cd.dm1.xDM1PortraitsData[2]+6, glbDummyData, 29*16);
			if (FILE_READ(glbDataFileHandle, (29*16), glbDummyData) == 0)
				break;
			memcpy(cd.dm1.xDM1PortraitsData[3]+6, glbDummyData, 29*16);
			//if (FILE_READ(glbDataFileHandle, 8, bp26) == 0)

			// adjust image data with meta-info
			for (iChampionIndex = 0; iChampionIndex < 4; iChampionIndex++)
			{
				cd.dm1.bDM1PortraitsActivated[iChampionIndex] = false;
				*((U16*)(&cd.dm1.xDM1PortraitsData[iChampionIndex])) = 4; // 4bpp
				*((U16*)(&cd.dm1.xDM1PortraitsData[iChampionIndex])+1) = 32; // x pitch
				*((U16*)(&cd.dm1.xDM1PortraitsData[iChampionIndex])+2) = 29; // y pitch
			}
			// Note: if there are only X champions (with X < 4), then the X+1 portrait would be a zero array.
			cd.pi.glbChampionsCount = 4;	// player cnt
			cd.dm1.bDM1PortraitsActivated[0] = true;
			for (iChampionIndex = 1; iChampionIndex < 4; iChampionIndex++)
			{
				// test array
				if (IS_ZERO_ARRAY(cd.dm1.xDM1PortraitsData[iChampionIndex]+6, 29*16) != 0) {
					cd.pi.glbChampionsCount = iChampionIndex;
					break;
				}
				cd.dm1.bDM1PortraitsActivated[iChampionIndex] = true;
			}

			_4976_524a = (0x24C0 + (4*29*16)); // file cursor to start dungeon structure

			// Init champions
			for (iChampionIndex = 0; iChampionIndex < 4; iChampionIndex++)
			{
				Champion *xChampion = &glbChampionSquad[iChampionIndex];
				INIT_CHAMPION(xChampion);
			}


			// the regular dungeon structure starts at 0x2C00
			SkD((DLV_DBG_GAME_LOAD, "Read Dungeon Structure DM1 Mode ...\n"));
			//getch();
			SkCodeParam::bDM1ReadSavegame = true;
			if (READ_DUNGEON_STRUCTURE(0) == 0)
			{
				break;
			}
			_4976_5bf2 = 1; // load is complete
			//getch();
			// Copy from DM2 read
			SkD((DLV_DBG_GAME_LOAD, "Suppress Init ...\n"));
			cd.mo.glbSpecialScreen = _MENU_SCREEN__RESUME_GAME_SELECT;
			SUPPRESS_INIT();
			SkD((DLV_DBG_GAME_LOAD, "Start Read File Handle %02d ...\n", glbDataFileHandle));
			SkD((DLV_DBG_GAME_LOAD, "Read Global Variables ...\n"));

			cd.pi.glbLeaderHandPossession.object = OBJECT_NULL;

			glbGameTick = _4976_4c1a = 1;	// game tick
			glbRandomSeed = 63;	// random seed
			//cd.pi.glbChampionsCount = 4;	// player cnt
			cd.pi.glbPlayerPosX = 10;	// column
			cd.pi.glbPlayerPosY = 4;	// row
			cd.pi.glbPlayerDir = 0;	// dir
			cd.pi.glbPlayerMap = 0;	// map#
			glbChampionLeader = 0;	// leader
			glbTimersCount = 0;	// wTimersCount
			_4976_0090 = 0;
			glbGameTick_4b80 = 0;
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
*/
			bp06 = glbTimersCount;

			SkD((DLV_DBG_GAME_LOAD, "Read Timers ...\n"));
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
			if (cd.mo.glbSpecialScreen != _MENU_SCREEN__RESUME_GAME_SELECT) {
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
			cd.pi.glbPlayerDefeated = 0x0000;
			_2fcf_0b8b(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, cd.pi.glbPlayerMap);
			glbDoLightCheck = 0x0001;
			//LOG_FULL_DUNGEON_INFO();
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
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_RED, BLACK);
	SkD((DLV_DBG_INIT, "GAME LOADED.\n"));
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	//LOG_FULL_DUNGEON_INFO();

	//^2066:3272
	_0aaf_0067(_0aaf_02f8_DIALOG_BOX((_4976_5bf2 == 0) ? 0 : ((_4976_5c9c != 0) ? ((_4976_5ca8 != 0) ? 0x13 : 0x14) : (0x07)), 0x001F));
	//^2066:32A6
	if (_4976_5bf2 == 0) {
		//^2066:32AD
		SK_PREPARE_EXIT();
	}
	//^2066:32B2
	U16 di = -1;
	//^2066:32B5
	return di;
}

//^0CEE:3035
U8 SkWinCore::QUERY_CLS2_OF_TEXT_RECORD(ObjectID recordLink)
{
	//^0CEE:3035
	Text *bp04 = GET_ADDRESS_OF_RECORD2(recordLink);
	if (bp04->TextMode() == 1) {
		//^0CEE:3059
		U16 si = bp04->SimpleTextExtUsage();
		//^0CEE:306B
		switch (si) {
			case 0x0000: // 0x00 - Creature transformer	// SPX: I think creature transformer is 0x01 ??
			case 0x0002: // 0x02 - Animate image
			case 0x0005: // 0x05 - ?
			case 0x000D: // 0x0d - 2-state switch
				//^0CEE:3080
				return U8(bp04->OrnateIndex());
		}
	}
	//^0CEE:3091
	return -1;
}

//^0CEE:241B
U8 SkWinCore::QUERY_CLS2_FROM_RECORD(ObjectID recordLink)
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
			return ((U8)recordLink.w) - 0x80;
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
				return (U8)QUERY_CLS2_FROM_RECORD(bp04->castToMissile()->GetMissileObject());
			case dbCloud:		//15
				//^0CEE:24CB
				return bp04->castToCloud()->CloudType();
		}
	}
	//^0CEE:24F5
	return -1;
}



//^0CEE:23DE
U8 SkWinCore::QUERY_CLS1_FROM_RECORD(ObjectID recordLink)
{
	//^0CEE:23DE
	ObjectID di = recordLink;
	//^0CEE:23E6
	if (di != OBJECT_NULL) {
		//^0CEE:23EB
		U16 si = di.DBType();
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
tiamat SkWinCore::_3e74_0756(U8 *xx, i32 size) { // TODO: Unr
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
			ref->cls1base(U8(glbGDatEntries.iTotalClass1));
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
		if (ref->cls1cur() < 0 || ref->cls1cur() > ref->cls1base() || ref->cls1base() > glbGDatEntries.iTotalClass1)
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
		case 0: return &xCRAM[t.val];
		case 1: return &xCEMS[t.val -0x200000];
	}
	ATLASSERT(false);
	return NULL;
}
tiamat SkWinCore::ptr2t(U8 *pb) {
	U32ptr val;
	val = pb - xCRAM;
	//if (val < sizeof(cram)) {
	if (val < skWinApp->iSizeCRAM) {
		tiamat v; v.val = (U32)val; return v;
	}
	val = pb - xCEMS;
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
			if (_4976_4bd8 != 0 && (si & 0x3f) == 0) { // display loading progression bar
				//^3E74:4225
				//DRAW_DIALOGUE_PROGRESS((((si +1) * 500) / glbGDatNumberOfData) + 500); // original, but it start right away half of the bar
				DRAW_DIALOGUE_PROGRESS((((si +1) * 1000) / glbGDatNumberOfData)); // this one is more logical
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

	//^2676:0131
	ENTER(60);
	//^2676:0137
	X16 bp3c = 0;
	_4976_5300 = 0;

	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN\n"));
	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: pCurrentTileMap :%08X\n", glbCurrentTileMap));

	_4976_52fc = reinterpret_cast<SkLoadEnt *>(ALLOC_MEMORY_RAM(sizeof(SkLoadEnt) * 400, 0,  1024));
	U8 *bp14 = ALLOC_MEMORY_RAM(0xfa, afZeroMem, 1024); // creature existance-on-level bytes?
	U8 *bp18 = ALLOC_MEMORY_RAM(0xfa, afZeroMem, 1024); // wall ornate existance-on-level bytes?
	U8 *bp1c = ALLOC_MEMORY_RAM(0xfa, afZeroMem, 1024); // floor ornate existance-on-level bytes?
	//^2676:01A6
	
	MARK_DYN_LOAD(0x01ff02ff); // Mark: Interface - main screen, snd, all
    MARK_DYN_LOAD(0x18ff02ff); // Mark: Teleporter, all, snd, all
	MARK_DYN_LOAD(0x07ff02ff); // Mark: Interface - character sheet, all, snd, all
	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: pCurrentTileMap :%08X\n", glbCurrentTileMap));
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
	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: pCurrentTileMap :%08X\n", glbCurrentTileMap));
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
	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: pCurrentTileMap :%08X\n", glbCurrentTileMap));
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
	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: pCurrentTileMap :%08X\n", glbCurrentTileMap));
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
	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: pCurrentTileMap :%08X\n", glbCurrentTileMap));
	//^2676:039D
	MARK_DYN_LOAD(0x10ffffff); // Mark: Weapons, all, all, all
	_4976_52fc[_4976_5300 -1].MarkContinuous();
	MARK_DYN_LOAD(0x15ffffff); // Mark: Misc, all, all, all
	MARK_DYN_LOAD(0xffff01f9); // Mark: All, all, image, 0xF9(Map chip)
	MARK_DYN_EXCLUDE_RANGE(0x0fff0510, 0x39); // Mark: Creature, all, text, 0x10(My enemies) to 0x39(?)
	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: pCurrentTileMap :%08X\n", glbCurrentTileMap));
	
	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: MARK_DYN done\n"));
	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: pGroundStack :%04X (#%04d) %08X\n", dunGroundStacks[*_4976_4c52], *_4976_4c52, &dunGroundStacks[*_4976_4c52]));

	//^2676:03E1
	_4976_52fc[_4976_5300 -1].MarkIncluded();
	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: pCurrentTileMap :%08X\n", glbCurrentTileMap));
	X8* pTile = *glbCurrentTileMap;	// bp08
	OID_T* pObjRef = &dunGroundStacks[*_4976_4c52]; // U16 *bp10
	i16 iTileX = 0; // bp26
	i16 si; // si
	for (; iTileX < glbCurrentMapWidth; iTileX++) {
		i16 iTileY = 0;	// bp28
		for (; iTileY < glbCurrentMapHeight; iTileY++) {
			SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: TILE (%d,%d) pT(%8X)\n", iTileX, iTileY, *pTile));
			U16 bp2a = *(pTile++); // bp2a
			SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: TILE (%02X) pT(%8X)\n", bp2a, *pTile));
			if ((bp2a & 0x10) != 0) {
				ObjectID xObjID = (ObjectID) *pObjRef; pObjRef++;	// di
				SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: OBJECT (%02X) pObj(%p)\n", *((U16*)(&xObjID)), pObjRef)); // mark
				do {
					Actuator* xActuator = NULL;	// bp0c
					Text* xText = NULL;	// bp20
					switch (xObjID.DBType()) {
						case dbActuator: //^_0468
							if (_4976_5bee[0] != 0)
								break;
							xActuator = GET_ADDRESS_OF_ACTU(xObjID);
							SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: Actuator (%04d) %8p\n", *((U16*)(&xObjID)), xActuator)); // mark
							switch (xActuator->ActuatorType()) {
								case ACTUATOR_TYPE_CREATURE_GENERATOR: // 0x2E: Creature generator
									if (U8(bp2a >> 5) != ttWall)
										break;
									bp14[xActuator->ActuatorData()] = 1;
									break;
								case ACTUATOR_TYPE_CHAMPION_MIRROR: // SPX: Add for DM1 retrocompatibility / 0x7F: Activator, champion mirror
								case ACTUATOR_TYPE_RESURECTOR: // 0x7E: Activator, resuscitation
									MARK_DYN_LOAD((U8(xActuator->ActuatorData()) << 16) + 0x1600ffff); // Mark: Champions, xxx, all, all
									break;
								case ACTUATOR_FLOOR_TYPE__CROSS_SCENE: // 0x27: Cross scene
									if (U8(bp2a >> 5) != ttTeleporter)
										break;
									si = xActuator->ActuatorData() - 1;
									if (si == glbCurrentMapIndex)
										break;
									U8 *bp24 = &glbMapTileValue[si << 2][dunMapsHeaders[si].RawColumn()][dunMapsHeaders[si].RawRow()];
									si = dunMapsHeaders[si].CreaturesTypes() - 1;
									for (; si >= 0; si--) {
										bp14[bp24[si]] |= 2;
									}
									break;
							}
							break;
						case dbText: //^_0599
							xText = GET_ADDRESS_OF_RECORD2(xObjID);
							if (xText->TextMode() != 1)
								break;
							switch (xText->SimpleTextExtUsage()) {
								case 14://^05D5
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
										bp18[xText->OrnateIndex()] = 1;
										break;
									}
									else {
										bp1c[xText->OrnateIndex()] = 1;
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
					xObjID = GET_NEXT_RECORD_LINK(xObjID);
					//^2676:0623
				} while (xObjID != OBJECT_END_MARKER);
			}
			//^2676:062B
		}
		//^2676:063A
	}
	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: Part 3\n"));
	//^2676:0649
	U16 bp02;
	if (_4976_5bee[0] == 0) {
		//^2676:0655
		for (bp02 = 0; bp02 < cd.pi.glbChampionsCount; bp02++) {
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
	if (_4976_5bee[0] != 0 || (_4976_5c8a != 0 && cd.pi.glbChampionsCount != 4)) {
		//^2676:06FD
		// SPX: x16 GDAT2 Champion category
		MARK_DYN_LOAD(0x16ffffff); // Mark: Champions, all, all, all
	}
	//^2676:0708
	if (_4976_5bee[0] != 0 || dunMapLocalHeader->UseTeleporter() != 0) {
		// SPX: x18 GDAT2 Teleporter category
		MARK_DYN_LOAD(0x18ffffff); // Mark: Teleporter, all, all, all
	}
	//^2676:072C
	/// Original code checks for "use door" info from map header. If flag is not properly set, then no door will be displayed.
	glbMapDoorType[0] = (dunMapLocalHeader->UseDoor0() != 0) ? dunMapLocalHeader->DoorType0() : 0xFF;
	glbMapDoorType[1] = (dunMapLocalHeader->UseDoor1() != 0) ? dunMapLocalHeader->DoorType1() : 0xFF;
	// We can always go for displaying the door, then no bad surprise
	if (SkCodeParam::bUseFixedMode) {
		glbMapDoorType[0] = dunMapLocalHeader->DoorType0();
		glbMapDoorType[1] = dunMapLocalHeader->DoorType1();
	}
	
	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: Part 4\n"));
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
	
	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: Part 5\n"));
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
	
	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: Part 6\n"));
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
	
	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: Part 7\n"));
	//^2676:0C8F
	if (_4976_5c8a == 0)
		_482b_060e_AUDIO();
	//^2676:0C9B
	
	LOAD_DYN4(_4976_52fc, _4976_5300);
	
	if (_4976_5c8a == 0)
		LOAD_MISCITEM();
	
	//^2676:0CBB
	glbRainHasThunderImage = U8(QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_ENVIRONMENT, glbMapGraphicsSet, dtImage, GDAT_ENVWTH_THUNDER_1)); // 0x64 thunder
	glbRainHasWetGround = U8(QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_ENVIRONMENT, glbMapGraphicsSet, dtImage, GDAT_ENVWTH_WETGROUND_1)); // 0x6a wet floor
	glbRainHasCloudSky = U8(QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_ENVIRONMENT, glbMapGraphicsSet, dtImage, GDAT_ENVWTH_CLOUDSKY_1)); // 0x67 ceil cloud
	glbRainHasRainFall = U8(QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_ENVIRONMENT, glbMapGraphicsSet, dtImage, GDAT_ENVWTH_RAINFALL_STRAIGHT_1)); // 0x71 vertical direct rain fall
	glbSceneColorKey = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, GDAT_GFXSET_SCENE_COLORKEY); // colorkey
    glbSceneFlags = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, glbMapGraphicsSet, dtWordValue, GDAT_GFXSET_SCENE_FLAGS);
    
	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: Part 8: DEALLOC\n"));
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

	SkD((DLV_DBG_INIT, "LOAD_LOCALLEVEL_DYN: Part 9: Ending\n"));
	//^2676:0DFE
	
	_4976_022c = 1;
	UPDATE_WEATHER(0);
	RECALC_LIGHT_LEVEL();
	//^2676:0E11
	
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
	MOUSE_STATE_443c_08ab(&bp0a, &bp0c, &_4976_5dae.rc4.cy);
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
	bp04 = (tlbPullPushPlayerMoveDirs[RCJ(6,si)] + cd.pi.glbPlayerDir) & 3;
	bp06 = cd.pi.glbPlayerPosX;
	bp08 = cd.pi.glbPlayerPosY;
	bp06 += glbXAxisDelta[bp04];
	bp08 += glbYAxisDelta[bp04];
	if (IS_TILE_BLOCKED(GET_TILE_VALUE(bp06, bp08)) == 0) {
		//^32CB:2861
		bp04 = (tlbPullPushObjectMoveDirs[RCJ(6,si)] + cd.pi.glbPlayerDir) & 3;
		if (IS_CREATURE_MOVABLE_THERE(cd.pi.glbPlayerPosX + glbXAxisDelta[cd.pi.glbPlayerDir], cd.pi.glbPlayerPosY + glbYAxisDelta[cd.pi.glbPlayerDir], bp04, NULL) != 0) {
			//^32CB:28A3
			HIGHLIGHT_ARROW_PANEL(tlbPullPushArrow[RCJ(6,si)], tlbPullPushArrowRectno[RCJ(6,si)], 1);
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
				return cd.pi.glbPlayerDir & 1;
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
				return cd.pi.glbPlayerDir & 1;
			//^32CB:5A86
			return 0;
		default: //^_5a8a
			//^32CB:5A8A
			return si;
	}
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
	sk4d1a *bp04 = &tblViewportClickableRectangles[xx];
	if (tblViewportClickableRectangles[xx].w8 == OBJECT_NULL) {
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
	tblViewportClickableRectangles[xx].b11 = U8(zz);
	tblViewportClickableRectangles[xx].w8 = rl;
	tblViewportClickableRectangles[xx].b10 = U8(yy);
	//^32CB:35BD
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
// SPX: DRAW_ITEMS_WITHIN_OBJECT renamed DRAW_ITEMS_WITHIN_OBJECT
void SkWinCore::DRAW_ITEMS_WITHIN_OBJECT(ObjectID rl, X16 xx, U32 yy, Creature *ref)
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
	//if (bp0c == OBJECT_END_MARKER)
	if (bp0c == OBJECT_END_MARKER || bp0c == OBJECT_NULL) // SPX : there is a bug when a flying fireball is visible on door if OBJECT_NULL is not handled here
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
				X16 bp10 = QUERY_CREATURE_BLIT_RECTI(di, bp1e, cd.pi.glbPlayerDir) |0x8000;
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
		DRAW_ITEMS_WITHIN_OBJECT(_4976_421a, xx, _4976_5a8a, _4976_5a8e);
	}
	_4976_421a = OBJECT_END_MARKER;
	//^32CB:3F0B
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
U16 SkWinCore::_2fcf_16ff(ObjectID rl)
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
	for (; bp0a < cd.pi.glbChampionsCount; bp0a++, champion++) {
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
		di = cd.pi.glbLeaderHandPossession.object;
		goto _1741;
	}
	return 0;
}


//^47EB:0048
U8 *SkWinCore::_47eb_0048(shelf_memory info, U16 physPage0to3)
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
		Creature* xCreature = GET_ADDRESS_OF_RECORD4(si); // bp04
		AIDefinition* xAIDef = QUERY_CREATURE_AI_SPEC_FROM_TYPE(xCreature->CreatureType()); // bp0c
		bp10 = xAIDef->IsStaticObject();
		if (((bp10 != 0) ? (_4976_4ef2 & 8) : (_4976_4ef2 & 0x20)) != 0) {
			//^19F0:20C0
			bp0e = 8;
			si = xCreature->GetPossessionObject();
			bp10 = (bp10 != 0) ? _48ae_01af(xAIDef->wc30, tt) : 0xf;
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
	return 0xFFFF;
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
						if (glbCreatureSomeFirstObjectOnTile == OBJECT_NULL) {
							glbCreatureSomeFirstObjectOnTile = GET_TILE_RECORD_LINK(ss, tt);
						}
						//^19F0:2262
						for (di = glbCreatureSomeFirstObjectOnTile; di != OBJECT_END_MARKER && (si = di.DBType()) <= dbActuator; di = GET_NEXT_RECORD_LINK(di)) {
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
						glbCreatureSomeObjectFromTile = di;
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
				bp02 = ccm1A_PickItem;
				bp04 = _4976_522e[0];
				di = glbCreatureSomeObjectFromTile;
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
					bp02 = ccm17_PlaceMerchandise;
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
							bp02 = ccm08_MeleeAttack;
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
				else if (bp02 == ccm17_PlaceMerchandise) {
					//^19F0:24DF
					di = _19f0_050f();
					si = _48ae_01af(CREATURE_0cee_2df4(di), vv);
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
			else if (bp02 == X16(ccmFF_Idle)) {
				si = (bp04 - glbCurrentThinkingCreatureData->b29 +1) & 3;
				if (bp0a == 0) {
					bp02 = (si < 2) ? ccm2B_PickItem : ccm2C_PickItem;
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
					if ((tblAIStats01[glbCreatureAIStatIndex] & 0x1000) != 0 && glbCurrentThinkingCreatureData->b29 != glbCurrentThinkingCreatureRec->b15_0_1()) {
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
ObjectID *SkWinCore::OVERSEE_RECORD(ObjectID *ref, U8 dir, ObjectID **recordMatched, U16 (SkWinCore::*pfnFinder)(ObjectID *ref, void *pvUser), void *pvUser, U16 alsoCreaturePossessions, U16 alsoContainedObjects)
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
		U16 si =  (xx->GetTick() == yy->GetTick()) ? 1 : 0;
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
		//if ((U16)xx <= (U16)yy) // loose conversion
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
void SkWinCore::_3a15_0486(U16 xx)
{
	//^3A15:0486
	//^3A15:048C
	U16 di = xx;
	_4976_4762 = -1;
	U16 bp06 = glbTimersCount -1;
	//^3A15:049C
	if (bp06 == 0)
		//^3A15:04A0
		return;
	//^3A15:04A3
	U16 bp0a = glbTimerNextEntries[di];
	//^3A15:04B3
	Timer *bp04 = &glbTimersTable[bp0a];
	//^3A15:04C8
	U16 bp08 = 0;
	//^3A15:04CD
	for (; di > 0; ) {
		//^3A15:04CF
		U16 si = (di -1) >> 1;
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
			U16 si = (di << 1) +1;
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
void SkWinCore::INVOKE_MESSAGE(i16 xpos, i16 ypos, U16 dir, U16 actionType, Bit32u tick)
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
	bp0a.Xcoord((U8)xpos);
	bp0a.Ycoord((U8)ypos);
	bp0a.Value2((U8)dir);
	bp0a.ActionType((U8)actionType);
	//^2FCF:15E5
	QUEUE_TIMER(&bp0a);
}



//^2FCF:15F3
void SkWinCore::INVOKE_ACTUATOR(Actuator *ref, U16 actionType, i16 delayPlus)
{
	SkD((DLV_DBM, "DBM: INVOKE_ACTUATOR(%4X,%2d,%d)\n"
		, ref, (Bitu)actionType, (Bitu)delayPlus));
	if (SkCodeParam::bNoTimer == true)
		return;
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
ObjectID *SkWinCore::_1c9a_06bd(ObjectID recordLink, U16 ss, U16 dir)
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
			(U8)dir,
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
	U16 si = _4976_581e;
	//^2FCF:00BE
	U16 di = ref->Rotation();
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
void SkWinCore::ROTATE_CREATURE(ObjectID recordLink, U16 rotationType, U16 rotation)
{
	SkD((DLV_DBM, "DBM: ROTATE_CREATURE(%04X,%d,%d)\n"
		, (Bitu)recordLink.w, (Bitu)rotationType, (Bitu)rotation));

	//^2FCF:0009
	//^2FCF:000F
	Creature *creature = GET_ADDRESS_OF_RECORD4(recordLink);	//*bp08
	//^2FCF:001E
	U16 di = creature->b15_0_1();
	//^2FCF:002A
	U16 si;
	if (rotationType != 0) {
		//^2FCF:0030
		si = rotation;
	}
	else {
		//^2FCF:0035
		si = (di + rotation) & 3;
	}
	//^2FCF:003F
	U16 bp0a = (si - di) & 3;
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
void SkWinCore::ROTATE_SQUAD(U16 dir)
{
	//^2C1D:01DD
	//^2C1D:01E3
	U16 di = dir;
	//^2C1D:01E6
	if (di != cd.pi.glbPlayerDir) {
		//^2C1D:01EC
		i16 dx = di - cd.pi.glbPlayerDir;
		//^2C1D:01F4
		if (dx < 0)
			//^2C1D:01F8
			dx += 4;
		//^2C1D:01FB
		Champion *bp04 = glbChampionSquad;
		//^2C1D:0203
		for (U16 si=0; si < cd.pi.glbChampionsCount; bp04++, si++) {
			//^2C1D:0207
			bp04->playerPos(bp04->playerPos() + (U8)dx);
			//^2C1D:0216
			bp04->playerDir(bp04->playerDir() + (U8)dx);
			//^2C1D:0222
		}
		//^2C1D:022E
		cd.pi.glbPlayerDir = di;
		//^2C1D:0232
		if (_4976_4c08 != 0) {
			//^2C1D:0239
			_4976_4c2c = glbDir_4976_4c10 = (glbDir_4976_4c10 + dx) & 3;
		}
		else {
			//^2C1D:0246
			_4976_4c2c = cd.pi.glbPlayerDir;
		}
	}
	//^2C1D:024C
	return;
}

//^2FCF:0434
// TODO related to teleporter and falling into pits
U16 SkWinCore::_2fcf_0434(ObjectID recordLink, i16 xpos, i16 ypos, i16 xx, i16 yy, U16 zz)
{
	//^2FCF:0434
	//^2FCF:043A
	ObjectID si = recordLink;
	_4976_5822 = 0;
	U16 bp14 = 0; // 1 if telefragged?
	U16 bp12 = 0;
	U16 bp20 = glbCurrentMapIndex; // curmap
	U16 di = glbCurrentMapIndex; // curmap
	//^2FCF:0452
	U16 bp28;
	U16 bp1e;
	U16 bp26;
	U16 bp24; // teleporter scope
	if (si == OBJECT_NULL) {
		//^2FCF:0457
		if (glbTryPushPullObject != 0) {
			glbTryPushPullObject = 0;
			DRAW_ARROW_PANEL();
		}
		//^2FCF:0469
		cd.pi.glbPlayerPosX = xx;
		cd.pi.glbPlayerPosY = yy;
		bp24 = 2;
		//^2FCF:047A
		bp26 = (glbChampionInventory == 0 && cd.pi.glbIsPlayerSleeping == 0) ? 1 : 0;
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
	for (U16 bp1a = 50; bp1a != 0; --bp1a) {
		//^2FCF:0518
		U16 bp16 = glbCurrentTileMap[xx][yy];
		//^2FCF:0532
		U16 bp18 = bp16 >> 5;	
		//^2FCF:053A
		if (bp18 == ttTeleporter) {
			//^2FCF:0542
			if ((bp16 & 0x0008) == 0)	// if teleporter tile is not active, just leave
				//^2FCF:0549
				break;
			//^2FCF:054C
			Teleporter *bp04 = GET_ADDRESS_OF_TILE_RECORD((U8)xx, (U8)yy)->castToTeleporter();
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
				cd.pi.glbPlayerPosX = xx;
				cd.pi.glbPlayerPosY = yy;
				//^2FCF:0610
				if (bp04->Sound() != 0) {
					//^2FCF:0621
					// SPX: TODO More in the spirit, it should call the 0x18 (teleporter) category instead of 0x03 (messages),
					// and use the index of teleporter (is it possible?)
					//if (!SkCodeParam::bUseDM2ExtendedMode)
						QUEUE_NOISE_GEN1(GDAT_CATEGORY_MESSAGES,0x00,SOUND_STD_TELEPORT_MESSAGE,0x61,0x80,cd.pi.glbPlayerPosX,cd.pi.glbPlayerPosY,-1);
					//else
					//	QUEUE_NOISE_GEN1(GDAT_CATEGORY_TELEPORTERS,0x00,SOUND_STD_TELEPORT,0x61,0x80,glbPlayerPosX,glbPlayerPosY,-1);
				}
				//^2FCF:063F
				if (bp20 == di) {
					//^2FCF:0644
					bp26 &= _4976_5bee[0];
				}
				//^2FCF:064C
				ROTATE_SQUAD((bp04->Rotation() + ((bp04->RotationType() != 0) ? 0 : cd.pi.glbPlayerDir)) & 3);
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
			U16 bp2a = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, dunMapLocalHeader->MapGraphicsStyle(), dtWordValue, GDAT_GFXSET_VOID_RANDOM_FALL);
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
				DISPLAY_VIEWPORT(cd.pi.glbPlayerDir, xx, yy);
				//^2FCF:0807
				CHANGE_VIEWPORT_TO_INVENTORY(1);
			}
			//^2FCF:080F
			if (bp2a != 0) {
				//^2FCF:0818
				U16 bp2e = 0;
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
				U16 bp30 = SEARCH_DUNGEON_FOR_SPECIAL_MARKER(SDFSM_CMD_COUNT_MARKERS, 0x0000, bp2e, &xx, &yy);
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
				cd.pi.glbPlayerPosX = xx;
				cd.pi.glbPlayerPosY = yy;
				//^2FCF:0915
				if (cd.pi.glbChampionsCount > 0) {
					//^2FCF:091F
					Champion *bp08;
					if (_4976_5824 != 0) {
						//^2FCF:0926
						bp26 = (_4976_5bee[0] != 0 && glbChampionInventory == 0 && cd.pi.glbIsPlayerSleeping == 0) ? 1 : 0;
						//^2FCF:0945
						bp08 = glbChampionSquad;
						//^2FCF:094D
						for (bp18=0; bp18 < cd.pi.glbChampionsCount; bp08++, bp18++) {
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
						for (bp18 = 0; (bp18 < cd.pi.glbChampionsCount); bp08++, bp18++) {
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
									0xFE,
									cd.pi.glbPlayerPosX,
									cd.pi.glbPlayerPosY,
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
		U16 bp22 = _0cee_06dc_GET_TILE_DIRECTION(xx, yy);
		//^2FCF:0AC8
		xx += glbXAxisDelta[bp22];
		//^2FCF:0AD4
		yy += glbYAxisDelta[bp22];
		//^2FCF:0AE0
		bp22 = (bp22 +2 ) & 3;
		//^2FCF:0AEB
		U16 bp1c = si.Dir();
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
	SOUND_482b_05bf(1);
	//^2FCF:0B84
	return bp28;
}

//^1C9A:03CF
// TODO related creature size ? b35
ObjectID SkWinCore::_1c9a_03cf(i16 *xx, i16 *yy, U16 dir)
{
	//^1C9A:03CF
	//^1C9A:03D5
	i16 bp0c = *xx;
	i16 di = *yy;
	U16 bp10;
	U16 bp12;
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
			U16 bp14;
			U16 bp16;
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
U16 SkWinCore::CALC_VECTOR_DIR(i16 x1, i16 y1, i16 x2, i16 y2)
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
U16 SkWinCore::CALC_SQUARE_DISTANCE(i16 x1, i16 y1, i16 x2, i16 y2)
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
// _075f_06bd renamed PROJECTILE_GET_IMPACT_ATTACK
// TODO: related to missile/item attack strength ?
// F0216_PROJECTILE_GetImpactAttack
U16 SkWinCore::PROJECTILE_GET_IMPACT_ATTACK(Missile *ref, ObjectID recordLink) //#DS=4976?
{
	//^075F:06BD
	//^075F:06C3
	glbPoisonAttackDamage = 0;
	glbProjectileAttackType = C3_ATTACK_BLUNT;
	U16 iKineticEnergy = ref->EnergyRemaining();	// di
	U16 iAttackDamage = 0; // si
	if (recordLink.DBType() != dbCloud) {
		iAttackDamage = QUERY_GDAT_DBSPEC_WORD_VALUE(recordLink, GDAT_ITEM_WEAPON_THROW_STRENGTH);	// 0x09
		if (iAttackDamage != 0) {
			iAttackDamage += (iKineticEnergy >> 1);
			U16 bp02 = ref->b5_4_7() +3;
			iAttackDamage = (bp02 * bp02 * iAttackDamage) >> 7;
			glbProjectileAttackType = C4_ATTACK_SHARP;
			glbPoisonAttackDamage = QUERY_GDAT_DBSPEC_WORD_VALUE(recordLink, GDAT_ITEM_STATS_POISONOUS);	// 0x0D
			if (glbPoisonAttackDamage != 0) {
				if ((RAND() & 0x007f) > iKineticEnergy) {
					glbPoisonAttackDamage -= RAND16((glbPoisonAttackDamage >> 1) + 1);
				}
			}
		}
		iAttackDamage += RAND02();
		iAttackDamage += QUERY_ITEM_WEIGHT(recordLink);
		if ((RAND() & 0x01ff) < ref->EnergyRemaining2()) {
			iAttackDamage <<= 1;
		}
	}
	else {
		if (recordLink == OBJECT_EFFECT_POISON_BLOB) {	// oFF81
			iAttackDamage = (RAND() & 0x000F);
			glbPoisonAttackDamage = iAttackDamage + 10;
			iAttackDamage += (RAND() & 0x001F);
		}
		else {
			if (recordLink >= OBJECT_EFFECT_DISPELL) {	// oFF83
				glbProjectileAttackType = C5_ATTACK_MAGIC;
				if (recordLink == OBJECT_EFFECT_POISON_BOLT) {	// oFF86
					glbPoisonAttackDamage = iKineticEnergy >> 1;	// DM1 has not this divided /2
					return (iKineticEnergy >> 4) + 1;
				}
				return 0;
			}
			glbProjectileAttackType = C1_ATTACK_FIRE;
			iAttackDamage = (RAND() & 0x000F) + (RAND() & 0x000F) + 10;	// 0 to 40
			if (recordLink == OBJECT_EFFECT_LIGHTNING) {	// oFF82
				glbProjectileAttackType = C7_ATTACK_LIGHTNING;
				iAttackDamage <<= 4;	// SPX: weirdly was written as iAttackDamage >>= 4 ! mistake ?!
				iAttackDamage += iKineticEnergy;
			}
		}
	}
	iAttackDamage = ((iAttackDamage + iKineticEnergy) >> 4) + 1;
	iAttackDamage += (RAND16((iAttackDamage >> 1) + 1) + RAND02());
	// SPX: should check V5 to be sure
	iAttackDamage = max_value(iAttackDamage, iAttackDamage - 32 - ((ref->EnergyRemaining2() >> 3) << 1));
	// AL0483_ui_Attack = F0025_MAIN_GetMaximumValue(AL0483_ui_Attack >> 1, AL0483_ui_Attack - (32 - (P0451_ps_Projectile->Attack >> 3)));
	iAttackDamage = min_value(iAttackDamage, iKineticEnergy << 1);
	printf("MISSILE ATTACK [%d] = %d\n", glbProjectileAttackType, iAttackDamage);
	return iAttackDamage;
}

//^1C9A:0958
// TODO: get some info of creature anim ?
U16 SkWinCore::CREATURE_1c9a_0958(ObjectID recordLink)
{
	Creature *xCreature = GET_ADDRESS_OF_RECORD(recordLink)->castToCreature();	//*bp04
	sk1c9a02c3* xInfoData = GET_CREATURE_INFO_DATA(xCreature, QUERY_CREATURE_AI_SPEC_FROM_TYPE(xCreature->CreatureType())); // bp08
	CreatureAnimationFrame* xAnim = CREATURE_4937_0036(xInfoData->iAnimSeq, &xInfoData->iAnimInfo);	// bp0c
	return (xAnim->w0 & 0x4000) >> 14;
}

//^0CD5:0118
// SPX: Random over 1 bits => range 0 to 1
U16 SkWinCore::_RAND01()
{
	// CSBwinSimilarity: TAG001050,STRandomBool

	//^0CD5:0118
	//^0CD5:011B
	glbRandomSeed = glbRandomSeed * 0xbb40e62d + 11;
	//^0CD5:013B
	return (glbRandomSeed >> 8) & 1;
	//^0CD5:0146
}



//^075F:0AF9
// SPX: Something to do when an item (missile) hit an obstacle
U16 SkWinCore::MISSILE_HIT_075f_0af9(i16 u16tileType, i16 xpos, i16 ypos, U16 dir, ObjectID rlMissile)
{
	//^075F:0AF9
	//^075F:0AFF
	U16 bp1c = 0;
	U16 bp2a = 0;
	//^075F:0B09
	Missile* bp08 = GET_ADDRESS_OF_RECORDE(rlMissile); // bp08
	//^075F:0B18
	ObjectID si = bp08->GetMissileObject();
	//^075F:0B21
	U16 bp2c = si.DBType();
	//^075F:0B2A
	U16 bp28;
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
				bp20 = ObjectID(BETWEEN_VALUE((i16)0xFF80, (i16)bp20.w, (i16)0xFFFF));
				bp28 = bp04->PotionPower();
				bp1e = si;
			}
			else
				bp2a = 0;	// won't explode
		}
// SPX: End of standard/custom block
	}
	//^075F:0B73
//		U16 bp26 = (bp2c == dbCloud && si != oFF81 && si != oFF86) ? 1 : 0;
	U16 bp26 = (bp2c == dbCloud && si != OBJECT_EFFECT_POISON_BLOB && si != OBJECT_EFFECT_POISON_BOLT) ? 1 : 0;
	//^075F:0B8D
	ObjectID *bp10 = NULL;
	U16 bp36 = 0;
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
	U16 bp16;
	switch (tileType) {
		case  ttWall:	// WALL
			{
				//^075F:0BF5
				U16 di = glbTimersTable[bp08->TimerIndex()].Direction();
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
				Door *door = GET_ADDRESS_OF_TILE_RECORD((U8)xpos, (U8)ypos)->castToDoor();	//*bp1a
				//^075F:0C79
				if (bp12 != 5 && (si == OBJECT_EFFECT_ZO_SPELL || si == OBJECT_EFFECT_ZO_2)) {	// (si == oFF84 || si == oFF8D
					//^075F:0C89
					if (door->Button() == 0 && door->DoorBit13C() == 0) {
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
				if (GET_DOOR_STAT_10(GET_GRAPHICS_FOR_DOOR(door)) != 0) {
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
				U16 bp16 = PROJECTILE_GET_IMPACT_ATTACK(bp08, si) +1;
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
				bp16 = PROJECTILE_GET_IMPACT_ATTACK(bp08, si);
				//^075F:0DAE
				if (bp16 != 0) {
					//^075F:0DB2
					if ((((glbChampionSquad[bp12].playerDir() == (glbTimersTable[bp08->TimerIndex()].Direction() +2)) ? 1 : 0) & 3) != 0) {
						//^075F:0DF2
						glbProjectileAttackType |= 0x8000;
					}
					//^075F:0DF8
					if (WOUND_PLAYER(bp12, bp16, 0, glbProjectileAttackType) != 0 && glbPoisonAttackDamage != 0 && RAND01() != 0) {
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
				if (CREATURE_1c9a_0958(bp2e) != 0) {
					//^075F:0E79
					if (tileType == ttTeleporter)
						//^075F:0E7F
						break;
					//^075F:0E82
					//^075F:0D42
					return 0;
				}
				//^075F:0E85
				Creature* xCreature = GET_ADDRESS_OF_RECORD(bp2e)->castToCreature(); // bp04
				//^075F:0E94
				AIDefinition* xAIDef = QUERY_CREATURE_AI_SPEC_FROM_TYPE(xCreature->CreatureType()); // bp0c
				if (xAIDef->w0_5_5() != 0 && si != OBJECT_EFFECT_DISPELL) {	// oFF83
					if (tileType == ttTeleporter)
						break;
					return 0;
				}
				//^075F:0EC3
				if (xAIDef->IsStaticObject() == 0) {
					//^075F:0ECD
					if (xAIDef->Weight != 255) {
						//^075F:0ED4
						goto _0f82;
					}
					else {
						//^075F:0ED7
						goto _0f11;
					}
				}
				//^075F:0ED9
				if ((xAIDef->wc30 & 0x0100) != 0 && si < OBJECT_EFFECT_FIREBALL) {	// oFF80
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
				if (xAIDef->w0_6_7() > 1) {
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
				if ((xAIDef->wc30 & AI_W30_FLAGS__TURN_MISSILE) == 0) {	// 0x0800
					//^075F:0F11
_0f11:
					U16 di = glbTimersTable[bp08->TimerIndex()].Direction();	// get direction
					//^075F:0F2F
					di = (di +2) &3;
					//^075F:0F36
					bp30 += glbXAxisDelta[di];
					//^075F:0F41
					bp32 += glbYAxisDelta[di];
					//^075F:0F4C
					U16 bp34 = rlMissile.Dir();
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
				bp16 = (PROJECTILE_GET_IMPACT_ATTACK(bp08, si) << 6) / xAIDef->ArmorClass;
				//^075F:0FA7
				if (bp16 != 0) {
					//^075F:0FAB
					ATTACK_CREATURE(
						bp2e,
						bp38,
						bp3a,
						0x200d,
						0x0064,
						((xAIDef->w24 & 0x1000) != 0 && glbProjectileAttackType != 1) ? 0 : (bp16 + APPLY_CREATURE_POISON_RESISTANCE(bp2e, glbPoisonAttackDamage))
						);
					//^075F:0FED
					if (bp26 == 0 && xAIDef->AbsorbsMissile() != 0) {	// if not a grey cloud and w0_9_9 = 1
						//^075F:0FFD
						// si is the item thrown
						if ((QUERY_GDAT_DBSPEC_WORD_VALUE(si, 0) & ITEM_FLAG_40) != 0) {	// 0x0040
							//^075F:100C
							bp10 = &xCreature->possession;	// bp04 is the creature
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
	U16 bp14;
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
	U16 di;
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
		_12b4_0d75_CREATURE(
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
	U16 bp04 = 0;
	//^2FCF:023F
	U8 bp10[4];
	ZERO_MEMORY(&bp10, 4);
	//^2FCF:0250
	U16 bp02;
	for (bp02=0; bp02 < 4; bp02++) {
		//^2FCF:0257
		if (GET_PLAYER_AT_POSITION(bp02) >= 0) {
			//^2FCF:0264
			bp10[bp02] = bp02 +1;
		}
		//^2FCF:0274
	}
	//^2FCF:027D
	U8 bp0c[4];
	if (xposTo >= 0 && CALC_SQUARE_DISTANCE(xposFrom, yposFrom, xposTo, yposTo) == 1) {
		//^2FCF:02A2
		bp02 = CALC_VECTOR_DIR(xposFrom, yposFrom, xposTo, yposTo);
		//^2FCF:02B9
		U16 di = (bp02 +1) & 3;
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
						if (MISSILE_HIT_075f_0af9(-3, xposFrom, yposFrom, bp02, si) != 0) {
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
U16 SkWinCore::_RAND16(U16 maxcnt)
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
void SkWinCore::LOAD_LOCALLEVEL_GRAPHICS_TABLE(U16 curmap) //#DS=4976?
{
	//^0CEE:00CD
	//^0CEE:00D3
	CHANGE_CURRENT_MAP_TO(cd.pi.glbPlayerMap = curmap);
	//^0CEE:00DF
	U8 *bp04 = (glbMapCreaturesTypeNumber = &glbCurrentTileMap[glbCurrentMapWidth -1][glbCurrentMapHeight]);
	//^0CEE:0107
	U16 si = dunMapLocalHeader->WallGraphics();
	//^0CEE:0114
	COPY_MEMORY(
		bp04 += dunMapLocalHeader->CreaturesTypes(), 
		_4976_4dc2,
		si
		);
	//^0CEE:0137
	U16 di = dunMapLocalHeader->FloorGraphics();
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
				U16 bp08 = si.Dir();
				//^075F:060A
				if ((QUERY_CREATURE_AI_SPEC_FROM_RECORD(bp06)->wc30 & 15) == 1) {
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

				SkD((DLV_TWEET, "Tweet: %s (x:%d, y:%d, map:%d) have fallen down! \n"
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
ObjectID SkWinCore::RECYCLE_A_RECORD_FROM_THE_WORLD(U16 itemdb, U8 itemtype)
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
	U16 bp1a = glbCurrentMapIndex;
	//^0CEE:0EE0
	U16 di = (_4976_4c08 != 0) ? glbMap_4976_4c12 : 0xffff;
	//^0CEE:0EF1
	U16 bp26 = 0;
	//^0CEE:0EF6
	U16 bp0e;
	U16 bp18;
	bp0e = bp18 = _4976_4c42[itemdb];
	//^0CEE:0F05
	if (bp0e == cd.pi.glbPlayerMap || bp0e == di) {
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
			} while (bp0e == di || bp0e == cd.pi.glbPlayerMap);
		}
		else {
			do {
				//^0CEE:0F4F
				if (dunHeader->nMaps <= (++bp0e)) {
					//^0CEE:0F63
					bp0e = 0;
				}
				//^0CEE:0F68
			} while (bp0e == cd.pi.glbPlayerMap);
			//^0CEE:0F71
			if (bp0e == bp18) {
				//^0CEE:0F76
				bp0e = cd.pi.glbPlayerMap;
			}
		}
	}
	//^0CEE:0F7C
	bp18 = bp0e;
	U16 bp20 = 0;

	while (true) {
		//^0CEE:0F87
        U16 bp14 = dunMapsHeaders[bp0e].RawColumn();
		//^0CEE:0FA0
		U16 bp16 = dunMapsHeaders[bp0e].RawRow();
		//^0CEE:0FB9
		U8 *bp04 = *glbMapTileValue[bp0e];
		//^0CEE:0FD5
		OID_T *bp0c = &dunGroundStacks[dunMapTilesObjectIndexPerColumn[dunMapColumnsSumArray[bp0e]]]; // U16 *bp0c
		//^0CEE:1000
		for (U16 bp10=0; bp10 <= bp14; bp10++) {
			//^0CEE:1008
			for (U16 bp12=0; bp12 <= bp16; bp12++) {
				//^0CEE:1010
				if ((*(bp04++) & 0x10) != 0) {
					//^0CEE:1024
					U8 bp27 = 0xff;
					//^0CEE:1028
					ObjectID si = *(bp0c++);
					//^0CEE:1032
					U16 bp24 = ((bp0e == cd.pi.glbPlayerMap) && (bp10 - cd.pi.glbPlayerPosX + 5 <= 10) && (bp12 - cd.pi.glbPlayerPosY + 5 <= 10)) ? 1 : 0;
					ObjectID bp22;

					// bp10: x-pos of GC(garbage-collector) cursor
					// bp12: y-pos of GC cursor
					// bp24: 1 if player is near from GC cursor. 0 if far

					while (true) {
						//^0CEE:1063
						U16 bp1c = si.DBType();
						//^0CEE:106E
						if (bp1c == dbActuator) {
							//^0CEE:1073
							Actuator *bp08 = GET_ADDRESS_OF_RECORD(si)->castToActuator();
							//^0CEE:107F
							U16 bp1e = bp08->ActuatorType();
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
										U16 bp1e = _bp08->ItemType();
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
										U16 bp1e = _bp08->ItemType();
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
										U16 bp1e = _bp08->PotionType();
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
										U16 bp1e = _bp08->ItemType();
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
							_4976_4c42[itemdb] = (U8)bp0e;
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
		if (bp0e == cd.pi.glbPlayerMap || dunHeader->nMaps <= 1) {
			//^0CEE:1364
			if (bp26 != 0) {
				//^0CEE:136A
				_4976_4c42[itemdb] = (U8)bp0e;
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
			} while (bp0e == di || bp0e == cd.pi.glbPlayerMap);
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
		} while (bp0e == cd.pi.glbPlayerMap);
		//^0CEE:13DE
		if (bp0e != bp18)
			continue;
		//^0CEE:13E6
		bp0e = cd.pi.glbPlayerMap;
	}
}

//^0CEE:1409
ObjectID SkWinCore::ALLOC_NEW_RECORD(U16 db)
{
	SkD((DLV_DBM, "DBM: ALLOC_NEW_RECORD(%2d)\n", (Bitu)db));

	//^0CEE:1409
	U16 si = db;
	ObjectID di = OBJECT_NULL;
	U16 bp08 = dunHeader->nRecords[(si & 0x7fff)];
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
	U16 bp06 = bp08;
	U16 bp0a = glbItemSizePerDB[si];
	U8 *bp04 = glbDBObjectData[si];

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
				bp04 = reinterpret_cast<U8 *>(GET_ADDRESS_OF_RECORD(di));
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



//^01B0:2B1B
// TODO : that one does nothing ?!
X16 SkWinCore::_01b0_2b1b()
{
	ENTER(0);
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
	glbTickStepValue = _01b0_0e80(&SkWinCore::TICK_STEP_CHECK);
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
	SkD((DLV_MEM, "MEM: ADD_MEM_TO_FREE_POOL => Expected Free = %05d, S(sk5d00) = %d\n", bp04->dw4, sizeof(sk5d00)));
	//^3E74:11E7
	_4976_5cb2 = _4976_5ce6 = reinterpret_cast<mement *>(&bp04[+1]);
	_4976_5cf0 = _4976_5cf4 = PTR_PADD(bp04,+bp04->dw4 + sizeof(sk5d00));
	glbFreeRAMMemPool = bp04->dw4;
	_4976_5cee = bp04->w16;
	//^3E74:124C
	bp04 = _3e74_059e(0xc00);
	if (bp04 != NULL || (bp04 = _3e74_059e(0x400)) != NULL) {
		//^3E74:127E
		_4976_5d6e = bp04->w16;
		if ((_4976_5d6e & 0x800) != 0) {
			//^3E74:1290
			_4976_5d5a = tiamat::From_cems(bp04->dw4 + sizeof(sk5d00));
		}
		else {
			//^3E74:12A7
			_4976_5d5a = ptr2t(PTR_PADD(bp04,+bp04->dw4 + sizeof(sk5d00)));
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
//	xCEMS = (X8*) calloc(MAX_CEMS, sizeof(X8));
	xCEMS = (X8*) skWinApp->xSKCEMS;
//#ifdef _SK_DYNAMIC_CEMS
//	printf("CEMS1 = %08X / CEMS2 = %08x\n", skWinApp->xSKCEMS, xCEMS);
//#endif // _SK_DYNAMIC_CEMS

#if UseAltic
	_4976_5ea0 = xCEMS;
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
// SPX: _4726_02f7 renamed INIT_CRAM_EMS_MEM
void SkWinCore::INIT_CRAM_EMS_MEM()
{
	//^4726:02F7
	ENTER(0);
	//^4726:02FA
	if (skWinApp != NULL) {
		glbCRAMSize = skWinApp->iSizeCRAM;
		glbCRAMData = skWinApp->xSKCRAM;
		xCRAM = glbCRAMData;
	}
	else {
//#if UseAltic
		//glbCRAMSize = sizeof(cram);
		//glbCRAMData = cram;
	}
// #else DOS
//	glbCRAMSize = _crt_coreleft();
//	glbCRAMData = _crt_farmalloc(glbCRAMSize);
//#endif
	if (glbCRAMData == NULL) {
		//^4726:0320
		__terminate(0);
	}
	SkD((DLV_MEM, "MEM: INIT_CRAM_EMS_MEM => CRAM = %08x (%08x) SZ = %05d\n", glbCRAMData, xCRAM, glbCRAMSize));
	//^4726:0328
	ATLVERIFY(ADD_MEM_TO_FREE_POOL(reinterpret_cast<sk5d00 *>(glbCRAMData), glbCRAMSize, 0x7f8));
	_4976_5ea6 = EMS_ALLOC_POOL(&_4976_5ea0);
	ATLVERIFY(ADD_MEM_TO_FREE_POOL(reinterpret_cast<sk5d00 *>(_4976_5ea0), _4976_5ea6, 0xc00));
	ATLVERIFY(ADD_MEM_TO_FREE_POOL(NULL, 0, 0));
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
	ENTER(0);
	return (xx << 8) + (xx >> 8);
}

U32 SkWinCore::SWAP32(U32 xx)
{
	return ((xx & 0xFF000000)>>24) + ((xx & 0x00FF0000)>>8) + ((xx & 0x0000FF00)<<8) + ((xx & 0x000000FF)<<24);
}

void SkWinCore::SWAPWPX(U16* xData, U16 iNbItems)
{
	U16 iSwapIndex = 0;
	for (iSwapIndex = 0; iSwapIndex < iNbItems; iSwapIndex++, xData++)
		*xData = SWAPW(*xData);
}

void SkWinCore::SWAP_OBJECTDATA(U16* xData, U8 iCategory, U16 iNbItems)
{
	int iItemIndex = 0;
	int iWordSwap = 0;
	int i16bSize = glbItemSizePerDB[iCategory]/2;
	for (iItemIndex = 0; iItemIndex < iNbItems; iItemIndex++)
	{
		// all data are swapped, except for 2 bytes within the monster structure.
		// to do so, just swap it again afterwards
		for (iWordSwap = 0; iWordSwap < i16bSize; iWordSwap++, xData++)
			*xData = SWAPW(*xData);
		
		if (iCategory == DB_CATEGORY_CREATURE)	// swap back 3rd word
		{
			xData-=6;
			*xData = SWAPW(*xData);
		}
	}
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
		if (bp05 == fmtWordVal || bp05 == fmtPicOff)	// (bp05 == 0xb || bp05 == 0xc)
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


#if DM2_EXTENDED_LOCALIZED_IMAGES == 1
// special table for localized image, actually used for interface
	U8 s_imageLangSel[GDAT_CATEGORY_LIMIT][0xFF][0xFF];
#endif // DM2_EXTENDED_LOCALIZED_IMAGES


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

		if (cls5 == skWinApp->skwin_GetLang()) {
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
#if DM2_EXTENDED_LOCALIZED_IMAGES == 1
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

			if (iLangSelect == skWinApp->skwin_GetLang()) {
				// If corresponding language, always prioritary
					s_imageLangSel[cls1][cls2][cls4] = iLangSelect;
					return 1;
			}
		return 0;
		}
	}
#endif // DM2_EXTENDED_LOCALIZED_IMAGES
	
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
	ref->iTotalClass1 = 0;
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
		if (ref->iTotalClass1 < bp0a) {
			//^3E74:1DD3
			ref->iTotalClass1 = bp0a;
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
	for (bp0a = 0; ref->iTotalClass1 >= bp0a; bp0a++) {
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
	ref->pw0 = reinterpret_cast<X16 *>(ALLOC_MEMORY_RAM((ref->iTotalClass1 +2) << 1, afUseUpper, 0x400));
	ref->pw4 = reinterpret_cast<X16 *>(ALLOC_MEMORY_RAM((ref->w14 +1) << 1, afUseUpper, 0x400));
	ref->pv8 = reinterpret_cast<RawEntry *>(ALLOC_MEMORY_RAM(U32(ref->w16) << 2, afUseUpper, 0x400));
	bp0c = 0;
	si = 0;
	U8 bp09;
	for (bp0a = 0; ref->iTotalClass1 >= bp0a; bp0a++) {
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
	ref->pw0[ref->iTotalClass1 +1] = bp0c;
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
#if DM2_EXTENDED_LOCALIZED_IMAGES == 1
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
	for (iCategoryIndex = 0; iCategoryIndex < GDAT_CATEGORY_MAX; iCategoryIndex++)
	{
		unsigned int iMaxItemsForCategory = 0;
		iMaxItemsForCategory = _3e74_2439_GET_ENTRIES_NUMBER(iCategoryIndex, 0x0B);
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
			U8* xDataPointer = NULL;
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
	ENTER(12);
	glbGDatOpenCloseFlag = 0;
	GRAPHICS_DATA_OPEN();
	TGDATFileHeader tFileHeader;	// bp0c
	if (READ_FILE(glbFileHandleGraphics1, 4, &tFileHeader) == 0)	// Read the first 4 bytes of GDAT, which hold GDAT signature + nb of data items
		goto _read_graphics_structure__raise_error;
	if ((tFileHeader.iSignature & 0x8000) == 0) {	// Default data is supposed to use little-end storage (PC)
		// try now big-end
		if ((SWAPW(tFileHeader.iSignature) & 0x8000) == 0)
			goto _read_graphics_structure__raise_error;
		// at this point, we try big-end
		SkCodeParam::bUseBigEnd = true;
		tFileHeader.iSignature = SWAPW(tFileHeader.iSignature);
		tFileHeader.iNbItems = SWAPW(tFileHeader.iNbItems);
	}
	glbGDATVersion = tFileHeader.iSignature & 0x7FFF;	// GDAT internal format version should be 5
	glbGDatNumberOfData = tFileHeader.iNbItems;

	U16 *bp04;
	X32 bp08;
	if (glbGDATVersion != 4 && glbGDATVersion != 5 && glbGDATVersion != 2)
		goto _read_graphics_structure__raise_error;

	glbShelfMemoryTable = reinterpret_cast<shelf_memory *>(ALLOC_MEMORY_RAM(sizeof(shelf_memory) * U32(glbGDatNumberOfData), afUseUpper, 0x400));
	_4976_5c82 = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(U32(glbGDatNumberOfData) << 1, afUseUpper, 0x400));
	FILL_U16(reinterpret_cast<i16 *>(_4976_5c82), glbGDatNumberOfData, -1, 2);
	bp08 = U32(glbGDatNumberOfData) << 1;	// nb items * 2 = nb of bytes for all item sizes
	bp04 = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(bp08, afDefault, 0x400));
	_4976_5d6a = bp08 +4;	// nb items sizes + (magic number + nb items) = offset of first raw data which must be the ENT1
	if (glbGDATVersion < 3) {
		if (READ_FILE(glbFileHandleGraphics1, bp08, bp04) == 0)
			goto _read_graphics_structure__raise_error;

		_4976_5d7a = *bp04;
		_4976_5d6a += _4976_5d7a;
	}
	else {
		if (READ_FILE(glbFileHandleGraphics1, 4, &_4976_5d7a) == 0)	// Read the next 4 bytes of GDAT which hold the size for the first item entry which must be the ENT1 item
			goto _read_graphics_structure__raise_error;
		_4976_5d6a += _4976_5d7a + 2;	// there _4976_5d6a gets the offset of the second item after ENT1 item
		if (READ_FILE(glbFileHandleGraphics1, bp08 -2, &bp04[1]) == 0)	// here read the size table before the ENT1 item (except the first item already read, which is exceptionnally on 4 bytes; all others are on 2 bytes max)
			goto _read_graphics_structure__raise_error;
	}
	bp04[0] = 0;
	_4976_5caa = _4976_5d6a;
	U16 si;
	for (si = 0; si < glbGDatNumberOfData; bp04++, si++) {
		_4976_5caa += *bp04;
		glbShelfMemoryTable[si] = shelf_memory::FromSizeOnUnloaded(*bp04);
		//printf("Shelf memory %05d: size = %06d\n", si, glbShelfMemoryTable[si].val & 0x7fff);
	}
	_4976_5cea = GET_FILE_SIZE(glbFileHandleGraphics1);
	if (_4976_5cea < _4976_5caa) {
		_4976_5ca8 = 1;
		_4976_5c9c = 1;
	}
	DEALLOC_UPPER_MEMORY(bp08);
	LOAD_ENT1();
	if (glbGDATVersion >= 2 && glbGDATVersion != 4 && QUERY_GDAT_ENTRY_DATA_INDEX(0x0, 0x0, dt08, 0x0) != 0xffff) {
        _4976_5d0c = reinterpret_cast<sk5d0c *>(ALLOC_MEMORY_RAM(
			_4976_5d78 = QUERY_GDAT_ENTRY_DATA_LENGTH(0x0, 0x0, dt08, 0x0),
			afUseUpper, 0x400));
		LOAD_GDAT_ENTRY_DATA_TO(0x0, 0x0, dt08, 0x0, reinterpret_cast<U8 *>(_4976_5d0c));
		_4976_5d78 = _4976_5d78 / 4;
	}

//DEBUG_DISPLAY_GDAT_MAIN_INFO();

	_4976_5cb0 = 1;
	_3e74_24b8();
	return;

_read_graphics_structure__raise_error:
	RAISE_SYSERR(SYSTEM_ERROR__INVALID_GRAPHICS_STRUCTURE);
	return;
}


//^32CB:0008
// SPX: _32cb_0008 renamed LOAD_GDAT_INTERFACE_00_0A
// This item does not exist in PC-DOS version but defines creatures/objects anim frames, then it is a must have.
// If not existing, load preextracted data to compensate
void SkWinCore::LOAD_GDAT_INTERFACE_00_0A()
{
	i32 iItemSize = 0;
	ENTER(4);
	U32 bp04 = 0;

	iItemSize = QUERY_GDAT_ENTRY_DATA_LENGTH(GDAT_CATEGORY_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_BASE_DATA, dt07, 0x0A);
	if (iItemSize <= 0) {
		i16 hAnimFrameTabHandle = -1;
		iItemSize = 1652;
		SkCodeParam::bDM2V5Mode = true;
		// Then, we do a hardfile read to make up for missing item, not pretty but working
		hAnimFrameTabHandle = FILE_OPEN((const U8*)"bin/v4/0653ctbl.bin");
		tblCreatureFrameInfo14 = reinterpret_cast<U8 (*)[14]>(ALLOC_MEMORY_RAM(iItemSize,	afUseUpper, 0x400));
		FILE_READ(hAnimFrameTabHandle, iItemSize, tblCreatureFrameInfo14);
		FILE_CLOSE(hAnimFrameTabHandle);
	}
	else {
	// SPX: This points to a 1652 bytes file .. seems to have struct of 14 bytes => 118 records. creature/objects anim frame size info and such
	tblCreatureFrameInfo14 = reinterpret_cast<U8 (*)[14]>(ALLOC_MEMORY_RAM(bp04 = QUERY_GDAT_ENTRY_DATA_LENGTH(GDAT_CATEGORY_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_BASE_DATA, dt07, 0x0A), afUseUpper, 0x400));
	LOAD_GDAT_ENTRY_DATA_TO(GDAT_CATEGORY_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_BASE_DATA, dt07, 0x0A, reinterpret_cast<U8 *>(tblCreatureFrameInfo14));
	}
	/*
	if (SkCodeParam::bUsePowerDebug) {
		//--- Display hex data of this table
		int iNbItems = 1652 / 14;
		U8* pTabBuffer = (U8*) tblCreatureFrameInfo14;
	
		for (int i = 0; i < iNbItems; i++) {
			printf("%03d)) ", i);
			for (int b = 0; b < 14; b++) {
				printf("%02X ", pTabBuffer[b]);
			}
			printf("\n");
			pTabBuffer += 14;
		}
	}*/

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
	U8 *bp04 = ALLOC_MEMORY_RAM(QUERY_GDAT_ENTRY_DATA_LENGTH(GDAT_CATEGORY_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_BASE_DATA, dt07, 0x2), afUseUpper, 0x400);
	LOAD_GDAT_ENTRY_DATA_TO(GDAT_CATEGORY_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_BASE_DATA, dt07, 0x2, bp04);
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
	// SPX: I changed to get size of item first then check it if zero, because this item does not exist in PC-DOS version
	i32 iItemSize = 0;
	ENTER(8);
	X8* pDataTable = NULL;	// bp04
	bool bSwap32 = false;	// SPX: addition of U32 swap when reading some special gdat

	iItemSize = QUERY_GDAT_ENTRY_DATA_LENGTH(GDAT_CATEGORY_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_BASE_DATA, dt06, 0x0);
	if (iItemSize <= 0) {
		i16 hCreatureTabHandle = -1;
		iItemSize = 5092;
		SkCodeParam::bDM2V5Mode = false;
		tlbCreaturesAnimationSequences = NULL;
		tlbCreaturesActionsGroupSets = NULL;
		tlbCreaturesActionsGroupOffsets = NULL;
		// Then, we do a hardfile read to make up for missing item, not pretty but working
		hCreatureTabHandle = FILE_OPEN((const U8*)"bin/v4/0652ctbl.bin");
		pDataTable = ALLOC_MEMORY_RAM(iItemSize,	afUseUpper, 0x400);
		FILE_READ(hCreatureTabHandle, iItemSize, pDataTable);
		FILE_CLOSE(hCreatureTabHandle);
		SkCodeParam::bDM2V5Mode = true;
	}
	else {
		pDataTable = ALLOC_MEMORY_RAM(
			iItemSize,
			afUseUpper, 0x400);
		LOAD_GDAT_ENTRY_DATA_TO(GDAT_CATEGORY_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_BASE_DATA, dt06, 0x0, pDataTable);
	}

	// First sequence : 16 80 F1 02 0A FF .. 18 00 F1 0e 01 FF .. 1A ..
	X32 iTableSize = READ_UI32(pDataTable,+0);	// b08 Read first DWORD of data, which is length of the first part : 0xC24 = 3108 = 6*518
	if (iTableSize > 0x10000) {	// then it is probably inverted
		iTableSize = SWAP32(iTableSize);
		bSwap32 = true;
	}
	pDataTable += 4;
	tlbCreaturesAnimationSequences = reinterpret_cast<CreatureAnimationFrame *>(pDataTable);	// This points to the first part
	pDataTable += iTableSize;

	iTableSize = READ_UI32(pDataTable,+0);	// @0C28; 0x760 = 1888 = 4*472				@0xC2C + 0x760 = @138C
	if (bSwap32) iTableSize = SWAP32(iTableSize);
	pDataTable += 4;
	tlbCreaturesActionsGroupSets = reinterpret_cast<CreatureCommandAnimation *>(pDataTable);	// This points to the second part
	pDataTable += iTableSize;

	iTableSize = READ_UI32(pDataTable,+0);	// @138C; 0x54 = 84 = 2*42
	if (bSwap32) iTableSize = SWAP32(iTableSize);
	pDataTable += 4;
	tlbCreaturesActionsGroupOffsets = reinterpret_cast<U16 *>(pDataTable);		// This points to the third part

	
	/// DUMP Level 1 tlbCreaturesActionsGroupOffsets (from GDAT value 0F-xx-00-00
	
	FILE* fp = NULL;
	fp = fopen("creature-anims.txt", "wt+");
	int i = 0;
	for (i = 0; i < 42; i++)
	{
		U16 iCAGO = tlbCreaturesActionsGroupOffsets[i];
		fprintf(fp, "CAGO %04X) OFFSET = %04X\n", i, iCAGO);
	}

	/// DUMP Level 2 tlbCreaturesActionsGroupSets / list of commands and start frame for each
	for (i = 0; i < 471; i++)
	{
		CreatureCommandAnimation* cca = &(tlbCreaturesActionsGroupSets[i]);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		if (cca->ccmReference == 0xFFFF)
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_RED, BLACK);
		fprintf(fp, "CMDANIM %04X) CCM = %04X OFF = %04X (CCM: %s)\n", i, cca->ccmReference, cca->animSeqOffset, getCreatureCommandName(cca->ccmReference));
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	}
	/// DUMP Level 3 tlbCreaturesAnimationSequences -- frame/gfx/sfx information
	for (i = 0; i < 518; i++)
	{
		CreatureAnimationFrame* cas = &(tlbCreaturesAnimationSequences[i]);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		if (cas->sound != 0xFF)
			CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
		fprintf(fp, "C-FRAME %04X) %04X %04X %02X SND=%02X (%s)\n", i, cas->w0, cas->w2, cas->b4, cas->sound, getCreatureSoundName(cas->sound));
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	}
	fclose(fp);
	
	

	return;
}
//^38C8:00C8
void SkWinCore::_38c8_00c8_ALLOC_PICT()
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
		bp0a = QUERY_GDAT_PICT_OFFSET(GDAT_CATEGORY_JAPANESE_FONT, cls2, bp0b);
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
	ENTER(0);
	return;
}

//^3929:0E16
void SkWinCore::_3929_0e16_FONT_LOAD()
{
	//^3929:0E16
	ENTER(8);
	//^3929:0E1B
	_3929_07e1(0, 0);
	_4976_5c08 = ALLOC_PICT_BUFF(_4976_013e, _4976_0140, afUseUpper, 8);
	_4976_5c0e = ALLOC_MEMORY_RAM(0x300, afUseUpper, 0x400);
	LOAD_GDAT_ENTRY_DATA_TO(0x1, 0x0, dt07, 0x0, _4976_5c0e);	// default font
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
void SkWinCore::_2405_0009_ALLOC_ITEM_HAND_PICT()
{
	ENTER(0);
	cd.pi.glbLeaderHandPossession.pb2 = ALLOC_PICT_BUFF(glbRectX_0106, glbRectY_0108, afUseUpper, 4);
	return;
}



//^443C:067A
void SkWinCore::_443c_067a_ALLOC_RAM_SRECT(sk0cea *ref)
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
void SkWinCore::_1031_07d6_SOME_INIT()
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
		_443c_067a_ALLOC_RAM_SRECT(&_4976_0ce0[1 +bp02]);
		//^1031:095E
	}
	//^1031:0967
	return;
}
//^3E74:2439
// _3e74_2439 _3e74_2439_GET_ENTRIES_NUMBER
U8 SkWinCore::_3e74_2439_GET_ENTRIES_NUMBER(X8 cls1, X8 cls4)
{

	ENTER(0);
	if (cls1 > U8(glbGDatEntries.iTotalClass1))
		return 0;

	X16 si = glbGDatEntries.pw0[cls1];
	if (glbGDatEntries.pw0[cls1 + 1] - si <= cls4)
		return 0;

	return glbGDatEntries.pv8[U32(glbGDatEntries.pw4[cls4 +si +1]) -1].cls2;
}


//^3E74:0A77
// _3e74_0a77 renamed DEALLOC_BIGPOOL_STRUCT_BEFORE
void SkWinCore::DEALLOC_BIGPOOL_STRUCT_BEFORE(U8 *ref)
{
	ENTER(0);
	///if (_4976_5d76 != 0)
	if (_4976_5d76 != 0 && ref != NULL)
		return;
	if (READ_UI16(ref,-6+4) == 0) {
		DEALLOC_UPPER_MEMORY(READ_UI32(ref,-6)); // how does this work with 64-bits
	}
	else {
		DEALLOC_LOWER_MEMORY(READ_UI32(ref,-6));
	}
	return;
}

//^2481:007D
// SPX: Main loop, display title&menu screen
// _2481_007d renamed SHOW_MENU_SCREEN
void SkWinCore::SHOW_MENU_SCREEN()
{
	//^2481:007D
	ENTER(0);
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_RED, BLACK);
	SkD((SkCodeParam::bEngineNoDisplay||DLV_DBG_INIT, "DISPLAY TITLE MENU SCREEN\n"));
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	REQUEST_PLAY_MUSIC(100, 100);	// SPX: title music

	SkD((DLV_DBG_INIT, "SHOW_MENU_SCREEN\n"));

	//^2481:0080
	cd.mo.glbImageCreditScreen = QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_TITLE, 0x0, 0x1);		// Credit screen (tombstone)
	if (QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_TITLE, 0x0, dt07, 0x4) != 0xFFFF)	// 64000 bytes raw data for menu screen ?
	{
		//^2481:00AA
		_4976_3d2c = 1;
		cd.mo.glbImageMenuScreen = QUERY_GDAT_ENTRY_DATA_PTR(GDAT_CATEGORY_TITLE, 0x0, dt07, 4);
	}
	else {
		//^2481:00C9
		cd.mo.glbImageMenuScreen = QUERY_GDAT_IMAGE_ENTRY_BUFF(GDAT_CATEGORY_TITLE, 0x0, 0x4);
		_4976_52ba = cd.mo.glbImageMenuScreen + (
			(READ_UI16(cd.mo.glbImageMenuScreen,-6) == 4)
				? (((READ_UI16(cd.mo.glbImageMenuScreen,-4) +1) & 0xFFFE) >> 1)
				: (  READ_UI16(cd.mo.glbImageMenuScreen,-4)     & 0xFFFF)
												) * READ_UI16(cd.mo.glbImageMenuScreen,-2);
	}
	// cd.mo.glbImageMenuScreen - 6   would be like 06 FA 00 00 | 02 00 => FA06 = 64006 | 0002 = afUseLower
	// cd.mo.glbImageCreditScreen - 6 would be like 04 00 40 01 | C8 00 => 


	//^2481:0116
	_4976_52b6 = cd.mo.glbImageCreditScreen +((READ_UI16(cd.mo.glbImageCreditScreen,-6) == 4) // 4bits image, meaning 2 pixel per byte
		? (((READ_UI16(cd.mo.glbImageCreditScreen,-4) +1) & 0xFFFE) >> 1)	// (0x140 + 1 & 0xFFFE) = 0x140 >> 1 = 0xA0 (160)
		: (  READ_UI16(cd.mo.glbImageCreditScreen,-4)     & 0xFFFF)
		) * READ_UI16(cd.mo.glbImageCreditScreen,-2)	// 00C8 (200)
		; // 0xA0 * 0xC8 = 0x7D00 (160*200)
	// then _4976_52b6 = p(cd.mo.glbImageCreditScreen) + 0x7D00 => end of this image
	GRAPHICS_DATA_CLOSE();
	_1031_0541(0);
	//^2481:015B
	do {
		//printf("SHOW_MENU_SCREEN Loop ...\n");
		DRAW_TITLE_MENU_SCREEN();
		FIRE_SHOW_MOUSE_CURSOR();
		_1031_098e();
		cd.mo.glbSpecialScreen = _MENU_SCREEN__TITLE_MENU;	// 0x63
		if (SkCodeParam::bOptionNewGame == _OPTION_CLI_TITLE_NEW_GAME)
			cd.mo.glbSpecialScreen = _MENU_SCREEN__LOAD_NEW_GAME;
		else if (SkCodeParam::bOptionNewGame == _OPTION_CLI_TITLE_RESUME_SCREEN)
		{
			cd.mo.glbSpecialScreen = _MENU_SCREEN__RESUME_GAME_SELECT;
			skWinApp->newgame = 0;
		}
		goto _0180;
_0171:
		//SkD((DLV_DBG_INIT, "_1031_0d36 / SPECIAL_UI_KEY_TRANSFORMATION\n"));
		_1031_0d36_KEYBOARD(0x20, SPECIAL_UI_KEY_TRANSFORMATION());
_0180:
		do {
			//SkD((DLV_DBG_INIT, "MessageLoop\n"));
			MessageLoop(true); // main menu

			if (IS_THERE_KEY_INPUT_2() != 0)
				goto _0171;
			MAIN_LOOP();
			//SkD((DLV_DBG_INIT, "Main Screen = %d\n", _MENU_SCREEN__TITLE_MENU));
		} while (cd.mo.glbSpecialScreen == _MENU_SCREEN__TITLE_MENU);	// 0x63
	} while (cd.mo.glbSpecialScreen == _MENU_SCREEN__SHOW_CREDITS); // 0xDA
	// At this point, we hit new game
	// SPX:
	//do {
	//	_0aaf_02f8_DIALOG_BOX(0x0020, 0x0000); // failing
	//} while (cd.mo.glbSpecialScreen == _MENU_SCREEN__RESUME_GAME_SELECT);
	FIRE_HIDE_MOUSE_CURSOR();
	if (_4976_3d2c != 0) {
		DEALLOC_BIGPOOL_STRUCT_BEFORE(cd.mo.glbImageMenuScreen);
	}
	else {
		FREE_PICT_ENTRY(cd.mo.glbImageMenuScreen);
	}
	FREE_PICT_ENTRY(cd.mo.glbImageCreditScreen);
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
void SkWinCore::_38c8_0104() // does nothing
{
	ENTER(0);
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
	ENTER(6);
	X16 bp06 = cd.pi.glbPlayerMap;
	_4976_5c8a = 1;
	i32 bp04 = i32(0x7FFFFFFF);
	U16 iMapIndex = 0;	// i16 si
	i16 di = 0; // i16 di

	SkD((DLV_DBG_INIT, "_38c8_0224 (%d,%d)\n", xx, yy));
	
	for (iMapIndex = 0; dunHeader->nMaps; iMapIndex++) {
		if ((iMapIndex & 3) == 0) {	// every four maps, make loading bar progress
			DRAW_DIALOGUE_PROGRESS((i32(dunHeader->nMaps * (_4976_5be8 -1) + iMapIndex +1) * 500) / i32(dunHeader->nMaps * _4976_5bf4));
		}
		SkD((DLV_DBG_INIT, "LOAD_NEWMAP %d\n", iMapIndex));
		LOAD_NEWMAP(U8(iMapIndex));
		if (_4976_5d62 < 0) {
			//^38C8:02AA
			if (_4976_5d62 < bp04) {
				//^38C8:02BD
				bp04 = _4976_5d62;
				di = iMapIndex;
			}
		}
		//^38C8:02CC
		if (_4976_5d26 -yy < bp04) {
			//^38C8:02E5
			bp04 = _4976_5d26 -yy;
			di = iMapIndex;
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
	SkD((DLV_DBG_INIT, "Ending _38c8_0224\n"));
	_4976_5c8a = 0;
	cd.pi.glbPlayerMap = bp06;
	if (bp04 >= 0)
		return 1;
	return 0;
}

//^3E74:1330
// SPX _3e74_1330 renamed ALLOC_CPX_SETUP
void SkWinCore::ALLOC_CPX_SETUP(X8 *xx)
{
	ENTER(26);

	SkD((DLV_DBG_INIT, "ALLOC_CPX_SETUP (%08x)\n", xx));

	U8 *bp08 = t2ptr(_4976_5d5a);
	X32 bp10 = glbFreeEMSMemPool;
	U8 *bp0c = _4976_5cf4;
	X32 bp14 = glbFreeRAMMemPool;
	mement *bp04 = _4976_5ce6;
	SkD((DLV_DBG_INIT, "ALLOC_CPX_SETUP:_3e74_07b2\n"));
	_3e74_07b2();
	X32 bp18 = 0;
	X16 si = 0;
	do {
		_4976_5d5a = ptr2t(bp08);
		glbFreeEMSMemPool = bp10;
		_4976_5cf4 = bp0c;
		glbFreeRAMMemPool = bp14;
		_4976_5ce6 = bp04;
		SkD((DLV_DBG_INIT, "ALLOC_CPX_SETUP:_3e74_0820\n"));
		_3e74_0820();
		X16 bp1a;
		si = _38c8_0109(&xx, &bp18, &bp1a);
		SkD((DLV_DBG_INIT, "ALLOC_CPX_SETUP:_38c8_0104\n"));
		_38c8_0104();
		if (i32(bp1a +0x100) > 0xffff) {
			glbNumberOfMements = 0xffff;
		}
		else {
			glbNumberOfMements = bp1a + 0x100;
		}
		SkD((DLV_DBG_INIT, "ALLOC_CPX_SETUP:tlbMementsPointers\n"));
		//tlbMementsPointers = reinterpret_cast<mement **>(ALLOC_MEMORY_RAM(glbNumberOfMements << 2, afZeroMem|afUseUpper, 0x400));
		tlbMementsPointers = reinterpret_cast<mement **>(ALLOC_MEMORY_RAM(glbNumberOfMements * sizeof(void*), afZeroMem|afUseUpper, 0x400));	// 32/64-bits adaptation
		_4976_5d24 = 0x80;
		_4976_5d08 = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(_4976_5d24 << 1, afUseUpper, 0x400));
		FILL_U16(reinterpret_cast<i16 *>(_4976_5d08), _4976_5d24, -1, 2);
//		_4976_5c7e_cache_ici = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(_4976_5d24 << 1, afUseUpper, 0x400));
		_4976_5c7e_cache_ici = reinterpret_cast<U16 *>(ALLOC_MEMORY_RAM(_4976_5d24 * sizeof (U16), afUseUpper, 0x400));
//		_4976_5c86_cache_hash = reinterpret_cast<U32 *>(ALLOC_MEMORY_RAM(_4976_5d24 << 2, afZeroMem|afUseUpper, 0x400));
		_4976_5c86_cache_hash = reinterpret_cast<U32 *>(ALLOC_MEMORY_RAM(_4976_5d24 * sizeof (U32), afZeroMem|afUseUpper, 0x400));
		_4976_5ce2 = _4976_5cf4 - _4976_00ec;
		SkD((DLV_DBG_INIT, "ALLOC_CPX_SETUP:INIT_CPXHEAP\n"));
		INIT_CPXHEAP(&_4976_5d12, ptr2t(_4976_5ce2), 0, _4976_5cee);
		if (glbFreeEMSMemPool < 12) {
			//^3E74:14E7
			INIT_CPXHEAP(&_4976_5d7e, ptr2t(NULL), 0, _4976_5d6e);
		}
		else {
			INIT_CPXHEAP(&_4976_5d7e, _4976_5d5a, glbFreeEMSMemPool, _4976_5d6e);
		}
		SkD((DLV_DBG_INIT, "ALLOC_CPX_SETUP:bUsePowerDebug\n"));
#ifndef __DJGPP__		
		if (!SkCodeParam::bUsePowerDebug)	// SPX: I don't get the meaning of this, overriding MEM left value with a low one?
			// then in certain condition it will raise error 40 complaining of no MEM left while there is ...
			glbFreeRAMMemPool = _4976_00ec;
#endif
		_4976_5cf8 = tiamat::Size(ptr2t(_4976_5ce2), ptr2t(reinterpret_cast<U8 *>(_4976_5ce6)));
		//^3E74:153C
		SkD((DLV_DBG_INIT, "ALLOC_CPX_SETUP:_38c8_0224 (!%d,%d)\n", si, bp18));
	} while (_38c8_0224(!si, bp18) == 0);
	_4976_5d76 = 1;
	SkD((DLV_DBG_INIT, "ALLOC_CPX_SETUP:Complete\n"));
	return;
}



//^38C8:04AA
// SPX: _38c8_04aa renamed INIT -- added a return code to handle pre-exit
UINT SkWinCore::INIT()
{
	U16 iCompatibilityFlag = 0;
	U16 iSpecialItem = 0;
	//^38C8:04AA
	SkD((SkCodeParam::bEngineNoDisplay||DLV_DBG_INIT, "INITIALIZATION\n"));
	SkD((DLV_DBG_DOS, "INITIALIZATION\n"));
	
	ENTER(4);
	//^38C8:04AF
	INIT_CRAM_EMS_MEM();
	_4976_474a = 1;
	_4976_474c = 1;
	_476d_018a();
	_2636_03d4();

	SkD((DLV_DBG_INIT, "READ_GRAPHICS_STRUCTURE\n"));
	READ_GRAPHICS_STRUCTURE();
	SkD((DLV_DBG_INIT, "_482b_0004\n"));
	_482b_0004();


	SkD((DLV_DBG_INIT, "LOAD_GDAT_INTERFACE_00_0A\n"));
	LOAD_GDAT_INTERFACE_00_0A(); // game will fail if this item is not loaded, but it does not exist in PC-DOS version
	U8 *bp04 = ALLOC_MEMORY_RAM(0x400, afUseLower, 1024);
//DEBUG_DUMP_ULP();
	LOAD_GDAT_ENTRY_DATA_TO(GDAT_CATEGORY_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_BASE_DATA, dtPalIRGB, 0xFE, bp04);	// C01=I00=EFE=T009 palette IRGB (0x1, 0x0, dt09, 0xFE, bp04)
	// SPX: Beta GDAT contains palette in raw7 data; if dtPalIRGB is not found, we should then look for raw7.
	//LOAD_GDAT_ENTRY_DATA_TO(GDAT_CATEGORY_INTERFACE_GENERAL, 0x0, dtRaw7, 0xFE, bp04); // test for beta
	SET_RGB_PALETTE_FROM_DATA(bp04);
	DEALLOC_LOWER_MEMORY(0x400);
	LOAD_GDAT_INTERFACE_00_02();
	//glbPaletteT16 = QUERY_GDAT_ENTRY_DATA_PTR(0x1, 0x0, dt0d, 0xfe);
	glbPaletteT16 = QUERY_GDAT_ENTRY_DATA_PTR(GDAT_CATEGORY_INTERFACE_GENERAL, GDAT_INTERFACE_SUBCAT_BASE_DATA, dtPalette16, 0xFE);
	_098d_1208();

	SkD((DLV_DBG_INIT, "EXTENDED_LOAD_AI_DEFINITION\n"));
	// SPX: Added extended load here (requires the GDAT to be initialized, but must be before dungeon loading)
	EXTENDED_LOAD_AI_DEFINITION();
	// SPX: Read compatibility mode value (none = standard DM2)
	iCompatibilityFlag = QUERY_GDAT_ENTRY_DATA_INDEX(0, 0, dtWordValue, 0x10);
	if (iCompatibilityFlag == 3) { // DM1 TQ
		SkCodeParam::bDM1Mode = true;
		SkCodeParam::bGFXFixModeDM1 = true;
		SkCodeParam::bDM1TQMode = true;
	}
	// SPX: Check item 00-00-01. If existing, it would be a V5, else a V4
	iSpecialItem = QUERY_GDAT_ENTRY_DATA_INDEX(0, 0, dtWordValue, 0x01);
	//-- Set sound playback base frequency depending on GDAT version
	if (iSpecialItem == 6)
		skWinApp->skwin_SndSetFrequency(11025);
	else
		skWinApp->skwin_SndSetFrequency(6000); // or 5500 ?

	
	SkD((DLV_DBG_INIT, "LOAD_GDAT_INTERFACE_00_00\n"));
	LOAD_GDAT_INTERFACE_00_00();
	_38c8_00c8_ALLOC_PICT();
	_3929_0e16_FONT_LOAD();
	glbTextEntryEncoded = QUERY_GDAT_ENTRY_DATA_INDEX(0x0, 0x0, dtWordValue, 0) & 8;	// Value is 0B ..
	_2405_0009_ALLOC_ITEM_HAND_PICT();
	IBMIO_INIT_CURSORS_MOUSE();
	_4976_4748 = 1;
	_1031_07d6_SOME_INIT();

	cd.gg.glbCreaturesMaxCount = _3e74_2439_GET_ENTRIES_NUMBER(GDAT_CATEGORY_CREATURES, fmtWordVal);	// 0xf, 0xb
	X16 iDefaultCreaturesMaxAllocation = (cd.gg.glbCreaturesMaxCount +1) * 3;	// si
	cd.gg.glbSomeCreatureTable = ALLOC_MEMORY_RAM(iDefaultCreaturesMaxAllocation, afUseUpper, 1024);
	FILL_STR(cd.gg.glbSomeCreatureTable, iDefaultCreaturesMaxAllocation, 0xFF, 1);
	//^38C8:05C1
	SkD((DLV_DBG_INIT, "while (SHOW_MENU_SCREEN()/GAME_LOAD())\n"));
//--- SHOW_MENU_SCREEN LOOP
	while (SHOW_MENU_SCREEN(), GAME_LOAD() != 1) {
		GRAPHICS_DATA_OPEN();
	}
	SkD((DLV_DBG_INIT, "INIT:GRAPHICS_DATA_OPEN\n"));
	GRAPHICS_DATA_OPEN();
	SkD((DLV_DBG_INIT, "INIT:__LOAD_CREATURE_FROM_DUNGEON\n"));
	__LOAD_CREATURE_FROM_DUNGEON();

	SkD((DLV_DBG_INIT, "INIT:ALLOC_CPX_SETUP\n"));
	ALLOC_CPX_SETUP(_4976_4736);
	SkD((DLV_DBG_INIT, "INIT:__INIT_GAME_38c8_03ad\n"));
	__INIT_GAME_38c8_03ad();
	SkD((DLV_DBG_INIT, "INIT:GRAPHICS_DATA_CLOSE\n"));
	GRAPHICS_DATA_CLOSE();

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_RED, BLACK);
	SkD((SkCodeParam::bEngineNoDisplay||DLV_DBG_INIT, "NEW GAME LOADED.\n"));
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	//LOG_FULL_DUNGEON_INFO();

	if (cd.mo.glbSpecialScreen != _MENU_SCREEN__RESUME_GAME_SELECT) {
		//^38C8:05FC
		MOVE_RECORD_TO(OBJECT_NULL, -1, 0, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
	}
	//^38C8:0612
	SkD((DLV_DBG_INIT, "INIT:FIRE_SHOW_MOUSE_CURSOR\n"));
	FIRE_SHOW_MOUSE_CURSOR();
	SkD((DLV_DBG_INIT, "INIT:_1031_098e\n"));
	_1031_098e();
	SkD((SkCodeParam::bEngineNoDisplay||DLV_DBG_INIT, "INIT COMPLETE\n"));

	return 0;
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
		i32 bp0c = _3e74_5673_CACHE(glbCurrentThinkingCreatureID.DBIndex()|0x28000000, &bp0e, 1);
		i32 bp08 = si << 1;
		if (bp0c < bp08) {
			//^14CD:0315
			bp04 = (bp0c != 0)
				? _3e74_5788_CACHE(bp0e, bp08)
				: ALLOC_CPXHEAP_MEM(bp0e, bp08);
			_3e74_585a_CACHE(bp0e, 1);
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
void SkWinCore::_14cd_0f0a_EXE_17(U8 func, U8 xx, U8 yy, sk1bf9 *ss)
{
	ENTER(0);
	func &= 31;
	_4976_4ef6 = 0xffff;
	(this->*_4976_38a4_PFN_17[RCJ(17,func)])(xx, yy, ss);
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
// _14cd_18cc renamed _14cd_18cc_PFN17_00
void SkWinCore::_14cd_18cc_PFN17_00(U8 xx, U8 yy, sk1bf9 *ss)
{
	ENTER(0);
	_14cd_0f3c(0, ss, _4976_1bcf, 0, 0, Ax3::Invalid, yy, xx);
	return;
}




//^14CD:18F2
// _14cd_18f2 renamed _14cd_18f2
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
// _14cd_19a4 renamed _14cd_19a4_PFN17_01
void SkWinCore::_14cd_19a4_PFN17_01(U8 xx, U8 yy, sk1bf9 *ss)
{
	ENTER(0);
	_14cd_18f2(xx, yy, ss, 0, Ax3::Invalid);
	return;
}

//^14CD:102E
// _14cd_102e renamed _14cd_102e
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
// _14cd_10d2 renamed _14cd_10d2
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
// _14cd_1a3c renamed _14cd_1a3c_PFN17_02
void SkWinCore::_14cd_1a3c_PFN17_02(U8 xx, U8 yy, sk1bf9 *ss)
{
	ENTER(0);
	_14cd_19c2(xx, yy, ss, 2, 1);
	return;
}
//^14CD:1A5A
// _14cd_1a5a renamed _14cd_1a5a_PFN17_03
void SkWinCore::_14cd_1a5a_PFN17_03(U8 xx, U8 yy, sk1bf9 *ss)
{
	ENTER(0);
	_14cd_19c2(xx, yy, ss, 4, 3);
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
// _14cd_1b74 renamed _14cd_1b74_PFN17_04
void SkWinCore::_14cd_1b74_PFN17_04(U8 xx, U8 yy, sk1bf9 *ss)
{
	ENTER(0);
	_14cd_1a78(xx, yy, ss, 1);
	return;
}

//^14CD:1B90
// _14cd_1b90 renamed _14cd_1b90_PFN17_05
void SkWinCore::_14cd_1b90_PFN17_05(U8 xx, U8 yy, sk1bf9 *ss) 
{
	ENTER(0);
	_14cd_1a78(xx, yy, ss, 3);
	return;
}

//^14CD:1C27
// _14cd_1c27 renamed _14cd_1c27_PFN17_06
void SkWinCore::_14cd_1c27_PFN17_06(U8 xx, U8 yy, sk1bf9 *ss)
{
	ENTER(0);
	_14cd_1bac(xx, yy, ss, 2, 1);
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
// _14cd_1c45 renamed _14cd_1c45_PFN17_08
void SkWinCore::_14cd_1c45_PFN17_08(U8 xx, U8 yy, sk1bf9 *ss)
{
	ENTER(0);
	_14cd_1bac(xx, yy, ss, 4, 3);
	return;
}
//^14CD:1C63
// _14cd_1c63 renamed _14cd_1c63_PFN17_09
void SkWinCore::_14cd_1c63_PFN17_09(U8 xx, U8 yy, sk1bf9 *ss)
{
	ENTER(0);
	_14cd_18f2(5, yy, ss, 0, (_4976_5151 == 13) ? _4976_5156 : Ax3::Invalid);
	return;
}
//^14CD:1C8D
// _14cd_1c8d renamed _14cd_1c8d_PFN17_09
void SkWinCore::_14cd_1c8d_PFN17_09(U8 xx, U8 yy, sk1bf9 *ss)
{
	ENTER(0);
	if (xx != 0 && glbCurrentThinkingCreatureRec->TriggerX() == glbCreatureTimerGetX && glbCurrentThinkingCreatureRec->TriggerY() == glbCreatureTimerGetY && glbCurrentThinkingCreatureRec->TriggerMap() == glbSomeMap_4976_4ee7)
		return;
	_14cd_18f2(6, yy, ss, 0, Ax3::Invalid);
	return;
}
//^14CD:1CEC
// _14cd_1cec renamed _14cd_1cec_PFN17_10
void SkWinCore::_14cd_1cec_PFN17_10(U8 xx, U8 yy, sk1bf9 *ss)
{
	ENTER(4);
	Missile *xMissile = GET_MISSILE_REF_OF_MINION(glbCurrentThinkingCreatureID, OBJECT_NULL); // bp04
	if (xMissile == NULL || xMissile->GetMissileObject().DBType() != dbContainer)
		return;
	_14cd_18f2(7, yy, ss, 0, GET_ADDRESS_OF_RECORD9(xMissile->GetMissileObject())->GetDest());
	return;
}
//^14CD:1D42
// _14cd_1d42 renamed _14cd_1d42_PFN17_11
void SkWinCore::_14cd_1d42_PFN17_11(U8 xx, U8 yy, sk1bf9 *ss)
{
	ENTER(0);
	_14cd_18f2(0x12, yy, ss, 0, (_4976_5151 == 5) ? _4976_5156 : Ax3::Invalid);
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
// _14cd_1e36 renamed _14cd_1e36_PFN17_12
void SkWinCore::_14cd_1e36_PFN17_12(U8 xx, U8 yy, sk1bf9 *ss)
{
	ENTER(0);
	_14cd_1d6c(xx, yy, ss, 15);
	return;
}

//^14CD:1E52
// _14cd_1e52 renamed _14cd_1e52_PFN17_13
void SkWinCore::_14cd_1e52_PFN17_13(U8 xx, U8 yy, sk1bf9 *ss) { // TODO: Unr
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
// _14cd_1e6e renamed _14cd_1e6e_PFN17_14
void SkWinCore::_14cd_1e6e_PFN17_14(U8 xx, U8 yy, sk1bf9 *ss)
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
				bp0e.w6 = glbCurrentThinkingCreatureRec->iAnimSeq;
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
// _14cd_1fa7 renamed _14cd_1fa7_PFN17_16
void SkWinCore::_14cd_1fa7_PFN17_16(U8 xx, U8 yy, sk1bf9 *ss)
{
	ENTER(2);
	Ax3 bp02;
	bp02.SetX(glbCreaturePosX);
	bp02.SetY(glbCreaturePosY);
	bp02.SetMap(glbCreatureMap);
	_14cd_18f2(0x16, yy, ss, 0, bp02);
	return;
}

//^14CD:1F8B
// _14cd_1f8b renamed _14cd_1f8b_PFN17_15
void SkWinCore::_14cd_1f8b_PFN17_15(U8 xx, U8 yy, sk1bf9 *ss)
{
	ENTER(0);
	_14cd_1eec(xx, yy, ss, 0x15);
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

		_14cd_0f0a_EXE_17(bp04[si].b5(), bp04[si].b6(), bp05, ref->pv2);

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
	i16 di = bp04->w8_0_4();	// di = x
	i16 bp10 = bp04->w8_5_9();	// bp10 = y
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
					if (MISSILE_HIT_075f_0af9(-3, di, bp10, si, bp0e) != 0) {
						return;
					}
				}
			}
		}
		//^075F:1313
		if (MISSILE_HIT_075f_0af9(-1, di, bp10, si, bp14) != 0)
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
		//ATLASSERT((bp12 >> 5) != ttMapExit); // SPX: why not? turn off this assert and see if it's that bad
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
		if (MISSILE_HIT_075f_0af9(bp12 >> 5, bp20, bp22, si, bp0e) != 0)
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
		if (bp08->GetMissileObject() == OBJECT_EFFECT_FIREBALL && glbCurrentMapIndex == cd.pi.glbPlayerMap) {	// oFF80
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
		if ((GET_TILE_VALUE(di, bp10) >> 5) == ttDoor && MISSILE_HIT_075f_0af9(4, di, bp10, bp12, bp14) != 0)
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
	for (; si < cd.pi.glbChampionsCount; si++) {
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
	si = cd.pi.glbChampionIndex;
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

	SkCodeParam::bUseIngameDebug = true;
	if (SkCodeParam::bUseIngameDebug)
	{
		U8 sMessage[128];  memset(sMessage, 0, 128);
		char sExtraInfo[16]; memset(sExtraInfo, 0, 16);
		if (SkCodeParam::bDM2V5Mode)
			strcpy(sExtraInfo, " V5-GFX");
//		sprintf((char*)sMessage, "RAM = %08d / EMS = %08d\n", glbFreeRAMMemPool, glbFreeEMSMemPool);
		sprintf((char*)sMessage, "%s (%s) [%s] <%s>%s\n", __SKWIN_PROGNAME__, strVersionNumber, __SKWIN_RELEASE_DATE__, __SKWIN_SYSTEM__, sExtraInfo);
		DISPLAY_HINT_TEXT(COLOR_YELLOW, sMessage);
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_YELLOW, BLACK);
		SkD((SkCodeParam::bEngineNoDisplay||DLV_DBG_INIT, "%s\n", sMessage));
		CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	}
	//^13AE:005C
	ENTER(0);
	//^13AE:0061
	glbTickSpeed = stdTickBalance;
	cd.gg.glbGameHasEnded = 0;
	FILL_ORPHAN_CAII();
	U16 iLocalMap = 0; // si
	while (true)
	{
		if (iLoopCount%10 == 0 || iLoopCount <= 10)
			SkD((SkCodeParam::bEngineNoDisplay||DLV_DBG_INIT, "GAME_LOOP (%08d)\n", iLoopCount));
		iLoopCount++,

		//DISPLAY_HINT_TEXT(COLOR_YELLOW, message);
		// SPX get speed from window menu
		stdTickBalance = skWinApp->spfact*4;
		//stdTickBalance = 1;
		glbTickSpeed = stdTickBalance;
		// SPX

		//^13AE:0072
		glbIntermediateTickCounter = 0;
		if (glbMapToLoad == 0xffff)
			goto _00a4;
		while (true)
		{
			//^13AE:007F
			//printf("LOAD_NEWMAP %d\n", glbMapToLoad);
			LOAD_NEWMAP(U8(glbMapToLoad));
			printf("MOVE_RECORD_TO\n");
			MOVE_RECORD_TO(OBJECT_NULL, -1, 0, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
			glbMapToLoad = 0xffff;
			//^13AE:00A4
_00a4:
			//printf("PROCEED_TIMERS\n");
			PROCEED_TIMERS();
			if (glbMapToLoad != 0xffff)
				continue;
			break;
		}
		//printf("UPDATE_WEATHER\n");
		UPDATE_WEATHER(0);
		
		SkD((DLV_DBG_RAIN, "Loop (Rain) >> lvl=%03d / strm=%03d / wet=%03d (r2:%d r3:%d mlt:%d) / tick=%d\n"
			, glbRainStrength, glbRainStormController
			, glbRainLevelForGround
			, glbRainRelated2, glbRainRelated3, glbRainMultiplicator
			, glbGameTick
			));

		if (cd.pi.glbIsPlayerSleeping == 0) {
			//^13AE:00C2
			if (cd.pi.glbNextChampionNumber == 0)
				//^13AE:00C9
				_38c8_0060();
			//^13AE:00CE
			if (glbChampionInventory == 0) {
				//^13AE:00D5
				if (glbDoLightCheck != 0)
					//^13AE:00DC
					CHECK_RECOMPUTE_LIGHT(cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
				//^13AE:00EB
				
				if (cd.pi.glbIsPlayerMoving != 0)
					//^13AE:00F2
					DISPLAY_VIEWPORT(_4976_4c40, _4976_4c32, _4976_4c34);
				else
					//^13AE:0100
					DISPLAY_VIEWPORT(cd.pi.glbPlayerDir, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
				//^13AE:0114
				
				CHANGE_VIEWPORT_TO_INVENTORY(1);
				_4976_4bc8 = 0;
			}
			//^13AE:0122
			if (cd.pi.glbIsPlayerMoving != 0) {
				//^13AE:0129
				if (cd.pi.glbIsPlayerMoving == 1) {
					//^13AE:0130
					//printf("Pending PERFORM MOVE\n");
					PERFORM_MOVE(cd.pi.glbPlayerLastMove);
				}
				//^13AE:013A
				cd.pi.glbIsPlayerMoving--;
			}
			//^13AE:013E
			_4976_4e64 = 0;
			if (glbShowMousePointer != 0) {
				//^13AE:014B
				glbShowMousePointer = 0;
				_443c_0434();	// DM2_events_443c_0434
			}
			//^13AE:0156
			//printf("GAME_LOOP: IBMIO_USER_INPUT_CHECK\n"); getch();
			IBMIO_USER_INPUT_CHECK();
		}
		//printf("UPDATES\n");
		//printf("GAME_LOOP: updates\n"); getch();
		//^13AE:015B
		CHOOSE_HIGHLIGHT_ARROW_PANEL();
		SOUND_482b_05bf(0);
		PROCESS_PLAYERS_DAMAGE();
		if (glbGlobalSpellEffects.AuraOfSpeed != 0)
			//^13AE:0174
			glbGlobalSpellEffects.AuraOfSpeed--;
		/// SPX: glbGameTick is a general tick counter : each 16 or 64 then do update
		if ((X16(glbGameTick) & ((cd.pi.glbIsPlayerSleeping != 0) ? 15 : 0x3f)) == 0)
			UPDATE_CHAMPIONS_STATS();
		//^13AE:0192
		GLOBAL_UPDATE_UNKNOW1();
		_2e62_0cfa(1);
		if (cd.pi.glbPlayerDefeated != 0)
			return;
		glbGameTick++;
		//ATLASSERT(ValidateMements());
		//printf("DEALLOC\n");
		PROCESS_QUEUED_DEALLOC_RECORD();
		//^13AE:01B3
		SkD((DLV_SYS,"SYS: Tick increased to %u ------------------------------\n", (Bitu)glbGameTick));
		SkD((DLV_DBG_SEED,"DBG: t %08X Seed %08X \n", (Bitu)glbGameTick, (Bitu)glbRandomSeed));
		SkD((DLV_DBG_SED3,"DBG: v %02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X \n"
			, (Bitu)(U8)glbTabCreaturesInfo[1].Command
			, (Bitu)(U8)glbTabCreaturesInfo[1].b27
			, (Bitu)(U8)glbTabCreaturesInfo[1].b28
			, (Bitu)(U8)glbTabCreaturesInfo[1].b29
			, (Bitu)(U8)glbTabCreaturesInfo[1].ItemToThrow
			, (Bitu)(U8)glbTabCreaturesInfo[1].b31
			, (Bitu)(U8)glbTabCreaturesInfo[1].b32
			, (Bitu)(U8)glbTabCreaturesInfo[1].iSeqControl
			));
		if ((X16(glbGameTick) & 0x1ff) == 0)	// every 511 tick, burn lighting items
			//^13AE:01BB
			BURN_PLAYER_LIGHTING_ITEMS();
		//^13AE:01C0
		// SPX: This is the freeze value
		if (glbGlobalSpellEffects.FreezeCounter != 0)
		{
			glbGlobalSpellEffects.FreezeCounter = glbGlobalSpellEffects.FreezeCounter -1;
		}
		if (_4976_4c00 != 0)
			_4976_4c00--;
		if (glbPlayerThrowCounter != 0)
			glbPlayerThrowCounter--;
		_3929_086f();
		glbTickStepReached = 0;
		CHOOSE_HIGHLIGHT_ARROW_PANEL();
		if (false) {
_01f7:
			_1031_0d36_KEYBOARD(0x20, SPECIAL_UI_KEY_TRANSFORMATION());
		}

		MessageLoop(true); // in game
		do {
			if (IS_THERE_KEY_INPUT_2() != 0) {
				goto _01f7;
			}
			//^13AE:020F
			if (_4976_4c3e != 0) {
				//^13AE:0216
				X16 di;
				di = _0cee_04e5(cd.pi.glbPlayerDir, 1, 0, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY);
				if ((di >> 5) != 6 || (di & 1) == 0 || (di & 4) != 0) {
					//^13AE:0247
					_4976_4c3e = 0;
					FIRE_MOUSE_RELEASE_CAPTURE();
					glbMouseVisibility = 1;
					FIRE_SHOW_MOUSE_CURSOR();
				}
			}
			//^13AE:025D
			//printf("MAIN LOOP\n");
			MAIN_LOOP();
			//^13AE:0262

			MessageLoop(false); // in game

		} while (glbTickStepReached == 0 || _4976_4c02 == 0);
		//printf("END OF LOOP\n");
		//^13AE:0270
		iLocalMap = glbMapToLoad;
		if (iLocalMap != 0xFFFF) {
			iLocalMap = glbCurrentMapIndex;
			CHANGE_CURRENT_MAP_TO(glbMapToLoad);
		}
		//^13AE:0288
		_2759_12e6();
		if (iLocalMap != 0xFFFF) {
			CHANGE_CURRENT_MAP_TO(iLocalMap);
		}
		//DM2DOS_R_BA7(ddata.v1e0266);
		REQUEST_PLAY_MUSIC_FROM_MAP(cd.pi.glbPlayerMap);
		continue;
	}
	//^13AE:029F
	return;
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


void SkWinCore::zeroMem(void *pv, size_t cb) {
	memset(pv, 0, cb);
}

void SkWinCore::_04bf_0102() { // TODO: Unr
	Unr();
}


void SkWinCore::_sys_intr() {
}

//}}SKSAVE_PUB_PROC





SkWinCore::SkWinCore() 
{
	//{{INIT_SKVAL1
#include <kskval1.h>
	//}}INIT_SKVAL1

#pragma warning(push)
#pragma warning(disable: 4309)
#include <kskval2.h>
#pragma warning(pop)

	skWinApp = NULL;
// SPX / DM1 special init
	cd.dm1.bDM1PortraitsActivated[0] = false;
	zeroMem(cd.dm1.xDM1PortraitsData, sizeof(cd.dm1.xDM1PortraitsData));
//

	//{{INIT_INSORTED
	__vram = pbVram = vram;
	for (int x=0; x<256; x++)
		intrvect[x] = &SkWinCore::_sys_intr;
	_int33_mouse_callback = &SkWinCore::_sys_intr;

	_4976_0cba_PFN_12[ 0] = &SkWinCore::RETURN_1;
	_4976_0cba_PFN_12[ 1] = &SkWinCore::IS_GAME_ENDED;
	_4976_0cba_PFN_12[ 2] = &SkWinCore::_1031_0023_PFN12_02;
	_4976_0cba_PFN_12[ 3] = &SkWinCore::_1031_003e_PFN12_03;
	_4976_0cba_PFN_12[ 4] = &SkWinCore::_1031_007b_PFN12_04;
	_4976_0cba_PFN_12[ 5] = &SkWinCore::_1031_009e_PFN12_05;
	_4976_0cba_PFN_12[ 6] = &SkWinCore::_1031_00c5_PFN12_06;
	_4976_0cba_PFN_12[ 7] = &SkWinCore::_1031_00f3_PFN12_07;
	_4976_0cba_PFN_12[ 8] = &SkWinCore::_1031_012d_PFN12_08;
	_4976_0cba_PFN_12[ 9] = &SkWinCore::_1031_014f_PFN12_09;
	_4976_0cba_PFN_12[10] = &SkWinCore::_1031_0184_PFN12_10;
	_4976_0cba_PFN_12[11] = &SkWinCore::_1031_01ba_PFN12_11;

	_4976_38a4_PFN_17[ 0] = &SkWinCore::_14cd_18cc_PFN17_00;
	_4976_38a4_PFN_17[ 1] = &SkWinCore::_14cd_19a4_PFN17_01;
	_4976_38a4_PFN_17[ 2] = &SkWinCore::_14cd_1a3c_PFN17_02;
	_4976_38a4_PFN_17[ 3] = &SkWinCore::_14cd_1a5a_PFN17_03;
	_4976_38a4_PFN_17[ 4] = &SkWinCore::_14cd_1b74_PFN17_04;
	_4976_38a4_PFN_17[ 5] = &SkWinCore::_14cd_1b90_PFN17_05;
	_4976_38a4_PFN_17[ 6] = &SkWinCore::_14cd_1c27_PFN17_06;
	_4976_38a4_PFN_17[ 7] = &SkWinCore::_14cd_1c45_PFN17_08;
	_4976_38a4_PFN_17[ 8] = &SkWinCore::_14cd_1c63_PFN17_09;
	_4976_38a4_PFN_17[ 9] = &SkWinCore::_14cd_1c8d_PFN17_09;
	_4976_38a4_PFN_17[10] = &SkWinCore::_14cd_1cec_PFN17_10;
	_4976_38a4_PFN_17[11] = &SkWinCore::_14cd_1d42_PFN17_11;
	_4976_38a4_PFN_17[12] = &SkWinCore::_14cd_1e36_PFN17_12;
	_4976_38a4_PFN_17[13] = &SkWinCore::_14cd_1e52_PFN17_13;
	_4976_38a4_PFN_17[14] = &SkWinCore::_14cd_1e6e_PFN17_14;
	_4976_38a4_PFN_17[15] = &SkWinCore::_14cd_1f8b_PFN17_15;
	_4976_38a4_PFN_17[16] = &SkWinCore::_14cd_1fa7_PFN17_16;
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
	glbRectX_0106 = 16; // cx
	glbRectY_0108 = 16; // cy
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
	cd.mk.glbMouseXPos = 0;
	cd.mk.glbMouseYPos = 0;
	zeroMem(&_04bf_1852, sizeof(_04bf_1852));
	cd.mk._04bf_17a8 = 0;
	glbMouseCursorVisible = 0;
	glbUIKeyReadCount = 0; //?
	glbDMode = 0;
	_04bf_0284 = 0;
	_04bf_0280 = 0;
	_04bf_03d4 = 0;
	_01b0_0933 = 0;
	_01b0_14d8 = 0;
	glbZMode = 1;
	_4976_04ee = 0;
	cd.sc.sndSoundInBuffer = 0;		// (_01b0_13d6)
	sndLockSoundBuffer = 0;
	cd.sc.sndSoundBufferSize = 0;
	cd.sc.sndSoundBuffer = NULL;
	_04bf_17e8 = 0;
	glbMouseStateRingIndex = 10;
	glbCounterZero_0517 = 0;
	glbSomeCounter_0519 = 0;
	_01b0_13c6 = 0;
	_04bf_18ae = NULL;
	_04bf_179e = NULL;
	_4976_4ea6 = 0;
	_4976_4e00 = 0;
	glbEndCounter = 0;
	_4976_4e46 = 0;
	glbAbsoluteTickCounter = 0;
	_4976_19a9 = -1;
	_4976_4e64 = 0;
//	glbPtrTransmittedUIEvent = NULL;
	glbHoldedContainerType = 0;
	_4976_53a4 = 0;
	zeroMem(glbItemSelected, sizeof(glbItemSelected));
	glbMagicalMapFlags = 0;
	glbPtrTransmittedUIEvent = NULL;
	zeroMem(&glbMousePosition, sizeof(glbMousePosition));
	glbMousePosition.XPos = -1;
	zeroMem(&_4976_4e9e, sizeof(_4976_4e9e));
	zeroMem(tlbUIClickEvents, sizeof(tlbUIClickEvents));
	glbTryPushPullObject = 0;
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
	glbMap_4976_4c12 = 0;
	glbCurrentMapIndex = -1;
	glbSomePosX_4976_4c04 = 0;
	glbSomePosY_4976_4c1e = 0;
//	glbMap_4976_4c12 = 0;
	glbDir_4976_4c10 = 0;
	_4976_0118 = 0x11;
	_4976_011a = 0x11;
	cd.pi.glbChampionsCount = 0;
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
	zeroMem(tlbMouseStateRing, sizeof(tlbMouseStateRing));
	zeroMem(tblViewportClickableRectangles, sizeof(tblViewportClickableRectangles));
	glbSquadInterfaceMapGfxSet = -1;
	glbMapGraphicsSet = -1;
	_4976_532c = 0;
	zeroMem(&_4976_495c, sizeof(_4976_495c));
	zeroMem(_4976_4df6, sizeof(_4976_4df6));
	zeroMem(&cd.pi.glbLeaderHandPossession, sizeof(cd.pi.glbLeaderHandPossession));
	zeroMem(cd.pi.glbChampionsPendingDamage, sizeof(cd.pi.glbChampionsPendingDamage));
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

	cd.mk.mice_x = cd.mk.mice_y = cd.mk.mice_btn = 0;
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

