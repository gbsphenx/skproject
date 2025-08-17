#ifndef C_IMAGE_H
  #define C_IMAGE_H

  #ifndef C_GFX_PAL_H
    #include <gfxpal.h>
  #endif

  #ifndef C_GFX_BMP_H
    #include <gfxbmp.h>
  #endif

  #ifndef C_RECT_H
    #include <skrect.h>
  #endif

  #ifndef C_ALLOC_H
    #include <skalloc.h>
  #endif

  class c_imgdesc // fix size 0x18, old name c_hex18
  {
    public:
      t_bmp* bmp; // @00
      i16 mode; // @04, possible bits: 0x4, 0x8, 0x10
      i16 w_06;
      i8 b_08;
      i8 b_09;
      i8 b_0a; // looks like it can be set, but is never requested
      i8 b_0b;
      t_dbidx bmpid; // @0c
      i16 x; // @0e
      i16 y; // @10
      i16 width; // @12
      i16 height; // @14
    protected:
      i16 w_16; // res (upper byte always 0)

    public:
      void init(void);
      c_imgdesc(void) { init(); }

      t_resolution getres(void) { return (t_resolution)w_16; }
      void setres(t_resolution res) { w_16 = mkuw(res) & 0xff; }
  };

  class c_image // fix size 0x13a
  {
    public:
      c_imgdesc imgdesc; // @00
      i16 query1; // @18
      i16 w_1a;
      i16 w_1c;
      i16 w_1e;
      i16 srcx; // @20
      i16 srcy; // @22
      c_rect rect; // @24
      t_bmp* bmp; // @2c
      i16 alphamask; // @30
      i16 blitmode; // e_blitmode @32
      i16 w_34;
      i16 w_36;
      i16 colors; // @38 number of colors in palette, -1 if no palette
      t_palette palette[PAL256]; // @3a

    public:
      void init(void);
      c_image(void) { init(); }
  };

  extern c_image dm2_image1; // was v1e1054
  extern c_image dm2_image2; // was v1e118e

  void init_global_images(void);

  class c_buttongroup;

  void DM2_image_0b36_11c0(c_image* image, c_buttongroup* buttongroup, i16 ebxw, i16 alphamask);
  c_image* DM2_QUERY_PICST_IT(c_image* image);
  void DM2_DRAW_PICST(c_image* image);
  i16 DM2_QUERY_MULTILAYERS_PIC(c_image* image, i8 edxb, i8 ebxb, i8 ecxb, i16 argw0, i16 argw1, i16 argw2, i16 argw3, i16 argw4, i16 alphamask);

  t_bmp* DM2_SET_IMAGE(t_dbidx bmpid, c_imgdesc* imgdesc);
  void DM2_ALLOCATE_IMG_COPY(c_imgdesc* imgdesc);
  void DM2_image_0b36_01cd(c_imgdesc* imgdesc);
  void DM2_DRAW_TEMP_PICST(void);
  void DM2_DRAW_TRANSPARENT_STATIC_PIC(i8 eaxb, i8 edxb, i8 ebxb, i16 query1, i16 alphamask);
  void DM2_DRAW_STATIC_PIC(i8 eaxb, i8 edxb, i8 ebxb, i16 query1, i16 alphamask);
  void DM2_DRAW_DUNGEON_GRAPHIC(i8 eaxb, i8 edxb, i8 ebxb, i16 query1, i16 alphamask, i16 blitmode);
  i16 DM2_image_0b36_1446(c_imgdesc* imgdesc, i16 srcx, i16 srcy, ui8 alphamask);

  void DEBUGBLIT(t_bmp* bitmap, c_rect* r, i16 srcw, t_resolution srcres, t_palette* palette);

#endif
