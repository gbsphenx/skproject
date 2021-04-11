#ifndef C_GFX_DECODE_H
  #define C_GFX_DECODE_H

  #ifndef TYPES_H
    #include "..\types.h"
  #endif

  void SKW_DECODE_IMG3_UNDERLAY(x16* eaxp, xxx* edxp);
  void SKW_DECODE_IMG3_OVERLAY(xxx* eaxp, xxx* ebxp, xxx* edxp);

  void SKW_DECODE_IMG9(xxx* eaxp, x32 ebxl, xxx* edxp);

#endif
