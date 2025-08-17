#include <stdio.h> // NULL

#include <skrect.h>

#include <dm2debug.h>

void c_rect::init(void)
{
  x = y = w = h = 0;
}

// was DM2_SET_SRECT
void c_rect::set(i16 nx, i16 ny, i16 nw, i16 nh)
{
  x = nx; y = ny; w = nw; h = nh;
}

// was DM2_SET_ORIGIN_RECT
void c_rect::set_origin(i16 nw, i16 nh)
{
  x = y = 0; w = nw; h = nh;
}

// was DM2_INFLATE_RECT
void c_rect::inflate(i16 dw, i16 dh)
{
  x -= dw;
  y -= dh;
  w += 2 * dw;
  h += 2 * dh;
}

// old name: DM2_UNION_RECT
c_rect* c_rect::unify(c_rect* rs, OO i16& ox, OO i16& oy)
{
  i16 dx0 = rs->x - x;
  if (dx0 <= 0)
    ox = 0;
  else
  {
    ox = dx0;
    x += dx0;
    w -= dx0;
  }

  i16 dy0 = rs->y - y;
  if (dy0 <= 0)
    oy = 0;
  else
  {
    oy = dy0;
    y += dy0;
    h -= dy0;
  }

  i16 dx1 = (x + w) - (rs->x + rs->w);
  if (dx1 > 0)
    w -= dx1;

  i16 dy1 = (y + h) - (rs->y + rs->h);
  if (dy1 > 0)
    h -= dy1;

  if (w <= 0 || h <= 0)
    return NULL;

  return this;
}

// was DM2_CALC_CENTERED_RECT_IN_RECT
void c_rect::calc_centered_rect_in_rect(c_rect* rs, i16 nw, i16 nh)
{
  x = rs->x + (rs->w - nw + 1) / 2;
  y = rs->y + (rs->h - nh + 1) / 2;
  w = nw;
  h = nh;
}

// was DM2_PT_IN_RECT
bool c_rect::pt_in_rect(i16 px, i16 py) const
{
  return    (x <= px) && (px <= (x + w - 1))
         && (y <= py) && (py <= (y + h - 1));
}

// ################################################################

c_tmprects tmprects;

void c_tmprects::init(void)
{
	SPX_DEBUG_PRINT("TMPRECTS:INIT\n");
	tmprectindex = 0;
  for (i16 i = 0; i < NUM_TMPRECTS; i++)
    tmprects[i].init();
}

// not really an allocation, and there is no deallocation,
// it simply uses a ringbuffer.

// was DM2_ALLOC_TEMP_RECT
c_rect* c_tmprects::alloc_tmprect(i16 x, i16 y, i16 w, i16 h)
{
  c_rect* r = &tmprects[tmprectindex];

  if (++tmprectindex >= NUM_TMPRECTS)
    tmprectindex = 0;

  r->set(x, y, w, h);

  return r;
}

// was DM2 _ALLOC_TEMP_ORIGIN_RECT
c_rect* c_tmprects::alloc_origin_tmprect(i16 w, i16 h)
{
  return alloc_tmprect(0, 0, w, h);
}

// ################################################################

// TODO: what are the global rectangles good for ?

c_glblrects glblrects;

void c_glblrects::init(void)
{
	SPX_DEBUG_PRINT("GLOBALRECTS:INIT\n");
  dm2rect1.init();
  dm2rect2.set(21, 8, 182, 110);
  dm2rect3.init();
  dm2rect4.init();
  dm2rect5.init();
}
