#ifndef C_ALLEGRO_H
  #define C_ALLEGRO_H

  #include <allegro5.h>
  #include <allegro_audio.h>
  #include <allegro_acodec.h>
  #include <allegro_direct3d.h>

  #ifndef TYPES_H
    #include "../types.h"
  #endif

  typedef struct
  {
    ALLEGRO_MUTEX* mutex;
    ALLEGRO_COND* cond;
  } ThreadInfo;

  class c_allegro
  {
    protected:
      ALLEGRO_DISPLAY* al5_display;
      ALLEGRO_TIMER* al5_timer;
      ALLEGRO_EVENT_QUEUE* al5_equeue;
      ALLEGRO_THREAD* al5_thread;

      int new_swidth;
      int new_sheight;
      int new_colordepth;

    public:
      c_allegro(void) : al5_display(NULL),
                        al5_timer(NULL),
                        al5_equeue(NULL),
                        al5_thread(NULL)
                        {}

      void start(void);
      void restart(void);
      void dtor(void);

      void set_mouse(i16 x, i16 y);
      void hide_mouse(void);

      void start_timer(void);
      void stop_timer(void);

      void vsync(void);
      void stretchblit(const i8* s);

      friend void event_loop_T1(void);
  };

  extern c_allegro alg;

#endif
