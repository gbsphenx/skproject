#include "util.h"
#include "calls.h"
#include "dm2data.h"
#include "c_alloc.h"
#include "c_gfx_blit.h"
#include "c_gfx_fill.h"
#include "c_gfx_str.h"

// needs:
t_gfxdata* SKW_3e74_5817(x16 eaxw);
void SKW_0b36_0d67(c_c5rects* eaxps, c_rect* edxr);

// ########################### c_font ################################

c_font C_font;

void c_font::init(void)
{
  int i = 0;
  fontptr = NULL;
  for ( i=0; i<sizeof(gfxletter); i++) gfxletter[i] = 0;
  for ( i=0; i<sizeof(unknown); i++) unknown[i] = 0;
}

void c_font::allocate(void)
{
  fontptr = DM2_ALLOC_FREEPOOL_MEMORY(0x300);
  SKW_LOAD_GDAT_ENTRY_DATA_TO(1, 7, 0, 0, fontptr);
}

// fontptr[] is 6*128 bytes - character needs 6 lines
// so graphics are organized that way:
// 1st line of all characters, 2nd line of ... etc.
//                                   eax         ebx        edx
// 128 characters available

t_gfxdata* c_font::SKW_QUERY_GFXLETTER(ui8 charidx, ui8 cmask1, ui8 cmask2)
{
  x16 idx = 0;
  for (x16 i = 0; i < 6; i++) // characterheight = 6 lines
  {
    x8 mask = fontptr[128 * i + unsignedlong(charidx)];

    for (x16 j = 0; j < 3; j++) // characterwidth = 3 bytes
    {
      x8 pattern = (mask & 0x10) == 0 ? (cmask1 << 4) : (cmask2 << 4);
      if ((mask & 8) == 0)
        pattern |= cmask1;
      else
        pattern |= cmask2;
      mask *= 4;
      gfxletter[idx++] = pattern;
    }
  }
  return gfxletter;
}

// ################### c_string_metrics ##############################

c_string_metrics C_strmetrics;

void c_string_metrics::init(void)
{
  height = 6;
  xofs = 1;
  yofs = 1;
  charwidth = 6;
}

// twidth/theight aren't valid if routine returns false
// theight: ebx
// twidth: edx
bool c_string_metrics::SKW_QUERY_STR_METRICS(const t_text* eaxt, OO x16& theight, OO x16& twidth)
{
  if (eaxt == NULL)
    return false;
  i16 th = height - yofs;
  i16 tw = -xofs;
  while (*eaxt++ != NULLBYTE)
    tw += charwidth;
  if (tw <= 0)
    return false;
  theight = th;
  twidth = tw;
  return true;
}

// #########################################################
// #########################################################
// #########################################################

// stacksize was 0x2c
// destbmp = eax
// destbits = argw3
// routine had destbits as parameter, but is always called with the same value!
void SKW_DRAW_STRING(t_gfxdata* destbmp, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, const t_text* argpt2, e_bitres destbits)
{
  t_palette palette[PAL16]; // was vcarr_00
  c_rect rc_10;
  x16 vw_18;
  x16 vw_1c;

  vw_1c = edxw;
  if (*argpt2 != NULLBYTE)
  {
    if ((argw1 & con(0x4000)) == con(0x0))
      vw_18 = con(0xffffffff);
    else
    {
      argw1 &= con(0xffffbfff);
      vw_18 = argw1;
    }
    rc_10.x = ebxw;
    if (destbits != E_BITRES4)
    {
      palette[0x0] = CUTX8(argw0);
      palette[0x1] = CUTX8(argw1);
      argw0 = con(0x0);
      argw1 = con(0x1);
      if (vw_18 != con(0xffffffff))
        vw_18 = con(0x1);
    }
    rc_10.h = C_strmetrics.height;
    ecxw += C_strmetrics.yofs - (rc_10.h - 1);
    rc_10.y = ecxw;
    rc_10.w = C_strmetrics.charwidth;
    do
    {
//                               srcbmp                charidx     cmask1         cmask2           srcofs destbmp                                            srcbits    destbits   palette
      SKW_FIRE_BLIT_PICTURE(C_font.SKW_QUERY_GFXLETTER(*argpt2, CUTX8(argw1), CUTX8(argw0)), &rc_10, 0, destbmp, con(0x0), con(0x5), vw_1c, vw_18, con(0x0), E_BITRES4, destbits, palette);
      rc_10.x += C_strmetrics.charwidth;
    } while (*(++argpt2) != NULLBYTE);
  }
}

// stacksize was 0x2c
void SKW_DRAW_STRONG_TEXT(t_gfxdata* eaxpg, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, t_text* argpt2)
{
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_18;

  vw_18 = edxw;
  vw_10 = ebxw;
  x16 vw_14 = ecxw;
  if (C_strmetrics.SKW_QUERY_STR_METRICS(argpt2, OO vw_08, OO vw_0c))
  {
    if ((argw1 & con(0x4000)) == con(0x0))
    {
      x32 longrg72 = signedlong(vw_18);
      x32 longrg62 = unsignedlong(argw1);
      vw_04 = vw_10 - 1;
      c_rect* rg1 = SKW_ALLOC_TEMP_RECT(vw_04, vw_14 - vw_08, vw_0c + con(0x2), vw_08 + con(0x2));
      SKW_FIRE_FILL_RECT_ANY(eaxpg, CUTLX8(longrg62), CUTX16(longrg72), rg1, con(0x8));
    }
    x16 vw_00 = vw_14 + con(0x1);
//                 destbmp
    SKW_DRAW_STRING(eaxpg, vw_10, vw_00, vw_18, unsignedword(ddata.paletteptr1[0]), unsignedword(ddata.paletteptr1[con(0xf)]) | con(0x4000), argpt2);
//                 destbmp
    SKW_DRAW_STRING(eaxpg, vw_10 + con(0x1), vw_00, vw_18, unsignedword(ddata.paletteptr1[0]), unsignedword(ddata.paletteptr1[con(0xf)]) | con(0x4000), argpt2);
//                 destbmp
    SKW_DRAW_STRING(eaxpg, vw_10, vw_14, vw_18, argw0, argw1 | con(0x4000), argpt2);
  }
}

// stacklen was 0x24
void SKW_DRAW_SIMPLE_STR(c_c5rects* eaxps, x16 ebxw, x16 ecxw, x16 edxw, t_text* argpt0)
{
  c_c5rects* esips;
  c_rect rc_00;
  x16 vw_08;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_14;

  esips = eaxps;
  vw_10 = ebxw;
  vw_0c = ecxw;
  if (C_strmetrics.SKW_QUERY_STR_METRICS(argpt0, OO vw_14, OO vw_08))
    if (SKW_QUERY_BLIT_RECT(NULL, edxw, &vw_08, &rc_00, &vw_14, con(0xffffffff)) != NULL)
    {
//                             destbmp
      SKW_DRAW_STRING(SKW_3e74_5817(esips->s_00.w_00), rc_00.x - esips->s_00.rc_02.x, rc_00.y + rc_00.h - 1 - esips->s_00.rc_02.y, esips->s_00.rc_02.w, vw_10, vw_0c, argpt0); // BUGFIX -spellbox - error
      SKW_0b36_0d67(esips, &rc_00);
    }
}

// stacklen was 0x24
void SKW_DRAW_NAME_STR(c_c5rects* eaxps, x16 ebxw, x16 ecxw, x16 edxw, t_text* argpt0)
{
  c_c5rects* esips;
  c_rect rc_00;
  x16 vw_08;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_14;

  esips = eaxps;
  vw_10 = ebxw;
  vw_0c = ecxw;
  if (C_strmetrics.SKW_QUERY_STR_METRICS(argpt0, OO vw_14, OO vw_08))
    if (SKW_QUERY_BLIT_RECT(NULL, edxw, &vw_08, &rc_00, &vw_14, con(0xffffffff)) != NULL)
    {
      SKW_DRAW_STRONG_TEXT(SKW_3e74_5817(esips->s_00.w_00), rc_00.x - esips->s_00.rc_02.x, rc_00.y + rc_00.h - 1 - esips->s_00.rc_02.y, esips->s_00.rc_02.w, vw_10, vw_0c, argpt0);
      SKW_0b36_0d67(esips, &rc_00);
    }
}

void SKW_DRAW_VP_STR(x16 eaxw, x16 ebxw, const t_text* ecxt, x16 edxw)
{
//                   destbmp
  SKW_DRAW_STRING(ddata.bitmapptr, eaxw, edxw, getprevious(ddata.bitmapptr)->w2, ebxw, unsignedword(ddata.paletteptr1[con(0xc)]) | con(0x4000), ecxt);
}

void SKW_DRAW_GUIDED_STR(const t_text* eaxt)
{
  t_text t_00[0x80];

  if (*eaxt == con(0xc))
  {
    eaxt++;
    SKW_QUERY_TOPLEFT_OF_RECT(con(0x22c), &ddata.stry, &ddata.strx);
  }
  if (*eaxt != NULLBYTE)
  {
    SKW_SK_STRCPY(t_00, eaxt);
    t_text* ptrrg6 = t_00;
    bool boolrg7 = false;
    t_text* tptr;
    while (*ptrrg6 != NULLBYTE)
    {
      if (SKW_SK_STRLEN(ptrrg6) <= con(0x12))
        tptr = NULL;
      else
      {
        tptr = ptrrg6 + con(0x11);
        while (*tptr != ' ')
          tptr--;
        *tptr = NULLBYTE;
        boolrg7 = true;
      }
      SKW_DRAW_VP_STR(ddata.stry, unsignedword(ddata.paletteptr1[con(0xd)]), ptrrg6, ddata.strx);
      ddata.strx += ddata.strxplus;
      if (!boolrg7 || tptr == NULL)
        *ptrrg6 = NULLBYTE;
      else
      {
        boolrg7 = false;
        ptrrg6 = tptr + con(0x1);
      }
    }
  }
}

void SKW_PRINT_SYSERR_TEXT(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, t_text* argpt0)
{
//                     destbmp
  SKW_DRAW_STRING(ddata.screenptr, eaxw, edxw, ddata.mouse_unk0c, ebxw, ecxw, argpt0);
  printf("(error)\n");
}

// TODO: try last argument x8
void SKW_DRAW_VP_RC_STR(x16 eaxw, const t_text* ebxt, x16 edxw)
{
  c_rect rc_00;
  x16 vw_08;
  x16 vw_0c;
  x16* parpw00;

  if (C_strmetrics.SKW_QUERY_STR_METRICS(ebxt, OO vw_0c, OO vw_08))
  {
    parpw00 = &vw_0c;
    if (SKW_QUERY_BLIT_RECT(NULL, eaxw, &vw_08, &rc_00, parpw00, con(0xffffffff)) != NULL)
      SKW_DRAW_VP_STR(rc_00.x, edxw, ebxt, rc_00.y + rc_00.h - 1);
  }
}

void SKW_DRAW_LOCAL_TEXT(x16 eaxw, x16 ebxw, t_text* ecxt, x16 edxw)
{
  c_rect rc_08;
  x16 vw_10;
  x16 vw_14;

  if (C_strmetrics.SKW_QUERY_STR_METRICS(ecxt, OO vw_14, OO vw_10))
    if (SKW_QUERY_BLIT_RECT(NULL, eaxw, &vw_10, &rc_08, &vw_14, con(0xffffffff)) != NULL)
      SKW_DRAW_STRONG_TEXT(ddata.bitmapptr, rc_08.x, rc_08.y + rc_08.h - 1, getprevious(ddata.bitmapptr)->w2, edxw, ebxw, ecxt);
}
