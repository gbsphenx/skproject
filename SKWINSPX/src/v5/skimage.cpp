#include <util.h>
#include <dm2data.h>
#include <gfxblit.h>
#include <bgdat.h>
#include <skgdtqdb.h>
#include <skalocdb.h>
#include <gfxmain.h>
#include <skmap.h>
#include <skxrect.h>
#include <startend.h>
#include <fileio.h>
#include <skimage.h>
#include <uikeybd.h>
#include <dm2debug.h>

c_image dm2_image1;
c_image dm2_image2;

// TODO: fileoperations should be completely in fileio.h
void init_global_images(void)
{
	SPX_DEBUG_PRINT("INIT_GLOBAL_IMAGES:INIT\n");
	// img1.dat is exactly 314 bytes long
  //DM2_READ_BINARY("img1.dat", VCAST(&dm2_image1), sizeof(c_image));	/// MSVC6 compiles c_image at 316 bytes, which is too long
  DM2_READ_BINARY("bin/v5/img1.dat", VCAST(&dm2_image1), 314);

  dm2_image2.init();
}

void c_imgdesc::init(void)
{
  bmp = NULL;
  mode = 0;
  w_06 = 0;
  b_08 = 0;
  b_09 = 0;
  b_0a = 0;
  b_0b = 0;
  bmpid = 0;
  x = 0;
  y = 0;
  width = 0;
  height = 0;
  w_16 = 0;
}

void c_image::init(void)
{
  imgdesc.init();
  query1 = 0;
  w_1a = 0;
  w_1c = 0;
  w_1e = 0;
  srcx = 0;
  srcy = 0;
  rect.init();
  bmp = NULL;
  alphamask = 0;
  blitmode = 0;
  w_34 = 0;
  w_36 = 0;
  colors = 0;
  for (int i = 0; i < PAL256; i++)
    color_to_palettecolor(&palette[i], E_COL00);
}

// Note: result gets only compared against 0
static i32 DM2_0b36_068f(c_image* image)
{
  return mkul(image->w_36 & 0x1f) | (mkul(image->w_34 & wcon(0x7f)) << 5) | ((mkul(image->imgdesc.w_06 & 0x1fff)) << 12);
}

void DM2_image_0b36_11c0(c_image* image, c_buttongroup* buttongroup, i16 ebxw, i16 alphamask)
{
  c_rect rc_00;
  i16 w = 0; // TODO: had to add init, there's a path!
  i16 h = 0; // TODO: had to add init, there's a path!

  image->bmp = dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ buttongroup->button.dbidx);
  if (ebxw != -1)
  {
    //m_BFD7:
    w = image->imgdesc.width;
    h = image->imgdesc.height;
    if (DM2_QUERY_BLIT_RECT(DM2_QUERY_PICT_BITS(&image->imgdesc), &rc_00, ebxw, IO w, IO h) == NULL)
      return;
  }
  else
    rc_00 = image->rect; // copy

  //m_C009:
  image->imgdesc.width += w;
  image->imgdesc.height += h;
  DM2_OFFSET_RECT(buttongroup, &image->rect, &rc_00);
  image->query1 = 0xffff;
  image->alphamask = alphamask;
  DM2_DRAW_PICST(image);
  DM2_ADJUST_BUTTONGROUP_RECTS(buttongroup, &rc_00);
}

c_image* DM2_QUERY_PICST_IT(c_image* image)
{
  c_image img;
  c_imgdesc imgdesc;
  i16 vw_154;
  i16 vw_15c;
  t_dbidx dbidx; // was vw_160;

  bool flag1 = (image->w_34 != 0x40 || image->w_36 != 0x40);
  bool flag2 = (image->imgdesc.w_06 != -1 || image->imgdesc.bmpid < 0);
  //m_B648:
  if (flag1)
  {
    image->w_1c = CUTX16(blitter.calc_stretched_size(image->w_1c, image->w_34));
    image->w_1e = CUTX16(blitter.calc_stretched_size(image->w_1e, image->w_36));
  }
  if ((image->blitmode & wcon(0x1)) != 0)
    image->w_1c = -image->w_1c;
  if (image->alphamask == wcon(0xfffd))
    return image;

  t_dbidx id;
  if (flag1) // was || RG3L != 0 here, but that's 0 ever
  {
    bool skip00102 = false;
    //m_B6CE:
    if (flag2)
    {
      //m_B70A:
      if (!dm2_dballochandler.DM2_dballoc_3e74_55f9(mkul(DM2_0b36_068f(image)), OO dbidx))
      {
        //m_B72A:
        imgdesc = image->imgdesc; // copy imgdesc
        DM2_QUERY_PICST_IMAGE(&imgdesc);
        skip00102 = true;
      }
      else
        id = dbidx;
    }
    else
    {
      dbidx = dm2_dballochandler.DM2_dballoc_3e74_5888();
      imgdesc = image->imgdesc; // copy imgdesc
      DM2_SET_IMAGE(imgdesc.bmpid, &imgdesc);
      skip00102 = true;
    }

    if (skip00102)
    {
      //m_B749:
      DM2_ALLOCATE_IMG_COPY(&imgdesc);
      if ((imgdesc.mode & 0x10) != 0)
      {
        img = *image; // copy image
        img.blitmode = 0;
        img.w_34 = img.w_36 = 0x40;
        if (!dm2_dballochandler.DM2_dballoc_3e74_55f9(mkul(DM2_0b36_068f(&img)), OO /*TAG dbidx*/ vw_15c))
        {
          c_pixel* destgfx = DM2_ALLOC_NEW_BMP(vw_15c, imgdesc.width, imgdesc.height, imgdesc.getres());
          c_pixel* srcgfx = PIXELCAST(DM2_QUERY_PICT_BITS(&imgdesc));

          ui16 destw;
          if (destgfx == gfxsys.dm2screen)
            destw = ORIG_SWIDTH;
          else
            destw = getbmpheader(BMPCAST(destgfx))->width;

          blitter.blit(
            srcgfx,
            destgfx,
            tmprects.alloc_origin_tmprect(imgdesc.width, imgdesc.height),
            imgdesc.x,
            imgdesc.y,
            getbmpheader(BMPCAST(srcgfx))->width, // TODO was height here?!?
            destw,
            NOALPHA,
            BLITMODE0,
            imgdesc.getres(),
            imgdesc.getres());
        }
        //m_B83C:
        DM2_image_0b36_01cd(&imgdesc);
        DM2_SET_IMAGE((t_dbidx)vw_15c, &imgdesc);
        DM2_ALLOCATE_IMG_COPY(&imgdesc);
      }
      //m_B86A:
      if (flag1)
      {
        i16 rg62 = CUTX16(blitter.calc_stretched_size(imgdesc.width, image->w_34));
        i16 rg4 = CUTX16(blitter.calc_stretched_size(imgdesc.height, image->w_36));
        if (rg62 > 0 && rg4 > 0)
        {
          //m_B8BF:
          vw_154 = rg4;
          i16 rg72 = rg62;
          DM2_ALLOC_NEW_BMP(dbidx, rg72, vw_154, imgdesc.getres());
          c_pixel* gfx3 = PIXELCAST(DM2_QUERY_PICT_BITS(&imgdesc));
          c_pixel* gfx1 = PIXELCAST(DM2_SET_IMAGE(dbidx, &image->imgdesc));
          if (imgdesc.getres() != BPP_8)
            //m_B942:
            blitter.stretch16(reinterpret_cast<c_pixel16*>(gfx3), reinterpret_cast<c_pixel16*>(gfx1), imgdesc.width, imgdesc.height, rg72, vw_154, NULL);
          else
            blitter.stretch256(reinterpret_cast<c_pixel256*>(gfx3), reinterpret_cast<c_pixel256*>(gfx1), imgdesc.width, imgdesc.height, rg72, vw_154);
        }
        else
        {
          image->imgdesc.width = rg62;
          image->imgdesc.height = rg4;
        }
      }
      //m_B96A:
      DM2_image_0b36_01cd(&imgdesc);
      dm2_dballochandler.DM2_ALLOCATION1(dbidx, false);
      return image;
    }
  }
  else
  {
    if (flag2)
    {
      //m_B6C2:
      DM2_QUERY_PICST_IMAGE(&image->imgdesc);
      return image;
    }
    id = image->imgdesc.bmpid;
  }
  //m_B6B3:
  DM2_SET_IMAGE(id, &image->imgdesc);
  return image;
}

void DM2_DRAW_PICST(c_image* image)
{
  i16 srcx;
  i16 srcy;

  if (image->imgdesc.width <= 0 || image->imgdesc.height <= 0)
    return;

  t_bmp* srcbmp = DM2_QUERY_PICT_BITS(&image->imgdesc);
  i16 query1 = image->query1;

  if (query1 != -1)
  {
    i16 y;
    //m_B9DA:
    if ((query1 & 0x8000) == 0 && image->w_1c == 0 && image->w_1e == 0)
    {
      //m_BA0D:
      srcx = image->imgdesc.width;
      y = image->imgdesc.height;
    }
    else
    {
      query1 |= 0x8000;
      srcx = image->srcx + image->w_1c;
      y = image->srcy + image->w_1e;
    }

    //m_BA19:
    srcy = y;
    if (DM2_QUERY_BLIT_RECT(UPCAST(t_bmp, srcbmp), &image->rect, query1, IO srcx, IO srcy, image->w_1a) == NULL)
      return;
  }
  else
  {
    srcx = image->srcx;
    srcy = image->srcy;
  }

  //m_BA43:
  srcx += image->imgdesc.x;
  srcy += image->imgdesc.y;

  i16 w = getbmpheader(srcbmp)->width;
  i16 deltax = image->rect.w + srcx;
  if (w > deltax && ((image->blitmode & 0x1) != 0))
    w -= deltax;
  else
    //m_BA79:
    w = 0;

  //m_BA7B:
  if ((image->blitmode & 0x1) != 0)
    srcx = 0;
  srcx += w;

  i16 h = getbmpheader(srcbmp)->height;
  i16 deltay = image->rect.h + srcy;
  if (h > deltay && (image->blitmode & 0x2) != 0)
    h -= deltay;
  else
    //m_BABC:
    h = 0;

  //m_BABE:
  if ((image->blitmode & 0x2) != 0)
    srcy = 0;
  srcy += h;

  t_resolution res;
  ui16 destw;
  if (PIXELCAST(image->bmp) != gfxsys.dm2screen)
  {
    //m_BAF3:
    destw = getbmpheader(image->bmp)->width;
    res = getbmpheader(image->bmp)->res;
  }
  else
  {
    destw = ORIG_SWIDTH;
    res = BPP_8;
  }

  //m_BAFE:
  if (image->alphamask != -2)
  {
    t_palette* pal;
    if (image->colors > 0)
      //m_BB14:
      pal = image->palette;
    else
      pal = NULL;
    //m_BB17:
    if (PIXELCAST(image->bmp) == gfxsys.dm2screen)
      destw = ORIG_SWIDTH;

    blitter.blit(
      PIXELCAST(srcbmp),
      PIXELCAST(image->bmp),
      &image->rect,
      srcx,
      srcy,
      getbmpheader(srcbmp)->width,
      destw,
      image->alphamask,
      (e_blitmode)image->blitmode,
      getbmpheader(srcbmp)->res,
      res,
      pal);
  }

  //m_BB55:
  image->srcx = srcx;
  image->srcy = srcy;
}

// SKW_32cb_09a7
i16 DM2_QUERY_MULTILAYERS_PIC(c_image* image, i8 edxb, i8 ebxb, i8 ecxb, i16 argw0, i16 argw1, i16 argw2, i16 argw3, i16 argw4, i16 alphamask)
{
  DM2_QUERY_TEMP_PICST(
    argw3, argw0, argw1, 0,
    0, argw2, -1, -1,
    argw4, -1, edxb, ebxb, ecxb);

// TODO:
// dm2_image2 ist die Wand mit dem Fensterinhalt und ist immer okay.
// Der Bug liegt beim Maskenbild.
  *image = dm2_image2; // copy image

// bleibt eigentlich nur diese Routine:
// das Image wurde kopiert, muß aber nun in den Cache eingebunden werden
  DM2_ALLOCATE_IMG_COPY(&image->imgdesc);

  image->alphamask = alphamask;
  image->rect.set_origin(image->imgdesc.width, image->imgdesc.height);

  i16 idx = /*TAG dbidx*/ dm2_dballochandler.DM2_dballoc_3e74_5888();
  image->bmp = DM2_ALLOC_NEW_BMP(idx, image->imgdesc.width, image->imgdesc.height, BPP_8);

#if 0
t_bmp* bp = image->bmp; //dm2_dballochandler.DM2_GET_BMP(image->bmp);
c_rect r; r.set(0, 0, getbmpheader(bp)->width, getbmpheader(bp)->height);
DEBUGBLIT(bp, &r, getbmpheader(bp)->width, BPP_8, NULL);
#endif

  return idx;
}

// was DM2_image_0b36_00c3, TODO: tell SPX new name
t_bmp* DM2_SET_IMAGE(t_dbidx bmpid, c_imgdesc* imgdesc)
{
  t_bmp* bmp = dm2_dballochandler.DM2_GET_BMP(bmpid);
  if (imgdesc != NULL)
  {
    imgdesc->bmp = bmp;
    imgdesc->mode = 0x8;
    imgdesc->bmpid = bmpid;
    imgdesc->x = 0;
    imgdesc->y = 0;
    imgdesc->width = getbmpheader(bmp)->width;
    imgdesc->height = getbmpheader(bmp)->height;
    imgdesc->setres(getbmpheader(bmp)->res);
  }
  return bmp;
}

// was DM2_image_0b36_018f, TODO tell SPX new name
void DM2_ALLOCATE_IMG_COPY(c_imgdesc* imgdesc)
{
  if ((imgdesc->mode & 0x4) == 0)
  {
    if ((imgdesc->mode & 0x8) != 0)
      dm2_dballochandler.DM2_ALLOCATE_GFX256((t_dbidx)imgdesc->bmpid);
  }
  else
    dm2_dballochandler.DM2_ALLOCATE_GFX16(imgdesc->b_08, imgdesc->b_09, imgdesc->b_0b);
}

void DM2_image_0b36_01cd(c_imgdesc* imgdesc)
{
  if ((imgdesc->mode & 0x4) == 0)
  {
    if ((imgdesc->mode & 0x8) != 0)
      dm2_dballochandler.DM2_ALLOCATION1((t_dbidx)imgdesc->bmpid, false);
  }
  else
    dm2_dballochandler.DM2_dballoc_3e74_5992(imgdesc->b_08, imgdesc->b_09, imgdesc->b_0b);
}

void DM2_DRAW_TEMP_PICST(void)
{
  DM2_DRAW_PICST(&dm2_image2);
}

// was SKW_0b36_1757
// stacksize was 0x144
void DM2_DRAW_TRANSPARENT_STATIC_PIC(i8 eaxb, i8 edxb, i8 ebxb, i16 query1, i16 alphamask)
{
  c_image image;

  DM2_QUERY_GDAT_SUMMARY_IMAGE(&image, eaxb, edxb, ebxb);
  image.alphamask = alphamask;
  image.query1 = query1;
  DM2_DRAW_PICST(DM2_QUERY_PICST_IT(&image));
}

// stacksize was 0x144
void DM2_DRAW_STATIC_PIC(i8 eaxb, i8 edxb, i8 ebxb, i16 query1, i16 alphamask)
{
  c_image image;

  DM2_QUERY_GDAT_SUMMARY_IMAGE(&image, eaxb, edxb, ebxb);
  image.alphamask = alphamask;
  image.query1 = query1;
  image.bmp = gfxsys.bitmapptr;
  image.w_1c = 0;
  image.w_1e = 0;
  DM2_DRAW_PICST(DM2_QUERY_PICST_IT(&image));
}

// stacksize was 0x144
void DM2_DRAW_DUNGEON_GRAPHIC(i8 eaxb, i8 edxb, i8 ebxb, i16 query1, i16 alphamask, i16 blitmode)
{
  c_image image;

if (eaxb==8)
  int stop=1;
  DM2_QUERY_GDAT_SUMMARY_IMAGE(&image, eaxb, edxb, ebxb);
  DM2_query_B073(image.palette, CUTX8(ddat.v1e12d2), alphamask, -1, &image.colors);
  image.alphamask = alphamask;
  image.query1 = query1;
  image.bmp = gfxsys.bitmapptr;
  image.blitmode = blitmode;
  if (ddat.v1e025c != 0)
  {
    if (query1 != wcon(0x2bc))
    {
      //m_3E45E:
      if (query1 == wcon(0x2bd))
        image.srcy += ddat.v1d2710;
    }
    else
      image.srcy += ddat.v1d270e;
  }
  //m_3E470:
  DM2_DRAW_PICST(DM2_QUERY_PICST_IT(&image));
}

// belongs to DM2_guivp_32cb_00f1
i16 DM2_image_0b36_1446(c_imgdesc* imgdesc, i16 srcx, i16 srcy, ui8 alphamask)
{
  t_bmp* bmp = DM2_QUERY_PICT_BITS(imgdesc);
  if (bmp == NULL)
    return -1;
  t_resolution res = imgdesc->getres();
  i16 vw_1c = /*TAG dbidx*/ dm2_dballochandler.DM2_dballoc_3e74_5888();
  DM2_ALLOC_NEW_BMP(vw_1c, res == BPP_4 ? 2 : 1, 1, res);
  i16 vw_20 = /*TAG dbidx*/ dm2_dballochandler.DM2_dballoc_3e74_5888();
  t_bmp* srcbmp = DM2_ALLOC_NEW_BMP(vw_20, res == BPP_4 ? 2 : 1, 1, res);
  t_bmp* destbmp = dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ vw_1c);
  DM2_FILL_ENTIRE_PICT(destbmp, ui8_to_pixel(alphamask));
  DM2_FILL_ENTIRE_PICT(srcbmp, ui8_to_pixel(alphamask));

  ui16 destw;
  if (PIXELCAST(destbmp) == gfxsys.dm2screen)
    destw = ORIG_SWIDTH;
  else
    destw = res == BPP_4 ? 2 : 1;

  blitter.blit(
    PIXELCAST(DM2_QUERY_PICT_BITS(imgdesc)),
    PIXELCAST(destbmp),
    tmprects.alloc_origin_tmprect(1, 1),
    srcx,
    srcy,
    getbmpheader(bmp)->width,
    destw,
    alphamask,
    BLITMODE0,
    res,
    res);

  i16 vw_10 = 1;
  i16 n = CUTX16(DM2_CALC_IMAGE_BYTE_LENGTH(destbmp));
  //m_C326:
  c_pixel* destgfx = PIXELCAST(destbmp);
  c_pixel* srcgfx = PIXELCAST(srcbmp);
  for (i16 i = 0; i < n; i++)
  {
    //m_C312:
    if (*destgfx++ != *srcgfx++)
    {
      vw_10 = 0;
      break;
    }
  }
  //m_C332:
  dm2_dballochandler.DM2_dballoc_3e74_58a3((t_dbidx)vw_1c);
  dm2_dballochandler.DM2_dballoc_3e74_58a3((t_dbidx)vw_20);
  return vw_10;
}

void DEBUGBLIT(t_bmp* bitmap, c_rect* r, i16 srcw, t_resolution srcres, t_palette* palette)
{
  blitter.blit(
    PIXELCAST(bitmap),
    gfxsys.dm2screen,
    r,
    0,
    0,
    srcw,
    ORIG_SWIDTH,
    NOALPHA,
    BLITMODE0,
    srcres,
    BPP_8,
    palette
  );
//  for(;;);
  dm2keybd.getkey();
}