#ifndef C_GUI_VP_H
  #define C_GUI_VP_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  class c_image;
  struct s_8wordarray;

  bool DM2_guivp_32cb_00f1(i32 eaxl, i32 edxl, i32 ebxl);
  void DM2_DRAW_ITEM(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, unk* xargp1, i32 argl2, i16 argw3, i32 argl4);
  void DM2_SUMMARIZE_STONE_ROOM(s_8wordarray* eaxp_s15, i32 edxl, i32 ebxl, i32 ecxl);
  i32 DM2_guivp_32cb_15b8(i32 eaxl, i32 edxl, i32 ebxl);
  void DM2_DISPLAY_VIEWPORT(i32 eaxl, i32 edxl, i32 ebxl);

#endif
