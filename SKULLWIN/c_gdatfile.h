#ifndef C_GDATFILE_H
  #define C_GDATFILE_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  struct s_xlink // size 0xe
  {
    s_xlink* s87p_00;
    t_dbidx dbidx; // @04
    i16 w_06;
    i8 b_08;
    i8 b_09;
    i16 w_0a;
    i16 w_0c;
  };

  class c_gdatfile
  {
    public:
      // old names did begin with gdat
      i16 fileopencounter;
      i16 filehandle;
      i16 xfilehandle;
      i32 filesize;
      i16 versionlo;
      ui16 entries;
      bool filetype1;
      bool filetype2;
      t_text* filename1;
      t_text* filename2;
      t_text* filename3;
      t_text* filename4;
      t_text* filename5;
      t_text* filename6;
      t_text* filename7;

      void init(void);
  };

struct s_gdat // size: 0x20
{
  i32 l_00; // used
  s_hex6 s19_04;
  s_hex6 s19_0a;
  s_bbw* u31p_10;
  i8 b_14; // used
  i8 b_15;
  i8 b_16; // used
  i8 b_17; // used
  i16 w_18; // used
  i16 w_1a; // used
  i16 w_1c; // used
  i16 w_1e;
};

  extern c_gdatfile gdat;

  void DM2_LOAD_GDAT_RAW_DATA(t_dbidx dbidx, unk* xedxp);
  void DM2_READ_GRAPHICS_STRUCTURE(void);
  c_pixel* DM2_EXTRACT_GDAT_IMAGE(t_dbidx dbidx, i32 edxl);
  void DM2_LOAD_DYN4(unk* xeaxp, i32 edxl);
  void DM2_FREE_PICT_ENTRY(c_pixel* gfx);
  void DM2_LOAD_GDAT_ENTRY_DATA_TO(i8 eaxb, i8 edxb, i8 ebxb, i8 ecxb, unk* xargp0);
  t_bmp* DM2_ALLOC_PICT_BUFF(i16 eaxw, i16 edxw, i16 ebxw, i8 ecxb);
  void DM2_FREE_PICT_BUFF(t_bmp* bmp);
  t_bmp* DM2_ALLOC_NEW_BMP(t_dbidx dbidx, i16 width, i16 height, t_resolution res);
  void DM2_GRAPHICS_DATA_OPEN(void);
  void DM2_GRAPHICS_DATA_CLOSE(void);
  void DM2_LOAD_GDAT_INTERFACE_00_02(void);
  i32 DM2_LOAD_GDAT_INTERFACE_00_0A(void);
  i32 DM2_3D93B(i32 eaxl, i32 edxl, i32 ebxl, i16* ecxpw, i16* argpw0);

#endif
