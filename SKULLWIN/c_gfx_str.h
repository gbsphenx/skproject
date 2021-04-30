#ifndef C_GFX_STR_H
  #define C_GFX_STR_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  #ifndef C_GFX_PIXEL_H
    #include "c_gfx_pixel.h"
  #endif

#ifdef NOTIFY
  extern t_text notify[320/8+1]; // TODO temporary
#endif

  class c_stringdata
  {
    public:
      c_pixel font[24];
      i16 strx;
      i16 stry;
      i16 strxplus;
      t_text* strptr;
      i16 gfxstrw1;
      i16 gfxstrw2;
      i16 gfxstrw3;
      i16 gfxstrw4;

      void init(void);
  };

  extern c_stringdata strdat;

  class c_buttongroup;

  void DM2_DRAW_STRING(c_pixel* destgfx, i16 edxw, i16 ebxw, i16 ecxw, i16 argw0, i16 argw1, t_text* argpt2);
  bool DM2_QUERY_STR_METRICS(t_text* text, i16* edxpw, i16* ebxpw);
  void DM2_DRAW_STRONG_TEXT(c_pixel* destgfx, i16 edxw, i16 ebxw, i16 ecxw, i16 argw0, i16 argw1, t_text* argpt2);
  void DM2_DRAW_BUTTON_STR(c_buttongroup* buttongroup, i16 edxw, i16 ebxw, i16 ecxw, t_text* argpt0);
  void DM2_DRAW_NAME_STR(c_buttongroup* buttongroup, i16 edxw, i16 ebxw, i16 ecxw, t_text* argpt0);
  void DM2_DRAW_VP_STR(i16 eaxw, i16 edxw, i16 ebxw, t_text* ecxtp);
  void DM2_DRAW_GUIDED_STR(t_text* eaxtp);
  void DM2_PRINT_SYSERR_TEXT(i16 eaxw, i16 edxw, i16 ebxw, i16 ecxw, t_text* argpt0);
  void DM2_DRAW_VP_RC_STR(i16 eaxw, i16 edxw, t_text* ebxtp);
  void DM2_DRAW_LOCAL_TEXT(i32 eaxl, i32 edxl, i16 ebxw, t_text* ecxtp);

  t_text* DM2_FORMAT_SKSTR(t_text* eaxtp, t_text* edxtp);
  t_text* DM2_QUERY_GDAT_TEXT(i8 eaxb, i8 edxb, i8 ebxb, t_text* ecxtp);
  void DM2_DRAW_TEXT_TO_BACKBUFF(i32 eaxl, i32 edxl, t_text* ebxtp);

  i32 DM2_gfxstr_3929_04e2(t_text* eaxtp, t_text* edxtp, i16* ebxpw, i32 ecxl);
  void DM2_gfxstr_24a5_0732(i32 eaxl, i32 edxl, t_text* ebxtp);
  void DM2_DISPLAY_HINT_TEXT(i32 eaxl, t_text* edxtp);

  void DM2_SCROLLBOX_MESSAGE(void);

#endif
