#include "xtypes.h"

void c_sizex16::init(void)
{
  s23_00.b_00 = 0;
  s23_00.b_01 = 0;
  s23_00.w_02 = 0;
  s23_00.b_04 = 0;
  s23_00.b_05 = 0;
  s23_00.b_06 = 0;
  s23_00.b_07 = 0;
  w_08 = 0;
  for (i16 i = 0; i < 0xc; i++) barr_0a[i] = 0;
}

// memberfunctions

i8 c_ql32::peek8(void) { return lv & lcon(0xff); }
i16 c_ql32::peek16(void) { return lv & lcon(0xffff); }
void c_ql32::poke16(i16 wn) { lv = mkl(wn) & lcon(0xffff); }
i32 c_ql32::peek32(void) { return lv; }
void c_ql32::poke32(i32 ln) { lv = ln; }

i8 c_o32::peek8(void) { return lv & lcon(0xff); }
i16 c_o32::peek16(void) { return lv & lcon(0xffff); }
void c_o32::poke16(i16 wn) { lv = mkl(wn) & lcon(0xffff); }
void c_o32::inc16(void) { i16 wtmp = lv; lv = mkl(wtmp + 1) & lcon(0xffff); }
void c_o32::dec16(void) { i16 wtmp = lv; lv = mkl(wtmp - 1) & lcon(0xffff); }
void c_o32::sub16(i16 wn) { i16 wtmp = lv; lv = mkl(wtmp - wn) & lcon(0xffff); }
void c_o32::add16(i16 wn) { i16 wtmp = lv; lv = mkl(wtmp + wn) & lcon(0xffff); }
void c_o32::shl16(ui8 ubn) { i16 wtmp = lv; wtmp <<= ubn; lv = mkl(wtmp) & lcon(0xffff); }
void c_o32::sar16(ui8 ubn) { ui16 utmp = lv; utmp >>= ubn; lv = mkl(utmp) & lcon(0xffff); }
i8 c_o32::getbyte1(void) { return lv >> 8; }
void c_o32::and16(i16 wn) { lv &= lcon(0xffff0000) | mkl(wn); }
void c_o32::or16(i16 wn) { lv |= mkl(wn) & lcon(0xffff); }
void c_o32::xor16(i16 wn) { lv ^= mkl(wn) & lcon(0xffff); }

void c_ol32::inc16(void) { i16 wtmp = lv; lv = mkl(wtmp + 1) & lcon(0xffff); }
void c_ol32::dec16(void) { i16 wtmp = lv; lv = mkl(wtmp - 1) & lcon(0xffff); }
void c_ol32::sub16(i16 wn) { i16 wtmp = lv; lv = mkl(wtmp - wn) & lcon(0xffff); }
void c_ol32::add16(i16 wn) { i16 wtmp = lv; lv = mkl(wtmp) + wn & lcon(0xffff); }
void c_ol32::inc32(void) { lv++; }
void c_ol32::shl16(ui8 ubn) { i16 wtmp = lv; wtmp <<= ubn; lv = mkl(wtmp) & lcon(0xffff); }
void c_ol32::shr16(ui8 ubn) { ui16 utmp = lv; utmp >>= ubn; lv = mkl(utmp) & lcon(0xffff); }
void c_ol32::dec32(void) { lv--; }
void c_ol32::add32(i32 ln) { lv += ln; }
void c_ol32::sub32(i32 ln) { lv -= ln; }
i8 c_ol32::getbyte1(void) { return lv >> 8; }
i8 c_ol32::getbyte3(void) { return lv >> 24; }
void c_ol32::and16(i16 wn) { lv &= lcon(0xffff0000) | mkl(wn); }
void c_ol32::or16(i16 wn) { lv |= mkl(wn) & lcon(0xffff); }
void c_ol32::or32(i32 ln) { lv |= ln; }

i32 s_bbw::getlong(void) const
{
  return (w_02 << 16) | (b_01 << 8) | b_00;
}
