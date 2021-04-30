#ifndef C_KEYBD_H
  #define C_KEYBD_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  #define KBD_QUEUE_LENGTH (10)

  class c_keybd
  {
    public:
      // all variables are received from the driver, had prefix DRVW_
      i16 queueidx_in;
      i16 queueidx_out;
      i16 queuecnt;
      i16 kbdqueue[KBD_QUEUE_LENGTH];

      void init(void);
      void putkey(i16 keycode);
      i16 getkey(void);
  };

  extern c_keybd dm2keybd;

  i16 DM2_GETKEY_TRANSLATED(void);
  bool DM2_HAS_KEY(void);
  i16 DM2_keybd_476d_05b6(i16 wn);

#endif
