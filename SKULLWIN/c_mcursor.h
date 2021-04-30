#ifndef C_MCURSOR_H
  #define C_MCURSOR_H

  #ifndef T_GFX_PAL_H
    #include "c_gfx_pal.h"
  #endif

  #ifndef C_RECT_H
    #include "c_rect.h"
  #endif

  #define MOUSECURSORFILESIZE2 (0x90)

  struct s_cursor2 // size: MOUSECURSORFILESIZE2
  {
    c_pixel16 pixel[0x80];
    c_rect rect1;
    c_rect rect2;
  };

  extern s_cursor2 mouse_cur2;

  struct s_mcursor // size 0x246
  {
    i8 hx; // @00 hotspot x
    i8 hy; // @01 hotspot y
    i8 w;  // @02 w
    i8 h;  // @03 h
    i8 alphamask; // @04 alphamask
    i8 dummy; // @05 not requested - probably just for alignment
    c_pixel256 pixel[0x240]; // @06 graphics data (4608 set/notset-pixel)
  };

  extern s_mcursor DRV_cursor[4];

  void init_mousecursors(void);

  void generate_cursor(c_pixel* srcmap, i16 cursoridx, i16 hx, i16 hy, i16 w, i16 h, t_resolution res, t_palette* palette, ui8 midx);
  void DM2_INITBASICCURSORS(void);

#endif
