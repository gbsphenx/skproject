#ifndef C_GFX_MISC_H
  #define C_GFX_MISC_H

  #ifndef TYPES_H
    #include "..\types.h"
  #endif

  void SKW_INIT_BACKBUFF(void);
  void SKW_DRAW_MIRRORED_PIC(t_gfxdata* eaxpg, t_gfxdata* edxpg);
  void SKW_CHANGE_VIEWPORT_TO_INVENTORY(bool eaxbool); // calls user interrupt
  void SKW_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN(void);
  void SKW_DRAW_DIALOGUE_PROGRESS(x32 eaxl);
  void SKW_DRAW_DIALOGUE_PARTS_PICT(t_gfxdata* srcbmp, x16 ebxw, t_palette* palette, c_rect* edxr);
  void SKW_DRAW_DIALOGUE_PICT(t_gfxdata* srcbmp, c_rect* ebxr, x16 srcofs, t_gfxdata* destbmp, x16 argw0, x16 argw1, t_palette* palette);

#endif
