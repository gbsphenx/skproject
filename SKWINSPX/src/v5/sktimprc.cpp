#include <dm2data.h>
#include <sktimer.h>
#include <skmap.h>
#include <skgdtqdb.h>
#include <SK0350.h>
#include <bitem.h>
#include <gfxstr.h>
#include <startend.h>
#include <util.h>
#include <skalocdb.h>
#include <bcreatur.h>
#include <sklight.h>
#include <sfxsnd.h>
#include <skhero.h>
#include <skcloud.h>
#include <skmovrec.h>
#include <skrandom.h>
#include <uiinput.h>
#include <skweathr.h>
#include <skai.h>
#include <SK1C9A.h>
#include <skmove.h>
#include <sktimprc.h>

// TODO: optimize temporaries
#include <regs.h>
#include <emu.h>

// was SKW_3a15_303f
static void DM2_PROCESS_TIMER_0C(i16 eaxw)
{
  c_hero* hero = &party.hero[eaxw];
  hero->timeridx = -1;
  if (hero->curHP != 0)
    hero->heroflag |= 0x800;
}

// was SKW_3a15_306c
static void DM2_PROCESS_TIMER_RESURRECTION(c_tim* eaxtimp)
{
  c_nreg _RG1;
  c_nreg _RG3;
  c_nreg _RG2;
  c_nreg _RG4;
  i32 RG5l;
  c_tim* RG6timp = eaxtimp;
  i16 vw_00;
  i16 vw_04;
  c_o32 vo_08; // poke 16 peek 8 dec16
  i16 vw_0c;

  RG1Bhi = 0;
  RG1Blo = RG6timp->getxA();
  vw_00 = RG1W;
  RG1Blo = RG6timp->getyA();
  vw_04 = RG1W;
  RG3W = unsignedword(RG3Blo);
  RG3Blo = RG6timp->getxB();
  RG1Blo = RG6timp->getactor();
  vw_0c = RG1W; // truncated
  RG1Blo = RG6timp->getyB();
  vo_08.poke16(RG1W);
  if (RG1UW < mkuw(1))
  {
    //m_49143:
    if (RG1W != 0)
      return;
    //m_4921A:
    DM2_BRING_CHAMPION_TO_LIFE(vw_0c);
    return;
  }
  if (RG1UW <= mkuw(1))
  {
    //m_4918C:
    RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(vw_00, vw_04));
    //m_4919F:
    for (;;)
    {
      RG2L = RG1L;
      if (RG2W == lcon(0xfffffffe))
        return;
      RG5l = unsignedlong(RG2W);
      RG1L = RG5l >> bcon(0xe);
      RG4L = unsignedlong(RG3W);
      if (RG1L == RG4L)
      {
        RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG5l);
        if (RG1Blo == lcon(0x15))
        {
          RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG5l);
          if (RG1Blo == 0)
          {
            RG4L = 0;
            RG1W = CUTX16(DM2_ADD_ITEM_CHARGE(RG5l, RG4L));
            RG4W = vw_0c;
            if (RG1W == RG4W)
            {
              DM2_CUT_RECORD_FROM(mk_record(RG5l), NULL, vw_00, vw_04);
              DM2_DEALLOC_RECORD(RG5l);
              RG6timp->incdata();
              break;
            }
          }
        }
      }
      //m_4920E:
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG2UW)));
    }
  }
  else
  {
    if (RG1W != 2)
      return;
    //m_49151:
    RG1L = unsignedlong(RG3W);
    i32 parl00 = RG1L;
    RG1L = DM2_CREATE_CLOUD(lcon(0xffe4), 0, unsignedlong(vw_00), unsignedlong(vw_04), parl00);
    RG6timp->adddata(5);
  }
  //m_49174:
  vo_08.dec16();
  RG1Blo = vo_08.peek8();
  RG6timp->setyB(RG1Blo);
  DM2_QUEUE_TIMER(RG6timp);
}

static void DM2_STEP_DOOR(c_tim* eaxtimp)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_tim* RG5timp = eaxtimp;
  i16 RG6w;
  c_lreg _RG7;
  i8* ebppb;
  i32 vl_00;
  unk* xp_04;
  i32 vl_08;
  i32 vl_0c;
  i32 vl_10;
  i16 vw_14;
  c_ql32 vql_18;
  i16 vw_1c;
  i16 vw_20;
  c_o32 vo_24; // poke 16 peek 16 32 sar16
  i16 vw_28;
  c_o32 vo_2c; // poke 16 peek 8 32 inc16
  i16 parw00;
  i16 parw01;
  i16 parw07;
  i16 parw08;
  i16 parw12;
  i16 parw13;
  i16 parw17;
  i16 parw18;
  bool skip00983 = false;

  RG4L = 0;
  vl_00 = RG4L;
  vl_10 = RG4L;
  RG6w = unsignedword(RG5timp->getxA());
  RG1P = DOWNCAST(t_tile*, &mapdat.map[unsignedlong(RG5timp->getxA())]);
  RG4W = unsignedword(RG4Blo);
  RG4Blo = RG5timp->getyA();
  vw_28 = RG4W;
  RG7L = unsignedlong(RG4Blo);
  RG1P = pointer_at(RG1P);
  ebppb = RG1P + RG7L;
  RG1Blo = *ebppb & lcon(0x7);
  RG4Blo = RG1Blo;
  vw_20 = RG4W;
  RG1L = unsignedlong(RG1Blo);
  if (RG1L == lcon(0x5))
    return;
  RG1W = ddat.v1d3248;
  if (RG1W == ddat.v1e0266)
    ddat.v1e0390.l_00 = 3;
  RG4L = signedlong(vw_28);
  RG1RP = DM2_GET_ADDRESS_OF_TILE_RECORD(RG6w, RG4W);
  xp_04 = UPCAST(unk, RG1P);
  if (jz_test8(location(RG1P + lcon(0x3)), lcon(0x4)))
    return;
  RG5timp->incdata();
  RG4Bhi = byte_at(RG1P, lcon(0x3));
  if ((RG4Bhi & bcon(0x2)) != 0)
  {
    //m_468CE:
    if (vw_20 == 0)
    {
      RG2Blo = RG4Bhi & lcon(0xfffffffb);
      mov8(location(RG1P + lcon(0x3)), RG2Blo);
      return;
    }
    skip00983 = true;
  }
  else
  {
    RG4L = signedlong(vw_20);
    if (RG4L != lcon(0x4))
    {
      RG4W = word_at(RG1P, 2);
      RG4L <<= bcon(0xa);
      RG4UW >>= bcon(0xf);
      vql_18.poke32(RG4L);
      RG1L = DM2_QUERY_DOOR_DAMAGE_RESIST(DM2_IS_REBIRTH_ALTAR(RG1P) & lcon(0xff));
      vo_24.poke16(RG1W);
      RG1W = ddat.v1d3248;
      if (RG1W == ddat.v1e0282)
      {
        RG4L = unsignedlong(RG6w);
        RG1L = signedlong(ddat.v1e0280);
        if (RG4L == RG1L)
        {
          RG4L ^= RG1L;
          RG4W = vw_28;
          RG1L = signedlong(ddat.v1e027e);
          if (RG4L == RG1L && vw_20 != 0)
          {
            vl_10 = 1;
            if (mkuw(party.heros_in_party) > mkuw(0))
            {
              *ebppb &= lcon(0xf8);
              RG2L = 2;
              RG1L = vql_18.peek32();
              RG1Blo |= lcon(0x8);
              RG1L = unsignedlong(RG1W);
              RG1Blo = RG1L != 0 ? 1 : 0;
              RG1L = unsignedlong(RG1Blo) + lcon(0x3);
              RG4L = unsignedlong(RG1W);
              RG1L = DM2_ATTACK_PARTY(unsignedlong(vo_24.peek16()), RG4L, RG2L);
              vw_1c = RG1W; // truncated
              if (RG1W != 0)
              {
                RG2L = 0;
                vo_2c.poke16(RG2W);
                //m_46742:
                for (;;)
                {
                  RG4L = signedlong(vo_2c.peek16());
                  if (RG4L >= lcon(0x4))
                    break;
                  //m_466E5:
                  RG3L = signedlong(vo_2c.peek16());
                  RG1L = 1 << RG3Blo;
                  RG3L = RG1L;
                  RG1L = unsignedlong(vw_1c);
                  if ((RG3L & RG1L) != 0)
                  {
                    RG1L = signedlong(vw_28);
                    put16(parw01, RG1W);
                    RG1L = signedlong(RG6w);
                    put16(parw00, RG1W);
                    RG1L = 263 * RG4L;
                    RG4L = unsignedlong(party.hero[RG4L].herotype);
                    RG3L = lcon(0xfe);
                    RG2L = lcon(0x8a);
                    DM2_QUEUE_NOISE_GEN2(lcon(0x16), RG4Blo, RG2Blo, RG3Blo, parw00, parw01, 1, lcon(0x64), lcon(0xc8));
                  }
                  vo_2c.inc16();
                }
              }
            }
          }
        }
      }
      //m_4674E:
      RG4L = unsignedlong(vw_28);
      RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG6w), RG4L);
      vw_14 = RG1W; // truncated
      RG1Blo = RG1W != wcon(0xffff) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L != 0)
      {
        RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(vw_14));
        if ((RG1Blo & bcon(0x20)) == 0)
        {
          if (vql_18.peek16() == 0)
            //m_467A3:
            RG1L = 1;
          else
          {
            RG1L = unsignedlong(RG1W) >> 6;
            RG1L &= 0x3;
            RG1L = unsignedlong(RG1W);
          }
          //m_467A8:
          RG4L = signedlong(vw_20);
          if (RG4L >= RG1L)
          {
            RG1P = DM2_QUERY_CREATURE_AI_SPEC_FROM_TYPE(unsignedlong(vw_14));
            if (jnz_test8(location(RG1P + lcon(0x19)), lcon(0x10)))
            {
              vo_24.sar16(1);
              RG4L = signedlong(vo_24.peek16());
              RG1L = signedlong(DM2_MAX(1, RG4W));
              vo_24.poke16(RG1W);
            }
            //m_467E4:
            i32 parl06 = unsignedlong(vo_24.peek16());
            RG2L = signedlong(vw_28);
            RG4L = signedlong(RG6w);
            DM2_ATTACK_CREATURE(unsignedlong(vw_14), RG4L, RG2L, lcon(0x2006), lcon(0x64), parl06);
            if (vw_20 != 0)
              //m_46815:
              RG1L = signedlong(vw_20) - 1;
            else
              RG1L = 0;
            //m_4681D:
            vw_20 = RG1W;
            RG1Blo = *ebppb & lcon(0xfffffff8);
            RG1Bhi = CUTX8(vw_20) | RG1Blo;
            *ebppb = RG1Bhi;
            RG1L = signedlong(vw_28);
            vl_08 = RG1L;
            put16(parw08, RG1W);
            RG1L = signedlong(RG6w);
            vl_0c = RG1L;
            put16(parw07, RG1W);
            RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(unsignedlong(vw_14));
            RG4L = unsignedlong(RG1Blo);
            DM2_QUEUE_NOISE_GEN2(lcon(0xf), RG4Blo, lcon(0x82), lcon(0xfe), parw07, parw08, 1, lcon(0x46), lcon(0x80));
            RG4L = vl_08;
            put16(parw13, RG4W);
            RG2L = vl_0c;
            put16(parw12, RG2W);
            RG3L = lcon(0xfe);
            DM2_QUEUE_NOISE_GEN2(lcon(0x15), RG3Blo, lcon(0x85), RG3Blo, parw12, parw13, 1, lcon(0x46), lcon(0x80));
            vl_10 = 1;
          }
        }
      }
      //m_468A2:
      if (vl_10 != 0)
      {
        RG4L = 1;
        vl_00 = RG4L;
        RG5timp->incdata();
      }
      skip00983 = true;
    }
  }

  if (skip00983)
  {
    //m_468B3:
    if (vl_00 == 0)
    {
      bool skip00984 = false;
      RG1P = DOWNCAST(unk, xp_04);
      if (jz_test8(location(RG1P + lcon(0x3)), 2))
        //m_468E3:
        RG4L = 1;
      else
        RG4L = lcon(0xffffffff);
      //m_468E8:
      RG1L = signedlong(vw_20) + RG4L;
      vw_20 = RG1W;
      RG1Blo = *ebppb & lcon(0xfffffff8);
      RG1Bhi = CUTX8(vw_20) | RG1Blo;
      *ebppb = RG1Bhi;
      RG1P = DOWNCAST(unk, xp_04);
      if (jz_test8(location(RG1P + lcon(0x3)), 2))
      {
        //m_46918:
        RG1L = signedlong(vw_20);
        if (RG1L != lcon(0x4))
          skip00984 = true;
      }
      else
      {
        if (vw_20 != 0)
          skip00984 = true;
      }

      if (skip00984)
        //m_46924:
        vl_00 = 1;
      bool skip00987 = false;
      //m_4692B:
      if (vl_00 == 0)
      {
        RG1L = signedlong(vw_20);
        if (RG1L == lcon(0x4))
        {
          RG1Blo = lcon(0xffffff8f);
          skip00987 = true;
        }
      }

      if (!skip00987)
        //m_46941:
        RG1Blo = lcon(0xffffff8e);

      //m_46943:
      RG1W = unsignedword(RG1Blo);
      vo_2c.poke16(RG1W);
      RG1L = signedlong(vw_28);
      put16(parw18, RG1W);
      RG1L = signedlong(RG6w);
      put16(parw17, RG1W);
      RG2L = unsignedlong(vo_2c.peek8());
      RG1L = DM2_IS_REBIRTH_ALTAR(DOWNCAST(unk, xp_04));
      RG4L = unsignedlong(RG1Blo);
      DM2_QUEUE_NOISE_GEN2(lcon(0xe), RG4Blo, RG2Blo, lcon(0xfe), parw17, parw18, 1, lcon(0x5f), lcon(0x80));
    }
    //m_46981:
    if (vl_00 != 0)
    {
      or8(location(DOWNCAST(unk, xp_04) + lcon(0x3)), lcon(0x4));
      DM2_QUEUE_TIMER(RG5timp);
      return;
    }
    //m_46998:
    RG1P = DOWNCAST(unk, xp_04);
  }
  //m_4699C:
  and8(location(RG1P + lcon(0x3)), lcon(0xfffffffb));
}

// was SKW_3a15_0d10
static i32 DM2_PROCESS_TIMER_DESTROY_DOOR(c_tim* eaxtimp)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;

  RG4L = unsignedlong(eaxtimp->getxA());
  RG2P = DOWNCAST(t_tile*, mapdat.map);
  RG1Blo = eaxtimp->getyA();
  RG1L = unsignedlong(RG1Blo);
  RG4P = pointer_at(RG2P + 4 * RG4L);
  RG1P = RG4P + RG1L;
  RG4Blo = byte_at(RG1P) & bcon(0xf8) | bcon(0x5);
  mov8(location(RG1P), RG4Blo);
  RG1W = ddat.v1d3248;
  if (RG1W == ddat.v1e0266)
    ddat.v1e0390.l_00 = 3;
  return RG1L;
}

static void DM2_STEP_MISSILE(c_tim* eaxtimp)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_tim* RG51timp;
  i32 RG52l;
  c_wlreg _RG61;
  c_wlreg _RG62;
  i16 RG63w;
  c_wlreg _RG64;
  c_wlreg _RG65;
  c_wlreg _RG66;
  i8* RG71p;
  c_wlreg _RG72;
  c_tim c38_00;
  i32 vl_0c;
  i32 vl_10;
  i32 vl_14;
  i32 vl_18;
  i16 vw_1c;
  i16 vw_20;
  c_ql32 vql_24;
  i16 vw_28;
  i16 vw_2c;
  c_ql32 vql_30;
  c_o32 vo_34; // poke 16 peek 32 add16
  c_o32 vo_38; // poke 16 peek 16 32 add16
  i16 parw03;

  c38_00 = *eaxtimp;
  RG51timp = &c38_00;
  RG1W = c38_00.getA();
  vql_30.poke16(RG1W); // adj
  vw_28 = RG1W; // adj
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(c38_00.getA())));
  RG2P = RG1P;
  RG71p = RG1P;
  RG1L = c38_00.getBlong();
  RG1W &= lcon(0x1f);
  vo_38.poke16(RG1W);
  RG1L = c38_00.getBlong() << 6;
  _RG1.ushiftr11();
  vo_34.poke16(RG1W);
  if (!c38_00.is_type(0x1d))
  {
    bool skip00057 = false;
    //m_7B8D:
    RG1L = unsignedlong(vql_30.peek16()) >> bcon(0xe);
    RG66L = RG1L;
    RG4L = unsignedlong(vo_34.peek16());
    RG1L = DM2_GET_CREATURE_AT(unsignedlong(vo_38.peek16()), RG4L);
    if (RG1W != lcon(0xffffffff))
    {
      RG4L = unsignedlong(RG1W);
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
      RG3P = RG1P;
      RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(RG4L);
      if ((RG1Blo & bcon(0x2)) != 0 && mkuw(word_at(RG2P + 2)) >= mkuw(lcon(0xffffff80)))
      {
        RG1W = word_at(RG3P, lcon(0xe));
        RG1UW = lrshift6e(RG1UW);
        RG1W &= 0x1;
        RG1L = unsignedlong(RG1W) << 4;
        RG4L = 4 * signedlong(RG66W) + RG1L;
        RG1L = c38_00.getBlong() << 4;
        RG1UW >>= 14;
        RG1L = unsignedlong(RG1W) + RG4L;
        RG4W = signedword(table1d26a8[RG1L]);
        RG1L = signedlong(table1d26a8[RG1L]);
        if (RG1L < lcon(0x4))
        {
          RG2W = word_at(RG2P, lcon(0x6));
          RG4W &= 0x3;
          c38_00.mask_wvalueB(0xf3ff, RG4W << 10);
          timdat.timerarray[RG2W].mask_wvalueB(0xf3ff, RG4W << 10);
        }
        skip00057 = true;
      }
    }

    if (!skip00057)
    {
      //m_7C61:
      RG1W = ddat.v1d3248;
      if (RG1W == ddat.v1e0282)
      {
        RG1W = vo_38.peek16();
        if (RG1W == ddat.v1e0280)
        {
          RG1W = vo_34.peek16();
          if (RG1W == ddat.v1e027e)
          {
            i32 parl00 = unsignedlong(vql_30.peek16());
            RG3L = signedlong(RG66W);
            RG1L = DM2_move_075f_0af9(lcon(0xfffffffd), signedlong(vo_38.peek16()), signedlong(vo_34.peek16()), RG3L, parl00);
            if (RG1L != 0)
              return;
          }
        }
      }
      i32 parl01 = unsignedlong(vw_28);
      RG3L = signedlong(RG66W);
      RG2L = signedlong(vo_34.peek16());
      RG1L = DM2_move_075f_0af9(lcon(0xffffffff), signedlong(vo_38.peek16()), RG2L, RG3L, parl01);
      if (RG1L != 0)
        return;
    }

    //m_7CE0:
    RG2W = RG51timp->getB();
    RG2UW >>= bcon(0xc);
    RG1L = unsignedlong(RG2W);
    RG4L = unsignedlong(byte_at(RG71p, lcon(0x4)));
    if (RG4L <= RG1L)
    {
      RG72L = signedlong(vo_34.peek16());
      RG52l = signedlong(vo_38.peek16());
      RG65L = unsignedlong(vw_28);
      DM2_CUT_RECORD_FROM(mk_record(RG65L), NULL, CUTX16(RG52l), RG72W);
      DM2_DELETE_MISSILE_RECORD(RG65L, NULL, RG52l, RG72L);
      return;
    }
    //m_7D2A:
    RG4Blo -= RG2Blo;
    mov8(location(RG71p + lcon(0x4)), RG4Blo);
    RG4L = unsignedlong(byte_at(RG71p, lcon(0x5)));
    if (RG4L >= RG1L)
    {
      //m_7D3E:
      RG4Bhi = RG4Blo - RG2Blo;
      mov8(location(RG71p + lcon(0x5)), RG4Bhi);
    }
    else
      mov8(location(RG71p + lcon(0x5)), 0);
  }
  else
    c38_00.settype(0x1e);
  bool skip00059 = false;
  //m_7D45:
  RG1W = RG51timp->getA();
  vql_30.poke32(RG1L);
  RG1L = unsignedlong(vql_30.peek16()) >> bcon(0xe);
  RG61L = RG1L;
  RG4L = signedlong(RG1W);
  RG1W = RG51timp->getB();
  RG1L <<= 4;
  RG1UW >>= 14;
  vql_24.poke32(RG1L);
  RG1L = unsignedlong(vql_24.peek16());
  if (RG1L == RG4L)
    skip00059 = true;
  else
  {
    RG1L++;
    RG1L &= 0x3;
    if (RG1L != RG4L)
      //m_7D85:
      RG1L = 0;
    else
      skip00059 = true;
  }

  if (skip00059)
    //m_7D7E:
    RG1L = 1;
  //m_7D87:
  vw_20 = RG1W; // truncated
  if (RG1W != 0)
  {
    bool skip00062 = false;
    RG1W = vo_38.peek16();
    vw_1c = RG1W;
    RG1W = vo_34.peek16();
    vw_2c = RG1W;
    RG1L = unsignedlong(vql_24.peek16());
    RG4W = table1d27fc[RG1L];
    vo_38.add16(RG4W);
    RG1W = table1d2804[RG1L];
    vo_34.add16(RG1W);
    RG1L = dm2_GET_TILE_VALUE(signedlong(vo_38.peek16()), signedlong(vo_34.peek16()));
    RG2W = unsignedword(RG2Blo);
    RG2Blo = RG1Blo;
    RG1L = unsignedlong(RG1Blo) >> 5;
    if (RG1Blo == 0)
      skip00062 = true;
    else
    {
      bool skip00061 = false;
      RG1L = unsignedlong(RG2W) >> 5 & lcon(0xff);
      if (RG1L != lcon(0x6))
        skip00061 = true;
      else
      {
        RG1L = RG2L;
        RG1Blo &= 0x5;
        RG1L = unsignedlong(RG1W);
        if (RG1L != 0)
          skip00061 = true;
      }

      if (skip00061)
      {
        //m_7E07:
        RG1L = unsignedlong(RG2W) >> 5 & lcon(0xff);
        if (RG1L == lcon(0x3))
        {
          RG4L = signedlong(vw_2c);
          RG1Blo = mapdat.map[vw_1c][RG4L];
          RG1L = unsignedlong(RG1Blo) >> 5;
          RG1L = unsignedlong(RG1Blo);
          if (RG1L == lcon(0x3))
            skip00062 = true;
        }
      }
      else
        skip00062 = true;
    }

    if (skip00062)
    {
      //m_7E49:
      i32 parl02 = unsignedlong(vql_30.peek16());
      RG3L = signedlong(RG61W);
      RG1L = signedlong(vw_2c);
      vl_18 = signedlong(vw_1c);
      RG2L = unsignedlong(RG2W) >> 5;
      RG4L = unsignedlong(RG2Blo);
      vl_10 = RG4L;
      RG2L = RG1L;
      RG1L = DM2_move_075f_0af9(vl_10, vl_18, RG2L, RG3L, parl02);
      if (RG1L != 0)
        return;
    }

    //m_7E8E:
    RG1L = DM2_GET_WALL_TILE_ANYITEM_RECORD(signedlong(vo_38.peek16()), vo_34.peek16());
    //m_7EA1:
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
          c_tim* RG11timp = &timdat.timerarray[word_at(RG71p, lcon(0x6))];
          RG4W = RG11timp->getB();
          RG4L <<= 4;
          RG4UW >>= 14;
          RG4L = unsignedlong(RG4W) + 2;
          RG4L &= 0x3;
          RG4W &= 0x3;
          RG51timp->mask_wvalueB(0xf3ff, RG4W << 10);
          RG11timp->mask_wvalueB(0xf3ff, RG4W << 10);
          RG1L = unsignedlong(vw_1c);
          vo_38.poke16(RG1W);
          RG1L = unsignedlong(vw_2c);
          vo_34.poke16(RG1W);
          RG1L = 0;
          vw_20 = RG1W; // truncated
          break;
        }
      }
      //m_7F3E:
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
    }
  }

  //m_7F4D:
  RG2L = RG61L;
  RG1L = vql_24.peek32();
  RG1W &= 0x1;
  RG4L = unsignedlong(RG1W);
  RG1L = RG61L;
  RG1W &= 0x1;
  RG1L = signedlong(RG1W);
  if (RG4L != RG1L)
    //m_7F6A:
    RG61L++;
  else
    RG61L--;
  //m_7F6B:
  RG61L &= 0x3;
  if (vw_20 == 0)
  {
    //m_81DE:
    vl_0c = signedlong(vo_34.peek16());
    RG1L = signedlong(vo_38.peek16());
    vl_14 = RG1L;
    RG1L = (dm2_GET_TILE_VALUE(RG1L, vl_0c) & lcon(0xff)) >> 5 & lcon(0xff);
    if (RG1L == lcon(0x4))
    {
      i32 parl08 = unsignedlong(vw_28);
      RG3L = signedlong(RG2W);
      RG1L = DM2_move_075f_0af9(lcon(0x4), vl_14, vl_0c, RG3L, parl08);
      if (RG1L != 0)
        return;
    }
    vl_18 = signedlong(vo_34.peek16());
    vl_10 = signedlong(vo_38.peek16());
    DM2_CUT_RECORD_FROM(mk_record(unsignedlong(vql_30.peek16())), NULL, CUTX16(vl_10), CUTX16(vl_18));
    RG1L = vql_30.peek32();
    RG1Bhi &= lcon(0x3f);
    RG61L <<= bcon(0xe);
    RG1L |= RG61L;
    vql_30.poke32(RG1L);
    DM2_APPEND_RECORD_TO(mk_record(unsignedlong(vql_30.peek16())), NULL, CUTX16(vl_10), CUTX16(vl_18));
  }
  else
  {
    RG1L = signedlong(vo_34.peek16());
    put16(parw03, RG1W);
    RG3L = signedlong(vo_38.peek16());
    RG2L = signedlong(vw_2c);
    RG4L = signedlong(vw_1c);
    RG1L = vql_30.peek32();
    RG1Bhi &= lcon(0x3f);
    RG61L <<= bcon(0xe);
    RG61L |= RG1L;
    RG1L = unsignedlong(RG61W);
    DM2_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw03);
    RG1L = unsignedlong(word_at(RG71p, 2));
    if (RG1L == lcon(0xff80))
    {
      RG1W = ddat.v1d3248;
      if (RG1W == ddat.v1e0266)
        ddat.v1e0390.b_00 |= 1;
    }
    RG1W = ddat.v1e102a & lcon(0x1f);
    RG51timp->mask_wvalueB(0xffe0, 0);
    RG2W = RG51timp->getB();
    RG2L |= RG1L;
    RG51timp->setB(RG2W);
    RG1W = ddat.v1e1028 & lcon(0x1f);
    RG3L = RG2L & lcon(0xfffffc1f);
    RG1L <<= bcon(0x5);
    RG62L = RG3L | RG1L;
    RG51timp->setB(RG62W);
    RG1W = ddat.v1e1024 & lcon(0x3);
    RG51timp->mask_wvalueB(0xf3ff, RG1W << 10);
    RG4L = vql_30.peek32();
    RG4Bhi &= lcon(0x3f);
    RG1W = ddat.v1e1026;
    RG1L <<= bcon(0xe);
    RG4L |= RG1L;
    vql_30.poke32(RG4L);
    RG51timp->setmticks(ddat.v1e102c, RG51timp->getticks());
    vo_38.poke16(ddat.v1d3248);
    RG1L = signedlong(vo_38.peek16());
    RG4L = unsignedlong(ddat.v1e102c);
    if (RG4L != RG1L)
      DM2_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e102c));
    RG1L = unsignedlong(ddat.v1e1024);
    RG4W = table1d27fc[RG1L];
    RG2W = ddat.v1e102a;
    RG2L += RG4L;
    ddat.v1e102a = RG2W;
    RG1W = table1d2804[RG1L];
    RG3W = ddat.v1e1028;
    RG3L += RG1L;
    ddat.v1e1028 = RG3W;
    RG4L = unsignedlong(RG3W);
    RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG2W), RG4L);
    if (RG1W != lcon(0xffffffff))
    {
      RG2L = signedlong(ddat.v1e1028);
      RG4L = signedlong(ddat.v1e102a);
      RG1L = unsignedlong(RG1W);
      DM2_ATTACK_CREATURE(RG1L, RG4L, RG2L, lcon(0x2006), lcon(0x64), 0);
    }
    RG4L = signedlong(ddat.v1e1028);
    RG1L = dm2_GET_TILE_VALUE(signedlong(ddat.v1e102a), RG4L);
    RG2W = unsignedword(RG2Blo);
    RG2Blo = RG1Blo;
    RG1L = unsignedlong(RG1Blo) >> 5;
    RG63w = unsignedword(RG1Blo);
    if (RG63w != 0)
    {
      bool skip00066 = false;
      RG1L = signedlong(RG63w);
      if (RG1L != lcon(0x6))
        skip00066 = true;
      else
      {
        RG1L = RG2L;
        RG1Blo &= 0x4;
        RG1L = unsignedlong(RG1W);
        if (RG1L != 0)
          skip00066 = true;
      }

      if (skip00066)
      {
        bool skip00067 = false;
        //m_812B:
        RG1L = signedlong(RG63w);
        if (RG1L != lcon(0x4))
          skip00067 = true;
        else
        {
          RG1Blo = RG2Blo & lcon(0x7);
          RG2W = unsignedword(RG2Blo);
          RG2Blo = RG1Blo;
          RG1L = unsignedlong(RG1Blo);
          if (RG1L == lcon(0x5))
            skip00067 = true;
          else
          {
            RG1L = unsignedlong(RG2W);
            if (RG1L <= 1)
              skip00067 = true;
          }
        }

        if (skip00067)
        {
          //m_814F:
          RG1L = unsignedlong(ddat.v1e1024);
          RG4W = table1d27fc[RG1L];
          RG64W = ddat.v1e102a;
          RG64UW += RG4UW;
          ddat.v1e102a = RG64W;
          RG1W = table1d2804[RG1L];
          ddat.v1e1028 += RG1W;
          RG4L = unsignedlong(ddat.v1e1028);
          RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG64W), RG4L);
          if (RG1W != lcon(0xffffffff))
          {
            RG2L = signedlong(ddat.v1e1028);
            RG4L = signedlong(ddat.v1e102a);
            RG1L = unsignedlong(RG1W);
            DM2_ATTACK_CREATURE(RG1L, RG4L, RG2L, lcon(0x2006), lcon(0x64), 0);
          }
        }
      }
    }
    //m_81BC:
    RG4L = unsignedlong(ddat.v1e102c);
    RG1L = signedlong(vo_38.peek16());
    if (RG4L != RG1L)
      DM2_CHANGE_CURRENT_MAP_TO(RG1L);
  }
  //m_8282:
  RG51timp->incdata();
  RG1L = vql_30.peek32();
  RG51timp->setA(RG1W);
  RG1W = DM2_QUEUE_TIMER(RG51timp);
  mov16(location(RG71p + lcon(0x6)), RG1W);
}

// was SKW_3a15_2feb
static void DM2_PROCESS_TIMER_3D(c_tim* eaxtimp)
{
  c_nreg _RG1;
  c_wlreg _RG6;
  c_wlreg _RG7;
  c_tim* esip_c38;

  esip_c38 = eaxtimp;
  RG7L = signedlong(unsignedword(esip_c38->getyA()));
  RG6L = signedlong(unsignedword(esip_c38->getxA()));
  RG1L = DM2_MOVE_RECORD_TO(unsignedlong(esip_c38->getB()), lcon(0xfffffffd), 0, RG6L, RG7W);
  if (RG1W != wcon(0x0) || esip_c38->is_type(0x3d))
    DM2_QUEUE_NOISE_GEN1(lcon(0x3), 0, lcon(0x89), lcon(0x61), lcon(0x80), RG6W, RG7W, 1);
}

// was SKW_3a15_316c
static void DM2_PROCESS_TIMER_LIGHT(c_tim* eaxtimp)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_tim c38_00;
  bool flag;

  RG1W = eaxtimp->getA();
  if (RG1W == 0)
    return; // RG1W;
  RG4Blo = RG1W < 0 ? 1 : 0;
  RG4L = unsignedlong(RG4Blo);
  flag = RG4L == 0;
  RG3L = RG4L;
  if (!flag)
    RG1L = -RG1L;
  RG4L = RG1L - 1;
  RG2L = signedlong(RG4W);
  RG2W = signedword(table1d6702[RG2L]);
  RG1L = signedlong(RG1W);
  RG1W = signedword(table1d6702[RG1L]);
  RG1L -= RG2L;
  if (RG3L == 0)
    //m_49273:
    RG1L = 2 * RG1L;
  else
  {
    RG1L = -RG1L;
    RG4L = -RG4L;
  }
  //m_49275:
  ddat.savegames1.w_00 += RG1W;
  if (RG4W == 0)
    return; // RG1W;
  c38_00.settype(0x46);
  c38_00.setA(RG4W);
  c38_00.setmticks(ddat.v1e0266, timdat.gametick + 8);
  c38_00.setactor(0);
  DM2_QUEUE_TIMER(&c38_00);
}

static void DM2_CONTINUE_ORNATE_ANIMATOR(c_tim* eaxtimp)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;
  c_wlreg _RG51;
  c_wlreg _RG52;

  eaxtimp->incdata();
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(eaxtimp->getA())));
  RG2P = RG1P;
  RG4L = DM2_GET_ORNATE_ANIM_LEN(RG1P, eaxtimp->getBlong());
  RG1W = word_at(RG2P, 2);
  RG51L = RG1L;
  RG51UW >>= bcon(0x7);
  RG51L++;
  RG51L &= lcon(0x1ff);
  RG1L &= lcon(0xffff007f);
  mov16(location(RG2P + 2), RG1W);
  RG1L = RG51L << 7;
  or16(location(RG2P + 2), RG1W);
  RG1L = unsignedlong(RG51W);
  RG52L = signedlong(RG4W);
  RG4L = RG1L % RG52L;
  RG1L /= RG52L;
  if (RG4L == 0)
  {
    and8(location(RG2P + lcon(0x4)), lcon(0xfffffffe));
    return; // RG1W
  }
  DM2_QUEUE_TIMER(eaxtimp);
}

static void DM2_CONTINUE_TICK_GENERATOR(c_tim* eaxtimp)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;
  i8* RG5p;
  c_lreg _RG6;
  bool skip01035 = false;

  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(eaxtimp->getA())));
  RG5p = RG1P;
  RG4W = word_at(RG1P, lcon(0x4)) & lcon(0x18);
  if (RG4W != lcon(0x18))
  {
    //m_494F0:
    RG4W = word_at(RG1P, lcon(0x4));
    RG4L <<= bcon(0xd);
    RG4UW >>= bcon(0xf);
    RG6L = unsignedlong(RG4W);
    if (RG6L == 0)
      skip01035 = true;
    else
    {
      RG4W = word_at(RG1P, lcon(0x4));
      RG4L <<= 11;
      RG4UW >>= 14;
      RG4L = unsignedlong(RG4W);
      RG2L = 0;
    }
  }
  else
  {
    RG4W = eaxtimp->getB();
    RG4L <<= 7;
    RG4UW >>= 15;
    RG4Blo ^= 1;
    RG2L = RG4L;
    RG2Bhi ^= RG4Bhi;
    RG2Blo &= 0x1;
    eaxtimp->mask_wvalueB(0xfeff, RG2W << 8);
    RG6L = unsignedlong(RG2W);
    RG1W = word_at(RG1P, lcon(0x4));
    RG1L <<= bcon(0xd);
    RG1UW >>= bcon(0xf);
    RG1L = unsignedlong(RG1W);
    RG6L |= RG1L;
    RG2L = 0;
    RG1Blo = ((eaxtimp->getyB() & 1) == 0) ? 1 : 0;
    RG1W = unsignedword(RG1Blo);
    RG4L = unsignedlong(RG1W);
    RG1P = RG5p;
  }

  if (!skip01035)
    //m_49517:
    DM2_INVOKE_ACTUATOR(RG1P, RG4L, RG2L);

  //m_4951C:
  if (RG6L != 0)
  {
    RG1W = word_at(RG5p, 2);
    RG1UW >>= bcon(0x7);
    RG4L = unsignedlong(RG1W);
    RG1W = eaxtimp->getB();
    RG1L = unsignedlong(unsignedword(RG1Blo)) * RG4L;
    eaxtimp->adddata(RG1L);
    DM2_QUEUE_TIMER(eaxtimp);
    return;
  }
  //m_49549:
  and8(location(RG5p + lcon(0x4)), lcon(0xfffffffe));
}

// was SKW_3a15_374b
static void DM2_PROCESS_TIMER_RELEASE_DOOR_BUTTON(c_tim* eaxtimp)
{
  c_nreg _RG1;

  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(eaxtimp->getA())));
  and8(location(RG1P + lcon(0x3)), lcon(0xfffffff7));
}

// was SKW_3a15_3766
static void DM2_PROCESS_TIMER_59(c_tim* eaxtimp)
{
  c_nreg _RG1;
  c_nreg _RG4;

  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(eaxtimp->getB())));
  RG4P = RG1P;
  if (jnz_test8(location(RG1P + lcon(0x4)), lcon(0x4)))
    return;
  RG1W = ddat.v1d3248;
  if (RG1W == ddat.v1e0266)
    ddat.v1e0390.b_00 |= 1;
  and8(location(RG4P + lcon(0x4)), lcon(0xfffffffe));
}

static void DM2_CONTINUE_ORNATE_NOISE(c_tim* eaxtimp)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5;
  i8 vb_00;
  i8 vb_04;
  i16 parw00;
  i16 parw01;
  bool flag;

  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(eaxtimp->getB())));
  RG3P = RG1P;
  if (jnz_test8(location(RG1P + lcon(0x4)), 1))
  {
    RG1W = signedword(eaxtimp->getmap());
    RG4L = unsignedlong(RG1W);
    RG1L = signedlong(ddat.v1e0266);
    if (RG4L == RG1L)
    {
      RG4L ^= RG1L;
      RG4Blo = eaxtimp->getyA();
      RG1L = (dm2_GET_TILE_VALUE(unsignedlong(eaxtimp->getxA()), RG4L) & lcon(0xff)) >> 5;
      RG1Blo = RG1Blo == bcon(0x0) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      flag = RG1L == 0;
      RG5L = RG1L;
      if (flag)
      {
        //m_4976C:
        vb_04 = lcon(0xa);
        RG1Blo = DM2_GET_FLOOR_DECORATION_OF_ACTUATOR(RG3P);
      }
      else
      {
        vb_04 = lcon(0x9);
        RG1Blo = DM2_GET_WALL_DECORATION_OF_ACTUATOR(RG3P);
      }
      //m_49778:
      vb_00 = RG1Blo;
      RG4L = RG5L;
      RG1L = DM2_GET_ORNATE_ANIM_LEN(RG3P, RG4L) & lcon(0xffff);
      eaxtimp->adddata(RG1L);
      DM2_QUEUE_TIMER(eaxtimp);
      RG3L = unsignedlong(eaxtimp->getxA());
      RG1L = unsignedlong(eaxtimp->getyA());
      if (RG5L != 0)
      {
        RG2Blo = eaxtimp->getyB();
        RG2L = unsignedlong(RG2Blo);
        RG5W = table1d27fc[RG2L];
        RG5L += RG3L;
        RG4W = table1d2804[RG2L];
        RG4L += RG1L;
        RG2W = ddat.v1d3248;
        if (RG2W != ddat.v1e0282 || RG5W != ddat.v1e0280 || RG4W != ddat.v1e027e)
        {
          RG3L = RG5L;
          RG1L = RG4L;
        }
      }
      RG1L = signedlong(RG1W);
      put16(parw01, RG1W);
      RG1L = signedlong(RG3W);
      put16(parw00, RG1W);
      DM2_QUEUE_NOISE_GEN2(vb_04, vb_00, lcon(0x88), lcon(0xfe), parw00, parw01, 1, lcon(0x8c), lcon(0x80));
      return;
    }
  }
  //m_49812:
  RG1W = word_at(RG3P, 2);
  RG4L = RG1L;
  RG4UW >>= bcon(0x7);
  RG4W = unsignedword(RG4Blo);
  RG1L &= lcon(0xffff007f);
  RG4L <<= bcon(0x7);
  RG2L = RG1L | RG4L;
  mov16(location(RG3P + 2), RG2W);
}

static void DM2_ACTIVATE_RELAY1(c_tim* eaxtimp, unk* xedxp, i32 ebxl)
{
  c_nreg _RG1;
  c_tim* RG11timp = eaxtimp;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4P = xedxp;
  i32 RG52l;
  i32 RG53l;

  RG3Blo = byte_at(RG4P, lcon(0x4));
  if ((RG3Blo & bcon(0x4)) != 0)
  {
    if ((RG3Blo & bcon(0x20)) != 0 || RG11timp->getyB() != 0)
    {
      if (jz_test8(location(RG4P + lcon(0x4)), lcon(0x20)))
        return;
      if (RG11timp->getyB() != 1)
        return;
    }
  }
  if (RG2L == 0)
  {
    //m_473AB:
    RG2W = word_at(RG4P, 2);
    RG2UW >>= bcon(0x7);
    RG2L = unsignedlong(RG2W);
    RG52l = timdat.gametick + RG2L;
    RG2W = word_at(RG4P, lcon(0x4));
    RG2L <<= bcon(0x5);
    RG2UW >>= bcon(0xc);
    RG3L = unsignedlong(RG2W) + RG52l;
  }
  else
  {
    RG2W = word_at(RG4P, 2);
    RG2UW >>= bcon(0x7);
    RG53l = unsignedlong(RG2W);
    RG3W = word_at(RG4P, lcon(0x4));
    RG3L <<= bcon(0x5);
    RG3UW >>= bcon(0xc);
    RG53l <<= RG3Blo;
    RG3L = RG53l + timdat.gametick;
  }
  //m_473D3:
  i32 parl00 = RG3L;
  if (jz_test8(location(RG4P + lcon(0x4)), lcon(0x4)))
  {
    //m_473EC:
    RG1Blo = RG11timp->getyB();
    RG1L = unsignedlong(RG1Blo);
  }
  else
  {
    RG1W = word_at(RG4P, lcon(0x4));
    RG1L <<= 11;
    RG1UW >>= 14;
    RG1L = unsignedlong(RG1W);
  }
  //m_473F4:
  RG3L = unsignedlong(RG1W);
  RG1W = word_at(RG4P, lcon(0x6));
  RG1L <<= bcon(0xa);
  RG1UW >>= 14;
  RG2L = unsignedlong(RG1W);
  RG1W = word_at(RG4P, lcon(0x6));
  RG4L = RG1L;
  _RG4.ushiftr11();
  RG4L = unsignedlong(RG4W);
  RG1L <<= bcon(0x5);
  _RG1.ushiftr11();
  RG1L = unsignedlong(RG1W);
  DM2_INVOKE_MESSAGE(RG1L, RG4L, RG2L, RG3L, parl00);
}

static void DM2_ACTIVATE_RELAY2(c_tim* eaxtimp, unk* xedxp, i32 ebxl)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4P = xedxp;
  c_tim* RG5timp = eaxtimp;
  i16 RG6w;
  i16 vw_00;

  RG3P = RG4P;
  RG6w = RG2W;
  RG4W = word_at(RG4P, lcon(0x4)) & lcon(0x18);
  if (RG4W != lcon(0x18))
  {
    //m_4731F:
    if (jnz_test8(location(RG3P + lcon(0x4)), lcon(0x20)) || RG5timp->getyB() != 0)
    {
      if (jz_test8(location(RG3P + lcon(0x4)), lcon(0x20)))
        return;
      if (RG5timp->getyB() != 1)
        return;
    }
    RG2L = signedlong(RG6w);
    RG4W = word_at(RG3P, lcon(0x4));
    RG4L <<= 11;
    RG4UW >>= 14;
    RG4L = unsignedlong(RG4W);
  }
  else
  {
    if (jz_test8(location(RG3P + lcon(0x4)), lcon(0x20)))
    {
      //m_47315:
      RG2L = signedlong(RG6w);
      RG4L = unsignedlong(byte_at(RG1P, lcon(0x9)));
    }
    else
    {
      RG4L = unsignedlong(byte_at(RG1P, lcon(0x9)));
      RG2L = 0;
      DM2_INVOKE_ACTUATOR(RG3P, RG4L, RG2L);
      if (RG6w == 0)
        return;
      RG1Blo = RG5timp->getyB();
      if (mkub(RG1Blo) < mkub(1))
      {
        //m_472EF:
        if (RG1Blo == 0)
          vw_00 = 1;
      }
      else
      {
        if (mkub(RG1Blo) <= mkub(1))
        {
          //m_472FC:
          RG2L = 0;
          vw_00 = RG2W; // truncated
        }
        else
        {
          if (RG1Blo == 2)
            //m_47303:
            vw_00 = 2;
        }
      }
      //m_4730A:
      RG2L = signedlong(RG6w);
      RG4L = unsignedlong(vw_00);
    }
  }
  //m_4734B:
  DM2_INVOKE_ACTUATOR(RG3P, RG4L, RG2L);
}

// ecxl comes in as 0 or 1
static void DM2_ACTIVATE_ORNATE_ANIMATOR(c_tim* eaxtimp, unk* xedxp, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4P = xedxp;
  i8* RG5p;
  c_tim* RG6timp = eaxtimp;
  c_nreg _RG7;
  c_tim c38_00;
  i32 vl_0c;
  i16 vw_10;
  i16 vw_14;
  i16 parw00;
  i16 parw01;
  i16 parw02;

  RG5p = RG4P;
  RG1L = unsignedlong(RG6timp->getxA());
  vw_14 = RG1W;
  RG1L = unsignedlong(RG6timp->getyA());
  vw_10 = RG1W;
  RG1Bhi = byte_at(RG4P, lcon(0x4));
  if ((RG1Bhi & bcon(0x1)) == 0)
  {
    RG4Blo = RG1Bhi | bcon(0x1);
    mov8(location(RG5p + lcon(0x4)), RG4Blo);
    and16(location(RG5p + 2), lcon(0x7f));
    c38_00.setmticks(ddat.v1d3248, timdat.gametick + 1);
    c38_00.settype(0x55);
    c38_00.setactor(0);
    c38_00.setA(RG2W);
    c38_00.setBlong(RG3L);
    DM2_QUEUE_TIMER(&c38_00);
    if (jnz_test8(location(RG5p + lcon(0x4)), lcon(0x40)))
    {
      if (RG3L == 0)
      {
        //m_47231:
        RG4Bhi = lcon(0xa);
        RG1Blo = DM2_GET_FLOOR_DECORATION_OF_ACTUATOR(RG5p);
        RG4Blo = RG1Blo;
      }
      else
      {
        RG4Bhi = lcon(0x9);
        RG1Blo = DM2_GET_WALL_DECORATION_OF_ACTUATOR(RG5p);
        RG4Blo = RG1Blo;
        RG1L = unsignedlong(RG6timp->getyB());
        RG3L = unsignedlong(vw_14);
        RG3W += table1d27fc[RG1L];
        RG2L = unsignedlong(vw_10);
        RG2W += table1d2804[RG1L];
        RG1L = RG2L;
        RG2W = ddat.v1d3248;
        if (RG2W != ddat.v1e0282 || RG3W != ddat.v1e0280 || RG1W != ddat.v1e027e)
        {
          vw_14 = RG3W;
          vw_10 = RG1W;
        }
      }
      //m_4723C:
      RG1L = unsignedlong(RG4Blo);
      vl_0c = RG1L;
      RG7L = unsignedlong(RG4Bhi);
      RG1L = mkl(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(RG7Blo, RG1Blo, lcon(0xb), lcon(0x88)) + 1);
      put16(parw02, RG1W);
      RG1L = signedlong(vw_10);
      put16(parw01, RG1W);
      RG1L = signedlong(vw_14);
      put16(parw00, RG1W);
      DM2_QUEUE_NOISE_GEN2(RG7Blo, CUTLX8(vl_0c), lcon(0x88), lcon(0xfe), parw00, parw01, parw02, lcon(0x8c), lcon(0x80));
    }
  }
  //m_4728C:
  if (jnz_test8(location(RG5p + lcon(0x4)), lcon(0x4)))
    DM2_ACTIVATE_RELAY2(RG6timp, RG5p, 0);
}

// was SKW_3a15_1650
static void DM2_ACTIVATE_TEST_FLAG(c_tim* eaxtimp, unk* xedxp)
{
  c_nreg _RG1;
  c_tim* RG2timp = eaxtimp;
  c_nreg _RG3;
  c_nreg _RG4; RG4P = xedxp;
  c_wlreg _RG5;

  RG3P = RG4P;
  RG4L = unsignedlong(byte_at(RG1P, lcon(0x9)));
  RG1W = word_at(RG3P, 2);
  RG1UW >>= bcon(0x7);
  RG1L = DM2_GET_GLOB_VAR(unsignedlong(RG1W));
  RG1Blo = RG1W != wcon(0x0) ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  RG5W = word_at(RG3P, lcon(0x4));
  RG5L <<= bcon(0xa);
  RG5UW >>= bcon(0xf);
  if (RG1W == RG5W || (RG4W != 0 && RG4W != 2))
  {
    RG5W = word_at(RG3P, lcon(0x4));
    RG5L <<= bcon(0xa);
    RG5UW >>= bcon(0xf);
    if (RG1W != RG5W)
      return;
    if (RG4W != 1)
      return;
  }
  RG1L = 0;
  if (jz_test8(location(RG3P + lcon(0x4)), lcon(0x4)))
    //m_4750F:
    RG4L = unsignedlong(RG2timp->getyB());
  else
  {
    RG4W = word_at(RG3P, lcon(0x4));
    RG4L <<= 11;
    RG4UW >>= 14;
    RG4L = unsignedlong(RG4W);
  }
  //m_47514:
  RG4L = unsignedlong(RG4W);
  DM2_INVOKE_ACTUATOR(RG3P, RG4L, RG1L);
}

// was SKW_3a15_15d8
static void DM2_ACTIVATE_INVERSE_FLAG(c_tim* eaxtimp, unk* xedxp)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4P = xedxp;
  c_tim* RG5timp = eaxtimp;

  RG3P = RG4P;
  if (jz_test8(location(RG4P + lcon(0x4)), lcon(0x20)))
    //m_47441:
    RG1L = 0;
  else
    RG1L = 3;
  //m_47443:
  RG4L = unsignedlong(RG1W);
  RG1L = unsignedlong(RG5timp->getyB()) + RG4L;
  RG2L = unsignedlong(RG1W);
  RG1W = word_at(RG3P, 2);
  RG1UW >>= bcon(0x7);
  RG1L = unsignedlong(RG1W);
  DM2_UPDATE_GLOB_VAR(RG1L, 1, RG2L);
  RG2L = 0;
  if (jz_test8(location(RG3P + lcon(0x4)), lcon(0x4)))
    //m_47485:
    RG1L = unsignedlong(RG5timp->getyB());
  else
  {
    RG1W = word_at(RG3P, lcon(0x4));
    RG1L <<= 11;
    RG1UW >>= 14;
    RG1L = unsignedlong(RG1W);
  }
  //m_4748A:
  RG4L = unsignedlong(RG1W);
  DM2_INVOKE_ACTUATOR(RG3P, RG4L, RG2L);
}

static i32 DM2_timproc_3a15_1da8(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4; RG4L = edxl;

  if (RG1UW < mkuw(lcon(0x1)))
    return RG1W == 0 ? lcon(0x1) : 0;
  if (RG1UW <= mkuw(lcon(0x1)))
    return 0;
  if (RG1W != 2)
    return 0;
  RG4Blo ^= 1;
  return RG4L;
}

static void DM2_ACTIVATE_CONTINUOUS_ORNATE_ANIMATOR(i32 eaxl, c_tim* edxtimp, unk* xebxp, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4;
  i8* RG5p;
  c_tim* RG6timp = edxtimp;
  i16 vw_00;
  c_tim c38_04;
  i16 vw_10;
  i16 parw00;

  vw_00 = RG1W; // truncated
  RG5p = RG2P;
  RG1W = word_at(RG2P, lcon(0x4));
  RG1L <<= bcon(0xd);
  RG1UW >>= bcon(0xf);
  RG2L = RG1L;
  RG4L = signedlong(RG1W);
  RG1L = DM2_timproc_3a15_1da8(unsignedlong(RG6timp->getyB()), RG4L);
  RG1W &= 0x1;
  and8(location(RG5p + lcon(0x4)), lcon(0xfffffffb));
  RG1L *= 4;
  RG4W = word_at(RG5p, lcon(0x4));
  RG4L |= RG1L;
  mov16(location(RG5p + lcon(0x4)), RG4W);
  RG1L = RG4L << bcon(0xd);
  RG1UW >>= bcon(0xf);
  if (RG2W != RG1W)
  {
    RG4L = RG3L;
    RG1L = DM2_GET_ORNATE_ANIM_LEN(RG5p, RG4L);
    RG2L = RG1L;
    vw_10 = RG1W; // truncated
    RG4Bhi = byte_at(RG5p, lcon(0x4));
    if ((RG4Bhi & bcon(0x4)) == 0)
    {
      //m_470BF:
      RG1W = word_at(RG5p, 2);
      RG1UW >>= bcon(0x7);
      RG1W = unsignedword(RG1Blo);
      RG1L = unsignedlong(RG1W) + timdat.gametick;
      RG3L = unsignedlong(RG2W);
      RG4L = RG1L % RG3L;
      RG1L /= RG3L;
      RG3L = RG4L;
      if (RG4W != 0)
      {
        //m_47100:
        RG2L -= RG4L;
        RG1L = signedlong(RG2W);
        c38_04.setmticks(ddat.v1d3248, timdat.gametick + RG1L);
        c38_04.settype(0x59);
        RG2W = unsignedword(RG2Blo);
        c38_04.setactor(0);
        c38_04.setB(vw_00);
        DM2_QUEUE_TIMER(&c38_04);
      }
      else
      {
        RG1W = ddat.v1d3248;
        if (RG1W == ddat.v1e0266)
          ddat.v1e0390.b_00 |= 1;
        and8(location(RG5p + lcon(0x4)), lcon(0xfffffffe));
      }
    }
    else
    {
      if ((RG4Bhi & bcon(0x1)) == 0)
      {
        RG1Bhi = RG4Bhi | bcon(0x1);
        mov8(location(RG5p + lcon(0x4)), RG1Bhi);
        RG1W = ddat.v1d3248;
        if (RG1W == ddat.v1e0266)
          ddat.v1e0390.b_00 |= 1;
        RG2L = unsignedlong(vw_10);
        RG4L = timdat.gametick % RG2L;
        RG1L = RG2L - RG4L;
        RG4L = RG1L % RG2L;
        RG1L /= RG2L;
        RG2L = RG4L & lcon(0xff);
        RG4W = word_at(RG5p, 2);
        RG1L = RG4L;
        RG1UW >>= bcon(0x7);
        RG1W &= lcon(0x100);
        RG1L = unsignedlong(RG1W) | mkul(RG2L);
        RG1Bhi &= 0x1;
        RG4L &= lcon(0xffff007f);
        RG1L <<= bcon(0x7);
        RG2L = RG4L | RG1L;
        mov16(location(RG5p + 2), RG2W);
        i32 parl01 = RG3L;
        RG1L = unsignedlong(vw_10);
        put16(parw00, RG1W);
        DM2_TRY_ORNATE_NOISE(RG5p, unsignedlong(vw_00), unsignedlong(RG6timp->getxA()), unsignedlong(RG6timp->getyA()), parw00, parl01);
      }
    }
  }
  //m_4713A:
  if (jz_test8(location(RG5p + lcon(0x4)), lcon(0x20)))
    return;
  RG1W = word_at(RG5p, lcon(0x4)) & lcon(0x18);
  if (RG1W == lcon(0x18))
    DM2_INVOKE_ACTUATOR(RG5p, unsignedlong(RG6timp->getyB()), 0);
}

static void DM2_ACTIVATE_SHOOTER(unk* xeaxp, c_tim* edxtimp)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_tim* RG41timp = edxtimp;
  c_wlreg _RG5;
  c_wlreg _RG61;
  c_nreg _RG62;
  i32 RG71l;
  c_nreg _RG72;
  i32 vl_00;
  i32 vl_04;
  i32 vl_08;
  i32 vl_0c;
  i32 vl_10;
  i32 vl_14;
  i32 vl_18;
  i16 vw_1c;
  i8 vb_20;
  i16 vw_24;
  c_o32 vo_28; // poke 16 peek 32 add16
  c_o32 vo_2c; // poke 16 peek 32 add16
  i8 vb_30;
  i8 parb01;
  i8 parb03;
  i8 parb05;
  i8 parb07;
  bool skip01005 = false;

  RG2P = RG1P;
  RG1W = unsignedword(RG1Blo);
  RG1Blo = RG41timp->getxA();
  vo_2c.poke16(RG1W);
  RG1Blo = RG41timp->getyA();
  vo_28.poke16(RG1W);
  RG1Blo = RG41timp->getxB();
  vw_1c = RG1W;
  RG1L += 2;
  RG1W &= 0x3;
  vw_24 = RG1W;
  RG1W = word_at(RG2P, 2) & lcon(0x7f);
  RG3W = word_at(RG2P, 2);
  RG3UW >>= bcon(0x7);
  RG4W = word_at(RG2P, lcon(0x6));
  RG4UW >>= bcon(0x4);
  RG2L = RG4L;
  RG2Bhi ^= RG4Bhi;
  vb_20 = RG2Blo;
  RG4L = unsignedlong(RG4W);
  RG4UL >>= bcon(0x8);
  vb_30 = RG4Blo;
  if (RG1W != lcon(0x7) && RG1W != lcon(0x8) && RG1W != lcon(0xe))
    //m_47B8E:
    RG2L = 0;
  else
    RG2L = 1;
  //m_47B90:
  RG71l = RG2L;
  RG5L = RG3L + lcon(0xff80);
  RG61L = RG5L;
  if (RG1W != lcon(0xe) && RG1W != lcon(0xf))
  {
    //m_47CC4:
    if (RG1W == lcon(0x8) || RG1W == lcon(0xa))
      skip01005 = true;
    else
    {
      RG4L = unsignedlong(RG3W);
      RG1L = DM2_ALLOC_NEW_DBITEM(RG4L);
      RG61L = RG1L;
      if (RG1W == lcon(0xffffffff))
        return;
      if (RG2L != 0)
        skip01005 = true;
      else
      {
        RG1L = DM2_ALLOC_NEW_DBITEM(RG4L);
        RG5L = RG1L;
        if (RG1W != lcon(0xffffffff))
          skip01005 = true;
      }
    }
  }
  else
  {
    RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(vo_2c.peek16(), vo_28.peek16()));
    //m_47BBF:
    for (;;)
    {
      RG61L = RG1L;
      if (RG61W == lcon(0xfffffffe))
        break;
      RG1L = unsignedlong(RG61W) >> bcon(0xe);
      RG4L = RG61L;
      RG4W &= lcon(0x3c00);
      RG4L = unsignedlong(RG4W) >> bcon(0xa);
      if (RG4L > lcon(0x3))
      {
        if (RG1W == vw_1c)
          break;
        RG4L = unsignedlong(vw_1c) + 1 & lcon(0x3);
        RG1L = unsignedlong(RG1W);
        if (RG1L == RG4L)
          break;
      }
      //m_47BFF:
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG61UW)));
    }
    //m_47C0B:
    if (RG61W == lcon(0xfffffffe))
      return;
    vl_08 = signedlong(vo_28.peek16());
    vl_14 = signedlong(vo_2c.peek16());
    RG1L = unsignedlong(RG61W);
    RG3L = vl_08;
    DM2_CUT_RECORD_FROM(mk_record(RG1L), NULL, CUTX16(vl_14), RG3W);
    if (RG71l != 0)
      skip01005 = true;
    else
    {
      RG5W = record_to_word(DM2_GET_TILE_RECORD_LINK(CUTX16(vl_14), CUTX16(vl_08)));
      RG2W = vw_1c;
      //m_47C5A:
      for (;;)
      {
        if (RG5W == lcon(0xfffffffe))
          break;
        RG1L = unsignedlong(RG5W) >> bcon(0xe);
        RG4L = RG5L;
        RG4W &= lcon(0x3c00);
        RG4L = unsignedlong(RG4W) >> bcon(0xa);
        if (RG4L > lcon(0x3))
        {
          if (RG1W == RG2W)
            break;
          RG4L = unsignedlong(RG2W) + 1 & lcon(0x3);
          RG1L = unsignedlong(RG1W);
          if (RG1L == RG4L)
            break;
        }
        //m_47C94:
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG5UW)));
        RG5L = RG1L;
      }
      //m_47CA2:
      if (RG5W != lcon(0xfffffffe))
      {
        RG3L = signedlong(vo_28.peek16());
        RG2L = signedlong(vo_2c.peek16());
        RG1L = unsignedlong(RG5W);
        DM2_CUT_RECORD_FROM(mk_record(RG1L), NULL, RG2W, RG3W);
        skip01005 = true;
      }
    }
  }

  if (!skip01005)
    //m_47CFB:
    RG71l = 1;

  //m_47D00:
  if (RG71l != 0)
  {
    RG1L = (DM2_RANDBIT() ? 1 : 0) + unsignedlong(vw_24);
    RG1W &= 0x3;
    vw_24 = RG1W;
  }
  RG1L = unsignedlong(vw_1c);
  RG4W = table1d27fc[RG1L];
  vo_2c.add16(RG4W);
  RG1W = table1d2804[RG1L];
  vo_28.add16(RG1W);
  ddat.v1e01a8 = 1;
  RG1L = unsignedlong(vb_30);
  vl_0c = RG1L;
  put8(parb03, RG1Blo);
  RG1L = unsignedlong(vb_20);
  vl_10 = RG1L;
  put8(parb01, RG1Blo);
  RG1L = signedlong(vw_1c);
  vl_04 = RG1L;
  i32 parl00 = RG1L;
  RG3L = signedlong(vw_24);
  vl_00 = signedlong(vo_28.peek16());
  vl_18 = signedlong(vo_2c.peek16());
  RG1L = unsignedlong(RG61W);
  DM2_SHOOT_ITEM(RG1L, vl_18, vl_00, RG3L, parl00, parb01, lcon(0x64), parb03);
  if (RG71l == 0)
  {
    RG62L = vl_0c;
    put8(parb07, RG62Blo);
    RG72L = vl_10;
    put8(parb05, RG72Blo);
    i32 parl04 = vl_04;
    RG1L = unsignedlong(vw_24) + 1;
    RG1W &= 0x3;
    RG3L = signedlong(RG1W);
    RG1L = unsignedlong(RG5W);
    DM2_SHOOT_ITEM(RG1L, vl_18, vl_00, RG3L, parl04, parb05, lcon(0x64), parb07);
  }
  //m_47DC6:
  RG4L = 0;
  ddat.v1e01a8 = RG4W;
}

static void DM2_ACTIVATE_TICK_GENERATOR(unk* xeaxp, i32 edxl)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  c_tim c38_00;

  RG2P = RG1P;
  RG5w = RG4W;
  RG1W = word_at(RG1P, 2) & lcon(0x7f);
  if (RG1UW < mkuw(lcon(0x34)))
  {
    //m_493D6:
    if (RG1UW >= mkuw(lcon(0x1e)))
    {
      if (RG1UW <= mkuw(lcon(0x1e)))
        //m_493E6:
        RG3L = 1;
      else
      {
        if (RG1W == lcon(0x33))
          //m_493ED:
          RG3L = lcon(0x8);
      }
    }
  }
  else
  {
    if (RG1UW <= mkuw(lcon(0x34)))
      //m_493F4:
      RG3L = lcon(0x10);
    else
    {
      if (RG1UW < mkuw(lcon(0x36)))
        //m_493FB:
        RG3L = lcon(0x20);
      else
      {
        if (RG1UW <= mkuw(lcon(0x36)))
          //m_49402:
          RG3L = lcon(0x40);
        else
        {
          if (RG1W == lcon(0x37))
            //m_49409:
            RG3L = lcon(0x80);
        }
      }
    }
  }
  //m_4940E:
  RG4W = word_at(RG2P, 2);
  if ((RG4W & wcon(0xff80)) != 0)
  {
    RG1L = RG4L;
    RG1UW >>= 7;
    RG4L = unsignedlong(RG1W);
    RG1L = unsignedlong(RG3W);
    c38_00.setmticks(ddat.v1d3248, (timdat.gametick % (RG4L * RG1L)) + timdat.gametick);
    c38_00.settype(0x56);
    c38_00.setactor(0);
    c38_00.setA(RG5w);
    c38_00.setxB(RG3Blo);
    c38_00.mask_wvalueB(0xfeff, 0); // TODO: and the other 7 bits in Bhi stay undefined?
    DM2_QUEUE_TIMER(&c38_00);
    or8(location(RG2P + lcon(0x4)), lcon(0x1));
  }
}

void SKW_3a15_0d5c(c_tim* eaxtimp, unk* xedxp)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4; RG4P = xedxp;

  RG2P = RG4P;
  if (jnz_test8(location(RG4P + lcon(0x4)), lcon(0x20)) || eaxtimp->getyB() != 0)
  {
    if (jz_test8(location(RG2P + lcon(0x4)), lcon(0x20)))
      return;
    if (eaxtimp->getyB() != 1)
      return;
  }
  RG1W = word_at(RG2P, lcon(0x6));
  RG4L = RG1L;
  _RG4.ushiftr11();
  RG4L = unsignedlong(RG4W);
  RG1L <<= bcon(0x5);
  _RG1.ushiftr11();
  RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG1W), RG4L);
  RG4L = RG1L;
  RG1Blo = RG1W != wcon(0xffff) ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  if (RG1L != 0)
  {
    RG1W = word_at(RG2P, 2);
    RG1UW >>= bcon(0x7);
    RG1W &= 0x3;
    RG2L = signedlong(RG1W);
    RG1L = unsignedlong(RG4W);
    DM2_ROTATE_CREATURE(RG1L, lcon(0x1), RG2L);
  }
}

static i32 DM2_ACTUATE_WALL_MECHA(c_tim* eaxtimp)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG51p;
  i8* RG52p;
  c_tim* RG6timp = eaxtimp;
  c_tim c38_00;
  i16 vw_0c;
  i16 vw_10;
  i16 vw_14;
  i16 vw_18;
  i16 vw_1c;
  i16 vw_20;
  c_ql32 vql_24;
  i16 vw_28;
  i16 parw01;
  i16 parw02;
  i16 parw03;
  unk* xparp07;
  i32 parl09;
  i16 parw10;
  i16 parw11;
  bool flag;

  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(unsignedword(RG6timp->getxA()), unsignedword(RG6timp->getyA())));
  //m_483CE:
  for (;;)
  {
    bool skip01032 = false;
    bool skip01029 = false;
    bool skip01028 = false;
    bool skip01027 = false;
    bool skip01026 = false;
    bool skip01025 = false;
    for (;;)
    {
      bool skip01021 = false;
      vql_24.poke32(RG1L);
      RG4L = vql_24.peek32();
      RG1Blo = RG4W != wcon(0xfffe) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L == 0)
        return RG1L;
      RG1L = RG4L;
      RG1Blo ^= RG4Blo;
      RG1Bhi &= lcon(0x3c);
      RG1L = unsignedlong(RG1W) >> bcon(0xa);
      if (RG1L > lcon(0x3))
        return RG1L;
      RG4L = unsignedlong(RG1W);
      if (RG4L != 2)
        skip01021 = true;
      else
      {
        RG2L = unsignedlong(RG6timp->getxB());
        RG4L = unsignedlong(vql_24.peek16());
        RG3L = RG4L >> bcon(0xe);
        if (RG3L != RG2L)
          skip01021 = true;
        else
        {
          RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
          RG51p = RG1P;
          if (jnz_test8(location(RG1P + 2), lcon(0x6)))
          {
            RG1W = word_at(RG1P, 2) & lcon(0x6);
            if (RG1W != 2)
            {
              RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
              continue;
            }
          }
          //m_48448:
          RG1W = word_at(RG51p, 2);
          RG1UW >>= bcon(0x3);
          RG4L = unsignedlong(RG1W);
          RG4UL >>= bcon(0x8);
          RG4L &= lcon(0x1f);
          vw_20 = RG4W; // truncated
          if (RG4UW < mkuw(lcon(0x7)))
          {
            //m_48476:
            if (RG4W != lcon(0x5))
            {
              RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
              continue;
            }
          }
          else
          {
            if (RG4UW > mkuw(lcon(0x7)))
            {
              if (RG4W == lcon(0x17))
              {
                //m_485DB:
                if (RG6timp->getyB() == 0)
                {
                  put16(parw03, unsignedword(RG6timp->getyA()));
                  put16(parw02, unsignedword(RG6timp->getxA()));
                  put16(parw01, lcon(0xc8));
                  RG2L = unsignedlong(RG1Blo);
                  RG1Blo = CUTX8(ddat.v1d3248) + 1;
                  RG4L = unsignedlong(RG1Blo);
                  RG3L = lcon(0x73);
                  DM2_QUEUE_NOISE_GEN1(lcon(0x3), RG4Blo, RG2Blo, RG3W, parw01, parw02, parw03, 1);
                }
              }
              RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
              continue;
            }
          }
          //m_48480:
          RG1P = RG51p + 2;
          RG3Bhi = RG6timp->getyB();
          if (RG3Bhi != 2)
            //m_48491:
            flag = RG3Bhi == bcon(0x0);
          else
            flag = jz_test8(location(RG51p + 2), 1);
          //m_48493:
          RG4Blo = flag ? 1 : 0;
          RG4W &= 0x1;
          and8(location(RG1P), lcon(0xfffffffe));
          or16(location(RG1P), RG4W);
          RG1L = unsignedlong(vw_20);
          if (RG1L == lcon(0x7))
          {
            RG1W = word_at(RG51p, 2);
            RG1UW >>= bcon(0x3);
            RG4L = unsignedlong(RG1Blo);
            RG3L = lcon(0x11);
            RG2L = lcon(0xb);
            RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, RG4Blo, 11, 17);
            if (RG1W == 0)
              //m_484DD:
              RG4L = 1;
            else
              RG4L = lcon(0xffffffff);
            //m_484E2:
            RG1L = unsignedlong(RG6timp->getxA());
            vw_0c = RG1W;
            RG1L = unsignedlong(RG6timp->getyA());
            vw_28 = RG1W;
            RG4L = signedlong(RG4W);
            RG1L = DM_LOCATE_OTHER_LEVEL(signedlong(ddat.v1e0266), RG4L, &vw_0c, &vw_28, NULL);
            if (RG1W >= 0)
            {
              vw_14 = ddat.v1d3248;
              RG1L = signedlong(RG1W);
              DM2_CHANGE_CURRENT_MAP_TO(RG1L);
              RG4L = signedlong(vw_28);
              RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(vw_0c, RG4W));
              //m_4853E:
              for (;;)
              {
                RG2L = RG1L;
                if (RG2W == lcon(0xfffffffe))
                  break;
                RG1L = RG2L;
                RG1Blo ^= RG2Blo;
                RG1Bhi &= lcon(0x3c);
                RG1L = unsignedlong(RG1W) >> bcon(0xa);
                if (RG1L == 2)
                {
                  RG4L = unsignedlong(RG6timp->getxB());
                  RG1L = unsignedlong(RG2W);
                  RG3L = RG1L >> bcon(0xe);
                  if (RG3L == RG4L)
                  {
                    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
                    RG4W = word_at(RG1P, 2) & lcon(0x6);
                    if (RG4W == 2)
                    {
                      RG4W = word_at(RG1P, 2);
                      RG3L = RG4L;
                      RG3UW >>= bcon(0x3);
                      vw_10 = RG3W; // truncated
                      RG3L = unsignedlong(vw_10);
                      RG3UL >>= bcon(0x8);
                      RG3L &= lcon(0x1f);
                      if (RG3L == lcon(0x7))
                      {
                        RG3W = word_at(RG51p, 2) & 1;
                        RG4L &= lcon(0xfffffffe);
                        mov16(location(RG1P + 2), RG4W);
                        RG4L = RG3L;
                        or16(location(RG1P + 2), RG4W);
                      }
                    }
                  }
                }
                RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG2UW)));
              }
              //m_485CA:
              DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_14));
            }
          }
          RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
          continue;
        }
      }

      if (skip01021)
        break;
    }
    //m_4861B:
    RG1L = unsignedlong(RG1W);
    if (RG1L != lcon(0x3))
    {
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
      continue;
    }
    RG1L = unsignedlong(RG6timp->getxB());
    RG2L = unsignedlong(vql_24.peek16());
    RG4L = RG2L >> bcon(0xe);
    if (RG4L != RG1L)
    {
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
      continue;
    }
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
    RG4P = RG1P;
    RG52p = RG1P;
    RG1W = word_at(RG1P, 2) & lcon(0x7f);
    vw_20 = RG1W; // truncated
    RG3W = word_at(RG4P, 2);
    RG3UW >>= bcon(0x7);
    RG1L -= lcon(0x7);
    vw_1c = RG1W; // truncated
    if (RG1UW > mkuw(lcon(0x42)))
    {
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
      continue;
    }
    RG1L = unsignedlong(vw_1c);
    switch (RG1L)
    {
      case 0:
      case 1:
      case 2:
      case 3:
      case 7:
      case 8:
        //m_48A0E:
        RG1P = RG52p;
        DM2_ACTIVATE_SHOOTER(RG1P, RG6timp);
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        skip01032 = true;
        break;

      case 4:
      case 5:
      case 6:
      case 9:
      case 10:
      case 12:
      case 13:
      case 14:
      case 16:
      case 17:
      case 18:
      case 19:
      case 20:
      case 21:
      case 24:
      case 26:
      case 27:
      case 28:
      case 29:
      case 30:
      case 32:
      case 33:
      case 34:
      case 35:
      case 36:
      case 40:
      case 41:
      case 49:
      case 50:
      case 51:
      case 55:
      case 56:
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        skip01032 = true;
        break;

      case 11:
        //m_48AC3:
        RG1L = 0;
        DM2_SELECT_PALETTE_SET(0);
        ddat.v1e0240 = 1;
        DM2_PREPARE_EXIT();
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        skip01032 = true;
        break;

      case 15:
        //m_48A1C:
        RG1L = RG3L;
        RG1Bhi ^= RG3Bhi;
        RG1Blo &= lcon(0x3f);
        c38_00.setmticks(RG1W, timdat.gametick);
        RG1Blo = RG6timp->getactor();
        c38_00.setactor(RG6timp->getactor());
        c38_00.setxyA(CUTX8(mkuw(word_at(RG52p, lcon(0x6))) >> 6), CUTX8(mkuw(word_at(RG52p, lcon(0x6))) >> 11));
        RG1L = RG3L;
        RG1Bhi ^= RG3Bhi;
        RG1Blo &= lcon(0x3f);
        RG4L = 4 * (RG1L & lcon(0xffff));
        RG1P = DOWNCAST(t_tile*, mapdat.tmpmap) + RG4L;
        RG4L = unsignedlong(c38_00.getxA()) << 2;
        RG1P = pointer_at(RG1P) + RG4L;
        RG4L = unsignedlong(c38_00.getyA());
        RG1P = pointer_at(RG1P);
        RG1Blo = byte_at(RG1P, RG4L);
        RG1L = unsignedlong(RG1Blo) >> 5;
        RG1W = unsignedword(RG1Blo);
        if (RG1W != 0)
          //m_48AA7:
          c38_00.setxB(RG1Bhi);
        else
        {
          RG1L = unsignedlong(RG3W) >> 6 & lcon(0x3);
          c38_00.setxB(RG1Blo);
        }
        //m_48AAB:
        c38_00.settype(0x4);
        RG1Blo = RG6timp->getyB();
        c38_00.setyB(RG1Blo);
        DM2_QUEUE_TIMER(&c38_00);
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        skip01032 = true;
        break;

      case 22:
        //m_487DF:
        RG2W = word_at(RG4P, 2);
        if ((RG2W & wcon(0xff80)) == 0)
          skip01025 = true;
        else
        {
          RG1L = RG2L;
          RG1UW >>= bcon(0x7);
          RG1W &= lcon(0x100);
          RG1L = unsignedlong(RG1W);
          if (RG1L == 0)
            //m_48803:
            RG1L = 0;
          else
            skip01025 = true;
        }
        break;

      case 23:
      case 44:
      case 45:
      case 46:
      case 47:
      case 48:
        //m_48993:
        RG1W = word_at(RG52p, lcon(0x4));
        RG1L <<= bcon(0xd);
        RG1UW >>= bcon(0xf);
        RG4L = signedlong(RG1W);
        RG1L = DM2_timproc_3a15_1da8(unsignedlong(RG6timp->getyB()), RG4L);
        RG1W &= 0x1;
        and8(location(RG52p + lcon(0x4)), lcon(0xfffffffb));
        RG1L *= 4;
        or16(location(RG52p + lcon(0x4)), RG1W);
        RG3Blo = byte_at(RG52p, lcon(0x4));
        if ((RG3Blo & bcon(0x1)) == 0 && (RG3Blo & bcon(0x4)) != 0)
        {
          RG4L = unsignedlong(vql_24.peek16());
          RG1P = RG52p;
          DM2_ACTIVATE_TICK_GENERATOR(RG1P, RG4L);
        }
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        skip01032 = true;
        break;

      case 25:
      case 62:
        //m_489E2:
        RG1Blo = vw_20 == wcon(0x45) ? 1 : 0;
        RG2L = unsignedlong(RG1Blo);
        RG4P = RG52p;
        DM2_ACTIVATE_RELAY1(RG6timp, RG4P, RG2L);
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        skip01032 = true;
        break;

      case 31:
        //m_48B4F:
        RG1W = word_at(RG4P, lcon(0x4));
        RG1L <<= bcon(0xd);
        RG1UW >>= bcon(0xf);
        RG4L = signedlong(RG1W);
        RG1L = DM2_timproc_3a15_1da8(signedlong(CUTX16(unsignedlong(RG6timp->getyB()))), RG4L);
        RG1W &= 0x1;
        and8(location(RG52p + lcon(0x4)), lcon(0xfffffffb));
        RG1L *= 4;
        or16(location(RG52p + lcon(0x4)), RG1W);
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        skip01032 = true;
        break;

      case 37:
        //m_48B35:
        RG3L = 1;
        RG2P = RG4P;
        DM2_ACTIVATE_CONTINUOUS_ORNATE_ANIMATOR(unsignedlong(vql_24.peek16()), RG6timp, RG2P, RG3L);
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        skip01032 = true;
        break;

      case 38:
        //m_488DE:
        RG1W = word_at(RG4P, 2);
        RG1Blo &= lcon(0xffffff80);
        if (RG1W == 0)
          skip01026 = true;
        else
        {
          RG1W = word_at(RG4P, 2);
          RG1Blo &= lcon(0xffffff80);
          if (RG1UW > mkuw(lcon(0xffffc800)))
            skip01026 = true;
          else
          {
            RG1W = word_at(RG4P, 2);
            RG1UW >>= bcon(0x7);
            RG1L--;
            RG1Bhi &= 0x1;
            RG3W = word_at(RG4P, 2);
            RG3L &= lcon(0xffff007f);
            mov16(location(RG4P + 2), RG3W);
            RG1L <<= bcon(0x7);
            RG4L = RG3L | RG1L;
            mov16(location(RG52p + 2), RG4W);
            RG2L = 0;
            RG1W = unsignedword(RG1Blo);
            RG1Blo = RG6timp->getyB();
            RG4L = unsignedlong(RG1W);
            RG1P = RG52p;
            DM2_INVOKE_ACTUATOR(RG1P, RG4L, RG2L);
            RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
            skip01032 = true;
          }
        }
        break;

      case 39:
        //m_4870E:
        if (RG6timp->getyB() == 0)
        {
          RG1W = word_at(RG4P, lcon(0x6));
          RG1L <<= bcon(0x5);
          _RG1.ushiftr11();
          vw_0c = RG1W;
          RG1W = word_at(RG4P, lcon(0x6));
          _RG1.ushiftr11();
          vw_28 = RG1W;
          vw_18 = 7;
          if (jz_test8(location(RG4P + lcon(0x4)), lcon(0x4)))
          {
            //m_4874A:
            RG1W = word_at(RG4P, lcon(0x4));
            RG1L <<= 11;
            RG1UW >>= 14;
          }
          else
            RG1L = signedlong(DM2_RANDDIR());
          //m_48755:
          i32 parl05 = unsignedlong(vw_28);
          RG3L = unsignedlong(vw_0c);
          RG2L = unsignedlong(RG1W);
          RG4L = unsignedlong(vw_18);
          RG1W = word_at(RG52p, 2);
          RG1UW >>= bcon(0x7);
          vw_10 = RG1W; // truncated
          RG1W = dm2_dballochandler.DM2_ALLOC_NEW_CREATURE(vw_10, RG4W, RG2W, RG3W, CUTX16(parl05));
          if (jnz_test8(location(RG52p + lcon(0x4)), lcon(0x20)))
          {
            if (RG1W != lcon(0xffffffff))
            {
              RG4W = word_at(RG52p, lcon(0x4));
              RG4L <<= bcon(0x5);
              RG4UW >>= bcon(0xc);
              RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
              mov16(location(RG1P + lcon(0x8)), RG4W);
            }
          }
          if (jnz_test8(location(RG52p + lcon(0x4)), lcon(0x40)))
          {
            put16(parw03, vw_28);
            put16(parw02, vw_0c);
            put16(parw01, lcon(0x80));
            RG3L = lcon(0x61);
            RG2L = lcon(0x89);
            RG4L = 0;
            DM2_QUEUE_NOISE_GEN1(lcon(0x3), RG4Blo, RG2Blo, RG3W, parw01, parw02, parw03, 1);
          }
        }
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        skip01032 = true;
        break;

      case 42:
        //m_48681:
        if (jnz_test8(location(RG4P + lcon(0x4)), 1))
          RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        else
        {
          bool skip01024 = false;
          c38_00.setmticks(ddat.v1d3248, unsignedlong(RG3W) + timdat.gametick);
          c38_00.settype(0x5b);
          RG3W = unsignedword(RG3Blo);
          c38_00.setactor(0);
          RG1L = vql_24.peek32();
          c38_00.setA(RG1W);
          RG1W = DM2_QUEUE_TIMER(&c38_00);
          RG1Blo = byte_at(RG4P, lcon(0x4)) | bcon(0x1);
          mov8(location(RG4P + lcon(0x4)), RG1Blo);
          if ((RG1Blo & bcon(0x20)) == 0 && RG6timp->getyB() == 0)
            skip01024 = true;
          else
          {
            if (jz_test8(location(RG52p + lcon(0x4)), lcon(0x20)) || RG6timp->getyB() != 1)
              RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
            else
              skip01024 = true;
          }

          if (skip01024)
          {
            //m_486E7:
            RG2L = 0;
            if (jz_test8(location(RG52p + lcon(0x4)), lcon(0x4)))
            {
              //m_48704:
              RG1L = 0;
              RG1Blo = RG6timp->getyB();
            }
            else
            {
              RG1W = word_at(RG52p, lcon(0x4));
              RG1L <<= 11;
              RG1UW >>= 14;
              RG1L = unsignedlong(RG1W);
            }
            RG4L = unsignedlong(RG1W);
            RG1P = RG52p;
            DM2_INVOKE_ACTUATOR(RG1P, RG4L, RG2L);
            RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
            continue;
          }
        }
        skip01032 = true;
        break;

      case 43:
        //m_48ADE:
        RG3L = 1;
        DM2_ACTIVATE_ORNATE_ANIMATOR(RG6timp, RG4P, RG2L, 1);
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        skip01032 = true;
        break;

      case 52:
        //m_48B7C:
        parl09 = 0;
        skip01027 = true;
        break;

      case 53:
        //m_48BB5:
        if (jz_test8(location(RG4P + lcon(0x4)), lcon(0x20)) && RG6timp->getyB() == 0)
          skip01029 = true;
        else
        {
          if (jz_test8(location(RG52p + lcon(0x4)), lcon(0x20)) || RG6timp->getyB() != 1)
          {
            RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
            skip01032 = true;
          }
          else
            skip01029 = true;
        }
        break;

      case 54:
        //m_489FD:
        RG2L = unsignedlong(RG3W);
        DM2_ACTIVATE_RELAY2(RG6timp, RG4P, RG2L);
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        skip01032 = true;
        break;

      case 57:
        //m_48BA3:
        parl09 = 0;
        xparp07 = NULL;
        RG3L = 0;
        DM2_ACTIVATE_ITEM_TELEPORT(RG6timp, RG4P, RG2L, RG3L, 1, xparp07, 0, parl09);
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        skip01032 = true;
        break;

      case 58:
        //m_48AEF:
        if (RG6timp->getyB() != 0)
          //m_48AF8:
          RG3L--;
        else
          RG3L++;
        //m_48AF9:
        RG4L = 1;
        RG1L = DM2_GET_ORNATE_ANIM_LEN(RG52p, RG4L);
        RG2L = unsignedlong(RG1W);
        RG4L = unsignedlong(RG3W);
        RG1L = RG4L;
        RG4L = RG1L % RG2L;
        RG1L /= RG2L;
        RG4Bhi &= 0x1;
        RG1L = RG4L;
        RG3W = word_at(RG52p, 2);
        RG3L &= lcon(0xffff007f);
        RG1L <<= bcon(0x7);
        RG4L = RG3L | RG1L;
        mov16(location(RG52p + 2), RG4W);
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        skip01032 = true;
        break;

      case 59:
        //m_48C8D:
        SKW_3a15_0d5c(RG6timp, RG4P);
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        skip01032 = true;
        break;

      case 60:
        //m_48C99:
        DM2_ACTIVATE_INVERSE_FLAG(RG6timp, RG4P);
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        skip01032 = true;
        break;

      case 61:
        //m_48CA5:
        DM2_ACTIVATE_TEST_FLAG(RG6timp, RG4P);
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        skip01032 = true;
        break;

      case 63:
        //m_48CAE:
        RG1W = word_at(RG4P, lcon(0x6));
        RG4L = RG1L;
        _RG4.ushiftr11();
        RG4L = signedlong(RG4W);
        RG1L <<= bcon(0x5);
        _RG1.ushiftr11();
        RG1RP = DM2_GET_ADDRESS_OF_TILE_RECORD(RG1W, RG4W);
        RG4Blo = RG6timp->getyB();
        if (mkub(RG4Blo) < mkub(1))
        {
          //m_48CD9:
          if (RG4Blo == 0)
            or8(location(RG1P + lcon(0x3)), lcon(0x20));
        }
        else
        {
          if (mkub(RG4Blo) <= mkub(1))
            //m_48CE3:
            and8(location(RG1P + lcon(0x3)), lcon(0xffffffdf));
          else
          {
            if (RG4Blo == 2)
            {
              //m_48CE9:
              RG4W = word_at(RG1P, 2);
              RG4L *= 4;
              RG4UW >>= bcon(0xf);
              RG4Blo ^= 1;
              RG4W &= 0x1;
              and8(location(RG1P + lcon(0x3)), lcon(0xffffffdf));
              RG4L <<= bcon(0xd);
              or16(location(RG1P + 2), RG4W);
            }
          }
        }
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
        skip01032 = true;
        break;

      case 64:
        //m_48B88:
        parl09 = 0;
        skip01028 = true;
        break;

      case 65:
        //m_48BAD:
        parl09 = 1;
        skip01027 = true;
        break;

      case 66:
        //m_48BB1:
        parl09 = 1;
        skip01028 = true;
        break;

      default: throw(THROW_DMABORT);
    }

    if (skip01032)
      continue;

    if (skip01029)
    {
      //m_48BD5:
      RG1W = word_at(RG52p, 2);
      RG1UW >>= bcon(0x7);
      RG1L = DM2_ALLOC_NEW_RECORD(DM2_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(unsignedlong(RG1W)) & lcon(0xffff));
      RG4L = RG1L;
      RG2L = RG1L;
      if (RG1W != lcon(0xffffffff))
      {
        RG1W = word_at(RG52p, 2);
        RG1UW >>= bcon(0x7);
        RG1L = DM2_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(unsignedlong(RG1W)) & lcon(0xff);
        RG3L = unsignedlong(RG4W);
        RG4L = RG1L;
        RG1L = RG3L;
        DM2_SET_ITEMTYPE(RG1L, RG4L);
        RG1W = word_at(RG52p, lcon(0x4)) & lcon(0x18);
        if (RG1W == lcon(0x18))
        {
          RG4L = 1;
          RG1L = RG3L;
          DM2_SET_ITEM_IMPORTANCE(RG1L, RG4L);
        }
        //m_48C3E:
        RG1W = word_at(RG52p, lcon(0x4));
        RG1L <<= bcon(0xd);
        RG1UW >>= bcon(0xf);
        RG1L = unsignedlong(RG1W);
        i32 parl14 = RG1L;
        RG1W = word_at(RG52p, lcon(0x6));
        RG1L <<= bcon(0xa);
        RG1UW >>= 14;
        RG1L = signedlong(RG1W);
        i32 parl12 = RG1L;
        RG1W = word_at(RG52p, lcon(0x6));
        RG4L = RG1L;
        _RG4.ushiftr11();
        RG4L = signedlong(RG4W);
        put16(parw11, RG4W);
        RG1L <<= bcon(0x5);
        _RG1.ushiftr11();
        RG1L = signedlong(RG1W);
        put16(parw10, RG1W);
        RG1L = unsignedlong(RG2W);
        RG3L = lcon(0xffffffff);
        RG2L = RG3L;
        RG4P = RG52p;
        DM2_MOVE_ITEM_TO(RG1L, RG4P, RG2L, RG3L, parw10, parw11, parl12, 0, parl14);
      }
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
      continue;
    }

    if (skip01028)
    {
      //m_48B8A:
      xparp07 = NULL;
      RG3L = 0;
      RG2L = lcon(0xffff);
      DM2_ACTIVATE_ITEM_TELEPORT(RG6timp, RG4P, RG2L, RG3L, 0, xparp07, 1, parl09);
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
      continue;
    }

    if (skip01027)
    {
      //m_48B7E:
      xparp07 = NULL;
      RG3L = 0;
      DM2_ACTIVATE_ITEM_TELEPORT(RG6timp, RG4P, RG2L, RG3L, 0, xparp07, 0, parl09);
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
      continue;
    }

    if (skip01026)
    {
      //m_48923:
      RG1W = word_at(RG52p, 2);
      RG1Blo &= lcon(0xffffff80);
      if (RG1UW > mkuw(lcon(0xffffc800)) && RG1UW < mkuw(lcon(0xfffffa00)))
      {
        RG1W = word_at(RG52p, 2);
        RG1UW >>= bcon(0x7);
        RG4L = unsignedlong(RG1W);
        RG1W = DM2_RAND16(100);
        RG4L -= lcon(0x190);
        RG1Blo = mkul(RG1L) < mkul(RG4L) ? 1 : 0;
        RG1L = unsignedlong(RG1Blo);
        RG4W = word_at(RG52p, lcon(0x4)) & lcon(0x18);
        if (RG4W != lcon(0x18))
        {
          //m_48981:
          if (RG1L != 0)
          {
            RG4L = unsignedlong(RG6timp->getyB());
            RG2L = 0;
            RG1P = RG52p;
            DM2_INVOKE_ACTUATOR(RG1P, RG4L, RG2L);
          }
        }
        else
        {
          RG2L = 0;
          flag = RG1L == 0;
          RG1Blo = flag ? 1 : 0;
          RG1W = unsignedword(RG1Blo);
          RG4L = unsignedlong(RG1W);
          RG1P = RG52p;
          DM2_INVOKE_ACTUATOR(RG1P, RG4L, RG2L);
        }
      }
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
      continue;
    }

    if (skip01025)
      //m_487FC:
      RG1L = 1;

    bool skip01023 = false;
    //m_48805:
    RG2L = RG1L;
    RG1Blo = RG6timp->getyB();
    if (RG1Blo != 1)
    {
      //m_4882E:
      if (RG1Blo == 0)
      {
        if (jz_test8(location(RG52p + lcon(0x4)), lcon(0x4)) || jnz_test16(location(RG52p + 2), lcon(0xffffff80)))
        {
          RG1W = word_at(RG52p, 2);
          RG4L = RG1L;
          RG4UW >>= bcon(0x7);
          RG4L--;
          RG4Bhi &= 0x1;
          RG1L &= lcon(0xffff007f);
          mov16(location(RG52p + 2), RG1W);
          RG1L = RG4L << 7;
          or16(location(RG52p + 2), RG1W);
        }
      }
    }
    else
    {
      RG1W = word_at(RG52p, 2);
      RG4L = RG1L;
      RG4UW >>= bcon(0x7);
      RG4L++;
      RG4Bhi &= 0x1;
      RG1L &= lcon(0xffff007f);
      RG4L <<= bcon(0x7);
      RG3L = RG1L | RG4L;
      mov16(location(RG52p + 2), RG3W);
    }
    //m_48860:
    RG1W = word_at(RG52p, 2);
    if ((RG1W & wcon(0xff80)) != 0)
    {
      RG1UW >>= bcon(0x7);
      RG1W &= lcon(0x100);
      RG1L = unsignedlong(RG1W);
      if (RG1L == 0)
      {
        //m_48881:
        RG1L = 0;
        skip01023 = true;
      }
    }

    if (!skip01023)
      //m_4887A:
      RG1L = 1;

    //m_48883:
    if (RG1W != RG2W)
    {
      RG4W = word_at(RG52p, lcon(0x4)) & lcon(0x18);
      if (RG4W != lcon(0x18))
      {
        //m_488B7:
        if (RG1W != 0)
        {
          RG1W = word_at(RG52p, lcon(0x4));
          RG1L <<= 11;
          RG1UW >>= 14;
          RG4L = unsignedlong(RG1W);
          RG2L = 0;
          RG1P = RG52p;
          DM2_INVOKE_ACTUATOR(RG1P, RG4L, RG2L);
        }
      }
      else
      {
        RG2L = 0;
        RG4W = word_at(RG52p, lcon(0x4));
        RG4L <<= bcon(0xa);
        RG4UW >>= bcon(0xf);
        flag = RG1W == RG4W;
        RG1Blo = flag ? 1 : 0;
        RG1W = unsignedword(RG1Blo);
        RG4L = unsignedlong(RG1W);
        RG1P = RG52p;
        DM2_INVOKE_ACTUATOR(RG1P, RG4L, RG2L);
      }
    }
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
  }
  FOREND
}

// belongs to DM2_ACTUATE_FLOOR_MECHA
// was SKW_13e4_08ff
static void DM2_ANIMATE_CREATURE(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  i16 RG6w;
  i32 RG7l;
  c_ql32 vql_00;
  i16 vw_04;

  vql_00.poke32(RG1L);
  RG6w = RG4W;
  RG7l = RG2L;
  RG2L = unsignedlong(RG4W);
  RG5l = unsignedlong(RG1W);
  RG4L = RG2L;
  RG1L = DM2_GET_CREATURE_AT(RG5l, RG4L);
  vw_04 = RG1W; // truncated
  if (RG1W == lcon(0xffffffff))
    return;
  RG3L = unsignedlong(RG1W);
  RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(RG3L);
  if ((RG1Blo & bcon(0x1)) != 0)
  {
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
    if (byte_at(RG1P, lcon(0x5)) == lcon(0xffffffff))
      DM2_ALLOC_CAII_TO_CREATURE(RG3L, RG5l, RG2L);
  }
  RG2L = signedlong(RG6w);
  RG4L = signedlong(vql_00.peek16());
  RG1P = DM2_PREPARE_LOCAL_CREATURE_VAR(unsignedlong(vw_04), RG4L, RG2L, lcon(0x21));
  RG4P = RG1P;
  if (s350.creatures != NULL)
  {
    if (RG7l != 0)
      //m_154BD:
      DM2_ai_13e4_0806();
    else
      DM2_ai_13e4_071b();
  }
  //m_154C2:
  DM2_UNPREPARE_LOCAL_CREATURE_VAR(RG4P);
}

// belongs to DM2_ACTUATE_FLOOR_MECHA
// stacksize was 0x24
void DM2_ACTIVATE_CREATURE_KILLER(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i32 argl1, i16 argw2, i16 argw3)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_wlreg _RG6;
  i32 vl_00;
  i16 vw_04;
  i32 vl_08;
  i32 vl_0c;
  i16 vw_10;

  RG6L = RG1L;
  vw_04 = RG4W; // truncated
  RG1L = signedlong(DM2_ABS(CUTX16(RG2L - argl0)));
  RG4L = RG1L;
  RG1L = signedlong(DM2_ABS(CUTX16(RG3L - argl1)));
  RG2L -= RG4L;
  argl0 = RG2L;
  RG3L -= RG1L;
  argl1 = RG3L;
  vl_08 = 2 * RG4L + 1;
  RG1L = 2 * RG1L + 1;
  vl_0c = RG1L;
  //m_46C93:
  for (;;)
  {
    RG1L = vl_0c - 1;
    vl_0c = RG1L;
    if (RG1W == lcon(0xffffffff))
      return;
    RG5L = vl_08;
    //m_46CA7:
    for (;;)
    {
      RG5L--;
      if (RG5W == lcon(0xffffffff))
        break;
      RG1L = argl0 + RG5L;
      if (RG1W < 0 || RG1W >= mapdat.map_width)
        continue;
      RG4L = argl1;
      RG3L = vl_0c;
      RG4L += RG3L;
      if (RG4W < 0 || RG4W >= mapdat.map_height)
        continue;
      RG4L = unsignedlong(RG4W);
      RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG1W), RG4L);
      vw_10 = RG1W; // truncated
      RG1Blo = RG1W != wcon(0xffff) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L == 0)
        continue;
      RG4L = unsignedlong(vw_04);
      RG1L = unsignedlong(vw_10);
      vl_00 = RG1L;
      RG1L = DM2_1c9a_09b9(RG1L, RG4L);
      if (RG1L == 0)
        continue;
      RG1L = signedlong(argw2);
      if (RG1L == lcon(0xb))
      {
        if (RG6UW < mkuw(lcon(0x1)))
        {
          //m_46D2B:
          if (RG6W != 0)
            return;
          continue;
        }
        if (RG6UW <= mkuw(lcon(0x1)))
          continue;
        if (RG6W != 2)
          return;
        //m_46D36:
        RG1L = argl1 + RG3L;
        RG2L = signedlong(RG1W);
        RG1L = argl0 + RG5L;
        RG4L = signedlong(RG1W);
        RG3L = lcon(0x13);
        DM2_ai_13e4_0360(vl_00, RG4L, RG2L, RG3L, lcon(0x1));
        continue;
      }
      //m_46D5A:
      if (RG1L != lcon(0x28))
        continue;
      RG1L = RG6L;
      if (argw3 != 0)
        RG1Bhi |= lcon(0xffffff80);
      //m_46D77:
      RG3L = unsignedlong(RG1W);
      RG1L = argl1 + vl_0c;
      RG2L = signedlong(RG1W);
      RG1L = argl0 + RG5L;
      RG4L = signedlong(RG1W);
      DM2_ATTACK_CREATURE(unsignedlong(vw_10), RG4L, RG2L, RG3L, lcon(0x64), 0);
    }
  }
  FOREND
}

static i32 DM2_ACTUATE_FLOOR_MECHA(c_tim* eaxtimp)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_tim* RG5timp = eaxtimp;
  c_wlreg _RG6;
  c_lreg _RG7;
  t_text tarr_00[0xc8];
  c_tim c38_c8;
  i32 vl_d4;
  i32 vl_d8;
  i16 vw_dc;
  i32 vl_e0;
  i16 vw_e4;
  i8 vb_e8;
  i16 parw01;
  i16 parw02;
  i16 parw06;
  i16 parw07;
  i32 parl08;
  unk* xparp09;
  i32 parl10;
  i32 parl11;

  RG1W = unsignedword(RG1Blo);
  RG1Blo = RG5timp->getyA();
  vw_dc = RG1W;
  RG4L = signedlong(vw_dc);
  RG1Blo = RG5timp->getxA();
  vw_e4 = RG1W;
  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(vw_e4, RG4W));
  //m_47566:
  for (;;)
  {
    RG6L = RG1L;
    RG1Blo = RG6W != wcon(0xfffe) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == 0)
      return RG1L;
    RG1L = RG6L;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    if (RG1L > lcon(0x3))
      return RG1L;
    RG1L = unsignedlong(RG1W);
    if (RG1L != 2)
    {
      //m_47802:
      if (RG1L == lcon(0x3))
      {
        bool skip00996 = false;
        RG2L = unsignedlong(RG6W);
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
        RG4P = RG1P;
        RG3P = RG1P;
        RG1W = word_at(RG1P, 2) & lcon(0x7f);
        if (RG1UW < mkuw(lcon(0x3b)))
        {
          bool skip00997 = false;
          //m_478A3:
          if (RG1UW < mkuw(lcon(0x28)))
          {
            //m_478E5:
            if (RG1UW < mkuw(lcon(0x20)))
            {
              //m_478F8:
              if (RG1W == lcon(0xb))
                skip00997 = true;
            }
            else
            {
              if (RG1UW <= mkuw(lcon(0x20)))
                skip00996 = true;
              else
              {
                if (RG1W == lcon(0x27))
                {
                  //m_47903:
                  RG1W = word_at(RG4P, lcon(0x4)) & 1;
                  RG4L = signedlong(RG1W);
                  RG1Blo = RG5timp->getyB();
                  RG1L = DM2_timproc_3a15_1da8(signedlong(RG1W), RG4L);
                  RG1W &= 0x1;
                  and8(location(RG3P + lcon(0x4)), lcon(0xfffffffe));
                  or16(location(RG3P + lcon(0x4)), RG1W);
                }
              }
            }
          }
          else
          {
            if (RG1UW <= mkuw(lcon(0x28)))
              skip00997 = true;
            else
            {
              if (RG1UW < mkuw(lcon(0x2e)))
              {
                //m_478DA:
                if (RG1W == lcon(0x2c))
                {
                  //m_47928:
                  RG4L = 0;
                  RG2P = RG3P;
                  RG3L = RG4L;
                  RG1L = unsignedlong(RG6W);
                  DM2_ACTIVATE_CONTINUOUS_ORNATE_ANIMATOR(RG1L, RG5timp, RG2P, RG3L);
                }
              }
              else
              {
                if (RG1UW <= mkuw(lcon(0x2e)))
                {
                  //m_479D2:
                  if (jz_test8(location(RG4P + lcon(0x4)), lcon(0x4)))
                  {
                    //m_479ED:
                    RG4W = vw_e4;
                    RG1W = vw_dc;
                  }
                  else
                  {
                    RG4W = word_at(RG3P, lcon(0x6));
                    RG4L <<= bcon(0x5);
                    _RG4.ushiftr11();
                    RG1W = word_at(RG3P, lcon(0x6));
                    _RG1.ushiftr11();
                  }
                  //m_479FB:
                  if (jz_test8(location(RG3P + lcon(0x4)), lcon(0x20)))
                  {
                    //m_47A13:
                    RG3W = word_at(RG3P, lcon(0x4));
                    RG3L <<= 11;
                    RG3UW >>= 14;
                    RG2L = unsignedlong(RG3W) + signedlong(ddat.v1e0258) & lcon(0x3);
                  }
                  else
                  {
                    RG3W = word_at(RG3P, lcon(0x4));
                    RG3L <<= 11;
                    RG3UW >>= 14;
                    RG2L = unsignedlong(RG3W);
                  }
                  //m_47A31:
                  vl_d4 = RG2L;
                  RG2L = vl_d4;
                  RG3L = signedlong(RG2W);
                  RG2L = signedlong(ddat.v1d3248);
                  RG1L = signedlong(RG1W);
                  vl_d8 = RG1L;
                  RG1L = signedlong(RG4W);
                  RG4L = vl_d8;
                  DM2_map_3BF83(RG1L, RG4L, RG2L, RG3L);
                }
                else
                {
                  if (RG1UW >= mkuw(lcon(0x32)))
                  {
                    if (RG1UW <= mkuw(lcon(0x32)))
                    {
                      //m_47A67:
                      RG3P = NULL;
                      RG2L = unsignedlong(RG6W);
                      DM2_ACTIVATE_ORNATE_ANIMATOR(RG5timp, RG4P, RG2L, 0);
                    }
                    else
                    {
                      if (RG1W == lcon(0x3a))
                      {
                        //m_47A7A:
                        RG1Blo = RG5timp->getyB() == 0 ? 1 : 0;
                        RG2L = unsignedlong(RG1Blo);
                        RG4L = signedlong(vw_dc);
                        DM2_ANIMATE_CREATURE(signedlong(vw_e4), RG4L, RG2L);
                      }
                    }
                  }
                }
              }
            }
          }

          if (skip00997)
          {
            //m_47971:
            RG4L = unsignedlong(RG5timp->getyB());
            put16(parw07, RG4W);
            RG1L = signedlong(RG1W);
            put16(parw06, RG1W);
            RG1W = word_at(RG3P, lcon(0x6));
            RG4L = RG1L;
            _RG4.ushiftr11();
            RG4L = signedlong(RG4W);
            i32 parl05 = RG4L;
            RG1L <<= bcon(0x5);
            _RG1.ushiftr11();
            RG1L = signedlong(RG1W);
            i32 parl04 = RG1L;
            RG2L = signedlong(vw_e4);
            RG1W = word_at(RG3P, 2);
            RG1UW >>= bcon(0x7);
            RG4L = unsignedlong(RG1W);
            RG4UL >>= bcon(0x4);
            RG4L &= lcon(0x1f);
            RG4L = unsignedlong(RG4W);
            RG1W &= lcon(0xf);
            RG1L = unsignedlong(RG1W);
            RG3L = signedlong(vw_dc);
            DM2_ACTIVATE_CREATURE_KILLER(RG1L, RG4L, RG2L, RG3L, parl04, parl05, parw06, parw07);
          }
        }
        else
        {
          bool skip00998 = false;
          bool skip01000 = false;
          if (RG1UW <= mkuw(lcon(0x3b)))
          {
            //m_47AA0:
            parl11 = 0;
            parl10 = 0;
            xparp09 = NULL;
            parl08 = 0;
            skip00998 = true;
          }
          else
          {
            if (RG1UW < mkuw(lcon(0x44)))
            {
              //m_47873:
              if (RG1UW < mkuw(lcon(0x40)))
              {
                //m_47894:
                if (RG1W == lcon(0x3d))
                {
                  //m_47958:
                  RG1W = word_at(RG4P, 2);
                  RG1UW >>= bcon(0x7);
                  RG2L = unsignedlong(RG1W);
                  DM2_ACTIVATE_RELAY2(RG5timp, RG4P, RG2L);
                }
              }
              else
              {
                if (RG1UW <= mkuw(lcon(0x40)))
                {
                  //m_47AC7:
                  parl11 = 0;
                  parl10 = 0;
                  xparp09 = NULL;
                  parl08 = 1;
                  skip00998 = true;
                }
                else
                {
                  if (RG1UW >= mkuw(lcon(0x42)))
                  {
                    if (RG1UW <= mkuw(lcon(0x42)))
                      //m_47AE1:
                      SKW_3a15_0d5c(RG5timp, RG4P);
                    else
                      //m_47AEA:
                      DM2_ACTIVATE_INVERSE_FLAG(RG5timp, RG4P);
                  }
                }
              }
            }
            else
            {
              if (RG1UW <= mkuw(lcon(0x44)))
                //m_47AF3:
                DM2_ACTIVATE_TEST_FLAG(RG5timp, RG4P);
              else
              {
                if (RG1UW < mkuw(lcon(0x47)))
                {
                  //m_47864:
                  if (RG1W == lcon(0x45))
                    skip00996 = true;
                }
                else
                {
                  bool skip00999 = false;
                  if (RG1UW <= mkuw(lcon(0x47)))
                  {
                    //m_47AAF:
                    parl11 = 0;
                    skip00999 = true;
                  }
                  else
                  {
                    if (RG1UW <= mkuw(lcon(0x48)))
                    {
                      //m_47AD1:
                      parl11 = 1;
                      parl10 = 0;
                      xparp09 = NULL;
                      parl08 = 0;
                      RG3P = NULL;
                      skip01000 = true;
                    }
                    else
                    {
                      if (RG1W == lcon(0x49))
                      {
                        //m_47ADD:
                        parl11 = 1;
                        skip00999 = true;
                      }
                    }
                  }

                  if (skip00999)
                  {
                    //m_47AB1:
                    parl10 = 1;
                    xparp09 = NULL;
                    parl08 = 0;
                    RG3P = NULL;
                    RG2L = lcon(0xffff);
                    skip01000 = true;
                  }
                }
              }
            }
          }

          if (skip00998)
          {
            //m_47AA8:
            RG3L = 1;
            skip01000 = true;
          }

          if (skip01000)
            //m_47ABE:
            DM2_ACTIVATE_ITEM_TELEPORT(RG5timp, RG4P, RG2L, RG3L, parl08, xparp09, parl10, parl11);
        }

        if (skip00996)
        {
          //m_4793F:
          RG1L = signedlong(RG1W);
          RG1Blo = RG1L == lcon(0x45) ? 1 : 0;
          RG2L = unsignedlong(RG1Blo);
          RG4P = RG3P;
          DM2_ACTIVATE_RELAY1(RG5timp, RG4P, RG2L);
        }
      }
    }
    else
    {
      bool skip00992 = false;
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG6UW));
      RG4P = RG1P;
      if (jz_test8(location(RG1P + 2), lcon(0x6)))
        skip00992 = true;
      else
      {
        RG1W = word_at(RG1P, 2) & lcon(0x6);
        if (RG1W == 2)
          skip00992 = true;
      }

      if (skip00992)
      {
        bool skip00993 = false;
        //m_475C4:
        RG2W = word_at(RG4P, 2);
        RG1L = RG2L;
        RG1UW >>= bcon(0x3);
        RG7L = unsignedlong(RG1W);
        RG1L = RG7L;
        RG1UL >>= bcon(0x8);
        RG1L &= lcon(0x1f);
        RG3L = signedlong(RG1W);
        if (RG3L == lcon(0x5))
          skip00993 = true;
        else
        {
          bool skip00994 = false;
          RG2W &= 0x6;
          if (RG2W != lcon(0x4))
            skip00994 = true;
          else
          {
            RG7UL >>= bcon(0x9);
            RG7L &= lcon(0xf);
            if (RG7L != 2)
              skip00994 = true;
          }

          if (skip00994)
          {
            bool skip00995 = false;
            //m_476E0:
            RG2L = signedlong(RG1W);
            if (RG2L != lcon(0x13) && RG2L != lcon(0x16))
              skip00995 = true;
            else
            {
              if (RG5timp->getyB() != 0)
                skip00995 = true;
              else
              {
                RG2L = signedlong(RG1W);
                if (RG2L != lcon(0x13))
                  //m_4770A:
                  RG2L = lcon(0x2000);
                else
                  RG2L = lcon(0x200);
                //m_4770F:
                if (jnz_test8(location(RG4P + 2), 1))
                {
                  RG1L = signedlong(RG1W);
                  if (RG1L != lcon(0x13))
                    //m_47722:
                    RG1L = 2;
                  else
                    RG1L = 4;
                  //m_47727:
                  RG2L *= RG1L;
                }
                //m_4772A:
                RG3L = 2 * RG2L;
                RG1L = DM2_RAND() & lcon(0xffff);
                vl_d8 = RG1L;
                c38_c8.setmticks(ddat.v1d3248, RG2L + ((RG3L - 1) & vl_d8) + timdat.gametick);
                RG3W = unsignedword(RG3Blo);
                c38_c8.setactor(0);
                c38_c8.setxyA(RG5timp->getxA(), RG5timp->getyA());
                c38_c8.setxyB(RG3Bhi, CUTX8(mkuw(word_at(RG4P, 2)) >> 3));
                c38_c8.settype(0x5e);
                DM2_QUEUE_TIMER(&c38_c8);
              }
            }

            if (skip00995)
            {
              //m_477B0:
              RG1L = signedlong(RG1W);
              if (RG1L == lcon(0x17) && RG5timp->getyB() == 0)
              {
                put16(parw02, unsignedword(RG5timp->getyA()));
                RG1L = unsignedlong(RG5timp->getxA());
                put16(parw01, RG1W);
                RG1W = word_at(RG4P, 2);
                RG1UW >>= bcon(0x3);
                RG2L = unsignedlong(RG1Blo);
                RG1Blo = CUTX8(ddat.v1d3248) + 1;
                RG4L = unsignedlong(RG1Blo);
                RG3L = lcon(0x73);
                DM2_QUEUE_NOISE_GEN1(lcon(0x3), RG4Blo, RG2Blo, RG3W, lcon(0xc8), parw01, parw02, 1);
              }
            }
          }
          else
            skip00993 = true;
        }

        if (skip00993)
        {
          //m_475FF:
          RG1W = word_at(RG4P, 2) & 1;
          RG1L = unsignedlong(RG1W);
          RG2P = RG4P + 2;
          RG3Blo = RG5timp->getyB();
          if (RG3Blo != 2)
          {
            //m_4762C:
            RG3Bhi = RG3Blo;
            vb_e8 = RG3Blo == bcon(0x0) ? 1 : 0;
            RG3Bhi ^= RG3Blo;
            RG3Blo = vb_e8;
            RG3W = unsignedword(RG3Blo);
          }
          else
          {
            vb_e8 = RG1L == 0 ? 1 : 0;
            RG3W = unsignedword(RG3Blo);
            RG3Blo = vb_e8;
          }
          //m_47643:
          RG3Blo &= 0x1;
          vl_e0 = RG3L;
          and8(location(RG2P), lcon(0xfffffffe));
          RG3L = vl_e0;
          or16(location(RG2P), RG3W);
          RG2Blo = byte_at(RG4P, 2);
          if ((RG2Blo & bcon(0x6)) == 0 && RG1L == 0 && (RG2Blo & bcon(0x1)) != 0)
          {
            RG1W = ddat.v1d3248;
            if (RG1W == ddat.v1e0282)
            {
              RG4L = unsignedlong(vw_e4);
              RG1L = signedlong(ddat.v1e0280);
              if (RG4L == RG1L)
              {
                RG4L ^= RG1L;
                RG4W = vw_dc;
                RG1L = signedlong(ddat.v1e027e);
                if (RG4L == RG1L)
                {
                  RG4L ^= RG1L;
                  RG4W = RG6W;
                  RG2L = 1;
                  DM2_QUERY_MESSAGE_TEXT(tarr_00, RG4L, RG2L);
                  RG4P = ADRESSOF(t_text, tarr_00);
                  DM2_DISPLAY_HINT_TEXT(lcon(0xf), RG4T);
                }
              }
            }
          }
        }
      }
    }

    //m_47AFA:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG6UW)));
  }
  FOREND
}

static void DM2_ADVANCE_TILES_TIME(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_wlreg _RG63;
  c_wlreg _RG64;
  c_lreg _RG71;
  c_lreg _RG72;
  c_lreg _RG73;
  c_ql32 vql_00;
  c_ql32 vql_04;
  i32 vl_08;
  c_ol32 vol_0c; // poke 32 peek 16 dec32 inc32
  i16 parw00;
  i16 parw01;
  i16 parw02;

  vql_04.poke32(RG1L);
  vql_00.poke32(RG4L);
  RG1W = ddat.v1d3248;
  if (RG1W == ddat.v1e0282)
  {
    RG1L = unsignedlong(vql_04.peek16());
    RG4L = signedlong(ddat.v1e0280);
    if (RG1L == RG4L)
    {
      RG4L = unsignedlong(vql_00.peek16());
      RG1L = signedlong(ddat.v1e027e);
      if (RG4L == RG1L)
      {
        RG2L = signedlong(vql_00.peek16());
        put16(parw00, RG2W);
        RG4L = signedlong(vql_04.peek16());
        RG3L = RG4L;
        DM2_MOVE_RECORD_TO(lcon(0xffff), RG4L, RG2L, RG3L, parw00);
      }
    }
  }
  //m_48D7A:
  RG1L = DM2_GET_CREATURE_AT(unsignedlong(vql_04.peek16()), unsignedlong(vql_00.peek16()));
  if (RG1W != lcon(0xffffffff))
  {
    RG2L = signedlong(vql_00.peek16());
    put16(parw01, RG2W);
    RG4L = signedlong(vql_04.peek16());
    RG1L = unsignedlong(RG1W);
    RG3L = RG4L;
    DM2_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw01);
  }
  //m_48DAD:
  RG4L = signedlong(vql_00.peek16());
  RG1L = DM2_GET_WALL_TILE_ANYITEM_RECORD(signedlong(vql_04.peek16()), RG4W);
  RG5L = RG1L;
  vl_08 = RG1L;
  RG2L = 0;
  vol_0c.poke32(RG2L);
  //m_48DCC:
  for (;;)
  {
    if (RG5W == lcon(0xfffffffe))
      break;
    RG1L = RG5L;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    if (RG1L > lcon(0x4))
      vol_0c.inc32();
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG5UW)));
    RG5L = RG1L;
  }
  //m_48DF8:
  for (;;)
  {
    c_tim* RG11timp;

    for (;;)
    {
      for (;;)
      {
        RG5L = vl_08;
        //m_48DFC:
        for (;;)
        {
          RG1Blo = RG5W != wcon(0xfffe) ? 1 : 0;
          RG1L = unsignedlong(RG1Blo);
          if (RG1L == 0)
            return;
          if (vol_0c.peek16() == 0)
            return;
          RG63L = unsignedlong(RG5W);
          RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG63UW)));
          vl_08 = RG1L;
          RG4L = RG5L;
          RG4W &= lcon(0x3c00);
          RG4L = unsignedlong(RG4W) >> bcon(0xa);
          RG71L = unsignedlong(RG4W);
          if (RG71L > lcon(0x4))
            break;
          RG5L = RG1L;
        }
        //m_48E46:
        RG2L = signedlong(vql_00.peek16());
        put16(parw02, RG2W);
        RG4L = signedlong(vql_04.peek16());
        vol_0c.dec32();
        RG3L = RG4L;
        RG1L = RG63L;
        DM2_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw02);
        if (RG71L != lcon(0xe))
          break;
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG63UW));
        RG11timp = &timdat.timerarray[word_at(RG1P, lcon(0x6))];
        RG4W = ddat.v1e102a & lcon(0x1f);
        RG11timp->mask_wvalueB(0xffe0, 0);
        RG2W = RG11timp->getB();
        RG2L |= RG4L;
        RG11timp->setB(RG2W);
        RG4W = ddat.v1e1028 & lcon(0x1f);
        RG3L = RG2L & lcon(0xfffffc1f);
        RG4L <<= bcon(0x5);
        RG11timp->setB(RG3W | RG4W);
        RG4W = ddat.v1e1024 & lcon(0x3);
        RG11timp->mask_wvalueB(0xf3ff, RG4W << 10);
        RG5L &= lcon(0xffff3fff);
        RG4W = ddat.v1e1026;
        RG4L <<= bcon(0xe);
        RG5L |= RG4L;
        RG11timp->setA(RG5W);
        RG2L = RG11timp->getticks();
        RG4L = unsignedlong(ddat.v1e102c) << bcon(0x18);
        RG2L |= RG4L;
        mov32(location(RG1P), RG2L);
      }
      //m_48F0D:
      if (RG71L == lcon(0xf))
      {
        RG11timp = timdat.timerarray;
        RG4L = 0;
        break;
      }
    }

    //m_48F6F:
    for (;;)
    {
      RG72L = unsignedlong(RG4W);
      RG64L = signedlong(timdat.num_timer_indices);
      if (RG72L >= RG64L)
        break;
      //m_48F1F:
      if (RG11timp->is_type(0x19))
      {
        RG3W = RG11timp->getB();
        if (RG5W == RG3W)
        {
          RG11timp->setxyA(CUTX8(ddat.v1e102a), CUTX8(ddat.v1e1028));
          RG2L = RG3L;
          RG2Bhi &= lcon(0x3f);
          RG64W = ddat.v1e1026;
          RG64L <<= bcon(0xe);
          RG2L |= RG64L;
          RG11timp->setB(RG2W);
          RG11timp->setmticks(ddat.v1e102c, RG11timp->getticks());
        }
      }
      RG11timp++;
      RG4L++;
    }
  }
  FOREND
}

static i32 DM2_ACTUATE_PITFALL(c_tim* eaxtimp)
{
  c_nreg _RG1;
  c_tim* RG2timp = eaxtimp;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  i16 RG6w;

  RG6w = unsignedword(RG2timp->getxA());
  RG1P = DOWNCAST(t_tile*, &mapdat.map[unsignedlong(RG2timp->getxA())]);
  RG5w = unsignedword(RG2timp->getyA());
  RG4L = unsignedlong(RG2timp->getyA());
  RG1P = pointer_at(RG1P) + RG4L;
  RG4W = unsignedword(RG4Blo);
  RG4Blo = RG2timp->getyB();
  RG3L = unsignedlong(RG4Blo);
  if (RG3L == 2)
  {
    RG4Blo = byte_at(RG1P) & lcon(0x8);
    RG4L = unsignedlong(RG4Blo);
    RG4Blo = RG4L != 0 ? 1 : 0;
    RG4W = unsignedword(RG4Blo);
  }
  if (RG4W == 0)
  {
    or8(location(RG1P), lcon(0x8));
    RG4L = unsignedlong(RG5w);
    RG1L = unsignedlong(RG6w);
    DM2_ADVANCE_TILES_TIME(RG1L, RG4L);
    return DM2_ACTUATE_FLOOR_MECHA(RG2timp);
  }
  //m_4906E:
  and8(location(RG1P), lcon(0xfffffff7));
  return DM2_ACTUATE_FLOOR_MECHA(RG2timp);
}

static void DM2_ACTUATE_DOOR(c_tim* eaxtimp)
{
  c_nreg _RG1;
  c_tim* RG2timp = eaxtimp;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  bool skip00991 = false;

  RG1L = unsignedlong(RG2timp->getxA());
  RG4L = 4 * RG1L;
  RG3P = DOWNCAST(t_tile*, &mapdat.map[RG1L]); // adjusted
  RG4L = unsignedlong(RG2timp->getyA());
  RG3Blo = byte_at(pointer_at(RG3P), RG4L) & lcon(0x7);
  RG5w = unsignedword(RG3Blo);
  RG3L = unsignedlong(RG3Blo);
  if (RG3L == lcon(0x5))
    return;
  RG1RP = DM2_GET_ADDRESS_OF_TILE_RECORD(RG1W, RG4W);
  RG3P = RG1P;
  if (jnz_test8(location(RG1P + lcon(0x3)), lcon(0x4)))
  {
    RG1W = word_at(RG1P, 2);
    RG1L <<= bcon(0x6);
    RG1UW >>= bcon(0xf);
    RG4L = signedlong(RG1W);
    RG1L = DM2_timproc_3a15_1da8(unsignedlong(RG2timp->getyB()), RG4L);
    RG1W &= 0x1;
    and8(location(RG3P + lcon(0x3)), lcon(0xfffffffd));
    RG4L = RG1L << bcon(0x9);
    or16(location(RG3P + 2), RG4W);
    if (RG1W == 0)
      and8(location(RG3P + lcon(0x3)), lcon(0xffffffef));
    return;
  }
  RG4L = 0;
  if (RG5w != wcon(0x0))
  {
    //m_46A42:
    if (RG5w != wcon(0x4))
    {
      //m_46A5F:
      RG3P = RG1P + 2;
      RG4Blo = RG2timp->getyB() == bcon(0x0) ? 1 : 0;
      RG4W &= 0x1;
      and8(location(RG3P + 1), lcon(0xfffffffd));
      RG4L <<= bcon(0x9);
      or16(location(RG3P), RG4W);
    }
    else
    {
      RG3Blo = RG2timp->getyB();
      if (RG3Blo != 0 && RG3Blo != 2)
        skip00991 = true;
      else
        or8(location(RG1P + lcon(0x3)), 2);
    }
  }
  else
  {
    RG3Blo = RG2timp->getyB();
    if (RG3Blo != 1 && RG3Blo != 2)
      skip00991 = true;
    else
      and8(location(RG1P + lcon(0x3)), lcon(0xfffffffd));
  }

  if (!skip00991)
    //m_46A78:
    RG4L = 1;

  //m_46A7D:
  RG4W &= 0x1;
  and8(location(RG1P + lcon(0x3)), lcon(0xfffffffb));
  RG3L = RG4L << bcon(0xa);
  or16(location(RG1P + 2), RG3W);
  if (RG4W == 0)
    return;
  RG3Bhi = byte_at(RG1P, lcon(0x3));
  if ((RG3Bhi & bcon(0x2)) == 0)
  {
    RG4Blo = RG3Bhi & lcon(0xffffffef);
    mov8(location(RG1P + lcon(0x3)), RG4Blo);
  }
  RG2timp->settype(0x1);
  DM2_QUEUE_TIMER(RG2timp);
}

static i32 DM2_ACTUATE_TELEPORTER(c_tim* eaxtimp)
{
  c_nreg _RG1;
  c_tim* RG2timp = eaxtimp;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  i16 RG6w;

  RG6w = unsignedword(RG2timp->getxA());
  RG1L = unsignedlong(RG2timp->getxA());
  RG4P = DOWNCAST(t_tile*, mapdat.map);
  RG5w = unsignedword(RG2timp->getyA());
  RG3L = unsignedlong(RG2timp->getyA());
  RG1P = pointer_at(RG4P + 4 * RG1L);
  RG3P = RG1P + RG3L;
  RG4L = signedlong(RG5w);
  RG1RP = DM2_GET_ADDRESS_OF_TILE_RECORD(RG6w, RG4W);
  RG1W = word_at(RG1P, lcon(0x4)) & lcon(0x6);
  if (RG1W == lcon(0x6))
    return DM2_ACTUATE_FLOOR_MECHA(RG2timp);
  RG1Blo = RG2timp->getyB();
  RG4L = unsignedlong(RG1Blo);
  if (RG4L == 2)
  {
    RG1Blo = byte_at(RG3P) & lcon(0x8);
    RG1L = unsignedlong(RG1Blo);
    RG1Blo = RG1L != 0 ? 1 : 0;
    RG1W = unsignedword(RG1Blo);
  }
  if (RG1W == 0)
  {
    or8(location(RG3P), lcon(0x8));
    RG4L = unsignedlong(RG5w);
    RG1L = unsignedlong(RG6w);
    DM2_ADVANCE_TILES_TIME(RG1L, RG4L);
    return DM2_ACTUATE_FLOOR_MECHA(RG2timp);
  }
  //m_48FFE:
  and8(location(RG3P), lcon(0xfffffff7));
  return DM2_ACTUATE_FLOOR_MECHA(RG2timp);
}

static i32 DM2_ACTUATE_TRICKWALL(c_tim* eaxtimp)
{
  c_nreg _RG1;
  c_tim* RG2timp = eaxtimp;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG6w;
  c_lreg _RG71;
  c_lreg _RG72;
  bool flag = false;

  RG6w = unsignedword(RG2timp->getxA());
  RG1L = unsignedlong(RG2timp->getxA()) << 2;
  RG4P = DOWNCAST(t_tile*, mapdat.map);
  RG1P = RG4P + RG1L;
  RG4W = unsignedword(RG4Blo);
  RG4Blo = RG2timp->getyA();
  RG3L = unsignedlong(RG4Blo);
  RG1P = pointer_at(RG1P);
  RG3P = RG1P + RG3L;
  RG1W = unsignedword(RG1Blo);
  RG1Blo = RG2timp->getyB();
  RG71L = unsignedlong(RG1Blo);
  if (RG71L == 2)
  {
    RG1Blo = byte_at(RG3P) & lcon(0x4);
    RG1L = unsignedlong(RG1Blo);
    RG1Blo = RG1L != 0 ? 1 : 0;
    RG1W = unsignedword(RG1Blo);
  }
  RG1L = unsignedlong(RG1W);
  if (RG1L != 1)
    //m_46B85:
    or8(location(RG3P), lcon(0x4));
  else
  {
    RG1W = ddat.v1d3248;
    if (RG1W == ddat.v1e0282)
    {
      RG72L = unsignedlong(RG6w);
      RG1L = signedlong(ddat.v1e0280);
      if (RG72L == RG1L)
      {
        RG1L = unsignedlong(RG4W);
        if (RG1L == signedlong(ddat.v1e027e))
          flag = true;
      }
    }
    RG4L = unsignedlong(RG4W);
    RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG6w), RG4L);
    RG4L = RG1L;
    RG1Blo = RG1W != wcon(0xffff) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 0)
    {
      RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG4W));
      if ((RG1Blo & bcon(0x20)) == 0)
        flag = true;
    }
    if (!flag)
      //m_46B80:
      and8(location(RG3P), lcon(0xfffffffb));
    else
    {
      RG2timp->incdata();
      DM2_QUEUE_TIMER(RG2timp);
    }
  }
  //m_46B88:
  RG1W = ddat.v1d3248;
  if (RG1W == ddat.v1e0266)
    ddat.v1e0390.l_00 = 3;
  return RG1L;
}

// needed in c_savegame.cpp too
// was SKW_3a15_320c
void DM2_PROCESS_TIMER_0E(c_tim* eaxtimp, i32 edxl)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  c_wlreg _RG6;
  i8* ebppb; // allocmemory
  i8* bp_00; // allocmemory
  i16 vw_04;

  vw_04 = RG4W;
  RG5l = unsignedlong(table_recordsizes[unsignedlong(eaxtimp->getA())]);
  RG2L = lcon(0x400);
  RG4L = 0;
  RG1L = RG5l;
  ebppb = DOWNCAST(unk, dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(RG1L));
  RG1W = eaxtimp->getA();
  RG1L <<= bcon(0xa);
  RG6L = unsignedlong(RG1W);
  bp_00 = DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(RG6UW)));
  DM2_COPY_MEMORY(ebppb, bp_00, RG5l);
  DM2_SET_ITEMTYPE(RG6L, unsignedlong(eaxtimp->getxB()));
  DM2_PROCESS_ITEM_BONUS(unsignedlong(eaxtimp->getactor()), RG6L, lcon(0xffffffff), signedlong(vw_04));
  DM2_COPY_MEMORY(bp_00, ebppb, RG5l);
  dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(RG5l);
}

void DM2_PROCEED_TIMERS(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_lreg _RG62;
  c_tim c38_00;
  c_tim* RG5timp = &c38_00;
  i16 parw00;
  bool flag;

  //m_49B1B:
  for (;;)
  {
    for (;;)
    {
      RG1L = DM2_IS_TIMER_TO_PROCEED() ? 1 : 0;
      if (RG1L == 0)
      {
        //m_49FAF:
        DM2_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
        return;
      }
      DM2_IBMIO_USER_INPUT_CHECK();
      DM2_GET_AND_DELETE_NEXT_TIMER(RG5timp);
      RG1L = signedlong(c38_00.getmap());
      DM2_CHANGE_CURRENT_MAP_TO(RG1L);
      RG4W = unsignedword(RG4Blo);
      RG4UBlo = c38_00.gettype();
      RG1L = RG4L;
      if (RG4UW < mkuw(lcon(0x47)))
      {
        //m_49BD4:
        if (RG4UW < mkuw(lcon(0xe)))
        {
          //m_49C3F:
          if (RG4UW >= mkuw(lcon(0x4)))
          {
            if (RG4UW <= mkuw(lcon(0x4)))
              break;
            if (RG4UW < mkuw(lcon(0xc)))
              continue;
            if (RG4UW <= mkuw(lcon(0xc)))
            {
              //m_49D55:
              RG1Blo = c38_00.getactor();
              RG1L = unsignedlong(RG1W);
              DM2_PROCESS_TIMER_0C(RG1W);
              continue;
            }
            //m_49E71:
            DM2_PROCESS_TIMER_RESURRECTION(RG5timp);
            continue;
          }
          //m_49C5C:
          if (RG4UW < mkuw(1))
            continue;
          if (RG4UW <= mkuw(1))
          {
            //m_49C96:
            DM2_STEP_DOOR(RG5timp);
            continue;
          }
          if (RG4W != 2)
            continue;
          //m_49CAE:
          DM2_PROCESS_TIMER_DESTROY_DOOR(RG5timp);
          continue;
        }
        if (RG4UW > mkuw(lcon(0xe)))
        {
          if (RG4UW < mkuw(lcon(0x1d)))
          {
            //m_49C24:
            if (RG4UW < mkuw(lcon(0x15)))
              continue;
            if (RG4UW > mkuw(lcon(0x15)))
            {
              if (RG4W != lcon(0x19))
                continue;
              //m_49CA2:
              DM2_PROCESS_CLOUD(RG5timp);
              continue;
            }
            //m_49D45:
            DM2_PROCESS_SOUND(c38_00.getA());
            continue;
          }
          if (RG4UW <= mkuw(lcon(0x1e)))
          {
            //m_49C8A:
            DM2_STEP_MISSILE(RG5timp);
            continue;
          }
          if (RG4UW < mkuw(lcon(0x3c)))
          {
            //m_49C0F:
            if (RG4UW < mkuw(lcon(0x21)))
              continue;
            if (RG4UW > mkuw(lcon(0x22)))
              continue;
            //m_49C73:
            RG2L = signedlong(RG4W);
            RG1Blo = c38_00.getyA();
            RG4L = signedlong(RG1W);
            RG1Blo = c38_00.getxA();
            RG1L = signedlong(RG1W);
            DM2_THINK_CREATURE(RG1L, RG4L, RG2L);
            continue;
          }
          if (RG4UW <= mkuw(lcon(0x3d)))
          {
            //m_49D39:
            DM2_PROCESS_TIMER_3D(RG5timp);
            continue;
          }
          if (RG4W != lcon(0x46))
            continue;
          //m_49D67:
          DM2_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
          DM2_PROCESS_TIMER_LIGHT(RG5timp);
          DM2_RECALC_LIGHT_LEVEL();
          continue;
        }
        //m_49E7D:
        RG4L = lcon(0xfffffffe);
        DM2_PROCESS_TIMER_0E(RG5timp, RG4L);
        continue;
      }
      if (RG4UW <= mkuw(lcon(0x47)))
      {
        //m_49D85:
        RG3Bhi = ddat.savegames1.b_02;
        flag = --RG3Bhi != 0;
        ddat.savegames1.b_02 = RG3Bhi;
        if (!flag && ddat.v1e0976 != 0)
        {
          RG2L = signedlong(ddat.v1e0976) - 1;
          RG1L = 263 * RG2L;
          party.hero[RG2L].heroflag |= 0x4000;
        }
        continue;
      }
      if (RG4UW < mkuw(lcon(0x58)))
      {
        //m_49B9A:
        if (RG4UW < mkuw(lcon(0x54)))
        {
          //m_49BBB:
          if (RG4UW <= mkuw(lcon(0x48)))
          {
            //m_49DC9:
            RG4Blo = 0;
            //m_49E24:
            for (;;)
            {
              RG2L = signedlong(RG4W);
              RG1L = unsignedlong(party.heros_in_party);
              if (RG2L >= RG1L)
                break;
              //m_49DCD:
              RG1L = unsignedlong(c38_00.getactor());
              RG3L = signedlong(RG4W);
              if (((1 << RG3Blo) & RG1L) != 0)
              {
                RG1L = 263 * RG2L;
                if (party.hero[RG2L].curHP != 0)
                {
                  RG2W = RG5timp->getA();
                  party.hero[RG1L / 263].ench_power -= RG2W;
                }
              }
              RG2L = signedlong(RG4W);
              RG1L = 263 * RG2L;
              if (party.hero[RG2L].ench_power < 0)
              {
                RG2L = 0;
                party.hero[RG1L / 263].ench_power = RG2W;
              }
              RG4L++;
            }
            continue;
          }
          if (RG4W != lcon(0x4b))
            continue;
          //m_49E38:
          RG4Blo = c38_00.getactor();
          RG2L = unsignedlong(RG4Blo);
          RG1L = 263 * RG2L;
          party.hero[RG2L].poisoned--;
          RG1W = c38_00.getA();
          party.hero[RG2L].poison -= RG1W;
          RG2L = unsignedlong(RG1W);
          RG1L = signedlong(RG4W);
          RG4L = RG2L;
          DM2_PROCESS_POISON(RG1L, RG4L);
          continue;
        }
        if (RG4UW <= mkuw(lcon(0x54)))
        {
          //m_49E8E:
          DM2_UPDATE_WEATHER(1);
          continue;
        }
        if (RG4UW < mkuw(lcon(0x56)))
        {
          //m_49E9D:
          DM2_CONTINUE_ORNATE_ANIMATOR(RG5timp);
          continue;
        }
        if (RG4UW <= mkuw(lcon(0x56)))
        {
          //m_49EA9:
          DM2_CONTINUE_TICK_GENERATOR(RG5timp);
          continue;
        }
      }
      else
      {
        if (RG4UW <= mkuw(lcon(0x58)))
        {
          //m_49EB5:
          DM2_PROCESS_TIMER_RELEASE_DOOR_BUTTON(RG5timp);
          continue;
        }
        if (RG4UW < mkuw(lcon(0x5b)))
        {
          //m_49B8B:
          if (RG4UW <= mkuw(lcon(0x59)))
          {
            //m_49EC1:
            DM2_PROCESS_TIMER_59(RG5timp);
            continue;
          }
          //m_49ECD:
          DM2_CONTINUE_ORNATE_NOISE(RG5timp);
          continue;
        }
        if (RG4UW > mkuw(lcon(0x5b)))
        {
          if (RG4UW < mkuw(lcon(0x5d)))
          {
            //m_49EED:
            RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(RG5timp->getA())));
            or8(location(RG1P + 2), 1);
            continue;
          }
          if (RG4UW <= mkuw(lcon(0x5d)))
          {
            //m_49F01:
            RG1Blo = c38_00.getxB();
            if (RG1W == ddat.v1e0266)
            {
              RG1W = c38_00.getA();
              RG1L <<= bcon(0x6);
              _RG1.ushiftr11();
              RG1L = signedlong(RG1W);
              put16(parw00, RG1W);
              RG1W = c38_00.getA() & 0x1f;
              RG3L = signedlong(RG1W);
              RG2L = signedlong(ddat.v1e0272);
              RG4L = signedlong(ddat.v1e0270);
              RG1L = lcon(0xffff);
              DM2_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw00);
              RG1W = c38_00.getA();
              RG1L <<= 4;
              RG1UW >>= 14;
              RG1L = signedlong(RG1W);
              party.rotate(RG1W);
            }
            continue;
          }
          if (RG4W != lcon(0x5e))
            continue;
          //m_49F58:
          i32 parl01 = unsignedlong(c38_00.getyA());
          RG62L = unsignedlong(c38_00.getxA());
          RG1L = signedlong(DM2_RANDDIR());
          if (RG1W == 0)
            //m_49F8F:
            RG1L = signedlong(DM2_RANDDIR());
          else
          {
            RG3L = signedlong(ddat.v1e0272);
            RG2L = signedlong(ddat.v1e0270);
            RG4L = unsignedlong(c38_00.getyA());
            RG1L = signedlong(DM2_CALC_VECTOR_DIR(unsignedword(c38_00.getxA()), RG4W, RG2W, RG3W));
          }
          //m_49F94:
          RG2L = unsignedlong(RG1W);
          RG1L = unsignedlong(c38_00.getyB());
          RG3L = RG62L;
          RG4L = lcon(0x7);
          dm2_dballochandler.DM2_ALLOC_NEW_CREATURE(RG1W, RG4W, RG2W, RG3W, CUTX16(parl01));
          continue;
        }
      }
      //m_49ED9:
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(RG5timp->getA())));
      and8(location(RG1P + lcon(0x4)), lcon(0xfffffffe));
    }
    //m_49CBA:
    RG1P = DOWNCAST(t_tile*, &mapdat.map[unsignedlong(c38_00.getxA())]);
    RG4L = unsignedlong(c38_00.getyA());
    RG1P = pointer_at(RG1P);
    RG1Blo = byte_at(RG1P, RG4L);
    RG1L = unsignedlong(RG1Blo) >> 5;
    if (mkub(RG1Blo) <= mkub(lcon(0x6)))
    {
      RG1L = unsignedlong(RG1Blo);
      switch (RG1L)
      {
        case 0:
          //m_49CF1:
          DM2_ACTUATE_WALL_MECHA(RG5timp);
          break;

        case 1:
          //m_49CFD:
          DM2_ACTUATE_FLOOR_MECHA(RG5timp);
          break;

        case 2:
          //m_49D09:
          DM2_ACTUATE_PITFALL(RG5timp);
        case 3:
          break;

        case 4:
          //m_49D15:
          DM2_ACTUATE_DOOR(RG5timp);
          break;

        case 5:
          //m_49D21:
          DM2_ACTUATE_TELEPORTER(RG5timp);
          break;

        case 6:
          //m_49D2D:
          DM2_ACTUATE_TRICKWALL(RG5timp);
          break;

        default: throw(THROW_DMABORT);
      }
    }
  }
  FOREND
}

// stacksize was 0x18
void DM2_INVOKE_MESSAGE(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_tim c38_00;

  c38_00.setmticks(ddat.v1d3248, argl0);
  c38_00.settype(0x4);
  if (RG3UW < mkuw(1))
  {
    //m_3BCC4:
    if (RG3W == 0)
      //m_3BCD9:
      c38_00.setactor(1);
  }
  else
  {
    if (RG3UW <= mkuw(1))
      //m_3BCCB:
      c38_00.setactor(3);
    else
    {
      if (RG3W == 2)
        //m_3BCD2:
        c38_00.setactor(2);
    }
  }
  //m_3BCDE:
  c38_00.setxyA(RG1Blo, RG4Blo);
  c38_00.setxyB(RG2Blo, RG3Blo);
  DM2_QUEUE_TIMER(&c38_00);
}

void DM2_INVOKE_ACTUATOR(unk* xeaxp, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  RG3W = word_at(RG1P, lcon(0x4));
  RG3L <<= bcon(0x5);
  RG3UW >>= bcon(0xc);
  RG3L = unsignedlong(RG3W) + timdat.gametick;
  RG2L = signedlong(RG2W) + RG3L;
  i32 parl00 = RG2L;
  RG3L = unsignedlong(RG4W);
  RG4W = word_at(RG1P, lcon(0x6));
  RG4L <<= bcon(0xa);
  RG4UW >>= 14;
  RG2L = unsignedlong(RG4W);
  RG1W = word_at(RG1P, lcon(0x6));
  RG4L = RG1L;
  _RG4.ushiftr11();
  RG4L = unsignedlong(RG4W);
  RG1L <<= bcon(0x5);
  _RG1.ushiftr11();
  DM2_INVOKE_MESSAGE(unsignedlong(RG1W), RG4L, RG2L, RG3L, parl00);
}

// belongs to DM2_GAME_LOAD
void DM2_PROCESS_ACTUATOR_TICK_GENERATOR(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  t_record* RG6rp;
  i8* ebppb;
  c_o32 vo_00; // poke 16 peek 32 inc16
  i16 vw_04;
  i16 vw_08;
  c_o32 vo_0c; // poke 16 peek 32 inc16

  RG5w = wcon(0x0);
  //m_4969A:
  for (;;)
  {
    RG4L = unsignedlong(CUTX8(ddat.savegamep4->warr_00[2]));
    RG1L = unsignedlong(RG5w);
    if (RG1L >= RG4L)
      return;
    //m_49562:
    RG4L = 16 * RG1L;
    RG2P = ddat.v1e03c8;
    RG4P = RG2P + RG4L;
    RG2W = word_at(RG4P, lcon(0x8));
    RG2L <<= bcon(0x5);
    _RG2.ushiftr11();
    vw_04 = RG2W; // truncated
    RG4W = word_at(RG4P, lcon(0x8));
    _RG4.ushiftr11();
    vw_08 = RG4W; // truncated
    ebppb = pointer_at(pointer_at(DOWNCAST(t_tile*, mapdat.tmpmap) + 4 * RG1L));
    RG1W = ddat.v1e03e4[RG1L];
    RG1L = unsignedlong(RG1W);
    RG6rp = dm2_v1e038c + word_at(ddat.v1e03d8, 2 * RG1L); // TODO: check, do we have to /2 the index here?
    RG4L = 0;
    vo_00.poke16(RG4W);
    //m_49683:
    for (;;)
    {
      RG1L = signedlong(vo_00.peek16());
      RG4L = unsignedlong(vw_04);
      if (RG1L > RG4L)
      {
        RG5w++;
        break;
      }
      //m_495C7:
      RG2L = 0;
      vo_0c.poke16(RG2W);
      //m_49669:
      for (;;)
      {
        RG1L = signedlong(vo_0c.peek16());
        RG4L = unsignedlong(vw_08);
        if (RG1L > RG4L)
        {
          vo_00.inc16();
          break;
        }
        //m_495D3:
        if ((*ebppb++ & bcon(0x10)) != 0)
        {
          RG3W = record_to_word(*RG6rp++);
          //m_495E5:
          for (;;)
          {
            RG1L = RG3L;
            RG1Blo ^= RG3Blo;
            RG1Bhi &= lcon(0x3c);
            RG1L = unsignedlong(RG1W) >> bcon(0xa);
            if (RG1L > lcon(0x3))
              break;
            if (RG1L == lcon(0x3))
            {
              RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
              RG2P = RG1P;
              RG1W = word_at(RG1P, 2) & lcon(0x7f);
              if (RG1UW >= mkuw(lcon(0x1e)))
              {
                if (RG1UW <= mkuw(lcon(0x1e)) || (RG1UW >= mkuw(lcon(0x33)) && RG1UW <= mkuw(lcon(0x37))))
                {
                  if (ddat.cd.mo.glbSpecialScreen != _MENU_SCREEN__RESUME_GAME_SELECT)
                  {
                    RG4Bhi = byte_at(RG2P, lcon(0x4));
                    if ((RG4Bhi & bcon(0x4)) == 0)
                    {
                      //m_4964F:
                      RG1Blo = RG4Bhi & lcon(0xfffffffe);
                      mov8(location(RG2P + lcon(0x4)), RG1Blo);
                    }
                    else
                    {
                      RG1L = signedlong(RG5w);
                      DM2_CHANGE_CURRENT_MAP_TO(RG1L);
                      RG4L = unsignedlong(RG3W);
                      RG1P = RG2P;
                      DM2_ACTIVATE_TICK_GENERATOR(RG1P, RG4L);
                    }
                  }
                }
              }
            }
            //m_49656:
            RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG3UW)));
            RG3L = RG1L;
          }
        }
        //m_49664:
        vo_0c.inc16();
      }
    }
  }
  FOREND
}

i32 DM2_OPERATE_PIT_TELE_TILE(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  i16 RG6w;
  i16 vw_00;
  bool skip00364 = false;

  RG6w = RG1W;
  vw_00 = RG4W;
  RG5w = RG2W;
  RG3L = 0;
  RG1L = unsignedlong(RG6w);
  RG4P = DOWNCAST(t_tile*, mapdat.map);
  RG2L = unsignedlong(vw_00);
  RG1P = pointer_at(RG4P + 4 * RG1L);
  RG2P = RG1P + RG2L;
  RG1L = unsignedlong(byte_at(RG2P)) >> 5 & lcon(0xff);
  if (RG1L != 2)
  {
    //m_19E42:
    if (RG1L != lcon(0x5))
      return RG3L;
    RG4L = signedlong(vw_00);
    RG1RP = DM2_GET_ADDRESS_OF_TILE_RECORD(RG6w, RG4W);
    RG1W = word_at(RG1P, lcon(0x4)) & lcon(0x6);
    if (RG1W == lcon(0x6))
      return RG3L;
    RG1Blo = byte_at(RG2P) & lcon(0x8);
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 0)
      RG3L = 1;
    RG1L = unsignedlong(RG5w);
    if (RG1L == 2)
    {
      RG1Blo = RG3L != 0 ? 1 : 0;
      RG5w = unsignedword(RG1Blo);
    }
    if (RG5w == 0)
      skip00364 = true;
  }
  else
  {
    RG1Blo = byte_at(RG2P) & lcon(0x8);
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 0)
      RG3L = 1;
    RG1L = unsignedlong(RG5w);
    if (RG1L == 2)
    {
      RG1Blo = RG3L != 0 ? 1 : 0;
      RG5w = unsignedword(RG1Blo);
    }
    if (RG5w == 0)
      skip00364 = true;
  }

  if (skip00364)
  {
    //m_19E20:
    RG1Blo = RG3L == 0 ? 1 : 0;
    RG3L = RG1L & lcon(0xff);
    or8(location(RG2P), lcon(0x8));
    RG4L = unsignedlong(vw_00);
    RG1L = unsignedlong(RG6w);
    DM2_ADVANCE_TILES_TIME(RG1L, RG4L);
    return RG3L;
  }

  //m_19E8C:
  and8(location(RG2P), lcon(0xfffffff7));
  return RG3L;
}

// stacksize was 0x2c
void DM2_TRY_ORNATE_NOISE(unk* xeaxp, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG51p;
  i32 RG52l;
  i16 RG6w;
  c_tim c38_00;
  i16 vw_0c;
  i16 vw_10;
  i8 vb_14;
  i8 vb_18;
  i16 parw00;
  i16 parw01;

  RG51p = RG1P;
  RG6w = RG4W;
  vw_10 = RG2W;
  vw_0c = RG3W;
  if (jz_test8(location(RG1P + lcon(0x4)), lcon(0x40)))
    return;
  RG1W = word_at(RG51p, 2);
  RG1UW >>= bcon(0x7);
  RG1W &= lcon(0x100);
  RG1L = unsignedlong(RG1W);
  if (RG1L != 0)
    return;
  if (argl1 == 0)
  {
    //m_46E66:
    vb_14 = lcon(0xa);
    RG1Blo = DM2_GET_FLOOR_DECORATION_OF_ACTUATOR(RG51p);
  }
  else
  {
    vb_14 = lcon(0x9);
    RG1Blo = DM2_GET_WALL_DECORATION_OF_ACTUATOR(RG51p);
  }
  //m_46E72:
  vb_18 = RG1Blo;
  RG1W = word_at(RG51p, 2);
  RG1UW >>= bcon(0x7);
  RG1W = unsignedword(RG1Blo);
  RG1L = unsignedlong(RG1W);
  RG1L = mkul(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(vb_14, vb_18, 11, lcon(0x88))) + timdat.gametick + RG1L;
  RG2L = unsignedlong(argw0);
  RG4L = RG1L % RG2L;
  RG1L /= RG2L;
  RG2L = unsignedlong(argw0);
  RG1L = unsignedlong(RG4W);
  RG2L -= RG1L;
  c38_00.setmticks(ddat.v1d3248, RG2L + timdat.gametick);
  c38_00.settype(0x5a);
  RG2W = unsignedword(RG2Blo);
  c38_00.setactor(0);
  c38_00.setB(RG6w);
  c38_00.setxyA(CUTX8(vw_10), CUTX8(vw_0c));
  DM2_QUEUE_TIMER(&c38_00);
  RG1W = word_at(RG51p, 2);
  RG2L = RG1L;
  RG2UW >>= bcon(0x7);
  RG2Bhi = 1;
  RG1L &= lcon(0xffff007f);
  mov16(location(RG51p + 2), RG1W);
  RG1L = RG2L << 7;
  or16(location(RG51p + 2), RG1W);
  if (RG4W != 0)
    return;
  if (argl1 != 0)
  {
    RG52l = unsignedlong(RG6w) >> bcon(0xe);
    RG4W = vw_10;
    RG4W += table1d27fc[RG52l];
    RG1W = vw_0c;
    RG1W += table1d2804[RG52l];
    if (ddat.v1d3248 != ddat.v1e0282 || RG4W != ddat.v1e0280 || RG1W != ddat.v1e027e)
    {
      vw_10 = RG4W;
      vw_0c = RG1W;
    }
  }
  RG1L = signedlong(vw_0c);
  put16(parw01, RG1W);
  RG1L = signedlong(vw_10);
  put16(parw00, RG1W);
  DM2_QUEUE_NOISE_GEN2(vb_14, vb_18, lcon(0x88), lcon(0xfe), parw00, parw01, lcon(0x1), lcon(0x8c), lcon(0x80));
}
