#include <startend.h>
#include <util.h>
#include <fileio.h>
#include <uitmouse.h>
#include <skmcursr.h>

#include <dm2debug.h>

// this one is shared by both threads.
// while the common program writes to those datas, the eventthread
// is read-only here.
// there are two additional rectangle-infos in mouse2.dat, requested
// by DM2_DRAW_DIALOGUE_PICT

#define MOUSECURSORFILE1 "bin/v5/mouse1.dat"
#define MOUSECURSORFILESIZE1 (0x60)
#define MOUSECURSORFILE2 "bin/v5/mouse2.dat"

c_pixel16 mouse_cur1[MOUSECURSORFILESIZE1];
s_cursor2 mouse_cur2;

void init_mousecursors(void)
{
	SPX_DEBUG_PRINT("INIT_MOUSECURSORS:INIT\n");
  DM2_READ_BINARY(MOUSECURSORFILE1, DOWNCAST(c_pixel16, mouse_cur1), MOUSECURSORFILESIZE1);
  DM2_READ_BINARY(MOUSECURSORFILE2, DOWNCAST(s_cursor2, &mouse_cur2), MOUSECURSORFILESIZE2);
}

s_mcursor DRV_cursor[4];
// Note: idx 2 and 3 preserved for icon-cursors

// generates the current cursor
// the incoming driver_blockmouseinput has been pulled out
// the final T1_execmousefifo() + driver_unblockmouseinput(); have been pulled out!
// old name DM2_MOUSE_EVENT_RECEIVER, TODO tell SPX new name
void generate_cursor(c_pixel* srcmap, i16 cursoridx, i16 hx, i16 hy, i16 w, i16 h, t_resolution res, t_palette* palette, ui8 midx)
{
  if (cursoridx < 0 || cursoridx >= 4)
    return; // but never happens, all calls are well indexed
  // note: in that case T1_execmousefifo() must not be called

  Tmouse.driver_blockmouseinput();

  s_mcursor* cursor = &DRV_cursor[cursoridx];

  ui8 alphamask;
  if (res == BPP_4)
  {
    for (i16 y = 0, i = 0; y < h; y++)
      for (i16 x = 0; x < w; x++, i++)
      {
        i16 wis = MK_EVEN(w) * y + x;
        c_pixel16* remap = reinterpret_cast<c_pixel16*>(srcmap);
        c_pixel16 p = remap[wis >> 1]; // TODO: check it, because sar used
        if (!IS_ODD(wis))
          p = p.ltor(); // TODO: check it, because sar used
        else
          p = p.getr();
        cursor->pixel[i] = palettecolor_to_pixel(palette[p.mkidx()]);
      }
    alphamask = palettecolor_to_ui8(palette[midx]);
  }
  else
  {
    c_pixel256* remap = reinterpret_cast<c_pixel256*>(srcmap);
    for (i16 i = 0; i < w * h; i++) // BUGFIX - was <=
      cursor->pixel[i] = remap[i];
    alphamask = midx;
  }

  cursor->hx = mkb(hx);
  cursor->hy = mkb(hy);
  cursor->w = mkb(w);
  cursor->h = mkb(h);
  cursor->alphamask = alphamask;

  Tmouse.driver_unblockmouseinput();
}

// was DM2_mouse_443c_0380, TODO tell SPX new name
void DM2_INITBASICCURSORS(void)
{
  generate_cursor(mouse_cur1, 0, 0, 0, 12, 16, BPP_4, paldat.palette, 12);
  generate_cursor(mouse_cur2.pixel, 1, 0, 0, 16, 16, BPP_4, paldat.palette, 12);
}
