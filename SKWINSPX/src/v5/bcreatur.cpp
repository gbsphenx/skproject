#include <stdio.h> // NULL

#include <types.h>
#include <bcreatur.h>
#include <skrandom.h>
#include <xtypes.h>
#include <SK0350.h>
#include <skcloud.h>
#include <sktimprc.h>
#include <sfx.h>
#include <bitem.h>
#include <util.h>
#include <skmap.h>
#include <mdata.h>
#include <skgdtqdb.h>
#include <dm2data.h>
#include <skmovrec.h>
#include <skhero.h>
#include <SK1C9A.h>
#include <skai.h>
#include <skmove.h>
#include <skevents.h>


#include <dm2debug.h>
#include <dosvideo.h>

#include <emu.h>
#include <regs.h>

c_creature* creatures; // was v1e089c

void init_creatures(void)
{
	SPX_DEBUG_PRINT("CREATURES:INIT\n");
DOS_VIDEO_STATIC_TEST();
  creatures = NULL;
}

// was SKW_1c9a_198e
i16 DM2_APPLY_CREATURE_POISON_RESISTANCE(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  if (RG4W == 0)
    return 0;
  RG1P = DM2_QUERY_CREATURE_AI_SPEC_FROM_TYPE(unsignedlong(RG1W));
  RG1W = word_at(RG1P, lcon(0x18));
  RG1L = unsignedlong(RG1W) >> bcon(0x8);
  RG1L &= lcon(0xf);
  RG2L = unsignedlong(RG1W);
  if (RG2L == lcon(0xf))
    return 0;
  RG1L = signedlong(DM2_RANDDIR());
  RG3L = unsignedlong(RG1W);
  RG1L = unsignedlong(RG4W);
  RG4L = (RG1L + RG3L) << 3;
  RG2L += 2;
  return CUTX16(RG4L / RG2L);
}

void DM2_ROTATE_CREATURE(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;

  RG3L = RG1L;
  RG5L = RG4L;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
  RG4P = RG1P;
  RG1W = word_at(RG1P, lcon(0xe));
  RG1L <<= bcon(0x6);
  RG1UW >>= 14;
  if (RG5L == 0)
  {
    RG2L += RG1L;
    RG2W &= 0x3;
  }
  RG5L = RG2L - RG1L & lcon(0x3);
  RG2W &= 0x3;
  and8(location(RG4P + lcon(0xf)), lcon(0xfffffffc));
  RG2L <<= bcon(0x8);
  or16(location(RG4P + lcon(0xe)), RG2W);
  RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG3W));
  if ((RG1Blo & bcon(0x1)) == 0)
    return;
  RG1P = RG4P + 2;
  RG3L = signedlong(RG5W);
  //m_39FFC:
  for (;;)
  {
    if (word_at(RG1P) == lcon(0xfffffffe))
      return;
    RG4L = unsignedlong(word_at(RG1P));
    RG2L = ((RG4L >> bcon(0xe)) + RG3L & lcon(0x3)) << bcon(0xe);
    RG4Bhi &= lcon(0x3f);
    RG4L |= RG2L;
    mov16(location(RG1P), RG4W);
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
  }
  FOREND
}

// was SKW_1c9a_0732 (new)
i16 DM2_CREATURE_CAN_HANDLE_ITEM_IN(i16 eaxw, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;

  RG5w = RG1W;
  RG3L = RG4L;
  //m_1FFD5:
  for (;;)
  {
    bool skip00530 = false;
    if (RG3W == lcon(0xfffffffe))
      return RG3W;
    RG1L = RG3L;
    RG1Blo ^= RG3Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    RG4L = RG1L;
    RG1L = signedlong(RG1W);
    if (RG1L > lcon(0x4) && RG1L < lcon(0xe))
      skip00530 = true;
    else
    {
      RG1L = signedlong(RG4W);
      if (RG1L == lcon(0x9))
        skip00530 = true;
    }

    if (skip00530)
    {
      bool skip00531 = false;
      //m_1FFFF:
      RG1L = unsignedlong(RG2Blo);
      if (RG1L == lcon(0xff))
        skip00531 = true;
      else
      {
        RG4L = unsignedlong(RG3W) >> bcon(0xe);
        if (RG1L == RG4L)
          skip00531 = true;
      }

      if (skip00531)
      {
        //m_20016:
        RG4L = signedlong(RG5w);
        RG1L = DM2_CREATURE_CAN_HANDLE_IT(unsignedlong(RG3W), RG4L);
        if (RG1L != 0)
          return RG3W;
      }
    }
    //m_20027:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG3UW)));
    RG3L = RG1L;
  }
  FOREND
}

i32 DM2_WOUND_CREATURE(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG5p;
  i8* RG61p;
  c_ql32 vql_00;
  c_ol32 vol_04; // poke 32 peek 8 - getbyte1
  i32 vl_08;
  i32 vl_0c;
  i16 vw_10;
  i16 parw02;
  i16 parw03;
  bool flag;

  vql_00.poke32(RG1L);
  RG5p = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG61p = s350.v1e0552;
  vl_0c = 0;
  RG1L = unsignedlong(byte_at(RG61p, 2));
  if (RG1L == lcon(0xff))
    return vl_0c;
  RG1W = word_at(RG61p) & lcon(0x1);
  RG1L = unsignedlong(RG1W);
  flag = RG1L != 0;
  vl_08 = RG1L;
  if (!flag)
  {
    RG1Blo = byte_at(RG5p, lcon(0x4));
    RG1L = table1d607e[DM2_QUERY_GDAT_CREATURE_WORD_VALUE(RG1L, lcon(0x1)) & lcon(0xffff)].ul_00;
    vol_04.poke32(RG1L);
    if ((vol_04.peek8() & bcon(0x4)) == 0)
    {
      vw_10 = ddat.v1d3248;
      RG1W = word_at(RG5p, lcon(0xc));
      RG1UW >>= bcon(0xa);
      RG1L = signedlong(RG1W);
      DM2_CHANGE_CURRENT_MAP_TO(RG1L);
      RG1L = timdat.gametick + 1;
      i32 parl00 = RG1L;
      RG1W = word_at(RG5p, lcon(0xc));
      RG1L <<= bcon(0x6);
      _RG1.ushiftr11();
      RG4L = unsignedlong(RG1W);
      RG1W = word_at(RG5p, lcon(0xc)) & lcon(0x1f);
      RG1L = unsignedlong(RG1W);
      DM2_INVOKE_MESSAGE(RG1L, RG4L, 0, lcon(0x1), parl00);
      DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_10));
    }
  }
  //m_214AC:
  RG1W = word_at(RG5p, lcon(0x6));
  if (RG1UW <= mkuw(vql_00.peek16()))
  {
    //m_21592:
    mov16(location(RG5p + lcon(0x6)), lcon(0x1));
    if (vl_08 == 0)
    {
      if ((vol_04.getbyte1() & bcon(0x8)) != 0)
      {
        RG1L = DM2_1c9a_17c7(unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), unsignedlong(s350.v1e0571));
        if (RG1L == 0)
          return vl_0c;
      }
      if ((vol_04.getbyte1() & bcon(0x8)) != 0)
        ddat.glbEndCounter = 0xA0;	// lcon(0xa0), that's more than 0x18 for V4
      DM2_ai_13e4_0360(unsignedlong(s350.v1e054c), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), lcon(0x13), lcon(0x1));
      return vl_0c;
    }
    vl_0c = 1;
    DM2_DELETE_CREATURE_RECORD(unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), 0, lcon(0x1));
    return vl_0c;
  }
  if (vl_08 == 0 && (vol_04.getbyte1() & bcon(0xffffff80)) != 0)
  {
    bool skip00553 = false;
    RG1L = DM2_RAND();
    RG1W &= lcon(0x7);
    RG1L = unsignedlong(RG1W);
    if (RG1L == 0)
      skip00553 = true;
    else
    {
      bool skip00551 = false;
      RG1L = unsignedlong(word_at(RG61p, lcon(0x4)));
      RG2L = lcon(0x64);
      RG1L = 3 * RG1L;
      RG4L = RG1L % RG2L;
      RG1L /= RG2L;
      RG2L = unsignedlong(vql_00.peek16());
      if (RG2L > RG1L)
        skip00551 = true;
      else
      {
        RG3L = lcon(0x64);
        RG1L = 5 * unsignedlong(word_at(RG5p, lcon(0x6)));
        RG4L = RG1L % RG3L;
        RG1L /= RG3L;
        if (RG2L > RG1L)
          skip00551 = true;
      }

      if (skip00551)
      {
        bool skip00552 = false;
        //m_21522:
        RG1W = word_at(RG5p, lcon(0xa)) & lcon(0x8);
        RG1L = unsignedlong(RG1W);
        if (RG1L == 0)
          skip00552 = true;
        else
        {
          RG1L = DM2_RANDBIT() ? 1 : 0;
          if (RG1W == 0)
            skip00552 = true;
        }

        if (skip00552)
        {
          //m_2153B:
          RG1L = signedlong(DM2_RANDDIR());
          if (RG1W == 0)
            skip00553 = true;
        }
        else
          skip00553 = true;
      }
    }

    if (skip00553)
    {
      //m_21545:
      put16(parw03, unsignedword(s350.v1e0562.getyA()));
      put16(parw02, unsignedword(s350.v1e0562.getxA()));
      RG3L = lcon(0x46);
      RG1L = DM2_RANDBIT() ? 1 : 0;
      RG1Blo = RG1W != wcon(0x0) ? 1 : 0;
      RG1W = unsignedword(RG1Blo);
      RG1L += lcon(0x9);
      RG2L = unsignedlong(RG1Blo);
      DM2_QUEUE_NOISE_GEN1(lcon(0xf), byte_at(RG5p, lcon(0x4)), RG2Blo, RG3W, lcon(0x80), parw02, parw03, lcon(0x1));
    }
  }

  //m_21586:
  RG1L = vql_00.peek32();
  sub16(location(RG5p + lcon(0x6)), RG1W);
  return vl_0c;
}

// stacklen was 0x34
void DM2_ATTACK_CREATURE(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1)
{
  c_nreg _RG4; RG4L = edxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG1; RG1L = eaxl;
  i8* RG5p;
  i8* RG61p;
  i8* RG62p;
  c_lreg _RG7;
  c_ol32 vol_00; // poke 16 32 peek 32 and16
  c_ql32 vql_04;
  c_ql32 vql_08;
  i16 vw_0c;
  i32 vl_10;
  i32 vl_14;
  i32 vl_18;
  unk* xp_1c;
  i16 vw_20;
  bool flag;

  vw_0c = RG1W; // truncated
  vql_08.poke32(RG4L);
  vql_04.poke32(RG2L);
  vol_00.poke32(RG3L);
  RG4L = 0;
  vl_14 = RG4L;
  if (RG1W == lcon(0xffffffff))
  {
    RG4W = RG2W;
    RG1L = DM2_GET_CREATURE_AT(unsignedlong(vql_08.peek16()), RG4L);
    vw_0c = RG1W; // truncated
    if (RG1W == lcon(0xffffffff))
      return;
  }
  RG1L = vol_00.peek32();
  RG1W &= lcon(0x4000);
  RG7L = unsignedlong(RG1W);
  if (RG7L != 0)
  {
    vol_00.and16(lcon(0xbfff)); // adj
    RG1L = DM2_RANDBIT() ? 1 : 0;
    if (RG1W != 0)
      RG7L = 0;
  }
  RG1L = vol_00.peek32();
  RG1W &= lcon(0x2000);
  RG1L = unsignedlong(RG1W);
  flag = RG1L == 0;
  vl_10 = RG1L;
  if (!flag)
    vol_00.and16(lcon(0xdfff)); // adj
  RG3L = unsignedlong(vw_0c);
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
  RG61p = RG1P;
  RG5p = RG1P;
  RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG61p, lcon(0x4)))));
  xp_1c = UPCAST(unk, RG1P);
  RG1W = word_at(RG1P) & lcon(0x1);
  RG1L = unsignedlong(RG1W);
  vl_18 = RG1L;
  if (byte_at(RG61p, lcon(0x5)) == lcon(0xffffffff))
  {
    if (RG1L == 0)
      return;
    RG2L = unsignedlong(vql_04.peek16());
    DM2_ALLOC_CAII_TO_CREATURE(RG3L, unsignedlong(vql_08.peek16()), RG2L);
  }
  RG4L = unsignedlong(byte_at(RG5p, lcon(0x5)));
  RG1L = 34 * RG4L;
  RG62p = DOWNCAST(c_creature, creatures) + RG1L;
  RG1L = argl1;
  RG2W = word_at(RG62p, lcon(0x14));
  RG2L += RG1L;
  mov16(location(RG62p + lcon(0x14)), RG2W);
  argl1 = RG2L;
  if (vl_18 == 0 && argw0 > wcon(0x0))
  {
    RG1W = word_at(RG5p, lcon(0xa)) & lcon(0x4);
    RG1L = unsignedlong(RG1W);
    if (RG1L == 0)
    {
      bool skip00245 = false;
      RG1W = RG2W;
      if (RG1L > lcon(0x1e))
        skip00245 = true;
      else
      {
        bool skip00244 = false;
        if (RG1L <= lcon(0x4))
          skip00244 = true;
        else
        {
          RG1L = signedlong(DM2_RANDDIR());
          if (RG1W != 0)
            skip00244 = true;
        }

        if (skip00244)
        {
          //m_15036:
          RG2L = 0;
          RG2W = word_at(DOWNCAST(unk, xp_1c), lcon(0x4));
          RG3L = unsignedlong(mkuw(argl1)); // truncated TODO check if arg is always word
          RG1L = 100 * RG3L / RG2L;
          if (mkul(RG1L) > mkul(lcon(0xf)))
            skip00245 = true;
        }
        else
          skip00245 = true;
      }

      if (skip00245)
      {
        //m_1505F:
        or8(location(RG5p + lcon(0xa)), lcon(0x4));
        RG7L = 1;
      }
    }

    //m_15068:
    if (RG7L != 0)
    {
      RG1L = DM2_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(byte_at(RG5p, lcon(0x4))), lcon(0x1)) & lcon(0xffff);
      if ((table1d607e[RG1L].uc[0] & ubcon(0x80)) == 0)
      {
        RG1L = DM2_RANDBIT() ? 1 : 0;
        if (RG1W != 0)
        {
          bool skip00248 = false;
          bool skip00247 = false;
          RG1L = signedlong(DM2_CALC_VECTOR_DIR(vql_08.peek16(), vql_04.peek16(), ddat.v1e0270, ddat.v1e0272));
          RG4L = RG1L;
          RG1W = word_at(RG5p, lcon(0xa)) & lcon(0x8);
          RG1L = unsignedlong(RG1W);
          if (RG1L == 0)
            skip00247 = true;
          else
          {
            RG1L = signedlong(DM2_RANDDIR());
            if (RG1W == 0)
              skip00247 = true;
          }

          if (skip00247)
          {
            //m_150E0:
            RG1W = word_at(RG5p, lcon(0xe));
            RG1L <<= bcon(0x6);
            RG1UW >>= 14;
            if (RG4W != RG1W)
            {
              RG1L = signedlong(DM2_RANDDIR());
              if (RG1W == 0)
                skip00248 = true;
            }
          }
          else
            skip00248 = true;

          if (skip00248)
          {
            //m_150FA:
            RG4L += 2;
            RG4W &= 0x3;
          }

          bool skip00251 = false;
          //m_15102:
          RG2L = RG4L + 2;
          RG2W &= 0x3;
          RG1W = word_at(RG5p, lcon(0xe));
          RG1L <<= bcon(0x6);
          RG1UW >>= 14;
          if (RG1W != RG2W)
          {
            //m_15133:
            if (RG1W == RG4W)
            {
              //m_15146:
              RG1L = signedlong(DM2_RANDDIR());
              if (RG1W != 0)
                //m_15164:
                RG4L = lcon(0xffffffff);
              else
              {
                RG1L = DM2_RANDBIT() ? 1 : 0;
                RG1Blo = RG1W == wcon(0x0) ? 1 : 0;
                RG4L = unsignedlong(RG1Blo) + lcon(0x6);
              }
              skip00251 = true;
            }
            else
            {
              RG4L--;
              RG4W &= 0x3;
              RG1Blo = RG1W == RG4W ? 1 : 0;
            }
          }
          else
          {
            RG1L = DM2_RANDBIT() ? 1 : 0;
            RG1Blo = RG1W != wcon(0x0) ? 1 : 0;
          }

          if (!skip00251)
          {
            //m_15127:
            RG1L = unsignedlong(RG1Blo) + lcon(0x6);
            RG4L = RG1L;
          }

          //m_15169:
          RG3L = signedlong(RG4W);
          if (RG3L != lcon(0xffffffff))
            DM2_ai_13e4_0360(unsignedlong(vw_0c), signedlong(vql_08.peek16()), signedlong(vql_04.peek16()), RG3L, 0);
        }
      }
    }

    //m_1518D:
    RG1W = DM2_RAND16(100);
    RG1Blo = argw0 > RG1W ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    flag = RG1L == 0;
    vl_14 = RG1L;
    if (!flag)
    {
      RG1L = vol_00.peek32();
      RG1W &= lcon(0x8000);
      vw_20 = RG1W; // truncated
      RG3L = vol_00.peek32();
      RG1L = lcon(0x1) << RG3Blo;
      vol_00.poke16(RG1W);
      if (vw_20 == 0)
      {
        //m_151E0:
        RG1L = vol_00.peek32();
        or16(location(RG5p + lcon(0xa)), RG1W);
      }
      else
      {
        RG1L = ~vol_00.peek32();
        and16(location(RG5p + lcon(0xa)), RG1W);
      }
    }
  }

  bool skip00256 = false;
  //m_151E7:
  if (vl_18 == 0 && vl_10 != 0 && argw0 == 0)
    skip00256 = true;
  else
  {
    bool skip00257 = false;
    if (vl_14 == 0)
      skip00257 = true;
    else
    {
      bool skip00255 = false;
      bool skip00254 = false;
      if (vw_20 != 0)
        skip00254 = true;
      else
      {
        RG1L = vol_00.peek32();
        RG1W &= lcon(0x40);
        RG1L = unsignedlong(RG1W);
        if (RG1L == 0)
          skip00254 = true;
      }

      if (skip00254)
      {
        //m_15222:
        if (vl_10 == 0)
          skip00257 = true;
        else
          skip00255 = true;
      }
      else
        skip00255 = true;

      if (skip00255)
      {
        //m_15229:
        RG1L = signedlong(byte_at(RG62p, lcon(0x1a)));
        RG2Blo = table1d613a[RG1L];
        RG1Blo = RG2Blo & lcon(0x10);
        RG1L = unsignedlong(RG1Blo);
        if (RG1L == 0)
          skip00256 = true;
        else
        {
          RG1L = DM2_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(byte_at(RG5p, lcon(0x4))), lcon(0x1)) & lcon(0xffff);
          if ((table1d607e[RG1L].uwarr_00[0] & uwcon(0x410)) == 0)
            skip00257 = true;
          else
          {
            RG1Blo = RG2Blo & 2;
            RG1L = unsignedlong(RG1Blo);
            if (RG1L == 0)
              skip00257 = true;
            else
              skip00256 = true;
          }
        }
      }
    }

    if (skip00257)
      //m_15272:
      RG1L = 0;
  }

  if (skip00256)
    //m_1526B:
    RG1L = 1;

  //m_15274:
  RG4L = signedlong(byte_at(RG62p, lcon(0x1a)));
  if (RG4L == lcon(0x13))
    return;
  if (RG1L == 0)
  {
    RG1L = argl1;
    if (RG1UW < mkuw(word_at(RG5p, lcon(0x6))))
      return;
  }
  DM2_1c9a_0db0(unsignedlong(vw_0c));
  DM2_1c9a_0cf7(unsignedlong(vql_08.peek16()), unsignedlong(vql_04.peek16()));
}

i32 DM2_CREATURE_ATTACKS_PLAYER(unk* xeaxp, i32 edxl)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG51l;
  c_wlreg _RG5; // TODO: maybe more
  i8* RG61p = xeaxp;
  i8* RG62p;
  i32 vl_00;
  i32 vl_04;
  i16 vw_08;
  i16 vw_0c;
  i16 parw00;
  i16 parw01;
  bool skip00075 = false;

  vw_0c = RG4W;
  RG1L = RG4L;
  if (RG1UW >= mkuw(party.heros_in_party))
    return 0;
  RG51l = unsignedlong(RG1W);
  c_hero* hero = &party.hero[RG51l];
  if (hero->curHP == 0)
    return 0;
  RG5L = 0;
  RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
  RG1W = word_at(RG1P, lcon(0xc));
  RG1UW >>= bcon(0xc);
  RG1L = 2 * unsignedlong(RG1W);
  vl_04 = RG1L;
  RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG61p, lcon(0x4)))));
  RG62p = RG1P;
  RG4W = unsignedword(RG4Blo);
  RG4Blo = byte_at(RG1P, lcon(0x1c));
  vw_08 = RG4W; // truncated
  if (ddat.savegames1.b_02 == 0 || jnz_test8(location(RG1P + lcon(0x1)), lcon(0x4)))
  {
    //m_888E:
    if (jz_test8(location(RG62p + lcon(0x1)), lcon(0x8)))
    {
      //m_8898:
      RG2W = ddat.v1e0286; // TODO: gets extended w/o conversion now
      RG2L = 2 * RG2L;
    }
    else
      RG2L = 0;
  }
  else
    RG2L = lcon(0x10);
  //m_88A1:
  RG4W = unsignedword(RG4Blo);
  RG4Blo = byte_at(RG62p, lcon(0x8));
  RG1L = unsignedlong(vw_08);
  if (RG1L != lcon(0x9))
  {
    //m_88C5:
    if (RG1L == lcon(0x8))
      RG4L = lcon(0xff);
  }
  else
    RG4L = signedlong(DM2_MIN(wcon(0xff), 2 * RG4W));
  //m_88CF:
  if (ddat.v1e0238 != 0)
    skip00075 = true;
  else
  {
    RG1L = unsignedlong(RG4W);
    if (RG1L == lcon(0xff))
      skip00075 = true;
    else
    {
      bool skip00074 = false;
      RG1L = DM2_RAND();
      RG1W &= lcon(0x1f);
      RG1L += RG4L;
      RG1L += vl_04;
      RG1L = signedlong(RG1W);
      RG4L = unsignedlong(RG2W) + RG1L;
      RG1L = mkul(DM2_USE_DEXTERITY_ATTRIBUTE(vw_0c) & lcon(0xffff));
      RG4L -= 16;
      if (RG1L < RG4L)
        skip00074 = true;
      else
      {
        RG1L = signedlong(DM2_RANDDIR());
        if (RG1W == 0)
          skip00074 = true;
      }

      if (skip00074)
      {
        //m_8921:
        RG4L = lcon(0x3c);
        if (!hero->use_luck(RG4W))
          skip00075 = true;
      }
    }
  }

  if (skip00075)
  {
    bool skip00076 = false;
    //m_8935:
    RG1L = DM2_RAND();
    RG2L = RG1L;
    RG4L = RG1L;
    RG4Bhi ^= RG1Bhi;
    RG4Blo &= lcon(0x70);
    RG4L = unsignedlong(RG4W);
    if (RG4L == 0)
      skip00076 = true;
    else
    {
      RG4W = word_at(RG62p, lcon(0x1a));
      if (RG4W == 0)
        skip00076 = true;
      else
      {
        RG2Bhi ^= RG1Bhi;
        RG2Blo &= lcon(0xf);
        if (RG2W == 0)
          RG2L = 1;
        RG3L = 0;
        //m_896C:
        for (;;)
        {
          RG1L = unsignedlong(RG3W);
          if (RG1L < lcon(0x3))
          {
            RG5L = RG4L & lcon(0xf) & lcon(0xffff);
            RG1L = unsignedlong(RG2W);
            if (RG1L > RG5L)
            {
              //m_8967:
              RG4UW >>= bcon(0x4);
              RG3L++;
              continue;
            }
          }
          break;
        }
        //m_898A:
        RG1L = unsignedlong(RG3W);
        RG3W = unsignedword(RG3Blo);
        RG3Blo = table1d26f8[RG1L];
      }
    }

    if (skip00076)
    {
      //m_8999:
      RG3L = RG2L;
      RG3Bhi ^= RG2Bhi;
      RG3Blo &= 0x1;
      RG3L++;
    }
    bool skip00078 = false;
    //m_89A1:
    RG5W = unsignedword(byte_at(RG62p, lcon(0x6)));
    RG1L = DM2_RAND();
    RG1W &= lcon(0xf);
    RG1L += vl_04;
    RG4L = signedlong(RG1W);
    RG1L = signedlong(DM2_MIN(unsignedword(byte_at(RG62p, lcon(0x6))), RG4W));
    RG5L += RG1L;
    RG1L = unsignedlong(vw_08);
    if (RG1L == lcon(0x8))
      skip00078 = true;
    else
    {
      RG1W = 2 * DM2_QUERY_PLAYER_SKILL_LV(vw_0c, 7, 1);
      RG5W -= RG1W;
      RG1L = signedlong(RG5W);
      if (RG1L > lcon(0x1))
        skip00078 = true;
      else
      {
        RG1L = DM2_RANDBIT() ? 1 : 0;
        if (RG1W == 0)
        {
          //m_8A01:
          RG5L = signedlong(DM2_RANDDIR()) + 2;
          skip00078 = true;
        }
        else
          RG5L = 0;
      }
    }

    if (skip00078)
    {
      //m_8A0B:
      RG5W >>= bcon(0x1);
      RG1W = DM2_RAND16(RG5W);
      RG4W = RG1W;
      RG1L = signedlong(DM2_RANDDIR()) + signedlong(RG4L);
      RG5L += RG1L;
      RG1W = DM2_RAND16(RG5W);
      RG5W += RG1W;
      RG5W >>= 2;
      RG1L = signedlong(DM2_RANDDIR()) + 1;
      RG5L += RG1L;
      RG1L = DM2_RANDBIT() ? 1 : 0;
      if (RG1W != 0)
      {
        RG1W = DM2_RAND16(RG5W / 2 + 1) - 1;
        RG5W -= RG1W;
      }
      RG1L = unsignedlong(vw_08);
      RG2L = unsignedlong(RG3W);
      RG4L = signedlong(RG5W);
      RG5L = signedlong(vw_0c);
      vl_00 = RG5L;
      RG1W = DM2_WOUND_PLAYER(RG5L, RG4L, RG2L, RG1L);
      RG5L = RG1L;
      if (RG1W != 0)
      {
        RG1L = signedlong(ddat.v1e0272);
        put16(parw01, RG1W);
        RG1L = signedlong(ddat.v1e0270);
        put16(parw00, RG1W);
        RG4L = unsignedlong(hero->herotype);
        RG3L = lcon(0xfe);
        RG2L = lcon(0x82);
        DM2_QUEUE_NOISE_GEN2(lcon(0x16), RG4Blo, RG2Blo, RG3Blo, parw00, parw01, 2, lcon(0x69), lcon(0xc8));
        RG2W = unsignedword(RG2Blo);
        RG2Blo = byte_at(RG62p, lcon(0x7));
        if (RG2W != 0)
        {
          RG1L = DM2_RANDBIT() ? 1 : 0;
          if (RG1W != 0)
          {
            RG2L = unsignedlong(RG2W);
            RG4L = 4;
            RG1L = hero->get_adj_ability2((e_ability)RG4W, RG2W);
            if (RG1W != 0)
            {
              RG4L = unsignedlong(RG1W);
              DM2_PROCESS_POISON(vl_00, RG4L);
            }
          }
        }
      }
    }
  }

  //m_8AF0:
  if (ddat.v1e0238 != 0)
    DM2_RESUME_FROM_WAKE();
  return RG5L;
}

// was SKW_1c9a_166f
i32 DM2_CREATURE_ATTACKS_CREATURE(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  i16 RG6w;
  i8* RG7p;
  i32 vl_00;
  i32 vl_04;
  c_ql32 vql_08;
  i16 vw_0c;

  RG6w = RG1W;
  RG3L = RG4L;
  RG4L = unsignedlong(RG3W);
  RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG6w), RG4L);
  vw_0c = RG1W; // truncated
  if (RG1W == lcon(0xffffffff))
    return lcon(0xffffffff);
  //m_211F5:
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
  RG4P = s350.v1e0552;
  RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG1P, lcon(0x4)))));
  RG2P = RG1P;
  RG7p = RG1P;
  RG1L = unsignedlong(byte_at(RG2P, lcon(0x8)));
  if (RG1L == lcon(0xff))
    return 0;
  RG1L = DM2_RAND();
  RG1W &= lcon(0x1f);
  vl_04 = RG1L;
  RG1Blo = byte_at(RG4P, lcon(0x8));
  vql_08.poke32(RG1L);
  RG1L = vl_04 + vql_08.peek32();
  vql_08.poke32(RG1L);
  RG1L = DM2_RAND();
  RG1W &= lcon(0x1f);
  RG2Blo = byte_at(RG2P, lcon(0x8));
  RG2W = unsignedword(RG2Blo);
  RG2L += RG1L;
  if (RG2W > vql_08.peek16())
  {
    RG1L = signedlong(DM2_RANDDIR());
    if (RG1W != 0)
      return 0;
  }
  RG1L = DM2_RAND();
  RG1W &= lcon(0x1f);
  RG2L = unsignedlong(RG1W);
  RG1L = (unsignedlong(byte_at(RG7p, 2)) + RG2L) / 8;
  vl_00 = RG1L;
  RG5L = 0;
  RG5W = unsignedword(byte_at(RG4P, lcon(0x6)));
  RG1L = DM2_RAND();
  RG1W &= lcon(0xf);
  RG2L = signedlong(RG1W);
  RG1L = signedlong(DM2_MIN(unsignedword(byte_at(RG4P, lcon(0x6))), RG2W));
  RG5L += RG1L;
  RG5L -= vl_00;
  RG1L = signedlong(RG5W);
  if (RG1L <= lcon(0x1))
  {
    RG1L = DM2_RANDBIT() ? 1 : 0;
    if (RG1W != 0)
      return 0;
    //m_212BE:
    RG5L = signedlong(DM2_RANDDIR()) + 2;
  }
  //m_212C8:
  RG1W = DM2_RAND16(RG5W);
  RG4W = RG1W;
  RG1L = signedlong(DM2_RANDDIR()) + signedlong(RG4W);
  RG5L += RG1L;
  RG1W = DM2_RAND16(RG5W);
  RG5W += RG1W;
  RG5W >>= 2;
  RG1L = signedlong(DM2_RANDDIR()) + 1;
  RG5L += RG1L;
  RG1L = DM2_RANDBIT() ? 1 : 0;
  if (RG1W != 0)
  {
    RG1W = DM2_RAND16(RG5W / 4 + 1);
    RG5W -= RG1W;
  }
  RG1L = unsignedlong(RG5W);
  i32 parl01 = RG1L;
  RG2L = signedlong(RG3W);
  RG4L = signedlong(RG6w);
  DM2_ATTACK_CREATURE(unsignedlong(vw_0c), RG4L, RG2L, 2, lcon(0x3c), parl01);
  return RG5L;
}

i32 DM2_CREATURE_ATTACKS_PARTY(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5;
  i8* RG51p;
  i16 RG6w;
  c_lreg _RG7;
  i8 vba_00[0x4];
  unk* xp_04;
  i32 vl_08;
  i16 vw_0c;
  i16 vw_10;
  i16 vw_14;
  c_o32 vo_18; // poke16 peek 16 32 inc16
  i16 vw_1c;
  bool skip00368 = false;

  xp_04 = DOWNCAST(c_creature, s350.creatures);
  RG51p = s350.v1e0552;
  vw_14 = ddat.v1e08a6;
  vw_10 = ddat.v1e08a4;
  RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG1W = word_at(RG1P, lcon(0xe));
  RG1L <<= bcon(0x6);
  RG1UW >>= 14;
  vw_0c = RG1W;
  RG1P = DOWNCAST(unk, xp_04);
  if (byte_at(RG1P + lcon(0x20)) == lcon(0x6))
  {
    RG4L = unsignedlong(vw_0c) + 2;
    vw_0c = RG4W;
    RG2L = RG4L;
    RG2Bhi ^= RG4Bhi;
    RG2Blo &= 0x3;
    vw_0c = RG2W;
  }
  RG1L = unsignedlong(s350.v1e0562.getxA());
  RG3W = vw_14;
  if (RG1W != RG3W)
    skip00368 = true;
  else
  {
    RG1L ^= RG3L;
    RG1Blo = s350.v1e0562.getyA();
    if (RG1W != vw_10)
      skip00368 = true;
    else
      RG1L = 1;
  }

  if (skip00368)
    //m_1A4F4:
    RG1L = 0;
  bool skip00370 = false;
  //m_1A4F6:
  RG7L = RG1L;
  if (RG1L == 0)
  {
    RG1L = signedlong(DM2_ABS(s350.v1e0562.getxA() - vw_14));
    RG4L = signedlong(RG1W);
    RG1L = signedlong(DM2_ABS(s350.v1e0562.getyA() - vw_10)) + RG4L;
    if (RG1L > lcon(0x1))
      return lcon(0x1);
    RG3L = signedlong(vw_10);
    RG1L = signedlong(DM2_CALC_VECTOR_DIR(unsignedword(s350.v1e0562.getxA()), unsignedword(s350.v1e0562.getyA()), vw_14, RG3W) & lcon(0xffff));
    RG4L = signedlong(vw_0c);
    if (RG1L != RG4L && jz_test8(location(RG51p), lcon(0x4)))
      return lcon(0x1);
  }
  RG1W = ddat.v1d3248;
  if (RG1W != ddat.v1e08d6)
    skip00370 = true;
  else
  {
    RG1W = vw_14;
    if (RG1W != ddat.v1e08d8)
      skip00370 = true;
    else
    {
      RG1W = vw_10;
      if (RG1W != ddat.v1e08d4)
        skip00370 = true;
      else
      {
        RG1L = timdat.gametick;
        ddat.v1d26a4 = RG1L;
        vl_08 = 0;
        RG1W = word_at(RG51p) & lcon(0x10);
        RG7L = unsignedlong(RG1W);
        RG2L = 0;
        vo_18.poke16(RG2W);
        RG1Blo = 0;
        //m_1A5E8:
        for (;;)
        {
          RG2L = signedlong(RG1W);
          RG4L = unsignedlong(party.heros_in_party);
          if (RG2L >= RG4L)
          {
            RG3W = vo_18.peek16();
            if (RG3W == 0)
              return lcon(0x1);
            if (jz_test8(location(RG51p), lcon(0x8)))
            {
              //m_1A627:
              if (jz_test8(location(RG51p + lcon(0x9)), lcon(0x20)))
                //m_1A636:
                vw_1c = 1;
              else
                vw_1c = 2;
            }
            else
            {
              if (RG7L == 0)
                //m_1A620:
                vw_1c = RG4W;
              else
              {
                RG1W = DM2_RAND16(RG3W) + 1;
                vw_1c = RG1W;
              }
            }
            break;
          }
          //m_1A5C2:
          if (party.hero[RG2L].curHP != 0)
          {
            RG4L = signedlong(vo_18.peek16());
            vba_00[RG4L] = RG1Blo;
            vo_18.inc16();
          }
          RG1L++;
        }
        //m_1A63D:
        RG1L = 0;
        RG1Blo = byte_at(DOWNCAST(unk, xp_04), lcon(0x1c));
        if (RG1L == lcon(0x4))
        {
          RG4L = unsignedlong(s350.v1e0562.getyA());
          RG1L = unsignedlong(s350.v1e0562.getxA());
          RG2L = lcon(0xff);
          RG1L = DM2_move_2c1d_028c(RG1L, RG4L, RG2L);
          RG4L = signedlong(RG1W);
          if (RG4L != lcon(0xffffffff))
          {
            RG1L = 263 * RG4L;
            RG1Blo = party.hero[RG4L].partypos;
            mov8(location(DOWNCAST(unk, xp_04) + lcon(0x1c)), RG1Blo);
          }
        }
        RG6w = unsignedword(byte_at(DOWNCAST(unk, xp_04), lcon(0x1c)));
        RG4L = signedlong(party.heros_in_party);
        RG1L = signedlong(DM2_MIN(vw_1c, RG4W));
        vw_1c = RG1W;
        //m_1A6A8:
        for (;;)
        {
          RG5L = unsignedlong(vw_1c) - 1;
          vw_1c = RG5W;
          if (RG5W == lcon(0xffffffff))
            break;
          if (RG7L == 0)
          {
            //m_1A705:
            RG1W = DM2_GET_PLAYER_AT_POSITION(RG6w);
            RG5L = RG1L;
            RG3L = unsignedlong(s350.v1e0562.getyA());
            RG2L = unsignedlong(s350.v1e0562.getxA());
            RG4L = signedlong(vw_10);
            RG1L = signedlong(DM2_CALC_VECTOR_DIR(vw_14, RG4W, RG2W, RG3W));
            RG4L = signedlong(RG6w);
            RG1L = signedlong(RG1W) + RG4L;
            if ((RG1Blo & bcon(0x1)) == 0)
              //m_1A741:
              RG6w++;
            else
              RG6w--;
            //m_1A742:
            RG6w &= wcon(0x3);
            RG1L = signedlong(RG5W);
            if (RG1L == lcon(0xffffffff))
            {
              RG4L = unsignedlong(s350.v1e0562.getyA());
              RG1L = unsignedlong(s350.v1e0562.getxA());
              RG2L = lcon(0xff);
              RG1L = DM2_move_2c1d_028c(RG1L, RG4L, RG2L);
              RG5L = RG1L;
            }
            RG1L = signedlong(RG5W);
            if (RG1L == lcon(0xffffffff))
              continue;
          }
          else
          {
            RG1W = DM2_RAND16(vo_18.peek16());
            RG5W = RG1W;
            RG1L = signedlong(RG1W);
            if (vba_00[RG1L] < bcon(0x0))
            {
              RG1L = 0;
              //m_1A6EB:
              for (;;)
              {
                RG4W = vo_18.peek16();
                if (RG1W < RG4W)
                {
                  //m_1A6D9:
                  RG5L++;
                  if (RG5W >= RG4W)
                    RG5L = 0;
                  RG4L = signedlong(RG5W);
                  if (vba_00[RG4L] < bcon(0x0))
                  {
                    RG1L++;
                    continue;
                  }
                }
                break;
              }
            }
            //m_1A6F4:
            RG1L = RG5L;
            RG4L = signedlong(RG5W);
            RG5W = signedword(vba_00[RG4L]);
            RG1L = signedlong(RG1W);
            vba_00[RG1L] = lcon(0xffffffff);
          }
          //m_1A774:
          vl_08 = 1;
          RG4L = unsignedlong(RG5W);
          RG1L = DM2_CREATURE_ATTACKS_PLAYER(DOWNCAST(SPX_Creature, s350.v1e054e), RG4L);
          RG2L = RG1L + 1;
          RG4L = signedlong(RG5W);
          c_hero* hero = &party.hero[RG4L];
          RG1L = unsignedlong(hero->b_29);
          if (RG2W > RG1W)
          {
            hero->b_29 = RG2Blo;
            RG3L = signedlong(ddat.v1e08d4);
            RG2L = signedlong(ddat.v1e08d8);
            RG4L = unsignedlong(s350.v1e0562.getyA());
            RG1L = signedlong(DM2_CALC_VECTOR_DIR(unsignedword(s350.v1e0562.getxA()), RG4W, RG2W, RG3W));
            RG1Blo += 2;
            RG1Blo &= 0x3;
            hero->b_28 = RG1Blo;
          }
        }
        //m_1A7E7:
        if (vl_08 == 0)
          return lcon(0x1);
      }
    }
  }

  if (skip00370)
  {
    //m_1A7F7:
    RG1L = (dm2_GET_TILE_VALUE(signedlong(vw_14), signedlong(vw_10)) & lcon(0xff)) >> 5 & lcon(0xff);
    if (RG1L == 4)
    {
      RG1L = unsignedlong(byte_at(RG51p, lcon(0x6))); // BUGFIX was RG53p here
      RG4L = RG1L / 2;
      RG1L += RG4L;
      RG1W = DM2_RAND16(RG1W);
      RG2L = signedlong(RG1W);
      RG4L = unsignedlong(vw_10);
      RG1L = DM2_ATTACK_DOOR(unsignedlong(vw_14), RG4L, RG2L, 0, 0);
      if (RG1W != 0)
        return lcon(0x1);
    }
    if (RG7L != 0)
      return lcon(0x1);
    RG4L = signedlong(vw_10);
    RG1L = DM2_CREATURE_ATTACKS_CREATURE(signedlong(vw_14), RG4L);
    if (RG1W < 0)
      return lcon(0x1);
  }
  bool skip00373 = false;
  //m_1A874:
  RG1L = signedlong(byte_at(DOWNCAST(unk, xp_04), lcon(0x1a)));
  if (RG1L != lcon(0x26))
    return 0;
  RG1L = DM2_GET_CREATURE_WEIGHT(unsignedlong(s350.v1e054c)) & lcon(0xffff);
  if (RG1L > lcon(0x64))
  {
    RG1L = DM2_RAND();
    RG1W &= lcon(0xf);
    RG1L = unsignedlong(RG1W);
    if (RG1L != 0)
      skip00373 = true;
  }

  if (!skip00373)
  {
    //m_1A8AA:
    RG1L = signedlong(DM2_RANDDIR());
    if (RG1W == 0)
      return 0;
  }

  //m_1A8B4:
  DM2_move_12b4_0d75(signedlong(vw_14), signedlong(vw_10), signedlong(vw_0c), lcon(0xfe));
  return 0;
}

i32 DM2_CONFUSE_CREATURE(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  i16 RG6w;

  RG6w = RG1W;
  RG3L = RG4L;
  RG5w = RG2W;
  RG4L = 0;
  RG2W = ddat.v1e0b4c;
  if (RG2W != lcon(0xffffffff))
  {
    RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW))), lcon(0x4)))));
    RG2W = word_at(RG1P, lcon(0x16)) >> 4 & 0xf;
    RG1W = DM2_RAND16(RG6w);
    if (RG2UW <= RG1UW)
    {
      RG1L = unsignedlong(RG2W);
      if (RG1L != lcon(0xf))
      {
        i32 parl01 = RG4L;
        RG2L = signedlong(RG5w);
        RG4L = signedlong(RG3W);
        DM2_ATTACK_CREATURE(unsignedlong(ddat.v1e0b4c), RG4L, RG2L, lcon(0x2005), lcon(0x64), parl01);
        RG4L = 1;
      }
    }
  }
  return RG4L;
}

// was SKW_1c9a_0006 (new)
i32 DM2_CREATURE_CAN_HANDLE_IT(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  c_wlreg _RG6;
  i8* RG7p;
  i16 vw_00;

  RG6L = RG1L;
  RG1L = RG4L;
  RG4W &= lcon(0x80);
  RG5l = signedlong(RG4W);
  RG1L &= lcon(0xffff003f);
  if (RG1W == lcon(0xffffffff) || RG1W == lcon(0xfffffffe))
    return 0;
  //m_1F713:
  if (RG1W != lcon(0x3f))
  {
    //m_1F728:
    if (RG1W != lcon(0x3e))
    {
      bool skip00517 = false;
      //m_1F752:
      if (RG1W != lcon(0x29))
        skip00517 = true;
      else
      {
        RG4L = unsignedlong(RG6W);
        RG1L = DM2_IS_CONTAINER_MONEYBOX(RG4L);
        if (RG1L == 0)
        {
          //m_1F77C:
          RG1L = lcon(0x7);
          skip00517 = true;
        }
        else
        {
          RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
          if (jz_test8(location(RG1P + lcon(0x7)), lcon(0x4)))
            return RG5l;
        }
      }

      if (skip00517)
      {
        bool skip00522 = false;
        bool skip00519 = false;
        bool skip00518 = false;
        //m_1F781:
        RG2Blo = byte_at(DOWNCAST(SPX_Creature, s350.v1e054e), lcon(0x4));
        RG4L = RG6L;
        RG4W &= lcon(0x3c00);
        RG4L = unsignedlong(RG4W) >> bcon(0xa);
        vw_00 = RG4W;
        if (RG1W < wcon(0x10))
          skip00518 = true;
        else
        {
          RG4L = signedlong(RG1W);
          if (RG4L >= lcon(0x13))
            skip00518 = true;
        }

        if (skip00518)
        {
          //m_1F7AC:
          if (RG1W != lcon(0x28))
            skip00522 = true;
          else
            skip00519 = true;
        }
        else
          skip00519 = true;

        if (skip00519)
        {
          bool skip00521 = false;
          //m_1F7B2:
          if (RG1W != lcon(0x28))
            skip00521 = true;
          else
          {
            bool skip00520 = false;
            RG4L = unsignedlong(RG6W);
            RG1L = DM2_IS_CONTAINER_MONEYBOX(RG4L);
            if (RG1L == 0)
              skip00520 = true;
            else
            {
              RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
              if (jnz_test8(location(RG1P + lcon(0x7)), lcon(0x4)))
                skip00520 = true;
            }

            if (skip00520)
            {
              //m_1F7D9:
              RG1L = lcon(0x10);
              skip00521 = true;
            }
          }

          if (skip00521)
          {
            //m_1F7DE:
            RG4P = DOWNCAST(SPX_Creature, s350.v1e054e);
            RG4W = word_at(RG4P, lcon(0x8)) * wcon(0x3);
            RG1L += RG4L;
            skip00522 = true;
          }
        }

        if (skip00522)
        {
          //m_1F7EB:
          RG4L = signedlong(RG1W) << 2;
          RG7p = s350.v1e07ee[RG4L / 4];
          if (RG7p == NULL)
          {
            s350.v1e0896--;
            RG3P = s350.v1e0898;
            s350.v1e07ee[RG4L / 4] = RG3P;
            RG7p = RG3P;
            RG4L = signedlong(vw_00);
            RG3P += lcon(0x40);
            s350.v1e0898 = RG3P;
            RG4Blo = RG4L == lcon(0x4) ? 1 : 0;
            RG3L = RG4L & lcon(0xff);
            RG4L = signedlong(RG1W);
            RG1L = unsignedlong(RG2Blo);
            RG2P = RG7p;
            RG1L = DM2_QUERY_CREATURES_ITEM_MASK(RG1L, RG4L, RG2P, RG3L);
            if (RG1L == 0)
              return RG1L;
          }
          //m_1F844:
          RG1L = signedlong(vw_00);
          if (RG1L != lcon(0x4))
          {
            //m_1F864:
            if (RG1L != lcon(0x3))
            {
              //m_1F8C1:
              if (RG1L < lcon(0x5) || RG1L > lcon(0xa))
                return 0;
              RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG6W);
            }
            else
            {
              bool skip00524 = false;
              RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG6UW));
              RG4P = RG1P;
              RG6W = word_at(RG1P, 2);
              RG6L &= lcon(0x7f);
              if (RG6UW < mkuw(lcon(0x1a)))
                skip00524 = true;
              else
              {
                bool skip00523 = false;
                if (RG6UW <= mkuw(lcon(0x1a)))
                  skip00523 = true;
                else
                {
                  if (RG6W == lcon(0x1b))
                  {
                    if (jz_test16(location(RG1P + 2), lcon(0xffffff80)))
                      return 0;
                    skip00523 = true;
                  }
                }

                if (skip00523)
                {
                  //m_1F896:
                  RG1Blo = DM2_GET_WALL_DECORATION_OF_ACTUATOR(RG4P);
                  RG4L = unsignedlong(RG1Blo);
                  RG3L = lcon(0xe);
                  RG2L = lcon(0xb);
                  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0x9), RG4Blo, lcon(0xb), lcon(0xe));
                }
                else
                  skip00524 = true;
              }

              if (skip00524)
                //m_1F8B7:
                RG1W = word_at(RG4P, 2) & lcon(0x7f);
            }
          }
          else
          {
            RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG6UW));
            RG1Blo = byte_at(RG1P, lcon(0x4));
            RG1W = unsignedword(RG1Blo);
          }

          //m_1F8DD:
          RG4L = signedlong(RG1W) / 8;
          RG7p += RG4L;
          RG4L = unsignedlong(byte_at(RG7p, 0));
          RG1W &= lcon(0x7);
          RG3L = signedlong(RG1W);
          RG1L = lcon(0x1) << RG3Blo;
          if ((RG1L & RG4L) == 0)
            return RG5l;
        }
      }
    }
    else
    {
      RG4L = unsignedlong(RG6W);
      RG1L = DM2_IS_CONTAINER_MONEYBOX(RG4L);
      if (RG1L == 0)
        return RG5l;
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
      if (jnz_test8(location(RG1P + lcon(0x7)), lcon(0x4)))
        return RG5l;
    }
  }

  //m_1F719:
  RG1Blo = RG5l == 0 ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  return RG1L;
}

// was SKW_1887_05aa (new)
static void DM2_CREATURE_CCM06(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;

  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4W = unsignedword(RG4Blo);
  RG4Blo = byte_at(RG1P, lcon(0x1d));
  RG2P = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG2W = word_at(RG2P, lcon(0xe));
  RG2L <<= bcon(0x6);
  RG2UW >>= 14;
  RG2L = unsignedlong(RG2W);
  RG1Blo = RG4Blo;
  RG1L = unsignedlong(RG1Blo);
  RG2L -= RG1L;
  RG2W &= 0x3;
  RG1L = signedlong(RG2W);
  if (RG1L == 2)
  {
    RG1L = DM2_RAND();
    RG1W &= 0x2;
    RG4L += RG1L;
    RG4L++;
    RG4W &= 0x3;
  }
  RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG4W &= 0x3;
  and8(location(RG1P + lcon(0xf)), lcon(0xfffffffc));
  RG4L <<= bcon(0x8);
  or16(location(RG1P + lcon(0xe)), RG4W);
}

// was SKW_1887_0205 (new)
static i32 DM2_CREATURE_CCM03(void)
{
  c_nreg _RG1;
  c_nreg _RG4;

  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG1Blo = byte_at(RG1P, lcon(0x1f));
  if (RG1Blo == 0)
  {
    //m_19FFD:
    DM2_CREATURE_CCM06();
    RG4L = 0;
  }
  else
  {
    if (RG1Blo == lcon(0x1))
      //m_1A006:
      RG4L = DM2_CREATURE_WALK_NOW();
  }
  //m_1A00D:
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4L = signedlong(RG4W);
  inc8(location(RG1P + lcon(0x1f)));
  return RG4L;
}

// was SKW_1887_0239 (new)
static i32 DM2_CREATURE_JUMPS(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG51l = 0; // TODO: added, was not initialized, check original
  i32 RG52l;
  i32 RG53l;
  c_wlreg _RG6;
  i16 parw00;

  RG4P = DOWNCAST(c_creature, s350.creatures);
  RG1Blo = byte_at(RG4P, lcon(0x1f));
  if (mkub(RG1Blo) < mkub(lcon(0x1)))
  {
    //m_1A041:
    if (RG1Blo != 0)
      return RG51l; // TODO: NOT INITIALIZED!
    mov8(location(RG4P + lcon(0x1f)), lcon(0x1));
    RG2Blo = s350.v1e07d8.b_01;
    RG53l = DM2_CREATURE_WALK_NOW();
    s350.v1e07d8.b_01 = RG2Blo;
    RG2W = word_at(RG4P, lcon(0x18)) & lcon(0x1f);
    RG2W += table1d27fc[signedlong(byte_at(RG4P, lcon(0x1b)))];
    RG2W &= lcon(0x1f);
    and8(location(RG4P + lcon(0x18)), lcon(0xffffffe0));
    RG6W = word_at(RG4P, lcon(0x18));
    RG6L |= RG2L;
    mov16(location(RG4P + lcon(0x18)), RG6W);
    RG2L = RG6L << 6;
    _RG2.ushiftr11();
    RG1L = signedlong(byte_at(RG4P, lcon(0x1b)));
    RG1W = table1d2804[RG1L];
    RG1L += RG2L;
    RG1W &= lcon(0x1f);
    RG2L = RG6L & lcon(0xfffffc1f);
    RG1L <<= bcon(0x5);
    RG3L = RG2L | RG1L;
    mov16(location(RG4P + lcon(0x18)), RG3W);
    return RG53l;
  }
  if (mkub(RG1Blo) <= mkub(lcon(0x1)))
  {
    //m_1A0BF:
    RG1L = DM2_CREATURE_WALK_NOW();
    RG52l = RG1L;
    if (RG1L != 0)
    {
      mov8(location(RG4P + lcon(0x1f)), 2);
      return RG52l;
    }
    mov8(location(RG4P + lcon(0x1f)), lcon(0x3));
    return RG52l;
  }
  if (RG1Blo != 2)
    return RG51l;
  //m_1A0D6:
  mov8(location(RG4P + lcon(0x1f)), lcon(0x3));
  RG2L = unsignedlong(s350.v1e0562.getyA());
  put16(parw00, RG2W);
  RG4L = unsignedlong(s350.v1e0562.getxA());
  RG1L = DM2_MOVE_RECORD_TO(unsignedlong(s350.v1e054c), RG4L, RG2L, RG4L, parw00);
  if (RG1W != 0)
    s350.v1e0570 = 1;
  s350.v1e0562.setxyA(CUTX8(ddat.v1e102a), CUTX8(ddat.v1e1028));
  RG1Blo = CUTX8(ddat.v1e102c);
  s350.v1e0571 = RG1Blo;
  DM2_1c9a_0648(unsignedlong(s350.v1e0571));
  return 0;
}

// was SKW_1887_034e (new)
static i32 DM2_CREATURE_USES_LADDER_HOLE(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG51p;
  i8* RG52p;
  c_wlreg _RG6; // TODO: maybe more
  i32 RG71l;
  c_lreg _RG72;
  unk* xp_00;
  i16 vw_04;
  i16 vw_08;
  i16 vw_0c;
  i16 parw00;
  i16 parw01;
  unk** xparpp02;
  i16 parw04;

  RG51p = DOWNCAST(c_creature, s350.creatures);
  put16(parw01, signedword(byte_at(RG51p, lcon(0x1b))));
  RG1L = signedlong(ddat.v1e08a4);
  put16(parw00, RG1W);
  RG3L = signedlong(ddat.v1e08a6);
  RG2L = unsignedlong(s350.v1e0562.getyA());
  RG4L = unsignedlong(s350.v1e0562.getxA());
  RG1L = DM2_CREATURE_GO_THERE(unsignedlong(mkub(byte_at(RG51p, lcon(0x20)) | bcon(0x80))), RG4L, RG2L, RG3L, parw00, parw01);
  if (RG1L == 0)
    return lcon(0x1);
  RG1L = unsignedlong(mkub(table1d613a[signedlong(byte_at(RG51p, lcon(0x1a)))] & bcon(0x4)));
  if (RG1L == 0)
    return lcon(0x1);
  RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG6W = word_at(RG1P, lcon(0xe));
  RG6L <<= bcon(0x6);
  RG6UW >>= 14;
  RG1W = word_at(RG51p, lcon(0x18)) & lcon(0x1f);
  ddat.v1e08a6 = RG1W;
  RG1W = word_at(RG51p, lcon(0x18));
  RG1L <<= bcon(0x6);
  _RG1.ushiftr11();
  ddat.v1e08a4 = RG1W;
  RG1L = signedlong(byte_at(RG51p, lcon(0x1a)));
  if (RG1L == lcon(0x39) || RG1L == lcon(0x3a))
  {
    bool skip00366 = false;
    RG3P = ADRESSOF(unk*, &xp_00);
    RG1L = signedlong(byte_at(RG51p, lcon(0x1a)));
    if (RG1L != lcon(0x39))
      //m_1A1FA:
      vw_08 = 1;
    else
      vw_08 = -1;
    //m_1A201:
    RG6L = signedlong(vw_08);
    RG1L = DM2_FIND_LADDAR_AROUND(unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), RG6L, UPCAST(unk*, RG3P));
    RG4L = RG1L;
    RG2P = DOWNCAST(unk, xp_00);
    if (RG2P == NULL)
    {
      //m_1A25D:
      RG1L = unsignedlong(s350.v1e0562.getxA());
      vw_0c = RG1W;
      RG1L = unsignedlong(s350.v1e0562.getyA());
      vw_04 = RG1W;
      xparpp02 = UPCAST(unk*, RG2P);
      RG71l = signedlong(ddat.v1d3248);
      RG3P = ADRESSOF(i16, &vw_04);
      RG2P = ADRESSOF(i16, &vw_0c);
      RG4L = RG6L;
      RG1L = signedlong(CUTX16(DM_LOCATE_OTHER_LEVEL(RG71l, RG4L, RG2P16, RG3P16, xparpp02)));
      DM2_CHANGE_CURRENT_MAP_TO(RG1L);
      RG2L = signedlong(mkw(-vw_08)); // why is the cast necessary?
      RG1L = DM2_FIND_LADDAR_AROUND(signedlong(vw_0c), signedlong(vw_04), RG2L, UPCAST(unk*, ADRESSOF(unk*, &xp_00)));
      RG4L = RG1L;
      RG6L = RG1L;
      DM2_CHANGE_CURRENT_MAP_TO(RG71l);
      RG1L = signedlong(RG4W);
      if (RG1L == lcon(0xffffffff))
      {
        RG1L = signedlong(DM2_RANDDIR());
        skip00366 = true;
      }
    }
    else
    {
      if (jz_test8(location(RG2P + lcon(0x4)), lcon(0x20)))
      {
        //m_1A240:
        RG1P = DOWNCAST(unk, xp_00);
        RG1W = word_at(RG1P, lcon(0x4));
        RG1L <<= 11;
        RG1UW >>= 14;
        RG1L = unsignedlong(RG1W);
        RG4L = signedlong(RG4W);
        RG1L += RG4L;
        RG1L &= 0x3;
        skip00366 = true;
      }
      else
      {
        RG6W = word_at(DOWNCAST(unk, xp_00), lcon(0x4));
        RG6L <<= 11;
        RG6UW >>= 14;
      }
    }

    if (skip00366)
      //m_1A2CC:
      RG6L = RG1L;
  }
  //m_1A2CE:
  RG1P = s350.v1e0552;
  RG1Blo = byte_at(RG1P, lcon(0x9)) & lcon(0x40);
  RG72L = unsignedlong(RG1Blo);
  if (RG72L != 0)
    RG1L = DM2_OPERATE_PIT_TELE_TILE(unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), lcon(0x1));
  DM2_MOVE_RECORD_TO(unsignedlong(s350.v1e054c), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), lcon(0xffffffff), 0);
  RG1W = word_at(RG51p, lcon(0x18));
  RG1UW >>= bcon(0xa);
  DM2_1c9a_0648(signedlong(RG1W)); // return unused
  RG1L = signedlong(byte_at(RG51p, lcon(0x1a)));
  if (RG1L == lcon(0x35) || RG1L == lcon(0x36))
    RG6L = DM2_query_0cee_06dc(signedlong(ddat.v1e08a6), signedlong(ddat.v1e08a4));
  RG52p = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG1L = RG6L;
  RG1W &= 0x3;
  and8(location(RG52p + lcon(0xf)), lcon(0xfffffffc));
  RG1L <<= bcon(0x8);
  or16(location(RG52p + lcon(0xe)), RG1W);
  RG1L = signedlong(ddat.v1e08a4);
  put16(parw04, RG1W);
  RG3L = signedlong(ddat.v1e08a6);
  RG1L = DM2_MOVE_RECORD_TO(unsignedlong(s350.v1e054c), lcon(0xffffffff), 0, RG3L, parw04);
  if (RG1W != 0)
  {
    s350.v1e0570 = 1;
    return 0;
  }
  s350.v1e0562.setxyA(CUTX8(ddat.v1e102a), CUTX8(ddat.v1e1028));
  RG1Blo = CUTX8(ddat.v1e102c);
  s350.v1e0571 = RG1Blo;
  DM2_1c9a_0648(unsignedlong(s350.v1e0571));
  if (RG72L != 0)
    RG1L = DM2_OPERATE_PIT_TELE_TILE(unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), 0);
  RG3Blo = s350.v1e07d8.b_01;
  if (RG3Blo != 0)
  {
    RG3Bhi = RG3Blo - 1;
    s350.v1e07d8.b_01 = RG3Bhi;
  }
  return 0;
}

// was SKW_1887_09ab (new)
static i32 DM2_CREATURE_CAST_SPELL(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG5p;
  c_o32 vo_00; // poke 16 peek 8 16 32 shl16
  i8 parb01;
  i8 parb02;
  i8 parb03;

  RG3P = DOWNCAST(c_creature, s350.creatures);
  RG5p = s350.v1e0552;
  if (mkub(byte_at(RG3P + lcon(0x20))) <= mkub(lcon(0x1)))
    ddat.v1d26a4 = timdat.gametick;
  RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG1W = word_at(RG1P, lcon(0xe));
  RG1L <<= bcon(0x6);
  RG1UW >>= 14;
  RG4W = signedword(byte_at(RG3P, lcon(0x1b)));
  if (RG1W != RG4W)
  {
    if (jz_test8(location(RG5p), lcon(0x4)))
      return lcon(0x1);
  }
  RG1L = unsignedlong(byte_at(RG5p, lcon(0x6))) / 4 + 1;
  vo_00.poke16(RG1W);
  RG1W = DM2_RAND16(vo_00.peek16());
  RG4W = vo_00.peek16() + RG1W;
  vo_00.poke16(RG4W);
  RG1W = DM2_RAND16(RG4W);
  RG2W = RG4W + RG1W;
  vo_00.poke16(RG2W);
  RG1Blo = byte_at(RG3P, lcon(0x1e));
  if (mkub(RG1Blo) < mkub(lcon(0xffffff87)))
  {
    //m_1A973:
    if (RG1Blo == lcon(0xffffff86))
    {
      //m_1A980:
      RG1L = 4 * RG2L;
      vo_00.poke16(RG1W);
    }
  }
  else
  {
    bool skip00374 = false;
    if (mkub(RG1Blo) <= mkub(lcon(0xffffff87)))
      skip00374 = true;
    else
    {
      if (mkub(RG1Blo) >= mkub(lcon(0xffffff89)))
      {
        if (mkub(RG1Blo) <= mkub(lcon(0xffffff8a)))
          skip00374 = true;
      }
    }

    if (skip00374)
      //m_1A979:
      vo_00.shl16(lcon(0x3));
  }
  //m_1A989:
  RG4L = signedlong(vo_00.peek16());
  RG2L = lcon(0xff);
  RG1L = signedlong(DM2_BETWEEN_VALUE(4, 255, RG4W));
  vo_00.poke16(RG1W);
  RG1L = signedlong(RG1W);
  if (RG1L > lcon(0x8))
  {
    //m_1A9B0:
    if (RG1L > lcon(0x10))
    {
      //m_1A9BC:
      if (RG1L > lcon(0x20))
        //m_1A9CD:
        RG4L = signedlong(DM2_RANDDIR()) + lcon(0x7);
      else
        RG4L = (DM2_RANDBIT() ? 1 : 0) + lcon(0x3);
    }
    else
      RG4L = 2;
  }
  else
    RG4L = 1;
  //m_1A9D7:
  RG1L = unsignedlong(RG4Blo);
  put8(parb03, RG1Blo);
  RG1L = unsignedlong(byte_at(RG5p, lcon(0x8)));
  put8(parb02, RG1Blo);
  RG1L = unsignedlong(vo_00.peek8());
  put8(parb01, RG1Blo);
  DM2_SHOOT_ITEM(signedlong(byte_at(RG3P, lcon(0x1e))), unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), unsignedlong(byte_at(RG3P, lcon(0x1c))), signedlong(byte_at(RG3P, lcon(0x1b))), parb01, parb02, parb03);
  if (jz_test8(location(RG5p + lcon(0x9)), lcon(0x10)))
    return 0;
  RG4W = vo_00.peek16();
  RG4L <<= bcon(0x10);
  RG4L >>= bcon(0x11);
  RG1L = DM2_WOUND_CREATURE(((DM2_RANDBIT() ? 1 : 0) & lcon(0xffff)) + RG4L & lcon(0xffff));
  if (RG1L == 0)
    return 0;
  s350.v1e0570 = 1;
  return lcon(0x1);
}

// was SKW_1887_0b1b (new)
static i32 DM2_CREATURE_STEAL_FROM_CHAMPION(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5;
  i32 RG6l;
  i32 vl_00;
  i16 vw_04;
  i16 vw_08;
  i16 vw_0c;
  bool flag;
  bool skip00376 = false;

  RG6l = 0;
  RG1W = ddat.v1d3248;
  if (RG1W != ddat.v1e08d6)
    skip00376 = true;
  else
  {
    RG1W = ddat.v1e08a6;
    if (RG1W != ddat.v1e08d8)
      skip00376 = true;
    else
    {
      RG1W = ddat.v1e08a4;
      if (RG1W == ddat.v1e08d4)
        //m_1AA90:
        RG1L = 0;
      else
        skip00376 = true;
    }
  }

  if (skip00376)
    //m_1AA89:
    RG1L = 1;
  bool skip00378 = false;
  //m_1AA92:
  if (RG1L != 0)
    return lcon(0x1);
  RG1W = DM2_GET_PLAYER_AT_POSITION(unsignedword(byte_at(DOWNCAST(c_creature, s350.creatures), lcon(0x1c))));
  RG3L = RG1L;
  vw_0c = RG1W;
  RG2L = signedlong(RG1W);
  if (RG2L == lcon(0xffffffff))
    return lcon(0x1);
  c_hero* hero = &party.hero[RG2L];
  RG1L = DM2_RAND();
  RG5L = RG1L;
  vl_00 = RG1L;
  RG1L = unsignedlong(DM2_USE_DEXTERITY_ATTRIBUTE(RG3W));
  RG4L = lcon(0x64) - RG1L;
  RG1L = RG4L;
  RG4L = unsignedlong(RG1W);
  if (!hero->use_luck(RG4W))
    skip00378 = true;
  else
  {
    RG5L &= lcon(0x70);
    RG1L = unsignedlong(RG5W);
    if (RG1L != 0)
      skip00378 = true;
  }

  if (skip00378)
  {
    bool skip00379 = false;
    //m_1AB11:
    RG5L = 0;
    vw_08 = RG5W; // truncated
    RG3W = hero->item[0];
    RG5W = hero->item[1];
    if (RG3W == lcon(0xffffffff))
      skip00379 = true;
    else
    {
      RG4L = signedlong(byte_at(DOWNCAST(c_creature, s350.creatures), lcon(0x1e)));
      RG2L = unsignedlong(RG3W);
      RG1L = DM2_CREATURE_CAN_HANDLE_IT(RG2L, RG4L);
      if (RG1L != 0)
      {
        //m_1AB51:
        RG1W = DM2_F958(RG2L);
        vw_04 = RG1W; // truncated
      }
      else
        skip00379 = true;
    }

    if (skip00379)
      //m_1AB47:
      vw_04 = -1;
    bool skip00382 = false;
    //m_1AB5C:
    if (RG5W != lcon(0xffffffff))
    {
      RG4L = signedlong(byte_at(DOWNCAST(c_creature, s350.creatures), lcon(0x1e)));
      RG2L = unsignedlong(RG5W);
      RG1L = DM2_CREATURE_CAN_HANDLE_IT(RG2L, RG4L);
      if (RG1L != 0)
      {
        //m_1AB85:
        RG1W = DM2_F958(RG2L);
        skip00382 = true;
      }
    }

    if (!skip00382)
      //m_1AB7E:
      RG1L = lcon(0xffffffff);

    //m_1AB8C:
    RG4L = vl_00;
    RG4W &= 0x3;
    RG4L = unsignedlong(RG4W);
    if (RG4L == 0)
    {
      //m_1ABAB:
      RG1L = vl_00;
      RG1W &= 0x4;
      RG1L = unsignedlong(RG1W);
      flag = RG1L == 0;
    }
    else
    {
      RG1Blo = RG1W > vw_04 ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      flag = RG1L == 0;
    }
    //m_1ABB7:
    if (!flag)
    {
      vw_08 = 1;
      RG3L = RG5L;
    }
    RG1Blo = RG3W != wcon(0xffff) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 0)
    {
      RG4L = signedlong(byte_at(DOWNCAST(c_creature, s350.creatures), lcon(0x1e)));
      RG1L = DM2_CREATURE_CAN_HANDLE_IT(unsignedlong(RG3W), RG4L);
      if (RG1L != 0)
      {
        RG1L = DM2_REMOVE_POSSESSION(unsignedlong(vw_0c), unsignedlong(vw_08));
        RG4P = DOWNCAST(SPX_Creature, s350.v1e054e) + 2;
        RG1L = unsignedlong(RG1W);
        DM2_APPEND_RECORD_TO(mk_record(RG1L), UPCAST(t_record, RG4P), -1, 0);
        RG6l = 1;
        if (ddat.v1e0238 != 0)
        {
          ddat.v1e0238 = 0;
          RG1L = DM2_RAND();
          RG1W &= lcon(0xf);
          RG3L = unsignedlong(RG1W);
          RG1W = (DM2_QUERY_PLAYER_SKILL_LV(vw_0c, CUTX16(RG6l), RG6l) & lcon(0xffff)) + RG3L;
          RG1Blo = RG1W > lcon(0x7) ? 1 : 0;
          RG1W = unsignedword(RG1Blo);
          ddat.v1e0238 = RG6l;
          if (RG1W != 0)
            RG1L = DM2_RESUME_FROM_WAKE();
        }
      }
    }
  }
  //m_1AC72:
  RG1Blo = RG6l == 0 ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  return RG1L;
}

// was SKW_1887_0c75 (new)
static i32 DM2_CREATURE_CCM0B(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG5l;
  i16 parw00;
  i16 parw01;

  RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG1W = word_at(RG1P, lcon(0xe));
  RG1L <<= bcon(0x6);
  RG1UW >>= 14;
  RG1L = signedlong(RG1W);
  put16(parw01, RG1W);
  put16(parw00, ddat.v1e08a4);
  RG3L = signedlong(ddat.v1e08a6);
  RG5l = unsignedlong(s350.v1e0562.getyA());
  RG4L = unsignedlong(s350.v1e0562.getxA());
  RG1L = DM2_19f0_0d10(unsignedlong(mkub(byte_at(DOWNCAST(c_creature, s350.creatures), bcon(0x20)) | lcon(0xffffff80))), RG4L, RG5l, RG3L, parw00, parw01);
  if (RG1L == 0)
    return lcon(0x1);
  RG1L = signedlong(byte_at(DOWNCAST(c_creature, s350.creatures), lcon(0x1a)));
  if (RG1L != lcon(0xb))
    return lcon(0x1);
  DM2_INVOKE_MESSAGE(unsignedlong(ddat.v1e08a6), unsignedlong(ddat.v1e08a4), 0, 2, timdat.gametick);
  return 0;
}

// was SKW_1887_0e19 (new)
static i32 DM2_CREATURE_TAKES_ITEM(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG5l;
  c_wlreg _RG6;
  c_ol32 vol_00; // poke 32 peek 16 inc32

  vol_00.poke32(0);
  RG4L = signedlong(ddat.v1e08a4);
  RG1L = DM2_GET_WALL_TILE_ANYITEM_RECORD(signedlong(ddat.v1e08a6), RG4W);
  //m_1AEB5:
  for (;;)
  {
    bool skip00385 = false;
    RG6L = RG1L;
    if (RG6W == lcon(0xfffffffe))
      break;
    RG1L = RG6L;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    if (RG1L == lcon(0x4))
      skip00385 = true;
    else
    {
      RG1P = DOWNCAST(c_creature, s350.creatures);
      RG1Blo = byte_at(RG1P, lcon(0x1c));
      RG1L = unsignedlong(RG1Blo);
      if (RG1L != lcon(0xff))
      {
        RG4L = unsignedlong(RG6W) >> bcon(0xe);
        if (RG4L != RG1L)
          skip00385 = true;
      }
    }

    if (!skip00385)
    {
      //m_1AEF9:
      RG4L = signedlong(byte_at(DOWNCAST(c_creature, s350.creatures), lcon(0x1e)));
      RG5l = unsignedlong(RG6W);
      RG1L = DM2_CREATURE_CAN_HANDLE_IT(RG5l, RG4L);
      if (RG1L != 0)
      {
        DM2_MOVE_RECORD_TO(RG5l, signedlong(ddat.v1e08a6), signedlong(ddat.v1e08a4), lcon(0xffffffff), 0);
        RG4P = DOWNCAST(SPX_Creature, s350.v1e054e) + 2;
        RG3L = 0;
        RG2L = lcon(0xffffffff);
        RG1L = RG5l;
        DM2_APPEND_RECORD_TO(mk_record(RG1L), UPCAST(t_record, RG4P), RG2W, RG3W);
        RG1P = DOWNCAST(c_creature, s350.creatures);
        RG1Blo = byte_at(RG1P, lcon(0x1e)) & lcon(0x40);
        RG1L = signedlong(RG1Blo);
        vol_00.inc32();
        if (RG1L == 0)
          break;
      }
    }

    //m_1AF60:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG6UW)));
  }
  //m_1AF6F:
  RG1Blo = vol_00.peek16() == wcon(0x0) ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  return RG1L;
}

// was SKW_1887_0ce1 (new)
static i32 DM2_CREATURE_CCM0C(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;

  RG4L = 0;
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG1Blo = byte_at(RG1P, lcon(0x1f));
  if (RG1Blo == 0)
  {
    //m_1AD30:
    RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
    RG1W = word_at(RG1P, lcon(0xe));
    RG1L <<= bcon(0x6);
    RG1UW >>= 14;
    RG2Blo = RG1Blo;
    DM2_CREATURE_CCM06();
    mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1d)), RG2Blo);
  }
  else
  {
    if (RG1Blo == lcon(0x1))
    {
      //m_1AD51:
      DM2_CREATURE_CCM06();
      RG4L = DM2_CREATURE_TAKES_ITEM();
    }
  }
  //m_1AD5D:
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4L = signedlong(RG4W);
  inc8(location(RG1P + lcon(0x1f)));
  return RG4L;
}

// was SKW_1887_0d33 (new)
static i32 DM2_CREATURE_PUTS_DOWN_ITEM(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG51;
  i32 RG52l;
  i16 RG6w = wcon(0x0);
  i16 parw00;
  i16 parw01;
  i16 parw02;
  i16 parw03;

  //m_1AD74:
  for (;;)
  {
    RG4P = DOWNCAST(SPX_Creature, s350.v1e054e);
    RG4W = word_at(RG4P, 2);
    RG4L = unsignedlong(RG4W);
    RG1W = DM2_CREATURE_CAN_HANDLE_ITEM_IN(signedword(byte_at(DOWNCAST(c_creature, s350.creatures), lcon(0x1e))), RG4L, lcon(0xff));
    RG51L = RG1L;
    if (RG1W != lcon(0xfffffffe))
    {
      RG4P = DOWNCAST(SPX_Creature, s350.v1e054e);
      RG6w++;
      RG4P += 2;
      RG1L = unsignedlong(RG51W);
      DM2_CUT_RECORD_FROM(mk_record(RG1L), UPCAST(t_record, RG4P), -1, 0);
      RG1P = DOWNCAST(c_creature, s350.creatures);
      RG1Blo = byte_at(RG1P, lcon(0x1c));
      RG1W = unsignedword(RG1Blo);
      RG1L <<= bcon(0xe);
      RG51L &= lcon(0xffff3fff);
      RG51L |= RG1L;
      RG1L = signedlong(ddat.v1e08a4);
      put16(parw00, RG1W);
      RG3L = signedlong(ddat.v1e08a6);
      RG52l = unsignedlong(RG51W);
      DM2_MOVE_RECORD_TO(RG52l, lcon(0xffffffff), 0, RG3L, parw00);
      RG1L = signedlong(RG6w);
      if (RG1L == lcon(0x1))
      {
        put16(parw03, RG1W);
        RG1L = signedlong(ddat.v1e08a4);
        put16(parw02, RG1W);
        RG1L = signedlong(ddat.v1e08a6);
        put16(parw01, RG1W);
        RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG52l);
        RG4L = unsignedlong(RG1Blo);
        RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG52l) & lcon(0xff);
        DM2_QUEUE_NOISE_GEN2(RG1Blo, RG4Blo, lcon(0x85), lcon(0xfe), parw01, parw02, parw03, lcon(0x3a), lcon(0x80));
      }
      //m_1AE43:
      RG1Blo = byte_at(DOWNCAST(c_creature, s350.creatures), lcon(0x1e)) & lcon(0x40);
      RG1L = signedlong(RG1Blo);
      if (RG1L != 0)
        continue;
    }
    break;
  }
  //m_1AE59:
  RG1Blo = RG6w == wcon(0x0) ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  return RG1L;
}

// was SKW_1887_0df7 (new)
static void DM2_CREATURE_KILL_ON_TIMER_POSITION(void)
{
  DM2_DELETE_CREATURE_RECORD(unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), 0, lcon(0x1));
  s350.v1e0570 = 1;
}

// was SKW_1887_0eca (new)
static i32 DM2_CREATURE_SHOOT_ITEM(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG5l;
  i8 parb01;
  i8 parb02;

  RG4L = unsignedlong(word_at(DOWNCAST(SPX_Creature, s350.v1e054e), 2));
  RG1W = DM2_CREATURE_CAN_HANDLE_ITEM_IN(signedword(byte_at(DOWNCAST(c_creature, s350.creatures), lcon(0x1e))), RG4L, lcon(0xff));
  if (RG1W == lcon(0xfffffffe))
    return lcon(0x1);
  RG4P = DOWNCAST(SPX_Creature, s350.v1e054e) + 2;
  RG5l = unsignedlong(RG1W);
  DM2_CUT_RECORD_FROM(mk_record(RG5l), UPCAST(t_record, RG4P), -1, 0);
  RG4L = unsignedlong(byte_at(s350.v1e0552, lcon(0x6))) / 4 + 1;
  RG1W = DM2_RAND16(RG4W);
  RG4W += RG1W;
  RG1W = DM2_RAND16(RG4W);
  RG4W += RG1W;
  RG1P = s350.v1e0552;
  RG1Blo = byte_at(RG1P, lcon(0x8));
  RG1L = unsignedlong(RG1Blo);
  put8(parb02, RG1Blo);
  RG4L = signedlong(RG4W);
  RG1L = signedlong(DM2_BETWEEN_VALUE(20, 255, RG4W)) & lcon(0xff);
  put8(parb01, RG1Blo);
  RG1P = DOWNCAST(c_creature, s350.creatures);
  DM2_SHOOT_ITEM(RG5l, unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), unsignedlong(byte_at(RG1P, lcon(0x1c))), signedlong(byte_at(RG1P, lcon(0x1b))), parb01, parb02, lcon(0x8));
  return 0;
}

// was SKW_1887_0f80 (new)
static i32 DM2_CREATURE_ROTATES_TARGET_CREATURE(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;

  RG1L = DM2_GET_CREATURE_AT(unsignedlong(ddat.v1e08a6), unsignedlong(ddat.v1e08a4));
  if (RG1W == lcon(0xffffffff))
    return lcon(0x1);
  RG2P = DOWNCAST(c_creature, s350.creatures);
  RG2Blo = byte_at(RG2P, lcon(0x1d));
  RG2L = unsignedlong(RG2Blo);
  RG1L = unsignedlong(RG1W);
  DM2_ROTATE_CREATURE(RG1L, 0, RG2L);
  return 0;
}

static i32 DM2_PLACE_MERCHANDISE(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG51l;
  i32 RG52l;
  c_wlreg _RG6;
  c_wlreg _RG7;
  i32 vl_00;
  i16 vw_04;
  i16 vw_08;
  c_o32 vo_0c; // poke 16 peek 16 32 inc16
  i16 parw00;
  i16 parw01;
  i16 parw02;
  i16 parw03;

  RG4L = 0;
  vo_0c.poke16(RG4W);
  RG1W = ddat.v1e08a4;
  vw_04 = RG1W;
  RG4L = unsignedlong(RG1W);
  vw_08 = ddat.v1e08a6;
  RG1L = DM2_GET_CREATURE_AT(unsignedlong(vw_08), RG4L);
  if (RG1W != lcon(0xffffffff))
  {
    RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG1W));
    if ((RG1Blo & bcon(0x1)) != 0)
    {
      //m_1B0EF:
      for (;;)
      {
        RG4L = unsignedlong(word_at(DOWNCAST(SPX_Creature, s350.v1e054e), 2));
        RG1W = DM2_CREATURE_CAN_HANDLE_ITEM_IN(signedword(byte_at(DOWNCAST(c_creature, s350.creatures), lcon(0x1e))), RG4L, lcon(0xff));
        RG6W = RG1W;
        if (RG1W != lcon(0xfffffffe))
        {
          RG4P = DOWNCAST(SPX_Creature, s350.v1e054e);
          vo_0c.inc16();
          RG4P += 2;
          RG1L = unsignedlong(RG1W);
          DM2_CUT_RECORD_FROM(mk_record(RG1L), UPCAST(t_record, RG4P), -1, 0);
          RG1P = DOWNCAST(c_creature, s350.creatures);
          RG1Blo = byte_at(RG1P, lcon(0x1c));
          RG1L = unsignedlong(RG1Blo);
          if (RG1L == lcon(0xff))
            RG1L = signedlong(DM2_RANDDIR()) & lcon(0xffff);
          RG6L &= lcon(0xffff3fff);
          RG1W = unsignedword(RG1Blo);
          RG1L <<= bcon(0xe);
          RG6L |= RG1L;
          RG51l = unsignedlong(RG6W);
          RG1L = DM2_IS_CONTAINER_MONEYBOX(RG51l);
          if (RG1L != 0)
          {
            RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(CUTX16(RG51l))));
            or8(location(RG1P + lcon(0x7)), lcon(0x4));
          }
          RG1L = signedlong(vw_04);
          vl_00 = RG1L;
          put16(parw00, RG1W);
          RG7L = signedlong(vw_08);
          RG52l = unsignedlong(RG6W);
          RG3L = RG7L;
          RG2L = lcon(0xffffffff);
          DM2_MOVE_RECORD_TO(RG52l, RG2L, RG2L, RG3L, parw00);
          RG1L = signedlong(vo_0c.peek16());
          if (RG1L == lcon(0x1))
          {
            put16(parw03, RG1W);
            RG2L = vl_00;
            put16(parw02, RG2W);
            put16(parw01, RG7W);
            RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG52l);
            RG4L = unsignedlong(RG1Blo);
            RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG52l) & lcon(0xff);
            DM2_QUEUE_NOISE_GEN2(RG1Blo, RG4Blo, lcon(0x85), lcon(0xfe), parw01, parw02, parw03, lcon(0x3a), lcon(0x80));
          }
          //m_1B1EC:
          RG1P = DOWNCAST(c_creature, s350.creatures);
          RG1Blo = byte_at(RG1P, lcon(0x1e)) & lcon(0x40);
          RG1L = signedlong(RG1Blo);
          if (RG1L != 0)
            continue;
        }
        break;
      }
    }
  }
  //m_1B201:
  RG1Blo = vo_0c.peek16() == wcon(0x0) ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  return RG1L;
}

// was S KW_1887_10db (new)
static i32 DM2_TAKE_MERCHANDISE(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5;
  i16 RG6w = wcon(0x0);
  i16 vw_00;

  RG1L = DM2_GET_CREATURE_AT(unsignedlong(ddat.v1e08a6), unsignedlong(ddat.v1e08a4));
  vw_00 = RG1W; // truncated
  if (RG1W != lcon(0xffffffff))
  {
    RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG1W));
    if ((RG1Blo & bcon(0x1)) != 0)
    {
      //m_1B253:
      for (;;)
      {
        RG2P = DOWNCAST(c_creature, s350.creatures);
        RG2Blo = byte_at(RG2P, lcon(0x1c));
        RG2L = unsignedlong(RG2Blo);
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(vw_00)));
        RG4L = unsignedlong(word_at(RG1P, 2));
        RG1W = DM2_CREATURE_CAN_HANDLE_ITEM_IN(signedword(byte_at(DOWNCAST(c_creature, s350.creatures), lcon(0x1e))), RG4L, RG2L);
        RG5W = RG1W;
        if (RG1W != lcon(0xfffffffe))
        {
          RG2L = signedlong(ddat.v1e08a4);
          RG4L = signedlong(ddat.v1e08a6);
          RG1L = unsignedlong(RG5W);
          RG6w++;
          DM2_MOVE_RECORD_TO(RG1L, RG4L, RG2L, lcon(0xffffffff), lcon(0xffff));
          RG5L &= lcon(0xffff3fff);
          RG4P = DOWNCAST(SPX_Creature, s350.v1e054e) + 2;
          RG1L = unsignedlong(RG5W);
          DM2_APPEND_RECORD_TO(mk_record(RG1L), UPCAST(t_record, RG4P), -1, 0);
          RG1P = DOWNCAST(c_creature, s350.creatures);
          RG1Blo = byte_at(RG1P, lcon(0x1e)) & lcon(0x40);
          RG1L = signedlong(RG1Blo);
          if (RG1L != 0)
            continue;
        }
        break;
      }
    }
  }
  //m_1B2E4:
  RG1Blo = RG6w == wcon(0x0) ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  return RG1L;
}

// was S KW_1887_1191 (new)
static i32 DM2_CREATURE_ACTIVATES_WALL(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG5p;
  c_wlreg _RG61;
  i16 RG62w;
  i8 vb_00;
  i16 vw_04;
  i16 parw00;
  i16 parw01;
  i16 parw02;

  RG5p = DOWNCAST(c_creature, s350.creatures);
  RG1W = signedword(byte_at(RG5p, lcon(0x1e)));
  vw_04 = RG1W; // truncated
  RG1L = signedlong(byte_at(RG5p, lcon(0x1e)));
  put16(parw02, RG1W);
  RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG1W = word_at(RG1P, lcon(0xe));
  RG1L <<= bcon(0x6);
  RG1UW >>= 14;
  RG1L = signedlong(RG1W);
  put16(parw01, RG1W);
  put16(parw00, ddat.v1e08a4);
  RG3L = signedlong(ddat.v1e08a6);
  RG2L = unsignedlong(s350.v1e0562.getyA());
  RG4L = unsignedlong(s350.v1e0562.getxA());
  RG61W = unsignedword(byte_at(RG5p, lcon(0x20)));
  RG1L = RG61L;
  RG1Blo |= lcon(0xffffff80);
  vb_00 = RG1Blo;
  RG1L = DM2_19f0_2813(unsignedlong(vb_00), RG4L, RG2L, RG3W, parw00, parw01, parw02) ? 1 : 0;
  if (RG1L == 0)
    return lcon(0x1);
  if (RG61W != 2 && vw_04 != lcon(0xffffffff))
  {
    //m_1B377:
    RG4P = DOWNCAST(SPX_Creature, s350.v1e054e);
    RG4W = word_at(RG4P, 2);
    RG4L = unsignedlong(RG4W);
    RG1W = DM2_CREATURE_CAN_HANDLE_ITEM_IN(signedword(byte_at(RG5p, lcon(0x1e))), RG4L, lcon(0xff));
    RG62w = RG1W;
    if (RG1W == lcon(0xfffffffe))
      return lcon(0x1);
    RG4P = DOWNCAST(SPX_Creature, s350.v1e054e) + 2;
    RG1L = unsignedlong(RG1W);
    DM2_CUT_RECORD_FROM(mk_record(RG1L), UPCAST(t_record, RG4P), -1, 0);
  }
  else
    RG62w = wcon(0xffff);
  //m_1B3B9:
  RG1L = unsignedlong(RG62w);
  i32 parl03 = RG1L;
  RG3L = unsignedlong(s350.v1e054c);
  RG1W = unsignedword(RG1Blo);
  RG1Blo = byte_at(RG5p, lcon(0x1d));
  RG1L += 2;
  RG1W &= 0x3;
  RG2L = unsignedlong(RG1W);
  RG1W = word_at(RG5p, lcon(0x18));
  RG1L <<= bcon(0x6);
  _RG1.ushiftr11();
  RG4L = unsignedlong(RG1W);
  RG1W = word_at(RG5p, lcon(0x18)) & lcon(0x1f);
  RG1L = unsignedlong(RG1W);
  DM2_events_3C1E5(RG1L, RG4L, RG2L, RG3L, parl03);
  return 0;
}

// was S KW_1887_126d (new)
static void DM2_CREATURE_TRANSFORM(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG51;
  i16 RG52w;
  i16 RG6w;
  i8* RG7p;
  i16 vw_00;
  i8 vb_04;
  unk** xparpp02;
  i16 parw05;
  i16 parw06;
  i16 parw07;

  RG7p = DOWNCAST(c_creature, s350.creatures);
  RG1L = unsignedlong(s350.v1e0562.getxA());
  vw_00 = RG1W;
  RG6w = unsignedword(s350.v1e0562.getyA());
  RG1Blo = byte_at(RG7p, lcon(0x1f));
  vb_04 = RG1Blo;
  RG1Blo++;
  mov8(location(RG7p + lcon(0x1f)), RG1Blo);
  RG1L = signedlong(byte_at(RG7p, lcon(0x1a)));
  if (RG1L != lcon(0x3b))
  {
    //m_1B5D0:
    RG3L = unsignedlong(RG6w);
    DM2_CREATE_CLOUD(lcon(0xffa8), lcon(0xff), unsignedlong(vw_00), RG3L, lcon(0xff));
    RG1L = signedlong(RG6w);
    put16(parw07, RG1W);
    put16(parw06, vw_00);
    put16(parw05, lcon(0xc8));
    RG3L = lcon(0x6c);
    RG2L = lcon(0x81);
  }
  else
  {
    RG51L = signedlong(DM2_RANDDIR()) + 1;
    //m_1B453:
    for (;;)
    {
      RG51L--;
      if (RG51W == lcon(0xffffffff))
        break;
      RG1W = DM2_RAND16(5);
      RG1Bhi = RG1Blo;
      RG4L = unsignedlong(RG1Blo);
      if (RG4L == lcon(0x4))
        RG1Bhi = lcon(0xffffffff);
      RG4L = unsignedlong(RG1Bhi);
      i32 parl00 = RG4L;
      RG3L = unsignedlong(RG6w);
      RG2L = unsignedlong(vw_00);
      RG1W = DM2_RAND16(156) + 100;
      RG4L = unsignedlong(RG1W);
      DM2_CREATE_CLOUD(lcon(0xff83), RG4L, RG2L, RG3L, parl00);
    }
    //m_1B4A2:
    RG1L = signedlong(vb_04);
    if (RG1L >= lcon(0x1))
    {
      RG3L = unsignedlong(RG6w);
      RG2L = unsignedlong(vw_00);
      RG1W = signedword(vb_04);
      RG1L *= lcon(0x55);
      RG4L = unsignedlong(RG1W);
      DM2_CREATE_CLOUD(lcon(0xffa8), RG4L, RG2L, RG3L, lcon(0xff));
    }
    RG1L = signedlong(vb_04);
    if (RG1L >= lcon(0x3))
    {
      //m_1B505:
      mov8(location(RG7p + lcon(0x1a)), lcon(0x3c));
      mov8(location(RG7p + lcon(0x1f)), 0);
      mov16(location(DOWNCAST(SPX_Creature, s350.v1e054e) + lcon(0xa)), 0);
      RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
      RG4Blo = byte_at(RG7p, lcon(0x1e));
      mov8(location(RG1P + lcon(0x4)), RG4Blo);
      RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(RG4Blo)));
      s350.v1e0552 = RG1P;
      RG52w = word_at(RG1P, lcon(0x4));
      RG1W = DM2_RAND16(RG52w / 8 + 1);
      RG52w += RG1W;
      mov16(location(DOWNCAST(SPX_Creature, s350.v1e054e) + lcon(0x6)), RG52w);
      xparpp02 = &s350.v1e055a;
      RG2P = s350.v1e055e;
      RG3P = RG2P + 2;
      RG4L = signedlong(byte_at(RG7p, lcon(0x1a)));
      RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
      RG1Blo = byte_at(RG1P, lcon(0x4));
      RG1L = unsignedlong(RG1Blo);
      DM2_GET_CREATURE_ANIMATION_FRAME(RG1L, RG4L, RG2P16, RG3P16, xparpp02, 0);
      RG1L = DM2_IS_CREATURE_ALLOWED_ON_LEVEL(unsignedlong(s350.v1e054c), unsignedlong(s350.v1e0571));
      if (RG1L == 0)
      {
        RG4L = unsignedlong(RG6w);
        RG1W = vw_00;
        DM2_DELETE_CREATURE_RECORD(RG1L, RG4L, 0, lcon(0x1));
        s350.v1e0570 = 1;
      }
      RG1L = signedlong(RG6w);
      put16(parw07, RG1W);
      put16(parw06, vw_00);
      put16(parw05, lcon(0xc8));
      RG3L = lcon(0x6c);
      RG2L = lcon(0x81);
    }
    else
    {
      RG1L = signedlong(RG6w);
      put16(parw07, RG1W);
      put16(parw06, vw_00);
      put16(parw05, lcon(0xc8));
      RG3L = lcon(0x6c);
      RG2L = lcon(0x8b);
    }
  }
  //m_1B60D:
  DM2_QUEUE_NOISE_GEN1(lcon(0x3), 0, RG2Blo, RG3W, parw05, parw06, parw07, lcon(0x1));
}

// was SKW_1887_141c (new)
static i32 DM2_CREATURE_EXPLODE_OR_SUMMON(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG51l;
  i16 parw02;
  i8 parb04;
  unk** xparpp05;

  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4Blo = byte_at(RG1P, lcon(0x20));
  if (RG4Blo == 0)
  {
    RG4L = unsignedlong(byte_at(s350.v1e0552, lcon(0x6))) / 4 + 1;
    RG1W = DM2_RAND16(RG4W);
    RG4W += RG1W;
    RG1W = DM2_RAND16(RG4W);
    RG4W += RG1W;
    RG3L = unsignedlong(s350.v1e0562.getyA());
    RG51l = unsignedlong(s350.v1e0562.getxA());
    RG4L = signedlong(RG4W);
    RG2L = lcon(0xff);
    RG1L = signedlong(DM2_BETWEEN_VALUE(20, 255, RG4W));
    RG4L = unsignedlong(RG1W);
    DM2_CREATE_CLOUD(lcon(0xff80), RG4L, RG51l, RG3L, lcon(0xff));
    RG1P = DOWNCAST(c_creature, s350.creatures);
    if (byte_at(RG1P + lcon(0x1e)) != 0)
      DM2_CREATURE_KILL_ON_TIMER_POSITION();
    return 0;
  }
  if (RG4Blo == lcon(0x1))
  {
    RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
    RG1W = word_at(RG1P, lcon(0xe));
    RG1L <<= bcon(0x6);
    RG1UW >>= 14;
    RG1L = signedlong(RG1Blo);
    put8(parb04, RG1Blo);
    RG1L = unsignedlong(s350.v1e0571);
    put16(parw02, RG1W);
    i32 parl01 = unsignedlong(s350.v1e0562.getyA());
    RG3L = unsignedlong(s350.v1e0562.getxA());
    RG1L = signedlong(DM2_RANDDIR());
    RG2L = unsignedlong(RG1W);
    RG1W = DM2_CREATE_MINION(signedlong(byte_at(DOWNCAST(c_creature, s350.creatures), lcon(0x1e))), lcon(0x7), RG2L, RG3L, parl01, parw02, lcon(0xffff), parb04);
    return RG1W == -1 ? lcon(0x1) : 0;
  }
  //m_1B710:
  if (RG4Blo != 2)
    return 0;
  mov8(location(RG1P + lcon(0x1a)), lcon(0x11));
  mov8(location(DOWNCAST(c_creature, s350.creatures) + lcon(0x1f)), 0);
  RG2P = DOWNCAST(c_creature, s350.creatures);
  RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG4Blo = byte_at(RG2P, lcon(0x1e));
  mov8(location(RG1P + lcon(0x4)), RG4Blo);
  RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(RG4Blo)));
  s350.v1e0552 = RG1P;
  RG4W = word_at(RG1P, lcon(0x4));
  RG1W = DM2_RAND16(RG4W / 8 + 1);
  RG4W += RG1W;
  mov16(location(DOWNCAST(SPX_Creature, s350.v1e054e) + lcon(0x6)), RG4W);
  xparpp05 = &s350.v1e055a;
  RG2P = s350.v1e055e;
  RG3P = RG2P + 2;
  RG4L = signedlong(byte_at(DOWNCAST(c_creature, s350.creatures), lcon(0x1a)));
  RG1P = DOWNCAST(SPX_Creature, s350.v1e054e);
  RG1Blo = byte_at(RG1P, lcon(0x4));
  RG1L = unsignedlong(RG1Blo);
  DM2_GET_CREATURE_ANIMATION_FRAME(RG1L, RG4L, RG2P16, RG3P16, xparpp05, 0);
  RG1L = DM2_IS_CREATURE_ALLOWED_ON_LEVEL(unsignedlong(s350.v1e054c), unsignedlong(s350.v1e0571));
  if (RG1L == 0)
  {
    RG4L = unsignedlong(s350.v1e0562.getyA());
    RG1Blo = s350.v1e0562.getxA();
    DM2_DELETE_CREATURE_RECORD(RG1L, RG4L, 0, lcon(0x1));
    s350.v1e0570 = 1;
  }
  return 0;
}

i32 DM2_CREATURE_WALK_NOW(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG5p;
  c_lreg _RG6;
  i16 parw00;
  i16 parw01;
  i16 parw02;

  RG5p = DOWNCAST(c_creature, s350.creatures);
  RG1P = s350.v1e0552;
  if (jnz_test8(location(RG1P + lcon(0x1)), lcon(0x1)))
  {
    mov8(location(RG5p + lcon(0x1a)), lcon(0x26));
    mov8(location(RG5p + lcon(0x1c)), lcon(0x4));
    DM2_CREATURE_ATTACKS_PARTY();
  }
  put16(parw01, signedword(byte_at(RG5p, lcon(0x1b))));
  RG1L = signedlong(ddat.v1e08a4);
  put16(parw00, RG1W);
  RG3L = signedlong(ddat.v1e08a6);
  RG2L = unsignedlong(s350.v1e0562.getyA());
  RG4L = unsignedlong(s350.v1e0562.getxA());
  RG1L = DM2_CREATURE_GO_THERE(unsignedlong(mkub(byte_at(RG5p, lcon(0x20)) | bcon(0x80))), RG4L, RG2L, RG3L, parw00, parw01);
  if (RG1L == 0)
    return lcon(0x1);
  RG1L = unsignedlong(mkub(table1d613a[signedlong(byte_at(RG5p, lcon(0x1a)))] & bcon(0x4)));
  if (RG1L == 0)
    return lcon(0x1);
  RG1P = s350.v1e0552;
  RG1Blo = byte_at(RG1P, lcon(0x9)) & lcon(0x40);
  RG6L = unsignedlong(RG1Blo);
  if (RG6L != 0)
  {
    RG4L = unsignedlong(s350.v1e0562.getyA());
    RG1L = unsignedlong(s350.v1e0562.getxA());
    RG1L = DM2_OPERATE_PIT_TELE_TILE(RG1L, RG4L, lcon(0x1));
  }
  RG1W = word_at(RG5p, lcon(0x18));
  RG1L <<= bcon(0x6);
  _RG1.ushiftr11();
  RG1L = signedlong(RG1W);
  put16(parw02, RG1W);
  RG1W = word_at(RG5p, lcon(0x18)) & lcon(0x1f);
  RG3L = signedlong(RG1W);
  RG2L = unsignedlong(s350.v1e0562.getyA());
  RG4L = unsignedlong(s350.v1e0562.getxA());
  RG1L = DM2_MOVE_RECORD_TO(unsignedlong(s350.v1e054c), RG4L, RG2L, RG3L, parw02);
  if (RG1W != 0)
  {
    s350.v1e0570 = 1;
    return 0;
  }
  s350.v1e0562.setxyA(CUTX8(ddat.v1e102a), CUTX8(ddat.v1e1028));
  RG1Blo = CUTX8(ddat.v1e102c);
  s350.v1e0571 = RG1Blo;
  DM2_1c9a_0648(unsignedlong(s350.v1e0571));
  if (RG6L != 0)
    RG1L = DM2_OPERATE_PIT_TELE_TILE(unsignedlong(s350.v1e0562.getxA()), unsignedlong(s350.v1e0562.getyA()), 0);
  RG4Bhi = s350.v1e07d8.b_01;
  if (RG4Bhi != 0)
  {
    RG2Blo = RG4Bhi - 1;
    s350.v1e07d8.b_01 = RG2Blo;
  }
  return 0;
}

static i32 DM2_1B7D5(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG3L = unsignedlong(s350.v1e0562.getyA());
  RG2L = unsignedlong(s350.v1e0562.getxA());
  RG1W = DM2_RAND16(40) + 20;
  RG4L = unsignedlong(RG1W);
  return DM2_CREATE_CLOUD(lcon(0xff8e), RG4L, RG2L, RG3L, lcon(0xff));
}

i32 DM2_PROCEED_CCM(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  bool skip00390 = false;
  bool skip00388 = false;

  RG3L = 0;
  RG2L = lcon(0x400);
  RG4L = 0;
  RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x100));
  s350.v1e0898 = RG1P;
  RG1P = DOWNCAST(c_creature, s350.creatures);
  RG4W = word_at(RG1P, lcon(0x18)) & lcon(0x1f);
  ddat.v1e08a6 = RG4W;
  RG4W = word_at(RG1P, lcon(0x18));
  RG4L <<= bcon(0x6);
  _RG4.ushiftr11();
  ddat.v1e08a4 = RG4W;
  RG1Blo = byte_at(RG1P, lcon(0x1a));
  if (mkub(RG1Blo) < mkub(lcon(0x17)))
  {
    bool skip00387 = false;
    //m_1B8E6:
    if (mkub(RG1Blo) < mkub(lcon(0x9)))
    {
      //m_1B922:
      if (mkub(RG1Blo) < mkub(lcon(0x5)))
      {
        //m_1B92E:
        if (mkub(RG1Blo) >= mkub(lcon(0x1)))
        {
          if (mkub(RG1Blo) > mkub(2))
          {
            //m_1B946:
            RG1L = DM2_CREATURE_CCM03();
            skip00390 = true;
          }
          else
            skip00387 = true;
        }
      }
      else
      {
        if (mkub(RG1Blo) <= mkub(lcon(0x5)))
        {
          //m_1B94D:
          RG1L = DM2_CREATURE_JUMPS(RG1L);
          skip00390 = true;
        }
        else
        {
          if (mkub(RG1Blo) <= mkub(lcon(0x7)))
            //m_1B954:
            DM2_CREATURE_CCM06();
          else
            skip00388 = true;
        }
      }
    }
    else
    {
      if (mkub(RG1Blo) <= mkub(lcon(0x9)))
        skip00387 = true;
      else
      {
        if (mkub(RG1Blo) < mkub(lcon(0xc)))
        {
          //m_1B91C:
          if (mkub(RG1Blo) <= mkub(lcon(0xa)))
            //m_1B96C:
            RG1L = DM2_CREATURE_STEAL_FROM_CHAMPION();
          else
            //m_1B973:
            RG1L = DM2_CREATURE_CCM0B();
          skip00390 = true;
        }
        else
        {
          if (mkub(RG1Blo) <= mkub(lcon(0xd)))
          {
            //m_1B97A:
            RG1L = DM2_CREATURE_CCM0C();
            skip00390 = true;
          }
          else
          {
            if (mkub(RG1Blo) < mkub(lcon(0x13)))
            {
              //m_1B90F:
              if (mkub(RG1Blo) <= mkub(lcon(0xf)))
              {
                //m_1B988:
                RG1L = DM2_CREATURE_SHOOT_ITEM();
                skip00390 = true;
              }
            }
            else
            {
              if (mkub(RG1Blo) <= mkub(lcon(0x13)))
                //m_1B98F:
                DM2_CREATURE_KILL_ON_TIMER_POSITION();
              else
              {
                if (mkub(RG1Blo) >= mkub(lcon(0x15)))
                {
                  //m_1B996:
                  RG1L = DM2_CREATURE_ROTATES_TARGET_CREATURE();
                  skip00390 = true;
                }
              }
            }
          }
        }
      }
    }

    if (skip00387)
    {
      //m_1B93A:
      RG1L = DM2_CREATURE_WALK_NOW();
      skip00390 = true;
    }
  }
  else
  {
    if (mkub(RG1Blo) <= mkub(lcon(0x17)))
    {
      //m_1B99D:
      RG1L = DM2_PLACE_MERCHANDISE();
      skip00390 = true;
    }
    else
    {
      bool skip00389 = false;
      bool skip00386 = false;
      if (mkub(RG1Blo) < mkub(lcon(0x2b)))
      {
        //m_1B8AC:
        if (mkub(RG1Blo) < mkub(lcon(0x1a)))
        {
          //m_1B8D4:
          if (mkub(RG1Blo) <= mkub(lcon(0x18)))
          {
            //m_1B9A4:
            RG1L = DM2_TAKE_MERCHANDISE();
            skip00390 = true;
          }
          else
            skip00386 = true;
        }
        else
        {
          if (mkub(RG1Blo) <= mkub(lcon(0x1a)))
            skip00389 = true;
          else
          {
            if (mkub(RG1Blo) < mkub(lcon(0x27)))
            {
              //m_1B8C7:
              if (RG1Blo == lcon(0x26))
                skip00388 = true;
            }
            else
            {
              if (mkub(RG1Blo) <= mkub(lcon(0x28)))
              {
                //m_1B965:
                RG1L = DM2_CREATURE_CAST_SPELL();
                skip00390 = true;
              }
              else
                skip00386 = true;
            }
          }
        }
      }
      else
      {
        if (mkub(RG1Blo) <= mkub(lcon(0x2c)))
          skip00389 = true;
        else
        {
          if (mkub(RG1Blo) < mkub(lcon(0x35)))
          {
            //m_1B897:
            if (mkub(RG1Blo) <= mkub(lcon(0x2e)))
              skip00386 = true;
            else
            {
              if (mkub(RG1Blo) <= mkub(lcon(0x31)))
              {
                //m_1B9AB:
                RG1L = DM2_CREATURE_ACTIVATES_WALL();
                skip00390 = true;
              }
            }
          }
          else
          {
            if (mkub(RG1Blo) <= mkub(lcon(0x3a)))
            {
              //m_1B9B2:
              RG1L = DM2_CREATURE_USES_LADDER_HOLE(RG1L);
              skip00390 = true;
            }
            else
            {
              if (mkub(RG1Blo) < mkub(lcon(0x3d)))
                //m_1B9B9:
                DM2_CREATURE_TRANSFORM();
              else
              {
                if (mkub(RG1Blo) <= mkub(lcon(0x40)))
                {
                  //m_1B9C0:
                  RG1L = DM2_CREATURE_EXPLODE_OR_SUMMON();
                  skip00390 = true;
                }
                else
                {
                  if (RG1Blo == lcon(0x55))
                    //m_1B9CA:
                    RG1L = DM2_1B7D5();
                }
              }
            }
          }
        }
      }

      if (skip00386)
      {
        //m_1B8DC:
        RG1L = DM2_CREATURE_PUTS_DOWN_ITEM();
        skip00390 = true;
      }

      if (skip00389)
      {
        //m_1B981:
        RG1L = DM2_CREATURE_TAKES_ITEM();
        skip00390 = true;
      }
    }
  }

  if (skip00388)
  {
    //m_1B95E:
    RG1L = DM2_CREATURE_ATTACKS_PARTY();
    skip00390 = true;
  }

  if (skip00390)
    //m_1B93F:
    RG3L = RG1L;

  //m_1B9CF:
  if (RG3W == 0)
  {
    //m_1B9DE:
    RG1P = DOWNCAST(c_creature, s350.creatures);
    RG4L = signedlong(byte_at(RG1P, lcon(0x1a)));
    RG4Blo = table1d613a[RG4L] & lcon(0x3);
    RG4L = unsignedlong(RG4Blo);
    if (RG4L != 0)
    {
      RG4Blo = CUTLX8(timdat.gametick);
      mov8(location(RG1P + lcon(0x4)), RG4Blo);
    }
  }
  else
  {
    RG1W = unsignedword(RG1Blo);
    s350.v1e07d8.b_01 = RG1Bhi;
  }
  //m_1BA03:
  s350.v1e0898 = NULL;
  dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0x100));
  return RG3L;
}

// was SKW_4937_00cc, R_4EF3
// stacksize was 0x18
i32 DM2_GET_CREATURE_ANIMATION_FRAME(i32 eaxl, i32 edxl, i16* ebxpw, i16* ecxpw, unk** argpp0, i32 argl1)
{
  c_nreg _RG4; RG4L = edxl;
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = DOWNCAST(i16, ebxpw);
  c_nreg _RG3; RG3P = DOWNCAST(i16, ecxpw);
  i16 RG51w;
  i32 RG52l;
  i8* RG6p;
  i8* RG7p;
  i16 vw_00;
  i8 vb_04;

  vb_04 = RG1Blo;
  RG51w = RG4W;
  RG7p = RG2P;
  RG6p = RG3P;
  RG4L = unsignedlong(RG1Blo);
  //RG3L = lcon(0xfb);
  //RG2L = lcon(0x8);
  //RG1P = DM2_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0xf), RG4Blo, lcon(0x8), lcon(0xfb));
  RG1P = DM2_QUERY_GDAT_ENTRY_DATA_PTR(GDAT_CATEGORY_CREATURES, RG4Blo, dtRaw8, lcon(GDAT_CREATURE_ANIM_ATTRIBUTION));
#ifndef __LINUX__
  if ((i32)RG1P == (i32)0xFFFFFFFF)	// SPX, in case -1, don't go further
	  return 0;
#endif
  //m_4F1B:
  for (;;)
  {
    RG2L = signedlong(word_at(RG1P));	// That fails if RG1P (np8) = -1
    if (RG2L == lcon(0xffffffff) || RG51w == word_at(RG1P))
      break;
    RG1P += 4;
  }
  //m_4F2D:
  RG1W = word_at(RG1P, 2);
  vw_00 = RG1W; // truncated
  mov16(location(RG7p + 0), RG1W);
  RG52l = unsignedlong(vb_04);
  //RG3L = lcon(0xfc);
  //RG2L = lcon(0x7);
  RG4L = RG52l;
  //RG1P = DM2_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0xf), RG4Blo, lcon(0x7), lcon(0xfc));
  RG1P = DM2_QUERY_GDAT_ENTRY_DATA_PTR(GDAT_CATEGORY_CREATURES, RG4Blo, dtRaw7, lcon(GDAT_CREATURE_ANIM_INFO_SEQUENCE));
  RG2P = RG1P;
  RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(RG52l));
  if (jnz_test8(location(RG1P), lcon(0x1)))
  {
    RG4L = 0;
    RG1P = RG2P + 4 * unsignedlong(vw_00);
    //m_4F6E:
    for (;;)
    {
      RG2Blo = byte_at(RG1P, lcon(0x1)) & lcon(0xfffffff0);
      RG2L = unsignedlong(RG2Blo) >> 4;
      RG4L++;
      RG1P += 4;
      if (RG2L == 0)
      {
        RG2L = argl1;
        if (RG2W == 0)
          //m_4F9E:
          RG4Bhi |= lcon(0xffffff90);
        else
        {
          RG1L = RG2L;
          RG1Bhi ^= RG2Bhi;
          RG1Blo &= lcon(0x3f);
          RG1L <<= bcon(0x6);
          RG1Bhi |= lcon(0xffffff80);
          RG4L |= RG1L;
        }
        break;
      }
    }
    //m_4FA1:
    mov16(location(RG6p), RG4W);
    return lcon(0x1);
  }
  //m_4FAB:
  mov16(location(RG6p), lcon(0xffffffff));
  return DM2_4FCC(RG52l, unsignedlong(vw_00), UPCAST(i16, RG6p), UPCAST(unk*, DOWNCAST(unk*, argpp0)));
}

i32 DM2_4FCC(i32 eaxl, i32 edxl, i16* ebxpw, unk** xecxpp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = DOWNCAST(i16, ebxpw);
  c_nreg _RG3; RG3P = DOWNCAST(unk*, xecxpp);
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  i8* RG6p;
  i8* RG7p;
  i16 vw_00;
  unk* xp_04;
  bool skip00004 = false;

  vw_00 = RG4W; // truncated
  RG6p = RG2P;
  RG7p = RG3P;
  RG4L = unsignedlong(RG1Blo);
  RG3L = lcon(0xfc);
  RG2L = lcon(0x7);
  RG1P = DM2_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0xf), RG4Blo, lcon(0x7), lcon(0xfc));
  xp_04 = UPCAST(unk, RG1P);
  RG2W = word_at(RG6p);
  RG4L = unsignedlong(RG2W);
  if (RG4L != lcon(0xffff))
  {
    //m_5007:
    RG4L += unsignedlong(vw_00);
    RG1Blo = byte_at(RG1P, 4 * RG4L + lcon(0x1)) & lcon(0xfffffff0);
    RG1L = unsignedlong(RG1Blo) >> 4;
    if (RG1W == 0)
    {
      RG1L = 0;
      skip00004 = true;
    }
    else
      RG2L += RG1L;
  }
  else
    RG2L = 0;

  if (!skip00004)
  {
    //m_5028:
    RG4L = unsignedlong(vw_00);
    RG1L = 4 * (unsignedlong(RG2W) + RG4L);
    RG4P = DOWNCAST(unk, xp_04) + RG1L;
    //m_503E:
    for (;;)
    {
      RG1Blo = byte_at(RG4P, lcon(0x1)) & lcon(0xfffffff0);
      RG1L = unsignedlong(RG1Blo) >> 4;
      if (RG1L == 0)
        RG1L = 0;
      else
      {
        RG1Blo = byte_at(RG4P, lcon(0x1)) & lcon(0xf);
        RG5w = unsignedword(RG1Blo);
        RG1L = unsignedlong(RG1Blo);
        if (RG1L != lcon(0xf))
        {
          RG1L = DM2_RAND();
          RG1W &= lcon(0xf);
          RG3L = unsignedlong(RG1W);
          RG1L = unsignedlong(RG5w);
          if (RG1L < RG3L)
          {
            RG2L++;
            RG4P += 4;
            continue;
          }
        }
        //m_507F:
        RG1Blo = byte_at(RG4P, lcon(0x3)) & lcon(0xfffffff0);
        RG3L = unsignedlong(RG1Blo) >> 4;
        RG1Blo = byte_at(RG4P, lcon(0x3)) & lcon(0xc);
        RG1L = unsignedlong(RG1Blo) / 4 + RG3L;
        if (RG1L == 0)
          RG1L = 0;
        else
          RG1L = 1;
      }
      break;
    }
  }

  //m_50A5:
  mov16(location(RG6p), RG2W);
  RG4L = 4 * ((RG2L & lcon(0xffff)) + unsignedlong(vw_00));
  movp(location(RG7p + 0), DOWNCAST(unk, xp_04) + RG4L);
  return RG1L;
}
