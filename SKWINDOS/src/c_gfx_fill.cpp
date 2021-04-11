#include "../driver.h" // driver_userint
#include "dm2data.h"
#include "util.h"
#include "c_gfx_fill.h"

// needs:
t_gfxdata* SKW_3e74_5817(x16 eaxw);

void SKW_FIRE_FILL_4BPP_PICT_LINE(x16 eaxw, x16 ebxw, t_gfxdata edxb)
{
  t_gfxdata vb_00;
  x8 vb_04;

  vb_00 = edxb;
  x16 wordrg1 = eaxw & con(0x1);
  eaxw /= 2;
  if (wordrg1 != con(0x0))
  {
    t_gfxdata* ptrrg4 = ddata.destbmp + unsignedlong(eaxw);
    *ptrrg4 = (*ptrrg4 & con(0xfffffff0)) | (vb_00 & con(0xf));
    eaxw++;
    ebxw--;
  }
  t_gfxdata byterg11 = vb_00 << con(0x4);
  byterg11 |= vb_00 & con(0xf);
  vb_04 = byterg11;
  while (ebxw >= con(0x2))
  {
    ddata.destbmp[eaxw++] = vb_04;
    ebxw -= con(0x2);
  }
  if (ebxw > con(0x0))
  {
    x32 longrg3 = unsignedlong(eaxw);
    ddata.destbmp[longrg3] = (ddata.destbmp[longrg3] & con(0xf)) | (vb_00 << con(0x4));
  }
}

void SKW_FIRE_FILL_8BPP_PICT_LINE(x16 lineofs, x16 n, x8 pixel256)
{
  t_gfxdata* dest = ddata.destbmp + unsignedlong(lineofs);
  if ((I32CAST(dest) & con(0x1)) != con(0x0))
  {
    *dest++ = pixel256;
    n--;
  }
  x16 w = n / 2; // in words
  if (w != con(0x0))
  {
    i16* wdest = UPCAST(x16, dest);
    ui16 tmp = (pixel256 << 8) | (pixel256 & 0xff);
    while (w--)
      *wdest++ = tmp;
    dest = UPCAST(t_gfxdata, wdest);
  }
  n &= con(0x1);
  if (n != con(0x0))
    *dest = pixel256;
}

static void SKW_FIRE_FILL_RECT_8BPP_PICT(t_gfxdata* eaxpg, x8 pixel256, x16 width, c_rect* edxr)
{
  ddata.destbmp = UPCAST(t_gfxdata, DOWNCAST(t_gfxdata, eaxpg)); // pair of (u)bytes
  if (edxr == NULL) // SPX: avoid null pointer
	return;
  x16 lineofs = edxr->y * width + edxr->x;

  for (x16 lines = 0; lines < edxr->h; lines++, lineofs += width)
    SKW_FIRE_FILL_8BPP_PICT_LINE(lineofs, edxr->w, pixel256);
}

void SKW_FIRE_FILL_RECT_4BPP_PICT(t_gfxdata* eaxpg, x8 ebxb, x16 ecxw, c_rect* edxr)
{
  x8 vb_00;
  x32 vl_04;

  vb_00 = ebxb;
  vl_04 = lextended(ecxw);
  ddata.destbmp = eaxpg;
  x16 wordrg1 = (ecxw + 1) & con(0xfffffffe);
  vl_04 = lextended(wordrg1);
  x16 wordrg3 = edxr->y * wordrg1 + edxr->x;
  x16 wordrg6 = con(0x0);

  for(;;)
  {
    if (unsignedlong(wordrg6) >= signedlong(edxr->h))
      return;
    SKW_FIRE_FILL_4BPP_PICT_LINE(wordrg3, edxr->w, t_gfxdata(vb_00));
    wordrg3 += CUTX16(vl_04);
    wordrg6++;
  }
}

void SKW_FIRE_FILL_RECT_ANY(t_gfxdata* eaxpg, x8 pixel256, x16 ecxw, c_rect* edxr, x16 argw0)
{
  if (eaxpg == ddata.screenptr)
  {
    ecxw = ORIG_SWIDTH;
    enable_video = false;
    while (!enable_drawing);
  }
  if (argw0 == con(0x4))
    SKW_FIRE_FILL_RECT_4BPP_PICT(UPCAST(t_gfxdata, eaxpg), pixel256, ecxw, edxr);
  else
    SKW_FIRE_FILL_RECT_8BPP_PICT(eaxpg, pixel256, ecxw, edxr);
}

static void SKW_FIRE_FILL_HALFTONE_RECTV(c_rect* eaxr, x16 edxw)
{
  SKW_COPY_RECT(eaxr, UPCAST(c_rect, ddata.drvinterfaceptr));
  userinterrupt(con(0x103), edxw);
}

void SKW_FILL_ENTIRE_PICT(t_gfxdata* eaxpg, x8 pixel256)
{
  c_rect rc_00;

  t_gfxdata* ptrrg5 = eaxpg;
  x16 wordrg41 = unsignedword(getprevious(ptrrg5)->b0);
  rc_00.y = con(0x0);
  rc_00.x = con(0x0);
  x32 longrg6 = signedlong(getprevious(ptrrg5)->w2) + (wordrg41 == con(0x4) ? 1 : 0);
  rc_00.w = CUTX16((~(wordrg41 == con(0x4) ? 1 : 0)) & longrg6);
  rc_00.h = getprevious(ptrrg5)->w4;
  SKW_FIRE_FILL_RECT_ANY(eaxpg, pixel256, rc_00.w, &rc_00, wordrg41);
}

void SKW_FIRE_FILL_BACKBUFF_RECT(c_rect* eaxr, x8 pixel256)
{
  SKW_FIRE_FILL_RECT_ANY(ddata.bitmapptr, pixel256, ddata.vx1d270a, eaxr, con(0x8));
}

void SKW_FIRE_FILL_SCREEN_RECT(x16 eaxw, x8 pixel256)
{
  c_rect rc_00;

  c_rect* r = SKW_QUERY_EXPANDED_RECT(eaxw, &rc_00);
  SKW_FIRE_FILL_RECT_ANY(ddata.screenptr, pixel256, rc_00.w, r, con(0x8));
}

void SKW_FIRE_FILL_HALFTONE_RECTI(x16 eaxw, x16 edxw)
{
  c_rect rc_00;

  SKW_FIRE_FILL_HALFTONE_RECTV(SKW_QUERY_EXPANDED_RECT(eaxw, &rc_00), edxw);
}

void SKW_44c8_1dfc(c_rect* eaxr, x8 pixel256)
{
  SKW_FIRE_FILL_RECT_ANY(ddata.screenptr, pixel256, ORIG_SWIDTH, eaxr, con(0x8));
}

void SKW_FILL_RECT_SUMMARY(c_c5rects* eaxps, x8 pixel256, c_rect* edxr)
{
  c_c5rects* esips;
  c_rect rc_00;

  esips = eaxps;
  if (edxr != NULL)
  {
    SKW_FIRE_FILL_RECT_ANY(SKW_3e74_5817(esips->s_00.w_00), pixel256, esips->s_00.rc_02.w, SKW_OFFSET_RECT(esips, &rc_00, edxr), con(0x8));
    SKW_0b36_0d67(esips, edxr);
  }
}
