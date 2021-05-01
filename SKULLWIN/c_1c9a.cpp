#include "dm2data.h"
#include "c_350.h"
#include "util.h"
#include "startend.h"
#include "c_querydb.h"
#include "c_random.h"
#include "c_hero.h"
#include "c_sfx.h"
#include "c_map.h"
#include "mdata.h"
#include "c_light.h"
#include "c_move.h"
#include "c_cloud.h"
#include "c_creature.h"
#include "c_ai.h"
#include "c_1c9a.h"

#include "emu.h"
#include "regs.h"

// also called indirect
// adress given to a functionpointer
static i32 DM2_1BAAD(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 vw_00;
  i16 vw_04;

  vw_04 = RG1W; // TODO input word only
  vw_00 = RG4W; // TODO input word only
  RG1L = dm2_GET_TILE_VALUE(signedlong(vw_04), signedlong(vw_00));
  RG2W = unsignedword(RG2Blo);
  RG2Blo = RG1Blo;
  RG1L = unsignedlong(RG1Blo) >> 5;
  RG3W = unsignedword(RG3Blo);
  RG3Blo = RG1Blo;
  if (RG3W == 0)
    return lcon(0x1);
  RG1L = unsignedlong(RG3W);
  if (RG1L == lcon(0x4))
  {
    bool skip00392 = false;
    RG1Blo = RG2Blo & lcon(0x7);
    RG4W = unsignedword(RG4Blo);
    RG4Blo = RG1Blo;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == lcon(0x3))
      skip00392 = true;
    else
    {
      RG1L = unsignedlong(RG4W);
      if (RG1L == lcon(0x4))
        skip00392 = true;
    }

    if (skip00392)
    {
      bool skip00394 = false;
      //m_1BB03:
      RG4L = signedlong(vw_00);
      RG1L = DM2_query_0cee_3275(DM2_IS_REBIRTH_ALTAR(UPCAST(unk, DOWNCAST(t_record, DM2_GET_ADDRESS_OF_TILE_RECORD(vw_04, RG4W)))) & lcon(0xff));
      if (RG1L != 0)
      {
        RG1L = DM2_RANDBIT() ? 1 : 0;
        if (RG1W != 0)
        {
          //m_1BB3A:
          RG1L = 0;
          skip00394 = true;
        }
      }

      if (!skip00394)
        //m_1BB33:
        RG1L = 1;

      //m_1BB3C:
      if (RG1L != 0)
        return lcon(0x1);
    }
  }
  //m_1BB44:
  RG1L = unsignedlong(RG3W);
  if (RG1L == lcon(0x6))
  {
    RG1L = RG2L;
    RG1Bhi ^= RG2Bhi;
    RG1Blo &= 0x4;
    RG1L = unsignedlong(RG1W);
    if (RG1L == 0)
      return lcon(0x1);
  }
  if ((RG2Blo & bcon(0x10)) == 0)
    return 0;
  RG1L = DM2_GET_WALL_TILE_ANYITEM_RECORD(signedlong(vw_04), vw_00);
  //m_1BB7B:
  for (;;)
  {
    RG3L = RG1L;
    if (RG3W == lcon(0xfffffffe))
      return 0;
    RG1L = RG3L;
    RG1Blo ^= RG3Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    RG4L = RG1L;
    RG1L = signedlong(RG1W);
    if (RG1L == lcon(0xf))
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
      RG1W = word_at(RG1P, 2) & lcon(0x7f);
      if (RG1W == lcon(0xe))
        return lcon(0x1);
    }
    RG1L = signedlong(RG4W);
    if (RG1L == lcon(0x4))
    {
      RG2L = lcon(0xff);
      RG4P = ADRESSOF(i16, &vw_00);
      RG1L = DM2_query_1c9a_03cf(&vw_04, RG4P16, RG2L);
      RG4L = RG1L;
      RG1Blo = RG1W != wcon(0xffff) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L != 0)
      {
        RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG4W));
        if ((RG1Blo & bcon(0x1)) == 0)
        {
          //m_1BC07:
          if ((RG1Blo & bcon(0x20)) == 0)
            return lcon(0x1);
        }
        else
        {
          RG1L = unsignedlong(RG1W) >> 6;
          RG1L &= 0x3;
          RG1L = unsignedlong(RG1W);
          if (RG1L < 2)
            return lcon(0x1);
        }
      }
    }
    //m_1BC0B:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG3UW)));
  }
  FOREND
}

static i32 DM2_1BC29(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4; RG4L = edxl;

  if (ddat.v1d3248 == ddat.v1e08d6 && RG1W == ddat.v1e08d8 && RG4W == ddat.v1e08d4)
    return lcon(0x1);
  return DM2_1BAAD(signedlong(RG1W), signedlong(RG4W));
}

// stacksize was 0x40
i32 DM2_19f0_0207(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, FPTR_L_LL argf0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_wlreg _RG6;
  i32 RG7l;
  i32 vl_00;
  i16 vw_04;
  i16 vw_08;
  i16 vw_0c;
  i16 vw_10;
  i32 vl_14;
  i16 vw_18;
  i16 vw_1c;
  i16 vw_20;
  i16 vw_24;
  i16 vw_28;
  i16 vw_2c;
  bool skip00396 = false;

  RG6L = RG1L;
  vw_2c = RG4W;
  vw_24 = RG2W;
  vw_20 = RG3W;
  RG1L -= RG2L;
  RG1L = signedlong(DM2_ABS(RG1W));
  RG4L = signedlong(RG1W);
  RG1L = signedlong(DM2_ABS(vw_2c - RG3W)) + RG4L;
  if (RG1L <= lcon(0x1))
    return lcon(0x1);
  RG3L = RG2L - RG6L;
  if (RG3W >= 0)
    //m_1BCAE:
    RG1L = signedlong(RG3W);
  else
    RG1L = -signedlong(RG3W);
  //m_1BCB1:
  RG3L = RG1L;
  RG2W = vw_20 - vw_2c;
  if (RG2W >= 0)
    //m_1BCC7:
    RG1L = signedlong(RG2W);
  else
    RG1L = -signedlong(RG2W);
  //m_1BCCA:
  RG2L = RG1L;
  RG1Blo = RG3W < RG1W ? 1 : 0;
  RG1W = unsignedword(RG1Blo);
  vw_10 = RG1W; // truncated
  RG1Blo = RG3W == RG2W ? 1 : 0;
  vw_28 = RG1W; // truncated
  RG3L = unsignedlong(vw_24);
  RG1L = signedlong(vw_24) - signedlong(RG6W);
  if (RG1W <= 0)
    //m_1BCFE:
    RG1L = 1;
  else
    RG1L = lcon(0xffffffff);
  //m_1BD03:
  vw_18 = RG1W;
  RG2L = unsignedlong(vw_20);
  RG1L = signedlong(vw_20) - signedlong(vw_2c);
  if (RG1W <= 0)
    //m_1BD28:
    RG5L = 1;
  else
    RG5L = lcon(0xffffffff);
  //m_1BD2D:
  if (vw_10 == 0)
  {
    //m_1BD66:
    RG1L = RG3L;
    RG1W -= RG6W;
    if (RG1W == 0)
      //m_1BD7B:
      RG1L = lcon(0x80);
    else
    {
      RG4L = signedlong(RG2W);
      RG7l = signedlong(vw_2c);
      RG4L -= RG7l;
      skip00396 = true;
    }
  }
  else
  {
    RG1L = RG2L;
    RG1W -= vw_2c;
    if (RG1W == 0)
      //m_1BD5F:
      RG1L = lcon(0x80);
    else
    {
      RG4L = signedlong(RG3W);
      vl_00 = RG4L;
      RG4L = signedlong(RG6W);
      RG7l = vl_00 - RG4L;
      RG4L = RG7l;
      skip00396 = true;
    }
  }

  if (skip00396)
  {
    //m_1BD4E:
    RG4L <<= bcon(0x6);
    RG1L = signedlong(RG1W);
    vl_00 = RG1L;
    RG1L = RG4L;
    RG4L >>= bcon(0x1f);
    RG4L = RG1L % vl_00;
    RG1L /= vl_00;
  }
  //m_1BD80:
  vw_08 = RG1W;
  //m_1BD85:
  for (;;)
  {
    bool skip00402 = false;
    RG1L = unsignedlong(vw_18) + RG3L;
    vw_0c = RG1W;
    if (vw_28 == 0)
    {
      bool skip00398 = false;
      //m_1BDDB:
      if (vw_10 == 0)
      {
        //m_1BE1D:
        RG1L = unsignedlong(vw_0c);
        RG1W -= RG6W;
        if (RG1W == 0)
          //m_1BE38:
          RG1L = lcon(0x80);
        else
        {
          RG4L = signedlong(RG2W);
          vl_00 = RG4L;
          RG4L = signedlong(vw_2c);
          RG7l = vl_00;
          skip00398 = true;
        }
      }
      else
      {
        RG1L = RG2L;
        RG1W -= vw_2c;
        if (RG1W == 0)
          //m_1BE16:
          RG1L = lcon(0x80);
        else
        {
          vl_00 = signedlong(RG3W);
          RG4L = signedlong(vw_18);
          RG7l = vl_00 + RG4L;
          RG4L = signedlong(RG6W);
          skip00398 = true;
        }
      }

      if (skip00398)
      {
        //m_1BE01:
        RG7l -= RG4L;
        RG4L = RG7l << 6;
        RG1L = signedlong(RG1W);
        vl_00 = RG1L;
        RG1L = RG4L;
        RG4L >>= bcon(0x1f);
        RG4L = RG1L % vl_00;
        RG1L /= vl_00;
      }
      bool skip00401 = false;
      //m_1BE3D:
      RG1L -= signedlong(vw_08);
      if (RG1W >= 0)
        //m_1BE52:
        RG4L = signedlong(RG1W);
      else
        RG4L = -signedlong(RG1W);
      //m_1BE55:
      vl_14 = RG4L;
      if (vw_10 == 0)
      {
        //m_1BE94:
        RG1L = RG3L;
        RG1W -= RG6W;
        if (RG1W == 0)
        {
          //m_1BEB2:
          RG1L = lcon(0x80);
          skip00401 = true;
        }
        else
        {
          RG4L = signedlong(RG2W);
          vl_00 = RG4L;
          RG4L = signedlong(RG5W) + vl_00;
          RG7l = signedlong(vw_2c);
          RG4L -= RG7l;
        }
      }
      else
      {
        RG1L = RG2L + RG5L;
        RG1W -= vw_2c;
        if (RG1W == 0)
        {
          //m_1BE8D:
          RG1L = lcon(0x80);
          skip00401 = true;
        }
        else
        {
          RG4L = signedlong(RG3W);
          vl_00 = RG4L;
          RG4L = signedlong(RG6W);
          RG7l = vl_00 - RG4L;
          RG4L = RG7l;
        }
      }

      if (!skip00401)
      {
        //m_1BE7C:
        RG4L <<= bcon(0x6);
        RG1L = signedlong(RG1W);
        vl_00 = RG1L;
        RG1L = RG4L;
        RG4L >>= bcon(0x1f);
        RG4L = RG1L % vl_00;
        RG1L /= vl_00;
      }

      //m_1BEB7:
      RG1L -= signedlong(vw_08);
      if (RG1W >= 0)
        //m_1BECA:
        RG1L = signedlong(RG1W);
      else
        RG1L = -signedlong(RG1W);
      //m_1BECB:
      vw_04 = RG1W; // truncated
      RG1L = vl_14;
      if (RG1W >= vw_04)
        //m_1BEE0:
        RG2L += RG5L;
      else
        RG3L += unsignedlong(vw_18);
      //m_1BEE2:
      RG4L = signedlong(RG2W);
      RG1L = INDCALL(argf0)(signedlong(RG3W), RG4L);
      if (RG1L != 0)
      {
        RG1L = vl_14;
        if (RG1W != vw_04)
          return 0;
        RG2L -= RG5L;
        skip00402 = true;
      }
    }
    else
    {
      RG4L = signedlong(RG2W);
      RG1L = INDCALL(argf0)(signedlong(vw_0c), RG4L);
      if (RG1L != 0)
      {
        RG1L = RG2L + RG5L;
        RG4L = signedlong(RG1W);
        RG1L = INDCALL(argf0)(signedlong(RG3W), RG4L);
        if (RG1L != 0)
          return 0;
      }
      RG2L += RG5L;
      skip00402 = true;
    }

    if (skip00402)
    {
      //m_1BDBE:
      RG4L = signedlong(RG2W);
      RG3L += unsignedlong(vw_18);
      RG1L = INDCALL(argf0)(signedlong(RG3W), RG4L);
      if (RG1L != 0)
        return 0;
    }

    //m_1BF06:
    RG1L = signedlong(DM2_ABS(CUTX16(RG3L - RG6L)));
    RG4L = signedlong(RG1W);
    RG1W = RG2W - vw_2c;
    vw_1c = RG1W;
    RG1L = signedlong(DM2_ABS(vw_1c)) + RG4L;
    if (RG1L <= lcon(0x1))
    {
      RG3L = signedlong(vw_20);
      RG2L = signedlong(vw_24);
      RG4L = signedlong(vw_2c);
      RG1W = DM2_CALC_SQUARE_DISTANCE(RG6W, RG4W, RG2W, RG3W);
      return RG1L;
    }
  }
  FOREND
}

i32 DM2_19f0_045a(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG4; RG4L = edxl;

  if (RG1W == ddat.v1e08a8 && RG4W == ddat.v1e08aa)
  {
    RG2W = ddat.v1d3248;
    if (RG2W == ddat.v1e08ac)
      return RG1L;
  }
  ddat.v1e08ac = ddat.v1d3248;
  ddat.v1e08aa = RG4W;
  RG4L = signedlong(RG4W);
  ddat.v1e08a8 = RG1W;
  RG1L = dm2_GET_TILE_VALUE(signedlong(RG1W), RG4L);
  RG1W = unsignedword(RG1Blo);
  ddat.v1e08ae = RG1W;
  RG1Blo = (ddat.v1e08ae & wcon(0x10)) != 0 ? 1 : 0;
  RG1W = unsignedword(RG1Blo);
  RG1L += lcon(0xfffe);
  ddat.v1e08b4 = RG1W;
  ddat.v1e08b2 = RG1W;
  ddat.v1e08b0 = RG1W;
  RG4Blo = 0;
  ddat.v1e08b6 = RG4Blo;
  ddat.v1e08b7 = RG4Blo;
  ddat.v1e08be = lcon(0xffffffff);
  ddat.v1e08c4 = 1;
  return RG1L;
}

static i32 DM2_19f0_04bf(void)
{
  c_nreg _RG1;
  c_nreg _RG4;

  RG1W = ddat.v1e08b2;
  if (RG1W != lcon(0xffffffff))
    return RG1L;
  RG1W = ddat.v1e08b0;
  if (RG1W == lcon(0xffffffff))
  {
    RG4L = signedlong(ddat.v1e08aa);
    RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(ddat.v1e08a8, RG4W));
    ddat.v1e08b0 = RG1W;
  }
  //m_1C027:
  for (;;)
  {
    RG4Blo = RG1W != wcon(0xfffe) ? 1 : 0;
    RG4L = unsignedlong(RG4Blo);
    if (RG4L != 0)
    {
      RG4L = RG1L;
      RG4Blo ^= RG1Blo;
      RG4Bhi &= lcon(0x3c);
      RG4L = unsignedlong(RG4W) >> bcon(0xa);
      if (RG4L <= lcon(0x3))
      {
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG1UW)));
        continue;
      }
    }
    break;
  }
  //m_1C057:
  ddat.v1e08b2 = RG1W;
  return RG1L;
}

static i32 DM2_19f0_050f(void)
{
  c_nreg _RG1;
  c_nreg _RG4;

  RG1W = ddat.v1e08b4;
  if (RG1W != lcon(0xffffffff))
    return RG1L;
  RG1L = DM2_19f0_04bf();
  //m_1C071:
  for (;;)
  {
    RG4Blo = RG1W != wcon(0xfffe) ? 1 : 0;
    RG4L = unsignedlong(RG4Blo);
    if (RG4L != 0)
    {
      RG4L = RG1L;
      RG4Blo ^= RG1Blo;
      RG4Bhi &= lcon(0x3c);
      RG4L = unsignedlong(RG4W) >> bcon(0xa);
      if (RG4L != lcon(0x4))
      {
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG1UW)));
        continue;
      }
    }
    break;
  }
  //m_1C0A1:
  ddat.v1e08b4 = RG1W;
  return RG1L;
}

// belongs to DM2_19f0_05e8
static i32 DM2_19f0_0547(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4; RG4L = edxl;

  return DM2_CREATURE_CAN_HANDLE_IT(unsignedlong(RG1W), signedlong(RG4W));
}

i32 DM2_19f0_0559(i16 eaxw)
{
  c_nreg _RG1; RG1L = signedlong(eaxw);
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG5l;
  bool skip00405 = false;
  bool skip00404 = false;

  RG4P = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG4W = word_at(RG4P, lcon(0xe));
  RG4L <<= bcon(0x6);
  RG4UW >>= 14;
  RG2L = signedlong(RG1W);
  RG5l = RG2L + 2 & lcon(0x3);
  RG3L = signedlong(RG4W);
  if (RG3L != RG5l)
  {
    //m_1C0F3:
    if (RG4W == RG1W)
    {
      //m_1C102:
      mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1a)), 0);
      s350.v1e056f = lcon(0xfffffffe);
      return 0;
    }
    RG2L--;
    RG2L &= 0x3;
    if (RG3L != RG2L)
      skip00405 = true;
    else
      skip00404 = true;
  }
  else
  {
    RG1L = DM2_RANDBIT() ? 1 : 0;
    if (RG1W == 0)
      skip00405 = true;
    else
      skip00404 = true;
  }

  if (skip00404)
    //m_1C0E5:
    RG1L = 1;

  if (skip00405)
    //m_1C0EC:
    RG1L = lcon(0xffffffff);

  //m_1C119:
  RG2Blo = RG4Blo + RG1Blo & lcon(0x3);
  mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1d)), RG2Blo);
  RG1L = signedlong(RG1W);
  RG1Blo = RG1L != lcon(0xffffffff) ? 1 : 0;
  RG1L = unsignedlong(RG1Blo) + lcon(0x6);
  RG4Blo = RG1Blo;
  mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1a)), RG4Blo);
  s350.v1e056f = lcon(0xfffffffc);
  return lcon(0x1);
}

// stacksize was 0x3c
static i32 DM2_19f0_05e8(i32 eaxl, unk* xedxp, unk* xebxp, i32 ecxl, i16 argw0, i32 argl1, i32 argl2)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4P = xedxp;
  c_wlreg _RG5; // TODO: probably more
  i8* RG52p;
  c_wlreg _RG61;
  i32 RG62l;
  c_ol32 vol_00; // poke 16 32 peek 32 add16
  unk* xp_04;
  unk* xp_08;
  c_ql32 vql_0c;
  c_ql32 vql_10;
  c_o32 vo_14; // poke 16 peek 16 32 add16
  i16 vw_18;
  i32 vl_1c; // inc32
  i32 vl_20;
  i16 vw_24;
  i32 vl_28;
  i16 parw00;
  i16 parw01;

  vql_0c.poke32(RG1L);
  xp_08 = UPCAST(unk, RG4P);
  xp_04 = UPCAST(unk, RG2P); // HOT BUG fixed?
  vol_00.poke32(RG3L);
  RG61L = argl1;
  RG1W = s350.v1e0578;
  RG1L &= lcon(0xff8);
  RG1L = unsignedlong(RG1W);
  if (RG1L == 0)
    //m_1C18C:
    RG4L = 1;
  else
  {
    RG1P = s350.v1e0552;
    RG1W = word_at(RG1P, lcon(0x14));
    RG1L = unsignedlong(RG1W) >> bcon(0xc);
    RG4L = unsignedlong(RG1W);
  }
  //m_1C191:
  vw_24 = RG4W; // truncated
  RG4L = 0;
  vql_10.poke32(RG4L);
  if (RG61W == lcon(0xffffffff))
  {
    RG61L = 0;
    vql_10.poke32(lcon(0x3));
  }
  //m_1C1A8:
  for (;;)
  {
    RG1L = vol_00.peek32();
    vw_18 = RG1W;
    RG1W = argw0;
    vo_14.poke16(RG1W);
    RG3L = 0;
    vl_1c = RG3L;
    //m_1C365:
    for (;;)
    {
      bool skip00409 = false;
      RG1L = vl_1c;
      if (RG1W >= vw_24)
        break;
      //m_1C1C0:
      RG1L = signedlong(RG61W);
      RG4W = table1d27fc[RG1L];
      RG2W = vw_18 + RG4W;
      vw_18 = RG2W;
      RG1W = table1d2804[RG1L];
      vo_14.add16(RG1W);
      if (RG2W >= 0 && RG2W < mapdat.map_width)
        //m_1C1F5:
        RG1L = 0;
      else
        RG1L = 1;
      //m_1C1F7:
      if (RG1L == 0)
      {
        RG2W = vo_14.peek16();
        if (RG2W < 0 || RG2W >= mapdat.map_height)
          RG1L = 1;
        if (RG1L == 0)
        {
          RG2L = signedlong(vo_14.peek16());
          RG5L = signedlong(vw_18);
          RG4L = RG2L;
          RG1L = dm2_GET_TILE_VALUE(RG5L, RG4L);
          if ((RG1Blo & bcon(0x10)) != 0)
          {
            RG1L = unsignedlong(RG1Blo) >> 5;
            if (RG1Blo != 0)
            {
              //m_1C2D9:
              if (argl2 == 0)
              {
                bool skip00408 = false;
                RG1L = unsignedlong(s350.v1e0562.getxA());
                RG5W = vw_18;
                if (RG1W == RG5W)
                {
                  RG1L ^= RG5L;
                  RG1Blo = s350.v1e0562.getyA();
                  if (RG1W == vo_14.peek16())
                  {
                    //m_1C302:
                    RG1L = 0;
                    skip00408 = true;
                  }
                }

                if (!skip00408)
                  //m_1C2FB:
                  RG1L = 1;

                //m_1C304:
                if (RG1L != 0)
                {
                  RG1L = DM2_GET_CREATURE_AT(unsignedlong(vw_18), unsignedlong(vo_14.peek16()));
                  RG5L = RG1L;
                  RG1Blo = RG1W != wcon(0xffff) ? 1 : 0;
                  RG1L = unsignedlong(RG1Blo);
                  if (RG1L != 0)
                  {
                    RG1L = DM2_19f0_0547(unsignedlong(RG5W), signedlong(vql_0c.peek16()));
                    if (RG1L != 0)
                      jump L_fin;
                  }
                }
              }
            }
            else
            {
              if (argl2 == 0)
                break;
              RG4L = RG2L;
              RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(RG5W, RG4W));
              //m_1C25D:
              for (;;)
              {
                RG5L = RG1L;
                if (RG5W == lcon(0xfffffffe))
                  break;
                RG1L = RG5L;
                RG1W &= lcon(0x3c00);
                RG1L = unsignedlong(RG1W) >> bcon(0xa);
                if (RG1L == lcon(0x3) || RG1L == 2)
                {
                  RG2L = signedlong(RG61W) + 2 & lcon(0x3);
                  RG1L = unsignedlong(RG5W);
                  RG4L = RG1L >> bcon(0xe);
                  if (RG4L == RG2L)
                  {
                    RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG1L);
                    if (RG1Blo != lcon(0xffffffff))
                    {
                      RG4L ^= RG2L;
                      RG4Blo = RG1Blo;
                      RG3L = lcon(0xf0);
                      RG2L = lcon(0xb);
                      RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0x9), RG4Blo, RG2Blo, RG3Blo) & lcon(0xffff);
                      RG4W = vql_0c.peek16();
                      if (RG1W == RG4W)
                        jump L_fin;
                    }
                  }
                }
                //m_1C2CD:
                RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG5UW)));
              }
            }
          }
          skip00409 = true;
        }
      }

      if (skip00409)
      {
        //m_1C33D:
        RG1L = signedlong(vw_18);
        RG2P = DOWNCAST(unk, xp_08) + (RG1L << 7);
        RG4L = signedlong(vo_14.peek16());
        RG5L = RG4L;
        if (byte_at(RG2P + 4 * RG5L) != 0)
          break;
        RG1L = DM2_1BC29(RG1L, RG4L);
        if (RG1L != 0)
          break;
      }

      //m_1C362:
      vl_1c++;
    }
    //m_1C372:
    RG61L++;
    RG5L = vql_10.peek32() - 1;
    vql_10.poke32(RG5L);
    if (RG5W < 0)
      return 0;
  }
  FOREND

L_fin:
  RG1L = signedlong(vol_00.peek16()) << 7;
  RG4P = DOWNCAST(unk, xp_08) + RG1L;
  RG1L = signedlong(argw0);
  RG1Blo = byte_at(RG4P, 4 * RG1L);
  RG1W = unsignedword(RG1Blo);
  vql_10.poke32(RG1L);
  RG61L += 2;
  RG61L &= 0x3;
  //m_1C3AC:
  for (;;)
  {
    RG2L = vl_1c + 1;
    vl_1c = RG2L;
    if (RG2W < vw_24)
    {
      vl_28 = vol_00.peek32();
      vl_20 = mkl(argw0);
      RG1L = signedlong(RG61W);
      RG4W = table1d27fc[RG1L];
      vol_00.add16(RG4W);
      RG1W = table1d2804[RG1L];
      argw0 += RG1W;
      RG1L = signedlong(vol_00.peek16());
      RG4L = RG1L << 7;
      RG52p = DOWNCAST(unk, xp_08) + RG4L;
      RG4L = signedlong(argw0);
      RG2L = RG4L;
      RG2Blo = byte_at(RG52p, 4 * RG2L);
      if (RG2Blo != 0)
      {
        RG2W = unsignedword(RG2Blo);
        if (RG2W <= vql_10.peek16())
        {
          RG1L = DM2_1BC29(RG1L, RG4L);
          if (RG1L == 0)
            continue;
        }
      }
      //m_1C410:
      RG1L = vl_28;
      vol_00.poke16(RG1W);
      RG1L = vl_20;
      argw0 = RG1W;
    }
    break;
  }
  //m_1C41E:
  RG61L += 2;
  RG61L &= 0x3;
  RG1L = signedlong(RG61W);
  put16(parw01, RG1W);
  RG1L = signedlong(vo_14.peek16());
  put16(parw00, RG1W);
  RG3L = signedlong(vw_18);
  RG2L = signedlong(argw0);
  RG4L = signedlong(vol_00.peek16());
  RG1L = DM2_19f0_0891((argl2 != 0 ? 1 : 0) + 2, RG4L, RG2L, RG3L, parw00, parw01);
  if (RG1L == 0)
    return RG1L;
  DM2_19f0_045a(signedlong(vol_00.peek16()), signedlong(argw0));
  RG1W = vw_18;
  RG1W &= lcon(0x1f);
  RG4P = DOWNCAST(unk, xp_04);
  and8(location(RG4P), lcon(0xffffffe0));
  RG5W = word_at(RG4P);
  RG5L |= RG1L;
  RG1W = vo_14.peek16();
  RG1W &= lcon(0x1f);
  RG62l = RG5L & lcon(0xfffffc1f);
  RG1L <<= bcon(0x5);
  RG2L = RG62l | RG1L;
  mov16(location(RG4P), RG2W);
  RG1W = ddat.v1d3248 & lcon(0x3f);
  and8(location(RG4P + lcon(0x1)), lcon(0x3));
  RG1L <<= bcon(0xa);
  or16(location(RG4P), RG1W);
  return lcon(0x1);
}

static i32 DM2_1c9a_0598(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG4L = 0;
  RG2L = 0;
  //m_1FE62:
  for (;;)
  {
    if (RG1L == 0)
      return RG2L;
    RG3L = signedlong(RG4W);
    if (RG3L >= lcon(0x20))
      return RG2L;
    //m_1FE5A:
    if ((RG1Blo & bcon(0x1)) != 0)
      RG2L++;
    RG1UL >>= bcon(0x1);
    RG4L++;
  }
  FOREND
}

// stacksize was 0x2c
i32 DM2_19f0_0891(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG51w;
  i16 RG52w;
  i16 RG53w;
  i16 RG54w;
  c_wlreg _RG55;
  i32 RG56l;
  i16 RG57w;
  c_wlreg _RG6;
  i32 RG7l;
  i16 vw_00;
  i16 vw_04;
  i16 vw_08;
  i16 vw_0c;
  c_o32 vo_10; // poke 16 peek 16 32 and16 and8
  c_ql32 vql_14;
  i8 vb_18; // and8(0x18, lcon(0x7f));
  i16 parw00;
  i16 parw01;
  FPTR_L_LL pf_02;
  bool flag;
  bool skip00411 = false;

  bool skip00426 = false;
  bool skip00425 = false;
  bool skip00429 = false;
  bool skip00428 = false;


  vb_18 = RG1Blo;
  RG6L = RG4L;
  vw_00 = RG2W;
  vw_08 = RG3W;
  RG1Blo &= lcon(0xffffff80);
  RG1W = unsignedword(RG1Blo);
  vw_04 = RG1W; // truncated
  if (RG1W != 0)
    vb_18 &= lcon(0x7f);
  RG1W = s350.v1e0578;
  vo_10.poke16(RG1W);
  if (RG1W == 0)
    jump L_fin;
  if (vw_08 == lcon(0xffffffff))
  {
    RG1L = signedlong(argw1);
    RG4W = table1d27fc[RG1L];
    RG4L += RG6L;
    vw_08 = RG4W;
    RG4L = unsignedlong(vw_00);
    RG4W += table1d2804[RG1L];
    argw0 = RG4W;
  }
  RG1W = vw_08;
  if (RG1W >= 0 && RG1W < mapdat.map_width)
    //m_1C539:
    RG1L = 0;
  else
    RG1L = 1;
  //m_1C53B:
  if (RG1L != 0)
    jump L_fin;
  RG2W = argw0;
  if (RG2W < 0 || RG2W >= mapdat.map_height)
    RG1L = 1;
  if (RG1L != 0)
    jump L_fin;
  RG1L = signedlong(RG6W);
  RG4L = signedlong(vw_08);
  if (RG1L == RG4L)
    skip00411 = true;
  else
  {
    RG4L = signedlong(vw_00);
    RG1L = signedlong(argw0);
    if (RG4L == RG1L)
      skip00411 = true;
    else
      RG1L = 1;
  }

  if (skip00411)
    //m_1C589:
    RG1L = 0;
  //m_1C58B:
  if (RG1L != 0)
    jump L_fin;
  RG3L = signedlong(argw0);
  RG2L = signedlong(vw_08);
  RG4L = signedlong(vw_00);
  RG1W = DM2_CALC_SQUARE_DISTANCE(RG6W, RG4W, RG2W, RG3W);
  vw_0c = RG1W;
  RG1L = signedlong(RG1W);
  if (RG1L > lcon(0x1))
  {
    RG51w = vo_10.peek16() & wcon(0xff8);
    flag = RG51w == 0;
    vo_10.poke16(RG51w);
    if (flag)
      jump L_fin;
  }
  //m_1C5CF:
  if (vw_0c == 0)
  {
    RG2W = vo_10.peek16();
    RG2W &= lcon(0x7);
    vo_10.poke16(RG2W);
    if (RG2W == 0)
      jump L_fin;
  }
  //m_1C5EE:
  RG1P = s350.v1e0552;
  RG1W = word_at(RG1P, lcon(0x14));
  RG1L = unsignedlong(RG1W) >> bcon(0xc);
  RG4L = unsignedlong(RG1W);
  RG1L = signedlong(vw_0c);
  if (RG1L > RG4L)
    jump L_fin;
  RG52w = vw_0c;
  if (RG52w == 0)
  {
    //m_1C651:
    for (;;)
    {
      RG4L = signedlong(RG52w);
      if (RG4L < lcon(0x4))
      {
        //m_1C61E:
        RG2W = table1d27fc[RG4L];
        RG2L += RG6L;
        RG3L = unsignedlong(vw_00);
        RG3W += table1d2804[RG4L];
        put16(parw01, RG4W);
        RG1L = signedlong(RG3W);
        put16(parw00, RG1W);
        RG3L = signedlong(RG2W);
        RG2L = signedlong(vw_00);
        RG4L = signedlong(RG6W);
        RG1L = DM2_CREATURE_GO_THERE(0, RG4L, RG2L, RG3L, parw00, parw01);
        if (RG1L == 0)
        {
          RG52w++;
          continue;
        }
      }
      break;
    }
    //m_1C659:
    RG1L = signedlong(RG52w);
    if (RG1L < lcon(0x4))
      jump L_fin;
  }
  //m_1C665:
  RG4Blo = vb_18;
  if (mkub(RG4Blo) <= mkub(lcon(0x1)))
  {
    //m_1C697:
    RG1W = ddat.v1d3248;
    if (RG1W != ddat.v1e08d6)
      jump L_fin;
    RG1W = vw_08;
    if (RG1W != ddat.v1e08d8)
      jump L_fin;
    RG1W = argw0;
    if (RG1W != ddat.v1e08d4)
      jump L_fin;
  }
  else
  {
    if (RG4Blo == 2)
    {
      DM2_19f0_045a(signedlong(vw_08), signedlong(argw0));
      RG1L = DM2_19f0_050f();
      if (RG1W == lcon(0xfffffffe))
        jump L_fin;
    }
  }
  //m_1C6CC:
  RG1L = signedlong(vw_0c);
  if (RG1L > lcon(0x1))
  {
    pf_02 = DM2_1BAAD;
    RG3L = signedlong(argw0);
    RG2L = signedlong(vw_08);
    RG4L = signedlong(vw_00);
    RG1L = DM2_19f0_0207(signedlong(RG6W), RG4L, RG2L, RG3L, pf_02);
    if (RG1W == 0)
      jump L_fin;
  }
  //m_1C703:
  RG1W = vo_10.peek16();
  RG1W &= 0x4;
  RG1L = unsignedlong(RG1W);
  if (RG1L != 0)
  {
    vql_14.poke32(lcon(0xffffffff));
    RG53w = 0;
    //m_1C7E0:
    for (;;)
    {
      RG1L = signedlong(RG53w);
      if (RG1L >= lcon(0x4))
      {
        if (vql_14.peek16() < wcon(0x0))
        {
          vo_10.and16(lcon(0xfffffffb));
          if (vo_10.peek16() == 0)
            jump L_fin;
        }
        break;
      }
      //m_1C725:
      RG2L = unsignedlong(RG53w);
      RG1L = DM2_move_2c1d_028c(signedlong(RG6W), signedlong(vw_00), RG2L);
      RG2L = RG1L;
      RG4L = signedlong(RG1W);
      if (RG4L != lcon(0xffffffff))
      {
        bool skip00413 = false;
        bool skip00414 = false;
        RG4W = party.hero[RG4L].item[1];
        RG1Blo = RG4W != wcon(0xffff) ? 1 : 0;
        RG1L = unsignedlong(RG1Blo);
        if (RG1L == 0)
          skip00413 = true;
        else
        {
          RG1L = DM2_CREATURE_CAN_HANDLE_IT(unsignedlong(RG4W), lcon(0xb));
          if (RG1L == 0)
            skip00413 = true;
        }

        if (skip00413)
        {
          //m_1C77B:
          RG4L = signedlong(RG2W);
          RG4W = party.hero[RG4L].item[0];
          RG1Blo = RG4W != wcon(0xffff) ? 1 : 0;
          RG1L = unsignedlong(RG1Blo);
          if (RG1L != 0)
          {
            RG1L = DM2_CREATURE_CAN_HANDLE_IT(unsignedlong(RG4W), lcon(0xb));
            if (RG1L != 0)
              skip00414 = true;
          }
        }
        else
          skip00414 = true;

        if (skip00414)
        {
          bool skip00415 = false;
          //m_1C7B2:
          if (vql_14.peek16() < wcon(0x0))
            skip00415 = true;
          else
          {
            RG1L = DM2_RANDBIT() ? 1 : 0;
            if (RG1W != 0)
              skip00415 = true;
          }

          if (skip00415)
          {
            //m_1C7C4:
            RG4L = signedlong(RG2W);
            RG1Blo = party.hero[RG4L].partypos;
            RG1W = unsignedword(RG1Blo);
            vql_14.poke32(RG1L);
          }
        }
      }

      //m_1C7DF:
      RG53w++;
    }
  }
  //m_1C805:
  RG1L = signedlong(s350.v1e0584);
  if ((table1d607e[RG1L].uc[1] & bcon(0x2)) != 0)
  {
    DM2_19f0_045a(signedlong(vw_08), signedlong(argw0));
    RG1L = DM2_19f0_04bf();
    //m_1C833:
    for (;;)
    {
      RG4L = RG1L;
      if (RG4W == lcon(0xfffffffe))
        break;
      RG1L = RG4L;
      RG1Blo ^= RG4Blo;
      RG1Bhi &= lcon(0x3c);
      RG1L = unsignedlong(RG1W) >> bcon(0xa);
      if (RG1L == lcon(0xf))
      {
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
        RG1W = word_at(RG1P, 2) & lcon(0x7f);
        if (RG1W == lcon(0xe))
        {
          RG54w = vo_10.peek16();
          RG54w &= lcon(0x7);
          flag = RG54w == 0;
          vo_10.poke16(RG54w);
          if (flag)
            jump L_fin;
          break;
        }
      }
      //m_1C87C:
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
    }
  }
  //m_1C888:
  RG1W = ddat.v1e08d6;
  RG4W = ddat.v1d3248;
  if (RG1W == RG4W)
  {
    RG1L = signedlong(s350.v1e0584);
    if ((table1d607e[RG1L].uc[1] & bcon(0x40)) != 0)
    {
      RG1P = s350.v1e0552;
      if (jz_test8(location(RG1P), lcon(0x20)))
      {
        bool skip00418 = false;
        RG1L = unsignedlong(s350.v1e0562.getxA());
        if (RG1W != RG6W)
          skip00418 = true;
        else
        {
          RG1L ^= RG6L;
          RG1Blo = s350.v1e0562.getyA();
          RG2L = unsignedlong(vw_00);
          if (RG1W != RG2W)
            skip00418 = true;
          else
          {
            RG1L ^= RG2L;
            RG1Blo = s350.v1e0571;
            if (RG1W != RG4W)
              skip00418 = true;
          }
        }

        if (skip00418)
        {
          //m_1C8E9:
          RG7l = signedlong(vw_00);
          RG55L = signedlong(RG6W);
          RG1L = (dm2_GET_TILE_VALUE(RG55L, RG7l) & lcon(0xff)) >> 5 & lcon(0xff);
          if (RG1L == lcon(0x4))
          {
            RG3L = signedlong(ddat.v1e08d4);
            RG2L = signedlong(ddat.v1e08d8);
            RG4L = RG7l;
            RG1L = signedlong(DM2_CALC_SQUARE_DISTANCE(RG55W, RG4W, RG2W, RG3W));
            if (RG1L < 2)
            {
              RG1L = signedlong(DM2_RANDDIR());
              if (RG1W != 0)
                jump L_fin;
            }
          }
        }
      }
    }
  }
  //m_1C93D:
  if (vw_04 == 0)
    return lcon(0x1);
  if (argw1 == lcon(0xffffffff))
  {
    bool skip00421 = false;
    if (vw_0c == 0)
    {
      RG1W = ddat.v1d3248;
      if (RG1W == ddat.v1e08d6)
      {
        RG1W = vw_08;
        if (RG1W == ddat.v1e08d8)
        {
          RG1W = argw0;
          if (RG1W == ddat.v1e08d4)
          {
            RG1W = ddat.v1e0258;
            RG1L += 2;
            RG1W &= 0x3;
            skip00421 = true;
          }
        }
      }
    }

    if (!skip00421)
    {
      //m_1C993:
      RG3L = signedlong(argw0);
      RG2L = signedlong(vw_08);
      RG4L = signedlong(vw_00);
      RG1L = signedlong(DM2_CALC_VECTOR_DIR(RG6W, RG4W, RG2W, RG3W));
    }

    //m_1C9B0:
    argw1 = RG1W;
  }
  skip00426 = false;
  skip00425 = false;
  //m_1C9B5:
  RG1L = DM2_19f0_0559(argw1);
  if (RG1L != 0)
    return lcon(0x1);
  RG1L = DM2_RANDBIT() ? 1 : 0;
  RG3L = unsignedlong(RG1W);
  RG1L = signedlong(vw_0c);
  if (RG1L > lcon(0x1))
    skip00425 = true;
  else
  {
    RG1W = vo_10.peek16();
    RG1W &= lcon(0x7);
    RG1L = unsignedlong(RG1W);
    if (RG1L == 0)
      skip00425 = true;
    else
    {
      bool skip00423 = false;
      RG1W = vo_10.peek16() & lcon(0xff8);
      if (RG1W == 0)
        skip00423 = true;
      else
      {
        RG1L = DM2_RANDBIT() ? 1 : 0;
        if (RG1W != 0)
          skip00423 = true;
      }

      if (skip00423)
      {
        bool skip00424 = false;
        //m_1CA10:
        RG1W = vo_10.peek16();
        RG1W &= lcon(0x7);
        vo_10.poke16(RG1W);
        if (mkub(vb_18) > mkub(lcon(0x1)))
          skip00424 = true;
        else
        {
          RG4L = signedlong(vw_00);
          RG1L = DM2_move_2c1d_028c(signedlong(RG6W), RG4L, lcon(0xff));
          RG4L = signedlong(RG1W);
          if (RG4L == lcon(0xffffffff))
            skip00424 = true;
          else
          {
            RG1L = 263 * RG4L;
            RG2L = unsignedlong(party.hero[RG1L / 263].partypos);
          }
        }

        if (skip00424)
        {
          //m_1CA59:
          RG2W = RG3W + wcon(0x2) + argw1;
          skip00426 = true;
        }
      }
      else
        skip00425 = true;
    }
  }

  if (skip00425)
  {
    //m_1CA67:
    vo_10.and16(lcon(0xff8));
    if (mkub(vb_18) <= mkub(lcon(0x1)))
    {
      RG1L = signedlong(DM2_RANDDIR());
      if (RG1W != 0)
      {
        RG1L = DM2_RANDBIT() ? 1 : 0;
        RG3L = unsignedlong(RG1W);
        if (RG3L == 0)
          //m_1CA9B:
          RG2W = argw1;
        else
        {
          RG2W = argw1 + wcon(0x2);
          RG2W &= 0x3;
        }
        //m_1CA9F:
        RG1W = DM2_GET_PLAYER_AT_POSITION(RG2W);
        if (RG1W == -1)
        {
          RG2L += 3;
          RG2W &= 0x3;
          RG1W = DM2_GET_PLAYER_AT_POSITION(RG2W);
          if (RG1W == -1)
          {
            RG1L = lcon(0x1) - RG3L;
            RG3L = RG1L;
          }
        }
      }
    }
    //m_1CAD0:
    RG2W = argw1 + RG3W;
    skip00426 = true;
  }

  if (skip00426)
    //m_1CAD6:
    RG2W &= 0x3;

  skip00429 = false;
  skip00428 = false;
  //m_1CADB:
  RG56l = unsignedlong(vo_10.peek16());
  RG1W = DM2_RAND16(CUTX16(DM2_1c9a_0598(RG56l))) + 1;
  RG4L = signedlong(RG1W);
  RG1L = DM2_COMPUTE_POWER_4_WITHIN(CUTX16(RG56l), RG4W);
  if (RG1UW < mkuw(lcon(0x20)))
  {
    //m_1CB5F:
    if (RG1UW < mkuw(lcon(0x4)))
    {
      //m_1CB82:
      if (RG1UW >= mkuw(lcon(0x1)))
      {
        if (RG1UW <= mkuw(lcon(0x1)))
          //m_1CBA6:
          mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1a)), lcon(0x8));
        else
        {
          if (RG1W == 2)
            mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1a)), lcon(0x26));
        }
      }
    }
    else
    {
      if (RG1UW <= mkuw(lcon(0x4)))
      {
        //m_1CBB4:
        mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1a)), lcon(0xa));
        mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1e)), lcon(0xb));
      }
      else
      {
        if (RG1UW >= mkuw(lcon(0x8)))
        {
          if (RG1UW <= mkuw(lcon(0x8)))
          {
            //m_1CC2E:
            RG1Blo = RG3L != 0 ? 1 : 0;
            RG4L = unsignedlong(RG1Blo) + lcon(0xe);
            skip00429 = true;
          }
          else
          {
            if (RG1W == lcon(0x10))
            {
              //m_1CBCB:
              vw_0c = wcon(0xff80);
              skip00428 = true;
            }
          }
        }
      }
    }
  }
  else
  {
    if (RG1UW <= mkuw(lcon(0x20)))
    {
      //m_1CBD4:
      vw_0c = wcon(0xff83);
      skip00428 = true;
    }
    else
    {
      if (RG1UW < mkuw(lcon(0x100)))
      {
        //m_1CB40:
        if (RG1UW >= mkuw(lcon(0x40)))
        {
          if (RG1UW <= mkuw(lcon(0x40)))
          {
            //m_1CBDD:
            vw_0c = wcon(0xff82);
            skip00428 = true;
          }
          else
          {
            if (RG1W == lcon(0x80))
            {
              //m_1CBE6:
              vw_0c = wcon(0xff87);
              skip00428 = true;
            }
          }
        }
      }
      else
      {
        if (RG1UW <= mkuw(lcon(0x100)))
        {
          //m_1CBEF:
          vw_0c = wcon(0xff86);
          skip00428 = true;
        }
        else
        {
          if (RG1UW < mkuw(lcon(0x400)))
          {
            //m_1CB31:
            if (RG1W == lcon(0x200))
            {
              //m_1CC0A:
              vw_0c = wcon(0xff81);
              skip00428 = true;
            }
          }
          else
          {
            if (RG1UW <= mkuw(lcon(0x400)))
            {
              //m_1CBF8:
              vw_0c = wcon(0xff89);
              skip00428 = true;
            }
            else
            {
              if (RG1W == lcon(0x800))
              {
                //m_1CC01:
                vw_0c = wcon(0xff8a);
                skip00428 = true;
              }
            }
          }
        }
      }
    }
  }

  if (skip00428)
  {
    //m_1CC11:
    RG1P = DOWNCAST(c_creature, s350.creatures);
    RG4Blo = CUTX8(vw_0c);
    mov8(location(RG1P + lcon(0x1e)), RG4Blo);
    RG1Blo = RG3L != 0 ? 1 : 0;
    RG1L = unsignedlong(RG1Blo) + lcon(0x27);
    RG4Blo = RG1Blo;
    skip00429 = true;
  }

  if (skip00429)
    //m_1CC3A:
    mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1a)), RG4Blo);

  //m_1CC42:
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4W = vw_08;
  RG4W &= lcon(0x1f);
  and8(location(RG1P + lcon(0x18)), lcon(0xffffffe0));
  or16(location(RG1P + lcon(0x18)), RG4W);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4W = argw0;
  RG4W &= lcon(0x1f);
  RG57w = word_at(RG1P, lcon(0x18));
  RG57w &= wcon(0xfc1f);
  RG4L <<= bcon(0x5);
  mov16(location(RG1P + lcon(0x18)), RG57w | RG4W);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4W = ddat.v1d3248 & lcon(0x3f);
  and8(location(RG1P + lcon(0x19)), lcon(0x3));
  RG4L <<= bcon(0xa);
  or16(location(RG1P + lcon(0x18)), RG4W);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4Blo = mkb(argw1);
  mov8(location(RG1P + lcon(0x1b)), RG4Blo);
  RG1L = signedlong(byte_at(DOWNCAST(c_creature, s350.creatures), lcon(0x1a)));
  if (RG1L == lcon(0xa))
    RG2L = vql_14.peek32();
  mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1c)), RG2Blo);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4Blo = vb_18;
  mov8(location(RG1P + lcon(0x20)), RG4Blo);
  s350.v1e056f = lcon(0xfffffffc);
  return lcon(0x1);

L_fin:
  if (vw_04 != 0)
    s350.v1e056f = lcon(0xfffffffd);
  return 0;
}

// stacksize was 0x30
i32 DM2_19f0_0d10(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG51w;
  i16 RG52w;
  i16 RG53w;
  i16 RG54w;
  c_wlreg _RG61;
  c_wlreg _RG62;
  i8* RG7p;
  i16 vw_00;
  i16 vw_04;
  i16 vw_08;
  i16 vw_0c;
  i16 vw_10;
  c_o32 vo_14; // poke 16 peek 16 32 and8
  i16 vw_18;
  i8 vb_1c; // and8(0x1c, lcon(0x7f));
  FPTR_L_LL pf_00;
  i16 parw01;
  i16 parw02;
  bool flag;
  bool skip00431 = false;

  vb_1c = RG1Blo;
  vw_08 = RG4W;
  vw_0c = RG2W;
  vw_10 = RG3W;
  RG4L = 0;
  vw_04 = RG4W;
  RG1Blo &= lcon(0xffffff80);
  RG1W = unsignedword(RG1Blo);
  vw_18 = RG1W;
  if (RG1W != 0)
    vb_1c &= lcon(0x7f);
  if (vb_1c != 0)
    //m_1CD32:
    RG1L = lcon(0x73);
  else
    RG1L = lcon(0x6f);
  //m_1CD37:
  RG1L &= unsignedlong(s350.v1e057a);
  vo_14.poke16(RG1W);
  if (RG1W == 0)
    jump L_fin;
  RG51w = vw_10;
  if (RG51w != wcon(0xffff))
  {
    //m_1CD89:
    if (argw1 != lcon(0xffffffff))
    {
      //m_1CDB9:
      if (RG51w == vw_08)
      {
        RG1L = unsignedlong(vw_0c);
        if (RG1W == argw0)
          jump L_fin;
      }
    }
    else
    {
      RG3L = signedlong(argw0);
      RG2L = signedlong(vw_10);
      RG4L = signedlong(vw_0c);
      RG1L = signedlong(DM2_CALC_VECTOR_DIR(vw_08, RG4W, RG2W, RG3W));
      argw1 = RG1W;
    }
  }
  else
  {
    RG1L = signedlong(argw1);
    RG4W = vw_08;
    RG3W = table1d27fc[RG1L];
    RG4L += RG3L;
    vw_10 = RG4W;
    RG4L = unsignedlong(vw_0c);
    RG4W += table1d2804[RG1L];
    argw0 = RG4W;
  }
  //m_1CDCF:
  RG61L = unsignedlong(vw_10);
  if (RG61W >= 0 && RG61W < mapdat.map_width)
    //m_1CDE8:
    RG1L = 0;
  else
    RG1L = 1;
  //m_1CDEA:
  if (RG1L != 0)
    jump L_fin;
  RG2W = argw0;
  if (RG2W < 0 || RG2W >= mapdat.map_height)
    RG1L = 1;
  if (RG1L != 0)
    jump L_fin;
  RG4L = signedlong(vw_08);
  RG1L = signedlong(vw_10);
  if (RG4L == RG1L)
    skip00431 = true;
  else
  {
    RG4L = signedlong(vw_0c);
    RG1L = signedlong(argw0);
    if (RG4L == RG1L)
      skip00431 = true;
    else
      RG1L = 1;
  }

  if (skip00431)
    //m_1CE3C:
    RG1L = 0;
  //m_1CE3E:
  if (RG1L != 0)
    jump L_fin;
  DM2_19f0_045a(signedlong(vw_10), signedlong(argw0));
  RG1L = unsignedlong(ddat.v1e08ae) >> 5 & lcon(0xff);
  if (RG1L != lcon(0x4))
    jump L_fin;
  RG1Blo = CUTX8(ddat.v1e08ae) & lcon(0x7);
  RG1W = unsignedword(RG1Blo);
  if (vb_1c != 0)
  {
    //m_1CE94:
    RG1L = unsignedlong(RG1W);
    if (RG1L == lcon(0x5))
      jump L_fin;
    flag = RG1L != 4;
  }
  else
    flag = RG1W != wcon(0x0);
  //m_1CE85:
  if (flag)
  {
    bool skip00433 = false;
    //m_1CEA7:
    if (ddat.v1e08b0 == lcon(0xffffffff))
    {
      RG4L = signedlong(argw0);
      RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(vw_10, RG4W));
      ddat.v1e08b0 = RG1W;
    }
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(ddat.v1e08b0)));
    RG7p = RG1P;
    if (vb_1c == 2 && jz_test8(location(RG1P + lcon(0x3)), lcon(0x10)))
      jump L_fin;
    RG3Blo = byte_at(RG7p, lcon(0x3));
    if ((RG3Blo & bcon(0x4)) == 0)
      skip00433 = true;
    else
    {
      if ((RG3Blo & bcon(0x2)) == 0)
      {
        //m_1CF03:
        if (vb_1c != 0)
          vw_04 = 2;
        else
        {
          vo_14.and16(lcon(0xfffffff3));
          if (vo_14.peek16() == 0)
            jump L_fin;
          skip00433 = true;
        }
      }
      else
      {
        if (vb_1c != 0)
          skip00433 = true;
        else
          vw_04 = 2;
      }
    }

    if (skip00433)
    {
      //m_1CF1F:
      RG3L = signedlong(argw0);
      RG2L = signedlong(vw_10);
      RG4L = signedlong(vw_0c);
      RG1W = DM2_CALC_SQUARE_DISTANCE(vw_08, RG4W, RG2W, RG3W);
      vw_00 = RG1W;
      RG4L = signedlong(RG1W);
      if (RG4L != lcon(0x1))
      {
        bool skip00436 = false;
        //m_1CFD8:
        RG1P = s350.v1e0552;
        RG1W = word_at(RG1P, lcon(0x14));
        RG1L = unsignedlong(RG1W) >> bcon(0xc);
        RG1L = unsignedlong(RG1W);
        if (RG4L > RG1L)
          jump L_fin;
        RG1W = vo_14.peek16();
        RG1W &= 0x1;
        RG1L = unsignedlong(RG1W);
        if ((RG1L != 0 && jnz_test8(location(RG7p + 2), lcon(0x40))) || jnz_test8(location(RG7p + lcon(0x3)), lcon(0x20)))
          skip00436 = true;
        else
        {
          bool skip00435 = false;
          if (vb_1c != 0)
            skip00435 = true;
          else
          {
            RG1W = vo_14.peek16();
            RG1W &= 0x4;
            RG1L = unsignedlong(RG1W);
            if (RG1L == 0 || jz_test8(location(RG7p + 2), lcon(0xffffff80)))
              skip00435 = true;
          }

          if (skip00435)
            //m_1D029:
            RG1L = 1;
          else
            skip00436 = true;
        }

        if (skip00436)
          //m_1D030:
          RG1L = 0;

        //m_1D032:
        if (RG1L != 0)
          jump L_fin;
        pf_00 = DM2_1BAAD;
        RG3L = signedlong(argw0);
        RG2L = signedlong(vw_10);
        RG4L = signedlong(vw_0c);
        RG1L = DM2_19f0_0207(signedlong(vw_08), RG4L, RG2L, RG3L, pf_00);
        if (RG1W == 0)
          jump L_fin;
      }
      else
      {
        RG1W = vo_14.peek16();
        RG1W &= 0x3;
        RG1L = unsignedlong(RG1W);
        if ((RG1L == 0 || jz_test8(location(RG7p + 2), lcon(0x40))) && jz_test8(location(RG7p + lcon(0x3)), lcon(0x20)))
        {
          bool skip00434 = false;
          if (vb_1c != 0)
            skip00434 = true;
          else
          {
            RG1W = vo_14.peek16();
            RG1W &= 0x4;
            RG1L = unsignedlong(RG1W);
            if (RG1L == 0 || jz_test8(location(RG7p + 2), lcon(0xffffff80)))
            {
              RG1W = vo_14.peek16();
              RG1W &= lcon(0x8);
              RG1L = unsignedlong(RG1W);
              if (RG1L == 0 || jz_test8(location(RG7p + lcon(0x3)), lcon(0x1)))
                skip00434 = true;
            }
          }

          if (skip00434)
          {
            //m_1CFAC:
            RG1W = vo_14.peek16();
            RG1W &= lcon(0x40);
            RG1L = unsignedlong(RG1W);
            if (RG1L == 0)
              jump L_fin;
          }
        }
      }

      //m_1D065:
      RG1W = vo_14.peek16();
      RG1W &= 0x1;
      RG1L = unsignedlong(RG1W);
      if (RG1L != 0)
      {
        RG62W = vw_08;
        RG3L = unsignedlong(vw_0c);
        RG52w = vw_00;
        //m_1D155:
        for (;;)
        {
          if (RG52w < 0)
            break;
          //m_1D088:
          RG1L = DM2_GET_WALL_TILE_ANYITEM_RECORD(signedlong(RG62W), RG3W);
          //m_1D093:
          bool skipbreak2 = false;
          for (;;)
          {
            bool skip00439 = false;
            bool skip00440 = false;
            RG4L = RG1L;
            if (RG4W == lcon(0xfffffffe))
            {
              //m_1D13D:
              RG1L = signedlong(argw1);
              RG62W += table1d27fc[RG1L];
              RG3W += table1d2804[RG1L];
              RG52w--;
              break;
            }
            RG1L = RG4L;
            RG1Blo ^= RG4Blo;
            RG1Bhi &= lcon(0x3c);
            RG1L = unsignedlong(RG1W) >> bcon(0xa);
            RG1L = signedlong(RG1W);
            if (RG1L != lcon(0xe))
            {
              //m_1D10B:
              if (RG1L != lcon(0xf))
                skip00440 = true;
              else
              {
                RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
                RG1W = word_at(RG1P, 2) & lcon(0x7f);
                if (RG1W == lcon(0xd))
                {
                  vw_04 = 2;
                  skipbreak2 = true;
                  break;
                }
                flag = RG1W != wcon(0x4);
                skip00439 = true;
              }
            }
            else
            {
              RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
              RG2P = RG1P;
              RG1W = word_at(RG1P, 2);
              RG1L = unsignedlong(RG1W);
              if (RG1L != lcon(0xff8d) && RG1L != lcon(0xff84))
                skip00440 = true;
              else
              {
                RG1W = timdat.timerarray[word_at(RG2P, lcon(0x6))].getB();
                RG1L <<= 4;
                RG1UW >>= 14;
                flag = RG1W != argw1;
                skip00439 = true;
              }
            }

            if (skip00439)
            {
              //m_1D0FF:
              if (!flag)
              {
                vw_04 = 2;
                skipbreak2 = true;
                break;
              }
              skip00440 = true;
            }

            if (skip00440)
            {
              //m_1D12E:
              RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
              continue;
            }
          }

          if (skipbreak2)
            break;
        }
      }
    }
  }
  else
    vw_04 = 1;

  //m_1D15E:
  if (vw_04 != 0 && vb_1c == 2)
    jump L_fin;
  if (vw_18 == 0)
    return lcon(0x1);
  if (argw1 == lcon(0xffffffff))
  {
    RG3L = signedlong(argw0);
    RG2L = signedlong(vw_10);
    RG4L = signedlong(vw_0c);
    RG1L = signedlong(DM2_CALC_VECTOR_DIR(vw_08, RG4W, RG2W, RG3W));
    argw1 = RG1W;
  }
  RG1L = unsignedlong(vw_04);
  if (RG1L == lcon(0x1))
  {
    s350.v1e056f = lcon(0xfffffffe);
    return lcon(0x1);
  }
  if (RG1L != 2)
  {
    bool skip00442 = false;
    //m_1D1D6:
    RG1L = DM2_19f0_0559(argw1);
    if (RG1L != 0)
      return lcon(0x1);
    RG1W = vo_14.peek16();
    RG1W &= lcon(0x20);
    RG1L = unsignedlong(RG1W);
    if (RG1L != 0)
    {
      vo_14.and16(lcon(0xffffffdf));
      if (vb_1c == 0)
        or8(location(RG7p + lcon(0x3)), lcon(0x10));
    }
    RG1L = signedlong(vw_00);
    if (RG1L > lcon(0x1))
    {
      RG1W = vo_14.peek16();
      RG1W &= 0x5;
      vo_14.poke16(RG1W);
    }
    RG1W = vo_14.peek16();
    RG1W &= lcon(0x42);
    RG1L = unsignedlong(RG1W);
    if (RG1L == 0)
      skip00442 = true;
    else
    {
      bool skip00441 = false;
      RG1W = vo_14.peek16();
      RG1Blo &= lcon(0xffffffbd);
      RG1L = unsignedlong(RG1W);
      if (RG1L == 0)
        skip00441 = true;
      else
      {
        RG1L = signedlong(DM2_RANDDIR());
        if (RG1W != 0)
          skip00441 = true;
      }

      if (skip00441)
        //m_1D248:
        mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1a)), lcon(0xb));
      else
        skip00442 = true;
    }

    if (skip00442)
    {
      bool skip00444 = false;
      //m_1D256:
      RG1W = vo_14.peek16();
      RG1W &= 0x1;
      RG1L = unsignedlong(RG1W);
      if (RG1L == 0)
        skip00444 = true;
      else
      {
        bool skip00443 = false;
        RG1W = vo_14.peek16();
        RG1Blo &= lcon(0xffffffbc);
        RG1L = unsignedlong(RG1W);
        if (RG1L == 0)
          skip00443 = true;
        else
        {
          RG1L = signedlong(DM2_RANDDIR());
          if (RG1W == 0)
            skip00443 = true;
        }

        if (skip00443)
        {
          //m_1D27C:
          RG1L = DM2_RANDBIT() ? 1 : 0;
          RG1Blo = RG1W != wcon(0x0) ? 1 : 0;
          RG1L = unsignedlong(RG1Blo) + lcon(0x27);
          RG4Blo = RG1Blo;
          mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1a)), RG4Blo);
          if (vb_1c != 0)
            //m_1D2AA:
            RG1L = lcon(0xff84);
          else
            RG1L = lcon(0xff8d);
          //m_1D369:
          RG4Blo = RG1Blo;
          mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1e)), RG4Blo);
        }
        else
          skip00444 = true;
      }

      if (skip00444)
      {
        bool skip00446 = false;
        //m_1D2B4:
        RG53w = s350.v1e0578;
        RG1W = vo_14.peek16();
        RG1W &= lcon(0x8);
        RG1L = unsignedlong(RG1W);
        if (RG1L == 0)
          skip00446 = true;
        else
        {
          RG1W = RG53w;
          RG1W &= 0x1;
          RG1L = unsignedlong(RG1W);
          if (RG1L == 0)
            skip00446 = true;
          else
          {
            bool skip00445 = false;
            RG1W = vo_14.peek16();
            RG1Blo &= lcon(0xffffffb4);
            RG1L = unsignedlong(RG1W);
            if (RG1L == 0)
              skip00445 = true;
            else
            {
              RG1L = DM2_RANDBIT() ? 1 : 0;
              if (RG1W != 0)
                skip00445 = true;
            }

            if (skip00445)
            {
              //m_1D2EE:
              RG2W = s350.v1e0578 & lcon(0x1);
              s350.v1e0578 = RG2W;
            }
            else
              skip00446 = true;
          }
        }

        if (skip00446)
        {
          //m_1D303:
          RG1W = vo_14.peek16();
          RG1W &= 0x4;
          RG1L = unsignedlong(RG1W);
          if (RG1L == 0)
            jump L_fin;
          RG4W = s350.v1e0578 & lcon(0x50);
          s350.v1e0578 = RG4W;
        }

        //m_1D329:
        put16(parw02, argw1);
        put16(parw01, argw0);
        RG1L = DM2_19f0_0891(lcon(0x84), signedlong(vw_08), signedlong(vw_0c), signedlong(vw_10), parw01, parw02);
        s350.v1e0578 = RG53w;
        RG1L = unsignedlong(RG1W);
        return RG1L;
      }
    }

    //m_1D373:
    RG1P = DOWNCAST(c_creature, s350.creatures);
    RG4L = unsignedlong(vw_10);
    RG4W &= lcon(0x1f);
    and8(location(RG1P + lcon(0x18)), lcon(0xffffffe0));
    or16(location(RG1P + lcon(0x18)), RG4W);
    RG1P = DOWNCAST(c_creature, s350.creatures);
    RG4W = argw0;
    RG4W &= lcon(0x1f);
    RG54w = word_at(RG1P, lcon(0x18)) & wcon(0xfc1f);
    RG4L <<= bcon(0x5);
    mov16(location(RG1P + lcon(0x18)), RG54w | RG4W);
    RG1P = DOWNCAST(c_creature, s350.creatures);
    RG4W = ddat.v1d3248 & lcon(0x3f);
    and8(location(RG1P + lcon(0x19)), lcon(0x3));
    RG4L <<= bcon(0xa);
    or16(location(RG1P + lcon(0x18)), RG4W);
    RG1P = DOWNCAST(c_creature, s350.creatures);
    RG4Blo = mkb(argw1);
    mov8(location(RG1P + lcon(0x1d)), RG4Blo);
    mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1b)), RG4Blo);
    RG1L = DM2_RANDBIT() ? 1 : 0;
    RG3Blo = RG4Blo;
    RG4Blo = RG1Blo + RG3Blo & lcon(0x3);
    mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1c)), RG4Blo);
    RG1P = DOWNCAST(c_creature, s350.creatures);
    RG4Blo = vb_1c;
    mov8(location(RG1P + lcon(0x20)), RG4Blo);
  }
  else
    mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1a)), 0);

  //m_1D3FE:
  s350.v1e056f = lcon(0xfffffffc);
  return lcon(0x1);

L_fin:
  if (vw_18 != 0)
    s350.v1e056f = lcon(0xfffffffd);
  return 0;
}

i32 DM2_19f0_13aa(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_wlreg _RG6;
  c_ql32 vql_00;
  i32 vl_04;
  c_o32 vo_08; // poke 16 peek 16 32 add16
  c_o32 vo_0c; // poke 16 peek 32 inc16

  vl_04 = RG1L;
  vql_00.poke32(RG4L);
  RG5L = 0;
  //m_1D5E9:
  for (;;)
  {
    bool skip00456 = false;
    RG1L = signedlong(s350.v1e0584);
    if ((table1d607e[RG1L].uc[1] & bcon(0x4)) != 0)
      skip00456 = true;
    else
    {
      bool skip00455 = false;
      RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
      RG4W = word_at(RG1P, lcon(0xa)) & lcon(0x80);
      RG4L = unsignedlong(RG4W);
      if (RG4L != 0)
        skip00455 = true;
      else
      {
        RG4P = s350.v1e0552;
        if (jz_test8(location(RG4P), lcon(0x4)))
        {
          RG4L = unsignedlong(s350.v1e0562.getxA());
          RG2L = vl_04;
          if (RG4W == RG2W)
          {
            RG4L ^= RG2L;
            RG4Blo = s350.v1e0562.getyA();
            if (RG4W == vql_00.peek16())
            {
              RG1W = word_at(RG1P, lcon(0xe));
              RG1L <<= bcon(0x6);
              RG1UW >>= 14;
              RG1L = unsignedlong(RG1W) + 2;
              RG1L &= 0x3;
              RG4L = signedlong(RG5W);
              if (RG4L == RG1L)
                skip00455 = true;
            }
          }
        }
      }

      if (skip00455)
      {
        //m_1D65C:
        RG1L = DM2_RAND();
        RG1W &= lcon(0x7);
        RG1L = unsignedlong(RG1W);
        if (RG1L == 0)
          skip00456 = true;
      }
      else
        skip00456 = true;
    }

    if (skip00456)
    {
      //m_1D670:
      RG6L = vl_04;
      RG1L = vql_00.peek32();
      vo_08.poke16(RG1W);
      RG1L = 0;
      vo_0c.poke16(RG1W);
      //m_1D683:
      for (;;)
      {
        bool skip00458 = false;
        RG1L = signedlong(vo_0c.peek16());
        vo_0c.inc16();
        if (RG1L >= lcon(0x3))
          break;
        RG1L = signedlong(RG5W);
        RG6W += table1d27fc[RG1L];
        RG1W = table1d2804[RG1L];
        vo_08.add16(RG1W);
        if (RG6W >= 0 && RG6W < mapdat.map_width)
          //m_1D6C5:
          RG1L = 0;
        else
          RG1L = 1;
        //m_1D6C7:
        if (RG1L != 0)
          break;
        RG4W = vo_08.peek16();
        if (RG4W < 0 || RG4W >= mapdat.map_height)
          RG1L = 1;
        if (RG1L != 0)
          break;
        RG4L = signedlong(vo_08.peek16());
        RG1L = DM2_GET_WALL_TILE_ANYITEM_RECORD(signedlong(RG6W), RG4W);
        //m_1D6FD:
        for (;;)
        {
          RG2L = RG1L;
          if (RG2W == lcon(0xfffffffe))
            break;
          RG1L = RG2L;
          RG1Blo ^= RG2Blo;
          RG1Bhi &= lcon(0x3c);
          RG1L = unsignedlong(RG1W) >> bcon(0xa);
          if (RG1L == lcon(0xe))
          {
            RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
            RG3W = timdat.timerarray[word_at(RG1P, lcon(0x6))].getB();
            RG3L <<= 4;
            RG3UW >>= 14;
            RG4L = RG5L + 2;
            RG4W &= 0x3;
            if (RG3W == RG4W)
            {
              RG4L = unsignedlong(word_at(RG1P, 2));
              RG1L = DM2_move_075f_06bd(RG1P, RG4L);
              if (RG1W != 0)
                return lcon(0x1);
            }
          }
          RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG2UW)));
        }
        //m_1D77D:
        RG1L = unsignedlong(s350.v1e0562.getxA());
        if (RG6W == RG1W)
        {
          RG1L = unsignedlong(s350.v1e0562.getyA());
          if (RG1W == vo_08.peek16())
          {
            //m_1D79E:
            RG1L = 0;
            skip00458 = true;
          }
        }

        if (!skip00458)
          //m_1D797:
          RG1L = 1;

        //m_1D7A0:
        if (RG1L != 0)
        {
          RG4L = signedlong(vo_08.peek16());
          RG1L = DM2_1BAAD(signedlong(RG6W), RG4L);
          if (RG1L != 0)
            break;
        }
      }
    }

    //m_1D7BF:
    RG5L++;
    RG1L = signedlong(RG5W);
    if (RG1L >= lcon(0x4))
      return 0;
  }
  FOREND
}

// belongs to DM2_CREATURE_GO_THERE
static i32 DM2_19f0_1511(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;

  return DM2_CREATURE_CAN_HANDLE_IT(unsignedlong(RG1W), lcon(0x9));
}

// belongs to DM2_CREATURE_GO_THERE
static unk* DM2_D283(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  c_wlreg _RG6;
  i8* RG7p;
  c_ql32 vql_00;
  c_5bytes c12_04;

  vql_00.poke32(RG1L);
  RG6L = RG4L;
  RG5l = signedlong(RG4W);
  RG3L = signedlong(vql_00.peek16());
  RG4L = RG5l;
  RG1L = dm2_GET_TILE_VALUE(RG3L, RG4L);
  RG4L = unsignedlong(RG1Blo) >> 5;
  RG1Bhi = RG4Blo;
  RG4L = unsignedlong(RG1Bhi);
  if (RG4L != lcon(0x5))
    return NULL;
  RG1Blo &= lcon(0x8);
  RG1L = unsignedlong(RG1Blo);
  if (RG1L == 0)
    return NULL;
  RG1L = vql_00.peek32() + 1;
  RG4L = signedlong(RG1W);
  RG2L = RG5l;
  RG1L = DM2_GET_TELEPORTER_DETAIL(&c12_04, RG4L, RG2L);
  if (RG1L == 0)
  {
    RG1L = vql_00.peek32() - 1;
    RG4L = signedlong(RG1W);
    RG2L = RG5l;
    RG1L = DM2_GET_TELEPORTER_DETAIL(&c12_04, RG4L, RG2L);
    if (RG1L == 0)
    {
      RG1L = RG6L + 1;
      RG2L = signedlong(RG1W);
      RG4L = RG3L;
      RG1L = DM2_GET_TELEPORTER_DETAIL(&c12_04, RG4L, RG2L);
      if (RG1L == 0)
      {
        RG1L = RG6L - 1;
        RG2L = signedlong(RG1W);
        RG4L = RG3L;
        RG1L = DM2_GET_TELEPORTER_DETAIL(&c12_04, RG4L, RG2L);
        if (RG1L == 0)
          return NULL;
      }
    }
  }
  RG4L = signedlong(RG6W);
  RG1RP = DM2_GET_ADDRESS_OF_TILE_RECORD(vql_00.peek16(), RG4W);
  RG7p = RG1P;
  RG2W = unsignedword(RG2Blo);
  RG2Blo = c12_04.b_04;
  RG4W = word_at(RG1P, lcon(0x4));
  RG4UW >>= bcon(0x8);
  if (RG2W != RG4W)
    return NULL;
  RG4W = word_at(RG1P, 2);
  RG4L <<= bcon(0x6);
  _RG4.ushiftr11();
  RG3L = signedlong(RG4W);
  RG1W = word_at(RG1P, 2) & lcon(0x1f);
  RG2L = signedlong(RG1W);
  RG4L = unsignedlong(c12_04.b_03);
  RG1L = signedlong(DM2_CALC_SQUARE_DISTANCE(unsignedword(c12_04.b_02), RG4W, RG2W, RG3W));
  return RG1L == lcon(0x1) ? RG7p : NULL;
}

// recursive!!!
// stacksize was 0x68
i32 DM2_CREATURE_GO_THERE(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5; // TODO: not checked yet
  c_wlreg _RG61;
  c_wlreg _RG62;
  i32 RG63l;
  unk* xp_00 = NULL; // serious!
  i32 vl_04;
  i32 vl_08;
  i32 vl_0c;
  i32 vl_10 = 0; // serious!
  i32 vl_14;
  i32 vl_18;
  i16 vw_20;
  i16 vw_24;
  i16 vw_28;
  i16 vw_2c;
  i16 vw_30;
  c_o32 vo_34; // poke 16 peek 16 32 or16
  i16 vw_38;
  i16 vw_3c;
  c_ql32 vql_40;
  i16 vw_44;
  c_o32 vo_48; // poke 16 peek 8 16 32 inc16
  i16 vw_4c;
  i8 vb_50;
  i8 vb_54;
  i16 parw01;
  i16 parw02;
  i16 parw05;
  i16 parw06;
  i16 parw07;
  i16 parw08;
  i16 parw09;
  i16 parw10;
  i16 parw11;
  i16 parw12;
  i16 parw13;
  i16 parw14;
  i16 parw15;
  i16 parw16;
  bool flag;
  bool skip00478 = false;
  bool skip00491 = false;

  i32 parl03 = RG1L;


  vb_54 = RG1Blo;
  vw_28 = RG4W;
  vw_24 = RG2W;
  RG5L = RG3L;
  RG4L = 0;
  vl_10 = RG4L;
  vl_08 = RG4L;
  vl_14 = RG4L;
  vl_18 = RG4L;
  RG1Blo &= lcon(0xffffff80);
  RG1W = unsignedword(RG1Blo);
  vw_30 = RG1W;
  if (RG1W != 0)
    vw_38 = -1;
  RG1Blo = vb_54 & lcon(0x40);
  RG1W = unsignedword(RG1Blo);
  vw_20 = RG1W;
  RG1Blo = vb_54 & lcon(0x20);
  RG61W = unsignedword(RG1Blo);
  RG1Bhi = vb_54 & lcon(0x1f);
  vb_54 = RG1Bhi;
  if (RG1Bhi == 2)
    jump L_prefin;
  RG1L = unsignedlong(vb_54);
  RG1Blo = table1d6290[RG1L];
  vb_50 = RG1Blo;
  if (RG1Blo == 0 && s350.v1e0576 == 0)
    jump L_prefin;
  RG1W = argw1;
  vw_44 = RG1W;
  RG2L = signedlong(argw1);
  if (RG2L != lcon(0x6))
  {
    //m_1D8EB:
    if (RG2L < lcon(0x4))
    {
      //m_1DB9A:
      if (RG5W != lcon(0xffffffff))
      {
        bool skip00463 = false;
        //m_1DBC3:
        if (RG5W == vw_28)
        {
          RG1W = vw_24; // adj
          if (RG1W == argw0)
          {
            RG1L = 1;
            skip00463 = true;
          }
        }

        if (!skip00463)
          //m_1DBD9:
          RG1L = 0;

        //m_1DBDB:
        vl_0c = RG1L;
        if (RG1L == 0 && argw1 == lcon(0xffffffff))
        {
          RG3L = signedlong(argw0);
          RG2L = signedlong(RG5W);
          RG4L = signedlong(vw_24);
          RG1L = signedlong(DM2_CALC_VECTOR_DIR(vw_28, RG4W, RG2W, RG3W));
          argw1 = RG1W;
          skip00491 = true;
        }
      }
      else
      {
        vl_0c = 0;
        RG5W = vw_28 + table1d27fc[RG2L]; // adj
        RG1W = vw_24; // adj
        RG4W = table1d2804[RG2L];
        RG1W += RG4W; // adj
        argw0 = RG1W;
      }
    }
    else
    {
      vl_0c = 0;
      RG1W = s350.v1e0576 & lcon(0x2000);
      RG1L = unsignedlong(RG1W);
      if (RG1L == 0)
        jump L_prefin;
      RG3L = 0;
      vql_40.poke32(RG3L);
      RG4L = signedlong(vw_24);
      RG1L = dm2_GET_TILE_VALUE(signedlong(vw_28), RG4L);
      RG4W = unsignedword(RG4Blo);
      RG4Blo = RG1Blo;
      vo_48.poke16(RG4W);
      RG1L = unsignedlong(RG1Blo) >> 5;
      RG1Blo--;
      if (mkub(RG1Blo) <= mkub(lcon(0x5)))
      {
        bool skip00482 = false;
        bool skip00481 = false;
        bool skip00483 = false;
        bool skip00488 = false;
        RG1L = unsignedlong(RG1Blo);
        switch (RG1L)
        {
          case 0:
          case 4:
          case 5:
            skip00481 = true;
            break;

          case 1:
            //m_1D94C:
            if (RG2L != lcon(0x5))
              skip00481 = true;
            else
            {
              RG1L = RG4L;
              RG1Blo &= lcon(0x8);
              RG1L = unsignedlong(RG1W);
              if (RG1L == 0)
                skip00481 = true;
              else
              {
                RG1L = RG4L;
                RG1Blo &= 0x1;
                RG1L = unsignedlong(RG1W);
                if (RG1L != 0)
                  skip00481 = true;
                else
                {
                  vql_40.poke32(lcon(0x8));
                  vw_38 = lcon(0x38);
                  skip00488 = true;
                }
              }
            }
            break;

          case 2:
            //m_1DA09:
            RG1L = RG4L;
            RG1Blo &= 0x4;
            RG1L = unsignedlong(RG1W);
            if (RG1L == 0)
            {
              //m_1DA21:
              if (RG2L == lcon(0x5))
              {
                vw_38 = lcon(0x36);
                skip00482 = true;
              }
            }
            else
            {
              if (RG2L == lcon(0x4))
              {
                vw_38 = lcon(0x35);
                skip00482 = true;
              }
            }
          case 3:
            break;

          default: throw(THROW_DMABORT);
        }

        if (skip00482)
        {
          //m_1DA2C:
          RG1L = DM2_query_0cee_06dc(signedlong(vw_28), signedlong(vw_24)) + 2;
          RG1W &= 0x3;
          vw_44 = RG1W;
          skip00483 = true;
        }

        if (skip00481)
        {
          //m_1D98D:
          RG1W = vo_48.peek16();
          RG1W &= 0x2;
          RG1L = unsignedlong(RG1W);
          if (RG1L != 0)
          {
            RG3P = ADRESSOF(unk*, &xp_00);
            RG1L = signedlong(argw1);
            if (RG1L != lcon(0x4))
              //m_1D9B4:
              RG1L = 1;
            else
              RG1L = lcon(0xffffffff);
            //m_1D9B9:
            RG2L = signedlong(RG1W);
            RG4L = signedlong(vw_24);
            RG1L = DM2_FIND_LADDAR_AROUND(signedlong(vw_28), RG4L, RG2L, UPCAST(unk*, RG3P));
            vw_44 = RG1W;
            if (RG1W < 0)
            {
              //m_1D9F7:
              vw_38 = lcon(0x37);
              vql_40.poke32(lcon(0x10));
              skip00488 = true;
            }
            else
            {
              RG1L = signedlong(argw1);
              RG1Blo = RG1L != lcon(0x4) ? 1 : 0;
              RG1L = unsignedlong(RG1Blo) + lcon(0x39);
              vw_38 = RG1W;
              skip00483 = true;
            }
          }
        }

        if (skip00488)
        {
          //m_1D974:
          RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
          RG1W = word_at(RG1P, lcon(0xe));
          RG1L <<= bcon(0x6);
          RG1UW >>= 14;
          vw_44 = RG1W;
        }

        if (skip00483)
          //m_1D9EE:
          vql_40.poke32(lcon(0x100));
      }

      bool skip00461 = false;
      //m_1DA4C:
      RG4L = unsignedlong(s350.v1e0576);
      RG1L = unsignedlong(vql_40.peek16());
      if ((RG1L & RG4L) == 0)
        jump L_prefin;
      vw_2c = ddat.v1d3248;
      if (DOWNCAST(unk, xp_00) != NULL)
      {
        RG1L = signedlong(vw_38);
        if (RG1L == lcon(0x39) || RG1L == lcon(0x3a))
        {
          RG1P = DOWNCAST(unk, xp_00);
          RG1W = word_at(RG1P, lcon(0x6));
          RG1L <<= bcon(0x5);
          _RG1.ushiftr11();
          vw_28 = RG1W;
          RG1P = DOWNCAST(unk, xp_00);
          RG1W = word_at(RG1P, lcon(0x6));
          _RG1.ushiftr11();
          vw_24 = RG1W;
          RG1P = DOWNCAST(unk, xp_00);
          RG1W = word_at(RG1P, 2);
          RG1UW >>= bcon(0x7);
          vw_4c = RG1W;
          skip00461 = true;
        }
      }

      if (!skip00461)
      {
        //m_1DAB8:
        RG3P = ADRESSOF(i16, &vw_24);
        RG2P = ADRESSOF(i16, &vw_28);
        RG1L = signedlong(argw1);
        if (RG1L != lcon(0x4))
          //m_1DAD2:
          RG1L = 1;
        else
          RG1L = lcon(0xffffffff);
        //m_1DAD7:
        RG4L = signedlong(RG1W);
        RG1L = DM_LOCATE_OTHER_LEVEL(signedlong(ddat.v1d3248), RG4L, RG2P16, RG3P16, NULL);
        vw_4c = RG1W;
        if (RG1W < 0)
          jump L_prefin;
        RG4L = signedlong(vw_38);
        if (RG4L == lcon(0x37))
        {
          RG1P = DOWNCAST(t_tile*, mapdat.tmpmap) + 4 * signedlong(RG1W);
          RG4L = signedlong(vw_28) << 2;
          RG1P = pointer_at(RG1P) + RG4L;
          RG4L = signedlong(vw_24);
          RG1P = pointer_at(RG1P) + RG4L;
          RG4W = unsignedword(RG4Blo);
          RG4Blo = byte_at(RG1P);
          vo_48.poke16(RG4W);
          RG1Blo = RG4Blo;
          RG1L = unsignedlong(RG1Blo) >> 5;
          RG1L = unsignedlong(RG1Blo);
          if (RG1L != 2)
            jump L_prefin;
          RG1L = RG4L;
          RG1Blo &= lcon(0x8);
          RG1L = unsignedlong(RG1W);
          if (RG1L == 0)
            jump L_prefin;
        }
      }

      //m_1DB51:
      RG4L = unsignedlong(vw_4c);
      RG1L = DM2_IS_CREATURE_ALLOWED_ON_LEVEL(unsignedlong(s350.v1e054c), RG4L);
      if (RG1L == 0)
        jump L_prefin;
      vl_08 = 1;
      DM2_1c9a_0648(signedlong(vw_4c));
      RG5W = vw_28; // adj
      argw0 = vw_24;
      RG1L = lcon(0xffffffff);
      vw_24 = RG1W;
      vw_28 = RG1W;
    }
  }
  else
  {
    vl_0c = 0;
    RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
    RG1W = word_at(RG1P, lcon(0xe));
    RG1L <<= bcon(0x6);
    RG1UW >>= 14;
    skip00491 = true;
  }

  if (skip00491)
    //m_1DC07:
    vw_44 = RG1W;

  //m_1DC0B:
  if (RG5W >= 0 && RG5W < mapdat.map_width)
    //m_1DC20:
    RG1L = 0;
  else
    RG1L = 1;
  //m_1DC22:
  if (RG1L != 0)
    jump L_prefin;
  RG3W = argw0;
  if (RG3W < 0 || RG3W >= mapdat.map_height)
    RG1L = 1;
  if (RG1L != 0)
    jump L_prefin;
  if (vb_50 != 0)
  {
    bool skip00494 = false;
    RG2Bhi = vb_54;
    if (mkub(RG2Bhi) < mkub(lcon(0x7)))
    {
      //m_1DC6A:
      if (RG2Bhi == lcon(0x3))
      {
        RG4L = signedlong(argw0);
        RG1L = signedlong(RG5W);
        DM2_19f0_045a(RG1L, RG4L);
        vo_48.poke16(ddat.v1e08ae);
        RG1L = unsignedlong(vo_48.peek16()) >> 5;
        if (RG1Blo == 0)
          jump L_fin;
        RG1L = unsignedlong(RG1Blo);
        if (RG1L == lcon(0x6))
        {
          RG1W = vo_48.peek16();
          RG1W &= 0x5;
          RG1L = unsignedlong(RG1W);
          if (RG1L == 0)
            jump L_fin;
        }
        skip00494 = true;
      }
    }
    else
    {
      if (mkub(RG2Bhi) <= mkub(lcon(0x7)))
      {
        //m_1DCC1:
        RG4L = signedlong(argw0);
        RG1L = DM2_1BA1B(signedlong(RG5W), RG4L);
        RG1Blo = RG1L == 0 ? 1 : 0;
        RG1L = unsignedlong(RG1Blo);
        flag = RG1L == 0;
        vl_10 = RG1L;
        if (flag)
          jump L_fin;
        RG4L = signedlong(argw0);
        RG1L = signedlong(RG5W);
        DM2_19f0_045a(RG1L, RG4L);
        jump L_fin;
      }
      if (RG2Bhi == lcon(0x8))
      {
        //m_1DCF5:
        RG2L = signedlong(argw0);
        RG3L = signedlong(RG5W);
        RG4L = RG2L;
        RG1L = RG3L;
        DM2_19f0_045a(RG1L, RG4L);
        RG1L = unsignedlong(ddat.v1e08ae) >> 5;
        if (mkub(RG1Blo) <= mkub(lcon(0x7)))
        {
          bool skip00485 = false;
          RG1L = unsignedlong(RG1Blo);
          switch (RG1L)
          {
            case 0:
              skip00485 = true;
              break;

            case 1:
            case 2:
              skip00494 = true;
              break;

            case 3:
            case 7:
              jump L_fin;

            case 4:
              //m_1DE10:
              RG1Blo = CUTX8(ddat.v1e08ae) & lcon(0x7);
              RG1W = unsignedword(RG1Blo);
              vo_48.poke16(RG1W);
              if (RG1W != 0)
              {
                RG5L = unsignedlong(RG1W);
                if (RG5L != lcon(0x5) && RG5L != lcon(0x1))
                {
                  RG4L = RG2L;
                  RG1L = DM2_QUERY_DOOR_STRENGTH(DM2_IS_REBIRTH_ALTAR(UPCAST(unk, DOWNCAST(t_record, DM2_GET_ADDRESS_OF_TILE_RECORD(RG3W, RG4W)))) & lcon(0xff));
                  ddat.v1e08c4 = RG1W;
                  if (RG5L != lcon(0x4))
                  {
                    RG4L = signedlong(RG1W);
                    if (RG4L > lcon(0x1))
                    {
                      RG2L = RG1L;
                      RG2W >>= bcon(0x1);
                      if (RG5L != 2)
                      {
                        //m_1DE9C:
                        if (RG4L > lcon(0x3))
                          //m_1DEA4:
                          ddat.v1e08c4 = RG2W;
                        else
                        {
                          RG1L--;
                          ddat.v1e08c4 = RG1W;
                        }
                      }
                      else
                      {
                        if (RG4L > lcon(0x3))
                        {
                          //m_1DE7A:
                          ddat.v1e08c4 = RG2W;
                          RG1L = signedlong(RG2W);
                          RG4L = RG1L / 2;
                          RG1L -= RG4L;
                          ddat.v1e08c4 = RG1W;
                        }
                        else
                          ddat.v1e08c4 = 1;
                      }
                    }
                  }
                }
              }
              skip00494 = true;
              break;

            case 5:
              //m_1DD27:
              RG1W = ddat.v1e08ae & lcon(0x8);
              RG1L = unsignedlong(RG1W);
              if (RG1L == 0)
              {
                //m_1DDD1:
                RG4L = RG3L;
                RG1L = DM2_GET_TELEPORTER_DETAIL(&ddat.v1e08b8, RG4L, RG2L);
                ddat.v1e08b7 = RG1Blo;
                if (RG1Blo != 0)
                {
                  RG1L = unsignedlong(ddat.v1e08b8.b_04);
                  if (RG1W != ddat.v1e0266)
                  {
                    if (RG1W != ddat.v1e027c)
                      jump L_fin;
                  }
                }
              }
              else
              {
                RG4L = RG2L;
                RG1P = DM2_D283(RG3L, RG4L);
                RG2P = RG1P;
                if (RG1P != NULL)
                {
                  bool skip00484 = false;
                  RG1W = word_at(RG1P, lcon(0x4));
                  RG1UW >>= bcon(0x8);
                  vo_48.poke16(RG1W);
                  RG1L = unsignedlong(vo_48.peek16());
                  RG4L = signedlong(ddat.v1e0266);
                  if (RG1L == RG4L)
                    skip00484 = true;
                  else
                  {
                    RG4L = signedlong(ddat.v1e027c);
                    if (RG1L == RG4L)
                      skip00484 = true;
                  }

                  if (skip00484)
                  {
                    //m_1DD7F:
                    vw_4c = ddat.v1d3248;
                    RG1L = signedlong(vo_48.peek16());
                    DM2_1c9a_0648(RG1L);
                    put16(parw02, lcon(0x6));
                    RG1W = word_at(RG2P, 2);
                    RG1L <<= bcon(0x6);
                    _RG1.ushiftr11();
                    RG1L = signedlong(RG1W);
                    put16(parw01, RG1W);
                    RG4W = word_at(RG2P, 2) & lcon(0x1f);
                    RG4L = signedlong(RG4W);
                    RG1L = DM2_CREATURE_GO_THERE(unsignedlong(vb_54), RG4L, RG1L, RG4L, parw01, parw02);
                    vl_10 = RG1L;
                    DM2_1c9a_0648(signedlong(vw_4c));
                    jump L_fin;
                  }
                }
              }
              skip00494 = true;
              break;

            case 6:
              //m_1DEAD:
              RG1W = ddat.v1e08ae & lcon(0x5);
              RG1L = unsignedlong(RG1W);
              if (RG1L != 0)
                skip00494 = true;
              else
                skip00485 = true;
              break;

            default: throw(THROW_DMABORT);
          }

          if (skip00485)
          {
            //m_1DEBE:
            RG1L = signedlong(ddat.v1e0400);
            if (RG1L >= lcon(0x32))
            {
              ddat.v1e08b6 = 1;
              jump L_fin;
            }
            //m_1DED7:
            RG1W = ddat.v1e0400;
            ddat.v1e08c4 = RG1W;
            skip00494 = true;
          }
        }
      }
    }

    if (skip00494)
    {
      //m_1DE90:
      vl_10 = 1;
      jump L_fin;
    }
  }

  //m_1DEDF:
  RG4L = signedlong(argw0);
  RG1L = signedlong(RG5W);
  DM2_19f0_045a(RG1L, RG4L);
  RG1W = s350.v1e0582;
  if (RG1W != 0)
  {
    RG2L = signedlong(argw1);
    if (RG2L != lcon(0x6))
    {
      RG4L = unsignedlong(RG1W);
      RG1L = signedlong(CUTX16(DM2_1c9a_1a48(lcon(0x1), RG4L)));
      if (RG1L != lcon(0xffffffff))
      {
        RG3L = RG2L + 2 & lcon(0x3);
        RG4L = lcon(0x1) << RG3Blo;
        if ((RG1L & RG4L) != 0)
          jump L_prefin;
      }
    }
  }
  //m_1DF2D:
  RG4Blo = vb_54;
  if (RG4Blo == lcon(0x4) || RG4Blo == lcon(0x5))
  {
    RG1P = s350.v1e0552;
    RG1Blo = byte_at(RG1P, 2);
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == lcon(0xff))
      vb_54 = 0;
  }
  vo_48.poke16(ddat.v1e08ae);
  RG1L = unsignedlong(vo_48.peek16()) >> 5;
  if (mkub(RG1Blo) <= mkub(lcon(0x7)))
  {
    bool skip00486 = false;
    RG1L = unsignedlong(RG1Blo);
    RG2W = vo_48.peek16();
    RG2W &= 0x1;
    RG4W = vo_48.peek16();
    RG4W &= lcon(0x8);
    switch (RG1L)
    {
      case 0:
        //m_1DF8B:
        vql_40.poke32(lcon(0x1));
        break;

      case 1:
        vql_40.poke32(2);
        break;

      case 2:
        //m_1DF97:
        RG1L = unsignedlong(RG4W);
        if (RG1L == 0)
          //m_1DFD3:
          vql_40.poke32(lcon(0x6));
        else
        {
          RG1L = unsignedlong(RG2W);
          if (RG1L == 0)
          {
            //m_1DFB5:
            if (vl_08 == 0)
              //m_1DFC7:
              vql_40.poke32(lcon(0x800c));
            else
              vql_40.poke32(lcon(0xc));
          }
          else
            vql_40.poke32(lcon(0x8024));
        }
        break;

      case 3:
        //m_1DFDF:
        vql_40.poke32(lcon(0x100));
        break;

      case 4:
        //m_1DFEB:
        RG1Blo = vo_48.peek8() & lcon(0x7);
        RG1W = unsignedword(RG1Blo);
        vo_48.poke16(RG1W);
        if (RG1W != 0)
          skip00486 = true;
        else
        {
          RG3Blo = vb_54;
          if (RG3Blo != lcon(0x4) && RG3Blo != lcon(0x5))
            vql_40.poke32(2);
          else
            skip00486 = true;
        }
        break;

      case 5:
        //m_1E0C7:
        RG1L = unsignedlong(RG4W);
        if (RG1L != 0)
        {
          //m_1E123:
          if (ddat.v1e08b0 == lcon(0xffffffff))
          {
            RG4L = signedlong(argw0);
            RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(RG5W, RG4W));
            ddat.v1e08b0 = RG1W;
          }
          RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(ddat.v1e08b0)));
          RG1W = word_at(RG1P, 2); // TODO: gets extended w/o conversion now
          RG1L = 2 * RG1L;
          RG1UW >>= 14;
          vo_48.poke16(RG1W);
          RG1L = unsignedlong(vo_48.peek16());
          if (RG1L != lcon(0x1) && RG1L != lcon(0x3))
            vql_40.poke32(2);
          else
            vql_40.poke32(lcon(0x400));
        }
        else
        {
          RG2L = signedlong(argw0);
          RG4L = signedlong(RG5W);
          RG1L = DM2_GET_TELEPORTER_DETAIL(&ddat.v1e08b8, RG4L, RG2L);
          if (RG1L == 0)
            //m_1E117:
            vql_40.poke32(lcon(0x402));
          else
          {
            ddat.v1e08b7 = 1;
            RG1L = DM2_IS_CREATURE_ALLOWED_ON_LEVEL(unsignedlong(s350.v1e054c), unsignedlong(ddat.v1e08b8.b_04));
            if (RG1L == 0)
              jump L_prefin;
            vql_40.poke32(lcon(0x2000));
          }
        }
        break;

      case 6:
        //m_1E179:
        RG1W = vo_48.peek16();
        RG1W &= 0x4;
        RG1L = unsignedlong(RG1W);
        if (RG1L != 0)
          vql_40.poke32(2);
        else
        {
          RG1W = RG2W;
          if (RG1L == 0)
            //m_1E19B:
            vql_40.poke32(lcon(0x40));
          else
            vql_40.poke32(lcon(0x80));
        }
        break;

      case 7:
        jump L_prefin;

      default: throw(THROW_DMABORT);
    }

    if (skip00486)
    {
      //m_1E00C:
      RG1L = unsignedlong(vo_48.peek16());
      if (RG1L == lcon(0x5))
        vql_40.poke32(2);
      else
      {
        bool skip00465 = false;
        if (RG1L != lcon(0x4))
        {
          bool skip00464 = false;
          //m_1E02C:
          if (ddat.v1e08b0 == lcon(0xffffffff))
          {
            RG4L = signedlong(argw0);
            RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(RG5W, RG4W));
            ddat.v1e08b0 = RG1W;
          }
          RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(ddat.v1e08b0)));
          RG2P = RG1P;
          if (vw_20 == 0)
            skip00464 = true;
          else
          {
            RG4Blo = byte_at(RG1P, lcon(0x3));
            if ((RG4Blo & bcon(0x4)) == 0)
              skip00464 = true;
            else
            {
              if ((RG4Blo & bcon(0x2)) != 0)
                vql_40.poke32(2);
              else
                skip00465 = true;
            }
          }

          if (skip00464)
          {
            //m_1E06F:
            RG4Bhi = byte_at(RG2P, lcon(0x3));
            if ((RG4Bhi & bcon(0x4)) == 0 || (RG4Bhi & bcon(0x2)) != 0)
            {
              //m_1E088:
              if (jz_test8(location(RG2P + 2), lcon(0x20)))
                //m_1E0A8:
                RG1L = 1;
              else
              {
                RG1P = s350.v1e0552;
                RG1W = word_at(RG1P);
                RG1L = unsignedlong(RG1W) >> 6;
                RG1L &= 0x3;
                RG1L = unsignedlong(RG1W);
              }
              //m_1E0AD:
              RG4L = unsignedlong(vo_48.peek16());
              if (RG4L > RG1L)
                skip00465 = true;
              else
                vql_40.poke32(2);
            }
            else
              vql_40.poke32(lcon(0x200));
          }
        }
        else
          skip00465 = true;

        if (skip00465)
          //m_1E020:
          vql_40.poke32(lcon(0x4200));
      }
    }
  }

  //m_1E1A2:
  RG1W = s350.v1e0576;
  RG2L = vql_40.peek32();
  RG2W &= RG1W;
  flag = RG2W == 0;
  vql_40.poke32(RG2L);
  if (flag)
    jump L_prefin;
  if (vb_54 == lcon(0x1))
  {
    RG1L = RG5L;
    RG1W -= ddat.v1e08d8;
    RG1L = signedlong(DM2_ABS(RG1W));
    RG4L = signedlong(RG1W);
    RG1W = argw0;
    RG1W -= ddat.v1e08d4;
    RG1L = signedlong(DM2_ABS(RG1W)) + RG4L;
    if (RG1L <= lcon(0x1))
      jump L_prefin;
  }
  //m_1E1EB:
  RG3L = 0;
  vo_34.poke16(RG3W);
  RG1W = ddat.v1d3248;
  if (RG1W == ddat.v1e08d6)
  {
    if (RG5W == ddat.v1e08d8)
    {
      RG1W = argw0;
      if (RG1W == ddat.v1e08d4)
        vo_34.poke16(lcon(0x800));
    }
  }
  if (vl_0c == 0)
  {
    RG1L = DM2_19f0_050f();
    RG3L = RG1L;
    RG1Blo = RG1W != wcon(0xfffe) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 0)
    {
      bool skip00466 = false;
      if (vw_20 == 0)
        skip00466 = true;
      else
      {
        RG1P = DM2_QUERY_CREATURE_AI_SPEC_FROM_TYPE(unsignedlong(RG3W));
        if (word_at(RG1P + lcon(0xa)) == 0)
          skip00466 = true;
      }

      if (skip00466)
      {
        bool skip00468 = false;
        //m_1E24E:
        if (RG61W != wcon(0x0) && vl_08 == 0)
        {
          RG1L = DM2_19f0_1511(unsignedlong(RG3W));
          vl_14 = RG1L;
          if (RG1L != 0)
          {
            //m_1E271:
            RG1L = 0;
            skip00468 = true;
          }
        }

        if (!skip00468)
          //m_1E26A:
          RG1L = 1;

        //m_1E273:
        if (RG1L != 0)
          vo_34.or16(lcon(0x1000)); // modified
      }
    }
  }
  //m_1E27B:
  RG4L = unsignedlong(s350.v1e0576);
  RG1L = unsignedlong(vo_34.peek16());
  RG4L &= RG1L;
  if (RG4L != RG1L)
    jump L_prefin;
  RG2Bhi = vb_54;
  if (RG2Bhi == lcon(0x4) || RG2Bhi == lcon(0x5))
  {
    RG1L = DM2_19f0_04bf();
    //m_1E2AA:
    for (;;)
    {
      bool skip00472 = false;
      RG3L = RG1L;
      if (RG3W == lcon(0xfffffffe))
      {
        //m_1E3C4:
        if (vb_54 == bcon(0x5) && vl_18 == 0)
        {
          RG1L = DM2_19f0_13aa(signedlong(RG5W), signedlong(argw0));
          if (RG1L != 0)
            jump L_prefin;
        }
        break;
      }
      RG4L = 0;
      RG1L = RG3L;
      RG1Blo ^= RG3Blo;
      RG1Bhi &= lcon(0x3c);
      RG1L = unsignedlong(RG1W) >> bcon(0xa);
      vo_48.poke16(RG1W);
      RG1L = unsignedlong(vo_48.peek16());
      if (RG1L != lcon(0xf))
      {
        //m_1E31E:
        if (RG1L == lcon(0xe))
        {
          bool skip00471 = false;
          RG1L = unsignedlong(s350.v1e0562.getxA());
          if (RG5W == RG1W)
          {
            RG1L = unsignedlong(s350.v1e0562.getyA());
            if (RG1W == argw0)
            {
              //m_1E347:
              RG1L = 0;
              skip00471 = true;
            }
          }

          if (!skip00471)
            //m_1E340:
            RG1L = 1;

          //m_1E349:
          if (RG1L != 0)
          {
            RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
            RG2P = RG1P;
            RG1L = signedlong(argw1);
            if (RG1L != lcon(0x6))
              //m_1E369:
              RG1W = argw1;
            else
              RG1W = vw_44;
            //m_1E36C:
            vw_3c = RG1W; // truncated
            RG1W = timdat.timerarray[word_at(RG2P, lcon(0x6))].getB();
            RG1L <<= 4;
            RG1UW >>= 14;
            if (RG1W != vw_3c)
            {
              RG4L = unsignedlong(word_at(RG2P, 2));
              RG1L = DM2_move_075f_06bd(RG2P, RG4L);
              skip00472 = true;
            }
          }
        }
      }
      else
      {
        RG2L = unsignedlong(RG3W);
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
        RG1W = word_at(RG1P, 2) & lcon(0x7f);
        RG1L = unsignedlong(RG1W);
        if (RG1L != lcon(0x7))
        {
          //m_1E309:
          if (RG1L == lcon(0xe))
            vl_18 = 1;
        }
        else
        {
          RG4L = unsignedlong(s350.v1e054c);
          RG1W = DM2_CALC_CLOUD_DAMAGE(RG2L, RG4L);
          skip00472 = true;
        }
      }

      if (skip00472)
        //m_1E3AA:
        RG4L = RG1L;

      //m_1E3AC:
      if (RG4W != 0)
        jump L_prefin;
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG3UW)));
    }
  }
  //m_1E3E6:
  if (vl_0c == 0)
  {
    RG1W = ddat.v1e08d6;
    if (RG1W == ddat.v1d3248)
    {
      RG1L = signedlong(s350.v1e0584);
      if ((table1d607e[RG1L].uc[1] & bcon(0x40)) != 0)
      {
        RG1P = s350.v1e0552;
        if (jz_test8(location(RG1P), lcon(0x20)))
        {
          RG1L = unsignedlong(ddat.v1e08ae) >> 5 & lcon(0xff);
          if (RG1L == lcon(0x4))
          {
            RG3L = signedlong(ddat.v1e08d4);
            RG2L = signedlong(ddat.v1e08d8);
            RG4L = signedlong(argw0);
            RG1L = signedlong(DM2_CALC_SQUARE_DISTANCE(RG5W, RG4W, RG2W, RG3W));
            if (RG1L < 2)
            {
              RG1L = unsignedlong(s350.v1e0562.getxA());
              RG2W = vw_28; // adj
              if (RG1W != RG2W)
                jump L_prefin;
              RG1W ^= RG2W; // adj
              RG1Blo = s350.v1e0562.getyA();
              if (RG1W != vw_24)
                jump L_prefin;
              RG1L = signedlong(DM2_RANDDIR());
              if (RG1W != 0)
                jump L_prefin;
            }
          }
        }
      }
    }
  }
  //m_1E490:
  vl_10 = 1;
  if (vl_08 == 0)
  {
    RG62L = vql_40.peek32();
    if (RG62W != lcon(0x400))
    {
      //m_1E598:
      if (vl_0c != 0)
        jump L_prefin;
      if (RG62W == lcon(0x4000))
      {
        RG1L = signedlong(vw_44);
        put16(parw08, RG1W);
        put16(parw07, argw0);
        RG3L = signedlong(RG5W);
        return DM2_19f0_0d10(unsignedlong(CUTX8(vw_30)), signedlong(vw_28), signedlong(vw_24), RG3L, parw07, parw08);
      }
      RG1L = RG62L;
      RG1W &= lcon(0x8000);
      RG1L = unsignedlong(RG1W);
      if (RG1L == 0)
        jump L_prefin;
      if (vb_54 != lcon(0x6))
      {
        //m_1E5F4:
        RG1W = s350.v1e0576;
        vo_34.poke16(RG1W);
        RG1L &= lcon(0xffff7db7);
        s350.v1e0576 = RG1W;
        RG1L = signedlong(vw_44);
        put16(parw10, RG1W);
        RG4W = argw0;
        RG4W += table1d2804[RG1L];
        RG4L = signedlong(RG4W);
        put16(parw09, RG4W);
        RG1W = table1d27fc[RG1L];
        RG1L += RG5L;
        RG3L = signedlong(RG1W);
        RG2L = signedlong(argw0);
        RG4L = signedlong(RG5W);
        RG1L = DM2_CREATURE_GO_THERE(unsignedlong(mkub(vb_54 | CUTX8(vw_20))), RG4L, RG2L, RG3L, parw09, parw10);
        vl_10 = RG1L;
        RG1W = vo_34.peek16();
        s350.v1e0576 = RG1W;
      }
      else
        vl_10 = 0;
      //m_1E651:
      if (vl_10 == 0)
      {
        //m_1E65F:
        RG1W = s350.v1e0576 & lcon(0x8);
        RG1L = unsignedlong(RG1W);
        if (RG1L != 0)
        {
          s350.v1e0576 &= lcon(0x7fff);
          put16(parw12, vw_44);
          RG1L = signedlong(argw0);
          put16(parw11, RG1W);
          RG3L = signedlong(RG5W);
          RG2L = signedlong(vw_24);
          RG4L = signedlong(vw_28);
          RG1L = DM2_CREATURE_GO_THERE(unsignedlong(mkub(vb_54 | CUTX8(vw_20))), RG4L, RG2L, RG3L, parw11, parw12);
          vl_10 = RG1L;
          if (RG1L != 0)
            vw_38 = lcon(0x38);
          RG1W = vo_34.peek16();
          s350.v1e0576 = RG1W;
        }
      }
      else
        vw_38 = 5;
      //m_1E6BA:
      if (vl_10 == 0)
        ddat.v1e08b6 = 1;
      jump L_prefin;
    }
  }
  //m_1E4AB:
  RG1L = signedlong(argw0);
  vl_04 = RG1L;
  parl03 = RG1L;
  RG63l = signedlong(RG5W);
  RG2L = signedlong(vw_24);
  RG4L = signedlong(vw_28);
  RG1L = unsignedlong(s350.v1e054c);
  RG3L = RG63l;
  DM2_move_2fcf_0434(RG1L, RG4L, RG2L, RG3L, parl03, 0);
  RG4L = signedlong(ddat.v1d3248);
  RG1L = unsignedlong(ddat.v1e102c);
  if (RG4L == RG1L)
  {
    RG1L = unsignedlong(ddat.v1e102a);
    if (RG63l == RG1L)
    {
      RG1L = unsignedlong(ddat.v1e1028);
      if (RG1L == vl_04)
        jump L_prefin;
    }
  }
  //m_1E507:
  vw_4c = ddat.v1d3248;
  RG1L = signedlong(vw_4c);
  RG4L = unsignedlong(ddat.v1e102c);
  if (RG1L != RG4L)
  {
    bool skip00474 = false;
    RG1W = s350.v1e0576 & lcon(0x2000);
    RG1L = unsignedlong(RG1W);
    if (RG1L == 0)
      skip00474 = true;
    else
    {
      RG1L = DM2_IS_CREATURE_ALLOWED_ON_LEVEL(unsignedlong(s350.v1e054c), RG4L);
      if (RG1L != 0)
        //m_1E551:
        RG1L = DM2_1c9a_0648(signedlong(ddat.v1e102c));
      else
        skip00474 = true;
    }

    if (skip00474)
    {
      //m_1E547:
      RG2L = 0;
      vl_10 = RG2L;
      jump L_prefin;
    }
  }
  //m_1E55E:
  put16(parw06, lcon(0x6));
  RG2L = signedlong(ddat.v1e1028);
  put16(parw05, RG2W);
  RG4L = signedlong(ddat.v1e102a);
  RG1Blo = vb_54 | CUTX8(vw_20);
  RG1L = unsignedlong(RG1Blo);
  RG3L = RG4L;
  RG1L = DM2_CREATURE_GO_THERE(RG1L, RG4L, RG2L, RG3L, parw05, parw06);
  vl_10 = RG1L;
  RG1L = signedlong(vw_4c);
  DM2_1c9a_0648(RG1L);

L_prefin:
  if (vw_30 == 0)
    jump L_fin;
  if (vl_10 == 0)
  {
    s350.v1e056f = lcon(0xfffffffd);
    jump L_fin;
  }
  //m_1E6E4:
  if (vl_0c != 0)
  {
    s350.v1e056f = lcon(0xfffffffe);
    jump L_fin;
  }
  //m_1E6F6:
  if (vl_14 != 0)
  {
    put16(parw14, lcon(0xffffffff));
    put16(parw13, argw0);
    RG3L = signedlong(RG5W);
    RG2L = signedlong(vw_24);
    RG1L = DM2_19f0_0891(lcon(0x82), signedlong(vw_28), RG2L, RG3L, parw13, parw14);
    if (RG1L != 0)
      jump L_fin;
  }
  //m_1E726:
  RG1W = s350.v1e057a & lcon(0x20);
  RG1L = unsignedlong(RG1W);
  if (RG1L != 0)
  {
    RG4L = 0;
    vo_48.poke16(RG4W);
    //m_1E7B2:
    for (;;)
    {
      RG1L = unsignedlong(vo_48.peek16());
      if (RG1L > lcon(0x3))
        break;
      //m_1E743:
      RG4L = signedlong(vw_44);
      if (RG1L != RG4L)
      {
        RG4W = vw_28 + table1d27fc[RG1L]; // adj
        RG2W = vw_24; // adj
        RG3W = table1d2804[RG1L];
        RG1W = RG2W + RG3W; // adj
        RG2L = signedlong(RG1W);
        RG3L = signedlong(RG4W);
        RG1L = (dm2_GET_TILE_VALUE(RG3L, RG2L) & lcon(0xff)) >> 5 & lcon(0xff);
        if (RG1L == lcon(0x4))
        {
          RG1L = signedlong(vo_48.peek16());
          put16(parw16, RG1W);
          put16(parw15, RG2W);
          RG2L = signedlong(vw_24);
          RG1L = DM2_19f0_0d10(lcon(0x82), signedlong(vw_28), RG2L, RG3L, parw15, parw16);
          if (RG1L != 0)
            jump L_fin;
        }
      }
      //m_1E7AE:
      vo_48.inc16();
    }
  }
  //m_1E7BD:
  if (vb_54 != lcon(0x6))
  {
    //m_1E7E2:
    RG1L = signedlong(vw_38);
    if (RG1L == lcon(0xffffffff))
    {
      RG1L = signedlong(DM2_ABS(vw_28 - RG5W));
      RG4L = signedlong(RG1W);
      RG1L = signedlong(DM2_ABS(vw_24 - argw0)) + RG4L;
      if (RG1L > lcon(0x1))
        //m_1E817:
        vw_38 = 1;
      else
        vw_38 = 2;
    }
  }
  else
  {
    RG1W = vw_44 + 2;
    vw_44 = RG1W;
    RG4L = RG1L;
    RG4Bhi ^= RG1Bhi;
    RG4Blo &= 0x3;
    vw_44 = RG4W;
    vw_38 = 9;
  }
  //m_1E81D:
  RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG1W = word_at(RG1P, lcon(0xe));
  RG1L <<= bcon(0x6);
  RG1UW >>= 14;
  vo_48.poke16(RG1W);
  RG1L = unsignedlong(vo_48.peek16());
  RG4L = signedlong(vw_44);
  if (RG1L == RG4L)
    skip00478 = true;
  else
  {
    bool skip00476 = false;
    RG4L += 2;
    RG4L &= 0x3;
    if (RG1L == RG4L)
      skip00476 = true;
    else
    {
      RG1L = signedlong(vw_38);
      if (RG1L != 2)
        skip00476 = true;
    }

    if (skip00476)
    {
      bool skip00477 = false;
      //m_1E856:
      RG4Bhi = vb_54;
      if (RG4Bhi != lcon(0x4) && RG4Bhi != lcon(0x5))
        skip00477 = true;
      else
      {
        RG1L = signedlong(s350.v1e0584);
        if ((table1d607e[RG1L].uc[1] & bcon(0x1)) != 0)
          skip00477 = true;
        else
        {
          RG1L = signedlong(DM2_RANDDIR());
          if (RG1W == 0)
            skip00477 = true;
          else
            vw_38 = 9;
        }
      }

      if (skip00477)
      {
        //m_1E88A:
        DM2_19f0_0559(vw_44);
        jump L_fin;
      }
    }
    else
      skip00478 = true;
  }

  if (skip00478)
  {
    bool skip00479 = false;
    //m_1E89A:
    RG1L = unsignedlong(vo_48.peek16());
    RG4L = signedlong(vw_44);
    if (RG1L == RG4L)
      skip00479 = true;
    else
    {
      RG2L = signedlong(vw_38);
      if (RG2L == lcon(0x9))
        skip00479 = true;
      else
      {
        RG1L++;
        RG1L &= 0x3;
        if (RG1L != RG4L)
          //m_1E8C5:
          vw_38 = 3;
        else
          vw_38 = 4;
      }
    }

    if (skip00479)
    {
      //m_1E8CD:
      RG1L = signedlong(s350.v1e056e);
      RG1Blo = table1d613a[RG1L] & 2;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L != 0)
      {
        if (vb_54 != lcon(0x6))
        {
          if (vl_08 == 0)
          {
            RG3L = 0;
            vw_38 = RG3W;
          }
        }
      }
    }
  }

  //m_1E8F6:
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG5L &= lcon(0x1f);
  and8(location(RG1P + lcon(0x18)), lcon(0xffffffe0));
  or16(location(RG1P + lcon(0x18)), RG5W);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4W = argw0;
  RG4W &= lcon(0x1f);
  RG3W = word_at(RG1P, lcon(0x18));
  RG3L &= lcon(0xfffffc1f);
  RG4L <<= bcon(0x5);
  RG5L = RG3L | RG4L;
  mov16(location(RG1P + lcon(0x18)), RG5W);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4W = ddat.v1d3248 & lcon(0x3f);
  and8(location(RG1P + lcon(0x19)), lcon(0x3));
  RG4L <<= bcon(0xa);
  or16(location(RG1P + lcon(0x18)), RG4W);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4Blo = mkb(argw1);
  mov8(location(RG1P + lcon(0x1b)), RG4Blo);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4Blo = CUTX8(vw_44);
  mov8(location(RG1P + lcon(0x1d)), RG4Blo);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4Blo = vb_54;
  mov8(location(RG1P + lcon(0x20)), RG4Blo);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4Blo = CUTX8(vw_38);
  mov8(location(RG1P + lcon(0x1a)), RG4Blo);
  s350.v1e056f = lcon(0xfffffffc);

L_fin:
  if (vl_08 != 0)
    DM2_1c9a_0648(signedlong(vw_2c));
  return vl_10;
}

// exclusive for DM2_19f0_2024()
union u_len10
{
  struct // TODO: this one is shorter!
  {
    i32 ldummy;
    i16 vw_04;
    i16 vw_08;
    i16 vw_0c;
  };
  i8 barr_00[0x10]; // TODO: oversized?
};

static i32 DM2_19f0_2024(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG4; RG4L = edxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG3;
  i16 RG51w;
  i32 RG52l;
  c_wlreg _RG6;
  i8* RG61p;
  u_len10 u21_00;
  bool skip00499 = false;
  bool skip00498 = false;

  RG6L = RG1L;
  u21_00.vw_0c = RG4W;
  u21_00.vw_04 = RG2W;
  RG1W = s350.v1e057c & lcon(0x10);
  RG1L = unsignedlong(RG1W);
  if (RG1L == 0)
    skip00498 = true;
  else
  {
    RG4L = unsignedlong(RG6W);
    RG1L = DM2_IS_CONTAINER_CHEST(RG4W) ? 1 : 0;
    if (RG1L == 0)
      skip00498 = true;
    else
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
      RG4L >>= 14;
      u21_00.vw_08 = RG4W;
      RG6W = word_at(RG1P, 2);
    }
  }

  if (skip00498)
  {
    //m_1E9DD:
    RG1L = RG6L;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    if (RG1L != lcon(0x4))
      return lcon(0xffffffff);
    RG1W = s350.v1e057c & lcon(0x28);
    RG1L = unsignedlong(RG1W);
    if (RG1L == 0)
      return lcon(0xffffffff);
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG6UW));
    RG61p = RG1P;
    RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG1P, lcon(0x4)))));
    RG3P = RG1P;
    RG2W = word_at(RG1P) & lcon(0x1);
    if (RG2W == 0)
      //m_1EA3E:
      RG1W = s350.v1e057c & lcon(0x20);
    else
      RG1W = s350.v1e057c & lcon(0x8);
    //m_1EA48:
    RG1L = unsignedlong(RG1W);
    if (RG1L == 0)
      return lcon(0xffffffff);
    u21_00.vw_08 = 8;
    RG6W = word_at(RG61p, 2);
    if (RG2W == 0)
      skip00499 = true;
    else
    {
      RG4L = unsignedlong(u21_00.vw_04);
      RG1L = DM2_query_48ae_01af(signedlong(word_at(RG3P, lcon(0x1e))), RG4L);
      RG2L = unsignedlong(RG1Blo);
    }
  }
  else
    skip00499 = true;

  if (skip00499)
    //m_1EA7B:
    RG2L = lcon(0xf);

  //m_1EA80:
  RG51w = 0;
  //m_1EA8F:
  for (;;)
  {
    RG3L = signedlong(RG51w);
    if (RG3L >= lcon(0x4))
      break;
    //m_1EA84:
    RG1Blo = RG2Blo & lcon(0x1);
    u21_00.barr_00[RG3L] = RG1Blo;
    RG2W >>= bcon(0x1);
    RG51w++;
  }
  //m_1EA97:
  for (;;)
  {
    if (RG6W == lcon(0xfffffffe))
      return lcon(0xffffffff);
    RG3L = unsignedlong(RG6W);
    RG1L = RG3L >> bcon(0xe);
    if (u21_00.barr_00[RG1L] != 0)
    {
      bool skip00501 = false;
      RG52l = signedlong(u21_00.vw_0c);
      RG1L = DM2_CREATURE_CAN_HANDLE_IT(RG3L, RG52l);
      if (RG1L != 0)
        skip00501 = true;
      else
      {
        RG2L = signedlong(u21_00.vw_04);
        RG4L = RG52l;
        RG1L = DM2_19f0_2024(RG3L, RG4L, RG2L);
        if (RG1W != lcon(0xffffffff))
          skip00501 = true;
      }

      if (skip00501)
      {
        //m_1EADB:
        if (u21_00.vw_08 == lcon(0x8))
        {
          RG4L = unsignedlong(RG6W) >> bcon(0xe);
          RG1L = signedlong(u21_00.vw_08) + RG4L;
          u21_00.vw_08 = RG1W;
        }
        return signedlong(u21_00.vw_08);
      }
    }
    //m_1EAFF:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG6UW)));
    RG6L = RG1L;
  }
  FOREND
}

// stacksize was 0x28
i32 DM2_19f0_2165(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i16 argw1, i16 argw2)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG51w;
  c_wlreg _RG52;
  c_wlreg _RG53;
  i16 RG54w;
  c_wlreg _RG6; // TODO: probably some more
  i32 RG7l; // TODO: bool, if RG1 is bool
  i32 vl_00;
  i32 vl_04;
  c_o32 vo_08; // poke 16 peek 8 16 32 inc16
  i16 vw_0c;
  i16 vw_10;
  i8 vb_14; // and8(0x14, lcon(0x7f));

  vb_14 = RG1Blo;
  RG51w = RG3W;
  RG6L = argl0;
  RG1Blo &= lcon(0xffffff80);
  RG1W = unsignedword(RG1Blo);
  vw_0c = RG1W; // truncated
  if (RG1W != 0)
  {
    vb_14 &= lcon(0x7f);
    vw_10 = -1;
  }
  if (s350.v1e057c == 0)
    jump L_fin;
  if (RG51w != wcon(0xffff))
  {
    //m_1EB75:
    if (RG4W != RG51w || RG2W != RG6W)
      //m_1EB86:
      RG1L = 0;
    else
      RG1L = 1;
    //m_1EB88:
    RG7l = RG1L;
    if (RG1L == 0)
    {
      if (argw1 == lcon(0xffffffff))
      {
        RG3L = signedlong(RG6W);
        vl_00 = signedlong(RG51w);
        RG1L = signedlong(RG2W);
        vl_04 = RG1L;
        RG1L = signedlong(RG4W);
        RG2L = vl_00;
        RG1L = signedlong(DM2_CALC_VECTOR_DIR(RG1W, CUTX16(vl_04), RG2W, RG3W));
        argw1 = RG1W;
      }
    }
  }
  else
  {
    RG7l = 0;
    RG1L = signedlong(argw1);
    RG51w = table1d27fc[RG1L] + RG4W;
    RG6W = table1d2804[RG1L];
    RG6L += RG2L;
  }
  //m_1EBBA:
  RG4L = signedlong(RG6W);
  DM2_19f0_045a(signedlong(RG51w), RG4L);
  if (vb_14 == 0 && (ddat.v1e08ae & wcon(0x10)) == 0)
    jump L_fin;
  RG1L = unsignedlong(ddat.v1e08ae) >> 5;
  if (RG1Blo != 0)
  {
    //m_1ED7A:
    RG1L = signedlong(ddat.v1e08be);
    if (RG1L == lcon(0xffffffff))
    {
      RG2Blo = 0;
      ddat.v1e08be = RG2Blo;
      ddat.v1e08c0[0] = lcon(0xffffffff);
    }
    if (RG7l == 0)
    {
      RG1W = ddat.v1d3248;
      if (RG1W == ddat.v1e08d6 && RG51w == ddat.v1e08d8 && RG6W == ddat.v1e08d4)
        jump L_fin;
    }
    //m_1EDBF:
    if (RG7l == 0)
      //m_1EDCA:
      RG1L = lcon(0x2a);
    else
      RG1L = 1;
    //m_1EDCF:
    RG1L = unsignedlong(RG1W);
    RG4L = unsignedlong(s350.v1e057c);
    if ((RG1L & RG4L) == 0)
      jump L_fin;
    if (vb_14 != lcon(0x1))
    {
      //m_1EE00:
      RG1L = DM2_19f0_04bf();
      //m_1EE05:
      for (;;)
      {
        bool skip00505 = false;
        bool skip00506 = false;
        RG54w = RG1W;
        if (RG54w == lcon(0xfffffffe))
          jump L_fin;
        RG3L = unsignedlong(RG54w);
        RG1L = RG3L >> bcon(0xe);
        vo_08.poke16(RG1W);
        RG1W = RG54w;
        RG1W &= lcon(0x3c00);
        RG1L = unsignedlong(RG1W) >> bcon(0xa);
        RG6L = signedlong(RG1W);
        if (RG6L < lcon(0x4) || RG6L >= lcon(0xe))
          skip00506 = true;
        else
        {
          bool skip00504 = false;
          if (RG6L == lcon(0x4))
            skip00504 = true;
          else
          {
            RG4L = signedlong(argw2);
            RG1L = DM2_CREATURE_CAN_HANDLE_IT(RG3L, RG4L);
            if (RG1L == 0)
              skip00504 = true;
          }

          if (skip00504)
          {
            //m_1EE5B:
            RG2L = signedlong(argw1);
            RG4L = signedlong(argw2);
            RG1L = DM2_19f0_2024(unsignedlong(RG54w), RG4L, RG2L);
            vo_08.poke16(RG1W);
            if (RG1W == lcon(0xffffffff))
              skip00506 = true;
            else
              skip00505 = true;
          }
          else
            skip00505 = true;
        }

        if (skip00505)
        {
          //m_1EE82:
          RG1L = unsignedlong(vo_08.peek16());
          if (RG1L >= lcon(0x8))
          {
            vw_10 = 24;
            RG6L = RG1L & lcon(0x3);
            vo_08.poke16(RG6W);
            break;
          }
          //m_1EEA4:
          RG1L = signedlong(ddat.v1e08c0[0]);
          if (RG1L == lcon(0xffffffff))
          {
            if (RG7l == 0)
            {
              //m_1EECB:
              RG4Blo = lcon(0xc);
              RG2W = argw1;
            }
            else
            {
              RG3W = argw1;
              if (RG3W != lcon(0xffffffff))
              {
                //m_1EEC5:
                RG4Blo = 3;
                RG2L = RG3L;
              }
              else
              {
                RG4Blo = lcon(0xf);
                RG2L = 0;
              }
            }
            //m_1EED1:
            RG1L = 0;
            //m_1EEEC:
            for (;;)
            {
              RG3L = signedlong(RG1W);
              if (RG3L >= lcon(0x4))
                break;
              //m_1EED5:
              RG4Bhi = RG4Blo & lcon(0x1);
              RG3L = signedlong(RG2W);
              ddat.v1e08c0[RG3L] = RG4Bhi;
              RG2L++;
              RG2W &= 0x3;
              RG4UBlo >>= bcon(0x1);
              RG1L++;
            }
          }
          //m_1EEF4:
          RG1L = unsignedlong(vo_08.peek16());
          if (ddat.v1e08c0[RG1L] != 0)
            break;
          ddat.v1e08be = 1;
          skip00506 = true;
        }

        if (skip00506)
        {
          //m_1EF0B:
          RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(RG54w))));
        }
      }
    }
    else
    {
      if (RG7l == 0)
        vw_10 = 23;
    }
  }
  else
  {
    RG1L = signedlong(ddat.v1e08be);
    if (RG1L == lcon(0xffffffff))
    {
      RG3W = unsignedword(RG3Blo);
      ddat.v1e08be = RG3Bhi;
      ddat.v1e08bf = RG3Bhi;
      if (RG7l == 0)
      {
        RG1W = s350.v1e057c & lcon(0x4);
        RG1L = unsignedlong(RG1W);
        if (RG1L != 0)
        {
          RG1Blo = mkb(argw1) + bcon(0x2) & bcon(0x3);
          ddat.v1e08c0[0] = RG1Blo;
          if (ddat.v1e08b0 == lcon(0xffffffff))
          {
            RG4L = signedlong(RG6W);
            RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(RG51w, RG4W));
            ddat.v1e08b0 = RG1W;
          }
          RG53W = ddat.v1e08b0;
          //m_1EC57:
          for (;;)
          {
            RG1Blo = RG53W != wcon(0xfffe) ? 1 : 0;
            RG1L = unsignedlong(RG1Blo);
            if (RG1L != 0)
            {
              RG1L = RG53L;
              RG1W &= lcon(0x3c00);
              RG1L = unsignedlong(RG1W) >> bcon(0xa);
              RG1L = signedlong(RG1W);
              if (RG1L <= lcon(0x3))
              {
                RG3L = unsignedlong(RG53W);
                RG1L = unsignedlong(ddat.v1e08b0);
                RG4L = RG3L;
                RG1L = DM2_IS_WALL_ORNATE_ALCOVE_FROM_RECORD(RG1L, RG4L);
                if (RG1L != 0)
                {
                  RG3L >>= 14;
                  RG1L = signedlong(ddat.v1e08c0[0]);
                  if (RG3L != RG1L)
                    //m_1ECAA:
                    ddat.v1e08be = 1;
                  else
                    ddat.v1e08bf = 1;
                }
                //m_1ECB1:
                RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG53UW)));
                RG53L = RG1L;
                continue;
              }
            }
            break;
          }
          //m_1ECBF:
          ddat.v1e08b2 = RG53W;
        }
      }
    }
    //m_1ECC6:
    if (ddat.v1e08bf == 0)
      jump L_fin;
    if (vb_14 != lcon(0x1))
    {
      //m_1ECE6:
      vw_10 = 26;
      vo_08.poke16(signedword(ddat.v1e08c0[0]));
      RG52W = ddat.v1e08b2;
      //m_1ED01:
      for (;;)
      {
        if (RG52W == lcon(0xfffffffe))
          jump L_fin;
        RG3L = unsignedlong(RG52W);
        RG1L = RG3L >> bcon(0xe);
        RG4L = unsignedlong(vo_08.peek16());
        if (RG1L == RG4L)
        {
          RG1L = RG52L;
          RG1W &= lcon(0x3c00);
          RG1L = unsignedlong(RG1W) >> bcon(0xa);
          RG1L = signedlong(RG1W);
          if (RG1L != lcon(0x4) && RG1L < lcon(0xe))
          {
            RG6L = signedlong(argw2);
            RG4L = RG6L;
            RG1L = DM2_CREATURE_CAN_HANDLE_IT(RG3L, RG4L);
            if (RG1L != 0)
              break;
            RG2L = signedlong(argw1);
            RG4L = RG6L;
            RG1L = DM2_19f0_2024(RG3L, RG4L, RG2L);
            if (RG1W != lcon(0xffffffff))
              break;
          }
        }
        //m_1ED6C:
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG52UW)));
        RG52L = RG1L;
      }
    }
    else
      vw_10 = 25;
  }
  //m_1EF30:
  if (vw_0c == 0)
    return lcon(0x1);
  if (argw1 == lcon(0xffffffff))
  {
    RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
    RG1W = word_at(RG1P, lcon(0xe));
    RG1L <<= bcon(0x6);
    RG1UW >>= 14;
    argw1 = RG1W;
  }
  if (RG7l == 0)
  {
    RG1L = DM2_19f0_0559(argw1);
    if (RG1L != 0)
      return lcon(0x1);
  }
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4Blo = mkb(argw1);
  mov8(location(RG1P + lcon(0x1d)), RG4Blo);
  if (vb_14 != lcon(0x1))
  {
    //m_1F0DD:
    RG1L = signedlong(vw_10);
    if (RG1L == lcon(0xffffffff))
    {
      RG1P = DOWNCAST(c_creature, s350.creatures);
      RG1Blo = byte_at(RG1P, lcon(0x1d));
      RG1W = unsignedword(RG1Blo);
      RG4W = vo_08.peek16() - RG1W;
      RG1L = RG4L + 1;
      RG1W &= 0x3;
      if (RG7l != 0)
      {
        bool skip00508 = false;
        //m_1F126:
        RG4L = signedlong(RG1W);
        RG4Blo = RG4L >= 2 ? 1 : 0;
        RG4W = unsignedword(RG4Blo);
        RG4L += lcon(0xc);
        vw_10 = RG4W;
        if (RG1W != 0)
        {
          //m_1F14A:
          RG1L = signedlong(RG1W);
          if (RG1L != lcon(0x3))
            skip00508 = true;
          else
          {
            RG1P = DOWNCAST(c_creature, s350.creatures);
            RG4Blo = byte_at(RG1P, lcon(0x1d)) + 1;
          }
        }
        else
        {
          RG1P = DOWNCAST(c_creature, s350.creatures);
          RG4Blo = byte_at(RG1P, lcon(0x1d)) - 1;
        }

        if (!skip00508)
        {
          //m_1F15A:
          RG4Blo &= 0x3;
          mov8(location(RG1P + lcon(0x1d)), RG4Blo);
        }

        //m_1F160:
        RG1L = signedlong(s350.v1e0584);
        if ((table1d607e[RG1L].uc[1] & bcon(0x10)) != 0)
        {
          RG1P = DOWNCAST(c_creature, s350.creatures);
          RG2W = unsignedword(RG2Blo);
          RG2Blo = byte_at(RG1P, lcon(0x1d));
          RG4P = DOWNCAST(SPX_Creature, s350.v1e054e);
          RG4W = word_at(RG4P, lcon(0xe));
          RG4L <<= bcon(0x6);
          RG4UW >>= 14;
          if (RG2W != RG4W)
          {
            RG1Blo = RG2Blo;
            RG1L = unsignedlong(RG1Blo);
            DM2_19f0_0559(RG1W);
            return lcon(0x1);
          }
        }
      }
      else
      {
        RG1L = signedlong(RG1W);
        if (RG1L >= 2)
          //m_1F11A:
          vw_10 = lcon(0x2c);
        else
          vw_10 = lcon(0x2b);
      }
    }
  }
  else
  {
    RG1L = signedlong(vw_10);
    if (RG1L != lcon(0x19))
    {
      //m_1EFA2:
      if (RG1L != lcon(0x17))
      {
        //m_1F010:
        RG1L = DM2_RANDBIT() ? 1 : 0;
        RG4L = RG1L;
        if (RG7l == 0)
          RG1L += 2;
        RG4W = argw1 + RG1W;
        RG4W &= 0x3;
        vo_08.poke16(RG4W);
        RG1L = signedlong(RG1W);
        RG1W = table1d6299[RG1L];
        vw_10 = RG1W;
      }
      else
      {
        RG4L = unsignedlong(argw1);
        RG1L = DM2_query_48ae_01af(signedlong(CUTX16(DM2_query_0cee_2df4(DM2_19f0_050f() & lcon(0xffff)))), RG4L);
        RG1W = unsignedword(RG1Blo);
        if (RG1W == 0)
          jump L_fin;
        RG2L = signedlong(RG1W);
        RG4W = CUTX16(DM2_RAND16(CUTX16(DM2_1c9a_0598(RG2L))) + 1);
        RG1L = DM2_COMPUTE_POWER_4_WITHIN(RG2W, RG4W);
        vo_08.poke16(wcon(0x0));
        //m_1EFF4:
        for (;;)
        {
          RG4L = RG1L;
          RG4Bhi ^= RG1Bhi;
          RG4Blo &= 0x1;
          RG4L = signedlong(RG4W);
          if (RG4L != 0)
            break;
          RG1W >>= bcon(0x1);
          vo_08.inc16();
        }
      }
    }
    else
    {
      RG1W = argw1;
      vo_08.poke16(RG1W);
    }
  }
  //m_1F03E:
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4W = ddat.v1e08a8 & lcon(0x1f);
  and8(location(RG1P + lcon(0x18)), lcon(0xffffffe0));
  or16(location(RG1P + lcon(0x18)), RG4W);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4W = ddat.v1e08aa & lcon(0x1f);
  RG3W = word_at(RG1P, lcon(0x18));
  RG3L &= lcon(0xfffffc1f);
  RG4L <<= bcon(0x5);
  mov16(location(RG1P + lcon(0x18)), RG3W | RG4W);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4W = ddat.v1e08ac & lcon(0x3f);
  and8(location(RG1P + lcon(0x19)), lcon(0x3));
  RG4L <<= bcon(0xa);
  or16(location(RG1P + lcon(0x18)), RG4W);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4Blo = vo_08.peek8();
  mov8(location(RG1P + lcon(0x1c)), RG4Blo);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4Blo = mkb(argw2);
  mov8(location(RG1P + lcon(0x1e)), RG4Blo);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4Blo = vb_14;
  mov8(location(RG1P + lcon(0x20)), RG4Blo);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4Blo = CUTX8(vw_10);
  mov8(location(RG1P + lcon(0x1a)), RG4Blo);
  s350.v1e056f = lcon(0xfffffffc);
  return lcon(0x1);

L_fin:
  if (vw_0c != 0)
    s350.v1e056f = lcon(0xfffffffd);
  return 0;
}

// belongs to DM2_19f0_2813
static i32 DM2_19f0_266c(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  i16 RG6w;
  i32 vl_00;
  i16 vw_04;
  bool flag;

  RG5L = RG1L;
  vw_04 = RG4W;
  RG6w = RG2W;
  vl_00 = lcon(0xffff);
  //m_1F1C0:
  for (;;)
  {
    RG1Blo = RG5W != wcon(0xfffe) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == 0)
      return vl_00;
    RG1Blo = RG5W != wcon(0xffff) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == 0)
      return vl_00;
    RG1L = RG5L;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    RG1L = signedlong(RG1W);
    if (RG1L != lcon(0x3))
    {
      //m_1F259:
      if (RG1L > lcon(0x3))
        return vl_00;
    }
    else
    {
      RG2L = unsignedlong(RG5W);
      RG4L = RG2L >> bcon(0xe);
      RG1L = signedlong(vw_04);
      if (RG4L == RG1L)
      {
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
        RG4W = word_at(RG1P, 2) & lcon(0x7f);
        if (RG4W != 0)
        {
          RG4L = signedlong(RG4W);
          if (RG4L != lcon(0x26))
          {
            vl_00 = RG5L;
            if (RG4L == lcon(0x1a))
            {
              if (jz_test8(location(RG1P + lcon(0x4)), lcon(0x4)))
                //m_1F253:
                flag = RG6w != wcon(0x1);
              else
                flag = RG6w != wcon(0x2);
              //m_1F241:
              if (!flag)
              {
                RG4L = signedlong(RG3W);
                RG1L = DM2_CREATURE_CAN_HANDLE_IT(RG2L, RG4L);
                if (RG1L != 0)
                  return vl_00;
              }
            }
          }
        }
      }
    }
    //m_1F25B:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG5UW)));
    RG5L = RG1L;
  }
  FOREND
}

// belongs to DM2_19f0_2813
static i32 DM2_19f0_2723(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  i16 RG6w;
  i16 vw_00;
  bool flag;
  bool skip00511 = false;

  RG6w = RG1W;
  RG5w = RG4W;
  vw_00 = RG2W;
  RG2L = 0;
  if (RG1W == lcon(0xfffffffe) || RG1W == lcon(0xffffffff))
    return RG2L;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(RG6w)));
  RG4P = RG1P;
  RG1W = word_at(RG1P, 2) & lcon(0x7f);
  if (RG1UW < mkuw(lcon(0x17)))
  {
    //m_1F2DF:
    if (RG1UW < mkuw(2))
    {
      //m_1F2F2:
      if (RG1W != lcon(0x1))
        return RG2L;
      return RG5w != 0 ? RG2L : lcon(0x1);
    }
    if (RG1UW <= mkuw(2))
    {
      //m_1F30F:
      if (RG5w != 0)
        return RG2L;
      return vw_00 != wcon(0xffffffff) ? lcon(0x1) : RG2L;
    }
    if (RG1W != lcon(0x3))
      return RG2L;
    //m_1F321:
    flag = RG5w == wcon(0x0);
    skip00511 = true;
  }
  else
  {
    bool skip00510 = false;
    if (RG1UW <= mkuw(lcon(0x17)))
    {
      //m_1F328:
      if (RG5w != 0)
        return RG2L;
      RG1Blo = jz_test8(location(RG4P + lcon(0x4)), lcon(0x4)) ? 1 : 0;
      RG4L = RG1L & lcon(0xff);
      RG1L = signedlong(RG3W);
      return RG1L == RG4L ? RG2L : lcon(0x1);
    }
    if (RG1UW < mkuw(lcon(0x1a)))
    {
      //m_1F2D9:
      flag = RG1W == wcon(0x18);
      skip00510 = true;
    }
    else
    {
      if (RG1UW <= mkuw(lcon(0x1a)))
      {
        //m_1F353:
        if (jz_test8(location(RG4P + lcon(0x4)), lcon(0x4)))
        {
          //m_1F35F:
          if (RG5w != lcon(0x1))
            return RG2L;
        }
        else
        {
          flag = RG5w == wcon(0x2);
          skip00511 = true;
        }
      }
      else
      {
        if (RG1UW <= mkuw(lcon(0x1b)))
        {
          //m_1F343:
          if (RG5w != lcon(0x1))
            return RG2L;
          if (jz_test16(location(RG4P + 2), lcon(0xffffff80)))
            return RG2L;
        }
        else
        {
          flag = RG1W == wcon(0x4a);
          skip00510 = true;
        }
      }
    }

    if (skip00510)
    {
      //m_1F2D2:
      if (!flag)
        return RG2L;
      return RG5w != 0 ? RG2L : lcon(0x1);
    }
  }

  if (skip00511)
  {
    //m_1F324:
    if (!flag)
      return RG2L;
  }

  //m_1F365:
  RG4L = signedlong(vw_00);
  return DM2_CREATURE_CAN_HANDLE_IT(unsignedlong(RG6w), RG4L);
}

// stacksize was 0x1c
bool DM2_19f0_2813(i32 eaxl, i32 edxl, i32 ebxl, i16 x, i16 y, i16 argw1, i16 argw2)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p;
  c_wlreg _RG61;
  c_wlreg _RG62;
  c_wlreg _RG65;
  i32 RG63l;
  i32 RG64l;
  c_wlreg _RG71;
  c_lreg _RG72;
  i16 vw_00;
  i16 vw_04;
  i8 vb_08; // and8
  bool skip00513 = false;

  vb_08 = RG1Blo;
  vw_04 = x;
  RG1Blo &= lcon(0xffffff80);
  RG1W = unsignedword(RG1Blo);
  vw_00 = RG1W; // truncated
  if (RG1W != 0)
    vb_08 &= lcon(0x7f);
  RG1W = s350.v1e057e & lcon(0x1);
  RG1L = unsignedlong(RG1W);
  if (RG1L == 0)
    jump L_fin;
  RG65L = unsignedlong(vw_04);
  if (RG65W != lcon(0xffffffff))
  {
    //m_1F3E6:
    if (RG4W == RG65W && RG2W == y)
      jump L_fin;
  }
  else
  {
    RG1L = signedlong(argw1);
    RG3W = table1d27fc[RG1L];
    RG3L += RG4L;
    vw_04 = RG3W;
    RG1W = table1d2804[RG1L];
    RG1L += RG2L;
    y = RG1W;
  }
  //m_1F3F6:
  RG61L = unsignedlong(vw_04);
  if (RG61W >= 0 && RG61W < mapdat.map_width)
    //m_1F40F:
    RG1L = 0;
  else
    RG1L = 1;
  //m_1F411:
  if (RG1L != 0)
    jump L_fin;
  if (y < 0 || y >= mapdat.map_height)
    RG1L = 1;
  if (RG1L != 0)
    jump L_fin;
  RG3L = signedlong(RG4W);
  RG1L = signedlong(vw_04);
  if (RG3L == RG1L)
    skip00513 = true;
  else
  {
    RG3L = signedlong(RG2W);
    RG1L = signedlong(y);
    if (RG3L == RG1L)
      skip00513 = true;
    else
      RG1L = 1;
  }

  if (skip00513)
    //m_1F45B:
    RG1L = 0;
  //m_1F45D:
  if (RG1L != 0)
    jump L_fin;
  if (argw1 == lcon(0xffffffff))
  {
    RG3L = signedlong(y);
    RG1L = signedlong(vw_04);
    RG64l = signedlong(RG2W);
    RG71L = signedlong(RG4W);
    RG2L = RG1L;
    RG4L = RG64l;
    RG1L = signedlong(DM2_CALC_VECTOR_DIR(RG71W, RG4W, RG2W, RG3W));
    argw1 = RG1W;
  }
  RG4L = signedlong(y);
  DM2_19f0_045a(signedlong(vw_04), RG4L);
  if ((ddat.v1e08ae & wcon(0x10)) == 0)
    jump L_fin;
  if (ddat.v1e08b0 == lcon(0xffffffff))
  {
    RG4L = signedlong(ddat.v1e08aa);
    RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(ddat.v1e08a8, RG4W));
    ddat.v1e08b0 = RG1W;
  }
  RG4W = ddat.v1e08b0;
  RG62L = 0;
  //m_1F4E0:
  for (;;)
  {
    if (RG4W != lcon(0xfffffffe))
    {
      RG1L = RG4L;
      RG1Blo ^= RG4Blo;
      RG1Bhi &= lcon(0x3c);
      RG1L = unsignedlong(RG1W) >> bcon(0xa);
      RG1L = signedlong(RG1W);
      if (RG1L <= lcon(0x3))
      {
        bool skip00515 = false;
        if (RG1L != lcon(0x3))
          skip00515 = true;
        else
        {
          RG2L = signedlong(argw1) + 2 & RG1L;
          RG1L = unsignedlong(RG4W);
          RG3L = RG1L >> bcon(0xe);
          if (RG3L != RG2L)
            skip00515 = true;
          else
          {
            RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
            RG5p = RG1P;
            RG1W = word_at(RG1P, 2) & lcon(0x7f);
            RG1Blo = RG1W == wcon(0x26) ? 1 : 0;
            RG62L = RG1L;
            if (RG1W == 0)
            {
              RG1L = signedlong(RG1W);
              if (RG1L == lcon(0x4a) && jz_test8(location(RG5p + lcon(0x4)), lcon(0x1)))
                break;
              skip00515 = true;
            }
          }
        }

        if (skip00515)
        {
          //m_1F544:
          RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
          RG4L = RG1L;
          continue;
        }
      }
    }

    //m_1F552:
    if (RG62W == 0)
      jump L_fin;
    RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
    RG4W = unsignedword(RG4Blo);
    RG4Blo = byte_at(RG1P, lcon(0x4));
    RG1W = word_at(RG5p, 2);
    RG1UW >>= bcon(0x7);
    if (RG1W != RG4W || jz_test8(location(RG5p + lcon(0x4)), lcon(0x4)))
      jump L_fin;
    break;
  }
  //m_1F580:
  RG63l = signedlong(argw2);
  RG72L = unsignedlong(vb_08);
  RG1W = argw1 + wcon(0x2);
  RG1W &= 0x3;
  RG4L = signedlong(RG1W);
  RG1L = DM2_19f0_266c(unsignedlong(ddat.v1e08b0), RG4L, RG72L, RG63l);
  if (RG1W == lcon(0xffffffff))
    jump L_fin;
  RG4W = word_at(RG5p, lcon(0x4));
  RG4L <<= 11;
  RG4UW >>= 14;
  RG3L = signedlong(RG4W);
  RG1L = DM2_19f0_2723(unsignedlong(RG1W), RG72L, RG63l, RG3L);
  if (RG1L == 0)
    jump L_fin;
  if (vw_00 == 0)
    return true;
  RG1L = DM2_19f0_0559(argw1);
  if (RG1L != 0)
    return true;
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4L = unsignedlong(vw_04);
  RG4W &= lcon(0x1f);
  and8(location(RG1P + lcon(0x18)), lcon(0xffffffe0));
  or16(location(RG1P + lcon(0x18)), RG4W);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4W = y & lcon(0x1f);
  RG3W = word_at(RG1P, lcon(0x18));
  RG3L &= lcon(0xfffffc1f);
  RG4L <<= bcon(0x5);
  mov16(location(RG1P + lcon(0x18)), RG3W | RG4W);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4W = ddat.v1d3248 & lcon(0x3f);
  and8(location(RG1P + lcon(0x19)), lcon(0x3));
  RG4L <<= bcon(0xa);
  or16(location(RG1P + lcon(0x18)), RG4W);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4Blo = mkb(argw1);
  mov8(location(RG1P + lcon(0x1d)), RG4Blo);
  mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1b)), RG4Blo);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4Blo = mkb(argw2);
  mov8(location(RG1P + lcon(0x1e)), RG4Blo);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4Blo = vb_08;
  mov8(location(RG1P + lcon(0x20)), RG4Blo);
  if (RG4Blo != 0)
  {
    //m_1F68A:
    if (RG4Blo != lcon(0x1))
      //m_1F69A:
      mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1a)), lcon(0x31));
    else
      mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1a)), lcon(0x30));
  }
  else
    mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1a)), lcon(0x2f));
  //m_1F6A3:
  if (vb_08 == 0)
  {
    RG1W = word_at(RG5p, lcon(0x4)) & lcon(0x18);
    if (RG1UW < mkuw(lcon(0x10)))
    {
      s350.v1e056f = lcon(0xfffffffc);
      return true;
    }
  }
  s350.v1e056f = lcon(0xfffffffe);
  return true;

L_fin:
  if (vw_00 != 0)
    s350.v1e056f = lcon(0xfffffffd);
  return false;
}

// belongs to DM2_1c9a_0958
static void DM2_4DEA(i8 eaxb, i32 edxl, i16* ebxpw, i32* ecxpl)
{
  DM2_COPY_MEMORY(DOWNCAST(i32, ecxpl), DM2_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0xf), eaxb, lcon(0x7), lcon(0xfc)) + 4 * (DM2_query_4E26(ebxpw) + edxl & lcon(0xffff)), lcon(0x4));
}

// adress given to a functionpointer
i32 DM2_1BA1B(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 vw_00;

  RG2L = signedlong(RG4W);
  RG3L = signedlong(RG1W);
  RG4L = RG2L;
  RG1L = dm2_GET_TILE_VALUE(RG3L, RG4L);
  RG4W = unsignedword(RG4Blo);
  RG4Blo = RG1Blo;
  vw_00 = RG4W;
  RG1L = unsignedlong(RG1Blo) >> 5;
  RG4Blo = RG1Blo;
  RG1W = unsignedword(RG1Blo);
  RG4L = unsignedlong(RG4Blo);
  if (RG4L == lcon(0x4))
  {
    RG1Blo = CUTX8(vw_00) & lcon(0x7);
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == RG4L)
    {
      RG4L = RG2L;
      RG1L = DM2_GET_GRAPHICS_FOR_DOOR(DM2_IS_REBIRTH_ALTAR(UPCAST(unk, DOWNCAST(t_record, DM2_GET_ADDRESS_OF_TILE_RECORD(RG3W, RG4W)))) & lcon(0xff));
      if (RG1L == 0)
        return lcon(0x1);
    }
    return 0;
  }
  if (RG1W == 0)
    return lcon(0x1);
  RG1L = unsignedlong(RG1W);
  if (RG1L == lcon(0x7))
    return lcon(0x1);
  if (RG1L != lcon(0x6))
    return 0;
  RG1W = vw_00;
  RG1W &= 0x4;
  RG1L = unsignedlong(RG1W);
  return RG1L == 0 ? lcon(0x1) : 0;
}

void DM2_1c9a_0247(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 vw_00;

  RG1Bhi &= 0x3;
  RG3L = RG1L;
  RG1W = s350.v1e054c;
  RG1Bhi &= 0x3;
  RG4L = RG1L & lcon(0xffff) | lcon(0x20000000);
  RG1L = dm2_dballochandler.DM2_ALLOCATION11(signedlong(RG3W) | RG4L, 0, OO /*TAG dbidx*/ vw_00);
  if (RG1L != 0)
  {
    RG1L = signedlong(vw_00);
    dm2_dballochandler.DM2_dballoc_3e74_58a3((t_dbidx)RG1W);
  }
  RG1W = s350.v1e054c;
  RG1Bhi &= 0x3;
  RG4L = RG1L & lcon(0xffff) | lcon(0x30000000);
  RG1L = dm2_dballochandler.DM2_ALLOCATION11(signedlong(RG3W) | RG4L, 0, OO /*TAG dbidx*/ vw_00);
  if (RG1L != 0)
    dm2_dballochandler.DM2_dballoc_3e74_58a3((t_dbidx)vw_00);
}

i32 DM2_1c9a_0648(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG3;
  c_nreg _RG4;

  RG4L = RG1L;
  if (RG1W == ddat.v1d3248)
    return RG1L;
  RG1L = signedlong(RG1W);
  DM2_CHANGE_CURRENT_MAP_TO(RG1L);
  RG3W = ddat.v1e027c;
  if (RG4W != RG3W)
  {
    RG1W = ddat.v1e0258;
    ddat.v1e08da = RG1W;
    RG1W = ddat.v1e0270;
    ddat.v1e08d8 = RG1W;
    RG1W = ddat.v1e0272;
    ddat.v1e08d4 = RG1W;
    RG1W = ddat.v1e0266;
    ddat.v1e08d6 = RG1W;
  }
  else
  {
    RG1W = party.absdir;
    ddat.v1e08da = RG1W;
    RG1W = ddat.v1e0260;
    ddat.v1e08d8 = RG1W;
    RG1W = ddat.v1e0262;
    ddat.v1e08d4 = RG1W;
    ddat.v1e08d6 = RG3W;
  }
  return RG1L;
}

// plugin for DM2_OVERSEE_RECORD
static i32 DM2_1c9a_0694(i16* eaxpw, i16* edxpw)
{
  c_nreg _RG1; RG1P = DOWNCAST(i16, eaxpw);
  c_nreg _RG4; RG4P = DOWNCAST(i16, edxpw);

  if (word_at(RG4P) != lcon(0xfffffffe))
  {
    RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(word_at(RG1P));
    if (RG1W != word_at(RG4P))
      return 0;
  }
  return lcon(0x1);
}

// TODO: I did set v_00 as a single word.
// In fact the routine can be given as parameter for DM2_OVERSEE_RECORD and then
// a wordarray of 5 could be requested.
// Now we don't have 5 words anyways here.
// v_04 is a i8* then, whereby it is not initialized!
i16* DM2_1c9a_06bd(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 vw_00;
  i16* wp_04;

  vw_00 = RG4W; // adj TODO incoming RG4W word only
  RG4L = 0;
  if (RG1W != lcon(0xffffffff))
  {
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
    i32 parl02 = RG4L;
    i32 parl01 = RG4L;
    RG4L = unsignedlong(RG2Blo);
    RG1P += 2;
    RG1P16 = DM2_OVERSEE_RECORD(RG1P16, RG4L, UPCAST(i16*, ADRESSOF(i16*, &wp_04)), DM2_1c9a_0694, &vw_00, parl01, parl02);
    RG4P = RG1P;
    if (RG1P != NULL)
    {
      if (word_at(RG1P) == lcon(0xfffffffe))
        RG4P = NULL;
    }
  }
  return RG4P16;
}

// recursive
// belongs to DM2_PROCEED_XACT_71
i32 DM2_1c9a_078b(unk* xeaxp, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG51w;
  i16 RG52w;
  c_lreg _RG61;
  c_wlreg _RG62;
  c_lreg _RG63;
  i8* RG64p;
  i8* RG7p;
  i32 vl_00;
  unk* xp_04;
  i16 vw_08;
  c_ql32 vql_0c;
  c_ql32 vql_10;
  i8 vb_14;

  RG7p = RG1P;
  vw_08 = RG4W;
  vb_14 = RG2Blo;
  RG1W = word_at(RG1P);
  vql_10.poke32(RG1L);
  //m_20053:
  for (;;)
  {
    for (;;)
    {
      for (;;)
      {
        RG1L = vql_10.peek32();
        vql_0c.poke32(RG1L);
        if (RG1W == lcon(0xfffffffe))
          return RG1L;
        vql_10.poke32(record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_10.peek16())))));
        RG1L = vql_0c.peek32();
        RG1W &= lcon(0x3c00);
        RG1L = unsignedlong(RG1W) >> bcon(0xa);
        RG51w = RG1W;
        RG1L = signedlong(RG1W);
        if (RG1L <= lcon(0x4) || RG1L >= lcon(0xe))
        {
          RG1L = signedlong(RG51w);
          if (RG1L != lcon(0x9))
            continue;
        }
        break;
      }
      //m_2009B:
      RG1L = unsignedlong(vb_14);
      if (RG1L != lcon(0xff))
      {
        RG4L = unsignedlong(vql_0c.peek16()) >> bcon(0xe);
        if (RG1L != RG4L)
          continue;
      }
      break;
    }
    //m_200B6:
    RG4L = signedlong(vw_08);
    RG61L = unsignedlong(vql_0c.peek16());
    RG1L = DM2_CREATURE_CAN_HANDLE_IT(RG61L, RG4L);
    RG4L = RG1L;
    vl_00 = RG1L;
    RG1L = signedlong(RG51w);
    if (RG1L == lcon(0x9))
    {
      RG1L = DM2_IS_CONTAINER_MONEYBOX(RG61L);
      if (RG1L != 0 && RG4L == 0)
        //m_200F2:
        RG1L = 0;
      else
        RG1L = 1;
      //m_200F4:
      if (RG1L != 0)
      {
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(vql_0c.peek16())));
        RG64p = RG1P;
        xp_04 = UPCAST(unk, RG1P);
        RG4L = signedlong(vw_08);
        RG1P += 2;
        RG2L = lcon(0xff);
        DM2_1c9a_078b(RG1P, RG4L, RG2L);
        if (vl_00 != 0)
        {
          RG62W = word_at(RG64p, 2);
          //m_2012C:
          for (;;)
          {
            RG52w = RG62W;
            if (RG62W == lcon(0xfffffffe))
              break;
            RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG62UW)));
            RG62L = RG1L;
            RG4P = DOWNCAST(unk, xp_04) + 2;
            RG1L = unsignedlong(RG52w);
            DM2_CUT_RECORD_FROM(mk_record(RG1L), UPCAST(t_record, RG4P), -1, 0);
            RG52w &= wcon(0x3fff);
            RG1W = unsignedword(RG1Blo);
            RG1Blo = vb_14;
            RG1L <<= bcon(0xe);
            RG52w |= RG1W;
            RG1L = unsignedlong(RG52w);
            RG3L = 0;
            RG2L = lcon(0xffffffff);
            RG4P = RG7p;
            DM2_APPEND_RECORD_TO(mk_record(RG1L), UPCAST(t_record, RG4P), RG2W, RG3W);
          }
        }
      }
    }
    //m_2017E:
    if (vl_00 != 0)
    {
      RG63L = unsignedlong(vql_0c.peek16());
      RG3L = 0;
      RG2L = lcon(0xffffffff);
      RG4P = RG7p;
      DM2_CUT_RECORD_FROM(mk_record(RG63L), UPCAST(t_record, RG4P), RG2W, RG3W);
      RG1L = RG63L;
      DM2_DEALLOC_RECORD(RG1L);
    }
  }
  FOREND
}

i32 DM2_1c9a_0958(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG5l;
  i32 vl_00;

  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
  RG2P = RG1P;
  RG1P = DOWNCAST(i16, DM2_query_1c9a_02c3(RG2P, DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG2P, lcon(0x4)))))));
  RG4P = RG1P + 2;
  RG1W = word_at(RG1P);
  RG1L = unsignedlong(RG1W);
  RG5l = unsignedlong(byte_at(RG2P, lcon(0x4)));
  RG3P = ADRESSOF(i32, &vl_00);
  RG2P = RG4P;
  RG4L = RG1L;
  RG1L = RG5l;
  DM2_4DEA(RG1Blo, RG4L, RG2P16, RG3P32);
  RG1Blo = CUTLX8(vl_00) & lcon(0xffffff80);
  RG1L = unsignedlong(RG1Blo) >> 7;
  return RG1L;
}

// belongs to DM2_ACTIVATE_CREATURE_KILLER
i32 DM2_1c9a_09b9(i32 eaxl, i32 edxl)
{
  c_nreg _RG1;
  c_nreg _RG4; RG4L = edxl;

  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(CUTX16(eaxl))));
  RG1Blo = RG4W == word_at(RG1P, lcon(0x8)) ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  return RG1L;
}

// belongs to DM2_FILL_CAII_CUR_MAP
void DM2_1c9a_09db(unk* xeaxp)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2P = xeaxp;
  c_nreg _RG3;
  c_nreg _RG4;
  unk* xp_00;
  i32 vl_04; // TODO size unknown, set to max length
  unk** xparpp00;

  xp_00 = UPCAST(unk, ADRESSOF(i32, &vl_04));
  RG1P = DOWNCAST(i16, DM2_query_1c9a_02c3(RG2P, DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG2P, lcon(0x4)))))));
  i32 parl01 = unsignedlong(word_at(RG2P, lcon(0xc)));
  xparpp00 = UPCAST(unk*, ADRESSOF(unk*, &xp_00));
  DM2_GET_CREATURE_ANIMATION_FRAME(unsignedlong(byte_at(RG2P, lcon(0x4))), lcon(0x11), RG1P16, UPCAST(i16, RG1P + 2), xparpp00, parl01);
}

// was SKW_1c9a_0a48
i32 DM2_CREATURE_SOMETHING_1c9a_0a48(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8 vba_00[0x4];
  i16 vw_04;
  i16 vw_08;
  i8 vb_0c;
  i8 vb_10;
  unk** xparpp00;
  i16 parw03;
  i16 parw04;

  RG4P = s350.v1e0552;
  RG1Blo = byte_at(RG4P, lcon(0x9));
  vb_10 = RG1Blo;
  RG3P = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG1W = unsignedword(RG1Blo);
  RG1Blo = byte_at(RG3P, lcon(0x4));
  vb_0c = RG1Blo;
  RG2P = s350.v1e055e;
  RG1W = word_at(RG2P);
  vw_04 = RG1W; // adj
  RG1W = word_at(RG2P, 2);
  vw_08 = RG1W; // adj
  if (s350.v1e055a == NULL)
  {
    if (jz_test8(location(RG4P), lcon(0x1)))
      //m_2033C:
      RG2L = 0;
    else
      RG2W = word_at(RG3P, lcon(0xc));
    //m_2033E:
    RG1L = unsignedlong(RG2W);
    i32 parl01 = RG1L;
    xparpp00 = &s350.v1e055a;
    RG4L = signedlong(byte_at(DOWNCAST(c_creature, s350.creatures), lcon(0x1a)));
    RG1L = unsignedlong(vb_0c);
    RG3P = ADRESSOF(i16, &vw_08);
    RG2P = ADRESSOF(i16, &vw_04);
    DM2_GET_CREATURE_ANIMATION_FRAME(RG1L, RG4L, RG2P16, RG3P16, xparpp00, parl01);
    if (s350.v1e055a == NULL)
    {
      DM2_ZERO_MEMORY(ADRESSOF(i8, vba_00), lcon(0x4));
      s350.v1e055a = ADRESSOF(i8, vba_00);
    }
  }
  //m_20383:
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG2W = unsignedword(RG2Blo);
  RG2Blo = byte_at(RG1P, lcon(0x7));
  RG4P = s350.v1e055a;
  RG4Blo = byte_at(RG4P, lcon(0x3)) & lcon(0x1);
  RG4L = unsignedlong(RG4Blo);
  if (RG4L != 0)
  {
    RG4W = signedword(byte_at(RG1P, lcon(0x1a)));
    RG1L = signedlong(byte_at(RG1P, lcon(0x1a)));
    if (RG1L != lcon(0x24))
    {
      RG1L = signedlong(RG4W);
      if (RG1L != lcon(0x23))
      {
        if (RG1L != lcon(0x25))
        {
          RG2Blo &= lcon(0xffffffc0);
          RG1Blo = vb_10 & lcon(0x3);
          RG4L = unsignedlong(RG1Blo);
          if (RG4W != 0)
          {
            RG1W = DM2_RAND16(RG4W);
            RG3W = RG1W;
            RG4W = RG1W;
            RG1L = DM2_RANDBIT() ? 1 : 0;
            if (RG1W != 0)
            {
              RG4L = -RG4L;
              RG4W &= lcon(0x7);
            }
            RG2L |= RG4L;
          }
          RG1L = unsignedlong(vb_10) / 4 & lcon(0x3);
          RG4L = unsignedlong(RG1Blo);
          if (RG4W != 0)
          {
            RG1W = DM2_RAND16(RG4W);
            RG3W = RG1W;
            RG4W = RG1W;
            RG1L = DM2_RANDBIT() ? 1 : 0;
            if (RG1W != 0)
            {
              RG4L = -RG4L;
              RG4W &= lcon(0x7);
            }
            RG4L *= 8;
            RG2L |= RG4L;
          }
        }
      }
    }
  }
  RG1P = s350.v1e055a;
  RG1Blo = byte_at(RG1P, lcon(0x3)) & 2;
  RG1L = unsignedlong(RG1Blo) / 2;
  if (RG1L != 0)
  {
    RG1L = DM2_RANDBIT() ? 1 : 0;
    if (RG1W == 0)
      //m_2045A:
      RG2Blo &= lcon(0xffffffbf);
    else
      RG2Blo |= lcon(0x40);
  }
  //m_2045D:
  mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x7)), RG2Blo);
  mov16(location(s350.v1e055e), vw_04);
  mov16(location(s350.v1e055e + 2), vw_08);
  RG1P = s350.v1e055a;
  RG1Blo = byte_at(RG1P) & lcon(0x7f);
  RG2L = unsignedlong(RG1Blo);
  if (RG2L != lcon(0x7f))
  {
    put16(parw04, unsignedword(s350.v1e0562.getyA()));
    put16(parw03, unsignedword(s350.v1e0562.getxA()));
    DM2_QUEUE_NOISE_GEN1(lcon(0xf), vb_0c, RG2Blo, lcon(0x46), lcon(0x80), parw03, parw04, lcon(0x1));
  }
  RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG1W = word_at(RG1P, lcon(0xa)) & lcon(0x40);
  RG1L = unsignedlong(RG1W);
  if (RG1L == 0)
  {
    bool skip00533 = false;
    //m_204F7:
    RG1P = s350.v1e055a;
    RG1Blo = byte_at(RG1P, lcon(0x3)) & lcon(0xc);
    RG1L = unsignedlong(RG1Blo) / 4;
    RG4L = RG1L;
    if (RG1W != 0)
    {
      RG1W = DM2_RAND16(RG4W);
      RG4W = RG1W;
    }
    RG1P = s350.v1e055a;
    RG1Blo = byte_at(RG1P, lcon(0x3)) & lcon(0xfffffff0);
    RG1L = unsignedlong(RG1Blo) >> 4;
    RG4L = signedlong(RG4W) + RG1L;
    RG1L = signedlong(byte_at(DOWNCAST(c_creature, s350.creatures), lcon(0x1a)));
    if (RG1L != lcon(0x13) || ddat.savegames1.b_03 == 0)
      skip00533 = true;
    else
    {
      RG1P = s350.v1e0552;
      if (jnz_test8(location(RG1P + lcon(0x1)), lcon(0x10)))
        skip00533 = true;
      else
        RG4L = RG4L * lcon(0x3);
    }

    if (skip00533)
    {
      bool skip00534 = false;
      //m_2055F:
      RG1W = ddat.v1d3248;
      if (RG1W == ddat.v1e08d6)
        skip00534 = true;
      else
      {
        RG1L = signedlong(s350.v1e0584);
        if ((table1d607e[RG1L].uc[2] & bcon(0x1)) != 0)
          skip00534 = true;
        else
        {
          RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
          RG2W = word_at(RG1P, lcon(0xa)) & lcon(0x8000);
          RG2L = unsignedlong(RG2W);
          if (RG2L == 0)
            skip00534 = true;
          else
          {
            RG1W = word_at(RG1P, lcon(0xa)) & 2;
            RG1L = unsignedlong(RG1W);
            if (RG1L != 0)
              skip00534 = true;
            else
            {
              RG4L *= 4;
              RG4L += (DM2_RANDBIT() ? 1 : 0);
            }
          }
        }
      }

      if (skip00534)
      {
        //m_205B4:
        if (ddat.v1e0238 == 0)
        {
          //m_205D2:
          RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
          RG2W = word_at(RG1P, lcon(0xa)) & lcon(0x8);
          RG2L = unsignedlong(RG2W);
          if (RG2L != 0)
          {
            bool skip00536 = false;
            RG1W = word_at(RG1P, lcon(0xa)) & lcon(0x4000);
            RG1L = unsignedlong(RG1W);
            if (RG1L != 0)
            {
              RG1L = signedlong(RG4W);
              if (RG1L < lcon(0x3))
                skip00536 = true;
            }

            if (!skip00536)
            {
              //m_20600:
              RG4L = signedlong(RG4W);
              RG1L = 5 * RG4L;
              RG4L = RG1L;
              RG1L <<= 4;
              RG1L -= RG4L;
              RG4L = RG1L;
              RG2L = lcon(0x64);
              RG4L = RG1L % RG2L;
              RG1L /= RG2L;
            }

            //m_2061D:
            RG4L = signedlong(DM2_MAX(1, RG1W));
          }
        }
        else
        {
          RG4L = 2 * RG4L;
          RG1W = ddat.v1d3248;
          if (RG1W != ddat.v1e08d6)
            RG4L = 2 * RG4L;
        }
      }
    }
    //m_2062C:
    RG1L = signedlong(RG4W);
  }
  else
  {
    RG1P = s350.v1e055a;
    RG1Blo = byte_at(RG1P, lcon(0x3)) & lcon(0xfffffff0);
    RG1L = unsignedlong(RG1Blo) >> 4;
    RG4L = signedlong(RG1W);
    RG1L = signedlong(DM2_MIN(1, RG4W));
  }
  //m_2062F:
  RG1L += timdat.gametick;
  RG4P = ADRESSOF(i8, vba_00);
  if (RG4P == DOWNCAST(unk, s350.v1e055a))
    s350.v1e055a = NULL;
  return RG1L;
}

void DM2_1c9a_0cf7(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  c_ql32 vql_00;
  c_tim c38_04;

  vql_00.poke32(RG1L);
  RG3L = RG4L;
  RG4L = unsignedlong(RG3W);
  RG1L = DM2_GET_CREATURE_AT(unsignedlong(vql_00.peek16()), RG4L);
  RG2L = unsignedlong(RG1W);
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
  RG4P = RG1P;
  RG5l = unsignedlong(byte_at(RG1P, lcon(0x5)));
  RG1L = 16 * RG5l;
  RG5l += RG1L;
  RG5l = 2 * RG5l;
  RG1L = signedlong(word_at(DOWNCAST(c_creature, creatures), RG5l + 2));
  if (RG1L != lcon(0xffffffff))
    DM2_1c9a_0db0(RG2L);
  c38_04.setmticks(ddat.v1d3248, timdat.gametick + 1);
  RG1Blo = (word_at(RG4P + wcon(0x8)) != wcon(0xffff) ? 1 : 0) + lcon(0x21);
  c38_04.settype(RG1Blo);
  RG1Blo = byte_at(RG4P, lcon(0x4));
  c38_04.setactor(RG1Blo);
  RG1Blo = vql_00.peek8();
  c38_04.setxyA(RG1Blo, RG3Blo);
  RG1W = DM2_QUEUE_TIMER(&c38_04);
  RG2L = RG1L;
  RG4Blo = byte_at(RG4P, lcon(0x5));
  RG4L = unsignedlong(RG4Blo);
  RG1L = 17 * RG4L;
  mov16(location(DOWNCAST(c_creature, creatures) + 2 * RG1L + 2), RG2W);
}

void DM2_1c9a_0db0(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4;

  RG4L = RG1L;
  RG4Blo ^= RG1Blo;
  RG4Bhi &= lcon(0x3c);
  RG4L = unsignedlong(RG4W) >> bcon(0xa);
  if (RG4L == lcon(0x4))
  {
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
    RG1Blo = byte_at(RG1P, lcon(0x5));
    if (RG1Blo != lcon(0xffffffff))
    {
      RG4L = unsignedlong(RG1Blo);
      RG1L = 16 * RG4L;
      RG4L += RG1L;
      RG4L = 2 * RG4L;
      RG1P = DOWNCAST(c_creature, creatures);
      RG1W = word_at(RG1P, RG4L + 2);
      if (RG1W >= 0)
      {
        RG1L = unsignedlong(RG1W);
        DM2_DELETE_TIMER(RG1W);
        mov16(location(DOWNCAST(c_creature, creatures) + RG4L + 2), lcon(0xffffffff));
      }
    }
  }
}

// belongs to DM2_ALLOC_CAII_TO_CREATURE
static void DM2_14cd_0802(void)
{
  mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x12)), lcon(0xffffffff));
  mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x13)), 0);
}

void DM2_ALLOC_CAII_TO_CREATURE(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p;
  i8* RG61p;
  i32 vl_00;
  i16 vw_04;
  i16 vw_08;

  RG3L = RG1L;
  vw_08 = RG4W;
  vw_04 = RG2W;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
  RG61p = RG1P;
  if (byte_at(RG1P + lcon(0x5)) != lcon(0xffffffff))
    return;
  RG4W = word_at(RG1P, lcon(0xe));
  RG4L <<= bcon(0x5);
  RG4UW >>= bcon(0xf);
  RG4L = unsignedlong(RG4W);
  vl_00 = RG4L;
  or8(location(RG1P + lcon(0xf)), lcon(0x4));
  //m_20799:
  for (;;)
  {
    RG5p = DOWNCAST(c_creature, creatures);
    RG2L = 0;
    RG4W = ddat.v1e08a0;
    //m_207B4:
    for (;;)
    {
      if (RG2UW < RG4UW)
      {
        //m_207AA:
        if (word_at(RG5p) >= wcon(0x0))
        {
          RG5p += lcon(0x22);
          RG2L++;
          continue;
        }
        //m_207EA:
        RG1L = vl_00;
        RG1W &= 0x1;
        and8(location(RG61p + lcon(0xf)), lcon(0xfffffffb));
        RG1L <<= bcon(0xa);
        or16(location(RG61p + lcon(0xe)), RG1W);
        ddat.v1d4020++;
        DM2_ZERO_MEMORY(RG5p, lcon(0x22));
        RG1L = RG3L;
        RG1Bhi &= 0x3;
        mov16(location(RG5p), RG1W);
        mov8(location(RG61p + lcon(0x5)), RG2Blo);
        mov16(location(RG5p + 2), lcon(0xffffffff));
        mov8(location(RG5p + lcon(0x1a)), lcon(0xffffffff));
        RG2L = signedlong(vw_04);
        RG4L = signedlong(vw_08);
        RG1P = DM2_PREPARE_LOCAL_CREATURE_VAR(unsignedlong(RG3W), RG4L, RG2L, lcon(0x22));
        RG2P = RG1P;
        DM2_14cd_0802();
        RG1L = timdat.gametick;
        RG1UL >>= bcon(0x2);
        RG1L--;
        mov8(location(RG5p + lcon(0x6)), RG1Blo);
        RG1Blo = CUTLX8(timdat.gametick) - bcon(0x7f);
        mov8(location(RG5p + lcon(0x4)), RG1Blo);
        RG1Blo = s350.v1e0562.getxA();
        RG1W &= lcon(0x1f);
        and8(location(RG5p + lcon(0xc)), lcon(0xffffffe0));
        RG4W = word_at(RG5p, lcon(0xc));
        RG4L |= RG1L;
        mov16(location(RG5p + lcon(0xc)), RG4W);
        RG1Blo = s350.v1e0562.getyA() & 0x1f;
        RG3L = RG4L & lcon(0xfffffc1f);
        RG1L <<= bcon(0x5);
        RG4L = RG3L | RG1L;
        mov16(location(RG5p + lcon(0xc)), RG4W);
        RG1W = ddat.v1d3248 & lcon(0x3f);
        and8(location(RG5p + lcon(0xd)), lcon(0x3));
        RG1L <<= bcon(0xa);
        or16(location(RG5p + lcon(0xc)), RG1W);
        RG1Blo = 0;
        s350.v1e07d8.b_01 = RG1Blo;
        mov8(location(RG5p + lcon(0x16)), lcon(0xffffffff));
        mov8(location(RG5p + lcon(0x17)), lcon(0xffffffff));
        mov8(location(RG5p + lcon(0x7)), RG1Blo);
        DM2_1c9a_0cf7(unsignedlong(vw_08), unsignedlong(vw_04));
        if (word_at(RG61p + lcon(0x8)) != lcon(0xffffffff))
          //m_208D7:
          RG1L = 0;
        else
          RG1L = lcon(0x11);
        //m_208D9:
        mov8(location(RG5p + lcon(0x1a)), RG1Blo);
        RG1P = s350.v1e0552;
        if (jz_test8(location(RG1P), lcon(0x1)))
        {
          or8(location(DOWNCAST(SPX_Creature, s350.v1e054e) + lcon(0xb)), lcon(0xffffff80));
          and8(location(DOWNCAST(SPX_Creature, s350.v1e054e) + lcon(0xb)), lcon(0xffffffbf));
          DM2_CREATURE_SOMETHING_1c9a_0a48();
        }
        //m_208FD:
        DM2_UNPREPARE_LOCAL_CREATURE_VAR(RG2P);
        return;
      }
      RG2W = ddat.v1d4020;
      //m_207C0:
      for (;;)
      {
        if (RG2UW > mkuw(ddat.v1d4020))
          break;
        RG4L = lcon(0xff);
        RG1L = DM2_RECYCLE_A_RECORD_FROM_THE_WORLD(lcon(0x4), RG4L);
        if (RG1W == lcon(0xffffffff))
          DM2_RAISE_SYSERR(0x47);
      }
      break;
    }
  }
  FOREND
}

void DM2_1c9a_0fcb(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  i8* RG61p;
  i16 vw_00;
  i16 vw_04;
  bool skip00540 = false;

  if (RG1UW > mkuw(ddat.v1e08a0))
    return;
  RG4L = unsignedlong(RG1W);
  RG1L = 34 * RG4L;
  RG4P = DOWNCAST(c_creature, creatures) + RG1L;
  if (word_at(RG4P) < wcon(0x0))
    return;
  RG2P = RG4P;
  RG5w = word_at(RG4P) | wcon(0x1000);
  RG3L = unsignedlong(RG5w);
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
  RG61p = RG1P;
  RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(RG3L);
  if ((RG1Blo & bcon(0x1)) == 0)
  {
    RG1L = signedlong(byte_at(RG4P, lcon(0x1a)));
    if (RG1L == lcon(0x13))
    {
      RG1L = 1;
      skip00540 = true;
    }
  }

  if (!skip00540)
    //m_20976:
    RG1L = 0;

  //m_20978:
  RG3L = RG1L;
  if (RG1L != 0)
  {
    RG1W = word_at(RG2P, 2);
    if (RG1W != -1)
    {
      //m_2098E:
      vw_00 = unsignedword(timdat.timerarray[RG1W].getxA());
      vw_04 = unsignedword(timdat.timerarray[RG1W].getyA());
    }
    else
      RG3L = 0;
  }
  //m_209B2:
  mov8(location(RG2P + lcon(0x1a)), 0);
  RG1L = unsignedlong(RG5w);
  DM2_1c9a_0db0(RG1L);
  ddat.v1d4020--;
  mov8(location(RG61p + lcon(0x5)), lcon(0xffffffff));
  mov16(location(RG2P), lcon(0xffffffff));
  if (RG3L != 0)
    DM2_DELETE_CREATURE_RECORD(unsignedlong(vw_00), unsignedlong(vw_04), 0, lcon(0x1));
}

// stacksize was 0x38
i16 DM2_CREATE_MINION(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i16 argw1, i32 argl2, i8 argb3)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  i16 RG6w;
  i32 vl_00;
  i16 vw_04;
  i16 vw_08;
  i16 vw_0c;
  i32 vl_10;
  i32 vl_14;
  i8 vb_18;
  i16 vw_1c;
  i16 vw_20;
  i16 vw_24;

  vw_0c = RG1W; // truncated
  vw_08 = RG4W; // truncated
  vw_04 = RG2W; // truncated
  vl_00 = RG3L;
  vw_20 = ddat.v1d3248;
  RG1L = signedlong(argw1);
  DM2_CHANGE_CURRENT_MAP_TO(RG1L);
  RG4L = lcon(0xffff);
  vw_1c = RG4W;
  RG1Blo = RG4W != mkw(argl2) ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  RG1Blo = RG1L != 0 ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  vl_14 = RG1L;
  vw_24 = RG3W;
  RG5L = argl0;
  if (argb3 >= bcon(0x0))
    //m_20A46:
    RG6w = wcon(0x4);
  else
    RG6w = wcon(0x0);
  //m_20A4B:
  RG1L = DM2_RANDBIT() ? 1 : 0;
  if (RG1W == 0)
    //m_20A5C:
    RG1L = lcon(0xffffffff);
  else
    RG1L = 1;
  //m_20A61:
  vb_18 = RG1Blo;
  //m_20A65:
  for (;;)
  {
    bool skip00542 = false;
    RG4L = signedlong(RG5W);
    RG1L = dm2_GET_TILE_VALUE(signedlong(vw_24), RG4L);
    RG4Blo = RG1Blo;
    RG1L = unsignedlong(RG4Blo);
    RG4L = unsignedlong(RG4Blo) >> 5;
    RG4W = unsignedword(RG4Blo);
    if (RG4W != 0)
    {
      RG3L = unsignedlong(RG4W);
      if (RG3L != lcon(0x3))
      {
        bool skip00541 = false;
        if (RG3L != lcon(0x6))
          skip00541 = true;
        else
        {
          RG2L = RG1L;
          RG2Bhi ^= RG1Bhi;
          RG2Blo &= 0x5;
          RG2L = signedlong(RG2W);
          if (RG2L != 0)
            skip00541 = true;
        }

        if (skip00541)
        {
          //m_20AB2:
          RG4L = unsignedlong(RG4W);
          if (RG4L != lcon(0x4))
            skip00542 = true;
          else
          {
            RG1Blo &= lcon(0x7);
            RG1W = unsignedword(RG1Blo);
            if (RG1W == 0)
              skip00542 = true;
            else
            {
              RG1L = signedlong(RG1W);
              if (RG1L == lcon(0x5))
                skip00542 = true;
            }
          }
        }
      }
    }

    if (skip00542)
    {
      //m_20ACC:
      RG4L = unsignedlong(RG5W);
      RG1L = DM2_GET_CREATURE_AT(unsignedlong(vw_24), RG4L);
      RG1Blo = RG1W != wcon(0xffff) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L == 0)
      {
        RG1W = RG5W;
        i32 parl00 = RG1L;
        RG3L = unsignedlong(vw_24);
        RG2L = unsignedlong(vw_04);
        vl_10 = unsignedlong(vw_08);
        if (vl_14 == 0)
          //m_20B10:
          RG1L = 0;
        else
          RG1L = lcon(0x8000);
        //m_20B12:
        RG1L = unsignedlong(RG1W) | unsignedlong(vw_0c);
        RG1L = unsignedlong(RG1W);
        RG4L = vl_10;
        RG1W = dm2_dballochandler.DM2_ALLOC_NEW_CREATURE(RG1W, RG4W, RG2W, RG3W, CUTX16(parl00));
        vw_1c = RG1W;
        if (RG1W != -1)
          break;
      }
    }

    //m_20B35:
    if (RG6w != 0)
    {
      RG1L = signedlong(argb3);
      RG4L = vl_00;
      RG3W = table1d27fc[RG1L];
      RG4L += RG3L;
      vw_24 = RG4W;
      RG5L = argl0;
      RG5W += table1d2804[RG1L];
      RG1Blo = argb3 + vb_18 & lcon(0x3);
      argb3 = RG1Blo;
    }
    RG6w--;
    if (RG6w == lcon(0xffffffff))
      break;
  }
  //m_20B72:
  if (vl_14 != 0)
  {
    RG2W = vw_1c;
    RG1Blo = RG2W != wcon(0xffff) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 0)
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
      RG1W = word_at(RG1P, 2);
      //m_20B97:
      for (;;)
      {
        RG4Blo = RG1W != wcon(0xfffe) ? 1 : 0;
        RG4L = unsignedlong(RG4Blo);
        if (RG4L != 0)
        {
          RG4L = RG1L;
          RG4Blo ^= RG1Blo;
          RG4Bhi &= lcon(0x3c);
          RG4L = unsignedlong(RG4W) >> bcon(0xa);
          if (RG4L != lcon(0xe))
          {
            RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG1UW)));
            continue;
          }
        }
        break;
      }
      //m_20BC7:
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
      RG4L = argl2;
      mov16(location(RG1P + 2), RG4W);
    }
  }
  //m_20BD8:
  DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_20));
  return vw_1c;
}

// belongs to DM2_ENGAGE_COMMAND
void DM2_RELEASE_MINION(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG5l;
  i16 RG6w;

  RG5l = unsignedlong(RG1W);
  RG4L = lcon(0xffff);
  RG1P = DM2_GET_MISSILE_REF_OF_MINION(RG5l, RG4L);
  RG4P = RG1P;
  if (RG1P != NULL)
  {
    RG6w = ddat.v1d3248;
    RG1W = word_at(RG1P, lcon(0x4));
    RG1UW >>= bcon(0xa);
    RG1L = signedlong(RG1W);
    DM2_CHANGE_CURRENT_MAP_TO(RG1L);
    RG1W = word_at(RG4P, lcon(0x4));
    RG1L <<= bcon(0x6);
    _RG1.ushiftr11();
    RG2L = signedlong(RG1W);
    RG1W = word_at(RG4P, lcon(0x4)) & lcon(0x1f);
    RG4L = signedlong(RG1W);
    DM2_ai_13e4_0360(RG5l, RG4L, RG2L, lcon(0x13), lcon(0x1));
    RG1L = signedlong(RG6w);
    DM2_CHANGE_CURRENT_MAP_TO(RG1L);
  }
}

// belongs to DM2_WOUND_CREATURE
i32 DM2_1c9a_17c7(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_wlreg _RG6;
  bool skip00550 = false;

  RG5L = RG1L;
  RG6L = RG4L;
  if (RG2W == ddat.v1e08d6 && ddat.v1e0238 == 0 && ddat.v1e0976 == 0)
  {
    RG1W = ddat.v1e08d8;
    RG1L -= RG5L;
    RG1L = signedlong(DM2_ABS(RG1W));
    RG4L = RG1L;
    RG1W = ddat.v1e08d4;
    RG1L -= RG6L;
    RG1L = signedlong(DM2_ABS(RG1W));
    if (RG4W != RG1W)
    {
      RG3L = signedlong(RG6W);
      RG2L = signedlong(RG5W);
      RG4L = signedlong(ddat.v1e08d4);
      RG1L = signedlong(DM2_CALC_VECTOR_DIR(ddat.v1e08d8, RG4W, RG2W, RG3W));
      RG4L = unsignedlong(RG1W);
      RG1L = signedlong(ddat.v1e08da);
      if (RG4L == RG1L)
      {
        RG1W = ddat.v1e08d8;
        RG1L -= RG5L;
        RG1L = signedlong(DM2_ABS(RG1W));
        if (RG1L <= 2)
        {
          RG1W = ddat.v1e08d4;
          RG1L -= RG6L;
          RG1L = signedlong(DM2_ABS(RG1W));
          if (RG1L <= 2)
          {
            RG1L = 1;
            skip00550 = true;
          }
        }
      }
    }
  }

  if (!skip00550)
    //m_213E9:
    RG1L = 0;

  //m_213EB:
  RG1Blo = RG1L != 0 ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  return RG1L;
}

void DM2_1c9a_19d4(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG4; RG4L = edxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG1; RG1L = eaxl;
  i16 RG5w;
  i32 vl_00;

  RG5w = RG1W;
  RG1L = signedlong(RG3W) & lcon(0x8000);
  vl_00 = RG1L;
  RG3Bhi &= lcon(0x7f);
  if (RG3UW < mkuw(lcon(0x3)))
    return;
  if (RG3UW <= mkuw(lcon(0x3)))
    return;
  if (RG3UW < mkuw(lcon(0x5)))
    return;
  if (RG3UW <= mkuw(lcon(0x5)))
    return;
  if (RG3UW > mkuw(lcon(0x15)))
    return;
  RG1L = RG3L - lcon(0x6);
  if (vl_00 != 0)
    RG1Bhi |= lcon(0xffffff80);
  RG3L = unsignedlong(RG1W);
  RG2L = signedlong(RG2W);
  RG4L = signedlong(RG4W);
  RG1L = unsignedlong(RG5w);
  DM2_ATTACK_CREATURE(RG1L, RG4L, RG2L, RG3L, lcon(0x64), 0);
}

i32 DM2_1c9a_1a48(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG6w;
  i16 vw_00;
  i16 vw_04;

  RG6w = RG1W;
  vw_04 = RG4W; // truncated
  RG1L = signedlong(RG1W);
  if (RG1L != lcon(0x1))
    //m_216EA:
    vw_00 = -1;
  else
    vw_00 = 2;
  //m_216F0:
  RG4W = ddat.v1e08b0;
  if (RG4W == lcon(0xffffffff))
  {
    RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(ddat.v1e08a8, ddat.v1e08aa));
    ddat.v1e08b0 = RG1W;
    RG4L = RG1L;
  }
  //m_2171B:
  for (;;)
  {
    RG1Blo = RG4W != wcon(0xfffe) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == 0)
      return lcon(0xffffffff);
    RG1L = RG4L;
    RG1Blo ^= RG4Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    RG1L = signedlong(RG1W);
    if (RG1L > lcon(0x3))
      return lcon(0xffffffff);
    if (RG1L == 2)
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
      RG2W = word_at(RG1P, 2) & lcon(0x6);
      if (RG2W == lcon(0x4))
      {
        bool skip00555 = false;
        RG2W = word_at(RG1P, 2);
        RG1L = RG2L;
        RG1UW >>= bcon(0x3);
        RG3L = unsignedlong(RG1W) >> bcon(0x9) & lcon(0xf);
        if (RG3L == signedlong(RG6w))
          skip00555 = true;
        else
        {
          if (RG3L == signedlong(vw_00) && (RG2Blo & bcon(0x1)) != 0)
            skip00555 = true;
        }

        if (skip00555)
        {
          //m_21794:
          RG3L = unsignedlong(RG1W) >> 5 & lcon(0xf);
          RG2L = lcon(0x1) << RG3Blo;
          RG3L = RG2L;
          RG2L = unsignedlong(vw_04);
          if ((RG2L & RG3L) != 0)
          {
            RG1W &= lcon(0x1f);
            return RG1L;
          }
        }
      }
    }
    //m_217B9:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
    RG4L = RG1L;
  }
  FOREND
}

i32 DM2_1c9a_1b16(i32 eaxl, i32 edxl)
{
  c_nreg _RG4; RG4L = edxl;
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  i32 RG5l;
  i16 RG6w;
  c_ql32 vql_00;

  vql_00.poke32(RG1L);
  RG6w = RG4W;
  RG4W = ddat.v1e08b0;
  if (RG4W == lcon(0xffffffff))
  {
    RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(ddat.v1e08a8, ddat.v1e08aa));
    ddat.v1e08b0 = RG1W;
    RG4L = RG1L;
  }
  //m_21807:
  for (;;)
  {
    RG1Blo = RG4W != wcon(0xfffe) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == 0)
      return lcon(0xffffffff);
    RG1L = RG4L;
    RG1Blo ^= RG4Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    RG1L = signedlong(RG1W);
    if (RG1L > lcon(0x3))
      return lcon(0xffffffff);
    if (RG1L == 2)
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
      RG2W = word_at(RG1P, 2) & lcon(0x6);
      if (RG2W == lcon(0x4))
      {
        RG2W = word_at(RG1P, 2);
        RG2UW >>= bcon(0x3);
        RG1L = unsignedlong(RG2W);
        RG5l = RG1L >> bcon(0x9) & lcon(0xf);
        RG3L = signedlong(vql_00.peek16());
        if (RG5l == RG3L)
        {
          RG3L = RG1L >> 5 & lcon(0xf);
          RG1L = signedlong(RG6w);
          if (RG3L == RG1L)
          {
            RG1L = RG2L;
            RG1Bhi ^= RG2Bhi;
            RG1Blo &= lcon(0x1f);
            return RG1L;
          }
        }
      }
    }
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
    RG4L = RG1L;
  }
  FOREND
}

// id: 0x2189F
// adress given to a functionpointer
static i32 DM2_1c9a_1bae(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG4; RG4L = edxl;

  RG2L = unsignedlong(s350.v1e0562.getxA());
  if (RG1W == RG2W)
  {
    RG2L = unsignedlong(s350.v1e0562.getyA());
    if (RG4W == RG2W)
      return 0;
  }
  return DM2_1BAAD(signedlong(RG1W), signedlong(RG4W));
}

// stacksize was 0x15c
// was SKW_1c9a_1bd8 (new)
i32 DM2_FIND_WALK_PATH(i32 eaxl, i32 edxl, i32 ebxl, i16 argw0, s_len8* argp_s23, c_button* button)
{
  c_nreg _RG4; RG4L = edxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG3;
  c_wlreg _RG51;
  c_wlreg _RG52; // TODO: check initialization
  c_nreg _RG6; // TODO: maybe more than one
  i8* RG61p;
  c_ql32 vql_00;
  c_ql32 vql_04;
  i8 vba_08[0x33]; // bytearray
  i32 vl_3c;
  i32 vl_40;
  c_ql32 vql_44;
  i32 vl_48;
  i32 vl_4c;
  i32 vl_50;
  i32 vl_54;
  i32 vl_58;
  i32 vl_5c;
  i32 vl_60;
  i32 vl_64;
  unk* xp_68;
  i32 vl_6c;
  i32 vl_70;
  i32 vl_74;
  unk* xp_78; // add32
  i32 vl_7c; // ad32
  i32 vl_80;
  i32 vl_84;
  i32 vl_88;
  unk* xp_8c;
  unk* xp_90;
  i32 vl_94;
  unk* xp_98;
  i32 vl_9c;
  i32 vl_a0;
  i32 vl_a4;
  c_ql32 vql_a8;
  s_bbw s_31_ac; // poke 8 32 peek 32  DANGEROUS!!!
  i32 vl_b0;
  unk* xp_b4; // add32
  i32 vl_b8;
  unk* xp_bc;
  i16 vw_c0;
  i32 vl_c4;
  i16 vw_c8;
  i16 vw_cc;
  i16 vw_d0;
  i16 vw_d4;
  i16 vw_d8;
  i16 vw_dc;
  i32 vl_e0 = 0; // ATTENTION! ORIGINAL BUG? WAS UNINITIALIZED!
  i16 vw_e4;
  c_o32 vo_e8; // poke 16 peek 8 16 32 inc16
  i16 vw_ec;
  c_ql32 vql_f0;
  c_o32 vo_f4; // poke 16 peek 8 16 32 inc16
  i16 vw_f8;
  c_o32 vo_fc; // poke 16 peek 8 16 32 inc16
  i16 vw_100;
  c_ql32 vql_104;
  i16 vw_108;
  c_o32 vo_10c; // poke 16 peek 16 32 inc16
  i16 vw_110;
  i16 vw_114;
  i16 vw_118;
  c_ql32 vql_11c;
  c_ql32 vql_120;
  c_ql32 vql_124;
  i16 vw_128;
  i16 vw_12c;
  i16 vw_130;
  c_ql32 vql_134;
  i8 vb_138;
  i8 vb_13c;
  i8 vb_140; // or8
  i8 vb_144;
  i8 vb_148; // or8
  i16 parw00;
  i16 parw01;
  FPTR_L_LL pf_02;
  FPTR_L_LL pf_03;
  i16 parw04;
  i16 parw05;
  i16 parw06;
  i32 parl09;
  i16 parw10;
  i16 parw11;
  FPTR_L_LL pf_12;
  i16 parw13;
  i16 parw14;
  i16 parw16;
  i16 parw20;
  i16 parw21;
  i16 parw22;
  i16 parw23;
  i16 parw24;
  i16 parw26;
  i16 parw27;
  bool flag;

  vql_04.poke32(RG1L);
  vql_00.poke32(RG4L);
  vb_148 = RG2Blo;
  RG1P = DOWNCAST(s_len8, argp_s23);
  if (byte_at(RG1P + lcon(0x7)) == 0)
  {
    RG1L = vql_04.peek32();
    RG1W &= lcon(0x1f);
    RG4P = DOWNCAST(s_len8, argp_s23);
    and8(location(RG4P + 2), lcon(0xffffffe0));
    RG52W = word_at(RG4P, 2);
    RG52L |= RG1L;
    RG1L = vql_00.peek32();
    RG1W &= lcon(0x1f);
    RG6L = RG52L & lcon(0xfffffc1f);
    RG1L <<= bcon(0x5);
    RG2L = RG6L | RG1L;
    mov16(location(RG4P + 2), RG2W);
    RG1W = ddat.v1d3248 & lcon(0x3f);
    and8(location(RG4P + lcon(0x3)), lcon(0x3));
    RG1L <<= bcon(0xa);
    RG3W = word_at(RG4P, 2);
    RG3L |= RG1L;
    mov16(location(RG4P + 2), RG3W);
    mov16(location(RG4P + lcon(0x4)), RG3W);
    RG1P = RG4P;
    mov8(location(RG4P + lcon(0x6)), 0);
    return 0;
  }
  RG1L = signedlong(argw0) << 2;
  vl_3c = RG1L;
  RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(RG1L, true);
  xp_68 = UPCAST(unk, RG1P);
  RG2L = lcon(0x400);
  RG4L = 0;
  RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x400));
  xp_90 = UPCAST(unk, RG1P);
  RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
  RG1Blo = byte_at(RG1P, lcon(0x4));
  RG1L = 4 * unsignedlong(RG1Blo);
  vl_7c = RG1L;
  RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(RG1L, true);
  RG61p = RG1P;
  xp_8c = UPCAST(unk, RG1P);
  RG1L = 2 * unsignedlong(CUTX8(ddat.savegamep4->warr_00[2]));
  vl_9c = RG1L;
  vl_7c += RG1L;
  RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(RG1L);
  xp_98 = UPCAST(unk, RG1P);
  vl_a0 = 0;
  RG4P = DOWNCAST(s_sgwords, ddat.savegamep4);
  RG4Blo = byte_at(RG4P, lcon(0x4));
  RG4L = unsignedlong(RG4Blo);
  DM2_FILL_I16TABLE(RG1P16, -1, RG4UW);
  RG1L = signedlong(mapdat.map_width) << 7;
  vl_9c = RG1L;
  RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(RG1L);
  xp_bc = UPCAST(unk, RG1P);
  vw_e4 = ddat.v1d3248;
  movp(location(RG61p + 4 * signedlong(vw_e4)), RG1P);
  RG1L = vl_3c + vl_9c + lcon(0x400);
  vl_7c += RG1L;
  RG1L = 0;
  vl_48 = RG1L;
  vl_40 = RG1L;
  RG2L = 0;
  vl_a4 = RG1L;
  vw_d8 = -1;
  RG4W = unsignedword(RG4Blo);
  vb_140 = RG4Bhi;
  RG2L ^= RG1L;
  vw_d4 = RG2W;
  RG1Blo = vb_148;
  RG1Blo = table1d6290[RG1L];
  vb_138 = RG1Blo;
  if (RG1Blo != 0)
    //m_21AF2:
    vl_50 = 0;
  else
  {
    RG1L = table1d607e[signedlong(s350.v1e0584)].ul_00 & lcon(0x2000);
    vl_50 = RG1L;
  }
  //m_21AFA:
  RG1L = unsignedlong(vb_148);
  RG1Blo &= lcon(0x1f);
  RG1Blo = RG1L == lcon(0x5) ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  vl_60 = RG1L;
  xp_78 = UPCAST(unk, DOWNCAST(unk, xp_68));
  xp_b4 = UPCAST(unk, DOWNCAST(s_len8, argp_s23));
  RG3L = 0;
  vo_10c.poke16(RG3W);
  //m_21FFD:
  for (;;)
  {
    RG1W = vo_10c.peek16();
    if (RG1W >= argw0)
    {
      RG1W = s350.v1e0576 & lcon(0x2000);
      RG1L = unsignedlong(RG1W);
      flag = RG1L == 0;
      vl_94 = RG1L;
      if (!flag)
      {
        RG1W = s350.v1e0576;
        RG1L &= lcon(0x118);
        RG1L = unsignedlong(RG1W);
        flag = RG1L == 0;
        vl_94 = RG1L;
        if (!flag)
        {
          RG1W = s350.v1e0576;
          RG1L &= lcon(0x110);
          RG1L = unsignedlong(RG1W);
          vl_74 = RG1L;
          RG1W = s350.v1e0576;
          RG1L &= lcon(0x108);
          RG1L = unsignedlong(RG1W);
          vl_70 = RG1L;
        }
      }
      if (vl_40 != 0 || vw_d4 != 0)
        DM2_ZERO_MEMORY(DOWNCAST(unk, xp_bc), vl_9c);
      //m_22078:
      RG4L = 0;
      vw_cc = RG4W;
      RG6L = 0;
      RG1L = unsignedlong(vb_148);
      RG1Blo &= lcon(0x1f);
      RG1Blo = RG1L == lcon(0x8) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      flag = RG1L == 0;
      vl_b8 = RG1L;
      if (!flag)
      {
        RG51L = 0;
        DM2_ZERO_MEMORY(ADRESSOF(i8, vba_08), lcon(0x33));
        vba_08[0x32] = lcon(0xffffffff);
      }
      //m_220B0:
      RG2L = 0;
      vo_e8.poke16(RG2W);
      RG1W = ddat.v1d3248;
      vw_dc = RG1W;
      vw_c8 = RG1W; // truncated
      RG1L = vql_04.peek32();
      vo_f4.poke16(RG1W);
      vw_100 = RG1W;
      RG1L = vql_00.peek32();
      vw_12c = RG1W;
      vw_ec = RG1W;
      vw_130 = 1;
      vw_f8 = -1;
      vb_144 = 0;
      vql_f0.poke32(lcon(0xffffffff));
      RG2L = signedlong(vql_00.peek16());
      RG3L = signedlong(vql_04.peek16());
      RG4L = RG2L;
      RG1L = RG3L;
      DM2_19f0_045a(RG1L, RG4L);
      if (vl_48 != 0)
      {
        RG4L = RG3L;
        RG1L = DM2_GET_TELEPORTER_DETAIL(&ddat.v1e08b8, RG4L, RG2L);
        ddat.v1e08b7 = RG1Blo;
        if (RG1Blo != 0)
        {
          vql_f0.poke32(unsignedlong(ddat.v1e08b8.b_04));
          RG1L = unsignedlong(ddat.v1e08b8.b_02);
          vw_108 = RG1W;
          RG1L = unsignedlong(ddat.v1e08b8.b_03);
          vw_128 = RG1W;
        }
      }
      break;
    }
    bool skip00557 = false;
    bool skip00558 = false;
    //m_21B29:
    RG1P = DOWNCAST(unk, xp_b4);
    mov8(location(RG1P + lcon(0x6)), lcon(0xffffffff));
    RG1W = word_at(RG1P, lcon(0xc));
    RG1Blo &= lcon(0x1f);
    RG4P = DOWNCAST(unk, xp_78);
    mov8(location(RG4P), RG1Blo);
    RG1P = DOWNCAST(unk, xp_b4);
    RG1W = word_at(RG1P, lcon(0xc));
    RG1L <<= bcon(0x6);
    _RG1.ushiftr11();
    mov8(location(RG4P + lcon(0x1)), RG1Blo);
    RG1P = DOWNCAST(unk, xp_b4);
    RG1W = word_at(RG1P, lcon(0xc));
    RG1UW >>= bcon(0xa);
    mov8(location(RG4P + 2), RG1Blo);
    RG1L = signedlong(byte_at(DOWNCAST(unk, xp_b4), lcon(0x7)));
    RG1Blo = table1d62ee[RG1L];
    RG4P = DOWNCAST(unk, xp_b4);
    mov8(location(RG4P + lcon(0x10)), RG1Blo);
    RG4Blo = RG1Blo;
    vb_140 |= RG1Blo;
    RG1P = DOWNCAST(unk, xp_b4);
    RG1Blo = byte_at(RG1P, lcon(0x7));
    if (mkub(RG1Blo) < mkub(lcon(0xa)))
    {
      //m_21BB8:
      if (mkub(RG1Blo) < mkub(lcon(0x5)))
      {
        //m_21BCF:
        if (mkub(RG1Blo) >= mkub(lcon(0x1)))
        {
          if (mkub(RG1Blo) <= mkub(lcon(0x1)))
          {
            //m_21CA8:
            RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
            RG1W = word_at(RG1P, lcon(0xc)) & lcon(0x1f);
            vw_114 = RG1W; // truncated
            RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
            RG1W = word_at(RG1P, lcon(0xc));
            RG1L <<= bcon(0x6);
            _RG1.ushiftr11();
            vw_118 = RG1W; // truncated
            RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
            RG1W = word_at(RG1P, lcon(0xc));
            RG1UW >>= bcon(0xa);
            vw_110 = RG1W; // truncated
            skip00557 = true;
          }
          else
          {
            if (RG1Blo == lcon(0x3))
            {
              RG1P = DOWNCAST(c_creature, s350.creatures);
              RG2W = word_at(RG1P, lcon(0xc)) & lcon(0x1f);
              vql_134.poke32(RG2L);
              RG2W = word_at(RG1P, lcon(0xc));
              RG2L <<= bcon(0x6);
              _RG2.ushiftr11();
              vw_d0 = RG2W; // truncated
              RG1W = word_at(RG1P, lcon(0xc));
              RG1UW >>= bcon(0xa);
              vql_11c.poke32(RG1L);
              RG1L = vql_134.peek32();
              if (RG1W == ddat.v1e08d6)
              {
                if (RG2W == ddat.v1e08d8)
                {
                  RG1L = vql_11c.peek32();
                  if (RG1W == ddat.v1e08d4)
                  {
                    RG4Blo ^= 3;
                    mov8(location(DOWNCAST(unk, xp_b4) + lcon(0x10)), RG4Blo);
                  }
                }
              }
              skip00557 = true;
            }
          }
        }
      }
      else
      {
        if (mkub(RG1Blo) <= mkub(lcon(0x5)))
        {
          //m_21CE0:
          RG6L = signedlong(word_at(DOWNCAST(unk, xp_b4), lcon(0x8)));
          RG1W = ddat.v1d3248;
          if (RG1W != ddat.v1e08d6)
            //m_21D24:
            vl_88 = lcon(0x64);
          else
          {
            RG3L = signedlong(ddat.v1e08d4);
            RG2L = signedlong(ddat.v1e08d8);
            RG4L = unsignedlong(s350.v1e0562.getyA());
            RG1L = signedlong(DM2_CALC_SQUARE_DISTANCE(unsignedword(s350.v1e0562.getxA()), RG4W, RG2W, RG3W));
            vl_88 = RG1L;
          }
          //m_21D2B:
          RG4L = RG6L;
          RG1L = signedlong(DM2_MAX(CUTX16(vl_88), RG4W));
          mov8(location(DOWNCAST(unk, xp_78) + lcon(0x3)), RG1Blo);
        }
        else
        {
          if (mkub(RG1Blo) > mkub(lcon(0x7)))
            //m_21D3E:
            vl_40 = 1;
          skip00558 = true;
        }
      }
    }
    else
    {
      if (mkub(RG1Blo) <= mkub(lcon(0xa)))
        skip00558 = true;
      else
      {
        if (mkub(RG1Blo) < mkub(lcon(0x17)))
        {
          //m_21BA3:
          if (mkub(RG1Blo) <= mkub(lcon(0xb)))
            skip00557 = true;
          else
          {
            if (RG1Blo == lcon(0xc))
            {
              //m_21C48:
              RG1P = DOWNCAST(unk, xp_78);
              RG1W = signedword(byte_at(RG1P, 2));
              if (RG1W != ddat.v1d3248)
                //m_21C90:
                vql_44.poke32(lcon(0x64));
              else
              {
                RG3L = signedlong(byte_at(DOWNCAST(unk, xp_78), lcon(0x1)));
                RG2L = signedlong(byte_at(DOWNCAST(unk, xp_78)));
                RG4L = signedlong(DM2_CALC_SQUARE_DISTANCE(unsignedword(s350.v1e0562.getxA()), unsignedword(s350.v1e0562.getyA()), RG2W, RG3W));
                RG1L = signedlong(DM2_MAX(1, RG4W));
                vql_44.poke32(RG1L);
              }
              //m_21C9A:
              RG1Blo = vql_44.peek8();
              mov8(location(DOWNCAST(unk, xp_78) + lcon(0x3)), RG1Blo);
              skip00557 = true;
            }
          }
        }
        else
        {
          if (mkub(RG1Blo) <= mkub(lcon(0x17)))
          {
            //m_21D53:
            s350.v1e0576 = lcon(0x227);
            RG6L = 0;
            ddat.v1e0974 = RG6W;
            ddat.v1e0978 = RG6W;
          }
          else
          {
            if (mkub(RG1Blo) <= mkub(lcon(0x18)))
              //m_21D79:
              mov8(location(DOWNCAST(unk, xp_78) + lcon(0x3)), lcon(0x7f));
            else
            {
              if (RG1Blo == lcon(0x1b))
                //m_21D6E:
                s350.v1e0576 = lcon(0x36e7);
            }
          }
        }
      }
    }

    if (skip00558)
    {
      //m_21D48:
      RG1W = s350.v1e0578;
      vl_c4 = RG1L;
    }

    if (skip00557)
      //m_21C39:
      vl_48 = 1;

    //m_21D80:
    RG3L = 0;
    vql_124.poke32(RG3L);
    if (vb_138 == 0)
    {
      RG1L = signedlong(byte_at(DOWNCAST(unk, xp_b4), lcon(0x7)));
      RG1Blo = table1d62ee[RG1L] & lcon(0x20);
      RG1L = unsignedlong(RG1Blo);
      if (RG1L != 0)
      {
        bool skip00560 = false;
        if (ddat.savegames1.b_02 == 0)
          skip00560 = true;
        else
        {
          RG1P = s350.v1e0552;
          if (jnz_test8(location(RG1P + lcon(0x1)), lcon(0x4)))
            skip00560 = true;
          else
            RG1L = 1;
        }

        if (skip00560)
          //m_21DC6:
          RG1L = 0;
        bool skip00564 = false;
        //m_21DC8:
        vql_120.poke32(RG1L);
        if (RG1W != 0)
          skip00564 = true;
        else
        {
          bool skip00563 = false;
          if (ddat.v1e0286 != 0 && ddat.savegames1.w_00 < wcon(0x0))
          {
            RG1P = s350.v1e0552;
            if (jz_test8(location(RG1P + lcon(0x1)), lcon(0x8)))
            {
              RG1L = 1;
              skip00563 = true;
            }
          }

          if (!skip00563)
            //m_21DF6:
            RG1L = 0;

          //m_21DF8:
          vql_124.poke32(RG1L);
          if (RG1W != 0)
            skip00564 = true;
        }

        if (skip00564)
        {
          bool skip00568 = false;
          //m_21E04:
          RG1L = unsignedlong(s350.v1e0571);
          if (RG1W != ddat.v1e08d6)
          {
            //m_21E37:
            RG1L = signedlong(ddat.v1e08d6) << 4;
            RG4P = ddat.v1e03c8;
            RG2L = unsignedlong(byte_at(RG4P, RG1L + lcon(0x7)));
            RG2W += ddat.v1e08d4;
            RG3L = signedlong(RG2W);
            RG1Blo = byte_at(RG4P, RG1L + lcon(0x6));
            RG1W = unsignedword(RG1Blo) + ddat.v1e08d8;
            RG2L = signedlong(RG1W);
            RG1P = RG4P + (unsignedlong(s350.v1e0571) << 4);
            vql_104.poke32(unsignedlong(byte_at(RG1P, lcon(0x7))));
            RG4L = signedlong(CUTX16(unsignedlong(s350.v1e0562.getyA()) + vql_104.peek32()));
            RG1Blo = byte_at(RG1P, lcon(0x6));
            RG1W = unsignedword(RG1Blo);
            vql_104.poke32(RG1L);
            RG1Blo = s350.v1e0562.getxA();
            RG1L += vql_104.peek32();
            RG1L = signedlong(RG1W);
          }
          else
          {
            RG3L = signedlong(ddat.v1e08d4);
            RG2L = signedlong(ddat.v1e08d8);
            RG4L = unsignedlong(s350.v1e0562.getyA());
            RG1L = unsignedlong(s350.v1e0562.getxA());
          }
          //m_21E9B:
          RG1W = DM2_CALC_SQUARE_DISTANCE(RG1W, RG4W, RG2W, RG3W);
          RG4L = RG1L;
          vo_fc.poke16(RG4W);
          RG1L = DM2_RAND();
          RG6L = RG1L;
          RG1W &= lcon(0x8000);
          RG1L = unsignedlong(RG1W);
          if (RG1L != 0)
            skip00568 = true;
          else
          {
            bool skip00566 = false;
            if (RG4W == 0)
              skip00566 = true;
            else
            {
              bool skip00565 = false;
              RG1L = signedlong(RG4W);
              if (RG1L != lcon(0x1))
                skip00565 = true;
              else
              {
                RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
                RG1W = word_at(RG1P, lcon(0xe));
                RG1L <<= bcon(0x6);
                RG1UW >>= 14;
                vql_104.poke32(RG1L);
                RG3L = signedlong(ddat.v1e08d4);
                RG2L = signedlong(ddat.v1e08d8);
                RG4L = unsignedlong(s350.v1e0562.getyA());
                RG1L = signedlong(DM2_CALC_VECTOR_DIR(unsignedword(s350.v1e0562.getxA()), RG4W, RG2W, RG3W));
                if (RG1W != vql_104.peek16())
                  skip00565 = true;
              }

              if (skip00565)
                //m_21F05:
                RG1L = 1;
              else
                skip00566 = true;
            }

            if (skip00566)
              //m_21F0C:
              RG1L = 0;

            //m_21F0E:
            if (RG1L != 0)
              skip00568 = true;
          }

          if (skip00568)
          {
            bool skip00570 = false;
            //m_21F16:
            RG1L = signedlong(vo_fc.peek16());
            if (RG1L > lcon(0x4))
              skip00570 = true;
            else
            {
              bool skip00569 = false;
              if (vql_120.peek16() == 0)
                skip00569 = true;
              else
              {
                RG4L = RG6L;
                RG4W &= lcon(0xf);
                RG4L = unsignedlong(RG4W);
                RG2L = lcon(0x4) - RG1L;
                RG4L -= RG2L;
                RG1P = s350.v1e0552;
                RG1W = word_at(RG1P, lcon(0x14)) & lcon(0xf);
                RG1L = unsignedlong(RG1W);
                if (RG4L <= RG1L)
                {
                  RG1L = RG6L;
                  RG1W &= lcon(0x4000);
                  RG1L = unsignedlong(RG1W);
                  if (RG1L == 0)
                    skip00569 = true;
                }
              }

              if (skip00569)
              {
                //m_21F66:
                if (vql_124.peek16() != 0)
                {
                  RG4L = signedlong(ddat.v1e0286);
                  RG1L = signedlong(vo_fc.peek16()) * RG4L;
                  RG4L = unsignedlong(RG6W) >> bcon(0x8) & lcon(0x7);
                  RG1L -= RG4L;
                  RG4L = RG1L;
                  RG1P = s350.v1e0552;
                  RG1W = word_at(RG1P, lcon(0x14)) & lcon(0xf);
                  RG1L = unsignedlong(RG1W);
                  if (RG4L > RG1L)
                    skip00570 = true;
                }
              }
              else
                skip00570 = true;
            }

            if (skip00570)
            {
              //m_21FA4:
              RG6L &= lcon(0x70);
              RG1L = unsignedlong(RG6W);
              if (RG1L == 0)
                //m_21FB8:
                sar8(location(DOWNCAST(unk, xp_b4)), lcon(0x3));
              else
                mov8(location(DOWNCAST(unk, xp_b4)), lcon(0xffffffff));
            }
          }
        }
      }
    }

    //m_21FBE:
    RG1P = DOWNCAST(unk, xp_b4);
    RG1W = signedword(byte_at(RG1P));
    vo_e8.poke16(RG1W);
    RG1P = DOWNCAST(unk, xp_b4);
    RG1Blo = byte_at(RG1P, lcon(0x10)) & 2;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 0)
      vo_e8.inc16();
    RG4L = signedlong(vo_e8.peek16());
    RG1L = signedlong(DM2_MAX(vw_d4, RG4W));
    vw_d4 = RG1W;
    xp_78 += 4; // DANGER
    xp_b4 += lcon(0x16); // DANGER
    vo_10c.inc16();
  }
  //m_22602:
  for (;;)
  {
    RG1W = ddat.v1d3248;
    vw_c0 = RG1W;
    RG1Blo = vb_140 & lcon(0x1);
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 0)
    {
      xp_78 = UPCAST(unk, DOWNCAST(unk, xp_68));
      xp_b4 = UPCAST(unk, DOWNCAST(s_len8, argp_s23));
      RG4L = 0;
      vo_10c.poke16(RG4W);
      //m_2310B:
      for (;;)
      {
        RG1W = vo_10c.peek16();
        if (RG1W >= argw0)
          break;
        //m_22636:
        RG1P = DOWNCAST(unk, xp_b4);
        RG1W = signedword(byte_at(RG1P));
        RG3W = vo_e8.peek16();
        if (RG1W < RG3W)
        {
          vo_10c.inc16();
          xp_b4 += lcon(0x16); // DANGER
          xp_78 += 4; // DANGER
          continue;
        }
        RG1P = DOWNCAST(unk, xp_b4);
        RG1Blo = byte_at(RG1P, lcon(0x10)) & lcon(0x1);
        RG1L = unsignedlong(RG1Blo);
        if (RG1L == 0)
        {
          vo_10c.inc16();
          xp_b4 += lcon(0x16); // DANGER
          xp_78 += 4; // DANGER
          continue;
        }
        RG1P = DOWNCAST(unk, xp_b4);
        RG1Blo = byte_at(RG1P, lcon(0x7));
        if (mkub(RG1Blo) <= mkub(lcon(0x1b)))
        {
          bool skip00614 = false;
          bool skip00613 = false;
          bool skip00612 = false;
          bool skip00611 = false;
          bool skip00610 = false;
          bool skip00609 = false;
          bool skip00607 = false;
          bool skip00604 = false;
          bool skip00616 = false;
          bool skip00618 = false;
          bool skip00626 = false;
          bool skip00628 = false;
          bool skip00630 = false;
          RG1L = unsignedlong(RG1Blo);
          switch (RG1L)
          {
            case 0:
              vl_a4 = 1;
              break;

            case 1:
              //m_22ED2:
              RG1W = vw_c0;
              if (RG1W != vw_110)
                skip00612 = true;
              else
              {
                RG1W = vo_f4.peek16();
                if (RG1W != vw_114)
                  skip00612 = true;
                else
                {
                  RG1W = vw_12c;
                  if (RG1W == vw_118)
                    vl_a4 = 1;
                  else
                    skip00612 = true;
                }
              }
              break;

            case 2:
              //m_22677:
              RG1P = DOWNCAST(unk, xp_b4);
              RG1W = word_at(RG1P, lcon(0x8));
              if (RG1UW < mkuw(2))
              {
                //m_22695:
                if (RG1W == 0)
                {
                  RG1W = ddat.v1d3248;
                  if (RG1W == ddat.v1e08d6)
                  {
                    RG1W = vo_f4.peek16();
                    if (RG1W == ddat.v1e08d8)
                    {
                      RG1W = vw_12c;
                      flag = RG1W != ddat.v1e08d4;
                      skip00616 = true;
                    }
                  }
                }
              }
              else
              {
                if (RG1UW <= mkuw(2))
                {
                  //m_226DD:
                  RG3L = signedlong(vw_12c);
                  RG2L = signedlong(vo_f4.peek16());
                  RG1L = signedlong(DM2_CALC_VECTOR_DIR(ddat.v1e08d8, ddat.v1e08d4, RG2W, RG3W) & lcon(0xffff));
                  RG4L = signedlong(ddat.v1e08da);
                  if (RG1L == RG4L)
                  {
                    pf_02 = DM2_1BA1B;
                    RG3L = signedlong(vw_12c);
                    RG2L = signedlong(vo_f4.peek16());
                    RG4L = signedlong(ddat.v1e08d4);
                    RG1L = DM2_19f0_0207(signedlong(ddat.v1e08d8), RG4L, RG2L, RG3L, pf_02);
                    vo_fc.poke16(RG1W);
                    if (RG1W > 0)
                    {
                      RG4P = DOWNCAST(unk, xp_b4);
                      if (RG1W <= word_at(RG4P, lcon(0xa)))
                        vl_a4 = 1;
                    }
                  }
                }
                else
                {
                  if (RG1W == lcon(0x4))
                  {
                    //m_2275C:
                    RG3L = signedlong(ddat.v1e08d4);
                    RG2L = signedlong(ddat.v1e08d8);
                    RG4L = signedlong(vw_12c);
                    RG1W = DM2_CALC_SQUARE_DISTANCE(vo_f4.peek16(), RG4W, RG2W, RG3W);
                    RG4P = DOWNCAST(unk, xp_b4);
                    if (RG1W == word_at(RG4P, lcon(0xa)))
                    {
                      bool skip00603 = false;
                      RG4L = signedlong(vo_f4.peek16());
                      RG1L = signedlong(ddat.v1e08d8);
                      if (RG4L == RG1L)
                        skip00603 = true;
                      else
                      {
                        RG4L = signedlong(vw_12c);
                        RG1L = signedlong(ddat.v1e08d4);
                        if (RG4L != RG1L)
                        {
                          RG1L = 1;
                          flag = RG1L == 0;
                        }
                        else
                          skip00603 = true;
                      }

                      if (skip00603)
                      {
                        //m_227B0:
                        pf_03 = DM2_1c9a_1bae;
                        RG3L = signedlong(vw_12c);
                        RG2L = signedlong(vo_f4.peek16());
                        RG4L = signedlong(ddat.v1e08d4);
                        RG1L = DM2_19f0_0207(signedlong(ddat.v1e08d8), RG4L, RG2L, RG3L, pf_03);
                        if (RG1W != 0)
                          //m_227E3:
                          RG1L = 0;
                        else
                          RG1L = 1;
                        flag = RG1L == 0;
                      }
                      skip00618 = true;
                    }
                  }
                }
              }
              break;

            case 3:
              //m_22E2B:
              RG1W = vw_c0;
              if (RG1W != vql_11c.peek16())
                skip00611 = true;
              else
              {
                RG1W = vo_f4.peek16();
                if (RG1W != vql_134.peek16())
                  skip00611 = true;
                else
                {
                  RG1W = vw_12c;
                  if (RG1W == vw_d0)
                    vl_a4 = 1;
                  else
                    skip00611 = true;
                }
              }
              break;

            case 4:
              //m_22AC8:
              RG1W = vw_c0;
              if (RG1W == ddat.v1e08d6)
              {
                RG3L = signedlong(ddat.v1e08d4);
                RG2L = signedlong(ddat.v1e08d8);
                RG4L = signedlong(vw_12c);
                RG1L = signedlong(DM2_CALC_SQUARE_DISTANCE(vo_f4.peek16(), RG4W, RG2W, RG3W));
                flag = RG1L != 2;
                skip00616 = true;
              }
              break;

            case 5:
              //m_22B2E:
              RG1P = DOWNCAST(unk, xp_b4);
              if (word_at(RG1P + lcon(0xc)) != lcon(0xffffffff))
                skip00609 = true;
              else
              {
                if (byte_at(RG1P + lcon(0x6)) > bcon(0x0))
                {
                  //m_22B49:
                  RG1W = ddat.v1d62ec & lcon(0x1);
                  RG1L = unsignedlong(RG1W);
                  if (RG1L == 0)
                    //m_22B6F:
                    ddat.v1d62ec >>= bcon(0x1);
                  else
                  {
                    RG1L = unsignedlong(ddat.v1d62ec) / 2;
                    RG1Bhi ^= lcon(0xffffffb4);
                    ddat.v1d62ec = RG1W;
                  }
                  //m_22B76:
                  RG1W = ddat.v1d62ec & lcon(0x7);
                  RG1L = unsignedlong(RG1W);
                  RG1Blo = RG1L == 0 ? 1 : 0;
                  RG1L = unsignedlong(RG1Blo);
                }
                else
                  RG1L = 1;
                //m_22B8D:
                if (RG1L != 0)
                {
                  RG1W = vw_c0;
                  if (RG1W != ddat.v1e08d6)
                  {
                    //m_22BCA:
                    RG4L = signedlong(vw_c0) << 4;
                    RG1P = ddat.v1e03c8;
                    RG4W = word_at(RG1P, RG4L + lcon(0x8)) & lcon(0x3f);
                    RG2L = signedlong(ddat.v1e08d6) << 4;
                    RG1W = word_at(RG1P, RG2L + lcon(0x8)) & lcon(0x3f);
                    RG1Blo = RG4W != RG1W ? 1 : 0;
                    RG1L = unsignedlong(RG1Blo);
                    vql_a8.poke32(RG1L);
                    RG1L = signedlong(ddat.v1e08d6) << 4;
                    RG4P = ddat.v1e03c8;
                    RG2L = unsignedlong(byte_at(RG4P, RG1L + lcon(0x7)));
                    RG2W += ddat.v1e08d4;
                    RG3L = signedlong(RG2W);
                    RG1Blo = byte_at(RG4P, RG1L + lcon(0x6));
                    RG1W = unsignedword(RG1Blo) + ddat.v1e08d8;
                    RG2L = signedlong(RG1W);
                    RG1P = RG4P + (signedlong(vw_c0) << 4);
                    RG4W = unsignedword(byte_at(RG1P, lcon(0x7))) + vw_12c;
                    vql_104.poke16(RG4W);
                    vl_84 = signedlong(vql_104.peek16());
                    RG1Blo = byte_at(RG1P, lcon(0x6));
                    RG1W = unsignedword(RG1Blo);
                    RG1W += vo_f4.peek16();
                    RG1L = signedlong(RG1W);
                    RG4L = vl_84;
                    RG1L = signedlong(DM2_CALC_SQUARE_DISTANCE(RG1W, RG4W, RG2W, RG3W));
                    RG3Blo = vql_a8.peek8();
                    RG1L <<= RG3Blo;
                  }
                  else
                  {
                    RG3L = signedlong(ddat.v1e08d4);
                    RG2L = signedlong(ddat.v1e08d8);
                    RG4L = signedlong(vw_12c);
                    RG1L = signedlong(DM2_CALC_SQUARE_DISTANCE(vo_f4.peek16(), RG4W, RG2W, RG3W));
                  }
                  //m_22C6D:
                  vl_4c = RG1L;
                  RG1L = vl_4c;
                  vo_fc.poke16(RG1W);
                  RG1P = DOWNCAST(unk, xp_78);
                  RG1W = signedword(byte_at(RG1P, lcon(0x3)));
                  if (RG1W < vo_fc.peek16())
                  {
                    bool skip00624 = false;
                    RG1W = vw_c0;
                    if (RG1W != ddat.v1e08d6)
                      skip00624 = true;
                    else
                    {
                      RG1L = signedlong(vo_fc.peek16());
                      if (RG1L >= lcon(0x5))
                        RG1L = 1;
                      else
                      {
                        bool skip00608 = false;
                        RG4L = signedlong(vo_f4.peek16());
                        RG1L = signedlong(ddat.v1e08d8);
                        if (RG4L == RG1L)
                          skip00608 = true;
                        else
                        {
                          RG1L = signedlong(vw_12c);
                          RG4L = signedlong(ddat.v1e08d4);
                          if (RG1L != RG4L)
                            RG1L = 1;
                          else
                            skip00608 = true;
                        }

                        if (skip00608)
                        {
                          //m_22CCF:
                          pf_12 = DM2_1c9a_1bae;
                          RG3L = signedlong(vw_12c);
                          RG2L = signedlong(vo_f4.peek16());
                          RG4L = signedlong(ddat.v1e08d4);
                          RG1L = DM2_19f0_0207(signedlong(ddat.v1e08d8), RG4L, RG2L, RG3L, pf_12);
                          if (RG1W != 0)
                            //m_22D02:
                            RG1L = 0;
                          else
                            RG1L = 1;
                        }
                      }
                      //m_22D04:
                      if (RG1L != 0)
                        skip00624 = true;
                    }

                    if (skip00624)
                    {
                      //m_22D0C:
                      vl_a4 = 1;
                      RG1Blo = vo_fc.peek8();
                      RG4P = DOWNCAST(unk, xp_78);
                      mov8(location(RG4P + lcon(0x3)), RG1Blo);
                    }
                  }
                }
              }
              break;

            case 6:
              //m_2284C:
              RG1W = vw_c0;
              if (RG1W == ddat.v1e08d6)
              {
                RG1P = DOWNCAST(unk, xp_b4);
                RG1W = word_at(RG1P, lcon(0xa));
                s350.v1e0578 &= RG1W;
                put16(parw05, lcon(0xffffffff));
                put16(parw04, ddat.v1e08d4);
                RG3L = signedlong(ddat.v1e08d8);
                RG2L = signedlong(vw_12c);
                RG4L = signedlong(vo_f4.peek16());
                RG1L = 1;
                RG1L = DM2_19f0_0891(RG1L, RG4L, RG2L, RG3L, parw04, parw05);
                vl_a4 = RG1L;
                RG1L = vl_c4;
                s350.v1e0578 = RG1W;
              }
              break;

            case 7:
              //m_227F2:
              RG1W = vw_c0;
              if (RG1W == ddat.v1e08d6)
              {
                RG1P = DOWNCAST(unk, xp_b4);
                RG1W = word_at(RG1P, lcon(0xa));
                s350.v1e0578 &= RG1W;
                put16(parw05, lcon(0xffffffff));
                put16(parw04, ddat.v1e08d4);
                RG3L = signedlong(ddat.v1e08d8);
                skip00604 = true;
              }
              break;

            case 8:
            case 9:
              //m_22895:
              RG1P = DOWNCAST(unk, xp_b4);
              RG1W = signedword(byte_at(RG1P));
              if (RG1W == vo_e8.peek16())
              {
                RG1P = DOWNCAST(unk, xp_b4);
                RG1W = word_at(RG1P, lcon(0xa));
                s350.v1e0578 &= RG1W;
                RG1P = DOWNCAST(unk, xp_b4);
                RG1Blo = byte_at(RG1P + lcon(0x7)) == bcon(0x9) ? 1 : 0;
                RG1L = unsignedlong(RG1Blo);
                i32 parl08 = RG1L;
                put16(parw06, vw_12c);
                RG3L = signedlong(vo_f4.peek16());
                RG2P = DOWNCAST(unk, xp_b4) + 2;
                RG1L = signedlong(word_at(DOWNCAST(unk, xp_b4), lcon(0x8)));
                RG4P = DOWNCAST(unk, xp_bc);
                RG1L = DM2_19f0_05e8(RG1L, RG4P, RG2P, RG3L, parw06, lcon(0xffffffff), parl08);
                vl_a4 = RG1L;
                if (RG1L == 0)
                {
                  RG1L = vl_c4;
                  s350.v1e0578 = RG1W;
                }
                else
                {
                  bool skip00620 = false;
                  RG1W = ddat.v1e08a8 & lcon(0x1f);
                  RG4P = DOWNCAST(unk, xp_b4);
                  and8(location(RG4P + lcon(0x4)), lcon(0xffffffe0));
                  RG3W = word_at(RG4P, lcon(0x4));
                  RG3L |= RG1L;
                  mov16(location(RG4P + lcon(0x4)), RG3W);
                  RG1W = ddat.v1e08aa & lcon(0x1f);
                  RG2L = RG3L & lcon(0xfffffc1f);
                  RG1L <<= bcon(0x5);
                  RG3L = RG2L | RG1L;
                  mov16(location(RG4P + lcon(0x4)), RG3W);
                  RG1W = ddat.v1e08ac & lcon(0x3f);
                  and8(location(RG4P + lcon(0x5)), lcon(0x3));
                  RG1L <<= bcon(0xa);
                  or16(location(RG4P + lcon(0x4)), RG1W);
                  RG1W = ddat.v1e08a8;
                  if (RG1W == vo_f4.peek16())
                  {
                    RG1W = ddat.v1e08aa;
                    if (RG1W == vw_12c)
                    {
                      RG1W = ddat.v1e08ac;
                      if (RG1W == vw_c0)
                      {
                        RG1Blo = vo_e8.peek8();
                        skip00620 = true;
                      }
                    }
                  }

                  if (!skip00620)
                  {
                    //m_2296B:
                    RG1P = DOWNCAST(unk, xp_8c) + 4 * signedlong(ddat.v1e08ac);
                    RG4L = signedlong(ddat.v1e08a8) << 7;
                    RG1P = pointer_at(RG1P);
                    RG4P = RG1P + RG4L;
                    RG1L = signedlong(ddat.v1e08aa);
                    RG1Blo = byte_at(RG4P, 4 * RG1L) - 1;
                  }

                  //m_22996:
                  RG4P = DOWNCAST(unk, xp_b4);
                  mov8(location(RG4P + lcon(0x6)), RG1Blo);
                  RG1L = vl_c4;
                  s350.v1e0578 = RG1W;
                }
              }
              break;

            case 10:
              //m_22A70:
              RG1W = vw_c0;
              if (RG1W == ddat.v1e08d6)
              {
                RG1L = signedlong(s350.v1e056e);
                RG1Blo = table1d613a[RG1L] & lcon(0x3);
                RG1L = unsignedlong(RG1Blo);
                if (RG1L == 0)
                {
                  RG1P = DOWNCAST(unk, xp_b4);
                  RG1W = word_at(RG1P, lcon(0xa));
                  s350.v1e0578 &= RG1W;
                  RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
                  RG1W = word_at(RG1P, lcon(0xe));
                  RG1L <<= bcon(0x6);
                  RG1UW >>= 14;
                  RG1L = signedlong(RG1W);
                  put16(parw05, RG1W);
                  RG1L = lcon(0xffffffff);
                  put16(parw04, RG1W);
                  RG3L = RG1L;
                  skip00604 = true;
                }
              }
              break;

            case 11:
              skip00609 = true;
              break;

            case 12:
              //m_22D79:
              RG1P = DOWNCAST(unk, xp_78);
              RG1W = signedword(byte_at(RG1P, 2));
              if (RG1W != vw_c0)
                skip00610 = true;
              else
              {
                RG1P = DOWNCAST(unk, xp_78);
                RG1W = signedword(byte_at(RG1P, lcon(0x3)));
                vql_104.poke32(RG1L);
                RG1P = DOWNCAST(unk, xp_78);
                RG1W = signedword(byte_at(RG1P, lcon(0x1)));
                RG3L = signedlong(RG1W);
                RG1P = DOWNCAST(unk, xp_78);
                RG1W = signedword(byte_at(RG1P));
                RG2L = signedlong(RG1W);
                RG4L = signedlong(vw_12c);
                vo_fc.poke16(DM2_CALC_SQUARE_DISTANCE(vo_f4.peek16(), RG4W, RG2W, RG3W));
                RG1L = vql_104.peek32();
                if (RG1W > vo_fc.peek16())
                  skip00626 = true;
                else
                  skip00610 = true;
              }
              break;

            case 13:
              //m_22E70:
              RG1P = DOWNCAST(unk, xp_b4);
              if (word_at(RG1P + lcon(0xc)) != lcon(0xffffffff))
                skip00609 = true;
              else
              {
                if (RG3W > 0)
                {
                  if (byte_at(RG1P + bcon(0x6)) <= 0)
                    vl_a4 = 1;
                  else
                  {
                    RG1W = ddat.v1d62ec & lcon(0x1);
                    RG1L = unsignedlong(RG1W);
                    if (RG1L == 0)
                      //m_22EB7:
                      ddat.v1d62ec >>= bcon(0x1);
                    else
                    {
                      RG1L = unsignedlong(ddat.v1d62ec) / 2;
                      RG1Bhi ^= lcon(0xffffffb4);
                      ddat.v1d62ec = RG1W;
                    }
                    //m_22EBE:
                    RG1W = ddat.v1d62ec & lcon(0x7);
                    RG1L = unsignedlong(RG1W);
                    flag = RG1L != 0;
                    skip00616 = true;
                  }
                }
              }
              break;

            case 14:
              //m_22F17:
              RG1P = DOWNCAST(unk, xp_b4);
              RG1W = word_at(RG1P, lcon(0x8));
              vo_fc.poke16(RG1W);
              if (RG1W >= 0)
              {
                //m_22F3B:
                if (RG1W > 0)
                {
                  RG1W = DM2_RAND16(vo_fc.peek16());
                  flag = RG1W != 0;
                  skip00616 = true;
                }
              }
              else
              {
                RG1W = DM2_RAND16(RG1W);
                flag = RG1W == 0;
                skip00618 = true;
              }
              break;

            case 15:
            case 16:
              //m_22B04:
              RG1L = signedlong(word_at(DOWNCAST(unk, xp_b4), lcon(0x8)));
              put16(parw11, RG1W);
              put16(parw10, lcon(0xffffffff));
              RG2L = signedlong(vw_12c);
              parl09 = RG2L;
              RG4L = signedlong(vo_f4.peek16());
              RG1L = DM2_19f0_2165(0, RG4L, RG2L, RG4L, parl09, parw10, parw11);
              skip00607 = true;
            case 17:
            case 18:
            case 19:
            case 22:
            case 26:
              break;

            case 20:
              //m_22F77:
              RG1W = vo_f4.peek16();
              if (RG1W != vql_04.peek16())
                skip00613 = true;
              else
              {
                RG1W = vw_12c;
                if (RG1W == vql_00.peek16())
                {
                  //m_22F96:
                  RG1L = 0;
                  skip00628 = true;
                }
                else
                  skip00613 = true;
              }
              break;

            case 21:
              //m_22F54:
              put16(parw14, lcon(0xffffffff));
              RG2L = signedlong(vw_12c);
              put16(parw13, RG2W);
              RG4L = signedlong(vo_f4.peek16());
              RG1L = 0;
              RG1Blo = byte_at(DOWNCAST(unk, xp_b4), lcon(0x8));
              RG1L = DM2_CREATURE_GO_THERE(RG1L, RG4L, RG2L, RG4L, parw13, parw14);
              skip00607 = true;
              break;

            case 23:
              //m_22FF8:
              if ((ddat.v1e08ae & wcon(0x10)) != 0)
              {
                RG3L = signedlong(vw_12c);
                RG2L = signedlong(vo_f4.peek16());
                RG1W = vw_f8;
                if (RG1W == lcon(0xffffffff))
                  RG1W = ddat.v1e0258;
                RG4L = signedlong(RG1W);
                DM2_ADD_BACKGROUND_LIGHT_FROM_TILE(signedlong(vo_e8.peek16()), RG4L, RG2L, RG3L, lcon(0x3));
              }
              break;

            case 24:
              //m_229A1:
              if ((ddat.v1e08ae & wcon(0x10)) != 0)
              {
                RG4L = signedlong(word_at(DOWNCAST(unk, xp_b4), lcon(0xa)));
                RG1L = DM2_1c9a_1b16(signedlong(word_at(DOWNCAST(unk, xp_b4), lcon(0x8))), RG4L);
                vo_fc.poke16(RG1W);
                RG4L = signedlong(RG1W);
                if (RG4L != lcon(0xffffffff))
                {
                  RG4P = DOWNCAST(unk, xp_78);
                  RG4W = signedword(byte_at(RG4P, lcon(0x3)));
                  if (RG1W < RG4W)
                  {
                    bool skip00606 = false;
                    if (RG1W == 0)
                      skip00606 = true;
                    else
                    {
                      RG1L = signedlong(byte_at(RG1P = DOWNCAST(unk, xp_78), lcon(0x3)));
                      if (RG1L < lcon(0x7f))
                        skip00606 = true;
                      else
                      {
                        RG1P = DOWNCAST(unk, xp_b4);
                        if (byte_at(RG1P) != 0)
                        {
                          RG1Blo = vo_fc.peek8();
                          RG4P = DOWNCAST(unk, xp_78);
                          mov8(location(RG4P + lcon(0x3)), RG1Blo);
                        }
                        else
                          skip00606 = true;
                      }
                    }

                    if (skip00606)
                    {
                      //m_22A00:
                      vl_a4 = 1;
                      and8(location(DOWNCAST(unk, xp_b4) + lcon(0x10)), lcon(0xfffffffb));
                    }
                  }
                }
              }
              break;

            case 25:
              //m_22A21:
              if (vw_f8 != lcon(0xffffffff) && (ddat.v1e08ae & wcon(0x10)) != 0)
              {
                RG4L = unsignedlong(s350.v1e0582);
                RG1L = signedlong(CUTX16(DM2_1c9a_1a48(lcon(0x1), RG4L)));
                if (RG1L != lcon(0xffffffff))
                {
                  RG3L = signedlong(vw_f8) + 2 & lcon(0x3);
                  RG4L = lcon(0x1) << RG3Blo;
                  flag = (RG1L & RG4L) != 0;
                  skip00616 = true;
                }
              }
              break;

            case 27:
              //m_23035:
              RG4Blo = vo_e8.peek8() + 1;
              RG1W = vw_c0;
              RG2W = ddat.v1d62a4;
              if (RG1W != RG2W)
              {
                //m_2305F:
                RG3L = vql_f0.peek32();
                if (RG3W < 0 || RG3W != RG2W)
                  skip00630 = true;
                else
                {
                  RG1L = signedlong(vw_108) << 5;
                  RG2P = ddat.v1e08cc + RG1L;
                  RG1L = signedlong(vw_128);
                  skip00614 = true;
                }
              }
              else
              {
                RG1L = signedlong(vo_f4.peek16()) << 5;
                RG2P = ddat.v1e08cc + RG1L;
                RG1L = signedlong(vw_12c);
                skip00614 = true;
              }
              break;

            default: throw(THROW_DMABORT);
          }

          if (skip00614)
          {
            //m_23080:
            mov8(location(RG2P + RG1L), RG4Blo);
            skip00630 = true;
          }

          if (skip00613)
          {
            //m_22F8F:
            RG1L = 1;
            skip00628 = true;
          }

          if (skip00612)
          {
            //m_22EF1:
            RG1L = vql_f0.peek32();
            if (RG1W == vw_110)
            {
              RG1W = vw_108;
              if (RG1W == vw_114)
              {
                RG1W = vw_128;
                flag = RG1W != vw_118;
                skip00616 = true;
              }
            }
          }

          if (skip00611)
          {
            //m_22E4A:
            RG1L = vql_f0.peek32();
            if (RG1W == vql_11c.peek16())
            {
              RG1W = vw_108;
              if (RG1W == vql_134.peek16())
              {
                RG1W = vw_128;
                flag = RG1W != vw_d0;
                skip00616 = true;
              }
            }
          }

          if (skip00610)
          {
            //m_22DC5:
            RG1P = DOWNCAST(unk, xp_78);
            RG1W = signedword(byte_at(RG1P, 2));
            if (RG1W == vql_f0.peek16())
            {
              RG1P = DOWNCAST(unk, xp_78);
              RG1W = signedword(byte_at(RG1P, lcon(0x3)));
              vql_104.poke32(RG1L);
              RG3L = signedlong(byte_at(DOWNCAST(unk, xp_78), lcon(0x1)));
              RG2L = signedlong(byte_at(DOWNCAST(unk, xp_78)));
              RG4L = signedlong(vw_128);
              RG1W = DM2_CALC_SQUARE_DISTANCE(vw_108, RG4W, RG2W, RG3W);
              vo_fc.poke16(RG1W);
              if (RG1W < vql_104.peek16())
                skip00626 = true;
            }
          }

          if (skip00609)
          {
            bool skip00580 = false;
            //m_22D18:
            RG1P = DOWNCAST(unk, xp_78);
            RG1W = signedword(byte_at(RG1P, 2));
            if (RG1W != vw_c0)
              skip00580 = true;
            else
            {
              RG1P = DOWNCAST(unk, xp_78);
              RG1W = signedword(byte_at(RG1P));
              if (RG1W != vo_f4.peek16())
                skip00580 = true;
              else
              {
                RG1P = DOWNCAST(unk, xp_78);
                RG1W = signedword(byte_at(RG1P, lcon(0x1)));
                if (RG1W == vw_12c)
                  vl_a4 = 1;
                else
                  skip00580 = true;
              }
            }

            if (skip00580)
            {
              //m_22D45:
              RG1P = DOWNCAST(unk, xp_78);
              RG1W = signedword(byte_at(RG1P, 2));
              if (RG1W == vql_f0.peek16())
              {
                RG1P = DOWNCAST(unk, xp_78);
                RG1W = signedword(byte_at(RG1P));
                if (RG1W == vw_108)
                {
                  RG1P = DOWNCAST(unk, xp_78);
                  RG1W = signedword(byte_at(RG1P, lcon(0x1)));
                  flag = RG1W != vw_128;
                  skip00616 = true;
                }
              }
            }
          }

          if (skip00607)
            //m_22B26:
            vl_a4 = RG1L;

          if (skip00604)
          {
            //m_22828:
            RG2L = signedlong(vw_12c);
            RG4L = signedlong(vo_f4.peek16());
            RG1L = 0;
            RG1L = DM2_19f0_0891(RG1L, RG4L, RG2L, RG3L, parw04, parw05);
            vl_a4 = RG1L;
            RG1L = vl_c4;
            s350.v1e0578 = RG1W;
          }

          if (skip00630)
          {
            bool skip00581 = false;
            //m_23086:
            RG4Blo = vo_e8.peek8() + 1;
            RG1W = vw_c0;
            RG3W = ddat.v1d62a6;
            if (RG1W != RG3W)
            {
              //m_230AF:
              RG1L = vql_f0.peek32();
              if (RG1W >= 0 && RG1W == RG3W)
              {
                RG1L = signedlong(vw_108) << 5;
                RG2P = ddat.v1e08c8 + RG1L;
                RG1L = signedlong(vw_128);
                skip00581 = true;
              }
            }
            else
            {
              RG2P = ddat.v1e08c8 + (signedlong(vo_f4.peek16()) << 5);
              RG1L = signedlong(vw_12c);
              skip00581 = true;
            }

            if (skip00581)
              //m_230D0:
              mov8(location(RG2P + RG1L), RG4Blo);
          }

          if (skip00628)
          {
            //m_22F98:
            if (RG1L != 0)
            {
              RG1L = DM2_19f0_050f();
              RG4Blo = RG1W != wcon(0xfffe) ? 1 : 0;
              RG4L = unsignedlong(RG4Blo);
              if (RG4L != 0)
              {
                RG4L = signedlong(word_at(DOWNCAST(unk, xp_b4), lcon(0x8)));
                RG1L = unsignedlong(RG1W);
                vl_6c = RG1L;
                RG1L = DM2_CREATURE_CAN_HANDLE_IT(RG1L, RG4L);
                if (RG1L != 0)
                {
                  RG3L = signedlong(word_at(DOWNCAST(unk, xp_b4), lcon(0xa)));
                  RG2L = signedlong(vw_12c);
                  RG4L = signedlong(vo_f4.peek16());
                  DM2_1c9a_19d4(vl_6c, RG4L, RG2L, RG3L);
                  vl_a4 = 1;
                }
              }
            }
          }

          if (skip00626)
          {
            //m_22E0E:
            vl_a4 = 1;
            RG1Blo = vo_fc.peek8();
            RG4P = DOWNCAST(unk, xp_78);
            mov8(location(RG4P + lcon(0x3)), RG1Blo);
            if (RG1Blo == 0)
              and8(location(DOWNCAST(unk, xp_b4) + lcon(0x10)), lcon(0xfffffffb));
          }

          if (skip00618)
          {
            //m_227E7:
            if (!flag)
              vl_a4 = 1;
          }

          if (skip00616)
          {
            //m_226CB:
            if (!flag)
              vl_a4 = 1;
          }
        }

        //m_230D6:
        if (vl_a4 != 0)
        {
          if (vl_50 != 0)
          {
            RG1W = ddat.v1d3248;
            if (RG1W != vw_c8 && RG1W != ddat.v1e08d6)
            {
              RG4L = 0;
              vl_a4 = RG4L;
              vo_10c.inc16();
              xp_b4 += lcon(0x16); // DANGER
              xp_78 += 4; // DANGER
              continue;
            }
          }
          break;
        }
        vo_10c.inc16();
        xp_b4 += lcon(0x16); // DANGER
        xp_78 += 4; // DANGER
      }
    }
    //m_2311B:
    RG4W = vo_e8.peek16();
    RG4W += ddat.v1e08c4;
    if (RG4W <= vw_d4)
    {
      RG1P = DOWNCAST(unk, xp_90) + 4 * signedlong(RG6W);
      RG2Blo = vo_f4.peek8();
      mov8(location(RG1P), RG2Blo);
      RG2Blo = CUTX8(vw_12c);
      mov8(location(RG1P + lcon(0x1)), RG2Blo);
      RG2Blo = CUTX8(vw_c0);
      mov8(location(RG1P + 2), RG2Blo);
      RG6L++;
      RG6L = unsignedlong(RG6Blo);
      RG1L = signedlong(RG4W);
      vba_08[RG1L]++;
    }
    s_31_ac.b_00 = RG4Blo;
    RG1Blo = CUTX8(vw_f8);
    s_31_ac.b_01 = RG1Blo;
    RG1W = vw_dc;
    RG1W &= lcon(0x3f);
    s_31_ac.w_02 &= lcon(0x3ff);
    RG1L <<= bcon(0xa);
    s_31_ac.w_02 |= RG1W;
    RG1W = vw_100;
    RG1W &= lcon(0x1f);
    s_31_ac.w_02 &= lcon(0xffe0);
    RG2W = s_31_ac.w_02 | RG1W; // adj, modified
    s_31_ac.w_02 = RG2W;
    RG1W = vw_ec;
    RG1W &= lcon(0x1f);
    RG3L = RG2L & lcon(0xfffffc1f);
    s_31_ac.w_02 = RG3W;
    RG1L <<= bcon(0x5);
    RG4L = RG3L | RG1L;
    s_31_ac.w_02 = RG4W;
    RG1P = DOWNCAST(unk, xp_bc) + (signedlong(vo_f4.peek16()) << 7) + 4 * signedlong(vw_12c);
    RG4L = s_31_ac.getlong();
    mov32(location(RG1P), RG4L);
    //m_23A30:
    for (;;)
    {
      if (vl_a4 != 0)
      {
        RG1P = DOWNCAST(unk, xp_b4);
        RG1Blo = byte_at(RG1P, lcon(0x10)) & lcon(0x10);
        RG1L = unsignedlong(RG1Blo);
        if (RG1L == 0)
        {
          RG1P = DOWNCAST(unk, xp_b4);
          RG1Blo = byte_at(RG1P, lcon(0x10)) & lcon(0x20);
          RG1L = unsignedlong(RG1Blo);
          if (RG1L == 0)
          {
            //m_23A9C:
            RG1W = vo_f4.peek16();
            RG1W &= lcon(0x1f);
            RG4P = DOWNCAST(unk, xp_b4);
            and8(location(RG4P + 2), lcon(0xffffffe0));
            RG2W = word_at(RG4P, 2);
            RG2L |= RG1L;
            RG1W = vw_12c;
            RG1W &= lcon(0x1f);
            RG3L = RG2L & lcon(0xfffffc1f);
            RG1L <<= bcon(0x5);
            RG2L = RG3L | RG1L;
            mov16(location(RG4P + 2), RG2W);
            RG1W = vw_c0;
          }
          else
          {
            RG1W = ddat.v1e08d8 & lcon(0x1f);
            RG4P = DOWNCAST(unk, xp_b4);
            and8(location(RG4P + 2), lcon(0xffffffe0));
            RG2W = word_at(RG4P, 2);
            RG2L |= RG1L;
            mov16(location(RG4P + 2), RG2W);
            RG1W = ddat.v1e08d4 & lcon(0x1f);
            RG3L = RG2L & lcon(0xfffffc1f);
            RG1L <<= bcon(0x5);
            RG2L = RG3L | RG1L;
            mov16(location(RG4P + 2), RG2W);
            RG1W = ddat.v1e08d6;
          }
          //m_23ACD:
          RG1W &= lcon(0x3f);
          and8(location(RG4P + lcon(0x3)), lcon(0x3));
          RG1L <<= bcon(0xa);
          or16(location(RG4P + 2), RG1W);
          if (vb_144 == 0)
          {
            //m_23B04:
            RG1Blo = vo_e8.peek8();
            mov8(location(DOWNCAST(unk, xp_b4) + lcon(0x6)), RG1Blo);
          }
          else
          {
            RG1Blo = vo_e8.peek8() - 1;
            mov8(location(DOWNCAST(unk, xp_b4) + lcon(0x6)), RG1Blo);
            RG1W = vw_100;
            vo_f4.poke16(RG1W);
            RG1W = vw_ec;
            vw_12c = RG1W;
            RG1W = vw_dc;
            vw_c0 = RG1W;
          }
          //m_23B0D:
          RG1W = vo_f4.peek16();
          RG1W &= lcon(0x1f);
          RG4P = DOWNCAST(unk, xp_b4);
          and8(location(RG4P + lcon(0x4)), lcon(0xffffffe0));
          RG2W = word_at(RG4P, lcon(0x4));
          RG2L |= RG1L;
          RG1W = vw_12c;
          RG1W &= lcon(0x1f);
          RG3L = RG2L & lcon(0xfffffc1f);
          RG1L <<= bcon(0x5);
          RG2L = RG3L | RG1L;
          mov16(location(RG4P + lcon(0x4)), RG2W);
          RG1W = vw_c0;
          RG1W &= lcon(0x3f);
          and8(location(RG4P + lcon(0x5)), lcon(0x3));
          RG1L <<= bcon(0xa);
          or16(location(RG4P + lcon(0x4)), RG1W);
        }
        //m_23B4D:
        RG1W = vo_10c.peek16();
        vw_d8 = RG1W;
        if (RG1W == 0)
        {
          RG1P = DOWNCAST(unk, xp_b4);
          RG1Blo = byte_at(RG1P, lcon(0x10)) & lcon(0x4);
          RG1L = unsignedlong(RG1Blo);
          if (RG1L == 0)
            jump L_fin1;
          RG1P = DOWNCAST(unk, xp_b4);
          RG1W = signedword(byte_at(RG1P));
          if (RG1W <= vo_e8.peek16())
            jump L_fin1;
        }
        //m_23B7D:
        RG1W = vo_10c.peek16() + 1;
        argw0 = RG1W;
        //m_23B88:
        for (;;)
        {
          RG2W = vo_10c.peek16() - 1;
          vo_10c.poke16(RG2W);
          RG4L = signedlong(vo_10c.peek16());
          RG1P = DOWNCAST(s_len8, argp_s23) + 22 * RG4L;
          if (byte_at(RG1P + lcon(0x1)) >= bcon(0x0))
            break;
          if (RG2W == 0)
            jump L_fin1;
        }
        //m_23BBB:
        RG1P = DOWNCAST(unk, xp_b4);
        RG1Blo = byte_at(RG1P, lcon(0x10)) & lcon(0x4);
        RG1L = unsignedlong(RG1Blo);
        if (RG1L == 0)
        {
          RG1L = RG2L + 1;
          argw0 = RG1W;
        }
        RG1P = DOWNCAST(unk, xp_b4);
        RG1Blo = byte_at(RG1P, lcon(0x10)) & lcon(0x40);
        RG1L = unsignedlong(RG1Blo);
        if (RG1L == 0)
        {
          RG4L = 0;
          //m_23BE9:
          for (;;)
          {
            RG2L = signedlong(vo_10c.peek16());
            RG1P = DOWNCAST(s_len8, argp_s23) + 22 * RG2L;
            RG1W = signedword(byte_at(RG1P, lcon(0x1)));
            vo_f4.poke16(RG1W);
            if (RG1W > 0)
              RG4L += RG1L;
            RG2L = signedlong(vo_10c.peek16());
            RG1P = DOWNCAST(s_len8, argp_s23) + 22 * RG2L;
            vl_64 = signedlong(byte_at(RG1P));
            vl_84 = signedlong(vo_e8.peek16());
            RG2L = signedlong(RG4W) + vl_84;
            if (RG2L < vl_64)
            {
              RG2Blo = vo_e8.peek8() + RG4Blo;
              mov8(location(RG1P), RG2Blo);
            }
            RG3W = vo_10c.peek16() - 1;
            vo_10c.poke16(RG3W);
            if (RG3W < 0)
              break;
          }
        }
        //m_23C5B:
        RG3W = unsignedword(RG3Blo);
        vb_140 = RG3Bhi;
        RG4L = 0;
        vw_d4 = RG4W;
        vo_10c.poke16(RG4W);
        //m_23CEF:
        for (;;)
        {
          RG1W = vo_10c.peek16();
          if (RG1W >= argw0)
          {
            vl_a4 = 0;
            break;
          }
          //m_23C6F:
          RG4L = signedlong(vo_10c.peek16());
          RG1P = DOWNCAST(s_len8, argp_s23) + 22 * RG4L;
          RG4L = unsignedlong(byte_at(RG1P, lcon(0x10)));
          vo_fc.poke16(RG4W);
          RG4Blo = vb_140 | vo_fc.peek8();
          vb_140 = RG4Blo;
          if (vo_10c.peek16() == 0 || byte_at(RG1P + lcon(0x6)) < bcon(0x0))
          {
            RG4L = signedlong(vo_10c.peek16());
            RG1P = DOWNCAST(s_len8, argp_s23) + 22 * RG4L;
            RG4W = signedword(byte_at(RG1P));
            RG1W = vo_fc.peek16();
            RG1W &= 0x2;
            RG1L = signedlong(RG1W);
            if (RG1L != 0)
              RG4L++;
            RG1L = signedlong(vw_d4);
            RG2L = signedlong(RG4W);
            RG4L = RG1L;
            RG1L = signedlong(DM2_MAX(RG2W, RG4W));
            vw_d4 = RG1W;
          }
          //m_23CEB:
          vo_10c.inc16();
        }
      }
      //m_23D04:
      bool skipbreak19 = false;
      for (;;)
      {
        RG1W = vw_f8 + CUTX16(vl_e0);
        RG1W &= 0x3;
        vw_f8 = RG1W;
        //m_23D12:
        bool skipbreak16 = false;
        for (;;)
        {
          bool skip00573 = false;
          bool skip00574 = false;
          bool skip00579 = false;
          RG2L = unsignedlong(vw_130);
          flag = --RG2W == 0;
          vw_130 = RG2W;
          if (!flag)
          {
            RG1L = signedlong(RG6W) + 1 & lcon(0xff);
            RG4L = signedlong(vw_cc);
            RG1Blo = RG4L != RG1L ? 1 : 0;
            RG1L = unsignedlong(RG1Blo);
            if (RG1L != 0)
            {
              //m_222D8:
              RG1W = vw_dc;
              if (RG1W != ddat.v1d3248)
              {
                RG4L = signedlong(vw_dc);
                DM2_1c9a_0648(RG4L);
                RG1P = pointer_at(DOWNCAST(unk, xp_8c) + 4 * RG4L);
                xp_bc = UPCAST(unk, RG1P);
              }
              if (vl_94 == 0)
                skip00573 = true;
              else
              {
                RG1L = signedlong(vw_130);
                if (RG1L > 2)
                  skip00573 = true;
                else
                {
                  if (RG1L != 2)
                  {
                    //m_22323:
                    if (vl_74 == 0)
                      break;
                    vw_f8 = 4;
                  }
                  else
                  {
                    if (vl_70 == 0)
                      break;
                    vw_f8 = 5;
                  }
                  //m_22333:
                  RG1W = vw_100;
                  vo_f4.poke16(RG1W);
                  RG1W = vw_ec;
                  vw_12c = RG1W;
                  skip00574 = true;
                }
              }
            }
          }

          if (skip00573)
          {
            //m_22343:
            RG1L = signedlong(vw_f8);
            RG4W = vw_100;
            RG2W = table1d27fc[RG1L];
            RG4L += RG2L;
            vo_f4.poke16(RG4W);
            RG4W = vw_ec;
            RG3W = table1d2804[RG1L];
            RG4L += RG3L;
            vw_12c = RG4W;
            RG1W = vo_f4.peek16();
            if (RG1W >= 0 && RG1W < mapdat.map_width)
              //m_22383:
              RG1L = 0;
            else
              RG1L = 1;
            //m_22385:
            if (RG1L != 0)
              break;
            RG2W = vw_12c;
            if (RG2W < 0 || RG2W >= mapdat.map_height)
              RG1L = 1;
            if (RG1L != 0)
              break;
            skip00574 = true;
          }

          if (skip00574)
          {
            bool skip00575 = false;
            //m_223AB:
            RG4W = unsignedword(RG4Blo);
            ddat.v1e08b6 = RG4Bhi;
            RG1L = signedlong(vo_f4.peek16()) << 7;
            RG4P = DOWNCAST(unk, xp_bc) + RG1L;
            RG1L = signedlong(vw_12c);
            RG1Blo = byte_at(RG4P, 4 * RG1L);
            vb_144 = RG1Blo;
            if (RG1Blo == 0)
              skip00575 = true;
            else
            {
              RG1L = signedlong(vw_f8);
              if (RG1L >= lcon(0x4))
                skip00575 = true;
            }

            if (skip00575)
            {
              //m_223E0:
              RG1L = signedlong(vw_f8);
              vl_5c = RG1L;
              put16(parw01, RG1W);
              put16(parw00, vw_12c);
              RG3L = signedlong(vo_f4.peek16());
              RG2L = signedlong(vw_ec);
              RG4L = signedlong(vw_100);
              RG1L = DM2_CREATURE_GO_THERE(unsignedlong(vb_148), RG4L, RG2L, RG3L, parw00, parw01);
              if (RG1L != 0)
              {
                bool skip00577 = false;
                //m_2241A:
                vql_f0.poke32(lcon(0xffffffff));
                RG1W = ddat.v1e08ac;
                if (RG1W == ddat.v1d3248)
                {
                  //m_22564:
                  if (vl_5c >= lcon(0x4))
                    skip00577 = true;
                  else
                  {
                    if (ddat.v1e08b7 == 0)
                    {
                      bool skip00576 = false;
                      //m_22591:
                      RG1W = ddat.v1e08a8;
                      if (RG1W != vo_f4.peek16())
                        skip00576 = true;
                      else
                      {
                        RG1W = ddat.v1e08aa;
                        if (RG1W != vw_12c)
                          skip00576 = true;
                      }

                      if (skip00576)
                      {
                        //m_225A9:
                        RG1W = ddat.v1d3248;
                        vql_f0.poke32(RG1L);
                      }
                      skip00577 = true;
                    }
                    else
                    {
                      vql_f0.poke32(unsignedlong(ddat.v1e08b8.b_04));
                      RG1L = unsignedlong(ddat.v1e08b8.b_02);
                      vw_108 = RG1W;
                      RG1L = unsignedlong(ddat.v1e08b8.b_03);
                    }
                  }
                }
                else
                {
                  RG1L = signedlong(ddat.v1e08ac);
                  RG4P = DOWNCAST(unk, xp_8c) + 4 * RG1L;
                  if (long_at(RG4P) == 0)
                  {
                    RG1L <<= 4;
                    RG1W = word_at(ddat.v1e03c8, RG1L + lcon(0x8));
                    RG1L <<= bcon(0x5);
                    _RG1.ushiftr11();
                    RG1L = unsignedlong(RG1W) << 7;
                    RG4L = RG1L + lcon(0x80);
                    vl_9c = RG4L;
                    RG1L += lcon(0x148);
                    if (RG1L <= dm2_dballochandler.bigpool_available())
                    {
                      //m_22500:
                      RG2L = lcon(0x400);
                      RG4L = lcon(0x8000);
                      RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(vl_9c, true);
                      xp_bc = UPCAST(unk, RG1P);
                      vl_7c += vl_9c;
                    }
                    else
                    {
                      RG1W = /*TAG dbidx*/ dm2_dballochandler.DM2_dballoc_3e74_5888();
                      RG3W = RG1W;
                      dm2_dballochandler.DM2_ALLOC_CPXHEAP_MEM(RG1W, vl_9c);
                      RG1L = 0;
                      vo_fc.poke16(RG1W);
                      //m_224C4:
                      for (;;)
                      {
                        RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
                        RG1Blo = byte_at(RG1P, lcon(0x4));
                        RG1W = unsignedword(RG1Blo);
                        if (RG1W <= vo_fc.peek16())
                        {
                          RG1P = DOWNCAST(unk, xp_98) + 2 * signedlong(ddat.v1e08ac);
                          mov16(location(RG1P), RG3W);
                          vl_a0 = 1;
                          RG1P = DOWNCAST(t_bmp, dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ RG3W));
                          xp_bc = UPCAST(unk, RG1P);
                          DM2_ZERO_MEMORY(RG1P, vl_9c);
                          break;
                        }
                        //m_2249C:
                        RG4L = signedlong(vo_fc.peek16());
                        RG1P = DOWNCAST(unk, xp_98) + 2 * RG4L;
                        RG2W = word_at(RG1P);
                        if (RG2W >= 0)
                        {
                          RG1P = DOWNCAST(t_bmp, dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ RG2W));
                          RG4L *= 4;
                          RG4P = DOWNCAST(unk, xp_8c) + RG4L;
                          movp(location(RG4P), RG1P);
                        }
                        vo_fc.inc16();
                      }
                    }
                    //m_2251B:
                    RG1P = DOWNCAST(unk, xp_8c) + 4 * signedlong(ddat.v1e08ac);
                    RG4P = DOWNCAST(unk, xp_bc);
                    movp(location(RG1P), RG4P);
                  }
                  //m_2252E:
                  RG1L = signedlong(vw_f8);
                  if (RG1L < lcon(0x4))
                  {
                    RG1W = ddat.v1d3248;
                    vql_f0.poke32(RG1L);
                  }
                  DM2_1c9a_0648(signedlong(ddat.v1e08ac));
                  RG1P = pointer_at(DOWNCAST(unk, xp_8c) + 4 * signedlong(ddat.v1d3248));
                  xp_bc = UPCAST(unk, RG1P);
                  skip00577 = true;
                }

                if (skip00577)
                {
                  //m_225B2:
                  RG1W = vo_f4.peek16();
                  vw_108 = RG1W;
                  RG1W = vw_12c;
                }

                //m_225BC:
                vw_128 = RG1W;
                RG3W = unsignedword(RG3Blo);
                vb_144 = RG3Bhi;
              }
              else
                vb_144 = lcon(0xffffffff);
            }
            skip00579 = true;
          }

          if (skip00579)
          {
            //m_225C5:
            RG1Blo = vb_144;
            if (RG1Blo != 0)
            {
              //m_231C6:
              if (RG1Blo == lcon(0xffffffff))
              {
                RG1L = signedlong(vw_f8);
                if (RG1L < lcon(0x4))
                {
                  RG1Blo = ddat.v1e08b6;
                  vb_13c = RG1Blo;
                  RG1Blo = vo_e8.peek8();
                  vb_144 = RG1Blo;
                  RG1Blo = vb_140 & 2;
                  RG1L = unsignedlong(RG1Blo);
                  if (RG1L != 0)
                  {
                    RG1Blo = vb_140 & lcon(0x8);
                    RG1L = unsignedlong(RG1Blo);
                    if (RG1L != 0)
                      DM2_19f0_045a(signedlong(vo_f4.peek16()), signedlong(vw_12c));
                    xp_b4 = UPCAST(unk, DOWNCAST(s_len8, argp_s23));
                    RG4L = 0;
                    vo_10c.poke16(RG4W);
                    //m_239FE:
                    for (;;)
                    {
                      bool skip01048 = false;
                      RG1W = vo_10c.peek16();
                      if (RG1W >= argw0)
                        break;
                      //m_2322C:
                      RG1L = signedlong(byte_at(DOWNCAST(unk, xp_b4)));
                      RG4L = signedlong(vo_e8.peek16());
                      vl_58 = RG4L;
                      RG1L++;
                      if (RG1L >= RG4L)
                      {
                        RG1P = DOWNCAST(unk, xp_b4);
                        RG1Blo = byte_at(RG1P, lcon(0x10)) & 2;
                        RG1L = unsignedlong(RG1Blo);
                        if (RG1L != 0)
                        {
                          bool skip00592 = false;
                          bool skip01047 = false;
                          RG1P = DOWNCAST(unk, xp_b4);
                          RG1Blo = byte_at(RG1P, lcon(0x7));
                          if (mkub(RG1Blo) < mkub(lcon(0x12)))
                          {
                            //m_232AA:
                            if (mkub(RG1Blo) < mkub(lcon(0x8)))
                            {
                              //m_232D8:
                              if (mkub(RG1Blo) < mkub(2))
                                skip01048 = true;
                              else
                              {
                                if (mkub(RG1Blo) <= mkub(2))
                                {
                                  //m_23322:
                                  RG1W = ddat.v1d3248;
                                  if (RG1W != ddat.v1e08d6)
                                    skip01048 = true;
                                  else
                                  {
                                    RG1W = vo_f4.peek16();
                                    if (RG1W != ddat.v1e08d8)
                                      skip01048 = true;
                                    else
                                    {
                                      RG1W = vw_12c;
                                      if (RG1W != ddat.v1e08d4)
                                        skip01048 = true;
                                      else
                                      {
                                        RG1P = DOWNCAST(unk, xp_b4);
                                        RG1W = word_at(RG1P, lcon(0x8));
                                        if (RG1UW < mkuw(lcon(0x1)))
                                          skip01048 = true;
                                        else
                                        {
                                          if (RG1UW <= mkuw(lcon(0x1)))
                                          {
                                            //m_23373:
                                            RG1P = DOWNCAST(unk, xp_b4);
                                            if (word_at(RG1P + lcon(0xa)) == 0)
                                              skip01047 = true;
                                            else
                                            {
                                              RG3L = signedlong(vw_ec);
                                              RG2L = signedlong(vw_100);
                                              RG4L = signedlong(ddat.v1e08d4);
                                              RG1L = signedlong(DM2_CALC_VECTOR_DIR(ddat.v1e08d8, RG4W, RG2W, RG3W));
                                              RG3L = unsignedlong(RG1W) - signedlong(ddat.v1e08da) & lcon(0x3);
                                              RG1L = lcon(0x1) << RG3Blo;
                                              RG4L = signedlong(word_at(DOWNCAST(unk, xp_b4), lcon(0xa)));
                                              if ((RG1L & RG4L) == 0)
                                                skip00592 = true;
                                              else
                                                skip01047 = true;
                                            }
                                          }
                                          else
                                          {
                                            if (RG1W == lcon(0x3))
                                            {
                                              //m_233D5:
                                              RG1L = signedlong(vw_f8);
                                              RG4W = vw_12c;
                                              RG2W = table1d2804[RG1L];
                                              RG1L = RG4L + RG2L;
                                              RG4L = signedlong(RG1W);
                                              RG1L = signedlong(vw_f8);
                                              RG2W = vo_f4.peek16();
                                              RG3W = table1d27fc[RG1L];
                                              RG1L = dm2_GET_TILE_VALUE(signedlong(CUTX16(RG2L + RG3L)), RG4L);
                                              RG1W = unsignedword(RG1Blo);
                                              RG4L = unsignedlong(RG1W) >> 5 & lcon(0xff);
                                              if (RG4L != 2)
                                                skip00592 = true;
                                              else
                                              {
                                                RG4L = RG1L;
                                                RG4Blo &= lcon(0x8);
                                                RG4L = unsignedlong(RG4W);
                                                if (RG4L == 0)
                                                  skip00592 = true;
                                                else
                                                {
                                                  RG1Blo &= 0x1;
                                                  RG1L = unsignedlong(RG1W);
                                                  if (RG1L == 0)
                                                    skip01047 = true;
                                                  else
                                                    skip00592 = true;
                                                }
                                              }
                                            }
                                            else
                                              skip01048 = true;
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                                else
                                {
                                  if (RG1Blo == lcon(0x3))
                                  {
                                    bool skip00583 = false;
                                    RG1W = ddat.v1d3248;
                                    if (RG1W == ddat.v1e08d6)
                                    {
                                      RG1W = vo_f4.peek16();
                                      if (RG1W == ddat.v1e08d8)
                                      {
                                        RG1W = vw_12c;
                                        if (RG1W == ddat.v1e08d4)
                                        {
                                          RG1L = 1;
                                          skip00583 = true;
                                        }
                                      }
                                    }

                                    if (!skip00583)
                                      //m_23318:
                                      RG1L = 0;

                                    //m_2331A:
                                    vl_a4 = RG1L;
                                  }
                                  skip01048 = true;
                                }
                              }
                            }
                            else
                            {
                              bool skip00584 = false;
                              if (mkub(RG1Blo) <= mkub(lcon(0x8)))
                              {
                                //m_23440:
                                RG1L = unsignedlong(ddat.v1e08ae) >> 5;
                                if (RG1Blo == 0)
                                  skip01048 = true;
                                else
                                  skip00584 = true;
                              }
                              else
                              {
                                if (mkub(RG1Blo) < mkub(lcon(0xf)))
                                {
                                  //m_232CB:
                                  if (RG1Blo == lcon(0x9))
                                    skip00584 = true;
                                  else
                                    skip01048 = true;
                                }
                                else
                                {
                                  bool skip00585 = false;
                                  if (mkub(RG1Blo) <= mkub(lcon(0xf)))
                                    skip00585 = true;
                                  else
                                  {
                                    if (RG1Blo == lcon(0x11))
                                      skip00585 = true;
                                  }

                                  if (skip00585)
                                  {
                                    //m_23533:
                                    RG1L = signedlong(word_at(DOWNCAST(unk, xp_b4), lcon(0x8)));
                                    put16(parw21, RG1W);
                                    put16(parw20, vw_f8);
                                    i32 parl19 = signedlong(vw_12c);
                                    RG3L = signedlong(vo_f4.peek16());
                                    RG2L = signedlong(vw_ec);
                                    RG4L = signedlong(vw_100);
                                    RG1L = DM2_19f0_2165(0, RG4L, RG2L, RG3L, parl19, parw20, parw21);
                                    vl_a4 = RG1L;
                                    if (RG1L != 0 || ddat.v1e08be == 0)
                                      skip01048 = true;
                                    else
                                      skip00592 = true;
                                  }
                                  else
                                    skip01048 = true;
                                }
                              }

                              if (skip00584)
                              {
                                //m_23453:
                                RG1P = DOWNCAST(unk, xp_b4);
                                RG1W = word_at(RG1P, lcon(0xa));
                                s350.v1e0578 &= RG1W;
                                RG1P = DOWNCAST(unk, xp_b4);
                                RG1Blo = byte_at(RG1P + lcon(0x7)) == bcon(0x9) ? 1 : 0;
                                RG1L = unsignedlong(RG1Blo);
                                i32 parl18 = RG1L;
                                i32 parl17 = signedlong(vw_f8);
                                put16(parw16, vw_ec);
                                RG3L = signedlong(vw_100);
                                RG2P = DOWNCAST(unk, xp_b4) + 2;
                                RG1L = signedlong(word_at(DOWNCAST(unk, xp_b4), lcon(0x8)));
                                RG4P = DOWNCAST(unk, xp_bc);
                                RG1L = DM2_19f0_05e8(RG1L, RG4P, RG2P, RG3L, parw16, parl17, parl18);
                                vl_a4 = RG1L;
                                if (RG1L != 0)
                                {
                                  RG1W = ddat.v1e08a8 & lcon(0x1f);
                                  RG4P = DOWNCAST(unk, xp_b4);
                                  and8(location(RG4P + lcon(0x4)), lcon(0xffffffe0));
                                  RG2W = word_at(RG4P, lcon(0x4));
                                  RG2L |= RG1L;
                                  mov16(location(RG4P + lcon(0x4)), RG2W);
                                  RG1W = ddat.v1e08aa & lcon(0x1f);
                                  RG3L = RG2L & lcon(0xfffffc1f);
                                  RG1L <<= bcon(0x5);
                                  RG2L = RG3L | RG1L;
                                  mov16(location(RG4P + lcon(0x4)), RG2W);
                                  RG1W = ddat.v1e08ac & lcon(0x3f);
                                  and8(location(RG4P + lcon(0x5)), lcon(0x3));
                                  RG1L <<= bcon(0xa);
                                  or16(location(RG4P + lcon(0x4)), RG1W);
                                  RG1P = DOWNCAST(unk, xp_8c) + 4 * signedlong(ddat.v1e08ac);
                                  RG4L = signedlong(ddat.v1e08a8) << 7;
                                  RG1P = pointer_at(RG1P);
                                  RG4P = RG1P + RG4L;
                                  RG1L = signedlong(ddat.v1e08aa);
                                  RG1Blo = byte_at(RG4P, 4 * RG1L) - 1;
                                  RG4P = DOWNCAST(unk, xp_b4);
                                  mov8(location(RG4P + lcon(0x6)), RG1Blo);
                                }
                                RG1L = vl_c4;
                                s350.v1e0578 = RG1W;
                                skip01048 = true;
                              }
                            }
                          }
                          else
                          {
                            bool skip00586 = false;
                            bool skip00591 = false;
                            if (mkub(RG1Blo) <= mkub(lcon(0x12)))
                              skip00586 = true;
                            else
                            {
                              if (mkub(RG1Blo) < mkub(lcon(0x16)))
                              {
                                //m_23295:
                                if (mkub(RG1Blo) <= mkub(lcon(0x13)))
                                {
                                  //m_23581:
                                  RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
                                  RG1W = word_at(RG1P, lcon(0xe));
                                  RG1L <<= bcon(0x6);
                                  RG1UW >>= 14;
                                  if (RG1W != vw_f8)
                                    skip01048 = true;
                                  else
                                    skip00586 = true;
                                }
                                else
                                {
                                  if (RG1Blo == lcon(0x14))
                                  {
                                    //m_236D5:
                                    RG1L = DM2_19f0_050f();
                                    RG4Blo = RG1W != wcon(0xfffe) ? 1 : 0;
                                    RG4L = unsignedlong(RG4Blo);
                                    if (RG4L == 0)
                                      skip01048 = true;
                                    else
                                    {
                                      RG4L = signedlong(word_at(DOWNCAST(unk, xp_b4), lcon(0x8)));
                                      RG1L = unsignedlong(RG1W);
                                      vl_54 = RG1L;
                                      RG1L = DM2_CREATURE_CAN_HANDLE_IT(RG1L, RG4L);
                                      if (RG1L == 0)
                                        skip01048 = true;
                                      else
                                      {
                                        RG3L = signedlong(word_at(DOWNCAST(unk, xp_b4), lcon(0xa)));
                                        RG2L = signedlong(vw_12c);
                                        RG4L = signedlong(vo_f4.peek16());
                                        DM2_1c9a_19d4(vl_54, RG4L, RG2L, RG3L);
                                        skip01047 = true;
                                      }
                                    }
                                  }
                                  else
                                    skip01048 = true;
                                }
                              }
                              else
                              {
                                if (mkub(RG1Blo) <= mkub(lcon(0x16)))
                                {
                                  //m_2372D:
                                  if ((ddat.v1e08ae & wcon(0x10)) == 0)
                                    skip01048 = true;
                                  else
                                  {
                                    RG1L = signedlong(word_at(DOWNCAST(unk, xp_b4), lcon(0x8)));
                                    put16(parw24, RG1W);
                                    put16(parw23, vw_f8);
                                    put16(parw22, vw_12c);
                                    RG3L = signedlong(vo_f4.peek16());
                                    RG2L = signedlong(vw_ec);
                                    vl_84 = signedlong(vw_100);
                                    RG1L = 0;
                                    RG1Blo = byte_at(DOWNCAST(unk, xp_b4), lcon(0xa));
                                    RG4L = vl_84;
                                    RG1L = DM2_19f0_2813(RG1L, RG4L, RG2L, RG3W, parw22, parw23, parw24) ? 1 : 0;
                                    skip00591 = true;
                                  }
                                }
                                else
                                {
                                  if (mkub(RG1Blo) < mkub(lcon(0x1a)))
                                  {
                                    //m_23288:
                                    if (RG1Blo == lcon(0x17))
                                    {
                                      //m_2377C:
                                      if ((ddat.v1e08ae & wcon(0x10)) == 0)
                                        skip01048 = true;
                                      else
                                      {
                                        RG3L = signedlong(vw_12c);
                                        RG2L = signedlong(vo_f4.peek16());
                                        RG4L = signedlong(vw_f8);
                                        DM2_ADD_BACKGROUND_LIGHT_FROM_TILE(vl_58, RG4L, RG2L, RG3L, lcon(0x4));
                                        skip00592 = true;
                                      }
                                    }
                                    else
                                      skip01048 = true;
                                  }
                                  else
                                  {
                                    if (mkub(RG1Blo) <= mkub(lcon(0x1a)))
                                    {
                                      bool skip00595 = false;
                                      //m_237AA:
                                      RG1P = DOWNCAST(unk, xp_b4);
                                      if (word_at(RG1P + lcon(0x8)) != lcon(0x1))
                                        skip00595 = true;
                                      else
                                      {
                                        bool skip00594 = false;
                                        RG1W = ddat.v1d3248;
                                        if (RG1W == ddat.v1e08d6)
                                        {
                                          RG1W = vo_f4.peek16();
                                          if (RG1W == ddat.v1e08d8)
                                          {
                                            RG1W = vw_12c;
                                            if (RG1W == ddat.v1e08d4)
                                            {
                                              //m_237E2:
                                              RG1L = 0;
                                              skip00594 = true;
                                            }
                                          }
                                        }

                                        if (!skip00594)
                                          //m_237DB:
                                          RG1L = 1;

                                        //m_237E4:
                                        if (RG1L != 0)
                                          skip01048 = true;
                                        else
                                          skip00595 = true;
                                      }

                                      if (skip00595)
                                      {
                                        //m_237EC:
                                        RG1L = signedlong(vw_f8);
                                        put16(parw27, RG1W);
                                        RG1L = signedlong(vw_12c);
                                        vl_b0 = RG1L;
                                        put16(parw26, RG1W);
                                        vl_80 = signedlong(vo_f4.peek16());
                                        RG2L = signedlong(vw_ec);
                                        RG4L = signedlong(vw_100);
                                        RG1L = 0;
                                        RG1Blo = byte_at(DOWNCAST(unk, xp_b4), lcon(0xa));
                                        RG3L = vl_80;
                                        RG1L = DM2_19f0_0d10(RG1L, RG4L, RG2L, RG3L, parw26, parw27);
                                        if (RG1L == 0)
                                          skip01048 = true;
                                        else
                                        {
                                          bool skip00596 = false;
                                          RG1P = DOWNCAST(unk, xp_b4);
                                          if (word_at(RG1P + lcon(0x8)) != 2)
                                            skip00596 = true;
                                          else
                                          {
                                            RG3L = signedlong(ddat.v1e08d4);
                                            RG2L = signedlong(ddat.v1e08d8);
                                            RG4L = vl_b0;
                                            RG1L = signedlong(DM2_CALC_SQUARE_DISTANCE(CUTX16(vl_80), RG4W, RG2W, RG3W));
                                            if (RG1L == lcon(0x1))
                                              skip00596 = true;
                                          }

                                          if (skip00596)
                                          {
                                            //m_2385B:
                                            RG4L = signedlong(vw_12c);
                                            DM2_19f0_045a(signedlong(vo_f4.peek16()), RG4L);
                                            RG1P = DOWNCAST(unk, xp_b4);
                                            if (word_at(RG1P + lcon(0xa)) == 0 && (ddat.v1e08ae & wcon(0x7)) == 0)
                                              skip01048 = true;
                                            else
                                            {
                                              bool skip00597 = false;
                                              RG1P = DOWNCAST(unk, xp_b4);
                                              if (word_at(RG1P + lcon(0xa)) != lcon(0x1))
                                                skip00597 = true;
                                              else
                                              {
                                                RG1Blo = CUTX8(ddat.v1e08ae) & lcon(0x7);
                                                RG1L = unsignedlong(RG1Blo);
                                                if (RG1L != lcon(0x4))
                                                  skip00597 = true;
                                              }

                                              if (skip00597)
                                              {
                                                //m_238A2:
                                                if (ddat.v1e08b0 == lcon(0xffffffff))
                                                {
                                                  RG4L = signedlong(vw_12c);
                                                  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(vo_f4.peek16(), RG4W));
                                                  ddat.v1e08b0 = RG1W;
                                                }
                                                RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(ddat.v1e08b0)));
                                                RG4P = RG1P;
                                                RG2Blo = byte_at(RG1P, lcon(0x3));
                                                if ((RG2Blo & bcon(0x4)) == 0)
                                                  skip01047 = true;
                                                else
                                                {
                                                  bool skip00598 = false;
                                                  if ((RG2Blo & bcon(0x2)) == 0)
                                                    skip00598 = true;
                                                  else
                                                  {
                                                    RG1P = DOWNCAST(unk, xp_b4);
                                                    if (word_at(RG1P + lcon(0xa)) != 0)
                                                      skip00598 = true;
                                                  }

                                                  if (skip00598)
                                                  {
                                                    //m_238F1:
                                                    if (jnz_test8(location(RG4P + lcon(0x3)), 2))
                                                      skip01047 = true;
                                                    else
                                                    {
                                                      RG1P = DOWNCAST(unk, xp_b4);
                                                      if (word_at(RG1P + lcon(0xa)) == lcon(0x1))
                                                        skip01048 = true;
                                                      else
                                                        skip01047 = true;
                                                    }
                                                  }
                                                  else
                                                    skip01048 = true;
                                                }
                                              }
                                              else
                                                skip01048 = true;
                                            }
                                          }
                                          else
                                            skip01048 = true;
                                        }
                                      }
                                    }
                                    else
                                    {
                                      if (RG1Blo == lcon(0x1b))
                                      {
                                        bool skip00599 = false;
                                        //m_2390E:
                                        if (vb_13c == 0)
                                          //m_23922:
                                          RG1W = vo_e8.peek16() + 1;
                                        else
                                        {
                                          RG3Blo = CUTX8(vw_f8);
                                          RG1L = lcon(0x1) << RG3Blo;
                                          RG1Blo |= lcon(0xffffff80);
                                        }
                                        //m_23926:
                                        vo_fc.poke16(RG1W);
                                        RG1W = vw_c0;
                                        RG2W = ddat.v1d62a4;
                                        if (RG1W != RG2W)
                                        {
                                          //m_23952:
                                          RG3L = vql_f0.peek32();
                                          if (RG3W >= 0 && RG3W == RG2W)
                                          {
                                            RG1P = ddat.v1e08cc + (signedlong(vw_108) << 5);
                                            RG4L = signedlong(vw_128);
                                            RG1P += RG4L;
                                            skip00599 = true;
                                          }
                                        }
                                        else
                                        {
                                          RG1L = signedlong(vo_f4.peek16()) << 5;
                                          RG4P = ddat.v1e08cc + RG1L;
                                          RG1P = RG4P + signedlong(vw_12c);
                                          skip00599 = true;
                                        }

                                        if (skip00599)
                                        {
                                          //m_23976:
                                          RG4Blo = vo_fc.peek8();
                                          or8(location(RG1P), RG4Blo);
                                        }
                                        bool skip00601 = false;
                                        //m_2397D:
                                        RG1W = vw_c0;
                                        RG2W = ddat.v1d62a6;
                                        if (RG1W != RG2W)
                                        {
                                          //m_239A2:
                                          RG3L = vql_f0.peek32();
                                          if (RG3W >= 0 && RG3W == RG2W)
                                          {
                                            RG1L = signedlong(vw_108) << 5;
                                            RG4P = ddat.v1e08c8 + RG1L;
                                            RG1L = signedlong(vw_128);
                                            skip00601 = true;
                                          }
                                        }
                                        else
                                        {
                                          RG1L = signedlong(vo_f4.peek16()) << 5;
                                          RG4P = ddat.v1e08c8 + RG1L;
                                          RG1L = signedlong(vw_12c);
                                          skip00601 = true;
                                        }

                                        if (skip00601)
                                        {
                                          //m_239C3:
                                          RG1P = RG4P + RG1L;
                                          RG4Blo = vo_fc.peek8();
                                          or8(location(RG1P), RG4Blo);
                                        }
                                      }
                                      skip01048 = true;
                                    }
                                  }
                                }
                              }
                            }

                            if (skip00586)
                            {
                              //m_2359B:
                              RG1L = DM2_19f0_050f();
                              RG4Blo = RG1W != wcon(0xfffe) ? 1 : 0;
                              RG4L = unsignedlong(RG4Blo);
                              if (RG4L == 0)
                                skip01048 = true;
                              else
                              {
                                RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
                                RG4P = RG1P;
                                RG1Blo = byte_at(RG1P, lcon(0x4));
                                RG1W = unsignedword(RG1Blo);
                                RG2P = DOWNCAST(unk, xp_b4);
                                if (RG1W != word_at(RG2P, lcon(0x8)))
                                  skip01048 = true;
                                else
                                {
                                  RG1L = signedlong(word_at(RG2P, lcon(0xa)));
                                  RG1Blo = RG1L == lcon(0x1) ? 1 : 0;
                                  RG1L = unsignedlong(RG1Blo);
                                  flag = RG1L != 0;
                                  vl_a4 = RG1L;
                                  if (flag)
                                    skip01048 = true;
                                  else
                                  {
                                    RG1L = signedlong(word_at(RG2P, lcon(0xa)));
                                    if (RG1L != 2)
                                    {
                                      bool skip00588 = false;
                                      bool skip00589 = false;
                                      //m_2363D:
                                      RG1L = signedlong(vw_f8);
                                      RG4W = vo_f4.peek16();
                                      RG2W = table1d27fc[RG1L];
                                      RG4L += RG2L;
                                      vql_120.poke32(RG4L);
                                      RG4W = vw_12c;
                                      RG3W = table1d2804[RG1L];
                                      RG4L += RG3L;
                                      vql_124.poke32(RG4L);
                                      RG1W = ddat.v1d3248;
                                      if (RG1W != ddat.v1e08d6)
                                        skip00588 = true;
                                      else
                                      {
                                        RG1L = vql_120.peek32();
                                        if (RG1W != ddat.v1e08d8)
                                          skip00588 = true;
                                        else
                                        {
                                          RG1L = vql_124.peek32();
                                          if (RG1W != ddat.v1e08d4)
                                            skip00588 = true;
                                        }
                                      }

                                      if (skip00588)
                                      {
                                        bool skip00590 = false;
                                        //m_2368A:
                                        RG4L = unsignedlong(vql_124.peek16());
                                        RG1L = DM2_GET_CREATURE_AT(unsignedlong(vql_120.peek16()), RG4L);
                                        RG4Blo = RG1W != wcon(0xffff) ? 1 : 0;
                                        RG4L = unsignedlong(RG4Blo);
                                        if (RG4L == 0)
                                          skip00590 = true;
                                        else
                                        {
                                          RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG1W));
                                          if ((RG1Blo & bcon(0x1)) != 0)
                                            skip00590 = true;
                                        }

                                        if (skip00590)
                                        {
                                          //m_236BF:
                                          RG1L = 0;
                                          skip00591 = true;
                                        }
                                        else
                                          skip00589 = true;
                                      }
                                      else
                                        skip00589 = true;

                                      if (skip00589)
                                      {
                                        //m_236B8:
                                        RG1L = 1;
                                        skip00591 = true;
                                      }
                                    }
                                    else
                                    {
                                      RG1W = word_at(RG4P, 2);
                                      //m_235FA:
                                      for (;;)
                                      {
                                        if (RG1W == lcon(0xfffffffe))
                                        {
                                          skip01048 = true;
                                          break;
                                        }
                                        RG4L = RG1L;
                                        RG4Blo ^= RG1Blo;
                                        RG4Bhi &= lcon(0x3c);
                                        RG4L = unsignedlong(RG4W) >> bcon(0xa);
                                        if (RG4L != lcon(0xe))
                                        {
                                          RG4L = unsignedlong(RG1W);
                                          RG2L = RG4L >> bcon(0xe);
                                          RG4L = signedlong(vw_f8);
                                          if (RG2L != RG4L)
                                          {
                                            skip01047 = true;
                                            break;
                                          }
                                        }
                                        //m_23631:
                                        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG1UW)));
                                      }
                                    }
                                  }
                                }
                              }
                            }

                            if (skip00591)
                            {
                              //m_236C1:
                              vl_a4 = RG1L;
                              if (RG1L != 0)
                                skip01048 = true;
                              else
                                skip00592 = true;
                            }
                          }

                          if (skip01047)
                          {
                            //m_233C9:
                            vl_a4 = 1;
                            skip01048 = true;
                          }

                          if (skip00592)
                          {
                            //m_236CC:
                            vb_144 = lcon(0xffffffff);
                            skip01048 = true;
                          }
                        }
                      }

                      if (skip01048)
                      {
                        //m_239CD:
                        if (vl_a4 != 0)
                        {
                          if (vl_50 == 0)
                            break;
                          RG1W = ddat.v1d3248;
                          if (RG1W == vw_c8 || RG1W == ddat.v1e08d6)
                            break;
                          RG3L = 0;
                          vl_a4 = RG3L;
                        }
                      }

                      //m_239F6:
                      vo_10c.inc16();
                      xp_b4 += lcon(0x16); // DANGER
                    }
                  }
                  //m_23A0E:
                  if (vb_13c == 0)
                  {
                    RG1P = DOWNCAST(unk, xp_bc) + (signedlong(vo_f4.peek16()) << 7);
                    RG4L = signedlong(vw_12c) << 2;
                    RG1P += RG4L;
                    RG4Blo = vb_144;
                    mov8(location(RG1P), RG4Blo);
                  }
                }
              }
              skipbreak16 = true;
              break;
            }
            vo_f4.poke16(ddat.v1e08a8);
            RG1L = signedlong(vo_f4.peek16()) << 7;
            RG4P = DOWNCAST(unk, xp_bc) + RG1L;
            vw_12c = ddat.v1e08aa;
            RG1L = signedlong(vw_12c);
            if (byte_at(RG4P + 4 * RG1L) == 0)
              skipbreak19 = true;
            break;
          }

          //m_23D3C:
          if (RG6W != vw_cc)
          {
            //m_2215D:
            for (;;)
            {
              RG1P = DOWNCAST(unk, xp_90) + 4 * signedlong(vw_cc);
              RG4L = unsignedlong(byte_at(RG1P));
              vw_100 = RG4W;
              RG4L = unsignedlong(byte_at(RG1P, lcon(0x1)));
              vw_ec = RG4W;
              RG1Blo = byte_at(RG1P, 2);
              RG1W = unsignedword(RG1Blo);
              vw_dc = RG1W;
              RG1W = vw_cc + 1;
              RG1W = unsignedword(RG1Blo);
              vw_cc = RG1W;
              RG1W = vw_dc;
              if (RG1W != ddat.v1d3248)
              {
                RG4L = signedlong(vw_dc);
                DM2_1c9a_0648(RG4L);
                RG1P = pointer_at(DOWNCAST(unk, xp_8c) + 4 * RG4L);
                xp_bc = UPCAST(unk, RG1P);
              }
              RG1L = signedlong(vw_100) << 7;
              RG4P = DOWNCAST(unk, xp_bc) + RG1L;
              RG1L = signedlong(vw_ec);
              RG1Blo = byte_at(RG4P, 4 * RG1L);
              RG1W = unsignedword(RG1Blo);
              vo_e8.poke16(RG1W);
              if (vl_b8 == 0)
                break;
              //m_221D6:
              bool skipbreak12 = false;
              for (;;)
              {
                RG4L = RG51L + 1; // TODO: is RG51L really initialized?
                if (RG51W >= vo_e8.peek16())
                {
                  //m_22216:
                  RG1L = signedlong(RG51W);
                  RG2Bhi = vba_08[RG1L];
                  flag = --RG2Bhi != 0;
                  vba_08[RG1L] = RG2Bhi;
                  if (!flag)
                    RG51L = RG4L;
                  skipbreak12 = true;
                  break;
                }
                RG1L = signedlong(RG51W);
                if (vba_08[RG1L] != 0)
                {
                  RG1P = DOWNCAST(unk, xp_90) + 4 * signedlong(RG6W);
                  RG4Blo = CUTX8(vw_100);
                  mov8(location(RG1P), RG4Blo);
                  RG4Blo = CUTX8(vw_ec);
                  mov8(location(RG1P + lcon(0x1)), RG4Blo);
                  RG4Blo = CUTX8(vw_dc);
                  mov8(location(RG1P + 2), RG4Blo);
                  RG6L++;
                  RG6L = unsignedlong(RG6Blo);
                  break;
                }
                //m_22212:
                RG51L = RG4L;
              }

              if (skipbreak12)
                break;
            }
            //m_2222D:
            RG1W = vo_e8.peek16();
            if (RG1W <= vw_d4)
            {
              RG1L = signedlong(vo_e8.peek16());
              if (RG1L > lcon(0x6))
                vb_148 |= lcon(0x40);
              if (vl_60 != 0)
              {
                RG1L = signedlong(vo_e8.peek16());
                if (RG1L > lcon(0x1))
                {
                  RG1Blo = vb_148 & bcon(0xe0) | bcon(0x4);
                  vb_148 = RG1Blo;
                  RG3L = 0;
                  vl_60 = RG3L;
                }
              }
              RG1W = ddat.v1d62ec & lcon(0x1);
              vl_e0 = RG1L;
              if (RG1W == 0)
              {
                RG3L = RG1L - 1;
                vl_e0 = RG3L;
              }
              RG1W = ddat.v1d62ec & lcon(0x1);
              RG1L = unsignedlong(RG1W);
              if (RG1L == 0)
                //m_222A7:
                ddat.v1d62ec >>= bcon(0x1);
              else
              {
                RG1L = unsignedlong(ddat.v1d62ec) / 2;
                RG1Bhi ^= lcon(0xffffffb4);
                ddat.v1d62ec = RG1W;
              }
              //m_222AE:
              RG1W = ddat.v1d62ec & lcon(0x3);
              vw_f8 = RG1W;
              if (vl_94 == 0)
                //m_222CD:
                vw_130 = 5;
              else
                vw_130 = 7;
              continue;
            }
          }
          //m_23D46:
          if (vw_d8 == lcon(0xffffffff))
            jump L_fin2;
          jump L_fin1;
        }

        if (skipbreak19)
          break;

        if (skipbreak16)
          break;
      }

      if (skipbreak19)
        break;
    }
  }
  FOREND

L_fin1:
  RG4L = signedlong(vw_d8);
  RG1L = 22 * RG4L;
  RG4P = DOWNCAST(s_len8, argp_s23) + RG1L;
  xp_b4 = UPCAST(unk, RG4P);
  RG1W = word_at(RG4P, lcon(0x4)) & lcon(0x1f);
  vw_100 = RG1W;
  RG1W = word_at(RG4P, lcon(0x4));
  RG1L <<= bcon(0x6);
  _RG1.ushiftr11();
  vw_ec = RG1W;
  RG1W = word_at(RG4P, lcon(0x4));
  RG1UW >>= bcon(0xa);
  vw_dc = RG1W;
  RG1W = signedword(byte_at(RG4P, lcon(0x6)));
  vo_e8.poke16(RG1W);
  //m_23DA0:
  for (;;)
  {
    RG4W = vo_e8.peek16() - 1;
    vo_e8.poke16(RG4W);
    if (RG4W < 0)
      jump L_fin2;
    RG1L = signedlong(vw_dc);
    if (RG1L < 0)
    int stop = 1; // TODO
    RG1L *= 4;
    RG1P = DOWNCAST(unk, xp_8c) + RG1L;
    RG4L = signedlong(vw_100) << 7;
    if (pointer_at(RG1P) == NULL) // that happens!!!
    int stop = 1; // TODO
    RG1P = pointer_at(RG1P);
    RG4P = RG1P + RG4L;
    s_31_ac.b_00 = byte_at(RG4P, 4 * signedlong(vw_ec));
    s_31_ac.b_01 = byte_at(RG4P, 4 * signedlong(vw_ec) + 1);
    s_31_ac.w_02 = word_at(RG4P, 4 * signedlong(vw_ec) + 2);
    RG1P = DOWNCAST(c_button, button) + 2 * signedlong(vo_e8.peek16()); // TODO exactly what?
    RG4Blo = s_31_ac.b_01;
    RG4W &= lcon(0x7);
    and8(location(RG1P), lcon(0xfffffff8));
    or16(location(RG1P), RG4W);
    RG4W = vw_100;
    RG4W &= lcon(0x1f);
    and8(location(RG1P), lcon(0x7));
    RG4L *= 8;
    or16(location(RG1P), RG4W);
    RG4W = vw_ec;
    RG4W &= lcon(0x1f);
    and8(location(RG1P + lcon(0x1)), lcon(0xffffffe0));
    RG4L <<= bcon(0x8);
    or16(location(RG1P), RG4W);
    RG4W = s_31_ac.w_02; // adj
    RG4UW >>= bcon(0xa);
    if (RG4W != vw_dc)
    {
      //m_23E28:
      vw_dc = RG4W;
      or8(location(RG1P + lcon(0x1)), lcon(0x20));
    }
    else
      and8(location(RG1P + lcon(0x1)), lcon(0xffffffdf));
    //m_23E30:
    RG1W = s_31_ac.w_02 & lcon(0x1f); // adj
    vw_100 = RG1W;
    RG1W = s_31_ac.w_02; // adj
    RG1L <<= bcon(0x6);
    _RG1.ushiftr11();
    vw_ec = RG1W;
  }
  FOREND

L_fin2:
  if (vl_a0 != 0)
  {
    RG3L = 0;
    vo_f4.poke16(RG3W);
    //m_23E7B:
    for (;;)
    {
      RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
      RG1Blo = byte_at(RG1P, lcon(0x4));
      RG1W = unsignedword(RG1Blo);
      if (RG1W <= vo_f4.peek16())
        break;
      //m_23E5C:
      RG3P = DOWNCAST(unk, xp_98) + 2 * signedlong(vo_f4.peek16());
      RG3W = word_at(RG3P);
      if (RG3W >= 0)
      {
        RG1L = signedlong(RG3W);
        dm2_dballochandler.DM2_dballoc_3e74_58a3((t_dbidx)RG1W);
      }
      vo_f4.inc16();
    }
  }
  //m_23E8B:
  dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(vl_7c);
  DM2_1c9a_0648(signedlong(vw_e4));
  return signedlong(vw_d8);
}

// was SKW_1c9a_37be
static void DM2___SET_CURRENT_THINKING_CREATURE_WALK_PATH(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;
  i16 vw_00;

  s350.v1e07e6 = NULL;
  if (s350.creatures == NULL)
    return;
  if (s350.v1e07d8.b_00 == 0)
    return;
  RG1W = s350.v1e054c;
  RG1Bhi &= 0x3;
  RG1L = unsignedlong(RG1W) | ulcon(0x30000000);
  RG1L = dm2_dballochandler.DM2_ALLOCATION11(RG1L, 0, OO /*TAG dbidx*/ vw_00);
  if (RG1L == 0)
  {
    s350.v1e07d8.b_01 = 0;
    RG4W = unsignedword(RG4Blo);
    s350.v1e07d8.b_00 = RG4Bhi;
    return;
  }
  s350.v1e07e6 = CHGCAST(i16, dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ vw_00)); // TODO: check return type vs. cast - diffs
}

i32 DM2_1c9a_381c(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG2P = DOWNCAST(c_creature, s350.creatures);
  RG4L = 0;
  DM2___SET_CURRENT_THINKING_CREATURE_WALK_PATH();
  RG1Bhi = s350.v1e07d8.b_00;
  if (RG1Bhi != 0)
  {
    RG4Blo = s350.v1e07d8.b_01;
    if (RG4W != 0)
    {
      RG3L = unsignedlong(RG1Bhi);
      RG1L = signedlong(RG4W);
      RG3L -= RG1L;
      RG1L = RG3L;
      RG1W = s350.v1e07e6[RG1L];
      RG1Blo &= lcon(0x7);
      mov8(location(RG2P + lcon(0x1b)), RG1Blo);
      return signedlong(RG4W);
    }
  }
  RG3W = unsignedword(RG3Blo);
  RG3Blo = s350.v1e0571;
  RG1W = s350.v1e07d8.w_08;
  RG2L = RG1L;
  RG2UW >>= bcon(0xa);
  if (RG3W == RG2W)
  {
    RG3Bhi ^= RG2Bhi;
    RG3Blo = s350.v1e0562.getxA();
    RG2L = RG1L;
    RG2Bhi ^= RG1Bhi;
    RG2Blo &= lcon(0x1f);
    if (RG3W == RG2W)
    {
      RG2W = unsignedword(RG2Blo);
      RG2Blo = s350.v1e0562.getyA();
      RG1L <<= bcon(0x6);
      _RG1.ushiftr11();
      if (RG2W == RG1W)
        s350.v1e07d8.w_08 = lcon(0xffffffff);
    }
  }
  return signedlong(RG4W);
}

// belongs to DM2_14cd_0389
i32 DM2_1c9a_38a8(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG51l;
  i8* RG52p;
  i16 RG53w;
  c_wlreg _RG61;
  c_wlreg _RG62;
  bool retflag = false;
  c_button button2;
  i32 vl_0c;
  unk* xparp01;
  c_button* button1;
  i16 parw03;
  i16 parw04;

  DM2___SET_CURRENT_THINKING_CREATURE_WALK_PATH();
  RG61L = 0;
  RG2W = s350.v1e07d8.w_04;
  //m_23FBD:
  for (;;)
  {
    RG1W = signedword(s350.v1e0674);
    if (RG61W >= RG1W)
      return retflag ? lcon(0x1) : 0;
    RG51l = signedlong(RG61W);
    RG1L = 22 * RG51l;
    RG4Blo = s350.v1e0678[RG51l].s23_00.b_07;
    if (RG4Blo == s350.v1e07d8.b_03 && RG2W == s350.v1e0678[RG51l].w_08)
      break;
    RG61L++;
  }
  //m_23FF9:
  if (RG61W != 0)
  {
    RG1P = DOWNCAST(c_sizex16, &s350.v1e0678[RG1L / 22]);
    DM2_COPY_MEMORY(DOWNCAST(c_sizex16, s350.v1e0678), RG1P, sizeof(c_sizex16));
  }
  //m_24012:
  s350.v1e0678[0].s23_00.b_00 = 0;
  button2.r.h = ddat.v1d3248;
  RG1W = s350.v1e07d8.w_08;
  RG1UW >>= bcon(0xa);
  RG1L = signedlong(RG1W);
  DM2_1c9a_0648(RG1L);
  RG1P = ADRESSOF(c_button, &button2);
  button1 = UPCAST(c_button, RG1P);
  xparp01 = DOWNCAST(c_sizex16, s350.v1e0678);
  RG1W = s350.v1e07d8.w_08;
  RG1L <<= bcon(0x6);
  _RG1.ushiftr11();
  button2.r.y = RG1W;
  RG4L = signedlong(button2.r.y);
  RG53w = s350.v1e07d8.w_08 & wcon(0x1f);
  RG1L = signedlong(RG53w);
  RG3L = 0;
  RG1L = DM2_FIND_WALK_PATH(RG1L, RG4L, 2, lcon(0x1), XUPCAST(s_len8, xparp01), button1);
  if (RG1W == 0 && s350.v1e0678[0].s23_00.b_06 == 0)
  {
    RG3L = unsignedlong(s350.v1e0562.getxA());
    RG4L = unsignedlong(s350.v1e0562.getyA());
    RG62W = unsignedword(s350.v1e07d8.b_01);
    if (RG62W != 0)
    {
      //m_240C8:
      DM2_1c9a_0648(signedlong(button2.r.h));
      vl_0c = 5;
      DM2___SET_CURRENT_THINKING_CREATURE_WALK_PATH();
      //m_240E1:
      for (;;)
      {
        RG1L = signedlong(RG62W);
        RG52p = DOWNCAST(i16, s350.v1e07e6 + unsignedlong(s350.v1e07d8.b_00) - RG1L); // adjusted
        RG1W = word_at(RG52p) & lcon(0x7);
        RG1L = signedlong(RG1W);
        put16(parw04, RG1W);
        put16(parw03, lcon(0xffffffff));
        RG2L = signedlong(RG4W);
        RG4L = signedlong(RG3W);
        RG1L = DM2_CREATURE_GO_THERE(unsignedlong(s350.v1e0673), RG4L, RG2L, lcon(0xffffffff), parw03, parw04);
        if (RG1L != 0)
        {
          RG1W = word_at(RG52p);
          RG1L <<= bcon(0x8);
          _RG1.ushiftr11();
          if (RG1W == ddat.v1e08a8)
          {
            RG1W = word_at(RG52p);
            RG1L *= 8;
            _RG1.ushiftr11();
            if (RG1W == ddat.v1e08aa)
            {
              RG1W = ddat.v1e08ac;
              RG1Blo = RG1W != ddat.v1d3248 ? 1 : 0;
              RG4W = unsignedword(RG4Blo);
              RG4Blo = RG1Blo;
              RG1W = word_at(RG52p);
              RG1L *= 4;
              RG1UW >>= bcon(0xf);
              if (RG1W == RG4W)
              {
                DM2_1c9a_0648(signedlong(ddat.v1e08ac));
                RG3W = ddat.v1e08a8;
                RG4W = ddat.v1e08aa;
                RG62L--;
                if (RG62W > 0)
                {
                  RG2L = vl_0c - 1;
                  vl_0c = RG2L;
                  if (RG2W > 0)
                    continue;
                }
                retflag = true;
              }
            }
          }
        }
        break;
      }
    }
    else
    {
      bool skip00633 = false;
      if (RG3W != RG53w || RG4W != button2.r.y)
        skip00633 = true;
      else
      {
        RG1W = ddat.v1d3248;
        if (RG1W == button2.r.h)
          retflag = true;
        else
          skip00633 = true;
      }

      if (skip00633)
        //m_240C1:
        retflag = false;
    }
  }
  //m_241A1:
  DM2_1c9a_0648(signedlong(button2.r.h));
  return retflag ? lcon(0x1) : 0;
}

i32 DM2_FILL_CAII_CUR_MAP(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  t_record* RG61rp;
  t_tile* RG7tilep;
  c_ol32 vol_00; // poke 32 peek16 inc32

  RG7tilep = *mapdat.map;
  RG1L = 2 * unsignedlong(*ddat.v1e03f4);
  RG61rp = dm2_v1e038c + RG1L / 2;
  vol_00.poke32(0);
  //m_242C8:
  for (;;)
  {
    RG1L = unsignedlong(vol_00.peek16());
    RG4L = signedlong(mapdat.map_width);
    if (RG1L >= RG4L)
      return RG1L;
    //m_241EA:
    RG5w = wcon(0x0);
    //m_242AF:
    for (;;)
    {
      RG1L = unsignedlong(RG5w);
      RG4L = signedlong(mapdat.map_height);
      if (RG1L >= RG4L)
      {
        vol_00.inc32();
        break;
      }
      //m_241F1:
      RG1P = DOWNCAST(t_tile, RG7tilep++);
      if (jnz_test8(location(RG1P), lcon(0x10)))
      {
        RG1W = record_to_word(*RG61rp++);
        //m_24203:
        for (;;)
        {
          RG4L = RG1L;
          RG4Blo ^= RG1Blo;
          RG4Bhi &= lcon(0x3c);
          RG4L = unsignedlong(RG4W) >> bcon(0xa);
          if (RG4L != lcon(0x4))
          {
            //m_2429A:
            RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG1UW)));
            if (RG1W != lcon(0xfffffffe))
              continue;
          }
          else
          {
            RG3L = unsignedlong(RG1W);
            RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
            RG4P = RG1P;
            if (byte_at(RG1P + lcon(0x5)) == lcon(0xffffffff))
            {
              RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG4P, lcon(0x4)))));
              if (jnz_test8(location(RG1P), lcon(0x1)))
              {
                //m_24257:
                RG2W = word_at(RG4P, lcon(0xa));
                DM2_1c9a_09db(RG4P);
                RG1L = RG2L;
                RG1Blo ^= RG2Blo;
                RG1Bhi &= lcon(0x60);
                RG3W = word_at(RG4P, lcon(0xa));
                RG3L |= RG1L;
                mov16(location(RG4P + lcon(0xa)), RG3W);
                RG2L &= lcon(0x803f);
                RG1L = unsignedlong(RG2W);
                if (RG1L == lcon(0x8001))
                {
                  RG1L = RG3L & lcon(0x7fc0);
                  RG2L = RG1L | lcon(0x8001);
                  mov16(location(RG4P + lcon(0xa)), RG2W);
                }
              }
              else
              {
                RG2L = unsignedlong(RG5w);
                RG4L = unsignedlong(vol_00.peek16());
                RG1L = RG3L;
                DM2_ALLOC_CAII_TO_CREATURE(RG1L, RG4L, RG2L);
              }
            }
          }
          break;
        }
      }
      //m_242AE:
      RG5w++;
    }
  }
  FOREND
}

void DM2_FILL_ORPHAN_CAII(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG3W = ddat.v1d3248;
  RG2L = unsignedlong(CUTX8(ddat.savegamep4->warr_00[2]));
  RG4L = 0;
  //m_2430D:
  for (;;)
  {
    if (RG4W >= RG2W)
    {
      RG1L = signedlong(RG3W);
      DM2_CHANGE_CURRENT_MAP_TO(RG1L);
      return;
    }
    //m_242FF:
    RG1L = signedlong(RG4W);
    DM2_CHANGE_CURRENT_MAP_TO(RG1L);
    RG1L = DM2_FILL_CAII_CUR_MAP();
    RG4L++;
  }
  FOREND
}
