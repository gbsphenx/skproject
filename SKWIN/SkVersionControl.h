#ifndef _SK_VERSION_CONTROL_H_
#define _SK_VERSION_CONTROL_H_

//------------------------------------------------------------------------------

// SPHENX: Define DM2_EXTENDED_MODE to change behaviour over the original code
#define	DM2_EXTENDED_MODE		1	// Load custom GDAT categories, enhance some code
#define DM2_EXTENDED_MAP		0	// Load a +128 maps test dungeon. (to increase maps from 64 to 256). Use Conv2XMAP to get DUNGEON_XMAP.DAT
#define DM2_EXTENDED_DATABASE	1	// Make usage of db11 db12 and db13 - partially implemented (tested with db11)
#define DM2_EXTENDED_OBJECT_DEF	0	// Allow OBJECT structure to hold more items per database - EXPERIMENTAL
#define DM2_EXTENDED_BUFFER		1	// Allow more memsize for loading objects
//#define DM2_INDIRECT_DATABASE	0	// Use an indirection table to increase item usage to its max : 65536 - NOT IMPLEMENTED YET
#define DM2_FULL_JAPANESE		1	// That must be activated to load custom GDAT

#define DM2_DEBUG_SUPER_MODE	1	// If on, champions at start will have big stats. Debug/test purpose.

//------------------------------------------------------------------------------

// Standard Releases
#define RELEASE_FREE		0	// Standard working debug exe
#define RELEASE_EN_JP		1
#define RELEASE_JP_JP		2
#define RELEASE_EXT_MOD		3
#define RELEASE_EXT_SUPER	4

#define CURRENT_RELEASE		RELEASE_FREE
//	SKWin.Release_EN_JP.exe

#if (CURRENT_RELEASE == RELEASE_EN_JP)
	#undef DM2_EXTENDED_MODE
	#define DM2_EXTENDED_MODE		0
	#undef DM2_FULL_JAPANESE
	#define DM2_FULL_JAPANESE		0
	#undef DM2_DEBUG_SUPER_MODE
	#define DM2_DEBUG_SUPER_MODE	0
#elif (CURRENT_RELEASE == RELEASE_JP_JP)
	#undef DM2_EXTENDED_MODE
	#define DM2_EXTENDED_MODE		0
	#undef DM2_FULL_JAPANESE
	#define DM2_FULL_JAPANESE		1
	#undef DM2_DEBUG_SUPER_MODE
	#define DM2_DEBUG_SUPER_MODE	0
#elif (CURRENT_RELEASE == RELEASE_EXT_MOD)
	#undef DM2_EXTENDED_MODE
	#define DM2_EXTENDED_MODE		1
	#undef DM2_FULL_JAPANESE
	#define DM2_FULL_JAPANESE		1
	#undef DM2_DEBUG_SUPER_MODE
	#define DM2_DEBUG_SUPER_MODE	0
#elif (CURRENT_RELEASE == RELEASE_EXT_SUPER)
	#undef DM2_EXTENDED_MODE
	#define DM2_EXTENDED_MODE		1
	#undef DM2_FULL_JAPANESE
	#define DM2_FULL_JAPANESE		1
	#undef DM2_DEBUG_SUPER_MODE
	#define DM2_DEBUG_SUPER_MODE	1
#endif


//------------------------------------------------------------------------------

#endif // _SK_VERSION_CONTROL_H_