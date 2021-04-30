#ifndef C_GFX_PIXEL_H
  #define C_GFX_PIXEL_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  enum e_color // index for PAL16
  {
    E_COL00,
    E_COL01,
    E_COL02,
    E_COL03,
    E_COL04,
    E_COL05,
    E_COL06,
    E_COL07,
    E_COL08,
    E_COL09,
    E_COL10,
    E_COL11,
    E_COL12,
    E_COL13,
    E_COL14,
    E_COL15,
    E_COLX90 = 0x90 // used in c_gfx_decode only, TODO check that
  };

  //##############
  // pixel-types
  //##############

  class t_palette;

  typedef ui8 t_resolution;
  // for t_resolution
  #define BPP_4  (4)
  #define BPP_8  (8)

  class c_pixel
  {
    protected:
      ui8 p;

      friend bool operator !=(c_pixel const& a, c_pixel const& b);
      friend bool operator ==(c_pixel const& a, e_color const& b);
      friend bool operator !=(c_pixel const& a, e_color const& b);
      friend c_pixel ui8_to_pixel(ui8 i);
      friend ui8 pixel_to_ui8(c_pixel p);

      friend void ui8_to_palettecolor(t_palette* pal, ui8 c);

    public:
      c_pixel(void) : p(0) {}
      c_pixel(e_color c) { p = c; }

      ui16 mkidx(void) const { return (ui16)p; }
      bool is(ui8 n) const { return p == n; }
  };

// Note: I have shortened the combination ltor().getr() to ltor()

  extern bool operator !=(c_pixel const& a, c_pixel const& b);

// Note: if a t_pixel16 gets compared with an e_color, it has to be
// the right pixel only with a 0-pixel in the left.
// TODO: check for E_COLX90 - is that a left pixel 9 ?
  extern bool operator ==(c_pixel const& a, e_color const& b);
  extern bool operator !=(c_pixel const& a, e_color const& b);

// helper to locate assignments easily
  c_pixel ui8_to_pixel(ui8 i);
  ui8 pixel_to_ui8(c_pixel p);

  class c_pixel16 : public c_pixel
  {
      friend c_pixel16 build_pixels16(c_pixel16 left, c_pixel16 right);
      friend c_pixel16 build_pixels_masked16(c_pixel16 left, c_pixel16 right, ui8 mask);

    public:
      c_pixel16 operator &(ui8 b) { c_pixel16 pix; pix.p = p & b; return pix; }
      c_pixel16 operator |(ui8 b) { c_pixel16 pix; pix.p = p | b; return pix; }
      c_pixel16& operator &=(ui8 b) { p &= b; return *this; }

      void set(ui8 b) { p = b; } // for the 4to8-stretcher

      // the left pixel gets nibble-shifted to the right, the left one gets color 0 then.
      c_pixel16 ltor(void) const { c_pixel16 pix; pix.p = p >> 4; return pix; }

      // the right pixel gets nibble-shifted to the left, the right one gets color 0 then.
      c_pixel16 rtol(void) const { c_pixel16 pix; pix.p = p << 4; return pix; }

      // getting the pure left pixel
      c_pixel16 getl(void) const { c_pixel16 pix; pix.p = p & 0xf0; return pix; }
      // getting the pure right pixel
      c_pixel16 getr(void) const { c_pixel16 pix; pix.p = p & 0xf; return pix; }
  };

  c_pixel16 build_pixels16(c_pixel16 left, c_pixel16 right);
  c_pixel16 build_pixels_masked16(c_pixel16 left, c_pixel16 right, ui8 mask);

  class c_pixel256 : public c_pixel
  {
    public:
      c_pixel256& operator= (const e_color e) { p = e; return *this; }
  };

// Note: there are only two commands now to get another type-information
// out of a c_pixel: mkidx() and pixel_to_ui8
// color_to_pixel and ui8_to_pixel are commands to generate a c_pixel
// by other types. (set is preserved for one routine)

#endif