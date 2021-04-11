#ifndef DM2DATA_H
  #define DM2DATA_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  #ifndef STARTEND_H
    #include "startend.h"
  #endif

  #ifndef DM2GLOBAL_H
    #include "../dm2global.h"
  #endif

  #define NUM_MOUSEBUFENTRIES 11
  #define NUM_QUERYRECTS  4
  #define NUM_TMPRECTS  4

  extern const i16 table1d27a0[0x10];
  struct s_xmouserect; // in driver.h
  struct s_mouseentry2; // in driver.h

// IMPORTANT NOTE:
// it's pretty dangerous to change the order of variables here.
// some things are obviously grouped to higher structures and
// belong together.

  class c_dm2data
  {
    public:
     i32 mainarg1; // TODO i8 is enough
     i8* mainarg2; // TODO checkout, convert, etc.

     // s_exittableentry exittable[(EXITTABLE2 - EXITTABLE1)/sizeof(s_exittableentry)];

     i8* drvinterfaceptr;
     t_text* datafoldername;
     const t_text* gdatfilename1;
     const t_text* gdatfilename2;
     i16 v1e0988; // value range: 0...1 - index for vp_1e097c
     t_text* vp_1e097c[2]; // points to two malloced 0x80 - areas
     i32 longmarkerror;
     i32 longerrhandling;
     ui8 errversion;
     i16 mouse_visibility;
     bool fetchmouse_busy;
     bool singlemousedata_available;
     c_mousedata singlemouse;
     bool mousebutton0x2;
     i16 mousebufindex;
     i16 mousebufentries;
     c_mousebufentry mousebuf[NUM_MOUSEBUFENTRIES];
     i16 tickconst;
     i32 longgametime;
     i16 ticksum;
     i16 ticktrig;
     bool tickevent;
     bool mouse_setrect;
     i16 mouse_rx0;
     i16 mouse_rx1;
     i16 mouse_ry0;
     i16 mouse_ry1;
     c_rect mouse_r;
     s_rnode* rectanglelist;
     i16 queryrectindex;
     c_rect queryrects[NUM_QUERYRECTS];
     i16 tmprectindex;
     c_rect tmprects[NUM_TMPRECTS];
     c_rect arect;
     x16 v1e01d0;
     bool v1e01d8;
     x16 v1e025c;
     c_rect vx1d2712;
     x16 vx1d270a;
     x16 vx1d270c;
     x16 mouse_unk01;
     x16 mouse_unk02;
     x16 mouse_unk03;
     x16 mouse_unk04;
     x16 mouse_unk05;
     x16 mouse_unk06;
     x16 mouse_unk07;
     x16 mouse_unk08;
     x16 mouse_unk09;
     x16 mouse_unk0a;
     s_xmouserect* drvxmouserectptr;
     x16 mouse_unk0b;
     x16 mouse_unk0c;
     x16 mouse_unk0d;
     x16 mouse_unk0e;
     bool mouse_unk0f;
     s_mouseentry2* drvmouseentryptr;
     s_munknown* mouse_unkptr;
     s_munknown* mouse_unkptr2;
     x16 filehandles; // was x32, not needed TODO: a const-define should do it
     x32* devicetableptr;
     x32 longstatecheck;
     x32 longfilestatexormask;
     s_hex12* v1e1044;
     t_gfxdata* srcbmp; // blit-operations
     t_gfxdata* destbmp; // blit-operations
     t_gfxdata dm2screen[ORIG_SWIDTH][ORIG_SHEIGHT];
     t_gfxdata* screenptr;
     x32 longmallocx;
     x16 gdatfileopencounter;
     x16 gdatfilehandle;
     x16 gdatxfilehandle;
     x32 longgdatfilesize;
     x16 gdatversionlo;
     ui16 gdatentries;
     x32 longv1e0a3c; // confirmed long
     x32 longv1e0a44; // confirmed long
     x32 longv1e0a40; // confirmed long
     bool gdatfiletype1;
     bool gdatfiletype2;
     x16 v1e0ac2;
     x8* p1e0a64;
     bool v1e0a88;
     x16 v1e0ad0;
     x16 v1e0ad2;
     ui32 ulongrandom; // confirmed
     x16 img3decode;
     x8* decodeptr;
     x16 decodew0;
     x16 decodew1;
     x16 decodew2;
     x16 decodew3;
     x16 decodew4;
     x16 decodew5;
     x8* decodeptr2;
     x8* decodeptr3;
     x16 decodew6;
     x16 decodew7;
     x8 decodemask[9];

// next one moved to c_font_metrics, kept because of data alignment
// seperating this ends in crashes!
     x8* dontuse_fontptr;
     t_gfxdata dontuse_gfxletter[6*3];
     x8 dontuse_unknown[6]; // let that stay here!

     t_gfxdata* bitmapptr;
     t_gfxdata* paletteptr1;

// next one moved to c_string_metrics, kept because of data alignment
// seperating this ends in crashes!
     x16 dontuse_gfxstrw1;
     x16 dontuse_gfxstrw2;
     x16 dontuse_gfxstrw3;
     x16 dontuse_gfxstrw4;

     x16 strx;
     x16 stry;
     x16 strxplus;
     t_gfxdata* stretchptr;
     x16 backbuff1;
     x16 backbuff2;
     bool v1e0080;
     x16 dialog1;
     bool dialog2;
     c_rect backbuffrect;
     t_gfxdata* paletteptr2;
     x8 blitb[0x1000];
     c_rect mouse_rect;
     x8 mouse_cur1[0x60];
     x8 mouse_cur2[0x90];
     bool savegamew1;
     const t_text* gdatfilename3;
     x16 filehandle1;
     const t_text* gdatfilename4;
     ui16 savegamew2;
     c_wpc savegamewpc;
     x32 longsavegamel1; // confirmed long
     bool boolsavegamel2;
     x16 savegamew4;
     x16 savegamew5;
     x16 savegamew6;
     c_hex2a* savegamep1;
     x16 savegamew7;
     x16 savegamew8; // one time used non-bool!
     c_wbbb savegames1;
     c_tim* savegametimarrp;
     x8 savegameb1;
     x16* savegamepw3;
     x16 savegamew9;
     c_hero hero[4];
     x16 v1d3248;
     s_sgwords* savegamep4;
     x16 savegamewa;
     x16 savegamewb;
     c_record* savegameparr5[0x10];
     x8 savegameb2[0x100]; // TODO: what? nothing done with that one
     x8** savegamepp1;
     x8 v1e092f;
     x8 v1e092e;
     x32 longv1e08e0; // TODO: sums up in SUPPRESS_READ/WRITE/FLUSH, but never requested, confirmed long
     bool v1d6525;
     bool v1d6529;
     x16 v1e0992;
     bool v1d154e; // a const setting
     x16 v1dffd8;
     x16 v1dffd6;
     x8 v1e099c;
     const t_text* gdatfilename5;
     const t_text* gdatfilename6;
     const t_text* gdatfilename7;
     x32 longv1e021c; // confirmed long
     x16 v1e0270;
     x16 v1e0272;
     x16 v1e0258; // often cutted to a byte when read
     x16 v1e0266;
     x32 longv1d26a4; // confirmed long
     x32 longv1e01a0; // confirmed long (cutted to a word one time)
     x16 v1e026e;
     x16 v1e025e;
     x16 v1e0274;
     x16 v1d26a0;
     x16 v1d26a2;
     x8 v1e147f;
     x8 v1e1480;
     x8 v1e1483;
     x8 v1e1482;
     x8 v1e147e;
     x8 v1e147d;
     x8 v1e1484;
     x16 v1e1474;
     x8 v1e147b;
     x8 v1e1478;
     x32 longv1e1434; // confirmed long
     x8 v1e0104[0x8];
     x8 globalb[0x40];
     x16 globalw[0x40];
     x8 v1d6316[0x4];
     x16 v1e142c;
     bool v1e0250;
     u_bl v1e0390;
     x8 v1d651b;
     x16* v1e08e4;
     x16* v1e08f0;
     bool v1d651d;
     x16 v1d6312;
     x16 v1d6310;
     bool v1d6521;
     x16* v1e03f4;
     x16* v1e038c;
     x16 v1d4020;
     s_d556* v1e089c;
     bool v1e08fc;
     x32 longv1e08f8; // confirmed long
     bool v1e13f4;
     c_hex2a v1e0900;
     s_size10* v1e03c8;
     x16* v1e03d8;
     x16 v1e0408;
     x16* v1e03d0;
     x8* v1e03e0;
     x16 v1d62a4;
     x16 v1d62a6;
     s_32bytes* v1e08c8;
     s_32bytes* v1e08cc;
     x16 v1e08d0;
     x16 v1e08d2;
     x16 v1e0282;
     x16 v1e027c;
     ui16 v1d2694;
     ui16 v1d2696;
     bool v1e0238;
     s_size10* v1e03c0;
     x8 v1dd1d1;
     x32 longv1da334[8]; // smth for MIDI, TODO: check if has preset
     x32 longv1dd1d2[8]; // smth for MIDI, TODO: check if has preset
     x16 v1d1400; // smth with sound, set, but never requested
     x16 v1d1410; // smth with sound, set, but never requested
     x16 v1d1420; // smth with sound, set, but never requested
     x32 longv1dff24; // not really needed anymore too
     x16 v1dfda4[0x40];
     x16 v1dff88;
     x16 v1d1438; // set, but never requested
     x32 longv1d143c; // set, but never requested, let it x32
     x32 longv1d140c; // set, but never requested, let it x32
     bool v1d14c2;
     bool v1d14cc;
     x16 v1d14ca;
     bool v1d14d0;
     bool v1d14d4;
     x16 v1dff86;
     bool v1d14be;
     x8 v1dff8a;
     x16 v1d1512;
     x16 v1dff2c;
     bool v1d14da;
     x8* v1d13f8;
     x16 v1d13fc;
     x16 v1d14d8;
     c_lw v1da374[0x8];
     t_text* v1d153a;
     t_text* v1d153e;
     t_text* v1d1542;
     t_text* v1d1546;
     t_text* v1d675a;
     bool v1d70dc;
     const t_text* v1d70e4;
     const t_text* v1d1520;
     const t_text* v1d1524;
     const t_text* v1d1528;
     bool v1e0220;
     bool v1e0224;
     bool v1e0228;
     bool v1e03a8;
     bool v1e048c;
     bool v1e0478;
     t_gfxdata* v1e0480;
     t_gfxdata* v1d66f4;
     t_text* v1d6766;
     t_text* v1d676a;
     x16* v1e1424;
     t_gfxdata* v1e1414;
     c_image v1e118e;
     s_hex11bytes* v1e1050;
     x32* v1e104c;
     s_hex11bytes* v1e1048;
     c_record* v1e103c;
     x32* v1e1038;
     x16* v1e09a0;
     t_gfxdata* v1e0950;
     t_gfxdata* v1e0954;	// (v1e0954) glbImageMenuScreen
     t_palette* v1e0958;
     x8* v1e03f0;
     x16* v1e03e4;
     x8* v1e03dc;
     x8* v1e03d4;
     x8*** v1e03cc;
     x8* v1e03c4;
     x8* v1e02ec;
     x16* v1e0210;
     s_bpp* v1e020c;
     t_text v1d70e8[4];
     x8 v1e1481;
     x8 v1e147c;
     x8 v1e147a;
     x8 v1e1479;
     x16 v1e1476;
     x16 v1e142a;
     x16 v1e1428;
     x16 v1e1472;
     x16 v1e1470;
     x16 v1e1420;
     x16 v1e141c;
     x16 v1e141e;
     x32 v1e1408[3]; // confirmed long
     bool v1e13f0;
     x32 longv1e1040; // TODO: check again, word seems to be enough, is a mask
     x16 v1e09a4;
     x32 longv1e1438; // confirmed long
     s_xabytes v1e143c[0x5];
     x16 v1e146e;
     x16 v1e12d6;
     i16 v1e12d8;
     x16 v1e12da;
     t_gfxdata v1e12dc[PAL16];
     x16 v1e12c8;
     x16 v1e12ca;
     x16 v1e12cc;
     x16 v1e12ce;
     x16 v1e12d0;
     x16 v1e12d2;
     x16 v1e098a;
     x16 v1e098c;
     x16 v1e098e;
     x16 v1e0994;
     x16 v1e0996;
     x16 v1e0998;
     x16 v1e099a;
     x16 v1e0ff6;
     x16 v1e0ff8;
     x8 v1e0ffa[0x12];
     t_text fnum[5];
     c_image v1e1054;
     x16 v1e1020;
     x16 v1e1024;
     x16 v1e1026;
     x16 v1e1028;
     x16 v1e102a;
     x16 v1e102c;
     x16 v1e102e;
     x16 v1e101e;
     x16 v1e101c;
     bool v1e1018;
     bool v1e1014;
     x16 v1e100c;
     x8 v1e13e9;
     x8 v1e13dc[0xd];
     s_size8 v1e0adc[0x4][0x2];
     x16 v1e0984;
     x16 v1e0976;
     x16 v1e0974;
     x16 v1e0978;
     x16 v1e0526;
     x16 v1e051e;
     x16 v1e0510;
     c_event v1e04d2;
     c_350 s350;
     bool v1e0484;
     bool v1e0488;
     x16 v1e0bb0[4];
     x16 v1e0ba8[4];
     x8 v1e0ba4;
     c_rect v1e0b9c;
     x16 v1e0b99;
     t_text v1e0b85[0x14];
     x8 v1e0b84;
     x8 v1e0b83;
     x8 v1e0b82;
     x8 v1e0b81;
     x8 v1e0b80;
     x8 v1e0b7f;
     x8 v1e0b7e;
     ui16 v1e0b7c;
     x16 v1e0b7a;
     x16 v1e0b78;
     x16 v1e0b76;
     x16 v1e0b74;
     x16 v1e0b72;
     x16 v1e0b70;
     x16 v1e0b6e;
     x16 v1e0b6c;
     x16 v1e0b6a;
     x16 v1e0b68;
     x16 v1e0b66;
     x16 v1e0b64;
     x16 v1e0b62;
     x16 v1e0b60;
     x16 v1e0b5e;
     x16 v1e0b5c;
     x16 v1e0b5a;
     x16 v1e0b58;
     x16 v1e0b56;
     x16 v1e0b54;
     x16 v1e0b52;
     x16 v1e0b50;
     x16 v1e0b4e;
     x16 v1e0b4c;
     c_bbbb v1e0b40[3];
     x8 v1e0b34[4][3];
     bool v1e0b30;
     bool v1e0b2c;
     bool v1e0b1c[4];
     x16 v1e095c[8];
     x8 v1e096c[4];
     t_text v1e0930[0x20];
     x16 v1e092a;
     x16 v1e08da;
     x16 v1e08d8;
     x16 v1e08d6;
     x16 v1e08d4;
     x16 v1e08c4;
     x8 v1e08c0[4];
     x8 v1e08bf;
     x8 v1e08be;
     c_5bytes v1e08b8;
     x8 v1e08b6;
     x8 v1e08b7;
     x16 v1e08b4;
     x16 v1e08b2;
     x16 v1e08b0;
     x16 v1e08ae;
     x16 v1e08ac;
     x16 v1e08aa;
     x16 v1e08a8;
     x16 v1e08a6;
     x16 v1e08a4;
     x16 v1e08a0;
     x8 v1e040e[2];
     x16 v1e040c;
     x16 v1e040a;
     x16 v1e0288;
     x16 v1e0286;
     x16 v1e0280;
     x16 v1e027e;
     x16 v1e0278;
     x16 v1e0276;
     x16 v1e026a;
     x16 v1e0268;
     x16 v1e0264;
     x16 v1e0262;
     x16 v1e0260;
     x16 v1e0256;
     x16 v1e0254;
     bool v1e0234;
     x16 v1e0218;
     bool v1e024c;
     bool v1e0248;
     bool v1e0240;
     bool v1e023c;
     x8 v1e0548;
     bool v1e01d4;
     c_rect v1e0470;
     bool v1e0438;
     x8 v1e0426[0x12];
     x8 v1e0414[0x12];
     x16 v1e03f8;
     x16 v1e03fa;
     x16 v1e03fc;
     x16 v1e03fe;
     x16 v1e0400;
     x16 v1e0402;
     x16 v1e0404;
     x16 v1e0406;
     x16 v1e03ac[0xa];
     x16 v1e0394[0xa];
     c_rwbb v1e02f0[0xd];
     x8 v1e02dc[0x10];
     x8 v1e02cc[0x10];
     x16 v1e0204;
     x16 v1e01a4;
     x16 v1e01a6;
     x16 v1e01a8;
     x16 v1e00b0;
     x16 v1e00b2;
     x16 v1e00b4;
     x16 v1e00b6;
     x16 v1e00b8;
     x16 v1e000c[0x38];
     x16 v1dffd4;
     x16 v1dffd2;
     x16 v1dffd0;
     bool v1d718c;
     bool v1d7188;
     x8 v1d7108[0x80];
     bool v1d70d3;
     x16 v1d70ea;
     bool v1d70cf;
     bool v1d70cb; // set, but never requested
     x16 v1d70c9;
     x32 longv1d70c5; // confirmed long
     x8 v1d7041;
     x16 v1d703f;
     x16 v1d703d;
     x32 longv1d3fed; // confirmed long (but word would be enough)
     x16 v1d4000;
     x16 v1d27f8;
     x16 v1d2736;
     x16 v1d2710;
     x16 v1d2708;
     bool v1d154a;
     bool v1d66f8;
     x16 v1d62a8;
     x16 v1d62aa;
     x16 v1d62ac;
     ui16 v1d62ec;
     bool v1d652d;
     x16 v1d674c;
     bool v1d6727;
     bool v1d268a;
     c_rect v1d2658;
     x16 v1d26fc;
     x16 v1d26fe;
     x16 v1d271a;
     x16 v1d271c;
     x16 v1d2722;
     x16 v1d2724;
     x16 v1d2726;
     x16 v1d2728;
     x16 v1d272a;
     x16 v1d272c;
     x16 v1d272e;
     x16 v1d2744;
     x16 v1d2746;
     x16 v1d2748;
     x16 v1d274a;
     x16 v1d274c;
     x16 v1d274e;
     x16 v1d2750;
     x16 v1d324a;
     x16 v1d3ff1;
     x16 v1d66fc;
     x16 v1d66fe;
     x16 v1d6700;
     s_wbbbbw v1d653c[0x37];
     x16 v1d6795;
     x16 v1d6c00;
     x8 v1d6c02;
     x16 v1d6c04;
     x16 v1d67bc;
     x16 v1d67d7;
     x8 v1d67d6;
     x16 v1d6948;
     x16 v1d694a;
     bool v1d6a2d;
     bool v1d6c15;
     u_fourteen v1d67c2;
     x16 v1d6bfa;
     x16 v1d6bfc; // was long, set, but never requested
     c_c5rects v1d69d4;
     c_c5rects v1d694c;
     x8 v1e0206;
     x8 v1e0207;
     x8 v1e0208;
     x16 v1e052e;
     x16 v1e0530;
     t_text v1e052c[0x2];
     bool v1e0534;
     x16 v1e0538;
     x16 v1e053a;
     x16 v1e053c;
     x16 v1e053e;
     x16 v1e0540;
     x16 v1e0542;
     x16 v1e0544;
     x16 v1e0546;
     x16 v1d2740;
     x16 v1d273e;
     x16 v1d270e;

    public:
     void init(void);
  };

  extern c_dm2data ddata;

  extern const x8 table1d6702[0x10];
  extern const x8 table1d6712[0x15];
  extern const ui8 table1d280c[0x10];
  extern const x8 table1d281c[0x10];
  extern const t_text table1d282c[0x10];
  extern const x8 table1d631a[0x3c];

  extern const x8 vsgame[0x78];
#define v1d6463 (vsgame+0x00)
#define v1d646f (vsgame+0x0c)
#define v1d6473 (vsgame+0x10)
#define v1d6477 (vsgame+0x14)
#define v1d647f (vsgame+0x1c)
#define v1d648f (vsgame+0x2c)
#define v1d649f (vsgame+0x3c)
#define v1d64a3 (vsgame+0x40)
#define v1d64a7 (vsgame+0x44)
#define v1d64ab (vsgame+0x48)
#define v1d64af (vsgame+0x4c)
#define v1d64b7 (vsgame+0x54)
#define v1d64bf (vsgame+0x5c)
#define v1d64c3 (vsgame+0x60)
#define v1d64c7 (vsgame+0x64)
#define v1d64cf (vsgame+0x6c)
#define v1d64d3 (vsgame+0x70)

  extern const t_text* table1d27e8[0x3];
  extern const t_text v1d1044[];
  extern const t_text v1d1118[];
  extern const t_text v1d1124[];
  extern const t_text v1d1128[];
  extern const t_text v1d112b[];
  extern const t_text v1d112e[];
  extern const t_text v1d1131[];
  extern const t_text v1d1134[];
  extern const t_text v1d1137[];
  extern const t_text v1d1140[];
  extern const t_text v1d1143[];
  extern t_text v1d10f4[1];
  extern const x8* table1d64db[0x10];
  extern const x8 table1d6356[0x107];
  extern const x8 table1d645d[0x6];
  extern const x16 table1d27fc[0x4];
  extern const x16 table1d2804[0x4];
  extern const ui16 table1d14e2[0x18];
  // extern ui32 table1d83ec[0x20];
  extern const x8 table1410ec[0x40];
  extern const t_text v1d10f2[];
  extern const t_text v1d10f0[];
  extern const t_text v1d10eb[];
  extern const t_text v1d10e4[];
  extern const x8 table1d70f0[0x18];
  extern const ui32 table1d7092[0x8];
  extern const ui32 table1d7072[0x8];
  extern const ui32 table1d7052[0x8];
  extern const ui32 table1d7042[0x4];
  extern const ui8 table1d7029[0x14];
  extern const ui8 table1d7012[0x17];
  extern const x16 table1d6fee[0x12];
  extern const x8 table1d6fdc[0x12];
  extern const x16 table1d6f9c[0x20];
  extern const x8 table1d6f7c[0x20];
  extern const x8 table1d6f5c[0x20];
  extern const ui8 table1d6f4c[0x10];
  extern const x16 table1d6f2c[0x10];
  extern const i8 table1d6f27[0x5];
  extern const x16 table1d6f0b[0xe];
  extern const x8 table1d6290[0x9];
  extern const x16 table1d6299[0x5];
  extern const x8 table1d26a8[0x20];
  extern const x8 table1d62ee[0x1e];
  extern const x8 table1d62e8[0x4];
  extern const x16 table1d62e0[0x4];
  extern const x16 table1d62b0[8][2];
  extern const x16 table1d62d0[4][2];
  extern const x8 table1d2558[0x2];
  extern const x8 table1d255a[0x2];
  extern const x8 table1d3ffc[0x4];
  extern const x8 table1d3ff8[0x4];
  extern const x16 table1d672b[0x9];
  extern const x16 table1d673d[0x7];
  extern t_text* table1d674e[0x5];
  extern const x16 table1d27c4[0x8];
  extern const x16 table1d27d4[0xa];
  extern const x8 table1d268e[0x6];
  extern const x8 table1d2660[0x10];
  extern const x16 table1d2670[0xd];
  extern const x8 table1d26c8[0x8];
  extern const s_4bytearray table1d26d0[0x8];
  extern const s_4bytearray table1d26f0[0x2];
  extern const x8 table1d26f8[0x4];
  extern const x16 table1d2752[0x4];
  extern const x8 table1d275a[0x20][0x2];
  extern const x8 table1d324c[0x2c];
  extern const x16 table1d3278[0x10];
  extern const x8 table1d3298[0x10];
  extern s_munknown table1d32d8[0x12];
  extern const x8 table1d3efd[0xec];
  extern s_bbw table1d3ed5[0xa];
  extern /*const*/ s_wwwb table1d3d23[0x3e];
  extern /*const*/ x8 table1d3cd0[0x53];
  extern /*const*/ s_bbw table1d3ba0[0x4c];
  extern /*const*/ s_2w v1d39bc[0x79];
  extern /*const*/ s_www v1d338c[0x108];
  extern t_text v1d5c31[0x9];
  extern const char v1d6a4f[0x5];
  extern const x8 table1d6afe[0x17];
  extern const s_4bytearray table1d6a74[0x17];
  extern const x8 table1d6ad0[0x17][2];
  extern const x8 table1d6b43[0x17];
  extern const x8 table1d6b5a[0x17];
  extern const x8 table1d6b2c[0x17];
  extern const x8 table1d6b15[0x17];
  extern const x8 table1d6a54[4][4];
  extern const x8 table1d6a64[4][4];
  extern const x8 table1d6b71[0x5];
  extern const x8 table1d6efd[0xe];
  extern const x8 table1d6ee1[0xe][0x2];
  extern const x8 table1d6ed3[0xe];
  extern const s_bb table1d6eb3[0x10];
  extern const x8 table1d6ea8[0x3];
  extern const x8 table1d6eab[0x4];
  extern const x8 table1d6eaf[0x4];
  extern const s_bb table1d6e68[0x4][0x8];
  extern const x8 table1d6e51[0x17];
  extern const x8 table1d6797[0x25];
  extern x8 v1d67be[4];
  extern const x8 table1d6e41[0x10];
  extern const x8 table1d6e35[0xc];
  extern const x16 table1d6c70[0x10];
  extern const x8 table1d6c90[0x10];
  extern const x8 table1d6ca0[0x10];
  extern const x8 table1d6cb0[0x10];
  extern const x16 table1d6cc0[0x10];
  extern const x8 table1d6c10[0x5];
  extern const x8 table1d6c19[0x5];
  extern const x8 table1d6c1e[0x17];
  extern const x8 table1d6c35[0x17];
  extern const x16 table1d6c4c[0x9];
  extern const x8 table1d6c5e[0x9];
  extern const x8 table1d6c67[0x9];
  extern const x8 table1d6c06[0x5];
  extern const x8 table1d6c0b[0x5];
  extern const s_www table1d6a31[0x5];
  extern const t_text v1d6a08[0x26];
  extern const x8 table1d6e03[0x19][0x2];
  extern const x8 table1d6de3[0x10][0x2];
  extern const x8 table1d6dd3[0x10];
  extern const x8 table1d69aa[0x6];
  extern const x8 table1d69a2[0x6];
  extern const x16 table1d6998[0x5];
  extern const x8 table1d6984[0x14];
  extern const x8 table1d6980[0x4];
  extern const x8 table1d69b0[0x20];
  extern const x8 table1d69d0[0x4];
  extern const x8 table1d6b76[0x84];
  extern const x16 table1d6d3c[0x6];
  extern const x8 table1d6d48[0x6];
  extern const x8 table1d6d4e[0x6];
  extern const x8 table1d6d54[0x6];
  extern const s_4bytearray table1d6ce0[0x17];
  extern const x8 table1d6d5a[0x4][0x5];
  extern const s_bb table1d6d6e[0x4];
  extern const x8 table1d6d76[0x2][0x4];
  extern const x8 table1d6d7e[0x2][0x4];
  extern const x8 table1d6d86[0x2];
  extern const x8 table1d6d88[0x19];
  extern const x8 table1d6da1[0x19];
  extern const x8 table1d6dba[0x19];
  extern const x8 table1d67d9[0x7];
  extern const x8 table1d67e0[0x5][0x6];
  extern const x8 table1d67fe[0x4];
  extern /*const*/ s_802 table1d6802[0x22];
  extern const t_text* table1d6912[0x12];
  extern /*const*/ c_event table1e04e0[0x3];
  extern const x16 table1d292c[0x20];
  extern /*const*/ c_aispec table1d296c[0x3f];
  extern const t_text v1d111f[];
  extern const x8 table1d70b4[0x11];

#endif
