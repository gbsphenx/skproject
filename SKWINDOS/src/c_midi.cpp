#pragma warning (disable:4312)

#include "allegro/include/allegro.h"
#include <stdio.h>
#define OLD_ALLEGRO
#include "../types.h"
#include "startend.h" // THROW...
#include "c_midi.h"

#define MIDIPATHNAME "./DATA/%02x.hmp.mid"

// SPX: added to handle music as wav
#include "c_music_wav.h"

void c_midi::do_music(i16 nr)
{
  bool bUseMIDI = false;
  char songname[30];
// MIDI music
  if (bUseMIDI)
  {
	sprintf(songname, MIDIPATHNAME, nr);
	thesong = load_midi(songname);
	if (thesong)
	    play_midi(thesong, true); // true means: looped
  }
  else
  {
	do_music_wav(nr);
  }

}

void c_midi::stop_music(void)
{
  if (thesong)
  {
    destroy_midi(thesong);
    thesong = NULL;
  }
}

void c_midi::set_midi_volume(i16 v)
{
  set_volume(-1, v);
}

void c_midi::init_midi(void)
{
  allegro_init();
  if (install_sound(-1, MIDI_AUTODETECT, NULL) != 0)
    throw(THROW_DMABORT);
  set_midi_volume(128);
}
