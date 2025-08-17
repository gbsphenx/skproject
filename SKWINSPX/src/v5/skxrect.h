#ifndef C_XRECT_H
  #define C_XRECT_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  #ifndef C_RECT_H
    #include <skrect.h>
  #endif

  #ifndef C_GFX_BMP_H
    #include <gfxbmp.h>
  #endif

  class c_buttongroup; // TODO forward

  #define NUM_QUERYRECTS  4

  struct s_rnode
  {
    s_rnode* next; // @00
    i16 min; // @04
    i16 max; // @06
    ui8 mask;  // @08
    i8 b_x;  // @09
    // continues different
  };

  class c_xrectdat
  {
    public:
     i16 queryrectsindex;
     c_rect queryrects[NUM_QUERYRECTS];
     s_rnode* rnodep_rectanglelist;

     void init(void);
  };

  extern c_xrectdat xrectdat;

  bool DM2_PT_IN_EXPANDED_RECT(i16 query, i16 x, i16 y);
  void DM2_rect_098d_04c7(i16 wa, i16 wd, i16 wb, i16* wcp, i16* wep);
  c_rect* DM2_QUERY_BLIT_RECT(t_bmp* bmp, c_rect* blitrect, i16 query1, IO i16& xout, IO i16& yout, i16 query2 = -1);
  void DM2_QUERY_TOPLEFT_OF_RECT(i16 wn, OO i16& x, OO i16& y);
  c_rect* DM2_SCALE_RECT(i16 query, i16 scalew, i16 scaleh, c_rect* r);
  c_rect* DM2_QUERY_EXPANDED_RECT(i16 query, c_rect* r);
  void DM2_COMPRESS_RECTS(void* buffer, s_rnode* firstnode);

#endif
