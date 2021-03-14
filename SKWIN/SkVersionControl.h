#ifndef _SK_VERSION_CONTROL_H_
#define _SK_VERSION_CONTROL_H_

//------------------------------------------------------------------------------

// SPHENX: Define DM2_EXTENDED_MODE to change behaviour over the original code
#define	DM2_EXTENDED_MODE		1	// Load custom GDAT categories, enhance some code
#define DM2_EXTENDED_MAP		0	// Load a +128 maps test dungeon. (to increase maps from 64 to 256)
#define DM2_EXTENDED_DATABASE	1	// Make usage of db11 db12 and db13 - partially implemented (tested with db11)
#define DM2_EXTENDED_OBJECT_DEF	0	// Allow OBJECT structure to hold more items per database - EXPERIMENTAL
#define DM2_EXTENDED_BUFFER		1	// Allow more memsize for loading objects
#define DM2_EXTENDED_LIGHT		0	// Some custom day/night light for outdoors - EXPERIMENTAL : not working because palette is applied over global viewport
//#define DM2_INDIRECT_DATABASE	0	// Use an indirection table to increase item usage to its max : 65536 - NOT IMPLEMENTED YET

#define DM2_DEBUG_SUPER_MODE	1	// If on, champions at start will have big stats. Debug/test purpose.

//------------------------------------------------------------------------------

// Debug Build
#define RELEASE_FREE		0	// Standard working debug exe 

// Standard Releases
#define RELEASE_STD			1	// Release Standard
#define RELEASE_EXT_MOD		2	// Release Extended mode
#define RELEASE_EXT_SUPER	3	// Release Extended mode + Super champions for debug/test

#define CURRENT_RELEASE		RELEASE_EXT_MOD

#if (CURRENT_RELEASE == RELEASE_STD)
	#undef DM2_EXTENDED_MODE
	#define DM2_EXTENDED_MODE		0
	#undef DM2_EXTENDED_MAP
	#define DM2_EXTENDED_MAP		0
	#undef DM2_EXTENDED_BUFFER
	#define DM2_EXTENDED_BUFFER		0
	#undef DM2_EXTENDED_DATABASE
	#define DM2_EXTENDED_DATABASE	0
	#undef DM2_DEBUG_SUPER_MODE
	#define DM2_DEBUG_SUPER_MODE	0
#elif (CURRENT_RELEASE == RELEASE_EXT_MOD)
	#undef DM2_EXTENDED_MODE
	#define DM2_EXTENDED_MODE		1
	#undef DM2_EXTENDED_MAP
	#define DM2_EXTENDED_MAP		0
	#undef DM2_EXTENDED_BUFFER
	#define DM2_EXTENDED_BUFFER		1
	#undef DM2_EXTENDED_DATABASE
	#define DM2_EXTENDED_DATABASE	1
	#undef DM2_DEBUG_SUPER_MODE
	#define DM2_DEBUG_SUPER_MODE	0
#elif (CURRENT_RELEASE == RELEASE_EXT_SUPER)
	#undef DM2_EXTENDED_MODE
	#define DM2_EXTENDED_MODE		1
	#undef DM2_EXTENDED_MAP
	#define DM2_EXTENDED_MAP		0
	#undef DM2_EXTENDED_BUFFER
	#define DM2_EXTENDED_BUFFER		1
	#undef DM2_EXTENDED_DATABASE
	#define DM2_EXTENDED_DATABASE	1
	#undef DM2_DEBUG_SUPER_MODE
	#define DM2_DEBUG_SUPER_MODE	1
#endif


//------------------------------------------------------------------------------

#endif // _SK_VERSION_CONTROL_H_