#include <skrandom.h>
#include <dm2data.h>
#include <util.h>
#include <fileio.h>
#include <uitmouse.h>
#include <skstr.h>
#include <skalloc.h>
#include <uiinput.h>
#include <skalocdb.h>
#include <bcreatur.h>
#include <startend.h>
#include <sktimer.h>
#include <skrecord.h>
#include <skeventq.h>
#include <skgdtqdb.h>
#include <gfxstr.h>
#include <gfxpal.h>
#include <gfxmain.h>
#include <skguidrw.h>
#include <skmap.h>
#include <skhero.h>
#include <bgdat.h>
#include <sktimprc.h>
#include <skxrect.h>
#include <uikeybd.h>
#include <startend.h>
#include <bitem.h>
#include <uidialog.h>
#include <SK1C9A.h>
#include <skweathr.h>
#include <skevents.h>
#include <skmove.h>
#include <SK0AAF.h>
#include <sksvgame.h>

struct s_hex30
{
  c_hex2a c27_00;
  i16 w_2a; // @64 not confirmed yet
  union
  {
    t_text* tp_2c; // @68 save
    i32 l_2c; // @68 load
  };
};

struct s_savegamebuffer // was s_hex3c size 0x3c
{
  i32 l_00;
  ui32 ul_04;
  i16 w_08;
  i16 w_0a;
  i16 w_0c;
  i16 w_0e;
  i16 w_10;
  i16 w_12; // e_hero
  i16 w_14; // num_timers
  i32 l_16;
  i32 l_1a;
  i16 w_1e;
  i16 w_20;
  i16 w_22;
  i32 l_24; // not confirmed yet
  i16 w_28;
  i32 l_2a;
  i8 b_2e;
  i8 b_2f;
  i8 b_30;
  i8 b_31;
  i8 b_32;
  i8 b_33;
  i16 w_34;
  i8 b_36;
  i8 b_37;
  i32 l_38;
};

#include "emu.h"
#include "regs.h"

// belongs to DM2_READ_DUNGEON_STRUCTURE
static void DM2_savegame_3a15_0002(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  if (timdat.timerarray == NULL)
  {
    RG2L = signedlong(timdat.max_timers);
    timdat.timerarray = UPCAST(c_tim, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(12 * RG2L));
    RG1P = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(2 * signedlong(timdat.max_timers));
    timdat.timer_indices = UPCAST(i16, RG1P);
  }
  //m_45E12:
  if (ddat.cd.mo.glbSpecialScreen != _MENU_SCREEN__RESUME_GAME_SELECT)
  {
    c_tim* RG11timp = timdat.timerarray;
    RG4L = 0;
    //m_45E30:
    for (;;)
    {
      RG3L = unsignedlong(RG4W);
      RG2L = signedlong(timdat.max_timers);
      if (RG3L >= RG2L)
      {
        RG2L = 0;
        timdat.num_timer_indices = RG2W;
        (RG11timp - 1)->setdataw(-1);
        RG3L ^= RG4L;
        timdat.num_timers = RG3W;
        timdat.available_timeridx = RG2W;
        break;
      }
      //m_45E25:
      RG11timp->clrtype();
      RG4L++;
      RG11timp->setdataw(RG4W);
      RG11timp++;
    }
  }
  //m_45E61:
  timdat.timer_unk = -1;
}

// belongs to DM2_READ_DUNGEON_STRUCTURE
static void DM2_savegame_2066_2498(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG5p;
  t_text vba_00[0xc8]; // text in the beginning, struct for DM2_QUERY_MESSAGE_TEXT()

  RG5p = ADRESSOF(t_text, vba_00);
  DM2_FILL_STR(vba_00, 49, 1, 64);
  RG3L = 0;
  RG2L = 0;
  //m_268D5:
  for (;;)
  {
    RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
    RG1Blo = byte_at(RG1P, lcon(0x4));
    RG1W = unsignedword(RG1Blo);
    if (RG2W >= RG1W)
    {
      RG1L = signedlong(RG3W) + 1;
      RG2L = lcon(0x400);
      RG1P = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(RG1L);
      ddat.v1e03d4 = RG1P;
      DM2_CHANGE_CURRENT_MAP_TO(0);
      RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
      RG4L = signedlong(CUTX16(unsignedlong(word_at(RG1P, lcon(0x8))) >> 5 & lcon(0x1f)));
      RG1W = word_at(RG1P, lcon(0x8)) & lcon(0x1f);
      RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(RG1W, RG4W));
      break;
    }
    //m_268B4:
    RG4L = signedlong(RG3W);
    RG1L = signedlong(RG2W) << 4;
    RG1W = word_at(ddat.v1e03c8, RG1L + lcon(0x8)) & lcon(0x3f);
    RG1L = signedlong(DM2_MAX(RG1W, RG4W));
    RG3L = RG1L;
    RG2L++;
  }
  //m_26925:
  for (;;)
  {
    RG4Blo = RG1W != wcon(0xfffe) ? 1 : 0;
    RG4L = unsignedlong(RG4Blo);
    if (RG4L != 0)
    {
      RG4L = RG1L;
      RG4Blo ^= RG1Blo;
      RG4Bhi &= lcon(0x3c);
      RG4L = unsignedlong(RG4W) >> bcon(0xa);
      if (RG4L != 2)
      {
        RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG1UW)));
        continue;
      }
    }
    break;
  }
  //m_26955:
  RG4Blo = RG1W != wcon(0xfffe) ? 1 : 0;
  RG4L = unsignedlong(RG4Blo);
  if (RG4L != 0)
  {
    RG4L = unsignedlong(RG1W);
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
    if (jz_test8(location(RG1P + 2), lcon(0x6)))
    {
      RG2L = lcon(0xffff8002);
      RG1P = RG5p;
      DM2_QUERY_MESSAGE_TEXT(RG1T, RG4L, RG2L);
    }
  }
  //m_26982:
  RG2L = 0;
  //m_269AA:
  for (;;)
  {
    if (RG2W > RG3W)
      return;
    //m_26986:
    for (;;)
    {
      RG1Blo = byte_at(RG5p);
      RG5p++;
      if (RG1Blo != 0)
      {
        RG4L = unsignedlong(RG1Blo);
        if (RG4L != lcon(0xa) && RG4L != lcon(0x20))
        {
          RG4Blo -= lcon(0x30);
          RG1P = ddat.v1e03d4;
          mov8(location(RG1P + signedlong(RG2W)), RG4Blo);
          RG2L++;
          break;
        }
      }
    }
  }
  FOREND
}

static i32 DM2_READ_DUNGEON_STRUCTURE(i32 eaxl)
{
  union u_mbuf
  {
    i8 barr_00[0x8];
    i16 w_00; // grabs the first word for a check TODO clean that, split the const
  };

  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG51;
  c_wlreg _RG52;
  c_wlreg _RG53;
  c_wlreg _RG54;
  c_wlreg _RG55;
  c_wlreg _RG61;
  c_wlreg _RG62;
  i32 RG63l;
  i32 RG71l;
  i8* RG72p;
  i32 vl_00;
  u_mbuf u24_04;
  unk* xp_0c;
  i16 vw_10;
  c_o32 vo_14; // poke 16 peek 16 32 inc16

  vl_00 = RG1L;
  RG4L = lcon(0x8);
  RG1L = DM2_SKLOAD_READ(ADRESSOF(i8, u24_04.barr_00), RG4W) ? 1 : 0;
  if (RG1L == 0)
    return 0;
  if (u24_04.w_00 == lcon(0xffff8104))
    return 0;
  RG1L = DM2_FILE_SEEK(signedlong(ddat.savegamefilehandle1), ddat.savegamel1) ? 1 : 0;
  if (RG1L == 0)
    return 0;
  if (ddat.v1d652d != 0)
  {
    RG2L = lcon(0x400);
    RG4L = 1;
    RG1P = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(lcon(0x2c));
    ddat.savegamep4 = UPCAST(s_sgwords, RG1P);
  }
  //m_26A24:
  RG1L = DM2_SKLOAD_READ(DOWNCAST(s_sgwords, ddat.savegamep4), lcon(0x2c)) ? 1 : 0;
  if (RG1L == 0)
    return 0;
  RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
  RG51W = word_at(RG1P, lcon(0x8));
  RG4L = RG51L;
  RG4W &= lcon(0x1f);
  ddat.v1e0270 = RG4W;
  RG51W >>= bcon(0x5);
  RG4L = RG51L;
  RG4W &= lcon(0x1f);
  ddat.v1e0272 = RG4W;
  RG4L = signedlong(RG51W) >> 5 & lcon(0x3);
  ddat.v1e0258 = RG4W;
  RG2L = 0;
  ddat.v1e0266 = RG2W;
  RG1Blo = byte_at(RG1P, lcon(0x4));
  RG1W = unsignedword(RG1Blo);
  vw_10 = RG1W;
  if (ddat.v1d652d != 0)
  {
    RG1L = signedlong(vw_10) << 4;
    RG2L = lcon(0x400);
    RG1P = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(RG1L);
    ddat.v1e03c8 = RG1P;
  }
  //m_26AAE:
  RG1W = vw_10 << 4;
  RG4L = unsignedlong(RG1W);
  RG1L = DM2_SKLOAD_READ(ddat.v1e03c8, RG4W) ? 1 : 0;
  if (RG1L == 0)
    return 0;
  if (ddat.v1d652d != 0)
  {
    RG2L = lcon(0x400);
    RG1P = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(2 * signedlong(vw_10));
    ddat.v1e03e4 = UPCAST(i16, RG1P);
  }
  //m_26AF2:
  RG52L = 0;
  RG61L = 0;
  RG3W = vw_10;
  //m_26B2F:
  for (;;)
  {
    if (RG52W >= RG3W)
    {
      ddat.v1e0408 = RG61W;
      RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
      RG52W = word_at(RG1P, lcon(0xa));
      if (vl_00 != 0)
      {
        RG4L = RG52L + lcon(0x12c);
        mov16(location(RG1P + lcon(0xa)), RG4W);
      }
      if (ddat.v1d652d != 0)
      {
        RG1P = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(2 * unsignedlong(RG61W));
        ddat.v1e03d8 = RG1P;
        RG1P = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(2 * unsignedlong(ddat.savegamep4->warr_00[5]));
        dm2_v1e038c = UPCAST(t_record, RG1P);
        RG1L = 2 * unsignedlong(ddat.savegamep4->warr_00[3]);
        RG2L = lcon(0x400);
        RG1P = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(RG1L);
        ddat.v1e03d0 = RG1P;
      }
      break;
    }
    //m_26AFC:
    RG1L = signedlong(RG52W);
    RG2L = RG1L;
    ddat.v1e03e4[RG2L] = RG61W;
    RG1L <<= 4;
    RG1W = word_at(ddat.v1e03c8, RG1L + lcon(0x8));
    RG1L <<= bcon(0x5);
    _RG1.ushiftr11();
    RG1L = unsignedlong(RG1W);
    RG61L = unsignedlong(RG61W);
    RG1L++;
    RG61L += RG1L;
    RG52L++;
  }
  //m_26BC2:
  RG61L = 2 * RG61L;
  RG4L = unsignedlong(RG61W);
  RG1L = DM2_SKLOAD_READ(ddat.v1e03d8, RG4W) ? 1 : 0;
  if (RG1L == 0)
    return 0;
  RG1L = 2 * RG52L;
  RG4L = unsignedlong(RG1W);
  RG1L = DM2_SKLOAD_READ(DOWNCAST(t_record, dm2_v1e038c), RG4W) ? 1 : 0;
  if (RG1L == 0)
    return 0;
  if (vl_00 != 0)
  {
    RG3L = 0;
    //m_26C11:
    for (;;)
    {
      RG1L = signedlong(RG3W);
      if (RG1L >= lcon(0x12c))
        break;
      //m_26C00:
      RG1L = signedlong(RG52W);
      RG4RP = dm2_v1e038c;
      RG52L++;
      mov16(location(RG4P + 2 * RG1L), lcon(0xffffffff));
      RG3L++;
    }
  }
  //m_26C1B:
  RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
  RG1W = word_at(RG1P, lcon(0x6)); // TODO: gets extended w/o conversion now
  RG1L = 2 * RG1L;
  RG4L = unsignedlong(RG1W);
  RG1L = DM2_SKLOAD_READ(ddat.v1e03d0, RG4W) ? 1 : 0;
  if (RG1L == 0)
    return 0;
  if (!ddat.savegamel2)
    timdat.max_timers = 50;
  RG53L = 0;
  //m_26D66:
  for (;;)
  {
    RG1L = signedlong(RG53W);
    if (RG1L >= lcon(0x10))
    {
      if (ddat.v1d652d != 0)
      {
        RG1P = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(unsignedlong(ddat.savegamep4->warr_00[1]));
        ddat.v1e03e0 = RG1P;
      }
      break;
    }
    //m_26C56:
    RG4L = RG1L;
    RG3P = DOWNCAST(s_sgwords, ddat.savegamep4);
    RG3W = word_at(RG3P, 2 * RG4L + lcon(0xc));
    if (vl_00 != 0)
    {
      RG4L ^= RG1L;
      RG4Blo = table1d281c[RG1L];
      RG4L += RG3L;
      RG4L = signedlong(RG4W);
      if (RG1L != lcon(0xf))
        //m_26C82:
        RG1L = lcon(0x400);
      else
        RG1L = lcon(0x300);
      //m_26C87:
      RG1L = signedlong(DM2_MIN(RG1W, RG4W));
      RG2L = 2 * signedlong(RG53W);
      ddat.savegamep4->warr_00[signedlong(RG53W) + lcon(0x6)] = RG1W;
    }
    //m_26C9D:
    RG71l = signedlong(RG53W);
    RG61W = unsignedword(table_recordsizes[RG71l]);
    if (ddat.v1d652d != 0)
    {
      RG1P = DOWNCAST(i16, &ddat.savegamep4->warr_00[RG71l]);
      RG4L = unsignedlong(word_at(RG1P, lcon(0xc)));
      RG1P = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(unsignedlong(RG61W) * RG4L);
      recordptr[RG71l] = UPCAST(t_record, RG1P);
    }
    //m_26CE1:
    RG71l = signedlong(RG53W);
    RG2RP = recordptr[RG71l];
    RG1L = RG61L * RG3L;
    RG4L = unsignedlong(RG1W);
    RG1L = DM2_SKLOAD_READ(RG2P, RG4W) ? 1 : 0;
    if (RG1L == 0)
      return 0;
    if (!ddat.savegamel2)
    {
      if (RG71l == lcon(0x4) || RG71l >= lcon(0xe))
      {
        RG1L = signedlong(RG53W);
        RG1W = ddat.savegamep4->warr_00[RG1L + lcon(0x6)];
        timdat.max_timers += RG1W;
      }
    }
    if (vl_00 != 0)
    {
      RG61UW >>= bcon(0x1);
      RG1L = unsignedlong(RG61W);
      RG4L = signedlong(RG3W);
      RG1L *= RG4L;
      RG1L = 2 * RG1L;
      RG2P += RG1L;
      RG1L = signedlong(RG53W);
      RG3L = unsignedlong(table1d281c[RG1L]);
      //m_26D60:
      for (;;)
      {
        if (RG3W == 0)
          break;
        //m_26D51:
        mov16(location(RG2P), lcon(0xffffffff));
        RG1L = 2 * unsignedlong(RG61W);
        RG2P += RG1L;
        RG3L--;
      }
    }
    //m_26D65:
    RG53L++;
  }
  //m_26D9D:
  RG4L = unsignedlong(ddat.savegamep4->warr_00[1]);
  RG1L = DM2_SKLOAD_READ(ddat.v1e03e0, RG4W) ? 1 : 0;
  if (RG1L == 0)
    return 0;
  if (ddat.v1d652d != 0)
  {
    RG1L = unsignedlong(ddat.v1e0408);
    RG3L = signedlong(vw_10);
    RG1L += RG3L;
    RG1L *= 4;
    RG1P = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(RG1L);
    mapdat.tmpmap = UPCAST(t_tile*, RG1P);
    RG4P = RG1P + 4 * RG3L;
    RG3L = 0;
    //m_26E5D:
    for (;;)
    {
      if (RG3W >= vw_10)
        break;
      //m_26DFA:
      RG1L = signedlong(RG3W);
      movp(location(DOWNCAST(t_tile*, mapdat.tmpmap) + 4 * RG1L), RG4P);
      RG1L <<= 4;
      RG1P = ddat.v1e03c8 + RG1L;
      RG2P = ddat.v1e03e0 + unsignedlong(word_at(RG1P));
      movp(location(RG4P), RG2P);
      RG54L = 1;
      RG4P += 4;
      //m_26E3F:
      for (;;)
      {
        RG62L = (signedlong(RG3W) << 4);
        RG1P = ddat.v1e03c8 + RG62L;
        RG62W = word_at(RG1P, lcon(0x8));
        RG62L <<= bcon(0x5);
        RG62UW >>= bcon(0xb);
        if (RG54UW > RG62UW)
        {
          RG3L++;
          break;
        }
        //m_26E29:
        RG1W = word_at(RG1P, lcon(0x8));
        _RG1.ushiftr11();
        RG1L = unsignedlong(RG1W) + 1;
        RG2P += RG1L;
        movp(location(RG4P), RG2P);
        RG4P += 4;
        RG54L++;
      }
    }
  }
  //m_26E64:
  RG3L = 0;
  RG2L = lcon(0x17);
  RG1L = 21 * signedlong(DM2_BETWEEN_VALUE(0, 23, DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0x3), 0, lcon(0xb), 0)));
  RG4L = RG1L;
  RG1L <<= bcon(0x6);
  RG1L += RG4L;
  ddat.v1e1438 = RG1L;
  DM2_savegame_3a15_0002();
  RG1Blo = ddat.v1d652d == 0 ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  DM2_weather_3df7_0037(RG1L);
  if (ddat.v1d652d != 0)
  {
    RG2L = lcon(0x400);
    RG4L = 1;
    RG1P = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(lcon(0x3f));
    ddat.v1e03f0 = RG1P;
    RG2L = lcon(0x400);
    RG4L = 1;
    RG1P = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(lcon(0x7e));
    ddat.v1e03dc = RG1P;
  }
  //m_26EF5:
  RG1P = ddat.v1e03dc;
  RG4P = ddat.v1e03f0;
  xp_0c = UPCAST(unk, RG4P);
  RG3L = 0;
  mov8(location(RG4P), 0);
  vo_14.poke16(RG3W);
  xp_0c = UPCAST(unk, RG4P + lcon(0x1));
  //m_26F6A:
  for (;;)
  {
    RG4L = signedlong(vo_14.peek16());
    if (RG4L >= lcon(0x3f))
    {
      if (vl_00 != 0)
        DM2_ARRANGE_DUNGEON();
      if (ddat.v1d652d != 0)
        DM2_savegame_2066_2498();
      ddat.v1d652d = 0;
      return lcon(0x1);
    }
    //m_26F17:
    RG4L = 0;
    //m_26F3D:
    for (;;)
    {
      RG2L = unsignedlong(RG4W);
      RG63l = signedlong(vw_10);
      RG55L = RG3L + 1;
      RG72p = RG1P + lcon(0x1);
      if (RG2L >= RG63l)
      {
        mov8(location(RG1P), lcon(0xffffffff));
        RG1P = RG72p;
        RG3L = RG55L;
        RG4P = DOWNCAST(unk, xp_0c);
        mov8(location(RG4P), RG3Blo);
        RG4P++;
        xp_0c = UPCAST(unk, RG4P);
        vo_14.inc16();
        break;
      }
      //m_26F1B:
      RG2L <<= 4;
      RG2P = ddat.v1e03c8 + RG2L;
      RG2W = word_at(RG2P, lcon(0x8)) & lcon(0x3f);
      if (RG2W == vo_14.peek16())
      {
        mov8(location(RG1P), RG4Blo);
        RG1P = RG72p;
        RG3L = RG55L;
      }
      RG4L++;
    }
  }
  FOREND
}

static i32 DM2_LOAD_NEW_DUNGEON(void)
{
  bool pass = false;

  if (ddat.savegamew1 != 0)
  {
    i16 wordrg11 = DM2_FILE_OPEN(DM2_FORMAT_SKSTR(gdat.filename3, NULL));
    ddat.savegamefilehandle1 = wordrg11;
    if (wordrg11 >= 0)
      pass = true;
  }
  if (!pass)
    ddat.savegamefilehandle1 = DM2_FILE_OPEN(DM2_FORMAT_SKSTR(gdat.filename4, NULL));
  if (ddat.savegamefilehandle1 < wcon(0x0))
    DM2_RAISE_SYSERR(0x32);
  party.heros_in_party = 0;
  ddat.savegamewpc.w_00 = lcon(0xffffffff);
  ddat.savegamel1 = 0;
  return DM2_READ_DUNGEON_STRUCTURE(1);
}

// eaxp is bytebuffer
bool DM2_SKLOAD_READ(unk* xeaxp, i16 edxw)
{
  if (edxw == 0)
    return true;
  return DM2_FILE_READ(ddat.savegamefilehandle1, xeaxp, unsignedlong(edxw));
}

union u_uwbb
{
  struct
  {
    i8 blo;
    i8 bhi;
  };
  ui16 uw_00;
};

static bool DM2_SUPPRESS_READER(unk* xeaxp, unk* xedxp, i32 ebxl, i32 ecxl, bool argflag)
{
  i8* bp_00;
  i32 vl_04;
  i32 vl_08; // inc32
  i32 vl_10; // inc32
  i8 vb_14; // one - byte - filebuffer
  i8 vb_18;

  bp_00 = xedxp;
  vl_04 = ecxl;
  i32 longrg5 = 0;
  i8 byterg11 = ddat.v1e092e;
  u_uwbb u103rg2;
  u103rg2.bhi = ddat.v1e092f;
  vl_08 = 0;

  for (;;)
  {
    if (mkul(vl_08) >= mkul(vl_04))
    {
      ddat.v1e092e = byterg11;
      ddat.v1e092f = u103rg2.bhi;
      ddat.v1e08e0 += longrg5;
      return false;
    }
    vl_10 = 0;
    u_uwbb u103rg3;
    while (mkuw(vl_10) < (u103rg3.uw_00 = mkuw(ebxl)))
    {
      if (argflag)
        u103rg3.uw_00 = unsignedword(u103rg3.blo);
      else
        u103rg3.bhi = *xeaxp;
      i8 byterg12 = *bp_00++;
      vb_18 = byterg12;
      if (byterg12 != 0)
      {
        u103rg2.blo = lcon(0x7);
        i32 longrg6 = unsignedlong(vb_18);
        while (u103rg2.blo >= 0)
        {
          u103rg3.blo = u103rg2.blo;
          if ((longrg6 & 1 << u103rg3.blo) != 0)
          {
            if (u103rg2.bhi == 0)
            {
              if (!DM2_SKLOAD_READ(ADRESSOF(i8, &vb_14), 1))
                return true;
              byterg11 = vb_14;
            }
            i8 byterg4 = byterg11 & lcon(0xffffff80);
            u103rg3.blo = u103rg2.blo;
            i8 byterg13 = 1 << u103rg3.blo;
            byterg11 += byterg11;
            if (byterg4 == 0)
              u103rg3.bhi &= ~byterg13;
            else
            {
              byterg11 |= 1;
              u103rg3.bhi |= byterg13;
            }
            longrg5++;
            u103rg2.bhi++;
            if (u103rg2.bhi == lcon(0x8))
              u103rg2.uw_00 = unsignedword(u103rg2.blo);
          }
          u103rg2.blo--;
        }
      }
      *xeaxp++ = u103rg3.bhi;
      vl_10++;
    }
    bp_00 -= unsignedlong(u103rg3.uw_00);
    vl_08++;
  }
}


// stacksize was 0x2c
static bool DM2_READ_1BIT(i32* eaxpl)
{
  i8 vb_00;

  bool boolrg4 = DM2_SUPPRESS_READER(ADRESSOF(i8, &vb_00), &ddat.savegameb1, 1, 1, true);
  *eaxpl = unsignedlong(vb_00);
  return boolrg4;
}

static i16 DM2_SELECT_LOAD_GAME(void)
{
  c_rect rc_00;
  i16 vw_08; // y0
  i16 vw_0c; // x0

  if (ddat.savegamel2)
    return ddat.savegamew4; // BUGFIX older bug
  DM2_dialog_OPEN_DIALOG_PANEL(lcon(0x80), lcon(0x5));
  DM2_dialog_2066_398a(lcon(0xffffffff));
  DM2_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
  if (ddat.dialog2 != 0)
    DM2_FADE_SCREEN(0);
  bool boolrg3 = false;
  i16 wordrg4 = lcon(0xffffffff);
  eventqueue.event_unk06 = lcon(0xff);
  do
  {
    DM2_EVENT_LOOP();
    if (eventqueue.event_unk06 < 2)
    {
      if (eventqueue.event_unk06 == 1)
      {
        wordrg4 = lcon(0xffffffff);
        boolrg3 = true;
      }
    }
    else if (eventqueue.event_unk06 == 2)
      boolrg3 = true;
    else if (eventqueue.event_unk06 == 3)
    {
      DM2_QUERY_EXPANDED_RECT(eventqueue.event_unk05, &rc_00);
      DM2_QUERY_TOPLEFT_OF_RECT(eventqueue.event_unk09, OO vw_0c, OO vw_08);
      wordrg4 = DM2_MIN((eventqueue.event_unk07 - (rc_00.y + vw_08)) / strdat.strxplus, lcon(0xa));
      if (ddat.savegamep1[wordrg4 / lcon(0x2a)].l_26 == lcon(0xdeadbeef))
        wordrg4 = lcon(0xffffffff);
      DM2_dialog_2066_398a(signedlong(wordrg4));
      DM2_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
    }

    eventqueue.event_unk06 = -1;
  } while (!boolrg3);
  DM2_dialog_2066_37f2();
  return wordrg4;
}

static void DM2_ADD_INDEX_TO_POSSESSION_INDICES(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG4W = ddat.savegamew6;
  RG3L = 2 * signedlong(RG4W);
  RG2P = DOWNCAST(i16, ddat.savegamep3);
  RG4L++;
  ddat.savegamew6 = RG4W;
  mov16(location(RG2P + RG3L), RG1W);
}

// stacksize was 0x48
// not static, because friend of c_timer
// *ebxpw can be changed
bool DM2_READ_RECORD_CHECKCODE(i32 eaxl, i16* ebxpw, i32 ecxl, i32 edxl, i32 argl0)
{
  i32 vl_00;
  c_ql32 vql_08;
  c_ql32 vql_0c;
  i32 vl_10;
  i32 vl_14;
  i32 vl_18;
  i32 vl_1c;
  i8* pb_20;
  i32 vl_24;
  i32 vl_28; // for DM2_SUPPRESS_READER()
  c_ql32 vql_2c;
  i8 vb_30; // for DM2_SUPPRESS_READER()
  i8 vb_34; // for DM2_SUPPRESS_READER()

  vql_0c.poke32(eaxl);
  vql_08.poke32(edxl);
  vl_00 = ecxl;

  for (;;)
  {
    i32 longrg4;
    if (DM2_READ_1BIT(&vl_24))
      return true;
    if (vl_24 == 0)
      return false;
    vb_34 = lcon(0xf);
    if (DM2_SUPPRESS_READER(ADRESSOF(i8, &vb_30), ADRESSOF(i8, &vb_34), 1, 1, true))
      return true;
    ui16 uwordrg6 = unsignedword(vb_30);
    if (vl_00 == 0) goto M_2586C;
    if (uwordrg6 == lcon(0x4))
    {
    M_2586C:
      longrg4 = 0;
    }
    else
    {
      vb_34 = 3;
      if (DM2_SUPPRESS_READER(ADRESSOF(i8, &vb_30), ADRESSOF(i8, &vb_34), 1, 1, true))
        return true;
      longrg4 = unsignedlong(vb_30);
    }
    if (unsignedlong(uwordrg6) == ulcon(0xf) && ddat.v1d6525 != 0)
    {
      vl_28 = lcon(0x7f);
      if (DM2_SUPPRESS_READER(ADRESSOF(i32, &vl_24), ADRESSOF(i32, &vl_28), 2, 1, true))
        return true;
      if (ebxpw == NULL)
        return false;
      *ebxpw = CUTX16(vl_24) | wcon(0xff80);
      return false;
    }
    vl_14 = unsignedlong(uwordrg6);
    longrg4 <<= bcon(0xe);
    vql_2c.poke32(DM2_ALLOC_NEW_RECORD(vl_14) & lcon(0xffff3fff) | longrg4);
    vl_1c = unsignedlong(vql_2c.peek16());
    DM2_APPEND_RECORD_TO(mk_record(vl_1c), UPCAST(t_record, DOWNCAST(i16, ebxpw)), vql_0c.peek16(), vql_08.peek16());
    s_rec* s100prg5 = XUPCAST(s_rec, DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(CUTX16(vl_1c))))));
    pb_20 = table1d64db[vl_14];
    if (pb_20 != NULL)
    {
      vl_18 = 0;
      if (uwordrg6 < mkuw(lcon(0x9)))
      {
        if (uwordrg6 == lcon(0x4))
        {
          vb_34 = lcon(0x7f);
          if (DM2_SUPPRESS_READER(ADRESSOF(i8, &vb_30), ADRESSOF(i8, &vb_34), 1, 1, true))
            return true;
          s100prg5->b_04 = vb_30;
          if ((DM2_QUERY_CREATURE_AI_SPEC_FLAGS(vl_1c) & 0x1) != 0)
            pb_20 = v1d648f;
        }
      }
      else if (uwordrg6 <= mkuw(lcon(0x9)))
      {
        vb_34 = 3;
        if (DM2_SUPPRESS_READER(ADRESSOF(i8, &vb_30), ADRESSOF(i8, &vb_34), 1, 1, true))
          return true;
        s100prg5->b_04 &= lcon(0xfffffff9);
        s100prg5->uw_04 |= mkuw(2 * vb_30) & uwcon(0x3);
        if (DM2_IS_CONTAINER_MAP(vl_1c) != 0)
        {
          pb_20 = v1d64b7;
          vl_18 = 1;
        }
      }
      else if (uwordrg6 == uwcon(0xe))
      {
        if (ddat.v1d6529 != 0)
        {
          pb_20 = v1d64d3;
          vl_18 = 1;
        }
      }

      if (DM2_SUPPRESS_READER(DOWNCAST(s_rec, s100prg5), pb_20, unsignedlong(table_recordsizes[unsignedlong(uwordrg6)]), 1, false))
        return true;
      if (uwordrg6 < mkuw(lcon(0x9)))
      {
        if (uwordrg6 == lcon(0x4))
        {
          ddat.v1d6529 = 1;
          s100prg5->uw_02 = lcon(0xfffffffe);
          if (DM2_READ_RECORD_CHECKCODE(lcon(0xffffffff), CHGCAST(i16, &s100prg5->uw_02), pb_20 != v1d647f ? 1 : 0, 0, 1))
            return true;
          ddat.v1d6529 = 0;
        }
      }
      else if (uwordrg6 <= mkuw(lcon(0x9)))
      {
        if (vl_18 == 0)
        {
          if (DM2_IS_CONTAINER_MONEYBOX(mkl(vql_2c.peek16())) != 0)
            table1d64db[0xa] = v1d64c3;
          s100prg5->uw_02 = lcon(0xfffffffe);
          if (DM2_READ_RECORD_CHECKCODE(lcon(0xffffffff), CHGCAST(i16, &s100prg5->uw_02), 0, 0, 1))
            return true;
          if (DM2_IS_CONTAINER_MONEYBOX(mkl(vql_2c.peek16())) != 0)
            table1d64db[0xa] = v1d64bf;
        }
        else
        {
          if (DM2_READ_1BIT(&vl_10))
            return true;
          if (vl_10 == 0)
            s100prg5->uw_02 = lcon(0xfffffffe);
          else
            DM2_ADD_INDEX_TO_POSSESSION_INDICES(mkl(vql_2c.peek16()));
        }
      }
      else if (uwordrg6 < mkuw(lcon(0xe)))
        ;
      else if (uwordrg6 <= mkuw(lcon(0xe)))
      {
        if (vl_18 != 0)
          DM2_ADD_INDEX_TO_POSSESSION_INDICES(mkl(vql_2c.peek16()));
        else
        {
          timdat.timerarray[unsignedlong(s100prg5->w_06)].setA(vql_2c.peek16());
          s100prg5->uw_02 = lcon(0xfffffffe);
          ddat.v1d6525 = 1;
          if (DM2_READ_RECORD_CHECKCODE(lcon(0xffffffff), CHGCAST(i16, &s100prg5->uw_02), vl_18, 0, vl_18))
            return true;
          ddat.v1d6525 = 0;
        }
      }
      else if (uwordrg6 == uwcon(0xf))
      {
        if (DM2_READ_1BIT(&vl_24))
          return true;
        if (vl_24 != 0)
        {
          vl_28 = lcon(0x3ff);
          if (DM2_SUPPRESS_READER(ADRESSOF(i32, &vl_24), ADRESSOF(i32, &vl_28), 2, 1, true))
            return true;
          timdat.timerarray[vl_24].setB(vql_2c.peek16());
        }
      }
    }

    if (argl0 == 0)
      return false;
  }
}

static bool DM2_2066_197c(void)
{
  c_tim* c38prg5 = timdat.timerarray;
  i16 wordrg6 = 0;
  for (;;)
  {
    if (mkul(signedlong(wordrg6)) >= unsignedlong(timdat.num_timers))
      return false;
    if (c38prg5->is_type(0x3c) || c38prg5->is_type(0x3d))
    {
      if (ddat.savegamew7 == 0)
        return true;
      c38prg5->setB(0xfffe);
      if (DM2_READ_RECORD_CHECKCODE(lcon(0xffffffff), &c38prg5->wvalueB, 0, 0, 0))
        return true;
    }
    c38prg5++;
    wordrg6++;
  }
}

struct s_ww
{
  i16 w_00; // unused ?!? could be two bytes
  i16 w_02;
};

static bool DM2_2066_062b(void)
{
  i32 vl_00; // TODO adress goes to DM2_SUPPRESS_READER, size unknown yet
  i32 vl_04;
  i32 vl_08; // TODO adress goes to DM2_SUPPRESS_READER, size unknown yet

  vl_04 = mkl(ddat.savegamew6);
  i16* wptrrg6 = ddat.savegamep3;
  for (;;)
  {
    i32 longrg4 = vl_04 - 1;
    vl_04 = longrg4;
    if (CUTX16(longrg4) == lcon(0xffffffff))
      return false;
    i16 wordrg4 = *wptrrg6++;
    s_ww* s102prg5 = XUPCAST(s_ww, DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(wordrg4)))));
    wordrg4 &= lcon(0x3c00);
    ui32 ulongrg1 = unsignedlong(wordrg4) >> bcon(0xa);
    if (ulongrg1 < mkul(lcon(0x9)))
      ;
    else if (ulongrg1 <= mkul(lcon(0x9)))
    {
      vl_08 = lcon(0x3ff);
      if (DM2_SUPPRESS_READER(ADRESSOF(i32, &vl_00), ADRESSOF(i32, &vl_08), 2, 1, true))
        return true;
      s102prg5->w_02 = CUTX16(vl_00) | wcon(0x1000);
    }
    else if (ulongrg1 == ulcon(0xe))
    {
      vl_08 = lcon(0x3ff);
      if (DM2_SUPPRESS_READER(ADRESSOF(i32, &vl_00), ADRESSOF(i32, &vl_08), 2, 1, true))
        return true;
      s102prg5->w_02 = CUTX16(vl_00) | wcon(0x2400);
    }
  }
}

// was SKW_2066_2b6c
static void DM2_PROCEED_GLOBAL_EFFECT_TIMERS(void)
{
  i32 vl_00;

  c_tim* c38prg5 = timdat.timerarray;
  DM2_ZERO_MEMORY(&ddat.savegames1, lcon(0x6));
  vl_00 = 0;
  for (;;)
  {
    if (CUTX16(vl_00) >= timdat.num_timer_indices)
    {
      i16 wordrg61 = 0;
      while (mkul(signedlong(wordrg61)) < unsignedlong(party.heros_in_party))
      {
        party.calc_player_weight((e_hero)wordrg61);
        wordrg61++;
      }
      return;
    }
    i16 wordrg62 = unsignedword(c38prg5->getactor());
    ui8 ubyterg1 = c38prg5->gettype();
    if (ubyterg1 < ubcon(0x47))
    {
      if (ubyterg1 < ubcon(0xe))
        ;
      else if (ubyterg1 == ubcon(0xe))
        DM2_PROCESS_TIMER_0E(c38prg5, lcon(0x3));
      else if (ubyterg1 == ubcon(0x46))
      {
        i16 wordrg4 = c38prg5->getA();
        i32 longrg1;
        if (wordrg4 != wcon(0x0) && (longrg1 = signedlong(wordrg4)) >= lcon(0xfffffff1) && longrg1 <= lcon(0xf))
        {
          if (wordrg4 >= 0)
            ddat.savegames1.w_00 -= 2 * signedword(table1d6702[longrg1]);
          else
            ddat.savegames1.w_00 += signedword(table1d6702[-longrg1]); // it's okay, longrg1 is negative here
        }
      }
    }
    else if (ubyterg1 <= ubcon(0x47))
      ddat.savegames1.b_02++;
    else if (ubyterg1 <= ubcon(0x48))
    {
      i16 wordrg41 = 0;
      i32 longrg2;
      while (mkul(longrg2 = signedlong(wordrg41)) < unsignedlong(party.heros_in_party))
      {
        if ((1 << wordrg41 & signedlong(wordrg62)) != 0)
        {
          if (party.hero[longrg2].curHP != 0)
            party.hero[longrg2].ench_power += c38prg5->getA();
        }
        wordrg41++;
      }
    }
    else if (ubyterg1 == ubcon(0x4b))
    {
      party.hero[signedlong(wordrg62)].poisoned++;
      party.hero[signedlong(wordrg62)].poison += c38prg5->getA();
    }

    c38prg5++;
    vl_00++;
  }
}

static i32 DM2_READ_SKSAVE_DUNGEON(void)
{
  i8* bprg7;
  c_5bytes c12_00;
  t_tile* tipub_08;
  i32 vl_0c; // but adress given to DM2_SUPPRESS_READER as unk*
  i32 vl_10; // but adress given to DM2_SUPPRESS_READER as unk*
  c_o32 vo_14; // poke 16 peek 32 inc16
  c_o32 vo_18; // poke 16 peek 32 inc16
  i16 vw_1c;
  c_o32 vo_20; // poke 16 peek 32 inc16
  i16 vw_24;
  c_o32 vo_28; // poke 16 peek 16 32 inc16
  i8 vb_2c;

  vo_20.poke16(0);

  while (mkul(signedlong(vo_20.peek16())) < unsignedlong(party.heros_in_party))
  {
    i16* wptrrg55 = party.hero[signedlong(vo_20.peek16())].item;
    i16 wordrg66 = 0;
    while (wordrg66 < wcon(0x1e))
    {
      *wptrrg55++ = lcon(0xfffffffe);
      wordrg66++;
    }
    vo_20.inc16();
  }
  ddat.savegamewpc.w_00 = lcon(0xfffffffe);
  vw_24 = ddat.v1d3248;
  vo_28.poke16(0);

  while (unsignedlong(vo_28.peek16()) < unsignedlong(CUTX8(ddat.savegamep4->warr_00[2])))
  {
    DM2_CHANGE_CURRENT_MAP_TO(signedlong(vo_28.peek16()));
    vo_14.poke16(0);

    while (vo_14.peek16() < mapdat.map_width)
    {
      vo_18.poke16(0);

      while (vo_18.peek16() < mapdat.map_height)
      {
        i16 wordrg17 = record_to_word(DM2_GET_TILE_RECORD_LINK(vo_14.peek16(), vo_18.peek16()));
      M_25CF2:
        i16 wordrg65;
        if ((wordrg65 = wordrg17) == lcon(0xfffffffe)) goto M_25D44;
        if (((wordrg65 & wcon(0x3c00)) >> bcon(0xa)) <= wcon(0x3))
        {
          wordrg17 = mkuw(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(wordrg65))));
          goto M_25CF2;
        }
        *XUPCAST(i16, DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(wordrg65))))) = wcon(0xfffe);
        DM2_CUT_RECORD_FROM(mk_record(unsignedlong(wordrg65)), NULL, vo_14.peek16(), vo_18.peek16());
      M_25D44:
        vo_18.inc16();
      }
      vo_14.inc16();
    }
    vo_28.inc16();
  }
  DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_24));
  vo_20.poke16(lcon(0x4));
  while (signedlong(vo_20.peek16()) < lcon(0x10))
  {
    i16* wptrrg51 = UPCAST(i16, DOWNCAST(t_record, recordptr[signedlong(vo_20.peek16())]));
    i16 wordrg41 = unsignedword(table_recordsizes[signedlong(vo_20.peek16())]) / 2;
    i16 wordrg64 = ddat.savegamep4->warr_00[signedlong(vo_20.peek16()) + lcon(0x6)];
    while (--wordrg64 != wcon(0xffff))
    {
      *wptrrg51 = lcon(0xffffffff);
      wptrrg51 += signedlong(wordrg41);
    }
    vo_20.inc16();
  }
  ddat.savegamep3 = XUPCAST(i16, dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0xc8)));
  ddat.savegamew6 = 0;
  vo_20.poke16(0);
  while (mkul(signedlong(vo_20.peek16())) < unsignedlong(party.heros_in_party))
  {
    i16* wptrrg52 = party.hero[signedlong(vo_20.peek16())].item;
    i16 wordrg63 = 0;
    while (wordrg63 < wcon(0x1e))
    {
      i16* wptrrg2 = wptrrg52++;
      if (DM2_READ_RECORD_CHECKCODE(lcon(0xffffffff), wptrrg2, 0, 0, 0))
        goto M_errexit;
      wordrg63++;
    }
    vo_20.inc16();
  }
  if (DM2_READ_RECORD_CHECKCODE(lcon(0xffffffff), &ddat.savegamewpc.w_00, 0, 0, 0))
    goto M_errexit;
  vo_20.poke16(0);

  while (mkul(signedlong(vo_20.peek16())) < unsignedlong(party.heros_in_party))
  {
    i16* wptrrg53 = party.hero[signedlong(vo_20.peek16())].item;
    i16 wordrg62 = 0;
    while (wordrg62 < wcon(0x1e))
    {
      if (*wptrrg53 != lcon(0xfffffffe))
      {
        i32 longrg41 = unsignedlong(*wptrrg53);
        DM2_PROCESS_ITEM_BONUS(signedlong(vo_20.peek16()), longrg41, signedlong(wordrg62), 0);
      }
      else
        *wptrrg53 = lcon(0xffffffff);
      wordrg62++;
      wptrrg53++;
    }
    vo_20.inc16();
  }
  if (ddat.savegamewpc.w_00 != lcon(0xfffffffe))
    DM2_PROCESS_ITEM_BONUS(signedlong(eventqueue.event_heroidx), unsignedlong(ddat.savegamewpc.w_00), lcon(0xffffffff), 0);
  else
    ddat.savegamewpc.w_00 = lcon(0xffffffff);

  if (DM2_2066_197c())
    goto M_errexit;
  vw_24 = ddat.v1d3248;
  vo_28.poke16(0);

  while (mkuw(vo_28.peek16()) < unsignedword(CUTX8(ddat.savegamep4->warr_00[2])))
  {
    DM2_CHANGE_CURRENT_MAP_TO(signedlong(vo_28.peek16()));
    tipub_08 = *mapdat.map;
    vo_14.poke16(0);
    while (vo_14.peek16() < mapdat.map_width)
    {
      vo_18.poke16(0);
      while (vo_18.peek16() < mapdat.map_height)
      {
        bool boolrg5 = false;
        ui8 ubyterg1 = *tipub_08 >> 5;
        if (ubyterg1 <= ubcon(0x7))
        {
          switch (ubyterg1)
          {
            case 0:
            case 1:
            case 3:
            case 7:
              vb_2c = 0;
            break;
            case 2:
              vb_2c = lcon(0x8);
            break;
            case 4:
              vb_2c = lcon(0x7);
            break;
            case 5:
              if (DM2_GET_TELEPORTER_DETAIL(&c12_00, signedlong(vo_14.peek16()), signedlong(vo_18.peek16())) == 0)
                vb_2c = lcon(0x8);
              else
              {
                vb_2c = 0;
                if (unsignedlong(vo_28.peek16()) > unsignedlong(c12_00.b_04))
                  boolrg5 = true;
              }
            break;
            case 6:
              vb_2c = 4;
            break;
            default: throw(THROW_DMABORT);
          }
        }
        if (vb_2c != 0)
        {
          if (DM2_SUPPRESS_READER(DOWNCAST(t_tile, tipub_08), ADRESSOF(i8, &vb_2c), 1, 1, false))
            goto M_errexit;
        }
        tipub_08++;
        if (!boolrg5)
        {
          i16 wordrg13 = record_to_word(DM2_GET_TILE_RECORD_LINK(vo_14.peek16(), vo_18.peek16()));
          for (;;)
          {
            i16 wordrg61 = wordrg13;
            if (wordrg61 == lcon(0xfffffffe))
            {
              if (DM2_READ_RECORD_CHECKCODE(signedlong(vo_14.peek16()), NULL, 1, signedlong(vo_18.peek16()), 1))
                goto M_errexit;
              break;
            }
            i16 wordrg12 = (wordrg61 & wcon(0x3c00)) >> bcon(0xa);
            vw_1c = wordrg12;
            i32 longrg42 = unsignedlong(wordrg12);
            bprg7 = table1d64db[longrg42];
            if (bprg7 != NULL)
            {
              s_ww* s102prg1 = XUPCAST(s_ww, DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(wordrg61)))));
              s_ww* s102prg54 = s102prg1;
              if (longrg42 == lcon(0x3))
              {
                i16 wordrg11 = s102prg1->w_02 & lcon(0x7f);
                if (   wordrg11 == wcon(0x27)
                    || wordrg11 == wcon(0x1b)
                    || wordrg11 == wcon(0x1d)
                    || wordrg11 == wcon(0x41)
                    || wordrg11 == wcon(0x2c)
                    || wordrg11 == wcon(0x32)
                    || wordrg11 == wcon(0x30)
                    || wordrg11 == wcon(0x2d)
                   )
                {
                  vl_10 = lcon(0x1ff);
                  if (DM2_SUPPRESS_READER(ADRESSOF(i32, &vl_0c), ADRESSOF(i32, &vl_10), 2, 1, true))
                    goto M_errexit;
                  s102prg54->w_02 = s102prg54->w_02 & wcon(0x7f) | CUTX16((vl_0c & lcon(0x1ff)) << 7);
                }
              }
              if (DM2_SUPPRESS_READER(DOWNCAST(s_ww, s102prg54), bprg7, unsignedlong(table_recordsizes[unsignedlong(vw_1c)]), 1, false))
                goto M_errexit;
            }
            wordrg13 = mkuw(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(wordrg61))));
          }
        }
        vo_18.inc16();
      }
      vo_14.inc16();
    }
    vo_28.inc16();
  }
  DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_24));
  if (DM2_2066_062b())
    goto M_errexit;
  vo_20.poke16(0);
  dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0xc8));
  return signedlong(vo_20.peek16());

M_errexit:
  vo_20.poke16(1);
  dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0xc8));
  return signedlong(vo_20.peek16());
}

static void DM2_SUPPRESS_INIT(void)
{
  ddat.v1e092f = 0;
  ddat.v1e092e = 0;
}

static void DM2_3a15_020f(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;

  RG4L = 0;
  //m_4601E:
  for (;;)
  {
    RG2L = signedlong(RG4W);
    if (RG2L >= lcon(0x4))
      break;
    //m_46008:
    RG1L = 263 * RG2L;
    party.hero[RG2L].timeridx = -1;
    RG4L++;
  }
  RG5w = timdat.num_timers;
  if (RG5w == 0)
    return;
  RG4L = 0;
  c_tim* RG2timp = timdat.timerarray;
  //m_4607C:
  for (;;)
  {
    RG1L = signedlong(RG4W);
    RG3L = unsignedlong(RG5w);
    if (RG1L >= RG3L)
      return;
    //m_4603C:
    RG1Blo = RG2timp->gettype();
    if (mkub(RG1Blo) >= mkub(lcon(0xc)))
    {
      if (mkub(RG1Blo) <= mkub(lcon(0xc)))
      {
        //m_4604F:
        RG3L = unsignedlong(RG2timp->getactor());
        RG1L = 263 * RG3L;
        party.hero[RG3L].timeridx = RG4W;
      }
      else
      {
        if (mkub(RG1Blo) >= mkub(lcon(0x1d)))
        {
          if (mkub(RG1Blo) <= mkub(lcon(0x1e)))
          {
            //m_46069:
            RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(RG2timp->getA())));
            mov16(location(RG1P + lcon(0x6)), RG4W);
          }
        }
      }
    }
    //m_46078:
    RG4L++;
    RG2P += lcon(0xc);
  }
  FOREND
}

// some stuff removed here, because fix ddat.v1d154e now
i16 DM2_GAME_LOAD(void)
{
  s_savegamebuffer s33_00;
  s_hex30 s29_3c;
  bool bypass = false;
  bool dontload = false;
  bool boolrg6;

  i16 wordrg11 = 0;

  s29_3c.l_2c = 0;
  bool boolrg5 = false;
  i8 byterg2;
_DEBUG_SHOW_BIGPOOL();
  if ((byterg2 = CUTX8(ddat.cd.mo.glbSpecialScreen)) == _MENU_SCREEN__RESUME_GAME_SELECT)
  {
    ddat.v1e0992 = 2;
    i16 wordrg14 = DM2_SELECT_LOAD_GAME();
    i8 byterg4 = CUTX8(wordrg14);
    if (wordrg14 < 0)
    {
      ddat.savegamew4 = 0;
      return -1;
    }
    ddat.savegamew4 = wordrg14;
    byterg4 += lcon(0x30);
    ddat.v1e099c = byterg4;
    boolrg6 = false;
    boolrg5 = true;
    i16 wordrg13 = DM2_FILE_OPEN(DM2_FORMAT_SKSTR(gdat.filename5, NULL));
    ddat.savegamefilehandle1 = wordrg13;
    if (wordrg13 < 0)
    {
      i16 wordrg12 = DM2_FILE_OPEN(DM2_FORMAT_SKSTR(gdat.filename6, NULL));
      ddat.savegamefilehandle1 = wordrg12;
      if (wordrg12 < 0)
      {
        if (ddat.savegamel2)
          goto M_27330;
        wordrg11 = DM2_FILE_OPEN(DM2_FORMAT_SKSTR(gdat.filename7, NULL));
        ddat.savegamefilehandle1 = wordrg11;
        if (wordrg11 < 0)
        {
        M_27330:
          boolrg5 = false;
          byterg2 = lcon(0x1c);
        }
        else
        {
          ddat.cd.mo.glbSpecialScreen = _MENU_SCREEN__LOAD_NEW_GAME;
          dontload = true; //goto M_2719F;
        }
      }
      else
        boolrg6 = boolrg5;
    }

    if (!dontload)
    {
      party.heros_in_party = 0;
      ddat.savegamewpc.w_00 = lcon(0xffffffff);
      DM2_SKLOAD_READ(ADRESSOF(s_hex30, &s29_3c), lcon(0x2a));
      ddat.savegamew7 = s29_3c.c27_00.w_00;
      ddat.savegamel1 = lcon(0x2a);
      ddat.cd.mo.glbSpecialScreen = !ddat.savegamel2 ? _MENU_SCREEN__LOAD_NEW_GAME : _MENU_SCREEN__RESUME_GAME_SELECT;
      if (DM2_READ_DUNGEON_STRUCTURE(0) == 0) goto M_exit;
      ddat.cd.mo.glbSpecialScreen = _MENU_SCREEN__RESUME_GAME_SELECT;
      DM2_SUPPRESS_INIT();
      if (DM2_SUPPRESS_READER(ADRESSOF(s_savegamebuffer, &s33_00), table1d631a, sizeof(s_savegamebuffer), 1, true)) goto M_exit;
      timdat.gametick = s33_00.l_00;
      ddat.v1e021c = s33_00.l_00;
      randdat.random = s33_00.ul_04;
      party.heros_in_party = s33_00.w_08;
      ddat.v1e0270 = s33_00.w_0a;
      ddat.v1e0272 = s33_00.w_0c;
      ddat.v1e0258 = s33_00.w_0e;
      ddat.v1e0266 = s33_00.w_10;
      eventqueue.event_heroidx = (e_hero)s33_00.w_12;
      timdat.num_timers = s33_00.w_14;
      ddat.v1d26a4 = s33_00.l_16;
      ddat.v1e01a0 = s33_00.l_1a;
      ddat.v1e026e = s33_00.w_1e;
      ddat.v1e025e = s33_00.w_20;
      ddat.v1e0274 = s33_00.w_22;
      ddat.v1d26a0 = s33_00.w_28 & lcon(0xf);
      ddat.v1d26a2 = s33_00.w_28 << bcon(0x10) >> bcon(0x14) & lcon(0xf);
      ddat.v1e147f = mkb(s33_00.l_2a);
      ddat.v1e1480 = s33_00.b_2e;
      ddat.v1e1483 = s33_00.b_2f;
      ddat.v1e1482 = s33_00.b_30;
      ddat.v1e147e = s33_00.b_31;
      ddat.v1e147d = s33_00.b_32;
      ddat.v1e1484 = s33_00.b_33;
      ddat.v1e1474 = s33_00.w_34;
      ddat.v1e147b = s33_00.b_36;
      ddat.v1e1478 = s33_00.b_37;
      ddat.v1e1434 = s33_00.l_38;
      if (DM2_SUPPRESS_READER(ddat.v1e0104, ddat.v1d6316, 1, lcon(0x8), true)) goto M_exit;
      if (DM2_SUPPRESS_READER(ddat.globalb, ddat.v1d6316, 1, lcon(0x40), true)) goto M_exit;
      if (DM2_SUPPRESS_READER(DOWNCAST(i16, ddat.globalw), ddat.v1d6316, 2, lcon(0x40), true)) goto M_exit;
      if (DM2_SUPPRESS_READER(DOWNCAST(c_hero, party.hero), table1d6356, sizeof(c_hero), unsignedlong(party.heros_in_party), true)) goto M_exit;
      if (DM2_SUPPRESS_READER(DOWNCAST(c_wbbb, &ddat.savegames1), table1d645d, sizeof(c_wbbb), 1, true)) goto M_exit;
      if (DM2_SUPPRESS_READER(DOWNCAST(c_tim, timdat.timerarray), v1d6463, sizeof(c_tim), unsignedlong(timdat.num_timers), true)) goto M_exit;

      i16 wordrg4 = timdat.num_timers;
      while (wordrg4 < timdat.max_timers)
      {
        timdat.timerarray[signedlong(wordrg4)].clrtype();
        wordrg4++;
      }
      DM2_SORT_TIMERS();
      if (DM2_READ_SKSAVE_DUNGEON() != 0) goto M_exit;
      DM2_PROCEED_GLOBAL_EFFECT_TIMERS();
      DM2_3a15_020f();
      ddat.savegamel2 = true;
      bypass = true;  // goto M_27641;
      // at this point another path removed, because ddat.v1d154e != 0 ever true
    }
  }

_DEBUG_SHOW_BIGPOOL();
//m_2719F:
  if (!bypass)
  {
    boolrg5 = true;
    s29_3c.l_2c = 0; // SKW_2066_03e0 returns always 1 now
    if (DM2_LOAD_NEW_DUNGEON() == 0) goto M_exit;
  }
_DEBUG_SHOW_BIGPOOL();
// M_27641:
  DM2_FILE_CLOSE(ddat.savegamefilehandle1);
  DM2_PROCESS_ACTUATOR_TICK_GENERATOR();
  gdat.filetype1 = false;
  i32 longrg1;
  if (ddat.cd.mo.glbSpecialScreen == _MENU_SCREEN__RESUME_GAME_SELECT || ddat.savegamel2)
  {
    if (boolrg6) // TODO: uninitialized possible? (yes! restart after party died)
      DM2_FILE_RENAME(DM2_FORMAT_SKSTR(gdat.filename6, UPCAST(t_text, NULL)), DM2_FORMAT_SKSTR(gdat.filename5, UPCAST(t_text, NULL)));
    longrg1 = 1;
  }
  else
  {
    if (mkw(s29_3c.l_2c) != 0)
      DM2_WAIT_SCREEN_REFRESH();
    longrg1 = 0;
  }
  DM2_0aaf_02f8(lcon(0xe), 0);
  dm2_dballochandler.v1e0200 = true;
  ddat.v1e0250 = 0;
  DM2_move_2fcf_0b8b(signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), unsignedlong(ddat.v1e0266));
  ddat.v1e0390.l_00 = 3;
  return 1;

M_exit:
  if (boolrg5)
    DM2_FILE_CLOSE(ddat.savegamefilehandle1);
  i8 byterg1;
  if (ddat.savegamel2)
  {
    if (!gdat.filetype2)
      byterg1 = lcon(0x7);
    else
      byterg1 = (!gdat.filetype1 ? 1 : 0) + lcon(0x13);
  }
  else
    byterg1 = 0;
  DM2_0aaf_0067(DM2_0aaf_02f8(unsignedlong(byterg1), lcon(0x1f)) & lcon(0xff));
  if (!ddat.savegamel2)
    DM2_PREPARE_EXIT();
  return -1;
}

static bool DM2_SKSAVE_WRITE(unk* xeaxp, i16 edxw)
{
  if (edxw == 0)
    return true;
  return DM2_FILE_WRITE(ddat.savegamefilehandle1, xeaxp, unsignedlong(edxw));
}

// eaxp and edxp are byte(array)pointers, whatever the original data type is
// routine works like a bytestream
static bool DM2_SUPPRESS_WRITER(unk* xeaxp, unk* xedxp, i32 ebxl, i32 ecxl)
{
  i32 vl_00;
  i32 vl_04; // inc
  i32 vl_08; // inc
  i8 vb_10;
  i8 vba_14[0x4]; // TODO adress used, maybe 4 bytes, maybe 1 only

  vl_00 = ecxl;
  i32 longrg5 = 0;
  i8 byterg11 = ddat.v1e092e;
  i8 byterg12 = ddat.v1e092f;
  vl_04 = 0;
  for (;;)
  {
    if (mkul(vl_04) >= mkul(vl_00))
    {
      ddat.v1e092e = byterg11;
      ddat.v1e092f = byterg12;
      ddat.v1e08e0 += longrg5;
      return false;
    }
    vl_08 = 0;
    u_uwbb u103rg2;
    while (mkuw(vl_08) < (u103rg2.uw_00 = mkuw(ebxl)))
    {
      vb_10 = *xeaxp++;
      u103rg2.bhi = *xedxp++;
      if (u103rg2.bhi != 0)
      {
        u103rg2.blo = lcon(0x7);
        while (u103rg2.blo >= 0)
        {
          i32 longrg3 = 1 << u103rg2.blo;
          if ((unsignedlong(u103rg2.bhi) & mkul(longrg3)) != 0)
          {
            byterg11 += byterg11;
            if ((unsignedlong(vb_10) & mkul(longrg3)) != 0)
              byterg11 |= 1;
            longrg5++;
            byterg12++;
            if (byterg12 == lcon(0x8))
            {
              vba_14[0x0] = byterg11;
              if (!DM2_SKSAVE_WRITE(ADRESSOF(i8, vba_14), 1))
                return true;
              byterg12 = 0;
              byterg11 = 0;
            }
          }
          u103rg2.blo--;
        }
      }
      vl_08++;
    }
    xedxp -= unsignedlong(u103rg2.uw_00);
    vl_04++;
  }
}

// TODO: parameter can be a byte/bool only, because only one bit used
// TODO: make bool return
static bool DM2_WRITE_1BIT(i32 eaxl)
{
  i8 vb_00 = eaxl & 1;
  return DM2_SUPPRESS_WRITER(&vb_00, &ddat.v1d651b, 1, 1);
}

static bool DM2_SUPPRESS_FLUSH(void)
{
  i8 vb_00;

  i8 byterg3 = ddat.v1e092f;
  if (byterg3 == 0)
    return false;
  i8 byterg11 = bcon(0x8) - byterg3;
  i8 byterg12 = ddat.v1e092e;
  while (--byterg11 != bcon(0xff))
    byterg12 = 2 * byterg12 | ((byterg12 & bcon(0x80)) != bcon(0x0) ? 1 : 0);
  vb_00 = byterg12;
  if (!DM2_SKSAVE_WRITE(ADRESSOF(i8, &vb_00), 1))
    return true;
  ddat.v1e092f = 0;
  ddat.v1e092e = 0;
  ddat.v1e08e0 += unsignedlong(byterg3);
  return false;
}

static bool DM2_WRITE_POSSESSION_INDICES(void)
{
  i32 vl_00; // TODO could be a word only
  i32 vl_04; // TODO could be a word only

  i16 wordrg6 = ddat.savegamew6;
  i16* wptrrg5 = ddat.savegamep3;
  do
  {
    if (--wordrg6 == lcon(0xffffffff))
      return false;
    i16 wordrg4 = *wptrrg5++;
    s_ww* s102prg1 = XUPCAST(s_ww, DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(wordrg4)))));
    wordrg4 &= lcon(0x3c00);
    i32 longrg43 = unsignedlong(wordrg4) >> bcon(0xa);
    i16 wordrg19;
    if (longrg43 < lcon(0x9))
      continue;
    else if (longrg43 <= lcon(0x9))
      wordrg19 = ddat.v1e08e4[s102prg1->w_02 & wcon(0x3ff)];
    else if (longrg43 == lcon(0xe))
      wordrg19 = ddat.v1e08f0[s102prg1->w_02 & wcon(0x3ff)];
    else
      continue;

    vl_00 = mkl(wordrg19);
    vl_04 = lcon(0x3ff);
  } while (!DM2_SUPPRESS_WRITER(ADRESSOF(i32, &vl_00), ADRESSOF(i32, &vl_04), 2, 1));
  return true;
}

static void DM2_COMPACT_TIMERLIST(void)
{
  i16 wordrg2 = 0;
  i16 wordrg3 = timdat.num_timers;
  if (wordrg3 == 0)
    return;
  c_tim* c38prg4 = timdat.timerarray;
  do
  {
    if (c38prg4->is_notype())
    {
      c_tim* c38prg1 = c38prg4 + 1; // next struct
      while (c38prg1->is_notype())
        c38prg1++;
      *c38prg4 = *c38prg1;
      c38prg1->clrtype();
    }
    c38prg4++;
    wordrg2++;
  } while (wordrg2 != wordrg3);
  DM2_SORT_TIMERS();
  DM2_3a15_020f();
}

static bool DM2_WRITE_RECORD_CHECKCODE(i16 eaxw, i32 edxl, i32 ebxl)
{
  i8* bprg7;
  i32 vl_00;
  i32 vl_04;
  i32 vl_08;
  i32 vl_0c;
  i32 vl_10; // but adress given as unk* to DM2_SUPPRESS_WRITER()
  i16 vw_14;
  c_ol32 vol_18; // but adress given as unk* to DM2_SUPPRESS_WRITER, one time truncated, inc32
  i8 vba_1c[0x4]; // array because adr given to DM2_SUPPRESS_WRITER, probably one byte only
  i8 vba_20[0x4]; // array because adr given to DM2_SUPPRESS_WRITER, probably one byte only

  vl_0c = edxl;
  vl_00 = ebxl;
  while (eaxw != wcon(0xfffe) && eaxw != wcon(0xffff))
  {
    ui16 uwordrg1 = (eaxw & wcon(0x3c00)) >> bcon(0xa);
    vw_14 = uwordrg1;
    i16 wordrg6 = uwordrg1;
    if (wordrg6 > wcon(0x3))
    {
      if (DM2_WRITE_1BIT(1))
        return true;
      vba_1c[0x0] = CUTX8(vw_14);
      vba_20[0x0] = lcon(0xf);
      if (DM2_SUPPRESS_WRITER(ADRESSOF(i8, vba_1c), ADRESSOF(i8, vba_20), 1, 1))
        return true;
      if (vl_0c != 0 && wordrg6 != wcon(0x4))
      {
        vba_1c[0x0] = CUTX8(eaxw >> bcon(0xe));
        vba_20[0x0] = 3;
        if (DM2_SUPPRESS_WRITER(ADRESSOF(i8, vba_1c), ADRESSOF(i8, vba_20), 1, 1))
          return true;
      }
    }
    if (vw_14 == wcon(0xf) && ddat.v1d651d != 0)
    {
      vol_18.poke16(eaxw);
      vl_10 = lcon(0x7f);
      if (DM2_SUPPRESS_WRITER(ADRESSOF(c_ol32, &vol_18), ADRESSOF(i32, &vl_10), 2, 1))
        return true;
      break;
    }
    bprg7 = table1d64db[unsignedlong(vw_14)];
    if (bprg7 != NULL)
    {
      vl_04 = unsignedlong(eaxw);
      s_rec* s100prg1 = XUPCAST(s_rec, DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(CUTX16(vl_04))))));
      s_rec* s100prg6 = s100prg1;
      vl_08 = 0;
      ui16 uwordrg42 = vw_14;
      if (uwordrg42 < uwcon(0x4))
      {
        if (uwordrg42 == lcon(0x3))
        {
          i16 wordrg15 = s100prg1->uw_02 & lcon(0x7f);
          vol_18.poke16(wordrg15);
          if (   wordrg15 == wcon(0x27)
              || wordrg15 == wcon(0x1b)
              || wordrg15 == wcon(0x1d)
              || wordrg15 == wcon(0x41)
              || wordrg15 == wcon(0x2c)
              || wordrg15 == wcon(0x32)
              || wordrg15 == wcon(0x30)
              || wordrg15 == wcon(0x2d)
             )
          {
            vol_18.poke16(s100prg6->uw_02 >> 7);
            vl_10 = lcon(0x1ff);
            if (DM2_SUPPRESS_WRITER(ADRESSOF(c_ol32, &vol_18), ADRESSOF(i32, &vl_10), 2, 1))
              return true;
          }
        }
      }
      else if (uwordrg42 <= uwcon(0x4))
      {
        vba_1c[0x0] = s100prg1->b_04;
        vba_20[0x0] = lcon(0x7f);
        if (DM2_SUPPRESS_WRITER(ADRESSOF(i8, vba_1c), ADRESSOF(i8, vba_20), 1, 1))
          return true;
        if ((DM2_QUERY_CREATURE_AI_SPEC_FLAGS(vl_04) & 0x1) != 0)
          bprg7 = v1d648f;
        i16 wordrg18 = ddat.v1d6312;
        ddat.v1d6312 = wordrg18 + 1;
        ddat.v1e08e4[mkul(eaxw & lcon(0x3ff))] = wordrg18;
      }
      else if (uwordrg42 < uwcon(0x9))
        ;
      else if (uwordrg42 <= uwcon(0x9))
      {
        vba_1c[0x0] = CUTX8(s100prg1->uw_04 << bcon(0xd) >> bcon(0xe));
        vba_20[0x0] = 3;
        if (DM2_SUPPRESS_WRITER(ADRESSOF(i8, vba_1c), ADRESSOF(i8, vba_20), 1, 1))
          return true;
        if (DM2_IS_CONTAINER_MAP(vl_04) != 0)
        {
          bprg7 = v1d64b7;
          vl_08 = 1;
        }
        i16 wordrg17 = ddat.v1d6310;
        ddat.v1d6310 = wordrg17 + 1;
        ddat.v1e08f0[mkul(eaxw & wcon(0x3ff))] = wordrg17;
      }
      else if (uwordrg42 == uwcon(0xe))
      {
        if (ddat.v1d6521 != 0)
        {
          bprg7 = v1d64d3;
          vl_08 = 1;
        }
      }
      if (DM2_SUPPRESS_WRITER(DOWNCAST(s_rec, s100prg6), bprg7, unsignedlong(table_recordsizes[unsignedlong(vw_14)]), 1))
        return true;
      ui16 uwordrg4 = vw_14;
      if (uwordrg4 < uwcon(0x9))
      {
        if (uwordrg4 == lcon(0x4))
        {
          ddat.v1d6521 = 1;
          i32 longrg2 = 1;
          if (DM2_WRITE_RECORD_CHECKCODE(s100prg6->uw_02, bprg7 != v1d647f ? 1 : 0, longrg2))
            return true;
          ddat.v1d6521 = 0;
        }
      }
      else if (uwordrg4 <= uwcon(0x9))
      {
        if (vl_08 == 0)
        {
          if (DM2_IS_CONTAINER_MONEYBOX(mkl(eaxw)) != 0)
            table1d64db[0xa] = v1d64c3;
          if (DM2_WRITE_RECORD_CHECKCODE(s100prg6->uw_02, 0, 1))
            return true;
          if (DM2_IS_CONTAINER_MONEYBOX(mkl(eaxw)) != 0)
            table1d64db[0xa] = v1d64bf;
        }
        else
        {
          bool boolrg4;
          if (s100prg6->uw_02 == uwcon(0xffff) || s100prg6->uw_02 == uwcon(0xfffe))
            boolrg4 = false;
          else
            boolrg4 = true;
          if (DM2_WRITE_1BIT(boolrg4 ? 1 : 0))
            return true;
          if (boolrg4)
            DM2_ADD_INDEX_TO_POSSESSION_INDICES(mkl(eaxw));
        }
      }
      else if (uwordrg4 < uwcon(0xe))
        ;
      else if (uwordrg4 <= uwcon(0xe))
      {
        if (vl_08 != 0)
          DM2_ADD_INDEX_TO_POSSESSION_INDICES(mkl(eaxw));
        else
        {
          ddat.v1d651d = 1;
          if (DM2_WRITE_RECORD_CHECKCODE(s100prg6->uw_02, 0, 0))
            return true;
          ddat.v1d651d = 0;
        }
      }
      else if (uwordrg4 == uwcon(0xf))
      {
        c_tim* c38prg19 = timdat.timerarray;
        vol_18.poke32(0);
      M_24E51:
        if (unsignedlong(vol_18.peek16()) < mkul(signedlong(timdat.num_timer_indices)))
        {
          if (!c38prg19->is_type(0x19) || eaxw != c38prg19->getB())
          {
            c38prg19++;
            vol_18.inc32();
            goto M_24E51;
                    }
                    if (DM2_WRITE_1BIT(1))
            return true;
                    vl_10 = lcon(0x3ff);
                    if (DM2_SUPPRESS_WRITER(ADRESSOF(c_ol32, &vol_18), ADRESSOF(i32, &vl_10), 2, 1))
            return true;
        }
        else
        {
          if (DM2_WRITE_1BIT(0))
            return true;
        }
      }
    }

    if (vl_00 == 0)
      break;
    eaxw = mkuw(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(eaxw))));
  }

  if (vl_00 == 0 && eaxw != wcon(0xffff))
    return false;
  if (DM2_WRITE_1BIT(0))
    return true;
  return false;
}

static bool DM2_2066_0b44(void)
{
  c_tim* c38prg3 = timdat.timerarray;

  for (i16 i=0; i < timdat.num_timers; i++)
  {
    if (c38prg3->is_type(0x3c) || c38prg3->is_type(0x3d))
    {
      if (DM2_WRITE_RECORD_CHECKCODE(c38prg3->getB(), 0, 0))
        return true;
    }
    c38prg3++;
  }
  return false;
}

static bool DM2_STORE_EXTRA_DUNGEON_DATA(void)
{
  t_tile* tiptrrg7;
  c_5bytes c12_00;
  i32 vl_08;
  c_o32 vo_0c; // poke 16 peek 16 32 inc16
  c_o32 vo_10; // poke 16 peek 32 inc16
  i16 vw_14;
  t_tile tiba_18;
  i8 vba_1c[0x4]; // TODO maybe one char only

  ddat.v1e08e0 = 0;
  if (DM2_2066_0b44())
    return false;
  vw_14 = ddat.v1d3248;
  vo_0c.poke16(0);
  for (;;)
  {
    if (mkuw(vo_0c.peek16()) >= unsignedword(CUTX8(ddat.savegamep4->warr_00[2])))
    {
      DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_14));
      return true;
    }
    DM2_CHANGE_CURRENT_MAP_TO(signedlong(vo_0c.peek16()));
    tiptrrg7 = mapdat.map[0];
    i16* wptrrg6 = UPCAST(i16, DOWNCAST(t_record, dm2_v1e038c + ddat.v1e03f4[0]));
    vo_10.poke16(0);
    while (vo_10.peek16() < mapdat.map_width)
    {
      i16 wordrg51 = 0;
      while (wordrg51 < mapdat.map_height)
      {
        tiba_18 = *tiptrrg7++;
        vl_08 = 0;
        ui8 ubyterg1 = tiba_18 >> 5;
        if (ubyterg1 <= ubcon(0x7))
        {
          switch (ubyterg1)
          {
            case 0:
            case 1:
            case 3:
            case 7:
              vba_1c[0x0] = 0;
            break;
            case 2:
              vba_1c[0x0] = lcon(0x8);
            break;
            case 4:
              vba_1c[0x0] = lcon(0x7);
            break;
            case 5:
              if (DM2_GET_TELEPORTER_DETAIL(&c12_00, signedlong(vo_10.peek16()), signedlong(wordrg51)) == 0)
                vba_1c[0x0] = lcon(0x8);
              else
              {
                vba_1c[0x0] = 0;
                if (unsignedlong(vo_0c.peek16()) > unsignedlong(c12_00.b_04))
                  vl_08 = 1;
              }
            break;
            case 6:
              vba_1c[0x0] = 4;
            break;
          }
        }
        if (vba_1c[0x0] != 0)
          if (DM2_SUPPRESS_WRITER(ADRESSOF(t_tile, &tiba_18), ADRESSOF(i8, vba_1c), 1, 1))
            return false;
        i16 wordrg19;
        if ((tiba_18 & bcon(0x10)) == 0)
          wordrg19 = lcon(0xfffe);
        else
          wordrg19 = *wptrrg6++;
        if (vl_08 == 0)
          if (DM2_WRITE_RECORD_CHECKCODE(wordrg19, 1, 1))
            return false;
        wordrg51++;
      }
      vo_10.inc16();
    }
    vo_0c.inc16();
  }
}

static void DM2_1c9a_3bab(void)
{
  i16 wordrg2 = 0;
  while (mkuw(ddat.v1d4020) > uwcon(0x0))
  {
    if (creatures[unsignedlong(wordrg2)].w_00 >= wcon(0x0))
      DM2_1c9a_0fcb(unsignedlong(wordrg2));
    wordrg2++;
  }
}

// replaced goto M_25747;  with macro return SUBSAVE(true);
// replaced goto M_25773;  with macro return SUBSAVE(false);
// splitted from M_25747 into the pre-routine FSUBSAVE
// solving DM2_GAME_SAVE_MENU is now much easier.

#define SUBSAVE(x) FSUBSAVE(x, s29_3c.tp_2c, boolrg7)

//void FSUBSAVE(bool flag, t_text* tpv, i32 val)
int FSUBSAVE(bool flag, t_text* tpv, i32 val)	// SPX: changed void to int for MSVC6 compilation convenience
{
//m_25747:
  if (flag)
  {
    DM2_FILE_CLOSE(ddat.savegamefilehandle1);
    DM2_FILE_DELETE(UPCAST(t_text, DOWNCAST(t_text, tpv)));
    DM2_0aaf_0067(DM2_0aaf_02f8(0, lcon(0x1b)) & lcon(0xff));
  }
//m_25773:
  if (ddat.savegamep3 != NULL)
    dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0xc8));
  if (ddat.v1e08e4 != NULL)
    dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(2 * unsignedlong(ddat.savegamep4->warr_00[0xa]));
  if (ddat.v1e08f0 != NULL)
    dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(2 * unsignedlong(ddat.savegamep4->warr_00[0xf]));
  DM2_FILL_ORPHAN_CAII();
  if (val != 0)
    DM2_END_GAME(0);
  DM2_events_38c8_0060();
  DM2_SHOW_MOUSE();
  return 0;	// SPX: added
}

//void DM2_GAME_SAVE_MENU(void)
int DM2_GAME_SAVE_MENU(void)		// SPX: changed void to int for MSVC6 compilation convenience
{
  s_savegamebuffer s33_00;
  s_hex30 s29_3c; s29_3c.tp_2c = NULL;

  bool boolrg7 = false;
  ddat.v1e08fc = 0;
  DM2_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
  DM2_UPDATE_RIGHT_PANEL(0);
  DM2_events_38c8_0002();
  DM2_HIDE_MOUSE();
  DM2_REARRANGE_TIMERLIST();
//m_25125:
  ddat.v1e0992 = 2;
//m_25133:
  i16 wordrg17;
  do
  {
    ddat.v1e08e0 = 0;
    bool boolrg5 = false;
    ddat.v1e08e4 = NULL;
    ddat.v1e08f0 = NULL;
    ddat.savegamep3 = NULL;
    i8 byterg2 = 0;
  //m_25151:
    do
    {
    //m_25168:
      i16 wordrg16 = DM2_0aaf_0067(DM2_0aaf_02f8(lcon(0x1b), unsignedlong(byterg2)) & lcon(0xff));
      i32 longrg3 = signedlong(wordrg16);
      bool doM_2519E = true;
      if (longrg3 != 2)
      {
      //m_25190:
        if (longrg3 == lcon(0x3))
          return SUBSAVE(false);
        if (longrg3 != 1)
        {
        //m_251F7:
          if (wordrg16 == 0)  // ddat.v1d154e != 0 always true, was if-else if-else - other paths removed
          //m_25211:
            boolrg5 = true;
          doM_2519E = false; // goto M_25241;
        }
      }
      else
        ddat.v1e08fc = 1;

    //m_2519E:
      if (doM_2519E)
      {
        i32 longrg11 = ddat.v1e021c + lcon(0x64);
        if (   mkul(longrg11) >= mkul(timdat.gametick)
            || mkul(ddat.v1e08f8 + lcon(0x64)) >= mkul(timdat.gametick)
            || (DM2_0aaf_0067(DM2_0aaf_02f8(lcon(0xc), 0) & lcon(0xff))) != lcon(0x3)
           )
        {
        //m_251E0:
          DM2_SHOW_MOUSE();
          ddat.v1e13f4 = 0;
          boolrg7 = true;
          return SUBSAVE(false);
        }
        byterg2 = 0;
      }

    //m_25241:
    } while (!boolrg5);
    wordrg17 = CUTX16(DM2_dialog_2066_33e7());
  } while (wordrg17 < 0);
  ddat.savegamew4 = wordrg17;
  ddat.v1e099c = CUTX8(ddat.savegamew4) + '0';
  DM2_0aaf_02f8(lcon(0xd), 0);
  t_text* tprg5 = DM2_FORMAT_SKSTR(gdat.filename5, NULL);
  s29_3c.tp_2c = tprg5;
  t_text* tprg3 = DM2_FORMAT_SKSTR(gdat.filename6, NULL);
  DM2_FILE_DELETE(tprg3);
  DM2_FILE_RENAME(tprg5, tprg3);
  i16 wordrg19 = DM2_FILE_CREATE(tprg5);
  ddat.savegamefilehandle1 = wordrg19;
  if (wordrg19 < 0)
    return SUBSAVE(true);
  i16 wordrg18 = DM2_FILE_OPEN(tprg3);
  if (wordrg18 < 0)
  //m_252E1:
    s29_3c.c27_00.l_26 = 0;
  else
  {
    DM2_FILE_READ(wordrg18, ADRESSOF(s_hex30, &s29_3c), lcon(0x2a));
    DM2_FILE_CLOSE(wordrg18);
  }
//m_252E7:
  s29_3c.c27_00.w_00 = 1;
  DM2_STRCPY(s29_3c.c27_00.text, ddat.v1e0900.text);
  DM2_SKSAVE_WRITE(ADRESSOF(s_hex30, &s29_3c), lcon(0x2a));
  if (!DM2_SKSAVE_WRITE(DOWNCAST(s_sgwords, ddat.savegamep4), lcon(0x2c)))
    return SUBSAVE(true);
  if (!DM2_SKSAVE_WRITE(DOWNCAST(unk, ddat.v1e03c8), unsignedword(CUTX8(ddat.savegamep4->warr_00[2])) << 4))
    return SUBSAVE(true);
  if (!DM2_SKSAVE_WRITE(DOWNCAST(unk, ddat.v1e03d8), 2 * ddat.v1e0408))
    return SUBSAVE(true);
  if (!DM2_SKSAVE_WRITE(DOWNCAST(t_record, dm2_v1e038c), 2 * ddat.savegamep4->warr_00[5]))
    return SUBSAVE(true);
  if (!DM2_SKSAVE_WRITE(DOWNCAST(unk, ddat.v1e03d0), 2 * ddat.savegamep4->warr_00[3]))
    return SUBSAVE(true);
  i16 wordrg62 = 0;
//m_253E1:
  while (wordrg62 < wcon(0x10))
  {
  //m_253AC:
    if (!DM2_SKSAVE_WRITE(DOWNCAST(t_record, recordptr[signedlong(wordrg62)]), mkul(ddat.savegamep4->warr_00[signedlong(wordrg62) + lcon(0x6)] * table_recordsizes[wordrg62])))
      return SUBSAVE(true);
    wordrg62++;
  }
  if (!DM2_SKSAVE_WRITE(DOWNCAST(unk, ddat.v1e03e0), unsignedlong(ddat.savegamep4->warr_00[1])))
    return SUBSAVE(true);
  DM2_1c9a_3bab();
  DM2_COMPACT_TIMERLIST();
  DM2_ZERO_MEMORY(ADRESSOF(s_savegamebuffer, &s33_00), sizeof(s_savegamebuffer));
  s33_00.l_00 = timdat.gametick;
  s33_00.ul_04 = randdat.random;
  s33_00.w_08 = party.heros_in_party;
  s33_00.w_0a = ddat.v1e0270;
  s33_00.w_0c = ddat.v1e0272;
  s33_00.w_0e = ddat.v1e0258;
  s33_00.w_10 = ddat.v1e0266;
  s33_00.w_12 = eventqueue.event_heroidx; // e_hero
  s33_00.w_14 = timdat.num_timers;
  s33_00.l_16 = ddat.v1d26a4;
  s33_00.l_1a = ddat.v1e01a0;
  s33_00.w_1e = ddat.v1e026e;
  s33_00.w_20 = ddat.v1e025e;
  s33_00.w_22 = ddat.v1e0274;
  s33_00.w_28 = ddat.v1d26a0 & wcon(0xf) | (ddat.v1d26a2 & wcon(0xf)) << 4;
  s33_00.l_2a = unsignedlong(ddat.v1e147f);
  s33_00.b_2e = ddat.v1e1480;
  s33_00.b_2f = ddat.v1e1483;
  s33_00.b_30 = ddat.v1e1482;
  s33_00.b_31 = ddat.v1e147e;
  s33_00.b_32 = ddat.v1e147d;
  s33_00.b_33 = ddat.v1e1484;
  s33_00.w_34 = ddat.v1e1474;
  s33_00.b_36 = ddat.v1e147b;
  s33_00.b_37 = ddat.v1e1478;
  s33_00.l_38 = ddat.v1e1434;
  DM2_SUPPRESS_INIT();
  if (DM2_SUPPRESS_WRITER(ADRESSOF(s_savegamebuffer, &s33_00), table1d631a, lcon(0x3c), 1))
    return SUBSAVE(true);
  if (DM2_SUPPRESS_WRITER(ddat.v1e0104, ddat.v1d6316, 1, lcon(0x8)))
    return SUBSAVE(true);
  if (DM2_SUPPRESS_WRITER(ddat.globalb, ddat.v1d6316, 1, lcon(0x40)))
    return SUBSAVE(true);
  if (DM2_SUPPRESS_WRITER(DOWNCAST(i16, ddat.globalw), ddat.v1d6316, 2, lcon(0x40)))
    return SUBSAVE(true);
  if (DM2_SUPPRESS_WRITER(DOWNCAST(c_hero, party.hero), table1d6356, sizeof(c_hero), unsignedlong(party.heros_in_party)))
    return SUBSAVE(true);
  if (DM2_SUPPRESS_WRITER(DOWNCAST(c_wbbb, &ddat.savegames1), table1d645d, lcon(0x6), 1))
    return SUBSAVE(true);
  if (DM2_SUPPRESS_WRITER(DOWNCAST(c_tim, timdat.timerarray), v1d6463, sizeof(c_tim), unsignedlong(timdat.num_timers)))
    return SUBSAVE(true);
  ddat.v1e08f0 = XUPCAST(i16, dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(2 * unsignedlong(ddat.savegamep4->warr_00[0xf])));
  ddat.v1e08e4 = XUPCAST(i16, dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(2 * unsignedlong(ddat.savegamep4->warr_00[0xa])));
  ddat.savegamep3 = XUPCAST(i16, dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0xc8)));
  ddat.savegamew6 = 0;
  ddat.v1d6312 = 0;
  ddat.v1d6310 = 0;
  ddat.v1e08e0 = 0;
  i16 wordrg61 = 0;
//m_256E5:
  i32 longrg13;
  while (mkul(longrg13 = signedlong(wordrg61)) < unsignedlong(party.heros_in_party))
  {
  //m_256A5:
    i32 longrg4 = longrg13;
    longrg13 = 263 * longrg4;
    i16* wptrrg3 = party.hero[longrg4].item;
    i16 wordrg51 = 0;
  //m_256DC:
    while (wordrg51 < wcon(0x1e))
    {
    //m_256C2:
      i16 wordrg18 = *wptrrg3++;
      if (DM2_WRITE_RECORD_CHECKCODE(wordrg18, 0, 0))
        return SUBSAVE(true);
      wordrg51++;
    }
    wordrg61++;
  }
  if (DM2_WRITE_RECORD_CHECKCODE(ddat.savegamewpc.w_00, 0, 0))
    return SUBSAVE(true);
  if (!DM2_STORE_EXTRA_DUNGEON_DATA())
    return SUBSAVE(true);
  if (DM2_WRITE_POSSESSION_INDICES())
    return SUBSAVE(true);
  if (DM2_SUPPRESS_FLUSH())
    return SUBSAVE(true);
  DM2_FILE_CLOSE(ddat.savegamefilehandle1);
  ddat.cd.mo.glbSpecialScreen = _MENU_SCREEN__RESUME_GAME_SELECT;
  ddat.v1e08f8 = timdat.gametick;
  return SUBSAVE(false);
}
