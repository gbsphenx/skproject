#ifndef C_TIMER_H
  #define C_TIMER_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  class c_tim // s_sk5c20, size: 0xc KEEP THAT!
  {
    protected:
      union
      {
        struct
        {
          i16 w_00; // @00
          i8 dummy2; // @02
          i8 b_03; // @03
        };
        i32 l_00; // @0 - DM2_QUEUE_TIMER just grabs a word here!
      };

      ui8 ttype; // @4 timertype
      ui8 actor; // @5

      union
      {
        struct
        {
          i8 bvalueAlo;  // @6
          i8 bvalueAhi;  // @7
        };
        i16 wvalueA; // @6
      };

    public:
    // TODO has to stay public as long as wvalueB can be a t_record.
    // The adress may be used by DM2_READ_RECORD_CHECKCODE and
    // subsequent being casted to a record-poinzer in DM2_APPEND_RECORD
    // Doesn't help to make them friends. All other accesses framed!
      union
      {
        struct
        {
          i8 bvalueBlo;  // @8
          i8 bvalueBhi;  // @9
        };
        i16 wvalueB; // @8 - critical, can be a t_record!
      };

      i16 dummya; // @a
      // DM2_SUPPRESS_READER and DM2_SUPPRESS_WRITER in fact work with
      // a size of 0xc to read and write a savegame.
      // This would make wvalueB a long sometimes!
      // In fact, this length is just needed for temporary leftshifts etc.
      // and is never really used! We can put a dummy here and modify
      // the long read/write-functions.

    public:
      void init(void);
      c_tim(void) { init(); }

      void setdataw(i16 n) { w_00 = n; } // that is an index then, can be -1
      i16 getdataw(void) const { return w_00; } // that is an index then, can be -1
      i16 getmap(void) const { return unsignedword(b_03); }
      i32 getticks(void) const { return l_00 & 0xffffff; }
      void setmticks(i16 m, i32 t) { l_00 = (signedlong(m) << 24) | t; }
      void incdata(void) { l_00++; }
      void adddata(i32 n) { l_00 += n; }

      void settype(ui8 t) { ttype = t; }
      void clrtype(void)  { ttype = 0; }
      bool is_type(ui8 t) const { return ttype == t; }
      bool is_notype(void) const { return ttype == 0; }
      ui8 gettype(void) const { return ttype; }

      void setactor(ui8 n) { actor = n; }
      ui8 getactor(void) const { return actor; }
      void and_actor(ui8 n) { actor &= n; }

      i8 getxA(void) const { return bvalueAlo; }
      i8 getyA(void) const { return bvalueAhi; }
      void setxyA(i8 lo, i8 hi) { bvalueAlo = lo; bvalueAhi = hi; }

      void setA(i16 n) { wvalueA = n; }
      i16 getA(void) const { return wvalueA; }
      void mask_wvalueA(i16 m1, i16 m2) { wvalueA &= m1; wvalueA |= m2; }

      void setxB(i8 x) { bvalueBlo = x; }
      void setyB(i8 y) { bvalueBhi = y; }
      void setxyB(i8 x, i8 y) { bvalueBlo = x; bvalueBhi = y; }
      i8 getxB(void) const { return bvalueBlo; }
      i8 getyB(void) const { return bvalueBhi; }

      void setB(i16 n) { wvalueB = n; }
      i16 getB(void) const { return wvalueB; }
      void mask_wvalueB(i16 m1, i16 m2) { wvalueB &= m1; wvalueB |= m2; }

      void setBlong(i32 n) { wvalueB = CUTX16(n); }
      i32 getBlong(void) const { return unsignedlong(wvalueB); }

    friend bool DM2_READ_RECORD_CHECKCODE(i32 eaxl, i16* ebxpw, i32 ecxl, i32 edxl, i32 argl0);
    // ebxpw points to wvalueB in one call - doesn't help to make it friend
  };

  class c_timerdata
  {
    public:
      c_tim* timerarray; // was savegametimarrp
      i16* timer_indices; // was v1e424
      i16 num_timer_indices; // was savegamew9
      i16 num_timers; // was savegamew5
      i16 available_timeridx; // was v1e142a
      i16 max_timers; // was v1e142c
      i16 timer_unk; // was v1e1428  TODO: reveal
      i32 gametick; // was mallocx

      void init(void);
  };

  extern c_timerdata timdat;

  void DM2_SORT_TIMERS(void);
  void DM2_REARRANGE_TIMERLIST(void);
  i16 DM2_GET_TIMER_NEW_INDEX(i16 eaxw);
  void DM2_DELETE_TIMER(i16 eaxw);
  i16 DM2_QUEUE_TIMER(c_tim* eaxtimp);
  void DM2_GET_AND_DELETE_NEXT_TIMER(c_tim* eaxtimp);
  bool DM2_IS_TIMER_TO_PROCEED(void);
  void DM2_timer_3a15_05f7(i16 eaxw);

#endif
