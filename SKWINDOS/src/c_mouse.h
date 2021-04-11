#ifndef C_MOUSE_H
  #define C_MOUSE_H

  x16 SKW_476d_05a9(void); // calls user interrupt
  x16 SKW_476d_050e(void); // calls user interrupt
  void SKW_1031_098e(void);
  void SKW_GETSINGLEMOUSEEVENT(void);
  void SKW_443c_00a9(s_munknown* p1, x16 y1, x16 x1, x16 y0, x16 x0); // calls user interrupt
  void SKW_FIRE_QUEUE_MOUSE_EVENT(x16 mx, x16 my, x16 buttons);
  x16 queue_int2e(void); // interrupt - routine
  void SKW_FIRE_HIDE_MOUSE_CURSOR(void); // calls user interrupt
  void SKW_FIRE_SHOW_MOUSE_CURSOR(void); // calls user interrupt
  void SKW_FIRE_MOUSE_EVENT_RECEIVER(x16 eaxw, x16 ebxw, x16 ecxw, t_gfxdata* edxpg, x16 argw0, x16 argw1, x16 argw2, t_gfxdata* argpg3, x16 argw4); // calls user interrupt
  void SKW_443c_0380(void);
  void SKW_FIRE_MOUSE_SET_CAPTURE(void); // calls user interrupt
  void SKW_FIRE_MOUSE_RELEASE_CAPTURE(void); // calls user interrupt
  void SKW_443c_040e(void); // calls user interrupt

#endif
