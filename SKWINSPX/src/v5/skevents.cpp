#include <dm2data.h>
#include <skeventq.h>
#include <uitmouse.h>
#include <skhero.h>
#include <skgdtqdb.h>
#include <skmovrec.h>
#include <skrandom.h>
#include <skcloud.h>
#include <sklight.h>
#include <util.h>
#include <gfxstr.h>
#include <skimage.h>
#include <sfxsnd.h>
#include <skguidrw.h>
#include <SK1031.h>
#include <gfxmain.h>
#include <SK1C9A.h>
#include <sfx.h>
#include <skxrect.h>
#include <sktimer.h>
#include <skmap.h>
#include <skguivwp.h>
#include <skguidrw.h>
#include <sktimprc.h>
#include <bcreatur.h>
#include <startend.h>
#include <skmove.h>
#include <bitem.h>
#include <skengage.h>
#include <SK0AAF.h>
#include <skmcursr.h>
#include <bgdat.h>
#include <skevents.h>

#include <emu.h>
#include <regs.h>

// Note:
// here click- and timerevents
// subs of DM2_HANDLE_UI_EVENT and proceed-timer statics
// most of those routines end with return DM2_UPDATE_RIGHT_PANEL

// belongs to DM2_HANDLE_UI_EVENT
// was SKW_2c1d_0889
void DM2_CLICK_ITEM_SLOT(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  i16 RG6w;
  c_ql32 vql_00;
  bool skip00771 = false;

  RG3L = RG1L;
  RG1L = signedlong(eventqueue.event_heroidx);
  if (RG1L == lcon(0xffffffff))
    return;
  RG1L = unsignedlong(RG3W);
  if (RG1L >= lcon(0x8))
  {
    //m_36157:
    RG3L -= lcon(0x8);
    RG1L = unsignedlong(RG3W);
    if (RG1L < lcon(0x1e))
      //m_3616D:
      RG5w = ddat.v1e0976;
    else
      RG5w = party.curacthero;
    //m_36174:
    RG5w--;
  }
  else
  {
    if (ddat.v1e0288 != 0)
      return;
    RG1L >>= bcon(0x1);
    RG5w = RG1W;
    if (RG1UW >= mkuw(party.heros_in_party))
      return;
    RG4L = unsignedlong(RG1W);
    RG1L = signedlong(ddat.v1e0976);
    RG2L = RG4L + lcon(0x1);
    if (RG2L == RG1L)
      return;
    RG1L = 263 * RG4L;
    if (party.hero[RG4L].curHP == 0)
      return;
    RG3W &= 0x1;
  }
  //m_36175:
  RG6w = ddat.savegamewpc.w_00;
  RG1L = unsignedlong(RG3W);
  RG2L = 2 * RG1L;
  if (RG1L < 30)
  {
    //m_36192:
    RG4L = unsignedlong(RG5w);
    RG1W = party.hero[RG4L].item[RG2L / 2];
  }
  else
    RG1W = party.hand_container[(RG2L - 60) / 2]; // EVIL! solved offsetproblem1 v1e0920
  //m_361AB:
  vql_00.poke32(RG1L);
  RG1L = vql_00.peek32();
  if (RG1W == lcon(0xffffffff))
  {
    if (RG6w == RG1W)
      return;
  }
  RG1Blo = RG6w != wcon(0xffff) ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  if (RG1L != 0)
  {
    RG4L = signedlong(RG3W);
    RG1L = DM2_IS_ITEM_FIT_FOR_EQUIP(RG6w, RG4L, 0);
    if (RG1L == 0)
      return;
  }
  //m_361E5:
  DM2_HIDE_MOUSE();
  if (RG6w != lcon(0xffffffff))
    RG1L = DM2_REMOVE_OBJECT_FROM_HAND();
  if (vql_00.peek16() != lcon(0xffffffff))
  {
    RG4L = unsignedlong(RG3W);
    RG1L = unsignedlong(RG5w);
    DM2_REMOVE_POSSESSION(RG1L, RG4L);
    DM2_TAKE_OBJECT(unsignedlong(vql_00.peek16()), lcon(0x1));
  }
  //m_3621B:
  if (RG6w != lcon(0xffffffff))
  {
    RG2L = unsignedlong(RG3W);
    RG4L = unsignedlong(RG6w);
    RG1L = unsignedlong(RG5w);
    DM2_EQUIP_ITEM_TO_HAND(RG1L, RG4L, RG2L);
  }
  //m_36235:
  DM2_events_2e62_0cfa(0);
  RG1L = unsignedlong(RG3W);
  if (RG1L != lcon(0xb) && RG1L != lcon(0x6) && RG1L != lcon(0xc))
  {
    if (RG1L < lcon(0x7) || RG1L > lcon(0x9))
    {
      RG1L = unsignedlong(RG3W);
      if (RG1L < lcon(0x1e))
        skip00771 = true;
    }
  }

  if (!skip00771)
  {
    //m_36264:
    ddat.v1e0b6c = 1;
    DM2_UPDATE_RIGHT_PANEL(0);
  }

  //m_36274:
  if (ddat.v1e0254 != 0)
  {
    ddat.v1e0254 = wcon(0x0);
    DM2_events_443c_0434();
  }
  DM2_SHOW_MOUSE();
}

// belongs to DM2_events_AB26 (and DM2_INIT)
void DM2_events_5BFB(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  i16 RG6w;
  i16 parw02;

  RG6w = RG1W;
  RG5w = RG4W;
  if (RG4W != 0)
  {
    //m_5C3F:
    if (RG4W == lcon(0xa))
      ddat.v1d26a2 = RG1W;
  }
  else
  {
    ddat.v1d26a0 = RG1W;
    put16(parw02, ddat.v1e0272);
    DM2_QUEUE_NOISE_GEN1(lcon(0x3), 0, lcon(0x8b), lcon(0x6c), lcon(0xc8), ddat.v1e0270, parw02, 0);
  }
  //m_5C4B:
  RG4L = signedlong(RG5w);
  RG1L = signedlong(RG6w);
  DM2_SOUND3(RG1W, RG4W);
}

// belongs to DM2_HANDLE_UI_EVENT
void DM2_events_AB26(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  t_text tarr_00[0x80];
  i16 vw_80;
  t_text tarr_84[0x4]; // TODO probably 0x2 only

  RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x1a), lcon(0x87), lcon(0x1), 0) ? 1 : 0;
  if (RG1L == 0)
    return;
  DM2_0aaf_02f8(lcon(0x87), 0);
  RG3P = ADRESSOF(t_text, tarr_00);
  RG2L = lcon(0x28);
  RG4L = lcon(0x87);
  RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x1a), bcon(0x87), bcon(0x28), RG3T);
  RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL11]);
  RG4L = unsignedlong(RG4Blo);
  DM2_DRAW_VP_RC_STR(lcon(0x15f), RG4W, RG1T);
  RG3P = ADRESSOF(t_text, tarr_00);
  RG2L = lcon(0x3c);
  RG4L = lcon(0x87);
  RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x1a), bcon(0x87), bcon(0x3c), RG3T);
  RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL11]);
  RG4L = unsignedlong(RG4Blo);
  DM2_DRAW_VP_RC_STR(lcon(0x170), RG4W, RG1T);
  RG1W = unsignedword(RG1Blo);
  tarr_84[0x1] = RG1Bhi;
  //m_AD1C:
  for (;;)
  {
    vw_80 = 8;
    //m_AC45:
    for (;;)
    {
      RG2W = vw_80 - 1;
      vw_80 = RG2W;
      if (RG2W < 0)
      {
        vw_80 = 8;
        break;
      }
      //m_ABD0:
      RG1Blo = CUTX8(vw_80) + bcon(0x30);
      tarr_84[0x0] = RG1Blo;
      RG1L = RG2L + lcon(0x160);
      RG3L = signedlong(RG1W);
      RG1Blo = (RG2W == ddat.v1d26a0 ? 1 : 0) + 2;
      RG2L = unsignedlong(RG1Blo);
      DM2_DRAW_STATIC_PIC(26, bcon(0x87), RG2Blo, RG3W, NOALPHA);
      if (vw_80 == 0)
      {
        //m_AC0F:
        RG3P = ADRESSOF(t_text, tarr_00);
        RG2L = lcon(0x29);
        RG4L = lcon(0x87);
        RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x1a), bcon(0x87), bcon(0x29), RG3T);
        RG2P = RG1P;
      }
      else
        RG2P = ADRESSOF(t_text, tarr_84);
      //m_AC28:
      RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL11]);
      RG4L = unsignedlong(RG4Blo);
      RG1L = signedlong(vw_80 + lcon(0x168));
      DM2_DRAW_VP_RC_STR(RG1W, RG4W, RG2T);
    }
    //m_ACD3:
    for (;;)
    {
      RG4W = vw_80 - 1;
      vw_80 = RG4W;
      if (RG4W < 0)
      {
        DM2_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
        RG1W = DM2_0aaf_0067(lcon(0x87));
        RG4W = RG1W;
        if (RG1W == lcon(0xdb))
          return;
        if (RG1UW < mkuw(lcon(0x9d)) || RG1UW > mkuw(lcon(0xa4)))
        {
          //m_AD27:
          RG1L = RG4L - lcon(0x95) & lcon(0xffff);
          RG4L = 0;
        }
        else
        {
          RG1L -= lcon(0x9d);
          RG1L = unsignedlong(RG1W);
          RG4L = lcon(0xa);
        }
        break;
      }
      //m_AC5E:
      RG1Blo = CUTX8(vw_80) + bcon(0x30);
      tarr_84[0x0] = RG1Blo;
      RG1L = RG4L + lcon(0x171);
      RG3L = signedlong(RG1W);
      RG1Blo = (RG4W == ddat.v1d26a2 ? 1 : 0) + 2;
      RG2L = unsignedlong(RG1Blo);
      DM2_DRAW_STATIC_PIC(26, bcon(0x87), RG2Blo, RG3W, NOALPHA);
      if (vw_80 == 0)
      {
        //m_AC9D:
        RG3P = ADRESSOF(t_text, tarr_00);
        RG2L = lcon(0x3d);
        RG4L = lcon(0x87);
        RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x1a), bcon(0x87), bcon(0x3d), RG3T);
        RG2P = RG1P;
      }
      else
        RG2P = ADRESSOF(t_text, tarr_84);
      //m_ACB6:
      RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL11]);
      RG4L = unsignedlong(RG4Blo);
      RG1L = signedlong(vw_80 + lcon(0x179));
      DM2_DRAW_VP_RC_STR(RG1W, RG4W, RG2T);
    }
    //m_AD17:
    DM2_events_5BFB(RG1L, RG4L);
  }
  FOREND
}

void DM2_events_38c8_0002(void)
{
  c_nreg _RG4;

  if (ddat.v1e13f0 != 0)
    return;
  if (ddat.dialog2 != 0)
    return;
  ddat.v1e13f0 = 1;
  if (ddat.v1e00b8 != 0)
  {
    DM2_CHAMPION_SQUAD_RECOMPUTE_POSITION();
    DM2_UPDATE_RIGHT_PANEL(0);
  }
  //m_44F25:
  DM2_1031_04F5();
  DM2_HIDE_MOUSE();
  DM2_move_12b4_0092();
  if (ddat.v1e0976 == 0)
  {
    RG4L = 0;
    DM2_FILL_HALFTONE_RECTI(9);
  }
  RG4L = 0;
  DM2_FILL_HALFTONE_RECTI(11);
  DM2_SHOW_MOUSE();
}

void DM2_events_38c8_0060(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;

  if (ddat.v1e13f0 == 0)
    return;
  RG2L = ddat.dialog2;
  if (RG2L != 0)
    return;
  if (ddat.v1e0238 != 0)
  {
    DM2_DRAW_WAKE_UP_TEXT();
    DM2_DRAWINGS_COMPLETED();
    return;
  }
  ddat.v1e13f0 = RG2L;
  if (party.curacthero != 0)
    //m_44FAA:
    ddat.v1e0b6c = 1;
  else
    ddat.v1e0b30 = 1;
  //m_44FB3:
  RG1W = ddat.v1e0976;
  if (RG1W != 0)
  {
    RG3L = 0;
    ddat.v1e0976 = RG3W;
    RG1L--;
    RG1L = signedlong(RG1W);
    DM2_guidraw_24a5_1798(RG1W);
    return;
  }
  DM2_INIT_BACKBUFF();
  DM2_guidraw_29ee_000f();
}

// belongs to DM2_HANDLE_UI_EVENT
// was SKW_2759_13f5
void DM2_CLICK_MAGICAL_MAP_RUNE(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG2L = unsignedlong(RG1W);
  RG4L = unsignedlong(table1d67fe[RG2L]);
  RG3Blo = RG1Blo;
  RG1L = lcon(0x1) << RG3Blo;
  RG3L = RG1L;
  RG2L = unsignedlong(ddat.v1e0b62);
  RG1L = unsignedlong(RG3W);
  if ((RG1L & RG2L) == 0)
  {
    //m_31164:
    c_hero* hero = &party.hero[party.curacthero - 1];
    RG2W = hero->curMP;
    if (RG4W > RG2W)
      return;
    hero->curMP = RG2W - RG4W;
    hero->heroflag |= 0x800;
  }
  else
    RG4L = -RG4L;
  //m_31195:
  ddat.v1e0b62 ^= RG3W;
  ddat.v1e0b4e += RG4W;
}

// belongs to DM2_HANDLE_UI_EVENT
void DM2_events_13262(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG3L = RG1L;
  RG4L = 0;
  //m_132D7:
  for (;;)
  {
    if (RG4W >= ddat.v1e0404)
      return;
    //m_1326B:
    RG2L = signedlong(RG4W);
    RG1L = 12 * RG2L;
    RG2L = ddat.v1e02f0[RG2L].b_0a;
    if (RG2W == RG3W)
    {
      RG1Blo = ddat.v1e02f0[RG1L / 0xc].b_0b;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L == lcon(0x4) || RG1L == lcon(0x6))
      {
        RG2L = signedlong(RG4W);
        RG1L = 3 * RG2L;
        RG4W = ddat.v1e02f0[RG2L].rc_00.y + ddat.v1d26fe;
        RG4L = unsignedlong(RG4W);
        RG1W = ddat.v1e02f0[RG2L].rc_00.x + ddat.v1d26fc;
        RG1L = unsignedlong(RG1W);
        DM2_CLICK_VWPT(RG1L, RG4L);
        return;
      }
    }
    RG4L++;
  }
  FOREND
}

// belongs to DM2_CLICK_VWPT
static i32 DM2_PUSH_PULL_RIGID_BODY(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  i16 RG6w;
  i16 vw_00;
  i16 vw_04;
  i16 vw_08;
  c_o32 vo_0c; // poke 16 peek 8 16 32 inc16
  i16 parw00;
  i16 parw01;
  bool skip00232 = false;
  bool skip00231 = false;

  if (RG1UW > mkuw(lcon(0x5)))
    return RG1L;
  RG1L = unsignedlong(RG1W);
  switch (RG1L)
  {
    case 0:
      //m_1406B:
      RG6w = ddat.v1e0258;
      RG5w = wcon(0x0);
      skip00231 = true;
      break;

    case 1:
      //m_14088:
      RG6w = ddat.v1e0258;
      RG6w++;
      RG6w &= wcon(0x3);
      RG5w = wcon(0x2);
      skip00231 = true;
      break;

    case 2:
      //m_140BA:
      vo_0c.poke16(lcon(0x4));
      RG5w = wcon(0x1);
      skip00232 = true;
      break;

    case 3:
      //m_140A1:
      vo_0c.poke16(lcon(0x5));
      RG5w = wcon(0x0);
      skip00232 = true;
      break;

    case 4:
      //m_140AC:
      vo_0c.poke16(lcon(0x6));
      RG5w = wcon(0x2);
      skip00232 = true;
      break;

    case 5:
      //m_14076:
      RG6w = ddat.v1e0258;
      RG6w--;
      RG6w &= wcon(0x3);
      RG5w = wcon(0x1);
      skip00231 = true;
      break;

    default: throw(THROW_DMABORT);
  }

  if (skip00232)
  {
    //m_140C6:
    RG6w = ddat.v1e0258;
    RG6w += wcon(0x2);
    RG6w &= wcon(0x3);
  }

  if (skip00231)
    //m_14098:
    vo_0c.poke16(lcon(0x3));

  //m_140D3:
  RG1L = signedlong(ddat.v1e0258);
  RG4W = ddat.v1e0270;
  RG2W = table1d27fc[RG1L];
  RG4L += RG2L;
  vw_08 = RG4W;
  RG4W = ddat.v1e0272;
  RG3W = table1d2804[RG1L];
  RG4L += RG3L;
  vw_00 = RG4W;
  RG2L = signedlong(RG6w);
  RG4L = signedlong(vw_00);
  RG1L = signedlong(vw_08);
  RG3P = ADRESSOF(i16, &vw_04);
  RG1L = DM2_IS_CREATURE_MOVABLE_THERE(RG1L, RG4L, RG2L, RG3P16);
  if (RG1L == 0)
    return RG1L;
  RG1L = DM2_move_12b4_099e(unsignedlong(vw_04));
  if (RG1L != 0)
  {
    //m_141BF:
    RG1L = unsignedlong(vw_08);
    ddat.v1e053c = RG1W;
    RG1L = unsignedlong(vw_00);
    ddat.v1e053a = RG1W;
    ddat.v1e0546 = ddat.v1e0270;
    ddat.v1e0544 = ddat.v1e0272;
    ddat.v1e0542 = RG6w;
    ddat.v1e0540 = ddat.v1e0266;
    ddat.v1e0538 = RG5w;
    ddat.v1d4000 = vw_04;
    return DM2_PERFORM_MOVE(vo_0c.peek16());
  }
  vo_0c.poke16(wcon(0x0));
  //m_141B0:
  for (;;)
  {
    RG1W = vo_0c.peek16();
    if (RG1UW >= mkuw(party.heros_in_party))
      return RG1L;
    //m_14145:
    RG2L = unsignedlong(RG1W);
    if (party.hero[RG2L].curHP != 0)
    {
      bool skip00229 = false;
      RG1L = signedlong(eventqueue.event_heroidx);
      if (RG2L == RG1L)
        skip00229 = true;
      else
      {
        RG1L = signedlong(DM2_RANDDIR());
        if (RG1W == 0)
          skip00229 = true;
      }

      if (skip00229)
      {
        //m_14176:
        RG1L = signedlong(ddat.v1e0272);
        put16(parw01, RG1W);
        RG1L = signedlong(ddat.v1e0270);
        put16(parw00, RG1W);
        RG4L = unsignedlong(vo_0c.peek8());
        RG3L = lcon(0xfe);
        DM2_QUEUE_NOISE_GEN2(lcon(0x16), RG4Blo, lcon(0x82), RG3Blo, parw00, parw01, lcon(0x1), lcon(0x69), lcon(0xc8));
      }
    }
    //m_141AB:
    vo_0c.inc16();
  }
  FOREND
}

// belongs to DM2_CLICK_VWPT
// was SKW_121e_00be
static void DM2_PLAYER_TESTING_WALL(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 parw00;
  i16 parw01;
  i16 parw02;

  if (party.heros_in_party == 0)
    return;
  if (RG1L == 0)
  {
    //m_1273F:
    put16(parw02, RG1W);
    RG1L = signedlong(RG2W);
    put16(parw01, RG1W);
    RG1L = signedlong(RG4W);
    put16(parw00, RG1W);
    DM2_QUEUE_NOISE_GEN2(lcon(0x8), ddat.v1d6c02, lcon(0x84), lcon(0xfe), parw00, parw01, parw02, lcon(0x8c), lcon(0xc8));
  }
  else
  {
    if (ddat.vcapture1)
      return;
    DM2_MOUSE_SET_CAPTURE();
    ddat.vcapture1 = true;
    DM2_GET_MOUSE_ENTRY_DATA(OO ddat.v1e00b6, OO ddat.v1e00b4, OO ddat.v1e00b2);
    RG1W = ddat.v1e00b2 & 2;
    RG1L = signedlong(RG1W);
    if (RG1L != 0)
      //m_12738:
      DM2_HIDE_MOUSE();
    else
    {
      DM2_MOUSE_RELEASE_CAPTURE();
      ddat.vcapture1 = false;
    }
  }
  //m_1276E:
  ddat.glbRefreshViewport = true;
}

// belongs to DM2_CLICK_VWPT
static i32 DM2_events_32cb_0287(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  i16 RG6w;
  i32 vl_00;
  unk* xp_04;
  i16 vw_08;
  i16 vw_0c;
  i16 vw_10;
  i16 vw_14;

  RG6w = RG1W;
  vw_10 = RG4W;
  vw_0c = RG2W;
  RG4L = 0;
  vl_00 = RG4L;
  RG5l = signedlong(RG1W);
  if (RG5l > lcon(0x3))
    return 0;
  //m_3DE98:
  xp_04 = ddat.ptr1e1044;
  RG2L = lcon(0x400);
  RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x48));
  ddat.ptr1e1044 = RG1P;
  vw_14 = ddat.v1e12cc;
  vw_08 = ddat.v1e12ca;
  DM2_CALC_VECTOR_W_DIR(ddat.v1e12c8, signedword(table1d6ad0[RG5l][1]), signedword(table1d6ad0[RG5l][0]), &vw_14, &vw_08);
  RG1L = 8 * RG5l;
  RG4L = 2 * (RG5l + RG1L);
  RG2P = ddat.ptr1e1044;
  RG1Blo = mkb(vw_14); // truncated
  mov8(location(RG2P + RG4L), RG1Blo);
  RG2P = ddat.ptr1e1044;
  RG1Blo = mkb(vw_08); // truncated
  mov8(location(RG2P + RG4L + lcon(0x1)), RG1Blo);
  RG3L = signedlong(vw_08);
  RG2L = signedlong(vw_14);
  RG4P = ddat.ptr1e1044 + RG4L;
  DM2_SUMMARIZE_STONE_ROOM(UPCAST(s_8wordarray, RG4P + 2), signedlong(ddat.v1e12c8), RG2L, RG3L);
  RG4L = signedlong(table1d6afe[RG5l]);
  RG2L = 0;
  RG1L = DM2_guivp_32cb_15b8(RG5l, RG4L, RG2L);
  if (RG1W >= 0)
  {
    RG2L = signedlong(RG1W);
    vl_00 = DM2_guivp_32cb_00f1(signedlong(vw_10), signedlong(vw_0c), RG2L) ? 1 : 0;
  }
  dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0x48));
  ddat.ptr1e1044 = DOWNCAST(unk, xp_04);
  RG1L = signedlong(RG6w);
  if (RG1L == lcon(0x3))
    return lcon(0x1);
  //m_3DF94:
  RG1L = signedlong(dm2_image2.imgdesc.width);
  if (RG1L < lcon(0x20))
    return lcon(0x1);
  RG1L = signedlong(dm2_image2.imgdesc.height);
  return RG1L >= lcon(0x20) ? vl_00 : lcon(0x1);
}

// belongs to DM2_CLICK_VWPT-subs
// stacksize was 0x38
static i32 DM2_events_32cb_03a6(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1, i16 argw2, i32 argl3)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG54;
  c_wlreg _RG55;
  i16 RG56w;
  c_wlreg _RG6;
  i8* RG71p;
  c_lreg _RG72;
  c_ql32 vql_00;
  c_ql32 vql_04;
  c_ql32 vql_08;
  i32 vl_0c;
  unk* xp_10;
  unk* xp_14;
  i16 vw_18;
  i32 vl_1c;
  i32 vl_20;
  c_ql32 vql_24;
  i16 parw00;
  unk* xparp01;
  i16 parw03;

  vql_08.poke32(RG1L);
  vql_04.poke32(RG4L);
  vql_00.poke32(RG2L);
  RG1L = RG3L;
  RG4L = unsignedlong(mkuw(argl1)) >> bcon(0xe); // truncated TODO check if arg is always word
  vw_18 = RG4W;
  RG4L = signedlong(argw2);
  if (RG4L != 2)
  {
    //m_3E002:
    RG71p = NULL;
    RG4L = signedlong(argw0);
    RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(RG3W, RG4W));
  }
  else
  {
    RG4L = unsignedlong(argw0);
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(CUTX16(DM2_GET_CREATURE_AT(unsignedlong(RG1W), RG4L) & lcon(0xffff)))));
    RG71p = RG1P;
    RG1W = word_at(RG1P, 2);
  }
  //m_3E013:
  vql_24.poke32(RG1L);
  if (vql_24.peek16() == lcon(0xfffffffe))
    return (argl3 == 0) ? lcon(0xffff) : argl1;
  //m_3E039:
  RG1L = signedlong(argw2);
  if (RG1L != lcon(0x3))
    //m_3E04C:
    RG6L = 0;
  else
    RG6L = 2;
  //m_3E04E:
  vl_1c = lcon(0xffff);
  RG4P = NULL;
  xp_14 = UPCAST(unk, RG4P);
  //m_3E05C:
  for (;;)
  {
    RG1L = vql_24.peek32();
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    RG1L = signedlong(RG1W);
    if (RG1L >= lcon(0x5) && RG1L <= lcon(0xa))
    {
      RG1L = unsignedlong(vql_24.peek16());
      vl_0c = RG1L;
      RG1L >>= 14;
      RG4L = signedlong(vw_18);
      if (RG1L == RG4L)
      {
        RG54L = 0;
        RG3L = 0;
        xp_10 = UPCAST(unk, RG3P);
        RG1L = DM2_IS_CONTAINER_CHEST(CUTX16(vl_0c)) ? 1 : 0;
        if (RG1L != 0)
        {
          RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(vl_0c);
          RG4L = unsignedlong(RG1Blo);
          RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x14), RG4Blo, lcon(0x1), lcon(0x48)) ? 1 : 0;
          if (RG1L != 0)
          {
            RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(CUTX16(vl_0c))));
            xp_10 = UPCAST(unk, RG1P);
            if (jz_test8(location(RG1P + lcon(0x4)), lcon(0x1)))
            {
              //m_3E0F1:
              if (argl3 == 0)
                RG54L = 2;
            }
            else
              RG54L = 3;
          }
        }
        //m_3E0FD:
        for (;;)
        {
          RG1L = signedlong(argw2);
          RG1Blo = RG1L == lcon(0x3) ? 1 : 0;
          RG1L = unsignedlong(RG1Blo);
          i32 parl04 = RG1L;
          RG1L = signedlong(RG54W);
          put16(parw03, RG1W);
          xparp01 = RG71p;
          RG1L = signedlong(RG6W);
          put16(parw00, RG1W);
          RG4L = signedlong(vql_00.peek16());
          RG1L = unsignedlong(vql_24.peek16());
          RG3L = 0;
          DM2_DRAW_ITEM(RG1L, RG4L, 0, RG3L, parw00, xparp01, 0, parw03, parl04);
          RG4L = signedlong(vql_04.peek16());
          RG1L = signedlong(vql_08.peek16());
          RG2L = lcon(0xa);
          RG1L = DM2_guivp_32cb_00f1(RG1L, RG4L, RG2L) ? 1 : 0;
          if (RG1L != lcon(0x1))
          {
            //m_3E165:
            if (RG54W != 0)
            {
              RG54L = 0;
              continue;
            }
          }
          else
          {
            xp_14 = UPCAST(unk, DOWNCAST(unk, xp_10));
            vl_1c = vql_24.peek32();
            vl_20 = RG54L;
          }
          break;
        }
        //m_3E16E:
        RG1L = signedlong(argw2);
        RG6L++;
        if (RG1L != lcon(0x3))
          //m_3E18A:
          RG6L &= lcon(0xf);
        else
        {
          RG1L = signedlong(RG6W);
          if (RG1L >= lcon(0xe))
            RG6L = 2;
        }
      }
    }
    //m_3E18D:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_24.peek16()))));
    vql_24.poke32(RG1L);
    if (RG1W == lcon(0xfffffffe))
    {
      RG1P = DOWNCAST(unk, xp_14);
      xp_10 = UPCAST(unk, RG1P);
      if (RG1P != NULL)
      {
        RG55L = vl_20;
        if (RG55UW < mkuw(2))
        {
          //m_3E1CE:
          if (RG55W != 0)
            return lcon(0xffff);
          if (argl3 != 0)
            return lcon(0xfffe);
          //m_3E1E8:
          RG2Blo = byte_at(RG1P, lcon(0x4));
          if ((RG2Blo & bcon(0x1)) == 0)
            return vl_1c;
          //m_3E1F9:
          RG2Bhi = RG2Blo & lcon(0xfffffffe);
          mov8(location(RG1P + lcon(0x4)), RG2Bhi);
          return lcon(0xffff);
        }
        if (RG55UW <= mkuw(2))
        {
          //m_3E206:
          or8(location(RG1P + lcon(0x4)), lcon(0x1));
          return lcon(0xffff);
        }
        if (RG55W != lcon(0x3))
          return lcon(0xffff);
        //m_3E20F:
        if (argl3 != 0)
        {
          RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(mkuw(argl1)), lcon(0x4)); // truncated TODO check if arg is always word
          RG1W &= lcon(0x8000);
          RG1L = unsignedlong(RG1W);
          if (RG1L != 0)
            return lcon(0xfffe);
        }
        RG1P = DOWNCAST(unk, xp_10);
        RG1W = word_at(RG1P, 2);
        RG56w = RG1W;
        RG4L = 0;
        //m_3E23F:
        for (;;)
        {
          if (RG1W == lcon(0xfffffffe))
            break;
          RG56w = RG1W;
          RG1L = unsignedlong(RG1W);
          RG4L++;
          RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG1UW)));
        }
        //m_3E254:
        RG1P = DOWNCAST(unk, xp_10) + 2;
        if (argl3 != 0)
        {
          RG4L = signedlong(RG4W);
          if (RG4L >= lcon(0x8))
            return lcon(0xfffe);
          DM2_APPEND_RECORD_TO(mk_record(unsignedlong(mkw(argl1))), UPCAST(t_record, RG1P), -1, 0);
          return lcon(0xffff);
        }
        if (RG4W > 0)
        {
          RG72L = unsignedlong(RG56w);
          DM2_CUT_RECORD_FROM(mk_record(RG72L), UPCAST(t_record, RG1P), -1, 0);
          ddat.v1e0ff6 = 1;
          DM2_TAKE_OBJECT(RG72L, lcon(0x1));
        }
        return lcon(0xffff);
      }
      break;
    }
  }
  //m_3E2BE:
  RG3L = vl_1c;
  if (RG3W != lcon(0xffffffff))
    return RG3L;
  return argl1;
}

// belongs to DM2_CLICK_VWPT
static void DM2_events_121e_013a(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG51l;
  i32 RG52l;
  i32 RG53l;
  i16 RG61w;
  i32 RG62l;
  c_wlreg _RG7;
  c_ql32 vql_00;
  c_ql32 vql_04;
  i16 vw_08;
  i16 parw00;
  i16 parw02;
  i16 parw04;

  vql_04.poke32(RG1L);
  vql_00.poke32(RG4L);
  RG1L = signedlong(eventqueue.event_heroidx);
  if (RG1L == lcon(0xffffffff))
    return;
  RG61w = ddat.v1e0270;
  RG3W = ddat.v1e0272;
  RG51l = unsignedlong(RG2W);
  RG1L = 3 * RG51l;
  RG4L = unsignedlong(ddat.v1e02f0[RG51l].b_0a);
  if (RG4W != 0)
  {
    if (RG4W != lcon(0x3))
      return;
    RG1L = signedlong(ddat.v1e0258);
    RG61w += table1d27fc[RG1L];
    RG3W += table1d2804[RG1L];
  }
  RG2L = unsignedlong(RG2W);
  RG52l = 12 * RG2L;
  RG1L = unsignedlong(ddat.v1e02f0[RG2L].b_0b);
  put16(parw02, RG1W);
  i32 parl01 = unsignedlong(ddat.v1e02f0[RG2L].w_08);
  RG7L = signedlong(RG3W);
  put16(parw00, RG7W);
  RG62l = signedlong(RG61w);
  RG2L = signedlong(RG4W);
  RG4L = signedlong(vql_00.peek16());
  RG1L = signedlong(vql_04.peek16());
  RG3L = RG62l;
  RG1L = DM2_events_32cb_03a6(RG1L, RG4L, RG2L, RG3L, parw00, parl01, parw02, 0);
  vw_08 = RG1W; // truncated
  if (RG1W != lcon(0xffffffff))
  {
    RG4L = unsignedlong(ddat.v1e02f0[RG52l / 0xc].b_0b);
    if (RG4L != 2)
      //m_1283D:
      put16(parw04, 0);
    else
      put16(parw04, lcon(0xffff));
    //m_1283F:
    RG1L = unsignedlong(RG1W);
    DM2_MOVE_RECORD_TO(RG1L, RG62l, RG7L, lcon(0xffffffff), parw04);
    ddat.v1e0ff6 = 1;
    RG53l = unsignedlong(vw_08);
    RG1L = DM2_IS_CONTAINER_MONEYBOX(RG53l);
    if (RG1L != 0)
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(CUTX16(RG53l))));
      and8(location(RG1P + lcon(0x7)), lcon(0xfffffffb));
    }
    DM2_TAKE_OBJECT(unsignedlong(vw_08), lcon(0x1));
  }
  //m_12889:
  ddat.glbRefreshViewport = true;
}

// belongs to DM2_CLICK_VWPT (and subs)
static i32 DM2_eventa_121e_0222(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  i16 RG6w;
  i8* RG7p;
  i32 vl_00;
  i32 vl_04;
  i8 vb_08;
  i16 parw00;

  RG6w = RG1W;
  RG5w = RG4W;
  RG3P = NULL;
  RG1L = signedlong(eventqueue.event_heroidx);
  if (RG1L == lcon(0xffffffff))
    return 0;
  RG1W = ddat.v1e0258;
  RG1L += RG2L;
  RG1W &= 0x3;
  vl_04 = RG1L;
  RG4L = unsignedlong(RG5w);
  RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG6w), RG4L);
  if (RG1W != lcon(0xffffffff))
  {
    RG3W = RG1W;
    RG1P = DM2_QUERY_CREATURE_AI_SPEC_FROM_TYPE(RG3L);
    RG4P = RG1P;
    RG7p = RG1P;
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
    RG3P = RG1P;
    if (jnz_test8(location(RG4P), lcon(0x1)))
    {
      RG1L = unsignedlong(RG2W);
      if (RG1L >= lcon(0x4))
      {
        if (RG1L <= lcon(0x7))
        {
          if (jz_test8(location(RG4P + lcon(0x1f)), lcon(0x8)))
            return 0;
        }
      }
      RG1L = unsignedlong(RG2W);
      if (RG1L < lcon(0x4))
      {
        if (jz_test8(location(RG7p + lcon(0x1f)), lcon(0x20)))
          return 0;
      }
    }
  }
  //m_1291F:
  vl_00 = DM2_REMOVE_OBJECT_FROM_HAND();
  RG1L = unsignedlong(RG2W);
  if (RG1L < lcon(0x4) || RG1L > lcon(0x7))
  {
    //m_129B9:
    RG1L = signedlong(RG5w);
    put16(parw00, RG1W);
    RG3L = signedlong(RG6w);
    RG4L = vl_00;
    RG4Bhi &= lcon(0x3f);
    RG1L = (vl_04 << bcon(0xe) | RG4L) & lcon(0xffff);
    RG2L = 0;
    RG4L = lcon(0xffffffff);
  }
  else
  {
    RG1Blo -= 4;
    RG1Blo += CUTX8(ddat.v1e0258);
    RG1Blo &= 0x3;
    vb_08 = RG1Blo;
    if (RG3P != NULL)
    {
      RG2W = word_at(RG3P, 2);
      //m_12954:
      for (;;)
      {
        if (RG2W == lcon(0xfffffffe))
          break;
        RG4L = unsignedlong(RG2W);
        RG1L = RG4L >> bcon(0xe);
        RG3L = unsignedlong(vb_08);
        if (RG1L == RG3L)
        {
          RG1L = DM2_IS_CONTAINER_MONEYBOX(RG4L);
          if (RG1L != 0)
          {
            RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
            and8(location(RG1P + lcon(0x7)), lcon(0xfffffffb));
          }
        }
        //m_12984:
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG2UW)));
        RG2L = RG1L;
      }
    }
    //m_12992:
    RG1L = signedlong(RG5w);
    put16(parw00, RG1W);
    RG3L = signedlong(RG6w);
    RG4L = vl_00;
    RG4Bhi &= lcon(0x3f);
    RG1W = unsignedword(RG1Blo);
    RG1Blo = vb_08;
    RG1L <<= bcon(0xe);
    RG1L |= RG4L;
    RG1L = unsignedlong(RG1W);
    RG2L = lcon(0xffffffff);
    RG4L = RG2L;
  }
  //m_129DC:
  DM2_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw00);
  ddat.glbRefreshViewport = true;
  return RG4L;
}

// belongs to DM2_CLICK_VWPT
static i32 DM2_events_121e_03ae(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1, i16 argw2)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG51;
  i32 RG52l;
  i32 RG53l;
  c_wlreg _RG61;
  c_wlreg _RG62;
  c_wlreg _RG7;
  i16 parw00;
  i16 parw02;

  RG7L = signedlong(argw2);
  put16(parw02, RG7W);
  RG51W = ddat.v1e0258;
  RG51L += argl1;
  RG51L &= 0x3;
  RG51L <<= bcon(0xe);
  RG61W = ddat.savegamewpc.w_00;
  RG61L &= lcon(0xffff3fff);
  RG51L |= RG61L;
  RG53l = unsignedlong(RG51W);
  i32 parl01 = RG53l;
  RG62L = signedlong(RG3W);
  put16(parw00, RG62W);
  RG52l = signedlong(RG2W);
  RG2L = signedlong(argw0);
  RG4L = signedlong(RG4W);
  RG1L = DM2_events_32cb_03a6(signedlong(RG1W), RG4L, RG2L, RG52l, parw00, parl01, parw02, lcon(0x1));
  if (RG1W == lcon(0xffffffff))
  {
    DM2_REMOVE_OBJECT_FROM_HAND(); // return not needed
    return lcon(0x1);
  }
  //m_12ACB:
  if (RG1W != lcon(0xfffffffe))
    return 0;
  if (RG7L == 2)
  {
    RG1L = argl1 + lcon(0x4);
    RG2L = unsignedlong(RG1W);
    return DM2_eventa_121e_0222(RG52l, RG62L, RG2L);
  }
  RG2L = unsignedlong(mkuw(argl1)); // truncated TODO check if arg is always word
  return DM2_eventa_121e_0222(RG52l, RG62L, RG2L);
}

// belongs to DM2_CLICK_VWPT
static void DM2_events_121e_0003(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG2L = RG1L;
  RG3L = 1;
  RG1W = ddat.v1e0270;
  RG4W = ddat.v1e0272;
  if (RG2UW < mkuw(2))
  {
    //m_1261C:
    if (RG2W == RG3W)
    {
      RG2L = signedlong(ddat.v1e0258) + lcon(0x3) & lcon(0x3);
      RG1W += table1d27fc[RG2L];
      RG4W += table1d2804[RG2L];
    }
  }
  else
  {
    if (RG2UW <= mkuw(2))
    {
      //m_12662:
      RG2L = signedlong(ddat.v1e0258) + 1 & lcon(0x3);
      RG1W += table1d27fc[RG2L];
      RG4W += table1d2804[RG2L];
      RG3L = 3;
    }
    else
    {
      if (RG2W == lcon(0x3))
      {
        //m_12642:
        RG2L = signedlong(ddat.v1e0258);
        RG1W += table1d27fc[RG2L];
        RG4W += table1d2804[RG2L];
        RG3L = 2;
      }
    }
  }
  //m_12684:
  if (RG1W >= 0)
  {
    if (RG1W < mapdat.map_width)
    {
      if (RG4W >= 0)
      {
        if (RG4W < mapdat.map_height)
        {
          RG2L = unsignedlong(ddat.savegamewpc.w_00);
          i32 parl00 = RG2L;
          RG2W = ddat.v1e0258;
          RG2L += RG3L;
          RG2W &= 0x3;
          RG2L = unsignedlong(RG2W);
          RG4L = unsignedlong(RG4W);
          RG1L = unsignedlong(RG1W);
          DM2_events_3C1E5(RG1L, RG4L, RG2L, lcon(0xffff), parl00);
        }
      }
    }
  }
}

// belongs to DM2_events_121e_0351
static i32 DM2_events_37BBB(i16 eaxw)
{
  if (eventqueue.event_heroidx == E_NOHERO)
    return 0;
  return DM2_hero_2c1d_1de2(eventqueue.event_heroidx, lcon(0xffffffff), unsignedlong(eaxw));
}

// belongs to DM2_CLICK_VWPT
static void DM2_events_121e_0351(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  i16 RG6w;
  i16 vw_00;
  bool skip00214 = false;

  RG3L = 0;
  if (signedlong(ddat.v1e0530) != lcon(0x11))
    //m_12A12:
    vw_00 = 0x2fd;
  else
    vw_00 = 0x300;
  //m_12A18:
  RG6w = RG4W;
  RG5w = RG1W;
  RG1L = DM2_PT_IN_EXPANDED_RECT(vw_00, RG5w, RG6w) ? 1 : 0;
  if (RG1L == 0)
  {
    //m_12A36:
    RG1L = DM2_PT_IN_EXPANDED_RECT(vw_00 + 1, RG5w, RG6w) ? 1 : 0;
    if (RG1L == 0)
      skip00214 = true;
    else
      RG1L = 1;
  }
  else
    RG1L = 0;

  if (!skip00214)
  {
    //m_12A4D:
    RG1L = DM2_events_37BBB(RG1W);
    RG3L = RG1L;
  }

  //m_12A54:
  if (RG3L != 0)
    ddat.glbRefreshViewport = true;
}

// belongs to DM2_CLICK_VWPT
// stacksize was 0x14
static bool DM2_guivp_32cb_01b6(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16* argpw0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p;
  i16 RG6w;
  i16 vw_00;
  i16 vw_04;
  i16 parw00;

  RG6w = RG1W;
  vw_04 = RG4W;
  RG5p = DOWNCAST(i16, argpw0);
  RG4L = unsignedlong(RG3W);
  RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG2W), RG4L);
  vw_00 = RG1W; // truncated
  if (RG1W == lcon(0xffffffff))
    return false;
  //m_3DDCE:
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
  RG2P = RG1P;
  if (byte_at(RG1P + lcon(0x5)) != lcon(0xffffffff))
  {
    //m_3DDE4:
    RG4L = unsignedlong(byte_at(RG1P, lcon(0x5)));
    RG1L = 34 * RG4L;
    RG3P = DOWNCAST(c_creature, creatures) + RG1L;
  }
  else
    RG3P = NULL;
  //m_3DDFA:
  RG1L = unsignedlong(vw_00);
  put16(parw00, RG1W);
  DM2_QUERY_CREATURE_PICST(lcon(0x3), signedlong(table1d6b15[3]), RG2P, RG3P, parw00);
  RG4W = dm2_image2.alphamask;
  dm2_image2.alphamask = lcon(0xfffffffe);
  DM2_DRAW_TEMP_PICST();
  RG2L = signedlong(RG4W);
  RG4L = signedlong(vw_04);
  RG1L = DM2_guivp_32cb_00f1(signedlong(RG6w), RG4L, RG2L) ? 1 : 0;
  if (RG1L == 0)
    return false;
  RG1L = signedlong(ddat.v1e12da);
  if (RG1L < lcon(0xffffffb5))
  {
    mov16(location(RG5p), lcon(0x3));
    return true;
  }
  if (RG1L > lcon(0x4b))
  {
    mov16(location(RG5p), 1);
    return true;
  }
  mov16(location(RG5p), 0);
  return true;
}

void DM2_CLICK_VWPT(i32 eaxl, i32 edxl)
{
  c_nreg _RG4; RG4L = edxl;
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  i32 RG51l;
  i8* RG52p;
  i32 RG53l;
  i16 RG54w;
  c_wlreg _RG55;
  c_wlreg _RG56;
  c_wlreg _RG57;
  i16 RG58w;
  i16 RG59w;
  c_wlreg _RG61;
  c_wlreg _RG62;
  i32 RG71l;
  i32 RG72l;
  c_wlreg _RG73;
  i32 RG74l;
  i32 RG75l;
  c_wlreg _RG76;
  c_ol32 vol_00; // poke 32 peek 32 sub16
  c_ol32 vol_04; // poke 32 peek 32 sub16
  c_tim c38_08;
  i32 vl_14;
  unk* xp_18;
  i32 vl_1c;
  i32 vl_20;
  i32 vl_24;
  i32 vl_28;
  i32 vl_2c;
  i32 vl_30;
  i32 vl_34;
  c_o32 vo_38; // poke 16 peek 16 32 add16
  i16 vw_3c;
  i16 vw_40;
  i16 vw_44;
  i16 vw_48;
  i16 vw_4c;
  c_ql32 vql_50;
  c_o32 vo_54; // poke 16 peek 16 32 add16
  i16 parw01;
  i16 parw02;
  i16 parw08;
  i16 parw13;
  i16 parw14;

  vol_04.poke32(RG1L);
  vol_00.poke32(RG4L);
  DM2_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
  if (ddat.v1e03a8 != 0)
  {
    ddat.v1e03a8 = 0;
    DM2_PUSH_PULL_RIGID_BODY(signedlong(ddat.v1d324a));
    DM2_guidraw_29ee_000f();
    return;
  }
  vol_04.sub16(ddat.v1d26fc);
  vol_00.sub16(ddat.v1d26fe);
  vo_38.poke16(ddat.v1e0270);
  vo_54.poke16(ddat.v1e0272);
  RG1L = signedlong(ddat.v1e0258);
  RG4W = table1d27fc[RG1L];
  vo_38.add16(RG4W);
  RG1W = table1d2804[RG1L];
  vo_54.add16(RG1W);
  RG1L = (dm2_GET_TILE_VALUE(signedlong(vo_38.peek16()), signedlong(vo_54.peek16())) & lcon(0xff)) >> 5;
  RG1W = unsignedword(RG1Blo);
  ddat.v1e0530 = RG1W;
  vw_48 = ddat.savegamewpc.w_00;
  RG61L = 0;
  //m_12E9A:
  for (;;)
  {
    bool skip00215 = false;
    RG1L = unsignedlong(RG61W);
    RG4L = signedlong(ddat.v1e0404);
    if (RG1L < RG4L)
    {
      //m_12BD9:
      RG71l = signedlong(vol_00.peek16());
      RG51l = signedlong(vol_04.peek16());
      RG3L = 12 * RG1L;
      RG1R = &ddat.v1e02f0[RG1L].rc_00;
      RG2L = RG71l;
      RG4L = RG51l;
      RG1L = RG1R->pt_in_rect(RG4W, RG2W) ? 1 : 0;
      if (RG1L != 0)
      {
        RG1Blo = ddat.v1e02f0[RG3L / 0xc].b_0b - 1;
        if (mkub(RG1Blo) <= mkub(lcon(0x5)))
        {
          RG1L = unsignedlong(RG1Blo);
          switch (RG1L)
          {
            case 0:
            case 1:
            case 2:
              //m_12DD8:
              RG2L = unsignedlong(RG61W);
              RG1L = 12 * RG2L;
              if (ddat.v1e02f0[RG2L].w_08 != lcon(0xffffffff))
              {
                if (vw_48 == lcon(0xffffffff))
                {
                  DM2_events_121e_013a(signedlong(vol_04.peek16()), signedlong(vol_00.peek16()), RG2L);
                  return;
                }
                RG1Blo = ddat.v1e02f0[RG1L / 0xc].b_0b;
                RG1L = unsignedlong(RG1Blo);
                if (RG1L != lcon(0x3))
                {
                  //m_12E55:
                  if (RG1L == lcon(0x1))
                  {
                    RG59w = wcon(0x0);
                    //m_12E91:
                    for (;;)
                    {
                      RG1L = signedlong(RG59w);
                      if (RG1L > lcon(0x1))
                        break;
                      //m_12E5E:
                      i32 parl10 = RG1L;
                      RG1L = DM2_events_121e_03ae(signedlong(vol_04.peek16()), signedlong(vol_00.peek16()), signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), 0, parl10, lcon(0x1));
                      if (RG1L != 0)
                        return;
                      RG59w++;
                    }
                  }
                }
                else
                {
                  put16(parw08, RG1W);
                  RG1L = DM2_events_121e_03ae(signedlong(vol_04.peek16()), signedlong(vol_00.peek16()), signedlong(vo_38.peek16()), signedlong(vo_54.peek16()), 0, 2, parw08);
                  if (RG1L != 0)
                    return;
                }
              }
              break;

            case 3:
              //m_12C26:
              RG1L = signedlong(eventqueue.event_heroidx);
              if (RG1L != lcon(0xffffffff))
              {
                RG1L = unsignedlong(ddat.v1e02f0[RG3L / 0xc].b_0a);
                if (RG1L != lcon(0x3))
                {
                  //m_12C53:
                  vw_44 = ddat.v1e0270;
                  RG1W = ddat.v1e0272;
                }
                else
                {
                  RG1W = vo_38.peek16();
                  vw_44 = RG1W;
                  RG1W = vo_54.peek16();
                }
                //m_12C64:
                vw_3c = RG1W;
                RG72l = signedlong(vw_3c);
                RG1L = signedlong(vw_44);
                vl_2c = RG1L;
                RG4L = RG72l;
                RG1RP = DM2_GET_ADDRESS_OF_TILE_RECORD(RG1W, RG4W);
                RG52p = RG1P;
                if (jz_test8(location(RG1P + 2), lcon(0x40)))
                {
                  //m_12D39:
                  RG1L = signedlong(ddat.v1e0258) + 1 & lcon(0x3);
                  RG4W = table1d27fc[RG1L];
                  RG3W = vw_44 + RG4W;
                  vw_44 = RG3W;
                  RG1W = table1d2804[RG1L];
                  RG58w = vw_3c + RG1W;
                  vw_3c = RG58w;
                  RG1L = unsignedlong(ddat.savegamewpc.w_00);
                  i32 parl05 = RG1L;
                  RG1W = ddat.v1e0258;
                  RG1L += 3;
                  RG1W &= 0x3;
                  RG2L = unsignedlong(RG1W);
                  RG4L = unsignedlong(RG58w);
                  RG1L = unsignedlong(RG3W);
                  DM2_events_3C1E5(RG1L, RG4L, RG2L, lcon(0xffff), parl05);
                }
                else
                {
                  if (vw_48 == lcon(0xffffffff))
                  {
                    put16(parw02, ddat.v1e0272);
                    put16(parw01, ddat.v1e0270);
                    DM2_QUEUE_NOISE_GEN1(lcon(0x3), 0, lcon(0x88), lcon(0x8c), lcon(0x80), parw01, parw02, lcon(0x1));
                    RG1L = timdat.gametick + 1;
                    i32 parl04 = RG1L;
                    DM2_INVOKE_MESSAGE(unsignedlong(vw_44), unsignedlong(vw_3c), 0, 2, parl04);
                    c38_08.setmticks(ddat.v1d3248, timdat.gametick + 1);
                    c38_08.settype(0x58);
                    RG2W = unsignedword(RG2Blo);
                    c38_08.setactor(0);
                    RG4L = RG72l;
                    RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(CUTX16(vl_2c), RG4W));
                    c38_08.setA(RG1W);
                    DM2_QUEUE_TIMER(&c38_08);
                    or8(location(RG52p + lcon(0x3)), lcon(0x8));
                    ddat.glbRefreshViewport = true;
                  }
                }
              }
            case 4:
              break;

            case 5:
              //m_12DA1:
              RG1L = unsignedlong(ddat.v1e02f0[RG3L / 0xc].b_0a);
              vw_4c = RG1W;
              RG1L = DM2_events_32cb_0287(unsignedlong(ddat.v1e02f0[RG3L / 0xc].b_0a), RG51l, RG71l);
              if (RG1L != 0)
              {
                DM2_events_121e_0003(signedlong(vw_4c));
                return;
              }
              break;

            default: throw(THROW_DMABORT);
          }
        }
      }
      //m_12E99:
      RG61L++;
      continue;
    }
    RG1L = dm2_GET_TILE_VALUE(signedlong(vo_38.peek16()), signedlong(vo_54.peek16()));
    RG61W = unsignedword(RG1Blo);
    RG1L = unsignedlong(RG1Blo) >> 5;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != lcon(0x6))
      skip00215 = true;
    else
    {
      RG1L = RG61L;
      RG1W &= 0x1;
      RG1L = unsignedlong(RG1W);
      if (RG1L == 0)
        skip00215 = true;
      else
      {
        RG61L &= 0x4;
        RG1L = unsignedlong(RG61W);
        if (RG1L != 0)
          skip00215 = true;
        else
          RG1L = 1;
      }
    }

    if (skip00215)
      //m_12EF9:
      RG1L = 0;

    //m_12EFB:
    vl_24 = RG1L;
    if (vw_48 != lcon(0xffffffff))
    {
      //m_13059:
      RG61L = 0;
      //m_130CE:
      for (;;)
      {
        RG1L = unsignedlong(RG61W);
        vl_14 = RG1L;
        if (RG1L > lcon(0x1))
        {
          if (ddat.v1e0530 == 0)
          {
            if (vl_24 == 0)
              return;
          }
          RG61L = 2;
          break;
        }
        //m_1305D:
        RG1L = signedlong(RG61W);
        i32 parl19 = RG1L;
        RG3L = signedlong(ddat.v1e0272);
        RG2L = signedlong(ddat.v1e0270);
        RG57L = signedlong(vol_00.peek16());
        RG73L = signedlong(vol_04.peek16());
        RG1L = DM2_events_121e_03ae(RG73L, RG57L, RG2L, RG3L, 0, parl19, lcon(0x1));
        if (RG1L != 0)
          return;
        RG1L = DM2_PT_IN_EXPANDED_RECT(RG61W + lcon(0x2f8), RG73W, RG57W) ? 1 : 0;
        if (RG1L != 0)
        {
          RG1L = DM2_eventa_121e_0222(signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), vl_14);
          if (RG1L != 0)
            return;
        }
        RG61L++;
      }
      //m_1316A:
      for (;;)
      {
        RG1L = unsignedlong(RG61W);
        vl_20 = RG1L;
        if (RG1L > lcon(0x3))
        {
          RG1L = DM2_GET_CREATURE_AT(unsignedlong(vo_38.peek16()), unsignedlong(vo_54.peek16()));
          vql_50.poke32(RG1L);
          if (RG1W != lcon(0xffffffff))
          {
            RG1P = DM2_QUERY_CREATURE_AI_SPEC_FROM_TYPE(unsignedlong(RG1W));
            xp_18 = UPCAST(unk, RG1P);
            if (jnz_test8(location(RG1P), lcon(0x1)))
            {
              RG61L = 0;
              //m_13231:
              for (;;)
              {
                RG1P = DOWNCAST(unk, xp_18);
                RG1W = word_at(RG1P, lcon(0x1e)) & lcon(0xf);
                if (RG61UW >= RG1UW)
                  break;
                //m_131B7:
                RG1L = signedlong(CUTX16(RG61L + 2));
                i32 parl25 = RG1L;
                vl_30 = signedlong(vo_54.peek16());
                RG74l = signedlong(vo_38.peek16());
                RG56L = signedlong(vol_00.peek16());
                RG1L = signedlong(vol_04.peek16());
                vl_34 = RG1L;
                RG1L = DM2_events_121e_03ae(RG1L, RG56L, RG74l, vl_30, lcon(0x3), parl25, 2);
                if (RG1L != 0)
                  return;
                RG1W = vql_50.peek16(); // TODO: gets extended w/o conversion now
                RG1L = DM2_PT_IN_EXPANDED_RECT(CUTX16(DM2_query_0cee_2e09(RG1L) + RG61L), CUTX16(vl_34), RG56W) ? 1 : 0;
                if (RG1L != 0)
                {
                  RG1L = RG61L + lcon(0x6);
                  RG2L = unsignedlong(RG1W);
                  RG1L = DM2_eventa_121e_0222(RG74l, vl_30, RG2L);
                  if (RG1L != 0)
                    return;
                }
                RG61L++;
              }
            }
          }
          break;
        }
        //m_130F8:
        RG1L = signedlong(RG61W);
        i32 parl22 = RG1L;
        vl_28 = signedlong(vo_54.peek16());
        RG75l = signedlong(vo_38.peek16());
        RG1L = signedlong(vol_00.peek16());
        vl_1c = RG1L;
        RG55L = signedlong(vol_04.peek16());
        RG1L = DM2_events_121e_03ae(RG55L, RG1L, RG75l, vl_28, lcon(0x3), parl22, lcon(0x1));
        if (RG1L != 0)
          return;
        RG1L = DM2_PT_IN_EXPANDED_RECT(RG61W + lcon(0x2f8), RG55W, CUTX16(vl_1c)) ? 1 : 0;
        if (RG1L != 0)
        {
          RG1L = DM2_eventa_121e_0222(RG75l, vl_28, vl_20);
          if (RG1L != 0)
            return;
        }
        RG61L++;
      }
      //m_13246:
      DM2_events_121e_0351(signedlong(vol_04.peek16()), signedlong(vol_00.peek16()));
      return;
    }
    if (ddat.v1e0530 == 0)
    {
      bool skip00217 = false;
      RG54w = vol_00.peek16();
      RG3L = signedlong(vol_04.peek16());
      RG1L = DM2_PT_IN_EXPANDED_RECT(lcon(0x2fd), RG3W, RG54w) ? 1 : 0;
      if (RG1L != 0)
        skip00217 = true;
      else
      {
        RG1L = DM2_PT_IN_EXPANDED_RECT(lcon(0x2fe), RG3W, RG54w) ? 1 : 0;
        if (RG1L != 0)
          skip00217 = true;
      }

      if (skip00217)
      {
        //m_12F47:
        DM2_PLAYER_TESTING_WALL(vl_24, signedlong(vo_38.peek16()), signedlong(vo_54.peek16()));
        return;
      }
    }
    //m_12F63:
    RG4L = unsignedlong(vo_54.peek16());
    RG1L = DM2_GET_CREATURE_AT(unsignedlong(vo_38.peek16()), RG4L);
    if (RG1W == lcon(0xffffffff))
      return;
    RG53l = unsignedlong(RG1W);
    RG1P = DM2_QUERY_CREATURE_AI_SPEC_FROM_TYPE(RG53l);
    if (jz_test8(location(RG1P), lcon(0x1)))
      return;
    RG62L = signedlong(vo_54.peek16());
    RG76L = signedlong(vo_38.peek16());
    RG1L = DM2_guivp_32cb_01b6(signedlong(vol_04.peek16()), signedlong(vol_00.peek16()), RG76L, RG62L, &vw_40) ? 1 : 0;
    if (RG1L == 0)
      return;
    RG1L = DM2_GET_CREATURE_WEIGHT(RG53l) & lcon(0xffff);
    if (RG1L != lcon(0xff))
    {
      if (vw_40 != 0)
      {
        DM2_ROTATE_CREATURE(RG53l, 0, signedlong(vw_40));
        return;
      }
      ddat.v1e03a8 = 1;
      RG1L = vol_04.peek32();
      ddat.v1e03fa = RG1W;
      RG1L = vol_00.peek32();
      ddat.v1e03fc = RG1W;
      DM2_guidraw_29ee_000f();
      ddat.v1d324a = 6;
      return;
    }
    put16(parw14, RG62W);
    put16(parw13, RG76W);
    RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG53l);
    RG4L = unsignedlong(RG1Blo);
    DM2_QUEUE_NOISE_GEN2(lcon(0xf), RG4Blo, lcon(0x84), lcon(0xfe), parw13, parw14, 0, lcon(0x8c), lcon(0x80));
    return;
  }
  FOREND
}

// belongs to DM2_HANDLE_UI_EVENT
// was SKW_24a5_174d
void DM2_CLICK_INVENTORY_EYE(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;

  DM2_MOUSE_SET_CAPTURE();
  ddat.vcapture3 = true;
  DM2_GET_MOUSE_ENTRY_DATA(OO ddat.v1e00b6, OO ddat.v1e00b4, OO ddat.v1e00b2);
  RG1W = ddat.v1e00b2 & 2;
  RG1L = signedlong(RG1W);
  if (RG1L != 0)
  {
    eventqueue.event_1031_098e();
    DM2_HIDE_MOUSE();
    DM2_SLEEP_SEVERAL_TIME(8);
  }
  else
  {
    DM2_MOUSE_RELEASE_CAPTURE();
    ddat.vcapture3 = false;
  }
}

// belongs to DM2_HANDLE_UI_EVENT
void DM2_ADD_RUNE_TO_TAIL(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  i32 RG51l;
  i32 RG52l;
  c_lreg _RG61;
  c_wlreg _RG62;
  i32 RG7l;
  bool flag;

  RG3L = RG1L;
  c_hero* hero = &party.hero[party.curacthero - 1];
  RG2W = unsignedword(RG2Blo);
  RG2Blo = hero->nrunes;
  RG51l = 6 * unsignedlong(RG2Blo); // TODO where is that needed?
  RG1L = signedlong(RG3W);
  RG1Blo = table1d67e0[unsignedlong(RG2Blo)][RG1L];
  RG1W = unsignedword(RG1Blo);
  if (RG2W != 0)
  {
    RG52l = unsignedlong((i8)table1d6797[unsignedlong(hero->rune[0]) + 1]); // EVIL offset was 0x1d6798
    RG1L = unsignedlong(RG1W) * RG52l;
    RG1L >>= bcon(0x3);
  }
  flag = false;
  RG7l = signedlong(hero->curMP);
  RG61L = unsignedlong(RG1W);
  if (RG61L <= RG7l)
  {
    hero->curMP -= RG1W;
    hero->heroflag |= 0x800;
    RG1Blo = RG2Blo;
    RG1Bhi = 6;
    RG1W = unsignedword(RG1Blo) * unsignedword(RG1Bhi);
    RG1Blo += lcon(0x60);
    RG3Blo += RG1Blo;
    RG1L = unsignedlong(RG2W);
    hero->rune[RG1L] = RG3Blo;
    RG2L++;
    hero->nrunes = RG2Blo;
    RG1L = unsignedlong(RG2Blo);
    hero->rune[RG1L] = 0;
    flag = true;
  }
  RG62L = 1;
  ddat.v1e0b6c = RG62W;
  DM2_UPDATE_RIGHT_PANEL(0);
  RG1L = DM2_107B0();
  if (flag)
  {
    RG1W = party.curacthero;
    RG1L -= RG62L;
    RG1L = unsignedlong(RG1W);
    DM2_REFRESH_PLAYER_STAT_DISP(RG1W);
  }
}

// belongs to DM2_HANDLE_UI_EVENT
void DM2_REMOVE_RUNE_FROM_TAIL(void)
{
  c_hero* hero = &party.hero[party.curacthero - 1];
  hero->rune[--hero->nrunes] = 0;
  ddat.v1e0b6c = 1;
  DM2_UPDATE_RIGHT_PANEL(0);
  DM2_107B0();
}

// belongs to DM2_HANDLE_UI_EVENT
void DM2_CLICK_MONEYBOX(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  bool skip00733 = false;

  RG4L = RG1L;
  RG1L = signedlong(ddat.v1d67bc);
  if (RG1L != lcon(0x4))
    return;
  RG3W = party.curacthero;
  RG3L--;
  RG2L = signedlong(RG3W);
  RG1W = party.hero[RG2L].item[party.curactmode];
  RG2W = ddat.savegamewpc.w_00;
  if (RG2W == lcon(0xffffffff))
  {
    //m_32B4C:
    RG4L = signedlong(RG4W);
    RG2L = unsignedlong(RG1W);
    RG1L = DM2_GET_ITEM_ORDER_IN_CONTAINER(RG2L, RG4L);
    RG4L = signedlong(RG1W);
    RG1L = DM2_TAKE_COIN_FROM_WALLET(RG2L, RG4L);
    if (RG1W == lcon(0xffffffff))
      skip00733 = true;
    else
    {
      RG1L = unsignedlong(RG1W);
      DM2_TAKE_OBJECT(RG1L, 0);
    }
  }
  else
  {
    RG4L = unsignedlong(RG2W);
    RG1L = DM2_ADD_COIN_TO_WALLET(unsignedlong(RG1W), RG4L);
    if (RG1L == 0)
      skip00733 = true;
    else
      RG1L = DM2_REMOVE_OBJECT_FROM_HAND();
  }

  if (!skip00733)
  {
    //m_32B77:
    ddat.v1e0b6c = 1;
    DM2_UPDATE_RIGHT_PANEL(0);
  }

  //m_32B87:
  party.calc_player_weight((e_hero)RG3W);
}

// stacksize was 0xa0
void DM2_events_2f3f_04ea(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG51w;
  i16 RG52w;
  c_lreg _RG6;
  c_ol32 vol_00; // add16
  c_ol32 vol_04; // add16
  t_text tarr_08[0x80];
  i16 vw_88;
  c_ql32 vql_8c;

  vol_04.poke32(RG1L);
  vol_00.poke32(RG4L);
  RG51w = RG2W;
  RG1W = ddat.v1e0288;
  RG1L--;
  vql_8c.poke32(RG1L);
  RG2L = unsignedlong(RG1W);
  c_hero* hero = &party.hero[RG2L];
  vw_88 = ddat.v1d3248;
  RG1L = signedlong(RG3W);
  DM2_CHANGE_CURRENT_MAP_TO(RG1L);
  RG4L = DM2_REMOVE_OBJECT_FROM_HAND();
  if (argw0 != lcon(0x93))
  {
    //m_39BF2:
    if (ddat.v1d6a2d == 0)
      DM2_DRAW_CRYOCELL_LEVER(lcon(0x1));
    if (RG4W != lcon(0xffffffff))
    {
      RG4L = unsignedlong(RG4W);
      DM2_ADD_ITEM_TO_PLAYER((e_hero)vql_8c.peek16(), RG4W);
    }
    //m_39C20:
    RG3L = 0;
    ddat.v1e0288 = RG3W;
    RG1L = signedlong(RG51w);
    RG4W = table1d27fc[RG1L];
    vol_04.add16(RG4W);
    RG1W = table1d2804[RG1L];
    vol_00.add16(RG1W);
    RG52w = wcon(0x0);
    //m_39C76:
    for (;;)
    {
      RG1L = unsignedlong(RG52w);
      if (RG1L >= lcon(0x1e))
      {
        RG4L = signedlong(vol_00.peek16());
        RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(vol_04.peek16(), RG4W));
        break;
      }
      //m_39C49:
      RG2W = hero->item[RG1L];
      if (RG2W != lcon(0xffffffff))
      {
        RG3L = signedlong(vol_00.peek16());
        RG1L = signedlong(vol_04.peek16());
        RG6L = unsignedlong(RG2W);
        RG2L = RG1L;
        RG4P = NULL;
        RG1L = RG6L;
        DM2_CUT_RECORD_FROM(mk_record(RG1L), UPCAST(t_record, RG4P), RG2W, RG3W);
      }
      //m_39C75:
      RG52w++;
    }
    //m_39C93:
    for (;;)
    {
      RG2L = RG1L;
      RG1L = RG2L;
      RG1Blo ^= RG2Blo;
      RG1Bhi &= lcon(0x3c);
      RG1L = unsignedlong(RG1W) >> bcon(0xa);
      if (RG1L == lcon(0x3))
      {
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
        RG3W = word_at(RG1P, 2) & lcon(0x7f);
        if (RG3W == lcon(0x7e))
        {
          //m_39CCE:
          and8(location(RG1P + lcon(0x4)), lcon(0xfffffffb));
          RG1L = unsignedlong(party.heros_in_party);
          if (RG1L == lcon(0x1))
            ddat.v1e01a0 = timdat.gametick;
          if (ddat.v1d6a2d == 0)
          {
            RG1L = vql_8c.peek32();
            ddat.v1e098c = RG1W;
            DM2_DISPLAY_HINT_NEW_LINE();
            RG3P = ADRESSOF(t_text, tarr_08);
            RG2L = lcon(0xe);
            RG4L = 0;
            RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x1), RG4Blo, RG2Blo, RG3T);
            RG2L = unsignedlong(vql_8c.peek16());
            RG2Blo = table1d69d0[RG2L];
            RG2L = unsignedlong(RG2Blo);
            DM2_DISPLAY_HINT_TEXT(RG2L, RG1T);
            if (ddat.v1e0976 != 0)
            {
              DM2_guidraw_24a5_1798(4);
              DM2_events_38c8_0060();
            }
          }
          break;
        }
      }
      //m_39CC2:
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG2UW)));
    }
  }
  else
  {
    DM2_guidraw_24a5_1798(4);
    DM2_HIDE_MOUSE();
    RG1L = unsignedlong(party.heros_in_party);
    if (RG1L == lcon(0x1))
      DM2_SELECT_CHAMPION_LEADER(lcon(0xffffffff));
    RG1W = ddat.v1e0288;
    RG4W = party.heros_in_party;
    if (RG1UW >= RG4UW)
    {
      RG2L = RG4L - 1;
      party.heros_in_party = RG2W;
    }
    RG3L = 0;
    ddat.v1e0288 = RG3W;
    hero->curHP = RG3W;
    RG2L = unsignedlong(vql_8c.peek16());
    RG1L = RG2L;
    ddat.v1e0b1c[RG2L] = 0;
    RG4Blo = lcon(0xffffffff);
    party.handitems.barr_00[RG2L] = RG4Blo;
    v1d67be[RG2L] = RG4Blo;
    RG2L *= 4;
    RG2L -= RG1L;
    RG3Blo = 0;
    ddat.v1e0b34[RG2L / 3][1] = RG3Blo;
    party.handitems.warr2d_00[RG1L + 1][1] = -1;
    ddat.v1e0b34[RG2L / 3][0] = RG3Blo;
    party.handitems.warr2d_00[RG1L + 1][0] = -1;
    ddat.v1e0b34[RG2L / 3][2] = RG3Blo;
    RG4L = unsignedlong(palettecolor_to_ui8(paldat.palette[E_COL00]));
    RG1L = signedlong(CUTX16(vql_8c.peek32() + lcon(0xa1)));
    DM2_FILL_SCREEN_RECT(/*TAG QUERY*/ RG1W, /*TAG PIXEL256*/ ui8_to_pixel(RG4UBlo));
    DM2_events_38c8_0060();
    DM2_SHOW_MOUSE();
  }
  //m_39D51:
  DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_88));
}

void DM2_events_2e62_0cfa(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG5l;

  RG5l = RG1L;
  RG4L = 0;
  //m_39125:
  for (;;)
  {
    if (RG4UW >= mkuw(party.heros_in_party))
    {
      RG3W = ddat.v1e0976;
      if (RG3W == 0)
      {
        DM2_UPDATE_RIGHT_PANEL(RG5l);
        return;
      }
      RG4L = RG3L - 1;
      RG2L = unsignedlong(RG4W);
      c_hero* hero = &party.hero[RG2L];
      if (hero->ench_power != 0)
      {
        RG2L = unsignedlong(hero->ench_aura);
        if (RG2L >= 3 && RG2L <= 6)
        {
          if (!ddat.vcapture3 || ddat.savegamewpc.w_00 != lcon(0xffffffff))
            //m_39190:
            RG3L = lcon(0x1000);
          else
            RG3L = lcon(0x3000);
          //m_39195:
          RG2L = unsignedlong(hero->heroflag) | mkul(RG3L);
          hero->heroflag = RG2W;
        }
      }
      break;
    }
    //m_3910A:
    RG1L = unsignedlong(RG4W);
    RG3L = signedlong(ddat.v1e0976);
    RG2L = RG1L + lcon(0x1);
    if (RG2L != RG3L)
      DM2_REFRESH_PLAYER_STAT_DISP(RG1W);
    RG4L++;
  }
  //m_3919F:
  RG1W = ddat.v1e0976;
  RG1L--;
  RG1L = unsignedlong(RG1W);
  DM2_REFRESH_PLAYER_STAT_DISP(RG1W);
  DM2_UPDATE_RIGHT_PANEL(RG5l);
}

// belongs to DM2_TRY_CAST_SPELL
static unk* DM2_FIND_SPELL_BY_RUNES(t_text* eaxp)
{
  c_nreg _RG1; RG1T = eaxp;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG5l;

  if (eaxp[1] == 0)
    return NULL;
  RG4L = lcon(0x18);
  RG2L = 0;
  //m_322BF:
  for (;;)
  {
    RG5l = unsignedlong(byte_at(RG1P));
    RG3L = signedlong(RG4W);
    RG5l <<= RG3Blo;
    RG2L |= RG5l;
    RG1P++;
    if (byte_at(RG1P) != 0)
    {
      RG4L -= lcon(0x8);
      if (RG4W >= 0)
        continue;
    }
    break;
  }
  //m_322D7:
  RG1P = table1d6802;
  RG4L = lcon(0x22);
  //m_322E1:
  for (;;)
  {
    RG4L--;
    if (RG4W == lcon(0xffffffff))
      return NULL;
    if (jz_test8(location(RG1P + lcon(0x3)), lcon(0xffffffff)))
    {
      //m_322F7:
      RG3L = RG2L & lcon(0xffffff);
      if (RG3L == long_at(RG1P))
        return RG1P;
    }
    else
    {
      if (RG2L == long_at(RG1P))
        return RG1P;
    }
    //m_32303:
    RG1P += lcon(0x8);
  }
  FOREND
}

// belongs to DM2_TRY_CAST_SPELL
// was SKW_2759_222e (new)
static i32 DM2_CAST_SPELL_PLAYER(i32 eaxl, i32 ebxl, unk* xedxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4P = xedxp;
  c_wlreg _RG51;
  c_wlreg _RG52;
  i16 RG53w;
  i32 RG54l;
  c_wlreg _RG55;
  c_wlreg _RG56;
  i8* RG61p;
  c_wlreg _RG72;
  c_lreg _RG73;
  c_ql32 vql_00;
  c_tim c38_04;
  c_ql32 vql_10;
  i32 vl_14;
  i16 vw_18;
  i16 vw_1c;
  c_ql32 vql_20;
  i16 vw_24;
  i32 parl00;
  i16 parw01;
  i8 parb03;
  i16 parw05;
  bool skip00730 = false;
  bool skip00728 = false;
  bool skip00727 = false;
  bool skip00726 = false;

  vql_00.poke32(RG1L);
  RG61p = RG4P;
  vw_24 = RG2W;
  RG4L = unsignedlong(RG1W);
  c_hero* hero = &party.hero[RG4L];
  RG4L = unsignedlong(word_at(RG61p, lcon(0x6))) >> bcon(0xa) & lcon(0x3f);
  RG1L = signedlong(vw_24) + lcon(0x12);
  RG4L *= RG1L;
  RG2L = lcon(0x18);
  RG1L = RG4L;
  RG4L = RG1L % RG2L;
  RG1L /= RG2L;
  vw_18 = RG1W; // truncated
  RG1W = unsignedword(RG1Blo);
  RG1Blo = byte_at(RG61p, lcon(0x4));
  RG52L = unsignedlong(vw_24) + RG1L;
  RG4L = unsignedlong(vw_24) - 1;
  RG2L = RG1L * RG4L * 8;
  RG1L = DM2_RAND();
  RG4L = RG1L;
  RG4Bhi ^= RG1Bhi;
  RG4Blo &= lcon(0x7);
  RG1L = 16 * RG52L + RG4L;
  RG2L += RG1L;
  RG1L = RG52L * RG52L;
  RG2L += RG1L;
  vw_1c = RG2W; // truncated
  RG4L = unsignedlong(byte_at(RG61p, lcon(0x5)));
  RG1L = signedlong(vql_00.peek16());
  RG2L = 1;
  RG1W = DM2_QUERY_PLAYER_SKILL_LV(RG1W, RG4W, RG2L);
  vql_20.poke16(RG1W);
  RG3W = RG52W - RG1W;
  //m_3244E:
  for (;;)
  {
    RG1L = RG3L;
    RG3L--;
    if (RG1W <= 0)
      break;
    RG1L = DM2_RAND();
    RG1W &= lcon(0x7f);
    RG1L = unsignedlong(RG1W);
    vql_10.poke32(RG1L);
    RG2L = 0;
    RG4L = 3;
    RG1W = DM2_MIN(hero->get_adj_ability1((e_ability)RG4W, (e_curmax)RG2W) + 15, 115);
    if (RG1W < vql_10.peek16())
    {
      RG1L = unsignedlong(RG52W) - unsignedlong(vql_20.peek16());
      RG3L = RG1L;
      RG2L = unsignedlong(vw_1c) >> RG3Blo;
      RG1W = unsignedword(RG1Blo);
      RG1Blo = byte_at(RG61p, lcon(0x5));
      RG4L = unsignedlong(RG1W);
      DM2_ADJUST_SKILLS(vql_00.peek16(), RG4L, RG2L);
      RG3L = lcon(0x10);
      jump L_fin2;
    }
  }
  //m_324C5:
  RG1W = word_at(RG61p, lcon(0x6)) & lcon(0xf);
  RG1L--;
  if (RG1UW > mkuw(lcon(0x3)))
    jump L_fin1;
  RG1L = unsignedlong(RG1W);
  RG4L = 2 * vql_20.peek32();
  switch (RG1L)
  {
    case 0:
      //m_324EB:
      RG1L = DM2_FIND_HAND_WITH_EMPTY_FLASK(hero);
      if (RG1W < 0)
      {
        RG3L = lcon(0x30);
        jump L_fin2;
      }
      //m_32501:
      RG2L = unsignedlong(RG1W);
      RG3L = unsignedlong(vql_00.peek16());
      RG4L = RG2L;
      RG1L = DM2_REMOVE_POSSESSION(RG3L, RG4L);
      RG4L = unsignedlong(word_at(RG61p, lcon(0x6))) >> 4 & lcon(0x3f);
      vql_10.poke32(RG4L);
      RG4Blo = vql_10.peek8();
      RG4L = unsignedlong(RG4Blo);
      RG72L = unsignedlong(RG1W);
      DM2_SET_ITEMTYPE(RG72L, RG4L);
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG72UW));
      RG4P = RG1P;
      RG1L = DM2_RAND();
      RG1W &= lcon(0xf);
      vl_14 = RG1L;
      RG1L = unsignedlong(vw_24) * lcon(0x28) + vl_14;
      RG1W = unsignedword(RG1Blo);
      mov8(location(RG4P + 2), RG1Bhi);
      or16(location(RG4P + 2), RG1W);
      DM2_EQUIP_ITEM_TO_HAND(RG3L, RG72L, RG2L);
      jump L_fin1;

    case 1:
      //m_3256F:
      RG1L = unsignedlong(word_at(RG61p, lcon(0x6))) >> 4 & lcon(0x3f);
      if (RG1L == lcon(0x4))
        vql_20.poke32(RG4L);
      RG1L = 2 * vql_20.peek32() + lcon(0x4);
      RG4L = unsignedlong(vw_24) + 2;
      RG1L *= RG4L;
      RG4L = signedlong(RG1W);
      RG2L = lcon(0xff);
      RG1L = signedlong(DM2_BETWEEN_VALUE(21, RG2W, RG4W));
      RG2L = signedlong(RG1W);
      RG1L = (unsignedlong(word_at(RG61p, lcon(0x6))) >> 4 & lcon(0x3f)) + lcon(0xff80);
      RG4L = signedlong(RG1W);
      DM2_CAST_CHAMPION_MISSILE_SPELL(unsignedlong(vql_00.peek16()), RG4L, RG2L, 0);
      jump L_fin1;

    case 2:
      break;

    case 3:
      //m_325D4:
      RG1L = unsignedlong(word_at(RG61p, lcon(0x6))) >> 4 & lcon(0x3f);
      if (RG1L == lcon(0x35))
      {
        RG1RP = recordptr[4];
        RG53w = word_at(DOWNCAST(s_sgwords, ddat.savegamep4), lcon(0x14));
        //m_32639:
        for (;;)
        {
          RG53w--;
          if (RG53w == lcon(0xffffffff))
            break;
          //m_325F6:
          if (word_at(RG1P) != lcon(0xffffffff) && byte_at(RG1P + lcon(0x4)) == lcon(0x35))
          {
            RG3L = unsignedlong(byte_at(RG1P, lcon(0x5)));
            if (RG3W != lcon(0xff))
            {
              RG2L = signedlong(RG3W);
              RG4L = 16 * RG2L;
              RG2L = 34 * RG2L;
              RG4P = DOWNCAST(c_creature, creatures) + RG2L;
              RG2L = signedlong(byte_at(RG4P, lcon(0x1a)));
              if (RG2L != lcon(0x13))
              {
                mov8(location(RG4P + lcon(0x17)), lcon(0x13));
                jump L_fin1;
              }
            }
          }
          //m_32636:
          RG1P += lcon(0x10);
        }
      }
      //m_32640:
      RG1L = signedlong(CUTX8(ddat.v1e0258));
      put8(parb03, RG1Blo);
      RG1L = signedlong(ddat.v1e0266);
      put16(parw01, RG1W);
      RG1L = signedlong(ddat.v1e0272);
      parl00 = RG1L;
      RG3L = signedlong(ddat.v1e0270);
      RG1W = ddat.v1e0258;
      RG1L += 2;
      RG1W &= 0x3;
      RG2L = unsignedlong(RG1W);
      RG1L = unsignedlong(vql_20.peek16());
      RG4L = 2 * RG1L;
      RG1L = signedlong(DM2_RANDDIR()) & lcon(0xffff);
      RG4L += RG1L;
      RG4L *= signedlong(vw_24);
      RG1L = RG4L;
      RG4L = RG1L % lcon(0x6);
      RG1L /= 6;
      RG4L = unsignedlong(RG1W);
      RG1W = DM2_CREATE_MINION(unsignedlong(word_at(RG61p, lcon(0x6))) >> 4 & ulcon(0x3f) & ulcon(0xffff), RG4L, RG2L, RG3L, parl00, parw01, lcon(0xffff), parb03);
      if (RG1W != lcon(0xffffffff))
        jump L_fin1;
      RG3L = unsignedlong(ddat.v1e0272);
      RG2L = unsignedlong(ddat.v1e0270);
      RG4L = lcon(0x6e);
      RG1L = lcon(0xffa8);
      DM2_CREATE_CLOUD(RG1L, RG4L, RG2L, RG3L, lcon(0xff));
      jump L_fin1;

    default: throw(THROW_DMABORT);
  }
  //m_326F9:
  RG1W = unsignedword(RG1Blo);
  c38_04.setactor(0);
  RG51L = (unsignedlong(vw_24) + 1) * 4;
  RG1L = unsignedlong(word_at(RG61p, lcon(0x6))) >> 4 & lcon(0x3f);
  if (mkul(RG1L) > mkul(lcon(0xf)))
    jump L_fin1;
  switch (RG1L)
  {
    case 0:
      //m_32724:
      RG3L = lcon(0x27);
      skip00726 = true;
      break;

    case 1:
      //m_3272B:
      RG3L = 6;
      skip00726 = true;
      break;

    case 2:
      //m_327B8:
      RG3L = 1;
      skip00727 = true;
      break;

    case 3:
      //m_3274A:
      c38_04.settype(0x47);
      RG1Blo = ddat.savegames1.b_02;
      RG1Bhi = RG1Blo + 1;
      ddat.savegames1.b_02 = RG1Bhi;
      if (RG1Blo == 0)
      {
        if (ddat.v1e0976 != 0)
        {
          RG4L = signedlong(ddat.v1e0976) - 1;
          party.hero[RG4L].heroflag |= 0x4000;
        }
      }
      RG51L *= 8;
      c38_04.setmticks(ddat.v1e0266, timdat.gametick + unsignedlong(RG51W));
      DM2_QUEUE_TIMER(&c38_04);
      jump L_fin1;

    case 4:
      //m_327C9:
      RG3L = 2;
      skip00730 = true;
      break;

    case 5:
      //m_32732:
      RG3L = lcon(0x26);
      skip00726 = true;
      break;

    case 6:
      //m_327D0:
      RG3L = 5;
      skip00728 = true;
      break;

    case 7:
      //m_327F2:
      RG3L = 4;
      skip00728 = true;
      break;

    case 8:
      //m_327C5:
      RG3L = 0;
      skip00727 = true;
      break;

    case 9:
      //m_327F9:
      RG3L = 6;
      skip00728 = true;
      break;

    case 10:
      //m_32800:
      RG3L = 3;
      skip00728 = true;
      break;

    case 11:
      //m_32807:
      RG4L = unsignedlong(ddat.savegames1.b_04);
      RG1L = signedlong(DM2_MIN(CUTX16(8 * RG51L + RG4L), wcon(0xff)));
      ddat.savegames1.b_04 = RG1Blo;
    case 12:
    case 13:
      jump L_fin1;

    case 14:
      //m_3282B:
      RG3L = unsignedlong(ddat.v1e0272);
      RG54l = unsignedlong(ddat.v1e0270);
      RG1L = RG4L + lcon(0x4);
      RG4L = unsignedlong(vw_24) + 2;
      RG1L *= RG4L;
      RG4L = signedlong(RG1W);
      RG2L = lcon(0xff);
      RG1L = signedlong(DM2_BETWEEN_VALUE(21, RG2W, RG4W));
      RG4L = unsignedlong(RG1W);
      RG2L = RG54l;
      RG1L = lcon(0xff8e);
      DM2_CREATE_CLOUD(RG1L, RG4L, RG2L, RG3L, lcon(0xff));
      jump L_fin1;

    case 15:
      //m_32876:
      RG3L = lcon(0x42);
      RG1L = DM2_ALLOC_NEW_DBITEM(mkl(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0xd), lcon(0xf), lcon(0xb), RG3Blo) & lcon(0xffff)));
      RG4L = RG1L;
      if (ddat.savegamewpc.w_00 == lcon(0xffffffff))
      {
        RG1L = unsignedlong(RG4W);
        DM2_TAKE_OBJECT(RG1L, lcon(0x1));
        jump L_fin1;
      }
      //m_328B6:
      RG1L = DM2_RANDBIT() ? 1 : 0;
      RG3W = ddat.v1e0258;
      RG55L = RG1L + RG3L & lcon(0x3);
      RG1L = signedlong(ddat.v1e0272);
      put16(parw05, RG1W);
      RG3L = signedlong(ddat.v1e0270);
      RG4Bhi &= lcon(0x3f);
      RG55L <<= bcon(0xe);
      RG55L |= RG4L;
      DM2_MOVE_RECORD_TO(unsignedlong(RG55W), lcon(0xffffffff), 0, RG3L, parw05);
      jump L_fin1;

    default: throw(THROW_DMABORT);
  }

  if (skip00728)
  {
    //m_327D5:
    RG51L += 3;
    skip00730 = true;
  }

  if (skip00727)
  {
    //m_327BD:
    RG51L *= RG51L;
    RG51L += lcon(0x64);
  }

  if (skip00726)
  {
    //m_32737:
    RG56L = RG51L * lcon(0x9);
    RG4L = signedlong(RG56W);
    RG1L = signedlong(RG3W);
    DM2_PROCEED_LIGHT(RG1L, RG4L);
    jump L_fin1;
  }

  if (skip00730)
    //m_327D8:
    RG51L *= RG51L;

  //m_327DB:
  RG2L = unsignedlong(RG51W);
  RG4L = unsignedlong(RG3Blo);
  DM2_hero_2c1d_0186(hero, RG4L, RG2L, 0);

L_fin1:
  RG3L = 0;
  if (vw_18 != 0)
  {
    RG2L = unsignedlong(vw_1c);
    RG4L = unsignedlong(byte_at(RG61p, lcon(0x5)));
    RG73L = unsignedlong(vql_00.peek16());
    DM2_ADJUST_SKILLS(vql_00.peek16(), RG4L, RG2L);
    DM2_ADJUST_HAND_COOLDOWN(RG73L, unsignedlong(vw_18), 2);
  }

L_fin2:
  if (RG3W == 0)
    return RG3L;
  RG51W = unsignedword(byte_at(RG61p, lcon(0x5)));
  RG1L = unsignedlong(byte_at(RG61p, lcon(0x5)));
  if (RG1L > lcon(0x3))
  {
    RG1L = unsignedlong(RG51W);
    RG51L = (RG1L - lcon(0x4)) / 4;
  }
  return RG3L | RG51L;
}

// belongs to DM2_TRY_CAST_SPELL
// was SKW_29ee_0b4a
static i32 DM2_PROCEED_SPELL_FAILURE(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG4L = RG1L;
  RG3L = 1;
  RG1Blo &= lcon(0xfffffff0);
  if (RG1UW < mkuw(lcon(0x20)))
  {
    //m_3391A:
    if (RG1W != lcon(0x10))
      return RG1L;
    RG4Bhi ^= RG1Bhi;
    RG4Blo &= lcon(0xf);
    RG1L = unsignedlong(RG4W);
    RG1Blo = RG1L == lcon(0x3) ? 1 : 0;
    RG1W = unsignedword(RG1Blo);
    RG1L -= 5;
    ddat.v1e0b7c = RG1W;
    RG1L = lcon(0x45);
  }
  else
  {
    if (RG1UW <= mkuw(lcon(0x20)))
    {
      //m_33946:
      ddat.v1e0b7c = lcon(0xfffffffd);
      RG1L = lcon(0x46);
    }
    else
    {
      if (RG1W != lcon(0x30))
        return RG1L;
      //m_33956:
      DM2_DRAW_TRANSPARENT_STATIC_PIC(1, 5, 11, 92, NOALPHA);
      RG1L = lcon(0x44);
      RG3L = 3;
    }
  }
  //m_3397B:
  RG1L = DM2_UPDATE_GLOB_VAR(unsignedlong(RG1W), lcon(0x1), lcon(0x3));
  RG4L = signedlong(CUTX16(lcon(0x8) - RG1L));
  RG1L = signedlong(DM2_MAX(RG3W, RG4W));
  ddat.v1e0b6c = RG1W;
  return RG1L;
}

// belongs to DM2_HANDLE_UI_EVENT
bool DM2_TRY_CAST_SPELL(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;
  i32 RG51l;
  i32 RG52l;
  bool flag;

  RG1W = party.curacthero;
  RG1L--;
  RG51l = unsignedlong(RG1W);
  c_hero* hero = &party.hero[RG51l];
  RG1P = DM2_FIND_SPELL_BY_RUNES(hero->rune);
  if (RG1P != NULL)
  {
    //m_32989:
    RG4L = unsignedlong(hero->rune[0]) - lcon(0x5f);
    RG2L = signedlong(RG4W);
    RG1L = DM2_CAST_SPELL_PLAYER(RG51l, RG2L, RG1P);
    RG4L = RG1L;
  }
  else
    RG4L = lcon(0x20);
  //m_3299F:
  RG1Blo = RG4W != wcon(0x0) ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  flag = RG1L == 0;
  RG52l = RG1L;
  if (!flag)
  {
    RG1L = unsignedlong(RG4W);
    DM2_PROCEED_SPELL_FAILURE(RG1L);
  }
  RG4W &= lcon(0xf0);
  if (RG4W != lcon(0x30))
  {
    hero->rune[0] = RG4Bhi;
    hero->nrunes = RG4Bhi;
    DM2_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
  }
  return RG52l != 0;
}

// belongs to DM2_HANDLE_UI_EVENT
// was SKW_2759_0589
void DM2_ACTIVATE_ACTION_HAND(i16 eaxw, i16 edxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4W = edxw;
  i32 RG5l;
  i16 RG6w;

  RG6w = RG1W;
  RG3L = RG4L;
  RG5l = unsignedlong(RG1W);
  c_hero* hero = &party.hero[RG5l];
  if (hero->curHP != 0)
  {
    RG2L = signedlong(RG3W);
    RG1L = DM2_IS_ITEM_HAND_ACTIVABLE(RG5l, unsignedlong(hero->item[RG3W]), RG2L);
    if (RG1L != 0)
    {
      party.curactmode = RG3W;
      ddat.v1e0b52 = RG3W;
      RG6w++;
      party.curacthero = RG6w;
      ddat.v1e0b54 = RG6w;
      ddat.v1e0b6c = 1;
      ddat.v1e0b62 = 0;
      DM2_CHAMPION_SQUAD_RECOMPUTE_POSITION();
      DM2_UPDATE_RIGHT_PANEL(0);
      DM2_REFRESH_PLAYER_STAT_DISP(CUTX16(RG5l));
    }
  }
}

// was SKW_2759_1204
bool DM2_PROCEED_COMMAND_SLOT(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG6w;
  bool flag;

  RG6w = RG1W;
  RG3L = 0;
  RG4W = party.curacthero;
  if (RG4W == 0)
    return false;
  RG2L = RG4L - 1;
  RG1L = unsignedlong(RG2W);
  c_hero* hero = &party.hero[RG1L];
  if (signedlong(RG6w) != lcon(0xffffffff))
  {
    bool skip00703 = false;
    //m_30EE5:
    hero->heroflag |= 0x800;
    RG4L = unsignedlong(RG6w);
    RG1L = DM2_ENGAGE_COMMAND(RG1L, RG4L);
    RG3L = RG1L;
    RG1L = signedlong(party.curactmode);
    if (RG1L == lcon(0x1) || party.curactmode == 0)
    {
      //m_30F10:
      RG1L = unsignedlong(RG2W) << 4;
      RG4P = &ddat.v1e0adc[RG1L];
      RG1L = signedlong(party.curactmode) << 3;
      RG4P += RG1L;
      flag = false; // assuming to be nonzero
    }
    else
    {
      RG4P = NULL;
      flag = true;
    }
    //m_30F2C:
    if (flag || word_at(RG4P + 2) == 0)
      skip00703 = true;
    else
    {
      if (byte_at(RG4P + lcon(0x4)) != 0)
      {
        RG1W = DM2_RAND16(mkuw(word_at(RG4P) + 1));
        if (RG1W == 0)
          skip00703 = true;
      }
    }

    if (skip00703)
    {
      //m_30F4E:
      RG1L = DM2_events_30DEA(signedlong(RG2W));
      if (RG1L != 0)
      {
        if (RG4P != NULL)
        {
          mov16(location(RG4P), 0);
          RG1W = word_at(RG4P);
          mov16(location(RG4P + 2), RG1W);
          mov8(location(RG4P + lcon(0x4)), RG1Blo);
        }
      }
    }
  }
  else
  {
    RG1L ^= RG2L;
    ddat.v1e0b62 = RG1W;
  }
  //m_30F6D:
  if (ddat.v1e0b62 == 0)
    //m_30F80:
    DM2_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
  else
    ddat.v1e0b62 &= lcon(0xfffffbff);
  //m_30F85:
  DM2_UPDATE_RIGHT_PANEL(0);
  return RG3L != 0;
}

i32 DM2_events_30DEA(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  bool flag;

  RG5w = RG1W;
  RG2L = unsignedlong(ddat.v1e0b7e);
  RG4L = unsignedlong(ddat.v1e0b80);
  RG1L = DM2_QUERY_CMDSTR_ENTRY(unsignedlong(ddat.v1e0b7f), RG4L, RG2L, lcon(0x8));
  RG4L = signedlong(RG1W);
  if (RG4L == lcon(0x10))
    RG1L = lcon(0xf);
  RG4L = signedlong(RG1W);
  if (RG4L != lcon(0x11) && RG4L != lcon(0x12))
  {
    RG1L = -RG1L;
    RG4L = signedlong(RG1W);
    DM2_ADD_ITEM_CHARGE(unsignedlong(ddat.v1e0b50), RG4L);
  }
  //m_30E3D:
  RG1W = DM2_ADD_ITEM_CHARGE(unsignedlong(ddat.v1e0b50), 0);
  RG1Blo = RG1W == wcon(0x0) ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  flag = RG1L == 0;
  RG2L = RG1L;
  if (!flag)
  {
    RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(ddat.v1e0b50), 0);
    RG1W &= lcon(0x800);
    RG1L = unsignedlong(RG1W);
    if (RG1L != 0)
    {
      RG4L = unsignedlong(party.curactmode);
      RG1L = unsignedlong(RG5w);
      DM2_REMOVE_POSSESSION(RG1L, RG4L);
      DM2_DEALLOC_RECORD(unsignedlong(ddat.v1e0b50));
    }
  }
  return RG2L;
}

// stacksize was 0x6c
void DM2_events_3C1E5(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG51p;
  i8* RG52p;
  i8* RG53p;
  i32 RG54l;
  i32 RG55l;
  i32 RG56l;
  i32 RG57l;
  c_wlreg _RG58;
  c_wlreg _RG6; // TODO: one or more variables?
  c_lreg _RG7; // TODO: one or more variables? always bool?
  i16 vw_00;
  i16 vw_04;
  c_ql32 vql_08;
  c_ql32 vql_0c;
  c_tim c38_10;
  i32 vl_1c;
  bool vbool_20;
  i32 vl_24;
  i16 vw_28;
  i32 vl_2c;
  i32 vl_30;
  c_ql32 vql_34;
  i16 vw_38;
  i16 vw_3c;
  i16 vw_40;
  i16 vw_44;
  c_ql32 vql_48;
  i16 vw_4c;
  c_ql32 vql_50;
  c_ql32 vql_54;
  i8 vb_58;
  i16 parw01;
  i16 parw02;
  i16 parw03;
  i16 parw07;
  i16 parw08;
  i16 parw09;
  i16 parw13;
  i16 parw14;
  i16 parw15;
  i16 parw16;
  bool flag;

  vql_0c.poke32(RG1L);
  vql_08.poke32(RG4L);
  vw_04 = RG2W; // truncated
  vw_00 = RG3W; // truncated
  RG6L = argl0;
  RG4L = 0;
  vbool_20 = false;
  vl_1c = 0;
  vl_2c = lcon(0xffff);
  RG4L = signedlong(vql_08.peek16());
  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(vql_0c.peek16(), RG4W));
  vql_48.poke32(RG1L);
  vw_28 = RG1W; // truncated
  vl_24 = 0;
  //m_3C226:
  for (;;)
  {
    RG2L = vql_48.peek32();
    RG1Blo = RG2W != wcon(0xfffe) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 0 && vl_24 == 0)
    {
      bool skip00857 = false;
      RG3L = 0;
      vw_40 = RG3W;
      RG3L = unsignedlong(RG2W);
      RG1L = RG3L >> bcon(0xe);
      vw_4c = RG1W;
      if (RG1W != vw_04)
        skip00857 = true;
      else
      {
        RG1L = DM2_IS_WALL_ORNATE_ALCOVE_FROM_RECORD(unsignedlong(vw_28), RG3L);
        if (RG1L == 0)
        {
          //m_3C38A:
          RG1L = DM2_query_0cee_319e(RG3L);
          if (RG1L == 0)
          {
            bool skip00851 = false;
            bool skip00852 = false;
            bool skip00854 = false;
            bool skip00855 = false;
            //m_3C48E:
            RG1L = RG2L;
            RG1Blo ^= RG2Blo;
            RG1Bhi &= lcon(0x3c);
            RG1L = unsignedlong(RG1W) >> bcon(0xa);
            RG4L = unsignedlong(RG1W);
            if (RG4L != lcon(0x3))
            {
              //m_3CAE7:
              if (vw_00 != lcon(0xffffffff) || RG4L != 2)
              {
                //m_3CDDA:
                RG1L = unsignedlong(RG1W);
                if (RG1L < lcon(0x3))
                  skip00857 = true;
              }
              else
              {
                RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
                RG51p = RG1P;
                RG4W = word_at(RG1P, 2) & lcon(0x6);
                if (RG4W != 2)
                  skip00857 = true;
                else
                {
                  RG4W = word_at(RG1P, 2);
                  RG2L = RG4L;
                  RG2UW >>= bcon(0x3);
                  RG3L = RG2L;
                  RG3Bhi ^= RG2Bhi;
                  vql_54.poke16(RG3W);
                  RG3Blo = vql_54.peek8();
                  vb_58 = RG3Blo;
                  RG3L = signedlong(eventqueue.event_heroidx);
                  if (RG3L == lcon(0xffffffff))
                    skip00857 = true;
                  else
                  {
                    bool skip00850 = false;
                    bool skip00856 = false;
                    RG2L = unsignedlong(RG2W);
                    RG2UL >>= bcon(0x8);
                    RG2L &= lcon(0x1f);
                    vql_34.poke32(RG2L);
                    if (RG2UW < mkuw(lcon(0x7)))
                    {
                      //m_3CB75:
                      if (RG2UW < mkuw(lcon(0x4)))
                        skip00857 = true;
                      else
                      {
                        if (RG2UW <= mkuw(lcon(0x4)))
                          skip00850 = true;
                        else
                        {
                          if (RG2W == lcon(0x6))
                            skip00856 = true;
                          else
                            skip00857 = true;
                        }
                      }
                    }
                    else
                    {
                      if (RG2UW <= mkuw(lcon(0x7)))
                      {
                        //m_3CD23:
                        if ((RG4Blo & bcon(0x1)) != 0)
                          skip00857 = true;
                        else
                          skip00856 = true;
                      }
                      else
                      {
                        if (RG2UW <= mkuw(lcon(0x8)))
                          skip00850 = true;
                        else
                        {
                          if (RG2W == lcon(0xd))
                          {
                            //m_3CDBC:
                            if (RG6W == lcon(0xffffffff))
                            {
                              RG2L = RG4L;
                              RG2Blo &= 0x1;
                              RG2Blo ^= 1;
                              RG2Bhi ^= RG4Bhi;
                              RG2Blo &= 0x1;
                              RG4L &= lcon(0xfffffffe);
                              RG4L |= RG2L;
                              mov16(location(RG1P + 2), RG4W);
                            }
                          }
                          skip00857 = true;
                        }
                      }
                    }

                    if (skip00856)
                    {
                      //m_3CD2C:
                      if (RG6W == lcon(0xffffffff))
                      {
                        RG4L = unsignedlong(vb_58);
                        RG3L = lcon(0x11);
                        RG2L = lcon(0xb);
                        RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, RG4Blo, RG2Blo, RG3Blo);
                        if (RG1W == 0)
                          //m_3CD5C:
                          RG4L = 1;
                        else
                          RG4L = lcon(0xffffffff);
                        //m_3CD61:
                        vw_3c = ddat.v1e0270;
                        vw_38 = ddat.v1e0272;
                        RG4L = signedlong(RG4W);
                        RG1L = DM_LOCATE_OTHER_LEVEL(signedlong(ddat.v1e0266), RG4L, &vw_3c, &vw_38, NULL);
                        if (RG1W >= 0)
                        {
                          RG3L = signedlong(ddat.v1e0258);
                          RG2L = signedlong(RG1W);
                          RG4L = signedlong(vw_38);
                          DM2_map_3BF83(signedlong(vw_3c), RG4L, RG2L, RG3L);
                          eventqueue.event_1031_098e();
                        }
                      }
                      skip00857 = true;
                    }

                    if (skip00850)
                    {
                      //m_3CB90:
                      RG4W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0x9), vb_58, lcon(0xb), lcon(0xe));
                      RG1L = signedlong(vw_4c) << 14;
                      vl_30 = RG1L;
                      if (jz_test8(location(RG51p + 2), lcon(0x1)))
                      {
                        //m_3CCCE:
                        RG1L = unsignedlong(vql_34.peek16());
                        if (RG1L == lcon(0x8))
                          skip00854 = true;
                        else
                        {
                          if (RG6W == lcon(0xffffffff))
                            skip00857 = true;
                          else
                          {
                            RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG6W);
                            if (RG1W != RG4W)
                              skip00857 = true;
                            else
                            {
                              vl_1c = 1;
                              RG1L = signedlong(vql_08.peek16());
                              put16(parw13, RG1W);
                              RG3L = signedlong(vql_0c.peek16());
                              RG6L &= lcon(0xffff3fff);
                              RG1L = RG6L | vl_30;
                              skip00851 = true;
                            }
                          }
                        }
                      }
                      else
                      {
                        if (RG6W != lcon(0xffffffff))
                          skip00855 = true;
                        else
                        {
                          bool skip00853 = false;
                          RG54l = unsignedlong(RG4W);
                          RG2L = signedlong(vw_4c);
                          RG4L = unsignedlong(vql_08.peek16());
                          RG1L = DM2_FIND_DISTINCTIVE_ITEM_ON_TILE(unsignedlong(vql_0c.peek16()), RG4L, RG2L, RG54l);
                          RG6L = RG1L;
                          if (RG1W != lcon(0xffffffff))
                            skip00853 = true;
                          else
                          {
                            RG1L = DM2_ALLOC_NEW_DBITEM(RG54l);
                            RG6L = RG1L;
                            if (RG1W != lcon(0xffffffff))
                            {
                              RG3L = signedlong(vql_08.peek16());
                              RG2L = signedlong(vql_0c.peek16());
                              RG1Bhi &= lcon(0x3f);
                              RG6L = vl_30 | RG1L;
                              RG1L = unsignedlong(RG6W);
                              DM2_APPEND_RECORD_TO(mk_record(RG1L), NULL, RG2W, RG3W);
                              skip00853 = true;
                            }
                          }

                          if (skip00853)
                          {
                            //m_3CC2A:
                            RG55l = unsignedlong(RG6W);
                            DM2_ADD_ITEM_CHARGE(RG55l, lcon(0xf));
                            RG2L = signedlong(vql_08.peek16());
                            RG4L = signedlong(vql_0c.peek16());
                            RG3L = lcon(0xffffffff);
                            RG1L = RG55l;
                            DM2_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, 0);
                            vl_2c = RG6L;
                            RG6L = lcon(0xffff);
                            RG1L = unsignedlong(vql_34.peek16());
                            if (RG1L == lcon(0x8))
                            {
                              RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0x9), vb_58, lcon(0xb), lcon(0x12)) & lcon(0xffff);
                              RG4L = timdat.gametick + mkl(RG1W);
                              c38_10.setmticks(ddat.v1d3248, RG4L + 2);
                              c38_10.settype(0x5c);
                              c38_10.setactor(0);
                              RG1L = vql_48.peek32();
                              c38_10.setA(RG1W);
                              DM2_QUEUE_TIMER(&c38_10);
                            }
                            skip00854 = true;
                          }
                          else
                            skip00857 = true;
                        }
                      }
                    }
                  }
                }
              }
            }
            else
            {
              RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
              RG52p = RG1P;
              RG4W = word_at(RG1P, 2) & lcon(0x7f);
              vql_34.poke32(RG4L);
              RG4W = word_at(RG1P, 2);
              RG4UW >>= bcon(0x7);
              vql_50.poke32(RG4L);
              RG1W = word_at(RG1P, lcon(0x4));
              RG1L <<= 11;
              RG1UW >>= 14;
              vw_44 = RG1W;
              RG1L = signedlong(eventqueue.event_heroidx);
              if (RG1L == lcon(0xffffffff) && vql_34.peek16() != lcon(0x7e))
                skip00857 = true;
              else
              {
                RG1Blo = DM2_GET_WALL_DECORATION_OF_ACTUATOR(RG52p);
                RG4Blo = RG1Blo;
                vb_58 = RG1Blo;
                RG2L = vql_34.peek32();
                if (RG2W == 0)
                  skip00857 = true;
                else
                {
                  bool skip00846 = false;
                  bool skip00848 = false;
                  bool skip00849 = false;
                  RG7L = 1;
                  RG1L = RG2L;
                  if (RG2UW < mkuw(lcon(0x1a)))
                  {
                    //m_3C571:
                    if (RG2UW < mkuw(lcon(0x3)))
                    {
                      //m_3C5A3:
                      if (RG2UW < mkuw(lcon(0x1)))
                        skip00857 = true;
                      else
                      {
                        if (RG2UW <= mkuw(lcon(0x1)))
                        {
                          //m_3C5C4:
                          RG7L = 0;
                          RG1W = word_at(RG52p, lcon(0x4)) & lcon(0x18);
                          if (RG1W == lcon(0x18))
                            skip00857 = true;
                          else
                            skip00848 = true;
                        }
                        else
                        {
                          //m_3C5DD:
                          RG1Blo = RG6W == wcon(0xffff) ? 1 : 0;
                          RG1Bhi ^= RG2Bhi;
                          RG4W = word_at(RG52p, lcon(0x4));
                          RG4L <<= bcon(0xa);
                          RG4UW >>= bcon(0xf);
                          RG1Blo = RG1W != RG4W ? 1 : 0;
                          RG7L = unsignedlong(RG1Blo);
                          skip00848 = true;
                        }
                      }
                    }
                    else
                    {
                      bool skip00845 = false;
                      if (RG2UW <= mkuw(lcon(0x3)))
                        skip00845 = true;
                      else
                      {
                        if (RG2UW < mkuw(lcon(0x17)))
                        {
                          //m_3C598:
                          if (RG2W == lcon(0x15))
                          {
                            //m_3C5FF:
                            RG4L = 0;
                            RG1W = DM2_ADD_ITEM_CHARGE(unsignedlong(RG6W), RG4L);
                            if (RG1W == 0)
                              skip00848 = true;
                            else
                              skip00845 = true;
                          }
                          else
                            skip00857 = true;
                        }
                        else
                        {
                          if (RG2UW <= mkuw(lcon(0x17)))
                          {
                            //m_3C78B:
                            if (RG6W != lcon(0xffffffff))
                              skip00857 = true;
                            else
                            {
                              RG1W = word_at(RG52p, lcon(0x4));
                              RG1L <<= bcon(0xd);
                              RG1UW >>= bcon(0xf);
                              RG1Blo ^= 1;
                              RG1W &= 0x1;
                              and8(location(RG52p + lcon(0x4)), lcon(0xfffffffb));
                              RG1L *= 4;
                              RG2W = word_at(RG52p, lcon(0x4));
                              RG2L |= RG1L;
                              mov16(location(RG52p + lcon(0x4)), RG2W);
                              RG1L = RG2L << bcon(0xd);
                              RG1UW >>= bcon(0xf);
                              RG4L = unsignedlong(RG1W);
                              RG1L = RG2L << bcon(0xa);
                              RG1UW >>= bcon(0xf);
                              RG1L = unsignedlong(RG1W);
                              RG1L ^= RG4L;
                              RG1Blo = RG1L == 0 ? 1 : 0;
                              RG7L = RG1L & lcon(0xff);
                              skip00848 = true;
                            }
                          }
                          else
                          {
                            if (RG2W == lcon(0x18))
                              skip00846 = true;
                            else
                              skip00857 = true;
                          }
                        }
                      }

                      if (skip00845)
                      {
                        //m_3C614:
                        RG1L = signedlong(DM2_GET_DISTINCTIVE_ITEMTYPE(RG6W));
                        RG4L = unsignedlong(vql_50.peek16());
                        RG1Blo = RG4L == RG1L ? 1 : 0;
                        RG1L = unsignedlong(RG1Blo);
                        RG4W = word_at(RG52p, lcon(0x4));
                        RG4L <<= bcon(0xa);
                        RG4UW >>= bcon(0xf);
                        RG4Blo = RG1W == RG4W ? 1 : 0;
                        RG7L = RG4L & lcon(0xff);
                        if (RG1L != 0 && jnz_test8(location(RG52p + lcon(0x4)), lcon(0x4)))
                        {
                          RG1L = DM2_REMOVE_OBJECT_FROM_HAND() & lcon(0xffff);
                          DM2_DEALLOC_RECORD(RG1L);
                        }
                        skip00848 = true;
                      }
                    }
                  }
                  else
                  {
                    if (RG2UW <= mkuw(lcon(0x1a)))
                    {
                      //m_3C837:
                      RG4L = unsignedlong(RG4Blo);
                      RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0x9), RG4Blo, lcon(0xb), lcon(0xe));
                      RG4W = RG1W;
                      if (jz_test8(location(RG52p + lcon(0x4)), lcon(0x4)))
                      {
                        //m_3C906:
                        if (RG6W == lcon(0xffffffff))
                          skip00857 = true;
                        else
                        {
                          RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG6W);
                          if (RG1W != RG4W)
                            skip00857 = true;
                          else
                          {
                            vl_1c = RG7L;
                            RG1L = signedlong(vql_08.peek16());
                            put16(parw13, RG1W);
                            RG3L = signedlong(vql_0c.peek16());
                            RG6L &= lcon(0xffff3fff);
                            RG1L = (signedlong(vw_4c) << 14) | RG6L;
                            skip00851 = true;
                          }
                        }
                      }
                      else
                      {
                        if (RG6W == lcon(0xffffffff))
                        {
                          bool skip00847 = false;
                          //m_3C86C:
                          RG56l = unsignedlong(RG1W);
                          RG2L = signedlong(vw_4c);
                          RG4L = unsignedlong(vql_08.peek16());
                          RG1L = DM2_FIND_DISTINCTIVE_ITEM_ON_TILE(unsignedlong(vql_0c.peek16()), RG4L, RG2L, RG56l);
                          RG6L = RG1L;
                          if (RG1W != lcon(0xffffffff))
                            skip00847 = true;
                          else
                          {
                            RG1L = DM2_ALLOC_NEW_DBITEM(RG56l);
                            RG6L = RG1L;
                            if (RG1W != lcon(0xffffffff))
                            {
                              RG3L = signedlong(vql_08.peek16());
                              RG2L = signedlong(vql_0c.peek16());
                              RG4P = NULL;
                              RG6L &= lcon(0xffff3fff);
                              RG1L = RG6L;
                              RG6L = (signedlong(vw_4c) << 14) | RG1L;
                              RG1L = unsignedlong(RG6W);
                              DM2_APPEND_RECORD_TO(mk_record(RG1L), UPCAST(t_record, RG4P), RG2W, RG3W);
                              skip00847 = true;
                            }
                          }

                          if (skip00847)
                          {
                            //m_3C8D3:
                            RG57l = unsignedlong(RG6W);
                            DM2_ADD_ITEM_CHARGE(RG57l, lcon(0xf));
                            RG2L = signedlong(vql_08.peek16());
                            RG4L = signedlong(vql_0c.peek16());
                            RG3L = lcon(0xffffffff);
                            RG1L = RG57l;
                            DM2_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, 0);
                            vl_2c = RG6L;
                            skip00852 = true;
                          }
                          else
                            skip00857 = true;
                        }
                        else
                        {
                          vl_24 = RG7L;
                          skip00857 = true;
                        }
                      }
                    }
                    else
                    {
                      if (RG2UW < mkuw(lcon(0x3f)))
                      {
                        //m_3C558:
                        if (RG2UW <= mkuw(lcon(0x1b)))
                        {
                          //m_3C96F:
                          if (vql_50.peek16() == 0)
                            skip00857 = true;
                          else
                          {
                            RG4W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, RG4Blo, 11, 14);
                            RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG6W);
                            if (RG1W != RG4W)
                              skip00857 = true;
                            else
                            {
                              vl_1c = RG7L;
                              RG6L = lcon(0xffff);
                              RG3L = vql_50.peek32();
                              flag = --RG3W != 0;
                              vql_50.poke32(RG3L);
                              if (!flag)
                              {
                                RG7L = 0;
                                or8(location(RG52p + lcon(0x4)), lcon(0x1));
                              }
                              RG1L = vql_50.peek32();
                              RG1Bhi &= 0x1;
                              RG4W = word_at(RG52p, 2);
                              RG4L &= lcon(0xffff007f);
                              RG1L <<= bcon(0x7);
                              RG2L = RG4L | RG1L;
                              mov16(location(RG52p + 2), RG2W);
                              skip00848 = true;
                            }
                          }
                        }
                        else
                        {
                          if (RG2W == lcon(0x1c))
                          {
                            //m_3C9E6:
                            RG1Blo = RG6W != wcon(0xffff) ? 1 : 0;
                            RG1L = unsignedlong(RG1Blo);
                            if (RG1L != 0)
                              skip00857 = true;
                            else
                            {
                              RG1Blo = byte_at(RG52p, lcon(0x4));
                              if ((RG1Blo & bcon(0x4)) != 0)
                                skip00857 = true;
                              else
                              {
                                if ((RG1Blo & bcon(0x20)) == 0)
                                {
                                  //m_3CA14:
                                  RG1W = word_at(RG52p, lcon(0x4));
                                  RG1L <<= 11;
                                  RG1UW >>= 14;
                                  RG4L = unsignedlong(RG1W);
                                  RG1L = signedlong(ddat.v1e0258) + RG4L & lcon(0x3);
                                  RG4L = RG1L;
                                }
                                else
                                {
                                  RG4W = word_at(RG52p, lcon(0x4));
                                  RG4L <<= 11;
                                  RG4UW >>= 14;
                                }
                                //m_3CA33:
                                RG3L = signedlong(RG4W);
                                RG1L = vql_50.peek32();
                                RG1W &= lcon(0x3f);
                                RG2L = signedlong(RG1W);
                                RG1W = word_at(RG52p, lcon(0x6));
                                vql_54.poke32(RG1L);
                                _RG1.ushiftr11();
                                RG4L = signedlong(RG1W);
                                RG1L = vql_54.peek32() << 5;
                                _RG1.ushiftr11();
                                vql_54.poke16(RG1W);
                                DM2_map_3BF83(signedlong(vql_54.peek16()), RG4L, RG2L, RG3L);
                                eventqueue.event_1031_098e();
                                skip00848 = true;
                              }
                            }
                          }
                          else
                            skip00857 = true;
                        }
                      }
                      else
                      {
                        if (RG2UW <= mkuw(lcon(0x3f)))
                        {
                          //m_3C5B1:
                          if (RG6W == lcon(0xffffffff))
                            and8(location(RG52p + lcon(0x4)), lcon(0xfffffffe));
                          skip00848 = true;
                        }
                        else
                        {
                          if (RG2UW < mkuw(lcon(0x4a)))
                          {
                            //m_3C549:
                            if (RG2W == lcon(0x46))
                            {
                              //m_3C66F:
                              RG1W = word_at(RG52p, lcon(0x6));
                              RG4L = RG1L;
                              _RG4.ushiftr11();
                              RG4L = signedlong(RG4W);
                              RG1L <<= bcon(0x5);
                              _RG1.ushiftr11();
                              RG1RP = DM2_GET_ADDRESS_OF_TILE_RECORD(RG1W, RG4W);
                              if (jz_test8(location(RG1P + lcon(0x3)), lcon(0x20)))
                                skip00857 = true;
                              else
                                skip00846 = true;
                            }
                            else
                              skip00857 = true;
                          }
                          else
                          {
                            if (RG2UW <= mkuw(lcon(0x4a)))
                              skip00846 = true;
                            else
                            {
                              if (RG2W == lcon(0x7e))
                              {
                                //m_3C7E5:
                                if (jnz_test8(location(RG52p + lcon(0x4)), lcon(0x4)))
                                {
                                  RG2L = signedlong(ddat.v1e0258);
                                  RG1L = RG2L + 2 & lcon(0x3);
                                  RG4L = unsignedlong(vw_04);
                                  if (RG4L == RG1L)
                                  {
                                    RG3L = signedlong(ddat.v1e0266);
                                    RG4L = signedlong(ddat.v1e0272);
                                    DM2_SELECT_CHAMPION(signedlong(ddat.v1e0270), RG4L, RG2L, RG3L);
                                    vl_24 = RG7L;
                                    RG7L = 0;
                                  }
                                }
                                skip00848 = true;
                              }
                              else
                                skip00857 = true;
                            }
                          }
                        }
                      }
                    }
                  }

                  if (skip00846)
                  {
                    //m_3C693:
                    RG1Blo = RG6W != wcon(0xffff) ? 1 : 0;
                    RG1L = unsignedlong(RG1Blo);
                    if (RG1L != 0 || jnz_test8(location(RG52p + lcon(0x4)), lcon(0x1)))
                      skip00857 = true;
                    else
                    {
                      c38_10.setmticks(ddat.v1d3248, unsignedlong(vql_50.peek16()) + timdat.gametick + 2);
                      c38_10.settype(0x57);
                      c38_10.setactor(0);
                      RG1L = vql_48.peek32();
                      c38_10.setA(RG1W);
                      RG1W = DM2_QUEUE_TIMER(&c38_10);
                      RG1Bhi = byte_at(RG52p, lcon(0x4)) | bcon(0x1);
                      mov8(location(RG52p + lcon(0x4)), RG1Bhi);
                      RG7L = 0;
                      if ((RG1Bhi & bcon(0x20)) != 0)
                        vw_40 = 0x10;
                      if (vw_44 != lcon(0x3)) // was || RG7 != 0, but that's always false here
                        skip00848 = true;
                      else
                      {
                        RG3L = 1;
                        vbool_20 = true;
                        if (jnz_test8(location(RG52p + lcon(0x4)), lcon(0x40)))
                        {
                          put16(parw09, RG3W);
                          RG1L = signedlong(ddat.v1e0272);
                          put16(parw08, RG1W);
                          RG1L = signedlong(ddat.v1e0270);
                          put16(parw07, RG1W);
                          DM2_QUEUE_NOISE_GEN2(lcon(0x9), vb_58, lcon(0x88), lcon(0xfe), parw07, parw08, parw09, lcon(0x8c), lcon(0x80));
                        }
                        DM2_INVOKE_ACTUATOR(RG52p, 0, signedlong(vw_40));
                        RG1L = vql_50.peek32() + 1 + unsignedlong(vw_40);
                        RG2L = signedlong(RG1W);
                        RG4L = 1;
                        skip00849 = true;
                      }
                    }
                  }

                  if (skip00848)
                  {
                    //m_3CA71:
                    if (vw_44 == lcon(0x3))
                    {
                      RG1Blo = RG7L != 0 ? 1 : 0;
                      RG1W = unsignedword(RG1Blo);
                      vw_44 = RG1W; // truncated
                      RG7L = 0;
                    }
                    if (RG7L != 0)
                      skip00857 = true;
                    else
                    {
                      RG7L = 1;
                      vbool_20 = true;
                      if (jnz_test8(location(RG52p + lcon(0x4)), lcon(0x40)))
                      {
                        put16(parw16, wcon(0x1));
                        RG1L = signedlong(ddat.v1e0272);
                        put16(parw15, RG1W);
                        RG1L = signedlong(ddat.v1e0270);
                        put16(parw14, RG1W);
                        DM2_QUEUE_NOISE_GEN2(lcon(0x9), vb_58, lcon(0x88), lcon(0xfe), parw14, parw15, parw16, lcon(0x8c), lcon(0x80));
                      }
                      RG2L = signedlong(vw_40);
                      RG4L = unsignedlong(vw_44);
                      skip00849 = true;
                    }
                  }

                  if (skip00849)
                  {
                    //m_3C77F:
                    RG1P = RG52p;
                    DM2_INVOKE_ACTUATOR(RG1P, RG4L, RG2L);
                    skip00857 = true;
                  }
                }
              }
            }

            if (skip00851)
            {
              //m_3C945:
              RG1L = unsignedlong(RG1W);
              RG2L = 0;
              RG4L = lcon(0xffffffff);
              DM2_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw13);
              skip00852 = true;
            }

            if (skip00852)
            {
              //m_3C956:
              RG6L = lcon(0xffff);
              skip00854 = true;
            }

            if (skip00854)
            {
              //m_3C95B:
              ddat.v1e0390.b_00 |= 1;
              skip00855 = true;
            }

            if (skip00855)
            {
              //m_3C962:
              vl_24 = 1;
              skip00857 = true;
            }
          }
          else
          {
            if (RG6W != lcon(0xffffffff))
            {
              bool skip00843 = false;
              //m_3C437:
              RG2L = unsignedlong(RG6W);
              RG4L = 0;
              RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG2L, RG4L);
              RG1W &= 0x1;
              RG1L = unsignedlong(RG1W);
              if (RG1L == 0)
              {
                //m_3C46D:
                RG1L = signedlong(DM2_GET_DISTINCTIVE_ITEMTYPE(RG2W));
                if (RG1L == lcon(0x194))
                {
                  DM2_SET_ITEMTYPE(RG2L, lcon(0xf));
                  skip00843 = true;
                }
              }
              else
              {
                DM2_ADD_ITEM_CHARGE(RG2L, lcon(0xf));
                skip00843 = true;
              }

              if (skip00843)
              {
                //m_3C45C:
                vl_1c = 1;
                vl_2c = RG6L;
              }
            }
            else
            {
              RG1L = signedlong(eventqueue.event_heroidx);
              if (RG1L != lcon(0xffffffff))
              {
                RG4L = 0;
                RG6L = lcon(0x800);
                //m_3C3DB:
                for (;;)
                {
                  RG3L = signedlong(RG4W);
                  if (RG3L >= lcon(0x4))
                  {
                    RG1L = signedlong(ddat.v1e0272);
                    put16(parw03, RG1W);
                    RG1L = signedlong(ddat.v1e0270);
                    put16(parw02, RG1W);
                    RG4L = signedlong(eventqueue.event_heroidx);
                    RG1L = 263 * RG4L;
                    DM2_QUEUE_NOISE_GEN2(lcon(0x16), party.hero[RG4L].herotype, lcon(0x83), lcon(0xfe), parw02, parw03, 0, lcon(0x96), lcon(0x80));
                    break;
                  }
                  //m_3C3BD:
                  RG1L = 263 * RG3L;
                  if (party.hero[RG3L].curHP != 0)
                    party.hero[RG3L].water = RG6W;
                  RG4L++;
                }
              }
            }
          }
        }
        else
        {
          if (RG6W != lcon(0xffffffff))
          {
            //m_3C351:
            vl_1c = 1;
            RG1L = signedlong(vql_08.peek16());
            put16(parw01, RG1W);
            RG3L = signedlong(vql_0c.peek16());
            RG6L &= lcon(0xffff3fff);
            RG1L = signedlong(vw_4c) << 14;
            RG6L |= RG1L;
            RG1L = unsignedlong(RG6W);
            DM2_MOVE_RECORD_TO(RG1L, lcon(0xffffffff), 0, RG3L, parw01);
          }
          else
          {
            RG1L = DM2_GET_WALL_TILE_ANYITEM_RECORD(signedlong(vql_0c.peek16()), vql_08.peek16());
            RG58L = RG6L;
            //m_3C29D:
            for (;;)
            {
              if (RG1W == lcon(0xfffffffe))
                break;
              RG4L = unsignedlong(RG1W) >> bcon(0xe);
              RG2L = unsignedlong(vw_4c);
              if (RG2L == RG4L)
                RG58L = RG1L;
              RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG1UW)));
            }
            //m_3C2C4:
            if (RG58W != lcon(0xffffffff))
            {
              RG1L = RG58L;
              RG1W &= lcon(0x3c00);
              RG1L = unsignedlong(RG1W) >> bcon(0xa);
              RG1L = signedlong(RG1W);
              if (RG1L >= lcon(0x5) && RG1L <= lcon(0xa))
              {
                RG2L = signedlong(vql_08.peek16());
                RG4L = signedlong(vql_0c.peek16());
                RG1L = unsignedlong(RG58W);
                DM2_MOVE_RECORD_TO(RG1L, RG4L, RG2L, lcon(0xffffffff), 0);
                vl_2c = RG58L;
              }
            }
          }
        }
      }

      if (skip00857)
      {
        //m_3CDE8:
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_48.peek16()))));
        vql_48.poke32(RG1L);
        continue;
      }
    }

    //m_3C313:
    if (vw_00 == lcon(0xffffffff))
    {
      if (vl_1c != 0)
        RG1L = DM2_REMOVE_OBJECT_FROM_HAND();
      RG3L = vl_2c;
      if (RG3W != lcon(0xffffffff))
      {
        RG1L = unsignedlong(RG3W);
        DM2_TAKE_OBJECT(RG1L, lcon(0x1));
      }
      ddat.glbRefreshViewport = vbool_20;
      return;
    }
    //m_3CDFD:
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(vw_00)));
    RG53p = RG1P;
    RG3L = vl_1c;
    if (RG3L == 0)
    {
      RG1Blo = mkw(argl0) != wcon(0xffff) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L != 0)
        DM2_APPEND_RECORD_TO(mk_record(unsignedlong(mkuw(argl0))), UPCAST(t_record, RG53p + 2), -1, RG3W);
    }
    RG2L = vl_2c;
    if (RG2W != lcon(0xffffffff))
    {
      RG4P = RG53p + 2;
      RG1L = unsignedlong(RG2W);
      DM2_APPEND_RECORD_TO(mk_record(RG1L), UPCAST(t_record, RG4P), -1, 0);
    }
    return;
  }
  FOREND
}

// belongs to DM2_events_443c_0434
static void DM2_443c_0004(t_bmp* srcbmp, t_bmp* destbmp, t_palette* palette)
{
  t_palette locpal[PAL16];

  t_bmp* bmp = DM2_ALLOC_PICT_BUFF(16, 16, BIGPOOL_LO, 4);
  //m_445A:
  for (i16 i=0; i < PAL16; i++)
    //m_444E:
    locpal[i] = paldat.palette[E_COL00];
  locpal[E_COL12] = palette[E_COL12];
  DM2_FILL_ENTIRE_PICT(destbmp, palettecolor_to_pixel(palette[E_COL12]));
  DM2_DRAW_DIALOGUE_PICT(srcbmp, destbmp, &mouse_cur2.rect1, 0, 0, 0xc, locpal);
  DM2_DRAW_DIALOGUE_PICT(srcbmp, destbmp, &mouse_cur2.rect2, 0, 0, 0xc, palette);
  DM2_FREE_PICT_BUFF(bmp);
}

void DM2_events_443c_0434(void)
{
  eventqueue.event_unk0f = ddat.savegamewpc.w_00 != -1;
  if (eventqueue.event_unk0f)
  {
    t_bmp* bmp = DM2_ALLOC_PICT_BUFF(18, 18, BIGPOOL_LO, 8);
    DM2_443c_0004(ddat.savegamewpc.bmp, bmp, ddat.savegamewpc.palette);
    generate_cursor(PIXELCAST(bmp), 2, 8, 8, 18, 18, BPP_8, NULL, palettecolor_to_ui8(ddat.savegamewpc.palette[12]));
    DM2_FREE_PICT_BUFF(bmp);
  }
  DM2_REFRESHMOUSE();
}
