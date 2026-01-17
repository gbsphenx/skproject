//--- SkWinCoreEOB -------------------------------------
//SPX: Experimental code about Eye of the Beholder dungeon reading

#include <StdAfx.h>	// Required for MVC6 compilation

#include <SkCodeParam.h>
#include <SkWinDebug.h>
#include <SkGlobal.h>
#include <SkWinCore.h>
#include <SkWinMIDI.h>

//--- Common part with A.cpp
#define UseAltic 1

using namespace DMEncyclopaedia;
using namespace DM2Internal;
using namespace kkBitBlt;

#if defined(_USE_MFC80) || defined(_USE_MFC60)
#include "SkWinMFC.h"
#include "resource.h"
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)


#ifdef _USE_SDL
#include "SkwinSDL.h"
#endif // _USE_SDL

