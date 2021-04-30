#ifndef C_ITEM_H
  #define C_ITEM_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  class c_tim;

  i32 DM2_RETRIEVE_ITEM_BONUS(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl);
  void DM2_PROCESS_ITEM_BONUS(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl);
  i16 DM2_ADD_ITEM_CHARGE(i32 eaxl, i32 edxl);
  i32 DM2_GET_MAX_CHARGE(i32 eaxl);
  i16 DM2_QUERY_ITEM_VALUE(i32 eaxl, i32 edxl);
  i16 DM2_QUERY_ITEM_WEIGHT(i16 eaxw);
  t_text* DM2_GET_ITEM_NAME(i16 eaxw);
  i32 DM2_IS_MISCITEM_DRINK_WATER(i32 eaxl);
  void DM2_MOVE_ITEM_TO(i32 eaxl, unk* xedxp, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1, i32 argl2, i32 argl3, i32 argl4);
  void DM2_ACTIVATE_ITEM_TELEPORT(c_tim* eaxtimp, unk* xedxp, i32 ebxl, i32 ecxl, i32 argl0, unk* xargp1, i32 argl2, i32 argl3);
  void DM2__CHECK_ROOM_FOR_CONTAINER(i32 eaxl, unk* xedxp);
  i32 DM2_PUT_OBJECT_INTO_CONTAINER(i32 eaxl);
  i16 DM2_F958(i32 eaxl);
  void DM2_SHOOT_ITEM(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i8 argb1, i8 argb2, i8 argb3);
  void DM2_TAKE_OBJECT(i32 eaxl, i32 edxl);

#endif
