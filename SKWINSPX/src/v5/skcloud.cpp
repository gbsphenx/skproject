#include <skrecord.h>
#include <skmap.h>
#include <xtypes.h>
#include <sktimer.h>
#include <sktimprc.h>
#include <dm2data.h>
#include <skgdtqdb.h>
#include <bcreatur.h>
#include <skhero.h>
#include <util.h>
#include <skrandom.h>
//#include "c_creature.h"
#include <sfx.h>
#include <SK1C9A.h>
#include <skmove.h>
#include <skcloud.h>

#include "emu.h"
#include "regs.h"

static i32 DM2_075f_0182(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  i16 vw_00;

  RG5L = RG1L;
  RG1L = RG4L;
  vw_00 = RG3W;
  RG4L = signedlong(RG2W);
  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(RG1W, RG4W));
  //m_66F9:
  for (;;)
  {
    RG3L = RG1L;
    if (RG3W == lcon(0xfffffffe))
      break;
    RG1L = RG3L;
    RG1Blo ^= RG3Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    if (RG1L == lcon(0x3))
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
      RG4P = RG1P;
      RG2W = word_at(RG1P, 2) & lcon(0x7f);
      if (RG2W == lcon(0x26))
      {
        bool skip00015 = false;
        RG2W = word_at(RG1P, 2);
        RG2Blo &= lcon(0xffffff80);
        if (RG2W == lcon(0xffffff80))
          skip00015 = true;
        else
        {
          RG2L = RG5L - lcon(0xff80);
          RG4W = word_at(RG1P, 2);
          RG4UW >>= bcon(0x7);
          if (RG4W != RG2W)
            //m_6765:
            RG4L = 0;
          else
            skip00015 = true;
        }

        if (skip00015)
          //m_675E:
          RG4L = 1;
        bool skip00017 = false;
        //m_6767:
        RG2W = word_at(RG1P, lcon(0x4));
        RG2L <<= bcon(0xa);
        RG2UW >>= bcon(0xf);
        RG2L = unsignedlong(RG2W);
        RG4L ^= RG2L;
        if (jnz_test8(location(RG1P + lcon(0x4)), lcon(0x4)))
        {
          RG2L = unsignedlong(RG5W);
          if (RG2L != lcon(0xff82))
            //m_6794:
            RG2L = lcon(0xff);
          else
            RG2L = lcon(0x7f);
          //m_6799:
          if (signedlong(vw_00) < RG2L)
            RG4L = 0;
        }
        //m_67A6:
        RG2W = word_at(RG1P, lcon(0x4)) & lcon(0x18);
        if (RG2W != lcon(0x18))
        {
          //m_67C6:
          if (RG4L != 0)
          {
            RG4W = word_at(RG1P, lcon(0x4));
            RG4L <<= 11;
            RG4UW >>= 14;
            RG4L = unsignedlong(RG4W);
            RG2L = 0;
            skip00017 = true;
          }
        }
        else
        {
          RG2L = 0;
          RG4Blo = RG4L == 0 ? 1 : 0;
          RG4W = unsignedword(RG4Blo);
          RG4L = unsignedlong(RG4W);
          skip00017 = true;
        }

        if (skip00017)
          //m_67DD:
          DM2_INVOKE_ACTUATOR(RG1P, RG4L, RG2L);
      }
    }
    //m_67E2:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG3UW)));
  }
  return RG1L;
}

// stacksize was 0x4c
i32 DM2_CREATE_CLOUD(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p;
  c_wlreg _RG61;
  c_wlreg _RG62;
  c_ql32 vql_00;
  c_tim c38_04;
  i32 vl_10;
  i32 vl_14;
  i32 vl_18;
  i16 vw_1c;
  i16 vw_20;
  i16 vw_24;
  i16 vw_28;
  i16 vw_2c;
  c_ql32 vql_30;
  i16 vw_34;
  i16 vw_38;
  i16 parw00;
  i16 parw01;
  i16 parw04;
  bool skip00019 = false;
  bool skipinloop = false;

  vql_00.poke32(RG1L);
  RG61L = RG4L;
  vw_34 = RG2W;
  vw_38 = RG3W;
  RG1L = DM2_ALLOC_NEW_RECORD(15);
  vql_30.poke32(RG1L);
  if (RG1W == lcon(0xffffffff))
    return RG1L;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
  RG5p = RG1P;
  RG1L = unsignedlong(RG2W);
  if (RG1L > lcon(0xff))
  {
    //m_6843:
    RG1Bhi ^= RG2Bhi;
    vw_2c = RG1W;
    RG1L = RG3L;
    RG1Bhi ^= RG3Bhi;
    vw_20 = RG1W;
    RG4L = RG2L;
    RG4UW >>= bcon(0x8);
    vw_34 = RG4W;
    RG2L = RG3L;
    RG2UW >>= bcon(0x8);
    vw_38 = RG2W;
    RG3L = RG4L - 1;
    vw_34 = RG3W;
  }
  else
  {
    vw_2c = RG2W;
    vw_20 = RG3W;
  }
  //m_686C:
  RG1L = unsignedlong(mkuw(argl0)); // truncated TODO check if arg is always word
  if (RG1L != lcon(0xff))
  {
    //m_687F:
    and8(location(RG5p + 2), lcon(0x7f));
    RG4L = vql_30.peek32();
    RG4Bhi &= lcon(0x3f);
    RG1L = argl0 << bcon(0xe);
    RG4L |= RG1L;
    vql_30.poke32(RG4L);
  }
  else
    or8(location(RG5p + 2), lcon(0xffffff80));
  //m_6894:
  RG1L = vql_00.peek32() - lcon(0xff80);
  RG1W &= lcon(0x7f);
  and8(location(RG5p + 2), lcon(0xffffff80));
  or16(location(RG5p + 2), RG1W);
  RG1L = RG61L;
  RG1W = unsignedword(RG1Blo);
  mov8(location(RG5p + lcon(0x3)), RG1Bhi);
  RG1L <<= bcon(0x8);
  or16(location(RG5p + 2), RG1W);
  RG1W = ddat.v1d3248;
  if (RG1W != ddat.v1e0282)
    skip00019 = true;
  else
  {
    RG4L = unsignedlong(vw_34);
    RG1L = signedlong(ddat.v1e0280);
    if (RG4L != RG1L)
      skip00019 = true;
    else
    {
      RG1L = unsignedlong(vw_38);
      RG4L = signedlong(ddat.v1e027e);
      if (RG1L != RG4L)
        skip00019 = true;
      else
        vw_28 = wcon(0xff);
    }
  }

  if (skip00019)
  {
    //m_68F3:
    RG1L = unsignedlong(RG61W) / 2 + lcon(0x80);
    RG4L = signedlong(RG1W);
    RG1L = signedlong(DM2_MIN(wcon(0xff), RG4W));
    vw_28 = RG1W;
  }
  bool skip00021 = false;
  //m_6910:
  RG1L = signedlong(vw_28);
  put16(parw04, RG1W);
  RG1L = signedlong(vw_38);
  vl_10 = RG1L;
  put16(parw01, RG1W);
  RG1L = signedlong(vw_34);
  vl_14 = RG1L;
  put16(parw00, RG1W);
  RG1W = word_at(RG5p, 2);
  RG1Blo &= lcon(0x7f);
  RG4L = unsignedlong(RG1Blo);
  DM2_QUEUE_NOISE_GEN2(lcon(0xd), RG4Blo, lcon(0x81), lcon(0xfe), parw00, parw01, lcon(0x1), lcon(0x6c), parw04);
  RG1L = unsignedlong(vql_30.peek16());
  RG3L = vl_10;
  DM2_APPEND_RECORD_TO(mk_record(RG1L), NULL, CUTX16(vl_14), RG3W);
  RG1L = unsignedlong(vql_00.peek16());
  if (RG1L != lcon(0xffe4))
  {
    //m_6974:
    if (RG1L != lcon(0xff8e))
      //m_6984:
      RG1L = 1;
    else
      RG1L = unsignedlong(RG61W) / 2;
  }
  else
    RG1L = 5;
  //m_6989:
  c38_04.setmticks(ddat.v1d3248, signedlong(RG1W) + timdat.gametick);
  c38_04.settype(0x19);
  RG2W = unsignedword(RG2Blo);
  c38_04.setactor(0);
  RG1L = vql_30.peek32();
  c38_04.setB(RG1W);
  c38_04.setxyA(CUTX8(vw_34), CUTX8(vw_38));
  DM2_QUEUE_TIMER(&c38_04);
  RG1L = unsignedlong(vql_00.peek16());
  if (RG1L != lcon(0xff82) && RG1L != lcon(0xff80) && RG1L != lcon(0xffb0) && RG1L != lcon(0xffb1))
  {
    //m_6BD6:
    if (RG1L != lcon(0xff84) && RG1L != lcon(0xff8d))
      jump L_fin;
    RG3L = signedlong(vw_38);
    RG2L = signedlong(vw_34);
    RG4L = RG3L;
    RG1L = dm2_GET_TILE_VALUE(RG2L, RG4L);
    RG4L = unsignedlong(RG1Blo) >> 5;
    RG1Bhi = RG4Blo;
    RG4L = unsignedlong(RG1Bhi);
    if (RG4L != lcon(0x4))
      jump L_fin;
    RG1Blo &= lcon(0x7);
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == lcon(0x5))
      jump L_fin;
    RG4L = RG3L;
    RG1RP = DM2_GET_ADDRESS_OF_TILE_RECORD(RG2W, RG4W);
    RG5p = RG1P;
    if (jz_test8(location(RG1P + 2), lcon(0x40)) && jz_test8(location(RG1P + lcon(0x3)), lcon(0x20)))
      jump L_fin;
    RG1L = timdat.gametick + 1;
    i32 parl07 = RG1L;
    RG1L = unsignedlong(vql_00.peek16());
    if (RG1L != lcon(0xff84))
      //m_6C4A:
      RG1Blo = 0;
    else
      RG1Blo = 2;
    //m_6C4C:
    RG3L = unsignedlong(RG1Blo);
    DM2_INVOKE_MESSAGE(unsignedlong(vw_34), unsignedlong(vw_38), 0, RG3L, parl07);
    jump L_fin;
  }
  RG1W = vw_2c;
  vw_34 = RG1W;
  vw_24 = RG1W;
  RG1W = vw_20;
  vw_38 = RG1W;
  vw_1c = RG1W;
  vl_18 = 1;
  RG62W = RG61W / 2 + 1;
  RG62W += DM2_RAND16(RG62W) + 1;
  vw_28 = RG62W;
  RG1L = unsignedlong(vql_00.peek16());
  if (RG1L == lcon(0xff80) || RG1L == lcon(0xffb0))
    skip00021 = true;
  else
  {
    RG62UW >>= bcon(0x1);
    if (RG62W != 0)
      skip00021 = true;
  }

  if (skip00021)
  {
    //m_6A44:
    RG1W = ddat.v1d3248;
    if (RG1W == ddat.v1e0282)
    {
      RG4L = unsignedlong(vw_34);
      RG1L = signedlong(ddat.v1e0280);
      if (RG4L == RG1L)
      {
        RG4L ^= RG1L;
        RG4W = vw_38;
        RG1L = signedlong(ddat.v1e027e);
        if (RG4L == RG1L)
          DM2_ATTACK_PARTY(unsignedlong(RG62W), lcon(0x3f), lcon(0x1));
      }
    }
    RG1L = DM2_GET_CREATURE_AT(unsignedlong(vw_34), unsignedlong(vw_38));
    vql_30.poke32(RG1L);
    if (RG1W == lcon(0xffffffff))
      skipinloop = true;
    //m_6AA9:
    for (;;)
    {

      if (!skipinloop)
      {
        RG2L = unsignedlong(vql_30.peek16());
        RG1L = DM2_1c9a_0958(RG2L);
        if (RG1L == 0)
        {
          RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
          RG1Blo = byte_at(RG1P, lcon(0x4));
          RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(RG1Blo)));
          RG2P = RG1P;
          if (jnz_test8(location(RG1P + lcon(0x19)), lcon(0x10)))
          {
            RG1L = unsignedlong(vql_00.peek16());
            if (RG1L != lcon(0xff80))
            {
              RG62UW >>= bcon(0x2);
              RG4L = signedlong(RG62W);
              RG62L = signedlong(DM2_MAX(1, RG4W));
            }
          }
          //m_6AFA:
          RG1L = unsignedlong(word_at(RG2P, lcon(0x18))) >> 4 & lcon(0xf);
          RG4L = signedlong(RG1W);
          if (RG4L != lcon(0xf))
          {
            if (jnz_test8(location(RG2P), lcon(0x20)))
              RG62UW >>= bcon(0x2);
            RG1W = DM2_RAND16(2 * RG1W + 1);
            RG62W -= RG1W;
            if (RG62W > 0)
            {
              RG1L = unsignedlong(RG62W);
              i32 parl06 = RG1L;
              RG2L = signedlong(vw_38);
              RG4L = signedlong(vw_34);
              RG1L = unsignedlong(vql_30.peek16());
              RG3L = lcon(0x200d);
              DM2_ATTACK_CREATURE(RG1L, RG4L, RG2L, RG3L, lcon(0x64), parl06);
            }
          }
        }
      }
      //m_6B4F:
      skipinloop = false;
      if (vl_18 == 0)
        break;
      RG2L = unsignedlong(mkuw(argl0)); // truncated TODO check if arg is always word
      RG4P = ADRESSOF(i16, &vw_1c);
      RG1L = DM2_query_1c9a_03cf(&vw_24, RG4P16, RG2L);
      vql_30.poke32(RG1L);
      RG1Blo = RG1W != wcon(0xffff) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L == 0)
        break;
      RG1L = signedlong(vw_24);
      RG4L = unsignedlong(vw_2c);
      if (RG1L == RG4L)
      {
        RG4L = signedlong(vw_1c);
        RG1L = unsignedlong(vw_20);
        if (RG4L == RG1L)
          break;
      }
      //m_6B97:
      vw_34 = vw_24;
      vw_38 = vw_1c;
      vl_18 = 0;
      RG1L = signedlong(vw_28);
      RG1L <<= bcon(0x10);
      RG1L >>= bcon(0x11);
      RG62L = RG1L;
    }
  }
  //m_6BB7:
  RG1W = ddat.v1d3248;
  if (RG1W != ddat.v1e0266)
    jump L_fin;
  ddat.v1e0390.b_00 |= 1;

L_fin:
  RG1W = word_at(RG5p, 2);
  RG1UW >>= bcon(0x8);
  RG3L = signedlong(RG1W);
  return DM2_075f_0182(unsignedlong(vql_00.peek16()), signedlong(vw_2c), signedlong(vw_20), RG3L);
}

i16 DM2_CALC_CLOUD_DAMAGE(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p;
  c_lreg _RG61;
  c_wlreg _RG62;
  i8* RG7p;
  i8 vb_00;
  i8 vb_04;

  RG3L = RG4L;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
  RG5p = RG1P;
  RG4L = 0;
  RG2W = word_at(RG1P, 2) & lcon(0x7f);
  RG61L = unsignedlong(RG2W);
  if (RG61L >= lcon(0x8))
    return RG4W;
  RG1Blo = table1d26c8[RG61L];
  vb_00 = RG1Blo;
  if (RG1Blo == 0)
    return RG4W;
  if (RG3W != lcon(0xffffffff))
  {
    bool skip00070 = false;
    //m_82FB:
    RG1L = RG3L;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    RG62L = RG1L;
    if (RG1W != 0)
      skip00070 = true;
    else
    {
      RG1Blo = vb_00 & 2;
      vb_04 = RG1Blo;
      RG1L = unsignedlong(vb_04);
      if (RG1L == 0)
        skip00070 = true;
      else
      {
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
        RG7p = RG1P;
      }
    }

    if (skip00070)
    {
      //m_8332:
      RG1L = signedlong(RG62W);
      if (RG1L != lcon(0x4))
        return RG4W;
      RG1Blo = vb_00 & lcon(0x8);
      vb_04 = RG1Blo;
      RG1L = unsignedlong(vb_04);
      if (RG1L == 0)
        return RG4W;
    }
  }
  else
  {
    RG1Blo &= 0x4;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == 0)
      return RG4W;
    RG62L = lcon(0xffffffff);
  }
  //m_8355:
  RG4W = word_at(RG5p, 2);
  RG4UW >>= bcon(0x8);
  RG1L = signedlong(RG62W);
  if (RG1L == lcon(0x4))
  {
    RG1P = DM2_QUERY_CREATURE_AI_SPEC_FROM_TYPE(unsignedlong(RG3W));
    if (jnz_test8(location(RG1P + lcon(0x19)), lcon(0x10)))
    {
      if (RG2W != 0)
        RG4W >>= bcon(0x2);
    }
  }
  RG1Blo = vb_00 & lcon(0x1);
  vb_04 = RG1Blo;
  RG1L = unsignedlong(vb_04);
  if (RG1L != 0)
  {
    RG1W = word_at(RG5p, 2);
    RG1UW >>= bcon(0x8);
    RG1L = unsignedlong(RG1W);
    RG1UL >>= bcon(0x1);
    RG1L++;
    RG4W = RG1W;
    RG1W = DM2_RAND16(RG4W) + 1;
    RG4W += RG1W;
  }
  if (RG2UW < mkuw(2))
  {
    //m_83C9:
    if (RG2W != 0)
      return RG4W;
  }
  else
  {
    if (RG2UW > mkuw(2))
    {
      if (RG2UW <= mkuw(lcon(0x3)))
      {
        //m_83E5:
        RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG3W));
        return (RG1Blo & bcon(0x20)) != 0 ? RG4W : 0;
      }
      if (RG2W != lcon(0x7))
        return RG4W;
      //m_83F3:
      RG1W = word_at(RG5p, 2);
      RG1UW >>= bcon(0x8);
      RG1L = unsignedlong(RG1W);
      RG1UL >>= bcon(0x5);
      RG1L = signedlong(DM2_MIN(RG1W, 4));
      RG4L = RG1L;
      RG1L = (DM2_RANDBIT() ? 1 : 0) + RG4L;
      RG4L = signedlong(RG1W);
      RG1L = signedlong(DM2_MAX(1, RG4W));
      RG4L = RG1L;
      if (signedlong(RG62W) != lcon(0x4))
        return RG4W;
      RG4L = unsignedlong(RG1W);
      return DM2_APPLY_CREATURE_POISON_RESISTANCE(unsignedlong(RG3W), RG4L);
    }
    //m_83D3:
    RG4W >>= bcon(0x1);
  }
  //m_83D6:
  if (RG62W != 0)
    return RG4W;
  return jnz_test8(location(RG7p + 2), lcon(0xffffff80)) ? RG4W : 0;
}

// belongs to DM2_PROCEED_TIMERS
void DM2_PROCESS_CLOUD(c_tim* eaxtimp)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG51p;
  c_wlreg _RG61;
  c_tim c38_00;
  i32 vl_0c;
  i16 vw_10;
  i16 parw03;
  i16 parw04;

  c_tim* RG7timp = eaxtimp;

  RG1W = unsignedword(RG1Blo);
  RG1Blo = RG7timp->getxA();
  vw_10 = RG1W;
  RG61W = unsignedword(RG7timp->getyA());
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(RG7timp->getB())));
  RG51p = RG1P;
  RG2L = signedlong(RG61W);
  RG3L = signedlong(vw_10);
  RG1L = (dm2_GET_TILE_VALUE(RG3L, RG2L) & lcon(0xff)) >> 5 & lcon(0xff);
  if (RG1L == lcon(0x4))
  {
    RG4L = RG2L;
    RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(RG3W, RG4W));
    RG4L = unsignedlong(RG1W);
    RG1W = DM2_CALC_CLOUD_DAMAGE(unsignedlong(RG7timp->getB()), RG4L);
    if (RG1W != 0)
    {
      RG2L = signedlong(RG1W);
      RG4L = unsignedlong(RG61W);
      RG1L = unsignedlong(vw_10);
      RG3L = 1;
      DM2_ATTACK_DOOR(RG1L, RG4L, RG2L, RG3L, 0);
    }
  }
  //m_84CB:
  RG4W = word_at(RG51p, 2) & lcon(0x7f);
  RG1L = signedlong(RG4W);
  if (RG1L != lcon(0xe) && RG4W != 0 && RG1L != 2)
  {
    bool skip00072 = false;
    RG1W = ddat.v1d3248;
    if (RG1W == ddat.v1e0282)
    {
      RG4L = unsignedlong(vw_10);
      RG1L = signedlong(ddat.v1e0280);
      if (RG4L == RG1L)
      {
        RG4L ^= RG1L;
        RG4W = RG61W;
        RG1L = signedlong(ddat.v1e027e);
        if (RG4L == RG1L)
        {
          RG1W = DM2_CALC_CLOUD_DAMAGE(unsignedlong(RG7timp->getB()), lcon(0xffff));
          if (RG1W != 0)
          {
            RG1L = unsignedlong(RG1W);
            RG2L = 0;
            DM2_ATTACK_PARTY(RG1L, 0, RG2L);
          }
        }
      }
    }
    //m_8548:
    RG4L = unsignedlong(RG61W);
    RG1L = DM2_GET_CREATURE_AT(unsignedlong(vw_10), RG4L);
    if (RG1W != lcon(0xffffffff))
    {
      RG1L = unsignedlong(RG1W);
      vl_0c = RG1L;
      RG1L = DM2_1c9a_0958(RG1L);
      if (RG1L == 0)
      {
        RG1W = RG7timp->getB();
        RG4L = vl_0c;
        RG1W = DM2_CALC_CLOUD_DAMAGE(RG1L, RG4L);
        if (RG1W != 0)
        {
          RG1L = unsignedlong(RG1W);
          i32 parl02 = RG1L;
          RG2L = signedlong(RG61W);
          RG4L = signedlong(vw_10);
          RG3L = lcon(0x200d);
          RG1L = vl_0c;
          DM2_ATTACK_CREATURE(RG1L, RG4L, RG2L, RG3L, lcon(0x64), parl02);
        }
      }
    }
    //m_85A3:
    RG1W = word_at(RG51p, 2) & lcon(0x7f);
    if (RG1UW < mkuw(lcon(0x28)))
    {
      //m_85BE:
      if (RG1W == lcon(0x7))
      {
        //m_8637:
        RG1W = word_at(RG51p, 2);
        RG1Blo = 0;
        if (RG1UW >= mkuw(lcon(0x600)))
        {
          RG1W = word_at(RG51p, 2);
          RG4L = RG1L;
          RG4UW >>= bcon(0x8);
          RG4L -= 3;
          RG4W = unsignedword(RG4Blo);
          RG1W = unsignedword(RG1Blo);
          RG4L <<= bcon(0x8);
          RG61L = RG1L | RG4L;
          mov16(location(RG51p + 2), RG61W);
          skip00072 = true;
        }
      }
    }
    else
    {
      if (RG1UW <= mkuw(lcon(0x28)))
      {
        //m_860D:
        RG1W = word_at(RG51p, 2);
        RG1Blo = 0;
        if (RG1UW > mkuw(lcon(0x3700)))
        {
          RG1W = word_at(RG51p, 2);
          RG4L = RG1L;
          RG4UW >>= bcon(0x8);
          RG4L -= lcon(0x28);
          RG4W = unsignedword(RG4Blo);
          RG1W = unsignedword(RG1Blo);
          RG4L <<= bcon(0x8);
          RG3L = RG1L | RG4L;
          mov16(location(RG51p + 2), RG3W);
          skip00072 = true;
        }
      }
      else
      {
        if (RG1W == lcon(0x64))
        {
          //m_85C9:
          RG4L = signedlong(RG61W);
          put16(parw04, RG4W);
          RG4L = signedlong(vw_10);
          put16(parw03, RG4W);
          RG4L = unsignedlong(RG1Blo);
          RG3L = lcon(0xfe);
          RG2L = lcon(0x81);
          RG1L = lcon(0xd);
          DM2_QUEUE_NOISE_GEN2(RG1Blo, RG4Blo, RG2Blo, RG3Blo, parw03, parw04, 1, lcon(0x6c), lcon(0xc8));
          RG1W = word_at(RG51p, 2) & lcon(0x7f);
          RG1L++;
          RG1W &= lcon(0x7f);
          and8(location(RG51p + 2), lcon(0xffffff80));
          or16(location(RG51p + 2), RG1W);
          skip00072 = true;
        }
      }
    }

    if (skip00072)
    {
      //m_865F:
      c38_00 = *RG7timp;
      c38_00.incdata();
      DM2_QUEUE_TIMER(&c38_00);
      return;
    }
  }
  //m_8672:
  RG1W = ddat.v1d3248;
  if (RG1W == ddat.v1e0266)
  {
    RG1W = word_at(RG51p, 2) & lcon(0x7f);
    if (RG1W == 2 || jnz_test8(location(RG51p + 2), lcon(0x7f)) || RG1W == lcon(0x30))
      ddat.v1e0390.b_00 |= 1;
  }
  RG3L = signedlong(RG61W);
  DM2_CUT_RECORD_FROM(mk_record(unsignedlong(RG7timp->getB())), NULL, vw_10, RG3W);
  DM2_DEALLOC_RECORD(unsignedlong(RG7timp->getB()));
}
