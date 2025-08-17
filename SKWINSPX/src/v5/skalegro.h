#ifndef OLD_ALLEGRO
#ifndef C_ALLEGRO_H
  #define C_ALLEGRO_H

// SPX note : this "non" allegro step allows to check compilation of overall code without allegro dependence.
#ifndef __SK_NO_ALLEGRO__
  #include <allegro5/allegro5.h>
  #include <allegro5/allegro_audio.h>
  #include <allegro5/allegro_acodec.h>
  #include <allegro5/allegro_direct3d.h>
#else
#include <stdlib.h>
	#define ALLEGRO_MUTEX unsigned int
	#define ALLEGRO_COND unsigned int
	typedef struct ALLEGRO_DISPLAY ALLEGRO_DISPLAY;
	#define ALLEGRO_TIMER unsigned int
	#define ALLEGRO_EVENT_QUEUE unsigned int
	#define ALLEGRO_THREAD unsigned int
	//#define ALLEGRO_EVENT unsigned int

	typedef unsigned int ALLEGRO_EVENT_TYPE;
	typedef unsigned int ALLEGRO_ANY_EVENT;
	typedef unsigned int ALLEGRO_DISPLAY_EVENT;
	typedef unsigned int ALLEGRO_JOYSTICK_EVENT;
	typedef unsigned int ALLEGRO_MOUSE_EVENT;
	typedef unsigned int ALLEGRO_TIMER_EVENT;
	typedef unsigned int ALLEGRO_USER_EVENT;

	typedef struct ALLEGRO_KEYBOARD_EVENT
	{
	   struct ALLEGRO_DISPLAY *display; /* the window the key was pressed in */
	   int keycode;                 /* the physical key pressed */
	   int unichar;                 /* unicode character or negative */
	   unsigned int modifiers;      /* bitfield */
	   bool repeat;                 /* auto-repeated or not */
	} ALLEGRO_KEYBOARD_EVENT;


	union ALLEGRO_EVENT
	{
	   /* This must be the same as the first field of _AL_EVENT_HEADER.  */
	   ALLEGRO_EVENT_TYPE type;
	   /* `any' is to allow the user to access the other fields which are
		* common to all event types, without using some specific type
		* structure.
		*/
	   ALLEGRO_ANY_EVENT      any;
	   ALLEGRO_DISPLAY_EVENT  display;
	   ALLEGRO_JOYSTICK_EVENT joystick;
	   ALLEGRO_KEYBOARD_EVENT keyboard;
	   ALLEGRO_MOUSE_EVENT    mouse;
	   ALLEGRO_TIMER_EVENT    timer;
	   ALLEGRO_USER_EVENT     user;
	};

	typedef struct // check allegro5/color.h
	{
	   float r, g, b, a;
	} ALLEGRO_COLOR;
	#define ALLEGRO_EVENT_DISPLAY_CLOSE	42
	#define ALLEGRO_EVENT_TIMER			30
	#define ALLEGRO_EVENT_KEY_CHAR		11
	#define ALLEGRO_MOUSE_MAX_EXTRA_AXES 4
	typedef struct // check allegro5/mouse.h
	{
	   int x;
	   int y;
	   int z;
	   int w;
	   int more_axes[ALLEGRO_MOUSE_MAX_EXTRA_AXES];
	   int buttons;
	   float pressure;
	   struct ALLEGRO_DISPLAY *display;
	} ALLEGRO_MOUSE_STATE;

	#define ALLEGRO_SAMPLE char
	#define ALLEGRO_AUDIO_DEPTH_UINT8		0x08
	#define ALLEGRO_CHANNEL_CONF_1			0x10		
	#define ALLEGRO_PLAYMODE_ONCE			0x100
	#define ALLEGRO_PLAYMODE_LOOP			0x101

	int al_init_acodec_addon();
	int al_install_audio();
	int al_reserve_samples(unsigned int nsamples);
	int al_destroy_sample(ALLEGRO_SAMPLE* s);
	ALLEGRO_SAMPLE* al_create_sample(char* bdest, unsigned int length, int recfreq, int adepth, int chanconf, bool b);
	int al_play_sample(ALLEGRO_SAMPLE* s, float vol, float pan, float freq, int playmode, int* x);
	ALLEGRO_SAMPLE* al_load_sample(const char* pathname);
	int al_stop_samples();
	int al_uninstall_audio();
	int al_rest(float sec);
	int al_get_next_event(ALLEGRO_EVENT_QUEUE* equeue, ALLEGRO_EVENT* event);
	int al_get_mouse_state(ALLEGRO_MOUSE_STATE* ret_state);
   

#endif // __SK_NO_ALLEGRO__

  #ifndef TYPES_H
    #include <types.h>
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
#ifndef __SK_NO_ALLEGRO__
      c_allegro(void) : al5_display(NULL),
    al5_timer(NULL),
    al5_equeue(NULL),
    al5_thread(NULL)
    {}
#else
	  c_allegro(void);
#endif // __SK_NO_ALLEGRO__

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
