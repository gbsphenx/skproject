#include <string.h> // memset

#include <startend.h> // THROW...
#include <util.h>
#include <gfxpal.h>
#include <skalocdb.h>
#include <gfxdec.h>
#include <gfxblit.h>

#define IMG9MEMSIZE (5003)

const ui8 decodemask[9] =
{
  0x0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff
};

c_imgdecode imgdecoder;

void c_imgdecode::init(void)
{
  img3decode = 0;
  decodeptr1 = NULL;
  decodew0 = 0;
  decodew1 = 0;
  decodew2 = false;
  decodew3 = 0;
  decodew4 = 0;
  decodew5 = 0;
  decodeptr2 = NULL;
  decodeptr3 = NULL;
  decodew6 = 0;
  decode7 = false;
  for (int i = 0; i < 9; i++)
    decodemask[i] = ::decodemask[i];
  img9mem1 = NULL;
  img9mem2 = NULL;
}

// was R_4ADA5, TODO: tell SPX new name
// had memory-allocation-function as argument, but is always the same
void c_imgdecode::alloc(void)
{
  img9mem1 = UPCAST(i16, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(3*IMG9MEMSIZE, false));
  img9mem2 = UPCAST(ui8, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(IMG9MEMSIZE, false));
}

// ###################################################
//                         IMG 3
// ###################################################

// was SKW_44c8_1202
void c_imgdecode::func_44c8_1202(ui16 offset, c_pixel16 dpix)
{
  c_pixel16* ptr = &blitter.bmpdata_dest16[offset >> 1];

  if (IS_ODD(offset))
    *ptr = build_pixels16(ptr->getl(), dpix.getr());
  else
    *ptr = build_pixels16(dpix.rtol(), ptr->getr());
}

// was SKW_SPILL_IMG3_PIXELS
void c_imgdecode::spill_img3_pixels(ui16 dofs, ui16 sofs, i16 num)
{
  ui16 d = dofs >> 1;
  sofs >>= 1;

  if (IS_ODD(dofs))
  {
    blitter.bmpdata_dest16[d] = build_pixels16(blitter.bmpdata_dest16[d].getl(), blitter.bmpdata_dest16[sofs].getr());
    d++;
    sofs++;
    num--;
  }

  while (num >= 2)
  {
    blitter.bmpdata_dest16[d++] = blitter.bmpdata_dest16[sofs++];
    num -= 2;
  }

  if (num > 0)
    blitter.bmpdata_dest16[d] = build_pixels16(blitter.bmpdata_dest16[sofs].getl(), blitter.bmpdata_dest16[d].getr());
}

// was SKW_READ_IMG3_NIBBLE
c_pixel16 c_imgdecode::read_img3_nibble(void)
{
  if (IS_ODD(img3decode))
    return blitter.bmpdata_src16[img3decode++ / 2].getr();
  return blitter.bmpdata_src16[img3decode++ / 2].ltor();
}

// was SKW_READ_IMG3_DURATION
ui16 c_imgdecode::read_img3_duration(void)
{
  c_pixel16 pix = read_img3_nibble();
  if (pix != E_COL15)
    return pix.mkidx() + 2;
  ui16 uw = read_img3_nibble().mkidx() << 4;
  uw = read_img3_nibble().mkidx() | uw;
  if (uw != 0xff)
    return uw + 17;
  uw = read_img3_nibble().mkidx() << 12;
  uw |= read_img3_nibble().mkidx() << 8;
  uw |= read_img3_nibble().mkidx() << 4;
  return read_img3_nibble().mkidx() | uw;
}

// was SKW_TRANSPARENT_IMG3_PIXELS
void c_imgdecode::transparent_img3_pixels(ui16 ofs, i16 num)
{
  ui16 d = ofs / 2;

  if (IS_ODD(ofs))
  {
    blitter.bmpdata_dest16[d] = build_pixels16(blitter.bmpdata_dest16[d].getl(), decodeptr1[d].getr());
    d++;
    num--;
  }

  while (num >= 2)
  {
    blitter.bmpdata_dest16[d] = decodeptr1[d];
    d++;
    num -= 2;
  }

  if (num != 0)
    blitter.bmpdata_dest16[d] = build_pixels16(decodeptr1[d].getl(), blitter.bmpdata_dest16[d].getr());
}

// was DM2_DECODE_IMG3_UNDERLAY
void c_imgdecode::decode_img3_underlay(i16* src, c_pixel* dest)
{
  c_pixel16 vba_00[6]; // Note: original dimension 8

  blitter.bmpdata_src16 = CHGCAST(c_pixel16, src); // pairs of (u)bytes
  blitter.bmpdata_dest = dest;

  ui16 pixels_0 = src[0] & 0x3ff;
  ui16 pixels_0e = MK_EVEN(pixels_0);
  ui16 maxofs = pixels_0e * (src[1] & 0x3ff);
  img3decode = 0x8;
  for (i16 i = 0; i < 6; i++)
    vba_00[i] = read_img3_nibble();

  if (pixels_0 != pixels_0e)
  {
    ui16 pixels1 = pixels_0;
    ui16 pixels_e = pixels_0e - pixels_0;
    ui16 dofs = 0;
    do
    {
      ui8 umask;
      if (IS_ODD(img3decode))
        umask = pixel_to_ui8(blitter.bmpdata_src16[img3decode++ / 2].getr());
      else
        umask = pixel_to_ui8(blitter.bmpdata_src16[img3decode++ / 2].ltor());
      i16 idx = mkw(umask & 0x7);
      if (idx != 6)
      {
        c_pixel16 pix;
        if (idx >= 6)
        {
          if (IS_ODD(img3decode))
            pix = blitter.bmpdata_src16[img3decode++ / 2].getr();
          else
            pix = blitter.bmpdata_src16[img3decode++ / 2].ltor();
        }
        else
          pix = vba_00[idx];

        umask &= 0x8;
        ui16 pixels2;
        if (umask == 0)
          pixels2 = 1;
        else
          pixels2 = read_img3_duration();

        while (pixels2 >= pixels1)
        {
          blitter.fill_line_4(dofs, pixels1, pix);
          dofs += pixels_e + pixels1;
          pixels2 -= pixels1;
          pixels1 = pixels_0;
        }

        if (pixels2 != 0)
        {
          blitter.fill_line_4(dofs, pixels2, pix);
          dofs += pixels2;
          pixels1 -= pixels2;
        }
      }
      else
      {
        umask &= 0x8;
        ui16 pixels2;
        if (umask == 0)
          pixels2 = 1;
        else
          pixels2 = read_img3_duration();
        ui16 sofs;
        for (;;)
        {
          sofs = dofs - pixels_0e;
          if (pixels2 < pixels1)
            break;
          spill_img3_pixels(dofs, sofs, pixels1);
          sofs = pixels_e + pixels1;
          dofs += sofs;
          pixels2 -= pixels1;
          pixels1 = pixels_0;
        }
        if (pixels2 != 0)
        {
          spill_img3_pixels(dofs, sofs, pixels1);
          dofs += pixels2;
          pixels1 -= pixels2;
        }
      }
    } while (dofs < maxofs);
  }
  else
  {
    ui16 dofs = 0;
    do
    {
      c_pixel16 spix;
      if (IS_ODD(img3decode))
        spix = blitter.bmpdata_src16[img3decode++ / 2].getr();
      else
        spix = blitter.bmpdata_src16[img3decode++ / 2].ltor();

      ui16 idx = (spix & 0x7).mkidx();
      c_pixel16 pix;
      if (idx != 6)
      {
        if (idx >= 6)
        {
          if (IS_ODD(img3decode))
            pix = blitter.bmpdata_src16[img3decode++ / 2].getr();
          else
            pix = blitter.bmpdata_src16[img3decode++ / 2].ltor();
        }
        else
          pix = vba_00[idx];

        spix &= 0x8;
        if (spix == E_COL00)
          func_44c8_1202(dofs++, pix);
        else
        {
          ui16 pixels = read_img3_duration();
          blitter.fill_line_4(dofs, pixels, pix);
          dofs += pixels;
        }
      }
      else
      {
        spix &= 0x8;
        ui16 num;
        if (spix == E_COL00)
          num = 1;
        else
          num = read_img3_duration();
        spill_img3_pixels(dofs, dofs - pixels_0e, num);
        dofs += num;
      }
    } while (dofs < maxofs);
  }
}

// was DM2 _DECODE_IMG3_OVERLAY
void c_imgdecode::decode_img3_overlay(c_pixel16* dptr, i16* src, c_pixel* dest)
{
  c_pixel16 vba_00[5]; // Note: original dimension was 8
  c_pixel16 fpix;

  blitter.bmpdata_src16 = CHGCAST(c_pixel16, src); // first four bytes are two words
  blitter.bmpdata_dest = dest;

  decodeptr1 = dptr;
  i16 v = src[0] & 0x3ff;
  ui16 pixels_0 = v;
  v = MK_EVEN(v);
  ui16 pixels_0e = v;
  v *= src[1] & 0x3ff;
  ui16 maxofs = v;
  img3decode = 0x8;
  for (i16 i=0; i < 5; i++)
    vba_00[i] = read_img3_nibble();

  if (pixels_0 == pixels_0e)
  {
    ui16 dofs = 0;
    do
    {
      ui8 kpix;
      if (IS_ODD(img3decode))
        kpix = pixel_to_ui8(blitter.bmpdata_src16[img3decode++ / 2].getr());
      else
        kpix = pixel_to_ui8(blitter.bmpdata_src16[img3decode++ / 2].ltor());

      i16 idx = mkw(kpix & 0x7);
      ui8 umask = kpix & 0x8;
      if (idx != 6)
      {
        if (idx != 5)
        {
          if (idx >= 5)
          {
            if (IS_ODD(img3decode))
              fpix = blitter.bmpdata_src16[img3decode++ / 2].getr();
            else
              fpix = blitter.bmpdata_src16[img3decode++ / 2].ltor();
          }
          else
            fpix = vba_00[idx];

          kpix &= 0x8;
          if (kpix == 0)
            func_44c8_1202(dofs++, fpix);
          else
          {
            ui16 pixels = read_img3_duration();
            blitter.fill_line_4(dofs, pixels, fpix);
            dofs += pixels;
          }
        }
        else
        {
          ui16 num;
          if (umask == 0)
            num = 1;
          else
            num = read_img3_duration();
          transparent_img3_pixels(dofs, num);
          dofs += num;
        }
      }
      else
      {
        ui16 num;
        if (umask == 0)
          num = 1;
        else
          num = read_img3_duration();
        spill_img3_pixels(dofs, dofs - pixels_0e, num);
        dofs += num;
      }

    } while (dofs < maxofs);
    return;
  }

  ui16 pixels1 = pixels_0;
  ui16 pixels_e = pixels_0e - pixels_0;
  ui16 dofs = 0;
  do
  {
    c_pixel16 pix;
    if (IS_ODD(img3decode))
      pix = blitter.bmpdata_src16[unsignedlong(img3decode++) / 2].getr();
    else
      pix = blitter.bmpdata_src16[unsignedlong(img3decode++) / 2].ltor();

    ui16 idx = (pix & 0x7).mkidx();
    ui8 umask = pixel_to_ui8(pix & 0x8);
    if (idx != 6)
    {
      if (idx != 5)
      {
        if (idx >= 5)
        {
          if (IS_ODD(img3decode))
            fpix = blitter.bmpdata_src16[unsignedlong(img3decode++) / 2].getr();
          else
            fpix = blitter.bmpdata_src16[unsignedlong(img3decode++) / 2].ltor();
        }
        else
          fpix = vba_00[idx];

        pix &= 0x8;
        ui16 pixels2;
        if (pix == E_COL00)
          pixels2 = 1;
        else
          pixels2 = read_img3_duration();
        while (pixels2 >= pixels1)
        {
          blitter.fill_line_4(dofs, pixels1, fpix);
          dofs += pixels_e + pixels1;
          pixels2 -= pixels1;
          pixels1 = pixels_0;
        }

        if (pixels2 != 0)
        {
          blitter.fill_line_4(dofs, pixels2, fpix);
          dofs += pixels2;
          pixels1 -= pixels2;
        }
      }
      else
      {
        ui16 pixels2;
        if (umask == 0)
          pixels2 = 1;
        else
          pixels2 = read_img3_duration();

        while (pixels2 >= pixels1)
        {
          transparent_img3_pixels(dofs, pixels1);
          dofs += pixels_e + pixels1;
          pixels2 -= pixels1;
          pixels1 = pixels_0;
        }

        if (pixels2 != 0)
        {
          transparent_img3_pixels(dofs, pixels1);
          dofs += pixels2;
          pixels1 -= pixels2;
        }
      }
    }
    else
    {
      ui16 pixels2;
      if (umask == 0)
        pixels2 = 1;
      else
        pixels2 = read_img3_duration();

      ui16 sofs;
      for (;;)
      {
        sofs = dofs - pixels_0e;
        if (pixels2 < pixels1)
          break;
        spill_img3_pixels(dofs, sofs, pixels1);
        dofs += pixels_e + pixels1;
        pixels2 -= pixels1;
        pixels1 = pixels_0;
      }

      if (pixels2 != 0)
      {
        spill_img3_pixels(dofs, sofs, pixels1);
        dofs += pixels2;
        pixels1 -= pixels2;
      }
    }
  } while (dofs < maxofs);
}

// ###################################################
//                       IMG 9
// ###################################################

// was R_4AA0C
i16 c_imgdecode::dec9_1sub(void)
{
  bool pass = false;
  bool pass2 = false;

  if (decodew3 <= decodew1)
  {
    if (!decodew2)
    {
      if (decodew5 != 0)
        pass = true;
    }
    else
    {
      decodew4 = 9;
      decodew1 = 0x1ff;
      decodew2 = false;
    }
  }
  else
  {
    decodew4++;
    if (decodew4 != 12)
      decodew1 = (1 << CUTX16(decodew4)) - 1;
    else
      decodew1 = 0x1000;
  }

  if (!pass)
  {
    i16 wordrg21;
    if (decodew4 >= decodew0)
    {
      wordrg21 = decodew0;
      if (wordrg21 == 0)
        return -1;
      decodew5 = (signedlong(wordrg21) << 3) / decodew4;
    }
    else
    {
      wordrg21 = decodew4;
      decodew5 = 8;
    }

    decodeptr3 = decodeptr2;
    decodeptr2 += wordrg21;
    decodew0 -= wordrg21;
    decodew6 = 0;
  }
  i16 wordrg11 = decodew6;
  i16 wordrg2 = decodew4;
  ui8* xptrrg4 = decodeptr3;
  decodeptr3 = xptrrg4 + 1;
  i16 wordrg4 = unsignedword(*xptrrg4) >> CUTX8(wordrg11);
  i16 wordrg39 = wcon(0x8) - wordrg11;
  i16 wordrg12 = wordrg39;
  wordrg2 -= CUTX16(wordrg39);
  if (signedlong(wordrg2) >= 8)
  {
    decodeptr3++;
    i16 wordrg52 = unsignedword(*decodeptr3);
    wordrg52 <<= CUTX8(wordrg12);
    wordrg4 |= wordrg52;
    wordrg2 -= 8;
    if (wordrg2 == 0)
      pass2 = true;
    else
      wordrg12 += 8;
  }
  if (!pass2)
  {
    i16 wordrg51 = unsignedword(decodemask[signedlong(wordrg2)]);
    wordrg51 &= unsignedword(*decodeptr3);
    wordrg4 |= wordrg51 << CUTX8(wordrg12);
  }
  decodew6 = wordrg2;
  decodew5--;
  return wordrg4;
}

// was R_4AB8B
// xeaxp seems to be a i16*
void c_imgdecode::dec9_1(ui8* data, c_pixel* gfx2, i16 num)
{
  i16* wptrrg6 = img9mem1;
  i8* pb_04 = CHGCAST(i8, wptrrg6 + IMG9MEMSIZE);
  ui8* xptrrg2 = img9mem2;
  ui8* pb_08 = xptrrg2;
  // i8* bp_00 = UPCAST(i8, RG4P); // TODO: request? lost? and RG4?
  decode7 = false;
  decodew2 = false;
  decodew0 = num;
  decodeptr2 = data;
  decodew4 = 9;
  decodew1 = 0x1ff;
  DM2_ZERO_MEMORY(wptrrg6, 0x200);
  i16 vw_14 = 0x100;

  while (vw_14-- >= 0)
    pb_04[vw_14] = vw_14; // truncated!

  decodew3 = 0x101;
  i16 wordrg11 = dec9_1sub();
  if (wordrg11 < 0)
    return;

  ui16 vw_10 = wordrg11;
  i8 vb_18 = CUTX8(wordrg11);
  c_pixel vb_1c;
  if (CUTUX8(wordrg11) == E_COLX90 && (wordrg11 & 0xff00) == 0)
    decode7 = true;
  else
  {
    vb_1c = ui8_to_pixel(CUTUX8(wordrg11));
    *gfx2++ = vb_1c;
  }

  for (;;)
  {
    i16 wordrg1 = dec9_1sub();
    vw_14 = wordrg1;
    if (wordrg1 < 0)
      return;

    if (wordrg1 != 0x100)
    {
      ui16 vw_0c = wordrg1;
      if (wordrg1 >= decodew3)
      {
        *xptrrg2++ = vb_18;
        vw_14 = vw_10;
      }

      i8* xptrrg51;
      ui8* ebppb;
      for (;;)
      {
        i32 longrg12 = signedlong(vw_14);
        ebppb = xptrrg2 + 1;
        xptrrg51 = pb_04 + longrg12;
        if (longrg12 < 0x100)
          break;
        *xptrrg2 = *xptrrg51;
        xptrrg2 = ebppb;
        vw_14 = wptrrg6[longrg12];
      }

      vb_18 = *xptrrg51;
      *xptrrg2 = *xptrrg51;
      xptrrg2 = ebppb;
      c_pixel* xptrrg52;
      do
      {
        xptrrg52 = gfx2 + 1;
        ui8* xptrrg1 = xptrrg2 - 1;
        if (decode7)
        {
          xptrrg2 = xptrrg1;
          ui8 b = *xptrrg1;
          if (b == 0)
          {
            *gfx2 = c_pixel(E_COLX90);
            gfx2 = xptrrg52;
          }
          else
          {
            c_pixel pix = vb_1c;
            while (--b != 0)
              *gfx2++ = pix;
          }
          decode7 = false;
        }
        else
        {
          xptrrg2 = xptrrg1;
          c_pixel b = ui8_to_pixel(*xptrrg1);
          if (b != E_COLX90)
          {
            vb_1c = b;
            *gfx2 = b;
            gfx2 = xptrrg52;
          }
          else
            decode7 = true;
        }
      } while (CHGCAST(ui8, xptrrg2) > pb_08); // TODO compares pointers!

      vw_14 = decodew3;
      i16 rg1 = vw_14;
      if (rg1 < lcon(0x1000))
      {
        wptrrg6[rg1] = vw_10;
        pb_04[rg1] = vb_18;
        decodew3++;
      }
      vw_10 = vw_0c;
    }
    else
    {
      DM2_ZERO_MEMORY(wptrrg6, 0x200);
      decodew2 = true;
      decodew3 = 0x100;
    }
  }
}

// was R_4ADDC
void c_imgdecode::dec9_2(ui8* data, c_pixel* gfx2, i32 num)
{
  ui16 wmask = 0;

  for (;;)
  {
    wmask >>= 1;
    if ((wmask & 0x100) == 0)
    {
      if (--num < 0)
        return;
      wmask = unsignedword(*data++) | uwcon(0xff00);
    }

    if (IS_ODD(wmask))
    {
      if (--num < 0)
        return;
      *gfx2++ = ui8_to_pixel(*data++);
    }
    else
    {
      num -= 2;
      if (num < 0)
        return;

      ui32 ul = unsignedlong(*data++);
      i32 cnt = (ul & lcon(0xffffff0f)) + 3;
      c_pixel* src = gfx2 - ((unsignedlong(*data++) << 4) + (ul >> 4));
      while (cnt--)
        *gfx2++ = *src++;
    }
  }
}

// was R_4AE3E
void c_imgdecode::dec9_3(ui8* data, c_pixel* gfx2, i32 num)
{
  ui16 wmask = 0;

  for (;;)
  {
    wmask >>= 1;
    if ((wmask & 0x100) == 0)
    {
      if (--num < 0)
        return;
      wmask = unsignedword(*data++) | uwcon(0xff00);
    }

    if (IS_ODD(wmask))
    {
      if (--num < 0)
        return;
      *gfx2++ = ui8_to_pixel(*data++);
    }
    else
    {
      num -= 2;
      if (num < 0)
        return;

      ui32 ul = unsignedlong(*data++);
      i32 cnt = (ul & lcon(0xffffff1f)) + 3;
      c_pixel* src = gfx2 - (8 * unsignedlong(*data++) + (ul >> 5));
      while (cnt--)
        *gfx2++ = *src++;
    }
  }
}

// was DM2_DECODE_IMG9
void c_imgdecode::decode_img9(ui8* data, c_pixel* gfx, i32 num)
{
  i16 mode = signedword(data[6]);
  num -= 8;
  data += 8;

  switch (mode)
  {
    case 1:
      dec9_1(data, gfx, CUTX16(num));
      break;

    case 2:
      dec9_2(data, gfx, num);
      break;

    case 3:
      dec9_3(data, gfx, num);
      break;

    default: throw(THROW_DMABORT);
  }
}
