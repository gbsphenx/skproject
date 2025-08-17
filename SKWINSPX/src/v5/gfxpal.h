#ifndef C_GFX_PAL_H
  #define C_GFX_PAL_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  #ifndef C_GFX_PIXEL_H
    #include <gfxpixel.h>
  #endif

  #define PAL16  (16)
  #define PAL256 (256)

  class t_colconv // a color-converter-table
  {
    public:
      ui8 p;
  };

  // Note: sometimes a t_palette is still used as a palette-converter

  class t_palette
  {
    protected:
      c_pixel256 c;

      friend void color_to_palettecolor(t_palette* pal, e_color c);
      friend void ui8_to_palettecolor(t_palette* pal, ui8 c);
      friend ui8 palettecolor_to_ui8(t_palette pal);
      friend c_pixel256 palettecolor_to_pixel(t_palette pal);

    public:
      ui16 mkidx(void) const { return c.mkidx(); }
      void conv(const t_colconv* colconv);
  };

// functions to easily find assignments:
  void color_to_palettecolor(t_palette* pal, e_color c);
  void ui8_to_palettecolor(t_palette* pal, ui8 c);
  ui8 palettecolor_to_ui8(t_palette pal);
  c_pixel256 palettecolor_to_pixel(t_palette pal);

  // TODO there are some more DM2_COPY_MEMORY for palettes
  #define DM2_COPY_SMALL_PALETTE(dest, src) DM2_COPY_MEMORY(DOWNCAST(t_palette, dest), DOWNCAST(t_palette, src), PAL16)
  #define DM2_COPY_LARGE_PALETTE(dest, src) DM2_COPY_MEMORY(DOWNCAST(t_palette, dest), DOWNCAST(t_palette, src), PAL256)

  class c_palettedata
  {
    public:
      t_palette* palette; // usually used as an array, index is enum e_color then.
      t_palette* pal16to256ptr; // was pal16to256ptr
      t_palette small_palette[PAL16]; // was v1e12dc
      t_palette* glbl_pal1; // was v1d66f4
      t_palette* glbl_pal2; // was v1e0958

      bool DRVb_immediate_colors; // ADR511B(0xfc)
      // if true, driver_convertalphapalette will call driver_setcolors as well (colors available then)
      // driver_setpalette will set this byte by the given argument.

      void init(void);
  };

  extern c_palettedata paldat;

  void DM2_SELECT_PALETTE_SET(i16 set);
  void DM2_UPDATE_BLIT_PALETTE(t_palette* palette);

  void DM2_CONVERT_DRIVERPALETTE(ui8* pb);
  t_palette* DM2_xlat_palette(t_palette* palette, const t_colconv* convtable, i16& colors);

#endif
