#include <dm2data.h>
#include <xtypes.h>
#include <skgdtqdb.h>
#include <skcloud.h>
#include <util.h>
#include <sfx.h>
#include <bgdat.h>
#include <skhero.h>
#include <startend.h>
#include <bcreatur.h>
#include <sktimer.h>
#include <sklight.h>
#include <skmovrec.h>
#include <SK1C9A.h>
#include <skmap.h>
#include <skrandom.h>
#include <skmove.h>
#include <skengage.h>

#include <emu.h>
#include <regs.h>

i32 DM2_ENGAGE_COMMAND(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG61l; // TODO: bool?
  c_wlreg _RG62;
  i16 RG63w;
  c_lreg _RG64;
  c_wlreg _RG65;
  i8* RG71p = NULL;
  c_lreg _RG72;
  c_lreg _RG73;
  i32 RG74l;
  c_ol32 vol_00; // poke 32 peek 8 16 32 and16
  c_ql32 vql_04;
  c_tim c38_08;
  i32 vl_14;
  i32 vl_18;
  i32 vl_1c;
  c_ol32 vol_20; // poke 32 peek 32 shr16
  i16 vw_24;
  i32 vl_28;
  c_ql32 vql_2c;
  i16 vw_30;
  i16 vw_34;
  i16 vw_38;
  i16 vw_3c;
  i16 vw_40;
  i16 vw_44;
  i16 vw_48;
  c_ql32 vql_4c;
  i16 vw_50;
  i16 vw_54;
  c_ql32 vql_58;
  i16 vw_5c;
  i16 vw_60;
  i16 vw_64;
  c_o32 vo_68; // poke 16 peek 16 32 sar16 add16
  i8 vb_6c;
  i8 vb_70;
  i16 parw01;
  i16 parw02;
  i16 parw03;
  i16 parw06;
  i16 parw07;
  i16 parw09;
  i16 parw10;
  i16 parw11;
  i16 parw12;
  i16 parw13;
  i16 parw15;
  i8 parb17;
  i16 parw20;
  i8 parb22;
  i16 parw23;
  i16 parw24;
  i16 parw25;
  bool flag;
  bool skip00715 = false;
  bool skip00714 = false;
  bool skip00713 = false;
  bool skip00712 = false;
  bool skip00711 = false;
  bool skip00717 = false;
  bool skip00719 = false;
  bool skip00720 = false;
  bool skip00722 = false;
  bool skip00723 = false;

  vql_04.poke32(RG1L);
  vol_00.poke32(RG4L);
  vl_14 = 0;
  RG1L = vol_00.peek32();
  RG1W &= lcon(0x8000);
  vw_48 = RG1W;
  vol_00.and16(lcon(0x7fff)); // modified
  RG1L = unsignedlong(vol_00.peek16());
  RG4Blo = ddat.v1e0b40[RG1L].b_00;
  ddat.v1e0b7f = RG4Blo;
  RG4Blo = ddat.v1e0b40[RG1L].b_01;
  ddat.v1e0b80 = RG4Blo;
  RG1Blo = ddat.v1e0b40[RG1L].b_02;
  ddat.v1e0b7e = RG1Blo;
  RG1W = party.curactmode;
  vw_34 = RG1W;
  RG1Blo ^= 1;
  vql_4c.poke32(RG1L);
  RG4L = unsignedlong(vql_04.peek16());
  c_hero* hero = &party.hero[RG4L];
  if (hero->curHP == 0)
    return 0;
  //m_315DC:
  RG1L = DM2_QUERY_CUR_CMDSTR_ENTRY(2);
  vw_54 = RG1W;
  RG1L = DM2_QUERY_CUR_CMDSTR_ENTRY(lcon(0xd));
  vb_6c = RG1Blo;
  RG3L = 0;
  vw_3c = RG3W;
  RG1L = unsignedlong(vw_34);
  hero->handcmd[RG1L] = RG3Blo = CUTX8(vw_54);
  RG1W = hero->item[RG1L];
  vql_58.poke32(RG1L);
  if (RG1W != lcon(0xffffffff))
  {
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(vql_58.peek16())));
    RG71p = RG1P;
  }
  RG1L = unsignedlong(hero->absdir);
  vw_44 = RG1W;
  RG1L = unsignedlong(hero->absdir);
  RG4W = ddat.v1e0270;
  RG3W = table1d27fc[RG1L];
  RG4L += RG3L;
  vw_5c = RG4W;
  RG4W = ddat.v1e0272;
  RG65L = 0; // TODO: added to define highword of RG6
  RG65W = table1d2804[RG1L];
  RG4L += RG65L;
  vw_24 = RG4W;
  RG1L = DM2_GET_CREATURE_AT(unsignedlong(vw_5c), unsignedlong(vw_24));
  ddat.v1e0b4c = RG1W;
  if (RG1W != lcon(0xffffffff))
  {
    RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(unsignedlong(RG1W));
    vb_70 = RG1Blo;
    RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(RG1Blo)));
    RG2P = RG1P;
  }
  //m_316A5:
  vol_20.poke32(DM2_QUERY_CUR_CMDSTR_ENTRY(lcon(0x3)));
  RG1L = DM2_QUERY_CUR_CMDSTR_ENTRY(0);
  vw_50 = RG1W;
  RG4L = DM2_QUERY_CUR_CMDSTR_ENTRY(lcon(0x4)) + (DM2_RANDBIT() ? 1 : 0);
  vw_40 = RG4W;
  RG1L = DM2_QUERY_CUR_CMDSTR_ENTRY(lcon(0x9));
  vo_68.poke16(RG1W);
  RG1L = DM2_QUERY_CUR_CMDSTR_ENTRY(lcon(0x5));
  vl_28 = RG1L;
  vw_60 = RG1W;
  RG1L = DM2_QUERY_CUR_CMDSTR_ENTRY(lcon(0x10));
  vw_64 = RG1W;
  RG1L = DM2_QUERY_CUR_CMDSTR_ENTRY(lcon(0x7));
  RG3L = unsignedlong(vw_34);
  hero->handdefenseclass[RG3L] = RG1Blo;
  RG4L = signedlong(vw_24);
  RG1L = dm2_GET_TILE_VALUE(signedlong(vw_5c), RG4L);
  RG1W = unsignedword(RG1Blo);
  vw_38 = RG1W;
  RG61l = 1;
  RG1W = vw_54 - 1;
  if (RG1UW > mkuw(lcon(0x35)))
    jump L_fin;
  RG1L = unsignedlong(RG1W);
  switch (RG1L)
  {
    case 0:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 36:
    case 39:
    case 40:
    case 42:
    case 51:
    case 52:
      jump L_fin;

    case 1:
      //m_317A6:
      RG4L = signedlong(vw_60);
      RG1L = signedlong(DM2_MAX(32, RG4W));
      vw_60 = RG1W;
      c38_08.settype(0x47);
      RG1Blo = ddat.savegames1.b_02;
      RG1Bhi = RG1Blo + 1;
      ddat.savegames1.b_02 = RG1Bhi;
      if (RG1Blo == 0)
      {
        if (ddat.v1e0976 != 0)
        {
          RG4L = signedlong(ddat.v1e0976) - lcon(0x1);
          party.hero[RG4L].heroflag |= 0x4000;
        }
      }
      c38_08.setmticks(ddat.v1e0266, signedlong(vw_60) + timdat.gametick);
      DM2_QUEUE_TIMER(&c38_08);
      jump L_fin;

    case 2:
      //m_31828:
      RG62L = DM2_QUERY_CUR_CMDSTR_ENTRY(lcon(0x6)) + lcon(0xff80);
      RG2L = 1;
      RG4L = unsignedlong(vw_50);
      RG1W = DM2_QUERY_PLAYER_SKILL_LV(vql_04.peek16(), RG4W, RG2L);
      RG4L = signedlong(RG1W);
      RG1L = signedlong(DM2_MIN(6, RG4W));
      RG4L = lcon(0x7) - RG1L;
      RG1L = RG4L;
      if (RG1W > hero->curMP)
      {
        RG2L = signedlong(hero->curMP);
        RG4L = signedlong(vw_60) * RG2L;
        RG2L = signedlong(RG1W);
        RG1L = RG4L;
        RG4L = RG1L % RG2L;
        RG1L /= RG2L;
        RG4L = signedlong(RG1W);
        RG1L = signedlong(DM2_MAX(2, RG4W));
        vw_60 = RG1W;
        RG1W = hero->curMP;
      }
      //m_3189E:
      RG3L = unsignedlong(RG1W);
      RG2L = signedlong(vw_60);
      RG4L = signedlong(RG62W);
      RG1L = DM2_CAST_CHAMPION_MISSILE_SPELL(unsignedlong(vql_04.peek16()), RG4L, RG2L, RG3L);
      RG61l = RG1L;
      if (RG1L != 0)
        jump L_fin;
      vo_68.sar16(lcon(0x1));
      jump L_fin;

    case 3:
    case 7:
      //m_318CD:
      RG1L = unsignedlong(vw_38) >> 5 & lcon(0xff);
      if (RG1L == lcon(0x4))
      {
        RG1Blo = CUTX8(vw_38) & lcon(0x7);
        RG1L = unsignedlong(RG1Blo);
        if (RG1L == lcon(0x4))
        {
          RG2L = signedlong(vw_50);
          RG4L = signedlong(vw_34);
          RG1W = DM2_COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(vql_04.peek16(), RG4L, RG2L);
          RG2L = signedlong(RG1W);
          DM2_ATTACK_DOOR(unsignedlong(vw_5c), unsignedlong(vw_24), RG2L, 0, 2);
          vol_20.poke32(lcon(0x6));
          RG2L = signedlong(vw_24);
          RG3L = signedlong(vw_5c);
          RG4L = RG2L;
          RG1RP = DM2_GET_ADDRESS_OF_TILE_RECORD(RG3W, RG4W);
          RG4L = unsignedlong(vw_3c) + 1;
          vql_2c.poke16(RG4W);
          RG4L = signedlong(vql_2c.peek16());
          put16(parw03, RG4W);
          put16(parw02, RG2W);
          put16(parw01, RG3W);
          RG1L = DM2_IS_REBIRTH_ALTAR(RG1P);
          RG4L = unsignedlong(RG1Blo);
          DM2_QUEUE_NOISE_GEN2(lcon(0xe), RG4Blo, lcon(0x8d), lcon(0xfe), parw01, parw02, parw03, lcon(0x3c), lcon(0xc8));
          jump L_fin;
        }
      }
      //m_31989:
      RG1Blo = ddat.v1e0b4c != wcon(0xffff) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L == 0 || jz_test8(location(RG2P), lcon(0x1)))
        skip00711 = true;
      else
      {
        RG1L = unsignedlong(byte_at(RG2P, 2));
        if (RG1L != lcon(0xff))
          skip00711 = true;
        else
        {
          RG1L = unsignedlong(byte_at(RG2P, lcon(0x8)));
          if (RG1L != lcon(0xff))
            skip00711 = true;
          else
          {
            vo_68.sar16(lcon(0x1));
            skip00717 = true;
          }
        }
      }
      break;

    case 4:
      //m_31758:
      RG1W = DM2_QUERY_PLAYER_SKILL_LV(vql_04.peek16(), 14, RG61l);
      RG3W = vw_60 + RG1W;
      vw_60 = RG3W;
      RG2L = unsignedlong(vw_24);
      RG4L = unsignedlong(vw_5c);
      RG1L = DM2_CONFUSE_CREATURE(unsignedlong(RG3W), RG4L, RG2L);
      RG61l = RG1L;
      if (RG1L == 0)
        vo_68.sar16(2);
      RG4L = 0;
      vw_3c = RG4W;
      jump L_fin;

    case 5:
    case 37:
    case 38:
      //m_31E71:
      DM2_PROCEED_LIGHT(signedlong(vw_54), signedlong(vw_60));
      jump L_fin;

    case 6:
      //m_31A52:
      RG3L = unsignedlong(ddat.v1e0272);
      RG2L = unsignedlong(ddat.v1e0270);
      RG4L = unsignedlong(DM2_MAX(2, vw_60));
      RG1L = lcon(0xff8e);
      break;

    case 8:
      //m_31A8A:
      RG4L = signedlong(vw_60);
      RG1L = signedlong(DM2_MAX(32, RG4W));
      vw_60 = RG1W;
      RG4L = lcon(0xff);
      RG1L = signedlong(DM2_MIN(CUTX16(unsignedlong(ddat.savegames1.b_04) + unsignedlong(vw_60)), RG4W));
      ddat.savegames1.b_04 = RG1Blo;
      jump L_fin;

    case 9:
      //m_31D40:
      RG1L = signedlong(ddat.v1e0258);
      RG4W = ddat.v1e0270;
      RG3W = table1d27fc[RG1L];
      RG4L += RG3L;
      vw_5c = RG4W;
      RG4W = ddat.v1e0272;
      RG2W = table1d2804[RG1L];
      RG4L += RG2L;
      vw_24 = RG4W;
      RG74l = RG61l;
      RG1L = DM2_GET_CREATURE_AT(unsignedlong(vw_5c), unsignedlong(vw_24));
      if (RG1W != lcon(0xffffffff))
      {
        RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG1W));
        if ((RG1Bhi & bcon(0x80)) == 0)
          RG74l ^= RG61l;
      }
      RG2L = signedlong(vw_24);
      RG3L = signedlong(vw_5c);
      RG1L = (dm2_GET_TILE_VALUE(RG3L, RG2L) & lcon(0xff)) >> 5 & lcon(0xff);
      if (RG1L == 2 && RG74l != 0)
      {
        ddat.v1e102e = 1;
        put16(parw13, RG2W);
        DM2_MOVE_RECORD_TO(lcon(0xffff), signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), RG3L, parw13);
        RG4L = 0;
        ddat.v1e102e = RG4W;
        jump L_fin;
      }
      //m_31E01:
      vol_20.poke32(0);
      jump L_fin;

    case 10:
      //m_31E0C:
      RG1L = unsignedlong(ddat.savegames1.b_03) + vl_28;
      RG4L = signedlong(RG1W);
      RG1L = signedlong(DM2_MIN(wcon(0xc8), RG4W));
      ddat.savegames1.b_03 = RG1Blo;
      jump L_fin;

    case 11:
      //m_31E2E:
      RG3L = 5;
      skip00712 = true;
      break;

    case 12:
      //m_31E5C:
      RG3L = 4;
      skip00712 = true;
      break;

    case 13:
      //m_31E63:
      RG3L = 6;
      skip00712 = true;
      break;

    case 14:
      //m_31E6A:
      RG3L = 3;
      skip00712 = true;
      break;

    case 15:
      //m_31C4C:
      DM2_PLAYER_CONSUME_OBJECT(signedlong(vql_04.peek16()), unsignedlong(vql_58.peek16()), signedlong(vw_34));
      jump L_fin;

    case 16:
      //m_31C6B:
      vl_14 = RG61l;
      RG4L = signedlong(vw_34);
      RG1L = DM2_FIND_POUCH_OR_SCABBARD_POSSESSION_POS(signedlong(vql_04.peek16()), RG4L);
      if (RG1W < 0)
        jump L_fin;
      RG4L = unsignedlong(RG1W);
      RG72L = unsignedlong(vql_04.peek16());
      RG1L = DM2_REMOVE_POSSESSION(RG72L, RG4L);
      vql_58.poke32(RG1L);
      RG4L = unsignedlong(RG1W);
      DM2_EQUIP_ITEM_TO_HAND(RG72L, RG4L, RG3L);
      jump L_fin;

    case 31:
      //m_31AC0:
      RG1L = unsignedlong(vql_4c.peek16());
      vl_1c = RG1L;
      RG2L = unsignedlong(hero->item[RG1L]);
      RG73L = unsignedlong(vql_04.peek16());
      RG1L = DM2_IS_MISSILE_VALID_TO_LAUNCHER(RG73L, RG3L, RG2L);
      if (RG1L == 0)
      {
        if (vw_48 == 0)
        {
          RG1L = DM2_UPDATE_GLOB_VAR(lcon(0x43), RG61l, lcon(0x3));
          RG4L = signedlong(CUTX16(lcon(0x8) - RG1L));
          RG1L = signedlong(DM2_MAX(CUTX16(RG61l), RG4W));
          ddat.v1e0b6c = RG1W;
          ddat.v1e0b7c = lcon(0xfffffffe);
          RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(vql_58.peek16()), lcon(0x5));
          ddat.v1e0b78 = RG1W;
          RG1L = vql_4c.peek32();
          ddat.v1e0b72 = RG1W;
          vo_68.poke16(wcon(0x0));
        }
        //m_31B48:
        RG61l = 0;
        jump L_fin;
      }
      //m_31B4F:
      vql_2c.poke32(DM2_REMOVE_POSSESSION(RG73L, vl_1c));
      RG3W = DM2_QUERY_PLAYER_SKILL_LV(vql_04.peek16(), 11, RG61l);
      RG73L = unsignedlong(vql_2c.peek16());
      RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG73L, lcon(0xc)) & lcon(0xffff);
      put16(parw12, RG1W);
      RG2L = unsignedlong(vql_58.peek16());
      RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG2L, lcon(0xa));
      RG4L = 2 * RG3L;
      RG1L += RG4L;
      RG1L = unsignedlong(RG1W);
      vl_18 = RG1L;
      RG2L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG2L, lcon(0x9));
      RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG73L, lcon(0x9)) + RG2L;
      RG3L += RG1L;
      RG2L = unsignedlong(RG3W);
      DM2_SHOOT_CHAMPION_MISSILE(hero, RG73L, RG2L, vl_18, parw12);
      vl_14 = RG61l;
      jump L_fin;

    case 32:
    case 33:
    case 34:
      //m_31BE5:
      RG1L = unsignedlong(vw_54);
      if (RG1L != lcon(0x21))
      {
        //m_31BF8:
        if (RG1L != lcon(0x22))
          //m_31C01:
          RG3L = 2;
        else
          RG3L = 0;
      }
      else
        RG3L = 1;
      //m_31C06:
      RG1L = signedlong(DM2_MAX(32, vw_60)) * lcon(0x3);
      vw_60 = RG1W;
      RG2L = unsignedlong(RG1W);
      RG4L = unsignedlong(RG3Blo);
      RG3L = 1;
      skip00719 = true;
      break;

    case 35:
      //m_31CB3:
      RG3W = hero->maxHP - hero->curHP;
      if (RG3W <= 0 || hero->curMP == 0)
        jump L_fin;
      RG1W = DM2_QUERY_PLAYER_SKILL_LV(vql_04.peek16(), 13, RG61l);
      RG4L = signedlong(RG1W);
      RG2L = signedlong(DM2_MIN(10, RG4W));
      vo_68.poke16(2);
      //m_31CF8:
      for (;;)
      {
        RG1L = signedlong(DM2_MIN(RG3W, RG2W));
        RG63w = RG1W;
        hero->curHP += RG63w;
        vo_68.add16(2);
        RG1W = hero->curMP;
        RG1L -= 2;
        hero->curMP = RG1W;
        if (RG1W > 0)
        {
          RG3W -= RG63w;
          if (RG3W != 0)
            continue;
        }
        break;
      }
      //m_31D25:
      if (hero->curMP < 0)
        hero->curMP = 0;
      hero->heroflag |= 0x800;
      RG61l = 1;
      jump L_fin;

    case 41:
      //m_31E89:
      RG1L = signedlong(ddat.v1e0258);
      RG2L = RG1L + RG61l & lcon(0x3);
      RG4L = unsignedlong(hero->partypos);
      if (RG4L != RG2L)
      {
        RG1L += 2;
        RG1L &= 0x3;
        if (RG4L != RG1L)
        {
          //m_31EB1:
          RG1L = 0;
          RG2L = unsignedlong(RG1W);
          RG61l = DM2_hero_2c1d_1de2((e_hero)vql_04.peek16(), signedlong(vw_34), RG2L);
          jump L_fin;
        }
      }
      skip00713 = true;
      break;

    case 43:
      //m_31ED2:
      RG1L = DM2_SET_DESTINATION_OF_MINION_MAP(unsignedlong(vql_58.peek16()), signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), signedlong(ddat.v1e0266));
      RG61l = RG1L;
      if (RG1L != 0)
        jump L_fin;
      RG1L = 0;
      vo_68.poke16(RG1W);
      vol_20.poke32(lcon(0x1));
      jump L_fin;

    case 44:
    case 45:
      //m_31F21:
      RG4L = unsignedlong(vql_58.peek16());
      RG1P = DM2_GET_MISSILE_REF_OF_MINION(unsignedlong(word_at(RG71p, 2)), RG4L);
      if (RG1P != NULL)
      {
        RG4Blo = (unsignedlong(vw_54) != (ui32) lcon(0x2d) ? 1 : 0) + lcon(0x32);
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(word_at(RG71p, 2))));
        mov8(location(RG1P + lcon(0x4)), RG4Blo);
        DM2_1c9a_0247(unsignedlong(word_at(RG71p, 2)));
        jump L_fin;
      }
      //m_31F65:
      RG1W = vw_54;
      if (RG1L != lcon(0x2e))
      {
        //m_31FA9:
        vb_70 = lcon(0x32);
        skip00714 = true;
      }
      else
      {
        vb_70 = lcon(0x33);
        RG2W = ddat.v1e0258;
        RG1W = word_at(RG71p, lcon(0x6)) & lcon(0x1f);
        vw_5c = RG1W;
        RG1W = word_at(RG71p, lcon(0x6));
        RG1L <<= bcon(0x6);
        _RG1.ushiftr11();
        vw_24 = RG1W;
        vw_44 = -1;
        RG3W = word_at(RG71p, lcon(0x6));
        RG3UW >>= bcon(0xa);
        skip00722 = true;
      }
      break;

    case 46:
      //m_31F17:
      vb_70 = lcon(0x30);
      skip00714 = true;
      break;

    case 47:
      //m_320BD:
      DM2_RELEASE_MINION(unsignedlong(word_at(RG71p, 2)));
      jump L_fin;

    case 48:
      //m_320DB:
      RG1L = lcon(0x31);
      skip00715 = true;
      break;

    case 49:
      //m_320CD:
      RG1L = lcon(0x34);
      skip00715 = true;
      break;

    case 50:
      //m_320D4:
      RG1L = lcon(0x35);
      skip00715 = true;
      break;

    case 53:
      //m_32164:
      RG1L = DM2_LOAD_GDAT_INTERFACE_00_0A();
      if (RG1L != 0)
        jump L_fin;
      RG1L = DM2_UPDATE_GLOB_VAR(lcon(0x47), RG61l, lcon(0x3));
      RG4L = signedlong(CUTX16(lcon(0x8) - RG1L));
      RG1L = signedlong(DM2_MAX(CUTX16(RG61l), RG4W));
      ddat.v1e0b6c = RG1W;
      ddat.v1e0b7c = lcon(0xfffffffa);
      jump L_fin;

    default: throw(THROW_DMABORT);
  }

  if (skip00715)
  {
    //m_320E0:
    RG4L = signedlong(CUTX8(ddat.v1e0258));
    put8(parb22, RG4Blo);
    i32 parl21 = unsignedlong(vql_58.peek16());
    RG4L = signedlong(ddat.v1e0266);
    put16(parw20, RG4W);
    RG4L = signedlong(ddat.v1e0272);
    i32 parl19 = RG4L;
    RG3L = signedlong(ddat.v1e0270);
    RG4W = ddat.v1e0258;
    RG4L += 2;
    RG4W &= 0x3;
    RG2L = unsignedlong(RG4W);
    RG4L = unsignedlong(vw_60);
    RG4L <<= bcon(0x10);
    RG4L >>= bcon(0x13);
    vl_18 = RG4L;
    RG4L = vl_18 & lcon(0xffff);
    RG1W = DM2_CREATE_MINION(unsignedlong(RG1W), RG4L, RG2L, RG3L, parl19, parw20, parl21, parb22);
    if (RG1W != lcon(0xffffffff))
      jump L_fin;
    RG3L = unsignedlong(ddat.v1e0272);
    RG2L = unsignedlong(ddat.v1e0270);
    skip00723 = true;
  }

  if (skip00714)
  {
    //m_31FAE:
    vw_5c = ddat.v1e0270;
    vw_24 = ddat.v1e0272;
    RG2W = ddat.v1e0258;
    RG2L += 2;
    RG2W &= 0x3;
    RG3W = ddat.v1e0266;
    skip00722 = true;
  }

  if (skip00713)
  {
    //m_31EAA:
    RG1L = 1;
    RG2L = unsignedlong(RG1W);
    RG61l = DM2_hero_2c1d_1de2((e_hero)vql_04.peek16(), signedlong(vw_34), RG2L);
    jump L_fin;
  }

  if (skip00712)
  {
    //m_31E33:
    RG1L = signedlong(DM2_MAX(32, vw_60)) << 2;
    vw_60 = RG1W;
    RG2L = unsignedlong(RG1W);
    RG4L = unsignedlong(RG3Blo);
    RG3L = 0;
    skip00719 = true;
  }

  if (skip00711)
  {
    //m_319BF:
    RG1L = DM2_QUERY_CUR_CMDSTR_ENTRY(lcon(0x6));
    RG4L = signedlong(vw_64);
    put16(parw07, RG4W);
    RG4L = unsignedlong(vw_50);
    put16(parw06, RG4W);
    RG3L = unsignedlong(vw_24);
    RG2L = unsignedlong(vw_5c);
    RG4L = signedlong(RG1W);
    RG1L = DM2_WIELD_WEAPON(vql_04.peek16(), RG4L, RG2L, RG3L, parw06, parw07) ? 1 : 0;
    RG61l = RG1L;
    if (RG1L != 0)
    {
      //m_31A06:
      if (ddat.v1e0b4c == lcon(0xffffffff))
        jump L_fin;
      skip00717 = true;
    }
    else
    {
      vo_68.sar16(lcon(0x1));
      skip00720 = true;
    }
  }

  if (skip00717)
  {
    //m_31A14:
    RG1L = signedlong(CUTX16(unsignedlong(vw_3c) + 1));
    put16(parw11, RG1W);
    put16(parw10, vw_24);
    put16(parw09, vw_5c);
    DM2_QUEUE_NOISE_GEN1(lcon(0xf), vb_70, lcon(0x8d), lcon(0x3c), lcon(0xc8), parw09, parw10, parw11);
    jump L_fin;
  }

  if (skip00719)
  {
    //m_31C2D:
    RG1L = DM2_hero_2c1d_0186(hero, RG4L, RG2L, RG3L) ? 1 : 0;
    if (RG1L != 0)
      jump L_fin;
    vo_68.sar16(2);
    skip00720 = true;
  }

  if (skip00720)
  {
    //m_31C42:
    vol_20.shr16(lcon(0x1));
    jump L_fin;
  }

  if (skip00722)
  {
    //m_31FDA:
    RG1L = signedlong(CUTX8(vw_44));
    put8(parb17, RG1Blo);
    i32 parl16 = unsignedlong(vql_58.peek16());
    RG1L = signedlong(RG3W);
    put16(parw15, RG1W);
    i32 parl14 = signedlong(vw_24);
    RG3L = signedlong(vw_5c);
    RG64L = unsignedlong(RG2W);
    RG4L = unsignedlong(vw_50);
    RG1W = DM2_QUERY_PLAYER_SKILL_LV(vql_04.peek16(), RG4W, 1);
    RG4L = unsignedlong(RG1W);
    RG1W = DM2_CREATE_MINION(unsignedlong(vb_70), RG4L, RG64L, RG3L, parl14, parw15, parl16, parb17);
    vw_30 = RG1W;
    RG1Blo = RG1W != wcon(0xffff) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    RG1Blo = RG1L != 0 ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    flag = RG1L == 0;
    RG61l = RG1L;
    if (!flag)
    {
      RG1L = unsignedlong(vw_54);
      if (RG1L == lcon(0x2f))
        DM2_SET_DESTINATION_OF_MINION_MAP(unsignedlong(vql_58.peek16()), signedlong(ddat.v1e102a), signedlong(ddat.v1e1028), signedlong(ddat.v1e102c));
      RG1W = vw_30;
      mov16(location(RG71p + 2), RG1W);
      jump L_fin;
    }
    //m_3208B:
    vo_68.sar16(2);
    vol_20.shr16(lcon(0x1));
    RG3L = unsignedlong(vw_24);
    RG2L = unsignedlong(vw_5c);
    skip00723 = true;
  }

  if (skip00723)
  {
    //m_320A9:
    RG4L = lcon(0x6e);
    RG1L = lcon(0xffa8);
  }

  //m_320B3:
  DM2_CREATE_CLOUD(RG1L, RG4L, RG2L, RG3L, 0xff);

L_fin:
  RG4L = vl_14;
  if (RG4L == 0)
  {
    put16(parw25, signedlong(vw_3c));
    RG1L = signedlong(ddat.v1e0272);
    put16(parw24, RG1W);
    RG1L = signedlong(ddat.v1e0270);
    put16(parw23, RG1W);
    RG2L = unsignedlong(vb_6c);
    RG4Blo = ddat.v1e0b80;
    DM2_QUEUE_NOISE_GEN2(ddat.v1e0b7f, RG4Blo, RG2Blo, lcon(0xfe), parw23, parw24, parw25, lcon(0x73), lcon(0xc8));
  }
  if (hero->curHP == 0)
    return RG61l;
  if (vw_48 != 0)
    return RG61l;
  RG1L = vol_20.peek32();
  if (RG1W != 0)
  {
    RG2L = signedlong(vw_34);
    RG4L = unsignedlong(RG1W);
    DM2_ADJUST_HAND_COOLDOWN(unsignedlong(vql_04.peek16()), RG4L, RG2L);
  }
  if (vw_40 != 0)
    DM2_ADJUST_STAMINA(vql_04.peek16(), vw_40);
  if (vo_68.peek16() != 0)
    DM2_ADJUST_SKILLS(vql_04.peek16(), unsignedlong(vw_50), signedlong(vo_68.peek16()));
  if (RG61l != 0)
  {
    RG4P = &ddat.v1e0adc[16 * unsignedlong(vql_04.peek16())];
    RG1L = unsignedlong(vw_34) << 3;
    RG4P += RG1L;
    RG1L = DM2_QUERY_CUR_CMDSTR_ENTRY(lcon(0xe));
    mov16(location(RG4P + 2), RG1W);
    if (RG1W != 0)
    {
      mov16(location(RG4P), RG1W);
      mov8(location(RG4P + lcon(0x4)), lcon(0x1));
      RG1L = vql_58.peek32();
      mov16(location(RG4P + lcon(0x6)), RG1W);
      RG1Blo = vol_00.peek8();
      mov8(location(RG4P + lcon(0x5)), RG1Blo);
    }
  }
  return RG61l;
}
