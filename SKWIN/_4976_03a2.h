
#pragma once

// Next table is located in DM2 PSV FIRE.exe (unlzexe'd ver) at 0x00048162

static const DM2Internal::AIDefinition dAITableGenuine[62] = { // by wordval 0F-xx-05
//FLAGS , A/D,   ?,initHP, Str, Poi ,Def, SpAtk| Fly|Typ?|Mini|   ?| Pit|Ghos,     ?,     ?,AtkSpl,     ?,Sw,     ?,     ?,Resist,     ?,  ?,Wei,     ?,     ?,   ?,   ?
{ 0x0045, 255,   0,   700,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0F1F,0xF000,  0,255,0x4000,0x0000,   0,   2 },//  0  ^TREE
{ 0x0085, 255,   0,   450,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0F1F,0xF000,  0,255,0xFD22,0x1068,   0,   2 },//  1	beta: LAB TABLE
{ 0x0085, 255,   0,   100,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xF000,  0,255,0x9E22,0x1014,   0,   2 },//  2  ^CAVERN TABLE
{ 0x0085, 100,   0,   120,   0,   0,   0,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0D1F,0xF000,  0,255,0x2000,0x0000,   0,   1 },//  3  ^BUSH
{ 0x0045, 255,   0,   100,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xF000,  0,255,0x4000,0x0000,   0,   1 },//  4  ^CAVERN TABLE ^PILLAR ^FACE PILLAR ^PIPE SHAFT ^LIGHTNING ROD ^BOILER
{ 0x0045, 255,   0,   100,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xF000,  0,255,0x4000,0x0000,   0,   1 },//  5  ^STALAGMITE
{ 0x0045, 255,   0,   100,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xF000,  0, 95,0x4000,0x0000,   2,   2 },//  6  ^BOULDER
{ 0x0085, 255,   0,   100,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xF000,  0,255,0x0A01,0x0FD4,   0,   2 },//  7  ^FOUNTAIN
{ 0x0041, 255,   0,   100,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xF000,  0,255,0x4000,0x0000,   0,   2 },//  8  ^TOMBSTONE ^MERCHANT TOMB ^OBELISK ^OBELISK ^STEAM ENGINE
{ 0x0081,  80,   0,   160,   0,   0,   0,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0F1F,0xF000,  0, 45,0xAC42,0x0FA0,   0,   2 },//  9  ^WOOD TABLE
{ 0x0085, 255,   0,   100,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xF000,  0,255,0x0B01,0x1004,   0,   2 },// 10  ^MAGIK CAULDRON
{ 0x0045, 255,   0,   100,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xF000,  0, 40,0x4000,0x0000,   0,   0 },// 11  ^SKULL BRAZIER
{ 0x0081, 255,   0,   400,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xF000,  0,255,0xED41,0x109F,   0,   0 },// 12  ^TRADING TABLE (SELLS) ^TRADING TABLE (BUYS)
{ 0xCC40, 130,   5,   200,   0,   0, 100,0          |0x10|0x08|0x04          ,0x6D9E,0x0000,0x0040,0x0021, 1,0x20FF,0x01F0,0x0C5F,0xFCA0,  4, 10,0x0000,0x0000,   0,   1 },// 13  ^SCOUT MINION
{ 0xC840, 150,  -1,   100,  40,   0, 140,0          |0x10|0x08|0x04          ,0x6C06,0x0000,0x0040,0x0001, 1,0x20F8,0x01DE,0x0CCC,0xFC50,  4, 10,0x0000,0x0000,   0,   1 },// 14  ^ATTACK MINION
{ 0xC840, 130,  30,   150,   0,   0,  40,0               |0x08|0x04          ,0x6D9E,0x0003,0x0000,0x0021, 1,0x20F7,0x01C0,0x0C55,0xFC50,  4, 10,0x0000,0x0000,   0,   1 },// 15  ^CARRY MINION
{ 0xC840, 130,  30,   150,   0,   0,  40,0               |0x08|0x04          ,0x6D9E,0x0003,0x0000,0x0021, 1,0x20F7,0x01C0,0x0C55,0xFC50,  4, 10,0x0000,0x0000,   0,   1 },// 16  ^FETCH MINION
{ 0xC840, 200, -15,   200,  60,   0,  20,0          |0x10|0x08|0x04          ,0x2C06,0x0000,0x0040,0x0000, 1,0x20F2,0x01FE,0x0CDD,0xFC50,  4,255,0x0000,0x0000,   0,   1 },// 17  ^GUARD MINION
{ 0xC840, 110, -10,   150,   0,   0,  10,0               |0x08|0x04          ,0x6D9E,0x0003,0x0000,0x0021, 1,0x20F7,0x01C0,0x0C55,0xFC50,  4, 10,0x0000,0x0000,   0,   1 },// 18  ^U-HAUL MINION
{ 0x0240,  85,  20,   400,  50,   0, 120,0                                   ,0x0002,0x0000,0x0002,0x0000, 0,0x1053,0x078B,0x0876,0xFD40,  9,254,0x0000,0x0000,   3,   2 },// 19  ^THORN DEMON
{ 0x8041, 255,   0,   900,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xF000,  0,255,0x6000,0x0000,   0,   1 },// 20  ^OBELISK ^DIRECTIONS (can pass through)
{ 0x8CA0, 150,   5,   100,  80,   0, 100,0                                   ,0x0006,0x0000,0x0040,0x0000, 0,0x2012,0x02FF,0x0F81,0xFD72,  4,255,0x0000,0x0000,   1,   1 },// 21  ^VORTEX
{ 0x8860, 100,  20,    80, 100,   0, 150,0                                   ,0x0A06,0x0000,0x0010,0x0004, 0,0x4035,0x03FD,0x0FF6,0xFD62,  4, 20,0x0000,0x0000,   1,   1 },// 22  ^FLAME ORB
{ 0x8840,  50,  10,    30,  10,  40, 200,0               |0x08|0x04          ,0x0806,0x0000,0x0001,0x0000, 0,0x10F3,0x0295,0x0311,0xF100,  4, 10,0x0000,0x0000,   1,   1 },// 23  ^CAVERN BAT
{ 0x0AC0,  65,  20,    36,   6,   0,  80,0                                   ,0x2402,0x0001,0x0001,0x0000, 0,0x1034,0x924A,0x06D7,0xFFFC,  4, 65,0x0000,0x0000,   1,   1 },// 24  ^GLOP
{ 0x0A80,  80,  10,   150,  40,   0,  80,0                                   ,0xE402,0x0003,0x000A,0x0002, 1,0x307A,0x328A,0x09CC,0xFE82,  4, 70,0x0000,0x0000,   1,   1 },// 25  ^ROCKY
{ 0x0280,  50,   5,    90,  20,   0, 240,0                                   ,0x64A2,0x004F,0x000C,0x0032, 1,0x20FD,0x02DF,0x064F,0xFED3,  4, 40,0x0000,0x0000,   1,   1 },// 26  ^GIGGLER
{ 0x0280,  70,  10,   175,  35,   0, 110,0                                   ,0x6402,0x0047,0x000D,0x0002, 1,0x30A7,0x047C,0x0855,0xFEA2,  4, 45,0x0000,0x0000,   1,   1 },// 27  ^THICKET THEIF
{ 0x0AC0, 100,  20,   250,  80, 150,  70,0                                   ,0x0002,0x0000,0x0001,0x0000, 0,0x10A2,0x86E4,0x0972,0xFFEC,  4, 90,0x0000,0x0000,   2,   2 },// 28  ^TIGER STRIPED WORM
{ 0x0A40, 135,  30,   350,  50,   0,  40,0                                   ,0x0002,0x0000,0x0001,0x0000, 0,0x10A3,0xC7CA,0x0E0C,0xFB10,  4,255,0x0000,0x0000,   0,   2 },// 29  ^TREE GORGON
{ 0x1C40, 175,   5,  1500, 135,   0, 170,0               |0x08               ,0x41A2,0x0000,0x0DD1,0x005F, 1,0x40FD,0x1AEF,0x0EDD,0xFD62,  4,254,0x0000,0x0000,   2,   2 },// 30  ^DRAGOTH
{ 0x0040, 220,  10,   600, 160,   0, 120,0                                   ,0x4082,0x0001,0x0183,0x000A, 1,0x4068,0x07CB,0x0B88,0xFC41,  9,254,0x0000,0x0000,   2,   2 },// 31  ^DRU TAN
{ 0x0040, 120,   0,   300,   0,   0,   5,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x01FF,0x0FFF,0xF800,  0,255,0x0000,0x0000,   1,   2 },// 32  ^CAVE IN
{ 0x0240, 180,   5,  1000, 130,   0, 200,0                                   ,0x4002,0x000F,0x000B,0x0002, 1,0x3057,0x08EA,0x0CDC,0xFC30,  4,110,0x0000,0x0000,   1,   1 },// 33  ^MERCHANT ^MERCHANT GUARD ^BAD MERCHANT ^MAGIC MERCHANT
{ 0xC840,  75,  -1,   100,   8,   0, 180,0          |0x10|0x08|0x04          ,0x6D86,0x0000,0x0040,0x0021, 1,0x20FA,0x04EA,0x0CDF,0xFCA0,  4, 10,0x0000,0x0000,   0,   1 },// 34  ^DRAGOTH ATTACK MINION
{ 0x8840,  50,  10,   120,  60,  75, 200,0               |0x08|0x04          ,0x0807,0x0000,0x0001,0x0000, 0,0x10F4,0x0295,0x0311,0xF000,  4, 10,0x0000,0x0000,   1,   1 },// 35  ^TOWER BAT
{ 0x0240, 160,  10,   420, 100,   0, 140,0                                   ,0x6402,0x000F,0x0009,0x0002, 1,0x3078,0x078C,0x097C,0xFC40,  4,110,0x0000,0x0000,   2,   2 },// 36  ^ARCHER-GUARD
{ 0x0043, 255,   0,   100,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xF000,  0, 70,0x4000,0x0001,   0,   2 },// 37  ^MAGIK REFLECTOR
{ 0x0045, 255,   0,   100,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xF000,  0,255,0x4100,0x0001,   0,   2 },// 38  ^POWER CRYSTAL
{ 0x0085, 255,   0,   100,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xF000,  0,255,0x0A01,0x0FE3,   0,   2 },// 39  ^EVIL FOUNTAIN
{ 0x0140, 220,   0,   500,  60,   0, 250,0     |0x20                         ,0x0002,0x0000,0x0002,0x0000, 0,0x0000,0x0100,0x0F4F,0xFC61,  8,255,0x0000,0x0000,   3,   2 },// 40  ^SPIKED WALL
{ 0x8C78, 100,  30,   150,   2,   0, 150,0                                   ,0x6606,0x0000,0x0001,0x0000, 0,0x4034,0x65CF,0x0FF6,0xFD62,  4,255,0x0000,0x0000,   1,   1 },// 41  ^SPECTRE
{ 0x0280,  65,  30,    80,  10,   0,  90,0                                   ,0x2402,0x0000,0x0001,0x0000, 0,0x1031,0x637D,0x041C,0xFB10,  4,254,0x0000,0x0000,   0,   2 },// 42  ^DIGGER WORM
{ 0xC840,  75,   5,   150,   8,   0, 180,0          |0x10|0x08|0x04          ,0x6D9E,0x0000,0x0040,0x0021, 1,0x20FF,0x03E9,0x0C7F,0xFCA0,  4, 10,0x0000,0x0000,   0,   1 },// 43  ^EVIL ATTACK MINION ^SARDES MINION ^EVIL SCOUT MINION
{ 0x0240, 115,  10,   175,  90,   0, 100,0                                   ,0x6402,0x004B,0x0009,0x0008, 1,0x3045,0x15EE,0x0A55,0xFEA2,  4, 70,0x0000,0x0000,   1,   1 },// 44  ^AXEMAN
{ 0x0081, 255,   0,   190,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xF000,  0,255,0x8C42,0x0FA0,   0,   2 },// 45  ^CAVERN TABLE ^CAVERN TABLE ^KEEP STONE TABLE ^CAVE WALL HOLE
{ 0x0240,  70,  30,    50,  35,   0,  60,0                                   ,0x4402,0x004F,0x0009,0x000A, 1,0x1024,0x25E2,0x1F3F,0xFEA2,  4, 50,0x0000,0x0000,   1,   1 },// 46  ^MUMMY
{ 0x2044, 255,   0,   800,   0,   0, 255,0     |0x20                         ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xFC61,  8,255,0x0000,0x0000,   0,   1 },// 47  ^VOID DOOR
{ 0x0290, 100,   5,   150,  15,   0, 160,0                                   ,0x4002,0x000F,0x05F9,0x0037, 1,0x309E,0x85E6,0x0C6F,0xFD93,  4, 40,0x0000,0x0000,   1,   1 },// 48  ^DARK VEXIRK
{ 0xC840,  75,   0,   100,  50,   0, 180,0               |0x08|0x04          ,0x2C06,0x0000,0x0040,0x0021, 1,0x20FF,0x03FE,0x0C7F,0xFCA0,  4, 10,0x0000,0x0000,   0,   1 },// 49  ^EVIL GUARD MINION
{ 0x0240, 115,  10,   175,  60,   0, 100,0               |0x08               ,0x6402,0x004F,0x0009,0x0008, 1,0x3014,0x14E8,0x0F85,0xFEA2,  4, 55,0x0000,0x0000,   1,   1 },// 50  ^SKELETON
{ 0x0041, 255,   0,   100,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xF000,  0, 50,0x4000,0x0000,   0,   1 },// 51	beta: AMPLIFIER
{ 0x0280,  90,  20,   150,  30,   0, 130,0                                   ,0xC002,0x0001,0x0001,0x0000, 0,0x10C7,0x03DA,0x0672,0xFEC5,  4, 65,0x0000,0x0000,   2,   2 },// 52  ^WOLF
{ 0xACB8, 255,  10,   200,   0,   0, 255,0|0x40                              ,0x0C04,0x0000,0x0000,0x0000, 0,0x40FF,0x01C7,0x0FFF,0xFD62,  4,255,0x0000,0x0000,   1,   1 },// 53  ^PIT GHOST
{ 0xACB8, 255,  10,   200,   0,   0, 255,0                                   ,0x4802,0x0000,0x0000,0x0060, 0,0x40FF,0x01C7,0x0FFF,0xFD62,  4,255,0x0000,0x0000,   1,   1 },// 54  ^DOOR GHOST
{ 0x0290, 140,   5,   250,  80,   0, 184,0                                   ,0x4002,0x000F,0x0010,0x0037, 1,0x30AF,0x55B9,0x0DAF,0xFD93,  4, 50,0x0000,0x0000,   1,   1 },// 55  ^VEXIRK KING
{ 0x8081, 255,   0,   900,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xF000,  0,255,0x6000,0x0000,   0,   1 },// 56 
{ 0x0240, 100,  10,   160,  60,   0, 120,0                                   ,0x6402,0x004B,0x000D,0x0008, 1,0x3075,0x158C,0x0A55,0xFEA2,  4, 75,0x0000,0x0000,   1,   1 },// 57  ^AXEMAN THIEF
{ 0x0880, 110,   1,   350,   0,   0,  10,0               |0x08|0x04          ,0x0002,0x0001,0x0000,0x0000, 0,0x10FF,0x03FF,0x1F9C,0xFC50,  4, 40,0x0000,0x0000,   0,   1 },// 58  ^CHEST
{ 0x0045, 140,   0,   260,   0,   0,   0,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0F3F,0xF000,  0, 55,0x6000,0x0000,   0,   2 },// 59  ^BARREL
{ 0x0085, 255,   0,   100,   0,   0, 255,0                                   ,0x0000,0x0000,0x0000,0x0000, 0,0x0000,0x0100,0x0FFF,0xF000,  0,255,0x4000,0x0000,   0,   2 },// 60  ^PEDISTAL
{ 0x8C78, 100,  30,   150,  40,   0, 150,0               |0x08|0x04     |0x01,0x6606,0x0000,0x0001,0x0000, 0,0x4034,0x44C6,0x0FF6,0xFD62,  4,255,0x0000,0x0000,   1,   1 },// 61  ^GHOST
/// SPX: PCDOS version has a 62 AI for Evil Attack Minions

};

// SPX: 86 * 36 = 3096
static const char _4976_03a2[] = { // by wordval 0F-xx-05
	"\x45\x00\xFF\x00\xBC\x02\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x1F\x0F\x00\xF0\x00\xFF\x00\x40\x00\x00\x00\x02"
	"\x85\x00\xFF\x00\xC2\x01\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x1F\x0F\x00\xF0\x00\xFF\x22\xFD\x68\x10\x00\x02"
	"\x85\x00\xFF\x00\x64\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x00\xF0\x00\xFF\x22\x9E\x14\x10\x00\x02"
	"\x85\x00\x64\x00\x78\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x1F\x0D\x00\xF0\x00\xFF\x00\x20\x00\x00\x00\x01"
	"\x45\x00\xFF\x00\x64\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x00\xF0\x00\xFF\x00\x40\x00\x00\x00\x01"
	"\x45\x00\xFF\x00\x64\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x00\xF0\x00\xFF\x00\x40\x00\x00\x00\x01"
	"\x45\x00\xFF\x00\x64\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x00\xF0\x00\x5F\x00\x40\x00\x00\x02\x02"
	"\x85\x00\xFF\x00\x64\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x00\xF0\x00\xFF\x01\x0A\xD4\x0F\x00\x02"
	"\x41\x00\xFF\x00\x64\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x00\xF0\x00\xFF\x00\x40\x00\x00\x00\x02"
	"\x81\x00\x50\x00\xA0\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x1F\x0F\x00\xF0\x00\x2D\x42\xAC\xA0\x0F\x00\x02"
	"\x85\x00\xFF\x00\x64\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x00\xF0\x00\xFF\x01\x0B\x04\x10\x00\x02"
	"\x45\x00\xFF\x00\x64\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x00\xF0\x00\x28\x00\x40\x00\x00\x00\x00"
	"\x81\x00\xFF\x00\x90\x01\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x00\xF0\x00\xFF\x41\xED\x9F\x10\x00\x00"
	"\x40\xCC\x82\x05\xC8\x00\x00\x00\x64\x1C\x9E\x6D\x00\x00\x40\x00\x21\x00\x01\x00\xFF\x20\xF0\x01\x5F\x0C\xA0\xFC\x04\x0A\x00\x00\x00\x00\x00\x01"
	"\x40\xC8\x96\xFF\x64\x00\x28\x00\x8C\x1C\x06\x6C\x00\x00\x40\x00\x01\x00\x01\x00\xF8\x20\xDE\x01\xCC\x0C\x50\xFC\x04\x0A\x00\x00\x00\x00\x00\x01"
	"\x40\xC8\x82\x1E\x96\x00\x00\x00\x28\x0C\x9E\x6D\x03\x00\x00\x00\x21\x00\x01\x00\xF7\x20\xC0\x01\x55\x0C\x50\xFC\x04\x0A\x00\x00\x00\x00\x00\x01"
	"\x40\xC8\x82\x1E\x96\x00\x00\x00\x28\x0C\x9E\x6D\x03\x00\x00\x00\x21\x00\x01\x00\xF7\x20\xC0\x01\x55\x0C\x50\xFC\x04\x0A\x00\x00\x00\x00\x00\x01"
	"\x40\xC8\xC8\xF1\xC8\x00\x3C\x00\x14\x1C\x06\x2C\x00\x00\x40\x00\x00\x00\x01\x00\xF2\x20\xFE\x01\xDD\x0C\x50\xFC\x04\xFF\x00\x00\x00\x00\x00\x01"
	"\x40\xC8\x6E\xF6\x96\x00\x00\x00\x0A\x0C\x9E\x6D\x03\x00\x00\x00\x21\x00\x01\x00\xF7\x20\xC0\x01\x55\x0C\x50\xFC\x04\x0A\x00\x00\x00\x00\x00\x01"
	"\x40\x02\x55\x14\x90\x01\x32\x00\x78\x00\x02\x00\x00\x00\x02\x00\x00\x00\x00\x00\x53\x10\x8B\x07\x76\x08\x40\xFD\x09\xFE\x00\x00\x00\x00\x03\x02"
	"\x41\x80\xFF\x00\x84\x03\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x00\xF0\x00\xFF\x00\x60\x00\x00\x00\x01"
	"\xA0\x8C\x96\x05\x64\x00\x50\x00\x64\x00\x06\x00\x00\x00\x40\x00\x00\x00\x00\x00\x12\x20\xFF\x02\x81\x0F\x72\xFD\x04\xFF\x00\x00\x00\x00\x01\x01"
	"\x60\x88\x64\x14\x50\x00\x64\x00\x96\x00\x06\x0A\x00\x00\x10\x00\x04\x00\x00\x00\x35\x40\xFD\x03\xF6\x0F\x62\xFD\x04\x14\x00\x00\x00\x00\x01\x01"
	"\x40\x88\x32\x0A\x1E\x00\x0A\x28\xC8\x0C\x06\x08\x00\x00\x01\x00\x00\x00\x00\x00\xF3\x10\x95\x02\x11\x03\x00\xF1\x04\x0A\x00\x00\x00\x00\x01\x01"
	"\xC0\x0A\x41\x14\x24\x00\x06\x00\x50\x00\x02\x24\x01\x00\x01\x00\x00\x00\x00\x00\x34\x10\x4A\x92\xD7\x06\xFC\xFF\x04\x41\x00\x00\x00\x00\x01\x01"
	"\x80\x0A\x50\x0A\x96\x00\x28\x00\x50\x00\x02\xE4\x03\x00\x0A\x00\x02\x00\x01\x00\x7A\x30\x8A\x32\xCC\x09\x82\xFE\x04\x46\x00\x00\x00\x00\x01\x01"
	"\x80\x02\x32\x05\x5A\x00\x14\x00\xF0\x00\xA2\x64\x4F\x00\x0C\x00\x32\x00\x01\x00\xFD\x20\xDF\x02\x4F\x06\xD3\xFE\x04\x28\x00\x00\x00\x00\x01\x01"
	"\x80\x02\x46\x0A\xAF\x00\x23\x00\x6E\x00\x02\x64\x47\x00\x0D\x00\x02\x00\x01\x00\xA7\x30\x7C\x04\x55\x08\xA2\xFE\x04\x2D\x00\x00\x00\x00\x01\x01"
	"\xC0\x0A\x64\x14\xFA\x00\x50\x96\x46\x00\x02\x00\x00\x00\x01\x00\x00\x00\x00\x00\xA2\x10\xE4\x86\x72\x09\xEC\xFF\x04\x5A\x00\x00\x00\x00\x02\x02"
	"\x40\x0A\x87\x1E\x5E\x01\x32\x00\x28\x00\x02\x00\x00\x00\x01\x00\x00\x00\x00\x00\xA3\x10\xCA\xC7\x0C\x0E\x10\xFB\x04\xFF\x00\x00\x00\x00\x00\x02"
	"\x40\x1C\xAF\x05\xDC\x05\x87\x00\xAA\x08\xA2\x41\x00\x00\xD1\x0D\x5F\x00\x01\x00\xFD\x40\xEF\x1A\xDD\x0E\x62\xFD\x04\xFE\x00\x00\x00\x00\x02\x02"
	"\x40\x00\xDC\x0A\x58\x02\xA0\x00\x78\x00\x82\x40\x01\x00\x83\x01\x0A\x00\x01\x00\x68\x40\xCB\x07\x88\x0B\x41\xFC\x09\xFE\x00\x00\x00\x00\x02\x02"
	
	"\x40\x00\x78\x00\x2C\x01\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xFF\x01\xFF\x0F\x00\xF8\x00\xFF\x00\x00\x00\x00\x01\x02"
	"\x40\x02\xB4\x05\xE8\x03\x82\x00\xC8\x00\x02\x40\x0F\x00\x0B\x00\x02\x00\x01\x00\x57\x30\xEA\x08\xDC\x0C\x30\xFC\x04\x6E\x00\x00\x00\x00\x01\x01"
	"\x40\xC8\x4B\xFF\x64\x00\x08\x00\xB4\x1C\x86\x6D\x00\x00\x40\x00\x21\x00\x01\x00\xFA\x20\xEA\x04\xDF\x0C\xA0\xFC\x04\x0A\x00\x00\x00\x00\x00\x01"
	"\x40\x88\x32\x0A\x78\x00\x3C\x4B\xC8\x0C\x07\x08\x00\x00\x01\x00\x00\x00\x00\x00\xF4\x10\x95\x02\x11\x03\x00\xF0\x04\x0A\x00\x00\x00\x00\x01\x01"
	"\x40\x02\xA0\x0A\xA4\x01\x64\x00\x8C\x00\x02\x64\x0F\x00\x09\x00\x02\x00\x01\x00\x78\x30\x8C\x07\x7C\x09\x40\xFC\x04\x6E\x00\x00\x00\x00\x02\x02"
	"\x43\x00\xFF\x00\x64\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x00\xF0\x00\x46\x00\x40\x01\x00\x00\x02"
	"\x45\x00\xFF\x00\x64\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x00\xF0\x00\xFF\x00\x41\x01\x00\x00\x02"
	"\x85\x00\xFF\x00\x64\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x00\xF0\x00\xFF\x01\x0A\xE3\x0F\x00\x02"
	"\x40\x01\xDC\x00\xF4\x01\x3C\x00\xFA\x20\x02\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x01\x4F\x0F\x61\xFC\x08\xFF\x00\x00\x00\x00\x03\x02"
	"\x78\x8C\x64\x1E\x96\x00\x02\x00\x96\x00\x06\x66\x00\x00\x01\x00\x00\x00\x00\x00\x34\x40\xCF\x65\xF6\x0F\x62\xFD\x04\xFF\x00\x00\x00\x00\x01\x01"
	"\x80\x02\x41\x1E\x50\x00\x0A\x00\x5A\x00\x02\x24\x00\x00\x01\x00\x00\x00\x00\x00\x31\x10\x7D\x63\x1C\x04\x10\xFB\x04\xFE\x00\x00\x00\x00\x00\x02"
	"\x40\xC8\x4B\x05\x96\x00\x08\x00\xB4\x1C\x9E\x6D\x00\x00\x40\x00\x21\x00\x01\x00\xFF\x20\xE9\x03\x7F\x0C\xA0\xFC\x04\x0A\x00\x00\x00\x00\x00\x01"
	"\x40\x02\x73\x0A\xAF\x00\x5A\x00\x64\x00\x02\x64\x4B\x00\x09\x00\x08\x00\x01\x00\x45\x30\xEE\x15\x55\x0A\xA2\xFE\x04\x46\x00\x00\x00\x00\x01\x01"
	"\x81\x00\xFF\x00\xBE\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x00\xF0\x00\xFF\x42\x8C\xA0\x0F\x00\x02"
	"\x40\x02\x46\x1E\x32\x00\x23\x00\x3C\x00\x02\x44\x4F\x00\x09\x00\x0A\x00\x01\x00\x24\x10\xE2\x25\x3F\x1F\xA2\xFE\x04\x32\x00\x00\x00\x00\x01\x01"
	"\x44\x20\xFF\x00\x20\x03\x00\x00\xFF\x20\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x61\xFC\x08\xFF\x00\x00\x00\x00\x00\x01"
	"\x90\x02\x64\x05\x96\x00\x0F\x00\xA0\x00\x02\x40\x0F\x00\xF9\x05\x37\x00\x01\x00\x9E\x30\xE6\x85\x6F\x0C\x93\xFD\x04\x28\x00\x00\x00\x00\x01\x01"
	"\x40\xC8\x4B\x00\x64\x00\x32\x00\xB4\x0C\x06\x2C\x00\x00\x40\x00\x21\x00\x01\x00\xFF\x20\xFE\x03\x7F\x0C\xA0\xFC\x04\x0A\x00\x00\x00\x00\x00\x01"
	"\x40\x02\x73\x0A\xAF\x00\x3C\x00\x64\x08\x02\x64\x4F\x00\x09\x00\x08\x00\x01\x00\x14\x30\xE8\x14\x85\x0F\xA2\xFE\x04\x37\x00\x00\x00\x00\x01\x01"
	"\x41\x00\xFF\x00\x64\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x00\xF0\x00\x32\x00\x40\x00\x00\x00\x01"
	"\x80\x02\x5A\x14\x96\x00\x1E\x00\x82\x00\x02\xC0\x01\x00\x01\x00\x00\x00\x00\x00\xC7\x10\xDA\x03\x72\x06\xC5\xFE\x04\x41\x00\x00\x00\x00\x02\x02"
	"\xB8\xAC\xFF\x0A\xC8\x00\x00\x00\xFF\x40\x04\x0C\x00\x00\x00\x00\x00\x00\x00\x00\xFF\x40\xC7\x01\xFF\x0F\x62\xFD\x04\xFF\x00\x00\x00\x00\x01\x01"
	"\xB8\xAC\xFF\x0A\xC8\x00\x00\x00\xFF\x00\x02\x48\x00\x00\x00\x00\x60\x00\x00\x00\xFF\x40\xC7\x01\xFF\x0F\x62\xFD\x04\xFF\x00\x00\x00\x00\x01\x01"
	"\x90\x02\x8C\x05\xFA\x00\x50\x00\xB8\x00\x02\x40\x0F\x00\x10\x00\x37\x00\x01\x00\xAF\x30\xB9\x55\xAF\x0D\x93\xFD\x04\x32\x00\x00\x00\x00\x01\x01"
	"\x81\x80\xFF\x00\x84\x03\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x00\xF0\x00\xFF\x00\x60\x00\x00\x00\x01"
	"\x40\x02\x64\x0A\xA0\x00\x3C\x00\x78\x00\x02\x64\x4B\x00\x0D\x00\x08\x00\x01\x00\x75\x30\x8C\x15\x55\x0A\xA2\xFE\x04\x4B\x00\x00\x00\x00\x01\x01"
	"\x80\x08\x6E\x01\x5E\x01\x00\x00\x0A\x0C\x02\x00\x01\x00\x00\x00\x00\x00\x00\x00\xFF\x10\xFF\x03\x9C\x1F\x50\xFC\x04\x28\x00\x00\x00\x00\x00\x01"
	"\x45\x00\x8C\x00\x04\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x3F\x0F\x00\xF0\x00\x37\x00\x60\x00\x00\x00\x02"
	"\x85\x00\xFF\x00\x64\x00\x00\x00\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xFF\x0F\x00\xF0\x00\xFF\x00\x40\x00\x00\x00\x02"
	"\x78\x8C\x64\x1E\x96\x00\x28\x00\x96\x0D\x06\x66\x00\x00\x01\x00\x00\x00\x00\x00\x34\x40\xC6\x44\xF6\x0F\x62\xFD\x04\xFF\x00\x00\x00\x00\x01\x01"
// SPX: I think the AI def don't go further than this	, then, are these data used for something ?? (=62 rows)
	"\xFF\xFF\x06\x00\x00\x01\x01\x00\x01\x00\x00\x01\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x01\x01\xFF\x81\xFF\x81\xFF\x81\xFF\x81\xB0\x01\x00\x00\x80\x00\xFC\x81\x80\x01\xE0\x01\x00\x01\xFF\x81"
	"\xFF\x81\xFF\x81\xFF\x81\xFF\x81\x0E\x18\xFF\xFF\x0F\x10\x11\x12\x13\x14\x15\xFF\xFF\xFF\xFF\x0D\x00\x00\x97\x09\x08\x00\x97\x09\x23\x00\x97\x09"
	"\x3E\x00\x97\x09\x7B\x00\x97\x09\x9E\x00\x97\x09\xC5\x00\x97\x09\xF3\x00\x97\x09\x2D\x01\x97\x09\x4F\x01\x97\x09\x84\x01\x97\x09\xBA\x01\x97\x09"
	"\x0B\x00\x00\x01\x00\x00\x00\x00\x00\x00\x5C\x00\x01\x01\x00\x00\x00\x00\x00\x00\xA1\x00\x01\x01\x00\x00\x00\x00\x00\x00\xA2\x00\x01\x01\x00\x00"
	"\x00\x00\x00\x00\xA3\x00\x01\x01\x00\x00\x00\x00\x00\x00\xA4\x00\x01\x01\x00\x00\x00\x00\x00\x00\x09\x00\x00\x01\x00\x00\x00\x00\x00\x00\x07\x00"
	"\x01\x0F\x00\x00\x00\x00\x00\x00\x07\x00\x01\x0F\x00\x60\x00\x00\x00\x00\x3B\x82\x00\x0F\x00\x00\x00\x00\x00\x00\xEE\x81\x00\x0F\x00\x00\x00\x00"
	"\x00\x00\x2F\x00\x02\x0F\x00\x40\x00\x00\x00\x00\x0B\x00\x00\x01\x00\x00\x00\x00\x00\x00\x9C\x00\x00\x02\x00\x00\x00\x00\x00\x00\x9D\x00\x00\x02"
	"\x00\x00\x00\x00\x00\x00\x9E\x00\x00\x02\x00\x00\x00\x00\x00\x00\x9F\x00\x00\x02\x00\x00\x00\x00\x00\x00\xEE\x81\x00\x0F\x00\x00\x00\x00\x00\x00"
	"\xD7\x80\x97\x01\x02\x80\xD8\x00\x97\x01\x10\x40\xD9\x00\x99\x01\x02\x00\xE0\x00\xB2\x01\x02\x00\xDA\x00\x9B\x01\x02\x00\xEF\x80\x02\x00\x02\x80"
	"\xEF\x00\x02\x00\x01\x40\x92\x80\x07\x00\x02\x80\x92\x00\x02\x00\x01\x40\x93\x00\x0D\x00\x02\x40\x93\x80\x0D\x00\x02\x40\x52\x00\x02\x00\x01\x40"
	"\x94\x80\x02\x00\x02\x80\x94\x00\x02\x00\x01\x40\x0B\x80\xA1\x00\x02\x80\x07\x80\xA1\x00\x01\x40\x07\x00\x9C\x00\x02\x80\xEA\x00\xB9\x00\x02\x80"
	"\x14\x00\xD1\x00\x02\x80\x15\x00\xD2\x00\x02\x00\x07\x80\x9C\x00\x02\x80\x0B\x80\xA2\x00\x02\x80\x08\x80\xA2\x00\x01\x40\xEB\x00\xBA\x00\x02\x80"
	"\x16\x00\xD3\x00\x02\x80\x17\x00\xD4\x00\x02\x00\x08\x80\x9D\x00\x02\x80\x0B\x80\xA3\x00\x02\x80\x09\x80\xA3\x00\x01\x40\xEC\x00\xBB\x00\x02\x80"
	"\x18\x00\xD5\x00\x02\x80\x19\x00\xD6\x00\x02\x00\x09\x80\x9E\x00\x02\x80\x0B\x80\xA4\x00\x02\x80\x0A\x80\xA4\x00\x01\x40\xED\x00\xBC\x00\x02\x80"
	"\x1A\x00\xD7\x00\x02\x80\x1B\x00\xD8\x00\x02\x00\x0A\x80\x9F\x00\x02\x80\x01\x80\x28\x00\x02\x80\x03\x00\x2A\x00\x02\x00\x02\x00\x29\x00\x02\x00"
	"\x06\x00\x2D\x00\x02\x00\x05\x00\x2C\x00\x02\x00\x04\x00\x2B\x00\x02\x00\x50\x80\x07\x00\x02\x10\x50\x80\x07\x00\x02\x10\x8C\x80\x32\x82\x02\x80"
	"\x91\x00\x34\x82\x02\x00\x0B\x00\x36\x82\x02\x00\x0B\x00\x02\x00\x01\x40\x1C\x80\xFB\x81\x02\x80\x1D\x00\xFC\x81\x02\x00\x1E\x00\xFD\x81\x02\x00"
	"\x1F\x00\xFE\x81\x02\x00\x20\x00\xFF\x81\x02\x00\x21\x00\x00\x82\x02\x00\x22\x00\x01\x82\x02\x00\x46\x00\x21\x82\x02\x00\x47\x00\x22\x82\x02\x00"
	"\x23\x00\x02\x82\x02\x00\x24\x00\x03\x82\x02\x00\x25\x00\x04\x82\x02\x00\x26\x00\x05\x82\x02\x00\x27\x00\x06\x82\x02\x00\x28\x00\x07\x82\x02\x00"
	"\x29\x00\x08\x82\x02\x00\x2A\x00\x09\x82\x02\x00\x2B\x00\x0A\x82\x02\x00\x2C\x00\x0B\x82\x02\x00\x2D\x00\x0C\x82\x02\x00\x2E\x00\x0D\x82\x02\x00"
	"\x2F\x00\x0E\x82\x02\x00\x30\x00\x0F\x82\x02\x00\x31\x00\x10\x82\x02\x00\x32\x00\x11\x82\x02\x00\x33\x00\x12\x82\x02\x00\x34\x00\x13\x82\x02\x00"
	"\x35\x00\x14\x82\x02\x00\x36\x00\x15\x82\x02\x00\x37\x00\x16\x82\x02\x00\x38\x00\x17\x82\x02\x00\x39\x00\x18\x82\x02\x00\xA1\x80\x36\x82\x02\x80"
	"\xA1\x00\x02\x00\x01\x40\xA0\x80\xEE\x81\x02\x80\x7D\x80\x31\x00\x02\x80\x7E\x00\x32\x00\x02\x80\x7F\x00\x33\x00\x02\x80\x80\x00\x34\x00\x02\x80"
};
