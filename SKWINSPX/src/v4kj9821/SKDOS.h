// CSkWinDOS, an attempt to get an empty skwin class to allow compilation in non MFCWin mode

#pragma once

#include <SkWin.h>

class SkWinCore;

// CSkWinMFC

class CSkWinDOS : public CSkWin
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

public:
	CSkWinDOS();
	virtual ~CSkWinDOS();

	void VersionMessage();
	void StartMessage();
	void ExitMessage();

	void PollKeyboard();
	void processKinput(unsigned char nChar, bool press);

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


#ifndef DOS_KEYS_H
#define DOS_KEYS_H

// Numpad keys
#define DK_NUMPAD_0			0x52
#define DK_NUMPAD_1			0x4F
#define DK_NUMPAD_2			0x50
#define DK_NUMPAD_3			0x51
#define DK_NUMPAD_4			0x4B
#define DK_NUMPAD_5			0x4C
#define DK_NUMPAD_6			0x4D
#define DK_NUMPAD_7			0x47
#define DK_NUMPAD_8			0x48
#define DK_NUMPAD_9			0x49
#define DK_NUMPAD_DOT		0x53

// Other keys
#define DK_ESCAPE			0x01
#define DK_TAB				0x0F
#define DK_SPACE			0x39
#define DK_ENTER			0x1C
#define DK_BACKSPACE		0x0E
#define DK_LEFT_ARROW		0x4B
#define DK_RIGHT_ARROW		0x4D
#define DK_UP_ARROW			0x48
#define DK_DOWN_ARROW		0x50

// Alphanumeric Keys (A-Z)
#define DK_A      0x1E  // A
#define DK_B      0x30  // B
#define DK_C      0x2E  // C
#define DK_D      0x20  // D
#define DK_E      0x12  // E
#define DK_F      0x21  // F
#define DK_G      0x22  // G
#define DK_H      0x23  // H
#define DK_I      0x17  // I
#define DK_J      0x24  // J
#define DK_K      0x25  // K
#define DK_L      0x26  // L
#define DK_M      0x32  // M
#define DK_N      0x31  // N
#define DK_O      0x18  // O
#define DK_P      0x19  // P
#define DK_Q      0x10  // Q
#define DK_R      0x13  // R
#define DK_S      0x1F  // S
#define DK_T      0x14  // T
#define DK_U      0x16  // U
#define DK_V      0x2F  // V
#define DK_W      0x11  // W
#define DK_X      0x2D  // X
#define DK_Y      0x15  // Y
#define DK_Z      0x2C  // Z

// Numeric Keys (0-9)

#define DK_0      0x0B
#define DK_1      0x02
#define DK_2      0x03
#define DK_3      0x04
#define DK_4      0x05
#define DK_5      0x06
#define DK_6      0x07
#define DK_7      0x08
#define DK_8      0x09
#define DK_9      0x0A

#endif // DOS_KEYS_H

