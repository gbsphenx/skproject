#include "util.h"
#include "xtypes.h"
#include "c_timer.h"
#include "c_querydb.h"
#include "dm2data.h"
#include "c_hero.h"
#include "c_moverec.h"
#include "c_light.h"
#include "c_map.h"
#include "c_random.h"
#include "c_gui_draw.h"
#include "c_sfx.h"
#include "c_tmouse.h"
#include "c_eventqueue.h"
#include "c_events.h"
#include "c_item.h"

#include "regs.h"
#include "emu.h"

// was SKW_2c1d_038b
i32 DM2_RETRIEVE_ITEM_BONUS(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;

  RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG1W), unsignedlong(RG4Blo));
  RG4L = RG1L;
  if (RG1W == 0)
    return 0;
  if ((RG1L & lcon(0x4000) & lcon(0xffff)) == 0)
  {
    //m_35B2E:
    if (RG2L == 0)
    {
      RG4Blo ^= RG1Blo;
      RG4Bhi &= lcon(0xffffff80);
      RG4L = unsignedlong(RG4W);
      if (RG4L == 0)
        return 0;
    }
  }
  else
  {
    if (RG3W != wcon(0xfffe) && RG3W != wcon(0x2))
    {
      if (RG3W != lcon(0x3))
        return 0;
    }
  }
  //m_35B3F:
  RG1Bhi = sgn8(RG1Blo);
  return RG3W >= 0 ? RG1L : -RG1L;
}

// was SKW_2c1d_03e7
void DM2_PROCESS_ITEM_BONUS(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG6;
  i32 RG7l;
  i16 vw_00;
  c_ql32 vql_04;
  c_tim c38_08;
  i32 vl_14;
  i32 vl_18;
  i32 vl_1c;
  c_o32 vo_20; // poke 16 peek 8 16 32 inc16
  i16 vw_24;

  vql_04.poke32(RG1L);
  vw_00 = RG4W; // truncated
  RG6L = RG3L;
  if (RG1W < 0)
    return;
  if (RG4W == lcon(0xffffffff))
    return;
  RG1L = signedlong(RG2W);
  if (RG1L < lcon(0x1e))
  {
    RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG4W), 0);
    vw_24 = RG1W;
    if (RG3W != 0)
    {
      RG1W &= lcon(0x10);
      RG1L = signedlong(RG1W);
      if (RG1L != 0)
        DM2_RECALC_LIGHT_LEVEL();
    }
    RG1W = vw_24;
    RG1W &= lcon(0x2000);
    RG1L = signedlong(RG1W);
    if (RG1L != 0)
    {
      RG4L = signedlong(vql_04.peek16());
      c_hero* hero = &party.hero[RG4L];
      RG4L = signedlong(RG2W);
      RG1L = unsignedlong(vw_00);
      vl_18 = RG1L;
      RG1L = DM2_IS_ITEM_FIT_FOR_EQUIP(RG1W, RG4L, 1);
      RG7l = RG1L;
      vl_14 = 0;
      if (RG6W != wcon(0x0) && RG6W != wcon(0x3) && RG6W != wcon(0xfffe))
      {
        RG3L = signedlong(RG6W);
        RG2L = RG1L;
        RG4L = lcon(0x14);
        RG1L = DM2_RETRIEVE_ITEM_BONUS(vl_18, RG4L, RG2L, RG3L);
        vw_24 = RG1W;
        if (RG1W != 0)
        {
          if (RG6W != 1 && RG6W != lcon(0xffffffff))
          {
            //m_35C36:
            if (RG6W == 2)
            {
              RG1W += hero->curMP;
              RG4L = signedlong(RG1W);
              RG2L = lcon(0x3e7);
              RG1L = signedlong(DM2_BETWEEN_VALUE(0, RG2W, RG4W));
              hero->curMP = RG1W;
            }
          }
          else
          {
            RG1W = vw_24;
            hero->maxMP += RG1W;
          }
          //m_35C53:
          hero->heroflag |= 0x800;
        }
        //m_35C57:
        RG3L = 0;
        vo_20.poke16(RG3W);
        //m_35CBB:
        for (;;)
        {
          RG1L = signedlong(vo_20.peek16());
          if (RG1L > lcon(0x6))
            break;
          //m_35C60:
          RG3L = signedlong(RG6W);
          RG1Blo = vo_20.peek8() + bcon(0x15);
          RG4L = unsignedlong(RG1Blo);
          RG1L = DM2_RETRIEVE_ITEM_BONUS(unsignedlong(vw_00), RG4L, RG7l, RG3L);
          vw_24 = RG1W;
          if (RG1W != 0)
          {
            if (RG6W != 1 && RG6W != lcon(0xffffffff))
            {
              //m_35CA1:
              RG2L = signedlong(RG1W);
              DM2_hero_2c1d_0300(hero, unsignedlong(vo_20.peek16()), RG2L);
            }
            else
            {
              RG1L = signedlong(vo_20.peek16());
              RG4Blo = CUTX8(vw_24);
              hero->eability[RG1L] += RG4Blo;
            }
            //m_35CB2:
            hero->heroflag |= 0x3000;
          }
          //m_35CB6:
          vo_20.inc16();
        }
      }
      //m_35CC7:
      RG4L = 0;
      vo_20.poke16(RG4W);
      //m_35D14:
      for (;;)
      {
        RG1L = signedlong(vo_20.peek16());
        vl_1c = RG1L;
        if (RG1L >= lcon(0x14))
        {
          RG3L = signedlong(RG6W);
          RG1L = DM2_RETRIEVE_ITEM_BONUS(unsignedlong(vw_00), lcon(0x33), RG7l, RG3L);
          vw_24 = RG1W;
          if (RG1W != 0)
          {
            RG1Blo = CUTX8(vw_24);
            hero->walkspeed += RG1Blo;
            vl_14 = 1;
          }
          if (RG6W != 2)
          {
            if (RG6W != lcon(0xfffffffe))
            {
              if (RG6W != lcon(0x3))
              {
                RG3L = signedlong(RG6W);
                RG1L = DM2_RETRIEVE_ITEM_BONUS(unsignedlong(vw_00), lcon(0x32), RG7l, RG3L);
                if (RG1W != 0)
                {
                  ddat.savegames1.w_00 += RG1W;
                  if (RG6W != 0)
                    RG1L = DM2_RECALC_LIGHT_LEVEL();
                }
              }
            }
          }
          if (vl_14 != 0 && RG6W == 2)
          {
            c38_08.settype(0xe);
            RG2L = unsignedlong(vw_00);
            RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG2L, lcon(0x13)) & lcon(0xffff);
            c38_08.setmticks(ddat.v1e0266, timdat.gametick + RG1L);
            RG1Blo = vql_04.peek8();
            c38_08.setactor(RG1UBlo);
            RG1L = RG2L;
            RG1Blo ^= RG2Blo;
            RG1Bhi &= lcon(0x3c);
            RG1L = unsignedlong(RG1W) >> bcon(0xa);
            c38_08.setA(RG1W);
            RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG2L);
            c38_08.setxB(RG1Blo);
            RG1W = DM2_QUEUE_TIMER(&c38_08);
          }
          break;
        }
        //m_35CD0:
        RG3L = signedlong(RG6W);
        RG1Blo = vo_20.peek8() + bcon(0x1e);
        RG4L = unsignedlong(RG1Blo);
        RG1L = DM2_RETRIEVE_ITEM_BONUS(unsignedlong(vw_00), RG4L, RG7l, RG3L);
        vw_24 = RG1W;
        if (RG1W != 0)
        {
          RG1L = vl_1c;
          RG4Blo = CUTX8(vw_24);
          hero->sbonus[RG1L / 4][RG1L % 4] += RG4Blo;
          hero->heroflag |= 0x2000;
          vl_14 = 1;
        }
        vo_20.inc16();
      }
    }
  }
  //m_35E04:
  if (RG6W != 0)
    party.calc_player_weight((e_hero)vql_04.peek16());
}

i16 DM2_ADD_ITEM_CHARGE(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG51w;
  c_wlreg _RG52;
  bool skip00174 = false;

  RG51w = RG1L;
  RG2L = RG4L;
  RG4L = 0;
  if (RG1W == lcon(0xffffffff))
    return RG4W;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(RG51w)));
  RG3P = RG1P;
  RG51w &= wcon(0x3c00);
  RG52L = unsignedlong(RG51w) >> bcon(0xa);
  if (RG52UW < mkuw(lcon(0x6)))
  {
    //m_F6F8:
    if (RG52W != lcon(0x5))
      return RG4W;
    RG4W = word_at(RG1P, 2);
    RG4L *= 4;
  }
  else
  {
    if (RG52UW <= mkuw(lcon(0x6)))
    {
      //m_F714:
      RG4W = word_at(RG1P, 2);
      RG4L *= 8;
    }
    else
    {
      if (RG52W != lcon(0xa))
        return RG4W;
      //m_F71D:
      RG4W = word_at(RG1P, 2);
      RG4UW >>= 14;
      RG1L = 3;
      skip00174 = true;
    }
  }

  if (!skip00174)
  {
    //m_F709:
    RG4UW >>= bcon(0xc);
    RG1L = lcon(0xf);
  }

  //m_F72A:
  RG4L += RG2L;
  RG2L = signedlong(RG1W);
  RG4L = signedlong(RG4W);
  RG1L = signedlong(DM2_BETWEEN_VALUE(0, RG2W, RG4W));
  RG4L = RG1L;
  RG2L = RG1L;
  RG2Bhi ^= RG1Bhi;
  RG2Blo &= lcon(0xf);
  if (RG52UW < mkuw(lcon(0x6)))
  {
    //m_F752:
    if (RG52W != lcon(0x5))
      return RG4W;
    and8(location(RG3P + lcon(0x3)), lcon(0xffffffc3));
    RG2L <<= bcon(0xa);
  }
  else
  {
    if (RG52UW > mkuw(lcon(0x6)))
    {
      if (RG52W != lcon(0xa))
        return RG4W;
      //m_F76E:
      RG1W &= 0x3;
      and8(location(RG3P + lcon(0x3)), lcon(0x3f));
      RG1L <<= bcon(0xe);
      or16(location(RG3P + 2), RG1W);
      return RG4W;
    }
    //m_F765:
    and8(location(RG3P + lcon(0x3)), lcon(0xffffffe1));
    RG2L <<= bcon(0x9);
  }
  //m_F75F:
  or16(location(RG3P + 2), RG2W);
  return RG4W;
}

i32 DM2_GET_MAX_CHARGE(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;

  if (RG1W == lcon(0xffffffff))
    return 0;
  RG1W &= lcon(0x3c00);
  RG1L = unsignedlong(RG1W) >> bcon(0xa);
  if (mkul(RG1L) < mkul(lcon(0x6)))
    return RG1L != lcon(0x5) ? 0 : lcon(0xf);
  if (mkul(RG1L) <= mkul(lcon(0x6)))
    return lcon(0xf);
  return RG1L == lcon(0xa) ? lcon(0x3) : 0;
}

// recursive!!!
i16 DM2_QUERY_ITEM_VALUE(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  c_lreg _RG61;
  c_lreg _RG62;
  c_lreg _RG63;
  c_lreg _RG7;
  i32 vl_00;
  i8 vb_04;

  RG2L = RG1L;
  vb_04 = RG4Blo;
  if (RG1W == lcon(0xffffffff))
    return 0;
  RG4L = unsignedlong(vb_04);
  RG3L = unsignedlong(RG1W);
  RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG3L, RG4L);
  RG5l = unsignedlong(RG1W);
  if (vb_04 == 1)
  {
    RG4L = lcon(0x34);
    RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG3L, RG4L);
    RG61L = unsignedlong(RG1W);
    if (RG61L > 0)
    {
      RG4L = 0;
      RG1W = signedlong(CUTX16(DM2_ADD_ITEM_CHARGE(RG3L, RG4L))) * RG61L;
      RG5l += signedlong(RG1W);
    }
  }
  //m_F7F2:
  if (vb_04 == 2)
  {
    RG3L = unsignedlong(RG2W);
    RG4L = lcon(0x35);
    RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG3L, RG4L);
    RG62L = unsignedlong(RG1W);
    if (RG62L > 0)
    {
      RG4L = 0;
      RG1W = signedlong(CUTX16(DM2_ADD_ITEM_CHARGE(RG3L, RG4L))) * RG62L;
      RG5l += signedlong(RG1W);
    }
    //m_F822:
    RG1L = RG2L;
    RG1Blo ^= RG2Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    if (RG1L == lcon(0x8))
    {
      if (RG5l > 1)
      {
        RG5l >>= bcon(0x1);
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
        RG1W = word_at(RG1P, 2);
        RG1W = unsignedword(RG1Blo);
        RG1L = unsignedlong(RG1W) * RG5l;
        RG3L = lcon(0xff);
        RG1L /= RG3L;
        RG5l += RG1L;
      }
    }
  }
  //m_F860:
  RG1L = RG2L;
  RG1Blo ^= RG2Blo;
  RG1Bhi &= lcon(0x3c);
  RG1L = unsignedlong(RG1W) >> bcon(0xa);
  if (RG1L != lcon(0x9))
    return RG5l;
  RG4L = unsignedlong(RG2W);
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
  RG2P = RG1P;
  if (jnz_test8(location(RG1P + lcon(0x4)), lcon(0x6)))
    return RG5l;
  vl_00 = DM2_IS_CONTAINER_MONEYBOX(RG4L);
  RG2W = word_at(RG2P, 2);
  RG63L = 0;
  //m_F8A0:
  for (;;)
  {
    bool skip00176 = false;
    if (RG2W == lcon(0xfffffffe))
    {
      //m_F91A:
      if (vl_00 == 0)
        return RG5l;
      if (vb_04 != 1)
        return RG5l + RG63L;
      RG4L = RG63L + lcon(0x4);
      RG2L = 5;
      RG1L = RG4L;
      RG4L = RG1L % RG2L;
      RG1L /= RG2L;
      return RG5l + RG1L;
    }
    if (vl_00 != 0)
    {
      RG1L = RG2L;
      RG1Blo ^= RG2Blo;
      RG1Bhi &= lcon(0x3c);
      RG1L = unsignedlong(RG1W) >> bcon(0xa);
      if (RG1L == lcon(0xa))
      {
        RG3L = unsignedlong(RG2W);
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
        RG1W = word_at(RG1P, 2);
        RG1L *= 4;
        RG1UW >>= bcon(0xa);
        RG7L = unsignedlong(RG1W);
        RG4L = unsignedlong(vb_04);
        RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG3L, RG4L) & lcon(0xffff);
        RG4L = RG7L + 1;
        RG1L *= RG4L;
        RG63L += RG1L;
        skip00176 = true;
      }
    }

    if (!skip00176)
    {
      //m_F8FA:
      RG4L = unsignedlong(vb_04);
      RG5l += unsignedlong(DM2_QUERY_ITEM_VALUE(unsignedlong(RG2W), RG4L));
    }

    //m_F90C:
    RG2L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG2UW)));
  }
  FOREND
}

i16 DM2_QUERY_ITEM_WEIGHT(i16 eaxw)
{
  return DM2_QUERY_ITEM_VALUE(unsignedlong(eaxw), 1);
}

// was SKW_2405_005e
t_text* DM2_GET_ITEM_NAME(i16 eaxw)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  i32 RG5l;

  RG2Blo = RG3Blo = DM2_QUERY_CLS1_FROM_RECORD(unsignedlong(eaxw));
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(unsignedlong(eaxw));
  RG2Bhi = RG1Blo;
  ddat.v1e098c = -1;
  if (RG2Blo == lcon(0x15) && RG1Blo == 0)
  {
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(eaxw));
    RG1W = word_at(RG1P, 2);
    RG1UW >>= 14;
    if (RG1W >= 0)
    {
      RG5l = signedlong(RG1W);
      if (RG5l < unsignedlong(party.heros_in_party))
        ddat.v1e098c = RG1W;
    }
  }
  return DM2_QUERY_GDAT_ITEM_NAME(unsignedlong(RG3Blo), unsignedlong(RG2Bhi));
}

i32 DM2_IS_MISCITEM_DRINK_WATER(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG3L = RG1L;
  RG2L = unsignedlong(RG1W);
  RG4L = 0;
  RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG2L, RG4L);
  RG1W &= 0x1;
  RG1L = unsignedlong(RG1W);
  if (RG1L == 0)
    return 0;
  RG4L = 0;
  RG1W = DM2_ADD_ITEM_CHARGE(RG2L, 0);
  if (RG1W == 0)
    return 0;
  DM2_ADD_ITEM_CHARGE(RG2L, lcon(0xffffffff));
  if (RG3W == ddat.savegamewpc.w_00)
    DM2_TAKE_OBJECT(RG2L, 0);
  return 1;
}

// stacksize was 0x14
void DM2_MOVE_ITEM_TO(i32 eaxl, unk* xedxp, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1, i32 argl2, i32 argl3, i32 argl4)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4P = xedxp;
  c_wlreg _RG5;
  i8* RG6p;
  i16 vw_00;
  i16 parw00;
  i16 parw01;
  bool skip01006 = false;

  RG5L = RG1L;
  RG6p = RG4P;
  RG4L = unsignedlong(RG1W);
  RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG4L);
  if (RG1Blo != lcon(0x15))
    skip01006 = true;
  else
  {
    RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L);
    if (RG1Blo != 0)
      skip01006 = true;
  }

  if (skip01006)
  {
    bool skip01008 = false;
    //m_47E24:
    RG1W = word_at(RG6p, lcon(0x4));
    RG1L <<= 11;
    RG1UW >>= 14;
    if (RG1UW < mkuw(lcon(0x1)))
    {
      //m_47E3F:
      if (RG1W != 0)
        skip01008 = true;
      else
        RG1L = DM2_GET_MAX_CHARGE(unsignedlong(RG5W));
    }
    else
    {
      if (RG1UW <= mkuw(lcon(0x1)))
      {
        //m_47E50:
        vw_00 = wcon(0xfff1);
        skip01008 = true;
      }
      else
      {
        if (RG1W == 2)
          //m_47E58:
          RG1W = DM2_RAND16(CUTX16(DM2_GET_MAX_CHARGE(unsignedlong(RG5W))) + 1);
        else
          skip01008 = true;
      }
    }

    if (!skip01008)
      //m_47E6D:
      vw_00 = RG1W;

    //m_47E71:
    DM2_ADD_ITEM_CHARGE(unsignedlong(RG5W), signedlong(vw_00));
  }
  //m_47E81:
  RG1L = (dm2_GET_TILE_VALUE(signedlong(argw0), signedlong(argw1)) & lcon(0xff)) >> 5;
  if (RG1Blo != 0)
  {
    RG1W = DM2_RAND16(4);
    argl2 = RG1L;
  }
  if (RG2W >= 0 && RG3W >= 0)
  {
    RG1Blo = argl3 == 0 ? 1 : 0;
    RG1L = unsignedlong(RG1Blo) - 1;
    RG1L = signedlong(RG1W);
    put16(parw00, RG1W);
    RG4L = signedlong(RG2W);
    RG1L = unsignedlong(RG5W);
    DM2_MOVE_RECORD_TO(RG1L, RG4L, signedlong(RG3W), lcon(0xffffffff), parw00);
  }
  //m_47EDB:
  RG1L = signedlong(argw1);
  put16(parw01, RG1W);
  RG3L = signedlong(argw0);
  RG1Blo = argl4 == 0 ? 1 : 0;
  RG1L = unsignedlong(RG1Blo) - 1;
  RG2L = signedlong(RG1W);
  RG5L &= lcon(0xffff3fff);
  RG1L = (argl2 << bcon(0xe) | RG5L) & lcon(0xffff);
  DM2_MOVE_RECORD_TO(RG1L, lcon(0xffffffff), RG2L, RG3L, parw01);
}

// recursive!
// stacksize was 0x44
void DM2_ACTIVATE_ITEM_TELEPORT(c_tim* eaxtimp, unk* xedxp, i32 ebxl, i32 ecxl, i32 argl0, unk* xargp1, i32 argl2, i32 argl3)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4P = xedxp;
  c_wlreg _RG5;
  i8* RG6p;
  c_tim* xtimp_00 = eaxtimp;
  unk* xp_04;
  i32 vl_08;
  i32 vl_0c;
  i16 vw_10;
  i16 vw_14;
  i16 vw_18;
  i16 vw_1c;
  c_ql32 vql_20;
  c_ol32 vol_24; // poke 32 peek 16 dec32 inc32
  i16 vw_28;
  i32 vl_2c;
  i16 vw_30;
  i16 parw00;
  i16 parw01;
  unk* xparp06;
  bool skip01010 = false;

  RG6p = RG4P;
  vql_20.poke32(RG2L);
  vl_0c = RG3L;
  if (argl2 == 0)
  {
    //m_47F78:
    RG1L = 0;
    RG1Blo = xtimp_00->getxA();
    vw_14 = RG1W;
    RG1L = unsignedlong(xtimp_00->getyA());
    vw_10 = RG1W;
    RG1L = unsignedlong(xtimp_00->getxB());
    vw_28 = RG1W;
    RG1W = word_at(RG6p, lcon(0x6));
    RG1L <<= bcon(0x5);
    _RG1.ushiftr11();
    vw_18 = RG1W;
    RG1W = word_at(RG6p, lcon(0x6));
    RG4L = RG1L;
    _RG4.ushiftr11();
    vw_30 = RG4W;
    RG1L <<= bcon(0xa);
    RG1UW >>= 14;
  }
  else
  {
    RG1W = word_at(RG6p, lcon(0x6));
    RG1L <<= bcon(0x5);
    _RG1.ushiftr11();
    vw_14 = RG1W;
    RG1W = word_at(RG6p, lcon(0x6));
    RG4L = RG1L;
    _RG4.ushiftr11();
    vw_10 = RG4W;
    RG1L <<= bcon(0xa);
    RG1UW >>= 14;
    vw_28 = RG1W;
    RG1L = 0;
    RG1Blo = xtimp_00->getxA();
    vw_18 = RG1W;
    RG1L = unsignedlong(xtimp_00->getyA());
    vw_30 = RG1W;
    RG1L = unsignedlong(xtimp_00->getxB());
  }
  //m_47FBA:
  vw_1c = RG1W;
  if (jnz_test8(location(RG6p + lcon(0x4)), lcon(0x20)))
    skip01010 = true;
  else
  {
    if (xtimp_00->getyB() != 0)
      skip01010 = true;
  }

  if (skip01010)
  {
    //m_47FCD:
    if (jz_test8(location(RG6p + lcon(0x4)), lcon(0x20)))
      return;
    if (xtimp_00->getyB() != 1)
      return;
  }
  //m_47FE4:
  if (vql_20.peek16() == lcon(0xffffffff))
  {
    RG2L = signedlong(vw_10);
    RG3L = signedlong(vw_14);
    RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(RG3W, RG2W));
    vql_20.poke32(RG1L);
    RG4L = RG2L;
    RG1L = (dm2_GET_TILE_VALUE(RG3L, RG4L) & lcon(0xff)) >> 5;
    if (RG1Blo != 0)
      vl_0c = 1;
  }
  RG5L = vql_20.peek32();
  vol_24.poke32(0);
  //m_48027:
  for (;;)
  {
    if (RG5W == lcon(0xfffffffe))
    {
      //m_48072:
      RG5L = vql_20.peek32();
      if (argl0 != 0)
      {
        RG2L = lcon(0x400);
        RG4L = 0;
        RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x40));
        RG2P = RG1P;
        xp_04 = UPCAST(unk, RG1P);
        RG1W = word_at(RG6p, lcon(0x4));
        RG1L <<= bcon(0x5);
        RG1UW >>= bcon(0xc);
        RG4L = unsignedlong(RG1W);
        RG1L = 3 * RG4L + lcon(0x10);
        RG4L = signedlong(RG1W);
        RG1W = word_at(RG6p, 2);
        RG1UW >>= bcon(0x7);
        RG1L = unsignedlong(RG1Blo);
        RG3L = 0;
        DM2_QUERY_CREATURES_ITEM_MASK(RG1L, RG4L, RG2P, RG3L);
      }
      break;
    }
    RG1L = RG5L;
    RG1W &= lcon(0x3c00);
    RG2L = unsignedlong(RG1W) >> bcon(0xa);
    RG1L = signedlong(RG2W);
    if (RG1L >= lcon(0x5) && RG1L <= lcon(0xa))
    {
      bool skip01012 = false;
      if (vl_0c != 0)
        skip01012 = true;
      else
      {
        RG4L = unsignedlong(RG5W) >> bcon(0xe);
        RG1L = signedlong(vw_28);
        if (RG4L == RG1L)
          skip01012 = true;
      }

      if (skip01012)
        //m_48061:
        vol_24.inc32();
    }
    //m_48064:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG5UW)));
    RG5L = RG1L;
  }
  //m_480C2:
  for (;;)
  {
    bool skip01016 = false;
    bool skip01017 = false;
    if (RG5W == lcon(0xfffffffe))
      break;
    RG1L = RG5L;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    RG2L = RG1L;
    RG4L = unsignedlong(RG5W);
    vl_2c = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
    if (vol_24.peek16() <= wcon(0x0))
      skip01017 = true;
    else
    {
      RG1L = signedlong(RG2W);
      if (RG1L < lcon(0x5) || RG1L > lcon(0xa))
        skip01017 = true;
      else
      {
        bool skip01014 = false;
        if (vl_0c != 0)
          skip01014 = true;
        else
        {
          RG4L >>= 14;
          RG1L = signedlong(vw_28);
          if (RG4L == RG1L)
            skip01014 = true;
        }

        if (skip01014)
        {
          bool skip01015 = false;
          //m_48123:
          RG4L = unsignedlong(RG5W);
          vol_24.dec32();
          RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG4W);
          RG2L = RG1L;
          if (argl0 == 0)
          {
            //m_4815D:
            RG2W = word_at(RG6p, 2);
            RG3L = RG2L;
            RG3UW >>= bcon(0x7);
            if (RG1W != RG3W)
            {
              //m_481E3:
              RG2Blo &= lcon(0xffffff80);
              RG1Blo = RG2W == wcon(0xff80) ? 1 : 0;
              skip01015 = true;
            }
            else
            {
              if (jz_test8(location(RG6p + lcon(0x4)), lcon(0x4)))
              {
                RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG4L);
                if (RG1Blo == lcon(0x15))
                {
                  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L);
                  if (RG1Blo == 0)
                  {
                    RG5L = vl_2c;
                    RG3L = signedlong(vw_10);
                    if (xargp1 == NULL)
                      //m_481A2:
                      RG1W = vw_14;
                    else
                      RG1W = -1;
                    //m_481A8:
                    RG2L = signedlong(RG1W);
                    RG1L = unsignedlong(RG4W);
                    vl_08 = RG1L;
                    DM2_CUT_RECORD_FROM(mk_record(RG1L), UPCAST(t_record, DOWNCAST(unk, xargp1)), RG2W, RG3W);
                    DM2_DEALLOC_RECORD(vl_08);
                    RG1W = DM2_ADD_ITEM_CHARGE(vl_08, 0) & lcon(0xffff);
                    DM2_BRING_CHAMPION_TO_LIFE(RG1W);
                    continue;
                  }
                }
              }
              //m_481DC:
              RG1L = 1;
              skip01016 = true;
            }
          }
          else
          {
            RG1P = DOWNCAST(unk, xp_04) + signedlong(RG1W) / 8;
            RG4L = unsignedlong(byte_at(RG1P));
            RG3Blo = RG2Blo & lcon(0x7);
            RG1Blo = lcon(0x1) << RG3Blo;
            RG1L = unsignedlong(RG1Blo);
            RG1Blo = (RG1L & RG4L) != 0 ? 1 : 0;
            skip01015 = true;
          }

          if (skip01015)
          {
            //m_481ED:
            RG1L = unsignedlong(RG1Blo);
            skip01016 = true;
          }
        }
        else
          skip01017 = true;
      }
    }

    if (skip01016)
    {
      //m_481F2:
      if (RG1L != 0)
      {
        RG1W = word_at(RG6p, lcon(0x4));
        RG1L <<= bcon(0xd);
        RG1UW >>= bcon(0xf);
        RG1L = unsignedlong(RG1W);
        i32 parl04 = RG1L;
        RG1Blo = xargp1 != NULL ? 1 : 0;
        RG1L = unsignedlong(RG1Blo);
        i32 parl03 = RG1L;
        i32 parl02 = signedlong(vw_1c);
        RG1L = signedlong(vw_30);
        put16(parw01, RG1W);
        RG1L = signedlong(vw_18);
        put16(parw00, RG1W);
        RG3L = signedlong(vw_10);
        RG2L = signedlong(vw_14);
        RG1L = unsignedlong(RG5W);
        RG4P = RG6p;
        DM2_MOVE_ITEM_TO(RG1L, RG4P, RG2L, RG3L, parw00, parw01, parl02, parl03, parl04);
        if (argl3 != 0)
          break;
      }
    }

    if (skip01017)
    {
      //m_4824D:
      if (argl0 == 0)
      {
        RG1L = signedlong(RG2W);
        if (RG1L == lcon(0x4))
        {
          RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG5UW));
          i32 parl08 = argl3;
          i32 parl07 = argl2;
          xparp06 = RG1P + 2;
          i32 parl05 = argl0;
          RG2L = unsignedlong(word_at(RG1P, 2));
          RG3L = vl_0c;
          RG4P = RG6p;
          DM2_ACTIVATE_ITEM_TELEPORT(xtimp_00, RG4P, RG2L, RG3L, parl05, xparp06, parl07, parl08);
        }
      }
    }

    //m_48288:
    RG5L = vl_2c;
  }
  //m_48290:
  if (argl0 != 0)
    dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0x40));
}

// was SKW_24a5_0990
void DM2__CHECK_ROOM_FOR_CONTAINER(i32 eaxl, unk* xedxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4P = xedxp;
  i8* RG51p;
  i32 RG52l;
  i16 RG6w;
  i32 vl_00;
  i16 vw_04;

  vl_00 = RG1L;
  RG51p = RG4P;
  if (RG1W == lcon(0xffffffff))
    return;
  if (RG1W == ddat.v1d6700)
    return;
  RG3L = 0;
  vw_04 = RG3W;
  RG6w = wcon(0x0);
  //m_29452:
  for (;;)
  {
    if (word_at(RG51p + 2) != lcon(0xfffffffe))
    {
      vw_04++;
      RG1L = signedlong(vw_04);
      if (RG1L <= lcon(0x8))
      {
        RG1L = signedlong(RG6w);
        RG4W = word_at(RG51p, 2);
        party.hand_container[RG1L] = RG4W;
        RG6w++;
        RG4P = RG51p + 2;
        RG1L = unsignedlong(word_at(RG51p, 2));
        RG3L = 0;
        DM2_CUT_RECORD_FROM(mk_record(RG1L), UPCAST(t_record, RG4P), -1, RG3W);
        continue;
      }
    }
    break;
  }
  //m_29491:
  RG2L = lcon(0xffff);
  //m_29496:
  for (;;)
  {
    RG52l = signedlong(RG6w);
    if (RG52l >= lcon(0x8))
      break;
    party.hand_container[RG52l] = RG2W;
    RG6w++;
  }
  //m_294A9:
  RG1L = vl_00;
  ddat.v1d6700 = RG1W;
}

i16 DM2_F958(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;

  RG1W = DM2_QUERY_ITEM_VALUE(unsignedlong(RG1W), 2);
  return RG1W <= -1 ? RG1W : -1;
}

// stacksize was 0x3c
void DM2_SHOOT_ITEM(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i8 argb1, i8 argb2, i8 argb3)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i32 RG51l;
  i8* RG52p;
  i32 RG53l;
  c_wlreg _RG6;
  c_wlreg _RG7;
  c_tim c38_00;
  unk* xp_0c;
  i32 vl_10;
  i32 vl_14;
  i32 vl_18;
  i32 vl_1c;
  i16 vw_20;
  i32 vl_24;
  i16 vw_28;
  i16 parw00;
  i16 parw01;
  i16 parw02;

  RG6L = RG1L;
  vw_28 = RG4W;
  vw_20 = RG2W;
  RG1L = DM2_ALLOC_NEW_RECORD(14);
  vl_24 = RG1L;
  RG3L <<= bcon(0xe);
  vl_18 = RG3L;
  if (RG1W == lcon(0xffffffff))
  {
    RG1L = RG6L;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    if (RG1L == lcon(0xf))
      return;
    RG1L = signedlong(vw_20);
    put16(parw00, RG1W);
    RG3L = signedlong(vw_28);
    RG6L &= lcon(0xffff3fff);
    RG6L |= vl_18;
    DM2_MOVE_RECORD_TO(unsignedlong(RG6W), lcon(0xffffffff), 0, RG3L, parw00);
    return;
  }
  //m_657C:
  RG1L = signedlong(vw_20);
  vl_10 = RG1L;
  put16(parw02, RG1W);
  RG1L = signedlong(vw_28);
  vl_14 = RG1L;
  put16(parw01, RG1W);
  RG51l = unsignedlong(RG6W);
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG51l);
  RG4L = unsignedlong(RG1Blo);
  RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG51l) & lcon(0xff);
  DM2_QUEUE_NOISE_GEN2(RG1Blo, RG4Blo, lcon(0x86), lcon(0xfe), parw01, parw02, lcon(0x1), lcon(0x6e), lcon(0x80));
  RG1L = vl_24;
  RG1Bhi &= lcon(0x3f);
  RG4L = vl_18 | RG1L;
  vl_1c = RG4L;
  RG7L = unsignedlong(RG4W);
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG7UW));
  RG52p = RG1P;
  xp_0c = UPCAST(unk, RG1P);
  mov16(location(RG1P + 2), RG6W);
  RG1L = signedlong(DM2_MIN(unsignedword(argb1), wcon(0xff)));
  mov8(location(RG52p + lcon(0x4)), RG1Blo);
  RG1Blo = argb2;
  mov8(location(RG52p + lcon(0x5)), RG1Blo);
  DM2_APPEND_RECORD_TO(mk_record(RG7L), NULL, CUTX16(vl_14), CUTX16(vl_10));
  RG1L = unsignedlong(word_at(RG52p, 2));
  if (RG1L == lcon(0xff80))
  {
    RG1W = ddat.v1d3248;
    if (RG1W == ddat.v1e0266)
      ddat.v1e0390.b_00 |= 1;
  }
  c38_00.setmticks(ddat.v1d3248, timdat.gametick + 1);
  RG1Blo = (ddat.v1e01a8 != wcon(0x0) ? 1 : 0) + lcon(0x1d);
  c38_00.settype(RG1Blo);
  c38_00.setactor(0);
  RG1L = vl_1c;
  c38_00.setA(RG1W);
  RG1L = unsignedlong(vw_28);
  RG1W &= lcon(0x1f);
  c38_00.mask_wvalueB(0xffe0, 0);
  RG53l = c38_00.getBlong() | RG1L;
  RG1L = unsignedlong(vw_20);
  RG1W &= lcon(0x1f);
  RG1L <<= bcon(0x5);
  c38_00.setB(CUTX16((RG53l & lcon(0xfffffc1f)) | RG1L));
  RG1Blo = argb3;
  RG1W &= lcon(0xf);
  c38_00.mask_wvalueB(0xfff, RG1W << 12);
  RG1L = argl0;
  RG1W &= 0x3;
  c38_00.mask_wvalueB(0xf3ff, RG1W << 10);
  mov16(location(DOWNCAST(unk, xp_0c) + lcon(0x6)), DM2_QUEUE_TIMER(&c38_00));
}

// was SKW_24a5_0930
i32 DM2_PUT_OBJECT_INTO_CONTAINER(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  i8* RG61p;

  RG4W = ddat.v1d6700;
  if (RG4W == lcon(0xffffffff))
    return RG1L;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
  RG61p = RG1P;
  ddat.v1d6700 = lcon(0xffffffff);
  RG5w = wcon(0x0);
  //m_2941F:
  for (;;)
  {
    RG1L = signedlong(RG5w);
    if (RG1L >= lcon(0x8))
      return RG1L;
    //m_293F2:
    RG1L = 2 * RG1L;
    RG2W = party.hand_container[RG1L / 2];
    if (RG2W != lcon(0xffffffff))
    {
      party.hand_container[RG1L / 2] = lcon(0xffffffff);
      RG4P = RG61p + 2;
      RG1L = unsignedlong(RG2W);
      RG3L = 0;
      RG2L = lcon(0xffffffff);
      DM2_APPEND_RECORD_TO(mk_record(RG1L), UPCAST(t_record, RG4P), RG2W, RG3W);
    }
    RG5w++;
  }
  FOREND
}

void DM2_TAKE_OBJECT(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;

  RG2L = RG1L;
  RG5l = RG4L;
  if (RG1W == lcon(0xffffffff))
    return;
  RG2Bhi &= lcon(0x3f);
  RG3L = unsignedlong(RG2W);
  RG4L = 0;
  RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG3L, RG4L);
  ddat.savegamewpc.w_18 = RG1W;
  RG1W = DM2_QUERY_ITEM_WEIGHT(RG3W);
  ddat.savegamewpc.weight = RG1W;
  ddat.savegamewpc.w_00 = RG2W;
  DM2_DRAW_ITEM_IN_HAND(&ddat.savegamewpc);
  DM2_HIDE_MOUSE();
  DM2_DISPLAY_TAKEN_ITEM_NAME(RG3W);
  if (RG5l == 0)
    //m_35F01:
    DM2_events_443c_0434();
  else
    ddat.v1e0254 = 1;
  //m_35F06:
  DM2_SHOW_MOUSE();
  RG4L = unsignedlong(RG2W);
  DM2_PROCESS_ITEM_BONUS(signedlong(eventqueue.event_heroidx), RG4L, lcon(0xffffffff), lcon(0x1));
  DM2_moverec_3CE7D(unsignedlong(ddat.v1e0270), unsignedlong(ddat.v1e0272), lcon(0xffff), lcon(0x1), lcon(0x1), 0);
}
