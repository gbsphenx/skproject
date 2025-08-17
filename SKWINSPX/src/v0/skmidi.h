#ifndef _SKMIDI_H_
#define _SKMIDI_H_

//------------------------------------------------------------------------------

#include <skver.h>

//------------------------------------------------------------------------------

class SkWinMIDI 
{
public:
	int		iCurrentRequestedMusic;
	int		iNextRequestedMusic;
	bool	bCurrentlyPlaying;
	char	sRootFolder[256];

public:
	SkWinMIDI(int dung, const char* folder, const char* defaultfolder);

	void REQUEST_PLAY_MUSIC_FROM_MAP(int iNumber);

private:
	//unsigned long MIDI_LOOP();
};


//------------------------------------------------------------------------------

#endif // _SKMIDI_H_
