#ifndef C_EVENTS_H
  #define C_EVENTS_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  void DM2_CLICK_ITEM_SLOT(i16 eaxw);
  void DM2_events_AB26(void);
  void DM2_events_38c8_0002(void);
  void DM2_events_38c8_0060(void);
  void DM2_CLICK_MAGICAL_MAP_RUNE(i16 eaxw);
  void DM2_events_13262(i16 eaxw);
  void DM2_CLICK_VWPT(i32 eaxl, i32 edxl);
  void DM2_CLICK_INVENTORY_EYE(void);
  void DM2_events_2f3f_04ea(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0);
  void DM2_events_2e62_0cfa(i32 eaxl);
  void DM2_CLICK_MONEYBOX(i16 eaxw);
  void DM2_ADD_RUNE_TO_TAIL(i16 eaxw);
  void DM2_REMOVE_RUNE_FROM_TAIL(void);
  bool DM2_TRY_CAST_SPELL(void);
  void DM2_ACTIVATE_ACTION_HAND(i16 eaxw, i16 edxw);
  bool DM2_PROCEED_COMMAND_SLOT(i16 eaxw);
  i32 DM2_events_30DEA(i32 eaxl);
  void DM2_events_5BFB(i32 eaxl, i32 edxl);
  void DM2_events_3C1E5(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0);
  void DM2_events_443c_0434(void);

#endif
