#ifndef C_CLOUD_H
  #define C_CLOUD_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  class c_tim;

  i32 DM2_CREATE_CLOUD(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0);
  i16 DM2_CALC_CLOUD_DAMAGE(i32 eaxl, i32 edxl);
  void DM2_PROCESS_CLOUD(c_tim* eaxtimp);

#endif
