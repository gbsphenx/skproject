#ifndef C_INPUT_H
  #define C_INPUT_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  extern const i8* v_evtable; // v1e0480

  class c_uievent // was c_wwwr
  {
    public:
      i16 x;
      i16 y;
      i16 idx;
      c_rect r;

    public:
      void init(void);
      c_uievent(void) { init(); }
  };

  class c_eventdata
  {
    public:
      c_uievent v1e04d2;
      c_uievent table1e04e0[3];
      i8* v_evtable;

    public:
      void init(void);
  };

  extern c_eventdata eventdata;

  void DM2_IBMIO_USER_INPUT_CHECK(void);
  void DM2_EVENT_LOOP(void);

#endif
