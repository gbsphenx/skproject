#include <uibutton.h>
#include <skstr.h>
#include <gfxstr.h>
#include <gfxblit.h>
#include <uitmouse.h>
#include <dm2data.h>
#include <skeventq.h>
#include <skxrect.h>
#include <bitem.h>
#include <util.h>
#include <sfx.h>
#include <skalocdb.h>
#include <skgdtqdb.h>
#include <bgdat.h>
#include <gfxmain.h>
#include <skrecord.h>
#include <gfxpal.h>
#include <skimage.h>
#include <skhero.h>
#include <skrandom.h>
#include <startend.h>
#include <skmap.h>
#include <sktimer.h>
#include <SK1031.h>
#include <skguivwp.h>
#include <skguidrw.h>

// TODO: NEEDS:
// DM2_DRAW_MAP_CHIP needs that one: (c_gui_vp.h)
void DM2_SUMMARIZE_STONE_ROOM(s_8wordarray* eaxp_s15, i32 edxl, i32 ebxl, i32 ecxl);

void DM2_DRAW_ICON_PICT_BUFF(t_bmp* srcbmp, c_buttongroup* buttongroup, c_rect* blitrect, i16 srcx, i16 srcy, t_alphamask alphamask, e_blitmode blitmode, t_palette* palette)
{
  c_rect rc_00;

  if (blitrect != NULL)
  {
    s_dm2bmpheader* sheader = getbmpheader(srcbmp);

    // assuming destination bitmap doesn't equal gfxsys.dm2screen
    blitter.blit(
      PIXELCAST(srcbmp),
      PIXELCAST(dm2_dballochandler.DM2_GET_BMP(buttongroup->button.dbidx)),
      DM2_OFFSET_RECT(buttongroup, &rc_00, blitrect),
      srcx,
      srcy,
      sheader->width,
      buttongroup->button.r.w,
      alphamask,
      blitmode,
      sheader->res,
      BPP_8,
      palette);

    DM2_ADJUST_BUTTONGROUP_RECTS(buttongroup, blitrect); // Note: this is the clickbutton-effect
  }
}

void DM2_DRAW_ICON_PICT_ENTRY(i8 lpalidx, i8 edxb, i8 ebxb, c_buttongroup* buttongroup, i16 argw0, t_alphamask alphamask)
{
  c_rect rect;

  i16 srcx = 0;
  i16 srcy = 0;

  t_bmp* srcbmp = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(lpalidx, edxb, ebxb));
  c_rect* blitrect = DM2_QUERY_BLIT_RECT(srcbmp, &rect, argw0, IO srcx, IO srcy);

  DM2_DRAW_ICON_PICT_BUFF(
  srcbmp,
  buttongroup,
  blitrect,
  srcx,
  srcy,
  alphamask,
  BLITMODE0,
  DM2_QUERY_GDAT_IMAGE_LOCALPAL(lpalidx, edxb, ebxb));
}

void DM2_DRAW_DIALOGUE_PROGRESS(i32 eaxl)
{
  c_rect rc_00;

  if (dm2_dballochandler.v1e0200)
  {
    DM2_QUERY_EXPANDED_RECT(474, &rc_00);
    i16 wordrg1 = signedlong(rc_00.w) * eaxl / lcon(0x3e8);
    rc_00.w = wordrg1;
    if (wordrg1 > 0 && wordrg1 != ddat.dialog1)
    {
      ddat.dialog1 = wordrg1;
      DM2_FILL_BACKBUFF_RECT(/*TAG blitrect*/ &rc_00, palettecolor_to_pixel(paldat.palette[E_COL09]));
      DM2_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
    }
  }
}

void DM2_DRAW_DIALOGUE_PARTS_PICT(t_bmp* bmp, c_rect* edxrp, i16 ebxw, t_palette* palette)
{
  s_dm2bmpheader* bmpheader = getbmpheader(bmp);

  // assuming gfxsys.bitmapptr != gfxsys.dm2screen
  blitter.blit(
    PIXELCAST(bmp),
    PIXELCAST(gfxsys.bitmapptr),
    /*TAG blitrect*/ edxrp,
    0,
    0,
    bmpheader->width,
    getbmpheader(gfxsys.bitmapptr)->width,
    /*TAG alphamask*/ ebxw,
    BLITMODE0,
    bmpheader->res,
    BPP_8,
    palette);
}

void DM2_DRAW_DIALOGUE_PICT(t_bmp* srcbmp, t_bmp* destbmp, c_rect* ebxrp, i16 ecxw, i16 argw0, i16 argw1, t_palette* palette)
{
  s_dm2bmpheader* sheader = getbmpheader(srcbmp);
  s_dm2bmpheader* dheader = getbmpheader(destbmp);

  ui16 destw;
  if (PIXELCAST(destbmp) == gfxsys.dm2screen)
    destw = ORIG_SWIDTH;
  else
    destw = dheader->width;
  blitter.blit(
    PIXELCAST(srcbmp),
    PIXELCAST(destbmp),
    /*TAG blitrect*/ ebxrp,
    /*TAG srcx*/ ecxw,
    /*TAG srcy*/ argw0,
    sheader->width,
    destw,
    /*TAG alphamask*/ argw1,
    BLITMODE0,
    sheader->res,
    dheader->res,
    palette);
}

// TODO: optimize temporaries
#include "regs.h"
#include "emu.h"

// was SKW_1031_1907
void DM2_DRAW_WAKE_UP_TEXT(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  t_text tarr_00[0x28];

  RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL00]);
  RG4L = unsignedlong(RG4Blo);
  DM2_FILL_ENTIRE_PICT(gfxsys.bitmapptr, ui8_to_pixel(RG4UBlo));
  RG3P = ADRESSOF(t_text, tarr_00);
  RG2L = lcon(0x11);
  RG4L = 0;
  RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x1), bcon(0x0), bcon(0x11), RG3T);
  RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL04]);
  RG4L = unsignedlong(RG4Blo);
  DM2_DRAW_VP_RC_STR(lcon(0x6), RG4W, RG1T);
}

void DM2_DRAW_PLAYER_3STAT_HEALTH_BAR(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  c_wlreg _RG6;
  c_ql32 vql_00;
  i16 vwa_04[0x6];
  c_rect rc_10;
  c_rect rc_18;

  vql_00.poke32(RG1L);
  RG1L += lcon(0xb9);
  RG1L = signedlong(RG1W);
  RG4R = &rc_18;
  RG1R = DM2_QUERY_EXPANDED_RECT(RG1W, RG4R);
  RG4R = RG1R;
  DM2_ADJUST_BUTTONGROUP_RECTS(&dm2_buttongroup1, RG4R);
  c_hero* hero = &party.hero[vql_00.peek16()];
  vwa_04[0] = hero->curHP;
  vwa_04[1] = hero->maxHP;
  vwa_04[2] = hero->curStamina;
  vwa_04[3] = hero->maxStamina;
  vwa_04[4] = hero->curMP;
  RG1L = signedlong(hero->curMP);
  RG2L = signedlong(hero->maxMP);
  RG4L = RG1L;
  RG1L = signedlong(DM2_MAX(RG2W, RG4W));
  vwa_04[5] = RG1W;
  RG6L = vql_00.peek32() + lcon(0xc1);
  RG5w = 0;
  //m_383CE:
  for (;;)
  {
    RG1L = signedlong(RG5w);
    if (RG1L >= 3)
      return;
    //m_38321:
    RG1L *= 4;
    if (vwa_04[RG1L / 2 + 1] != 0)
    {
      RG4L = signedlong(vwa_04[RG1L / 2]) * 10000; // idxadj
      RG2L = signedlong(vwa_04[RG1L / 2 + 1]);
      RG1L = RG4L;
      RG4L = RG1L % RG2L;
      RG1L /= RG2L;
      RG3L = unsignedlong(RG1W);
      RG1L = signedlong(RG6W);
      RG2L = 10000;
      RG4R = &rc_18;
      RG1R = DM2_SCALE_RECT(RG1W, RG2W, RG3W, RG4R);
      if (RG1R != NULL)
      {
        rc_10 = rc_18; // copy
        rc_10.x += ddat.v1d2728;
        rc_10.y += ddat.v1d272a;
        DM2_FILL_RECT_SUMMARY(&dm2_buttongroup1, /*TAG BLITRECT*/ &rc_10, palettecolor_to_pixel(paldat.palette[E_COL00]));
        RG4L = unsignedlong(table1d69d0[unsignedlong(vql_00.peek16())]);
        RG2L = unsignedlong(palettecolor_to_ui8(paldat.palette[RG4L]));
        DM2_FILL_RECT_SUMMARY(&dm2_buttongroup1, /*TAG BLITRECT*/ &rc_18, ui8_to_pixel(RG2UBlo));
      }
    }
    //m_383CA:
    RG5w++;
    RG6L += 4;
  }
  FOREND
}

void DM2_DRAW_CUR_MAX_HMS(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  t_text tarr_00[0x8];

  RG5w = RG1L;
  RG3L = RG2L;
  RG1T = DM2_FMT_NUM(RG4W, 1, 3);
  DM2_STRCPY(tarr_00, RG1T);
  DM2_STRCAT(tarr_00, CHGCAST(t_text, v1d1124));
  RG1T = DM2_FMT_NUM(RG3W, 1, 3);
  DM2_STRCAT(tarr_00, RG1T);
  RG4W = unsignedword(RG4Blo);
  RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL01]);
  RG4Bhi |= lcon(0x40);
  DM2_DRAW_LOCAL_TEXT(signedlong(RG5w), unsignedlong(palettecolor_to_ui8(paldat.palette[E_COL13])), RG4W, tarr_00);
}

void DM2_DRAW_PLAYER_3STAT_TEXT(c_hero* hero)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;

  DM2_DRAW_CUR_MAX_HMS(lcon(0x226), unsignedlong(hero->curHP), unsignedlong(hero->maxHP));
  RG4L = signedlong(hero->maxStamina);
  RG1L = RG4L;
  RG4L = RG1L % 10;
  RG1L /= 10;
  RG2L = unsignedlong(RG1W);
  RG4L = signedlong(hero->curStamina);
  RG1L = RG4L;
  RG4L = RG1L % 10;
  RG1L /= 10;
  RG4L = unsignedlong(RG1W);
  DM2_DRAW_CUR_MAX_HMS(lcon(0x227), RG4L, RG2L);
  DM2_DRAW_CUR_MAX_HMS(lcon(0x228), unsignedlong(hero->curMP), unsignedlong(hero->maxMP));
}

void DM2_DRAW_PLAYER_NAME_AT_CMDSLOT(void)
{
  DM2_DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 4, 20, &dm2_buttongroup2, lcon(0x3c));
  DM2_DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 4, 14, &dm2_buttongroup2, lcon(0x3b));
  e_hero heroidx = (e_hero)(party.curacthero - 1);
  DM2_DRAW_NAME_STR(&dm2_buttongroup2, lcon(0x3d), unsignedword(palettecolor_to_ui8(paldat.palette[(heroidx != eventqueue.event_heroidx) ? E_COL15 : E_COL09])), unsignedword(palettecolor_to_ui8(paldat.palette[E_COL12])) | lcon(0x4000), party.hero[heroidx].name1);
}

void DM2_DRAW_PLAYER_DAMAGE(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5;
  i16 RG6w;
  i16 parw00;
  t_text* par_tp02;

  RG6w = RG1W;
  RG1L += lcon(0xb1);
  RG5L = signedlong(RG1W);
  put16(parw00, RG5W);
  DM2_DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 2, 3, &dm2_buttongroup1, parw00, ALPHA_A);
  RG4L = unsignedlong(RG6w);
  par_tp02 = DM2_FMT_NUM(party.hero[RG4L].damagesuffered, 0, 3); // BUGFIX RG4L
  DM2_DRAW_BUTTON_STR(&dm2_buttongroup1, RG5L, unsignedword(palettecolor_to_ui8(paldat.palette[E_COL15])), unsignedword(palettecolor_to_ui8(paldat.palette[E_COL08])), par_tp02);
}

// stacksize was 0x20
void DM2_DRAW_CHIP_OF_MAGIC_MAP(t_bmp* bmp, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, t_palette* palette)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG6w;
  c_rect rc_00;
  i16 vw_08;
  i16 vw_0c;
  i16 parw00;
  i16 parw01;
  i16 parw03;

  RG6w = RG4W;
  if (bmp == NULL)
    return;
  put16(parw00, ddat.v1d274c);
  RG1L = signedlong(ddat.v1d274a);
  RG4L = signedlong(RG2W);
  RG2L = signedlong(RG3W);
  RG3L = RG1L;
  RG1R = &rc_00;
  RG1R->set(RG4W, RG2W, RG3W, parw00);
  RG3P16 = &vw_08;
  RG2P16 = &vw_0c;
  RG4R = &glblrects.dm2rect3;
  RG1R = RG1R->unify(RG4R, OO vw_0c, OO vw_08);
  if (RG1R == NULL)
    return;
  RG1W = ddat.v1d274a - rc_00.w;
  if (RG1W != 0)
  {
    RG4L = argl0;
    RG4W &= 0x1;
    RG4L = unsignedlong(RG4W);
    if (RG4L != 0)
    {
      if (vw_0c == 0)
        //m_33A9F:
        vw_0c = RG1W; // adj
      else
      {
        RG3L = 0;
        vw_0c = RG3W; // adj
      }
    }
  }
  //m_33AA3:
  RG1W = ddat.v1d274c - rc_00.h;
  if (RG1W != 0)
  {
    RG4L = argl0;
    RG4W &= 0x2;
    RG4L = unsignedlong(RG4W);
    if (RG4L != 0)
    {
      if (vw_08 == 0)
        //m_33AD2:
        vw_08 = RG1W;
      else
      {
        RG3L = 0;
        vw_08 = RG3W;
      }
    }
  }
  //m_33AD7:
  RG1L = unsignedlong(mkuw(argl0)); // truncated TODO check if arg is always word
  put16(parw03, RG1W);
  RG1L = signedlong(vw_08);
  put16(parw01, RG1W);
  RG6w *= ddat.v1d274a;
  RG6w += vw_0c; // adj
  RG3L = signedlong(RG6w);
  RG2R = &rc_00;
  DM2_DRAW_ICON_PICT_BUFF(bmp, &dm2_buttongroup2, /*TAG blitrect*/ RG2R, /*TAG srcx*/ RG3W, /*TAG srcy*/ parw01, 0xa, (e_blitmode)parw03, palette);
}

void DM2_QUERY_GDAT_SQUAD_ICON(t_bmp* bmp, i32 edxl, t_palette* palette)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_rect* RG6r;

  RG3L = unsignedlong(party.hero[RG4W].absdir);
  RG1L = signedlong(ddat.v1e0258);
  RG3L += 4;
  RG3L -= RG1L;
  RG3L &= 0x3;
  if (ddat.savegames1.b_02 == 0)
    //m_38172:
    RG1W = 0;
  else
    RG1W = 4;
  //m_38174:
  DM2_COPY_SMALL_PALETTE(palette, DM2_QUERY_GDAT_IMAGE_LOCALPAL(1, 6, RG4Blo));
  DM2_DRAW_DIALOGUE_PICT(BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(1, 6, RG4Blo)), bmp, tmprects.alloc_origin_tmprect(ddat.v1d272c, ddat.v1d272e), ddat.v1d272c * (RG3W + RG1W), 0, 2, NULL);
}

// was SKW_24a5_105b
void DM2_DRAW_CRYOCELL_LEVER(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG5l;
  i16 parw02;
  i16 parw03;

  RG5l = RG1L;
  RG3L = lcon(0x1ee);
  RG2Blo = (RG1L != 0 ? 1 : 0) + lcon(0xfffffffa);
  RG2L = unsignedlong(RG2Blo);
  DM2_DRAW_STATIC_PIC(9, bcon(0x5b), RG2Blo, RG3W, NOALPHA);
  if (RG5l != 0)
  {
    DM2_DRAWINGS_COMPLETED();
    put16(parw03, ddat.v1e0272);
    put16(parw02, ddat.v1e0270);
    DM2_QUEUE_NOISE_GEN1(lcon(0x9), lcon(0x5b), lcon(0xfb), lcon(0xff), lcon(0xc8), parw02, parw03, 0);
    return;
  }
  ddat.v1d66fc = lcon(0x7);
}

// was SKW_2e62_064a
void DM2_DRAW_CHARSHEET_OPTION_ICON(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  RG3L = unsignedlong(ddat.v1e100c);
  RG2L = unsignedlong(RG2W);
  if ((RG2L & RG3L) != 0)
    RG1Blo++;
  RG3L = signedlong(RG4W);
  RG2L = unsignedlong(RG1Blo);
  DM2_DRAW_STATIC_PIC(7, 0, RG2Blo, RG3W, NOALPHA);
}

// was SKW_24a5_0a06
void DM2_MONEY_BOX_SURVEY(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG3;
  c_nreg _RG2;
  c_nreg _RG4;
  i16 RG5w;
  i32 RG6l;
  i32 RG7l;
  i16 vw_00;
  i16 vwa_04[0xa];
  t_text tarr_18[0xc];

  vw_00 = RG1W; // truncated
  RG7l = 0;
  DM2_COUNT_BY_COIN_TYPES(unsignedlong(vw_00), vwa_04);
  RG3L = lcon(0x26a);
  RG5w = 0;
  //m_2952D:
  for (;;)
  {
    RG4L = signedlong(RG5w);
    if (RG4L >= lcon(0xa))
    {
      RG4P = ADRESSOF(t_text, tarr_18);
      RG1T = DM2_LTOA10(RG7l, RG4T);
      RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL13]);
      RG4L = unsignedlong(RG4Blo);
      DM2_DRAW_VP_RC_STR(lcon(0x239), RG4W, RG1T);
      return;
    }
    //m_294DE:
    RG1L = DM2_GET_ITEM_ORDER_IN_CONTAINER(unsignedlong(vw_00), RG4L);
    if (RG1W >= 0)
    {
      RG1L = signedlong(RG1W);
      RG2L = signedlong(vwa_04[RG1L]);
      RG1L = signedlong(ddat.v1e03ac[RG1L]) * RG2L;
      RG7l += RG1L;
      RG4P = ADRESSOF(t_text, tarr_18);
      RG1T = DM2_LTOA10(RG2L, RG4T);
      RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL13]);
      RG4L = unsignedlong(RG4Blo);
      RG6l = signedlong(RG3W);
      RG3L++;
      RG2P = RG1P;
      RG1L = RG6l;
      DM2_DRAW_VP_RC_STR(RG1W, RG4W, RG2T);
    }
    RG5w++;
  }
  FOREND
}

void DM2_DRAW_MONEYBOX(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG51w;
  c_wlreg _RG52;
  c_lreg _RG61;
  c_wlreg _RG63;
  i16 vw_00;
  i16 vwa_04[0xa];
  c_rect rc_18;
  c_rect rc_20;
  t_palette* palette;
  i32 vl_2c;
  i16 vw_30;
  i16 vw_34;
  c_o32 vo_38; // poke 16 peek 32 inc16
  i16 parw04;

  vw_00 = RG1W;
  RG61L = unsignedlong(RG1W);
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG61L);
  RG4L = unsignedlong(RG1Blo);
  RG3P = DOWNCAST(c_buttongroup, &dm2_buttongroup2);
  RG2L = lcon(0x10);
  DM2_DRAW_ICON_PICT_ENTRY(20, RG4Blo, RG2Blo, UPCAST(c_buttongroup, RG3P), lcon(0x5c));
  RG4P = ADRESSOF(i16, vwa_04);
  RG1L = RG61L;
  DM2_COUNT_BY_COIN_TYPES(RG1L, RG4P16);
  RG4L = 0;
  vo_38.poke16(RG4W);
  //m_3559C:
  for (;;)
  {
    RG4L = signedlong(vo_38.peek16());
    if (RG4L >= lcon(0xa))
      return;
    //m_35449:
    RG1L = DM2_GET_ITEM_ORDER_IN_CONTAINER(unsignedlong(vw_00), RG4L);
    if (RG1W >= 0)
    {
      RG1L = 2 * signedlong(RG1W);
      RG51w = vwa_04[RG1L / 2];
      if (RG51w > 0)
      {
        RG1W = ddat.v1e0394[RG1L / 2];
        RG2L = unsignedlong(RG1W);
        RG1L = DM2_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG2L) & lcon(0xff);
        vl_2c = RG1L;
        RG1Blo = DM2_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(RG2L);
        RG3L = unsignedlong(RG1Blo);
        RG2L = lcon(0x18);
        RG4L = vl_2c;
        t_bmp* bmp = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(RG3Blo, RG4Blo, RG2Blo));
        palette = DM2_QUERY_GDAT_IMAGE_LOCALPAL(RG3Blo, CUTLX8(vl_2c), lcon(0x18));
        RG2L = 0;
        vw_30 = RG2W;
        vw_34 = RG2W;
        RG1W = vo_38.peek16() + lcon(0xdd);
        RG2L = signedlong(RG1W);
        RG3P16 = &vw_34;
        RG4R = &rc_18;
        DM2_QUERY_BLIT_RECT(bmp, RG4R, RG2W, IO vw_34, IO vw_30);
        rc_20.w = rc_18.w;
        rc_20.h = rc_18.h;
        RG63W = vo_38.peek16() << 3;
        RG1W = RG51w - wcon(0x20);
        RG4L = signedlong(RG1W);
        RG63L += signedlong(DM2_MAX(0, RG4W));
        RG4L = signedlong(RG51w);
        RG52L = signedlong(DM2_MIN(32, RG4W));
        //m_35524:
        for (;;)
        {
          RG63L++;
          RG63L &= lcon(0x1f);
          RG1L = signedlong(RG63W);
          RG2W = signedword(table1d275a[RG1L][0]);
          RG4W = rc_18.x + RG2W; // adj
          rc_20.x = RG4W;
          RG1W = signedword(table1d275a[RG1L][1]);
          RG4W = rc_18.y + RG1W; // adj
          rc_20.y = RG4W;
          RG4R = &dm2_buttongroup2.button.r;
          RG1R = &rc_20;
          RG1R->unify(RG4R, OO vw_34, OO vw_30);
          RG1L = signedlong(vw_30);
          put16(parw04, RG1W);
          RG3L = signedlong(vw_34);
          RG2R = &rc_20;
          DM2_DRAW_ICON_PICT_BUFF(bmp, &dm2_buttongroup2, /*TAG blitrect*/ RG2R, /*TAG srcx*/ RG3W, /*TAG srcy*/ parw04, 0xc, BLITMODE0, palette);
          RG52L--;
          if (RG52W <= 0)
            break;
        }
      }
    }
    //m_35597:
    vo_38.inc16();
  }
  FOREND
}

void DM2_guidraw_0b36_0c52(c_buttongroup* buttongroup, i32 edxl, i32 ebxl)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG6l;

  RG6l = RG2L;
  if (RG4W != lcon(0xffffffff))
  {
    RG2R = &buttongroup->button.r;
    RG1L = signedlong(RG4W);
    RG4R = RG2R;
    DM2_QUERY_EXPANDED_RECT(RG1W, RG4R);
  }
  i16 h = buttongroup->button.r.h;
  i16 w = buttongroup->button.r.w;
  buttongroup->button.dbidx = dm2_dballochandler.DM2_dballoc_3e74_5888();
  DM2_ALLOC_NEW_BMP(buttongroup->button.dbidx, w, h, BPP_8);
  buttongroup->button.groupsize = 0;
  if (RG6l != 0)
    DM2_ADJUST_BUTTONGROUP_RECTS(buttongroup, &buttongroup->button.r);
}

void DM2_DRAW_PLAYER_3STAT_PANE(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  i8 vb_00;
  i16 parw00;

  RG3L = RG1L;
  RG5l = RG4L;
  if (dm2_buttongroup1.button.dbidx != NODATA)
    return;
  RG2L = unsignedlong(RG3W);
  if (party.hero[RG2L].curHP != 0)
  {
    //m_3822D:
    RG1L = signedlong(ddat.v1e0976);
    RG2L++;
    if (RG1L != RG2L)
    {
      //m_38240:
      RG1W = unsignedword(RG1Blo);
      vb_00 = RG1Bhi;
    }
    else
      vb_00 = lcon(0x9);
  }
  else
    vb_00 = 1;
  //m_38245:
  RG3L += lcon(0xa1);
  RG3L = signedlong(RG3W);
  DM2_guidraw_0b36_0c52(&dm2_buttongroup1, RG3L, RG5l);
  put16(parw00, RG3W);
  DM2_DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 2, vb_00, &dm2_buttongroup1, parw00);
  if (RG5l == 0)
    dm2_buttongroup1.button.groupsize = 0;
}

void DM2_guidraw_29ee_00a3(i32 eaxl)
{
  if (dm2_buttongroup2.button.dbidx == NODATA)
  {
    DM2_guidraw_0b36_0c52(&dm2_buttongroup2, 11, eaxl);
    if (eaxl != 0)
      DM2_FILL_RECT_SUMMARY(&dm2_buttongroup2, &dm2_buttongroup2.button.r, ui8_to_pixel(palettecolor_to_ui8(paldat.palette[E_COL00])));
  }
}

void DM2_DRAW_CMD_SLOT(i16 eaxw, i8 edxb)
{
  c_nreg _RG4;
  t_text* par_tp04;

  DM2_guidraw_29ee_00a3(0);
  if (ddat.v1e0b62 != 0)
  {
    DM2_DRAW_ICON_PICT_ENTRY(20, ddat.v1e0ba4, 2 * (ddat.v1e0b40[eaxw].b_02 - bcon(0x8)) + bcon(0x41) + edxb, &dm2_buttongroup2, eaxw + wcon(0x6e));
    return;
  }
  DM2_DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 4, edxb + 21, &dm2_buttongroup2, eaxw + wcon(0x3f)); // engage command button
  RG4Blo = ddat.v1e0b40[eaxw].b_01;
  par_tp04 = DM2_QUERY_CMDSTR_NAME(ddat.v1e0b40[eaxw].b_00, RG4Blo, ddat.v1e0b40[eaxw].b_02);
  RG4W = unsignedword(RG4Blo);
  RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL00]);
  RG4Bhi |= lcon(0x40);
  DM2_DRAW_NAME_STR(&dm2_buttongroup2, eaxw + wcon(0x42), unsignedword(palettecolor_to_ui8(paldat.palette[E_COL15])), RG4W, par_tp04);
}

// was SKW_29ee_093e
void DM2_DRAW_SPELL_TO_BE_CAST(i32 eaxl)
{
  t_text tarr_04[0x2];

  DM2_guidraw_29ee_00a3(0);
  if (eaxl != 0)
    DM2_DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 5, 9, &dm2_buttongroup2, lcon(0xfc));	// cast spell button
  c_hero* hero = &party.hero[party.curacthero - 1];
  tarr_04[1] = '\0';
  i16 vw_00 = CUTX16(DM2_STRLEN(hero->rune));
  //m_33729:
  for (i16 rg5 = 0, rg6 = 0x105; rg5 < vw_00; rg5++, rg6++)
  {
    //m_336F2:
    tarr_04[0] = hero->rune[rg5];
    DM2_DRAW_BUTTON_STR(&dm2_buttongroup2, rg6, unsignedword(palettecolor_to_ui8(paldat.palette[E_COL00])), unsignedword(palettecolor_to_ui8(paldat.palette[13])) | 0x4000, tarr_04);
  }
}

void DM2_DRAW_PLAYER_ATTACK_DIR(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG51l;
  c_image image;
  t_palette palette[PAL16];
  c_rect rect;
  i16 vw_154;
  i16 vw_158;
  i16 parw04;

  DM2_guidraw_29ee_00a3(0);
  DM2_DRAW_ICON_PICT_ENTRY(8, ddat.v1d67d6, -10, &dm2_buttongroup2, lcon(0x5d));
  RG4L = signedlong(ddat.v1d272e);
  t_bmp* bmp = DM2_ALLOC_PICT_BUFF(ddat.v1d272c, RG4W, BIGPOOL_LO, lcon(0x4));
  RG4L = 0;
  vw_154 = RG4W;
  vw_158 = RG4W;
  RG4P16 = &vw_154;
  RG3P16 = &vw_158;
  RG2L = lcon(0x5e);
  RG4R = &rect;
  RG1R = DM2_QUERY_BLIT_RECT(bmp, RG4R, RG2W, IO vw_158, IO vw_154);
  if (RG1R != NULL)
  {
    if (ddat.savegames1.b_04 != 0)
    {
      RG1L = signedlong(DM2_RANDDIR());
      if (RG1W != 0)
      {
        RG1L -= 2;
        rect.y += RG1W;
        RG1L = signedlong(DM2_RANDDIR());
        if (RG1W != 0)
        {
          RG1L -= 2;
          rect.x += RG1W;
        }
      }
    }
    RG1W = party.curacthero;
    RG1L--;
    RG51l = signedlong(RG1W);
    DM2_QUERY_GDAT_SQUAD_ICON(PIXELCAST(bmp), RG51l, palette);
    RG1L = signedlong(vw_154);
    put16(parw04, RG1W);
    RG3L = signedlong(vw_158);
    RG2R = &rect;
    DM2_DRAW_ICON_PICT_BUFF(bmp, &dm2_buttongroup2, /*TAG blitrect*/ RG2R, /*TAG srcx*/ RG3W, /*TAG srcy*/ parw04, 0xc, BLITMODE0, palette);
    if (party.hero[RG51l].ench_power != 0)
    {
      DM2_QUERY_PICST_IT(DM2_QUERY_GDAT_SUMMARY_IMAGE(&image, 1, 7, party.hero[RG51l].ench_aura));
      image.imgdesc.mode |= 0x10;
      RG1L = (unsignedlong(party.hero[RG51l].absdir) + 4 - signedlong(ddat.v1e0258)) & 0x3;
      image.imgdesc.width >>= 2;
      RG1L *= signedlong(image.imgdesc.width);
      image.imgdesc.x = RG1W;
      image.imgdesc.height >>= 2;
      image.imgdesc.y = (CUTX16(timdat.gametick) & lcon(0x3)) * image.imgdesc.height;
      DM2_image_0b36_11c0(&image, &dm2_buttongroup2, 94, 0xc);
    }
  }
  //m_33638:
  DM2_FREE_PICT_BUFF(bmp);
  DM2_DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 4, 16, &dm2_buttongroup2, lcon(0x60));
  DM2_DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 4, 18, &dm2_buttongroup2, lcon(0x61));
}

// was SKW_29ee_09d5
void DM2_DRAW_SPELL_PANEL(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  t_text tarr_00[0x4]; // TODO dim maybe 0x2 only
  c_o32 vo_04; // poke 16 peek 8 16 inc16
  i8 vb_08; // inc
  t_text* par_tp02;

  RG4L = signedlong(party.curacthero) - 1;
  RG1Blo = party.hero[party.curacthero - 1].nrunes;
  RG1W = unsignedword(RG1Blo);
  vo_04.poke16(RG1W);
  RG1Blo = vo_04.peek8() + 1;
  RG2L = unsignedlong(RG1Blo);
  DM2_DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 5, RG2Blo, &dm2_buttongroup2, lcon(0x5c));
  RG1L = unsignedlong(vo_04.peek16());
  if (RG1L < lcon(0x4))
  {
    RG1W = unsignedword(RG1Blo);
    tarr_00[0x1] = RG1Bhi;
    RG1Blo = vo_04.peek8();
    RG1Bhi = 6;
    RG1W = unsignedword(RG1Blo) * unsignedword(RG1Bhi);
    RG1Blo += lcon(0x60);
    vb_08 = RG1Blo;
    RG5w = wcon(0xff);
    RG4L = 0;
    vo_04.poke16(RG4W);
    //m_337EA:
    for (;;)
    {
      RG1L = unsignedlong(vo_04.peek16());
      if (RG1L >= lcon(0x6))
        break;
      //m_337B3:
      RG1Blo = vb_08;
      tarr_00[0x0] = RG1Blo;
      par_tp02 = tarr_00;
      RG4W = unsignedword(RG4Blo);
      RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL13]);
      RG4Bhi |= lcon(0x40);
      RG3L = unsignedlong(RG4W);
      RG2L = unsignedlong(palettecolor_to_ui8(paldat.palette[E_COL00]));
      RG4L = signedlong(RG5w);
      vb_08++;
      RG5w++;
      DM2_DRAW_BUTTON_STR(&dm2_buttongroup2, RG4W, RG2W, RG3W, par_tp02);
      vo_04.inc16();
    }
  }
  //m_337F6:
  DM2_DRAW_SPELL_TO_BE_CAST(0);
  DM2_DRAW_PLAYER_ATTACK_DIR();
}

void DM2_SHOW_ATTACK_RESULT(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG51;
  c_wlreg _RG52;
  c_image image;
  t_text tarr_13c[0x8];
  i8 vb_144; // inc8

  RG51L = RG1L;
  RG1L = 1;
  DM2_guidraw_29ee_00a3(RG1L);
  vb_144 = 4;
  RG1Blo = 1;
  if (RG51W < 0)
  {
    bool skip00734 = false;
    if (RG51UW < mkuw(lcon(0xfffffffe)))
    {
      //m_32CB6:
      if (RG51W != wcon(0xfffa))
        skip00734 = true;
      else
        RG1Blo = lcon(0x19);
    }
    else
    {
      if (RG51UW <= mkuw(lcon(0xfffffffe)))
        //m_32CC4:
        RG1Blo = lcon(0x17);
      else
      {
        if (RG51W == wcon(0xffff))
          //m_32CC0:
          RG1Blo = lcon(0x18);
        else
          skip00734 = true;
      }
    }

    if (skip00734)
    {
      //m_32CC8:
      vb_144 = 5;
      if (RG51W != wcon(0xfffd))
      {
        //m_32CD6:
        if (RG51W != wcon(0xfffc))
          //m_32CE0:
          RG1Blo = lcon(0xd);
        else
          RG1Blo = lcon(0xc);
      }
      else
        RG1Blo = lcon(0xe);
    }
  }
  //m_32CE2:
  RG3L = unsignedlong(RG1Blo);
  DM2_QUERY_GDAT_SUMMARY_IMAGE(&image, 1, vb_144, RG3Blo);
  if (RG51W >= 0)
  {
    RG1L = signedlong(RG51W);
    if (RG1L <= lcon(0x28))
    {
      if (RG1L <= lcon(0xf))
        //m_32D18:
        image.w_34 = lcon(0x1f);
      else
        image.w_34 = lcon(0x2f);
      //m_32D21:
      image.w_36 = lcon(0x35);
    }
  }
  //m_32D2A:
  DM2_image_0b36_11c0(DM2_QUERY_PICST_IT(&image), &dm2_buttongroup2, 57, 0xffff);
  if (RG51W >= 0)
  {
    RG2L = 5;
    tarr_13c[0x5] = 0;
    //m_32D5D:
    for (;;)
    {
      RG4L = signedlong(RG51W);
      RG3L = lcon(0xa);
      RG1L = RG4L;
      RG4L = RG1L % RG3L;
      RG1L /= RG3L;
      RG4L += lcon(0x30);
      RG2L--;
      RG3L = unsignedlong(RG2W);
      tarr_13c[RG3L] = RG4Blo;
      RG1L = RG51L;
      RG4W = RG1W % wcon(0xa);
      RG1W /= wcon(0xa);
      RG51L = RG1L;
      if (RG1W == 0)
      {
        DM2_DRAW_BUTTON_STR(&dm2_buttongroup2, lcon(0x39), unsignedword(palettecolor_to_ui8(paldat.palette[E_COL04])), unsignedword(palettecolor_to_ui8(paldat.palette[E_COL00])), tarr_13c + RG3L);
        return;
      }
    }
    FOREND
  }
  //m_32DB0:
  if (RG51W != lcon(0xfffffffe))
    return;
  RG3P = DOWNCAST(c_buttongroup, &dm2_buttongroup2);
  if (ddat.v1e0b72 != 0)
    //m_32DD1:
    RG1Blo = lcon(0x34);
  else
    RG1Blo = lcon(0x38);
  //m_32DD3:
  RG2L = unsignedlong(RG1Blo);
  DM2_DRAW_ICON_PICT_ENTRY(7, 0, RG2Blo, UPCAST(c_buttongroup, RG3P), lcon(0x7e));
  RG4W = unsignedword(RG4Blo);
  vb_144 = RG4Bhi;
  //m_32E46:
  for (;;)
  {
    RG3Bhi = vb_144;
    if (mkub(RG3Bhi) >= mkub(lcon(0xffffffff)))
      return;
    //m_32DEA:
    RG52L = unsignedlong(RG3Bhi);
    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0x10), CUTLX8(RG52L), lcon(0xb), lcon(0x5));
    if (RG1W != 0)
    {
      RG4L = RG1L;
      RG4Blo ^= RG1Blo;
      RG4Bhi &= lcon(0xffffff80);
      RG4L = unsignedlong(RG4W);
      if (RG4L == 0)
      {
        RG3L = unsignedlong(RG1W);
        RG4W = ddat.v1e0b78;
        if ((RG4L & RG3L) != 0)
        {
          DM2_DRAW_ICON_PICT_ENTRY(16, CUTLX8(RG52L), 24, &dm2_buttongroup2, lcon(0x7d));
          return;
        }
      }
    }
    vb_144++;
  }
  FOREND
}

void DM2_guidraw_0b36_105b(c_buttongroup* buttongroup, c_rect* edxrp, i16 ebxw) // serious! entry to deactivated - rectangle - blit HOTHOT
{
  c_nreg _RG4;
  c_rect rc_00;

  if (edxrp != NULL)
  {
    RG4R = DM2_OFFSET_RECT(buttongroup, &rc_00, edxrp);
    blitter.stretch_4to8(reinterpret_cast<c_pixel256*>(PIXELCAST(dm2_dballochandler.DM2_GET_BMP(buttongroup->button.dbidx))), RG4R, CUTX8(ebxw), buttongroup->button.r.w);
    DM2_ADJUST_BUTTONGROUP_RECTS(buttongroup, edxrp);
  }
}

// was SKW_29ee_058d
void DM2_DRAW_SQUAD_SPELL_AND_LEADER_ICON(i16 eaxw, i32 edxl)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  i16 RG6w;
  i32 RG7l;
  c_image image;
  c_rect rc_13c;
  i32 vl_144;
  i8 vb_148;
  bool skip00737 = false;

  RG6w = RG1W;
  RG7l = RG4L;
  DM2_guidraw_29ee_00a3(0);
  RG3L = signedlong(RG6w);
  RG1L = 263 * RG3L;
  RG3L = RG1L;
  RG1W = unsignedword(RG1Blo);
  RG1Blo = party.hero[RG3L / 263].partypos;
  RG5L = RG1L + lcon(0x4);
  RG5W -= ddat.v1e0258;
  RG5L &= 0x3;
  RG2L = unsignedlong(palettecolor_to_ui8(paldat.palette[E_COL00]));
  RG1L = signedlong(CUTX16(RG5L + lcon(0x4f)));
  RG4R = &rc_13c;
  RG1R = DM2_QUERY_EXPANDED_RECT(RG1W, RG4R);
  RG4R = RG1R;
  DM2_FILL_RECT_SUMMARY(&dm2_buttongroup2, /*TAG BLITRECT*/ RG4R, ui8_to_pixel(RG2UBlo));
  if (party.hero[RG3L / 263].curHP == 0)
    return;
  RG1L = unsignedlong(RG5W);
  if (RG1L != 1 && RG1L != 2)
    //m_3329F:
    vl_144 = 0;
  else
    vl_144 = 1;
  //m_332A8:
  RG1L = unsignedlong(RG5W);
  if (RG1L > 1)
  {
    //m_332BE:
    vb_148 = lcon(0xc);
    RG1Blo = lcon(0x8);
  }
  else
  {
    vb_148 = lcon(0xa);
    RG1Blo = 6;
  }
  //m_332C8:
  if ((e_hero)RG6w == eventqueue.event_heroidx)
    vb_148++;
  if (RG7l != 0)
    RG1Blo++;
  RG3L = unsignedlong(RG1Blo);
  DM2_QUERY_GDAT_SUMMARY_IMAGE(&image, 1, 4, RG3Blo);
  RG1L = vl_144;
  image.blitmode = RG1W;
  RG1L = RG5L + lcon(0x57);
  RG2L = signedlong(RG1W);
  DM2_image_0b36_11c0(DM2_QUERY_PICST_IT(&image), &dm2_buttongroup2, RG2W, 0x4);
  if (ddat.v1e0238 == 0)
  {
    RG3L = signedlong(RG6w);
    RG1L = 263 * RG3L;
    if (party.hero[RG3L].handcooldown[2] == 0)
      skip00737 = true;
  }

  if (!skip00737)
  {
    //m_33340:
    image.rect.x += dm2_buttongroup2.button.r.x;
    image.rect.y += dm2_buttongroup2.button.r.y;
    DM2_guidraw_0b36_105b(&dm2_buttongroup2, &image.rect, 0);
  }

  //m_33366:
  DM2_QUERY_GDAT_SUMMARY_IMAGE(&image, 1, 4, vb_148);
  RG1L = vl_144;
  image.blitmode = RG1W;
  RG5L += lcon(0x53);
  RG2L = signedlong(RG5W);
  DM2_image_0b36_11c0(DM2_QUERY_PICST_IT(&image), &dm2_buttongroup2, RG2W, 0x4);
  if (ddat.v1e0238 != 0)
  {
    image.rect.x += dm2_buttongroup2.button.r.x;
    image.rect.y += dm2_buttongroup2.button.r.y;
    DM2_guidraw_0b36_105b(&dm2_buttongroup2, &image.rect, 0);
  }
}

void DM2_guidraw_24a5_0e82(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG51w;
  i16 RG6w;
  i32 RG7l;
  c_rect rc_00;
  c_rect rc_08;
  c_rect rc_10;

  RG7l = argl1;

  RG51w = RG4W;
  RG4L = signedlong(RG1W);
  if (RG4L >= lcon(0xfffffe00))
  {
    //m_299CE:
    if (RG1W >= 0)
      //m_299DA:
      RG6w = RG2W;
    else
      RG6w = wcon(0xb);
  }
  else
    RG6w = wcon(0x8);
  //m_299DC:
  RG1L -= RG3L;
  RG2L = signedlong(argw0);
  RG4L = signedlong(RG3W);
  RG2L -= RG4L;
  RG4L = signedlong(RG1W) * lcon(0x2710);
  RG1L = RG4L / RG2L;
  RG2L = unsignedlong(RG1W);
  RG3L = lcon(0x2710);
  RG4R = &rc_08;
  RG1R = DM2_SCALE_RECT(RG51w, RG2W, RG3W, RG4R);
  if (RG1R == NULL)
    return;
  if (RG7l == 0)
    //m_29A86:
    rc_10 = rc_08; // copy
  else
  {
    RG4R = &rc_00;
    DM2_QUERY_EXPANDED_RECT(RG51w, RG4R);
    rc_10 = rc_00; // copy
    RG2L = signedlong(rc_08.x) + signedlong(rc_08.w);
    RG1L = signedlong(rc_00.x) + signedlong(rc_00.w);
    RG4L = RG2L - 1;
    RG1L--;
    if (RG4L != RG1L)
    {
      //m_29A64:
      RG2W = rc_00.x + rc_00.w - 1; // adj
      RG1W = rc_08.x + rc_08.w; // adj
      RG4W = RG1W - 1; // adj
      RG2W -= RG4W; // adj
      rc_00.w = RG2W;
      rc_00.x = RG1W;
    }
    else
      RG7l = 0;
  }
  //m_29A98:
  RG1W = ddat.v1d2748;
  rc_10.x += RG1W;
  rc_10.y += RG1W;
  RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL00]);
  RG4L = unsignedlong(RG4Blo);
  RG1R = &rc_10;
  DM2_FILL_BACKBUFF_RECT(/*TAG blitrect*/ RG1R, ui8_to_pixel(RG4UBlo));
  RG4L = unsignedlong(RG6w);
  RG4UBlo = palettecolor_to_ui8(paldat.palette[RG4L]);
  RG4L = unsignedlong(RG4Blo);
  RG1R = &rc_08;
  DM2_FILL_BACKBUFF_RECT(/*TAG blitrect*/ RG1R, ui8_to_pixel(RG4UBlo));
  if (RG7l != 0)
  {
    RG4UBlo = palettecolor_to_ui8(paldat.palette[RG7l]);
    RG4L = unsignedlong(RG4Blo);
    RG1R = &rc_00;
    DM2_FILL_BACKBUFF_RECT(/*TAG blitrect*/ RG1R, ui8_to_pixel(RG4UBlo));
  }
}

// was SKW_24a5_0fb8
void DM2_DRAW_FOOD_WATER_POISON_PANEL(void)
{
  c_hero* hero = &party.hero[ddat.v1e0976 - 1];
  ddat.v1d66fc = 1;
  DM2_DRAW_STATIC_PIC(7, 0, 1, wcon(0x1ee), NOALPHA);
  DM2_guidraw_24a5_0e82(signedlong(hero->food), lcon(0x1f0), 5, lcon(0xfffffc00), lcon(0x800), 0);
  DM2_guidraw_24a5_0e82(signedlong(hero->water), lcon(0x1f1), 14, lcon(0xfffffc00), lcon(0x800), 0);
  DM2_DRAW_STATIC_PIC(7, 0, 6, wcon(0x1f4), 0xc);
  DM2_DRAW_STATIC_PIC(7, 0, 7, wcon(0x1f5), 0xc);
  if (hero->poisoned != 0)
  {
    DM2_guidraw_24a5_0e82(signedlong(hero->poison), lcon(0x1f3), 8, 0, lcon(0xc00), 0);
    DM2_DRAW_STATIC_PIC(7, 0, 8, wcon(0x1f6), 0xc);
  }
}

// was SKW_24a5_0b0f
void DM2_DRAW_ITEM_STATS_BAR(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  i16 RG6w;
  c_rect rc_00;
  t_text tarr_08[0x14]; // TODO maybe the string is shorter, internal used largest idx 0x4
  i16 parw00;
  i16 parw02;
  t_text* par_tp04;
  i16 parw05;
  t_text* par_tp07;
  i16 parw08;
  t_text* par_tp10;

  RG6w = RG4W;
  tarr_08[0x4] = RG3Blo;
  RG5L = signedlong(RG1W);
  RG4R = &rc_00;
  RG1R = DM2_QUERY_EXPANDED_RECT(RG5W, RG4R);
  if (RG1R != NULL)
  {
    RG1L = signedlong(RG2W);
    put16(parw00, RG1W);
    RG2L = unsignedlong(argw0);
    DM2_guidraw_24a5_0e82(signedlong(RG6w), RG5L, RG2L, 0, parw00, 1);
    tarr_08[0x1] = 0;
    tarr_08[0x0] = tarr_08[0x4];
    par_tp04 = tarr_08;
    rc_00.h -= 2;
    RG4L = unsignedlong(palettecolor_to_ui8(paldat.palette[E_COL00]));
    RG4Bhi |= lcon(0x40);
    RG4L = signedlong(RG4W);
    i32 parl03 = RG4L;
    RG1UBlo = palettecolor_to_ui8(paldat.palette[argw0]);
    RG1L = unsignedlong(RG1Blo);
    put16(parw02, RG1W);
    RG1W = rc_00.y + rc_00.h - 1; // adj
    RG3L = signedlong(RG1W);
    RG1W = rc_00.x - 9;
    RG2L = signedlong(RG1W);
    DM2_DRAW_STRONG_TEXT(PIXELCAST(gfxsys.bitmapptr), getbmpheader(gfxsys.bitmapptr)->width, RG2W, RG3W, parw02, CUTX16(parl03), par_tp04);
    tarr_08[0x0] = lcon(0x60);
    par_tp07 = tarr_08;
    RG4L = unsignedlong(palettecolor_to_ui8(paldat.palette[E_COL00]));
    RG4Bhi |= lcon(0x40);
    RG4L = signedlong(RG4W);
    i32 parl06 = RG4L;
    RG1UBlo = palettecolor_to_ui8(paldat.palette[E_COL15]);
    RG1L = unsignedlong(RG1Blo);
    put16(parw05, RG1W);
    RG1W = rc_00.y + rc_00.h - 1; // adj
    RG3L = signedlong(RG1W);
    RG1W = rc_00.x + 3; // adj
    RG2L = signedlong(RG1W);
    DM2_DRAW_STRONG_TEXT(PIXELCAST(gfxsys.bitmapptr), getbmpheader(gfxsys.bitmapptr)->width, RG2W, RG3W, parw05, CUTX16(parl06), par_tp07);
    tarr_08[0x0] = lcon(0x65);
    par_tp10 = tarr_08;
    RG1L = unsignedlong(palettecolor_to_ui8(paldat.palette[E_COL00]));
    RG1Bhi |= lcon(0x40);
    RG1L = signedlong(RG1W);
    i32 parl09 = RG1L;
    RG1L = unsignedlong(palettecolor_to_ui8(paldat.palette[E_COL15]));
    put16(parw08, RG1W);
    RG1W = rc_00.y + rc_00.h - 1; // adj
    RG3L = signedlong(RG1W);
    RG1W = rc_00.x + rc_00.w - 1 - strdat.gfxstrw1 - 1; // adj
    RG2L = signedlong(RG1W);
    DM2_DRAW_STRONG_TEXT(PIXELCAST(gfxsys.bitmapptr), getbmpheader(gfxsys.bitmapptr)->width, RG2W, RG3W, parw08, CUTX16(parl09), par_tp10);
  }
}

i8 DM2_guidraw_2405_014a(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  c_wlreg _RG61;
  c_lreg _RG62;
  i32 RG63l;
  i32 RG64l;
  c_lreg _RG65;
  c_lreg _RG71;
  i32 RG72l;
  i16 vw_00;
  i16 vw_04;
  i32 vl_08;
  bool skip00649 = false;
  bool skip00648 = false;

  RG61L = RG1L;
  vw_04 = RG4W;
  RG3Blo = lcon(0x18);
  if (RG2L == 0)
  {
    RG1Blo = RG3Blo;
    return RG1Blo;
  }
  RG71L = unsignedlong(RG1W);
  RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG71L, lcon(0x6));
  vl_08 = RG1L;
  RG5w = RG1W;
  RG5w &= wcon(0x1f);
  if (RG5w == 0)
  {
    RG1Blo = RG3Blo;
    return RG1Blo;
  }
  RG4L = 0;
  RG1W &= lcon(0x8000);
  RG1L = unsignedlong(RG1W);
  if (RG1L != 0)
  {
    RG4L = signedlong(vw_04);
    RG2L = 1;
    RG1L = DM2_IS_ITEM_FIT_FOR_EQUIP(CUTX16(RG71L), RG4L, RG2L);
    RG4L = RG1L;
    if (RG1L == 0)
    {
      RG1Blo = RG3Blo;
      return RG1Blo;
    }
  }
  //m_28089:
  RG1L = vl_08;
  RG1W &= lcon(0x4000);
  RG1L = unsignedlong(RG1W);
  if (RG1L != 0)
  {
    if (party.curacthero == 0)
    {
      RG1Blo = RG3Blo;
      return RG1Blo;
    }
    RG4L = signedlong(party.curacthero);
    RG1L = 2 * signedlong(party.curactmode);
    if (RG61W != party.handitems.warr2d_00[RG4L][party.curactmode])
    {
      RG1Blo = RG3Blo;
      return RG1Blo;
    }
    RG4L = 1;
  }
  //m_280CD:
  if (RG4L != 0)
  {
    RG3Blo++;
    RG5w--;
  }
  if (RG5w == wcon(0x0))
  {
    RG1Blo = RG3Blo;
    return RG1Blo;
  }
  RG2L = timdat.gametick;
  RG1L = vl_08;
  RG1W &= lcon(0x1f00);
  RG1L = unsignedlong(RG1W) >> bcon(0x8);
  if (mkul(RG1L) > mkul(lcon(0x6)))
  {
    RG1Blo = RG3Blo;
    return RG1Blo;
  }
  RG4L = RG61L;
  RG4Bhi &= 0x3;
  switch (RG1L)
  {
    case 0:
      skip00648 = true;
      break;

    case 1:
      //m_2812D:
      RG1W = DM2_RAND16(RG5w);
      RG3Blo += RG1Blo;
      RG1Blo = RG3Blo;
      return RG1Blo;

    case 2:
      //m_2813B:
      RG3Blo += CUTX8(ddat.v1e0258);
      RG1Blo = RG3Blo;
      return RG1Blo;

    case 3:
      //m_28143:
      RG2L = unsignedlong(RG61W);
      RG4L = 0;
      RG1W = DM2_ADD_ITEM_CHARGE(RG2L, RG4L);
      if (RG1W == 0)
      {
        RG1Blo = RG3Blo;
        return RG1Blo;
      }
      RG4L = unsignedlong(RG1W) * unsignedlong(RG5w);
      RG1L = DM2_GET_MAX_CHARGE(RG2L) & lcon(0xffff);
      RG2L = RG1L + 1;
      RG1L = RG4L;
      RG4L = RG1L % RG2L;
      RG1L /= RG2L;
      RG3L = unsignedlong(RG3Blo);
      RG1L++;
      break;

    case 4:
      skip00649 = true;
      break;

    case 5:
      //m_2810A:
      RG1L = unsignedlong(RG4W);
      RG2L += RG1L;
      skip00648 = true;
      break;

    case 6:
      //m_28184:
      RG1L = unsignedlong(RG4W);
      RG2L += RG1L;
      skip00649 = true;
      break;

    default: throw(THROW_DMABORT);
  }

  if (skip00649)
  {
    //m_2818B:
    RG1L = vl_08;
    RG1W &= lcon(0xe0);
    RG1L = unsignedlong(RG1W) >> 5;
    vw_04 = RG1W;
    RG62L = unsignedlong(RG61W);
    RG4L = 0;
    RG1W = DM2_ADD_ITEM_CHARGE(RG62L, RG4L);
    vw_00 = RG1W;
    if (RG1W == 0)
    {
      RG1Blo = RG3Blo;
      return RG1Blo;
    }
    RG4L = unsignedlong(RG5w);
    RG72l = signedlong(vw_04);
    RG1L = RG4L / RG72l;
    RG4L = unsignedlong(vw_00) * RG1L;
    RG1L = DM2_GET_MAX_CHARGE(RG62L) & lcon(0xffff);
    RG63l = RG1L + 1;
    RG1L = RG4L;
    RG4L = RG1L % RG63l;
    RG1L /= RG63l;
    RG64l = RG1L * RG72l;
    RG1L = RG2L;
    RG4L = RG1L % RG72l;
    RG1L = unsignedlong(RG4W);
    RG64l++;
    RG1L += RG64l;
    RG3L = unsignedlong(RG3Blo);
  }

  if (skip00648)
  {
    //m_28111:
    RG65L = unsignedlong(RG5w);
    RG1L = RG2L;
    RG4L = RG1L % RG65L;
    RG1L /= RG65L;
    RG3W = unsignedword(RG3Blo);
    RG3L += RG4L;
    RG1Blo = RG3Blo;
    return RG1Blo;
  }

  //m_28180:
  RG3L += RG1L;
  RG1Blo = RG3Blo;
  return RG1Blo;
}

// was SKW_2405_04f4
void DM2_DRAW_ITEM_IN_HAND(c_itemrecord* xeaxp)
{
  c_nreg _RG1;

  i8 vb_0c = DM2_QUERY_CLS1_FROM_RECORD(unsignedlong(xeaxp->w_00));
  i8 vb_08 = DM2_QUERY_CLS2_FROM_RECORD(unsignedlong(xeaxp->w_00));
  RG1Blo = DM2_guidraw_2405_014a(unsignedlong(xeaxp->w_00), lcon(0xffffffff), 1);
  t_bmp* srcbmp = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(vb_0c, vb_08, RG1Blo));
  DM2_COPY_SMALL_PALETTE(xeaxp->palette, DM2_QUERY_GDAT_IMAGE_LOCALPAL(vb_0c, vb_08, RG1Blo));
  blitter.blit(
    PIXELCAST(srcbmp),
    PIXELCAST(xeaxp->bmp),
    tmprects.alloc_origin_tmprect(ddat.v1d271a, ddat.v1d271c),
    0,
    0,
    getbmpheader(srcbmp)->width,
    (PIXELCAST(xeaxp->bmp) == gfxsys.dm2screen) ? ORIG_SWIDTH : ddat.v1d271a,
    NOALPHA,
    BLITMODE0,
    BPP_4,
    BPP_4);
}

// was SKW_29ee_21d8
void DM2_DRAW_CONTAINER_PANEL(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_lreg _RG61;
  c_lreg _RG63;
  c_wlreg _RG7;
  c_rect rc_00;
  c_rect rc_08;
  c_ql32 vql_10;
  i16 vw_14;
  i8 vb_18;
  i16 parw04;
  i16 parw06;
  i16 parw08;

  RG7L = RG4L;
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(unsignedlong(RG1W));
  vb_18 = RG1Blo;
  if (RG4L != 0)
  {
    RG61L = unsignedlong(RG1Blo);
    DM2_DRAW_ICON_PICT_ENTRY(20, CUTLX8(RG61L), 16, &dm2_buttongroup2, lcon(0x5c));
    RG3P = DOWNCAST(c_buttongroup, &dm2_buttongroup2);
    RG2L = lcon(0x12);
    RG4L = RG61L;
    DM2_DRAW_ICON_PICT_ENTRY(20, RG4Blo, RG2Blo, UPCAST(c_buttongroup, RG3P), lcon(0xe3), ALPHA_A);
  }
  if (RG7L == 0)
  {
    RG4R = &rc_08;
    DM2_QUERY_EXPANDED_RECT(92, RG4R);
  }
  //m_356F7:
  for (i16 RG5w = 0; RG5w < 8; RG5w++)
  {
    RG1L = signedlong(RG5w);
    //m_35623:
    RG1W = party.hand_container[RG1L];
    vql_10.poke32(RG1L);
    if (RG1W != lcon(0xffffffff))
    {
      RG1W = RG5w + wcon(0xe5);
      vw_14 = RG1W;
      if (RG7L == 0)
      {
        RG1L = signedlong(vw_14);
        RG4R = &rc_00;
        DM2_QUERY_EXPANDED_RECT(RG1W, RG4R);
        RG3L = unsignedlong(vb_18);
        RG2L = lcon(0x10);
        RG4L = RG3L;
        t_bmp* bmp = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(20, RG4Blo, RG2Blo));
        t_palette* pal = DM2_QUERY_GDAT_IMAGE_LOCALPAL(20, RG3Blo, lcon(0x10));
        put16(parw06, RG7W);
        RG1L = signedlong(mkw(rc_00.y - rc_08.y));
        put16(parw04, RG1W);
        RG1W = rc_00.x - rc_08.x; // adj
        RG3L = signedlong(RG1W);
        RG2R = &rc_00;
        RG4P = DOWNCAST(c_buttongroup, &dm2_buttongroup2);
        DM2_DRAW_ICON_PICT_BUFF(bmp, /*TAG buttongroup*/ UPCAST(c_buttongroup, RG4P), /*TAG blitrect*/ RG2R, /*TAG srcx*/ RG3W, /*TAG srcy*/ parw04, 0xffff, (e_blitmode)parw06, pal);
      }
      //m_356AA:
      RG1L = signedlong(vw_14);
      put16(parw08, RG1W);
      RG1W = RG5w + wcon(0x1e);
      RG4L = signedlong(RG1W);
      RG3L = unsignedlong(vql_10.peek16());
      RG2L = 1;
      RG1Blo = DM2_guidraw_2405_014a(RG3L, RG4L, RG2L);
      RG2L = unsignedlong(RG1Blo);
      RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG3L);
      RG4L = unsignedlong(RG1Blo);
      RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG3L) & lcon(0xff);
      RG3P = DOWNCAST(c_buttongroup, &dm2_buttongroup2);
      i8 lpalidx = RG1Blo;
      DM2_DRAW_ICON_PICT_ENTRY(lpalidx, RG4Blo, RG2Blo, UPCAST(c_buttongroup, RG3P), parw08, ALPHA_C);
    }
    //m_356F6:
  }
  FOREND
}

void DM2_guidraw_2405_011f(i32 eaxl, c_rect* edxrp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4R = edxrp;

  RG3R = RG4R;
  RG1L = signedlong(RG1W);
  DM2_guidraw_2405_00ec(RG1L, RG4R);
  RG3R->inflate(ddat.v1d2726, ddat.v1d2726);
}

void DM2_guidraw_2405_00ec(i32 eaxl, c_rect* edxrp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4R = edxrp;
  i16 vw_00;
  i16 vw_04;
  i16* parpw00;

  RG2L = RG1L;
  vw_04 = ddat.v1d271a; // adj
  vw_00 = ddat.v1d271c; // adj
  DM2_QUERY_BLIT_RECT(NULL, RG4R, RG2W, IO vw_04, IO vw_00);
}

// was SKW_2405_02e8
// stacksize was 0x2c
void DM2_DRAW_ITEM_ICON(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  i32 RG7l;
  c_rect rc_00;
  c_ql32 vql_08;
  i16 vw_0c;
  i8 vb_10;
  i8 lpalidx;
  i8 vb_18;
  i16 parw00;
  i16 parw11;

  RG5w = RG4W;
  vql_08.poke32(RG2L);
  RG7l = RG3L;
  if (RG1W != lcon(0xffffffff))
  {
    //m_2823F:
    RG3L = unsignedlong(RG1W);
    RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG3L);
    lpalidx = RG1Blo;
    RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG3L);
    vb_10 = RG1Blo;
    if (RG4W >= wcon(0x8))
    {
      //m_28277:
      RG1L = RG4L - lcon(0x8);
      RG4L = signedlong(RG1W);
      RG2L = 1;
      RG1Blo = DM2_guidraw_2405_014a(RG3L, RG4L, RG2L);
    }
    else
    {
      RG1L = RG4L;
      RG1Bhi ^= RG4Bhi;
      RG1Blo &= 0x1;
      RG4L = signedlong(RG1W);
      RG2L = 1;
      RG1Blo = DM2_guidraw_2405_014a(RG3L, RG4L, RG2L);
    }
  }
  else
  {
    lpalidx = 7;
    vb_10 = 0;
    RG1L = signedlong(RG5w);
    RG1Blo = ddat.v1d653c[RG1L].b_02;
    RG4Bhi = vql_08.peek8();
    RG1Blo += RG4Bhi;
  }
  //m_2828B:
  vb_18 = RG1Blo;
  RG1L = signedlong(RG5w);
  RG1W = ddat.v1d653c[RG1L].w_00;
  vw_0c = RG1W;
  if (RG5w < wcon(0x26))
  {
    RG1L = signedlong(vw_0c);
    RG4R = &rc_00;
    DM2_QUERY_EXPANDED_RECT(RG1W, RG4R);
    if (RG5w >= wcon(0x8))
    {
      //m_28313:
      RG2L = 0;
      RG4L = 0;
      DM2_DRAW_DIALOGUE_PICT(BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(7, 0, 0)), gfxsys.bitmapptr, &rc_00, rc_00.x, rc_00.y, lcon(0xffff), DM2_QUERY_GDAT_IMAGE_LOCALPAL(7, 0, 0));
    }
    else
    {
      RG2L = 0;
      RG4L = 2;
      t_bmp* bmp = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(1, 2, 0));
      t_palette* pal1 = DM2_QUERY_GDAT_IMAGE_LOCALPAL(1, 2, 0);
      put16(parw00, rc_00.y - dm2_buttongroup1.button.r.y);
      RG1W = rc_00.x - dm2_buttongroup1.button.r.x; // adj
      RG3L = signedlong(RG1W);
      RG2R = &rc_00;
      DM2_DRAW_ICON_PICT_BUFF(bmp, &dm2_buttongroup1, /*TAG blitrect*/ RG2R, /*TAG srcx*/ RG3W, /*TAG srcy*/ parw00, 0xffff, BLITMODE0, pal1);
    }
  }
  //m_28354:
  if (argl0 != 0 && RG5w < wcon(0xe))
  {
    RG1L = signedlong(vw_0c);
    RG4R = &rc_00;
    DM2_guidraw_2405_011f(RG1L, RG4R);
    if (RG7l == 0)
    {
      //m_2837F:
      if (vql_08.peek32() == 0)
        //m_2838A:
        RG1Blo = 4;
      else
        RG1Blo = 5;
    }
    else
      RG1Blo = 6;
    //m_2838C:
    RG3L = unsignedlong(RG1Blo);
    RG2L = RG3L;
    RG4L = 2;
    t_bmp* bmp = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(1, 2, RG2Blo));
    RG2L = RG3L;
    RG4L = 2;
    t_palette* pal = DM2_QUERY_GDAT_IMAGE_LOCALPAL(1, 2, RG2Blo);
    if (RG5w >= wcon(0x8))
    {
      //m_283D5:
      RG2L = lcon(0xc);
      RG4R = &rc_00;
      DM2_DRAW_DIALOGUE_PARTS_PICT(bmp, RG4R, RG2W, pal);
    }
    else
    {
      RG3L = 0;
      RG2R = &rc_00;
      DM2_DRAW_ICON_PICT_BUFF(bmp, &dm2_buttongroup1, /*TAG blitrect*/ RG2R, /*TAG srcx*/ RG3W, 0, 0xc, BLITMODE0, pal);
    }
  }
  //m_283E5:
  if (vb_18 != lcon(0xffffffff))
  {
    if (RG5w < wcon(0x8))
    {
      RG1L = signedlong(vw_0c);
      put16(parw11, RG1W);
      DM2_DRAW_ICON_PICT_ENTRY(lpalidx, vb_10, vb_18, &dm2_buttongroup1, parw11, ALPHA_C);
    }
    else
      DM2_DRAW_STATIC_PIC(lpalidx, vb_10, vb_18, vw_0c, 0xc);
  }
}

// was SKW_24a5_0ad2
void DM2_DRAW_CONTAINER_SURVEY(unk* xeaxp)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG5l;
  c_wlreg _RG6;

  RG6L = 0;
  RG1W = word_at(RG1P, 2);
  //m_2956E:
  for (;;)
  {
    if (RG1W == lcon(0xfffffffe))
      return;
    RG4L = signedlong(RG6W);
    if (RG4L >= lcon(0x8))
      return;
    RG4L = signedlong(CUTX16(RG6L + lcon(0x2f)));
    RG5l = unsignedlong(RG1W);
    DM2_DRAW_ITEM_ICON(RG5l, RG4L, 0, 0, 0);
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(CUTX16(RG5l)))));
    RG6L++;
  }
  FOREND
}

// was SKW_2e62_05d4
void DM2_DRAW_EYE_MOUTH_COLORED_RECTANGLE(i8 eaxb, i16 edxw)
{
  c_rect rc_00;

  DM2_guidraw_2405_011f(signedlong(edxw), &rc_00);
  t_palette* palette = DM2_QUERY_GDAT_IMAGE_LOCALPAL(1, 2, eaxb);
  DM2_DRAW_DIALOGUE_PARTS_PICT(BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(1, 2, eaxb)), &rc_00, 12, palette);
}

i32 DM2_guidraw_2e62_03b5(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  c_wlreg _RG6;
  i32 RG7l;
  c_ql32 vql_00;
  i32 vl_04;
  i32 vl_08;
  i32 vl_0c;
  c_ql32 vql_10;
  bool skip00786 = false;

  vql_00.poke32(RG1L);
  RG6L = RG4L;
  RG7l = RG2L;
  RG4L = 0;
  vl_0c = RG4L;
  vl_04 = RG4L;
  vl_08 = RG2L;
  RG1L = unsignedlong(vql_00.peek16());
  RG4L = signedlong(ddat.v1e0976);
  RG1L++;
  if (RG4L == RG1L)
    //m_385B7:
    RG5w = RG6W + wcon(0x8);
  else
  {
    RG4L = unsignedlong(RG6W);
    if (RG4L > 1)
      return 0;
    RG4L = unsignedlong(ddat.v1e0288);
    if (RG4L == RG1L)
      return 0;
    RG5w = 2 * vql_00.peek16() + RG6W;
  }
  //m_385BC:
  RG2L = unsignedlong(vql_00.peek16());
  RG1L = unsignedlong(RG6W);
  RG3L = 2 * RG1L;
  RG4W = party.hero[RG2L].item[RG1L];
  vql_10.poke32(RG4L);
  if (RG1L <= 1)
  {
    RG4L = signedlong(party.curacthero);
    RG2L++;
    if (RG2L == RG4L)
    {
      RG4L = signedlong(party.curactmode);
      if (RG1L == RG4L)
        vl_0c = 1;
    }
    RG1L = unsignedlong(RG5w);
    RG1Blo = ddat.v1d653c[RG1L].b_05 & 1;
    RG1L = unsignedlong(RG1Blo);
    RG1Blo = RG1L == 0 ? 1 : 0;
    RG4L = RG1L & lcon(0xff);
    RG1Blo = vl_0c == 0 ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != RG4L)
    {
      vl_08 = 1;
      RG1L = unsignedlong(RG5w);
      ddat.v1d653c[RG1L].b_05 ^= 1;
    }
  }
  RG1L = unsignedlong(RG6W);
  if (RG1L <= lcon(0x5))
  {
    RG3L = unsignedlong(vql_00.peek16());
    RG1W = party.hero[RG3L].bodyflag;
    RG1L = unsignedlong(RG1W);
    RG3L = RG6L;
    RG4L = 1 << RG3Blo;
    if ((RG4L & RG1L) != 0)
      vl_04 = 1;
    RG1L = unsignedlong(RG5w);
    RG1Blo = ddat.v1d653c[RG1L].b_05 & 2;
    RG1L = unsignedlong(RG1Blo);
    RG1Blo = RG1L == 0 ? 1 : 0;
    RG4L = RG1L & lcon(0xff);
    RG1Blo = vl_04 == 0 ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != RG4L)
    {
      vl_08 = 1;
      RG1L = unsignedlong(RG5w);
      ddat.v1d653c[RG1L].b_05 ^= 2;
    }
  }
  RG4L = vql_10.peek32();
  if (RG4W != lcon(0xffffffff))
  {
    RG3L = unsignedlong(RG4W);
    RG4L = 0;
    RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG3L, RG4L);
    RG1W &= lcon(0x8000);
    RG1L = unsignedlong(RG1W);
    if (RG1L == 0)
    {
      //m_38706:
      RG4L = signedlong(RG6W);
      RG2L = 0;
    }
    else
    {
      RG4L = signedlong(RG6W);
      RG2L = 1;
    }
    //m_3870B:
    RG1Blo = DM2_guidraw_2405_014a(RG3L, RG4L, RG2L);
    RG3Blo = RG1Blo;
    RG1L = 8 * unsignedlong(RG5w);
    if (RG3Blo != ddat.v1d653c[unsignedlong(RG5w)].b_03)
    {
      RG7l = 1;
      ddat.v1d653c[unsignedlong(RG5w)].b_03 = RG3Blo;
    }
    RG3L = 8 * unsignedlong(RG5w);
    RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(unsignedlong(vql_10.peek16()));
    if (RG1Blo != ddat.v1d653c[unsignedlong(RG5w)].b_04)
    {
      RG7l = 1;
      ddat.v1d653c[unsignedlong(RG5w)].b_04 = RG1Blo;
    }
  }
  //m_38756:
  if (RG7l == 0)
  {
    if (vl_08 == 0)
    {
      RG1L = unsignedlong(RG5w);
      RG1W = ddat.v1d653c[RG1L].w_06;
      if (RG1W == vql_10.peek16())
        return 0;
    }
  }
  if (vql_10.peek16() != lcon(0xffffffff))
  {
    RG1L = unsignedlong(RG5w);
    if (RG1L >= lcon(0x8))
      skip00786 = true;
  }

  if (!skip00786)
    //m_38787:
    vl_08 = 1;

  //m_3878F:
  RG1L = unsignedlong(RG5w);
  if (RG1L < lcon(0x8))
    DM2_DRAW_PLAYER_3STAT_PANE(unsignedlong(vql_00.peek16()), 0);
  RG1L = unsignedlong(RG5w);
  RG4L = vql_10.peek32();
  ddat.v1d653c[RG1L].w_06 = RG4W;
  RG4L = signedlong(RG5w);
  DM2_DRAW_ITEM_ICON(unsignedlong(vql_10.peek16()), RG4L, vl_04, vl_0c, vl_08);
  return 1;
}

void DM2_DRAW_SCROLL_TEXT(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG51p;
  i16 RG52w;
  t_text tarr_00[0xc8];
  t_text tarr_c8[0xc8];
  c_rect rc_190;
  i16 vw_198;
  i16 vw_19c; // y0
  i16 vw_1a0; // x0

  RG4L = unsignedlong(RG1W);
  RG51p = ADRESSOF(t_text, tarr_c8);
  DM2_QUERY_MESSAGE_TEXT(tarr_c8, RG4L, lcon(0xffff8002));
  ddat.v1d66fc = 5;
  DM2_DRAW_STATIC_PIC(7, 0, 1, wcon(0x1ee), NOALPHA);
  RG3L = lcon(0x1ee);
  RG2L = lcon(0x10);
  DM2_DRAW_STATIC_PIC(18, 0, RG2Blo, RG3W, 0xc);
  RG3L = 0;
  //m_2929D:
  for (;;)
  {
    RG1L = RG3L + 1;
    if (byte_at(RG51p) == 0)
      break;
    RG4L = unsignedlong(byte_at(RG51p));
    if (RG4L == lcon(0xa))
      RG3L = RG1L;
    RG51p++;
  }
  //m_292B3:
  RG4L = unsignedlong(byte_at(RG51p, -1));
  if (RG4L == lcon(0xa))
  {
    //m_292C1:
    RG1L = unsignedlong(byte_at(RG51p, -2));
    if (RG1L == RG4L)
      RG3L--;
  }
  else
    RG3L = RG1L;
  //m_292CB:
  DM2_QUERY_TOPLEFT_OF_RECT(lcon(0x230), OO vw_1a0, OO vw_19c);
  RG4R = &rc_190;
  DM2_QUERY_EXPANDED_RECT(wcon(0x1ee), RG4R);
  RG52w = strdat.strxplus;
  RG4L = signedlong(RG52w);
  RG1L = (signedlong(RG3W) * RG4L - signedlong(strdat.gfxstrw3) - 1) / 2 - signedlong(strdat.gfxstrw1);
  RG4L = RG1L;
  RG1L = signedlong(vw_19c) - RG4L;
  vw_19c = RG1W;
  RG2L = 0;
  vw_198 = RG2W;
  //m_2933D:
  for (;;)
  {
    RG1L = signedlong(vw_198);
    if (tarr_c8[RG1L] == bcon(0x0))
      return;
    DM2_gfxstr_3929_04e2(tarr_c8, tarr_00, &vw_198, signedlong(rc_190.w));
    DM2_gfxstr_24a5_0732(signedlong(vw_1a0), signedlong(vw_19c), tarr_00);
    vw_19c += RG52w;
    RG1L = signedlong(vw_198);
    RG1Blo = tarr_c8[RG1L];
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == lcon(0xa))
      vw_198++;
  }
  FOREND
}

i32 DM2_DRAW_ITEM_SURVEY(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG51l;
  i32 RG52l;
  i32 RG53l;
  i32 RG54l;
  i32 RG55l;
  c_wlreg _RG6;
  i8* RG7p;
  t_text tarr_00[0x80];
  i32 vla_80[0x4];

  RG6L = RG1L;
  vla_80[0x0] = lcon(0x02710270); // original location: 0x289ac
  if (RG6W == lcon(0xffffffff))
    return 0;
  //m_29701:
  RG51l = unsignedlong(RG6W);
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(CUTX16(RG51l))));
  RG7p = RG1P;
  DM2_DRAW_GUIDED_STR(CHGCAST(t_text, v1d10f0));
  RG1L = RG6L;
  RG1W &= lcon(0x3c00);
  RG1L = unsignedlong(RG1W) >> bcon(0xa);
  vla_80[0x2] = RG1L;
  RG1L = unsignedlong(mkw(vla_80[0x2]));
  if (RG1L == lcon(0x7))
  {
    DM2_DRAW_SCROLL_TEXT(RG51l);
    return 1;
  }
  ddat.v1d66fc = 3;
  if (RG4L == 0)
    return 0;
  DM2_DRAW_STATIC_PIC(7, 0, 1, wcon(0x1ee), NOALPHA);
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG51l) & lcon(0xff);
  vla_80[0x1] = RG1L;
  RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG51l);
  RG55l = unsignedlong(RG1Blo);
  RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(CUTLX8(RG55l), CUTLX8(vla_80[0x1]), 1, lcon(0x11)) ? 1 : 0;
  if (RG1L != 0)
    DM2_DRAW_STATIC_PIC(CUTLX8(RG55l), CUTLX8(vla_80[0x1]), 17, wcon(0x1ee), 0xc);
  DM2_DRAW_STATIC_PIC(7, 0, 5, wcon(0x1f8), 0xc);
  RG52l = unsignedlong(RG6W);
  RG1T = DM2_GET_ITEM_NAME(RG52l);
  RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL13]);
  RG4L = unsignedlong(RG4Blo);
  DM2_DRAW_VP_RC_STR(lcon(0x1fa), RG4W, RG1T);
  DM2_DRAW_ITEM_ICON(RG52l, lcon(0x2e), 0, 0, 0);
  RG1W = DM2_QUERY_ITEM_WEIGHT(CUTX16(RG52l));
  RG2L = unsignedlong(RG1W);
  RG3L = lcon(0xa);
  RG1L = RG2L;
  RG4L = RG1L % RG3L;
  RG1L /= RG3L;
  ddat.v1e099a = RG1W;
  RG1L = RG2L;
  RG4L = RG1L % RG3L;
  RG1L /= RG3L;
  ddat.v1e098e = RG4W;
  RG3P = ADRESSOF(t_text, tarr_00);
  RG2L = lcon(0x1f);
  RG4L = 0;
  RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x7), RG4Blo, RG2Blo, RG3T);
  DM2_DRAW_GUIDED_STR(RG1T);
  RG1L = unsignedlong(mkw(vla_80[0x2]));
  if (RG1L == lcon(0x9))
  {
    RG1L = DM2_IS_CONTAINER_CHEST(CUTX16(RG52l)) ? 1 : 0;
    if (RG1L != 0)
    {
      DM2_DRAW_CONTAINER_SURVEY(RG7p);
      return 1;
    }
    RG1L = DM2_IS_CONTAINER_MONEYBOX(RG52l);
    if (RG1L != 0)
      DM2_MONEY_BOX_SURVEY(RG52l);
    return 1;
  }
  //m_2989C:
  RG3L = 0;
  vla_80[0x3] = RG3L; // extended
  RG1L = DM2_query_2759_0155(RG52l);
  if (RG1L != 0)
  {
    RG4L = 0;
    RG1W = DM2_ADD_ITEM_CHARGE(RG52l, RG4L);
    RG2W = RG1W;
    if (RG1W != 0)
    {
      RG1L = signedlong(CUTX16(DM2_GET_MAX_CHARGE(RG52l)));
      RG4L = signedlong(RG2W);
      RG54l = signedlong(mkw(vla_80[0x0])); // truncated
      vla_80[0x3] = 1; // extended
      RG3L = lcon(0x71);
      RG2L = RG1L;
      RG1L = RG54l;
      DM2_DRAW_ITEM_STATS_BAR(RG1L, RG4L, RG2L, RG3L, lcon(0x4));
    }
  }
  //m_298F0:
  RG1L = RG6L;
  RG1W &= lcon(0x3c00);
  RG2L = unsignedlong(RG1W) >> bcon(0xa);
  if (mkul(RG2L) < mkul(lcon(0x6)))
  {
    //m_29910:
    if (RG2L != lcon(0x5))
      return 1;
    //m_29929:
    RG3Blo = lcon(0x72);
    RG2L = lcon(0x50);
    RG53l = unsignedlong(RG6W);
    RG4L = lcon(0x8);
    RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG53l, RG4L);
    if (RG1W == 0)
    {
      RG4L = lcon(0x9);
      RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG53l, RG4L);
      RG3Blo = lcon(0x73);
      RG2L = lcon(0x64);
    }
  }
  else
  {
    if (mkul(RG2L) <= mkul(lcon(0x6)))
    {
      //m_2995B:
      RG3Blo = lcon(0x72);
      RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG6W), lcon(0xb));
      RG1W = unsignedword(RG1Blo);
      RG2L = lcon(0xc8);
    }
    else
    {
      if (RG2L != lcon(0x8))
        return 1;
      //m_2991A:
      RG3Blo = lcon(0x75);
      RG1W = word_at(RG7p, 2) & lcon(0xff);
      RG2L = lcon(0xff);
    }
  }
  //m_29973:
  if (RG1W != 0)
  {
    RG3L = unsignedlong(RG3Blo);
    RG2L = signedlong(RG2W);
    RG4L = signedlong(RG1W);
    RG1L = signedlong(mkw(vla_80[0x3]));
    RG1L = signedlong(mkw(vla_80[RG1L / 2])); // truncated
    DM2_DRAW_ITEM_STATS_BAR(RG1L, RG4L, RG2L, RG3L, lcon(0x5));
  }
  return 1;
}

// belongs to DM2_DRAW_HAND_ACTION_ICONS
// was SKW_2405_05b4
static t_bmp* DM2_DRAW_ITEM_ON_WOOD_PANEL(i32 eaxl, i32 edxl, c_imgdesc* imgdesc)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  c_image image;
  t_palette palette[PAL16];
  i32 vl_14c;
  i16 vw_150;
  i8 vb_154;
  i8 vb_158;
  i32 l99; // 99 is not the offset!

  RG3L = RG1L;
  RG5l = signedlong(RG4W);
  RG2L = signedlong(RG1W);
  l99 = RG2L;
  RG1L = 263 * RG2L;
  // RG7L = 2 * RG5l + RG1L;  TODO: where is that needed?
  RG4L = unsignedlong(party.hero[l99].item[RG5l]);
  RG1L = unsignedlong(RG3W);
  RG2L = RG5l;
  RG1L = DM2_IS_ITEM_HAND_ACTIVABLE(RG1L, RG4L, RG2L);
  if (RG1L == 0)
    return NULL;
  RG1W = ddat.v1d271a;
  RG4W = ddat.v1d2722;
  RG4L += RG1L;
  RG1W = ddat.v1d271c;
  RG2W = ddat.v1d2724;
  RG2L += RG1L;
  RG1W = /*TAG dbidx*/ dm2_dballochandler.DM2_dballoc_3e74_5888();
  vw_150 = RG1W;
  RG2L = signedlong(RG2W);
  RG4L = signedlong(RG4W);
  RG1L = signedlong(RG1W);
  DM2_ALLOC_NEW_BMP(RG1W, RG4W, RG2W, BPP_8);
  RG1W = party.hero[l99].item[RG5l];
  if (RG1W != lcon(0xffffffff))
  {
    //m_285AD:
    RG5l = unsignedlong(RG1W);
    RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG5l);
    vb_158 = RG1Blo;
    RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG5l);
    vb_154 = RG1Blo;
    RG2L = 0;
    RG4L = lcon(0xffffffff);
    RG1Blo = DM2_guidraw_2405_014a(RG5l, RG4L, RG2L);
    vl_14c = 0;
  }
  else
  {
    vb_158 = 1;
    vb_154 = 2;
    RG1Blo = (RG5l == 1 ? 1 : 0) + lcon(0x7);
    vl_14c = 1;
  }
  //m_285E5:
  RG3L = unsignedlong(RG1Blo);
  DM2_QUERY_GDAT_SUMMARY_IMAGE(&image, vb_158, vb_154, RG3Blo);
  DM2_QUERY_PICST_IT(&image);
  RG3UBlo = palettecolor_to_ui8(paldat.palette[E_COL08]);
  RG2L = unsignedlong(RG3Blo);
  RG1L = signedlong(vw_150);
  t_bmp* bmp = DM2_SET_IMAGE((t_dbidx)RG1W, imgdesc);
  DM2_FILL_ENTIRE_PICT(bmp, ui8_to_pixel(RG2UBlo));
  if (vl_14c == 0)
  {
    //m_28655:
    RG4L = 0;
    //m_28669:
    for (;;)
    {
      RG1L = signedlong(RG4W);
      if (RG1L >= lcon(0x10))
      {
        ui8_to_palettecolor(&palette[E_COL12], RG3UBlo);
        RG1R = tmprects.alloc_tmprect(ddat.v1d2722, ddat.v1d2724, ddat.v1d271a, ddat.v1d271c);
        RG2R = RG1R;
        DM2_DRAW_DIALOGUE_PICT(DM2_QUERY_PICT_BITS(&image.imgdesc), bmp, RG2R, 0, 0, lcon(0xffff), palette);
        RG3L = 1;
        DM2_CONVERT_PALETTE256(image.palette, RG3Blo, 0, RG3Blo, IO image.colors);
        bmp = DM2_SET_IMAGE((t_dbidx)vw_150, imgdesc);
        RG4L = signedlong(ddat.v1d271c);
        RG1L = signedlong(ddat.v1d271a);
        break;
      }
      //m_28659:
      RG2UBlo = palettecolor_to_ui8(paldat.palette[E_COL00]);
      ui8_to_palettecolor(&palette[RG1L], RG2UBlo);
      RG4L++;
    }
  }
  else
  {
    RG4L = signedlong(image.imgdesc.height);
    RG1L = signedlong(image.imgdesc.width);
  }
  //m_28703:
  DM2_DRAW_DIALOGUE_PICT(DM2_QUERY_PICT_BITS(&image.imgdesc), bmp, tmprects.alloc_origin_tmprect(RG1W, RG4W), 0, 0, 12, image.palette);
  return bmp;
}

// was SKW_29ee_026c
void DM2_DRAW_HAND_ACTION_ICONS(i16 eaxw, i32 edxl, i32 ebxl)
{
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i8* RG52p;
  i16 RG53w;
  c_ql32 vql_00;
  c_ql32 vql_04;
  c_imgdesc imgdesc;
  c_rect rc_20;
  c_rect rc_28;
  i16 parw00;
  i16 parw03;

  vql_04.poke32(RG1L);
  vql_00.poke32(RG4L);
  DM2_guidraw_29ee_00a3(0);
  c_hero* hero = &party.hero[vql_04.peek16()];
  RG1L = unsignedlong(RG4W);
  if (RG1L != 1)
    //m_32E91:
    RG1L = lcon(0x4a);
  else
    RG1L = lcon(0x46);
  //m_32E96:
  RG4L = unsignedlong(hero->partypos) + lcon(0x4);
  RG4W -= ddat.v1e0258;
  RG4W &= 0x3;
  RG1L += RG4L;
  RG53w = RG1W;
  RG4R = &rc_28;
  DM2_QUERY_EXPANDED_RECT(RG53w, RG4R);
  if (hero->curHP == 0)
  {
    DM2_FILL_RECT_SUMMARY(&dm2_buttongroup2, /*TAG BLITRECT*/ &rc_28, palettecolor_to_pixel(paldat.palette[E_COL00]));
    return;
  }
  //m_32EDD:
  put16(parw00, RG53w);
  RG1Blo = 2 * vql_00.peek8() + bcon(0x2) + RG2Blo;
  RG2L = unsignedlong(RG1Blo);
  DM2_DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 4, RG2Blo, &dm2_buttongroup2, parw00);
  RG4L = signedlong(vql_00.peek16());
  t_bmp* bmp = DM2_DRAW_ITEM_ON_WOOD_PANEL(signedlong(vql_04.peek16()), RG4L, &imgdesc);
  if (bmp != NULL)
  {
    RG1UBlo = palettecolor_to_ui8(paldat.palette[E_COL08]);
    RG1L = unsignedlong(RG1Blo);
    put16(parw03, RG1W);
    RG3L = signedlong(getbmpheader(bmp)->height);
    RG2L = signedlong(getbmpheader(bmp)->width);
    RG4R = &rc_28;
    RG1R = &rc_20;
    RG1R->calc_centered_rect_in_rect(RG4R, RG2W, RG3W);
    RG3L = 0;
    RG2R = RG1R;
    DM2_DRAW_ICON_PICT_BUFF(bmp, &dm2_buttongroup2, /*TAG blitrect*/ RG2R, /*TAG srcx*/ RG3W, 0, /*TAG alphamask*/ parw03, BLITMODE0);
    DM2_image_0b36_01cd(&imgdesc);
  }
  RG1L = unsignedlong(vql_00.peek16());
  if (hero->handcooldown[RG1L] == 0)
  {
    if (ddat.v1e0238 == 0)
      return;
  }
  DM2_guidraw_0b36_105b(&dm2_buttongroup2, &rc_28, 0);
}

// stacksize was 0xa8
void DM2_DRAW_MAP_CHIP(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1, i8 argb2, i32 argl3)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_nreg _RG5; // not checked yet, a lot of variables
  i8* RG52p;
  c_wlreg _RG61;
  c_lreg _RG62;
  c_wlreg _RG63;
  c_lreg _RG64;
  c_wlreg _RG65;
  c_lreg _RG66;
  c_lreg _RG67;
  c_lreg _RG68;
  i8* RG62p;
  c_ql32 vql_00;
  i16 vw_04;
  i16 vw_08;
  i16 vw_0c;
  s_8wordarray s15_10;
  t_palette palette1[PAL16];
  t_palette palette2[PAL16];
  c_rect rc_40;
  c_pixel* xp_48;
  c_pixel* xp_4c;
  unk* xp_50;
  i32 vl_54;
  t_bmp* bmp_58;
  i32 vl_5c;
  u_lp u85_60; // ATTENTION!
  unk* xp_64; // set to 1 at one location!
  c_ql32 vql_6c;
  i16 vw_70;
  c_o32 vo_74; // poke 16 peek 8 32 add16 inc16
  i16 vw_78;
  i16 vw_7c;
  i8 vb_80;
  i16 vw_84;
  i8 vb_88;
  c_ql32 vql_8c;
  c_ql32 vql_90;
  i16 vw_94;
  i16 parw00;
  t_palette* pal1;
  i32 parl17;
  unk** xparpp25;
  i32 parl27;
  bool flag;
  bool skip00738 = false;

  vw_0c = RG1W; // TODO: incoming RG1W word only
  vw_08 = RG4W; // adj TODO: incoming RG4W word only
  vw_04 = RG2W; // adj TODO: incoming RG2W word only
  vql_00.poke32(RG3L);
  xp_48 = NULL;
  RG3L = signedlong(vw_04);
  DM2_SUMMARIZE_STONE_ROOM(&s15_10, signedlong(vql_00.peek16()), signedlong(vw_08), RG3L);
  RG4L = unsignedlong(argb2);
  DM2_QUERY_4BPP_PICT_BUFF_AND_PAL(lcon(0x8), RG4L, &xp_48, palette1);
  RG1L = argl3;
  RG1W &= lcon(0x10);
  RG1L = unsignedlong(RG1W);
  if (RG1L != 0)
  {
    RG1W = ddat.v1d2750; // TODO: gets extended w/o conversion now
    RG1L = 2 * RG1L;
    RG1W += ddat.v1d274c;
    RG1L = signedlong(RG1W);
    put16(parw00, RG1W);
    RG1W = ddat.v1d274e; // TODO: gets extended w/o conversion now
    RG1L = 2 * RG1L;
    RG1W += ddat.v1d274a;
    RG3L = signedlong(RG1W);
    RG1W = argw1 - ddat.v1d2750; // adj
    RG2L = signedlong(RG1W);
    RG1W = argw0 - ddat.v1d274e; // adj
    RG4L = signedlong(RG1W);
    RG1R = &rc_40;
    RG1R->set(RG4W, RG2W, RG3W, parw00);
    RG4R = &glblrects.dm2rect3;
    i16 dummyx, dummyy;
    RG1R->unify(RG4R, OO dummyx, OO dummyy);
    RG2L = unsignedlong(palettecolor_to_ui8(paldat.palette[E_COL08]));
    RG4R = &rc_40;
    RG1P = DOWNCAST(c_buttongroup, &dm2_buttongroup2);
    DM2_FILL_RECT_SUMMARY(UPCAST(c_buttongroup, RG1P), /*TAG BLITRECT*/ RG4R, ui8_to_pixel(RG2UBlo));
  }
  //m_33BFE:
  RG1W = s15_10.warr_00[0x1]; // adj
  RG1L <<= bcon(0x10);
  RG1L >>= bcon(0x15);
  RG1L = unsignedlong(RG1Blo);
  if (RG1L != lcon(0x6))
    skip00738 = true;
  else
  {
    RG1W = s15_10.warr_00[0x1] & 1; // adj
    RG1L = signedlong(RG1W);
    if (RG1L == 0)
      skip00738 = true;
    else
      RG1L = 1;
  }

  if (skip00738)
    //m_33C21:
    RG1L = 0;
  //m_33C23:
  vl_54 = RG1L;
  RG2P = NULL;
  xp_64 = UPCAST(unk, RG2P);
  //m_33C2B:
  for (;;)
  {
    bool skip00740 = false;
    bool skip00743 = false;
    bool skip00748 = false;
    bool skip00749 = false;
    RG1W = s15_10.warr_00[0x0];
    if (RG1UW < mkuw(lcon(0x5)))
    {
      //m_33C6A:
      if (RG1UW < mkuw(1))
      {
        //m_33C85:
        if (RG1W == 0)
          skip00740 = true;
      }
      else
      {
        if (RG1UW <= mkuw(1))
        {
          //m_33DD0:
          RG1W = s15_10.warr_00[0x1] & lcon(0x8); // adj
          RG1L = signedlong(RG1W);
          if (RG1L != 0)
          {
            RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
            RG1W = word_at(RG1P, lcon(0x8)) & lcon(0x3f);
            RG1L = unsignedlong(RG1W);
            RG4P = ddat.v1e03d4;
            RG4Blo = byte_at(RG4P, RG1L);
            RG4L = unsignedlong(RG4Blo);
            DM2_QUERY_4BPP_PICT_BUFF_AND_PAL(lcon(0x8), RG4L, &xp_48, palette1);
            parl27 = 0;
            RG3L = signedlong(argw1);
            RG2L = signedlong(argw0);
            RG4L = 0;
            jump L_fin;
          }
          skip00743 = true;
        }
        else
        {
          if (RG1W == 2)
          {
            //m_33FDE:
            vo_74.poke16(lcon(0x3));
            if (s15_10.warr_00[0x3] != 0)
            {
              RG1L = argl3;
              RG1W &= 0x2;
              RG1L = unsignedlong(RG1W);
              flag = RG1L == 0;
              vo_74.poke16(lcon(0x4));
              if (!flag)
              {
                if ((CUTLX8(timdat.gametick) & bcon(0x1)) != 0)
                  vo_74.poke16(lcon(0x5));
              }
            }
            skip00748 = true;
          }
        }
      }
    }
    else
    {
      if (RG1UW <= mkuw(lcon(0x5)))
        skip00743 = true;
      else
      {
        if (RG1UW < mkuw(lcon(0x10)))
        {
          //m_33C5F:
          if (RG1W == lcon(0x7))
            skip00740 = true;
        }
        else
        {
          bool skip00746 = false;
          if (RG1UW <= mkuw(lcon(0x10)))
          {
            //m_33F58:
            vo_74.poke16(lcon(0xb));
            skip00746 = true;
          }
          else
          {
            if (RG1UW <= mkuw(lcon(0x11)))
            {
              //m_33F60:
              vo_74.poke16(lcon(0xa));
              skip00746 = true;
            }
            else
            {
              if (RG1UW <= mkuw(lcon(0x13)))
              {
                //m_33FA2:
                RG1L = DM2_query_0cee_06dc(signedlong(vw_08), signedlong(vw_04)) - vql_00.peek32();
                RG1W &= 0x3;
                vo_74.poke16(RG1W);
                if (s15_10.warr_00[0x3] != 0)
                {
                  RG3L = RG1L + 2;
                  vo_74.poke16(RG3W);
                  RG5L = RG3L & lcon(0x3);
                  vo_74.poke16(RG5W);
                }
                vo_74.add16(lcon(0x6));
                skip00748 = true;
              }
            }
          }

          if (skip00746)
          {
            bool skip00747 = false;
            //m_33F66:
            RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(s15_10.warr_00[0x4])));
            if (jz_test8(location(RG1P + 2), lcon(0x40)) && jz_test8(location(RG1P + lcon(0x3)), lcon(0x20)))
              vo_74.add16(2);
            if (s15_10.warr_00[0x3] == 0)
              skip00747 = true;
            else
            {
              RG1L = signedlong(s15_10.warr_00[0x3]);
              if (RG1L == lcon(0x5))
                skip00747 = true;
            }

            if (skip00747)
              //m_33F98:
              vo_74.add16(lcon(0x4));
            skip00748 = true;
          }
        }
      }
    }

    if (skip00740)
    {
      //m_33C8E:
      RG1L = argl3;
      RG1W &= 0x2;
      RG1L = unsignedlong(RG1W);
      if (RG1L == 0 || vl_54 == 0 || (CUTLX8(timdat.gametick) & bcon(0x2)) == 0)
      {
        //m_33CB9:
        if (xp_64 == NULL)
        {
          RG3L = signedlong(argw1);
          RG2L = signedlong(argw0);
          RG4L = 0;
          DM2_DRAW_CHIP_OF_MAGIC_MAP(BMPCAST(xp_48), RG4L, RG2L, RG3L, 0, palette1);
        }
        //m_33CE1:
        RG3L = 0;
        vo_74.poke16(0);
        //m_33DBC:
        for (;;)
        {
          RG1L = signedlong(vo_74.peek16());
          if (RG1L > lcon(0x3))
            break;
          //m_33CEC:
          RG1W = s15_10.warr_00[3 + RG1L];
          RG1W = unsignedword(RG1Blo);
          vw_94 = RG1W;
          RG1Blo = CUTX8(vw_94);
          if (RG1Blo != lcon(0xffffffff))
          {
            bool skip00741 = false;
            RG1L = argl3;
            RG1W &= 0x2;
            RG1L = unsignedlong(RG1W);
            if (RG1L != 0)
              skip00741 = true;
            else
            {
              RG4L = unsignedlong(CUTX8(vw_94));
              RG3L = lcon(0xf9);
              RG2L = lcon(0xb);
              RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, RG4Blo, RG2Blo, RG3Blo);
              if (RG1W == 0)
                skip00741 = true;
            }

            if (skip00741)
            {
              //m_33D35:
              RG4L = unsignedlong(CUTX8(vw_94));
              RG1L = DM2_QUERY_4BPP_PICT_BUFF_AND_PAL(lcon(0x9), RG4L, &xp_4c, palette2);
              if (RG1W != 0)
              {
                RG4L = signedlong(vo_74.peek16());
                RG4W = s15_10.warr_00[0x3 + RG4L]; // replacement for mov8(RG4Blo, lst.o_peek8(0x17, 2 * RG4L));
                RG4UW >>= bcon(0x8);
                RG4L <<= bcon(0x18);
                RG4L >>= bcon(0x1a);
                RG3L = RG4L;
                RG4L = 2 * signedlong(RG4W);
                RG2L = signedlong(RG1W);
                RG4L += 2;
                RG1W = vo_74.peek16();
                RG1W &= 0x1;
                if (RG2L >= RG4L)
                {
                  //m_33D7A:
                  RG3L = 2 * RG3L + RG1L;
                  vw_94 = RG3W;
                }
                else
                  vw_94 = RG1W;
                //m_33D82:
                RG1L = signedlong(vo_74.peek16());
                RG1Blo = table1d6980[RG1L];
                RG1L = unsignedlong(RG1Blo);
                i32 parl03 = RG1L;
                RG3L = signedlong(argw1);
                RG2L = signedlong(argw0);
                RG4L = signedlong(vw_94);
                DM2_DRAW_CHIP_OF_MAGIC_MAP(BMPCAST(xp_4c), RG4L, RG2L, RG3L, parl03, palette2);
              }
            }
          }
          //m_33DB8:
          vo_74.inc16();
        }
      }
      else
      {
        vo_74.poke16(1);
        skip00748 = true;
      }
    }

    c_pixel* xgfx;
    if (skip00743)
    {
      //m_33E2C:
      RG3L = signedlong(argw1);
      RG2L = signedlong(argw0);
      RG4L = 2;
      DM2_DRAW_CHIP_OF_MAGIC_MAP(BMPCAST(xp_48), RG4L, RG2L, RG3L, 0, palette1);
      RG1W = s15_10.warr_00[0x5]; // adj
      RG1W = unsignedword(RG1Blo);
      vo_74.poke16(RG1W);
      RG1Blo = vo_74.peek8();
      if (RG1Blo != lcon(0xffffffff))
      {
        bool skip00744 = false;
        RG1L = argl3;
        RG1W &= 0x2;
        RG1L = unsignedlong(RG1W);
        if (RG1L != 0)
          skip00744 = true;
        else
        {
          RG4L = unsignedlong(vo_74.peek8());
          RG3L = lcon(0xf9);
          RG2L = lcon(0xb);
          RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0xa), RG4Blo, RG2Blo, RG3Blo);
          if (RG1W == 0)
            skip00744 = true;
        }

        if (skip00744)
        {
          //m_33E94:
          RG4L = unsignedlong(vo_74.peek8());
          RG1L = DM2_QUERY_4BPP_PICT_BUFF_AND_PAL(10, RG4L, &xp_4c, palette2);
          RG2L = RG1L;
          if (RG1W != 0)
          {
            RG4L = signedlong(mkb(s15_10.warr_00[0x5] >> bcon(0x8))); // truncated
            RG3L = lcon(0xa);
            RG1L = RG4L;
            RG4L = RG1L % RG3L;
            RG1L /= RG3L;
            RG3L = RG1L;
            RG1L = signedlong(RG1W);
            RG4L = signedlong(RG2W);
            RG1L++;
            if (RG4L >= RG1L)
              //m_33ED5:
              vo_74.poke16(RG3W);
            else
            {
              RG4L = 0;
              vo_74.poke16(0);
            }
            //m_33ED9:
            RG3L = signedlong(argw1);
            RG2L = signedlong(argw0);
            RG4L = signedlong(vo_74.peek16());
            DM2_DRAW_CHIP_OF_MAGIC_MAP(BMPCAST(xp_4c), RG4L, RG2L, RG3L, 0, palette2);
          }
        }
      }
      //m_33EFF:
      RG1L = signedlong(s15_10.warr_00[0x0]);
      if (RG1L == lcon(0x5))
      {
        RG4L = 0;
        RG1L = DM2_QUERY_4BPP_PICT_BUFF_AND_PAL(24, RG4L, &xp_4c, palette2);
        if (RG1W != 0)
        {
          pal1 = palette2;
          RG3L = signedlong(argw1);
          RG2L = signedlong(argw0);
          RG5L = signedlong(RG1W);
          RG4L = signedlong(CUTX16(timdat.gametick % RG5L));
          xgfx = xp_4c;
          skip00749 = true;
        }
      }
    }

    if (skip00748)
    {
      //m_34011:
      pal1 = palette1;
      RG3L = signedlong(argw1);
      RG2L = signedlong(argw0);
      RG4L = signedlong(vo_74.peek16());
      xgfx = xp_48;
      skip00749 = true;
    }

    if (skip00749)
      //m_34032:
      DM2_DRAW_CHIP_OF_MAGIC_MAP(BMPCAST(xgfx), RG4L, RG2L, RG3L, 0, pal1);

    //m_34037:
    RG1L = argl3;
    RG1W &= lcon(0x10);
    RG1L = unsignedlong(RG1W);
    if (RG1L != 0)
    {
      RG1L = 0;
      vo_74.poke16(RG1W);
      //m_34107:
      for (;;)
      {
        vl_5c = signedlong(vo_74.peek16());
        RG1L = unsignedlong(party.heros_in_party);
        if (RG1L <= vl_5c)
          break;
        //m_34057:
        RG5L = 263 * vl_5c;
        if (party.hero[vl_5c].curHP != 0)
        {
          RG68L = unsignedlong(vw_0c);
          RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG68L);
          RG4Blo = RG1Blo;
          RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG68L);
          RG3L = unsignedlong(RG1Blo);
          RG67L = unsignedlong(RG4Blo);
          RG2L = lcon(0x40);
          RG4L = RG3L;
          bmp_58 = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(CUTLX8(RG67L), RG4Blo, RG2Blo));
          t_palette* pal2 = DM2_QUERY_GDAT_IMAGE_LOCALPAL(CUTLX8(RG67L), RG3Blo, lcon(0x40));
          RG1L = unsignedlong(party.hero[RG5L / 263].partypos);
          RG4L = signedlong(vql_00.peek16());
          RG1L += 4;
          RG1L -= RG4L;
          RG1L &= 0x3;
          RG1Blo = table1d6984[RG1L];
          RG1L = unsignedlong(RG1Blo);
          i32 parl11 = RG1L;
          RG3L = signedlong(argw1);
          RG2L = signedlong(argw0);
          if (ddat.savegames1.b_02 == 0)
            //m_340F2:
            RG1L = vl_5c + lcon(0x5);
          else
            RG1L = lcon(0x9);
          //m_340F8:
          RG4L = signedlong(RG1W);
          DM2_DRAW_CHIP_OF_MAGIC_MAP(bmp_58, RG4L, RG2L, RG3L, parl11, pal2);
        }
        //m_34103:
        vo_74.inc16();
      }
    }
    //m_34123:
    RG1L = argl3;
    RG1W &= lcon(0x40);
    RG1L = unsignedlong(RG1W);
    if (RG1L != 0)
    {
      bool skip00752 = false;
      RG1W = ddat.v1e0b56;
      if (RG1UW >= mkuw(2))
      {
        if (RG1UW <= mkuw(2))
        {
          //m_34154:
          vo_74.poke16(lcon(0xb));
          skip00752 = true;
        }
        else
        {
          if (RG1W == lcon(0x3))
          {
            //m_3415C:
            vo_74.poke16(lcon(0xa));
            skip00752 = true;
          }
        }
      }

      if (!skip00752)
        //m_3414C:
        vo_74.poke16(2);

      //m_34162:
      RG4L = unsignedlong(vw_0c);
      RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG4L);
      RG2Blo = RG1Blo;
      RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L);
      RG5L = unsignedlong(RG1Blo);
      RG3L = unsignedlong(RG2Blo);
      RG2L = lcon(0x40);
      RG4L = RG5L;
      t_bmp* bmp61 = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(RG3Blo, RG4Blo, RG2Blo));
      t_palette* pal3 = DM2_QUERY_GDAT_IMAGE_LOCALPAL(RG3Blo, RG5Blo, lcon(0x40));
      RG3L = signedlong(argw1);
      RG2L = signedlong(argw0);
      RG4L = signedlong(vo_74.peek16());
      DM2_DRAW_CHIP_OF_MAGIC_MAP(bmp61, RG4L, RG2L, RG3L, 0, pal3);
    }
    //m_341BF:
    RG1L = argl3;
    RG1W &= lcon(0xa5);
    RG1L = unsignedlong(RG1W);
    if (RG1L == 0)
      return;
    RG1L = lcon(0xfffe);
    vql_8c.poke32(RG1L);
    vql_6c.poke32(RG1L);
    vql_90.poke32(RG1L);
    RG5W = s15_10.warr_00[0x2]; // adj
    //m_341E5:
    for (;;)
    {
      bool skip00759 = false;
      if (RG5W == lcon(0xfffffffe))
      {
        //m_3438E:
        RG2L = vql_90.peek32();
        if (RG2W == RG5W)
          break;
        RG4L = unsignedlong(RG2W);
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
        RG52p = RG1P;
        RG62p = RG1P;
        RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(RG4L);
        vw_84 = RG1W;
        vb_80 = RG1Blo;
        RG1L = argl3;
        RG1W &= lcon(0x20);
        RG1L = unsignedlong(RG1W);
        t_palette* pal4;
        c_pixel* xgfx;
        if (RG1L == 0)
        {
          //m_3443B:
          RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L) & lcon(0xff);
          u85_60.l_00 = RG1L;
          RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG4L) & lcon(0xff);
          RG4L = u85_60.l_00;
          RG1L = DM2_QUERY_4BPP_PICT_BUFF_AND_PAL(RG1L, RG4L, &xp_4c, palette2);
          if (RG1W == 0)
          {
            //m_34507:
            if ((vw_84 & 0x1) != 0 || (vw_84 & wcon(0x2000)) != 0)
              skip00759 = true;
            else
            {
              RG4L = unsignedlong(vw_0c);
              RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG4L);
              RG2Blo = RG1Blo;
              RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L);
              RG3L = unsignedlong(RG1Blo);
              RG5L = unsignedlong(RG2Blo);
              RG2L = lcon(0x40);
              RG4L = RG3L;
              u85_60.xp_00 = DOWNCAST(t_bmp, BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(RG5Blo, RG4Blo, RG2Blo)));
              pal4 = DM2_QUERY_GDAT_IMAGE_LOCALPAL(RG5Blo, RG3Blo, lcon(0x40));
              parl17 = 0;
              RG3L = signedlong(argw1);
              RG2L = signedlong(argw0);
              RG1W = CUTX16(timdat.gametick) & 1;
              RG4L = signedlong(RG1W);
              xgfx = UPCAST(c_pixel, u85_60.xp_00);
            }
          }
          else
          {
            if ((vw_84 & 0x1) == 0)
            {
              //m_3448E:
              RG4L = signedlong(RG1W);
              RG1L = RG4L;
              RG4L >>= bcon(0x1f);
              RG1L -= RG4L;
              RG1L >>= bcon(0x1);
              RG2L = RG1L;
              RG4L = timdat.gametick % RG2L;
              RG1L = 2 * signedlong(RG4W);
              vo_74.poke16(RG1W);
            }
            else
            {
              RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG52p, lcon(0x4)))));
              RG4P = RG1P;
              RG1P = RG52p;
              DM2_query_1c9a_02c3(RG1P, RG4P);
              RG3L = 0;
              vo_74.poke16(RG3W);
            }
            //m_344AE:
            RG1W = word_at(RG62p, lcon(0xe));
            RG1UW = lrshift6e(RG1UW);
            RG4L = unsignedlong(RG1W);
            RG1L = signedlong(vql_00.peek16()) - RG4L & lcon(0x3);
            vw_94 = RG1W;
            pal4 = palette2; // ATTENTION: there are three of them
            RG1L = signedlong(vw_94);
            RG1Blo = table1d6980[RG1L];
            RG1L = unsignedlong(RG1Blo);
            parl17 = RG1L;
            RG3L = signedlong(argw1);
            RG2L = signedlong(argw0);
            RG1W = vw_94;
            RG1W &= 0x1;
            RG1W += vo_74.peek16();
            RG4L = signedlong(RG1W);
            xgfx = xp_4c;
          }
        }
        else
        {
          RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(unsignedlong(vw_0c));
          RG3Bhi = RG1Blo;
          RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(unsignedlong(vw_0c));
          RG3Blo = RG1Blo;
          RG2L = lcon(0x40);
          RG4L = unsignedlong(RG1Blo);
          bmp_58 = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(RG3Bhi, RG4Blo, RG2Blo));
          RG2L = lcon(0x40);
          RG4L = unsignedlong(RG3Blo);
          RG1L = unsignedlong(RG3Bhi);
          pal4 = DM2_QUERY_GDAT_IMAGE_LOCALPAL(RG1Blo, RG4Blo, RG2Blo);
          RG1L = 0;
          parl17 = RG1L;
          RG3L = signedlong(argw1);
          RG2L = signedlong(argw0);
          RG1W = CUTX16(timdat.gametick) & 1;
          RG1L += 3;
          RG4L = signedlong(RG1W);
          xgfx = PIXELCAST(bmp_58);
        }

        if (!skip00759)
          //m_34574:
          DM2_DRAW_CHIP_OF_MAGIC_MAP(BMPCAST(xgfx), RG4L, RG2L, RG3L, parl17, pal4);
      }
      else
      {
        RG1L = RG5L;
        RG1W &= lcon(0x3c00);
        RG1L = unsignedlong(RG1W) >> bcon(0xa);
        vo_74.poke16(RG1W);
        RG1L = signedlong(vo_74.peek16());
        if (RG1L != lcon(0x4))
        {
          //m_34270:
          RG4L = argl3;
          RG4W &= 0x4;
          RG4L = unsignedlong(RG4W);
          if (RG4L != 0)
          {
            if (RG1L != lcon(0xe))
            {
              //m_3429F:
              if (RG1L != lcon(0xf))
              {
                //m_342B7:
                if (RG1L >= lcon(0x5) && RG1L <= lcon(0xa))
                {
                  RG61L = unsignedlong(RG5W);
                  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG61L);
                  RG4L = unsignedlong(RG1Blo);
                  RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG61L) & lcon(0xff);
                  RG1L = DM2_QUERY_4BPP_PICT_BUFF_AND_PAL(RG1L, RG4L, &xp_4c, palette2);
                  if (RG1W != 0)
                  {
                    bool skip00755 = false;
                    RG61L >>= 14;
                    RG61L -= signedlong(vql_00.peek16());
                    RG4L = RG61L & lcon(0x3);
                    vw_94 = RG4W;
                    if (s15_10.warr_00[0x0] != 0 || vl_54 != 0)
                    {
                      //m_3433A:
                      RG61L = 0;
                      vo_74.poke16(RG61W);
                      RG1L = signedlong(vw_94);
                      RG1Blo = table1d6984[RG1L];
                      skip00755 = true;
                    }
                    else
                    {
                      if (RG1W > wcon(0x2))
                      {
                        RG1W = vw_94;
                        RG1W &= 0x1;
                        RG1L++;
                        vo_74.poke16(RG1W);
                        RG1L = signedlong(vw_94);
                        RG1Blo = table1d6980[RG1L];
                        skip00755 = true;
                      }
                    }

                    if (skip00755)
                    {
                      //m_3434C:
                      RG1W = unsignedword(RG1Blo);
                      vw_94 = RG1W;
                      i32 parl15 = unsignedlong(vw_94);
                      RG3L = signedlong(argw1);
                      RG2L = signedlong(argw0);
                      RG4L = signedlong(vo_74.peek16());
                      DM2_DRAW_CHIP_OF_MAGIC_MAP(BMPCAST(xp_4c), RG4L, RG2L, RG3L, parl15, palette2);
                    }
                  }
                }
              }
              else
              {
                if (vql_8c.peek16() == lcon(0xfffffffe))
                  vql_8c.poke32(RG5L);
              }
            }
            else
            {
              if (vql_6c.peek16() == lcon(0xfffffffe))
                vql_6c.poke32(RG5L);
            }
          }
        }
        else
        {
          bool skip00754 = false;
          RG62L = unsignedlong(RG5W);
          RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(RG62L);
          vb_88 = RG1Blo;
          RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG62L);
          RG4L = unsignedlong(RG1Blo);
          RG3L = lcon(0xf9);
          RG2L = lcon(0xb);
          RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(15, RG4Blo, RG2Blo, RG3Blo);
          if (RG1W != 0)
            skip00754 = true;
          else
          {
            if ((vb_88 & bcon(0x1)) == 0)
            {
              //m_34264:
              RG1L = argl3;
              RG1W &= lcon(0x21);
            }
            else
            {
              RG1L = argl3;
              RG1W &= lcon(0x80);
            }
            //m_34251:
            RG1L = unsignedlong(RG1W);
            if (RG1L != 0)
              skip00754 = true;
          }

          if (skip00754)
            //m_3425C:
            vql_90.poke32(RG5L);
        }
        //m_3437D:
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG5UW)));
        RG5L = RG1L;
        continue;
      }

      //m_34579:
      if ((vb_80 & bcon(0x1)) != 0)
      {
        RG1L = argl3;
        RG1W &= 0x4;
        RG1L = unsignedlong(RG1W);
        if (RG1L != 0)
        {
          RG1L = DM2_query_0cee_2df4(unsignedlong(vql_90.peek16()));
          if ((RG1Blo & bcon(0xf)) != 0)
          {
            RG5W = word_at(RG62p);
            //m_345AE:
            for (;;)
            {
              if (RG5W == lcon(0xfffffffe))
                break;
              RG1L = RG5L;
              RG1W &= lcon(0x3c00);
              RG1L = unsignedlong(RG1W) >> bcon(0xa);
              vo_74.poke16(RG1W);
              RG1L = signedlong(vo_74.peek16());
              if (RG1L >= lcon(0x5) && RG1L <= lcon(0xa))
              {
                RG66L = unsignedlong(RG5W);
                RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG66L);
                RG4L = unsignedlong(RG1Blo);
                RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG66L) & lcon(0xff);
                RG1L = DM2_QUERY_4BPP_PICT_BUFF_AND_PAL(RG1L, RG4L, &xp_4c, palette2);
                if (RG1W != 0)
                {
                  RG66L >>= 14;
                  RG66L -= signedlong(vql_00.peek16());
                  RG1L = RG66L & lcon(0x3);
                  RG1Blo = table1d6984[RG1L];
                  RG1L = unsignedlong(RG1Blo);
                  i32 parl19 = RG1L;
                  RG3L = signedlong(argw1);
                  RG2L = signedlong(argw0);
                  RG4L = 0;
                  DM2_DRAW_CHIP_OF_MAGIC_MAP(BMPCAST(xp_4c), RG4L, RG2L, RG3L, parl19, palette2);
                }
              }
              //m_34643:
              RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG5UW)));
              RG5L = RG1L;
            }
          }
        }
      }
      break;
    }
    //m_34654:
    RG5L = vql_6c.peek32();
    //m_34657:
    for (;;)
    {
      if (RG5W != lcon(0xfffffffe))
      {
        RG1L = RG5L;
        RG1W &= lcon(0x3c00);
        RG1L = unsignedlong(RG1W) >> bcon(0xa);
        if (RG1L == lcon(0xe))
        {
          RG63L = unsignedlong(RG5W);
          RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG63L);
          RG4L = unsignedlong(RG1Blo);
          RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG63L) & lcon(0xff);
          RG1L = DM2_QUERY_4BPP_PICT_BUFF_AND_PAL(RG1L, RG4L, &xp_4c, palette2);
          if (RG1W != 0)
          {
            if (RG1W > wcon(0x3))
            {
              //m_346BA:
              RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG63UW));
              RG1W = timdat.timerarray[word_at(RG1P, lcon(0x6))].getB();
              RG1L <<= 4;
              RG1UW >>= 14;
              vw_94 = RG1W;
              RG1L -= vql_00.peek32();
              RG1W &= 0x3;
              vw_94 = RG1W;
              RG2L = 3;
              vo_74.poke16(RG2W);
              RG1L = DM2_query_48ae_011a(RG63L) + 1;
              if (RG1UW <= mkuw(lcon(0x4)))
              {
                RG1L = signedlong(RG1W);
                switch (RG1L)
                {
                  case 0:
                    //m_34713:
                    RG4L = 0;
                    vo_74.poke16(RG4W);
                    break;

                  case 1:
                    //m_34747:
                    RG1L = 0;
                    vo_74.poke16(0);
                    break;

                  case 2:
                    //m_3471B:
                    RG1W = vw_94 + RG2W;
                    vo_74.poke16(RG1W);
                    RG63L >>= 14;
                    RG63L -= signedlong(vql_00.peek16());
                    RG63L &= 0x3;
                    RG1L = signedlong(vo_74.peek16());
                    RG1W = signedword(table1d6984[4 * (RG1L - 2) + RG63L]);
                    vo_74.add16(RG1W);
                    break;

                  case 3:
                    //m_3474F:
                    RG3L = 0;
                    vo_74.poke16(0);
                  case 4:
                    break;

                  default: throw(THROW_DMABORT);
                }
              }
            }
            else
            {
              RG2L = 0;
              vo_74.poke16(RG2W);
            }
            //m_34755:
            RG4L = (unsignedlong(RG5W) >> bcon(0xe)) - signedlong(vql_00.peek16());
            RG1L = RG4L & lcon(0x3);
            RG1Blo = table1d6984[RG1L];
            RG1L = unsignedlong(RG1Blo);
            i32 parl21 = RG1L;
            RG3L = signedlong(argw1);
            RG2L = signedlong(argw0);
            RG4L = signedlong(vo_74.peek16());
            DM2_DRAW_CHIP_OF_MAGIC_MAP(BMPCAST(xp_4c), RG4L, RG2L, RG3L, parl21, palette2);
          }
        }
        //m_3479A:
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG5UW)));
        RG5L = RG1L;
        continue;
      }
      //m_347AB:
      RG5L = vql_8c.peek32();
      //m_347AE:
      for (;;)
      {
        if (RG5W == lcon(0xfffffffe))
          break;
        RG1L = RG5L;
        RG1W &= lcon(0x3c00);
        RG1L = unsignedlong(RG1W) >> bcon(0xa);
        if (RG1L == lcon(0xf))
        {
          RG64L = unsignedlong(RG5W);
          RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG64L);
          RG4L = unsignedlong(RG1Blo);
          RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG64L) & lcon(0xff);
          RG1L = DM2_QUERY_4BPP_PICT_BUFF_AND_PAL(RG1L, RG4L, &xp_4c, palette2);
          if (RG1W != 0)
          {
            RG1L = signedlong(DM2_RANDDIR()) & lcon(0xffff);
            i32 parl23 = RG1L;
            RG3L = signedlong(argw1);
            RG2L = signedlong(argw0);
            RG1W = CUTX16(timdat.gametick) & 1;
            RG1L++;
            RG4L = signedlong(RG1W);
            DM2_DRAW_CHIP_OF_MAGIC_MAP(BMPCAST(xp_4c), RG4L, RG2L, RG3L, parl23, palette2);
          }
        }
        //m_3482F:
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG5UW)));
        RG5L = RG1L;
      }
      //m_34840:
      if (s15_10.warr_00[0x0] == 0)
      {
        RG2P = DOWNCAST(unk, xp_64);
        if (RG2P == NULL)
        {
          vw_78 = vw_08; // adj
          vw_70 = vw_04; // adj
          xp_50 = UPCAST(unk, RG2P);
          RG1W = ddat.v1d3248;
          //m_3486B:
          for (;;)
          {
            vo_74.poke16(RG1W);
            RG1W = vo_74.peek16();
            if (RG1W == ddat.v1d3248)
            {
              RG1L = signedlong(vo_74.peek16());
              if (RG1L != lcon(0xffffffff))
              {
                xparpp25 = UPCAST(unk*, ADRESSOF(unk*, &xp_50));
                RG1L = DM_LOCATE_OTHER_LEVEL(signedlong(ddat.v1d3248), 0, &vw_08, &vw_04, xparpp25);
                continue;
              }
            }
            break;
          }
          //m_348A1:
          RG65W = vo_74.peek16();
          if (RG65W >= 0 && RG65W != ddat.v1d3248)
          {
            RG1L = signedlong(vo_74.peek16());
            RG4P = DOWNCAST(t_tile*, mapdat.tmpmap) + 4 * RG1L;
            RG2L = signedlong(vw_08) << 2;
            RG4P = pointer_at(RG4P) + RG2L;
            RG2L = signedlong(vw_04);
            RG4P = pointer_at(RG4P);
            RG4Blo = byte_at(RG4P, RG2L);
            RG4L = unsignedlong(RG4Blo) >> 5;
            if (RG4Blo == 0)
            {
              xp_64 = XDOWNCAST(special, 1); // DANGER!!!
              vw_7c = ddat.v1d3248;
              DM2_CHANGE_CURRENT_MAP_TO(RG1L);
              RG3L = signedlong(vw_04);
              RG2L = signedlong(vw_08);
              RG4L = signedlong(vql_00.peek16());
              RG1P = ADRESSOF(s_8wordarray, &s15_10);
              DM2_SUMMARIZE_STONE_ROOM(UPCAST(s_8wordarray, RG1P), RG4L, RG2L, RG3L);
              break;
            }
          }
        }
      }
      //m_34924:
      if (DOWNCAST(unk, xp_64) != NULL)
      {
        vw_08 = vw_78;
        vw_04 = vw_70;
        DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_7c));
        DM2_SUMMARIZE_STONE_ROOM(&s15_10, signedlong(vql_00.peek16()), signedlong(vw_08), signedlong(vw_04));
      }
      //m_3495D:
      RG1L = DM_LOCATE_OTHER_LEVEL(signedlong(ddat.v1d3248), lcon(0xffffffff), &vw_08, &vw_04, NULL);
      if (RG1W < 0)
        return;
      RG1P = DOWNCAST(t_tile*, mapdat.tmpmap) + 4 * signedlong(RG1W);
      RG4L = signedlong(vw_08) << 2;
      RG1L = long_at(RG1P);
      RG1P += RG4L;
      RG1P = pointer_at(RG1P) + signedlong(vw_04);
      RG4L = unsignedlong(byte_at(RG1P));
      vo_74.poke16(RG4W);
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
      parl27 = 0;
      RG3L = signedlong(argw1);
      RG2L = signedlong(argw0);
      RG4L = lcon(0x12);
      jump L_fin;
    }
  }
  FOREND

L_fin:
  DM2_DRAW_CHIP_OF_MAGIC_MAP(BMPCAST(xp_48), RG4L, RG2L, RG3L, parl27, palette1);
}

void DM2_guidraw_29ee_1d03(i8 eaxb)
{
  c_nreg _RG1; RG1L = unsignedlong(eaxb);
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5;
  t_text tarr_00[0x4]; // TODO probably even just tarr_00[0x2]
  c_o32 vo_04;
  i16 parw00;
  t_text* par_tp02;

  RG5L = RG1L;
  RG1L = 0;
  DM2_guidraw_29ee_00a3(RG1L);
  if (party.curacthero <= wcon(0x0))
    return;
  RG1W = ddat.v1e0b62 & lcon(0x800);
  RG1L = unsignedlong(RG1W);
  if (RG1L == 0)
    return;
  RG5W ^= ddat.v1e0b62;
  RG5L &= lcon(0xf);
  RG1W = unsignedword(RG1Blo);
  tarr_00[0x1] = RG1Bhi;
  RG3L = 0;
  vo_04.poke16(RG3W);
  //m_3505F:
  for (;;)
  {
    RG1L = signedlong(vo_04.peek16());
    if (RG1L >= lcon(0x4))
      return;
    //m_34FE4:
    RG1Blo = lcon(0x49);
    RG4L = RG5L;
    RG4W &= 0x1;
    RG4L = unsignedlong(RG4W);
    if (RG4L != 0)
      RG1Blo = lcon(0x4a);
    RG5UW >>= bcon(0x1);
    RG4W = signedlong(vo_04.peek16() + lcon(0x65));
    put16(parw00, RG4W);
    RG2L = unsignedlong(RG1Blo);
    DM2_DRAW_ICON_PICT_ENTRY(20, ddat.v1e0ba4, RG2Blo, &dm2_buttongroup2, parw00);
    RG1Blo = vo_04.peek8() + bcon(0x72);
    tarr_00[0x0] = RG1Blo;
    par_tp02 = tarr_00;
    RG4W = unsignedword(RG4Blo);
    RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL02]);
    RG4Bhi |= lcon(0x40);
    RG3L = unsignedlong(RG4W);
    RG2L = unsignedlong(palettecolor_to_ui8(paldat.palette[E_COL00]));
    RG1W = vo_04.peek16() + lcon(0x69);
    RG4L = signedlong(RG1W);
    DM2_DRAW_BUTTON_STR(&dm2_buttongroup2, RG4W, RG2W, RG3W, par_tp02);
    vo_04.inc16();
  }
  FOREND
}

// stacksize was 0x74
void DM2_guidraw_29ee_1946(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1, i32 argl2, i32 argl3)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG51;
  i32 RG53l;
  c_wlreg _RG61;
  c_wlreg _RG62;
  c_ql32 vql_00;
  c_ql32 vql_04;
  c_ql32 vql_08;
  i16 vw_0c;
  c_5bytes c12_10;
  i16 vw_18;
  i16 vw_1c;
  i16 vw_20;
  i16 vw_24;
  i32 vl_28;
  i32 vl_2c;
  i16 vw_30;
  i16 vw_34;
  i16 vw_38;
  i16 vw_3c;
  c_o32 vo_40; // poke 16 peek 32 add16
  c_o32 vo_44; // poke 16 peek 32 dec16
  i16 vw_48;
  i16 vw_4c;
  c_o32 vo_50; // poke 16 peek 32 add16
  i16 vw_54;
  i16 vw_58;
  i16 vw_5c;
  i16 vw_60;
  i8 vb_64;
  i16 parw02;
  i16 parw03;
  i8 parb04;

  vw_0c = RG1W; // TODO: incoming RG1W word only
  vql_08.poke32(RG4L);
  vql_04.poke32(RG2L);
  vql_00.poke32(RG3L);
  RG61L = argl1;
  RG51L = argl2;
  RG1L = argl3;
  RG1W &= lcon(0x8);
  RG1L = unsignedlong(RG1W);
  if (RG1L == 0)
  {
    //m_34AD5:
    RG1Blo = vql_08.peek8();
    ddat.v1e0b82 = RG1Blo;
    RG1Blo = vql_04.peek8();
    ddat.v1e0b83 = RG1Blo;
    RG1Blo = vql_00.peek8();
    ddat.v1e0b81 = RG1Blo;
    RG1Blo = mkb(argw0);
    ddat.v1e0b84 = RG1Blo;
    ddat.v1e0b5a = RG61W;
    ddat.v1e0b58 = RG51W;
  }
  else
  {
    vql_08.poke16(signedword(ddat.v1e0b82));
    vql_04.poke16(signedword(ddat.v1e0b83));
    vql_00.poke16(signedword(ddat.v1e0b81));
    argw0 = signedword(ddat.v1e0b84);
    RG61W = ddat.v1e0b5a;
    RG51W = ddat.v1e0b58;
  }
  //m_34B06:
  RG4L = 0;
  ddat.v1e0b56 = RG4W;
  vw_60 = ddat.v1d3248;
  DM2_guidraw_29ee_00a3(0);
  RG2UBlo = palettecolor_to_ui8(paldat.palette[E_COL00]);
  RG2L = unsignedlong(RG2Blo);
  RG4R = &glblrects.dm2rect3;
  RG1R = DM2_QUERY_EXPANDED_RECT(99, RG4R);
  RG4R = RG1R;
  RG1P = DOWNCAST(c_buttongroup, &dm2_buttongroup2);
  DM2_FILL_RECT_SUMMARY(UPCAST(c_buttongroup, RG1P), /*TAG BLITRECT*/ RG4R, ui8_to_pixel(RG2UBlo));
  RG1W = ddat.v1d2750; // TODO: gets extended w/o conversion now
  RG1L = -RG1L;
  RG2L = signedlong(RG1W);
  RG1W = ddat.v1d274e; // TODO: gets extended w/o conversion now
  RG1L = -RG1L;
  RG4L = signedlong(RG1W);
  RG1R = &glblrects.dm2rect3;
  RG1R->inflate(RG4W, RG2W);
  RG1W = glblrects.dm2rect3.x;
  RG1L += RG61L;
  vl_28 = RG1L;
  RG1W = glblrects.dm2rect3.y;
  RG1L += RG51L;
  vo_40.poke16(RG1W);
  RG1L = argl3;
  RG1W &= lcon(0x10);
  vw_5c = RG1W;
  RG1L = argl3;
  RG1W &= lcon(0x20);
  vw_34 = RG1W;
  RG1L = argl3;
  RG1W &= lcon(0x40);
  vw_30 = RG1W;
  if (vw_34 != 0 || RG1W != 0)
  {
    RG4L = unsignedlong(vw_0c);
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
    RG3P = RG1P;
    if (vw_34 != 0)
    {
      RG1P = DM2_GET_MISSILE_REF_OF_MINION(unsignedlong(word_at(RG1P, 2)), RG4L);
      RG4W = word_at(RG1P, lcon(0x4)) & lcon(0x1f);
      vw_20 = RG4W;
      RG4W = word_at(RG1P, lcon(0x4));
      RG4L <<= bcon(0x6);
      _RG4.ushiftr11();
      vw_3c = RG4W;
      RG1W = word_at(RG1P, lcon(0x4));
      RG1UW >>= bcon(0xa);
      vw_38 = RG1W;
      DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_38));
      RG2L = signedlong(vw_3c);
      RG4L = signedlong(vw_20);
      RG1L = DM2_GET_TELEPORTER_DETAIL(&c12_10, RG4L, RG2L);
      if (RG1L == 0)
        //m_34C5D:
        vw_1c = -1;
      else
      {
        RG1L = unsignedlong(c12_10.b_02);
        vw_54 = RG1W; // truncated
        RG1L = unsignedlong(c12_10.b_03);
        vw_58 = RG1W; // truncated
        RG1L = unsignedlong(c12_10.b_04);
        vw_1c = RG1W; // truncated
      }
    }
    //m_34C65:
    if (vw_30 != 0)
    {
      RG1W = word_at(RG3P, lcon(0x6));
      RG4L = RG1L;
      RG4UW >>= bcon(0xa);
      RG2L = signedlong(RG4W);
      RG4L = RG1L << 6;
      _RG4.ushiftr11();
      RG4L = signedlong(RG4W);
      RG1W &= lcon(0x1f);
      RG1L = signedlong(RG1W);
      DM2_query_29ee_18eb(RG1L, RG4L, RG2L);
    }
  }
  //m_34C90:
  DM2_CHANGE_CURRENT_MAP_TO(signedlong(vql_00.peek16()));
  RG1L = 3;
  vw_48 = wcon(0xfffd);
  if (RG51W < 0)
    RG1L = 4;
  vl_2c = lcon(0xfffffffd);
  vw_4c = 3;
  if (RG61W < 0)
    vl_2c = lcon(0xfffffffc);
  vo_44.poke16(RG1W);
  //m_34F71:
  for (;;)
  {
    RG1W = vo_44.peek16();
    if (RG1W < vw_48)
    {
      DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_60));
      return;
    }
    //m_34CDA:
    RG1L = vl_28;
    vo_50.poke16(RG1W);
    RG62L = vl_2c;
    //m_34F4F:
    for (;;)
    {
      if (RG62W > vw_4c)
      {
        RG1W = ddat.v1d274c + ddat.v1d2750;
        vo_40.add16(RG1W);
        vo_44.dec16();
        break;
      }
      bool skip00760 = false;
      //m_34CEC:
      RG1L = vql_08.peek32();
      vw_18 = RG1W;
      RG1L = vql_04.peek32();
      vw_24 = RG1W;
      RG2L = signedlong(RG62W);
      DM2_CALC_VECTOR_W_DIR(argw0, vo_44.peek16(), RG2W, &vw_18, &vw_24);
      RG53l = signedlong(vql_00.peek16());
      RG1L = DM_LOCATE_OTHER_LEVEL(RG53l, 0, &vw_18, &vw_24, NULL);
      RG3L = RG1L;
      if (RG1W >= 0)
      {
        //m_34D8A:
        RG1L = signedlong(RG1W);
        DM2_CHANGE_CURRENT_MAP_TO(RG1L);
        skip00760 = true;
      }
      else
      {
        RG3L = vql_00.peek32();
        RG1L = RG53l;
        DM2_CHANGE_CURRENT_MAP_TO(RG1L);
        RG1L = (dm2_GET_TILE_VALUE(signedlong(vw_18), signedlong(vw_24)) & lcon(0xff)) >> 5 & lcon(0xff);
        if (RG1L != lcon(0x7))
          skip00760 = true;
        else
        {
          RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
          RG1W = word_at(RG1P, lcon(0x8)) & lcon(0x3f);
          RG1L = unsignedlong(RG1W);
          RG1Blo = ddat.v1e03d4[RG1L];
        }
      }

      if (skip00760)
      {
        //m_34D90:
        RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
        RG1W = word_at(RG1P, lcon(0xe));
        RG1L <<= bcon(0x8);
        RG1UW >>= bcon(0xc);
      }
      //m_34DA0:
      vb_64 = RG1Blo;
      argl3 &= lcon(0xffffff8f);
      if (vw_34 != 0)
      {
        bool skip00762 = false;
        bool skip00763 = false;
        RG1W = vw_18; // adj
        if (RG1W != vw_20)
          skip00762 = true;
        else
        {
          RG1W = vw_24; // adj
          if (RG1W != vw_3c || RG3W != vw_38)
            skip00762 = true;
        }

        if (skip00762)
        {
          //m_34DD1:
          if (RG3W == vw_1c)
          {
            RG1W = vw_18; // adj
            if (RG1W == vw_54)
            {
              RG1W = vw_24; // adj
              if (RG1W == vw_58)
                skip00763 = true;
            }
          }
        }
        else
          skip00763 = true;

        if (skip00763)
          //m_34DEE:
          argl3 |= lcon(0x20);
      }

      //m_34DF6:
      if (vw_30 != 0 && ddat.v1e0b56 == 0)
      {
        bool skip00765 = false;
        RG1W = vw_18; // adj
        if (RG1W != ddat.v1e0b6e)
          skip00765 = true;
        else
        {
          RG1W = vw_24; // adj
          if (RG1W != ddat.v1e0b70 || RG3W != ddat.v1e0b64)
            skip00765 = true;
          else
            ddat.v1e0b56 = 1;
        }

        if (skip00765)
        {
          bool skip00766 = false;
          bool skip00767 = false;
          //m_34E41:
          RG1W = vw_18; // adj
          if (RG1W != ddat.v1e0b68)
            skip00766 = true;
          else
          {
            RG1W = vw_24; // adj
            if (RG1W != ddat.v1e0b6a || RG3W != ddat.v1e0b60)
              skip00766 = true;
            else
              ddat.v1e0b56 = 2;
          }

          if (skip00766)
          {
            //m_34E6F:
            RG1W = vw_18; // adj
            if (RG1W == ddat.v1e0b5e)
            {
              RG1W = vw_24; // adj
              if (RG1W == ddat.v1e0b5c && RG3W == ddat.v1e0b66)
              {
                ddat.v1e0b56 = 3;
                skip00767 = true;
              }
            }
          }
          else
            skip00767 = true;

          if (skip00767)
          {
            //m_34E9B:
            RG2L = signedlong(RG3W);
            DM2_query_29ee_18eb(signedlong(vw_18), signedlong(vw_24), RG2L);
          }
        }

        //m_34EB1:
        if (ddat.v1e0b56 != 0)
          argl3 |= lcon(0x40);
      }

      //m_34EC3:
      if (vw_5c != 0)
      {
        if (RG3W == ddat.v1e0282)
        {
          RG1W = vw_18; // adj
          if (RG1W == ddat.v1e0280)
          {
            RG1W = vw_24; // adj
            if (RG1W == ddat.v1e027e)
              argl3 |= lcon(0x10);
          }
        }
      }
      i32 parl05 = unsignedlong(mkuw(argl3));
      RG1L = unsignedlong(vb_64);
      put8(parb04, RG1Blo);
      RG1L = signedlong(vo_40.peek16());
      put16(parw03, RG1W);
      RG1L = signedlong(vo_50.peek16());
      put16(parw02, RG1W);
      RG3L = signedlong(argw0);
      RG2L = signedlong(vw_24);
      RG4L = signedlong(vw_18);
      DM2_DRAW_MAP_CHIP(unsignedlong(vw_0c), RG4L, RG2L, RG3L, parw02, parw03, parb04, parl05);
      RG1W = ddat.v1d274a + ddat.v1d274e;
      vo_50.add16(RG1W);
      RG62L++;
    }
  }
  FOREND
}

void DM2_DRAW_MAJIC_MAP(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG51p;
  i32 RG52l;
  i8* RG6p;
  i16 vw_00;
  c_rect rc_04;
  i16 vw_0c;
  i16 vw_10;
  c_o32 vo_14; // poke 16 peek 32 add16
  i16 vw_18;
  i16 vw_1c;
  i16 parw02;

  vw_00 = RG1W; // TODO: incoming RG1W word only
  RG4L = unsignedlong(RG1W);
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L);
  ddat.v1e0ba4 = RG1Blo;
  ddat.v1e0b62 |= lcon(0x90); // FFIX
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
  RG51p = RG1P;
  RG2W = word_at(RG1P, lcon(0x4));
  RG2UW >>= bcon(0xd);
  vw_1c = RG2W;
  RG2L = signedlong(vw_1c);
  if (RG2L != lcon(0x3))
  {
    ddat.v1e0b62 |= lcon(0x800);
    RG2W = word_at(RG1P, lcon(0x4));
    RG2UW >>= bcon(0xd);
    vw_1c = RG2W;
    if (RG2W != 0)
    {
      RG1P = DM2_GET_MISSILE_REF_OF_MINION(unsignedlong(word_at(RG1P, 2)), RG4L);
      RG6p = RG1P;
      if (RG1P == NULL)
      {
        //m_350F0:
        RG1W = ddat.v1e0b62 & lcon(0x20);
        RG1L = unsignedlong(RG1W);
        if (RG1L != 0)
          ddat.v1e0b62 &= lcon(0xfffffbdf);
      }
      else
        ddat.v1e0b62 |= lcon(0x20);
    }
  }
  //m_3510A:
  RG1W = ddat.v1e0b62 & lcon(0x400);
  RG1L = unsignedlong(RG1W);
  if (RG1L == 0)
  {
    RG4L = unsignedlong(ddat.v1e0ba4);
    RG3P = DOWNCAST(c_buttongroup, &dm2_buttongroup2);
    DM2_DRAW_ICON_PICT_ENTRY(20, RG4Blo, 16, UPCAST(c_buttongroup, RG3P), lcon(0x5c));
    RG2L = 0;
    //m_3514D:
    for (;;)
    {
      RG4L = signedlong(RG2W);
      RG1L = unsignedlong(ddat.v1e0b7a);
      if (RG4L >= RG1L)
      {
        DM2_guidraw_29ee_1d03(0);
        ddat.v1e0b62 |= lcon(0x400);
        break;
      }
      //m_35140:
      RG1L = unsignedlong(RG2W);
      DM2_DRAW_CMD_SLOT(RG1W, 0);
      RG2L++;
    }
  }
  //m_3516A:
  RG3W = ddat.v1e0270;
  vo_14.poke16(ddat.v1e0272);
  vw_0c = ddat.v1e0266;
  RG1L = 0;
  vw_18 = RG1W;
  vw_10 = RG1W;
  if (vw_1c != 0)
  {
    RG1L = signedlong(vw_1c);
    if (RG1L != lcon(0x3))
    {
      RG1W = word_at(RG51p, lcon(0x6)) & lcon(0xfc00);
      if (RG1W != lcon(0xfffffc00))
        ddat.v1e0b62 |= lcon(0x40);
      RG1L = signedlong(vw_1c);
      if (RG1L != 1)
      {
        //m_3533E:
        RG1W = ddat.v1e0b62 & lcon(0x40);
        RG1L = unsignedlong(RG1W);
        if (RG1L != 0)
        {
          RG3W = word_at(RG51p, lcon(0x6)) & lcon(0x1f);
          RG1W = word_at(RG51p, lcon(0x6));
          RG1L <<= bcon(0x6);
          _RG1.ushiftr11();
          vo_14.poke16(RG1W);
          RG1W = word_at(RG51p, lcon(0x6));
          RG1UW >>= bcon(0xa);
          vw_0c = RG1W;
        }
      }
      else
      {
        if (RG6p != NULL)
        {
          //m_351E5:
          RG3W = word_at(RG6p, lcon(0x4)) & lcon(0x1f);
          RG1W = word_at(RG6p, lcon(0x4));
          RG1L <<= bcon(0x6);
          _RG1.ushiftr11();
          vo_14.poke16(RG1W);
          RG1W = word_at(RG6p, lcon(0x4));
          RG1UW >>= bcon(0xa);
          vw_0c = RG1W;
          RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(word_at(RG51p, 2))));
          RG4W = word_at(RG1P, lcon(0xe));
          RG4UW = lrshift6e(RG4UW);
          RG4L = unsignedlong(RG4W);
          RG4Blo = table1d69a2[RG4L];
          RG4W = unsignedword(RG4Blo) + ddat.v1e0258 & uwcon(0x3);
          RG4L = signedlong(RG4W);
          RG1L = DM2_QUERY_CREATURE_5x5_POS(RG1P, RG4L);
          RG52l = signedlong(RG1W);
          RG2L = 5;
          RG1L = RG52l;
          RG4L = RG1L % RG2L;
          RG1L /= RG2L;
          vw_10 = RG4W;
          RG1L = RG52l;
          RG4L = RG1L % RG2L;
          RG1L /= RG2L;
          vw_18 = RG1W;
          RG1L = signedlong(vw_10);
          if (RG1L > 2)
          {
            RG2W = ddat.v1e0258;
            RG2L++;
            RG2W &= 0x3;
            RG1L = signedlong(RG2W);
            RG3W += table1d27fc[RG1L];
            RG1W = table1d2804[RG1L];
            vo_14.add16(RG1W);
          }
          RG1L = signedlong(vw_18);
          if (RG1L > 2)
          {
            RG2W = ddat.v1e0258;
            RG2L += 2;
            RG2W &= 0x3;
            RG1L = signedlong(RG2W);
            RG3W += table1d27fc[RG1L];
            RG1W = table1d2804[RG1L];
            vo_14.add16(RG1W);
          }
          RG4L = 2 * signedlong(ddat.v1d274e) + signedlong(ddat.v1d274a) + 1;
          RG2L = 5;
          RG1L = RG4L;
          RG4L = RG1L % RG2L;
          RG1L /= RG2L;
          RG1L *= signedlong(table1d6998[signedlong(vw_10)]);
          vw_10 = RG1W;
          RG1L = 2 * signedlong(ddat.v1d2750);
          RG4L = signedlong(ddat.v1d274c) + RG1L + 1;
          RG1L = RG4L;
          RG4L = RG1L % RG2L;
          RG1L /= RG2L;
          RG1L *= signedlong(table1d6998[signedlong(vw_18)]);
          vw_18 = RG1W;
        }
        else
          ddat.v1e0b62 &= lcon(0xffffffbf);
      }
    }
  }
  //m_35375:
  i32 parl05 = unsignedlong(ddat.v1e0b62);
  i32 parl04 = signedlong(vw_18);
  i32 parl03 = signedlong(vw_10);
  RG1L = signedlong(ddat.v1e0258);
  put16(parw02, RG1W);
  RG1L = signedlong(vw_0c);
  RG2L = signedlong(vo_14.peek16());
  RG4L = signedlong(RG3W);
  RG3L = RG1L;
  RG1L = unsignedlong(vw_00);
  DM2_guidraw_29ee_1946(RG1L, RG4L, RG2L, RG3L, parw02, parl03, parl04, parl05);
  RG1W = ddat.v1e0b62 & lcon(0x100);
  RG1L = unsignedlong(RG1W);
  if (RG1L == 0)
  {
    RG1W = ddat.v1e0b62 & lcon(0x200);
    RG1L = unsignedlong(RG1W);
    if (RG1L != 0)
      return;
  }
  RG4R = DM2_QUERY_EXPANDED_RECT(99, &rc_04);
  RG2L = 0;
  DM2_guidraw_0b36_105b(&dm2_buttongroup2, RG4R, RG2W);
}

// was SKW_3929_0b5a
void DM2_DISPLAY_HINT_NEW_LINE(void)
{
  DM2_DISPLAY_HINT_TEXT(0, ddat.v1d70e8);
}

// was SKW_2405_00c9
void DM2_DISPLAY_TAKEN_ITEM_NAME(i16 wn)
{
  DM2_DISPLAY_HINT_NEW_LINE();
  DM2_DISPLAY_HINT_TEXT(lcon(0xd), DM2_GET_ITEM_NAME(wn));
}

// TODO: includes mouse, maybe move to C_GFX_MAIN
void DM2_HIGHLIGHT_ARROW_PANEL(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_buttongroup buttongroup;
  i16 parw00;

  RG3Blo = RG1Blo;
  ddat.v1e0548 = RG1Blo;
  ddat.v1e053e = RG4W;
  ddat.v1e0534 = RG2L;
  if (RG2L != 0)
    RG3Blo++;
  DM2_HIDE_MOUSE();
  RG5L = signedlong(RG4W);
  DM2_guidraw_0b36_0c52(&buttongroup, RG5L, 1);
  DM2_FILL_ENTIRE_PICT(dm2_dballochandler.DM2_GET_BMP(buttongroup.button.dbidx), palettecolor_to_pixel(paldat.palette[E_COL00]));
  put16(parw00, RG5W);
  RG2L = unsignedlong(RG3Blo);
  DM2_DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 3, RG2Blo, &buttongroup, parw00);
  DM2_gfxmain_0b36_0cbe(&buttongroup, true);
  DM2_SHOW_MOUSE();
  DM2_WAIT_SCREEN_REFRESH();
}

// TODO: UPDATE-function!
// was SKW_2759_0541
void DM2_DISPLAY_RIGHT_PANEL_SQUAD_HANDS(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  bool skip00686 = false;

  RG1W = party.curacthero;
  if (RG1W == 0)
    return;
  if (ddat.v1e0b7c != 0)
  {
    RG1L = unsignedlong(ddat.v1e0b6c);
    if (RG1L > 1)
      skip00686 = true;
  }

  if (!skip00686)
  {
    //m_2FDEC:
    RG2L = 0;
    ddat.v1e0b6c = RG2W;
  }

  //m_2FDF5:
  RG3L = 0;
  party.curacthero = RG3W;
  ddat.v1d67bc = RG3W;
  ddat.v1e0b62 = RG3W;
  if (ddat.v1d6700 != lcon(0xffffffff))
    RG1L = DM2_PUT_OBJECT_INTO_CONTAINER(RG1L);
  ddat.v1e0b30 = 1;
}

// belongs to DM2_REFRESH_PLAYER_STAT_DISP
static void DM2_24a5_1532(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG51;
  i32 RG52l;
  i32 RG53l;
  t_text tarr_00[0x80];
  i16 vw_80; // x0 rectB
  i16 vw_84; // y0 rectB, y0 rectA
  i16 vw_88;
  i16 vw_8c; // x0 rectA
  c_o32 vo_90; // poke 16 peek 8 16 inc16
  i16 vw_94;
  i16 vw_98;

  RG1W = ddat.v1e0976;
  RG1L--;
  vw_98 = RG1W;
  RG4L = unsignedlong(RG1W);
  c_hero* hero = &party.hero[RG4L];
  ddat.v1d66fc = 2;
  RG3L = lcon(0x1ee);
  DM2_DRAW_STATIC_PIC(7, 0, 1, RG3W, NOALPHA);
  DM2_QUERY_TOPLEFT_OF_RECT(lcon(0x22d), OO vw_8c, OO vw_84);
  RG2L = 0;
  vo_90.poke16(RG2W);
  //m_2A31D:
  for (;;)
  {
    RG51L = unsignedlong(vo_90.peek16());
    if (RG51L > lcon(0x3))
    {
      DM2_QUERY_TOPLEFT_OF_RECT(lcon(0x22f), OO vw_80, OO vw_84);
      vo_90.poke16(1);
      break;
    }
    //m_2A256:
    RG1W = DM2_QUERY_PLAYER_SKILL_LV(vw_98, RG51W, 1);
    RG4L = signedlong(RG1W);
    RG1L = signedlong(DM2_MIN(16, RG4W));
    if (RG1L != 1)
    {
      ddat.v1e0994 = RG51W;
      DM2_QUERY_GDAT_TEXT(bcon(0x7), bcon(0x0), table1d6712[RG1L + 4], tarr_00); // index shifted, was 0x1d6716
      RG3P = ADRESSOF(t_text, tarr_00);
      RG1L = unsignedlong(vw_98);
      if (ddat.v1e0ffa[4 * RG1L + RG51L + 2] == 0)
        //m_2A2BD:
        RG1L = lcon(0xd);
      else
        RG1L = lcon(0x7);
      //m_2A2C2:
      DM2_DRAW_VP_STR(vw_8c, vw_84, unsignedword(palettecolor_to_ui8(paldat.palette[RG1L])), RG3T);
      RG1L = unsignedlong(vw_98);
      RG53l = unsignedlong(vo_90.peek16());
      RG1Blo = ddat.v1e0ffa[4 * RG1L + RG53l + 2]; // EVIL offset shifted by 2, was 0x1e0ffc
      ddat.v1e096c[RG53l] = RG1Blo;
      vw_84 += strdat.strxplus;
    }
    //m_2A315:
    vo_90.inc16();
  }
  //m_2A49C:
  for (;;)
  {
    RG52l = unsignedlong(vo_90.peek16());
    if (RG52l > lcon(0x6))
      return;
    //m_2A357:
    RG1Blo = vo_90.peek8() + bcon(0x20);
    RG2L = unsignedlong(RG1Blo);
    RG3P = ADRESSOF(t_text, tarr_00);
    RG4L = 0;
    RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x7), bcon(0x0), RG2Blo, RG3T);
    DM2_DRAW_VP_STR(vw_8c, vw_84, unsignedword(palettecolor_to_ui8(paldat.palette[13])), RG1T);
    RG2L = 0;
    RG4L = RG52l;
    RG1W = hero->get_adj_ability1((e_ability)RG4W, (e_curmax)RG2W);
    RG3W = RG1W;
    vw_94 = RG1W;
    RG2L = 1;
    RG4L = RG52l;
    RG1W = hero->get_adj_ability1((e_ability)RG4W, (e_curmax)RG2W);
    vw_88 = RG1W;
    RG4L = signedlong(RG3W);
    RG1L = unsignedlong(RG1W);
    if (RG4L >= RG1L)
    {
      //m_2A3D6:
      if (RG4L <= RG1L)
        //m_2A3DF:
        RG3L = lcon(0xd);
      else
        RG3L = lcon(0x7);
    }
    else
      RG3L = lcon(0x8);
    //m_2A3E4:
    RG1T = DM2_FMT_NUM(vw_94, 1, 3);
    DM2_DRAW_VP_STR(vw_80, vw_84, unsignedword(palettecolor_to_ui8(paldat.palette[RG3W])), RG1T);
    DM2_STRCPY(tarr_00, CHGCAST(t_text, v1d10f2));
    RG1T = DM2_STRCAT(tarr_00, DM2_FMT_NUM(vw_88, 1, 3));
    RG2UBlo = palettecolor_to_ui8(paldat.palette[E_COL13]);
    RG2L = unsignedlong(RG2Blo);
    RG4L = signedlong(vw_84);
    RG1L = signedlong(mkw(strdat.gfxstrw4 * wcon(0x3) + vw_80));
    DM2_DRAW_VP_STR(RG1W, RG4W, RG2W, tarr_00);
    vw_84 += strdat.strxplus;
    vo_90.inc16();
  }
  FOREND
}

// belongs to DM2_REFRESH_PLAYER_STAT_DISP
static void DM2_2e62_061d(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  c_rect rc_00;
  i16 parw00;

  RG4L = RG1L + lcon(0xad);
  RG5w = RG4W;
  put16(parw00, RG5w);
  RG4L = unsignedlong(RG1W);
  RG1L = 263 * RG4L;
  DM2_DRAW_ICON_PICT_ENTRY(22, party.hero[RG4L].herotype, 0, &dm2_buttongroup1, parw00);
  if (ddat.savegames1.b_02 != 0)
  {
    RG4R = &rc_00;
    RG1R = DM2_QUERY_EXPANDED_RECT(RG5w, RG4R);
    RG2L = lcon(0xc);
    RG4R = RG1R;
    DM2_guidraw_0b36_105b(&dm2_buttongroup1, RG4R, RG2W);
  }
}

void DM2_REFRESH_PLAYER_STAT_DISP(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG61;
  c_wlreg _RG62;
  t_text tarr_00[0x80];
  c_rect rc_80;
  i32 vl_88;
  i32 vl_8c;
  t_text* tp_90;
  t_bmp* bmp_94;
  i32 vl_98;
  i32 vl_9c;
  c_ql32 vql_a0;
  c_ql32 vql_a4;
  c_ql32 vql_a8;
  c_o32 vo_ac; // poke 16 poke 32 or16
  t_text* par_tp01;
  i16 parw02;
  t_text* par_tp07;
  i16 parw08;
  bool skip00787 = false;

  RG61L = RG1L;
  RG3L = 0;
  RG4L = 0;
  vl_88 = RG4L;
  RG2L = 0;
  vl_9c = RG4L;
  vl_98 = RG4L;
  vl_8c = RG4L;
  RG2W = RG1W;
  c_hero* hero = &party.hero[RG2L];
  vo_ac.poke16(hero->heroflag);
  RG1L = signedlong(ddat.v1e0976);
  RG2L++;
  RG1Blo = RG2L == RG1L ? 1 : 0;
  RG1W = unsignedword(RG1Blo);
  vql_a4.poke32(RG1L);
  if (RG1W != 0)
  {
    if (gfxsys.backbuff2 != 0)
      vo_ac.or16(lcon(0x8000)); // modified
    if (ddat.v1d26a0 != 0 || ddat.v1d26a2 != 0)
      //m_389CA:
      ddat.v1e100c &= lcon(0xffffffef);
    else
      ddat.v1e100c |= lcon(0x10);
  }
  //m_389D1:
  RG4L = signedlong(ddat.v1e0976);
  RG1L = unsignedlong(ddat.v1e101e);
  if (RG4L != RG1L)
    skip00787 = true;
  else
  {
    RG1W = vo_ac.peek16();
    RG1W &= lcon(0x8000);
    RG1L = unsignedlong(RG1W);
    if (RG1L == 0)
    {
      //m_38A3C:
      if (vql_a4.peek16() != 0)
      {
        RG3W = ddat.v1e100c;
        RG3W ^= ddat.v1e101c;
      }
    }
    else
      skip00787 = true;
  }

  if (skip00787)
  {
    //m_389F5:
    RG1L = vql_a4.peek32();
    if (RG1W == 0)
      //m_38A34:
      ddat.v1e101e = RG1W;
    else
    {
      DM2_DRAW_STATIC_PIC(7, 0, 0, 4, NOALPHA);
      RG4L = 1;
      vl_8c = RG4L;
      vl_9c = RG4L;
      ddat.v1e101e = ddat.v1e0976;
      RG3L = lcon(0x1f);
      vo_ac.or16(lcon(0x7c00)); // modified
    }
  }
  //m_38A51:
  if (RG3W != 0)
  {
    RG1L = RG3L;
    RG1Bhi ^= RG3Bhi;
    RG1Blo &= lcon(0x10);
    RG1L = unsignedlong(RG1W);
    if (RG1L != 0)
      DM2_DRAW_CHARSHEET_OPTION_ICON(lcon(0x11), lcon(0x238), lcon(0x10));
    if (ddat.v1e0288 == 0)
    {
      RG1L = RG3L;
      RG1Bhi ^= RG3Bhi;
      RG1Blo &= 0x1;
      RG1L = unsignedlong(RG1W);
      if (RG1L != 0)
      {
        DM2_DRAW_CHARSHEET_OPTION_ICON(lcon(0x13), lcon(0x267), 1);
        vo_ac.or16(lcon(0x400)); // modified
      }
      RG1L = RG3L;
      RG1Bhi ^= RG3Bhi;
      RG1Blo &= lcon(0x8);
      RG1L = unsignedlong(RG1W);
      if (RG1L != 0)
        DM2_DRAW_CHARSHEET_OPTION_ICON(lcon(0xf), lcon(0x232), lcon(0x8));
      RG1L = RG3L;
      RG1Bhi ^= RG3Bhi;
      RG1Blo &= 0x4;
      RG1L = unsignedlong(RG1W);
      if (RG1L != 0)
        DM2_DRAW_CHARSHEET_OPTION_ICON(lcon(0xd), lcon(0x234), lcon(0x4));
    }
    RG3W &= 0x2;
    RG1L = unsignedlong(RG3W);
    if (RG1L != 0)
      DM2_DRAW_CHARSHEET_OPTION_ICON(lcon(0xb), lcon(0x236), 2);
    vl_9c = 1;
    ddat.v1e101c = ddat.v1e100c;
  }
  RG1W = vo_ac.peek16();
  RG1W &= lcon(0x4000);
  RG1L = unsignedlong(RG1W);
  if (RG1L != 0)
  {
    RG2L = unsignedlong(RG61W);
    DM2_DRAW_PLAYER_3STAT_PANE(RG2L, 1);
    vl_98 = 1;
    if (hero->curHP == 0)
    {
      //m_38B70:
      par_tp01 = hero->name1;
      RG4W = unsignedword(RG4Blo);
      RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL01]);
      RG4Bhi |= lcon(0x40);
      RG3L = unsignedlong(RG4W);
      RG2L = unsignedlong(palettecolor_to_ui8(paldat.palette[E_COL15]));
      RG1L = RG61L + lcon(0xa5);
      RG4L = signedlong(RG1W);
      DM2_DRAW_NAME_STR(&dm2_buttongroup1, RG4W, RG2W, RG3W, par_tp01);
    }
    else
    {
      if (vql_a4.peek16() == 0)
        //m_38B63:
        vo_ac.or16(lcon(0xc00)); // modified
      else
      {
        DM2_2e62_061d(RG2L);
        vo_ac.or16(lcon(0x800)); // modified
      }
      //m_38B67:
      vl_88 = 1;
    }
  }
  //m_38B9C:
  if (hero->curHP != 0)
  {
    RG1W = vo_ac.peek16();
    RG1W &= lcon(0x800);
    RG1L = unsignedlong(RG1W);
    if (RG1L != 0)
    {
      RG2L = unsignedlong(RG61W);
      DM2_DRAW_PLAYER_3STAT_PANE(RG2L, 0);
      vl_98 = 1;
      DM2_DRAW_PLAYER_3STAT_HEALTH_BAR(RG2L);
      RG1L = signedlong(hero->timeridx);
      if (RG1L != lcon(0xffffffff))
      {
        RG1L = RG2L;
        DM2_DRAW_PLAYER_DAMAGE(RG1L);
      }
      //m_38BE8:
      if (vql_a4.peek16() != 0)
      {
        RG4R = &rc_80;
        DM2_QUERY_EXPANDED_RECT(549, RG4R);
        t_palette* pal1 = DM2_QUERY_GDAT_IMAGE_LOCALPAL(7, 0, 0);
        RG1L = signedlong(rc_80.y);
        put16(parw02, RG1W);
        RG3L = signedlong(rc_80.x);
        bmp_94 = gfxsys.bitmapptr;
        RG2L = 0;
        RG4L = 0;
        DM2_DRAW_DIALOGUE_PICT(BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(7, RG4Blo, RG2Blo)), bmp_94, &rc_80, RG3W, parw02, lcon(0xffff), pal1);
        DM2_DRAW_PLAYER_3STAT_TEXT(hero);
        if (hero->food >= 0 && hero->water >= 0 && hero->poisoned == 0)
          //m_38C5E:
          RG4Bhi = 4;
        else
          RG4Bhi = 5;
        //m_38C60:
        RG1L = unsignedlong(RG4Bhi);
        RG4L = lcon(0x221);
        DM2_DRAW_EYE_MOUTH_COLORED_RECTANGLE(RG1Blo, RG4W);
        RG4Bhi = 4;
        RG3L = 1;
        //m_38C86:
        for (;;)
        {
          RG1L = signedlong(RG3W);
          if (RG1L <= lcon(0x6))
          {
            //m_38C77:
            RG4Blo = hero->ability[RG1L][E_CUR];
            if (RG4Blo >= hero->ability[RG1L][E_MAX])
            {
              //m_38C85:
              RG3L++;
              continue;
            }
            RG4Bhi = 5;
          }
          break;
        }
        //m_38C8E:
        RG1L = unsignedlong(RG4Bhi);
        DM2_DRAW_EYE_MOUTH_COLORED_RECTANGLE(RG1Blo, 0x222);
        vl_9c = 1;
      }
    }
    //m_38CA3:
    if (vql_a4.peek16() == 0)
    {
      //m_38E34:
      RG3L = 0;
      //m_38E4E:
      for (;;)
      {
        RG1L = signedlong(RG3W);
        if (RG1L > 1)
          break;
        //m_38E38:
        RG4L = unsignedlong(RG3W);
        RG1L = DM2_guidraw_2e62_03b5(unsignedlong(RG61W), RG4L, vl_88);
        vl_98 |= RG1L;
        RG3L++;
      }
    }
    else
    {
      bool skip00789 = false;
      if (!ddat.vcapture3)
      {
        //m_38CBF:
        if (!ddat.vcapture2)
        {
          //m_38CD1:
          RG1W = hero->item[1];
          skip00789 = true;
        }
        else
          vql_a8.poke32(lcon(0xffff));
      }
      else
      {
        RG1W = ddat.savegamewpc.w_00;
        skip00789 = true;
      }

      if (skip00789)
        //m_38CD8:
        vql_a8.poke32(RG1L);
      bool skip00794 = false;
      bool skip00791 = false;
      //m_38CDB:
      RG1W = ddat.v1d66fe;
      if (RG1W != vql_a8.peek16())
        skip00791 = true;
      else
      {
        RG1L = ddat.vcapture2 ? 1 : 0;
        if (RG1L != ddat.v1e1018)
          skip00791 = true;
        else
        {
          RG1L = ddat.vcapture3 ? 1 : 0;
          if (RG1L != ddat.v1e1014)
            skip00791 = true;
          else
          {
            RG1W = vo_ac.peek16();
            RG1W &= lcon(0x2000);
            RG1L = unsignedlong(RG1W);
            if (RG1L == 0)
            {
              //m_38DCA:
              RG4W = ddat.v1d66fe;
              RG1Blo = RG4W != wcon(0xffff) ? 1 : 0;
              RG1L = unsignedlong(RG1Blo);
              if (RG1L != 0)
              {
                RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG4W), 0);
                RG1W &= lcon(0x20);
                RG1L = unsignedlong(RG1W);
                if (RG1L != 0)
                {
                  DM2_DRAW_ITEM_SURVEY(unsignedlong(ddat.v1d66fe), ddat.vcapture3 ? 1 : 0);
                  skip00794 = true;
                }
              }
            }
            else
              skip00791 = true;
          }
        }
      }

      if (skip00791)
      {
        //m_38D14:
        DM2_DRAW_STATIC_PIC(7, 0, 37, wcon(0x221), NOALPHA);
        RG3L = lcon(0x222);
        RG1Blo = (ddat.vcapture3 ? 1 : 0) + lcon(0x20);
        RG2L = unsignedlong(RG1Blo);
        DM2_DRAW_STATIC_PIC(7, 0, RG2Blo, RG3W, NOALPHA);
        if (!ddat.vcapture3)
        {
          bool skip00792 = false;
          //m_38D79:
          if (ddat.v1e0288 == 0)
          {
            //m_38D95:
            RG1L = DM2_DRAW_ITEM_SURVEY(unsignedlong(vql_a8.peek16()), 0);
            if (RG1L == 0)
              skip00792 = true;
          }
          else
          {
            if (ddat.vcapture2)
              skip00792 = true;
            else
              DM2_DRAW_CRYOCELL_LEVER(0);
          }

          if (skip00792)
            //m_38DA6:
            DM2_DRAW_FOOD_WATER_POISON_PANEL();
        }
        else
        {
          RG3L = vql_a8.peek32();
          if (RG3W != lcon(0xffffffff))
          {
            //m_38D68:
            RG1L = unsignedlong(RG3W);
            DM2_DRAW_ITEM_SURVEY(RG1L, 1);
          }
          else
            DM2_24a5_1532();
        }
        //m_38DAB:
        RG1L = vql_a8.peek32();
        ddat.v1d66fe = RG1W;
        ddat.v1e1014 = ddat.vcapture3 ? 1 : 0;
        ddat.v1e1018 = ddat.vcapture2 ? 1 : 0;
        skip00794 = true;
      }

      if (skip00794)
        //m_38E09:
        vl_9c = 1;

      //m_38E10:
      RG3L = 0;
      //m_38E2A:
      for (;;)
      {
        RG1L = signedlong(RG3W);
        if (RG1L >= lcon(0x1e))
          break;
        //m_38E14:
        RG4L = unsignedlong(RG3W);
        RG1L = DM2_guidraw_2e62_03b5(unsignedlong(RG61W), RG4L, vl_8c);
        vl_9c |= RG1L;
        RG3L++;
      }
    }

    //m_38E56:
    RG1W = vo_ac.peek16();
    RG1W &= lcon(0x400);
    RG1L = unsignedlong(RG1W);
    if (RG1L != 0)
    {
      RG4L = unsignedlong(RG61W);
      RG1L = signedlong(eventqueue.event_heroidx);
      if (RG4L != RG1L || ddat.v1e0288 != 0)
        //m_38E8B:
        RG1L = lcon(0xf);
      else
        RG1L = lcon(0x9);
      //m_38E90:
      RG3L = unsignedlong(palettecolor_to_ui8(paldat.palette[RG1L]));
      if (vql_a4.peek16() == 0)
      {
        //m_38F21:
        RG1L = unsignedlong(RG61W);
        DM2_DRAW_PLAYER_3STAT_PANE(RG1L, 0);
        vl_98 = 1;
        par_tp07 = hero->name1;
        RG1Blo = palettecolor_to_ui8(paldat.palette[E_COL01]);
        RG1W = unsignedword(RG1Blo);
        RG1Bhi |= lcon(0x40);
        RG1L = unsignedlong(RG1W);
        RG2L = unsignedlong(RG3W);
        RG4L = signedlong(CUTX16(RG61L + lcon(0xa5)));
        DM2_DRAW_NAME_STR(&dm2_buttongroup1, RG4W, RG2W, RG1W, par_tp07);
      }
      else
      {
        DM2_STRCPY(tarr_00, hero->name1);
        RG1Blo = hero->name2[0];
        RG1L = unsignedlong(RG1Blo);
        if (RG1L != lcon(0x2c) && RG1L != lcon(0x3b) && RG1L != lcon(0x2d))
          DM2_STRCAT(tarr_00, ddat.v1d70e4);
        DM2_STRCAT(tarr_00, hero->name2);
        tp_90 = tarr_00;
        if (ddat.v1e0288 == 0)
          //m_38EFC:
          RG4L = lcon(0x4000);
        else
          RG4L = unsignedlong(palettecolor_to_ui8(paldat.palette[E_COL01]));
        //m_38F01:
        RG2L = unsignedlong(RG4W);
        RG4L = unsignedlong(RG3W);
        DM2_DRAW_LOCAL_TEXT(lcon(0x229), RG4L, RG2W, tp_90);
        vl_9c = 1;
      }
    }
    //m_38F63:
    if (vql_a4.peek16() != 0)
    {
      RG1W = vo_ac.peek16();
      RG1W &= lcon(0x1000);
      RG1L = unsignedlong(RG1W);
      if (RG1L != 0)
      {
        RG4R = &rc_80;
        DM2_QUERY_EXPANDED_RECT(554, RG4R);
        t_palette* pal2 = DM2_QUERY_GDAT_IMAGE_LOCALPAL(7, 0, 0);
        RG1L = signedlong(rc_80.y);
        put16(parw08, RG1W);
        RG3L = signedlong(rc_80.x);
        bmp_94 = gfxsys.bitmapptr;
        RG2L = 0;
        RG4L = 0;
        DM2_DRAW_DIALOGUE_PICT(BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(7, RG4Blo, RG2Blo)), bmp_94, &rc_80, RG3W, parw08, lcon(0xffff), pal2);
        RG1W = party.get_player_weight((e_hero)RG61W);
        RG4L = RG1L;
        vql_a0.poke32(RG1L);
        RG1W = hero->get_max_load();
        RG2L = signedlong(RG1W);
        RG1L = unsignedlong(RG4W);
        if (RG1L <= RG2L)
        {
          //m_38FF6:
          RG4L = 8 * RG1L;
          RG1L = 5 * RG2L;
          if (RG4L <= RG1L)
            //m_3900D:
            RG62L = lcon(0xd);
          else
            RG62L = lcon(0xb);
        }
        else
          RG62L = lcon(0x8);
        //m_39012:
        RG4L = unsignedlong(vql_a0.peek16());
        RG2L = lcon(0xa);
        RG1L = RG4L;
        RG4L = RG1L % RG2L;
        RG1L /= RG2L;
        ddat.v1e0998 = RG1W;
        RG3L = RG1L * lcon(0xa);
        RG1L = vql_a0.peek32() - RG3L;
        ddat.v1e0996 = RG1W;
        RG1W = hero->get_max_load();
        RG4L = unsignedlong(RG1W);
        RG1L = RG4L;
        RG4L = RG1L % RG2L;
        RG1L /= RG2L;
        ddat.v1e098a = RG1W;
        DM2_QUERY_GDAT_TEXT(bcon(0x7), bcon(0x0), bcon(0x2a), tarr_00);
        RG4W = unsignedword(RG4Blo);
        RG4Blo = palettecolor_to_ui8(paldat.palette[E_COL01]);
        RG4Bhi |= lcon(0x40);
        RG2L = unsignedlong(RG4W);
        RG4L = unsignedlong(RG62W);
        DM2_DRAW_LOCAL_TEXT(lcon(0x22b), unsignedlong(palettecolor_to_ui8(paldat.palette[RG4L])), RG2W, tarr_00);
        vl_9c = 1;
      }
    }
  }

  //m_39096:
  if (vl_98 != 0)
    DM2_gfxmain_0b36_0cbe(&dm2_buttongroup1, true);
  if (vl_9c != 0)
  {
    gfxsys.backbuff2 = 0;
    DM2_DRAWINGS_COMPLETED();
  }
  hero->heroflag &= 0x300;
}

// was SKW_29ee_000f
i32 DM2_guidraw_29ee_000f(void)
{
  c_buttongroup buttongroup;
  i8 iArrowButtonID;	// vb_34

  if (ddat.v1e03a8 == 0)
    //m_32BAC:
    iArrowButtonID = 2; // start with normal arrow panel; 2 = turn left
  else
    iArrowButtonID = 14;	// start with pull/push arrow panel; 14 = push towards left
  //m_32BB1:
  DM2_guidraw_0b36_0c52(&buttongroup, 9, 1);
  DM2_FILL_ENTIRE_PICT(dm2_dballochandler.DM2_GET_BMP(buttongroup.button.dbidx), palettecolor_to_pixel(paldat.palette[E_COL00]));
  //m_32C0A:
  for (i16 i = 40; i < 46; i++)
  {
    //m_32BE8:
    DM2_DRAW_ICON_PICT_ENTRY(GDAT_CATEGORY_INTERFACE_GENERAL, 3, iArrowButtonID, &buttongroup, i);	//arrow
    iArrowButtonID += 2;
  }
  DM2_gfxmain_0b36_0cbe(&buttongroup, true);
  if (ddat.v1e03a8 == 0)
    return DM2_1031_0541(5);
  return DM2_1031_0541(6);
}

void DM2_guidraw_24a5_1798(i16 eaxw)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5;
  bool skip00661 = false;
  bool flag = false;

  RG5W = eaxw;
  RG4L = 0;
  ddat.v1d66fc = RG4W;
  ddat.v1d66fe = lcon(0xffffffff);
  RG4L = signedlong(RG5W);
  if (RG4L < lcon(0x4))
  {
    RG1L = 263 * RG4L;
    if (party.hero[RG4L].curHP == 0)
      return;
  }
  RG4L = ddat.vcapture1 ? 1 : 0;
  if (RG4L != 0)
    return;
  if (ddat.v1e03a8 != 0)
    return;
  if (ddat.vcapture2)
    return;
  if (ddat.vcapture3)
    return;
  ddat.glbRefreshViewport = true;
  RG3W = ddat.v1e0976;
  RG1L = signedlong(RG5W);
  RG4W = RG3W;
  RG1L++;
  if (RG1L == RG4L)
    RG5L = 4;
  if (RG3W != 0)
  {
    RG1L = signedlong(RG5W);
    if (RG1L != lcon(0x4))
      skip00661 = true;
  }

  if (!skip00661)
  {
    //m_2A5B5:
    flag = true;
    DM2_HIDE_MOUSE();
  }

  //m_2A5BF:
  if (RG3W != 0)
  {
    RG1L = 0;
    ddat.v1e0976 = RG1W;
    RG1L = unsignedlong(RG3W);
    RG4L = RG1L - lcon(0x1);
    c_hero* hero = &party.hero[RG4L];
    if (hero->curHP != 0)
    {
      if (ddat.v1e0288 == 0)
      {
        hero->heroflag |= 0x4000;
        RG1L = RG3L - 1 & lcon(0xffff);
        DM2_REFRESH_PLAYER_STAT_DISP(RG1W);
      }
    }
    RG2L = 0;
    //m_2A626:
    for (;;)
    {
      RG1L = signedlong(RG2W);
      if (RG1L >= lcon(0x4))
      {
        if (ddat.v1e0238 != 0)
        {
          if (flag)
          {
            DM2_REFRESHMOUSE();
            DM2_SHOW_MOUSE();
          }
          DM2_107B0();
          return;
        }
        break;
      }
      //m_2A60F:
      RG4L = 4 * unsignedlong(RG3W) + RG1L;
      RG1Blo = ddat.v1e096c[RG1L];
      ddat.v1e0ffa[RG4L - 2] -= RG1Blo; // EVIL offset shifted by 2, was 1e0ff8
      RG2L++;
    }
    //m_2A64E:
    RG1L = signedlong(RG5W);
    if (RG1L == lcon(0x4))
    {
      if (ddat.v1d6727 != 0)
        DM2_guidraw_29ee_000f();
      if (flag)
        DM2_SHOW_MOUSE();
      DM2_INIT_BACKBUFF();
      DM2_107B0();
      return;
    }
  }
  //m_2A674:
  RG1L = RG5L + 1;
  ddat.v1e0976 = RG1W;
  if (RG3W == 0)
  {
    RG4L = 0;
    RG1L = lcon(0x9);
    DM2_FILL_HALFTONE_RECTI(9);
  }
  DM2_ZERO_MEMORY(ddat.v1e096c, lcon(0x4));
  RG4L = signedlong(RG5W);
  party.hero[RG4L].heroflag |= wcon(0xfc00);
  RG3L = 0;
  ddat.v1e100c = RG3W;
  RG1L = unsignedlong(RG5W);
  DM2_REFRESH_PLAYER_STAT_DISP(RG1W);
  if (flag)
    DM2_SHOW_MOUSE();
  DM2_1031_0541(8);
}

// belongs to DM2_UPDATE_RIGHT_PANEL
static void DM2_29ee_0396(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG52l;
  i32 RG53l;
  c_image image;
  t_palette palette[PAL16];
  c_rect rc_14c;
  i16 vw_154;
  i16 vw_158;
  c_o32 vo_15c; // poke 16 peek 32 inc16
  i16 vw_160;
  i16 parw04;

  DM2_guidraw_29ee_00a3(0);
  DM2_DRAW_ICON_PICT_ENTRY(8, ddat.v1d67d6, -11, &dm2_buttongroup2, lcon(0x2f));
  RG4L = signedlong(ddat.v1d272e);
  RG1L = signedlong(ddat.v1d272c);
  RG3L = 4;
  RG2L = 0;
  t_bmp* bmp = DM2_ALLOC_PICT_BUFF(RG1Blo, RG4W, BIGPOOL_LO, RG3Blo);
  RG4L = 0;
  vo_15c.poke16(RG4W);
  //m_331B9:
  for (;;)
  {
    RG1L = signedlong(vo_15c.peek16());
    RG4L = unsignedlong(party.heros_in_party);
    if (RG1L >= RG4L)
    {
      DM2_FREE_PICT_BUFF(bmp);
      if (ddat.v1e0238 != 0)
      {
        RG4R = &rc_14c;
        RG1R = DM2_QUERY_EXPANDED_RECT(47, RG4R);
        RG2L = 0;
        RG4R = RG1R;
        DM2_guidraw_0b36_105b(&dm2_buttongroup2, RG4R, RG2W);
      }
      return;
    }
    //m_32FEE:
    RG4W = unsignedword(RG4Blo);
    RG4Blo = party.hero[RG1L].partypos;
    RG4L += 4;
    RG2W = ddat.v1e0258;
    RG4L -= RG2L;
    RG2L = RG4L;
    RG2Bhi ^= RG4Bhi;
    RG2Blo &= 0x3;
    if (party.hero[RG1L].curHP != 0)
    {
      RG1L = unsignedlong(RG2W);
      RG4L = unsignedlong(ddat.v1e00b8);
      RG1L++;
      if (RG4L != RG1L)
      {
        vw_158 = wcon(0x0);
        vw_154 = wcon(0x0);
        RG2L += lcon(0x35);
        vw_160 = RG2W;
        RG2L = signedlong(vw_160);
        RG3P16 = &vw_154;
        RG4R = &rc_14c;
        RG1R = DM2_QUERY_BLIT_RECT(bmp, RG4R, RG2W, IO vw_154, IO vw_158);
        if (RG1R != NULL)
        {
          if (ddat.savegames1.b_04 != 0)
          {
            RG1L = signedlong(DM2_RANDDIR());
            if (RG1W != 0)
            {
              RG1L -= 2;
              rc_14c.y += RG1W;
              RG1L = signedlong(DM2_RANDDIR());
              if (RG1W != 0)
              {
                RG1L -= 2;
                rc_14c.x += RG1W;
              }
            }
          }
          RG1L = signedlong(vw_158);
          put16(parw04, RG1W);
          RG3L = signedlong(vw_154);
          RG53l = signedlong(vo_15c.peek16());
          DM2_QUERY_GDAT_SQUAD_ICON(bmp, RG53l, palette);
          RG2R = &rc_14c;
          DM2_DRAW_ICON_PICT_BUFF(bmp, &dm2_buttongroup2, /*TAG blitrect*/ RG2R, /*TAG srcx*/ RG3W, /*TAG srcy*/ parw04, 0xc, BLITMODE0, palette);
          RG1L = 263 * RG53l;
          RG52l = RG1L;
          if (party.hero[RG53l].ench_power != 0)
          {
            DM2_QUERY_PICST_IT(DM2_QUERY_GDAT_SUMMARY_IMAGE(&image, 1, 7, party.hero[RG53l].ench_aura));
            image.imgdesc.mode |= 0x10;
            RG1L = unsignedlong(party.hero[RG53l].absdir);
            RG4L = signedlong(ddat.v1e0258);
            RG1L += 4 - RG4L;
            RG4L = RG1L & lcon(0x3);
            image.imgdesc.width >>= 2;
            RG1L = signedlong(image.imgdesc.width) * RG4L;
            image.imgdesc.x = RG1W;
            image.imgdesc.height >>= 2;
            RG4L = signedlong(image.imgdesc.height);
            RG1W = CUTX16(timdat.gametick) & lcon(0x3);
            RG1L = signedlong(RG1W) * RG4L;
            image.imgdesc.y = RG1W;
            RG2L = signedlong(vw_160);
            RG3L = lcon(0xc);
            DM2_image_0b36_11c0(&image, &dm2_buttongroup2, RG2W, RG3W);
          }
        }
      }
    }
    //m_331B1:
    vo_15c.inc16();
  }
  FOREND
}

// belongs to DM2_UPDATE_RIGHT_PANEL
static void DM2_29ee_0b2b(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;

  RG2L = 0;
  //m_338EB:
  for (;;)
  {
    if (RG2UW >= mkuw(ddat.v1e0b7a))
    {
      DM2_DRAW_PLAYER_ATTACK_DIR();
      return;
    }
    //m_338DE:
    RG1L = unsignedlong(RG2W);
    DM2_DRAW_CMD_SLOT(RG1W, 0);
    RG2L++;
  }
  FOREND
}

// was SKW_2759_0644
void DM2_UPDATE_RIGHT_PANEL(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG56l;
  i32 RG57l;
  i32 RG58l;
  i32 RG59l;
  c_lreg _RG61;
  i32 RG62l;
  i16 RG63w;
  c_wlreg _RG64;
  c_wlreg _RG65;
  i16 RG66w;
  c_wlreg _RG67;
  i16 RG68w;
  i32 RG7l = 0;
  i32 vl_00;
  i8 vba_04[0x4];
  i32 vl_08;
  i32 vl_0c;
  i32 vl_10;
  i32 vl_14;
  i32 vl_18;
  c_o32 vo_1c; // poke 16 peek 8 16 32 inc16
  bool flag;

  vl_00 = RG1L;
  RG4L = 0;
  vl_08 = RG4L;
  vl_14 = RG4L;
  if (ddat.v1e0288 != 0)
    return;
  if (party.heros_in_party != 0)
  {
    //m_2FF83:
    if (RG1L != 0)
    {
      RG3L = 0;
      vo_1c.poke16(RG3W);
      //m_2FFDD:
      for (;;)
      {
        RG4L = signedlong(vo_1c.peek16());
        RG1L = unsignedlong(party.heros_in_party);
        if (RG4L >= RG1L)
        {
          RG1L = signedlong(ddat.v1d67bc);
          if (RG1L == lcon(0x3))
          {
            RG4L = signedlong(party.curacthero) - lcon(0x1);
            c_hero* hero1 = &party.hero[RG4L];
            RG1W = ddat.v1e0b62 & lcon(0x200);
            RG1L = unsignedlong(RG1W);
            if (RG1L == 0)
            {
              //m_30086:
              if (hero1->curMP != 0)
              {
                ddat.v1e0b6c = 1;
                RG3L = 0;
                ddat.v1e0b62 = RG3W;
              }
            }
            else
            {
              RG68w = ddat.v1e0b99;
              flag = --RG68w != 0;
              ddat.v1e0b99 = RG68w;
              if (!flag)
              {
                RG4W = hero1->curMP;
                if (RG4W == 0)
                {
                  //m_30074:
                  ddat.v1e0b6c = 1;
                  ddat.v1e0b62 = RG68w;
                }
                else
                {
                  RG67L = RG4L - 1;
                  hero1->curMP = RG67W;
                  hero1->heroflag |= 0x800;
                  RG2L = signedlong(ddat.v1e0b4e);
                  RG1L = lcon(0x19);
                  RG4L = RG1L % RG2L;
                  RG1L /= RG2L;
                  ddat.v1e0b99 = RG1W;
                }
              }
            }
          }
          break;
        }
        //m_2FF94:
        RG3L = 0;
        //m_2FFCE:
        for (;;)
        {
          RG61L = unsignedlong(RG3W);
          if (RG61L > 2)
          {
            vo_1c.inc16();
            break;
          }
          //m_2FF98:
          RG59l = signedlong(vo_1c.peek16());
          RG4Blo = party.hero[RG59l].handcooldown[RG61L];
          if (RG4Blo != 0)
          {
            RG4Bhi = RG4Blo;
            flag = --RG4Bhi != 0;
            party.hero[RG59l].handcooldown[RG61L] = RG4Bhi;
            if (!flag)
            {
              RG4L = signedlong(RG3W);
              RG1L = RG59l;
              DM2_LOAD_PROJECTILE_TO_HAND(RG1L, RG4L);
            }
          }
          RG3L++;
        }
      }
    }
    //m_3009F:
    if (ddat.v1e0b7c == 0)
    {
      bool skip00689 = false;
      bool skip00701 = false;
      //m_300D7:
      RG2W = party.curacthero;
      if (RG2W > 0)
      {
        RG1L = RG2L - 1;
        vo_1c.poke16(RG1W);
        RG3W = party.curactmode;
        if (RG3W >= 0)
        {
          if (RG3W != 2)
          {
            RG2L = signedlong(vo_1c.peek16());
            RG1L = 263 * RG2L;
            RG4L = 2 * signedlong(party.curactmode);
            RG1L += RG4L;
            RG4W = party.handitems.warr2d_00[RG2L + 1][party.curactmode];
            if (RG4W != party.hero[RG2L].item[party.curactmode])
              DM2_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
          }
        }
      }
      if (party.curacthero >= wcon(0x0))
      {
        RG4L = 0;
        vo_1c.poke16(RG4W);
        RG2Bhi = CUTX8(ddat.v1e0258);
        //m_301E2:
        for (;;)
        {
          RG4L = signedlong(vo_1c.peek16());
          RG1L = unsignedlong(party.heros_in_party);
          if (RG4L >= RG1L)
          {
            RG1Blo = ddat.v1d67d6;
            if (RG1Blo == ddat.v1d6c02 && ddat.savegames1.b_04 == 0)
            {
              RG1Blo = ddat.savegames1.b_02 == bcon(0x0) ? 1 : 0;
              RG1L = unsignedlong(RG1Blo);
              RG4Blo = ddat.v1e0b2c == 0 ? 1 : 0;
              RG4L = unsignedlong(RG4Blo);
              if (RG4L == RG1L)
                break;
            }
            //m_30232:
            vl_14 = 1;
            RG1Blo = ddat.v1d6c02;
            ddat.v1d67d6 = RG1Blo;
            RG1Blo = ddat.savegames1.b_02 != bcon(0x0) ? 1 : 0;
            RG1L = unsignedlong(RG1Blo);
            ddat.v1e0b2c = RG1L;
            break;
          }
          bool skip00688 = false;
          //m_30153:
          c_hero* hero2 = &party.hero[RG4L];
          RG2Blo = 0;
          vba_04[RG4L] = RG2Blo;
          RG1Bhi = (hero2->absdir + 4 - RG2Bhi) & 0x3;
          RG1Blo = (hero2->partypos + 4 - RG2Bhi) & 0x3;
          if (hero2->curHP != 0)
          {
            RG4L = signedlong(RG1Blo);
            RG58l = unsignedlong(ddat.v1e00b8);
            RG4L++;
            if (RG58l != RG4L)
              skip00688 = true;
          }

          if (!skip00688)
            //m_30198:
            RG1Blo = lcon(0xffffffff);

          //m_3019A:
          RG4L = signedlong(vo_1c.peek16());
          if (RG1Blo != party.handitems.barr_00[RG4L] || RG1Bhi != v1d67be[RG4L])
          {
            RG57l = 1;
            vl_14 = RG57l;
            RG7l = RG57l;
            RG4L = signedlong(RG1Blo);
            if (RG4L != lcon(0xffffffff))
              ddat.v1e0b30 = RG57l;
            RG4L = signedlong(vo_1c.peek16());
            party.handitems.barr_00[RG4L] = RG1Blo;
            v1d67be[RG4L] = RG1Bhi;
          }
          vo_1c.inc16();
        }
      }
      //m_30258:
      if (ddat.v1e0b30 == 0)
        skip00689 = true;
      else
      {
        RG1L = unsignedlong(ddat.v1e0b6c);
        if (RG1L <= lcon(0x1))
        {
          //m_3027D:
          DM2_guidraw_29ee_00a3(lcon(0x1));
          ddat.v1e0b30 = 0;
          vl_08 = 1;
          skip00689 = true;
        }
        else
          skip00701 = true;
      }

      if (skip00689)
      {
        bool skip00696 = false;
        bool skip00697 = false;
        //m_30297:
        RG4W = party.curacthero;
        if (RG4W == 0)
        {
          ddat.v1d67bc = RG4W;
          if (vl_08 != 0 || RG7l != 0)
            ddat.v1d694a = lcon(0xffffffff);
          vo_1c.poke16(wcon(0x0));
          //m_30566:
          for (;;)
          {
            RG4L = signedlong(vo_1c.peek16());
            RG1L = unsignedlong(party.heros_in_party);
            if (RG4L >= RG1L)
            {
              if (ddat.v1e0b54 < wcon(0x0))
              {
                RG4L = 0;
                ddat.v1e0b54 = RG4W;
              }
              ddat.v1e0b54 = -ddat.v1e0b54;
              if (vl_08 != 0 || vl_14 != 0)
                DM2_29ee_0396();
              break;
            }
            bool skip00700 = false;
            //m_302CE:
            c_hero* hero3 = &party.hero[RG4L];
            if (hero3->curHP == 0)
            {
              RG1W = DM2_GET_PLAYER_AT_POSITION(unsignedword(hero3->partypos));
              if (RG1W >= 0)
                skip00700 = true;
            }

            if (!skip00700)
            {
              //m_302FB:
              RG1L = signedlong(vo_1c.peek16());
              RG4L = 0;
              RG4Blo = vba_04[RG1L];
              vl_10 = RG4L;
              RG66w = hero3->curHP;
              RG4Blo = RG66w > 0 ? 1 : 0;
              RG4L = unsignedlong(RG4Blo);
              if (RG4L != ddat.v1e0b1c[RG1L])
              {
                RG1Blo = RG66w > 0 ? 1 : 0;
                RG4L = RG1L & lcon(0xff);
                ddat.v1e0b1c[signedlong(vo_1c.peek16())] = RG4L;
                vl_10 = 1;
                ddat.v1d694a = lcon(0xffffffff);
              }
              RG4L = signedlong(vo_1c.peek16());
              RG1L = signedlong(DM2_ABS(ddat.v1e0b54)) - 1;
              RG1Blo = RG1L == RG4L ? 1 : 0;
              RG1L = unsignedlong(RG1Blo);
              vl_18 = RG1L;
              RG3L = 0;
              //m_30457:
              for (;;)
              {
                RG1L = unsignedlong(RG3W);
                if (RG1L > lcon(0x1))
                {
                  RG62l = 0;
                  if (vl_18 != 0)
                  {
                    if (ddat.v1e0b52 == 2)
                    {
                      if (ddat.v1e0b54 > wcon(0x0))
                        RG62l = 1;
                      vl_10 = 1;
                    }
                  }
                  RG2Blo = (hero3->handcooldown[2] != 0) ? 1 : 0;
                  RG4L = signedlong(vo_1c.peek16());
                  RG1L = 3 * RG4L;
                  if (RG2Blo != ddat.v1e0b34[RG1L / 3][2])
                  {
                    ddat.v1e0b34[RG1L / 3][2] = RG2Blo;
                    vl_10 = 1;
                  }
                  if (vl_08 == 0 && vl_10 == 0)
                  {
                    RG1W = party.curactevhero; // (e_hero)
                    RG4W = eventqueue.event_heroidx; // (e_hero)
                    if (RG1W == RG4W)
                      break;
                    RG2W = vo_1c.peek16();
                    if (RG1W != RG2W && RG4W != RG2W)
                      break;
                  }
                  //m_304E8:
                  RG1L = signedlong(vo_1c.peek16());
                  RG4L = RG62l;
                  DM2_DRAW_SQUAD_SPELL_AND_LEADER_ICON(RG1W, RG4L);
                  break;
                }
                //m_3037D:
                RG62l = 0;
                RG4L = 0;
                if (vl_18 != 0)
                {
                  if (signedlong(ddat.v1e0b52) == RG1L)
                  {
                    if (ddat.v1e0b54 > wcon(0x0))
                      RG62l = 1;
                    RG4L = 1;
                  }
                }
                RG2Blo = (hero3->handcooldown[RG3W] != 0) ? 1 : 0;
                RG1L = signedlong(vo_1c.peek16());
                vl_0c = RG1L;
                RG1L = 3 * vl_0c;
                RG1L += unsignedlong(RG3W);
                if (RG2Blo != ddat.v1e0b34[vl_0c][unsignedlong(RG3W)])
                {
                  ddat.v1e0b34[vl_0c][unsignedlong(RG3W)] = RG2Blo;
                  RG4L = 1;
                }
                RG1L = 2 * unsignedlong(RG3W);
                RG2W = party.handitems.warr2d_00[vo_1c.peek16() + 1][RG3W];
                if (RG2W != hero3->item[RG3W] || vl_08 != 0 || vl_10 != 0)
                {
                  RG1L = 2 * unsignedlong(RG3W);
                  RG4W = hero3->item[RG3W];
                  party.handitems.warr2d_00[vo_1c.peek16() + 1][RG3W] = RG4W;
                  RG4L = 1;
                }
                if (vl_08 != 0 || vl_10 != 0 || RG4L != 0)
                {
                  RG4L = unsignedlong(RG3W);
                  RG1L = unsignedlong(vo_1c.peek16());
                  RG2L = RG62l;
                  DM2_DRAW_HAND_ACTION_ICONS(RG1W, RG4L, RG2L);
                }
                RG3L++;
              }
              //m_304F6:
              RG4L = signedlong(vo_1c.peek16());
              RG1L = 263 * RG4L;
              if (party.hero[RG4L].ench_power != 0 || ddat.v1d6948 != 0)
                vl_14 = 1;
              RG4L = signedlong(vo_1c.peek16());
              RG1L = 263 * RG4L;
              RG4L = RG1L;
              RG3Blo = vo_1c.peek8();
              RG1L = lcon(0x1) << RG3Blo;
              if (party.hero[RG4L / 263].ench_power == 0)
              {
                //m_30558:
                RG1L = ~RG1L;
                ddat.v1d6948 &= RG1W;
              }
              else
                ddat.v1d6948 |= RG1W;
            }

            //m_30561:
            vo_1c.inc16();
          }
        }
        //m_305AA:
        party.curactevhero = eventqueue.event_heroidx;
        if (party.curacthero <= E_HERO0)
          skip00696 = true;
        else
        {
          bool skip00691 = false;
          if (ddat.v1e0b6c != 0 || vl_08 != 0 || vl_14 != 0)
            skip00691 = true;
          else
          {
            RG1L = signedlong(ddat.v1d67bc);
            if (RG1L == lcon(0x3) && vl_00 != 0)
              skip00691 = true;
          }

          if (skip00691)
          {
            //m_305F7:
            RG1L = unsignedlong(ddat.v1e0b6c);
            if (RG1L > lcon(0x1))
              skip00701 = true;
            else
            {
              bool skip00692 = false;
              if (ddat.v1e0b62 == 0)
                skip00692 = true;
              else
              {
                if (RG1W != 0 || vl_08 != 0)
                  //m_30625:
                  RG1L = 0;
                else
                  RG1L = 1;
                //m_30627:
                if (RG1L == 0)
                  skip00692 = true;
                else
                {
                  RG1L = 0;
                  DM2_guidraw_29ee_00a3(RG1L);
                }
              }

              if (skip00692)
              {
                //m_30634:
                RG1L = 1;
                DM2_guidraw_29ee_00a3(RG1L);
                DM2_DRAW_PLAYER_NAME_AT_CMDSLOT();
                ddat.v1e0b62 &= lcon(0xfffffbff);
              }
              //m_3064A:
              RG4W = party.curactmode;
              if (RG4W != 2)
              {
                //m_30668:
                RG1W = party.curacthero;
                RG1L--;
                vo_1c.poke16(RG1W);
                RG4L = signedlong(vo_1c.peek16());
                c_hero* hero4 = &party.hero[RG4L];
                RG3W = hero4->item[party.curactmode];
                RG1L = DM2_IS_CONTAINER_MAP(unsignedlong(RG3W));
                if (RG1L == 0 || ddat.v1e0b62 != 0)
                {
                  //m_306FE:
                  RG56l = unsignedlong(RG3W);
                  RG1L = DM2_IS_CONTAINER_MONEYBOX(RG56l);
                  if (RG1L == 0)
                  {
                    //m_30719:
                    RG1L = DM2_IS_CONTAINER_CHEST(CUTX16(RG56l)) ? 1 : 0;
                    if (RG1L != 0)
                    {
                      RG1L = signedlong(ddat.v1d67bc);
                      if (RG1L != lcon(0x5))
                      {
                        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(CUTX16(RG56l))));
                        DM2__CHECK_ROOM_FOR_CONTAINER(RG56l, RG1P);
                        ddat.v1d67bc = 5;
                      }
                    }
                  }
                  else
                    ddat.v1d67bc = 4;
                }
                else
                {
                  RG1L = 1;
                  ddat.v1e0b4e = RG1W;
                  ddat.v1e0b99 = 25;
                  RG2W = hero4->curMP;
                  if (RG2W == 0)
                    //m_306EA:
                    ddat.v1e0b62 = lcon(0xffff8000);
                  else
                  {
                    RG65L = RG2L - RG1L;
                    hero4->curMP = RG65W;
                    hero4->heroflag |= 0x800;
                    ddat.v1e0b62 = wcon(0x8200);
                  }
                  //m_306F3:
                  ddat.v1d67bc = 3;
                }
                //m_3074A:
                RG1L = signedlong(ddat.v1d67bc);
                if (RG1L != lcon(0x4))
                {
                  //m_30766:
                  if (RG1L != lcon(0x5))
                  {
                    bool skip00694 = false;
                    //m_3077F:
                    RG1L = DM2_IS_ITEM_HAND_ACTIVABLE(unsignedlong(vo_1c.peek16()), unsignedlong(RG3W), signedlong(party.curactmode));
                    if (RG1L == 0)
                      skip00694 = true;
                    else
                    {
                      RG1L = signedlong(ddat.v1d67bc);
                      if (RG1L == lcon(0x3))
                        skip00694 = true;
                      else
                      {
                        RG63w = wcon(0x1);
                        ddat.v1d67bc = RG63w;
                        if (RG63w != ddat.v1d694a)
                        {
                          RG1L = DM2_107B0();
                          ddat.v1d694a = ddat.v1d67bc;
                        }
                        DM2_29ee_0b2b();
                      }
                    }

                    if (skip00694)
                    {
                      //m_307D7:
                      RG1L = signedlong(ddat.v1d67bc);
                      if (RG1L != lcon(0x3))
                      {
                        //m_3080B:
                        RG2L = 0;
                        party.curacthero = RG2W;
                        ddat.v1d67bc = RG2W;
                      }
                      else
                      {
                        RG1W = ddat.v1e0b62 & lcon(0x400);
                        RG1L = unsignedlong(RG1W);
                        if (RG1L == 0)
                          ddat.v1d694a = lcon(0xffffffff);
                        DM2_DRAW_MAJIC_MAP(unsignedlong(RG3W));
                      }
                    }
                  }
                  else
                    DM2_DRAW_CONTAINER_PANEL(unsignedlong(RG3W), lcon(0x1));
                }
                else
                  DM2_DRAW_MONEYBOX(RG3W);
              }
              else
              {
                ddat.v1d67bc = RG4W;
                DM2_DRAW_SPELL_PANEL();
              }
              //m_3081B:
              RG2L = 0;
              ddat.v1e0b6c = RG2W;
              skip00697 = true;
            }
          }
          else
            skip00696 = true;
        }

        if (skip00696)
        {
          //m_30826:
          if (party.curacthero > wcon(0x0))
          {
            RG1L = signedlong(ddat.v1d67bc);
            if (RG1L == lcon(0x5))
            {
              if (vl_00 != 0)
              {
                DM2_guidraw_29ee_00a3(0);
                RG4L = signedlong(party.curacthero) - 1;
                RG1W = party.hero[RG4L].item[party.curactmode];
                RG1L = unsignedlong(RG1W);
                DM2_DRAW_CONTAINER_PANEL(RG1L, 0);
              }
            }
          }
          skip00697 = true;
        }

        if (skip00697)
        {
          //m_3087E:
          if (party.curacthero > wcon(0x0))
          {
            RG1L = signedlong(ddat.v1d67bc);
            if (table1d67d9[RG1L] != 0)
            {
              RG4L = signedlong(party.curacthero) - lcon(0x1);
              RG1L = 263 * RG4L;
              if (party.hero[RG4L].ench_power != 0 || ddat.v1d6948 != 0)
              {
                DM2_DRAW_PLAYER_ATTACK_DIR();
                RG4L = signedlong(party.curacthero) - lcon(0x1);
                RG1L = 263 * RG4L;
                RG4L = RG1L;
                RG3W = party.curacthero;
                RG3L--;
                RG1L = lcon(0x1) << RG3Blo;
                if (party.hero[RG4L / 263].ench_power == 0)
                {
                  //m_3090C:
                  RG1L = ~RG1L;
                  ddat.v1d6948 &= RG1W;
                }
                else
                  ddat.v1d6948 |= RG1W;
              }
            }
          }
        }
      }

      if (skip00701)
      {
        //m_3026E:
        RG64L = RG1L - 1;
        ddat.v1e0b6c = RG64W;
      }
    }
    else
    {
      DM2_SHOW_ATTACK_RESULT(ddat.v1e0b7c);
      ddat.v1e0b30 = 1;
      RG2L = 0;
      ddat.v1e0b7c = RG2W;
      ddat.v1d67bc = 6;
    }
  }
  else
  {
    if (ddat.v1e0b30 != 0 || ddat.v1e0b6c != 0)
    {
      ddat.v1e0b6c = 0;
      ddat.v1e0b30 = 0;
      DM2_guidraw_29ee_00a3(1);
    }
    ddat.v1d67bc = -1;
  }

  //m_30915:
  if (dm2_buttongroup2.button.dbidx != NODATA)
  {
    RG4L = 1;
    RG1P = DOWNCAST(c_buttongroup, &dm2_buttongroup2);
    DM2_gfxmain_0b36_0cbe(UPCAST(c_buttongroup, RG1P), true);
  }
  RG1W = ddat.v1d67bc;
  if (RG1W != ddat.v1d694a)
  {
    RG1L = DM2_107B0();
    RG1W = ddat.v1d67bc;
    ddat.v1d694a = RG1W;
  }
}
