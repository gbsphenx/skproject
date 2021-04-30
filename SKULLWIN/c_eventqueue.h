#ifndef C_EVENTQUEUE_H
  #define C_EVENTQUEUE_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  #define EVENTQUEUELEN 11 // was NUM_MOUSEBUFENTRIES

  class c_eventqueue // was c_mouseentrybuf
  {
    public:
      i16 idx;
      i16 out_idx; // was mouse_unk01
      i16 entries;
      bool fetch_busy;
      c_evententry data[EVENTQUEUELEN];
      bool singleevent_available;
      c_evententry singleevent;

      i16 event_unk02;
      i16 event_unk03;
      i16 event_unk04;
      i16 event_unk05; // a rectangle-index
      i16 event_unk06;
      i16 event_unk07; // an y-value
      i16 event_unk08; // a x-value
      i16 event_unk09;
      i16 event_unk0a;
      // event_unk0b is static
      // 0c/0d are ORIG_SWIDTH and ORIG_SHEIGHT (const)
      e_hero event_heroidx; // was event_unk0e
      bool event_unk0f;

      void init(void);
      void set(i16 i, i16 x, i16 y, i16 b);
      void QUEUE_EVENT(i16 mx, i16 my, i16 mb);
      void QUEUE_0x20(i16 edxw);
      void event_1031_098e(void);
      void PROCESS_SINGLEEVENT(void);
  };

  extern c_eventqueue eventqueue;

#endif
