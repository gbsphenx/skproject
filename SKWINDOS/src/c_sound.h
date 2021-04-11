#ifndef SK_SOUND_H
  #define SK_SOUND_H

  #ifndef TYPES_H
    #include "../types.h"
  #endif

  #ifndef C_SFX_H
    #include "c_sfx.h"
  #endif

  #ifndef C_MIDI_H
    #include "c_midi.h"
  #endif

  struct s_pww
  {
    xxx* p0;
    x16 w4;
    x16 w6;
  };

  struct s_ssound
  {
    x16 w_00;
    x8 b_02;
    x8 b_03;
    x8 b_04;
    x16 w_05;
  };

  class c_sound : public c_sfx,
                 public c_midi
  {
    public:
      s_size10* sndptr1;
      s_size7* xsndptr2; // there's a struct s_ssound at sndptr2 + 7 * x
      s_sfx* xsndptr3;
      xxx* sndptr4; // there's a struct pww at sndptr4 + 16 * x
      s_sfx* xsndptr5;
      xxx* sndptr6;
      ui16 v1d2698;
      ui16 v1d269a;
      ui16 v1e0ace;
      x32 v1d269c;

    public:
      c_sound(void);
      bool init(void);
      void stop_all_sound(void);
      void dtor(void);
  };

  extern c_sound dm2sound;

  void R_B37(void);
  void R_BA7(x16 eaxw);

  void SKW_482b_060e(void);
  void SKW_482b_07c2(void);
  void SKW_482b_0004(void);
  ui16 SKW_482b_015c(x16 eaxw);
  void SKW_PROCESS_SOUND(x16 eaxw);
  void SKW_482b_05bf(bool eaxbool);

#endif
