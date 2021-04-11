#ifndef C_MIDI_H
 #define C_MIDI_H

 #ifndef TYPES_H
   #include "../types.h"
 #endif

 struct MIDI; // allegro4

 class c_midi
 {
   protected:
     MIDI* thesong;

   public:
     c_midi(void) : thesong(NULL) {}
     void init_midi(void);
     void set_midi_volume(i16 v);
     void stop_music(void);
     void do_music(i16 nr);
 };

#endif
