#include "addon.h"
#include "driver.h" // DRVW_mousevisicnt
#include "dm2global.h"
#include "dm2data.h"
#include "mdata.h"
#include "src/c_alloc.h"
#include "src/c_dballoc.h"
#include "src/c_mouse.h"
#include "src/c_gfx_pal.h"
#include "src/c_gfx_stretch.h"
#include "src/c_gfx_misc.h"
#include "src/c_gfx_blit.h"
#include "src/c_gfx_str.h"
#include "src/c_gfx_fill.h"
#include "src/c_gfx_decode.h"
#include "src/c_savegame.h"
#include "calls.h"


#include "../skwin/defines.h"

#include "dm2debug.h"


// SPX: Name of spell type
const char* getSpellTypeName(ui8 spelltype)
{
	static char text[128] = {0};
	if (spelltype == SPELL_TYPE_POTION) return "POTION";
	if (spelltype == SPELL_TYPE_MISSILE) return "MISSILE";
	if (spelltype == SPELL_TYPE_GENERAL) return "ENCHANTMENT";
	if (spelltype == SPELL_TYPE_SUMMON) return "SUMMON";
	if (text[0] != 0)
		return text;
	return "Unknown";
}


// SPX: Name of skill
const char* getSkillName(ui8 skill)
{
	static char text[128] = {0};
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
		return text;
	return "UNKNOWN";
}

// SPX: Name of bonus
const char* getStatBonusName(ui8 bonus)
{
	static char text[128] = {0};
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
		return text;
	return "UNKNOWN";
}

// SPX: Name of UI Event
const char* getUIEventName(ui8 event)
{
	static char text[128] = {0};

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
		return text;
	return "Unknown";
}

// SPX: Name of AI
const char* getAIName(ui8 ai)
{
	static char text[128] = {0};

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
		return text;
	return "Unknown";

}

/*
const char* getRecordNameOf(ObjectID recordLink)
{
	static char text[128] = {0};
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
		return text;
	return "Unknown";
}
*/