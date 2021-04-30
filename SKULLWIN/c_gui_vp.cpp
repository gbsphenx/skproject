#include "dm2data.h"
#include "startend.h"
#include "util.h"
#include "c_image.h"
#include "c_str.h"
#include "c_map.h"
#include "c_querydb.h"
#include "c_creature.h"
#include "c_dballoc.h"
#include "c_gdatfile.h"
#include "c_gfx_main.h"
#include "c_gfx_blit.h"
#include "c_tmouse.h"
#include "c_gui_draw.h"
#include "c_gfx_str.h"
#include "c_gfx_pal.h"
#include "c_eventqueue.h"
#include "c_xrect.h"
#include "c_random.h"
#include "c_timer.h"
#include "dm2global.h"
#include "c_input.h"
#include "c_gui_vp.h"

// TODO optimize temporaries
#include "regs.h"
#include "emu.h"

// this table is for DM2_guivp_098d_0cd7

const i16 table1d27a0[16] = {
 -1,     0x1149, 0x1162, -1, 0x1194, 0x11ad, -1,     0x11df,
 0x11f8, 0x1211, 0x122a, -1, 0x125c, 0x1275, 0x128e, 0x12a7
};

// TODO: optimize temporaries
#include "regs.h"
#include "emu.h"

// The procedure SKW_COPY_SKIMAGE_HEADER is just used once, and a one-line too,
// so I replaced it directly

// The procedure SKW_APPLY_MIRROR_FLIP_TO is just used once,
// so I replaced it directly

// This whole code was SKW_APPLY_MIRROR_FLIP_TO (with blitmode variable)
static void DM2_DRAW_MIRRORED_PIC(t_bmp* srcbmp, t_bmp* destbmp)
{
  s_dm2bmpheader* sheader = getbmpheader(srcbmp);
  s_dm2bmpheader* dheader = getbmpheader(destbmp);

  // the next line was SKW_COPY_SKIMAGE_HEADER
  DM2_COPY_BMPHEADER(dheader, sheader);

  i16 w = sheader->width;
  i16 h = sheader->height;

  ui16 destw;
  if (PIXELCAST(destbmp) == gfxsys.dm2screen)
    destw = ORIG_SWIDTH;
  else
    destw = w;

  blitter.blit(
    PIXELCAST(srcbmp),
    PIXELCAST(destbmp),
    tmprects.alloc_origin_tmprect(w, h),
    0,
    0,
    w,
    destw,
    NOALPHA,
    BLITMODE1,
    sheader->res,
    sheader->res);
}

// SKW_32cb_59ca
static i32 DM2_SET_GRAPHICS_FLIP_FROM_POSITION(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;

  RG5L = RG1L;
  RG2L = signedlong(RG2W);
  RG1L = signedlong(RG3W) + RG2L;
  RG3L = signedlong(RG4W) + RG1L;
  RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
  RG3L += unsignedlong(byte_at(RG1P, lcon(0x6)));
  RG2L = unsignedlong(byte_at(RG1P, lcon(0x7))) + RG3L;
  RG1W = word_at(RG1P, lcon(0x8)) & lcon(0x3f);
  RG1L = unsignedlong(RG1W) + RG2L;
  RG1L &= 0x1;
  if (RG5UW < mkuw(1))
    return signedlong(RG1W);
  RG2L = timdat.gametick & lcon(0x7);
  RG4W = ddat.v1e0258 & 1;
  if (RG5UW <= mkuw(1))
  {
    //m_44988:
    RG3W = ddat.v1e12d6 & lcon(0x8);
    RG3L = unsignedlong(RG3W);
    if (RG3L == 0)
    {
      //m_449C2:
      RG1W = ddat.v1e12d6 & lcon(0x40);
      RG1L = unsignedlong(RG1W);
      return RG1L == 0 ? RG1L : signedlong(RG4W);
    }
    RG4W = ddat.v1e12d6 & lcon(0x10);
    RG3L = unsignedlong(RG4W);
    if (RG3L == 0)
      return signedlong(RG1W);
    RG1Blo = mkul(RG2L) > mkul(lcon(0x3)) ? 1 : 0;
  }
  else
  {
    if (RG5W != lcon(0x20))
      return signedlong(RG1W);
    //m_449D8:
    RG3W = ddat.v1e12d6 & 2;
    RG3L = unsignedlong(RG3W);
    if (RG3L == 0)
    {
      //m_44A19:
      RG1W = ddat.v1e12d6 & lcon(0x20);
      RG1L = unsignedlong(RG1W);
      return RG1L == 0 ? RG1L : signedlong(RG4W);
    }
    RG4W = ddat.v1e12d6 & lcon(0x4);
    RG3L = unsignedlong(RG4W);
    if (RG3L == 0)
      //m_44A11:
      RG1Blo = RG1W == wcon(0x0) ? 1 : 0;
    else
    {
      RG1Blo = mkul(RG2L) <= mkul(lcon(0x3)) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      RG1Blo = RG1L != 0 ? 1 : 0;
    }
  }
  //m_449BB:
  RG1L = unsignedlong(RG1Blo);
  return RG1L;
}

static i16 DM2_guivp_098d_0cd7(i16 wa, i16 wc, bool vbool)
{
  if (!vbool)
    return 25 * wa + 3100 + wc;
  return table1d27a0[wa] + wc;
}

// SKW_32cb_2367
static void DM2_DRAW_PIT_ROOF(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  i32 RG6l;
  i16 vw_00;
  i16 vw_04;
  i16 vw_08;
  i8 vb_0c;
  i16 parw01;

  RG5w = RG1W;
  RG1Blo = ddat.v1d6c02;
  vb_0c = RG1Blo;
  vw_08 = ddat.v1d6c00;
  RG6l = signedlong(RG5w);
  if (RG6l > lcon(0x8))
    return;
  RG1W = ddat.v1e12d6 & 1;
  RG1L = unsignedlong(RG1W);
  if (RG1L == 0)
    return;
  RG1L = 18 * RG6l;
  RG4P = ddat.ptr1e1044 + RG1L;
  RG1W = signedword(byte_at(RG4P));
  vw_00 = RG1W;
  RG1W = signedword(byte_at(RG4P, 1));
  vw_04 = RG1W;
  RG1L = DM_LOCATE_OTHER_LEVEL(signedlong(ddat.v1d3248), lcon(0xffffffff), &vw_00, &vw_04, NULL);
  if (RG1W < 0)
    return;
  RG4L = signedlong(RG1W) << 2;
  RG1P = DOWNCAST(t_tile*, mapdat.tmpmap) + RG4L;
  RG4L = signedlong(vw_00) << 2;
  RG1P = pointer_at(pointer_at(RG1P) + RG4L) + signedlong(vw_04);
  RG4L = unsignedlong(byte_at(RG1P));
  RG1Blo = RG4Blo;
  RG1L = unsignedlong(RG1Blo) >> 5;
  RG1L = unsignedlong(RG1Blo);
  if (RG1L != 2)
    return;
  RG1L = RG4L;
  RG1Bhi ^= RG4Bhi;
  RG1Blo &= lcon(0x8);
  RG1L = signedlong(RG1W);
  if (RG1L == 0)
    return;
  RG1W = unsignedword(RG1Blo);
  RG1Blo = table1d6c67[RG6l];
  if (RG5w == 0)
  {
    RG1L = DM2_SET_GRAPHICS_FLIP_FROM_POSITION(lcon(0x20), signedlong(ddat.v1e12cc), signedlong(ddat.v1e12ca), signedlong(ddat.v1e12c8));
    if (RG1L == 0)
      //m_40971:
      RG1L = 0;
    else
      RG1L = 1;
  }
  //m_40973:
  RG1L = unsignedlong(RG1W);
  i32 parl02 = RG1L;
  RG1L = signedlong(vw_08);
  put16(parw01, RG1W);
  RG1L = signedlong(RG5w);
  RG4L = RG1L;
  RG3L = signedlong(table1d6c4c[RG1L]);
  RG2L = unsignedlong(table1d6c5e[RG1L]);
  RG4L ^= RG1L;
  RG4Blo = vb_0c;
  DM2_DRAW_DUNGEON_GRAPHIC(8, RG4Blo, RG2Blo, RG3W, parw01, CUTX16(parl02));
}

// SKW_32cb_245a
static void DM2_DRAW_PIT_TILE(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  i16 RG6w;
  i16 vw_00;
  i8 vb_04;
  i16 parw00;

  RG5w = RG1W;
  RG1Blo = ddat.v1d6c02;
  vb_04 = RG1Blo;
  vw_00 = ddat.v1d6c00;
  RG1L = signedlong(RG5w);
  RG6w = table1d6c70[RG1L];
  if (RG6w < 0)
    return;
  if (RG1L >= lcon(0xb))
  {
    RG4L = RG1L;
    RG1L *= 8;
    RG1L += RG4L;
    RG4P = ddat.ptr1e1044;
    if (word_at(RG4P + 2 * RG1L + lcon(0x8)) != 0)
      return;
  }
  RG1L = signedlong(RG5w);
  RG1Blo = table1d6c90[RG1L];
  RG1W = unsignedword(RG1Blo);
  if (RG5w == wcon(0x0))
  {
    RG1L = DM2_SET_GRAPHICS_FLIP_FROM_POSITION(1, signedlong(ddat.v1e12cc), signedlong(ddat.v1e12ca), signedlong(ddat.v1e12c8));
    if (RG1L == 0)
      //m_40A41:
      RG1L = 0;
    else
      RG1L = 1;
  }
  //m_40A43:
  RG1L = unsignedlong(RG1W);
  i32 parl01 = RG1L;
  RG1L = signedlong(vw_00);
  put16(parw00, RG1W);
  RG3L = signedlong(RG6w);
  RG4L = signedlong(RG5w);
  RG2L = 18 * RG4L;
  RG1P = ddat.ptr1e1044;
  if (word_at(RG1P + RG2L + lcon(0x8)) == 0)
    //m_40A75:
    RG1Blo = table1d6cb0[RG4L];
  else
    RG1Blo = table1d6ca0[RG4L];
  //m_40A7B:
  RG2L = unsignedlong(RG1Blo);
  DM2_DRAW_DUNGEON_GRAPHIC(8, vb_04, RG2Blo, RG3W, parw00, CUTX16(parl01));
}

// belongs to DM2_ENVIRONMENT_DISPLAY_ELEMENTS
// SKW_32cb_56bc
static void DM2_ENVIRONMENT_DRAW_DISTANT_ELEMENT(unk* xeaxp, i32 ebxl, i32 ecxl, i32 edxl)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p;
  c_wlreg _RG6;
  i16 vw_00;
  i16 vw_04;
  i16 vw_08;
  i16 vw_0c;
  i16 vw_10;
  i16 parw04;
  i8 parb07;
  i8 parb08;
  bool skip00972 = false;
  bool skip00969 = false;

  RG5p = RG1P;
  RG6L = RG4L;
  vw_04 = RG2W;
  vw_08 = RG3W;
  if (word_at(RG1P + 2) == 0)
    return;
  RG1L = unsignedlong(byte_at(RG5p, 1));
  if (RG1L != lcon(0x8))
    skip00969 = true;
  else
  {
    RG3L = signedlong(vw_08);
    RG2L = signedlong(vw_04);
    RG4L = signedlong(RG6W);
    RG1L = DM2_SET_GRAPHICS_FLIP_FROM_POSITION(1, RG2L, RG3L, RG4L);
    if (RG1L == 0)
      skip00969 = true;
  }

  if (skip00969)
  {
    bool skip00970 = false;
    //m_445B6:
    RG1L = unsignedlong(byte_at(RG5p, 1));
    if (RG1L != 2)
      skip00970 = true;
    else
    {
      RG3L = signedlong(vw_08);
      RG2L = signedlong(vw_04);
      RG4L = signedlong(RG6W);
      RG1L = DM2_SET_GRAPHICS_FLIP_FROM_POSITION(lcon(0x20), RG2L, RG3L, RG4L);
      if (RG1L == 0)
        skip00970 = true;
    }

    if (skip00970)
    {
      bool skip00971 = false;
      //m_445E8:
      RG1L = unsignedlong(byte_at(RG5p, 1));
      if (RG1L != lcon(0x40))
        skip00971 = true;
      else
      {
        RG3L = signedlong(vw_08);
        RG2L = signedlong(vw_04);
        RG4L = signedlong(RG6W);
        RG1L = DM2_SET_GRAPHICS_FLIP_FROM_POSITION(1, RG2L, RG3L, RG4L);
        if (RG1L == 0)
          skip00971 = true;
      }

      if (skip00971)
      {
        bool skip00973 = false;
        //m_44611:
        RG1L = unsignedlong(byte_at(RG5p, 1));
        if (RG1L != lcon(0x20))
          skip00973 = true;
        else
        {
          RG3L = signedlong(vw_08);
          RG2L = signedlong(vw_04);
          RG4L = signedlong(RG6W);
          RG1L = DM2_SET_GRAPHICS_FLIP_FROM_POSITION(RG1L, RG2L, RG3L, RG4L);
          if (RG1L == 0)
            skip00973 = true;
        }

        if (skip00973)
        {
          //m_44635:
          RG2L = 0;
          vw_00 = RG2W; // truncated
        }
        else
          skip00972 = true;
      }
      else
        skip00972 = true;
    }
    else
      skip00972 = true;
  }
  else
    skip00972 = true;

  if (skip00972)
    //m_445DF:
    vw_00 = 1;

  //m_4463A:
  RG3L = unsignedlong(byte_at(RG5p, lcon(0x8)));
  RG1L = unsignedlong(byte_at(RG5p, lcon(0x9)));
  vw_0c = RG1W;
  RG1W = word_at(RG5p, lcon(0x4));
  vw_10 = RG1W;
  RG6W = word_at(RG5p, lcon(0x6));
  if (ddat.v1e025c != 0)
  {
    RG1L = blitter.calc_stretched_size(vw_10, lcon(0x34));
    RG2L = RG1L;
    vw_10 = RG1W;
    RG1L = blitter.calc_stretched_size(RG6W, lcon(0x34));
    RG6L = RG1L;
    RG1L = blitter.calc_stretched_size(RG3W, lcon(0x34));
    RG3L = RG1L;
    RG1L = blitter.calc_stretched_size(vw_0c, lcon(0x34));
    vw_0c = RG1W;
    RG1L = unsignedlong(byte_at(RG5p, lcon(0x8)));
    if (RG1L != lcon(0x40))
      //m_446E5:
      RG6W += ddat.v1d703d;
    else
    {
      RG1W = glblrects.dm2rect2.x;
      RG1L += RG2L;
      vw_10 = RG1W;
      RG1L = signedlong(word_at(RG5p, 2));
      if (RG1L != lcon(0x1771))
        //m_446DC:
        RG6W += glblrects.dm2rect2.y;
      else
        RG6W += ddat.v1d703f;
    }
  }
  //m_446EC:
  RG1L = unsignedlong(byte_at(RG5p));
  put8(parb08, RG1Blo);
  RG1L = unsignedlong(ddat.v1d6c02);
  put8(parb07, RG1Blo);
  RG1L = signedlong(ddat.v1d6c00);
  put16(parw04, RG1W);
  RG1W = word_at(RG5p, 2);
  RG1Bhi |= lcon(0xffffff80);
  RG1L = signedlong(RG1W);
  i32 parl02 = RG1L;
  RG2L = signedlong(vw_0c);
  RG4L = signedlong(RG3W);
  DM2_QUERY_TEMP_PICST(
    vw_00, RG4W, RG2W, 0,
    0, 0, CUTX16(parl02), -1,
    parw04, lcon(0xffff), lcon(0x17), parb07, parb08);

  RG1L = unsignedlong(vw_10);
  dm2_image2.srcx += RG1W;
  dm2_image2.srcy += RG6W;
  DM2_DRAW_TEMP_PICST();
}

// belongs to DM2_DRAW_DUNGEON_TILES
// SKW_32cb_4e1c
static void DM2_DRAW_STAIRS_FRONT(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_nreg _RG51;
  i16 RG52w;
  c_lreg _RG6;
  c_nreg _RG7;
  i16 vw_00;
  i16 vw_04;
  i16 parw00;
  i16 parw06;
  i8 parb09;
  i8 parb10;

  RG52w = RG1L;
  RG4Blo = ddat.v1d6c02;
  vw_04 = ddat.v1d6c00;
  RG3L = signedlong(RG52w);
  RG1L = 8 * RG3L;
  RG3L += RG1L;
  RG3L = 2 * RG3L;
  RG1P = ddat.ptr1e1044;
  RG1Blo = word_at(RG1P + RG3L + lcon(0x8)) != 0 ? 1 : 0;
  RG6L = unsignedlong(RG1Blo);
  RG3L = signedlong(RG52w);
  RG1L = RG3L;
  RG1W = table1d6f9c[RG6L + 2 * RG1L];
  vw_00 = RG1W;
  if (RG1W < 0)
    return;
  RG3L = 2 * RG3L;
  RG6L += RG3L;
  RG1Blo = table1d6f5c[RG6L];
  RG7L = unsignedlong(RG1Blo);
  RG51L = unsignedlong(RG4Blo);
  RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x8), CUTLX8(RG51L), 1, RG7Blo) ? 1 : 0;
  if (RG1L != 0)
  {
    RG1L = signedlong(vw_04);
    put16(parw00, RG1W);
    DM2_DRAW_DUNGEON_GRAPHIC(8, CUTLX8(RG51L), CUTLX8(RG7L), vw_00, parw00, 0);
    return;
  }
  RG1Blo = table1d6f7c[RG6L];
  put8(parb10, RG1Blo);
  put8(parb09, RG51Blo);
  RG1L = signedlong(vw_04);
  put16(parw06, RG1W);
  i32 parl04 = signedlong(vw_00);
  RG3L = 0;
  RG2L = lcon(0x40);
  DM2_QUERY_TEMP_PICST(
    1, RG2W, RG2W, RG3W,
    0, 0, CUTX16(parl04), -1,
    parw06, lcon(0xffff), lcon(0x8), parb09, parb10);

  DM2_DRAW_TEMP_PICST();
}

// belongs to DM2_DRAW_DUNGEON_TILES
// SKW_32cb_4ecc
static void DM2_DRAW_STAIRS_SIDE(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8 vb_00;
  i16 parw00;

  RG4L = RG1L;
  RG1Blo = ddat.v1d6c02;
  vb_00 = RG1Blo;
  RG3W = ddat.v1d6c00;
  RG2L = signedlong(RG4W);
  if (RG2L <= lcon(0x8))
  {
    RG1L = 8 * RG2L;
    RG2L += RG1L;
    RG2L = 2 * RG2L;
    RG1P = ddat.ptr1e1044;
    RG1Blo = word_at(RG1P + RG2L + lcon(0x8)) != wcon(0x0) ? 1 : 0;
    RG1W = unsignedword(RG1Blo);
    RG2L = RG1L;
    RG1L = signedlong(RG4W);
    RG4L = RG1L;
    RG2L = signedlong(RG2W);
    RG4Blo = table1d6fdc[2 * RG4L + RG2L];
    if (RG4Blo != lcon(0xffffffff))
    {
      RG3L = signedlong(RG3W);
      put16(parw00, RG3W);
      RG3L = signedlong(table1d6fee[2 * RG1L + RG2L]);
      RG2L = unsignedlong(RG4Blo);
      DM2_DRAW_DUNGEON_GRAPHIC(8, vb_00, RG2Blo, RG3W, parw00, 0);
    }
  }
}

static void DM2_TRIM_BLIT_RECT(i16 x, i16 y, i16 w, i16 h)
{
  glblrects.dm2rect1.set(x, y, gfxsys.backbuffer_w - (x + w), gfxsys.backbuffer_h - (y + h));
}

static void DM2_DRAW_WALL(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG52w;
  i16 RG53w;
  i16 RG6w;
  c_ql32 vql_00;
  i16 vw_04;
  i16 vw_08;
  i8 vb_0c;
  i8 vb_10; // sub8
  i8 vb_14;
  i16 parw01;
  i16 parw04;
  i8 parb07;
  i8 parb08;
  bool skip00958 = false;

  vql_00.poke32(RG1L);
  RG1Blo = ddat.v1d6c02;
  vb_0c = RG1Blo;
  vw_04 = ddat.v1d6c00;
  RG1L = signedlong(vql_00.peek16());
  RG4Blo = table1d6afe[RG1L];
  vb_14 = RG4Blo;
  RG4W = signedword(table1d6b15[RG1L]);
  vw_08 = RG4W;
  RG1L = signedlong(table1d6b15[RG1L]);
  RG1Blo = (RG1L > 1 ? 1 : 0) + lcon(0x6e);
  vb_10 = RG1Blo;
  RG6w = wcon(0x0);
  RG52w = wcon(0x0);
  RG3L = unsignedlong(RG1Blo);
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0x8), ddat.v1d6c02, lcon(0xb), RG3Blo);
  RG2W = RG1W;
  RG4L = signedlong(vw_08);
  if (RG4L <= 1)
  {
    //m_43DCA:
    if (vw_08 <= 0)
      skip00958 = true;
    else
    {
      RG4P = ddat.ptr1e1044;
      if (word_at(RG4P + lcon(0x14)) == 0)
      {
        RG1L = unsignedlong(RG1W) >> bcon(0x8);
        RG52w = RG1W;
      }
      RG1P = ddat.ptr1e1044;
      if (word_at(RG1P + lcon(0x26)) != 0)
        skip00958 = true;
    }
  }
  else
  {
    RG4P = ddat.ptr1e1044;
    if (word_at(RG4P + lcon(0x4a)) == 0)
    {
      RG1L = unsignedlong(RG1W) >> bcon(0x8);
      RG52w = RG1W;
    }
    RG1P = ddat.ptr1e1044;
    if (word_at(RG1P + lcon(0x5c)) != 0)
      skip00958 = true;
  }

  if (!skip00958)
  {
    //m_43DF4:
    RG2W = unsignedword(RG2Blo);
    RG6w = RG2W;
  }

  //m_43DF8:
  if (RG52w != wcon(0x0) || RG6w != wcon(0x0))
  {
    RG2L = signedlong(RG6w);
    RG1L = signedlong(RG52w);
    DM2_TRIM_BLIT_RECT(RG52w, 0, RG6w, 0);
  }
  //m_43E11:
  RG1Blo = vb_14 > bcon(0x0) ? 1 : 0;
  RG53w = unsignedword(RG1Blo);
  RG1L = signedlong(vql_00.peek16());
  if (RG1L < lcon(0x10))
  {
    //m_43E46:
    if (ddat.v1e12d0 == 0)
    {
      //m_43E88:
      RG1Blo = vql_00.peek8() + bcon(0x22);
      vb_10 = RG1Blo;
    }
    else
    {
      RG1Blo = table1d6b2c[RG1L] + bcon(0xb0);
      vb_10 = RG1Blo;
      RG3L = unsignedlong(RG1Blo);
      RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x8), vb_0c, 1, RG3Blo) ? 1 : 0;
      if (RG1L == 0)
      {
        if (vb_14 == 0)
          RG53w = wcon(0x1);
        vb_10 -= lcon(0xffffff8e);
      }
    }
  }
  else
  {
    RG1L = signedlong(vb_14);
    if (RG1L == lcon(0xfffffffe) || RG1L == 2)
      RG53w = wcon(0x0);
    RG53w ^= ddat.v1e12d0;
    vb_10 = lcon(0x32);
  }
  //m_43E90:
  RG1L = unsignedlong(vb_10);
  put8(parb08, RG1Blo);
  RG1L = unsignedlong(vb_0c);
  put8(parb07, RG1Blo);
  RG1L = signedlong(vw_04);
  put16(parw04, RG1W);
  RG1L = signedlong(CUTX16(vql_00.peek32() + lcon(0x2be)));
  i32 parl02 = RG1L;
  if (ddat.v1e025c == 0)
    //m_43EC7:
    RG1L = 0;
  else
    RG1L = -signedlong(vw_08);
  //m_43EC9:
  RG1L = signedlong(RG1W);
  put16(parw01, RG1W);
  RG1L = unsignedlong(RG53w);
  RG3L = 0;
  RG2L = lcon(0x40);
  DM2_QUERY_TEMP_PICST(
    RG1W, RG2W, RG2W, RG3W,
    0, parw01, CUTX16(parl02), -1,
    parw04, lcon(0xffff), lcon(0x8), parb07, parb08);

  DM2_DRAW_TEMP_PICST();
  ddat.v1e01d0 = false;
}

bool DM2_guivp_32cb_00f1(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  i32 RG6l;
  i16 vw_00;

  RG3L = RG1L;
  RG5w = RG4W;
  vw_00 = RG2W;
  RG2L = signedlong(RG4W);
  RG4L = signedlong(RG1W);
  RG1L = dm2_image2.rect.pt_in_rect(RG4W, RG2W) ? 1 : 0;
  if (RG1L == 0)
    return false;
  RG1L = unsignedlong(mkuw(dm2_image2.blitmode & wcon(0x1)));
  if (RG1L == 0)
  {
    //m_3DCE4:
    RG3W -= dm2_image2.rect.x;
    RG3W += dm2_image2.srcx;
  }
  else
  {
    RG1W = dm2_image2.rect.x + dm2_image2.rect.w;
    RG1L--;
    RG1L -= RG3L;
    RG3W = dm2_image2.srcx;
    RG3L += RG1L;
  }
  //m_3DCF2:
  RG5w -= dm2_image2.rect.y;
  RG5w += dm2_image2.srcy;
  c_pixel* gfx2 = PIXELCAST(DM2_QUERY_PICT_BITS(&dm2_image2.imgdesc));
  RG1L = signedlong(getbmpheader(BMPCAST(gfx2))->width) / 2;
  RG4L = signedlong(dm2_image2.imgdesc.x);
  RG6l = signedlong(RG3W);
  RG3L = RG6l - RG4L;
  RG4L = RG3L;
  RG3L = signedlong(RG1W);
  RG4L -= RG3L;
  RG1L = 24 * RG4L;
  RG4L += RG1L;
  RG4L *= 4;
  RG1L = RG4L;
  RG4L = RG1L % RG3L;
  RG1L /= RG3L;
  ddat.v1e12da = RG1W;
  RG1L = signedlong(getbmpheader(BMPCAST(gfx2))->height) / 2;
  RG2L = signedlong(RG5w);
  RG3L = signedlong(RG1W);
  RG4L = RG2L - RG3L;
  RG1L = 24 * RG4L;
  RG4L += RG1L;
  RG4L *= 4;
  RG1L = RG4L;
  RG4L = RG1L % RG3L;
  RG1L /= RG3L;
  ddat.v1e12d8 = RG1W;
  RG3L = signedlong(vw_00);
  RG4L = RG6l;
  RG1W = DM2_image_0b36_1446(&dm2_image2.imgdesc, RG4W, RG2W, RG3UBlo);
  if (RG1W != -1 && RG1W != 1)
    return true;
  return false;
}

static t_palette* DM2_guivp_32cb_0649(i8 eaxb, i8 edxb, i8 ebxb, i16 ecxw)
{
  c_nreg _RG1; RG1Blo = eaxb;
  c_nreg _RG2; RG2Blo = ebxb;
  c_nreg _RG3; RG3W = ecxw;
  c_nreg _RG4; RG4Blo = edxb;
  i16 vw_00;

  RG2L = unsignedlong(RG2Blo);
  RG4L = unsignedlong(RG4Blo);
  t_palette* pal = DM2_QUERY_GDAT_IMAGE_LOCALPAL(RG1Blo, RG4Blo, RG2Blo);
  if (pal != NULL)
  {
    //m_3E30E:
    DM2_COPY_SMALL_PALETTE(paldat.small_palette, pal);
    vw_00 = lcon(0x10); // adj
  }
  else
  {
    RG1L = signedlong(ddat.v1e12d2);
    if (RG1L == lcon(0x3f))
      return NULL;
    RG2L = 0;
    vw_00 = RG2W; // adj
  }
  //m_3E324:
  RG2L = signedlong(RG3W);
  return DM2_query_B073(paldat.small_palette, CUTX8(ddat.v1e12d2), RG2W, -1, &vw_00);
}

// SKW_32cb_24fb
static void DM2_DRAW_TELEPORTER_TILE(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  const i8* RG5p;
  c_imgdesc imgdesc;
  c_rect rc_18;
  i32 vl_20;
  i16 vw_24;
  i16 vw_28;
  i16 vw_2c;
  i16 vw_30;
  i16 vw_34;
  i16 vw_38;
  i16 vw_3c;
  i8 vb_40;
  i8 vb_44;
  i8 vb_48;
  i16 parw03;
  i16 parw04;
  i16 parw10;
  i16 parw11;
  i16 parw12;
  i16 parw13;

  vb_44 = RG4Blo;
  vb_48 = RG2Blo;
  RG1L = signedlong(RG1W);
  if (RG1L > lcon(0xf))
    return;
  RG5p = table1d6ce0 + 4 * RG1L;
  RG4W = table1d6cc0[RG1L];
  RG2L = 0;
  vw_2c = RG2W;
  RG1Blo = byte_at(RG5p, 1) & lcon(0x7f);
  vb_40 = RG1Blo;
  RG1Blo = byte_at(RG5p, 1) & lcon(0xffffff80);
  RG1L = unsignedlong(RG1Blo);
  RG1Blo = RG1L != 0 ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  vw_30 = RG1W; // truncated
  RG1L = unsignedlong(byte_at(RG5p, 2));
  vw_24 = RG1W; // adj
  RG1L = unsignedlong(byte_at(RG5p, lcon(0x3)));
  vw_28 = RG1W; // adj
  RG2L = signedlong(RG4W);
  RG3P16 = &vw_24;
  RG4R = &rc_18;
  RG1R = DM2_QUERY_BLIT_RECT(NULL, RG4R, RG2W, IO vw_24, IO vw_28);
  if (RG1R == NULL)
    return;
  t_bmp* srcbmp1;
  if (byte_at(RG5p + 1) != lcon(0xffffffff))
  {
    //m_40B29:
    RG1Blo = vb_44;
    imgdesc.b_08 = RG1Blo;
    RG1Blo = vb_48;
    imgdesc.b_09 = RG1Blo;
    RG1Blo = vb_40;
    imgdesc.b_0b = RG1Blo;
    srcbmp1 = DM2_QUERY_PICST_IMAGE(&imgdesc);
    DM2_ALLOCATE_IMG_COPY(&imgdesc);
    if (vw_30 != 0)
    {
      RG1W = /*TAG dbidx*/ dm2_dballochandler.DM2_dballoc_3e74_5888();
      vw_34 = RG1W;
      RG2W = getbmpheader(srcbmp1)->height;
      vw_38 = RG2W;
      RG2L = signedlong(vw_38);
      RG4W = getbmpheader(srcbmp1)->width;
      vw_3c = RG4W;
      RG4L = signedlong(vw_3c);
      RG1L = signedlong(RG1W);
      c_pixel* destgfx = DM2_ALLOC_NEW_BMP(RG1W, RG4W, RG2W, BPP_4);
      c_pixel* srcgfx1 = PIXELCAST(DM2_QUERY_PICT_BITS(&imgdesc));
      RG1W = getbmpheader(BMPCAST(srcgfx1))->width & 1;
      if (RG1W != 0)
      {
        RG4L = 2 - RG1L;
        RG1L = RG4L;
      }
      RG4L = signedlong(vw_3c);
      put16(parw04, RG4W);
      put16(parw03, RG4W);
      RG4L = signedlong(vw_38);
      RG2L = unsignedlong(vw_3c) - RG1L;
      RG1L = signedlong(RG2W);
      RG1R = tmprects.alloc_origin_tmprect(RG1W, RG4W);
      RG3L = 0;
      RG2R = RG1R;
      ui16 destw;
      if (destgfx == gfxsys.dm2screen)
        destw = ORIG_SWIDTH;
      else
        destw = parw04;

      blitter.blit(
        srcgfx1,
        destgfx,
        /*TAG blitrect*/ RG2R,
        /*TAG srcx*/ RG3W,
        0,
        /*TAG srcw*/ parw03,
        /*TAG destw*/ destw,
        NOALPHA,
        BLITMODE1,
        BPP_4,
        BPP_4);

      DM2_image_0b36_01cd(&imgdesc);
      DM2_SET_IMAGE((t_dbidx)vw_34, &imgdesc);
      DM2_ALLOCATE_IMG_COPY(&imgdesc);
    }
  }
  else
    srcbmp1 = NULL;
  //m_40C08:
  RG4L = unsignedlong(vb_48);
  t_bmp* srcbmp2 = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(vb_44, RG4Blo, 20));
  if (byte_at(RG5p + 1) != lcon(0xffffffff))
    srcbmp1 = DM2_QUERY_PICT_BITS(&imgdesc);
  vl_20 = signedlong(vw_2c);
  t_palette* pal = DM2_guivp_32cb_0649(vb_44, vb_48, 20, CUTX16(vl_20));
  RG4L = vl_20;
  put16(parw13, RG4W);
  s_dm2bmpheader* bmpheader = getbmpheader(gfxsys.bitmapptr);
  RG1L = signedlong(bmpheader->width);
  put16(parw12, RG1W);
  RG1L = DM2_RAND();
  RG1W = unsignedword(RG1Blo);
  RG1L = signedlong(RG1W);
  put16(parw11, RG1W);
  RG1L = signedlong(CUTX16(16 * ((DM2_RANDBIT() ? 1 : 0) + unsignedlong(byte_at(RG5p)))));
  put16(parw10, RG1W);
  DM2_blit_specialeffects(PIXELCAST(srcbmp2), PIXELCAST(gfxsys.bitmapptr), PIXELCAST(srcbmp1), &rc_18, parw10, parw11, parw12, parw13, pal);
  if (byte_at(RG5p + 1) != lcon(0xffffffff))
    DM2_image_0b36_01cd(&imgdesc);
}

static void DM2_guivp_32cb_0a4c(c_rect* eaxrp, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1R = eaxrp;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i8 vb_00;
  i8 vb_04;

  RG3R = RG1R;
  vb_04 = RG4Blo;
  vb_00 = RG2Blo;
  RG4L = signedlong(ddat.v1e0404);
  RG1L = 12 * RG4L;
  RG4R = &ddat.v1e02f0[RG4L].rc_00;
  RG1R = RG3R;
  *RG4R = *RG1R; // copy
  RG3L = signedlong(ddat.v1e0404);
  RG1L = 3 * RG3L;
  RG4Blo = vb_04;
  ddat.v1e02f0[RG3L].b_0b = RG4Blo;
  RG4Blo = vb_00;
  ddat.v1e02f0[RG3L].b_0a = RG4Blo;
  ddat.v1e02f0[RG3L].w_08 = lcon(0xffffffff);
  ddat.v1e0404++;
}

void DM2_guivp_32cb_0c7d(c_image* image, t_dbidx bmpid, i16 colidx)
{
  if (ddat.v1e1482 == 0)
  {
    ui8 fc;
    if (image->colors > 0)
      //m_3EA0C:
      fc = palettecolor_to_ui8(image->palette[colidx]);
    else
      fc = CUTUX8(colidx);
    //m_3EA19:
    DM2_FILL_ENTIRE_PICT(dm2_dballochandler.DM2_GET_BMP(bmpid), ui8_to_pixel(fc));
    return;
  }
  //m_3EA33:
  i8 vb_1c;
  i16 bm;
  DM2_QUERY_RAINFALL_PARAM(OO vb_1c, OO bm);
  i16 dummy;
  i16 vw_0c;
  DM2_QUERY_GDAT_IMAGE_METRICS(23, ddat.v1d6c02, vb_1c, &dummy, &vw_0c);
  t_dbidx bmpid2 = dm2_dballochandler.DM2_dballoc_3e74_5888();
  DM2_ALLOC_NEW_BMP(bmpid2, image->rect.w, vw_0c, BPP_4);
  i16 w = MK_EVEN(image->rect.w);
  t_bmp* srcbmp1 = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(23, ddat.v1d6c02, vb_1c));
  c_rect* blitrect = tmprects.alloc_origin_tmprect(w, getbmpheader(srcbmp1)->height);
  ui16 destw;
  c_pixel* destgfx = PIXELCAST(dm2_dballochandler.DM2_GET_BMP(bmpid2));
  if (destgfx == gfxsys.dm2screen)
    destw = ORIG_SWIDTH;
  else
    destw = w;

// check sourcebitmap before drawing
// first graphics looks like the rain-graphic

  blitter.blit(
    PIXELCAST(srcbmp1),
    destgfx,
    blitrect,
    0,
    0,
    getbmpheader(srcbmp1)->width,
    destw,
    NOALPHA,
    bm == 1 ? BLITMODE1 : BLITMODE0,
    BPP_4,
    BPP_4);

  t_bmp* srcgfx2 = dm2_dballochandler.DM2_GET_BMP(bmpid2);
  t_palette* pal = DM2_guivp_32cb_0649(23, ddat.v1d6c02, vb_1c, 0);
  ui8 c;
  if (image->colors > 0)
    //m_3EB1C:
    c = palettecolor_to_ui8(image->palette[colidx]);
  else
    c = CUTUX8(colidx);
  //m_3EB26:
  ui8_to_palettecolor(&pal[E_COL00], c);
  i16 rg3 = getbmpheader(srcgfx2)->width * getbmpheader(BMPCAST(srcgfx2))->height;
  DM2_blit_specialeffects(PIXELCAST(srcgfx2), PIXELCAST(dm2_dballochandler.DM2_GET_BMP(bmpid)), NULL, &image->rect, rg3 - CUTX16(DM2_RAND() & 0x1f) - 16, DM2_RAND16(rg3 - 60), image->imgdesc.width, -1, pal);
  dm2_dballochandler.DM2_dballoc_3e74_58a3(bmpid2);
}

static void DM2_guivp_32cb_2cf3(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  i16 RG6w;
  i32 vl_00;
  i8 vb_04;

  vb_04 = RG1Blo;
  RG6w = RG2W;
  vl_00 = ecxl;
  RG4L &= lcon(0xfffffffe);
  RG4L = signedlong(RG4W);
  RG5L = signedlong(DM2_BETWEEN_VALUE(8, 64, RG4W));
  DM2_QUERY_GDAT_SUMMARY_IMAGE(&dm2_image2, 13, vb_04, 65);
  dm2_image2.srcx += dm2_image2.w_1c;
  dm2_image2.srcy += dm2_image2.w_1e;
  RG1L = 0;
  dm2_image2.w_1e = RG1W;
  dm2_image2.w_1c = RG1W;
  RG4L = vl_00;
  dm2_image2.query1 = RG4W;
  dm2_image2.w_1a = RG1W;
  dm2_image2.alphamask = 0xa;
  dm2_image2.bmp = gfxsys.bitmapptr;
  dm2_image2.blitmode = RG6w;
  dm2_image2.w_36 = RG5W;
  dm2_image2.w_34 = RG5W;
  DM2_query_B073(dm2_image2.palette, CUTX8(ddat.v1e12d2), 10, -1, &dm2_image2.colors);
  DM2_QUERY_PICST_IT(&dm2_image2);
}

static i32 DM2_guivp_32cb_2d8c(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i8* RG51p;
  c_wlreg _RG52;
  i32 RG53l;
  i8* RG61p;
  c_wlreg _RG62;
  c_lreg _RG63;
  c_lreg _RG71;
  c_wlreg _RG72;
  i32 RG73l;
  i32 vl_00;
  c_ql32 vql_04;
  c_ql32 vql_08;
  c_rect rc_0c;
  i32 vl_14;
  i32 vl_18;
  i32 vl_1c;
  i16 vw_20;
  i32 vl_24;
  i16 vw_28;
  c_o32 vo_2c; // poke 16 peek 32 inc16
  i16 vw_30;
  i16 vw_34;
  i16 vw_38;
  i8 vb_3c;
  i16 parw00;
  i16 parw01;
  i16 parw02;
  bool flag;

  vql_08.poke32(RG1L);
  vql_04.poke32(RG4L);
  vl_00 = RG2L;
  vl_1c = RG1L;
  if (RG1W == lcon(0xfffffffe))
    return RG1L;
  RG1L = signedlong(vql_04.peek16());
  if (table1d6b5a[RG1L] < bcon(0x0))
    return RG1L;
  RG1W = signedword(table1d6b15[RG1L]);
  vw_38 = RG1W;
  RG2L = 0;
  vo_2c.poke16(RG2W);
  //m_41949:
  for (;;)
  {
    RG2L = signedlong(vo_2c.peek16());
    if (RG2L >= lcon(0x5))
      return RG1L;
    //m_41523:
    RG4L = signedlong(ddat.v1e12c8);
    RG1L = 5 * RG4L;
    RG1W = signedword(table1d6d5a[RG4L][RG2L]);
    vw_20 = RG1W;
    vql_08.poke32(vl_1c);
    //m_41549:
    for (;;)
    {
      RG1L = vql_08.peek32();
      RG1W &= lcon(0x3c00);
      RG1L = unsignedlong(RG1W) >> bcon(0xa);
      if (RG1L == lcon(0xf))
      {
        RG1L = unsignedlong(vql_08.peek16());
        RG4L = RG1L >> bcon(0xe);
        vw_30 = RG4W;
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
        RG51p = RG1P;
        RG61p = RG1P;
        RG1W = word_at(RG1P, 2);
        RG1Blo &= lcon(0x7f);
        vb_3c = RG1Blo;
        RG71L = unsignedlong(RG1Blo);
        RG3L = lcon(0x14);
        RG2L = 1;
        RG4L = RG71L;
        RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xd), RG4Blo, RG2Blo, RG3Blo) ? 1 : 0;
        if (RG1L == 0)
        {
          //m_415CC:
          if (jz_test8(location(RG51p + 2), lcon(0xffffff80)))
          {
            //m_415DE:
            RG1W = vw_20;
            flag = RG1W == vw_30;
          }
          else
          {
            RG1L = signedlong(vw_20);
            flag = RG1L == lcon(0xffffffff);
          }
          //m_415E7:
          RG1Blo = flag ? 1 : 0;
          RG1L = unsignedlong(RG1Blo);
          if (RG1L != 0)
          {
            bool skip00910 = false;
            bool skip00911 = false;
            vl_14 = 0;
            vl_18 = signedlong(DM2_RANDDIR());
            RG52L = unsignedlong(vb_3c);
            RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(13, CUTLX8(RG52L), lcon(0xb), 0);
            RG4L = vl_18 & RG1W;
            vw_28 = RG4W; // truncated
            RG3L = lcon(0x41);
            RG2L = lcon(0xb);
            RG4L = RG52L;
            RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(13, RG4Blo, RG2Blo, RG3Blo);
            if (RG1W == 0)
            {
              //m_41648:
              RG52W = word_at(RG61p, 2);
              RG52UW >>= bcon(0x8);
            }
            else
              RG52L = lcon(0xff);
            //m_41650:
            if (vql_04.peek16() != 0)
            {
              //m_4175C:
              RG1L = unsignedlong(vb_3c);
              if (RG1L == lcon(0x30))
              {
                RG2L = 3;
                RG4L = 1;
                DM2_UPDATE_GLOB_VAR(lcon(0x40), RG4L, RG2L);
              }
              if (vw_38 != 0)
                skip00910 = true;
              else
              {
                RG1L = signedlong(vo_2c.peek16());
                if (RG1L < lcon(0x3))
                  skip00910 = true;
              }
            }
            else
            {
              vl_14 = 1;
              RG1L = unsignedlong(vb_3c);
              if (RG1L != lcon(0x30))
                skip00911 = true;
              else
              {
                RG2L = 3;
                DM2_UPDATE_GLOB_VAR(lcon(0x41), 1, RG2L);
                skip00910 = true;
              }
            }

            if (skip00910)
            {
              //m_41793:
              if (jz_test8(location(RG61p + 2), lcon(0xffffff80)))
              {
                //m_417A0:
                RG1L = signedlong(vw_30);
                RG1W = table1d2752[RG1L];
              }
              else
                RG1L = lcon(0xc);
              //m_417AF:
              RG4L = signedlong(ddat.v1e12c8);
              RG72L = signedlong(RG1W);
              RG1W = DM2_ROTATE_5x5_POS(RG72W, t_rotate(RG4UW));
              RG3L = signedlong(RG1W);
              RG4L = 1 << RG3Blo;
              if ((RG4L & vl_00) != 0)
              {
                RG3L = 4 * signedlong(vw_38);
                RG4L = signedlong(RG1W);
                RG2L = 5;
                RG1L = RG4L;
                RG4L = RG1L % RG2L;
                RG1L /= RG2L;
                RG4L = RG3L + 2 - RG1L;
                RG1Blo = table1d6b76[RG4L];
                RG1W = unsignedword(RG1Blo);
                vw_34 = RG1W;
                RG2L = signedlong(ddat.v1e0258);
                RG1L = signedlong(vql_04.peek16());
                RG4L = RG72L;
                RG1W = DM2_QUERY_CREATURE_BLIT_RECTI(RG1W, t_rotate(RG2UW), RG4W);
                RG1Bhi |= lcon(0xffffff80);
                RG2L = RG1L;
                RG1L = RG52L + 1;
                RG4L = signedlong(RG1W);
                RG1L = signedlong(DM2_MAX(48, RG4W));
                RG4L = signedlong(RG1W);
                RG1L = signedlong(vw_34) * RG4L >> bcon(0x8);
                RG3L = signedlong(RG2W);
                RG2L = unsignedlong(vw_28);
                RG4L = signedlong(RG1W);
                DM2_guivp_32cb_2cf3(unsignedlong(vb_3c), RG4L, RG2L, RG3L);
                RG2L = RG52L - lcon(0xa0);
                if (RG2W > 0)
                {
                  RG1L = lcon(0xc0) - RG2L;
                  RG2L = RG1L;
                  if (vl_14 != 0)
                  {
                    RG1W = signedword(table1d6b15[0]);
                    if (RG1W == vw_38)
                      RG2L = lcon(0x1e);
                  }
                  RG4L = signedlong(gfxsys.backbuffer_h) << 3;
                  RG73l = signedlong(RG2W);
                  RG1L = RG4L / RG73l;
                  RG4L = signedlong(dm2_image2.srcy) + RG1L;
                  dm2_image2.srcy = RG4W;
                  if (jz_test8(location(RG61p + 2), lcon(0xffffff80)))
                  {
                    RG4L = signedlong(gfxsys.backbuffer_w) << 3;
                    RG73l -= lcon(0xa);
                    RG1L = RG4L;
                    RG4L = RG1L % RG73l;
                    RG1L /= RG73l;
                    RG2L = RG1L;
                    RG1L = signedlong(vo_2c.peek16());
                    if (RG1L == 1 || RG1L == lcon(0x4))
                      RG2L = -RG2L;
                    dm2_image2.srcx += RG2W;
                  }
                }
                RG62L = signedlong(vw_34);
                RG1L = blitter.calc_stretched_size(dm2_image2.srcx, RG62W);
                dm2_image2.srcx = RG1W;
                RG1L = signedlong(dm2_image2.srcy);
                RG4L = RG62L;
                RG1L = blitter.calc_stretched_size(RG1W, RG4W);
                dm2_image2.srcy = RG1W;
                DM2_DRAW_TEMP_PICST();
                if (vl_14 != 0)
                  skip00911 = true;
              }
            }

            if (skip00911)
            {
              //m_41688:
              RG2L = 0;
              RG1L = unsignedlong(RG52W) >> 5;
              vw_30 = RG1W;
              if (RG1W != 0)
              {
                RG1L = signedlong(vw_30);
                RG2L = 1;
                if (RG1L > lcon(0x3))
                  RG2L = 2;
              }
              RG1Blo = RG2Blo + bcon(0x42);
              RG53l = unsignedlong(RG1Blo);
              RG63L = unsignedlong(vb_3c);
              RG2L = RG53l;
              RG4L = RG63L;
              t_bmp* srcbmp = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(13, RG4Blo, RG2Blo));
              RG2L = RG53l;
              RG4L = RG63L;
              dm2_dballochandler.DM2_ALLOCATE_GFX16(lcon(0xd), RG4Blo, RG2Blo);
              RG4W = getbmpheader(srcbmp)->width;
              RG2W = getbmpheader(srcbmp)->height;
              RG4L *= RG2L;
              vl_24 = RG4L;
              RG3L = 0;
              t_palette* pal = DM2_guivp_32cb_0649(13, CUTLX8(RG63L), CUTLX8(RG53l), RG3W);
              s_dm2bmpheader* bmpheader = getbmpheader(gfxsys.bitmapptr);
              RG1L = signedlong(bmpheader->width);
              put16(parw02, RG1W);
              RG4L = vl_24 - 40;
              RG1W = DM2_RAND16(RG4W);
              put16(parw01, RG1W);
              RG1L = DM2_RAND();
              RG1W &= lcon(0x1f);
              RG4L += RG1L;
              RG1L = signedlong(RG4W);
              put16(parw00, RG1W);
              RG4R = &rc_0c;
              RG1R = DM2_QUERY_EXPANDED_RECT(4, RG4R);
              DM2_blit_specialeffects(PIXELCAST(srcbmp), gfxsys.bitmapptr, NULL, RG1R, parw00, parw01, parw02, 0, pal);
              RG2L = RG53l;
              RG4L = RG63L;
              dm2_dballochandler.DM2_dballoc_3e74_5992(lcon(0xd), RG4Blo, RG2Blo);
            }
          }
        }
        else
        {
          RG1L = signedlong(vo_2c.peek16());
          if (RG1L == lcon(0x4))
          {
            RG1L = signedlong(vql_04.peek16());
            RG2L = RG71L;
            RG4L = lcon(0xd);
            DM2_DRAW_TELEPORTER_TILE(RG1L, RG4L, RG2L);
          }
        }
      }

      //m_4192A:
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_08.peek16()))));
      vql_08.poke32(RG1L);
      if (RG1W == lcon(0xfffffffe))
      {
        vo_2c.inc16();
        break;
      }
    }
  }
  FOREND
}

// belongs to DM2_DRAW_ITEM
static i32 DM2_guivp_32cb_35c1(i16* eaxpw, i16* edxpw, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1P = DOWNCAST(i16, eaxpw);
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4P = DOWNCAST(i16, edxpw);
  c_wlreg _RG5;
  i8* RG6p;
  i8* RG7p;
  i32 vl_00;

  RG6p = RG1P;
  RG7p = RG4P;
  RG5L = RG2L;
  RG1L = RG3L;
  RG3W = word_at(RG6p);
  RG2W = word_at(RG4P);
  if (RG1W != 0)
  {
    RG1L = RG1L * lcon(0x5);
    RG2L -= RG1L;
    if (RG2W >= 0)
    {
      //m_41EDD:
      RG1L = signedlong(RG2W);
      if (RG1L > lcon(0x18))
      {
        RG1L = signedlong(RG3W);
        RG3W = signedword(table1d6a74[RG1L].barr_00[3]);
        RG2L -= lcon(0x14);
      }
    }
    else
    {
      RG1L = signedlong(RG3W);
      RG3W = signedword(table1d6a74[RG1L].barr_00[2]);
      RG2L += lcon(0x14);
    }
    //m_41EF4:
    if (RG3W < 0)
      return 0;
  }
  //m_41EFD:
  if (RG5W != 0)
  {
    RG4L = signedlong(RG2W);
    vl_00 = 5;
    RG1L = RG4L;
    RG4L = RG1L % vl_00;
    RG1L /= vl_00;
    RG1L = signedlong(RG5W);
    RG4L += RG1L;
    RG1L = RG4L;
    RG4L = signedlong(RG4W);
    if (RG4L <= lcon(0x4))
    {
      //m_41F36:
      if (RG1W >= 0)
        //m_41F4E:
        RG2L += RG5L;
      else
      {
        RG5L += 4;
        RG2L += RG5L;
        RG1L = signedlong(RG3W);
        RG3W = signedword(table1d6a74[RG1L].barr_00[0]);
      }
    }
    else
    {
      RG5L -= 4;
      RG2L += RG5L;
      RG1L = signedlong(RG3W);
      RG3W = signedword(table1d6a74[RG1L].barr_00[1]);
    }
    //m_41F50:
    if (RG3W < 0)
      return 0;
  }
  //m_41F55:
  mov16(location(RG6p), RG3W);
  mov16(location(RG7p + 0), RG2W);
  return 1;
}

// belongs to DM2_guivp_32cb_4185
// stacklen was 0x2c
static void DM2_guivp_32cb_4069(i32 eaxl, i32 edxl, i32 ebxl, i16* ecxpw, i16* argpw0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3P = DOWNCAST(i16, ecxpw);
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG52;
  c_wlreg _RG53;
  c_wlreg _RG54;
  c_wlreg _RG61;
  c_lreg _RG62;
  c_wlreg _RG63;
  c_wlreg _RG64;
  c_lreg _RG65;
  i32 vl_00;
  i16 vw_04;
  i32 vl_08;
  i16 vw_0c;
  i32 vl_10;
  c_ql32 vql_14;
  c_ql32 vql_18;
  i8 vb_1c; // set

  RG52L = RG1L;
  RG61L = RG4L;
  RG4P = DOWNCAST(i16, argpw0);
  if (RG2W == 0)
    return;
  RG1L = signedlong(RG2W);
  if (RG1L > lcon(0x3))
    RG2L = 3;
  RG1W = word_at(RG3P);
  vql_18.poke32(RG1L);
  RG1W = word_at(RG4P);
  vql_14.poke32(RG1L);
  RG2L--;
  RG1L = RG52L - vql_18.peek32();
  vw_04 = RG1W;
  RG1L = signedlong(DM2_ABS(vw_04));
  vl_10 = RG1L;
  if (RG1W != 0)
  {
    //m_42BB5:
    RG1L = vql_14.peek32() - RG61L;
    vw_04 = RG1W;
    RG1L = signedlong(DM2_ABS(vw_04));
    vw_0c = RG1W; // truncated
    if (RG1W == 0)
    {
      if (RG52W < vql_18.peek16())
      {
        RG1L = signedlong(RG2W);
        RG1W = signedword(table1d6ea8[RG1L]);
        sub16(location(RG3P), RG1W);
        return;
      }
      RG1L = signedlong(RG2W);
      RG1W = signedword(table1d6ea8[RG1L]);
      add16(location(RG3P), RG1W);
      return;
    }
    RG1Blo = RG61W < vql_14.peek16() ? 1 : 0;
    RG62L = RG1L & lcon(0xff);
    RG1Blo = RG52W < vql_18.peek16() ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    RG1L ^= RG62L;
    RG1Blo = RG1L == 0 ? 1 : 0;
    RG1W = unsignedword(RG1Blo);
    RG63L = vl_10;
    vb_1c = RG63W < vw_0c ? 1 : 0;
    vl_00 = unsignedlong(vb_1c);
    RG64L = signedlong(RG1W);
    RG64L ^= vl_00;
    vb_1c = RG64L == 0 ? 1 : 0;
    RG64W = unsignedword(vb_1c);
    vl_08 = RG64L;
    vb_1c = RG52W < vql_18.peek16() ? 1 : 0;
    RG53L = 0;
    RG53W = 4 * unsignedword(vb_1c);
    RG1L = 2 * RG1L | RG53L;
    RG54L = vl_08 | RG1L;
    RG65L = 16 * signedlong(RG2W);
    RG1L = signedlong(RG54W);
    RG2W = signedword(table1d6e68[RG65L / 0x10][RG1L].b_00);
    add16(location(RG3P), RG2W);
    RG1W = signedword(table1d6e68[RG65L / 0x10][RG1L].b_01);
  }
  else
  {
    if (RG61W < vql_14.peek16())
    {
      RG1L = signedlong(RG2W);
      RG1W = signedword(table1d6ea8[RG1L]);
      sub16(location(RG4P), RG1W);
      return;
    }
    RG1L = signedlong(RG2W);
    RG1W = signedword(table1d6ea8[RG1L]);
  }
  //m_42C8C:
  add16(location(RG4P), RG1W);
}

// belongs to DM2_DRAW_DOOR
static i32 DM2_guivp_32cb_48d5(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;

  RG2L = RG4L;
  RG5l = unsignedlong(RG1W);
  RG4L = RG5l << 7;
  RG3L = unsignedlong(RG2W);
  RG4L += lcon(0x40);
  RG1L = RG4L / RG3L / 2;
  RG3L = RG1L;
  RG4L = signedlong(RG1W);
  RG1L = signedlong(CUTX16(blitter.calc_stretched_size(RG2W, RG4W)));
  return RG1L >= RG5l ? RG3L : RG3L + 1;
}

// belongs to DM2_ENVIRONMENT_SET_DISTANT_ELEMENT
// stacksize was 0x10
static i32 DM2_guivp_32cb_54ce(i32 eaxl, i16* edxpw, i16* ebxpw, i32 ecxl, i32 argl0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = DOWNCAST(i16, ebxpw);
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4P = DOWNCAST(i16, edxpw);
  c_wlreg _RG5;
  c_wlreg _RG6;
  i16 vw_00;

  vw_00 = RG1W; // truncated
  if (RG1UW <= mkuw(lcon(0x3)))
  {
    RG1L = unsignedlong(vw_00);
    switch (RG1L)
    {
      case 0:
        //m_443BE:
        RG5W = word_at(RG2P);
        RG5L -= argl0;
        RG1W = word_at(RG4P);
        RG6L = RG3L - RG1L;
        break;

      case 1:
        //m_443CE:
        RG1W = word_at(RG4P);
        RG5L = RG3L - RG1L;
        RG6L = argl0;
        RG6W -= word_at(RG2P);
        break;

      case 2:
        //m_443DE:
        RG5L = argl0;
        RG5W -= word_at(RG2P);
        RG6W = word_at(RG4P);
        RG6L -= RG3L;
        break;

      case 3:
        //m_443EC:
        RG5W = word_at(RG4P);
        RG5L -= RG3L;
        RG6W = word_at(RG2P);
        RG6L -= argl0;
        break;

      default: throw(THROW_DMABORT);
    }
  }
  //m_443F8:
  mov16(location(RG4P), RG6W);
  mov16(location(RG2P), RG5W);
  RG1L = signedlong(RG5W);
  if (RG1L < 1)
    return 0;
  //m_4440A:
  RG6L *= RG6L;
  RG5L *= RG5L;
  RG5L += RG6L;
  RG1L = unsignedlong(RG5W);
  if (RG1L <= 2)
    return 1;
  //m_44423:
  RG1L >>= bcon(0x1);
  //m_44425:
  for (;;)
  {
    RG3L = RG1L;
    RG4L = unsignedlong(RG5W);
    RG2L = unsignedlong(RG1W);
    RG1L = RG4L;
    RG4L = RG1L % RG2L;
    RG1L /= RG2L;
    RG1L += RG2L;
    RG1L >>= bcon(0x1);
    if (RG1UW >= RG3UW)
      return RG3L;
  }
  FOREND
}

// belongs to DM2_DISPLAY_VIEWPORT
static void DM2_guivp_32cb_5a8f(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG5l;

  RG5l = 0;
  RG2L = 0;
  RG4L = 0;
  //m_44A5E:
  for (;;)
  {
    RG3L = signedlong(RG4W);
    if (RG3L >= lcon(0x10))
    {
      RG1L = RG2L;
      RG1UL >>= bcon(0x1);
      RG1L &= 0x3;
      RG5l |= table1d7042[RG1L];
      if ((RG2Blo & bcon(0x8)) == 0)
        //m_44A80:
        RG1L = 0;
      else
        RG1L = RG2L & lcon(0x6);
      break;
    }
    //m_44A3B:
    RG1L = 8 * RG3L;
    RG3L += RG1L;
    RG3L = 2 * RG3L;
    RG1P = ddat.ptr1e1044;
    if (word_at(RG1P + RG3L + 2) == 0)
    {
      RG3L = signedlong(RG4W);
      RG1L = 1 << RG3Blo;
      RG2L |= RG1L;
    }
    RG4L++;
  }
  //m_44A82:
  RG3L = RG2L;
  RG3UL >>= bcon(0x3);
  RG4L = RG3L & lcon(0x7);
  RG1L |= RG4L;
  RG5l |= table1d7052[RG1L];
  if ((RG2Blo & bcon(0x40)) == 0)
    //m_44AA1:
    RG1L = 0;
  else
    RG1L = RG3L & lcon(0x6);
  //m_44AA3:
  RG3L = RG2L;
  RG3UL >>= bcon(0x6);
  RG4L = RG3L & lcon(0x7);
  RG1L |= RG4L;
  RG5l |= table1d7072[RG1L];
  if ((RG2Bhi & bcon(0x8)) == 0)
    //m_44AC0:
    RG3L = 0;
  else
    RG3L &= 0x6;
  //m_44AC2:
  RG1L = RG2L;
  RG1UL >>= bcon(0xb);
  RG1L &= lcon(0x7);
  RG1L |= RG3L;
  RG5l |= table1d7092[RG1L];
  RG2L |= RG5l;
  RG1L = RG2L;
  RG1Blo ^= RG2Blo;
  RG1Bhi &= lcon(0x10);
  RG1L = unsignedlong(RG1W);
  if (RG1L != 0)
  {
    bool skip00975 = false;
    RG1L = RG2L;
    RG1Blo ^= RG2Blo;
    RG1Bhi &= lcon(0x40);
    RG1L = unsignedlong(RG1W);
    if (RG1L != 0)
      skip00975 = true;
    else
    {
      RG1L = RG2L;
      RG1Blo ^= RG2Blo;
      RG1Bhi &= 0x2;
      RG1L = unsignedlong(RG1W);
      if (RG1L != 0)
        skip00975 = true;
    }

    if (skip00975)
      //m_44AFF:
      RG5l |= lcon(0x280000);
  }
  //m_44B05:
  RG1L = RG2L;
  RG1Blo ^= RG2Blo;
  RG1Bhi &= lcon(0x20);
  RG1L = unsignedlong(RG1W);
  if (RG1L != 0)
  {
    bool skip00977 = false;
    RG1L = RG2L;
    RG1W &= lcon(0x8000);
    RG1L = unsignedlong(RG1W);
    if (RG1L != 0)
      skip00977 = true;
    else
    {
      RG2W &= lcon(0x400);
      RG1W = RG2W;
      if (RG1L != 0)
        skip00977 = true;
    }

    if (skip00977)
      //m_44B2D:
      RG5l |= lcon(0x500000);
  }
  //m_44B33:
  RG3L = 1;
  RG4L = 0;
  //m_44B61:
  for (;;)
  {
    RG2L = signedlong(RG4W);
    if (RG2L > lcon(0x16))
      return;
    //m_44B3C:
    if ((RG3L & RG5l) != 0)
    {
      RG1P = ddat.ptr1e1044 + 18 * RG2L;
      if (word_at(RG1P + 2) == 0)
        mov16(location(RG1P + 2), lcon(0x7));
    }
    RG3L = 2 * RG3L;
    RG4L++;
  }
  FOREND
}

// belongs to DM2_DISPLAY_VIEWPORT
static i32 DM2_guivp_32cb_5c67(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_5bytes c12_00;

  RG3L = 0;
  //m_44BE1:
  for (;;)
  {
    RG1L = signedlong(RG3W);
    if (RG1L >= lcon(0x15))
      return RG1L;
    //m_44B78:
    RG1P = ddat.ptr1e1044 + 18 * RG1L;
    RG4L = signedlong(word_at(RG1P, 2));
    if (RG4L == 1)
    {
      RG2L = signedlong(byte_at(RG1P, 1));
      RG4L = signedlong(byte_at(RG1P));
      RG1L = DM2_GET_TELEPORTER_DETAIL(&c12_00, RG4L, RG2L);
      if (RG1L != 0)
      {
        RG4L = 0;
        //m_44BD8:
        for (;;)
        {
          RG1L = signedlong(RG4W);
          if (RG1L >= lcon(0x4))
            break;
          //m_44BA8:
          RG1Blo = table1d6a74[signedlong(RG3W)].barr_00[RG1L];
          if ((RG1Blo & 0x80) == 0)
          {
            RG2L = unsignedlong(RG1Blo);
            RG1P = ddat.ptr1e1044 + 18 * RG2L;
            RG2L = signedlong(word_at(RG1P, 2));
            if (RG2L == lcon(0x7))
              mov16(location(RG1P + 2), 0);
          }
          //m_44BD7:
          RG4L++;
        }
      }
    }
    //m_44BE0:
    RG3L++;
  }
  FOREND
}

// belongs to DM2_DRAW_DOOR_FRAMES
static void DM2_DRAW_DEFAULT_DOOR_BUTTON(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  c_wlreg _RG6;
  i16 vw_00;
  i32 vl_04;
  i16 vw_08;
  i8 vb_0c;
  i8 vb_10;
  i8 vb_14;
  i16 parw01;
  i16 parw04;
  i8 parb06;
  i8 parb07;
  i8 parb08;
  bool skip00944 = false;

  vb_10 = RG1Blo;
  vb_0c = RG4Blo;
  vb_14 = RG2Blo;
  RG5w = RG3W;
  RG1L = signedlong(RG3W);
  RG6W = signedword(table1d6ed3[RG1L]);
  if (RG6W < 0)
    return;
  if (RG4Blo == lcon(0xffffffff))
    return;
  RG4W = signedword(table1d6b15[RG1L]);
  vw_08 = RG4W;
  RG1L = signedlong(table1d6b15[RG1L]);
  RG1Blo = table1d6b71[RG1L];
  RG1W = unsignedword(RG1Blo);
  vw_00 = RG1W;
  if (vb_10 != lcon(0xc))
  {
    //m_431FE:
    vl_04 = 0;
    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(vb_10, vb_0c, lcon(0xb), lcon(0x4));
    RG4W = RG1W;
    if (RG1W != 0)
      skip00944 = true;
  }
  else
  {
    RG1W = 5 * DM2_QUERY_GDAT_ENTRY_DATA_INDEX(12, vb_0c, 11, 8);
    vl_04 = mkl(RG1W);
  }

  if (!skip00944)
    //m_43226:
    RG4W = ddat.v1d6c00;

  //m_4322D:
  RG1L = unsignedlong(vb_14);
  put8(parb08, RG1Blo);
  RG1L = unsignedlong(vb_0c);
  put8(parb07, RG1Blo);
  RG1L = unsignedlong(vb_10);
  put8(parb06, RG1Blo);
  RG1L = signedlong(RG4W);
  put16(parw04, RG1W);
  RG1L = signedlong(CUTX16(vl_04 + lcon(0x79e) + RG6L)); // TODO: highword of RG6L is undefined!
  i32 parl02 = RG1L;
  RG1L = signedlong(vw_08);
  put16(parw01, RG1W);
  RG4L = signedlong(vw_00);
  DM2_QUERY_TEMP_PICST(
    0, RG4W, RG4W, 0,
    0, parw01, CUTX16(parl02), -1,
    parw04, lcon(0xffff), parb06, parb07, parb08);

  DM2_DRAW_TEMP_PICST();
  RG1L = signedlong(RG6W);
  if (RG1L != lcon(0x3))
  {
    if (RG1L != lcon(0x4))
      return;
  }
  RG2L = signedlong(RG5w);
  RG4L = 4;
  RG1R = &dm2_image2.rect;
  DM2_guivp_32cb_0a4c(RG1R, RG4L, RG2L);
}

// belongs to DM2_DRAW_PLAYER_TILE
// was SKW_32cb_0b11
static void DM2_DRAW_RAIN(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG52l;
  i16 RG6w;
  c_rect rc_00;
  i16 vw_08;
  i16 vw_0c;
  i16 vw_10;
  i8 vb_14;
  i16 parw02;
  i16 parw03;
  i16 parw04;

  if (ddat.v1e1482 == 0)
    return;

  RG1L = signedlong(ddat.v1e1472);
  if (table1d6b76[4 * RG1L + 0x72] == 0)
    return;

  vw_10 = gfxsys.backbuffer_w; // adj
  vw_08 = gfxsys.backbuffer_h; // adj
  RG3P16 = &vw_10;
  RG2L = lcon(0x2be);
  RG4R = &rc_00;
  RG1R = DM2_QUERY_BLIT_RECT(NULL, RG4R, RG2W, IO vw_10, IO vw_08);
  if (RG1R == NULL)
    return;
  DM2_QUERY_RAINFALL_PARAM(OO vb_14, OO vw_0c);
  RG2L = unsignedlong(vb_14);
  RG4L = unsignedlong(ddat.v1d6c02);
  t_bmp* srcbmp = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(23, RG4Blo, RG2Blo));
  dm2_dballochandler.DM2_ALLOCATE_GFX16(23, ddat.v1d6c02, vb_14);
  RG1L = signedlong(vw_0c);
  if (RG1L == 1)
  {
    RG1W = /*TAG dbidx*/ dm2_dballochandler.DM2_dballoc_3e74_5888();
    RG6w = RG1W;
    RG2L = signedlong(word_at(RG3P, -2));
    RG4L = signedlong(word_at(RG3P, -lcon(0x4)));
    RG52l = signedlong(RG1W);
    DM2_ALLOC_NEW_BMP(CUTX16(RG52l), RG4W, RG2W, BPP_4);
    RG2L = unsignedlong(vb_14);
    RG4L = unsignedlong(ddat.v1d6c02);
    DM2_DRAW_MIRRORED_PIC(BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(23, RG4Blo, RG2Blo)), dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ CUTX16(RG52l)));
    srcbmp = dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ CUTX16(RG52l));
  }
  //m_3E94D:
  RG2L = unsignedlong(vb_14);
  RG4L = unsignedlong(ddat.v1d6c02);
  t_palette* pal = DM2_guivp_32cb_0649(23, RG4Blo, RG2Blo, 0);
  s_dm2bmpheader* bmpheader = getbmpheader(gfxsys.bitmapptr);
  RG1L = signedlong(bmpheader->width);
  put16(parw04, RG1W);
  RG1L = DM2_RAND();
  RG1W = unsignedword(RG1Blo);
  RG1L = signedlong(RG1W);
  put16(parw03, RG1W);
  RG1W = getbmpheader(srcbmp)->width;
  RG4W = getbmpheader(srcbmp)->height;
  RG4L *= RG1L;
  RG4L -= lcon(0x28);
  RG1L = DM2_RAND();
  RG1W &= lcon(0x1f);
  RG4L -= RG1L;
  RG1L = signedlong(RG4W);
  put16(parw02, RG1W);
  RG3R = &rc_00;
  DM2_blit_specialeffects(PIXELCAST(srcbmp), PIXELCAST(gfxsys.bitmapptr), NULL, RG3R, parw02, parw03, parw04, 0, pal);
  RG1L = signedlong(vw_0c);
  if (RG1L == 1)
  {
    RG1L = signedlong(RG6w); // TODO: uninitialized if vw_0c changes in between, possible?
    dm2_dballochandler.DM2_dballoc_3e74_58a3((t_dbidx)RG1W);
  }
  dm2_dballochandler.DM2_dballoc_3e74_5992(lcon(0x17), ddat.v1d6c02, vb_14);
}

// was SKW_32cb_3672
// stacksize was 0x48
void DM2_DRAW_ITEM(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, unk* xargp1, i32 argl2, i16 argw3, i32 argl4)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  c_wlreg _RG6;
  i8* RG7p = xargp1;
  i16 vw_00;
  i16 vw_04;
  i32 vl_08;
  i16 vw_0c; vw_0c = 0; // TODO: BUG Problem, can reach it uninitialized
  i16 vw_10;
  i16 vw_14;
  i8 vb_18;
  i16 vw_1c;
  i16 vw_20;
  i16 vw_24;
  i8 vb_28;
  i8 vb_2c;
  i8 vb_30;
  i8 vb_34;
  i16 parw01;
  i8 parb06;
  i8 parb07;
  i8 parb08;
  bool skip00927 = false;

  vw_04 = RG1W; // truncated
  vw_00 = RG4W; // adj TODO: incoming RG4W word only
  RG6L = RG2L;
  RG5w = RG3W;
  if (RG7p == NULL)
  {
    //m_4203E:
    if (argl4 == 0)
    {
      RG4L = signedlong(ddat.v1e12c8);
      RG1L = DM2_QUERY_OBJECT_5x5_POS(unsignedlong(vw_04), RG4L);
      vw_0c = RG1W;
    }
  }
  else
  {
    RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG7p, lcon(0x4)))));
    RG1W = word_at(RG1P, lcon(0x1e)) & lcon(0xf);
    RG1L = unsignedlong(RG1W);
    RG1Blo = RG1L <= 1 ? 1 : 0;
    RG2L = unsignedlong(RG1Blo);
    RG1L = DM2_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(byte_at(RG7p, lcon(0x4))), lcon(0x41));
    RG4L = RG1L;
    if (RG1W == 0)
    {
      RG1L = signedlong(RG2W);
      RG4W = signedword(table1d6d86[RG1L]);
    }
    RG5w -= RG4W;
    RG4L = (unsignedlong(vw_04) >> bcon(0xe)) - signedlong(ddat.v1e12c8) & lcon(0x3);
    RG1L = signedlong(vw_00);
    if (RG1L != lcon(0x3) || ddat.v1e12ce == 0)
      //m_42008:
      vw_0c = lcon(0xc);
    else
    {
      RG1L = signedlong(ddat.v1d324a);
      RG1W = signedword(table1d6b76[RG1L + 0x42]);
      vw_0c = RG1W;
    }
    //m_4200F:
    RG1L = signedlong(RG2W);
    RG4L = signedlong(RG4W);
    RG3L = signedlong(table1d6d7e[RG1L][RG4L]);
    RG2L = signedlong(table1d6d76[RG1L][RG4L]);
    RG4P = ADRESSOF(i16, &vw_0c);
    RG1L = DM2_guivp_32cb_35c1(&vw_00, RG4P16, RG2L, RG3L);
    if (RG1L == 0)
      return;
  }
  //m_4205F:
  RG1L = signedlong(vw_00);
  RG1W = signedword(table1d6b15[RG1L]);
  vw_14 = RG1W;
  if (argl4 == 0)
  {
    //m_42102:
    RG4L = signedlong(vw_0c);
    RG2L = 5;
    RG1L = RG4L / RG2L;
    RG4L = lcon(0x4) - RG1L;
    vw_1c = RG4W; // truncated
    if (vw_00 == 0)
    {
      RG1L = unsignedlong(vw_1c);
      if (RG1L < 2)
        return;
    }
    RG4L = signedlong(vw_0c);
    RG1W = DM2_QUERY_CREATURE_BLIT_RECTI(vw_00, t_rotate(0), RG4W);
    RG1Bhi |= lcon(0xffffff80);
  }
  else
  {
    RG2L = 0;
    vw_1c = RG2W;
    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(dm2_image1.imgdesc.b_08, dm2_image1.imgdesc.b_09, lcon(0xc), lcon(0xfd));
    vb_18 = RG1Blo;
    RG1L = unsignedlong(RG1W) >> bcon(0x8);
    RG1L = signedlong(RG1Blo);
    RG2L = signedlong(dm2_image1.w_1c) + RG1L;
    RG4L = signedlong(argw0);
    RG1L = signedlong(table1d6b76[unsignedlong(table1d6b76[2 * RG4L + 0x22]) + 0x50]) + RG2L;
    RG6L = RG1L;
    RG1L = unsignedlong(table1d6b76[2 * RG4L + 0x23]);
    RG1W = signedword(table1d6b76[RG1L + 0x50]);
    RG5w = signedword(vb_18) + dm2_image1.w_1e + RG1W;
    RG1W = dm2_image1.query1;
  }
  //m_4214F:
  vw_10 = RG1W;
  vl_08 = 0;
  RG4L = unsignedlong(vw_04);
  RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG4L);
  RG2Blo = RG1Blo;
  vb_34 = RG1Blo;
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L);
  vb_28 = RG1Blo;
  if (RG2Blo == lcon(0x14))
  {
    RG1L = unsignedlong(RG1Blo);
    if (RG1L < lcon(0x8))
    {
      if (argw3 == 0)
      {
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
        RG1W = word_at(RG1P, lcon(0x4)) & 1;
        argw3 += RG1W;
      }
      vl_08 = 1;
    }
  }
  RG1L = signedlong(argw3);
  RG1Blo = table1d6d6e[RG1L].b_00;
  vb_30 = RG1Blo;
  RG2L = 0;
  vw_20 = RG2W; // truncated
  RG1L = signedlong(vw_00);
  RG1W = signedword(table1d6afe[RG1L]);
  if (RG1W != 0)
  {
    //m_4225D:
    if (vl_08 != 0)
    {
      RG1L = signedlong(RG1W);
      if (RG1L == 1)
        skip00927 = true;
    }
  }
  else
  {
    bool skip00926 = false;
    RG4L = signedlong(vw_0c);
    RG2L = 5;
    RG1L = RG4L;
    RG4L = RG1L % RG2L;
    RG1L /= RG2L;
    vw_24 = RG4W;
    RG1L = signedlong(vw_24);
    if (RG1L != 2 && argl4 == 0)
      skip00926 = true;
    else
    {
      RG1L = signedlong(argw3);
      RG1Blo = table1d6d6e[RG1L].b_01;
      vb_2c = RG1Blo;
      RG3L = unsignedlong(RG1Blo);
      RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(vb_34, vb_28, 1, RG3Blo) ? 1 : 0;
      if (RG1L == 0)
        skip00926 = true;
      else
      {
        RG1Blo = vb_2c;
        vb_30 = RG1Blo;
      }
    }

    if (skip00926)
    {
      //m_42241:
      if (vl_08 != 0 && argl4 == 0)
      {
        RG1L = signedlong(vw_24);
        if (RG1L > 2)
          skip00927 = true;
      }
    }
  }

  if (skip00927)
    //m_4226A:
    vw_20 = 1;

  //m_42272:
  RG4L = signedlong(argw0);
  if (RG4L != lcon(0xffffffff))
  {
    RG4L = 2 * RG4L;
    RG1L = table1d6b76[RG4L + 0x22];
    RG1W = signedword(table1d6b76[RG1L + 0x50]);
    RG6L += RG1L;
    if (RG7p == NULL)
    {
      RG1L = unsignedlong(table1d6b76[RG4L + 0x23]);
      RG1W = signedword(table1d6b76[RG1L + 0x50]);
      RG5w += RG1W;
    }
  }
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(vb_34, lcon(0xfe), 12, vb_30);
  vb_18 = RG1Blo;
  if (RG1W != 0)
  {
    RG1L = unsignedlong(RG1W) >> bcon(0x8);
    RG1Bhi = sgn8(RG1Blo);
    RG6L += RG1L;
    RG1W = signedword(vb_18);
    RG5w += RG1W;
  }
  RG1L = signedlong(vw_14);
  RG2L = 4 * RG1L;
  RG4L = unsignedlong(vw_1c) + RG2L;
  RG2L = unsignedlong(vb_30);
  put8(parb08, RG2Blo);
  RG2L = unsignedlong(vb_28);
  put8(parb07, RG2Blo);
  RG2L = unsignedlong(vb_34);
  put8(parb06, RG2Blo);
  i32 parl02 = signedlong(vw_10);
  put16(parw01, RG1W);
  RG1L = signedlong(RG5w);
  i32 parl00 = RG1L;
  RG3L = signedlong(RG6W);
  RG4Blo = table1d6b76[RG4L + 0x1];
  RG4L = unsignedlong(RG4Blo);
  DM2_QUERY_TEMP_PICST(
    vw_20, RG4W, RG4W, RG3W,
    CUTX16(parl00), parw01, CUTX16(parl02), 0,
    lcon(0xa), lcon(0xffff), parb06, parb07, parb08);

  if (argl2 == 0)
    dm2_image2.alphamask = 0xfffe;
  DM2_DRAW_TEMP_PICST();
}

// belongs to DM2_DRAW_DOOR
// was SKW_32cb_4715
static void DM2_DRAW_DOOR_FRAMES(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG51w;
  i32 RG52l;
  i8* RG6p;
  i32 RG7l;
  i32 vl_00;
  i16 vw_04;
  i16 vw_08;
  i16 vw_0c;
  i8 vb_10;
  i16 parw00;
  i16 parw06;
  i8 parb09;
  i8 parb10;
  i16 parw15;
  i8 parb18;
  i8 parb19;

  RG51w = RG1W;
  vl_00 = RG4L;
  RG1Blo = ddat.v1d6c02;
  vb_10 = RG1Blo;
  vw_0c = ddat.v1d6c00;
  RG7l = signedlong(RG51w);
  RG1L = 8 * RG7l;
  RG4L = 2 * (RG7l + RG1L);
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(word_at(ddat.ptr1e1044, RG4L + lcon(0xa)))));
  RG6p = RG1P;
  RG1W = word_at(RG1P, 2) & 1;
  RG4L = unsignedlong(RG1W);
  RG4Blo = ddat.v1e040e[RG4L];
  RG4L = unsignedlong(RG4Blo);
  RG3L = lcon(0x40);
  RG2L = lcon(0xb);
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0xe), RG4Blo, RG2Blo, RG3Blo);
  if (RG1W != 0)
    return;
  RG1L = vl_00;
  RG1W &= 0x1;
  RG1L = unsignedlong(RG1W);
  if (RG1L != 0)
  {
    RG1Blo = table1d6efd[RG7l];
    if (RG1Blo != lcon(0xffffffff))
    {
      RG4L = signedlong(vw_0c);
      put16(parw00, RG4W);
      RG3L = signedlong(table1d6f0b[RG7l]);
      RG2L = unsignedlong(RG1Blo);
      DM2_DRAW_DUNGEON_GRAPHIC(8, vb_10, RG2Blo, RG3W, parw00, 0);
    }
  }
  if (ddat.v1e12d0 == 0)
  {
    //m_4337E:
    vw_04 = RG51w;
    RG1L = 0;
    vw_08 = 1;
  }
  else
  {
    RG1L = signedlong(RG51w);
    RG1W = signedword(table1d6b2c[RG1L]);
    vw_04 = RG1W;
    RG1L = 1;
    RG3L = 0;
    vw_08 = RG3W; // truncated
  }
  //m_4338D:
  RG4L = vl_00;
  RG4W &= 0x2;
  RG4L = unsignedlong(RG4W);
  if (RG4L != 0)
  {
    RG4L = signedlong(vw_04);
    RG1L = unsignedlong(RG1W);
    RG1Blo = table1d6ee1[RG4L][RG1L];
    if (RG1Blo != lcon(0xffffffff))
    {
      RG1L = unsignedlong(RG1Blo);
      put8(parb10, RG1Blo);
      RG1L = unsignedlong(vb_10);
      put8(parb09, RG1Blo);
      RG1L = signedlong(vw_0c);
      put16(parw06, RG1W);
      RG1L = signedlong(DM2_QUERY_CREATURE_BLIT_RECTI(RG51w, t_rotate(0), lcon(0xa)));
      i32 parl04 = RG1L;
      RG3L = 0;
      RG2L = lcon(0x40);
      DM2_QUERY_TEMP_PICST(
        0, RG2W, RG2W, RG3W,
        0, 0, CUTX16(parl04), 4,
        parw06, lcon(0xffff), lcon(0x8), parb09, parb10);

      DM2_DRAW_TEMP_PICST();
    }
  }
  //m_433F9:
  RG1L = vl_00;
  RG1W &= 0x4;
  RG1L = unsignedlong(RG1W);
  if (RG1L == 0)
    return;
  RG4L = 2 * signedlong(vw_04);
  RG1L = unsignedlong(vw_08);
  RG1Blo = table1d6ee1[RG4L / 2][RG1L];
  if (RG1Blo == lcon(0xffffffff))
    return;
  RG1L = unsignedlong(RG1Blo);
  put8(parb19, RG1Blo);
  RG1L = unsignedlong(vb_10);
  put8(parb18, RG1Blo);
  RG1L = signedlong(vw_0c);
  put16(parw15, RG1W);
  RG52l = signedlong(RG51w);
  RG2L = 0;
  RG4L = lcon(0xe);
  RG1L = signedlong(DM2_QUERY_CREATURE_BLIT_RECTI(RG51w, t_rotate(RG2UW), RG4W));
  i32 parl13 = RG1L;
  RG3L = 0;
  RG2L = lcon(0x40);
  RG4L = RG2L;
  RG1L = 1;
  DM2_QUERY_TEMP_PICST(
    RG1W, RG4W, RG2W, RG3W,
    0, 0, CUTX16(parl13), 3,
    parw15, lcon(0xffff), lcon(0x8), parb18, parb19);

  DM2_DRAW_TEMP_PICST();
  if (jz_test8(location(RG6p + 2), lcon(0x40)))
  {
    //m_434A0:
    RG1P = ddat.ptr1e1044 + 18 * RG52l;
    if (word_at(RG1P + lcon(0xc)) == lcon(0xff))
      return;
    RG4L = signedlong(byte_at(RG1P, lcon(0xd))) + 1;
    RG2L = unsignedlong(RG4Blo);
    RG1Blo = byte_at(RG1P, lcon(0xc));
    RG4L = unsignedlong(RG1Blo);
    RG3L = RG52l;
    RG1L = lcon(0x9);
  }
  else
  {
    RG1W = word_at(RG6p, 2);
    RG1L <<= 4;
    RG1UW >>= bcon(0xf);
    RG4L = unsignedlong(RG1W);
    RG1L = 5 * RG4L;
    RG2L = unsignedlong(RG1Blo);
    RG3L = RG52l;
    RG4L = 0;
    RG1L = lcon(0xc);
  }
  //m_434D2:
  DM2_DRAW_DEFAULT_DOOR_BUTTON(RG1L, RG4L, RG2L, RG3L);
}

// was SKW_0cee_1dbe
void DM2_SUMMARIZE_STONE_ROOM(s_8wordarray* eaxp_s15, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1P = DOWNCAST(s_8wordarray, eaxp_s15);
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p;
  c_wlreg _RG61;
  c_wlreg _RG62;
  i32 RG71l;
  c_lreg _RG72;
  i32 vl_00;
  i32 vl_04;
  i8 vb_08;
  i16 vw_0c;
  i16 vw_10;
  i16 vw_14;
  i16 vw_18;
  i16 vw_1c;
  c_o32 vo_20; // poke 16 peek 8 16 inc16
  i16 vw_24;
  i32 vl_28;
  i16 vw_2c;
  i16 vw_30;
  i8 vb_34;
  i8 vb_38;
  i16 parw00;
  i16 parw02;
  i16 parw03;
  bool flag;
  bool skip00168 = false;
  bool skip00160 = false;
  bool skip00159 = false;
  bool skip00158 = false;
  bool skip00166 = false;
  bool skip00164 = false;

  RG5p = RG1P;
  vw_24 = RG4W;
  vw_18 = RG2W;
  vw_1c = RG3W;
  mov16(location(RG1P + lcon(0x6)), lcon(0xff));
  mov16(location(RG1P + lcon(0x8)), lcon(0xff));
  mov16(location(RG1P + lcon(0xa)), lcon(0xff));
  mov16(location(RG1P + lcon(0xc)), lcon(0xff));
  mov16(location(RG1P + lcon(0xe)), 0);
  RG3L = signedlong(vw_1c);
  RG71l = signedlong(vw_18);
  RG4L = RG3L;
  RG1L = dm2_GET_TILE_VALUE(RG71l, RG4L);
  RG2Blo = RG1Blo;
  vb_38 = RG1Blo;
  RG1L = unsignedlong(RG2Blo);
  mov16(location(RG5p + 2), RG1W);
  RG4L = RG3L;
  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(CUTX16(RG71l), RG4W));
  RG61L = RG1L;
  RG1L = unsignedlong(RG2Blo) >> 5;
  RG1W = unsignedword(RG1Blo);
  mov16(location(RG5p), RG1W);
  if (RG1UW > mkuw(lcon(0x6)))
  {
    mov16(location(RG5p + lcon(0x4)), RG61W);
    return;
  }
  RG1L = 4 * unsignedlong(RG1W);
  vl_00 = RG1L;
  RG1Blo = RG2Blo & lcon(0x4);
  RG2Bhi = RG2Blo & lcon(0x8);
  RG4W = vw_24;
  RG4W &= 0x1;
  vw_0c = RG4W;
  RG4L = vl_00;
  switch (RG4L >> 2)
  {
    case 0:
      //m_EA60:
      if (mkuw(vw_24) <= mkuw(lcon(0x3)))
      {
        bool skip00157 = false;
        RG3L = unsignedlong(vw_24);
        RG1Bhi = RG2Blo & 1;
        RG4Blo = RG2Blo & 2;
        switch (RG3L)
        {
          case 0:
            //m_EA85:
            RG2Blo = RG2Bhi;
            RG2W = unsignedword(RG2Blo);
            vw_2c = RG2W;
            RG2Blo = RG1Blo;
            vw_30 = RG2W;
            RG4W = unsignedword(RG4Blo);
            vw_10 = RG4W;
            RG1Blo = RG1Bhi;
            skip00157 = true;
            break;

          case 1:
            //m_EAA2:
            RG3L = unsignedlong(RG1Blo);
            vw_2c = RG3W;
            RG4W = unsignedword(RG4Blo);
            vw_30 = RG4W;
            RG1Blo = RG1Bhi;
            RG1W = unsignedword(RG1Blo);
            vw_10 = RG1W;
            RG1Blo = RG2Bhi;
            break;

          case 2:
            //m_EABF:
            RG4W = unsignedword(RG4Blo);
            vw_2c = RG4W;
            RG4Blo = RG1Bhi;
            vw_30 = RG4W;
            RG4Blo = RG2Bhi;
            vw_10 = RG4W;
            skip00157 = true;
            break;

          case 3:
            //m_EAD6:
            RG3L = unsignedlong(RG1Bhi);
            vw_2c = RG3W;
            RG2Blo = RG2Bhi;
            RG2W = unsignedword(RG2Blo);
            vw_30 = RG2W;
            RG1W = unsignedword(RG1Blo);
            vw_10 = RG1W;
            RG1Blo = RG4Blo;
            break;

          default: throw(THROW_DMABORT);
        }

        if (skip00157)
          //m_EA9E:
          RG1W = unsignedword(RG1Blo);

        //m_EAF1:
        vw_14 = RG1W;
      }
      skip00164 = true;
      break;

    case 1:
      skip00168 = true;
      break;

    case 2:
      //m_EBEB:
      RG4L = unsignedlong(RG2Bhi);
      if (RG4L == 0)
        skip00158 = true;
      else
      {
        RG1W = unsignedword(RG1Blo);
        mov16(location(RG5p + lcon(0x6)), RG1W);
      }
      break;

    case 3:
      //m_EEC9:
      RG1L = unsignedlong(RG2Bhi) / 8 & lcon(0xff);
      RG4L = signedlong(vw_0c);
      RG1Blo = RG1L != RG4L ? 1 : 0;
      RG1L = unsignedlong(RG1Blo) + lcon(0x12);
      mov16(location(RG5p), RG1W);
      RG1Blo = RG2Blo & lcon(0x4);
      RG1W = unsignedword(RG1Blo);
      mov16(location(RG5p + lcon(0x6)), RG1W);
      skip00160 = true;
      break;

    case 4:
      //m_EEFB:
      RG1Blo = RG2Blo & lcon(0x7);
      RG1W = unsignedword(RG1Blo);
      mov16(location(RG5p + lcon(0x6)), RG1W);
      RG4L = RG3L;
      RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(CUTX16(RG71l), RG4W));
      mov16(location(RG5p + lcon(0x8)), RG1W);
      RG1L = unsignedlong(RG2Bhi) / 8;
      RG4L = unsignedlong(RG1Blo);
      RG1L = signedlong(vw_0c);
      if (RG4L != RG1L)
      {
        //m_EF2F:
        mov16(location(RG5p), lcon(0x11));
        RG4W = vw_24 + 1;
        vw_24 = RG4W;
        RG2L = RG4L;
        RG2Bhi ^= RG4Bhi;
        RG2Blo &= 0x3;
        vw_24 = RG2W;
        RG2L = signedlong(vw_24);
        RG3W = vw_18;
        RG3W += table1d27fc[RG2L];
        RG4W = vw_1c;
        RG4W += table1d2804[RG2L];
        RG4L = signedlong(RG4W);
        RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(RG3W, RG4W));
        RG4L = unsignedlong(RG1W);
        RG3L = 1;
        RG1P = RG5p;
        DM2_query_0cee_1a46(UPCAST(s_8wordarray, RG1P), RG4L, RG2L, RG3L);
      }
      else
        mov16(location(RG5p), lcon(0x10));
      skip00160 = true;
      break;

    case 5:
      //m_EEA8:
      RG4L = unsignedlong(RG2Bhi);
      if (RG4L == 0)
        skip00159 = true;
      else
      {
        RG1L = unsignedlong(RG1Blo);
        if (RG1L == 0)
          skip00159 = true;
      }
      break;

    case 6:
      //m_EC6D:
      RG1L = unsignedlong(RG1Blo);
      if (RG1L != 0)
      {
        //m_EC83:
        vb_38 = 1;
        skip00158 = true;
      }
      else
      {
        mov16(location(RG5p), 0);
        RG71l = 1;
        skip00166 = true;
      }
      break;

    default: throw(THROW_DMABORT);
  }

  if (skip00160)
  {
    //m_EF85:
    for (;;)
    {
      RG1Blo = RG61W != wcon(0xfffe) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L != 0)
      {
        RG1L = RG61L;
        RG1W &= lcon(0x3c00);
        RG1L = unsignedlong(RG1W) >> bcon(0xa);
        if (RG1L <= lcon(0x3))
        {
          RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG61UW)));
          RG61L = RG1L;
          continue;
        }
      }
      break;
    }
    mov16(location(RG5p + lcon(0x4)), RG61W);
    return;
  }

  if (skip00159)
  {
    //m_EEBB:
    mov16(location(RG5p), 1);
    and8(location(RG5p + 2), lcon(0xfffffff7));
  }

  if (skip00158)
  {
    //m_EBFE:
    mov16(location(RG5p), 1);
    skip00168 = true;
  }

  if (skip00164)
  {
    //m_EAF6:
    RG1L = signedlong(vw_1c);
    put16(parw03, RG1W);
    RG1L = signedlong(vw_18);
    put16(parw02, RG1W);
    i32 parl01 = signedlong(vw_24);
    RG1L = signedlong(vw_14);
    put16(parw00, RG1W);
    DM2_map_0cee_185a(RG5p, signedlong(vw_2c), signedlong(vw_30), signedlong(vw_10), parw00, parl01, parw02, parw03);
    RG71l = 0;
    skip00166 = true;
  }

  if (skip00166)
  {
    //m_EB34:
    RG2L = signedlong(vw_24);
    RG4L = unsignedlong(RG61W);
    RG3L = 0;
    RG1L = DM2_query_0cee_1a46(UPCAST(s_8wordarray, RG5p), RG4L, RG2L, RG3L);
    RG62L = RG1L;
    if (RG71l != 0)
    {
      RG1W = ddat.v1e0270;
      if (RG1W != vw_18)
      {
        RG1W = ddat.v1e0272;
        if (RG1W != vw_1c)
        {
          mov16(location(RG5p + lcon(0x4)), lcon(0xfffffffe));
          return;
        }
      }
    }
    else
    {
      RG3L = 0;
      vo_20.poke16(RG3W);
      //m_EBD4:
      for (;;)
      {
        RG2L = unsignedlong(vo_20.peek16());
        if (RG2L >= lcon(0x4))
          break;
        //m_EB85:
        RG1L = signedlong(vw_24) + RG2L & lcon(0x3);
        RG3W = vw_18;
        RG3W += table1d27fc[RG1L];
        RG4W = vw_1c;
        RG4W += table1d2804[RG1L];
        RG4L = signedlong(RG4W);
        RG1L = (dm2_GET_TILE_VALUE(signedlong(RG3W), RG4L) & lcon(0xff)) >> 5 & lcon(0xff);
        if (RG1L == lcon(0x4))
          mov16(location(RG5p + 2 * RG2L + lcon(0x6)), lcon(0xff));
        vo_20.inc16();
      }
    }
    mov16(location(RG5p + lcon(0x4)), RG62W);
    return;
  }

  if (skip00168)
  {
    //m_EC03:
    RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
    RG1W = word_at(RG1P, lcon(0xe));
    RG1L <<= bcon(0x8);
    RG1UW >>= bcon(0xc);
    RG4L = unsignedlong(RG1Blo);
    RG3L = lcon(0x6b);
    RG2L = lcon(0xb);
    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0x8), RG4Blo, lcon(0xb), lcon(0x6b));
    vo_20.poke16(RG1W);
    if (RG1W != 0)
    {
      RG1W &= lcon(0x8000);
      RG1L = unsignedlong(RG1W);
      if (RG1L == 0)
      {
        //m_EC8D:
        RG1L = unsignedlong(vo_20.peek8());
        mov16(location(RG5p + lcon(0xa)), RG1W);
      }
      else
      {
        RG1L = DM2_QUERY_ORNATE_ANIM_FRAME(lcon(0xa), unsignedlong(vo_20.peek8()), timdat.gametick, 0) * lcon(0xa) << bcon(0x8);
        RG4L = unsignedlong(vo_20.peek8()) | mkul(RG1L);
        mov16(location(RG5p + lcon(0xa)), RG4W);
      }
    }
  }

  //m_EC97:
  for (;;)
  {
    bool skip00153 = false;
    bool skip00154 = false;
    bool skip00155 = false;
    RG1Blo = RG61W != wcon(0xfffe) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == 0)
      break;
    RG1L = RG61L;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    vw_24 = RG1W;
    RG3L = signedlong(vw_24);
    if (RG3L > lcon(0x3))
      break;
    RG2L = 0;
    vl_28 = 0;
    RG72L = timdat.gametick;
    vl_04 = RG2L;
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG61UW));
    RG4P = RG1P;
    if (RG3L != 2)
    {
      //m_EDC0:
      if (RG3L == lcon(0x3))
      {
        RG1W = word_at(RG1P, 2) & lcon(0x7f);
        if (RG1W == lcon(0x30))
          mov16(location(RG5p + lcon(0xc)), 0);
        RG1Blo = DM2_GET_FLOOR_DECORATION_OF_ACTUATOR(RG4P);
        vb_34 = RG1Blo;
        if (RG1Blo != lcon(0xffffffff))
        {
          RG1W = word_at(RG4P, 2) & lcon(0x7f);
          if (RG1UW < mkuw(lcon(0x2c)))
          {
            //m_EE08:
            if (RG1W == lcon(0x27))
            {
              //m_EE23:
              RG1W = word_at(RG4P, 2);
              RG1UW >>= bcon(0x7);
              RG1L = unsignedlong(RG1W);
              RG3L = signedlong(ddat.v1d3248);
              RG1L--;
              if (RG1L == RG3L)
              {
                flag = jz_test8(location(RG4P + lcon(0x4)), 1);
                skip00153 = true;
              }
            }
            else
              skip00155 = true;
          }
          else
          {
            if (RG1UW <= mkuw(lcon(0x2c)))
            {
              //m_EE47:
              if (jz_test8(location(RG4P + lcon(0x4)), 1))
                skip00155 = true;
              else
              {
                RG1W = word_at(RG4P, 2);
                RG1UW >>= bcon(0x7);
                RG1W = unsignedword(RG1Blo);
                RG1L = unsignedlong(RG1W);
                vl_04 = RG1L;
                skip00154 = true;
              }
            }
            else
            {
              if (RG1W == lcon(0x32))
              {
                //m_EE10:
                if (jz_test8(location(RG4P + lcon(0x4)), 1))
                  skip00155 = true;
                else
                {
                  RG1W = word_at(RG4P, 2);
                  RG1UW >>= bcon(0x7);
                  RG72L = unsignedlong(RG1W);
                  skip00154 = true;
                }
              }
              else
                skip00155 = true;
            }
          }
        }
      }
    }
    else
    {
      RG4W = word_at(RG1P, 2) & lcon(0x6);
      if (RG4W == 2)
      {
        RG1W = word_at(RG1P, 2);
        RG4L = RG1L;
        RG4UW >>= bcon(0x3);
        RG3L = RG4L;
        RG3Bhi ^= RG4Bhi;
        vb_08 = RG3Blo;
        RG3Blo = vb_08;
        vb_34 = RG3Blo;
        RG4L = unsignedlong(RG4W);
        RG4UL >>= bcon(0x8);
        RG4L &= lcon(0x1f);
        if (mkul(RG4L) < mkul(lcon(0xa)))
        {
          //m_ED5D:
          if (mkul(RG4L) < mkul(2))
          {
            //m_ED72:
            if (RG4L == 0)
              skip00155 = true;
          }
          else
          {
            if (mkul(RG4L) <= mkul(2))
              skip00154 = true;
            else
            {
              if (RG4L == lcon(0x5))
              {
                //m_ED9B:
                flag = (RG1Blo & bcon(0x1)) == 0;
                skip00153 = true;
              }
            }
          }
        }
        else
        {
          if (mkul(RG4L) <= mkul(lcon(0xa)))
            skip00155 = true;
          else
          {
            if (mkul(RG4L) < mkul(lcon(0x10)))
            {
              //m_ED53:
              if (RG4L == lcon(0xf))
              {
                //m_EDA8:
                vl_28 = 1;
                skip00155 = true;
              }
            }
            else
            {
              if (mkul(RG4L) <= mkul(lcon(0x10)))
                skip00155 = true;
              else
              {
                if (mkul(RG4L) <= mkul(lcon(0x11)))
                {
                  //m_ED7F:
                  RG1Blo = vb_38 & lcon(0x8);
                  RG1L = unsignedlong(RG1Blo);
                  RG1Blo = RG1L != 0 ? 1 : 0;
                  RG1L = unsignedlong(RG1Blo);
                  vl_28 = RG1L;
                  skip00155 = true;
                }
                else
                {
                  if (RG4L == lcon(0x14))
                    //m_EDB5:
                    mov16(location(RG5p + lcon(0xc)), 0);
                }
              }
            }
          }
        }
      }
    }

    if (skip00153)
    {
      //m_ED9D:
      if (flag)
        skip00155 = true;
      else
        skip00154 = true;
    }

    if (skip00154)
    {
      //m_EE60:
      RG2L = 1;
      skip00155 = true;
    }

    if (skip00155)
    {
      //m_EE65:
      if (RG2L != 0)
        vl_28 = DM2_QUERY_ORNATE_ANIM_FRAME(lcon(0xa), unsignedlong(vb_34), RG72L, vl_04);
      RG1L = vl_28 * lcon(0xa) << bcon(0x8);
      RG4L = unsignedlong(vb_34) | mkul(RG1L);
      mov16(location(RG5p + lcon(0xa)), RG4W);
    }

    //m_EE97:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG61UW)));
    RG61L = RG1L;
  }
  mov16(location(RG5p + lcon(0x4)), RG61W);
}

static void DM2_guivp_32cb_4185(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_wlreg _RG6;
  i32 RG7l;
  c_ql32 vql_00;
  c_ql32 vql_04;
  c_ql32 vql_08;
  i16 vw_0c;
  i16 vw_10;
  i16 vw_14;
  i16 vw_18;
  i16 vw_1c;
  i16 vw_20;

  vql_08.poke32(RG1L);
  vql_04.poke32(RG4L);
  vql_00.poke32(RG2L);
  RG6L = RG3L;
  vw_1c = RG1W;
  vw_10 = RG4W;
  RG7l = signedlong(vql_00.peek16());
  RG2L = signedlong(table1d6ad0[RG7l][0]);
  RG4L = signedlong(table1d6ad0[RG7l][1]);
  RG5L = signedlong(RG3W);
  DM2_CALC_VECTOR_W_DIR(RG5W, RG4W, RG2W, &vw_1c, &vw_10);
  RG1L = 8 * RG7l;
  RG4L = 2 * (RG7l + RG1L);
  RG2P = ddat.ptr1e1044;
  RG1Blo = mkb(vw_1c); // truncated
  mov8(location(RG2P + RG4L), RG1Blo);
  RG2P = ddat.ptr1e1044;
  RG1Blo = mkb(vw_10); // truncated
  mov8(location(RG2P + RG4L + 1), RG1Blo);
  RG3L = signedlong(vw_10);
  RG2L = signedlong(vw_1c);
  RG1P = ddat.ptr1e1044 + RG4L + 2;
  DM2_SUMMARIZE_STONE_ROOM(UPCAST(s_8wordarray, RG1P), RG5L, RG2L, RG3L);
  if (ddat.v1e025c != 0)
  {
    RG2L = vql_00.peek32();
    if (RG2W == 0)
    {
      RG1P = ddat.ptr1e1044;
      if (word_at(RG1P + 2) != 0)
      {
        //m_42E70:
        RG4L = signedlong(word_at(RG1P, 2));
        if (RG4L == lcon(0x11))
        {
          mov16(location(RG1P + 2), 1);
          mov16(location(ddat.ptr1e1044 + lcon(0x4)), RG2W);
          mov16(location(ddat.ptr1e1044 + lcon(0xc)), lcon(0xff));
        }
      }
      else
      {
        RG4L = signedlong(word_at(RG1P, lcon(0x38)));
        if (RG4L != lcon(0x5))
        {
          //m_42D6B:
          mov16(location(RG1P + lcon(0x8)), lcon(0xff));
          mov16(location(ddat.ptr1e1044 + lcon(0xa)), lcon(0xff));
          mov16(location(ddat.ptr1e1044 + lcon(0xc)), lcon(0xff));
          mov16(location(ddat.ptr1e1044 + lcon(0xe)), lcon(0xff));
          mov16(location(ddat.ptr1e1044 + lcon(0x10)), RG2W);
          mov16(location(ddat.ptr1e1044 + lcon(0x6)), lcon(0xfffffffe));
        }
        else
          mov16(location(RG1P + 2), lcon(0x5));
        //m_42DA6:
        RG5L = 1;
        //m_42E62:
        for (;;)
        {
          bool skip00938 = false;
          RG4L = signedlong(RG5W);
          if (RG4L > 2)
            break;
          //m_42DB0:
          RG1P = ddat.ptr1e1044 + 18 * RG4L + 2;
          mov16(location(RG1P + lcon(0x4)), lcon(0xfffffffe));
          RG2L = RG4L + lcon(0x3);
          RG4L = 8 * RG2L;
          RG2L += RG4L;
          RG2L = 2 * RG2L;
          RG4P = ddat.ptr1e1044;
          RG4W = word_at(RG4P, RG2L + lcon(0x4)) & lcon(0xe0);
          mov16(location(RG1P + 2), RG4W);
          RG4P = ddat.ptr1e1044;
          RG4W = word_at(RG4P, RG2L + 2);
          mov16(location(RG1P), RG4W);
          if (RG4UW < mkuw(2))
          {
            //m_42E10:
            if (RG4W == 0)
            {
              mov16(location(RG1P + lcon(0xc)), lcon(0xff));
              RG4W = word_at(RG1P, lcon(0xc));
              mov16(location(RG1P + lcon(0xa)), RG4W);
              mov16(location(RG1P + lcon(0x8)), RG4W);
              mov16(location(RG1P + lcon(0x6)), RG4W);
              mov16(location(RG1P + lcon(0xe)), lcon(0xffffffff));
              skip00938 = true;
            }
          }
          else
          {
            if (RG4UW <= mkuw(2))
            {
              //m_42E33:
              mov16(location(RG1P), 1);
              mov16(location(RG1P + 2), 0);
              skip00938 = true;
            }
            else
            {
              if (RG4UW < mkuw(lcon(0x5)))
                skip00938 = true;
              else
              {
                if (RG4UW > mkuw(lcon(0x5)))
                {
                  if (RG4W != lcon(0x10))
                    skip00938 = true;
                }
              }
            }
          }

          if (!skip00938)
            //m_42E40:
            mov16(location(RG1P + lcon(0xa)), lcon(0xff));

          //m_42E46:
          RG4L = signedlong(RG5W);
          mov32(location(ddat.v1e1038 + 4 * RG4L), 0);
          mov32(location(ddat.v1e104c + 4 * RG4L), 0);
          RG5L++;
        }
      }
    }
  }
  //m_42E94:
  RG4L = signedlong(vql_00.peek16());
  RG2L = RG4L;
  mov32(location(ddat.v1e1038 + 4 * RG2L), 0);
  mov32(location(ddat.v1e104c + 4 * RG2L), 0);
  if (RG4L < lcon(0x10))
  {
    RG1L = 9 * RG4L;
    RG2P = ddat.ptr1e1044;
    if (word_at(RG2P + 2 * RG1L + 2) != 0)
    {
      RG2L = unsignedlong(table1d6eb3[RG4L].b_00);
      RG1P = ddat.v1e1048 + 17 * RG2L;
      RG2P = RG1P + unsignedlong(table1d6eb3[RG4L].b_01);
      RG5L = 0;
      RG3Blo = vql_00.peek8();
      //m_42F0D:
      for (;;)
      {
        RG1L = signedlong(RG5W);
        if (RG1L >= lcon(0x5))
          break;
        //m_42EF9:
        RG1L = 0;
        //m_42F01:
        for (;;)
        {
          RG4L = signedlong(RG1W);
          if (RG4L >= lcon(0x4))
          {
            RG2P += lcon(0x11);
            RG5L++;
            break;
          }
          //m_42EFD:
          mov8(location(RG2P + RG4L), RG3Blo);
          RG1L++;
        }
      }
    }
  }
  //m_42F15:
  RG4L = signedlong(vql_00.peek16());
  RG1L = 9 * RG4L;
  RG5W = word_at(ddat.ptr1e1044, 2 * RG1L + lcon(0x6));
  if (RG5W == lcon(0xfffffffe))
    return;
  RG2L = signedlong(table1d6eaf[signedlong(RG6W)]);
  RG4L = signedlong(vql_04.peek16());
  DM2_query_098d_000f(vql_08.peek16(), RG4W, RG2W, OO vw_0c, OO vw_18);
  //m_42F60:
  for (;;)
  {
    RG1L = RG5L;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    RG1L = signedlong(RG1W);
    if (RG1L != lcon(0x4))
    {
      bool skip00941 = false;
      //m_430EB:
      if (RG1L != lcon(0xe))
      {
        //m_43102:
        if (RG1L >= lcon(0x5) && RG1L <= lcon(0xa))
        {
          RG4L = signedlong(vql_00.peek16());
          RG1P = ddat.ptr1e1044 + 18 * RG4L;
          RG4L = signedlong(word_at(RG1P, 2));
          if (RG4L != 1 || word_at(RG1P + lcon(0xe)) != 0)
          {
            RG1L = signedlong(vql_00.peek16()) << 2;
            RG2P = ddat.v1e104c;
            skip00941 = true;
          }
        }
      }
      else
      {
        RG1L = signedlong(vql_00.peek16()) << 2;
        RG2P = ddat.v1e1038;
        skip00941 = true;
      }

      if (skip00941)
      {
        //m_43145:
        RG2P += RG1L;
        RG4L = signedlong(ddat.v1e12c8);
        RG1L = DM2_QUERY_OBJECT_5x5_POS(unsignedlong(RG5W), RG4L);
        RG3L = RG1L;
        RG1L = 1 << RG3Blo;
        or32(location(RG2P), RG1L);
      }
    }
    else
    {
      bool skip00940 = false;
      if (ddat.v1e12ce != 0)
      {
        RG1L = signedlong(vql_00.peek16());
        if (RG1L == lcon(0x3))
        {
          RG1L = signedlong(ddat.v1d324a);
          RG1W = signedword(table1d6b76[RG1L + 0x42]);
          RG4L = signedlong(RG6W);
          RG4Blo = table1d6eab[RG4L];
          RG4L = unsignedlong(RG4Blo);
          RG1W = DM2_ROTATE_5x5_POS(RG1W, t_rotate(RG4UW));
          skip00940 = true;
        }
      }

      if (!skip00940)
        //m_42FB6:
        RG1L = DM2_QUERY_OBJECT_5x5_POS(unsignedlong(RG5W), 0);

      //m_42FC2:
      RG2L = unsignedlong(RG1W);
      RG4L = signedlong(vw_10);
      DM2_query_098d_000f(vw_1c, RG4W, RG2W, OO vw_14, OO vw_20);
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG5UW));
      if (vql_00.peek16() != 0)
      {
        RG2L = signedlong(DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG1P, lcon(0x4))))->b_22);
        RG4L = signedlong(vw_18);
        RG1L = signedlong(vw_0c);
        RG3P = ADRESSOF(i16, &vw_14);
        DM2_guivp_32cb_4069(RG1L, RG4L, RG2L, RG3P16, &vw_20);
      }
      //m_43023:
      if (RG6UW > mkuw(lcon(0x3)))
      {
        RG3W = vw_18 - vw_20; // adj
        RG4W = vw_0c - vw_14; // adj
      }
      else
      {
        RG1L = unsignedlong(RG6W);
        RG2W = vw_14 - vw_0c; // adj
        RG4W = vw_20 - vw_18; // adj
        switch (RG1L)
        {
          case 0:
            //m_43046:
            RG3W = RG2W; // adj
            RG4W = vw_18 - vw_20; // adj
            break;

          case 1:
            //m_43052:
            RG3W = RG4W; // adj
            RG4W = RG2W; // adj
            break;

          case 2:
            //m_43058:
            RG3W = vw_0c - vw_14; // adj
            break;

          case 3:
            RG3W = vw_18 - vw_20; // adj
            RG4W = vw_0c - vw_14; // adj
            break;

          default: throw(THROW_DMABORT);
        }
      }
      //m_43072:
      RG1L = signedlong(RG4W);
      if (RG1L < lcon(0x11))
      {
        RG1L = signedlong(DM2_ABS(RG3W));
        if (RG1L < lcon(0xa))
        {
          //m_43090:
          for (;;)
          {
            if (RG4W >= 0)
            {
              RG2L = signedlong(RG3W) + lcon(0xa);
              RG1L = 16 * RG2L;
              RG2L += RG1L;
              RG2P = ddat.v1e1050 + RG2L;
              RG1L = signedlong(RG4W);
              RG1Blo = byte_at(RG2P, RG1L);
              RG1L = unsignedlong(RG1Blo);
              if (RG1L != lcon(0xff))
              {
                RG4L--;
                continue;
              }
            }
            break;
          }
          //m_430BE:
          if (RG4W >= 0)
          {
            RG1L = signedlong(RG3W);
            RG2L = RG1L + lcon(0xa);
            RG1L = 17 * RG2L;
            RG2P = ddat.v1e1050;
            RG1P = RG2P + RG1L;
            RG4L = signedlong(RG4W);
            RG1P += RG4L;
            RG4Blo = vql_00.peek8();
            mov8(location(RG1P), RG4Blo);
          }
        }
      }
    }
    //m_43165:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG5UW)));
    RG5L = RG1L;
    if (RG1W == lcon(0xfffffffe))
      return;
  }
  FOREND
}

// belongs to DM2_DRAW_STATIC_OBJECT
// was SKW_32cb_3143
static void DM2_DRAW_FLYING_ITEM(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_wlreg _RG6;
  i32 RG7l;
  c_ql32 vql_00;
  c_ql32 vql_04;
  i16 vw_08;
  i16 vw_0c;
  i16 vw_10;
  i32 vl_14;
  i16 vw_18;
  i16 vw_1c;
  c_o32 vo_20; // poke 16 peek 32 xor8 or8
  i32 vl_24;
  i16 vw_28;
  i8 vb_2c;
  i8 vb_30;
  i8 vb_34;
  i16 parw06;
  i8 parb11;
  i8 parb12;
  i8 parb13;
  bool flag;

  vw_08 = RG1W; // truncated
  vql_04.poke32(RG4L);
  vql_00.poke32(RG2L);
  if (RG1W == lcon(0xfffffffe))
    return;
  RG1L = DM2_DIR_FROM_5x5_POS(unsignedlong(RG2W));
  RG5L = RG1L;
  RG1L = signedlong(RG1W);
  if (RG1L == lcon(0xffffffff))
    return;
  RG4L = signedlong(vql_04.peek16());
  RG4W = signedword(table1d6b15[RG4L]);
  vw_28 = RG4W;
  if (RG4W == 0)
  {
    if (RG1L >= 2)
      return;
  }
  RG1L = signedlong(vql_04.peek16());
  RG4W = signedword(table1d6afe[RG1L]);
  vw_0c = RG4W;
  if (table1d6b43[RG1L] < bcon(0x0))
    return;
  RG4L = RG1L;
  RG1L *= 8;
  RG1L += RG4L;
  RG1P = ddat.ptr1e1044 + 2 * RG1L;
  RG4W = signedword(byte_at(RG1P));
  vw_10 = RG4W;
  RG1W = signedword(byte_at(RG1P, 1));
  vw_1c = RG1W;
  RG7l = lcon(0xff);
  //m_419F7:
  for (;;)
  {
    RG4L = signedlong(ddat.v1e12c8);
    RG3L = unsignedlong(vw_08);
    RG1L = DM2_QUERY_OBJECT_5x5_POS(RG3L, RG4L) & lcon(0xffff);
    RG4L = signedlong(vql_00.peek16());
    if (RG4L == RG1L)
    {
      RG1L = RG3L;
      RG1Blo ^= RG3Blo;
      RG1Bhi &= lcon(0x3c);
      RG1L = unsignedlong(RG1W) >> bcon(0xa);
      if (RG1L == lcon(0xe))
      {
        RG6L = signedlong(vql_04.peek16());
        RG1W = DM2_QUERY_CREATURE_BLIT_RECTI(vql_04.peek16(), t_rotate(0), RG4W);
        vl_24 = RG1L;
        RG1L = DM2_query_48ae_011a(RG3L);
        vl_14 = RG1L;
        if (RG1W != lcon(0xffffffff))
        {
          bool skip00913 = false;
          bool skip00914 = false;
          bool skip00915 = false;
          //m_41A9C:
          RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
          RG2P = RG1P;
          RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG3L);
          RG4Blo = RG1Blo;
          vb_2c = RG1Blo;
          RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG3L);
          vb_34 = RG1Blo;
          RG1Blo = RG4Blo == bcon(0xd) ? 1 : 0;
          RG1L = unsignedlong(RG1Blo);
          flag = RG1L == 0;
          RG3L = RG1L;
          if (flag)
            skip00913 = true;
          else
          {
            RG1L = unsignedlong(byte_at(RG2P, lcon(0x4)));
            if (RG1L == lcon(0xff))
              skip00913 = true;
          }

          if (skip00913)
          {
            //m_41AD8:
            if (vw_28 != 0)
              skip00914 = true;
            else
            {
              RG1L = lcon(0x40);
              vw_18 = RG1W;
              RG6L = RG1L;
              skip00915 = true;
            }
          }
          else
            skip00914 = true;

          if (skip00914)
          {
            //m_41AF1:
            RG1L = 2 * signedlong(vw_28);
            RG4L = signedlong(RG5W) / 2;
            RG1L -= RG4L;
            RG6L = RG1L;
            if (RG1W >= 0)
            {
              RG1L = signedlong(RG1W);
              RG1Blo = table1d6b76[RG1L + 0x1b];
              RG1W = unsignedword(RG1Blo);
              vw_18 = RG1W;
              RG6L = RG1L;
              if (RG3L != 0)
              {
                RG1Blo = byte_at(RG2P, lcon(0x4));
                RG4L = unsignedlong(word_at(RG2P, 2));
                if (RG4L == lcon(0xff82))
                  RG1L = signedlong(RG1W) / 2 + lcon(0x80);
                RG3L = signedlong(vw_18);
                RG4L = unsignedlong(RG1W) << 7;
                RG1L = RG4L;
                RG4L = RG1L % RG7l;
                RG1L /= RG7l;
                RG1L++;
                RG1L >>= bcon(0x1);
                RG1L = blitter.calc_stretched_size(RG1W, RG3W);
                RG4L = signedlong(RG1W);
                RG1L = signedlong(DM2_MAX(8, RG4W));
                vw_18 = RG1W;
              }
              skip00915 = true;
            }
          }

          if (skip00915)
          {
            //m_41B6C:
            RG4L = 0;
            vo_20.poke16(RG4W);
            RG3L = vl_14;
            if (RG3W != lcon(0x3))
            {
              bool skip00923 = false;
              //m_41B87:
              RG1W = timdat.timerarray[word_at(RG2P, lcon(0x6))].getB();
              RG1L <<= 4;
              RG1UW >>= 14;
              RG4L = RG1L;
              RG4Bhi ^= RG1Bhi;
              RG4Blo &= 0x1;
              RG2L = signedlong(RG4W);
              RG4W = ddat.v1e12c8 & 1;
              RG4L = signedlong(RG4W);
              if (RG2L == RG4L)
              {
                bool skip00918 = false;
                //m_41C22:
                if (RG3W != 0)
                {
                  //m_41C62:
                  if (RG3W == 2)
                    skip00918 = true;
                  else
                  {
                    if (RG3W == 1 && RG1W != ddat.v1e12c8)
                      skip00918 = true;
                    else
                      vb_30 = lcon(0xa);
                  }
                }
                else
                {
                  bool skip00919 = false;
                  RG4L = signedlong(vw_10);
                  RG1L = signedlong(vw_1c) + RG4L;
                  if ((RG1Blo & bcon(0x1)) == 0)
                  {
                    //m_41C58:
                    RG1L = signedlong(RG5W);
                    if (RG1L < 2)
                      skip00919 = true;
                  }
                  else
                  {
                    vo_20.poke16(2);
                    RG1L = signedlong(RG5W);
                    if (RG1L >= 2)
                      skip00919 = true;
                  }

                  if (skip00919)
                    //m_41C51:
                    vb_30 = lcon(0x9);
                  else
                    skip00918 = true;
                }

                if (skip00918)
                  //m_41C4A:
                  vb_30 = lcon(0x8);

                //m_41C7C:
                RG1L = signedlong(vw_0c);
                if (RG1L < 1)
                {
                  bool skip00921 = false;
                  if (vw_0c != 0)
                    skip00921 = true;
                  else
                  {
                    RG1L = signedlong(RG5W);
                    if (RG1L != 1 && RG1L != 2)
                      skip00921 = true;
                  }

                  if (skip00921)
                    //m_41C9D:
                    vo_20.or16(1);
                }
                //m_41CA2:
                RG1L = RG5L;
                RG1W &= 0x1;
                RG1L = signedlong(RG1W);
                if (RG1L != 0 && vb_2c == lcon(0xd))
                  skip00923 = true;
              }
              else
              {
                vb_30 = lcon(0xc);
                if (RG3W != 0)
                {
                  //m_41C00:
                  RG4L = signedlong(ddat.v1e12c8) + 1 & lcon(0x3);
                  RG1L = signedlong(RG1W);
                  if (RG4L == RG1L)
                    vo_20.poke16(1);
                }
                else
                {
                  bool skip00917 = false;
                  if (RG5W != 0)
                  {
                    RG1L = signedlong(RG5W);
                    if (RG1L != lcon(0x3))
                      skip00917 = true;
                  }

                  if (!skip00917)
                    //m_41BD9:
                    vo_20.or16(1);

                  //m_41BDE:
                  RG4L = signedlong(vw_10);
                  RG1L = signedlong(vw_1c) + RG4L;
                  if ((RG1Blo & bcon(0x1)) != 0)
                    skip00923 = true;
                  else
                    vo_20.xor16(1);
                }
              }

              if (skip00923)
                //m_41CB4:
                vo_20.or16(2);
            }
            else
              vb_30 = lcon(0x8);

            //m_41CB9:
            if (vb_2c != lcon(0xd))
              //m_41CDC:
              RG1W = 3;
            else
              RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0xd), vb_34, lcon(0xb), 1);
            //m_41CE1:
            RG4L = unsignedlong(vb_30);
            put8(parb13, RG4Blo);
            RG4L = unsignedlong(vb_34);
            put8(parb12, RG4Blo);
            RG4L = unsignedlong(vb_2c);
            put8(parb11, RG4Blo);
            RG4L = vl_24;
            RG4Bhi |= lcon(0xffffff80);
            RG4L = signedlong(RG4W);
            i32 parl07 = RG4L;
            RG4L = signedlong(vw_28);
            put16(parw06, RG4W);
            RG4L = signedlong(vw_18);
            RG1W &= vo_20.peek16();
            RG1L = unsignedlong(RG1W);
            DM2_QUERY_TEMP_PICST(
              RG1W, RG4W, RG4W, 0,
              0, parw06, CUTX16(parl07), 0,
              lcon(0xa), lcon(0xffff), parb11, parb12, parb13);

            RG2L = signedlong(RG6W);
            RG4L = RG2L;
            RG1L = blitter.calc_stretched_size(0, RG4W);
            dm2_image2.srcx += RG1W;
            RG4L = RG2L;
            RG1L = blitter.calc_stretched_size(lcon(0xffffffa4), RG4W);
            dm2_image2.srcy += RG1W;
            DM2_DRAW_TEMP_PICST();
          }
        }
        else
        {
          RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
          RG1W = word_at(RG1P, 2);
          RG1L = unsignedlong(RG1W);
          RG1Bhi &= lcon(0x3f);
          RG3L >>= 14;
          RG3L <<= bcon(0xe);
          RG1L |= RG3L;
          RG1L = unsignedlong(RG1W);
          RG3L = lcon(0xffffffa4);
          RG2L = 0;
          RG4L = RG6L;
          DM2_DRAW_ITEM(RG1L, RG4L, RG2L, RG3L, lcon(0xffff), NULL, 1, 0, 0);
        }
      }
    }

    //m_41D55:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vw_08))));
    vw_08 = RG1W; // truncated
    if (RG1W == lcon(0xfffffffe))
      return;
  }
  FOREND
}

// was SKW_32cb_347f
static void DM2_MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  i16 RG6w;
  i8 vb_00;
  i8 vb_04;

  RG5w = RG1W;
  RG6w = RG4W;
  vb_04 = RG2Blo;
  vb_00 = RG3Blo;
  RG1L = signedlong(RG1W);
  RG3L = 3 * RG1L;
  RG1L = 4 * RG3L;
  RG3R = &ddat.v1e02f0[RG3L / 3].rc_00;
  if (ddat.v1e02f0[RG1L / 0xc].w_08 != lcon(0xffffffff))
  {
    //m_41E07:
    RG1W = dm2_image2.rect.x + dm2_image2.rect.w;
    RG4L = signedlong(RG1W);
    RG1L = signedlong(DM2_MAX(word_at(RG3P) + word_at(RG3P, lcon(0x4)), RG4W));
    RG2L = RG1L;
    RG1L = signedlong(DM2_MIN(word_at(RG3P), dm2_image2.rect.x));
    mov16(location(RG3P), RG1W);
    RG2L -= RG1L;
    mov16(location(RG3P + lcon(0x4)), RG2W);
    RG1W = dm2_image2.rect.y + dm2_image2.rect.h;
    RG4L = signedlong(RG1W);
    RG1L = signedlong(DM2_MAX(word_at(RG3P, 2) + word_at(RG3P, lcon(0x6)), RG4W));
    RG2L = RG1L;
    RG1L = signedlong(DM2_MIN(word_at(RG3P, 2), dm2_image2.rect.y));
    mov16(location(RG3P + 2), RG1W);
    RG2L -= RG1L;
    mov16(location(RG3P + lcon(0x6)), RG2W);
  }
  else
  {
    *RG3R = dm2_image2.rect; // copy
    RG1W = word_at(RG3P, lcon(0x6));
    RG4L = signedlong(RG1W);
    if (RG4L < lcon(0xf))
    {
      if (RG4L < lcon(0xb))
        mov16(location(RG3P + lcon(0x6)), lcon(0xb));
      RG1L = signedlong(RG1W);
      RG4L = RG1L / 2;
      RG1L = signedlong(word_at(RG3P, 2));
      RG4L -= lcon(0x7);
      RG1L += RG4L;
      mov16(location(RG3P + 2), RG1W);
    }
    RG1L = signedlong(word_at(RG3P, lcon(0x4)));
    if (RG1L < lcon(0xe))
    {
      mov16(location(RG3P + lcon(0x4)), lcon(0xe));
      RG4L = RG1L / 2;
      RG1L = signedlong(word_at(RG3P));
      RG4L -= lcon(0x7);
      RG1L += RG4L;
      mov16(location(RG3P), RG1W);
    }
  }
  //m_41E7D:
  RG1L = signedlong(RG5w);
  RG3L = 3 * RG1L;
  RG1Blo = vb_00;
  ddat.v1e02f0[RG3L / 3].b_0b = RG1Blo;
  ddat.v1e02f0[RG3L / 3].w_08 = RG6w;
  RG1Blo = vb_04;
  ddat.v1e02f0[RG3L / 3].b_0a = RG1Blo;
}

// was SKW_32cb_3991
static void DM2_DRAW_PUT_DOWN_ITEM(i32 eaxl, i32 edxl, i32 ebxl, unk* xecxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3P = xecxp;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  c_wlreg _RG6;
  i32 RG71l;
  c_lreg _RG72;
  unk* xp_00;
  i32 vl_04;
  i16 vw_08;
  i16 vw_0c;
  i16 vw_10;
  i16 vw_14;
  i16 vw_18;
  i16 vw_1c;
  i16 parw00;
  unk* xparp01;
  i16 parw05;
  i16 parw08;

  RG6L = RG1L;
  vw_18 = RG4W;
  vw_1c = RG2W;
  xp_00 = UPCAST(unk, RG3P);
  if (RG1W == lcon(0xfffffffe))
    return;
  RG1L = signedlong(table1d6b15[signedlong(vw_18)]);
  if (RG1L > lcon(0x3))
    return;
  RG5w = wcon(0x0);
  if (RG3P != NULL)
  {
    RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG3P, lcon(0x4)))));
    RG4W = word_at(RG1P, lcon(0x1e));
    vw_10 = RG4W;
    RG1W = word_at(RG1P, lcon(0x20));
    vw_14 = RG1W;
  }
  //m_423B2:
  for (;;)
  {
    RG1L = RG6L;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    RG1L = signedlong(RG1W);
    if (RG1L >= lcon(0x5) && RG1L <= lcon(0xa))
    {
      RG1L = unsignedlong(RG6W);
      RG2L = (RG1L >> bcon(0xe)) - signedlong(ddat.v1e12c8) & lcon(0x3);
      RG4L = signedlong(vw_1c);
      vl_04 = RG4L;
      if (RG2L == RG4L)
      {
        bool skip00930 = false;
        if (xp_00 == NULL)
        {
          //m_424E1:
          put16(parw08, wcon(0x0));
          RG4L = signedlong(RG5w);
          put16(parw05, RG4W);
          RG71l = signedlong(vw_18);
          RG3L = 0;
          RG2L = 0;
          RG4L = RG71l;
          DM2_DRAW_ITEM(RG1L, RG4L, RG2L, RG3L, parw05, NULL, 1, parw08, 0);
          RG5w++;
          RG5w &= wcon(0xf);
          if (vw_18 == 0 || (RG71l == lcon(0x3) && vl_04 > 1))
          {
            bool skip00929 = false;
            RG4L = signedlong(vw_18);
            RG1L = 18 * RG4L;
            RG4P = ddat.ptr1e1044 + RG1L;
            RG1W = signedword(byte_at(RG4P));
            vw_08 = RG1W; // adj
            RG1W = signedword(byte_at(RG4P, 1));
            vw_0c = RG1W; // adj
            RG1L = unsignedlong(RG6W) >> bcon(0xe);
            RG2L = unsignedlong(RG1W);
            RG4P = ADRESSOF(i16, &vw_0c);
            RG1L = DM2_query_1c9a_03cf(&vw_08, RG4P16, RG2L);
            RG4L = RG1L;
            RG1Blo = RG1W != wcon(0xffff) ? 1 : 0;
            RG1L = unsignedlong(RG1Blo);
            if (RG1L == 0)
              skip00929 = true;
            else
            {
              RG4L = unsignedlong(RG4W);
              RG1L = DM2_IS_CREATURE_FLOATING(RG4L);
              if (RG1L != 0)
                skip00929 = true;
              else
              {
                RG1L = DM2_query_0cee_2df4(RG4L);
                if ((RG1Bhi & bcon(0x20)) != 0)
                  skip00929 = true;
              }
            }

            if (skip00929)
            {
              //m_4258C:
              RG4L = unsignedlong(vw_1c);
              RG1L = 1;
              skip00930 = true;
            }
          }
        }
        else
        {
          if ((vw_10 & wcon(0x400)) == 0)
          {
            RG1L = unsignedlong(vw_10) >> 4 & lcon(0xf);
            if (RG1W == 0)
            {
              RG4L = signedlong(ddat.v1e0404);
              RG1L = 12 * RG4L;
              RG4R = &ddat.v1e02f0[RG4L].rc_00;
              DM2_QUERY_EXPANDED_RECT(vw_14, RG4R);
              RG4L = signedlong(ddat.v1e0404);
              RG1L = 3 * RG4L;
              ddat.v1e02f0[RG4L].b_0b = 2;
              ddat.v1e02f0[RG4L].w_08 = RG6W;
              RG4Blo = CUTX8(vw_18);
              ddat.v1e02f0[RG1L / 3].b_0a = RG4Blo;
              ddat.v1e0404++;
              return;
            }
          }
          xparp01 = DOWNCAST(unk, xp_00);
          RG1L = signedlong(RG5w);
          put16(parw00, RG1W);
          RG4L = signedlong(vw_18);
          RG1L = unsignedlong(RG6W);
          RG3L = 0;
          RG2L = 0;
          DM2_DRAW_ITEM(RG1L, RG4L, RG2L, RG3L, parw00, xparp01, 1, 0, 0);
          RG5w++;
          RG5w &= wcon(0xf);
          RG1L = DM2_query_48ae_01af(signedlong(vw_10), 0);
          RG4L = unsignedlong(RG1Blo);
          RG3L = signedlong(vw_1c);
          RG1L = 1 << RG3Blo;
          if ((RG1L & RG4L) != 0)
          {
            RG4L = unsignedlong(vw_1c) + lcon(0x4);
            RG1L = 2;
            skip00930 = true;
          }
        }

        if (skip00930)
        {
          //m_42595:
          RG3L = signedlong(RG1W);
          RG2L = signedlong(vw_18);
          RG72L = unsignedlong(RG6W);
          RG1L = signedlong(RG4W);
          RG4L = RG72L;
          DM2_MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE(RG1L, RG4L, RG2L, RG3L);
        }
      }
    }

    //m_425AC:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG6UW)));
    RG6L = RG1L;
    if (RG1W == lcon(0xfffffffe))
      return;
  }
  FOREND
}

static i32 DM2_guivp_32cb_3e08(i32 eaxl, i32 edxl, i32 ebxl, unk* xecxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3P = xecxp;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  i8* RG6p;
  i32 RG7l;
  i32 vl_00;
  c_ql32 vql_04;
  c_ql32 vql_08;
  i16 vw_0c;

  vql_08.poke32(RG1L);
  vql_04.poke32(RG4L);
  vl_00 = RG2L;
  RG6p = RG3P;
  if (RG1W == lcon(0xfffffffe))
    return RG1L;
  RG1L = signedlong(vql_04.peek16());
  RG4L = signedlong(table1d6b43[RG1L]);
  if (RG4L == lcon(0xffffffff))
    return RG1L;
  ddat.v1d6c04 = lcon(0xfffffffe);
  if (RG1L > lcon(0xf))
    return RG1L;
  RG4L = unsignedlong(table1d6e41[RG1L]);
  vw_0c = RG4W;
  RG1Blo = RG4Blo;
  RG1L = unsignedlong(RG1Blo);
  if (RG1L == lcon(0x3))
    return RG1L;
  RG7l = 0;
  RG5w = wcon(0x0);
  //m_42917:
  for (;;)
  {
    RG1L = signedlong(RG5w);
    if (RG1L >= lcon(0x4))
    {
      if (RG7l != 0)
      {
        RG1L = vql_08.peek32();
        ddat.v1d6c04 = RG1W;
        ddat.v1e1040 = RG7l;
        ddat.v1e103c = RG6p;
      }
      return RG1L;
    }
    //m_428CE:
    RG4L = signedlong(vw_0c) << 2;
    RG1L += RG4L;
    RG4L = unsignedlong(table1d6e35[RG1L]);
    RG3Blo = RG4Blo;
    RG1L = 1 << RG3Blo;
    if ((RG1L & vl_00) == 0)
      //m_42914:
      RG7l |= RG1L;
    else
    {
      RG1L = DM2_DIR_FROM_5x5_POS(unsignedlong(RG4W));
      RG2L = signedlong(RG1W);
      RG4L = signedlong(vql_04.peek16());
      RG1L = unsignedlong(vql_08.peek16());
      RG3P = RG6p;
      DM2_DRAW_PUT_DOWN_ITEM(RG1L, RG4L, RG2L, RG3P);
    }
    //m_42916:
    RG5w++;
  }
  FOREND
}

static i32 DM2_SUMMARY_DRAW_CREATURE(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p;
  c_wlreg _RG6;
  i8* RG7p;
  i32 vl_00;
  c_ol32 vol_04; // poke 32 peek 32 peek8(0x7) => getbyte3
  i16 vw_08;
  i16 vw_0c;
  i16 parw00;

  RG6L = RG1L;
  vw_0c = RG4W;
  vol_04.poke32(RG2L);
  RG4L = 0;
  vl_00 = RG4L;
  if (ddat.v1e025c != 0)
  {
    if (vw_0c == 0)
      return vl_00;
  }
  RG1L = signedlong(vw_0c);
  RG1W = signedword(table1d6b15[RG1L]);
  vw_08 = RG1W;
  //m_412F5:
  for (;;)
  {
    if (RG6W == lcon(0xfffffffe))
      return vl_00;
    RG1L = RG6L;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    if (RG1L == lcon(0x4))
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG6UW));
      RG5p = RG1P;
      if (byte_at(RG1P + lcon(0x5)) != lcon(0xffffffff))
      {
        //m_4132D:
        RG4L = unsignedlong(byte_at(RG1P, lcon(0x5)));
        RG1L = 34 * RG4L;
        RG3P = DOWNCAST(c_creature, creatures) + RG1L;
      }
      else
        RG3P = NULL;
      //m_41343:
      RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG5p, lcon(0x4)))));
      RG7p = RG1P;
      if ((vol_04.getbyte3() & bcon(0x40)) == 0 || jnz_test8(location(RG1P), 1))
      {
        vl_00 = 1;
        RG1L = unsignedlong(RG6W);
        put16(parw00, RG1W);
        RG4L = signedlong(vw_08);
        RG1L = signedlong(vw_0c);
        RG2P = RG5p;
        DM2_QUERY_CREATURE_PICST(RG1L, RG4L, RG2P, RG3P, parw00);
        DM2_DRAW_TEMP_PICST();
        RG1W = word_at(RG7p, 0) & 1;
        RG1L = unsignedlong(RG1W);
        if (RG1L == 0)
          or8(location(RG5p + lcon(0xa)), 2);
        if (RG1L != 0)
        {
          if (jnz_test8(location(RG7p + lcon(0x1e)), lcon(0xf)))
          {
            RG1L = signedlong(vw_08);
            if (RG1L < lcon(0x4))
            {
              RG4L = signedlong(vw_0c);
              RG1L = signedlong(word_at(ddat.ptr1e1044, 18 * RG4L + 2));
              if (RG1L == lcon(0x11))
                vol_04.poke32(lcon(0x3ff));
              RG4L = signedlong(vw_0c);
              RG1L = unsignedlong(word_at(RG5p, 2));
              RG3P = RG5p;
              RG2L = vol_04.peek32();
              DM2_guivp_32cb_3e08(RG1L, RG4L, RG2L, RG3P);
            }
          }
        }
      }
    }
    //m_413EE:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG6UW)));
    RG6L = RG1L;
  }
  FOREND
}

// was SKW_32cb_3b9d
static void DM2_DRAW_STATIC_OBJECT(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG52w;
  c_wlreg _RG53;
  i16 RG6w;
  const i8* RG7p;
  i32 vl_00;
  i32 vl_04;
  c_ql32 vql_08;
  i32 vl_0c;
  i32 vl_10;
  c_ol32 vol_14; // poke 32 peek 16 32 inc32
  i16 vw_18;
  i16 vw_1c;
  i32 vl_20;
  i32 vl_24;
  i16 vw_28;
  bool flag;

  vql_08.poke32(RG1L);
  vl_04 = RG4L;
  vl_00 = RG2L;
  RG1L = signedlong(vql_08.peek16());
  if (RG1L > lcon(0xf))
    return;
  if (table1d6dd3[RG1L] == 0)
    return;
  RG4L = 18 * RG1L;
  RG2P = ddat.ptr1e1044;
  RG4P = RG2P + RG4L;
  RG2W = word_at(RG4P, 2);
  vw_1c = RG2W; // truncated
  RG4W = word_at(RG4P, lcon(0x6));
  vw_18 = RG4W; // truncated
  RG2W = signedword(table1d6de3[RG1L][0]);
  vl_24 = RG2L;
  RG4W = signedword(table1d6de3[RG1L][1]);
  vl_20 = RG4L;
  RG4Bhi = table1d6afe[RG1L];
  if (RG4Bhi >= 0)
  {
    //m_4263C:
    if (RG4Bhi != 0)
      //m_42645:
      RG7p = table1d6da1;
    else
      RG7p = table1d6dba;
  }
  else
    RG7p = table1d6d88;
  //m_4264A:
  vol_14.poke32(0);
  //m_42820:
  for (;;)
  {
    if (vql_08.peek16() != 0)
      //m_4282F:
      RG1L = lcon(0x18);
    else
      RG1L = lcon(0xe);
    //m_42834:
    RG4L = signedlong(RG1W);
    RG1L = unsignedlong(vol_14.peek16());
    if (RG1L > RG4L)
    {
      if (vl_00 != 0)
        DM2_guivp_32cb_2d8c(unsignedlong(vw_18), signedlong(vql_08.peek16()), vl_04);
      return;
    }
    //m_42655:
    RG4W = unsignedword(RG4Blo);
    RG4Blo = byte_at(RG7p, RG1L);
    vw_28 = RG4W;
    RG3Blo = RG4Blo;
    RG1L = 1 << RG3Blo;
    RG4L = vl_04 & RG1L;
    flag = RG4L == 0;
    vl_10 = RG4L;
    if (!flag)
    {
      bool skip00933 = false;
      RG4L = signedlong(vql_08.peek16());
      RG2L = RG4L;
      RG1L = long_at(ddat.v1e104c, 4 * RG2L);
      if ((RG1L & vl_10) != 0)
      {
        RG1L = DM2_DIR_FROM_5x5_POS(unsignedlong(vw_28));
        RG2L = signedlong(RG1W);
        RG1L = unsignedlong(vw_18);
        RG3P = NULL;
        DM2_DRAW_PUT_DOWN_ITEM(RG1L, RG4L, RG2L, RG3P);
      }
      //m_426AC:
      RG1L = unsignedlong(vw_28);
      RG3W = signedword(table1d6e03[RG1L][0]);
      RG3L += vl_24;
      RG1L = vl_20 - signedlong(signedword(table1d6e03[RG1L][1]));
      RG52w = RG1W;
      if (vw_1c != 0)
      {
        RG4L = signedlong(RG3W);
        RG1P = ddat.v1e1048 + 17 * RG4L;
        RG1Blo = byte_at(RG1P, signedlong(RG52w));
        RG1W = unsignedword(RG1Blo);
        if (RG1W != vql_08.peek16())
          skip00933 = true;
      }

      if (!skip00933)
      {
        //m_426FE:
        RG4L = signedlong(RG3W);
        RG1P = ddat.v1e1050 + 17 * RG4L;
        RG1Blo = byte_at(RG1P, signedlong(RG52w));
        RG4L = unsignedlong(RG1Blo);
        if (RG4L != lcon(0xff))
        {
          RG4L = vl_04;
          if ((RG4L & vl_10) != 0)
          {
            RG2L = RG4L;
            RG4L = unsignedlong(vw_28);
            if (RG4L != lcon(0xc))
              RG2L &= lcon(0xbfffffff);
            RG4L = unsignedlong(RG1Blo);
            RG1L = 18 * RG4L;
            vl_0c = RG1L;
            RG1L = DM2_SUMMARY_DRAW_CREATURE(unsignedlong(word_at(ddat.ptr1e1044 + vl_0c, lcon(0x6))), RG4L, RG2L);
            if (RG1L != 0)
            {
              RG4L = signedlong(RG3W);
              RG1L = 17 * RG4L;
              RG4P = ddat.v1e1050 + RG1L;
              RG1L = signedlong(RG52w);
              mov8(location(RG4P + RG1L), lcon(0xffffffff));
              RG4L = signedlong(vql_08.peek16()) << 2;
              RG1P = ddat.v1e1038;
              if (long_at(RG1P + RG4L) != 0)
              {
                RG53L = vol_14.peek32() - lcon(0x7);
                //m_427EB:
                for (;;)
                {
                  if (RG53UW >= mkuw(vol_14.peek16()))
                    break;
                  //m_427AF:
                  RG1L = unsignedlong(RG53W);
                  RG6w = unsignedword(byte_at(RG7p, RG1L));
                  RG4L = signedlong(vql_08.peek16());
                  RG3L = 4 * RG4L;
                  RG2P = ddat.v1e1038 + RG3L;
                  RG3Blo = byte_at(RG7p, RG1L);
                  RG1L = 1 << RG3Blo;
                  if ((RG1L & long_at(RG2P)) != 0)
                  {
                    RG2L = signedlong(RG6w);
                    DM2_DRAW_FLYING_ITEM(unsignedlong(vw_18), RG4L, RG2L);
                  }
                  //m_427EA:
                  RG53L++;
                }
              }
            }
          }
        }
      }

      //m_427F2:
      RG4L = signedlong(vql_08.peek16());
      RG2L = RG4L;
      RG1L = long_at(ddat.v1e1038, 4 * RG2L);
      if ((RG1L & vl_10) != 0)
      {
        RG2L = signedlong(vw_28);
        DM2_DRAW_FLYING_ITEM(unsignedlong(vw_18), RG4L, RG2L);
      }
    }
    //m_4281C:
    vol_14.inc32();
  }
  FOREND
}

// was SKW_32cb_1f3e
static i32 DM2_DRAW_EXTERNAL_TILE(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG52l;
  i16 RG53w;
  i32 RG54l;
  i16 RG55w;
  c_wlreg _RG6;
  c_lreg _RG7;
  c_ql32 vql_00;
  c_image image;
  c_5bytes c12_140;
  i32 vl_148;
  i32 vl_14c;
  i32 vl_150;
  i32 vl_154;
  i32 vl_158;
  i32 vl_15c;
  i16 vw_160;
  i16 vw_164;
  i16 vw_168;
  i16 vw_16c;
  i16 vw_170;
  i16 vw_174;
  c_o32 vo_178; // poke 16 peek 16 xor8
  i16 vw_17c;
  i16 vw_180;
  i32 vl_184;
  i16 vw_188;
  i8 vb_18c;
  i8 vb_190;
  i16 parw01;
  i8 parb07;
  i8 parb08;
  i16 parw10;
  i16 parw13;
  i8 parb16;
  i8 parb17;
  i16 parw19;
  i16 parw20;
  i16 parw21;
  i16 parw22;
  i16 parw23;
  i16 parw26;
  i16 parw29;
  i8 parb32;
  i8 parb33;

  vql_00.poke32(RG1L);
  RG4L = unsignedlong(RG1W);
  RG1Blo = table1d6c1e[RG4L];
  vb_18c = RG1Blo;
  if (RG1Blo == lcon(0xffffffff))
    return 0;
  RG1L = 18 * RG4L;
  RG2P = ddat.ptr1e1044 + RG1L;
  RG1W = signedword(byte_at(RG2P));
  vw_168 = RG1W;
  RG1W = signedword(byte_at(RG2P, 1));
  vw_180 = RG1W;
  RG1W = signedword(table1d6b15[RG4L]);
  vw_17c = RG1W;
  RG2L = signedlong(vw_180);
  RG4L = signedlong(vw_168);
  RG1L = DM2_GET_TELEPORTER_DETAIL(&c12_140, RG4L, RG2L);
  vl_14c = RG1L;
  RG4L = signedlong(vw_17c);
  if (RG4L > lcon(0x3))
  {
    if (RG1L == 0)
      return 0;
  }
  if (vl_14c != 0)
  {
    RG1L = unsignedlong(vql_00.peek16());
    if (RG1L != 1 && RG1L != 2)
    {
      RG1L = unsignedlong(c12_140.b_00);
      if (RG1W != ddat.v1e0258)
      {
        RG1L = signedlong(ddat.v1e0258) + 2 & lcon(0x3);
        RG4L = unsignedlong(c12_140.b_00);
        if (RG4L != RG1L)
          return 0;
      }
    }
  }
  RG4L = unsignedlong(vql_00.peek16());
  RG1L = 8 * RG4L;
  RG2L = 2 * (RG4L + RG1L);
  RG2W = word_at(ddat.ptr1e1044, RG2L + lcon(0xc));
  RG1L = unsignedlong(RG2W) >> bcon(0x8);
  vw_164 = RG1W;
  RG1L = unsignedlong(vb_18c) + signedlong(vw_164);
  vb_18c = RG1Blo;
  vb_190 = RG2Blo;
  if (RG2Blo == lcon(0xffffffff))
    return 0;
  if (RG2Blo == 0)
    return 0;
  if (RG4L == lcon(0xe) || RG4L == lcon(0xf))
  {
    RG4L = unsignedlong(vb_190);
    RG3L = lcon(0x61);
    RG2L = lcon(0xb);
    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0xa), RG4Blo, RG2Blo, RG3Blo);
    if (RG1W != 0)
      return 0;
  }
  //m_402A5:
  vo_178.poke16(wcon(0x0));
  RG1L = unsignedlong(vql_00.peek16());
  if (table1d6afe[RG1L] != 0)
  {
    //m_40308:
    RG1L = signedlong(table1d6afe[RG1L]);
    if (RG1L >= 1)
      vo_178.poke16(1);
  }
  else
  {
    RG4L = unsignedlong(vb_190);
    RG3L = lcon(0x7);
    RG2L = lcon(0xb);
    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(10, RG4Blo, RG2Blo, RG3Blo);
    if (RG1W == 0)
    {
      vo_178.poke16(ddat.v1e12d0);
      RG1W = vw_17c;
      RG1W &= 0x1;
      RG1L = signedlong(RG1W);
      if (RG1L == 0)
        vo_178.xor16(1);
    }
  }
  //m_40320:
  RG4L = unsignedlong(vb_190);
  RG3L = 4;
  RG2L = lcon(0xb);
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(10, RG4Blo, RG2Blo, RG3Blo);
  vw_174 = RG1W;
  if (RG1W == 0)
    vw_174 = ddat.v1d6c00;
  RG4L = unsignedlong(vb_190);
  RG3L = 5;
  RG2L = lcon(0xb);
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(10, RG4Blo, RG2Blo, RG3Blo);
  if (RG1W != 0)
  {
    //m_4038B:
    RG4L = unsignedlong(RG1W) >> bcon(0x8);
    vw_170 = RG4W;
    RG1W = unsignedword(RG1Blo);
    RG1L--;
  }
  else
  {
    RG1L = lcon(0xc);
    RG4L = 0;
    vw_170 = RG4W;
  }
  //m_4039E:
  RG4L = signedlong(RG1W);
  RG1W = DM2_QUERY_CREATURE_BLIT_RECTI(vql_00.peek16(), t_rotate(0), RG4W);
  vw_16c = RG1W;
  vw_188 = 0x40;
  RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xa), vb_190, 1, vb_18c) ? 1 : 0;
  if (RG1L == 0)
  {
    RG1W = vql_00.peek16();
    RG1Blo = table1d6c35[RG1L];
    RG1Bhi = CUTX8(vw_164);
    RG1Blo += RG1Bhi;
    vb_18c = RG1Blo;
    RG1L = signedlong(vw_17c);
    RG1Blo = table1d6b71[RG1L];
    RG1W = unsignedword(RG1Blo);
    vw_188 = RG1W;
  }
  RG1L = unsignedlong(vql_00.peek16());
  if (RG1L == 1 || RG1L == 2)
  {
    RG4L = unsignedlong(vb_190);
    RG3L = lcon(0x60);
    RG2L = lcon(0xb);
    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(10, RG4Blo, RG2Blo, RG3Blo);
    if (RG1W != 0)
      vw_188 = 0x60;
  }
  //m_40456:
  if (vw_17c != 0)
  {
    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(10, vb_190, 11, 99) & lcon(0xffff);
    vl_158 = mkl(RG1W);
    if (vl_14c != 0 || RG1W != 0)
    {
      RG1L = unsignedlong(vb_18c);
      put8(parb08, RG1Blo);
      RG1L = unsignedlong(vb_190);
      vl_154 = RG1L;
      put8(parb07, RG1Blo);
      RG1L = signedlong(vw_170);
      vl_150 = RG1L;
      i32 parl03 = RG1L;
      RG1L = signedlong(vw_16c);
      vl_15c = RG1L;
      i32 parl02 = RG1L;
      RG1L = signedlong(vw_17c);
      vl_148 = RG1L;
      put16(parw01, RG1W);
      RG52l = signedlong(vw_188);
      RG6L = unsignedlong(vo_178.peek16());
      RG3L = 0;
      RG4L = RG2L = RG52l;
      RG1L = RG6L;
      DM2_QUERY_TEMP_PICST(
        RG1W, RG4W, RG2W, RG3W,
        0, parw01, CUTX16(parl02), CUTX16(parl03),
        lcon(0xfffd), lcon(0xfffd), lcon(0xa), parb07, parb08);

      RG7L = unsignedlong(vql_00.peek16());
      if (table1d6afe[RG7L] == 0)
      {
        RG1Blo = vb_18c + bcon(0xc8);
        RG1L = unsignedlong(RG1Blo);
        put8(parb17, RG1Blo);
        RG4L = vl_154;
        put8(parb16, RG4Blo);
        RG1L = signedlong(vw_174);
        put16(parw13, RG1W);
        i32 parl12 = vl_150;
        i32 parl11 = vl_15c;
        RG1L = vl_148;
        put16(parw10, RG1W);
        i32 parl09 = signedlong(dm2_image2.w_1e);
        RG3L = signedlong(dm2_image2.w_1c);
        DM2_QUERY_TEMP_PICST(
          RG6W, CUTX16(RG52l), CUTX16(RG52l), RG3W,
          CUTX16(parl09), parw10, CUTX16(parl11), CUTX16(parl12),
          parw13, lcon(0xffff), lcon(0xa), parb16, parb17);

        DM2_DRAW_TEMP_PICST();
        RG1L = unsignedlong(c12_140.b_02);
        vw_168 = RG1W;
        RG1L = unsignedlong(c12_140.b_03);
        vw_180 = RG1W;
        RG1W = ddat.v1e12c8;
        vl_184 = RG1L;
        RG1L = unsignedlong(c12_140.b_01);
        ddat.v1e12c8 = RG1W;
        vw_160 = ddat.v1d3248;
        DM2_CHANGE_CURRENT_MAP_TO(unsignedlong(c12_140.b_04));
        ddat.v1d6bfa = 1;
        RG6W = signedword(table1d6b15[RG7L]);
        RG1L = -RG6L;
        RG4L = signedlong(RG1W);
        DM2_CALC_VECTOR_W_DIR(ddat.v1e12c8, RG4W, 0, &vw_168, &vw_180);
        RG53w = wcon(0x4);
        //m_40651:
        for (;;)
        {
          if (RG53w < RG6W)
          {
            RG53w = wcon(0x4);
            break;
          }
          //m_40622:
          RG3L = signedlong(ddat.v1e12c8);
          RG1L = signedlong(RG53w);
          DM2_guivp_32cb_4185(signedlong(vw_168), signedlong(vw_180), signedlong(table1d6c19[RG1L]), RG3L);
          RG53w--;
        }
        //m_40679:
        for (;;)
        {
          if (RG53w <= RG6W)
          {
            DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_160));
            RG1L = 0;
            ddat.v1d6bfa = RG1W;
            RG1L = vl_184;
            ddat.v1e12c8 = RG1W;
            DM2_DRAW_STATIC_OBJECT(signedlong(vql_00.peek16()), lcon(0x3ff), 1);
            break;
          }
          //m_4065D:
          RG1L = signedlong(table1d6c19[signedlong(RG53w)]);
          DM2_DRAW_STATIC_OBJECT(RG1L, lcon(0x1ffffff), 1);
          RG53w--;
        }
      }
      //m_406B8:
      RG54l = unsignedlong(vb_190);
      RG3L = lcon(0x11);
      RG2L = lcon(0xb);
      RG4L = RG54l;
      RG1L = signedlong(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0xa), RG4Blo, RG2Blo, RG3Blo));
      i32 parl24 = RG1L;
      RG1L = signedlong(vw_174);
      put16(parw23, RG1W);
      RG1L = unsignedlong(vo_178.peek16());
      put16(parw22, RG1W);
      RG1L = signedlong(vw_17c);
      put16(parw21, RG1W);
      RG1L = signedlong(vw_188);
      put16(parw20, RG1W);
      put16(parw19, RG1W);
      RG3L = unsignedlong(vb_18c);
      RG2L = RG54l;
      RG4L = lcon(0xa);
      RG1W = DM2_QUERY_MULTILAYERS_PIC(&image, RG4Blo, RG2Blo, RG3Blo, parw19, parw20, parw21, parw22, parw23, /*TAG alphamask*/ CUTX16(parl24));
      RG3W = RG1W;
      if (image.colors > 0)
      {
        //m_40734:
        RG1L = unsignedlong(vw_174);
        RG55w = unsignedword(palettecolor_to_ui8(image.palette[RG1L]));
      }
      else
        RG55w = vw_174;
      //m_40743:
      RG4L = vl_158;
      if (RG4L == 0)
      {
        //m_40766:
        RG4W = RG55w;
        RG1L = signedlong(RG3W);
        DM2_FILL_ENTIRE_PICT(dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ RG1W), ui8_to_pixel(RG4UBlo));
      }
      else
      {
        RG2L = signedlong(vw_174);
        RG4L = signedlong(RG3W);
        DM2_guivp_32cb_0c7d(&image, RG4L, RG2W);
      }
      //m_40776:
      RG2L = signedlong(RG3W);
      image.bmp = dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ RG2W);
      DM2_DRAW_PICST(&image);
      DM2_image_0b36_01cd(&image.imgdesc);
      image.alphamask = RG55w;
      image.bmp = gfxsys.bitmapptr;
      RG1L = unsignedlong(vw_16c);
      image.query1 = RG1W;
      RG1L = unsignedlong(vw_170);
      image.w_1a = RG1W;
      image.colors = 0;
      DM2_SET_IMAGE((t_dbidx)RG2W, &image.imgdesc);
      DM2_DRAW_PICST(&image);
      dm2_dballochandler.DM2_dballoc_3e74_58a3((t_dbidx)RG2W);
      return vl_14c;
    }
  }
  //m_407E7:
  RG1L = unsignedlong(vb_18c);
  put8(parb33, RG1Blo);
  RG1L = unsignedlong(vb_190);
  put8(parb32, RG1Blo);
  RG1L = signedlong(vw_174);
  put16(parw29, RG1W);
  i32 parl28 = signedlong(vw_170);
  i32 parl27 = signedlong(vw_16c);
  RG1L = signedlong(vw_17c);
  put16(parw26, RG1W);
  RG4L = signedlong(vw_188);
  DM2_QUERY_TEMP_PICST(
    vo_178.peek16(), RG4W, RG4W, 0,
    0, parw26, CUTX16(parl27), CUTX16(parl28),
    parw29, lcon(0xffff), lcon(0xa), parb32, parb33);

  DM2_DRAW_TEMP_PICST();
  return 0;
}

// belongs to DM2_DRAW_DOOR_TILE
// was SKW_32cb_4905
static void DM2_DRAW_DOOR(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG51;
  i32 RG52l;
  c_wlreg _RG53;
  c_wlreg _RG61;
  i16 RG62w;
  i8* RG7p;
  i32 vl_00;
  c_ql32 vql_04;
  c_image image;
  i32 vl_144;
  i16 vw_148;
  i16 vw_14c;
  i16 vw_150;
  i16 vw_154;
  i32 vl_158;
  i16 vw_15c;
  i16 vw_160;
  i32 vl_164;
  i16 vw_168;
  i16 vw_16c;
  i8 vb_170;
  i8 vb_174;
  i8 vb_178;
  i16 parw00;
  i16 parw01;
  i16 parw02;
  i16 parw04;
  i16 parw08;
  i16 parw11;
  i8 parb14;
  i16 parw17;
  i16 parw20;
  i16 parw21;
  i8 parb23;
  i16 parw26;
  i16 parw29;
  i8 parb32;
  i8 parb33;

  vql_04.poke32(RG1L);
  vl_00 = RG2L;
  if (RG4W != 0)
  {
    RG4L = unsignedlong(RG4W);
    DM2_DRAW_DOOR_FRAMES(signedlong(vql_04.peek16()), RG4L);
  }
  //m_43541:
  if (RG3L != 0)
    DM2_DRAW_STATIC_OBJECT(signedlong(vql_04.peek16()), RG3L, 0);
  RG4L = signedlong(vql_04.peek16());
  RG1P = ddat.ptr1e1044 + 18 * RG4L;
  RG2W = word_at(RG1P, lcon(0x8));
  vw_168 = RG2W; // truncated
  if (RG2W != 0)
  {
    RG2W = word_at(RG1P, lcon(0xa));
    RG1W = signedword(table1d6b15[RG4L]);
    vw_16c = RG1W;
    RG1L = signedlong(table1d6b15[RG4L]);
    if (RG1L <= lcon(0x3))
    {
      RG1L = signedlong(vw_16c);
      RG1W = signedword(table1d6f27[RG1L]);
      vl_158 = RG1L;
      if (RG1W >= 0)
      {
        RG1W = table1d6f2c[RG4L];
        vl_164 = RG1L;
        if (RG1W >= 0)
        {
          bool skip00945 = false;
          RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
          RG7p = RG1P;
          RG1W = word_at(RG1P, 2) & 1;
          RG1L = unsignedlong(RG1W);
          RG1Blo = ddat.v1e040e[RG1L];
          vb_174 = RG1Blo;
          RG4L = unsignedlong(RG1Blo);
          RG3L = 4;
          RG2L = lcon(0xb);
          RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(14, RG4Blo, RG2Blo, RG3Blo);
          vw_160 = RG1W;
          if (RG1W == 0)
            vw_160 = 10;
          RG1Blo = CUTX8(vw_16c) - 1;
          vb_178 = RG1Blo;
          RG61L = lcon(0x40);
          RG1L = 0;
          vw_150 = RG1W;
          if (vw_16c == 0)
            skip00945 = true;
          else
          {
            RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xe), vb_174, 1, vb_178) ? 1 : 0;
            if (RG1L == 0)
              skip00945 = true;
          }

          if (skip00945)
          {
            //m_43685:
            RG2W = vw_16c;
            if (RG2W == 0)
              //m_436AD:
              RG61L = lcon(0x71);
            else
            {
              vw_150 = RG2W;
              RG61W = unsignedword(table1d6b71[signedlong(vw_16c)]);
            }
            //m_436B2:
            RG1W = unsignedword(RG1Blo);
            vb_178 = RG1Bhi;
          }
          bool skip00948 = false;
          //m_436BB:
          RG51W = word_at(RG7p, 2);
          RG51L <<= 11;
          RG51UW >>= bcon(0xc);
          if (RG51W == 0)
          {
            RG1L = unsignedlong(vw_168);
            if (RG1L != lcon(0x5))
            {
              //m_43978:
              vw_15c = -1;
              RG1L = unsignedlong(vb_178);
              put8(parb33, RG1Blo);
              RG1L = unsignedlong(vb_174);
              put8(parb32, RG1Blo);
              put16(parw29, signedlong(vw_160));
              RG1L = signedlong(vw_150);
              put16(parw26, RG1W);
              RG4L = signedlong(RG61W);
              DM2_QUERY_TEMP_PICST(
                0, RG4W, RG4W, 0,
                0, parw26, -1, -1,
                parw29, lcon(0xffff), lcon(0xe), parb32, parb33);

              skip00948 = true;
            }
          }

          if (!skip00948)
          {
            //m_436DE:
            RG1L = signedlong(vw_160);
            put16(parw04, RG1W);
            RG1L = signedlong(vw_150);
            put16(parw02, RG1W);
            RG1L = signedlong(RG61W);
            put16(parw01, RG1W);
            put16(parw00, RG1W);
            RG3L = unsignedlong(vb_178);
            RG1L = unsignedlong(vb_174);
            vl_144 = RG1L;
            RG1W = DM2_QUERY_MULTILAYERS_PIC(&image, 14, RG1Blo, RG3Blo, parw00, parw01, parw02, 0, parw04, /*TAG alphamask*/ -1);
            vw_15c = RG1W;
            DM2_DRAW_PICST(&image);
            DM2_image_0b36_01cd(&image.imgdesc);
            RG1L = unsignedlong(RG61W);
            if (RG1L != lcon(0x40))
              //m_437A6:
              vw_154 = RG61W;
            else
            {
              RG3P16 = &vw_148;
              DM2_QUERY_GDAT_IMAGE_METRICS(14, CUTLX8(vl_144), 0, RG3P16, &vw_14c);
              RG61L = DM2_guivp_32cb_48d5(unsignedlong(dm2_image2.imgdesc.width), unsignedlong(vw_148));
              RG4L = unsignedlong(vw_14c);
              RG1L = DM2_guivp_32cb_48d5(unsignedlong(dm2_image2.imgdesc.height), RG4L);
              vw_154 = RG1W;
            }
            //m_437AE:
            if (RG51W != 0)
            {
              RG51L--;
              RG1L = unsignedlong(RG51W);
              RG1Blo = ddat.v1e0414[RG1L];
              vb_170 = RG1Blo;
              RG4L = unsignedlong(RG1Blo);
              RG3L = 4;
              RG2L = lcon(0xb);
              RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(11, RG4Blo, 11, 4);
              if (RG1W == 0)
                RG1L = lcon(0x9);
              RG4L = unsignedlong(vb_170);
              put8(parb14, RG4Blo);
              RG1L = signedlong(RG1W);
              put16(parw11, RG1W);
              RG3L = lcon(0x8);
              RG2L = lcon(0xb);
              RG1L = signedlong(4 * DM2_QUERY_GDAT_ENTRY_DATA_INDEX(RG2Blo, RG4Blo, RG2Blo, RG3Blo) + lcon(0x7d0) + CUTX16(vl_158));
              i32 parl09 = RG1L;
              RG1L = signedlong(vw_16c);
              put16(parw08, RG1W);
              RG2L = signedlong(vw_154);
              RG4L = signedlong(RG61W);
              DM2_QUERY_TEMP_PICST(
                0, RG4W, RG2W, 0,
                0, parw08, CUTX16(parl09), -1,
                parw11, lcon(0xffff), lcon(0xb), parb14, 0);

              dm2_image2.bmp = dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ vw_15c);
              DM2_DRAW_TEMP_PICST();
            }
            //m_4385B:
            RG1L = unsignedlong(vw_168);
            if (RG1L == lcon(0x5))
            {
              RG4L = unsignedlong(vb_174);
              RG3L = lcon(0x12);
              RG2L = lcon(0xb);
              RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(14, RG4Blo, RG2Blo, RG3Blo);
              if (RG1W == 0)
                RG1L = lcon(0x9);
              RG4L = unsignedlong(vb_174);
              put8(parb23, RG4Blo);
              put16(parw21, signedlong(vw_160));
              RG1L = signedlong(RG1W);
              put16(parw20, RG1L);
              RG1L = signedlong(4 * DM2_QUERY_GDAT_ENTRY_DATA_INDEX(14, RG4Blo, 11, 10) + lcon(0x7d0) + CUTX16(vl_158));
              i32 parl18 = RG1L;
              RG1L = signedlong(vw_16c);
              put16(parw17, RG1W);
              RG2L = signedlong(vw_154);
              RG4L = signedlong(RG61W);
              DM2_QUERY_TEMP_PICST(
                0, RG4W, RG2W, 0,
                0, parw17, CUTX16(parl18), -1,
                parw20, parw21, lcon(0xe), parb23, lcon(0x41));

              dm2_image2.bmp = dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ vw_15c);
              DM2_DRAW_TEMP_PICST();
            }
            //m_43913:
            if (image.colors > 0)
            {
              //m_43927:
              RG1L = unsignedlong(vw_160);
              RG1UBlo = palettecolor_to_ui8(image.palette[RG1L]);
              RG1L = unsignedlong(RG1Blo);
            }
            else
              RG1L = unsignedlong(vw_160);
            //m_4393A:
            image.alphamask = RG1W;
            RG3L = 0;
            image.colors = 0;
            DM2_SET_IMAGE((t_dbidx)vw_15c, &image.imgdesc);
            dm2_image2 = image; // copy image
            dm2_image2.bmp = gfxsys.bitmapptr;
          }

          //m_439C4:
          RG1L = unsignedlong(vw_168);
          if (RG1L < lcon(0x4))
          {
            RG52l = vl_164 + RG1L;
            vl_164 = RG52l;
            if (jz_test8(location(RG7p + 2), lcon(0x20)))
            {
              RG62w = dm2_image2.imgdesc.width / 2;
              dm2_image2.imgdesc.width = RG62w;
              dm2_image2.imgdesc.mode |= 0x10;
              dm2_image2.imgdesc.x += RG62w;
              RG1L = RG52l + 6;
              dm2_image2.query1 = RG1W;
              DM2_DRAW_TEMP_PICST();
              dm2_image2.imgdesc.x -= RG62w;
              RG2L = RG52l + lcon(0x3);
              vl_164 = RG2L;
            }
          }
          //m_43A2D:
          RG1L = vl_164;
          dm2_image2.query1 = RG1W;
          DM2_DRAW_TEMP_PICST();
          if (vw_15c >= wcon(0x0))
            dm2_dballochandler.DM2_dballoc_3e74_58a3((t_dbidx)vw_15c);
        }
      }
    }
  }
  //m_43A59:
  RG53L = vl_00;
  if (RG53W != 0)
  {
    RG4L = unsignedlong(RG53W);
    DM2_DRAW_DOOR_FRAMES(signedlong(vql_04.peek16()), RG4L);
  }
}

// belongs to DM2_DRAW_DOOR_TILE
static i32 DM2_guivp_32cb_3edd(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  if (ddat.v1d6c04 != lcon(0xfffffffe))
  {
    RG3P = ddat.v1e103c;
    RG2L = ddat.v1e1040;
    RG4L = signedlong(RG1W);
    RG1L = DM2_guivp_32cb_3e08(unsignedlong(ddat.v1d6c04), RG4L, RG2L, RG3P);
  }
  ddat.v1d6c04 = lcon(0xfffffffe);
  return RG1L;
}

// was SKW_32cb_4cdf
static void DM2_DRAW_DOOR_TILE(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5;
  i32 RG6l;
  bool skip00955 = false;
  bool skip00953 = false;
  bool skip00952 = false;
  bool skip00951 = false;
  bool skip00950 = false;

  RG5L = RG1L;
  RG1L = signedlong(RG1W);
  if (table1d6f4c[RG1L] == 0)
    return;
  RG3L = 0;
  RG4L = RG1L;
  RG1L *= 8;
  RG4L += RG1L;
  RG4L = 2 * RG4L;
  RG1L = signedlong(word_at(ddat.ptr1e1044, RG4L + lcon(0x8)));
  if (RG1L != lcon(0x5))
    RG3L = lcon(0x40001000);
  RG1Blo = RG5W != wcon(0x0) ? 1 : 0;
  RG2L = unsignedlong(RG1Blo);
  RG6l = signedlong(RG5W);
  DM2_DRAW_STATIC_OBJECT(RG6l, lcon(0x3ff), RG2L);
  RG4L = 0;
  RG1L = 0;
  if (RG5UW < mkuw(lcon(0x7)))
  {
    bool skip00954 = false;
    //m_43B1D:
    if (RG5UW < mkuw(lcon(0x4)))
    {
      //m_43B30:
      if (RG5W == 0)
      {
        //m_43B40:
        if (RG3L != 0)
          DM2_DRAW_STATIC_OBJECT(RG6l, lcon(0x1000), 0);
        RG1L = signedlong(RG5W);
        DM2_DRAW_DOOR(RG1L, lcon(0x6), 0, 0);
        return;
      }
      if (RG5W == lcon(0x3))
        skip00954 = true;
    }
    else
    {
      if (RG5UW <= mkuw(lcon(0x4)))
        skip00950 = true;
      else
      {
        if (RG5UW <= mkuw(lcon(0x5)))
          skip00952 = true;
        else
          skip00954 = true;
      }
    }

    if (skip00954)
    {
      //m_43B95:
      RG1L = lcon(0x7);
      skip00955 = true;
    }
  }
  else
  {
    if (RG5UW <= mkuw(lcon(0x7)))
      skip00950 = true;
    else
    {
      if (RG5UW < mkuw(lcon(0xc)))
      {
        //m_43B04:
        if (RG5UW <= mkuw(lcon(0x8)))
          skip00952 = true;
        else
        {
          if (RG5W == lcon(0xb))
          {
            //m_43B8E:
            RG1L = 6;
            skip00955 = true;
          }
        }
      }
      else
      {
        if (RG5UW <= mkuw(lcon(0xc)))
        {
          //m_43B80:
          RG1L = 2;
          skip00951 = true;
        }
        else
        {
          if (RG5UW <= mkuw(lcon(0xd)))
          {
            //m_43B87:
            RG1L = 4;
            skip00953 = true;
          }
          else
          {
            if (RG5UW <= mkuw(lcon(0xf)))
              skip00955 = true;
          }
        }
      }
    }
  }

  if (skip00950)
  {
    //m_43B68:
    RG1L = 1;
    skip00951 = true;
  }

  if (skip00951)
  {
    //m_43B6D:
    RG4L = 4;
    skip00955 = true;
  }

  if (skip00952)
  {
    //m_43B74:
    RG1L = 1;
    skip00953 = true;
  }

  if (skip00953)
  {
    //m_43B79:
    RG4L = 2;
    skip00955 = true;
  }

  if (skip00955)
  {
    //m_43B9A:
    RG2L = unsignedlong(RG4W);
    RG4L = unsignedlong(RG1W);
    RG1L = signedlong(RG5W);
    DM2_DRAW_DOOR(RG1L, RG4L, RG2L, RG3L);
  }

  //m_43BAC:
  RG3L = signedlong(RG5W);
  DM2_DRAW_STATIC_OBJECT(RG3L, lcon(0x1fffc00), 1);
  DM2_guivp_32cb_3edd(RG3L);
}

// belongs to DM2_DISPLAY_VIEWPORT
// was SKW_32cb_5340
static i32 DM2_DRAW_PLAYER_TILE(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5; // W and UW only
  i16 RG6w;
  c_5bytes c12_00;
  i16 parw00;
  i32 parl01;
  i16 parw02;
  bool skip00967 = false;

  RG1P = ddat.ptr1e1044;
  RG5W = word_at(RG1P, 2);
  if (RG5UW < mkuw(lcon(0x10)))
  {
    //m_441FD:
    if (RG5W == 2)
      //m_4437C:
      DM2_DRAW_PIT_TILE(0);
    skip00967 = true;
  }
  else
  {
    bool skip00966 = false;
    if (RG5UW <= mkuw(lcon(0x10)))
    {
      //m_4420C:
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(word_at(RG1P, lcon(0xa)))));
      RG1W = word_at(RG1P, 2) & 1;
      RG1L = unsignedlong(RG1W);
      RG4L = unsignedlong(ddat.v1e040e[RG1L]);
      RG3L = lcon(0x40);
      RG2L = lcon(0xb);
      RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(14, RG4Blo, RG2Blo, RG3Blo);
      if (RG1W != 0)
        skip00967 = true;
      else
      {
        parl01 = unsignedlong(ddat.v1e12d0);
        RG1L = signedlong(ddat.v1d6c00);
        put16(parw00, RG1W);
        RG2L = 0;
        RG4L = 2;
        RG1W = DM2_QUERY_CREATURE_BLIT_RECTI(0, t_rotate(RG2UW), RG4W);
        RG3L = signedlong(RG1W);
        RG4L = unsignedlong(ddat.v1d6c02);
        RG2L = 6;
        skip00966 = true;
      }
    }
    else
    {
      if (RG5UW <= mkuw(lcon(0x11)))
      {
        //m_44388:
        DM2_DRAW_DOOR_TILE(0);
        RG3L = 0;
      }
      else
      {
        if (RG5W == lcon(0x13))
        {
          //m_442FF:
          RG6w = word_at(RG1P, lcon(0x8));
          RG1L = signedlong(ddat.v1d6c00);
          put16(parw02, RG1W);
          if (RG6w == wcon(0x0))
            //m_4431B:
            RG1L = lcon(0x338);
          else
            RG1L = lcon(0x32b);
          //m_44320:
          RG3L = signedlong(RG1W);
          if (RG6w == wcon(0x0))
            //m_4432B:
            RG1Blo = lcon(0x4d);
          else
            RG1Blo = lcon(0x39);
          //m_4432D:
          RG2L = unsignedlong(RG1Blo);
          DM2_DRAW_DUNGEON_GRAPHIC(8, ddat.v1d6c02, RG2Blo, RG3W, parw02, 0);
          parl01 = 0;
          RG1L = signedlong(ddat.v1d6c00);
          put16(parw00, RG1W);
          if (RG6w == wcon(0x0))
            //m_44359:
            RG1L = lcon(0x339);
          else
            RG1L = lcon(0x32c);
          //m_4435E:
          RG3L = signedlong(RG1W);
          if (RG6w == wcon(0x0))
            //m_44369:
            RG1Blo = lcon(0x4e);
          else
            RG1Blo = lcon(0x3a);
          //m_4436B:
          RG2L = unsignedlong(RG1Blo);
          RG4L = unsignedlong(ddat.v1d6c02);
          skip00966 = true;
        }
        else
          skip00967 = true;
      }
    }

    if (skip00966)
    {
      //m_4427C:
      DM2_DRAW_DUNGEON_GRAPHIC(8, RG4Blo, RG2Blo, RG3W, parw00, CUTX16(parl01));
      skip00967 = true;
    }
  }

  if (skip00967)
  {
    //m_44286:
    DM2_DRAW_PIT_ROOF(0);
    RG1P = ddat.ptr1e1044;
    RG2L = signedlong(byte_at(RG1P, 1));
    RG4L = signedlong(byte_at(RG1P));
    RG1L = DM2_GET_TELEPORTER_DETAIL(&c12_00, RG4L, RG2L);
    RG3L = RG1L;
    if (RG1L == 0)
      DM2_DRAW_EXTERNAL_TILE(RG1L);
    DM2_DRAW_STATIC_OBJECT(0, lcon(0x1ffffff), 0);
    RG1L = signedlong(RG5W);
    if (RG1L == lcon(0x5))
      DM2_DRAW_TELEPORTER_TILE(0, lcon(0x18), 0);
  }

  //m_442D0:
  DM2_DRAW_RAIN();
  if (RG3L != 0)
    DM2_DRAW_EXTERNAL_TILE(0);
  return DM2_guivp_32cb_2d8c(unsignedlong(word_at(ddat.ptr1e1044, lcon(0x6))), 0, lcon(0x1ffffff));
}

// belongs to DM2_guivp_32cb_15b8
static void DM2_guivp_32cb_3f0d(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5;
  c_nreg _RG6;
  i32 RG7l;
  c_ql32 vql_00;
  c_o32 vo_04; // poke 16 peek 32 inc16
  i16 vw_08;
  i16 vw_0c;
  i16 parw00;
  i8 parb12;

  vql_00.poke32(RG1L);
  RG4L = signedlong(vql_00.peek16());
  RG1L = 9 * RG4L;
  RG5W = word_at(ddat.ptr1e1044, 2 * RG1L + lcon(0x6));
  if (RG5W == lcon(0xfffffffe))
    return;
  vo_04.poke16(2);
  //m_429AA:
  for (;;)
  {
    RG1L = unsignedlong(RG5W);
    RG4L = RG1L >> bcon(0xe);
    if (RG4L == ((signedlong(ddat.v1e0258) + 2) & lcon(0x3)))
    {
      RG4W = RG5W;
      RG4W &= lcon(0x3c00);
      RG4L = unsignedlong(RG4W) >> 10;
      if (RG4W < 5 || RG4W > 10)
      {
        //m_42A48:
        RG1L = signedlong(RG4W);
        if (RG1L == lcon(0xf))
        {
          RG3W = vql_00.peek16();
          RG1L = signedlong(table1d6e51[RG3W]);
          if (RG1L != lcon(0xffffffff))
          {
            bool skip00935 = false;
            RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG5UW));
            RG1W = word_at(RG1P, 2);
            vw_08 = DM2_RANDDIR();
            RG6L = mkul(RG1Blo & lcon(0x7f));
            if (RG6L != 100)
            {
              //m_42ADF:
              if (RG6L == lcon(0x65))
              {
                RG1W = DM2_guivp_098d_0cd7(RG3W, lcon(0xc), false);
                RG7l = signedlong(RG1W);
                RG4L = unsignedlong(table1d6b76[signedlong(table1d6e51[RG3W]) + 0x58]);
                RG1L = RG6L;
                DM2_guivp_32cb_2cf3(RG1L, RG4L, unsignedlong(vw_08), RG7l);
                skip00935 = true;
              }
            }
            else
            {
              RG1L = signedlong(table1d6e51[RG3W]);
              RG1Blo = table1d6b76[RG1L + 0x5f];
              RG1W = unsignedword(RG1Blo);
              vw_0c = RG1W;
              put8(parb12, RG6Blo);
              RG1L = signedlong(DM2_guivp_098d_0cd7(RG3W, lcon(0xc), false));
              i32 parl07 = RG1L;
              RG1L = unsignedlong(vw_08);
              DM2_QUERY_TEMP_PICST(
                RG1W, vw_0c, vw_0c, 0,
                0, 0, CUTX16(parl07), -1,
                lcon(0xa), lcon(0xffff), lcon(0xd), parb12, lcon(0xc));

              skip00935 = true;
            }

            if (skip00935)
              //m_42B16:
              DM2_DRAW_TEMP_PICST();
          }
        }
      }
      else
      {
        put16(parw00, vo_04.peek16());
        DM2_DRAW_ITEM(RG1L, signedlong(vql_00.peek16()), 0, 0, parw00, NULL, 1, 0, 1);
        vo_04.inc16();
        RG1L = signedlong(vo_04.peek16());
        if (RG1L >= lcon(0xe))
          vo_04.poke16(2);
        if (vql_00.peek16() == 3)
          DM2_MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE(lcon(0x8), unsignedlong(RG5W), signedlong(vql_00.peek16()), signedlong(vql_00.peek16()));
      }
    }
    //m_42B1B:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG5UW)));
    RG5W = RG1W;
    if (RG1W == lcon(0xfffffffe))
      return;
  }
  FOREND
}

// belongs to DM2_guivp_32cb_15b8
// stacksize was 0x64
static void DM2_guivp_32cb_0f82(unk* xeaxp, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1, i16 argw2, i16 argw3, i16 argw4)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG51p;
  i8* RG52p;
  i32 RG53l;
  i8* RG61p;
  c_wlreg _RG62;
  c_nreg _RG63;
  c_lreg _RG64;
  c_wlreg _RG65;
  i8* RG71p;
  i8* RG72p;
  c_wlreg _RG73;
  i32 RG74l;
  c_rect rc_00;
  i8 vba_08[0x8]; // really a bytearray
  i32 vl_10;
  unk* xp_14;
  i32 vl_18;
  i32 vl_1c;
  i16 vw_20; // x0
  i16 vw_24; // y0
  i16 vw_28;
  i16 vw_2c;
  i16 vw_30;
  i16 vw_34;
  i16 vw_38;
  i16 vw_3c;
  c_o32 vo_40; // poke 16 peek 16 32 inc16 and16
  c_ql32 vql_44;
  i32 vl_48;
  i8 vb_4c;
  i8 vb_50;
  i16 parw01;
  i16 parw04;
  i8 parb06;
  i8 parb07;
  i16 parw10;
  i16 parw13;
  i8 parb16;
  i16 parw19;
  i16 parw22;
  i8 parb25;
  i16 parw33;
  i8 parb39;
  i16 parw45;
  i16 parw47;

  RG51p = RG1P;
  vb_50 = RG4Blo;
  vw_38 = RG2W;
  vw_28 = RG3W;
  RG61p = NULL;
  RG1W = word_at(RG1P, lcon(0x6));
  RG4L = RG1L;
  _RG4.ushiftr11();
  RG4L = signedlong(RG4W);
  RG1L <<= bcon(0x5);
  _RG1.ushiftr11();
  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(RG1W, RG4W));
  //m_3ECFC:
  for (;;)
  {
    RG2L = RG1L;
    RG1Blo = RG2W != wcon(0xfffe) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == 0)
      break;
    RG1L = RG2L;
    RG1Blo ^= RG2Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    RG1L = signedlong(RG1W);
    if (RG1L > lcon(0x3))
      break;
    if (RG1L == lcon(0x3))
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
      RG61p = RG1P;
      RG1W = word_at(RG1P, 2) & lcon(0x7f);
      if (RG1W == lcon(0x30))
        break;
    }
    //m_3ED3D:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG2UW)));
  }
  //m_3ED49:
  RG1W = word_at(RG61p, 2);
  RG1UW >>= bcon(0x7);
  vo_40.poke16(RG1W);
  vw_3c = RG1W;
  RG1W = word_at(RG51p, 2);
  RG1UW >>= bcon(0x7);
  vb_4c = RG1Blo;
  RG1Bhi = byte_at(RG51p, lcon(0x4));
  if ((RG1Bhi & bcon(0x1)) == 0 || (RG1Bhi & bcon(0x4)) != 0)
  {
    RG1W = word_at(RG51p, lcon(0x4));
    RG1L <<= bcon(0x5);
    RG1UW >>= bcon(0xc);
    RG4L = unsignedlong(RG1W);
    RG1L = 3 * RG4L + lcon(0x10);
    RG3L = RG1L;
    RG2L = lcon(0x400);
    RG4L = 0;
    RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x40));
    xp_14 = UPCAST(unk, RG1P);
    RG4L = signedlong(RG3W);
    RG1L = DM2_QUERY_CREATURES_ITEM_MASK(unsignedlong(vb_4c), RG4L, RG1P, 0);
    if (RG1L != 0)
    {
      RG1L = unsignedlong(vb_50);
      vl_18 = RG1L;
      RG3L = lcon(0x13);
      RG2L = lcon(0x8);
      RG4L = RG1L;
      RG1P = DM2_QUERY_GDAT_ENTRY_DATA_BUFF(lcon(0x9), RG4Blo, RG2Blo, RG3Blo);
      RG71p = RG1P;
      if (RG1P != NULL)
      {
        RG3L = lcon(0x13);
        RG1L = (DM2_QUERY_GDAT_ENTRY_DATA_LENGTH(lcon(0x9), CUTLX8(vl_18), lcon(0x8), RG3Blo) & lcon(0xffff)) / 2;
        vql_44.poke32(RG1L);
        RG4L = 0;
        vo_40.poke16(RG4W);
        RG2L = vql_44.peek32();
        //m_3EE3A:
        for (;;)
        {
          if (RG2W > vo_40.peek16())
          {
            //m_3EE1A:
            RG1L = signedlong(vo_40.peek16());
            RG1W = word_at(RG71p, 2 * RG1L + 0);
            RG1Bhi &= 0x1;
            RG1L = signedlong(RG1W);
            RG4L = unsignedlong(vw_3c);
            if (RG4L != RG1L)
            {
              vo_40.inc16();
              continue;
            }
          }
          break;
        }
      }
      //m_3EE41:
      for (;;)
      {
        if (jz_test8(location(RG51p + lcon(0x4)), lcon(0x4)) && ddat.v1e12d8 >= wcon(0x0))
          //m_3EE58:
          RG4L = lcon(0xffffffff);
        else
          RG4L = 1;
        //m_3EE5D:
        RG1L = signedlong(vo_40.peek16()) + RG4L;
        vo_40.poke16(RG1W);
        if (RG71p == NULL)
        {
          //m_3EE9F:
          vo_40.and16(lcon(0x1ff)); // modified
          RG1W = vo_40.peek16();
        }
        else
        {
          if (RG1W >= 0)
          {
            //m_3EE80:
            if (RG1W >= vql_44.peek16())
            {
              RG3L = 0;
              vo_40.poke16(RG3W);
            }
          }
          else
          {
            RG1L = vql_44.peek32() - 1;
            vo_40.poke16(RG1W);
          }
          //m_3EE8E:
          RG1L = signedlong(vo_40.peek16());
          RG1W = word_at(RG71p, 2 * RG1L + 0);
          RG1Bhi &= 0x1;
        }
        //m_3EEA8:
        vw_3c = RG1W;
        RG2L = unsignedlong(vw_3c);
        RG1P = DOWNCAST(unk, xp_14) + RG2L / 8;
        RG1Blo = byte_at(RG1P);
        RG1L = unsignedlong(RG1Blo);
        RG3Blo = CUTX8(vw_3c) & lcon(0x7);
        RG4Blo = 1 << RG3Blo;
        RG4L = unsignedlong(RG4Blo);
        if ((RG4L & RG1L) != 0)
        {
          RG1L = DM2_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG2L);
          RG4L = unsignedlong(RG1Blo);
          RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(DM2_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(RG2L), RG4Blo, 11, 0);
          RG1W &= lcon(0x1000);
          RG1L = unsignedlong(RG1W);
          if (RG1L != 0)
          {
            RG3L = signedlong(vw_3c);
            RG2L = 1;
            RG4L = RG3L;
            RG1L = DM2_IS_DISTINCTIVE_ITEM_ON_ACTUATOR(RG51p, RG4L, RG2L);
            if (RG1L == 0)
            {
              RG2L = 0;
              RG4L = RG3L;
              RG1L = DM2_IS_DISTINCTIVE_ITEM_ON_ACTUATOR(RG61p, RG4L, RG2L);
              if (RG1L == 0)
              {
                RG1L = DM2_query_2fcf_16ff(RG3L);
                if (RG1L == 0)
                  continue;
              }
            }
          }
          break;
        }
      }
      //m_3EF43:
      and8(location(RG51p + lcon(0x4)), lcon(0xfffffffb));
    }
    //m_3EF47:
    or8(location(RG51p + lcon(0x4)), 1);
    or8(location(RG61p + lcon(0x4)), 1);
    RG1W = vw_3c;
    RG1Bhi &= 0x1;
    RG4W = word_at(RG61p, 2);
    RG4L &= lcon(0xffff007f);
    RG1L <<= bcon(0x7);
    RG2L = RG4L | RG1L;
    mov16(location(RG61p + 2), RG2W);
    dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0x40));
  }
  //m_3EF75:
  RG1L = unsignedlong(vb_50);
  vl_10 = RG1L;
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0x9), RG1Blo, lcon(0xc), lcon(0xfd));
  RG2W = RG1W;
  vw_34 = RG1W;
  RG4L = unsignedlong(vw_3c);
  RG1L = DM2_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG4L) & lcon(0xff);
  put8(parb07, RG1Blo);
  RG1Blo = DM2_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(RG4L) & lcon(0xff);
  put8(parb06, RG1Blo);
  RG1L = signedlong(argw4);
  put16(parw04, RG1W);
  i32 parl03 = signedlong(argw3);
  i32 parl02 = signedlong(argw2);
  RG1L = signedlong(vw_38);
  put16(parw01, RG1W);
  i32 parl00 = signedlong(CUTX8(vw_34));
  RG1L = unsignedlong(RG2W) >> bcon(0x8);
  RG3L = signedlong(RG1Blo);
  DM2_QUERY_TEMP_PICST(
    0, argw0, argw1, RG3W,
    CUTX16(parl00), parw01, CUTX16(parl02), CUTX16(parl03),
    parw04, lcon(0xffff), parb06, parb07, 0);

  DM2_DRAW_TEMP_PICST();
  RG3L = signedlong(vw_3c);
  RG2L = 1;
  RG4L = RG3L;
  RG1L = DM2_IS_DISTINCTIVE_ITEM_ON_ACTUATOR(RG51p, RG4L, RG2L);
  if (RG1L == 0)
  {
    RG2L = 0;
    RG4L = RG3L;
    RG1L = DM2_IS_DISTINCTIVE_ITEM_ON_ACTUATOR(RG61p, RG4L, RG2L);
    if (RG1L == 0)
    {
      bool skip00885 = false;
      RG3L = lcon(0x13);
      RG2L = lcon(0x8);
      RG4L = vl_10;
      RG1P = DM2_QUERY_GDAT_ENTRY_DATA_BUFF(lcon(0x9), RG4Blo, RG2Blo, RG3Blo);
      RG72p = RG1P;
      if (RG1P == NULL)
        skip00885 = true;
      else
      {
        RG3L = lcon(0x13);
        RG2L = lcon(0x8);
        RG4L = vl_10;
        RG1L = (DM2_QUERY_GDAT_ENTRY_DATA_LENGTH(lcon(0x9), RG4Blo, RG2Blo, RG3Blo) & lcon(0xffff)) / 2;
        vql_44.poke32(RG1L);
        RG3L = 0;
        vo_40.poke16(RG3W);
        RG62L = vql_44.peek32();
        //m_3F0C1:
        for (;;)
        {
          if (RG62W > vo_40.peek16())
          {
            //m_3F09F:
            RG1L = signedlong(vo_40.peek16());
            RG1W = word_at(RG72p, 2 * RG1L + 0);
            RG1Bhi &= 0x1;
            RG4L = signedlong(RG1W);
            RG1L = unsignedlong(vw_3c);
            if (RG1L != RG4L)
            {
              vo_40.inc16();
              continue;
            }
          }
          break;
        }
        //m_3F0C8:
        RG1L = signedlong(word_at(RG72p, 2 * signedlong(vo_40.peek16()) + 0));
        if ((RG1Bhi & bcon(0x80)) == 0)
          skip00885 = true;
        else
        {
          RG1L = DM2_ALLOC_NEW_DBITEM_DEEPLY(unsignedlong(vw_3c));
          if (RG1W == lcon(0xffffffff))
            skip00885 = true;
          else
          {
            RG4W = word_at(RG51p, lcon(0x6));
            RG2L = RG4L;
            _RG2.ushiftr11();
            RG3L = signedlong(RG2W);
            RG4L <<= bcon(0x5);
            _RG4.ushiftr11();
            RG2L = signedlong(RG4W);
            RG1L = unsignedlong(RG1W);
            DM2_APPEND_RECORD_TO(mk_record(RG1L), NULL, RG2W, RG3W);
          }
        }
      }

      if (skip00885)
      {
        //m_3F110:
        RG1L = unsignedlong(vb_50);
        put8(parb16, RG1Blo);
        RG1L = signedlong(argw4);
        put16(parw13, RG1W);
        i32 parl12 = signedlong(argw3);
        i32 parl11 = signedlong(argw2);
        RG1L = signedlong(vw_38);
        put16(parw10, RG1W);
        i32 parl09 = signedlong(CUTX8(vw_34));
        RG1L = unsignedlong(vw_34) >> bcon(0x8);
        RG3L = signedlong(RG1Blo);
        DM2_QUERY_TEMP_PICST(
          0, argw0, argw1, RG3W,
          CUTX16(parl09), parw10, CUTX16(parl11), CUTX16(parl12),
          parw13, lcon(0xffff), lcon(0x9), parb16, lcon(0x11));

        DM2_DRAW_TEMP_PICST();
      }
    }
  }
  //m_3F17B:
  RG63L = unsignedlong(vb_50);
  put8(parb25, RG63Blo);
  RG1L = signedlong(argw4);
  put16(parw22, RG1W);
  i32 parl21 = signedlong(argw3);
  RG73L = signedlong(argw2);
  i32 parl20 = RG73L;
  RG1L = signedlong(vw_38);
  put16(parw19, RG1W);
  DM2_QUERY_TEMP_PICST(
    0, argw0, argw1, 0,
    0, parw19, CUTX16(parl20), CUTX16(parl21),
    parw22, lcon(0xffff), lcon(0x9), parb25, lcon(0xf));

  DM2_DRAW_TEMP_PICST();
  RG1L = signedlong(vw_28);
  if (RG1L == lcon(0x3))
  {
    DM2_QUERY_TOPLEFT_OF_RECT(RG73W, OO vw_20, OO vw_24);
    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, CUTLX8(RG63L), 12, lcon(0xf2));
    vw_34 = RG1W;
    RG1L = unsignedlong(RG1W) >> bcon(0x8);
    RG1Bhi = sgn8(RG1Blo);
    vw_20 += RG1W;
    vw_24 += signedword(CUTX8(vw_34));
    RG4L = unsignedlong(vw_3c);
    RG1L = DM2_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG4L);
    RG2L = unsignedlong(RG1Blo);
    RG1T = DM2_QUERY_GDAT_ITEM_NAME(DM2_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(RG4L) & lcon(0xff), RG2L);
    DM2_DRAW_TEXT_TO_BACKBUFF(signedlong(vw_20), signedlong(vw_24), RG1T);
  }
  //m_3F264:
  RG1L = signedlong(vw_28);
  if (RG1L != lcon(0x3))
    return;
  RG1W = word_at(RG51p, lcon(0x4));
  RG1L <<= bcon(0x5);
  RG1UW >>= bcon(0xc);
  RG2L = signedlong(RG1W);
  RG64L = unsignedlong(vb_4c);
  RG74l = signedlong(vw_3c);
  RG1L = DM2_query_48ae_05ae(RG74l, RG64L, RG2L, 1, 1, lcon(0xffffffff));
  vl_1c = RG1L;
  RG1W = word_at(RG51p, lcon(0x4));
  RG1L <<= bcon(0x5);
  RG1UW >>= bcon(0xc);
  RG2L = signedlong(RG1W);
  RG3L = 0;
  RG4L = RG64L;
  RG1L = DM2_query_48ae_05ae(RG74l, RG4L, RG2L, RG3L, 1, lcon(0xffffffff));
  vl_48 = RG1L;
  RG65L = 0;
  //m_3F45C:
  for (;;)
  {
    RG1L = signedlong(RG65W);
    if (RG1L >= 2)
      return;
    //m_3F2CC:
    RG1Blo = (RG65W == wcon(0x0) ? 1 : 0) + lcon(0xfffffff0);
    RG3L = unsignedlong(RG1Blo);
    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, vb_50, 12, RG3Blo);
    vw_34 = RG1W;
    RG1L = unsignedlong(RG1W) >> bcon(0x8);
    RG1Bhi = sgn8(RG1Blo);
    vw_20 = RG1W;
    vw_24 = signedword(CUTX8(vw_34));
    if (RG65W == 0)
      //m_3F317:
      RG1L = vl_48;
    else
      RG1L = vl_1c;
    //m_3F31B:
    if (RG1W != 0)
    {
      RG4L = signedlong(RG65W);
      i32 parl31 = RG4L;
      RG1L = signedlong(RG1W);
      DM2_query_48ae_0767(RG1L, lcon(0x6), UPCAST(i16, ADRESSOF(i8, vba_08)), ADRESSOF(i16, &vw_2c)); // parl31 unused
      if (vw_2c > wcon(0x0))
      {
        RG1W = /*TAG dbidx*/ dm2_dballochandler.DM2_dballoc_3e74_5888();
        vw_30 = RG1W;
        RG4L = signedlong(mkw(8 * vw_2c));
        RG53l = signedlong(RG1W);
        DM2_ALLOC_NEW_BMP(CUTX16(RG53l), RG4W, lcon(0x7), BPP_4);
        RG1L = unsignedlong(vb_50);
        put8(parb39, RG1Blo);
        RG1L = signedlong(vw_38);
        put16(parw33, RG1W);
        RG3L = 0;
        RG2L = lcon(0x40);
        DM2_QUERY_TEMP_PICST(
          0, RG2W, RG2W, RG3W,
          0, parw33, -1, argw3,
          lcon(0xffff), lcon(0xffff), lcon(0x9), parb39, lcon(0x10));

        RG52p = DOWNCAST(t_bmp, dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ CUTX16(RG53l)));
        RG2L = lcon(0x7);
        rc_00.set_origin(8, RG2W);
        RG1L = 0;
        vo_40.poke16(RG1W);
        //m_3F3FB:
        for (;;)
        {
          RG1W = vo_40.peek16();
          if (RG1W >= vw_2c)
          {
            RG1L = signedlong(argw4);
            put16(parw47, RG1W);
            put16(parw45, argw3);
            RG1W = argw2; // adj
            RG1Bhi |= lcon(0xffffff80);
            RG2L = signedlong(RG1W);
            RG3P16 = &vw_20;
            RG4R = &rc_00;
            RG1R = DM2_QUERY_BLIT_RECT(UPCAST(t_bmp, RG52p), RG4R, RG2W, IO vw_20, IO vw_24, parw45);
            RG3L = 0;
            RG2R = RG1R;
            DM2_DRAW_DIALOGUE_PICT(UPCAST(t_bmp, RG52p), gfxsys.bitmapptr, RG2R, RG3W, 0, parw47, dm2_image2.palette);
            dm2_dballochandler.DM2_dballoc_3e74_58a3((t_dbidx)vw_30);
            break;
          }
          //m_3F3C8:
          RG1L = signedlong(vo_40.peek16());
          RG1Blo = vba_08[RG1L];
          RG1W = unsignedword(RG1Blo);
          RG1L *= 8;
          RG3L = signedlong(RG1W);
          RG2R = &rc_00;
          DM2_DRAW_DIALOGUE_PICT(dm2_image2.imgdesc.bmp, UPCAST(t_bmp, RG52p), RG2R, RG3W, 0, lcon(0xffff), NULL);
          rc_00.x += 8;
          vo_40.inc16();
        }
      }
    }
    //m_3F45B:
    RG65L++;
  }
  FOREND
}

// basically to DM2_DRAW_WALL_TILE, but also called by an event-routine
i32 DM2_guivp_32cb_15b8(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG51w;
  i32 RG52l;
  i16 RG53w;
  i32 RG54l;
  i16 RG55w;
  c_nreg _RG56;
  i8* RG61p;
  c_wlreg _RG62;
  i16 RG63w;
  c_wlreg _RG64;
  i32 RG71l;
  i8* RG72p;
  c_ql32 vql_00;
  c_image image1;
  c_image image2;
  t_text tarr_27c[0x50];
  c_rect rc_2cc;
  i32 vl_2d4;
  i32 vl_2d8;
  i32 vl_2dc;
  i32 vl_2e0;
  i32 vl_2e4;
  i32 vl_2e8;
  t_bmp* bmp_2ec;
  i32 vl_2f0;
  i32 vl_2f4;
  i32 vl_2f8;
  i32 vl_2fc;
  i32 vl_300;
  i16 vw_304;
  i16 vw_308;
  i16 vw_30c;
  i16 vw_310;
  i16 vw_314;
  i16 vw_318;
  i16 vw_31c;
  i16 vw_320;
  i16 vw_324;
  i16 vw_328;
  i32 vl_32c;
  i16 vw_330;
  i16 vw_334;
  i16 vw_338;
  i8 vb_33c;
  i16 vw_340;
  i8 vb_344;
  i8 vb_348; // add8
  i16 parw01;
  i8 parb07;
  i16 parw10;
  i16 parw13;
  i8 parb16;
  i8 parb17;
  i16 parw20;
  i8 parb31;
  i16 parw48;
  i16 parw51;
  i8 parb54;
  i8 parb55;
  i16 parw56;
  i16 parw57;
  i16 parw58;
  i16 parw59;
  i16 parw60;
  i16 parw62;
  i16 parw65;
  i8 parb68;
  bool skip00887 = false;

  vql_00.poke32(RG1L);
  RG51w = RG4W;
  RG71l = RG2L;
  RG1L = signedlong(RG4W);
  if (RG1L > lcon(0xffffffff))
  {
    //m_3F4AE:
    if (RG1L < 1)
      //m_3F4BA:
      RG4L = 5;
    else
      RG4L = 6;
  }
  else
    RG4L = 4;
  //m_3F4BF:
  RG2L = signedlong(vql_00.peek16());
  RG1L = 18 * RG2L;
  RG3P = ddat.ptr1e1044 + RG1L;
  RG1L = signedlong(RG4W);
  RG4W = word_at(RG3P, 2 * RG1L + 2);
  RG1L = signedlong(RG4W) >> bcon(0x8);
  vb_33c = RG1Blo;
  RG1W = signedword(table1d6b15[RG2L]);
  vw_310 = RG1W;
  vb_344 = RG4Blo;
  if (RG4Blo == lcon(0xffffffff))
    return lcon(0xffffffff);
  RG1Blo = RG4Blo == bcon(0x0) ? 1 : 0;
  RG2L = unsignedlong(RG1Blo);
  vl_300 = RG2L;
  RG4L = unsignedlong(vb_344);
  RG1L = DM2_IS_WALL_ORNATE_ALCOVE(RG4L);
  vw_308 = RG1W;
  if (RG2L == 0)
    DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, RG4Blo, 11, lcon(0x7)); // TODO: no return-assignment?
  vl_2fc = ddat.v1d6c15;
  RG62W = wcon(0x0);
  RG4L = vl_300;
  if (RG4L != 0)
    skip00887 = true;
  else
  {
    RG4Blo = vb_344;
    RG3L = 4;
    RG2L = lcon(0xb);
    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, RG4Blo, RG2Blo, RG3Blo);
    vw_324 = RG1W;
    if (RG1W == 0)
      skip00887 = true;
  }

  if (skip00887)
    //m_3F594:
    vw_324 = ddat.v1d6c00;
  bool skip00889 = false;
  //m_3F5A2:
  if (vl_300 != 0)
    skip00889 = true;
  else
  {
    RG4L = unsignedlong(vb_344);
    RG3L = 5;
    RG2L = lcon(0xb);
    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, RG4Blo, RG2Blo, RG3Blo);
    if (RG1W != 0)
    {
      //m_3F5DF:
      RG4L = unsignedlong(RG1W) >> bcon(0x8);
      vw_338 = RG4W;
      RG1W = unsignedword(RG1Blo);
      RG1L--;
    }
    else
      skip00889 = true;
  }

  if (skip00889)
  {
    //m_3F5CE:
    RG1L = lcon(0xc);
    RG4L = 0;
    vw_338 = RG4W;
  }
  bool skip00891 = false;
  //m_3F5F2:
  RG4Blo = RG51w != wcon(0x0) ? 1 : 0;
  RG2L = unsignedlong(RG4Blo);
  RG4L = signedlong(RG1W);
  RG1L = signedlong(vql_00.peek16());
  vl_2f8 = RG1L;
  RG1W = DM2_guivp_098d_0cd7(RG1W, RG4W, RG51w != wcon(0x0));
  vw_30c = RG1W;
  vw_334 = RG1W;
  vl_2f0 = signedlong(vw_310);
  RG1W = unsignedword(RG1Blo);
  RG1Blo = table1d6b71[vl_2f0];
  vw_318 = RG1W;
  vw_31c = RG1W;
  RG1L = signedlong(vw_308);
  if (RG1L == lcon(0x3) && RG51w == wcon(0x0))
  {
    RG4W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, vb_344, 12, lcon(0xfd));
    RG2L = vl_2f8;
    RG1L = 8 * RG2L;
    RG2L += RG1L;
    RG2L = 2 * RG2L;
    RG1P = ddat.ptr1e1044;
    RG1Blo = byte_at(RG1P, RG2L + lcon(0x10));
    RG1L = unsignedlong(RG1Blo);
    put8(parb07, RG1Blo);
    i32 parl03 = signedlong(vw_338);
    i32 parl02 = signedlong(vw_30c);
    RG1L = vl_2f0;
    put16(parw01, RG1W);
    RG1L = signedlong(RG4Blo);
    i32 parl00 = RG1L;
    RG1L = unsignedlong(RG4W) >> bcon(0x8);
    RG3L = signedlong(RG1Blo);
    RG4L = signedlong(vw_318);
    RG1L = 0;
    DM2_QUERY_TEMP_PICST(
      RG1W, RG4W, RG4W, RG3W,
      CUTX16(parl00), parw01, CUTX16(parl02), CUTX16(parl03),
      lcon(0xffff), lcon(0xffff), lcon(0x16), parb07, 1);

    if (RG71l == 0)
      dm2_image2.alphamask = 0xfffe;
    DM2_DRAW_TEMP_PICST();
    if (RG71l == 0)
      return signedlong(vw_324);
    RG1L = signedlong(vw_310);
    if (RG1L == 1)
    {
      RG2L = signedlong(vql_00.peek16());
      RG4L = 6;
      RG1R = &dm2_image2.rect;
      DM2_guivp_32cb_0a4c(RG1R, RG4L, RG2L);
    }
  }
  //m_3F736:
  RG1L = signedlong(vw_310);
  if (RG1L != 2)
    skip00891 = true;
  else
  {
    RG1L = signedlong(RG51w);
    if (RG1L > lcon(0xfffffffe) && RG1L < 2)
      skip00891 = true;
    else
      vw_31c = 0x72;
  }

  if (skip00891)
  {
    //m_3F75E:
    RG1L = signedlong(vw_310);
    if (RG1L == lcon(0x3))
    {
      RG1L = signedlong(RG51w);
      if (RG1L <= lcon(0xfffffffe) || RG1L >= 2)
        vw_31c = 0x4c;
    }
  }
  bool skip00894 = false;
  //m_3F784:
  if (vl_300 != 0)
  {
    if (RG51w != 0)
    {
      //m_3F7A1:
      RG1L = signedlong(RG51w);
      if (RG1L > lcon(0xffffffff))
      {
        //m_3F7B3:
        RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x8), ddat.v1d6c02, 1, lcon(0xfe)) ? 1 : 0;
        if (RG1L == 0)
        {
          //m_3F7DD:
          vb_348 = lcon(0xfffffffd);
          RG62W = wcon(0x1);
        }
        else
          vb_348 = lcon(0xfffffffe);
      }
      else
        vb_348 = lcon(0xfffffffd);
    }
    else
      vb_348 = lcon(0xfffffffc);
    //m_3F7EA:
    RG1L = unsignedlong(vb_348);
    put8(parb17, RG1Blo);
    RG1L = unsignedlong(ddat.v1d6c02);
    put8(parb16, RG1Blo);
    RG1L = signedlong(vw_324);
    put16(parw13, RG1W);
    i32 parl12 = signedlong(vw_338);
    i32 parl11 = signedlong(vw_334);
    RG1L = signedlong(vw_310);
    put16(parw10, RG1W);
    RG2L = signedlong(vw_318);
    RG4L = signedlong(vw_31c);
    RG1L = unsignedlong(RG62W);
    DM2_QUERY_TEMP_PICST(
      RG1W, RG4W, RG2W, 0,
      0, parw10, CUTX16(parl11), CUTX16(parl12),
      parw13, lcon(0xffff), lcon(0x8), parb16, parb17);

    if (RG71l == 0)
      dm2_image2.alphamask = 0xfffe;
    DM2_DRAW_TEMP_PICST();
    if (RG51w != 0)
      return signedlong(vw_324);
    RG4L = signedlong(vql_00.peek16());
    RG1L = 9 * RG4L;
    RG63w = word_at(ddat.ptr1e1044, 2 * RG1L + lcon(0x10));
    RG1Blo = RG63w != wcon(0xffff) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == 0)
      return signedlong(vw_324);
    RG52l = unsignedlong(vb_348);
    RG2L = unsignedlong(ddat.v1d6c02);
    RG3L = RG52l;
    DM2_QUERY_GDAT_SUMMARY_IMAGE(&image1, 8, RG2Blo, RG3Blo);
    vw_340 = image1.w_1c; // adj
    vw_320 = image1.w_1e; // adj
    DM2_QUERY_GDAT_IMAGE_METRICS(8, ddat.v1d6c02, CUTLX8(RG52l), &vw_314, &vw_304);
    RG1W = /*TAG dbidx*/ dm2_dballochandler.DM2_dballoc_3e74_5888();
    vw_328 = RG1W;
    RG4L = unsignedlong(RG63w);
    RG2L = 2;
    DM2_QUERY_MESSAGE_TEXT(tarr_27c, RG4L, RG2L);
    RG53w = wcon(0x0);
    RG64L = 1;
    //m_3F917:
    for (;;)
    {
      RG1L = signedlong(RG53w);
      RG1Blo = tarr_27c[RG1L];
      RG53w++;
      if (RG1Blo == 0)
        break;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L == lcon(0xa))
        RG64L++;
    }
    //m_3F933:
    RG1W = ddat.v1d273e;
    vl_32c = RG1L;
    RG1W = ddat.v1d2740;
    RG1L += 2;
    vw_330 = RG1W;
    RG2L = signedlong(vw_304);
    RG4L = signedlong(vw_314);
    RG54l = signedlong(vw_328);
    DM2_FILL_ENTIRE_PICT(BMPCAST(DM2_ALLOC_NEW_BMP(CUTX16(RG54l), RG4W, RG2W, BPP_4)), ui8_to_pixel(CUTUX8(vw_324)));
    RG1L = signedlong(vw_330);
    RG4L = signedlong(RG64W) * RG1L / 2;
    RG1W = vw_304;
    RG1L <<= bcon(0x10);
    RG1L >>= bcon(0x11);
    RG1L -= RG4L;
    rc_2cc.y = RG1W;
    RG4L = unsignedlong(ddat.v1d6c02);
    RG2L = 3;
    bmp_2ec = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(8, RG4Blo, RG2Blo));
    RG4L = vl_32c;
    rc_2cc.w = RG4W;
    s_dm2bmpheader* bmpheader = getbmpheader(bmp_2ec);
    rc_2cc.h = bmpheader->height;
    RG72p = DOWNCAST(t_bmp, dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ CUTX16(RG54l)));
    RG61p = ADRESSOF(t_text, tarr_27c);
    //m_3F9FB:
    for (;;)
    {
      RG55w = wcon(0x0);
      //m_3F9FD:
      for (;;)
      {
        RG1P = RG61p + signedlong(RG55w);
        RG4Bhi = byte_at(RG1P);
        if (RG4Bhi != 0)
        {
          RG1Blo = RG4Bhi;
          RG1L = unsignedlong(RG1Blo);
          if (RG1L != lcon(0xa))
          {
            RG55w++;
            continue;
          }
        }
        break;
      }
      //m_3FA17:
      RG1L = vl_32c * signedlong(RG55w);
      if (RG55w != 0)
      {
        RG4W = vw_314;
        RG4L <<= bcon(0x10);
        RG4L >>= bcon(0x11);
        RG1L = signedlong(RG1W) / 2;
        RG4L -= RG1L;
        rc_2cc.x = RG4W;
        if (RG4W >= 0)
        {
          //m_3FA4D:
          for (;;)
          {
            RG55w--;
            if (RG55w == wcon(0xffff))
              break;
            RG4L = unsignedlong(ddat.v1d6c02);
            RG2L = 3;
            t_palette* pal = DM2_QUERY_GDAT_IMAGE_LOCALPAL(8, RG4Blo, RG2Blo);
            RG1L = signedlong(vw_324);
            put16(parw20, RG1W);
            RG1L = unsignedlong(byte_at(RG61p));
            RG61p++;
            t_text t = RG1Blo;
            t = DM2_SKCHR_TO_SCRIPTCHR(t);
            RG1Blo = t;
            RG1W = unsignedword(RG1Blo);
            RG1L *= vl_32c;
            RG3L = signedlong(RG1W);
            RG2R = &rc_2cc;
            RG4P = RG72p;
            DM2_DRAW_DIALOGUE_PICT(bmp_2ec, UPCAST(t_bmp, RG4P), RG2R, RG3W, 0, parw20, pal);
            RG1L = vl_32c;
            rc_2cc.x += RG1W;
          }
        }
        else
        {
          RG1L = signedlong(RG55w);
          RG61p += RG1L;
        }
      }
      //m_3FAB6:
      RG1L = unsignedlong(vw_330);
      rc_2cc.y += RG1W;
      RG1P = RG61p;
      RG61p++;
      if (byte_at(RG1P) == 0)
      {
        DM2_QUERY_GDAT_SUMMARY_IMAGE(&image1, -1, 0, 0);
        image1.imgdesc.bmpid = (t_dbidx)vw_328;
        image1.imgdesc.w_06 = lcon(0xffffffff);
        image1.w_34 = vw_31c;
        image1.w_36 = vw_318;
        DM2_COPY_SMALL_PALETTE(image1.palette, DM2_QUERY_GDAT_IMAGE_LOCALPAL(8, ddat.v1d6c02, lcon(0x3)));
        image1.colors = PAL16;
        DM2_query_32cb_0804(image1.palette, signedlong(vw_310), signedlong(vw_324), lcon(0xffffffff), &image1.colors);
        image1.bmp = gfxsys.bitmapptr;
        image1.query1 = vw_334;
        image1.w_1a = vw_338;
        image1.w_1c = vw_340;
        image1.w_1e = vw_320;
        image1.alphamask = vw_324;
        DM2_DRAW_PICST(DM2_QUERY_PICST_IT(&image1));
        dm2_dballochandler.DM2_dballoc_3e74_58a3((t_dbidx)vw_328);
        return signedlong(vw_324);
      }
    }
    FOREND
  }
  //m_3FBC8:
  if (RG51w != wcon(0x0))
  {
    //m_3FC0E:
    RG4W = unsignedword(RG4Blo);
    vb_348 = RG4Bhi;
    RG1L = signedlong(RG51w);
    if (RG1L >= 1)
    {
      RG1Blo = vb_33c + bcon(0x2);
      RG3L = unsignedlong(RG1Blo);
      RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x9), vb_344, 1, RG3Blo) ? 1 : 0;
      if (RG1L == 0)
        //m_3FC52:
        RG62L = 1;
      else
        vb_348 = 2;
    }
  }
  else
  {
    vb_348 = 1;
    if (vl_2fc == 0)
    {
      RG1L = signedlong(vql_00.peek16());
      RG1Blo = table1d6b15[RG1L] & 1;
      RG1L = signedlong(RG1Blo);
      if (RG1L == 0)
      {
        //m_3FC02:
        RG62W = ddat.v1e12d0;
        RG62L ^= 1;
      }
      else
        RG62W = ddat.v1e12d0;
    }
  }
  //m_3FC57:
  RG1Blo = vb_33c;
  vb_348 += RG1Blo;
  if (RG51w == wcon(0x0))
  {
    RG1L = unsignedlong(vb_344);
    vl_2f4 = RG1L;
    RG3L = lcon(0x63);
    RG2L = lcon(0xb);
    RG4L = RG1L;
    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, RG4Blo, RG2Blo, RG3Blo);
    if (RG1W != 0)
    {
      vl_2d4 = signedlong(vw_338);
      vl_2d8 = signedlong(vw_334);
      vl_2dc = signedlong(vw_310);
      vl_2e0 = signedlong(vw_318);
      vl_2e8 = signedlong(vw_31c);
      vl_2e4 = unsignedlong(RG62W);
      DM2_QUERY_TEMP_PICST(
        CUTX16(vl_2e4), CUTX16(vl_2e8), CUTX16(vl_2e0), 0,
        0, vw_310, CUTX16(vl_2d8), CUTX16(vl_2d4),
        lcon(0xfffd), lcon(0xfffd), 9, CUTLX8(vl_2f4), vb_348);

      RG3L = vl_2f4;
      RG1L = signedlong(vw_324);
      RG2L = vl_2dc;
      DM2_QUERY_TEMP_PICST(
        CUTX16(vl_2e4), CUTX16(vl_2e8), CUTX16(vl_2e0), dm2_image2.w_1c,
        dm2_image2.w_1e, RG2W, CUTX16(vl_2d8), CUTX16(vl_2d4),
        RG1W, lcon(0xfffff), 9, RG3Blo, lcon(0xc8));

      if (RG71l == 0)
        dm2_image2.alphamask = 0xfffe;
      // in a window, that's the window-content (landscape) now:
      DM2_DRAW_TEMP_PICST();
      vl_2e4 = unsignedlong(vb_344);
      RG3L = lcon(0x11);
      RG1L = signedlong(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, vb_344, 11, 17));
      i32 parl46 = RG1L;
      vl_2e8 = signedlong(vw_324);
      RG2L = unsignedlong(vb_344);
      RG4L = lcon(0x9);
      RG1W = DM2_QUERY_MULTILAYERS_PIC(
        &image2, 9, RG2Blo, vb_348,
        vw_31c, vw_318, vw_310, RG62W,
        vw_324, /*TAG alphamask*/ CUTX16(parl46));

      vw_328 = RG1W;
      RG3L = signedlong(vw_328);
      DM2_guivp_32cb_0c7d(&image2, signedlong(vw_328), CUTX16(vl_2e8));
      image2.bmp = dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ vw_328);
      DM2_DRAW_PICST(&image2);
      DM2_image_0b36_01cd(&image2.imgdesc);

      if (image2.colors > 0)
      {
        //m_3FE81:
        RG1UBlo = palettecolor_to_ui8(image2.palette[vw_324]);
        RG1L = unsignedlong(RG1Blo);
      }
      else
        RG1L = unsignedlong(vw_324);
      //m_3FE97:
      image2.alphamask = RG1W;
      image2.bmp = gfxsys.bitmapptr;
      RG1W = vw_334;
      image2.query1 = RG1W;
      RG1L = unsignedlong(vw_338);
      image2.w_1a = RG1W;
      RG1L = 0;
      image2.colors = 0;
      RG2L = signedlong(vw_328);
      DM2_SET_IMAGE((t_dbidx)RG2W, &image2.imgdesc);
      DM2_DRAW_PICST(&image2);
      RG3R = &image2.rect;
      dm2_dballochandler.DM2_dballoc_3e74_58a3((t_dbidx)RG2W);
      skip00894 = true;
    }
  }

  if (!skip00894)
  {
    //m_3FF0A:
    RG1L = unsignedlong(vb_348);
    put8(parb55, RG1Blo);
    RG1L = unsignedlong(vb_344);
    put8(parb54, RG1Blo);
    RG1L = signedlong(vw_324);
    put16(parw51, RG1W);
    i32 parl50 = signedlong(vw_338);
    i32 parl49 = signedlong(vw_334);
    RG1L = signedlong(vw_310);
    put16(parw48, RG1W);
    RG2L = signedlong(vw_318);
    RG4L = signedlong(vw_31c);
    RG1L = unsignedlong(RG62W);
    DM2_QUERY_TEMP_PICST(
      RG1W, RG4W, RG2W, 0,
      0, parw48, CUTX16(parl49), CUTX16(parl50),
      parw51, lcon(0xffff), lcon(0x9), parb54, parb55);

    if (RG71l == 0)
      dm2_image2.alphamask = 0xfffe;
    DM2_DRAW_TEMP_PICST();
    RG3R = &dm2_image2.rect;
    if (RG71l == 0)
      return signedlong(vw_324);
  }

  //m_3FF8F:
  if (RG71l != 0)
  {
    RG1L = signedlong(vw_308);
    if (RG1L != lcon(0x3))
    {
      RG1L = signedlong(vql_00.peek16());
      if (RG1L == 1 || RG1L == 2 || RG1L == lcon(0x3))
      {
        RG2L = signedlong(vql_00.peek16());
        RG4L = 6;
        RG1R = RG3R;
        DM2_guivp_32cb_0a4c(RG1R, RG4L, RG2L);
      }
    }
  }
  if (vw_308 == 0)
    return signedlong(vw_324);
  if (RG51w != wcon(0x0))
    return signedlong(vw_324);
  dm2_image1 = dm2_image2; // copy image
  RG56L = unsignedlong(vb_344);
  RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x9), CUTLX8(RG56L), 1, lcon(0xf)) ? 1 : 0;
  if (RG1L != 0)
  {
    RG1L = signedlong(vw_308);
    if (RG1L == 2)
    {
      RG1L = signedlong(vw_324);
      put16(parw60, RG1W);
      RG1L = signedlong(vw_338);
      put16(parw59, RG1W);
      RG1L = unsignedlong(vw_334);
      put16(parw58, RG1W);
      RG1L = signedlong(vw_318);
      put16(parw57, RG1W);
      RG1L = signedlong(vw_31c);
      put16(parw56, RG1W);
      RG3L = signedlong(vql_00.peek16());
      RG2L = signedlong(vw_310);
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(word_at(ddat.ptr1e1044, 18 * RG3L + lcon(0x10)))));
      DM2_guivp_32cb_0f82(RG1P, RG56L, RG2L, RG3L, parw56, parw57, parw58, parw59, parw60);
      return signedlong(vw_324);
    }
    put8(parb68, RG56Blo);
    RG1L = signedlong(vw_324);
    put16(parw65, RG1W);
    i32 parl64 = signedlong(vw_338);
    i32 parl63 = signedlong(vw_334);
    put16(parw62, vw_310);
    RG2L = signedlong(vw_318);
    RG4L = signedlong(vw_31c);
    RG1L = unsignedlong(RG62W);
    DM2_QUERY_TEMP_PICST(
      RG1W, RG4W, RG2W, 0,
      0, parw62, CUTX16(parl63), CUTX16(parl64),
      parw65, lcon(0xffff), lcon(0x9), parb68, lcon(0xf));

    if (RG71l == 0)
      dm2_image2.alphamask = 0xfffe;
    DM2_DRAW_TEMP_PICST();
    return signedlong(vw_324);
  }
  //m_40105:
  RG1L = signedlong(vw_308);
  if (RG1L == 1)
    DM2_guivp_32cb_3f0d(signedlong(vql_00.peek16()));
  return signedlong(vw_324);
}

// belongs to DM2_DRAW_DUNGEON_TILES
// was SKW_32cb_50fe
static void DM2_DRAW_WALL_TILE(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;

  RG5w = RG1W;
  RG3L = signedlong(RG1W);
  DM2_DRAW_WALL(RG3L);
  RG1L = 0;
  RG1Blo = table1d7012[RG3L];
  if (mkub(RG1Blo) < mkub(2))
  {
    //m_43F17:
    if (RG1Blo != 1)
      return;
    RG2L = 1;
    RG4L = 0;
    RG1L = RG3L;
  }
  else
  {
    if (mkub(RG1Blo) > mkub(2))
    {
      if (RG1Blo != lcon(0x3))
        return;
      //m_43F26:
      DM2_guivp_32cb_15b8(RG3L, 0, 1);
    }
    //m_43F34:
    RG1L = signedlong(RG5w);
    RG4L = signedlong(table1d6afe[RG1L]);
    RG2L = 1;
  }
  //m_43F45:
  DM2_guivp_32cb_15b8(RG1L, RG4L, RG2L);
}

// belongs to DM2_ENVIRONMENT_DISPLAY_ELEMENTS
// was SKW_32cb_5598
// stacksize was 0x18
static i32 DM2_ENVIRONMENT_SET_DISTANT_ELEMENT(unk* xeaxp, t_text* edxtp, i32 ebxl, i32 ecxl, i32 argl0)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4T = edxtp;
  i8* RG5p;
  t_text* RG6t;
  i16 vw_00;
  i16 vw_04;
  i16 vw_08;

  RG5p = RG1P;
  RG6t = UPCAST(t_text, RG4P);
  vw_00 = RG2W;
  mov16(location(RG1P + lcon(0x6)), 0);
  RG1W = word_at(RG1P, lcon(0x6));
  mov16(location(RG5p + lcon(0x4)), RG1W);
  RG4T = v1d1128;
  RG1L = DM2_QUERY_CMDSTR_TEXT(RG6t, RG4T);
  if (RG1W == 0)
  {
    mov8(location(RG5p + lcon(0x8)), lcon(0x40));
    mov8(location(RG5p + lcon(0x9)), lcon(0x40));
    return 1;
  }
  RG1L = signedlong(RG1W);
  if (RG1L != 1)
    return 0;
  RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
  RG3L += unsignedlong(byte_at(RG1P, lcon(0x6)));
  vw_04 = RG3W;
  RG1Blo = byte_at(RG1P, lcon(0x7));
  RG1W = unsignedword(RG1Blo);
  RG4L = argl0 + RG1L;
  vw_08 = RG4W;
  RG4T = v1d112b;
  RG1L = DM2_QUERY_CMDSTR_TEXT(RG6t, RG4T);
  RG2L = RG1L;
  RG4T = v1d112e;
  RG1L = signedlong(CUTX16(DM2_QUERY_CMDSTR_TEXT(RG6t, RG4T)));
  RG3L = DM2_guivp_32cb_54ce(signedlong(vw_00), &vw_04, &vw_08, signedlong(RG2W), RG1L);
  RG1L = signedlong(vw_08);
  if (RG1L < 1)
    return 0;
  RG4T = v1d1131;
  RG1L = DM2_QUERY_CMDSTR_TEXT(RG6t, RG4T);
  RG4L = RG3L - RG1L;
  RG1L = RG4L;
  RG4L = signedlong(CUTX16(lcon(0x40) - RG1L));
  RG1L = signedlong(DM2_MAX(1, RG4W));
  RG4L = unsignedlong(RG1W) << 7;
  RG1L = RG4L;
  RG1L -= jul_shl32(RG4L, lcon(0x6));
  RG1L >>= bcon(0x6);
  RG2L = (RG1L + 1) / 2;
  RG1L = 14 * signedlong(vw_04);
  RG4L = 15 * RG1L;
  RG1L = RG4L;
  RG4L = RG1L % signedlong(RG3W);
  RG1L /= signedlong(RG3W);
  mov16(location(RG5p + lcon(0x4)), RG1W);
  mov8(location(RG5p + lcon(0x8)), RG2Blo);
  mov8(location(RG5p + lcon(0x9)), RG2Blo);
  return 1;
}

// belongs to DM2_DISPLAY_VIEWPORT
// was SKW_32cb_5824
static void DM2_ENVIRONMENT_DISPLAY_ELEMENTS(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG51p;
  i32 RG52l;
  i8* RG53p;
  i32 RG6l;
  i32 RG7l;
  c_ql32 vql_00;
  c_ql32 vql_04;
  c_ql32 vql_08;
  t_text tarr_0c[0x80];
  i32 vl_8c;
  i8 vb_90;

  vql_08.poke32(RG1L);
  vql_04.poke32(RG4L);
  vql_00.poke32(RG2L);
  if (ddat.v1e147f != 0)
  {
    RG51p = DOWNCAST(s_hexa, ddat.v1e143c);
    //m_44765:
    for (;;)
    {
      if (byte_at(RG51p) == lcon(0xffffffff))
        break;
      RG2L = signedlong(vql_04.peek16());
      RG4L = signedlong(vql_08.peek16());
      RG1P = RG51p;
      RG51p += 10;
      DM2_ENVIRONMENT_DRAW_DISTANT_ELEMENT(RG1P, RG2L, signedlong(vql_00.peek16()), RG4L);
    }
  }
  //m_4478B:
  RG1L = signedlong(ddat.v1d7041);
  RG4L = unsignedlong(ddat.v1d6c02);
  if (RG1L != RG4L)
  {
    ddat.v1d7041 = RG4Blo;
    ddat.v1e13e9 = 1;
    DM2_ZERO_MEMORY(ddat.v1e13dc, lcon(0xd));
    vb_90 = 0;
    //m_4482C:
    for (;;)
    {
      RG2Bhi = vb_90;
      if (mkub(RG2Bhi) > mkub(lcon(0x63)))
        break;
      //m_447CA:
      RG52l = unsignedlong(RG2Bhi);
      RG4L = unsignedlong(ddat.v1d6c02);
      RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x17), RG4Blo, 1, CUTX8(RG52l)) ? 1 : 0;
      if (RG1L != 0)
      {
        RG4L = unsignedlong(ddat.v1d6c02);
        RG2L = RG52l;
        RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x17), RG4Blo, RG2Blo, tarr_0c);
        if (byte_at(RG1P) != 0)
        {
          RG52l >>= bcon(0x3);
          RG1Blo = 1 << (vb_90 & 0x7);
          ddat.v1e13dc[RG52l] |= RG1Blo;
          ddat.v1e13e9 = 0;
        }
      }
      vb_90++;
    }
  }
  //m_44838:
  RG4Blo = ddat.v1e13e9;
  if (RG4Blo != 0)
    return;
  RG53p = DOWNCAST(s_hexa, ddat.v1e143c);
  vb_90 = RG4Blo;
  //m_4490C:
  while (mkub(vb_90) <= mkub(lcon(0x63)))
  {
    //m_44857:
    RG2L = unsignedlong(vb_90);
    RG1L = RG2L / 8;
    RG4L = unsignedlong(ddat.v1e13dc[RG1L]);
    RG1L = 1 << (RG2Blo & 0x7);
    if ((RG1L & RG4L) != 0)
    {
      DM2_QUERY_GDAT_TEXT(bcon(0x17), ddat.v1d6c02, RG2Blo, tarr_0c);
      RG1L = signedlong(vql_00.peek16());
      vl_8c = RG1L;
      i32 parl00 = RG1L;
      RG7l = signedlong(vql_04.peek16());
      RG6l = signedlong(vql_08.peek16());
      RG2L = RG6l;
      RG4P = ADRESSOF(t_text, tarr_0c);
      RG1L = DM2_ENVIRONMENT_SET_DISTANT_ELEMENT(RG53p, RG4T, RG2L, RG7l, parl00);
      if (RG1L != 0)
      {
        RG1Blo = vb_90;
        mov8(location(RG53p), RG1Blo);
        RG4T = v1d1134;
        RG1L = DM2_QUERY_CMDSTR_TEXT(tarr_0c, RG4T);
        mov16(location(RG53p + 2), RG1W);
        RG4T = v1d1137;
        RG1L = DM2_QUERY_CMDSTR_TEXT(tarr_0c, RG4T);
        mov8(location(RG53p + 1), RG1Blo);
        RG2L = RG7l;
        RG4L = RG6l;
        DM2_ENVIRONMENT_DRAW_DISTANT_ELEMENT(RG53p, RG2L, vl_8c, RG4L);
      }
    }
    //m_44905:
    vb_90++;
  }
}

// belongs to DM2_DISPLAY_VIEWPORT
// was SKW_32cb_5146
static i32 DM2_DRAW_DUNGEON_TILES(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;
  c_wlreg _RG5; // W and UW only
  i16 RG6w;
  i32 RG7l;
  c_o32 vo_00; // poke 16 peek 32 inc16
  i16 parw04;
  i8 parb07;

  RG7l = lcon(0x1ffffff);
  RG4L = 0;
  vo_00.poke16(RG4W);
  //m_441B0:
  for (;;)
  {
    bool skip00959 = false;
    bool skip00960 = false;
    RG1L = signedlong(vo_00.peek16());
    if (mkul(RG1L) >= mkul(lcon(0x14)))
      return RG1L;
    //m_43F68:
    DM2_IBMIO_USER_INPUT_CHECK();
    RG1L = signedlong(vo_00.peek16());
    RG6w = unsignedword(table1d7029[RG1L]);
    i32 tmp31 = unsignedlong(table1d7029[RG1L]);
    RG1L = 8 * tmp31;
    RG4L = 2 * (tmp31 + RG1L);
    RG1P = ddat.ptr1e1044;
    RG5W = word_at(RG1P, RG4L + 2);
    RG4W = word_at(RG1P, RG4L + 6);
    if (tmp31 <= 15)
    {
      bool skip00963 = false;
      bool skip00964 = false;
      //m_44028:
      if (RG5UW < mkuw(lcon(0x5)))
      {
        //m_4405C:
        if (RG5UW < mkuw(1))
        {
          //m_44073:
          if (RG5W == 0)
          {
            if (RG4W == lcon(0xfffffffe))
              skip00960 = true;
            else
            {
              RG1L = signedlong(RG6w);
              i32 tmp3 = unsignedlong(RG4W);
              RG2L = lcon(0x1ffffff);
              RG4L = RG1L;
              RG1L = tmp3;
              skip00959 = true;
            }
          }
        }
        else
        {
          if (RG5UW <= mkuw(1))
            skip00963 = true;
          else
          {
            if (RG5W == 2)
            {
              //m_440C3:
              DM2_DRAW_PIT_TILE(signedlong(RG6w));
              skip00963 = true;
            }
          }
        }
      }
      else
      {
        if (RG5UW <= mkuw(lcon(0x5)))
          skip00963 = true;
        else
        {
          if (RG5UW < mkuw(lcon(0x11)))
          {
            //m_4404D:
            if (RG5W == lcon(0x10))
            {
              //m_440D0:
              RG4L = signedlong(RG6w);
              if (RG4L == lcon(0x3))
              {
                RG4L = unsignedlong(ddat.v1e040e[unsignedlong(mkuw(word_at(DOWNCAST(unk, DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(word_at(RG1P, lcon(0x40))))))), 2) & wcon(0x1)))]);
                RG2L = lcon(0xb);
                RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(14, RG4Blo, RG2Blo, 0x40);
                if (RG1W == 0)
                {
                  RG1L = unsignedlong(ddat.v1d6c02);
                  put8(parb07, RG1Blo);
                  RG1L = signedlong(ddat.v1d6c00);
                  put16(parw04, RG1W);
                  RG2L = 0;
                  RG4L = 2;
                  RG1L = signedlong(DM2_QUERY_CREATURE_BLIT_RECTI(3, t_rotate(RG2UW), RG4W));
                  i32 parl02 = RG1L;
                  RG1L = unsignedlong(ddat.v1e12d0);
                  RG2L = lcon(0x2b);
                  DM2_QUERY_TEMP_PICST(
                    RG1W, RG2W, RG2W, 0,
                    0, 2, CUTX16(parl02), -1,
                    parw04, lcon(0xffff), lcon(0x8), parb07, lcon(0x6));

                  DM2_DRAW_TEMP_PICST();
                }
              }
              skip00963 = true;
            }
          }
          else
          {
            if (RG5UW <= mkuw(lcon(0x11)))
              //m_4409C:
              DM2_DRAW_DOOR_TILE(signedlong(RG6w));
            else
            {
              if (RG5UW <= mkuw(lcon(0x12)))
              {
                //m_440A9:
                DM2_DRAW_STAIRS_SIDE(signedlong(RG6w));
                skip00964 = true;
              }
              else
              {
                if (RG5W == lcon(0x13))
                {
                  //m_440B6:
                  DM2_DRAW_STAIRS_FRONT(signedlong(RG6w));
                  skip00964 = true;
                }
              }
            }
          }
        }
      }

      if (skip00963)
      {
        //m_44163:
        RG1L = DM2_DRAW_EXTERNAL_TILE(unsignedlong(RG6w));
        if (RG1L == 0)
          //m_44178:
          RG7l = lcon(0x1ffffff);
        else
          RG7l = lcon(0x1fffc00);
        //m_4417D:
        DM2_DRAW_PIT_ROOF(signedlong(RG6w));
        skip00964 = true;
      }

      if (skip00964)
      {
        //m_44185:
        RG2L = 1;
        RG4L = RG7l;
        DM2_DRAW_STATIC_OBJECT(signedlong(RG6w), RG4L, RG2L);
        RG1L = signedlong(RG5W);
        if (RG1L == lcon(0x5))
        {
          RG2L = 0;
          DM2_DRAW_TELEPORTER_TILE(signedlong(RG6w), 24, RG2L);
        }
      }
    }
    else
    {
      if (RG5UW < mkuw(1))
      {
        //m_43FC9:
        if (RG5W == 0)
        {
          if (RG4W == lcon(0xfffffffe))
            skip00960 = true;
          else
          {
            RG1L = unsignedlong(RG4W);
            RG2L = lcon(0x1ffffff);
            RG4L = signedlong(RG6w);
            skip00959 = true;
          }
        }
      }
      else
      {
        bool skip00961 = false;
        bool skip00962 = false;
        if (RG5UW <= mkuw(2))
          skip00961 = true;
        else
        {
          if (RG5UW >= mkuw(lcon(0x5)))
          {
            if (RG5UW <= mkuw(lcon(0x5)))
              skip00961 = true;
            else
            {
              if (RG5W == lcon(0x11))
                skip00962 = true;
            }
          }
        }

        if (skip00961)
        {
          //m_43FF9:
          DM2_DRAW_EXTERNAL_TILE(unsignedlong(RG6w));
          skip00962 = true;
        }

        if (skip00962)
        {
          //m_44003:
          if (RG4W != lcon(0xfffffffe))
          {
            RG1L = signedlong(RG6w);
            RG2L = lcon(0x1ffffff);
            DM2_SUMMARY_DRAW_CREATURE(unsignedlong(RG4W), RG1L, RG2L);
          }
        }
      }
    }

    if (skip00959)
    {
      //m_43FE7:
      DM2_SUMMARY_DRAW_CREATURE(RG1L, RG4L, RG2L);
      skip00960 = true;
    }

    if (skip00960)
      //m_43FEC:
      DM2_DRAW_WALL_TILE(signedlong(RG6w));
    //m_441AC:
    vo_00.inc16();
  }
  FOREND
}

// belongs to DM2_DISPLAY_VIEWPORT
static void DM2_CHANCE_TABLE_OPERATION(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;
  i16 RG51w;
  c_wlreg _RG61;
  c_wlreg _RG62;
  c_lreg _RG63;
  i32 RG7l;
  i16 vw_00;
  i16 vw_04;
  bool skip00897 = false;

  if (ddat.v1e12ce == 0)
    return;
  if (ddat.v1d4000 != lcon(0xffffffff))
    return;
  DM2_GET_MOUSE_ENTRY_DATA(OO vw_00, OO vw_04, OO ddat.v1e00b2);
  RG61W = vw_00 - ddat.v1d26fc; // adj
  RG51w = vw_04 - ddat.v1d26fe; // adj
  if (RG61W < 0 || RG51w < 0 || RG61W >= gfxsys.backbuffer_w)
    skip00897 = true;
  else
  {
    RG4L = signedlong(ORIG_SHEIGHT) - signedlong(ddat.v1d26fe);
    RG1L = signedlong(RG51w);
    if (RG1L >= RG4L)
      skip00897 = true;
    else
    {
      bool skip00896 = false;
      RG4L = signedlong(gfxsys.backbuffer_w) >> 1; // was long_at(idx(0x1d2708)) >> bcon(0x11);
      RG62L = signedlong(RG61W) - RG4L;
      RG4L = signedlong(gfxsys.backbuffer_h) >> 1; // was long_at(idx(0x1d270a)) >> bcon(0x11);
      RG1L -= RG4L;
      RG51w = RG1W;
      RG1L = signedlong(DM2_ABS(RG62W));
      if (RG1W >= 20)
      {
        //m_40D77:
        if (RG62W >= 0)
        {
          //m_40D9D:
          RG1L = signedlong(DM2_ABS(RG51w));
          if (RG1W >= 15)
          {
            //m_40DB2:
            if (RG51w >= 0)
            {
              RG51w = 2;
              skip00896 = true;
            }
          }
          else
          {
            RG51w = 1;
            skip00896 = true;
          }
        }
        else
        {
          RG1L = signedlong(DM2_ABS(RG51w));
          if (RG1W >= 15)
          {
            //m_40D91:
            if (RG51w >= 0)
            {
              RG51w = 4;
              skip00896 = true;
            }
          }
          else
          {
            RG51w = 5;
            skip00896 = true;
          }
        }
      }
      else
      {
        RG1L = signedlong(DM2_ABS(RG51w));
        if (RG1W < 15)
          skip00897 = true;
        else
        {
          if (RG51w >= 0)
          {
            //m_40D70:
            RG51w = 3;
            skip00896 = true;
          }
        }
      }

      if (!skip00896)
        //m_40D6C:
        RG51w = 0;

      //m_40DBC:
      RG63L = signedlong(RG51w);
      RG1L = unsignedlong(table1d6d54[RG63L]);
      RG1W += ddat.v1e0258;
      RG1W &= 0x3;
      RG4W = ddat.v1e0270;
      RG2W = ddat.v1e0272;
      RG1L = signedlong(RG1W);
      RG4W += table1d27fc[RG1L];
      RG2W += table1d2804[RG1L];
      RG2L = signedlong(RG2W);
      RG1L = DM2_IS_TILE_BLOCKED(dm2_GET_TILE_VALUE(signedlong(RG4W), RG2L) & lcon(0xff));
      if (RG1L != 0)
        skip00897 = true;
      else
      {
        RG1L = unsignedlong(table1d6d4e[RG63L]);
        RG1W += ddat.v1e0258;
        RG1W &= 0x3;
        RG2L = signedlong(RG1W);
        RG1L = signedlong(ddat.v1e0258);
        RG4W = ddat.v1e0272 + table1d2804[RG1L];
        RG7l = signedlong(RG4W);
        RG4W = ddat.v1e0270 + table1d27fc[RG1L];
        RG1L = DM2_IS_CREATURE_MOVABLE_THERE(signedlong(RG4W), RG7l, RG2L, UPCAST(i16, NULL));
        if (RG1L == 0)
          skip00897 = true;
        else
        {
          RG1L = RG63L;
          RG4L = signedlong(table1d6d3c[RG63L]);
          RG1L ^= RG63L;
          RG1Blo = table1d6d48[RG63L];
          DM2_HIGHLIGHT_ARROW_PANEL(RG1L, RG4L, lcon(0x1));
        }
      }
    }
  }

  if (skip00897)
    //m_40E7B:
    RG51w = 6;

  //m_40E80:
  ddat.v1d324a = RG51w;
}

// was SKW_32cb_5d0d
// TODO: not a good name!
void DM2_DISPLAY_VIEWPORT(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG51w;
  i16 RG52w;
  i32 RG53l;
  i16 RG61w;
  i32 RG62l;
  i32 RG7l;
  c_ql32 vql_00;
  i16 vw_04;

  vql_00.poke32(RG1L);
  RG61w = RG4W;
  vw_04 = RG2W;
  ddat.v1e040a = RG4W;
  ddat.v1e040c = RG2W;
  ddat.v1e0406 = ddat.v1e0266;
  RG1L = vql_00.peek32();
  ddat.v1e0402 = RG1W;
  DM2_IBMIO_USER_INPUT_CHECK();
  RG1W = ddat.v1e0286; // TODO: gets extended w/o conversion now
  RG1L = RG1L * lcon(0xa);
  ddat.v1e12d2 = RG1W;
  RG1L = vql_00.peek32();
  ddat.v1e12c8 = RG1W;
  ddat.v1e12cc = RG4W;
  ddat.v1e12ca = RG2W;
  RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x19e));
  ddat.ptr1e1044 = RG1P;
  RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x166), true);
  ddat.v1e1048 = RG1P;
  RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x166));
  ddat.v1e1050 = RG1P;
  DM2_FILL_STR(RG1T, bcon(0xff), 1, lcon(0x166));
  RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x5c), true);
  ddat.v1e1038 = RG1P;
  RG2L = lcon(0x400);
  RG4L = lcon(0x8000);
  RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x5c), true);
  ddat.v1e104c = RG1P;
  ddat.v1e01d8 = true;
  DM2_INIT_BACKBUFF();
  ddat.v1e0404 = lcon(0x9);
  RG51w = wcon(0x0);
  //m_44D12:
  for (;;)
  {
    RG1L = signedlong(RG51w);
    if (RG1L >= lcon(0x9))
    {
      DM2_IBMIO_USER_INPUT_CHECK();
      if (ddat.v1e03a8 == 0 && ddat.v1d4000 == lcon(0xffffffff))
        //m_44D39:
        RG1L = 0;
      else
        RG1L = 1;
      break;
    }
    //m_44CF9:
    RG4L = RG1L;
    RG1L = 3 * RG4L;
    ddat.v1e02f0[RG4L].w_08 = -1;
    ddat.v1e02f0[RG4L].b_0b = 0;
    RG51w++;
  }
  //m_44D3B:
  ddat.v1e12ce = RG1W;
  DM2_CHANCE_TABLE_OPERATION();
  RG52w = wcon(0x16);
  //m_44D69:
  for (;;)
  {
    if (RG52w < 0)
    {
      RG52w = wcon(0x0);
      RG2L = 0;
      RG4Blo = lcon(0xffffffff);
      RG1P = ddat.ptr1e1044;
      if (word_at(RG1P + lcon(0x80)) == 0)
      {
        if (word_at(RG1P + lcon(0x6e)) == 0)
        {
          if (word_at(RG1P + lcon(0x92)) == 0)
            RG4Blo = lcon(0x71);
        }
      }
      RG1P = ddat.ptr1e1044;
      if (word_at(RG1P + lcon(0x4a)) == 0)
      {
        if (word_at(RG1P + lcon(0x38)) == 0)
        {
          if (word_at(RG1P + lcon(0x5c)) == 0)
            RG4Blo = lcon(0x70);
        }
      }
      if (RG4Blo != lcon(0xffffffff))
      {
        RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(8, ddat.v1d6c02, 11, RG4Blo);
        RG4L = unsignedlong(RG1W) >> bcon(0x8);
        RG52w = RG4W;
        RG1W = unsignedword(RG1Blo);
        RG2L = RG1L;
      }
      break;
    }
    //m_44D4D:
    RG2L = signedlong(RG52w);
    DM2_guivp_32cb_4185(signedlong(RG61w), signedlong(vw_04), RG2L, signedlong(ddat.v1e12c8));
    RG52w--;
  }
  //m_44DE0:
  DM2_TRIM_BLIT_RECT(0, 0, 0, RG2W);
  RG2L = signedlong(RG61w);
  RG1L = DM2_SET_GRAPHICS_FLIP_FROM_POSITION(lcon(0x20), RG2L, signedlong(vw_04), signedlong(vql_00.peek16()));
  //m_44E12:
  DM2_DRAW_DUNGEON_GRAPHIC(8, ddat.v1d6c02, 1, wcon(0x2bc), NOALPHA, RG1L == 0 ? 0 : 1);
  ddat.v1e01d0 = false;
  RG4L = signedlong(RG52w);
  DM2_TRIM_BLIT_RECT(0, RG52w, 0, 0);
  RG2L = signedlong(RG61w);
  RG1L = DM2_SET_GRAPHICS_FLIP_FROM_POSITION(1, RG2L, signedlong(vw_04), signedlong(vql_00.peek16()));
  //m_44E6A:
  DM2_DRAW_DUNGEON_GRAPHIC(8, ddat.v1d6c02, 0, wcon(0x2bd), NOALPHA, RG1L == 0 ? 0 : 1);
  RG4L = 0;
  ddat.v1e01d0 = false;
  RG7l = signedlong(vw_04);
  RG53l = signedlong(RG61w);
  RG62l = signedlong(vql_00.peek16());
  ddat.v1e12d0 = CUTX16(DM2_SET_GRAPHICS_FLIP_FROM_POSITION(0, RG53l, RG7l, RG62l));
  DM2_IBMIO_USER_INPUT_CHECK();
  DM2_ENVIRONMENT_DISPLAY_ELEMENTS(RG62l, RG53l, RG7l);
  DM2_guivp_32cb_5c67();
  DM2_guivp_32cb_5a8f();
  DM2_DRAW_DUNGEON_TILES();
  DM2_IBMIO_USER_INPUT_CHECK();
  DM2_DRAW_PLAYER_TILE();
  DM2_IBMIO_USER_INPUT_CHECK();
  dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0x522));
}
