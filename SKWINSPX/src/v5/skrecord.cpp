#include <mdata.h>
#include <skrandom.h>
#include <util.h>
#include <sfx.h>
#include <dm2data.h>
#include <startend.h>
#include <skgdtqdb.h>
#include <skmap.h>
#include <bcreatur.h>
#include <sktimer.h>
#include <skmovrec.h>
#include <sktimprc.h>
#include <SK1C9A.h>
#include <skrecord.h>

#include <dm2debug.h>

// TODO: optimize temporaries
#include <regs.h>
#include <emu.h>

// TODO: data-class
t_record* dm2_v1e038c;

t_record* recordptr[16]; // TODO: missing original location
// each of those pointers points to an array of records with different sizes
// the record-sizes are stored in table_recordsizes
// so t_record is always the first word of a record!

const i8 table_recordsizes[16] =
{
  4, 6, 4, 8, 16, 4, 4, 4, 4, 8, 4, 0, 0, 0, 8, 4
};

void init_global_records(void)
{
	SPX_DEBUG_PRINT("INIT_GLOBAL_RECORDS:INIT\n");
  for (int i = 0; i < 16; i++)
    recordptr[i] = NULL;
  dm2_v1e038c = NULL;
}

// TODO: helpers, casts!
t_record mk_record(i16 n) { return (t_record)n; }
i16 record_to_word(t_record r) { return mkw(r); }
i32 record_to_long(t_record r) { return (i32)r; }

// bitformat of r: --aa aabb bbbb bbbb  a = tableidx (4 bits), b = factor (10 bits)
t_record* DM2_GET_ADDRESS_OF_RECORD(t_record r)
{
  ui8 idx = (r >> 10) & 0xf; // range 0-15 (4 bits)
  ui16 ofs = unsignedword(table_recordsizes[idx]) * (r & 0x3ff); // max. range 14 bits
  // must go down to bytepointer now to add the offset
  return UPCAST(t_record, DOWNCAST(t_record, recordptr[idx]) + ofs);
}

t_record DM2_GET_NEXT_RECORD_LINK(t_record r)
{
  return *DM2_GET_ADDRESS_OF_RECORD(r);
}

// TODO: meaning of the used globals
void DM2_APPEND_RECORD_TO(t_record eaxrec, t_record* edxrp, i16 x, i16 y)
{
  t_record* dest;
  t_tile* tilep;
  t_record record;

  if (eaxrec == OBJECT_END_MARKER || eaxrec == OBJECT_NULL)
    return;
  dest = DM2_GET_ADDRESS_OF_RECORD(eaxrec);
  *dest = OBJECT_END_MARKER;
  if (x < 0)
  {
    //m_D5DA:
    dest = edxrp;
    //m_D5DC:
    while (*dest != OBJECT_END_MARKER)
      dest = DM2_GET_ADDRESS_OF_RECORD(*dest);
  }
  else
  {
    tilep = &mapdat.map[x][y];
    if ((*tilep & 0x10) == 0)
    {
      if (dm2_v1e038c[word_at(DOWNCAST(s_sgwords, ddat.savegamep4), 10) - 1] != -1)
        DM2_RAISE_SYSERR(0x48);
      *tilep |= 0x10;
      i16* wp = ddat.v1e03f4 + x + 1;
      i16 k = ddat.v1e0408 - (word_at(DOWNCAST(i16, ddat.v1e03e4), 2 * ddat.v1d3248) + x) - 1;
      //m_D539:
      while (--k != -1)
        *wp++ += 1;
      //m_D54A:
      i16 j = 0;
      tilep -= y;
      i16 n = ddat.v1e03f4[x];
      //m_D55E:
      while (j++ != y)
        if ((*tilep++ & 0x10) != 0)
          n++;
      //m_D576:
      t_record* recp = dm2_v1e038c + n;
      DM2_COPY_MEMORY(DOWNCAST(t_record, recp + 1), DOWNCAST(t_record, recp), 2 * (word_at(DOWNCAST(s_sgwords, ddat.savegamep4), 10) - n - 1));
      *recp = eaxrec;
      return;
    }
    //m_D5AD:
    t_record rec1 = DM2_GET_TILE_RECORD_LINK(x, y);
    //m_D5B2:
    t_record rec2;
    do
    {
      rec2 = rec1;
      rec1 = DM2_GET_NEXT_RECORD_LINK(rec1);
    } while (rec1 != OBJECT_END_MARKER);
    dest = DM2_GET_ADDRESS_OF_RECORD(rec2);
  }
  //m_D5CE:
  *dest = eaxrec;
}

// TODO: meaning of the used globals
void DM2_CUT_RECORD_FROM(t_record eaxrec, t_record* edxrecp, i16 x, i16 y)
{
  t_record* sourcep;
  t_record* RG4recp;

  if (eaxrec == OBJECT_END_MARKER || eaxrec == OBJECT_NULL)
    return;
  t_record vw_00 = eaxrec & wcon(0x3fff);
  sourcep = DM2_GET_ADDRESS_OF_RECORD(vw_00);
  if (x < 0)
    //m_D72A:
    RG4recp = edxrecp;
  else
  {
    i16 objidx = DM2_GET_OBJECT_INDEX_FROM_TILE(x, y);
    t_record* RG1recp = dm2_v1e038c + objidx;
    if (*sourcep == OBJECT_END_MARKER && (*RG1recp & wcon(0x3fff)) == vw_00)
    {
      mapdat.map[x][y] &= bcon(0xef);
      i16 vw_04 = ddat.savegamep4->warr_00[5] - 1;
      DM2_COPY_MEMORY(DOWNCAST(t_record, RG1recp), DOWNCAST(t_record, RG1recp + 1), 2 * (vw_04 - objidx));
      dm2_v1e038c[vw_04] = -1;
      i16* recp3 = ddat.v1e03f4 + x + 1;
      i16 n = ddat.v1e0408 - (x + ddat.v1e03e4[ddat.v1d3248]) - 1;
      //m_D6F4:
      while (--n != -1)
        *(recp3++) -= 1;
      *sourcep = OBJECT_END_MARKER;
      return;
    }
    //m_D709:
    if ((*RG1recp & wcon(0x3fff)) == vw_00)
    {
      *RG1recp = *sourcep;
      *sourcep = OBJECT_END_MARKER;
      return;
    }
    RG4recp = RG1recp;
  }

  //m_D72C:
  while (*RG4recp != OBJECT_END_MARKER && (*RG4recp & wcon(0x3fff)) != vw_00)
    RG4recp = DM2_GET_ADDRESS_OF_RECORD(*RG4recp);
  //m_D75D:
  if (*RG4recp != OBJECT_END_MARKER)
    *RG4recp = DM2_GET_NEXT_RECORD_LINK(*RG4recp);
  //m_D773:
  *sourcep = OBJECT_END_MARKER;
}

// ###############################################################

// all about ddat.savegamepaar5

i16 DM2_GET_DISTINCTIVE_ITEMTYPE(i16 eaxw)
{
  c_nreg _RG1; RG1L = unsignedlong(eaxw);
  c_nreg _RG2;
  c_nreg _RG4;

  RG4L = RG1L;
  if (RG1W == lcon(0xffffffff))
    return lcon(0x1ff);
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(unsignedlong(RG4W));
  RG4W &= lcon(0x3c00);
  RG4L = unsignedlong(RG4W) >> bcon(0xa);
  RG4L = signedlong(RG4W);
  RG4W = table1d3278[RG4L];
  RG2L = RG4L;
  RG2Blo ^= RG4Blo;
  RG2Bhi &= lcon(0xffffff80);
  RG2L = unsignedlong(RG2W);
  if (RG2L != 0)
  {
    RG4Bhi &= lcon(0x7f);
    RG1Blo = 0;
  }
  RG1W = unsignedword(RG1Blo);
  RG1L += RG4L;
  return RG1W;
}

i8 DM2_QUERY_CLS2_FROM_RECORD(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG4;

  RG4L = RG1L;
  //m_F05D:
  for (;;)
  {
    if (RG4W == lcon(0xffffffff))
      return bcon(0xff);
    if (RG4UW >= mkuw(lcon(0xffffff80)))
      return RG4Blo - bcon(0x80);
    RG2L = unsignedlong(RG4W);
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
    RG4W &= lcon(0x3c00);
    RG4L = unsignedlong(RG4W) >> bcon(0xa);
    if (mkul(RG4L) > mkul(lcon(0xf)))
      return bcon(0xff);

    switch (RG4L)
    {
      case 0:
      case 1:
      case 11:
      case 12:
      case 13:
        return bcon(0xff);

      case 2:
        return DM2_QUERY_CLS2_OF_TEXT_RECORD(RG2L);

      case 3:
        return DM2_GET_WALL_DECORATION_OF_ACTUATOR(DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW))));

      case 4:
        //m_F09B:
        return byte_at(RG1P, lcon(0x4));

      case 5:
      case 6:
      case 10:
      case 15:
        //m_F0A1:
        RG1W = word_at(RG1P, 2);
        return RG1Blo & bcon(0x7f);

      case 7:
        //m_F0AA:
        return bcon(0x0);

      case 8:
        //m_F0AF:
        RG1W = word_at(RG1P, 2); // TODO: gets extended w/o conversion now
        RG1L = 2 * RG1L;
        RG1UW >>= bcon(0x9);
        return RG1Blo;

      case 9:
        //m_F0BC:
        RG4W = word_at(RG1P, lcon(0x4));
        RG4L <<= bcon(0xd);
        RG4UW >>= 14;
        RG4L = 8 * unsignedlong(RG4W);
        RG4L = unsignedlong(RG4Blo);
        RG1W = word_at(RG1P, lcon(0x4));
        RG1UW >>= bcon(0xd);
        return RG1Blo | RG4Blo;

      case 14:
        //m_F0E8:
        RG4W = word_at(RG1P, 2);
        break;

      default: throw(THROW_DMABORT);
    }
  }
  FOREND
}

void DM2_SET_ITEMTYPE(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;

  RG2L = RG1L;
  if (RG1W == lcon(0xffffffff) || RG1UW >= mkuw(lcon(0xffffff80)))
    return;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
  RG2W &= lcon(0x3c00);
  RG2L = unsignedlong(RG2W) >> bcon(0xa);
  RG2L -= 4;
  if (mkul(RG2L) > mkul(lcon(0x6)))
    return;
  switch (RG2L)
  {
    case 0:
      //m_F171:
      mov8(location(RG1P + lcon(0x4)), RG4Blo);
    case 3:
      return;

    case 1:
    case 2:
    case 6:
      //m_F177:
      RG2Blo = RG4Blo;
      RG2W &= lcon(0x7f);
      and8(location(RG1P + 2), lcon(0xffffff80));
      break;

    case 4:
      //m_F189:
      RG2Blo = RG4Blo;
      RG2W &= lcon(0x7f);
      and8(location(RG1P + lcon(0x3)), lcon(0xffffff80));
      RG2L <<= bcon(0x8);
      break;

    case 5:
      //m_F199:
      RG2L = unsignedlong(RG4Blo) / 8;
      RG2W &= 0x3;
      and8(location(RG1P + lcon(0x4)), lcon(0xfffffff9));
      RG2L = 2 * RG2L;
      or16(location(RG1P + lcon(0x4)), RG2W);
      RG2Blo = RG4Blo & lcon(0x7);
      RG2W &= lcon(0x7);
      and8(location(RG1P + lcon(0x5)), lcon(0x1f));
      RG2L <<= bcon(0xd);
      RG5W = word_at(RG1P, lcon(0x4));
      RG5L |= RG2L; // TODO highword undefined
      mov16(location(RG1P + lcon(0x4)), RG5W);
      RG2L = RG5L; // TODO highword undefined
      RG2W &= 0x6;
      if (RG2W == 2)
        mov16(location(RG1P + lcon(0x6)), lcon(0xffffffff));
      return;

    default: throw(THROW_DMABORT);
  }

  //m_F182:
  or16(location(RG1P + 2), RG2W);
}

i32 DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  if (RG1W == lcon(0xffffffff))
    return 0;
  RG3L = unsignedlong(RG4Blo);
  RG4L = unsignedlong(RG1W);
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L);
  return mkl(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(DM2_QUERY_CLS1_FROM_RECORD(RG4L) & bcon(0xff), RG1Blo, 11, RG3Blo));
}

i32 DM2_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4;

  RG1Bhi &= 0x1;
  RG1L = unsignedlong(RG1W);
  RG4L = RG1L >> 7;
  if (mkul(RG4L) > mkul(lcon(0x3)))
    return lcon(0xffff);
  switch (RG4L)
  {
    case 0:
      return lcon(0x5);

    case 1:
      return lcon(0x6);

    case 2:
      return lcon(0xa);

    case 3:
      //m_F261:
      if (RG1L > lcon(0x1fc))
        return lcon(0xffff);
      if (RG1L == lcon(0x1fc))
        return lcon(0x7);
      if (RG1L >= lcon(0x1e0))
        return lcon(0x9);
      return RG1L < lcon(0x1b0) ? lcon(0x8) : lcon(0x4);

    default: throw(THROW_DMABORT);
  }
  SWITCHEND
}

i32 DM2_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG4;

  RG1Bhi &= 0x1;
  RG4L = unsignedlong(RG1W);
  RG2L = RG4L >> 7;
  if (mkul(RG2L) > mkul(lcon(0x3)))
    return RG1L;
  switch (RG2L)
  {
    case 0:
      return RG1L;

    case 1:
      //m_F2C9:
      RG1L -= lcon(0x80);
      return RG1L;

    case 2:
      //m_F2D2:
      RG1L -= lcon(0x100);
      return RG1L;

    case 3:
      //m_F2DB:
      if (RG4L >= lcon(0x1fc))
        return 0;
      if (RG4L >= lcon(0x1e0))
      {
        RG1L -= lcon(0x1e0);
        return RG1L;
      }
      if (RG4L < lcon(0x1b0))
        RG1L -= lcon(0x180);
      else
        RG1L -= lcon(0x1b0);
      return RG1L;

    default: throw(THROW_DMABORT);
  }
  SWITCHEND
}

i8 DM2_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(i32 eaxl)
{
  return DM2_QUERY_CLS1_FROM_RECORD(DM2_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(eaxl & lcon(0xffff)) << bcon(0xa) & lcon(0xffff));
}

i8 DM2_QUERY_CLS1_FROM_RECORD(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4;

  //m_F326:
  while (RG1W != wcon(0xffff))
  {
    RG4L = RG1L;
    RG4Blo ^= RG1Blo;
    RG4Bhi &= lcon(0x3c);
    RG4L = unsignedlong(RG4W) >> bcon(0xa);
    RG4L = signedlong(RG4W);
    if (RG4L != lcon(0xe))
      //m_F00D:
      return table1d3298[RG4L];
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
    RG1W = word_at(RG1P, 2);
  }
  //m_F015:
  return 0xff;
}

i32 DM2_GET_WALL_TILE_ANYITEM_RECORD(i32 eaxl, i16 edxw)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4; RG4L = signedlong(edxw);

  RG4L = signedlong(RG4W);
  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(signedlong(RG1W), RG4W));
  //m_D46B:
  for (;;)
  {
    RG4L = RG1L;
    RG4Blo ^= RG1Blo;
    RG4Bhi &= lcon(0x3c);
    RG4L = unsignedlong(RG4W) >> bcon(0xa);
    if (RG4L > lcon(0x3))
      return RG1L;
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG1UW)));
  }
  FOREND
}

void DM2_SET_ITEM_IMPORTANCE(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  RG2L = RG1L;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
  RG2W &= lcon(0x3c00);
  RG3L = (unsignedlong(RG2W) >> bcon(0xa)) - lcon(0x4);
  if (mkul(RG3L) > mkul(lcon(0x6)))
    return;
  RG4W &= 0x1;
  RG2L = RG4L << 7;
  switch (RG3L)
  {
    case 0:
      //m_D7D2:
      and8(location(RG1P + lcon(0xf)), lcon(0xfffffffb));
      RG4L <<= bcon(0xa);
      or16(location(RG1P + lcon(0xe)), RG4W);
      return;

    case 1:
    case 2:
    case 6:
      //m_D7E0:
      and8(location(RG1P + 2), lcon(0x7f));
      or16(location(RG1P + 2), RG2W);
    case 3:
    case 5:
      return;

    case 4:
      //m_D7EB:
      and8(location(RG1P + lcon(0x3)), lcon(0x7f));
      RG4L <<= bcon(0xf);
      or16(location(RG1P + 2), RG4W);
      return;

    default: throw(THROW_DMABORT);
  }
  SWITCHEND
}

i32 DM2_RECYCLE_A_RECORD_FROM_THE_WORLD(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_wlreg _RG6; // TODO: maybe more
  i8* RG71p;
  i8* RG72p;
  i8* RG73p;
  c_lreg _RG74;
  i32 vl_00;
  i32 vl_04;
  i32 vl_08;
  i32 vl_0c;
  unk* xp_10; // add
  unk* xp_14;
  i32 vl_18;
  i16 vw_1c;
  i32 vl_20;
  c_o32 vo_24; // poke 16 peek 16 32 inc16
  i16 vw_28;
  i16 vw_2c;
  c_o32 vo_30; // poke 16 peek 8 16 32 inc16
  i16 vw_34;
  i8 vb_38;
  i8 vb_3c;
  i16 parw00;
  bool skip00131 = false;
  bool skipfar = false;

  RG5L = RG1L;
  vb_38 = RG4Blo;
  RG1L = unsignedlong(RG5W);
  if (RG1L == lcon(0xf))
    return lcon(0xffff);
  //m_D81B:
  vw_28 = ddat.v1d3248;
  if (ddat.v1e0234 == 0)
    //m_D83A:
    RG2L = lcon(0xffffffff);
  else
    RG2L = signedlong(ddat.v1e027c);
  //m_D83F:
  RG3L = 0;
  vl_18 = RG3L;
  RG1L = unsignedlong(RG5W);
  RG4W = unsignedword(RG4Blo);
  RG4Blo = ddat.v1e0426[RG1L];
  vo_30.poke16(RG4W);
  vw_34 = RG4W; // truncated
  RG4L = unsignedlong(ddat.v1e0426[RG1L]);
  RG1L = signedlong(ddat.v1e0266);
  if (RG4L == RG1L)
    skip00131 = true;
  else
  {
    RG4L = unsignedlong(vo_30.peek16());
    RG1L = signedlong(RG2W);
    if (RG4L == RG1L)
      skip00131 = true;
  }

  if (skip00131)
  {
    //m_D881:
    RG1L = signedlong(RG2W);
    if (RG1L == lcon(0xffffffff))
    {
      //m_D8DF:
      for (;;)
      {
        RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
        RG1Blo = byte_at(RG1P, lcon(0x4));
        RG1L = unsignedlong(RG1Blo);
        vo_30.inc16();
        RG4L = unsignedlong(vo_30.peek16());
        if (RG4L >= RG1L)
        {
          RG3L = 0;
          vo_30.poke16(RG3W);
        }
        RG1L = unsignedlong(vo_30.peek16());
        RG4L = signedlong(ddat.v1e0266);
        if (RG1L != RG4L)
        {
          if (RG1W == vw_34)
            vo_30.poke16(ddat.v1e0266);
          break;
        }
      }
    }
    else
    {
      //m_D889:
      for (;;)
      {
        RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
        RG1Blo = byte_at(RG1P, lcon(0x4));
        RG1L = unsignedlong(RG1Blo);
        vo_30.inc16();
        RG4L = unsignedlong(vo_30.peek16());
        if (RG4L >= RG1L)
        {
          RG3L = 0;
          vo_30.poke16(RG3W);
        }
        RG1W = vo_30.peek16();
        if (RG1W == vw_34)
        {
          vo_30.poke16(RG2W);
          RG2L = lcon(0xffffffff);
        }
        RG1L = unsignedlong(vo_30.peek16());
        RG4L = signedlong(RG2W);
        if (RG1L != RG4L)
        {
          RG4L = signedlong(ddat.v1e0266);
          if (RG1L != RG4L)
            break;
        }
      }
    }
  }
  //m_D929:
  RG1W = vo_30.peek16();
  vw_34 = RG1W; // truncated
  RG6L = 0;
  vl_04 = 0;
  //m_D937:
  for (;;)
  {
    RG4L = unsignedlong(vo_30.peek16());
    RG1L = 16 * RG4L;
    RG71p = ddat.v1e03c8;
    RG3W = word_at(RG71p, RG1L + lcon(0x8));
    RG3L <<= bcon(0x5);
    _RG3.ushiftr11();
    vw_2c = RG3W; // truncated
    RG1W = word_at(RG71p, RG1L + lcon(0x8));
    _RG1.ushiftr11();
    vw_1c = RG1W; // truncated
    RG1L = RG4L;
    RG1P = pointer_at(pointer_at(DOWNCAST(t_tile*, mapdat.tmpmap) + 4 * RG1L));
    xp_14 = UPCAST(unk, RG1P);
    RG1L = RG4L;
    RG1W = ddat.v1e03e4[RG1L];
    RG4L = 2 * (RG1L & lcon(0xffff));
    RG1L = 2 * unsignedlong(word_at(ddat.v1e03d8, RG4L));
    xp_10 = UPCAST(unk, DOWNCAST(t_record, dm2_v1e038c + RG1L / 2));
    RG1L = 0;
    vo_24.poke16(RG1W);
    //m_DD72:
    for (;;)
    {
      RG1L = signedlong(vo_24.peek16());
      RG4L = unsignedlong(vw_2c);
      if (RG1L > RG4L)
      {
        RG4L = unsignedlong(vo_30.peek16());
        RG1L = signedlong(ddat.v1e0266);
        if (RG4L != RG1L)
        {
          RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
          RG1Blo = byte_at(RG1P, lcon(0x4));
          RG1L = unsignedlong(RG1Blo);
          if (RG1L > 1)
          {
            //m_DDD5:
            RG1L = signedlong(RG2W);
            if (RG1L == lcon(0xffffffff))
            {
              //m_DE36:
              for (;;)
              {
                RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
                RG1Blo = byte_at(RG1P, lcon(0x4));
                RG1L = unsignedlong(RG1Blo);
                vo_30.inc16();
                RG4L = unsignedlong(vo_30.peek16());
                if (RG4L >= RG1L)
                {
                  RG3L = 0;
                  vo_30.poke16(RG3W);
                }
                RG4L = unsignedlong(vo_30.peek16());
                RG1L = signedlong(ddat.v1e0266);
                if (RG4L != RG1L)
                {
                  if (RG4W == vw_34)
                    vo_30.poke16(ddat.v1e0266);
                  break;
                }
              }
            }
            else
            {
              //m_DDDD:
              for (;;)
              {
                RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
                RG1Blo = byte_at(RG1P, lcon(0x4));
                RG1L = unsignedlong(RG1Blo);
                vo_30.inc16();
                RG4L = unsignedlong(vo_30.peek16());
                if (RG4L >= RG1L)
                {
                  RG3L = 0;
                  vo_30.poke16(RG3W);
                }
                RG1W = vo_30.peek16();
                if (RG1W == vw_34)
                {
                  vo_30.poke16(RG2W);
                  RG2L = lcon(0xffffffff);
                }
                RG1L = unsignedlong(vo_30.peek16());
                RG4L = signedlong(RG2W);
                if (RG1L != RG4L)
                {
                  RG4L = signedlong(ddat.v1e0266);
                  if (RG1L != RG4L)
                    break;
                }
              }
            }
            break;
          }
        }
        //m_DDAD:
        if (vl_18 != 0)
        {
          RG1L = unsignedlong(RG5W);
          RG4Blo = vo_30.peek8();
          ddat.v1e0426[RG1L] = RG4Blo;
          return lcon(0xffff);
        }
        vl_18 = 1;
        break;
      }
      //m_D9B6:
      RG3L = 0;
      //m_DD5B:
      for (;;)
      {
        RG4L = signedlong(RG3W);
        RG1L = unsignedlong(vw_1c);
        if (RG4L > RG1L)
        {
          vo_24.inc16();
          break;
        }
        //m_D9BD:
        RG1P = DOWNCAST(unk, xp_14);
        xp_14 = UPCAST(unk, RG1P + 1);
        if (jnz_test8(location(RG1P), lcon(0x10)))
        {
          bool skip00133 = false;
          vb_3c = lcon(0xffffffff);
          RG6W = word_at(DOWNCAST(unk, xp_10));
          RG4L = unsignedlong(vo_30.peek16());
          RG1L = signedlong(ddat.v1e0266);
          xp_10 += 2; // DANGER
          if (RG4L != RG1L)
            skip00133 = true;
          else
          {
            RG1W = vo_24.peek16();
            RG1W -= ddat.v1e0270;
            RG1L += 5;
            RG1L = unsignedlong(RG1W);
            if (RG1L > lcon(0xa))
              skip00133 = true;
            else
            {
              RG1L = RG3L;
              RG1W -= ddat.v1e0272;
              RG1L += 5;
              RG1L = unsignedlong(RG1W);
              if (RG1L > lcon(0xa))
                skip00133 = true;
              else
                RG1L = 1;
            }
          }

          if (skip00133)
            //m_DA2E:
            RG1L = 0;
          //m_DA30:
          vl_0c = RG1L;
          //m_DA34:
          for (;;)
          {
            for (;;)
            {
              if (!skipfar)
              {
                bool skipbreak9 = false;
                for (;;)
                {
                  bool skip00137 = false;
                  bool skip00138 = false;
                  RG1L = RG6L;
                  RG1W &= lcon(0x3c00);
                  RG1L = unsignedlong(RG1W) >> bcon(0xa);
                  RG4L = unsignedlong(RG1W);
                  if (RG4L == lcon(0x3))
                  {
                    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG6UW));
                    RG72p = RG1P;
                    RG4W = word_at(RG1P, 2) & lcon(0x7f);
                    RG1L = signedlong(RG4W);
                    if (RG1L < lcon(0x2c) && table1d324c[RG1L] != 0)
                    {
                      skipbreak9 = true;
                      break;
                    }
                    if (vl_18 != 0 && vl_0c != 0)
                    {
                      RG4L = signedlong(RG4W);
                      if (RG4L == lcon(0x30))
                      {
                        RG4L = 0;
                        vl_0c = RG4L;
                        RG4W = word_at(RG72p, 2);
                        RG4UW >>= bcon(0x7);
                        RG4L = unsignedlong(RG4W);
                        RG1L = DM2_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(RG4L);
                        if (RG1W == RG5W)
                        {
                          RG1L = DM2_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG4L);
                          vb_3c = RG1Blo;
                        }
                      }
                    }
                    break;
                  }
                  //m_DAF9:
                  if (RG4L == 2)
                  {
                    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG6UW));
                    RG4W = word_at(RG1P, 2) & lcon(0x6);
                    if (RG4W != 2)
                      break;
                    RG1W = word_at(RG1P, 2);
                    RG1UW >>= bcon(0x3);
                    RG1L = unsignedlong(RG1W);
                    RG1UL >>= bcon(0x8);
                    RG1L &= lcon(0x1f);
                    if (RG1L == lcon(0x4))
                      skipbreak9 = true;
                    break;
                  }
                  //m_DB31:
                  if (RG4L > lcon(0x3) && vl_0c != 0)
                    break;
                  if (RG1W != RG5W)
                  {
                    //m_DCFF:
                    RG1L = unsignedlong(RG1W);
                    if (RG1L == lcon(0x4))
                    {
                      RG1L = unsignedlong(RG5W);
                      if (RG1L != lcon(0xe))
                      {
                        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG6UW));
                        RG4P = RG1P;
                        if (word_at(RG1P + 2) != lcon(0xfffffffe))
                        {
                          RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG4P, lcon(0x4)))));
                          if (jnz_test8(location(RG1P), 1))
                          {
                            vl_04 = 1;
                            vl_20 = RG6L;
                            RG6W = word_at(RG4P, 2);
                            continue;
                          }
                        }
                      }
                    }
                    break;
                  }
                  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG6UW));
                  RG73p = RG1P;
                  if (RG5UW < mkuw(lcon(0x6)))
                  {
                    //m_DB88:
                    if (RG5UW < mkuw(lcon(0x4)))
                      jump L_fin;
                    if (RG5UW <= mkuw(lcon(0x4)))
                    {
                      if (jz_test8(location(RG1P + lcon(0xf)), lcon(0x4)))
                      {
                        RG1Blo = word_at(RG1P + 2) != wcon(0xfffe) ? 1 : 0;
                        RG1L = unsignedlong(RG1Blo);
                        if (RG1L == 0)
                        {
                          RG4Bhi = vb_38;
                          if (RG4Bhi == lcon(0xffffffff) || RG4Bhi == byte_at(RG73p, lcon(0x4)))
                          {
                            DM2_CHANGE_CURRENT_MAP_TO(signedlong(vo_30.peek16()));
                            RG4L = unsignedlong(RG3W);
                            DM2_DELETE_CREATURE_RECORD(unsignedlong(vo_24.peek16()), RG4L, 1, lcon(0xffffffff));
                            jump L_fin;
                          }
                        }
                      }
                      break;
                    }
                    skip00137 = true;
                  }
                  else
                  {
                    if (RG5UW <= mkuw(lcon(0x6)))
                      skip00137 = true;
                    else
                    {
                      if (RG5UW < mkuw(lcon(0xa)))
                      {
                        //m_DB79:
                        if (RG5W != lcon(0x8))
                          jump L_fin;
                        //m_DC66:
                        if (jnz_test8(location(RG1P + lcon(0x3)), lcon(0xffffff80)))
                          break;
                        RG1W = word_at(RG1P, 2);
                        RG4L = 2 * RG1L;
                        RG4UW >>= bcon(0x9);
                        skip00138 = true;
                      }
                      else
                      {
                        if (RG5UW > mkuw(lcon(0xa)))
                        {
                          if (RG5W != lcon(0xe))
                            jump L_fin;
                          //m_DBF3:
                          RG4Blo = vb_38;
                          if (RG4Blo != lcon(0xffffffff))
                          {
                            RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(unsignedlong(word_at(RG1P, 2)));
                            if (RG1Blo != RG4Blo)
                              break;
                          }
                          //m_DC12:
                          RG74L = unsignedlong(RG6W);
                          DM2_075f_056c(RG74L);
                          RG1L = signedlong(RG3W);
                          vl_08 = RG1L;
                          RG1L = signedlong(vo_24.peek16());
                          vl_00 = RG1L;
                          DM2_CUT_RECORD_FROM(mk_record(RG74L), NULL, RG1W, CUTX16(vl_08));
                          DM2_DELETE_MISSILE_RECORD(RG74L, NULL, vl_00, vl_08);
                          jump L_fin;
                        }
                        skip00137 = true;
                      }
                    }
                  }

                  if (skip00137)
                  {
                    //m_DC51:
                    if (jnz_test8(location(RG1P + 2), lcon(0xffffff80)))
                      break;
                    RG4W = word_at(RG1P, 2) & lcon(0x7f);
                    skip00138 = true;
                  }

                  if (skip00138)
                  {
                    //m_DC7C:
                    if (RG4Blo != vb_3c)
                    {
                      //m_DC8C:
                      RG1Bhi = vb_38;
                      if (RG1Bhi == lcon(0xffffffff) || RG4Blo == RG1Bhi)
                      {
                        RG1L = signedlong(vo_30.peek16());
                        DM2_CHANGE_CURRENT_MAP_TO(RG1L);
                        RG1Blo = vl_04 == 0 ? 1 : 0;
                        RG1L = unsignedlong(RG1Blo) - 1;
                        RG1L = signedlong(RG1W);
                        put16(parw00, RG1W);
                        RG2L = signedlong(RG3W);
                        RG4L = signedlong(vo_24.peek16());
                        RG1L = unsignedlong(RG6W);
                        DM2_MOVE_RECORD_TO(RG1L, RG4L, RG2L, lcon(0xffffffff), parw00);
                        jump L_fin;
                      }
                    }
                    else
                      vb_3c = lcon(0xffffffff);
                    break;
                  }
                }

                if (skipbreak9)
                  break;
              }
              //m_DACC:
              skipfar = false;
              RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG6UW)));
              RG6L = RG1L;
              if (RG1W == lcon(0xfffffffe))
                break;
            }
            //m_DAE2:
            if (vl_04 == 0)
              break;
            RG6L = vl_20;
            vl_04 = 0;
            skipfar = true;
          }
        }
        //m_DD5A:
        RG3L++;
      }
    }
  }
  FOREND

L_fin:
  DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_28));
  RG1L = unsignedlong(RG5W);
  RG4Blo = vo_30.peek8();
  ddat.v1e0426[RG1L] = RG4Blo;
  return RG6L & lcon(0xffff3fff);
}

i32 DM2_ALLOC_NEW_RECORD(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5;
  c_wlreg _RG6;

  RG5L = RG1L;
  RG1Bhi &= lcon(0x7f);
  RG1L = unsignedlong(RG1W);
  RG6W = word_at(DOWNCAST(s_sgwords, ddat.savegamep4), 2 * RG1L + lcon(0xc));
  RG1L = unsignedlong(RG5W);
  if (RG1L != lcon(0x800a))
  {
    //m_DEB5:
    if (RG1L == lcon(0xa))
      RG6L -= 3;
  }
  else
    RG5L = lcon(0xa);
  //m_DEBD:
  RG1L = RG6L;
  RG4L = unsignedlong(RG5W);
  RG3L = unsignedlong(table_recordsizes[RG4L]);
  RG2RP = recordptr[RG4L];
  //m_DED3:
  for (;;)
  {
    if (word_at(RG2P) != lcon(0xffffffff))
    {
      //m_DEE4:
      if (--RG1W != 0)
      {
        //m_DF0D:
        RG3W &= lcon(0xfffe);
        RG4L = signedlong(RG3W);
        RG2P += RG4L;
        continue;
      }
      RG1L = DM2_RECYCLE_A_RECORD_FROM_THE_WORLD(unsignedlong(RG5W), lcon(0xff));
      RG6L = RG1L;
      if (RG1W == lcon(0xffffffff))
        return RG1L;
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
      RG2P = RG1P;
    }
    else
    {
      RG6L -= RG1L;
      RG1L = RG5L << bcon(0xa);
      RG6L |= RG1L;
    }
    break;
  }
  //m_DF18:
  RG4L = signedlong(RG3W);
  DM2_ZERO_MEMORY(RG2P, RG4L);
  mov16(location(RG2P), lcon(0xfffffffe));
  RG1L = unsignedlong(RG5W);
  if (RG1L == lcon(0x9))
    mov16(location(RG2P + 2), lcon(0xfffffffe));
  return RG6L;
}

i32 DM2_ALLOC_NEW_DBITEM(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG4L = RG1L;
  RG2L = unsignedlong(RG1W);
  RG1L = DM2_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(RG2L);
  RG4W &= lcon(0x8000);
  RG1L |= RG4L;
  RG1L = DM2_ALLOC_NEW_RECORD(unsignedlong(RG1W));
  RG4L = RG1L;
  RG3L = RG1L;
  if (RG1W != lcon(0xffffffff))
  {
    RG1L = DM2_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG2L);
    RG2L = unsignedlong(RG1Blo);
    RG1L = unsignedlong(RG4W);
    DM2_SET_ITEMTYPE(RG1L, RG2L);
  }
  return RG3L;
}

i32 DM2_ALLOC_NEW_DBITEM_DEEPLY(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG51w;
  i32 RG52l;
  i8 vb_00;

  RG2L = unsignedlong(RG1W);
  RG1L = DM2_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG2L);
  vb_00 = RG1Blo;
  RG4L = unsignedlong(RG1Blo);
  RG1L = DM2_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(RG2L);
  RG51w = RG1W;
  RG2L = unsignedlong(RG1W);
  RG1L = DM2_RECYCLE_A_RECORD_FROM_THE_WORLD(RG2L, RG4L);
  RG3L = RG1L;
  if (RG1W == lcon(0xffffffff))
  {
    RG1L = DM2_ALLOC_NEW_RECORD(RG2L);
    RG3L = RG1L;
    if (RG1W == lcon(0xffffffff))
      return RG3L;
  }
  RG4L = unsignedlong(RG51w);
  RG4Blo = table_recordsizes[RG4L];
  RG4L = unsignedlong(RG4Blo);
  RG52l = unsignedlong(RG3W);
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(CUTX16(RG52l))));
  RG2P = RG1P;
  DM2_ZERO_MEMORY(RG1P, RG4L);
  mov16(location(RG2P), lcon(0xfffffffe));
  DM2_SET_ITEMTYPE(RG52l, unsignedlong(vb_00));
  return RG3L;
}

void DM2_DEALLOC_RECORD(i32 eaxl)
{
  mov16(location(DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(CUTX16(eaxl)))))), lcon(0xffffffff));
}

i8 DM2_QUERY_CLS2_OF_TEXT_RECORD(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG4;

  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
  RG4P = RG1P;
  RG2W = word_at(RG1P, 2) & lcon(0x6);
  if (RG2W == 2)
  {
    bool skip00179 = false;
    RG1W = word_at(RG1P, 2);
    RG1UW >>= bcon(0x3);
    RG1L = unsignedlong(RG1W);
    RG1UL >>= bcon(0x8);
    RG1L &= lcon(0x1f);
    if (mkul(RG1L) < mkul(2))
    {
      //m_FCC9:
      if (RG1L == 0)
        skip00179 = true;
    }
    else
    {
      if (mkul(RG1L) <= mkul(2))
        skip00179 = true;
      else
      {
        if (mkul(RG1L) >= mkul(lcon(0x5)))
        {
          if (mkul(RG1L) > mkul(lcon(0x5)))
            if (RG1L != lcon(0xd))
              return bcon(0xff);
          skip00179 = true;
        }
      }
    }

    if (skip00179)
    {
      //m_FCCD:
      RG1W = word_at(RG4P, 2);
      RG1UW >>= bcon(0x3);
      return RG1Blo;
    }
  }
  //m_FCD8:
  return bcon(0xff);
}

i8 DM2_GET_WALL_DECORATION_OF_ACTUATOR(unk* xeaxp)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2;
  c_nreg _RG4;

  RG4W = word_at(RG1P, lcon(0x4));
  RG4UW >>= bcon(0xc);
  if (RG4W == 0)
    return bcon(0xff);

  RG2L = signedlong(mapdat.map_width);
  RG1P = DOWNCAST(t_tile*, &mapdat.map[RG2L]); // adjusted
  RG2L = signedlong(mapdat.map_height);
  RG1P = pointer_at(RG1P - lcon(0x4));
  RG2P = RG1P + RG2L; // the adress is now &mapdat.map[&mapdat.map_width-1][mapdat.map_height], isn't it?
  RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
  RG1W = word_at(RG1P, lcon(0xc));
  RG1L <<= bcon(0x8);
  RG1UW >>= bcon(0xc);
  RG1L = unsignedlong(RG1W);
  RG2P += RG1L; // oops? that is after the map, isn't it?
  RG1L = signedlong(RG4W); // is that one negative then? wouldn't make sense if not
  RG1Blo = byte_at(RG2P, RG1L + lcon(0xffffffff));
  return RG1Blo;
}

i8 DM2_GET_FLOOR_DECORATION_OF_ACTUATOR(unk* xeaxp)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG4W = word_at(RG1P, lcon(0x4));
  RG4UW >>= bcon(0xc);
  if (RG4W == 0)
    return bcon(0xff);

  RG1P = DOWNCAST(t_tile*, &mapdat.map[signedlong(mapdat.map_width)]); // adjusted
  RG2L = signedlong(mapdat.map_height);
  RG1P = pointer_at(RG1P - lcon(0x4));
  RG3P = RG1P + RG2L;
  RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
  RG2W = word_at(RG1P, lcon(0xc));
  RG2L <<= bcon(0x8);
  RG2UW >>= bcon(0xc);
  RG2P = RG3P + unsignedlong(RG2W);
  RG1W = word_at(RG1P, lcon(0xa)) & lcon(0xf);
  RG1L = unsignedlong(RG1W);
  RG2P += RG1L;
  RG1L = signedlong(RG4W);
  RG1Blo = byte_at(RG2P, RG1L + lcon(0xffffffff));
  return RG1Blo;
}

i32 DM2_IS_WALL_ORNATE_ALCOVE_FROM_RECORD(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  s_8wordarray s15_00;
  i8 vb_10;

  RG2L = RG1L;
  RG4L = unsignedlong(RG4W);
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L);
  vb_10 = RG1Blo;
  RG1L = signedlong(CUTX16(DM2_IS_WALL_ORNATE_ALCOVE(unsignedlong(vb_10))));
  if (RG1L != 1)
    return 0;
  RG4L >>= 14;
  RG1L = signedlong(CUTX16(RG4L + 2 & lcon(0x3)));
  RG4L = unsignedlong(RG2W);
  RG1L = DM2_query_0cee_1a46(&s15_00, RG4L, RG1L, 1);
  RG1Blo = mkb(s15_00.warr_00[0x5]); // truncated
  return (RG1Blo != vb_10) ? 0 : 1;
}

unk* DM2_QUERY_CREATURE_AI_SPEC_FROM_TYPE(i32 eaxl)
{
  return DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(CUTX16(eaxl))))), lcon(0x4)))));
}

i16 DM2_QUERY_CREATURE_AI_SPEC_FLAGS(i32 eaxl)
{
  return word_at(DM2_QUERY_CREATURE_AI_SPEC_FROM_TYPE(eaxl & lcon(0xffff)));
}

c_aidef* DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(i32 eaxl)
{
  return UPCAST(c_aidef, table1d296c[unsignedlong(CUTX16(DM2_QUERY_GDAT_CREATURE_WORD_VALUE(eaxl & lcon(0xff), lcon(0x5))))]);
}

void DM2_DELETE_CREATURE_RECORD(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG4; RG4L = edxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG1; RG1L = eaxl;
  i8* RG51p;
  i32 RG52l;
  i8* RG61p;
  c_ql32 vql_00;
  i16 vw_04;
  c_ql32 vql_08;
  c_ql32 vql_0c;
  i16 vw_10;
  i16 vw_14;
  i16 parw02;

  vql_0c.poke32(RG1L);
  vql_08.poke32(RG4L);
  vw_04 = RG2W; // truncated
  vql_00.poke32(RG3L);
  RG1L = DM2_GET_CREATURE_AT(unsignedlong(vql_0c.peek16()), unsignedlong(vql_08.peek16()));
  vw_14 = RG1W; // truncated
  if (RG1W == lcon(0xffffffff))
    return;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
  RG51p = RG1P;
  RG61p = RG1P;
  RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG51p, lcon(0x4)))));
  if (jz_test8(location(RG1P), 1))
  {
    RG1L = table1d607e[signedlong(CUTX16(DM2_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(byte_at(RG51p, lcon(0x4))), 1)))].ul_00;
    if ((RG1Blo & bcon(0x4)) == 0)
    {
      RG51p += lcon(0xc);
      vw_10 = ddat.v1d3248;
      RG1W = word_at(RG51p);
      RG1UW >>= bcon(0xa);
      RG1L = signedlong(RG1W);
      DM2_CHANGE_CURRENT_MAP_TO(RG1L);
      RG1L = timdat.gametick + 1;
      i32 parl00 = RG1L;
      RG1W = word_at(RG51p);
      RG1L <<= bcon(0x6);
      _RG1.ushiftr11();
      RG4L = unsignedlong(RG1W);
      RG1W = word_at(RG51p) & lcon(0x1f);
      RG1L = unsignedlong(RG1W);
      DM2_INVOKE_MESSAGE(RG1L, RG4L, 0, 0, parl00);
      DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_10));
    }
    //m_2114C:
    RG4Bhi = byte_at(RG61p, lcon(0x5));
    if (RG4Bhi != lcon(0xffffffff))
    {
      RG1L = unsignedlong(RG4Bhi);
      mov8(location(DOWNCAST(c_creature, creatures) + 34 * RG1L + lcon(0x1a)), 0);
    }
  }
  //m_21169:
  RG2L = signedlong(vql_08.peek16());
  RG4L = signedlong(vql_0c.peek16());
  RG52l = unsignedlong(vw_14);
  DM2_MOVE_RECORD_TO(RG52l, RG4L, RG2L, lcon(0xfffffffc), 0);
  RG1L = signedlong(vql_00.peek16());
  put16(parw02, RG1W);
  DM2_DROP_CREATURE_POSSESSION(RG52l, unsignedlong(vql_0c.peek16()), unsignedlong(vql_08.peek16()), unsignedlong(vw_04), parw02);
  DM2_1c9a_0247(RG52l);
  DM2_DEALLOC_RECORD(RG52l);
}

void DM2_DELETE_MISSILE_RECORD(i32 eaxl, unk* xedxp, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4P = xedxp;
  c_wlreg _RG51;
  i8* RG52p;
  i16 RG6w;
  i8* RG71p;
  c_lreg _RG72;
  c_nreg _RG73;
  i16 vw_00;
  i16 parw00;
  i16 parw01;
  i16 parw02;

  RG51L = RG1L;
  RG6w = RG2W;
  vw_00 = RG3W;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG51UW));
  RG71p = RG1P;
  RG3W = word_at(RG1P, 2);
  RG1L = RG3L;
  RG1Blo ^= RG3Blo;
  RG1Bhi &= lcon(0x3c);
  RG1L = unsignedlong(RG1W) >> bcon(0xa);
  if (RG1L != lcon(0xf))
  {
    if (RG4P == NULL)
    {
      bool skip00024 = false;
      //m_6D0D:
      RG4W = vw_00;
      RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG2W), RG4L);
      RG4L = RG1L;
      RG1Blo = RG1W != wcon(0xffff) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L != 0)
      {
        RG2L = unsignedlong(RG4W);
        RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(RG2L);
        if ((RG1Blo & bcon(0x1)) != 0)
        {
          RG1L = unsignedlong(RG51W) >> bcon(0xe);
          RG4L = RG1L;
          RG1W = word_at(DM2_QUERY_CREATURE_AI_SPEC_FROM_TYPE(RG2L), lcon(0x1e)) & lcon(0xf);
          RG1L = unsignedlong(RG1W);
          if (RG1L == 1)
          {
            RG4W = timdat.timerarray[word_at(RG71p, lcon(0x6))].getB();
            RG4L <<= 4; // TODO: 4 bytes of the pointer remaining???
            RG4UW >>= 14;
          }
          RG1L = signedlong(vw_00);
          put16(parw00, RG1W);
          RG2L = signedlong(RG6w);
          RG3Bhi &= lcon(0x3f);
          RG4L <<= bcon(0xe);
          RG3L |= RG4L;
          RG1L = unsignedlong(RG3W);
          RG3L = RG2L;
          RG2L = lcon(0xffffffff);
          RG4L = lcon(0xfffffffe);
          skip00024 = true;
        }
      }

      if (!skip00024)
      {
        //m_6DA7:
        RG1L = signedlong(vw_00);
        put16(parw00, RG1W);
        RG1L = signedlong(RG6w);
        RG3Bhi &= lcon(0x3f);
        RG4L = RG51L;
        RG4W &= lcon(0xc000);
        RG4L |= RG3L;
        RG72L = unsignedlong(RG4W);
        RG3L = RG1L;
        RG2L = 0;
        RG4L = lcon(0xfffffffe);
        RG1L = RG72L;
      }

      //m_6DCC:
      DM2_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw00);
    }
    else
    {
      RG1L = unsignedlong(RG3W);
      DM2_APPEND_RECORD_TO(mk_record(RG1L), UPCAST(t_record, RG4P), -1, 0);
    }
    //m_6DD1:
    RG1L = signedlong(vw_00);
    put16(parw02, RG1W);
    RG1L = signedlong(RG6w);
    put16(parw01, RG1W);
    RG4L = unsignedlong(RG51W);
    RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L);
    RG73L = unsignedlong(RG1Blo);
    RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG4L) & lcon(0xff);
    DM2_QUEUE_NOISE_GEN2(RG1Blo, RG73Blo, lcon(0x85), lcon(0xfe), parw01, parw02, 1, lcon(0x3a), lcon(0x80));
  }
  //m_6E12:
  RG1L = unsignedlong(RG51W);
  DM2_DEALLOC_RECORD(RG1L);
}

// stacklen was 0x34
void DM2_DROP_CREATURE_POSSESSION(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG51;
  i32 RG52l;
  i32 RG53l;
  c_wlreg _RG54;
  c_wlreg _RG62;
  c_wlreg _RG71;
  c_wlreg _RG72;
  c_ql32 vql_00;
  c_ql32 vql_04;
  i16 vw_08;
  unk* xp_0c;
  i32 vl_10;
  i32 vl_14;
  c_ol32 vol_18; // poke 32 peek 16 shr16
  c_o32 vo_1c; // poke 16 peek 8 32 inc16
  i32 vl_20;
  i16 parw00;
  i16 parw01;
  i16 parw02;
  i16 parw03;
  i16 parw06;
  i16 parw07;
  i16 parw08;
  i16 parw09;

  vw_08 = RG1W; // truncated
  vql_04.poke32(RG4L);
  vql_00.poke32(RG2L);
  if (RG3W == 2)
    return;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(vw_08)));
  xp_0c = UPCAST(unk, RG1P);
  if (RG3W == 0)
  {
    vo_1c.poke16(lcon(0xa));
    //m_20F6B:
    for (;;)
    {
      RG1L = signedlong(vo_1c.peek16());
      if (RG1L > lcon(0x14))
        break;
      //m_20E2E:
      RG4L = unsignedlong(vo_1c.peek8());
      RG1L = 0;
      RG1Blo = byte_at(DOWNCAST(unk, xp_0c), lcon(0x4));
      RG1L = DM2_QUERY_GDAT_CREATURE_WORD_VALUE(RG1L, RG4L);
      vol_18.poke32(RG1L);
      if (RG1W != 0)
      {
        RG4L = RG1L;
        RG4Bhi ^= RG1Bhi;
        RG4Blo &= lcon(0xf);
        RG4L++;
        vl_14 = RG4L;
        RG1W &= lcon(0x70);
        RG1L = unsignedlong(RG1W) >> 4;
        if (RG1W != 0)
        {
          RG1L++;
          RG1W = DM2_RAND16(RG1W);
          RG2L = RG4L + RG1L;
          vl_14 = RG2L;
        }
        vol_18.shr16(lcon(0x7));
        //m_20E85:
        for (;;)
        {
          bool skip00545 = false;
          RG51L = vl_14 - 1;
          vl_14 = RG51L;
          if (RG51W == lcon(0xffffffff))
            break;
          RG1L = DM2_ALLOC_NEW_DBITEM(unsignedlong(vol_18.peek16()));
          RG62L = RG1L;
          if (RG1W == lcon(0xffffffff))
            break;
          RG4L = unsignedlong(vql_04.peek16());
          RG1L = signedlong(ddat.v1e0270);
          if (RG4L == RG1L)
          {
            RG4L ^= RG1L;
            RG4W = vql_00.peek16();
            RG1L = signedlong(ddat.v1e0272);
            if (RG4L == RG1L)
            {
              RG1L = DM2_RANDBIT() ? 1 : 0;
              RG1W += ddat.v1e0258;
              RG1W &= 0x3;
              skip00545 = true;
            }
          }

          if (!skip00545)
            //m_20EE7:
            RG1L = signedlong(DM2_RANDDIR());

          //m_20EEC:
          RG71L = signedlong(vql_00.peek16());
          put16(parw00, RG71W);
          RG54L = signedlong(vql_04.peek16());
          RG4L = RG62L;
          RG4Bhi &= lcon(0x3f);
          RG1L <<= bcon(0xe);
          RG1L |= RG4L;
          RG1L = unsignedlong(RG1W);
          RG3L = RG54L;
          RG2L = 0;
          RG4L = lcon(0xffffffff);
          DM2_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw00);
          if (argw0 >= wcon(0x0))
          {
            RG1L = signedlong(argw0);
            put16(parw03, RG1W);
            put16(parw02, RG71W);
            put16(parw01, RG54W);
            RG4L = unsignedlong(RG62W);
            RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L);
            RG53l = unsignedlong(RG1Blo);
            RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG4L) & lcon(0xff);
            RG3L = lcon(0xfe);
            RG2L = lcon(0x85);
            RG4L = RG53l;
            DM2_QUEUE_NOISE_GEN2(RG1Blo, RG4Blo, RG2Blo, RG3Blo, parw01, parw02, parw03, lcon(0x3a), lcon(0x80));
          }
        }
      }
      //m_20F66:
      vo_1c.inc16();
    }
  }
  //m_20F7B:
  RG62W = word_at(DOWNCAST(unk, xp_0c), 2);
  if (RG62W == lcon(0xfffffffe))
    return;
  //m_20F8D:
  for (;;)
  {
    vl_20 = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG62UW)));
    RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(vw_08));
    if ((RG1Blo & bcon(0x1)) == 0)
    {
      bool skip00547 = false;
      RG1L = unsignedlong(vql_04.peek16());
      RG4L = signedlong(ddat.v1e0270);
      if (RG1L == RG4L)
      {
        RG4L = unsignedlong(vql_00.peek16());
        RG1L = signedlong(ddat.v1e0272);
        if (RG4L == RG1L)
        {
          RG1L = DM2_RANDBIT() ? 1 : 0;
          RG1W += ddat.v1e0258;
          RG1W &= 0x3;
          skip00547 = true;
        }
      }

      if (!skip00547)
        //m_20FE3:
        RG1L = signedlong(DM2_RANDDIR());

      //m_20FE8:
      RG62L &= lcon(0xffff3fff);
      RG1L <<= bcon(0xe);
      RG62L |= RG1L;
    }
    //m_20FF3:
    RG1L = RG62L;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    if (RG1L != lcon(0xe))
    {
      //m_21013:
      RG1L = signedlong(vql_00.peek16());
      vl_10 = RG1L;
      put16(parw06, RG1W);
      RG72L = signedlong(vql_04.peek16());
      RG52l = unsignedlong(RG62W);
      RG3L = RG72L;
      RG2L = 0;
      RG4L = lcon(0xffffffff);
      DM2_MOVE_RECORD_TO(RG52l, RG4L, RG2L, RG3L, parw06);
      if (argw0 >= wcon(0x0))
      {
        RG1L = signedlong(argw0);
        put16(parw09, RG1W);
        RG2L = vl_10;
        put16(parw08, RG2W);
        put16(parw07, RG72W);
        RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG52l);
        RG4L = unsignedlong(RG1Blo);
        RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG52l) & lcon(0xff);
        RG3L = lcon(0xfe);
        RG2L = lcon(0x85);
        DM2_QUEUE_NOISE_GEN2(RG1Blo, RG4Blo, RG2Blo, RG3Blo, parw07, parw08, parw09, lcon(0x3a), lcon(0x80));
      }
    }
    else
    {
      RG1L = unsignedlong(RG62W);
      DM2_DEALLOC_RECORD(RG1L);
    }
    //m_2107E:
    RG62L = vl_20;
    if (RG62W == lcon(0xfffffffe))
      return;
  }
  FOREND
}

// stacksize was 0x1c
// recursive!!!
i16* DM2_OVERSEE_RECORD(i16* eaxpw, i32 edxl, i16** ebxppw, FPTR_L_WPWP ecxf, i16* argpw0, i32 argl1, i32 argl2)
{
  c_nreg _RG1; RG1P16 = eaxpw;
  c_nreg _RG2; RG2P = DOWNCAST(i16*, ebxppw);
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p;
  i8* RG6p;
  i16* RG7p16;
  i16* wp_00;
  FPTR_L_WPWP pf_4;
  i8 vb_08;

  RG5p = RG1P;
  vb_08 = RG4Blo;
  RG6p = RG2P;
  pf_4 = ecxf;
  RG7p16 = argpw0;
  wp_00 = RG1P16;
  //m_C9AF:
  for (;;)
  {
    bool skip00110 = false;
    RG4W = word_at(RG5p);
    if (RG4W == lcon(0xfffffffe) || RG4W == lcon(0xffffffff))
      return NULL;
    RG1L = unsignedlong(vb_08);
    if (RG1L != lcon(0xff))
    {
      RG4L = unsignedlong(word_at(RG5p)) >> bcon(0xe);
      if (RG4L != RG1L)
        skip00110 = true;
    }

    if (!skip00110)
    {
      //m_C9E3:
      RG1W = word_at(RG5p) & lcon(0x3c00);
      RG1L = unsignedlong(RG1W) >> bcon(0xa);
      if (RG1L == lcon(0x4))
      {
        RG4L = argl1;
        if (RG4L != 0)
        {
          RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(word_at(RG5p)))) + 2;
          i32 parl02 = argl2;
          i32 parl01 = RG4L;
          RG2P = RG6p;
          RG1P16 = DM2_OVERSEE_RECORD(RG1P16, lcon(0xff), UPCAST(i16*, RG2P), pf_4, RG7p16, parl01, parl02);
          if (RG1P != NULL)
            return RG1P16;
        }
      }
      if (argl2 != 0)
      {
        RG1L = DM2_IS_CONTAINER_CHEST(word_at(RG5p)) ? 1 : 0;
        if (RG1L != 0)
        {
          RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(word_at(RG5p))));
          i32 parl05 = argl2;
          i32 parl04 = argl1;
          RG1P += 2;
          RG2P = RG6p;
          RG1P16 = DM2_OVERSEE_RECORD(RG1P16, lcon(0xff), UPCAST(i16*, RG2P), pf_4, RG7p16, parl04, parl05);
          if (RG1P != NULL)
            return RG1P16;
        }
      }
      RG4P = DOWNCAST(i16, RG7p16);
      RG1L = INDCALL(pf_4)(UPCAST(i16, RG5p), RG4P16);
      if (RG1L != 0)
      {
        movp(location(RG6p), DOWNCAST(i16, wp_00));
        return UPCAST(i16, RG5p);
      }
    }

    //m_CA7E:
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(word_at(RG5p))));
    RG5p = RG1P;
  }
  FOREND
}

// belongs to DM2_move_2fcf_0434
i32 DM2_ROTATE_RECORD_BY_TELEPORTER(unk* xeaxp, i32 edxl)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  RG2P = xeaxp;

  RG1L = RG4L;
  RG4W = ddat.v1e1024;
  RG3W = word_at(RG2P, 2);
  RG3L <<= 4;
  RG3UW >>= 14;
  if (jz_test8(location(RG2P + lcon(0x3)), lcon(0x10)))
  {
    //m_3A050:
    RG4L += RG3L;
    RG4W &= 0x3;
    RG2L = unsignedlong(RG1W);
    RG1L = (signedlong(RG3W) + (RG2L >> bcon(0xe)) & lcon(0x3)) << bcon(0xe);
    RG2Bhi &= lcon(0x3f);
    RG1L |= RG2L;
  }
  else
    RG4L = RG3L;
  //m_3A071:
  ddat.v1e1024 = RG4W;
  return RG1L;
}

void DM2_075f_056c(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4;

  RG4L = RG1L;
  RG4Blo ^= RG1Blo;
  RG4Bhi &= lcon(0x3c);
  RG4L = unsignedlong(RG4W) >> bcon(0xa);
  if (RG4L == lcon(0xe))
  {
    RG1L = unsignedlong(RG1W);
    RG1W = word_at(DOWNCAST(t_record, DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW))), lcon(0x6));
    RG1L = unsignedlong(RG1W);
    DM2_DELETE_TIMER(RG1W);
  }
}
