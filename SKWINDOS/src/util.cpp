#include "util.h"
#include "dm2data.h"
#include "c_allegro.h"

void wait_for_vsync(void); // -> addon.h TODO

x16 DM2_ABS(x16 n)
{
  if (n >= con(0x0))
    return n;
  return -n;
}

x16 DM2_BETWEEN_VALUE(x16 min, x16 max, x16 val)
{
  if (val < min)
    return min;
  if (val > max)
    return max;
  return val;
}

// returns 24 bit ulongrandom
x32 SKW_RAND(void)
{
  ui32 longrg1 = ddata.ulongrandom * con(0xbb40e62d) + con(0xb);
  ddata.ulongrandom = longrg1;
  longrg1 >>= con(0x8);
  return longrg1;
}

x16 SKW_RAND16(x16 eaxw)
{
  if (eaxw != con(0x0))
    return CUTX16(SKW_RAND() % unsignedlong(eaxw));
  return con(0x0);
}

// TODO: return of a bool only possible, range = 0-1
x16 SKW_RAND01(void)
{
  ui32 longrg1 = ddata.ulongrandom * con(0xbb40e62d) + con(0xb);
  ddata.ulongrandom = longrg1;
  longrg1 >>= con(0x8);
  longrg1 &= con(0x1);
  return CUTX16(longrg1);
}

// TODO: return of a byte only possible, range = 0-3
x16 SKW_RAND02(void)
{
  ui32 longrg1 = ddata.ulongrandom * con(0xbb40e62d) + con(0xb);
  ddata.ulongrandom = longrg1;
  longrg1 >>= con(0x8);
  longrg1 &= con(0x3);
  return CUTX16(longrg1);
}

x16 SKW_CALC_SQUARE_DISTANCE(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  x16 wordrg1 = eaxw - ebxw;
  if (wordrg1 < con(0x0))
    wordrg1 = -wordrg1;
  x16 wordrg4 = edxw - ecxw;
  if (wordrg4 < con(0x0))
    wordrg4 = -wordrg4;
  wordrg1 += wordrg4;
  return wordrg1;
}

// TODO: return of byte possible, range 0-3
x16 SKW_CALC_VECTOR_DIR(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  x16 wordrg5 = eaxw - ebxw;
  x16 wordrg2 = DM2_ABS(wordrg5);
  x16 wordrg4 = edxw - ecxw;
  x16 wordrg3 = DM2_ABS(wordrg4);
  if (DM2_ABS(wordrg5) == DM2_ABS(wordrg4))
  {
    if (SKW_RAND01() == con(0x0))
      wordrg3++;
    else
      wordrg2++;
  }
  if (wordrg2 >= wordrg3)
  {
    if (wordrg5 <= con(0x0))
      return con(0x1);
    return con(0x3);
  }
  if (wordrg4 <= con(0x0))
    return con(0x2);
  return con(0x0);
}

t_text* SKW_SK_LTOA10(x32 eaxl, t_text* edxt)
{
  t_text t_00[0x10];

  x32 longrg2 = eaxl;
  if (longrg2 < con(0x0))
    longrg2 = -longrg2;
  t_text* tptrrg3 = &t_00[0xd];
  t_00[0xd] = con(0x0);
  do
  {
    x8 byterg4 = CUTLX8((longrg2 % con(0xa)) + con(0x30));
    *--tptrrg3 = byterg4;
    longrg2 /= con(0xa);
  } while (longrg2 != con(0x0));
  if (eaxl < con(0x0))
    *--tptrrg3 = con(0x2d);
  return SKW_SK_STRCPY(edxt, tptrrg3);
}

// note: return never requested
// function may fill different types of buffers, not only t_text
ui16 SKW_FILL_STR(x8* eaxb, x8 ebxb, x16 ecxw, x16 edxw)
{
  x16 vw_00;
  x8 vb_04;

  vw_00 = edxw;
  vb_04 = ebxb;
  x16 wordrg41 = con(0x0);
  ui16 uwordrg1 = con(0x0);

  while (uwordrg1 < UI16CAST(vw_00))
  {
    eaxb[wordrg41] = vb_04;
    wordrg41 += ecxw;
    uwordrg1++;
  }
  return uwordrg1;
}

// was SKW_FILL_U16
// parameter 3 deleted (was stepwidth, always 2 in bytes)
void DM2_FILL_I16TABLE(x16* table, x16 value, ui16 entries)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("DM2_FILL_I16TABLE");
  for (ui16 n = 0; n < entries; n++)
    table[n] = value;
SPX_DEBUG_POP;
}

x16 SKW_0cd5_0176(x16 eaxw, x16 ebxw, x8 edxb)
{
  return (unsignedlong(eaxw) * unsignedlong(ebxw)) >> edxb;
}

x32 SKW_CALC_IMAGE_BYTE_LENGTH(t_gfxdata* eaxpg)
{
  x16 wordrg4;
  if (eaxpg == NULL) // SPX: avoid null pointer
	  return 0;
  if (getprevious(eaxpg)->b0 != con(0x4))
    wordrg4 = getprevious(eaxpg)->width;
  else
    wordrg4 = ((unsignedlong(getprevious(eaxpg)->width) + 1) & con(0xfffffffe)) >> con(0x1);
  return unsignedlong(getprevious(eaxpg)->height) * unsignedlong(wordrg4);
}

// TODO: limited to word (like it is obviously handled!)
x16 SKW_CALC_STRETCHED_SIZE(x16 eaxw, x16 edxw)
{
  x32 longrg2 = signedlong(eaxw);
  x32 longrg1 = signedlong(edxw);
  longrg2 *= longrg1;
  longrg1 >>= con(0x1);
  longrg1 += longrg2;
  longrg1 >>= con(0x6);
  return CUTX16(longrg1);
}

t_text* SKW_FMT_NUM(x16 eaxw, x16 ebxw, x32 edxl)
{
  x16 vw_00;

  x32 longrg7 = edxl; // TODO: bool?
  vw_00 = ebxw;
  if (edxl != con(0x0))
    SKW_FILL_STR(UPCAST(x8, ddata.fnum), ' ', con(0x1), con(0x4));
  t_text* tptrrg2 = &ddata.fnum[4];
  *tptrrg2 = NULLBYTE;
  if (eaxw != con(0x0))
  {
    for(;;)
    {
      x16 wordrg6 = eaxw;
      if (eaxw == con(0x0))
        break;
      eaxw /= con(0xa);
      *--tptrrg2 = CUTLX8(unsignedlong(wordrg6) + '0' - 10 * unsignedlong(eaxw));
    }
  }
  else
    *--tptrrg2 = '0';

  if (longrg7 == con(0x0))
    return tptrrg2;
  return &ddata.fnum[4 - unsignedlong(vw_00)];
}

void SKW_WAIT_SCREEN_REFRESH(void)
{
//  x32 tmp1;
#if 0
$M_393D:
$  in8(RG1Blo, con(0x3da)); // Video status bits:
$  // 0: retrace.  1=display is in vert or horiz retrace.
$  // 1: 1=light pen is triggered// 0=armed
$  // 2: 1=light pen switch is open// 0=closed
$  // 3: 1=vertical sync pulse is occurring.
$  if (jnz_and8(RG1Blo, con(0x8))) goto M_393D;
$M_3942:
$  in8(RG1Blo, con(0x3da)); // Video status bits:
$  // 0: retrace.  1=display is in vert or horiz retrace.
$  // 1: 1=light pen is triggered// 0=armed
$  // 2: 1=light pen switch is open// 0=closed
$  // 3: 1=vertical sync pulse is occurring.
$  if (jz_and8(RG1Blo, con(0x8))) goto M_3942;
#else
  // TODO ALLEGRO_PAUSE(1000/60);
#endif
  wait_for_vsync();
}

void SKW_SLEEP_SEVERAL_TIME(x16 eaxw)
{
  while (--eaxw != con(0xffffffff))
    SKW_WAIT_SCREEN_REFRESH();
}

x16 DM2_MIN(x16 a, x16 b)
{
  if (a < b)
    return a;
  return b;
}

x16 DM2_MAX(x16 a, x16 b)
{
  if (a > b)
    return a;
  return b;
}

// id: 0xFF4
xxx* SKW_COPY_MEMORY(xxx* eaxp, x32 ebxl, xxx* edxp)
{
  if (eaxp == NULL || edxp == NULL) // TODO: hmmm....
    return eaxp;
  return UPCAST(xxx, memcpy(edxp, eaxp, ebxl));
}

// was R_2C45C - moved parameter 4 to position 2
x32 SKW_4BYTES_TO_LONG(x8 b0, x8 b1, x8 b2, x8 b3)
{
  return ((b3 << con(0x18)) | (b2 << con(0x10)) | (b1 << con(0x8)) | b0); // BUGFIX Phase P1
}

// remaining emu-function
t_gfxdata xlat8(const t_gfxdata* table, t_gfxdata idx)
{
  return table[idx];
}

// remaining emu-function
i32 jul_shl32(i32 m, i8  n)
{
  bool jul;

  m >>= con(0x1f);
  int i = n & 0x1f;
  while (i--)
  {
    jul = (m & 0x80000000)!=0;
    m <<= 1;
    m &= 0xfffffffe;
  }
  if (jul)
    m++;
  return m;
}

// extra help-function to guarantee an unsigned right-shift
// (for some reason, value gets shifted left before too)
ui16 ulrshift(x16 v, x16 l, x16 r)
{
  ui16 u = v;
  u <<= l;
  u >>= r;
  return u;
}

ui16 urshift(x16 v, x16 r)
{
  ui16 u = v;
  u >>= r;
  return u;
}

ui8 ubrshift(x8 v, x16 r)
{
  ui32 u = v & 0xff;
  u >>= r;
  return (ui8)u;
}

ui32 ulrlongshift(x32 v, x16 l, x16 r)
{
  ui32 u = v;
  u <<= l;
  u >>= r;
  return u;
}
