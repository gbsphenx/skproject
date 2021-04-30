#ifndef C_BUTTONS_H
  #define C_BUTTONS_H

  #ifndef C_RECT_H
    #include "c_rect.h"
  #endif

  #ifndef C_DBALLOC_H
    #include "c_dballoc.h" // t_dbidx
  #endif 

  class c_button // size 0xc, was c_hexc
  {
    public:
      t_dbidx dbidx;   // @00
      c_rect r;        // @02
      i16 groupsize;   // @0a

    public:
      void init(void);
      c_button(void) { init(); }
  };

  class c_buttongroup // size 0x34, was c_c5rects
  {
    public:
      c_button button; // @00
      c_rect rects[5]; // @0c

    public:
      void init(void);
      c_buttongroup(void) { init(); }
  };

  extern c_buttongroup dm2_buttongroup1;
  extern c_buttongroup dm2_buttongroup2;

  void init_global_buttongroups(void);

  c_rect* DM2_OFFSET_RECT(c_buttongroup* buttongroup, c_rect* rd, c_rect* rs);
  void DM2_ADJUST_BUTTONGROUP_RECTS(c_buttongroup* buttongroup, c_rect* rect);
  void DM2_FILL_RECT_SUMMARY(c_buttongroup* buttongroup, c_rect* blitrect, c_pixel pixel256);

#endif
