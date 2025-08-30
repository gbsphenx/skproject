#include <dm2data.h>
#include <util.h>
#include <sktimer.h>
#include <skguivwp.h>
#include <sklight.h>
#include <sktimprc.h>
#include <skgdtqdb.h>
#include <sfx.h>
#include <skrandom.h>
#include <skmap.h>
#include <skrect.h>
#include <skxrect.h>
#include <skcloud.h>
#include <xtypes.h>
#include <skweathr.h>

// TODO: optimize temporaries
#include <regs.h>
#include <emu.h>

// was SKW_3df7_000a
void DM2_SET_TIMER_WEATHER(i32 eaxl)
{
  c_tim tim;

  tim.setmticks(0, timdat.gametick + eaxl);
  tim.settype(0x54);
  tim.setactor(0);
  DM2_QUEUE_TIMER(&tim);
}

// was SKW_3df7_014d
void DM2_UPDATE_WEATHER(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG51w;
  c_wlreg _RG52;
  i16 RG54w;
  i16 RG61w;
  i16 RG62w;
  i32 RG71l;
  i32 RG72l;
  c_wlreg _RG73;
  s_8wordarray s15_00;
  c_5bytes c12_10;
  i32 vl_18;
  unk* xp_1c; // add32
  i32 vl_20;
  i32 vl_24;
  c_ql32 vql_28;
  c_ql32 vql_2c;
  i16 parw01;
  i16 parw02;
  i16 parw05;
  unk* xparp07;
  i16 parw09;
  i16 parw10;
  i16 parw11;
  bool flag;

  RG4L = RG1L;
  RG1L = signedlong(ddat.v1e1472);
  RG1Blo = table1d6b76[4 * RG1L + 0x70];
  ddat.v1e147f = RG1Blo;
  if (RG4L != 0)
  {
    RG3Bhi = ddat.v1e147b + 1;
    ddat.v1e147b = RG3Bhi;
    RG1L = unsignedlong(RG3Bhi);
    if (RG1L > lcon(0x1f))
    {
      DM2_weather_3df7_0037(0);
      return;
    }
    ddat.v1e146e = ddat.v1e1474;
    RG4L = unsignedlong(ddat.v1e1478) << 5;
    RG4W = signedword(ddat.v1d7108[RG4L + RG1L]);
    RG1L = unsignedlong(ddat.v1e1484) * RG4L;
    ddat.v1e1474 += RG1W;
    RG4L = signedlong(ddat.v1e1474);
    RG2L = lcon(0xff);
    RG1L = signedlong(DM2_BETWEEN_VALUE(0, RG2W, RG4W));
    ddat.v1e1474 = RG1W;
    RG1W = (DM2_RAND16(256)) + 50;
    DM2_SET_TIMER_WEATHER(signedlong(RG1W));
    return;
  }
  //m_4A1D8:
  RG1L = timdat.gametick;
  if (mkul(RG1L) >= mkul(ddat.v1e1434))
  {
    RG1L += ddat.v1e1438;
    RG1L /= lcon(0x555);
    RG2L = lcon(0x18);
    RG4L = RG1L % RG2L;
    RG1L /= RG2L;
    RG1W = signedword(table1d70f0[RG4L]);
    ddat.v1e1476 = RG1W;
    RG1L = timdat.gametick + lcon(0x555);
    ddat.v1e1434 = RG1L;
    if (ddat.v1e147f != 0)
      RG1L = DM2_RECALC_LIGHT_LEVEL();
  }
  vql_28.poke32(0);
  if (ddat.v1e1474 == 0)
  {
    //m_4A32D:
    if (mkub(ddat.v1e147d) > ubcon(0x0))
    {
      RG2L = 3;
      RG4L = timdat.gametick % RG2L;
      if (RG4L == 0)
        ddat.v1e147d--;
    }
    RG1W = DM2_RAND16(64);
    RG1Blo = RG1W == 0 ? 1 : 0;
    RG1W = unsignedword(RG1Blo);
    vql_28.poke32(RG1L);
    ddat.v1e1482 = RG1Bhi;
    ddat.v1e147e = 1;
  }
  else
  {
    RG1L = DM2_RAND();
    RG1W &= lcon(0xf);
    RG2L = lcon(0x100);
    RG2W -= ddat.v1e1474;
    RG2L += RG1L;
    RG1L = signedlong(ddat.v1e1474);
    if (RG1L < lcon(0xcd))
      //m_4A269:
      RG1L = lcon(0x7);
    else
      RG1L = lcon(0x28);
    //m_4A26E:
    RG51w = RG1W;
    RG1Blo = CUTX8(ddat.v1e1474);
    ddat.v1e147e = RG1Blo;
    if (ddat.v1e1482 == 0)
    {
      RG1W = DM2_RAND16(RG2W);
      RG1Blo = RG1W <= 7 ? 1 : 0;
    }
    ddat.v1e1482 = RG1Blo;
    RG3Bhi = ddat.v1e1482;
    if (RG3Bhi != 0)
    {
      RG1L = unsignedlong(ddat.v1e147d);
      if (RG1L < lcon(0xff))
      {
        bool skip01038 = false;
        RG4L = unsignedlong(RG3Bhi);
        RG1Blo++;
        if (RG4L < lcon(0x80))
        {
          //m_4A2CA:
          if (RG4L >= lcon(0x40) && (CUTLX8(timdat.gametick) & bcon(0x1)) == 0)
            skip01038 = true;
          else
          {
            bool skip01040 = false;
            bool skip01039 = false;
            RG1L = unsignedlong(ddat.v1e1482);
            if (RG1L < lcon(0x10))
              skip01039 = true;
            else
            {
              RG3L = 3;
              RG4L = timdat.gametick % RG3L;
              if (RG4L != 0)
                skip01039 = true;
            }

            if (skip01039)
            {
              //m_4A2F6:
              if ((CUTLX8(timdat.gametick) & bcon(0x3)) == 0)
                skip01040 = true;
            }
            else
              skip01040 = true;

            if (skip01040)
              //m_4A2FF:
              ddat.v1e147d++;
          }
        }
        else
          skip01038 = true;

        if (skip01038)
          //m_4A2C3:
          ddat.v1e147d = RG1Blo;
      }
    }

    //m_4A305:
    if (ddat.v1e1481 != 0)
    {
      RG1W = DM2_RAND16(RG2W);
      RG4L = unsignedlong(RG1W);
      RG1L = signedlong(RG51w);
      RG1Blo = RG4L <= RG1L ? 1 : 0;
      RG1W = unsignedword(RG1Blo);
      vql_28.poke32(RG1L);
    }
  }

  //m_4A371:
  if (ddat.v1e147f == 0)
    return;
  if (ddat.v1e024c != 0)
  {
    ddat.v1e024c = 0;
    RG1L = DM2_RECALC_LIGHT_LEVEL();
  }
  xp_1c = UPCAST(unk, DOWNCAST(s_hexa, ddat.v1e143c));
  if (ddat.v1e147a != 0)
  {
    RG1L = unsignedlong(ddat.v1e147e);
    if (RG1L >= lcon(0x10))
    {
      if (RG1L < lcon(0x80))
      {
        //m_4A3C8:
        if (RG1L < lcon(0x40))
          //m_4A3D6:
          ddat.v1e143c[0].barr_00[0] = 0x67;
        else
          ddat.v1e143c[0].barr_00[0] = 0x68;
      }
      else
      {
        ddat.v1e1480 = 1;
        ddat.v1e143c[0].barr_00[0] = 0x69;
      }
      //m_4A3DD:
      RG1L = DM2_RETRIEVE_ENVIRONMENT_CMD_CD_FW(DOWNCAST(unk, xp_1c));
      if (RG1L != 0)
        xp_1c += lcon(0xa); // DANGER
    }
  }
  //m_4A3EF:
  if (ddat.v1e1479 != 0)
  {
    RG1L = unsignedlong(ddat.v1e147d);
    if (RG1L >= lcon(0x40))
    {
      if (RG1L < lcon(0xc0))
      {
        //m_4A414:
        if (RG1L < lcon(0x80))
          //m_4A424:
          mov8(location(DOWNCAST(unk, xp_1c)), lcon(0x6a));
        else
          mov8(location(DOWNCAST(unk, xp_1c)), lcon(0x6b));
      }
      else
        mov8(location(DOWNCAST(unk, xp_1c)), lcon(0x6c));
      //m_4A42B:
      RG1L = DM2_RETRIEVE_ENVIRONMENT_CMD_CD_FW(DOWNCAST(unk, xp_1c));
      if (RG1L != 0)
        xp_1c += lcon(0xa); // DANGER
    }
  }
  //m_4A43D:
  if (vql_28.peek16() != 0)
  {
    vql_28.poke32(0);
    RG1L = signedlong(ddat.v1e1474);
    RG1Blo = RG1L < lcon(0xb6) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    flag = RG1L == 0;
    vl_18 = RG1L;
    if (!flag)
      DM2_UPDATE_GLOB_VAR(lcon(0x40), 0, lcon(0x6));
    RG1L = 0;
    vql_2c.poke32(RG1L);
    RG1W = DM2_RAND16(mkuw(ddat.v1e1474 + 1));
    if (RG1W >= 60)
    {
      RG1L = DM2_RAND();
      RG1W &= lcon(0x7);
      RG1L++;
      vql_2c.poke32(RG1L);
      RG4L = unsignedlong(ddat.v1d6c02);
      RG3L = lcon(0x6c);
      RG2L = lcon(0xb);
      RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(8, RG4Blo, RG2Blo, RG3Blo);
      if (RG1W == 0)
      {
        //m_4A537:
        for (;;)
        {
          bool skip01044 = false;
          RG1W = DM2_RAND16(32);
          RG4W = RG1W;
          RG61w = RG1W;
          RG52W = DM2_RAND16(32);
          if (RG4W >= 0 && RG4W < mapdat.map_width)
            //m_4A566:
            RG1L = 0;
          else
            RG1L = 1;
          //m_4A568:
          if (RG1L == 0)
          {
            if (RG52W < 0 || RG52W >= mapdat.map_height)
              RG1L = 1;
            if (RG1L == 0)
            {
              RG71l = signedlong(RG52W);
              RG1L = signedlong(RG61w);
              vl_24 = RG1L;
              RG3L = RG71l;
              RG2L = RG1L;
              DM2_SUMMARIZE_STONE_ROOM(&s15_00, 0, RG2L, RG3L);
              RG1L = signedlong(s15_00.warr_00[0x0]);
              if (RG1L == 1)
              {
                RG2L = RG71l;
                RG4L = vl_24;
                RG1L = DM2_GET_TELEPORTER_DETAIL(&c12_10, RG4L, RG2L);
                if (RG1L == 0)
                {
                  bool skip01043 = false;
                  RG1W = ddat.v1e0266;
                  if (RG1W != ddat.v1d3248 || RG61w != ddat.v1e0270 || RG52W != ddat.v1e0272)
                    skip01043 = true;
                  else
                  {
                    RG1L = signedlong(ddat.v1e1474);
                    if (RG1L >= lcon(0xb6) && mkul(timdat.gametick) >= mkul(lcon(0x2ee0)))
                    {
                      RG1L = DM2_GET_GLOB_VAR(lcon(0x40));
                      if (RG1W > ddat.v1e1470)
                        skip01043 = true;
                    }
                  }

                  if (skip01043)
                  {
                    //m_4A624:
                    RG4L = unsignedlong(RG52W);
                    RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG61w), RG4L);
                    if (RG1W == lcon(0xffffffff))
                      skip01044 = true;
                    else
                    {
                      RG1L = unsignedlong(byte_at(DM2_QUERY_CREATURE_AI_SPEC_FROM_TYPE(unsignedlong(RG1W)), 2));
                      if (RG1L != lcon(0xff))
                      {
                        RG1L = signedlong(RG52W);
                        vl_20 = RG1L;
                        RG72l = signedlong(RG61w);
                        RG4L = signedlong(ddat.v1e0272);
                        RG3L = vl_20;
                        RG2L = RG72l;
                        RG1L = signedlong(DM2_CALC_SQUARE_DISTANCE(ddat.v1e0270, RG4W, RG2W, RG3W));
                        if (RG1L <= 2)
                          skip01044 = true;
                        else
                        {
                          if (RG1L <= lcon(0x3))
                          {
                            RG4L = signedlong(ddat.v1e0272);
                            RG3L = vl_20;
                            RG2L = RG72l;
                            RG4L = unsignedlong(DM2_CALC_VECTOR_DIR(ddat.v1e0270, RG4W, RG2W, RG3W));
                            RG1L = signedlong(ddat.v1e0258);
                            if (RG1L == RG4L)
                              skip01044 = true;
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }

          if (skip01044)
          {
            //m_4A6C0:
            RG3L = unsignedlong(RG52W);
            RG2L = unsignedlong(RG61w);
            DM2_CREATE_CLOUD(lcon(0xffb0), unsignedlong(ddat.v1e1474), RG2L, RG3L, lcon(0xff));
            if (ddat.v1e0976 != 0 || ddat.v1e0238 != 0)
            {
              if (vl_18 == 0)
              {
                RG52L = signedlong(RG52W);
                RG73L = signedlong(RG61w);
                RG4L = signedlong(ddat.v1e0272);
                RG1L = signedlong(DM2_CALC_SQUARE_DISTANCE(ddat.v1e0270, RG4W, RG73W, RG52W));
                if (RG1L <= lcon(0x3))
                {
                  RG4L = signedlong(ddat.v1e0272);
                  RG1L = signedlong(DM2_CALC_VECTOR_DIR(ddat.v1e0270, RG4W, RG73W, RG52W));
                  RG4L = unsignedlong(RG1W);
                  RG1L = signedlong(ddat.v1e0258);
                  if (RG1L == RG4L)
                    DM2_UPDATE_GLOB_VAR(lcon(0x41), 1, lcon(0x3));
                }
              }
            }
            break;
          }

          //m_4A76B:
          RG3L = vql_2c.peek32();
          flag = --RG3W != 0;
          vql_2c.poke32(RG3L);
          if (!flag)
            break;
        }
      }
      else
      {
        RG4L = timdat.gametick + 1;
        i32 parl00 = RG4L;
        RG54w = RG1W & wcon(0xff);
        RG4L = unsignedlong(RG54w);
        RG1L = unsignedlong(RG1W) >> bcon(0x8);
        RG62w = RG1W;
        RG1L = unsignedlong(RG62w);
        DM2_INVOKE_MESSAGE(RG1L, RG4L, 0, 0, parl00);
        RG1W = DM2_RAND16(160);
        put16(parw05, RG1W);
        RG1L = signedlong(RG54w);
        put16(parw02, RG1W);
        RG1L = signedlong(RG62w);
        put16(parw01, RG1W);
        DM2_QUEUE_NOISE_GEN2(lcon(0xd), lcon(0x30), lcon(0x81), lcon(0xfe), parw01, parw02, 1, lcon(0x6c), parw05);
      }
    }
    //m_4A77B:
    if (vql_2c.peek16() == 0)
    {
      RG1L = DM2_RANDBIT() ? 1 : 0;
      if (RG1W != 0)
      {
        RG1W = DM2_RAND16(3);
        RG1Blo += 100;
        RG4P = DOWNCAST(unk, xp_1c);
        mov8(location(RG4P), RG1Blo);
        RG1L = DM2_RETRIEVE_ENVIRONMENT_CMD_CD_FW(RG4P);
        if (RG1L != 0)
        {
          vql_28.poke32(1);
          xparp07 = RG4P + lcon(0x6);
          RG3P = RG4P + lcon(0x4);
          RG1W = DM2_RAND16(100);
          RG2L = signedlong(RG1W);
          RG1W = word_at(RG4P, 2);
          RG1L++;
          RG4L = signedlong(RG1W);
          RG1L = signedlong(word_at(DOWNCAST(unk, xp_1c), 2));
          DM2_rect_098d_04c7(RG1W, RG4W, RG2W, RG3P16, XUPCAST(i16, xparp07));
          RG1L = signedlong(DM2_RANDDIR());
          RG4P = DOWNCAST(unk, xp_1c);
          mov8(location(RG4P + 1), RG1Blo);
          xp_1c = UPCAST(unk, RG4P + lcon(0xa));
        }
      }
    }
    //m_4A7EF:
    if (vql_28.peek16() != 0)
    {
      if (ddat.v1d718c != 0)
        //m_4A891:
        ddat.v1d718c = 0;
      else
      {
        if (ddat.v1e1474 != 0)
        {
          //m_4A823:
          RG4L = signedlong(ddat.v1e1474);
          RG2L = unsignedlong(ddat.v1e1484);
          RG1L = RG4L / RG2L;
          RG4L = lcon(0x4c) - RG1L;
          RG1L = RG4L;
        }
        else
          RG1W = DM2_RAND16(10) + 5;
        //m_4A844:
        RG4L = signedlong(RG1W);
        RG2L = lcon(0xf);
        RG1L = signedlong(DM2_BETWEEN_VALUE(1, RG2W, RG4W));
        put16(parw11, RG1W);
        put16(parw10, ddat.v1e0272);
        put16(parw09, ddat.v1e0270);
        DM2_QUEUE_NOISE_GEN1(lcon(0x17), ddat.v1d6c02, 0, lcon(0x19), lcon(0x40), parw09, parw10, parw11);
        ddat.v1d718c = 1;
      }
    }
    //m_4A899:
    ddat.v1e024c = 1;
    DM2_RECALC_LIGHT_LEVEL();
  }
  //m_4A8A8:
  mov8(location(DOWNCAST(unk, xp_1c)), lcon(0xffffffff));
}

i32 DM2_weather_3df7_0037(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  if (RG1L == 0)
  {
    //m_4A013:
    DM2_UPDATE_GLOB_VAR(lcon(0x40), 0, lcon(0x6));
    RG1L = timdat.gametick + lcon(0x555);
    ddat.v1e1434 = RG1L;
    RG1W = unsignedword(RG1Blo);
    ddat.v1e1480 = RG1Bhi;
    ddat.v1e147f = RG1Bhi;
    RG4L = ddat.v1d7188;
    if (RG4L == 0)
    {
      //m_4A070:
      RG1W = DM2_RAND16(8000);
      RG4W = RG1W;
      RG4L += lcon(0x1f4);
      RG1L = signedlong(DM2_RANDDIR());
      ddat.v1e1478 = RG1Blo;
      RG1W = DM2_RAND16(3);
      RG1Blo++;
      ddat.v1e1484 = RG1Blo;
    }
    else
    {
      ddat.v1e147d = RG1Bhi;
      RG1W = DM2_RAND16(500);
      RG4L = unsignedlong(RG1W);
      ddat.v1e1478 = 3;
      ddat.v1e1484 = 1;
    }
    //m_4A09E:
    ddat.v1e147e = 1;
    RG3W = unsignedword(RG3Blo);
    ddat.v1e1482 = RG3Bhi;
    RG2L = 0;
    ddat.v1e1474 = RG2W;
    RG3Blo = 0;
    ddat.v1e146e = RG3W;
    ddat.v1e147b = 0;
    RG1L = signedlong(DM2_RANDDIR());
    ddat.v1e1483 = RG1Blo;
    DM2_SET_TIMER_WEATHER(RG4L);
  }
  else
  {
    ddat.v1e146e = wcon(0x0);
    if (ddat.v1e1484 == 0)
      ddat.v1e1484 = 1;
  }
  //m_4A0D7:
  RG1W = DM2_RAND16(4) + 4;
  ddat.v1e1470 = RG1W;
  RG1L = (timdat.gametick + ddat.v1e1438) / lcon(0x555);
  RG2L = lcon(0x18);
  RG4L = RG1L % RG2L;
  RG1L /= RG2L;
  RG1W = signedword(table1d70f0[RG4L]);
  ddat.v1e1476 = RG1W;
  ddat.v1d7188 = 0;
  return RG1L;
}
