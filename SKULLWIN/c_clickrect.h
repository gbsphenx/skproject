#ifndef C_CLICKRECT_H
  #define C_CLICKRECT_H

  #ifndef C_RECT_H
    #include "c_rect.h"
  #endif

  class c_clickrectdata; // forward

  class c_clickrectnode // size 0xa, was c_munknown
  {
    public:
      i16 w_00;
      i8 b_02;
      i8 b_03;
      i8 buttons; // @04
      i8 b_05;
      c_clickrectdata* node; // @06

    public:
      void DM2_REFRESH_CLICKRECTLINK_1(void);
      void DM2_REFRESH_CLICKRECTLINK_2(void);
      void DM2_SET_CLICKRECTDATAS(i16 x0, i16 y0, i16 x1, i16 y1);
      void DM2_ALLOC_CLICKRECTDATA(void);
  };

  extern c_clickrectnode table1d32d8[18];

  class c_clickrectdata // size 0xc, was s_between
  {
    public:
      c_clickrectnode* next;
      c_rect r; // @04
  };

  void init_clickrecttable(void);

#endif
