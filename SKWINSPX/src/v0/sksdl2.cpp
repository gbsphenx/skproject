//------------------------------------------------------------------------------

#include <skver.h>

#include <sktypes.h>

#include <skparam.h>
#include <skvram.h>

#include <skwinapp.h>

#include <sksdl2.h>

#include <skdebug.h>

#if defined (__NO_SDL__)
#elif defined (__LINUX__)
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_mixer.h>
#elif !defined (__DJGPP__)
	#include <SDL.h>
	#include <SDL_mixer.h>
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


UINT SkRendererSDL::InitAudio()
{
#if !defined (__NO_SDL__)
    SDL_AudioSpec want = {0};

    //want.freq = 11025;
    want.freq = 6000;	// V4
    want.format = AUDIO_S8;
    want.channels = 1;
    want.samples = 1024;
    want.callback = NULL;           // use queueing
/*
    sdlAudioDeviceId = SDL_OpenAudioDevice(
        NULL, 0, &want, NULL, 0);

    SDL_PauseAudioDevice(sdlAudioDeviceId, 0);
*/
	if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024) < 0) {
	   printf("Mixer error: %s\n", Mix_GetError());
	}


#endif // not __NO_SDL__
	return 0;
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

	//SDL_SetPaletteColors(SDL_AllocPalette(256), palette, 0, 256);

    for (UINT i = 0; i < 256; i++) {
        sdlPalette[i].r = (U8) i;
        sdlPalette[i].g = (U8) i;
        sdlPalette[i].b = (U8) i;
    }

	SDL_ShowCursor(SDL_DISABLE);

	InitAudio();

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

	if (sdlAudioDeviceId != 0) {
		Mix_CloseAudio();
		//SDL_CloseAudioDevice(sdlAudioDeviceId);
	}

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
    if (SDL_LoadWAV(sSampleName, &xWavSample, (Uint8**) &xWavData, (Uint32*) &iWavLength) == NULL) {
        printf("Could not open %s: %s\n", sSampleName, SDL_GetError());
        SDL_Quit();
        return 1;
    }	

    xWavSample.callback = NULL;
    xWavSample.userdata = NULL;

    sdlAudioDeviceId = SDL_OpenAudioDevice(NULL, 0, &xWavSample, NULL, 0);
    if (sdlAudioDeviceId == 0) {
        printf("SDL_OpenAudioDevice error: %s\n", SDL_GetError());
        SDL_FreeWAV((Uint8*)xWavData);
		return 1;
    }

    SDL_QueueAudio(sdlAudioDeviceId, xWavData, iWavLength);
    SDL_PauseAudioDevice(sdlAudioDeviceId, 0);

#endif // not __NO_SDL__
	return 0;
}


bool SkRendererSDL::ML()
{
//#if defined(__SDL__) && !defined(__NO_SDL__)
#if !defined (__NO_SDL__)
	while (true) {
		//SkD((DLV_MOUSE,"before SDL_PumpEvents\n"));
		SDL_Event event;
		SDL_PumpEvents();
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
				//SkD((DLV_MOUSE,"Event type = %d\n", event.type));
				case SDL_KEYDOWN:
					xMasterWinApp->processKinput(event.key.keysym.sym, true);
					break;
				case SDL_KEYUP:
					xMasterWinApp->processKinput(event.key.keysym.sym, false);
					break;
				case SDL_MOUSEMOTION:
					xMasterWinApp->processMinput(-1, false, event.motion.x, event.motion.y);
					break;
				case SDL_MOUSEBUTTONDOWN:
					xMasterWinApp->processMinput(event.button.button, true, event.button.x, event.button.y);
					break;
				case SDL_MOUSEBUTTONUP:
					xMasterWinApp->processMinput(event.button.button, false, event.button.x, event.button.y);
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
			xMasterWinApp->skwin_Sleep(1);
			break;
		}
	}
#endif // __SDL__
	//SkD((DLV_MOUSE,"Ended SDL_PumpEvents\n"));
	//xMasterWinApp->skwin_Sleep(1);
	return true;
}
#include <math.h>
/* SDL_QueueAudio version
UINT SkRendererSDL::AudioPlaySound(const U8 *xSoundBuffer, U32 iBufferSize, i8 iSoundVolume, U16 iPlaybackFrequency)
{
#if !defined (__NO_SDL__)
	static int cnt = 0;
	static Uint8 buffInt[65536];   // final PCM buffer
    uint32_t x;
    int c;

    if (cnt != 0) {
        SDL_ClearQueuedAudio(sdlAudioDeviceId); // equivalent to SND_PURGE
    }
//	printf("Volume = %d => scale = %f\n", iSoundVolume, (float)(iSoundVolume / 16.f));
    for (x = 0; x < iBufferSize; x++) {	// original sound is signed 8bits (0 at 0x80)
		c = (xSoundBuffer[x]);
		if (c >= 0 && c < 0x80)
			c = (int)((float)c * (float)(iSoundVolume / 16.f));
		if (c > 0x80)	// value x80 to xFF
			c = ((int)(((float)(signed char)c) * (float)(iSoundVolume / 16.f)));
		buffInt[x] = (Uint8)(c);
//		printf("BUFF%05d: %02X V(%f)> %02X\n", x, xSoundBuffer[x], (float)(iSoundVolume / 16.f), buffInt[x]);
    }

    SDL_QueueAudio(sdlAudioDeviceId, buffInt, iBufferSize);
    cnt |= 1;

#endif // not __NO_SDL__
	return 0;
}
*/

#if !defined (__NO_SDL__)
Mix_Chunk* MakeMixChunkFromPCM(const U8* buffInt, Uint32 buffSize, int volume)
{
    Mix_Chunk* chunk = (Mix_Chunk*)malloc(sizeof(Mix_Chunk));
    if (!chunk) return NULL;

    chunk->abuf = (Uint8*)malloc(buffSize);
    if (!chunk->abuf)
    {
        free(chunk);
        return NULL;
    }

    memcpy(chunk->abuf, buffInt, buffSize);
    chunk->alen = buffSize;
    chunk->volume = volume;   // 0-128
    chunk->allocated = 1;     // mixer frees abuf when freeing chunk
    return chunk;
}
#endif // not __NO_SDL__

#if !defined (__NO_SDL__)
Mix_Chunk* MakeMixChunkFromConvertedPCM(const U8* xSoundBuffer, Uint32 iBufferSize, int volume)
{
	SDL_AudioCVT cvt;
	int iAudioFrequency = 6000; // PC-98xx V4
	if (SkCodeParam::bDM2V5Mode)
		iAudioFrequency = 11025;	// PC-DOS V5

	SDL_BuildAudioCVT(
		&cvt,
		AUDIO_S8,        // source format (example: unsigned 8-bit)
		1,               // source channels (mono)
		iAudioFrequency, // source frequency
		AUDIO_S16SYS,    // destination format
		2,               // destination channels (stereo)
		44100            // destination frequency
	);

	cvt.len = iBufferSize;   // size of original buffer in bytes
	cvt.buf = (Uint8*)SDL_malloc(cvt.len * cvt.len_mult);

	memcpy(cvt.buf, xSoundBuffer, iBufferSize);

	SDL_ConvertAudio(&cvt);
	Uint8* resampledBuffer = cvt.buf;
	int    resampledSize   = cvt.len_cvt;

	Mix_Chunk* chunk = (Mix_Chunk*)malloc(sizeof(Mix_Chunk));

	chunk->abuf = resampledBuffer;
	chunk->alen = resampledSize;
	chunk->volume = MIX_MAX_VOLUME;
	chunk->allocated = 1;   // mixer will free abuf

    return chunk;
}
#endif // not __NO_SDL__



UINT SkRendererSDL::AudioPlaySound(const U8* xSoundBuffer, U32 iBufferSize, i8 iSoundVolume, U16 iPlaybackFrequency)
{
#if !defined (__NO_SDL__)
	static int cnt = 0;

    Mix_Chunk* chunk = MakeMixChunkFromConvertedPCM(xSoundBuffer, iBufferSize, 128); // full volume
	int channel = Mix_PlayChannel(-1, chunk, 0); // play once

    cnt |= 1;

#endif // not __NO_SDL__
	return 0;
}



UINT SkRendererSDL::AudioPlayFile(const char* sFilename, i8 iSoundVolume)
{
#if !defined (__NO_SDL__)
	printf("SDL: play sound: %s (@%d)\n", sFilename, iSoundVolume);
	Mix_Chunk* sound = Mix_LoadWAV(sFilename);
	if (!sound) {
		printf("Failed to load WAV: %s\n", Mix_GetError());
	}
	else {
		Mix_VolumeChunk(sound, (int)((float)iSoundVolume*128.f / 100.f));
		Mix_PlayChannel(-1, sound, 0);
	}
#endif // not __NO_SDL__
	return 0;
}


UINT SkRendererSDL::AudioStop()
{
#if !defined (__NO_SDL__)
	Mix_HaltMusic();
	Mix_HaltChannel(-1);
#endif // not __NO_SDL__
	return 0;
}
