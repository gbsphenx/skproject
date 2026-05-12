//--- Skwin-Startup -------------------------------------
//SPX: This part of code contains common code to prepare and run SKWIN or SKULL
//	Main options :
//	- Engine
//		V4 : runs the PC-9821 V4 version
//		V5 : runs the PC-DOS V5 version
//	- Render
//		MFC : windows with menus
//		SDL : window-box without menus

// MSVC6 note: uncheck "use precompiled header file .pch" else expect "unexpected #endif" there.
#if defined (SKWINSPX) || defined (SK9821V4)
	#if defined (SKDLL_EXPORTS) || defined (LIBSKWINDLL_EXPORTS)
	#include <StdAfx.h>
	#endif // LIBSKWINDLL_EXPORTS
#endif

#include <skapi.h>

#include <skver.h>
#include <skparam.h>
#include <skdebug.h>

#if defined (SK9821V4) || defined (SKWINSPX)
	#include <skglobal.h>
	#include <skcore.h>
#endif // SK9821V4

#include <stdio.h>
#include <xcorelog.h>
#include <xsfxsdl.h>
#include <skcnsole.h>

#include <skwinapp.h>

#include <vector>
#include <string>
#include <sstream>

//==============================================================================

#if defined (SKDOSV5)
	#define CHANGE_CONSOLE_COLOR(_A_,_F_,_B_) ;;
#endif

//#if defined (__DJGPP__)
//	int SKWIN_V5BITDOS_MAIN(SkWinApp* skWinApplication) { return (int)1; }
//#elif defined (SKDOSV5) || defined (SKWINSPX)
#if defined (SKDOSV5) || defined (SKWINSPX)
	extern int SKWIN_V5BITDOS_MAIN(SkWinApp* skWinApplication);
#else
	int SKWIN_V5BITDOS_MAIN(SkWinApp* skWinApplication) { return (int)1; }
#endif


//==============================================================================

int SKWIN_START_MAIN(int iEngine, int argc, char **argv);

int SKWIN_V5BIT_MAIN(SkWinApp* skWinApplication);

//==============================================================================
//#ifndef _SKPROBE_
//#if !defined (_SKPROBE_) || !defined (_SK_NOMAIN_)
#ifndef _SK_NOMAIN_
int main(int argc, char* argv[])
{
	char sBuildInfo[100];
	int iEngine = __SK_ENGINE_V4_;
	int iRender = __SK_RENDERING_MFC_;

//--- If this is the V4 version
#if defined (SK9821V4)
	iEngine = __SK_ENGINE_V4_;
#endif // SK9821V4

//--- If this is the V5 version
#if defined (SKDOSV5)
	iEngine = __SK_ENGINE_V5_;
	iRender = __SK_RENDERING_SDL_;
#endif // SKDOSV5

//--- If this is the V6 version
#if defined (SKWINSPX)
	iEngine = __SK_ENGINE_V6_;
#endif // SKWINSPX


	CHANGE_CONSOLE_COLOR(BRIGHT, WHITE, BLACK);
	printf("SKWIN-SKULL STARTUP [%s] <%s>\n", __SKWIN_PROGNAME__, __SKWIN_SYSTEM__);
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
    // Combine __DATE__ and __TIME__ into one string
    sprintf(sBuildInfo, "Build date and time: %s %s", __SKWIN_RELEASE_DATE__, __TIME__);
    printf("%s\n", sBuildInfo);

	SKWIN_START_MAIN(iEngine, argc, argv);

	return 0;
}
#endif // !_SKPROBE_

//==============================================================================

int SKWIN_START_MAIN(int iEngine, int argc, char **argv)
{
	int iGameRC = 1;

	SkWinApp*	skWinApplication = NULL;

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
	//if (XAUDIO_SDL_INIT())
	//	SkCodeParam::bUseAudioSDL = true;

	skWinApplication = new SkWinApp();

#if defined (__LINUX__)
	SkCodeParam::bRenderingEngineSDL = true;
	SkCodeParam::bRenderingEngineDOS = false;
	SkCodeParam::bRenderingEngineMFC = false;
#endif // __LINUX__
#if defined (__DJGPP__)
	SkCodeParam::bRenderingEngineSDL = false;
	SkCodeParam::bRenderingEngineDOS = true;
	SkCodeParam::bRenderingEngineMFC = false;
#endif // __DJGPP__

	skWinApplication->ProcessArgs(argc, argv);
	// depending on option, set the renderer
	if (SkCodeParam::bRenderingEngineDOS)
		skWinApplication->setRenderer(GFX_RENDERER_DOS);
	else if (SkCodeParam::bRenderingEngineSDL)
		skWinApplication->setRenderer(GFX_RENDERER_SDL);
	else if (SkCodeParam::bRenderingEngineMFC)
		skWinApplication->setRenderer(GFX_RENDERER_MFC);

	{
#if defined(SK9821V4) || defined (SKWINSPX)
		SkWinCore skApp;
		skApp.skWinApp = skWinApplication;
#endif // SK9821V4
		skWinApplication->setVideoMode();
		skWinApplication->StartMessage();
		skWinApplication->ProcessArgs(argc, argv); // just to display again
		skWinApplication->MemInfo();
		skWinApplication->runSplashTitle();
		skWinApplication->skwin_Sleep(500);
#if defined(SK9821V4) || defined (SKWINSPX)
	// IBMIO_BOOTSTRAP => IBMIO_MAIN => IBMIO_EXEC (FIRE.EXE) => FIRE_BOOTSTRAP => FIRE_MAIN => INIT then GAME_LOOP
		iGameRC = skApp.IBMIO_BOOTSTRAP();
#endif // SK9821V4
#ifdef SKDOSV5
	//skWinApplication->runWindowTest();
	//skWinApplication->setVideoMode();
	SKWIN_V5BIT_MAIN(skWinApplication);
#endif
	}
	//XAUDIO_SDL_CLOSE();
	skWinApplication->resetVideoMode();
	skWinApplication->ExitMessage();
	free(skWinApplication);
	

	return iGameRC;
}

//==============================================================================

int SKWIN_V5BIT_MAIN(SkWinApp* skWinApplication)
{
	return SKWIN_V5BITDOS_MAIN(skWinApplication);	// goes into DM2 V5 Bit's Code
}

//==============================================================================
//==============================================================================
//	 LIBRARY CALL
//==============================================================================


int SKWIN_START_MAIN_HEADLESS(tSKWinContext* xSKWinContext, int iEngine, int argc, char **argv)
{
	int iGameRC = 1;

	SkWinApp*	skWinApplication = NULL;

	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);

	SkCodeParam::bUseAudioSDL = true;

	skWinApplication = new SkWinApp();

	SkCodeParam::bRenderingEngineSDL = false;
	SkCodeParam::bRenderingEngineDOS = false;
	SkCodeParam::bRenderingEngineMFC = false;

	skWinApplication->ProcessArgs(argc, argv);
	if (SkCodeParam::bRenderingEngineDOS)
		skWinApplication->setRenderer(GFX_RENDERER_DOS);
	else if (SkCodeParam::bRenderingEngineSDL)
		skWinApplication->setRenderer(GFX_RENDERER_SDL);
	else if (SkCodeParam::bRenderingEngineMFC)
		skWinApplication->setRenderer(GFX_RENDERER_MFC);

	{
		xSKWinContext->xSkCore = new SkWinCore();
//
//		SkWinCore skApp;
//		skApp.skWinApp = skWinApplication;
		xSKWinContext->xSkCore->skWinApp = skWinApplication;

		skWinApplication->setVideoMode();
		skWinApplication->StartMessage();
		skWinApplication->ProcessArgs(argc, argv); // just to display again
		skWinApplication->MemInfo();

		iGameRC = xSKWinContext->xSkCore->SK_INIT();
	}
	
	return iGameRC;
}

SK_API int SK_MainFromCommandLine(const char* sCommandLine)
{
	int iResult = 0;
	//char* argv[] = {"-sdl", "-en", "-data", "DATA-DM2", "-gdat", "DATA-DM2\\DM2V52PC\\G2SKV52C.DAT", "-dungeon", "DATA-DM2\\DM2V52PC\\DNGSK52D.DAT"};
	char* argv[] = {"-sdl", "-en", "-data", "DATA", "-new"};
	int argc = 5;

    return SKWIN_START_MAIN(__SK_ENGINE_V4_, argc, argv);
//	return iResult;
}

SK_API int SK_MainStartHeadless(tSKWinContext* xSKWinContext, const char* sCommandLine)
{
	int iResult = 0;
	int argc = 6;
//	char* argv[] = {"-sdl", "-en", "-data", "DATA", "-new"};
	argc = 5;
	//char* argv[] = {"-en", "-data", "DATA", "-new"}; argc = 4;
	//char* argv[] = {"-sdl", "-en", "-data", "DATA-DM2", "-new", "-gdat", "DATA-DM2\\DM2V52PC\\G2SKV52C.DAT", "-dungeon", "DATA-DM2\\DM2V52PC\\DNGSK52D.DAT"};
	//char* argv[] = {"-sdl", "-en", "-data", "DATA-DM2", "-new", "-gdat", "DATA-DM2\\COMMON\\G2SKV42X.DAT", "-dungeon", "DATA-DM2\\DM2V52PC\\DNGSK52D.DAT"}; argc = 9;
	char* argv[] = {"-audio", "-en", "-data", "DATA-DM2", "-new", "-gdat", "DATA-DM2\\COMMON\\G2SKV42X.DAT", "-dungeon", "DATA-DM2\\DM2V52PC\\DNGSK52D.DAT"}; argc = 9;
	//char* argv[] = {"-en", "-data", "DATA-DM2", "-new", "-gdat", "DATA-DM2\\COMMON\\G2SKV42X.DAT", "-dungeon", "DATA-DM2\\DM2V52PC\\DNGSK52D.DAT"}; argc = 8;
	//argc = 9;
    return SKWIN_START_MAIN_HEADLESS(xSKWinContext, __SK_ENGINE_V4_, argc, argv);
//	return iResult;
}


SK_API int SK_GameLoad(tSKWinContext* xSKWinContext)
{
	int iResult = 0;

    return xSKWinContext->xSkCore->SK_GAMELOAD();
}


SK_API int SK_StartGameLoop(tSKWinContext* xSKWinContext)
{
	int iResult = 0;

    return xSKWinContext->xSkCore->SK_GAMELOOP();
}

SK_API int SK_StartGameLoop(tSKWinContext* xSKWinContext, MouseProviderFunc fMP)
{
	int iResult = 0;

    return xSKWinContext->xSkCore->SK_GAMELOOP();
}

SK_API void SK_RegisterMouseProvider(tSKWinContext* xSKWinContext, MouseProviderFunc func)
{
    xSKWinContext->xSkCore->skWinApp->RegisterMouseProvider(xSKWinContext->xController, func);
}

//==============================================================================
