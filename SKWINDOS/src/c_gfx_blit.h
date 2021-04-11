#ifndef C_GFX_BLIT_H
  #define C_GFX_BLIT_H

  #ifndef TYPES_H
    #include "..\types.h"
  #endif

  void SKW_FIRE_UPDATE_BLIT_PALETTE(t_palette* palette);
  void SKW_44c8_08ae(x16 srcofs, x32 ebxl, x8 ecxb, x16 edxw);
  void DM2_blit8pppixel(ui16 srcofs, x16 width, ui16 destofs);
  void R_15B5(x16 srcofs, x16 ebxw, x16 edxw);
  void SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(x16 srcofs, x16 ebxw, x16 edxw);
  void SKW_FIRE_BLIT_PICTURE(t_gfxdata* srcbmp, c_rect* rect, ui16 srcofs, t_gfxdata* destbmp, x16 argw0, x16 argw1, x16 argw2, x16 argw3, x16 argw4, e_bitres srcbits, e_bitres destbits, t_palette* palette = NULL);

#endif
