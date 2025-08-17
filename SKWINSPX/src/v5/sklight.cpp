#include <util.h>
#include <dm2data.h>
#include <skhero.h>
#include <skgdtqdb.h>
#include <skguivwp.h>
#include <skmap.h>
#include <bitem.h>
#include <sktimer.h>
#include <sklight.h>

// TODO: optimize temporaries
#include <regs.h>
#include <SK1C9A.h>
#include <emu.h>

i32 DM2_RECALC_LIGHT_LEVEL(void)
{
  c_nreg _RG1;
  c_nreg _RG3;
  c_nreg _RG2;
  c_nreg _RG4;
  i16 RG51w;
  c_wlreg _RG52;
  c_wlreg _RG6;
  i16 vwa_00[0x9];
  c_o32 vo_14; // poke 16 peek 32 add16

  RG1L = signedlong(ddat.v1e0266) << 4;
  RG4P = ddat.v1e03c8;
  if (jnz_test8(location(RG4P + RG1L + lcon(0xd)), lcon(0xfffffff0)))
  {
    //m_28AA1:
    DM2_ZERO_MEMORY(ADRESSOF(i16, vwa_00), lcon(0x12));
    RG51w = wcon(0x0);
    RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(ddat.savegamewpc.w_00), 0);
    RG1W &= lcon(0x10);
    RG1L = unsignedlong(RG1W);
    if (RG1L != 0)
    {
      RG1W = DM2_ADD_ITEM_CHARGE(unsignedlong(ddat.savegamewpc.w_00), 0);
      vwa_00[0x0] = RG1W;
      RG51w = 1;
    }
    //m_28AE1:
    RG6L = 0;
    //m_28B38:
    for (;;)
    {
      RG4L = signedlong(RG6W);
      RG1L = unsignedlong(party.heros_in_party);
      if (RG4L >= RG1L)
      {
        RG6L = 0;
        break;
      }
      //m_28AE5:
      RG3L = 0;
      //m_28B2F:
      for (;;)
      {
        RG4L = signedlong(RG3W);
        if (RG4L > 1)
        {
          RG6L++;
          break;
        }
        //m_28AE9:
        RG2L = signedlong(RG6W);
        RG1W = party.hero[RG2L].item[RG4L];
        RG2L = unsignedlong(RG1W);
        RG4L = 0;
        RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG2L, RG4L);
        RG1W &= lcon(0x10);
        RG1L = unsignedlong(RG1W);
        if (RG1L != 0)
        {
          RG4L = 0;
          RG1W = DM2_ADD_ITEM_CHARGE(RG2L, 0);
          RG4L = signedlong(RG51w);
          vwa_00[RG4L] = RG1W;
          RG51w++;
        }
        //m_28B2E:
        RG3L++;
      }
    }
    //m_28B69:
    for (;;)
    {
      RG4L = signedlong(RG51w);
      RG1L = signedlong(RG6W);
      RG4L--;
      if (RG1L >= RG4L)
      {
        RG4L = 6;
        RG2L = 0;
        vo_14.poke16(RG2W);
        RG6L = 0;
        break;
      }
      //m_28B4B:
      RG1L = 2 * RG1L;
      RG4W = vwa_00[RG1L / 2];
      if (RG4W > vwa_00[1 + RG1L / 2])
      {
        RG3L = RG4L;
        RG4W = vwa_00[1 + RG1L / 2];
        vwa_00[RG1L / 2] = RG4W;
        vwa_00[1 + RG1L / 2] = RG3W;
      }
      RG6L++;
    }
    //m_28BBA:
    for (;;)
    {
      if (RG6W >= RG51w)
      {
        RG1W = ddat.v1e0974;
        RG52W = vo_14.peek16() + RG1W;
        vo_14.poke16(RG52W);
        RG1W = ddat.savegames1.w_00;
        RG6L = RG52L + RG1L;
        vo_14.poke16(RG6W);
        RG4L = unsignedlong(ddat.v1d6c02);
        RG3L = lcon(0x67);
        RG2L = lcon(0xb);
        RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0x8), RG4Blo, RG2Blo, RG3Blo);
        RG4W = RG6W + RG1W;
        vo_14.poke16(RG4W);
        if (ddat.v1e147f != 0)
        {
          RG1L = unsignedlong(ddat.v1e1480);
          RG1W += ddat.v1e1476;
          RG4L = signedlong(RG1W);
          RG2L = 5;
          RG1L = signedlong(DM2_BETWEEN_VALUE(0, 5, RG4W));
          RG1W = signedword(table1d6712[RG1L]);
          vo_14.add16(RG1W);
        }
        ddat.v1e0286 = wcon(0x0);
        RG6W = vo_14.peek16();
        break;
      }
      //m_28B84:
      RG2L = signedlong(RG6W);
      RG2L = signedlong(table1d6702[signedlong(vwa_00[RG2L])]);
      RG3L = RG4L;
      RG2L <<= RG3Blo;
      RG2L >>= bcon(0x6);
      RG1L = signedlong(vo_14.peek16()) + RG2L;
      vo_14.poke16(RG1W);
      RG4L--;
      RG4L = signedlong(RG4W);
      RG1L = signedlong(DM2_MAX(0, RG4W));
      RG4L = RG1L;
      RG6L++;
    }
    //m_28C5B:
    for (;;)
    {
      RG1L = signedlong(ddat.v1e0286);
      if (RG1L < lcon(0x5))
      {
        //m_28C47:
        RG1W = signedword(table1d6712[RG1L]);
        if (RG1W >= RG6W)
        {
          ddat.v1e0286++;
          continue;
        }
      }
      break;
    }
    //m_28C68:
    RG4L = unsignedlong(ddat.v1d6c02);
    RG3L = lcon(0x68);
    RG2L = lcon(0xb);
    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(8, RG4Blo, 11, lcon(0x68));
    if (RG1W > ddat.v1e0286)
      ddat.v1e0286 = RG1W;
    if (ddat.v1e147f != bcon(0x0) && ddat.v1e024c != 0)
    {
      RG2L = 0;
      ddat.v1e0286 = RG2W;
    }
  }
  else
    ddat.v1e0286 = 1;
  //m_28CAE:
  RG1L = signedlong(ddat.v1e0978);
  RG1Blo = RG1L > lcon(0xc) ? 1 : 0;
  RG4L = RG1L & lcon(0xff);
  RG1L = signedlong(ddat.v1e0286) - RG4L;
  ddat.v1e0286 = RG1W;
  RG1L = signedlong(DM2_BETWEEN_VALUE(0, 5, ddat.v1e0286));
  ddat.v1e0286 = RG1W;
  return RG1L;
}

// was SKW_24a5_036a
// stacksize was 0x44
void DM2_ADD_BACKGROUND_LIGHT_FROM_TILE(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_wlreg _RG6;
  i32 RG7l;
  c_ql32 vql_00;
  c_ql32 vql_04;
  c_ql32 vql_08;
  s_8wordarray s15_0c;
  c_5bytes c12_1c;
  i16 vw_24;
  c_o32 vo_28; // poke 16 peek 16 32 and16
  c_o32 vo_2c; // poke 16 peek 8 pick2ndbyte
  i8 vb_30;
  bool skip00657 = false;

  vql_08.poke32(RG1L);
  vql_04.poke32(RG2L);
  vql_00.poke32(RG3L);
  RG6L = 0;
  vw_24 = RG6W;
  RG5L = 0;
  RG3L = signedlong(vql_00.peek16());
  RG2L = signedlong(vql_04.peek16());
  RG4L = signedlong(RG4W);
  RG1P = ADRESSOF(s_8wordarray, &s15_0c);
  DM2_SUMMARIZE_STONE_ROOM(UPCAST(s_8wordarray, RG1P), RG4L, RG2L, RG3L);
  RG3W = s15_0c.warr_00[0x0]; // adj
  if (RG3UW < mkuw(1))
  {
    //m_28D60:
    if (RG3W != 0)
      jump L_fin;
    RG1L = argl0;
    RG1W &= 0x4;
    RG1L = unsignedlong(RG1W);
    if (RG1L == 0)
      jump L_fin;
    vo_2c.poke16(s15_0c.warr_00[0x5]);
    RG1Blo = vo_2c.peek8();
    vb_30 = RG1Blo;
    if (RG1Blo == lcon(0xffffffff))
      jump L_fin;
    RG4L = unsignedlong(RG1Blo);
    RG3L = lcon(0xf8);
    RG2L = lcon(0xb);
    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, RG4Blo, 11, lcon(0xf8));
    vo_28.poke16(RG1W);
    if (RG1W == 0)
      jump L_fin;
    RG1W &= lcon(0x8000);
    RG1L = unsignedlong(RG1W);
    RG1Blo = RG1L != 0 ? 1 : 0;
    RG7l = RG1L & lcon(0xff);
    vo_28.and16(lcon(0x7fff)); // adj
    RG4L = unsignedlong(vb_30);
    RG3L = lcon(0x63);
    RG2L = lcon(0xb);
    RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, RG4Blo, 11, 99);
    if (RG1W != 0)
    {
      RG1L = unsignedlong(ddat.v1e1480);
      RG1W += ddat.v1e1476;
      RG4L = signedlong(RG1W);
      RG2L = 5;
      RG1L = signedlong(table1d6712[signedlong(DM2_BETWEEN_VALUE(0, RG2W, RG4W))]);
      RG4L = unsignedlong(vo_28.peek16()) * RG1L;
      RG2L = lcon(0x64);
      RG1L = RG4L / RG2L;
      RG4L = RG1L;
      RG1L = signedlong(vw_24) + RG4L;
      vw_24 = RG1W;
      jump L_fin;
    }
    //m_28E46:
    if (RG7l != 0)
    {
      RG1L = signedlong(vo_2c.getbyte1());
      if (RG1L == 0)
        jump L_fin;
    }
    //m_28E59:
    RG6W += vo_28.peek16();
    jump L_fin;
  }
  if (RG3UW > mkuw(2))
  {
    if (RG3UW < mkuw(lcon(0x5)))
      jump L_fin;
    if (RG3UW > mkuw(lcon(0x5)))
    {
      if (RG3UW < mkuw(lcon(0x10)))
        jump L_fin;
      if (RG3UW > mkuw(lcon(0x13)))
        jump L_fin;
    }
  }
  //m_28E62:
  RG1L = argl0;
  RG1W &= 0x1;
  RG1L = unsignedlong(RG1W);
  if (RG1L != 0)
  {
    RG1L = signedlong(s15_0c.warr_00[0x0]);
    if (RG1L == 1 || RG1L == 2 || RG1L == lcon(0x5))
    {
      vo_2c.poke16(s15_0c.warr_00[0x5]);
      RG1Blo = vo_2c.peek8();
      vb_30 = RG1Blo;
      if (RG1Blo != lcon(0xffffffff))
      {
        RG4L = unsignedlong(RG1Blo);
        RG3L = lcon(0xf8);
        RG2L = lcon(0xb);
        RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(10, RG4Blo, 11, lcon(0xf8));
        vo_28.poke16(RG1W);
        if (RG1W == 0)
          jump L_fin;
        RG1W &= lcon(0x8000);
        RG1L = unsignedlong(RG1W);
        RG1Blo = RG1L != 0 ? 1 : 0;
        RG3L = unsignedlong(RG1Blo);
        vo_28.and16(lcon(0x7fff));
        RG2L = signedlong(vql_00.peek16());
        RG4L = signedlong(vql_04.peek16());
        RG1L = DM2_GET_TELEPORTER_DETAIL(&c12_1c, RG4L, RG2L);
        if (RG1L == 0)
        {
          bool skip00652 = false;
          //m_28F4E:
          if (RG3L == 0)
            skip00652 = true;
          else
          {
            RG1L = signedlong(vo_2c.getbyte1());
            if (RG1L != 0)
              skip00652 = true;
          }

          if (skip00652)
            //m_28F5D:
            RG6W += vo_28.peek16();
        }
        else
        {
          RG1L = unsignedlong(ddat.v1e1480);
          RG1W += ddat.v1e1476;
          RG4L = signedlong(RG1W);
          RG1L = signedlong(table1d6712[signedlong(DM2_BETWEEN_VALUE(0, 5, RG4W))]);
          RG4L = unsignedlong(vo_28.peek16()) * RG1L;
          RG2L = lcon(0x64);
          RG1L = RG4L / RG2L;
          RG4L = RG1L;
          RG1L = signedlong(vw_24) + RG4L;
          vw_24 = RG1W;
        }
      }
    }
    //m_28F61:
    RG4W = s15_0c.warr_00[0x2]; // adj
    //m_28F65:
    // TODO this loop runs endless, if the mouse gets not handled!
    for (;;)
    {
      bool skip00655 = false;
      if (RG4W == lcon(0xfffffffe))
        break;
      RG1L = RG4L;
      RG1Blo ^= RG4Blo;
      RG1Bhi &= lcon(0x3c);
      RG1L = unsignedlong(RG1W) >> bcon(0xa);
      RG1L = unsignedlong(RG1W);
      if (RG1L != lcon(0xf))
      {
        //m_28FC0:
        if (RG1L != lcon(0xe))
          skip00655 = true;
        else
        {
          RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
          RG3L = unsignedlong(word_at(RG1P, 2));
          if (RG3L != lcon(0xff80))
            skip00655 = true;
          else
          {
            RG1Blo = byte_at(RG1P, lcon(0x4));
            RG3L = signedlong(table1d673d[0x3]);
            RG1L = unsignedlong(RG1Blo) / 2 - RG3L;
          }
        }
      }
      else
      {
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
        RG3P = RG1P;
        RG2W = word_at(RG1P, 2) & lcon(0x7f);
        if (RG2W != 2 && jnz_test8(location(RG1P + 2), lcon(0x7f)) && RG2W != lcon(0x30))
          skip00655 = true;
        else
        {
          RG1W = word_at(RG3P, 2);
          RG1UW >>= bcon(0x8);
          RG1L = unsignedlong(RG1W);
          RG1UL >>= bcon(0x1);
        }
      }

      if (!skip00655)
        //m_28FF2:
        RG5L = signedlong(RG5W) + RG1L;

      //m_28FF7:
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
      RG4L = RG1L;
    }
  }
  //m_29008:
  RG1L = unsignedlong(mkuw(CUTX16(argl0) & wcon(0x2)));
  if (RG1L != 0)
  {
    RG1L = DM2_GET_CREATURE_AT(unsignedlong(vql_04.peek16()), unsignedlong(vql_00.peek16()));
    if (RG1W != lcon(0xffffffff))
    {
      RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG1W), lcon(0xf8));
      RG1Bhi &= lcon(0x7f);
      RG6L += RG1L;
    }
  }

L_fin:
  RG1L = signedlong(vql_08.peek16());
  if (RG1L > lcon(0x8))
    skip00657 = true;
  else
  {
    RG2L = unsignedlong(vw_24);
    if (RG2W == 0)
      skip00657 = true;
    else
    {
      RG4L = RG2L;
      RG2W = table1d672b[RG1L];
      RG4L = signedlong(CUTX16(RG4L - RG2L));
      RG1L = signedlong(DM2_MAX(3, RG4W));
      vw_24 = RG1W;
    }
  }

  if (skip00657)
  {
    //m_2907A:
    RG3L = 0;
    vw_24 = RG3W;
  }

  //m_29081:
  RG1L = signedlong(vql_08.peek16());
  if (RG1L > lcon(0x5))
    vql_08.poke16(lcon(0x5));
  if (RG6W != 0)
  {
    RG6W -= table1d673d[signedlong(vql_08.peek16())];
    RG4L = signedlong(RG6W);
    RG6L = signedlong(DM2_MAX(2, RG4W));
  }
  //m_290B7:
  RG6L += unsignedlong(vw_24);
  ddat.v1e0974 += RG6W;
  if (RG5W != 0)
  {
    RG5W -= table1d673d[signedlong(vql_08.peek16())];
    RG4L = signedlong(RG5W);
    RG5L = signedlong(DM2_MAX(2, RG4W));
  }
  //m_290E5:
  ddat.v1e0978 += RG5W;
}

// was SKW_1c9a_02f6
// stacksize was 0x4c
// about s23_34: basically I had only one instance here, but in fact
// the adress is given to s23p_4, and then is argument in DM2_FIND_WALK_PATH (argp_23),
// which accesses adress+0x10 then. That looks like two more of them.
// for now I filled up the full original stacksize here.
i32 DM2_CHECK_RECOMPUTE_LIGHT(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG6w;
  c_buttongroup buttongroup;
  s_len8 s23_34[3];
  s_len8* s23p_1;
  c_button* button1;
  s_len8* s23p_4;
  c_button* button2;
  bool flag;
  
  RG6w = ddat.v1d3248;
  DM2_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
  if ((ddat.v1e0390.b_00 & bcon(0x2)) != 0)
  {
    ddat.v1d62a4 = ddat.v1e0266;
    ddat.v1e08d2 = mapdat.map_width;
    RG4L = signedlong(ddat.v1e08d2) << 5;
    DM2_ZERO_MEMORY(ddat.v1e08cc, RG4L);
    RG1W = ddat.v1e027c;
    ddat.v1d62a6 = RG1W;
    if (RG1W >= 0)
    {
      RG1L = signedlong(ddat.v1e027c) << 4;
      RG1W = word_at(ddat.v1e03c8, RG1L + lcon(0x8));
      RG1L <<= bcon(0x5);
      _RG1.ushiftr11();
      RG1L = unsignedlong(RG1W) + 1;
      ddat.v1e08d0 = RG1W;
      RG4L = signedlong(ddat.v1e08d0) << 5;
      DM2_ZERO_MEMORY(ddat.v1e08c8, RG4L);
    }
    //m_1FA4D:
    s23_34[0].b_00 = lcon(0x19);
    RG4W = unsignedword(RG4Blo);
    s23_34[0].b_01 = RG4Bhi;
    s23_34[0].b_07 = lcon(0x1b);
    button1 = &buttongroup.button;
    s23p_1 = &s23_34[0];
    RG4L = signedlong(ddat.v1e0272);
    RG1L = signedlong(ddat.v1e0270);
    RG3L = 0;
    DM2_FIND_WALK_PATH(RG1L, RG4L, lcon(0x8), 1, s23p_1, button1);
  }
  //m_1FA84:
  if ((ddat.v1e0390.b_00 & bcon(0x1)) != 0)
  {
    RG1L = signedlong(ddat.v1e0266);
    flag = RG1L != ddat.v1d62ac;
    RG2W = ddat.v1e03f8;
    if (RG2W != 0)
    {
      bool skip00527 = false;
      //m_1FAC6:
      RG1L = unsignedlong(RG2W);
      if (RG1L > lcon(0x8))
        ddat.v1e03f8 = lcon(0x8);
      RG1Blo = CUTX8(ddat.v1e03f8);
      s23_34[0].b_00 = RG1Blo;
      s23_34[0].b_01 = 0;
      s23_34[0].b_07 = lcon(0x17);
      button2 = &buttongroup.button;
      s23p_4 = &s23_34[0];
      RG4L = signedlong(ddat.v1e0272);
      RG1L = signedlong(ddat.v1e0270);
      RG3L = 0;
      DM2_FIND_WALK_PATH(RG1L, RG4L, lcon(0x7), 1, s23p_4, button2);
      RG1W = ddat.v1e0974;
      if (RG1W != ddat.v1d62a8)
        skip00527 = true;
      else
      {
        RG1W = ddat.v1e0978;
        if (RG1W != ddat.v1d62aa)
          skip00527 = true;
      }

      if (skip00527)
        //m_1FB32:
        flag = true;
    }
    else
    {
      ddat.v1e0974 = RG2W;
      ddat.v1e0978 = RG2W;
    }
    //m_1FB37:
    ddat.v1d62a8 = ddat.v1e0974;
    ddat.v1d62aa = ddat.v1e0978;
    if (flag)
      DM2_RECALC_LIGHT_LEVEL();
  }
  //m_1FB58:
  RG1L = signedlong(RG6w);
  DM2_CHANGE_CURRENT_MAP_TO(RG1L);
  ddat.v1e0390.l_00 = 0;
  RG1L = signedlong(ddat.v1e0266);
  ddat.v1d62ac = RG1L;
  return RG1L;
}

// was SKW_2759_15d0 (new)
void DM2_PROCEED_LIGHT(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  i32 RG6l;
  c_tim c38_00;
  bool skip00710 = false;

  RG3L = RG1L;
  RG6l = 1;
  RG4L += RG6l;
  RG4L = signedlong(RG4W);
  RG2L = lcon(0x100);
  RG4L = signedlong(DM2_BETWEEN_VALUE(32, RG2W, RG4W) / 8);
  RG1L = signedlong(DM2_MAX(8, RG4W));
  RG4L = RG1L;
  RG2L = RG1L - lcon(0x8);
  if (RG3UW < mkuw(lcon(0x26)))
  {
    //m_313DC:
    if (RG3W != lcon(0x6))
      skip00710 = true;
    else
    {
      RG5L = 16 * RG2L + lcon(0x10);
      RG6l = lcon(0xfffffffe);
    }
  }
  else
  {
    if (RG3UW <= mkuw(lcon(0x26)))
    {
      //m_313F1:
      RG5L = ((RG1L - lcon(0x3)) << 7) + lcon(0x7d0);
      RG4W >>= bcon(0x2);
      RG4L += RG6l;
      skip00710 = true;
    }
    else
    {
      if (RG3W == lcon(0x27))
        //m_31407:
        RG5L = (RG2L << bcon(0x9)) + lcon(0x2710);
      else
        skip00710 = true;
    }
  }

  if (!skip00710)
  {
    //m_31412:
    RG4W >>= bcon(0x1);
    RG4L--;
  }

  //m_31416:
  c38_00.settype(0x46);
  if (RG3W != lcon(0x6))
    //m_31426:
    RG1L = -signedlong(RG4W);
  else
    RG1L = signedlong(RG4W);
  //m_3142B:
  c38_00.setA(RG1W);
  c38_00.setmticks(ddat.v1e0266, unsignedlong(RG5W) + timdat.gametick);
  c38_00.setactor(0);
  DM2_QUEUE_TIMER(&c38_00);
  RG1L = signedlong(RG4W);
  RG1W = signedword(table1d6702[RG1L]);
  RG1L *= RG6l;
  ddat.savegames1.w_00 += RG1W;
  DM2_RECALC_LIGHT_LEVEL();
}
