#ifndef C_GFX_FILL_H
  #define C_GFX_FILL_H

  #ifndef TYPES_H
    #include "..\types.h"
  #endif

  void SKW_FIRE_FILL_RECT_ANY(t_gfxdata* eaxpg, x8 pixel256, x16 ecxw, c_rect* edxr, x16 argw0);
  void SKW_FILL_ENTIRE_PICT(t_gfxdata* eaxpg, x8 pixel256);
  void SKW_FIRE_FILL_BACKBUFF_RECT(c_rect* eaxr, x8 pixel256);
  void SKW_FIRE_FILL_SCREEN_RECT(x16 eaxw, x8 pixel256);
  void SKW_FIRE_FILL_HALFTONE_RECTI(x16 eaxw, x16 edxw);
  void SKW_44c8_1dfc(c_rect* eaxr, x8 pixel256);
  void SKW_FILL_RECT_SUMMARY(c_c5rects* eaxps, x8 pixel256, c_rect* edxr);

#endif
