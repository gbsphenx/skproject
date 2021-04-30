#ifndef C_MOVE_H
  #define C_MOVE_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  i32 DM2_PERFORM_MOVE(i16 eaxw);
  void DM2_move_12b4_0d75(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl);
  i32 DM2_move_12b4_099e(i32 eaxl);
  void DM2_move_12b4_0092(void);
  void DM2_move_12b4_00af(i32 eaxl);
  void DM2_move_2fcf_0b8b(i32 eaxl, i32 edxl, i32 ebxl);
  i32 DM2_move_2fcf_0434(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1);
  i32 DM2_move_075f_0af9(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0);
  i32 DM2_move_075f_06bd(unk* xeaxp, i32 edxl);
  i32 DM2_ATTACK_DOOR(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0);
  i32 DM2_move_2c1d_028c(i32 eaxl, i32 edxl, i32 ebxl);

#endif
