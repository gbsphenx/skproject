//--- Skwin-Startup -------------------------------------
//SPX: This part of code contains common code to prepare and run SKWIN or SKULL
//	Main options :
//	- Engine
//		V4 : runs the PC-9821 V4 version
//		V5 : runs the PC-DOS V5 version
//	- Render
//		MFC : windows with menus
//		SDL : window-box without menus

#if defined (SK9821V4)
	#include <StdAfx.h>	// Required for MVC6 compilation
#endif // SK9821V4

#include <SKVER.H>

#if defined (SK9821V4)
	#include <SKPARAM.H>
	#include <SKDEBUG.H>
	#include <SKGLOBAL.H>
	#include <KCORE.H>
#endif // SK9821V4

#include <stdio.h>
#include <XCORELOG.H>

//==============================================================================

#if defined (SKDOSV5)
	#define CHANGE_CONSOLE_COLOR(_A_,_F_,_B_) ;;
#endif

//==============================================================================
#if defined (SK9821V4)
	int SKWIN_V5BITDOS_MAIN(int argc, char* argv[]) { return (int)1; }
#endif

int SKWIN_V4KJ_MAIN_MFC(int argc, char **argv);
int SKWIN_V4KJ_DOS_LINUX_MINGW(int argc, char **argv);
int SKWIN_V5BIT_MAIN_SDL(int argc, char* argv[]);

//==============================================================================

int main(int argc, char* argv[])
{
	char buildInfo[100];
	int iEngine = __SK_ENGINE_V4_;
	int iRender = __SK_RENDERING_MFC_;

//--- If this is the V4 version
#if defined (SK9821V4)
	iEngine = __SK_ENGINE_V4_;
	iRender = __SK_RENDERING_MFC_;
	#if defined (__MINGW__) || defined (__LINUX__)
		iRender = __SK_RENDERING_SDL_;
	#elif defined (__DJGPP__)
		iRender = __SK_RENDERING_DOS_VRAM;
	#endif
#endif // SK9821V4

//--- If this is the V5 version
#if defined (SKDOSV5)
	iEngine = __SK_ENGINE_V5_;
	iRender = __SK_RENDERING_SDL_;
#endif // SKDOSV5
//--- If this is the V6 version


	CHANGE_CONSOLE_COLOR(BRIGHT, WHITE, BLACK);
	printf("SKWIN-SKULL STARTUP\n");
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);
    // Combine __DATE__ and __TIME__ into one string
    sprintf(buildInfo, "Build date and time: %s %s", __DATE__, __TIME__);
    printf("%s\n", buildInfo);

	if (iEngine == 4 && iRender == __SK_RENDERING_MFC_) {
		return SKWIN_V4KJ_MAIN_MFC(argc, argv);
	}
	else if (iEngine == 4 && iRender != __SK_RENDERING_MFC_) {
		return SKWIN_V4KJ_DOS_LINUX_MINGW(argc, argv);
	}
	if (iEngine == 5 && iRender == __SK_RENDERING_SDL_) {
		return SKWIN_V5BIT_MAIN_SDL(argc, argv);
	}

	return 0;
}

//==============================================================================

// MAIN call for SKWin MFC
int SKWIN_V4KJ_MAIN_MFC(int argc, char **argv)
{
	int r = 1;
#if defined(_USE_MFC80) || defined(_USE_MFC60)
	HRESULT hr;
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);


	if (AfxWinInit(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOW)) {
		if (SUCCEEDED(hr = CoInitialize(NULL))) {
			int f = skwin.sxfact;
			CRect rc(0, 0, 320 * f, 200 * f);
			AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, true, 0);
			rc.OffsetRect(-rc.left, -rc.top);
			if (skwin.CreateEx(0, AfxRegisterWndClass(0, AfxGetApp()->LoadCursor(IDC_POINTER), (HBRUSH)GetStockObject(BLACK_BRUSH), NULL), "SKWin-9821", WS_OVERLAPPEDWINDOW, rc, NULL, 0, NULL)) {
				skwin.ShowWindow(SW_SHOWDEFAULT);

				SkWinCore a;
				skwin.skw = &a;
				skwin.ProcessArgs(argc, argv);
				r = a.IBMIO_BOOTSTRAP();

				skwin.DestroyWindow();
			}
		}
	}
#endif // _USE_MFC80 / _USE_MFC60
	return r;
}


// MAIN call for SKWin MinGW / DOS / Linux
int SKWIN_V4KJ_DOS_LINUX_MINGW(int argc, char **argv)
{
	int r = 1;
#if defined(__DJGPP__) || defined (__MINGW__) || defined (__LINUX__)
	SkWinCore a;
	skwin.skw = &a;
	skwin.StartMessage();
	skwin.ProcessArgs(argc, argv);
	skwin.InitVideo();
	r = a.IBMIO_BOOTSTRAP();
	skwin.ResetVideo();
	skwin.ExitMessage();
#endif // __DJGPP__
	return r;
}

//==============================================================================

int SKWIN_V5BIT_MAIN_SDL(int argc, char* argv[])
{
	return SKWIN_V5BITDOS_MAIN(argc, argv);
}

//==============================================================================
