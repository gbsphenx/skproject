#ifndef _SKTYPES_H_
#define _SKTYPES_H_

//#pragma once

//------------------------------------------------------------------------------

#include <assert.h>

//------------------------------------------------------------------------------
// WINDOWS / MSVC6 / MSVC8

#if !defined(SKDOSV5) && (defined (_WINDOWS) || (_MSC_VER >= 1200) || defined(_USE_MFC80) || defined(_USE_MFC60))
#include <stdafx.h>
#endif

#ifndef SKDOSV5
#if defined (_WINDOWS) || (_MSC_VER >= 1200) || defined(_USE_MFC80) || defined(_USE_MFC60)
#include <windows.h>		// also for UINT
typedef __int8				int8_t;
typedef __int16				int16_t;
typedef __int32				int32_t;
typedef unsigned __int8		uint8_t;
typedef unsigned __int16	uint16_t;
typedef unsigned __int32	uint32_t;
//typedef unsigned __int32	uint_t;

#endif
#endif // SKDOSV5
//..............................................................................
// LINUX

#if defined(__LINUX__)
#include <assert.h>
typedef signed char		int8_t;
typedef signed short	int16_t;
typedef signed int		int32_t;
typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;
typedef unsigned int	uint_t;

	#if defined(__LP64__) || defined (_LP64)
	#include <stdint.h>
	#endif

#endif // __LINUX__

//..............................................................................
// MINGW / DOS (DJGPP)

#if defined(__DJGPP__) || defined(__MINGW__)
#include <assert.h>
typedef signed char		int8_t;
typedef signed short	int16_t;
typedef signed int		int32_t;
typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;
typedef unsigned int	uint_t;
#endif // __DJGPP__



//------------------------------------------------------------------------------
// Generic

#if !defined(_MSC_VER)	// defined from winatl.h for win version
	#define ATLASSERT(X)		assert(X)
	#define ATLVERIFY(X)		(X)
#endif // MFC

//#define min(A,B) ((A < B) ? A : B)
//#define max(A,B) ((A < B) ? B : A)

/// BEWARE, currently defined within DME.h => better here
// Signed (i)nteger
#ifndef SKDOSV5
typedef int8_t		i8;
typedef int16_t		i16;
typedef int32_t		i32;

// (U)nsigned integer
typedef uint8_t		U8;
typedef uint16_t	U16;
typedef uint32_t	U32;

// Sign indetermined integer. Either signed or unsigned. Treat as unsigned in default.
typedef uint8_t		X8;
typedef uint16_t	X16;
typedef uint32_t	X32;


typedef uint8_t				Bit8u;
typedef uint16_t			Bit16u;
typedef uint32_t			Bit32u;
typedef unsigned			Bitu;
typedef signed				Bits;
#endif // SKDOSV5

#ifdef SKDOSV5
typedef signed char		i8;
typedef signed short	i16;
typedef signed int		i32;
typedef unsigned char	X8;
typedef unsigned short	X16;
typedef unsigned int	X32;
typedef unsigned char	U8;
typedef unsigned short	U16;
typedef unsigned int	U32;
typedef unsigned int	UINT;
//typedef int	INT;
#endif

//------------------------------------------------------------------------------

#if !defined(WIN32) && !defined (_WINDOWS) && !defined(_USE_MFC80) && !defined(_USE_MFC60)
typedef uint32_t	UINT;	// generic unsigned int for RC, index & loops
#elif defined (__MINGW__)
typedef uint32_t	UINT;
typedef int32_t		INT;
#endif // Win MSVC6 UINT is (unsigned int)


#if defined (__LINUX__)
typedef int32_t	INT;
#endif

//------------------------------------------------------------------------------
// For Win, LPCSTR is in WINNT.H
#if defined(__MINGW__) || defined(__DJGPP__) || defined(__LINUX__)
#ifndef LPCSTR
	#define LPCSTR const char*
#endif // LPCSTR
#endif

//------------------------------------------------------------------------------
// Compensation for V5 non-MFC/windows.h

//#if defined(SKDOSV5) && (defined (_WINDOWS) || (_MSC_VER >= 1200) || defined(_USE_MFC80) || defined(_USE_MFC60))
#if !defined(SKDOSV5) && ((defined (_WINDOWS) || (_MSC_VER >= 1200) || defined(_USE_MFC80) || defined(_USE_MFC60)))
//typedef uint32_t	UINT;	// generic unsigned int for RC, index & loops
#define LPCSTR const char*
#endif

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// SPECIFIC DM2 TYPES

// Object ID (for object reference), original default is 2 bytes. Extended would
// require more like 4 bytes, but that would cause impact in all data structures.
#define OID_T							U16

//..............................................................................

#define CALL_IBMIO 
#define INDIRECT_CALL
#define LOADDS(X)
#define ENTER(X)

//------------------------------------------------------------------------------

#endif // _SKTYPES_H_