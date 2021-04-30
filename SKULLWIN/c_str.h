#ifndef C_STR_H
  #define C_STR_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  extern t_text fnum[5];

  #define DM2_STRLEN(z1) mkl(strlen(z1))
  #define DM2_STRCAT(z1, z2) strcat(z1, z2)
  #define DM2_STRCPY(z1, z2) strcpy(z1, z2)
  #define DM2_STRSTR(z1, z2) strstr(z1, z2)

  t_text DM2_SKCHR_TO_SCRIPTCHR(t_text eaxl);
  t_text* DM2_LTOA10(i32 eaxl, t_text* edxtp);
  t_text* DM2_FMT_NUM(i16 eaxw, i32 edxl, i16 ebxw);
  void DM2_FILL_STR(t_text* txtp, i8 entry, i16 step, i16 num);

#endif
