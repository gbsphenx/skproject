#ifndef C_GFX_BMP_H
  #define C_GFX_BMP_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  #ifndef DM2GLOBAL_H
    #include "dm2global.h"
  #endif

  #ifndef C_GFX_PIXEL_H
    #include "c_gfx_pixel.h"
  #endif

  // this type is for pointers only that are basically c_pixel,
  // but have the s_dm2bmpheader before.
  // so neg. offsets possible: (-2:word, -4:word, -6:t_resolution)
  // in fact for t_resolution the first byte is used, the second is unused
  // but has to stay there as fillbyte.

  typedef c_pixel t_bmp;

// from t_bmp
  #define PIXELCAST(a) CHGCAST(c_pixel, a) // from c_pixel to t_bmp
  #define BMPCAST(a) CHGCAST(t_bmp, a) // from t_bmp to c_pixel

  // use s_dm2bmpheader* getbmpheader(t_bmp*) to access.
  struct s_dm2bmpheader
  {
    t_resolution res;
    i8 unused;
    i16 width;
    i16 height;
  };

  s_dm2bmpheader* getbmpheader(t_bmp* bmp);

  #define DM2_COPY_BMPHEADER(a, b) DM2_COPY_MEMORY(DOWNCAST(s_dm2bmpheader, a), DOWNCAST(s_dm2bmpheader, b), sizeof(s_dm2bmpheader))

  struct s_screen256bmp // was s_transferbitmap
  {
    s_dm2bmpheader header;
    c_pixel256 pixel[ORIG_SWIDTH * ORIG_SHEIGHT];
  };

  extern s_screen256bmp DRV_screen256;

  void init_bitmaps(void);

  i32 DM2_CALC_IMAGE_BYTE_LENGTH(t_bmp* bmp);

#endif
