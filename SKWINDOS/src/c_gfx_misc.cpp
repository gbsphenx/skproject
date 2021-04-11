#include "../driver.h"
#include "util.h"
#include "dm2data.h"
#include "c_mouse.h"
#include "c_dballoc.h"
#include "c_gfx_fill.h"
#include "c_gfx_blit.h"
#include "c_gfx_misc.h"

void SKW_INIT_BACKBUFF(void)
{
  if (ddata.backbuff1 != con(0x0))
    SKW_FIRE_FILL_BACKBUFF_RECT(&ddata.backbuffrect, ddata.paletteptr1[0]);
  s_gfxdatatail* ptrbefore = getprevious(ddata.bitmapptr);
  ptrbefore->w2 = ddata.vx1d270a;
  ptrbefore->w4 = ddata.vx1d270c;
  ptrbefore->b0 = con(0x8);
  ddata.backbuff2 = con(0x0);
}

static void SKW_COPY_SKIMAGE_HEADER(t_gfxdata* eaxpg, t_gfxdata* edxpg)
{
  s_gfxdatatail* a = getprevious(eaxpg);
  s_gfxdatatail* d = getprevious(edxpg);
  SKW_COPY_MEMORY(DOWNCAST(s_gfxdatatail, a), con(0x6), DOWNCAST(s_gfxdatatail, d));
}

// srcbmp = eax
// destbmp = edx
static void SKW_APPLY_MIRROR_FLIP_TO(t_gfxdata* srcbmp, x16 ebxw, t_gfxdata* destbmp)
{
  x16 wordrg3 = getprevious(srcbmp)->w2;
  x16 wordrg1 = getprevious(srcbmp)->w4;
  e_bitres res = e_bitres(getprevious(srcbmp)->b0);
  SKW_COPY_SKIMAGE_HEADER(srcbmp, destbmp);
  c_rect* r1 = SKW_ALLOC_TEMP_ORIGIN_RECT(wordrg3, wordrg1);
//                      srcbmp   srcofs destbmp                                            srcbits destbits
  SKW_FIRE_BLIT_PICTURE(srcbmp, r1, 0, destbmp, con(0x0), wordrg3, wordrg3, con(0xffff), ebxw, res, res);
}

void SKW_DRAW_MIRRORED_PIC(t_gfxdata* eaxpg, t_gfxdata* edxpg)
{
//                         srcbmp
  SKW_APPLY_MIRROR_FLIP_TO(eaxpg, con(0x1), edxpg);
}

void SKW_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN(void)
{
  c_rect rc_00;
  x16 vw_08;
  x16 vw_0c;

  if (!ddata.dialog2)
    SKW_CHANGE_VIEWPORT_TO_INVENTORY(false);
  else
  {
    SKW_FIRE_HIDE_MOUSE_CURSOR();
    vw_08 = con(0x0);
    vw_0c = con(0x0);
    if (SKW_QUERY_BLIT_RECT(ddata.bitmapptr, con(0x5), &vw_08, &rc_00, &vw_0c, con(0xffffffff)))
//                            srcbmp                 srcofs  destbmp                                                                                                               srcbits                  destbits
      SKW_FIRE_BLIT_PICTURE(ddata.bitmapptr, &rc_00, vw_08, ddata.screenptr, vw_0c, getprevious(ddata.bitmapptr)->w2, ddata.mouse_unk0c, con(0xffff), con(0x0), e_bitres(getprevious(ddata.bitmapptr)->b0), E_BITRES8);
    SKW_FIRE_SHOW_MOUSE_CURSOR();
  }
}

// was SKW_44c8_1be8
// TODO: setting of eaxbool is obsolete, but we keep it
void SKW_CHANGE_VIEWPORT_TO_INVENTORY(bool eaxbool)
{
  c_rect rc_00;
  i16* iface = UPCAST(i16, ddata.drvinterfaceptr);

  x16 wordrg3 = ddata.v1e025c;
  ddata.v1e025c = 0;
  SKW_QUERY_EXPANDED_RECT(con(0x7), &rc_00);
  ddata.v1e025c = wordrg3;
  bool boolrg5 = false; // TODO: default value wasn't set
  if (!ddata.v1e0080)
  {
    iface[0] = con(0x1);
    userinterrupt(con(0xe), con(0x0));
    x16 wordrg5 = iface[1];
    x16 wordrg2 = iface[2];
    if (signedlong(iface[0]) > signedlong(rc_00.w) + signedlong(rc_00.x) - 1)
      boolrg5 = false;
    else
    {
      x32 longrg4 = signedlong(wordrg5);
      if (longrg4 > signedlong(rc_00.y) + (signedlong(rc_00.h)) - 1)
        boolrg5 = false;
      else
      {
        longrg4 += signedlong(wordrg2);
        if (longrg4 >= signedlong(rc_00.y))
        {
          SKW_FIRE_HIDE_MOUSE_CURSOR();
          boolrg5 = true;
        }
        else
         boolrg5 = false;
      }
    }
  }

  bool boolrg3;
  if (wordrg3 == con(0x0) || !ddata.v1e01d8)
    boolrg3 = false;
  else
    boolrg3 = true;
  SKW_COPY_MEMORY(DOWNCAST(c_rect, &rc_00), con(0x8), DOWNCAST(i8, ddata.drvinterfaceptr));
  x16 wordrg1;
  if (!boolrg3)
    wordrg1 = con(0x8);
  else
    wordrg1 = con(0x8008);
  iface[4] = wordrg1;
  userinterrupt(con(0x109), con(0x0));
  if (!ddata.v1e0080 && boolrg5)
    SKW_FIRE_SHOW_MOUSE_CURSOR();
}

void SKW_DRAW_DIALOGUE_PROGRESS(x32 eaxl)
{
  c_rect rc_00;

  if (dm2_dballochandler.v1e0200)
  {
    SKW_QUERY_EXPANDED_RECT(con(0x1da), &rc_00);
    x16 wordrg1 = signedlong(rc_00.w) * eaxl / con(0x3e8);
    rc_00.w = wordrg1;
    if (wordrg1 > 0 && wordrg1 != ddata.dialog1)
    {
      ddata.dialog1 = wordrg1;
      SKW_FIRE_FILL_BACKBUFF_RECT(&rc_00, ddata.paletteptr1[con(0x9)]);
      SKW_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
    }
  }
}

// srcbmp = eax
// palette = ecx
void SKW_DRAW_DIALOGUE_PARTS_PICT(t_gfxdata* srcbmp, x16 ebxw, t_palette* palette, c_rect* edxr)
{
	if (srcbmp == NULL || palette == NULL || edxr == NULL) // SPX: avoid null pointers
		return;
//                      srcbmp     srcofs destbmp                                                                                                               srcbits          destbits  palette
  SKW_FIRE_BLIT_PICTURE(srcbmp, edxr, 0, ddata.bitmapptr, con(0x0), getprevious(srcbmp)->w2, getprevious(ddata.bitmapptr)->w2, ebxw, con(0x0), e_bitres(getprevious(srcbmp)->b0), E_BITRES8, palette);
}

// srcbmp = eax
// srcofs = ecx
// destbmp = edx
// palette = argp2
void SKW_DRAW_DIALOGUE_PICT(t_gfxdata* srcbmp, c_rect* ebxr, x16 srcofs, t_gfxdata* destbmp, x16 argw0, x16 argw1, t_palette* palette)
{
//                      srcbmp        srcofs  destbmp                                                                                  srcbits                           destbits                    palette
  SKW_FIRE_BLIT_PICTURE(srcbmp, ebxr, srcofs, destbmp, argw0, getprevious(srcbmp)->w2, getprevious(destbmp)->w2, argw1, con(0x0), e_bitres(getprevious(srcbmp)->b0), e_bitres(getprevious(destbmp)->b0), palette);
}
