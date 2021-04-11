#ifndef C_GFX_PAL_H
  #define C_GFX_PAL_H

  #ifndef TYPES_H
    #include "..\types.h"
  #endif

  void SKW_FIRE_SELECT_PALETTE_SET(x8 index); // calls user interrupt
  // index may be 0 or 1

  void SKW_FIRE_FADE_SCREEN(bool eaxbool);

#endif
