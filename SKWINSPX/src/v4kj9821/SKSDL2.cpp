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

bool CSkWinSDL2::ML() {
	return true;
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

void CSkWinSDL2::UpdateRect(i16 x, i16 y, i16 cx, i16 cy)
{
	printf("CSkWinSDL2::UpdateRect\n");
#if defined(__LINUX__) && defined (__SDL__)
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
/*
    // Set each pixel to a random color
    for (int x = 0; x < 640; x++) {
        for (int y = 0; y < 480; y++) {
            // Generate random RGB values
            Uint8 r = std::rand() % 256;
            Uint8 g = std::rand() % 256;
            Uint8 b = std::rand() % 256;

            // Set the color and draw the pixel
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
*/
	//--- convert pixel screen to RGB
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	int iy = 0;
	int ix = 0;
    for (iy = 0; iy < SCREEN_HEIGHT; ++iy) {
        for (ix = 0; ix < SCREEN_WIDTH; ++ix) {
			
			U8 iPalID = vram[iy * SCREEN_WIDTH + ix];  // Copy pixel data
			r = g = b = iPalID; // greyscale
			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
			SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    SDL_RenderPresent(renderer);
#endif // __LINUX__
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
	SkD((DLV_MOUSE,"CSkWinSDL2::GetMousePosButtons: SDL M(%3d,%3d) Btn:(%2d)\n", iSDLMouseX, iSDLMouseY, iSDLButtons));
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
          640, 480, SDL_WINDOW_SHOWN);
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

