#ifndef C_LOADLEVEL_H
  #define C_LOADLEVEL_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  void DM2_LOAD_LOCALLEVEL_DYN(void);
  void DM2_LOAD_LOCALLEVEL_GRAPHICS_TABLE(i32 eaxl, i32 edxl, i32 ebxl);
  void DM2_LOAD_NEWMAP(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl);

#endif
