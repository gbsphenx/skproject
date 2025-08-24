//------------------------------------------------------------------------------

#include <skver.h>

#include <sktypes.h>

#include <skparam.h>
#include <skvram.h>

#include <skwinapp.h>

#include <sksdl2.h>

#if defined (__NO_SDL__)
#elif defined (__LINUX__)
	#include <SDL2/SDL.h>
#elif !defined (__DJGPP__)
	#include <SDL.h>
//	#include <SDL_mixer.h>
#endif // __LINUX__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



//..............................................................................


//------------------------------------------------------------------------------


//#define WIDTH 320
//#define HEIGHT 200

//------------------------------------------------------------------------------

SkRendererSDL::SkRendererSDL()
{
	;
}

SkRendererSDL::~SkRendererSDL()
{
	;
}

UINT SkRendererSDL::Init(SkVRAM* xVRAM)
{
	char sSDLWindowName[128];
	
	SkRendererGeneric::Init(xVRAM);

	memset(sSDLWindowName, 0, 128);
	sprintf(sSDLWindowName, "%s <%s> [SDL]", __SKWIN_PROGNAME__, __SKWIN_SYSTEM__);

#if !defined (__NO_SDL__)
	printf("Starting SDL init...\n");
	//if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER|SDL_INIT_EVENTS) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

	this->xVRAM = xVRAM;
	//iScreenWidth = iScreenWidth;
	//iScreenHeight = iScreenHeight;
	xRGBBuffer = (uint32_t*) calloc (iScreenWidth * iScreenHeight, sizeof (uint32_t));

    sdlWindow = SDL_CreateWindow(sSDLWindowName,
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  iScreenWidth*SkCodeParam::iVideoScale, (int)(iScreenHeight*(SkCodeParam::iVideoScale*SkCodeParam::fVideoYScale)),
                                  SDL_WINDOW_SHOWN);
    if (!sdlWindow) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!sdlRenderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();
        return 1;
    }

    sdlTexture = SDL_CreateTexture(sdlRenderer,
                                             SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             iScreenWidth, iScreenHeight);
	
//	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//SDL_SetPaletteColors(SDL_AllocPalette(256), palette, 0, 256);

    for (UINT i = 0; i < 256; i++) {
        sdlPalette[i].r = (U8) i;
        sdlPalette[i].g = (U8) i;
        sdlPalette[i].b = (U8) i;
    }

	SDL_ShowCursor(SDL_DISABLE);

#endif // not __NO_SDL__
	return 0;
}


UINT SkRendererSDL::ResizeWindow()
{
#if !defined (__NO_SDL__)
	SDL_SetWindowSize(sdlWindow, iScreenWidth*SkCodeParam::iVideoScale, (int)(iScreenHeight*SkCodeParam::iVideoScale*SkCodeParam::fVideoYScale));
#endif // not __NO_SDL__
	return 0;
}


UINT SkRendererSDL::SetVGAPaletteRGB(X8 *xVGAPalette)
{
#if !defined (__NO_SDL__)
    for (UINT i = 0; i < 256; i++) {
        sdlPalette[i].r = (U8) (xVGAPalette[i * 4 + 0]);
        sdlPalette[i].g = (U8) (xVGAPalette[i * 4 + 1]);
        sdlPalette[i].b = (U8) (xVGAPalette[i * 4 + 2]);
		sdlPalette[i].a = (U8) (xVGAPalette[i * 4 + 3]);
    }
#endif // not __NO_SDL__
	return 0;
}

UINT SkRendererSDL::ConvertVRAMToRGB()
{
#if !defined (__NO_SDL__)
	X8* xVRAMData = NULL;
	if (xVRAM != NULL) {
		xVRAMData = xVRAM->GET_VIDEO_ARRAY();
		SetVGAPaletteRGB(xVRAM->GET_PALETTE());
	}

    for (UINT i = 0; i < iScreenWidth * iScreenHeight; i++) {
        SDL_Color c = sdlPalette[xVRAMData[i]];

		xRGBBuffer[i] = (c.r << 24) | (c.g << 16) | (c.b << 8) | c.a;
    }
	SDL_UpdateTexture(sdlTexture, NULL, xRGBBuffer, iScreenWidth * sizeof(uint32_t));

#endif // not __NO_SDL__
	return 0;
}


UINT SkRendererSDL::Render()
{
#if !defined (__NO_SDL__)
	ConvertVRAMToRGB();
	//SDL_Rect src = { 0, 0, 160, 100 };   // from texture
	SDL_Rect dst = { 0, 0, (int)iScreenWidth, (int)iScreenHeight }; // to screen
	SDL_SetRenderDrawColor(sdlRenderer, 0, 128, 0, 255);
    SDL_RenderClear(sdlRenderer);
	SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
	//printf("Render = %08X, Texture = %08X\n", sdlRenderer, sdlTexture);
	//SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &dst);
    SDL_RenderPresent(sdlRenderer);
#endif // not __NO_SDL__
	return 0;
}

UINT SkRendererSDL::Close()
{
#if !defined (__NO_SDL__)
	SDL_DestroyTexture(sdlTexture);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);

	if (sdlAudio != NULL)
		SDL_CloseAudioDevice(sdlAudio);

    SDL_Quit();
#endif // not __NO_SDL__
	return 0;
}



UINT SkRendererSDL::StartAudioSample(const char* sSampleName)
{
#if !defined (__NO_SDL__)
	printf("Sample %s\n", sSampleName);

	SDL_AudioSpec xWavSample;
    X8* xWavData;
    U32 iWavLength;

    // Load the WAV
    if (SDL_LoadWAV(sSampleName, &xWavSample, &xWavData, &iWavLength) == NULL) {
        printf("Could not open %s: %s\n", sSampleName, SDL_GetError());
        SDL_Quit();
        return 1;
    }	

    xWavSample.callback = NULL;
    xWavSample.userdata = NULL;

    sdlAudio = SDL_OpenAudioDevice(NULL, 0, &xWavSample, NULL, 0);
    if (sdlAudio == NULL) {
        printf("SDL_OpenAudioDevice error: %s\n", SDL_GetError());
        SDL_FreeWAV(xWavData);
		return 1;
    }

    SDL_QueueAudio(sdlAudio, xWavData, iWavLength);
    SDL_PauseAudioDevice(sdlAudio, 0);

#endif // not __NO_SDL__
	return 0;
}
