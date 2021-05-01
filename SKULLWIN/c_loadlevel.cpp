#include "dm2data.h"
#include "c_tmouse.h"
#include "c_eventqueue.h"
#include "util.h"
#include "c_map.h"
#include "c_hero.h"
#include "c_random.h"
#include "c_gfx_main.h"
#include "c_querydb.h"
#include "c_sound.h"
#include "c_gdatfile.h"
#include "c_dballoc.h"
#include "c_tim_proc.h"
#include "c_weather.h"
#include "c_light.h"
#include "c_1c9a.h"
#include "c_loadlevel.h"

// TODO: optimize temporaries
#include "regs.h"
#include "emu.h"

#include "dm2debug.h"

static void DM2_MARK_DYN_LOAD(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG4L = RG1L;
  RG2W = ddat.v1e09a4;
  RG3L = signedlong(RG2W);
  RG1L = 6 * RG3L;
  RG3P = DOWNCAST(i16, ddat.v1e09a0);
  RG2L++;
  ddat.v1e09a4 = RG2W;
  RG1P = RG3P + RG1L;
  mov16(location(RG1P), 0);
  RG3L = RG4L;
  RG3UL >>= bcon(0x18);
  mov8(location(RG1P + 2), RG3Blo);
  RG3L = RG4L;
  RG3UL >>= bcon(0x10);
  mov8(location(RG1P + lcon(0x3)), RG3Blo);
  RG3L = RG4L;
  RG3UL >>= bcon(0x8);
  mov8(location(RG1P + lcon(0x4)), RG3Blo);
  mov8(location(RG1P + lcon(0x5)), RG4Blo);
}

// belongs to DM2_LOAD_LOCALLEVEL_DYN
static void DM2_2676_008f(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  RG2L = RG1L;
  RG3Blo = RG4Blo;
  DM2_MARK_DYN_LOAD(RG1L);
  RG4L = signedlong(ddat.v1e09a4) - 1;
  RG1L = 3 * RG4L;
  ddat.v1e09a0[RG1L] = lcon(0xffff8001);
  RG2Blo = 0;
  RG1L = unsignedlong(RG3Blo) | mkul(RG2L);
  DM2_MARK_DYN_LOAD(RG1L);
}


// belongs to DM2_LOAD_LOCALLEVEL_DYN
static void DM2_2676_00d0(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  RG3L = unsignedlong(RG2Blo);
  RG4L = unsignedlong(RG4Blo);
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(RG1Blo, RG4Blo, 11, RG3Blo);
  RG2W = RG1W;
  if (RG1W == 0)
    return;
  RG4Blo = RG1Blo;
  if (RG1Blo != 0)
  {
    RG1L = (unsignedlong(RG2Blo) << bcon(0x10)) + lcon(0xd00ffff);
    DM2_MARK_DYN_LOAD(RG1L);
  }
  //m_2ABC6:
  RG2UW >>= bcon(0x8);
  if (RG2W != 0)
  {
    RG1L = (unsignedlong(RG2Blo) << bcon(0x10)) + lcon(0xd00ffff);
    DM2_MARK_DYN_LOAD(RG1L);
  }
}


// belongs to DM2_LOAD_LOCALLEVEL_DYN
static void DM2_2676_006a(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4;

  DM2_MARK_DYN_LOAD(RG1L);
  RG4L = signedlong(ddat.v1e09a4) - 1;
  RG1L = 3 * RG4L;
  RG4L = 2 * RG1L;
  ddat.v1e09a0[RG1L] |= lcon(0x8000);
}


// belongs to DM2_LOAD_LOCALLEVEL_DYN
static void DM2_LOAD_MISCITEM(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5;
  c_wlreg _RG61;
  i32 RG62l;
  i32 RG7l;
  i32 vl_00;
  i8 vb_04;

  if (ddat.v1d268a != 0)
    return;
  ddat.v1d268a = 1;
  RG1W = unsignedword(RG1Blo);
  vb_04 = RG1Bhi;
  //m_5686:
  for (;;)
  {
    RG5L = unsignedlong(vb_04);
    if (RG5L >= lcon(0x80))
      return;
    //m_55BC:
    RG3L = 0;
    RG2L = lcon(0xb);
    RG4L = RG5L;
    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(21, RG4Blo, 11, 0);
    RG1W &= lcon(0x4000);
    RG1L = unsignedlong(RG1W);
    if (RG1L != 0)
    {
      RG3L = 2;
      RG2L = lcon(0xb);
      RG4L = RG5L;
      RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(21, RG4Blo, RG2Blo, RG3Blo);
      RG5W = RG1W;
      RG3L = 0;
      RG4W = ddat.v1e03fe;
      //m_5610:
      for (;;)
      {
        if (RG3W < RG4W)
        {
          //m_5602:
          RG1L = signedlong(RG3W);
          if (RG5W > ddat.v1e03ac[RG1L])
          {
            RG3L++;
            continue;
          }
        }
        break;
      }
      //m_5615:
      RG61W = ddat.v1e03fe;
      if (RG3W < RG61W)
      {
        RG1L = 2 * (RG61L - RG3L);
        RG62l = signedlong(RG1W);
        RG7l = 2 * signedlong(RG3W);
        RG1L = RG7l + 2;
        vl_00 = RG1L;
        DM2_COPY_MEMORY(DOWNCAST(i16, &ddat.v1e03ac[RG1L / 2]), DOWNCAST(i16, &ddat.v1e03ac[RG7l / 2]), RG62l);
        RG4P = DOWNCAST(i16, &ddat.v1e0394[vl_00 / 2]);
        RG1P = DOWNCAST(i16, &ddat.v1e0394[RG7l / 2]);
        RG2L = RG62l;
        DM2_COPY_MEMORY(RG4P, RG1P, RG2L);
      }
      //m_5660:
      RG1L = signedlong(RG3W);
      ddat.v1e03ac[RG1L] = RG5W;
      RG4L = unsignedlong(vb_04);
      RG4Bhi++;
      ddat.v1e0394[RG1L] = RG4W;
      ddat.v1e03fe++;
    }
    //m_5682:
    vb_04++;
  }
  FOREND
}


void DM2_LOAD_LOCALLEVEL_DYN(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5; // W and UW only
  i8* RG51p;
  c_wlreg _RG63;
  c_wlreg _RG64;
  i32 RG71l;
  i8* RG72p;
  unk* xp_00; // add32
  t_tile* tilep_04;
  i32 vl_08;
  unk* xp_0c;
  unk* xp_10;
  i16 vw_14;
  i16 vw_18;
  c_o32 vo_1c; // poke 16 peek 8 16 32 inc16
  i16 vw_20;
  i16 vw_24;
  i32 vl_28;
  i32 vl_2c;
  i16 vw_30;
  i8 vb_34;
  i16 parw02;
  bool flag1;
  bool flag2;
  bool skip00678 = false;

  vl_08 = 0;
  RG4L = 0;
  ddat.v1e09a4 = RG4W;
  RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x960));
  ddat.v1e09a0 = UPCAST(i16, RG1P);
  RG51p = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0xfa), true);
  RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0xfa), true);
  xp_0c = UPCAST(unk, RG1P);
  RG2L = lcon(0x400);
  RG4L = lcon(0x8000);
  RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0xfa), true);
  xp_10 = UPCAST(unk, RG1P);
  DM2_MARK_DYN_LOAD(lcon(0x1ff02ff));
  DM2_MARK_DYN_LOAD(lcon(0x18ff02ff));
  DM2_MARK_DYN_LOAD(lcon(0x7ff02ff));
  if (dm2_dballochandler.v1e13fe[0] == 0)
  {
    //m_2AC84:
    DM2_MARK_DYN_LOAD(lcon(0xd0002ff));
    RG4L = signedlong(ddat.v1e09a4) - 1;
    RG1L = 3 * RG4L;
    ddat.v1e09a0[RG1L] = 1;
    DM2_MARK_DYN_LOAD(lcon(0xd2f02ff));
    DM2_MARK_DYN_LOAD(lcon(0xd7e02ff));
    RG4L = signedlong(ddat.v1e09a4) - 1;
    RG1L = 3 * RG4L;
    ddat.v1e09a0[RG1L] = 1;
    RG1L = lcon(0xd9f02ff);
  }
  else
    RG1L = lcon(0xdff02ff);
  //m_2ACE1:
  DM2_MARK_DYN_LOAD(RG1L);
  DM2_MARK_DYN_LOAD(lcon(0x10ff02ff));
  RG4L = signedlong(ddat.v1e09a4) - 1;
  RG1L = 3 * RG4L;
  ddat.v1e09a0[RG1L] = 1;
  DM2_MARK_DYN_LOAD(lcon(0x15ff02ff));
  DM2_MARK_DYN_LOAD(lcon(0x30002ff));
  DM2_MARK_DYN_LOAD(lcon(0x8fe02ff));
  DM2_MARK_DYN_LOAD(lcon(0x16fe02ff));
  DM2_MARK_DYN_LOAD(lcon(0x9fe02ff));
  DM2_MARK_DYN_LOAD(lcon(0xafe02ff));
  DM2_MARK_DYN_LOAD(lcon(0xfff08fb));
  DM2_MARK_DYN_LOAD(lcon(0xfff07fc));
  DM2_MARK_DYN_LOAD(lcon(0x1ffffff));
  DM2_MARK_DYN_LOAD(lcon(0x1000400));
  RG4L = signedlong(ddat.v1e09a4) - 1;
  RG1L = 3 * RG4L;
  ddat.v1e09a0[RG1L] |= lcon(0x8000);
  DM2_MARK_DYN_LOAD(lcon(0x1000600));
  RG4L = signedlong(ddat.v1e09a4) - 1;
  RG1L = 3 * RG4L;
  ddat.v1e09a0[RG1L] |= lcon(0x8000);
  DM2_MARK_DYN_LOAD(lcon(0x100070a));
  RG4L = signedlong(ddat.v1e09a4) - 1;
  RG1L = 3 * RG4L;
  RG4L = 2 * RG1L;
  ddat.v1e09a0[RG1L] |= lcon(0x8000);
  DM2_MARK_DYN_LOAD(lcon(0x1a80ffff));
  DM2_MARK_DYN_LOAD(lcon(0x1a81ffff));
  DM2_MARK_DYN_LOAD(lcon(0x300ffff));
  DM2_MARK_DYN_LOAD(lcon(0x700ffff));
  if (dm2_dballochandler.v1e13fe[0] == 0)
  {
    //m_2AE17:
    DM2_MARK_DYN_LOAD(lcon(0xd00ffff));
    RG4L = signedlong(ddat.v1e09a4) - 1;
    RG1L = 3 * RG4L;
    RG4L = 2 * RG1L;
    ddat.v1e09a0[RG1L] = 1;
    DM2_MARK_DYN_LOAD(lcon(0xd2fffff));
    DM2_MARK_DYN_LOAD(lcon(0xd7effff));
    RG4L = signedlong(ddat.v1e09a4) - 1;
    RG1L = 3 * RG4L;
    ddat.v1e09a0[RG1L] = 1;
    RG1L = lcon(0xd9fffff);
  }
  else
    RG1L = lcon(0xdffffff);
  //m_2AE77:
  DM2_MARK_DYN_LOAD(RG1L);
  DM2_MARK_DYN_LOAD(lcon(0x10ffffff));
  RG4L = signedlong(ddat.v1e09a4) - 1;
  RG1L = 3 * RG4L;
  ddat.v1e09a0[RG1L] = 1;
  DM2_MARK_DYN_LOAD(lcon(0x15ffffff));
  DM2_MARK_DYN_LOAD(lcon(0xffff01f9));
  DM2_2676_008f(lcon(0xfff0510), lcon(0x39));
  RG4L = signedlong(ddat.v1e09a4) - 2;
  RG1L = 3 * RG4L;
  ddat.v1e09a0[RG1L] &= lcon(0x7fff);
  RG1L = ((signedlong(ddat.v1d3248) + 1) << bcon(0x10)) + lcon(0x30002ff);
  DM2_MARK_DYN_LOAD(RG1L);
  tilep_04 = *mapdat.map;
  RG1L = 2 * unsignedlong(*ddat.v1e03f4);
  RG4RP = dm2_v1e038c + RG1L / 2;
  xp_00 = UPCAST(unk, RG4P);
  RG2L = 0;
  vl_28 = RG2L;
  //m_2B1B4:
  for (;;)
  {
    RG1L = vl_28;
    if (RG1W >= mapdat.map_width)
    {
      RG4L = signedlong(ddat.v1d3248);
      RG1Blo = table1410ec[RG4L];
      RG1L = unsignedlong(RG1Blo) << bcon(0x10);
      RG1L += lcon(0x4000300);
      DM2_MARK_DYN_LOAD(RG1L);
      if (dm2_dballochandler.v1e13fe[0] == 0)
      {
        RG63L = 0;
        vo_1c.poke16(wcon(0x0));
        //m_2B279:
        for (;;)
        {
          RG1W = vo_1c.peek16();
          if (RG1UW >= mkuw(party.heros_in_party))
            break;
          //m_2B1FE:
          RG4L = unsignedlong(RG1W);
          RG1L = 263 * RG4L;
          RG4L = RG1L;
          RG1L = (unsignedlong(party.hero[RG4L / 263].herotype) << 16) + lcon(0x16000100);
          DM2_MARK_DYN_LOAD(RG1L);
          RG1L = (unsignedlong(party.hero[RG4L / 263].herotype) << 16) + lcon(0x160002ff);
          DM2_MARK_DYN_LOAD(RG1L);
          RG1L = (unsignedlong(party.hero[RG4L / 263].herotype) << 16) + lcon(0x16000508);
          DM2_2676_008f(RG1L, lcon(0xb));
          RG4L = signedlong(ddat.v1e09a4) - 2;
          RG1L = 3 * RG4L;
          RG4L = 2 * RG1L;
          ddat.v1e09a0[RG1L] &= lcon(0x7fff);
          vo_1c.inc16();
        }
      }
      //m_2B28A:
      if (dm2_dballochandler.v1e13fe[0] == 0)
      {
        if (!dm2_dballochandler.v1e0a84)
          break;
        RG1L = unsignedlong(party.heros_in_party);
        if (RG1L == lcon(0x4))
          break;
      }
      //m_2B2A9:
      DM2_MARK_DYN_LOAD(lcon(0x16ffffff));
      break;
    }
    //m_2AF2B:
    vl_2c = 0;
    //m_2B19F:
    for (;;)
    {
      RG1L = vl_2c;
      if (RG1W >= mapdat.map_height)
      {
        vl_28++;
        break;
      }
      //m_2AF36:
      RG63W = unsignedword(byte_at(DOWNCAST(t_tile, tilep_04)));
      RG1L = RG63L;
      RG1Blo &= lcon(0x10);
      tilep_04++;
      if (RG1Blo != 0)
      {
        RG3P = DOWNCAST(unk, xp_00);
        RG3W = word_at(RG3P);
        xp_00 += 2; // DANGER
        //m_2AF58:
        for (;;)
        {
          RG1L = RG3L;
          RG1Blo ^= RG3Blo;
          RG1Bhi &= lcon(0x3c);
          RG1L = unsignedlong(RG1W) >> bcon(0xa);
          if (mkul(RG1L) >= mkul(2))
          {
            bool skip00676 = false;
            if (mkul(RG1L) <= mkul(2))
            {
              //m_2B0A8:
              RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
              RG2P = RG1P;
              RG4W = word_at(RG1P, 2) & lcon(0x6);
              if (RG4W == 2)
              {
                bool skip00674 = false;
                RG1W = word_at(RG1P, 2);
                RG1UW >>= bcon(0x3);
                RG1L = unsignedlong(RG1W);
                RG1UL >>= bcon(0x8);
                RG1L &= lcon(0x1f);
                if (mkul(RG1L) < mkul(lcon(0xd)))
                {
                  bool skip00673 = false;
                  //m_2B111:
                  if (mkul(RG1L) < mkul(2))
                  {
                    //m_2B12D:
                    flag1 = RG1L == 0;
                    skip00673 = true;
                  }
                  else
                  {
                    if (mkul(RG1L) <= mkul(2))
                      skip00674 = true;
                    else
                    {
                      if (mkul(RG1L) >= mkul(lcon(0x4)))
                      {
                        if (mkul(RG1L) <= mkul(lcon(0x8)))
                          skip00674 = true;
                        else
                        {
                          flag1 = RG1L == lcon(0xa);
                          skip00673 = true;
                        }
                      }
                    }
                  }

                  if (skip00673)
                  {
                    //m_2B129:
                    if (flag1)
                      skip00674 = true;
                  }
                }
                else
                {
                  if (mkul(RG1L) <= mkul(lcon(0xd)))
                    skip00674 = true;
                  else
                  {
                    if (mkul(RG1L) < mkul(lcon(0xf)))
                      //m_2B131:
                      vl_08 = 1;
                    else
                    {
                      if (mkul(RG1L) <= mkul(lcon(0x11)))
                        skip00674 = true;
                      else
                      {
                        if (mkul(RG1L) >= mkul(lcon(0x13)))
                        {
                          bool skip00675 = false;
                          if (mkul(RG1L) <= mkul(lcon(0x13)))
                            skip00675 = true;
                          else
                          {
                            if (mkul(RG1L) >= mkul(lcon(0x15)))
                            {
                              if (mkul(RG1L) <= mkul(lcon(0x16)))
                                skip00675 = true;
                            }
                          }

                          if (skip00675)
                          {
                            //m_2B172:
                            RG1W = word_at(RG2P, 2);
                            RG1UW >>= bcon(0x3);
                            RG1W = unsignedword(RG1Blo);
                            skip00676 = true;
                          }
                        }
                      }
                    }
                  }
                }

                if (skip00674)
                {
                  //m_2B13B:
                  RG1L = signedlong(RG63W) >> 5;
                  if (RG1Blo != 0)
                  {
                    //m_2B15D:
                    RG1W = word_at(RG2P, 2);
                    RG1UW >>= bcon(0x3);
                    RG1P = DOWNCAST(unk, xp_10) + unsignedlong(unsignedword(RG1Blo));
                  }
                  else
                  {
                    RG1W = word_at(RG2P, 2);
                    RG1UW >>= bcon(0x3);
                    RG1P = DOWNCAST(unk, xp_0c) + unsignedlong(unsignedword(RG1Blo));
                  }
                  //m_2B158:
                  mov8(location(RG1P), 1);
                }
              }
            }
            else
            {
              if (RG1L == lcon(0x3) && dm2_dballochandler.v1e13fe[0] == 0)
              {
                RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
                RG4W = word_at(RG1P, 2) & lcon(0x7f);
                if (RG4UW < mkuw(lcon(0x2e)))
                {
                  //m_2AFB2:
                  if (RG4W == lcon(0x27))
                  {
                    //m_2AFF7:
                    RG4L = signedlong(RG63W) >> 5 & lcon(0xff);
                    if (RG4L == lcon(0x5))
                    {
                      RG1W = word_at(RG1P, 2);
                      RG1UW >>= bcon(0x7);
                      RG1L = unsignedlong(RG1W) - 1;
                      RG4L = RG1L;
                      if (RG1W != ddat.v1d3248)
                      {
                        RG2L = signedlong(RG1W);
                        RG1L = 16 * RG2L;
                        RG4P = ddat.v1e03c8;
                        RG1P = RG4P + RG1L;
                        RG4W = word_at(RG1P, lcon(0x8));
                        RG4L <<= bcon(0x5);
                        _RG4.ushiftr11();
                        vw_20 = RG4W; // truncated
                        RG4L = unsignedlong(vw_20) << 2;
                        RG2L *= 4;
                        RG2P = pointer_at(DOWNCAST(t_tile*, mapdat.tmpmap) + RG2L) + RG4L;
                        RG4W = word_at(RG1P, lcon(0x8));
                        _RG4.ushiftr11();
                        vw_20 = RG4W; // truncated
                        RG2P = pointer_at(RG2P) + unsignedlong(vw_20);
                        RG2P++;
                        RG1W = word_at(RG1P, lcon(0xc));
                        RG1L <<= bcon(0x8);
                        RG1UW >>= bcon(0xc);
                        RG1L = unsignedlong(RG1W) - 1;
                        RG4L = RG1L;
                        //m_2B09E:
                        for (;;)
                        {
                          if (RG4W < 0)
                            break;
                          //m_2B08E:
                          RG1L = signedlong(RG4W);
                          RG1Blo = byte_at(RG2P, RG1L);
                          RG1L = unsignedlong(RG1Blo);
                          or8(location(RG51p + RG1L), 2);
                          RG4L--;
                        }
                      }
                    }
                  }
                }
                else
                {
                  if (RG4UW <= mkuw(lcon(0x2e)))
                  {
                    //m_2AFBD:
                    RG4L = signedlong(RG63W) >> 5;
                    if (RG4Blo == 0)
                    {
                      RG1W = word_at(RG1P, 2);
                      RG1UW >>= bcon(0x7);
                      skip00676 = true;
                    }
                  }
                  else
                  {
                    if (RG4W == lcon(0x7e))
                    {
                      //m_2AFD8:
                      RG1W = word_at(RG1P, 2);
                      RG1UW >>= bcon(0x7);
                      RG1L = (unsignedlong(RG1Blo) << bcon(0x10)) + lcon(0x1600ffff);
                      DM2_MARK_DYN_LOAD(RG1L);
                    }
                  }
                }
              }
            }

            if (skip00676)
            {
              //m_2B17C:
              RG1L = unsignedlong(RG1W);
              mov8(location(RG51p + RG1L), 1);
            }
          }

          //m_2B185:
          RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG3UW)));
          RG3L = RG1L;
          if (RG1W == lcon(0xfffffffe))
            break;
        }
      }
      //m_2B19B:
      vl_2c++;
    }
  }
  //m_2B2B3:
  if (dm2_dballochandler.v1e13fe[0] != 0)
    skip00678 = true;
  else
  {
    RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
    if (jnz_test8(location(RG1P + 2), lcon(0x20)))
      skip00678 = true;
  }

  if (skip00678)
    //m_2B2C7:
    DM2_MARK_DYN_LOAD(lcon(0x18ffffff));

  //m_2B2D1:
  RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
  if (jz_test8(location(RG1P + 2), lcon(0xffffff80)))
    //m_2B2E9:
    RG1Blo = lcon(0xffffffff);
  else
  {
    RG1W = word_at(RG1P, lcon(0xe));
    RG1L <<= 4;
    RG1UW >>= bcon(0xc);
  }
  //m_2B2EB:
  ddat.v1e040e[0] = RG1Blo;
  RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
  if (jz_test8(location(RG1P + lcon(0x3)), 1))
    //m_2B305:
    RG1Blo = lcon(0xffffffff);
  else
  {
    RG1W = word_at(RG1P, lcon(0xe));
    RG1UW >>= bcon(0xc);
  }
  //m_2B307:
  ddat.v1e040e[1] = RG1Blo;
  if (dm2_dballochandler.v1e13fe[0] == 0)
  {
    //m_2B324:
    flag2 = false;
    RG71l = 0;
    RG3L = 0;
    vo_1c.poke16(RG3W);
    //m_2B37B:
    for (;;)
    {
      RG1L = unsignedlong(vo_1c.peek16());
      if (RG1L >= 2)
      {
        if (RG71l != 0)
        {
          RG1L = lcon(0xefe02ff);
          DM2_MARK_DYN_LOAD(RG1L);
        }
        break;
      }
      //m_2B331:
      RG1Blo = ddat.v1e040e[RG1L];
      vb_34 = RG1Blo;
      if (RG1Blo != lcon(0xffffffff))
      {
        RG71l = 1;
        RG4L = unsignedlong(RG1Blo);
        RG3L = lcon(0x40);
        RG2L = lcon(0xb);
        RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(14, RG4Blo, 11, 64);
        if (RG1W == 0)
          flag2 = RG71l != 0;
        RG1L = (unsignedlong(vb_34) << bcon(0x10)) + lcon(0xe00ffff);
        DM2_MARK_DYN_LOAD(RG1L);
      }
      //m_2B376:
      vo_1c.inc16();
    }
  }
  else
  {
    flag2 = true;
    RG1L = lcon(0xeffffff);
    DM2_MARK_DYN_LOAD(RG1L);
  }
  //m_2B395:
  RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
  RG1W = word_at(RG1P, lcon(0xe));
  RG1L <<= bcon(0x8);
  RG1UW >>= bcon(0xc);
  vb_34 = RG1Blo;
  RG1L = unsignedlong(vb_34);
  ddat.v1d6c02 = CUTX8(RG1W); // was wordtransfer, but hibyte never read
  RG4L = unsignedlong(RG1Blo);
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(8, RG4Blo, 11, lcon(0x69)) & lcon(0xffff);
  ddat.v1d6bfc = mkl(RG1W);
  RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
  RG1W = word_at(RG1P, lcon(0xe));
  RG1L <<= bcon(0x8);
  RG1UW >>= bcon(0xc);
  RG4L = unsignedlong(RG1Blo);
  RG3L = lcon(0x6b);
  RG2L = lcon(0xb);
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(8, RG4Blo, 11, lcon(0x6b));
  if (RG1W != 0)
  {
    RG1W = unsignedword(RG1Blo);
    RG1P = DOWNCAST(unk, xp_10) + signedlong(RG1W);
    mov8(location(RG1P), 1);
  }
  if (dm2_dballochandler.v1e13fe[0] != 0)
    vb_34 = lcon(0xffffffff);
  RG2L = unsignedlong(vb_34) << bcon(0x10);
  RG1L = RG2L + lcon(0x800ffff);
  DM2_MARK_DYN_LOAD(RG1L);
  RG1L = RG2L + lcon(0x1700ffff);
  DM2_MARK_DYN_LOAD(RG1L);
  if (dm2_dballochandler.v1e13fe[0] == 0)
  {
    if (!flag2)
    {
      RG1L = RG2L + lcon(0x8000106);
      DM2_2676_008f(RG1L, lcon(0x1a));
      RG1L = RG2L + lcon(0x80001d3);
      DM2_2676_008f(RG1L, lcon(0xd8));
    }
    RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
    if (jz_test8(location(RG1P + 2), 1))
    {
      RG1L = (unsignedlong(vb_34) << bcon(0x10)) + lcon(0x800016b);
      DM2_2676_008f(RG1L, lcon(0x79));
    }
    RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
    if (jz_test8(location(RG1P + 2), 2))
    {
      RG1L = (unsignedlong(vb_34) << bcon(0x10)) + lcon(0x8000199);
      DM2_2676_008f(RG1L, lcon(0xa0));
    }
    RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
    if (jz_test8(location(RG1P + 2), lcon(0x4)))
    {
      RG1L = (unsignedlong(vb_34) << bcon(0x10)) + lcon(0x8000182);
      DM2_2676_008f(RG1L, lcon(0x89));
    }
    RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
    if (jz_test8(location(RG1P + 2), lcon(0x8)))
    {
      RG1L = (unsignedlong(vb_34) << bcon(0x10)) + lcon(0x80001c7);
      DM2_2676_008f(RG1L, lcon(0xcc));
    }
    RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
    if (jz_test8(location(RG1P + 2), lcon(0x10)))
    {
      RG1L = (unsignedlong(vb_34) << bcon(0x10)) + lcon(0x80001cd);
      DM2_2676_008f(RG1L, lcon(0xd2));
    }
    RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
    if (jz_test8(location(RG1P + 2), lcon(0x40)))
    {
      if (vl_08 == 0)
      {
        RG4L = unsignedlong(vb_34) << bcon(0x10);
        RG1L = RG4L + lcon(0x8000103);
        DM2_2676_006a(RG1L);
        RG1L = RG4L + lcon(0x80001fc);
        DM2_2676_008f(RG1L, lcon(0xfe));
      }
    }
  }
  RG4L = unsignedlong(ddat.v1d6c02);
  RG3L = lcon(0x66);
  RG2L = lcon(0xb);
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(8, RG4Blo, 11, lcon(0x66));
  ddat.v1e1472 = RG1W;
  if (dm2_dballochandler.v1e13fe[0] == 0)
  {
    RG1L = signedlong(RG1W);
    if (table1d6b76[4 * RG1L + 0x70] != 0)
      DM2_MARK_DYN_LOAD(lcon(0xd30ffff));
  }
  RG4L = 0;
  vo_1c.poke16(RG4W);
  RG4P = DOWNCAST(unk, xp_0c);
  //m_2B5BB:
  for (;;)
  {
    RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
    RG1W = word_at(RG1P, lcon(0xa)) & lcon(0xf);
    RG2W = vo_1c.peek16();
    if (RG1UW <= RG2UW)
    {
      if (dm2_dballochandler.v1e13fe[0] == 0)
      {
        //m_2B5E6:
        RG3L = 0;
        vo_1c.poke16(RG3W);
        //m_2B625:
        for (;;)
        {
          RG1L = unsignedlong(vo_1c.peek16());
          if (RG1L >= lcon(0xfa))
            break;
          //m_2B5EF:
          RG1P = DOWNCAST(unk, xp_0c) + RG1L;
          if (byte_at(RG1P) != 0)
          {
            RG1L = (unsignedlong(vo_1c.peek8()) << bcon(0x10)) + lcon(0x900ffff);
            DM2_MARK_DYN_LOAD(RG1L);
            RG4L = unsignedlong(vo_1c.peek8());
            RG2L = lcon(0x40);
            DM2_2676_00d0(lcon(0x9), RG4L, RG2L);
          }
          //m_2B620:
          vo_1c.inc16();
        }
      }
      else
        DM2_MARK_DYN_LOAD(lcon(0x9ffffff));
      break;
    }
    //m_2B59E:
    RG1P = RG4P + unsignedlong(ddat.v1e02cc[unsignedlong(RG2W)]);
    mov8(location(RG1P), 1);
    RG1L = RG2L + 1;
    vo_1c.poke16(RG1W);
  }
  //m_2B633:
  RG2L = 0;
  vo_1c.poke16(RG2W);
  //m_2B658:
  for (;;)
  {
    RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
    RG1W = word_at(RG1P, lcon(0xa));
    RG1L <<= 4;
    RG1UW >>= bcon(0xc);
    RG3W = vo_1c.peek16();
    if (RG1UW <= RG3UW)
    {
      if (dm2_dballochandler.v1e13fe[0] == 0)
      {
        //m_2B686:
        RG1L = 0;
        vo_1c.poke16(RG1W);
        RG72p = DOWNCAST(unk, xp_10);
        //m_2B6C7:
        for (;;)
        {
          RG1L = unsignedlong(vo_1c.peek16());
          if (RG1L >= lcon(0xfa))
            break;
          //m_2B693:
          RG1P = RG72p + RG1L;
          if (byte_at(RG1P) != 0)
          {
            RG1L = (unsignedlong(vo_1c.peek8()) << bcon(0x10)) + lcon(0xa00ffff);
            DM2_MARK_DYN_LOAD(RG1L);
          }
          //m_2B6AD:
          RG4L = unsignedlong(vo_1c.peek8());
          RG2L = lcon(0x40);
          DM2_2676_00d0(lcon(0xa), RG4L, RG2L);
          vo_1c.inc16();
        }
      }
      else
        DM2_MARK_DYN_LOAD(lcon(0xaffffff));
      break;
    }
    //m_2B63C:
    RG1P = DOWNCAST(unk, xp_10) + unsignedlong(ddat.v1e02dc[unsignedlong(RG3W)]);
    mov8(location(RG1P), 1);
    vo_1c.inc16();
  }
  //m_2B6D5:
  if (dm2_dballochandler.v1e13fe[0] == 0)
  {
    //m_2B6EA:
    RG3L = 0;
    vo_1c.poke16(RG3W);
    //m_2B718:
    for (;;)
    {
      RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
      RG1W = word_at(RG1P, lcon(0xc)) & lcon(0xf);
      RG4W = vo_1c.peek16();
      if (RG1UW <= RG4UW)
        break;
      //m_2B6F3:
      RG1L = unsignedlong(RG4W);
      RG1Blo = ddat.v1e0414[RG1L];
      RG1L = unsignedlong(RG1Blo) << bcon(0x10);
      RG1L += lcon(0xb00ffff);
      DM2_MARK_DYN_LOAD(RG1L);
      RG2L = RG4L + 1;
      vo_1c.poke16(RG2W);
    }
  }
  else
    DM2_MARK_DYN_LOAD(lcon(0xbffffff));
  //m_2B72E:
  if (dm2_dballochandler.v1e13fe[0] != 0 || flag2)
    DM2_MARK_DYN_LOAD(lcon(0xcffffff));
  //m_2B745:
  RG3L = 0;
  vo_1c.poke16(RG3W);
  //m_2B76C:
  for (;;)
  {
    RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
    RG1W = word_at(RG1P, lcon(0xc));
    RG1L <<= bcon(0x8);
    RG1UW >>= bcon(0xc);
    RG64W = vo_1c.peek16();
    if (RG1UW <= RG64UW)
    {
      if (dm2_dballochandler.v1e13fe[0] != 0)
        DM2_MARK_DYN_LOAD(lcon(0xfffffff));
      RG4W = unsignedword(RG4Blo);
      vb_34 = RG4Bhi;
      break;
    }
    //m_2B74E:
    RG4L = unsignedlong(RG64W);
    RG1P = ddat.v1e03c4;
    RG1Blo = byte_at(RG1P, RG4L);
    RG1L = unsignedlong(RG1Blo);
    mov8(location(RG51p + RG1L), 1);
    RG1L = RG64L + 1;
    vo_1c.poke16(RG1W);
  }
  //m_2B87B:
  for (;;)
  {
    if (mkub(vb_34) >= mkub(lcon(0xfffffffa)))
    {
      if (!dm2_dballochandler.v1e0a84)
        DM2_SOUND4();
      DM2_LOAD_DYN4(DOWNCAST(i16, ddat.v1e09a0), signedlong(ddat.v1e09a4));
      if (!dm2_dballochandler.v1e0a84)
        DM2_LOAD_MISCITEM();
      RG4L = unsignedlong(ddat.v1d6c02);
      RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x17), RG4Blo, 1, lcon(0x64)) ? 1 : 0;
      ddat.v1e1481 = RG1Blo;
      RG4L = unsignedlong(ddat.v1d6c02);
      RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x17), RG4Blo, 1, lcon(0x6a)) ? 1 : 0;
      ddat.v1e1479 = RG1Blo;
      RG4L = unsignedlong(ddat.v1d6c02);
      RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x17), RG4Blo, 1, lcon(0x67)) ? 1 : 0;
      ddat.v1e147a = RG1Blo;
      RG4L = unsignedlong(ddat.v1d6c02);
      RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x17), RG4Blo, 1, lcon(0x71)) ? 1 : 0;
      ddat.v1e147c = RG1Blo;
      RG4L = unsignedlong(ddat.v1d6c02);
      RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(8, RG4Blo, 11, 100);
      ddat.v1d6c00 = RG1W;
      RG4L = unsignedlong(ddat.v1d6c02);
      RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(8, RG4Blo, 11, lcon(0x65));
      ddat.v1e12d6 = RG1W;
      RG4L = unsignedlong(ddat.v1d6c02);
      RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(8, RG4Blo, 11, lcon(0x6d));
      ddat.v1e03f8 = RG1W;
      RG4L = unsignedlong(ddat.v1d6c02);
      RG3L = lcon(0x72);
      RG2L = lcon(0xb);
      RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(8, RG4Blo, 11, lcon(0x72));
      ddat.v1e0400 = RG1W;
      if (RG1W == 0)
        ddat.v1e0400 = 1;
      dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0xfa));
      dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0xfa));
      dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0xfa));
      dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0x960));
      RG2L = dm2_dballochandler.v1e0a84 ? 1 : 0;
      if (RG2L != 0)
        return;
      DM2_QUERY_GDAT_IMAGE_METRICS(8, ddat.v1d6c02, RG2Blo, &vw_14, &vw_30);
      RG5W = vw_14 * vw_30;
      DM2_QUERY_GDAT_IMAGE_METRICS(8, ddat.v1d6c02, 1, &vw_24, &vw_18);
      RG3W = vw_18; // adj
      RG4W = vw_24 * RG3W; // adj
      RG1W = ddat.v1d2708 - RG4W; // adj
      gfxsys.backbuff1 = RG1W;
      if (RG5UW >= RG1UW)
      {
        //m_2BAA3:
        RG1L = 0;
        gfxsys.backbuff1 = RG1W;
      }
      else
      {
        RG4W = RG1W - RG5W; // adj
        gfxsys.backbuff1 = RG4W;
        RG1W = RG3W + vw_30; // adj
        RG4W = gfxsys.backbuffer_h - RG1W; // adj
        RG1L = signedlong(RG4W);
        put16(parw02, RG1W);
        RG3L = signedlong(gfxsys.backbuffer_w);
        RG2L = signedlong(vw_18);
        RG4L = 0;
        RG1R = &gfxsys.backbuffrect;
        RG1R->set(RG4W, RG2W, RG3W, parw02);
      }
      break;
    }
    //m_2B7A3:
    RG2L = unsignedlong(vb_34);
    RG4L = 6;
    RG1L = DM2_QUERY_GDAT_CREATURE_WORD_VALUE(RG2L, RG4L);
    if (RG1W != 0)
      mov8(location(RG51p + RG2L), 1);
    RG4P = RG51p + unsignedlong(vb_34);
    if (byte_at(RG4P) != 0 && dm2_dballochandler.v1e13fe[0] == 0)
    {
      RG2L = unsignedlong(vb_34) << bcon(0x10);
      RG1L = RG2L + lcon(0xf0007fd);
      DM2_MARK_DYN_LOAD(RG1L);
      RG1L = RG2L + lcon(0xf0007fe);
      DM2_MARK_DYN_LOAD(RG1L);
      RG1Blo = byte_at(RG4P) & 1;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L == 0 && dm2_dballochandler.v1e13fe[1] == 0)
      {
        //m_2B84A:
        RG1L = RG2L + lcon(0xf00fffa);
        DM2_2676_008f(RG1L, lcon(0xfd));
        RG4L = signedlong(ddat.v1e09a4) - 2;
        RG1L = 3 * RG4L;
        ddat.v1e09a0[RG1L] &= lcon(0x7fff);
      }
      else
      {
        RG4L = unsignedlong(vb_34) << bcon(0x10);
        RG1L = RG4L + lcon(0xf00ffff);
        DM2_MARK_DYN_LOAD(RG1L);
        RG1L = RG4L + lcon(0xf0001fa);
        DM2_2676_008f(RG1L, lcon(0xfd));
        DM2_2676_00d0(lcon(0xf), unsignedlong(vb_34), lcon(0x40));
      }
    }
    //m_2B877:
    vb_34++;
  }
  //m_2BAAB:
  gfxsys.backbuff2 = 1;
  DM2_UPDATE_WEATHER(0);
  ddat.v1e0390.l_00 = 1;
  RG1L = DM2_CHECK_RECOMPUTE_LIGHT();
}

void DM2_LOAD_LOCALLEVEL_GRAPHICS_TABLE(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  i16 RG6w;

  ddat.v1e0270 = RG1W;
  ddat.v1e0272 = RG4W;
  ddat.v1e0266 = RG2W;
  RG1L = signedlong(RG2W);
  DM2_CHANGE_CURRENT_MAP_TO(RG1L);
  RG3L = signedlong(mapdat.map_height);
  RG1P = DOWNCAST(t_tile, mapdat.map[mapdat.map_width - 1]);
  RG3P = RG1P + RG3L;
  ddat.v1e03c4 = RG3P;
  RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
  RG6w = word_at(RG1P, lcon(0xa)) & wcon(0xf);
  RG2L = signedlong(RG6w);
  RG1W = word_at(RG1P, lcon(0xc));
  RG1L <<= bcon(0x8);
  RG1UW >>= bcon(0xc);
  RG1L = unsignedlong(RG1W);
  RG3P += RG1L;
  DM2_COPY_MEMORY(ddat.v1e02cc, RG3P, RG2L);
  RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
  RG1W = word_at(RG1P, lcon(0xa));
  RG1L <<= 4;
  RG1UW >>= bcon(0xc);
  RG5l = signedlong(RG1W);
  RG1L = signedlong(RG6w);
  RG3P += RG1L;
  DM2_COPY_MEMORY(ddat.v1e02dc, RG3P, RG5l);
  RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
  RG1W = word_at(RG1P, lcon(0xc)) & lcon(0xf);
  DM2_COPY_MEMORY(ddat.v1e0414, RG3P + RG5l, unsignedlong(RG1W));
}

// belongs to DM2_LOAD_NEWMAP()
void DM2_3a15_38b6(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG51p;
  i8* RG52p;
  c_wlreg _RG6;
  c_wlreg _RG7;
  i32 vl_00;
  unk* xp_04; // add32
  unk* xp_08;
  i32 vl_0c;
  i16 vw_10;
  c_o32 vo_14; // poke 16 peek 16 32 inc16
  i16 vw_18;
  c_o32 vo_1c; // poke 16 peek 16 32 inc16
  i16 vw_20;
  i16 parw00;

  vl_00 = RG1L;
  vw_20 = ddat.v1d3248;
  DM2_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
  RG4L = signedlong(ddat.v1d3248);
  RG1L = 16 * RG4L;
  RG2P = ddat.v1e03c8;
  RG1P = RG2P + RG1L;
  RG2W = word_at(RG1P, lcon(0x8));
  RG2L <<= bcon(0x5);
  _RG2.ushiftr11();
  vw_18 = RG2W; // truncated
  RG1W = word_at(RG1P, lcon(0x8));
  _RG1.ushiftr11();
  vw_10 = RG1W; // truncated
  xp_08 = UPCAST(unk, pointer_at(pointer_at(DOWNCAST(t_tile*, mapdat.tmpmap) + 4 * RG4L)));
  RG1P = DOWNCAST(i16, ddat.v1e03e4);
  RG1W = word_at(RG1P, 2 * RG4L);
  RG4L = 2 * (RG1L & lcon(0xffff));
  RG1L = 2 * unsignedlong(word_at(ddat.v1e03d8, RG4L));
  xp_04 = UPCAST(unk, DOWNCAST(t_record, dm2_v1e038c + RG1L / 2));
  RG4L = 0;
  vo_14.poke16(RG4W);
  //m_49AC5:
  for (;;)
  {
    RG4L = signedlong(vo_14.peek16());
    RG1L = unsignedlong(vw_18);
    if (RG4L > RG1L)
    {
      DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_20));
      return;
    }
    //m_498D4:
    RG2L = 0;
    vo_1c.poke16(RG2W);
    //m_49AAA:
    for (;;)
    {
      RG4L = signedlong(vo_1c.peek16());
      RG1L = unsignedlong(vw_10);
      if (RG4L > RG1L)
      {
        vo_14.inc16();
        break;
      }
      //m_498E0:
      RG1L = 0;
      RG4P = DOWNCAST(unk, xp_08);
      RG1Blo = byte_at(RG4P);
      RG1L >>= bcon(0x5);
      RG1Blo = RG1Blo == bcon(0x0) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      vl_0c = RG1L;
      RG1P = RG4P;
      RG4P++;
      xp_08 = UPCAST(unk, RG4P);
      if (jnz_test8(location(RG1P), lcon(0x10)))
      {
        RG6W = word_at(DOWNCAST(unk, xp_04));
        xp_04 += 2; // DANGER
        //m_49915:
        for (;;)
        {
          RG1L = RG6L;
          RG1W &= lcon(0x3c00);
          RG1L = unsignedlong(RG1W) >> bcon(0xa);
          RG4L = signedlong(RG1W);
          if (RG4L > lcon(0x3))
            break;
          if (RG1L != lcon(0x3))
          {
            //m_49A14:
            if (RG4L == 2)
            {
              RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG6UW));
              RG51p = RG1P;
              RG1W = word_at(RG1P, 2) & lcon(0x6);
              if (RG1W == 2)
              {
                RG1W = word_at(RG51p, 2);
                RG4L = RG1L;
                RG4UW >>= bcon(0x3);
                RG4L = unsignedlong(RG4W);
                RG4UL >>= bcon(0x8);
                RG4L &= lcon(0x1f);
                if (RG4L == lcon(0x15))
                {
                  if (vl_00 != 0)
                  {
                    if ((RG1Blo & bcon(0x1)) == 0)
                    {
                      RG1Blo |= 1;
                      mov16(location(RG51p + 2), RG1W);
                      i32 parl02 = unsignedlong(vo_1c.peek16());
                      RG3L = unsignedlong(vo_14.peek16());
                      RG1L = signedlong(DM2_RANDDIR());
                      RG2L = unsignedlong(RG1W);
                      RG1W = word_at(RG51p, 2);
                      RG1UW >>= bcon(0x3);
                      RG1W = unsignedword(RG1Blo);
                      RG1L = unsignedlong(RG1W);
                      dm2_dballochandler.DM2_ALLOC_NEW_CREATURE(RG1W, 7, RG2W, RG3W, CUTX16(parl02));
                    }
                  }
                }
              }
            }
          }
          else
          {
            RG7L = unsignedlong(RG6W);
            RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG7UW));
            RG52p = RG1P;
            RG4W = word_at(RG1P, 2) & lcon(0x7f);
            if (RG4UW >= mkuw(lcon(0x21)))
            {
              if (RG4UW <= mkuw(lcon(0x21)))
              {
                bool skip01036 = false;
                //m_49965:
                RG4W = word_at(RG1P, lcon(0x4)) & lcon(0x18);
                if (RG4W != lcon(0x18))
                {
                  //m_4999F:
                  RG4Blo = jz_test8(location(RG1P + lcon(0x4)), lcon(0x20)) ? 1 : 0;
                  RG4L = unsignedlong(RG4Blo);
                  if (RG4L == vl_00)
                  {
                    RG4W = word_at(RG1P, lcon(0x4));
                    RG4L <<= 11;
                    RG4UW >>= 14;
                    RG4L = unsignedlong(RG4W);
                    RG2L = 0;
                    skip01036 = true;
                  }
                }
                else
                {
                  RG2L = 0;
                  RG4W = word_at(RG1P, lcon(0x4));
                  RG4L <<= bcon(0xa);
                  RG4UW >>= bcon(0xf);
                  RG4L = unsignedlong(RG4W);
                  RG4L ^= vl_00;
                  RG4Blo = RG4L == 0 ? 1 : 0;
                  RG4W = unsignedword(RG4Blo);
                  RG4L = unsignedlong(RG4W);
                  skip01036 = true;
                }

                if (skip01036)
                  //m_49995:
                  DM2_INVOKE_ACTUATOR(RG1P, RG4L, RG2L);
              }
              else
              {
                if (RG4W == lcon(0x2c))
                {
                  //m_499CA:
                  if (vl_00 != 0)
                  {
                    RG1W = word_at(RG52p, lcon(0x4)) & 1;
                    if (RG1W == 1)
                    {
                      RG4L = vl_0c;
                      i32 parl01 = RG4L;
                      RG1L = DM2_GET_ORNATE_ANIM_LEN(RG52p, RG4L) & lcon(0xffff);
                      put16(parw00, RG1W);
                      RG3L = signedlong(vo_1c.peek16());
                      RG2L = signedlong(vo_14.peek16());
                      RG4L = RG7L;
                      RG1P = RG52p;
                      DM2_TRY_ORNATE_NOISE(RG1P, RG4L, RG2L, RG3L, parw00, parl01);
                    }
                  }
                }
              }
            }
          }
          //m_49A94:
          RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG6UW)));
          RG6L = RG1L;
        }
      }
      //m_49AA5:
      vo_1c.inc16();
    }
  }
  FOREND
}

void DM2_LOAD_NEWMAP(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;

  RG5w = RG1W;
  if (!dm2_dballochandler.v1e0a84)
  {
    DM2_HIDE_MOUSE();
    if (RG3L != 0)
      DM2_3a15_38b6(0);
  }
  RG2L = signedlong(RG2W);
  RG4L = signedlong(RG4W);
  RG1L = signedlong(RG5w);
  DM2_LOAD_LOCALLEVEL_GRAPHICS_TABLE(RG1L, RG4L, RG2L);
  DM2_LOAD_LOCALLEVEL_DYN();
DEBUG_DUMP_ULP();

  if (dm2_dballochandler.v1e0a84)
    return;
  DM2_3a15_38b6(1);
  RG1L = DM2_FILL_CAII_CUR_MAP();
  ddat.v1e0390.b_00 |= 2;
  RG1L = DM2_CHECK_RECOMPUTE_LIGHT();
  if (dm2_dballochandler.v1e13fe[0] == 0)
    eventqueue.event_1031_098e();
  DM2_SHOW_MOUSE();
}

