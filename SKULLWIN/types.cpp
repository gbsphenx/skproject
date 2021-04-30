#include "types.h"

i8 bcon(i8 bn) { return bn; }
i16 wcon(i16 wn) { return wn; }
i32 lcon(i32 ln) { return ln; }
ui8 ubcon(i8 bn) { return bn; }
ui16 uwcon(i16 wn) { return wn; }
ui32 ulcon(i32 ln) { return ln; }

// ==========================================================

i16 signedword(i8 bv)  { i16 wn = mkw(bv); return wn; }
i32 signedlong(i16 wv) { i32 ln = mkl(wv); /*ln <<= 0x10; sar32(ln, 0x10);*/ return ln; }

ui16 unsignedword(i8 bn) { ui16 uv = mkuw(bn); return uv & uwcon(0xff); }
ui32 unsignedlong(i8 bn) { ui32 ulv = mkul(bn); return ulv & ulcon(0xff); }
ui32 unsignedlong(ui8 ubn) { ui32 ulv = mkul(ubn); return ulv & ulcon(0xff); }
ui32 unsignedlong(i16 wn) { ui32 ulv = mkul(wn); return ulv & ulcon(0xffff); }
ui32 unsignedlong(ui16 un) { ui32 ulv = mkul(un); return ulv & ulcon(0xffff); }
ui32 unsignedlong(t_text ubn) { ui32 ulv = mkul(ubn); return ulv & ulcon(0xffff); }

// ==========================================================

i16 CUTX16(i32 ln) { return ln; }
i8 CUTLX8(i32 ln) { return ln; }
// WARNING NOTE: signedlong(CUTX16(n));
// 0x0000ffff would be 0xffffffff then!
i8 CUTX8(i16 wn) { return wn; }
ui8 CUTUX8(i16 wn) { return wn; }

// ==========================================================

i8 mkb(i32 ln) { return ln; }
ui8 mkub(i32 ln) { return ln; }
i16 mkw(i32 ln) { return ln; }
ui16 mkuw(i32 ln) { return ln; }
i32 mkl(i32 ln) { return ln; }
ui32 mkul(i32 ln) { return ln; }

// ==========================================================

const SPX_Ax3 SPX_Ax3::Invalid(SPX_Ax3::Frm(0xffff));

void SPX_Ax3::SetX(ui16 val)
{
	val &= 0x1f;
	w0 &= ~0x1f; // 0xffe0
	w0 |= val;
}

void SPX_Ax3::SetY(ui16 val)
{
	val &= 0x1f;
	w0 &= ~(0x1f << 5); // 0xfc1f
	w0 |= val << 5;
}

void SPX_Ax3::SetMap(ui16 val)
{
	val &= 0x3f;
	w0 &= ~(0x3f << 10); // 0x03ff
	w0 |= val << 10;
}
