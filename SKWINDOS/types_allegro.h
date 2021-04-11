#ifndef TYPES_ALLEGRO_H
  #define TYPES_ALLEGRO_H

  // #######################
  // PORT TO ALLEGRO
  // #######################

#ifndef OLD_ALLEGRO
  #include <allegro5.h>

#if 0
  extern ALLEGRO_TIMER* al5_timer;
  extern ALLEGRO_EVENT_QUEUE* al5_equeue;
  extern ALLEGRO_DISPLAY* al5_display;
#endif
#endif

#if 0 // tracktrack left to do:
  #define ALLEGRO_SETVOLUME      al_set_volume
  #define ALLEGRO_VSYNC          al_vsync
  #define ALLEGRO_PAUSE          al_rest
  #define ALLEGRO_GETCONFIGINT   al_get_config_int
#endif

  #define SAMPLE_REC_FREQ        (12000)  // guessed original frequency of sfx

#endif