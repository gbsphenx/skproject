#include "dm2global.h" // ORIG_SWIDTH
#include "startend.h" // THROW...
#include "util.h" // IS_ODD, MK_EVEN
#include "c_gfx_pal.h" // paldat.pal16to256ptr, DM2_UPDATE_BLIT_PALETTE
#include "c_gfx_bmp.h"
#include "c_rect.h"
#include "fileio.h"
#include "c_gfx_pixel.h"
#include "c_gfx_blit.h"

/*
   BLITMODE-variations:

   BLITMODE 1 & 3: horizontal mirrored
   BLITMODE 2 & 3: vertical mirrored
   if alphamask >=0 masked
   if palette != NULL by palette
*/

#define BLITDATAFILENAME "xblitb.dat"
#define BLITDATASIZE 0x1000

ui8 xblitb[BLITDATASIZE]; // blit 4bpp-to-4bpp-table, after init readonly

// TODO: fileoperations should be completely in fileio.h
void c_blitter::init(void)
{
  DM2_READ_BINARY(BLITDATAFILENAME, xblitb, BLITDATASIZE);
  bmpdata_src = NULL;
  bmpdata_dest = NULL;
}

c_blitter blitter;

// ####################################################################

// this palette-routine just used here

static c_pixel256 xlat(const t_palette* palette, c_pixel xlatidx)
{
  return palettecolor_to_pixel(palette[xlatidx.mkidx()]);
}

// ####################################################################

// was sub_25AF
// added first argument
void c_blitter::blit_within_screen(c_pixel256* screen, c_rect* rectp, i16 yofs)
{
  for (i32 wi = rectp->y + yofs; rectp->y + rectp->h - 1 >= wi; wi++)
    DM2_COPY_MEMORY(DOWNCAST(c_pixel256, screen) + (wi - yofs) * ORIG_SWIDTH, DOWNCAST(c_pixel, screen) + wi * ORIG_SWIDTH, rectp->w);
}

// all loops changed from do-while to while
// Note: srcofs = srcx; is signed to unsigned!
// the incoming offsets are pixeloffsets, independend of pixelformat

// was DM2_blit_15B5 TODO tell SPX new name
void c_blitter::blitline_44_(ui16 srcofs, ui16 destofs, ui16 pixels)
{
  if (IS_ODD(srcofs) != IS_ODD(destofs))
  {
    if (IS_ODD(destofs))
    {
      bmpdata_dest16[destofs / 2] = build_pixels16(bmpdata_dest16[destofs / 2].getl(), bmpdata_src16[srcofs++ / 2].ltor());
      destofs++;
      pixels--;
    }

    ui16 srcofs_2 = srcofs / 2;
    ui16 destofs_2 = destofs / 2;
    while (pixels > 1)
    {
      bmpdata_dest16[destofs_2++] = build_pixels16(bmpdata_src16[srcofs_2].rtol(), bmpdata_src16[srcofs_2 + 1].ltor());
      srcofs_2++;
      pixels -= 2;
    }

    if (pixels > 0)
      bmpdata_dest16[destofs_2] = build_pixels16(bmpdata_src16[srcofs_2].rtol(), bmpdata_dest16[destofs_2].getr());
  }
  else
  {
    if (IS_ODD(destofs))
    {
      bmpdata_dest16[destofs / 2] = build_pixels16(bmpdata_dest16[destofs / 2].getl(), bmpdata_src16[srcofs++ / 2].getr());
      destofs++;
      pixels--;
    }

    ui16 srcofs_2 = srcofs / 2;
    ui16 destofs_2 = destofs / 2;
    while (pixels > 1)
    {
      bmpdata_dest16[destofs_2++] = bmpdata_src16[srcofs_2++];
      pixels -= 2;
    }

    if (pixels > 0)
      bmpdata_dest16[destofs_2] = build_pixels16(bmpdata_src16[srcofs_2].getl(), bmpdata_dest16[destofs_2].getr());
  }
}

// was R_190F TODO tell SPX new name
void c_blitter::blitline_44_ma(ui16 srcofs, ui16 destofs, ui16 pixels, ui8 alpha)
{
  ui8* masktable = &xblitb[(unsignedword(alpha) << 8)];

  if (IS_ODD(srcofs) == IS_ODD(destofs))
  {
    if (IS_ODD(destofs))
    {
      c_pixel16 pix16 = bmpdata_src16[srcofs++ / 2].getr();
      if (!pix16.is(alpha))
        bmpdata_dest16[destofs / 2] = build_pixels16(bmpdata_dest16[destofs / 2].getl(), pix16);
      destofs++;
      pixels--;
    }

    ui16 srcofs_2 = srcofs / 2;
    ui16 destofs_2 = destofs / 2;
    while (pixels > 1)
    {
      c_pixel16 pix16 = bmpdata_src16[srcofs_2++];
      bmpdata_dest16[destofs_2++] = build_pixels_masked16(pix16, bmpdata_dest16[destofs_2], masktable[pix16.mkidx()]);
      pixels -= 2;
    }

    if (pixels > 0)
    {
      c_pixel16 pix16 = bmpdata_src16[srcofs_2].getl();
      if (!pix16.is(alpha << 4))
        bmpdata_dest16[destofs_2] = build_pixels16(pix16, bmpdata_dest16[destofs_2].getr());
    }
  }
  else
  {
    if (IS_ODD(destofs))
    {
      c_pixel16 pix16 = bmpdata_src16[srcofs++ / 2].ltor();
      if (!pix16.is(alpha))
        bmpdata_dest16[destofs / 2] = build_pixels16(bmpdata_dest16[destofs / 2].getl(), pix16);
      destofs++;
      pixels--;
    }

    ui16 srcofs_2 = srcofs / 2;
    ui16 destofs_2 = destofs / 2;
    while (pixels > 1)
    {
      c_pixel16 pix16 = build_pixels16(bmpdata_src16[srcofs_2].rtol(), bmpdata_src16[srcofs_2 + 1].ltor());
      bmpdata_dest16[destofs_2++] = build_pixels_masked16(pix16, bmpdata_dest16[destofs_2], masktable[pix16.mkidx()]);
      srcofs_2++;
      pixels -= 2;
    }

    if (pixels > 0)
    {
      c_pixel16 pix16 = bmpdata_src16[srcofs_2].getr();
      if (!pix16.is(alpha))
        bmpdata_dest16[destofs_2] = build_pixels16(pix16.rtol(), bmpdata_dest16[destofs_2].getr());
    }
  }
}

// was R_1761 TODO tell SPX new name
void c_blitter::blitline_44_mi(ui16 srcofs, ui16 destofs, ui16 pixels)
{
  srcofs += pixels - 1;

  if (IS_ODD(srcofs) != IS_ODD(destofs))
  {
    if (IS_ODD(destofs))
    {
      bmpdata_dest16[destofs / 2] = build_pixels16(bmpdata_dest16[destofs / 2].getl(), bmpdata_src16[srcofs / 2].ltor());
      pixels--;
      destofs++;
      srcofs--;
    }

    ui16 srcofs_2 = srcofs / 2;
    ui16 destofs_2 = destofs / 2;
    while (pixels > 1)
    {
      c_pixel16 pix16 = bmpdata_src16[srcofs_2];
      bmpdata_dest16[destofs_2++] = build_pixels16(pix16.rtol(), pix16.ltor());
      srcofs_2--;
      pixels -= 2;
    }

    if (pixels > 0)
      bmpdata_dest16[destofs_2] = build_pixels16(bmpdata_src16[srcofs_2].rtol(), bmpdata_dest16[destofs_2].getr());
  }
  else
  {
    if (IS_ODD(destofs))
    {
      bmpdata_dest16[destofs / 2] = build_pixels16(bmpdata_dest16[destofs / 2].getl(), bmpdata_src16[srcofs / 2].getr());
      pixels--;
      destofs++;
      srcofs--;
    }

    srcofs = srcofs / 2;
    while (pixels > 1)
    {
      bmpdata_dest16[destofs++ / 2] = build_pixels16(bmpdata_src16[srcofs].getl(), bmpdata_src16[srcofs - 1].getr());
      srcofs--;
      pixels -= 2;
    }

    if (pixels > 0)
      bmpdata_dest16[destofs / 2] = build_pixels16(bmpdata_src16[srcofs].getl(), bmpdata_dest16[srcofs].getr());
  }
}

// was SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO4BPP TODO tell SPX new name
void c_blitter::blitline_44_mima(ui16 srcofs, ui16 destofs, ui16 pixels, ui8 alpha)
{
  ui8* masktable = &xblitb[(unsignedword(alpha) << 8)];
  srcofs += pixels - 1;

  if (IS_ODD(srcofs) != IS_ODD(destofs))
  {
    if (IS_ODD(destofs))
    {
      c_pixel16 pix16 = bmpdata_src16[srcofs / 2].ltor();
      if (!pix16.is(alpha))
        bmpdata_dest16[destofs / 2] = build_pixels16(bmpdata_dest16[destofs / 2].getl(), pix16);
      destofs++;
      srcofs--;
      pixels--;
    }

    ui16 srcofs_2 = srcofs / 2;
    ui16 destofs_2 = destofs / 2;
    while (pixels > 1)
    {
      c_pixel16 spix16 = bmpdata_src16[srcofs_2--];
      c_pixel16 dpix16 = build_pixels16(spix16.rtol(), spix16.ltor());
      c_pixel16 olddpix = bmpdata_dest16[destofs_2];
      bmpdata_dest16[destofs_2++] = build_pixels_masked16(dpix16, olddpix, masktable[dpix16.mkidx()]);
      pixels -= 2;
    }

    if (pixels > 0)
    {
      c_pixel16 pix16 = bmpdata_src16[srcofs_2].getr();
      if (!pix16.is(alpha))
        bmpdata_dest16[destofs_2] = build_pixels16(pix16.rtol(), bmpdata_dest16[destofs_2].getr());
    }
  }
  else
  {
    if (IS_ODD(destofs))
    {
      c_pixel16 pix16 = bmpdata_src16[srcofs / 2].getr();
      if (!pix16.is(alpha))
        bmpdata_dest16[destofs / 2] = build_pixels16(bmpdata_dest16[destofs / 2].getl(), pix16);
      destofs++;
      srcofs--;
      pixels--;
    }

    ui16 srcofs_2 = srcofs / 2;
    ui16 destofs_2 = destofs / 2;
    while (pixels > 1)
    {
      c_pixel16 pix16 = build_pixels16(bmpdata_src16[srcofs_2].getl(), bmpdata_src16[srcofs_2 - 1].getr());
      c_pixel16 olddpix16 = bmpdata_dest16[destofs_2++];
      bmpdata_dest16[destofs_2] = build_pixels_masked16(pix16, olddpix16, masktable[pix16.mkidx()]); // Note: instead a ~ there was a - in the assembly here
      srcofs_2--;
      pixels -= 2;
    }

    if (pixels > 0)
    {
      c_pixel16 pix16 = bmpdata_src16[srcofs_2].getl();
      if (pix16.is(alpha << 4))
        bmpdata_dest16[destofs_2] = build_pixels16(pix16, bmpdata_dest16[destofs_2].getr());
    }
  }
}

// was SKW_FIRE_BLIT_TO_MEMORY_4TO4BPP
void c_blitter::blitline_44(c_pixel16* srcgfx, c_pixel16* destgfx, c_rect* blitrect, i16 srcx, i16 srcy, ui16 srcw, ui16 destw, t_alphamask alphamask, e_blitmode blitmode)
{
  i16 i = 0;

  if (blitrect->w == 0 || blitrect->h == 0)
    return;

  bmpdata_src16 = srcgfx;
  bmpdata_dest16 = destgfx;

  srcw = MK_EVEN(srcw);
  destw = MK_EVEN(destw);

  ui16 srcofs = srcx;
  ui16 destofs = blitrect->y * destw + blitrect->x;
  ui16 pixels = blitrect->w;
  ui8 alpha = alphamask; // truncated

  switch (blitmode)
  {
    case BLITMODE1:
      srcofs += srcy * srcw;
      for (i = 0; i < blitrect->h; i++, srcofs += srcw, destofs += destw)
        if (alphamask >= 0)
          blitline_44_mima(srcofs, destofs, pixels, alpha);
        else
          blitline_44_mi(srcofs, destofs, pixels);
      break;

    case BLITMODE2:
      srcofs += (srcy + blitrect->h - 1) * srcw;
      for (i = 0; i < blitrect->h; i++, srcofs -= srcw, destofs += destw)
        if (alphamask >= 0)
          blitline_44_ma(srcofs, destofs, pixels, alpha);
        else
          blitline_44_(srcofs, destofs, pixels);
      break;

    case BLITMODE3:
      srcofs += (srcy + blitrect->h - 1) * srcw;
      for (i = 0; i < blitrect->h; i++, srcofs -= srcw, destofs += destw)
        if (alphamask >= 0)
          blitline_44_mima(srcofs, destofs, pixels, alpha);
        else
          blitline_44_mi(srcofs, destofs, pixels);
      break;

    default:
      srcofs += srcy * srcw;
      for (i = 0; i < blitrect->h; i++, srcofs += srcw, destofs += destw)
        if (alphamask >= 0)
          blitline_44_ma(srcofs, destofs, pixels, alpha);
        else
          blitline_44_(srcofs, destofs, pixels);
  }
}

// ####################################################################

// was DM2_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY TODO tell SPX new name
void c_blitter::blitline_48_(ui16 srcofs, ui16 destofs, ui16 pixels)
{
  if (IS_ODD(srcofs))
  {
    bmpdata_dest256[destofs++] = palettecolor_to_pixel(paldat.pal16to256ptr[bmpdata_src16[srcofs++ / 2].getr().mkidx()]);
    pixels--;
  }

  srcofs /= 2;

  while (pixels >= 2)
  {
    c_pixel16 pix16 = bmpdata_src16[srcofs++];
    bmpdata_dest256[destofs++] = palettecolor_to_pixel(paldat.pal16to256ptr[pix16.ltor().mkidx()]);
    bmpdata_dest256[destofs++] = palettecolor_to_pixel(paldat.pal16to256ptr[pix16.getr().mkidx()]);
    pixels -= 2;
  }

  if (pixels > 0)
    bmpdata_dest256[destofs] = palettecolor_to_pixel(paldat.pal16to256ptr[bmpdata_src16[srcofs].ltor().mkidx()]);
}

// was DM2_blit_44c8_08ae TODO: tell SPX new name
// ORIGINAL: inconsequent handling of alpha!
void c_blitter::blitline_48_ma(ui16 srcofs, ui16 destofs, ui16 pixels, ui8 alpha)
{
  alpha &= 0xf; // added: ensure left nibble of alpha is 0

  if (IS_ODD(srcofs))
  {
    c_pixel16 pix16 = bmpdata_src16[srcofs++ / 2].getr();
    if (!pix16.is(alpha))
      bmpdata_dest256[destofs] = palettecolor_to_pixel(paldat.pal16to256ptr[pix16.mkidx()]);
    destofs++;
    pixels--;
  }

  srcofs /= 2;
  i16 n = pixels / 2;
  ui16 ofsstep = n;

  if (n != 0)
  {
    c_pixel16* spixp16 = bmpdata_src16 + srcofs;
    c_pixel256* dpixp256 = bmpdata_dest256 + destofs;
    ui8 twoalpha = (alpha << 4) | alpha;

    while (n--)
    {
      c_pixel16 pix16 = *spixp16++;
      if (!pix16.is(twoalpha))
      {
        c_pixel16 lpix16 = pix16.ltor();
        if (!lpix16.is(alpha))
          *dpixp256 = xlat(paldat.pal16to256ptr, lpix16);
        dpixp256++;

        c_pixel16 rpix16 = pix16.getr();
        if (!rpix16.is(alpha))
          *dpixp256 = xlat(paldat.pal16to256ptr, rpix16);
        dpixp256++;
      }
      else
        dpixp256 += 2;
    }

    srcofs += ofsstep;
    destofs += 2 * ofsstep;
    // here was a pretty odd handling of 'pixels'
  }

  if (IS_ODD(pixels)) // that does it
  {
    c_pixel16 pix16 = bmpdata_src16[srcofs].ltor();
    if (!pix16.is(alpha))
      bmpdata_dest256[destofs] = palettecolor_to_pixel(paldat.pal16to256ptr[pix16.mkidx()]);
  }
}

// was R_2035 TODO: tell SPX new name
void c_blitter::blitline_48_mi(ui16 srcofs, ui16 destofs, ui16 pixels)
{
  destofs += pixels - 1;

  if (IS_ODD(srcofs))
  {
    bmpdata_dest256[destofs--] = palettecolor_to_pixel(paldat.pal16to256ptr[bmpdata_src16[srcofs++ / 2].getr().mkidx()]);
    pixels--;
  }

  srcofs /= 2;

  while (pixels >= 2)
  {
    c_pixel16 pix16 = bmpdata_src16[srcofs++];
    bmpdata_dest256[destofs--] = palettecolor_to_pixel(paldat.pal16to256ptr[pix16.ltor().mkidx()]);
    bmpdata_dest256[destofs--] = palettecolor_to_pixel(paldat.pal16to256ptr[pix16.getr().mkidx()]);
    pixels -= 2;
  }

  if (pixels > 0)
    bmpdata_dest256[destofs] = palettecolor_to_pixel(paldat.pal16to256ptr[bmpdata_src16[srcofs].ltor().mkidx()]);
}

// was SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP TODO tell SPX new name
void c_blitter::blitline_48_mima(ui16 srcofs, ui16 destofs, ui16 pixels, ui8 alpha)
{
  destofs += pixels - 1;

  if (IS_ODD(srcofs))
  {
    c_pixel16 pix16 = bmpdata_src16[srcofs++ / 2].getr();

    if (!pix16.is(alpha))
      bmpdata_dest256[destofs] = palettecolor_to_pixel(paldat.pal16to256ptr[pix16.mkidx()]);

    destofs--;
    pixels--;
  }

  srcofs /= 2;

  while (pixels >= 2)
  {
    c_pixel16 pix16 = bmpdata_src16[srcofs++];
    c_pixel16 lpix16 = pix16.ltor();

    if (!lpix16.is(alpha))
      bmpdata_dest256[destofs] = palettecolor_to_pixel(paldat.pal16to256ptr[lpix16.mkidx()]);
    destofs--;

    pix16 = pix16.getr();
    if (!pix16.is(alpha))
      bmpdata_dest256[destofs] = palettecolor_to_pixel(paldat.pal16to256ptr[pix16.mkidx()]);
    destofs--;

    pixels -= 2;
  }

  if (pixels > 0)
  {
    c_pixel16 pix16 = bmpdata_src16[srcofs].ltor();

    if (!pix16.is(alpha))
      bmpdata_dest256[destofs] = palettecolor_to_pixel(paldat.pal16to256ptr[pix16.mkidx()]);
  }
}

// was SKW_FIRE_BLIT_TO_MEMORY_4TO8BPP
void c_blitter::blitline_48(c_pixel16* srcgfx, c_pixel256* destgfx, c_rect* blitrect, i16 srcofs, i16 srcy, ui16 srcw, ui16 destw, t_alphamask alphamask, e_blitmode blitmode, t_palette* palette)
{
  i16 i = 0;

  if (blitrect->w == 0 || blitrect->h == 0)
    return;

  DM2_UPDATE_BLIT_PALETTE(palette);

  bmpdata_src16 = srcgfx;
  bmpdata_dest256 = destgfx;

  srcw = MK_EVEN(srcw);
  ui16 destofs = blitrect->y * destw + blitrect->x;
  ui16 pixels = blitrect->w;
  ui8 alpha = CUTX16(alphamask);

  switch (blitmode)
  {
    case BLITMODE1:
      srcofs += srcy * srcw;
      for (i = 0; i < blitrect->h; i++, srcofs += srcw, destofs += destw)
        if (alphamask >= 0)
          blitline_48_mima(srcofs, destofs, pixels, alpha);
        else
          blitline_48_mi(srcofs, destofs, pixels);
      break;

    case BLITMODE2:
      srcofs += (srcy + blitrect->h - 1) * srcw;
      for (i = 0; i < blitrect->h; i++, srcofs -= srcw, destofs += destw)
        if (alphamask >= 0)
          blitline_48_ma(srcofs, destofs, pixels, alpha);
        else
          blitline_48_(srcofs, destofs, pixels);
      break;

    case BLITMODE3:
      srcofs += (srcy + blitrect->h - 1) * srcw;
      for (i = 0; i < blitrect->h; i++, srcofs -= srcw, destofs += destw)
        if (alphamask >= 0)
          blitline_48_mima(srcofs, destofs, pixels, alpha);
        else
          blitline_48_mi(srcofs, destofs, pixels);
      break;

    default:
      srcofs += srcy * srcw;
      for (i = 0; i < blitrect->h; i++, srcofs += srcw, destofs += destw)
        if (alphamask >= 0)
          blitline_48_ma(srcofs, destofs, pixels, alpha);
        else
          blitline_48_(srcofs, destofs, pixels);
  }
}

// ####################################################################

// was SKW_44c8_0b8d, DM2_blit8pppixel
void c_blitter::blitline_88_(ui16 srcofs, ui16 destofs, ui16 pixels)
{
  c_pixel256* srcgfx = bmpdata_src256 + srcofs;
  c_pixel256* destgfx = bmpdata_dest256 + destofs;

  while (pixels--)
    *destgfx++ = *srcgfx++;
}

// was SKW_44c8_0bc5, DM2_blit8pppixelmasked
void c_blitter::blitline_88_ma(ui16 srcofs, ui16 destofs, ui16 pixels, ui8 alpha)
{
  c_pixel256* srcgfx = bmpdata_src256 + srcofs;
  c_pixel256* destgfx = bmpdata_dest256 + destofs;

  while (pixels--)
  {
    c_pixel256 pix256 = *srcgfx++;
    if (!pix256.is(alpha))
      *destgfx = pix256;
    destgfx++;
  }
}

// was SKW_44c8_0bf8, DM2_blit8pppixelmirrored
void c_blitter::blitline_88_mi(ui16 srcofs, ui16 destofs, ui16 pixels)
{
  c_pixel256* srcgfx = bmpdata_src256 + srcofs + pixels - 1;
  c_pixel256* destgfx = bmpdata_dest256 + destofs;

  while (pixels--)
    *destgfx++ = *srcgfx--;
}

// was SKW_44c8_0c3c, DM2_blit8pppixelmirroredmasked
void c_blitter::blitline_88_mima(ui16 srcofs, ui16 destofs, ui16 pixels, ui8 alpha)
{
  c_pixel256* srcgfx = bmpdata_src256 + srcofs + pixels - 1;
  c_pixel256* destgfx = bmpdata_dest256 + destofs;

  while (pixels--)
  {
    c_pixel256 pix256 = *srcgfx--;
    if (!pix256.is(alpha))
      *destgfx = pix256;
    destgfx++;
  }
}

// was SKW_FIRE_BLIT_TO_MEMORY_8TO8BPP
void c_blitter::blitline_88(c_pixel256* srcgfx, c_pixel256* destgfx, c_rect* blitrect, i16 srcx, i16 srcy, ui16 srcw, ui16 destw, t_alphamask alphamask, e_blitmode blitmode)
{
  i16 i = 0;

  if (blitrect->w <= 0 || blitrect->h <= 0)
    return;

  bmpdata_src256 = srcgfx;
  bmpdata_dest256 = destgfx;

  ui16 srcofs = srcx;
  ui16 destofs = blitrect->y * destw + blitrect->x;
  ui16 pixels = blitrect->w;
  ui8 alpha = alphamask; // truncated

  switch (blitmode)
  {
    case BLITMODE1:
      srcofs += srcy * srcw;
      for (i = 0; i < blitrect->h; i++, srcofs += srcw, destofs += destw)
        if (alphamask >= 0)
          blitline_88_mima(srcofs, destofs, pixels, alpha);
        else
          blitline_88_mi(srcofs, destofs, pixels);
      break;

    case BLITMODE2:
      srcofs += (srcy + blitrect->h - 1) * srcw;
      for (i = 0; i < blitrect->h; i++, srcofs -= srcw, destofs += destw)
        if (alphamask >= 0)
          blitline_88_ma(srcofs, destofs, pixels, alpha);
        else
          blitline_88_(srcofs, destofs, pixels);
      break;

    case BLITMODE3:
      srcofs += (srcy + blitrect->h - 1) * srcw;
      for (i = 0; i < blitrect->h; i++, srcofs -= srcw, destofs += destw)
        if (alphamask >= 0)
          blitline_88_mima(srcofs, destofs, pixels, alpha);
        else
          blitline_88_mi(srcofs, destofs, pixels);
      break;

    default:
      srcofs += srcy * srcw;
      for (i = 0; i < blitrect->h; i++, srcofs += srcw, destofs += destw)
        if (alphamask >= 0)
          blitline_88_ma(srcofs, destofs, pixels, alpha);
        else
          blitline_88_(srcofs, destofs, pixels);
  }
}

// ####################################################################

// was R_2871
void c_blitter::blitline_88xlat_(ui16 srcofs, ui16 destofs, ui16 pixels, t_palette* palette)
{
  c_pixel256* srcgfx = bmpdata_src256 + srcofs;
  c_pixel256* destgfx = bmpdata_dest256 + destofs;

  while (pixels--)
    *destgfx++ = palettecolor_to_pixel(palette[(*srcgfx++).mkidx()]);
}

// was R_28A2
void c_blitter::blitline_88xlat_ma(ui16 srcofs, ui16 destofs, ui16 pixels, ui8 alpha, t_palette* palette)
{
  c_pixel256* srcgfx = bmpdata_src256 + srcofs;
  c_pixel256* destgfx = bmpdata_dest256 + destofs;

  while (pixels--)
  {
    c_pixel256 pix256 = *srcgfx++;
    if (!pix256.is(alpha))
      *destgfx = xlat(palette, pix256);
    destgfx++;
  }
}

// was R_28DF
void c_blitter::blitline_88xlat_mi(ui16 srcofs, ui16 destofs, ui16 pixels, t_palette* palette)
{
  c_pixel256* srcgfx = bmpdata_src256 + srcofs + pixels - 1;
  c_pixel256* destgfx = bmpdata_dest256 + destofs;

  while (pixels--)
    *destgfx++ = xlat(palette, *srcgfx--);
}

// was R_291B
void c_blitter::blitline_88xlat_mima(ui16 srcofs, ui16 destofs, ui16 pixels, ui8 alpha, t_palette* palette)
{
  c_pixel256* srcgfx = bmpdata_src256 + srcofs + pixels - 1;
  c_pixel256* destgfx = bmpdata_dest256 + destofs;

  while (pixels--)
  {
    c_pixel256 pix256 = *srcgfx--;
    if (!pix256.is(alpha))
      *destgfx = xlat(palette, pix256);
    destgfx++;
  }
}

// was SKW_44c8_0f29, SKW_FIRE_BLIT_TO_MEMORY_8TO8BPP_TRANSLATED
// DM2_BLIT_TO_MEMORY_8TO8BPP_TRANSLATED
void c_blitter::blitline_88xlat(c_pixel256* srcgfx, c_pixel256* destgfx, c_rect* blitrect, i16 srcx, i16 srcy, ui16 srcw, ui16 destw, t_alphamask alphamask, e_blitmode blitmode, t_palette* palette)
{
  i16 i = 0;

  if (blitrect->w == 0 || blitrect->h == 0)
    return;

  bmpdata_src256 = srcgfx;
  bmpdata_dest256 = destgfx;

  ui16 srcofs = srcx;
  ui16 destofs = blitrect->y * destw + blitrect->x;
  ui16 pixels = blitrect->w;
  ui8 alpha = alphamask; // truncated

  switch (blitmode)
  {
    case BLITMODE1:
      srcofs += srcy * srcw;
      for (i = 0; i < blitrect->h; i++, srcofs += srcw, destofs += destw)
        if (alphamask >= 0)
          blitline_88xlat_mima(srcofs, destofs, pixels, alpha, palette);
        else
          blitline_88xlat_mi(srcofs, destofs, pixels, palette);
      break;

    case BLITMODE2:
      srcofs += (srcy + blitrect->h - 1) * srcw;
      for (i = 0; i < blitrect->h; i++, srcofs -= srcw, destofs += destw)
        if (alphamask >= 0)
          blitline_88xlat_ma(srcofs, destofs, pixels, alpha, palette);
        else
          blitline_88xlat_(srcofs, destofs, pixels, palette);
      break;

    case BLITMODE3:
      srcofs += (srcy + blitrect->h - 1) * srcw;
      for (i = 0; i < blitrect->h; i++, srcofs -= srcw, destofs += destw)
        if (alphamask >= 0)
          blitline_88xlat_mima(srcofs, destofs, pixels, alpha, palette);
        else
          blitline_88xlat_mi(srcofs, destofs, pixels, palette);
      break;

    default:
      srcofs += srcy * srcw;
      for (i = 0; i < blitrect->h; i++, srcofs += srcw, destofs += destw)
        if (alphamask >= 0)
          blitline_88xlat_ma(srcofs, destofs, pixels, alpha, palette);
        else
          blitline_88xlat_(srcofs, destofs, pixels, palette);
  }
}

// ####################################################################

// request if destgfx == gfxsys.dm2screen pulled out (dest = ORIG_SWIDTH then)
// was DM2_BLIT_PICTURE
void c_blitter::blit(c_pixel* srcgfx, c_pixel* destgfx, c_rect* blitrect, i16 srcx, i16 srcy, ui16 srcw, ui16 destw, t_alphamask alphamask, e_blitmode blitmode, t_resolution sbpp, t_resolution dbpp, t_palette* palette)
{
  if (blitrect == NULL)
    return;

  if (sbpp == BPP_4)
  {
    if (dbpp == BPP_4)
      blitline_44(reinterpret_cast<c_pixel16*>(srcgfx), reinterpret_cast<c_pixel16*>(destgfx), blitrect, srcx, srcy, srcw, destw, alphamask, blitmode);
    else
      blitline_48(reinterpret_cast<c_pixel16*>(srcgfx), reinterpret_cast<c_pixel256*>(destgfx), blitrect, srcx, srcy, srcw, destw, alphamask, blitmode, palette);
  }
  else
  {
    if (palette == NULL)
      blitline_88(reinterpret_cast<c_pixel256*>(srcgfx), reinterpret_cast<c_pixel256*>(destgfx), blitrect, srcx, srcy, srcw, destw, alphamask, blitmode);
    else
      blitline_88xlat(reinterpret_cast<c_pixel256*>(srcgfx), reinterpret_cast<c_pixel256*>(destgfx), blitrect, srcx, srcy, srcw, destw, alphamask, blitmode, palette);
  }
}

//###################################################################
//                         FILL-functions
//###################################################################

// was DM2_FILL_4BPP_PICT_LINE
void c_blitter::fill_line_4(ui16 ofs, ui16 pixels, c_pixel16 pix16)
{
  bool flag = IS_ODD(ofs);
  ofs /= 2;
  if (flag)
  {
    bmpdata_dest16[ofs] = build_pixels16(bmpdata_dest16[ofs].getl(), pix16.getr());
    ofs++;
    pixels--;
  }

  c_pixel16 dpix = build_pixels16(pix16.rtol(), pix16.getr());
  while (pixels >= 2)
  {
    bmpdata_dest16[ofs++] = dpix;
    pixels -= 2;
  }

  if (pixels > 0)
    bmpdata_dest16[ofs] = build_pixels16(pix16.rtol(), bmpdata_dest16[ofs].getr());
}

// was DM2_FILL_8BPP_PICT_LINE
void c_blitter::fill_line_8(ui16 ofs, ui16 pixels, c_pixel256 pixel256)
{
  c_pixel256* dest8 = bmpdata_dest256 + unsignedlong(ofs);
  if (is_odd_pointer(dest8))
  {
    *dest8++ = pixel256;
    pixels--;
  }

  i16 ww = pixels / 2; // in words
  if (ww != 0)
  {
    ui16* wpdest = CHGCAST(ui16, dest8);
    ui16 dpix = (pixel256.mkidx() << 8) | (pixel256.mkidx() & 0xff);
    while (ww--)
      *wpdest++ = dpix;
    dest8 = CHGCAST(c_pixel256, wpdest); // from ui16*
  }

  if (IS_ODD(pixels))
    *dest8 = pixel256;
}

// was DM2_FILL_RECT_4BPP_PICT
void c_blitter::fill_4(c_pixel16* gfxdata, c_pixel16 pix, ui16 pixels, c_rect* blitrect)
{
  bmpdata_dest16 = gfxdata;
  ui16 width = MK_EVEN(pixels + 1);
  ui16 ofs = blitrect->y * width + blitrect->x;

  for (i16 j=0; j < blitrect->h; j++, ofs += width)
    fill_line_4(ofs, blitrect->w, pix);
}

// was DM2_FILL_RECT_8BPP_PICT
void c_blitter::fill_8(c_pixel256* gfxdata, c_pixel256 pixel256, ui16 pixels, c_rect* blitrect)
{
  bmpdata_dest256 = gfxdata;
  ui16 ofs = blitrect->y * pixels + blitrect->x;

  for (i16 lines = 0; lines < blitrect->h; lines++, ofs += pixels)
    fill_line_8(ofs, blitrect->w, pixel256);
}

// in original, there was a request if gfxdata equals gfxsys.dm2screen
// if so, pixels gets overwritten by ORIG_SWIDTH
// this request got pulled before the routine in every call!
// was DM2_FILL_RECT_ANY
void c_blitter::fill(c_pixel* gfxdata, c_pixel pix, ui16 pixels, c_rect* blitrect, t_resolution res)
{
  if (res == BPP_4)
  {
    c_pixel16* pp = reinterpret_cast<c_pixel16*>(&pix);
    fill_4(reinterpret_cast<c_pixel16*>(gfxdata), *pp, pixels, blitrect);
  }
  else
  {
    c_pixel256* pp = reinterpret_cast<c_pixel256*>(&pix);
    fill_8(reinterpret_cast<c_pixel256*>(gfxdata), *pp, pixels, blitrect);
  }
}

//###################################################################
//                         STRETCH-functions
//###################################################################

// was DM2_CALC_STRETCHED_SIZE
i32 c_blitter::calc_stretched_size(i16 eaxw, i16 edxw)
{
  i32 longrg2 = signedlong(eaxw);
  i32 longrg1 = signedlong(edxw);
  longrg2 *= longrg1;
  longrg1 >>= 1;
  longrg1 += longrg2;
  longrg1 >>= 6;
  return longrg1;
}

// stretchptr added, was global - routine is definately superflous!
// was SKW_44c8_2143
void c_blitter::stretch16_sub1(c_pixel16* stretchptr, ui16 srcofs, ui16 destofs, ui16 srcfrac, i16 pixels)
{
  ui16 srcplus = srcfrac / 2;
  ui16 dest = destofs / 2;
  ui16 destlim = (destofs + pixels + 1) / 2;

  while (dest < destlim)
  {
    ui16 src = srcofs + (srcplus >> 7);
    ui8 umask;
#if 1 // TODO is that correct? cannot decide, IT IS NEVER CALLED, because the stretchptr is ever NULL!
    if (IS_ODD(src))
      umask = pixel_to_ui8(stretchptr[bmpdata_src16[src / 2].getr().mkidx()]);
    else
      umask = pixel_to_ui8(stretchptr[bmpdata_src16[src / 2].ltor().mkidx()]);
#else // maybe make it equiv to sub2
    if (IS_ODD(src))
      umask = pixel_to_ui8(stretchptr[bmpdata_src16[src / 2].rtol().mkidx()]);
    else
      umask = pixel_to_ui8(stretchptr[bmpdata_src16[src / 2].getl().mkidx()]);
#endif
    srcplus += srcfrac;
    umask <<= 4;

    src = srcofs + (srcplus >> 7);
    if (IS_ODD(src))
      bmpdata_dest16[dest] = stretchptr[bmpdata_src16[src / 2].getr().mkidx()] | umask;
    else
      bmpdata_dest16[dest] = stretchptr[bmpdata_src16[src / 2].ltor().mkidx()] | umask;
    srcplus += srcfrac;
    dest++;
  }
}

// was SKW_44c8_20e5
void c_blitter::stretch16_sub2(ui16 srcofs, ui16 destofs, ui16 srcfrac, i16 pixels)
{
  ui16 srcbase = srcfrac / 2;
  ui16 dest = destofs / 2;
  pixels = (pixels + 1) / 2;

  while (--pixels != -1)
  {
    ui16 src = (srcbase >> 7) + srcofs;
    ui8 umask;
    if (IS_ODD(src))
      umask = pixel_to_ui8(bmpdata_src16[src / 2].rtol());
    else
      umask = pixel_to_ui8(bmpdata_src16[src / 2].getl());
    srcbase += srcfrac;

    src = (srcbase >> 7) + srcofs;
    if (IS_ODD(src))
      bmpdata_dest16[dest] = bmpdata_src16[src / 2].getr() | umask;
    else
      bmpdata_dest16[dest] = bmpdata_src16[src / 2].ltor() | umask;
    srcbase += srcfrac;
    dest++;
  }
}

// stacksize was 0x30
// was DM2_STRETCH_BLIT_TO_MEMORY_4TO4BPP
// (there is only one call, and then the stretchptr is NULL!)
void c_blitter::stretch16(c_pixel16* srcgfx, c_pixel16* destgfx, i16 width, i16 height, i16 xpixels, i16 totalpixels, c_pixel16* stretchptr)
{
  i16 olddeltax = 0; // TODO added: needs initialization, was vw_0c

  bmpdata_src16 = srcgfx;
  bmpdata_dest16 = destgfx;
  i16 evenx = MK_EVEN(xpixels);
  ui16 srcfrac = (width << 7) / xpixels;
  ui16 deltay = (height << 7) / totalpixels;
  ui16 srcbase = deltay / 2;
  width = MK_EVEN(width);

  for (i16 n = 0; n < totalpixels; n++)
  {
    ui16 deltax = srcbase >> 7;
    if (deltax != olddeltax || n == 0)
    {
      if (stretchptr != NULL)
        stretch16_sub1(stretchptr, deltax * width, n * evenx, srcfrac, xpixels);
      else
        stretch16_sub2(deltax * width, n * evenx, srcfrac, xpixels);
    }
    else
    {
      bmpdata_src16 = destgfx;
      blitline_44_((n - 1) * evenx, n * evenx, xpixels);
      bmpdata_src16 = srcgfx;
    }

    olddeltax = deltax;
    srcbase += deltay;
  }
}

// #################################################################

// original BUG: here is a chance for an uninitialized RG5W. It cannot be the incoming, because that is a pointer,
// but here it is no pointer. For that I set a preinit of 0.
// However, this one is not used to stretch items in distance!
// stacksize was 0x2c
// was DM2_image_44c8_2351, TODO tell SPX new name
void c_blitter::stretch256(c_pixel256* srcgfx, c_pixel256* destgfx, i16 width, i16 height, i16 argw0, i16 argw1)
{
  i16 rg51 = 0; // BUGFIX init necessary
  c_pixel256* gfx1 = destgfx;
  bmpdata_src256 = srcgfx;
  bmpdata_dest256 = destgfx;
  i16 vw_04 = CUTX16((signedlong(width) << 7) / signedlong(argw0));
  i16 vw_10 = CUTX16((signedlong(height) << 7) / signedlong(argw1));
  i16 vw_08 = vw_10 / 2;
  //m_4407:
  for (i16 vw_0c = 0; vw_0c < argw1; vw_0c++)
  {
    //m_435B:
    i16 vw_18 = CUTX16(unsignedlong(vw_08) >> 7);
    if (rg51 != vw_18 || vw_0c == 0)
    {
      // TODO ugly!
      union _hlp
      {
        ui16 w;
        struct
        {
          ui8 blo;
          ui8 bhi;
        };
      };
      _hlp rg1;
      rg1.w = vw_04;
      //m_43A6:
      i16 count = argw0;
      c_pixel256* gfx1x = gfx1 + signedlong(argw0) * vw_0c;
      rg51 = vw_18 * width;
      c_pixel256* rg52p = srcgfx + unsignedlong(rg51) + unsignedlong(rg1.bhi);
      i8 rg4bhi = rg1.blo;
      rg1.w *= 2;
      i8 rg4blo = rg1.blo;
      i8 rg2blo = rg1.bhi;
      //m_43EC:
      for (;;)
      {
        *gfx1x++ = *rg52p;
        ui16 utmp = unsignedword(rg4bhi) + unsignedword(rg4blo);
        rg4bhi += rg4blo;
        bool carry = (utmp & uwcon(0xff00)) != 0; // was jul_add8 (BUG fixed version P1)
        rg52p += unsignedlong(rg2blo) + (carry ? 1 : 0);
        if (--count == 0)
          break;
      }
    }
    else
    {
      bmpdata_src256 = gfx1;
      blitter.blitline_88_(/*TAG srcx*/ (vw_0c - 1) * argw0, vw_0c * argw0, /*TAG pixels*/ argw0);
      bmpdata_src256 = srcgfx;
    }
    //m_43F7:
    rg51 = vw_18;
    vw_08 += vw_10;
  }
}

// #################################################################

#include "regs.h"
#include "emu.h"
#include "xtypes.h"

const i8 table1d255a[2] =
{
  0xf0, 0x0f
};

// was DM2_blit_44c8_1e43 TODO tell SPX new name
// obviously the upper byte of argw6 is used to indicate a mask at all (TODO: could be splitted)
void DM2_sub_blit_specialeffects(c_pixel* srcgfx, c_pixel* destgfx, c_pixel* gfx, c_rect* ecxrp, i16 xend, i16 srcofs, i16 argw2, i16 argw3, i16 argw4, i16 pixperline, i16 argw6, t_palette* palette)
{
  DM2_UPDATE_BLIT_PALETTE(palette);
  i16 width = ecxrp->w;
  i16 height = ecxrp->h;

  i32 pixofs = ecxrp->x + ecxrp->y * pixperline;
  i16 pixels = xend - srcofs;
  i16 destofs;
  blitter.bmpdata_src = srcgfx;
  blitter.bmpdata_dest = destgfx;

  if (gfx == NULL)
  {
    //m_3DC9:
    for (i16 h = 0; h < height; h++)
    {
      //m_3CF5:
      i16 w = width;
      i16 vw_1c = 0;
      //m_3D03:
      i16 wp;
      for (;;)
      {
        destofs = pixofs + vw_1c;
        wp = w;
        if (pixels > wp)
          break;
        if (argw6 >= 0)
          //m_3D32:
          blitter.blitline_48_ma(srcofs, destofs, pixels, /*TAG alpha*/ mkub(argw6));
        else
          blitter.blitline_48_(srcofs, destofs, pixels);
        //m_3D4C:
        w -= pixels;
        vw_1c += pixels;
        pixels = xend;
        srcofs = 0;
      }
      //m_3D5C:
      if (wp != 0)
      {
        if (argw6 >= 0)
          //m_3D7F:
          blitter.blitline_48_ma(srcofs, destofs, /*TAG pixels*/ wp, /*TAG alpha*/ mkub(argw6));
        else
          blitter.blitline_48_(srcofs, destofs, /*TAG pixels*/ wp);
        //m_3D99:
        pixels -= w;
        srcofs += w;
      }
      //m_3DA1:
      if (IS_ODD(width))
      {
        if (--pixels == 0)
        {
          //m_3DB7:
          pixels = xend;
          srcofs = 0;
        }
        else
          srcofs++;
      }
      //m_3DBD:
      pixofs += pixperline;
    }
  }
  else
  {
    //m_3DDD:
    argw4 = MK_EVEN(argw4 + 1);
    i16 vw_08 = argw2 + argw3 * argw4;
    //m_3F7C:
    for (i16 h = 0; h < height; h++)
    {
      //m_3E06:
      i16 destofs = width;
      i16 vw_1c = vw_08;
      //m_3E12:
      for (;;)
      {
        if (destofs == 0)
          break;
        i16 rg2l = unsignedlong(pixel_to_ui8(gfx[vw_1c / 2]));
        if ((unsignedlong(table1d255a[vw_1c & 0x1]) & rg2l) != 0)
          break;
        destofs--;
        vw_1c++;
        srcofs++;
        if (--pixels == 0)
        {
          srcofs = 0;
          pixels = xend;
        }
      }
      //m_3E56:
      vw_1c -= vw_08;
      i16 w = destofs;
      destofs = vw_08 + width;
      //m_3E68:
      for (;;)
      {
        destofs--;
        if (w == 0)
          break;
        i32 rg2l = unsignedlong(pixel_to_ui8(gfx[destofs / 2]));
        if ((unsignedlong(table1d255a[destofs & 0x1]) & rg2l) != 0)
          break;
        w--;
      }
      //m_3EA0:
      if (w > 0)
      {
        //m_3EAC:
        for (;;)
        {
          destofs = pixofs + vw_1c;
          if (pixels > w)
            break;
          if (argw6 >= 0)
            //m_3EDB:
            blitter.blitline_48_ma(srcofs, destofs, pixels, /*TAG alpha*/ mkub(argw6));
          else
            blitter.blitline_48_(srcofs, destofs, pixels);
          //m_3EF5:
          w -= pixels;
          vw_1c += pixels;
          pixels = xend;
          srcofs = 0;
        }
        //m_3F05:
        if (w != 0)
        {
          if (argw6 >= 0)
            //m_3F28:
            blitter.blitline_48_ma(srcofs, destofs, /*TAG pixels*/ w, /*TAG alpha*/ mkub(argw6));
          else
            blitter.blitline_48_(srcofs, destofs, /*TAG pixels*/ w);
          //m_3F44:
          pixels -= w;
          srcofs += w;
        }
      }
      //m_3F4C:
      if (IS_ODD(width))
      {
        if (--pixels == 0)
        {
          //m_3F62:
          pixels = xend;
          srcofs = 0;
        }
        else
          srcofs++;
      }
      //m_3F68:
      vw_08 += argw4;
      pixofs += pixperline;
    }
  }
}

// was DM2_blit_44c8_20a4
// used for drawing teleporters, rain and other special things
void DM2_blit_specialeffects(c_pixel* srcgfx, c_pixel* destgfx, c_pixel* gfx, c_rect* ecxrp, i16 argw0, i16 argw1, i16 argw2, i16 argw3, t_palette* palette)
{
  DM2_sub_blit_specialeffects(srcgfx, destgfx, gfx, ecxrp, argw0, argw1, 0, 0, ecxrp->w, argw2, argw3, palette);
}

// ###############################################################

// belongs to DM2_guidraw_0b36_105b
// was DM2_guidraw_44c8_1aca, TODO tell SPX new name
// still, this one does not draw items in distance
void c_blitter::stretch_4to8(c_pixel256* destgfx, c_rect* rect, ui8 mask, i16 width)
{
  c_pixel16 vca_00[160];
  c_pixel16 vca_a0[160];

  i16 i = 0;

  if (rect == NULL)
    return;

  DM2_UPDATE_BLIT_PALETTE(paldat.palette);

  ui8 alpha = mask ^ 0xf;
  ui8 xmask = alpha | (mask << 4); // left nibble 0

  blitter.bmpdata_src16 = vca_00;
  blitter.bmpdata_dest256 = destgfx;

  i16 pixels = rect->w;
  if (pixels <= 0)
    return;

  if (rect->h == 0)
    return;

  //m_3899:
  for (i = 0; i < (pixels + 1) / 2; i++)
  {
    //m_3884:
    vca_00[i].set(xmask);
    vca_a0[i].set(~xmask);
  }

  i16 destofs = rect->x + rect->y * width;
  bool flag = (((destofs / width) ^ destofs) & 0x1) != 0;
  //m_3925:
  for (i = 0; i < rect->h; i++, destofs += width)
  {
    //m_38D8:
    if (flag)
      blitter.bmpdata_src16 = vca_a0;
    else
      blitter.bmpdata_src16 = vca_00;
    flag = !flag;
    blitter.blitline_48_ma(0, destofs, pixels, alpha);
  }
}
