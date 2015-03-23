
#include "StdAfx.h"
#include "DME.h"
#include <windows.h>
#include <math.h>

#ifdef _USE_SDL

#include "SkwinSDL.h"

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "user32.lib")

extern U8 vram[65536];
extern U8 _04bf_09f0[256][3];

U8 tbl_gammac[64];

// -- * -- * --
inline U8 Gammac(BYTE v) {
	return tbl_gammac[v];
}

// -- * -- * --
CSkWinSDL::CSkWinSDL() {
	curMiceState = 0;
	sxfact = 1;
	spfact = 2;
	pScreen = NULL;
	pressCtrl = 0;

	//for (int x=0; x<64; x++) tbl_gammac[x] = U8(pow((double)x / 63.0, 0.5) * 255.0);
	for (int x=0; x<64; x++) tbl_gammac[x] = U8((double)x / 63.0 * 255.0);
}
// -- * -- * --
void CSkWinSDL::paint(SDL_Surface *surface) {
#if SDL_VERSION_ATLEAST(1,3,0)
	if (SDL_LockSurface(surface) == 0) {
		switch (sxfact) {
		case 1: default:
			memcpy(surface->pixels, vram, 65536);
			break;
		case 2:
			{
				U8 *pbDest = reinterpret_cast<U8 *>(surface->pixels);
				const U8 *pbSrc = vram;
				for (int y=0; y<400; y++) {
					for (int x=0; x<640; x++) {
						pbDest[640*y +x] = pbSrc[320*(y/2) +(x/2)];
					}
				}
				break;
			}
		case 3:
			{
				U8 *pbDest = reinterpret_cast<U8 *>(surface->pixels);
				const U8 *pbSrc = vram;
				for (int y=0; y<600; y++) {
					for (int x=0; x<960; x++) {
						pbDest[960*y +x] = pbSrc[320*(y/3) +(x/3)];
					}
				}
				break;
			}
		case 4:
			{
				U8 *pbDest = reinterpret_cast<U8 *>(surface->pixels);
				const U8 *pbSrc = vram;
				for (int y=0; y<800; y++) {
					for (int x=0; x<1280; x++) {
						pbDest[1280*y +x] = pbSrc[320*(y/4) +(x/4)];
					}
				}
				break;
			}
		}
		SDL_UnlockSurface(surface);

		SDL_Color clrs[256];
		for (int x=0; x<256; x++)
			clrs[x].r = Gammac(_04bf_09f0[x][0]),
			clrs[x].g = Gammac(_04bf_09f0[x][1]),
			clrs[x].b = Gammac(_04bf_09f0[x][2]);
		SDL_SetPalette(surface, SDL_PHYSPAL, clrs, 0, 256);
	}
#else
	SDL_Surface *surfMine = SDL_CreateRGBSurface(0, 320*sxfact, 200*sxfact, 8, 0, 0, 0, 0);
	if (surfMine != NULL) {
		if (SDL_LockSurface(surfMine) == 0) {
			switch (sxfact) {
			case 1: default:
				memcpy(surfMine->pixels, vram, 320*200);
				break;
			case 2:
				{
					U8 *pbDest = reinterpret_cast<U8 *>(surfMine->pixels);
					const U8 *pbSrc = vram;
					for (int y=0; y<400; y++) {
						for (int x=0; x<640; x++) {
							pbDest[640*y +x] = pbSrc[320*(y/2) +(x/2)];
						}
					}
					break;
				}
			case 3:
				{
					U8 *pbDest = reinterpret_cast<U8 *>(surfMine->pixels);
					const U8 *pbSrc = vram;
					for (int y=0; y<600; y++) {
						for (int x=0; x<960; x++) {
							pbDest[960*y +x] = pbSrc[320*(y/3) +(x/3)];
						}
					}
					break;
				}
			case 4:
				{
					U8 *pbDest = reinterpret_cast<U8 *>(surfMine->pixels);
					const U8 *pbSrc = vram;
					for (int y=0; y<800; y++) {
						for (int x=0; x<1280; x++) {
							pbDest[1280*y +x] = pbSrc[320*(y/4) +(x/4)];
						}
					}
					break;
				}
			}
			SDL_UnlockSurface(surfMine);

			SDL_Color clrs[256];
			for (int x=0; x<256; x++)
				clrs[x].r = Gammac(_04bf_09f0[x][0]),
				clrs[x].g = Gammac(_04bf_09f0[x][1]),
				clrs[x].b = Gammac(_04bf_09f0[x][2]);
			SDL_SetPalette(surfMine, SDL_LOGPAL, clrs, 0, 256);

			if (SDL_BlitSurface(surfMine, NULL, surface, NULL) == 0) {
				printf("");
			}
		}
		SDL_FreeSurface(surfMine);
	}
#endif
}
// -- * -- * --
void CSkWinSDL::UpdateRect(i16 x, i16 y, i16 cx, i16 cy) {
}
// -- * -- * --
void CSkWinSDL::SndPlayHi(const U8 *buff, U32 buffSize, i8 vol) {
}
// -- * -- * --
void CSkWinSDL::SndPlayLo(const U8 *buff, U32 buffSize, i8 dX, i8 dY) {
	SDL_LockAudio();
	for (int x = 0; x < MAX_SB; x++) {
		if (sbs[x].IsFree()) {
			sbs[x].Alloc(buff, buffSize, 0 +abs(dX) +abs(dY));
			break;
		}
	}
	SDL_UnlockAudio();
}
// -- * -- * --
bool CSkWinSDL::ML() {
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
			paint(pScreen);
			SDL_Flip(pScreen);
			this->Sleep(1);
			break;
		}
	}
	return true;
}
// -- * -- * --
U32 CSkWinSDL::GetTickCount() {
	return SDL_GetTicks();
}
// -- * -- * --
void CSkWinSDL::Sleep(U32 millisecs) {
	SDL_Delay(millisecs);
}
// -- * -- * --
bool CSkWinSDL::IsAvail() {
	return (pScreen != NULL);
}
// -- * -- * --
void CSkWinSDL::processMinput(U8 button, bool pressed, int x, int y)
{
	x /= sxfact;
	y /= sxfact;

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
}
// -- * -- * --
void CSkWinSDL::processKinput(SDLKey nChar, bool press)
{
	if (press) {
		if (nChar == SDLK_LCTRL) pressCtrl |= 1;
		if (nChar == SDLK_RCTRL) pressCtrl |= 2;
	}
	else {
		if (nChar == SDLK_LCTRL) pressCtrl &= ~1;
		if (nChar == SDLK_RCTRL) pressCtrl &= ~2;
	}

	if (press && nChar == SDLK_F1) {
		switch (sxfact) {
			case 1: sxfact = 2; break;
			case 2: sxfact = 3; break;
			case 3: sxfact = 4; break;
			default: sxfact = 1; break;
		}
		CreateSurface();
		return;
	}
	else if (press && nChar == SDLK_F2) {
		// make slow
		switch (spfact) {
			case 0: case 1: case 2: case 3: case 4:
				spfact++;
				UpdateTitle();
				break;
		}
		return;
	}
	else if (press && nChar == SDLK_F3) {
		// make fast
		switch (spfact) {
			case 1: case 2: case 3: case 4: case 5:
				spfact--;
				UpdateTitle();
				break;
		}
		return;
	}

	CSkKinput *p = allocKinput();
	if (p != NULL) {
		BYTE v = 0;
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

		case SDLK_KP7: v = 71; break;
		case SDLK_KP8: v = 72; break;
		case SDLK_KP9: v = 73; break;
		case SDLK_KP4: v = 75; break;
		case SDLK_KP5: v = 76; break;
		case SDLK_KP6: v = 77; break;
		case SDLK_KP1: v = 79; break;
		case SDLK_KP2: v = 80; break;
		case SDLK_KP3: v = 81; break;

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
}
// -- * -- * --
void CSkWinSDL::ShowMessage(const char *psz) {
#ifdef WIN32
	MessageBoxA(NULL, psz, NULL, MB_ICONERROR|MB_TASKMODAL);
#else
#error Write a code to show error message to user!
#endif
}
// -- * -- * --
static SDL_Surface *OpenScreen(const int width, const int height, const int bpp, const int flags)
{
	SDL_Surface *screen = SDL_SetVideoMode(width, height, bpp, flags);
	if (!screen)
	{
		return NULL;
	}
	return screen;
}
// -- * -- * --
bool CSkWinSDL::CreateSurface() {
	int f = sxfact;

	int videoflags = 0;
	pScreen = OpenScreen(320 * f, 200 * f, 0, videoflags);
	if (!pScreen)
		return false;

	UpdateTitle();
	return true;
}
// -- * -- * --
void CSkWinSDL::UpdateTitle() {
	static const char *pszSpeeds[8] = {
		"1ULTRAFAST",
		"2FAST",
		"3NORMAL",
		"4SLOW",
		"5VERYSLOW",
		"6GLACIAL",
		"",
		"",
	};
	char ttl[100];
	_snprintf(ttl, sizeof(ttl), "skwin [F1:screen size, F2:slower, F3:faster] %s"
		, pszSpeeds[spfact & 7]
		);
	SDL_WM_SetCaption(ttl, NULL);
}
// -- * -- * --
bool CSkWinSDL::AskMe(const char *psz) {
	return true;
}
// -- * -- * --
bool CSkWinSDL::OpenAudio() {
	SDL_AudioSpec as;
	ZeroMemory(&as, sizeof(as));
	as.freq = 6000;
	as.format = AUDIO_U8;
	as.channels = 1;
	as.samples = 6000 / 10;
	as.callback = sdlAudMix;
	as.userdata = reinterpret_cast<void *>(this);
	ZeroMemory(&asavail, sizeof(asavail));
	if (SDL_OpenAudio(&as, &asavail) != 0)
		return false;
	SDL_PauseAudio(0);
	return true;
}
// -- * -- * --
void CSkWinSDL::CloseAudio() {
	SDL_CloseAudio();
}
// -- * -- * --
void CSkWinSDL::sdlAudMix(void *userdata, Uint8 *stream, int len) {
	CSkWinSDL *p = reinterpret_cast<CSkWinSDL *>(userdata);
	p->sdlAudMix(stream, len);
}
// -- * -- * --
void CSkWinSDL::sdlAudMix(Uint8 *stream, int len) {
	for (int x = 0; x < MAX_SB; x++) {
		SndBuf *p = &sbs[x];
		if (p->IsOnline()) {
			Uint32 mixlen = min((Uint32)len, (Uint32)(p->len - p->pos));
			SDL_MixAudio(stream, reinterpret_cast<Uint8 *>(p->pMem) + p->pos, mixlen, max(0, (int)(SDL_MIX_MAXVOLUME - p->dist * 30)));
			p->pos += mixlen;
		}
	}
}
// -- * -- * --
BYTE CSkWinSDL::GetLang() {
	return 0x10; // English
}
// -- * -- * --
// -- * -- * --
// -- * -- * --

#endif // _USE_SDL
