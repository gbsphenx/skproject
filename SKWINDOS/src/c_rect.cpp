#include "util.h"
#include "c_rect.h"
#include "dm2data.h"

void c_rect::init(void)
{
  x = y = w = h = 0;
}

void SKW_SET_SRECT(c_rect* r, x16 x, x16 y, x16 w, x16 h)
{
  r->x = x;
  r->y = y;
  r->w = w;
  r->h = h;
}

void SKW_SET_ORIGIN_RECT(c_rect* r, x16 w, x16 h)
{
  r->x = 0;
  r->y = 0;
  r->w = w;
  r->h = h;
}

c_rect* SKW_ALLOC_TEMP_RECT(x16 x, x16 y, x16 w, x16 h)
{
  c_rect* r = &ddata.tmprects[ddata.tmprectindex];

  if (++ddata.tmprectindex >= NUM_TMPRECTS)
    ddata.tmprectindex = 0;
  r->x = x;
  r->y = y;
  r->w = w;
  r->h = h;
  return r;
}

c_rect* SKW_ALLOC_TEMP_ORIGIN_RECT(x16 w, x16 h)
{
  return SKW_ALLOC_TEMP_RECT(0, 0, w, h);
}

void SKW_INFLATE_RECT(c_rect* r, x16 w, x16 h)
{
  r->x -= w;
  r->y -= h;
  r->w += 2 * w;
  r->h += 2 * h;
}

c_rect* SKW_UNION_RECT(c_rect* rd, x16* tx, x16* ty, c_rect* rs)
{
  i16 dx0 = rs->x - rd->x;
  if (dx0 <= con(0x0))
    *tx = con(0x0);
  else
  {
    *tx = dx0;
    rd->x += dx0;
    rd->w -= dx0;
  }

  i16 dy0 = rs->y - rd->y;
  if (dy0 <= con(0x0))
    *ty = con(0x0);
  else
  {
    *ty = dy0;
    rd->y += dy0;
    rd->h -= dy0;
  }

  i16 dx1 = (rd->x + rd->w - 1) - (rs->x + rs->w - 1);
  if (dx1 > con(0x0))
    rd->w -= dx1;

  i16 dy1 = (rd->y + rd->h - 1) - (rs->y + rs->h - 1);
  if (dy1 > con(0x0))
    rd->h -= dy1;

  if (rd->w <= con(0x0) || rd->h <= con(0x0))
    rd = NULL;

  return rd;
}

void SKW_CALC_CENTERED_RECT_IN_RECT(c_rect* rd, x16 w, x16 h, c_rect* rs)
{
	if (rd == NULL || rs == NULL) return;	// SPX: add check to prevent crash
  rd->w = w;
  rd->x = rs->x + (rs->w - rd->w + 1) / 2;
  rd->h = h;
  rd->y = rs->y + (rs->h - rd->h + 1) / 2;
}

bool SKW_PT_IN_RECT(c_rect* r, x16 x, x16 y)
{
	if (r == NULL) return false;	// SPX: add check to prevent crash
  return
     (    x >= r->x && x <= (r->x + r->w - 1)
       && y >= r->y && y <= (r->y + r->h - 1)
     );
}

// was SKW_098d_02a2
bool SKW_PT_IN_EXPANDED_RECT(x16 n, x16 x, x16 y)
{
  c_rect r;
  if (SKW_QUERY_EXPANDED_RECT(n, &r) == NULL)
    return 0;
  return SKW_PT_IN_RECT(&r, x, y);
}

static x16 SKW_CALC_SIZE_OF_COMPRESSED_RECT(ui8 mask)
{
  x16 ret = 8;
  if (mask & con(0x4))
    ret = 6;
  else
  {
    if (mask & con(0x2))
      ret = 6;
    if (mask & con(0x1))
      ret -= 2;
  }
  if (mask & con(0x18))
    ret -= 2;
  return ret;
}

// return: c_rect* oder NULL
// ptr is the same pointer for each call (ddata.rectanglelist)
static c_rect* SKW_QUERY_RECT(s_rnode* ptr, x16 val)
{
  if (val == 0)
    return NULL;

  x16 n;
  for(;;)
  {
    if (ptr == NULL)
      return NULL;
    n = ptr->lo;
    if (val >= n && val <= ptr->hi)
      break;
    ptr = ptr->next;
  }
  val -= n;

  if (++ddata.queryrectindex >= NUM_QUERYRECTS)
    ddata.queryrectindex = 0;

  c_rect* r = &ddata.queryrects[ddata.queryrectindex];

  ui8 mask = ptr->mask;

  if (mask & con(0x2))
    r->x = unsignedword(ptr->x);

  // now to different extensions of the struct
  // (this includes the option that ptr->x got no meaning
  //  and x comes later)

  x8* p = (x8*)ptr; p += 10;

  if (mask & con(0x1)) { r->y = *(x16*)p; p += 2; }

  p += signedlong(val) * unsignedlong(SKW_CALC_SIZE_OF_COMPRESSED_RECT(mask));

  if (mask & con(0x4))
  {
    r->x = unsignedword(*p++);
    r->y = unsignedword(*p++); // WARNING: original code did not extend to word at this point!
  }
  else // if maskbytes not set, x and/or y are already set
  {
    if (!(mask & con(0x2))) { r->x = *(x16*)p; p += 2; }
    if (!(mask & con(0x1))) { r->y = *(x16*)p; p += 2; }
  }

  // width and height either two bytes or two words
  if (mask & con(0x8))
  {
    r->w = signedword(p[0]);
    r->h = signedword(p[1]);
  }
  else
  {
    if (mask & con(0x10))
    {
      r->w = unsignedword(p[0]);
      r->h = unsignedword(p[1]);
    }
    else
    {
      x16* wp = (x16*)p;
      r->w = wp[0];
      r->h = wp[1];
    }
  }

  return r;
}

// stacksize was 0x14
// TODO: return never checked
x16 SKW_098d_04c7(x16 a, x16 b, x16* cp, x16 d, x16* ep)
{
  c_rect* r1 = SKW_QUERY_RECT(ddata.rectanglelist, a);
  if (r1 == NULL) // BUGFIX Phase P1
    return con(0xffffffff);

  c_rect* r2 = SKW_QUERY_RECT(ddata.rectanglelist, d);
  if (r2 == NULL) // BUGFIX Phase P1
    return con(0xffffffff);

  b = DM2_BETWEEN_VALUE(0, 100, b);
  if (b != 0)
  {
    *cp = (r2->w - r1->w) * b / 100;
    *ep = (r2->h - r1->h) * b / 100;
  }
  else
    *cp = *ep = 0;
  return a;
}

// stacksize was 0x4c
c_rect* SKW_QUERY_BLIT_RECT(t_gfxdata* gfxdataarr, x16 query1, x16* xaddptr, c_rect* blitrect, x16* yaddptr, x16 query2)
{
  c_rect* rect1;
  c_rect* rect2;
  c_rect rc_04;
  c_rect rc_0c;
  bool flag;
  x16 deltax;
  x16 deltay;
  x16 x0;
  x16 y0;

  ui16 mode1st;
  x16 modex;
  x16 modey = con(0x0);

  if (query1 == con(0xffffffff))
    return NULL;

  bool sign = query1 & con(0x8000);
  if (sign)
    query1 &= con(0x7fff);

  rect2 = SKW_QUERY_RECT(ddata.rectanglelist, query1);
  if (rect2 == NULL)
    return NULL;

  SKW_SET_SRECT(&rc_0c, con(0xffffd8f0), con(0xffffd8f0), con(0x4e20), con(0x4e20));
  if (query2 != con(0xffffffff))
  {
    SKW_COPY_RECT(rect2, &rc_04);
    rc_04.x = query2;
    modex = query2;
    rect2 = &rc_04;
  }
  else
    modex = rect2->x;
  if (modex > con(0x8))
  {
    if (modex == con(0x9))
      return NULL;
    x0 = con(0x0);
    y0 = con(0x0);
    modex -= con(0xa);
  }
  else
  {
    x0 = rect2->w;
    y0 = rect2->h;
  }
  if (sign)
  {
    x0 += *xaddptr;
    y0 += *yaddptr;
    *xaddptr = con(0x0);
    *yaddptr = con(0x0);
  }
  if (gfxdataarr == NULL && (*xaddptr <= con(0x0) || *yaddptr <= con(0x0)))
    return NULL;
  deltax = con(0x0);
  flag = false;

  while (rect2->y != con(0x0))
  {
    if (rect2->x < con(0xa) || rect2->x > con(0x12))
    {
      rect1 = SKW_QUERY_RECT(ddata.rectanglelist, rect2->y);
      if (rect1 == NULL)
        break;
      modey = rect2->y;
      deltax = rect1->w;
      deltay = rect1->h;
      if (rect1->x != con(0x1))
      {
        if (rect1->x != con(0x9))
        {
          if (rect1->x <= con(0x8))
            flag = true;
        }
        else
        {
          if (rect2->x <= con(0x8))
          {
            switch (rect2->x)
            {
              case 0:
                deltax = rect2->w - (rect1->w + 1) / 2;
                deltay = rect2->h - (rect1->h + 1) / 2;
              break;

              case 1:
                deltax = rect2->w;
                deltay = rect2->h;
              break;

              case 2:
                deltax = rect2->w - rect1->w + 1;
                deltay = rect2->h;
              break;

              case 3:
                deltax = rect2->w - rect1->w + 1;
                deltay = rect2->h - rect1->h + 1;
              break;

              case 4:
                deltax = rect2->w;
                deltay = rect2->h - rect1->h + 1;
              break;

              case 5:
                deltax = rect2->w - (rect1->w + 1) / 2;
                deltay = rect2->h;
              break;

              case 6:
                deltax = rect2->w - rect1->w + 1;
                deltay = rect2->h - (rect1->h + 1) / 2;
              break;

              case 7:
                deltax = rect2->w - (rect1->w + 1) / 2;
                deltay = rect2->h - rect1->h + 1;
              break;

              case 8:
                deltax = rect2->w;
                deltay = rect2->h - (rect1->h + 1) / 2;
              break;

              default: throw(THROW_DMABORT);
            }
          }

          if (flag)
          {
            flag = false;
            x0 += deltax;
            y0 += deltay;
            rc_0c.x += deltax;
            rc_0c.y += deltay;
          }

          if (deltax > rc_0c.x)
            rc_0c.x = deltax;
          if (rc_0c.w + rc_0c.x - 1 >= deltax + rect1->w)
            rc_0c.w = rect1->w - rc_0c.x + deltax;
          if (rc_0c.y < deltay)
            rc_0c.y = deltay;
          deltax = rect1->h + deltay;
          if (rc_0c.y + rc_0c.h - 1 >= deltax)
            rc_0c.h = deltay + rect1->h - rc_0c.y;
        }
      }
      else
      {
        x0 += deltax;
        y0 += deltay;
        rc_0c.x += deltax;
        rc_0c.y += deltay;
      }
    }
    else
    {
      c_rect* tmprect = SKW_QUERY_RECT(ddata.rectanglelist, rect2->y);
      if (tmprect == NULL) // BUGFIX Phase P1
        break;
      deltax = tmprect->w;
      deltay = tmprect->h;
      mode1st = tmprect->x;
      modey = tmprect->y;

      rect1 = SKW_QUERY_RECT(ddata.rectanglelist, modey);
      if (rect1 == NULL) // BUGFIX Phase P1
        break;

      switch (mode1st)
      {
        case 0:
          deltay -= (rect1->h + 1) / 2;
        case 5:
          deltax -= (rect1->w + 1) / 2;
        case 1:
        break;

        case 3:
          deltay -= rect1->h - 1;
        case 2:
          deltax -= rect1->w - 1;
        break;

        case 6:
          deltax -= rect1->w - 1;
        case 8:
          deltay -= (rect1->h + 1) / 2;
        break;

        case 7:
          deltax -= (rect1->w + 1) / 2;
        case 4:
          deltay -= rect1->h - 1;
        break;

        default:
        return NULL;
      }

      rc_0c.x += deltax;
      if (deltax > rc_0c.x)
        rc_0c.x = deltax;

      if (rect1->w + deltax <= rc_0c.x + rc_0c.w - 1)
        rc_0c.w = rect1->w - rc_0c.x + deltax;
      else
        rc_0c.w = rect1->w + deltax;

      rc_0c.y += deltay;
      if (rc_0c.y < deltay)
        rc_0c.y = deltay;
      if (deltay + rect1->h <= rc_0c.y + rc_0c.h - 1)
        rc_0c.h = deltay + rect1->h - rc_0c.y;

      switch (rect2->x - con(0xa))
      {
        case 0:
          deltay += (rect1->h + 1) / 2;
        case 5:
          deltax += (rect1->w + 1) / 2;
        break;

        case 3:
          deltay += rect1->h - 1;
        case 2:
          deltax += rect1->w - 1;
        break;

        case 6:
          deltax += rect1->w - 1;
        case 8:
          deltay += (rect1->h + 1) / 2;
        case 1:
        break;

        case 7:
          deltax += (rect1->w + 1) / 2;
        case 4:
          deltay += rect1->h - 1;
        break;

        default:
        return NULL;
      }

      deltax += rect2->w;
      x0 += deltax;
      y0 += deltay + rect2->h;
    }
    rect2 = rect1;
  } // loop

  x16 basex = *xaddptr;
  if (basex == con(0x0))
    //basex = getprevious(gfxdataarr)->w2;
	basex = *((ui16*)(gfxdataarr-6));	/// get the x dim

  x16 basey = *yaddptr;
  if (basey == con(0x0))
    //basey = getprevious(gfxdataarr)->w4;
	basey = *((ui16*)(gfxdataarr-4));	/// get the y dim

  switch (modex)
  {
    case 0:
      blitrect->x = x0 - (basex + 1) / 2;
      blitrect->y = y0 - (basey + 1) / 2;
    break;

    case 1:
      blitrect->x = x0;
      blitrect->y = y0;
    break;

    case 2:
      blitrect->x = x0 - basex + 1;
      blitrect->y = y0;
    break;

    case 3:
      blitrect->x = x0 - basex + 1;
      blitrect->y = y0 - basey + 1;
    break;

    case 4:
      blitrect->x = x0;
      blitrect->y = y0 - basey + 1;
    break;

    case 5:
      blitrect->x = x0 - (basex + 1) / 2;
      blitrect->y = y0;
    break;

    case 6:
      blitrect->x = x0 - basex + 1;
      blitrect->y = y0 - (basey + 1) / 2;
    break;

    case 7:
      blitrect->x = x0 - (basex + 1) / 2;
      blitrect->y = y0 - basey + 1;
    break;

    case 8:
      blitrect->x = x0;
      blitrect->y = y0 - (basey + 1) / 2;
    break;

    default:
    return NULL;
  }

  if (ddata.v1e01d0 != 0)
    SKW_COPY_RECT(&ddata.arect, &rc_0c);

  if (ddata.v1e01d8 && ddata.v1e025c != 0 && modey == 3)
    if (SKW_UNION_RECT(&rc_0c, xaddptr, yaddptr, &ddata.vx1d2712) == NULL)
      return NULL;

  x16 dx = rc_0c.x - blitrect->x;
  if (dx > con(0x0))
  {
    *xaddptr = dx;
    blitrect->x = rc_0c.x;
    blitrect->w = DM2_MIN(basex - dx, rc_0c.w);
  }
  else
  {
    *xaddptr = con(0x0);
    blitrect->w = DM2_MIN(basex, dx + rc_0c.w);
  }

  x16 dy = rc_0c.y - blitrect->y;
  if (dy > con(0x0))
  {
    *yaddptr = dy;
    blitrect->y = rc_0c.y;
    blitrect->h = DM2_MIN(basey - dy, rc_0c.h);
  }
  else
  {
    *yaddptr = con(0x0);
    blitrect->h = DM2_MIN(basey, dy + rc_0c.h);
  }

  if (blitrect->w <= con(0x0) || blitrect->h <= con(0x0))
    return NULL;
  return blitrect;
}

void SKW_TRIM_BLIT_RECT(x16 x, x16 y, x16 w, x16 h)
{
  SKW_SET_SRECT(&ddata.arect, x, y, ddata.vx1d270a - (x + w), ddata.vx1d270c - (y + h));
}

x16 SKW_ROTATE_5x5_POS(x16 a, t_rotate rotate)
{
  x16 tmp;

  x16 mod = a % 5 - 2;
  x16 div = a / 5 - 2;

  switch (rotate)
  {
    case 0:
    break;
    case 1:
      tmp = mod;
      mod = div;
      div = -tmp;
    break;
    case 2:
      mod = -mod;
      div = -div;
    break;
    case 3:
      tmp = mod;
      mod = -div;
      div = tmp;
    break;
  }

  return 5 * (div + 2) + (mod + 2);
}

x16 SKW_QUERY_CREATURE_BLIT_RECTI(x16 a, t_rotate rotate, x16 b)
{
  return SKW_ROTATE_5x5_POS(b, rotate) + a * con(0x19) + con(0x1388);
}

x16 SKW_098d_0cd7(x16 a, bool b, x16 c)
{
  if (!b)
    return a * con(0x19) + con(0xc1c) + c;
  return table1d27a0[a] + c;
}

void SKW_QUERY_TOPLEFT_OF_RECT(x16 n, x16* dx, x16* dy)
{
  c_rect r;

  x16 vw_08 = con(0x1);
  x16 vw_0c = con(0x1);

  SKW_QUERY_BLIT_RECT(NULL, n, &vw_0c, &r, &vw_08, con(0xffffffff));

  *dx = r.x;
  *dy = r.y;
}

c_rect* SKW_SCALE_RECT(x16 query, x16 scalew, x16 scaleh, c_rect* rs)
{
  x16 w;
  x16 h;

  c_rect* qrect1 = SKW_QUERY_RECT(ddata.rectanglelist, query);
  if (qrect1 == NULL || qrect1->y == con(0x0)) // BUGFIX Phase P1
    return NULL;
  c_rect* qrect2 = SKW_QUERY_RECT(ddata.rectanglelist, qrect1->y);
  if (qrect2 == NULL || qrect2->x != con(0x9))
    return NULL;

  if (scalew != con(0x2710))
    w = scalew * qrect2->w / con(0x2710);
  else
    w = qrect2->w;

  if (scaleh != con(0x2710))
    h = scaleh * qrect2->h / con(0x2710);
  else
    h = qrect2->h;

  if (w == con(0x0) && scalew != con(0x0))
    w = con(0x1);

  if (h == con(0x0) && scaleh != con(0x0))
    h = con(0x1);

  if (w <= con(0x0) || h <= con(0x0))
    return NULL;
  return SKW_QUERY_BLIT_RECT(NULL, query, &w, rs, &h, con(0xffffffff));
}

c_rect* SKW_QUERY_EXPANDED_RECT(x16 n, c_rect* r)
{
  return SKW_SCALE_RECT(signedlong(n), con(0x2710), con(0x2710), r);
}

// lots of pointeroperations here
// TODO: type of eaxp?
void SKW_COMPRESS_RECTS(x8* eaxpb, s_rnode** ebxp, FPTR_XP_Lb ecxf)
{
  s_rnode* p_04;
  x16* p_08;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_14;
  x16 vw_18;
  x16 vw_1c;

  u_vptr rg1p;
  u_vptr rg2p;

  rg1p.p8 = eaxpb;
  p_04 = UPCAST(s_rnode, ebxp); // adress of the first node pointer, is an s_rnode* too
  rg2p.p16 = rg1p.p16++;
  if (*rg2p.p16 != con(0xfffffc0d))
    return;
  vw_10 = *rg1p.p16++;
  p_08 = rg1p.p16;
  rg1p.p16 += 2 * vw_10;
  while (p_04->next != NULL)
    p_04 = p_04->next;

  while (--vw_10 >= con(0x0))
  {
    vw_14 = *p_08++;
    vw_1c = *p_08++;
    x16 loop1 = vw_1c - vw_14 + 1;
    x8* ptr7 = rg1p.p8;
    x8 byte4 = con(0x1f);
    vw_0c = *rg1p.p16;
    vw_18 = *(rg1p.p16 + 1);
    do
    {
      if (*rg1p.p16++ != vw_0c)
        byte4 &= con(0xfffffffd);
      if (*rg1p.p16 != vw_18)
        byte4 &= con(0xfffffffe);
      if (*rg1p.p16++ > con(0xff))
        byte4 &= con(0xfffffffb);
      for (x16 rg3w = 2; rg3w != con(0x0); rg3w--)
      {
        x16 rg2w = *rg1p.p16++;
        if (rg2w < 0 || rg2w > con(0xff))
          byte4 &= con(0xffffffef);
        if (rg2w < con(0xffffff80) || rg2w > con(0x7f))
          byte4 &= con(0xfffffff7);
      }
    } while (--loop1 > con(0x0));

    if ((byte4 & con(0x3)) != con(0x0))
      byte4 &= con(0xfffffffb);
    x16 loop2 = vw_1c - vw_14 + 1;
    x16 rg1w = SKW_CALC_SIZE_OF_COMPRESSED_RECT(byte4) * loop2 + con(0xa);
    if ((byte4 & con(0x1)) != con(0x0))
      rg1w += con(0x2);
    s_rnode* oldptr = p_04; // inserted!!!
    p_04 = UPCAST(s_rnode, INDCALL(ecxf)(signedlong(rg1w), false));
    // gets an uninitialized new data area

    oldptr->next = p_04; // that should fix it!
    p_04->next = NULL;
    p_04->lo = vw_14;
    p_04->hi = vw_1c;
    p_04->mask = byte4;
    p_04->x = CUTX8(vw_0c);

    rg2p.p8 = DOWNCAST(xxx, p_04) + con(0xa); // points to the rnode's datas then
    if ((byte4 & con(0x1)) != con(0x0))
      *rg2p.p16++ = vw_18;

    rg1p.p8 = ptr7;
    do
    {
      if ((byte4 & con(0x4)) == con(0x0))
      {
        if ((byte4 & con(0x2)) == con(0x0))
          *rg2p.p16++ = *rg1p.p16;
        rg1p.p16++;
        if ((byte4 & con(0x1)) == con(0x0))
          *rg2p.p16++ = *rg1p.p16;
        rg1p.p16++;
      }
      else
      {
        *rg2p.p8++ = *rg1p.p8++;
        rg1p.p8++;
        *rg2p.p8++ = *rg1p.p8++;
        rg1p.p8++;
      }

      if ((byte4 & con(0x18)) == con(0x0))
      {
        *rg2p.p16++ = *rg1p.p16++;
        *rg2p.p16++ = *rg1p.p16++;
      }
      else
      {
        *rg2p.p8++ = *rg1p.p8++;
        rg1p.p8++;
        *rg2p.p8++ = *rg1p.p8++;
        rg1p.p8++;
      }
    } while (--loop2 > con(0x0));
  }
}

c_rect* SKW_OFFSET_RECT(c_c5rects* rc, c_rect* rd, c_rect* rs)
{
  rd->x = rs->x - rc->s_00.rc_02.x;
  rd->y = rs->y - rc->s_00.rc_02.y;
  rd->w = rs->w;
  rd->h = rs->h;
  return rd;
}

// TODO next function needs:
void SKW_0b36_0cbe(c_c5rects* eaxps, bool edxbool);

// TODO: move into an extra-file like c_gfx_rect
// (depends on graphics while all other funcs here
// work on an abstract rectangle)
void SKW_0b36_0d67(c_c5rects* eaxps, c_rect* edxr)
{
  c_c5rects* esips;
  x32 vl_00;

  esips = eaxps;
  c_rect* rptrrg31 = edxr;
  if (edxr == NULL)
    return;

  x16 wordrg4 = con(0x0);
  for(;;)
  {
    if (wordrg4 < esips->s_00.w_0a)
    {
      c_rect* rptrrg4 = &esips->rcarr_0c[wordrg4];
      if (rptrrg4->x <= rptrrg31->x)
      {
        vl_00 = signedlong(rptrrg4->x) + signedlong(rptrrg4->w);
        if ((vl_00 - 1) >= (signedlong(rptrrg31->w) + signedlong(rptrrg31->x) - 1))
        {
          if (rptrrg4->y <= rptrrg31->y)
          {
            x32 longrg23 = signedlong(rptrrg4->y) + signedlong(rptrrg4->h) - 1;
            if (longrg23 >= (signedlong(rptrrg31->y) + signedlong(rptrrg31->h) - 1))
              return;
          }
        }
      }

      c_rect* rptrrg41 = &esips->rcarr_0c[wordrg4];
      if (rptrrg41->x >= rptrrg31->x)
      {
        vl_00 = signedlong(rptrrg31->w);
        if ((signedlong(rptrrg41->w) + signedlong(rptrrg41->x) - 1) <= (signedlong(rptrrg31->x) + vl_00 - 1) && rptrrg41->y >= rptrrg31->y)
        {
          x32 longrg21 = signedlong(rptrrg41->y) + signedlong(rptrrg41->h) - 1;
          if (longrg21 <= (signedlong(rptrrg31->y) + signedlong(rptrrg31->h) - 1))
            break;
        }
      }
    }
    else
    {
      if (wordrg4 >= con(0x5))
        SKW_0b36_0cbe(esips, false);
      wordrg4 = esips->s_00.w_0a;
      esips->s_00.w_0a = wordrg4 + 1;
      break;
    }
    wordrg4++;
  }

  c_rect* rectptrrg6 = &esips->rcarr_0c[wordrg4];

  SKW_COPY_RECT(rptrrg31, rectptrrg6);
  c_rect* rptrrg32 = rectptrrg6;

  x16 wordrg41 = rectptrrg6->x - esips->s_00.rc_02.x;
  if (wordrg41 < con(0x0))
  {
    x16 wordrg21 = rectptrrg6->w + wordrg41;
    rectptrrg6->w = wordrg21;
    if (wordrg21 <= con(0x0))
    {
      esips->s_00.w_0a--;
      return;
    }
    rectptrrg6->x -= wordrg41;
  }

  x16 wordrg42 = rptrrg32->y - esips->s_00.rc_02.y;
  if (wordrg42 < con(0x0))
  {
    rptrrg32->h += wordrg42;
    if (rptrrg32->h <= con(0x0))
    {
      esips->s_00.w_0a--;
      return;
    }
    rptrrg32->y -= wordrg42;
  }
  x16 wordrg2 = esips->s_00.rc_02.x + esips->s_00.rc_02.w - 1 - (rptrrg32->x + rptrrg32->w - 1);
  if (wordrg2 < con(0x0))
    rptrrg32->w += wordrg2;

  x16 wordrg43 = esips->s_00.rc_02.h + esips->s_00.rc_02.y - 1 - (rptrrg32->y + rptrrg32->h - 1);
  if (wordrg43 < con(0x0))
    rptrrg32->h += wordrg43;
}
