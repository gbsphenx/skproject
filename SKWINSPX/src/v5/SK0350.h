#ifndef C_350_H
  #define C_350_H

  #ifndef C_TIMER_H
    #include <sktimer.h>
  #endif

  #ifndef C_BUTTONS_H
    #include <uibutton.h>
  #endif

  class c_4b3wp // size 0xe
  {
    public:
      i8 b_00;
      i8 b_01;
      i8 b_02;
      i8 b_03;
      i16 w_04;
      i16 w_06;
      i16 w_08;
      s_hexe* xp_0a;

    public:
      void init(void);
      c_4b3wp(void) { init(); }
  };

  class c_creature;
  class SPX_Creature;

  class c_350
  {
    public:
      i16 v1e054c;
      SPX_Creature* v1e054e;
      unk* v1e0552;
      c_creature* creatures; // was v1e0556
      unk* v1e055a;
      unk* v1e055e;
      c_tim v1e0562;
      i8 v1e056e;
      i8 v1e056f;
      i8 v1e0570;
      i8 v1e0571;
      i16 v1e0572;
      i16 v1e0574;
      i16 v1e0576;
      i16 v1e0578;
      i16 v1e057a;
      i16 v1e057c;
      i16 v1e057e;
      i16 v1e0580;
      i16 v1e0582;
      i16 v1e0584;
      i16 v1e0586;
      unk* v1e0588;
      i8 v1e058c;
      i8 v1e058d;
      i8 v1e058e[0x80];
      c_button v1e060e[8];
      i8 v1e066e[5]; // five bytes not adressed
      i8 v1e0673;
      i8 v1e0674;
      i8 v1e0675;
      i8 v1e0676[2]; // two bytes not adressed
      c_sizex16 v1e0678[0x10];
      c_4b3wp v1e07d8;
      i16* v1e07e6;
      i8 v1e07ea;
      i8 v1e07eb;
      i8 v1e07ec;
      i8 v1e07ed;
      unk* v1e07ee[0x2a];
      unk v1e0896;
      unk* v1e0898;

    public:
      void init(void);
      c_350(void) { init(); }
  };

  #define DM2_COPY_C350(dest, src)  DM2_COPY_MEMORY(DOWNCAST(c_350, dest), DOWNCAST(c_350, src), sizeof(c_350))

  extern c_350 s350;

#endif
