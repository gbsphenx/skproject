#ifndef XTYPES_H
  #define XTYPES_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  // local variables
  // Note: all pure c_q32 have been sorted out

  class c_o32 // 16-bit only now!!!
  {
    protected:
      i16 lv;

    public:
      i8 peek8(void);
      i8 getbyte1(void);

      void poke16(i16 wn);
      i16 peek16(void);
      void inc16(void);
      void dec16(void);
      void sub16(i16 wn);
      void add16(i16 wn);
      void shl16(ui8 ubn);
      void sar16(ui8 ubn);
      void and16(i16 wn);
      void or16(i16 wn);
      void xor16(i16 wn);
  };

  class c_ql32 // transferfunctions only
  {
    protected:
      i32 lv;

    public:
      i8 peek8(void);
      void poke16(i16 wn);
      i16 peek16(void);
      void poke32(i32 ln);
      i32 peek32(void);
  };

  class c_ol32 : public c_ql32 // c_ql32 with xtrafunctions
  {
    public:
      i8 getbyte1(void);
      i8 getbyte3(void);

      void shl16(ui8 ubn);
      void shr16(ui8 ubn);
      void and16(i16 wn);
      void or16(i16 wn);
      void inc16(void);
      void dec16(void);
      void add16(i16 wn);
      void sub16(i16 wn);

      void or32(i32 ln);
      void inc32(void);
      void dec32(void);
      void add32(i32 ln);
      void sub32(i32 ln);
  };

// ##################################################################

  struct s_hexe // size 0xe
  {
		i8 b_00;	// @0
		i8 b_01;	// @1
		i16 w_02;	// @2
		i16 w_04;	// @4
		i16 w_06;	// @6
		i8 b_08;	// @8 // max eye distance?
		i8 b_09;	// @9
		i8 b_0a;	// @a
		i8 b_0b;	// @b
		i8 b_0c;	// @c
		i8 b_0d;	// @d
  };

// ---------------------------------------------

  struct s_len8 // size: 0x8
  {
    i8 b_00;
    i8 b_01;
    i16 w_02;
    i8 b_04;
    i8 b_05;
    i8 b_06;
    i8 b_07;
  };

  class c_sizex16 // size: 0x16
  {
    public:
      s_len8 s23_00;
      i16 w_08;
      i8 barr_0a[0xc];

    public:
      void init(void);
      c_sizex16(void) { init(); }
  };

  struct s_sizee // size: 0xe, maybe longer, smth with sfx and gfx
  {
    union
    {
      i32 l_00; // value 1 used
      s_sizee* s54p_00;
    };
    i8 barr_04[0x6]; // only barr_04[2] and barr_04[3] used yet
    i16 w_0a;
    i16 w_0c;
  };

  struct s_bbw
  {
    i8 b_00;
    i8 b_01;
    i16 w_02;

    i32 getlong(void) const;
  };

  struct s_hexa
  {
    i8 barr_00[10];
  };

	class c_aidef // size 0x24 SPX-comments
  {
    public:
		  i16 b_00;	// @0 // flags
		  i8 b_02;  // @2	// armor/defense
		  i8 b_03;  // @3
		  i16 w_04; // @4 // initial HP	 
		  i8 b_06;	// @6	// attack strength
		  i8 b_07;	// @7	// poison
		  i8 b_08;	// @8	// defense (255 = undestroyable)
		  i8 b_09;	// @9 // 0x40: pit ghost
		  i16 w_0a;	// @a
		  i16 w_0c;	// @c
		  i16 w_0e;	// @e // Attack and Spells commands (checked at ^19F0:0BD4)
		  i16 w_10;	// @10
		  i16 w_12;	// @12 // Can switch triggers, I think
		  i16 w_14;	// @14
		  i16 w_16;	// @16
		  i16 w_18;	// @18 // Resistance & Fire resistance & Poison resistance & ??
		  i16 w_1a;	// @1a		
		  i8 b_1c;	// @1c
		  i8 b_1d;	// @1d // Weight = Push resistance (255 = can't be pushed/moved)
		  i16 w_1e;	// @1e if 0x0800 : then it can turn missiles
		  i16 w_20;	// @20
		  i8 b_22;	// @22
		  i8 b_23;	// @23 // Cloud size when dying ? 0/default = 0x6E / 1 => 0xBE / 2 => 0xFF
	};

  // #######################
  // PORT TO ALLEGRO
  // #######################

#ifndef OLD_ALLEGRO
  #include "./allegro5/include/allegro5/allegro.h"

#if 0
  extern ALLEGRO_TIMER* al5_timer;
  extern ALLEGRO_EVENT_QUEUE* al5_equeue;
  extern ALLEGRO_DISPLAY* al5_display;
#endif
#endif

#if 0 // TODO:
  #define ALLEGRO_SETVOLUME      al_set_volume
  #define ALLEGRO_VSYNC          al_vsync
  #define ALLEGRO_PAUSE          al_rest
  #define ALLEGRO_GETCONFIGINT   al_get_config_int
#endif

  #define SAMPLE_REC_FREQ        (12000)  // guessed original frequency of sfx

#endif
