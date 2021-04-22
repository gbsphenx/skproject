#include "addon.h"
#include "driver.h" // DRVW_mousevisicnt
#include "dm2global.h"
#include "dm2data.h"
#include "mdata.h"
#include "src/c_alloc.h"
#include "src/c_dballoc.h"
#include "src/c_mouse.h"
#include "src/c_gfx_pal.h"
#include "src/c_gfx_stretch.h"
#include "src/c_gfx_misc.h"
#include "src/c_gfx_blit.h"
#include "src/c_gfx_str.h"
#include "src/c_gfx_fill.h"
#include "src/c_gfx_decode.h"
#include "src/c_savegame.h"
#include "calls.h"



#include "../skwin/defines.h"

#include "dm2debug.h"


void SKW_14cd_1e6e(x8 eaxb, s_hexe* ebxphe, x8 edxb)
{
  if (R_3DC4C(ddata.s350.v1e0571))
  {
    bool doit = false; // added
    if (eaxb != con(0x0))
    {
      if ((SKW_RAND() & con(0x1f)) == con(0x0))
      {
        ddata.s350.v1e054e->ua.b.b0 &= con(0x7f);
      }
      doit = true;
    }
    else if ((ddata.s350.v1e054e->ua.w & con(0x80)) == con(0x0))
    {
      if ((SKW_RAND() & con(0x3f)) != con(0x0))
        return;
      ddata.s350.v1e054e->ua.b.b0 |= con(0xffffff80);
      doit = true;
    }
    if (doit)
    {
      SKW_14cd_0f3c(con(0x0), getadr_r1d41d6(), con(0x0), ebxphe, con(0x0), con(0xffff), edxb, eaxb);
      return;
    }
  }
  ddata.s350.v1e054e->ua.b.b0 &= con(0x7f);
}

x8 SKW_PROCEED_XACT(x8 eaxb)
{
  ui8 ubyterg8 = eaxb - con(0x3f);
  if (ubyterg8 > con(0x23))
    return con(0xfffffffe);
  switch (ubyterg8)
  {
    case 0:
    return SKW_PROCEED_XACT_56();

    case 1:
      SKW_PROCEED_XACT_57();
    return con(0xfffffffe);

    case 2:
      ddata.s350.v1e0556->u.v.b1a = con(0x13);
    return con(0xfffffffe);

    case 20:
      ddata.s350.v1e0572 = con(0xffffffff);
      ddata.s350.v1e0574 = con(0x0);
    case 3: // no break before
    return SKW_PROCEED_XACT_59_76();

    case 4:
      ddata.s350.v1e0556->u.v.b1a = con(0x0);
    case 5: // no break before
    return con(0xfffffffe);

    case 6:
    return SKW_PROCEED_XACT_62();

    case 7:
    return SKW_PROCEED_XACT_63();

    case 8:
    return SKW_PROCEED_XACT_64();

    case 9:
    return SKW_PROCEED_XACT_65();

    case 10:
    return SKW_PROCEED_XACT_66();

    case 11:
    return SKW_PROCEED_XACT_67();

    case 12:
    return SKW_PROCEED_XACT_68();

    case 13:
      SKW_PROCEED_XACT_69();
    return con(0xfffffffe);

    case 14:
    return SKW_PROCEED_XACT_70();

    case 15:
    return SKW_PROCEED_XACT_71();

    case 16:
    case 31:
    case 32:
      SKW_PROCEED_XACT_72_87_88();
    return con(0xfffffffe);

    case 17:
    return SKW_PROCEED_XACT_73();

    case 18:
    return SKW_PROCEED_XACT_74();

    case 19:
    return SKW_PROCEED_XACT_75();

    case 21:
    return SKW_PROCEED_XACT_77();

    case 22:
    return SKW_PROCEED_XACT_78();

    case 23:
      SKW_PROCEED_XACT_79();
    return con(0xfffffffe);

    case 24:
    return SKW_PROCEED_XACT_80();

    case 25:
    return SKW_PROCEED_XACT_81();

    case 26:
    return SKW_PROCEED_XACT_82();

    case 27:
    return SKW_PROCEED_XACT_83();

    case 28:
    return SKW_PROCEED_XACT_84();

    case 29:
    return SKW_PROCEED_XACT_85();

    case 30:
    return SKW_PROCEED_XACT_86();

    case 33:
    return SKW_PROCEED_XACT_89();

    case 34:
    return SKW_PROCEED_XACT_90();

    case 35:
    return SKW_PROCEED_XACT_91();

    default: throw(THROW_DMABORT);
  }
}

// was SKW_14cd_274f
x8 SKW_PROCEED_XACT_66(void)
{
  x8 byterg1;
  if (!SKW_14cd_2662(con(0x2)))
  {
    x16 wordrg16 = ddata.s350.v1e0572;
    ddata.s350.v1e0572 = con(0x10);
    ddata.s350.v1e0574 = con(0x2);
    if (SKW_PROCEED_XACT_63() == con(0xfffffffe))
      return con(0xfffffffe);
    if (wordrg16 != con(0x0))
    {
      ddata.s350.v1e0572 = con(0x7);
      if (SKW_PROCEED_XACT_63() == con(0xfffffffe))
        return con(0xfffffffe);
    }
    if (ddata.s350.v1e0556->u.v.w0e-- <= con(0x0))
    {
      ddata.s350.v1e0556->u.v.w0e = con(0x5);
      ddata.s350.v1e0556->u.v.b1a = con(0x1e);
      return con(0xfffffffd);
    }
    if (ddata.s350.v1e0556->u.v.w0e > con(0x5))
    {
      ddata.s350.v1e0556->u.v.w0e -= con(0x5);
    }
    byterg1 = con(0xfffffffd);
  }
  else
  {
    if (ddata.s350.v1e0556->u.v.w0e-- <= con(0x5))
    {
      ddata.s350.v1e0556->u.v.w0e = con(0x9);
      ddata.s350.v1e0556->u.v.b1a = con(0x1f);
      return con(0xfffffffc);
    }
    byterg1 = con(0xfffffffc);
  }
  ddata.s350.v1e0556->u.v.b1a = con(0x1d);
  return byterg1;
}

// was SKW_14cd_28d2
x8 SKW_PROCEED_XACT_67(void)
{
  x16* wptrrg7;
  t_gfxdata vga_00[0x14];
  x16 vw_18;
  x16 vw_1c;
  x16 vw_20;
  x8 vb_24;

  vb_24 = con(0xfffffffd);
  vw_20 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
  if (!SKW_14cd_2662(con(0x2)))
  {
    x16 wordrg14 = SKW_GET_CREATURE_AT(table1d27fc[vw_20] + unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_07) + table1d2804[vw_20]);
    if (wordrg14 == con(0xffffffff))
      return vb_24;
    ddata.s350.v1e0556->u.v.b1a = con(0x1d);
    wptrrg7 = &SKW_GET_ADDRESS_OF_RECORD(wordrg14)->u2.w;
    x16 wordrg6 = SKW_14cd_2886(wptrrg7, CUTX8(vw_20), con(0x0), con(0x10), con(0x0), con(0x0));
    x16 wordrg13 = SKW_14cd_2886(wptrrg7, CUTX8(vw_20), con(0x0), con(0x7), con(0x0), con(0x0));
    if (wordrg13 != con(0xffffffff))
    {
      if (wordrg6 != con(0xffffffff))
        wordrg6 += wordrg13;
      else
        wordrg6 = wordrg13;
    }
    if (wordrg6 == con(0xffffffff))
    {
      ddata.s350.v1e0556->u.v.w10 = con(0x0);
      return vb_24;
    }
    x16 wordrg2 = SKW_48ae_0767(SKW_14cd_2886(wptrrg7, (CUTX8(vw_20) + con(0x2)) & con(0x3), con(0x1), con(0x10), con(0x1), con(0x0)), vga_00, &vw_1c, con(0x12));
    vw_18 = wordrg2;
    x16 wordrg3 = ddata.s350.v1e0556->u.v.w10;
    ddata.s350.v1e0556->u.v.w10 = wordrg6;
    if (wordrg2 > con(0x10))
      wordrg2 = wordrg2 - SKW_RAND16(con(0x10)) * wordrg2 / con(0x64);
    x16 wordrg450 = 100 * wordrg6 / wordrg2;
    if ((wordrg6 == wordrg3) || (wordrg6 >= UI16CAST(wordrg2)))
    {
      if (wordrg6 >= UI16CAST(wordrg2))
      {
        ddata.s350.v1e0556->u.v.w0e = DM2_MIN(wordrg6, vw_18);
        ddata.s350.v1e0556->u.v.b1a = con(0x1c);
        return con(0xfffffffe);
      }
      x16 wordrg11 = ddata.s350.v1e0556->u.v.w0e;
      x16 wordrg21 = wordrg11 - con(0x1);
      ddata.s350.v1e0556->u.v.w0e = wordrg21;
      if (wordrg11 > con(0x0))
      {
        if (ddata.s350.v1e0556->u.v.w0e > con(0x6))
          ddata.s350.v1e0556->u.v.w0e = wordrg21 - con(0x4);
        ddata.s350.v1e0556->u.v.b1a = con(0x1d);
        return con(0xfffffffc);
      }
      if (wordrg450 > con(0x4c))
      {
        if (SKW_RAND02() == con(0x0) != SKW_RAND16(DM2_MAX(con(0x1), con(0x64) - wordrg450)) < con(0x5))
        {
          ddata.s350.v1e0556->u.v.b1a = con(0x20);
          return vb_24;
        }
      }
      ddata.s350.v1e0556->u.v.b1a = con(0x1b);
      return vb_24;
    }
    if (wordrg3 != con(0xffffffff) && wordrg6 > wordrg3)
    {
      if (SKW_RAND02() == con(0x0) || (wordrg450 > (CUTX16(SKW_RAND() & con(0x7)) + con(0x4c))))
      {
        ddata.s350.v1e0556->u0c.w = con(0x0);
        bool boolrg40 = SKW_RAND16(DM2_MAX(con(0x1), con(0x64) - wordrg450)) < con(0x5);
        if ((SKW_RAND02() == con(0x0)) == boolrg40)
          ddata.s350.v1e0556->u.v.b1a = con(0x1b);
        else
          ddata.s350.v1e0556->u.v.b1a = con(0x20);
        return con(0xfffffffc);
      }
    }
  }
  else
  {
    x16 wordrg12 = ddata.s350.v1e0556->u.v.w0e;
    x16 wordrg402 = wordrg12;
    wordrg12--;
    ddata.s350.v1e0556->u.v.w0e = wordrg12;
    if (wordrg402 <= con(0x6))
    {
      ddata.s350.v1e0556->u.v.w0e = SKW_RAND02() + con(0x9);
      ddata.s350.v1e0556->u.v.b1a = con(0x1f);
      return con(0xfffffffc);
    }
  }
  ddata.s350.v1e0556->u.v.b1a = con(0x1d);
  return con(0xfffffffc);
}

// was SKW_14cd_2e6e
x8 SKW_PROCEED_XACT_65(void)
{
  ddata.s350.v1e0556->u0c.w = con(0xffffffff);
  x16 wordrg1 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
  x16 wordrg3 = 2 * table1d27fc[wordrg1] + unsignedword(ddata.s350.v1e0562.u.b.b_06);
  x16 wordrg2 = 2 * table1d2804[wordrg1] + unsignedword(ddata.s350.v1e0562.u.b.b_07);
  x16 wordrg4 = SKW_GET_CREATURE_AT(wordrg3, wordrg2);
  if (wordrg4 != con(0xffffffff))
  {
    if ((SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg4) & con(0x1)) == con(0x0))
      return con(0xfffffffe);
  }
  if (ddata.v1d3248 != ddata.v1e08d6 || wordrg3 != ddata.v1e08d8 || wordrg2 != ddata.v1e08d4)
  {
    ddata.s350.v1e0556->u.v.b1a = con(0x1d);
    return con(0xfffffffc);
  }
  return con(0xfffffffe);
}

// was SKW_14cd_3087
x8 SKW_PROCEED_XACT_71(void)
{
  x16* wptrrg3;

  wptrrg3 = &ddata.s350.v1e054e->u2.w;
  x16 wordrg1 = ddata.s350.v1e0574;
  if (wordrg1 != con(0xfffffffe))
  {
    bool boolrg4;
    if (wordrg1 != con(0xffffffff) || (wordrg1 = ddata.s350.v1e07d8.w6, wordrg1 != con(0xffffffff)))
      boolrg4 = true;
    else
      boolrg4 = false;
    if (boolrg4)
      if (*wptrrg3 != con(0xfffffffe))
        SKW_1c9a_078b(wptrrg3, con(0xff), wordrg1);
  }
  if (*wptrrg3 == con(0xfffffffe))
    return con(0xfffffffd);
  x16 wordrg3 = ddata.s350.v1e0572;
  if (wordrg3 == con(0xffffffff))
  {
    wordrg3 = ddata.s350.v1e07d8.w4;
    if (wordrg3 == con(0xffffffff))
      return con(0xfffffffe);
  }
  if (SKW_1c9a_0732(wordrg3, con(0xff), *wptrrg3) == con(0xfffffffe))
    return con(0xfffffffd);
  SKW_19f0_2165(con(0x81), unsignedword(ddata.s350.v1e0562.u.b.b_07), unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_07), con(0xffffffff), wordrg3);
  return ddata.s350.v1e056f;
}

// was SKW_14cd_3139
x8 SKW_PROCEED_XACT_73(void)
{
  s_hexe* sheptrrg1;
  s_hexe* sheptrrg2;
  bool flag;

  ui16 uwordrg1 = ddata.s350.v1e0574;
  if (uwordrg1 >= con(0x3))
  {
    if (uwordrg1 <= con(0x4))
    {
      x8 byterg1 = ((uwordrg1 != con(0x3)) ? 1 : 0) + con(0x13);
      x16 wordrg5 = ddata.s350.v1e054e->ua.w;
      sheptrrg1 = ddata.s350.v1e07d8.phe_a;
      if (sheptrrg1 == NULL)
        return con(0xfffffffd);
      do
      {
        if (byterg1 == sheptrrg1->b_0c)
        {
          if (sheptrrg1->w_04 != con(0x0))
          {
            if (sheptrrg1->w_04 == con(0x1))
              ddata.s350.v1e054e->ua.w &= ~(con(0x1) << CUTX8(sheptrrg1->w_06));
          }
          else
            ddata.s350.v1e054e->ua.w |= con(0x1) << CUTX8(sheptrrg1->w_06);
        }
        sheptrrg2 = sheptrrg1;
        sheptrrg1++;
      } while (sheptrrg2->b_0d != con(0x0));
      if (wordrg5 != ddata.s350.v1e054e->ua.w)
        ddata.s350.v1e0556->u.v.b1a = con(0x33);
      flag = wordrg5 != ddata.s350.v1e054e->ua.w;
      return (flag ? 1 : 0) - con(0x3);
    }
    if (uwordrg1 < con(0x10) || uwordrg1 > con(0x12))
      return con(0xfffffffd);
  }
  x16 wordrg6 = uwordrg1 & con(0x10);
  uwordrg1 &= con(0xf);
  x16 wordrg2 = con(0x1) << CUTX8(ddata.s350.v1e0572);
  bool boolrg3 = (ddata.s350.v1e054e->ua.w & wordrg2) == wordrg2;
  x16 wordrg51 = ddata.s350.v1e054e->ua.w;
  if (uwordrg1 != con(0x0))
  {
    if (uwordrg1 == con(0x1))
      ddata.s350.v1e054e->ua.w = wordrg51 | wordrg2;
  }
  else
    ddata.s350.v1e054e->ua.w = wordrg51 & ~wordrg2;
  if (wordrg6 == con(0x0) && wordrg51 != ddata.s350.v1e054e->ua.w)
    ddata.s350.v1e0556->u.v.b1a = con(0x33);
  return (boolrg3 ? 1 : 0) - con(0x3);
}

// return not needed
void SKW_14cd_3582(x16 eaxw, x16 edxw)
{
  x16 vwa_04[0xa];
  x16 vwa_18[0xa];

  x16 wordrg6 = con(0x0);
  SKW_COUNT_BY_COIN_TYPES(edxw, vwa_18);
  x16 wordrg3 = con(0x0);
  x16 wordrg24 = con(0x0);
  while (wordrg24 < ddata.v1e03fe)
  {
    wordrg3 += ddata.v1e03ac[wordrg24] * vwa_18[wordrg24];
    wordrg24++;
  }
  DM2_ZERO_MEMORY(DOWNCAST(x16, vwa_04), con(0x14));
  x16 wordrg23 = ddata.v1e03fe;
  while ((--wordrg23 >= con(0x0)) && wordrg3 > con(0x0))
  {
    for(;;)
    {
      x16 wordrg444 = ddata.v1e03ac[wordrg23];
      if (wordrg3 < wordrg444)
        break;
      vwa_04[wordrg23]++;
      wordrg3 -= wordrg444;
    }
    wordrg6 |= vwa_18[wordrg23] != vwa_04[wordrg23] ? 1 : 0; // BUGFIX 4.6/2014
    if (wordrg6 != con(0x0))
      if (eaxw == con(0x1))
        return;
  }
  if (wordrg6 == con(0x0))
    return;
  if (eaxw == con(0x1))
    return;
  x16 wordrg22 = con(0x0);
  while (wordrg22 < ddata.v1e03fe)
  {
    for(;;)
    {
      x16 wordrg13 = SKW_TAKE_COIN_FROM_WALLET(edxw, wordrg22);
      if (wordrg13 == con(0xffffffff))
        break;
      SKW_DEALLOC_RECORD(wordrg13);
    }
    wordrg22++;
  }
  x16 wordrg21 = con(0x0);
  while (wordrg21 < ddata.v1e03fe)
  {
    for(;;)
    {
      x16 wordrg405 = vwa_04[wordrg21];
      vwa_04[wordrg21] = wordrg405 - con(0x1);
      if (wordrg405 == con(0x0))
        break;
      x16 wordrg11 = SKW_ALLOC_NEW_DBITEM(ddata.v1e0394[wordrg21]);
      if (wordrg11 != con(0xffffffff))
        SKW_ADD_COIN_TO_WALLET(edxw, wordrg11);
    }
    wordrg21++;
  }
}

bool SKW_1887_034e(void)
{
  c_record* recoptrrg5;
  c_record** recopptrrg3;
  c_record* prec_00;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;

  if (!SKW_CREATURE_GO_THERE(ddata.s350.v1e0556->u.v.b20 | con(0xffffff80), unsignedword(ddata.s350.v1e0562.u.b.b_07), ddata.v1e08a6, unsignedword(ddata.s350.v1e0562.u.b.b_06), ddata.v1e08a4, signedword(ddata.s350.v1e0556->u.v.b1b)) || UI8CAST(table1d613a[ddata.s350.v1e0556->u.v.b1a] & con(0x4)) == con(0x0))
    return true;
  x16 wordrg6 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
  ddata.v1e08a6 = ddata.s350.v1e0556->u.v.u18.w & con(0x1f);
  ddata.v1e08a4 = ulrshift(ddata.s350.v1e0556->u.v.u18.w, con(0x6), con(0xb));
  if (ddata.s350.v1e0556->u.v.b1a == con(0x39) || ddata.s350.v1e0556->u.v.b1a == con(0x3a))
  {
    recopptrrg3 = &prec_00;
    if (ddata.s350.v1e0556->u.v.b1a != con(0x39))
      vw_08 = con(0x1);
    else
      vw_08 = con(0xffffffff);
    wordrg6 = vw_08;
    x16 wordrg4 = SKW_FIND_LADDAR_AROUND(unsignedword(ddata.s350.v1e0562.u.b.b_06), wordrg6, recopptrrg3, unsignedword(ddata.s350.v1e0562.u.b.b_07));
    if (prec_00 == NULL)
    {
      vw_0c = unsignedword(ddata.s350.v1e0562.u.b.b_06);
      vw_04 = unsignedword(ddata.s350.v1e0562.u.b.b_07);
      x16 wordrg7 = ddata.v1d3248;
      SKW_CHANGE_CURRENT_MAP_TO(CUTX16(SKW_LOCATE_OTHER_LEVEL(ddata.v1d3248, &vw_0c, &vw_04, wordrg6, NULL)));
      wordrg6 = SKW_FIND_LADDAR_AROUND(vw_0c, -vw_08, &prec_00, vw_04);
      SKW_CHANGE_CURRENT_MAP_TO(wordrg7);
      if (wordrg6 == con(0xffffffff))
        wordrg6 = SKW_RAND02();
    }
    else
    {
      if ((prec_00->u4.b.b0 & con(0x20)) == con(0x0))
        wordrg6 = (ulrshift(prec_00->u4.w, con(0xb), con(0xe)) + wordrg4) & con(0x3);
      else
        wordrg6 = ulrshift(prec_00->u4.w, con(0xb), con(0xe));
    }
  }
  x8 byterg7 = ddata.s350.v1e0552->barr06[con(0x3)] & con(0x40);
  if (byterg7 != con(0x0))
    SKW_OPERATE_PIT_TELE_TILE(unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0x1), unsignedword(ddata.s350.v1e0562.u.b.b_07));
  SKW_MOVE_RECORD_TO(ddata.s350.v1e054c, unsignedword(ddata.s350.v1e0562.u.b.b_07), con(0xffffffff), unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0x0));
  SKW_1c9a_0648(urshift(ddata.s350.v1e0556->u.v.u18.w, con(0xa)));
  if (ddata.s350.v1e0556->u.v.b1a == con(0x35) || ddata.s350.v1e0556->u.v.b1a == con(0x36))
    wordrg6 = SKW_0cee_06dc(ddata.v1e08a6, ddata.v1e08a4);
  recoptrrg5 = ddata.s350.v1e054e;
  recoptrrg5->ue.b.b1 &= con(0xfffffffc);
  recoptrrg5->ue.w |= (wordrg6 & con(0x3)) << con(0x8);
  if (SKW_MOVE_RECORD_TO(ddata.s350.v1e054c, con(0x0), ddata.v1e08a6, con(0xffffffff), ddata.v1e08a4) == con(0x0))
  {
    ddata.s350.v1e0562.u.b.b_06 = CUTX8(ddata.v1e102a);
    ddata.s350.v1e0562.u.b.b_07 = CUTX8(ddata.v1e1028);
    ddata.s350.v1e0571 = CUTX8(ddata.v1e102c);
    SKW_1c9a_0648(unsignedword(ddata.s350.v1e0571));
    if (byterg7 != con(0x0))
      SKW_OPERATE_PIT_TELE_TILE(unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0x0), unsignedword(ddata.s350.v1e0562.u.b.b_07));
    if (ddata.s350.v1e07d8.b1 != con(0x0))
      ddata.s350.v1e07d8.b1--;
  }
  else
    ddata.s350.v1e0570 = con(0x1);
  return false;
}

// was SKW_14cd_32a4
x8 SKW_PROCEED_XACT_74(void)
{
  x16 wordrg430 = urshift(ddata.s350.v1e0552->w16, con(0xc));
  bool boolrg4;
  if (wordrg430 == con(0x0))
    boolrg4 = false;
  else
  {
    if ((ddata.s350.v1e054e->ua.w & con(0x2000)) != con(0x0))
      wordrg430 = wordrg430 / 4;
    if ((CUTX16(SKW_RAND()) & con(0xf)) >= wordrg430)
      boolrg4 = false;
    else
      boolrg4 = true;
  }
  bool boolrg6 = boolrg4;
  if (SKW_1c9a_381c() != con(0x0))
  {
    if (boolrg6)
    {
      ddata.s350.v1e0556->u.v.b1a = con(0x0);
      return con(0xfffffffc);
    }
    x8 byterg4 = con(0xffffff80);
    if ((ddata.s350.v1e054e->ua.w & con(0x1000)) != con(0x0))
      byterg4 = con(0xffffffa0);
    SKW_CREATURE_GO_THERE(byterg4, unsignedword(ddata.s350.v1e0562.u.b.b_07), con(0xffffffff), unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0xffffffff), signedword(ddata.s350.v1e0556->u.v.b1b));
    return ddata.s350.v1e056f;
  }
  if (unsignedword(ddata.s350.v1e0562.u.b.b_06) == (ddata.s350.v1e0556->u.v.u18.w & con(0x1f)))
    if (unsignedword(ddata.s350.v1e0562.u.b.b_07) == ulrshift(ddata.s350.v1e0556->u.v.u18.w, con(0x6), con(0xb)) && urshift(ddata.s350.v1e0556->u.v.u18.w, con(0xa)) == ddata.v1d3248)
      return con(0xfffffffe);
  x16 wordrg402 = SKW_CALC_VECTOR_DIR(unsignedword(ddata.s350.v1e0562.u.b.b_06), ddata.s350.v1e0556->u.v.u18.w & con(0x1f), ulrshift(ddata.s350.v1e0556->u.v.u18.w, con(0x6), con(0xb)), unsignedword(ddata.s350.v1e0562.u.b.b_07));
  if (ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe)) == wordrg402)
    return con(0xfffffffe);
  if (!boolrg6 || SKW_RAND01() == con(0x0))
  {
    SKW_19f0_0559(wordrg402);
    return ddata.s350.v1e056f;
  }
  ddata.s350.v1e0556->u.v.b1a = con(0x0);
  return con(0xfffffffc);
}

bool SKW_PROCEED_CCM(void)
{
  bool boolrg3;

  boolrg3 = false;
  ddata.s350.v1e0898 = UPCAST(x8, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0x100)));
  ddata.v1e08a6 = ddata.s350.v1e0556->u.v.u18.w & con(0x1f);
  ddata.v1e08a4 = ulrshift(ddata.s350.v1e0556->u.v.u18.w, con(0x6), con(0xb));
  ui8 ubyterg1 = ddata.s350.v1e0556->u.v.b1a;
  if (ubyterg1 < con(0x17))
  {
    if (ubyterg1 < con(0x9))
    {
      if (ubyterg1 < con(0x5))
      {
        if (ubyterg1 >= con(0x1))
        {
          if (ubyterg1 > con(0x2))
            boolrg3 = SKW_1887_0205();
          else
            boolrg3 = SKW_CREATURE_WALK_NOW();
        }
      }
      else if (ubyterg1 <= con(0x5))
        boolrg3 = SKW_1887_0239();
      else if (ubyterg1 <= con(0x7))
        SKW_1887_05aa();
      else
        boolrg3 = SKW_CREATURE_ATTACKS_PARTY();
    }
    else if (ubyterg1 <= con(0x9))
      boolrg3 = SKW_CREATURE_WALK_NOW();
    else if (ubyterg1 < con(0xc))
    {
      if (ubyterg1 <= con(0xa))
        boolrg3 = SKW_1887_0b1b();
      else
        boolrg3 = SKW_1887_0c75();
    }
    else if (ubyterg1 <= con(0xd))
      boolrg3 = SKW_1887_0ce1();
    else if (ubyterg1 < con(0x13))
    {
      if (ubyterg1 <= con(0xf))
        boolrg3 = SKW_1887_0eca();
    }
    else if (ubyterg1 <= con(0x13))
      SKW_1887_0df7();
    else if (ubyterg1 >= con(0x15))
      boolrg3 = SKW_1887_0f80();
  }
  else if (ubyterg1 <= con(0x17))
    boolrg3 = SKW_PLACE_MERCHANDISE();
  else if (ubyterg1 < con(0x2b))
  {
    if (ubyterg1 < con(0x1a))
    {
      if (ubyterg1 <= con(0x18))
        boolrg3 = SKW_1887_10db();
      else
        boolrg3 = SKW_1887_0d33();
    }
    else if (ubyterg1 <= con(0x1a))
      boolrg3 = SKW_1887_0e19();
    else if (ubyterg1 < con(0x27))
    {
      if (ubyterg1 == con(0x26))
        boolrg3 = SKW_CREATURE_ATTACKS_PARTY();
    }
    else if (ubyterg1 <= con(0x28))
      boolrg3 = SKW_1887_09ab();
    else
      boolrg3 = SKW_1887_0d33();
  }
  else if (ubyterg1 <= con(0x2c))
    boolrg3 = SKW_1887_0e19();
  else if (ubyterg1 < con(0x35))
  {
    if (ubyterg1 <= con(0x2e))
      boolrg3 = SKW_1887_0d33();
    else if (ubyterg1 <= con(0x31))
      boolrg3 = SKW_1887_1191();
  }
  else if (ubyterg1 <= con(0x3a))
    boolrg3 = SKW_1887_034e();
  else if (ubyterg1 < con(0x3d))
    SKW_1887_126d();
  else if (ubyterg1 <= con(0x40))
    boolrg3 = SKW_1887_141c();
  else if (ubyterg1 == con(0x55))
    R_1B7D5();
  if (!boolrg3)
  {
    if ((table1d613a[ddata.s350.v1e0556->u.v.b1a] & con(0x3)) != con(0x0))
      ddata.s350.v1e0556->barr04[con(0x0)] = CUTLX8(ddata.longmallocx);
  }
  else
    ddata.s350.v1e07d8.b1 = con(0x0);
  ddata.s350.v1e0898 = NULL;
  DM2_DEALLOC_LOBIGPOOL(con(0x100));
  return boolrg3;
}

// was SKW_14cd_2c23
x8 SKW_PROCEED_XACT_62(void)
{
  x16 vw_0c;
  x16 vw_10;
  x16 vw_14;

  bool vbool_00 = false;
  x16 wordrg127 = ddata.s350.v1e057c;
  x16 vw_08 = wordrg127;
  wordrg127 &= con(0x77);
  if (wordrg127 == con(0x0))
    return con(0xfffffffd);
  if (ddata.s350.v1e0574 == con(0x1))
    if (SKW_1c9a_0732(con(0x10), con(0xff), ddata.s350.v1e054e->u2.w) != con(0xfffffffe))
      return con(0xfffffffe);
  x16 wordrg125;
  if (ddata.s350.v1e0572 == con(0x0))
  {
    vw_10 = ddata.s350.v1e0556->u.v.u18.w & con(0x1f);
    wordrg125 = ulrshift(ddata.s350.v1e0556->u.v.u18.w, con(0x6), con(0xb));
  }
  else
  {
    vw_10 = unsignedword(ddata.s350.v1e0562.u.b.b_06);
    wordrg125 = unsignedword(ddata.s350.v1e0562.u.b.b_07);
  }
  vw_0c = wordrg125;
  x16 wordrg6 = vw_10;
  x16 wordrg123 = SKW_FIND_TILE_ACTUATOR(wordrg6, con(0xff), con(0x30), vw_0c);
  if (wordrg123 == con(0xffffffff))
    return con(0xfffffffd);
  vw_14 = urshift(SKW_GET_ADDRESS_OF_RECORD(wordrg123)->u2.w, con(0x7));
  x16 wordrg11;
  if (ddata.s350.v1e0574 != con(0x2))
    wordrg11 = SKW_GET_WALL_TILE_ANYITEM_RECORD(wordrg6, vw_0c);
  else
  {
    x16 wordrg106 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
    x16 wordrg107 = SKW_GET_CREATURE_AT(unsignedword(ddata.s350.v1e0562.u.b.b_06) + table1d27fc[wordrg106], unsignedword(ddata.s350.v1e0562.u.b.b_07) + table1d2804[wordrg106]);
    if (wordrg107 == con(0xffffffff))
      return con(0xfffffffd);
    if (SKW_1c9a_06bd(wordrg107, CUTX8((ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe)) + con(0x2)) & con(0x3)), vw_14) == NULL)
      return con(0xfffffffd);
    return con(0xfffffffe);
  }
  x16 wordrg4;
  for(;;)
  {
    wordrg4 = wordrg11;
    if (wordrg4 == con(0xfffffffe))
      break;
    if ((wordrg4 & con(0x3c00), con(0xa)) >= con(0x5))
      break;
    wordrg11 = SKW_GET_NEXT_RECORD_LINK(wordrg4);
  }
  if (wordrg4 != con(0xfffffffe))
  {
    if (SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg4) != vw_14)
    {
      x16 wordrg21 = wordrg4;
      for(;;)
      {
        if (wordrg4 == con(0xfffffffe))
          break;
        if (urshift(wordrg4 & con(0x3c00), con(0xa)) > con(0xa))
          break;
        if (SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg4) == vw_14)
          break;
        wordrg4 = SKW_GET_NEXT_RECORD_LINK(wordrg4);
      }
      if (wordrg4 != con(0xfffffffe))
      {
        vbool_00 = true;
        for(;;)
        {
          if (wordrg21 == con(0xfffffffe))
            break;
          if (urshift(wordrg21 & con(0x3c00), con(0xa)) > con(0xa))
            break;
          x16 wordrg5 = wordrg21;
          if (SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg5) == vw_14)
            break;
          x16 vw_04 = SKW_GET_NEXT_RECORD_LINK(wordrg5);
          SKW_CUT_RECORD_FROM(wordrg5, vw_10, vw_0c, NULL);
          wordrg21 = vw_10;
          SKW_APPEND_RECORD_TO(wordrg5, wordrg21, vw_0c, NULL);
          wordrg21 = vw_04;
        }
      }
    }
    else
      vbool_00 = true;
  }
  if (vbool_00)
  {
    ddata.s350.v1e057c &= con(0x77);
    SKW_19f0_2165(con(0x80), unsignedword(ddata.s350.v1e0562.u.b.b_07), vw_10, unsignedword(ddata.s350.v1e0562.u.b.b_06), vw_0c, con(0xffffffff), con(0x10));
    ddata.s350.v1e057c = vw_08;
    return ddata.s350.v1e056f;
  }
  if (ddata.s350.v1e0556->u.v.w0e-- > con(0x0))
    ddata.s350.v1e0556->u.v.b1a = con(0x1d);
  else
  {
    ddata.s350.v1e0556->u.v.w0e = con(0x3);
    ddata.s350.v1e0556->u.v.b1a = con(0x1e);
  }
  return con(0xfffffffd);
}

bool SKW_19f0_0559(x16 eaxw)
{
  x8 byterg1;
  x16 wordrg4 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
  x16 wordrg2 = eaxw;
  x16 wordrg3 = wordrg4;
  if (wordrg3 != ((wordrg2 + con(0x2)) & con(0x3)))
  {
    if (wordrg4 == eaxw)
    {
      ddata.s350.v1e0556->u.v.b1a = con(0x0);
      ddata.s350.v1e056f = con(0xfffffffe);
      return con(0x0);
    }
    wordrg2--;
    wordrg2 &= con(0x3);
    if (wordrg3 == wordrg2)
      byterg1 = con(0x1);
    else
      byterg1 = con(0xffffffff);
  }
  else if (SKW_RAND01() == con(0x0))
    byterg1 = con(0xffffffff);
  else
    byterg1 = con(0x1);
  ddata.s350.v1e0556->u.v.b1d = (CUTX8(wordrg4) + byterg1) & con(0x3);
  ddata.s350.v1e0556->u.v.b1a = ((byterg1 != con(0xffffffff)) ? 1 : 0) + con(0x6);
  ddata.s350.v1e056f = con(0xfffffffc);
  return con(0x1);
}

x16 SKW_19f0_2024(x16 eaxw, x16 ebxw, x16 edxw)
{
  c_aispec* aiptrrg3;
  c_record* recoptrrg1;
  c_record* recoptrrg6;
  u_len10 vs_00;

  x16 wordrg6 = eaxw;
  vs_00.s_00.vw_0c = edxw;
  vs_00.s_00.vl_04 = lextended(ebxw);
  x8 byterg2;
  if ((ddata.s350.v1e057c & con(0x10)) == con(0x0) || !SKW_IS_CONTAINER_CHEST(wordrg6))
  {
    if (urshift(wordrg6 & con(0x3c00), con(0xa)) != con(0x4) || (ddata.s350.v1e057c & con(0x28)) == con(0x0))
      return con(0xffffffff);
    recoptrrg6 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
    aiptrrg3 = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(recoptrrg6->u4.b.b0);
    x16 wordrg2 = aiptrrg3->u0.w & con(0x1);
    x16 wordrg100;
    if (wordrg2 == con(0x0))
      wordrg100 = ddata.s350.v1e057c & con(0x20);
    else
      wordrg100 = ddata.s350.v1e057c & con(0x8);
    if (wordrg100 == con(0x0))
      return con(0xffffffff);
    vs_00.s_00.vl_08 = con(0x8);
    wordrg6 = recoptrrg6->u2.w;
    if (wordrg2 == con(0x0))
      byterg2 = con(0xf);
    else
      byterg2 = SKW_48ae_01af(aiptrrg3->u1e.w, CUTX16(vs_00.s_00.vl_04));
  }
  else
  {
    recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
    vs_00.s_00.vl_08 = lextended(wordrg6 >> con(0xe));
    wordrg6 = recoptrrg1->u2.w;
    byterg2 = con(0xf);
  }
  for (x16 wordrg5 = con(0x0); wordrg5 < con(0x4); wordrg5++)
  {
    vs_00.barr_00[wordrg5] = byterg2 & con(0x1);
    byterg2 >>= con(0x1);
  }
  for(;;)
  {
    if (wordrg6 == con(0xfffffffe))
      return con(0xffffffff);
    if (vs_00.barr_00[wordrg6 >> con(0xe)] != con(0x0))
    {
      x16 wordrg51 = vs_00.s_00.vw_0c;
      if (SKW_1c9a_0006(wordrg6, wordrg51 != con(0x0)))
        break;
      if (SKW_19f0_2024(wordrg6, CUTX16(vs_00.s_00.vl_04), wordrg51) != con(0xffffffff))
        break;
    }
    wordrg6 = SKW_GET_NEXT_RECORD_LINK(wordrg6);
  }
  if (CUTX16(vs_00.s_00.vl_08) == con(0x8))
    vs_00.s_00.vl_08 += lextended(urshift(wordrg6, con(0xe)));
  return CUTX16(vs_00.s_00.vl_08);
}

x16 SKW_19f0_266c(x16 eaxw, x8 ebxb, x16 ecxw, x16 edxw)
{
  c_record* recoptrrg1;
  bool flag;
  x16 vw_04;

  x16 wordrg5 = eaxw;
  vw_04 = edxw;
  for(;;)
  {
    if (wordrg5 == con(0xfffffffe) || wordrg5 == con(0xffffffff))
      return con(0xffff);
    x16 wordrg1 = urshift(wordrg5 & con(0x3c00), con(0xa));
    if (wordrg1 != con(0x3))
    {
      if (wordrg1 > con(0x3))
        return con(0xffff);
    }
    else
    {
      x16 wordrg2 = wordrg5;
      if ((wordrg2 >> con(0xe)) == vw_04)
      {
        recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg2);
        if ((recoptrrg1->u2.w & con(0x7f)) != con(0x0) && (recoptrrg1->u2.w & con(0x7f)) != con(0x26))
        {
          if ((recoptrrg1->u2.w & con(0x7f)) == con(0x1a))
          {
            if ((recoptrrg1->u4.b.b0 & con(0x4)) == con(0x0))
              flag = ebxb != con(0x1);
            else
              flag = ebxb != con(0x2);
            if (!flag)
              if (SKW_1c9a_0006(wordrg2, ecxw) != con(0x0))
                return wordrg5;
          }
        }
      }
    }
    wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg5);
  }
}

x16 SKW_1c9a_0732(x16 eaxw, x8 ebxb, x16 edxw)
{
  x16 wordrg3 = edxw;
  for(;;)
  {
    if (wordrg3 == con(0xfffffffe))
      return wordrg3;
    x16 wordrg4 = urshift(wordrg3 & con(0x3c00), con(0xa));
    if ((wordrg4 > con(0x4) && wordrg4 < con(0xe)) || wordrg4 == con(0x9))
      if (ebxb == con(0xffffffff) || ebxb == CUTX8(urshift(wordrg3, con(0xe)))) // BUGFIX 4.1/2014
        if (SKW_1c9a_0006(wordrg3, eaxw) != con(0x0))
          return wordrg3;
    wordrg3 = SKW_GET_NEXT_RECORD_LINK(wordrg3);
  }
}

// was SKW_1c9a_0a48
x32 SKW_CREATURE_SOMETHING_1c9a_0a48(void)
{
  c_record* recoptrrg3;
  t_gfxdata vga_00[0x4];
  x16 vw_04;
  x16 vw_08;
  x8 vb_0c;
  x8 vb_10;

  vb_10 = ddata.s350.v1e0552->barr06[con(0x3)];
  recoptrrg3 = ddata.s350.v1e054e;
  vb_0c = recoptrrg3->u4.b.b0;
  vw_04 = ddata.s350.v1e055e[con(0x0)];
  vw_08 = ddata.s350.v1e055e[con(0x1)];
  if (ddata.s350.v1e055a == NULL)
  {
    x16 wordrg2 = ((ddata.s350.v1e0552->u0.b.b0 & con(0x1)) == con(0x0)) ? con(0x0) : recoptrrg3->uc.w;
    SKW_GET_CREATURE_ANIMATION_FRAME(vb_0c, &vw_04, &vw_08, signedword(ddata.s350.v1e0556->u.v.b1a), &ddata.s350.v1e055a, wordrg2);
    if (ddata.s350.v1e055a == NULL)
    {
      DM2_ZERO_MEMORY(DOWNCAST(t_gfxdata, vga_00), con(0x4));
      ddata.s350.v1e055a = vga_00;
    }
  }
  x8 byterg2 = ddata.s350.v1e0556->barr04[con(0x3)];
  if ((ddata.s350.v1e055a[con(0x3)] & con(0x1)) != con(0x0) && ddata.s350.v1e0556->u.v.b1a != con(0x24) && ddata.s350.v1e0556->u.v.b1a != con(0x23) && ddata.s350.v1e0556->u.v.b1a != con(0x25))
  {
    byterg2 &= con(0xffffffc0);
    x8 byterg11 = vb_10 & con(0x3);
    if (byterg11 != con(0x0))
    {
      x16 wordrg480 = SKW_RAND16(unsignedword(byterg11));
      if (SKW_RAND01() != con(0x0))
      {
        wordrg480 = -wordrg480;
        wordrg480 &= con(0x7);
      }
      byterg2 |= CUTX8(wordrg480);
    }
    x8 byterg12 = (vb_10 / 4) & con(0x3);
    if (byterg12 != con(0x0))
    {
      x16 wordrg481 = SKW_RAND16(unsignedword(byterg12));
      if (SKW_RAND01() != con(0x0))
      {
        wordrg481 = -wordrg481;
        wordrg481 &= con(0x7);
      }
      wordrg481 *= 8;
      byterg2 |= CUTX8(wordrg481);
    }
  }
  if (((ddata.s350.v1e055a[con(0x3)] & con(0x2)) / 2) != con(0x0))
  {
    if (SKW_RAND01() == con(0x0))
      byterg2 &= con(0xffffffbf);
    else
      byterg2 |= con(0x40);
  }
  ddata.s350.v1e0556->barr04[con(0x3)] = byterg2;
  ddata.s350.v1e055e[con(0x0)] = vw_04;
  ddata.s350.v1e055e[con(0x1)] = vw_08;
  x8 byterg15 = ddata.s350.v1e055a[con(0x0)] & con(0x7f);
  if (byterg15 != con(0x7f))
    SKW_QUEUE_NOISE_GEN1(con(0xf), byterg15, con(0x46), vb_0c, con(0x80), unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_07), con(0x1));
  x32 longrg1;
  if ((ddata.s350.v1e054e->ua.w & con(0x40)) == con(0x0))
  {
    x16 wordrg499 = unsignedword((ddata.s350.v1e055a[con(0x3)] & con(0xc))) / 4;
    if (wordrg499 != con(0x0))
      wordrg499 = SKW_RAND16(wordrg499);
    wordrg499 += urshift(ddata.s350.v1e055a[con(0x3)] & con(0xfffffff0), con(0x4));
    if (ddata.s350.v1e0556->u.v.b1a != con(0x13) || ddata.savegames1.b3 == con(0x0) || (ddata.s350.v1e0552->u0.b.b1 & con(0x10)) != con(0x0))
    {
      if (ddata.v1d3248 == ddata.v1e08d6 || (table1d607e[ddata.s350.v1e0584].u.b[2] & con(0x1)) != con(0x0) || (ddata.s350.v1e054e->ua.w & con(0x8000)) == con(0x0) || (ddata.s350.v1e054e->ua.w & con(0x2)) != con(0x0))
      {
        if (!ddata.v1e0238)
        {
          if ((ddata.s350.v1e054e->ua.w & con(0x8)) != con(0x0))
          {
            x16 wordrg101 = ddata.s350.v1e054e->ua.w & con(0x4000);
            if (wordrg101 == con(0x0) || wordrg499 >= con(0x3))
            {
              wordrg101 = 75 * wordrg499 / con(0x64);
              wordrg499 *= 5;
            }
            wordrg499 = DM2_MAX(con(0x1), wordrg101);
          }
        }
        else
        {
          wordrg499 *= 2;
          if (ddata.v1d3248 != ddata.v1e08d6)
            wordrg499 *= 2;
        }
      }
      else
      {
        wordrg499 *= 4;
        wordrg499 += SKW_RAND01();
      }
    }
    else
      wordrg499 *= con(0x3);
    longrg1 = signedlong(wordrg499);
  }
  else
    longrg1 = signedlong(DM2_MIN(con(0x1), urshift(unsignedword(ddata.s350.v1e055a[con(0x3)] & con(0xfffffff0)), con(0x4))));
  longrg1 += ddata.longmallocx;
  if (vga_00 == ddata.s350.v1e055a)
    ddata.s350.v1e055a = NULL;
  return longrg1;
}

void SKW_DROP_CREATURE_POSSESSION(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0)
{
  x8 byterg5;
  x16 wordrg5;
  x16 vw_04;
  x16 vw_08;
  c_record* prec_0c;

  vw_08 = eaxw;
  vw_04 = edxw;
  if (ecxw == con(0x2))
    return;
  prec_0c = SKW_GET_ADDRESS_OF_RECORD(vw_08);
  if (ecxw == con(0x0))
  {
    for (x16 vw_1c = con(0xa); vw_1c <= con(0x14); vw_1c++)
    {
      x16 wordrg167 = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(prec_0c->u4.b.b0, CUTX8(vw_1c));
      ui16 vuw_18 = wordrg167;
      if (wordrg167 != con(0x0))
      {
        x16 vw_14 = (lextended(wordrg167) & con(0xffff000f)) + con(0x1);
        wordrg167 &= con(0x70);
        wordrg167 = urshift(wordrg167, con(0x4));
        if (wordrg167 != con(0x0))
        {
          wordrg167++;
          wordrg167 = SKW_RAND16(wordrg167);
          vw_14 += wordrg167;
        }
        vuw_18 >>= con(0x7);
        for(;;)
        {
          if (--vw_14 == con(0xffffffff))
            break;
          x16 wordrg6 = SKW_ALLOC_NEW_DBITEM(vuw_18);
          if (wordrg6 == con(0xffffffff))
            break;
          x16 wordrg181;
          if (vw_04 != ddata.v1e0270)
            wordrg181 = SKW_RAND02();
          else
          {
            if (ebxw != ddata.v1e0272)
              wordrg181 = SKW_RAND02();
            else
              wordrg181 = (SKW_RAND01() + ddata.v1e0258) & con(0x3);
          }
          wordrg5 = vw_04;
          wordrg181 <<= con(0xe);
          wordrg181 |= wordrg6 & con(0x3fff);
          SKW_MOVE_RECORD_TO(wordrg181, con(0x0), wordrg5, con(0xffffffff), ebxw);
          if (argw0 >= con(0x0))
          {
            byterg5 = SKW_QUERY_CLS2_FROM_RECORD(wordrg6);
            SKW_QUEUE_NOISE_GEN2(SKW_QUERY_CLS1_FROM_RECORD(wordrg6), con(0x85), con(0xfe), byterg5, wordrg5, ebxw, argw0, con(0x3a), con(0x80));
          }
        }
      }
    }
  }
  x16 wordrg61 = prec_0c->u2.w;
  if (wordrg61 != con(0xfffffffe))
  {
    do
    {
      x16 vw_20 = SKW_GET_NEXT_RECORD_LINK(wordrg61);
      if ((SKW_QUERY_CREATURE_AI_SPEC_FLAGS(vw_08) & con(0x1)) == con(0x0))
      {
        x16 wordrg11;
        if (vw_04 != ddata.v1e0270 || ebxw != ddata.v1e0272)
          wordrg11 = SKW_RAND02();
        else
          wordrg11 = (SKW_RAND01() + ddata.v1e0258) & con(0x3);
        wordrg61 &= con(0xffff3fff);
        wordrg11 <<= con(0xe);
        wordrg61 |= wordrg11;
      }
      if (urshift(wordrg61 & con(0x3c00), con(0xa)) != con(0xe))
      {
        wordrg5 = wordrg61;
        SKW_MOVE_RECORD_TO(wordrg5, con(0x0), vw_04, con(0xffffffff), ebxw);
        if (argw0 >= con(0x0))
          SKW_QUEUE_NOISE_GEN2(SKW_QUERY_CLS1_FROM_RECORD(wordrg5), con(0x85), con(0xfe), SKW_QUERY_CLS2_FROM_RECORD(wordrg5), vw_04, ebxw, argw0, con(0x3a), con(0x80));
      }
      else
        SKW_DEALLOC_RECORD(wordrg61);
      wordrg61 = vw_20;
    } while (wordrg61 != con(0xfffffffe));
  }
}

void SKW_RESET_CAII(void)
{
  c_record* recoptrrg1;

  x16 wordrg41 = con(0x0);
  ddata.v1d4020 = wordrg41;
  while (UI16CAST(wordrg41) < UI16CAST(ddata.v1e08a0))
  {
    ddata.v1e089c[wordrg41].w00 = con(0xffffffff);
    wordrg41++;
  }
  recoptrrg1 = ddata.savegameparr5[4];
  x16 wordrg4 = ddata.savegamep4->w0[con(0xa)];
  for(;;)
  {
    wordrg4--;
    if (wordrg4 == con(0xffffffff))
    {
      SKW_FILL_ORPHAN_CAII();
      return;
    }
    recoptrrg1->u4.b.b1 = con(0xffffffff);
    recoptrrg1++;
  }
}

void SKW_2066_2498(void)
{
  t_text* tptrrg5;
  x8 vba_00[0xc8]; // text in the beginning, struct for SKW_QUERY_MESSAGE_TEXT

  tptrrg5 = UPCAST(t_text, vba_00);
  SKW_FILL_STR(vba_00, con(0x31), con(0x1), con(0x40));
  x16 wordrg3 = con(0x0);
  x16 wordrg21 = con(0x0);
  while (wordrg21 < unsignedword(CUTX8(ddata.savegamep4->w0[con(0x2)])))
  {
    wordrg3 = DM2_MAX(ddata.v1e03c8[wordrg21].w8 & con(0x3f), wordrg3);
    wordrg21++;
  }
  ddata.v1e03d4 = DM2_ALLOC_FREEPOOL_MEMORY(signedlong(wordrg3) + con(0x1));
  SKW_CHANGE_CURRENT_MAP_TO(con(0x0));
  x16 wordrg407 = CUTX16((unsignedlong(ddata.savegamep4->w0[con(0x4)]) >> con(0x5)) & con(0x1f));
  x16 wordrg1 = SKW_GET_TILE_RECORD_LINK(ddata.savegamep4->w0[con(0x4)] & con(0x1f), wordrg407);
  for (;; wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg1))
  {
    if (wordrg1 != con(0xfffffffe))
      break;
    if (urshift(wordrg1 & con(0x3c00), con(0xa)) == con(0x2))
      break;
  }
  if (wordrg1 != con(0xfffffffe))
  {
    if ((SKW_GET_ADDRESS_OF_RECORD(wordrg1)->u2.b.b0 & con(0x6)) == con(0x0))
      SKW_QUERY_MESSAGE_TEXT(tptrrg5, con(0xffff8002), wordrg1);
  }
  x16 wordrg22 = con(0x0);
  x8 byterg402;
  while (wordrg22 <= wordrg3)
  {
    do
    {
      do
      {
        byterg402 = *tptrrg5++;
      } while (byterg402 == con(0x0));
    } while (byterg402 == con(0xa) || byterg402 == con(0x20));
    byterg402 -= con(0x30);
    ddata.v1e03d4[wordrg22] = byterg402;
    wordrg22++;
  }
}

// was SKW_2759_0541
// return never requested
x16 SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS(void)
{
  x16 wordrg1 = ddata.v1e0b74;
  if (wordrg1 == con(0x0))
    return con(0x0);
  if (ddata.v1e0b7c == con(0x0) || (wordrg1 = ddata.v1e0b6c, wordrg1 <= con(0x1)))
    ddata.v1e0b6c = con(0x0);
  ddata.v1e0b74 = con(0x0);
  ddata.v1d67bc = con(0x0);
  ddata.v1e0b62 = con(0x0);
  if (ddata.v1d6700 != con(0xffffffff))
    wordrg1 = SKW_PUT_OBJECT_INTO_CONTAINER(wordrg1);
  ddata.v1e0b30 = true;
  return wordrg1;
}

void SKW_2759_15d0(ui16 eaxuw, x16 edxw)
{
  x16 wordrg5;
  c_tim vtim_00;

  x16 wordrg6 = con(0x1);
  x16 wordrg1 = DM2_MAX(con(0x8), DM2_BETWEEN_VALUE(con(0x20), con(0x100), edxw + con(0x1)) / con(0x8));
  x16 wordrg402 = wordrg1;
  x16 wordrg2 = wordrg1 - con(0x8);
  if (eaxuw < con(0x26))
  {
    if (eaxuw == con(0x6))
    {
      wordrg5 = 16 * wordrg2 + con(0x10);
      wordrg6 = con(0xfffffffe);
      wordrg402 >>= con(0x1);
      wordrg402--;
    }
  }
  else if (eaxuw <= con(0x26))
  {
    wordrg5 = ((wordrg1 - con(0x3)) << con(0x7)) + con(0x7d0);
    wordrg402 >>= con(0x2);
    wordrg402 += wordrg6;
  }
  else if (eaxuw == con(0x27))
  {
    wordrg5 = (wordrg2 << con(0x9)) + con(0x2710);
    wordrg402 >>= con(0x1);
    wordrg402--;
  }
  vtim_00.b_04 = con(0x46);
  if (eaxuw != con(0x6))
    vtim_00.u.w_06 = -wordrg402;
  else
    vtim_00.u.w_06 = wordrg402;
  vtim_00.un.l_00 = (unsignedlong(wordrg5) + ddata.longmallocx) | (signedlong(ddata.v1e0266) << con(0x18));
  vtim_00.b_05 = con(0x0);
  SKW_QUEUE_TIMER(&vtim_00);
  ddata.savegames1.w0 += signedword(table1d6702[wordrg402]) * wordrg6;
  SKW_RECALC_LIGHT_LEVEL();
}

s_802* SKW_FIND_SPELL_BY_RUNES(t_text* eaxt)
{
  x32 longrg5;
  s_802* sptrrg1;

  if (eaxt[0x1] == NULLBYTE)
    return NULL;
  x16 wordrg402 = con(0x18);
  x32 longrg2 = con(0x0);
  do
  {
    longrg5 = unsignedlong(*eaxt);
    longrg5 <<= CUTX8(wordrg402);
    longrg2 |= longrg5;
    eaxt++;
    if (*eaxt == NULLBYTE)
      break;
    wordrg402 -= con(0x8);
  } while (wordrg402 >= con(0x0));
  sptrrg1 = table1d6802;
  x16 wordrg4 = con(0x22);
  for(;;)
  {
    wordrg4--;
    if (wordrg4 == con(0xffffffff))
      return NULL;
    if ((sptrrg1->l0 & con(0xff000000)) == con(0x0))
    {
      if ((longrg2 & con(0xffffff)) == sptrrg1->l0)
        return sptrrg1;
    }
    else
    {
      if (longrg2 == sptrrg1->l0)
        return sptrrg1;
    }
    sptrrg1++;
  }
}

void SKW_29ee_1d03(x16 eaxw)
{
  x16 wordrg5;
  t_text t_00[0x4]; // TODO probably even just t_00[0x2]

  wordrg5 = eaxw;
  SKW_29ee_00a3(false);
  if (ddata.v1e0b74 <= con(0x0) || (ddata.v1e0b62 & con(0x800)) == con(0x0))
    return;
  wordrg5 ^= ddata.v1e0b62;
  wordrg5 &= con(0xf);
  t_00[0x1] = NULLBYTE;
  for (x16 vw_04 = con(0x0); vw_04 < con(0x4); vw_04++)
  {
    x8 byterg1 = con(0x49);
    if ((wordrg5 & con(0x1)) != con(0x0))
      byterg1 = con(0x4a);
    wordrg5 = urshift(wordrg5, con(0x1));
    SKW_DRAW_ICON_PICT_ENTRY(con(0x14), byterg1, &ddata.v1d694c, ddata.v1e0ba4, vw_04 + con(0x65), con(0xffff));
    t_00[0x0] = CUTX8(vw_04) + con(0x72);
    SKW_DRAW_SIMPLE_STR(&ddata.v1d694c, unsignedword(ddata.paletteptr1[con(0x0)]), unsignedword(ddata.paletteptr1[con(0x2)]) | con(0x4000), vw_04 + con(0x69), t_00);
  }
}

void SKW_DRAW_MAJIC_MAP(x16 eaxw)
{
  x16 wordrg5;
  c_record* recoptrrg1;
  c_record* recoptrrg5;
  c_record* recoptrrg6;
  x16 vw_00;
  c_rect rc_04;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_18;
  x16 vw_1c;

  vw_00 = eaxw;
  ddata.v1e0ba4 = SKW_QUERY_CLS2_FROM_RECORD(eaxw);
  ddata.v1e0b62 |= con(0x90);
  recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(eaxw);
  vw_1c = urshift(recoptrrg5->u4.w, con(0xd));
  if (vw_1c != con(0x3))
  {
    ddata.v1e0b62 |= con(0x800);
    vw_1c = urshift(recoptrrg5->u4.w, con(0xd));
    if (vw_1c != con(0x0))
    {
      recoptrrg6 = SKW_GET_MISSILE_REF_OF_MINION(recoptrrg5->u2.w, eaxw);
      if (recoptrrg6 == NULL)
      {
        if ((ddata.v1e0b62 & con(0x20)) != con(0x0))
          ddata.v1e0b62 &= con(0xfffffbdf);
      }
      else
        ddata.v1e0b62 |= con(0x20);
    }
  }
  if ((ddata.v1e0b62 & con(0x400)) == con(0x0))
  {
    SKW_DRAW_ICON_PICT_ENTRY(con(0x14), con(0x10), &ddata.v1d694c, ddata.v1e0ba4, con(0x5c), con(0xffff));
    x16 wordrg21 = con(0x0);
    while (wordrg21 < ddata.v1e0b7a)
    {
      SKW_DRAW_CMD_SLOT(wordrg21, false);
      wordrg21++;
    }
    SKW_29ee_1d03(con(0x0));
    ddata.v1e0b62 |= con(0x400);
  }
  x16 wordrg3 = ddata.v1e0270;
  x16 vw_14 = ddata.v1e0272;
  vw_0c = (ddata.v1e0266);
  vw_18 = con(0x0);
  vw_10 = con(0x0);
  if (vw_1c != con(0x0) && vw_1c != con(0x3))
  {
    if ((recoptrrg5->u6.w & con(0xfc00)) != con(0xfffffc00))
      ddata.v1e0b62 |= con(0x40);
    if (vw_1c != con(0x1))
    {
      if ((ddata.v1e0b62 & con(0x40)) != con(0x0))
      {
        wordrg3 = recoptrrg5->u6.w & con(0x1f);
        vw_14 = ulrshift(recoptrrg5->u6.w, con(0x6), con(0xb));
        vw_0c = urshift(recoptrrg5->u6.w, con(0xa));
      }
    }
    else
    {
      if (recoptrrg6 != NULL)
      {
        wordrg3 = recoptrrg6->u4.w & con(0x1f);
        vw_14 = ulrshift(recoptrrg6->u4.w, con(0x6), con(0xb));
        vw_0c = urshift(recoptrrg6->u4.w, con(0xa));
        recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(recoptrrg5->u2.w);
        wordrg5 = SKW_QUERY_CREATURE_5x5_POS(recoptrrg1, ((unsignedword(table1d69a2[ulrshift(recoptrrg1->ue.w, con(0x6), con(0xe))]) + ddata.v1e0258) & con(0x3)));
        vw_10 = wordrg5 % con(0x5);
        vw_18 = wordrg5 / con(0x5);
        if (vw_10 > con(0x2))
        {
          x16 wordrg12 = (ddata.v1e0258 + con(0x1)) & con(0x3);
          wordrg3 += table1d27fc[wordrg12];
          vw_14 += table1d2804[wordrg12];
        }
        if (vw_18 > con(0x2))
        {
          x16 wordrg11 = (ddata.v1e0258 + con(0x2)) & con(0x3);
          wordrg3 += table1d27fc[wordrg11];
          vw_14 += table1d2804[wordrg11];
        }
        vw_10 = CUTX16(((2 * signedlong(ddata.v1d274e) + signedlong(ddata.v1d274a) + con(0x1)) / con(0x5)) * signedlong(table1d6998[vw_10]));
        vw_18 = CUTX16(((signedlong(ddata.v1d274c) + 2 * signedlong(ddata.v1d2750) + con(0x1)) / con(0x5)) * signedlong(table1d6998[vw_18]));
      }
      else
      {
        ddata.v1e0b62 &= con(0xffffffbf);
      }
    }
  }
  wordrg5 = vw_00;
  SKW_29ee_1946(wordrg5, vw_14, vw_0c, wordrg3, ddata.v1e0258, vw_10, vw_18, ddata.v1e0b62);
  if ((ddata.v1e0b62 & con(0x100)) == con(0x0))
    if ((ddata.v1e0b62 & con(0x200)) != con(0x0))
      return;
  SKW_0b36_105b(&ddata.v1d694c, con(0x0), SKW_QUERY_EXPANDED_RECT(con(0x63), &rc_04));
}

void SKW_ROTATE_SQUAD(x16 eaxw)
{
  c_hero* heroptrrg1;

  if (eaxw == ddata.v1e0258)
    return;
  x16 wordrg2 = eaxw - ddata.v1e0258;
  if (wordrg2 < con(0x0))
    wordrg2 += con(0x4);
  heroptrrg1 = ddata.hero;
  ui16 uwordrg4 = con(0x0);
  while (uwordrg4 < UI16CAST(ddata.savegamew2))
  {
    heroptrrg1->b1d = (heroptrrg1->b1d + CUTX8(wordrg2)) & con(0x3);
    heroptrrg1->b1c = (heroptrrg1->b1c + CUTX8(wordrg2)) & con(0x3);
    heroptrrg1++;
    uwordrg4++;
  }
  ddata.v1e0258 = eaxw;
  if (ddata.v1e0234)
  {
    wordrg2 += ddata.v1e0276;
    wordrg2 &= con(0x3);
    ddata.v1e0276 = wordrg2;
    ddata.v1e0264 = wordrg2;
  }
  else
    ddata.v1e0264 = eaxw;
}

x16 SKW_2c1d_028c(x16 eaxw, x16 ebxw, x16 edxw)
{
  x16 wordrg5;
  s_4bytearray v4ba_00;

  if (ddata.savegamew2 == con(0x0))
    return con(0xffffffff);
  wordrg5 = DM2_ABS(eaxw - ddata.v1e0280);
  if ((DM2_ABS(edxw - ddata.v1e027e) + wordrg5) > con(0x1))
    return con(0xffffffff);
  SKW_075f_1bc2(&v4ba_00, ddata.v1e027e, eaxw, ddata.v1e0280, edxw, ebxw);
  x16 wordrg4 = con(0x0);
  while (wordrg4 < con(0x4))
  {
    x16 wordrg11 = SKW_GET_PLAYER_AT_POSITION(unsignedword(v4ba_00.barr_00[wordrg4]));
    if (wordrg11 >= con(0x0))
      return wordrg11;
    wordrg4++;
  }
  return con(0xffffffff);
}

// was SKW_2c1d_038b
// TODO: return is a byte only after all
x16 SKW_RETRIEVE_ITEM_BONUS(x16 eaxw, x16 ebxw, x16 ecxw, x8 edxb)
{
  x16 wordrg1 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(eaxw, edxb);
  x16 wordrg4 = wordrg1;
  if (wordrg4 == con(0x0))
    return con(0x0);
  if ((wordrg4 & con(0x4000)) == con(0x0))
  {
    if (ebxw == con(0x0))
      if ((wordrg4 & con(0x8000)) == con(0x0))
        return con(0x0);
  }
  else if (ecxw != con(0xfffffffe) && ecxw != con(0x2) && ecxw == con(0x3))
    return con(0x0);
  x16 wordrg11 = signedword(CUTX8(wordrg1));
  if (ecxw >= con(0x0))
    return wordrg11;
  return -wordrg11;
}

// was SKW_2c1d_03e7
void SKW_PROCESS_ITEM_BONUS(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  c_hero* heroptrrg5;
  x16 vw_00;
  x16 vw_04;
  c_tim vtim_08;

  vw_04 = eaxw;
  vw_00 = edxw;
  if (eaxw < con(0x0) || edxw == con(0xffffffff))
    return;
  if (ebxw < con(0x1e))
  {
    x16 vw_24 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(edxw, con(0x0));
    if (ecxw != con(0x0))
      if ((vw_24 & con(0x10)) != con(0x0))
        SKW_RECALC_LIGHT_LEVEL();
    if ((vw_24 & con(0x2000)) != con(0x0))
    {
      heroptrrg5 = &ddata.hero[vw_04];
      x16 wordrg7 = SKW_IS_ITEM_FIT_FOR_EQUIP(vw_00, true, ebxw);
      bool vbool_14 = false;
      if (ecxw != con(0x0) && ecxw != con(0x3) && ecxw != con(0xfffffffe))
      {
        vw_24 = SKW_RETRIEVE_ITEM_BONUS(vw_00, wordrg7, ecxw, con(0x14));
        if (vw_24 != con(0x0))
        {
          if (ecxw != con(0x1) && ecxw != con(0xffffffff))
          {
            if (ecxw == con(0x2))
              heroptrrg5->w3e = DM2_BETWEEN_VALUE(con(0x0), con(0x3e7), vw_24 + heroptrrg5->w3e);
          }
          else
            heroptrrg5->w40 += vw_24;
          heroptrrg5->u32.b.b1 |= con(0x8);
        }
        for (x16 vw_20 = con(0x0); vw_20 <= con(0x6); vw_20++)
        {
          vw_24 = SKW_RETRIEVE_ITEM_BONUS(vw_00, wordrg7, ecxw, CUTX8(vw_20) + con(0x15));
          if (vw_24 != con(0x0))
          {
            if (ecxw != con(0x1) && ecxw != con(0xffffffff))
              SKW_2c1d_0300(heroptrrg5, vw_24, vw_20);
            else
              heroptrrg5->b58[vw_20] += CUTX8(vw_24);
            heroptrrg5->u32.b.b1 |= con(0x30);
          }
        }
      }
      for (x16 vw_20_1 = con(0x0); vw_20_1 < con(0x14); vw_20_1++)
      {
        vw_24 = SKW_RETRIEVE_ITEM_BONUS(vw_00, wordrg7, ecxw, CUTX8(vw_20_1) + con(0x1e));
        if (vw_24 != con(0x0))
        {
          heroptrrg5->baf[vw_20_1] += CUTX8(vw_24);
          heroptrrg5->u32.b.b1 |= con(0x20);
          vbool_14 = true;
        }
      }
      vw_24 = SKW_RETRIEVE_ITEM_BONUS(vw_00, wordrg7, ecxw, con(0x33));
      if (vw_24 != con(0x0))
      {
        heroptrrg5->b105[con(0x0)] += CUTX8(vw_24);
        vbool_14 = true;
      }
      if (ecxw != con(0x2) && ecxw != con(0xfffffffe) && ecxw != con(0x3))
      {
        x16 wordrg16 = SKW_RETRIEVE_ITEM_BONUS(vw_00, wordrg7, ecxw, con(0x32));
        if (wordrg16 != con(0x0))
        {
          ddata.savegames1.w0 += wordrg16;
          if (ecxw != con(0x0))
            SKW_RECALC_LIGHT_LEVEL();
        }
      }
      if (vbool_14)
      {
        if (ecxw == con(0x2))
        {
          vtim_08.b_04 = con(0xe);
          x32 longrg402 = ddata.longmallocx + unsignedlong(SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(vw_00, con(0x13)));
          longrg402 |= signedlong(ddata.v1e0266) << con(0x18);
          vtim_08.un.l_00 = longrg402;
          vtim_08.b_05 = CUTX8(vw_04);
          vtim_08.u.w_06 = urshift(vw_00 & con(0x3c00), con(0xa));
          vtim_08.v.b.b_08 = SKW_QUERY_CLS2_FROM_RECORD(vw_00);
          SKW_QUEUE_TIMER(&vtim_08);
        }
      }
    }
  }
  if (ecxw != con(0x0))
    SKW_CALC_PLAYER_WEIGHT(vw_04);
}

void SKW_PUT_ITEM_TO_PLAYER(x16 eaxw)
{
  if ((ddata.savegamewpc.w0 == con(0xffffffff)) || (ddata.hero[eaxw].w36 == con(0x0)))
    return;
  x16 wordrg2 = con(0xd);
  x16 wordrg3;
  while (wordrg3 = wordrg2, wordrg3 < con(0x1e))
  {
    if (ddata.hero[eaxw].warrc3[wordrg3] == con(0xffffffff))
      break;
    wordrg2++;
  }
  if (wordrg2 != con(0x1e))
  {
    x16 wordrg1 = SKW_REMOVE_OBJECT_FROM_HAND();
    if (wordrg1 != con(0xffffffff))
      SKW_EQUIP_ITEM_TO_HAND(eaxw, wordrg2, wordrg1);
  }
}

// was SKW_2c1d_0889
void SKW_CLICK_ITEM_SLOT(x16 eaxw)
{
  x16 wordrg5;
  x16 vw_00;

  x16 wordrg3 = eaxw;
  if (ddata.mouse_unk0e == con(0xffffffff))
    return;
  if (wordrg3 >= con(0x8))
  {
    wordrg3 -= con(0x8);
    if (wordrg3 < con(0x1e))
      wordrg5 = ddata.v1e0976;
    else
      wordrg5 = ddata.v1e0b74;
    wordrg5--;
  }
  else
  {
    if (ddata.v1e0288 != con(0x0))
      return;
    wordrg5 = wordrg3 >> con(0x1);
    if (UI16CAST(wordrg5) >= UI16CAST(ddata.savegamew2))
      return;
    if ((wordrg5 + con(0x1)) == ddata.v1e0976 || ddata.hero[wordrg5].w36 == con(0x0))
      return;
    wordrg3 &= con(0x1);
  }
  x16 wordrg6 = ddata.savegamewpc.w0;
  x16 wordrg2 = 2 * wordrg3;
  x16 wordrg11;
  if (wordrg3 < con(0x1e))
    wordrg11 = ddata.hero[wordrg5].warrc3[wordrg2 / 2];
  else
    wordrg11 = ddata.v1e095c[(wordrg2 - 0x3c) / 2]; // EVIL! offsetproblem1
  vw_00 = wordrg11;
  if (vw_00 == con(0xffffffff) && wordrg6 == vw_00)
    return;
  if (wordrg6 != con(0xffffffff))
    if (SKW_IS_ITEM_FIT_FOR_EQUIP(wordrg6, false, wordrg3) == con(0x0))
      return;
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  if (wordrg6 != con(0xffffffff))
    SKW_REMOVE_OBJECT_FROM_HAND();
  if (vw_00 != con(0xffffffff))
  {
    SKW_REMOVE_POSSESSION(wordrg5, wordrg3);
    SKW_TAKE_OBJECT(vw_00, true);
  }
  if (wordrg6 != con(0xffffffff))
    SKW_EQUIP_ITEM_TO_HAND(wordrg5, wordrg3, wordrg6);
  SKW_2e62_0cfa(false);
  if (wordrg3 == con(0xb) || wordrg3 == con(0x6) || wordrg3 == con(0xc) || (wordrg3 >= con(0x7) && wordrg3 <= con(0x9)) || wordrg3 >= con(0x1e))
  {
    ddata.v1e0b6c = con(0x1);
    SKW_UPDATE_RIGHT_PANEL(false);
  }
  if (ddata.v1e0254 != con(0x0))
  {
    ddata.v1e0254 = con(0x0);
    SKW_443c_0434();
  }
  SKW_FIRE_SHOW_MOUSE_CURSOR();
}

x16 SKW_2c1d_09d9(void)
{
  ui32 ulongrg5;

  ulongrg5 = con(0x0);
  x16 wordrg6 = con(0x0);
  while (wordrg6 < ddata.savegamew2)
  {
    x16 wordrg21 = con(0x0);
    x16 wordrg3;
    while (wordrg3 = wordrg21, wordrg3 <= con(0x3))
    {
      ulongrg5 += ddata.hero[wordrg6].l5f[wordrg3];
      wordrg21++;
    }
    wordrg6++;
  }
  x16 wordrg22 = con(0x1);
  for(;;)
  {
    if (ulongrg5 < UI32CAST(con(0x200)))
      return wordrg22;
    ulongrg5 >>= con(0x1);
    wordrg22++;
  }
}

void SKW_ADJUST_SKILLS(x16 eaxw, x16 ebxw, x16 edxw)
{
  c_hero* heroptrrg5;
  x16 vw_04;
  x16 vw_08;
  t_text t_0c[0x80];
  x8 vb_94;
  x8 vb_9c;

  vw_08 = eaxw;
  vw_04 = edxw;
  x32 vl_00 = unsignedlong(ebxw);
  if (edxw >= con(0x4) && edxw <= con(0xb))
    if (UI32CAST(ddata.longmallocx - con(0x96)) > UI32CAST(ddata.longv1d26a4))
      vl_00 = unsignedlong(urshift(ebxw, con(0x1)));
  if (vl_00 == con(0x0))
    return;
  x16 wordrg31 = urshift(ddata.v1e03c0->wc, con(0xc));
  if (wordrg31 != con(0x0))
    vl_00 *= unsignedlong(wordrg31);
  heroptrrg5 = &ddata.hero[vw_08];
  x16 wordrg6 = vw_04;
  if (wordrg6 >= con(0x4))
  {
    wordrg6 -= con(0x4);
    wordrg6 >>= con(0x2);
  }
  ui16 vuw_90 = SKW_QUERY_PLAYER_SKILL_LV(vw_08, false, wordrg6);
  if (vw_04 >= con(0x4))
    if ((ddata.longmallocx - con(0x28)) < ddata.longv1d26a4)
      vl_00 *= 2;
  heroptrrg5->l5f[vw_04] += vl_00;
  if (vw_04 >= con(0x4))
    heroptrrg5->l5f[wordrg6] += vl_00;

  // SPX DEBUG
	//if (SkCodeParam::bUsePowerDebug)
	{
		char message[64];
		sprintf((char*)message, "%s: +%d XP %s (XP=%d)!\n"
			, "CHAMPION", vl_00
			, getSkillName(vw_04), heroptrrg5->l5f[vw_04]);
		//SKW_DISPLAY_HINT_TEXT(glbChampionColor[player], message);
		SKW_DISPLAY_HINT_TEXT(table1d69d0[vw_08], message);
		
	}
  // SPX DEBUG

  x16 vw_8c = SKW_QUERY_PLAYER_SKILL_LV(vw_08, false, wordrg6);
  for(;;)
  {
    ui16 uwordrg40 = vw_8c;
    if (vuw_90++ >= uwordrg40)
      return;
    x16 vw_00 = uwordrg40;
    x8 byterg2 = CUTX8(SKW_RAND01());
    vb_94 = CUTX8(SKW_RAND01() + con(0x1));
    x8 byterg3 = CUTX8(SKW_RAND01());
    if (wordrg6 != con(0x2))
      byterg3 = CUTX8(vw_00) & byterg3;
    heroptrrg5->b4a[con(0x9)] += byterg3;
    x16 wordrg32 = heroptrrg5->u3c.w;
    heroptrrg5->b4a[con(0xd)] += CUTX8(SKW_RAND01()) & ~CUTX8(vw_00);
    if (wordrg6 <= con(0x3))
    {
      bool skip = false; // added
      switch (wordrg6)
      {
        case 0:
          vb_9c = con(0x6);
          wordrg32 = urshift(wordrg32, con(0x4));
          vw_00 *= 3;
          heroptrrg5->b4a[con(0x3)] += vb_94;
          heroptrrg5->b4a[con(0x5)] += byterg2;
          skip = true;
        break;

        case 1:
          vb_9c = con(0x7);
          wordrg32 /= con(0x15);
          vw_00 *= 2;
          heroptrrg5->b4a[con(0x3)] += byterg2;
          heroptrrg5->b4a[con(0x5)] += vb_94;
          skip = true;
        break;

        case 2:
          vb_9c = con(0x8);
          wordrg32 /= con(0x19);
          heroptrrg5->w40 += vw_00;
          vw_00 += urshift(vw_00 + con(0x1), con(0x1));
          heroptrrg5->b4a[con(0x7)] += byterg2;
        break;

        case 3:
          vb_9c = con(0x9);
          wordrg32 = urshift(wordrg32, con(0x5));
          heroptrrg5->w40 += urshift(vw_00, con(0x1)) + vw_00;
          heroptrrg5->b4a[con(0x7)] += vb_94;
        break;

        default: throw(THROW_DMABORT);
      }
      if (!skip)
      {
        heroptrrg5->w40 += DM2_MIN(CUTX16(SKW_RAND02()), vw_8c - con(0x1));
        if (heroptrrg5->w40 > con(0x384))
          heroptrrg5->w40 = con(0x384);
        heroptrrg5->b4a[con(0xb)] += CUTX8(SKW_RAND02());
      }
    }
    heroptrrg5->w38 += SKW_RAND16(urshift(vw_00, con(0x1)) + con(0x1)) + vw_00;
    if (heroptrrg5->w38 > con(0x3e7))
      heroptrrg5->w38 = con(0x3e7);
    heroptrrg5->u3c.w += SKW_RAND16(wordrg32 / 2 + con(0x1)) + wordrg32;
    if (heroptrrg5->u3c.w > con(0x270f))
      heroptrrg5->u3c.w = con(0x270f);
    heroptrrg5->u32.b.b1 |= con(0x38);
    ddata.v1e0ffa[wordrg6 + 4 * vw_08 + con(0x2)]++; // EVIL offset shifted by 2, was 0x1e0ffc
    SKW_DISPLAY_HINT_NEW_LINE();
    ddata.v1e098c = vw_08;
    SKW_DISPLAY_HINT_TEXT(table1d69d0[vw_08], SKW_QUERY_GDAT_TEXT(con(0x1), vb_9c, t_0c, con(0x0)));
  }
}

void SKW_2c1d_1bb5(x16 eaxw, x16 edxw)
{
  x16 wordrg5;
  c_hero* heroptrrg2;
  c_tim* timptrrg4;

  x16 wordrg3 = edxw;
  if (eaxw == con(0xffffffff))
    return;
  heroptrrg2 = &ddata.hero[eaxw];
  timptrrg4 = ddata.savegametimarrp;
  wordrg5 = con(0x0);
  while (wordrg5 < ddata.savegamew9)
  {
    if (timptrrg4->b_04 == con(0x4b))
    {
      x16 wordrg6 = unsignedword(timptrrg4->b_05);
      if (wordrg6 == eaxw)
      {
        x16 wordrg6 = timptrrg4->u.w_06;
        if (wordrg3 < wordrg6)
        {
          timptrrg4->u.w_06 = wordrg6 - wordrg3;
          heroptrrg2->w48 -= wordrg3;
          return;
        }
        wordrg3 -= wordrg6;
        heroptrrg2->w48 -= wordrg6;
        SKW_DELETE_TIMER(wordrg5);
        if (--heroptrrg2->b1f == con(0x0) || wordrg3 <= con(0x0))
          return;
      }
    }
    timptrrg4++;
    wordrg5++;
  }
}

void SKW_CALC_PLAYER_WEIGHT(x16 eaxw)
{
  x16 wordrg5;

  x16 wordrg2 = con(0x0);
  x16 wordrg4 = con(0x0);
  while (wordrg5 = wordrg4, wordrg5 < con(0x1e))
  {
    wordrg2 += SKW_QUERY_ITEM_WEIGHT(ddata.hero[eaxw].warrc3[wordrg5]);
    wordrg4++;
  }
  x16 wordrg3 = ddata.v1e0b74 - con(0x1);
  if (eaxw == wordrg3 && ddata.v1e0b76 < con(0x2) && SKW_IS_CONTAINER_CHEST(ddata.v1d67c2.w0[wordrg3 + con(0x1)][ddata.v1e0b76]))
  {
    x16 wordrg402 = con(0x0);
    while (wordrg402 < con(0x8))
    {
      wordrg2 += SKW_QUERY_ITEM_WEIGHT(ddata.v1e095c[wordrg402]);
      wordrg402++;
    }
  }
  ddata.hero[eaxw].wff = wordrg2;
  ddata.hero[eaxw].u32.b.b1 |= con(0x10);
}

void SKW_ADD_ITEM_TO_PLAYER(x16 eaxw, x16 edxw)
{
  ui16 uwordrg5;
  x16 vw_00;
  x16 vw_04;

  vw_00 = eaxw;
  vw_04 = con(0x0);
  for(;;)
  {
    if (vw_04 >= con(0x5))
      return;
    uwordrg5 = table1d6a31[vw_04].w0[con(0x0)];
    for (;; uwordrg5++) // TODO: ugly loopform because of continues
    {
      x16 wordrg3 = 6 * vw_04;
      if (uwordrg5 <= UI16CAST(table1d6a31[vw_04].w0[con(0x1)]))
      {
        if (ddata.hero[vw_00].warrc3[uwordrg5] != con(0xffffffff))
          continue;
        if (SKW_IS_ITEM_FIT_FOR_EQUIP(edxw, false, uwordrg5) == con(0x0))
          continue;
        x16 wordrg1 = table1d6a31[wordrg3 / 6].w0[con(0x2)];
        if (wordrg1 != con(0xffffffff))
        {
          if (urshift(edxw & con(0x3c00), con(0xa)) != wordrg1)
            continue;
        }
        SKW_EQUIP_ITEM_TO_HAND(vw_00, uwordrg5, edxw);
        return;
      }
      wordrg3 = vw_04 + con(0x1);
      vw_04 = wordrg3;
      break;
    }
  }
}

// TODO: make argument unsigned
bool SKW_IS_TILE_BLOCKED(x8 eaxb)
{
  ui8 ubyterg1 = eaxb;
  ui8 ubyterg4 = ubyterg1 >> con(0x5);
  if (ubyterg4 < con(0x4))
    return ubyterg4 == con(0x0);
  if (ubyterg4 <= con(0x4))
  {
    ubyterg1 &= con(0x7);
    return ubyterg1 != con(0x0) && ubyterg1 != con(0x1) && ubyterg1 != con(0x5);
  }
  if (ubyterg4 < con(0x6))
    return false;
  if (ubyterg4 <= con(0x6))
  {
    if ((ubyterg1 & con(0x4)) != con(0x0))
      return false;
    ubyterg1 &= con(0x1);
    return ubyterg1 == con(0x0);
  }
  return ubyterg4 == con(0x7);
}

bool SKW_32cb_01b6(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16* argpw0)
{
  c_record* recoptrrg2;
  s_d556* sd5ptrrg3;
  x16 vw_00;
  x16 vw_04;

  vw_04 = edxw;
  vw_00 = SKW_GET_CREATURE_AT(ebxw, ecxw);
  if (vw_00 == con(0xffffffff))
    return false;
  recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(vw_00);
  if (recoptrrg2->u4.b.b1 != con(0xffffffff))
    sd5ptrrg3 = &ddata.v1e089c[recoptrrg2->u4.b.b1];
  else
    sd5ptrrg3 = NULL;
  SKW_QUERY_CREATURE_PICST(con(0x3), recoptrrg2, sd5ptrrg3, table1d6b15[3], vw_00);
  x16 wordrg402 = ddata.v1e118e.w_30;
  ddata.v1e118e.w_30 = con(0xfffffffe);
  SKW_DRAW_TEMP_PICST();
  if (SKW_32cb_00f1(eaxw, wordrg402, vw_04))
  {
    if (ddata.v1e12da >= con(0xffffffb5))
    {
      if (ddata.v1e12da <= con(0x4b))
        *argpw0 = con(0x0);
      else
        *argpw0 = con(0x1);
    }
    else
      *argpw0 = con(0x3);
    return true;
  }
  return false;
}

bool SKW_32cb_0287(x16 eaxw, x16 ebxw, x16 edxw)
{
  x16 wordrg5;
  s_hex12* h12ptrrg4;
  bool vbool_00;
  s_hex12* ps_04;
  x16 vw_08;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_14;

  vw_10 = edxw;
  vw_0c = ebxw;
  vbool_00 = false;
  wordrg5 = eaxw;
  if (wordrg5 > con(0x3))
    return false;
  ps_04 = ddata.v1e1044;
  ddata.v1e1044 = UPCAST(s_hex12, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0x48)));
  vw_14 = ddata.v1e12cc;
  vw_08 = ddata.v1e12ca;
  SKW_CALC_VECTOR_W_DIR(ddata.v1e12c8, table1d6ad0[wordrg5][0], &vw_14, table1d6ad0[wordrg5][1], &vw_08);
  ddata.v1e1044[wordrg5].b_00 = CUTX8(vw_14);
  ddata.v1e1044[wordrg5].b_01 = CUTX8(vw_08);
  h12ptrrg4 = &ddata.v1e1044[wordrg5];
  SKW_SUMMARIZE_STONE_ROOM(&h12ptrrg4->s_02, vw_14, vw_08, ddata.v1e12c8);
  x16 wordrg1 = SKW_32cb_15b8(wordrg5, false, signedword(table1d6afe[wordrg5]));
  if (wordrg1 >= con(0x0))
    vbool_00 = SKW_32cb_00f1(vw_10, wordrg1, vw_0c);
  DM2_DEALLOC_LOBIGPOOL(con(0x48));
  ddata.v1e1044 = ps_04;
  if (eaxw == con(0x3))
    return true;
  if (ddata.v1e118e.s_00.w_12 >= con(0x20))
  {
    if (ddata.v1e118e.s_00.w_14 >= con(0x20))
      return vbool_00;
  }
  return true;
}

void SKW_DRAW_DUNGEON_GRAPHIC(x8 eaxb, x8 ebxb, x16 ecxw, x8 edxb, x16 argw0, x16 argw1)
{
  c_image vs_00;

  SKW_QUERY_GDAT_SUMMARY_IMAGE(&vs_00, edxb, ebxb, eaxb);
  R_B073(vs_00.carr_3a, argw0, con(0xffffffff), CUTX8(ddata.v1e12d2), &vs_00.w_38);
  vs_00.w_30 = argw0;
  vs_00.w_18 = ecxw;
  vs_00.pg_2c = ddata.bitmapptr;
  vs_00.w_32 = argw1;
  if (ddata.v1e025c != con(0x0))
  {
    if (ecxw != con(0x2bc))
    {
      if (ecxw == con(0x2bd))
        vs_00.w_22 += ddata.v1d2710;
    }
    else
      vs_00.w_22 += ddata.v1d270e;
  }
  SKW_DRAW_PICST(SKW_QUERY_PICST_IT(&vs_00));
}

void SKW_QUERY_TEMP_PICST(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2, x16 argw3, x16 argw4, x16 argw5, x8 argb6, x8 argb7, x8 argb8)
{
  x16 wordrg5;

  wordrg5 = edxw;
  SKW_QUERY_GDAT_SUMMARY_IMAGE(&ddata.v1e118e, argb7, argb8, argb6);
  if (wordrg5 != ebxw)
  {
    bool skip = false; // added
    x8 byterg1;
    if (argw1 == con(0x2))
      byterg1 = con(0x14);
    else if (argw1 == con(0x3))
      byterg1 = con(0x15);
    else
      skip = true;
    if (!skip)
    {
      x16 wordrg1 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(argb6, con(0xb), byterg1, argb7);
      if (wordrg1 != con(0x0))
      {
        x16 wordrg40 = urshift(wordrg1, con(0x8)) << con(0x7);
        x16 wordrg2 = unsignedword(CUTX8(wordrg1));
        wordrg5 = ((wordrg40 / wordrg2) + con(0x1)) >> con(0x1);
      }
    }
  }
  ddata.v1e118e.w_32 = eaxw;
  ddata.v1e118e.w_34 = wordrg5;
  ddata.v1e118e.w_36 = ebxw;
  ddata.v1e118e.w_1c += ecxw;
  ddata.v1e118e.w_1e += argw0;
  ddata.v1e118e.w_18 = argw2;
  ddata.v1e118e.w_1a = argw3;
  ddata.v1e118e.pg_2c = ddata.bitmapptr;
  ddata.v1e118e.w_30 = argw4;
  SKW_32cb_0804(ddata.v1e118e.carr_3a, argw4, argw5, argw1, &ddata.v1e118e.w_38);
  SKW_QUERY_PICST_IT(&ddata.v1e118e);
}

void SKW_QUERY_RAINFALL_PARAM(x8* eaxpb, x16* edxpw)
{
  *edxpw = (ddata.v1e1483 - ddata.v1e12c8) & con(0x3);
  x8 byterg3;
  bool skip = false; // added
  if (*edxpw != con(0x0))
  {
    if (*edxpw != con(0x2))
    {
      skip = true;
      byterg3 = con(0x6d);
    }
  }
  if (!skip)
    byterg3 = con(0x71);
  x8 byterg4 = con(0x0);
  if (ddata.v1e1482 < con(0x80))
  {
    if (ddata.v1e1482 < con(0x40))
    {
      if (ddata.v1e1482 < con(0x10))
      {
        // if (RG1Blo != con(0x0)) TODO: useless command
        {
          // cmp32(RG1L, con(0x10)); TODO: useless command
        }
      }
      else
        byterg4 = con(0x1);
    }
    else
      byterg4 = con(0x2);
  }
  else
    byterg4 = con(0x3);
  byterg3 += byterg4;
  *eaxpb = byterg3;
}

bool SKW_IS_DISTINCTIVE_ITEM_ON_ACTUATOR(c_record* eaxprec, bool ebxbool, x16 edxw)
{
  x16 wordrg4 = urshift(eaxprec->u6.w , con(0xb));
  x16 wordrg1 = SKW_GET_TILE_RECORD_LINK(ulrshift(eaxprec->u6.w, con(0x5), con(0xb)), wordrg4);
  for(;;)
  {
    x16 wordrg2 = wordrg1;
    if (wordrg2 == con(0xfffffffe))
      return false;
    x16 wordrg13 = urshift(wordrg2 & con(0x3c00), con(0xa));
    if (!ebxbool || wordrg13 < con(0x5) || wordrg13 > con(0xa))
    {
      if (wordrg13 == con(0x4))
      {
        x16 wordrg11 = SKW_GET_ADDRESS_OF_RECORD(wordrg2)->u2.w;
        while (wordrg11 != con(0xfffffffe))
        {
          x16 wordrg40 = wordrg11;
          if (SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg40) == edxw)
            return true;
          wordrg11 = SKW_GET_NEXT_RECORD_LINK(wordrg40);
        }
      }
    }
    else
    {
      if (SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg2) == edxw)
        return true;
    }
    wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg2);
  }
}

void SKW_DRAW_WALL(x16 eaxw)
{
  x16 wordrg5;
  x16 vw_04;
  x16 vw_08;
  x8 vb_0c;
  x8 vb_10;
  x8 vb_14;

  vb_0c = ddata.v1d6c02;
  vw_04 = ddata.v1d6c00;
  vb_14 = table1d6afe[eaxw];
  vw_08 = signedword(table1d6b15[eaxw]);
  vb_10 = ((table1d6b15[eaxw] > con(0x1)) ? 1 : 0) + con(0x6e);
  x16 wordrg6 = con(0x0);
  wordrg5 = con(0x0);
  x16 wordrg19 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x8), con(0xb), vb_10, ddata.v1d6c02);
  x8 byterg2 = CUTX8(wordrg19);
  if (vw_08 <= con(0x1))
  {
    if (vw_08 > con(0x0))
    {
      if (ddata.v1e1044[con(0x1)].s_02.warr_00[con(0x0)] == con(0x0))
        wordrg5 = urshift(wordrg19, con(0x8));
      if (ddata.v1e1044[con(0x2)].s_02.warr_00[con(0x0)] == con(0x0))
        wordrg6 = unsignedword(byterg2);
    }
  }
  else
  {
    if (ddata.v1e1044[con(0x4)].s_02.warr_00[con(0x0)] == con(0x0))
      wordrg5 = urshift(wordrg19, con(0x8));
    if (ddata.v1e1044[con(0x5)].s_02.warr_00[con(0x0)] == con(0x0))
      wordrg6 = unsignedword(byterg2);
  }
  if ((wordrg5 != con(0x0)) || (wordrg6 != con(0x0)))
    SKW_TRIM_BLIT_RECT(wordrg5, con(0x0), wordrg6, con(0x0));
  wordrg5 = (vb_14 > con(0x0)) ? 1 : 0;
  if (eaxw < con(0x10))
  {
    if (ddata.v1e12d0 == con(0x0))
      vb_10 = CUTX8(eaxw) + con(0x22);
    else
    {
      vb_10 = table1d6b2c[eaxw] + con(0xffffffb0);
      if (!SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0x8), con(0x1), vb_10, vb_0c))
      {
        if (vb_14 == con(0x0))
          wordrg5 = con(0x1);
        vb_10 -= con(0xffffff8e);
      }
    }
  }
  else
  {
    if (vb_14 == con(0xfffffffe) || vb_14 == con(0x2))
      wordrg5 = con(0x0);
    wordrg5 ^= ddata.v1e12d0;
    vb_10 = con(0x32);
  }
  SKW_QUERY_TEMP_PICST(wordrg5, con(0x40), con(0x0), con(0x40), con(0x0), ddata.v1e025c == con(0x0) ? con(0x0) : -vw_08, eaxw + con(0x2be), con(0xffffffff), vw_04, con(0xffff), con(0x8), vb_0c, vb_10);
  SKW_DRAW_TEMP_PICST();
  ddata.v1e01d0 = con(0x0);
}

// was SKW_32cb_50fe
void SKW_DRAW_WALL_TILE(ui16 eaxuw)
{
  SKW_DRAW_WALL(eaxuw);
  ui8 ubyterg1 = table1d7012[eaxuw];
  x16 wordrg4;
  if (ubyterg1 < con(0x2))
  {
    if (ubyterg1 != con(0x1))
      return;
    wordrg4 = con(0x0);
  }
  else
  {
    if (ubyterg1 > con(0x2))
    {
      if (ubyterg1 != con(0x3))
        return;
      SKW_32cb_15b8(eaxuw, true, con(0x0));
    }
    wordrg4 = signedlong(table1d6afe[eaxuw]);
  }
  SKW_32cb_15b8(eaxuw, true, wordrg4);
}

// was SKW_32cb_5598
bool SKW_ENVIRONMENT_SET_DISTANT_ELEMENT(s_xabytes* eaxpxa, x16 ebxw, x16 ecxw, t_text* edxt, x16 argw0)
{
  x16 vw_00;
  x16 vw_04;
  x16 vw_08;

  vw_00 = ebxw;
  eaxpxa->w6 = con(0x0);
  eaxpxa->w4 = eaxpxa->w6;
  x16 wordrg1 = SKW_QUERY_CMDSTR_TEXT(edxt, v1d1128);
  if (wordrg1 == con(0x0))
  {
    eaxpxa->b8 = con(0x40);
    eaxpxa->b9 = con(0x40);
    return true;
  }
  if (wordrg1 != con(0x1))
    return false;
  vw_04 = ecxw + ddata.v1e03c0->b4[con(0x2)];
  vw_08 = argw0 + unsignedword(ddata.v1e03c0->b4[con(0x3)]);
  ui16 uwordrg3 = SKW_32cb_54ce(vw_00, &vw_08, SKW_QUERY_CMDSTR_TEXT(edxt, v1d112b), &vw_04, SKW_QUERY_CMDSTR_TEXT(edxt, v1d112e));
  if (vw_08 < con(0x1))
    return false;
  x32 longrg40 = unsignedlong(DM2_MAX(con(0x1), con(0x40) - uwordrg3 + SKW_QUERY_CMDSTR_TEXT(edxt, v1d1131))) << con(0x7);
  x8 byterg2 = (CUTX8((longrg40 - jul_shl32(longrg40, con(0x6))) >> con(0x6)) + con(0x1)) / 2;
  eaxpxa->w4 = 210 * vw_04 / uwordrg3;
  eaxpxa->b8 = byterg2;
  eaxpxa->b9 = byterg2;
  return true;
}

void SKW_32cb_5a8f(void)
{
  x32 longrg5; // confirmed long
  s_hex12* h12ptrrg1;
  x32 longrg2;
  x32 longrg3;

  longrg5 = con(0x0);
  longrg2 = con(0x0);
  x16 wordrg41 = con(0x0);
  while (wordrg41 < con(0x10))
  {
    if (ddata.v1e1044[wordrg41].s_02.warr_00[con(0x0)] == con(0x0))
      longrg2 |= con(0x1) << CUTX8(wordrg41);
    wordrg41++;
  }
  longrg5 |= table1d7042[(longrg2 >> con(0x1)) & con(0x3)];
  x32 longrg11;
  if ((longrg2 & con(0x8)) == con(0x0))
    longrg11 = con(0x0);
  else
    longrg11 = longrg2 & con(0x6);
  longrg3 = longrg2;
  longrg3 = UI32CAST(longrg3) >> con(0x3);
  longrg11 |= longrg3 & con(0x7);
  longrg5 |= table1d7052[longrg11];
  x32 longrg1;
  if ((longrg2 & con(0x40)) == con(0x0))
    longrg1 = con(0x0);
  else
    longrg1 = longrg3 & con(0x6);
  longrg3 = longrg2;
  longrg3 = UI32CAST(longrg3) >> con(0x6);
  longrg1 |= longrg3 & con(0x7);
  longrg5 |= table1d7072[longrg1];
  if ((longrg2 & con(0x800)) == con(0x0))
    longrg3 = con(0x0);
  else
    longrg3 &= con(0x6);
  longrg5 |= table1d7092[((longrg2 >> con(0xb)) & con(0x7)) | longrg3];
  longrg2 |= longrg5;
  if ((longrg2 & con(0x1000)) != con(0x0))
    if ((longrg2 & con(0x4000)) != con(0x0) || (longrg2 & con(0x200)) != con(0x0))
      longrg5 |= con(0x280000);
  if ((longrg2 & con(0x2000)) != con(0x0))
    if ((longrg2 & con(0x8000)) != con(0x0) || (longrg2 &= con(0xffff0400), CUTX16(longrg2) != con(0x0)))
      longrg5 |= con(0x500000);
  longrg3 = con(0x1);
  x16 wordrg42 = con(0x0);
  for(;;)
  {
    longrg2 = signedlong(wordrg42);
    if (longrg2 > con(0x16))
      return;
    if ((longrg3 & longrg5) != con(0x0))
    {
      h12ptrrg1 = &ddata.v1e1044[longrg2];
      if (h12ptrrg1->s_02.warr_00[con(0x0)] == con(0x0))
        h12ptrrg1->s_02.warr_00[con(0x0)] = con(0x7);
    }
    longrg3 += longrg3;
    wordrg42++;
  }
}

void SKW_32cb_5c67(void)
{
  s_hex12* h12ptrrg11;
  s_hex12* h12ptrrg12;
  c_5bytes vs_00;

  for (x16 wordrg3 = con(0x0); ; wordrg3++)
  {
    if (wordrg3 >= con(0x15))
      return;
    h12ptrrg11 = &ddata.v1e1044[wordrg3];
    if (h12ptrrg11->s_02.warr_00[con(0x0)] == con(0x1))
    {
      if (SKW_GET_TELEPORTER_DETAIL(&vs_00, signedword(h12ptrrg11->b_01), signedword(h12ptrrg11->b_00)))
      {
        x16 wordrg402 = con(0x0);
        while (wordrg402 < con(0x4))
        {
          x8 byterg1 = table1d6a74[wordrg3].barr_00[wordrg402];
          if (!(byterg1 & 0x80)) // PROBLEM! if < 0 continue (SKWIN)
          {
            h12ptrrg12 = &ddata.v1e1044[byterg1];
            if (h12ptrrg12->s_02.warr_00[con(0x0)] == con(0x7))
            {
              h12ptrrg12->s_02.warr_00[con(0x0)] = con(0x0);
            }
          }
          wordrg402++;
        }
      }
    }
  }
}

bool SKW_38c8_0224(bool eaxbool, x32 edxl)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("SKW_38c8_0224");
  x16 wordrg5;

  x16 vw_10 = lextended(ddata.v1e0266);
  x16 vw_08 = lextended(ddata.v1e0270);
  x16 vw_04 = lextended(ddata.v1e0272);
  dm2_dballochandler.v1e0a84 = true;
  i32 longrg6 = con(0x7fffffff); // confirmed long
  wordrg5 = con(0x0);
  x16 vw_0c; // TODO: uninitialized possible?
  while (wordrg5 < unsignedword(CUTX8(ddata.savegamep4->w0[con(0x2)]))) // TODO: check the cut!!!	// dunHeader->nMaps
  {
    if ((wordrg5 & con(0x3)) == con(0x0))
    {
      x32 longrg2 = unsignedlong(CUTX8(ddata.savegamep4->w0[con(0x2)]));
      x32 longrg1 = (signedlong(dm2_dballochandler.v1e13fa) - 1) * longrg2; // BUGFIX soundproblem
      longrg2 *= signedlong(dm2_dballochandler.v1e13f8);
      longrg1 = 500 * (signedlong(wordrg5) + con(0x1) + longrg1) / longrg2;
	  // skwin ((i32(dunHeader->nMaps * (_4976_5be8 -1) +si +1) * 500) / i32(dunHeader->nMaps * _4976_5bf4));
      SKW_DRAW_DIALOGUE_PROGRESS(longrg1);
    }
    SKW_LOAD_NEWMAP(con(0x0), wordrg5, false, con(0x0));
    if (dm2_dballochandler.v1e0a74 < con(0x0) && longrg6 > dm2_dballochandler.v1e0a74)
    {
      longrg6 = dm2_dballochandler.v1e0a74;
      vw_0c = wordrg5;
    }
    i32 longrg1 = dm2_dballochandler.v1e0a8c - edxl;	// x16 longrg1
    if (longrg1 < longrg6)
    {
      longrg6 = longrg1;
      vw_0c = wordrg5;
    }
    if ((longrg6 < con(0x0) && !eaxbool) || dm2_dballochandler.v1e13fe[0] != con(0x0))
      break;
    wordrg5++;
  }
  if (longrg6 > ddata.longv1d70c5)
  {
    ddata.longv1d70c5 = longrg6;
    ddata.v1d70c9 = vw_0c;
  }
  if (eaxbool)
    if (longrg6 < con(0x0))
    {
      ddata.longv1d70c5 = longrg6;
      ddata.v1d70c9 = vw_0c;
      SKW_0aaf_0067(SKW_0aaf_02f8(con(0x2), con(0x0)));
      SKW_SK_PREPARE_EXIT();
    }
  dm2_dballochandler.v1e0a84 = false;
  ddata.v1e0266 = vw_10;	// SPX: glbPlayerMap ????
  ddata.v1e0270 = vw_08;
  ddata.v1e0272 = vw_04;
  if (longrg6 >= con(0x0))
  {
    if (dm2_dballochandler.v1e13fe[2] != con(0x0))
      SKW_0aaf_02f8(con(0xe), con(0x20));
    SKW_DRAW_DIALOGUE_PROGRESS(con(0x1f4));
  }
SPX_DEBUG_POP;
  return longrg6 >= con(0x0);
}

x16 SKW_3929_04e2(const t_text* eaxt, x16* ebxpw, x16 ecxw, t_text* edxt)
{
  t_text textrg5;
  t_text* tptrrg7;
  const t_text* tptrrg3;
  const t_text* tp_00;
  x16 vw_08;
  x16 vw_10;
  x16 vw_14;

  tp_00 = eaxt;
  tptrrg7 = edxt;
  vw_10 = ecxw;
  x16 wordrg4 = *ebxpw;
  x16 wordrg1 = con(0x0);
  x16 wordrg2 = con(0x0);
  vw_08 = con(0xffffffff);
  vw_14 = con(0x0);
  x16 vw_0c = con(0x0);
  for(;;)
  {
    tptrrg7[wordrg2] = NULLBYTE;
    tptrrg3 = &tp_00[wordrg4];
    textrg5 = *tptrrg3;
    if ((textrg5 == con(0xa)) || (*tptrrg3 == NULLBYTE))
      break;
    if (textrg5 == ' ')
    {
      vw_08 = wordrg4;
      vw_14 = wordrg2;
      vw_0c = wordrg1;
    }
    if ((wordrg1 + C_strmetrics.charwidth) > vw_10)
    {
      if (vw_08 == con(0xffffffff))
        break;
      *ebxpw = vw_08 + con(0x1);
      tptrrg7[vw_14] = NULLBYTE;
      return vw_0c;
    }
    if (wordrg1 == con(0x0))
      wordrg1 = -C_strmetrics.xofs;
    wordrg1 += C_strmetrics.charwidth;
    tptrrg7[wordrg2++] = tp_00[wordrg4++];
  }
  *ebxpw = wordrg4;
  return wordrg1;
}

// was SKW_3a15_008e
void SKW___SORT_TIMERS(void)
{
  c_tim* timptrrg3;
  c_tim* timptrrg5;
  c_tim* timptrrg6;
  x16* wptrrg4;
  x16 vw_04;
  x16 vw_0c;

  vw_0c = ddata.savegamew5;
  if (ddata.savegamew5 != con(0x0))
  {
    vw_04 = con(0x0);
    x16 wordrg3 = vw_0c;
    while (wordrg3 > UI16CAST(vw_04))
    {
      ddata.v1e1424[vw_04] = vw_04;
      vw_04 = vw_04 + con(0x1);
    }
    x16 wordrg11 = wordrg3;
    if (wordrg11 != con(0x1))
    {
      wordrg11 -= con(0x2);
      wordrg11 >>= con(0x1);
      x16 vw_00 = wordrg11;
      while (vw_00 >= con(0x0))
      {
        x16 wordrg6 = vw_00;
        vw_04 = wordrg6;
        timptrrg5 = &ddata.savegametimarrp[ddata.v1e1424[wordrg6]];
        for(;;)
        {
          x16 wordrg2 = 2 * vw_04 + con(0x1);
          if (UI16CAST(wordrg2) >= UI16CAST(vw_0c))
          {
            vw_00--;
            break;
          }
          wordrg6 = wordrg2;
          wptrrg4 = &ddata.v1e1424[wordrg6];
          timptrrg3 = &ddata.savegametimarrp[*wptrrg4];
          wordrg6++;
          if (wordrg6 >= vw_0c)
          {
            if (SKW_3a15_0381(timptrrg5, timptrrg3))
            {
              vw_00--;
              break;
            }
          }
          else
          {
            timptrrg6 = &ddata.savegametimarrp[*(wptrrg4 + con(0x1))];
            x16 vw_10 = wordrg2 + con(0x1);
            if (!SKW_3a15_0381(timptrrg5, timptrrg3))
            {
              if (SKW_3a15_0381(timptrrg6, timptrrg3))
                wordrg2 = vw_10;
            }
            else
            {
              if (SKW_3a15_0381(timptrrg5, timptrrg6))
              {
                vw_00--;
                break;
              }
              wordrg2 = vw_10;
            }
          }
          x16 vw_08 = ddata.v1e1424[vw_04];
          ddata.v1e1424[vw_04] = ddata.v1e1424[wordrg2];
          ddata.v1e1424[wordrg2] = vw_08; // BUGFIX 4.1/2014
          vw_04 = wordrg2;
        }
      }
    }
  }
  ddata.v1e1428 = con(0xffffffff);
  SKW_REARRANGE_TIMERLIST();
}

void SKW_3a15_0486(x16 eaxw)
{
  x16* wptrrg1;
  x16* wptrrg6;
  c_tim* timptrrg5;
  c_tim* ebpptim;
  bool flag;
  x16 vw_04;

  ddata.v1e1428 = con(0xffffffff);
  x16 wordrg13 = ddata.savegamew5;
  flag = --wordrg13 == con(0x0);
  vw_04 = wordrg13;
  if (flag)
    return;
  x16 wordrg12 = ddata.v1e1424[eaxw];
  x16 vw_08 = wordrg12;
  timptrrg5 = &ddata.savegametimarrp[wordrg12];
  bool vbool_00 = false;
  x16 wordrg3 = eaxw;
  for(;;)
  {
    if (wordrg3 == con(0x0))
      break;
    x16 wordrg6 = wordrg3;
    x16 wordrg7 = (wordrg6 - con(0x1)) / 2;
    if (!SKW_3a15_0381(timptrrg5, &ddata.savegametimarrp[ddata.v1e1424[wordrg7]]))
      break;
    ddata.v1e1424[wordrg6] = ddata.v1e1424[wordrg7];
    wordrg3 = wordrg7;
    vbool_00 = true;
  }
  if (!vbool_00)
  {
    vw_04 = (vw_04 - 1) / 2;
    for(;;)
    {
      if (wordrg3 > UI16CAST(vw_04))
        break;
      x16 wordrg2 = 2 * wordrg3 + con(0x1);
      if ((wordrg2 + con(0x1)) < ddata.savegamew5)
      {
        ebpptim = &ddata.savegametimarrp[ddata.v1e1424[wordrg2]];
        if (SKW_3a15_0381(&ddata.savegametimarrp[ddata.v1e1424[wordrg2 + con(0x1)]], ebpptim))
          wordrg2++;
      }
      if (!SKW_3a15_0381(&ddata.savegametimarrp[ddata.v1e1424[wordrg2]], timptrrg5))
        break;
      wptrrg1 = ddata.v1e1424;
      wptrrg6 = &wptrrg1[wordrg2];
      wptrrg1 += unsignedlong(wordrg3);
      *wptrrg1 = *wptrrg6;
      wordrg3 = wordrg2;
    }
  }
  ddata.v1e1424[wordrg3] = vw_08;
}

void SKW_ACTUATE_DOOR(c_tim* eaxptim)
{
  x8 byterg5;
  c_record* recoptrrg1;
  c_record* recoptrrg3;

  byterg5 = ddata.savegamepp1[eaxptim->u.b.b_06][eaxptim->u.b.b_07] & con(0x7);
  if (byterg5 == con(0x5))
    return;
  recoptrrg1 = SKW_GET_ADDRESS_OF_TILE_RECORD(unsignedword(eaxptim->u.b.b_06), unsignedword(eaxptim->u.b.b_07));
  recoptrrg3 = recoptrrg1;
  if ((recoptrrg1->u2.b.b1 & con(0x4)) == con(0x0))
  {
    x16 wordrg402 = con(0x0);
    bool skip = false; // added
    if (byterg5 != con(0x0))
    {
      if (byterg5 != con(0x4))
      {
        wordrg402 = (eaxptim->v.b.b_09 == con(0x0)) ? 1 : 0;
        recoptrrg1->u2.w &= con(0xfffffdff);
        wordrg402 <<= con(0x9);
        recoptrrg1->u2.w |= wordrg402;
      }
      else
      {
        if ((eaxptim->v.b.b_09 != con(0x0)) && (eaxptim->v.b.b_09 != con(0x2)))
          skip = true;
        else
          recoptrrg1->u2.b.b1 |= con(0x2);
      }
    }
    else
    {
      if ((eaxptim->v.b.b_09 != con(0x1)) && (eaxptim->v.b.b_09 != con(0x2)))
        skip = true;
      else
        recoptrrg1->u2.b.b1 &= con(0xfffffffd);
    }
    if (!skip)
      wordrg402 = con(0x1);
    wordrg402 &= con(0x1);
    recoptrrg1->u2.b.b1 &= con(0xfffffffb);
    recoptrrg1->u2.w |= (wordrg402 << con(0xa));
    if (wordrg402 != con(0x0))
    {
      if ((recoptrrg1->u2.b.b1 & con(0x2)) == con(0x0))
      {
        recoptrrg1->u2.b.b1 &= con(0xffffffef);
      }
      eaxptim->b_04 = con(0x1);
      SKW_QUEUE_TIMER(eaxptim);
    }
    return;
  }
  x16 wordrg1 = SKW_3a15_1da8(unsignedword(eaxptim->v.b.b_09), ulrshift(recoptrrg1->u2.w, con(0x6), con(0xf))) & con(0x1);
  recoptrrg3->u2.b.b1 &= con(0xfffffffd);
  recoptrrg3->u2.w |= wordrg1 << con(0x9);
  if (wordrg1 == con(0x0))
    recoptrrg3->u2.b.b1 &= con(0xffffffef);
}

void SKW_ACTIVATE_SHOOTER(c_record* eaxprec, c_tim* edxptim)
{
  x16 wordrg5;
  x8 vb_20;
  x16 vw_24;
  x8 vb_30;

  x16 vw_2c = unsignedword(edxptim->u.b.b_06);
  x16 vw_28 = unsignedword(edxptim->u.b.b_07);
  x16 vw_1c = unsignedword(edxptim->v.b.b_08);
  vw_24 = (unsignedword(edxptim->v.b.b_08) + con(0x2)) & con(0x3);
  x16 wordrg1 = eaxprec->u2.w & con(0x7f);
  x16 wordrg3 = urshift(eaxprec->u2.w, con(0x7));
  vb_20 = CUTX8(urshift(eaxprec->u6.w, con(0x4)) & con(0xff));
  vb_30 = CUTX8(urshift(eaxprec->u6.w, con(0xc)));
  bool boolrg2 = wordrg1 == con(0x7) || wordrg1 == con(0x8) || wordrg1 == con(0xe);
  bool boolrg7 = boolrg2;
  wordrg5 = wordrg3 + con(0xff80);
  x16 wordrg6 = wordrg5;
  if (wordrg1 != con(0xe) && wordrg1 != con(0xf))
  {
    if (wordrg1 != con(0x8) && wordrg1 != con(0xa))
    {
      wordrg6 = SKW_ALLOC_NEW_DBITEM(wordrg3);
      if (wordrg6 == con(0xffffffff))
        return;
      if (!boolrg2)
      {
        wordrg5 = SKW_ALLOC_NEW_DBITEM(wordrg3);
        if (wordrg5 == con(0xffffffff))
          boolrg7 = true;
      }
    }
  }
  else
  {
    x16 wordrg100 = SKW_GET_TILE_RECORD_LINK(vw_2c, vw_28);
    for(;;)
    {
      wordrg6 = wordrg100;
      if (wordrg6 == con(0xfffffffe))
        break;
      x16 wordrg101 = urshift(wordrg6, con(0xe));
      if (urshift(wordrg6 & con(0x3c00), con(0xa)) > con(0x3) && (wordrg101 == vw_1c || wordrg101 == ((vw_1c + con(0x1)) & con(0x3))))
        break;
      wordrg100 = SKW_GET_NEXT_RECORD_LINK(wordrg6);
    }
    if (wordrg6 == con(0xfffffffe))
      return;
    SKW_CUT_RECORD_FROM(wordrg6, vw_2c, vw_28, NULL);
    if (!boolrg7)
    {
      wordrg5 = SKW_GET_TILE_RECORD_LINK(vw_2c, vw_28);
      x16 wordrg2 = vw_1c;
      for(;;)
      {
        if (wordrg5 == con(0xfffffffe))
          break;
        x16 wordrg14 = urshift(wordrg5, con(0xe));
        if (urshift(wordrg5 & con(0x3c00), con(0xa)) > con(0x3) && (wordrg14 == wordrg2 || wordrg14 == ((wordrg2 + 1) & con(0x3))))
          break;
        wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg5);
      }
      if (wordrg5 == con(0xfffffffe))
        boolrg7 = true;
      else
        SKW_CUT_RECORD_FROM(wordrg5, vw_2c, vw_28, NULL);
    }
  }
  if (boolrg7)
    vw_24 = (SKW_RAND01() + vw_24) & con(0x3);
  vw_2c += table1d27fc[vw_1c];
  vw_28 += table1d2804[vw_1c];
  ddata.v1e01a8 = con(0x1);
  SKW_SHOOT_ITEM(wordrg6, vw_28, vw_24, vw_2c, CUTX8(vw_1c), vb_20, con(0x64), vb_30);
  if (!boolrg7)
    SKW_SHOOT_ITEM(wordrg5, vw_28, (vw_24 + con(0x1)) & con(0x3), vw_2c, CUTX8(vw_1c), vb_20, con(0x64), CUTX8(vw_1c));
  ddata.v1e01a8 = con(0x0);
}

void SKW_ADVANCE_TILES_TIME(x16 eaxw, x16 edxw)
{
  x16 wordrg5;
  c_tim* timptrrg1;
  x16 vw_04;

  vw_04 = eaxw;
  if (ddata.v1d3248 == ddata.v1e0282 && vw_04 == ddata.v1e0280)
    if (edxw == ddata.v1e027e)
      SKW_MOVE_RECORD_TO(con(0xffff), edxw, vw_04, vw_04, edxw);
  x16 wordrg12 = SKW_GET_CREATURE_AT(vw_04, edxw);
  if (wordrg12 != con(0xffffffff))
    SKW_MOVE_RECORD_TO(wordrg12, edxw, vw_04, vw_04, edxw);
  wordrg5 = SKW_GET_WALL_TILE_ANYITEM_RECORD(vw_04, edxw);
  x16 vw_08 = wordrg5;
  x16 vw_0c = con(0x0);
  while (wordrg5 != con(0xfffffffe))
  {
    if (urshift(wordrg5 & con(0x3c00), con(0xa)) > con(0x4))
      vw_0c++;
    wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg5);
  }
  for(;;)
  {
    wordrg5 = vw_08;
    for(;;)
    {
      if (wordrg5 == con(0xfffffffe) || vw_0c == con(0x0))
        return;
      x16 wordrg6 = wordrg5;
      x16 wordrg11 = SKW_GET_NEXT_RECORD_LINK(wordrg6);
      vw_08 = wordrg11;
      x16 wordrg7 = urshift(wordrg5 & con(0x3c00), con(0xa));
      if (wordrg7 > con(0x4))
      {
        x16 wordrg21 = edxw;
        vw_0c--;
        SKW_MOVE_RECORD_TO(wordrg6, wordrg21, vw_04, vw_04, wordrg21);
        if (wordrg7 != con(0xe))
        {
          if (wordrg7 == con(0xf))
          {
            timptrrg1 = ddata.savegametimarrp;
            x16 wordrg40 = con(0x0);
            while (wordrg40 < ddata.savegamew9)
            {
              if (timptrrg1->b_04 == con(0x19))
              {
                if (wordrg5 == timptrrg1->v.w_08)
                {
                  timptrrg1->u.b.b_06 = CUTX8(ddata.v1e102a);
                  timptrrg1->u.b.b_07 = CUTX8(ddata.v1e1028);
                  timptrrg1->v.w_08 = (timptrrg1->v.w_08 & con(0x3fff)) | (ddata.v1e1026 << con(0xe));
                  timptrrg1->un.l_00 = (timptrrg1->un.l_00 & con(0xffffff)) | (unsignedlong(ddata.v1e102c) << con(0x18));
                }
              }
              timptrrg1++;
              wordrg40++;
            }
          }
        }
        else
        {
          timptrrg1 = &ddata.savegametimarrp[SKW_GET_ADDRESS_OF_RECORD(wordrg6)->u6.w];
          timptrrg1->v.b.b_08 &= con(0xffffffe0);
          x16 wordrg22 = timptrrg1->v.w_08 | (ddata.v1e102a & con(0x1f));
          timptrrg1->v.w_08 = wordrg22;
          timptrrg1->v.w_08 = (wordrg22 & con(0xfffffc1f)) | ((ddata.v1e1028 & con(0x1f)) << con(0x5));
          timptrrg1->v.b.b_09 &= con(0xfffffff3);
          timptrrg1->v.w_08 |= ((ddata.v1e1024 & con(0x3)) << con(0xa));
          wordrg5 &= con(0xffff3fff);
          wordrg5 |= ddata.v1e1026 << con(0xe);
          timptrrg1->u.w_06 = wordrg5;
          x32 longrg2 = timptrrg1->un.l_00 & con(0xffffff);
          longrg2 |= unsignedlong(ddata.v1e102c) << con(0x18);
          timptrrg1->un.l_00 = longrg2;
        }
        break;
      }
      wordrg5 = wordrg11;
    }
  }
}

x16 SKW_476d_030a(x16 eaxw)
{
  const t_text* tptrrg5;
  x8 vba_00[0x10]; // TODO check deeper, just adress_of(0x0); used here

  if (eaxw == con(0x1))
  {
    if (ddata.v1d154a)
    {
      ddata.v1dffd6 = con(0x0);
      return ddata.v1dffd6;
    }
    x16 wordrg1 = ddata.v1dffd4;
    tptrrg5 = ddata.v1d1524;
    for(;;)
    {
      x32 longrg2 = signedlong(wordrg1);
      ui16 uwordrg1 = R_1278(longrg2); // TODO: readsectors?
      x16 wordrg3 = uwordrg1;
      if (uwordrg1 == con(0x0))
      {
        ddata.v1dffd6 = con(0x4);
        return ddata.v1dffd6;
      }
      else if (uwordrg1 > UI16CAST(con(0x2)))
      {
        ddata.v1dffd6 = con(0x3);
        return ddata.v1dffd6;
      }
      wordrg1 = CUTX16(longrg2);
      R_1296(vba_00); // TODO: findfirst
      if (R_52DC0(vba_00, tptrrg5))
      {
        if (wordrg3 != con(0x2))
        {
          if (ddata.gdatfiletype2)
          {
            if (longrg2 == unsignedlong(ddata.v1dffd0) && ddata.v1dffd6 == con(0x0))
            {
              ddata.v1dffd6 = con(0x2);
              return ddata.v1dffd6;
            }
          }
          ddata.v1dffd6 = con(0x1);
          return ddata.v1dffd6;
        }
        ddata.v1dffd6 = con(0x2);
        return ddata.v1dffd6;
      }
      ddata.v1dffd6 = con(0x0);
      if (!ddata.gdatfiletype2 || ddata.gdatfiletype1 || longrg2 == lextended(ddata.v1dffd0))
        return ddata.v1dffd6;
      tptrrg5 = ddata.v1d1528;
    }
  }
  else if (eaxw != con(0x2))
  {
    ddata.v1dffd6 = con(0x4);
    return ddata.v1dffd6;
  }
  if (ddata.v1d154e)
  {
    ddata.v1dffd6 = con(0x1);
    return ddata.v1dffd6;
  }
  ui16 uwordrg1 = R_1278(signedlong(ddata.v1dffd2)); // TODO: readsectors?
  if (uwordrg1 < con(0x1))
  {
    if (uwordrg1 == con(0x0)) // TODO: BUG? what's here possible except 0 ?!? else???
      ddata.v1dffd6 = con(0x4);
    else
      ddata.v1dffd6 = con(0x3);
    return ddata.v1dffd6;
  }
  if (uwordrg1 <= con(0x1))
    ddata.v1dffd6 = con(0x1);
  else if (uwordrg1 == con(0x2))
    ddata.v1dffd6 = con(0x2);
  else
  {
    ddata.v1dffd6 = con(0x3);
    return ddata.v1dffd6;
  }
  // rg1l = signedlong(ddata.v1dffd2);  --- what for?
  R_1296(DOWNCAST(x8, vba_00)); // TODO: findfirst
  if (((!R_52DC0(vba_00, ddata.v1d1524)) || (!R_52DC0(vba_00, ddata.v1d1528))) || (!R_52DC0(vba_00, ddata.v1d1520)))
    ddata.v1dffd6 = con(0x0);
  return ddata.v1dffd6;
}

x16 SKW_GET_ITEM_ORDER_IN_CONTAINER(x16 eaxw, x16 edxw)
{
  x16 wordrg5;
  t_text t_04[0x80];
  t_text* tptrrg7;
  x16 vw_84;

  tptrrg7 = SKW_QUERY_GDAT_TEXT(con(0x14), con(0x40), t_04, SKW_QUERY_CLS2_FROM_RECORD(eaxw));
  if (*tptrrg7 == NULLBYTE)
    return con(0xffffffff);
  vw_84 = con(0x0);
  x16 vw_88 = con(0x0);
  x16 vw_8c = con(0x0);
  for(;;)
  {
    wordrg5 = con(0xffffffff);
    x16 wordrg6 = con(0xffffffff);
    for(;;)
    {
      if (vw_84 != con(0x0))
        return con(0xffffffff);
      ui8 ubyterg1 = *tptrrg7++;
      if (ubyterg1 < con(0x30) || ubyterg1 > con(0x39))
      {
        if (ubyterg1 < con(0x2d))
        {
          if (ubyterg1 == con(0x0))
            vw_84 = con(0x1);
          break;
        }
        if (ubyterg1 <= con(0x2d))
        {
          wordrg5 = vw_88;
          vw_88 = con(0x0);
          continue;
        }
        if (ubyterg1 == con(0x4a))
        {
          if (vw_88 == con(0x0))
          {
            wordrg6 = con(0x100);
            continue;
          }
          tptrrg7--;
        }
        break;
      }
      x16 wordrg4 = vw_88 * con(0xa); // BUGFIX 4/2014
      wordrg4 += unsignedword(ubyterg1) - con(0x30);
      vw_88 = wordrg4;
    }
    if (wordrg5 < con(0x0))
      wordrg5 = vw_88;
    while (wordrg5 <= vw_88)
    {
      if (vw_8c == edxw)
      {
        x16 wordrg12 = con(0x0);
        while (wordrg12 < ddata.v1e03fe)
        {
          if (ddata.v1e0394[wordrg12] == (wordrg6 + wordrg5))
            return wordrg12;
          wordrg12++;
        }
      }
      vw_8c++;
      wordrg5++;
    }
    vw_88 = con(0x0);
  }
}

x16 SKW_075f_0182(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  x16 wordrg5;
  c_record* recoptrrg1;
  x16 vw_00;

  wordrg5 = eaxw;
  vw_00 = ecxw;
  x16 wordrg1 = SKW_GET_TILE_RECORD_LINK(edxw, ebxw);
  x16 wordrg3;
  for (;; wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg3))
  {
    wordrg3 = wordrg1;
    if (wordrg3 == con(0xfffffffe))
      return con(0xfffffffe);
    if (urshift(wordrg3 & con(0x3c00), con(0xa)) == con(0x3) && (recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg3), (recoptrrg1->u2.w & con(0x7f)) == con(0x26)))
    {
      x32 longrg44;
      if ((recoptrrg1->u2.w & con(0xffffff80)) == con(0xffffff80))
        longrg44 = con(0x1);
      else
      {
        if (urshift(recoptrrg1->u2.w, con(0x7)) != (wordrg5 - con(0xff80)))
          longrg44 = con(0x0);
        else
          longrg44 = con(0x1);
      }
      longrg44 ^= unsignedlong(ulrshift(recoptrrg1->u4.w, con(0xa), con(0xf))); // TODO: make right side bool, then left variable too
      if ((recoptrrg1->u4.b.b0 & con(0x4)) != con(0x0))
      {
        if (vw_00 < ((wordrg5 != con(0xff82)) ? con(0xff) : con(0x7f)))
          longrg44 = con(0x0);
      }
      bool skip = false;
      x16 wordrg22 = recoptrrg1->u4.w & con(0x18);
      x16 wordrg408;
      if (wordrg22 != con(0x18))
      {
        if (longrg44 == con(0x0))
          skip = true;
        else
        {
          wordrg408 = ulrshift(recoptrrg1->u4.w, con(0xb), con(0xe));
          wordrg22 = con(0x0);
        }
      }
      else
      {
        wordrg22 = con(0x0);
        wordrg408 = (longrg44 == con(0x0)) ? 1 : 0;
      }
      if (!skip)
        SKW_INVOKE_ACTUATOR(recoptrrg1, wordrg22, wordrg408);
    }
  }
}

// TODO: make eaxw unsigned?
ui16 SKW_32cb_54ce(x16 eaxw, x16* ebxpw, x16 ecxw, x16* edxpw, x16 argw0)
{
  x16 wordrg5;
  x16 vw_00;

  x16 wordrg6;
  vw_00 = eaxw;
  if (UI16CAST(eaxw) <= con(0x3))
  {
    switch (vw_00)
    {
      case 0:
        wordrg5 = *ebxpw - argw0;
        wordrg6 = ecxw - *edxpw;
      break;

      case 1:
        wordrg5 = ecxw - *edxpw;
        wordrg6 = argw0 - *ebxpw;
      break;

      case 2:
        wordrg5 = argw0 - *ebxpw;
        wordrg6 = *edxpw - ecxw;
      break;

      case 3:
        wordrg5 = *edxpw - ecxw;
        wordrg6 = *ebxpw - argw0;
      break;

      default: throw(THROW_DMABORT);
    }
  }
  *edxpw = wordrg6;
  *ebxpw = wordrg5;
  if (wordrg5 < con(0x1))
    return con(0x0);
  wordrg5 = wordrg5 * wordrg5 + wordrg6 * wordrg6;
  if (wordrg5 <= con(0x2))
    return con(0x1);
  ui16 uwordrg15 = urshift(wordrg5, con(0x1));
  ui16 uwordrg3;
  ui16 uwordrg16;
  do
  {
    uwordrg3 = uwordrg15;
    uwordrg16 = ((wordrg5 / uwordrg3) + uwordrg3) >> con(0x1);
  } while (uwordrg15 < uwordrg3);
  return uwordrg3;
}

void SKW_3a15_38b6(bool eaxbool)
{
  c_record* recoptrrg5;
  x8* bptrrg4;
  x8* bptrrg1;
  x16* pw_04;
  x8* pb_08;
  x16 vw_10;
  x16 vw_18;
  x16 vw_20;

  vw_20 = ddata.v1d3248;
  SKW_CHANGE_CURRENT_MAP_TO(ddata.v1e0266);
  x16 wordrg4 = ddata.v1d3248;
  vw_18 = ulrshift(ddata.v1e03c8[wordrg4].w8, con(0x5), con(0xb));
  vw_10 = urshift(ddata.v1e03c8[wordrg4].w8, con(0xb));
  x8** hlpptr1 = ddata.v1e03cc[wordrg4];
  pb_08 = *hlpptr1;
  pw_04 = &ddata.v1e038c[ddata.v1e03d8[ddata.v1e03e4[wordrg4]]];
  x16 vw_14 = con(0x0);
  // M_49AC5
  for(;;)
  {
    if (vw_14 > vw_18)
    {
      SKW_CHANGE_CURRENT_MAP_TO(vw_20);
      return;
    }
    x16 vw_1c = con(0x0);
    // M_49AAA
    for (;; vw_1c++)
    {
      if (vw_1c > vw_10)
      {
        vw_14++;
        break;
      }
      bptrrg4 = pb_08;
      bool vbool_0c = urshift(unsignedword(*bptrrg4), con(0x5)) == con(0x0);
      bptrrg1 = bptrrg4++;
      pb_08 = bptrrg4;
      if ((*bptrrg1 & con(0x10)) != con(0x0))
      {
        x16 wordrg6 = *pw_04++;
        // M_49915 with continue-label M_49A94
        for (;; wordrg6 = SKW_GET_NEXT_RECORD_LINK(wordrg6))
        {
          x16 wordrg424 = urshift(wordrg6 & con(0x3c00), con(0xa));
          if (wordrg424 > con(0x3))
            break;
          if (wordrg424 != con(0x3))
          {
            if (wordrg424 == con(0x2))
            {
              recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
              if ((recoptrrg5->u2.w & con(0x6)) == con(0x2))
              {
                x16 wordrg133 = recoptrrg5->u2.w;
                if ((urshift(urshift(wordrg133, con(0x3)), con(0x8)) & con(0x1f)) == con(0x15) && eaxbool)
                {
                  if ((wordrg133 & con(0x1)) == con(0x0))
                  {
                    wordrg133 |= con(0x1);
                    recoptrrg5->u2.w = wordrg133;
                    x16 wordrg2 = SKW_RAND02();
                    SKW_ALLOC_NEW_CREATURE(unsignedword(CUTX8(urshift(recoptrrg5->u2.w, con(0x3)))), wordrg2, vw_14, con(0x7), vw_1c);
                  }
                }
              }
            }
            continue;
          }
          x16 wordrg7 = wordrg6;
          recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
          ui16 uwordrg405 = recoptrrg5->u2.w & con(0x7f);
          if (uwordrg405 < con(0x21))
            continue;
          if (uwordrg405 <= con(0x21))
          {
            if ((recoptrrg5->u4.w & con(0x18)) != con(0x18))
            {
              if (((recoptrrg5->u4.b.b0 & con(0x20)) == con(0x0)) == eaxbool)
                SKW_INVOKE_ACTUATOR(recoptrrg5, con(0x0), ulrshift(recoptrrg5->u4.w, con(0xb), con(0xe)));
            }
            else
              SKW_INVOKE_ACTUATOR(recoptrrg5, con(0x0), ((ulrshift(recoptrrg5->u4.w, con(0xa), con(0xf)) ^ (eaxbool ? 1 : 0)) == con(0x0)) ? 1 : 0);
            continue;
          }
          if (uwordrg405 == con(0x2c))
            if (eaxbool)
              if ((recoptrrg5->u4.w & con(0x1)) == con(0x1))
                SKW_TRY_ORNATE_NOISE(recoptrrg5, vw_14, vw_1c, wordrg7, SKW_GET_ORNATE_ANIM_LEN(recoptrrg5, vbool_0c), vbool_0c);
        }
      }
    }
  }
}

void SKW_ACTIVATE_ITEM_TELEPORT(c_tim* eaxptim, x16 ebxw, bool ecxbool, c_record* edxprec, bool argbool0, x16* argp1, bool argbool2, bool argbool3)
{
  s_x40bytes* ptrrg2;
  c_record* recoptrrg1;
  c_tim* ptim_00;
  s_x40bytes* ps_04;
  x16 vw_10;
  x16 vw_14;
  x16 vw_18;
  x16 vw_1c;
  x16 vw_28;
  x16 vw_30;

  ptim_00 = eaxptim;
  x16 vw_20 = ebxw;
  bool vbool_0c = ecxbool;
  if (!argbool2)
  {
    vw_14 = unsignedword(ptim_00->u.b.b_06);
    vw_10 = unsignedword(ptim_00->u.b.b_07);
    vw_28 = unsignedword(ptim_00->v.b.b_08);
    vw_18 = ulrshift(edxprec->u6.w, con(0x5), con(0xb));
    vw_30 = urshift(edxprec->u6.w, con(0xb));
    vw_1c = ulrshift(edxprec->u6.w, con(0xa), con(0xe));
  }
  else
  {
    vw_14 = ulrshift(edxprec->u6.w, con(0x5), con(0xb));
    vw_10 = urshift(edxprec->u6.w, con(0xb));
    vw_28 = ulrshift(edxprec->u6.w, con(0xa), con(0xe));
    vw_18 = unsignedword(ptim_00->u.b.b_06);
    vw_30 = unsignedword(ptim_00->u.b.b_07);
    vw_1c = unsignedword(ptim_00->v.b.b_08);
  }
  if ((edxprec->u4.b.b0 & con(0x20)) != con(0x0) || ptim_00->v.b.b_09 != con(0x0))
  {
    if ((edxprec->u4.b.b0 & con(0x20)) == con(0x0))
      return;
    if (ptim_00->v.b.b_09 != con(0x1))
      return;
  }
  if (vw_20 == con(0xffffffff))
  {
    vw_20 = SKW_GET_TILE_RECORD_LINK(vw_14, vw_10);
    if (((SKW_GET_TILE_VALUE(vw_14, vw_10) & con(0xff)) >> con(0x5)) != con(0x0))
      vbool_0c = true;
  }
  x16 wordrg5 = vw_20;
  x16 vw_24 = con(0x0);
  for(;;)
  {
    if (wordrg5 == con(0xfffffffe))
      break;
    x16 wordrg11 = urshift(wordrg5 & con(0x3c00), con(0xa));
    if (wordrg11 >= con(0x5) && wordrg11 <= con(0xa) && (vbool_0c || urshift(wordrg5, con(0xe)) == vw_28))
      vw_24++;
    wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg5);
  }
  x16 wordrg51 = vw_20;
  if (argbool0)
  {
    ptrrg2 = UPCAST(s_x40bytes, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0x40)));
    ps_04 = ptrrg2;
    SKW_QUERY_CREATURES_ITEM_MASK(CUTX8(urshift(edxprec->u2.w, con(0x7))), ptrrg2, false, CUTX8(3 * ulrshift(edxprec->u4.w, con(0x5), con(0xc)) + con(0x10)));
  }
  for(;;)
  {
    if (wordrg51 == con(0xfffffffe))
      break;
    x16 wordrg23 = urshift(wordrg51 & con(0x3c00), con(0xa));
    x16 vw_2c = SKW_GET_NEXT_RECORD_LINK(wordrg51);
    if (vw_24 <= con(0x0) || wordrg23 < con(0x5) || wordrg23 > con(0xa) || (!vbool_0c && urshift(wordrg51, con(0xe)) != vw_28))
    {
      if (!argbool0)
      {
        if (wordrg23 == con(0x4))
        {
          recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg51);
          SKW_ACTIVATE_ITEM_TELEPORT(ptim_00, recoptrrg1->u2.w, vbool_0c, edxprec, argbool0, &recoptrrg1->u2.w, argbool2, argbool3);
        }
      }
      wordrg51 = vw_2c;
      continue;
    }
    vw_24--;
    x16 wordrg22 = SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg51);
    bool boolrg1;
    if (argbool0)
      boolrg1 = (((con(0x1) << (CUTX16(wordrg22) & con(0x7))) & ps_04->barr_00[signedlong(wordrg22) / 8]) != con(0x0));
    else if (wordrg22 != urshift(edxprec->u2.w, con(0x7)))
      boolrg1 = ((edxprec->u2.w & con(0xffffff80)) == con(0xffffff80));
    else if ((edxprec->u4.b.b0 & con(0x4)) != con(0x0) || SKW_QUERY_CLS1_FROM_RECORD(wordrg51) != con(0x15) || SKW_QUERY_CLS2_FROM_RECORD(wordrg51) != con(0x0))
      boolrg1 = true;
    else
    {
      x16 wordrg45 = wordrg51;
      wordrg51 = vw_2c;
      SKW_CUT_RECORD_FROM(wordrg45, (argp1 == NULL) ? vw_14 : con(0xffffffff), vw_10, argp1);
      SKW_DEALLOC_RECORD(wordrg45);
      SKW_BRING_CHAMPION_TO_LIFE(SKW_ADD_ITEM_CHARGE(wordrg45, con(0x0)));
      continue;
    }
    if (boolrg1)
    {
      SKW_MOVE_ITEM_TO(wordrg51, vw_14, vw_10, edxprec, vw_18, vw_30, vw_1c, argp1 != NULL, ulrshift(edxprec->u4.w, con(0xd), con(0xf)));
      if (argbool3)
        break;
    }
    wordrg51 = vw_2c;
  }
  if (argbool0)
    DM2_DEALLOC_LOBIGPOOL(con(0x40));
}

void SKW_MOVE_ITEM_TO(x16 eaxw, x16 ebxw, x16 ecxw, c_record* edxprec, x16 argw0, x16 argw1, x16 argw2, bool argbool3, x16 argw4)
{
  x16 wordrg5;
  x16 vw_00;

  wordrg5 = eaxw;
  if (SKW_QUERY_CLS1_FROM_RECORD(eaxw) != con(0x15) || SKW_QUERY_CLS2_FROM_RECORD(eaxw) != con(0x0))
  {
    ui16 uwordrg1 = ulrshift(edxprec->u4.w, con(0xb), con(0xe));
    if (uwordrg1 < con(0x1))
    {
      if (uwordrg1 == con(0x0))  // TODO again: what's left except 0 ?!?
        vw_00 = SKW_GET_MAX_CHARGE(wordrg5);
    }
    else if (uwordrg1 <= UI16CAST(con(0x1)))
      vw_00 = con(0xfffffff1);
    else if (uwordrg1 == con(0x2))
      vw_00 = SKW_RAND16(SKW_GET_MAX_CHARGE(wordrg5) + con(0x1));
    SKW_ADD_ITEM_CHARGE(wordrg5, signedlong(vw_00));
  }
  if (((SKW_GET_TILE_VALUE(argw0, argw1) & con(0xff)) >> con(0x5)) != con(0x0))
    argw2 = SKW_RAND16(con(0x4));
  if (ebxw >= con(0x0) && ecxw >= con(0x0))
    SKW_MOVE_RECORD_TO(wordrg5, ecxw, con(0xffffffff), ebxw, ((!argbool3) ? 1 : 0) - con(0x1));
  wordrg5 &= con(0xffff3fff);
  SKW_MOVE_RECORD_TO((argw2 << con(0xe)) | wordrg5, ((argw4 == con(0x0)) ? 1 : 0), argw0, con(0xffffffff), argw1);
}

void SKW_ACTIVATE_CREATURE_KILLER(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2, x16 argw3)
{
  x16 wordrg5;
  x16 vw_04;
  x16 vw_10;

  x16 wordrg3 = ecxw;
  x16 wordrg2 = ebxw;
  vw_04 = edxw;
  x16 wordrg4 = DM2_ABS(wordrg2 - argw0);
  x16 wordrg111 = DM2_ABS(wordrg3 - argw1);
  wordrg2 -= wordrg4;
  argw0 = wordrg2;
  wordrg3 -= wordrg111;
  argw1 = wordrg3;
  wordrg5 = 2 * wordrg4 + con(0x1);
  x16 vw_08 = wordrg5;
  x16 vw_0c = 2 * wordrg111 + con(0x1);
  // M_46C93
  for(;;)
  {
    if (--vw_0c == con(0xffffffff))
      return;
    wordrg5 = vw_08;
    // M_46CA7
    for(;;)
    {
      wordrg5--;
      if (wordrg5 == con(0xffffffff))
        break;
      x16 wordrg100 = argw0 + wordrg5;
      if (wordrg100 < con(0x0) || wordrg100 >= ddata.savegamewa)
        continue;
      x16 wordrg3 = vw_0c;
      x16 wordrg40 = argw1 + wordrg3;
      if (wordrg40 < con(0x0) || wordrg40 >= ddata.savegamewb)
        continue;
      vw_10 = SKW_GET_CREATURE_AT(wordrg100, wordrg40);
      if (vw_10 == con(0xffffffff))
        continue;
      x16 vw_00 = vw_10;
      if (!SKW_1c9a_09b9(vw_10, vw_04))
        continue;
      if (argw2 != con(0xb))
      {
        if (argw2 == con(0x28))
        {
          x16 wordrg33;
          if (argw3 == con(0x0))
            wordrg33 = eaxw;
          else
            wordrg33 = eaxw | con(0x8000);
          SKW_ATTACK_CREATURE(vw_10, argw1 + vw_0c, wordrg33, argw0 + wordrg5, con(0x64), con(0x0));
        }
        continue;
      }
      if (eaxw < con(0x1))
      {
        if (eaxw != con(0x0))
          return;
        continue;
      }
      if (eaxw == con(0x1))
        continue;
      if (eaxw != con(0x2))
        return;
      SKW_13e4_0360(vw_00, argw1 + wordrg3, con(0x13), argw0 + wordrg5, true);
    }
  }
}

t_gfxdata* R_B073(t_gfxdata* eaxpg, x16 ebxw, x16 ecxw, x8 edxb, x16* argpw0)
{
  bool flag;
  t_gfxdata* pg_00;
  s_bpp* sptrrg41;
  s_bpp* sptrrg42;
  x8* bptrrg5;
  x8* bptrrg4;
  x16 vw_10;
  x16 vw_14;
  x16 vw_18;
  x16 vw_1c;
  x16 vw_20;
  x16 vw_24;
  x8 vb_28;

  pg_00 = eaxpg;
  vb_28 = edxb;
  vw_18 = ebxw;
  vw_14 = ecxw;
  if ((pg_00 == NULL) || (edxb == con(0x0)))
    return pg_00;
  vb_28 = CUTX8(DM2_MAX(con(0x0), con(0x40) - unsignedword(edxb)));
  vw_24 = *argpw0;
  bool vbool_08 = vw_24 <= con(0x0);
  flag = vw_24 > con(0x0);
  if (!flag)
  {
    vw_24 = con(0x100);
    *argpw0 = con(0x100);
    if (SKW_3e74_55f9(unsignedlong(vb_28) | ((unsignedlong(CUTX8(vw_18)) << con(0xe)) | con(0x60000000) | (unsignedlong(CUTX8(vw_14)) << con(0x6))), &vw_10))
    {
      SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, SKW_3e74_5817(vw_10)), PAL256, DOWNCAST(t_gfxdata, pg_00));
      return pg_00;
    }
  }
  // M_B2F0
  for (x16 vw_0c = con(0x0); vw_0c < vw_24; vw_0c++)
  {
    x32 longrg47;
    x16 wordrg14;
    if (!vbool_08)
    {
      u_bbw* tmpp2 = UPCAST(u_bbw, &ddata.v1e0210[pg_00[vw_0c]]); // access on both bytes then
      vw_20 = unsignedword(tmpp2->b.b0);
      bptrrg5 = ddata.v1e020c[tmpp2->b.b0].pb1;
      wordrg14 = unsignedword(bptrrg5[tmpp2->b.b1]);
      longrg47 = unsignedlong(vb_28);
    }
    else
    {
      u_bbw* tmpp1 = UPCAST(u_bbw, &ddata.v1e0210[vw_0c]); // access on both bytes then
      vw_20 = unsignedword(tmpp1->b.b0);
      bptrrg5 = ddata.v1e020c[tmpp1->b.b0].pb1;
      longrg47 = unsignedlong(bptrrg5[tmpp1->b.b1]);
      wordrg14 = unsignedword(vb_28);
    }
    x16 wordrg6 = DM2_MAX(CUTX16((wordrg14 * longrg47) >> con(0x6)), con(0x0));
    x16 wordrg18 = con(0x0);
    vw_1c = unsignedword(ddata.v1e020c[vw_20].b0) - con(0x1);
    // M_B1FE
    for (; wordrg18 < vw_1c; wordrg18++) // TODO: ugly loopform
    {
      bptrrg4 = bptrrg5 + wordrg18;
      x16 wordrg31 = wordrg6 - unsignedword(*bptrrg4);
      if (wordrg31 < con(0x0))
        continue;
      x16 wordrg441 = unsignedword(*(bptrrg4 + con(0x1))) - wordrg6;
      if (wordrg441 < con(0x0))
        continue;
      if (wordrg31 > wordrg441)
        wordrg18++;
      break;
    }
    sptrrg41 = &ddata.v1e020c[vw_20];
    x8 byterg405 = sptrrg41->pb5[wordrg18];
    if (unsignedword(byterg405) == vw_18 || unsignedword(byterg405) == vw_14)
    {
      x16 wordrg32 = wordrg18 - con(0x1);
      x16 wordrg26 = wordrg18 + con(0x1);
      // M_B26B
      do
      {
        x16 wordrg17;
        if (wordrg32 < con(0x0))
          wordrg17 = wordrg26++;
        else
        {
          bool skip = false; // added
          if (wordrg26 <= vw_1c)
          {
            wordrg17 = wordrg6;
            if ((unsignedword(bptrrg5[wordrg26]) - wordrg6) < (wordrg6 - unsignedword(bptrrg5[wordrg32])))
            {
              wordrg17 = wordrg26++;
              skip = true;
            }
          }
          if (!skip)
          {
            wordrg17 = wordrg32;
            wordrg32--;
          }
        }
        sptrrg42 = &ddata.v1e020c[vw_20];
        byterg405 = sptrrg42->pb5[wordrg17];
      } while (unsignedword(byterg405) == vw_18 || unsignedword(byterg405) == vw_14);
    }
    pg_00[vw_0c] = byterg405;
  }
  if (vw_18 >= con(0x0))
    pg_00[vw_18] = CUTX8(vw_18);
  if (vw_14 >= con(0x0))
    pg_00[vw_14] = CUTX8(vw_14);
  if (vbool_08)
  {
    SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, pg_00), PAL256, DOWNCAST(s_xmalloc, SKW_ALLOC_CPXHEAP_MEM(vw_10, con(0x100))));
    SKW_3e74_585a(vw_10, false);
  }
  return pg_00;
}

x16* SKW_OVERSEE_RECORD(x16* eaxpw, x16** ebxppw, FPTR_b_WPWP ecxf, x8 edxb, x16* argpw0, x16 argw1, x16 argw2)
{
  x16* wptrrg5;
  x16* wptrrg1;
  x8 vb_08;

  wptrrg5 = eaxpw;
  vb_08 = edxb;
  // M_C9AF: co-label M_CA7E
  for (;; wptrrg5 = &SKW_GET_ADDRESS_OF_RECORD(*wptrrg5)->w0)
  {
    if (*wptrrg5 == con(0xfffffffe) || *wptrrg5 == con(0xffffffff))
      return NULL;
    if (vb_08 != con(0xffffffff) && (urshift(*wptrrg5, con(0xe)) != unsignedword(vb_08)))
      continue;
    if (urshift(*wptrrg5 & con(0x3c00), con(0xa)) == con(0x4) && argw1 != con(0x0))
    {
      wptrrg1 = SKW_OVERSEE_RECORD(&SKW_GET_ADDRESS_OF_RECORD(*wptrrg5)->u2.w, ebxppw, ecxf, con(0xff), argpw0, argw1, argw2);
      if (wptrrg1 != NULL)
        return wptrrg1;
    }
    if ((argw2 != con(0x0)) && SKW_IS_CONTAINER_CHEST(*wptrrg5))
    {
      wptrrg1 = SKW_OVERSEE_RECORD(&SKW_GET_ADDRESS_OF_RECORD(*wptrrg5)->u2.w, ebxppw, ecxf, con(0xff), argpw0, argw1, argw2);
      if (wptrrg1 != NULL)
        return wptrrg1;
    }
    if (INDCALL(ecxf)(wptrrg5, argpw0))
    {
      *ebxppw = eaxpw;
      return wptrrg5;
    }
  }
}

// code doubled for some parts - obviously macros used
// spreaded:
// M_16F11, M_16F91, M_16F0B, M_16F6D, M_16F6F, M_170ED, M_1700F and M_16F74
// moved cases 3 and 18, so more code doubling avoided
// TODO: indexproblem inside
bool SKW_14cd_1316(x8 eaxb, x8 ebxb, x16 edxw)
{
  x8* bptrrg5;
  c_record* recoptrrg3;
  s_d556* sd5ptrrg1;
  x16* wptrrg2;
  x16 vw_0c;
  x16 vw_14;
  x8 vb_18; // and8

  vb_18 = eaxb;
  vw_0c = edxw;
  recoptrrg3 = ddata.s350.v1e054e;
  if ((vb_18 & con(0x40)) != con(0x0))
  {
    if (ebxb == ddata.s350.v1e0556->u.v.b12)
      return true;
    vb_18 &= con(0xffffffbf);
  }
  bool vbool_04 = (vb_18 & con(0xffffff80)) != con(0x0); // made a comparison, makes expressions easier
  vb_18 &= con(0x7f);
  bool vbool_00 = false; // ...solves a lot of silly expressions
  if (UI8CAST(vb_18) > UI8CAST(con(0x16)))
    return !vbool_04 == vbool_00;
  switch (vb_18)
  {
    case 0:
    return !vbool_04;

    case 1:
    case 22:
      if (unsignedword(ddata.s350.v1e0571) != ddata.v1e08d6)
        return !vbool_04 == vbool_00;
      if (SKW_CALC_VECTOR_DIR(ddata.v1e08d8, unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_07), ddata.v1e08d4) != ddata.v1e08da)
        return !vbool_04 == vbool_00;
      if (vb_18 == con(0x1))
        return !vbool_04;
      {
        x16 wordrg15 = SKW_CALC_SQUARE_DISTANCE(ddata.v1e08d8, unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_07), ddata.v1e08d4);
        if (wordrg15 <= con(0x1))
          return !vbool_04;
        if (wordrg15 > vw_0c || ddata.v1e0976 != con(0x0))
          return !vbool_04 == vbool_00;
      }
      if (SKW_19f0_0207(ddata.v1e08d8, unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_07), ddata.v1e08d4, R_1BA1B) == con(0x0))
        return !vbool_04 == vbool_00;
    return !vbool_04;

    case 2:
      {
        bool boolrg13;
        if (unsignedword(ddata.s350.v1e0571) != ddata.v1e08d6)
          boolrg13 = false;
        else
        {
          if (unsignedword(ddata.s350.v1e0562.u.b.b_06) != ddata.v1e08d8)
            boolrg13 = false;
          else
            boolrg13 = unsignedword(ddata.s350.v1e0562.u.b.b_07) == ddata.v1e08d4;
        }
        return !vbool_04 == boolrg13;
      }

    case 4:
    return !vbool_04 == (ddata.s350.v1e058d != con(0x0));

    case 5:
    case 13:
      if (unsignedword(ddata.s350.v1e0571) != urshift(recoptrrg3->uc.w, con(0xa)))
        return !vbool_04 == vbool_00;
      if (unsignedword(ddata.s350.v1e0562.u.b.b_06) != (recoptrrg3->uc.w & con(0x1f)))
        return !vbool_04 == vbool_00;
      if (unsignedword(ddata.s350.v1e0562.u.b.b_07) != ulrshift(recoptrrg3->uc.w, con(0x6), con(0xb)))
        return !vbool_04 == vbool_00;
      if (vb_18 != con(0xd))
        return !vbool_04;
    // no break here
    case 3:
      if (SKW_1c9a_0732(vw_0c, con(0xff), recoptrrg3->u2.w) == con(0xfffffffe))
        return !vbool_04 == vbool_00;
    return !vbool_04;

    case 6:
      if (((con(0x1) << CUTX8(vw_0c)) & unsignedlong(recoptrrg3->ua.w)) == con(0x0))
        return !vbool_04 == vbool_00;
    return !vbool_04;

    case 7:
      if (ddata.s350.v1e0571 != ddata.v1e08d6)
        return !vbool_04 == vbool_00;
    return !vbool_04;

    case 8:
      {
        x16 wordrg6 = con(0x0);
        for (;; wordrg6++)
        {
          if (wordrg6 >= con(0x4))
            return (!vbool_04) ? vbool_00 : !vbool_00;
          vw_14 = SKW_GET_PLAYER_AT_POSITION(wordrg6);
          if (vw_14 == con(0xffffffff))
            continue;
          x16 wordrg420 = ddata.hero[vw_14].warrc3[1];
          if (wordrg420 != con(0xffffffff) && SKW_1c9a_0006(wordrg420, con(0xb)) != con(0x0))
            return !vbool_04;
          x16 wordrg421 = ddata.hero[vw_14].warrc3[0];
          if (wordrg421 == con(0xffffffff))
            continue;
          if (SKW_1c9a_0006(wordrg421, con(0xb)) != con(0x0))
            return !vbool_04;
        }
      }
    // deadend

    case 9:
      if (SKW_2c1d_09d9() < vw_0c)
        return !vbool_04 == vbool_00;
    return !vbool_04;

    case 10:
      {
        if (unsignedword(ddata.s350.v1e0571) != ddata.v1e08d6)
          return !vbool_04 == vbool_00;
        bool boolrg1;
        if (unsignedword(ddata.s350.v1e0562.u.b.b_06) != ddata.v1e08d8)
          boolrg1 = true;
        else
          boolrg1 = unsignedword(ddata.s350.v1e0562.u.b.b_07) != ddata.v1e08d4;
        if (!boolrg1)
          return !vbool_04 == vbool_00;
        if ((DM2_ABS(unsignedword(ddata.s350.v1e0562.u.b.b_07) - ddata.v1e08d4) + DM2_ABS(unsignedword(ddata.s350.v1e0562.u.b.b_06) - ddata.v1e08d8)) > con(0x1))
          return !vbool_04 == vbool_00;
        x16 wordrg1 = SKW_CALC_VECTOR_DIR(unsignedword(ddata.s350.v1e0562.u.b.b_06), ddata.v1e08d8, ddata.v1e08d4, unsignedword(ddata.s350.v1e0562.u.b.b_07));
        vw_14 = unsignedword(SKW_GET_TILE_VALUE(ddata.v1e08d8 + table1d27fc[wordrg1], ddata.v1e08d4 + table1d2804[wordrg1]));
        if ((urshift(vw_14, con(0x5))) != con(0x2))
          return !vbool_04 == vbool_00;
        if ((vw_14 & con(0x8)) == con(0x0))
          return !vbool_04 == vbool_00;
        if ((vw_14 & con(0x1)) != con(0x0))
          return !vbool_04 == vbool_00;
      }
    return !vbool_04;

    case 11:
      vw_14 = (ulrshift(recoptrrg3->ue.w, con(0x6), con(0xe)) + con(0x2)) & con(0x3);
      if ((ddata.s350.v1e057a & con(0x20)) == con(0x0))
        return !vbool_04 == (vbool_00);
      if ((((SKW_GET_TILE_VALUE(unsignedword(ddata.s350.v1e0562.u.b.b_06) + table1d27fc[vw_14], unsignedword(ddata.s350.v1e0562.u.b.b_07) + table1d2804[vw_14]) & con(0xff)) >> con(0x5)) & con(0xff)) != con(0x4))
        return !vbool_04 == (vbool_00);
      if (!SKW_19f0_0d10(con(0x1), unsignedword(ddata.s350.v1e0562.u.b.b_07), con(0xffffffff), unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0xffffffff), vw_14))
        return !vbool_04 == vbool_00;
    return !vbool_04;

    case 12:
      {
        if (ddata.s350.v1e0571 != ddata.v1e08d6)
          return !vbool_04 == vbool_00;
        x16 wordrg12 = ulrshift(recoptrrg3->ue.w, con(0x6), con(0xe));
        x16 wordrg13 = SKW_GET_CREATURE_AT(unsignedword(ddata.s350.v1e0562.u.b.b_06) + table1d27fc[wordrg12], unsignedword(ddata.s350.v1e0562.u.b.b_07) + table1d2804[wordrg12]);
        if (wordrg13 == con(0xffffffff))
          return !vbool_04 == vbool_00;
        if ((SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(SKW_GET_ADDRESS_OF_RECORD(wordrg13)->u4.b.b0)->u0.b.b0 & con(0x1)) == con(0x0))
          return !vbool_04 == vbool_00;
        x16 wordrg61 = con(0x0);
        // M_174BA:
        for (;; wordrg61++)
        {
          if (wordrg61 >= con(0x4))
            return !vbool_04 == vbool_00;
          x16 vw_08 = ddata.v1e08d8 + table1d27fc[wordrg61];
          x16 wordrg2 = ddata.v1e08d4 + table1d2804[wordrg61];
          vw_14 = SKW_GET_TILE_VALUE(vw_08, wordrg2);
          if (urshift(vw_14, con(0x5)) != con(0x0) || (vw_14 & con(0x10)) == con(0x0))
            continue;
          x16 wordrg12 = SKW_GET_TILE_RECORD_LINK(vw_08, wordrg2);
          // M_1745D
          for(;;)
          {
            x16 wordrg418 = wordrg12;
            if (wordrg418 == con(0xfffffffe))
              break;
            vw_14 = urshift(wordrg418 & con(0x3c00), con(0xa));
            if (vw_14 == con(0x3))
            {
              if ((SKW_GET_ADDRESS_OF_RECORD(wordrg418)->u2.w & con(0x7f)) == con(0x3f))
                return !vbool_04;
            }
            // M_174A1:
            if (vw_14 > con(0x3))
              break;
            wordrg12 = SKW_GET_NEXT_RECORD_LINK(wordrg418);
          }
        }
      }
    // deadend here

    case 14:
      if (UI32CAST(52 * unsignedlong(recoptrrg3->u6.w) / unsignedlong(ddata.s350.v1e0552->w4)) > UI32CAST(signedlong(vw_0c)))
        return !vbool_04 == vbool_00;
    return !vbool_04;

    case 15:
      {
        x16 vw_10 = con(0x0);
        x16 wordrg62 = con(0x0);
        bptrrg5 = *ddata.savegamepp1;
        wptrrg2 = &ddata.v1e038c[*ddata.v1e03f4];
        // M_175B1
        for (x16 vw_08_1 = con(0x0); vw_08_1 < ddata.savegamewa; vw_08_1++)
        {
          // M_175A3 and cont-label M_175A2
          for (x16 wordrg3 = con(0x0); wordrg3 < ddata.savegamewb; wordrg3++)
          {
            // M_1753E
            if ((*bptrrg5++ & con(0x10)) != con(0x0))
            {
              x16 wordrg417 = *wptrrg2++;
              // M_1754C:
              for(;;)
              {
                if (urshift(wordrg417 & con(0x3c00), con(0xa)) != con(0x4))
                {
                  // M_17590:
                  wordrg417 = SKW_GET_NEXT_RECORD_LINK(wordrg417);
                  if (wordrg417 == con(0xfffffffe))
                    break;
                }
                else
                {
                  x16 wordrg11 = unsignedword(SKW_GET_ADDRESS_OF_RECORD(wordrg417)->u4.b.b0);
                  vw_14 = wordrg11;
                  if (wordrg11 != vw_0c)
                  {
                    // M_1757E:
                    if (wordrg11 == con(0x34) || wordrg11 == con(0x31))
                      // M_1758A:
                      vw_10++;
                  }
                  else
                    wordrg62++;
                  break;
                }
              }
            }
          }
        }
        if (wordrg62 >= DM2_MIN(con(0x4), vw_10 + con(0x1)))
          return (!vbool_04) ? vbool_00 : !vbool_00;
        return !vbool_04;
      }
    // deadend here

    case 16:
      if (unsignedword(ddata.s350.v1e0571) != urshift(recoptrrg3->uc.w, con(0xa)))
        return !vbool_04 == vbool_00;
    return !vbool_04;

    case 17:
      SKW_19f0_045a(unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_07));
      if ((ddata.v1e08ae & con(0x10)) == con(0x0))
        return !vbool_04 == vbool_00;
      if (SKW_1c9a_1b16(vw_0c, signedword(recoptrrg3->w8)) != con(0x0))
        return !vbool_04 == vbool_00;
    return !vbool_04;

    case 19:
      if (((con(0x1) << CUTX8(vw_0c)) & recoptrrg3->ua.w) != con(0x0))
        return !vbool_04 == vbool_00;
      if (ddata.s350.v1e0571 == ddata.v1e0266)
        return !vbool_04 == vbool_00;
    return !vbool_04;

    case 20:
      if (((con(0x1) << CUTX8(vw_0c)) & unsignedlong(recoptrrg3->ua.w)) == con(0x0))
        return !vbool_04 == vbool_00;
      // no break here
    case 18:
      if (ddata.s350.v1e0571 != ddata.v1e0266)
        return !vbool_04 == vbool_00;
    return !vbool_04;

    case 21:
      sd5ptrrg1 = &ddata.s350.v1e0556[2 * signedlong(vw_0c) / 34]; // TODO: indexing correct? check edxw
      if ((sd5ptrrg1->u.v.w0e & con(0x1f)) != unsignedword(ddata.s350.v1e0562.u.b.b_06))
        return !vbool_04 == vbool_00;
      if (ulrshift(sd5ptrrg1->u.v.w0e, con(0x6), con(0xb)) != unsignedword(ddata.s350.v1e0562.u.b.b_07))
        return !vbool_04 == vbool_00;
      if (urshift(sd5ptrrg1->u.v.w0e, con(0xa)) != unsignedword(ddata.s350.v1e0571))
        return !vbool_04 == vbool_00;
    return !vbool_04;

    default: throw(THROW_DMABORT);
  }
}

bool SKW_075f_084d(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1)
{
  bool boolrg3;
  x16 wordrg5;
  c_record* recoptrrg5;
  c_record* recoptrrg7;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;
  x16 vw_14;
  x16 vw_18;

  vw_0c = eaxw;
  vw_08 = edxw;
  vw_04 = ebxw;
  x16 wordrg6 = argw1;
  boolrg3 = false;
  argw0 = (argw0 + con(0x2)) & con(0x3);
  vw_18 = SKW_GET_TILE_RECORD_LINK(vw_0c, vw_08);
  vw_14 = vw_18;
  recoptrrg7 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
  // M_7085
  for (;; vw_18 = SKW_GET_NEXT_RECORD_LINK(vw_18))
  {
    bool skip = false; // added
    x16 wordrg3;
    wordrg5 = vw_18;
    if (wordrg5 == con(0xfffffffe))
      break;
    x16 vw_10 = urshift(wordrg5, con(0xe));
    if (argw0 != vw_10)
      continue;
    x16 wordrg2;
    if (urshift(recoptrrg7->u2.w & con(0x3c00), con(0xa)) == con(0xf) || boolrg3 || SKW_0cee_317f(vw_14, wordrg5) == con(0x0) || SKW_RAND02() != con(0x0))
    {
      // M_7125
      if (urshift(vw_18 & con(0x3c00), con(0xa)) != con(0x3))
        continue;
      x16 wordrg407 = recoptrrg7->u2.w;
      if (urshift(wordrg407 & con(0x3c00), con(0xa)) == con(0xf))
        continue;
      recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(vw_18);
      x16 wordrg11 = recoptrrg5->u2.w;
      wordrg2 = urshift(wordrg11, con(0x7));
      wordrg11 &= con(0x7f);
      if (wordrg11 < con(0x22))
        continue;
      if (wordrg11 <= con(0x22))
      {
        x32 longrg10;
        // M_7192
        if (wordrg2 == con(0x1ff))
          longrg10 = con(0x1);
        else
        {
          if (wordrg2 != SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg407))
            longrg10 = con(0x0);
          else
            longrg10 = con(0x1);
        }
        longrg10 ^= unsignedlong(ulrshift(recoptrrg5->u4.w, con(0xa), con(0xf)));
        if (longrg10 == con(0x0))
          continue;
        x16 wordrg406;
        if ((recoptrrg5->u4.w & con(0x18)) != con(0x18))
        {
          wordrg406 = ulrshift(recoptrrg5->u4.w, con(0xb), con(0xe));
          wordrg2 = con(0x0);
        }
        else
        {
          wordrg2 = con(0x0);
          wordrg406 = (longrg10 == con(0x0)) ? 1 : 0;
        }
        SKW_INVOKE_ACTUATOR(recoptrrg5, wordrg2, wordrg406);
        if ((boolrg3) || ((recoptrrg5->u4.b.b0 & con(0x4)) == con(0x0)))
          continue;
        wordrg3 = ecxw;
        wordrg2 = vw_04;
        SKW_CUT_RECORD_FROM(wordrg6, wordrg2, ecxw, NULL);
        wordrg6 &= con(0xffff3fff);
        wordrg6 |= argw0 << con(0xe);
      }
      else
      {
        if (wordrg11 != con(0x23))
          continue;
        if (boolrg3 || (wordrg2 != con(0x1ff) && wordrg2 != SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg407)))
          continue;
        wordrg2 = vw_04;
        SKW_CUT_RECORD_FROM(wordrg6, wordrg2, ecxw, NULL);
        x16 wordrg405 = urshift(recoptrrg5->u6.w, con(0xb));
        x8 byterg11 = (SKW_GET_TILE_VALUE(ulrshift(recoptrrg5->u6.w, con(0x5), con(0xb)), wordrg405) & con(0xff)) >> con(0x5);
        wordrg6 &= con(0x3fff);
        x16 wordrg155;
        if (byterg11 != con(0x0))
          wordrg155 = ulrshift(recoptrrg5->u4.w, con(0xb), con(0xe));
        else
          wordrg155 = ulrshift(recoptrrg5->u6.w, con(0xa), con(0xe));
        wordrg6 |= wordrg155 << con(0xe);
        wordrg3 = urshift(recoptrrg5->u6.w, con(0xb));
        wordrg2 = ulrshift(recoptrrg5->u6.w, con(0x5), con(0xb));
        skip = true;
      }
    }
    else
    {
      wordrg3 = ecxw;
      wordrg2 = vw_04;
      wordrg5 = wordrg6;
      SKW_CUT_RECORD_FROM(wordrg5, wordrg2, ecxw, NULL);
      x16 wordrg11 = vw_10 << con(0xe);
      wordrg5 &= con(0x3fff);
      wordrg11 |= wordrg5;
      wordrg6 = wordrg11;
    }
    // M_7112
    if (!skip)
    {
      wordrg3 = vw_08;
      wordrg2 = vw_0c;
    }
    // M_72EB
    SKW_DELETE_MISSILE_RECORD(wordrg6, wordrg2, wordrg3, NULL);
    boolrg3 = true;
  }
  return boolrg3;
}

x16 SKW_RECYCLE_A_RECORD_FROM_THE_WORLD(x16 eaxw, x8 edxb)
{
  x16 wordrg6;
  c_record* recoptrrg1;
  c_record* recoptrrg4;
  c_record* recoptrrg7;
  bool vbool_04;
  x16* pw_10;
  x8* pb_14;
  bool vbool_18;
  x16 vw_1c;
  x16 vw_20;
  x16 vw_24;
  x16 vw_28;
  x16 vw_2c;
  x16 vw_30;
  x16 vw_34;
  x8 vb_38;
  x8 vb_3c;

  ui16 uwordrg5 = eaxw;
  vb_38 = edxb;
  if (uwordrg5 == con(0xf))
    return con(0xffff);
  vw_28 = ddata.v1d3248;
  x16 wordrg2 = !ddata.v1e0234 ? con(0xffffffff) : ddata.v1e027c;
  vbool_18 = false;
  vw_34 = vw_30 = unsignedword(ddata.v1e0426[uwordrg5]);
  if (ddata.v1e0426[vw_30] == ddata.v1e0266 || vw_30 == wordrg2)
  {
    if (wordrg2 == con(0xffffffff))
    {
      do
      {
        vw_30++;
        if (vw_30 >= unsignedword(CUTX8(ddata.savegamep4->w0[con(0x2)])))
          vw_30 = con(0x0);
      } while (vw_30 == ddata.v1e0266);
      if (vw_30 == vw_34)
        vw_30 = ddata.v1e0266;
    }
    else
    {
      do
      {
        vw_30++;
        if (vw_30 >= unsignedword(CUTX8(ddata.savegamep4->w0[con(0x2)])))
          vw_30 = con(0x0);
        if (vw_30 == vw_34)
        {
          vw_30 = wordrg2;
          wordrg2 = con(0xffffffff);
        }
      } while (vw_30 == wordrg2 || vw_30 == ddata.v1e0266);
    }
  }
  vw_34 = vw_30;
  wordrg6 = con(0x0);
  vbool_04 = false;
  for(;;)
  {
    vw_2c = ulrshift(ddata.v1e03c8[vw_30].w8, con(0x5), con(0xb));
    vw_1c = urshift(ddata.v1e03c8[vw_30].w8, con(0xb));
    pb_14 = *(ddata.v1e03cc[vw_30]);
    pw_10 = &ddata.v1e038c[ddata.v1e03d8[ddata.v1e03e4[vw_30]]];
    vw_24 = con(0x0);
    // loop M_DD72
    while (vw_24 <= vw_2c)
    {
      // M_DD5B, co-label M_DD5A
      for (x16 wordrg3 = con(0x0);; wordrg3++)
      {
        if (wordrg3 > vw_1c)
          break;
        if ((*pb_14++ & con(0x10)) == con(0x0))
          continue; // loop M_DD5B
        vb_3c = con(0xffffffff);
        wordrg6 = *pw_10;
        pw_10++;
        bool boolrg1;
        if (vw_30 != ddata.v1e0266)
          boolrg1 = false;
        else
        {
          if ((vw_24 - ddata.v1e0270 + con(0x5)) > con(0xa))
            boolrg1 = false;
          else
          {
            if ((wordrg3 - ddata.v1e0272 + con(0x5)) > con(0xa))
              boolrg1 = false;
            else
              boolrg1 = true;
          }
        }
        bool vbool_0c = boolrg1;
        // loop M_DA34:
        for(;;)
        {
          bool farbreak = false;
          x16 wordrg41 = urshift(wordrg6 & con(0x3c00), con(0xa));
          bool doit = false;
          if (wordrg41 != con(0x3))
          {
            if (wordrg41 != con(0x2))
            {
              if (wordrg41 <= con(0x3) || !vbool_0c)
              {
                if (wordrg41 != uwordrg5)
                {
                  if (wordrg41 != con(0x4))
                    ;
                  else if (uwordrg5 == con(0xe))
                    ;
                  else
                  {
                    recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
                    if (recoptrrg4->u2.w != con(0xfffffffe))
                    {
                      if ((SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(recoptrrg4->u4.b.b0)->u0.b.b0 & con(0x1)) != con(0x0))
                      {
                        vbool_04 = true;
                        vw_20 = wordrg6;
                        wordrg6 = recoptrrg4->u2.w;
                        continue; // loop M_DA34
                      }
                    }
                  }
                }
                else
                {
                  bool dodc51 = false;
                  bool dodc7c = false;
                  recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
                  recoptrrg7 = recoptrrg1;
                  if (uwordrg5 < con(0x6))
                  {
                    if (uwordrg5 < con(0x4))
                      goto M_exit;
                    if (uwordrg5 > con(0x4))
                      dodc51 = true;
                    else
                    {
                      if ((recoptrrg1->ue.b.b1 & con(0x4)) == con(0x0))
                      {
                        if (recoptrrg1->u2.w == con(0xfffffffe))
                        {
                          if (vb_38 == con(0xffffffff) || vb_38 == recoptrrg7->u4.b.b0)
                          {
                            SKW_CHANGE_CURRENT_MAP_TO(vw_30);
                            SKW_DELETE_CREATURE_RECORD(vw_24, con(0x1), con(0xffffffff), wordrg3);
                            goto M_exit;
                          }
                        }
                      }
                    }
                  }
                  else if (uwordrg5 <= con(0x6))
                    dodc51 = true;
                  else if (uwordrg5 < con(0xa))
                  {
                    if (uwordrg5 != con(0x8))
                      goto M_exit;
                    if ((recoptrrg1->u2.b.b1 & con(0xffffff80)) != con(0x0))
                      doit = true;
                    else
                    {
                      wordrg41 = urshift(2 * recoptrrg1->u2.w, con(0x9));
                      dodc7c = true;
                    }
                  }
                  else if (uwordrg5 <= con(0xa))
                    dodc51 = true;
                  else if (uwordrg5 != con(0xe))
                    goto M_exit;
                  else if (vb_38 == con(0xffffffff) || SKW_QUERY_CLS2_FROM_RECORD(recoptrrg1->u2.w) == vb_38)
                  {
                    SKW_075f_056c(wordrg6);
                    x16 vw_08 = wordrg3;
                    x16 vw_00 = vw_24;
                    SKW_CUT_RECORD_FROM(wordrg6, vw_00, vw_08, NULL);
                    SKW_DELETE_MISSILE_RECORD(wordrg6, vw_00, vw_08, NULL);
                    goto M_exit;
                  }
                  if (dodc51)
                  {
                    if ((recoptrrg1->u2.b.b0 & con(0xffffff80)) == con(0x0))
                    {
                      wordrg41 = recoptrrg1->u2.w & con(0x7f);
                      dodc7c = true;
                    }
                  }
                  if (dodc7c)
                  {
                    if (CUTX8(wordrg41) != vb_3c)
                    {
                      if (vb_38 == con(0xffffffff) || CUTX8(wordrg41) == vb_38)
                      {
                        SKW_CHANGE_CURRENT_MAP_TO(vw_30);
                        SKW_MOVE_RECORD_TO(wordrg6, wordrg3, con(0xffffffff), vw_24, unsignedword(!vbool_04 ? 1 : 0) - con(0x1));
                        goto M_exit;
                      }
                    }
                    else
                      vb_3c = con(0xffffffff);
                  }
                }
              }
              doit = true;
            }
            else
            {
              recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
              if ((recoptrrg1->u2.w & con(0x6)) != con(0x2))
                doit = true;
              else
              {
                if ((urshift(urshift(recoptrrg1->u2.w, con(0x3)), con(0x8)) & con(0x1f)) != con(0x4))
                  doit = true;
              }
            }
          }
          else
          {
            recoptrrg7 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
            x16 wordrg42 = recoptrrg7->u2.w & con(0x7f);
            if (wordrg42 >= con(0x2c) || table1d324c[wordrg42] == con(0x0))
            {
              if (vbool_18)
              {
                if (vbool_0c)
                {
                  if (wordrg42 == con(0x30))
                  {
                    vbool_0c = false;
                    x16 wordrg43 = urshift(recoptrrg7->u2.w, con(0x7));
                    if (SKW_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(wordrg43) == uwordrg5)
                      vb_3c = SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(wordrg43);
                  }
                }
              }
              doit = true;
            }
          }
          for(;;)
          {
            if (doit)
            {
              wordrg6 = SKW_GET_NEXT_RECORD_LINK(wordrg6);
              if (wordrg6 != con(0xfffffffe))
                break;
            }
            if (!vbool_04)
            {
              farbreak = true;
              break;
            }
            wordrg6 = vw_20;
            vbool_04 = false;
          }
          if (farbreak)
            break;
        } // loop M_DA34
      }
      vw_24++;
    }
    if (vw_30 == ddata.v1e0266 || CUTX8(ddata.savegamep4->w0[con(0x2)]) <= con(0x1))
    {
      if (vbool_18)
      {
        ddata.v1e0426[uwordrg5] = CUTX8(vw_30);
        return con(0xffff);
      }
      vbool_18 = true;
    }
    else
    {
      if (wordrg2 == con(0xffffffff))
      {
        do
        {
          vw_30++;
          if (vw_30 >= unsignedword(CUTX8(ddata.savegamep4->w0[con(0x2)])))
            vw_30 = con(0x0);
        } while (vw_30 == ddata.v1e0266);
        if (vw_30 == vw_34)
          vw_30 = ddata.v1e0266;
      }
      else
      {
        do
        {
          vw_30++;
          if (vw_30 >= unsignedword(CUTX8(ddata.savegamep4->w0[con(0x2)])))
            vw_30 = con(0x0);
          if (vw_30 == vw_34)
          {
            vw_30 = wordrg2;
            wordrg2 = con(0xffffffff);
          }
        } while (vw_30 == wordrg2 || vw_30 == ddata.v1e0266);
      }
    }
  }
  // deadend here

M_exit:
  SKW_CHANGE_CURRENT_MAP_TO(vw_28);
  ddata.v1e0426[uwordrg5] = CUTX8(vw_30);
  return wordrg6 & con(0xffff3fff);
}

x8 SKW_0aaf_02f8(x8 eaxb, x8 edxb)
{
  ui16 uwordrg5;
  t_text* tptrrg2;
  t_text* tptrrg1;
  s_tww* mptrrg5;
  t_gfxdata* gptrrg1;
  t_gfxdata* gptrrg3;
  t_gfxdata* gptrrg4;
  t_gfxdata* gptrrg6;
  t_text t_00[0x320];
  t_text t_320[0x80];
  t_text t_3a0[0x80];
  t_text* tpa_420[0x14];
  t_text t_470[0x3c];
  s_tww v_4ac[0x2];
  c_rect rc_4bc;
  // TODO rest of the sequence a bit critical
  x16 vwa_4c4[0x2];
  x16 vw_4cc;
  x16 vw_4d4;
  x16 vw_4d8;
  x8 vb_4dc;
  x8 vb_4e0;
  x8 vb_4e4;

  vb_4e0 = eaxb;
  vb_4dc = edxb;
  bool vbool_4c8;
  if ((eaxb == con(0xe) || eaxb == con(0xffffff87)) && vb_4dc != con(0x0))
    vbool_4c8 = false;
  else
    vbool_4c8 = true;
  if (vb_4e0 == con(0x7) || vb_4e0 == con(0x13))
    if (SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0x1a), con(0x1), con(0x0), con(0x59)))
      vb_4e0 = con(0x59);
  if (vb_4dc != con(0x0) && vb_4e0 != con(0x0) && vb_4e0 != con(0xe))
    if (SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0x1a), con(0x1), con(0x0), con(0x0)))
    {
      SKW_0aaf_0067(SKW_0aaf_02f8(con(0x0), vb_4dc));
      vb_4dc = con(0x0);
    }
  if (vbool_4c8)
    SKW_FIRE_FADE_SCREEN(true);
  uwordrg5 = con(0x0);
  vb_4e4 = con(0x0);
  while (UI8CAST(vb_4e4) < UI8CAST(con(0x14)))
  {
    x16 wordrg6 = uwordrg5;
    tptrrg1 = SKW_QUERY_GDAT_TEXT(con(0x1a), vb_4e4, &t_00[40 * wordrg6], vb_4e0);
    tpa_420[wordrg6] = tptrrg1;
    if (*tptrrg1 != NULLBYTE)
      uwordrg5++;
    vb_4e4++;
  }
  bool opt = false; // added
  if (!ddata.v1e0a88)
    opt = true;
  else if (!dm2_dballochandler.gfxalloc_done)
  {
    if ((((unsignedlong(ddata.vx1d270c) * UI32CAST(((ddata.vx1d270a + con(0x1)) & con(0xfffffffe)) >> con(0x1))) & con(0xffff)) + con(0x1e)) > dm2_allochandler.bigpool_available)
      opt = true;
  }
  if (opt)
  {
    SKW_SET_ORIGIN_RECT(&rc_4bc, ddata.vx1d270a, ddata.vx1d270c);
    SKW_FIRE_FILL_BACKBUFF_RECT(&rc_4bc, ddata.paletteptr1[con(0x1)]);
    SKW_INFLATE_RECT(&rc_4bc, con(0xfffffff6), con(0xfffffff6));
    SKW_FIRE_FILL_BACKBUFF_RECT(&rc_4bc, ddata.paletteptr1[con(0x5)]);
    x16 wordrg64;
    if (uwordrg5 < UI16CAST(con(0x3)))
      wordrg64 = con(0x0);
    else if (uwordrg5 <= UI16CAST(con(0x3)))
      wordrg64 = con(0x1);
    else if (uwordrg5 == con(0x4))
      wordrg64 = uwordrg5;
    else
      wordrg64 = con(0x0);
    x16 wordrg31 = con(0x0);
    x16 wordrg11;
    while (wordrg11 = wordrg31, wordrg11 < uwordrg5)
    {
      wordrg11 += wordrg64;
      SKW_0aaf_01db(table1d27c4[wordrg11], false);
      wordrg31++;
    }
  }
  else
  {
    gptrrg1 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x1a), con(0x0), vb_4e0);
    gptrrg4 = gptrrg1;
    gptrrg6 = gptrrg1;
    if (!dm2_dballochandler.gfxalloc_done)
      gptrrg3 = &gptrrg4[SKW_CALC_IMAGE_BYTE_LENGTH(gptrrg1)];
    else
      gptrrg3 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(con(0x1a), con(0x0), vb_4e0);
//                               srcbmp                     palette
    SKW_DRAW_DIALOGUE_PARTS_PICT(gptrrg6, con(0xffffffff), gptrrg3, SKW_QUERY_EXPANDED_RECT(con(0x4), &rc_4bc));
    gptrrg1 = gptrrg6;
    SKW_FREE_PICT_ENTRY(gptrrg6);
  }
  SKW_DRAW_VP_RC_STR(con(0x1c2), v1d1044, unsignedword(ddata.paletteptr1[con(0xc)]));
  if (uwordrg5 >= con(0x2))
    vw_4d8 = con(0x1d7);
  else
    vw_4d8 = con(0x1d5);
  x16 wordrg63;
  if (uwordrg5 < UI16CAST(con(0x3)))
  {
    if (uwordrg5 != con(0x2))
      wordrg63 = con(0x0);
    else
      wordrg63 = con(0x1);
  }
  else if (uwordrg5 <= UI16CAST(con(0x3)))
    wordrg63 = con(0x3);
  else if (uwordrg5 == con(0x4))
    wordrg63 = con(0x6);
  else
    wordrg63 = con(0x0);
  x16 wordrg32 = con(0x0);
  x16 wordrg11;
  while (wordrg11 = wordrg32, wordrg11 < uwordrg5)
  {
    tptrrg2 = tpa_420[wordrg11];
    wordrg11 += signedlong(wordrg63);
    SKW_DRAW_VP_RC_STR(table1d27d4[wordrg11], tptrrg2, unsignedword(ddata.paletteptr1[con(0xb)]));
    wordrg32++;
  }
  if (vb_4e0 != con(0xffffff87))
  {
    SKW_QUERY_EXPANDED_RECT(vw_4d8, &rc_4bc);
    vw_4d4 = (rc_4bc.w);
    vw_4cc = (rc_4bc.h);
    if (vb_4dc == con(0x0) || vb_4e0 != con(0x0))
    {
      vwa_4c4[0x0] = unsignedword(ddata.paletteptr1[con(0xb)]);
      v_4ac[0x0].tp_00 = SKW_QUERY_GDAT_TEXT(con(0x1a), con(0x14), t_320, vb_4e0);
      if (vb_4dc == con(0x0))
        tptrrg1 = NULL;
      else
      {
        vwa_4c4[0x1] = unsignedword(ddata.paletteptr1[con(0xf)]);
        tptrrg1 = SKW_QUERY_GDAT_TEXT(con(0x1a), vb_4dc, t_3a0, con(0x0));
      }
      v_4ac[0x1].tp_00 = tptrrg1;
    }
    else
    {
      vwa_4c4[0x0] = unsignedword(ddata.paletteptr1[con(0xf)]);
      v_4ac[0x0].tp_00 = SKW_QUERY_GDAT_TEXT(con(0x1a), vb_4dc, t_320, vb_4e0);
      v_4ac[0x1].tp_00 = NULL;
    }
    x16 wordrg62 = con(0x0);
    mptrrg5 = v_4ac;
    for (x16 vw_4d0 = con(0x0); vw_4d0 < con(0x2); vw_4d0++, mptrrg5++)
    {
      tptrrg1 = mptrrg5->tp_00;
      if (tptrrg1 != NULL)
      {
        C_strmetrics.SKW_QUERY_STR_METRICS(tptrrg1, OO mptrrg5->w_06, OO mptrrg5->w_04);
        x16 wordrg111 = vw_4d4;
        x16 wordrg2 = wordrg111 - wordrg111 / 8;
        x16 wordrg406 = signedlong(mptrrg5->w_04);
        if (wordrg406 > wordrg2)
        {
          wordrg111 += wordrg111 / 4;
          if (wordrg406 >= wordrg111)
            mptrrg5->w_04 = vw_4d4;
          else
          {
            wordrg406 -= wordrg406 / 4;
            mptrrg5->w_04 = wordrg406;
          }
        }
        vw_4d8 = con(0x0);
        for(;;)
        {
          tptrrg1 = mptrrg5->tp_00;
          if (tptrrg1[vw_4d8] == NULLBYTE)
            break;
          SKW_3929_04e2(tptrrg1, &vw_4d8, mptrrg5->w_04, t_470);
          wordrg62 += 2 * ddata.v1d2736 + mptrrg5->w_06 - C_strmetrics.yofs + con(0x1);
          if (mptrrg5->tp_00[vw_4d8] == con(0xa))
          {
            vw_4d8++;
          }
        }
      }
    }
    if (wordrg62 != con(0x0))
    {
      x16 wordrg61 = rc_4bc.y + DM2_MAX(con(0x0), C_strmetrics.height + (vw_4cc - (wordrg62 - 2 * ddata.v1d2736)) / 2 - con(0x1));
      mptrrg5 = v_4ac;
      for (x16 vw_4d0_1 = con(0x0); vw_4d0_1 < con(0x2); vw_4d0_1++, mptrrg5++)
      {
        if (mptrrg5->tp_00 != NULL)
        {
          vw_4d8 = con(0x0);
          for(;;)
          {
            tptrrg1 = mptrrg5->tp_00;
            if (tptrrg1[vw_4d8] == NULLBYTE)
              break;
            SKW_DRAW_VP_STR((ddata.vx1d270a - SKW_3929_04e2(tptrrg1, &vw_4d8, mptrrg5->w_04, t_470)) / 2, vwa_4c4[vw_4d0_1], t_470, wordrg61);
            wordrg61 += ddata.strxplus + ddata.v1d2736;
            if (mptrrg5->tp_00[vw_4d8] == con(0xa))
              vw_4d8++;
          }
        }
      }
    }
  }
  if (vbool_4c8)
  {
    SKW_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
    SKW_FIRE_FADE_SCREEN(false);
  }
  ddata.backbuff2 = con(0x1);
  return vb_4e0;
}

// was SKW_3df7_014d
void SKW_UPDATE_WEATHER(bool eaxbool)
{
  x16 wordrg5;
  s_8wordarray vs_00;
  c_5bytes vs_10;
  s_xabytes* psxa_1c;
  x16 vw_28;
  bool flag;

  ddata.v1e147f = table1d6b76[4 * ddata.v1e1472 + 0x70];
  if (eaxbool)
  {
    x32 longrg15 = unsignedlong(++ddata.v1e147b);
    if (longrg15 <= con(0x1f))
    {
      ddata.v1e146e = ddata.v1e1474;
      ddata.v1e1474 += ddata.v1e1484 * signedword(ddata.v1d7108[(unsignedlong(ddata.v1e1478) << con(0x5)) + longrg15]);
      ddata.v1e1474 = DM2_BETWEEN_VALUE(con(0x0), con(0xff), ddata.v1e1474);
      SKW_SET_TIMER_WEATHER(SKW_RAND16(con(0x100)) + con(0x32));
    }
    else
      SKW_3df7_0037(false);
    return;
  }
  if (UI32CAST(ddata.longmallocx) >= UI32CAST(ddata.longv1e1434))
  {
    ddata.v1e1476 = signedword(table1d70f0[((ddata.longmallocx + ddata.longv1e1438) / con(0x555)) % con(0x18)]);
    ddata.longv1e1434 = ddata.longmallocx + con(0x555);
    if (ddata.v1e147f != con(0x0))
      SKW_RECALC_LIGHT_LEVEL();
  }
  vw_28 = con(0x0);
  if (ddata.v1e1474 == con(0x0))
  {
    if (UI8CAST(ddata.v1e147d) > con(0x0))
      if ((ddata.longmallocx % con(0x3)) == con(0x0))
        ddata.v1e147d--;
    vw_28 = (SKW_RAND16(con(0x40)) == con(0x0)) ? 1 : 0;
    ddata.v1e1482 = con(0x0);
    ddata.v1e147e = con(0x1);
  }
  else
  {
    x16 wordrg21 = con(0x100) - ddata.v1e1474 + (CUTX16(SKW_RAND()) & con(0xf));
    wordrg5 = (ddata.v1e1474 < con(0xcd)) ? con(0x7) : con(0x28);
    x8 byterg11 = CUTX8(ddata.v1e1474);
    ddata.v1e147e = byterg11;
    if (ddata.v1e1482 == con(0x0))
      byterg11 = (SKW_RAND16(wordrg21) <= con(0x7)) ? 1 : 0;
    ddata.v1e1482 = byterg11;
    if (ddata.v1e1482 != con(0x0))
    {
      ui32 ulongrg1 = unsignedlong(ddata.v1e147d);
      if (ulongrg1 < con(0xff)) // TODO: being casted unsigned long, less 0xff means != 0xff
      {
        ulongrg1++; // was RG1Blo++ only
        if (ddata.v1e1482 < con(0x80) && (ddata.v1e1482 < con(0x40) || ((CUTLX8(ddata.longmallocx) & con(0x1)) != con(0x0))))
        {
          if ((ddata.v1e1482 >= con(0x10) && ((ddata.longmallocx % con(0x3)) == con(0x0))) || ((CUTLX8(ddata.longmallocx) & con(0x3)) == con(0x0)))
            ddata.v1e147d++;
        }
        else
          ddata.v1e147d = CUTX8(ulongrg1);
      }
    }
    if (ddata.v1e1481 != con(0x0))
    {
      x16 wordrg406 = SKW_RAND16(wordrg21);
      vw_28 = (wordrg406 <= wordrg5) ? 1 : 0;
    }
  }
  if (ddata.v1e147f == con(0x0))
    return;
  if (ddata.v1e024c)
  {
    ddata.v1e024c = false;
    SKW_RECALC_LIGHT_LEVEL();
  }
  psxa_1c = ddata.v1e143c;
  if (ddata.v1e147a != con(0x0))
  {
    x16 wordrg105 = unsignedword(ddata.v1e147e);
    if (wordrg105 >= con(0x10))
    {
      if (wordrg105 < con(0x80))
      {
        if (wordrg105 < con(0x40))
          ddata.v1e143c[0x0].b0 = con(0x67);
        else
          ddata.v1e143c[0x0].b0 = con(0x68);
      }
      else
      {
        ddata.v1e1480 = con(0x1);
        ddata.v1e143c[0x0].b0 = con(0x69);
      }
      if (SKW_RETRIEVE_ENVIRONMENT_CMD_CD_FW(psxa_1c))
        psxa_1c++;
    }
  }
  if (ddata.v1e1479 != con(0x0))
  {
    x16 wordrg104 = unsignedword(ddata.v1e147d);
    if (wordrg104 >= con(0x40))
    {
      if (wordrg104 < con(0xc0))
      {
        if (wordrg104 < con(0x80))
          psxa_1c->b0 = con(0x6a);
        else
          psxa_1c->b0 = con(0x6b);
      }
      else
        psxa_1c->b0 = con(0x6c);
      if (SKW_RETRIEVE_ENVIRONMENT_CMD_CD_FW(psxa_1c))
        psxa_1c++;
    }
  }
  if (vw_28 != con(0x0))
  {
    vw_28 = con(0x0);
    bool vbool_18 = ddata.v1e1474 < con(0xb6);
    if (vbool_18)
      SKW_UPDATE_GLOB_VAR(con(0x40), con(0x6), false);
    x16 vw_2c = con(0x0);
    if (SKW_RAND16(ddata.v1e1474 + con(0x1)) >= con(0x3c))
    {
      vw_2c = (CUTX16(SKW_RAND()) & con(0x7)) + con(0x1);
      x16 wordrg110 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x8), con(0xb), con(0x6c), ddata.v1d6c02);
      if (wordrg110 == con(0x0))
      {
        for(;;)
        {
          bool pass = false; // added
          x16 wordrg405 = SKW_RAND16(con(0x20));
          x16 wordrg6 = wordrg405;
          wordrg5 = SKW_RAND16(con(0x20));
          if (wordrg405 >= con(0x0) && wordrg405 < ddata.savegamewa)
          {
            if (wordrg5 >= con(0x0) && wordrg5 < ddata.savegamewb)
            {
              SKW_SUMMARIZE_STONE_ROOM(&vs_00, wordrg6, wordrg5, con(0x0));
              if (vs_00.warr_00[0x0] == con(0x1))
              {
                if (!SKW_GET_TELEPORTER_DETAIL(&vs_10, wordrg5, wordrg6))
                {
                  if (ddata.v1e0266 != ddata.v1d3248 || wordrg6 != ddata.v1e0270 || wordrg5 != ddata.v1e0272 || ((ddata.v1e1474 >= con(0xb6) && (UI32CAST(ddata.longmallocx) >= UI32CAST(con(0x2ee0)))) && SKW_GET_GLOB_VAR(con(0x40)) > ddata.v1e1470))
                  {
                    x16 wordrg182 = SKW_GET_CREATURE_AT(wordrg6, wordrg5);
                    if (wordrg182 != con(0xffffffff))
                    {
                      if (SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(wordrg182)->b2 == con(0xffffffff))
                        pass = true;
                      else
                      {
                        x16 wordrg181 = SKW_CALC_SQUARE_DISTANCE(ddata.v1e0270, wordrg6, wordrg5, ddata.v1e0272);
                        if (wordrg181 > con(0x2))
                        {
                          if (wordrg181 > con(0x3))
                            pass = true;
                          else if (ddata.v1e0258 != SKW_CALC_VECTOR_DIR(ddata.v1e0270, wordrg6, wordrg5, ddata.v1e0272))
                            pass = true;
                        }
                      }
                    }
                    if (!pass)
                    {
                      SKW_CREATE_CLOUD(con(0xffb0), wordrg6, wordrg5, ddata.v1e1474, con(0xff));
                      if ((ddata.v1e0976 != con(0x0) || ddata.v1e0238) && !vbool_18)
                        if (SKW_CALC_SQUARE_DISTANCE(ddata.v1e0270, wordrg6, wordrg5, ddata.v1e0272) <= con(0x3))
                          if (ddata.v1e0258 == SKW_CALC_VECTOR_DIR(ddata.v1e0270, wordrg6, wordrg5, ddata.v1e0272))
                            SKW_UPDATE_GLOB_VAR(con(0x41), con(0x3), true);
                      break;
                    }
                  }
                }
              }
            }
          }
          vw_2c--;
          flag = vw_2c != con(0x0);
          if (!flag)
            break;
        }
      }
      else
      {
        wordrg5 = wordrg110 & con(0xff);
        x16 wordrg61 = urshift(wordrg110, con(0x8));
        SKW_INVOKE_MESSAGE(wordrg61, con(0x0), con(0x0), wordrg5, ddata.longmallocx + con(0x1));
        SKW_QUEUE_NOISE_GEN2(con(0xd), con(0x81), con(0xfe), con(0x30), wordrg61, wordrg5, con(0x1), con(0x6c), SKW_RAND16(con(0x20)) + con(0x80)); // BUGFIX 2/2014
      }
    }
    if (vw_2c == con(0x0) && SKW_RAND01() != con(0x0))
    {
      psxa_1c->b0 = CUTX8(SKW_RAND16(con(0x3))) + con(0x64);
      if (SKW_RETRIEVE_ENVIRONMENT_CMD_CD_FW(psxa_1c))
      {
        vw_28 = con(0x1);
        SKW_098d_04c7(psxa_1c->w2, SKW_RAND16(con(0x64)), UPCAST(x16, psxa_1c + con(0x4)), psxa_1c->w2 + con(0x1), UPCAST(x16, psxa_1c + con(0x6)));
        psxa_1c->b1 = CUTX8(SKW_RAND02());
        psxa_1c++;
      }
    }
    if (vw_28 != con(0x0))
    {
      if (ddata.v1d718c)
        ddata.v1d718c = false;
      else
      {
        x16 wordrg103;
        if (ddata.v1e1474 != con(0x0))
          wordrg103 = con(0x4c) - ddata.v1e1474 / ddata.v1e1484;
        else
          wordrg103 = SKW_RAND16(con(0xa)) + con(0x5);
        SKW_QUEUE_NOISE_GEN1(con(0x17), con(0x0), con(0x19), ddata.v1d6c02, con(0x40), ddata.v1e0270, ddata.v1e0272, DM2_BETWEEN_VALUE(con(0x1), con(0xf), wordrg103));
        ddata.v1d718c = true;
      }
    }
    ddata.v1e024c = true;
    SKW_RECALC_LIGHT_LEVEL();
  }
  psxa_1c->b0 = con(0xffffffff);
}

// was SKW_32cb_5824
void SKW_ENVIRONMENT_DISPLAY_ELEMENTS(x16 eaxw, x16 ebxw, x16 edxw)
{
  x8 byterg5;
  s_xabytes* sxaptrrg5;
  s_xabytes* sxaptrrg1;
  t_text* tptrrg1;
  x16 vw_04;
  x16 vw_08;
  t_text t_0c[0x80];
  x8 vb_90;

  vw_08 = eaxw;
  vw_04 = edxw;
  if (ddata.v1e147f != con(0x0))
  {
    sxaptrrg5 = ddata.v1e143c;
    while (sxaptrrg5->b0 != con(0xffffffff))
    {
      sxaptrrg1 = sxaptrrg5++;
      SKW_ENVIRONMENT_DRAW_DISTANT_ELEMENT(sxaptrrg1, vw_04, ebxw, vw_08);
    }
  }
  if (ddata.v1d7041 != ddata.v1d6c02)
  {
    ddata.v1d7041 = ddata.v1d6c02;
    ddata.v1e13e9 = con(0x1);
    DM2_ZERO_MEMORY(ddata.v1e13dc, con(0xd));
    vb_90 = con(0x0);
    while (vb_90 <= UI8CAST(con(0x63)))
    {
      byterg5 = vb_90;
      if (SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0x17), con(0x1), byterg5, ddata.v1d6c02))
      {
        tptrrg1 = SKW_QUERY_GDAT_TEXT(con(0x17), byterg5, t_0c, ddata.v1d6c02);
        if (*tptrrg1 != NULLBYTE)
        {
          byterg5 >>= con(0x3);
          ddata.v1e13dc[byterg5] |= con(0x1) << (vb_90 & con(0x7));
          ddata.v1e13e9 = con(0x0);
        }
      }
      vb_90++;
    }
  }
  if (ddata.v1e13e9 != con(0x0))
    return;
  sxaptrrg5 = ddata.v1e143c;
  vb_90 = ddata.v1e13e9;
  while (UI8CAST(vb_90) <= UI8CAST(con(0x63)))
  {
    x32 longrg400 = unsignedlong(ddata.v1e13dc[vb_90 / 8]);
    if (((con(0x1) << (vb_90 & con(0x7))) & longrg400) != con(0x0))
    {
      SKW_QUERY_GDAT_TEXT(con(0x17), vb_90, t_0c, ddata.v1d6c02);
      if (SKW_ENVIRONMENT_SET_DISTANT_ELEMENT(sxaptrrg5, vw_08, vw_04, t_0c, ebxw))
      {
        sxaptrrg5->b0 = vb_90;
        sxaptrrg5->w2 = SKW_QUERY_CMDSTR_TEXT(t_0c, v1d1134);
        sxaptrrg5->b1 = CUTX8(SKW_QUERY_CMDSTR_TEXT(t_0c, v1d1137));
        SKW_ENVIRONMENT_DRAW_DISTANT_ELEMENT(sxaptrrg5, vw_04, ebxw, vw_08);
      }
    }
    vb_90++;
  }
}

// was SKW_32cb_56bc
void SKW_ENVIRONMENT_DRAW_DISTANT_ELEMENT(s_xabytes* eaxpxa, x16 ebxw, x16 ecxw, x16 edxw)
{
  x16 vw_00;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;
  x16 vw_10;

  vw_04 = ebxw;
  vw_08 = ecxw;
  if (eaxpxa->w2 == con(0x0))
    return;
  if (unsignedword(eaxpxa->b1) == con(0x8) && SKW_SET_GRAPHICS_FLIP_FROM_POSITION(con(0x1), vw_04, vw_08, edxw) != con(0x0))
    vw_00 = con(0x1);
  else
  {
    if (unsignedword(eaxpxa->b1) == con(0x2) && SKW_SET_GRAPHICS_FLIP_FROM_POSITION(con(0x20), vw_04, vw_08, edxw) != con(0x0))
      vw_00 = con(0x1);
    else
    {
      if (unsignedword(eaxpxa->b1) == con(0x40) && SKW_SET_GRAPHICS_FLIP_FROM_POSITION(con(0x1), vw_04, vw_08, edxw) != con(0x0))
        vw_00 = con(0x1);
      else
      {
        if (unsignedword(eaxpxa->b1) == con(0x20) && SKW_SET_GRAPHICS_FLIP_FROM_POSITION(unsignedword(eaxpxa->b1), vw_04, vw_08, edxw) != con(0x0))
          vw_00 = con(0x1);
        else
          vw_00 = con(0x0);
      }
    }
  }
  x16 wordrg3 = unsignedword(eaxpxa->b8);
  vw_0c = unsignedword(eaxpxa->b9);
  vw_10 = eaxpxa->w4;
  x16 wordrg6 = eaxpxa->w6;
  if (ddata.v1e025c != con(0x0))
  {
    x16 wordrg2 = SKW_CALC_STRETCHED_SIZE(vw_10, con(0x34));
    vw_10 = wordrg2;
    wordrg6 = SKW_CALC_STRETCHED_SIZE(wordrg6, con(0x34));
    wordrg3 = SKW_CALC_STRETCHED_SIZE(wordrg3, con(0x34));
    vw_0c = SKW_CALC_STRETCHED_SIZE(vw_0c, con(0x34));
    if (eaxpxa->b8 != con(0x40))
      wordrg6 += ddata.v1d703d;
    else
    {
      vw_10 = ddata.vx1d2712.x + wordrg2;
      if (eaxpxa->w2 != con(0x1771))
        wordrg6 += ddata.vx1d2712.y;
      else
        wordrg6 += ddata.v1d703f;
    }
  }
  SKW_QUERY_TEMP_PICST(vw_00, vw_0c, con(0x0), wordrg3, con(0x0), con(0x0), eaxpxa->w2 | con(0x8000), con(0xffffffff), ddata.v1d6c00, con(0xffff), con(0x17), ddata.v1d6c02, eaxpxa->b0);
  ddata.v1e118e.w_20 += vw_10;
  ddata.v1e118e.w_22 += wordrg6;
  SKW_DRAW_TEMP_PICST();
}

