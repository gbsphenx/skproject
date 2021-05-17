#ifndef C_SOUND_H
  #define C_SOUND_H

  #ifndef XTYPES_H
    #include "xtypes.h"
  #endif

  #ifndef C_MIDI_H
    #include "c_midi.h"
  #endif

  #ifndef C_SFX_H
    #include "c_sfx.h"
  #endif

  class c_lw
  {
    public:
      i32 l_00;
      i16 w_04;

    public:
      void init(void);
      c_lw(void) { init(); }
  };

  struct s_bw // size: 0x3
  {
    ui8 ub_00;
    i16 w_01;
  };

  struct s_ssound // size: 0x7
  {
    i16 w_00;
    i8 b_02;
    i8 b_03;
    i8 b_04;
    i16 w_05;
  };

  struct s_sfx // size: 0xc
  {
    s_sizee* s54p_00;
    ui8 ub_04;
    ui8 ub_05;
    ui8 ub_06;
    ui8 ub_07;
    s_bw s59_08;
    ui8 ub_0b;
    i8 filler; // alignment, TODO: necessary?
  };

  class c_sound : public c_sfx,
                  public c_midi
  {
    public:
      s_sizee* sndptr1;
      s_ssound* xsndptr2; // there's a struct s_ssound at xsndptr2 + 7 * x
      s_sfx* xsndptr3;
      unk* sndptr4; // there's a struct pww at sndptr4 + 16 * x
      s_sfx* xsndptr5;
      unk* sndptr6;
      ui16 v1d2698;
      ui16 v1d269a;
      ui16 v1e0ace;
      i32 v1d269c;
      i8 v1dd1d1;
      i32 v1da334[8];
      c_lw v1da374[8];
      unk* v1d14c2;
      bool v1d14da;
      bool v1d14cc;
      bool v1d14d4;
      bool v1d14d0;
      i16 v1dff86;
      i32 v1dd1d2[8];
      i32 v1dff2c;
      i16 v1d14ca;
      i16 v1d14d8;
      bool v1d14be;
      i16 v1dff88;
      i8 v1dff8a;	// music hmp number, i8 means no more than 127 musics index
      i32 v1dfda4[64];
      i32 v1d1410;
      i16 v1d2694;
      i16 v1d2696;
      i32 v1d140c;
      i16 v1d1512;
      unk* v1d13f8;
      bool v1d143c;
      i32 v1d1438;
      i32 v1d1420;
      i32 v1d1400;

    public:
      bool init(void);
      c_sound(void);
      void stop_all_sound(void);
      void dtor(void);
  };

  extern c_sound dm2sound;

  void DM2_PLAY_SOUND(i16 eaxw, s_sfx* edxp_s60);
  void DM2_PROCESS_SOUND(i16 eaxw);

  void DM2_SOUND1(void);
  void DM2_SOUND2(i16 n);
  void DM2_SOUND3(i16 eaxw, i16 edxw);
  void DM2_SOUND4(void);
  void DM2_SOUND5(void);
  void DM2_SOUND6(void);
  ui16 DM2_SOUND7(i16 n);
  void DM2_SOUND8(bool flag);
  void DM2_SOUND9(i8 eaxb, i8 edxb, i8 ebxb);

  i16 DM2_QUERY_SND_ENTRY_INDEX(i8 eaxb, i8 edxb, i8 ebxb);

#endif
