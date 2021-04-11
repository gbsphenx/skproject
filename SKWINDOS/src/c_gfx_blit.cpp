#include "../dm2global.h" // VGABASE, ORIG_SWIDTH
#include "startend.h" // THROW...
#include "util.h"
#include "dm2data.h"
#include "c_gfx_blit.h"

// t_palette: eax
void SKW_FIRE_UPDATE_BLIT_PALETTE(t_palette* palette)
{
  ddata.paletteptr2 = palette;
}

// srcofs = eax
void SKW_44c8_08ae(x16 srcofs, x32 ebxl, x8 ecxb, x16 edxw)
{
  x32 vl_08;

  x16 vw_04 = edxw;
  x32 vl_00 = ebxl;
  x8 vb_10 = ecxb;
  x16 vw_0c = srcofs / 2;
  srcofs &= con(0x1);
  if (srcofs != con(0x0))
  {
    t_gfxdata byterg14 = ddata.srcbmp[vw_0c] & con(0xf);
    if (byterg14 != ecxb)
    {
      t_gfxdata* ptrrg4 = ddata.paletteptr2 + byterg14;
      ddata.destbmp[vw_04] = ptrrg4[0];
    }
    vw_0c++;
    vw_04++;
    vl_00--;
  }
  x16 wordrg11 = CUTX16(vl_00) / 2;
  vl_08 = lextended(wordrg11);
  if (wordrg11 != con(0x0))
  {
    x32 longrg3 = unsignedlong(wordrg11);
    t_gfxdata* ptrrg2 = ddata.paletteptr2;
    t_gfxdata* ptrrg6 = ddata.destbmp + vw_04;
    t_gfxdata* ptrrg5 = ddata.srcbmp + vw_0c;
    t_gfxdata byterg11 = vb_10 & con(0xf);
    t_gfxdata byterg41 = (byterg11 << con(0x4)) | byterg11;

    do
    {
      t_gfxdata byterg13 = *ptrrg5++;
      if (byterg13 != byterg41)
      {
        t_gfxdata byterg42 = byterg13;
        byterg13 >>= con(0x4);
        byterg13 &= con(0xf);
        if (byterg13 == byterg11)
          ptrrg6++;
        else
          *ptrrg6++ = xlat8(ptrrg2, byterg13);
        t_gfxdata byterg12 = byterg42 & con(0xf);
        if (byterg12 == byterg11)
          ptrrg6++;
        else
          *ptrrg6++ = xlat8(ptrrg2, byterg12);
      }
      else
        ptrrg6 += con(0x2);
    } while (--longrg3 != 0);

    vw_0c += CUTX16(vl_08);
    vw_04 += 2 * CUTX16(vl_08);
    vl_00 &= con(0xffff0001);
  }
  if (CUTX16(vl_00) != con(0x0))
  {
    x8 byterg15 = CUTLX8(unsignedlong(ddata.srcbmp[vw_0c]) >> con(0x4));
    if (byterg15 != vb_10)
      ddata.destbmp[vw_04] = ddata.paletteptr2[byterg15];
  }
}

// was SKW_44c8_0b8d
void DM2_blit8pppixel(ui16 srcofs, x16 width, ui16 destofs)
{
  t_gfxdata* src = ddata.srcbmp + srcofs;
  t_gfxdata* dest = ddata.destbmp + destofs;
  while (width--)
    *dest++ = *src++;
}

// was SKW_44c8_0bc5
static void DM2_blit8pppixelmasked(ui16 srcofs, ui16 width, t_gfxdata mask, ui16 dstofs)
{
  t_gfxdata* src = ddata.srcbmp + srcofs;
  t_gfxdata* dest = ddata.destbmp + dstofs;
  do
  {
    t_gfxdata pixel = *src++;
    if (pixel == mask)
      dest++;
    else
      *dest++ = pixel;
  } while (--width != 0);
}

// was SKW_44c8_0bf8
static void DM2_blit8pppixelmirrored(ui16 srcofs, ui16 width, ui16 dstofs)
{
  t_gfxdata* src = ddata.srcbmp + srcofs + width - 1;
  t_gfxdata* dest = ddata.destbmp + dstofs;
  do
  {
    *dest++ = *src--;
  } while (--width != 0);
}

// was SKW_44c8_0c3c
static void DM2_blit8pppixelmirroredmasked(ui16 srcofs, ui16 width, t_gfxdata mask, ui16 dstofs)
{
  t_gfxdata* src = ddata.srcbmp + srcofs + width - 1;
  t_gfxdata* dest = ddata.destbmp + dstofs;
  do
  {
    x8 pixel = *src--;
    if (pixel == mask)
      dest++;
    else
      *dest++ = pixel;
  } while (--width != 0);
}

// srcofs = eaxw
static void R_1761(x16 srcofs, x16 ebxw, x16 edxw)
{
  x32 vl_00;
  x32 vl_04;
  x8 vb_08;

  x16 wordrg43 = edxw;
  x16 wordrg3 = srcofs + ebxw - 1;
  x32 longrg7 = UI32CAST(wordrg3 & con(0x1));
  x32 longrg1 = UI32CAST(wordrg43 & con(0x1));
  vl_00 = lextended(wordrg43 + 1);
  x32 longrg6 = lextended(ebxw) - 1; // TODO: make it word
  vl_04 = lextended(wordrg3) - 1;
  if (longrg7 != longrg1)
  {
    if (longrg1 != con(0x0))
    {
      x16 wordrg13 = wordrg43 / 2;
      x32 longrg22 = unsignedlong(wordrg3) / 2;
      wordrg43 = CUTX16(vl_00);
      wordrg3 = CUTX16(vl_04);
      x32 longrg21 = unsignedlong(ddata.srcbmp[longrg22]) >> 4;
      x32 longrg55 = unsignedlong(wordrg13);
      t_gfxdata* ptrrg54 = ddata.destbmp + longrg55;
      *ptrrg54 = CUTLX8(UI32CAST(*ptrrg54 & con(0xfffffff0)) | longrg21);
      ebxw = CUTX16(longrg6);
    }
    x16 wordrg12 = wordrg43 / 2;
    x16 wordrg42 = wordrg3 / 2;
    while ((wordrg3 = ebxw) > con(0x1))
    {
      x32 longrg53 = unsignedlong(ddata.srcbmp[unsignedlong(wordrg42)]);
      x32 longrg3 = 16 * longrg53;
      longrg53 >>= con(0x4);
      longrg3 |= longrg53;
      t_gfxdata* ptrrg62 = ddata.destbmp;
      ptrrg62[unsignedlong(wordrg12++)] = CUTLX8(longrg3);
      wordrg42--;
      ebxw -= con(0x2);
    }
    if (ebxw <= con(0x0))
      return;
    t_gfxdata* ptrrg32 = ddata.destbmp + unsignedlong(wordrg12);
    *ptrrg32 = (ddata.srcbmp[unsignedlong(wordrg42)] << con(0x4)) | (*ptrrg32 & con(0xf));
    return;
  }
  else if (longrg1 != con(0x0))
  {
    x16 wordrg11 = wordrg43 / 2;
    x32 longrg51 = lextended(wordrg3 / 2);
    wordrg43 = CUTX16(vl_00);
    wordrg3 = CUTX16(vl_04);
    t_gfxdata* ptrrg52 = ddata.destbmp + unsignedlong(wordrg11);
    *ptrrg52 = (*ptrrg52 & con(0xfffffff0)) | (ddata.srcbmp[longrg51] & con(0xf));
    ebxw = CUTX16(longrg6);
  }
  x16 wordrg1 = wordrg43 / 2;
  x16 wordrg41 = wordrg3 / 2;
  while (ebxw > con(0x1))
  {
    t_gfxdata* ptrrg51 = ddata.srcbmp + unsignedlong(wordrg41);
    t_gfxdata* ptrrg52 = ddata.srcbmp + unsignedlong(wordrg41) - 1;
    vb_08 = *ptrrg52 & con(0xf);
    ddata.destbmp[unsignedlong(wordrg1++)] = (*ptrrg51 & con(0xfffffff0)) | vb_08;
    wordrg41--;
    ebxw -= con(0x2);
  }
  if (ebxw <= con(0x0))
    return;
  t_gfxdata* ptrrg31 = ddata.destbmp + unsignedlong(wordrg1);
  *ptrrg31 = (ddata.srcbmp[unsignedlong(wordrg41)] & con(0xfffffff0)) | (*ptrrg31 & con(0xf));
}

// srcofs = eaxw
static void R_190F(x16 srcofs, x16 ebxw, x8 ecxb, x16 edxw)
{
  x8 vb_04;
  x8 vb_08;
  x8 vb_0c;

  x16 wordrg48 = edxw;
  x16 vw_00 = srcofs;
  x16 wordrg5 = wordrg48;
  vb_04 = ecxb;
  xxx* ptrrg6 = ddata.blitb + (unsignedlong(ecxb) << con(0x8));
  x16 wordrg3 = vw_00 & con(0x1);
  x32 longrg47 = UI32CAST(wordrg48 & con(0x1));
  x16 wordrg1 = vw_00 + 1;
  if (wordrg3 == CUTX16(longrg47))
  {
    if (longrg47 != con(0x0))
    {
      x32 longrg46 = unsignedlong(vw_00) / 2;
      vw_00 = wordrg1;
      x8 byterg14 = ddata.srcbmp[longrg46] & con(0xf);
      if (byterg14 != vb_04)
      {
        wordrg3 = unsignedword(CUTX16(unsignedlong(wordrg5) / 2));
        byterg14 |= ddata.destbmp[wordrg3] & con(0xfffffff0);
        ddata.destbmp[wordrg3] = byterg14;
      }
      wordrg5++;
      ebxw--;
    }
    x16 wordrg44 = wordrg5 / 2;
    x16 wordrg31 = vw_00 / 2;
    while (ebxw > con(0x1))
    {
      x8 byterg13 = ddata.srcbmp[unsignedlong(wordrg31++)];
      vb_08 = ptrrg6[unsignedlong(byterg13)];
      t_gfxdata* ptrrg52 = ddata.destbmp + unsignedlong(wordrg44);
      vb_0c = *ptrrg52 & vb_08;
      byterg13 &= ~vb_08;
      byterg13 |= vb_0c;
      *ptrrg52 = byterg13;
      wordrg44++;
      ebxw -= con(0x2);
    }
    if (ebxw > con(0x0))
    {
      x8 byterg12 = ddata.srcbmp[unsignedlong(wordrg31)] & con(0xfffffff0);
      x32 longrg6 = unsignedlong(byterg12);
      if (longrg6 != (unsignedlong(vb_04) << con(0x4)))
      {
        x32 longrg53 = unsignedlong(wordrg44);
        byterg12 |= ddata.destbmp[longrg53] & con(0xf);
        ddata.destbmp[longrg53] = byterg12;
      }
    }
    return;
  }
  else if (longrg47 != con(0x0))
  {
    x32 longrg42 = unsignedlong(vw_00) / 2;
    t_gfxdata* ptrrg32 = ddata.srcbmp;
    vw_00 = wordrg1;
    x8 byterg15 = CUTLX8(unsignedlong(ptrrg32[longrg42]) >> con(0x4));
    if (byterg15 != vb_04)
    {
      x32 longrg41 = (unsignedlong(wordrg5) / 2) & con(0xffff);
      t_gfxdata* ptrrg31 = ddata.destbmp;
      byterg15 |= ptrrg31[longrg41] & con(0xfffffff0);
      ptrrg31[longrg41] = byterg15;
    }
    wordrg5++;
    ebxw--;
  }
  x16 wordrg41 = wordrg5 / 2;
  x16 wordrg31 = vw_00 / 2;
  while (ebxw > con(0x1))
  {
    x32 longrg52 = unsignedlong(ddata.srcbmp[unsignedlong(wordrg31) + con(0x1)]) >> 4;
    x8 byterg16 = CUTLX8((unsignedlong(ddata.srcbmp[unsignedlong(wordrg31)]) << con(0x4)) | longrg52); // BUGFIX SQUADICON 1/2

    vb_08 = ptrrg6[unsignedlong(byterg16)];
    t_gfxdata* ptrrg51 = ddata.destbmp + unsignedlong(wordrg41);
    vb_0c = *ptrrg51 & vb_08;
    byterg16 &= ~vb_08; // BUGFIX SQUADICON 2/2
    byterg16 |= vb_0c;
    *ptrrg51 = byterg16;
    wordrg41++;
    wordrg31++;
    ebxw -= con(0x2);
  }
  if (ebxw > con(0x0))
  {
    x8 byterg11 = ddata.srcbmp[unsignedlong(wordrg3)] & con(0xf);
    if (byterg11 != vb_04)
    {
      x32 longrg51 = unsignedlong(wordrg41);
      byterg11 <<= con(0x4);
      byterg11 |= ddata.destbmp[longrg51] & con(0xf);
      ddata.destbmp[longrg51] = byterg11;
    }
  }
}

// srcofs = eax
void R_15B5(x16 srcofs, x16 ebxw, x16 edxw)
{
  x16 vw_00;
  x32 vl_04;
  x8 vb_08;

  x16 wordrg43 = edxw;
  x16 wordrg3 = srcofs;
  srcofs &= con(0x1);
  x32 longrg7 = unsignedlong(srcofs);
  x32 longrg1 = UI32CAST(wordrg43 & 0x1);
  x32 longrg52 = lextended(wordrg3 + 1);
  vl_04 = lextended(wordrg43 + 1);
  x32 longrg2 = lextended(ebxw - 1); // TODO: make it x16
  if (longrg7 != longrg1)
  {
    if (longrg1 != con(0x0))
    {
      x16 wordrg13 = wordrg43 / 2;
      x32 longrg22 = unsignedlong(wordrg3) / 2;
      t_gfxdata* ptrrg71 = ddata.srcbmp;
      wordrg43 = CUTX16(vl_04);
      wordrg3 = CUTX16(longrg52);
      x32 longrg23 = unsignedlong(ptrrg71[longrg22]) >> con(0x4);
      x32 longrg53 = unsignedlong(wordrg13);
      t_gfxdata* ptrrg54 = ddata.destbmp + longrg53;
      *ptrrg54 = CUTLX8(UI32CAST(*ptrrg54 & con(0xfffffff0)) | longrg23);
      ebxw = CUTX16(longrg2);
    }
    x16 wordrg12 = wordrg43 / 2;
    x16 wordrg42 = wordrg3 / 2;
    while ((wordrg3 = ebxw) > con(0x1))
    {
      t_gfxdata* ptrrg32 = ddata.srcbmp + unsignedlong(wordrg42);
      x32 longrg51 = unsignedlong(ptrrg32[con(0x1)]) >> 4;
      vw_00 = CUTX16((unsignedlong(*ptrrg32) << con(0x4)) | longrg51);
      t_gfxdata* ptrrg62 = ddata.destbmp;
      ptrrg62[unsignedlong(wordrg12++)] = CUTX8(vw_00);
      wordrg42++;
      ebxw -= con(0x2);
    }
    if (ebxw <= con(0x0))
      return;
    t_gfxdata* ptrrg31 = ddata.destbmp + unsignedlong(wordrg12);
    vb_08 = *ptrrg31 & con(0xf);
    *ptrrg31 = ((ddata.srcbmp[unsignedlong(wordrg42)] << con(0x4)) | vb_08);
    return;
  }
  else if (longrg1 != con(0x0))
  {
    x16 wordrg11 = wordrg43 / 2;
    x32 longrg21 = unsignedlong(wordrg3) / 2;
    t_gfxdata* ptrrg72 = ddata.srcbmp;
    wordrg43 = CUTX16(vl_04);
    wordrg3 = CUTX16(longrg52);
    t_gfxdata* ptrrg52 = ddata.destbmp + unsignedlong(wordrg11);
    *ptrrg52 = (*ptrrg52 & con(0xfffffff0)) | (ptrrg72[longrg21] & con(0xf));
    ebxw = CUTX16(longrg2);
  }
  x16 wordrg1 = wordrg43 / 2;
  x16 wordrg41 = wordrg3 / 2;
  while (ebxw > con(0x1))
  {
    t_gfxdata* ptrrg51 = ddata.srcbmp + unsignedlong(wordrg41++);
    x32 longrg1 = unsignedlong(wordrg1++);
    ddata.destbmp[longrg1] = *ptrrg51;
    ebxw -= con(0x2);
  }
  if (ebxw <= con(0x0))
    return;
  t_gfxdata* ptrrg31 = ddata.destbmp + unsignedlong(wordrg1);
  *ptrrg31 = (ddata.srcbmp[unsignedlong(wordrg41)] & con(0xfffffff0)) | (*ptrrg31 & con(0xf));
}

// srcofs = eax
static void R_2035(x16 srcofs, x16 ebxw, x16 edxw)
{
  x16 wordrg4 = edxw + ebxw - 1;
  if ((srcofs & con(0x1)) != con(0x0))
  {
    t_gfxdata* ptrrg64 = ddata.paletteptr2 + UI32CAST(ddata.srcbmp[unsignedlong(srcofs++) / 2] & con(0xf));
    t_gfxdata* ptrrg53 = ddata.destbmp + unsignedlong(wordrg4--);
    *ptrrg53 = *ptrrg64;
    ebxw--;
  }
  x16 wordrg1 = unsignedlong(srcofs) / 2;
  while (ebxw >= con(0x2))
  {
    t_gfxdata* ptrrg63 = ddata.srcbmp;
    x8 byterg3 = ptrrg63[unsignedlong(wordrg1++)];
    t_gfxdata* ptrrg52 = ddata.paletteptr2 + (unsignedlong(byterg3) >> 4);
    x32 longrg62 = unsignedlong(wordrg4);
    t_gfxdata* ptrrg7 = ddata.destbmp;
    wordrg4--;
    t_gfxdata* ptrrg62 = ptrrg7 + longrg62;
    *ptrrg62 = *ptrrg52;
    byterg3 &= con(0xf);
    t_gfxdata* ptrrg61 = ddata.paletteptr2 + unsignedlong(byterg3);
    t_gfxdata* ptrrg51 = ddata.destbmp + unsignedlong(wordrg4--);
    *ptrrg51 = *ptrrg61;
    ebxw -= con(0x2);
  }
  if (ebxw != con(0x0))
  {
    x32 longrg61 = unsignedlong(wordrg4);
    ddata.destbmp[longrg61] = ddata.paletteptr2[(ddata.srcbmp[unsignedlong(wordrg1)] & con(0xff)) >> 4];
  }
}

// srcofs = eax
static void R_291B(x16 srcofs, x16 ebxw, t_gfxdata ecxb, x16 edxw, t_gfxdata* argpg0)
{
  t_gfxdata* ptrrg5 = ddata.srcbmp + unsignedlong(srcofs) + unsignedlong(ebxw) - 1;
  t_gfxdata* ptrrg6 = ddata.destbmp + unsignedlong(edxw);
  x32 longrg3 = unsignedlong(ebxw);
  do
  {
    t_gfxdata byterg1 = *ptrrg5--;
    if (byterg1 == ecxb)
      ptrrg6++;
    else
      *ptrrg6++ = xlat8(argpg0, byterg1);
  } while (--longrg3 != 0);
}

// srcofs = eax
static void R_28DF(x16 srcofs, x16 ebxw, t_gfxdata* ecxpg, x16 edxw)
{
  t_gfxdata* ptrrg5 = ddata.srcbmp + unsignedlong(srcofs) + unsignedlong(ebxw) - 1;
  t_gfxdata* ptrrg6 = ddata.destbmp + unsignedlong(edxw);
  x32 longrg3 = unsignedlong(ebxw);
  do
  {
    *ptrrg6++ = xlat8(ecxpg, *ptrrg5--);
  } while (--longrg3 != 0);
}

// srcofs = eax
static void R_28A2(x16 srcofs, x16 ebxw, t_gfxdata ecxb, x16 edxw, t_gfxdata* argpg0)
{
  t_gfxdata* ptrrg5 = ddata.srcbmp + unsignedlong(srcofs);
  t_gfxdata* ptrrg6 = ddata.destbmp + unsignedlong(edxw);
  x32 longrg3 = unsignedlong(ebxw);
  do
  {
    t_gfxdata byterg1 = *ptrrg5++;
    if (byterg1 == ecxb)
      ptrrg6++;
    else
      *ptrrg6++ = xlat8(argpg0, byterg1);
  } while (--longrg3 != 0);
}

// translates a given number of colors by coltable
// routine belongs to SKW_44c8_0f29
// moved parameter 4 to position 2
static void R_2871(ui16 srcofs, ui16 ofsdest, ui16 num, t_gfxdata* coltable)
{
  t_gfxdata* src = ddata.srcbmp + srcofs;
  t_gfxdata* dest = ddata.destbmp + ofsdest;
  do
  {
    *dest++ = coltable[*src++];
  } while (--num != 0);
}

// stacksize was 0x1c
// was SKW_44c8_0f29
// srcbmp = eax
// destbmp = edx
// srcofs = ecx
static void SKW_FIRE_BLIT_TO_MEMORY_8TO8BPP_TRANSLATED(t_gfxdata* srcbmp, c_rect* ebxr, ui16 srcofs, t_gfxdata* destbmp, x16 argw0, x16 argw1, x16 argw2, x16 argw3, x16 argw4, t_gfxdata* argpg5)
{
	x16 vl_08 = 0;

  ddata.srcbmp = srcbmp;
  ddata.destbmp = destbmp;
  if (ebxr->w == 0 || ebxr->h == 0)
    return;
  x16 wordrg6 = ebxr->y * argw2 + ebxr->x;
  if (argw4 <= 3)
  {
    x16 wordrg1 = (ebxr->h + argw0 - 1) * argw1 + srcofs;
    switch (argw4)
    {
      case 1:
        srcofs += argw0 * argw1;
        for ( vl_08 = 0; vl_08 < ebxr->h; vl_08++, srcofs += argw1, wordrg6 += argw2)
        {
          if (argw3 >= con(0x0))
            R_291B(srcofs, ebxr->w, UI8CAST(argw3), wordrg6, argpg5); // destinationtype of argw3 is t_gfxdata
          else
            R_28DF(srcofs, ebxr->w, argpg5, wordrg6);
        }
        return;

      case 2:
        srcofs = wordrg1;
        for ( vl_08 = 0; vl_08 < ebxr->h; vl_08++, srcofs -= argw1, wordrg6 += argw2)
        {
          if (argw3 >= con(0x0))
            R_28A2(srcofs, ebxr->w, UI8CAST(argw3), wordrg6, argpg5); // destinationtype of argw3 is t_gfxdata
          else
            R_2871(srcofs, wordrg6, unsignedlong(ebxr->w), argpg5);
        }
        return;

      case 3:
        srcofs = wordrg1;
        for ( vl_08 = 0; vl_08 < ebxr->h; vl_08++, srcofs -= argw1, wordrg6 += argw2)
        {
          if (argw3 >= con(0x0))
            R_291B(srcofs, ebxr->w, UI8CAST(argw3), wordrg6, argpg5); // destinationtype of argw3 is t_gfxdata
          else
            R_28DF(srcofs, ebxr->w, argpg5, wordrg6);
        }
        return;

      case 0:
      ;
    }
  }
  srcofs += argw0 * argw1;
  for ( vl_08 = 0; vl_08 < ebxr->h; vl_08++, srcofs += argw1, wordrg6 += argw2)
  {
    if (argw3 >= con(0x0))
      R_28A2(srcofs, ebxr->w, UI8CAST(argw3), wordrg6, argpg5); // destinationtype of argw3 is t_gfxdata
    else
      R_2871(srcofs, wordrg6, unsignedlong(ebxr->w), argpg5);
  }
}

// stacksize was 0x18
// srcbmp = eax
// srcofs = ecx
// destbmp = edx
static void SKW_FIRE_BLIT_TO_MEMORY_8TO8BPP(t_gfxdata* srcbmp, c_rect* ebxr, ui16 srcofs, t_gfxdata* destbmp, x16 argw0, x16 argw1, x16 argw2, x16 argw3, x16 argw4)
{
	x16 vw_08 = 0;

  ddata.srcbmp = srcbmp;
  ddata.destbmp = destbmp;
  if (ebxr->w <= 0 || ebxr->h <= con(0x0))
    return;
  x16 wordrg6 = ebxr->y * argw2 + ebxr->x;
  x16 wordrg1 = (ebxr->h + argw0 - 1) * argw1 + srcofs;
  switch (argw4)
  {
    case 1:
      srcofs += argw0 * argw1;
      for ( vw_08 = 0; vw_08 < ebxr->h; vw_08++, srcofs += argw1, wordrg6 += argw2)
      {
        if (argw3 >= con(0x0))
          DM2_blit8pppixelmirroredmasked(srcofs, ebxr->w, CUTX8(argw3), wordrg6);
        else
          DM2_blit8pppixelmirrored(srcofs, ebxr->w, wordrg6);
      }
    break;
    case 2:
      srcofs = wordrg1;
      for ( vw_08 = 0; vw_08 < ebxr->h; vw_08++, srcofs -= argw1, wordrg6 += argw2)
      {
        if (argw3 >= con(0x0))
          DM2_blit8pppixelmasked(srcofs, ebxr->w, CUTX8(argw3), wordrg6);
        else
//                                  width   destofs
          DM2_blit8pppixel(srcofs, ebxr->w, wordrg6);
      }
    break;
    case 3:
      srcofs = wordrg1;
      for ( vw_08 = 0; vw_08 < ebxr->h; vw_08++, srcofs -= argw1, wordrg6 += argw2)
      {
        if (argw3 >= con(0x0))
          DM2_blit8pppixelmirroredmasked(srcofs, ebxr->w, CUTX8(argw3), wordrg6);
        else
          DM2_blit8pppixelmirrored(srcofs, ebxr->w, wordrg6);
      }
    break;
    default:
      srcofs += argw0 * argw1;
      for ( vw_08 = 0; vw_08 < ebxr->h; vw_08++, srcofs += argw1, wordrg6 += argw2)
      {
        if (argw3 >= con(0x0))
          DM2_blit8pppixelmasked(srcofs, ebxr->w, CUTX8(argw3), wordrg6);
        else
//                                 width   destofs
          DM2_blit8pppixel(srcofs, ebxr->w, wordrg6);
      }
  }
}

// srcofs = eax
static void SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(x16 srcofs, x16 ebxw, x8 ecxb, x16 edxw)
{
  x8 vb_04;

  x16 wordrg4 = edxw;
  vb_04 = ecxb;
  wordrg4 += ebxw - 1;
  x16 wordrg5 = srcofs / 2;
  x16 wordrg31 = wordrg5;
  srcofs &= con(0x1);
  if (srcofs != con(0x0))
  {
    x8 byterg12 = ddata.srcbmp[unsignedlong(wordrg5)] & con(0xf);
    if (byterg12 != vb_04)
    {
      t_gfxdata* ptrrg64 = ddata.paletteptr2 + unsignedlong(byterg12);
      ddata.destbmp[unsignedlong(wordrg4)] = *ptrrg64;
    }
    wordrg31++;
    wordrg4--;
    ebxw--;
  }
  while (ebxw >= con(0x2))
  {
    x8 byterg11 = ddata.srcbmp[unsignedlong(wordrg31++)];
    x16 vw_00 = ubrshift(byterg11, con(0x4));
    if (CUTX8(vw_00) != vb_04)
    {
      t_gfxdata* ptrrg63 = ddata.paletteptr2 + unsignedlong(CUTX8(vw_00));
      ddata.destbmp[wordrg4] = *ptrrg63;
    }
    byterg11 &= con(0xf);
    wordrg4--;
    if (byterg11 != vb_04)
    {
      t_gfxdata* ptrrg62 = ddata.paletteptr2 + unsignedlong(byterg11);
      ddata.destbmp[unsignedlong(wordrg4)] = *ptrrg62;
    }
    wordrg4--;
    ebxw -= con(0x2);
  }
  if (ebxw == con(0x0))
    return;
  x8 byterg13 = CUTLX8(unsignedlong(ddata.srcbmp[unsignedlong(wordrg31)]) >> con(0x4));
  if (byterg13 == vb_04)
    return;
  ddata.destbmp[unsignedlong(wordrg4)] = ddata.paletteptr2[unsignedlong(byterg13)];
}

// stacksize was 0x18
// palette = argpg5
// srcbmp = eax
// destbmp = edx
// srcofs = ecx
static void SKW_FIRE_BLIT_TO_MEMORY_4TO8BPP(t_gfxdata* srcbmp, c_rect* ebxr, ui16 srcofs, t_gfxdata* destbmp, x16 argw0, x16 argw1, x16 argw2, x16 argw3, x16 argw4, t_palette* palette)
{
	x16 vl_08 = 0;

  SKW_FIRE_UPDATE_BLIT_PALETTE(palette);
  ddata.srcbmp = srcbmp;
  ddata.destbmp = destbmp;
  argw1 = (argw1 + 1) & con(0xfffffffe);
  if (ebxr->w == 0 || ebxr->h == 0)
    return;
  x16 wordrg6 = ebxr->y * argw2 + ebxr->x;
  if (argw4 <= 3)
  {
    x16 wordrg1 = (ebxr->h + argw0 - 1) * argw1 + srcofs;
    switch (argw4)
    {
      case 1:
        srcofs += argw0 * argw1;
        for ( vl_08 = 0; vl_08 < ebxr->h; vl_08++, srcofs += argw1, wordrg6 += argw2)
        {
          if (argw3 >= con(0x0))
            SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(srcofs, ebxr->w, CUTX8(argw3), wordrg6);
          else
            R_2035(srcofs, ebxr->w, wordrg6);
        }
        return;
      case 2:
        srcofs = wordrg1;
        for ( vl_08 = 0; vl_08 < ebxr->h; vl_08++, srcofs -= argw1, wordrg6 += argw2)
        {
          if (argw3 >= con(0x0))
            SKW_44c8_08ae(srcofs, unsignedlong(ebxr->w), CUTX8(argw3), wordrg6);
          else
            SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(srcofs, ebxr->w, wordrg6);
        }
        return;
      case 3:
        srcofs = wordrg1;
        for ( vl_08 = 0; vl_08 < ebxr->h; vl_08++, srcofs -= argw1, wordrg6 += argw2)
        {
          if (argw3 >= con(0x0))
            SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP(srcofs, ebxr->w, CUTX8(argw3), wordrg6);
          else
            R_2035(srcofs, ebxr->w, wordrg6);
        }
        return;
      case 0:
      ;
    }
  }
  srcofs += argw0 * argw1;
  for ( vl_08 = 0; vl_08 < ebxr->h; vl_08++, srcofs += argw1, wordrg6 += argw2)
  {
    if (argw3 >= con(0x0))
      SKW_44c8_08ae(srcofs, unsignedlong(ebxr->w), CUTX8(argw3), wordrg6);
    else
      SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(srcofs, ebxr->w, wordrg6);
  }
}

// srcofs = eax
static void SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO4BPP(x16 srcofs, x16 ebxw, x8 ecxb, x16 edxw)
{
  x8 vb_04;
  x8 vb_08;
  x8 vb_0c;

  x16 wordrg5 = srcofs;
  x16 vw_00 = edxw;
  vb_08 = ecxb;
  xxx* ptrrg6 = ddata.blitb + (unsignedlong(ecxb) << con(0x8));
  wordrg5 += ebxw - 1;
  x32 longrg3 = UI32CAST(wordrg5 & con(0x1));
  x32 longrg47 = UI32CAST(edxw & con(0x1));
  x16 wordrg1 = wordrg5 - 1;
  if (longrg3 != longrg47)
  {
    if (longrg47 != con(0x0))
    {
      x32 longrg46 = unsignedlong(wordrg5) / 2;
      wordrg5 = wordrg1;
      x8 byterg16 = CUTLX8(unsignedlong(ddata.srcbmp[longrg46]) >> con(0x4));
      if (byterg16 != vb_08)
      {
        x32 longrg45 = (unsignedlong(vw_00) / 2) & con(0xffff);
        t_gfxdata* ptrrg31 = ddata.destbmp;
        byterg16 |= ptrrg31[longrg45] & con(0xfffffff0);
        ptrrg31[longrg45] = byterg16;
      }
      vw_00++;
      ebxw--;
    }
    x16 wordrg43 = vw_00 / 2;
    x16 wordrg32 = wordrg5 / 2;
    while (ebxw > con(0x1))
    {
      x32 longrg52 = unsignedlong(ddata.srcbmp[wordrg32]);
      x16 longrg11 = 16 * longrg52;
      longrg52 >>= con(0x4);
      longrg11 |= longrg52;
      x8 byterg15 = CUTLX8(longrg11);
      vb_04 = ptrrg6[unsignedlong(byterg15)];
      t_gfxdata* ptrrg52 = ddata.destbmp + unsignedlong(wordrg43);
      vb_0c = *ptrrg52 & vb_04;
      byterg15 &= -vb_04;
      byterg15 |= vb_0c;
      wordrg43++;
      *ptrrg52 = byterg15;
      wordrg32--;
      ebxw -= con(0x2);
    }
    if (ebxw <= con(0x0))
      return;
    t_gfxdata byterg14 = ddata.srcbmp[unsignedlong(wordrg32)] & con(0xf);
    if (byterg14 == vb_08)
      return;
    x32 longrg51 = unsignedlong(wordrg43);
    t_gfxdata* ptrrg4 = ddata.destbmp;
    byterg14 <<= con(0x4);
    byterg14 |= ptrrg4[longrg51] & con(0xf);
    ptrrg4[longrg51] = byterg14;
    return;
  }
  else if (longrg47 != con(0x0))
  {
    x32 longrg42 = unsignedlong(wordrg5) / 2;
    wordrg5 = wordrg1;
    t_gfxdata byterg13 = ddata.srcbmp[longrg42] & con(0xf);
    if (byterg13 != vb_08)
    {
      x32 longrg4 = (unsignedlong(vw_00) / 2) & con(0xffff);
      t_gfxdata* ptrrg31 = ddata.destbmp;
      byterg13 |= ptrrg31[longrg4] & con(0xfffffff0);
      ptrrg31[longrg4] = byterg13;
    }
    vw_00++;
    ebxw--;
  }
  x16 wordrg4 = unsignedlong(vw_00) / 2;
  x16 wordrg31 = wordrg5 / 2;
  while (ebxw > con(0x1))
  {
    t_gfxdata* ptrrg53 = ddata.srcbmp + unsignedlong(wordrg31);
    t_gfxdata* ptrrg54 = ddata.srcbmp + unsignedlong(wordrg31) - 1;
    vb_0c = *ptrrg54 & con(0xf);
    x8 byterg12 = (*ptrrg53 & con(0xfffffff0)) | vb_0c;
    vb_04 = ptrrg6[unsignedlong(byterg12)];
    t_gfxdata* ptrrg52 = ddata.destbmp + unsignedlong(wordrg4);
    vb_0c = *ptrrg52 & vb_04;
    byterg12 &= -vb_04;
    byterg12 |= vb_0c;
    wordrg4++;
    *ptrrg52 = byterg12;
    wordrg31--;
    ebxw -= con(0x2);
  }
  if (ebxw <= con(0x0))
    return;
  t_gfxdata byterg11 = ddata.srcbmp[unsignedlong(wordrg31)] & con(0xfffffff0);
  x32 longrg6 = unsignedlong(byterg11);
  if (longrg6 != (unsignedlong(vb_08) << con(0x4)))
    return;
  t_gfxdata* ptrrg51 = ddata.destbmp;
  byterg11 |= ptrrg51[unsignedlong(wordrg4)] & con(0xf);
  ptrrg51[unsignedlong(wordrg4)] = byterg11;
}

// stacksize was 0x18
// srcbmp = eax
// destbmp = edx
// srcofs = ecx
static void SKW_FIRE_BLIT_TO_MEMORY_4TO4BPP(t_gfxdata* srcbmp, c_rect* ebxr, ui16 srcofs, t_gfxdata* destbmp, x16 argw0, x16 argw1, x16 argw2, x16 argw3, x16 argw4)
{
	x16 vl_08 = 0;

  ddata.srcbmp = srcbmp;
  ddata.destbmp = destbmp;
  argw1 = (argw1 + 1) & con(0xfffffffe);
  argw2 = (argw2 + 1) & con(0xfffffffe);
  if (ebxr->w == 0 || ebxr->h == 0)
    return;
  x16 wordrg6 = ebxr->y * argw2 + ebxr->x;
  if (argw4 <= 3)
  {
    x16 wordrg4 = (ebxr->h + argw0 - 1) * argw1 + srcofs;
    switch (argw4)
    {
      case 1:
        srcofs += argw0 * argw1;
        for ( vl_08 = 0; vl_08 < ebxr->h; vl_08++, srcofs += argw1, wordrg6 += argw2)
        {
          if (argw3 >= con(0x0))
            SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO4BPP(srcofs, ebxr->w, CUTX8(argw3), wordrg6);
          else
            R_1761(srcofs, ebxr->w, wordrg6);
        }
        return;
      case 2:
        srcofs = wordrg4;
        for ( vl_08 = 0; vl_08 < ebxr->h; vl_08++, srcofs -= argw1, wordrg6 += argw2)
        {
          if (argw3 >= con(0x0))
            R_190F(srcofs, ebxr->w, CUTX8(argw3), wordrg6);
          else
            R_15B5(srcofs, ebxr->w, wordrg6);
        }
        return;
      case 3:
        srcofs = wordrg4;
        for ( vl_08 = 0; vl_08 < ebxr->h; vl_08++, srcofs -= argw1, wordrg6 += argw2)
        {
          if (argw3 >= con(0x0))
            SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO4BPP(srcofs, ebxr->w, CUTX8(argw3), wordrg6);
          else
            R_1761(srcofs, ebxr->w, wordrg6);
        }
        return;
      case 0:
      ;
    }
  }

  srcofs += argw0 * argw1;
  for ( vl_08 = 0; vl_08 < ebxr->h; vl_08++, srcofs += argw1, wordrg6 += argw2)
  {
    if (argw3 >= con(0x0))
      R_190F(srcofs, ebxr->w, CUTX8(argw3), wordrg6);
    else
      R_15B5(srcofs, ebxr->w, wordrg6);
  }
}

// srcofs = eax
void SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(x16 srcofs, x16 ebxw, x16 edxw)
{
  x16 wordrg4 = edxw;
  if ((srcofs & con(0x1)) != con(0x0))
  {
    t_gfxdata* ptrrg52 = ddata.paletteptr2 + UI32CAST(ddata.srcbmp[unsignedlong(srcofs++) / 2] & con(0xf));
    x32 longrg62 = unsignedlong(wordrg4++);
    t_gfxdata* ptrrg64 = ddata.destbmp + longrg62;
    *ptrrg64 = *ptrrg52;
    ebxw--;
  }
  x16 wordrg1 = srcofs / 2;
  while (ebxw >= con(0x2))
  {
    x8 byterg3 = ddata.srcbmp[unsignedlong(wordrg1++)];
    t_gfxdata* ptrrg51 = ddata.paletteptr2 + (unsignedlong(byterg3) >> 4);
    x32 longrg61 = unsignedlong(wordrg4++);
    t_gfxdata* ptrrg7 = ddata.destbmp;
    t_gfxdata* ptrrg63 = ptrrg7 + longrg61;
    *ptrrg63 = *ptrrg51;
    byterg3 &= con(0xf);
    t_gfxdata* ptrrg62 = ddata.paletteptr2 + unsignedlong(byterg3);
    ddata.destbmp[unsignedlong(wordrg4++)] = *ptrrg62;
    ebxw -= con(0x2);
  }
  if (ebxw != con(0x0))
    ddata.destbmp[unsignedlong(wordrg4)] = ddata.paletteptr2[unsignedlong(ddata.srcbmp[unsignedlong(wordrg1)]) >> 4];
}

// srcbits = argw5
// destbits = argw6
// srcofs = ecxw
void SKW_FIRE_BLIT_PICTURE(t_gfxdata* srcbmp, c_rect* rect, ui16 srcofs, t_gfxdata* destbmp, x16 argw0, x16 argw1, x16 argw2, x16 argw3, x16 argw4, e_bitres srcbits, e_bitres destbits, t_palette* palette)
{
  if (rect == NULL)
    return;
  if (destbmp == ddata.screenptr)
  {
    argw2 = ORIG_SWIDTH;
    enable_video = false;
    while (!enable_drawing);
  }
  if (srcbits == E_BITRES4)
  {
    if (destbits == E_BITRES4)
      SKW_FIRE_BLIT_TO_MEMORY_4TO4BPP(srcbmp, rect, srcofs, destbmp, argw0, argw1, argw2, argw3, argw4);
    else
      SKW_FIRE_BLIT_TO_MEMORY_4TO8BPP(srcbmp, rect, srcofs, destbmp, argw0, argw1, argw2, argw3, argw4, palette);
  }
  else
  {
    if (palette == NULL)
      SKW_FIRE_BLIT_TO_MEMORY_8TO8BPP(srcbmp, rect, srcofs, destbmp, argw0, argw1, argw2, argw3, argw4);
    else
      SKW_FIRE_BLIT_TO_MEMORY_8TO8BPP_TRANSLATED(srcbmp, rect, srcofs, destbmp, argw0, argw1, argw2, argw3, argw4, palette);
  }
  enable_video = true;
}
