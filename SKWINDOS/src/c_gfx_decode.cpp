#include "startend.h" // THROW...
#include "util.h"
#include "dm2data.h"
#include "c_gfx_fill.h"
#include "c_gfx_decode.h"
#include "c_dballoc.h"

// ###################################################
//                      IMG 3
// ###################################################

// from c_gfx_fill.h:
void SKW_FIRE_FILL_4BPP_PICT_LINE(x16 eaxw, x16 ebxw, t_gfxdata edxb);
void SKW_FIRE_FILL_8BPP_PICT_LINE(x16 lineofs, x16 n, x8 pixel256);

static void SKW_44c8_1202(ui16 eaxw, t_gfxdata edxb)
{
  x16 wordrg2 = eaxw & con(0x1);
  x16 uwordrg11 = eaxw >> con(0x1);
  t_gfxdata byterg2;
  t_gfxdata* Xptrrg1;
  if (wordrg2 == con(0x0))
  {
    Xptrrg1 = ddata.destbmp + unsignedlong(uwordrg11);
    byterg2 = *Xptrrg1 & con(0xf);
    edxb <<= con(0x4);
  }
  else
  {
    Xptrrg1 = ddata.destbmp + unsignedlong(uwordrg11);
    byterg2 = *Xptrrg1 & con(0xfffffff0);
    edxb &= con(0xf);
  }
  edxb |= byterg2;
  *Xptrrg1 = edxb;
}

static void SKW_SPILL_IMG3_PIXELS(x16 eaxw, x16 ebxw, ui16 edxw)
{
  x8 vb_00;

  ui16 uwordrg11 = eaxw >> con(0x1);
  edxw >>= con(0x1);
  if ((eaxw & con(0x1)) != con(0x0))
  {
    x32 longrg5 = unsignedlong(uwordrg11);
    t_gfxdata* Xptrrg61 = ddata.destbmp;
    vb_00 = Xptrrg61[longrg5] & con(0xfffffff0);
    Xptrrg61[longrg5] = (Xptrrg61[edxw] & con(0xf)) | vb_00;
    uwordrg11++;
    edxw++;
    ebxw--;
  }

  while (ebxw >= con(0x2))
  {
    ddata.destbmp[uwordrg11++] = ddata.destbmp[edxw++];
    ebxw -= con(0x2);
  }

  if (ebxw > con(0x0))
    ddata.destbmp[uwordrg11] = (ddata.destbmp[edxw] & con(0xfffffff0)) | (ddata.destbmp[uwordrg11] & con(0xf));
}

static x8 SKW_READ_IMG3_NIBBLE(void)
{
  if ((ddata.img3decode & con(0x1)) == con(0x0))
    return (ddata.srcbmp[ddata.img3decode++ / 2] >> con(0x4)) & con(0xf);
  return ddata.srcbmp[ddata.img3decode++ / 2] & con(0xf);
}

static x16 SKW_READ_IMG3_DURATION(void)
{
  x8 byterg4 = SKW_READ_IMG3_NIBBLE();
  x16 wordrg11 = unsignedword(byterg4);
  if (byterg4 != con(0xf))
    return wordrg11 + con(0x2);
  x32 wordrg41 = 16 * unsignedword(SKW_READ_IMG3_NIBBLE());
  x16 wordrg12 = unsignedword(SKW_READ_IMG3_NIBBLE()) | wordrg41;
  if (wordrg12 != con(0xff))
    return wordrg12 + con(0x11);
  x16 wordrg4 = unsignedword(SKW_READ_IMG3_NIBBLE()) << con(0xc);
  wordrg4 |= unsignedword(SKW_READ_IMG3_NIBBLE()) << con(0x8);
  wordrg4 |= unsignedword(SKW_READ_IMG3_NIBBLE()) << con(0x4);
  return unsignedword(SKW_READ_IMG3_NIBBLE()) | wordrg4;
}

void SKW_DECODE_IMG3_UNDERLAY(x16* eaxp, xxx* edxp)
{
  t_gfxdata vba_00[0x8];
  x16 vw_08;
  x32 vl_0c;
  x32 vl_10;
  x32 vl_14;
  t_gfxdata vb_18;

  ddata.srcbmp = UPCAST(t_gfxdata, eaxp); // pairs of (u)bytes
  ddata.destbmp = UPCAST(t_gfxdata, edxp);
  x16 wordrg46 = eaxp[0] & con(0x3ff);
  vl_14 = lextended(wordrg46);
  x16 wordrg1 = eaxp[1] & con(0x3ff);
  wordrg46++;
  wordrg46 &= con(0xfffffffe);
  vl_0c = lextended(wordrg46);
  wordrg46 *= wordrg1;
  vw_08 = wordrg46;
  ddata.img3decode = con(0x8);
  x16 wordrg3 = con(0x0);
  x32 longrg5;
  while ((longrg5 = signedlong(wordrg3)) < con(0x6))
  {
    vba_00[longrg5] = t_gfxdata(SKW_READ_IMG3_NIBBLE());
    wordrg3++;
  }
  if (CUTX16(vl_14) != CUTX16(vl_0c))
  {
    ui16 uwordrg3 = UI16CAST(vl_14);
    vl_10 = lextended(CUTX16(vl_0c) - uwordrg3);
    ui16 uwordrg51 = con(0x0);
    do
    {
      x8 byterg15;
      if ((ddata.img3decode & con(0x1)) == con(0x0))
        byterg15 = (ddata.srcbmp[ddata.img3decode++ / 2] >> con(0x4)) & con(0xf);
      else
        byterg15 = ddata.srcbmp[ddata.img3decode++ / 2] & con(0xf);
      x32 longrg42 = UI32CAST(byterg15 & con(0x7));
      if (longrg42 != con(0x6))
      {
        if (longrg42 >= con(0x6))
        {
          if ((ddata.img3decode & con(0x1)) == con(0x0))
            vb_18 = (ddata.srcbmp[ddata.img3decode++ / 2] >> con(0x4)) & con(0xf);
          else
            vb_18 = ddata.srcbmp[ddata.img3decode++ / 2] & con(0xf);
        }
        else
          vb_18 = vba_00[longrg42];

        byterg15 &= con(0x8);
        ui16 uwordrg61;
        if (byterg15 == con(0x0))
          uwordrg61 = con(0x1);
        else
          uwordrg61 = SKW_READ_IMG3_DURATION();

        while (uwordrg61 >= uwordrg3)
        {
          SKW_FIRE_FILL_4BPP_PICT_LINE(uwordrg51, uwordrg3, vb_18);
          uwordrg51 += UI16CAST(vl_10) + uwordrg3;
          uwordrg61 -= uwordrg3;
          uwordrg3 = UI16CAST(vl_14);
        }

        if (uwordrg61 != 0)
        {
          SKW_FIRE_FILL_4BPP_PICT_LINE(uwordrg51, uwordrg61, vb_18);
          uwordrg51 += uwordrg61;
          uwordrg3 -= uwordrg61;
        }
      }
      else
      {
        byterg15 &= con(0x8);
        ui16 uwordrg62;
        if (byterg15 == con(0x0))
          uwordrg62 = con(0x1);
        else
          uwordrg62 = SKW_READ_IMG3_DURATION();
        ui16 uwordrg1;
        for(;;)
        {
          uwordrg1 = uwordrg51 - CUTX16(vl_0c);
          if (uwordrg62 < uwordrg3)
            break;
          SKW_SPILL_IMG3_PIXELS(uwordrg51, uwordrg3, uwordrg1);
          uwordrg1 = UI16CAST(vl_10) + uwordrg3;
          uwordrg51 += uwordrg1;
          uwordrg62 -= uwordrg3;
          uwordrg3 = UI16CAST(vl_14);
        }
        if (uwordrg62 != 0)
        {
          SKW_SPILL_IMG3_PIXELS(uwordrg51, uwordrg3, uwordrg1);
          uwordrg51 += uwordrg62;
          uwordrg3 -= uwordrg62;
        }
      }
    } while (uwordrg51 < UI16CAST(vw_08));
  }
  else
  {
    ui16 uwordrg52 = con(0x0);
    do
    {
      x16 byterg11;
      if ((ddata.img3decode & con(0x1)) == con(0x0))
        byterg11 = (ddata.srcbmp[ddata.img3decode++ / 2] >> con(0x4)) & con(0xf);
      else
        byterg11 = ddata.srcbmp[ddata.img3decode++ / 2] & con(0xf);

      x32 longrg45 = UI32CAST(byterg11 & con(0x7));
      if (longrg45 != con(0x6))
      {
        if (longrg45 >= con(0x6))
        {
          if ((ddata.img3decode & con(0x1)) == con(0x0))
            vb_18 = (ddata.srcbmp[ddata.img3decode++ / 2] >> con(0x4)) & con(0xf);
          else
            vb_18 = ddata.srcbmp[ddata.img3decode++ / 2] & con(0xf);
        }
        else
          vb_18 = vba_00[longrg45];

        byterg11 &= con(0x8);
        if (byterg11 == con(0x0))
          SKW_44c8_1202(uwordrg52++, vb_18);
        else
        {
          x16 wordrg19 = SKW_READ_IMG3_DURATION();
          x16 wordrg31 = wordrg19;
          x16 wordrg21 = wordrg19;
          SKW_FIRE_FILL_4BPP_PICT_LINE(uwordrg52, wordrg21, vb_18);
          uwordrg52 += wordrg31;
        }
      }
      else
      {
        byterg11 &= con(0x8);
        ui16 uwordrg63;
        if (byterg11 == con(0x0))
          uwordrg63 = con(0x1);
        else
          uwordrg63 = SKW_READ_IMG3_DURATION();
        SKW_SPILL_IMG3_PIXELS(uwordrg52, uwordrg63, uwordrg52 - CUTX16(vl_0c));
        uwordrg52 += uwordrg63;
      }
    } while (uwordrg52 < UI16CAST(vw_08));
  }
}

static void SKW_TRANSPARENT_IMG3_PIXELS(x16 eaxw, x16 edxw)
{
  x8 vb_00;

  x16 wordrg3 = unsignedlong(eaxw) / 2;
  x16 wordrg1 = wordrg3;
  if ((eaxw & con(0x1)) != con(0x0))
  {
    x32 longrg5 = unsignedlong(wordrg3);
    vb_00 = ddata.destbmp[longrg5] & con(0xfffffff0);
    ddata.destbmp[longrg5] = (ddata.decodeptr[longrg5] & con(0xf)) | vb_00;
    wordrg1++;
    edxw--;
  }
  while (edxw >= con(0x2))
  {
    x32 longrg32 = unsignedlong(wordrg1);
    ddata.destbmp[longrg32] = ddata.decodeptr[longrg32];
    wordrg1++;
    edxw -= con(0x2);
  }
  if (edxw != con(0x0))
  {
    x32 longrg31 = unsignedlong(wordrg1);
    ddata.destbmp[longrg31] = (ddata.destbmp[longrg31] & con(0xf)) | (ddata.decodeptr[longrg31] & con(0xfffffff0));
  }
}

void SKW_DECODE_IMG3_OVERLAY(xxx* eaxp, xxx* ebxp, xxx* edxp)
{
  t_gfxdata vba_00[0x8];
  x16 vw_08;
  x32 vl_0c;
  x32 vl_10;
  x32 vl_14;
  t_gfxdata vb_18;

  ddata.srcbmp = UPCAST(t_gfxdata, edxp); // pairs of (u)bytes
  ddata.destbmp = UPCAST(t_gfxdata, ebxp);
  ddata.decodeptr = eaxp;
  x16 wordrg1 = *P16CAST(edxp) & con(0x3ff);
  vl_14 = lextended(wordrg1);
  wordrg1++;
  wordrg1 &= con(0xfffffffe);
  vl_0c = lextended(wordrg1);
  wordrg1 *= *P16CAST(edxp + con(0x2)) & con(0x3ff);
  vw_08 = wordrg1;
  ddata.img3decode = con(0x8);
  ui16 uwordrg32 = con(0x0);
  while (uwordrg32 < con(0x5))
  {
    t_gfxdata byterg1 = t_gfxdata(SKW_READ_IMG3_NIBBLE());
    vba_00[uwordrg32] = byterg1;
    uwordrg32++;
  }
  if (CUTX16(vl_14) == CUTX16(vl_0c))
  {
    ui16 uwordrg51 = con(0x0);
    do
    {
      x8 byterg15;
      if ((ddata.img3decode & con(0x1)) == con(0x0))
        byterg15 = (ddata.srcbmp[ddata.img3decode++ / 2] >> con(0x4)) & con(0xf);
      else
        byterg15 = ddata.srcbmp[ddata.img3decode++ / 2] & con(0xf);

      x32 longrg43 = UI32CAST(byterg15 & con(0x7));
      x8 byterg14 = byterg15 & con(0x8);
      if (longrg43 != con(0x6))
      {
        if (longrg43 != con(0x5))
        {
          if (longrg43 >= con(0x5))
          {
            if ((ddata.img3decode & con(0x1)) == con(0x0))
              vb_18 = (ddata.srcbmp[ddata.img3decode++ / 2] >> con(0x4)) & con(0xf);
            else
              vb_18 = ddata.srcbmp[ddata.img3decode++ / 2] & con(0xf);
          }
          else
            vb_18 = vba_00[longrg43];

          byterg15 &= con(0x8);
          if (byterg15 == con(0x0))
            SKW_44c8_1202(uwordrg51++, vb_18);
          else
          {
            ui16 uwordrg33 = SKW_READ_IMG3_DURATION();
            x16 wordrg2 = uwordrg33;
            SKW_FIRE_FILL_4BPP_PICT_LINE(uwordrg51, wordrg2, vb_18);
            uwordrg51 += uwordrg33;
          }
        }
        else
        {
          ui16 uwordrg61;
          if (byterg14 == con(0x0))
            uwordrg61 = con(0x1);
          else
            uwordrg61 = SKW_READ_IMG3_DURATION();
          SKW_TRANSPARENT_IMG3_PIXELS(uwordrg51, uwordrg61);
          uwordrg51 += uwordrg61;
        }
      }
      else
      {
        ui16 uwordrg62;
        if (byterg14 == con(0x0))
          uwordrg62 = con(0x1);
        else
          uwordrg62 = SKW_READ_IMG3_DURATION();
        SKW_SPILL_IMG3_PIXELS(uwordrg51, uwordrg62, uwordrg51 - CUTX16(vl_0c));
        uwordrg51 += uwordrg62;
      }

    } while (uwordrg51 < UI16CAST(vw_08));
    return;
  }
  x32 longrg13 = vl_0c;
  ui16 uwordrg3 = CUTX16(vl_14);
  longrg13 -= vl_14;
  vl_10 = longrg13;
  ui16 uwordrg52 = con(0x0);
  do
  {
    x8 byterg18;
    if ((ddata.img3decode & con(0x1)) == con(0x0))
      byterg18 = (ddata.srcbmp[ddata.img3decode++ / 2] >> con(0x4)) & con(0xf);
    else
      byterg18 = ddata.srcbmp[ddata.img3decode++ / 2] & con(0xf);

    x32 longrg41 = UI32CAST(byterg18 & con(0x7));
    x8 byterg17 = byterg18 & con(0x8);
    if (longrg41 != con(0x6))
    {
      if (longrg41 != con(0x5))
      {
        if (longrg41 >= con(0x5))
        {
          if ((ddata.img3decode & con(0x1)) == con(0x0))
            vb_18 = (ddata.srcbmp[ddata.img3decode++ / 2] >> con(0x4)) & con(0xf);
          else
            vb_18 = ddata.srcbmp[ddata.img3decode++ / 2] & con(0xf);
        }
        else
          vb_18 = vba_00[longrg41];

        byterg18 &= con(0x8);
        ui16 uwordrg63;
        if (byterg18 == con(0x0))
          uwordrg63 = con(0x1);
        else
          uwordrg63 = SKW_READ_IMG3_DURATION();
        while (uwordrg63 >= uwordrg3)
        {
          SKW_FIRE_FILL_4BPP_PICT_LINE(uwordrg52, uwordrg3, vb_18);
          uwordrg52 += UI16CAST(vl_10) + uwordrg3;
          uwordrg63 -= uwordrg3;
          uwordrg3 = UI16CAST(vl_14);
        }

        if (uwordrg63 != 0)
        {
          SKW_FIRE_FILL_4BPP_PICT_LINE(uwordrg52, uwordrg63, vb_18);
          uwordrg52 += uwordrg63;
          uwordrg3 -= uwordrg63;
        }
      }
      else
      {
        ui16 uwordrg64;
        if (byterg17 == con(0x0))
          uwordrg64 = con(0x1);
        else
          uwordrg64 = SKW_READ_IMG3_DURATION();

        while (uwordrg64 >= uwordrg3)
        {
          SKW_TRANSPARENT_IMG3_PIXELS(uwordrg52, uwordrg3);
          uwordrg52 += UI16CAST(vl_10) + uwordrg3;
          uwordrg64 -= uwordrg3;
          uwordrg3 = UI16CAST(vl_14);
        }

        if (uwordrg64 != 0)
        {
          SKW_TRANSPARENT_IMG3_PIXELS(uwordrg52, uwordrg3);
          uwordrg52 += uwordrg64;
          uwordrg3 -= uwordrg64;
        }
      }
    }
    else
    {
      ui16 uwordrg65;
      if (byterg17 == con(0x0))
        uwordrg65 = con(0x1);
      else
        uwordrg65 = SKW_READ_IMG3_DURATION();

      ui16 uwordrg11;
      for(;;)
      {
        uwordrg11 = uwordrg52 - CUTX16(vl_0c);
        if (uwordrg65 < uwordrg3)
          break;
        SKW_SPILL_IMG3_PIXELS(uwordrg52, uwordrg3, uwordrg11);
        uwordrg52 += UI16CAST(vl_10) + uwordrg3;
        uwordrg65 -= uwordrg3;
        uwordrg3 = UI16CAST(vl_14);
      }

      if (uwordrg65 != 0)
      {
        SKW_SPILL_IMG3_PIXELS(uwordrg52, uwordrg3, uwordrg11);
        uwordrg52 += uwordrg65;
        uwordrg3 -= uwordrg65;
      }
    }
  } while (uwordrg52 < UI16CAST(vw_08));
}

// ###################################################
//                      IMG 9
// ###################################################

static x16 R_4AA0C(void)
{
  bool pass = false;
  bool pass2 = false;

  if (ddata.decodew3 <= ddata.decodew1)
  {
    if (ddata.decodew2 <= con(0x0))
    {
      if (ddata.decodew5 != con(0x0))
        pass = true;
    }
    else
    {
      ddata.decodew4 = con(0x9);
      ddata.decodew1 = con(0x1ff);
      ddata.decodew2 = con(0x0);
    }
  }
  else
  {
    ddata.decodew4++;
    if (ddata.decodew4 != con(0xc))
      ddata.decodew1 = (con(0x1) << CUTX16(ddata.decodew4)) - 1;
    else
      ddata.decodew1 = con(0x1000);
  }

  if (!pass)
  {
    x16 wordrg21;
    if (ddata.decodew4 >= ddata.decodew0)
    {
      wordrg21 = ddata.decodew0;
      if (wordrg21 == con(0x0))
        return con(0xffffffff);
      ddata.decodew5 = (signedlong(wordrg21) << con(0x3)) / ddata.decodew4;
    }
    else
    {
      wordrg21 = ddata.decodew4;
      ddata.decodew5 = con(0x8);
    }

    ddata.decodeptr3 = ddata.decodeptr2;
    ddata.decodeptr2 += signedlong(wordrg21);
    ddata.decodew0 -= wordrg21;
    ddata.decodew6 = con(0x0);
  }
  x16 wordrg11 = ddata.decodew6;
  x16 wordrg2 = ddata.decodew4;
  xxx* ptrrg4 = ddata.decodeptr3;
  ddata.decodeptr3 = ptrrg4 + con(0x1);
  x16 wordrg4 = unsignedword(*ptrrg4) >> CUTX8(wordrg11);
  x32 wordrg39 = con(0x8) - wordrg11;
  x16 wordrg12 = wordrg39;
  wordrg2 -= CUTX16(wordrg39);
  if (signedlong(wordrg2) >= con(0x8))
  {
    ddata.decodeptr3++;
    x16 wordrg52 = unsignedword(*ddata.decodeptr3);
    wordrg52 <<= CUTX8(wordrg12);
    wordrg4 |= wordrg52;
    wordrg2 -= con(0x8);
    if (wordrg2 == con(0x0))
      pass2 = true;
    else
      wordrg12 += con(0x8);
  }
  if (!pass2)
  {
    x16 wordrg51 = unsignedword(ddata.decodemask[wordrg2]);
    wordrg51 &= unsignedword(*ddata.decodeptr3);
    wordrg4 |= wordrg51 << CUTX8(wordrg12);
  }
  ddata.decodew6 = wordrg2;
  ddata.decodew5--;
  return wordrg4;
}

static void R_4AB8B(xxx* eaxp, x16 ebxw, xxx* edxp)
{
  x8* ebppb;
  //x8* pb_00;
  x8* pb_04;
  x8* pb_08;
  x32 vl_0c;
  x32 vl_10;
  x16 vw_14;
  x8 vb_18;
  x8 vb_1c;

  x16* wptrrg6 = UPCAST(x16, dm2_dballochandler.v1e149c);
  pb_04 = UPCAST(x8, wptrrg6 + con(0x2716) / 2);
  xxx* ptrrg2 = dm2_dballochandler.v1e14a0;
  pb_08 = UPCAST(x8, ptrrg2);
  // pb_00 = UPCAST(x8, RG4P); // TODO: request? lost? and RG4?
  ddata.decodew7 = con(0x0);
  vw_14 = con(0xc);
  ddata.decodew2 = con(0x0);
  ddata.decodew0 = ebxw;
  ddata.decodeptr2 = eaxp;
  ddata.decodew4 = con(0x9);
  ddata.decodew1 = con(0x1ff);
  DM2_ZERO_MEMORY(wptrrg6, con(0x200));
  vw_14 = con(0x100);
  for(;;)
  {
    x16 wordrg5 = vw_14 - 1;
    vw_14 = wordrg5;
    if (wordrg5 < con(0x0))
      break;
    pb_04[vw_14] = CUTX8(vw_14);
  }
  ddata.decodew3 = con(0x101);
  x16 wordrg11 = R_4AA0C();
  vl_10 = lextended(wordrg11);
  vb_18 = CUTX8(wordrg11);
  if (wordrg11 < con(0x0))
    return;
  if (wordrg11 != con(0x90))
  {
    vb_1c = CUTX8(wordrg11);
    *edxp++ = CUTX8(wordrg11);
  }
  else
    ddata.decodew7 = con(0x1);

  for(;;)
  {
    x16 wordrg1 = R_4AA0C();
    vw_14 = wordrg1;
    if (wordrg1 < con(0x0))
      return;
    if (wordrg1 != con(0x100))
    {
      vl_0c = lextended(wordrg1);
      if (wordrg1 >= ddata.decodew3)
      {
        *ptrrg2++ = vb_18;
        vw_14 = CUTX16(vl_10);
      }
      xxx* ptrrg51;
      for(;;)
      {
        x32 longrg12 = signedlong(vw_14);
        ebppb = UPCAST(x8, ptrrg2 + con(0x1));
        ptrrg51 = DOWNCAST(x8, pb_04) + longrg12;
        if (longrg12 < con(0x100))
          break;
        *ptrrg2 = *ptrrg51;
        ptrrg2 = DOWNCAST(x8, ebppb);
        vw_14 = wptrrg6[longrg12];
      }
      vb_18 = *ptrrg51;
      *ptrrg2 = *ptrrg51;
      ptrrg2 = DOWNCAST(x8, ebppb);
      xxx* ptrrg52;
      do
      {
        ptrrg52 = edxp + con(0x1);
        xxx* ptrrg1 = ptrrg2 - con(0x1);
        if (ddata.decodew7 != con(0x0))
        {
          ptrrg2 = ptrrg1;
          x8 byterg17 = *ptrrg1;
          if (byterg17 == con(0x0))
          {
            *edxp = con(0xffffff90);
            edxp = ptrrg52;
          }
          else
          {
            x8 byterg18 = vb_1c;
            while (--byterg17 != 0)
            {
              *edxp = byterg18;
              edxp++;
            }
          }
          ddata.decodew7 = con(0x0);
        }
        else
        {
          ptrrg2 = ptrrg1;
          x8 byterg19 = *ptrrg1;
          if (byterg19 != con(0xffffff90))
          {
            vb_1c = byterg19;
            *edxp = byterg19;
            edxp = ptrrg52;
          }
          else
            ddata.decodew7 = con(0x1);
        }
      } while (ptrrg2 > DOWNCAST(x8, pb_08));
      vw_14 = ddata.decodew3;
      x32 longrg1 = signedlong(vw_14);
      if (longrg1 < con(0x1000))
      {
        wptrrg6[longrg1] = CUTX16(vl_10);
        pb_04[longrg1] = X8CAST(vb_18);
        ddata.decodew3++;
      }
      vl_10 = vl_0c;
    }
    else
    {
      DM2_ZERO_MEMORY(wptrrg6, con(0x200));
      ddata.decodew2 = con(0x1);
      ddata.decodew3 = con(0x100);
    }
  }
}

static void repmsb(i32& c, i8*& esip, i8*& edip)
{
  while (c--)
    *edip++ = *esip++;
}

static void R_4ADDC(x8* eaxpb, x32 ebxl, x8* edxpb)
{
  x8* esipb;
  x8* edipb;
  x8* ebppb;
  ui16 tmp;

  ebppb = edxpb;
  edipb = ebppb;
  esipb = eaxpb;
  ui16 uwordrg1 = con(0x0);
  for(;;)
  {
    uwordrg1 >>= con(0x1);
    if ((uwordrg1 & con(0x100)) == 0)
    {
      if (--ebxl < con(0x0))
        return;
      uwordrg1 = unsignedword(*esipb++) | con(0xff00);
    }
    if ((uwordrg1 & con(0x1)) == 0)
    {
      ebxl -= con(0x2);
      if (ebxl < con(0x0))
        return;
      tmp = uwordrg1;
      ui32 ulongrg11 = unsignedlong(*esipb++);
      ui32 ulongrg2 = ulongrg11;
      ulongrg2 >>= con(0x4);
      x32 longrg3 = (ulongrg11 & con(0xffffff0f)) + con(0x3);
      ui32 ulongrg12 = (unsignedlong(*esipb++) << con(0x4)) + ulongrg2;
      ebppb = esipb;
      esipb = edipb - ulongrg12;
      repmsb(longrg3, esipb, edipb);
      esipb = ebppb;
      uwordrg1 = tmp;
    }
    else
    {
      if (--ebxl < con(0x0))
        return;
      *edipb++ = *esipb++;
    }
  }
}

static void R_4AE3E(x8* eaxpb, x32 ebxl, x8* edxpb)
{
  x8* esipb;
  x8* edipb;
  x8* ebppb1;
  x8* ebppb2;
  ui16 tmp;

  ebppb1 = edxpb;
  edipb = ebppb1;
  esipb = eaxpb;
  ui16 uwordrg11 = con(0x0);
  for(;;)
  {
    uwordrg11 >>= con(0x1);
    if ((uwordrg11 & con(0x100)) == 0)
    {
      if (--ebxl < con(0x0))
        return;
      uwordrg11 = unsignedword(*esipb++) | con(0xff00);
    }
    if ((uwordrg11 & con(0x1)) == 0)
    {
      ebxl -= con(0x2);
      if (ebxl < con(0x0))
        return;
      tmp = uwordrg11;
      ui32 ulongrg11 = unsignedlong(*esipb++);
      ui32 ulongrg2 = ulongrg11;
      ulongrg2 >>= con(0x5);
      x32 longrg3 = (ulongrg11 & con(0xffffff1f)) + con(0x3);
      ui32 ulongrg12 = 8 * unsignedlong(*esipb++) + ulongrg2;
      ebppb2 = esipb;
      esipb = edipb - ulongrg12;
      repmsb(longrg3, esipb, edipb);
      esipb = ebppb2;
      uwordrg11 = tmp;
    }
    else
    {
      if (--ebxl < con(0x0))
        return;
      *edipb++ = *esipb++;
    }
  }
}

void SKW_DECODE_IMG9(xxx* eaxp, x32 ebxl, xxx* edxp)
{
  x8 byterg3 = eaxp[con(0x6)];
  x32 longrg2 = ebxl - con(0x8);
  eaxp += con(0x8);
  switch (byterg3)
  {
    case 1:
      R_4AB8B(eaxp, CUTX16(longrg2), edxp);
    break;
    case 2:
      R_4ADDC(UPCAST(x8, eaxp), longrg2, UPCAST(x8, edxp));
    break;
    case 3:
      R_4AE3E(UPCAST(x8, eaxp), longrg2, UPCAST(x8, edxp));
    break;
    default: throw(THROW_DMABORT);
  }
}
