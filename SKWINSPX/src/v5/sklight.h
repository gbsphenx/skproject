#ifndef C_LIGHT_H
  #define C_LIGHT_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  i32 DM2_RECALC_LIGHT_LEVEL(void);
  void DM2_ADD_BACKGROUND_LIGHT_FROM_TILE(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0);
  i32 DM2_CHECK_RECOMPUTE_LIGHT(void);
  void DM2_PROCEED_LIGHT(i32 eaxl, i32 edxl);

#endif
