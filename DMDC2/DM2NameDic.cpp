// DM2NameDic.cpp: CDM2NameDic �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "exp00069.h"
#include "DM2NameDic.h"
#include "CSVrw.h"
#include "DM2OrnateTreeDic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace
{

CDM2NameDic g_cDic;

#if 0
//LPCSTR g_pszChests[256] = {
//	"CHEST",
//	"BAG",
//	"MONEY BOX",
//	"QUIVER",
//	"",
//	"",
//	"",
//	"",
//	"MAGIC MAP",
//	"SCOUT MAP",
//	"MINION MAP",
//	"MAGIC MAP",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//};
//
//LPCSTR g_pszClothes[256] = {
//	"CAPE",
//	"CLOAK OF NIGHT",
//	"BLUE PANTS",
//	"SANDALS",
//	"BLACK BOOTS",
//	"BLACK TOP",
//	"BLACK SKIRT",
//	"FINE ROBE TOP",
//	"FINE ROBE ",
//	"FIRE PLATE",
//	"BODICE",
//	"TABARD",
//	"GUNNA",
//	"DOUBLET",
//	"HUKE",
//	"MITHRAL HOSEN",
//	"LEATHER JERKIN",
//	"LEATHER PANTS",
//	"LEATHER BOOTS",
//	"BRIGANDINE",
//	"TUNIC",
//	"SCALE HAUBERK",
//	"SCALE MAIL",
//	"BANDANA",
//	"GREAT HELM",
//	"HORNED HELM",
//	"HELMET",
//	"BASCINET",
//	"SHIELD OF FIRE",
//	"CRYSTAL SHIELD",
//	"WOOD SHIELD",
//	"SMALL SHIELD",
//	"BREASTPLATE",
//	"THIGH PLATES",
//	"MITHRAL MAIL",
//	"MITHRAL HUKE",
//	"MAIL HELMET",
//	"GREAVES",
//	"ARMET",
//	"TORSO PLATE",
//	"LEG PLATE",
//	"FOOT PLATE",
//	"{SAR} SHIELD",
//	"{RA}{SAR} HELM",
//	"{RA}{SAR} PLATE",
//	"{RA}{SAR} POLEYN",
//	"{RA}{SAR} GREAVE",
//	"{RA}{SAR} SHIELD",
//	"TECHELM",
//	"TECHPLATE",
//	"TECHPOLEYN ",
//	"TECHBOOTS",
//	"TECHSHIELD",
//	"FIRE HELM",
//	"FIRE PLATE",
//	"FIRE POLEYN",
//	"FIRE GREAVE",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//};
//
//LPCSTR g_pszMisc[256] = {
//	"?'BONES",
//	"BOTA",
//	"JEWEL OF SYMAL",
//	"ILLUMULET",
//	"",
//	"COMPASS",
//	"COPPER COIN",
//	"SMALL PLANK",
//	"GOLD COIN",
//	"IRON KEY",
//	"BLOOD KEY",
//	"SOLID KEY",
//	"MOON KEY",
//	"LIGHTNING KEY",
//	"CROSS KEY",
//	"ONYX KEY",
//	"SKULL KEY",
//	"GOLD KEY",
//	"{IR} KEY",
//	"{YA} KEY",
//	"{OH} KEY",
//	"{VI} KEY",
//	"{FUL} KEY",
//	"{RA} KEY",
//	"MASTER KEY",
//	"BOULDER",
//	"BLUE GEM",
//	"RED GEM",
//	"GREEN GEM",
//	"APPLE",
//	"JICAMA",
//	"BREAD ",
//	"CHEESE",
//	"VACUUM FUSE",
//	"SPIRIT CAP",
//	"SHANK",
//	"STEAK",
//	"MEZMAR",
//	"SUZERAIN",
//	"MOONSTONE",
//	"RUNE CHARM",
//	"CLAN CHIEF GEM",
//	"METEOR METAL",
//	"MAGIKAL BOX",
//	"LARGE GEAR",
//	"ROPE",
//	"",
//	"MANA BLOSSOM",
//	"SUN CREST",
//	"CLAN KEY PIECE",
//	"PALMAPPLE",
//	"FAIRY CUSHION",
//	"BONES",
//	"PLANK",
//	"COVER PLATE",
//	"TAPESTRY",
//	"SILVER COIN",
//	"WORM ROUND",
//	"PYRO",
//	"DEAD BAT",
//	"TANKARD",
//	"BONE",
//	"{YA} {EW}",
//	"ATTACK MINION",
//	"GUARD MINION",
//	"",
//	"TECHEYE",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//};
//
//LPCSTR g_pszPotions[256] = {
//	"",
//	"",
//	"",
//	"{VEN} BOMB",
//	"",
//	"",
//	"{OH} {BRO} {ROS} POTION",
//	"{FUL} {BRO} {KU} POTION",
//	"{YA} {BRO} {DAIN} POTION",
//	"{YA} {BRO} {NETA} POTION",
//	"{VI} {BRO} POTION",
//	"{YA} POTION",
//	"{YA} {BRO} POTION",
//	"{ZO} {BRO} {RA} POTION",
//	"{VI} POTION",
//	"WATER FLASK",
//	"",
//	"",
//	"",
//	"{FUL} BOMB",
//	"FLASK",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//};
//
//LPCSTR g_pszScrolls[256] = {
//	"SCROLL",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//};
//
//LPCSTR g_pszWeapons[256] = {
//	"EYE OF TIME",
//	"STORMRING",
//	"TORCH",
//	"KALAN GAUNTLET",
//	"ROGUE STAVE",
//	"TEMPEST",
//	"FURY",
//	"SCYTHE",
//	"DAGGER",
//	"MACHETE",
//	"SWORD",
//	"RAPIER",
//	"SABRE",
//	"KATANA",
//	"EXCSYMYR",
//	"BLUE STEELE",
//	"VORPAL BLADE",
//	"{ZO} BLADE",
//	"AXE",
//	"VORAX",
//	"MACE",
//	"TECHMACE",
//	"MORNINGSTAR",
//	"CLUB",
//	"WAR CLUB",
//	"BOW",
//	"CROSSBOW",
//	"ARROW",
//	"SLAYER ARROW",
//	"SLING",
//	"ROCK",
//	"POISON DART",
//	"SHURIKEN",
//	"BRANCH",
//	"STAFF",
//	"RAINBOW WAND",
//	"SPIRAL STAFF",
//	"EMERALD ORB",
//	"COMBAT STAFF",
//	"STAFF OF {NETA}",
//	"SERPENT STAFF",
//	"BAINBLOOM",
//	"NUMENSTAFF",
//	"SCARAB",
//	"SPEEDBOW",
//	"METEOR METAL",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//};
//
//LPCSTR g_pszCreatures[256] = {
//	"THORN DEMON",
//	"CAVERN TABLE",
//	"VORTEX",
//	"",
//	"BUSH",
//	"PILLAR",
//	"STALAGMITE",
//	"BOULDER",
//	"FACE PILLAR",
//	"GLOP",
//	"",
//	"",
//	"ROCKY",
//	"GIGGLER",
//	"THICKET THEIF",
//	"TIGER STRIPED WORM",
//	"SPECTRE",
//	"POWER CRYSTAL",
//	"TREE GORGON",
//	"FOUNTAIN",
//	"TREE",
//	"",
//	"SKULL BRAZIER",
//	"DRAGOTH",
//	"CAVE IN",
//	"WOOD TABLE",
//	"MAGIK CAULDRON",
//	"EVIL ATTACK MINION",
//	"TOWER BAT",
//	"ARCHER-GUARD",
//	"",
//	"VEXIRK KING",
//	"DARK VEXIRK",
//	"PIT GHOST",
//	"MERCHANT",
//	"CAVERN BAT",
//	"SPIKED CAPSTAN",
//	"EVIL FOUNTAIN",
//	"MUMMY",
//	"PIPE SHAFT",
//	"",
//	"LIGHTNING ROD",
//	"SARDES MINION",
//	"EVIL SCOUT MINION",
//	"MERCHANT GUARD",
//	"BARREL",
//	"MAGIK REFLECTOR",
//	"BOILER",
//	"SCOUT MINION",
//	"ATTACK MINION",
//	"CARRY MINION",
//	"FETCH MINION",
//	"GUARD MINION",
//	"U-HAUL MINION",
//	"",
//	"FLAME ORB",
//	"DIGGER WORM",
//	"TOMBSTONE",
//	"MERCHANT TOMB",
//	"TRADING TABLE (SELLS)",
//	"TRADING TABLE (BUYS)",
//	"OBELISK",
//	"OBELISK",
//	"OBELISK",
//	"",
//	"SKELETON",
//	"AXEMAN",
//	"BAD MERCHANT",
//	"MAGIC MERCHANT",
//	"EVIL SCOUT MINION",
//	"VOID DOOR",
//	"STEAM ENGINE",
//	"AXEMAN THIEF",
//	"GHOST",
//	"WOLF",
//	"",
//	"DRAGOTH ATTACK MINION",
//	"CAVERN TABLE",
//	"DOOR GHOST",
//	"",
//	"",
//	"SPIKED WALL",
//	"DRU TAN",
//	"CAVERN TABLE",
//	"CHEST",
//	"PEDISTAL",
//	"",
//	"EVIL ATTACK MINION",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//};
//
//LPCSTR g_pszHeroes[256] = {
//	"ALIAI MON",
//	"KOL DEL TAC",
//	"ANDERS LIGHT WIELDER",
//	"HET FARVIL",
//	"EQUUS",
//	"GRAEN OZBOR",
//	"CLETUS",
//	"BANE BLADE CLEAVER",
//	"ODO ALU KAILO",
//	"CORDAIN DAWNKEEPER",
//	"TRESA VULPES",
//	"SERI FLAMEHAIR",
//	"UGGO THE MAD",
//	"JAROD NIGHTWIELDER",
//	"TORHAM ZED",
//	"SAROS SHADOW FOLLOWER",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//	"",
//};
#endif

//CDM2ActuHint g_Actuators[256] = {
//	//FLOOR
//#include "ACTUF.h"
//	//WALL
//#include "ACTUW.h"
//};

};

//////////////////////////////////////////////////////////////////////
// 

namespace
{

static bool ParseA1t(CString str, int &a1t)
{
	if (false) a1t;
	else if (str.CompareNoCase("no") == 0) a1t = a1tNo;
	else if (str.CompareNoCase("item") == 0) a1t = a1tItem;
	else if (str.CompareNoCase("map") == 0) a1t = a1tMap;
	else if (str.CompareNoCase("n") == 0) a1t = a1tN;
	else if (str.CompareNoCase("missile") == 0) a1t = a1tMissile;
	else if (str.CompareNoCase("creature") == 0) a1t = a1tCreature;
	else if (str.CompareNoCase("hero") == 0) a1t = a1tHero;
	else if (str.CompareNoCase("aistate") == 0) a1t = a1tAIState;
	else return false;

	return true;
}

static bool ParseA2t(CString str, int &a2t)
{
	if (false) a2t;
	else if (str.CompareNoCase("no") == 0) a2t = a2tNo;
	else return false;

	return true;
}

static bool ParseA3t(CString str, int &a3t)
{
	if (false) a3t;
	else if (str.CompareNoCase("no") == 0) a3t = a3tNo;
	else if (str.CompareNoCase("locator") == 0) a3t = a3tLocator;
	else if (str.CompareNoCase("str") == 0) a3t = a3tStr;
	else return false;

	return true;
}

static bool ParseA3lt(CString str, int &a3lt)
{
	if (false) a3lt;
	else if (str.CompareNoCase("no") == 0) a3lt = a3ltNo;
	else if (str.CompareNoCase("tile") == 0) a3lt = a3ltTile;
	else if (str.CompareNoCase("trap") == 0) a3lt = a3ltTrap;
	else return false;

	return true;
}

};

//////////////////////////////////////////////////////////////////////
// CDM2NameDic

LPCSTR CDM2NameDic::GetName(int i, int x)
{
	x = x & 255;
	switch (i) {
	case  2: return strMessages[x];
	case  3: return hintActuators[x].pszName;
	case  4: return strCreatures[x];
	case  5: return strWeapons[x];
	case  6: return strClothes[x];
	case  7: return strScrolls[x];
	case  8: return strPotions[x];
	case  9: return strChests[x];
	case 10: return strMisc[x];
	}
	return "";
}

LPCSTR CDM2NameDic::GetHeroName(int x)
{
	x = x & 255;
	return strHeroes[x];
}

const CDM2ActuHint &CDM2NameDic::GetActuHint(int i, BOOL fWall)
{
	i = i & 127;
	if (fWall) i |= 128;
	return hintActuators[i];
}

LPCSTR CDM2NameDic::GetDATDBName(int i, int x)
{
	x = x & 255;
	switch (i) {
	case 0x03: return strMessages[x];
	case 0x09: return CDM2OrnateTreeDic::Get1(0).GetName(x); // Wall
	case 0x0A: return CDM2OrnateTreeDic::Get1(1).GetName(x); // Floor
	case 0x0D: return strMissiles[x]; // Missile
	case 0x0F: return strCreatures[x];
	case 0x10: return strWeapons[x];
	case 0x11: return strClothes[x];
	case 0x12: return strScrolls[x];
	case 0x13: return strPotions[x];
	case 0x14: return strChests[x];
	case 0x15: return strMisc[x];
	case 0x16: return strHeroes[x];
	}
	return "";
}

bool CDM2NameDic::Load(DO2::CDO2PathStr strWorkDir)
{
	int nCount = 0, nDone = 0;
	nCount++, nDone += LoadTbl(strWorkDir ^ "_Chests.Dic.csv", strChests) ? 1 : 0;
	nCount++, nDone += LoadTbl(strWorkDir ^ "_Clothes.Dic.csv", strClothes) ? 1 : 0;
	nCount++, nDone += LoadTbl(strWorkDir ^ "_Misc.Dic.csv", strMisc) ? 1 : 0;
	nCount++, nDone += LoadTbl(strWorkDir ^ "_Potions.Dic.csv", strPotions) ? 1 : 0;
	nCount++, nDone += LoadTbl(strWorkDir ^ "_Scrolls.Dic.csv", strScrolls) ? 1 : 0;
	nCount++, nDone += LoadTbl(strWorkDir ^ "_Weapons.Dic.csv", strWeapons) ? 1 : 0;
	nCount++, nDone += LoadTbl(strWorkDir ^ "_Creatures.Dic.csv", strCreatures) ? 1 : 0;
	nCount++, nDone += LoadTbl(strWorkDir ^ "_Heroes.Dic.csv", strHeroes) ? 1 : 0;
	nCount++, nDone += LoadTbl(strWorkDir ^ "_Missiles.Dic.csv", strMissiles, 64) ? 1 : 0;
	nCount++, nDone += LoadTblA(strWorkDir ^ "_Actuf.Dic.csv", true) ? 1 : 0;
	nCount++, nDone += LoadTblA(strWorkDir ^ "_Actuw.Dic.csv", false) ? 1 : 0;
	nCount++, nDone += LoadTbl(strWorkDir ^ "_Messages.Dic.csv", strMessages, 64) ? 1 : 0;

	nCount++, nDone += CDM2OrnateTreeDic::Get1(0).Load(strWorkDir ^ "_Wall_Ornates.Dic.csv") ? 1 : 0;
	nCount++, nDone += CDM2OrnateTreeDic::Get1(1).Load(strWorkDir ^ "_Floor_Ornates.Dic.csv") ? 1 : 0;

	return nCount == nDone;
}

bool CDM2NameDic::LoadTbl(LPCTSTR psz, CString strTbl[256], int nMax)
{
	CSVR_t csvr;
	if (!csvr.Open(psz)) return false;

	UINT i;
	for (i = 0; i < nMax && csvr.ReadNextLine(); i++) {
		CString str;
		strTbl[i].Empty();
		if (!csvr.ReadNextToken(str)) continue;
		strTbl[i] = str;
	}
	return true;
}

CDM2NameDic &CDM2NameDic::GetInstance()
{
	return g_cDic;
}

bool CDM2NameDic::LoadTblA(LPCTSTR psz, bool fFloor)
{
	CSVR_t csvr;
	if (!csvr.Open(psz)) return false;

	UINT x = fFloor ? 0 : 128;
	UINT i;
	for (i = 0; i < 128; i++) hintActuators[x + i] = CDM2ActuHint();
	for (i = 0; i < 128 && csvr.ReadNextLine(); i++, x++) {
		CString str[5];
		if (false
			|| !csvr.ReadNextToken(str[0])
			|| !csvr.ReadNextToken(str[1])
			|| !csvr.ReadNextToken(str[2])
			|| !csvr.ReadNextToken(str[3])
			|| !csvr.ReadNextToken(str[4])
		) {
			i--;
			continue;
		}
		hintActuators[x].pszName = str[0];
		if (false
			|| !ParseA1t(str[1], hintActuators[x].a1t)
			|| !ParseA2t(str[2], hintActuators[x].a2t)
			|| !ParseA3t(str[3], hintActuators[x].a3t)
			|| !ParseA3lt(str[4], hintActuators[x].a3lt)
		) {
			i--;
			continue;
		}
	}
	return (i == 128);
}

LPCSTR CDM2NameDic::GetCreatureName(int x)
{
	return GetName(0x4, x);
}

LPCSTR CDM2NameDic::GetOrnateName(int x, BOOL fWall)
{
	return GetDATDBName(fWall ? 0x09 : 0x0A, x);
}