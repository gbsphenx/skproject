// FILE c_allegro.cpp

#include <dm2globl.h> // ORIG_SHEIGHT, ORIG_SWIDTH
#include <startend.h> // THROW...
#include <sfxsnd.h>
#include <skalegro.h>
#include <dosvideo.h>

//#include <sktypes5.h>

c_allegro alg;

//ALLEGRO_COLOR mypal[256];

void* thread_func(ALLEGRO_THREAD* thr, void* vparg);
#ifdef THIRDTHREAD
void* thread_func2(ALLEGRO_THREAD* thr, void* vparg);
#endif

//------------------------------------------------------------------------------

#ifdef __SK_NO_ALLEGRO__

int al_init_acodec_addon() { return 0;}
int al_install_audio() { return 0;}
int al_reserve_samples(unsigned int nsamples) { return 0;}
int al_destroy_sample(ALLEGRO_SAMPLE* s) { return 0;}
ALLEGRO_SAMPLE* al_create_sample(char* bdest, unsigned int length, int recfreq, int adepth, int chanconf, bool b) { return NULL;}
int al_play_sample(ALLEGRO_SAMPLE* s, float vol, float pan, float freq, int playmode, int* x) { return 0;}
ALLEGRO_SAMPLE* al_load_sample(const char* pathname) { return NULL;}
int al_stop_samples() { return 0;}
int al_uninstall_audio() { return 0;}
int al_rest(float sec) { return 0;}
int al_get_next_event(ALLEGRO_EVENT_QUEUE* equeue, ALLEGRO_EVENT* event) { return 0;}
int al_get_mouse_state(ALLEGRO_MOUSE_STATE* ret_state) { return 0;}

c_allegro::c_allegro(void)
{
	;
}

#endif // //




//------------------------------------------------------------------------------

void c_allegro::restart(void)
{
#ifndef __SK_NO_ALLEGRO__
  dm2sound.stop_all_sound();
  al_stop_timer(al5_timer);
  al_flush_event_queue(al5_equeue);
//  al_clear_to_color(al_map_rgba(0, 0, 0, 255));
#endif // __SK_NO_ALLEGRO__
}

void c_allegro::start(void)
{
#ifndef __SK_NO_ALLEGRO__
  if (!al_init()) exit(5);
  al_install_keyboard();
  al_install_mouse();

  al5_timer = al_create_timer(0.01); // 100 hz timer

  al5_equeue = al_create_event_queue();

  bool windowed = true; // HOT ALLEGRO_GETCONFIGINT("game","windowed",1) ? true : false;
  new_swidth = 640;
  new_sheight = 400;
  new_colordepth = 32;

  int gfxflags = 0;
  if (windowed)
  {
    gfxflags |= ALLEGRO_WINDOWED;
    gfxflags |= ALLEGRO_RESIZABLE;
  }
  else
    gfxflags |= ALLEGRO_FULLSCREEN; // _WINDOW;

  al_set_new_display_option(ALLEGRO_COLOR_SIZE, 32, ALLEGRO_REQUIRE);
  al_set_new_display_flags(gfxflags);
  al_set_new_display_refresh_rate(60);

  al5_display = al_create_display(new_swidth, new_sheight);
  if (al5_display == NULL)
    throw(THROW_DMABORT);

  if (!dm2sound.init()) throw(THROW_DMABORT);

  al_register_event_source(al5_equeue, al_get_keyboard_event_source());
#if 0 // see c_addon.cpp
  al_register_event_source(al5_equeue, al_get_mouse_event_source());
#endif
  al_register_event_source(al5_equeue, al_get_display_event_source(al5_display));
  al_register_event_source(al5_equeue, al_get_timer_event_source(al5_timer));

  al5_thread = al_create_thread(thread_func, NULL);
  if (al5_thread == NULL) exit(1);
  al_start_thread(al5_thread);

#ifdef THIRDTHREAD
  al5_thread2 = al_create_thread(thread_func2, NULL);
  if (al5_thread2 == NULL) exit(1);
  al_start_thread(al5_thread2);
#endif
#elif __DJGPP__ // DOS MODE
  DOS_VIDEO_INIT_VGA_MODE();
  thread_func(NULL, NULL);
#endif // __SK_NO_ALLEGRO__
}

void c_allegro::dtor(void)
{
#ifndef __SK_NO_ALLEGRO__
  al_join_thread(al5_thread, NULL);
  al_destroy_thread(al5_thread);

  if (al5_timer)
  {
    al_destroy_timer(al5_timer);
    al5_timer = NULL;
  }
  if (al5_equeue)
  {
    al_destroy_event_queue(al5_equeue);
    al5_equeue = NULL;
  }
  if (al5_display)
  {
    al_destroy_display(al5_display);
    al5_display = NULL;
  }
#endif // __SK_NO_ALLEGRO__
}

void c_allegro::set_mouse(i16 x, i16 y)
{
#ifndef __SK_NO_ALLEGRO__
  al_set_mouse_xy(al5_display, x << 1, y << 1);
#endif // __SK_NO_ALLEGRO__
}

void c_allegro::vsync(void)
{
//  al_wait_for_vsync();
}

void c_allegro::stretchblit(i8* bptr)
{
#ifndef __SK_NO_ALLEGRO__
  while (al_is_d3d_device_lost(al5_display));

  ALLEGRO_LOCKED_REGION* ar = al_lock_bitmap(al_get_backbuffer(al5_display), ALLEGRO_PIXEL_FORMAT_ANY, 0);
  if (ar == NULL)
    throw(THROW_DMABORT);

  for (i16 y = 0; y < wcon(ORIG_SHEIGHT); y++)
  {
    i8* d1 = DOWNCAST(void, ar->data) + 2 * y * ar->pitch;
    i8* d2 = DOWNCAST(void, ar->data) + (2 * y + 1) * ar->pitch;
    for (i16 x = 0; x < ORIG_SWIDTH; x++)
    {
      ui8 ic = *bptr++;
      *d1++ = mypal[ic].b;
      *d1++ = mypal[ic].g;
      *d1++ = mypal[ic].r;
      *d1++ = 255;
      *d1++ = mypal[ic].b;
      *d1++ = mypal[ic].g;
      *d1++ = mypal[ic].r;
      *d1++ = 255;
      *d2++ = mypal[ic].b;
      *d2++ = mypal[ic].g;
      *d2++ = mypal[ic].r;
      *d2++ = 255;
      *d2++ = mypal[ic].b;
      *d2++ = mypal[ic].g;
      *d2++ = mypal[ic].r;
      *d2++ = 255;
    }
  }

  al_unlock_bitmap(al_get_backbuffer(al5_display));
  al_flip_display();
#elif __DJGPP__
  DOS_VIDEO_UPDATE_SCREEN((unsigned char*) bptr);
#endif // __SK_NO_ALLEGRO__
}

void c_allegro::start_timer(void)
{
#ifndef __SK_NO_ALLEGRO__
  al_start_timer(al5_timer);
#endif // __SK_NO_ALLEGRO__
}

void c_allegro::stop_timer(void)
{
#ifndef __SK_NO_ALLEGRO__
  al_stop_timer(al5_timer);
#endif // __SK_NO_ALLEGRO__
}

void c_allegro::hide_mouse(void)
{
#ifndef __SK_NO_ALLEGRO__
  al_hide_mouse_cursor(al5_display);
#endif // __SK_NO_ALLEGRO__
}

//------------------------------------------------------------------------------
