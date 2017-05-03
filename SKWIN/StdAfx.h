
#if defined(_USE_MFC80) || defined(_USE_MFC60)
// MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>MFC>
#pragma once

#define WINVER 0x0410
#define DIRECTSOUND_VERSION 0x800

#include <afx.h>
#include <afxwin.h>
#ifdef _USE_MFC80
# include <atlcore.h>
# include <atlcoll.h>
# include <atlstr.h>
# include <atlcom.h>
#else
# include <atlbase.h>
extern CComModule _Module;
# include <atlcom.h>
# define ATLVERIFY VERIFY // use mfc's one
#endif
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <setjmp.h>
#include <time.h>
#include <direct.h>
#include <io.h>
#include <fcntl.h>
#include <mmsystem.h>
#include <io.h>
#include <sys/stat.h>
#include <stack>
#include <vector>

#include <afxcmn.h>

// You require: Microsoft DirectX 9.0 SDK Update (October 2004)
#include <dsound.h>

typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;

// You require: OpenAL 1.1 SDK
#include <al.h>
#include <alc.h>

typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned uint_t;

// <MFC<MFC<MFC<MFC<MFC<MFC<MFC<MFC<MFC<MFC<MFC<MFC<MFC<MFC<MFC<MFC<MFC<MFC<MFC
#endif // _USE_MFC80

#ifdef _USE_SDL
// SDL>SDL>SDL>SDL>SDL>SDL>SDL>SDL>SDL>SDL>SDL>SDL>SDL>SDL>SDL>SDL>SDL>SDL>SDL>

#include <SDL.h>
#include <assert.h>
#include <io.h>
#include <fcntl.h>
#include <setjmp.h>
#include <direct.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stack>

#ifndef _STDINT_H
//typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned uint_t;
#endif // _STDINT_H

#define ATLASSERT(X) assert(X)
#define ATLVERIFY(X) (X)
#define min(A,B) ((A < B) ? A : B)
#define max(A,B) ((A < B) ? B : A)

// <SDL<SDL<SDL<SDL<SDL<SDL<SDL<SDL<SDL<SDL<SDL<SDL<SDL<SDL<SDL<SDL<SDL<SDL<SDL
#endif

typedef uint8_t Bit8u;
typedef uint16_t Bit16u;
typedef uint32_t Bit32u;

typedef unsigned Bitu;

typedef signed Bits;

#include "MemVuff.h"
