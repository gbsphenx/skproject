#pragma warning (disable:4312)

#include "dm2data.h"
#include "c_allegro.h"
#include "c_midi.h"
#include "dm2global.h"
#include "c_keybd.h"
#include "c_gfx_main.h"
#include "ints.h"
#include "c_tmouse.h"
#include "c_addon.h"

void wait_for_vsync(void); // forward

static i16 vsynccounter = 0;
static i16 tick = 0;

// ################
// Timing emulation
// ################

// The message - loop gets called now from almost each return - command of a pmode - routine.
// Also from certain places. The tick decides which 'interrupts' are served.
// Can't do real interrupts, because I'd have to lock almost all then.
// Timerspeed should be 100 Hz, blit happens with 25 Hz then.

static ALLEGRO_EVENT* evp = NULL;
static ALLEGRO_EVENT ev;

void event_loop_T1(void)
{
  static int t25hz = 0;
  static bool b25hz = false;
  static i16 oldmb = -1, oldmx = -1, oldmy = -1;

//  i64 count = al_get_timer_count(alg.al5_timer);

  while (al_get_next_event(alg.al5_equeue, &ev) && !main_exit)
  {
    if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
      close_win = true;
    }
    if (ev.type == ALLEGRO_EVENT_TIMER)
    {
//      if (loops<5) // limit timerevents!
      {
//        loops++;
        t25hz++;
        if (t25hz == 4)
        {
          t25hz = 0;
          b25hz = true;
        }
        driver_timerint_T1();
        tick++;
      }
    }
    else if (ev.type == ALLEGRO_EVENT_KEY_CHAR)
    {
      dm2keybd.putkey(ev.keyboard.keycode);
    }
#if 0 // DANGER! ALLEGRO-BUG produces ghost clicks!
    else if (   ev.type == ALLEGRO_EVENT_MOUSE_AXES
   || ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN
   || ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP
  )
    {
      c_evententry me;
      me.x = ev.mouse.x >> 1;
      me.y = ev.mouse.y >> 1;
      me.b = ev.mouse.button & 0x3;

      if (me.x != oldmx || me.y != oldmy || me.b != oldmb)
      {
        while (!Tmouse.T1_driver_mouseint(me))
          Tmouse.command_interpreter(); // HOTTAG
      }
      oldmb = me.b; oldmx = me.x; oldmy = me.y;
    }

#else

    ALLEGRO_MOUSE_STATE ret_state;

    al_get_mouse_state(&ret_state);

    c_evententry me;
    me.x = ret_state.x >> 1;
    me.y = ret_state.y >> 1;
    me.b = ret_state.buttons & 0x3;

    if (me.x != oldmx || me.y != oldmy || me.b != oldmb)
    {
      while (!Tmouse.T1_driver_mouseint(me))
        Tmouse.command_interpreter(); // HOTTAG
    }
    oldmb = me.b; oldmx = me.x; oldmy = me.y;

#endif

  }

  Tmouse.command_interpreter(); // HOTTAG

  if (b25hz)
  {
    if (vsynccounter)
    {
      alg.vsync(); // TODO: the disable_video-flag should do that better
      vsynccounter = 0;
    }
    if (!disable_video)
    {
      Tmouse.T1_drawmouse(); //HOTTAG
      alg.stretchblit(DOWNCAST(t_pixel256, gfxsys.dm2mscreen));
    }
    b25hz = false;
  }
}

void wait_for_vsync(void)
{
  vsynccounter++;
}

void wft(void)
{
  while (tick == 0);
  tick = 0;
}
