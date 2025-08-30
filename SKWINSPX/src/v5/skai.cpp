#include <types.h>
#include <dm2data.h>
#include <SK0350.h>
#include <util.h>
#include <mdata.h>
#include <skgdtqdb.h>
#include <skrandom.h>
#include <skhero.h>
#include <bitem.h>
#include <skcloud.h>
#include <skmap.h>
#include <bcreatur.h>
#include <SK1C9A.h>
#include <skevents.h>
#include <startend.h>
#include <skai.h>

#include <emu.h>
#include <regs.h>

using namespace skv5;

// belongs to DM2_14cd_2886
static i32 DM2_14cd_2807(i16* eaxpw, i16* edxpw)
{
  c_nreg _RG1; RG1P = DOWNCAST(i16, eaxpw);
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4P = DOWNCAST(i16, edxpw);
  i8* RG5p;
  c_lreg _RG6;

  RG2P = RG1P;
  RG5p = RG4P;
  RG4L = signedlong(word_at(RG4P, 2));
  RG1L = DM2_CREATURE_CAN_HANDLE_IT(unsignedlong(word_at(RG2P)), RG4L);
  if (RG1L == 0)
    return 0;
  if (word_at(RG5p) == lcon(0xffffffff))
    mov16(location(RG5p), 0);
  if (word_at(RG5p + lcon(0x8)) == 0)
    //m_18738:
    RG1W = -1;
  else
    RG1W = DM2_ADD_ITEM_CHARGE(unsignedlong(word_at(RG2P)), 0);
  //m_1873D:
  RG1L = signedlong(RG1W);
  i32 parl01 = RG1L;
  i32 parl00 = unsignedlong(word_at(RG5p, lcon(0x6)));
  RG3L = unsignedlong(word_at(RG5p, lcon(0x4)));
  RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG4L = signedlong(word_at(RG1P, lcon(0x8)));
  RG6L = unsignedlong(byte_at(RG1P, lcon(0x4)));
  RG1L = DM2_query_48ae_05ae(signedlong(DM2_GET_DISTINCTIVE_ITEMTYPE(word_at(RG2P))), RG6L, RG4L, RG3L, parl00, parl01);
  add16(location(RG5p), RG1W);
  return 0;
}

static i16 DM2_14cd_2886(t_record* eaxp, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i32 argl1)
{
  c_nreg _RG1; RG1P = DOWNCAST(t_record, eaxp);
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 vwa_00[0x5];
  i16* pwa_0c[0x2];

  vwa_00[0x0] = lcon(0xffffffff);
  vwa_00[0x1] = RG4W;
  vwa_00[0x2] = RG3W;
  RG4L = argl0;
  vwa_00[0x3] = RG4W;
  RG4L = argl1;
  vwa_00[0x4] = RG4W;
  RG4L = unsignedlong(RG2Blo);
  DM2_OVERSEE_RECORD(RG1P16, RG4L, UPCAST(i16*, DOWNCAST(i16*, pwa_0c)), DM2_14cd_2807, vwa_00, 0, lcon(0x1));
  return vwa_00[0x0];
}

// was SKW_14cd_226d
static i8 DM2_PROCEED_XACT_56(void)
{
  if (DM2_CREATURE_GO_THERE(lcon(0x80), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), -1, -1, lrshift6e(s350.v1e054e->w_0e)) != 0)
    return -4;
  return -2;
}

// was SKW_14cd_22a3
static void DM2_PROCEED_XACT_57(void)
{
  i16 rg1;

  if (!DM2_RANDBIT())
    //m_18038:
    rg1 = -1;
  else
    rg1 = 1;
  //m_1803D:

  ui16 rg6 = lrshift6e(s350.v1e054e->w_0e);
  i16 rg5 = (rg1 + rg6) & 0x3;
  if (DM2_CREATURE_GO_THERE(lcon(0x80), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), -1, -1, rg5 != 0))
    return;
  if (DM2_CREATURE_GO_THERE(lcon(0x80), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), -1, -1, ((rg6 - rg1) & 0x3)) == 0)
    DM2_19f0_0559(rg5);
}

// was SKW_14cd_232b
static i8 DM2_PROCEED_XACT_59_76(void)
{
  i16 rg3 = s350.v1e0572;
  if (rg3 == -1)
    rg3 = s350.v1e07d8.w_04;
  if (s350.v1e0574 != 0)
    if (DM2_CREATURE_CAN_HANDLE_ITEM_IN(rg3, unsignedlong(s350.v1e054e->possession.w_00), lcon(0xff)) != -2)
      return -2;
  DM2_19f0_2165(lcon(0x80), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), signedlong(s350.creatures->w_18.GetX()), signedlong(s350.creatures->w_18.GetY()), -1, rg3);
  return s350.v1e056f;
}

// was SKW_14cd_2c23
static i8 DM2_PROCEED_XACT_62(void)
{
  c_nreg _RG1;
  i32 vl_04;
  i16 vw_0c;
  i16 vw_10;
  i16 vw_14;

  i32 vl_00 = 0;
  RG1W = s350.v1e057c;
  i32 vl_08 = RG1L;
  RG1W &= lcon(0x77);
  RG1L = unsignedlong(RG1W);
  if (RG1L != 0)
  {
    bool skip00337 = false;
    bool skip00335 = false;
    //m_18B88:
    RG1L = signedlong(s350.v1e0574);
    if (RG1L != 1)
      skip00335 = true;
    else
    {
      RG1W = DM2_CREATURE_CAN_HANDLE_ITEM_IN(16, unsignedlong(s350.v1e054e->possession.w_00), lcon(0xff));
      RG1Blo = RG1W != wcon(0xfffe) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L == 0)
        skip00335 = true;
    }

    if (skip00335)
    {
      //m_18BC2:
      if (s350.v1e0572 == 0)
      {
        //m_18BE1:
        vw_10 = s350.creatures->w_18.GetX();
        RG1UW = s350.creatures->w_18.GetY();
      }
      else
      {
        RG1L = unsignedlong(s350.v1e0562.getxA());
        vw_10 = RG1W;
        RG1L = unsignedlong(s350.v1e0562.getyA());
      }
      //m_18BFF:
      vw_0c = RG1W;
      if (DM2_FIND_TILE_ACTUATOR(signedlong(vw_10), signedlong(vw_0c), 255, 48) != -1)
      {
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
        RG1W = word_at(RG1P, 2);
        RG1UW >>= bcon(0x7);
        vw_14 = RG1W;
        RG1L = signedlong(s350.v1e0574);
        if (RG1L != 2)
        {
          //m_18CE5:
          RG1L = DM2_GET_WALL_TILE_ANYITEM_RECORD(signedlong(vw_10), vw_0c);
          //m_18CEE:
          i16 rg41;
          for (;;)
          {
            rg41 = RG1W;
            RG1L = rg41 != -2 ? 1 : 0;
            if (RG1L == 0)
              break;
            RG1L = mkul(rg41 & 0x3c00) >> 10;
            if (RG1L >= 5)
              break;
            RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(rg41)));
          }
          //m_18D1E:
          if (rg41 != -2)
          {
            RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(rg41);
            if (RG1W != vw_14)
            {
              //m_18D45:
              i16 rg2 = rg41;
              i16 rg4 = rg41;
              //m_18D4B:
              for (;;)
              {
                RG1Blo = rg4 != wcon(0xfffe) ? 1 : 0;
                RG1L = unsignedlong(RG1Blo);
                if (RG1L == 0)
                  break;
                RG1W = rg4 & 0x3c00;
                RG1L = unsignedlong(RG1W) >> 10;
                if (RG1L > lcon(0xa))
                  break;
                i16 rg52 = rg4;
                RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(rg52);
                if (RG1W == vw_14)
                  break;
                RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(rg52)));
                rg4 = RG1W;
              }
              //m_18D89:
              if (rg4 != -2)
              {
                vl_00 = 1;
                //m_18D9A:
                for (;;)
                {
                  RG1Blo = rg2 != wcon(0xfffe) ? 1 : 0;
                  RG1L = unsignedlong(RG1Blo);
                  if (RG1L == 0)
                    break;
                  RG1W = rg2 & lcon(0x3c00);
                  RG1L = unsignedlong(RG1W) >> bcon(0xa);
                  if (RG1L > lcon(0xa))
                    break;
                  i16 rg53 = rg2;
                  RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(rg53);
                  if (RG1W == vw_14)
                    break;
                  vl_04 = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(rg53)));
                  DM2_CUT_RECORD_FROM(mk_record(rg53), NULL, vw_10, CUTX16(vw_0c));
                  RG1W = rg53;
                  DM2_APPEND_RECORD_TO(mk_record(RG1W), NULL, vw_10, vw_0c);
                  rg2 = CUTX16(vl_04);
                }
              }
            }
            else
              vl_00 = 1;
          }
          //m_18E08:
          if (vl_00 != 0)
          {
            //m_18E4A:
            s350.v1e057c &= lcon(0x77);
            i32 parl00 = signedlong(vw_0c);
            DM2_19f0_2165(lcon(0x80), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), signedlong(vw_10), parl00, -1, 16);
            RG1L = vl_08;
            s350.v1e057c = RG1W;
            return s350.v1e056f;
          }
          if (s350.creatures->w_0e-- > 0)
            //m_18E3C:
            s350.creatures->b_1a = 29;
          else
          {
            s350.creatures->w_0e = 3;
            s350.creatures->b_1a = 30;
          }
        }
        else
        {
          RG1W = s350.v1e054e->w_0e;
          RG1UW = lrshift6e(RG1UW);
          RG1L = unsignedlong(RG1W);
          RG1L = DM2_GET_CREATURE_AT(mkul(unsignedword(s350.v1e0562.getxA()) + table1d27fc[RG1L]), mkul(unsignedword(s350.v1e0562.getyA()) + table1d2804[RG1L]));
          if (RG1W != -1)
          {
            RG1P = DOWNCAST(i16, DM2_1c9a_06bd(unsignedlong(RG1W), signedlong(vw_14), unsignedlong(CUTLX8((unsignedlong(lrshift6e(s350.v1e054e->w_0e)) + 2) & 0x3))));
            if (RG1P != NULL)
              skip00337 = true;
          }
        }
      }
    }
    else
      skip00337 = true;

    if (skip00337)
      //m_18BBB:
      return -2;
  }

  //m_18B81:
  return -3;
}

// was SKW_14cd_25b8
static i8 DM2_PROCEED_XACT_63(void)
{
  c_nreg _RG1;

  i8 ret = -3;
  RG1W = s350.v1e0572;
  i16 vw_00 = RG1W;
  if (RG1W != -1)
  {
    i16 rg41 = unsignedword(s350.v1e0562.getxA());
    i16 rg51 = unsignedword(s350.v1e0562.getyA());
    i8 rg2 = CUTX8(s350.v1e0574);
    RG1L = unsignedlong(rg2);
    if (RG1L != lcon(0xff))
    {
      RG1UW = lrshift6e(s350.v1e054e->w_0e);
      RG1L = (unsignedlong(rg2) + unsignedlong(RG1W) + 2) & 0x3;
      rg2 = RG1Blo;
    }
    RG1UW = lrshift6e(s350.v1e054e->w_0e);
    RG1L = unsignedlong(RG1W);
    rg41 += table1d27fc[RG1L];
    rg51 += table1d2804[RG1L];
    RG1L = unsignedlong(rg51);
    RG1L = DM2_GET_CREATURE_AT(unsignedlong(rg41), RG1L);
    if (RG1W != wcon(0xffff))
    {
      RG1W = DM2_CREATURE_CAN_HANDLE_ITEM_IN(vw_00, unsignedlong(word_at(DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW))), 2)), unsignedlong(rg2));
      RG1Blo = RG1W != wcon(0xfffe) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L != 0)
        ret = -2;
    }
  }
  return ret;
}

// was SKW_14cd_240e
static i8 DM2_PROCEED_XACT_64(void)
{
  c_nreg _RG1;
  i16 parw02;

  if (s350.v1e054e->possession.w_00 != -2)
  {
    i16 rg5 = s350.v1e057c;
    RG1W = rg5 & 0x8;
    if (RG1W != 0)
    {
      //m_18217:
      RG1W = s350.v1e0572;
      if (RG1W == -1)
        RG1W = 63;
      if (DM2_CREATURE_CAN_HANDLE_ITEM_IN(RG1W, unsignedlong(s350.v1e054e->possession.w_00), lcon(0xff)) != -2)
      {
        s350.v1e057c &= lcon(0x8);
        put16(parw02, RG1W);
        RG1UW = lrshift6e(s350.v1e054e->w_0e);
        DM2_19f0_2165(lcon(0x81), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), -1, -1, RG1W, parw02);
        s350.v1e057c = rg5;
        return s350.v1e056f;
      }
    }
  }
  //m_18210:
  return -3;
}

// was SKW_14cd_2e6e
static i8 DM2_PROCEED_XACT_65(void)
{
  c_nreg _RG1;
  bool skip00339 = false;

  s350.creatures->w_0c.set(-1);
  RG1UW = lrshift6e(s350.v1e054e->w_0e);
  RG1L = signedlong(RG1W);
  i16 rg3 = 2 * table1d27fc[RG1L] + unsignedword(s350.v1e0562.getxA());
  i16 rg2 = 2 * table1d2804[RG1L] + unsignedword(s350.v1e0562.getyA());
  RG1L = DM2_GET_CREATURE_AT(unsignedlong(rg3), unsignedlong(rg2));
  i16 rg4 = RG1W;
  RG1Blo = RG1W != wcon(0xffff) ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  if (RG1L != 0)
  {
    RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(rg4));
    if ((RG1Blo & bcon(0x1)) == 0)
      skip00339 = true;
  }

  if (!skip00339)
  {
    //m_18F14:
    if (ddat.v1d3248 != ddat.v1e08d6 || rg3 != ddat.v1e08d8 || rg2 != ddat.v1e08d4)
    {
      //m_18F3B:
      s350.creatures->b_1a = 29;
      return -4;
    }
  }

  //m_18F35:
  return -2;
}

static i32 DM2_14cd_2662(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG5l;
  c_ql32 vql_00;
  i8 vb_04;

  vb_04 = RG1Blo;
  RG2L = lcon(0xfffe);
  RG4L = unsignedlong(s350.v1e0562.getxA());
  RG3L = unsignedlong(s350.v1e0562.getyA());
  RG1L = unsignedlong(vb_04);
  if (RG1L != lcon(0xff))
  {
    RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
    RG1W = word_at(RG1P, lcon(0xe));
    RG1L <<= bcon(0x6);
    RG1UW >>= 14;
    RG5l = unsignedlong(RG1W) + 2;
    RG1L = unsignedlong(vb_04);
    RG1L += RG5l;
    RG1L &= 0x3;
    vql_00.poke32(RG1L);
    RG1Blo = vql_00.peek8();
    vb_04 = RG1Blo;
  }
  RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG1W = word_at(RG1P, lcon(0xe));
  RG1L <<= bcon(0x6);
  RG1UW >>= 14;
  RG1L = unsignedlong(RG1W);
  RG4W += table1d27fc[RG1L];
  RG3W += table1d2804[RG1L];
  RG3L = unsignedlong(RG3W);
  RG1L = unsignedlong(RG4W);
  RG4L = RG3L;
  RG1L = DM2_GET_CREATURE_AT(RG1L, RG4L);
  if (RG1W != lcon(0xffffffff))
  {
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
    RG2W = word_at(RG1P, 2);
    //m_18581:
    for (;;)
    {
      bool skip00322 = false;
      bool skip00325 = false;
      if (RG2W == lcon(0xfffffffe))
        break;
      RG1L = RG2L;
      RG1Blo ^= RG2Blo;
      RG1Bhi &= lcon(0x3c);
      RG1L = unsignedlong(RG1W) >> bcon(0xa);
      RG3L = signedlong(RG1W);
      if (RG3L > lcon(0x4) && RG3L < lcon(0xe))
        skip00322 = true;
      else
      {
        RG1L = signedlong(RG1W);
        if (RG1L == lcon(0x9))
          skip00322 = true;
      }

      if (skip00322)
      {
        bool skip00323 = false;
        //m_185AD:
        RG1L = unsignedlong(vb_04);
        if (RG1L == lcon(0xff))
          skip00323 = true;
        else
        {
          RG3L = unsignedlong(RG2W) >> bcon(0xe);
          if (RG1L == RG3L)
            skip00323 = true;
        }

        if (skip00323)
        {
          //m_185C6:
          RG3L = unsignedlong(RG2W);
          RG4L = lcon(0x10);
          RG1L = DM2_CREATURE_CAN_HANDLE_IT(RG3L, RG4L);
          if (RG1L == 0)
          {
            RG4L = lcon(0x7);
            RG1L = DM2_CREATURE_CAN_HANDLE_IT(RG3L, RG4L);
            if (RG1L == 0)
            {
              RG1L = 1;
              skip00325 = true;
            }
          }

          if (!skip00325)
            //m_185F2:
            RG1L = 0;
        }
      }

      //m_185F4:
      if (RG1L != 0)
        break;
      //m_185F8:
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG2UW)));
      RG2L = RG1L;
    }
  }
  //m_18609:
  RG1Blo = RG2W != wcon(0xfffe) ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  RG1Blo = RG1L != 0 ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  return RG1L;
}

// was SKW_14cd_274f
static i8 DM2_PROCEED_XACT_66(void)
{
  c_nreg _RG1;

  RG1L = DM2_14cd_2662(2);
  if (RG1L == 0)
  {
    //m_1866D:
    i16 rg21 = s350.v1e0572;
    s350.v1e0572 = 16;
    s350.v1e0574 = 2;
    RG1Blo = DM2_PROCEED_XACT_63();
    if (RG1Blo == -2)
      return -2;
    if (rg21 != 0)
    {
      s350.v1e0572 = lcon(0x7);
      RG1Blo = DM2_PROCEED_XACT_63();
      if (RG1Blo == -2)
        return -2;
    }
    i16 rg22 = s350.creatures->w_0e;
    s350.creatures->w_0e = rg22 - 1;
    if (rg22 <= 0)
    {
      s350.creatures->w_0e = 5;
      RG1Blo = -3;
      s350.creatures->b_1a = 30;
      return RG1Blo;
    }
    if (s350.creatures->w_0e > 5)
      s350.creatures->w_0e -= 5;
    RG1Blo = -3;
  }
  else
  {
    RG1Blo = -4;
    i16 rg3 = s350.creatures->w_0e;
    s350.creatures->w_0e--;
    if (rg3 <= 5)
    {
      s350.creatures->w_0e = 9;
      s350.creatures->b_1a = 31;
      return RG1Blo;
    }
  }
  //m_186EE:
  s350.creatures->b_1a = 29;
  return RG1Blo;
}

// was SKW_14cd_28d2
static i8 DM2_PROCEED_XACT_67(void)
{
  c_nreg _RG1;
  i8 vba_00[0x14];
  i32 vl_14;
  i16 vw_18;
  i16 vw_1c;
  bool skip00330 = false;
  bool skip00327 = false;

  i8 vb_24 = -3;
  RG1UW = lrshift6e(s350.v1e054e->w_0e);
  i16 vw_20 = RG1W;
  RG1L = DM2_14cd_2662(2);
  if (RG1L == 0)
  {
    //m_1882A:
    RG1L = signedlong(vw_20);
    RG1L = DM2_GET_CREATURE_AT(mkul(mkuw(table1d27fc[RG1L] + unsignedword(s350.v1e0562.getxA()))), mkul(unsignedword(s350.v1e0562.getyA()) + table1d2804[RG1L]));
    if (RG1W != -1)
    {
      s350.creatures->b_1a = 29;
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
      i16* rg7p16 = UPCAST(i16, RG1P + 2);
      RG1L = unsignedlong(CUTX8(vw_20));
      vl_14 = RG1L;
      RG1W = DM2_14cd_2886(CHGCAST(t_record, rg7p16), 16, RG1L, 0, 0, 0);
      i16 rg62 = RG1W;
      RG1W = DM2_14cd_2886(CHGCAST(t_record, rg7p16), 7, vl_14, 0, 0, 0);
      if (RG1W != -1)
      {
        if (rg62 != -1)
        {
          //m_188C9:
          RG1L = unsignedlong(RG1W);
          rg62 += RG1W;
        }
        else
          rg62 = RG1W;
      }
      //m_188D5:
      if (rg62 != -1)
      {
        //m_188E6:
        RG1Blo = (CUTX8(vw_20) + bcon(0x2)) & bcon(0x3);
        RG1L = DM2_query_48ae_0767(signedlong(DM2_14cd_2886(CHGCAST(t_record, rg7p16), 16, unsignedlong(RG1Blo), 1, 1, 0)), lcon(0x12), UPCAST(i16, ADRESSOF(i8, vba_00)), ADRESSOF(i16, &vw_1c)); // unused extra argument was 1
        i16 rg21 = RG1W;
        vw_18 = RG1W;
        i16 rg3 = s350.creatures->w_10;
        s350.creatures->w_10 = rg62;
        if (vw_18 > 16)
        {
          RG1L = unsignedlong(DM2_RAND16(16)) * vw_18;
          RG1L /= 100;
          rg21 = vw_18 - RG1W;
        }
        RG1L = (100 * unsignedlong(rg62)) / signedlong(rg21);
        i16 rg4 = RG1W;
        if (rg62 == rg3 || rg62 >= rg21)
        {
          //m_18A1D:
          if (rg62 < rg21)
          {
            //m_18A43:
            RG1W = s350.creatures->w_0e;
            i16 rg22 = RG1W - 1;
            s350.creatures->w_0e = rg22;
            if (RG1W > 0)
            {
              //m_18AAC:
              RG1L = signedlong(s350.creatures->w_0e);
              if (RG1L > lcon(0x6))
                s350.creatures->w_0e = rg22 - 4;
              s350.creatures->b_1a = 29;
              skip00330 = true;
            }
            else
            {
              bool skip00329 = false;
              if (rg4 <= 76)
                skip00329 = true;
              else
              {
                if ((DM2_RAND16(DM2_MAX(1, 100 - rg4)) < 5) == (DM2_RANDDIR() == 0))
                  skip00329 = true;
                else
                  s350.creatures->b_1a = 32;
              }

              if (skip00329)
                //m_18AA6:
                s350.creatures->b_1a = 27;
            }
          }
          else
          {
            RG1L = signedlong(DM2_MIN(rg62, vw_18));
            s350.creatures->w_0e = RG1W;
            s350.creatures->b_1a = 28;
            vb_24 = -2;
          }
        }
        else
        {
          if (rg3 == -1 || rg62 <= rg3)
            skip00327 = true;
          else
          {
            bool skip00328 = false;
            RG1L = signedlong(DM2_RANDDIR());
            if (RG1W == 0)
              skip00328 = true;
            else
            {
              RG1L = DM2_RAND();
              RG1W &= lcon(0x7);
              RG1L = unsignedlong(RG1W);
              RG1L += lcon(0x4c);
              if (signedlong(rg4) > RG1L)
                skip00328 = true;
            }

            if (skip00328)
            {
              //m_189C0:
              s350.creatures->w_0c.set(0);
              RG1W = DM2_RAND16(DM2_MAX(1, 100 - rg4));
              RG1Blo = RG1W < 5 ? 1 : 0;
              RG1L = mkul(DM2_RANDDIR() == wcon(0x0) ? 1 : 0) ^ mkul(RG1W & lcon(0xff));
              if (RG1L == 0)
                //m_18A14:
                s350.creatures->b_1a = 27;
              else
                s350.creatures->b_1a = 32;
              skip00330 = true;
            }
            else
              skip00327 = true;
          }
        }
      }
      else
        s350.creatures->w_10 = 0;
    }
  }
  else
  {
    if (s350.creatures->w_0e-- > 6)
      skip00327 = true;
    else
    {
      RG1L = signedlong(DM2_RANDDIR()) + lcon(0x9);
      s350.creatures->w_0e = RG1W;
      s350.creatures->b_1a = 31;
      skip00330 = true;
    }
  }

  if (skip00327)
  {
    //m_18821:
    s350.creatures->b_1a = 29;
    skip00330 = true;
  }

  if (skip00330)
    //m_18AC4:
    vb_24 = -4;

  //m_18AC9:
  return vb_24;
}

// was SKW_14cd_2f07
static i8 DM2_PROCEED_XACT_68(void)
{
  c_nreg _RG1;
  i8 vba_00[0x14];
  i16 vw_14;
  i32 vl_18;
  i16 vw_1c;
  i32 vl_20;
  i16 vw_24;
  i8 vb_28;

  vb_28 = -3;
  RG1UW = lrshift6e(s350.v1e054e->w_0e);
  vw_14 = RG1W;
  RG1L = DM2_14cd_2662(unsignedlong(mkub(CUTX8(s350.v1e0572) + bcon(0x2) & bcon(0x3))));
  if (RG1L == 0)
  {
    //m_18F97:
    RG1W = unsignedword(RG1Blo);
    RG1Blo = s350.v1e0562.getyA();
    RG1W += table1d2804[vw_14];
    i32 rg21 = unsignedlong(RG1W);
    RG1W = unsignedword(RG1Blo);
    RG1Blo = s350.v1e0562.getxA();
    RG1W += table1d27fc[vw_14];
    RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG1W), rg21);
    if (RG1W != -1)
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
      t_record* rg5p = UPCAST(t_record, RG1P + 2);
      RG1W = (vw_14 + s350.v1e0572) & 0x3;
      vw_14 = RG1W;
      RG1Blo = (CUTX8(vw_14) + bcon(0x2)) & bcon(0x3);
      i32 rg61l = unsignedlong(RG1Blo);
      RG1L = DM2_query_48ae_0767(signedlong(DM2_14cd_2886(rg5p, 16, rg61l, 1, 1, 0)), lcon(0x12), UPCAST(i16, ADRESSOF(i8, vba_00)), ADRESSOF(i16, &vw_24)); // unused extra argument was 1
      vl_18 = RG1L;
      vl_20 = RG1L;
      RG1W = DM2_14cd_2886(rg5p, 7, rg61l, 0, 0, 0);
      vw_1c = RG1W;
      if (RG1W == -1)
      {
        //m_19061:
        RG1L = 0;
        vw_1c = RG1W; // truncated
      }
      else
        vl_20 = vl_18 + RG1L;
      //m_19067:
      RG1W = DM2_14cd_2886(rg5p, 16, unsignedlong(CUTX8(vw_14)), 0, 0, 0);
      i16 rg62 = RG1W;
      RG1W = DM2_14cd_2886(rg5p, 7, unsignedlong(CUTX8(vw_14)), 0, 0, 0);
      if (rg62 == -1)
        rg62 = 0;
      if (RG1W != -1)
        rg62 += RG1W;
      RG1L = signedlong(s350.creatures->w_0e);
      i32 rg22 = unsignedlong(vw_1c) + RG1L;
      RG1L = unsignedlong(rg62);
      if (RG1L >= rg22)
      {
        //m_190CA:
        if (RG1L != signedlong(s350.creatures->w_10))
        {
          s350.creatures->b_1a = 28;
          s350.creatures->w_10 = rg62;
        }
        s350.creatures->w_0c.set(DM2_MAX(0, rg62 - CUTX16(vl_20)));
        vb_28 = -2;
      }
      else
        s350.creatures->b_1a = 27;
    }
  }
  else
  {
    vb_28 = -3;
    s350.creatures->b_1a = 31;
  }
  //m_190FF:
  return vb_28;
}

// was SKW_14cd_2398
static void DM2_PROCEED_XACT_69(void)
{
  c_nreg _RG1;

  i16 rg4 = lrshift6e(s350.v1e054e->w_0e);
  i16 rg2 = (unsignedword(s350.v1e0562.getxA()) + table1d27fc[rg4]) & 0x1f;
  s350.creatures->w_18.mask();
  i16 rg5 = s350.creatures->w_18.get() | rg2;
  s350.creatures->w_18.set(rg5);
  s350.creatures->w_18.set((rg5 & wcon(0xfc1f)) | (((unsignedword(s350.v1e0562.getyA()) + table1d2804[rg4]) & 0x1f) << 5));
  s350.creatures->b_1d = CUTX8(s350.v1e0572);
  s350.creatures->b_1a = (s350.creatures->b_1d == 1 ? 1 : 0) + 21;
}

// was SKW_14cd_248d
static i8 DM2_PROCEED_XACT_70(void)
{
  c_nreg _RG1;

  i8 ret = -3;
  RG1W = s350.v1e0572;
  i16 vw_00 = RG1W;
  if (RG1W == -1)
    vw_00 = lcon(0x3f);
  RG1Blo = s350.v1e0562.getxA();
  RG1W &= lcon(0x1f);
  s350.creatures->w_18.mask();
  i16 rg6 = s350.creatures->w_18.get() | RG1W;
  s350.creatures->w_18.set(rg6);
  RG1Blo = s350.v1e0562.getyA() & 0x1f;
  s350.creatures->w_18.set((rg6 & wcon(0xfc1f)) | (RG1W << 5));
  RG1UW = lrshift6e(s350.v1e054e->w_0e);
  RG1L = unsignedlong(RG1W) + 2;
  RG1L &= 0x3;
  s350.creatures->b_1c = RG1Blo;
  RG1Blo = CUTX8(vw_00);
  s350.creatures->b_1e = RG1Blo;
  i16 rg41 = (s350.creatures->w_18.GetX() + table1d27fc[lrshift6e(s350.v1e054e->w_0e)]) & 0x1f;
  s350.creatures->w_18.mask();
  RG1W = s350.creatures->w_18.get() | rg41;
  s350.creatures->w_18.set(RG1W);
  ui16 rg42 = (RG1UW << 6) >> 11;
  RG1UW = lrshift6e(s350.v1e054e->w_0e);
  RG1L = unsignedlong(RG1W);
  RG1W = ((table1d2804[RG1L] + rg42) & 0x1f) << 5;
  i16 rg5 = (s350.creatures->w_18.get() & wcon(0xfc1f)) | RG1W;
  s350.creatures->w_18.set(rg5);
  RG1W = rg5 << 6; // was longoperation
  RG1UW >>= bcon(0xb);
  RG1L = DM2_GET_CREATURE_AT(unsignedlong(mkuw(rg5 & wcon(0x1f))), unsignedlong(RG1W));
  if (RG1W != -1)
  {
    ret = s350.creatures->b_1c;
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
    RG1W = DM2_CREATURE_CAN_HANDLE_ITEM_IN(vw_00, unsignedlong(word_at(RG1P, 2)), unsignedlong(ret));
    if (RG1W != -2)
    {
      //m_183E8:
      s350.creatures->b_1a = 24;
      ret = -4;
    }
    else
      ret = -2;
  }
  //m_183EE:
  return ret;
}

// was SKW_14cd_3087
static i8 DM2_PROCEED_XACT_71(void)
{
  c_nreg _RG1;

  ObjectID* rg3p = &s350.v1e054e->possession;
  RG1W = s350.v1e0574;
  if (RG1W != -2)
  {
    bool flag;
    bool skip00341 = false;
    if (RG1W == -1)
    {
      RG1W = s350.v1e07d8.w_06;
      if (RG1W == -1)
      {
        //m_1913C:
        flag = false;
        skip00341 = true;
      }
    }

    if (!skip00341)
      //m_19135:
      flag = true;

    //m_1913E:
    if (flag)
      if (rg3p->w_00 != wcon(0xfffe))
        RG1L = DM2_1c9a_078b(DOWNCAST(ObjectID, rg3p), signedlong(RG1W), lcon(0xff));
  }
  //m_19160:
  if (rg3p->w_00 != -2)
  {
    //m_1916C:
    RG1W = s350.v1e0572;
    if (RG1W == -1)
    {
      RG1W = s350.v1e07d8.w_04;
      if (RG1W == -1)
        return -2;
    }
    if (DM2_CREATURE_CAN_HANDLE_ITEM_IN(RG1W, unsignedlong(rg3p->w_00), lcon(0xff)) != -2)
    {
      DM2_19f0_2165(lcon(0x81), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), -1, RG1W);
      return s350.v1e056f;
    }
  }
  //m_19166:
  return -3;
}

// was SKW_14cd_3aee
static void DM2_PROCEED_XACT_72_87_88(void)
{
  i8 n = CUTX8(s350.v1e0572);
  if (n == -1)
    n = CUTX8(s350.v1e07d8.w_04);
  s350.creatures->b_1a = n;
}

// was SKW_14cd_3139
static i8 DM2_PROCEED_XACT_73(void)
{
  c_nreg _RG1;
  bool flag1;
  bool skip00346 = false;
  bool skip00343 = false;

  RG1W = s350.v1e0574;
  if (RG1UW < mkuw(lcon(0x3)))
    skip00343 = true;
  else
  {
    bool skip00347 = false;
    if (RG1UW <= mkuw(lcon(0x4)))
    {
      //m_19273:
      RG1L = signedlong(RG1W);
      RG1Blo = RG1L != lcon(0x3) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo) + lcon(0x13);
      i8 rg3 = RG1Blo;
      i16 rg51 = s350.v1e054e->w_0a;
      RG1P = DOWNCAST(s_hexe, s350.v1e07d8.xp_0a);
      if (RG1P == NULL)
        skip00347 = true;
      else
      {
        //m_19291:
        for (;;)
        {
          if (rg3 == byte_at(RG1P, lcon(0xc)))
          {
            if (word_at(RG1P + lcon(0x4)) != 0)
            {
              //m_192AD:
              if (word_at(RG1P, lcon(0x4)) == 1)
                s350.v1e054e->w_0a &= ~(1 << byte_at(RG1P, lcon(0x6)));
            }
            else
              s350.v1e054e->w_0a |= 1 << byte_at(RG1P, lcon(0x6));
          }
          //m_192C8:
          unk* rg2p = RG1P;
          RG1P += lcon(0xe);
          if (byte_at(rg2p + lcon(0xd)) == 0)
          {
            if (rg51 != s350.v1e054e->w_0a)
              s350.creatures->b_1a = 51;
            flag1 = rg51 != s350.v1e054e->w_0a;
            break;
          }
        }
        skip00346 = true;
      }
    }
    else
    {
      if (RG1UW < mkuw(lcon(0x10)) || RG1UW > mkuw(lcon(0x12)))
        skip00347 = true;
      else
        skip00343 = true;
    }

    if (skip00347)
      //m_192E8:
      RG1Blo = -3;
  }

  if (skip00343)
  {
    bool skip00345 = false;
    //m_19202:
    bool flag2 = (RG1W & 0x10) == 0;
    RG1W &= lcon(0xf);
    i16 rg2 = 1 << CUTX8(s350.v1e0572);
    i32 rg5l = unsignedlong(s350.v1e054e->w_0a);
    rg5l &= unsignedlong(rg2);
    flag1 = rg5l == (i32)(unsignedlong(rg2) ? 1 : 0); // SPX CHECK
    i16 rg52 = s350.v1e054e->w_0a;
    if (RG1W != 0)
    {
      //m_19246:
      if (RG1W != 1)
        skip00345 = true;
      else
        RG1W = rg52 | rg2;
    }
    else
      RG1W = rg52 & ~rg2;

    if (!skip00345)
      //m_19250:
      s350.v1e054e->w_0a = RG1W;

    //m_19254:
    if (flag2)
      if (rg52 != s350.v1e054e->w_0a)
        s350.creatures->b_1a = 51;
    skip00346 = true;
  }

  if (skip00346)
    //m_19269:
    RG1Blo = (flag1 ? 1 : 0) - bcon(0x3);

  //m_15D04:
  return RG1Blo;
}

// was SKW_14cd_32a4
static i8 DM2_PROCEED_XACT_74(void)
{
  bool skip00349 = false;
  c_nreg _RG1;
  i16 parw00;
  i16 parw01;

  RG1L = unsignedlong(word_at(s350.v1e0552, lcon(0x16))) >> bcon(0xc);
  i16 rg4 = RG1W;
  if (rg4 == 0)
    skip00349 = true;
  else
  {
    if ((s350.v1e054e->w_0a & 0x2000) != 0)
      rg4 = rg4 / 4;
    //m_19332:
    RG1L = DM2_RAND();
    RG1W &= lcon(0xf);
    if (RG1W >= rg4)
      skip00349 = true;
    else
      rg4 = 1;
  }

  if (skip00349)
    //m_1934B:
    rg4 = 0;
  bool skip00353 = false;
  //m_1934D:
  bool flag = rg4 == 0;
  RG1L = DM2_1c9a_381c();
  if (RG1L != 0)
  {
    //m_19406:
    if (!flag)
      skip00353 = true;
    else
    {
      i8 rg4b = 0x80;
      if ((s350.v1e054e->w_0a & 0x1000) != 0)
        rg4b = 0xa0;
      RG1L = DM2_CREATURE_GO_THERE(unsignedlong(rg4b), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), -1, -1, signedword(s350.creatures->b_1b));
    }
  }
  else
  {
    bool skip00352 = false;
    bool skip00351 = false;
    RG1W = s350.creatures->w_18.GetX();
    if (unsignedword(s350.v1e0562.getxA()) != RG1W)
      skip00351 = true;
    else
    {
      RG1Blo = s350.v1e0562.getyA();
      if (RG1W != s350.creatures->w_18.GetY())
        skip00351 = true;
      else
      {
        RG1W = s350.creatures->w_18.get();
        RG1UW >>= 10;
        if (RG1W != ddat.v1d3248)
          skip00351 = true;
      }
    }

    if (skip00351)
    {
      //m_1939E:
      i16 rg4 = DM2_CALC_VECTOR_DIR(unsignedword(s350.v1e0562.getxA()), unsignedword(s350.v1e0562.getyA()), signedlong(s350.creatures->w_18.GetX()), s350.creatures->w_18.GetY());
      if (lrshift6e(s350.v1e054e->w_0e) == RG1UW)
        skip00352 = true;
      else
      {
        bool skip00354 = false;
        if (flag)
          skip00354 = true;
        else
        {
          RG1L = DM2_RANDBIT() ? 1 : 0;
          if (RG1W == 0)
            skip00354 = true;
        }

        if (skip00354)
          //m_193FC:
          RG1L = DM2_19f0_0559(rg4);
        else
          skip00353 = true;
      }
    }
    else
      skip00352 = true;

    if (skip00352)
      //m_19397:
      return -2;
  }

  if (skip00353)
  {
    //m_193F1:
    s350.creatures->b_1a = 0;
    return -4;
  }

  //m_1944B:
  return s350.v1e056f;
}

// belongs to DM2_ai_14cd_10d2
// stacksize was 0x1c
// recursive!!!
static i32 DM2_14cd_102e(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
{
  c_nreg _RG4; RG4L = edxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG1; RG1L = eaxl;
  c_wlreg _RG5;
  i32 RG6l;
  i32 RG7l;
  c_ql32 vql_00;
  i16 vw_04;
  i8 vb_08;
  i32 parl00;

  vql_00.poke32(RG1L);
  RG5L = RG4L;
  vb_08 = RG2Blo;
  RG7l = RG3L;
  RG6l = 0;
  //m_16A3D:
  for (;;)
  {
    bool skip00300 = false;
    bool skip00301 = false;
    if (RG5W == wcon(0xfffe))
      return RG6l;
    RG1L = RG5L;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    vw_04 = RG1W;
    if (RG7l == 0)
      skip00300 = true;
    else
    {
      RG1L = signedlong(vw_04);
      if (RG1L != lcon(0x4))
        skip00300 = true;
      else
      {
        parl00 = argl0;
        RG2L = unsignedlong(vb_08);
        RG1L = unsignedlong(RG5W);
      }
    }

    if (skip00300)
    {
      //m_16A7D:
      if (argl0 != 0)
      {
        RG4L = unsignedlong(RG5W);
        RG1L = DM2_IS_CONTAINER_CHEST(RG4W) ? 1 : 0;
        if (RG1L != 0)
        {
          parl00 = argl0;
          RG2L = unsignedlong(vb_08);
          RG1L = RG4L;
          skip00301 = true;
        }
      }
    }
    else
      skip00301 = true;

    if (skip00301)
    {
      //m_16AA1:
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
      RG4L = unsignedlong(word_at(RG1P, 2));
      RG1L = signedlong(vql_00.peek16());
      RG3L = RG7l;
      RG1L = DM2_14cd_102e(RG1L, RG4L, RG2L, RG3L, parl00);
      RG6l += RG1L;
    }

    //m_16ABC:
    RG1L = signedlong(vw_04);
    if (RG1L > lcon(0x4) && RG1L < lcon(0xe))
    {
      bool skip00303 = false;
      RG1L = unsignedlong(vb_08);
      if (RG1L == lcon(0xff))
        skip00303 = true;
      else
      {
        RG4L = unsignedlong(RG5W) >> bcon(0xe);
        if (RG1L == RG4L)
          skip00303 = true;
      }

      if (skip00303)
      {
        //m_16AE6:
        RG4L = signedlong(vql_00.peek16());
        RG1L = DM2_CREATURE_CAN_HANDLE_IT(unsignedlong(RG5W), RG4L);
        if (RG1L != 0)
          RG6l++;
      }
    }
    //m_16AFC:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG5UW)));
    RG5L = RG1L;
  }
  FOREND
}

static unk* DM2_ai_14cd_10d2(unk* xeaxp, i32 edxl)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p;
  i8* RG61p;
  i8* RG7p;
  c_ql32 vql_00;
  unk* xp_04;
  i32 vl_08;
  unk* xp_0c;
  i16 vw_10;
  i16 vw_14;
  c_o32 vo_18; // poke 16 peek 32 inc16
  i16 vw_1c;
  i16 vw_20;
  i8 vb_24;

  xp_04 = xeaxp;
  vql_00.poke32(RG4L);
  RG4L = 0;
  vw_10 = RG4W;
  vw_14 = RG4W;
  vw_1c = RG4W;
  vw_20 = RG4W;
  if (s350.v1e058c != 0)
  {
    DM2_ZERO_MEMORY(s350.v1e058e, lcon(0x80));
    s350.v1e058c = 0;
  }
  //m_16B58:
  RG4P = s350.v1e058e;
  xp_0c = UPCAST(unk, RG4P);
  RG5p = RG4P;
  vo_18.poke16(wcon(0x0));
  RG4L = vql_00.peek32();
  RG3P = DOWNCAST(unk, xp_04);
  //m_16B99:
  for (;;)
  {
    RG1L = signedlong(vo_18.peek16());
    if (RG1L >= lcon(0x4))
    {
      movp(location(DOWNCAST(unk, xp_0c)), DOWNCAST(unk, xp_04));
      RG1Blo = vql_00.peek8();
      RG4P = DOWNCAST(unk, xp_0c);
      mov8(location(RG4P + lcon(0x4)), RG1Blo);
      RG7p = RG4P + lcon(0x8);
      RG61p = RG7p;
      RG5p = RG7p;
      mov8(location(RG4P + lcon(0x7)), 0);
      RG1Blo = byte_at(RG4P, lcon(0x7));
      mov8(location(RG4P + lcon(0x6)), RG1Blo);
      mov8(location(RG4P + lcon(0x5)), RG1Blo);
      RG1Bhi = sgn8(RG1Blo);
      vo_18.poke16(RG1W);
      break;
    }
    //m_16B73:
    RG2P = pointer_at(RG5p);
    if (RG2P != NULL)
    {
      //m_16B7F:
      if (RG2P == RG3P)
      {
        RG1W = signedword(byte_at(RG5p, lcon(0x4)));
        if (RG1W == RG4W)
          return RG5p;
      }
    }
    else
      xp_0c = UPCAST(unk, RG5p);
    //m_16B91:
    vo_18.inc16();
    RG5p += lcon(0x20);
  }
  //m_16BD4:
  for (;;)
  {
    RG1L = signedlong(vo_18.peek16());
    if (RG1L > lcon(0x5))
      return DOWNCAST(unk, xp_0c);
    RG1P = DOWNCAST(unk, xp_04);
    RG1W = signedword(byte_at(RG1P, lcon(0xc)));
    if (RG1W == vql_00.peek16())
    {
      RG4P = DOWNCAST(unk, xp_04);
      RG4W = word_at(RG4P, lcon(0x4));
      if (RG4W != lcon(0xffffffff))
      {
        RG1P = DOWNCAST(unk, xp_04);
        if (byte_at(RG1P) != lcon(0x8))
        {
          RG1L = signedlong(byte_at(RG1P, lcon(0xa)));
          if (RG1L != lcon(0xffffffff))
          {
            RG3Blo = CUTX8(vw_1c);
            //m_16C25:
            for (;;)
            {
              if (RG61p >= RG7p)
                break;
              RG1Blo = CUTX8(vw_10) - RG3Blo;
              mov8(location(RG61p + lcon(0x1)), RG1Blo);
              RG61p += 4;
            }
            //m_16C37:
            RG1P = DOWNCAST(unk, xp_04);
            RG1W = signedword(byte_at(RG1P, lcon(0xa)));
            vw_10 = RG1W;
            RG1Blo = CUTX8(vw_10);
            RG3P = DOWNCAST(unk, xp_0c);
            add8(location(RG3P + lcon(0x6)), RG1Blo);
            RG1L = 0;
            vw_1c = RG1W;
          }
          //m_16C57:
          RG1L = signedlong(byte_at(DOWNCAST(unk, xp_04), lcon(0xb)));
          if (RG1L != lcon(0xffffffff))
          {
            RG2Blo = CUTX8(vw_20);
            //m_16C6A:
            for (;;)
            {
              if (RG5p >= RG7p)
                break;
              RG1Blo = CUTX8(vw_14) - RG2Blo;
              mov8(location(RG5p + 2), RG1Blo);
              RG5p += 4;
            }
            //m_16C7C:
            RG1P = DOWNCAST(unk, xp_04);
            RG1W = signedword(byte_at(RG1P, lcon(0xb)));
            vw_14 = RG1W;
            RG1Blo = CUTX8(vw_14);
            add8(location(DOWNCAST(unk, xp_0c) + lcon(0x7)), RG1Blo);
            RG2L = 0;
            vw_20 = RG2W;
          }
          //m_16C9C:
          RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
          RG1W = word_at(RG1P, 2);
          RG1L = unsignedlong(RG1W);
          vl_08 = RG1L;
          RG1L = DM2_14cd_102e(signedlong(RG4W), vl_08, lcon(0xff), 0, lcon(0x1));
          RG2L = RG1L;
          mov8(location(RG7p + 0), RG1Blo);
          add8(location(DOWNCAST(unk, xp_0c) + lcon(0x5)), RG1Blo);
          RG1W = vw_10 - vw_1c;
          RG4L = signedlong(RG1W);
          RG3L = signedlong(RG2W);
          RG1L = signedlong(DM2_MIN(RG3W, RG4W));
          vb_24 = RG1Blo;
          RG1Blo = vb_24;
          sub8(location(DOWNCAST(unk, xp_0c) + lcon(0x6)), RG1Blo);
          RG1W = vw_14 - vw_20;
          RG4L = signedlong(RG1W);
          RG1L = signedlong(DM2_MIN(RG3W, RG4W));
          sub8(location(DOWNCAST(unk, xp_0c) + lcon(0x7)), RG1Blo);
          RG1W = vw_1c + RG2W;
          RG4L = signedlong(RG1W);
          RG1L = signedlong(DM2_MIN(vw_10, RG4W));
          vw_1c = RG1W;
          RG2W += vw_20;
          RG4L = signedlong(RG2W);
          RG1L = signedlong(DM2_MIN(vw_14, RG4W));
          vw_20 = RG1W;
        }
      }
      //m_16D3F:
      RG7p += 4;
      vo_18.inc16();
    }
    //m_16D47:
    RG1P = DOWNCAST(unk, xp_04);
    xp_04 = UPCAST(unk, RG1P + lcon(0xe));
    if (byte_at(RG1P, lcon(0xd)) == 0)
      return DOWNCAST(unk, xp_0c);
  }
  FOREND
}

// was SKW_14cd_2162
static i8 DM2_PROCEED_XACT_75(void)
{
  unk* rg4p = DM2_ai_14cd_10d2(DOWNCAST(s_hexe, s350.v1e07d8.xp_0a), signedlong(s350.v1e07d8.b_02));
  i8 rg1 = s350.v1e07d8.b_03;
  if (rg1 != 8)
  {
    //m_17F05:
    if (rg1 != 9)
      //m_17F0D:
      rg1 = 0;
    else
      rg1 = 3;
  }
  else
    rg1 = 2;
  //m_17F0F:
  s350.creatures->b_1e = CUTX8(s350.v1e07d8.w_04);
  i16 rg5 = s350.v1e0578;
  if ((rg5 & 0x8) != 0)
    if (byte_at(rg4p + lcon(0x5)) <= 0)
      s350.v1e0578 &= lcon(0xfffffff7);
  s350.v1e0578 &= s350.v1e07d8.w_06;
  DM2_19f0_0891(mkul(rg1 | bcon(0x80)), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), signedlong(s350.creatures->w_18.GetX()), s350.creatures->w_18.GetY(), -1);
  s350.v1e0578 = rg5;
  return s350.v1e056f;
}

// stacksize was 0x10
static void DM2_ai_14cd_0f3c(i32 eaxl, unk* xedxp, s_hexe* ebxp_s18, i32 ecxl, i8 argb0, i32 argl1, i8 argb2, i8 argb3)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = DOWNCAST(s_hexe, ebxp_s18);
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4P = xedxp;
  i8* RG51p;
  i16 RG52w;
  i8* RG61p;
  c_ql32 vql_00;

  vql_00.poke32(RG1L);
  RG61p = RG4P;
  RG51p = RG2P;
  RG2Blo = argb0;
  if (RG51p == NULL || RG4P == NULL)
    return;
  RG1L = signedlong(s350.v1e0674);
  if (RG1L >= lcon(0x10))
    return;
  RG1Blo = byte_at(RG51p, lcon(0x8));
  RG4L = unsignedlong(s350.v1e0571);
  if (RG4W != ddat.v1e08d6)
  {
    if (RG1Blo > 0)
    {
      RG4P = s350.v1e0552;
      if (jz_test8(location(RG4P + lcon(0x1)), lcon(0x40)))
      {
        RG1Blo >>= bcon(0x2);
        RG2Blo >>= bcon(0x2);
      }
    }
  }
  RG1Bhi = sgn8(RG1Blo);
  RG4W = signedword(RG2Blo);
  RG1L += RG4L;
  RG4L = signedlong(RG1W);
  RG1L = signedlong(DM2_BETWEEN_VALUE(-1, 127, RG4W));
  RG4L = RG1L;
  if (RG1Blo >= 0)
  {
    RG4Bhi = s350.v1e0674;
    RG2L = signedlong(RG4Bhi);
    RG1L = 22 * RG2L;
    RG4Bhi++;
    s350.v1e0674 = RG4Bhi;
    RG1P = DOWNCAST(c_sizex16, &s350.v1e0678[RG2L]);
    RG4Bhi = argb2;
    mov8(location(RG1P + lcon(0xe)), RG4Bhi);
    RG4Bhi = argb3;
    mov8(location(RG1P + lcon(0xf)), RG4Bhi);
    RG2L = argl1;
    mov16(location(RG1P + lcon(0xc)), RG2W);
    mov8(location(RG1P), RG4Blo);
    RG4Blo = byte_at(RG51p, lcon(0x9));
    mov8(location(RG1P + lcon(0x1)), RG4Blo);
    RG4Blo = vql_00.peek8();
    mov8(location(RG1P + lcon(0x7)), RG4Blo);
    RG4W = word_at(RG51p, lcon(0x4));
    mov16(location(RG1P + lcon(0x8)), RG4W);
    RG52w = word_at(RG51p, lcon(0x6));
    RG4W = s350.v1e0580;
    RG52w &= RG4W;
    mov16(location(RG1P + lcon(0xa)), RG52w);
    mov8(location(RG1P + lcon(0x11)), RG3Blo);
    movp(location(RG1P + lcon(0x12)), RG61p);
  }
}

// was SKW_14cd_33e1
static i8 DM2_PROCEED_XACT_77(i8 eaxb)
{
  c_nreg _RG1; RG1Blo = eaxb;
  s_len8* xparp05;
  c_button* button;

  RG1W = unsignedword(RG1Blo);
  s350.v1e0674 = RG1Bhi;
  s_hexe* rg5p = s350.v1e07d8.xp_0a;
  if (rg5p != NULL)
  {
    //m_1946F:
    for (;;)
    {
      RG1L = signedlong(rg5p->b_0c);
      if (RG1L == 17)
        if (rg5p->w_04 != -1)
          DM2_ai_14cd_0f3c(lcon(0x14), DOWNCAST(s_hexe, rg5p), rg5p, RG1L, 0, lcon(0xffff), lcon(0xff), 0);
      RG1P = DOWNCAST(s_hexe, rg5p);
      rg5p++;
      if (byte_at(RG1P + lcon(0xd)) == 0)
      {
        if (s350.v1e0674 != 0)
        {
          button = &s350.v1e060e[0];
          xparp05 = &s350.v1e0678[0].s23_00;
          RG1L = signedlong(s350.v1e0674);
          RG1L = DM2_FIND_WALK_PATH(unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), lcon(0x3), RG1W, xparp05, button);
          s350.v1e0675 = RG1Blo;
          RG1L = signedlong(RG1Blo);
          if (RG1L != -1)
            return -2;
        }
        break;
      }
    }
  }
  //m_194F2:
  return -3;
}

// was SKW_14cd_3474
static i8 DM2_PROCEED_XACT_78(void)
{
  c_nreg _RG1;

  s350.v1e056f = -3;
  RG1W = ddat.v1d3248;
  if (RG1W == ddat.v1e08d6)
  {
    RG1L = signedlong(DM2_CALC_VECTOR_DIR(unsignedword(s350.v1e0562.getxA()), unsignedword(s350.v1e0562.getyA()), ddat.v1e08d8, ddat.v1e08d4));
    i32 rg5 = signedlong(RG1W);
    RG1L = (DM2_map_0cee_04e5(rg5, 1, 0, unsignedlong(s350.v1e0562.getxA()), unsignedword(s350.v1e0562.getyA())) & lcon(0xff)) >> 5;
    if (RG1Blo != 0)
      DM2_19f0_0559(CUTX16(rg5));
  }
  return s350.v1e056f;
}

// was SKW_14cd_221a
static void DM2_PROCEED_XACT_79(void)
{
  c_nreg _RG1;

  i8 rg3 = DM2_RANDBIT() ? 1 : 0;
  s350.creatures->b_1e = bcon(0x82);
  s350.creatures->b_1a = (rg3 != 0 ? 1 : 0) + 39;
  RG1L = signedlong(DM2_RANDDIR());
  s350.creatures->b_1b = RG1Blo;
  RG1Blo += rg3;
  RG1Blo &= 0x3;
  s350.creatures->b_1c = RG1Blo;
  s350.creatures->b_20 = 0;
}

// was SKW_14cd_34d3
static i8 DM2_PROCEED_XACT_80(void)
{
  c_nreg _RG1;
  i16 parw00;
  i16 parw01;

  i16 rg4 = s350.v1e0572;
  if (rg4 != 0)
    //m_1958F:
    RG1Blo = 6;
  else
    RG1Blo = 0;
  //m_19591:
  i8 rg3 = RG1Blo;
  i16 rg5 = s350.v1e0576;
  s350.v1e0576 = rg5 | lcon(0x1800);
  RG1UW = lrshift6e(s350.v1e054e->w_0e);
  RG1L = signedlong((CUTX16(unsignedlong(RG1W) + signedlong(rg4)) & lcon(0x3)));
  put16(parw01, RG1W);
  put16(parw00, -1);
  RG1Blo = rg3 | ubcon(0x80);
  DM2_CREATURE_GO_THERE(unsignedlong(RG1Blo), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), -1, parw00, parw01);
  s350.v1e0576 = rg5;
  return s350.v1e056f;
}

// was SKW_14cd_3535
static i8 DM2_PROCEED_XACT_81(void)
{
  DM2_19f0_2813(mkul(CUTX8(s350.v1e07d8.w_06) | bcon(0x80)), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), s350.creatures->w_18.GetX(), s350.creatures->w_18.GetY(), -1, s350.v1e07d8.w_04);
  return s350.v1e056f;
}

// belongs to DM2_PROCEED_XACT_82
static void DM2_14cd_3582(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  i32 RG6l;
  c_ql32 vql_00;
  i16 vwa_04[0xa];
  i16 vwa_18[0xa];

  vql_00.poke32(RG1L);
  RG5w = RG4W;
  RG6l = 0;
  RG1L = unsignedlong(RG4W);
  RG4P = ADRESSOF(i16, vwa_18);
  DM2_COUNT_BY_COIN_TYPES(RG1L, RG4P16);
  RG3L = 0;
  RG2L = 0;
  //m_1967D:
  for (;;)
  {
    if (RG2W >= ddat.v1e03fe)
    {
      RG4L = lcon(0x14);
      DM2_ZERO_MEMORY(ADRESSOF(i16, vwa_04), RG4L);
      RG2W = ddat.v1e03fe;
      break;
    }
    //m_19669:
    RG1L = signedlong(RG2W);
    RG4W = ddat.v1e03ac[RG1L];
    RG4W *= vwa_18[RG1L];
    RG3L += RG4L;
    RG2L++;
  }
  //m_196DD:
  for (;;)
  {
    RG2L--;
    if (RG2W < 0 || RG3W <= 0)
    {
      if (RG6l != 0)
      {
        RG1L = signedlong(vql_00.peek16());
        if (RG1L != lcon(0x1))
        {
          //m_196FF:
          RG2L = 0;
          //m_19723:
          for (;;)
          {
            if (RG2W >= ddat.v1e03fe)
            {
              RG2L = 0;
              break;
            }
            //m_19703:
            for (;;)
            {
              RG4L = signedlong(RG2W);
              RG1L = DM2_TAKE_COIN_FROM_WALLET(unsignedlong(RG5w), RG4L);
              if (RG1W == lcon(0xffffffff))
                break;
              RG1L = unsignedlong(RG1W);
              DM2_DEALLOC_RECORD(RG1L);
            }
            //m_19722:
            RG2L++;
          }
          //m_19770:
          for (;;)
          {
            if (RG2W >= ddat.v1e03fe)
              return;
            //m_19730:
            for (;;)
            {
              RG1L = 2 * signedlong(RG2W);
              RG4W = vwa_04[RG1L / 2];
              RG3L = RG4L - 1;
              vwa_04[RG1L / 2] = RG3W;
              if (RG4W == 0)
                break;
              RG1W = ddat.v1e0394[RG1L / 2];
              RG1L = DM2_ALLOC_NEW_DBITEM(unsignedlong(RG1W));
              if (RG1W != lcon(0xffffffff))
              {
                RG4L = unsignedlong(RG1W);
                DM2_ADD_COIN_TO_WALLET(unsignedlong(RG5w), RG4L);
              }
            }
            //m_1976F:
            RG2L++;
          }
          FOREND
        }
      }
      break;
    }
    //m_1969D:
    for (;;)
    {
      RG1L = 2 * signedlong(RG2W);
      RG4W = ddat.v1e03ac[RG1L / 2];
      if (RG3W < RG4W)
        break;
      vwa_04[RG1L / 2]++;
      RG3L -= RG4L;
    }
    //m_196B7:
    RG4W = vwa_18[RG1L / 2];
    RG1Blo = RG4W != vwa_04[RG1L / 2] ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG6l != 0 || RG1L != 0)
    {
      RG1L = signedlong(vql_00.peek16());
      if (RG1L == lcon(0x1))
        return;
    }
  }
  //m_196F8:
}

// was SKW_14cd_36b3
static i8 DM2_PROCEED_XACT_82(void)
{
  c_nreg _RG1;
  i8 vba_00[0x14];
  i16 vw_14;
  i8 vb_18;

  vb_18 = -3;
  i16 rg61 = -1;
  ui16 rg3 = lrshift6e(s350.v1e054e->w_0e);
  RG1L = signedlong(rg3) + unsignedlong(s350.v1e0562.getxA());
  RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG1W), mkul(unsignedword(s350.v1e0562.getyA()) + table1d2804[rg3]));
  if (RG1W != rg61)
  {
    bool skip00357 = false;
    s350.creatures->b_1a = 29;
    i16 rg21 = unsignedlong(RG1W);
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(rg21));
    i8* rg51p = RG1P; // TODO type is t_record
    if (s350.v1e0572 != 0)
    {
      //m_198B0:
      RG1L = signedlong(s350.v1e0572);
      if (RG1L != 1)
      {
        //m_198C8:
        if (RG1L != 2)
          //m_198D8:
          rg51p = DOWNCAST(i16, &s350.creatures->w_0c);
        else
          rg51p = DOWNCAST(i16, &s350.creatures->w_0e);
      }
      else
        rg51p = DOWNCAST(i16, &s350.creatures->w_10);
      //m_198E1:
      i16 rg2 = s350.v1e054e->possession.w_00;
      //m_198EB:
      while (rg2 != -2)
      {
        RG1L = DM2_IS_CONTAINER_MONEYBOX(unsignedlong(rg2));
        if (RG1L != 0)
        {
          rg61 = rg2;
          DM2_14cd_3582(0, unsignedlong(rg2));
        }
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(rg2)));
        rg2 = RG1W;
      }
      //m_19918:
      s350.v1e0572 = lcon(0x47);
      if (word_at(rg51p) > 0)
      {
        //m_1993F:
        RG1L = signedlong(word_at(rg51p));
        RG1L = DM2_query_48ae_0767(RG1L, lcon(0x12), UPCAST(i16, ADRESSOF(i8, vba_00)), ADRESSOF(i16, &vw_14)); // unused extra argument was 0
        mov16(location(rg51p), -1);
        if (vw_14 <= 0)
          skip00357 = true;
        else
        {
          i16 rg53 = vw_14 - 1;
          //m_1996B:
          for (;;)
          {
            RG1L = unsignedlong(rg53);
            RG1Blo = vba_00[RG1L];
            RG1L = DM2_ALLOC_NEW_DBITEM(unsignedlong(ddat.v1e0394[unsignedlong(RG1Blo)]));
            if (RG1W == -1)
              break;
            if (rg61 == -1)
            {
              //m_199A7:
              RG1L = unsignedlong(RG1W);
              DM2_APPEND_RECORD_TO(mk_record(RG1L), UPCAST(t_record, DOWNCAST(ObjectID, &s350.v1e054e->possession)), -1, 0);
            }
            else
              RG1L = DM2_ADD_COIN_TO_WALLET(unsignedlong(rg61), unsignedlong(RG1W));
            //m_199C1:
            if (--rg53 == -1)
              break;
          }
          //m_199C8:
          RG1Blo = DM2_PROCEED_XACT_64();
          vb_18 = -4;
        }
      }
      else
      {
        RG1Blo = DM2_PROCEED_XACT_64();
        vb_18 = RG1Blo;
        if (RG1Blo != -4)
          skip00357 = true;
      }
    }
    else
    {
      bool skip00356 = false;
      RG1UW = lrshift6e(s350.v1e054e->w_0e);
      RG1L = unsignedlong(RG1W) + 2 & lcon(0x3);
      RG1W = DM2_CREATURE_CAN_HANDLE_ITEM_IN(62, unsignedlong(word_at(DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(rg21))), 2)), unsignedlong(RG1Blo));
      i16 rg62 = RG1W;
      if (RG1W == -2)
        rg62 = -1;
      RG1Blo = (CUTX8(rg3) + bcon(0x2)) & bcon(0x3);
      RG1W = DM2_14cd_2886(UPCAST(t_record, rg51p + 2), lcon(0x10), unsignedlong(RG1Blo), 0, 1, 1);
      i16 rg52 = RG1W;
      if (RG1W != 0 && RG1W != -1)
        skip00356 = true;
      else
      {
        RG1L = rg62 != -1 ? 1 : 0;
        if (RG1L != 0)
          skip00356 = true;
      }

      if (skip00356)
      {
        //m_19892:
        RG1L = signedlong(DM2_MAX(0, rg52));
        s350.creatures->w_10 = RG1W;
        skip00357 = true;
      }
    }

    if (skip00357)
      //m_198A6:
      vb_18 = -2;
  }

  //m_199D2:
  return vb_18;
}

// was SKW_14cd_38c2
static i8 DM2_PROCEED_XACT_83(void)
{
  c_nreg _RG1;

  i8 rg3 = -2;
  RG1W = s350.v1e054e->w_0a & wcon(0x80);
  RG1L = unsignedlong(RG1W);
  if (RG1L != 0 || s350.v1e0572 != 0)
  {
    //m_19A05:
    RG1L = signedlong(DM2_BETWEEN_VALUE(0, 2, s350.v1e0572));
    s350.creatures->b_1a = RG1Blo + bcon(0x23);
    RG1W = s350.v1e054e->w_0a & wcon(0x80);
    RG1L = unsignedlong(RG1W);
    if (RG1L != 0)
    {
      RG1L = signedlong(s350.v1e0572);
      if (RG1L == 1)
        rg3 = -4;
    }
  }
  else
    rg3 = -3;
  //m_19A4A:
  return rg3;
}

// was SKW_14cd_3919
static i8 DM2_PROCEED_XACT_84(void)
{
  c_nreg _RG1;
  bool flag;

  i8 vb_00 = -3;
  i16 rg4 = s350.v1e054e->possession.w_00;
  if (rg4 != -2)
  {
    i16 rg5 = rg4;
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(rg5));
    i16 rg21 = (mkul(rg4 & 0x3c00) >> 10) - 5;
    if (rg21 > 5)
      RG1L = 0;
    else
    {
      bool skip00360 = false;
      bool skip00359 = false;
      switch (rg21)
      {
        case 0:
        case 1:
          //m_19AB6:
          flag = jz_test8(location(RG1P + 2), lcon(0xffffff80));
          skip00359 = true;
          break;

        case 2:
        case 4:
          RG1L = 0;
          break;

        case 3:
          //m_19AE1:
          flag = jz_test8(location(RG1P + lcon(0x3)), lcon(0xffffff80));
          skip00359 = true;
          break;

        case 5:
          //m_19AC4:
          if (jz_test8(location(RG1P + 2), lcon(0xffffff80)))
            skip00360 = true;
          else
          {
            RG1L = DM2_QUERY_GDAT_FOOD_VALUE_FROM_RECORD(rg5);
            if (RG1W == 0)
              //m_19ADD:
              RG1L = 0;
            else
              skip00360 = true;
          }
          break;

        default: throw(THROW_DMABORT);
      }

      if (skip00360)
        //m_19AD6:
        RG1L = 1;

      if (skip00359)
      {
        //m_19ABA:
        RG1Blo = flag ? 1 : 0;
        RG1L = unsignedlong(RG1Blo);
      }
    }

    //m_19AE9:
    if (RG1L == 0)
    {
      //m_19B23:
      RG1L = DM2_RAND();
      RG1W &= lcon(0x7);
      RG1L = unsignedlong(RG1W);
      if (RG1L != 0)
      {
        //m_19BA4:
        RG1L = DM2_RAND();
        RG1W &= lcon(0xf);
        RG1L = unsignedlong(RG1W);
        if (RG1L == 0)
          s350.creatures->w_0e = -1;
      }
      else
      {
        DM2_19f0_2165(lcon(0x81), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), -1, 6);
        RG1P = DOWNCAST(i16, &s350.creatures->w_0e);
        and8(location(RG1P), lcon(0xffffffe0));
        i16 rg22 = word_at(RG1P) | (unsignedword(s350.v1e0562.getxA()) & 0x1f);
        mov16(location(RG1P), rg22);
        mov16(location(RG1P), (rg22 & lcon(0xfc1f)) | ((unsignedword(s350.v1e0562.getyA()) & 0x1f) << 5));
        and8(location(RG1P + 1), lcon(0x3));
        or16(location(RG1P), (unsignedword(s350.v1e0571) & lcon(0x3f)) << 10);
      }
    }
    else
    {
      RG1P = DOWNCAST(ObjectID, &s350.v1e054e->possession);
      DM2_CUT_RECORD_FROM(mk_record(unsignedlong(rg4)), UPCAST(t_record, RG1P), -1, 0);
      DM2_DEALLOC_RECORD(unsignedlong(rg4));
      s350.creatures->b_1a = 0;
      vb_00 = -2;
    }
  }

  //m_19BBF:
  return vb_00;
}

// was SKW_14cd_3a0b
static i8 DM2_PROCEED_XACT_85(void)
{
  c_nreg _RG1;

  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(unsignedword(s350.v1e0562.getxA()), unsignedword(s350.v1e0562.getyA())));
  //m_19BDE:
  for (;;)
  {
    i16 rg4 = RG1W;
    RG1Blo = rg4 != wcon(0xfffe) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == 0)
      break;
    RG1W = rg4 & 0x3c00;
    RG1L = unsignedlong(RG1W) >> 10;
    RG1L = signedlong(RG1W);
    if (RG1L > lcon(0x3))
      break;
    if (RG1L == 2)
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(rg4));
      if ((word_at(RG1P, 2) & 0x6) == 0x2)
      {
        RG1W = word_at(RG1P, 2);
        RG1UW >>= bcon(0x3);
        if (((RG1UW >> 8) & 0x1f) == 0x1)
        {
          s350.creatures->b_1e = RG1Blo;
          s350.creatures->b_1a = 59;
          return -2;
        }
      }
    }
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(rg4)));
  }
  //m_19C65:
  DM2_ai_13e4_0360(unsignedlong(s350.v1e054c), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), lcon(0x13), 1);
  s350.creatures->b_1a = 51;
  return -3;
}

// was SKW_14cd_3abf
static i8 DM2_PROCEED_XACT_86(void)
{
  s350.creatures->b_20 = CUTX8(s350.v1e07d8.w_04);
  s350.creatures->b_1e = CUTX8(s350.v1e07d8.w_06);
  s350.creatures->b_1a = CUTX8(s350.v1e0572) + 61;
  return -2;
}

// was SKW_14cd_3b0b
static i8 DM2_PROCEED_XACT_89(void)
{
  DM2_19f0_0d10(unsignedlong(mkub(CUTX8(s350.v1e07d8.w_06) | bcon(0x80))), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), s350.creatures->w_18.GetX(), s350.creatures->w_18.GetY(), -1);
  return s350.v1e056f;
}

// was SKW_14cd_3b4c
static i8 DM2_PROCEED_XACT_90(void)
{
  return (s350.v1e0572 > DM2_RAND16(100) ? 1 : 0) - 3;
}

// was SKW_14cd_3b69
static i8 DM2_PROCEED_XACT_91(void)
{
  if (DM2_CREATURE_CAN_HANDLE_ITEM_IN(s350.v1e0572, unsignedlong(s350.v1e054e->possession.w_00), lcon(0xff)) == -2)
    if (DM2_CREATURE_CAN_HANDLE_ITEM_IN(s350.v1e0574, unsignedlong(s350.v1e054e->possession.w_00), lcon(0xff)) == -2)
      //m_19DB8:
      return -3;
  //m_19DB3:
  return -2;
}

static i8 DM2_PROCEED_XACT(i8 eaxb)
{
  i8 ret = -2;
  i8 opt = eaxb - 63;
  if (opt <= 35)
  {
    switch (opt)
    {
      case 0:
        //m_17D9E:
        ret = DM2_PROCEED_XACT_56();
        break;

      case 1:
        //m_17DA8:
        DM2_PROCEED_XACT_57();
        break;

      case 2:
        //m_17DB2:
        s350.creatures->b_1a = 19;
        break;

      case 3:
        //m_17DD2:
        ret = DM2_PROCEED_XACT_59_76();
        break;

      case 4:
        //m_17DDC:
        s350.creatures->b_1a = 0;
        break;

      case 5:
        break;

      case 6:
        //m_17DEA:
        ret = DM2_PROCEED_XACT_62();
        break;

      case 7:
        //m_17DF4:
        ret = DM2_PROCEED_XACT_63();
        break;

      case 8:
        //m_17DFE:
        ret = DM2_PROCEED_XACT_64();
        break;

      case 9:
        //m_17E08:
        ret = DM2_PROCEED_XACT_65();
        break;

      case 10:
        //m_17E12:
        ret = DM2_PROCEED_XACT_66();
        break;

      case 11:
        //m_17E1C:
        ret = DM2_PROCEED_XACT_67();
        break;

      case 12:
        //m_17E26:
        ret = DM2_PROCEED_XACT_68();
        break;

      case 13:
        //m_17E30:
        DM2_PROCEED_XACT_69();
        break;

      case 14:
        //m_17E3A:
        ret = DM2_PROCEED_XACT_70();
        break;

      case 15:
        //m_17E44:
        ret = DM2_PROCEED_XACT_71();
        break;

      case 16:
      case 31:
      case 32:
        //m_17EB5:
        DM2_PROCEED_XACT_72_87_88();
        break;

      case 17:
        //m_17E4E:
        ret = DM2_PROCEED_XACT_73();
        break;

      case 18:
        //m_17E58:
        ret = DM2_PROCEED_XACT_74();
        break;

      case 19:
        //m_17E62:
        ret = DM2_PROCEED_XACT_75();
        break;

      case 20:
        //m_17DC0:
        s350.v1e0572 = -1;
        s350.v1e0574 = 0;
        //m_17DD2:
        ret = DM2_PROCEED_XACT_59_76();
        break;

      case 21:
        //m_17E6C:
        ret = DM2_PROCEED_XACT_77(opt);
        break;

      case 22:
        //m_17E73:
        ret = DM2_PROCEED_XACT_78();
        break;

      case 23:
        //m_17E7A:
        DM2_PROCEED_XACT_79();
        break;

      case 24:
        //m_17E84:
        ret = DM2_PROCEED_XACT_80();
        break;

      case 25:
        //m_17E8B:
        ret = DM2_PROCEED_XACT_81();
        break;

      case 26:
        //m_17E92:
        ret = DM2_PROCEED_XACT_82();
        break;

      case 27:
        //m_17E99:
        ret = DM2_PROCEED_XACT_83();
        break;

      case 28:
        //m_17EA0:
        ret = DM2_PROCEED_XACT_84();
        break;

      case 29:
        //m_17EA7:
        ret = DM2_PROCEED_XACT_85();
        break;

      case 30:
        //m_17EAE:
        ret = DM2_PROCEED_XACT_86();
        break;

      case 33:
        //m_17EBF:
        ret = DM2_PROCEED_XACT_89();
        break;

      case 34:
        //m_17EC6:
        ret = DM2_PROCEED_XACT_90();
        break;

      case 35:
        //m_17ECD:
        ret = DM2_PROCEED_XACT_91();
        break;

      default: throw(THROW_DMABORT);
    }
  }

  //m_17ED4:
  return ret;
}
static void DM2_13e4_01a3(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 vw_00;

  if (s350.v1e07eb != 0)
    return;
  s350.v1e07eb = 1;
  RG2P = s350.v1e0552;
  RG3P = DOWNCAST(SPX_Creature, s350.v1e054e);
  if (s350.v1e0584 == lcon(0xffffffff))
  {
    RG1L = DM2_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(byte_at(RG3P, lcon(0x4))), lcon(0x1));
    s350.v1e0584 = RG1W;
  }
  RG1W = word_at(RG2P, lcon(0xa));
  s350.v1e0576 = RG1W;
  RG1W = word_at(RG2P, lcon(0xe));
  s350.v1e0578 = RG1W;
  RG1W = word_at(RG2P, lcon(0x10));
  s350.v1e057a = RG1W;
  RG1W = word_at(RG2P, lcon(0xc));
  s350.v1e057c = RG1W;
  RG1W = word_at(RG2P, lcon(0x12));
  s350.v1e057e = RG1W;
  RG1L = DM2_query_1c9a_08bd(RG3P);
  if (RG1L != 0)
    s350.v1e0576 &= lcon(0x7fff);
  s350.v1e058c = 1;
  DM2_ZERO_MEMORY(s350.v1e07ee, lcon(0xa8));
  s350.v1e0898 = NULL;
  s350.v1e0896 = 4;
  RG1W = unsignedword(RG1Blo);
  s350.v1e07ec = RG1Bhi;
  RG1L = DM2_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(byte_at(RG3P, lcon(0x4))), lcon(0x7));
  s350.v1e0582 = RG1W;
  RG3P = DOWNCAST(c_creature, s350.creatures);
  RG1Blo = CUTLX8(timdat.gametick) - byte_at(RG3P, lcon(0x4));
  RG3W = signedword(RG1Blo);
  if (RG3W < 0)
    RG3Bhi++;
  RG1W = word_at(RG2P, lcon(0x16)) & lcon(0xf);
  RG2L = 2 * (lcon(0xf) - RG1L);
  RG1L = lcon(0x1837a7);
  if (RG1L == 0)
    //m_14DB5:
    RG4L = 0;
  else
    RG4L = signedlong(RG2W) / 4;
  //m_14DB7:
  RG2L++;
  RG1W = DM2_RAND16(RG2W);
  RG4W += RG1W;
  RG1W = RG3W;
  RG1Blo = RG4W <= RG1W ? 1 : 0;
  s350.v1e058d = RG1Blo;
  RG1W = s350.v1e054c;
  RG1Bhi &= 0x3;
  RG1L = unsignedlong(RG1W) | ulcon(0x20000000);
  RG1L = dm2_dballochandler.DM2_ALLOCATION11(RG1L, 0, OO /*TAG dbidx*/ vw_00);
  if (RG1L == 0)
  {
    DM2_ZERO_MEMORY(DOWNCAST(c_4b3wp, &s350.v1e07d8), lcon(0xe));
    s350.v1e07d8.b_03 = lcon(0xffffffff);
    s350.v1e07d8.w_08 = lcon(0xffffffff);
    return;
  }
  //m_14E17:
  DM2_COPY_MEMORY(DOWNCAST(c_4b3wp, &s350.v1e07d8), DOWNCAST(t_bmp, dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ vw_00)), lcon(0xe)); // TODO: ATTENTION destination type
}

static i32 DM2_14cd_062e(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;

  RG4Blo = 0;
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4Bhi = byte_at(RG1P, lcon(0x12));
  RG1Blo = byte_at(RG1P, lcon(0x13));
  if (RG4Bhi != lcon(0xffffffff))
  {
    RG4L = signedlong(RG4Bhi);
    RG2L = signedlong(RG1Blo);
    RG1L = 7 * RG2L;
    RG4P = DOWNCAST(s_seven, table1d5f82[RG4L]);
    RG1P = RG4P + RG1L;
    RG4Blo = byte_at(RG1P, lcon(0x5)) & lcon(0xffffffe0);
    RG1Blo = byte_at(RG1P, lcon(0x5)) & lcon(0x60);
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == lcon(0x40))
    {
      RG1L = unsignedlong(s350.v1e0571);
      if (RG1W != ddat.v1e08d6)
        RG4Blo = 0;
    }
  }
  RG1Blo = RG4Blo;
  return RG1L;
}

// belongs to DM2_14cd_0f0a
static void DM2_14cd_18cc(i32 eaxl, i32 edxl, unk* xebxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i8 parb02;
  i8 parb03;

  RG1Bhi = RG1Blo;
  RG1Blo = RG4Blo;
  RG4P = RG2P;
  RG2L = signedlong(RG1Bhi);
  put8(parb03, RG2Blo);
  RG1L = signedlong(RG1Blo);
  put8(parb02, RG1Blo);
  DM2_ai_14cd_0f3c(0, RG4P, CHGCAST(s_hexe, getadr_r1d41d6()), 0, 0, lcon(0xffff), parb02, parb03);
}

// belongs to DM2_14cd_1316
static i32 DM2_2c1d_09d9(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_lreg _RG5;
  c_wlreg _RG6;

  RG5L = 0;
  RG6L = 0;
  //m_362CA:
  for (;;)
  {
    RG4L = signedlong(RG6W);
    RG1L = unsignedlong(party.heros_in_party);
    if (RG4L >= RG1L)
    {
      RG2L = 1;
      break;
    }
    //m_362A4:
    RG2L = 0;
    //m_362BF:
    for (;;)
    {
      RG3L = unsignedlong(RG2W);
      if (RG3L > lcon(0x3))
      {
        RG6L++;
        break;
      }
      //m_362A8:
      RG4L = signedlong(RG6W);
      RG1L = 263 * RG4L;
      RG5L += party.hero[RG4L].skill[RG3L / 4][RG3L % 4];
      RG2L++;
    }
  }
  //m_362DE:
  for (;;)
  {
    if (mkul(RG5L) < mkul(lcon(0x200)))
      return RG2L;
    RG5UL >>= bcon(0x1);
    RG2L++;
  }
  FOREND
}

static i32 DM2_14cd_1316(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG51l;
  t_tile* RG52tilep;
  i16 RG61w;
  i16 RG62w;
  i16 RG63w;
  i32 vl_00;
  i32 vl_04;
  c_o32 vo_08; // poke 16 peek 32 inc16
  i16 vw_0c;
  i32 vl_10; // inc32
  i16 vw_14;
  i8 vb_18; // and8
  FPTR_L_LL pf_00;
  i16 parw01;
  i16 parw02;
  bool flag;
  bool skip00312 = false;
  bool skip00310 = false;
  bool skip00309 = false;
  bool skip00308 = false;
  bool skip00307 = false;
  bool skip00306 = false;
  bool skip00305 = false;

  vb_18 = RG1Blo;
  vw_0c = RG4W;
  RG3P = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4Blo = vb_18 & lcon(0x40);
  RG4L = unsignedlong(RG4Blo);
  if (RG4L != 0)
  {
    if (RG2Blo == byte_at(RG1P, lcon(0x12)))
      return lcon(0x1);
    vb_18 &= lcon(0xffffffbf);
  }
  RG4Blo = vb_18 & lcon(0xffffff80);
  RG4L = unsignedlong(RG4Blo);
  vl_04 = RG4L;
  RG2Blo = vb_18 & lcon(0x7f);
  vb_18 = RG2Blo;
  RG4L = 0;
  vl_00 = RG4L;
  if (mkub(RG2Blo) > mkub(lcon(0x16)))
    jump L_fin2;
  RG4Blo = RG2Blo;
  switch (RG4L)
  {
    case 0:
      jump L_fin1;

    case 1:
    case 22:
      //m_16E38:
      RG1L = unsignedlong(s350.v1e0571);
      if (RG1W != ddat.v1e08d6)
        jump L_fin2;
      RG3L = unsignedlong(s350.v1e0562.getyA());
      RG2L = unsignedlong(s350.v1e0562.getxA());
      RG4L = signedlong(ddat.v1e08d4);
      RG1L = signedlong(DM2_CALC_VECTOR_DIR(ddat.v1e08d8, RG4W, RG2W, RG3W));
      RG4L = unsignedlong(RG1W);
      RG1L = signedlong(ddat.v1e08da);
      if (RG4L != RG1L)
        jump L_fin2;
      if (vb_18 == lcon(0x1))
        jump L_fin1;
      RG3L = unsignedlong(s350.v1e0562.getyA());
      RG2L = unsignedlong(s350.v1e0562.getxA());
      RG4L = signedlong(ddat.v1e08d4);
      RG1W = DM2_CALC_SQUARE_DISTANCE(ddat.v1e08d8, RG4W, RG2W, RG3W);
      RG4L = signedlong(RG1W);
      if (RG4L <= lcon(0x1))
        jump L_fin1;
      if (RG1W > vw_0c || ddat.v1e0976 != 0)
        jump L_fin2;
      pf_00 = DM2_1BA1B;
      RG3L = unsignedlong(s350.v1e0562.getyA());
      RG2L = unsignedlong(s350.v1e0562.getxA());
      RG4L = signedlong(ddat.v1e08d4);
      RG1L = DM2_19f0_0207(signedlong(ddat.v1e08d8), RG4L, RG2L, RG3L, pf_00);
      flag = RG1W != wcon(0x0);
      skip00305 = true;
      break;

    case 2:
      //m_16F30:
      RG1L = unsignedlong(s350.v1e0571);
      RG4W = ddat.v1e08d6;
      if (RG1W == RG4W)
      {
        RG1L ^= RG4L;
        RG1Blo = s350.v1e0562.getxA();
        RG2W = ddat.v1e08d8;
        if (RG1W == RG2W)
        {
          RG1L ^= RG2L;
          RG1Blo = s350.v1e0562.getyA();
          if (RG1W == ddat.v1e08d4)
          {
            RG1L = 1;
            vl_00 = RG1L;
            jump L_fin2;
          }
        }
      }
      skip00306 = true;
      break;

    case 3:
      skip00307 = true;
      break;

    case 4:
      //m_16F9D:
      RG1L = signedlong(s350.v1e058d);
      vl_00 = RG1L;
      jump L_fin2;

    case 5:
    case 13:
      //m_16FA7:
      RG4W = unsignedword(RG4Blo);
      RG4Blo = s350.v1e0571;
      RG1W = word_at(RG3P, lcon(0xc));
      RG2L = RG1L;
      RG2UW >>= bcon(0xa);
      if (RG4W != RG2W)
        jump L_fin2;
      RG4Bhi ^= RG2Bhi;
      RG4Blo = s350.v1e0562.getxA();
      RG2L = RG1L;
      RG2Bhi ^= RG1Bhi;
      RG2Blo &= lcon(0x1f);
      if (RG4W != RG2W)
        jump L_fin2;
      RG4Bhi ^= RG2Bhi;
      RG4Blo = s350.v1e0562.getyA();
      RG1L <<= bcon(0x6);
      _RG1.ushiftr11();
      if (RG4W != RG1W)
        jump L_fin2;
      if (vb_18 != lcon(0xd))
        jump L_fin1;
      skip00307 = true;
      break;

    case 6:
      //m_17069:
      RG4L = unsignedlong(word_at(RG3P, lcon(0xa)));
      RG3L = signedlong(vw_0c);
      RG1L = lcon(0x1) << RG3Blo;
      flag = (RG1L & RG4L) != 0;
      skip00305 = true;
      break;

    case 7:
      //m_17084:
      RG1L = unsignedlong(s350.v1e0571);
      flag = RG1W == ddat.v1e08d6;
      break;

    case 8:
      //m_170F8:
      RG61w = wcon(0x0);
      //m_17192:
      for (;;)
      {
        RG1L = signedlong(RG61w);
        if (RG1L >= lcon(0x4))
          jump L_fin2;
        //m_170FF:
        RG1W = DM2_GET_PLAYER_AT_POSITION(RG61w);
        vw_14 = RG1W;
        RG2L = signedlong(RG1W);
        if (RG2L != lcon(0xffffffff))
        {
          RG1L = 263 * RG2L;
          RG4W = party.hero[RG2L].item[1];
          RG1Blo = RG4W != wcon(0xffff) ? 1 : 0;
          RG1L = unsignedlong(RG1Blo);
          if (RG1L != 0)
          {
            RG1L = unsignedlong(RG4W);
            RG4L = lcon(0xb);
            RG1L = DM2_CREATURE_CAN_HANDLE_IT(RG1L, RG4L);
            if (RG1L != 0)
              jump L_fin1;
          }
          //m_17152:
          RG2L = signedlong(vw_14);
          RG4W = party.hero[RG2L].item[0];
          RG1Blo = RG4W != wcon(0xffff) ? 1 : 0;
          RG1L = unsignedlong(RG1Blo);
          if (RG1L != 0)
          {
            RG1L = unsignedlong(RG4W);
            RG4L = lcon(0xb);
            RG1L = DM2_CREATURE_CAN_HANDLE_IT(RG1L, RG4L);
            if (RG1L != 0)
              jump L_fin1;
          }
        }
        //m_17191:
        RG61w++;
      }
      FOREND

    case 9:
      //m_171A3:
      RG1L = DM2_2c1d_09d9();
      RG4L = unsignedlong(RG1W);
      RG1L = signedlong(vw_0c);
      if (RG4L >= RG1L)
        jump L_fin1;
      jump L_fin2;

    case 10:
      //m_171C1:
      RG1L = unsignedlong(s350.v1e0571);
      RG4W = ddat.v1e08d6;
      if (RG1W != RG4W)
        jump L_fin2;
      if (RG4W != RG4W)
        skip00310 = true;
      else
      {
        RG1L ^= RG4L;
        RG1Blo = s350.v1e0562.getxA();
        RG3W = ddat.v1e08d8;
        if (RG1W != RG3W)
          skip00310 = true;
        else
        {
          RG1L ^= RG3L;
          RG1Blo = s350.v1e0562.getyA();
          if (RG1W == ddat.v1e08d4)
          {
            //m_17207:
            RG1L = 0;
            skip00312 = true;
          }
          else
            skip00310 = true;
        }
      }
      break;

    case 11:
      //m_172D6:
      RG1W = word_at(RG3P, lcon(0xe));
      RG1L <<= bcon(0x6);
      RG1UW >>= 14;
      RG1L = unsignedlong(RG1W) + 2;
      RG1L &= 0x3;
      vw_14 = RG1W;
      RG1W = s350.v1e057a & lcon(0x20);
      RG1L = unsignedlong(RG1W);
      if (RG1L == 0)
        jump L_fin2;
      RG2L = signedlong(vw_14);
      RG4L = unsignedlong(s350.v1e0562.getyA());
      RG4W += table1d2804[RG2L];
      RG4L = signedlong(RG4W);
      RG3L = unsignedlong(s350.v1e0562.getxA());
      RG3W += table1d27fc[RG2L];
      RG1L = (dm2_GET_TILE_VALUE(signedlong(RG3W), RG4L) & lcon(0xff)) >> 5 & lcon(0xff);
      if (RG1L != lcon(0x4))
        jump L_fin2;
      put16(parw02, RG2W);
      put16(parw01, lcon(0xffffffff));
      RG1L = DM2_19f0_0d10(lcon(0x1), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), lcon(0xffffffff), parw01, parw02);
      flag = RG1L != 0;
      skip00305 = true;
      break;

    case 12:
      //m_17377:
      RG1L = unsignedlong(s350.v1e0571);
      if (RG1W != ddat.v1e08d6)
        jump L_fin2;
      RG1W = word_at(RG3P, lcon(0xe));
      RG1L <<= bcon(0x6);
      RG1UW >>= 14;
      RG1L = unsignedlong(RG1W);
      RG4W = unsignedword(RG4Blo);
      RG4Blo = s350.v1e0562.getyA();
      RG4W += table1d2804[RG1L];
      RG4L = unsignedlong(RG4W);
      RG2W = unsignedword(RG2Blo);
      RG2Blo = s350.v1e0562.getxA();
      RG2W += table1d27fc[RG1L];
      RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG2W), RG4L);
      if (RG1W == lcon(0xffffffff))
        jump L_fin2;
      RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW))), lcon(0x4)))));
      if (jz_test8(location(RG1P), lcon(0x1)))
        jump L_fin2;
      RG62w = wcon(0x0);
      //m_174BA:
      for (;;)
      {
        RG1L = signedlong(RG62w);
        if (RG1L >= lcon(0x4))
          jump L_fin2;
        //m_173FC:
        RG4W = ddat.v1e08d8;
        RG2W = table1d27fc[RG1L];
        RG4L += RG2L;
        vo_08.poke16(RG4W);
        RG3W = ddat.v1e08d4 + table1d2804[RG1L];
        RG2L = signedlong(RG3W);
        RG51l = signedlong(vo_08.peek16());
        RG1L = dm2_GET_TILE_VALUE(RG51l, RG2L);
        RG4L = unsignedlong(RG1Blo);
        vw_14 = RG4W;
        RG1L = unsignedlong(RG1Blo) >> 5;
        if (RG1Blo == 0 && (CUTX8(vw_14) & bcon(0x10)) != 0)
        {
          RG4L = RG2L;
          RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(CUTX16(RG51l), RG4W));
          //m_1745D:
          for (;;)
          {
            RG4L = RG1L;
            if (RG4W != lcon(0xfffffffe))
            {
              RG1L = RG4L;
              RG1Blo ^= RG4Blo;
              RG1Bhi &= lcon(0x3c);
              RG1L = unsignedlong(RG1W) >> bcon(0xa);
              vw_14 = RG1W;
              RG1L = signedlong(vw_14);
              if (RG1L == lcon(0x3))
              {
                RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
                RG1W = word_at(RG1P, 2) & lcon(0x7f);
                if (RG1W == lcon(0x3f))
                  jump L_fin1;
              }
              //m_174A1:
              RG1L = signedlong(vw_14);
              if (RG1L <= lcon(0x3))
              {
                RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
                continue;
              }
            }
            break;
          }
        }
        //m_174B9:
        RG62w++;
      }
      FOREND

    case 14:
      //m_174CB:
      RG2L = unsignedlong(word_at(RG3P, lcon(0x6)));
      RG1L = 52 * RG2L / unsignedlong(word_at(s350.v1e0552, lcon(0x4)));
      RG4L = signedlong(vw_0c);
      if (mkul(RG1L) <= mkul(RG4L))
        jump L_fin1;
      jump L_fin2;

    case 15:
      //m_17504:
      vl_10 = 0;
      RG63w = 0;
      RG52tilep = *mapdat.map;
      RG1L = 2 * unsignedlong(*ddat.v1e03f4);
      RG2RP = dm2_v1e038c + RG1L / 2;
      RG1L = 0;
      vo_08.poke16(RG1W);
      //m_175B1:
      for (;;)
      {
        RG1W = vo_08.peek16();
        if (RG1W >= mapdat.map_width)
        {
          RG1L = vl_10 + 1;
          RG4L = signedlong(RG1W);
          RG1L = signedlong(DM2_MIN(4, RG4W));
          if (RG63w < RG1W)
            jump L_fin1;
          jump L_fin2;
        }
        //m_17537:
        RG3L = 0;
        //m_175A3:
        for (;;)
        {
          if (RG3W >= mapdat.map_height)
          {
            vo_08.inc16();
            break;
          }
          //m_1753E:
          RG1P = DOWNCAST(t_tile, RG52tilep++);
          if (jnz_test8(location(RG1P), lcon(0x10)))
          {
            RG4W = word_at(RG2P);
            RG2P += 2;
            //m_1754C:
            for (;;)
            {
              RG1L = RG4L;
              RG1Blo ^= RG4Blo;
              RG1Bhi &= lcon(0x3c);
              RG1L = unsignedlong(RG1W) >> bcon(0xa);
              if (RG1L != lcon(0x4))
              {
                //m_17590:
                RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
                RG4L = RG1L;
                if (RG1W != lcon(0xfffffffe))
                  continue;
              }
              else
              {
                RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
                RG1Blo = byte_at(RG1P, lcon(0x4));
                RG1W = unsignedword(RG1Blo);
                vw_14 = RG1W;
                if (RG1W != vw_0c)
                {
                  //m_1757E:
                  if (RG1W == lcon(0x34) || RG1W == lcon(0x31))
                    vl_10++;
                }
                else
                  RG63w++;
              }
              break;
            }
          }
          //m_175A2:
          RG3L++;
        }
      }
      FOREND

    case 16:
      //m_16FFF:
      RG4W = unsignedword(RG4Blo);
      RG4Blo = s350.v1e0571;
      RG1W = word_at(RG3P, lcon(0xc));
      RG1UW >>= bcon(0xa);
      skip00308 = true;
      break;

    case 17:
      //m_175E2:
      RG4L = unsignedlong(s350.v1e0562.getyA());
      RG1L = unsignedlong(s350.v1e0562.getxA());
      DM2_19f0_045a(RG1L, RG4L);
      if ((ddat.v1e08ae & wcon(0x10)) == 0)
        jump L_fin2;
      RG4L = signedlong(word_at(RG3P, lcon(0x8)));
      RG1L = DM2_1c9a_1b16(signedlong(vw_0c), RG4L);
      flag = RG1W == wcon(0x0);
      break;

    case 18:
      skip00309 = true;
      break;

    case 19:
      //m_17094:
      RG1L = unsignedlong(word_at(RG3P, lcon(0xa)));
      RG3L = signedlong(vw_0c);
      RG4L = lcon(0x1) << RG3Blo;
      if ((RG4L & RG1L) != 0)
        jump L_fin2;
      RG1L = unsignedlong(s350.v1e0571);
      flag = RG1W != ddat.v1e0266;
      skip00305 = true;
      break;

    case 20:
      //m_170C3:
      RG4L = unsignedlong(word_at(RG3P, lcon(0xa)));
      RG3L = signedlong(vw_0c);
      RG1L = lcon(0x1) << RG3Blo;
      if ((RG1L & RG4L) == 0)
        jump L_fin2;
      skip00309 = true;
      break;

    case 21:
      //m_17017:
      RG4L = 2 * signedlong(vw_0c);
      RG1P += RG4L;
      RG2W = word_at(RG1P, lcon(0xe)) & lcon(0x1f);
      RG4W = unsignedword(RG4Blo);
      RG4Blo = s350.v1e0562.getxA();
      if (RG2W != RG4W)
        jump L_fin2;
      RG2W = word_at(RG1P, lcon(0xe));
      RG2L <<= bcon(0x6);
      _RG2.ushiftr11();
      RG4Blo = s350.v1e0562.getyA();
      if (RG2W != RG4W)
        jump L_fin2;
      RG1W = word_at(RG1P, lcon(0xe));
      RG4L = RG1L;
      RG4UW >>= bcon(0xa);
      RG1W = unsignedword(RG1Blo);
      RG1Blo = s350.v1e0571;
      skip00308 = true;
      break;

    default: throw(THROW_DMABORT);
  }

  if (skip00310)
  {
    //m_17200:
    RG1L = 1;
    skip00312 = true;
  }

  if (skip00309)
  {
    //m_170DF:
    RG1L = unsignedlong(s350.v1e0571);
    flag = RG1W == ddat.v1e0266;
  }

  if (skip00308)
    //m_1700F:
    flag = RG4W == RG1W;

  if (skip00307)
  {
    //m_16F74:
    RG4L = unsignedlong(word_at(RG3P, 2));
    RG1W = DM2_CREATURE_CAN_HANDLE_ITEM_IN(vw_0c, RG4L, lcon(0xff));
    if (RG1W == lcon(0xfffffffe))
      jump L_fin2;
    jump L_fin1;
  }

  if (skip00306)
  {
    //m_16F6D:
    RG1L = 0;
    vl_00 = RG1L;
    jump L_fin2;
  }

  if (skip00305)
  {
    //m_16F0B:
    if (flag)
      jump L_fin1;
    jump L_fin2;
  }

  if (skip00312)
  {
    //m_17209:
    if (RG1L == 0)
      jump L_fin2;
    RG1L = unsignedlong(s350.v1e0562.getxA());
    RG1W -= ddat.v1e08d8;
    RG1L = signedlong(DM2_ABS(RG1W));
    RG4L = signedlong(RG1W);
    RG1L = signedlong(CUTX16(signedlong(DM2_ABS(unsignedword(s350.v1e0562.getyA()) - ddat.v1e08d4)))) + RG4L;
    if (RG1L > lcon(0x1))
      jump L_fin2;
    RG3L = signedlong(ddat.v1e08d4);
    RG2L = signedlong(ddat.v1e08d8);
    RG4L = unsignedlong(s350.v1e0562.getyA());
    RG1L = signedlong(DM2_CALC_VECTOR_DIR(unsignedword(s350.v1e0562.getxA()), RG4W, RG2W, RG3W));
    RG4W = ddat.v1e08d4 + table1d2804[RG1L];
    RG2L = signedlong(RG4W);
    RG4W = ddat.v1e08d8 + table1d27fc[RG1L];
    RG1L = dm2_GET_TILE_VALUE(signedlong(RG4W), RG2L);
    RG4L = unsignedlong(RG1Blo);
    vw_14 = RG4W;
    RG1L = unsignedlong(RG1Blo) >> 5;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 2)
      jump L_fin2;
    RG1L = RG4L;
    RG1Bhi ^= RG4Bhi;
    RG1Blo &= lcon(0x8);
    RG1L = signedlong(RG1W);
    if (RG1L == 0)
      jump L_fin2;
    RG1L = RG4L;
    RG1Bhi ^= RG4Bhi;
    RG1Blo &= 0x1;
    RG1L = signedlong(RG1W);
    flag = RG1L == 0;
  }

  //m_170ED:
  if (!flag)
    jump L_fin2;

L_fin1:
  vl_00 = 1;

L_fin2:
  if (vl_04 == 0)
    return vl_00;
  RG1Blo = vl_00 == 0 ? 1 : 0;
  return RG1L & lcon(0xff);
}

// stacksize was 0x30
static void DM2_14cd_18f2(i32 eaxl, i32 edxl, unk* xebxp, i32 ecxl, i16 argw0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG51p;
  i8* RG61p;
  i8* RG7p;
  s_hexe s18_00;
  i32 vl_10;
  i8 vb_14;
  i8 vb_18; // neg8
  i8 vb_1c; // signed byte!
  i8 parb00;
  i8 parb02;
  i8 parb03;
  bool flag;

  vb_18 = RG1Blo;
  vb_14 = RG4Blo;
  RG7p = RG2P;
  vb_1c = RG3Blo;
  if (RG2P == NULL)
    return;
  RG1Blo = RG1Blo < 0 ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  flag = RG1L == 0;
  vl_10 = RG1L;
  if (!flag)
    vb_18 = -vb_18;
  //m_1767F:
  for (;;)
  {
    RG1Blo = byte_at(RG7p, lcon(0xc));
    if (RG1Blo == vb_18)
    {
      RG2L = signedlong(vb_14);
      RG4L = signedlong(word_at(RG7p, 2));
      RG1L = DM2_14cd_1316(unsignedlong(byte_at(RG7p, lcon(0x1))), RG4L, RG2L);
      if (RG1L != 0)
      {
        RG61p = ADRESSOF(s_hexe, &s18_00);
        RG51p = RG7p;
        DM2_COPY_MEMORY(RG61p, RG51p, 14);
        RG61p += 14;
        RG51p += 14;
        if (vl_10 != 0)
        {
          RG2Blo = 0;
          s18_00.b_08 = RG2Blo;
          s18_00.b_09 = RG2Blo;
        }
        RG3L = signedlong(vb_18);
        put8(parb03, RG3Blo);
        RG1L = signedlong(vb_14);
        put8(parb02, RG1Blo);
        i32 parl01 = unsignedlong(argw0);
        RG1L = signedlong(vb_1c);
        put8(parb00, RG1Blo);
        RG1L = signedlong(byte_at(RG7p));
        RG2P = ADRESSOF(s_hexe, &s18_00);
        RG4P = RG7p;
        DM2_ai_14cd_0f3c(RG1L, RG4P, UPCAST(s_hexe, RG2P), RG3L, parb00, parl01, parb02, parb03);
      }
    }
    RG3P = RG7p;
    RG7p += lcon(0xe);
    if (byte_at(RG3P + lcon(0xd)) == 0)
      return;
  }
  FOREND
}

// belongs to DM2_14cd_0f0a
void DM2_14cd_19a4(i32 eaxl, i32 edxl, unk* xebxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG4; RG4L = edxl;

  RG4L = signedlong(RG4Blo);
  RG1L = signedlong(RG1Blo);
  DM2_14cd_18f2(RG1L, RG4L, RG2P, 0, lcon(0xffff));
}

// stacklen was 0x10
static void DM2_14cd_19c2(i32 eaxl, unk* xebxp, i32 edxl, i32 ecxl, i8 argb0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8 vb_00;
  i8 vb_04;

  vb_04 = RG1Blo;
  vb_00 = RG4Blo;
  if (RG2P == NULL || s350.v1e058d == 0)
    return;
  RG4L = signedlong(argb0);
  RG1P = DM2_ai_14cd_10d2(RG2P, RG4L);
  if (byte_at(RG1P + lcon(0x5)) <= bcon(0x0))
    s350.v1e0580 &= lcon(0xfffffff7);
  if (s350.v1e0578 == 0)
    return;
  RG1L = 0;
  if (vb_04 != 0)
    RG3Blo = -RG3Blo;
  RG1L = signedlong(RG1Blo);
  RG4L = signedlong(vb_00);
  DM2_14cd_18f2(signedlong(RG3Blo), RG4L, RG2P, RG1L, lcon(0xffff));
}

// belongs to DM2_14cd_0f0a
static void DM2_14cd_1a3c(i32 eaxl, i32 edxl, unk* xebxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG4; RG4L = edxl;

  RG4L = signedlong(RG4Blo);
  RG1L = signedlong(RG1Blo);
  DM2_14cd_19c2(RG1L, RG2P, RG4L, 2, lcon(0x1));
}

// belongs to DM2_14cd_0f0a
static void DM2_14cd_1a5a(i32 eaxl, i32 edxl, unk* xebxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG4; RG4L = edxl;

  RG4L = signedlong(RG4Blo);
  RG1L = signedlong(RG1Blo);
  DM2_14cd_19c2(RG1L, RG2P, RG4L, lcon(0x4), lcon(0x3));
}

static void DM2_14cd_1a78(i32 eaxl, i32 edxl, unk* xebxp, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG51p;
  i8* RG61p;
  i8* RG7p;
  s_hexe s18_00;
  unk* xp_10;
  i8 vb_14;
  i8 vb_18;
  i8 vb_1c;
  i8 parb00;
  i8 parb02;
  i8 parb03;

  vb_18 = RG1Blo;
  vb_14 = RG4Blo;
  RG7p = RG2P;
  vb_1c = RG3Blo;
  if (RG2P == NULL)
    return;
  RG4L = signedlong(vb_1c);
  RG1P = DM2_ai_14cd_10d2(RG2P, RG4L);
  if (byte_at(RG1P + lcon(0x7)) == 0)
    return;
  RG1P += lcon(0x8);
  xp_10 = UPCAST(unk, RG1P);
  //m_177DE:
  for (;;)
  {
    RG1Blo = byte_at(RG7p, lcon(0xc));
    if (RG1Blo == vb_1c)
    {
      if (word_at(RG7p + lcon(0x4)) != lcon(0xffffffff))
      {
        RG2L = signedlong(vb_14);
        RG4L = signedlong(word_at(RG7p, 2));
        RG1L = DM2_14cd_1316(unsignedlong(byte_at(RG7p, lcon(0x1))), RG4L, RG2L);
        if (RG1L != 0)
        {
          RG4L = signedlong(byte_at(DOWNCAST(unk, xp_10)));
          RG1L = -signedlong(DM2_MIN(signedword(byte_at(RG7p, lcon(0x8))), RG4W));
          RG61p = ADRESSOF(s_hexe, &s18_00);
          RG51p = RG7p;
          DM2_COPY_MEMORY(RG61p, RG51p, 14);
          RG61p += 14;
          RG51p += 14;
          if (vb_18 != 0)
          {
            RG2W = unsignedword(RG2Blo);
            s18_00.b_08 = RG2Bhi;
            s18_00.b_09 = RG2Bhi;
            RG1L = 0;
          }
          RG4L = signedlong(vb_18);
          put8(parb03, RG4Blo);
          RG4L = signedlong(vb_14);
          put8(parb02, RG4Blo);
          RG1L = signedlong(RG1Blo);
          put8(parb00, RG1Blo);
          RG3L = signedlong(vb_1c);
          RG1L = signedlong(byte_at(RG7p));
          RG2P = ADRESSOF(s_hexe, &s18_00);
          RG4P = RG7p;
          DM2_ai_14cd_0f3c(RG1L, RG4P, UPCAST(s_hexe, RG2P), RG3L, parb00, lcon(0xffff), parb02, parb03);
        }
      }
      //m_17870:
      xp_10 += 4; // DANGER
    }
    //m_17875:
    RG1P = RG7p;
    RG7p += lcon(0xe);
    if (byte_at(RG1P + lcon(0xd)) == 0)
      return;
  }
  FOREND
}

// belongs to DM2_14cd_0f0a
static void DM2_14cd_1b74(i32 eaxl, i32 edxl, unk* xebxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG4; RG4L = edxl;

  RG4L = signedlong(RG4Blo);
  RG1L = signedlong(RG1Blo);
  DM2_14cd_1a78(RG1L, RG4L, RG2P, lcon(0x1));
}

// belongs to DM2_14cd_0f0a
static void DM2_14cd_1b90(i32 eaxl, i32 edxl, unk* xebxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG4; RG4L = edxl;

  RG4L = signedlong(RG4Blo);
  RG1L = signedlong(RG1Blo);
  DM2_14cd_1a78(RG1L, RG4L, RG2P, lcon(0x3));
}

// stacklen was 0xc
static void DM2_14cd_1bac(i32 eaxl, i32 edxl, unk* xebxp, i32 ecxl, i8 argb0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8 vb_00;
  i8 vb_04;

  vb_00 = RG1Blo;
  vb_04 = RG4Blo;
  if (RG2P == NULL || s350.v1e058d == 0)
    return;
  RG4L = signedlong(argb0);
  RG1P = DM2_ai_14cd_10d2(RG2P, RG4L);
  RG4W = s350.v1e0578 & lcon(0x8);
  RG4L = unsignedlong(RG4W);
  if (RG4L != 0)
  {
    RG1Blo = byte_at(RG1P + lcon(0x5)) <= bcon(0x0) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 0)
      s350.v1e0580 &= lcon(0xfffffff7);
  }
  if (s350.v1e0578 == 0)
    return;
  if (vb_00 != 0)
    RG3Blo = -RG3Blo;
  RG4L = signedlong(vb_04);
  RG1L = signedlong(RG3Blo);
  DM2_14cd_18f2(RG1L, RG4L, RG2P, 0, lcon(0xffff));
}

// belongs to DM2_14cd_0f0a
static void DM2_14cd_1c27(i32 eaxl, i32 edxl, unk* xebxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG4; RG4L = edxl;

  RG4L = signedlong(RG4Blo);
  RG1L = signedlong(RG1Blo);
  DM2_14cd_1bac(RG1L, RG4L, RG2P, 2, lcon(0x1));
}

// belongs to DM2_14cd_0f0a
static void DM2_14cd_1c45(i32 eaxl, i32 edxl, unk* xebxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG4; RG4L = edxl;

  RG4L = signedlong(RG4Blo);
  RG1L = signedlong(RG1Blo);
  DM2_14cd_1bac(RG1L, RG4L, RG2P, lcon(0x4), lcon(0x3));
}

// belongs to DM2_14cd_0f0a
static void DM2_14cd_1c63(i32 edxl, unk* xebxp)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 parw00;

  if (s350.v1e07d8.b_03 != lcon(0xd))
    //m_1795F:
    RG1L = lcon(0xffff);
  else
    RG1W = s350.v1e07d8.w_08;
  //m_17964:
  RG1L = unsignedlong(RG1W);
  put16(parw00, RG1W);
  RG4L = signedlong(RG4Blo);
  DM2_14cd_18f2(lcon(0x5), RG4L, RG2P, 0, parw00);
}

// belongs to DM2_14cd_0f0a
static void DM2_14cd_1c8d(i32 eaxl, i32 edxl, unk* xebxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG51w;
  i16 RG52w;

  if (RG1Blo != 0)
  {
    RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
    RG3W = word_at(RG1P, lcon(0xc)) & lcon(0x1f);
    RG51w = unsignedword(s350.v1e0562.getxA());
    if (RG51w == RG3W)
    {
      RG3W = word_at(RG1P, lcon(0xc));
      RG3L <<= bcon(0x6);
      _RG3.ushiftr11();
      RG52w = unsignedword(s350.v1e0562.getyA());
      if (RG52w == RG3W)
      {
        RG1W = word_at(RG1P, lcon(0xc));
        RG1UW >>= bcon(0xa);
        RG3W = unsignedword(RG3Blo);
        RG3Blo = s350.v1e0571;
        if (RG3W == RG1W)
          return;
      }
    }
  }
  RG4L = signedlong(RG4Blo);
  DM2_14cd_18f2(lcon(0x6), RG4L, RG2P, 0, lcon(0xffff));
}

// belongs to DM2_14cd_0f0a
static void DM2_14cd_1cec(i32 edxl, unk* xebxp)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 parw00;

  RG3Blo = RG4Blo;
  RG1L = unsignedlong(s350.v1e054c);
  RG4L = lcon(0xffff);
  RG1P = DM2_GET_MISSILE_REF_OF_MINION(RG1L, RG4L);
  if (RG1P != NULL)
  {
    RG4W = word_at(RG1P, 2) & lcon(0x3c00);
    RG4L = unsignedlong(RG4W) >> bcon(0xa);
    if (RG4L == lcon(0x9))
    {
      RG1L = unsignedlong(word_at(DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(word_at(RG1P, 2))))), lcon(0x6)));
      put16(parw00, RG1W);
      RG4L = signedlong(RG3Blo);
      DM2_14cd_18f2(lcon(0x7), RG4L, RG2P, 0, parw00);
    }
  }
}

// belongs to DM2_14cd_0f0a
static void DM2_14cd_1d42(i32 edxl, unk* xebxp)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG4; RG4L = edxl;
  i16 parw00;

  if (s350.v1e07d8.b_03 != lcon(0x5))
    //m_17A49:
    RG1L = lcon(0xffff);
  else
    RG1W = s350.v1e07d8.w_08;
  //m_17A4E:
  RG1L = unsignedlong(RG1W);
  put16(parw00, RG1W);
  RG4L = signedlong(RG4Blo);
  DM2_14cd_18f2(lcon(0x12), RG4L, RG2P, 0, parw00);
}

static void DM2_14cd_1d6c(i32 eaxl, i32 edxl, unk* xebxp, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG51p;
  i8* RG61p;
  i8* RG7p;
  s_hexe s18_00;
  i8 vb_10;
  i8 vb_14;
  i8 vb_18;
  i8 parb02;
  i8 parb03;

  vb_18 = RG1Blo;
  vb_10 = RG4Blo;
  RG7p = RG2P;
  vb_14 = RG3Blo;
  if (RG2P == NULL)
    return;
  //m_17A7C:
  for (;;)
  {
    RG2W = word_at(RG7p, lcon(0x4));
    RG1W = word_at(RG7p, lcon(0x6));
    RG4Blo = byte_at(RG7p, lcon(0xc));
    if (RG4Blo == vb_14)
    {
      bool skip00315 = false;
      if (RG2W == lcon(0xffffffff))
        skip00315 = true;
      else
      {
        if (RG1W != 0 && RG1W != lcon(0x1))
          skip00315 = true;
        else
        {
          RG4P = DOWNCAST(SPX_Creature, s350.v1e054e);
          RG4W = word_at(RG4P, 2);
          RG4L = unsignedlong(RG4W);
          RG1L = signedlong(RG2W);
          RG2L = lcon(0xff);
          RG1W = DM2_CREATURE_CAN_HANDLE_ITEM_IN(RG1W, RG4L, RG2L);
          if (RG1W != lcon(0xfffffffe))
            skip00315 = true;
        }
      }

      if (skip00315)
      {
        //m_17AC5:
        RG2L = signedlong(vb_10);
        RG4L = signedlong(word_at(RG7p, 2));
        RG1L = DM2_14cd_1316(unsignedlong(byte_at(RG7p, lcon(0x1))), RG4L, RG2L);
        if (RG1L != 0)
        {
          RG61p = ADRESSOF(s_hexe, &s18_00);
          RG51p = RG7p;
          DM2_COPY_MEMORY(RG61p, RG51p, 14);
          RG61p += 14;
          RG51p += 14;
          if (vb_18 != 0)
          {
            RG2W = unsignedword(RG2Blo);
            s18_00.b_08 = RG2Bhi;
            s18_00.b_09 = RG2Bhi;
          }
          RG1L = signedlong(vb_18);
          put8(parb03, RG1Blo);
          RG1L = signedlong(vb_10);
          put8(parb02, RG1Blo);
          RG3L = signedlong(vb_14);
          RG1L = signedlong(byte_at(RG7p));
          RG2P = ADRESSOF(s_hexe, &s18_00);
          RG4P = RG7p;
          DM2_ai_14cd_0f3c(RG1L, RG4P, UPCAST(s_hexe, RG2P), RG3L, 0, lcon(0xffff), parb02, parb03);
        }
      }
    }
    //m_17B27:
    RG1P = RG7p;
    RG7p += lcon(0xe);
    if (byte_at(RG1P + lcon(0xd)) == 0)
      return;
  }
  FOREND
}

// belongs to DM2_14cd_0f0a
static void DM2_14cd_1e36(i32 eaxl, i32 edxl, unk* xebxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG4; RG4L = edxl;

  RG4L = signedlong(RG4Blo);
  RG1L = signedlong(RG1Blo);
  DM2_14cd_1d6c(RG1L, RG4L, RG2P, lcon(0xf));
}

// belongs to DM2_14cd_0f0a
static void DM2_14cd_1e52(i32 eaxl, i32 edxl, unk* xebxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG4; RG4L = edxl;

  RG4L = signedlong(RG4Blo);
  RG1L = signedlong(RG1Blo);
  DM2_14cd_1d6c(RG1L, RG4L, RG2P, lcon(0x10));
}

// belongs to DM2_14cd_1e6e
static i32 DM2_3DC4C(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG1L = signedlong(RG1W) << 4;
  RG1W = word_at(ddat.v1e03c8, RG1L + lcon(0xe));
  RG1L <<= bcon(0x8);
  RG1UW >>= bcon(0xc);
  RG4L = unsignedlong(RG1Blo);
  RG3L = lcon(0x65);
  RG2L = lcon(0xb);
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0x8), RG4Blo, RG2Blo, RG3Blo);
  RG1W &= lcon(0x20);
  RG1L = unsignedlong(RG1W);
  RG1Blo = RG1L == 0 ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  return RG1L;
}

// belongs to DM2_14cd_0f0a
static void DM2_14cd_1e6e(i32 eaxl, i32 edxl, unk* xebxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i8 parb02;
  i8 parb03;

  RG3Blo = RG1Blo;
  RG3Bhi = RG4Blo;
  RG4P = RG2P;
  RG1L = DM2_3DC4C(unsignedlong(s350.v1e0571));
  if (RG1L != 0)
  {
    bool skip00318 = false;
    if (RG3Blo != 0)
    {
      //m_17BB3:
      RG1L = DM2_RAND();
      RG1W &= lcon(0x1f);
      RG1L = unsignedlong(RG1W);
      if (RG1L == 0)
        and8(location(DOWNCAST(SPX_Creature, s350.v1e054e) + lcon(0xa)), lcon(0x7f));
      skip00318 = true;
    }
    else
    {
      RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
      RG1W = word_at(RG1P, lcon(0xa)) & lcon(0x80);
      RG1L = unsignedlong(RG1W);
      if (RG1L == 0)
      {
        //m_17B98:
        RG1L = DM2_RAND();
        RG1W &= lcon(0x3f);
        RG1L = unsignedlong(RG1W);
        if (RG1L != 0)
          return;
        or8(location(DOWNCAST(SPX_Creature, s350.v1e054e) + lcon(0xa)), lcon(0xffffff80));
        skip00318 = true;
      }
    }

    if (skip00318)
    {
      //m_17BCC:
      RG1L = signedlong(RG3Blo);
      put8(parb03, RG1Blo);
      RG1L = signedlong(RG3Bhi);
      put8(parb02, RG1W);
      DM2_ai_14cd_0f3c(0, RG4P, CHGCAST(s_hexe, getadr_r1d41d6()), 0, 0, lcon(0xffff), parb02, parb03);
      return;
    }
  }
  //m_17B8D:
  and8(location(DOWNCAST(SPX_Creature, s350.v1e054e) + lcon(0xa)), lcon(0x7f));
}

// belongs to DM2_14cd_1f8b
static void DM2_14cd_1eec(i32 eaxl, i32 edxl, unk* xebxp, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG51p;
  i8* RG61p;
  i8* RG7p;
  s_hexe s18_00;
  i8 vb_10;
  i8 vb_14;
  i8 vb_18;
  i8 parb02;
  i8 parb03;

  vb_18 = RG1Blo;
  vb_14 = RG4Blo;
  RG7p = RG2P;
  vb_10 = RG3Blo;
  if (RG2P == NULL)
    return;
  //m_17C07:
  for (;;)
  {
    RG1Blo = byte_at(RG7p, lcon(0xc));
    if (RG1Blo == vb_10)
    {
      RG2L = signedlong(vb_14);
      RG1L = DM2_14cd_1316(unsignedlong(byte_at(RG7p, lcon(0x1))), signedlong(word_at(RG7p, 2)), RG2L);
      if (RG1L != 0)
      {
        RG61p = ADRESSOF(s_hexe, &s18_00);
        RG51p = RG7p;
        DM2_COPY_MEMORY(RG61p, RG51p, 14);
        RG61p += 14;
        RG51p += 14;
        RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
        RG1W = word_at(RG1P, lcon(0x8));
        s18_00.w_06 = RG1W;
        if (vb_18 != 0)
        {
          RG4W = unsignedword(RG4Blo);
          s18_00.b_08 = RG4Bhi;
          s18_00.b_09 = RG4Bhi;
        }
        RG1L = signedlong(vb_18);
        put8(parb03, RG1Blo);
        RG1L = signedlong(vb_14);
        put8(parb02, RG1Blo);
        RG3L = signedlong(vb_10);
        RG1L = signedlong(byte_at(RG7p));
        RG2P = ADRESSOF(s_hexe, &s18_00);
        DM2_ai_14cd_0f3c(RG1L, RG7p, UPCAST(s_hexe, RG2P), RG3L, 0, lcon(0xffff), parb02, parb03);
      }
    }
    RG4P = RG7p;
    RG7p += lcon(0xe);
    if (byte_at(RG4P + lcon(0xd)) == 0)
      return;
  }
  FOREND
}

// belongs to DM2_14cd_0f0a
static void DM2_14cd_1f8b(i32 eaxl, i32 edxl, unk* xebxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG4; RG4L = edxl;

  RG4L = signedlong(RG4Blo);
  RG1L = signedlong(RG1Blo);
  DM2_14cd_1eec(RG1L, RG4L, RG2P, lcon(0x15));
}

// belongs to DM2_14cd_0f0a
static void DM2_14cd_1fa7(i32 edxl, unk* xebxp)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 parw00;

  RG1W = ddat.v1e08d8 & lcon(0x1f);
  RG3Blo &= lcon(0xffffffe0);
  RG3L |= RG1L;
  RG1W = ddat.v1e08d4 & lcon(0x1f);
  RG3L &= lcon(0xfffffc1f);
  RG1L <<= bcon(0x5);
  RG3L |= RG1L;
  RG1W = ddat.v1e08d6 & lcon(0x3f);
  RG3Bhi &= 0x3;
  RG1L <<= bcon(0xa);
  RG3L |= RG1L;
  RG1L = unsignedlong(RG3W);
  put16(parw00, RG1W);
  RG4L = signedlong(RG4Blo);
  DM2_14cd_18f2(lcon(0x16), RG4L, RG2P, 0, parw00);
}

static void DM2_14cd_0f0a(i32 eaxl, i32 edxl, i32 ebxl, unk* xecxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3P = xecxp;
  c_nreg _RG4; RG4L = edxl;

  RG1Bhi = RG4Blo;
  RG4Blo = RG2Blo;
  RG2P = RG3P;
  RG1Blo &= lcon(0x1f);
  s350.v1e0580 = lcon(0xffffffff);
  RG3L = unsignedlong(RG1Blo);
  RG4L = signedlong(RG4Blo);
  RG1L = signedlong(RG1Bhi);
  switch (RG3L)
  {
    case 0:
      DM2_14cd_18cc(RG1L, RG4L, RG2P);
      break;

    case 1:
      DM2_14cd_19a4(RG1L, RG4L, RG2P);
      break;

    case 2:
      DM2_14cd_1a3c(RG1L, RG4L, RG2P);
      break;

    case 3:
      DM2_14cd_1a5a(RG1L, RG4L, RG2P);
      break;

    case 4:
      DM2_14cd_1b74(RG1L, RG4L, RG2P);
      break;

    case 5:
      DM2_14cd_1b90(RG1L, RG4L, RG2P);
      break;

    case 6:
      DM2_14cd_1c27(RG1L, RG4L, RG2P);
      break;

    case 7:
      DM2_14cd_1c45(RG1L, RG4L, RG2P);
      break;

    case 8:
      DM2_14cd_1c63(RG4L, RG2P);
      break;

    case 9:
      DM2_14cd_1c8d(RG1L, RG4L, RG2P);
      break;

    case 10:
      DM2_14cd_1cec(RG4L, RG2P);
      break;

    case 11:
      DM2_14cd_1d42(RG4L, RG2P);
      break;

    case 12:
      DM2_14cd_1e36(RG1L, RG4L, RG2P);
      break;

    case 13:
      DM2_14cd_1e52(RG1L, RG4L, RG2P);
      break;

    case 14:
      DM2_14cd_1e6e(RG1L, RG4L, RG2P);
      break;

    case 15:
      DM2_14cd_1f8b(RG1L, RG4L, RG2P);
      break;

    case 16:
      DM2_14cd_1fa7(RG4L, RG2P);
      break;

    default: throw(THROW_DMABORT);
  }
}

// belongs to DM2_14cd_0684
static i8 DM2_14cd_0389(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG5p;
  i8* RG61p;
  i8 vb_00;

  RG5p = DOWNCAST(c_creature, s350.creatures);
  RG1L = 0;
  if (s350.v1e07d8.b_00 != 0 && s350.v1e07d8.b_01 != 0 && s350.v1e07d8.b_03 != lcon(0xffffffff))
  {
    RG2W = unsignedword(RG2Blo);
    s350.v1e0674 = RG2Bhi;
    RG3Blo = byte_at(RG5p, lcon(0x12));
    if (RG3Blo != lcon(0xffffffff))
    {
      vb_00 = RG3Blo;
      RG4Blo = byte_at(RG5p, lcon(0x13));
      RG2L = signedlong(vb_00);
      RG61p = DOWNCAST(s_seven, table1d5f82[RG2L]);
      RG3P = DOWNCAST(s_hexe, s350.v1e07d8.xp_0a);
      RG4L = signedlong(RG4Blo);
      RG1L = 7 * RG4L;
      RG4L = signedlong(byte_at(RG61p, RG1L + lcon(0x6)));
      RG1Blo = byte_at(RG61p, RG1L + lcon(0x5));
      RG1L = unsignedlong(RG1Blo);
      DM2_14cd_0f0a(RG1L, RG4L, RG2L, RG3P);
    }
    if (s350.v1e0674 != 0)
    {
      //m_15E83:
      RG1L = DM2_1c9a_38a8();
      if (RG1L != 0)
        mov16(location(RG5p + lcon(0x18)), s350.v1e0678[0].s23_00.w_02);
    }
    else
      RG1L = 0;
  }
  //m_15E97:
  if (RG1L != 0)
    return vb_00;
  return 0xFF;
}

// belongs to DM2_14cd_0684
static i32 DM2_14cd_0457(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG5p;
  c_wlreg _RG6;
  c_o32 vo_00; // poke 16 peek 8 + 16 + 32 sub16 dec16

  RG1W = signedword(s350.v1e0674);
  vo_00.poke16(RG1W);
  if (RG1W == 0)
    return RG1L;
  RG5p = DOWNCAST(c_sizex16, s350.v1e0678);
  RG6W = unsignedword(s350.v1e07d8.b_00);
  //m_15EF7:
  for (;;)
  {
    if (vo_00.peek16() <= wcon(0x0))
    {
      RG1W = signedword(s350.v1e0674);
      vo_00.poke16(RG1W);
      RG5p = DOWNCAST(c_sizex16, s350.v1e0678);
      RG6L = 0;
      break;
    }
    //m_15ED2:
    RG3W = signedword(byte_at(RG5p));
    if (RG3W > 0)
    {
      RG1L = RG6L - 2;
      RG4L = signedlong(RG1W);
      RG1L = signedlong(DM2_MIN(RG3W / 2, RG4W));
      mov8(location(RG5p), RG1Blo);
    }
    //m_15EF0:
    vo_00.dec16();
    RG5p += lcon(0x16);
  }
  //m_15F6A:
  for (;;)
  {
    if (RG6W >= vo_00.peek16())
    {
      RG4L = signedlong(vo_00.peek16()) - lcon(0x1);
      RG1L = 11 * RG4L;
      if (s350.v1e0678[RG4L].s23_00.b_00 < bcon(0x0))
        vo_00.dec16();
      RG1Blo = vo_00.peek8();
      s350.v1e0674 = RG1Blo;
      return RG1L;
    }
    //m_15F13:
    if (byte_at(RG5p) < bcon(0x0))
    {
      RG3L = RG6L + 1;
      RG1P = RG5p + lcon(0x16);
      //m_15F29:
      for (;;)
      {
        if (RG3W < vo_00.peek16())
        {
          //m_15F20:
          if (byte_at(RG1P) < bcon(0x0))
          {
            RG3L++;
            RG1P += lcon(0x16);
            continue;
          }
        }
        break;
      }
      //m_15F2F:
      if (RG3W < vo_00.peek16())
      {
        //m_15F3E:
        RG2L = signedlong(vo_00.peek16());
        RG4L = signedlong(RG3W);
        RG2L -= RG4L;
        RG4L = 11 * RG2L;
        RG2L = 2 * RG4L;
        RG4P = RG5p;
        DM2_COPY_MEMORY(RG4P, RG1P, RG2L);
        RG3L -= RG6L;
        vo_00.sub16(RG3W);
      }
      else
      {
        RG1L = RG6L + 1;
        vo_00.poke16(RG1W);
      }
    }
    //m_15F66:
    RG6L++;
    RG5p += lcon(0x16);
  }
  FOREND
}

// belongs to DM2_14cd_0684
void DM2_14cd_0550(unk* xeaxp, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p;
  i32 RG6l;
  i8* RG7p;
  i32 vl_00;
  i8 vb_04;
  i8 vb_08;

  RG5p = RG1P;
  vb_04 = RG4Blo;
  vb_08 = RG2Blo;
  vl_00 = RG3L;
  RG6l = 0;
  //m_15FB6:
  for (;;)
  {
    bool skip00279 = false;
    RG2Blo = byte_at(RG5p);
    if (RG2Blo == vb_04)
    {
      //m_15FFD:
      if (vl_00 == 0 && s350.v1e07ec == 0)
        //m_16013:
        RG1L = 0;
      else
        RG1L = 1;
      //m_16015:
      RG6l = RG1L;
      RG4W = signedword(vb_08);
      skip00279 = true;
    }
    else
    {
      if (s350.v1e07ec == 0)
      {
        bool skip00278 = false;
        RG1W = signedword(byte_at(RG5p, lcon(0x1)));
        if (RG1W == 0)
          skip00278 = true;
        else
        {
          if (RG1W <= 0)
          {
            //m_15FE8:
            RG1W = DM2_RAND16(-RG1W);
            if (RG1W != 0)
              skip00278 = true;
          }
          else
          {
            RG1W = DM2_RAND16(RG1W);
            if (RG1W == 0)
              skip00278 = true;
          }
        }

        if (skip00278)
        {
          //m_15FF9:
          RG4L = 0;
          skip00279 = true;
        }
      }
    }

    if (skip00279)
    {
      //m_1601D:
      RG2L = signedlong(RG2Blo);
      RG7p = DOWNCAST(s_seven, table1d5f82[RG2L]);
      RG3P = pointer_at(RG5p + 2);
      RG4L = signedlong(RG4W);
      RG1L = 7 * RG4L;
      RG4L = signedlong(byte_at(RG7p, RG1L + lcon(0x6)));
      RG1Blo = byte_at(RG7p, RG1L + lcon(0x5));
      RG1L = unsignedlong(RG1Blo);
      DM2_14cd_0f0a(RG1L, RG4L, RG2L, RG3P);
    }

    //m_16049:
    if (RG6l != 0)
      return;
    RG1P = RG5p;
    RG5p += lcon(0x7);
    if (byte_at(RG1P, lcon(0x6)) == 0)
      return;
  }
  FOREND
}

// belongs to DM2_14cd_0684
static void DM2_14cd_0276(unk* xeaxp)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG5p;
  i16 vw_00;

  RG2P = RG1P;
  RG4L = signedlong(byte_at(RG1P, lcon(0x6)));
  RG1L = signedlong(DM2_MAX(0, RG4W));
  s350.v1e07d8.b_01 = RG1Blo;
  s350.v1e07d8.b_00 = RG1Blo;
  RG4W = word_at(RG2P, lcon(0x4));
  s350.v1e07d8.w_08 = RG4W;
  RG4Blo = byte_at(RG2P, lcon(0x7));
  s350.v1e07d8.b_03 = RG4Blo;
  RG4W = word_at(RG2P, lcon(0x8));
  s350.v1e07d8.w_04 = RG4W;
  RG4W = word_at(RG2P, lcon(0xa));
  s350.v1e07d8.w_06 = RG4W;
  RG4Blo = byte_at(RG2P, lcon(0x11));
  s350.v1e07d8.b_02 = RG4Blo;
  s350.v1e07d8.xp_0a = UPCAST(s_hexe, pointer_at(RG2P + lcon(0x12)));
  RG5p = NULL;
  if (RG1W > 0)
  {
    RG3L = 2 * signedlong(RG1W);
    RG1W = s350.v1e054c;
    RG1Bhi &= 0x3;
    RG1L = unsignedlong(RG1W) | ulcon(0x30000000);
    RG1L = dm2_dballochandler.DM2_ALLOCATION11(RG1L, lcon(0x1), OO /*TAG dbidx*/ vw_00);
    if (RG1L >= RG3L)
      //m_15DD0:
      RG5p = DOWNCAST(t_bmp, dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ vw_00));
    else
    {
      if (RG1L == 0)
        //m_15DAD:
        RG1P = DOWNCAST(s_xmalloc, dm2_dballochandler.DM2_ALLOC_CPXHEAP_MEM((t_dbidx)vw_00, RG3L));
      else
        RG1P = DOWNCAST(s_xmalloc, dm2_dballochandler.DM2_ALLOCATION9((t_dbidx)vw_00, RG3L));
      //m_15DBB:
      RG5p = RG1P;
      dm2_dballochandler.DM2_ALLOCATION1((t_dbidx)vw_00, true);
    }
    //m_15DDE:
    DM2_COPY_MEMORY(RG5p, DOWNCAST(c_button, s350.v1e060e), RG3L);
  }
  //m_15DEC:
  s350.v1e07e6 = UPCAST(i16, RG5p);
}

// belongs to DM2_14cd_09e2
static i32 DM2_14cd_0684(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG5p;
  i32 RG6l;
  i8 vb_00;
  i16 parw00;
  unk* xparp01;
  c_button* button;
  bool skip00283 = false;

  RG5p = DOWNCAST(c_creature, s350.creatures);
  RG1Blo = DM2_14cd_0389();
  RG1Blo = RG1Blo != bcon(0xff) ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  RG6l = RG1L;
  vb_00 = lcon(0xffffffff);
  RG4L = signedlong(s350.v1e0584);
  if ((table1d607e[RG4L].uc[0] & ubcon(0x1)) != 0)
    skip00283 = true;
  else
  {
    bool skip00282 = false;
    if (RG1L != 0)
    {
      RG1L = signedlong(DM2_RANDDIR());
      if (RG1W != 0)
      {
        //m_16104:
        RG1L = 0;
        skip00282 = true;
      }
    }

    if (!skip00282)
      //m_160FD:
      RG1L = 1;

    //m_16106:
    if (RG1L != 0)
      skip00283 = true;
  }

  if (skip00283)
  {
    //m_1610E:
    RG2W = unsignedword(RG2Blo);
    s350.v1e0674 = RG2Bhi;
    RG1L = DM2_14cd_062e();
    s350.v1e07ec = RG1Blo;
    RG1Bhi = byte_at(RG5p, lcon(0x12));
    RG1Blo = byte_at(RG5p, lcon(0x13));
    RG2L = signedlong(RG1Blo);
    RG4L = signedlong(RG1Bhi);
    RG1P = pointer_at(s350.v1e0588 + 2);
    DM2_14cd_0550(RG1P, RG4L, RG2L, RG6l);
    if (RG6l != 0)
      RG1L = DM2_14cd_0457();
    if (s350.v1e0674 != 0)
    {
      RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
      RG1W = word_at(RG1P, lcon(0xa));
      RG1L &= lcon(0x2048);
      RG1L = unsignedlong(RG1W);
      if (RG1L == 0)
        //m_1616D:
        RG1Blo = 0;
      else
        RG1Blo = s350.v1e07ed;
      //m_1616F:
      RG1Bhi = RG1Blo;
      RG4P = DOWNCAST(SPX_Creature, s350.v1e054e);
      RG4W = word_at(RG4P, lcon(0xa)) & lcon(0x1000);
      RG4L = unsignedlong(RG4W);
      if (RG4L != 0)
        RG1Bhi |= lcon(0x20);
      button = &s350.v1e060e[0];
      xparp01 = DOWNCAST(c_sizex16, s350.v1e0678);
      RG4L = signedlong(s350.v1e0674);
      put16(parw00, RG4W);
      RG2L = unsignedlong(RG1Bhi);
      RG4L = unsignedlong(s350.v1e0562.getyA());
      RG1L = unsignedlong(s350.v1e0562.getxA());
      RG3L = 0;
      RG1L = DM2_FIND_WALK_PATH(RG1L, RG4L, RG2L, parw00, XUPCAST(s_len8, xparp01), button);
      s350.v1e0675 = RG1Blo;
      RG4L = signedlong(RG1W);
      if (RG4L == lcon(0xffffffff))
      {
        //m_161EE:
        if (s350.v1e07ec != 0)
        {
          RG1Blo = lcon(0xfffffffd);
          return RG1L;
        }
      }
      else
      {
        RG1P = DOWNCAST(c_sizex16, &s350.v1e0678[RG4L]);
        RG4Blo = byte_at(RG1P, lcon(0xe));
        vb_00 = RG4Blo;
        RG4W = word_at(RG1P, 2);
        mov16(location(RG5p + lcon(0x18)), RG4W);
        DM2_14cd_0276(RG1P);
      }
    }
  }

  //m_161FE:
  if (vb_00 != lcon(0xffffffff) || RG6l == 0)
  {
    RG1Bhi = vb_00;
    if (RG1Bhi != lcon(0xffffffff))
    {
      //m_1622B:
      RG4Blo = byte_at(RG5p, lcon(0x12));
      if (RG4Blo != lcon(0xffffffff))
      {
        if (RG4Blo != RG1Bhi)
        {
          //m_16244:
          mov8(location(RG5p + lcon(0x12)), lcon(0xffffffff));
          mov8(location(RG5p + lcon(0x13)), 0);
        }
        else
          vb_00 = lcon(0xffffffff);
      }
    }
    else
    {
      vb_00 = lcon(0x11);
      s350.v1e07d8.b_01 = 0;
      s350.v1e07d8.b_03 = RG1Bhi;
    }
  }
  //m_15E9B:
  RG1Blo = vb_00;
  return RG1L;
}

// belongs to DM2_14cd_09e2
static i32 DM2_14cd_08f5(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG61p;
  i8 vb_00;

  RG4P = DOWNCAST(c_creature, s350.creatures);
  RG2L = 0;
  RG1Bhi = byte_at(RG4P, lcon(0x12));
  vb_00 = RG1Bhi;
  RG1Bhi = byte_at(RG4P, lcon(0x13));
  RG61p = DOWNCAST(s_seven, table1d5f82[signedlong(vb_00)]);
  if (RG1Blo != lcon(0xfffffffe))
  {
    //m_16335:
    RG3L = 7 * signedlong(RG1Bhi);
    RG1Blo = byte_at(RG61p, RG3L + 2);
  }
  else
  {
    RG3L = 7 * signedlong(RG1Bhi);
    RG1Blo = byte_at(RG61p, RG3L + lcon(0x1));
  }
  //m_16343:
  if (RG1Blo == lcon(0xfffffffe) || RG1Blo == lcon(0xfffffffd))
  {
    mov8(location(RG4P + lcon(0x12)), lcon(0xffffffff));
    mov8(location(RG4P + lcon(0x13)), 0);
    return lcon(0x1);
  }
  if (RG1Blo > bcon(0xfb) || RG1Blo < bcon(0xf8))
  {
    //m_1638D:
    RG1Bhi = RG1Bhi != RG1Blo ? 1 : 0;
    RG2L = unsignedlong(RG1Bhi);
    RG1Bhi = RG1Blo;
  }
  else
  {
    if (RG1Blo != lcon(0xfffffff9))
    {
      if (RG1Blo != lcon(0xfffffff8))
      {
        //m_1636F:
        if (RG1Blo != lcon(0xfffffffb))
          //m_1637A:
          RG2L = lcon(0xffffffff);
        else
          RG2L = 1;
        //m_1637F:
        RG1L = signedlong(RG1Bhi) + RG2L;
        RG1Bhi = RG1Blo;
      }
      else
        RG1Bhi += 2;
      //m_16386:
      RG2L = 1;
    }
  }
  //m_16398:
  RG1Blo = vb_00;
  mov8(location(RG4P + lcon(0x12)), RG1Blo);
  mov8(location(RG4P + lcon(0x13)), RG1Bhi);
  return RG2L;
}

// belongs to DM2_14cd_09e2
static i8 DM2_DECIDE_NEXT_XACT(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG61p;
  i8 vb_00;

  RG4P = DOWNCAST(c_creature, s350.creatures);
  RG1Blo = byte_at(RG4P, lcon(0x12));
  vb_00 = RG1Blo;
  RG4Bhi = byte_at(RG4P, lcon(0x13));
  RG61p = DOWNCAST(s_seven, table1d5f82[signedlong(vb_00)]);
  //m_16289:
  for (;;)
  {
    RG2L = signedlong(RG4Bhi);
    RG1P = RG61p + 7 * RG2L;
    RG4Blo = byte_at(RG1P);
    if (RG4Blo >= 0)
      break;
    if (RG4Blo == lcon(0xfffffff6))
    {
      RG4Blo = byte_at(RG1P, lcon(0x1));
      if (RG4Blo >= 0)
      {
        RG2L = signedlong(RG4Blo);
        if (RG2L < 2)
        {
          RG3W = signedword(byte_at(RG1P, 2));
          mov16(location(DOWNCAST(c_creature, s350.creatures) + 2 * RG2L + lcon(0xe)), RG3W);
        }
      }
    }
    RG4Bhi++;
  }
  //m_162C5:
  RG2P = DOWNCAST(c_creature, s350.creatures);
  RG4Blo = vb_00;
  mov8(location(RG2P + lcon(0x12)), RG4Blo);
  mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x13)), RG4Bhi);
  RG4Blo = byte_at(RG1P);
  RG2W = signedword(byte_at(RG1P, lcon(0x3)));
  s350.v1e0572 = RG2W;
  s350.v1e0574 = signedword(byte_at(RG1P, lcon(0x4)));
  return RG4Blo;
}

// belongs to DM2_SELECT_CREATURE_COUNT_AI_REFTAB
static i32 DM2_14cd_0067(unk* xeaxp)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG5p = xeaxp;
  i32 RG61l;
  c_wlreg _RG62;
  c_wlreg _RG63;
  i32 RG64l;
  i8* RG71p;
  c_lreg _RG72;
  c_lreg _RG73;
  unk* xp_00;
  i16 vw_04;
  i16 vw_08;

  xp_00 = DOWNCAST(c_creature, s350.creatures);
  RG71p = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG2W = word_at(RG71p, lcon(0xa));
  RG3L = DM2_RAND();
  RG1L = unsignedlong(s350.v1e0571);
  if (RG1W != ddat.v1e08d6)
  {
    //m_15AB6:
    RG1P = DOWNCAST(unk, xp_00);
    if (byte_at(RG1P + lcon(0x12)) == lcon(0xffffffff))
      //m_15AC6:
      RG1L = lcon(0x30);
    else
      RG1L = lcon(0x70);
    //m_15ACB:
    RG4L = unsignedlong(RG3W);
    if ((RG1L & RG4L) == 0)
    {
      RG2W |= lcon(0x8000);
      RG2Bhi &= lcon(0xffffffbf);
    }
  }
  else
    RG2Bhi &= lcon(0x7f);
  //m_15ADA:
  RG1L = RG2L;
  RG1W &= lcon(0x8000);
  RG1L = unsignedlong(RG1W);
  if (RG1L == 0)
  {
    RG1L = RG2L;
    RG1Bhi ^= RG2Bhi;
    RG1Blo &= lcon(0x8);
    RG1L = unsignedlong(RG1W);
    if (RG1L == 0)
      //m_15AFC:
      RG1L = lcon(0xf80);
    else
      RG1L = lcon(0x380);
    //m_15B01:
    RG4L = unsignedlong(RG3W);
    if ((RG1L & RG4L) == 0)
      RG2Bhi ^= lcon(0x40);
  }
  //m_15B0D:
  RG1P = s350.v1e0552;
  RG1W = word_at(RG1P, lcon(0x16));
  RG1L = unsignedlong(RG1W) >> 4;
  RG1L &= lcon(0xf);
  RG1L = unsignedlong(RG1W);
  RG61l = lcon(0x10) - RG1L;
  RG4L = unsignedlong(RG3W);
  RG1L = RG4L;
  RG4L = RG1L % RG61l;
  RG1L /= RG61l;
  if (RG4L == 0)
    RG2Blo &= lcon(0xffffffdf);
  RG1L = signedlong(s350.v1e0584);
  if ((table1d607e[RG1L].uc[1] & ubcon(0x4)) == 0)
  {
    //m_15B57:
    RG1L = RG3L;
    RG1Bhi ^= RG3Bhi;
    RG1Blo &= lcon(0x38);
    RG1L = unsignedlong(RG1W);
    if (RG1L == 0)
      RG2Bhi &= lcon(0xffffffdf);
  }
  else
    RG2Bhi |= lcon(0x20);
  //m_15B67:
  RG1L = RG3L;
  RG1Blo ^= RG3Blo;
  RG1Bhi &= lcon(0x30);
  RG1L = unsignedlong(RG1W);
  if (RG1L == 0)
    RG2Blo &= lcon(0xffffffef);
  RG1L = RG3L;
  RG1Bhi ^= RG3Bhi;
  RG1Blo &= 0x3;
  RG1L = unsignedlong(RG1W);
  if (RG1L == 0)
    RG2Blo &= lcon(0xffffffbf);
  RG3L &= lcon(0x8008);
  RG1L = unsignedlong(RG3W);
  if (RG1L == 0)
    RG2Bhi &= lcon(0xffffffef);
  RG62L = 2;
  RG1L = signedlong(s350.v1e0584);
  if ((table1d607e[RG1L].uc[0] & ubcon(0x2)) == 0)
  {
    RG1W = s350.v1e054c & lcon(0x3);
    RG1L++;
    RG1L = 2 * RG1L - 1;
    RG62L *= RG1L;
  }
  RG1W = DM2_RAND16(RG62W);
  if (RG1W == 0)
  {
    RG4L = unsignedlong(word_at(RG71p, lcon(0x6)));
    RG3L = 100 * RG4L;
    RG4L = signedlong(word_at(s350.v1e0552, lcon(0x4)));
    RG64l = signedlong(DM2_MAX(1, RG4W));
    RG1L = RG3L;
    RG4L = RG1L % RG64l;
    RG1L /= RG64l;
    if (mkul(RG1L) >= mkul(lcon(0x19)))
      //m_15C0E:
      RG2Blo &= lcon(0xfffffff7);
    else
      RG2Blo |= lcon(0x8);
  }
  //m_15C11:
  RG1L = RG2L;
  RG1Blo &= lcon(0xfffffffd);
  mov16(location(RG71p + lcon(0xa)), RG1W);
  RG3L = lcon(0xffffffff);
  vw_08 = RG3W;
  RG63L = RG3L;
  RG3L = 0;
  //m_15C9F:
  for (;;)
  {
    RG4W = word_at(RG5p);
    if (RG4W == 0)
      break;
    //m_15C2C:
    RG1L = RG4L;
    RG1Blo ^= RG4Blo;
    RG1Bhi &= lcon(0xffffffc0);
    vw_04 = RG1W; // truncated
    RG1L = unsignedlong(vw_04);
    if (RG1L != lcon(0xc000))
    {
      //m_15C61:
      RG1L = signedlong(vw_08);
      if (RG1L == lcon(0xffffffff))
      {
        RG1L = unsignedlong(RG4W);
        RG72L = unsignedlong(RG2W);
        if ((RG72L & RG1L) != 0)
          vw_08 = RG3W;
      }
      RG1L = signedlong(RG63W);
      if (RG1L == lcon(0xffffffff))
      {
        RG1L = unsignedlong(RG4W);
        RG73L = unsignedlong(RG2W) & mkul(RG1L);
        if (RG1L == RG73L)
          RG63L = RG3L;
      }
      if (RG4W == RG2W)
        break;
    }
    else
    {
      RG1L = RG4L;
      RG1Bhi &= lcon(0x3f);
      vw_04 = RG1W; // truncated
      RG1L = DM2_GET_GLOB_VAR(unsignedlong(vw_04));
      if (RG1W != 0)
        break;
    }
    //m_15C9B:
    RG3L++;
    RG5p += 6;
  }
  //m_15CA7:
  if (RG4W == 0)
  {
    bool skip00276 = false;
    RG1L = signedlong(RG63W);
    if (RG1L != lcon(0xffffffff))
      skip00276 = true;
    else
    {
      RG1L = signedlong(vw_08);
      if (RG1L != lcon(0xffffffff))
        skip00276 = true;
    }

    if (skip00276)
    {
      //m_15CC0:
      RG1L = signedlong(RG63W);
      if (RG1L != lcon(0xffffffff))
        //m_15CCE:
        RG3L = RG63L;
      else
        RG3L = unsignedlong(vw_08);
    }
  }
  //m_15CD0:
  RG1P = DOWNCAST(unk, xp_00);
  RG1W = signedword(byte_at(RG1P, lcon(0x16)));
  if (RG3W != RG1W)
  {
    s350.v1e07d8.b_01 = 0;
    RG1P = DOWNCAST(unk, xp_00);
    mov8(location(RG1P + lcon(0x12)), lcon(0xffffffff));
    mov8(location(RG1P + lcon(0x13)), 0);
    s350.v1e07d8.b_03 = lcon(0xffffffff);
  }
  RG1P = DOWNCAST(unk, xp_00);
  mov8(location(RG1P + lcon(0x16)), RG3Blo);
  RG1W = signedword(RG3Blo);
  return RG1L;
}

// belongs to DM2_14cd_09e2
static void DM2_SELECT_CREATURE_COUNT_AI_REFTAB(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;

  if (s350.v1e0584 == lcon(0xffffffff))
  {
    RG1L = DM2_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(byte_at(DOWNCAST(SPX_Creature, s350.v1e054e), lcon(0x4))), lcon(0x1));
    s350.v1e0584 = RG1W;
  }
  RG1L = DM2_14cd_0067(DOWNCAST(s_wpm, table1d6190[signedlong(s350.v1e0584)]));
  s350.v1e0586 = RG1W;
  RG2L = signedlong(s350.v1e0584);
  RG4L = 6 * signedlong(RG1W);
  RG1P = DOWNCAST(s_wpm, table1d6190[RG2L]) + RG4L;
  s350.v1e0588 = RG1P;
}

// belongs to DM2_13e4_0982
static void DM2_14cd_09e2(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG51w;
  i16 RG52w;
  i8* RG61p;
  c_5bytes c12_00;
  c_ql32 vql_08;
  unk* xp_0c;
  i32 vl_10;
  i32 vl_14;
  i8 vb_18;
  i8 vb_1c;
  i8 vb_20;
  i16 parw00;
  i16 parw01;
  i16 parw02;
  i16 parw03;
  i16 parw04;
  i16 parw05;
  bool skip00294 = false;
  bool skip00285 = false;

  RG61p = DOWNCAST(c_creature, s350.creatures);
  xp_0c = DOWNCAST(SPX_Creature, s350.v1e054e);
  DM2_SELECT_CREATURE_COUNT_AI_REFTAB();
  vql_08.poke32(table1d607e[signedlong(s350.v1e0584)].ul_00);
  RG1Bhi = vql_08.peek8();
  if ((RG1Bhi & bcon(0x40)) == 0)
  {
    //m_163EA:
    RG1Blo = ((RG1Bhi & bcon(0x20)) == 0 ? 1 : 0) + lcon(0x4);
    s350.v1e07ed = RG1Blo;
  }
  else
  {
    RG4W = unsignedword(RG4Blo);
    s350.v1e07ed = RG4Bhi;
  }
  //m_163F7:
  RG4P = pointer_at(s350.v1e0588 + 2);
  if (RG4P != DOWNCAST(void, getadr_u1d439d()))
  {
    //m_16410:
    if (RG4P != DOWNCAST(void, getadr_u1d4396()))
    {
      bool skip00288 = false;
      bool skip00287 = false;
      //m_16553:
      DM2_13e4_01a3();
      RG2L = lcon(0x400);
      RG4L = 0;
      RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x100));
      s350.v1e0898 = RG1P;
      RG1L = signedlong(DM2_RANDDIR());
      RG1Blo = RG1W == wcon(0x0) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      vl_10 = RG1L;
      if ((vql_08.peek8() & bcon(0x40)) != 0)
        jump L_fin;
      put16(parw01, lcon(0xffffffff));
      RG2L = unsignedlong(s350.v1e0562.getyA());
      put16(parw00, RG2W);
      RG4L = unsignedlong(s350.v1e0562.getxA());
      RG1L = unsignedlong(s350.v1e07ed);
      RG3L = RG4L;
      RG1L = DM2_CREATURE_GO_THERE(RG1L, RG4L, RG2L, RG3L, parw00, parw01);
      if (RG1L != 0)
        jump L_fin;
      if (s350.v1e07ed != lcon(0x5))
        skip00287 = true;
      else
      {
        RG4L = unsignedlong(s350.v1e0562.getyA());
        RG1Blo = s350.v1e0562.getxA();
        RG1L = DM2_19f0_13aa(RG1L, RG4L);
        if (RG1L == 0)
          skip00287 = true;
      }

      if (skip00287)
      {
        //m_165D3:
        if (s350.v1e058d == 0)
          skip00288 = true;
        else
        {
          RG1P = DOWNCAST(unk, xp_0c);
          RG1W = word_at(RG1P, lcon(0xe));
          RG1L <<= bcon(0x6);
          RG1UW >>= 14;
          RG1L = signedlong(RG1W);
          put16(parw03, RG1W);
          put16(parw02, lcon(0xffffffff));
          RG2L = unsignedlong(s350.v1e0562.getyA());
          RG4L = unsignedlong(s350.v1e0562.getxA());
          RG3L = lcon(0xffffffff);
          RG1L = DM2_19f0_0891(0, RG4L, RG2L, RG3L, parw02, parw03);
          if (RG1L == 0)
            skip00288 = true;
          else
          {
            RG1W = DM2_RAND16((word_at(s350.v1e0552, lcon(0x18)) >> 8 & 0xf) / 4 + 1);
            if (RG1W > 1)
              //m_16646:
              RG1L = 0;
            else
              skip00288 = true;
          }
        }
      }
      else
        skip00288 = true;

      if (skip00288)
        //m_1663F:
        RG1L = 1;

      //m_16648:
      if (RG1L == 0)
        jump L_fin;
      or8(location(DOWNCAST(unk, xp_0c) + lcon(0xb)), lcon(0x20));
      //m_16657:
      for (;;)
      {
        if ((vql_08.peek8() & bcon(0x10)) != 0)
        {
          bool skip00290 = false;
          bool skip00292 = false;
          RG1P = DOWNCAST(unk, xp_0c);
          RG1W = word_at(RG1P, lcon(0xa)) & lcon(0x8);
          RG1L = unsignedlong(RG1W);
          if (RG1L == 0)
            skip00290 = true;
          else
          {
            RG1L = signedlong(DM2_RANDDIR());
            if (RG1W == 0)
              skip00290 = true;
          }

          if (skip00290)
          {
            bool skip00291 = false;
            //m_1667D:
            RG1P = DOWNCAST(unk, xp_0c);
            RG1W = word_at(RG1P, lcon(0xa)) & lcon(0x40);
            RG1L = unsignedlong(RG1W);
            if (RG1L == 0)
              skip00291 = true;
            else
            {
              RG1L = DM2_RANDBIT() ? 1 : 0;
              if (RG1W == 0)
                skip00291 = true;
            }

            if (skip00291)
            {
              //m_16699:
              RG1L = signedlong(DM2_RANDDIR());
              if (RG1W == 0)
                skip00292 = true;
            }
            else
              skip00292 = true;
          }
          else
            skip00292 = true;

          if (skip00292)
          {
            //m_166A3:
            RG4L = unsignedlong(s350.v1e0562.getyA());
            RG1L = DM2_19f0_13aa(unsignedlong(s350.v1e0562.getxA()), RG4L);
            if (RG1L != 0)
            {
              mov8(location(RG61p + lcon(0x12)), lcon(0xffffffff));
              mov8(location(RG61p + lcon(0x13)), 0);
              mov8(location(RG61p + lcon(0x1a)), lcon(0x55));
              s350.v1e0898 = NULL;
              dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0x100));
              return;
            }
          }
        }

        //m_166CC:
        RG51w = wcon(0x4);
        RG1L = DM2_1c9a_381c();
        if (RG1L <= 0)
        {
          //m_166DF:
          RG1L = DM2_RANDBIT() ? 1 : 0;
          if (RG1W == 0)
            //m_16704:
            RG1L = signedlong(DM2_RANDDIR());
          else
          {
            RG1P = DOWNCAST(unk, xp_0c);
            RG1W = word_at(RG1P, lcon(0xe));
            RG1L <<= bcon(0x6);
            RG1UW >>= 14;
            RG1L = unsignedlong(RG1W) + 2;
            RG1L &= 0x3;
          }
        }
        else
          RG1Blo = byte_at(RG61p, lcon(0x1b));
        //m_16709:
        vb_20 = RG1Blo;
        RG1L = DM2_RANDBIT() ? 1 : 0;
        if (RG1W == 0)
          //m_1671D:
          RG1L = lcon(0xffffffff);
        else
          RG1L = 1;
        //m_16722:
        vb_1c = RG1Blo;
        //m_16725:
        for (;;)
        {
          if (--RG51w != lcon(0xffff))
          {
            RG1L = unsignedlong(vb_20);
            put16(parw05, RG1W);
            put16(parw04, lcon(0xffffffff));
            RG3L = lcon(0xffffffff);
            RG2L = unsignedlong(s350.v1e0562.getyA());
            RG4L = unsignedlong(s350.v1e0562.getxA());
            if (vl_10 == 0)
              //m_16753:
              RG1Blo = s350.v1e07ed;
            else
              RG1Blo = 0;
            //m_16758:
            RG1Blo |= lcon(0xffffff80);
            RG1L = DM2_CREATURE_GO_THERE(unsignedlong(RG1Blo), RG4L, RG2L, RG3L, parw04, parw05);
            if (RG1L == 0)
            {
              //m_16775:
              RG1Blo = vb_20 + vb_1c & lcon(0x3);
              vb_20 = RG1Blo;
              continue;
            }
            mov8(location(RG61p + lcon(0x12)), lcon(0xffffffff));
            mov8(location(RG61p + lcon(0x13)), 0);
            s350.v1e0898 = NULL;
            dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0x100));
            return;
          }
          //m_16782:
          RG3L = vl_10 + 1;
          vl_10 = RG3L;
          if (RG3L != lcon(0x1))
            jump L_fin;
          RG1L = DM2_RANDBIT() ? 1 : 0;
          if (RG1W == 0)
            jump L_fin;
          break;
        }
      }
      FOREND
    }
    else
    {
      RG1L = DM2_RAND();
      RG1Blo &= lcon(0x7);
      vb_1c = RG1Blo;
      RG1L = signedlong(vb_1c);
      if (RG1L <= lcon(0x3))
      {
        if (vb_1c == 0)
        {
          //m_1653E:
          RG1W = CUTX16(timdat.gametick) & lcon(0x3);
          RG1L = signedlong(RG1W);
          DM2_19f0_0559(RG1W);
          return;
        }
        RG1P = DOWNCAST(unk, xp_0c);
        RG1W = word_at(RG1P, lcon(0xe));
        RG1L <<= bcon(0x6);
        RG1UW >>= 14;
        mov8(location(RG61p + lcon(0x1b)), RG1Blo);
        RG1P = DOWNCAST(unk, xp_0c);
        RG1W = word_at(RG1P, lcon(0xe));
        RG1L <<= bcon(0x6);
        RG1UW >>= 14;
        RG1L = unsignedlong(RG1W);
        RG4W = unsignedword(RG4Blo);
        RG4Blo = s350.v1e0562.getxA();
        RG4W += table1d27fc[RG1L];
        RG4W &= lcon(0x1f);
        and8(location(RG61p + lcon(0x18)), lcon(0xffffffe0));
        RG3W = word_at(RG61p, lcon(0x18));
        RG3L |= RG4L;
        mov16(location(RG61p + lcon(0x18)), RG3W);
        RG1P = DOWNCAST(unk, xp_0c);
        RG1W = word_at(RG1P, lcon(0xe));
        RG1L <<= bcon(0x6);
        RG1UW >>= 14;
        RG1L = unsignedlong(RG1W);
        RG4Blo = s350.v1e0562.getyA();
        RG4W += table1d2804[RG1L];
        RG4W &= lcon(0x1f);
        RG1L = RG3L & lcon(0xfffffc1f);
        RG4L <<= bcon(0x5);
        RG2L = RG1L | RG4L;
        mov16(location(RG61p + lcon(0x18)), RG2W);
        RG1L = RG2L << 6;
        _RG1.ushiftr11();
        RG4L = signedlong(RG1W);
        RG1L = RG2L;
        RG1Bhi ^= RG2Bhi;
        RG1Blo &= lcon(0x1f);
        RG1L = (dm2_GET_TILE_VALUE(signedlong(RG1W), RG4L) & lcon(0xff)) >> 5;
        RG4Bhi = RG1Blo;
        if (RG1Blo != 0)
        {
          RG1L = unsignedlong(RG4Bhi);
          if (RG1L != lcon(0x7))
          {
            bool skip00286 = false;
            if (RG1L != lcon(0x5))
              skip00286 = true;
            else
            {
              RG1W = word_at(RG61p, lcon(0x18));
              RG1L <<= bcon(0x6);
              _RG1.ushiftr11();
              RG2L = signedlong(RG1W);
              RG1W = word_at(RG61p, lcon(0x18)) & lcon(0x1f);
              RG4L = signedlong(RG1W);
              RG1L = DM2_GET_TELEPORTER_DETAIL(&c12_00, RG4L, RG2L);
              if (RG1L == 0)
                skip00286 = true;
              else
              {
                RG1L = DM2_IS_CREATURE_ALLOWED_ON_LEVEL(unsignedlong(s350.v1e054c), unsignedlong(c12_00.b_04));
                if (RG1L != 0)
                  skip00286 = true;
              }
            }

            if (skip00286)
            {
              //m_16535:
              mov8(location(RG61p + lcon(0x1a)), 2);
              return;
            }
          }
        }
      }
      skip00285 = true;
    }
  }
  else
    skip00285 = true;

  if (skip00285)
  {
    //m_16407:
    mov8(location(RG61p + lcon(0x1a)), 0);
    return;
  }

L_fin:
  mov8(location(RG61p + lcon(0x1a)), lcon(0xffffffff));
  RG1L = DM2_14cd_0684();
  RG4Blo = RG1Blo;
  if (RG1Blo == lcon(0xfffffffd))
  {
    RG1Blo = s350.v1e07ec & lcon(0xffffff80);
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 0)
    {
      //m_167C2:
      RG1L = DM2_14cd_08f5(lcon(0xfffffffd));
      if (RG1L != 0)
      {
        RG2W = unsignedword(RG2Blo);
        s350.v1e07d8.b_01 = RG2Bhi;
      }
      mov8(location(RG61p + lcon(0x1a)), lcon(0x33));
    }
    else
      mov8(location(RG61p + lcon(0x1a)), 0);
    s350.v1e0898 = NULL;
    dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0x100));
    return;
  }
  //m_167E1:
  if (RG1Blo < 0)
    skip00294 = true;
  else
  {
    RG2L = signedlong(RG1Blo);
    if (RG2L >= lcon(0x3f))
      skip00294 = true;
    else
    {
      mov8(location(RG61p + lcon(0x12)), RG1Blo);
      RG2Blo = 0;
      vb_1c = RG2Blo;
      mov8(location(RG61p + lcon(0x13)), RG2Blo);
    }
  }

  if (skip00294)
  {
    //m_167FA:
    RG4Blo = byte_at(RG61p, lcon(0x12));
    RG1Blo = byte_at(RG61p, lcon(0x13));
    vb_1c = RG1Blo;
  }

  //m_16803:
  RG2P = DOWNCAST(s_seven, table1d5f82[signedlong(RG4Blo)]);
  RG4L = signedlong(vb_1c);
  RG1L = 7 * RG4L;
  RG4Bhi = byte_at(RG2P, RG1L + lcon(0x5)) & lcon(0x1f);
  RG1Blo = byte_at(RG2P, RG1L + lcon(0x6));
  vb_18 = RG1Blo;
  RG52w = wcon(0x0);
  vl_10 = 0;
  //m_1682F:
  for (;;)
  {
    bool skip00298 = false;
    RG4Blo = byte_at(RG61p, lcon(0x12));
    RG1Blo = byte_at(RG61p, lcon(0x13));
    vb_1c = RG1Blo;
    RG2P = DOWNCAST(s_seven, table1d5f82[signedlong(RG4Blo)]);
    if (vl_10 != 0)
    {
      bool skip00297 = false;
      RG1L = signedlong(vb_1c);
      vl_14 = RG1L;
      RG1L *= 8;
      RG1L -= vl_14;
      RG1Blo = byte_at(RG2P, RG1L + lcon(0x5)) & lcon(0x1f);
      vb_20 = RG1Blo;
      if (RG4Bhi == RG1Blo && RG1Blo != lcon(0x1))
        skip00297 = true;
      else
      {
        RG3Bhi = vb_20;
        if (RG3Bhi == 0)
          skip00297 = true;
        else
        {
          bool skip00296 = false;
          if (RG4Bhi != lcon(0x1) || RG3Bhi != RG4Bhi)
            skip00296 = true;
          else
          {
            RG1L = signedlong(vb_1c);
            vl_14 = RG1L;
            RG1L *= 8;
            RG1L -= vl_14;
            RG1Blo = byte_at(RG2P, RG1L + lcon(0x6));
            if (RG1Blo != vb_18)
              skip00296 = true;
          }

          if (skip00296)
          {
            //m_16890:
            RG1L = 1;
            skip00298 = true;
          }
          else
            skip00297 = true;
        }
      }

      if (skip00297)
      {
        //m_16897:
        RG1L = 0;
        skip00298 = true;
      }
    }

    if (skip00298)
    {
      //m_16899:
      if (RG1L != 0)
      {
        mov8(location(RG61p + lcon(0x1a)), lcon(0x33));
        break;
      }
    }

    //m_168A1:
    RG1Blo = DM2_PROCEED_XACT(DM2_DECIDE_NEXT_XACT(RG1L));
    RG4Blo = RG1Blo;
    if (RG1Blo != lcon(0xfffffffc))
    {
      RG1L = DM2_14cd_08f5(signedlong(RG1Blo));
      vl_10 |= RG1L;
    }
    if (vl_10 != 0)
      s350.v1e07d8.b_01 = 0;
    RG1L = signedlong(RG52w);
    RG52w++;
    if (RG1L > lcon(0x20))
      mov8(location(RG61p + lcon(0x1a)), 0);
    RG1L = signedlong(byte_at(RG61p, lcon(0x1a)));
    if (RG1L == lcon(0xffffffff))
    {
      if (RG4Blo != lcon(0xfffffffc))
      {
        RG1Blo = byte_at(RG61p, lcon(0x12)) == bcon(0xff) ? 1 : 0;
        RG1L = unsignedlong(RG1Blo);
        if (RG1L != 0)
          mov8(location(RG61p + lcon(0x1a)), 0);
      }
    }
    RG1L = signedlong(byte_at(RG61p, lcon(0x1a)));
    if (RG1L != lcon(0xffffffff))
      break;
  }
  s350.v1e0898 = NULL;
  dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0x100));
}

// belongs to DM2_13e4_0982
static i32 DM2_50CB(i32 eaxl, i32 edxl, unk* xebxp, unk** xecxpp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG3; RG3P = DOWNCAST(unk*, xecxpp);
  c_nreg _RG4; RG4L = edxl;
  i8* RG51p;
  i32 RG52l;
  i16 RG6w;
  i8* RG7p;
  bool skip00006 = false;

  RG6w = RG4W;
  RG51p = RG2P;
  RG7p = RG3P;
  RG4L = unsignedlong(RG1Blo);
  RG3L = lcon(0xfc);
  RG2L = lcon(0x7);
  RG1P = DM2_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0xf), RG4Blo, lcon(0x7), lcon(0xfc));
  RG2P = RG1P;
  RG4L = unsignedlong(word_at(RG51p));
  if (RG4L != lcon(0xffff))
  {
    //m_5102:
    RG3L = unsignedlong(RG6w);
    RG4L += RG3L;
    RG1Blo = byte_at(RG1P, 4 * RG4L + 2) & lcon(0x3f);
    RG1W = unsignedword(RG1Blo);
    if (RG1W != 0)
      //m_511D:
      add16(location(RG51p), RG1W);
    else
    {
      RG1L = 2;
      skip00006 = true;
    }
  }
  else
    mov16(location(RG51p), 0);

  if (!skip00006)
  {
    //m_5120:
    RG1L = unsignedlong(word_at(RG51p));
    RG4L = unsignedlong(RG6w);
    RG1L += RG4L;
    RG1Blo = byte_at(RG2P, 4 * RG1L + lcon(0x1)) & lcon(0xfffffff0);
    RG1L = unsignedlong(RG1Blo) >> 4;
    if (RG1L != 0)
      //m_5142:
      RG1L = 1;
    else
      RG1L = 0;
  }

  //m_5147:
  RG4L = unsignedlong(word_at(RG51p));
  RG52l = unsignedlong(RG6w);
  RG4L += RG52l;
  RG4L *= 4;
  RG2P += RG4L;
  movp(location(RG7p + 0), RG2P);
  return RG1L;
}

// belongs to DM2_THINK_CREATURE
static void DM2_13e4_0982(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG51p;
  i8* RG52p;
  i8* RG61p;
  i16 vw_00;
  unk** xparpp01;
  bool flag;
  bool skip00261 = false;

  RG51p = DOWNCAST(c_creature, s350.creatures);
  if (ddat.savegames1.b_03 == 0)
    skip00261 = true;
  else
  {
    RG1P = s350.v1e0552;
    if (jnz_test8(location(RG1P + lcon(0x1)), lcon(0x10)))
      skip00261 = true;
    else
    {
      bool skip00260 = false;
      RG1L = signedlong(byte_at(RG51p, lcon(0x1a)));
      if (RG1L != lcon(0x13))
      {
        RG1L = signedlong(byte_at(RG51p, lcon(0x17)));
        if (RG1L != lcon(0x13))
        {
          RG1L = 1;
          skip00260 = true;
        }
      }

      if (!skip00260)
        //m_15511:
        RG1L = 0;

      //m_15513:
      if (RG1L == 0)
        skip00261 = true;
      else
        s350.v1e0562.adddata(4);
    }
  }

  if (skip00261)
  {
    bool skip00269 = false;
    //m_15523:
    RG61p = s350.v1e055e;
    if (!s350.v1e0562.is_type(0x22))
    {
      //m_156C7:
      flag = false;
      RG2P = RG61p + 2;
      RG4L = unsignedlong(word_at(RG61p));
      RG1L = DM2_4FCC(unsignedlong(byte_at(DOWNCAST(SPX_Creature, s350.v1e054e), lcon(0x4))), RG4L, RG2P16, &s350.v1e055a);
      skip00269 = true;
    }
    else
    {
      flag = true;
      RG1Blo = byte_at(RG51p, lcon(0x17));
      mov8(location(RG51p + lcon(0x1a)), RG1Blo);
      RG1L = signedlong(RG1Blo);
      if (RG1L == lcon(0xffffffff))
        //m_155D7:
        DM2_14cd_09e2();
      else
      {
        bool skip00262 = false;
        mov8(location(RG51p + lcon(0x17)), lcon(0xffffffff));
        RG1L = DM2_14cd_062e();
        if (RG1Blo == 0)
          skip00262 = true;
        else
        {
          RG1L = signedlong(byte_at(RG51p, lcon(0x1a)));
          if (RG1L == lcon(0x13))
            skip00262 = true;
          else
            mov8(location(RG51p + lcon(0x1a)), lcon(0xffffffff));
        }

        if (skip00262)
        {
          //m_1556E:
          RG1L = signedlong(byte_at(RG51p, lcon(0x1a)));
          RG1Blo = table1d613a[RG1L] & lcon(0x4);
          RG1L = unsignedlong(RG1Blo);
          if (RG1L != 0)
          {
            RG2Blo = 0;
            s350.v1e07d8.b_01 = RG2Blo;
            s350.v1e07d8.b_03 = lcon(0xffffffff);
          }
          RG1Blo = byte_at(RG51p, lcon(0x1a));
          if (mkub(RG1Blo) >= mkub(lcon(0x6)))
          {
            bool skip00263 = false;
            if (mkub(RG1Blo) <= mkub(lcon(0x6)))
            {
              //m_155A1:
              RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
              RG1W = word_at(RG1P, lcon(0xe));
              RG1L <<= bcon(0x6);
              RG1UW >>= 14;
              RG1L = unsignedlong(RG1W) - 1;
              skip00263 = true;
            }
            else
            {
              if (RG1Blo == lcon(0x7))
              {
                //m_155BF:
                RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
                RG1W = word_at(RG1P, lcon(0xe));
                RG1L <<= bcon(0x6);
                RG1UW >>= 14;
                RG1L = unsignedlong(RG1W) + 1;
                skip00263 = true;
              }
            }

            if (skip00263)
            {
              //m_155B7:
              RG1L &= 0x3;
              mov8(location(RG51p + lcon(0x1d)), RG1Blo);
            }
          }
        }
      }
      //m_155DC:
      RG1L = signedlong(byte_at(RG51p, lcon(0x1a)));
      if (RG1L == lcon(0x13))
      {
        RG1W = word_at(RG61p);
        mov16(location(RG51p + lcon(0xe)), RG1W);
        RG1W = word_at(RG61p, 2);
        mov16(location(RG51p + lcon(0x10)), RG1W);
        RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
        RG1Blo = byte_at(RG1P, lcon(0x4));
        RG1L = unsignedlong(RG1Blo);
        RG4L = 1;
        RG1L = DM2_QUERY_GDAT_CREATURE_WORD_VALUE(RG1L, RG4L) & lcon(0xffff);
        if ((table1d607e[RG1L].uc[0] & ubcon(0x8)) == 0)
        {
          bool skip00266 = false;
          RG1P = s350.v1e0552;
          RG1Blo = byte_at(RG1P, lcon(0x23));
          if (mkub(RG1Blo) >= mkub(lcon(0x1)))
          {
            if (mkub(RG1Blo) <= mkub(lcon(0x1)))
            {
              //m_15639:
              RG1L = lcon(0xbe);
              skip00266 = true;
            }
            else
            {
              if (RG1Blo == 2)
              {
                //m_15640:
                RG1L = lcon(0xff);
                skip00266 = true;
              }
            }
          }

          if (!skip00266)
            //m_15632:
            RG1L = lcon(0x6e);

          //m_15645:
          RG3L = unsignedlong(s350.v1e0562.getyA());
          RG2L = unsignedlong(s350.v1e0562.getxA());
          RG4L = RG1L & lcon(0xffff);
          DM2_CREATE_CLOUD(lcon(0xffa8), RG4L, RG2L, RG3L, lcon(0xff));
        }
      }
      bool skip00268 = false;
      //m_1566C:
      RG1W = signedword(byte_at(RG51p, lcon(0x1a)));
      RG4L = signedlong(byte_at(RG51p, lcon(0x1a)));
      if (RG4L < lcon(0x32))
        skip00268 = true;
      else
      {
        RG4L = signedlong(RG1W);
        if (RG4L > lcon(0x34))
          skip00268 = true;
        else
          s350.v1e0562.setmticks(s350.v1e0571, RG4L + timdat.gametick - 50);
      }

      if (skip00268)
      {
        //m_156A4:
        xparpp01 = &s350.v1e055a;
        RG3P = RG61p + 2;
        RG4L = signedlong(RG1W);
        RG1L = DM2_GET_CREATURE_ANIMATION_FRAME(unsignedlong(byte_at(DOWNCAST(SPX_Creature, s350.v1e054e), lcon(0x4))), RG4L, UPCAST(i16, RG61p), RG3P16, xparpp01, 0);
        skip00269 = true;
      }
    }

    if (skip00269)
    {
      //m_156E8:
      RG4L = RG1L;
      //m_156EA:
      for (;;)
      {
        bool skip00271 = false;
        if (!flag && byte_at(RG51p, lcon(0x21)) != 0)
        {
          RG1P = s350.v1e055a;
          RG1Blo = byte_at(RG1P, 2) & lcon(0x40);
          RG1L = unsignedlong(RG1Blo) >> 6;
          if (RG1L != 0)
          {
            //m_15711:
            RG1L = 0;
            skip00271 = true;
          }
        }

        if (!skip00271)
          //m_1570A:
          RG1L = 1;

        //m_15713:
        if (RG1L != 0)
        {
          RG1P = s350.v1e055a;
          RG1Blo = byte_at(RG1P, 2) & lcon(0xffffff80);
          RG1L = unsignedlong(RG1Blo) >> 7;
          if (RG1L != 0)
          {
            DM2_13e4_01a3();
            RG1L = DM2_PROCEED_CCM();
            or8(location(RG51p + lcon(0x21)), RG1Blo);
          }
        }
        //m_1573A:
        if (byte_at(RG51p, lcon(0x21)) != 0)
        {
          RG1P = s350.v1e055a;
          RG1Blo = byte_at(RG1P, 2) & lcon(0x40);
          RG1L = unsignedlong(RG1Blo) >> 6;
          if (RG1L != 0)
          {
            RG2P = RG61p + 2;
            RG4L = unsignedlong(word_at(RG61p));
            RG1L = DM2_50CB(unsignedlong(byte_at(DOWNCAST(SPX_Creature, s350.v1e054e), lcon(0x4))), RG4L, RG2P, &s350.v1e055a);
            RG4L = RG1L;
            RG1L = unsignedlong(RG1W);
            if (RG1L != 2)
              continue;
          }
        }
        break;
      }
      //m_15785:
      RG1L = unsignedlong(RG4W);
      RG1Blo = (RG1L != lcon(0x1) ? 1 : 0) + lcon(0x21);
      s350.v1e0562.settype(RG1Blo);
      if (s350.v1e0570 != 0)
        return;
      s350.v1e0562.setmticks(s350.v1e0571, DM2_CREATURE_SOMETHING_1c9a_0a48());
    }
  }

  //m_157BC:
  if (s350.v1e07eb != 0)
  {
    if (s350.v1e07d8.b_00 != 0 || s350.v1e07d8.b_03 != lcon(0xffffffff))
    {
      RG1W = s350.v1e054c;
      RG1Bhi &= 0x3;
      RG1L = unsignedlong(RG1W) | ulcon(0x20000000);
      RG1L = dm2_dballochandler.DM2_ALLOCATION11(RG1L, lcon(0x1), OO /*TAG dbidx*/ vw_00);
      if (RG1L != 0)
        //m_15824:
        RG52p = DOWNCAST(t_bmp, dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ vw_00));
      else
      {
        RG1P = DOWNCAST(s_xmalloc, dm2_dballochandler.DM2_ALLOC_CPXHEAP_MEM((t_dbidx)vw_00, lcon(0xe)));
        RG52p = RG1P;
        dm2_dballochandler.DM2_ALLOCATION1((t_dbidx)vw_00, true);
      }
      //m_15832:
      DM2_COPY_MEMORY(RG52p, DOWNCAST(c_4b3wp, &s350.v1e07d8), lcon(0xe));
    }
  }
  //m_15843:
  RG1L = signedlong(word_at(DOWNCAST(c_creature, s350.creatures), 2));
  if (RG1L != lcon(0xffffffff))
    DM2_1c9a_0db0(unsignedlong(s350.v1e054c));
  RG1W = DM2_QUEUE_TIMER(&s350.v1e0562);
  RG4L = RG1L;
  mov16(location(DOWNCAST(c_creature, s350.creatures) + 2), RG4W);
}

void DM2_THINK_CREATURE(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG4; RG4L = edxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG3;
  i16 RG51w;
  i32 RG52l;
  i16 RG53w;
  i32 RG61l;
  c_wlreg _RG62;
  i8* RG7p;
  i32 vl_00;
  i32 vl_04;
  c_ol32 vol_08; // poke 16 peek 16 32 sub 16 add 32
  bool skip00274 = false;

  RG3L = RG1L;
  RG53w = RG4W;
  RG4L = unsignedlong(RG53w);
  RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG3W), RG4L);
  if (RG1W == lcon(0xffffffff))
    return;
  RG61l = signedlong(RG2W);
  RG2L = signedlong(RG53w);
  RG4L = signedlong(RG3W);
  RG1P = DM2_PREPARE_LOCAL_CREATURE_VAR(unsignedlong(RG1W), RG4L, RG2L, RG61l);
  RG7p = RG1P;
  RG2P = DOWNCAST(c_creature, s350.creatures);
  RG3P = s350.v1e0552;
  mov16(location(RG2P + 2), lcon(0xffffffff));
  RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
  if (word_at(RG1P + lcon(0x6)) == 0)
  {
    mov16(location(RG2P + lcon(0x14)), 1);
    RG4W = word_at(RG2P, lcon(0x14));
    RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
    mov16(location(RG1P + lcon(0x6)), RG4W);
  }
  RG62W = word_at(RG2P, lcon(0x14));
  mov16(location(RG2P + lcon(0x14)), 0);
  RG51w = signedword(byte_at(RG3P, lcon(0x3)));
  if (RG51w != 0)
  {
    if (RG51w < 0)
      RG51w = -RG51w;
    RG1L = timdat.gametick;
    RG1UL >>= bcon(0x2);
    RG1W = unsignedword(RG1Blo);
    vol_08.poke16(RG1W);
    RG1Blo = byte_at(RG2P, lcon(0x6));
    if (RG1W > vol_08.peek16())
      vol_08.add32(0x100); // inc byte1
    RG1L = unsignedlong(byte_at(RG2P, lcon(0x6)));
    vol_08.sub16(RG1W);
    RG4L = signedlong(vol_08.peek16());
    RG1L = signedlong(RG51w);
    vl_00 = RG1L;
    RG1L = RG4L;
    RG4L >>= bcon(0x1f);
    RG4L = RG1L % vl_00;
    RG1L /= vl_00;
    vl_04 = RG1L;
    if (RG1W > 0)
    {
      if (byte_at(RG3P, lcon(0x3)) >= bcon(0x0))
      {
        //m_15954:
        RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
        RG4W = word_at(RG1P, lcon(0x6));
        if (RG4UW < mkuw(word_at(RG3P, lcon(0x4))))
        {
          RG4L = vl_04;
          add16(location(RG1P + lcon(0x6)), RG4W);
        }
      }
      else
        RG62L += RG1L;
      //m_1596B:
      RG4L = signedlong(vol_08.peek16());
      RG52l = signedlong(RG51w);
      RG1L = RG4L;
      RG4L = RG1L % RG52l;
      RG1L /= RG52l;
      RG1L = timdat.gametick;
      RG1UL >>= bcon(0x2);
      RG1L -= RG4L;
      mov8(location(RG2P + lcon(0x6)), RG1Blo);
    }
  }
  //m_15989:
  if (RG62W > 0)
  {
    if (jz_test8(location(RG3P), 1))
      and8(location(DOWNCAST(SPX_Creature, s350.v1e054e) + lcon(0xb)), lcon(0x7f));
    RG1L = DM2_WOUND_CREATURE(unsignedlong(RG62W));
    if (RG1L != 0)
      skip00274 = true;
  }

  if (!skip00274)
  {
    //m_159AA:
    if (jnz_test8(location(RG3P), 1))
    {
      //m_159B6:
      RG1P = s350.v1e055e;
      RG4W = word_at(RG1P, 2) & lcon(0x4000);
      RG4L = unsignedlong(RG4W);
      if (RG4L == 0)
      {
        //m_159D3:
        RG1W = word_at(RG1P, 2) & lcon(0x2000);
        RG1L = unsignedlong(RG1W);
        if (RG1L != 0)
          DM2_ai_13e4_0806();
      }
      else
        DM2_ai_13e4_071b();
      //m_159E8:
      RG1L = signedlong(word_at(RG2P, 2));
      if (RG1L == lcon(0xffffffff))
      {
        RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
        RG1Blo = byte_at(RG1P, lcon(0x5));
        RG1L = unsignedlong(RG1Blo);
        DM2_1c9a_0fcb(RG1L);
      }
    }
    else
      DM2_13e4_0982();
  }

  //m_15A04:
  DM2_UNPREPARE_LOCAL_CREATURE_VAR(RG7p);
}

static i32 DM2_4EA8(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;

  RG5w = RG4W;
  RG4L = unsignedlong(RG1Blo);
  RG3L = lcon(0xfc);
  RG2L = lcon(0x7);
  RG1P = DM2_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0xf), RG4Blo, lcon(0x7), lcon(0xfc));
  RG4L = unsignedlong(RG5w) << 2;
  RG1P += RG4L;
  RG4L = 1;
  //m_4EDA:
  for (;;)
  {
    RG2Blo = byte_at(RG1P, lcon(0x1)) & lcon(0xfffffff0);
    RG2L = unsignedlong(RG2Blo) >> 4;
    if (RG2L == 0)
      break;
    //m_4ED6:
    RG4L++;
    RG1P += 4;
  }
  return RG4L;
}

// was SKW_13e4_0004 (new)
unk* DM2_PREPARE_LOCAL_CREATURE_VAR(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p = NULL;
  i16 RG6w;
  i16 vw_00;
  i8 vb_04;
  i16 vw_08;
  i8 vb_0c;

  RG6w = RG1W;
  vb_0c = RG4Blo;
  vb_04 = RG2Blo;
  vw_08 = RG3W;
  vw_00 = ddat.v1d3248;
  RG4L = lcon(0xffffffff);
  ddat.v1d3248 = RG4W;
  DM2_1c9a_0648(signedlong(vw_00));
  ddat.v1e08a8 = RG4W;
  if (s350.v1e07ea != 0)
  {
    RG2L = lcon(0x400);
    RG4L = 0;
    RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x350));
    RG5p = RG1P;
    DM2_COPY_C350(RG1P, &s350);
  }
  //m_14BA7:
  s350.v1e07ea = 1;
  RG4W = unsignedword(RG4Blo);
  s350.v1e07eb = RG4Bhi;
  s350.v1e054c = RG6w;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(RG6w)));
  RG3P = RG1P;
  s350.v1e054e = UPCAST(SPX_Creature, RG1P);
  if (byte_at(RG1P, lcon(0x5)) == lcon(0xffffffff))
    //m_14BEC:
    RG2P = NULL;
  else
  {
    RG2L = unsignedlong(byte_at(RG1P, lcon(0x5)));
    RG1L = 34 * RG2L;
    RG2P = DOWNCAST(c_creature, creatures) + RG1L;
  }
  //m_14BEE:
  s350.creatures = UPCAST(c_creature, RG2P);
  RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG3P, lcon(0x4)))));
  s350.v1e0552 = RG1P;
  RG1P = DOWNCAST(i16, DM2_query_1c9a_02c3(RG3P, RG1P));
  s350.v1e055e = RG1P;
  RG1Blo = CUTX8(vw_00);
  s350.v1e0571 = RG1Blo;
  s350.v1e0562.setmticks(vw_00, timdat.gametick);
  s350.v1e0562.setactor(byte_at(RG3P, lcon(0x4)));
  RG1Blo = CUTX8(vw_08);
  s350.v1e0562.settype(CUTX8(vw_08));
  s350.v1e0562.setxyA(vb_0c, vb_04);
  s350.v1e055a = NULL;
  s350.v1e0570 = 0;
  s350.v1e0584 = lcon(0xffffffff);
  if (vw_08 != lcon(0x22))
    return RG5p;
  RG1L = 0;
  s350.v1e0572 = RG1W;
  s350.v1e0574 = RG1W;
  RG1L = signedlong(byte_at(RG2P, lcon(0x1a)));
  if (RG1L == lcon(0xffffffff))
    RG1L = 0;
  s350.v1e056e = RG1Blo;
  DM2_ZERO_MEMORY(RG2P + lcon(0x18), lcon(0xa));
  mov8(location(RG2P + lcon(0x1a)), lcon(0xffffffff));
  return RG5p;
}

// was SKW_13e4_0329
void DM2_UNPREPARE_LOCAL_CREATURE_VAR(unk* xeaxp)
{
  c_nreg _RG1; RG1P = xeaxp;

  if (RG1P != NULL)
  {
    DM2_COPY_C350(&s350, RG1P);
    dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0x350));
  }
  else
  {
    RG1W = unsignedword(RG1Blo);
    s350.v1e07ea = RG1Bhi;
  }
}

// stacksize was 0x10
void DM2_ai_13e4_0360(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  c_lreg _RG61;
  c_lreg _RG62;
  i8 vb_00;

  RG5w = RG4W;
  vb_00 = RG3Blo;
  if (RG1W == lcon(0xffffffff))
  {
    RG4L = unsignedlong(RG2W);
    RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG5w), RG4L);
    if (RG1W == lcon(0xffffffff))
      return;
  }
  RG4L = unsignedlong(RG1W);
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
  RG1Blo = byte_at(RG1P, lcon(0x5));
  if (RG1Blo == lcon(0xffffffff))
    return;
  RG61L = unsignedlong(RG1Blo);
  RG1L = 16 * RG61L;
  RG61L += RG1L;
  RG1P = DOWNCAST(c_creature, creatures) + 2 * RG61L;
  if (signedlong(byte_at(RG1P, lcon(0x17))) == lcon(0x13))
    return;
  if (signedlong(byte_at(RG1P, lcon(0x1a))) == lcon(0x13))
    return;
  RG3Blo = vb_00;
  mov8(location(RG1P + lcon(0x17)), RG3Blo);
  if (argl0 == 0)
    return;
  RG3Blo = table1d613a[signedlong(byte_at(RG1P, lcon(0x1a)))] & lcon(0x10);
  RG62L = unsignedlong(RG3Blo);
  if (RG62L != 0)
    mov8(location(RG1P + lcon(0x21)), lcon(0x1));
  else
  {
    DM2_1c9a_0db0(RG4L);
    RG4L = unsignedlong(RG2W);
    RG1L = unsignedlong(RG5w);
    DM2_1c9a_0cf7(RG1L, RG4L);
  }
}

void DM2_ai_13e4_071b(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  i32 RG6l;

  RG2P = s350.v1e055e;
  RG1W = word_at(RG2P, 2);
  RG1L &= lcon(0xe03f);
  RG1L = unsignedlong(RG1W);
  if (RG1L == lcon(0x8001))
    return;
  RG4L = unsignedlong(word_at(RG2P));
  RG1L = DM2_4EA8(unsignedlong(byte_at(DOWNCAST(SPX_Creature, s350.v1e054e), lcon(0x4))), RG4L);
  RG3L = RG1L;
  RG5w = word_at(RG2P, 2) & wcon(0xfc0);
  RG1L = unsignedlong(RG5w) + timdat.gametick;
  RG6l = signedlong(RG3W);
  RG4L = RG1L % RG6l;
  RG1L /= RG6l;
  if (RG4W == 0)
  {
    RG5w |= wcon(0x8001);
    mov16(location(RG2P + 2), RG5w);
  }
  else
  {
    RG5w |= RG3W;
    RG5w |= wcon(0xc000);
    mov16(location(RG2P + 2), RG5w);
    DM2_1c9a_0db0(unsignedlong(s350.v1e054c));
    s350.v1e0562.setmticks(s350.v1e0571, signedlong(RG3W - RG4W) + timdat.gametick);
    mov16(location(DOWNCAST(c_creature, s350.creatures) + 2), DM2_QUEUE_TIMER(&s350.v1e0562));
  }
}

void DM2_ai_13e4_0806(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5;

  RG2P = s350.v1e055e;
  RG1W = word_at(RG2P, 2) & lcon(0xe000);
  RG1L = unsignedlong(RG1W);
  if (RG1L == lcon(0x8000))
  {
    RG1W = word_at(RG2P, 2) & lcon(0x3f);
    RG1L = unsignedlong(RG1W);
    if (RG1L > lcon(0x1))
      return;
  }
  RG4L = unsignedlong(word_at(RG2P));
  RG1L = DM2_4EA8(unsignedlong(byte_at(DOWNCAST(SPX_Creature, s350.v1e054e), lcon(0x4))), RG4L);
  RG3L = RG1L;
  RG5W = word_at(RG2P, 2);
  RG5L &= lcon(0xfc0);
  RG1L = unsignedlong(RG5W) + timdat.gametick;
  RG4L = RG1L % signedlong(RG3W);
  RG1L = RG3L | RG5L;
  if (RG4W == 0)
  {
    RG1Bhi |= lcon(0xffffff80);
    mov16(location(RG2P + 2), RG1W);
  }
  else
  {
    RG1Bhi |= lcon(0xffffffa0);
    mov16(location(RG2P + 2), RG1W);
    DM2_1c9a_0db0(unsignedlong(s350.v1e054c));
    s350.v1e0562.setmticks(s350.v1e0571, signedlong(RG3W - RG4W) + timdat.gametick);
    mov16(location(DOWNCAST(c_creature, s350.creatures) + 2), DM2_QUEUE_TIMER(&s350.v1e0562));
  }
}

