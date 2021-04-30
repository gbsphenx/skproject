#include "util.h"
#include "c_gfx_blit.h"
#include "c_buttons.h"

// TODO one function needs:
void DM2_gfxmain_0b36_0cbe(c_buttongroup* buttongroup, bool alloc);

c_buttongroup dm2_buttongroup1; // old name v1d6964
c_buttongroup dm2_buttongroup2; // old name v1d694c 

void c_button::init(void)
{
  dbidx = NODATA;
  r.init();
  groupsize = 0;
}

void c_buttongroup::init(void)
{
  button.init();
  for (int i = 0; i < 5; i++)
    rects[i].init();
}

void init_global_buttongroups(void)
{
  dm2_buttongroup1.init();
  dm2_buttongroup2.init();
}

c_rect* DM2_OFFSET_RECT(c_buttongroup* buttongroup, c_rect* rd, c_rect* rs)
{
  rd->set(
    rs->x - buttongroup->button.r.x,
    rs->y - buttongroup->button.r.y,
    rs->w,
    rs->h);

  return rd;
}

// was DM2_buttons_0b36_0d67 TODO tell SPX new name
void DM2_ADJUST_BUTTONGROUP_RECTS(c_buttongroup* buttongroup, c_rect* rect)
{
  if (rect == NULL)
    return;

  i16 cnt = 0;
  for (;;)
  {
    if (cnt < buttongroup->button.groupsize)
    {
      c_rect* r = &buttongroup->rects[cnt];
      if (   r->x <= rect->x
          && (r->x + r->w - 1) >= (rect->x + rect->w - 1)
          && r->y <= rect->y
          && (r->y + r->h - 1) >= (rect->y + rect->h - 1)
         )
        return;

      r = &buttongroup->rects[cnt];
      if (   r->x >= rect->x
          && (r->x + r->w - 1) <= (rect->x + rect->w - 1)
          && r->y >= rect->y
          && (r->y + r->h - 1) <= (rect->y + rect->h - 1)
         )
        break;
    }
    else
    {
      if (cnt >= 5)
        DM2_gfxmain_0b36_0cbe(buttongroup, false);
      cnt = buttongroup->button.groupsize++;
      break;
    }
    cnt++;
  }

  c_rect* r = &buttongroup->rects[cnt];

  *r = *rect; // copy

  i16 deltax = r->x - buttongroup->button.r.x;
  if (deltax < 0)
  {
    r->w += deltax;
    if (r->w <= 0)
    {
      buttongroup->button.groupsize--;
      return;
    }
    r->x -= deltax;
  }

  i16 deltay = r->y - buttongroup->button.r.y;
  if (deltay < 0)
  {
    r->h += deltay;
    if (r->h <= 0)
    {
      buttongroup->button.groupsize--;
      return;
    }
    r->y -= deltay;
  }

  deltax = (buttongroup->button.r.x + buttongroup->button.r.w - 1) - (r->x + r->w - 1);
  if (deltax < 0)
    r->w += deltax;

  deltay = (buttongroup->button.r.y + buttongroup->button.r.h - 1) - (r->y + r->h - 1);
  if (deltay < 0)
    r->h += deltay;
}

void DM2_FILL_RECT_SUMMARY(c_buttongroup* buttongroup, c_rect* blitrect, c_pixel pixel)
{
  if (blitrect != NULL)
  {
    c_rect r;

    // assuming: 1st argument doesn't equal gfxsys.dm2screen
    blitter.fill(PIXELCAST(dm2_dballochandler.DM2_GET_BMP(buttongroup->button.dbidx)), pixel, mkuw(buttongroup->button.r.w), DM2_OFFSET_RECT(buttongroup, &r, blitrect), BPP_8);
    DM2_ADJUST_BUTTONGROUP_RECTS(buttongroup, blitrect);
  }
}
