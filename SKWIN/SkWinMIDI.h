#ifndef _SKMIDI_H_
#define _SKMIDI_H_

//------------------------------------------------------------------------------

#include <SkVersionControl.h>

//------------------------------------------------------------------------------

class SkWinMIDI 
{
public:
	int		iCurrentRequestedMusic;
	int		iNextRequestedMusic;
	bool	bCurrentlyPlaying;
	char	sCustomFolder[256];

public:
	SkWinMIDI(int dung, const char* folder);

	void REQUEST_PLAY_MUSIC(int iNumber);

private:
	//unsigned long MIDI_LOOP();
};


//------------------------------------------------------------------------------

#endif _SKMIDI_H_
