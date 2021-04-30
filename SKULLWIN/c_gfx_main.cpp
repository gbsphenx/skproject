#include "util.h"
#include "c_tmouse.h"
#include "c_buttons.h"
#include "dm2data.h"
#include "c_gfx_decode.h"
#include "c_dballoc.h"
#include "c_querydb.h"
#include "c_timer.h"
#include "c_image.h"
#include "c_gfx_blit.h"
#include "c_gfx_pal.h"
#include "c_gfx_str.h"
#include "c_xrect.h"
#include "c_tmouse.h"
#include "c_gfx_main.h"

c_gfx_system gfxsys;

void c_gfx_system::init(void)
{
  bitmapptr = NULL;
  backbuff1 = 0;
  backbuff2 = 0;
  backbuffrect.init();
  backbuffer_w = 0xe0;
  backbuffer_h = 0x88;
  pictbuff = NULL;

  for (i32 i = 0; i < ORIG_SHEIGHT * ORIG_SWIDTH; i++)
    dm2screen[i] = E_COL00;

  blitter.init();
  paldat.init();
  imgdecoder.init();
}

void DM2_INIT_BACKBUFF(void)
{
  if (gfxsys.backbuff1 != 0)
    DM2_FILL_BACKBUFF_RECT(&gfxsys.backbuffrect, palettecolor_to_pixel(paldat.palette[E_COL00]));
  s_dm2bmpheader* bmpheader = getbmpheader(gfxsys.bitmapptr);
  bmpheader->width = gfxsys.backbuffer_w;
  bmpheader->height = gfxsys.backbuffer_h;
  bmpheader->res = BPP_8;
  gfxsys.backbuff2 = 0;
}

void DM2_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN(void)
{
  if (ddat.dialog2 == 0)
    DM2_DRAWINGS_COMPLETED();
  else
  {
    c_rect blitrect;
    i16 srcx = 0;
    i16 srcy = 0;

    DM2_HIDE_MOUSE();

    if (DM2_QUERY_BLIT_RECT(gfxsys.bitmapptr, &blitrect, 5, IO srcx, IO srcy))
    {
      s_dm2bmpheader* bmpheader = getbmpheader(gfxsys.bitmapptr);

      blit_toscreen(
        PIXELCAST(gfxsys.bitmapptr),
        &blitrect,
        srcx,
        srcy,
        bmpheader->width,
        NOALPHA,
        BLITMODE0,
        bmpheader->res,
        BPP_8);
    }
    DM2_SHOW_MOUSE();
  }
}

void DM2_gfxmain_3929_0914(void)
{
  if (ddat.v1e141e == 2)
  {
    if (ddat.v1e1420 != 0)
      DM2_SCROLLBOX_MESSAGE();
    DM2_FILL_ENTIRE_PICT(gfxsys.pictbuff, palettecolor_to_pixel(paldat.palette[E_COL00]));
    ddat.v1e1420 = 1;
    //m_45942:
    for (i16 i=0; i < 2; i++)
      //m_45933:
      ddat.v1e1408[i] = ddat.v1e1408[i + 1];
    ddat.v1e1408[2] = lcon(0xffffffff);
  }
  else
  {
    //m_45959:
    ddat.v1e1420 = 0;
    ddat.v1e141e++;
  }
}

void DM2_gfxmain_3929_0929(i16 eaxw, t_text* edxtp)
{
  c_rect rc_00;

  DM2_QUERY_EXPANDED_RECT(15, &rc_00);
  if (ddat.v1e1420 == 0)
  {
    //m_459D1:
    DM2_HIDE_MOUSE();
    i16 n = ddat.v1e141e * strdat.strxplus + rc_00.y + strdat.gfxstrw1 - strdat.gfxstrw3 - 1 + ddat.v1e141c; // adj
    DM2_PRINT_SYSERR_TEXT(rc_00.x + ddat.v1d70ea, n, eaxw, unsignedword(palettecolor_to_ui8(paldat.palette[E_COL00])), edxtp);
    DM2_SHOW_MOUSE();
  }
  else
    DM2_DRAW_STRING(PIXELCAST(gfxsys.pictbuff), ddat.v1d2744, ddat.v1d70ea, strdat.strxplus - strdat.gfxstrw3 - 2, eaxw, unsignedword(palettecolor_to_ui8(paldat.palette[E_COL00])), edxtp);
  //m_45A31:
  ddat.v1e1408[ddat.v1e141e] = timdat.gametick + 90;
}

void DM2_gfxmain_0b36_0cbe(c_buttongroup* buttongroup, bool alloc)
{
  if (buttongroup->button.groupsize != 0)
  {
    c_rect* r = &buttongroup->rects[0];
    DM2_HIDE_MOUSE();
    do
    {
      blit_toscreen(
        PIXELCAST(dm2_dballochandler.DM2_GET_BMP(buttongroup->button.dbidx)),
        r,
        r->x - buttongroup->button.r.x,
        r->y - buttongroup->button.r.y,
        buttongroup->button.r.w,
        NOALPHA,
        BLITMODE0,
        BPP_8,
        BPP_8);

      r++;
    } while (--buttongroup->button.groupsize != 0);
    DM2_SHOW_MOUSE();
  }

  if (alloc)
  {
    dm2_dballochandler.DM2_dballoc_3e74_58a3(buttongroup->button.dbidx);
    buttongroup->button.dbidx = NODATA;
  }
}

// ################# fill-functions #################

void DM2_FILL_BACKBUFF_RECT(c_rect* blitrect, c_pixel pixel)
{
  // assuming gfxsys.bitmapptr doesn't equal gfxsys.dm2screen
  blitter.fill(gfxsys.bitmapptr, pixel, gfxsys.backbuffer_w, blitrect, BPP_8);
}

void DM2_FILL_SCREEN_RECT(i16 query, c_pixel pixel)
{
  c_rect r;

  // third arg overwritten because of destination
  blitter.fill(gfxsys.dm2screen, pixel, ORIG_SWIDTH, DM2_QUERY_EXPANDED_RECT(query, &r), BPP_8);
}

// was DM2_44c8_1dfc TODO tell SPX new name
void DM2_FILL_FULLSCREEN(c_rect* blitrect, c_pixel pixel)
{
  blitter.fill(gfxsys.dm2screen, pixel, ORIG_SWIDTH, blitrect, BPP_8);
}

void DM2_FILL_ENTIRE_PICT(t_bmp* bmp, c_pixel pix)
{
  c_rect blitrect; //autoinit!

  s_dm2bmpheader* bmpheader = getbmpheader(bmp);
  t_resolution res = bmpheader->res;
  i16 width = bmpheader->width + (res == BPP_4 ? 1 : 0);
  blitrect.w = (res == BPP_4 ? 0xfffe : 0xffff) & width;
  blitrect.h = bmpheader->height;
  ui16 pixels;
  if (bmp == gfxsys.dm2screen)
    pixels = ORIG_SWIDTH;
  else
    pixels = mkuw(blitrect.w);
  blitter.fill(PIXELCAST(bmp), pix, pixels, &blitrect, res);
}

// placed the code of driver_showscreenrectdisabled (driver::sub_22FC) here
// grey out a rectangle in screen
static void DM2_FILL_HALFTONE_RECTV(c_rect* r)
{
  for (i16 y = r->y; y < r->y + r->h - 1; y++)
  {
    c_pixel* bptr = gfxsys.dm2screen + y * ORIG_SWIDTH + r->x;

    for (i16 x = r->x; x < r->x + r->w - 1; x++, bptr++)
      if (!IS_ODD(x ^ y))
        *bptr = c_pixel(E_COL00);
  }
}

void DM2_FILL_HALFTONE_RECTI(i16 query)
{
  c_rect r;

  DM2_FILL_HALFTONE_RECTV(DM2_QUERY_EXPANDED_RECT(query, &r));
}

// request of ddat.dialog2 pulled out
void DM2_FADE_SCREEN(i16 mode)
{
  if (mode == 0)
    DM2_SELECT_PALETTE_SET(1);
  else
  {
    DM2_SELECT_PALETTE_SET(0);
    DM2_FILL_SCREEN_RECT(2, c_pixel(E_COL00));
  }
}

// function added
void blit_toscreen(c_pixel* srcgfx, c_rect* blitrect, i16 srcx, i16 srcy, ui16 srcw, t_alphamask alphamask, e_blitmode blitmode, t_resolution sbpp, t_resolution dbpp, t_palette* palette)
{
  disable_video++;

  blitter.blit(
    srcgfx,
    gfxsys.dm2screen,
    blitrect,
    srcx,
    srcy,
    srcw,
    ORIG_SWIDTH,
    alphamask,
    blitmode,
    sbpp,
    dbpp,
    palette);

  if (disable_video > 0)
    disable_video--;
}

#define STRETCH_DELTAX (0x67)
#define STRETCH_START  (0x3200)
#define STRETCH_END    (0xdc00)

// was sub_29AD
// stretch - effect if MSB of 'stretched' is set
// final T1_execmousefifo pulled out
// 'ab'using the mouse's blitroutinen.
void _specialblit(c_pixel256* dest, c_pixel256* srcmap, c_rect* r, i16 stretched)
{
  bool stretch = (stretched & 0x8000) != 0;

  Tmouse.driver_blockmouseinput();

  if (stretch)
  {
    for (ui16 us = mkuw(STRETCH_START), ud = 0; us < mkuw(STRETCH_END); us += mkuw(ORIG_SWIDTH), ud += mkuw(STRETCH_DELTAX))
      Tmouse.mblitter.blit_hline_stretched(srcmap, dest, ((ud >> 7) + 8) * 0xe0 + 0x15, us);
  }
  else
    Tmouse.mblitter.blit(srcmap, dest, r, 0, 0, r->w, ORIG_SWIDTH, NOMASK, NOMASK);

  Tmouse.driver_unblockmouseinput();
}

// was SKW_44c8_1be8
// was DM2_CHANGE_VIEWPORT_TO_INVENTORY, TODO tell SPX new name
void DM2_DRAWINGS_COMPLETED(void)
{
  c_rect rc_00;

  i16 wordrg3 = ddat.v1e025c;
  ddat.v1e025c = 0;
  DM2_QUERY_EXPANDED_RECT(7, &rc_00);
  ddat.v1e025c = wordrg3;
  _specialblit(gfxsys.dm2screen, DRV_screen256.pixel, &rc_00, (wordrg3 != 0 && ddat.v1e01d8) ? 0x8008 : 0x8);
  //send_command(7);
}
