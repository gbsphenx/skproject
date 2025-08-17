#ifndef C_RECT_H
  #define C_RECT_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  class c_rect
  {
    public:
      i16 x; // @00
      i16 y; // @02
      i16 w; // @04
      i16 h; // @06

    public:
      void init(void);
      c_rect(void) : x(0), y(0), w(0), h(0) {}

      void set(i16 nx, i16 ny, i16 nw, i16 nh);
      void set_origin(i16 nw, i16 nh);
      void inflate(i16 dw, i16 dh);
      c_rect* unify(c_rect* rs, OO i16& ox, OO i16& oy);
      void calc_centered_rect_in_rect(c_rect* rs, i16 nw, i16 nh);
      bool pt_in_rect(i16 px, i16 py) const;
  };

// #################################################################

  #define NUM_TMPRECTS  4

  class c_tmprects
  {
    public:
      i16 tmprectindex;
      c_rect tmprects[NUM_TMPRECTS];

    public:
      void init(void);
      c_rect* alloc_tmprect(i16 x, i16 y, i16 w, i16 h);
      c_rect* alloc_origin_tmprect(i16 w, i16 h);
  };

  extern c_tmprects tmprects;

// #################################################################

  class c_glblrects
  {
    public:
      c_rect dm2rect1; // was arect
      c_rect dm2rect2; // was vx1d2712
      c_rect dm2rect3; // was v1e0b9c
      c_rect dm2rect4; // was v1e0470
      c_rect dm2rect5; // was v1d2658

    public:
      void init(void);
  };

  extern c_glblrects glblrects;

#endif
