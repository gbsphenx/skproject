#include <util.h>
#include <startend.h>
#include <skrandom.h>
#include <dm2data.h>
#include <skhero.h>
#include <skalocdb.h>
#include <sklodlvl.h>
#include <skrecord.h>
#include <skmovrec.h>
#include <skgdtqdb.h>
#include <skmap.h>

#include <dm2debug.h>

// TODO: a lot of type-adjustment todo, search for t_tile
// also search for mapdat.map which delivers values without conversion atm

#define TILE_00 0x0
#define TILE_01 0x1
#define TILE_02 0x2
#define TILE_04 0x4
#define TILE_08 0x8
#define TILE_E0 0xe0

c_mapdata mapdat;

ui32 tile_to_ulong(t_tile n)
{
  return (ui32)n;
}

ui8 tile_to_ubyte(t_tile n)
{
  return (ui8)n;
}

void c_mapdata::init(void)
{
	SPX_DEBUG_PRINT("MAPDATA:INIT\n");
  map_width = 0;
  map_height = 0;
  map = NULL;
  tmpmap = NULL;
}

// TODO: reveal the meaning of ddat.v1e03f4 and the meaning of the bitmask 0x10
i16 DM2_GET_OBJECT_INDEX_FROM_TILE(i16 x, i16 y)
{
  t_tile* ptr = mapdat.map[x];

  if (   x < 0 || x >= mapdat.map_width
      || y < 0 || y >= mapdat.map_height
      || (ptr[y] & 0x10) == 0)
    return -1;

  i16 idx = ddat.v1e03f4[x];
  while (--y >= 0)
    if ((*ptr++ & 0x10) != 0)
      idx++;
  return idx;
}

// TODO: table-meaning of v1e038c? (spx: dunGroundStacks)
t_record DM2_GET_TILE_RECORD_LINK(i16 x, i16 y)
{
  i16 idx = DM2_GET_OBJECT_INDEX_FROM_TILE(x, y);
  if (idx == -1)
    return OBJECT_END_MARKER;
  return dm2_v1e038c[idx];
}

t_record* DM2_GET_ADDRESS_OF_TILE_RECORD(i16 x, i16 y)
{
  return DM2_GET_ADDRESS_OF_RECORD(DM2_GET_TILE_RECORD_LINK(x, y));
}

// TODO optimize temporaries
#include "regs.h"
#include "emu.h"

// TODO bool return, i16 coords?
static i32 DM2_IS_TILE_PASSAGE(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  RG1L = signedlong(RG1W);
  RG3L = RG1L;
  RG4L = signedlong(RG4W);
  RG2Blo = mapdat.map[RG3L][RG4W];
  RG2L = unsignedlong(RG2Blo) >> 5;
  RG3Blo = RG2Blo;
  RG2W = unsignedword(RG2Blo);
  RG3L = unsignedlong(RG3Blo);
  if (RG3L == lcon(0x5))
  {
    RG1RP = DM2_GET_ADDRESS_OF_TILE_RECORD(RG1W, RG4W);
    if (jnz_test8(location(RG1P + lcon(0x4)), 1))
      return 0;
  }
  if (RG2W == 0)
    return 0;
  RG1L = unsignedlong(RG2W);
  return RG1L == lcon(0x7) ? 0 : 1;
}

// TODO i16 coords?
t_tile DM2_GET_TILE_VALUE(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG52l;
  i32 RG53l;
  t_tile vti_00; //vq_00; // poke 16 peek 8,16 hiword unused
  t_tile vti_04; //vq_04; // poke 16 peek 8,16 hiword unused
  bool flag;
  bool skip00119 = false;
  bool skip00118 = false;
  bool skip00117 = false;

  RG2L = RG1L;
  RG3L = RG4L;
  if (RG4W < 0 || RG3W >= mapdat.map_height)
    //m_CB66:
    vti_04 = TILE_00;
  else
    vti_04 = TILE_01;
  //m_CB68:
  if (RG2W < 0 || RG2W >= mapdat.map_width)
    //m_CB82:
    flag = false;
  else
    flag = true;
  //m_CB84:
  vti_00 = flag ? TILE_01 : TILE_00;
  if (flag)
  {
    if (vti_04 != TILE_00)
    {
      RG4L = signedlong(RG3W);
      return mapdat.map[RG2W][RG4L];
    }
  }
  RG1L = RG2L - 1;
  if (vti_04 == TILE_00)
  {
    //m_CC43:
    if (vti_00 == TILE_00)
    {
      bool skip00115 = false;
      //m_CCBB:
      if (RG2W != lcon(0xffffffff))
      {
        //m_CCC5:
        if (RG2W == mapdat.map_width)
        {
          RG2L = RG1L;
          skip00115 = true;
        }
      }
      else
      {
        RG2L = 0;
        skip00115 = true;
      }

      if (skip00115)
      {
        bool skip00116 = false;
        //m_CCD0:
        if (RG3W != lcon(0xffffffff))
          skip00116 = true;
        else
        {
          RG1L = DM2_IS_TILE_PASSAGE(signedlong(RG2W), 0);
          if (RG1L == 0)
            skip00116 = true;
        }

        if (skip00116)
        {
          //m_CCE8:
          if (RG3W == mapdat.map_height)
          {
            RG3L--;
            skip00117 = true;
          }
        }
        else
          skip00119 = true;
      }
    }
    else
    {
      bool skip00113 = false;
      if (RG3W != lcon(0xffffffff))
      {
        //m_CC5F:
        if (RG3W == mapdat.map_height)
        {
          vti_04 = TILE_08;
          RG3L--;
          skip00113 = true;
        }
      }
      else
      {
        RG3L = 0;
        vti_04 = TILE_02;
        skip00113 = true;
      }

      if (skip00113)
      {
        bool skip00114 = false;
        //m_CC74:
        RG52l = signedlong(RG3W);
        RG1L = DM2_IS_TILE_PASSAGE(signedlong(RG2W), RG52l);
        if (RG1L != 0)
          return vti_04;
        if (RG2W <= 0)
          skip00114 = true;
        else
        {
          RG1L = DM2_IS_TILE_PASSAGE(signedlong(CUTX16(RG2L - 1)), RG52l);
          if (RG1L == 0)
            skip00114 = true;
        }

        if (skip00114)
        {
          //m_CCA2:
          RG1L = signedlong(mapdat.map_width);
          RG4L = signedlong(RG2W) + 1;
          if (RG4L < RG1L)
          {
            RG4L = signedlong(RG3W);
            RG2L++;
            skip00118 = true;
          }
        }
        else
          skip00119 = true;
      }
    }
  }
  else
  {
    bool skip00111 = false;
    if (RG2W != lcon(0xffffffff))
    {
      //m_CBCF:
      if (RG2W == mapdat.map_width)
      {
        vti_00 = TILE_01;
        RG2L = RG1L;
        skip00111 = true;
      }
    }
    else
    {
      RG2L = 0;
      vti_00 = TILE_04;
      skip00111 = true;
    }

    if (skip00111)
    {
      bool skip00112 = false;
      //m_CBE4:
      RG4L = signedlong(RG3W);
      RG53l = signedlong(RG2W);
      RG1L = DM2_IS_TILE_PASSAGE(RG53l, RG4L);
      if (RG1L != 0)
        return vti_00;
      if (RG3W <= 0)
        skip00112 = true;
      else
      {
        RG1L = RG3L - 1;
        RG4L = signedlong(RG1W);
        RG1L = DM2_IS_TILE_PASSAGE(RG53l, RG4L);
        if (RG1L == 0)
          skip00112 = true;
      }

      if (skip00112)
      {
        //m_CC13:
        RG1L = signedlong(RG3W);
        RG4L = signedlong(mapdat.map_height);
        RG1L++;
        if (RG1L < RG4L)
        {
          RG3L++;
          skip00117 = true;
        }
      }
      else
        skip00119 = true;
    }
  }

  if (skip00117)
  {
    //m_CC29:
    RG4L = signedlong(RG3W);
    skip00118 = true;
  }

  if (skip00118)
  {
    //m_CC2C:
    RG1L = DM2_IS_TILE_PASSAGE(signedlong(RG2W), RG4L);
    if (RG1L != 0)
      skip00119 = true;
  }

  if (skip00119)
    //m_CC3C:
    return TILE_00;

  //m_CCF7:
  return TILE_E0;
}

void DM2_CHANGE_CURRENT_MAP_TO(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  bool skip00108 = false;

  if (RG1W == ddat.v1d3248 || RG1W < 0)
    return;
  ddat.v1d3248 = RG1W;
  RG2L = signedlong(RG1W);
  RG4P = pointer_at(DOWNCAST(t_tile*, mapdat.tmpmap) + 4 * RG2L);
  mapdat.map = UPCAST(t_tile*, RG4P);
  RG4L = 16 * RG2L;
  RG3P = ddat.v1e03c8 + RG4L;
  ddat.v1e03c0 = UPCAST(s_sizee, RG3P);
  RG4P = RG3P;
  RG3W = word_at(RG3P, lcon(0x8));
  RG3L <<= bcon(0x5);
  _RG3.ushiftr11();
  RG3L = unsignedlong(RG3W) + 1;
  mapdat.map_width = RG3W;
  RG4W = word_at(RG4P, lcon(0x8));
  _RG4.ushiftr11();
  RG4L = unsignedlong(RG4W) + 1;
  mapdat.map_height = RG4W;
  RG4L = 2 * unsignedlong(ddat.v1e03e4[RG2L]);
  ddat.v1e03f4 = XUPCAST(i16, ddat.v1e03d8 + RG4L);
  if (ddat.v1e0234 != 0)
  {
    RG2W = ddat.v1e027c;
    if (RG1W == RG2W)
    {
      ddat.v1e0280 = ddat.v1e0260;
      ddat.v1e027e = ddat.v1e0262;
      ddat.v1e0282 = RG2W;
      RG1W = party.absdir;
      skip00108 = true;
    }
  }

  if (!skip00108)
  {
    //m_C8AA:
    ddat.v1e0280 = ddat.v1e0270;
    ddat.v1e027e = ddat.v1e0272;
    ddat.v1e0282 = ddat.v1e0266;
    RG1W = ddat.v1e0258;
  }

  //m_C8D4:
  ddat.v1e0264 = RG1W;
}

// stacksize was 0x2c
i32 DM_LOCATE_OTHER_LEVEL(i32 eaxl, i32 edxl, i16* ebxpw, i16* ecxpw, unk** argpp0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = DOWNCAST(i16, ebxpw);
  c_nreg _RG3; RG3P = DOWNCAST(i16, ecxpw);
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG51; // probably 2 variables, but highword critical
  i8* RG52p;
  i16 RG53w;
  i32 RG6l;
  i32 RG71l;
  c_lreg _RG72;
  i32 RG73l;
  i32 RG74l;
  i16* wp_00;
  i16* wp_04;
  i16 vw_08;
  i16 vw_0c;
  i16 vw_10;
  i16 vw_14;
  i16 vw_18;

  wp_04 = RG2P16;
  wp_00 = RG3P16;
  RG1L = signedlong(RG1W);
  RG2L = 16 * RG1L;
  RG1P = ddat.v1e03c8 + RG2L;
  RG2L = unsignedlong(byte_at(RG1P, lcon(0x6)));
  RG3P = DOWNCAST(i16, wp_04);
  RG3W = word_at(RG3P);
  RG3L += RG2L;
  vw_10 = RG3W;
  RG2L = unsignedlong(byte_at(RG1P, lcon(0x7)));
  RG3P = DOWNCAST(i16, wp_00);
  RG3W = word_at(RG3P);
  RG3L += RG2L;
  vw_08 = RG3W;
  RG1W = word_at(RG1P, lcon(0x8)) & lcon(0x3f);
  RG2L = unsignedlong(RG1W);
  RG1L = signedlong(RG4W) + RG2L;
  if (RG1W >= 0)
  {
    RG1L = signedlong(RG1W);
    if (RG1L < lcon(0x3f))
    {
      RG1P = ddat.v1e03f0 + RG1L;
      RG4L = unsignedlong(byte_at(RG1P));
      RG1Blo = RG4Blo;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L != lcon(0xffffffff))
      {
        bool skip00122 = false;
        RG2P = DOWNCAST(unk*, argpp0);
        if (RG2P != NULL)
        {
          RG3P = pointer_at(RG2P);
          if (RG3P != NULL)
          {
            RG2P = RG3P + 1;
            skip00122 = true;
          }
        }

        if (!skip00122)
          //m_CDBD:
          RG2P = ddat.v1e03dc + signedlong(RG4W);

        //m_CDC7:
        for (;;)
        {
          RG1W = signedword(byte_at(RG2P));
          vw_14 = RG1W;
          RG1L = signedlong(byte_at(RG2P));
          if (RG1L == lcon(0xffffffff))
            break;
          RG6l = signedlong(vw_14);
          RG4L = 16 * RG6l;
          RG1P = ddat.v1e03c8 + RG4L;
          RG3L = unsignedlong(byte_at(RG1P, lcon(0x6)));
          RG4L = unsignedlong(RG3Blo);
          RG71l = signedlong(vw_10);
          RG4L--;
          if (RG71l >= RG4L)
          {
            RG51W = word_at(RG1P, lcon(0x8));
            RG51L <<= bcon(0x5);
            RG51UW >>= bcon(0xb);
            RG72L = unsignedlong(RG51W);
            RG4L = signedlong(RG3W) + RG72L;
            RG73l = signedlong(vw_10);
            RG4L++;
            if (mkul(RG73l) <= mkul(RG4L))
            {
              RG51W = unsignedword(byte_at(RG1P, lcon(0x7)));
              RG4L = unsignedlong(byte_at(RG1P, lcon(0x7)));
              RG4L--;
              if (signedlong(vw_08) >= RG4L)
              {
                RG1W = word_at(RG1P, lcon(0x8));
                _RG1.ushiftr11();
                RG1L = unsignedlong(RG1W);
                RG4L = signedlong(RG51W);
                RG1L += RG4L;
                RG4L = signedlong(vw_08);
                RG1L++;
                if (mkul(RG4L) <= mkul(RG1L))
                {
                  RG1L = unsignedlong(vw_10) - RG3L;
                  RG3L = RG1L;
                  RG1L = unsignedlong(vw_08) - RG51L;
                  RG53w = RG1W;
                  vw_0c = ddat.v1d3248;
                  DM2_CHANGE_CURRENT_MAP_TO(RG6l);
                  RG74l = signedlong(RG53w);
                  RG6l = signedlong(RG3W);
                  RG1L = (dm2_GET_TILE_VALUE(RG6l, RG74l) & lcon(0xff)) >> 5;
                  RG4W = unsignedword(RG4Blo);
                  RG4Blo = RG1Blo;
                  vw_18 = RG4W; // truncated
                  RG1L = unsignedlong(RG1Blo);
                  if (RG1L == lcon(0x5))
                  {
                    RG4L = RG74l;
                    RG1RP = DM2_GET_ADDRESS_OF_TILE_RECORD(CUTX16(RG6l), RG4W);
                    if (jnz_test8(location(RG1P + lcon(0x4)), 1))
                      vw_18 = 7;
                  }
                  DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_0c));
                  RG1L = unsignedlong(vw_18);
                  if (RG1L != lcon(0x7))
                  {
                    mov16(location(DOWNCAST(i16, wp_04)), RG3W);
                    mov16(location(DOWNCAST(i16, wp_00)), RG53w);
                    RG3P = DOWNCAST(unk*, argpp0);
                    if (RG3P != NULL)
                      movp(location(RG3P), RG2P);
                    RG1L = unsignedlong(vw_14);
                    return RG1L;
                  }
                }
              }
            }
          }
          //m_CEFE:
          RG2P++;
        }
      }
    }
  }
  //m_CF04:
  RG52p = DOWNCAST(unk*, argpp0);
  if (RG52p != NULL)
    mov32(location(RG52p), 0);
  return lcon(0xffffffff);
}

i32 DM2_SET_DESTINATION_OF_MINION_MAP(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  c_wlreg _RG6;
  i16 vw_00;

  RG6L = RG1L;
  vw_00 = ddat.v1d3248;
  RG1L = signedlong(RG3W);
  DM2_CHANGE_CURRENT_MAP_TO(RG1L);
  if (RG4W < 0 || RG4W >= mapdat.map_width || RG2W < 0 || RG2W >= mapdat.map_height)
    //m_FBB5:
    RG1L = 0;
  else
    RG1L = 1;
  //m_FBB7:
  RG5l = RG1L;
  if (RG1L != 0)
  {
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG6UW));
    RG4W &= lcon(0x1f);
    and8(location(RG1P + lcon(0x6)), lcon(0xffffffe0));
    RG6W = word_at(RG1P, lcon(0x6));
    RG6L |= RG4L;
    RG2W &= lcon(0x1f);
    RG4L = RG6L & lcon(0xfffffc1f);
    RG2L <<= bcon(0x5);
    mov16(location(RG1P + lcon(0x6)), RG4W | RG2W);
    RG4L = RG3L;
    RG4Bhi ^= RG3Bhi;
    RG4Blo &= lcon(0x3f);
    and8(location(RG1P + lcon(0x7)), lcon(0x3));
    RG4L <<= bcon(0xa);
    or16(location(RG1P + lcon(0x6)), RG4W);
  }
  DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_00));
  return RG5l;
}

i32 DM2_map_0cee_1815(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  c_lreg _RG6;

  RG5w = RG4W;
  if (RG1W != 0)
  {
    RG4L = unsignedlong(argw0);
    RG1W = ddat.v1d3248;
    RG1L <<= bcon(0x6);
    RG1L += lcon(0xbb8);
    RG1W += mapdat.map_width;
    RG1W += mapdat.map_height;
    RG6L = unsignedlong(RG1W);
    RG1L = DM2_map_0cee_17e7(32 * RG2L + lcon(0x7d0) + RG3L & lcon(0xffff), RG6L, RG4L);
    if (RG1W < RG5w)
    {
      RG1L = signedlong(RG1W);
      RG1Blo = ddat.v1e02cc[RG1L];
      return RG1L;
    }
  }
  RG1Blo = lcon(0xffffffff);
  return RG1L;
}

// stacksize was 0x1c
void DM2_map_0cee_185a(unk* xeaxp, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1, i16 argw2, i16 argw3)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p = xeaxp;
  i32 RG6l;
  i32 RG7l;
  i32 vl_00;
  i16 vw_04;
  i16 vw_08;

  vw_04 = RG2W;
  vw_08 = RG3W;
  RG2W = ddat.v1e03c0->w_0a;
  RG2L <<= bcon(0x8);
  RG2UW >>= bcon(0xc);
  RG1L = argl1;
  RG1W &= 0x3;
  RG3L = signedlong(RG1W);
  argw3++;
  vl_00 = signedlong(argw3);
  RG1L = (RG3L + 1) * vl_00;
  RG3L = signedlong(RG1W);
  RG6l = signedlong(argw2);
  RG7l = signedlong(RG2W);
  RG1L = DM2_map_0cee_1815(signedlong(RG4W), RG7l, RG6l, RG3L, lcon(0x1e));
  RG1W = unsignedword(RG1Blo);
  mov16(location(RG5p + lcon(0x6)), RG1W);
  RG1L = argl1 + 1;
  argl1 = RG1L;
  RG1W &= 0x3;
  RG1L = (signedlong(RG1W) + 1) * vl_00;
  RG3L = signedlong(RG1W);
  RG1L = DM2_map_0cee_1815(signedlong(vw_04), RG7l, RG6l, RG3L, lcon(0x1e));
  RG1W = unsignedword(RG1Blo);
  mov16(location(RG5p + lcon(0x8)), RG1W);
  RG4L = argl1 + 1;
  argl1 = RG4L;
  RG1L = RG4L;
  RG1Bhi ^= RG4Bhi;
  RG1Blo &= 0x3;
  RG1L = (signedlong(RG1W) + 1) * vl_00;
  RG3L = signedlong(RG1W);
  RG1L = DM2_map_0cee_1815(signedlong(vw_08), RG7l, RG6l, RG3L, lcon(0x1e));
  RG1W = unsignedword(RG1Blo);
  mov16(location(RG5p + lcon(0xa)), RG1W);
  RG2L = argl1 + 1;
  argl1 = RG2L;
  RG1L = RG2L;
  RG1Bhi ^= RG2Bhi;
  RG1Blo &= 0x3;
  RG1L = signedlong(RG1W);
  argw3--;
  RG1L++;
  RG1L *= vl_00;
  RG3L = signedlong(RG1W);
  RG1L = signedlong(argw0);
  RG2L = RG6l;
  RG4L = RG7l;
  RG1L = DM2_map_0cee_1815(RG1L, RG4L, RG2L, RG3L, lcon(0x1e));
  RG1W = unsignedword(RG1Blo);
  mov16(location(RG5p + lcon(0xc)), RG1W);
  if (argw2 >= 0 && argw2 < mapdat.map_width)
    //m_E439:
    RG1L = 0;
  else
    RG1L = 1;
  //m_E43B:
  if (RG1L == 0)
  {
    RG2W = argw3; // adj
    if (RG2W < 0 || RG2W >= mapdat.map_height)
      RG1L = 1;
    if (RG1L == 0)
      return;
  }
  RG2L = 3;
  //m_E47A:
  for (;;)
  {
    RG1L = signedlong(RG2W);
    if (RG1L > lcon(0x6))
      return;
    //m_E461:
    RG1L = 2 * RG1L;
    RG4P = RG5p + RG1L;
    RG1L = DM2_IS_WALL_ORNATE_ALCOVE(unsignedlong(byte_at(RG4P)));
    if (RG1W != 0)
      mov16(location(RG4P), lcon(0xff));
    RG2L++;
  }
  FOREND
}

// TODO: bool return
static i32 DM2_map_2066_1f37(i32 eaxl, i32 edxl, i16* ebxpw)
{
  c_nreg _RG4; RG4L = edxl;
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = DOWNCAST(i16, ebxpw);
  c_nreg _RG3;
  c_lreg _RG5; // TODO: bool
  c_wlreg _RG6;
  i32 vl_00;

  RG3L = RG1L;
  vl_00 = RG4L;
  RG5L = 0;
  //m_2627A:
  for (;;)
  {
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG3UW)));
    RG3L = RG1L;
    if (RG1W == lcon(0xfffffffe))
      return RG5L;
    RG4L = RG1L;
    RG4Blo ^= RG1Blo;
    RG4Bhi &= lcon(0x3c);
    RG4L = unsignedlong(RG4W) >> bcon(0xa);
    if (RG4L == lcon(0x3))
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
      RG4W = word_at(RG1P, 2) & lcon(0x7f);
      if (RG4W == lcon(0x27))
      {
        RG5L = 1;
        if (jz_test16(location(RG1P + 2), lcon(0xffffff80)))
        {
          RG4L = vl_00 + 1;
          RG4Bhi &= 0x1;
          RG6W = word_at(RG1P, 2);
          RG6L &= lcon(0xffff007f);
          RG4L <<= bcon(0x7);
          RG6L |= RG4L;
          mov16(location(RG1P + 2), RG6W);
          inc16(location(RG2P));
        }
      }
    }
  }
  FOREND
}

static i32 DM2_map_2066_1ec9(i32 eaxl, i32 edxl)
{
  c_nreg _RG4; RG4L = edxl;
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_wlreg _RG5;
  c_lreg _RG6;

  RG5L = RG1L;
  if (RG1W == lcon(0xfffffffe))
    return RG4L;
  //m_26200:
  if (RG4W == lcon(0xfffffffe))
    return RG5L;
  //m_26206:
  for (;;)
  {
    RG1Blo = RG4W != wcon(0xfffe) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 0)
    {
      RG1L = RG4L;
      RG1Blo ^= RG4Blo;
      RG1Bhi &= lcon(0x3c);
      RG1L = unsignedlong(RG1W) >> bcon(0xa);
      if (RG1L < lcon(0x4))
      {
        RG2L = RG5L;
        RG5L = RG4L;
        RG4L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG5UW));
        mov16(location(RG1P), RG2W);
        continue;
      }
    }
    break;
  }
  //m_26247:
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG5UW));
  RG6L = unsignedlong(RG4W);
  RG3L = lcon(0xffffffff);
  DM2_APPEND_RECORD_TO(mk_record(RG6L), UPCAST(t_record, RG1P), RG3W, RG3W);
  return RG5L;
}

void SKW_2066_1ea3(i32 eaxl, i32 ebxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  RG3L = RG1L;
  RG2L = signedlong(RG2W) << 2;
  RG1P = DOWNCAST(t_tile*, mapdat.tmpmap) + RG2L;
  RG2L = signedlong(RG3W) << 2;
  RG1P = pointer_at(RG1P) + RG2L;
  RG2L = signedlong(RG4W);
  RG1P = pointer_at(RG1P);
  or8(location(RG1P + RG2L), 2);
}

void DM2_ARRANGE_DUNGEON(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5;
  i8* RG61p;
  i32 RG7l;
  unk* xp_00; // add32
  unk* xp_04; // inc32
  i16 vw_08;
  i16 vw_0c;
  i16 vw_10;
  i16 vw_14;
  i16 vw_18;
  i16 vw_1c;
  c_o32 vo_20; // poke 16 peek 16 32 inc16
  c_o32 vo_24; // poke 16 peek 16 32 inc16
  c_o32 vo_28; // poke 16 peek 16 32 inc16
  i8 vb_2c;

  RG4L = 0;
  vo_24.poke16(RG4W);
  //m_2686A:
  for (;;)
  {
    RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
    RG1Blo = byte_at(RG1P, lcon(0x4));
    RG1L = unsignedlong(RG1Blo);
    RG4L = unsignedlong(vo_24.peek16());
    if (RG4L >= RG1L)
      return;
    //m_26315:
    RG1L = 16 * RG4L;
    RG2P = ddat.v1e03c8;
    RG1P = RG2P + RG1L;
    RG2W = word_at(RG1P, lcon(0x8));
    RG2L <<= bcon(0x5);
    _RG2.ushiftr11();
    vw_18 = RG2W; // truncated
    RG1W = word_at(RG1P, lcon(0x8));
    _RG1.ushiftr11();
    vw_1c = RG1W; // truncated
    RG1P = pointer_at(pointer_at(DOWNCAST(t_tile*, mapdat.tmpmap) + 4 * RG4L));
    xp_04 = UPCAST(unk, RG1P);
    RG1L = RG4L;
    RG1W = ddat.v1e03e4[RG1L];
    RG4L = 2 * (RG1L & lcon(0xffff));
    RG1P = ddat.v1e03d8;
    RG1W = word_at(RG1P, RG4L);
    RG4L = 2 * (RG1L & lcon(0xffff));
    xp_00 = UPCAST(unk, DOWNCAST(t_record, dm2_v1e038c + RG4L / 2));
    RG2L = 0;
    vo_28.poke16(RG2W);
    //m_2684F:
    for (;;)
    {
      RG4L = signedlong(vo_28.peek16());
      RG1L = unsignedlong(vw_18);
      if (RG4L > RG1L)
      {
        vo_24.inc16();
        break;
      }
      //m_2638D:
      RG5L = 0;
      vo_20.poke16(RG5W);
      //m_26834:
      for (;;)
      {
        RG4L = signedlong(vo_20.peek16());
        RG1L = unsignedlong(vw_1c);
        if (RG4L > RG1L)
        {
          vo_28.inc16();
          break;
        }
        //m_26399:
        RG1P = DOWNCAST(unk, xp_04);
        RG1Blo = byte_at(RG1P);
        vb_2c = RG1Blo;
        xp_04++; // DANGER
        if ((RG1Blo & bcon(0x10)) != 0)
        {
          RG5W = word_at(DOWNCAST(unk, xp_00));
          xp_00 += 2; // DANGER
          //m_263B9:
          for (;;)
          {
            RG1L = RG5L;
            RG1W &= lcon(0x3c00);
            RG1L = unsignedlong(RG1W) >> bcon(0xa);
            RG1L--;
            if (mkul(RG1L) <= mkul(lcon(0x3)))
            {
              bool skip00639 = false;
              bool skip00636 = false;
              bool skip00635 = false;
              switch (RG1L)
              {
                case 0:
                  //m_2665A:
                  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG5UW));
                  RG4P = RG1P;
                  RG61p = RG1P;
                  RG1W = word_at(RG1P, lcon(0x4));
                  RG1UW >>= bcon(0x8);
                  RG1L = signedlong(RG1W);
                  DM2_CHANGE_CURRENT_MAP_TO(RG1L);
                  RG1W = word_at(RG4P, 2);
                  RG1L <<= bcon(0x6);
                  _RG1.ushiftr11();
                  RG1L = signedlong(RG1W);
                  RG4W = word_at(RG4P, 2) & lcon(0x1f);
                  RG2L = signedlong(RG4W);
                  RG4L = RG1L;
                  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(RG2W, RG4W));
                  vw_14 = RG1W; // truncated
                  if (RG1W != lcon(0xfffffffe))
                  {
                    RG4L = RG1L;
                    RG4Blo ^= RG1Blo;
                    RG4Bhi &= lcon(0x3c);
                    RG4L = unsignedlong(RG4W) >> bcon(0xa);
                    if (RG4L == 1)
                    {
                      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
                      RG3P = RG1P;
                      RG4W = word_at(RG1P, 2) & lcon(0x1f);
                      if (RG4W == vo_28.peek16())
                      {
                        RG4W = word_at(RG1P, 2);
                        RG4L <<= bcon(0x6);
                        _RG4.ushiftr11();
                        if (RG4W == vo_20.peek16())
                        {
                          RG1W = word_at(RG1P, lcon(0x4));
                          RG1UW >>= bcon(0x8);
                          if (RG1W == vo_24.peek16())
                          {
                            RG2L = 0;
                            vw_08 = RG2W;
                            RG2P = ADRESSOF(i16, &vw_08);
                            RG1W = word_at(RG61p, lcon(0x4));
                            RG1UW >>= bcon(0x8);
                            RG4L = signedlong(RG1W);
                            RG1L = DM2_map_2066_1f37(unsignedlong(vw_14), RG4L, RG2P16);
                            RG7l = RG1L;
                            RG2P = ADRESSOF(i16, &vw_08);
                            RG4L = signedlong(vo_24.peek16());
                            RG1L = DM2_map_2066_1f37(unsignedlong(RG5W), RG4L, RG2P16);
                            RG7l |= RG1L;
                            RG1L = signedlong(vw_08);
                            if (RG1L != 2)
                            {
                              //m_2677B:
                              if (vw_08 == 0 && RG7l != 0)
                                skip00639 = true;
                            }
                            else
                            {
                              RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vw_14))));
                              RG4L = unsignedlong(RG1W);
                              RG1L = DM2_map_2066_1ec9(record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG5UW))) & lcon(0xffff), RG4L);
                              mov16(location(RG3P), RG1W);
                              mov16(location(RG61p), RG1W);
                            }
                          }
                        }
                      }
                    }
                  }
                  break;

                case 1:
                  //m_263DA:
                  RG4L = unsignedlong(RG5W);
                  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
                  RG2W = word_at(RG1P, 2) & lcon(0x6);
                  if (RG2W == 2)
                  {
                    RG3W = word_at(RG1P, 2);
                    RG2L = RG3L;
                    RG2UW >>= bcon(0x3);
                    RG2L = unsignedlong(RG2W);
                    RG2UL >>= bcon(0x8);
                    RG2L &= lcon(0x1f);
                    if (mkul(RG2L) >= mkul(lcon(0x6)))
                    {
                      if (mkul(RG2L) <= mkul(lcon(0x7)))
                      {
                        //m_26427:
                        RG4L >>= 14;
                        RG2L = signedlong(vo_24.peek16());
                        RG1L = signedlong(RG4W);
                        skip00635 = true;
                      }
                      else
                      {
                        if (RG2L == lcon(0x8))
                        {
                          //m_2645E:
                          RG3Blo |= 1;
                          mov16(location(RG1P + 2), RG3W);
                        }
                      }
                    }
                  }
                  break;

                case 2:
                  //m_2646A:
                  RG3L = unsignedlong(RG5W);
                  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG3UW));
                  RG2P = RG1P;
                  RG4P = RG1P;
                  RG1W = word_at(RG1P, 2) & lcon(0x7f);
                  if (RG1UW < mkuw(lcon(0x2c)))
                  {
                    //m_264A1:
                    if (RG1UW >= mkuw(lcon(0x1c)))
                    {
                      if (RG1UW <= mkuw(lcon(0x1c)))
                      {
                        //m_264CF:
                        RG3L >>= 14;
                        RG2L = signedlong(vo_24.peek16());
                        RG1L = signedlong(RG3W);
                        skip00635 = true;
                      }
                      else
                      {
                        if (RG1W == lcon(0x27))
                          skip00636 = true;
                      }
                    }
                  }
                  else
                  {
                    if (RG1UW <= mkuw(lcon(0x2c)))
                      skip00636 = true;
                    else
                    {
                      if (RG1UW >= mkuw(lcon(0x3f)))
                      {
                        if (RG1UW <= mkuw(lcon(0x3f)))
                        {
                          //m_264E1:
                          RG3Blo = byte_at(RG2P, lcon(0x4)) & lcon(0xfffffffa);
                          RG3Bhi = RG3Blo | bcon(0x4);
                          mov8(location(RG2P + lcon(0x4)), RG3Bhi);
                        }
                        else
                        {
                          if (RG1W == lcon(0x46))
                          {
                            //m_264F4:
                            RG1L = signedlong(vo_24.peek16());
                            DM2_CHANGE_CURRENT_MAP_TO(RG1L);
                            RG1W = word_at(RG2P, lcon(0x6));
                            RG4L = RG1L;
                            _RG4.ushiftr11();
                            RG4L = signedlong(RG4W);
                            RG1L <<= bcon(0x5);
                            _RG1.ushiftr11();
                            RG1L = (dm2_GET_TILE_VALUE(signedlong(RG1W), RG4L) & lcon(0xff)) >> 5 & lcon(0xff);
                            if (RG1L == lcon(0x4))
                            {
                              RG1W = word_at(RG2P, lcon(0x6));
                              RG4L = RG1L;
                              _RG4.ushiftr11();
                              RG4L = signedlong(RG4W);
                              RG1L <<= bcon(0x5);
                              _RG1.ushiftr11();
                              RG1RP = DM2_GET_ADDRESS_OF_TILE_RECORD(RG1W, RG4W);
                              RG4W = word_at(RG2P, lcon(0x4));
                              RG4L <<= bcon(0xd);
                              RG4UW >>= bcon(0xf);
                              RG4W &= 0x1;
                              and8(location(RG1P + lcon(0x3)), lcon(0xffffffdf));
                              RG4L <<= bcon(0xd);
                              or16(location(RG1P + 2), RG4W);
                            }
                          }
                        }
                      }
                    }
                  }
                  break;

                case 3:
                  //m_2656A:
                  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG5UW));
                  RG4P = RG1P;
                  RG3P = RG1P;
                  RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG4P, lcon(0x4)))));
                  RG2W = word_at(RG1P, lcon(0x4));
                  mov16(location(RG4P + lcon(0x6)), RG2W);
                  if (jnz_test8(location(RG1P), 1))
                  {
                    //m_265DA:
                    mov16(location(RG4P + lcon(0xa)), 0);
                    RG1W = word_at(RG4P, lcon(0xa));
                    mov16(location(RG4P + lcon(0x8)), RG1W);
                    if (jz_test8(location(RG4P + lcon(0xe)), lcon(0xffffff80)))
                    {
                      RG2W = word_at(RG4P, 2);
                      RG1Blo = RG2W != wcon(0xfffe) ? 1 : 0;
                      RG1L = unsignedlong(RG1Blo);
                      if (RG1L != 0)
                      {
                        RG1L = signedlong(DM2_RANDDIR()) << bcon(0xe);
                        RG4L = RG2L;
                        RG4Bhi &= lcon(0x3f);
                        RG4L |= RG1L;
                        mov16(location(RG3P + 2), RG4W);
                        //m_2661B:
                        for (;;)
                        {
                          if (RG2W == lcon(0xfffffffe))
                            break;
                          RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
                          RG4P = RG1P;
                          if (word_at(RG1P) != lcon(0xfffffffe))
                          {
                            RG1L = signedlong(DM2_RANDDIR());
                            RG3L = RG1L << bcon(0xe);
                            RG1W = word_at(RG4P);
                            RG1Bhi &= lcon(0x3f);
                            RG1L |= RG3L;
                            mov16(location(RG4P), RG1W);
                          }
                          RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG2UW)));
                          RG2L = RG1L;
                        }
                      }
                    }
                  }
                  else
                  {
                    mov16(location(RG4P + lcon(0xa)), 0);
                    RG1W = vo_28.peek16();
                    RG1W &= lcon(0x1f);
                    and8(location(RG4P + lcon(0xc)), lcon(0xffffffe0));
                    RG2W = word_at(RG4P, lcon(0xc));
                    RG2L |= RG1L;
                    RG1W = vo_20.peek16();
                    RG1W &= lcon(0x1f);
                    RG3L = RG2L & lcon(0xfffffc1f);
                    RG1L <<= bcon(0x5);
                    mov16(location(RG4P + lcon(0xc)), RG3W | RG1W);
                    RG1W = vo_24.peek16();
                    RG1W &= lcon(0x3f);
                    and8(location(RG4P + lcon(0xd)), lcon(0x3));
                    RG1L <<= bcon(0xa);
                    or16(location(RG4P + lcon(0xc)), RG1W);
                  }
                  break;

                default: throw(THROW_DMABORT);
              }

              if (skip00639)
                break;

              if (skip00636)
              {
                //m_264B7:
                RG1Blo = byte_at(RG4P, lcon(0x4));
                if ((RG1Blo & bcon(0x4)) != 0)
                {
                  RG1Bhi = RG1Blo | bcon(0x1);
                  mov8(location(RG4P + lcon(0x4)), RG1Bhi);
                }
              }

              if (skip00635)
              {
                //m_26434:
                RG4W = vo_20.peek16();
                RG4W += table1d2804[RG1L];
                RG4L = signedlong(RG4W);
                RG3W = vo_28.peek16();
                i16 tmp = RG3W + table1d27fc[RG1L];
                RG1L = signedlong(tmp);
                SKW_2066_1ea3(RG1L, RG2L, RG4L);
              }
            }

            //m_26787:
            RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG5UW)));
            RG5L = RG1L;
            if (RG1W == lcon(0xfffffffe))
              break;
          }
        }
        //m_2679D:
        RG1L = unsignedlong(vb_2c) >> 5 & lcon(0xff);
        if (RG1L == 2)
        {
          RG4L = unsignedlong(vo_24.peek16()) << 4;
          RG1P = ddat.v1e03c8;
          RG1W = word_at(RG1P, RG4L + lcon(0xe));
          RG1L <<= bcon(0x8);
          RG1UW >>= bcon(0xc);
          RG4L = unsignedlong(RG1Blo);
          RG3L = lcon(0x6a);
          RG2L = lcon(0xb);
          RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(8, RG4Blo, 11, lcon(0x6a));
          if (RG1W == 0)
          {
            RG1W = vo_28.peek16();
            vw_10 = RG1W;
            RG1W = vo_20.peek16();
            vw_0c = RG1W;
            RG1L = signedlong(vo_24.peek16());
            RG3P = ADRESSOF(i16, &vw_0c);
            RG1L = DM_LOCATE_OTHER_LEVEL(RG1L, 1, &vw_10, RG3P16, NULL);
            RG2L = signedlong(RG1W);
            SKW_2066_1ea3(signedlong(vw_10), RG2L, signedlong(vw_0c));
          }
        }
        //m_2682F:
        vo_20.inc16();
      }
    }
  }
  FOREND
}

// stacksize was 0x8
i32 DM2_map_0cee_04e5(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 vw_00;

  vw_00 = RG3W; // adj
  RG2L = signedlong(RG2W);
  RG4L = signedlong(RG4W);
  RG1L = signedlong(RG1W);
  DM2_CALC_VECTOR_W_DIR(RG1W, RG4W, RG2W, &vw_00, &argw0);
  return dm2_GET_TILE_VALUE(signedlong(vw_00), signedlong(argw0));
}

i32 DM2_map_0cee_17e7(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  RG3L = (RG1L & lcon(0xffff)) * lcon(0x7ab9) / 2;
  RG4L = unsignedlong(RG4W);
  RG1L = 11 * RG4L;
  RG4L = RG3L + RG1L;
  RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
  RG1W = word_at(RG1P);
  RG1L = unsignedlong(RG1W);
  RG4L += RG1L;
  RG4L >>= bcon(0x2);
  RG3L = unsignedlong(RG2W);
  return RG4L % RG3L;
}

void DM2_map_3B001(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4; RG4L = edxl;

  ddat.v1e0270 = RG1W;
  ddat.v1e0272 = RG4W;
  RG1W = ddat.v1d3248;
  ddat.v1d3248 = lcon(0xffffffff);
  RG1L = signedlong(RG1W);
  DM2_CHANGE_CURRENT_MAP_TO(RG1L);
}

void DM2_map_3BF83(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG51w;
  i32 RG52l;
  i16 RG6w;
  c_wlreg _RG7;
  i16 vw_00;
  i16 vw_04;
  i16 parw01;
  bool flag;

  RG6w = RG1W;
  RG51w = RG4W;
  vw_00 = RG2W;
  vw_04 = RG3W;
  RG1Blo = RG2W != ddat.v1e0266 ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  flag = RG1L == 0;
  RG4L = RG1L;
  if (!flag)
    DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_00));
  if (RG6w >= 0 && RG6w < mapdat.map_width && RG51w >= 0 && RG51w < mapdat.map_height)
  {
    if (RG4L == 0)
    {
      //m_3C03C:
      RG1L = signedlong(RG51w);
      put16(parw01, RG1W);
      RG3L = signedlong(RG6w);
      RG2L = signedlong(ddat.v1e0272);
      RG4L = signedlong(ddat.v1e0270);
    }
    else
    {
      DM2_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
      DM2_MOVE_RECORD_TO(lcon(0xffff), signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), lcon(0xffffffff), 0);
      RG2L = signedlong(vw_00);
      RG7L = signedlong(RG51w);
      RG52l = signedlong(RG6w);
      DM2_LOAD_NEWMAP(RG52l, RG7L, RG2L, lcon(0x1));
      put16(parw01, RG7W);
      RG3L = RG52l;
      RG2L = 0;
      RG4L = lcon(0xffffffff);
    }
    //m_3C055:
    DM2_MOVE_RECORD_TO(lcon(0xffff), RG4L, RG2L, RG3L, parw01);
    party.rotate(vw_04);
    return;
  }
  //m_3C06D:
  if (RG4L != 0)
    DM2_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
}
