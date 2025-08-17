#ifndef _SKWIN_LUA_H_
#define _SKWIN_LUA_H_

//------------------------------------------------------------------------------

//#include <SkVersionControl.h>
class SkWinCore;

//------------------------------------------------------------------------------

float SkWinLua_Init_Lua(SkWinCore* xSkCore, char* sVersionName);
bool SkWinLua_RunFunctionFromScript(SkWinCore* xSkCore, const char* sScriptFilename, const char* sFunctionName, int iArg1, int iArg2, int iArg3, int iArg4);

#endif // _SKWIN_LUA_H_