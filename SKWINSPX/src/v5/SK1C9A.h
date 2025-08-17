#ifndef C_1C9A_H
  #define C_1C9A_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  i32 DM2_19f0_0207(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, FPTR_L_LL argf0);
  i32 DM2_19f0_045a(i32 eaxl, i32 edxl);
  i32 DM2_19f0_0559(i16 eaxw);
  i32 DM2_19f0_0891(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1);
  i32 DM2_19f0_0d10(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1);
  i32 DM2_19f0_13aa(i32 eaxl, i32 edxl);
  i32 DM2_CREATURE_GO_THERE(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1);
  i32 DM2_19f0_2165(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i16 argw1, i16 argw2);
  bool DM2_19f0_2813(i32 eaxl, i32 edxl, i32 ebxl, i16 x, i16 y, i16 argw1, i16 argw2);

  i32 DM2_1BA1B(i32 eaxl, i32 edxl); // also called indirect
  void DM2_1c9a_0247(i32 eaxl);
  i32 DM2_1c9a_0648(i32 eaxl);
  i16* DM2_1c9a_06bd(i32 eaxl, i32 edxl, i32 ebxl);
  i32 DM2_1c9a_078b(unk* xeaxp, i32 edxl, i32 ebxl);
  i32 DM2_1c9a_0958(i32 eaxl);
  i32 DM2_1c9a_09b9(i32 eaxl, i32 edxl);
  void DM2_1c9a_09db(unk* xeaxp);
  i32 DM2_CREATURE_SOMETHING_1c9a_0a48(void);
  void DM2_1c9a_0cf7(i32 eaxl, i32 edxl);
  void DM2_1c9a_0db0(i32 eaxl);
  void DM2_ALLOC_CAII_TO_CREATURE(i32 eaxl, i32 edxl, i32 ebxl);
  void DM2_1c9a_0fcb(i32 eaxl);
  i16 DM2_CREATE_MINION(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i16 argw1, i32 argl2, i8 argb3);
  void DM2_RELEASE_MINION(i32 eaxl);
  i32 DM2_1c9a_17c7(i32 eaxl, i32 edxl, i32 ebxl);
  void DM2_1c9a_19d4(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl);
  i32 DM2_1c9a_1a48(i32 eaxl, i32 edxl);
  i32 DM2_1c9a_1b16(i32 eaxl, i32 edxl);
  i32 DM2_FIND_WALK_PATH(i32 eaxl, i32 edxl, i32 ebxl, i16 argw0, s_len8* argp_s23, c_button* button);
  i32 DM2_1c9a_381c(void);
  i32 DM2_1c9a_38a8(void);
  i32 DM2_FILL_CAII_CUR_MAP(void);
  void DM2_FILL_ORPHAN_CAII(void);

#endif
