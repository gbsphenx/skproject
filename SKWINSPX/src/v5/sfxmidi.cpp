#pragma warning (disable:4312)



#ifndef __SK_NO_ALLEGRO__
#include <allegro/include/allegro.h>
#else
	#include <stdlib.h>
	typedef struct MIDI                    /* a midi file */
	{
	   int divisions;                      /* number of ticks per quarter note */
	   struct {
		  unsigned char *data;             /* MIDI message stream */
		  int len;                         /* length of the track data */
	   } track[32];
	} MIDI;
	MIDI* load_midi(const char *filename) { return NULL; } 
	int play_midi(MIDI *midi, int loop) { return 0; } 
	void destroy_midi(MIDI *midi) { return; } 
	void set_volume(int digi_volume, int midi_volume) { return; } 
	int allegro_init() { return 0; } 
	int install_sound(int digi, int midi, const char *cfg_path) { return 0; } 
#define MIDI_AUTODETECT		-1
#endif

#include <stdio.h>
#define OLD_ALLEGRO
#include <types.h>
#include <startend.h> // ADDED NEW
#include <dm2globl.h>
#include <sfxmidi.h>

#define MIDIPATHNAME "./DATA/%02x.hmp.mid" // SPX TODO




// SPX: added to handle music as wav
#include <sfxmuwav.h>
bool bUseMIDI = true;

extern int __GlobalOption_DM2Skull_MusicMode;	// from main.cpp

void c_midi::do_music(i32 songnr)
{
  char songname[30];

  if (__GlobalOption_DM2Skull_MusicMode == 0)	// HMP
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
  if (__GlobalOption_DM2Skull_MusicMode == 0)	// HMP
  {
	if (thesong)
	{
	  destroy_midi(thesong);
	  thesong = NULL;
	}
  }
  else if (__GlobalOption_DM2Skull_MusicMode == 1)	// MOD(ogg)
  {
	  do_music_wav_stop();
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
