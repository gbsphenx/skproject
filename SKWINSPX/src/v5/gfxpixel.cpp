#include <gfxpixel.h>

bool operator !=(c_pixel const& a, c_pixel const& b) { return a.p != b.p; }

bool operator ==(c_pixel const& a, e_color const& b) { return a.p == b; }
bool operator !=(c_pixel const& a, e_color const& b) { return a.p != b; }

c_pixel ui8_to_pixel(ui8 i) { c_pixel pix; pix.p = i; return pix; }
ui8 pixel_to_ui8(c_pixel p) { return p.p; }

// 'left' is a pixelpair with the right one blackend
// 'right' is a pixelpair with the left one blackend
c_pixel16 build_pixels16(c_pixel16 left, c_pixel16 right)
{
  c_pixel16 pix;
  pix.p = left.p | right.p;
  return pix;
}

c_pixel16 build_pixels_masked16(c_pixel16 left, c_pixel16 right, ui8 mask)
{
  c_pixel16 pix;
  pix.p = (left.p & ~mask) | (right.p & mask);
  return pix;
}
