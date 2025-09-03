#include <skhero.h>
#include <skgdtqdb.h>
#include <skxrect.h>
#include <skmap.h>
#include <util.h>
#include <uiinput.h>
#include <skeventq.h>
#include <uitmouse.h>
#include <skguidrw.h>
#include <dm2data.h>
#include <SK1C9A.h>
#include <startend.h>
#include <SK1031.h>

// Note: old SKW_1031_0000 (SKW_RETURN_1) replaced directly

// TODO: optimize temporaries
#include "emu.h"
#include "regs.h"

#include "dm2debug.h"

c_rect* DM2_1031_01d5(i16 eaxw, c_rect* edxrp)
{
  c_rect* r;
  i16 vw_00;
  i16 vw_04;

  r = DM2_QUERY_EXPANDED_RECT(eaxw & wcon(0x3fff), edxrp);
  if (r == NULL)
    return r;
  i16 idx;
  if ((eaxw & wcon(0x8000)) == 0)
  {
    //m_10376:
    if ((eaxw & wcon(0x4000)) == 0)
      return r;
    idx = 18;
  }
  else
    idx = 7;
  //m_10387:
  DM2_QUERY_TOPLEFT_OF_RECT(idx, OO vw_04, OO vw_00);
  r->x += vw_04;
  r->y += vw_00;
  return r;
}

const i8* DM2_1031_023b(s_bbw* eaxp)
{
  return (i8*)&table1d3cd0[eaxp->w_02];
}

s_www* DM2_1031_024c(s_bbw* eaxp)
{
  i16 n = table1d3d23[eaxp->w_02].w_02;
  return (n != -1) ? &v1d338c[n] : NULL;
}

// four times extracted
bool gate_1031(i32 c, s_bbw* p)
{
  bool b;
  i8 v = p->b_01;

  switch (c)
  {
    case  0:
      b = true;
      break;

    case  1: // DM2_IS_GAME_ENDED SKW_1031_0008
      return unsignedlong(v) == (ui32) ddat.dialog2;

    case  2: // DM2_1031_0023
      return unsignedword(v) == ddat.v1e0204;

    case  3: // DM2_1031_003e
    {
      c_nreg _RG4;

      RG4W = unsignedword(v);
      if (RG4W == ddat.v1e0976)
        return true;
      i16 rg1w = unsignedword(RG4Blo);
      if (rg1w <= 4)
        return false;
      if (ddat.v1e0976 == 0)
        return true;
      return (rg1w - 4) != ddat.v1e0976;
    }

    case  4: // DM2_1031_007b
      return party.hero[v].curHP != 0;

    case  5: // DM2_1031_009e
      return DM2_GET_PLAYER_AT_POSITION((unsignedword(v) + ddat.v1e0258) & 0x3) >= 0;

    case  6: // DM2_1031_00c5
      if (v == 0)
      {
        if (ddat.v1e00b8 == 0)
          return true;
        else
          return false;
      }
      return ddat.v1e00b8 != 0;

    case  7: // DM2_1031_00f3
      if (party.curacthero != 0)
        return false;
      if (v > 3)
        return true;
      return DM2_GET_PLAYER_AT_POSITION((unsignedword(v) + ddat.v1e0258) & 0x3) >= 0;

    case  8: // DM2_1031_012d
      if (party.curacthero == 0)
        return false;
      return unsignedword(v) == ddat.v1e0b7a;

    case  9: // DM2_1031_014f
    {
      if (party.curacthero == 0)
        return false;
      i32 rg4l = 1 << party.hero[party.curacthero - 1].nrunes;
      return (rg4l & unsignedlong(v)) != 0;
    }

    case 10: // DM2_1031_0184
      if (party.curacthero == 0)
        return false;
      if ((ddat.v1e0b62 & wcon(0x800)) != 0)
        return unsignedword(v) == ddat.v1e0b7a;
      return v == 5;

    case 11: // DM2_1031_01ba
      return unsignedword(v) == ddat.v1d67bc;

    default: throw(THROW_DMABORT);
  }
  return b;
}

void DM2_1031_027e(s_bbw* eaxp)
{
  c_nreg _RG1;
  c_nreg _RG3;
  c_nreg _RG4;
  s_bbw* rg4;

  const i8* rg2p = DM2_1031_023b(eaxp);
  //m_103F1:
  for (;;)
  {
    RG1L = unsignedlong(byte_at(rg2p));
    RG1Blo &= lcon(0x7f);
    rg4 = &table1d3ba0[RG1L];
    RG1Blo = eaxp->b_00 & lcon(0x7f);
    RG3L = unsignedlong(RG1Blo);
    if (gate_1031(RG3L, rg4))
    {
      RG1Blo = rg4->b_00 & lcon(0xffffff80);
      RG1L = unsignedlong(RG1Blo);
      if (RG1L != lcon(0x80))
      {
        //m_10430:
        RG1L = signedlong(rg4->w_02);
        RG4L = 7 * RG1L;
        table1d3d23[RG1L].b_06 |= lcon(0x40);
      }
      else
        DM2_1031_027e(rg4);
    }
    //m_10443:
    rg2p++;
    RG1Blo = byte_at(rg2p) & lcon(0xffffff80);
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 0)
      return;
  }
  FOREND
}

i32 DM2_1031_030a(s_bbw* eaxp, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  const i8* RG6p;
  c_wlreg _RG7;
  c_ql32 vql_00;
  c_ql32 vql_04;
  c_ql32 vql_08;
  s_bbw* xp_0c;
  c_rect rc_10;
  i32 vl_18;

  xp_0c = eaxp;
  vql_08.poke32(RG4L);
  vql_04.poke32(RG2L);
  vql_00.poke32(RG3L);
  RG3L = 0;
  RG6p = DM2_1031_023b(eaxp);
  //m_10467:
  for (;;)
  {
    s_bbw* RG52p = &table1d3ba0[unsignedword(byte_at(RG6p)) & 0x7f];
    RG1Blo = xp_0c->b_00 & lcon(0x7f);
    RG4L = unsignedlong(RG1Blo);
    if (gate_1031(RG4L, RG52p))
    {
      bool skip00181 = false;
      RG1Blo = RG52p->b_00 & 0x80;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L != lcon(0x80))
      {
        //m_104C3:
        vl_18 = signedlong(vql_04.peek16());
        RG7L = signedlong(vql_08.peek16());
        RG4L = signedlong(RG52p->w_02);
        RG1L = signedlong(table1d3d23[RG4L].w_00);
        RG4R = &rc_10;
        RG1R = DM2_1031_01d5(RG1W, RG4R);
        RG2L = vl_18;
        RG1L = RG1R->pt_in_rect(RG7W, RG2W) ? 1 : 0;
        if (RG1L != 0)
        {
          RG3L = signedlong(vql_00.peek16());
          RG1P = DOWNCAST(s_www, DM2_1031_024c(RG52p));
          RG2L = vl_18;
          RG4L = RG7L;
          RG1L = DM2_1031_0a88(RG1P, RG4L, RG2L, RG3L);
          skip00181 = true;
        }
      }
      else
      {
        RG3L = signedlong(vql_00.peek16());
        RG2L = signedlong(vql_04.peek16());
        RG4L = signedlong(vql_08.peek16());
        RG1L = DM2_1031_030a(RG52p, RG4L, RG2L, RG3L);
        skip00181 = true;
      }

      if (skip00181)
      {
        //m_1051B:
        RG3L = RG1L;
        if (RG1W != 0)
          return RG3L;
      }
    }
    //m_10522:
    RG6p++;
    RG1Blo = byte_at(RG6p) & lcon(0xffffff80);
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 0)
      return RG3L;
  }
  FOREND
}

void DM2_1031_04F5(void)
{
  if (ddat.v1e03a8 != 0)
  {
    ddat.v1e03a8 = 0;
    DM2_guidraw_29ee_000f();
  }
}

void DM2_10777(void)
{
  ddat.vcapture1 = ddat.vcapture2 = ddat.vcapture3 = false;
  ddat.v1e03a8 = 0;
  ddat.v1e048c = 0;
  ddat.v1e0478 = 0;
  eventdata.v_evtable = NULL;
  DM2_CHAMPION_SQUAD_RECOMPUTE_POSITION();
  DM2_RELEASE_MOUSE_CAPTURES();
}

i32 DM2_107B0(void)
{
  return DM2_1031_0541(ddat.v1d3ff1);
}

i32 DM2_1031_0541(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG51l = 0;
  i32 RG52l = 0;
  ui32 RG6ul = 0;

  RG4W = RG1W;
  if (RG1W != ddat.v1d3ff1)
    eventqueue.event_1031_098e();
  ddat.v1d3ff1 = RG4W;
  DM2_1031_027e(&table1d3ed5[RG4W]);
  RG1P = DOWNCAST(s_wwwb, table1d3d23);
  RG2L = 0;	// SPX: seems to serve as loop counter
  //m_1070A:
  for (;;)
  {
LOGX(("DM2_1031_0541:[%03d]\n", RG2L));
_DEBUG_SHOW_BIGPOOL();
	RG4L = signedlong(RG2W);
    if (RG4L >= lcon(0x3e))
    {
      break;
    }
    //m_1067E:
    RG4L = unsignedlong(byte_at(RG1P, lcon(0x6)));
    RG3L = RG4L;
    RG3Bhi ^= RG4Bhi;
    RG3Blo &= lcon(0x40);
    RG3L = signedlong(RG3W);
    RG3Blo = RG3L == 0 ? 1 : 0;
    RG51l = RG3L & lcon(0xff);
    RG3L = RG4L;
    RG3Bhi ^= RG4Bhi;
    RG3Blo &= lcon(0xffffff80);
    RG3L = signedlong(RG3W);
    RG3Blo = RG3L == 0 ? 1 : 0;
    RG3L = unsignedlong(RG3Blo);
    if (RG51l != RG3L)
    {
      RG3L = RG4L;
      RG3Bhi ^= RG4Bhi;
      RG3Blo &= lcon(0xffffff80);
      RG3L = signedlong(RG3W);
      RG4W &= lcon(0x3f);
      if (RG3L == 0)
      {
        //m_106E6:
        if (RG4W != 0)
        {
          RG3L = signedlong(RG4W) - 1;
          RG4L = 5 * RG3L;
          table1d32d8[RG3L].b_03 |= 0x10; // EVIL index shifted, was 0x1d32db
        }
        or8(location(RG1P + lcon(0x6)), lcon(0xffffff80));
      }
      else
      {
        if (RG4W != 0)
        {
          RG4L = signedlong(RG4W);
          RG3L = RG4L - 1;
          RG4L = 5 * RG3L;
          table1d32d8[RG3L].b_03 |= 0x20; // EVIL index shifted, was 0x1d32db
        }
        and8(location(RG1P + lcon(0x6)), lcon(0x7f));
      }
    }
    //m_10702:
    and8(location(RG1P + lcon(0x6)), lcon(0xffffffbf));


    RG1P += lcon(0x7);
    RG2L++;
  }

  c_clickrectnode* node = table1d32d8;
  RG2L = 0;
  //m_1076A:
  for (;;)
  {
    RG1L = signedlong(RG2W);
    if (RG1L >= lcon(0x12))
      break;
    //m_1071F:
    RG1Blo = node->b_03 & 0x10;
    RG1L = unsignedlong(RG1Blo);
    RG3Blo = node->b_03 & 0x20;
    RG6ul = unsignedlong(RG3Blo);
    RG3Blo = RG1L == 0 ? 1 : 0;
    RG52l = RG3L & lcon(0xff);
    RG3Blo = RG6ul == 0 ? 1 : 0;
    RG3L = unsignedlong(RG3Blo);
    if (RG52l != RG3L)
    {
      if (RG1L == 0)
        //m_1075B:
        node->DM2_REFRESH_CLICKRECTLINK_1();
      else
        node->DM2_REFRESH_CLICKRECTLINK_2();
    }
    //m_10762:
    node->b_03 &= 0xcf;
    node++;
    RG2L++;
  }
  return RG1L;
}

i32 DM2_1031_0675(i16 eaxw)
{
  ddat.v1e0510 = ddat.v1d3ff1;
  DM2_10777();
  return DM2_1031_0541(eaxw);
}

i32 DM2_1031_06a5(void)
{
  return DM2_1031_0541(ddat.v1e0510);
}

// recursive!!!
unk* DM2_1031_06b3(s_bbw* eaxp, i32 edxl)
{
  c_nreg _RG1;
  c_nreg _RG2;
  s_bbw* rg3;
  c_nreg _RG4; RG4L = edxl;

  RG2L = RG4L;
  const i8* RG5p = DM2_1031_023b(eaxp);
  //m_10824:
  for (;;)
  {
    RG1L = unsignedlong(byte_at(RG5p));
    RG1Blo &= lcon(0x7f);
    rg3 = &table1d3ba0[RG1L];
    RG1Blo = eaxp->b_00 & lcon(0x7f);
    RG4L = unsignedlong(RG1Blo);
    if (gate_1031(RG4L + 5, rg3))
    {
      RG1Blo = rg3->b_00 & lcon(0xffffff80);
      RG1L = unsignedlong(RG1Blo);
      if (RG1L != lcon(0x80))
      {
        //m_1086F:
        RG1P = DOWNCAST(s_www, DM2_1031_024c(rg3));
        if (RG1P != NULL)
        {
          //m_1087A:
          for (;;)
          {
            RG4W = word_at(RG1P);
            RG4Bhi &= lcon(0x7);
            if (RG4W == 0)
              break;
            if (RG2W == RG4W)
              return RG1P;
            RG1P += 6;
          }
        }
      }
      else
      {
        RG4L = unsignedlong(RG2W);
        RG1P = DM2_1031_06b3(rg3, RG4L);
        if (RG1P != NULL)
          return RG1P;
      }
    }
    //m_1088F:
    RG5p++;
    RG1Blo = byte_at(RG5p) & lcon(0xffffff80);
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 0)
      return NULL;
  }
  FOREND
}

void DM2_1031_0781(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG4;
  c_rect rc_00;

  RG4L = unsignedlong(RG1W);
  RG1P = DM2_1031_06b3(&table1d3ed5[ddat.v1d3ff1], RG4L); // 1st par adj
  RG2P = RG1P;
  if (RG1P != NULL)
  {
    RG1L = signedlong(word_at(RG1P, 2));
    RG4R = &rc_00;
    RG1R = DM2_1031_01d5(RG1W, RG4R);
    if (RG1R != NULL)
    {
      RG1W = word_at(RG2P, lcon(0x4));
      RG1W = unsignedword(RG1Blo);
      RG2L = signedlong(RG1W);
      eventqueue.QUEUE_EVENT(rc_00.x, rc_00.y, RG2W);
    }
  }
}

void DM2_1031_07d6(void)
{
  c_nreg _RG1;
  s_bbw* rg1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 vwa_00[0x40];
  i16 vwa_80[0x12];
  i8 vba_a4[0x1c];

  RG1L = 0;
  RG4L = 0;
  //m_1092C:
  for (;;)
  {
    RG2L = signedlong(RG4W);
    if (mkul(RG2L) >= mkul(lcon(0x108)))
    {
      RG1L = 0;
      RG4L = 0;
      break;
    }
    //m_10907:
    RG3L = signedlong(RG4W);
    RG2L = signedlong(v1d338c[RG3L].w_00);
    if ((RG2L & lcon(0xffff8000)) != 0)
    {
      RG2L = signedlong(RG1W);
      vwa_00[RG2L] = RG4W;
      RG1L++;
    }
    RG4L++;
  }
  //m_1095F:
  for (;;)
  {
    RG2L = signedlong(RG4W);
    if (mkul(RG2L) >= mkul(lcon(0x79)))
    {
      RG1L = 0;
      RG4L = 0;
      break;
    }
    //m_1093D:
    RG2L = signedlong(v1d39bc[signedlong(RG4W)].w_00);
    if ((RG2L & lcon(0xffff8000)) != 0)
    {
      RG2L = signedlong(RG1W);
      vwa_80[RG2L] = RG4W;
      RG1L++;
    }
    RG4L++;
  }
  //m_1098D:
  for (;;)
  {
    RG2L = signedlong(RG4W);
    if (mkul(RG2L) >= mkul(lcon(0x53)))
    {
      RG4L = 0;
      break;
    }
    //m_1096D:
    RG2L = signedlong(RG4W);
    RG2Blo = table1d3cd0[RG2L] & lcon(0xffffff80);
    RG2L = unsignedlong(RG2Blo);
    if (RG2L != 0)
    {
      RG2L = signedlong(RG1W);
      vba_a4[RG2L] = RG4Blo;
      RG1L++;
    }
    RG4L++;
  }
  //m_109C1:
  for (;;)
  {
    RG1L = signedlong(RG4W);
    if (RG1L >= lcon(0x4c))
    {
      RG4L = 0;
      break;
    }
    //m_10999:
    rg1 = &table1d3ba0[RG1L];
    RG2Blo = rg1->b_00 & lcon(0xffffff80);
    RG2L = unsignedlong(RG2Blo);
    if (RG2L != 0)
    {
      RG2L = signedlong(rg1->w_02);
      RG2Blo = vba_a4[RG2L];
      RG2W = unsignedword(RG2Blo);
      rg1->w_02 = RG2W;
    }
    RG4L++;
  }
  //m_109F8:
  for (;;)
  {
    RG1L = signedlong(RG4W);
    if (mkul(RG1L) >= mkul(lcon(0xa)))
    {
      RG4L = 0;
      break;
    }
    //m_109CD:
    RG1P = DOWNCAST(s_bbw, &table1d3ed5[RG4W]); // adj
    RG2Blo = byte_at(RG1P) & lcon(0xffffff80);
    RG2L = unsignedlong(RG2Blo);
    if (RG2L != 0)
    {
      RG2L = signedlong(word_at(RG1P, 2));
      RG2Blo = vba_a4[RG2L];
      RG2W = unsignedword(RG2Blo);
      mov16(location(RG1P + 2), RG2W);
    }
    RG4L++;
  }
  //m_10A3A:
  for (;;)
  {
    RG2L = signedlong(RG4W);
    if (RG2L >= lcon(0x3e))
    {
      RG4L = 0;
      break;
    }
    //m_10A04:
    RG1P = DOWNCAST(s_wwwb, table1d3d23 + RG2L); // adj
    RG2L = signedlong(word_at(RG1P, 2));
    if (RG2L != lcon(0xffffffff))
    {
      RG2W = vwa_00[RG2L];
      mov16(location(RG1P + 2), RG2W);
    }
    RG2L = signedlong(word_at(RG1P, lcon(0x4)));
    if (RG2L != lcon(0xffffffff))
    {
      RG2W = vwa_80[RG2L];
      mov16(location(RG1P + lcon(0x4)), RG2W);
    }
    RG4L++;
  }
  //m_10A5A:
  for (;;)
  {
    RG1L = signedlong(RG4W);
    if (RG1L >= lcon(0x12))
      return;
    //m_10A46:
    RG2L = RG1L;
    table1d32d8[RG2L].DM2_ALLOC_CLICKRECTDATA();
    RG4L++;
  }
  FOREND
}

i32 DM2_1031_0a88(unk* xeaxp, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p;
  i16 RG6w;
  i8* RG7p;
  i32 vl_00;
  i16 vw_04;

  RG5p = RG1P;
  RG6w = RG4W;
  vw_04 = RG2W;
  if (RG1P == NULL)
    return 0;
  RG4L = 0;
  vl_00 = RG4L;
  RG7p = RG1P;
  //m_10BD3:
  for (;;)
  {
    if (jz_test8(location(RG5p + lcon(0x5)), lcon(0x8)))
    {
      RG1W = word_at(RG5p, lcon(0x4));
      RG1W = unsignedword(RG1Blo);
      RG1L = signedlong(RG1W);
      RG4L = signedlong(RG3W);
      if ((RG1L & RG4L) != 0)
      {
        RG1L = signedlong(word_at(RG5p, 2));
        RG4R = &glblrects.dm2rect4;
        RG1R = DM2_1031_01d5(RG1W, RG4R);
        if (RG1R != NULL)
        {
          RG2L = signedlong(vw_04);
          RG4L = signedlong(RG6w);
          RG1R = &glblrects.dm2rect4;
          RG1L = glblrects.dm2rect4.pt_in_rect(RG4W, RG2W) ? 1 : 0;
          if (RG1L != 0)
          {
            RG1W = word_at(RG5p, 2);
            eventqueue.event_unk05 = RG1W;
            RG1L = signedlong(eventqueue.event_unk05);
            if ((RG1Bhi & bcon(0x80)) == 0)
            {
              //m_10C44:
              RG1W = eventqueue.event_unk05 & lcon(0x4000);
              RG1L = signedlong(RG1W);
              if (RG1L == 0)
                //m_10C5F:
                eventqueue.event_unk09 = lcon(0xffffffff);
              else
                eventqueue.event_unk09 = lcon(0x12);
            }
            else
              eventqueue.event_unk09 = lcon(0x7);
            //m_10C68:
            eventqueue.event_unk05 &= lcon(0x3fff);
            eventqueue.event_unk08 = RG6w;
            RG1L = unsignedlong(vw_04);
            eventqueue.event_unk07 = RG1W;
            RG1W = word_at(RG5p);
            RG1Bhi &= lcon(0x7);
            vl_00 = RG1L;
            eventqueue.event_unk0a = RG1W;
            RG3L = 0;
            ddat.v1e051e = RG3W;
            RG1W = word_at(RG7p, 0);
            RG1Bhi &= lcon(0x7);
            RG1L--;
            RG4L = vl_00 - RG1L;
            eventqueue.event_unk06 = RG4W;
            return vl_00;
          }
        }
      }
    }
    //m_10CAE:
    RG5p += 6;
    RG1L = signedlong(word_at(RG5p));
    if ((RG1L & lcon(0xffff8000)) != 0)
    {
      RG2L = 0;
      glblrects.dm2rect4.w = glblrects.dm2rect4.h = RG2W;
      return vl_00;
    }
  }
  FOREND
}

void DM2_1031_0c58(i16 eaxw, s_www* xedxp)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4P = DOWNCAST(s_www, xedxp);
  i8* RG5p;

  RG2P = RG4P;
  eventqueue.event_unk09 = lcon(0xffffffff);
  RG5p = RG2P;
  if (RG2P != NULL)
  {
    //m_10E05:
    for (;;)
    {
      RG3W = word_at(RG2P);
      RG3Bhi &= lcon(0x7);
      if (RG3W == 0)
        break;
      if (jz_test8(location(RG2P + lcon(0x5)), lcon(0x8)) && RG3W == RG1W)
      {
        RG1L = signedlong(word_at(RG2P, 2));
        RG4R = &glblrects.dm2rect4;
        DM2_1031_01d5(RG1W, RG4R);
        RG1W = word_at(RG2P, 2);
        eventqueue.event_unk05 = RG1W;
        RG1L = signedlong(eventqueue.event_unk05);
        if ((RG1Bhi & bcon(0x80)) == 0)
        {
          //m_10E58:
          RG1W = eventqueue.event_unk05 & lcon(0x4000);
          RG1L = signedlong(RG1W);
          if (RG1L != 0)
            eventqueue.event_unk09 = lcon(0x12);
        }
        else
          eventqueue.event_unk09 = lcon(0x7);
        //m_10E71:
        eventqueue.event_unk05 &= lcon(0x3fff);
        RG1W = glblrects.dm2rect4.x;
        eventqueue.event_unk08 = RG1W;
        RG1W = glblrects.dm2rect4.y;
        eventqueue.event_unk07 = RG1W;
        eventqueue.event_unk0a = RG3W;
        RG1W = word_at(RG5p);
        RG1Bhi &= lcon(0x7);
        RG1L--;
        RG3L -= RG1L;
        eventqueue.event_unk06 = RG3W;
        return;
      }
      //m_10EB0:
      RG2P += 6;
    }
    //m_10EB8:
    glblrects.dm2rect4.w = glblrects.dm2rect4.h = RG3W;
  }
  //m_10EC6:
  eventqueue.event_unk05 = lcon(0xffffffff);
  RG2L = 0;
  eventqueue.event_unk08 = RG2W;
  eventqueue.event_unk07 = RG2W;
  eventqueue.event_unk0a = RG1W;
  eventqueue.event_unk06 = RG2W;
}

void DM2_1031_10c8(c_buttongroup* buttongroup, c_rect* edxrp, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_rect* RG6r; RG6r = edxrp;
  i16 vw_00;

  vw_00 = RG2W;
  if (buttongroup->button.dbidx == NODATA)
  {
    RG1R = &eventdata.v1e04d2.r;
    buttongroup->button.r = *RG1R; // copy
    DM2_guidraw_0b36_0c52(buttongroup, lcon(0xffffffff), 0);
  }
  RG3L = signedlong(RG3W);
  RG6r->calc_centered_rect_in_rect(&buttongroup->button.r, vw_00, RG3W);
}

// was SKW_1031_16fd
void DM2_CLICK_MAGICAL_MAP_AT(i16 eaxw, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i8* RG51p;
  i16 RG52w;
  i16 RG53w;
  c_wlreg _RG6;
  i32 RG71l;
  i32 RG72l;
  i16 vw_00;
  c_rect rc_04;
  unk* xp_0c;
  i32 vl_10;
  i16 vw_14;
  i16 vw_18;
  i16 vw_1c;

  vw_00 = RG1W; // truncated
  RG3L = RG4L;
  RG4L = signedlong(party.curacthero) - 1;
  RG1L = 263 * RG4L;
  RG1W = party.hero[RG4L].item[party.curactmode];
  vw_1c = RG1W;
  RG6L = unsignedlong(RG1W);
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG6UW));
  RG51p = RG1P;
  xp_0c = UPCAST(unk, RG1P);
  if (vw_00 != lcon(0x55))
    return;
  RG1W = word_at(RG1P, lcon(0x4)) & lcon(0xe000);
  if (RG1W != lcon(0x2000))
    return;
  RG4R = &rc_04;
  RG1L = lcon(0x63);
  DM2_QUERY_EXPANDED_RECT(RG1W, RG4R);
  RG1W = rc_04.x - ddat.v1e0b5a; // adj
  RG3W -= RG1W; // adj
  RG1W = rc_04.y - ddat.v1e0b58; // adj
  RG2L -= RG1L;
  RG1L = signedlong(ddat.v1d274a);
  RG71l = signedlong(ddat.v1d274e);
  RG1L += RG71l;
  vl_10 = RG1L;
  RG4L = signedlong(RG3W);
  RG1L = RG4L;
  RG4L = RG1L % vl_10;
  RG1L /= vl_10;
  if (RG4L < RG71l)
    return;
  RG1L = signedlong(ddat.v1d274c);
  RG72l = signedlong(ddat.v1d2750);
  RG1L += RG72l;
  vl_10 = RG1L;
  RG4L = signedlong(RG2W);
  RG1L = RG4L;
  RG4L >>= bcon(0x1f);
  RG4L = RG1L % vl_10;
  RG1L /= vl_10;
  if (RG4L < RG72l)
    return;
  RG1P = DM2_GET_MISSILE_REF_OF_MINION(unsignedlong(word_at(RG51p, 2)), RG6L);
  if (RG1P == NULL)
    return;
  RG1W = ddat.v1d274a;
  RG52w = ddat.v1d274e + RG1W;
  RG1L = RG3L;
  RG4W = RG1W % RG52w;
  RG1W /= RG52w;
  RG3L = RG1L;
  RG1L = RG2L;
  RG1W /= ddat.v1d274c + ddat.v1d2750;
  RG2L = RG1L;
  vw_18 = signedword(ddat.v1e0b82);
  vw_14 = signedword(ddat.v1e0b83);
  RG3L -= 3;
  RG1L = signedlong(RG3W);
  RG4L = signedlong(CUTX16(lcon(0x3) - RG2L));
  DM2_CALC_VECTOR_W_DIR(signedword(ddat.v1e0b84), RG4W, RG1W, &vw_18, &vw_14);
  RG1L = DM_LOCATE_OTHER_LEVEL(signedlong(ddat.v1e0b81), 0, &vw_18, &vw_14, NULL);
  RG2L = RG1L;
  if (RG1W < 0)
    return;
  RG53w = ddat.v1d3248;
  RG1L = signedlong(RG1W);
  DM2_CHANGE_CURRENT_MAP_TO(RG1L);
  RG4L = signedlong(vw_14);
  RG1L = dm2_GET_TILE_VALUE(signedlong(vw_18), RG4L);
  RG4W = unsignedword(RG4Blo);
  RG4Blo = RG1Blo;
  RG1L = unsignedlong(RG1Blo) >> 5;
  RG1W = unsignedword(RG1Blo);
  if (RG1W != 0)
  {
    RG1L = unsignedlong(RG1W);
    if (RG1L != lcon(0x7))
    {
      bool skip00196 = false;
      if (RG1L != lcon(0x6))
        skip00196 = true;
      else
      {
        RG1L = RG4L;
        RG1Blo &= 0x5;
        RG1L = unsignedlong(RG1W);
        if (RG1L != 0)
          skip00196 = true;
      }

      if (skip00196)
        //m_11CE1:
        RG1L = 1;
      //m_11CE6:
      if (RG1L != 0)
      {
        RG1W = vw_18; // adj
        if (RG1W == ddat.v1e0b6e)
        {
          RG1W = vw_14; // adj
          if (RG1W == ddat.v1e0b70 && RG2W == ddat.v1e0b64)
          {
            bool skip00198 = false;
            if (ddat.v1e0b56 != 1)
              skip00198 = true;
            else
            {
              RG4W = ddat.v1e0b60;
              if (RG4W < 0)
                skip00198 = true;
              else
              {
                vw_18 = ddat.v1e0b68;
                vw_14 = ddat.v1e0b6a;
                RG2L = RG4L;
              }
            }

            if (skip00198)
            {
              //m_11D4D:
              if (ddat.v1e0b66 >= wcon(0x0))
              {
                if (ddat.v1e0b56 == wcon(0x1) || ddat.v1e0b56 == wcon(0x3))
                {
                  vw_18 = ddat.v1e0b5e;
                  vw_14 = ddat.v1e0b5c;
                  RG2W = ddat.v1e0b66;
                }
              }
            }
          }
        }
        //m_11D87:
        RG3L = signedlong(RG2W);
        DM2_SET_DESTINATION_OF_MINION_MAP(unsignedlong(vw_1c), signedlong(vw_18), signedlong(vw_14), RG3L);
        RG1L = 0;
        RG1W = word_at(DOWNCAST(unk, xp_0c), 2);
        DM2_1c9a_0247(RG1L);
        DM2_UPDATE_RIGHT_PANEL(0);
      }
    }
  }
  //m_11DBA:
  RG1L = signedlong(RG53w);
  DM2_CHANGE_CURRENT_MAP_TO(RG1L);
}
