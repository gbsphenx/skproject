#ifndef _SK_VERSION_CONTROL_H_
#define _SK_VERSION_CONTROL_H_

//------------------------------------------------------------------------------

// Alternate
#define UseAltic 1

//------------------------------------------------------------------------------

//#define _BUILD_DATE_	__DATE__
//#define _BUILD_TIME_	__TIME__

// Activate this for preparing public release version
//#define __SKWIN_PUBLIC_VERSION__	1
#define __SKWIN_RELEASE_DATE__ "2024-11-17"

// There are 3 different define possible at project level
// SK9821V4	-> 9821 + SKULL V4
// SKDOSV5	-> PC-DOS + SKULL V5
// SKWINSPX	-> WIN-SPX + SKULL V6

// BASE PROGNAME SKWIN VS. SKULL
#if defined (__DJGPP__)
	#define __SKWIN_BASEPROGNAME__ "SKULL"
#else
	#define __SKWIN_BASEPROGNAME__ "SKWIN"
#endif // __DJGPP__

// PROG MAJOR VERSION
#if defined (SK9821V4)
	#define __SKULL_SUBVER__	"-V4"
	#define __SKWIN_SUBVER__	"-9821"
#elif defined (SKDOSV5)
	#define __SKULL_SUBVER__	"-V5"
	#define __SKWIN_SUBVER__	"-DOS"
#elif defined (SKWINSPX)
	#define __SKULL_SUBVER__	"-V6"
	#define __SKWIN_SUBVER__	"-SPX"
#endif // SUBVER

// INTERNAL PROGNAME
#if defined (__DJGPP__)
	#define __SKWIN_PROGNAME__ __SKWIN_BASEPROGNAME__ "" __SKULL_SUBVER__
#elif defined (__MINGW__)
	#define TOSTRING(x) #x
	#define __SKWIN_PROGNAME__ TOSTRING(__SKWIN_BASEPROGNAME__) "" TOSTRING(__SKWIN_SUBVER__)
#else
	#define __SKWIN_PROGNAME__ __SKWIN_BASEPROGNAME__ "" __SKWIN_SUBVER__
#endif

// COMPILATION TARGET
#if defined (__DJGPP__)
	#define __SKWIN_SYSTEM__ "DOS"
#elif defined (__MINGW__)
	#define __SKWIN_SYSTEM__ "MINGW32"
#elif defined (__LINUX__) && (defined(__x86_64__) || defined(__amd64__))
	#define __SKWIN_SYSTEM__ "LINUX64"
#elif defined (__LINUX__)
	#define __SKWIN_SYSTEM__ "LINUX"
#else
	#define __SKWIN_SYSTEM__ "WIN32"
#endif


//------------------------------------------------------------------------------

#define _SK_DOSFILENAME__		1	// Keep filenames/foldernames compatible DOS 8.3, expected to ensure MSDOS/FreeDOS build
									// Can also be used for Win build

#define _SK_DYNAMIC_CEMS		1	// Do not allocate static CEMS mem (32 to 64 MB); allows to keep normal exe size with DJGPP

//------------------------------------------------------------------------------

// SPHENX: Define DM2_EXTENDED_MODE to change behaviour over the original code
#define	DM2_EXTENDED_MODE		1	// Load custom GDAT categories, enhance some code
#define DM2_EXTENDED_MAP		0	// Load a +128 maps test dungeon. (to increase maps from 64 to 256)
#define DM2_EXTENDED_DATABASE	1	// Make usage of db11 db12 and db13 - partially implemented (tested with db11)
#define DM2_EXTENDED_OBJECT_DEF	0	// Allow OBJECT structure to hold more items per database; uses 64bits Ref  - EXPERIMENTAL
#define DM2_EXTENDED_BUFFER		1	// Allow more memsize for loading objects
#define DM2_EXTENDED_LIGHT		0	// Some custom day/night light for outdoors - EXPERIMENTAL : not working because palette is applied over global viewport
//#define DM2_INDIRECT_DATABASE	0	// Use an indirection table to increase item usage to its max : 65536 - NOT IMPLEMENTED YET

//#define DM2_DEBUG_SUPER_MODE	1	// If on, champions at start will have big stats. Debug/test purpose.

//------------------------------------------------------------------------------

#define XDM2_EXTENDED_SPELLS_TAB	1	// For using custom spells defined within graphics.dat
#define XDM2_EXTENDED_AI_TABSIZE	1
#define XDM1_EXTENDED_SEETHRUWALLS	1

//------------------------------------------------------------------------------

// Debug Build
//#define RELEASE_FREE		0	// Standard working debug exe 

// Standard Releases
//#define RELEASE_STD			1	// Release Standard
//#define RELEASE_EXT_MOD		2	// Release Extended mode
//#define RELEASE_EXT_SUPER	3	// Release Extended mode + Super champions for debug/test

// 2024 : REMOVE ALL THIS, NOT NEEDED ANYMORE

//#define CURRENT_RELEASE		RELEASE_EXT_MOD

/*
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
*/

//------------------------------------------------------------------------------

#endif // _SK_VERSION_CONTROL_H_
