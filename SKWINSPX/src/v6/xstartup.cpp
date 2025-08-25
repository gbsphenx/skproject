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
#include <StdAfx.h>	// Required for MSVC6 compilation
#endif

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

//==============================================================================

int SKWIN_START_MAIN(int iEngine, int argc, char **argv);

int SKWIN_V5BIT_MAIN(SkWinApp* skWinApplication);

//==============================================================================
#ifndef _SKPROBE_
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
	int r = 1;

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
		SkWinCore a;
		a.skWinApp = skWinApplication;
#endif // SK9821V4
		skWinApplication->setVideoMode();
		skWinApplication->StartMessage();
		skWinApplication->ProcessArgs(argc, argv); // just to display again
		skWinApplication->MemInfo();
		skWinApplication->runSplashTitle();
		skWinApplication->skwin_Sleep(1000);
		if (SkCodeParam::bRenderingEngineDOS) {
			skWinApplication->runSplashTitle();
			skWinApplication->skwin_Sleep(4000);
		}
#if defined(SK9821V4) || defined (SKWINSPX)
	// IBMIO_BOOTSTRAP => IBMIO_MAIN => IBMIO_EXEC (FIRE.EXE) => FIRE_BOOTSTRAP => FIRE_MAIN => INIT then GAME_LOOP
		r = a.IBMIO_BOOTSTRAP();
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
	

	return r;
}

//==============================================================================

int SKWIN_V5BIT_MAIN(SkWinApp* skWinApplication)
{
	return SKWIN_V5BITDOS_MAIN(skWinApplication);	// goes into DM2 V5 Bit's Code
}

//==============================================================================
