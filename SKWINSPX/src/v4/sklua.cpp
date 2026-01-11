#include <StdAfx.h>
#include <sktypes.h>
#include <sklua.h>

#include <string.h>
#include <skglobal.h>
#include <skcore.h> // skwincore CHANGE_CONSOLE_COLOR
#include <skdebug.h>

#define CHANGE_CONSOLE_COLOR(x,y,z) ;;;

//------------------------------------------------------------------------------

#define _DISPLAY_LUA_ERROR_	0

//------------------------------------------------------------------------------

#if !defined(__DJGPP__) && !defined(__MINGW__) && !defined(__LINUX__)

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#endif

//// To remove MinGW warnings: "warning: deprecated conversion from string constant to 'char*'"
#if defined (__MINGW__)
#define STR_TO_CHAR_COMPAT_MINGW	(char*)
#else
#define STR_TO_CHAR_COMPAT_MINGW
#endif

//------------------------------------------------------------------------------

static bool	bLUAValid = false;
SkWinCore*	xCurrentGame = NULL;

//------------------------------------------------------------------------------

// no LUA there ...
#if defined(__DJGPP__) || defined(__MINGW__) || defined(__LINUX__)
#define lua_State void

float SkWinLua_Init_Lua(SkWinCore* xSkCore, char* sVersionName)
{
	return 0;
}

void SkWinLua_Bail(lua_State *L, char *msg, const char* sScriptName, const char* sFunctionName)
{
	return;
}

bool SkWinLua_RunScript(const char* sScriptFilename)
{
	return false;
}

bool SkWinLua_RunFunctionFromScript(SkWinCore* xSkCore, const char* sScriptFilename, const char* sFunctionName, int iArg1, int iArg2, int iArg3, int iArg4)
{
	return false;
}

#else // LUA code

//------------------------------------------------------------------------------

float SkWinLua_Init_Lua(SkWinCore* xSkCore, char* sVersionName)
{
	const char* sLuaRelease = LUA_RELEASE;
	const char* sLuaVersion = LUA_VERSION;
	float fVersion = atof(&sLuaVersion[4]);
	bool	bLuaDynamicLibLoaded = false;
	char sLocalVersionName[128];

	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	SkD((DLV_DBG_LUA, "LuaInit: initializing ...\n"));

	xCurrentGame = xSkCore;

	if (sVersionName == NULL)
		sVersionName = sLocalVersionName;

	strcpy(sVersionName, sLuaRelease+4);	// The form is "Lua M.m.r", and I don't want Lua_

	//bLuaDynamicLibLoaded = ZDynamicLib::CheckAndReloadDynamicLibrary("lua5.1");
	bLuaDynamicLibLoaded = true;

	if (bLuaDynamicLibLoaded)
	{
		lua_State *L;
		L = lua_open();

		luaL_openlibs(L);
		lua_getglobal(L, "_VERSION");
		unsigned int size = 0;
		char sString[64];
		strcpy(sString, lua_tolstring(L, -1, &size));
		lua_close(L);
		if (sString[0] != 0)
		{
			strcat(sVersionName, " - Runtime ");
			strcat(sVersionName, sString);
			bLUAValid = true;
		}
		else
			strcat(sVersionName, " - No runtime found.");
	}
	else if (!bLuaDynamicLibLoaded)
	{
		fVersion = 0;
	}
	SkD((DLV_DBG_LUA, "LuaInit: version = %s\n", sVersionName));
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	return fVersion;
}



//------------------------------------------------------------------------------

//--- Test function for print
static int SkWinLua_Print(lua_State *L)
{
	double arg1 = luaL_checknumber (L, 1);
	fprintf(stdout, "Message from LUA function.\n");

	return 0;
}


void SkWinLua_Bail(lua_State *L, char *msg, const char* sScriptName, const char* sFunctionName)
{
#if(_DISPLAY_LUA_ERROR_==1)
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_RED, BLACK);
	fprintf(stderr, "\nFATAL ERROR:\n  %s: %s || (%s | %s)\n",
		msg, lua_tostring(L, -1), sScriptName, sFunctionName);
	fprintf(stderr, "LUA failed. %s: %s || (%s | %s)\n", msg, lua_tostring(L, -1), sScriptName, sFunctionName);
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	//exit(1);
#endif // _DISPLAY_LUA_ERROR_
}


// Generic Command from DME Scripts
static int DME_GenericCommand(lua_State *L)
{
	int iCommandReturnValue = 0;
	//--- Generic command takes
	//-- String1 : Object Type / Scope
	//-- String2 : Command
	//-- String3 : Sub Command / Name / Attribute
	//-- Integer1 : map
	//-- Integer2 : x
	//-- Integer3 : y
	//-- Integer4 : facing / direction
	//-- Integer5 : value
	const char *object_scope = luaL_checkstring(L, 1);
	const char *command = luaL_checkstring(L, 2);
	const char *subcommand = luaL_checkstring(L, 3);
	double arg1 = luaL_checknumber (L, 4);
	double arg2 = luaL_checknumber (L, 5);
	double arg3 = luaL_checknumber (L, 6);
	double arg4 = luaL_checknumber (L, 7);
	double arg5 = luaL_checknumber (L, 8);
	//lua_pushnumber(L, arg2);

	//CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	SkD((DLV_DBG_LUA, "LuaScript GenericCommand : %s %s %s %d %d %d %d %d => %d\n", command, object_scope, subcommand, (int)arg1, (int)arg2, (int)arg3, (int)arg4, (int)arg5, iCommandReturnValue));
	//fprintf(stdout, "LuaScript GenericCommand : %s %s %s %d %d %d %d %d => %d\n", command, object_scope, subcommand, (int)arg1, (int)arg2, (int)arg3, (int)arg4, (int)arg5, iCommandReturnValue);
	//CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);

	if (MVALID(xCurrentGame))
		iCommandReturnValue = xCurrentGame->EXT_PROCEED_DCS_GENERIC_COMMAND(command, object_scope, subcommand, (int)arg1, (int)arg2, (int)arg3, (int)arg4, (int)arg5);


	lua_pushnumber(L, iCommandReturnValue);

	return 1;	// return 1 result
}

//------------------------------------------------------------------------------

bool
SkWinLua_RunScript(const char* sScriptFilename)
{
	if (!bLUAValid)
	{
		fprintf(stderr, "LUA context is not available. Can't run script <%s>\n", sScriptFilename);
		return false;
	}

	//---
	{
		lua_State *L;
		L = lua_open();

		luaL_openlibs(L);

		//lua_pushcfunction(L, SkWinLua_Print);
		//lua_setglobal(L, "SkWinLua_Print");

		lua_pushcfunction(L, DME_GenericCommand);
		lua_setglobal(L, "DME_GenericCommand");
		

		if(luaL_loadfile(L, sScriptFilename))
			SkWinLua_Bail(L, STR_TO_CHAR_COMPAT_MINGW "luaL_loadfile() failed", sScriptFilename, "-");

		//--- Priming run
		//lua_pcall(L, 0, 0, 0);
		if (lua_pcall(L, 0, 0, 0))
			SkWinLua_Bail(L, STR_TO_CHAR_COMPAT_MINGW "lua_pcall() failed", sScriptFilename, "-");

		//lua_getglobal(L, "tellme"); // tell what function to run

		///--- Running script
		if (lua_pcall(L, 0, 0, 0))
			SkWinLua_Bail(L, STR_TO_CHAR_COMPAT_MINGW "lua_pcall() failed", sScriptFilename, "-");

			/*
		lua_pcall(
			L, 
			number_of_args, 
			number_of_returns, 
			errfunc_idx
			);
			*/

		lua_close(L);
	}
	return true;

}

char sLUAScriptFilename[512] = "";
void* xSavedLuaState = NULL;

bool SkWinLua_RunFunctionFromScript(SkWinCore* xSkCore, const char* sScriptFilename, const char* sFunctionName, int iArg1, int iArg2, int iArg3, int iArg4)
{
	CHANGE_CONSOLE_COLOR(BRIGHT, CYAN, BLACK);
	// handle of DCS Uncompatibility mode (which has coordinates +1/+1 in regard to original data)
	//iArg2++;
	//iArg3++;
	SkD((DLV_DBG_LUA, "LuaScript: %s :: %s (%d %d %d %d)\n", sScriptFilename, sFunctionName, iArg1, iArg2, iArg3, iArg4));
	//fprintf(stdout, "LuaScript: %s :: %s (%d %d %d %d)\n", sScriptFilename, sFunctionName, iArg1, iArg2, iArg3, iArg4);
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	if (!bLUAValid)
	{
		SkWinLua_Init_Lua(xSkCore, NULL);
		if (!bLUAValid)
			return false;
	}

	if (!MVALID(sScriptFilename) || !MVALID(sFunctionName))
		return false;

	xCurrentGame = xSkCore;

	//--- That version of script would check if the scriptfile is already opened before going again through the Lua lib at each call

	float fScriptOpen = 0;
	bool bInitScriptFirstTime = true;
	void* xLuaState = NULL;

	{
		bool bValueExists = false;
		if (!strcmp(sLUAScriptFilename, sScriptFilename))
			bValueExists = true;
		if (bValueExists == true)
		{
			bInitScriptFirstTime = false;
			xLuaState = xSavedLuaState;
		}
	}
	SkD((DLV_DBG_LUA, "Scripts %s / %s\n", sLUAScriptFilename, sScriptFilename));


	//---
	{
		lua_State *LuaState = NULL;

		if (bInitScriptFirstTime == true)
		{
			int iRetLUA = 0;
			LuaState = lua_open();

			if (MVALID(LuaState))
			{
				luaL_openlibs(LuaState);

				//lua_pushcfunction(LuaState, SkWinLua_Print);
				//lua_setglobal(LuaState, "SkWinLua_Print");

				lua_pushcfunction(LuaState, DME_GenericCommand);
				lua_setglobal(LuaState, "DME_GenericCommand");
				
				iRetLUA = luaL_loadfile(LuaState, sScriptFilename);
				SkD((DLV_DBG_LUA, "Load Scriptfile %s => %d\n", sScriptFilename, iRetLUA));
				if(iRetLUA)
					SkWinLua_Bail(LuaState, STR_TO_CHAR_COMPAT_MINGW "luaL_loadfile() failed", sScriptFilename, sFunctionName);

				//--- Priming run
				//lua_pcall(L, 0, 0, 0);
				if (lua_pcall(LuaState, 0, 0, 0))
					SkWinLua_Bail(LuaState, STR_TO_CHAR_COMPAT_MINGW "lua_pcall() failed", sScriptFilename, sFunctionName);
			}
		
		}
		else
		{
			LuaState = (lua_State*) xLuaState;
		}

		SkD((DLV_DBG_LUA, "Lua Context = %p => function = %s\n", LuaState, sFunctionName));
		if (MVALID(LuaState))
		{
			int iRetLUA = 0;
			lua_getglobal(LuaState, sFunctionName); // tell what function to run
			lua_pushnumber(LuaState, iArg1);
			lua_pushnumber(LuaState, iArg2);
			lua_pushnumber(LuaState, iArg3);
			lua_pushnumber(LuaState, iArg4);

			///--- Running script
			iRetLUA = lua_pcall(LuaState, 4, 0, 0);
			if (iRetLUA != 0) {
				SkWinLua_Bail(LuaState, STR_TO_CHAR_COMPAT_MINGW "lua_pcall() failed", sScriptFilename, sFunctionName);
				CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_RED, BLACK);
			}
			SkD((DLV_DBG_LUA, "Lua Execution done RC = %d\n", iRetLUA));
			CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
		}


		if (bInitScriptFirstTime == true)
		{
			strcpy(sLUAScriptFilename, sScriptFilename);
			xSavedLuaState = (void*)LuaState;
		}
		// Don't close context here
		//lua_close(LuaState);
	}
	return true;
}


//------------------------------------------------------------------------------

#endif // __DJGPP__

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
