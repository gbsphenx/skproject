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
#include <skglobal.h>
#include <skparam.h>

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

	fpLog = fopen(sLogSkMini, "a");
	if (fpLog == NULL) { 
		printf("Failed to open %s.\n", sLogSkMini);
		return 1;
	}

	//--- Execute the xsysinfo script
	strcpy(sXSysInfoScript, "xsysinfo.bat");
#if defined (__LINUX__)
	strcpy(sXSysInfoScript, "./xsysinfo.sh");
#endif
	system(sXSysInfoScript);

	//--- Open skinfo file
    fpInSys = fopen("skmini.inf", "r");
    if (fpInSys == NULL) {
        perror("Error opening input file (skmini.inf)");
		printf("Please run %s on your environment.\n", sXSysInfoScript);
        bInSysFile =false;
    }
	else
		bInSysFile = true;


	fprintf(fpLog, "****************************************\n");
	fprintf(fpLog, "%s <%s>\n", __SKWIN_PROGNAME__, __SKWIN_SYSTEM__);
    // Read and write each line
	if (bInSysFile == true) {
		while (fgets(line, sizeof(line), fpInSys)) {
			fputs(line, fpLog);
		}
	}
	if (fpInSys != NULL)
		fclose(fpInSys);
	fprintf(fpLog, "----------------------------------------\n");
	fprintf(fpLog, "Basic sizes\n");
	fprintf(fpLog, "char -> %ld	short-> %ld	int  -> %ld	longint-> %ld\n", sizeof(char), sizeof(short), sizeof(int), sizeof(long int));
	fprintf(fpLog, "U8   -> %ld	X8   -> %ld	i8   -> %ld\n", sizeof(U8), sizeof(X8), sizeof(i8));
	fprintf(fpLog, "U16  -> %ld	X16  -> %ld	i16  -> %ld\n", sizeof(U16), sizeof(X16), sizeof(i16));
	fprintf(fpLog, "U32  -> %ld	X32  -> %ld	i32  -> %ld\n", sizeof(U32), sizeof(X32), sizeof(i32));
	fprintf(fpLog, "----------------------------------------\n");
	fprintf(fpLog, "MAX CRAM = %d\n", MAX_CRAM);	// Conventional memory: 640KiB
	fprintf(fpLog, "MAX CEMS = %d\n", MAX_CEMS);	// EMS memory space: 6MiB or more
	fprintf(fpLog, "MAX VRAM = %d\n", MAX_VRAM);	// VRAM memory space: 64 KiB

	fprintf(fpLog, "Dungeon File Header = %ld\n", sizeof(File_header));
	fprintf(fpLog, "Map Definition = %ld\n", sizeof(Map_definition_info));
	
	fprintf(fpLog, "Champion struct. = %ld\n", sizeof(Champion));
	fprintf(fpLog, "****************************************\n");

	if (fpLog != NULL)
		fclose(fpLog);
	printf("Information written into %s\n", sLogSkMini);
	printf("Done.\n");

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

	skWinApplication.runTest();
/*
	// screen testing
	if (SkCodeParam::bRenderingEngineMFC || SkCodeParam::bRenderingEngineSDL || SkCodeParam::bRenderingEngineDOS)
	{
		printf("Next test for full window/screen.\n");
		printf("Press enter to continue ...\n");
		scanf("%c", &cKeyboardInput);

		skWinApplication.setVideoMode();	// also init
		//skWinApplication.setVideoTestGradient();
		skWinApplication.setVideoTestStatic(10);
		printf("Next test for title image display.\n");
		printf("Press enter to continue ...\n");
		scanf("%c", &cKeyboardInput);

		skWinApplication.setVideoTestTitle();
		printf("Press enter to continue ...\n");
		scanf("%c", &cKeyboardInput);

		skWinApplication.resetVideoMode();
	}*/

	printf("Test complete.\n");

	return 0;
}
#endif // _SKPROBE_

//------------------------------------------------------------------------------

