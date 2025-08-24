//------------------------------------------------------------------------------

#ifndef _SKCODE_WINAPP_H_
#define _SKCODE_WINAPP_H_

#pragma once

#include <sktypes.h>
#include <skrender.h>

#include <skinput.h>

//class SkRendererSDL;
//class SkRendererDOS;

//#include <sksdl2.h>
//#include <skdos.h>
//#include <skmfc.h>

//------------------------------------------------------------------------------

typedef enum {
	GFX_RENDERER_NONE = -1,
	GFX_RENDERER_WIN = 0,
	GFX_RENDERER_DOS = 1,
	GFX_RENDERER_SDL = 2,
	GFX_RENDERER_MFC = 3,
} eRendererType;

#define DOS_VIDEO_VGA_16 16

class SkVRAM;

//------------------------------------------------------------------------------


class SkWinApp
{
private:
	UINT	iVideoRenderer;
//	SkRendererSDL* xRendererSDL;
//	SkRendererDOS* xRendererDOS;
//	SkRendererMFC* xRendererMFC;
	SkRendererGeneric* xSkWinRenderer;
	SkVRAM*	xVRAM;
	//CSkWin	SKInput;

public:
//	SkWinCore* skw;
	int sxfact;
	int sblast;
	int sbdsbf;
	int sbdsaf;
	int spfact;
	int lang;
	int dung;
	int freq;
	int sksave;		// savegame to resume
	int newgame;	// start new game
	char*	sCustomGraphicsDatFilename;
	char*	sCustomDungeonDatFilename;
	char*	sCustomDataFolder;


	//--- main memory used within SK Core
	X8* xSKCRAM;
	X8* xSKCEMS;
	X8* xSKVRAM;
	U32 iSizeCRAM;
	U32 iSizeCEMS;
	U32 iSizeVRAM;

public:
	SkWinApp();
	virtual ~SkWinApp();

	static const char* getBuildDateISO();

	void ProcessArgs(int argc, char** argv);
	void MemInfo();

	UINT runWindowTest();
	UINT runSplashTitle();

	UINT setRenderer(eRendererType iRenderer);

	UINT startAudioTest();

	UINT setVideoMode();
	UINT setVideoMode(UINT iSpecificMode);
	UINT setVideoTestStatic(UINT iFrames);
	UINT setVideoTestGradient();
	UINT setVideoTestTitle();
	UINT resetVideoMode();

	UINT renderScreen(i16 x, i16 y, i16 cx, i16 cy);
	UINT renderScreen(X8* xVRAMArray, i16 x, i16 y, i16 cx, i16 cy);

	UINT setRGBPalette(X8* xPalette);
	UINT setRGB6Palette(X8* xPalette);

	X8* getVRAM();

	void VersionMessage();
	void StartMessage();
	void ExitMessage();

//-- old compatibility
	const X8* GET_DATA_FOLDER_NAME();

	void skwin_SndPlayHi(const U8 *buff, U32 buffSize, i8 vol);
	void skwin_SndPlayLo(const U8 *buff, U32 buffSize, i8 dX, i8 dY);

	X8 skwin_GetLang();
	void skwin_SndSetFrequency(U16 iSoundPlaybackFrequency);
	void skwin_ShowMessage(const char *psz);
	void skwin_Sleep(U32 millisecs);
	U32 skwin_GetTickCount();
	bool skwin_IsAvail();
	bool skwin_ML();

//------------------------------------------------------------------------------
// CSkWin part for Minput / Kinput
	CSkMinput xMiceInput[MAXMICEIN];
	int cntMiceIn;
	CSkKinput xKeybInput[MAXKEYBIN];
	int cntKeybIn;
	int curKeybIn;

	CSkKinput *DequeueKinput();/* {
		CSkKinput *p = NULL;
		if (cntKeybIn != 0) {
			p = &xKeybInput[curKeybIn];
			curKeybIn = (curKeybIn +1) % MAXKEYBIN;
			cntKeybIn--;
		}
		return p;
	}*/

	CSkMinput *allocMinput() {
		if (cntMiceIn < MAXMICEIN) {
			CSkMinput *p = &xMiceInput[cntMiceIn];
			cntMiceIn++;
			return p;
		}
		return NULL;
	}
	CSkKinput *allocKinput() {
		if (cntKeybIn < MAXKEYBIN) {
			CSkKinput *p = &xKeybInput[(curKeybIn + cntKeybIn) % MAXKEYBIN];
			cntKeybIn++;
			return p;
		}
		return NULL;
	}

	void GetMousePosButtons(U16 *x, U16 *y, U16 *buttons);
	void skwin_SndPlayFile(const char* sFilename, i8 vol);
	void processMinput(U8 button, bool pressed, int x, int y);
	void processKinput(U32 nChar, bool press);
//------------------------------------------------------------------------------


	void LogParams();
};

//------------------------------------------------------------------------------

#endif // _SKCODE_WINAPP_H_