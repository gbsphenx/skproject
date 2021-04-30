#ifndef C_GFX_BLIT_H
  #define C_GFX_BLIT_H

  #ifndef NULL
    #define NULL (0)
  #endif

  #ifndef TYPES_H
    #include "types.h"
  #endif

  #ifndef C_GFX_PIXEL_H
    #include "c_gfx_pixel.h"
  #endif

  class c_rect;
  class t_palette;

  // Note: c_blitter can blit in different ways
  // from/to bitmaps and gfxsys.dm2screen,
  // but not to gfxsys.dm2mscreen

  class c_blitter
  {
    protected:
      union
      {
        c_pixel256* bmpdata_src256; // was colidxptr1
        c_pixel16* bmpdata_src16;
        c_pixel* bmpdata_src; // TODO: type 16/256 not revealed yet
      };
      union
      {
        c_pixel256* bmpdata_dest256; // was colidxptr2
        c_pixel16* bmpdata_dest16;
        c_pixel* bmpdata_dest; // TODO: type 16/256 not revealed yet
      };

    protected:
      void blit_within_screen(c_pixel256* screen, c_rect* rectp, i16 yofs);

      void blitline_44_(ui16 srcofs, ui16 destofs, ui16 pixels);
      void blitline_44_ma(ui16 srcofs, ui16 destofs, ui16 pixels, ui8 alpha);
      void blitline_44_mi(ui16 srcofs, ui16 destofs, ui16 pixels);
      void blitline_44_mima(ui16 srcofs, ui16 destofs, ui16 pixels, ui8 alpha);
      void blitline_44(c_pixel16* srcgfx, c_pixel16* destgfx, c_rect* blitrect, i16 srcx, i16 srcy, ui16 srcw, ui16 destw, t_alphamask alphamask, e_blitmode blitmode);

      void blitline_48_(ui16 srcofs, ui16 destofs, ui16 pixels);
      void blitline_48_ma(ui16 srcofs, ui16 destofs, ui16 pixels, ui8 alpha);
      void blitline_48_mi(ui16 srcofs, ui16 destofs, ui16 pixels);
      void blitline_48_mima(ui16 srcofs, ui16 destofs, ui16 pixels, ui8 alpha);
      void blitline_48(c_pixel16* srcgfx, c_pixel256* destgfx, c_rect* blitrect, i16 srcofs, i16 srcy, ui16 srcw, ui16 destw, t_alphamask alphamask, e_blitmode blitmode, t_palette* palette);

      void blitline_88_(ui16 srcofs, ui16 destofs, ui16 pixels);
      void blitline_88_ma(ui16 srcofs, ui16 destofs, ui16 pixels, ui8 alpha);
      void blitline_88_mi(ui16 srcofs, ui16 destofs, ui16 pixels);
      void blitline_88_mima(ui16 srcofs, ui16 destofs, ui16 pixels, ui8 alpha);
      void blitline_88(c_pixel256* srcgfx, c_pixel256* destgfx, c_rect* blitrect, i16 srcx, i16 srcy, ui16 srcw, ui16 destw, t_alphamask alphamask, e_blitmode blitmode);

      void blitline_88xlat_(ui16 srcofs, ui16 destofs, ui16 pixels, t_palette* palette);
      void blitline_88xlat_ma(ui16 srcofs, ui16 destofs, ui16 pixels, ui8 alpha, t_palette* palette);
      void blitline_88xlat_mi(ui16 srcofs, ui16 destofs, ui16 pixels, t_palette* palette);
      void blitline_88xlat_mima(ui16 srcofs, ui16 destofs, ui16 pixels, ui8 alpha, t_palette* palette);
      void blitline_88xlat(c_pixel256* srcgfx, c_pixel256* destgfx, c_rect* blitrect, i16 srcx, i16 srcy, ui16 srcw, ui16 destw, t_alphamask alphamask, e_blitmode blitmode, t_palette* palette);

      void fill_line_4(ui16 ofs, ui16 pixels, c_pixel16 pix16);
      void fill_line_8(ui16 ofs, ui16 pixels, c_pixel256 pixel256);
      void fill_4(c_pixel16* gfxdata, c_pixel16 pix, ui16 pixels, c_rect* blitrect);
      void fill_8(c_pixel256* gfxdata, c_pixel256 pixel256, ui16 pixels, c_rect* blitrect);
      void stretch16_sub1(c_pixel16* stretchptr, ui16 srcofs, ui16 destofs, ui16 srcfrac, i16 pixels);
      void stretch16_sub2(ui16 eaxw, ui16 destofs, ui16 srcfrac, i16 pixels);

// TODO
      friend class c_imgdecode;
      friend void DM2_sub_blit_specialeffects(c_pixel* srcgfx, c_pixel* destgfx, c_pixel* gfx, c_rect* ecxrp, i16 xend, i16 srcofs, i16 argw2, i16 argw3, i16 argw4, i16 pixperline, i16 argw6, t_palette* palette);
      friend void DM2_SCROLLBOX_MESSAGE(void);

    public:
      void init(void);

      void blit(c_pixel* srcgfx, c_pixel* destgfx, c_rect* blitrect, i16 srcx, i16 srcy, ui16 srcw, ui16 destw, t_alphamask alphamask, e_blitmode blitmode, t_resolution sbpp, t_resolution dbpp, t_palette* palette = NULL);

      void fill(c_pixel* gfxdata, c_pixel pix, ui16 pixels, c_rect* blitrect, t_resolution res);

      i32 calc_stretched_size(i16 eaxw, i16 edxw);
      void stretch16(c_pixel16* srcgfx, c_pixel16* destgfx, i16 width, i16 height, i16 xpixels, i16 totalpixels, c_pixel16* stretchptr);
      void stretch256(c_pixel256* srcgfx, c_pixel256* destgfx, i16 width, i16 height, i16 argw0, i16 argw1);
      void stretch_4to8(c_pixel256* destgfx, c_rect* rect, ui8 mask, i16 width);
  };

  extern c_blitter blitter;

  void DM2_blit_specialeffects(c_pixel* srcgfx, c_pixel* destgfx, c_pixel* gfx, c_rect* ecxrp, i16 argw0, i16 argw1, i16 argw2, i16 argw3, t_palette* palette);

#endif
