#include <dm2data.h>
#include <skhero.h>
#include <skeventq.h>
#include <skmovrec.h>
#include <sklodlvl.h>
#include <gfxmain.h>
#include <skmap.h>
#include <skguivwp.h>
#include <skrandom.h>
#include <bgdat.h>
#include <bcreatur.h>
#include <sfxsnd.h>
#include <skguidrw.h>
#include <skgdtqdb.h>
#include <bitem.h>
#include <skcloud.h>
#include <SK1C9A.h>
#include <mdata.h>
#include <startend.h>
#include <sfx.h>
#include <util.h>
#include <sktimer.h>
#include <sktimprc.h>
#include <skmove.h>

#include <emu.h>
#include <regs.h>

#include <dm2debug.h>

// belongs to DM2_12b4_0881
static i32 DM2_12b4_0953(unk* xeaxp, i32 edxl)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2;
  c_nreg _RG4; RG4L = edxl;

  RG2W = word_at(RG1P, lcon(0xe));
  RG2L <<= bcon(0x6);
  RG2UW >>= 14;
  RG2L = unsignedlong(RG2W);
  RG4L = signedlong(RG4W);
  RG2L += RG4L;
  RG2L &= 0x3;
  RG4L = signedlong(RG2W);
  if (RG4L != lcon(0x1))
  {
    if (RG4L != lcon(0x3))
      return 0;
  }
  RG4L = signedlong(RG2W);
  RG1L = DM2_QUERY_CREATURE_5x5_POS(RG1P, RG4L);
  RG4L = signedlong(RG1W);
  RG2L = 5;
  RG1L = RG4L;
  RG4L = RG1L % RG2L;
  RG1L /= RG2L;
  RG4L -= 2;
  return RG4L == 0 ? 0 : lcon(0x1);
}

// belongs to DM2_PERFORM_MOVE
// stacksize was 0xc
static i32 DM2_12b4_0881(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i16* argpw1)
{
  c_nreg _RG4; RG4L = edxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG1; RG1L = eaxl;
  i16 RG5w;
  i16 vw_00;

  RG5w = RG1W;
  RG1L = RG2L;
  vw_00 = RG3W; // adj
  RG2P = DOWNCAST(i16, argpw1);
  mov16(location(RG2P), lcon(0xffffffff));
  RG4L = unsignedlong(RG4W) >> 5;
  RG4L = unsignedlong(RG4Blo);
  RG4Blo = RG4L == lcon(0x3) ? 1 : 0;
  RG4W = unsignedword(RG4Blo);
  if (RG4W != 0)
  {
    RG4L = signedlong(RG5w);
    if (RG4L == 2)
      return lcon(0x1);
  }
  RG4L = unsignedlong(RG1W) >> 5 & lcon(0xff);
  if (RG4L == lcon(0x3))
    return 2;
  RG1L = DM2_IS_TILE_BLOCKED(unsignedlong(RG1Blo));
  if (RG1L != 0)
    return lcon(0x3);
  RG4L = unsignedlong(argw0);
  RG1W = vw_00; // TODO: gets extended w/o conversion now
  RG1L = DM2_GET_CREATURE_AT(RG1L, RG4L);
  mov16(location(RG2P), RG1W);
  if (RG1W != lcon(0xffffffff))
  {
    RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG1W));
    if ((RG1Bhi & bcon(0x80)) == 0)
    {
      RG4L = signedlong(ddat.v1e0258);
      RG1L = DM2_12b4_0953(DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(word_at(RG2P))))), RG4L);
      return RG1L == 0 ? lcon(0x4) : lcon(0x5);
    }
  }
  RG2L = lcon(0xff);
  RG4P = DOWNCAST(i16, &argw0);
  RG1L = DM2_query_1c9a_03cf(&vw_00, RG4P16, RG2L);
  if (RG1W == lcon(0xffffffff))
    return lcon(0x6);
  RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG1W));
  return (RG1Bhi & bcon(0x80)) == 0 ? lcon(0x5) : lcon(0x6);
}

static void DM2_move_12b4_023f(i32 eaxl, i32 edxl, i16* ebxpw, i16* ecxpw, i32 argl0, i32 argl1)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = DOWNCAST(i16, ebxpw);
  c_nreg _RG3; RG3P = DOWNCAST(i16, ecxpw);
  c_nreg _RG4; RG4L = edxl;
  i32 RG51l;
  i32 RG52l;
  i8* RG61p;
  i16 RG62w;
  i32 RG7l;
  c_ql32 vql_00;
  c_ql32 vql_04;
  i16 vwa_08[0xa];
  i16 parw00;
  i16 parw01;

  vql_04.poke32(RG1L);
  vql_00.poke32(RG4L);
  RG61p = RG2P;
  RG1L = argl0 + 2;
  RG4L = argl1 + RG1L;
  argl1 = RG4L;
  RG1L = RG4L;
  RG1Bhi ^= RG4Bhi;
  RG1Blo &= 0x3;
  RG2L = unsignedlong(RG1W);
  RG51l = signedlong(vql_00.peek16());
  RG7l = signedlong(vql_04.peek16());
  RG4L = RG51l;
  RG1L = DM2_move_2c1d_028c(RG7l, RG4L, RG2L);
  vwa_08[0x0] = RG1W;
  mov16(location(RG61p), RG1W);
  RG1L = argl1 + 1;
  RG1W &= 0x3;
  RG2L = unsignedlong(RG1W);
  RG4L = RG51l;
  RG1L = DM2_move_2c1d_028c(RG7l, RG4L, RG2L);
  vwa_08[0x1] = RG1W;
  mov16(location(RG3P), RG1W);
  RG1W = vwa_08[0x0];
  if (RG1W == vwa_08[0x1])
    vwa_08[0x1] = lcon(0xffffffff);
  // RG7L = 0; unused
  RG62w = wcon(0x0);
  //m_13645:
  for (;;)
  {
    RG1L = signedlong(RG62w);
    if (RG1L >= 2)
      return;
    //m_135D7:
    RG1W = vwa_08[RG1L];
    if (RG1W >= 0)
    {
      RG52l = signedlong(RG1W);
      RG3L = 2;
      RG2L = lcon(0x18);
      RG4L = 1;
      RG1W = DM2_WOUND_PLAYER(RG52l, lcon(0x1), lcon(0x18), 2);
      if (RG1W != 0)
      {
        // RG7L = 1; unused
        RG1L = signedlong(vql_00.peek16());
        put16(parw01, RG1W);
        RG1L = signedlong(vql_04.peek16());
        put16(parw00, RG1W);
        RG4L = unsignedlong(party.hero[RG52l].herotype);
        RG3L = lcon(0xfe);
        RG2L = lcon(0x8a);
        DM2_QUEUE_NOISE_GEN2(lcon(0x16), RG4Blo, RG2Blo, RG3Blo, parw00, parw01, 1, 100, lcon(0xc8));
      }
    }
    RG62w++;
  }
  FOREND
}

// recursive!
// was SKW_12b4_0300
i32 DM2_PERFORM_MOVE(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5; // TODO: maybe more
  c_lreg _RG61;
  c_lreg _RG62;
  c_wlreg _RG63;
  c_lreg _RG64;
  c_lreg _RG7;
  c_ql32 vql_00;
  c_5bytes c12_04;
  c_ql32 vql_0c;
  i16 vw_10;
  i16 vw_14;
  i16 vw_18;
  i16 vw_1c;
  i16 vw_20;
  i16 vw_24;
  i16 vw_28;
  i16 vw_2c;
  i16 vw_30;
  i16 vw_34;
  i16 parw02;
  i16 parw04;
  i16 parw10;
  i32 parl11;
  i32 parl12;

  c_hero* hero1 = NULL;

  vql_00.poke32(RG1L);
  RG4L = signedlong(ddat.v1e027e);
  RG1L = dm2_GET_TILE_VALUE(signedlong(ddat.v1e0280), RG4L);
  RG4W = unsignedword(RG4Blo);
  RG4Blo = RG1Blo;
  vql_0c.poke32(RG4L);
  RG1L = unsignedlong(RG1Blo) >> 5;
  RG1L = unsignedlong(RG1Blo);
  RG1Blo = RG1L == lcon(0x3) ? 1 : 0;
  RG1W = unsignedword(RG1Blo);
  vw_18 = RG1W; // truncated
  RG5L = 1;
  c_hero* hero4 = &party.hero[0];
  RG2L = 0;
  //m_136E4:
  for (;;)
  {
    bool skip00221 = false;
    if (RG2UW < mkuw(party.heros_in_party))
    {
      //m_136BF:
      if (hero4->curHP != 0)
      {
        RG1L = DM2_CALC_PLAYER_WALK_DELAY(unsignedlong(RG2W));
        RG4L = signedlong(RG1W);
        RG1L = signedlong(DM2_MAX(RG5W, RG4W));
        RG5L = RG1L;
      }
      //m_136DD:
      hero4++; // struct advance
      RG2L++;
      continue;
    }
    RG1L = signedlong(RG5W);
    if (RG1L <= lcon(0x1) || ddat.v1e025c != 0)
      break;
    RG3L = vql_00.peek32();
    if (RG3W == lcon(0x3))
      skip00221 = true;
    else
    {
      if (RG3W == lcon(0x5))
      {
        if (ddat.v1e0234 != 0 || vw_18 != 0)
          //m_1372E:
          RG1L = 0;
        else
          RG1L = 1;
        //m_13730:
        if (RG1L != 0)
          skip00221 = true;
      }
    }

    if (!skip00221)
    {
      //m_13734:
      if (ddat.v1d4000 == lcon(0xffffffff))
        break;
    }

    //m_1373E:
    RG1L = signedlong(RG5W) / 2;
    ddat.v1e025c = RG1W;
    ddat.v1e026a = ddat.v1e0270;
    ddat.v1e0268 = ddat.v1e0272;
    ddat.v1e0278 = ddat.v1e0258;
    RG1L = vql_00.peek32();
    ddat.v1e0256 = RG1W;
    if (RG1W == lcon(0x3))
      jump L_fin1;
    RG1L = signedlong(ddat.v1e0258);
    DM2_CALC_VECTOR_W_DIR(RG1W, -1, 0, &ddat.v1e026a, &ddat.v1e0268);
    jump L_fin1;
  }
  //m_137A3:
  if (ddat.v1d4000 != lcon(0xffffffff))
  {
    RG4L = signedlong(ddat.v1e0258);
    RG1L = unsignedlong(ddat.v1e0278);
    if (RG4L == RG1L)
    {
      RG1W = ddat.v1e0266;
      if (RG1W == ddat.v1e0540)
      {
        RG1W = ddat.v1e0270;
        if (RG1W == ddat.v1e0546)
        {
          RG1W = ddat.v1e0272;
          if (RG1W == ddat.v1e0544)
          {
            RG4L = unsignedlong(ddat.v1e053a);
            RG1L = DM2_GET_CREATURE_AT(unsignedlong(ddat.v1e053c), RG4L);
            if (RG1W == ddat.v1d4000)
            {
              ddat.v1d4000 = lcon(0xffffffff);
              vw_30 = ddat.v1e0266;
              if (vql_00.peek16() == lcon(0x3))
                DM2_move_12b4_0d75(signedlong(ddat.v1e053c), signedlong(ddat.v1e053a), signedlong(ddat.v1e0542), lcon(0xfe));
              RG1L = DM2_PERFORM_MOVE(vql_00.peek16());
              if (RG1L != 0)
              {
                DM2_PERFORM_TURN_SQUAD(unsignedlong(ddat.v1e0538));
                if (vql_00.peek16() != lcon(0x3))
                {
                  DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_30));
                  DM2_move_12b4_0d75(signedlong(ddat.v1e053c), signedlong(ddat.v1e053a), signedlong(ddat.v1e0542), lcon(0xfe));
                }
              }
              //m_138B8:
              DM2_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
            }
          }
        }
      }
    }
    //m_138C5:
    ddat.v1d4000 = lcon(0xffffffff);
    if (ddat.v1e03a8 != 0)
      return lcon(0x1);
    ddat.v1d324a = 6;
    return lcon(0x1);
  }
  //m_138EA:
  vw_30 = ddat.v1d3248;
  RG1L = vql_00.peek32() - lcon(0x3);
  vw_28 = RG1W;
  if (ddat.v1e0234 != 0)
  {
    RG1L = signedlong(vw_28);
    if (RG1L == 2)
      DM2_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e027c));
  }
  vw_14 = ddat.v1e0280;
  vw_20 = ddat.v1e027e;
  vw_34 = ddat.v1e0264;
  RG1L = signedlong(vw_28);
  DM2_CALC_VECTOR_W_DIR(vw_34, signedword(table1d3ff8[RG1L]), signedword(table1d3ffc[RG1L]), &vw_14, &vw_20);
  RG4L = signedlong(vw_20);
  RG1L = dm2_GET_TILE_VALUE(signedlong(vw_14), RG4L);
  RG1W = unsignedword(RG1Blo);
  vw_24 = RG1W;
  ddat.glbRefreshViewport = true;
  hero1 = &party.hero[0];
  RG2L = 0;
  //m_139E3:
  for (;;)
  {
    bool skip00226 = false;
    bool skip00223 = false;
    bool skip00222 = false;
    if (RG2UW < mkuw(party.heros_in_party))
    {
      //m_1399E:
      if (hero1->curHP != 0)
      {
        RG1W = party.get_player_weight((e_hero)RG2W);
        RG4L = unsignedlong(RG1W);
        RG1L = 3 * RG4L;
        RG4L = RG1L;
        RG1W = hero1->get_max_load();
        RG61L = unsignedlong(RG1W);
        RG4L = signedlong(CUTX16(RG4L / RG61L + 1));
        RG1L = DM2_ADJUST_STAMINA(RG2W, RG4W);
      }
      //m_139DC:
      hero1++; // struct advance
      RG2L++;
      continue;
    }
    DM2_RESET_SQUAD_DIR();
    RG1L = signedlong(vw_20);
    put16(parw02, RG1W);
    RG3L = signedlong(vw_14);
    RG7L = unsignedlong(vw_24);
    RG4L = unsignedlong(vql_0c.peek16());
    RG62L = signedlong(vw_28);
    RG2L = RG7L;
    RG1L = DM2_12b4_0881(RG62L, RG4L, RG2L, RG3L, parw02, &vw_2c) - 1;
    if (RG1UW > mkuw(lcon(0x5)))
      jump L_fin2;
    RG1L = unsignedlong(RG1W);
    switch (RG1L)
    {
      case 0:
        skip00222 = true;
        break;

      case 1:
        //m_13A39:
        DM2_MOVE_RECORD_TO(lcon(0xffff), signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), lcon(0xffffffff), 0);
        ddat.v1e0270 = vw_14;
        ddat.v1e0272 = vw_20;
        vql_0c.poke32(signedlong(vw_24));
        skip00222 = true;
        break;

      case 2:
        //m_13CA1:
        if (party.heros_in_party == 0)
          jump L_fin2;
        parl12 = RG62L;
        parl11 = signedlong(vw_34);
        DM2_move_12b4_023f(signedlong(vw_14), signedlong(vw_20), &vw_1c, &vw_10, parl11, parl12);
        RG1L = RG7L >> 5 & lcon(0xff);
        if (RG1L != lcon(0x4))
          jump L_fin2;
        RG1Blo = CUTX8(vw_24) & lcon(0x7);
        RG1L = unsignedlong(RG1Blo);
        if (RG1L != lcon(0x4))
          jump L_fin2;
        RG5L = 0;
        RG4L = signedlong(vw_1c);
        if (RG4L != lcon(0xffffffff))
        {
          c_hero* hero2 = &party.hero[RG4L];
          RG5L = DM2_RAND() & lcon(0xf);
          RG2L = 0;
          RG4L = 1;
          RG1W = hero2->get_adj_ability1((e_ability)RG4W, (e_curmax)RG2W) + RG5W;
          RG4L = signedlong(RG1W);
          RG1W = hero2->get_stamina_adj(RG4W);
          RG5L = RG1L;
        }
        //m_13D41:
        RG1W = vw_10; // adj
        if (RG1W != vw_1c)
        {
          RG4L = signedlong(vw_10);
          if (RG4L != lcon(0xffffffff))
          {
            c_hero* hero3 = &party.hero[RG4L];
            RG63W = DM2_RAND() & 0xf;
            RG2L = 0;
            RG4L = 1;
            RG1W = hero3->get_adj_ability1(E_STRENGTH, E_CUR) + RG63W;
            RG4L = signedlong(RG1W);
            RG1W = hero3->get_stamina_adj(RG4W);
            RG5L += RG1L;
          }
        }
        //m_13D91:
        RG2L = signedlong(RG5W);
        DM2_ATTACK_DOOR(unsignedlong(vw_14), unsignedlong(vw_20), RG2L, 0, 0);
        jump L_fin2;

      case 3:
        //m_13A8B:
        if (party.heros_in_party == 0)
          jump L_fin2;
        RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(vw_2c));
        RG1W &= 0x1;
        RG7L = unsignedlong(RG1W);
        if (RG7L != 0)
          skip00223 = true;
        else
        {
          RG1L = DM2_GET_CREATURE_WEIGHT(unsignedlong(vw_2c)) & lcon(0xffff);
          if (RG1L == lcon(0xff))
            skip00223 = true;
        }
        break;

      case 4:
        jump L_fin2;

      case 5:
        skip00226 = true;
        break;

      default: throw(THROW_DMABORT);
    }

    if (skip00226)
      break;

    if (skip00223)
    {
      //m_13AC8:
      i32 parl06 = signedlong(vw_28);
      i32 parl05 = signedlong(vw_34);
      DM2_move_12b4_023f(signedlong(vw_14), signedlong(vw_20), &vw_1c, &vw_10, parl05, parl06);
    }

    if (skip00222)
    {
      //m_13A78:
      RG1L = vql_0c.peek32();
      RG1W &= 0x4;
      RG1L = signedlong(RG1W);
      DM2_move_12b4_00af(RG1L);
      jump L_fin1;
    }

    //m_13AF3:
    RG1L = unsignedlong(vw_34) + vql_00.peek32() - lcon(0x3);
    RG1W &= 0x3;
    RG64L = signedlong(RG1W);
    RG1L = DM2_IS_CREATURE_MOVABLE_THERE(signedlong(vw_14), signedlong(vw_20), RG64L, &vw_2c);
    if (RG1L != 0)
    {
      RG1L = DM2_move_12b4_099e(unsignedlong(vw_2c));
      if (RG1L != 0)
      {
        DM2_move_12b4_0d75(signedlong(vw_14), signedlong(vw_20), RG64L, lcon(0xfe));
        break;
      }
    }
    //m_13B4D:
    if (RG7L != 0)
      jump L_fin2;
    i32 parl08 = RG7L;
    DM2_ATTACK_CREATURE(lcon(0xffff), signedlong(vw_14), signedlong(vw_20), lcon(0x4005), lcon(0x5), parl08);
    jump L_fin2;
  }
  //m_13B7A:
  if (ddat.v1e0234 != 0)
  {
    RG1L = signedlong(vw_28);
    if (RG1L == 2)
    {
      DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_30));
      vw_14 = ddat.v1e0280;
      vw_20 = ddat.v1e027e;
      vw_34 = ddat.v1e0264;
      DM2_CALC_VECTOR_W_DIR(vw_34, -1, 0, &vw_14, &vw_20);
    }
  }
  //m_13BD8:
  if (vw_18 == 0)
  {
    //m_13BF8:
    RG2L = signedlong(vw_20);
    RG4L = signedlong(vw_14);
    RG1L = DM2_GET_TELEPORTER_DETAIL(&c12_04, RG4L, RG2L);
    if (RG1L != 0)
    {
      RG3L = unsignedlong(c12_04.b_01);
      RG4L = RG3L + 2 & lcon(0x3);
      RG1L = signedlong(vw_34);
      if (RG4L != RG1L)
      {
        RG2L = unsignedlong(c12_04.b_04);
        RG4L = unsignedlong(c12_04.b_03);
        RG1L = unsignedlong(c12_04.b_02);
        DM2_map_3BF83(RG1L, RG4L, RG2L, RG3L);
        jump L_fin1;
      }
    }
    //m_13C43:
    RG1L = signedlong(vw_20);
    put16(parw10, RG1W);
    RG3L = signedlong(vw_14);
    RG2L = signedlong(ddat.v1e0272);
    RG4L = signedlong(ddat.v1e0270);
  }
  else
  {
    RG1L = signedlong(vw_20);
    put16(parw10, RG1W);
    RG3L = signedlong(vw_14);
    RG2L = 0;
    RG4L = lcon(0xffffffff);
  }
  //m_13C64:
  RG1L = lcon(0xffff);
  DM2_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw10);

L_fin1:
  RG1Blo = ddat.v1e025c != 0 ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  RG4L = signedlong(RG1W);
  RG1L = signedlong(DM2_MAX(RG5W / 2, RG4W));
  ddat.v1e026e += RG1W;
  RG3L = 0;
  ddat.v1e025e = RG3W;
  return lcon(0x1);

L_fin2:
  eventqueue.event_1031_098e();
  ddat.glbRefreshViewport = false;
  return 0;
}

void DM2_move_12b4_0d75(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG51w;
  i16 RG53w;
  i16 RG61w;
  i16 RG62w;
  c_ql32 vql_00;
  c_ql32 vql_04;
  c_ql32 vql_08;
  c_ql32 vql_0c;
  i16 vw_10;
  i8 vb_14;
  i16 vw_18;
  i16 vw_1c;
  i16 vw_20;
  c_o32 vo_24; // poke 16 peek 16 32 inc16
  i16 parw02;
  i16 parw03;
  i16 parw05;
  i16 parw06;
  i16 parw07;

  vql_0c.poke32(RG1L);
  vql_08.poke32(RG4L);
  vql_04.poke32(RG2L);
  vql_00.poke32(RG3L);
  RG2L = signedlong(vql_04.peek16());
  RG4L = signedlong(vql_08.peek16());
  RG1L = signedlong(vql_0c.peek16());
  RG3P = ADRESSOF(i16, &vw_1c);
  RG1L = DM2_IS_CREATURE_MOVABLE_THERE(RG1L, RG4L, RG2L, RG3P16);
  if (RG1L != 0)
  {
    bool skip00238 = false;
    RG1L = DM2_GET_CREATURE_WEIGHT(unsignedlong(vw_1c));
    RG4L = vql_00.peek32();
    if (RG4W >= wcon(0xfe) || RG1W <= RG4W)
      skip00238 = true;
    else
    {
      RG4L = (signedlong(RG1W) - signedlong(vql_00.peek16())) / 4;
      RG1W = DM2_RAND16(RG4W + 1);
      if (RG1W == 0)
        skip00238 = true;
    }

    if (skip00238)
    {
      //m_14448:
      RG1L = signedlong(vql_04.peek16());
      RG61w = vql_0c.peek16(); // was long-op
      RG61w += table1d27fc[RG1L];
      RG51w = vql_08.peek16() + table1d2804[RG1L];
      RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(vw_1c));
      vb_14 = RG1Blo;
      RG4W = ddat.v1d3248;
      if (RG4W != ddat.v1e0282 || RG61w != ddat.v1e0280 || RG51w != ddat.v1e027e || (RG1Bhi & bcon(0x80)) != 0)
      {
        //m_144E2:
        DM2_1c9a_0247(unsignedlong(vw_1c));
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(vw_1c)));
        RG1Blo = byte_at(RG1P, lcon(0x5));
        if (RG1Blo != lcon(0xffffffff))
        {
          RG4L = unsignedlong(RG1Blo);
          RG1L = 16 * RG4L;
          RG4L += RG1L;
          RG4L = 2 * RG4L;
          RG1P = DOWNCAST(c_creature, creatures) + RG4L;
          RG4W = signedword(byte_at(RG1P, lcon(0x1a)));
          vo_24.poke16(RG4W);
          RG1L = signedlong(byte_at(RG1P, lcon(0x1a)));
          if (RG1L != lcon(0xffffffff))
          {
            RG1L = signedlong(vo_24.peek16());
            if (RG1L != lcon(0x13))
            {
              RG1Blo = table1d613a[RG1L] & lcon(0x4);
              RG1L = unsignedlong(RG1Blo);
              if (RG1L != 0)
              {
                DM2_1c9a_0db0(unsignedlong(vw_1c));
                DM2_1c9a_0cf7(unsignedlong(vql_0c.peek16()), unsignedlong(vql_08.peek16()));
              }
            }
          }
        }
        RG1L = signedlong(RG51w);
        put16(parw02, RG1W);
        RG3L = signedlong(RG61w);
        RG2L = signedlong(vql_08.peek16());
        DM2_MOVE_RECORD_TO(unsignedlong(vw_1c), signedlong(vql_0c.peek16()), RG2L, RG3L, parw02);
        RG4L = unsignedlong(RG51w);
        RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG61w), RG4L);
        RG1Blo = RG1W != wcon(0xffff) ? 1 : 0;
        RG1L = unsignedlong(RG1Blo);
        if (RG1L != 0)
        {
          if ((vb_14 & bcon(0x1)) == 0)
          {
            RG2L = signedlong(RG51w);
            RG4L = signedlong(RG61w);
            RG3L = lcon(0x4002);
            DM2_ATTACK_CREATURE(lcon(0xffff), RG4L, RG2L, RG3L, lcon(0xa), 0);
          }
        }
      }
      else
      {
        RG1L = vql_04.peek32() + 2;
        RG1W &= 0x3;
        vo_24.poke16(RG1W);
        i32 parl01 = signedlong(vo_24.peek16());
        i32 parl00 = signedlong(ddat.v1e0258);
        RG4L = signedlong(vql_08.peek16());
        RG1L = signedlong(vql_0c.peek16());
        RG3P = ADRESSOF(i16, &vw_10);
        RG2P = ADRESSOF(i16, &vw_18);
        DM2_move_12b4_023f(RG1L, RG4L, RG2P16, RG3P16, parl00, parl01);
      }
    }
  }
  //m_145C2:
  RG1W = ddat.v1d3248;
  if (RG1W != ddat.v1e0282)
    return;
  RG1L = vql_0c.peek32();
  if (RG1W != ddat.v1e0280)
    return;
  RG1L = vql_08.peek32();
  if (RG1W != ddat.v1e027e)
    return;
  RG62w = 0;
  vw_20 = RG62w;
  c_hero* hero = &party.hero[0];
  vo_24.poke16(RG62w);
  //m_1467B:
  for (;;)
  {
    RG1L = signedlong(RG62w);
    if (RG1L >= lcon(0x4))
    {
      if (vo_24.peek16() == 0)
        return;
      RG1W = vw_20;
      RG2W = vo_24.peek16();
      RG4W = RG1W % RG2W;
      RG1W /= RG2W;
      vw_20 = RG1W;
      RG3L = RG1L + lcon(0x28);
      vw_20 = RG3W;
      RG53w = vql_00.peek16();
      if (RG53w < wcon(0xfe) && RG53w <= RG3W)
        return;
      RG1L = vql_04.peek32();
      RG1W -= ddat.v1e0258;
      RG1W &= 0x3;
      RG1L += 3;
      DM2_PERFORM_MOVE(RG1W);
      return;
    }
    //m_1460A:
    if (hero->curHP != 0)
    {
      RG1L = signedlong(vql_08.peek16());
      put16(parw06, RG1W);
      RG1L = signedlong(vql_0c.peek16());
      put16(parw05, RG1W);
      RG4L = unsignedlong(hero->herotype);
      RG3L = lcon(0xfe);
      DM2_QUEUE_NOISE_GEN2(lcon(0x16), RG4Blo, lcon(0x8a), RG3Blo, parw05, parw06, lcon(0x1), lcon(0x64), lcon(0xc8));
      RG1W = party.get_player_weight((e_hero)RG62w);
      RG4L = unsignedlong(RG1W);
      RG2L = lcon(0xa);
      RG1L = RG4L;
      RG4L = RG1L % RG2L;
      RG1L /= RG2L;
      RG4L = signedlong(vw_20) + RG1L;
      vw_20 = RG4W;
      vo_24.inc16();
    }
    hero++; // struct advance
    RG62w++;
  }
  FOREND
}

i32 DM2_move_12b4_099e(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG51l;
  i32 RG53l;
  i16 RG6w;
  i32 RG7l; // TODO is bool

  RG1L = DM2_GET_CREATURE_WEIGHT(unsignedlong(RG1W));
  RG6w = RG1W;
  if (RG1W > wcon(0xfd))
    return 0;
  RG7l = 0;
  RG3L = 0;
  //m_13FAE:
  for (;;)
  {
    bool skip00227 = false;
    RG51l = signedlong(RG3W);
    RG1L = unsignedlong(party.heros_in_party);
    if (RG51l >= RG1L)
      break;
    //m_13F34:
    c_hero* hero = &party.hero[RG51l];
    if (hero->curHP != 0)
    {
      RG2L = 0;
      RG4L = 1;
      RG1W = hero->get_adj_ability1(E_STRENGTH, E_CUR);
      RG4W = RG1W;
      if ((e_hero)RG3W == eventqueue.event_heroidx)
      {
        RG1L = signedlong(RG1W);
        RG4L = RG1L / 8;
        RG1L += RG4L;
      }
      if (RG6w <= wcon(0x2d))
      {
        RG1L = signedlong(RG1W);
        RG4L = RG1L / 4;
        RG1L += RG4L;
      }
      RG4L = signedlong(RG1W);
      RG1W = hero->get_stamina_adj(RG4W);
      if (RG1W >= RG6w)
        skip00227 = true;
      else
      {
        RG4L = signedlong(RG6w);
        RG1L = signedlong(RG1W);
        RG4L -= RG1L;
        RG1W = DM2_RAND16(RG4W / 2 + 1);
        if (RG1W == 0)
          skip00227 = true;
      }
    }

    if (skip00227)
    {
      //m_13FA6:
      RG7l = 1;
      break;
    }

    //m_13FAD:
    RG3L++;
  }
  //m_13FC1:
  RG3L = 0;
  //m_1401C:
  for (;;)
  {
    RG53l = signedlong(RG3W);
    RG1L = unsignedlong(party.heros_in_party);
    if (RG53l >= RG1L)
      return RG7l;
    //m_13FC5:
    RG1L = 263 * RG53l;
    if (party.hero[RG53l].curHP != 0)
    {
      RG4L = party.hero[RG53l].maxStamina >> 4;
      RG1L = signedlong(party.hero[RG53l].curStamina);
      if (RG1L > RG4L)
      {
        if (RG7l == 0)
          //m_1400B:
          RG1L = 5;
        else
        {
          RG4L = signedlong(RG6w / 4);
          RG1L = signedlong(DM2_MAX(5, RG4W));
        }
        //m_14010:
        RG4L = signedlong(RG1W);
        RG1L = signedlong(RG3W);
        DM2_ADJUST_STAMINA(RG1W, RG4W);
      }
    }
    //m_1401B:
    RG3L++;
  }
  FOREND
}

void DM2_move_12b4_0092(void)
{
  if (ddat.v1e0534 != 0)
    DM2_HIGHLIGHT_ARROW_PANEL(unsignedlong(ddat.v1e0548), signedlong(ddat.v1e053e), 0);
}

void DM2_move_12b4_00af(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;

  RG5w = RG1W;
  DM2_MOVE_RECORD_TO(lcon(0xffff), signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), lcon(0xffffffff), 0);
  RG3P = DOWNCAST(i16, &ddat.v1e0272);
  RG2P = DOWNCAST(i16, &ddat.v1e0270);
  if (RG5w == 0)
    //m_133CF:
    RG1L = 1;
  else
    RG1L = lcon(0xffffffff);
  //m_133D4:
  RG4L = signedlong(RG1W);
  RG1L = DM_LOCATE_OTHER_LEVEL(signedlong(ddat.v1e0266), RG4L, RG2P16, RG3P16, NULL);
  ddat.v1d27f8 = RG1W;
  RG1L = signedlong(RG1W);
  DM2_CHANGE_CURRENT_MAP_TO(RG1L);
  RG4L = signedlong(ddat.v1e0272);
  RG1L = signedlong(CUTX16(DM2_query_0cee_06dc(signedlong(ddat.v1e0270), RG4L)));
  party.rotate(RG1W);
  DM2_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
}

void DM2_move_2fcf_0b8b(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  i32 RG6l;
  c_5bytes c12_00;
  i16 vw_08;

  RG6l = RG1L;
  RG5L = RG4L;
  vw_08 = RG2W;
  ddat.v1e027c = lcon(0xffffffff);
  RG2L = signedlong(RG5W);
  RG4L = signedlong(RG1W);
  RG1L = DM2_GET_TELEPORTER_DETAIL(&c12_00, RG4L, RG2L);
  ddat.v1e0234 = RG1L;
  if (RG1L == 0)
  {
    //m_3AF48:
    RG3L = 0;
    //m_3AFD7:
    for (;;)
    {
      RG1L = unsignedlong(RG3W);
      if (RG1L < lcon(0x4))
      {
        //m_3AF4F:
        RG4W = table1d2804[RG1L];
        RG4L += RG5L;
        RG2L = signedlong(RG4W);
        RG1W = table1d27fc[RG1L];
        RG1L += RG6l;
        RG4L = signedlong(RG1W);
        RG1L = DM2_GET_TELEPORTER_DETAIL(&c12_00, RG4L, RG2L);
        if (RG1L == 0)
        {
          //m_3AFD6:
          RG3L++;
          continue;
        }
        RG1L = unsignedlong(c12_00.b_04);
        ddat.v1e027c = RG1W;
        RG1L = unsignedlong(c12_00.b_01) + lcon(0x6) - unsignedlong(c12_00.b_00);
        RG4L = RG1L;
        RG3L += RG1L;
        RG3L += 2;
        RG3W &= 0x3;
        RG1L = unsignedlong(RG3W);
        RG2L = unsignedlong(c12_00.b_02);
        RG3W = table1d27fc[RG1L];
        RG2L += RG3L;
        ddat.v1e0260 = RG2W;
        RG2L = unsignedlong(c12_00.b_03);
        RG1W = table1d2804[RG1L];
        RG2L += RG1L;
        ddat.v1e0262 = RG2W;
        RG4W += ddat.v1e0258;
        RG4W &= 0x3;
        party.absdir = RG4W;
      }
      break;
    }
  }
  else
  {
    RG1L = unsignedlong(c12_00.b_04);
    ddat.v1e027c = RG1W;
    RG1L = unsignedlong(c12_00.b_02);
    ddat.v1e0260 = RG1W;
    RG1L = unsignedlong(c12_00.b_03);
    ddat.v1e0262 = RG1W;
    RG4L = unsignedlong(c12_00.b_01) - unsignedlong(c12_00.b_00);
    RG4W += ddat.v1e0258;
    RG4W &= 0x3;
    party.absdir = RG4W;
  }
  //m_3AFE5:
  ddat.v1d3248 = lcon(0xffffffff);
  DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_08));
}

// belongs to DM2_ATTACK_WALL
static i32 DM2_0cee_317f(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  RG2L = unsignedlong(RG4W);
  RG1L = DM2_IS_WALL_ORNATE_ALCOVE_FROM_RECORD(unsignedlong(RG1W), RG2L);
  if (RG1L == 0)
    return RG1L;
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG2L);
  if (RG1Blo == lcon(0xffffffff))
    return 0;
  RG4L = unsignedlong(RG1Blo);
  return mkul(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, RG4Blo, 11, 19));
}

// belongs to DM2_move_075f_0af9
// stacksize was 0x2c
// was SKW_075f_084d (new)
static i32 DM2_ATTACK_WALL(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i32 argl1)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG51w;
  i8* RG52p;
  c_wlreg _RG53;
  c_wlreg _RG6; // TODO: maybe more
  i8* RG7p;
  c_ql32 vql_00;
  c_ql32 vql_04;
  c_ql32 vql_08;
  c_ql32 vql_0c;
  i32 vl_10;
  i16 vw_14;
  c_ql32 vql_18;

  vql_0c.poke32(RG1L);
  vql_08.poke32(RG4L);
  vql_04.poke32(RG2L);
  vql_00.poke32(RG3L);
  RG6L = argl1;
  RG3L = 0;
  RG4L = argl0 + 2;
  argl0 = RG4L;
  RG2L = RG4L;
  RG2Bhi ^= RG4Bhi;
  RG2Blo &= 0x3;
  argl0 = RG2L;
  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(vql_0c.peek16(), vql_08.peek16()));
  vql_18.poke32(RG1L);
  vw_14 = RG1W; // truncated
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG6UW));
  RG7p = RG1P;
  //m_7085:
  for (;;)
  {
    RG51w = vql_18.peek16();
    if (RG51w == wcon(0xfffe))
      break;
    RG4L = unsignedlong(RG51w);
    RG1L = RG4L >> bcon(0xe);
    vl_10 = RG1L;
    RG1L = unsignedlong(mkuw(argl0)); // truncated TODO check if arg is always word
    if (RG1L == vl_10)
    {
      bool skip00027 = false;
      bool skip00030 = false;
      bool skip00032 = false;
      RG1W = word_at(RG7p, 2) & lcon(0x3c00);
      RG1L = unsignedlong(RG1W) >> bcon(0xa);
      if (RG1L == lcon(0xf) || RG3L != 0)
        skip00027 = true;
      else
      {
        RG1L = DM2_0cee_317f(unsignedlong(vw_14), RG4L);
        if (RG1L == 0)
          skip00027 = true;
        else
        {
          RG1L = signedlong(DM2_RANDDIR());
          if (RG1W != 0)
            skip00027 = true;
          else
          {
            RG3L = signedlong(vql_00.peek16());
            RG2L = signedlong(vql_04.peek16());
            RG53L = unsignedlong(RG6W);
            DM2_CUT_RECORD_FROM(mk_record(RG53L), NULL, RG2W, RG3W);
            RG1L = vl_10 << bcon(0xe);
            RG53W &= lcon(0x3fff);
            RG1L |= RG53L;
            RG6L = RG1L;
          }
        }
      }

      if (skip00027)
      {
        //m_7125:
        RG1L = vql_18.peek32();
        RG1W &= lcon(0x3c00);
        RG1L = unsignedlong(RG1W) >> bcon(0xa);
        if (RG1L == lcon(0x3))
        {
          RG4W = word_at(RG7p, 2);
          RG1L = RG4L;
          RG1Blo ^= RG4Blo;
          RG1Bhi &= lcon(0x3c);
          RG1L = unsignedlong(RG1W) >> bcon(0xa);
          if (RG1L != lcon(0xf))
          {
            RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(vql_18.peek16())));
            RG52p = RG1P;
            RG1W = word_at(RG1P, 2);
            RG2L = RG1L;
            RG2UW >>= bcon(0x7);
            RG1W &= lcon(0x7f);
            if (RG1UW >= mkuw(lcon(0x22)))
            {
              if (RG1UW <= mkuw(lcon(0x22)))
              {
                bool skip00029 = false;
                //m_7192:
                RG1L = signedlong(RG2W);
                if (RG1L != lcon(0x1ff))
                {
                  RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG4W);
                  if (RG2W != RG1W)
                  {
                    //m_71B4:
                    RG1L = 0;
                    skip00029 = true;
                  }
                }

                if (!skip00029)
                  //m_71AD:
                  RG1L = 1;

                //m_71B6:
                RG4W = word_at(RG52p, lcon(0x4));
                RG4L <<= bcon(0xa);
                RG4UW >>= bcon(0xf);
                RG4L = unsignedlong(RG4W);
                RG1L ^= RG4L;
                if (RG1L != 0)
                {
                  RG4W = word_at(RG52p, lcon(0x4)) & lcon(0x18);
                  if (RG4W != lcon(0x18))
                  {
                    //m_71EE:
                    RG1W = word_at(RG52p, lcon(0x4));
                    RG1L <<= 11;
                    RG1UW >>= 14;
                    RG4L = unsignedlong(RG1W);
                    RG2L = 0;
                  }
                  else
                  {
                    RG2L = 0;
                    RG1Blo = RG1L == 0 ? 1 : 0;
                    RG1W = unsignedword(RG1Blo);
                    RG4L = unsignedlong(RG1W);
                  }
                  //m_7200:
                  DM2_INVOKE_ACTUATOR(RG52p, RG4L, RG2L);
                  if (RG3L == 0 && jnz_test8(location(RG52p + lcon(0x4)), lcon(0x4)))
                  {
                    RG3L = signedlong(vql_00.peek16());
                    RG2L = signedlong(vql_04.peek16());
                    RG1L = unsignedlong(RG6W);
                    DM2_CUT_RECORD_FROM(mk_record(RG1L), NULL, RG2W, RG3W);
                    RG6L &= lcon(0xffff3fff);
                    RG1L = argl0 << bcon(0xe);
                    RG6L |= RG1L;
                    skip00030 = true;
                  }
                }
              }
              else
              {
                if (RG1W == lcon(0x23))
                {
                  //m_7247:
                  if (RG3L == 0)
                  {
                    bool skip00031 = false;
                    RG1L = signedlong(RG2W);
                    if (RG1L == lcon(0x1ff))
                      skip00031 = true;
                    else
                    {
                      RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG4W);
                      if (RG2W == RG1W)
                        skip00031 = true;
                    }

                    if (skip00031)
                    {
                      //m_726E:
                      RG3L = signedlong(vql_00.peek16());
                      RG2L = signedlong(vql_04.peek16());
                      RG6L = unsignedlong(RG6W);
                      RG4P = NULL;
                      RG1L = RG6L;
                      DM2_CUT_RECORD_FROM(mk_record(RG1L), NULL, RG2W, RG3W);
                      RG1W = word_at(RG52p, lcon(0x6));
                      RG4L = RG1L;
                      _RG4.ushiftr11();
                      RG4L = signedlong(RG4W);
                      RG1L <<= bcon(0x5);
                      _RG1.ushiftr11();
                      RG1L = (dm2_GET_TILE_VALUE(signedlong(RG1W), RG4L) & lcon(0xff)) >> 5;
                      RG6W &= lcon(0x3fff);
                      if (RG1Blo != 0)
                      {
                        //m_72BF:
                        RG1W = word_at(RG52p, lcon(0x4));
                        RG1L <<= 11;
                      }
                      else
                      {
                        RG1W = word_at(RG52p, lcon(0x6));
                        RG1L <<= bcon(0xa);
                      }
                      //m_72C6:
                      RG1UW >>= 14;
                      RG1L = unsignedlong(RG1W) << bcon(0xe);
                      RG6L |= RG1L;
                      RG1W = word_at(RG52p, lcon(0x6));
                      RG4L = RG1L;
                      _RG4.ushiftr11();
                      RG3L = signedlong(RG4W);
                      RG1L <<= bcon(0x5);
                      _RG1.ushiftr11();
                      RG2L = signedlong(RG1W);
                      skip00032 = true;
                    }
                  }
                }
              }
            }
          }
        }
      }
      else
        skip00030 = true;

      if (skip00030)
      {
        //m_7112:
        RG3L = signedlong(vql_08.peek16());
        RG2L = signedlong(vql_0c.peek16());
        skip00032 = true;
      }

      if (skip00032)
      {
        //m_72EB:
        DM2_DELETE_MISSILE_RECORD(unsignedlong(RG6W), NULL, RG2L, RG3L);
        RG3L = 1;
      }
    }

    //m_72FC:
    vql_18.poke32(record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_18.peek16())))));
  }
  return RG3L;
}

// stacksize was 0x74
i32 DM2_move_075f_0af9(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  i8* RG61p;
  c_lreg _RG62;
  c_ql32 vql_00;
  c_ol32 vol_04; // poke 32 peek 16 32 and16
  c_ol32 vol_08; // poke 32 peek 16 32 and16
  c_ql32 vql_0c;
  unk* xp_10;
  unk* xp_14;
  unk* xp_18;
  i32 vl_1c;
  i32 vl_20;
  c_o32 vo_24; // poke 16 peek 32 add16
  c_o32 vo_28; // poke 16 peek 32 add16
  i32 vl_2c;
  i16 vw_30;
  i16 vw_34;
  i16 vw_38;
  i32 vl_3c;
  i16 vw_40;
  i16 vw_44;
  i16 vw_48;
  c_ql32 vql_4c;
  i16 vw_50;
  i16 vw_54;
  i16 vw_58;
  i16 vw_5c;
  i16 vw_60;
  i16 parw05;

  vql_0c.poke32(RG1L);
  vol_08.poke32(RG4L);
  vol_04.poke32(RG2L);
  vql_00.poke32(RG3L);
  RG4L = 0;
  vw_58 = RG4W;
  vw_38 = RG4W;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(mkuw(argl0)))); // truncated TODO check if arg is always word
  RG61p = RG1P;
  RG5W = word_at(RG1P, 2);
  RG1L = RG5L; // TODO: high word of RG5 undefined
  RG1W &= lcon(0x3c00);
  RG1L = unsignedlong(RG1W) >> bcon(0xa);
  vw_34 = RG1W; // truncated
  RG1L = unsignedlong(vw_34);
  if (RG1L == lcon(0x8))
  {
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG5UW));
    xp_14 = UPCAST(unk, RG1P);
    RG1W = word_at(RG1P, 2); // TODO: gets extended w/o conversion now
    RG1L = 2 * RG1L;
    RG1UW >>= bcon(0x9);
    if (RG1UW >= mkuw(lcon(0x3)))
    {
      bool skip00034 = false;
      if (RG1UW <= mkuw(lcon(0x3)))
      {
        //m_7386:
        vl_2c = lcon(0xff87);
        skip00034 = true;
      }
      else
      {
        if (RG1W == lcon(0x13))
        {
          //m_738F:
          vl_2c = lcon(0xff80);
          skip00034 = true;
        }
      }

      if (skip00034)
      {
        //m_7396:
        vw_38 = 1;
        RG1P = DOWNCAST(unk, xp_14);
        RG1W = word_at(RG1P, 2);
        RG1W = unsignedword(RG1Blo);
        vl_3c = RG1L; // TODO surely not a long then, or pointer-msw survives
        vw_40 = RG5W; // truncated
      }
    }
  }
  bool skip00036 = false;
  //m_73AC:
  RG1L = unsignedlong(vw_34);
  if (RG1L != lcon(0xf))
    skip00036 = true;
  else
  {
    RG1L = unsignedlong(RG5W);
    if (RG1L == lcon(0xff81) || RG1L == lcon(0xff86))
      skip00036 = true;
    else
      RG1L = 1;
  }

  if (skip00036)
    //m_73D1:
    RG1L = 0;
  bool skip00042 = false;
  //m_73D3:
  vw_50 = RG1W; // truncated
  RG4P = NULL;
  xp_10 = UPCAST(unk, RG4P);
  vl_20 = 0;
  RG1L = signedlong(vol_08.peek16());
  if (RG1L > lcon(0xff))
  {
    //m_73FB:
    RG1L >>= bcon(0x8);
    RG1L--;
    vw_54 = RG1W;
    RG1L = signedlong(vol_04.getbyte1());
    vw_60 = RG1W;
    RG1W = unsignedword(RG1Blo);
    vol_08.and16(lcon(0xff)); // did set highbyte to RG1Bhi=0
    vol_04.and16(lcon(0xff)); // did set highbyte to RG1Bhi=0
  }
  else
  {
    RG1L = vol_08.peek32();
    vw_54 = RG1W;
    RG1L = vol_04.peek32();
    vw_60 = RG1W;
  }
  //m_7415:
  RG1W = vw_54;
  vo_28.poke16(RG1W);
  vw_30 = RG1W;
  RG1W = vw_60;
  vo_24.poke16(RG1W);
  vw_48 = RG1W;
  RG2L = vql_0c.peek32();
  if (RG2W < wcon(0xffff))
  {
    //m_744E:
    if (RG2W >= wcon(0xfffffffd))
    {
      //m_75F2:
      RG1W = DM2_GET_PLAYER_AT_POSITION(vql_00.peek16());
      RG2L = RG1L;
      if (RG1W >= 0)
      {
        //m_7614:
        vl_20 = 1;
        RG3L = unsignedlong(RG5W);
        RG4L = RG3L;
        RG1L = DM2_move_075f_06bd(RG61p, RG4L);
        vw_44 = RG1W;
        if (RG1W != 0)
        {
          if (RG3L == lcon(0xff86))
          {
            RG4W = ddat.v1e01a4; // TODO: gets extended w/o conversion now
            RG4L = 2 * RG4L;
            ddat.v1e01a4 = RG4W;
          }
          RG1W = timdat.timerarray[word_at(RG61p, lcon(0x6))].getB();
          RG1L <<= 4;
          RG1UW >>= 14;
          RG3L = unsignedlong(RG1W);
          RG4L = signedlong(RG2W);
          RG1Blo = party.hero[RG4L].absdir;
          RG1L = unsignedlong(RG1Blo);
          RG3L += 2;
          RG1Blo = RG1L == RG3L ? 1 : 0;
          RG1L = unsignedlong(RG1Blo);
          if (RG1L != 0)
            ddat.v1e01a6 |= lcon(0x8000);
          RG3L = unsignedlong(ddat.v1e01a6);
          RG4L = signedlong(vw_44);
          RG1L = signedlong(RG2W);
          vl_1c = RG1L;
          RG1W = DM2_WOUND_PLAYER(RG1L, RG4L, 12, RG3L);
          if (RG1W != 0)
          {
            if (ddat.v1e01a4 != 0)
            {
              RG1L = DM2_RAND();
              RG1W &= lcon(0x7);
              RG1L = unsignedlong(RG1W);
              if (RG1L != 0)
              {
                RG4L = unsignedlong(ddat.v1e01a4);
                DM2_PROCESS_POISON(vl_1c, RG4L);
              }
            }
          }
        }
        if (vql_0c.peek16() != lcon(0xfffffffe))
          skip00042 = true;
      }
      else
      {
        if (vql_0c.peek16() != lcon(0xfffffffd))
          return 0;
        skip00042 = true;
      }
    }
  }
  else
  {
    if (RG2W == wcon(0xffff))
      skip00042 = true;
    else
    {
      if (RG2W <= 0)
      {
        //m_745D:
        RG1W = timdat.timerarray[word_at(RG61p, lcon(0x6))].getB();
        RG1L <<= 4;
        RG1UW >>= 14;
        vql_4c.poke32(RG1L);
        RG4L = unsignedlong(RG1W);
        RG1L = RG4L;
        RG2W = vw_54;
        RG3W = table1d27fc[RG4L];
        RG2L += RG3L;
        vw_30 = RG2W;
        RG2W = vw_60;
        RG3W = table1d2804[RG4L];
        RG2L += RG3L;
        vw_48 = RG2W;
        RG1L ^= RG4L;
        RG1W = mkw(argl0);
        i32 parl01 = RG1L;
        i32 parl00 = RG4L;
        RG3L = signedlong(vw_60);
        RG2L = signedlong(vw_54);
        RG4L = signedlong(vw_48);
        RG1L = DM2_ATTACK_WALL(signedlong(vw_30), RG4L, RG2L, RG3L, parl00, parl01);
        if (RG1L != 0)
          return lcon(0x1);
      }
      else
      {
        if (RG2W == lcon(0x4))
        {
          bool skip00038 = false;
          //m_74DE:
          RG1L = signedlong(vol_08.peek16());
          RG4L = signedlong(vol_04.peek16());
          RG3Blo = mapdat.map[RG1L][RG4L] & lcon(0x7);
          RG2L = unsignedlong(RG3Blo);
          RG1RP = DM2_GET_ADDRESS_OF_TILE_RECORD(RG1W, RG4W);
          RG3L = unsignedlong(RG3Blo);
          if (RG3L == lcon(0x5))
            skip00038 = true;
          else
          {
            RG4L = unsignedlong(RG5W);
            if (RG4L != lcon(0xff84) && RG4L != lcon(0xff8d))
              skip00038 = true;
          }

          if (skip00038)
          {
            //m_7530:
            RG4L = signedlong(RG2W);
            if (RG4L == lcon(0x5) || RG4L <= lcon(0x1))
              return 0;
            RG1L = DM2_query_0cee_3275(DM2_IS_REBIRTH_ALTAR(RG1P) & lcon(0xff));
            if (RG1L != 0)
            {
              RG1L = unsignedlong(vw_34);
              if (RG1L != lcon(0xf))
              {
                bool skip00039 = false;
                //m_7577:
                RG1L = DM2_RAND();
                RG1W &= lcon(0x7f);
                RG4L = unsignedlong(RG1W);
                RG1L = unsignedlong(byte_at(RG61p, lcon(0x5)));
                if (RG1L <= RG4L)
                  skip00039 = true;
                else
                {
                  RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG5W), 0);
                  RG1W &= 0x2;
                  RG1L = unsignedlong(RG1W);
                  if (RG1L == 0)
                    skip00039 = true;
                  else
                    RG1L = 1;
                }

                if (skip00039)
                  //m_75AC:
                  RG1L = 0;
              }
              else
              {
                RG1L = unsignedlong(RG5W);
                RG1Blo = RG1L >= lcon(0xff83) ? 1 : 0;
                RG1L = unsignedlong(RG1Blo);
              }
              //m_75AE:
              if (RG1L != 0)
                return 0;
            }
            //m_75B6:
            RG4L = unsignedlong(RG5W);
            RG1L = DM2_move_075f_06bd(RG61p, RG4L) + 1;
            vw_44 = RG1W;
            RG1W = DM2_RAND16(vw_44) + vw_44;
            RG2L = signedlong(RG1W);
            RG4L = unsignedlong(vol_04.peek16());
            DM2_ATTACK_DOOR(unsignedlong(vol_08.peek16()), RG4L, RG2L, 0, 0);
          }
        }
      }
    }
  }

  if (skip00042)
  {
    bool skip00044 = false;
    //m_7700:
    RG1L = vol_08.peek32();
    vw_30 = RG1W;
    RG1L = vol_04.peek32();
    vw_48 = RG1W;
    RG2L = unsignedlong(vql_00.peek16());
    RG4P = ADRESSOF(i16, &vw_48);
    RG1L = DM2_query_1c9a_03cf(&vw_30, RG4P16, RG2L);
    vw_5c = RG1W; // truncated
    if (RG1W != lcon(0xffffffff))
    {
      //m_773A:
      RG4L = unsignedlong(RG1W);
      RG1L = DM2_1c9a_0958(RG4L);
      if (RG1L != 0)
        skip00044 = true;
      else
      {
        bool skip00043 = false;
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
        xp_14 = UPCAST(unk, RG1P);
        RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG1P, lcon(0x4)))));
        xp_18 = UPCAST(unk, RG1P);
        if (jz_test8(location(RG1P), lcon(0x20)))
          skip00043 = true;
        else
        {
          RG1L = unsignedlong(RG5W);
          if (RG1L == lcon(0xff83))
            skip00043 = true;
        }

        if (skip00043)
        {
          bool skip00046 = false;
          bool skip00045 = false;
          //m_7775:
          RG1P = DOWNCAST(unk, xp_18);
          if (jnz_test8(location(RG1P), lcon(0x1)))
          {
            //m_7791:
            if (jnz_test8(location(RG1P + lcon(0x1f)), lcon(0x1)) && RG5UW < mkuw(lcon(0xffffff80)))
              skip00044 = true;
            else
            {
              RG1L = 0;
              RG4P = DOWNCAST(unk, xp_18);
              RG1W = word_at(RG4P);
              RG1L >>= bcon(0x6);
              RG1L &= 0x3;
              RG1L = unsignedlong(RG1W);
              if (RG1L > lcon(0x1))
                skip00044 = true;
              else
              {
                if (jnz_test8(location(RG4P + lcon(0x1f)), lcon(0x8)))
                  skip00046 = true;
                else
                  skip00045 = true;
              }
            }
          }
          else
          {
            RG1L = 0;
            RG1Blo = byte_at(DOWNCAST(unk, xp_18), lcon(0x1d));
            if (RG1L == lcon(0xff))
              skip00045 = true;
            else
              skip00046 = true;
          }

          if (skip00045)
          {
            //m_77C3:
            RG1W = timdat.timerarray[word_at(RG61p, lcon(0x6))].getB();
            RG1L <<= 4;
            RG1UW >>= 14;
            vql_4c.poke32(RG1L);
            RG1L += 2;
            RG1W &= 0x3;
            vql_4c.poke32(RG1L);
            RG1L = unsignedlong(vql_4c.peek16());
            RG4W = table1d27fc[RG1L];
            vo_28.add16(RG4W);
            RG1W = table1d2804[RG1L];
            vo_24.add16(RG1W);
            RG1L = unsignedlong(mkuw(argl0)) >> bcon(0xe); // truncated TODO check if arg is always word
            RG4L = vql_4c.peek32();
            RG4W &= 0x1;
            RG2L = unsignedlong(RG4W);
            RG4L = RG1L;
            RG4Bhi ^= RG1Bhi;
            RG4Blo &= 0x1;
            RG4L = unsignedlong(RG4W);
            if (RG2L != RG4L)
              //m_7839:
              RG1L++;
            else
              RG1L--;
            //m_783A:
            RG4L = RG1L;
            RG4Bhi ^= RG1Bhi;
            RG4Blo &= 0x3;
            RG4L <<= bcon(0xe);
            RG1L = argl0;
            RG1Bhi &= lcon(0x3f);
            RG1L |= RG4L;
            argl0 = RG1L;
            skip00046 = true;
          }

          if (skip00046)
          {
            bool skip00047 = false;
            //m_784F:
            RG4L = unsignedlong(RG5W);
            RG1L = DM2_move_075f_06bd(RG61p, RG4L) << 6;
            RG4L = unsignedlong(RG1W);
            RG2L = 0;
            RG2Blo = byte_at(DOWNCAST(unk, xp_18), 2);
            RG1L = RG4L;
            RG4L = RG1L % RG2L;
            RG1L /= RG2L;
            vw_44 = RG1W;
            if (RG1W != 0)
            {
              RG1W = DM2_APPLY_CREATURE_POISON_RESISTANCE(unsignedlong(vw_5c), unsignedlong(ddat.v1e01a4)) + signedlong(vw_44);
              RG4P = DOWNCAST(unk, xp_18);
              if (jnz_test8(location(RG4P + lcon(0x19)), lcon(0x10)))
              {
                RG4L = unsignedlong(ddat.v1e01a6);
                if (RG4L != lcon(0x1))
                  RG1W >>= bcon(0x2);
              }
              RG1L = unsignedlong(RG1W);
              i32 parl04 = RG1L;
              RG2L = signedlong(vw_48);
              RG4L = signedlong(vw_30);
              DM2_ATTACK_CREATURE(unsignedlong(vw_5c), RG4L, RG2L, lcon(0x200d), lcon(0x64), parl04);
              if (vw_50 == 0)
              {
                RG1P = DOWNCAST(unk, xp_18);
                if (jnz_test8(location(RG1P + lcon(0x1)), 2))
                {
                  RG1L = unsignedlong(RG5W);
                  RG4L = 0;
                  RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG1L, RG4L);
                  RG1W &= lcon(0x40);
                  RG1L = unsignedlong(RG1W);
                  if (RG1L != 0)
                    xp_10 = UPCAST(unk, DOWNCAST(unk, xp_14) + 2);
                }
              }
            }
            if (vw_38 != 0)
              skip00047 = true;
            else
            {
              RG1L = unsignedlong(vw_34);
              if (RG1L == lcon(0x5))
                skip00047 = true;
              else
              {
                if (RG1L == lcon(0xf))
                {
                  RG1L = unsignedlong(RG5W);
                  if (RG1L != lcon(0xff84))
                  {
                    if (RG1L >= lcon(0xff89) && RG1L <= lcon(0xff8e))
                      //m_7941:
                      RG1L = 0;
                    else
                      RG1L = 1;
                    //m_7943:
                    if (RG1L != 0)
                      skip00047 = true;
                  }
                }
              }
            }

            if (skip00047)
            {
              bool skip00049 = false;
              //m_7947:
              RG1L = unsignedlong(vw_44);
              if (RG1L < lcon(0x5))
              {
                RG1L = unsignedlong(RG5W);
                if (RG1L != lcon(0xff87))
                {
                  RG1L = lcon(0x1e);
                  skip00049 = true;
                }
              }

              if (!skip00049)
                //m_7967:
                RG1L = lcon(0x5f);

              //m_796C:
              RG1L = signedlong(RG1W);
              put16(parw05, RG1W);
              RG2L = signedlong(vw_48);
              RG4L = signedlong(vw_30);
              DM2_ATTACK_CREATURE(unsignedlong(vw_5c), RG4L, RG2L, 2, parw05, 0);
            }
            //m_798A:
            vl_20 = 1;
          }
        }
        else
          skip00044 = true;
      }
    }
    else
      skip00044 = true;

    if (skip00044)
    {
      //m_7728:
      if (vql_0c.peek16() != lcon(0xfffffffd))
        return 0;
    }
  }

  bool skip00052 = false;
  //m_7991:
  if (vql_0c.peek16() == lcon(0xfffffffd))
  {
    if (vl_20 == 0)
      return 0;
  }
  if (vw_50 != 0 || vw_38 != 0)
    skip00052 = true;
  else
  {
    RG1L = unsignedlong(RG5W);
    if (RG1L == lcon(0xff86))
      skip00052 = true;
  }

  if (skip00052)
  {
    bool skip00054 = false;
    //m_79C0:
    if (vw_38 == 0)
    {
      bool skip00053 = false;
      //m_79DE:
      RG4W = unsignedword(RG4Blo);
      RG4Blo = byte_at(RG61p, lcon(0x4));
      RG1L = unsignedlong(RG5W);
      if (RG1L != lcon(0xff86))
        skip00053 = true;
      else
      {
        RG4UW >>= bcon(0x2);
        if (RG4W != 0)
          skip00053 = true;
      }

      if (skip00053)
      {
        //m_79FC:
        RG1L = unsignedlong(RG5W);
        if (RG1L != lcon(0xff82))
          skip00054 = true;
        else
        {
          RG4UW >>= bcon(0x1);
          if (RG4W != 0)
            skip00054 = true;
        }
      }
    }
    else
    {
      RG4L = vl_3c;
      DM2_DEALLOC_RECORD(unsignedlong(vw_40));
      RG5L = vl_2c;
      mov16(location(RG61p + 2), RG5W);
      skip00054 = true;
    }

    if (skip00054)
    {
      //m_7A14:
      if (RG5UW >= mkuw(lcon(0xffffff89)))
      {
        if (RG5UW <= mkuw(lcon(0xffffff8a)))
        {
          RG1W = timdat.timerarray[word_at(RG61p, lcon(0x6))].getB();
          RG1L <<= 4;
          RG1UW >>= 14;
          vql_4c.poke32(RG1L);
          RG1W = unsignedword(RG1Blo);
          RG1Blo = byte_at(RG61p, lcon(0x4));
          vw_58 = RG1W;
        }
      }
      RG1L = unsignedlong(RG5W);
      if (RG1L != lcon(0xff87) && RG1L != lcon(0xff84) && RG1L != lcon(0xff8d))
        //m_7A70:
        RG1L = signedlong(vql_00.peek16());
      else
        RG1L = lcon(0xff);
      //m_7A76:
      RG1L = unsignedlong(RG1W);
      i32 parl07 = RG1L;
      RG3L = unsignedlong(vw_60);
      RG2L = unsignedlong(vw_54);
      RG4L = unsignedlong(RG4W);
      RG1L = unsignedlong(RG5W);
      DM2_CREATE_CLOUD(RG1L, RG4L, RG2L, RG3L, parl07);
    }
  }

  //m_7A98:
  RG3L = signedlong(vw_60);
  RG2L = signedlong(vw_54);
  RG62L = unsignedlong(mkuw(argl0)); // truncated TODO check if arg is always word
  DM2_CUT_RECORD_FROM(mk_record(RG62L), NULL, RG2W, RG3W);
  DM2_DELETE_MISSILE_RECORD(RG62L, DOWNCAST(unk, xp_10), signedlong(vo_28.peek16()), signedlong(vo_24.peek16()));
  if (vw_58 == 0)
    return lcon(0x1);
  RG1L = signedlong(DM2_MAX(DM2_MIN(vw_58, wcon(0xfe)), 40));
  RG3L = signedlong(RG1W);
  RG1L = unsignedlong(RG5W);
  if (RG1L != lcon(0xff89))
    //m_7B02:
    RG1L = unsignedlong(vql_4c.peek16()) + 2 & lcon(0x3);
  else
    RG1L = unsignedlong(vql_4c.peek16());
  //m_7B0E:
  RG2L = signedlong(RG1W);
  DM2_move_12b4_0d75(signedlong(vol_08.peek16()), signedlong(vol_04.peek16()), RG2L, RG3L);
  return lcon(0x1);
}

i32 DM2_move_075f_06bd(unk* xeaxp, i32 edxl)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_wlreg _RG6;
  c_lreg _RG7;
  i32 vl_00;
  i32 vl_04;

  RG3P = RG1P;
  RG6L = RG4L;
  RG4L = 0;
  ddat.v1e01a4 = RG4W;
  ddat.v1e01a6 = 3;
  RG5W = unsignedword(byte_at(RG1P, lcon(0x4)));
  RG1L = RG6L;
  RG1W &= lcon(0x3c00);
  RG1L = unsignedlong(RG1W) >> bcon(0xa);
  if (RG1L == lcon(0xf))
  {
    //m_6F32:
    RG4L = unsignedlong(RG6W);
    if (RG4L != lcon(0xff81))
    {
      //m_6F62:
      if (RG4L >= lcon(0xff83))
      {
        ddat.v1e01a6 = 5;
        if (RG4L != lcon(0xff86))
          return 0;
        ddat.v1e01a4 = RG5W;
        RG1L = unsignedlong(RG5W) / 8 + 1;
        return RG1L;
      }
      ddat.v1e01a6 = 1;
      RG1L = DM2_RAND();
      RG2L = RG1L;
      RG2Bhi ^= RG1Bhi;
      RG2Blo &= lcon(0xf);
      RG1L = DM2_RAND();
      RG1W &= lcon(0xf);
      RG2L += RG1L;
      RG2L += lcon(0xa);
      if (RG4L == lcon(0xff82))
      {
        ddat.v1e01a6 = lcon(0x7);
        RG2L <<= 4;
        RG2L += RG5L;
      }
    }
    else
    {
      RG1L = DM2_RAND();
      RG1W &= lcon(0xf);
      RG2L = RG1L;
      RG1L += lcon(0xa);
      ddat.v1e01a4 = RG1W;
      RG1L = DM2_RAND();
      RG1W &= lcon(0x1f);
      RG2L += RG1L;
    }
  }
  else
  {
    RG7L = unsignedlong(RG6W);
    RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG7L, lcon(0x9));
    RG2L = RG1L;
    if (RG1W != 0)
    {
      RG4L = unsignedlong(RG5W);
      vl_04 = RG4L;
      RG4L >>= bcon(0x1);
      RG2L = unsignedlong(RG1W) + RG4L;
      RG1L = (unsignedlong(byte_at(RG3P, lcon(0x5))) >> 4) + lcon(0x3) & lcon(0xffff);
      vl_00 = RG1L * RG1L;
      RG1L = unsignedlong(RG2W) * vl_00 >> 7;
      RG2L = RG1L;
      ddat.v1e01a6 = 4;
      RG4L = lcon(0xd);
      RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG7L, RG4L);
      ddat.v1e01a4 = RG1W;
      if (RG1W != 0)
      {
        RG1L = DM2_RAND();
        RG1W &= lcon(0x7f);
        RG1L = unsignedlong(RG1W);
        if (RG1L > vl_04)
        {
          RG1W = DM2_RAND16(ddat.v1e01a4 / 2 + 1);
          ddat.v1e01a4 -= RG1W;
        }
      }
    }
    RG2L += signedlong(DM2_RANDDIR());
    RG1W = DM2_QUERY_ITEM_WEIGHT(RG6W);
    RG2L += RG1L;
    RG1L = DM2_RAND();
    RG1Bhi &= 0x1;
    RG1L = unsignedlong(RG1W);
    RG4L = unsignedlong(byte_at(RG3P, lcon(0x5)));
    if (RG4L > RG1L)
      RG2L = 2 * RG2L;
  }
  //m_6FD0:
  RG1L = unsignedlong(RG2W);
  RG4L = unsignedlong(RG5W);
  RG2L = (RG1L + RG4L) / 16 + 1;
  RG1W = DM2_RAND16(RG2W / 2 + 1);
  RG4W = RG1W;
  RG1L = signedlong(DM2_RANDDIR()) + signedlong(RG4W);
  RG2L += RG1L;
  RG1L = unsignedlong(byte_at(RG3P, lcon(0x5))) / 8;
  RG4L = lcon(0x20) - RG1L;
  RG1L = RG4L;
  RG4L = unsignedlong(RG2W) - RG1L;
  RG1L = 2 * RG4L;
  RG4L = signedlong(RG1W);
  RG1L = signedlong(DM2_MAX(RG2W, RG4W));
  return signedlong(DM2_MIN(RG1W, 2 * RG5W));
}

// stacksize was 0x18
i32 DM2_ATTACK_DOOR(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_ql32 vql_00;
  c_ql32 vql_04;
  c_tim c38_08;
  bool flag;

  vql_04.poke32(RG1L);
  vql_00.poke32(RG4L);
  RG4L = signedlong(vql_00.peek16());
  RG1RP = DM2_GET_ADDRESS_OF_TILE_RECORD(vql_04.peek16(), RG4W);
  RG4P = RG1P;

// SPX: If "Weak door" mode is activated, then bypass these tests. It will lead to the destroy door part
#ifndef _DEBUG_OPTION_BREAK_DOOR_
  if (RG3L == 0)
    //m_8E73:
    flag = jz_test8(location(RG1P + lcon(0x3)), lcon(0x1));
  else
    flag = jz_test8(location(RG1P + 2), lcon(0xffffff80));
  //m_8E77:
  RG1Blo = flag ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  if (RG1L != 0)
    return 0;
  RG1L = DM2_GET_DOOR_STAT_0X10(DM2_IS_REBIRTH_ALTAR(RG4P) & lcon(0xff));
  if (RG2W < RG1W)
    return 0;
#endif // _DEBUG_OPTION_BREAK_DOOR_

  t_tile* RG11tilep = &mapdat.map[vql_04.peek16()][vql_00.peek16()];
  RG4Blo = tile_to_ubyte(*RG11tilep & 0x7);
  RG4L = unsignedlong(RG4Blo);
  if (RG4L != lcon(0x4))
    return 0;
  if (argw0 != 0)
  {
    c38_08.setmticks(ddat.v1d3248, unsignedlong(argw0) + timdat.gametick);
    c38_08.settype(0x2);
    c38_08.setactor(0);
    c38_08.setxyA(vql_04.peek8(), vql_00.peek8());
    DM2_QUEUE_TIMER(&c38_08);
    return lcon(0x1);
  }
  //m_8F17:
  *RG11tilep = (*RG11tilep & bcon(0xf8)) | 0x5;
  return lcon(0x1);
}

// argw0 corrected to i16
// stacksize was 0x50
i32 DM2_move_2fcf_0434(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG51p;
  i8* RG54p;
  i16 RG56w;
  i32 RG57l;
  i8 RG58b;
  i16 RG59w;
  i16 RG510w;
  i8 RG511b;
  c_wlreg _RG61;
  i16 RG62w;
  i32 RG71l;
  i32 RG72l;
  i32 RG73l;
  i32 vl_00;
  i32 vl_04;
  i32 vl_08;
  c_o32 vo_0c; // more often casted to a i16  poke 16 peek 16 32 add16
  i16 vw_10;
  c_ol32 vol_14; // poke 32 peek 16 32 inc32
  i16 vw_18;
  i16 vw_1c;
  i16 vw_20;
  i16 vw_24;
  i16 vw_28;
  i16 vw_2c;
  i16 vw_30;
  c_o32 vo_34; // poke 16 peek 16 32 inc16
  i16 vw_38;
  i32 vl_3c;
  i16 parw01;
  i16 parw02;
  i16 parw05;
  i16 parw06;
  i16 parw09;
  i16 parw10;
  i16 parw15;
  i16 parw16;
  i16 parw23;
  i16 parw24;
  i16 parw28;
  i16 parw29;
  bool flag;

  RG61L = RG1L;
  vw_20 = RG4W;
  vw_38 = RG2W;
  vo_0c.poke16(RG3W);
  RG4L = 0;
  ddat.v1e1020 = RG4L;
  vl_00 = RG4L;
  vol_14.poke32(0);
  RG1W = ddat.v1d3248;
  vw_28 = RG1W;
  vw_30 = RG1W;
  if (RG61W != lcon(0xffffffff))
  {
    //m_3A49B:
    RG2L = unsignedlong(RG61W);
    vl_04 = DM2_IS_OBJECT_FLOATING(RG2L);
    RG1L = RG61L;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    vw_2c = RG1W;
    vw_1c = 3;
    RG1L = signedlong(vw_2c);
    if (RG1L != lcon(0x4))
    {
      //m_3A4F5:
      if (RG1L == lcon(0xe))
      {
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
        RG1W = timdat.timerarray[word_at(RG1P, lcon(0x6))].getB();
        RG1L <<= 4;
        RG1UW >>= 14;
        ddat.v1e1024 = RG1W;
      }
    }
    else
    {
      RG4L = lcon(0x1e);
      RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG2L, RG4L);
      if (RG1W == 0)
        //m_3A4EC:
        vw_1c = 1;
      else
        vw_1c = 2;
    }
    //m_3A52A:
    RG3L = 0;
    vl_08 = RG3L;
  }
  else
  {
    if (ddat.v1e03a8 != 0)
    {
      ddat.v1e03a8 = 0;
      DM2_guidraw_29ee_000f();
    }
    DM2_map_3B001(signedlong(vo_0c.peek16()), signedlong(argw0));
    vw_1c = 2;
    if (ddat.v1e0976 != 0 || ddat.v1e0238 != 0)
      //m_3A483:
      RG1L = 0;
    else
      RG1L = 1;
    //m_3A485:
    vl_08 = RG1L;
    vw_2c = -1;
    vl_04 = 0;
  }
  //m_3A530:
  vl_3c = lcon(0x32);
  //m_3AD30:
  for (;;)
  {
    bool skip00804 = false;
    bool skip00813 = false;
    RG1L = vl_3c;
    flag = --RG1W != 0;
    vl_3c = RG1L;
    if (!flag)
      break;
    //m_3A53D:
    RG2L = signedlong(vo_0c.peek16());
    RG4L = RG2L;
    RG1P = DOWNCAST(t_tile*, mapdat.map);
    RG3L = signedlong(argw0);
    RG1P = pointer_at(RG1P + 4 * RG4L);
    RG4L ^= RG2L;
    RG4Blo = byte_at(RG1P, RG3L);
    RG1Blo = RG4Blo;
    RG1L = unsignedlong(RG1Blo) >> 5;
    RG56w = unsignedword(RG1Blo);
    vo_34.poke16(RG56w);
    RG511b = RG1Blo;
    RG1L = RG4L;
    RG1Bhi ^= RG4Bhi;
    RG1Blo &= lcon(0x8);
    if (RG511b != bcon(0x5))
    {
      //m_3A830:
      RG2L = signedlong(vo_34.peek16());
      if (RG2L != 2 || vl_04 != 0)
        skip00813 = true;
      else
      {
        RG1L = signedlong(RG1W);
        if (RG1L == 0)
          skip00813 = true;
        else
        {
          RG1L = RG4L;
          RG1Bhi ^= RG4Bhi;
          RG1Blo &= 0x1;
          RG1L = signedlong(RG1W);
          if (RG1L != 0)
            skip00813 = true;
          else
          {
            bool skip00811 = false;
            RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
            RG1W = word_at(RG1P, lcon(0xe));
            RG1L <<= bcon(0x8);
            RG1UW >>= bcon(0xc);
            RG4L = unsignedlong(RG1Blo);
            RG3L = lcon(0x6a);
            RG2L = lcon(0xb);
            RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(8, RG4Blo, RG2Blo, RG3Blo);
            RG3L = unsignedlong(RG1W);
            if (RG3L == 0)
              skip00811 = true;
            else
            {
              RG1L = signedlong(vw_2c);
              if (RG1L != lcon(0x4))
                skip00811 = true;
              else
              {
                RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG61UW));
                RG51p = RG1P;
                RG1W = word_at(RG1P, lcon(0xc)) & lcon(0x1f);
                vo_0c.poke16(RG1W);
                RG1W = word_at(RG51p, lcon(0xc));
                RG1L <<= bcon(0x6);
                _RG1.ushiftr11();
                argw0 = RG1W;
                RG1W = word_at(RG51p, lcon(0xc));
                RG1UW >>= bcon(0xa);
                vw_30 = RG1W;
              }
            }

            if (skip00811)
            {
              //m_3A8E7:
              if (vl_08 != 0)
              {
                if (RG3L == 0)
                {
                  if (ddat.v1e102e == 0)
                  {
                    vl_08 = unsignedlong(dm2_dballochandler.v1e13fe[0]);
                    if (vol_14.peek16() != 0)
                    {
                      DM2_LOAD_LOCALLEVEL_GRAPHICS_TABLE(signedlong(vo_0c.peek16()), signedlong(argw0), signedlong(vw_30));
                      DM2_LOAD_LOCALLEVEL_DYN();
                    }
                    RG510w = ddat.v1e025c;
                    RG4L = 0;
                    ddat.v1e025c = 0;
                    RG2L = signedlong(argw0);
                    DM2_DISPLAY_VIEWPORT(signedlong(ddat.v1e0258), signedlong(vo_0c.peek16()), RG2L);
                    DM2_DRAWINGS_COMPLETED();
                    ddat.v1e025c = RG510w;
                  }
                }
              }
              if (RG3L == 0)
              {
                //m_3AA4E:
                RG1L = signedlong(vw_30);
                vol_14.inc32();
                RG3P = DOWNCAST(i16, &argw0);
                RG2P = ADRESSOF(c_o32, &vo_0c);
                RG1L = DM_LOCATE_OTHER_LEVEL(RG1L, lcon(0x1), RG2P16, RG3P16, NULL);
              }
              else
              {
                RG3L = 0;
                RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(vo_0c.peek16(), argw0));
                //m_3A98F:
                for (;;)
                {
                  RG4L = RG1L;
                  RG1Blo = RG4W != wcon(0xfffe) ? 1 : 0;
                  RG1L = unsignedlong(RG1Blo);
                  if (RG1L == 0)
                    break;
                  RG1L = RG4L;
                  RG1Blo ^= RG4Blo;
                  RG1Bhi &= lcon(0x3c);
                  RG1L = unsignedlong(RG1W) >> bcon(0xa);
                  vw_2c = RG1W;
                  RG1L = signedlong(vw_2c);
                  if (RG1L > lcon(0x3))
                    break;
                  if (RG1L == 2)
                  {
                    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
                    RG2W = word_at(RG1P, 2) & lcon(0x6);
                    if (RG2W == 2)
                    {
                      RG1W = word_at(RG1P, 2);
                      RG1UW >>= bcon(0x3);
                      RG2L = unsignedlong(RG1W);
                      RG2UL >>= bcon(0x8);
                      RG2L &= lcon(0x1f);
                      if (RG2L == lcon(0xc))
                      {
                        RG1W = unsignedword(RG1Blo);
                        RG3L = RG1L;
                        break;
                      }
                    }
                  }
                  //m_3A9FF:
                  RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
                }
                //m_3AA0B:
                RG57l = unsignedlong(RG3W);
                RG1W = DM2_RAND16(CUTX16(DM2_3D93B(lcon(0x3), 0, RG57l, UPCAST(i16, ADRESSOF(c_o32, &vo_0c)), &argw0))) + 1;
                RG4L = unsignedlong(RG1W);
                RG3P = ADRESSOF(c_o32, &vo_0c);
                RG2L = RG57l;
                RG1L = DM2_3D93B(2, RG4L, RG2L, RG3P16, &argw0);
              }
              //m_3AA6C:
              vw_30 = RG1W;
              DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_30));
              if (RG61W == lcon(0xffffffff))
              {
                RG4L = signedlong(argw0);
                DM2_map_3B001(signedlong(vo_0c.peek16()), RG4L);
                if (mkuw(party.heros_in_party) > mkuw(0))
                {
                  RG3W = ddat.v1e102e;
                  if (RG3W == 0)
                  {
                    //m_3AB54:
                    c_hero* hero1 = &party.hero[0];
                    vo_34.poke16(RG3W);
                    //m_3ABE9:
                    for (;;)
                    {
                      RG71l = signedlong(vo_34.peek16());
                      RG1L = unsignedlong(party.heros_in_party);
                      if (RG71l >= RG1L)
                        break;
                      //m_3AB63:
                      if (hero1->curHP != 0)
                      {
                        RG1L = signedlong(DM2_MIN(hero1->curHP >> 2, 17));
                        RG4L = RG1L;
                        RG1L = (signedlong(DM2_RANDDIR()) + RG4L) * vol_14.peek32();
                        RG4L = signedlong(RG1W);
                        DM2_WOUND_PLAYER(RG71l, RG4L, lcon(0x30), 2);
                        RG1L = signedlong(ddat.v1e0272);
                        put16(parw16, RG1W);
                        RG1L = signedlong(ddat.v1e0270);
                        put16(parw15, RG1W);
                        RG4L = unsignedlong(hero1->herotype);
                        RG3L = lcon(0xfe);
                        RG2L = lcon(0x87);
                        DM2_QUEUE_NOISE_GEN2(lcon(0x16), RG4Blo, RG2Blo, RG3Blo, parw15, parw16, lcon(0xffff), lcon(0xbe), lcon(0xff));
                      }
                      //m_3ABD7:
                      vl_00 = 1;
                      hero1++; // struct advance
                      vo_34.inc16();
                    }
                  }
                  else
                  {
                    if (dm2_dballochandler.v1e13fe[0] == 0 || ddat.v1e0976 != 0 || ddat.v1e0238 != 0)
                      //m_3AADB:
                      RG1L = 0;
                    else
                      RG1L = 1;
                    //m_3AADD:
                    vl_08 = RG1L;
                    c_hero* hero2 = &party.hero[0];
                    RG2L = 0;
                    vo_34.poke16(RG2W);
                    //m_3AB3C:
                    for (;;)
                    {
                      RG72l = signedlong(vo_34.peek16());
                      RG1L = unsignedlong(party.heros_in_party);
                      if (RG72l >= RG1L)
                        break;
                      //m_3AAEF:
                      if (hero2->curHP != 0)
                      {
                        RG1W = party.get_player_weight((e_hero)vo_34.peek16());
                        RG4L = unsignedlong(RG1W);
                        RG1L = 24 * RG4L;
                        RG4L += RG1L;
                        RG1W = hero2->get_max_load();
                        RG2L = unsignedlong(RG1W);
                        RG1L = RG4L;
                        RG4L = RG1L % RG2L;
                        RG1L /= RG2L;
                        RG1L++;
                        RG4L = signedlong(RG1W);
                        DM2_ADJUST_STAMINA(CUTX16(RG72l), RG4W);
                      }
                      //m_3AB31:
                      hero2++; // struct advance
                      vo_34.inc16();
                    }
                  }
                }
                //m_3AC00:
                RG4L = 0;
                ddat.v1e102e = RG4W;
                continue;
              }
              //m_3AC0E:
              RG1L = signedlong(vw_2c);
              if (RG1L != lcon(0x4) || argl1 == 0)
                continue;
              if (vw_20 >= wcon(0x0))
              {
                DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_28));
                RG2L = signedlong(vw_38);
                RG4L = signedlong(vw_20);
                RG1L = unsignedlong(RG61W);
                RG3L = 0;
                DM2_ATTACK_CREATURE(RG1L, RG4L, RG2L, RG3L, 0, lcon(0x14));
              }
            }
            //m_3A8D6:
            DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_30));
            continue;
          }
        }
      }
    }
    else
    {
      RG1L = signedlong(RG1W);
      if (RG1L == 0)
        break;
      RG4L = RG3L;
      RG1RP = DM2_GET_ADDRESS_OF_TILE_RECORD(RG2W, RG4W);
      RG54p = RG1P;
      RG1W = word_at(RG1P, 2) & lcon(0x6000);
      if (RG1W == lcon(0x2000))
      {
        RG1L = signedlong(vw_2c);
        if (RG1L != lcon(0x4))
          break;
      }
      skip00804 = true;
    }

    if (skip00804)
    {
      //m_3A5B2:
      RG1L = signedlong(vw_1c);
      if (RG1L != lcon(0x3))
      {
        RG1W = word_at(RG54p, 2); // TODO: gets extended w/o conversion now
        RG1L = 2 * RG1L;
        RG1UW >>= 14;
        RG1L = unsignedlong(RG1W);
        RG4L = signedlong(vw_1c);
        if ((RG4L & RG1L) == 0)
          break;
      }
      bool skip00806 = false;
      //m_3A5DC:
      RG1W = word_at(RG54p, 2) & lcon(0x1f);
      if (RG1W != vo_0c.peek16())
        skip00806 = true;
      else
      {
        RG1W = word_at(RG54p, 2);
        RG1L <<= bcon(0x6);
        _RG1.ushiftr11();
        if (RG1W != argw0)
          skip00806 = true;
        else
        {
          RG1W = word_at(RG54p, lcon(0x4));
          RG1UW >>= bcon(0x8);
          if (RG1W != vw_30)
            skip00806 = true;
          else
            RG1L = 1;
        }
      }

      if (skip00806)
        //m_3A613:
        RG1L = 0;
      //m_3A615:
      RG73l = RG1L;
      RG1W = word_at(RG54p, 2) & lcon(0x1f);
      vo_0c.poke16(RG1W);
      RG1W = word_at(RG54p, 2);
      RG1L <<= bcon(0x6);
      _RG1.ushiftr11();
      argw0 = RG1W;
      RG1W = word_at(RG54p, 2);
      RG1UW >>= bcon(0xf);
      RG1L = unsignedlong(RG1W);
      ddat.v1e1020 = RG1L;
      RG1W = word_at(RG54p, lcon(0x4));
      RG1UW >>= bcon(0x8);
      vw_30 = RG1W;
      DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_30));
      if (RG61W != lcon(0xffffffff))
      {
        //m_3A6FA:
        RG1L = signedlong(vw_2c);
        if (RG1L != lcon(0x4) || argl1 == 0)
        {
          bool skip00808 = false;
          //m_3A7CD:
          RG1L = signedlong(vw_2c);
          if (RG1L != lcon(0xe))
          {
            //m_3A7E7:
            if (jz_test8(location(RG54p + lcon(0x3)), lcon(0x10)))
            {
              RG1L = signedlong(vw_20);
              if (RG1L != lcon(0xfffffffe))
              {
                RG1L = unsignedlong(RG61W);
                RG2L = RG1L >> bcon(0xe);
                RG4W = word_at(RG54p, 2);
                RG4L <<= 4;
                RG4UW >>= 14;
                RG4L = unsignedlong(RG4W) + RG2L;
                RG4L &= 0x3;
                RG4L <<= bcon(0xe);
                RG1Bhi &= lcon(0x3f);
                RG1L |= RG4L;
                skip00808 = true;
              }
            }
          }
          else
          {
            RG4L = unsignedlong(RG61W);
            RG1L = DM2_ROTATE_RECORD_BY_TELEPORTER(RG54p, RG4L);
            skip00808 = true;
          }

          if (skip00808)
            //m_3A821:
            RG61L = RG1L;
        }
        else
        {
          if (jnz_test8(location(RG54p + lcon(0x3)), lcon(0xffffff80)))
          {
            RG1W = ddat.v1d3248;
            if (RG1W != vw_28)
            {
              vw_18 = RG1W;
              DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_28));
              put16(parw06, vw_38);
              put16(parw05, vw_20);
              DM2_QUEUE_NOISE_GEN1(lcon(0x18), 0, lcon(0x89), lcon(0x61), lcon(0x80), parw05, parw06, lcon(0x1));
              DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_18));
            }
            //m_3A776:
            put16(parw10, argw0);
            put16(parw09, vo_0c.peek16());
            RG3L = lcon(0x61);
            RG2L = lcon(0x89);
            RG4L = 0;
            RG1L = lcon(0x18);
            DM2_QUEUE_NOISE_GEN1(RG1Blo, RG4Blo, RG2Blo, RG3W, lcon(0x80), parw09, parw10, lcon(0x1));
          }
          //m_3A7A3:
          RG1W = word_at(RG54p, 2);
          RG1L <<= 4;
          RG1UW >>= 14;
          RG2L = signedlong(RG1W);
          RG1W = word_at(RG54p, 2);
          RG1L *= 8;
          RG1UW >>= bcon(0xf);
          RG4L = unsignedlong(RG1W);
          DM2_ROTATE_CREATURE(unsignedlong(RG61W), RG4L, RG2L);
        }
      }
      else
      {
        RG4L = signedlong(argw0);
        DM2_map_3B001(signedlong(vo_0c.peek16()), RG4L);
        if (jnz_test8(location(RG54p + lcon(0x3)), lcon(0xffffff80)))
        {
          put16(parw02, ddat.v1e0272);
          put16(parw01, ddat.v1e0270);
          RG3L = lcon(0x61);
          RG2L = lcon(0x89);
          RG4L = 0;
          DM2_QUEUE_NOISE_GEN1(lcon(0x18), RG4Blo, RG2Blo, RG3W, lcon(0x80), parw01, parw02, lcon(0xffff));
        }
        RG1W = vw_28;
        if (RG1W != vw_30)
          vl_08 &= unsignedlong(dm2_dballochandler.v1e13fe[0]);
        if (jz_test8(location(RG54p + lcon(0x3)), lcon(0x10)))
          //m_3A6D1:
          RG1L = signedlong(ddat.v1e0258);
        else
          RG1L = 0;
        //m_3A6D9:
        RG4W = word_at(RG54p, 2);
        RG4L <<= 4;
        RG4UW >>= 14;
        RG4L = unsignedlong(RG4W);
        RG1L += RG4L;
        RG1L &= 0x3;
        RG1L = signedlong(RG1W);
        party.rotate(RG1W);
      }
      //m_3A823:
      if (RG73l != 0)
        break;
    }

    if (skip00813)
    {
      //m_3AC64:
      RG1L = signedlong(vo_34.peek16());
      if (RG1L == lcon(0x3))
      {
        RG1Blo = RG61W != wcon(0xffff) ? 1 : 0;
        RG1L = unsignedlong(RG1Blo);
        if (RG1L != 0)
        {
          RG1L = signedlong(vw_2c);
          if (RG1L != lcon(0xe) && RG1L != lcon(0x4))
          {
            RG4W &= 0x4;
            RG1L = signedlong(RG4W);
            if (RG1L == 0)
            {
              RG1L = signedlong(vw_30);
              RG3P = DOWNCAST(i16, &argw0);
              RG1L = DM_LOCATE_OTHER_LEVEL(RG1L, lcon(0x1), UPCAST(i16, ADRESSOF(c_o32, &vo_0c)), RG3P16, NULL);
              vw_30 = RG1W;
              RG1L = signedlong(RG1W);
              DM2_CHANGE_CURRENT_MAP_TO(RG1L);
            }
            //m_3ACD0:
            RG1L = DM2_query_0cee_06dc(signedlong(vo_0c.peek16()), signedlong(argw0));
            RG59w = RG1W;
            RG4L = signedlong(RG1W);
            RG1W = table1d27fc[RG4L];
            vo_0c.add16(RG1W);
            RG1W = table1d2804[RG4L];
            argw0 += RG1W;
            RG59w += wcon(0x2);
            RG59w &= wcon(0x3);
            RG1L = unsignedlong(RG61W) >> bcon(0xe);
            RG4L = unsignedlong(RG1W);
            RG1L = signedlong(RG59w);
            RG4L -= RG1L;
            RG4L++;
            RG4L &= 0x2;
            RG4L >>= bcon(0x1);
            RG1L += RG4L;
            RG1L &= 0x3;
            RG61L &= lcon(0xffff3fff);
            RG1L <<= bcon(0xe);
            RG61L |= RG1L;
            continue;
          }
        }
      }
      break;
    }
  }
  //m_3AD40:
  RG1W = vo_0c.peek16();
  ddat.v1e102a = RG1W;
  ddat.v1e1028 = argw0;
  RG1W = vw_30;
  ddat.v1e102c = RG1W;
  RG1L = unsignedlong(RG61W) >> bcon(0xe);
  ddat.v1e1026 = RG1W;
  if (vol_14.peek16() != 0 && argl1 != 0)
  {
    RG1Blo = RG61W != wcon(0xffff) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 0)
    {
      RG1L = signedlong(argw0);
      put16(parw24, RG1W);
      RG1L = signedlong(vo_0c.peek16());
      put16(parw23, RG1W);
      RG4L = unsignedlong(RG61W);
      RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L);
      RG58b = RG1Blo;
      RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG4L) & lcon(0xff);
      RG3L = lcon(0xfe);
      DM2_QUEUE_NOISE_GEN2(RG1Blo, RG58b, lcon(0x85), RG3Blo, parw23, parw24, lcon(0x1), lcon(0x3a), lcon(0x80));
    }
  }
  //m_3ADCE:
  if (vl_00 != 0)
  {
    vw_18 = ddat.v1d3248;
    vw_10 = ddat.v1e0266;
    vw_24 = ddat.v1e0270;
    RG62w = ddat.v1e0272;
    ddat.v1d3248 = lcon(0xffffffff);
    RG2L = signedlong(vw_30);
    DM2_LOAD_LOCALLEVEL_GRAPHICS_TABLE(signedlong(vo_0c.peek16()), signedlong(argw0), RG2L);
    c_hero* hero3 = &party.hero[0];
    RG1L = 0;
    vo_34.poke16(RG1W);
    //m_3AE78:
    for (;;)
    {
      RG1L = signedlong(vo_34.peek16());
      RG4L = unsignedlong(party.heros_in_party);
      if (RG1L >= RG4L)
      {
        RG2L = signedlong(vw_10);
        RG4L = signedlong(RG62w);
        DM2_LOAD_LOCALLEVEL_GRAPHICS_TABLE(signedlong(vw_24), RG4L, RG2L);
        DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_18));
        break;
      }
      //m_3AE31:
      if (hero3->curHP != 0)
      {
        RG1L = signedlong(argw0);
        put16(parw29, RG1W);
        RG1L = signedlong(vo_0c.peek16());
        put16(parw28, RG1W);
        RG4L = unsignedlong(hero3->herotype);
        RG3L = lcon(0xfe);
        RG2L = lcon(0x82);
        DM2_QUEUE_NOISE_GEN2(lcon(0x16), RG4Blo, RG2Blo, RG3Blo, parw28, parw29, 2, lcon(0x69), lcon(0xc8));
      }
      hero3++; // struct advance
      vo_34.inc16();
    }
  }
  //m_3AEAE:
  DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_28));
  DM2_SOUND8(true);
  return vl_04;
}

// belongs to DM2_move_2c1d_028c
static void DM2_move_075f_1bc2(s_4bytearray* eaxp_s08, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p = DOWNCAST(s_4bytearray, eaxp_s08);
  i16 RG6w;
  i16 RG7w;

  RG6w = RG4W;
  RG1L = RG3L;
  RG3L = signedlong(argw0);
  RG7w = RG1W;
  RG4L = signedlong(RG2W);
  RG1L = signedlong(DM2_CALC_VECTOR_DIR(RG6w, RG4W, RG7w, RG3W));
  RG4L = unsignedlong(mkuw(argl1)); // truncated TODO check if arg is always word
  if (RG4L == lcon(0xff))
  {
    i16 n = RG1W / 4; // TODO, check that RG1W % 4 == 0
    RG1P = DOWNCAST(s_4bytearray, &table1d26f0[n]);
    DM2_COPY_MEMORY(RG5p, RG1P, lcon(0x4));
    RG1L = DM2_RANDBIT() ? 1 : 0;
    if (RG1W != 0)
    {
      RG1Bhi = byte_at(RG5p);
      RG1Blo = byte_at(RG5p, lcon(0x1));
      mov8(location(RG5p), RG1Blo);
      mov8(location(RG5p + lcon(0x1)), RG1Bhi);
    }
    RG1L = DM2_RANDBIT() ? 1 : 0;
    if (RG1W != 0)
    {
      RG1Bhi = byte_at(RG5p, 2);
      RG1Blo = byte_at(RG5p, lcon(0x3));
      mov8(location(RG5p + 2), RG1Blo);
      mov8(location(RG5p + lcon(0x3)), RG1Bhi);
    }
    return;
  }
  //m_87C3:
  RG1L = 2 * RG1L;
  RG4L = RG1L;
  RG4Bhi ^= RG1Bhi;
  RG4Blo &= 0x2;
  RG4L = unsignedlong(RG4W);
  if (RG4L == 0)
    argl1++;
  RG4L = unsignedlong(mkuw(argl1)) / 2 & lcon(0x1); // truncated TODO check if arg is always word
  RG1P = DOWNCAST(s_4bytearray, &table1d26d0[CUTX16(unsignedlong(RG1W) + RG4L)]); // adj
  DM2_COPY_MEMORY(RG5p, RG1P, lcon(0x4));
}

i32 DM2_move_2c1d_028c(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  s_4bytearray s08_00;
  i16 parw00;

  RG3L = RG1L;
  if (party.heros_in_party == 0)
    return lcon(0xffffffff);
  RG1W -= ddat.v1e0280;
  RG1L = signedlong(DM2_ABS(RG1W));
  RG5l = signedlong(RG1W);
  RG1L = RG4L;
  RG1W -= ddat.v1e027e;
  RG1L = signedlong(DM2_ABS(RG1W)) + RG5l;
  if (RG1L > lcon(0x1))
    return lcon(0xffffffff);
  RG1L = unsignedlong(RG2W);
  i32 parl01 = RG1L;
  RG1L = signedlong(RG4W);
  put16(parw00, RG1W);
  RG3L = signedlong(RG3W);
  RG2L = signedlong(ddat.v1e027e);
  DM2_move_075f_1bc2(&s08_00, signedlong(ddat.v1e0280), RG2L, RG3L, parw00, parl01);
  RG4L = 0;
  //m_35A64:
  for (;;)
  {
    RG1L = unsignedlong(RG4W);
    if (RG1L >= lcon(0x4))
      return lcon(0xffffffff);
    //m_35A51:
    RG1W = DM2_GET_PLAYER_AT_POSITION(unsignedword(s08_00.barr_00[RG1L]));
    if (RG1W >= 0)
      return RG1L;
    RG4L++;
  }
  FOREND
}
