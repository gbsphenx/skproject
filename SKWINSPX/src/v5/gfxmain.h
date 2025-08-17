#ifndef C_GFX_MAIN_H
  #define C_GFX_MAIN_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  #ifndef DM2GLOBAL_H
    #include <dm2global.h> // ORIG_SWIDTH and ORIG_SHEIGHT
  #endif

  #ifndef C_RECT_H
    #include <skrect.h>	// c_rect
  #endif

  #ifndef C_GFX_BMP_H
    #include <gfxbmp.h>
  #endif

  class c_buttongroup;

  class c_gfx_system
  {
    public:
      t_bmp* bitmapptr;
      i16 backbuff1;
      i16 backbuff2;
      c_rect backbuffrect;
      i16 backbuffer_w; // vx1d270a
      i16 backbuffer_h; // vx1d270c
      c_pixel256 dm2screen[ORIG_SWIDTH * ORIG_SHEIGHT];
      c_pixel256 dm2mscreen[ORIG_SWIDTH * ORIG_SHEIGHT]; //HOTTAG
      t_bmp* pictbuff; // was v1e1414

      void init(void);
  };

  extern c_gfx_system gfxsys;

  void DM2_INIT_BACKBUFF(void);
  void DM2_gfxmain_3929_0914(void);
  void DM2_gfxmain_3929_0929(i16 eaxw, t_text* edxtp);
  void DM2_gfxmain_0b36_0cbe(c_buttongroup* buttongroup, bool alloc);

  void blit_toscreen(c_pixel* srcgfx, c_rect* blitrect, i16 srcx, i16 srcy, ui16 srcw, t_alphamask alphamask, e_blitmode blitmode, t_resolution sbpp, t_resolution dbpp, t_palette* palette = NULL);

  void DM2_FILL_BACKBUFF_RECT(c_rect* blitrect, c_pixel pixel);
  void DM2_FILL_SCREEN_RECT(i16 query, c_pixel pixel);
  void DM2_FILL_FULLSCREEN(c_rect* blitrect, c_pixel pixel);
  void DM2_FILL_ENTIRE_PICT(t_bmp* bmp, c_pixel pix);
  void DM2_FILL_HALFTONE_RECTI(i16 query);
  void DM2_FADE_SCREEN(i16 mode);
  void DM2_DRAWINGS_COMPLETED(void);

#endif
