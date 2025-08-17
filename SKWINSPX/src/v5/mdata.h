#ifndef MDATA_H
  #define MDATA_H

  struct s_seven
  {
    unsigned char uc0[7];
  };

  struct s_fourteen
  {
    unsigned char uc0[0xe];
  };

  struct s_fourb
  {
    union
    {
      unsigned char uc[4];
      unsigned short uwarr_00[2];
      unsigned long ul_00;
    };
  };

#ifdef __cplusplus
extern "C" {
#endif

  extern struct s_seven* table1d5f82[];
  extern struct s_fourb table1d607e[0x2f];
  extern unsigned char table1d613a[];
  extern struct s_wpm* table1d6190[0x2f];
  void* getadr_r1d41d6(void);
  void* getadr_u1d4396(void);
  void* getadr_u1d439d(void);

#ifdef __cplusplus
}
#endif

#endif
