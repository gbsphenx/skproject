// SPX: Code added mainly for using Amiga music as wav/ogg file
#pragma warning (disable:4312)

#include <stdio.h>
#include <types.h>
#include <startend.h> // THROW...

// SPX: added to handle music as wav
//#include <allegro5.h>
//#include <allegro_audio.h>
#ifndef __SK_NO_ALLEGRO__
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#else
#include <skalegro.h>
#endif // __SK_NO_ALLEGRO__

void do_music_wav(i16 nr)
{
  char songname[30];

  {
	ALLEGRO_SAMPLE* thesongwav = NULL;
    sprintf(songname, "./DATA/sk%02d.ogg", nr);
	printf("request songname : <%s>\n", songname);
	thesongwav = al_load_sample(songname);
	if (thesongwav)
		al_play_sample(thesongwav, 1.0, 0, 1, ALLEGRO_PLAYMODE_LOOP,NULL);
  }

}

void do_music_wav_stop()
{
	al_stop_samples();
}