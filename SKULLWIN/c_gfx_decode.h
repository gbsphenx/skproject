#ifndef C_GFX_DECODE_H
  #define C_GFX_DECODE_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  class c_imgdecode
  {
    protected:
      i16 img3decode;
      c_pixel16* decodeptr1;
      i16 decodew0;
      i16 decodew1;
      bool decodew2;
      i16 decodew3;
      i16 decodew4;
      i16 decodew5;
      ui8* decodeptr2;
      ui8* decodeptr3;
      i16 decodew6;
      bool decode7;
      ui8 decodemask[9];
      i16* img9mem1; // was dm2_dballochandler.v1e149c;
      ui8* img9mem2; // was dm2_dballochandler.v1e14a0;

    protected:
      void func_44c8_1202(ui16 offset, c_pixel16 dpix);
      void spill_img3_pixels(ui16 dofs, ui16 sofs, i16 num);
      c_pixel16 read_img3_nibble(void);
      ui16 read_img3_duration(void);
      void transparent_img3_pixels(ui16 ofs, i16 num);

      i16 dec9_1sub(void);
      void dec9_1(ui8* data, c_pixel* gfx2, i16 num);
      void dec9_2(ui8* data, c_pixel* gfx2, i32 num);
      void dec9_3(ui8* data, c_pixel* gfx2, i32 num);

    public:
      void init(void);
      void alloc(void);

      void decode_img3_underlay(i16* src, c_pixel* dest);
      void decode_img3_overlay(c_pixel16* dptr, i16* src, c_pixel* dest);

      void decode_img9(ui8* data, c_pixel* gfx, i32 num);
  };

  extern c_imgdecode imgdecoder;

#endif
