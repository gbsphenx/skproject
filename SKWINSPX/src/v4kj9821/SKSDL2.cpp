//--- SkSDL2 -------------------------------------
//SPX: SkSDL2 replaces SkMFC when compiled for Linux

#include <StdAfx.h>
#include <DME.h>	// defining types for skwin
#include <SKSDL2.h>
#include <SKGlobal.h>

#include <SKDebug.h>

#include <stdlib.h>
#include <time.h>

#if defined(__LINUX__) && defined(__SDL__)
#include <SDL2/SDL.h>
#include <iostream>
#endif // __LINUX__

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#define VRAM_SCREEN_WIDTH 320
#define VRAM_SCREEN_HEIGHT 200

#define SCREEN_SCALE 4
#define SCREEN_RATIO_Y	1.2f

#define SCREEN_WIDTH_FULL	(int)(SCREEN_WIDTH*SCREEN_SCALE)
#define SCREEN_HEIGHT_FULL	(int)(SCREEN_HEIGHT*SCREEN_SCALE*SCREEN_RATIO_Y)


CSkWinSDL2::CSkWinSDL2()
{
	sxfact = 1;
	sblast = 1;
	sbdsbf = 0;
	sbdsaf = 0;
	spfact = 0;
	lang = _GDAT_LANG_ENGLISH_;
	dung = _OPTION_DUNGEON_DM2_SK_;
	sksave = -1;
	newgame = 0;
}

CSkWinSDL2::~CSkWinSDL2()
{
#if defined(__LINUX__) && defined(__SDL__)
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();	
#endif // __LINUX__
	;
}


// Fake methods

void CSkWinSDL2::OnPaint()
{
	;
}

/*

typedef struct SDL_Keysym {
    SDL_Scancode scancode; // Hardware-independent scancode
    SDL_Keycode sym;       // Keycode representing the key
    Uint16 mod;            // Current key modifiers (e.g., Shift, Ctrl)
    Uint32 unused;         // Reserved for future use
} SDL_Keysym;
*/

bool CSkWinSDL2::ML() {
#if defined(__LINUX__) && defined(__SDL__)
	while (true) {
		SDL_Event event;
		SDL_PumpEvents();
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					processKinput(event.key.keysym.sym, true);
					break;
				case SDL_KEYUP:
					processKinput(event.key.keysym.sym, false);
					break;
				case SDL_MOUSEMOTION:
					processMinput(-1, false, event.motion.x, event.motion.y);
					break;
				case SDL_MOUSEBUTTONDOWN:
					processMinput(event.button.button, true, event.button.x, event.button.y);
					break;
				case SDL_MOUSEBUTTONUP:
					processMinput(event.button.button, false, event.button.x, event.button.y);
					break;
				case SDL_QUIT:
					return false;
				default:
					break;
			}
		}
		else {
			//paint(pScreen);
			//SDL_Flip(pScreen);
			this->Sleep(1);
			break;
		}
	}
#endif
	return true;
}

// -- * -- * --
void CSkWinSDL2::processMinput(U8 button, bool pressed, int x, int y)
{
#if defined(__LINUX__) && defined(__SDL__)
	x /= sxfact;
	y /= sxfact;
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
#endif
}
// -- * -- * --
void CSkWinSDL2::processKinput(SDL_Keycode nChar, bool press)
{
#if defined(__LINUX__) && defined(__SDL__)
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
		switch (sxfact) {
			case 1: sxfact = 2; break;
			case 2: sxfact = 3; break;
			case 3: sxfact = 4; break;
			default: sxfact = 1; break;
		}
		//CreateSurface();
		return;
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

	CSkKinput *p = allocKinput();
	if (p != NULL) {
		U8 v = 0;
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
		}
		p->raw = (press) ? (v) : (v | 0x80);
	}

#endif
}

void CSkWinSDL2::ProcessArgs(int argc, char** argv)
{
	unsigned int iArgIndex = 0;

	printf("CSkWinSDL2::ProcessArgs\n");

	sCustomGraphicsDatFilename = NULL;
	sCustomDungeonDatFilename = NULL;
	sCustomDataFolder = NULL;

	while (iArgIndex < argc)
	{
		if (argv[iArgIndex] == NULL)
			break;

		if (!strcmp(argv[iArgIndex], "-en"))
			lang = _GDAT_LANG_ENGLISH_;	// english
		else if (!strcmp(argv[iArgIndex], "-jp"))
			lang = _GDAT_LANG_JAPANESE_;	// japanese
		else if (!strcmp(argv[iArgIndex], "-de"))
			lang = _GDAT_LANG_GERMAN_;	// german
		else if (!strcmp(argv[iArgIndex], "-fr"))
			lang = _GDAT_LANG_FRENCH_;	// french
		else if (!strcmp(argv[iArgIndex], "-es"))
			lang = _GDAT_LANG_SPANISH_;	// spanish
		else if (!strcmp(argv[iArgIndex], "-it"))
			lang = _GDAT_LANG_ITALIAN_;	// italian

		else if (!strcmp(argv[iArgIndex], "-r0"))	// resume game sksave0
		{
			sksave = 0;
			newgame = 2;
		}
		else if (!strcmp(argv[iArgIndex], "-resume"))	// go to resume screen
			newgame = 2;
		else if (!strcmp(argv[iArgIndex], "-new"))	// directly start new game
			newgame = 1;


		else if (!strcmp(argv[iArgIndex], "-dm1_kid"))
			dung = _OPTION_DUNGEON_DM1_KID_;
		else if (!strcmp(argv[iArgIndex], "-dm1_dm"))
			dung = _OPTION_DUNGEON_DM1_DM_;
		else if (!strcmp(argv[iArgIndex], "-dm1_csb"))
			dung = _OPTION_DUNGEON_DM1_CSB_;
		else if (!strcmp(argv[iArgIndex], "-dm1_tq"))
			dung = _OPTION_DUNGEON_DM1_TQ_;
		else if (!strcmp(argv[iArgIndex], "-dm2_dm"))
			dung = _OPTION_DUNGEON_DM2_DM_;
		else if (!strcmp(argv[iArgIndex], "-dm2_csb"))
			dung = _OPTION_DUNGEON_DM2_CSB_;
		else if (!strcmp(argv[iArgIndex], "-dm2_tq"))
			dung = _OPTION_DUNGEON_DM2_TQ_;
		else if (!strcmp(argv[iArgIndex], "-dm2_beta"))
			dung = _OPTION_DUNGEON_DM2_SK_BETA_;
		else if (!strcmp(argv[iArgIndex], "-dm2_demo"))
			dung = _OPTION_DUNGEON_DM2_SK_DEMO_;
		else if (!strcmp(argv[iArgIndex], "-dm2_sk"))
			dung = _OPTION_DUNGEON_DM2_SK_;
		else if (!strcmp(argv[iArgIndex], "-dm2_skx"))
			dung = _OPTION_DUNGEON_DM2_SK_EXT_;

#ifndef __SKWIN_PUBLIC_VERSION__
		else if (!strcmp(argv[iArgIndex], "-bwy_bw"))
			dung = _OPTION_DUNGEON_BWY_BW_;
		else if (!strcmp(argv[iArgIndex], "-eob_eob1"))
			dung = _OPTION_DUNGEON_EOB_EOB1_;
		else if (!strcmp(argv[iArgIndex], "-dmx"))
			dung = _OPTION_DUNGEON_DMX_;
#endif

		else if (!strcmp(argv[iArgIndex], "-fhz6"))
			freq = 6;
		else if (!strcmp(argv[iArgIndex], "-fhz11"))
			freq = 11;

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

		iArgIndex++;
	}

}

unsigned char CSkWinSDL2::GetLang()
{
	return (unsigned char)lang;
}

void CSkWinSDL2::SndPlayLo(const U8 *buff, U32 buffSize, i8 dX, i8 dY)
{
	;
}

void CSkWinSDL2::SndPlayHi(const U8 *buff, U32 buffSize, i8 vol)
{
	;
}

void CSkWinSDL2::SndPlayFile(const char* sFilename, i8 vol)
{
	;
}


void CSkWinSDL2::SndSetFrequency(U16 iSoundPlaybackFrequency)
{
	;
}

void CSkWinSDL2::ShowMessage(const char *psz) {
	;
}

#ifndef __LINUX__
	#define SDL_SetRenderDrawColor(Z, R, G, B, A) ;
	#define SDL_RenderClear(Z) ;
	#define SDL_RenderDrawPoint(Z, X, Y) ;
	#define SDL_RenderPresent(Z) ;
#endif


void CSkWinSDL2::UpdateRect(i16 x, i16 y, i16 cx, i16 cy)
{
	printf("CSkWinSDL2::UpdateRect\n");
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

	//--- convert pixel screen to RGB
	U8 r = 0;
	U8 g = 0;
	U8 b = 0;
	int iy = 0;
	int ix = 0;
	/// SCALE 1-1
	/*
    for (iy = 0; iy < SCREEN_HEIGHT; iy++) {
        for (ix = 0; ix < SCREEN_WIDTH; ix++) {
			
			U8 iPalID = vram[iy * SCREEN_WIDTH + ix];  // Copy pixel data
			r = g = b = iPalID*4; // greyscale
			r = glbPaletteRGB[iPalID][0]*4;
			g = glbPaletteRGB[iPalID][1]*4;
			b = glbPaletteRGB[iPalID][2]*4;
			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
			SDL_RenderDrawPoint(renderer, ix, iy);
        }
    }
	*/
	int vy = 0;	// vram y	(1-200)
	int vx = 0;	// vram x	(1-320)

	int lx = 0;
	int ly = 0;

	int rx = 0;	// render x (1-320*SCALE)
	int ry = 0;	// render y (1-200*SCALE*RATIO)

	ry = 0;

	int iExtraPixelsY = (int)(SCREEN_HEIGHT*SCREEN_SCALE*SCREEN_RATIO_Y) - (int)(SCREEN_HEIGHT*SCREEN_SCALE);
	int iExtraStep = (int)(((int)(SCREEN_HEIGHT*SCREEN_SCALE)) / iExtraPixelsY);
	//printf("SCREEN %d * %d / EXTRA = %d\n", SCREEN_WIDTH_FULL, SCREEN_HEIGHT_FULL, iExtraStep);

    for (vy = 0; vy < VRAM_SCREEN_HEIGHT; vy++) {
		for (ly = 0; ly < SCREEN_SCALE; ly++) {
			int iExtraLine = 0;
			if ((ry%iExtraStep) == 0) {
				iExtraLine = 1;
			}
			for (; iExtraLine >= 0; iExtraLine--) {
				rx = 0;
				for (vx = 0; vx < VRAM_SCREEN_WIDTH; vx++) {
					U8 iPalID = vram[vy * VRAM_SCREEN_WIDTH + vx];  // Copy pixel data
					r = g = b = iPalID*4; // greyscale
					r = glbPaletteRGB[iPalID][0]*4;
					g = glbPaletteRGB[iPalID][1]*4;
					b = glbPaletteRGB[iPalID][2]*4;

					for (lx = 0; lx < SCREEN_SCALE; lx++) {
						SDL_SetRenderDrawColor(renderer, r, g, b, 255);
						SDL_RenderDrawPoint(renderer, rx, ry);
						rx++;
					}
				}
				ry++;
			}
		}
    }

    SDL_RenderPresent(renderer);
}

bool CSkWinSDL2::IsAvail() 
{
	return true;
}

U32 CSkWinSDL2::GetTickCount() 
{
    return (U32)(clock() * 1000 / CLOCKS_PER_SEC);
}
void CSkWinSDL2::Sleep(U32 millisecs)
{
#ifdef __DJGPP__
	usleep(millisecs * 1000);
#endif
}

void CSkWinSDL2::GetMousePosButtons(U16 *x, U16 *y, U16 *buttons) 
{
	int iSDLMouseX = 0;
	int iSDLMouseY = 0;
	U32 iSDLButtons = 0;
#if defined(__LINUX__)
	iSDLButtons = (U32) SDL_GetMouseState(&iSDLMouseX, &iSDLMouseY);
#endif // __LINUX__
	*x = U16(iSDLMouseX);
	*y = U16(iSDLMouseY);
	*buttons = 0;
#if defined (__LINUX__)
	if (iSDLButtons & SDL_BUTTON(SDL_BUTTON_LEFT))
		*buttons += 1; // left
	if (iSDLButtons & SDL_BUTTON(SDL_BUTTON_RIGHT))
		*buttons += 2; // right
	if (iSDLButtons & SDL_BUTTON(SDL_BUTTON_MIDDLE))
		*buttons += 4; // middle
#endif // __LINUX__
	//SkD((DLV_MOUSE,"CSkWinSDL2::GetMousePosButtons: SDL M(%3d,%3d) Btn:(%2d)\n", iSDLMouseX, iSDLMouseY, iSDLButtons));
}
	


void CSkWinSDL2::VersionMessage()
{
	printf("\nDungeon Master II PC-9821/PC-DOS for Linux\n");
	printf("Unofficial Port by Kentaro-k.21 + Sphenx (%s - build %s).\n", strVersionNumber, __SKWIN_RELEASE_DATE__);
}


void CSkWinSDL2::StartMessage()
{
	VersionMessage();
}

void CSkWinSDL2::InitVideo()
{
	printf("CSkWinSDL2::InitVideo\n");
#if defined(__LINUX__) && defined (__SDL__)
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return;
    }
	printf("SDL_Init::done\n");
    

    window = SDL_CreateWindow("SKWin-9821-Linux", 
          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
          SCREEN_WIDTH_FULL, SCREEN_HEIGHT_FULL, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }
	printf("SDL_CreateWindow::done\n");

    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
	printf("SDL_CreateRenderer::done\n");
#endif // __LINUX__
}


void CSkWinSDL2::ResetVideo()
{

}

void CSkWinSDL2::ExitMessage()
{
#if defined(__LINUX__) && defined(__SDL__)
    SDL_Quit();
#endif // __LINUX__
	VersionMessage();
	printf("\nbye !\n");
}

