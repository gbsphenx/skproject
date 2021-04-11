#include "util.h"
#include "../driver.h"
#include "dm2data.h"
#include "c_mouse.h"

x16 SKW_476d_05a9(void)
{
  userinterrupt(con(0x2), con(0x0));
  return *P16CAST(ddata.drvinterfaceptr);
}

x16 SKW_476d_050e(void)
{
  i16 val;

  // SPX: test with commenting this to bypass infinite loop
  // Note: Skwin handles this loop within _01b0_04e4
  //while (SKW_476d_05a9() == con(0x0));

  userinterrupt(con(0x1), con(0x0));
  val = *P16CAST(ddata.drvinterfaceptr);
  if (val < con(0x104d))
  {
    if (val < con(0x1048))
      return val;
    else if (val == con(0x1048))
      return con(0x4c);
    if (val == con(0x104b))
      return con(0x4b);
    return val;
  }
  else if (val == con(0x104d))
    return con(0x4d);
  else if (val < con(0x124b))
  {
    if (val == con(0x1050))
      return con(0x50);
    return val;
  }
  else if (val == con(0x124b))
    return con(0x4f);
  else if (val == con(0x124d))
    return con(0x51);
  return val;
}

// was SKW_1031_0b7e
void SKW_GETSINGLEMOUSEEVENT(void)
{
  if (ddata.singlemousedata_available)
  {
    ddata.singlemousedata_available = false;
    SKW_FIRE_QUEUE_MOUSE_EVENT(ddata.singlemouse.x, ddata.singlemouse.y, ddata.singlemouse.b);
  }
}

void SKW_1031_098e(void)
{
  while (SKW_476d_05a9() != con(0x0))
    SKW_476d_050e();

  ddata.fetchmouse_busy = true;
  x16 i = ddata.mousebufindex + 1;
  if (i > (NUM_MOUSEBUFENTRIES - 1))
    i = con(0x0);
  if (i != ddata.mouse_unk01)
  {
    x16 index1 = ddata.mouse_unk01;
    x16 index2 = ddata.mouse_unk01;
    ddata.mousebufentries = con(0x0);

    for(;;)
    {
      if (    ddata.mousebuf[index1].b == con(0x40)
           || ddata.mousebuf[index1].b == con(0x60)
           || ddata.mousebuf[index1].b == con(0x4)
         )
      {
        SKW_COPY_MEMORY(DOWNCAST(c_mousebufentry, &ddata.mousebuf[index2]), sizeof(c_mousebufentry), DOWNCAST(c_mousebufentry, &ddata.mousebuf[index1]));
        index2++;
        if (index2 > (NUM_MOUSEBUFENTRIES - 1))
          index2 = con(0x0);
        ddata.mousebufentries++;
      }
      if (index1 == ddata.mousebufindex)
        break;
      index1++;
      if (index1 > (NUM_MOUSEBUFENTRIES - 1))
        index1 = con(0x0);
    }

    index2--;
    if (index2 == con(0xffffffff))
      index2 = NUM_MOUSEBUFENTRIES - 1;
    ddata.mousebufindex = index2;
  }

  ddata.mouse_unk02 = con(0x0);
  ddata.mouse_unk03 = con(0x0);
  ddata.mouse_unk04 = con(0x0);
  ddata.mouse_unk05 = con(0xffffffff);
  ddata.mouse_unk09 = con(0xffffffff);
  ddata.mouse_unk08 = con(0x0);
  ddata.mouse_unk07 = con(0x0);
  ddata.mouse_unk0a = con(0x0);
  ddata.mouse_unk06 = con(0x0);
  ddata.fetchmouse_busy = false;
  SKW_GETSINGLEMOUSEEVENT();
}

void SKW_443c_00a9(s_munknown* p1, x16 x1, x16 y1, x16 x0, x16 y0)
{
  ddata.mouse_unkptr = p1;
  ddata.mouse_ry0 = y0;
  ddata.mouse_r.y = y0;
  ddata.mouse_ry1 = y1;
  ddata.mouse_r.h = y1 - y0 + 1;
  ddata.mouse_rx0 = x0;
  ddata.mouse_r.x = x0;
  ddata.mouse_rx1 = x1;
  ddata.mouse_r.w = x1 - x0 + 1;
  SKW_COPY_MEMORY(DOWNCAST(c_rect, &ddata.mouse_r), sizeof(c_rect), DOWNCAST(c_rect, &ddata.drvxmouserectptr->r));
  ddata.drvxmouserectptr->v = con(0x20);
  userinterrupt(con(0xc), con(0x0));
}

void SKW_FIRE_QUEUE_MOUSE_EVENT(x16 mx, x16 my, x16 buttons)
{
  static bool semaphore = false; // used by both threads

  while (semaphore);
  semaphore = true;

  if (ddata.fetchmouse_busy)
  {
    ddata.singlemousedata_available = true;
    ddata.singlemouse.x = mx;
    ddata.singlemouse.y = my;
    ddata.singlemouse.b = buttons;
  }
  else
  {
    ddata.fetchmouse_busy = true;
    i16 val5 = con(0x9);
    if (buttons != con(0x4) || ddata.mousebutton0x2)
    {
      if (buttons != con(0x40) && buttons != con(0x60))
        val5 = con(0x7);
    }
    ddata.mousebutton0x2 = false;
    x16 val1 = ddata.mousebufindex + 1;
    if (val1 > (NUM_MOUSEBUFENTRIES - 1))
      val1 -= NUM_MOUSEBUFENTRIES;
    if (val5 > ddata.mousebufentries)
    {
      ddata.mousebufindex = val1;
      ddata.mousebuf[val1].b = buttons;
      ddata.mousebuf[val1].x = mx;
      ddata.mousebuf[val1].y = my;
      ddata.mousebufentries++;
    }
    else
    {
      if (buttons == con(0x2))
        ddata.mousebutton0x2 = true;
    }
    ddata.fetchmouse_busy = false;
  }

  semaphore = false;
}

// belongs to queue_int2e
static x16 SKW_443c_00f8(x16 a, x16 b)
{
  c_rect* ptr2;
  x16 val3;
  s_munknown* ptr5;
  x16 val6;
  bool xflag;
  x16 vw_00 = b;
  x16 vw_04 = a;
  x16 vw_08;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_14;
  x16 vw_18;

  if (    ddata.mouse_setrect
       || vw_04 > ddata.mouse_rx1 // x/y exchanged BUGFIX Phase P1
       || vw_04 < ddata.mouse_rx0
       || vw_00 > ddata.mouse_ry1
       || vw_00 < ddata.mouse_ry0
     )
  {
    ddata.mouse_setrect = false;
    val6 = con(0x0);
    ddata.mouse_unk0b = val6;
    ptr5 = ddata.mouse_unkptr2;
    vw_14 = con(0x0);
    val3 = ddata.mouse_unk0c;
    vw_18 = ddata.mouse_unk0d;

    while (ptr5 != NULL)
    {
      ptr2 = &ptr5->u.pb6->r4;
      xflag = false;
      if (vw_04 >= ptr2->x)
      {
        vw_08 = vw_04;
        vw_0c = ptr2->x + ptr2->w - 1;
        if (vw_08 <= vw_0c)
        {
          xflag = true;
          val6 = DM2_MAX(val6, ptr2->x);
          val3 = DM2_MIN(val3, ptr2->x + ptr2->w - 1);
        }
        else
          val6 = DM2_MAX(val6, ptr2->x + ptr2->w - 1);
      }
      else
        val3 = DM2_MIN(val3, ptr2->x);

      if (vw_00 < ptr2->y)
      {
        vw_18 = DM2_MIN(vw_18, ptr2->y);
        ptr5 = ptr5->u.pb6->p0;
        continue;
      }

      vw_10 = vw_00;
      vw_0c = ptr2->y + ptr2->h - 1;

      if (vw_10 <= vw_0c)
      {
        vw_14 = DM2_MAX(vw_14, ptr2->y);
        vw_18 = DM2_MIN(vw_18, ptr2->y + ptr2->h - 1);
        if (!xflag)
        {
          ptr5 = ptr5->u.pb6->p0;
          continue;
        }
        ddata.mouse_unk0b = unsignedword(ptr5->b2);
        if (ptr5 != ddata.mouse_unkptr)
        {
          x8 buttons = ptr5->b4;
          if (buttons != con(0x0))
            SKW_FIRE_QUEUE_MOUSE_EVENT(vw_04, vw_10, unsignedword(buttons));
        }
        break;
      }
      else
      {
        vw_14 = DM2_MAX(vw_14, ptr2->y + ptr2->h - 1);;
        ptr5 = ptr5->u.pb6->p0;
      }
    }

    if (ddata.mouse_unkptr != NULL && ddata.mouse_unkptr != ptr5)
    {
      x8 buttons = ddata.mouse_unkptr->b5;
      if (buttons != con(0x0))
        SKW_FIRE_QUEUE_MOUSE_EVENT(vw_04, vw_00, unsignedword(buttons));
    }

    SKW_443c_00a9(ptr5, val3, vw_18, val6, vw_14); // x/y-parameters swapped
  }

  if (ddata.mouse_unk0b == con(0x2))
    return con(0x3);
  if (ddata.mouse_unk0b != con(0x1) || ddata.mouse_unk0e < con(0x0))
    return con(0x0);
  if (!ddata.mouse_unk0f)
    return con(0x1);
  return con(0x2);
}

// was interruptroutine R_4799, gets called by the driver
// to execute a mouseclick.
x16 queue_int2e(void)
{
  x16 val;

  if (ddata.drvmouseentryptr->b >= con(0x20))
    val = SKW_443c_00f8(ddata.drvmouseentryptr->x, ddata.drvmouseentryptr->y);
  else
  {
    SKW_FIRE_QUEUE_MOUSE_EVENT(ddata.drvmouseentryptr->x, ddata.drvmouseentryptr->y, ddata.drvmouseentryptr->b);
    val = con(0xffff);
  }
  ddata.drvmouseentryptr->x = val;

  return val;
}

void SKW_FIRE_HIDE_MOUSE_CURSOR(void)
{
  ddata.mouse_visibility++;
  userinterrupt(con(0x9), con(0x0));
}

void SKW_FIRE_SHOW_MOUSE_CURSOR(void)
{
  ddata.mouse_visibility--;
  userinterrupt(con(0x8), con(0x0));
}

void SKW_FIRE_MOUSE_EVENT_RECEIVER(x16 eaxw, x16 ebxw, x16 ecxw, t_gfxdata* edxpg, x16 argw0, x16 argw1, x16 argw2, t_gfxdata* argpg3, x16 argw4)
{
  i16* iface = P16CAST(ddata.drvinterfaceptr);
  iface[0] = eaxw;
  iface[1] = ebxw;
  iface[2] = ecxw;
  iface[3] = argw0;
  iface[4] = argw1;
  iface[5] = argw2;
  iface[6] = argw4;
  SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, argpg3), con(0x10), DOWNCAST(i8, ddata.drvinterfaceptr) + con(0xe));
  SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, edxpg), con(0x144), DOWNCAST(i8, ddata.drvinterfaceptr) + con(0x1e));
  userinterrupt(con(0xa), con(0x0));
}

void SKW_443c_0380(void)
{
  SKW_FIRE_MOUSE_EVENT_RECEIVER(con(0x0), con(0x0), con(0x0), UPCAST(t_gfxdata, ddata.mouse_cur1), con(0xc), con(0x10), con(0x4), ddata.paletteptr1, con(0xc));
  SKW_FIRE_MOUSE_EVENT_RECEIVER(con(0x1), con(0x0), con(0x0), UPCAST(t_gfxdata, ddata.mouse_cur2), con(0x10), con(0x10), con(0x4), ddata.paletteptr1, con(0xc));
#if 0
$  userinterrupt(con(0xb), con(0x0));
$  RG4W = CSSEG;
$  R_4A9A4();
#endif
}

void SKW_FIRE_MOUSE_SET_CAPTURE(void)
{
  userinterrupt(con(0x6), con(0x0));
}

void SKW_FIRE_MOUSE_RELEASE_CAPTURE(void)
{
  userinterrupt(con(0x7), con(0x0));
}

void SKW_443c_040e(void)
{
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  ddata.mouse_setrect = true;
  SKW_COPY_MEMORY(DOWNCAST(c_rect, &ddata.mouse_rect), sizeof(c_rect), DOWNCAST(c_rect, &ddata.drvxmouserectptr->r));
  ddata.drvxmouserectptr->v = con(0x20);
  userinterrupt(con(0xc), con(0x0));
  SKW_FIRE_SHOW_MOUSE_CURSOR();
}
