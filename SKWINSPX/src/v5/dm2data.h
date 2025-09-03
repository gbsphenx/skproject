#ifndef DM2DATA_H
  #define DM2DATA_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  #ifndef C_BUTTONS_H
    #include <uibutton.h>
  #endif

  #ifndef C_GFX_PAL_H
    #include <gfxpal.h>
  #endif

  #ifndef C_GFX_BMP_H
    #include <gfxbmp.h>
  #endif

  #ifndef DM2GLOBAL_H
    #include <dm2global.h>
  #endif

#include <skcorev4.h>

// #################################################################

  class c_bbbb
  {
    public:
      i8 b_00;
      i8 b_01;
      i8 b_02;
      i8 b_03;

    public:
      void init(void);
      c_bbbb(void) { init(); }
  };

// #################################################################

  struct s_bb
  {
    U8 b_00;
    U8 b_01;
  };

  union u_bl
  {
    i32 l_00;
    i8 b_00; // three upper bytes not used then
  };

  struct s_ww2
  {
    i16 w_00;
    i16 w_02;
  };

  struct s_4bytearray
  {
    U8 barr_00[0x4];
  };

  struct s_www
  {
    i16 w_00;
    i16 w_02;
    U16 w_04;
  };

  struct s_wbbbbw
  {
    i16 w_00;
    i8 b_02;
    i8 b_03;
    i8 b_04;
    i8 b_05;
    i16 w_06;
  };

  struct s_wwwb
  {
    U16 w_00;
    U16 w_02;
    U16 w_04;
    U8 b_06;
  };

  struct s_sgwords // size 0x2c
  {
    i16 warr_00[0x16];
    // types of calling:
    // w[1],w[2] this one byte only,w[3],[5],[0xa],[0xf] direct
    // from [6] as array
    // but also indexed from [0]
  };

// #################################################################

  class c_itemrecord
  {
    public:
      i16 w_00;
      t_bmp* bmp; // @02
      t_palette palette[PAL16]; // @06
      i8 b_16;
      i8 b_17; // unused fillbyte?
      i16 w_18; // written, but never read
      i16 weight; // @1a

    public:
      void init(void);
      c_itemrecord(void) { init(); }
  };

// #################################################################

  class c_wbbb // size 0x6 KEEP THAT!
  {
    public:
      i16 w_00;
      i8 b_02;
      i8 b_03;
      i8 b_04;
      i8 dummy; // unused fillbyte - needed when struct used with DM2_ZERO_MEMORY, DM2_SUPPRESS_READER/WRITER

    public:
      void init(void);
      c_wbbb(void) { init(); }
  };

// #################################################################

  class c_5bytes
  {
    public:
      i8 b_00;
      i8 b_01;
      i8 b_02;
      i8 b_03;
      i8 b_04;

    public:
      void init(void);
      c_5bytes(void) { init(); }
  };

// #################################################################

  class c_hex2a // size 0x2a
  {
    public:
      i16 w_00; // @00
      t_text text[0x24]; // @02
      i32 l_26; // @26

    public:
      void init(void);
      c_hex2a(void) { init(); }
  };

// #################################################################

  class c_rwbb
  {
    public:
      c_rect rc_00;
      i16 w_08;
      i8 b_0a;
      i8 b_0b;

    public:
      void init(void);
      c_rwbb(void) { init(); }
  };

// #################################################################

  class c_dm2data
  {
    public:
     i32 mainarg1; // TODO i8 is enough
     i8* mainarg2; // TODO checkout, convert, etc.

	SKCoreData	cd;

     // s_exittableentry exittable[(EXITTABLE2 - EXITTABLE1)/sizeof(s_exittableentry)];

     t_text* datafoldername;
     i16 v1e0988; // value range: 0...1 - index for vp_1e097c
     i8* vp_1e097c[2]; // points to two malloced 0x80 - areas
     i32 markerror;
     i32 errhandling;
     ui8 errversion;
     i16 tickconst;
     i32 gametime;
     i16 ticksum;
     i16 ticktrig;
     bool tickevent;
     bool v1e01d0; // was i16, always 0
     bool v1e01d8;
     i16 v1e025c;
     unk* ptr1e1044;
     i32 vlong1e0a3c;
     i32 vlong1e0a44;
     i32 vlong1e0a40;
     i16 v1e0ac2;
     unk* p1e0a64;
     bool v1e0a88;
     i16 v1e0ad0;
     i16 v1e0ad2;
     // i32 v1e0080; obsolete
     i16 dialog1;
     i32 dialog2;
     i16 savegamew1;
     i16 savegamefilehandle1; // was filehandle1
     c_itemrecord savegamewpc;
     i32 savegamel1;
     bool savegamel2;
     i16 savegamew4;
     i16 savegamew6;
     c_hex2a* savegamep1;
     i16 savegamew7;
     //i16 savegamew8;	// (savegamew8) glbSpecialScreen ?
     c_wbbb savegames1;
     i8 savegameb1;
     i16* savegamep3;
     i16 v1d3248;
     s_sgwords* savegamep4;
     i8 savegameb2[0x100]; // TODO: what? nothing done with that one
     i8 v1e092f;
     i8 v1e092e;
     i32 v1e08e0; // TODO: sums up in SUPPRESS_READ/WRITE/FLUSH, never requested
     i32 v1d6525; // TODO bool?
     i32 v1d6529; // TODO bool?
     i16 v1e0992;
     //i32 v1d154e; // TODO bool? obsolete, always 1
     //i16 v1dffd8; obsolete now, always 1
     //i16 v1dffd6; obsolete now
     i8 v1e099c;
     i32 v1e021c;
     i16 v1e0270;	// (v1e0270) glbPlayerPosX ?
     i16 v1e0272;	// (v1e0272) glbPlayerPosY ?
     i16 v1e0258; // often cutted to a byte when read
     i16 v1e0266;
     i32 v1d26a4;
     i32 v1e01a0;
     i16 v1e026e;
     i16 v1e025e;
     i16 v1e0274;
     i16 v1d26a0;
     i16 v1d26a2;
     i8 v1e147f;
     i8 v1e1480;
     i8 v1e1483;
     i8 v1e1482;
     i8 v1e147e;
     i8 v1e147d;
     i8 v1e1484;
     i16 v1e1474;
     i8 v1e147b;
     i8 v1e1478;
     i32 v1e1434;
     i8 v1e0104[0x8];
     i8 globalb[0x40];
     i16 globalw[0x40];
     i8 v1d6316[0x4];
     i32 v1e0250;
     u_bl v1e0390;
     i8 v1d651b;
     i16* v1e08e4;
     i16* v1e08f0;
     i32 v1d651d;
     i16 v1d6312;
     i16 v1d6310;
     i32 v1d6521;
     i16* v1e03f4;
     i16 v1d4020;
     i32 v1e08fc;
     i32 v1e08f8;
     i32 v1e13f4;
     c_hex2a v1e0900;
     unk* v1e03c8;
     unk* v1e03d8;
     i16 v1e0408;
     unk* v1e03d0;
     unk* v1e03e0;
     i16 v1d62a4;
     i16 v1d62a6;
     unk* v1e08c8;
     unk* v1e08cc;
     i16 v1e08d0;
     i16 v1e08d2;
     i16 v1e0282;
     i16 v1e027c;
     i32 v1e0238;
     s_sizee* v1e03c0;
     i32 v1dff24;
     i16 v1d13fc;
     t_text* v1d153a;
     t_text* v1d153e;
     t_text* v1d1542;
     t_text* v1d1546;
     t_text* v1d675a;	// hodling ".Z008DATA\"
     i32 v1d70dc;
     t_text* v1d70e4;
     //t_text* v1d1520; obsolete, init was v1d102c
     //t_text* v1d1524; obsolete, init was v1d1034
     //t_text* v1d1528; obsolete, init was v1d103c
     //te_text v1d102c[] = "SKDISKA"; obsolete
     //te_text v1d1034[] = "SKDISKB"; obsolete
     //te_text v1d103c[] = "SKDISKC"; obsolete
     bool vcapture2; // v1e0220
     bool vcapture1; // v1e0224
     bool vcapture3; // v1e0228
     i32 v1e03a8;
     i32 v1e048c;
     i32 v1e0478;
     t_text* v1d6766;
     t_text* v1d676a;
     unk* v1e1050;
     unk* v1e104c;
     unk* v1e1048;
     unk* v1e103c;
     unk* v1e1038;
     i16* v1e09a0;
     c_pixel* glbImageCreditScreen; // (v1e0950) 1st palette	// glbImageCreditScreen
     c_pixel* glbImageMenuScreen; // (v1e0954) 2nd palette	// glbImageMenuScreen
     unk* v1e03f0;
     i16* v1e03e4;
     unk* v1e03dc;
     i8* v1e03d4;
     unk* v1e03c4;
     unk* v1e02ec;
     i16* v1e0210;
     unk* v1e020c;
     t_text v1d70e8[4];
     i8 v1e1481;
     i8 v1e147c;
     i8 v1e147a;
     i8 v1e1479;
     i16 v1e1476;
     i16 v1e1472;
     i16 v1e1470;
     i16 v1e1420;
     i16 v1e141c;
     i16 v1e141e;
     i32 v1e1408[3];
     i32 v1e13f0;
     i32 v1e1040;
     i16 v1e09a4;
     i32 v1e1438;
     s_hexa v1e143c[5];
     i16 v1e146e;
     i16 v1e12d6;
     i16 v1e12d8;
     i16 v1e12da;
     i16 v1e12c8;
     i16 v1e12ca;
     i16 v1e12cc;
     i16 v1e12ce;
     i16 v1e12d0;
     i16 v1e12d2;
     i16 v1e098a;
     i16 v1e098c;
     i16 v1e098e;
     i16 v1e0994;
     i16 v1e0996;
     i16 v1e0998;
     i16 v1e099a;
     i16 v1e0ff6;
     i16 v1e0ff8;
     i8 v1e0ffa[0x12];
     i32 v1e1020;
     i16 v1e1024;
     i16 v1e1026;
     i16 v1e1028;
     i16 v1e102a;
     i16 v1e102c;
     i16 v1e102e;
     i16 v1e101e;
     i16 v1e101c;
     i32 v1e1018;
     i32 v1e1014;
     i16 v1e100c;
     i8 v1e13e9;
     i8 v1e13dc[0xd];
     i8 v1e0adc[0x40];
     i32 v1e0984;
     i16 v1e0976;
     i16 v1e0974;
     i16 v1e0978;
     i16 glbEndCounter; // v1e0526
     i16 v1e051e;
     i16 v1e0510;
     i32 v1e0484;
     bool glbRefreshViewport; // v1e0488
     i16 v1e0bb0[4];
     i16 v1e0ba8[4];
     i8 v1e0ba4;
     i16 v1e0b99;
     t_text v1e0b85[20];
     i8 v1e0b84;
     i8 v1e0b83;
     i8 v1e0b82;
     i8 v1e0b81;
     i8 v1e0b80;
     i8 v1e0b7f;
     i8 v1e0b7e;
     i16 v1e0b7c;
     i16 v1e0b7a;
     i16 v1e0b78;
     i16 v1e0b72;
     i16 v1e0b70;
     i16 v1e0b6e;
     i16 v1e0b6c;
     i16 v1e0b6a;
     i16 v1e0b68;
     i16 v1e0b66;
     i16 v1e0b64;
     i16 v1e0b62;
     i16 v1e0b60;
     i16 v1e0b5e;
     i16 v1e0b5c;
     i16 v1e0b5a;
     i16 v1e0b58;
     i16 v1e0b56;
     i16 v1e0b54;
     i16 v1e0b52;
     i16 v1e0b50;
     i16 v1e0b4e;
     i16 v1e0b4c;
     c_bbbb v1e0b40[3];
     i8 v1e0b34[4][3];
     i32 v1e0b30;
     i32 v1e0b2c;
     i32 v1e0b1c[4];
     i8 v1e096c[4];
     t_text v1e0930[0x20];
     i16 v1e092a;
     i16 v1e08da;
     i16 v1e08d8;
     i16 v1e08d6;
     i16 v1e08d4;
     i16 v1e08c4;
     i8 v1e08c0[4];
     i8 v1e08bf;
     i8 v1e08be;
     c_5bytes v1e08b8;
     i8 v1e08b6;
     i8 v1e08b7;
     i16 v1e08b4;
     i16 v1e08b2;
     i16 v1e08b0;
     i16 v1e08ae;
     i16 v1e08ac;
     i16 v1e08aa;
     i16 v1e08a8;
     i16 v1e08a6;
     i16 v1e08a4;
     i16 v1e08a0;
     i8 v1e040e[2];
     i16 v1e040c;
     i16 v1e040a;
     i16 v1e0288;
     i16 v1e0286;
     i16 v1e0280;
     i16 v1e027e;
     i16 v1e0278;
     i16 v1e026a;
     i16 v1e0268;
     i16 v1e0264;
     i16 v1e0262;
     i16 v1e0260;
     i16 v1e0256;
     i16 v1e0254;
     i32 v1e0234;
     i16 v1e0218;
     i32 v1e024c;
     i32 v1e0248;
     i32 v1e0240;
     i32 v1e023c;
     i8 v1e0548;
     i32 v1e01d4;
     i32 v1e0438;
     i8 v1e0426[0x12];
     i8 v1e0414[0x12];
/*
	 v1e0414[17] = glbCreaturesMaxCount
	 */
     i16 v1e03f8;
     i16 v1e03fa;
     i16 v1e03fc;
     i16 v1e03fe;
     i16 v1e0400;
     i16 v1e0402;
     i16 v1e0404;
     i16 v1e0406;
     i16 v1e03ac[0xa];
     i16 v1e0394[0xa];
     c_rwbb v1e02f0[0xd];
     i8 v1e02dc[0x10];
     i8 v1e02cc[0x10];
     i16 v1e0204;
     i16 v1e01a4;
     i16 v1e01a6;
     i16 v1e01a8;
     i16 v1e00b0;
     i16 v1e00b2;
     i16 v1e00b4;
     i16 v1e00b6;
     i16 v1e00b8;
     i16 v1e000c[0x38];
     //i16 v1dffd4; obsolete, init was 3
     //i16 v1dffd2; obsolete, init was 3
     // i16 v1dffd0; obsolete, init was 0
     i32 v1d718c;
     i32 v1d7188;
     i8 v1d7108[0x80];
     i32 v1d70d3;
     i16 v1d70ea;
     i32 v1d70cf;
     i32 v1d70cb;
     i16 v1d70c9;
     i32 v1d70c5;
     i8 v1d7041;
     i16 v1d703f;
     i16 v1d703d;
     i32 last_inputtime; // was v1d3fed
     i16 v1d4000;
     i16 v1d27f8;
     i16 v1d2736;
     i16 v1d2710;
     i16 v1d2708;
     //i32 v1d154a; obsolete, always 1
     i32 v1d66f8;
     i16 v1d62a8;
     i16 v1d62aa;
     i32 v1d62ac;
     ui16 v1d62ec;
     i32 v1d652d;
     i16 v1d674c;
     i32 v1d6727;
     i32 v1d268a;
     i16 v1d26fc;
     i16 v1d26fe;
     i16 v1d271a;
     i16 v1d271c;
     i16 v1d2722;
     i16 v1d2724;
     i16 v1d2726;
     i16 v1d2728;
     i16 v1d272a;
     i16 v1d272c;
     i16 v1d272e;
     i16 v1d2744;
     i16 v1d2746;
     i16 v1d2748;
     i16 v1d274a;
     i16 v1d274c;
     i16 v1d274e;
     i16 v1d2750;
     i16 v1d324a;
     i16 v1d3ff1;
     i16 v1d66fc;
     i16 v1d66fe;
     i16 v1d6700;
     s_wbbbbw v1d653c[0x37];
     i16 v1d6795;
     i16 v1d6c00;
     i8 v1d6c02;
     i16 v1d6c04;
     i16 v1d67bc;
     i8 v1d67d6;
     i16 v1d6948;
     i16 v1d694a;
     i32 v1d6a2d;
     i32 v1d6c15;
     i16 v1d6bfa;
     i32 v1d6bfc;
     i8 v1e0206;
     i8 v1e0207;
     i8 v1e0208;
     i16 v1e052e;
     i16 v1e0530;
     t_text v1e052c[0x2];
     i32 v1e0534;
     i16 v1e0538;
     i16 v1e053a;
     i16 v1e053c;
     i16 v1e053e;
     i16 v1e0540;
     i16 v1e0542;
     i16 v1e0544;
     i16 v1e0546;
     i16 v1d2740;
     i16 v1d273e;
     i16 v1d270e;

    public:
     void init(void);
  };

  extern c_dm2data ddat;

  extern const i8 table1d6702[0x10];
  extern const i8 table1d6712[0x15];
  extern const U8 table1d281c[0x10];
  extern /*const*/ i8 table1d282c[0x10];
  extern U8 table1d631a[0x3c]; // TODO later const, when last param of DM2_SUPPRESS_READER can be const

  extern U8 vsgame[0x78];
#define v1d6463 (i8*)(vsgame + (U8)0x00)
#define v1d646f (i8*)(vsgame + (U8)0x0c)
#define v1d6473 (i8*)(vsgame + (U8)0x10)
#define v1d6477 (i8*)(vsgame + (U8)0x14)
#define v1d647f (i8*)(vsgame + (U8)0x1c)
#define v1d648f (i8*)(vsgame + (U8)0x2c)
#define v1d649f (i8*)(vsgame + (U8)0x3c)
#define v1d64a3 (i8*)(vsgame + (U8)0x40)
#define v1d64a7 (i8*)(vsgame + (U8)0x44)
#define v1d64ab (i8*)(vsgame + (U8)0x48)
#define v1d64af (i8*)(vsgame + (U8)0x4c)
#define v1d64b7 (i8*)(vsgame + (U8)0x54)
#define v1d64bf      (vsgame + (U8)0x5c)
#define v1d64c3      (vsgame + (U8)0x60)
#define v1d64c7 (i8*)(vsgame + (U8)0x64)
#define v1d64cf (i8*)(vsgame + (U8)0x6c)
#define v1d64d3 (i8*)(vsgame + (U8)0x70)

  extern /*const*/ t_text* table1d27e8[3];
  extern te_text strVersionNumber[];	// v1d1044
  extern te_text v1d1118[];
  extern te_text v1d1124[];
  extern te_text v1d1128[];
  extern te_text v1d112b[];
  extern te_text v1d112e[];
  extern te_text v1d1131[];
  extern te_text v1d1134[];
  extern te_text v1d1137[];
  extern te_text v1d1140[];
  extern te_text v1d1143[];
  extern te_text v1d10f4[];
  extern U8* table1d64db[16];
  extern U8 table1d6356[263];
  extern i8 table1d645d[6];
  extern i16 table1d27fc[4];
  extern i16 table1d2804[4];
  // extern ui32 table1d83ec[32];
  extern U8 tblMusicsMap[64];
  extern te_text v1d10f2[];
  extern te_text v1d10f0[];
  //extern te_text v1d10eb[];	// replaced by strVersionNumber
  extern te_text v1d10e4[];
  extern i8 table1d70f0[24];
  extern const ui32 table1d7092[8];
  extern const ui32 table1d7072[8];
  extern const ui32 table1d7052[8];
  extern const ui32 table1d7042[4];
  extern const ui8 table1d7029[20];
  extern const ui8 table1d7012[23];
  extern const U16 table1d6fee[18];
  extern const U8 table1d6fdc[18];
  extern const U16 table1d6f9c[32];
  extern const U8 table1d6f7c[32];
  extern const U8 table1d6f5c[32];
  extern const ui8 table1d6f4c[16];
  extern const U16 table1d6f2c[16];
  extern const U8 table1d6f27[5];
  extern const i16 table1d6f0b[14];
  extern const i8 table1d6290[9];
  extern const i16 table1d6299[5];
  extern const i8 table1d26a8[32];
  extern const i8 table1d62ee[30];
  extern const i8 table1d62e8[4];
  extern const i16 table1d62e0[4];
  extern const i16 table1d62b0[8][2];
  extern const i16 table1d62d0[4][2];
  extern const i8 table1d3ffc[4];
  extern const i8 table1d3ff8[4];
  extern const i16 table1d672b[9];
  extern const i16 table1d673d[7];
  extern te_text* table1d674e[5];
  extern /*const*/ te_text* _table1d674e[5];
  extern const i16 table1d27c4[8];
  extern const i16 table1d27d4[10];
  extern const i8 table1d268e[6];
  extern const i8 table1d2660[16];
  extern const i16 table1d2670[13];
  extern const i8 table1d26c8[8];
  extern const s_4bytearray table1d26d0[8];
  extern const s_4bytearray table1d26f0[2];
  extern const i8 table1d26f8[4];
  extern const i16 table1d2752[4];
  extern const U8 table1d275a[32][2];
  extern const i8 table1d324c[44];
  extern const U16 table1d3278[16];
  extern const U8 table1d3298[16];
  extern s_bbw table1d3ed5[10];
  extern /*const*/ s_bbw _table1d3ed5[10];
  extern s_wwwb _table1d3d23[62];
  extern s_wwwb table1d3d23[62];
  extern const U8 table1d3cd0[83];
  extern s_bbw table1d3ba0[76];
  extern /*const*/ s_bbw _table1d3ba0[76];
  extern /*const*/ s_ww2 v1d39bc[121]; // fileread
  extern /*const*/ s_www v1d338c[264]; // fileread
  extern t_text v1d5c31[9];
  extern t_text v1d6a4f[5];
  extern const U8 table1d6afe[23];
  extern const s_4bytearray table1d6a74[23];
  extern const U8 table1d6ad0[23][2];
  extern const U8 table1d6b43[23];
  extern const U8 table1d6b5a[23];
  extern const i8 table1d6b2c[23];
  extern const i8 table1d6b15[23];
  extern /*const*/ i8 table1d6a54[4][4]; // really const
  extern /*const*/ i8 table1d6a64[4][4]; // really const
  extern const i8 table1d6b71[5];
  extern const U8 table1d6efd[14];
  extern const U8 table1d6ee1[14][2];
  extern const U8 table1d6ed3[14];
  extern const s_bb table1d6eb3[16];
  extern const i8 table1d6ea8[3];
  extern const i8 table1d6eab[4];
  extern const i8 table1d6eaf[4];
  extern const s_bb table1d6e68[4][8];
  extern const U8 table1d6e51[23];
  extern const U8 table1d6797[37];
  extern i8 v1d67be[4];
  extern U8 _v1d67be[4];
  extern const i8 table1d6e41[16];
  extern const i8 table1d6e35[12];
  extern const U16 table1d6c70[16];
  extern const i8 table1d6c90[16];
  extern const U8 table1d6ca0[16];
  extern const U8 table1d6cb0[16];
  extern const i16 table1d6cc0[16];
  extern const i8 table1d6c10[5];
  extern const i8 table1d6c19[5];
  extern const i8 table1d6c1e[23];
  extern const i8 table1d6c35[23];
  extern const i16 table1d6c4c[9];
  extern const U8 table1d6c5e[9];
  extern const i8 table1d6c67[9];
  extern const U8 table1d6c06[5];
  extern const i8 table1d6c0b[5];
  extern const s_www table1d6a31[5];
  extern const t_text v1d6a08[38];
  extern const i8 table1d6e03[26][2];
  extern const i8 table1d6de3[16][2];
  extern const i8 table1d6dd3[16];
  extern const i8 table1d69aa[6];
  extern const i8 table1d69a2[6];
  extern const U16 table1d6998[5];
  extern const U8 table1d6984[20];
  extern const i8 table1d6980[4];
  extern const i8 table1d69b0[32];
  extern const i8 table1d69d0[4];
  extern const U8 table1d6b76[132];
  extern const i16 table1d6d3c[6];
  extern const i8 table1d6d48[6];
  extern const i8 table1d6d4e[6];
  extern const i8 table1d6d54[6];
  extern const U8 table1d6ce0[23 * 4];
  extern const U8 table1d6d5a[4][5];
  extern const s_bb table1d6d6e[4];
  extern const U8 table1d6d76[2][4];
  extern const U8 table1d6d7e[2][4];
  extern const i8 table1d6d86[2];
  extern const i8 table1d6d88[25];
  extern const i8 table1d6da1[25];
  extern const i8 table1d6dba[25];
  extern const i8 table1d67d9[7];
  extern const i8 table1d67e0[5][6];
  extern const i8 table1d67fe[4];
  extern /*const*/ i8 table1d6802[272]; // fileread
  extern const t_text* table1d6912[18];
  extern const i16 table1d292c[32];
  extern /*const*/ i8 table1d296c[63][36]; // fileread
  extern const te_text v1d111f[];
  extern i8 table1d70b4[17];
  extern U8 _table1d70b4[17];

#endif
