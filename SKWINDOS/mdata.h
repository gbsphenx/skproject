#ifndef MDATA_H
  #define MDATA_H

  #ifndef DM2BT_H
    #include "basetypes.h"
  #endif

  struct s_wpm
  {
    x16 w0;
    struct s_up* p2;
  };

  struct s_seven
  {
    ui8 b0[7];
  };

  struct s_hexe // size 0xe
  {
    x8 b_00;
    x8 b_01;
    union u_bbw u_02;
    x16 w_04;
    x16 w_06; // used
    x8 b_08; // used
    x8 b_09; // used
    x8 b_0a;
    x8 b_0b;
    x8 b_0c;
    x8 b_0d;
  };

  struct s_fourb
  {
    union {
    unsigned char b[4];
    unsigned short w[2];
    unsigned long l;
    } u;
  };

  struct s_anientry
  {
    x8 b0;
    x8 b1;
    struct s_hexe* ps2;
    x8 b6;
  };

#ifdef __cplusplus
extern "C" {
#endif

  struct s_up
  {
    struct s_anientry s0[];
  };

  extern const struct s_seven* table1d5f82[];
  extern const struct s_fourb table1d607e[0x2f];
  extern const unsigned char table1d613a[];
  extern const struct s_wpm* table1d6190[0x2f];
  const struct s_hexe* getadr_r1d41d6(void);
  const struct s_up* getadr_u1d4396(void);
  const struct s_up* getadr_u1d439d(void);

#ifdef __cplusplus
}
#endif

#endif