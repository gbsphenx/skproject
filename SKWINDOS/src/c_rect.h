#ifndef C_RECT_H
  #define C_RECT_H

  #ifndef TYPES_H
    #include "../types.h"
  #endif

  class c_rect
  {
    public:
	    i16 x;
	    i16 y;
	    i16 w;
	    i16 h;

    public:
      void init(void);
      c_rect(void) { init(); }
  };

  class c_c5rects; // TODO forward
  struct s_rnode; // TODO forward

  void SKW_SET_SRECT(c_rect* r, x16 x, x16 y, x16 w, x16 h);
  void SKW_SET_ORIGIN_RECT(c_rect* r, x16 w, x16 h);
  c_rect* SKW_ALLOC_TEMP_RECT(x16 x, x16 y, x16 w, x16 h);
  c_rect* SKW_ALLOC_TEMP_ORIGIN_RECT(x16 w, x16 h);
  void SKW_INFLATE_RECT(c_rect* r, x16 w, x16 h);
  c_rect* SKW_UNION_RECT(c_rect* rd, x16* tx, x16* ty, c_rect* rs);
  void SKW_CALC_CENTERED_RECT_IN_RECT(c_rect* rd, x16 w, x16 h, c_rect* rs);
  bool SKW_PT_IN_RECT(c_rect* r, x16 x, x16 y);
  bool SKW_PT_IN_EXPANDED_RECT(x16 n, x16 x, x16 y);
  x16 SKW_098d_04c7(x16 a, x16 b, x16* cp, x16 d, x16* ep);
  c_rect* SKW_QUERY_BLIT_RECT(t_gfxdata* gfxdataarr, x16 query1, x16* xaddptr, c_rect* blitrect, x16* yaddptr, x16 query2);
  void SKW_TRIM_BLIT_RECT(x16 x, x16 y, x16 w, x16 h);
  x16 SKW_ROTATE_5x5_POS(x16 a, t_rotate rotate);
  x16 SKW_QUERY_CREATURE_BLIT_RECTI(x16 a, t_rotate rotate, x16 b);
  x16 SKW_098d_0cd7(x16 a, bool b, x16 c);
  void SKW_QUERY_TOPLEFT_OF_RECT(x16 n, x16* dx, x16* dy);
  c_rect* SKW_SCALE_RECT(x16 query, x16 scalew, x16 scaleh, c_rect* rs);
  c_rect* SKW_QUERY_EXPANDED_RECT(x16 n, c_rect* r);
  void SKW_COMPRESS_RECTS(x8* eaxpb, s_rnode** ebxp, FPTR_XP_Lb ecxf);
  c_rect* SKW_OFFSET_RECT(c_c5rects* rc, c_rect* rd, c_rect* rs);
  void SKW_0b36_0d67(c_c5rects* eaxps, c_rect* edxr);

#endif
