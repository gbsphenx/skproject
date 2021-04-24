#pragma warning (disable:4312)

#include <stdio.h>
#include "../types.h"
#include "startend.h" // THROW...

// SPX: added to handle music as wav
#include <allegro5.h>
#include <allegro_audio.h>

void do_music_wav(i16 nr)
{
  char songname[30];

  {
	ALLEGRO_SAMPLE* thesongwav = NULL;
    sprintf(songname, "./DATA/sk%02d.wav", nr);
	thesongwav = al_load_sample(songname);
	if (thesongwav)
		al_play_sample(thesongwav, 1.0, 0, 1, ALLEGRO_PLAYMODE_LOOP,NULL);
  }

}
