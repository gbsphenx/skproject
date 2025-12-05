//--- SkWinApp -------------------------------------

#include <skwinapp.h>
#include <skparam.h>
#include <skdefine.h>

#include <sktypes.h>

#include <skvram.h>
#include <skdebug.h>

//--- the 3 different renderers
#include <skdos.h>
#include <sksdl2.h>
#include <skmfc.h>

#include <skinput.h>

#include <dme.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//#if !defined (SKDOSV5) && ((_MSC_VER >= 1200) || defined (_USE_MFC80) || defined (_USE_MFC60))
#if (_MSC_VER >= 1200) || defined (_USE_MFC80) || defined (_USE_MFC60)
	#include <windows.h> // sleep
#elif defined (__LINUX__)
	#include <unistd.h> // sleep
#elif defined (__MINGW__)
	//#include <process.h> // _sleep
	//#include <windows.h> // sleep
	#ifdef _WIN32
	extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long ms);	// do this to avoid importing the whole <windows.h> for that
	extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount(void);
	#endif
#elif defined (__DJGPP__)
	#include <dos.h>	// delay (sleep)
#endif


const char* SkWinApp::getBuildDateISO() {
    static char dateStr[11]; // YYYY-MM-DD
    const char* date = __DATE__; // e.g., "Aug  7 2025"

    char monthStr[4] = { date[0], date[1], date[2], '\0' };
    int day, year;
    sscanf(date + 4, "%d %d", &day, &year);

    const char* monthNum =
        strcmp(monthStr, "Jan") == 0 ? "01" :
        strcmp(monthStr, "Feb") == 0 ? "02" :
        strcmp(monthStr, "Mar") == 0 ? "03" :
        strcmp(monthStr, "Apr") == 0 ? "04" :
        strcmp(monthStr, "May") == 0 ? "05" :
        strcmp(monthStr, "Jun") == 0 ? "06" :
        strcmp(monthStr, "Jul") == 0 ? "07" :
        strcmp(monthStr, "Aug") == 0 ? "08" :
        strcmp(monthStr, "Sep") == 0 ? "09" :
        strcmp(monthStr, "Oct") == 0 ? "10" :
        strcmp(monthStr, "Nov") == 0 ? "11" : "12";

    sprintf(dateStr, "%04d-%s-%02d", year, monthNum, day);
    return dateStr;
}



//// Internal MEM areas from PC-9821 / PC-DOS
//U8* glb_SK_CRAM = NULL;
//U8* glb_SK_CEMS = NULL;
//U8* glb_SK_VRAM = NULL;
/*
#define MEM_EMS_MB 16
#define MAX_CRAM (1024U*640)		// Conventional memory: 640KiB
#define MAX_CEMS (1024U*1024*MEM_EMS_MB)	// EMS memory space: 6MiB or more
#define MAX_VRAM (1024U*64)		// VRAM memory space: 64 KiB
*/

SkWinApp::SkWinApp()
{
	iVideoRenderer = GFX_RENDERER_WIN;

	iSizeCRAM = MAX_CRAM;
	iSizeCEMS = MAX_CEMS;
	iSizeVRAM = MAX_VRAM;

	xSKCRAM = (X8*) calloc(iSizeCRAM, sizeof(X8));
	xSKCEMS = (X8*) calloc(iSizeCEMS, sizeof(X8));
	xSKVRAM = (X8*) calloc(iSizeVRAM, sizeof(X8));

	xVRAM = new SkVRAM();

	sxfact = 0;
	sblast = 0;
	sbdsbf = 0;
	sbdsaf = 0;
	spfact = 1; // speed factor
	lang = 0;
	dung = -1;
	freq = 11050;
	sksave = 0;		// savegame to resume
	newgame = 0;	// start new game

	xSkWinRenderer = NULL;

	cntMiceIn = 0;
	cntKeybIn = 0;
	curKeybIn = 0;

	iCallbackMouseX = 0;
	iCallbackMouseY = 0;
	iCallbackMouseButton = 0;

}

SkWinApp::~SkWinApp()
{
	free(xSKCRAM);
	free(xSKCEMS);
	free(xSKVRAM);

	free(xVRAM);
}


X8* SkWinApp::getVRAM()
{
	return xSKVRAM;
}


UINT SkWinApp::setRenderer(eRendererType iRenderer)
{
	iVideoRenderer = iRenderer;
	printf("Renderer set to %d\n", iVideoRenderer);
	return 0;
}

void SkWinApp::MemInfo()
{
	printf("Allocated CRAM = %d KiB (%p)\n", iSizeCRAM/1024, xSKCRAM);
	printf("Allocated CEMS = %d MiB (%p)\n", iSizeCEMS/1024/1024, xSKCEMS);
	printf("Allocated VRAM = %d KiB (%p)\n", iSizeVRAM/1024, xSKVRAM);
}

void SkWinApp::ProcessArgs(int argc, char** argv)
{
	int iArgIndex = 0;

	sCustomGraphicsDatFilename = NULL;
	sCustomDungeonDatFilename = NULL;
	sCustomDataFolder = NULL;

	while (iArgIndex < argc)
	{
		if (argv[iArgIndex] == NULL)
			break;
			
		// Options: renderer mode
		if (!strcmp(argv[iArgIndex], "-sdl")) {
			printf("Option: renderer = SDL\n");
#ifdef __DJGPP__
			printf("(option ignored)\n");
#else
			SkCodeParam::bRenderingEngineSDL = true;
			SkCodeParam::bRenderingEngineDOS = false;
			SkCodeParam::bRenderingEngineMFC = false;
#endif
			//SkCodeParam::bEngineNoDisplay = false;
		}
		else if (!strcmp(argv[iArgIndex], "-mfc")) {	// windows only
			printf("Option: renderer = MFC\n");
#ifndef _MSC_VER
			printf("(option ignored)\n");
#else
			SkCodeParam::bRenderingEngineMFC = true;
			SkCodeParam::bRenderingEngineSDL = false;
			SkCodeParam::bRenderingEngineDOS = false;
#endif
			//SkCodeParam::bEngineNoDisplay = false;
		}
		else if (!strcmp(argv[iArgIndex], "-dos")) {	// dos only
			printf("Option: renderer = DOS\n");
#ifndef __DJGPP__
			printf("(option ignored)\n");
#else
			SkCodeParam::bRenderingEngineDOS = true;
			SkCodeParam::bRenderingEngineMFC = false;
			SkCodeParam::bRenderingEngineSDL = false;
#endif
			//SkCodeParam::bEngineNoDisplay = false;
		}
		else if (!strcmp(argv[iArgIndex], "-headless")) {
			printf("Option: renderer = none\n");
			SkCodeParam::bRenderingEngineDOS = false;
			SkCodeParam::bRenderingEngineMFC = false;
			SkCodeParam::bRenderingEngineSDL = false;
			SkCodeParam::bEngineNoDisplay = true;
		}

		// Options: language
		else if (!strcmp(argv[iArgIndex], "-en")) {
			printf("Option: language = english\n");
			lang = _GDAT_LANG_ENGLISH_;	// english
		}
		else if (!strcmp(argv[iArgIndex], "-jp")) {
			printf("Option: language = japanese\n");
			lang = _GDAT_LANG_JAPANESE_;	// japanese
		}
		else if (!strcmp(argv[iArgIndex], "-de")) {
			printf("Option: language = german\n");
			lang = _GDAT_LANG_GERMAN_;	// german
		}
		else if (!strcmp(argv[iArgIndex], "-fr")) {
			printf("Option: language = french\n");
			lang = _GDAT_LANG_FRENCH_;	// french
		}
		else if (!strcmp(argv[iArgIndex], "-es")) {
			printf("Option: language = spanish\n");
			lang = _GDAT_LANG_SPANISH_;	// spanish
		}
		else if (!strcmp(argv[iArgIndex], "-it")) {
			printf("Option: language = italian\n");
			lang = _GDAT_LANG_ITALIAN_;	// italian
		}


		// specify any graphics.dat filename
		else if (!strcmp(argv[iArgIndex], "-gdat"))
		{
			iArgIndex++;
			if (argv[iArgIndex] != NULL)
			{
				unsigned int iArgLength = 0;
				iArgLength = strlen(argv[iArgIndex]);
				if (iArgLength > 0)
				{
					sCustomGraphicsDatFilename = (char*) calloc(iArgLength+1, sizeof(char));
					if (sCustomGraphicsDatFilename != NULL)
						strcpy(sCustomGraphicsDatFilename, argv[iArgIndex]);
				}
			}
		}
		// specify any dungeon.dat filename
		else if (!strcmp(argv[iArgIndex], "-dungeon"))
		{
			iArgIndex++;
			if (argv[iArgIndex] != NULL)
			{
				unsigned int iArgLength = 0;
				iArgLength = strlen(argv[iArgIndex]);
				if (iArgLength > 0)
				{
					sCustomDungeonDatFilename = (char*) calloc(iArgLength+1, sizeof(char));
					if (sCustomDungeonDatFilename != NULL)
						strcpy(sCustomDungeonDatFilename, argv[iArgIndex]);
				}
			}
		}

		// specify any data folder (by default, dungeon.dat and graphics.dat will be taken from that folder)
		else if (!strcmp(argv[iArgIndex], "-data"))
		{
			iArgIndex++;
			if (argv[iArgIndex] != NULL)
			{
				unsigned int iArgLength = 0;
				iArgLength = strlen(argv[iArgIndex]);
				if (iArgLength > 0)
				{
					sCustomDataFolder = (char*) calloc(iArgLength+1, sizeof(char));
					if (sCustomDataFolder != NULL)
						strcpy(sCustomDataFolder, argv[iArgIndex]);
				}
			}
		}

		/// Options: specific game modes
		else if (!strcmp(argv[iArgIndex], "-tq")) {	// Theron's Quest
			printf("Option: game mode = Theron's Quest\n");
			SkCodeParam::bTQMode = true;
		}
		else if (!strcmp(argv[iArgIndex], "-bwy")) {	// Bloodwych mode
			printf("Option: game mode = Bloodwych\n");
			//dung = _OPTION_DUNGEON_BWY_BW_;
			SkCodeParam::bBWMode = true;
		}
		else if (!strcmp(argv[iArgIndex], "-eob")) {	// Eye of the Beholder mode
			printf("Option: game mode = Eye of the Beholder\n");
			//dung = _OPTION_DUNGEON_EOB_EOB1_;
			SkCodeParam::bEOBMode = true;
		}
		else if (!strcmp(argv[iArgIndex], "-dmx")) {	// Extended Dungeon Master mode
			printf("Option: game mode = Dungeon Master Extended\n");
			//dung = _OPTION_DUNGEON_DMX_;
			SkCodeParam::bGameModeExtended = true;
		}

		/// Options: audio playback frequency
		else if (!strcmp(argv[iArgIndex], "-fhz6"))
			freq = 6;
		else if (!strcmp(argv[iArgIndex], "-fhz11"))
			freq = 11;


		/// Options: special quick start
		else if (!strcmp(argv[iArgIndex], "-new")) {	// directly start new game
			printf("Option: quick start = start new game\n");
			//newgame = 1;
			SkCodeParam::bOptionNewGame = 1;
		}
		else if (!strcmp(argv[iArgIndex], "-resume")) {	// go to resume screen
			printf("Option: quick start = start on resume screen\n");
			//newgame = 2;
			SkCodeParam::bOptionNewGame = 2;
		}
		else if (!strcmp(argv[iArgIndex], "-credits")) {	// go to credits screen
			printf("Option: quick start = start on credits screen\n");
			//newgame = 2;
			SkCodeParam::bOptionNewGame = 3;
		}

		// r0 to r9 (r"x") to load and resume savegame "x"
		else if (strncmp(argv[iArgIndex], "-r", 2) == 0) {
			char *sParam = argv[iArgIndex] + 2;
			int iSavegameVal = atoi(sParam);
			printf("Option: quick start = load and resume savegame %d\n", iSavegameVal);
			SkCodeParam::bOptionResumeSaveGame = iSavegameVal;
			SkCodeParam::bOptionNewGame = 2;
		}

		iArgIndex++;
	}

}


UINT SkWinApp::setVideoMode()
{
	if (iVideoRenderer == GFX_RENDERER_SDL) {
		xSkWinRenderer = (SkRendererGeneric*) new SkRendererSDL();
	}
	else if (iVideoRenderer == GFX_RENDERER_DOS) {
		xSkWinRenderer = (SkRendererGeneric*) new SkRendererDOS();
	}
	else if (iVideoRenderer == GFX_RENDERER_MFC) {
		xSkWinRenderer = (SkRendererGeneric*) new SkRendererMFC();
	}

	if (xSkWinRenderer != NULL) {
		xSkWinRenderer->Init(xVRAM);
		xSkWinRenderer->InitWinApp(this);
	}
	else
		return 1;

	return 0;
}

UINT SkWinApp::setVideoMode(UINT iSpecificMode)
{
	setVideoMode();
	if (iSpecificMode == DOS_VIDEO_VGA_16) {
		SkRendererDOS* xRendererDOS = (SkRendererDOS*) xSkWinRenderer;
		if (xRendererDOS != NULL)
			xRendererDOS->SetModeVGA16();
	}

	return 0;
}

UINT SkWinApp::setVideoTestStatic(UINT iFrames)
{
	for (UINT i = 0; i < iFrames; i++) {
		//SkWinApp::skwin_ML();	// for call to poll event
		SkWinApp::skwin_Sleep(125);
		if (xVRAM != NULL)
			xVRAM->SET_TEST_VRAM_STATIC();
		if (xSkWinRenderer != NULL)
			xSkWinRenderer->Render();
	}
	return 0;
}

UINT SkWinApp::setVideoTestGradient()
{
	if (xVRAM != NULL)
		xVRAM->SET_TEST_VRAM_GRADIENT();

	if (xSkWinRenderer != NULL)
		xSkWinRenderer->Render();
	return 0;
}


UINT SkWinApp::startAudioTest()
{
#if !defined (__NO_SDL__)
	if (iVideoRenderer == GFX_RENDERER_SDL) {
	printf("Start Audio Test\n");
		xSkWinRenderer->StartAudioSample("DATADM2/64.wav");
	}
#endif
	return 0;
}


UINT SkWinApp::setVideoTestTitle()
{
	if (xVRAM != NULL)
		xVRAM->SET_TEST_TITLE();

	if (xSkWinRenderer != NULL)
		xSkWinRenderer->Render();
	return 0;
}


UINT SkWinApp::resetVideoMode()
{

	if (xSkWinRenderer != NULL) {
		xSkWinRenderer->Close();
		free(xSkWinRenderer);
	}
	return 0;
}

UINT SkWinApp::renderScreen(i16 x, i16 y, i16 cx, i16 cy)
{

	if (xSkWinRenderer != NULL)
		xSkWinRenderer->Render();
	return 0;
}

void X_LOG_HEXA(X8* pData, UINT iNbBytes)
{
	X8* pX = (X8*) pData;

	if (SkCodeParam::bRenderingEngineDOS)
		return;

	printf("PX:%p | ", pX);
	for (UINT i = 0; i < iNbBytes; i++) {
		printf("%02X ", (UINT) *pX);
		pX++;
	}
	printf("\n");
}

UINT SkWinApp::renderScreen(X8* xVRAMArray, i16 x, i16 y, i16 cx, i16 cy)
{
	if (xVRAMArray != NULL)
		xVRAM->COPY_VRAM_DATA(xVRAMArray);
	{
//		X_LOG_HEXA(xVRAMArray+(320*50)+160, 32);
//		X_LOG_HEXA(xVRAM->GET_VIDEO_ARRAY()+(320*50)+160, 32);
	}
	renderScreen(x, y, cx, cy);

	//setVideoTestStatic(1);

	return 0;
}

UINT SkWinApp::setRGBPalette(X8* xPalette)
{
	xVRAM->SET_VGA_PALETTE_3RGB(xPalette);
	return 0;
}

UINT SkWinApp::setRGB6Palette(X8* xPalette)
{
	xVRAM->SET_VGA_PALETTE_3RGB6(xPalette);
	return 0;
}


void SkWinApp::LogParams()
{
	printf("CUSTOM FOLDER = %s\n", sCustomDataFolder);
	printf("DUNGEON FILE  = %s\n", sCustomDungeonDatFilename);
	printf("GRAPHICS FILE = %s\n", sCustomGraphicsDatFilename);
}

//------------------------------------------------------------------------------

const X8* SkWinApp::GET_DATA_FOLDER_NAME()
{
	if (sCustomDataFolder != NULL && sCustomDataFolder[0])
		return (const X8*) sCustomDataFolder;
	// default folder is DATA
	return (const X8*) "DATA\\";
}


// Old compatibility with CSkWin -----------------------------------------------

void SkWinApp::skwin_SndPlayHi(const U8 *buff, U32 buffSize, i8 vol)
{
	;
}

void SkWinApp::skwin_SndPlayLo(const U8 *buff, U32 buffSize, i8 dX, i8 dY)
{
	;
}



X8 SkWinApp::skwin_GetLang() {
	return (X8)lang;
}

void SkWinApp::skwin_SndSetFrequency(U16 iSoundPlaybackFrequency)
{
	//sbds.SetPlaybackFrequency(iSoundPlaybackFrequency);
}

void SkWinApp::skwin_ShowMessage(const char *psz)
{
	;
}

void SkWinApp::skwin_Sleep(U32 millisecs)
{
	static int iCall = 0;
#if (_MSC_VER >= 1200) || defined (_USE_MFC80) || defined (_USE_MFC60)
	//::Sleep(millisecs);
	Sleep(millisecs);
#elif defined(__MINGW__)
	Sleep(millisecs);
	//_sleep(millisecs);
#elif defined (__LINUX__)
	usleep(millisecs*1000);
#elif defined (__DJGPP__)
	delay(millisecs);
#endif
}

//..............................................................................
#if defined (__LINUX__)
#include <time.h>
unsigned long linux_GetTickCount() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000); // milliseconds
}
#endif // __LINUX__

#if defined (__DJGPP__)
unsigned long DOS_GetTickCount() {
    union REGS regs;
    regs.h.ah = 0x00;  // BIOS function 00h: Get system time
    int86(0x1A, &regs, &regs);
    return ((unsigned long)regs.x.cx << 16) | regs.x.dx;
}
#endif // __DJGPP__

U32 SkWinApp::skwin_GetTickCount()
{
#if (_MSC_VER >= 1200) || defined (_USE_MFC80) || defined (_USE_MFC60)
	//return ::GetTickCount();
	return GetTickCount();
#elif defined (__MINGW__)
	return GetTickCount();
#elif defined (__LINUX__)
	return linux_GetTickCount();
#elif defined (__DJGPP__)
	return DOS_GetTickCount();
#endif
	//return 1;	// return 0 => this breaks the MessageLoop function to fall into infinite loop
}


UINT SkWinApp::runWindowTest()
{
	X8 cKeyboardInput = ' ';
	// screen testing
	if (SkCodeParam::bRenderingEngineMFC || SkCodeParam::bRenderingEngineSDL || SkCodeParam::bRenderingEngineDOS)
	{
		printf("Next test for window/screen.\n");
		printf("Press enter to continue ...\n");
		scanf("%c", &cKeyboardInput);

		setVideoMode();	// also init
		//skWinApplication.setVideoTestGradient();
		setVideoTestStatic(10);
		printf("Next test for title image display.\n");
		printf("Press enter to continue ...\n");
		scanf("%c", &cKeyboardInput);

		startAudioTest();
		setVideoTestTitle();
		printf("Press enter to continue ...\n");
		scanf("%c", &cKeyboardInput);

		resetVideoMode();
	}
	return 0;
}

UINT SkWinApp::runSplashTitle()
{
	if (SkCodeParam::bRenderingEngineMFC || SkCodeParam::bRenderingEngineSDL || SkCodeParam::bRenderingEngineDOS)
	{
		setVideoTestTitle();
	}
	return 0;
}

bool SkWinApp::skwin_IsAvail()
{
	return (xVRAM != NULL ? true : false);
}


bool SkWinApp::skwin_ML() {

	if (SkCodeParam::bRenderingEngineSDL || SkCodeParam::bRenderingEngineMFC)
		xSkWinRenderer->ML();
	else
		this->skwin_Sleep(1);

	return skwin_IsAvail();
}


CSkKinput* SkWinApp::DequeueKinput()
{
	CSkKinput* pKInput = NULL;
	if (cntKeybIn != 0) {
		pKInput = &xKeybInput[curKeybIn];
		curKeybIn = (curKeybIn +1) % MAXKEYBIN;
		cntKeybIn--;
	}
	return pKInput;
}

void SkWinApp::GetMousePosButtons(U16 *x, U16 *y, U16 *buttons) 
{
	static UINT iCallCount = 0;
	int iDeviceMouseX = 0;
	int iDeviceMouseY = 0;
	U32 iDeviceButtons = 0;

	//SkD((DLV_MOUSE,"GetMousePosButton\n"));
	skwin_ML();
	//SkD((DLV_MOUSE,"after skwin_ML\n"));

	*buttons = 0;
	if (SkCodeParam::bRenderingEngineSDL) {
#if defined(__SDL__) && !defined(__NO_SDL__)
		iDeviceButtons = (U32) SDL_GetMouseState(&iDeviceMouseX, &iDeviceMouseY);
		if (iDeviceButtons & SDL_BUTTON(SDL_BUTTON_LEFT))
			*buttons += 1; // left
		if (iDeviceButtons & SDL_BUTTON(SDL_BUTTON_RIGHT))
			*buttons += 2; // right
		if (iDeviceButtons & SDL_BUTTON(SDL_BUTTON_MIDDLE))
			*buttons += 4; // middle
#endif // __NO_SDL__
		*x = U16(iDeviceMouseX)/SkCodeParam::iVideoScale;
		*y = U16(iDeviceMouseY)/(SkCodeParam::iVideoScale*SkCodeParam::fVideoYScale);
	}

	// For MFC, get the intern values
	if (SkCodeParam::bRenderingEngineMFC) {
		iDeviceMouseX = (int)iCallbackMouseX;	// this is set up from CSkWinMFC::OnMouseMove and such
		iDeviceMouseY = (int)iCallbackMouseY;
		iDeviceButtons = (U32)iCallbackMouseButton;
		*x = U16(iDeviceMouseX);
		*y = U16(iDeviceMouseY);
		*buttons = U16(iDeviceButtons);
	}

		//SkD((DLV_MOUSE,"SDL_GetMouseState\n"));

	//SkD((DLV_MOUSE,"SDL_GetMouseState %d,%d buttons=%d / scale=%d\n", U16(iDeviceMouseX), U16(iDeviceMouseY), U16(*buttons), SkCodeParam::iVideoScale));

	//SkD((DLV_MOUSE,"GetMousePosButtons (%05d): M(%3d,%3d) Btn:(%2d)\n", iCallCount++, iDeviceMouseX, iDeviceMouseY, iDeviceButtons));
}


void SkWinApp::skwin_SndPlayFile(const char* sFilename, i8 vol)
{
	;
}

//..............................................................................

void SkWinApp::processMinput(U8 button, bool pressed, int x, int y)
{
//#if defined(__LINUX__) && defined(__SDL__)
	/*
	x /= sxfact;
	y /= sxfact;
	*/
/*
	if (pressed) {
		switch (button) {
			case SDL_BUTTON_LEFT: curMiceState |= SDL_BUTTON_LMASK; break;
			case SDL_BUTTON_RIGHT: curMiceState |= SDL_BUTTON_RMASK; break;
		}
	}
	else {
		switch (button) {
			case SDL_BUTTON_LEFT: curMiceState &= ~SDL_BUTTON_LMASK; break;
			case SDL_BUTTON_RIGHT: curMiceState &= ~SDL_BUTTON_RMASK; break;
		}
	}

	CSkMinput *p = allocMinput();
	if (p != NULL) {
		p->btn = 0
			| (((curMiceState & SDL_BUTTON_LMASK) != 0) ? 1 : 0)
			| (((curMiceState & SDL_BUTTON_RMASK) != 0) ? 2 : 0)
			;
		p->x = (U16)x;
		p->y = (U16)y;
	}
	*/
//#endif
}

void SkWinApp::processKinput(U32 nChar, bool press)
{
	printf("processKinput %d %d\n", nChar, press);
#if defined(__SDL__)
/*	
	if (press) {
		if (nChar == SDLK_LCTRL) pressCtrl |= 1;
		if (nChar == SDLK_RCTRL) pressCtrl |= 2;
	}
	else {
		if (nChar == SDLK_LCTRL) pressCtrl &= ~1;
		if (nChar == SDLK_RCTRL) pressCtrl &= ~2;
	}
*/
	
	if (press && nChar == SDLK_F1) {
		switch (SkCodeParam::iVideoScale) {
			case 1: SkCodeParam::iVideoScale = 2; break;
			case 2: SkCodeParam::iVideoScale = 3; break;
			case 3: SkCodeParam::iVideoScale = 4; break;
			case 4: SkCodeParam::iVideoScale = 5; break;
			default: SkCodeParam::iVideoScale = 1; break;
		}
		//CreateSurface();
		//printf("New scale : %d\n", SkCodeParam::iVideoScale);
		xSkWinRenderer->ResizeWindow();
		return;
	}
	else if (press && nChar == SDLK_F4) {
		if (SkCodeParam::fVideoYScale != 1)
			SkCodeParam::fVideoYScale = 1.0f;
		else
			SkCodeParam::fVideoYScale = 1.20f;
		xSkWinRenderer->ResizeWindow();
		xSkWinRenderer->Render();
	}
	else if (press && nChar == SDLK_F2) {
		// make slow
		switch (spfact) {
			case 0: case 1: case 2: case 3: case 4:
				spfact++;
				//UpdateTitle();
				break;
		}
		return;
	}
	else if (press && nChar == SDLK_F3) {
		// make fast
		switch (spfact) {
			case 1: case 2: case 3: case 4: case 5:
				spfact--;
				//UpdateTitle();
				break;
		}
		return;
	}

	//CSkKinput *p = SKInput.allocKinput();
	CSkKinput *p = allocKinput();
	//CSkKinput *p = NULL;
	if (p != NULL) {
		U8 v = 0;
		//printf("Keyboard char = %d\n", nChar);
		switch (nChar) {
		case SDLK_ESCAPE: v = 1; break;
		case SDLK_1: v = 2; break;
		case SDLK_2: v = 3; break;
		case SDLK_3: v = 4; break;
		case SDLK_4: v = 5; break;
		case SDLK_5: v = 6; break;
		case SDLK_6: v = 7; break;
		case SDLK_7: v = 8; break;
		case SDLK_8: v = 9; break;
		case SDLK_9: v = 10; break;

		case SDLK_BACKSPACE: v = 14; break;
		case SDLK_TAB: v = 15; break;

		case SDLK_RETURN: v = 28; break;

		case SDLK_KP_7: v = 71; break;
		case SDLK_KP_8: v = 72; break;
		case SDLK_KP_9: v = 73; break;
		case SDLK_KP_4: v = 75; break;
		case SDLK_KP_5: v = 76; break;
		case SDLK_KP_6: v = 77; break;
		case SDLK_KP_1: v = 79; break;
		case SDLK_KP_2: v = 80; break;
		case SDLK_KP_3: v = 81; break;

		case SDLK_q: v = 16; break;
		case SDLK_w: v = 17; break;
		case SDLK_e: v = 18; break;
		case SDLK_r: v = 19; break;
		case SDLK_t: v = 20; break;
		case SDLK_y: v = 21; break;
		case SDLK_u: v = 22; break;
		case SDLK_i: v = 23; break;
		case SDLK_o: v = 24; break;
		case SDLK_p: v = 25; break;

		case SDLK_a: v = 30; break;
		case SDLK_s: v = 31; break;
		case SDLK_d: v = 32; break;
		case SDLK_f: v = 33; break;
		case SDLK_g: v = 34; break;
		case SDLK_h: v = 35; break;
		case SDLK_j: v = 36; break;
		case SDLK_k: v = 37; break;
		case SDLK_l: v = 38; break;

		case SDLK_z: v = 44; break;
		case SDLK_x: v = 45; break;
		case SDLK_c: v = 46; break;
		case SDLK_v: v = 47; break;
		case SDLK_b: v = 48; break;
		case SDLK_n: v = 49; break;
		case SDLK_m: v = 50; break;

		case SDLK_SPACE: v = 57; break;

		//case SDL_SCANCODE_GRAVE: v = 57; break;
/*
		if (event.key.keysym.sym == SDLK_F12 &&
			(event.key.keysym.mod & KMOD_CTRL)) {
			// Ctrl + F12 kill switch
		}*/

		}
		p->raw = (press) ? (v) : (v | 0x80);
	}

#endif
}



//------------------------------------------------------------------------------
#ifdef SKDOSV5
	extern char strVersionNumber[]; // (te_text) dm2data
#else
	extern const X8* const strVersionNumber; // skglobal.cpp
#endif

void SkWinApp::VersionMessage()
{
	// strVersionNumber is in global.h
	printf("\nDungeon Master II PC-9821/PC-DOS for %s\n", __SKWIN_SYSTEM__);
	printf("Unofficial Port by Kentaro-k.21 + Bit + Sphenx (%s - build %s).\n", strVersionNumber, __SKWIN_RELEASE_DATE__);
}


void SkWinApp::StartMessage()
{
	VersionMessage();
}

void SkWinApp::ExitMessage()
{
	VersionMessage();
	printf("\nbye !\n");
}

//------------------------------------------------------------------------------
