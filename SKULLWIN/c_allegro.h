#ifndef OLD_ALLEGRO
#ifndef C_ALLEGRO_H
  #define C_ALLEGRO_H

  #include "allegro5/include/allegro5/allegro5.h"
  #include "allegro5/include/allegro5/allegro_audio.h"
  #include "allegro5/include/allegro5/allegro_acodec.h"
  #include "allegro5/include/allegro5/allegro_direct3d.h"

  #ifndef TYPES_H
    #include "types.h"
  #endif

  struct ThreadInfo
  {
    ALLEGRO_MUTEX* mutex;
    ALLEGRO_COND* cond;
  };

  class c_allegro
  {
    protected:
      ALLEGRO_DISPLAY* al5_display;
      ALLEGRO_TIMER* al5_timer;
      ALLEGRO_EVENT_QUEUE* al5_equeue;
      ALLEGRO_THREAD* al5_thread;
#ifdef THIRDTHREAD
      ALLEGRO_THREAD* al5_thread2;
#endif

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
      void stretchblit(i8* bptr);

      friend void event_loop_T1(void);
  };

  extern c_allegro alg;

#endif
#endif
