
#ifndef _SKWINSDL_H_
#define _SKWINSDL_H_

#include <SDL.h>
#include "SkWin.h"

class CSkWinSDL : public CSkWin {
public:
	//
	SDL_Surface *pScreen;
	U8 curMiceState;

	// 
	void UpdateRect(i16 x, i16 y, i16 cx, i16 cy);
	void SndPlayHi(const U8 *buff, U32 buffSize, i8 vol);
	void SndPlayLo(const U8 *buff, U32 buffSize, i8 dX, i8 dY);
	bool ML();
	U32 GetTickCount();
	void Sleep(U32 millisecs);
	bool IsAvail();
	void ShowMessage(const char *psz);
	bool AskMe(const char *psz);
	bool OpenAudio();
	void CloseAudio();
	U8 GetLang();

	// 
	CSkWinSDL();
	bool CreateSurface();
	void UpdateTitle();

public:
	//
	int spfact;

protected:
	// 
	int sxfact;
	U8 pressCtrl;

	// 
	class SndBuf {
	public:
		void *pMem;
		int pos, len, dist;

		SndBuf(): pMem(NULL), pos(0), len(0) { }
		~SndBuf() {
			if (pMem != NULL) free(pMem), pMem = NULL, pos = len = 0;
		}
		void Alloc(const void *pSrc, int cchSrc, int dist) {
			void *p = realloc(pMem, cchSrc);
			if (p != NULL) {
				pMem = p;
				this->dist = dist;
				for (int x=0; x<cchSrc; x++)
					((U8 *)pMem)[x] = 0x80 +((const U8 *)pSrc)[x];
				pos = 0; len = cchSrc;
			}
		}
		bool IsFree() const { return pos == len; }
		bool IsOnline() const { return pos != len; }
	};

	// 
	SDL_AudioSpec asavail;
#define MAX_SB 16U
	SndBuf sbs[MAX_SB];

	// 
	void paint(SDL_Surface *surface);
	void processMinput(U8 button, bool pressed, int x, int y);
	void processKinput(SDLKey nChar, bool press);
	static void sdlAudMix(void *userdata, Uint8 *stream, int len);
	void sdlAudMix(Uint8 *stream, int len);
};

#endif // _SKWINSDL_H_
