#include <uikeybd.h>
#include <uitmouse.h>
#include <skeventq.h>
#include <dm2data.h>
#include <skstr.h>
#include <gfxstr.h>

#include <dm2debug.h>

c_eventqueue eventqueue;

void c_eventqueue::init(void)
{
	SPX_DEBUG_PRINT("EVENTQUEUE:INIT\n");
  idx = 0;
  out_idx = entries = 0;
  for (i16 i = 0; i < EVENTQUEUELEN; i++)
    data[i].init();
  fetch_busy = false;
  singleevent_available = false;
  singleevent.init();

  event_unk02 = 0;
  glbUIClickEventIndex = 0;
  glbUIClickEventLast = 0;
  event_unk05 = 0;
  event_unk06 = 0;
  event_unk07 = 0;
  event_unk08 = 0;
  event_unk09 = 0;
  event_unk0a = 0;
  event_heroidx = E_HERO0;
  event_unk0f = false;
}

void c_eventqueue::set(i16 i, i16 x, i16 y, i16 b)
{
  idx = i;
  data[i].x = x;
  data[i].y = y;
  data[i].b = b;
  entries++;
}

// was SKW_1031_0b7e
// was DM2_GETSINGLEMOUSEEVENT, TODO: tell SPX new name
void c_eventqueue::PROCESS_SINGLEEVENT(void)
{
  if (singleevent_available)
  {
    singleevent_available = false;
    QUEUE_EVENT(eventqueue.singleevent.x, eventqueue.singleevent.y, eventqueue.singleevent.b);
  }
}

// can be called by both threads
// was DM2_QUEUE_MOUSE_EVENT, TODO tell SPX new name
void c_eventqueue::QUEUE_EVENT(i16 mx, i16 my, i16 mb)
{
  static bool semaphore = false; // used by both threads
  static bool button0x2 = false;

  while (semaphore);
  semaphore = true;

  if (fetch_busy)
  {
    singleevent_available = true;
    singleevent.x = mx;
    singleevent.y = my;
    singleevent.b = mb;
  }
  else
  {
    fetch_busy = true;
    i16 val5 = 9;
    if ((mb != 0x4 || button0x2) && mb != 0x40 && mb != 0x60)
      val5 = 7;
    button0x2 = false;
    i16 i1 = idx + 1;
    if (i1 >= EVENTQUEUELEN)
      i1 = 0;
    if (val5 > entries)
      set(i1, mx, my, mb);
    else if (mb == 0x2)
      button0x2 = true;
    fetch_busy = false;
  }

  semaphore = false;
}

// TODO: optimize temporaries
#include <emu.h>
#include <regs.h>

static void DM2_3929_0b01(i32 eaxl, t_text c)
{
  c_nreg _RG1; RG1L = eaxl;
  t_text tarr_00[2];

  tarr_00[0] = c;
  tarr_00[1] = 0;
  RG1L = signedlong(RG1W);
  DM2_DISPLAY_HINT_TEXT(RG1L, tarr_00);
}

static void DM2_3929_0b20(i16 eaxw, i32 edxl)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2W = eaxw;
  t_text tarr_00[0x10];

  DM2_LTOA10(edxl, tarr_00);
  RG2L = signedlong(RG2W);
  DM2_DISPLAY_HINT_TEXT(RG2L, tarr_00);
  DM2_DISPLAY_HINT_TEXT(RG2L, ddat.v1d70e4);
}

static void DM2_3e74_5b7c(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  t_text tarr_00[0x10];

  RG2L = RG1L;
  RG4W = ddat.v1d6795;
  RG1L = signedlong(RG4W);
  RG3L = unsignedlong(table1d6797[RG1L]);
  RG1L = unsignedlong(RG2W);
  RG4L++;
  ddat.v1d6795 = RG4W;
  RG1W = DM2_keybd_476d_05b6(RG1W);
  if (RG3W == RG1W)
  {
    RG1L = signedlong(ddat.v1d6795);
    if (table1d6797[RG1L] != 0)
      return;
    DM2_DISPLAY_HINT_TEXT(lcon(0xf), CHGCAST(t_text, v1d1118));
    RG4P = ADRESSOF(t_text, tarr_00);
    RG1T = DM2_LTOA10(2, RG4T);
    DM2_DISPLAY_HINT_TEXT(lcon(0xf), RG1T);
    DM2_3929_0b01(lcon(0xf), '.');
    DM2_3929_0b20(15, 12);
    DM2_3929_0b20(15, dm2_dballochandler.v1e0a0c);
    DM2_STRCPY(tarr_00, CHGCAST(t_text, v1d111f));
    if (dm2_dballochandler.v1e13fe[0] != 0)
      tarr_00[0x0] = lcon(0x47);
    if (dm2_dballochandler.v1e13fe[1] != 0)
      tarr_00[0x1] = lcon(0x53);
    if (dm2_dballochandler.v1e13fe[2] != 0)
      tarr_00[0x2] = lcon(0x45);
    DM2_DISPLAY_HINT_TEXT(lcon(0xf), tarr_00);
  }
  RG4L = 0;
  ddat.v1d6795 = RG4W;
}

// adds a non-mousebutton-entry,
// .b = 0x20 was argument, but called with 0x20 only
// was SKW_1031_0d36, TODO tell SPX new name
void c_eventqueue::QUEUE_0x20(i16 edxw)
{
  DM2_3e74_5b7c(unsignedlong(edxw));
  fetch_busy = true;
  if (entries < 7)
  {
    if (++idx >= EVENTQUEUELEN)
      idx = 0;
    data[idx].b = 0x20;
    data[idx].x = edxw;
    entries++;
  }
  fetch_busy = false;
  PROCESS_SINGLEEVENT();
}

void c_eventqueue::event_1031_098e(void)
{
  while (DM2_HAS_KEY())
    DM2_GETKEY_TRANSLATED();

  fetch_busy = true;
  i16 i1 = idx + 1;
  if (i1 >= EVENTQUEUELEN)
    i1 = 0;
  if (i1 != out_idx)
  {
    i16 index1 = out_idx;
    i16 index2 = out_idx;
    entries = 0;

    for (;;)
    {
      if (    data[index1].b == 0x40
           || data[index1].b == 0x60
           || data[index1].b == 0x4
         )
      {
        if (index1 != index2) // added!
          data[index1] = data[index2]; // copy struct
        if (++index2 >= EVENTQUEUELEN)
          index2 = 0;
        entries++;
      }
      if (index1 == idx)
        break;
      if (++index1 >= EVENTQUEUELEN)
        index1 = 0;
    }

    if (--index2 == -1)
      index2 = EVENTQUEUELEN - 1;
    idx = index2;
  }

  event_unk02 = 0;
  glbUIClickEventIndex = 0;
  glbUIClickEventLast = 0;
  event_unk05 = -1;
  event_unk06 = 0;
  event_unk07 = 0;
  event_unk08 = 0;
  event_unk09 = -1;
  event_unk0a = 0;
  fetch_busy = false;
  PROCESS_SINGLEEVENT();
}
