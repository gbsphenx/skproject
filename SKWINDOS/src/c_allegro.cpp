#include "../dm2global.h" // ORIG_SHEIGHT, ORIG_SWIDTH
#include "startend.h" // THROW...
#include "c_sound.h"
#include "c_allegro.h"

c_allegro alg;

struct ALLEGRO_COLOR mypal[256];

ThreadInfo thread_info;

void* thread_func(ALLEGRO_THREAD* thr, void* arg);

void c_allegro::restart(void)
{
  dm2sound.stop_all_sound();
  al_stop_timer(al5_timer);
  al_flush_event_queue(al5_equeue);
//  al_clear_to_color(al_map_rgba(0, 0, 0, 255));
}

void c_allegro::start(void)
{
  if (!al_init()) exit(5);
  al_install_keyboard();
  al_install_mouse();

  al5_timer = al_create_timer(0.01); // 100 hz timer

  al5_equeue = al_create_event_queue();

  bool windowed = 1; // HOT ALLEGRO_GETCONFIGINT("game","windowed",1) ? true : false;
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
//  al_register_event_source(al5_equeue, al_get_mouse_event_source());
  al_register_event_source(al5_equeue, al_get_display_event_source(al5_display));
  al_register_event_source(al5_equeue, al_get_timer_event_source(al5_timer));

//  thread_info.mutex = al_create_mutex();
//  if (!thread_info.mutex) exit(1);
//  thread_info.cond = al_create_cond();
//  if (!thread_info.cond) exit(1);
  al5_thread = al_create_thread(thread_func, NULL/*&thread_info*/);
  if (!al5_thread) exit(1);
  al_start_thread(al5_thread);
}

void c_allegro::dtor(void)
{
//  al_set_thread_should_stop(al5_thread);
//  al_lock_mutex(thread_info.mutex);
//  al_broadcast_cond(thread_info.cond);
//  al_unlock_mutex(thread_info.mutex);
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
}

void c_allegro::set_mouse(i16 x, i16 y)
{
  al_set_mouse_xy(al5_display, x << 1, y << 1);
}

void c_allegro::vsync(void)
{
//  al_wait_for_vsync();
}

void c_allegro::stretchblit(const i8* s)
{
  while (al_is_d3d_device_lost(al5_display));

  ALLEGRO_LOCKED_REGION* ar =
   al_lock_bitmap(al_get_backbuffer(al5_display), ALLEGRO_PIXEL_FORMAT_ANY, 0);
  if (ar==NULL)
    throw(THROW_DMABORT);

  for (int y=0; y<ORIG_SHEIGHT; y++)
  {
    ui8* d1 = UP8CAST(ar->data) + 2 * y * ar->pitch;
    ui8* d2 = UP8CAST(ar->data) + (2 * y + 1) * ar->pitch;
    for (int x=0; x<ORIG_SWIDTH; x++)
    {
      ui8 c = *s++;
      *d1++ = mypal[c].b;
      *d1++ = mypal[c].g;
      *d1++ = mypal[c].r;
      *d1++ = 0xff;
      *d1++ = mypal[c].b;
      *d1++ = mypal[c].g;
      *d1++ = mypal[c].r;
      *d1++ = 0xff;
      *d2++ = mypal[c].b;
      *d2++ = mypal[c].g;
      *d2++ = mypal[c].r;
      *d2++ = 0xff;
      *d2++ = mypal[c].b;
      *d2++ = mypal[c].g;
      *d2++ = mypal[c].r;
      *d2++ = 0xff;
    }
  }

  al_unlock_bitmap(al_get_backbuffer(al5_display));
  al_flip_display();
}

void c_allegro::start_timer(void)
{
  al_start_timer(al5_timer);
}

void c_allegro::stop_timer(void)
{
  al_stop_timer(al5_timer);
}

void c_allegro::hide_mouse(void)
{
  al_hide_mouse_cursor(al5_display);
}
