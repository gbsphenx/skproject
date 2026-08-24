//------------------------------------------------------------------------------
// SKMINI
//	Minimal main to test & confirm compatibility/portability among different systems.
//	SKMini writes some timestamped information, then opens test screen display.
//		
//	Target systems are:
//	DOS/DJGPP
//		- compiled on FreeDOS VM => produces "SKULLV0.EXE"
//		- tested on FreeDOS VM & DOSBox emulation
//	Linux + SDL2:
//		- compiled on WSL2 => produces "skwinv0"
//		- tested on Win10/11 WSL2 (Ubuntu 24.04)
//	Windows:
//		- compiled on Win/MSVC6 => produces "skwinv0.exe"
//		- tested on Win7/10/11
//
//
// Note
//	MSVC6: Add SDL_MAIN_HANDLED as preprocessor definitions
//		_MSC_VER = 1200

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <sktypes.h>
#include <skver.h>
#include <skparam.h>

#include <skglobal.h>

#include <skwinapp.h>
#include <skdos.h>
#include <sksdl2.h>

//#include <conio.h>	// getch


SkWinApp	skWinApplication;

const char* sLogSkMini = "skmini.log";

#ifndef _SKPROBE_
int main(int argc, char* argv[])
{
	FILE *fpInSys = NULL;
	FILE *fpLog = NULL;
	bool bInSysFile = false;
	//X8 cKeyboardInput = ' ';
	char line[1024];
	char sBuildInfo[100];
	char sXSysInfoScript[100];


	//--- DOS, change text mode display before full screen test
	if (SkCodeParam::bRenderingEngineDOS) {
		skWinApplication.setRenderer(GFX_RENDERER_DOS);
		skWinApplication.setVideoMode(DOS_VIDEO_VGA_16);
	}
	printf("SKWin-Mini-V0 Check System -- %s <%s>\n", __SKWIN_PROGNAME__, __SKWIN_SYSTEM__);
    sprintf(sBuildInfo, "Build date and time: %s %s", __SKWIN_RELEASE_DATE__, __TIME__);
    printf("%s\n", sBuildInfo);


#if defined (__LINUX__)
	SkCodeParam::bRenderingEngineSDL = true;
	SkCodeParam::bRenderingEngineDOS = false;
	SkCodeParam::bRenderingEngineMFC = false;
#endif // __LINUX__

	skWinApplication.ProcessArgs(argc, argv);
	skWinApplication.MemInfo();

	// depending on option, set the renderer
	if (SkCodeParam::bRenderingEngineDOS)
		skWinApplication.setRenderer(GFX_RENDERER_DOS);
	else if (SkCodeParam::bRenderingEngineSDL)
		skWinApplication.setRenderer(GFX_RENDERER_SDL);
	else if (SkCodeParam::bRenderingEngineMFC)
		skWinApplication.setRenderer(GFX_RENDERER_MFC);

	skWinApplication.runWindowTest();

	printf("Test complete.\n");

	return 0;
}
#endif // _SKPROBE_

//------------------------------------------------------------------------------

