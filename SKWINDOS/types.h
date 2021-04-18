#ifndef TYPES_H
  #define TYPES_H

  #ifndef DM2BT_H
    #include "basetypes.h"
  #endif

  typedef ui8 t_gfxdata; // different data - structures possible.
  // can have a palette in the beginning.
  // can point into the large bitmap - memory - graphics - structure, so neg. offsets possible: (-2:word, -4:word, -6:byte)
  // can also point to VGABASE.

  typedef ui8 t_palette;
  // maybe other t_gfxdata are attached, but starts with a palette

  typedef char t_text;

  i32 con(const i32 x);
  ui16 ucon16(const i32 x);
  ui8 ucon8(const i32 x);
  i8 icon8(const i32 x);

  i16 signedword(x8);
  i32 signedlong(bool);
  i32 signedlong(x8);
  i32 signedlong(ui8);
  i32 signedlong(x16);
  i32 signedlong(ui16);
  i32 signedlong(t_text); // TODO
  ui16 unsignedword(x8);
  ui32 unsignedlong(x8);
  ui32 unsignedlong(ui8);
  ui32 unsignedlong(x16);
  ui32 unsignedlong(ui16);
  ui32 unsignedlong(t_text); // TODO

  // TODO: whereever a CUT-function appears, it can be assumed that the
  // variable should have the smaller type and gets unnecessary extended
  x16 CUTX16(x32);
  x8 CUTLX8(x32);
  x8 CUTX8(x16);

  #define VCAST(x)    reinterpret_cast<void*>(x)
  #define CVPCAST(x)  reinterpret_cast<const void*>(x)
  #define I8CAST(x)   ((i8)(x))
  #define UI8CAST(x)  ((ui8)(x))
  #define X8CAST(x)   ((x8)(x))
  #define I16CAST(x)  ((i16)(x))
  #define UI16CAST(x) ((ui16)(x))
  #define X16CAST(x)  ((x16)(x))
  #define I32CAST(x)  ((i32)(x))
  #define UI32CAST(x) ((ui32)(x))
  #define P8CAST(x)   ((i8*)(x))
  #define UP8CAST(x)  ((ui8*)(x))
  #define P16CAST(x)  ((i16*)(x))
  #define UP16CAST(x) ((ui16*)(x))
  #define P32CAST(x)  ((i32*)(x))
  #define STRINGCAST(x) reinterpret_cast<char*>(x)

// TODO temporary helpers:
  #define tCAST(x)    ((t_text)(x))
  typedef i16 x16;
  typedef i32 x32;

  typedef i8 xxx; // USE ONLY FOR POINTERS TO UNKNOWN TYPES!!!

  // whereever the lextended-macro is used, it can be assumed that
  // the variable should have the smaller type
  #define lextended(x) (I32CAST(x))

  #define PAL16 0x10
  #define PAL256 0x100

  #define DOWNCAST(st, x) (P8CAST(x))
  #define UPCAST(st, x) ((st*)(x))

//  ----------------

  #define INDCALL(x)  (*(x))

  typedef bool (*FPTR_b_WPWP)(x16*, x16*);
  typedef bool (*FPTR_b_W)(i16);
  typedef bool (*FPTR_b_WW)(i16, i16);
  typedef xxx* (*FPTR_XP_Lb)(x32, bool);

  typedef ui16 t_rotate; // 0-3

  #ifndef C_RECT_H
    #include "src/c_rect.h"
  #endif

  // necessary forwards from mdata.h
  struct s_wpm;
  struct s_up;

  struct s_cursor
  {
    i8 hx; // hotspot x
    i8 hy; // hotspot y
    i8 w;  // w
    i8 h;  // h
    i8 v4;
    i8 v5; // not requested - probably just for alignment
    i8 v[0x240]; // a bitmap
  }; // total size 0x246

  struct s_transferbitmap
  {
    i16 i; // TODO graphics index? or palette - index?
    i16 w; // w
    i16 h; // h
    i8 v[0x7d00]; // 32000 bytes
  };

  // === some temporary helperclasses for local byte/word/longs-variables ==

  class q16
  {
    protected:
      x16 v;

    public:
      x16 peek16(void) { return v & 0xffff; }
      void poke16(x16 n) { v = n & 0xffff; }
  };

  // =======================================================

  union u_bbbbl
  {
    struct
    {
      x8 b0;
      x8 b1;
      x8 b2;
      x8 b3;
    } b;
    x32 l;
  };

  struct s_4bytearray
  {
    x8 barr_00[0x4];
  };

  struct s_8bytearray
  {
    x8 barr_00[0x8];
  };

  struct s_size8
  {
    x16 w0;
    x16 w2;
    x8 b4;
    x8 b5;
    x16 w6;
  };

  struct s_32bytes
  {
    x8 barr_00[0x20];
  };

  struct s_16bytearray // size 0x10
  {
    x8 barr_00[0x10];
  };

  class c_5bytes
  {
    public:
      x8 b_00;
      x8 b_01;
      x8 b_02;
      x8 b_03;
      x8 b_04;

    public:
     c_5bytes(void) { init(); }
     void init(void) { b_00=b_01=b_02=b_03=b_04=0; }
  };

  class c_rwbb
  {
    public:
      c_rect r0;
      x16 w8;
      x8 ba;
      x8 bb;

    public:
      void init(void);
      c_rwbb(void) { init(); }
  };

  struct s_3words // size 0x8
  {
    x16 w_00;
    x16 w_02;
    x16 w_04;
  };

  struct s_8wordarray
  {
    x16 warr_00[0x8];
  };

  class c_hexc
  {
    public:
      x16 w_00;
      c_rect rc_02;
      x16 w_0a;

    public:
      void init(void);
      c_hexc(void) { init(); }
  };

  class c_len10x
  {
    public:
      c_hexc s_00;
      x32 l_0c;

    public:
      void init(void);
      c_len10x(void) { init(); }
  };

  struct s_hex6	// SPX: different from SkLoadEnt ?
  {
    union
    {
      struct
      {
        x16 w_00;
        x8 b_02;
        x8 b_03;
      } s_00;
      x32 l_00;	// SkEnt4 ?
    } u;
    x8 b_04;	// X16 w0_ ?
    x8 b_05;

    void setlatelong(x32 l)
    {
      u.s_00.b_02 = l;
      u.s_00.b_03 = l >> 8;
      b_04 = l >> 16;
      b_05 = l >> 24;
    }
  };

  struct s_lllw
  {
    x32 dummy;
    x32 vl_04; // set/get always a word!
    x32 vl_08; // set/get always a word!
    x16 vw_0c;
  };

  union u_len10
  {
    s_lllw s_00;
    x8 barr_00[0x10];
  };

  class c_c5rects // size 0x34
  {
    public:
      c_hexc s_00;
      c_rect rcarr_0c[5];

    public:
      void init(void);
      c_c5rects(void) { init(); }
  };

  struct s_len8
  {
    x8 b_00;
    x8 b_01;
    u_bbw u_02;
    u_bbw u_04;
    x8 b_06;
    x8 b_07;
  };

  union u_mbuf
  {
    x8 barr_00[0x8];
    x16 w_00; // grabs the first word for a check TODO clean that, split the const
  };

  union u_spec
  {
    s_hex6* ps_00;
    x32 l_00;
  };

  class c_hex28
  {
    public:
      t_text t_00[0x24]; // @3e
      x32 l_24; // @62

    public:
      c_hex28(void) { init(); }
      void init(void);
  };

  class c_hex2a
  {
    public:
      x16 w_00; // @3c
      c_hex28 s_02;

    public:
      c_hex2a(void) { init(); }
      void init(void) { w_00 = 0; s_02.init(); }
  };

  struct s_whex2a
  {
    x16 w_00;
    t_text t_02[0x1e];
    x16 w_20[0x5];
  };

  struct s_hex30
  {
    c_hex2a s_00;
    x16 w_2a; // @64 not confirmed yet
    union
    {
      t_text* tp_2c; // @68 save
      x32 l_2c; // @68 load
    } u;
  };

  class c_hex18 // length 0x18
  {
    public:
      t_gfxdata* pg_00; // used
      x16 w_04; // used
      x16 w_06; // used
      x8 b_08; // used
      x8 b_09; // used
      x8 b_0a;
      x8 b_0b; // used
      x16 w_0c; // used
      x16 w_0e; // used
      x16 w_10; // used
      x16 w_12; // used
      x16 w_14; // used
      x16 w_16; // used

    public:
      void init(void);
      c_hex18(void) { init(); }
  };

  struct s_8ubbw
  {
    u_bbw s_00[0x8];
  };

  union u_bbwlong
  {
    struct
    {
      x8 b_00;		// cls2
      x8 b_01;		// cls4
      x16 w_02;		// data
    } u;
    x32 l_00;
  };
  // SPX: u_bbwlong matches RawEntry
/*
	struct RawEntry {
		Bit8u cls2;
		Bit8u cls4;
		Bit16u data;
	};  
  */

  struct s_tww
  {
    t_text* tp_00;
    x16 w_04;
    x16 w_06;
  };

  struct s_hex3c
  {
    x32 l_00;
    ui32 l_04;
    x16 w_08;
    x16 w_0a;
    x16 w_0c;
    x16 w_0e;
    x16 w_10;
    x16 w_12;
    x16 w_14;
    x32 l_16;
    x32 l_1a;
    x16 w_1e;
    x16 w_20;
    x16 w_22;
    x32 l_24; // not confirmed yet
    x16 w_28;
    x32 l_2a;
    x8 b_2e;
    x8 b_2f;
    x8 b_30;
    x8 b_31;
    x8 b_32;
    x8 b_33;
    x16 w_34;
    x8 b_36;
    x8 b_37;
    x32 l_38;
  };

  struct s_1stname // size 0x8 - hero first name
  {
    t_text noname[0x8];
  };

  struct s_2ndname // size 0x14 - hero second name
  {
    t_text noname[0x14];
  };

  struct s_gdat // size 0x20
  {
    x32 l_00; // used
    s_hex6 s_04;	// SkLoadEnt
    s_hex6 s_0a;	// SkLoadEnt
    u_bbwlong* p_10;	// RawEntry
    x8 b_14; // used
    x8 b_15;
    x8 b_16; // used
    x8 b_17; // used
    x16 w_18; // used
    x16 w_1a; // used
    x16 w_1c; // used
    x16 w_1e;
  };
// SPX: s_gdat should match skwin SkEntIter
/*
	struct SkEntIter {	// ? bytes
		X16 w0;		// @0 // enum init flag: 1=first num(need init), 0=second or later enum, 0x8000=ranged
		SkLoadEnt x2;	// @2 bp5a // single, or from
		SkLoadEnt x8;	// @8 bp54 // to
		RawEntry *pv14;	// @14 bp4e // found raw ent
		U8 b18_;	// @18 // cls1 current
		U8 b19_;	// @19 // cls1 base
		U8 b20_;	// @20 // cls3(type) current
		U8 b21_;	// @21 // cls3 base
		X16 w22;	// @22 // cls2 table index
		X16 w24;	// @24 // found raw entry index (pv14)
		U16 w26;	// @26 // cls3 table index
  */


  union u_lpg
  {
    x32 l_00;
    t_gfxdata* pg_00;
  };

  class c_image // 0x13a (old:0x144) bytes - graphic operations
  {
    public:
      c_hex18 s_00; // b00[0x4] used
      x16 w_18;
      x16 w_1a;
      x16 w_1c;
      x16 w_1e;
      x16 w_20;
      x16 w_22;
      c_rect rc_24;
      t_gfxdata* pg_2c;
      x16 w_30;
      x16 w_32;
      x16 w_34;
      x16 w_36;
      x16 w_38;
      t_gfxdata carr_3a[PAL256];

    public:
      void init(void);
      c_image(void) { init(); }
  };

  // track TODO: kentaro's struct has a dword in the beginning,
  // but SKW_QUEUE_TIMER requests the first word!
  // Then: the last word is added here, because SKW_QUEUE_TIMER
  // uses three times movsd to copy the structure - size 0xc

  class c_tim // s_sk5c20 // pointer parameter for SKW_QUEUE_TIMER
  {
    public:
      union
      {
        struct
        {
  		    x8 b_00;	// @0 unused
	  	    x8 b_01;	// @1 unused
	  	    x8 b_02;	// @2 unused
	  	    x8 b_03;	// @3 used !!
        } b;
        x16 w_00; // 2nd word unused then
        x32 l_00; // @0 used - SKW_QUEUE_TIMER just grabs a word here!
      } un;
		  x8  b_04; // @4 used
		  x8  b_05;	// @5 used
      union
      {
        struct
        {
  		    x8 b_06;	// @6 used
	  	    x8 b_07;	// @7 used
        } b;
        x16 w_06; // @6 used
      } u;
      union
      {
        struct
        {
  		    x8 b_08;	// @8 used
	  	    x8 b_09;	// @9 used
        } b;
        x16 w_08; // @8 used
        x32 l_08; // @8  that one 2 bytes more (SKW_ACTIVATE_ORNATE_ANIMATOR)
      } v;

    public:
      void init(void);
      c_tim(void) { init(); }
  };

  class c_mousedata
  {
    public:
      i16 x;
      i16 y;
      i16 b;

    public:
      c_mousedata(void) { init(); }
      void init(void) { x=y=b=0; }
  };

  class c_mousebufentry // my god... there are more combinations possible...
  {
    public:
      i16 b;
      i16 x;
      i16 y;

    public:
       c_mousebufentry(void) { init(); }
       void init(void) { b=x=y=0; }
  };

  struct s_rnode
  {
    s_rnode* next;
    x16 lo; // 4
    x16 hi; // 6
    x8 mask;  // 8
    x8 x;  // 9
    // continues different
  };

  // this structure is the end of a structure of variable
  // type and length, that pointer is of the type t_gfxdata
  // two t_gfxdata-structs are concatenated in memory and
  // this structure is used to get access to the first
  // structure if you got the pointer of the second.
  // use s_gfxdatatail* getprevious(t_gfxdata*) to access.
  struct s_gfxdatatail
  {
    x8 b0;
    x8 b1;
    x16 width;	// width	(w2)
    x16 height;	// height	(w4)
  };

  s_gfxdatatail* getprevious(t_gfxdata*);

  union u_vptr
  {
    x8* p8;
    x16* p16;
  };

  struct s_between; // forward

  struct s_munknown
  {
    x16 w0;
    x8 b2;
    x8 b3;
    x8 b4;
    x8 b5;
    union
    {
      s_between* pb6;
      s_munknown** pm6;
    } u;
  };

  struct s_between
  {
    s_munknown* p0;
    c_rect r4;
  };

  struct s_pppw
  {
    x16* pw0;
    x16* pw4;
    x16* pw8;
    x16 wc;
  };

  struct s_bbw
  {
    x8 b0;
    x8 b1;
    x16 w2;
  };

  class c_wbbb
  {
    public:
      x16 w0;
      x8 b2;
      x8 b3;
      x8 b4;
      x8 b5; // unused fillbyte - needed when struct used with DM2_ZERO_MEMORY, SUPPRESS_READER/WRITER

    public:
      c_wbbb(void) { init(); }
      void init(void) { w0=0; b2=b3=b4=b5=0; }
  };

  class c_bbbbwwwp // size 0xe
  {
    public:
      x8 b0;
      x8 b1;
      x8 b2;
      x8 b3;
      x16 w4;
      x16 w6;
      x16 w8;
      struct s_hexe* phe_a;

    public:
      void init(void);
      c_bbbbwwwp(void) { init(); }
  };

  class c_sizex16 // 22 bytes
  {
    public:
      s_len8 s_00;
      u_bbw u_08;
      u_bbw u_0a;
      x16 w_0c;
      x8 b_0e[0x4];
      s_hexe* ps_12;

    public:
      void init(void);
      c_sizex16(void) { init(); }
  };

  class c_hero // 0x107 <=> 263
  {
    public:
      t_text t00[0x1c]; // splitted at 0x8
      x8 b1c;
      x8 b1d;
      x8 b1e;
      x8 b1f;
      x8 b20[0x2];
      t_text t22[0x8]; // TODO: last two separate?
      x8 b2a[0x4];
      x16 w2e;
      x16 w30;
      union
      {
        struct
        {
          x8 b0; // not used
          x8 b1;
        } b;
        x16 w;
      } u32;
      x16 w34;
      x16 w36;
      x16 w38;
      x16 w3a;
      union
      {
        struct
        {
          x8 b0; // not used
          x8 b1;
        } b;
        x16 w;
      } u3c;
      x16 w3e;
      x16 w40;
      x8 b42[0x2];
      x16 w44;
      x16 w46;
      x16 w48;
      x8 b4a[0xe];
      x8 b58[0x7];
      x32 l5f[0x14];
      x8 baf[0x14];
      x16 warrc3[0x1e]; // direct used: 0xc5 = warrc3[0x1], 0xdb = warrc3[0xc]
      x16 wff;
      x8 b101;
      x8 b102;
      x16 w103;
      x8 b105[0x2];

    public:
      void init(void);
      c_hero(void) { init(); }
  };

  struct s_sgwords // total length 0x2c
  {
    x16 w0[0x16];
    // types of calling:
    // w[1],w[2] this one byte only,w[3],[5],[0xa],[0xf] direct
    // from [6] as array
    // but also indexed from [0]
  };

  union u_bl
  {
    x32 l0;
    x8 b0; // three upper bytes not used then
  };

  class c_wpc // itemrecord
  {
    public:
      x16 w0;
      t_gfxdata* p2;
      t_gfxdata barr6[PAL16];
      x8 b16;
      x8 b17; // unused fillbyte?
      x16 w18; // written, but never read
      x16 w1a;

    public:
      c_wpc(void) { init(); }
      void init(void);
  };

  struct s_d556 // (was also s_size22)
  {
    x16 w00;
    x16 w02;
    x8 barr04[0x3];
    x8 b07;
    x16 w08;
    x8 barr0a[0x2];
    u_bbw u0c;
    union
    {
      struct
      {
        x16 w0e;
        x16 w10;
        x8 b12;
        x8 b13;
        x16 w14;
        x8 barr16[0x2];
        u_bbw u18;
        x8 b1a;
        x8 b1b;
        x8 b1c;
        x8 b1d;
        x8 b1e;
        x8 b1f;
        x8 b20;
        x8 b21;
      } v;
      struct // one time used:
      {
        x16 w0e[0xa];
      } arr;
    } u;
  };

  struct s_d89c // (also size 0x22)
  {
    x16 w00;
    x16 warr02[0x10];
  };

  struct s_size10 // maybe longer, smth with sfx
  {
    union
    {
      s_size10* p;
      u_bbbbl u; // byte2 and byte3 used
    } u0;
    x8 b4[0x4]; // only b4[2] and b4[3] used yet
    x16 w8;
    x16 wa;
    x16 wc;
    x16 we;
  };

  struct s_size7 // maybe longer, smth with sfx
  {
    x16 w0; // not used yet
    x8 b2;
    x8 b3;
    x8 b4;
    x16 w5;
  };

  class c_lw // only for datastructure v1da374
  {
    public:
      x32 l0;
      x16 w4;

    public:
      c_lw(void) { init(); }
      void init(void) { l0=0; w4=0; }
  };

  struct s_bwb // TODO: odd behaviour of s_pwwp
  {
    x8 b0;
    x16 w1;
    x8 b3;
  };

  struct s_pwwp
  {
    xxx* p0;
    x16 w4;
    x16 w6;
    union {
      s_pwwp* p8;
      s_bwb s8;
    } u;
  };

  struct s_xabytes // TODO: there are words in, compare to structs with same size
  {
    x8 b0;
    x8 b1;
    x16 w2;
    x16 w4;
    x16 w6;
    x8 b8;
    x8 b9;
  };

  struct s_bw // size 0x3 only member of s_sfx
  {
    ui8 b0;
    x16 w1;
  };

  struct s_sfx // size 0xc
  {
    s_size10* p0;
    ui8 b4;
    ui8 b5;
    ui8 b6;
    ui8 b7;
    s_bw s8;
    ui8 bb;
  };

  struct s_bpp
  {
    x8 b0;
    x8* pb1;
    x8* pb5;
  };

  struct c_aispec // for table1d296c, size 0x24
  {
    u_bbw u0;
    x8 b2;
    x8 b3;
    x16 w4;
    x8 barr06[0x4];
    x16 wa;
    x16 wc;
    x16 we;
    x16 w10;
    x16 w12;
    x16 w14; // not confirmed
    x16 w16;
    u_bbw u18;
    x16 w1a;
    u_bbw u1c;
    u_bbw u1e;
    x16 w20;
    x8 barr22[0x2]; // unused?
  };

  struct c_record
  {
    x16 w0;
    u_bbw u2;
    u_bbw u4;
    u_bbw u6;
    x16 w8;
    u_bbw ua;
    u_bbw uc;
    u_bbw ue;
  };

  struct s_imghex12
  {
    x8 b_00[0xa]; // unused yet
    x16 w_0a;
    x8 b_0c;
    x8 b_0d;
    x16 w_0e;
    x16 w_10;
  };

  struct s_hex12
  {
    x8 b_00;
    x8 b_01;
    s_8wordarray s_02;
  };

  struct s_hex20 // when pointers used on this, there's usually just sarr_04[0] used
  {
    s_hexe* ps_00;
    s_4bytearray sarr_04[0x7];
  };

  struct s_hex11bytes // size dec 17
  {
    x8 barr_00[0x11];
  };

  class c_350
  {
    public:
      x16 v1e054c;
      c_record* v1e054e;
      c_aispec* v1e0552;
      s_d556* v1e0556;
      t_gfxdata* v1e055a; // pointer to a x8[4]
      x16* v1e055e;
      c_tim v1e0562;
      x8 v1e056e;
      x8 v1e056f;
      x8 v1e0570;
      x8 v1e0571;
      x16 v1e0572;
      x16 v1e0574;
      x16 v1e0576;
      x16 v1e0578;
      x16 v1e057a;
      x16 v1e057c;
      x16 v1e057e;
      x16 v1e0580;
      x16 v1e0582;
      x16 v1e0584;
      x16 v1e0586;
      const s_wpm* v1e0588;
      x8 v1e058c;
      x8 v1e058d;
      s_hex20 v1e058e[0x4];
      u_bbw v1e060e[0x30];
      x8 v1e066e[5]; // five bytes not adressed
      x8 v1e0673;
      x8 v1e0674;
      x8 v1e0675;
      x8 v1e0676[2]; // two bytes not adressed
      c_sizex16 v1e0678[0x10];
      c_bbbbwwwp v1e07d8;
      x16* v1e07e6;
      x8 v1e07ea;
      x8 v1e07eb;
      x8 v1e07ec;
      x8 v1e07ed;
      xxx* v1e07ee[0x2a];
      xxx v1e0896;
      x8* v1e0898; // pointer to a temporary byte-buffer, either 0x40 or 0x100 bytes

    public:
      void init(void);
      c_350(void) { init(); }
  };

  class c_event
  {
    public:
      x16 warr0[0x3];
      c_rect r6;

    public:
      c_event(void) { init(); }
      void init(void) { for (int i=0; i<3; i++) warr0[i]=0; r6.init(); };
  };

  class c_bbbb
  {
    public:
      x8 b0;
      x8 b1;
      x8 b2;
      x8 b3;

    public:
      void init(void);
      c_bbbb(void) { init(); }
  };

  class c_bbb
  {
    public:
      x8 b0;
      x8 b1;
      x8 b2;

    public:
      void init(void);
      c_bbb(void) { init(); }
  };

  // SPX: seems to be SKwin sk16ed structure
  struct s_wwwb
  {
    x16 w0;
    x16 w2;
    x16 w4;
    x8 b6;
  };

  struct s_2w // TODO: probably x,y for 2d-metrics
  {
    x16 w0[2];
  };

  struct s_www
  {
    x16 w0[3];
  };

  struct s_x40bytes
  {
    x8 barr_00[0x40];
  };

  struct s_wbbbbw
  {
    x16 w0;
    x8 b2;
    x8 b3;
    x8 b4;
    x8 b5;
    x16 w6;
  };

  struct s_bb
  {
    x8 b0;
    x8 b1;
  };

  union u_fourteen
  {
    x8 b0[0x14];
    x16 w0[0x5][0x2];
    // base adress: 0x1d672
    // 1st extra adress: 0x1d676 -> added +1 to 1st dimension
    // 2nd extra adress: 0x1d678 -> added +1 to 1st dimension, set 2nd dimension to 1
  };

  struct s_802 // for table1d6802  size 0x8
  {
    x32 l0;
    x8 b4;
    x8 b5;
    x16 w6;
  };

// src and dest come as c_image*
#define SKW_COPY_IMAGE(src, dest)  SKW_COPY_MEMORY(DOWNCAST(c_image, src), sizeof(c_image), DOWNCAST(c_image, dest))
// src and dest come as c_rect*
#define SKW_COPY_RECT(src, dest)  SKW_COPY_MEMORY(DOWNCAST(c_rect, src), sizeof(c_rect), DOWNCAST(c_rect, dest))
// src and dest come as c_s_tim*
#define SKW_COPY_TIMER(src, dest)  SKW_COPY_MEMORY(DOWNCAST(c_tim, src), sizeof(c_tim), DOWNCAST(c_tim, dest))
// src and dest come as x32*
#define SKW_COPY_LONG(src, dest)  *dest = *src // SKW_COPY_MEMORY(P8CAST(src), sizeof(x32), P8CAST(dest));
#define SKW_COPY_TEXT(src, n, dest)  SKW_COPY_MEMORY(P8CAST(src), n, P8CAST(dest))

  // this enum decides (for source and destination) the bitplane-resolutions
  // that are used to select the correct blit-algorithm

  enum e_bitres
  {
    E_BITRES4 = 4,
    E_BITRES8 = 8
  };

// SPX 2021-03-27 : Cut the Allegro stuff into "types_allegro.h" to remove some dependancies.

#endif