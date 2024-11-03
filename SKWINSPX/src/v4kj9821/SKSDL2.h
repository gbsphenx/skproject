// CSkWinSDL2, skwin class to render GFX on Linux

#pragma once

#include <SkWin.h>

class SkWinCore;


#if defined(__LINUX__) && defined(__SDL__)
#include <SDL2/SDL.h>
#endif // __LINUX__

class CSkWinSDL2 : public CSkWin
{
public:
	SkWinCore* skw;
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

private:
#if defined(__LINUX__) && defined(__SDL__)
	SDL_Renderer* renderer;
	SDL_Window* window;
#endif // __LINUX__

public:
	CSkWinSDL2();
	virtual ~CSkWinSDL2();

	void VersionMessage();
	void StartMessage();
	void ExitMessage();

	void InitVideo();
	void ResetVideo();
	void OnPaint();
	bool ML();
	void ProcessArgs(int argc, char** argv);
	void SndPlayLo(const U8 *buff, U32 buffSize, i8 dX, i8 dY);
	void SndPlayHi(const U8 *buff, U32 buffSize, i8 vol);
	void SndPlayFile(const char* sFilename, i8 vol);
	void SndSetFrequency(U16 iSoundPlaybackFrequency);
	void ShowMessage(const char *psz);
	unsigned char GetLang();
	void UpdateRect(i16 x, i16 y, i16 cx, i16 cy);
	bool IsAvail();

	void GetMousePosButtons(U16 *x, U16 *y, U16 *buttons);

	U32 GetTickCount();
	void Sleep(U32 millisecs);
};


