#ifndef C_GFX_STR_H
  #define C_GFX_STR_H

  #ifndef TYPES_H
    #include "../types.h"
  #endif

  class c_font // one instance only
  {
    protected:
      x8* fontptr;
      t_gfxdata gfxletter[6*3];
      x8 unknown[6]; // let that stay here!

    public:
      void init(void);
      void allocate(void);
      t_gfxdata* SKW_QUERY_GFXLETTER(ui8 charidx, ui8 cmask1, ui8 cmask2);
  };

  extern c_font C_font;

  class c_string_metrics // one instance only
  {
    public: // all variables const after init()
      x16 height;
      x16 xofs;
      x16 yofs;
      x16 charwidth;

    public:
      void init(void);
      bool SKW_QUERY_STR_METRICS(const t_text* eaxt, OO x16& ebxpw, OO x16& edxpw);
  };

  extern c_string_metrics C_strmetrics;

  void SKW_DRAW_STRING(t_gfxdata* destbmp, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, const t_text* argpt2, e_bitres destbits = E_BITRES8);
  void SKW_DRAW_STRONG_TEXT(t_gfxdata* eaxpg, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, t_text* argpt2);
  void SKW_DRAW_SIMPLE_STR(c_c5rects* eaxps, x16 ebxw, x16 ecxw, x16 edxw, t_text* argpt0);
  void SKW_DRAW_NAME_STR(c_c5rects* eaxps, x16 ebxw, x16 ecxw, x16 edxw, t_text* argpt0);
  void SKW_DRAW_VP_STR(x16 eaxw, x16 ebxw, const t_text* ecxt, x16 edxw);
  void SKW_DRAW_GUIDED_STR(const t_text* eaxt);
  void SKW_PRINT_SYSERR_TEXT(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, t_text* argpt0);
  void SKW_DRAW_VP_RC_STR(x16 eaxw, const t_text* ebxt, x16 edxw);
  void SKW_DRAW_LOCAL_TEXT(x16 eaxl, x16 ebxw, t_text* ecxt, x16 edxl);

#endif
