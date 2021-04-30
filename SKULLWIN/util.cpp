#include "util.h"
#include "c_random.h"
#include "dm2data.h" // TODO: unknown tables here!
#include "c_allegro.h"

// NEEDS: TODO
void wait_for_vsync(void); // -> addon.h TODO

i16 DM2_ABS(i16 wn)
{
  return (wn >= 0) ? wn : -wn;
}

i16 DM2_BETWEEN_VALUE(i16 min, i16 max, i16 wval)
{
  if (wval < min)
    return min;
  if (wval > max)
    return max;
  return wval;
}

i16 DM2_CALC_SQUARE_DISTANCE(i16 a, i16 d, i16 b, i16 c)
{
  return DM2_ABS(a - b) + DM2_ABS(d - c);
}

// TODO: return of byte possible, range 0-3
// use enumeration?
i16 DM2_CALC_VECTOR_DIR(i16 a, i16 d, i16 b, i16 c)
{
  i16 rg5 = a - b;
  i16 rg2 = DM2_ABS(rg5);
  i16 rg4 = d - c;
  i16 rg3 = DM2_ABS(rg4);
  if (rg2 == rg3)
  {
    if (CUTX16((DM2_RANDBIT() ? 1 : 0)) == wcon(0x0))
      rg3++;
    else
      rg2++;
  }
  if (rg2 >= rg3)
    return (rg5 <= 0) ? 1 : 3;
  return (rg4 <= 0) ? 2 : 0;
}

void DM2_CALC_VECTOR_W_DIR(i16 a, i16 d, i16 b, i16* cpw, i16* argpw0)
{
  i16 idx = a;
  *cpw += d * table1d27fc[idx];
  *argpw0 += d * table1d2804[idx];
  idx++;
  idx &= 0x3;
  *cpw += b * table1d27fc[idx];
  *argpw0 += b * table1d2804[idx];
}

// was SKW_1c9a_0551
i32 DM2_COMPUTE_POWER_4_WITHIN(i16 a, i16 d)
{
  i32 result = 1;

  for (i16 n=0; n < 32; n++)
  {
    if ((result & a) != 0)
    {
      if (--d == 0)
        return result;
    }
    result <<= 1;
  }
  return result;
}

// was SKW_FILL_U16
// parameter 3 deleted (was stepwidth, always 2 in bytes)
void DM2_FILL_I16TABLE(i16* table, i16 value, ui16 entries)
{
  for (ui16 uw = 0; uw < entries; uw++)
    table[uw] = value;
}

// was SKW_0cd5_0176 TODO tell SPX new name
i32 DM2_ATIMESB_RSHIFTC(i16 a, i8 c, i16 b)
{
  return unsignedlong(a) * unsignedlong(b) >> c;
}

void DM2_WAIT_SCREEN_REFRESH(void)
{
//  i32 tmp1;
#if 0
$M_393D:
$  in8(RG1Blo, lcon(0x3da)); // Video status bits:
$  // 0: retrace.  1=display is in vert or horiz retrace.
$  // 1: 1=light pen is triggered// 0=armed
$  // 2: 1=light pen switch is open// 0=closed
$  // 3: 1=vertical sync pulse is occurring.
$  if (jnz_and8(RG1Blo, lcon(0x8))) goto M_393D;
$M_3942:
$  in8(RG1Blo, lcon(0x3da)); // Video status bits:
$  // 0: retrace.  1=display is in vert or horiz retrace.
$  // 1: 1=light pen is triggered// 0=armed
$  // 2: 1=light pen switch is open// 0=closed
$  // 3: 1=vertical sync pulse is occurring.
$  if (jz_and8(RG1Blo, lcon(0x8))) goto M_3942;
#else
  // TODO ALLEGRO_PAUSE(1000/60);
#endif
  wait_for_vsync();
}

void DM2_SLEEP_SEVERAL_TIME(i16 vsyncs)
{
  while (--vsyncs != -1)
    DM2_WAIT_SCREEN_REFRESH();
}

i16 DM2_MIN(i16 wa, i16 wb)
{
  if (wa < wb)
    return wa;
  return wb;
}

i16 DM2_MAX(i16 wa, i16 wb)
{
  if (wa > wb)
    return wa;
  return wb;
}

// id: 0xFF4
void DM2_COPY_MEMORY(unk* dest, unk* src, i32 size)
{
  if (src != NULL && dest != NULL && size != 0)
    memcpy(dest, src, size);
}

// was R_2C45C - moved parameter 4 to position 2
i32 DM2_4BYTES_TO_LONG(i8 argb0, i8 argb1, i8 argb2, i8 argb3)
{
  return argb3 << 24 | argb2 << 16 | argb1 << 8 | argb0;
}

i16 DM2_ROTATE_5x5_POS(i16 wa, t_rotate rotate)
{
  i16 wtmp;

  i16 mod = wa % 5 - 2;
  i16 div = wa / 5 - 2;

  switch (rotate)
  {
    case 0:
    break;

    case 1:
      wtmp = mod;
      mod = div;
      div = -wtmp;
    break;

    case 2:
      mod = -mod;
      div = -div;
    break;

    case 3:
      wtmp = mod;
      mod = -div;
      div = wtmp;
    break;
  }

  return 5 * (div + 2) + mod + 2;
}

// remaining emu-function, does not return a bool but a byte
i8 sgn8(i8 bn)
{
  return (bn & bcon(0x80)) != bcon(0x0) ? bcon(0xff) : bcon(0x0);
}

// remaining emu-function
i32 jul_shl32(i32 lm, i8 bn)
{
  bool jul;

  lm >>= bcon(0x1f);
  int i = bn & bcon(0x1f);
  while (i--)
  {
    jul = (lm & 0x80000000) != 0;
    lm <<= 1;
    lm &= 0xfffffffe;
  }
  if (jul)
    lm++;
  return lm;
}
