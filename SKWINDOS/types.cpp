#include "types.h"

#include <stdlib.h> // SPX: for NULL

i32 con(const i32 x) { return x; }
ui32 ucon32(const i32 x) { return x; }	// SPX: added for some compatibility
ui16 ucon16(const i32 x) { return x; }
ui8 ucon8(const i32 x) { return x; }
i8 icon8(const i32 x) { return x; }

s_gfxdatatail* getprevious(t_gfxdata* ptr)
{
	// SPX: in case of null, pointer, also return null instead of doing crazy things ...
	if (ptr == NULL) return NULL;
  return (s_gfxdatatail*)(P8CAST(ptr) - sizeof(s_gfxdatatail));
}

// extend signed
i16 signedword(x8 v)  { i16 n = (i16)v; return n; }
i32 signedlong(bool v) { return (v)?1:0; }
i32 signedlong(x8 v)  { i32 n = (i32)v; /*n <<= 0x18; sar32(n, 0x18);*/ return n; }
i32 signedlong(ui8 v) { i32 n = (i32)v; /*n <<= 0x18; sar32(n, 0x18);*/ return n; }
i32 signedlong(x16 v) { i32 n = (i32)v; /*n <<= 0x10; sar32(n, 0x10);*/ return n; }
i32 signedlong(ui16 v) { i32 n = (i32)v; /*n <<= 0x10; sar32(n, 0x10);*/ return n; }
i32 signedlong(t_text v) { i32 n = (i32)v; return n; }

// extend unsigned
ui16 unsignedword(x8 n)  { ui16 v = (ui16)n; return v & 0xff; }
ui32 unsignedlong(x8 n)  { ui32 v = (ui32)n; return v & 0xff; }
ui32 unsignedlong(ui8 n)  { ui32 v = (ui32)n; return v & 0xff; }
ui32 unsignedlong(x16 n) { ui32 v = (ui32)n; return v & 0xffff; }
ui32 unsignedlong(ui16 n) { ui32 v = (ui32)n; return v & 0xffff; }
ui32 unsignedlong(t_text n) { ui32 v = (ui32)n; return v & 0xffff; }

x16 CUTX16(x32 n) { return n; }
x8 CUTLX8(x32 n) { return n; }
// WARNING NOTE: signedlong(CUTX16(n));
// 0x0000ffff would be 0xffffffff then!
x8 CUTX8(x16 n) { return n; }

void c_wpc::init(void)
{
  w0 = 0;
  p2 = (t_gfxdata*) NULL;
  for (int i=0; i<PAL16; i++) barr6[i]=0;
  b16 = b17 = 0;
  w18 = w1a = 0;
}

void c_rwbb::init(void)
{
  w8 = 0;
  ba = 0;
  bb = 0;
}

void c_bbb::init(void)
{
  b0 = 0;
  b1 = 0;
  b2 = 0;
}

void c_bbbb::init(void)
{
  b0 = 0;
  b1 = 0;
  b2 = 0;
  b3 = 0;
}

void c_hero::init(void)
{
int i=0;
  for ( i=0; i<0x1c; i++) t00[i]=0;
  b1c = 0;
  b1d = 0;
  b1e = 0;
  b1f = 0;
  b20[0] = b20[1] = 0;
  for ( i=0; i<0x8; i++) t22[i]=NULLBYTE;
  for ( i=0; i<0x4; i++) b2a[i]=0;
  w2e = 0;
  w30 = 0;
  u32.w = 0;
  w34 = 0;
  w36 = 0;
  w38 = 0;
  w3a = 0;
  u3c.w = 0;
  w3e = 0;
  w40 = 0;
  for ( i=0; i<0x4; i++) b42[i]=0;
  w46 = 0;
  w48 = 0;
  for ( i=0; i<0xe; i++) b4a[i]=0;
  for ( i=0; i<0x7; i++) b58[i]=0;
  for ( i=0; i<0x14; i++) l5f[i]=0;
  for ( i=0; i<0x14; i++) baf[i]=0;
  for ( i=0; i<0x1e; i++) warrc3[i]=0;
  wff = 0;
  b101 = 0;
  b102 = 0;
  w103 = 0;
  for ( i=0; i<0x2; i++) b105[i]=0;
}

void c_image::init(void)
{
  s_00.init();
  w_18 = 0;
  w_1a = 0;
  w_1c = 0;
  w_1e = 0;
  w_20 = 0;
  w_22 = 0;
  rc_24.init();
  pg_2c = NULL;
  w_30 = 0;
  w_32 = 0;
  w_34 = 0;
  w_36 = 0;
  w_38 = 0;
  for (int i=0; i<PAL256; i++) xLocalPalette[i] = 0;
};

void c_c5rects::init(void)
{
  s_00.init();
  for (int i=0; i<5; i++) rcarr_0c[i].init();
}

void c_hex18::init(void)
{
  pg_00 = NULL;
  w_04 = 0;
  w_06 = 0;
  b_08 = 0;
  b_09 = 0;
  b_0a = 0;
  b_0b = 0;
  w_0c = 0;
  w_0e = 0;
  w_10 = 0;
  width = 0;	// w_12
  height = 0;	// w_14
  w_16 = 0;
}

void c_tim::init(void)
{
  un.l_00 = 0;
  b_04 = 0;
  b_05 = 0;
  u.w_06 = 0;
  v.l_08 = 0;
}

void c_hexc::init(void)
{
  w_00 = 0;
  rc_02.init();
  w_0a = 0;
}

void c_len10x::init(void)
{
  s_00.init();
  l_0c = 0;
}

void c_sizex16::init(void)
{
  s_00.b_00 = 0;
  s_00.b_01 = 0;
  s_00.u_02.w = 0;
  s_00.u_04.w = 0;
  s_00.b_06 = 0;
  s_00.b_07 = 0;
  u_08.w = 0;
  u_0a.w = 0;
  w_0c = 0;
  for (int i=0; i<0x4; i++) b_0e[i] = 0;
  ps_12 = NULL;
}

void c_bbbbwwwp::init(void)
{
  b0 = 0;
  b1 = 0;
  b2 = 0;
  b3 = 0;
  w4 = 0;
  w6 = 0;
  w8 = 0;
  phe_a = NULL;
}

void c_350::init(void)
{
	int i = 0;
  v1e054c = 0;
  v1e054e = NULL;
  v1e0552 = NULL;
  v1e0556 = NULL;
  v1e055a = NULL;
  v1e055e = NULL;
  v1e0562.init();
  v1e056e = 0;
  v1e056f = 0;
  v1e0570 = 0;
  v1e0571 = 0;
  v1e0572 = 0;
  v1e0574 = 0;
  v1e0576 = 0;
  v1e0578 = 0;
  v1e057a = 0;
  v1e057c = 0;
  v1e057e = 0;
  v1e0580 = 0;
  v1e0582 = 0;
  v1e0584 = 0;
  v1e0586 = 0;
  v1e0588 = NULL;
  v1e058c = 0;
  v1e058d = 0;
  for ( i=0; i<0x4; i++)
  {
    v1e058e[i].ps_00 = NULL;
    for (int j=0; j<0x7; j++)
      for (int k=0; k<0x4; k++)
        v1e058e[i].sarr_04[j].barr_00[k] = 0;
  }
  for ( i=0; i<0x30; i++)
    v1e060e[i].w = 0;
  for ( i=0; i<0x5; i++) v1e066e[i] = 0;
  v1e0673 = 0;
  v1e0674 = 0;
  v1e0675 = 0;
  for ( i=0; i<0x2; i++) v1e0676[i] = 0;
  for ( i=0; i<0x10; i++) v1e0678[i].init();
  v1e07d8.init();
  v1e07e6 = NULL;
  v1e07ea = 0;
  v1e07eb = 0;
  v1e07ec = 0;
  v1e07ed = 0;
  for ( i=0; i<0x2a; i++) v1e07ee[i] = NULL;
  v1e0896 = 0;
  v1e0898 = NULL;
}

void c_hex28::init(void)
{
  for (int i=0; i<0x24; i++) t_00[i] = 0;
  l_24 = 0;
}
