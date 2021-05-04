#pragma warning (disable:4312)

#include "allegro/include/allegro.h"
#include <stdio.h>
#define OLD_ALLEGRO
#include "types.h"
#include "startend.h" // ADDED NEW
#include "dm2global.h"
#include "c_midi.h"

#define MIDIPATHNAME "./DATA/%02x.hmp.mid"

// SPX: added to handle music as wav
#include "c_music_wav.h"
bool bUseMIDI = true;

extern int __dm2skull_music;	// from main.cpp

void c_midi::do_music(i32 songnr)
{
  char songname[30];

  if (__dm2skull_music == 0)
	bUseMIDI = true;
  else
	bUseMIDI = false;


  if (bUseMIDI == true)
  {
  sprintf(songname, MIDIPATHNAME, songnr);
  thesong = load_midi(songname);
  if (thesong)
    play_midi(thesong, true); // true means: looped
  }
  else
	do_music_wav(songnr);	// call c_music_wav.cpp
}

void c_midi::stop_music(void)
{
  if (thesong)
  {
    destroy_midi(thesong);
    thesong = NULL;
  }
}

void c_midi::set_midi_volume(i16 wv)
{
  set_volume(-1, wv);
}

void c_midi::init_midi(void)
{
  allegro_init();
  if (install_sound(-1, MIDI_AUTODETECT, NULL) != 0)
    throw(THROW_DMABORT);
  set_midi_volume(128);
}
