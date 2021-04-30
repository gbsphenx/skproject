#include "dm2data.h"
#include "c_1c9a.h"
#include "c_timer.h"
#include "c_random.h"
#include "c_sfx.h"
#include "c_map.h"
#include "c_creature.h"
#include "c_hero.h"
#include "c_loadlevel.h"
#include "util.h"
#include "startend.h"
#include "c_item.h"
#include "c_gfx_str.h"
#include "c_querydb.h"
#include "c_eventqueue.h"
#include "c_tim_proc.h"
#include "c_move.h"
#include "c_moverec.h"

#include "emu.h"
#include "regs.h"

// was SKW_2fcf_0cdf
// stacksize was 0x24
static i32 DM2_TRY_PUSH_OBJECT_TO(i32 eaxl, i32 edxl, i32 ebxl, i16* ecxpw, i16* argpw0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3P = DOWNCAST(i16, ecxpw);
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_wlreg _RG6;
  i8* RG7p;
  unk* xp_00;
  i32 vl_04;
  i16 vw_08;
  i32 vl_0c;
  i32 vl_10;

  RG6L = RG1L;
  vl_10 = RG4L;
  vl_04 = RG2L;
  RG7p = RG3P;
  RG1L = signedlong(DM2_RANDDIR());
  if (RG6W != lcon(0xffffffff))
  {
    //m_3B0B1:
    vl_0c = 0;
    RG1L = 4 * signedlong(RG1W);
    RG4P = DOWNCAST(i8, table1d6a64);
  }
  else
  {
    RG4W = ddat.v1e0258;
    vl_0c = RG4L;
    RG1L = signedlong(RG1W) << 2;
    RG4P = DOWNCAST(i8, table1d6a54);
  }
  //m_3B0C0:
  RG4P += RG1L; // 2nd dimension
  xp_00 = UPCAST(unk, RG4P);
  RG3L = 0;
  //m_3B1A0:
  for (;;)
  {
    RG1L = signedlong(RG3W);
    if (RG1L > lcon(0x3))
      return 0;
    //m_3B0CC:
    RG1P = DOWNCAST(unk, xp_00) + RG1L;
    RG1W = signedword(byte_at(RG1P));
    RG2L = vl_0c;
    RG4L = RG1L + RG2L;
    RG4W &= 0x3;
    RG1L = signedlong(RG4W);
    RG2L = vl_10;
    RG2W += table1d27fc[RG1L];
    RG5L = vl_04;
    RG5W += table1d2804[RG1L];
    RG4L = signedlong(RG5W);
    RG1L = dm2_GET_TILE_VALUE(signedlong(RG2W), RG4L);
    RG4L = unsignedlong(RG1Blo);
    RG1L = DM2_IS_TILE_BLOCKED(unsignedlong(RG1Blo));
    if (RG1L == 0)
    {
      RG1L = signedlong(RG4W) >> 5;
      RG4L = unsignedlong(RG1Blo);
      RG1L = unsignedlong(RG1Blo);
      if (RG1L != lcon(0x3))
      {
        bool skip00814 = false;
        RG1L = RG6L;
        RG1W &= lcon(0x3c00);
        vw_08 = RG1W;
        RG1L = unsignedlong(vw_08) >> bcon(0xa);
        if (RG1L != lcon(0x4))
          skip00814 = true;
        else
        {
          RG1L = signedlong(RG4W);
          if (RG1L != lcon(0x5) && RG1L != 2)
            skip00814 = true;
        }

        if (skip00814)
        {
          bool skip00815 = false;
          //m_3B15B:
          RG4L = unsignedlong(RG5W);
          RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG2W), RG4L);
          RG4L = RG1L;
          if (RG1W == lcon(0xffffffff))
            skip00815 = true;
          else
          {
            RG1Blo = RG6W != wcon(0xffff) ? 1 : 0;
            RG1L = unsignedlong(RG1Blo);
            if (RG1L == 0)
            {
              RG1W = RG4W; // TODO: gets extended w/o conversion now
              RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(RG1L);
              if ((RG1Bhi & bcon(0x80)) != 0)
                skip00815 = true;
            }
          }

          if (skip00815)
          {
            //m_3B18D:
            mov16(location(RG7p + 0), RG2W);
            mov16(location(DOWNCAST(i16, argpw0)), RG5W);
            return lcon(0x1);
          }
        }
      }
    }
    //m_3B19F:
    RG3L++;
  }
  FOREND
}

// belongs to DM2_MOVE_RECORD_TO
static void DM2_2fcf_0234(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i32 RG51l;
  i32 RG52l;
  c_lreg _RG61;
  c_wlreg _RG62;
  c_ql32 vql_00;
  c_ql32 vql_04;
  c_ql32 vql_08;
  c_ql32 vql_0c;
  i8 vba_10[0x4];
  i8 vba_14[0x4];
  i32 vl_18;
  i32 vl_1c;
  i16 vw_20;
  i16 vw_24;
  c_o32 vo_28; // poke 16 peek 8 16 inc16
  bool flag = false;

  vql_0c.poke32(RG1L);
  vql_08.poke32(RG4L);
  vql_04.poke32(RG2L);
  vql_00.poke32(RG3L);
  DM2_ZERO_MEMORY(ADRESSOF(i8, vba_10), lcon(0x4));
  RG4L = 0;
  vo_28.poke16(RG4W);
  //m_3A1ED:
  for (;;)
  {
    RG4L = unsignedlong(vo_28.peek16());
    if (RG4L >= lcon(0x4))
    {
      if (vql_04.peek16() >= wcon(0x0))
      {
        RG61L = signedlong(vql_00.peek16());
        RG51l = signedlong(vql_04.peek16());
        vl_18 = signedlong(vql_08.peek16());
        RG1L = signedlong(vql_0c.peek16());
        vl_1c = RG1L;
        RG3L = RG61L;
        RG2L = RG51l;
        RG1L = signedlong(DM2_CALC_SQUARE_DISTANCE(RG1W, CUTX16(vl_18), RG2W, RG3W));
        if (RG1L == lcon(0x1))
        {
          RG3L = RG61L;
          RG2L = RG51l;
          RG1L = signedlong(DM2_CALC_VECTOR_DIR(CUTX16(vl_1c), CUTX16(vl_18), RG2W, RG3W));
          RG2L = RG1L;
          vo_28.poke16(RG1W);
          RG3L = RG1L + 1;
          RG3W &= 0x3;
          DM2_ZERO_MEMORY(ADRESSOF(i8, vba_14), lcon(0x4));
          RG4L = unsignedlong(RG2W);
          RG1L = RG4L + lcon(0x3) & lcon(0x3);
          RG4Blo = vba_10[RG4L];
          vba_14[RG1L] = RG4Blo;
          if (RG4Blo != 0)
            flag = true;
          RG4L = signedlong(RG3W);
          RG1L = RG4L + lcon(0x1) & lcon(0x3);
          RG4Blo = vba_10[RG4L];
          vba_14[RG1L] = RG4Blo;
          if (RG4Blo != 0)
            flag = true;
          RG1L = unsignedlong(vo_28.peek16());
          if (vba_10[RG1L] == 0)
          {
            RG4L = RG1L + lcon(0x3) & lcon(0x3);
            RG4Blo = vba_10[RG4L];
            vba_10[RG1L] = RG4Blo;
          }
          RG1L = signedlong(RG3W);
          if (vba_10[RG1L] == 0)
          {
            RG4L = RG1L + lcon(0x1) & lcon(0x3);
            RG4Blo = vba_10[RG4L];
            vba_10[RG1L] = RG4Blo;
          }
        }
      }
      break;
    }
    //m_3A1D2:
    RG1W = DM2_GET_PLAYER_AT_POSITION(RG4W);
    if (RG1W >= 0)
    {
      RG1Blo = vo_28.peek8() + 1;
      vba_10[RG4L] = RG1Blo;
    }
    vo_28.inc16();
  }
  //m_3A2D8:
  RG1L = vql_0c.peek32();
  vw_20 = RG1W;
  RG1L = vql_08.peek32();
  vw_24 = RG1W;
  //m_3A2EA:
  for (;;)
  {
    RG4L = signedlong(vw_24);
    RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(vw_20, RG4W));
    //m_3A2FD:
    for (;;)
    {
      RG62L = RG1L;
      if (RG62W == lcon(0xfffffffe))
      {
        //m_3A394:
        if (!flag)
          return;
        RG1L = vql_04.peek32();
        vw_20 = RG1W;
        RG4L = unsignedlong(RG1W) << bcon(0x8);
        RG1L = signedlong(vql_0c.peek16());
        RG4L += lcon(0x100);
        RG1L |= RG4L;
        vql_0c.poke16(RG1W);
        RG1L = vql_00.peek32();
        vw_24 = RG1W;
        RG4L = unsignedlong(RG1W) << bcon(0x8);
        RG1L = signedlong(vql_08.peek16()) | RG4L;
        vql_08.poke16(RG1W);
        RG2L = 4;
        DM2_COPY_MEMORY(ADRESSOF(i8, vba_10), ADRESSOF(i8, vba_14), RG2L);
        flag = false;
        break;
      }
      RG1L = RG62L;
      RG1W &= lcon(0x3c00);
      RG1L = unsignedlong(RG1W) >> bcon(0xa);
      if (RG1L == lcon(0xe))
      {
        RG52l = unsignedlong(RG62W);
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(CUTX16(RG52l))));
        if (!timdat.timerarray[word_at(RG1P, lcon(0x6))].is_type(0x1d))
        {
          RG1L = RG52l >> bcon(0xe);
          RG1Blo = vba_10[RG1L];
          RG1W = unsignedword(RG1Blo);
          vo_28.poke16(RG1W);
          if (RG1W != 0)
          {
            i32 parl00 = RG52l;
            RG1L--;
            RG3L = signedlong(RG1W);
            RG2L = signedlong(vql_08.peek16());
            RG4L = signedlong(vql_0c.peek16());
            RG1L = DM2_move_075f_0af9(lcon(0xfffffffd), RG4L, RG2L, RG3L, parl00);
            if (RG1L != 0)
            {
              RG1L = RG52l;
              DM2_075f_056c(RG1L);
              break;
            }
          }
        }
      }
      //m_3A385:
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG62UW)));
    }
  }
  FOREND
}

static void DM2_SET_MINION_RECENT_OPEN_DOOR_LOCATION(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  i32 RG6l;

  RG6l = RG4L;
  RG5l = RG2L;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
  RG4W = word_at(RG1P, 2);
  //m_3A0CA:
  for (;;)
  {
    if (RG4W == lcon(0xfffffffe))
      return;
    RG1L = RG4L;
    RG1Blo ^= RG4Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    if (RG1L == lcon(0xe))
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
      RG2L = RG6l;
      RG2W &= lcon(0x1f);
      and8(location(RG1P + lcon(0x4)), lcon(0xffffffe0));
      or16(location(RG1P + lcon(0x4)), RG2W);
      RG2L = RG5l;
      RG2W &= lcon(0x1f);
      and16(location(RG1P + lcon(0x4)), lcon(0xfffffc1f));
      RG2L <<= bcon(0x5);
      or16(location(RG1P + lcon(0x4)), RG2W);
      RG2L = RG3L;
      RG2Bhi ^= RG3Bhi;
      RG2Blo &= lcon(0x3f);
      and8(location(RG1P + lcon(0x5)), lcon(0x3));
      RG2L <<= bcon(0xa);
      or16(location(RG1P + lcon(0x4)), RG2W);
      RG2L = argl0;
      RG2W &= 0x1;
      and8(location(RG1P + lcon(0x6)), lcon(0xfffffffe));
      or16(location(RG1P + lcon(0x6)), RG2W);
    }
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
    RG4L = RG1L;
  }
  FOREND
}

// belongs to DM2_MOVE_RECORD_TO
// stacksize was 0x18
static void DM2_moverec_2fcf_01c5(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_ql32 vql_00;
  c_tim c38_04;

  vql_00.poke32(RG2L);
  c38_04.setmticks(RG3W, timdat.gametick + 5);
  RG2Blo = (argl0 != 0 ? 1 : 0) + lcon(0x3c);
  c38_04.settype(RG2Blo);
  c38_04.setactor(0);
  c38_04.setxyA(RG4Blo, vql_00.peek8());
  c38_04.setB(RG1W);
  RG3L = signedlong(RG3W);
  RG2L = signedlong(vql_00.peek16());
  RG4L = signedlong(RG4W);
  RG1L = unsignedlong(RG1W);
  DM2_SET_MINION_RECENT_OPEN_DOOR_LOCATION(RG1L, RG4L, RG2L, RG3L, lcon(0x1));
  DM2_QUEUE_TIMER(&c38_04);
}

// stacksize was 0x98
// -------->  FEHLER RG64 hat einen Pfad, der nicht initialisiert wird.
// recursive
i32 DM2_MOVE_RECORD_TO(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG55w;
  i32 RG56l;
  i16 RG57w;
  i32 RG58l;
  i32 RG59l;
  c_wlreg _RG62;
  c_lreg _RG63;
  i16 RG64w = 0; // TODO: needs initialization!?
  i32 vl_00;
  i32 vl_04;
  i32 vl_08;
  i32 vl_0c;
  i32 vl_10;
  i32 vl_14;
  i32 vl_18;
  i32 vl_1c;
  i32 vl_20;
  i32 vl_24;
  i32 vl_28;
  i32 vl_2c;
  i32 vl_30;
  unk* xp_34;
  i32 vl_38;
  i32 vl_3c;
  i32 vl_40;
  i32 vl_44;
  i32 vl_48;
  i16 vw_4c;
  i16 vw_50;
  i32 vl_54 = 0; // ATTENTION! ORIGINAL BUG? WAS UNINITIALIZED!
  i16 vw_58;
  i16 vw_5c;
  i16 vw_60;
  i8 vb_64;
  i16 vw_68;
  i16 vw_6c;
  i16 vw_70;
  i16 vw_74;
  i16 vw_78;
  i16 vw_7c;
  i16 vw_80;
  i16 vw_84;
  i16 parw08;
  i16 parw12;
  i16 parw16;
  i16 parw17;
  i16 parw21;
  i16 parw25;
  i16 parw30;
  i32 parl34;
  i32 parl35;
  bool skip00823 = false;

  RG55w = RG1W;
  vw_60 = RG4W;
  vw_58 = RG2W;
  vw_68 = RG3W;
  RG4L = 0;
  ddat.v1e1020 = RG4L;
  vl_3c = RG4L;
  vl_44 = RG4L;
  vw_84 = ddat.v1d3248;
  vw_80 = ddat.v1e0258;
  if (RG55w != lcon(0xffffffff))
  {
    //m_3B235:
    RG1W = RG55w;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    vw_78 = RG1W;
    RG1L = signedlong(vw_78);
    if (RG1L == lcon(0x4))
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(RG55w)));
      xp_34 = UPCAST(unk, RG1P);
    }
    RG1L = unsignedlong(RG55w) >> bcon(0xe);
    vw_74 = RG1W;
  }
  else
  {
    ddat.v1e0390.l_00 = 3;
    vw_78 = -1;
    if (vw_60 >= wcon(0x0))
      DM2_2fcf_0234(signedlong(vw_60), signedlong(vw_58), signedlong(vw_68), signedlong(argw0));
  }
  //m_3B26B:
  if (vw_68 < wcon(0x0))
    skip00823 = true;
  else
  {
    i32 parl00 = signedlong(argw0);
    RG3L = signedlong(vw_68);
    RG2L = signedlong(vw_58);
    RG4L = signedlong(vw_60);
    RG63L = unsignedlong(RG55w);
    RG1L = DM2_move_2fcf_0434(RG63L, RG4L, RG2L, RG3L, parl00, lcon(0x1));
    vl_3c = RG1L;
    if (RG55w != wcon(0xffff))
    {
      RG4L = lcon(0xf8);
      RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG63L, RG4L);
      if (RG1W != 0)
      {
        bool skip00817 = false;
        RG4L = unsignedlong(vw_84);
        RG1L = signedlong(ddat.v1e0266);
        if (RG4L == RG1L)
          skip00817 = true;
        else
        {
          RG4L = unsignedlong(ddat.v1e102c);
          if (RG4L == RG1L)
            skip00817 = true;
        }

        if (skip00817)
          //m_3B2D9:
          ddat.v1e0390.b_00 |= 1;
      }
      //m_3B2E0:
      RG55w &= wcon(0x3fff);
      RG1W = RG55w;
      RG55w = ddat.v1e1026;
      RG55w <<= bcon(0xe);
      RG55w |= RG1W;
    }
    bool skip00820 = false;
    //m_3B2F4:
    RG1W = ddat.v1e102a;
    vw_68 = RG1W;
    RG1W = ddat.v1e1028;
    argw0 = RG1W;
    RG1W = ddat.v1e1024;
    vl_54 = RG1L;
    RG64w = ddat.v1e102c;
    RG1L = signedlong(RG64w);
    DM2_CHANGE_CURRENT_MAP_TO(RG1L);
    RG1L = signedlong(vw_78);
    if (RG1L == lcon(0x4))
    {
      RG4L = unsignedlong(RG64w);
      RG1L = unsignedlong(RG55w);
      vl_18 = RG1L;
      RG1L = DM2_IS_CREATURE_ALLOWED_ON_LEVEL(RG1L, RG4L);
      if (RG1L == 0)
      {
        DM2_DROP_CREATURE_POSSESSION(vl_18, unsignedlong(vw_68), unsignedlong(argw0), 0, lcon(0x1));
        DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_84));
        RG62W = vw_60;
        if (RG62W >= 0)
        {
          RG4L = unsignedlong(vw_58);
          RG1L = unsignedlong(RG62W);
          DM2_DELETE_CREATURE_RECORD(RG1L, RG4L, 2, lcon(0xffffffff));
        }
        return lcon(0x1);
      }
    }
    //m_3B396:
    if (RG64w != vw_84)
      skip00820 = true;
    else
    {
      RG1W = vw_68;
      if (RG1W != vw_60)
        skip00820 = true;
      else
      {
        RG1W = argw0;
        if (RG1W != vw_58)
          skip00820 = true;
        else
          RG1L = 1;
      }
    }

    if (skip00820)
      //m_3B3B8:
      RG1L = 0;
    //m_3B3BA:
    vl_44 = RG1L;
    if (RG1L == 0)
    {
      //m_3B3FD:
      RG3W = vw_84;
      if (RG64w != RG3W)
      {
        bool skip00822 = false;
        if (RG55w != wcon(0xffff))
          skip00822 = true;
        else
        {
          RG4L = signedlong(ddat.v1e0266);
          RG1W = RG3W;
          if (RG4L == RG1L)
            skip00822 = true;
          else
            DM2_LOAD_LOCALLEVEL_GRAPHICS_TABLE(signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), signedlong(vw_84));
        }

        if (skip00822)
          //m_3B439:
          DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_84));
      }
      skip00823 = true;
    }
    else
    {
      if (RG55w != wcon(0xffff))
      {
        //m_3B3E0:
        RG1W = ddat.v1e1026;
        if (RG1W != vw_74)
          skip00823 = true;
        else
        {
          RG1L = signedlong(vw_78);
          if (RG1L == lcon(0xe))
            skip00823 = true;
        }
      }
      else
      {
        RG4L = signedlong(ddat.v1e0258);
        RG1L = unsignedlong(vw_80);
        if (RG4L != RG1L)
          skip00823 = true;
      }
    }
  }

  if (skip00823)
  {
    //m_3B444:
    RG2W = vw_60;
    if (RG2W >= 0)
    {
      if (RG55w != wcon(0xffff))
      {
        //m_3B477:
        if (vl_3c == 0)
        {
          bool skip00825 = false;
          //m_3B497:
          if (vw_68 >= wcon(0x0) || argw0 >= wcon(0x0))
            //m_3B4AF:
            RG1L = 0;
          else
            RG1L = 1;
          //m_3B4B1:
          i32 parl06 = RG1L;
          RG1W = ddat.v1d3248;
          if (RG1W == ddat.v1e0282)
          {
            RG1W = vw_60;
            if (RG1W == ddat.v1e0280)
            {
              RG1W = vw_58;
              if (RG1W == ddat.v1e027e)
              {
                RG3L = 1;
                skip00825 = true;
              }
            }
          }

          if (!skip00825)
            //m_3B4E2:
            RG3L = 0;

          //m_3B4E4:
          DM2_moverec_3CE7D(unsignedlong(vw_60), unsignedlong(vw_58), unsignedlong(RG55w), RG3L, 0, parl06);
        }
        else
          DM2_CUT_RECORD_FROM(mk_record(unsignedlong(RG55w)), NULL, vw_60, vw_58);
        //m_3B4FA:
        RG1L = signedlong(vw_78);
        if (RG1L == lcon(0x4))
        {
          if (vw_68 < wcon(0xffffffff))
          {
            RG1L = 0;
            RG1Blo = byte_at(DOWNCAST(unk, xp_34), lcon(0x5));
            vw_7c = RG1W;
            if (RG1W != lcon(0xff))
              DM2_1c9a_0fcb(unsignedlong(vw_7c));
          }
        }
      }
      else
      {
        RG4L = unsignedlong(vw_58);
        RG1L = unsignedlong(RG2W);
        DM2_moverec_3CE7D(RG1L, RG4L, lcon(0xffff), vl_44, 0, 0);
      }
    }
    //m_3B529:
    RG2W = vw_68;
    if (RG2W >= 0)
    {
      bool skip00839 = false;
      if (RG55w != wcon(0xffff))
      {
        //m_3B769:
        RG4L = signedlong(vw_78);
        if (RG4L != lcon(0x4))
        {
          //m_3BB93:
          RG1L = signedlong(RG64w);
          DM2_CHANGE_CURRENT_MAP_TO(RG1L);
          if (RG4L != lcon(0xe))
          {
            bool skip00838 = false;
            //m_3BBC0:
            if (vw_60 >= wcon(0x0) || vw_58 >= wcon(0x0))
              //m_3BBD5:
              RG1L = 0;
            else
              RG1L = 1;
            //m_3BBD7:
            RG2L = RG1L;
            if (RG1L == 0 && RG64w != vw_84)
            {
              RG4L = unsignedlong(argw0);
              RG1W = vw_68; // TODO: gets extended w/o conversion now
              RG1L = DM2_GET_CREATURE_AT(RG1L, RG4L);
              if (RG1W != lcon(0xffffffff))
              {
                RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG1W));
                RG1W &= 0x1;
                RG2W = RG1W;
              }
            }
            parl35 = RG2L;
            parl34 = 1;
            RG1W = ddat.v1d3248;
            if (RG1W == ddat.v1e0282)
            {
              RG1W = vw_68;
              if (RG1W == ddat.v1e0280)
              {
                RG1W = argw0; // adj
                if (RG1W == ddat.v1e027e)
                {
                  RG3L = 1;
                  skip00838 = true;
                }
              }
            }

            if (!skip00838)
              //m_3BC40:
              RG3L = 0;

            //m_3BC42:
            RG2L = unsignedlong(RG55w);
            RG4L = unsignedlong(argw0);
            RG1L = unsignedlong(vw_68);
            skip00839 = true;
          }
          else
          {
            RG3L = signedlong(argw0);
            RG2L = signedlong(vw_68);
            RG1L = unsignedlong(RG55w);
            DM2_APPEND_RECORD_TO(mk_record(RG1L), NULL, RG2W, RG3W);
          }
        }
        else
        {
          bool skip00834 = false;
          bool skip00831 = false;
          RG1L = signedlong(RG64w);
          DM2_CHANGE_CURRENT_MAP_TO(RG1L);
          RG1L = 0;
          RG1Blo = byte_at(DOWNCAST(unk, xp_34), lcon(0x5));
          vw_7c = RG1W;
          RG4L = unsignedlong(RG64w);
          RG1L = signedlong(ddat.v1e0282);
          if (RG4L != RG1L)
            skip00831 = true;
          else
          {
            RG1L = RG2L;
            if (RG1W != ddat.v1e0280)
              skip00831 = true;
            else
            {
              RG1W = argw0;
              if (RG1W != ddat.v1e027e)
                skip00831 = true;
              else
              {
                RG1L = unsignedlong(RG55w);
                vl_20 = RG1L;
                RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(RG1L);
                if ((RG1Bhi & bcon(0x80)) != 0)
                  skip00831 = true;
                else
                {
                  vl_48 = signedlong(argw0);
                  RG1L = signedlong(vw_68);
                  vl_2c = RG1L;
                  RG1L = DM2_TRY_PUSH_OBJECT_TO(lcon(0xffff), RG1L, vl_48, &vw_4c, &vw_70);
                  if (RG1L != 0)
                  {
                    RG1L = DM2_GET_CREATURE_WEIGHT(vl_20);
                    RG4W = RG1W / 8;
                    RG1W = DM2_RAND16(10) + RG4W;
                    DM2_ATTACK_PARTY(RG1L, lcon(0x4), 2);
                    RG2L = vl_48;
                    put16(parw17, RG2W);
                    RG3L = vl_2c;
                    put16(parw16, RG3W);
                    RG3L = lcon(0xfe);
                    DM2_QUEUE_NOISE_GEN2(lcon(0x16), RG3Blo, lcon(0x8a), RG3Blo, parw16, parw17, lcon(0x1), lcon(0x64), lcon(0xc8));
                    RG1L = signedlong(vw_70);
                    put16(parw21, RG1W);
                    DM2_MOVE_RECORD_TO(lcon(0xffff), vl_2c, vl_48, signedlong(vw_4c), parw21);
                    skip00831 = true;
                  }
                }
              }
            }
          }

          if (skip00831)
          {
            bool skip00836 = false;
            //m_3B885:
            RG1L = DM2_GET_CREATURE_AT(unsignedlong(vw_68), unsignedlong(argw0));
            RG4L = RG1L;
            vw_50 = RG1W; // truncated
            if (RG1W == lcon(0xffffffff))
              skip00836 = true;
            else
            {
              bool skip00835 = false;
              RG2L = unsignedlong(RG55w);
              RG1L = DM2_GET_CREATURE_WEIGHT(RG2L);
              vw_5c = RG1W; // truncated
              vw_6c = RG1W;
              RG4L = unsignedlong(RG4W);
              RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(RG4L);
              RG3L = RG1L;
              vb_64 = RG1Blo;
              RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(RG2L);
              if ((RG1Blo & bcon(0x1)) == 0 || vw_5c <= wcon(0xfd))
              {
                bool skip00833 = false;
                //m_3B95A:
                if ((vb_64 & bcon(0x1)) == 0)
                  skip00833 = true;
                else
                {
                  RG1L = DM2_GET_CREATURE_WEIGHT(unsignedlong(vw_50)) & lcon(0xffff);
                  if (RG1L <= lcon(0xfd))
                    skip00833 = true;
                  else
                  {
                    RG2L = signedlong(argw0);
                    RG4L = signedlong(vw_68);
                    RG1L = unsignedlong(RG55w);
                    vl_0c = RG1L;
                    RG1L = DM2_TRY_PUSH_OBJECT_TO(RG1L, RG4L, RG2L, &vw_4c, &vw_70);
                    if (RG1L == 0)
                      skip00834 = true;
                    else
                    {
                      RG1L = signedlong(vw_70);
                      put16(parw25, RG1W);
                      DM2_MOVE_RECORD_TO(vl_0c, lcon(0xffffffff), 0, signedlong(vw_4c), parw25);
                    }
                  }
                }

                if (skip00833)
                {
                  //m_3B9BF:
                  RG1L = DM2_TRY_PUSH_OBJECT_TO(unsignedlong(vw_50), signedlong(vw_68), signedlong(argw0), &vw_4c, &vw_70);
                  if (RG1L != 0)
                    skip00835 = true;
                  else
                    skip00834 = true;
                }
              }
              else
              {
                bool skip00832 = false;
                if ((RG3Blo & bcon(0x1)) == 0)
                  skip00832 = true;
                else
                {
                  RG1L = DM2_GET_CREATURE_WEIGHT(RG4L) & lcon(0xffff);
                  if (RG1L <= lcon(0xfd))
                    skip00832 = true;
                }

                if (skip00832)
                {
                  //m_3B901:
                  vl_1c = signedlong(argw0);
                  vl_38 = signedlong(vw_68);
                  RG1L = DM2_TRY_PUSH_OBJECT_TO(unsignedlong(vw_50), vl_38, vl_1c, &vw_4c, &vw_70);
                  if (RG1L != 0)
                    skip00835 = true;
                  else
                  {
                    RG59l = unsignedlong(RG55w);
                    DM2_1c9a_0db0(RG59l);
                    RG3L = signedlong(RG64w);
                    DM2_moverec_2fcf_01c5(RG59l, vl_38, vl_1c, RG3L, 0);
                  }
                }
                else
                  skip00834 = true;
              }

              if (skip00835)
              {
                //m_3BA24:
                RG1L = unsignedlong(vw_6c);
                RG1L <<= bcon(0x10);
                RG1L >>= bcon(0x12);
                RG4L = RG1L;
                RG1W = DM2_RAND16(RG1W / 2 + 1);
                RG4W += RG1W;
                RG1L = unsignedlong(RG4W);
                i32 parl29 = RG1L;
                vl_00 = signedlong(argw0);
                vl_04 = signedlong(vw_68);
                RG1L = unsignedlong(vw_50);
                vl_08 = RG1L;
                DM2_ATTACK_CREATURE(RG1L, vl_04, vl_00, lcon(0x2000), 0, parl29);
                RG1L = signedlong(vw_70);
                put16(parw30, RG1W);
                DM2_MOVE_RECORD_TO(vl_08, vl_04, vl_00, signedlong(vw_4c), parw30);
                skip00836 = true;
              }
            }

            if (skip00836)
            {
              //m_3BA8D:
              RG4L = vl_3c;
              if (RG4L == 0)
              {
                //m_3BAB1:
                i32 parl32 = RG4L;
                RG2L = unsignedlong(RG55w);
                RG4W = argw0;
                DM2_moverec_3CE7D(unsignedlong(vw_68), RG4L, RG2L, 0, lcon(0x1), parl32);
              }
              else
              {
                RG3L = signedlong(argw0);
                RG2L = signedlong(vw_68);
                RG1L = unsignedlong(RG55w);
                DM2_APPEND_RECORD_TO(mk_record(RG1L), NULL, RG2W, RG3W);
              }
              //m_3BACD:
              RG3L = signedlong(RG64w);
              RG2L = signedlong(argw0);
              RG4L = signedlong(vw_68);
              RG56l = unsignedlong(RG55w);
              DM2_SET_MINION_RECENT_OPEN_DOOR_LOCATION(RG56l, RG4L, RG2L, RG3L, 0);
              if (vw_7c != lcon(0xff))
              {
                //m_3BB20:
                RG4L = signedlong(vw_7c);
                RG1L = 17 * RG4L;
                RG1W = word_at(DOWNCAST(c_creature, creatures), 2 * RG1L + 2);
                vw_7c = RG1W;
                RG4L = signedlong(vw_7c);
                if (RG4L != lcon(0xffffffff))
                {
                  timdat.timerarray[RG1L].setxyA(CUTX8(vw_68), CUTX8(argw0));
                  timdat.timerarray[RG1L].setmticks(RG64w, timdat.timerarray[RG1L].getticks());
                }
              }
              else
              {
                RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(RG56l);
                if ((RG1Blo & bcon(0x1)) == 0)
                  DM2_ALLOC_CAII_TO_CREATURE(RG56l, unsignedlong(vw_68), unsignedlong(argw0));
              }
            }
          }
          else
            skip00834 = true;

          if (skip00834)
          {
            //m_3B9E4:
            RG4W = vw_7c;
            if (RG4W != lcon(0xff))
            {
              RG1L = unsignedlong(RG4W);
              DM2_1c9a_0fcb(RG1L);
            }
            i32 parl27 = ddat.v1e1020;
            RG3L = signedlong(RG64w);
            RG2L = signedlong(argw0);
            RG4L = signedlong(vw_68);
            RG1L = unsignedlong(RG55w);
            DM2_moverec_2fcf_01c5(RG1L, RG4L, RG2L, RG3L, parl27);
            return 2;
          }
        }
      }
      else
      {
        bool skip00830 = false;
        RG1L = signedlong(RG64w);
        vl_40 = RG1L;
        DM2_CHANGE_CURRENT_MAP_TO(RG1L);
        RG2L = unsignedlong(RG64w);
        vl_30 = signedlong(argw0);
        RG1L = signedlong(vw_68);
        vl_14 = RG1L;
        DM2_move_2fcf_0b8b(RG1L, vl_30, RG2L);
        RG1L = DM2_GET_CREATURE_AT(unsignedlong(ddat.v1e0270), unsignedlong(ddat.v1e0272));
        RG57w = RG1W;
        if (RG1W == lcon(0xffffffff))
          skip00830 = true;
        else
        {
          RG4L = unsignedlong(RG1W);
          RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(RG4L);
          if ((RG1Bhi & bcon(0x80)) != 0)
            skip00830 = true;
          else
          {
            bool skip00829 = false;
            bool skip00828 = false;
            if ((RG1Blo & bcon(0x1)) == 0)
              skip00828 = true;
            else
            {
              RG1L = DM2_GET_CREATURE_WEIGHT(RG4L) & lcon(0xffff);
              if (RG1L <= lcon(0xfd))
                skip00828 = true;
              else
              {
                RG1L = DM2_TRY_PUSH_OBJECT_TO(lcon(0xffff), vl_14, vl_30, &vw_4c, &vw_70);
                if (RG1L == 0)
                  skip00829 = true;
                else
                {
                  DM2_LOAD_LOCALLEVEL_GRAPHICS_TABLE(signedlong(vw_4c), signedlong(vw_70), vl_40);
                  ddat.v1d27f8 = RG64w;
                  RG1L = signedlong(vw_70);
                  put16(parw08, RG1W);
                  DM2_MOVE_RECORD_TO(lcon(0xffff), lcon(0xffffffff), 0, signedlong(vw_4c), parw08);
                  DM2_LOAD_LOCALLEVEL_GRAPHICS_TABLE(signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), signedlong(vw_84));
                  RG4L = signedlong(ddat.v1d27f8);
                  RG1L = unsignedlong(vw_84);
                  if (RG4L == RG1L)
                  {
                    ddat.v1d27f8 = lcon(0xffffffff);
                    skip00830 = true;
                  }
                }
              }
            }

            if (skip00828)
            {
              //m_3B65B:
              vl_28 = signedlong(argw0);
              vl_24 = signedlong(vw_68);
              RG1L = unsignedlong(RG57w);
              vl_10 = RG1L;
              RG1L = DM2_TRY_PUSH_OBJECT_TO(RG1L, vl_24, vl_28, &vw_4c, &vw_70);
              if (RG1L == 0)
                skip00829 = true;
              else
              {
                RG1L = DM2_GET_PARTY_SPECIAL_FORCE();
                RG4L = unsignedlong(RG1W) / 4;
                RG1W = DM2_RAND16(RG4W / 2 + 1);
                RG4W += RG1W;
                RG1L = unsignedlong(RG4W);
                i32 parl11 = RG1L;
                DM2_ATTACK_CREATURE(vl_10, vl_24, vl_28, lcon(0x2000), 0, parl11);
                RG1L = signedlong(vw_70);
                put16(parw12, RG1W);
                DM2_MOVE_RECORD_TO(vl_10, vl_24, vl_28, signedlong(vw_4c), parw12);
                skip00830 = true;
              }
            }

            if (skip00829)
            {
              //m_3B6E5:
              RG58l = unsignedlong(RG57w);
              DM2_1c9a_0db0(RG58l);
              RG2L = signedlong(ddat.v1e0272);
              RG4L = signedlong(ddat.v1e0270);
              RG3L = lcon(0xfffffffd);
              RG1L = DM2_MOVE_RECORD_TO(RG58l, RG4L, RG2L, RG3L, 0);
              if (RG1W == 0)
              {
                RG3L = signedlong(RG64w);
                DM2_moverec_2fcf_01c5(RG58l, signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), RG3L, 0);
              }
              skip00830 = true;
            }
          }
        }

        if (skip00830)
        {
          //m_3B735:
          if (RG64w != vw_84)
            //m_3B75D:
            ddat.v1d27f8 = RG64w;
          else
          {
            parl35 = 0;
            parl34 = 1;
            RG4L = unsignedlong(ddat.v1e0272);
            RG1L = unsignedlong(ddat.v1e0270);
            RG3L = vl_44;
            RG2L = lcon(0xffff);
            skip00839 = true;
          }
        }
      }

      if (skip00839)
        //m_3BC56:
        DM2_moverec_3CE7D(RG1L, RG4L, RG2L, RG3L, parl34, parl35);
    }
  }

  //m_3BC5B:
  RG1L = vl_54;
  ddat.v1e1024 = RG1W;
  RG1W = vw_68;
  ddat.v1e102a = RG1W;
  ddat.v1e1028 = argw0;
  ddat.v1e102c = RG64w;
  DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_84));
  return 0;
}

// stacksize was 0x12c
void DM2_moverec_3CE7D(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i32 argl1)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG51p;
  i32 RG52l;
  i32 RG53l;
  i32 RG54l;
  i16 RG55w;
  i8* RG61p;
  c_wlreg _RG62;
  c_wlreg _RG63;
  i32 vl_00;
  i16 vw_04;
  c_ql32 vql_08;
  c_ql32 vql_0c;
  t_text tarr_10[0xc8];
  c_tim c38_d8;
  i32 vl_e4;
  i32 vl_e8;
  i32 vl_ec;
  unk* xp_f0;
  i32 vl_f4;
  i32 vl_f8;
  i32 vl_fc;
  unk* xp_100;
  i16 vw_104;
  i16 vw_108;
  i16 vw_10c;
  i16 vw_110;
  i32 vl_114;
  i8 vb_118;
  i16 parw00;
  i16 parw01;
  i16 parw07;
  i16 parw08;
  i16 parw13;
  i16 parw14;
  bool flag;

  vql_0c.poke32(RG1L);
  vql_08.poke32(RG4L);
  vw_04 = RG2W; // truncated
  vl_00 = RG3L;
  RG4P = NULL;
  xp_f0 = UPCAST(unk, RG4P);
  if (RG2W == lcon(0xffffffff))
  {
    //m_3CF0F:
    RG2L = lcon(0xffffffff);
    vw_10c = RG2W;
    vw_110 = RG2W;
  }
  else
  {
    RG1L = RG2L;
    RG1Blo ^= RG2Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    vw_10c = RG1W;
    RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG2W);
    vw_110 = RG1W;
    RG4W = vql_08.peek16();
    RG1L = DM2_GET_CREATURE_AT(unsignedlong(vql_0c.peek16()), RG4L);
    if (RG1W != lcon(0xffffffff))
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
      RG4P = RG1P;
      RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG1P, lcon(0x4)))));
      if (argl1 != 0)
      {
        if (argl0 == 0 || (jnz_test8(location(RG1P), lcon(0x1)) && jnz_test8(location(RG1P + lcon(0x1f)), lcon(0x8))))
        {
          RG4P += 2;
          xp_f0 = UPCAST(unk, RG4P);
        }
      }
    }
  }
  //m_3CF1C:
  if (argl0 == 0)
  {
    RG1L = signedlong(vw_10c);
    if (RG1L != lcon(0xffffffff))
    {
      RG61p = DOWNCAST(unk, xp_f0);
      if (RG61p != NULL)
      {
        //m_3CF53:
        RG1L = unsignedlong(vw_04);
        RG3L = 0;
        RG2L = lcon(0xffffffff);
        RG4P = RG61p;
      }
      else
      {
        RG3L = signedlong(vql_08.peek16());
        RG2L = signedlong(vql_0c.peek16());
        RG1L = unsignedlong(vw_04);
        RG4P = NULL;
      }
      //m_3CF65:
      DM2_CUT_RECORD_FROM(mk_record(RG1L), UPCAST(t_record, RG4P), RG2W, RG3W);
    }
  }
  //m_3CF6A:
  RG1P = DOWNCAST(t_tile*, &mapdat.map[unsignedlong(vql_0c.peek16())]); // adj
  RG4L = unsignedlong(vql_08.peek16());
  RG1P = pointer_at(RG1P);
  RG1Blo = byte_at(RG1P, RG4L);
  RG1L = unsignedlong(RG1Blo) >> 5;
  if (RG1Blo != 0)
    //m_3CFAA:
    vw_104 = -1;
  else
  {
    RG1L = unsignedlong(vw_04) >> bcon(0xe);
    vw_104 = RG1W;
  }
  //m_3CFB0:
  RG1L = 0;
  vl_e4 = RG1L;
  vl_ec = RG1L;
  vl_fc = RG1L;
  vl_f4 = RG1L;
  RG62W = record_to_word(DM2_GET_TILE_RECORD_LINK(vql_0c.peek16(), vql_08.peek16()));
  RG1L = signedlong(vw_104);
  if (RG1L != lcon(0xffffffff))
  {
    //m_3D10C:
    //m_3D111:
    for (;;)
    {
      if (RG62W == lcon(0xfffffffe))
        break;
      RG1L = unsignedlong(RG62W);
      RG4L = RG1L >> bcon(0xe);
      RG2L = signedlong(vw_104);
      if (RG2L == RG4L)
      {
        RG4L = RG62L;
        RG4W &= lcon(0x3c00);
        RG4L = unsignedlong(RG4W) >> bcon(0xa);
        if (RG4L > lcon(0x4))
        {
          vl_f4 = 1;
          RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG1W);
          RG1Blo = RG1W == vw_110 ? 1 : 0;
          RG1L = unsignedlong(RG1Blo);
          vl_ec |= RG1L;
          RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG62W);
          RG1Blo = RG1W != vw_110 ? 1 : 0;
          RG1L = unsignedlong(RG1Blo);
          vl_e4 |= RG1L;
        }
      }
      //m_3D170:
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG62UW)));
      RG62L = RG1L;
    }
  }
  else
  {
    //m_3CFE6:
    for (;;)
    {
      bool skip00858 = false;
      if (RG62W == lcon(0xfffffffe))
        break;
      RG1L = RG62L;
      RG1W &= lcon(0x3c00);
      RG1L = unsignedlong(RG1W) >> bcon(0xa);
      RG4L = RG1L;
      RG1L = unsignedlong(RG4W);
      if (RG1L != lcon(0x4))
        skip00858 = true;
      else
      {
        RG52l = unsignedlong(RG62W);
        RG1L = DM2_IS_OBJECT_FLOATING(RG52l);
        if (RG1L != 0)
          skip00858 = true;
        else
        {
          RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(RG52l);
          if ((RG1Blo & bcon(0x1)) != 0)
          {
            RG4L = signedlong(vw_110);
            RG2L = lcon(0xff);
            RG1P = DOWNCAST(i16, DM2_1c9a_06bd(RG52l, RG4L, RG2L));
            if (RG1P != NULL)
              vl_ec = 1;
          }
          vl_fc = 1;
        }
      }

      if (skip00858)
      {
        bool skip00859 = false;
        //m_3D04F:
        RG1L = unsignedlong(RG4W);
        if (RG1L != 2)
          skip00859 = true;
        else
        {
          RG1L = signedlong(vw_10c);
          if (RG1L != lcon(0xffffffff) || argl0 == 0 || vl_00 != 0)
            skip00859 = true;
          else
          {
            RG4L = unsignedlong(RG62W);
            RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
            RG3Blo = byte_at(RG1P, 2);
            if ((RG3Blo & bcon(0x6)) == 0 && (RG3Blo & bcon(0x1)) != 0)
            {
              RG2L = 1;
              DM2_QUERY_MESSAGE_TEXT(tarr_10, RG4L, RG2L);
              DM2_DISPLAY_HINT_TEXT(lcon(0xf), tarr_10);
            }
          }
        }

        if (skip00859)
        {
          //m_3D0B2:
          RG1L = unsignedlong(RG4W);
          if (RG1L > lcon(0x4) && RG1L < lcon(0xe))
          {
            vl_f4 = 1;
            RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG62W);
            RG3W = vw_110;
            RG1Blo = RG1W == RG3W ? 1 : 0;
            RG1L = unsignedlong(RG1Blo);
            vl_ec |= RG1L;
            RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG62W);
            RG1Blo = RG1W != RG3W ? 1 : 0;
            RG1L = unsignedlong(RG1Blo);
            vl_e4 |= RG1L;
          }
        }
      }

      //m_3D0FB:
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG62UW)));
      RG62L = RG1L;
    }
  }
  //m_3D17E:
  if (argl0 != 0)
  {
    RG1L = signedlong(vw_10c);
    if (RG1L != lcon(0xffffffff))
    {
      RG1P = DOWNCAST(unk, xp_f0);
      if (RG1P != NULL)
      {
        //m_3D1B3:
        RG1L = unsignedlong(vw_04);
        RG3L = 0;
        RG2L = lcon(0xffffffff);
        RG4P = DOWNCAST(unk, xp_f0);
      }
      else
      {
        RG3L = signedlong(vql_08.peek16());
        RG2L = signedlong(vql_0c.peek16());
        RG1W = vw_04;
        RG4P = NULL;
      }
      //m_3D1C6:
      DM2_APPEND_RECORD_TO(mk_record(RG1L), UPCAST(t_record, RG4P), RG2W, RG3W);
    }
  }
  //m_3D1CB:
  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(vql_0c.peek16(), vql_08.peek16()));
  //m_3D1E2:
  for (;;)
  {
    RG63L = RG1L;
    if (RG63W == lcon(0xfffffffe))
      return;
    RG1L = signedlong(vw_104);
    if (RG1L != lcon(0xffffffff))
    {
      RG1L = DM2_IS_WALL_ORNATE_SPRING(unsignedlong(RG63W));
      if (RG1L != 0)
      {
        RG2L = unsignedlong(vw_04);
        RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG2L);
        if (RG1Blo == lcon(0x15))
        {
          RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG2L);
          if (RG1Blo == 0)
          {
            RG1W = DM2_ADD_ITEM_CHARGE(RG2L, 0);
            RG4W = RG1W;
            if (RG1W < 0)
              return;
            if (RG1W >= party.heros_in_party)
              return;
            c38_d8.setmticks(ddat.v1d3248, timdat.gametick + 1);
            c38_d8.settype(0xd);
            c38_d8.setactor(RG4Blo);
            c38_d8.setxyA(vql_0c.peek8(), vql_08.peek8());
            c38_d8.setxyB(CUTX8(vw_104), 2);
            DM2_QUEUE_TIMER(&c38_d8);
            return;
          }
        }
      }
    }
    //m_3D29B:
    RG1L = RG63L;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    RG4L = RG1L;
    RG1L = unsignedlong(RG4W);
    if (RG1L != lcon(0x3))
    {
      //m_3D587:
      if (RG1L != 2)
      {
        //m_3D920:
        if (RG1L >= lcon(0x4))
          return;
      }
      else
      {
        RG4L = unsignedlong(RG63W);
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
        RG2P = RG1P;
        xp_100 = UPCAST(unk, RG1P);
        RG1W = word_at(RG1P, 2) & lcon(0x6);
        if (RG1W == 2)
        {
          RG1L = signedlong(vw_104);
          if (RG1L != lcon(0xffffffff))
          {
            //m_3D895:
            RG4L >>= 14;
            if (RG1L == RG4L)
            {
              RG1W = word_at(RG2P, 2);
              RG1UW >>= bcon(0x3);
              RG1L = unsignedlong(RG1W);
              RG1UL >>= bcon(0x8);
              RG1L &= lcon(0x1f);
              if (mkul(RG1L) >= mkul(lcon(0x4)))
              {
                bool skip00863 = false;
                if (mkul(RG1L) <= mkul(lcon(0x4)))
                  skip00863 = true;
                else
                {
                  if (RG1L == lcon(0x8))
                    skip00863 = true;
                }

                if (skip00863)
                {
                  //m_3D8C3:
                  if (vl_ec == 0)
                  {
                    RG1P = DOWNCAST(unk, xp_100);
                    RG1W = word_at(RG1P, 2) & lcon(0x1);
                    if (RG1W != mkw(argl0))
                    {
                      RG1P = DOWNCAST(unk, xp_100);
                      RG1W = word_at(RG1P, 2);
                      RG1UW >>= bcon(0x3);
                      RG4L = unsignedlong(RG1Blo);
                      RG3L = lcon(0xe);
                      RG2L = lcon(0xb);
                      RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, RG4Blo, RG2Blo, RG3Blo);
                      RG4L = unsignedlong(RG1W);
                      RG1L = signedlong(vw_110);
                      if (RG1L == RG4L)
                      {
                        RG1L = argl0;
                        RG1W &= 0x1;
                        RG4P = DOWNCAST(unk, xp_100);
                        and8(location(RG4P + 2), lcon(0xfffffffe));
                        or16(location(RG4P + 2), RG1W);
                      }
                    }
                  }
                }
              }
            }
          }
          else
          {
            RG3W = word_at(RG2P, 2);
            RG3UW >>= bcon(0x3);
            RG1L = unsignedlong(RG3W);
            RG1UL >>= bcon(0x8);
            RG1L &= lcon(0x1f);
            if (mkul(RG1L) >= mkul(lcon(0x9)))
            {
              if (mkul(RG1L) <= mkul(lcon(0x9)))
              {
                //m_3D5EE:
                RG1L = signedlong(vw_10c);
                if (RG1L == lcon(0xffffffff) && vl_00 == 0 && party.heros_in_party != 0)
                {
                  RG3W = unsignedword(RG3Blo);
                  RG4L = unsignedlong(RG3W);
                  RG1W = DM2_RAND16(100);
                  if (mkul(RG1W) < mkul(RG4L))
                  {
                    RG1W = word_at(RG2P, 2) & lcon(0x1);
                    if (RG1W != mkw(argl0))
                    {
                      RG1L = timdat.gametick + 1;
                      i32 parl05 = RG1L;
                      RG53l = unsignedlong(vql_08.peek16());
                      RG1L = unsignedlong(vql_0c.peek16());
                      vl_f8 = RG1L;
                      DM2_INVOKE_MESSAGE(RG1L, RG53l, 0, 0, parl05);
                      RG1L = timdat.gametick + lcon(0x5);
                      i32 parl06 = RG1L;
                      RG3L = 1;
                      RG2L = 0;
                      RG4L = RG53l;
                      DM2_INVOKE_MESSAGE(vl_f8, RG4L, RG2L, RG3L, parl06);
                    }
                  }
                }
              }
              else
              {
                if (RG1L == lcon(0xa))
                {
                  //m_3D68E:
                  RG1L = signedlong(vw_10c);
                  if (RG1L == lcon(0xffffffff) && argl0 == 0 && party.heros_in_party != 0)
                  {
                    bool skip00862 = false;
                    RG2L = 0;
                    RG3L = 0;
                    //m_3D705:
                    for (;;)
                    {
                      RG1L = signedlong(RG3W);
                      RG4L = unsignedlong(party.heros_in_party);
                      if (RG1L >= RG4L)
                      {
                        RG1P = DOWNCAST(unk, xp_100);
                        if (jz_test8(location(RG1P + 2), lcon(0x1)))
                          //m_3D725:
                          RG4L = lcon(0x19);
                        else
                          RG4L = lcon(0x32);
                        break;
                      }
                      //m_3D6BB:
                      RG4L = RG1L;
                      RG1L <<= bcon(0x5);
                      RG1L += RG4L;
                      RG1L *= 8;
                      RG1L -= RG4L;
                      if (party.hero[RG1L / 263].curHP != 0)
                      {
                        RG1W = party.hero[RG1L / 263].get_max_load();
                        RG54l = (RG1L & lcon(0xffff)) / 2;
                        RG1W = party.get_player_weight((e_hero)RG3W);
                        RG4L = unsignedlong(RG1W);
                        RG1L = RG4L;
                        RG4L = RG1L % RG54l;
                        RG1L /= RG54l;
                        RG4L = RG1L;
                        RG1L = signedlong(RG2W) + RG4L;
                        RG2L = RG1L;
                      }
                      //m_3D704:
                      RG3L++;
                    }
                    //m_3D72A:
                    RG2L = signedlong(RG2W);
                    RG1L = 10 * RG2L + RG4L;
                    RG4L = signedlong(DM2_MIN(90, RG1W));
                    if (vl_00 != 0)
                      skip00862 = true;
                    else
                    {
                      RG1W = DM2_RAND16(100);
                      if (RG1W >= RG4W)
                        skip00862 = true;
                      else
                      {
                        c38_d8.setmticks(ddat.v1d3248, timdat.gametick);
                        c38_d8.settype(0x5d);
                        c38_d8.setactor(0);
                        RG1L = vql_0c.peek32();
                        RG1W &= lcon(0x1f);
                        c38_d8.mask_wvalueA(0xffe0, 0);
                        RG3W = c38_d8.getA() | RG1W; // adj
                        RG1L = vql_08.peek32();
                        RG1W &= lcon(0x1f);
                        RG55w = RG3W & lcon(0xfc1f); // adj
                        RG1L <<= bcon(0x5);
                        RG4W = RG55w | RG1W; // adj
                        c38_d8.setA(RG4W);
                        RG1Blo = CUTX8(ddat.v1e0266);
                        c38_d8.setxB(RG1Blo);
                        RG1W = ddat.v1e0258 & lcon(0x3);
                        c38_d8.mask_wvalueA(0xf3ff, RG1W << 10);
                        DM2_QUEUE_TIMER(&c38_d8);
                        RG1L = signedlong(DM2_RANDDIR());
                        RG3L = RG1L;
                        RG4L = signedlong(RG1W);
                        if (party.hero[RG4L].curHP == 0)
                          RG3W = eventqueue.event_heroidx;
                        RG1L = signedlong(ddat.v1e0272);
                        put16(parw08, RG1W);
                        RG1L = signedlong(ddat.v1e0270);
                        put16(parw07, RG1W);
                        RG4L = signedlong(RG3W);
                        RG1L = 263 * RG4L;
                        RG4L = unsignedlong(party.hero[RG4L].herotype);
                        RG3L = lcon(0xfe);
                        RG2L = lcon(0x82);
                        DM2_QUEUE_NOISE_GEN2(lcon(0x16), RG4Blo, RG2Blo, RG3Blo, parw07, parw08, lcon(0x1), lcon(0x69), lcon(0xc8));
                      }
                    }

                    if (skip00862)
                    {
                      //m_3D854:
                      put16(parw14, ddat.v1e0272);
                      put16(parw13, ddat.v1e0270);
                      RG1P = DOWNCAST(unk, xp_100);
                      RG1W = word_at(RG1P, 2);
                      RG1UW >>= bcon(0x3);
                      RG4L = unsignedlong(RG1Blo);
                      RG3L = lcon(0x8c);
                      RG2L = lcon(0x88);
                      DM2_QUEUE_NOISE_GEN1(lcon(0xa), RG4Blo, RG2Blo, RG3W, lcon(0x80), parw13, parw14, lcon(0x1));
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    else
    {
      RG4L = unsignedlong(RG63W);
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
      RG51p = RG1P;
      if (jnz_test8(location(RG1P + 2), lcon(0x7f)))
      {
        bool skip00869 = false;
        bool skip00871 = false;
        RG1W = word_at(RG1P, 2);
        vl_114 = RG1L;
        RG1UW >>= bcon(0x7);
        vl_e8 = argl0;
        RG2L = signedlong(vw_104);
        if (RG2L != lcon(0xffffffff))
        {
          //m_3D3F3:
          RG4L >>= 14;
          if (RG2L == RG4L)
          {
            RG4L = vl_114;
            RG4W &= lcon(0x7f);
            RG2L = RG4L;
            if (RG4UW < mkuw(lcon(0x29)))
            {
              //m_3D421:
              if (RG4W == lcon(0x1a))
              {
                //m_3D466:
                if (vl_ec == 0)
                {
                  RG1W = word_at(RG51p, lcon(0x4));
                  RG1L <<= bcon(0xd);
                  RG1UW >>= bcon(0xf);
                  if (RG1W != mkw(argl0))
                  {
                    RG3L = lcon(0xe);
                    RG2L = lcon(0xb);
                    RG1Blo = DM2_GET_WALL_DECORATION_OF_ACTUATOR(RG51p);
                    RG4L = unsignedlong(RG1Blo);
                    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, RG4Blo, RG2Blo, RG3Blo);
                    RG4L = unsignedlong(RG1W);
                    RG1L = signedlong(vw_110);
                    if (RG1L == RG4L)
                    {
                      RG1L = argl0;
                      RG1W &= 0x1;
                      and8(location(RG51p + lcon(0x4)), lcon(0xfffffffb));
                      RG1L *= 4;
                      or16(location(RG51p + lcon(0x4)), RG1W);
                      skip00871 = true;
                    }
                  }
                }
              }
            }
            else
            {
              if (RG4UW <= mkuw(lcon(0x29)))
              {
                //m_3D42C:
                flag = vl_f4 != 0;
                skip00869 = true;
              }
              else
              {
                if (RG4UW <= mkuw(lcon(0x2a)))
                {
                  //m_3D435:
                  if (vl_ec == 0)
                  {
                    RG1W = word_at(RG51p, 2);
                    RG1UW >>= bcon(0x7);
                    flag = RG1W != vw_110;
                    skip00869 = true;
                  }
                }
                else
                {
                  if (RG4W == lcon(0x2b))
                  {
                    //m_3D450:
                    if (vl_e4 == 0 && RG1W != vw_110)
                      skip00871 = true;
                  }
                }
              }
            }
          }
        }
        else
        {
          RG4W = word_at(RG51p, 2) & lcon(0x7f);
          RG4L--;
          if (RG4UW <= mkuw(lcon(0x7)))
          {
            bool skip00865 = false;
            bool skip00864 = false;
            bool skip00867 = false;
            RG4L = unsignedlong(RG4W);
            switch (RG4L)
            {
              case 0:
                //m_3D314:
                if (vl_00 == 0)
                {
                  flag = vl_f4 != 0;
                  skip00864 = true;
                }
                break;

              case 1:
                //m_3D33A:
                RG1L = signedlong(vw_10c);
                if (RG1L <= lcon(0x4))
                {
                  flag = vl_00 != 0;
                  skip00864 = true;
                }
                break;

              case 2:
                //m_3D352:
                RG4L = signedlong(vw_10c);
                if (RG4L == RG2L && party.heros_in_party != 0)
                {
                  if (RG1W != 0)
                  {
                    //m_3D37C:
                    RG3L = argl0;
                    if (RG3L != 0)
                    {
                      //m_3D38B:
                      RG4L = signedlong(ddat.v1e0258);
                      RG1L = signedlong(RG1W);
                      RG4L++;
                      RG1Blo = RG1L == RG4L ? 1 : 0;
                      RG1L = unsignedlong(RG1Blo);
                      skip00865 = true;
                    }
                    else
                    {
                      vl_e8 = RG3L;
                      skip00871 = true;
                    }
                  }
                  else
                  {
                    flag = vl_00 != 0;
                    skip00869 = true;
                  }
                }
                break;

              case 3:
                //m_3D3A8:
                if (RG1W == vw_110)
                {
                  flag = vl_ec != 0;
                  skip00869 = true;
                }
              case 4:
              case 5:
                break;

              case 6:
                //m_3D3BB:
                RG1L = signedlong(vw_10c);
                if (RG1L <= lcon(0x4))
                {
                  RG1L = signedlong(vw_10c);
                  if (RG1L != RG2L)
                    skip00867 = true;
                }
                break;

              case 7:
                //m_3D3DD:
                RG4L = signedlong(vw_10c);
                if (RG4L == RG2L)
                {
                  RG1L = DM2_query_2fcf_16ff(signedlong(RG1W));
                  skip00865 = true;
                }
                break;

              default: throw(THROW_DMABORT);
            }

            if (skip00865)
            {
              //m_3D3A0:
              vl_e8 = RG1L;
              skip00871 = true;
            }

            if (skip00864)
            {
              //m_3D325:
              if (!flag)
                skip00867 = true;
            }

            if (skip00867)
            {
              //m_3D32B:
              flag = vl_fc != 0;
              skip00869 = true;
            }
          }
        }

        if (skip00869)
        {
          //m_3D32F:
          if (!flag)
            skip00871 = true;
        }

        if (skip00871)
        {
          bool skip00861 = false;
          //m_3D4C9:
          RG1W = word_at(RG51p, lcon(0x4));
          RG1L <<= bcon(0xa);
          RG1UW >>= bcon(0xf);
          RG1L = unsignedlong(RG1W);
          RG3L = vl_e8;
          RG3L ^= RG1L;
          vl_e8 = RG3L;
          RG1W = word_at(RG51p, lcon(0x4));
          RG1L <<= 11;
          RG1UW >>= 14;
          vw_108 = RG1W; // truncated
          if (RG1W != lcon(0x3))
          {
            //m_3D501:
            if (RG3L != 0)
              skip00861 = true;
          }
          else
          {
            RG1Blo = RG3L == 0 ? 1 : 0;
            RG1W = unsignedword(RG1Blo);
            vw_108 = RG1W; // truncated
            skip00861 = true;
          }

          if (skip00861)
          {
            //m_3D509:
            if (jnz_test8(location(RG51p + lcon(0x4)), lcon(0x40)))
            {
              RG1L = signedlong(vql_08.peek16());
              put16(parw01, RG1W);
              RG1L = signedlong(vql_0c.peek16());
              put16(parw00, RG1W);
              RG3L = lcon(0xfe);
              RG2L = lcon(0x88);
              RG1L = signedlong(vw_104);
              if (RG1L != lcon(0xffffffff))
                //m_3D54D:
                RG1Blo = DM2_GET_WALL_DECORATION_OF_ACTUATOR(RG51p);
              else
                RG1Blo = DM2_GET_FLOOR_DECORATION_OF_ACTUATOR(RG51p);
              //m_3D554:
              RG4L = unsignedlong(RG1Blo);
              RG1L = signedlong(vw_104);
              RG1Blo = (RG1L == lcon(0xffffffff) ? 1 : 0) + lcon(0x9);
              vb_118 = RG1Blo;
              DM2_QUEUE_NOISE_GEN2(vb_118, RG4Blo, RG2Blo, RG3Blo, parw00, parw01, lcon(0x1), lcon(0x8c), lcon(0x80));
            }
            //m_3D573:
            RG4L = unsignedlong(vw_108);
            RG2L = 0;
            RG1P = RG51p;
            DM2_INVOKE_ACTUATOR(RG1P, RG4L, RG2L);
          }
        }
      }
    }

    //m_3D925:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG63UW)));
  }
  FOREND
}
