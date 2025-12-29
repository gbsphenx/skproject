

#include <StdAfx.h>
#include <skmidi.h>

#if !defined(__DJGPP__) && !defined(__LINUX__)
#include <windows.h>
#endif

#include <string.h>
#include <stdio.h>
#include <skparam.h>
#include <skdefine.h>
#include <skdebug.h>

int iDungeonGame = 0;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

// static table of musics mapped to maps -- that should be read from SONGLIST.DAT
int tMusicMaps[64] =
{
	0x02, 0x11, 0x0e, 0x1b,		0x04, 0x0c, 0x0c, 0x12,
	0x0f, 0x0d, 0x0c, 0x0c,		0x10, 0x06, 0x15, 0x0e,
	
	0x11, 0x11, 0x11, 0x11,		0x03, 0x08, 0x11, 0x0e,
	0x02, 0x17, 0x16, 0x14,		0x11, 0x00, 0x02, 0x02,
	
	0x02, 0x09, 0x02, 0x03,		0x0e, 0x10, 0x1c, 0x16,
	0x13, 0x09, 0x16, 0x03,		0x11, 0x02, 0xFF, 0xFF,

	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF
};



//------------------------------------------------------------------------------

SkWinMIDI::SkWinMIDI(int dung, const char* folder, const char* defaultfolder)
{
	iDungeonGame = dung;
	iCurrentRequestedMusic = -1; // no music
	iNextRequestedMusic = -1;
	bCurrentlyPlaying = false;

	memset(sRootFolder, 0, 256);
	if (folder != NULL)
		sprintf(sRootFolder, "%s", folder);
	else if (defaultfolder != NULL)
		sprintf(sRootFolder, "%s", defaultfolder);
}


void
SkWinMIDI::REQUEST_PLAY_MUSIC_FROM_MAP(int iMapNumber)
{
	if (!SkCodeParam::bMIDIMusicEnabled)
		return;
	SkD((DLV_DBG_SND, "REQUESTED MIDI MUSIC %02d\n", iMapNumber));
	
	iNextRequestedMusic = tMusicMaps[iMapNumber];

}

//------------------------------------------------------------------------------
/*
#else // __DJGPP__ , __MINGW__ , __LINUX__


SkWinMIDI::SkWinMIDI(int dung, const char* folder, const char* defaultfolder)
{
	iDungeonGame = dung;
	iCurrentRequestedMusic = -1; // no music
	iNextRequestedMusic = -1;
	bCurrentlyPlaying = false;
	memset(sRootFolder, 0, 256);
	if (folder != NULL)
		strcpy(sRootFolder, folder);
	else if (defaultfolder != NULL)
		strcpy(sRootFolder, defaultfolder);
}


void
SkWinMIDI::REQUEST_PLAY_MUSIC_FROM_MAP(int iMapNumber)
{
	;
}
*/

//#endif // __DJGPP__