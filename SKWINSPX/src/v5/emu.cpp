#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <dm2globl.h>
#include <emu.h>

// ---------- read/write memory --------------------

static i8 bpeek(const i8* bpn) { return *bpn; }
static i16 wpeek(const i8* bpn) { return *UPCAST(i16, bpn); }
static i32 lpeek(const i8* bpn) { return *UPCAST(i32, bpn); }

static void bpoke(i8* bpm, i8 bn) { *bpm = bn; }
static void wpoke(i8* bpm, i16 wn) { *UPCAST(i16, bpm) = wn; }
static void lpoke(i8* bpm, i32 ln) { *UPCAST(i32, bpm) = ln; }

// ----------- indirect access ---------------------

i8 byte_at(const i8* bptr, i32 offset) { return *(bptr + offset); }
ui8 ubyte_at(const i8* bptr, i32 offset) { return *UPCAST(ui8, bptr + offset); }
i16 word_at(const i8* bptr, i32 offset) { return *UPCAST(i16, bptr + offset); }
ui16 uword_at(const i8* bptr, i32 offset) { return *UPCAST(ui16, bptr + offset); }
i32 long_at(const i8* bptr, i32 offset) { return *UPCAST(i32, bptr + offset); }

i8* pointer_at(const i8* bptr)
{
  return DOWNCAST(i32, *UPCAST(i32, bptr));
}

// ----------------- operations -------------------------

void mov8(i8* bpm, i8 bn) { bpoke(bpm, bn); }
void mov16(i8* bpm, i16 wn) { wpoke(bpm, wn); }
void mov32(i8* bpm, i32 ln) { lpoke(bpm, ln); }
#ifdef __LINUX__
void movp(i8* bpm, i8* bpn) { bpm = (i8*)bpn; }
#else
void movp(i8* bpm, i8* bpn) { lpoke(bpm, pointer_to_long(bpn)); }
#endif


void put8(i8& bm, i8 bn) { bm = bn; }
void put16(i16& wm, i16 wn) { wm = wn; }

void inc8(i8* bpn) { i8 bv = bpeek(bpn); bv++; bpoke(bpn, bv); }
void inc16(i8* bpn) { i16 wv = wpeek(bpn); wv++; wpoke(bpn, wv); }

void dec8(i8* bpn) { i8 bv = bpeek(bpn); bv--; bpoke(bpn, bv); }
void dec16(i8* bpn) { i16 wv = wpeek(bpn); wv--; wpoke(bpn, wv); }

void add8(i8* bpm, i8 bn) { bpoke(bpm, bpeek(bpm) + bn); }
void add16(i8* bpm, i16 wn) { wpoke(bpm, wpeek(bpm) + wn); }
void add32(i8* bpm, i32 ln) { lpoke(bpm, lpeek(bpm) + ln); }

void sub8(i8* bpm, i8 bn) { bpoke(bpm, bpeek(bpm) - bn); }
void sub16(i8* bpm, i16 wn) { wpoke(bpm, wpeek(bpm) - wn); }

void and8(i8* bpm, i8 bn) { bpoke(bpm, bpeek(bpm) & bn); }
void and16(i8* bpm, i16 wn) { wpoke(bpm, wpeek(bpm) & wn); }

void or8(i8* bpm, i8 bn) { bpoke(bpm, bpeek(bpm) | bn); }
void or16(i8* bpm, i16 wn) { wpoke(bpm, wpeek(bpm) | wn); }
void or32(i8* bpm, i32 ln) { lpoke(bpm, lpeek(bpm) | ln); }

bool jz_test8(const i8* bpm, i8 bn) { return (bpeek(bpm) & bn) == 0; }
bool jz_test16(const i8* bpm, i16 wn) { return (wpeek(bpm) & wn) == 0; }

bool jnz_test8(const i8* bpm, i8 bn) { return (bpeek(bpm) & bn) != 0; }
bool jnz_test16(const i8* bpm, i16 wn) { return (wpeek(bpm) & wn) != 0; }

// shift
// --------------
// note: shift logic/arithmetic should work with unsigned/signed automatically without
// masking - i'll do it anyways here...

i8 static _ar8(i8 bm, i8 bn)
{
  i8 bk = bm; int i = bn & 0x1f; while (i--) { i8 msb = bk & 0x80; bk >>= 1; bk |= msb; }
  return bk;
}

i16 static _lr16(i16 wm, i8 bn)
{
  int i = bn & 0x1f; while (i--) { wm >>= 1; wm &= 0x7fff; }
  return wm;
}

void shr16(i8* bpm, i8 bn) { wpoke(bpm, _lr16(wpeek(bpm), bn)); }

void sar8(i8* bpm, i8 bn) { bpoke(bpm, _ar8(bpeek(bpm), bn)); }

ui16 lrshift6e(ui16 n)
{
  n >>= bcon(0x8); // <<6 >>0xe
  return n;
}

