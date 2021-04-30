#include "dm2data.h"
#include "util.h"
#include "c_random.h"
#include "c_querydb.h"
#include "c_map.h"
#include "c_gfx_main.h"
#include "c_keybd.h"
#include "c_record.h"
#include "startend.h"
#include "c_gdatfile.h"
#include "c_str.h"
#include "c_light.h"
#include "c_eventqueue.h"
#include "c_tmouse.h"
#include "c_xrect.h"
#include "c_image.h"
#include "c_gui_draw.h"
#include "c_input.h"
#include "c_sfx.h"
#include "c_item.h"
#include "c_gfx_str.h"
#include "c_gfx_blit.h"
#include "c_mcursor.h"
#include "c_1031.h"
#include "c_gui_draw.h"
#include "c_creature.h"
#include "c_events.h"
#include "c_moverec.h"
#include "c_eventqueue.h"
#include "c_item.h"
#include "c_timer.h"
#include "c_1c9a.h"
#include "c_move.h"
#include "c_hero.h"

// TODO: member-functions have new names, tell SPX new names

// ######################################################################

void c_hero::init(void)
{
  for (i16 i = 0; i < HERO_SIZE_1STNAME; i++) name1[i] = 0;
  for (i16 i = 0; i < HERO_SIZE_2NDNAME; i++) name2[i] = 0;
  absdir = 0;
  partypos = 0;
  nrunes = 0;
  poisoned = 0;
  for (i16 i = 0; i < 10; i++) handcmd[i] = 0;
  for (i16 i = 0; i < 4; i++) handcooldown[i] = 0;
  timeridx = 0;
  damagesuffered = 0;
  heroflag = 0;
  bodyflag = 0;
  curHP = 0;
  maxHP = 0;
  curStamina = 0;
  maxStamina = 0;
  curMP = 0;
  maxMP = 0;
  for (i16 i = 0; i < 2; i++) handdefenseclass[i] = 0;
  food = 0;
  water = 0;
  poison = 0;
  for (i16 i = 0; i < NUM_ABILITIES; i++)
    ability[(e_ability)i][E_CUR] = ability[(e_ability)i][E_MAX] = eability[(e_ability)i] = 0;
  for (i16 i = 0; i < 5; i++)
    for (i16 j = 0; i < 4; i++)
      skill[i][j] = sbonus[i][j] = 0;
  for (i16 i = 0; i < NUM_ITEMS; i++) item[i] = 0;
  weight = 0;
  herotype = 0;
  ench_aura = 0;
  ench_power = 0;
  walkspeed = 0;
  b_106 = 0;
}

// was DM2_GET_PLAYER_ABILITY
i16 c_hero::get_adj_ability1(e_ability abi, e_curmax curmax)
{
  i16 a = ability[abi][curmax];
  if (curmax == E_CUR)
  {
    if (ench_power != 0)
    {
      i16 w = unsignedword(ench_aura);
      if (w >= 3 && w <= 6)
      {
        w -= 2;
        if (abi == w)
          a += DM2_RAND16(((DM2_MIN(ench_power, 100) * a) >> 7) + 1) + 4;
      }
    }
  }
  return DM2_BETWEEN_VALUE(10, 220, signedword(eability[abi]) + a);
}

// was DM2_hero_2c1d_0f2a
i32 c_hero::get_adj_ability2(e_ability abi, i16 in)
{
  i16 w = 170 - get_adj_ability1(abi, E_CUR);
  if (w < 16)
    return unsignedlong(in) / 8;
  return DM2_ATIMESB_RSHIFTC(in, 7, w);
}

// was DM2_STAMINA_ADJUSTED_ATTR
i16 c_hero::get_stamina_adj(i16 in)
{
  i16 mhalf = maxStamina >> 1;
  i16 result;
  if (curStamina < mhalf)
  {
    in >>= 1;
    result = ((curStamina * in) / mhalf) + in;
  }
  else
    result = in;

  return result;
}

// was DM2_MAX_LOAD
i16 c_hero::get_max_load(void)
{
  i16 s = get_stamina_adj(8 * get_adj_ability1(E_STRENGTH, E_CUR) + 100);
  if (bodyflag != 0)
    s = s - (s >> (((bodyflag & 0x10) == 0 ? 1 : 0) + 2));
  s += 9;
  return s - (s % 10);
}

// was SKW_2c1d_0f67, DM2_USE_LUCK_ATTRIBUTE
bool c_hero::use_luck(i16 in)
{
  if (DM2_RANDBIT())
    if (DM2_RAND16(100) > in)
      return true;

  bool result = DM2_RAND16(2 * get_adj_ability1(E_LUCK, E_CUR)) > in;
  i16 w2 = DM2_MIN(220, get_adj_ability1(E_LUCK, E_MAX));
  i16 w1 = (result ? -2 : 2) + unsignedword(ability[E_LUCK][E_CUR]);
  ability[E_LUCK][E_CUR] = CUTX8(DM2_BETWEEN_VALUE(10, w2, w1));
  return result;
}

// ######################################################################

c_party party;

void c_party::init(void)
{
  for (i16 i = 0; i < MAX_HEROS; i++)
    hero[i].init();
  heros_in_party = 0;
  absdir = 0;
  curactevhero = E_NOHERO;
  for (i16 i = 0; i < CONTAINERSIZE; i++) hand_container[i] = 0;
  curactmode = 0;
  curacthero = 0;
  for (int i = 0; i < 20; i++) handitems.barr_00[i] = -1;
}

// was DM2_ROTATE_SQUAD
void c_party::rotate(i16 eaxw)
{
  if (eaxw == ddat.v1e0258)
    return;
  i16 turns = eaxw - ddat.v1e0258;
  if (turns < 0)
    turns += 4;
  //m_35961:
  for (i16 i = 0; i < heros_in_party; i++)
  {
    //m_35945:
    hero[i].partypos = (hero[i].partypos + turns) & 0x3;
    hero[i].absdir = (hero[i].absdir + turns) & 0x3;
  }
  ddat.v1e0258 = eaxw;
  if (ddat.v1e0234 != 0)
    absdir = ddat.v1e0264 = (party.absdir + turns) & 0x3;
  else
    ddat.v1e0264 = eaxw;
}

// was SKW_2e62_0cd8, DM2_SET_PARTY_HERO_FLAG, TODO: tell SPX new name
void c_party::set_hero_flags(void)
{
  for (i16 i=0; i<heros_in_party; i++)
    hero[i].heroflag |= 0x4000;
}

// was DM2_GET_PLAYER_WEIGHT
i16 c_party::get_player_weight(e_hero heroidx)
{
  if (hero[heroidx].curHP == 0)
    return 0;
  i16 w = hero[heroidx].weight;
  if (heroidx == eventqueue.event_heroidx)
    w += ddat.savegamewpc.weight;
  return w;
}

// was DM2_CALC_PLAYER_WEIGHT
void c_party::calc_player_weight(e_hero heroidx)
{
  i16 wsum = 0;

  for (i16 i = 0; i < NUM_ITEMS; i++)
    wsum += DM2_QUERY_ITEM_WEIGHT(hero[heroidx].item[i]);

  e_hero h = (e_hero)(curacthero - 1);
  if (heroidx == h && curactmode < 2)
    if (DM2_IS_CONTAINER_CHEST(handitems.warr2d_00[h + 1][curactmode]))
      for (i16 i = 0; i < CONTAINERSIZE; i++)
        wsum += DM2_QUERY_ITEM_WEIGHT(hand_container[i]);

  hero[heroidx].weight = wsum;
  hero[heroidx].heroflag |= 0x1000;
}

// ######################################################################

// TODO optimize temporaries
#include "regs.h"
#include "emu.h"

// stacksize was 0x24
i16 DM2_CALC_PLAYER_ATTACK_DAMAGE(c_hero* hero, i16 heroidx, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1, i16 argw2, i16 argw3, i16 argw4)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4;
  c_wlreg _RG5; // TODO: maybe more
  i32 vl_00;
  i16 vw_04;
  i16 vw_08;
  i16 vw_0c;
  i32 vl_10;
  bool skip00085 = false;

  vw_0c = RG2W; // truncated
  vw_04 = RG3W;
  RG2L = argl1;
  if (heroidx >= party.heros_in_party || hero->curHP == 0)
    return 0;
  RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
  RG1W = word_at(RG1P, lcon(0xc));
  RG1UW >>= bcon(0xc);
  RG1L = 2 * unsignedlong(RG1W);
  vw_08 = RG1W;
  RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(vw_0c)))), lcon(0x4)))));
  RG3P = RG1P;
  RG1Blo = byte_at(RG1P, lcon(0x8));
  RG1L = unsignedlong(RG1Blo);
  if (RG1L == lcon(0xff))
    skip00085 = true;
  else
  {
    bool skip00080 = false;
    RG1L = DM2_GET_CREATURE_AT(unsignedlong(vw_04), unsignedlong(argw0));
    if (RG1W == lcon(0xffffffff))
      skip00080 = true;
    else
    {
      RG1L = DM2_1c9a_0958(unsignedlong(RG1W));
      if (RG1L == 0)
        skip00080 = true;
    }

    if (skip00080)
    {
      //m_8BA8:
      RG1L = RG2L;
      RG1W &= lcon(0x8000);
      RG2Bhi &= lcon(0x7f);
      if (jnz_test8(location(RG3P), lcon(0x20)) && RG1W == 0)
        skip00085 = true;
      else
      {
        bool skip00081 = false;
        RG1L = DM2_RAND();
        RG1W &= lcon(0x1f);
        RG4L = unsignedlong(RG1W);
        RG1L = unsignedlong(byte_at(RG3P, lcon(0x8))) + RG4L;
        RG4L = signedlong(vw_08) + RG1L;
        RG1L = (2 * signedlong(ddat.v1e0286) + RG4L - 16) / 2;
        RG4L = signedlong(RG1W);
        RG1L = mkul(DM2_USE_DEXTERITY_ATTRIBUTE(heroidx) & lcon(0xffff));
        if (RG1L > RG4L)
          skip00081 = true;
        else
        {
          RG1L = signedlong(DM2_RANDDIR());
          if (RG1W == 0)
            skip00081 = true;
          else
          {
            RG1L = lcon(0x4b) - RG2L;
            RG4L = unsignedlong(RG1W);
            RG1L = hero->use_luck(RG4W) ? 1 : 0;
            if (RG1L != 0)
              skip00081 = true;
          }
        }

        if (skip00081)
        {
          bool skip00084 = false;
          bool skip00082 = false;
          //m_8C2A:
          RG2L = signedlong(argw3);
          RG4L = signedlong(party.curactmode);
          RG1W = DM2_COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(heroidx, RG4L, RG2L);
          RG5L = RG1L;
          if (RG1W == 0)
            skip00082 = true;
          else
          {
            RG1W = DM2_RAND16(RG1W / 2 + 1);
            RG5L += RG1L;
            RG4L = signedlong(RG5W);
            RG1L = unsignedlong(argw2) * RG4L >> 5;
            RG5L = RG1L;
            RG1L = DM2_RAND();
            RG1W &= lcon(0x1f);
            RG1L += unsignedlong(byte_at(RG3P, 2));
            RG4L = unsignedlong(vw_08) + RG1L;
            RG1L = DM2_RAND();
            RG1W &= lcon(0x1f);
            RG5L += RG1L;
            RG5L -= RG4L;
            vl_10 = RG5L;
            RG1L = signedlong(RG5W);
            if (RG1L <= 1)
              skip00082 = true;
          }

          if (skip00082)
          {
            //m_8C9A:
            RG1L = signedlong(DM2_RANDDIR());
            if (RG1W == 0)
              skip00085 = true;
            else
            {
              bool skip00083 = false;
              RG5L = RG1L + 1;
              RG1L = DM2_RAND();
              RG1W &= lcon(0xf);
              RG4L = vl_10 + RG1L;
              vl_10 = RG4L;
              if (RG4W > 0)
                skip00083 = true;
              else
              {
                RG1L = DM2_RANDBIT() ? 1 : 0;
                if (RG1W != 0)
                  skip00083 = true;
              }

              if (skip00083)
              {
                //m_8CCD:
                RG5L += signedlong(DM2_RANDDIR());
                RG1L = signedlong(DM2_RANDDIR());
                if (RG1W == 0)
                {
                  RG1L = DM2_RAND();
                  RG1W &= lcon(0xf);
                  RG1L += vl_10;
                  RG4L = signedlong(RG1W);
                  RG5L += signedlong(DM2_MAX(0, RG4W));
                }
              }
              skip00084 = true;
            }
          }
          else
            skip00084 = true;

          if (skip00084)
          {
            //m_8CF7:
            RG5W >>= 1;
            RG1W = DM2_RAND16(RG5W);
            RG4L = RG1L;
            RG1L = signedlong(DM2_RANDDIR()) + RG4L;
            RG5L += RG1L;
            RG1W = DM2_RAND16(RG5W);
            RG5L += RG1L;
            RG5W >>= bcon(0x2);
            RG1L = signedlong(DM2_RANDDIR()) + 1;
            RG5L += RG1L;
            RG1L = DM2_RAND();
            RG1W &= lcon(0x3f);
            RG1L = unsignedlong(RG1W);
            vl_00 = RG1L;
            RG4L = unsignedlong(argw3);
            RG1W = DM2_QUERY_PLAYER_SKILL_LV(heroidx, RG4W, 1) & lcon(0xffff);
            if (RG1W > vl_00)
            {
              RG1W = RG5W + 10;
              RG5W += RG1W;
            }
            RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(hero->item[party.curactmode]), 13);
            RG4L = RG1L;
            if (RG1W != 0)
            {
              RG1L = DM2_RAND();
              RG1W &= lcon(0x1f);
              RG1L = unsignedlong(RG1W);
              RG2L = signedlong(RG5W);
              if (RG2L > RG1L)
              {
                RG4L = unsignedlong(RG4W);
                RG5W += DM2_APPLY_CREATURE_POISON_RESISTANCE(unsignedlong(vw_0c), RG4L);
              }
            }
            RG1L = (unsignedlong(word_at(RG3P, lcon(0x16))) >> bcon(0x8) & lcon(0xf) & lcon(0xffff)) * signedlong(RG5W) >> 4;
            RG2L = RG1L + 3;
            RG4L = unsignedlong(argw3);
            DM2_ADJUST_SKILLS(heroidx, RG4L, RG2L);
            RG1L = signedlong(DM2_RANDDIR()) + 4;
          }
        }
        else
          skip00085 = true;
      }
    }
    else
      skip00085 = true;
  }

  if (skip00085)
  {
    //m_8DE8:
    RG5L = 0;
    RG1L = (DM2_RANDBIT() ? 1 : 0) + 2;
  }

  //m_8DF4:
  RG4L = signedlong(RG1W);
  DM2_ADJUST_STAMINA(heroidx, RG4W);
  if (jnz_test8(location(RG3P + lcon(0x19)), lcon(0x10)))
  {
    RG1L = signedlong(argw4);
    if (RG1L != 1)
    {
      RG1L = (DM2_RANDBIT() ? 1 : 0) + 1;
      RG3L = signedlong(RG1W);
      RG5W >>= RG3Blo;
    }
  }
  RG1L = unsignedlong(RG5W);
  DM2_ATTACK_CREATURE(unsignedlong(vw_0c), signedlong(vw_04), signedlong(argw0), lcon(0x6002), lcon(0x5a), RG1L);
  return RG5W;
}

void DM2_hero_39796(c_hero* hero)
{
  c_nreg _RG1;
  c_nreg _RG3;
  c_nreg _RG2;
  c_nreg _RG4;
  t_text s35_04[HERO_SIZE_2NDNAME];
  t_text s34_18[HERO_SIZE_1STNAME];
  i16 vw_20; // x0 rectB
  i16 vw_24;
  i16 vw_28; // y0 rectA
  i16 vw_2c; // x0 rectA
  i16 vw_30;
  i16 vw_34; // y0 rectB
  c_o32 vo_38; // poke 16 peek32 sub16 add16
  bool flag;

  ddat.v1d66fc = lcon(0x8);
  DM2_COPY_MEMORY(DOWNCAST(t_text, &s34_18[0]), DOWNCAST(t_text, &hero->name1[0]), HERO_SIZE_1STNAME);
  DM2_COPY_MEMORY(DOWNCAST(t_text, &s35_04[0]), DOWNCAST(t_text, &hero->name2[0]), HERO_SIZE_2NDNAME);
  DM2_QUERY_TOPLEFT_OF_RECT(lcon(0x229), OO vw_2c, OO vw_28);
  DM2_QUERY_TOPLEFT_OF_RECT(lcon(0x273), OO vw_20, OO vw_34);
  RG1L = DM2_1031_0675(9);
  ddat.v1e023c = 1;
  DM2_events_38c8_0002();
  //m_39A39:
  for (;;)
  {
    flag = true;
    vw_24 = 7;
    t_text* name = &hero->name1[0];
    i16 tlen = CUTX16(DM2_STRLEN(name));
    RG4L = signedlong(tlen) * signedlong(strdat.gfxstrw4);
    RG1L = signedlong(vw_2c) + RG4L;
    vo_38.poke16(RG1W);
    RG1W = vw_28; // adj
    //m_39A72:
    for (;;)
    {
      vw_30 = RG1W;
      //m_3998A:
      bool skipbreak6 = false;
      for (;;)
      {
        bool skipbreak7 = false;
        DM2_DRAW_STATIC_PIC(7, 0, 20, wcon(0x267), NOALPHA);
        DM2_DRAW_VP_RC_STR(lcon(0x229), unsignedword(palettecolor_to_ui8(paldat.palette[11])), &hero->name1[0]);
        DM2_DRAW_VP_RC_STR(lcon(0x273), unsignedword(palettecolor_to_ui8(paldat.palette[11])), &hero->name2[0]);
        RG2UBlo = palettecolor_to_ui8(paldat.palette[E_COL09]);
        RG2L = unsignedlong(RG2Blo);
        RG4L = signedlong(vw_30);
        DM2_DRAW_VP_STR(vo_38.peek16(), RG4W, RG2W, v1d6a4f);
        DM2_HIDE_MOUSE();
        DM2_DRAWINGS_COMPLETED();
        DM2_SHOW_MOUSE();
        //m_39A0B:
        for (;;)
        {
          RG3L = 0;
          eventqueue.event_unk06 = 0;
          eventqueue.event_unk0a = 0;
          DM2_EVENT_LOOP();
          RG1L = signedlong(eventqueue.event_unk0a);
          if (RG1L == lcon(0x49) && !flag)
          {
            skipbreak6 = true;
            break;
          }
          //m_3980D:
          RG1L = signedlong(eventqueue.event_unk0a);
          if (RG1L != lcon(0x4a) || !flag)
          {
            //m_3985C:
            RG1L = signedlong(eventqueue.event_unk0a);
            if (RG1L == lcon(0x4b))
            {
              DM2_COPY_MEMORY(DOWNCAST(t_text, &hero->name1[0]), DOWNCAST(t_text, &s34_18[0]), HERO_SIZE_1STNAME);
              DM2_COPY_MEMORY(DOWNCAST(t_text, &hero->name2[0]), DOWNCAST(t_text, &s35_04[0]), HERO_SIZE_2NDNAME);
              jump L_fin;
            }
            //m_39893:
            if (eventqueue.event_unk0a == 0)
              continue;
            RG2W = ddat.v1e051e;
            if (RG2W == lcon(0xe))
            {
              if (tlen > 0)
              {
                RG1L = signedlong(--tlen);
                name[RG1L] = '\0';
                vo_38.sub16(strdat.gfxstrw4);
              }
              break;
            }
            //m_398CF:
            if (RG2W != lcon(0x1c) && RG2W != lcon(0xf))
            {
              //m_39913:
              if (tlen < vw_24)
              {
                RG1W = DM2_keybd_476d_05b6(RG2W);
                RG2W = RG1W;
                RG4L = signedlong(RG1W);
                if (RG4L >= lcon(0x61) && RG4L <= lcon(0x7a))
                  RG2L -= lcon(0x20);
                RG4L = 0;
                //m_3994C:
                for (;;)
                {
                  RG1L = signedlong(RG4W);
                  if (RG1L < lcon(0x25))
                  {
                    //m_3993E:
                    RG1Blo = v1d6a08[RG1L];
                    RG1W = unsignedword(RG1Blo);
                    if (RG2W != RG1W)
                    {
                      RG4L++;
                      continue;
                    }
                  }
                  break;
                }
                //m_39954:
                RG1L = signedlong(RG4W);
                if (RG1L == lcon(0x25))
                  continue;
                RG1L = signedlong(RG2W);
                if (RG1L == lcon(0x20) && tlen == 0)
                  continue;
                RG1L = signedlong(tlen++);
                name[RG1L] = RG2Blo;
                RG1L = signedlong(tlen);
                name[tlen] = '\0';
                vo_38.add16(strdat.gfxstrw4);
              }
              break;
            }
            tlen = CUTX16(DM2_STRLEN(name));
            //m_398E4:
            for (;;)
            {
              RG1T = &name[--tlen];
              RG4L = unsignedlong(RG1T[0]);
              if (RG4L != lcon(0x20))
                break;
              RG1T[0] = '\0';
            }
            //m_398F8:
            if (!flag)
            {
              if (ddat.v1e051e != lcon(0xf))
                jump L_fin;
              skipbreak6 = true;
              break;
            }
          }
          //m_3981E:
          flag = false;
          vw_24 = 15;
          tlen = CUTX16(DM2_STRLEN(&hero->name2[0]));
          RG4L = signedlong(tlen) * signedlong(strdat.gfxstrw4);
          RG1L = signedlong(vw_20) + RG4L;
          vo_38.poke16(RG1W);
          RG1W = vw_34; // adj
          skipbreak7 = true;
          break;
        }

        if (skipbreak6 || skipbreak7)
          break;
      }

      if (skipbreak6)
        break;
    }
  }
  FOREND

L_fin:
  ddat.v1e023c = 0;
  DM2_DRAW_STATIC_PIC(7, 0, 19, wcon(0x267), NOALPHA);
  hero->heroflag |= 0x400;
  DM2_REFRESH_PLAYER_STAT_DISP(ddat.v1e0976 - 1);
  DM2_events_38c8_0060();
  DM2_1031_06a5();
}

void DM2_hero_2c1d_0300(c_hero* hero, i32 edxl, i32 ebxl)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG4; RG4L = edxl;

  i16 rg3n = RG4W;
  RG1W = hero->ability[rg3n][E_CUR] - hero->ability[rg3n][E_MAX] + RG2W;
  bool rg5 = RG1W < 0;
  bool rg4 = RG2W < 0;
  if (rg5 == rg4)
  {
    RG1L = signedlong(DM2_ABS(RG1W));
    //m_35ABC:
    for (;;)
    {
      RG4L = signedlong(RG1W);
      if (RG4L <= lcon(0x14))
        break;
      RG4L = signedlong(RG2W);
      RG2L = RG4L / 4;
      RG4L -= RG2L;
      RG2L = RG4L;
      RG1L -= lcon(0x14);
    }
  }
  //m_35AD5:
  RG1L = unsignedlong(hero->ability[rg3n][E_CUR]) + RG2L;
  RG4L = signedlong(RG1W);
  RG2L = lcon(0xdc);
  RG1L = signedlong(DM2_BETWEEN_VALUE(10, RG2W, RG4W));
  hero->ability[rg3n][E_CUR] = RG1Blo;
}

// was SKW_2c1d_1cf3
// stacksize was 0x10
void DM2_SHOOT_CHAMPION_MISSILE(c_hero* hero, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 vw_00;
  i8 parb01;
  i8 parb02;
  i8 parb03;

  vw_00 = RG4W; // truncated
  i16 direction = unsignedword(hero->absdir);
  RG1L = signedlong(DM2_MIN(argw0, wcon(0xff))) & lcon(0xff);
  put8(parb03, RG1Blo);
  RG1L = signedlong(DM2_MIN(RG3W, wcon(0xff))) & lcon(0xff);
  put8(parb02, RG1Blo);
  RG1L = signedlong(DM2_MIN(RG2W, wcon(0xff))) & lcon(0xff);
  put8(parb01, RG1Blo);
  RG1L = signedlong(direction);
  i32 parl00 = RG1L;
  RG4L = unsignedlong(hero->partypos);
  RG1L = unsignedlong(direction);
  RG4L -= RG1L;
  RG4L++;
  RG4L &= 0x2;
  RG4L >>= 1;
  RG1L += RG4L;
  RG1L &= 0x3;
  RG3L = signedlong(RG1W);
  DM2_SHOOT_ITEM(unsignedlong(vw_00), signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), RG3L, parl00, parb01, parb02, parb03);
  ddat.v1e025e = 4;
  ddat.v1e0274 = direction;
}

// was SKW_2c1d_0009 (new)
static void DM2_PROCEED_ENCHANTMENT_SELF(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  i32 RG6l;
  c_ol32 vol_00; // poke 32 peek 8 32 and16
  c_tim c38_04;
  c_o32 vo_10; // poke 16 peek 8 32 inc16
  i16 vw_14;
  i8 vb_18;

  vol_00.poke32(RG1L);
  vb_18 = RG4Blo;
  RG5w = RG2W;
  vw_14 = RG3W; // truncated
  RG6l = 0;
  RG4L = 0;
  vo_10.poke16(RG4W);
  //m_35819:
  for (;;)
  {
    RG2L = signedlong(vo_10.peek16());
    if (RG2L >= lcon(0x4))
    {
      if (RG6l != 0)
        RG5w >>= bcon(0x2);
      RG2L = 0;
      vo_10.poke16(RG2W);
      RG4Blo = vb_18;
      break;
    }
    //m_35729:
    RG3Blo = vo_10.peek8();
    RG4L = lcon(0x1) << RG3Blo;
    RG3L = signedlong(vol_00.peek16());
    RG1L = signedlong(RG4W);
    if ((RG1L & RG3L) != 0)
    {
      if (party.hero[RG2L].curHP == 0)
      {
        RG4L = ~RG4L;
        vol_00.and16(RG4W);
      }
      RG2L = signedlong(vo_10.peek16());
      RG1L = 263 * RG2L;
      RG4Blo = vb_18;
      if (RG4Blo != party.hero[RG2L].ench_aura || party.hero[RG2L].curHP == 0)
      {
        party.hero[vo_10.peek16()].ench_power = 0;
        RG2L = 0;
        RG4L = 0;
        c_tim* RG3timp = timdat.timerarray;
        //m_357E5:
        while (RG4W < timdat.num_timer_indices)
        {
          //m_357B1:
          if (RG3timp->is_type(0x48))
          {
            RG1L = unsignedlong(RG3timp->getactor());
            RG2L = signedlong(vol_00.peek16());
            if ((RG2L & RG1L) != 0)
            {
              RG2L = ~RG2L;
              if ((RG2L & RG1L) != 0)
                //m_357D9:
                RG3timp->and_actor(~vol_00.peek8());
              else
                DM2_DELETE_TIMER(RG4W);
            }
          }
          //m_357E1:
          RG3timp++;
          RG4L++;
        }
      }
    }
    //m_357EE:
    RG2L = signedlong(vo_10.peek16());
    RG1L = signedlong(party.hero[RG2L].ench_power);
    if (RG1L > 50)
      RG6l = 1;
    vo_10.inc16();
  }
  //m_35875:
  for (;;)
  {
    RG2L = signedlong(vo_10.peek16());
    RG1L = unsignedlong(party.heros_in_party);
    if (RG2L >= RG1L)
    {
      c38_04.setA(RG5w);
      c38_04.settype(0x48);
      RG1Blo = vol_00.peek8();
      c38_04.setactor(vol_00.peek8());
      c38_04.setmticks(ddat.v1e0266, unsignedlong(vw_14) + timdat.gametick);
      DM2_QUEUE_TIMER(&c38_04);
      return;
    }
    //m_3583E:
    RG3L = signedlong(vo_10.peek16());
    RG1L = lcon(0x1) << RG3Blo;
    RG3L = signedlong(vol_00.peek16());
    if ((RG1L & RG3L) != 0)
    {
      RG1L = 263 * RG2L;
      party.hero[RG2L].ench_aura = RG4Blo;
      party.hero[RG2L].ench_power += RG5w;
    }
    vo_10.inc16();
  }
  FOREND
}

bool DM2_hero_2c1d_0186(c_hero* hero, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  bool flag = true;

  if (RG3W != 0)
  {
    if (hero->curMP == 0)
      return false;
    RG3L = signedlong(hero->curMP);
    if (RG3L >= 4)
      //m_358F4:
      hero->curMP -= 4;
    else
    {
      RG2UW >>= bcon(0x1);
      hero->curMP = 0;
      flag = false;
    }
  }
  //m_358F9:
  RG3L = unsignedlong(RG2W);
  RG1L = RG3L >> 5;
  RG2L = signedlong(RG1W);
  RG4L = unsignedlong(RG4Blo);
  DM2_PROCEED_ENCHANTMENT_SELF(lcon(0xf), RG4L, RG2L, RG3L);
  return flag;
}

// belongs to DM2_BRING_CHAMPION_TO_LIFE
static void R_36EFE(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  bool skip00781 = false;

  RG2L = unsignedlong(RG1W);
  c_hero* hero = &party.hero[RG2L];
  RG3W = hero->curHP;
  hero->curHP = 0;
  RG1L = signedlong(hero->partypos);
  if (RG1L != lcon(0xffffffff))
  {
    RG1W = DM2_GET_PLAYER_AT_POSITION(unsignedword(hero->partypos));
    if (RG1W == -1)
      skip00781 = true;
  }

  if (!skip00781)
  {
    //m_36F3E:
    RG4L = 0;
    //m_36F40:
    for (;;)
    {
      RG1W = DM2_GET_PLAYER_AT_POSITION((ddat.v1e0258 + RG4W) & 0x3);
      if (RG1W == -1)
        break;
      RG4L++;
    }
    //m_36F5F:
    RG1Blo = (CUTX8(ddat.v1e0258) + RG4Blo) & 0x3;
    hero->partypos = RG1Blo;
  }

  //m_36F6B:
  hero->absdir = CUTX8(ddat.v1e0258);
  hero->curHP = RG3W;
}

// was SKW_2f3f_06fe
void DM2_BRING_CHAMPION_TO_LIFE(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG4;

  RG1L = unsignedlong(RG1W);
  c_hero* hero = &party.hero[RG1L];
  R_36EFE(RG1L);
  hero->weight = 0;
  RG1L = 0;
  //m_39DA3:
  for (;;)
  {
    RG4L = unsignedlong(RG1W);
    if (RG4L >= lcon(0x1e))
    {
      RG1W = hero->maxHP;
      RG1L = unsignedlong(RG1W);
      RG4L = RG1L >> 6;
      RG1L -= RG4L;
      RG1L--;
      RG4L = signedlong(RG1W);
      RG1L = signedlong(DM2_MAX(25, RG4W));
      hero->maxHP = RG1W;
      RG1L = signedlong(RG1W) / 2;
      hero->curHP = RG1W;
      hero->heroflag |= 0x4000;
      hero->ench_aura = 0;
      hero->ench_power = 0;
      DM2_107B0();
      DM2_REFRESHMOUSE();
      return;
    }
    //m_39D98:
    hero->item[RG4L] = -1;
    RG1L++;
  }
  FOREND
}

// belongs to DM2_SELECT_CHAMPION()
static void DM2_REVIVE_PLAYER(i8 htype, i8 direction)
{
  t_text tbuffer[128];

  c_hero* hero = &party.hero[party.heros_in_party];
  hero->init();
  hero->herotype = htype;
  hero->handcmd[0] = hero->handcmd[1] = -1;
  hero->timeridx = -1;
  hero->absdir = direction;

  i16 k = 0;
  //m_39353:
  while (DM2_GET_PLAYER_AT_POSITION((k + ddat.v1e0258) & 0x3) != -1)
    k++;
  //m_3937F:
  hero->partypos = (CUTX8(k) + CUTX8(ddat.v1e0258)) & 0x3;

  hero->b_28 = CUTX8(ddat.v1e0258);

  //m_393BA:
  for (i16 i=0; i<30; i++)
    //m_393A5:
    hero->item[i] = -1;

  t_text* tp = DM2_QUERY_GDAT_TEXT(22, htype, 24, tbuffer);

  //m_393EF:
  t_text c1;
  i16 n1 = 0;
  for (;;)
  {
    c1 = *tp++;
    if (c1 == ' ' || c1 == 0)
      break;
    if (n1 >= 7)
      break;
    hero->name1[n1++] = c1;
  }

  //m_39421:
  if (c1 != 0)
  {
    i16 n2 = 0;
    //m_39430:
    do
    {
      t_text c2 = *tp++;
      if (c2 == 0)
        break;
      hero->name2[n2++] = c2;
    } while (n2 != 19);
  }

  //m_3945D:
  i16* dp = UPCAST(i16, DM2_QUERY_GDAT_ENTRY_DATA_PTR(22, htype, 8, 0));
  hero->curHP = hero->maxHP = dp[0];
  hero->curStamina = hero->maxStamina = dp[1];
  hero->curMP = hero->maxMP = dp[2];

  //m_394CC:
  for (i16 i=0; i<7; i++)
    //m_394A8:
    hero->ability[i][E_CUR] = hero->ability[i][E_MAX] = CUTX8(DM2_MAX(30, dp[i + 3]));

  //m_3951D:
  for (i16 i=4; i<=19; i++)
  {
    //m_394E7:
    i32 v = 0;
    if (dp[i + 6] != 0)
      v = 0x40 << CUTX8(dp[i + 6]);
    //m_39504:
    hero->skill[i / 4][i % 4] = v;
  }

  //m_39573:
  for (i16 j=0; j<4; j++)
  {
    //m_39538:
    i32 sum = 0;
    //m_39550:
    for (i16 i=0; i<4; i++)
      //m_39544:
      sum += hero->skill[(j + 1)][i];
    hero->skill[0][j] = sum;
  }
  hero->food = unsignedword(CUTLX8(DM2_RAND())) + 1500;
  hero->water = unsignedword(CUTLX8(DM2_RAND())) + 1500;
}

// was SKW_2f3f_0343
void DM2_SELECT_CHAMPION(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_ql32 vql_00;
  c_ol32 vol_04; // poke 32 peek 32 add16(0x4, RG1W);
  c_ol32 vol_08; // poke 32 peek 32 add16(0x8, RG4W);
  i8 vb_0c;
  i16 vw_10;

  vol_08.poke32(RG1L);
  vol_04.poke32(RG4L);
  vql_00.poke32(RG2L);
  if (ddat.savegamewpc.w_00 != wcon(0xffff))
    return;
  RG5W = party.heros_in_party;
  if (RG5W >= 4)
    return;
  //m_395DC:
  vw_10 = ddat.v1d3248;
  RG1L = signedlong(RG3W);
  DM2_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = signedlong(vql_00.peek16());
  RG4W = table1d27fc[RG1L];
  vol_08.add16(RG4W);
  RG1W = table1d2804[RG1L];
  vol_04.add16(RG1W);
  RG4L = signedlong(vol_04.peek16());
  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(vol_08.peek16(), RG4W));
  //m_39623:
  for (;;)
  {
    RG2L = RG1L;
    if (RG2W == lcon(0xfffffffe))
      break;
    RG1L = RG2L;
    RG1Blo ^= RG2Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    if (RG1L == lcon(0x3))
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
      RG4W = word_at(RG1P, 2) & lcon(0x7f);
      if (RG4W == lcon(0x7e))
      {
        RG1W = word_at(RG1P, 2);
        RG1UW >>= bcon(0x7);
        vb_0c = RG1Blo;
        break;
      }
    }
    //m_39667:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG2UW)));
  }
  //m_39673:
  RG2L = signedlong(vql_00.peek16());
  RG1L = RG2L + 2 & lcon(0x3);
  RG4W = table1d27fc[RG1L];
  vol_08.add16(RG4W);
  RG1W = table1d2804[RG1L];
  vol_04.add16(RG1W);
  RG4L = signedlong(RG5W);
  DM2_REVIVE_PLAYER(vb_0c, RG2Blo);
  if (RG5W != 0)
  {
    DM2_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
    DM2_UPDATE_RIGHT_PANEL(0);
  }
  ddat.v1e0288 = RG5W + 1;
  party.heros_in_party++;
  if (RG5W == 0)
    DM2_SELECT_CHAMPION_LEADER(0);
  // hmmm, from here on RG5W is one less than heros_in_party, so really a heroidx (?!?)
  i16 heroidx = (vql_00.peek16() + 2) & 0x3;
  RG1L = signedlong(vql_00.peek16());
  RG4W = table1d27fc[RG1L];
  vol_08.add16(RG4W);
  RG1W = table1d2804[RG1L];
  vol_04.add16(RG1W);
  RG4L = signedlong(vol_04.peek16());
  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(vol_08.peek16(), RG4W));
  //m_39712:
  for (;;)
  {
    RG2L = RG1L;
    if (RG2W == lcon(0xfffffffe))
      break;
    RG1L = RG2L;
    RG1Blo ^= RG2Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> 10;
    if (RG1L > 3)
    {
      RG4L = unsignedlong(RG2W);
      RG3L = RG4L >> 14;
      if (RG3L == unsignedlong(heroidx))
        DM2_ADD_ITEM_TO_PLAYER((e_hero)RG5W, RG4W);
    }
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG2UW)));
  }
  //m_39757:
  if (ddat.v1d6a2d == 0)
  {
    DM2_guidraw_24a5_1798(RG5W); // not argtype not e_hero!
    DM2_1031_0541(7);
    DM2_events_38c8_0002();
  }
  DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_10));
  party.calc_player_weight((e_hero)RG5W);
}

i32 DM2_ADJUST_SKILLS(i16 heroidx, i32 edxl, i32 ebxl)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_ol32 vol_00; // poke 32 peek 8 32 add32
  i16 vw_04;
  c_ql32 vql_08;
  t_text tarr_0c[0x80];
  i32 vl_8c;
  i32 vl_90; // inc32
  i8 vb_94;
  i32 vl_98;
  i8 vb_9c;

  vql_08.poke16(heroidx);
  vw_04 = RG4W; // truncated
  vol_00.poke32(RG2L);
  RG1L = unsignedlong(RG4W);
  if (RG1L >= lcon(0x4))
  {
    if (RG1L <= lcon(0xb))
    {
      RG1L = timdat.gametick - lcon(0x96);
      if (mkul(RG1L) > mkul(ddat.v1d26a4))
      {
        RG2UL >>= bcon(0x1);
        vol_00.poke32(RG2L);
      }
    }
  }
  if (vol_00.peek32() == 0)
    return RG1L;
  RG3W = ddat.v1e03c0->w_0c;
  RG3UW >>= bcon(0xc);
  if (RG3W != 0)
  {
    RG1L = unsignedlong(RG3W);
    RG4L = vol_00.peek32() * RG1L;
    vol_00.poke32(RG4L);
  }
  RG4L = unsignedlong(vql_08.peek16());
  c_hero* hero = &party.hero[RG4L];
  i16 c = vw_04;
  if (c >= 4)
  {
    c -= 4;
    c >>= 2;
  }
  RG3W = DM2_QUERY_PLAYER_SKILL_LV(vql_08.peek16(), c, 0);
  RG1L = unsignedlong(vw_04);
  if (RG1L >= lcon(0x4))
  {
    RG1L = timdat.gametick - lcon(0x28);
    if (RG1L < ddat.v1d26a4)
    {
      RG1L = 2 * vol_00.peek32();
      vol_00.poke32(RG1L);
    }
  }
  RG4L = unsignedlong(vw_04);
  RG1L = RG4L;
  RG2L = vol_00.peek32();
  hero->skill[RG1L / 4][RG1L % 4] += RG2L;
  if (RG4L >= 4)
  {
    RG1L ^= RG4L;
    RG1W = c;
    hero->skill[RG1L / 4][RG1L % 4] += RG2L;
  }
  RG1W = DM2_QUERY_PLAYER_SKILL_LV(vql_08.peek16(), c, 0) & lcon(0xffff);
  vol_00.poke16(RG1W);
  vl_90 = RG3L;
  vl_8c = vol_00.peek32();
  //m_364B9:
  for (;;)
  {
    RG1L = vl_90;
    RG4L = vl_8c;
    if (RG1UW >= RG4UW)
      return RG1L;
    RG1L = unsignedlong(RG4W);
    vol_00.poke32(RG1L);
    vl_90++;
    RG2L = (DM2_RANDBIT() ? 1 : 0);
    RG1L = (DM2_RANDBIT() ? 1 : 0) + 1;
    vb_94 = RG1Blo;
    RG1L = DM2_RANDBIT() ? 1 : 0;
    RG3L = RG1L;
    RG4L = unsignedlong(c);
    if (RG4L != 2)
      RG3L = vol_00.peek32() & RG1L;
    hero->ability[E_VITALITY][E_MAX] += RG3Blo;
    RG3W = hero->maxStamina;
    RG1L = DM2_RANDBIT() ? 1 : 0;
    RG1Bhi = vol_00.peek8();
    RG1Bhi = ~RG1Bhi;
    RG1Blo &= RG1Bhi;
    hero->ability[E_ANTIFIRE][E_MAX] += RG1Blo;
    if (c <= 3)
    {
      bool skip00772 = false;
      RG1L = unsignedlong(c);
      switch (RG1L)
      {
        case 0:
          //m_36537:
          vb_9c = 6;
          RG3UW >>= bcon(0x4);
          RG4L = vol_00.peek32();
          RG1L = 3 * RG4L;
          vol_00.poke32(RG1L);
          RG1Blo = vb_94;
          hero->ability[E_STRENGTH][E_MAX] += RG1Blo;
          hero->ability[E_DEXTERITY][E_MAX] += RG2Blo;
          break;

        case 1:
          //m_36562:
          vb_9c = lcon(0x7);
          vl_98 = lcon(0x15);
          RG4L = 0;
          RG1L = RG3L;
          RG3L = vl_98;
          RG4W = RG1W % RG3W;
          RG1W /= RG3W;
          RG3L = RG1L;
          vol_00.poke32(2 * vol_00.peek32());
          hero->ability[E_STRENGTH][E_MAX] += RG2Blo;
          RG1Blo = vb_94;
          hero->ability[E_DEXTERITY][E_MAX] += RG1Blo;
          break;

        case 2:
          //m_365CB:
          vb_9c = lcon(0x8);
          vl_98 = lcon(0x19);
          RG4L = 0;
          RG1L = RG3L;
          RG3L = vl_98;
          RG4W = RG1W % RG3W;
          RG1W /= RG3W;
          RG3L = RG1L;
          RG1L = vol_00.peek32();
          hero->maxMP += RG1W;
          RG1L = vol_00.peek32() + 1;
          RG1UL >>= bcon(0x1);
          vol_00.add32(RG1L);
          hero->ability[E_WIZARDRY][E_MAX] += RG2Blo;
          skip00772 = true;
          break;

        case 3:
          //m_3659F:
          vb_9c = lcon(0x9);
          RG3UW >>= bcon(0x5);
          RG4L = vol_00.peek32();
          RG4UL >>= bcon(0x1);
          RG4L += vol_00.peek32();
          RG1L = signedlong(hero->maxMP) + RG4L;
          hero->maxMP = RG1W;
          RG1Blo = vb_94;
          hero->ability[E_WIZARDRY][E_MAX] += RG1Blo;
          skip00772 = true;
          break;

        default: throw(THROW_DMABORT);
      }

      if (skip00772)
      {
        //m_36601:
        RG1L = vl_8c - 1;
        RG4L = signedlong(RG1W);
        RG1L = signedlong(DM2_MIN(CUTX16(signedlong(DM2_RANDDIR())), RG4W));
        hero->maxMP += RG1W;
        RG1L = signedlong(hero->maxMP);
        if (RG1L > lcon(0x384))
          hero->maxMP = 0x384;
        RG1L = signedlong(DM2_RANDDIR());
        hero->ability[E_ANTIMAGIC][E_MAX] += RG1Blo;
      }
    }

    //m_36636:
    RG1L = vol_00.peek32();
    RG1UL >>= bcon(0x1);
    RG1L++;
    RG1W = DM2_RAND16(RG1W) + vol_00.peek16();
    hero->maxHP += RG1W;
    RG1L = signedlong(hero->maxHP);
    if (RG1L > 0x3e7)
      hero->maxHP = 0x3e7;
    RG1W = DM2_RAND16(RG3W / 2 + 1) + RG3W;
    hero->maxStamina += RG1W;
    RG1L = signedlong(hero->maxStamina);
    if (RG1L > lcon(0x270f))
      hero->maxStamina = wcon(0x270f);
    hero->heroflag |= 0x3800;
    i32 rg7 = unsignedlong(vql_08.peek16());
    RG4L = rg7;
    ddat.v1e0ffa[unsignedlong(c) + 4 * RG4UL + 2]++; // EVIL offset shifted by 2, was 0x1e0ffc
    DM2_DISPLAY_HINT_NEW_LINE();
    RG1L = vql_08.peek32();
    ddat.v1e098c = RG1W;
    RG2L = unsignedlong(vb_9c);
    RG3P = ADRESSOF(t_text, tarr_0c);
    RG4L ^= rg7;
    RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x1), RG4Blo, RG2Blo, RG3T);
    RG2L = unsignedlong(table1d69d0[rg7]);
    DM2_DISPLAY_HINT_TEXT(RG2L, RG1T);
  }
  FOREND
}

// belongs to DM2_hero_2c1d_135d
static i32 DM2_hero_2c1d_132c(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG4; RG4L = edxl;

  RG2L = RG1L;
  RG1W = unsignedword(RG1Blo);
  if (RG4L != 0)
  {
    RG2Blo ^= RG1Blo;
    RG2Bhi &= lcon(0x7);
    RG4L = (unsignedlong(RG2W) >> bcon(0x8)) + lcon(0x4);
    RG2L = unsignedlong(RG4W);
    RG1L = DM2_ATIMESB_RSHIFTC(RG1W, 3, RG2W);
  }
  return RG1L;
}

// belongs to DM2_WOUND_PLAYER()
static i32 DM2_hero_2c1d_135d(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_ol32 vol_00; // poke 32 peek 16 32 and16
  c_ql32 vql_04;
  i16 vw_08;
  i16 vw_0c;

  vql_04.poke32(RG1L);
  vol_00.poke32(RG4L);
  RG4L = signedlong(vql_04.peek16());
  c_hero* hero = &party.hero[RG4L];
  RG1L = vol_00.peek32();
  RG1W &= lcon(0x8000);
  vw_0c = RG1W;
  if (RG1W != 0)
    vol_00.and16(lcon(0x7fff)); // adj
  RG2L = 0;
  vw_08 = RG2W;
  RG5L = 0;
  //m_36D5C:
  for (;;)
  {
    RG3L = signedlong(RG5W);
    if (RG3L > 1)
    {
      RG2L = 0;
      RG4L = 4;
      RG1W = DM2_RAND16((hero->get_adj_ability1((e_ability)RG4W, (e_curmax)RG2W)) / 8 + 1);
      RG5L = RG1L;
      if (vw_0c != 0)
        RG5W >>= bcon(0x1);
      RG4L = signedlong(vql_04.peek16());
      if (party.hero[RG4L].ench_aura == 2)
        RG5W += party.hero[RG4L].ench_power;
      RG4W = signedword(hero->handdefenseclass[0]);
      RG1W = signedword(hero->handdefenseclass[1]);
      RG1L += RG4L;
      RG1L += unsignedlong(vw_08);
      RG5L += RG1L;
      RG1L = unsignedlong(vol_00.peek16());
      if (RG1L > 1)
      {
        RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(hero->item[RG1L]), lcon(0xb));
        RG4L = unsignedlong(vw_0c);
        RG1L = DM2_hero_2c1d_132c(unsignedlong(RG1W), RG4L);
        RG5L += RG1L;
      }
      RG4L = unsignedlong(hero->bodyflag);
      RG3L = vol_00.peek32();
      RG1L = 1 << RG3Blo;
      if ((RG1L & RG4L) != 0)
      {
        RG1L = signedlong(DM2_RANDDIR()) + 8;
        RG5L -= RG1L;
      }
      if (ddat.v1e0238 != 0)
        RG5W >>= bcon(0x1);
      RG1L = signedlong(RG5W) / 2;
      RG4L = signedlong(RG1W);
      return signedlong(DM2_BETWEEN_VALUE(0, 100, RG4W));
    }
    //m_36CD5:
    RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(hero->item[RG3L]), lcon(0xb));
    i16 wv = RG1W;
    RG1W &= lcon(0x8000);
    RG1L = unsignedlong(RG1W);
    if (RG1L != 0)
    {
      RG1W = DM2_COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(vql_04.peek16(), RG3L, 7);
      RG2L = unsignedlong(RG1W);
      RG4L = unsignedlong(vw_0c);
      RG1L = DM2_hero_2c1d_132c(unsignedlong(wv), RG4L);
      RG4L = unsignedlong(RG1W) + RG2L;
      RG1L = unsignedlong(vol_00.peek16());
      RG2L = unsignedlong(table1d69aa[RG1L]);
      RG4L *= RG2L;
      RG1Blo = RG3L != RG1L ? 1 : 0;
      RG3L = unsignedlong(RG1Blo) + lcon(0x4);
      RG1L = RG4L >> RG3Blo;
      RG3L = signedlong(vw_08) + RG1L;
      vw_08 = RG3W;
    }
    //m_36D5B:
    RG5L++;
  }
  FOREND
}

i16 DM2_WOUND_PLAYER(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG52;
  c_ql32 vql_00;
  i32 vl_04;
  i16 vw_08;
  c_o32 vo_0c; // poke16 peek16 peek32 and16
  c_ql32 vql_10;
  c_ol32 vol_14; // poke 32 peek 16 32 inc32

  vql_00.poke32(RG1L);
  i16 wound = RG4W;
  vql_10.poke32(RG2L);
  vo_0c.poke16(RG3W);
  RG4L = signedlong(vql_00.peek16());
  if (RG4L == lcon(0xffffffff))
    return 0;
  RG1L = unsignedlong(ddat.v1e0288);
  RG2L = RG4L + 1;
  if (RG2L == RG1L)
    return 0;
  if (ddat.v1e0240 != 0)
    return 0;
  if (wound <= 0)
    return 0;
  c_hero* hero = &party.hero[RG4L];
  if (hero->curHP == 0)
    return 0;
  RG1L = RG3L;
  RG1W &= lcon(0x8000);
  vw_08 = RG1W;
  vo_0c.and16(lcon(0x7fff)); // adj
  if (vo_0c.peek16() != 0)
  {
    vol_14.poke32(0);
    RG2L = 0;
    RG52L = 0;
    //m_373B3:
    for (;;)
    {
      RG1L = signedlong(RG2W);
      if (RG1L > lcon(0x5))
      {
        if (vol_14.peek16() != 0)
        {
          RG4L = 0;
          RG1L = RG52L;
          RG2L = vol_14.peek32();
          RG4W = RG1W % RG2W;
          RG1W /= RG2W;
          RG52L = RG1L;
        }
        RG3L = 0;
        RG2L = 0;
        break;
      }
      //m_3736C:
      RG3L = signedlong(RG2W);
      RG1L = 1 << RG3Blo;
      RG4L = unsignedlong(vql_10.peek16());
      if ((RG1L & RG4L) != 0)
      {
        RG1L = unsignedlong(vo_0c.peek16());
        vol_14.inc32();
        if (RG1L != lcon(0x4))
          //m_37398:
          RG1L = 0;
        else
          RG1L = lcon(0x8000);
        //m_3739A:
        RG4L = signedlong(RG2W);
        RG1L |= RG4L;
        RG4L = unsignedlong(RG1W);
        RG1L = DM2_hero_2c1d_135d(signedlong(vql_00.peek16()), RG4L);
        RG52L += RG1L;
      }
      //m_373B2:
      RG2L++;
    }
    //m_373E6:
    for (;;)
    {
      RG1L = signedlong(RG2W);
      if (RG1L > 1)
      {
        if (RG3W != 0)
        {
          RG1L = unsignedlong(RG3W) / 8;
          vl_04 = RG1L;
          RG1L = signedlong(vql_00.peek16());
          RG2L = 1;
          RG1W = DM2_QUERY_PLAYER_SKILL_LV(RG1W, 7, RG2L);
          RG4L = unsignedlong(RG1W) + vl_04;
          RG1L = DM2_RAND();
          RG1W &= lcon(0xf);
          RG1L = unsignedlong(RG1W);
          if (RG4L > RG1L)
          {
            if (vw_08 != 0)
            {
              wound -= RG3W;
              if (wound <= 0)
                return 0;
            }
            RG1L = unsignedlong(RG3W) / 4;
            RG52L = unsignedlong(RG52W) + RG1L;
          }
        }
        RG1W = vo_0c.peek16() - 1;
        if (RG1UW <= mkuw(lcon(0x8)))
        {
          bool skip00784 = false;
          RG1L = unsignedlong(RG1W);
          switch (RG1L)
          {
            case 0:
              //m_374C1:
              RG2L = unsignedlong(wound);
              RG4L = 6;
              RG1L = hero->get_adj_ability2((e_ability)RG4W, RG2W);
              wound = RG1W;
              if (hero->ench_aura == 0)
                wound -= hero->ench_power;
              break;

            case 1:
            case 7:
              //m_374E6:
              RG52UW >>= bcon(0x1);
              RG2L = 1;
              RG1W = DM2_QUERY_PLAYER_SKILL_LV(vql_00.peek16(), RG2W, RG2L);
              RG52W += RG1W;
            case 2:
            case 3:
            case 6:
            case 8:
              break;

            case 4:
              //m_37498:
              RG2L = unsignedlong(wound);
              RG4L = 5;
              RG1L = hero->get_adj_ability2((e_ability)RG4W, RG2W);
              wound = RG1W;
              RG4L = unsignedlong(hero->ench_aura);
              if (RG4L == 1)
                wound -= hero->ench_power;
              skip00784 = true;
              break;

            case 5:
              //m_3746F:
              RG2L = 0;
              RG4L = 3;
              RG1W = hero->get_adj_ability1((e_ability)RG4W, (e_curmax)RG2W);
              RG2W = lcon(0x73) - RG1W;
              if (RG2W <= 0)
                return 0;
              RG2L = unsignedlong(RG2W);
              RG1L = DM2_ATIMESB_RSHIFTC(wound, 6, RG2W);
              wound = RG1W;
              skip00784 = true;
              break;

            default: throw(THROW_DMABORT);
          }

          if (skip00784)
            break;
        }

        //m_374FE:
        if (wound <= 0)
          return 0;
        RG1L = lcon(0x82) - RG52L;
        RG2L = unsignedlong(RG1W);
        RG1L = DM2_ATIMESB_RSHIFTC(wound, 6, RG2W);
        wound = RG1W;
        break;
      }
      //m_373D6:
      if (hero->handcmd[RG1L] == 1)
      {
        RG1W = signedword(hero->handdefenseclass[RG1L]);
        RG3L += RG1L;
      }
      RG2L++;
    }
    //m_37524:
    if (wound <= 0)
      return 0;
    RG1L = DM2_RAND();
    RG1W &= lcon(0x7f);
    RG1L += lcon(0xa);
    RG2L = unsignedlong(RG1W);
    RG4L = 4;
    RG1L = hero->get_adj_ability2((e_ability)RG4W, RG2W);
    RG2L = RG1L;
    if (wound > RG1W)
    {
      //m_37553:
      for (;;)
      {
        RG4L = signedlong(vql_00.peek16());
        RG3L = DM2_RAND();
        RG3Blo &= lcon(0x7);
        RG1L = 1 << RG3Blo & vql_10.peek32();
        ddat.v1e0ba8[RG4L] |= RG1W;
        RG2L = 2 * RG2L;
        if (wound <= RG2W || RG2W == 0)
          break;
      }
    }
    //m_37583:
    if (ddat.v1e0238 != 0)
      DM2_RESUME_FROM_WAKE();
  }
  //m_37591:
  ddat.v1e0bb0[signedlong(vql_00.peek16())] += wound;
  return wound;
}

i32 DM2_ADJUST_STAMINA(i16 heroidx, i16 edxw)
{
  c_nreg _RG1; RG1W = heroidx;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG1L = signedlong(RG1W);
  if (RG1L == lcon(0xffffffff))
    return RG1L;
  c_hero* hero = &party.hero[RG1L];
  RG4W = hero->curStamina;
  RG4W -= edxw;
  hero->curStamina = RG4W;
  if (RG4W > 0)
  {
    //m_37800:
    RG2W = hero->maxStamina;
    if (RG4W > RG2W)
      hero->curStamina = RG2W;
  }
  else
  {
    hero->curStamina = 0;
    RG4L = signedlong(CUTX16(-signedlong(RG4W) / 2));
    DM2_WOUND_PLAYER(RG1L, RG4L, 0, 0);
  }
  //m_3780D:
  RG1L = signedlong(DM2_ABS(edxw));
  if (RG1L >= 10)
    hero->heroflag |= 0x800;
  return RG1L;
}

// was SKW_2c1d_210a
void DM2_UPDATE_CHAMPIONS_STATS(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG51w;
  i16 RG52w;
  i16 RG53w;
  c_wlreg _RG54;
  c_wlreg _RG55;
  i16 rg63;
  i32 vl_00;
  c_o32 vo_04; // poke 16 peek 16 32 inc16
  c_ql32 vql_08;

  if (party.heros_in_party == 0)
    return;
  RG2W = ddat.v1e0ff8;
  RG2L += lcon(0x38);
  ddat.v1e0ff8 = RG2W;
  RG1L = unsignedlong(RG2W);
  if (RG1L > lcon(0x80))
  {
    RG3L = RG2L - lcon(0x80);
    ddat.v1e0ff8 = RG3W;
  }
  vo_04.poke16(wcon(0x0));
  c_hero* hero = &party.hero[0];
  //m_38117:
  for (;;)
  {
    RG1W = vo_04.peek16();
    if (RG1UW >= mkuw(party.heros_in_party))
      return;
    //m_37DC8:
    if (hero->curHP != 0)
    {
      RG1L = unsignedlong(vo_04.peek16());
      RG4L = unsignedlong(ddat.v1e0288);
      RG1L++;
      if (RG1L != RG4L)
      {
        RG1W = hero->curMP;
        RG4W = hero->maxMP;
        if (RG1W >= RG4W)
        {
          //m_37EB6:
          if (RG1W > RG4W)
            hero->curMP = RG1W - 1;
        }
        else
        {
          RG55W = DM2_QUERY_PLAYER_SKILL_LV(vo_04.peek16(), 3, 1) + DM2_QUERY_PLAYER_SKILL_LV(vo_04.peek16(), 2, 1);
          vql_08.poke16(RG55W);
          i16 rg6 = RG55W;
          RG2L = 0;
          RG4L = 3;
          RG1W = hero->get_adj_ability1((e_ability)RG4W, (e_curmax)RG2W) + rg6;
          RG4W = ddat.v1e0ff8;
          if (RG4W < RG1W)
          {
            RG4L = signedlong(hero->maxMP);
            RG2L = lcon(0x28);
            RG1L = RG4L;
            RG4L = RG1L % RG2L;
            RG1L /= RG2L;
            RG54L = RG1L + 1;
            if (ddat.v1e0238 != 0)
              RG54L = 2 * RG54L;
            RG1L = lcon(0x10) - vql_08.peek32();
            RG4L = signedlong(RG1W);
            RG1L = signedlong(DM2_MAX(7, RG4W)) * RG54L;
            RG4L = signedlong(RG1W);
            DM2_ADJUST_STAMINA(vo_04.peek16(), RG4W);
            RG1W = hero->maxMP - hero->curMP;
            RG4L = signedlong(RG1W);
            RG1L = signedlong(DM2_MIN(RG54W, RG4W));
            hero->curMP += RG1W;
          }
        }
        //m_37EBF:
        rg63 = 4;
        RG51w = hero->maxStamina;
        //m_37EC8:
        for (;;)
        {
          RG51w >>= bcon(0x1);
          if (RG51w <= hero->curStamina)
            break;
          rg63 += 2;
        }
        //m_37ED6:
        RG52w = wcon(0x0);
        RG1L = signedlong(hero->maxStamina & 0xff00) - 1; // did access high byte of maxStamina without and-mask
        RG4L = signedlong(RG1W);
        RG1L = signedlong(DM2_BETWEEN_VALUE(1, 6, RG4W));
        RG2L = RG1L;
        RG4W = CUTX16(timdat.gametick) - CUTX16(ddat.v1e01a0); // TODO: really not longs?
        vql_08.poke32(RG4L);
        RG4L = unsignedlong(vql_08.peek16());
        if (RG4L > lcon(0x50))
        {
          RG1L++;
          if (RG4L > lcon(0xfa))
            RG1L++;
        }
        if (ddat.v1e0238 != 0)
          RG1L = 2 * RG1L;
        //m_37F26:
        for (;;)
        {
          RG4L = unsignedlong(rg63);
          RG4Blo = RG4L <= lcon(0x4) ? 1 : 0;
          RG4W = unsignedword(RG4Blo);
          RG2L = RG4L;
          vql_08.poke32(RG4L);
          RG4L = signedlong(hero->food);
          if (RG4L >= lcon(0xfffffe00))
          {
            //m_37F55:
            if (hero->food >= 0)
              RG52w -= RG1W;
            if (vql_08.peek16() == 0)
              //m_37F70:
              RG4L = unsignedlong(rg63) / 2;
            else
              RG4L = 2;
            //m_37F77:
            vl_00 = signedlong(hero->food) - RG4L;
            RG4L = vl_00;
            hero->food = RG4W;
          }
          else
          {
            if (RG2W != 0)
            {
              RG52w += RG1W;
              hero->food -= 2;
            }
          }
          //m_37F88:
          RG4L = signedlong(hero->water);
          if (RG4L >= lcon(0xfffffe00))
          {
            //m_37FA6:
            if (hero->water >= 0)
              RG52w -= RG1W;
            if (vql_08.peek16() == 0)
              //m_37FC1:
              RG4L = mkul(rg63 / 4);
            else
              RG4L = 1;
            //m_37FC9:
            vl_00 = signedlong(hero->water) - RG4L;
            RG4L = vl_00;
            hero->water = RG4W;
          }
          else
          {
            if (vql_08.peek16() != 0)
            {
              RG52w += RG1W;
              hero->water--;
            }
          }
          //m_37FDA:
          if (--rg63 != 0)
          {
            RG4L = signedlong(hero->curStamina);
            RG2L = signedlong(RG52w);
            RG4L -= RG2L;
            RG2L = RG4L;
            RG4L = signedlong(hero->maxStamina);
            if (RG2L < RG4L)
              continue;
          }
          break;
        }
        //m_37FF9:
        RG4L = signedlong(RG52w);
        DM2_ADJUST_STAMINA(vo_04.peek16(), RG4W);
        if (hero->food < wcon(0xfc00))
          hero->food = wcon(0xfc00);
        if (hero->water < wcon(0xfc00))
          hero->water = wcon(0xfc00);
        RG1W = hero->curHP;
        if (RG1W < hero->maxHP)
        {
          RG1W = hero->curStamina >> 2;
          RG4L = signedlong(hero->curStamina);
          if (RG4L >= RG1L)
          {
            RG2L = 0;
            RG4L = 4;
            RG1W = hero->get_adj_ability1((e_ability)RG4W, (e_curmax)RG2W);
            RG4W = ddat.v1e0ff8;
            RG1W += 12;
            if (RG4W < RG1W)
            {
              RG1L = (hero->curHP >> 7) + 1;
              if (ddat.v1e0238 != 0)
                RG1L = 2 * RG1L;
              RG4L = signedlong(hero->maxHP - hero->curHP);
              RG1L = signedlong(DM2_MIN(RG1W, RG4W));
              hero->curHP += RG1W;
            }
          }
        }
        //m_38088:
        if (ddat.v1e0238 == 0)
          //m_38098:
          RG1L = lcon(0xff);
        else
          RG1L = lcon(0x3f);
        //m_3809D:
        RG4L = unsignedlong(CUTX16(timdat.gametick));
        if ((RG1L & RG4L) == 0)
        {
          RG53w = wcon(0x0);
          //m_380E7:
          for (;;)
          {
            RG1L = signedlong(RG53w);
            if (RG1L > 6)
              break;
            //m_380AE:
            i16 rg2n = CUTX16(RG1L);
            RG1Blo = hero->ability[rg2n][E_MAX];
            vql_08.poke32(signedlong(RG1Blo));
            i16 rg64 = hero->ability[rg2n][E_CUR];
            RG1W = hero->ability[rg2n][E_MAX];
            if (rg64 >= RG1W)
            {
              //m_380CE:
              i16 rg7w = vql_08.peek16();
              if (rg64 > rg7w)
              {
                RG1W = rg64;
                RG4W = RG1W % rg7w;
                RG1W /= rg7w;
                rg64 -= RG1W;
                RG1W = rg64;
                hero->ability[rg2n][E_CUR] = RG1Blo;
              }
            }
            else
              hero->ability[rg2n][E_CUR]++;
            //m_380E6:
            RG53w++;
          }
        }
        //m_380EF:
        hero->heroflag |= 0x800;
        RG1L = unsignedlong(vo_04.peek16());
        RG4L = signedlong(ddat.v1e0976);
        RG1L++;
        if (RG1L == RG4L)
          hero->heroflag |= 0x3000;
      }
    }
    //m_3810C:
    vo_04.inc16();
    hero++; // struct advance
  }
  FOREND
}

// was SKW_2c1d_111e
i16 DM2_USE_DEXTERITY_ATTRIBUTE(i16 heroidx)
{
  c_nreg _RG1; RG1W = heroidx;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  i32 RG5l = unsignedlong(RG1W);
  c_hero* hero = &party.hero[RG5l];
  RG2L = 0;
  RG4L = 2;
  RG1W = hero->get_adj_ability1((e_ability)RG4W, (e_curmax)RG2W);
  RG2W = (DM2_RAND() & 0x7) + RG1W;
  RG4L = signedlong(RG2W) / 2;
  RG1W = party.get_player_weight((e_hero)CUTX16(RG5l));
  RG4L *= RG1W; //!
  RG1W = hero->get_max_load();
  RG3L = unsignedlong(RG1W);
  RG1L = RG4L;
  RG4L = RG1L % RG3L;
  RG1L /= RG3L;
  RG2L -= RG1L;
  RG4L = signedlong(RG2W);
  RG2L = signedlong(DM2_MAX(2, RG4W));
  if (ddat.v1e0238 != 0)
    RG2W >>= bcon(0x1);
  RG1L = DM2_RAND();
  RG1W &= lcon(0x7);
  RG4L = signedlong(CUTX16(lcon(0x64) - RG1L));
  RG1L = signedlong(RG2W) / 2;
  RG3L = signedlong(RG1W);
  RG1L = DM2_RAND();
  RG1W &= lcon(0x7);
  RG1L++;
  return signedlong(DM2_BETWEEN_VALUE(RG1W, RG4W, RG3W));
}

// stacksize was 0x18
bool DM2_WIELD_WEAPON(i16 eaxw, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 vw_00;
  c_ql32 vql_04;
  i16 vw_08;
  i16 parw00;
  i16 parw03;
  i16 parw04;

  vw_08 = RG1W;
  vql_04.poke32(RG4L);
  vw_00 = RG3W; // truncated
  c_hero* hero = &party.hero[RG1W];
  if (ddat.v1e0b4c == lcon(0xffffffff))
    return false;
  RG4L = unsignedlong(RG3W);
  RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG2W), RG4L);
  if (RG1W == lcon(0xffffffff))
    return false;
  i16 position = unsignedword(hero->partypos);
  RG1L = unsignedlong(hero->absdir);
  RG4L = unsignedlong(hero->partypos) + 4 - RG1L;
  RG1L = RG4L & lcon(0x3);
  if (mkul(RG1L) >= mkul(2))
  {
    bool skip00707 = false;
    if (mkul(RG1L) <= mkul(2))
    {
      //m_3128D:
      RG4L = 3;
      skip00707 = true;
    }
    else
    {
      if (RG1L == lcon(0x3))
      {
        //m_31294:
        RG4L = 1;
        skip00707 = true;
      }
    }

    if (skip00707)
    {
      //m_31299:
      RG4W += position;
      RG4W &= 0x3;
      RG1W = DM2_GET_PLAYER_AT_POSITION(RG4W);
      if (RG1W != -1)
      {
        ddat.v1e0b7c = lcon(0xffffffff);
        RG1L = DM2_UPDATE_GLOB_VAR(lcon(0x42), 1, lcon(0x3));
        RG4L = signedlong(CUTX16(lcon(0x8) - RG1L));
        RG1L = signedlong(DM2_MAX(1, RG4W));
        ddat.v1e0b6c = RG1W;
        return false;
      }
    }
  }
  //m_312EC:
  RG1L = signedlong(vql_04.peek16());
  if (RG1L == 1)
  {
    RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(ddat.v1e0b4c));
    if ((RG1Blo & bcon(0x20)) == 0)
      return false;
  }
  RG1L = DM2_QUERY_CUR_CMDSTR_ENTRY(10);
  RG3L = RG1L;
  RG4L = RG1L;
  i16 rg62 = CUTX16(DM2_QUERY_CUR_CMDSTR_ENTRY(11));
  RG1L = DM2_QUERY_CUR_CMDSTR_ENTRY(15);
  if (RG1W != 0)
  {
    RG3Bhi |= lcon(0xffffff80);
    RG4L = RG3L;
  }
  RG1L = signedlong(argw1);
  put16(parw04, RG1W);
  RG1L = unsignedlong(argw0);
  put16(parw03, RG1W);
  RG1L = unsignedlong(RG4W);
  i32 parl01 = RG1L;
  RG1L = unsignedlong(vw_00);
  put16(parw00, RG1W);
  RG3L = unsignedlong(RG2W);
  RG2L = unsignedlong(ddat.v1e0b4c);
  RG4L = unsignedlong(vw_08);
  RG1W = DM2_CALC_PLAYER_ATTACK_DAMAGE(hero, RG4W, RG2L, RG3L, parw00, parl01, rg62, parw03, parw04);
  ddat.v1e0b7c = RG1W;
  return true;
}

void DM2_EQUIP_ITEM_TO_HAND(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;

  RG3L = RG1L;
  if (RG4W == lcon(0xffffffff))
    return;
  c_hero* hero = &party.hero[RG1W];
  RG4Bhi &= lcon(0x3f);
  RG5l = unsignedlong(RG2W);
  RG1L = 2 * RG5l;
  if (RG5l < lcon(0x1e))
    //m_360B8:
    hero->item[RG5l] = RG4W;
  else
    party.hand_container[(RG1L - 0x3c) / 2] = RG4W; // EVIL! offsetproblem1
  //m_360C0:
  RG2L = signedlong(RG2W);
  RG4L = unsignedlong(RG4W);
  RG1L = signedlong(RG3W);
  DM2_PROCESS_ITEM_BONUS(RG1L, RG4L, RG2L, 1);
}

void DM2_ADD_ITEM_TO_PLAYER(e_hero heroidx, i16 edxw)
{
  c_nreg _RG1; RG1W = heroidx; // e_hero
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5;
  i16 vw_00;
  i16 vw_04;

  vw_00 = RG1W; // TODO: incoming RG1W word only
  RG4L = 0;
  vw_04 = RG4W; // truncated
  //m_392DB:
  for (;;)
  {
    RG4L = unsignedlong(vw_04);
    if (RG4L >= lcon(0x5))
      return;
    //m_39231:
    RG1L = 3 * RG4L;
    RG5W = table1d6a31[RG4L].w_00;
    //m_392B7:
    for (;;)
    {
      RG4L = unsignedlong(vw_04);
      RG3L = 6 * RG4L;
      if (RG5UW > mkuw(table1d6a31[RG4L].w_02))
      {
        RG3L = RG4L + 1;
        vw_04 = RG3W; // truncated
        break;
      }
      //m_39245:
      if (party.hero[vw_00].item[RG5W] == -1)
      {
        RG4L = signedlong(RG5W);
        RG1L = unsignedlong(edxw);
        RG2L = 0;
        RG1L = DM2_IS_ITEM_FIT_FOR_EQUIP(RG1W, RG4L, RG2L);
        if (RG1L != 0)
        {
          bool skip00798 = false;
          RG1W = table1d6a31[RG3L / 6].w_04;
          if (RG1W == lcon(0xffffffff))
            skip00798 = true;
          else
          {
            RG4W = RG1W;
            RG1W = edxw;
            RG1W &= lcon(0x3c00);
            RG1W = RG1W >> 10;
            if (RG1W == RG4W)
              skip00798 = true;
          }

          if (skip00798)
          {
            //m_3929F:
            RG2L = unsignedlong(RG5W);
            RG4L = unsignedlong(edxw);
            DM2_EQUIP_ITEM_TO_HAND(unsignedlong(vw_00), RG4L, RG2L);
            return;
          }
        }
      }
      //m_392B6:
      RG5L++;
    }
  }
  FOREND
}

// was SKW_24a5_069b
void DM2_BURN_PLAYER_LIGHTING_ITEMS(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 vl_00;
  bool flag = false;

  RG1W = party.heros_in_party;
  vl_00 = RG1L;
  if (ddat.v1e0288 != 0)
  {
    RG2L = RG1L - 1;
    vl_00 = RG2L;
  }
  c_hero* hero = &party.hero[0];
  //m_2911D:
  for (;;)
  {
    RG3L = vl_00 - 1;
    vl_00 = RG3L;
    if (RG3W == lcon(0xffffffff))
      break;
    RG3L = 2;
    //m_2912F:
    for (;;)
    {
      RG3L--;
      if (RG3W == lcon(0xffffffff))
        break;
      RG1W = hero->item[RG3W];
      RG2L = unsignedlong(RG1W);
      RG4L = 0;
      RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG2L, RG4L);
      RG1W &= lcon(0x10);
      RG1L = unsignedlong(RG1W);
      if (RG1L == 0)
        continue;
      RG4L = 0;
      RG1W = DM2_ADD_ITEM_CHARGE(RG2L, RG4L);
      if (RG1W == 0)
        continue;
      RG4L = lcon(0xffffffff);
      RG1W = DM2_ADD_ITEM_CHARGE(RG2L, RG4L);
      if (RG1W == 0)
      {
        RG4L = 0;
        RG1L = RG2L;
        DM2_SET_ITEM_IMPORTANCE(RG1L, RG4L);
      }
      //m_29182:
      flag = true;
    }
    //m_29189:
    hero++; // struct advance
  }
  //m_29191:
  if (flag)
    RG1L = DM2_RECALC_LIGHT_LEVEL();
}

// was SKW_1031_16a0
void DM2_SELECT_CHAMPION_LEADER(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG4;

  RG4L = RG1L;
  if ((e_hero)RG1W == eventqueue.event_heroidx)
    return;
  if (RG1W != lcon(0xffffffff))
  {
    RG2L = signedlong(RG1W);
    if (party.hero[RG2L].curHP == 0)
      return;
  }
  RG2L = signedlong(eventqueue.event_heroidx);
  if (RG2L != lcon(0xffffffff))
    party.hero[RG2L].heroflag |= lcon(0x1400);
  eventqueue.event_heroidx = (e_hero)RG4W;
  if (RG4W == lcon(0xffffffff))
    return;
  RG2L = signedlong(RG4W);
  RG4L = unsignedlong(ddat.v1e0288);
  RG1L = RG2L + 1;
  if (RG1L == RG4L)
    return;
  party.hero[RG2L].heroflag |= lcon(0x1400);
}

i32 DM2_REMOVE_OBJECT_FROM_HAND(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5;

  RG5W = ddat.savegamewpc.w_00;
  if (RG5W != lcon(0xffffffff))
  {
    RG4L = 0;
    ddat.savegamewpc.w_18 = 0;
    ddat.savegamewpc.weight = 0;
    ddat.savegamewpc.w_00 = lcon(0xffffffff);
    ddat.savegamewpc.b_16 = lcon(0xffffffff);
    DM2_HIDE_MOUSE();
    DM2_events_443c_0434();
    DM2_SHOW_MOUSE();
    RG4L = unsignedlong(RG5W);
    RG1L = signedlong(eventqueue.event_heroidx);
    RG3L = lcon(0xffffffff);
    DM2_PROCESS_ITEM_BONUS(RG1L, RG4L, RG3L, RG3L);
    DM2_moverec_3CE7D(unsignedlong(ddat.v1e0270), unsignedlong(ddat.v1e0272), lcon(0xffff), 1, 0, 0);
  }
  return RG5L; // TODO: highword undefined
}

// belongs to DM2_GET_PARTY_SPECIAL_FORCE
static i32 DM2_hero_37BEA(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG2L = RG1L;
  RG4L = signedlong(RG1W);
  if (party.hero[RG4L].curHP == 0)
    return 0;
  RG1W = party.hero[RG4L].heroflag & lcon(0x10);
  RG3L = unsignedlong(RG1W);
  RG1W = party.get_player_weight((e_hero)RG2W);
  RG4L = unsignedlong(RG1W);
  RG2L = 10;
  RG1L = RG4L;
  RG4L = RG1L % RG2L;
  RG1L /= RG2L;
  RG1L += RG3L;
  return RG1L != 0 ? lcon(0x32) : lcon(0x28);
}

// was SKW_2c1d_201d
i32 DM2_GET_PARTY_SPECIAL_FORCE(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG2L = 0;
  RG4L = 0;
  //m_37C60:
  for (;;)
  {
    RG1L = signedlong(RG4W);
    RG3L = unsignedlong(party.heros_in_party);
    if (RG1L >= RG3L)
      return RG2L;
    //m_37C58:
    RG1L = DM2_hero_37BEA(RG1L);
    RG2L += RG1L;
    RG4L++;
  }
  FOREND
}

// was SKW_2759_0f39
void DM2_ADJUST_HAND_COOLDOWN(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;

  c_hero* hero = &party.hero[RG1W];
  RG1L = unsignedlong(RG4W);
  RG1L += RG1L / 4;
  RG4L = RG1L;
  if (RG2W != lcon(0xffffffff))
    //m_30AF3:
    RG3L = 1;
  else
  {
    RG3L = 3;
    RG2L = 0;
  }
  //m_30AF8:
  if (ddat.savegames1.b_04 != 0)
    RG4UW >>= bcon(0x2);
  RG4L += 2;
  //m_30B08:
  for (;;)
  {
    RG1L = signedlong(RG2W);
    RG1Blo = hero->handcooldown[RG1L];
    RG1W = unsignedword(RG1Blo);
    if (RG4UW <= RG1UW)
    {
      //m_30B27:
      RG5l = unsignedlong(RG4W) / 2;
      RG1L = unsignedlong(RG1W);
    }
    else
    {
      RG5l = (RG1L & lcon(0xffff)) / 2;
      RG1L = unsignedlong(RG4W);
    }
    //m_30B33:
    RG1L += RG5l;
    if (unsignedlong(RG1W) > ulcon(0xff))
      RG1L = lcon(0xff);
    hero->handcooldown[RG2W] = RG1Blo;
    RG2L++;
    if (--RG3W == 0)
      return;
  }
  FOREND
}

i32 DM2_REMOVE_POSSESSION(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_lreg _RG62;

  RG2L = RG1L;
  RG3L = unsignedlong(RG1W);
  c_hero* hero = &party.hero[RG3L];
  RG3L = unsignedlong(RG4W);
  RG1L = 2 * RG3L;
  if (RG3L < 30)
  {
    //m_36015:
    RG5W = hero->item[RG3L];
    hero->item[RG3L] = -1;
  }
  else
  {
    RG5W = party.hand_container[(RG1L - 60) / 2]; // EVIL! solved offsetproblem1 v1e0920
    party.hand_container[(RG1L - 60) / 2] = lcon(0xffffffff); // EVIL! solved offsetproblem1 v1e0920
  }
  //m_36027:
  if (RG5W == lcon(0xffffffff))
    return RG5L;
  RG3L = signedlong(party.curacthero);
  RG1L = unsignedlong(RG2W);
  RG3L--;
  if (RG1L == RG3L)
  {
    RG1L ^= RG3L;
    RG1W = RG4W;
    if (RG1L <= 1)
    {
      RG3L = signedlong(party.curactmode);
      if (RG1L == RG3L)
        DM2_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
    }
  }
  RG4L = signedlong(RG4W);
  RG62L = unsignedlong(RG5W);
  RG1L = signedlong(RG2W);
  DM2_PROCESS_ITEM_BONUS(RG1L, RG62L, RG4L, lcon(0xffffffff));
  return RG5L;
}

// belongs to DM2_PLAYER_DEFEATED
static i32 DM2_DROP_PLAYER_ITEMS(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  i16 RG6w;
  i32 vl_00;
  i16 parw00;

  RG6w = RG1W;
  RG4L = unsignedlong(RG1W);
  RG1L = 263 * RG4L;
  RG1Blo = party.hero[RG4L].partypos;
  RG1W = unsignedword(RG1Blo);
  vl_00 = RG1L;
  RG5w = wcon(0x0);
  //m_36EEF:
  for (;;)
  {
    RG1L = unsignedlong(RG5w);
    if (RG1L >= lcon(0x1e))
      return RG1L;
    //m_36EA4:
    RG4L = unsignedlong(table1d69b0[RG1L]);
    RG1L = DM2_REMOVE_POSSESSION(unsignedlong(RG6w), RG4L);
    if (RG1W != lcon(0xffffffff))
    {
      RG4L = signedlong(ddat.v1e0272);
      put16(parw00, RG4W);
      RG3L = signedlong(ddat.v1e0270);
      RG1Bhi &= lcon(0x3f);
      RG4L = RG1L;
      RG1L = (vl_00 << bcon(0xe) | RG4L) & lcon(0xffff);
      RG2L = 0;
      DM2_MOVE_RECORD_TO(RG1L, lcon(0xffffffff), RG2L, RG3L, parw00);
    }
    //m_36EEE:
    RG5w++;
  }
  FOREND
}

// was SKW_2c1d_1bb5 (new)
static i32 DM2_CURE_POISON(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  c_wlreg _RG6;
  c_ql32 vql_00;

  vql_00.poke32(RG1L);
  RG3L = RG4L;
  RG4L = signedlong(vql_00.peek16());
  if (RG4L == lcon(0xffffffff))
    return RG1L;
  c_hero* hero = &party.hero[RG4L];
  c_tim* RG4timp = timdat.timerarray;
  RG5w = wcon(0x0);
  //m_37702:
  for (;;)
  {
    RG1L = unsignedlong(RG5w);
    RG6L = signedlong(timdat.num_timer_indices);
    if (RG1L >= RG6L)
      return RG1L;
    //m_376B8:
    if (RG4timp->is_type(0x4b))
    {
      RG6W = unsignedword(RG4timp->getactor());
      if (RG6W == vql_00.peek16())
      {
        RG6W = RG4timp->getA();
        if (RG3W < RG6W)
        {
          //m_376F0:
          RG1L = RG6L - RG3L;
          RG4timp->setA(RG1W);
          hero->poison -= RG3W;
          return RG1L;
        }
        RG3L -= RG6L;
        hero->poison -= RG6W;
        DM2_DELETE_TIMER(RG1W);
        hero->poisoned = RG1Bhi = hero->poisoned - 1;
        if (RG1Bhi == 0)
          return RG1L;
        if (RG3W <= 0)
          return RG1L;
      }
    }
    //m_376FE:
    RG4timp++;
    RG5w++;
  }
  FOREND
}

void DM2_PLAYER_DEFEATED(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG52; // W and UW only
  c_wlreg _RG6;
  c_ql32 vql_00;
  i16 parw00;

  RG6L = RG1L;
  RG4L = unsignedlong(RG1W);
  c_hero* hero = &party.hero[RG4L];
  RG1L = signedlong(party.curacthero) - 1;
  if (RG4L == RG1L)
    DM2_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
  hero->curHP = 0;
  hero->handcooldown[0] = hero->handcooldown[1] = hero->handcooldown[2] = 0;
  hero->heroflag |= 0x4000;
  RG1L = unsignedlong(RG6W);
  RG4L = signedlong(ddat.v1e0976);
  RG1L++;
  if (RG1L == RG4L)
  {
    if (ddat.vcapture3)
    {
      ddat.vcapture3 = false;
      DM2_MOUSE_RELEASE_CAPTURE();
      RG4W = ddat.savegamewpc.w_00;
      if (RG4W != lcon(0xffffffff))
      {
        RG1L = unsignedlong(RG4W);
        DM2_DISPLAY_TAKEN_ITEM_NAME(RG1W);
      }
      //m_37007:
      Tmouse.hide();
      DM2_SHOW_MOUSE();
    }
    //m_37015:
    if (ddat.vcapture2)
    {
      ddat.vcapture2 = false;
      DM2_MOUSE_RELEASE_CAPTURE();
      Tmouse.hide();
      DM2_SHOW_MOUSE();
    }
    //m_37039:
    DM2_guidraw_24a5_1798(4);
  }
  //m_37043:
  RG4L = unsignedlong(RG6W);
  RG1L = signedlong(eventqueue.event_heroidx);
  if (RG4L == RG1L && ddat.vcapture1)
  {
    ddat.vcapture1 = false;
    DM2_MOUSE_RELEASE_CAPTURE();
    Tmouse.hide();
    DM2_SHOW_MOUSE();
  }
  //m_37078:
  RG1L = DM2_DROP_PLAYER_ITEMS(unsignedlong(RG6W));
  RG1W = unsignedword(RG1Blo);
  RG1Blo = hero->partypos;
  vql_00.poke32(RG1L);
  RG1L = DM2_ALLOC_NEW_RECORD(lcon(0x800a));
  RG4L = RG1L;
  if (RG1W != lcon(0xffffffff))
  {
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
    or8(location(RG1P + 2), lcon(0xffffff80));
    and16(location(RG1P + 2), lcon(0x3f80));
    RG2L = RG6L;
    RG2W &= 0x3;
    RG2L <<= bcon(0xe);
    or16(location(RG1P + 2), RG2W);
    RG1L = signedlong(ddat.v1e0272);
    put16(parw00, RG1W);
    RG3L = signedlong(ddat.v1e0270);
    RG4Bhi &= lcon(0x3f);
    RG1L = (vql_00.peek32() << bcon(0xe) | RG4L) & lcon(0xffff);
    DM2_MOVE_RECORD_TO(RG1L, lcon(0xffffffff), 0, RG3L, parw00);
  }
  //m_370ED:
  DM2_HIDE_MOUSE();
  hero->nrunes = 0;
  hero->rune[0] = '\0';
  hero->absdir = CUTX8(ddat.v1e0258);
  hero->b_29 = 0;
  RG4L = signedlong(ddat.v1e0258);
  RG1L = (unsignedlong(vql_00.peek16()) + 4 - RG4L) & lcon(0x3);
  RG4L = unsignedlong(ddat.v1e00b8);
  RG1L++;
  if (RG1L == RG4L)
    DM2_CHAMPION_SQUAD_RECOMPUTE_POSITION();
  if (hero->poisoned != 0)
    DM2_CURE_POISON(signedlong(RG6W), lcon(0x2710));
  RG4L = 0;
  RG52W = party.heros_in_party;
  //m_3716A:
  for (;;)
  {
    if (RG4UW < RG52UW)
    {
      //m_3714E:
      RG2L = unsignedlong(RG4W);
      RG1L = 263 * RG2L;
      if (party.hero[RG2L].curHP == 0)
      {
        RG4L++;
        continue;
      }
    }
    break;
  }
  //m_3716F:
  if (RG4W != party.heros_in_party)
  {
    //m_37197:
    RG2L = unsignedlong(RG6W);
    RG1L = signedlong(eventqueue.event_heroidx);
    if (RG2L == RG1L)
    {
      RG1L = signedlong(RG4W);
      DM2_SELECT_CHAMPION_LEADER(RG1L);
    }
  }
  else
  {
    ddat.v1e0250 = 1;
    eventqueue.event_1031_098e();
    ddat.v1e0238 = 0;
    ddat.v1e13f0 = 0;
    RG3L = 0; // faller
  }
  //m_371B0:
  DM2_107B0();
  DM2_SHOW_MOUSE();
}

// belongs to DM2_GAME_LOOP
void DM2_PROCESS_PLAYERS_DAMAGE(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  i32 RG52l;
  i16 RG53w;
  c_tim c38_00;

  c_hero* hero = &party.hero[0];
  RG3L = 0;
  //m_372A0:
  while (RG3UW < mkuw(party.heros_in_party))
  {
    //m_371D8:
    RG2L = unsignedlong(RG3W);
    hero->bodyflag |= ddat.v1e0ba8[RG2L];
    ddat.v1e0ba8[RG2L] = 0;
    RG53w = ddat.v1e0bb0[RG2L];
    if (RG53w != 0)
    {
      ddat.v1e0bb0[RG2L] = 0;
      RG1W = hero->curHP;
      if (RG1W != 0)
      {
        RG1W -= RG53w;
        if (RG1W > 0)
        {
          //m_3722A:
          hero->curHP = RG1W;
          hero->damagesuffered = RG53w;
          hero->heroflag |= 0x800;
          RG1W = hero->timeridx;
          RG52l = signedlong(RG1W);
          RG2L = timdat.gametick + lcon(0x5);
          if (RG52l != lcon(0xffffffff))
          {
            //m_37271:
            timdat.timerarray[RG52l].setmticks(ddat.v1e0266, RG2L);
            DM2_timer_3a15_05f7(RG1W);
          }
          else
          {
            c38_00.settype(0xc);
            c38_00.setmticks(ddat.v1e0266, RG2L);
            c38_00.setactor(RG3UBlo);
            RG1W = DM2_QUEUE_TIMER(&c38_00);
            hero->timeridx = RG1W;
          }
        }
        else
          DM2_PLAYER_DEFEATED(RG2L);
      }
    }
    //m_37299:
    RG3L++;
    hero++; // struct advance
  }
}

// belongs to DM2_HANDLE_UI_EVENT
void DM2_PUT_ITEM_TO_PLAYER(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;

  RG5w = RG1W;
  if (ddat.savegamewpc.w_00 == lcon(0xffffffff))
    return;
  RG4L = signedlong(RG1W);
  RG1L = 263 * RG4L;
  if (party.hero[RG4L].curHP == 0)
    return;
  RG2L = lcon(0xd);
  RG4L = signedlong(RG5w);
  //m_35E6B:
  for (;;)
  {
    RG3L = signedlong(RG2W);
    if (RG3L < lcon(0x1e))
    {
      //m_35E53:
      RG1L = 263 * RG4L;
      if (party.hero[RG4L].item[RG3L] != -1)
      {
        RG2L++;
        continue;
      }
    }
    break;
  }
  //m_35E73:
  RG1L = signedlong(RG2W);
  if (RG1L != lcon(0x1e))
  {
    RG1L = DM2_REMOVE_OBJECT_FROM_HAND();
    if (RG1W != lcon(0xffffffff))
    {
      RG2L = unsignedlong(RG2W);
      RG4L = unsignedlong(RG1W);
      RG1L = unsignedlong(RG5w);
      DM2_EQUIP_ITEM_TO_HAND(RG1L, RG4L, RG2L);
    }
  }
}

// was SKW_12b4_0141
void DM2_PERFORM_TURN_SQUAD(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  c_5bytes c12_00; // TODO .b_00 undefined!

  RG5w = RG1W;
  if (RG1W == 0)
    return;
  ddat.v1e0488 = true;
  DM2_RESET_SQUAD_DIR();
  RG1L = dm2_GET_TILE_VALUE(signedlong(ddat.v1e0270), signedlong(ddat.v1e0272));
  RG4W = unsignedword(RG4Blo);
  RG4Blo = RG1Blo;
  RG1L = unsignedlong(RG1Blo) >> 5;
  RG1L = unsignedlong(RG1Blo);
  if (RG1L == lcon(0x3))
  {
    RG4Blo &= 0x4;
    RG1L = signedlong(RG4W);
    DM2_move_12b4_00af(RG1L);
    return;
  }
  RG2L = signedlong(ddat.v1e0272);
  RG4L = signedlong(ddat.v1e0270);
  RG1L = DM2_GET_TELEPORTER_DETAIL(&c12_00, RG4L, RG2L);
  if (RG1L != 0)
  {
    DM2_map_3BF83(unsignedlong(c12_00.b_02), unsignedlong(c12_00.b_03), unsignedlong(c12_00.b_04), unsignedlong(c12_00.b_01));
    return;
  }
  i32 parl01 = RG1L;
  i32 parl00 = RG1L;
  RG4L = unsignedlong(ddat.v1e0272);
  RG1W = ddat.v1e0270;
  DM2_moverec_3CE7D(RG1L, RG4L, lcon(0xffff), 1, parl00, parl01);
  if (RG5w != 2)
    //m_13514:
    RG1L = 3;
  else
    RG1L = 1;
  //m_13519:
  RG1L += signedlong(ddat.v1e0258);
  RG1L &= 0x3;
  RG1L = signedlong(RG1W);
  party.rotate(RG1W);
  DM2_moverec_3CE7D(unsignedlong(ddat.v1e0270), unsignedlong(ddat.v1e0272), lcon(0xffff), 1, 1, 0);
}

void DM2_RESET_SQUAD_DIR(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;

  RG4L = 0;
  //m_13442:
  for (;;)
  {
    if (RG4UW >= mkuw(party.heros_in_party))
      return;
    //m_13424:
    RG1L = 263 * unsignedlong(RG4W);
    RG2Blo = CUTX8(ddat.v1e0258);
    party.hero[RG4W].absdir = RG2Blo;
    RG4L++;
  }
  FOREND
}

// SKW_2c1d_14ba
i32 DM2_RESUME_FROM_WAKE(void)
{
  ddat.v1e0488 = true;
  ddat.v1e0238 = 0;
  ddat.ticktrig = lcon(0x8);
  DM2_INIT_BACKBUFF();
  return DM2_1031_0541(5);
}

// was SKW_2759_0602
void DM2_SET_SPELLING_CHAMPION(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG4L = RG1L;
  RG2L = unsignedlong(RG1W);
  RG1L = 263 * RG2L;
  if (party.hero[RG2L].curHP != 0)
  {
    RG3L = 2;
    party.curactmode = RG3W;
    ddat.v1e0b52 = RG3W;
    RG4L++;
    party.curacthero = RG4W;
    RG1L = RG4L;
    ddat.v1e0b54 = RG4W;
    ddat.v1e0b6c = 1;
    RG1L ^= RG4L;
    ddat.v1e0b62 = RG1W;
    DM2_CHAMPION_SQUAD_RECOMPUTE_POSITION();
    DM2_UPDATE_RIGHT_PANEL(0);
  }
}

void DM2_PLAYER_CONSUME_OBJECT(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG61;
  i16 RG62w;
  c_wlreg _RG63;
  i16 RG64w;
  i32 RG65l;
  c_lreg _RG66;
  c_ql32 vql_00;
  i16 vw_04;
  c_ql32 vql_08;
  i32 vl_0c;
  i32 vl_10;
  i32 vl_14;
  i32 vl_18;
  i16 vw_1c;
  i16 parw01;
  i16 parw02;

  vql_08.poke32(RG1L);
  vw_04 = RG4W; // truncated
  vql_00.poke32(RG2L);
  RG3L = 1;
  if (RG4W == lcon(0xffffffff))
  {
    //m_29D0B:
    RG2W = ddat.savegamewpc.w_00;
    if (RG2W == RG4W)
    {
      DM2_MOUSE_SET_CAPTURE();
      ddat.vcapture2 = true;
      DM2_GET_MOUSE_ENTRY_DATA(OO ddat.v1e00b6, OO ddat.v1e00b4, OO ddat.v1e00b2);
      RG1W = ddat.v1e00b2 & 2;
      RG1L = signedlong(RG1W);
      if (RG1L == 0)
      {
        DM2_MOUSE_RELEASE_CAPTURE();
        ddat.vcapture2 = false;
        return;
      }
      //m_29D57:
      Tmouse.hide();
      return;
    }
    //m_29D6A:
    RG1W = ddat.v1e0976;
    RG1L--;
    vql_08.poke16(RG1W);
    vw_04 = RG2W; // truncated
    vql_00.poke16(lcon(0xffffffff));
  }
  else
    RG3L = 0;
  //m_29D7E:
  if (ddat.v1e0288 != 0)
    return;
  RG1L = signedlong(vql_08.peek16());
  vl_10 = RG1L;
  c_hero* hero = &party.hero[RG1L];
  RG2L = unsignedlong(vw_04);
  RG1L = DM2_QUERY_GDAT_FOOD_VALUE_FROM_RECORD(RG2W);
  vl_18 = RG1L;
  if (RG1W != 0)
  {
    if (RG3L != 0)
    {
      DM2_HIDE_MOUSE();
      RG1L = DM2_REMOVE_OBJECT_FROM_HAND();
      RG61L = 5;
      //m_29E10:
      for (;;)
      {
        if (--RG61W == 0)
        {
          DM2_SHOW_MOUSE();
          break;
        }
        //m_29DD6:
        RG3L = lcon(0x221);
        RG1L = RG61L;
        RG1W &= 0x1;
        RG1Blo = (unsignedlong(RG1W) == 0 ? 1 : 0) + lcon(0x25);
        RG2L = unsignedlong(RG1Blo);
        DM2_DRAW_STATIC_PIC(7, 0, RG2Blo, RG3W, NOALPHA);
        DM2_DRAWINGS_COMPLETED();
        RG1L = lcon(0x8);
        DM2_SLEEP_SEVERAL_TIME(RG1W);
      }
    }
    //m_29E19:
    RG1W = hero->food;
    RG1L += vl_18;
    RG1L = signedlong(DM2_MIN(RG1W, wcon(0x800)));
    hero->food = RG1W;
    vl_14 = signedlong(vql_00.peek16());
    RG66L = unsignedlong(vw_04);
    DM2_PROCESS_ITEM_BONUS(signedlong(vql_08.peek16()), RG66L, vl_14, 2);
    DM2_DEALLOC_RECORD(RG66L);
    RG65l = vl_14;
    if (RG65l == lcon(0xffffffff))
      jump L_fin2;
    hero->item[RG65l] = -1;
    jump L_fin2;
  }
  //m_29E77:
  RG1L = DM2_IS_MISCITEM_DRINK_WATER(RG2L);
  if (RG1L != 0)
  {
    RG1W = hero->water;
    RG1L += lcon(0x320);
    RG1L = signedlong(DM2_MIN(RG1W, wcon(0x800)));
    hero->water = RG1W;
    RG1L = signedlong(vql_00.peek16());
    if (RG1L == lcon(0xffffffff))
      jump L_fin2;
    hero->item[RG1L] = RG2W;
    jump L_fin2;
  }
  //m_29EB6:
  RG1L = RG2L;
  RG1Blo ^= RG2Blo;
  RG1Bhi &= lcon(0x3c);
  RG1L = unsignedlong(RG1W) >> bcon(0xa);
  RG3L = RG1L;
  RG1L = unsignedlong(RG3W);
  if (RG1L != lcon(0x8))
    return;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
  RG3P = RG1P;
  RG1W = word_at(RG1P, 2);
  RG1W = unsignedword(RG1Blo);
  vw_1c = RG1W; // truncated
  RG2L = unsignedlong(RG1W);
  RG4L = RG2L + 1;
  RG1L = RG4L;
  RG1L -= jul_shl32(RG4L, lcon(0x3));
  RG1L >>= bcon(0x3);
  RG4L = lcon(0x1ff) - RG2L;
  RG1L += lcon(0x20);
  vl_0c = RG1L;
  RG1L = RG4L;
  RG4L = RG1L % vl_0c;
  RG1L /= vl_0c;
  RG1L >>= bcon(0x1);
  RG64w = RG1W;
  vl_0c = lcon(0x19);
  RG1L = RG2L;
  RG4L = RG1L % vl_0c;
  RG1L /= vl_0c;
  RG1L += lcon(0x8);
  RG2L = RG1L;
  RG1W = word_at(RG3P, 2); // TODO: gets extended w/o conversion now
  RG1L = 2 * RG1L;
  RG1UW >>= bcon(0x9);
  RG1L -= 6;
  if (RG1UW > mkuw(lcon(0x9)))
    return;
  RG1L = unsignedlong(RG1W);
  switch (RG1L)
  {
    case 0:
      //m_29F4F:
      RG2L = signedlong(RG2W);
      RG4L = 2;
      break;

    case 1:
      //m_29F63:
      RG1L = unsignedlong(vw_1c) / lcon(0x23) + lcon(0x5);
      RG2L = signedlong(RG1W);
      RG4L = 1;
      break;

    case 2:
      //m_29F7F:
      RG2L = signedlong(RG2W);
      RG4L = 3;
      break;

    case 3:
      //m_29F89:
      RG2L = signedlong(RG2W);
      RG4L = 4;
      break;

    case 4:
      //m_29F93:
      RG3L = unsignedlong(vw_1c);
      RG1L = RG3L;
      RG4L = RG1L % lcon(0x7);
      RG1L /= lcon(0x7);
      RG63L = RG1L;
      RG1L = unsignedlong(RG63W) * unsignedlong(RG63W);
      RG3L += RG1L;
      RG1L = unsignedlong(RG2W) + RG3L;
      RG4L = signedlong(RG1W);
      DM2_CURE_POISON(vl_10, RG4L);
      jump L_fin1;

    case 5:
      //m_29FC7:
      RG2L = unsignedlong(RG64w);
      RG1L = signedlong(hero->maxStamina);
      RG4L = RG1L % RG2L;
      RG1L /= RG2L;
      RG4L = signedlong(RG1W);
      RG1L = signedlong(DM2_MIN(hero->maxStamina - hero->curStamina, RG4W));
      hero->curStamina += RG1W;
      jump L_fin1;

    case 6:
      //m_29FF3:
      RG1L = unsignedlong(RG2W);
      RG4L = RG1L / 2;
      RG1L += RG4L;
      RG2L = RG1L;
      RG1L *= RG1L;
      RG4L = unsignedlong(RG1W);
      RG2L = signedlong(RG2W);
      RG3Blo = vql_08.peek8();
      RG1L = signedlong(CUTX16(1 << RG3Blo));
      DM2_PROCEED_ENCHANTMENT_SELF(RG1L, 2, RG2L, RG4L);
      jump L_fin1;

    case 7:
      //m_2A027:
      RG4W = hero->curMP;
      RG4L += RG2L;
      RG1L = RG2L - lcon(0x8) + RG4L;
      RG4L = signedlong(RG1W);
      RG1L = signedlong(DM2_MIN(wcon(0x384), RG4W));
      RG3L = RG1L;
      RG4L = signedlong(hero->maxMP);
      RG2L = unsignedlong(RG1W);
      if (RG2L > RG4L)
      {
        RG1L = signedlong(DM2_MAX(hero->curMP, RG4W));
        RG4L = RG2L - RG1L;
        RG1L = RG4L / 2;
        RG2L -= RG1L;
        RG3L = RG2L;
      }
      //m_2A06A:
      hero->curMP = RG3W;
      jump L_fin1;

    case 8:
      //m_2A073:
      RG1L = unsignedlong(vw_1c) / lcon(0x2a);
      RG4L = signedlong(RG1W);
      RG3L = signedlong(DM2_MAX(1, RG4W));
      RG4L = signedlong(hero->maxHP);
      RG2L = unsignedlong(RG64w);
      RG1L = RG4L;
      RG4L = RG1L % RG2L;
      RG1L /= RG2L;
      RG4L = signedlong(hero->curHP) + RG1L;
      hero->curHP = RG4W;
      RG4W = hero->bodyflag;
      if (RG4W == 0)
        jump L_fin1;
      RG62w = 10;
      //m_2A0BD:
      for (;;)
      {
        RG2L = 0;
        //m_2A0CB:
        for (;;)
        {
          if (RG2UW >= RG3UW)
          {
            RG3L = 1;
            if (RG4W != hero->bodyflag)
              jump L_fin1;
            RG62w -= RG3W;
            if (RG62w == 0)
              jump L_fin1;
            break;
          }
          //m_2A0C1:
          RG1L = DM2_RAND();
          hero->bodyflag &= RG1W;
          RG2L++;
        }
      }
      FOREND

    case 9:
      //m_2A0E2:
      RG1W = hero->water;
      RG1L += lcon(0x640);
      RG1L = signedlong(DM2_MIN(RG1W, wcon(0x800)));
      hero->water = RG1W;
      jump L_fin1;

    default: throw(THROW_DMABORT);
  }

  //m_29F57:
  DM2_hero_2c1d_0300(hero, RG4L, RG2L);

L_fin1:
  RG1L = signedlong(vql_00.peek16());
  if (RG1L == lcon(0xffffffff))
    RG1L = DM2_REMOVE_OBJECT_FROM_HAND();
  DM2_DEALLOC_RECORD(unsignedlong(vw_04));
  RG1L = DM2_ALLOC_NEW_RECORD(8);
  RG3L = RG1L;
  if (RG1W == lcon(0xffffffff))
    return;
  RG2L = unsignedlong(RG1W);
  DM2_SET_ITEMTYPE(RG2L, lcon(0x14));
  RG1L = signedlong(vql_00.peek16());
  if (RG1L == lcon(0xffffffff))
  {
    DM2_TAKE_OBJECT(RG2L, 0);
    jump L_fin2;
  }
  //m_2A152:
  hero->item[RG1L] = RG3W;

L_fin2:
  RG1W = hero->curStamina;
  RG4W = hero->maxStamina;
  if (RG1W > RG4W)
    hero->curStamina = RG4W;
  RG1W = hero->curHP;
  RG2W = hero->maxHP;
  if (RG1W > RG2W)
    hero->curHP = RG2W;
  RG1L = signedlong(ddat.v1e0272);
  put16(parw02, RG1W);
  RG1L = signedlong(ddat.v1e0270);
  put16(parw01, RG1W);
  DM2_QUEUE_NOISE_GEN2(lcon(0x16), hero->herotype, lcon(0x83), lcon(0xfe), parw01, parw02, 0, lcon(0x96), lcon(0x80));
  hero->heroflag |= 0x3800;
  RG1L = signedlong(vql_00.peek16());
  if (RG1L != lcon(0xffffffff))
  {
    hero->heroflag |= 0x4000;
    party.calc_player_weight((e_hero)vql_08.peek16());
  }
}

i32 DM2_ATTACK_PARTY(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_wlreg _RG6;
  i16 vw_00;
  i16 vw_04;
  c_o32 vo_08; // poke 16 peek 8 32 inc16
  i16 vw_0c;

  RG6L = RG1L;
  vw_04 = RG4W; // truncated
  vw_00 = RG2W; // truncated
  if (RG1W == 0)
    return RG1L;
  RG5L = 0;
  RG1L = unsignedlong(RG6W) / 8 + 1;
  vw_0c = RG1W; // truncated
  RG6L -= RG1L;
  RG2L = 2 * RG1L;
  vw_0c = RG2W; // truncated
  vo_08.poke16(RG5W);
  //m_377A2:
  for (;;)
  {
    RG1W = vo_08.peek16();
    if (RG1UW >= mkuw(party.heros_in_party))
      return RG5L;
    //m_37754:
    RG3L = unsignedlong(vw_00);
    RG2L = unsignedlong(vw_04);
    RG1W = DM2_RAND16(vw_0c) + RG6W;
    RG4L = signedlong(RG1W);
    RG1L = signedlong(DM2_MAX(1, RG4W));
    RG4L = signedlong(RG1W);
    RG1W = DM2_WOUND_PLAYER(signedlong(vo_08.peek16()), RG4L, RG2L, RG3L);
    if (RG1W != 0)
    {
      RG3Blo = vo_08.peek8();
      RG1L = lcon(0x1) << RG3Blo;
      RG5L |= RG1L;
    }
    vo_08.inc16();
  }
  FOREND
}

// was SKW_2c1d_1b0f
void DM2_PROCESS_POISON(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG6w;
  i32 RG7l;
  c_ql32 vql_00;
  c_tim c38_04;

  vql_00.poke32(RG1L);
  RG6w = RG4W;
  RG7l = signedlong(vql_00.peek16());
  if (RG7l == lcon(0xffffffff))
    return; // RG1W;
  RG1L = unsignedlong(ddat.v1e0288);
  RG4L = RG7l + lcon(0x1);
  if (RG4L == RG1L)
    return; // RG1W;
  c_hero* hero = &party.hero[RG7l];
  RG1L = 0;
  RG1W = RG6w;
  RG1L += lcon(0x1e);
  RG1L >>= bcon(0x6);
  RG4L = signedlong(RG1W);
  RG1L = signedlong(DM2_MAX(1, RG4W));
  RG4L = signedlong(RG1W);
  RG3L = 0;
  RG2L = 0;
  RG1W = DM2_WOUND_PLAYER(RG7l, RG4L, RG2L, RG3L);
  hero->heroflag |= 0x2800;
  if (--RG6w == wcon(0x0))
    return; // RG1W;
  RG1L = signedlong(hero->poison);
  RG4L = unsignedlong(RG6w);
  RG1L += RG4L;
  if (RG1L > lcon(0xc00))
  {
    RG6w = wcon(0xc00);
    RG6w -= hero->poison;
    if (RG6w == wcon(0x0))
      return; // RG1W;
  }
  hero->poison += RG6w;
  hero->poisoned++;
  c38_04.settype(0x4b);
  RG1Blo = vql_00.peek8();
  c38_04.setactor(vql_00.peek8());
  c38_04.setmticks(ddat.v1e0266, timdat.gametick + lcon(0x24));
  c38_04.setA(RG6w);
  DM2_QUEUE_TIMER(&c38_04);
}

i32 DM2_TAKE_COIN_FROM_WALLET(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_wlreg _RG6;

  RG2L = signedlong(RG4W);
  RG2W = ddat.v1e0394[RG2L];
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
  RG3P = RG1P;
  RG4W = word_at(RG1P, 2);
  RG5L = lcon(0xffff);
  //m_F5E2:
  for (;;)
  {
    if (RG4W == lcon(0xfffffffe))
      break;
    RG1L = RG4L;
    RG1Blo ^= RG4Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    if (RG1L != lcon(0xa))
      return lcon(0xffff);
    RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG4W);
    if (RG1W == RG2W)
      RG5L = RG4L;
    //m_F619:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
    RG4L = RG1L;
  }
  //m_F627:
  if (RG5W == lcon(0xffffffff))
    return RG5L;
  RG6L = unsignedlong(RG5W);
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG6UW));
  if (jz_test8(location(RG1P + lcon(0x3)), lcon(0x3f)))
  {
    DM2_CUT_RECORD_FROM(mk_record(RG6L), UPCAST(t_record, RG3P + 2), -1, 0);
    return RG5L;
  }
  //m_F652:
  RG4W = word_at(RG1P, 2);
  RG4L *= 4;
  RG4UW >>= bcon(0xa);
  RG4L--;
  RG4W &= lcon(0x3f);
  and8(location(RG1P + lcon(0x3)), lcon(0xffffffc0));
  RG4L <<= bcon(0x8);
  or16(location(RG1P + 2), RG4W);
  return DM2_ALLOC_NEW_DBITEM(unsignedlong(RG2W));
}

i32 DM2_ADD_COIN_TO_WALLET(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  i16 RG6w;
  i8* RG7p;

  RG6w = RG4W;
  RG4L = unsignedlong(RG1W);
  RG1L = DM2_IS_CONTAINER_MONEYBOX(RG4L);
  if (RG1L == 0)
    return 0;
  RG2L = unsignedlong(RG6w);
  RG1L = DM2_IS_MISCITEM_CURRENCY(RG2L);
  if (RG1L == 0)
    return 0;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
  RG2P = RG1P;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
  RG7p = RG1P;
  RG4W = word_at(RG1P, 2);
  //m_F509:
  for (;;)
  {
    if (RG4W == lcon(0xfffffffe))
      break;
    RG1L = RG4L;
    RG1Blo ^= RG4Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    if (RG1L != lcon(0xa))
      return 0;
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
    RG3W = word_at(RG1P, 2) & lcon(0x7f);
    RG5w = word_at(RG2P, 2);
    RG5w &= wcon(0x7f);
    if (RG3W == RG5w)
    {
      RG3W = word_at(RG1P, 2) & lcon(0x3f00);
      if (RG3UW < mkuw(lcon(0x3f00)))
      {
        RG3W = word_at(RG1P, 2);
        RG3L *= 4;
        RG3UW >>= bcon(0xa);
        RG3L++;
        RG3W &= lcon(0x3f);
        and8(location(RG1P + lcon(0x3)), lcon(0xffffffc0));
        RG3L <<= bcon(0x8);
        or16(location(RG1P + 2), RG3W);
        RG1L = unsignedlong(RG6w);
        DM2_DEALLOC_RECORD(RG1L);
        break;
      }
    }
    //m_F585:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
    RG4L = RG1L;
  }
  //m_F596:
  if (RG4W == lcon(0xfffffffe))
  {
    and8(location(RG2P + lcon(0x3)), lcon(0xffffffc0));
    RG4P = RG7p + 2;
    RG1L = unsignedlong(RG6w);
    DM2_APPEND_RECORD_TO(mk_record(RG1L), UPCAST(t_record, RG4P), -1, 0);
  }
  return lcon(0x1);
}

// belongs to DM2_LOAD_PROJECTILE_TO_HAND
// TODO returns bool
static i32 DM2_2759_0e93(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG51w;
  i16 RG52w;
  i16 RG53w;
  i16 RG6w;
  bool flag = false;

  RG51w = RG1W;
  RG6w = RG2W;
  if (RG4W == lcon(0xffffffff))
    return flag ? 1 : 0;
  RG4L = unsignedlong(RG4W);
  RG1L = DM2_IS_ITEM_HAND_ACTIVABLE(unsignedlong(RG51w), RG4L, lcon(0xffffffff));
  if (RG1L != 0)
  {
    RG53w = wcon(0x0);
    //m_30A4E:
    for (;;)
    {
      RG1L = signedlong(RG53w);
      RG4L = unsignedlong(ddat.v1e0b7a);
      if (RG1L < RG4L)
      {
        //m_30A12:
        RG2L = unsignedlong(ddat.v1e0b40[RG1L].b_02);
        RG4L = unsignedlong(ddat.v1e0b40[RG1L].b_01);
        RG1L = DM2_QUERY_CMDSTR_ENTRY(unsignedlong(ddat.v1e0b40[RG1L].b_00), RG4L, RG2L, 2) & lcon(0xffff);
        RG4L = signedlong(RG6w);
        if (RG1L != RG4L)
        {
          //m_30A4D:
          RG53w++;
          continue;
        }
        flag = true;
      }
      break;
    }
  }
  //m_30A5E:
  RG52w = party.curacthero;
  if (RG52w == 0)
    return flag ? 1 : 0;
  if (party.curactmode != 0)
  {
    RG1L = signedlong(party.curactmode);
    if (RG1L != lcon(0x1))
      return flag ? 1 : 0;
  }
  RG2L = signedlong(party.curactmode);
  RG52w--;
  RG4W = party.hero[RG52w].item[RG2L];
  RG4L = unsignedlong(RG4W);
  RG1L = unsignedlong(RG52w);
  DM2_IS_ITEM_HAND_ACTIVABLE(RG1L, RG4L, RG2L);
  return flag ? 1 : 0;
}

void DM2_LOAD_PROJECTILE_TO_HAND(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl; // range must be 0 or 1 because of c_hero::barr_20 and barr_42
  c_wlreg _RG51;
  c_wlreg _RG52; // probably two variables
  c_wlreg _RG53;
  c_wlreg _RG54;
  c_wlreg _RG55;
  c_wlreg _RG56;
  c_wlreg _RG57;
  c_wlreg _RG61;
  c_wlreg _RG62;
  i8* RG7p;
  c_ql32 vql_00;
  i16 vw_04;
  i16 vw_08;
  i16 vw_0c;

  vql_00.poke32(RG1L);
  RG61L = signedlong(vql_00.peek16());
  c_hero* hero = &party.hero[RG61L];
  hero->handcooldown[RG4W] = 0;
  if (hero->curHP == 0)
    return;
  if (RG4W >= 2)
    return;
  RG2W = RG4W;
  RG2Blo ^= 0x1;
  vw_04 = RG2W;
  vw_08 = RG4W;
  RG2L = unsignedlong(hero->handcmd[RG4W]);
  vw_0c = RG2W;
  hero->handcmd[RG4W] = -1;
  hero->handdefenseclass[RG4W] = 0;
  if (RG2W == lcon(0x20))
  {
    RG2L = unsignedlong(vw_04);
    if (hero->item[RG2L] != -1)
      return;
    RG51L = lcon(0xc);
    RG2L = unsignedlong(hero->item[12]);
    RG4L = unsignedlong(RG4W);
    RG1L = DM2_IS_MISSILE_VALID_TO_LAUNCHER(unsignedlong(vql_00.peek16()), RG4L, RG2L);
    if (RG1L != 0)
    {
      RG4L = unsignedlong(RG51W);
      RG57L = DM2_REMOVE_POSSESSION(unsignedlong(vql_00.peek16()), RG4L);
      RG2L = unsignedlong(vw_04);
      RG4L = unsignedlong(RG57W);
      DM2_EQUIP_ITEM_TO_HAND(unsignedlong(vql_00.peek16()), RG4L, RG2L);
      return;
    }
    //m_30C2B:
    RG51W = hero->item[12];
    RG4L = unsignedlong(RG51W);
    RG1L = DM2_IS_CONTAINER_CHEST(RG4W) ? 1 : 0;
    if (RG1L != 0)
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
      RG7p = RG1P + 2;
      RG51W = word_at(RG1P + 2, 0);
      //m_30C50:
      for (;;)
      {
        if (RG51W != lcon(0xfffffffe))
        {
          RG61L = unsignedlong(RG51W);
          RG4L = unsignedlong(vw_08);
          RG1L = unsignedlong(vql_00.peek16());
          RG2L = RG61L;
          RG1L = DM2_IS_MISSILE_VALID_TO_LAUNCHER(RG1L, RG4L, RG2L);
          if (RG1L == 0)
          {
            //m_30C85:
            RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG61UW)));
            RG51L = RG1L;
            continue;
          }
          DM2_CUT_RECORD_FROM(mk_record(RG61L), UPCAST(t_record, RG7p), -1, 0);
          RG2L = unsignedlong(vw_04);
          RG4L = unsignedlong(RG51W);
          DM2_EQUIP_ITEM_TO_HAND(unsignedlong(vql_00.peek16()), RG4L, RG2L);
          return;
        }
        break;
      }
    }
    //m_30C90:
    RG56L = lcon(0x7);
    //m_30CC0:
    for (;;)
    {
      RG1L = signedlong(RG56W);
      if (RG1L > lcon(0x9))
        return;
      //m_30C97:
      RG2W = hero->item[RG1L];
      RG2L = unsignedlong(RG2W);
      RG1L = DM2_IS_MISSILE_VALID_TO_LAUNCHER(unsignedlong(vql_00.peek16()), unsignedlong(vw_08), RG2L);
      if (RG1L != 0)
        break;
      RG56L++;
    }
    RG4L = unsignedlong(RG56W);
    RG55L = DM2_REMOVE_POSSESSION(unsignedlong(vql_00.peek16()), RG4L);
    RG2L = unsignedlong(vw_04);
    RG4L = unsignedlong(RG55W);
    DM2_EQUIP_ITEM_TO_HAND(unsignedlong(vql_00.peek16()), RG4L, RG2L);
    return;
  }
  //m_30CCD:
  if (RG2W != lcon(0x2a))
    return;
  RG4L = unsignedlong(RG4W);
  if (hero->item[RG4L] != -1)
    return;
  RG2L = signedlong(vw_0c);
  RG52L = lcon(0xc);
  RG4L = unsignedlong(hero->item[12]);
  RG1L = DM2_2759_0e93(RG61L, RG4L, RG2L);
  if (RG1L != 0)
  {
    RG4L = unsignedlong(RG52W);
    RG54L = DM2_REMOVE_POSSESSION(unsignedlong(vql_00.peek16()), RG4L);
    RG2L = unsignedlong(vw_08);
    RG4L = unsignedlong(RG54W);
    DM2_EQUIP_ITEM_TO_HAND(unsignedlong(vql_00.peek16()), RG4L, RG2L);
    return;
  }
  //m_30D2A:
  RG52W = hero->item[12];
  RG4L = unsignedlong(RG52W);
  RG1L = DM2_IS_CONTAINER_CHEST(RG4W) ? 1 : 0;
  if (RG1L != 0)
  {
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
    RG7p = RG1P + 2;
    RG52W = word_at(RG7p, 0);
    //m_30D4F:
    for (;;)
    {
      if (RG52W == lcon(0xfffffffe))
        break;
      RG2L = signedlong(vw_0c);
      RG62L = unsignedlong(RG52W);
      RG1L = DM2_2759_0e93(signedlong(vql_00.peek16()), RG62L, RG2L);
      if (RG1L != 0)
      {
        DM2_CUT_RECORD_FROM(mk_record(RG62L), UPCAST(t_record, RG7p), -1, 0);
        RG2L = unsignedlong(vw_08);
        RG4L = unsignedlong(RG52W);
        DM2_EQUIP_ITEM_TO_HAND(unsignedlong(vql_00.peek16()), RG4L, RG2L);
        return;
      }
      //m_30D85:
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG62UW)));
      RG52L = RG1L;
    }
  }
  //m_30D90:
  RG53L = lcon(0x7);
  //m_30DBD:
  for (;;)
  {
    RG1L = signedlong(RG53W);
    if (RG1L <= lcon(0x9))
    {
      //m_30D97:
      RG2L = signedlong(vw_0c);
      RG4L = unsignedlong(hero->item[RG1L]);
      RG1L = DM2_2759_0e93(signedlong(vql_00.peek16()), RG4L, RG2L);
      if (RG1L == 0)
      {
        RG53L++;
        continue;
      }
      RG4L = unsignedlong(RG53W);
      RG53L = DM2_REMOVE_POSSESSION(unsignedlong(vql_00.peek16()), RG4L);
    }
    else
    {
      RG1L = DM2_REMOVE_POSSESSION(unsignedlong(vql_00.peek16()), lcon(0xc));
      RG53L = RG1L;
      if (RG1W == lcon(0xffffffff))
        return;
    }
    break;
  }
  RG2L = unsignedlong(vw_08);
  RG4L = unsignedlong(RG53W);
  DM2_EQUIP_ITEM_TO_HAND(unsignedlong(vql_00.peek16()), RG4L, RG2L);
}

// was SKW_2c1d_1d64
i32 DM2_CAST_CHAMPION_MISSILE_SPELL(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG6w;
  i16 parw00;

  RG6w = RG4W;
  RG1L = unsignedlong(RG1W);
  c_hero* hero = &party.hero[RG1L];
  RG4L = signedlong(hero->curMP);
  RG1L = unsignedlong(RG3W);
  if (RG4L < RG1L)
    return 0;
  hero->curMP -= RG3W;
  hero->heroflag |= 0x800;
  RG1W = hero->curMP >> 5;
  RG4L = signedlong(RG1W);
  RG1L = signedlong(DM2_MIN(6, RG4W));
  RG4L = lcon(0xa) - RG1L;
  RG1L = RG4L;
  RG4L = signedlong(RG4W) << 2;
  RG3L = signedlong(RG2W);
  if (RG3L < RG4L)
  {
    RG2L += 4;
    RG1L = signedlong(RG2W) / 4;
  }
  RG1L = unsignedlong(RG1W);
  put16(parw00, RG1W);
  RG2L = unsignedlong(RG2W);
  RG4L = unsignedlong(RG6w);
  DM2_SHOOT_CHAMPION_MISSILE(hero, RG4L, RG2L, lcon(0x5a), parw00);
  return lcon(0x1);
}

// belongs to DM2_hero_2c1d_1de2
static i32 DM2_2c1d_0e23(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG1W = DM2_QUERY_ITEM_WEIGHT(RG1W);
  RG3L = unsignedlong(RG1W) / 2;
  RG4L = signedlong(RG3W);
  RG2L = 10;
  RG1L = signedlong(DM2_BETWEEN_VALUE(1, RG2W, RG4W));
  //m_36711:
  for (;;)
  {
    RG3L -= lcon(0xa);
    if (RG3W <= 0)
      return RG1L;
    RG4L = signedlong(RG3W) / 2;
    RG1L = unsignedlong(RG1W) + RG4L;
  }
  FOREND
}

i32 DM2_hero_2c1d_1de2(e_hero heroidx, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = heroidx; // (e_hero)
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG51l;
  i32 RG53l;
  i16 RG6w;
  i32 vl_00;
  i32 vl_04;
  c_o32 vo_08; // poke 16 peek 8 32 add16
  i16 vw_0c;
  i16 vw_10;
  i16 vw_14;
  i16 parw00;
  i16 parw01;
  i8 parb06;
  i8 parb07;
  i8 parb08;

  RG6w = RG1W;
  vw_14 = RG4W;
  vl_04 = RG2L;
  RG51l = unsignedlong(RG1W);
  c_hero* hero = &party.hero[RG51l];
  RG3L = 0;
  if (RG4W < 0)
  {
    if (ddat.savegamewpc.w_00 == lcon(0xffffffff))
      return 0;
    //m_37993:
    RG1W = hero->item[1];
    vl_00 = RG1L;
    RG1L = DM2_REMOVE_OBJECT_FROM_HAND();
    vw_0c = RG1W; // truncated
    hero->item[1] = RG1W;
    vw_14 = 1;
    RG3L = 1;
  }
  //m_379B9:
  RG4L = signedlong(vw_14);
  RG1W = DM2_COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(RG6w, RG4L, 10);
  vo_08.poke16(RG1W);
  if (RG3L == 0)
  {
    //m_379E2:
    RG4L = unsignedlong(vw_14);
    RG1L = DM2_REMOVE_POSSESSION(unsignedlong(RG6w), RG4L);
    vw_0c = RG1W; // truncated
    if (RG1W == lcon(0xffffffff))
      return 0;
  }
  else
  {
    RG1L = vl_00;
    hero->item[1] = RG1W;
  }
  //m_379FD:
  RG1L = signedlong(ddat.v1e0272);
  put16(parw01, RG1W);
  RG1L = signedlong(ddat.v1e0270);
  put16(parw00, RG1W);
  RG53l = unsignedlong(vw_0c);
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG53l);
  RG4L = unsignedlong(RG1Blo);
  RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG53l) & lcon(0xff);
  DM2_QUEUE_NOISE_GEN2(RG1Blo, RG4Blo, lcon(0x86), lcon(0xfe), parw00, parw01, lcon(0x1), lcon(0x6e), lcon(0x80));
  RG1L = DM2_2c1d_0e23(RG53l);
  RG4L = signedlong(RG1W);
  RG1L = signedlong(RG6w);
  DM2_ADJUST_STAMINA(RG1W, RG4W);
  RG2L = signedlong(vw_14);
  RG1L = unsignedlong(RG6w);
  DM2_ADJUST_HAND_COOLDOWN(RG1L, lcon(0x4), RG2L);
  vw_10 = 8;
  RG3L = 1;
  RG4L = lcon(0x9);
  RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG53l, RG4L);
  if (RG1W != 0)
  {
    RG4L = unsignedlong(RG1W) / 4 + lcon(0xc);
    vw_10 = RG4W;
    RG3L = RG1L;
  }
  RG2L = signedlong(vw_10);
  RG1L = unsignedlong(RG6w);
  DM2_ADJUST_SKILLS(RG1W, 10, RG2L);
  vo_08.add16(RG3W);
  RG1W = DM2_QUERY_PLAYER_SKILL_LV(RG6w, 10, 1);
  RG3W = RG1W;
  RG1W = vo_08.peek16();
  RG1L <<= 16;
  RG1L >>= 18;
  RG1L += lcon(0x8);
  RG1W = DM2_RAND16(RG1W) + RG3W;
  vo_08.add16(RG1W);
  RG1L = signedlong(DM2_MIN(vo_08.peek16(), wcon(0xff)));
  vo_08.poke16(RG1W);
  RG1L = DM2_RAND();
  RG1W &= lcon(0x1f);
  RG4L = 8 * RG3L;
  RG1L += RG4L;
  RG4L = signedlong(RG1W);
  RG2L = lcon(0xc8);
  RG1L = signedlong(DM2_BETWEEN_VALUE(40, RG2W, RG4W));
  vw_10 = RG1W;
  RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(vw_0c), lcon(0xc));
  if (RG1W == 0)
  {
    RG1L = lcon(0xb) - RG3L;
    RG4L = signedlong(RG1W);
    RG1L = signedlong(DM2_MAX(5, RG4W));
  }
  //m_37B4D:
  RG3L = RG1L;
  RG1L = unsignedlong(RG3Blo);
  put8(parb08, RG1Blo);
  RG1L = unsignedlong(CUTX8(vw_10));
  put8(parb07, RG1Blo);
  RG1L = unsignedlong(vo_08.peek8());
  put8(parb06, RG1Blo);
  RG1L = signedlong(ddat.v1e0258);
  i32 parl05 = RG1L;
  RG1W = ddat.v1e0258;
  RG1L += vl_04;
  RG1W &= 0x3;
  RG3L = signedlong(RG1W);
  DM2_SHOOT_ITEM(unsignedlong(vw_0c), signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), RG3L, parl05, parb06, parb07, parb08);
  ddat.v1e025e = 4;
  ddat.v1e0274 = ddat.v1e0258;
  return lcon(0x1);
}

void DM2_CHANGE_PLAYER_POS(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG61w;
  c_ql32 vql_00;
  t_palette palette[PAL16];
  i16 vw_14;
  i16 parw01;
  i16 parw02;
  bool flag = true;

  vql_00.poke32(RG1L);
  RG1W &= lcon(0x8000);
  RG1L = unsignedlong(RG1W);
  if (RG1L != 0)
  {
    flag = false;
    RG4L = vql_00.peek32();
    RG4W &= 0x3;
    vql_00.poke16(RG4W);
  }
  RG1W = DM2_GET_PLAYER_AT_POSITION((vql_00.peek16() + ddat.v1e0258) & 0x3);
  vw_14 = RG1W;
  RG2L = RG1L;
  RG3W = ddat.v1e00b8;
  if (RG3W != 0)
  {
    //m_49B7:
    RG4L = RG3L - 1;
    ddat.v1e00b8 = 0;
    RG1W = DM2_GET_PLAYER_AT_POSITION((ddat.v1e0258 + RG4W) & 0x3);
    RG61w = RG1W;
    if (flag)
    {
      RG3Blo = CUTX8(ddat.v1e0258);
      party.hero[RG1W].absdir = RG3Blo;
    }
    if (RG4W != vql_00.peek16())
    {
      if (RG2W >= 0)
      {
        RG1Blo = CUTX8(ddat.v1e0258);
        RG4Blo += RG1Blo;
        RG4Blo &= 0x3;
        party.hero[RG2W].partypos = RG4Blo;
      }
      RG4Blo = vql_00.peek8() + CUTX8(ddat.v1e0258) & lcon(0x3);
      RG1L = 263 * signedlong(RG61w);
      party.hero[RG61w].partypos = RG4Blo;
    }
  }
  else
  {
    if (RG1W < 0)
      return;
    glblrects.dm2rect5.w = ddat.v1d272c;
    glblrects.dm2rect5.h = ddat.v1d272e;
    RG4L = signedlong(ddat.v1d272e);
    t_bmp* srcbmp = DM2_ALLOC_PICT_BUFF(ddat.v1d272c, RG4W, BIGPOOL_LO, lcon(0x4));
    t_bmp* destbmp = DM2_ALLOC_PICT_BUFF(ddat.v1d272c, ddat.v1d272e, BIGPOOL_LO, lcon(0x8));
    DM2_QUERY_GDAT_SQUAD_ICON(srcbmp, signedlong(vw_14), palette);
    DM2_FILL_ENTIRE_PICT(destbmp, c_pixel(E_COL12));
    RG1L = signedlong(ddat.v1d272c);
    RG3L = 0;
    RG2R = &glblrects.dm2rect5;
    ui16 destw;
    if (PIXELCAST(destbmp) == gfxsys.dm2screen)
      destw = ORIG_SWIDTH;
    else
      destw = ddat.v1d272c;

    blitter.blit(
      PIXELCAST(srcbmp),
      PIXELCAST(destbmp),
      /*TAG blitrect*/ RG2R,
      0,
      0,
      /*TAG srcw*/ ddat.v1d272c,
      /*TAG destw*/ destw,
      ALPHA_4,
      BLITMODE0,
      BPP_4,
      BPP_8,
      palette);

    DM2_HIDE_MOUSE();
    generate_cursor(PIXELCAST(destbmp), 3, 8, 6, ddat.v1d272c, ddat.v1d272e, BPP_8, NULL, palettecolor_to_ui8(palette[E_COL12]));
    DM2_CHOOSE_CURSOR3();
    DM2_FREE_PICT_BUFF(destbmp);
    DM2_FREE_PICT_BUFF(srcbmp);
    RG1L = vql_00.peek32();
    ddat.v1e00b0 = RG1W;
    RG1L++;
    ddat.v1e00b8 = RG1W;
    DM2_SHOW_MOUSE();
  }
  //m_4A45:
  DM2_REFRESHMOUSE();
}

// was SKW_443c_0662
void DM2_CHAMPION_SQUAD_RECOMPUTE_POSITION(void)
{
  if (ddat.v1e00b8 != 0)
    DM2_CHANGE_PLAYER_POS(ddat.v1e00b0 | 0x8000);
}
