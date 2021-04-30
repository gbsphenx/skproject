#include "c_gfx_str.h"
#include "util.h"
#include "c_xrect.h"
#include "c_querydb.h"
#include "c_dballoc.h"
#include "c_str.h"
#include "c_gfx_blit.h"
#include "c_gfx_pal.h"
#include "c_gfx_main.h"
#include "c_hero.h"
#include "c_tmouse.h"
#include "dm2data.h"

c_stringdata strdat;

#ifdef NOTIFY
t_text notify[320/8+1]; // TODO temporary
#endif

void c_stringdata::init(void)
{
  i16 i = 0;
  for (i = 0; i < 5; i++)
    fnum[i] = '\0'; // in c_str.cpp

  for (i = 0; i < 24; i++)
    font[i] = c_pixel(E_COL00);

  strx = 0;
  stry = 0;
  strxplus = 7;
  strptr = NULL;
  gfxstrw1 = 6;
  gfxstrw2 = 1;
  gfxstrw3 = 1;
  gfxstrw4 = 6;
}

static c_pixel* DM2_QUERY_FONT(i8 eaxb, i8 edxb, i8 ebxb)
{
  i16 k = 0;
  for (i16 i=0; i < 6; i++)
  {
    i8 byterg11 = strdat.strptr[(i << 7) + unsignedword(eaxb)];

    for (i16 j=0; j < 3; j++)
    {
      ui8 ubyterg12;
      if ((byterg11 & 0x10) == 0)
        ubyterg12 = ebxb << 4;
      else
        ubyterg12 = edxb << 4;
      if ((byterg11 & 0x8) == 0)
        ubyterg12 |= ebxb;
      else
        ubyterg12 |= edxb;
      byterg11 *= 4;
      strdat.font[k++] = ui8_to_pixel(ubyterg12);
    }
  }
  return strdat.font;
}

bool DM2_QUERY_STR_METRICS(t_text* text, i16* edxpw, i16* ebxpw)
{
  if (text == NULL)
    return false;
  i16 wordrg2 = -strdat.gfxstrw2;
  i16 wordrg6 = strdat.gfxstrw1 - strdat.gfxstrw3;
  while (*text++ != '\0')
    wordrg2 += strdat.gfxstrw4;
  if (wordrg2 <= 0)
    return false;
  *edxpw = wordrg2;
  *ebxpw = wordrg6;
  return true;
}

// stacksize was 0x2c
// last argument removed, is always BPP_8
void DM2_DRAW_STRING(c_pixel* destgfx, i16 edxw, i16 ebxw, i16 ecxw, i16 argw0, i16 argw1, t_text* argpt2)
{
  t_palette palette[PAL16];
  c_rect rc_10;
  i16 vw_18;
  i16 vw_1c;
  t_resolution dbpp = BPP_8;

  vw_1c = edxw;
  if (*argpt2 != '\0')
  {
    if ((argw1 & wcon(0x4000)) == wcon(0x0))
      vw_18 = -1;
    else
    {
      argw1 &= lcon(0xffffbfff);
      vw_18 = argw1;
    }
    rc_10.x = ebxw;
    if (dbpp != BPP_4) // always true now - kept for possible extensions
    {
      ui8_to_palettecolor(&palette[E_COL00], mkub(argw0));
      ui8_to_palettecolor(&palette[E_COL01], mkub(argw1));
      argw0 = 0;
      argw1 = 1;
      if (vw_18 != lcon(0xffffffff))
        vw_18 = 1;
    }
    rc_10.h = strdat.gfxstrw1;
    ecxw += strdat.gfxstrw3 - (rc_10.h - 1);
    rc_10.y = ecxw;
    rc_10.w = strdat.gfxstrw4;
    do
    {
      ui16 destw;
      if (destgfx == gfxsys.dm2screen)
        destw = ORIG_SWIDTH;
      else
        destw = vw_1c;

      blitter.blit(
        DM2_QUERY_FONT(*argpt2, CUTX8(argw0), CUTX8(argw1)),
        destgfx,
        /*TAG blitrect*/ &rc_10,
        0,
        0,
        5,
        destw,
        /*TAG alphamask*/ vw_18,
        BLITMODE0,
        BPP_4,
        dbpp,
        palette);

      rc_10.x += strdat.gfxstrw4;
    } while (*++argpt2 != '\0');
  }
}

// stacksize was 0x2c
void DM2_DRAW_STRONG_TEXT(c_pixel* destgfx, i16 edxw, i16 ebxw, i16 ecxw, i16 argw0, i16 argw1, t_text* argpt2)
{
  i16 vw_08;
  i16 vw_0c;

  if (DM2_QUERY_STR_METRICS(argpt2, &vw_0c, &vw_08))
  {
    if (!(argw1 & 0x4000))
    {
      c_rect* rptrrg1 = tmprects.alloc_tmprect(ebxw - 1, ecxw - vw_08, vw_0c + wcon(0x2), vw_08 + wcon(0x2));
      ui16 pixels;
      if (destgfx == gfxsys.dm2screen)
        pixels = ORIG_SWIDTH;
      else
        pixels = edxw;
      blitter.fill(destgfx, /*TAG pix*/ ui8_to_pixel(CUTUX8(argw1)), /*TAG pixels*/ pixels, /*TAG blitrect*/ rptrrg1, BPP_8);
    }
    DM2_DRAW_STRING(destgfx, edxw, ebxw, ecxw + 1, unsignedword(palettecolor_to_ui8(paldat.palette[E_COL00])), unsignedword(palettecolor_to_ui8(paldat.palette[15])) | uwcon(0x4000), argpt2);
    DM2_DRAW_STRING(destgfx, edxw, ebxw + 1, ecxw + 1, unsignedword(palettecolor_to_ui8(paldat.palette[E_COL00])), unsignedword(palettecolor_to_ui8(paldat.palette[15])) | uwcon(0x4000), argpt2);
    DM2_DRAW_STRING(destgfx, edxw, ebxw, ecxw, argw0, argw1 | wcon(0x4000), argpt2);
  }
}

// stacklen was 0x24
// old name: DM2_DRAW_SIMPLE_STR, TODO: tell SPX new name
void DM2_DRAW_BUTTON_STR(c_buttongroup* buttongroup, i16 edxw, i16 ebxw, i16 ecxw, t_text* argpt0)
{
  c_rect rc_00;
  i16 vw_08;
  i16 vw_0c;
  i16 vw_10;
  i16 vw_14;

  vw_10 = ebxw;
  vw_0c = ecxw;
  if (DM2_QUERY_STR_METRICS(argpt0, &vw_08, &vw_14))
    if (DM2_QUERY_BLIT_RECT(NULL, &rc_00, edxw, IO vw_08, IO vw_14) != NULL)
    {
      DM2_DRAW_STRING(PIXELCAST(dm2_dballochandler.DM2_GET_BMP(buttongroup->button.dbidx)), buttongroup->button.r.w, rc_00.x - buttongroup->button.r.x, rc_00.y + rc_00.h - 1 - buttongroup->button.r.y, vw_10, vw_0c, argpt0);
      DM2_ADJUST_BUTTONGROUP_RECTS(buttongroup, &rc_00);
    }
}

// stacklen was 0x24
void DM2_DRAW_NAME_STR(c_buttongroup* buttongroup, i16 edxw, i16 ebxw, i16 ecxw, t_text* argpt0)
{
  c_rect rc_00;
  i16 vw_08;
  i16 vw_14;

  i16 vw_10 = ebxw;
  i16 vw_0c = ecxw;
  if (DM2_QUERY_STR_METRICS(argpt0, &vw_08, &vw_14))
  {
    if (DM2_QUERY_BLIT_RECT(NULL, &rc_00, edxw, IO vw_08, IO vw_14) != NULL)
    {
      DM2_DRAW_STRONG_TEXT(PIXELCAST(dm2_dballochandler.DM2_GET_BMP(buttongroup->button.dbidx)), buttongroup->button.r.w, rc_00.x - buttongroup->button.r.x, rc_00.y + rc_00.h - 1 - buttongroup->button.r.y, vw_10, vw_0c, argpt0);
      DM2_ADJUST_BUTTONGROUP_RECTS(buttongroup, &rc_00);
    }
  }
}

void DM2_DRAW_VP_STR(i16 eaxw, i16 edxw, i16 ebxw, t_text* ecxtp)
{
  DM2_DRAW_STRING(PIXELCAST(gfxsys.bitmapptr), getbmpheader(gfxsys.bitmapptr)->width, eaxw, edxw, ebxw, unsignedword(palettecolor_to_ui8(paldat.palette[12])) | uwcon(0x4000), ecxtp);
}

void DM2_DRAW_GUIDED_STR(t_text* eaxtp)
{
  t_text tarr_00[0x80];

  if (*eaxtp == lcon(0xc))
  {
    eaxtp++;
    DM2_QUERY_TOPLEFT_OF_RECT(lcon(0x22c), OO strdat.stry, OO strdat.strx);
  }
  if (*eaxtp != '\0')
  {
    DM2_STRCPY(tarr_00, eaxtp);
    t_text* tprg6 = tarr_00;
    bool boolrg7 = false;
    while (*tprg6 != '\0')
    {
      if (DM2_STRLEN(tprg6) <= lcon(0x12))
        eaxtp = NULL;
      else
      {
        eaxtp = tprg6 + lcon(0x11);
        while (*eaxtp != ' ')
          eaxtp--;
        *eaxtp = '\0';
        boolrg7 = true;
      }
      DM2_DRAW_VP_STR(strdat.stry, strdat.strx, unsignedword(palettecolor_to_ui8(paldat.palette[13])), tprg6);
      strdat.strx += strdat.strxplus;
      if (!boolrg7 || eaxtp == NULL)
        *tprg6 = '\0';
      else
      {
        boolrg7 = false;
        tprg6 = eaxtp + 1;
      }
    }
  }
}

void DM2_PRINT_SYSERR_TEXT(i16 eaxw, i16 edxw, i16 ebxw, i16 ecxw, t_text* argpt0)
{
  DM2_DRAW_STRING(gfxsys.dm2screen, ORIG_SWIDTH, eaxw, edxw, ebxw, ecxw, argpt0);
}

void DM2_DRAW_VP_RC_STR(i16 eaxw, i16 edxw, t_text* ebxtp)
{
  c_rect rc_00;
  i16 vw_08;
  i16 vw_0c;

  if (DM2_QUERY_STR_METRICS(ebxtp, &vw_08, &vw_0c))
    if (DM2_QUERY_BLIT_RECT(NULL, &rc_00, eaxw, IO vw_08, IO vw_0c) != NULL)
      DM2_DRAW_VP_STR(rc_00.x, rc_00.y + rc_00.h - 1, edxw, ebxtp);
}

void DM2_DRAW_LOCAL_TEXT(i32 eaxl, i32 edxl, i16 ebxw, t_text* ecxtp)
{
  c_ql32 vql_00;
  c_ql32 vql_04;
  c_rect rc_08;
  i16 vw_10;
  i16 vw_14;

  vql_04.poke32(eaxl);
  vql_00.poke32(edxl);
  if (DM2_QUERY_STR_METRICS(ecxtp, &vw_10, &vw_14))
    if (DM2_QUERY_BLIT_RECT(NULL, &rc_08, vql_04.peek16(), IO vw_10, IO vw_14) != NULL)
      DM2_DRAW_STRONG_TEXT(PIXELCAST(gfxsys.bitmapptr), getbmpheader(gfxsys.bitmapptr)->width, rc_08.x, rc_08.y + rc_08.h - 1, vql_00.peek16(), ebxw, ecxtp);
}

//#######################################
// Attention: those two call each other!
//#######################################

#include "c_alloc.h"
#include "c_gdatfile.h"
#include "startend.h"

// TODO: optimize temporaries
#include "regs.h"
#include "emu.h"

t_text* DM2_FORMAT_SKSTR(t_text* eaxtp, t_text* edxtp)
{
  c_nreg _RG1;
  c_nreg _RG2;
  t_text* RG4t = edxtp;
  t_text* RG5t = eaxtp;
  t_text* RG6t;
  t_text* RG7t = edxtp;
  t_text tarr_00[128];
  t_text tarr_80[128];
  t_text tarr_104[3]; // dim was 4, but 3 seems to be enough
  tarr_104[2] = '\0'; // added: go safe, there's a strcpy
  i8 opt51;
  i8 opt52;
  i8 opt53;
  i32 opt6;

  if (RG4t == NULL)
  {
    RG7t = RG4t = UPCAST(t_text, ddat.vp_1e097c[ddat.v1e0988]);
    ddat.v1e0988++;
    if (ddat.v1e0988 >= 2)
      ddat.v1e0988 = 0;
  }

  //m_2A79E:
  for (;;)
  {
    i8 c;
    for (;;)
    {
      c = *RG5t;
      if (c == '\0')
      {
        //m_2A9C2:
        *RG4t = c;
        return RG7t;
      }
      if (c == '.' && RG5t[1] == 'Z')
        break;
      //m_2A7BB:
      if (c == 1)
        break;
      //m_2A9B9:
      *RG4t++ = c;
      RG5t++;
    }

    //m_2A7C8:
    if (c != 1)
    {
      //m_2A7DF:
      RG2L = 0;
      RG5t += 2;
      //m_2A7F6:
      for (i16 i=0; i<3; i++)
      {
        //m_2A7E8:
        i32 RG3l = unsignedlong(*RG5t++);
        RG2L = RG2L * lcon(0xa) + RG3l - lcon(0x30);
      }
    }
    else
    {
      RG5t += unsignedlong(c);
      RG2L = unsignedlong(*RG5t);
      RG2L -= lcon(0x20);
      RG5t += unsignedlong(c);
    }
    //m_2A7FE:
    *RG4t = '\0';
    RG6t = tarr_80;
    opt53 = 1;

    int option = -1; // range -1...6
    if (RG2UW > mkuw(lcon(0x1c)))
      option = 0;
    else
    {
      RG1L = unsignedlong(RG2W);
      switch (RG1L)
      {
        case 0:
          //m_2A821:
          opt6 = unsignedlong(ddat.v1e0218);
          option = 6;
          break;

        case 1:
          //m_2A82B:
          opt6 = (-ddat.v1d70c5 + lcon(0x3ff)) >> bcon(0xa);
          option = 6;
          break;

        case 2:
          //m_2A873:
          opt51 = -2;
          opt52 = RG1Bhi ^ RG2Bhi;
          option = 5;
          break;

        case 3:
          option = 4;
          break;

        case 4:
          option = 3;
          break;

        case 5:
        case 6:
        case 16:
        case 18:
        case 19:
        case 21:
          option = 0;
          break;

        case 7:
          //m_2A8AA:
          if (ddat.v1e098c != -1)
            RG6t = party.hero[ddat.v1e098c].name1;
          break;

        case 8:
          //m_2A901:
          RG6t = ddat.v1d1542;
          break;

        case 9:
          //m_2A90C:
          RG6t = ddat.v1d1546;
          break;

        case 10:
          //m_2A843:
          opt6 = signedlong(ddat.v1e099a);
          option = 6;
          break;

        case 11:
          //m_2A84A:
          opt6 = signedlong(ddat.v1e098e);
          option = 6;
          break;

        case 12:
          //m_2A851:
          opt6 = signedlong(ddat.v1e0998);
          option = 6;
          break;

        case 13:
          //m_2A858:
          opt6 = signedlong(ddat.v1e0996);
          option = 6;
          break;

        case 14:
          //m_2A85F:
          opt6 = signedlong(ddat.v1e098a);
          option = 6;
          break;

        case 15: // ddat.v1e0992 == 1 can't be anymore! TODO option 4 still needed?
          //m_2A8D4:
          if (ddat.v1e0992 == 2)
            option = 3;
          break;

        case 17:
          //m_2A879:
          opt53 = 7;
          opt51 = RG1Blo ^ RG2Blo;
          opt52 = CUTX8(ddat.v1e0994);
          option = 5;
          break;

        case 20:
          option = 2;
          break;

        case 22: // some code removed, ddat.v1d154e always 1
          //m_2A93C:
          RG6t = ddat.datafoldername;
          break;

        case 23:
          //m_2A955:
          tarr_104[0] = ddat.v1e099c;
          tarr_104[1] = RG1Bhi ^ RG2Bhi;
          DM2_STRCPY(tarr_80, tarr_104);
          break;

        case 24:
          //m_2A97F:
          RG6t = table1d674e[signedlong(ddat.v1d674c)];
          break;

        case 25:
          //m_2A83C:
          opt6 = signedlong(ddat.v1d70c9);
          option = 6;
          break;

        case 26:
          option = 1;
          break;

        case 27:
          //m_2A933:
          opt51 = -2;
          opt52 = 6;
          option = 5;
          break;

        case 28:
          //m_2A917:
          if (gdat.filetype2)
            option = 1;
          else
            option = 2;
          break;

        default: throw(THROW_DMABORT);
      }
    }

    switch (option)
    {
      case 0:
        //m_2A991:
        RG6t = v1d10f4;
        break;

      case 1:
        //m_2A92B:
        RG6t = ddat.v1d676a;
        break;

      case 2:
        //m_2A920:
        RG6t = ddat.v1d675a;
        break;

      case 3:
        //m_2A8F6:
        RG6t = ddat.v1d153e;
        break;

      case 4:
        //m_2A8EB:
        RG6t = ddat.v1d153a;
        break;

      case 5:
        //m_2A883:
        DM2_QUERY_GDAT_TEXT(opt53, opt51, opt52, RG6t);
        break;

      case 6:
        //m_2A867:
        DM2_LTOA10(opt6, RG6t);
        break;
    }

    //m_2A996:
    DM2_FORMAT_SKSTR(RG6t, tarr_00);
    DM2_STRCAT(RG7t, tarr_00);
    RG4t = RG7t + CUTX16(DM2_STRLEN(RG7t));
  }
  FOREND
}

// IMPORTANT TODO:
//   is it possible to get DM2_FORMAT_SKSTR(tp_08, RG6t); out of this?
t_text* DM2_QUERY_GDAT_TEXT(i8 eaxb, i8 edxb, i8 ebxb, t_text* ecxtp)
{
  *ecxtp = '\0';
  t_text* tp_00 = UPCAST(t_text, DM2_QUERY_GDAT_ENTRY_DATA_BUFF(eaxb, edxb, 5, ebxb));
  if (tp_00 == NULL)
    return ecxtp;
  i16 len = CUTX16(DM2_QUERY_GDAT_ENTRY_DATA_LENGTH(eaxb, edxb, 5, ebxb));
  t_text* tp_08 = new t_text[len];
  DM2_COPY_MEMORY(DOWNCAST(t_text, tp_08), DOWNCAST(t_text, tp_00), signedlong(len));
  if (ddat.v1e0984 != 0)
  {
    t_text* tp = tp_08;
    i8 blo = 0;
    //m_2AA51:
    i16 i = len;
    while (--i != -1)
      *tp++ = ~*tp - blo++;
  }
  //m_2AA6F:
  DM2_FORMAT_SKSTR(tp_08, ecxtp);
  delete [] tp_08;
  return ecxtp;
}

void DM2_DRAW_TEXT_TO_BACKBUFF(i32 eaxl, i32 edxl, t_text* ebxtp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2T = ebxtp;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  t_text* RG5t;
  i16 RG6w;
  t_palette palette[PAL16];
  i16 vw_10;
  i16 vw_14;
  i16 vw_18;
  i16 vw_1c;
  i16 parw01;
  i16 parw02;
  t_text* par_tp03;

  RG6w = RG1W;
  vw_1c = RG4W;
  RG5t = UPCAST(t_text, RG2P);
  vw_10 = lcon(0x10); // adj
  RG2P = ADRESSOF(i16, &vw_18);
  RG4P = ADRESSOF(i16, &vw_14);
  RG1L = DM2_QUERY_STR_METRICS(RG5t, RG4P16, RG2P16) ? 1 : 0;
  if (RG1L != 0)
  {
    DM2_COPY_SMALL_PALETTE(palette, paldat.palette);
    RG4L = unsignedlong(CUTX8(ddat.v1e12d2));
    RG3L = lcon(0xffffffff);
    DM2_query_B073(palette, RG4Blo, RG3W, RG3W, &vw_10);
    par_tp03 = RG5t;
    RG1L = unsignedlong(palettecolor_to_ui8(palette[E_COL00]));
    RG1Bhi |= lcon(0x40);
    put16(parw02, RG1W);
    RG1L = unsignedlong(palettecolor_to_ui8(palette[E_COL15]));
    put16(parw01, RG1W);
    RG3L = signedlong(vw_1c);
    RG1W = vw_14;
    RG1L <<= bcon(0x10);
    RG1L >>= bcon(0x11);
    RG4L = signedlong(RG6w) - RG1L;
    RG2L = signedlong(RG4W);
    DM2_DRAW_STRONG_TEXT(PIXELCAST(gfxsys.bitmapptr), getbmpheader(gfxsys.bitmapptr)->width, RG2W, RG3W, parw01, parw02, par_tp03);
  }
}

i32 DM2_gfxstr_3929_04e2(t_text* eaxtp, t_text* edxtp, i16* ebxpw, i32 ecxl)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2P = DOWNCAST(i16, ebxpw);
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4T = edxtp;
  i32 RG51l;
  t_text* RG52t;
  i32 RG53l;
  i32 RG54l;
  i32 RG6l;
  t_text* RG7t;
  t_text* tp_00;
  i16* wp_04;
  i16 vw_08;
  i32 vl_0c;
  i16 vw_10;
  i16 vw_14;

  tp_00 = eaxtp;
  RG7t = RG4T;
  wp_04 = RG2P16;
  vw_10 = RG3W;
  RG4W = word_at(RG2P);
  RG1L = 0;
  RG2L = 0;
  vw_08 = -1;
  vw_14 = RG1W;
  vl_0c = RG1L;
  //m_455D6:
  for (;;)
  {
    *(RG7t + signedlong(RG2W)) = '\0';
    RG3T = tp_00 + signedlong(RG4W);
    RG51l = unsignedlong(byte_at(RG3P));
    if (RG51l != lcon(0xa) && byte_at(RG3P) != 0)
    {
      //m_455FC:
      if (RG51l == lcon(0x20))
      {
        vw_08 = RG4W;
        vw_14 = RG2W;
        vl_0c = RG1L;
      }
      RG53l = signedlong(RG1W) + signedlong(strdat.gfxstrw4);
      RG3L = signedlong(vw_10);
      if (RG53l <= RG3L)
      {
        if (RG1W == 0)
        {
          RG1W = strdat.gfxstrw2; // TODO: gets extended w/o conversion now
          RG1L = -RG1L;
        }
        //m_45660:
        RG1W += strdat.gfxstrw4;
        RG54l = signedlong(RG4W);
        RG4L++;
        RG52t = tp_00 + RG54l;
        RG6l = signedlong(RG2W);
        RG2L++;
        RG3Blo = *RG52t;
        *(RG7t + RG6l) = RG3Blo;
        continue;
      }
      //m_45637:
      RG3L = signedlong(vw_08);
      if (RG3L != lcon(0xffffffff))
      {
        RG1L = unsignedlong(vw_08) + 1;
        *wp_04 = RG1W;
        *(RG7t + signedlong(vw_14)) = '\0';
        return vl_0c;
      }
    }
    break;
  }
  //m_455F0:
  *wp_04 = RG4W;
  return RG1L;
}

void DM2_gfxstr_24a5_0732(i32 eaxl, i32 edxl, t_text* ebxtp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2T = ebxtp;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  i16 RG6w;
  t_text tarr_00[0xcc];
  i16 vw_cc;
  i16 vw_d0;

  RG6w = RG1W;
  RG5w = RG4W;
  RG3P = RG2P;
  RG4L = 0;
  vw_cc = RG4W; // adj
  RG2P = ADRESSOF(i16, &vw_cc);
  RG4P = ADRESSOF(i16, &vw_d0);
  RG1L = DM2_QUERY_STR_METRICS(RG3T, RG4P16, RG2P16) ? 1 : 0;
  if (RG1L == 0)
    return;
  RG4L = 0;
  //m_291D5:
  for (;;)
  {
    RG2L = unsignedlong(RG4W);
    RG1Blo = byte_at(RG3P, RG2L);
    if (RG1Blo == 0)
      break;
    RG2L = unsignedlong(RG1Blo);
    if (RG2L < lcon(0x41) || RG2L > lcon(0x5a))
    {
      //m_291F3:
      RG2L = unsignedlong(RG1Blo);
      if (RG2L >= lcon(0x7b))
        RG1Blo -= lcon(0x60);
    }
    else
      RG1Blo -= lcon(0x40);
    //m_291FE:
    RG2L = unsignedlong(RG4W);
    tarr_00[RG2L] = RG1Blo;
    RG4L++;
  }
  //m_29209:
  tarr_00[RG2L] = RG1Blo;
  RG2L = unsignedlong(palettecolor_to_ui8(paldat.palette[E_COL00]));
  RG4L = signedlong(RG5w);
  RG1W = vw_d0; // adj
  RG1L <<= bcon(0x10);
  RG1L >>= bcon(0x11);
  RG3L = signedlong(RG6w) - RG1L;
  RG1L = signedlong(RG3W);
  DM2_DRAW_VP_STR(RG1W, RG4W, RG2W, tarr_00);
}

// was SKW_3929_09fb
void DM2_DISPLAY_HINT_TEXT(i32 eaxl, t_text* edxtp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4T = edxtp;
  i8* RG5p;
  i16 RG61w;
  i16 RG62w;
  t_text tarr_00[0x64];
  i16 vw_64;

  RG61w = RG1W;
  RG5p = RG4P;
  if (ddat.v1e1420 != 0)
  {
    DM2_SCROLLBOX_MESSAGE();
    RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL00]);
    RG4L = unsignedlong(RG4Blo);
    DM2_FILL_ENTIRE_PICT(gfxsys.pictbuff, ui8_to_pixel(RG4UBlo));
    RG2L = 0;
    ddat.v1e1420 = RG2W;
  }
  RG62w = unsignedword(palettecolor_to_ui8(paldat.palette[RG61w]));
  RG3L = 0;
  vw_64 = RG3W;
  //m_45AA0:
  for (;;)
  {
    RG1L = signedlong(vw_64);
    if (byte_at(RG5p + RG1L) == 0)
      break;
    RG1W = ddat.v1d2744 - ddat.v1d70ea;
    RG3L = signedlong(RG1W);
    RG2P = ADRESSOF(i16, &vw_64);
    RG4P = ADRESSOF(t_text, tarr_00);
    RG1L = DM2_gfxstr_3929_04e2(UPCAST(t_text, RG5p), RG4T, RG2P16, RG3L);
    RG2L = RG1L;
    RG1P = RG5p + signedlong(vw_64);
    RG4L = unsignedlong(byte_at(RG1P));
    if (RG4L == lcon(0xa))
    {
      DM2_gfxmain_3929_0914();
      DM2_gfxmain_3929_0929(RG62w, tarr_00);
      RG4L = 0;
      ddat.v1d70ea = RG4W;
      vw_64++;
      continue;
    }
    //m_45B03:
    if (RG4Blo != 0)
    {
      DM2_gfxmain_3929_0929(RG62w, tarr_00);
      DM2_gfxmain_3929_0914();
      ddat.v1d70ea = lcon(0xc);
      continue;
    }
    //m_45B26:
    DM2_gfxmain_3929_0929(RG62w, tarr_00);
    ddat.v1d70ea += RG2W;
  }
  //m_45B3E:
  if (ddat.v1e1420 != 0)
  {
    DM2_SCROLLBOX_MESSAGE();
    DM2_FILL_ENTIRE_PICT(gfxsys.pictbuff, palettecolor_to_pixel(paldat.palette[E_COL00]));
    ddat.v1e1420 = wcon(0x0);
  }
}

// was DM2_gfxmain_44c8_1c99, TODO: tell SPX new name
void DM2_SCROLLBOX_MESSAGE(void)
{
  c_rect blitrect;

  DM2_HIDE_MOUSE();
  DM2_QUERY_EXPANDED_RECT(15, &blitrect);
  blitter.blit_within_screen(gfxsys.dm2screen, &blitrect, ddat.v1d2746);
  blitrect.y += blitrect.h - strdat.gfxstrw1;
  blitrect.h = strdat.gfxstrw1;

  blit_toscreen(
    PIXELCAST(gfxsys.pictbuff),
    &blitrect,
    0,
    0,
    /*TAG srcw*/ ddat.v1d2744,
    NOALPHA,
    BLITMODE0,
    BPP_8,
    BPP_8);

  DM2_SHOW_MOUSE();
}
