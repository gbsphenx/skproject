#include "c_xrect.h"
#include "util.h"
#include "startend.h"
#include "c_gfx_bmp.h"
#include "dm2data.h"

#define SCALE 10000

void c_xrectdat::init(void)
{
  queryrectsindex = 0;
  for (i16 i = 0; i < NUM_QUERYRECTS; i++) queryrects[i].init();
  rnodep_rectanglelist = NULL;
}

c_xrectdat xrectdat;

// ################################################################

// was SKW_098d_02a2
bool DM2_PT_IN_EXPANDED_RECT(i16 query, i16 x, i16 y)
{
  c_rect rc;

  if (DM2_QUERY_EXPANDED_RECT(query, &rc) == NULL)
    return false;

  return rc.pt_in_rect(x, y);
}

static i16 DM2_CALC_SIZE_OF_COMPRESSED_RECT(ui8 mask)
{
  i16 wret = 8;

  if (mask & 0x4)
    wret = 6;
  else
  {
    if (mask & 0x2)
      wret = 6;
    if (mask & 0x1)
      wret -= 2;
  }

  if (mask & 0x18)
    wret -= 2;

  return wret;
}

union u_vptr
{
  i8* b;
  i16* w;
};

// return: c_rect* or NULL
// was SKW_QUERY_RECT
static c_rect* DM2_QUERY_RECT(s_rnode* rnodep, i16 query)
{
  if (query == 0)
    return NULL;

  for (;;)
  {
    if (rnodep == NULL)
      return NULL;
    if (rnodep->min <= query && query <= rnodep->max)
      break;
    rnodep = rnodep->next;
  }

  query -= rnodep->min;

  if (++xrectdat.queryrectsindex >= NUM_QUERYRECTS)
    xrectdat.queryrectsindex = 0;

  c_rect* r = &xrectdat.queryrects[xrectdat.queryrectsindex];

  ui8 mask = rnodep->mask;

  if (mask & 0x2)
    r->x = unsignedword(rnodep->b_x);

  // now to different extensions of the struct

  u_vptr ptr;
  ptr.b = DOWNCAST(s_rnode, rnodep) + 10;

  if (mask & 0x1)
    r->y = *ptr.w++;

  ptr.b += query * DM2_CALC_SIZE_OF_COMPRESSED_RECT(mask);

  if (mask & 0x4)
  {
    r->x = unsignedword(*ptr.b++);
    r->y = unsignedword(*ptr.b++); // WARNING: original code did not extend to word at this point!
  }
  else // if maskbytes not set, x and/or y are already set
  {
    if ((mask & 0x2) == 0) r->x = *ptr.w++;
    if ((mask & 0x1) == 0) r->y = *ptr.w++;
  }

  // width and height either two bytes or two words
  if (mask & 0x8)
  {
    r->w = signedword(ptr.b[0]);
    r->h = signedword(ptr.b[1]);
  }
  else
  {
    if (mask & 0x10)
    {
      r->w = unsignedword(ptr.b[0]);
      r->h = unsignedword(ptr.b[1]);
    }
    else
    {
      r->w = ptr.w[0];
      r->h = ptr.w[1];
    }
  }

  return r;
}

// return never used
// TODO make references from wcp and wep
void DM2_rect_098d_04c7(i16 wa, i16 wd, i16 wb, i16* wcp, i16* wep)
{
  c_rect* rect1 = DM2_QUERY_RECT(xrectdat.rnodep_rectanglelist, wa);
  if (rect1 == NULL)
    return; // -1

  c_rect* rect2 = DM2_QUERY_RECT(xrectdat.rnodep_rectanglelist, wd);
  if (rect2 == NULL)
    return; // -1

  wb = DM2_BETWEEN_VALUE(0, 100, wb);
  if (wb != 0)
  {
    *wcp = (rect2->w - rect1->w) * wb / 100;
    *wep = (rect2->h - rect1->h) * wb / 100;
  }
  else
    *wcp = *wep = 0;
  // return wa;
}

class c_rinfo // basically a union to c_rect
{
  public:
    i16 mode1; // @00
    i16 mode2; // @02
    i16 datax; // @04
    i16 datay; // @06
};

// this one got pulled out from DM2_QUERY_BLIT_RECT
static void crdecode(i16 mode, i16 x0, i16 y0, i16 x1, i16 y1, OO i16& x, OO i16& y)
{
  switch (mode)
  {
    case 0:
      x = x0 - (x1 + 1) / 2;
      y = y0 - (y1 + 1) / 2;
    break;

    case 1:
      x = x0;
      y = y0;
    break;

    case 2:
      x = x0 - x1 + 1;
      y = y0;
    break;

    case 3:
      x = x0 - x1 + 1;
      y = y0 - y1 + 1;
    break;

    case 4:
      x = x0;
      y = y0 - y1 + 1;
    break;

    case 5:
      x = x0 - (x1 + 1) / 2;
      y = y0;
    break;

    case 6:
      x = x0 - x1 + 1;
      y = y0 - (y1 + 1) / 2;
    break;

    case 7:
      x = x0 - (x1 + 1) / 2;
      y = y0 - y1 + 1;
    break;

    case 8:
      x = x0;
      y = y0 - (y1 + 1) / 2;
    break;

    default: throw(THROW_DMABORT);
  }
}

// the rectangle informations coming from DM2_QUERY_RECT-calls here give
// special coded values that are interpreted then
c_rect* DM2_QUERY_BLIT_RECT(t_bmp* bmp, c_rect* blitrect, i16 query1, IO i16& xout, IO i16& yout, i16 query2)
{
  c_rinfo* rinfop1;
  c_rinfo* rinfop2;
  c_rinfo ri_04;
  c_rect rc;
  i16 x0;
  i16 y0;

  i16 blitrectmode;

  if (query1 == -1)
    return NULL;

  bool sign = query1 < 0;
  if (sign)
    query1 &= 0x7fff;

  rinfop2 = CHGCAST(c_rinfo, DM2_QUERY_RECT(xrectdat.rnodep_rectanglelist, query1));
  if (rinfop2 == NULL)
    return NULL;

  rc.set(-10000, -10000, 20000, 20000);
  if (query2 != -1)
  {
    ri_04 = *rinfop2; // copy
    ri_04.mode1 = query2;
    blitrectmode = query2;
    rinfop2 = &ri_04;
  }
  else
    blitrectmode = rinfop2->mode1;

  if (blitrectmode > 8)
  {
    if (blitrectmode == 9)
      return NULL;
    x0 = 0;
    y0 = 0;
    blitrectmode -= 10;
  }
  else
  {
    x0 = rinfop2->datax;
    y0 = rinfop2->datay;
  }

  if (sign)
  {
    x0 += xout;
    y0 += yout;
    xout = 0;
    yout = 0;
  }

  if (bmp == NULL && (xout <= 0 || yout <= 0))
    return NULL;

  i16 deltax = 0;
  i16 deltay = 0;
  bool flag = false;
  i16 query_w2 = 0;

  while (rinfop2->mode2 != 0)
  {
    if (rinfop2->mode1 < 10 || rinfop2->mode1 > 18)
    {
      rinfop1 = CHGCAST(c_rinfo, DM2_QUERY_RECT(xrectdat.rnodep_rectanglelist, rinfop2->mode2));
      if (rinfop1 == NULL)
        break;

      query_w2 = rinfop2->mode2;
      deltax = rinfop1->datax;
      deltay = rinfop1->datay;
      if (rinfop1->mode1 != 1)
      {
        if (rinfop1->mode1 != 9)
        {
          if (rinfop1->mode1 <= 8)
            flag = true;
        }
        else
        {
          if (rinfop2->mode1 <= 8)
            crdecode(rinfop2->mode1, rinfop2->datax, rinfop2->datay, rinfop1->datax, rinfop1->datay, OO deltax, OO deltay);

          if (flag)
          {
            flag = false;
            x0 += deltax;
            y0 += deltay;
            rc.x += deltax;
            rc.y += deltay;
          }

          if (deltax > rc.x)
            rc.x = deltax;
          if (rc.w + rc.x - 1 >= deltax + rinfop1->datax)
            rc.w = rinfop1->datax - rc.x + deltax;
          if (rc.y < deltay)
            rc.y = deltay;
          deltax = rinfop1->datay + deltay;
          if (rc.y + rc.h - 1 >= deltax)
            rc.h = deltay + rinfop1->datay - rc.y;
        }
      }
      else
      {
        x0 += deltax;
        y0 += deltay;
        rc.x += deltax;
        rc.y += deltay;
      }
    }
    else
    {
      c_rinfo* tmprect = CHGCAST(c_rinfo, DM2_QUERY_RECT(xrectdat.rnodep_rectanglelist, rinfop2->mode2));
      if (tmprect == NULL)
        break;
      query_w2 = tmprect->mode2;
      deltax = tmprect->datax;
      deltay = tmprect->datay;

      rinfop1 = CHGCAST(c_rinfo, DM2_QUERY_RECT(xrectdat.rnodep_rectanglelist, query_w2));
      if (rinfop1 == NULL)
        break;

      switch (tmprect->mode1)
      {
        case 0:
          deltay -= (rinfop1->datay + 1) / 2;
        case 5:
          deltax -= (rinfop1->datax + 1) / 2;
        case 1:
        break;

        case 3:
          deltay -= rinfop1->datay - 1;
        case 2:
          deltax -= rinfop1->datax - 1;
        break;

        case 6:
          deltax -= rinfop1->datax - 1;
        case 8:
          deltay -= (rinfop1->datay + 1) / 2;
        break;

        case 7:
          deltax -= (rinfop1->datax + 1) / 2;
        case 4:
          deltay -= rinfop1->datay - 1;
        break;

        default:
          return NULL;
      }

      rc.x += deltax;
      if (deltax > rc.x)
        rc.x = deltax;

      if (rinfop1->datax + deltax <= rc.x + rc.w - 1)
        rc.w = rinfop1->datax - rc.x + deltax;
      else
        rc.w = rinfop1->datax + deltax;

      rc.y += deltay;
      if (rc.y < deltay)
        rc.y = deltay;
      if (deltay + rinfop1->datay <= rc.y + rc.h - 1)
        rc.h = deltay + rinfop1->datay - rc.y;

      switch (rinfop2->mode1 - 10)
      {
        case 0:
          deltay += (rinfop1->datay + 1) / 2;
        case 5:
          deltax += (rinfop1->datax + 1) / 2;
        case 1:
        break;

        case 3:
          deltay += rinfop1->datay - 1;
        case 2:
          deltax += rinfop1->datax - 1;
        break;

        case 6:
          deltax += rinfop1->datax - 1;
        case 8:
          deltay += (rinfop1->datay + 1) / 2;
        break;

        case 7:
          deltax += (rinfop1->datax + 1) / 2;
        case 4:
          deltay += rinfop1->datay - 1;
        break;

        default:
          return NULL;
      }

      deltax += rinfop2->datax;
      x0 += deltax;
      y0 += deltay + rinfop2->datay;
    }
    rinfop2 = rinfop1;
  }

  i16 basex = xout;
  if (basex == 0)
    basex = getbmpheader(bmp)->width;

  i16 basey = yout;
  if (basey == 0)
    basey = getbmpheader(bmp)->height;

  crdecode(blitrectmode, x0, y0, basex, basey, OO blitrect->x, OO blitrect->y);

  if (ddat.v1e01d0) // but is always false atm
    rc = glblrects.dm2rect1; // copy

  if (ddat.v1e01d8 && ddat.v1e025c != 0 && query_w2 == 3)
    if (rc.unify(&glblrects.dm2rect2, OO xout, OO yout) == NULL)
      return NULL;

  i16 dx = rc.x - blitrect->x;
  if (dx > 0)
  {
    xout = dx;
    blitrect->x = rc.x;
    blitrect->w = DM2_MIN(basex - dx, rc.w);
  }
  else
  {
    xout = 0;
    blitrect->w = DM2_MIN(basex, dx + rc.w);
  }

  i16 dy = rc.y - blitrect->y;
  if (dy > 0)
  {
    yout = dy;
    blitrect->y = rc.y;
    blitrect->h = DM2_MIN(basey - dy, rc.h);
  }
  else
  {
    yout = 0;
    blitrect->h = DM2_MIN(basey, dy + rc.h);
  }

  if (blitrect->w <= 0 || blitrect->h <= 0)
    return NULL;

  return blitrect;
}

void DM2_QUERY_TOPLEFT_OF_RECT(i16 wn, OO i16& x, OO i16& y)
{
  c_rect rc;

  i16 vw_08 = 1;
  i16 vw_0c = 1;

  DM2_QUERY_BLIT_RECT(NULL, &rc, wn, IO vw_0c, IO vw_08);

  x = rc.x;
  y = rc.y;
}

c_rect* DM2_SCALE_RECT(i16 query, i16 scalew, i16 scaleh, c_rect* r)
{
  i16 w;
  i16 h;

  c_rect* qrect1 = DM2_QUERY_RECT(xrectdat.rnodep_rectanglelist, query);
  if (qrect1 == NULL || qrect1->y == 0)
    return NULL;

  c_rect* qrect2 = DM2_QUERY_RECT(xrectdat.rnodep_rectanglelist, qrect1->y);
  if (qrect2 == NULL || qrect2->x != 9) // 9 is a rinfo-marker!
    return NULL;

  if (scalew != SCALE)
    w = scalew * qrect2->w / SCALE;
  else
    w = qrect2->w;

  if (scaleh != SCALE)
    h = scaleh * qrect2->h / SCALE;
  else
    h = qrect2->h;

  if (w == 0 && scalew != 0)
    w = 1;

  if (h == 0 && scaleh != 0)
    h = 1;

  if (w <= 0 || h <= 0)
    return NULL;

  return DM2_QUERY_BLIT_RECT(NULL, r, query, IO w, IO h);
}

c_rect* DM2_QUERY_EXPANDED_RECT(i16 query, c_rect* r)
{
  return DM2_SCALE_RECT(query, SCALE, SCALE, r);
}

// had a 3rd argument (function pointer to memoryallocator), but it's always the same one
void DM2_COMPRESS_RECTS(void* buffer, s_rnode* firstnode)
{
  u_vptr src;
  u_vptr dst;

  src.b = DOWNCAST(void, buffer);
  s_rnode* np = firstnode;
  dst.w = src.w++;
  if (*dst.w != wcon(0xfc0d))
    return;
  i16 loop0 = *src.w++;
  i16* wptr = src.w;
  src.w += 2 * loop0;
  while (np->next != NULL)
    np = np->next;

  while (--loop0 >= 0)
  {
    i16 min = *wptr++;
    i16 max = *wptr++;
    i16 loop1 = max - min + 1;
    u_vptr bupptr = src;
    ui8 mask = 0x1f;
    i16 val1 = *src.w;
    i16 val2 = *(src.w + 1);
    do
    {
      if (*src.w++ != val1)
        mask &= 0xfd;
      if (*src.w != val2)
        mask &= 0xfe;
      if (*src.w++ > 0xff)
        mask &= 0xfb;
      for (i16 i = 2; i != 0; i--)
      {
        i16 v = *src.w++;
        if (v < 0 || v > 0xff)
          mask &= 0xef;
        if (v < wcon(0xff80) || v > 0x7f)
          mask &= 0xf7;
      }
    } while (--loop1 > 0);

    if (mask & 0x3)
      mask &= 0xfb;
    i16 loop2 = max - min + 1;
    i16 size = DM2_CALC_SIZE_OF_COMPRESSED_RECT(mask) * loop2 + lcon(0xa);
    if (mask & 0x1)
      size += 2;
    np->next = XUPCAST(s_rnode, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(signedlong(size), false));

    np = np->next;
    np->next = NULL;
    np->min = min;
    np->max = max;
    np->mask = mask;
    np->b_x = val1; // truncated

    dst.b = DOWNCAST(s_rnode, np) + 10;
    if (mask & 0x1)
      *dst.w++ = val2;

    src = bupptr;
    do
    {
      if (mask & 0x4)
      {
        *dst.b++ = *src.b++;
        src.b++;
        *dst.b++ = *src.b++;
        src.b++;
      }
      else
      {
        if (!(mask & 0x2))
          *dst.w++ = *src.w;
        src.w++;
        if (!(mask & 0x1))
          *dst.w++ = *src.w;
        src.w++;
      }

      if (mask & 0x18)
      {
        *dst.b++ = *src.b++;
        src.b++;
        *dst.b++ = *src.b++;
        src.b++;
      }
      else
      {
        *dst.w++ = *src.w++;
        *dst.w++ = *src.w++;
      }
    } while (--loop2 > 0);
  }
}
