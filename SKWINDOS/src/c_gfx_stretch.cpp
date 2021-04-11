#include "util.h"
#include "dm2data.h"
#include "c_gfx_blit.h"
#include "c_gfx_stretch.h"

static void SKW_44c8_2143(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  x16 vw_00;
  x16 vw_04;
  t_gfxdata vb_08;

  vw_00 = eaxw;
  x32 longrg6 = lextended(ebxw);
  x16 wordrg1 = ebxw / 2;
  x32 longrg4 = unsignedlong(edxw);
  vw_04 = (ecxw + edxw + 1) >> con(0x1);
  longrg4 >>= con(0x1);
  ui16 wordrg2 = CUTX16(longrg4);
  while (wordrg2 < UI16CAST(vw_04))
  {
    x16 wordrg31 = CUTX16((unsignedlong(wordrg1) >> con(0x7)) + unsignedlong(vw_00));
    t_gfxdata byterg42 = ddata.srcbmp[wordrg31 / 2];
    wordrg1 += CUTX16(longrg6);
    wordrg31 &= con(0x1);
    t_gfxdata byterg41;
    if (wordrg31 == con(0x0))
      byterg41 = ddata.stretchptr[byterg42 >> con(0x4)];
    else
    {
      byterg42 &= con(0xf);
      byterg41 = ddata.stretchptr[byterg42];
    }
    byterg41 <<= con(0x4);
    x16 wordrg32 = unsignedlong(vw_00) + (unsignedlong(wordrg1) >> con(0x7));
    byterg42 = ddata.srcbmp[wordrg32 / 2];
    wordrg1 += CUTX16(longrg6);
    wordrg32 &= con(0x1);
    if (wordrg32 == con(0x0))
      ddata.destbmp[wordrg2] = ddata.stretchptr[byterg42 >> con(0x4)] | byterg41;
    else
    {
      byterg42 &= con(0xf);
      vb_08 = ddata.stretchptr[byterg42] | byterg41;
      t_gfxdata* ptrrg41 = ddata.destbmp + unsignedlong(wordrg2);
      *ptrrg41 = vb_08;
    }
    wordrg2++;
  }
}

static void SKW_44c8_20e5(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  x16 vw_00;
  x32 vl_08;
  x8 vb_0c;

  vw_00 = eaxw;
  vl_08 = lextended(ebxw);
  x16 wordrg4 = ebxw / 2;
  x16 wordrg3 = (ecxw + 1) / 2;
  x16 wordrg2 = edxw / 2;
  while (--wordrg3 != con(0xffffffff))
  {
    x16 wordrg51 = CUTX16((unsignedlong(wordrg4) >> con(0x7)) + unsignedlong(vw_00));
    x8 byterg12 = ddata.srcbmp[wordrg51 / 2];
    wordrg4 += CUTX16(vl_08);
    wordrg51 &= con(0x1);
    if (unsignedlong(wordrg51) == con(0x0))
      byterg12 &= con(0xfffffff0);
    else
      byterg12 <<= con(0x4);
    x16 wordrg52 = CUTX16(unsignedlong(vw_00) + (unsignedlong(wordrg4) >> con(0x7)));
    t_gfxdata byterg11 = ddata.srcbmp[wordrg52 / 2];
    wordrg4 += CUTX16(vl_08);
    wordrg52 &= con(0x1);
    if (unsignedlong(wordrg52) == con(0x0))
      vb_0c = byterg12 | (ubrshift(byterg11, con(0x4)));
    else
    {
      byterg11 &= con(0xf);
      byterg11 |= byterg12;
      vb_0c = byterg11;
    }
    ddata.destbmp[wordrg2] = vb_0c;
    wordrg2++;
  }
}

// stacksize was 0x30
void SKW_FIRE_STRETCH_BLIT_TO_MEMORY_4TO4BPP(t_gfxdata* eaxpg, x16 ebxw, x16 ecxw, t_gfxdata* edxpg, x16 argw0, x16 argw1, t_gfxdata* argp2)
{
  t_gfxdata* pg_00;
  x32 vl_04;
  x16 vw_0c = con(0x0); // TODO: used uninitialized
  x16 vw_10;
  x16 vw_18;
  x16 vw_1c;

  pg_00 = eaxpg;
  t_gfxdata* gptrrg7 = edxpg;
  ddata.srcbmp = eaxpg;
  ddata.destbmp = edxpg;
  ddata.stretchptr = argp2;
  vw_18 = (argw0 + 1) & con(0xfffffffe);
  vl_04 = (lextended(ebxw) + 1) & con(0xfffffffe);
  vw_1c = CUTX16((signedlong(ebxw) << con(0x7)) / signedlong(argw0));
  x16 vw_08 = CUTX16((signedlong(ecxw) << con(0x7)) / signedlong(argw1));
  x16 vw_14 = vw_08 / 2;
  x16 wordrg5 = con(0x0);
  while (wordrg5 < argw1)
  {
    x16 wordrg11 = urshift(vw_14, con(0x7));
    x16 wordrg6 = wordrg11;
    if (wordrg11 != vw_0c || wordrg5 == 0)
    {
      vw_10 = CUTX16(vl_04 * wordrg6);
      x16 wordrg12 = vw_18 * wordrg5;
      if (argp2 != NULL)
        SKW_44c8_2143(vw_10, vw_1c, argw0, wordrg12);
      else
        SKW_44c8_20e5(vw_10, vw_1c, argw0, wordrg12);
    }
    else
    {
      ddata.srcbmp = gptrrg7;
      x16 wordrg41 = vw_18 * wordrg5;
//                  srcofs
      R_15B5(((wordrg5 - 1) * vw_18), argw0, wordrg41);
      ddata.srcbmp = pg_00;
    }

    vw_0c = wordrg6;
    vw_14 += vw_08;
    wordrg5++;
  }
}
