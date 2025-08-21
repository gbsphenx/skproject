#include <dm2globl.h>
//#include <SKALEGRO.h>
#include <util.h>
#include <skmcursr.h>
#include <gfxmain.h>
#include <skeventq.h>
#include <dm2data.h>
#include <skxrect.h>
#include <uiclrect.h>
#include <uitmouse.h>

#include <dm2debug.h>

#ifdef NOTIFY
#include <gfxstr.h> // TODO temporary (notify)
#endif

#include <skwinapp.h>
extern SkWinApp* skWinApplication;	// in GFXMAIN.cpp

void c_xmouserect::init(void)
{
  rc.init();
  b = 0;
}

bool c_commandqueue::push(c_servercommand sc)
{
  i16 i = idx_in + 1;
  if (i == COMMAND_QUEUE_LENGTH)
    i = 0;
  if (i == idx_out)
    return false; // full
  queue[idx_in] = sc;
  idx_in = i;
  return true;
}

bool c_commandqueue::pop(OO c_servercommand& sc)
{
  if (idx_in == idx_out)
    return false; // empty
  sc = queue[idx_out];
  i16 i = idx_out + 1;
  if (i == COMMAND_QUEUE_LENGTH)
    i = 0;
  idx_out = i;
  return true;
}

c_commandqueue cqueue;

void send_command(i16 s)
{
  Tmouse.driver_blockmouseinput();
  while (!cqueue.push(c_servercommand(s)));
  Tmouse.driver_unblockmouseinput();
}

// #############################################################

c_Tmouse Tmouse;

void c_Tmouse::command_interpreter(void)
{
  c_servercommand sc;
  bool static semaphore = false;

  while (semaphore); // TODO needed at all?
  semaphore = true;

  while (cqueue.pop(sc))
  {
    switch (sc.command)
    {
      case 1:
//HOTTAG        T1_drawmouse();
		  T1_drawmouse();
        lastx = entry.x;
        lasty = entry.y;
        break;

      case 2:
        //alg.set_mouse(entry.x + 5, entry.y + 15);	// SPX: REMOVED ALLEGRO
        DRVW_mousecapturedcounter--;
        break;

      case 3:
        mouse_setrect = true;
        DRV_xmouserectptr->rc = mdummyrect; // copy (empty rect)
        DRV_xmouserectptr->b = 0x20;
        // running into command 4
      case 4:
        DRVb_use_rect2 = false;
        DRVR_rect2 = DRVR_rect1; // copy whole struct
        DRVb_use_rect2 = true;
        break;

      case 5:
        DRVW_mousecapturedcounter++;
        break;

      case 6:
        DRVW_cursoridx = CURSOR3;
        break;

      case 7:
        //DM2_DRAWINGS_COMPLETED();
        break;
    }
  }

// now old T1_execmousefifo (exploded)
// was sub_3586
// Note: final driver_unblockmouseinput() pulled out
// the only caller is now T1_command_interpreter

  static i16 DRVW_oldmb = 0;
  c_evententry me;

  while (mousequeue.pop(OO me))
  {
// me.b & 0x1 = left button
// me.b & 0x2 = right button

    // this part is exploded (was sub_34E1 driver_execmouseentry)
    entry.x = me.x;
    entry.y = me.y;
    if ((me.x != lastx || me.y != lasty) && is_visible())
    {
//HOTTAG      T1_drawmouse();
		T1_drawmouse();
      lastx = me.x;
      lasty = me.y;
    }
    entry.b = me.b; // ((me.b >> 1) & 0x1) | ((me.b << 1) & 0x2);
    i16 DRVW_mouseb = me.b ^ DRVW_oldmb;
    if (DRVW_mouseb & 0x1)
      T1_queue_event(me.x, me.y, (me.b & 0x1) ? 2 : 4);
    if (DRVW_mouseb & 0x2)
      T1_queue_event(me.x, me.y, (me.b & 0x2) ? 1 : 8);
    DRVW_oldmb = me.b;
  }

  semaphore = false;
}

void c_Tmouse::init(void)
{
SPX_DEBUG_PRINT("TMOUSE:INIT\n");

  mouse_invisible = 0;
  DRVW_blockmouseinputcounter = 0;

  // blit
  DRVR_mouserect.x = DRVR_mouserect.y = 0;
  DRVR_mouserect.w = 12;
  DRVR_mouserect.h = 16;
  DRVb_use_rect2 = false;
  entry.init();
  lastx = lasty = 0;
  // mousecursor
  DRVW_cursoridx = CURSOR0;
  // misc
  DRVW_mousecapturedcounter = 0;
  mousequeue.init();
  // clickrectlist
  mouse_setrect = false;
  mouse_rx0 = 0;
  mouse_rx1 = 0;
  mouse_ry0 = 0;
  mouse_ry1 = 0;

  rectlist1 = NULL;
  rectlist2 = NULL;
  mdummyrect.init();
  DRVR_rect1.init();
  DRVR_rect2.init();

  DRV_xmouserectptr = &DRVR_rect1;
}

void c_mousequeue::init(void)
{
  counter = 0;
  idx_in = 0;
  idx_out = 0;
  for (i16 i=0; i<MOUSE_QUEUE_LENGTH; i++)
    queue[i].init();
}

bool c_mousequeue::push(c_evententry me)
{
  static bool semaphore = false; // TODO needed at all?
  while (semaphore);
  semaphore = true;

  if (counter >= MOUSE_QUEUE_LENGTH)
  {
    semaphore = false;
    return false;
  }

  counter++;

  idx_in++;
  if (idx_in == MOUSE_QUEUE_LENGTH)
    idx_in = 0;

  queue[idx_in] = me;
  semaphore = false;
  return true;
}

bool c_mousequeue::pop(OO c_evententry& me)
{
#ifdef NOTIFY
  static i16 last_x = -1; // TODO temporary
  static i16 last_y = -1;
#endif

  if (counter == 0)
    return false;

  counter--;

  idx_out++;
  if (idx_out == MOUSE_QUEUE_LENGTH)
    idx_out = 0;

  me = queue[idx_out];

#ifdef NOTIFY
  if (me.x != last_x || me.y != last_y)
  {
    sprintf(notify, "x = %d, y = %d", me.x, me.y);
    last_x = me.x;
    last_y = me.y;
  }
#endif

  return true;
}

// -------------------

// belongs to T1_queue_event (before: queue_int2e)
// was T1_443c_00f8, TODO: tell SPX new name
e_cursoridx c_Tmouse::T1_queue_0x20(i16 x, i16 y)
{
  static i16 mouse_unk0b = 0; // TODO: really static! was global but used only here

  if (    mouse_setrect
       || x < mouse_rx0
       || x > mouse_rx1
       || y < mouse_ry0
       || y > mouse_ry1
     )
  {
    mouse_setrect = false;
    mouse_unk0b = 0;
    c_clickrectnode* node = rectlist2;
    i16 x0 = 0;
    i16 y0 = 0;
    i16 x1 = ORIG_SWIDTH;
    i16 y1 = ORIG_SHEIGHT;

    while (node != NULL)
    {
      c_rect* r = &node->node->r;
      bool xflag = false;
      if (x >= r->x)
      {
        if (x <= (r->x + r->w - 1))
        {
          xflag = true;
          x0 = DM2_MAX(x0, r->x);
          x1 = DM2_MIN(x1, r->x + r->w - 1);
        }
        else
          x0 = DM2_MAX(x0, r->x + r->w - 1);
      }
      else
        x1 = DM2_MIN(x1, r->x);

      if (y < r->y)
      {
        y1 = DM2_MIN(y1, r->y);
        node = node->node->next;
        continue;
      }

      if (y <= (r->y + r->h - 1))
      {
        y0 = DM2_MAX(y0, r->y);
        y1 = DM2_MIN(y1, r->y + r->h - 1);
        if (!xflag)
        {
          node = node->node->next;
          continue;
        }
        mouse_unk0b = unsignedword(node->b_02);
        if (node != rectlist1)
        {
          i8 bbuttons = node->buttons;
          if (bbuttons != 0)
            eventqueue.QUEUE_EVENT(x, y, unsignedword(bbuttons));
        }
        break;
      }
      else
      {
        y0 = DM2_MAX(y0, r->y + r->h - 1);
        node = node->node->next;
      }
    }

    if (rectlist1 != NULL && rectlist1 != node)
      if (rectlist1->b_05 != 0)
        eventqueue.QUEUE_EVENT(x, y, unsignedword(rectlist1->b_05));

    node->DM2_SET_CLICKRECTDATAS(x0, y0, x1, y1);
  }

  if (mouse_unk0b == 2)
    return CURSOR3;
  if (mouse_unk0b != 1 || eventqueue.event_heroidx == E_NOHERO)
    return CURSOR0;
  if (!eventqueue.event_unk0f)
    return CURSOR1;
  return CURSOR2;
}

// returns a cursorindex or -1
// FIRE_MOUSE_EVENT_RECEIVER
e_cursoridx c_Tmouse::T1_queue_event(i16 x, i16 y, i16 b)
{
  entry.x = x;
  entry.y = y;
  entry.b = b;

  e_cursoridx cidx;
	printf("MOUSE : %d %d => BUTTON : %d\n", x, y, b);
  if (entry.b >= 0x20)	// not clicked
    cidx = T1_queue_0x20(entry.x, entry.y);
  else
  {
	  //printf("CLICK!\n");
    eventqueue.QUEUE_EVENT(entry.x, entry.y, entry.b);
    cidx = NOCURSOR;
  }
  // entry.x = cidx; // TODO: what? values are 0-3, -1
	//skWinApplication->skwin_Sleep(250);
  return cidx;
}

// was sub_3313
// in the new version called by the event-thread only
void c_Tmouse::T1_drawmouse(void)
{
  c_rect rc;
  s_mcursor* cursor;

	// DEBUG
  if (DRVW_cursoridx > 4 || DRVW_cursoridx < 0)
	  DRVW_cursoridx = CURSOR0;


//HOTTAG
  DM2_COPY_MEMORY(DOWNCAST(c_pixel256, gfxsys.dm2mscreen), DOWNCAST(c_pixel256, gfxsys.dm2screen), ORIG_SWIDTH*ORIG_SHEIGHT);

  if (DRVb_use_rect2)
  {
    if (   entry.x < DRVR_rect2.rc.x
        || (DRVR_rect2.rc.x + DRVR_rect2.rc.w - 1) < entry.x
        || entry.y < DRVR_rect2.rc.y
        || (DRVR_rect2.rc.y + DRVR_rect2.rc.h - 1) < entry.y
       )
    {
      DRVb_use_rect2 = false;
      e_cursoridx cidx = T1_queue_event(entry.x, entry.y, DRVR_rect2.b); // returns 0-3, -1
      if (cidx != NOCURSOR)
        DRVW_cursoridx = cidx;
    }
  }

  cursor = &DRV_cursor[DRVW_cursoridx];

  i16 DRVW_deltax = entry.x - mkuw(cursor->hx);
  i16 DRVW_deltay = entry.y - mkuw(cursor->hy);

  i16 DRVW_cursorw = mkuw(cursor->w);
  i16 DRVW_cursorh = mkuw(cursor->h);

  i16 DRVW_ndeltax = 0;
  i16 DRVW_ndeltay = 0;

  if (DRVW_deltax < 0)
  {
    DRVW_ndeltax = -DRVW_deltax;
    DRVW_deltax = 0;
  }

  if (DRVW_deltay < 0)
  {
    DRVW_ndeltay = -DRVW_deltay;
    DRVW_deltay = 0;
  }

  if (DRVW_deltax < ORIG_SWIDTH && DRVW_deltay < ORIG_SHEIGHT)
  {
    rc.x = DRVW_deltax;
    rc.w = DRVW_cursorw - DRVW_ndeltax;

    if (rc.x + rc.w > ORIG_SWIDTH )
      rc.w -= rc.x + rc.w - (ORIG_SWIDTH - 1);

    rc.y = DRVW_deltay;
    rc.h = DRVW_cursorh - DRVW_ndeltay;

    if (rc.y + rc.h > ORIG_SHEIGHT)
      rc.h -= rc.y + rc.h - (ORIG_SHEIGHT - 1);

    // subroutine exploded
    DRVR_mouserect.x = rc.x;
    DRVR_mouserect.y = rc.y;
    DRVR_mouserect.w = rc.w;
    DRVR_mouserect.h = rc.h;
	
    if (is_visible()) // HOTTAG
      mblitter.blit(cursor->pixel, gfxsys.dm2mscreen, &rc, DRVW_ndeltax, DRVW_ndeltay, DRVW_cursorw, ORIG_SWIDTH, 0, cursor->alphamask);
	//	mblitter.blit(cursor->pixel, gfxsys.dm2screen, &rc, DRVW_ndeltax, DRVW_ndeltay, DRVW_cursorw, ORIG_SWIDTH, 0, cursor->alphamask);
  }
}

// was sub_3714
bool c_Tmouse::T1_driver_mouseint(c_evententry me)
{
  if (DRVW_blockmouseinputcounter || eventqueue.fetch_busy)
    return false;

  if (DRVW_mousecapturedcounter != 0)
  {
    me.x = entry.x;
    me.y = entry.y;
  }

  mousequeue.push(me);
  return true;
}

// --------------------------------------------------

// DRIVER-blitter

// blit a piece of a line within given bitmaps
void c_mblitter::blit_hline(c_pixel256* srcgfx, c_pixel256* destgfx, ui16 srcofs, ui16 destofs, ui16 pixels)
{
  DM2_COPY_MEMORY(DOWNCAST(c_pixel256, destgfx + destofs), DOWNCAST(c_pixel256, srcgfx + srcofs), pixels);
}

// blit masked a piece of a line within given bitmaps
// source is DRVP_srcmapptr
// destination is DRVP_destmapptr
// pixels: number of pixels
// mask: that pixels will be skipped in the copy
// was sub_2136
void c_mblitter::blit_hline_masked(c_pixel256* srcgfx, c_pixel256* destgfx, ui16 srcofs, ui16 destofs, ui16 pixels, c_pixel mask)
{
  srcgfx += srcofs;
  destgfx += destofs;

  for (i16 i = 0; i < pixels; i++)
  {
    c_pixel256 p = *srcgfx++;
    if (pixel_to_ui8(p) == pixel_to_ui8(mask))
      destgfx++;
    else
      *destgfx++ = p;
  }
}

// blit rectangle, remember source and destination - blitmap
// was sub_2169
void c_mblitter::blit(c_pixel256* srcmap, c_pixel256* destmap, c_rect* rectp, i16 sx, i16 sy, i16 sw, i16 dw, i8 masked, ui8 bmask)
{
  if (rectp->w <= 0 || rectp->h <= 0)
    return;

  i16 sofs = sy * sw + sx;
  i16 dofs = rectp->y * dw + rectp->x;

  for (i16 line = 0; line < rectp->h; sofs += sw, dofs += dw, line++)
  {
    i16 wm = (masked << 8) | (bmask & 0xff); // TODO not funny
    if (wm >= 0)
      blit_hline_masked(srcmap, destmap, sofs, dofs, rectp->w, ui8_to_pixel(bmask));
    else
      blit_hline(srcmap, destmap, sofs, dofs, rectp->w);
  }
}

// was sub_297A
// does 14 times this stretching:
// src:  abc defgh ijkl m  13 pixels
// dest: abccdefghhijkllm  16 pixels
void c_mblitter::blit_hline_stretched(c_pixel256* srcgfx, c_pixel256* destgfx, ui16 srcx, ui16 destx)
{
  c_pixel256* dssi = srcgfx + srcx;
  c_pixel256* esdi = destgfx + destx;
  c_pixel256 p;

  for (i16 i = 0; i < 14; i++)
  {
    *esdi++ = *dssi++;
    *esdi++ = *dssi++;
    p = *dssi++;
    *esdi++ = p;
    *esdi++ = p;
    *esdi++ = *dssi++;
    *esdi++ = *dssi++;
    *esdi++ = *dssi++;
    *esdi++ = *dssi++;
    p = *dssi++;
    *esdi++ = p;
    *esdi++ = p;
    *esdi++ = *dssi++;
    *esdi++ = *dssi++;
    *esdi++ = *dssi++;
    p = *dssi++;
    *esdi++ = p;
    *esdi++ = p;
    *esdi++ = *dssi++;
  }
}

// -----------------------

// was sub_3586
void c_Tmouse::driver_blockmouseinput(void)
{
  DRVW_blockmouseinputcounter++;
}

// added
void c_Tmouse::driver_unblockmouseinput(void)
{
  if (DRVW_blockmouseinputcounter > 0)
    DRVW_blockmouseinputcounter--;
}

// #############################################################
// #############################################################

// pulled driver function 9 driver_hidemouse (sub_387D)
void DM2_HIDE_MOUSE(void)
{
  Tmouse.mouse_invisible++;
}

// pulled driver function 8 driver_showmouse (was sub_3847)
void DM2_SHOW_MOUSE(void)
{
  if (Tmouse.mouse_invisible > 0)
    if (Tmouse.mouse_invisible-- == 1) // block executed 1->0 - triggered
      send_command(1);
}

// pulled driver function 6 driver_setmousecaptured(); was sub_3608
void DM2_MOUSE_SET_CAPTURE(void)
{
  send_command(5);
}

// pulled driver-function 7
// (was sub_361B, releases capture - mode)
void DM2_MOUSE_RELEASE_CAPTURE(void)
{
  send_command(2);
}

// was DM2_mouse_443c_040e, TODO: tell SPX new name
void DM2_REFRESHMOUSE(void)
{
  DM2_HIDE_MOUSE();
  send_command(3);
  DM2_SHOW_MOUSE();
}

// was sub_39BA
void DM2_CHOOSE_CURSOR3(void)
{
  send_command(6);
}

// was SKW_1031_050C, TODO: tell SPX new name
void DM2_RELEASE_MOUSE_CAPTURES(void)
{
  if (ddat.vcapture1 || ddat.vcapture2 || ddat.vcapture3)
  {
    ddat.vcapture1 = ddat.vcapture2 = ddat.vcapture3 = false;
    DM2_MOUSE_RELEASE_CAPTURE();
    Tmouse.hide();
    DM2_SHOW_MOUSE();
  }
}

// was SKW_443c_08ab, TODO: tell SPX new name
void DM2_GET_MOUSE_ENTRY_DATA(OO i16& mx, OO i16& my, OO i16& mb)
{
  Tmouse.driver_blockmouseinput();
  mx = Tmouse.entry.x;
  my = Tmouse.entry.y;
  mb = Tmouse.entry.b;
  Tmouse.driver_unblockmouseinput();
}
