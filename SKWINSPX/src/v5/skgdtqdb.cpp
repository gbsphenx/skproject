#include <util.h>
#include <skrandom.h>
#include <skalocdb.h>
#include <skgdtqdb.h>
#include <skstr.h>
#include <bitem.h>
#include <sktimer.h>
#include <bcreatur.h>
#include <gfxstr.h>
#include <dm2data.h>
#include <skeventq.h>
#include <skrecord.h>
#include <skmap.h>
#include <skimage.h>
#include <gfxblit.h>
#include <gfxmain.h>
#include <skhero.h>
#include <skxrect.h>
#include <startend.h>

#include <bgdat.h>

// Note: smth with 5x5 POS
void DM2_query_098d_000f(i16 eaxw, i16 edxw, i16 ebxw, OO i16& w1, OO i16& w2)
{
  w1 = ebxw % 5 + 4 * eaxw;
  w2 = ebxw / 5 + 4 * edxw;
}

bool DM2_IS_CLS1_CRITICAL_FOR_LOAD(i8 eaxb)
{
  if (eaxb == bcon(0x1b) || eaxb == bcon(0x6) || eaxb == bcon(0x5))
    return true;
  return false;
}

unk* DM2_QUERY_GDAT_DYN_BUFF(t_dbidx dbidx_in, IO t_dbidx& dbidx_out, bool ebxbool)
{
  s_malloctail* s78prg2;
  if (!dm2_dballochandler.gfxalloc_done)
  {
    i32 longrg6 = unsignedlong(CUTX16(dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH(dbidx_in))) + lcon(0x6);
    if (!ebxbool)
      s78prg2 = XUPCAST(s_malloctail, dm2_dballochandler.DM2_ALLOC_HIBIGPOOL_MEMORY(longrg6) + lcon(0x6));
    else
      s78prg2 = XUPCAST(s_malloctail, dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(longrg6) + lcon(0x6));
    DM2_LOAD_GDAT_RAW_DATA(dbidx_in, DOWNCAST(s_malloctail, s78prg2));
    if (!ebxbool)
      (s78prg2 - 1)->w_04 = 2; // TODO: neg. offset
    else
      (s78prg2 - 1)->w_04 = 0; // TODO: neg. offset
    (s78prg2 - 1)->l_00 = longrg6; // TODO: neg. offset
  }
  else if (dm2_dballochandler.DM2_dballoc_3e74_55f9(unsignedlong(dbidx_in) - lcon(0x20000), OO dbidx_out))
  {
    s78prg2 = CHGCAST(s_malloctail, dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ dbidx_out));
    if (ebxbool)
      dm2_dballochandler.DM2_ALLOCATE_GFX256(dbidx_out);
  }
  else
  {
    s78prg2 = CHGCAST(s_malloctail, dm2_dballochandler.DM2_ALLOC_CPXHEAP_MEM(dbidx_out, unsignedlong(CUTX16(dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH(dbidx_in))))); // TODO: upcast from s_xmalloc
    DM2_LOAD_GDAT_RAW_DATA(dbidx_in, DOWNCAST(s_malloctail, s78prg2));
    if (!ebxbool)
      dm2_dballochandler.DM2_ALLOCATION1(dbidx_out, false);
  }
  return DOWNCAST(s_malloctail, s78prg2);
}

s_bbw* DM2_QUERY_GDAT_ENTRYPTR(i8 eaxb, i8 edxb, i8 ebxb, i8 ecxb)
{
  if (unsignedlong(eaxb) > unsignedlong(dm2_dballochandler.gdattable.entries))
    return NULL;

  i16* wp1 = &dm2_dballochandler.gdattable.w_table1[unsignedword(eaxb)];
  if (unsignedlong(ebxb) >= unsignedlong(wp1[1]) - unsignedlong(wp1[0]))
    return NULL;

  i16 w3 = wp1[0] + unsignedword(ebxb); // TODO: unexpected extension happens here!
  i16* wp2 = &dm2_dballochandler.gdattable.w_table2[w3];
  s_bbw* u31prg7 = &dm2_dballochandler.gdattable.u31p_08[wp2[0] - 1];

  i16 w5 = wp2[1] - wp2[0] + 1;
  i16 w32 = 0;
  for (;;)
  {
    i16 w4 = (w5 + w32) >> 1; // TODO: check if shift or div
    if (w4 == w32)
      return NULL;
    s_bbw* u31prg11 = &u31prg7[w4];
    i16 w6 = unsignedword(u31prg11->b_00) - unsignedword(edxb);
    if (w6 == 0)
    {
      w6 = unsignedword(u31prg11->b_01) - unsignedword(ecxb); // TODO: critical, first one was not extended
      if (w6 == 0)
        return u31prg11;
    }
    if (w6 <= 0)
      w32 = w4;
    else
      w5 = w4;
  }
}

i16 DM2_QUERY_GDAT_ENTRY_DATA_INDEX(i8 eaxb, i8 edxb, i8 ebxb, i8 ecxb)
{
  s_bbw* u31prg4 = DM2_QUERY_GDAT_ENTRYPTR(eaxb, edxb, ebxb, ecxb);
  if (u31prg4 == NULL)
  {
    if (ebxb == 11 || ebxb == 12)
      return 0;
    return -1;
  }
  if (ebxb == 11 || ebxb == 12)
    return u31prg4->w_02;
  return u31prg4->w_02 & 0x7fff;
}

c_pixel* DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(i8 eaxb, i8 edxb, i8 ebxb)
{
  if (timdat.gametick != dm2_dballochandler.malloci)
    dm2_dballochandler.DM2_ALLOCATION10();
  s_bbw* u31prg4 = DM2_QUERY_GDAT_ENTRYPTR(eaxb, edxb, 1, ebxb);
  bool boolrg6 = false;
  i16 wordrg1;
  if (u31prg4 != NULL)
  {
    boolrg6 = DM2_IS_CLS1_CRITICAL_FOR_LOAD(eaxb);
    wordrg1 = u31prg4->w_02 & lcon(0x7fff);
  }
  else
    wordrg1 = lcon(0xffff);

  if (wordrg1 == wcon(0xffff) || dm2_ulp.islen(wordrg1) && !boolrg6)
    wordrg1 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(21, lcon(0xfe), 1, lcon(0xfe));
  return DM2_EXTRACT_GDAT_IMAGE((t_dbidx)wordrg1, 0);
}

// TODO: can return pointers of different types
t_uniptr DM2_QUERY_GDAT_ENTRY_DATA_PTR(i8 eaxb, i8 edxb, i8 ebxb, i8 ecxb)
{
  i16 vw_00;

  i16 wordrg2 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, edxb, ebxb, ecxb);
  if (ebxb == 1 && !DM2_IS_CLS1_CRITICAL_FOR_LOAD(eaxb) && (wordrg2 == wcon(0xffff) || dm2_ulp.islen(wordrg2)))
    wordrg2 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(21, lcon(0xfe), 1, lcon(0xfe));
  if (dm2_ulp.islen(unsignedlong(wordrg2)))
    if (DM2_IS_CLS1_CRITICAL_FOR_LOAD(eaxb))
      return DM2_QUERY_GDAT_DYN_BUFF(wordrg2, IO /*TAG dbidx*/ vw_00, false);
  return dm2_ulp.getp(wordrg2);
}

i32 DM2_QUERY_GDAT_ENTRY_DATA_LENGTH(i8 eaxb, i8 edxb, i8 ebxb, i8 ecxb)
{
  return dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH((t_dbidx)DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, edxb, ebxb, ecxb));
}

unk* DM2_QUERY_GDAT_ENTRY_DATA_BUFF(i8 eaxb, i8 edxb, i8 ebxb, i8 ecxb)
{
  i16 vw_00;

  i16 wordrg1 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, edxb, ebxb, ecxb);
  if (wordrg1 == lcon(0xffffffff))
    return NULL;
  if (dm2_ulp.islen(unsignedlong(wordrg1)))
  {
    if (!DM2_IS_CLS1_CRITICAL_FOR_LOAD(eaxb))
      return NULL;
    return DM2_QUERY_GDAT_DYN_BUFF((t_dbidx)wordrg1, IO /*TAG dbidx*/ vw_00, false);
  }
  return dm2_ulp.getp(unsignedlong(wordrg1));
}

bool DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(i8 eaxb, i8 edxb, i8 ebxb, i8 ecxb)
{
  s_bbw* u31prg4 = DM2_QUERY_GDAT_ENTRYPTR(eaxb, edxb, ebxb, ecxb);
  if (u31prg4 == NULL)
    return false;
  if (ebxb == bcon(0xb) || ebxb == bcon(0xc))
    return true;
  if (!dm2_ulp.islen(u31prg4->w_02 & wcon(0x7fff)))
    return true;
  if (!DM2_IS_CLS1_CRITICAL_FOR_LOAD(eaxb))
    return false;
  return true;
}

i16 DM2_QUERY_GDAT_PICT_OFFSET(i8 eaxb, i8 edxb, i8 ebxb)
{
  s_bbw* u31prg1 = DM2_QUERY_GDAT_ENTRYPTR(eaxb, edxb, 1, ebxb);
  if (u31prg1 == NULL)
    return 0;
  s_wwbb* s84prg5;
  bool vbool = false;
  if (!dm2_ulp.islen(unsignedlong((i16)u31prg1->w_02)))
    s84prg5 = XUPCAST(s_wwbb, dm2_ulp.getp(unsignedlong((i16)u31prg1->w_02)));
  else
  {
    vbool = true;
    s84prg5 = XUPCAST(s_wwbb, DM2_QUERY_GDAT_ENTRY_DATA_PTR(eaxb, edxb, 1, ebxb));
  }
  i16 wordrg1 = s84prg5->w_00 >> 10;
  i16 wordrg4 = wordrg1;
  if (wordrg1 != wcon(0xffe0))
  {
    bool nowrt5 = false;
    i16 wordrg11 = s84prg5->w_02 >> 10;
    if (wordrg11 != dm2_dballochandler.v1e0acc)
    {
      if (wordrg11 != lcon(0xffffffe0))
        nowrt5 = true;
    }
    else
      wordrg4 = signedword(s84prg5->b_04);
    i8 byterg1 = CUTX8(wordrg11);
    if (!nowrt5)
      byterg1 = s84prg5->b_05;
    wordrg4 <<= bcon(0x8);
    wordrg4 += unsignedword(byterg1);
  }
  else
    wordrg4 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, edxb, lcon(0xc), ebxb);
  if (vbool)
    dm2_dballochandler.DM2_DEALLOC_BIGPOOL_STRUCT_BEFORE(CHGCAST(s_malloctail, s84prg5));
  return wordrg4;
}

// recursive!!!
t_palette* DM2_QUERY_GDAT_IMAGE_LOCALPAL(i8 eaxb, i8 edxb, i8 ebxb)
{
  i16 vw_00 = 0;

  i16 didx = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, edxb, 1, ebxb);
  if (didx == -1)
    goto M_fin;
  
  s_wwbb* s84prg4;
  if (!dm2_ulp.islen(unsignedlong(didx)))
    s84prg4 = XUPCAST(s_wwbb, dm2_ulp.getp(unsignedlong(didx)));
  else
  {
    if (!DM2_IS_CLS1_CRITICAL_FOR_LOAD(eaxb))
      goto M_fin;
    s84prg4 = XUPCAST(s_wwbb, DM2_QUERY_GDAT_DYN_BUFF((t_dbidx)didx, IO /*TAG dbidx*/ vw_00, false));
  }
  if ((s84prg4->w_02 >> 10) == wcon(0xffe0))
  {
    if (s84prg4->b_04 != 4)
      return NULL;
  }
  if ((s84prg4->w_02 >> 10) == dm2_dballochandler.v1e0acc)
    return NULL;

  return UPCAST(t_palette, DOWNCAST(s_wwbb, s84prg4) + (dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH(didx) & lcon(0xffff)) - lcon(0x10));

M_fin:
  return DM2_QUERY_GDAT_IMAGE_LOCALPAL(21, lcon(0xfe), lcon(0xfe));
}

// TODO: optimize temporaries
#include "regs.h"
#include "emu.h"

t_text* DM2_QUERY_GDAT_ITEM_NAME(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4; RG4L = edxl;

  t_text tarr_00[0x20];
  RG4L = unsignedlong(RG4Blo);
  RG1L = unsignedlong(RG1Blo);
  return DM2_FORMAT_SKSTR(DM2_QUERY_GDAT_TEXT(RG1Blo, RG4Blo, bcon(0x18), tarr_00), ddat.v1e0930);
}

i32 DM2_QUERY_CMDSTR_TEXT(t_text* eaxtp, const t_text* edxtp)
{
  c_nreg _RG1; RG1T = eaxtp;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  bool flag;

  RG3L = 0;
  //m_2F84A:
  for (;;)
  {
    RG1T = DM2_STRSTR(RG1T, edxtp);
    RG4P = RG1P;
    if (RG1P == NULL)
      return RG3L;
    RG1P = RG4P + signedlong(CUTX16(DM2_STRLEN(edxtp)));
    RG4L = unsignedlong(byte_at(RG1P));
    if (RG4L == lcon(0x3d))
      RG1P++;
    flag = false;
    RG4L = unsignedlong(byte_at(RG1P));
    if (RG4L == lcon(0x2d))
    {
      flag = true;
      RG1P++;
    }
    //m_2F87D:
    for (;;)
    {
      RG2Blo = byte_at(RG1P);
      RG4L = unsignedlong(RG2Blo);
      if (RG4L < lcon(0x30) || RG4L > lcon(0x39))
        break;
      RG3L = RG3L * lcon(0xa);
      RG4L = unsignedlong(RG2Blo) - lcon(0x30);
      RG3L += RG4L;
      RG1P++;
    }
    //m_2F89C:
    if (flag)
      RG3L = -RG3L;
  }
  FOREND
}

t_text* DM2_QUERY_CMDSTR_NAME(i8 eaxb, i8 edxb, i8 ebxb)
{
  c_nreg _RG1;
  t_text tarr_00[0x80];

  RG1T = DM2_QUERY_GDAT_TEXT(eaxb, edxb, ebxb, tarr_00);
  if (*RG1T == 0)
    return NULL;
  //m_2F80A:
  //m_2F818:
  for (i16 i=0; ; i++)
  {
    t_text c = *RG1T++;
    if (c == 0x3a || c == 0)
    {
      ddat.v1e0b85[i] = 0;
      return ddat.v1e0b85;
    }
    //m_2F80E:
    ddat.v1e0b85[i] = c;
  }
  FOREND
}

i32 DM2_QUERY_CMDSTR_ENTRY(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  t_text tarr_00[0x80];

  RG5l = unsignedlong(RG3W);
  if (RG5l >= lcon(0x12))
    return 0;
  RG2L = unsignedlong(RG2Blo);
  RG4L = unsignedlong(RG4Blo);
  RG1T = DM2_QUERY_GDAT_TEXT(RG1Blo, RG4Blo, RG2Blo, tarr_00);
  if (byte_at(RG1P) == 0)
    return 0;
  return DM2_QUERY_CMDSTR_TEXT(RG1T, table1d6912[RG5l]);
}

i32 DM2_QUERY_CUR_CMDSTR_ENTRY(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG3;

  RG3L = unsignedlong(RG1W);
  return DM2_QUERY_CMDSTR_ENTRY(unsignedlong(ddat.v1e0b7f), unsignedlong(ddat.v1e0b80), unsignedlong(ddat.v1e0b7e), RG3L);
}

t_bmp* DM2_QUERY_PICT_BITS(c_imgdesc* imgdesc)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;

  RG1W = imgdesc->mode;
  RG4L = RG1L;
  RG4Bhi ^= RG1Bhi;
  RG4Blo &= 0x4;
  RG4L = unsignedlong(RG4W);
  t_bmp* bmp;
  if (RG4L == 0)
  {
    //m_AE4D:
    RG1W &= lcon(0x8);
    RG1L = unsignedlong(RG1W);
    if (RG1L == 0)
      return imgdesc->bmp;
    bmp = dm2_dballochandler.DM2_GET_BMP(imgdesc->bmpid);
  }
  else
  {
    RG2L = unsignedlong(imgdesc->b_0b);
    RG4L = unsignedlong(imgdesc->b_09);
    bmp = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(imgdesc->b_08, RG4Blo, RG2Blo));
  }
  //m_AE47:
  imgdesc->bmp = bmp;
  return bmp;
}

// stacksize was 0x14
i32 DM2_QUERY_GDAT_IMAGE_METRICS(i8 eaxb, i8 edxb, i8 ebxb, i16* ecxpw, i16* argpw0)
{
  c_nreg _RG2; RG2Blo = ebxb;
  c_nreg _RG1; RG1Blo = eaxb;
  c_nreg _RG3;
  c_nreg _RG4; RG4Blo = edxb;
  i32 RG5l;
  c_nreg _RG6;
  c_nreg _RG7;
  i16* wp_00;

  wp_00 = UPCAST(i16, DOWNCAST(i16, ecxpw));
  RG7L = unsignedlong(RG2Blo);
  RG6L = unsignedlong(RG4Blo);
  RG5l = unsignedlong(RG1Blo);
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(CUTLX8(RG5l), RG6Blo, 1, RG7Blo) & lcon(0xffff);
  if (!dm2_ulp.islen(mkl(RG1W)))
  {
    //m_2F6AB:
    RG3L = RG7L;
    RG2L = 1;
    RG4L = RG6L;
    RG1P = DM2_QUERY_GDAT_ENTRY_DATA_PTR(CUTLX8(RG5l), RG4Blo, RG2Blo, RG3Blo);
    RG4W = word_at(RG1P);
    RG4Bhi &= 0x3;
    mov16(location(DOWNCAST(i16, wp_00)), RG4W);
    RG1W = word_at(RG1P, 2);
    RG1Bhi &= 0x3;
  }
  else
  {
    RG2L = RG7L;
    RG4L = RG6L;
    t_bmp* bmp = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(CUTLX8(RG5l), RG4Blo, RG2Blo));
    RG4W = getbmpheader(bmp)->width;
    mov16(location(DOWNCAST(i16, wp_00)), RG4W);
    RG1W = getbmpheader(bmp)->height;
  }
  //m_2F6CE:
  mov16(location(DOWNCAST(i16, argpw0)), RG1W);
  return RG1L;
}

i32 DM2_QUERY_ORNATE_ANIM_FRAME(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_nreg _RG6;
  c_nreg _RG7;
  i32 vl_00;
  i32 vl_04;
  t_text tarr_08[0x80];
  i32 vl_88;
  i32 vl_8c;

  vl_04 = RG2L;
  vl_00 = RG3L;
  RG2L = 0;
  vl_8c = RG2L;
  vl_88 = RG2L;
  RG7L = unsignedlong(RG4Blo);
  RG6L = unsignedlong(RG1Blo);
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(RG6Blo, RG7Blo, lcon(0xb), lcon(0xd));
  RG4W = RG1W;
  if (RG1W != 0)
  {
    //m_E50E:
    RG2P = NULL;
    vl_88 = 1;
    RG3L = RG1L;
    RG3Blo ^= RG1Blo;
    RG3Bhi &= lcon(0xffffff80);
    RG3L = unsignedlong(RG3W);
    if (RG3L != 0)
    {
      vl_8c = 1;
      RG1Bhi &= lcon(0x7f);
      RG4L = RG1L;
    }
  }
  else
  {
    bool skip00141 = false;
    RG3P = ADRESSOF(t_text, tarr_08);
    RG2L = lcon(0xd);
    RG4L = RG7L;
    RG1T = DM2_QUERY_GDAT_TEXT(RG6Blo, RG4Blo, RG2Blo, RG3T);
    RG2P = RG1P;
    RG1Blo = byte_at(RG1P) != bcon(0x0) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == 0)
      skip00141 = true;
    else
    {
      RG1L = DM2_STRLEN(RG2T);
      RG4L = RG1L;
      if (RG1W == 0)
        skip00141 = true;
      else
        vl_88 = 1;
    }

    if (skip00141)
      //m_E507:
      RG4L = 1;
  }
  //m_E537:
  if (vl_88 == 0)
    return 1;
  RG1L = vl_04 + vl_00;
  RG3L = unsignedlong(RG4W);
  RG4L = RG1L % RG3L;
  RG1L /= RG3L;
  RG4L += vl_8c;
  if (RG2P == NULL)
    return RG4L;
  RG5L = RG4L;
  RG1L = unsignedlong(RG4W);
  RG2P += RG1L;
  RG5W = unsignedword(byte_at(RG2P));
  RG1L = unsignedlong(byte_at(RG2P));
  if (RG1L >= lcon(0x30))
  {
    RG1L = unsignedlong(RG5W);
    if (RG1L <= lcon(0x39))
    {
      RG5L -= lcon(0x30);
      return RG5L;
    }
  }
  RG1L = unsignedlong(RG5W);
  if (RG1L >= lcon(0x41))
  {
    if (RG1L <= lcon(0x5a))
      RG5L -= lcon(0x4b);
  }
  return RG5L;
}

t_bmp* DM2_QUERY_PICST_IMAGE(c_imgdesc* imgdesc)
{
  t_bmp* bmp = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(imgdesc->b_08, imgdesc->b_09, imgdesc->b_0b));
  imgdesc->bmp = bmp;
  imgdesc->mode = 0x4;
  imgdesc->x = 0;
  imgdesc->y = 0;
  imgdesc->width = getbmpheader(bmp)->width;
  imgdesc->height = getbmpheader(bmp)->height;
  imgdesc->setres(getbmpheader(bmp)->res);
  return bmp;
}

i32 DM2_QUERY_4BPP_PICT_BUFF_AND_PAL(i32 eaxl, i32 edxl, c_pixel** xebxp, t_palette* palette)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG51l;
  c_nreg _RG6;

  RG6L = unsignedlong(RG4Blo);
  RG51l = unsignedlong(RG1Blo);
  RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(CUTLX8(RG51l), RG6Blo, 1, lcon(0xf9)) ? 1 : 0;
  if (RG1L == 0)
    return 0;
  RG2L = lcon(0xf9);
  RG4L = RG6L;
  *xebxp = DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(CUTLX8(RG51l), RG4Blo, RG2Blo);
  DM2_COPY_SMALL_PALETTE(palette, DM2_QUERY_GDAT_IMAGE_LOCALPAL(CUTLX8(RG51l), RG6Blo, lcon(0xf9)));
  RG4L = signedlong(getbmpheader(BMPCAST(*xebxp))->width);
  return RG4L / signedlong(ddat.v1d274a);
}

i16 DM2_QUERY_PLAYER_SKILL_LV(i16 heroidx, i16 edxw, i32 ebxl)
{
  c_nreg _RG1; RG1W = heroidx;
  c_nreg _RG3;
  c_nreg _RG4; RG4W = edxw;
  i32 RG5l;

  RG3L = RG4L;
  RG5l = ebxl;
  RG4L = ddat.v1e0238;
  if (RG4L != 0)
    return 1;
  //m_3630C:
  c_hero* hero = &party.hero[RG1W];
  RG4W = RG3W;
  RG1L = hero->skill[RG4L / 4][RG4L % 4];
  if (RG4L > 3)
  {
    if (RG5l == 0)
      //m_36347:
      RG4L = 1;
    else
    {
      RG4L -= 4;
      RG4L >>= 2;
      RG4L = signedlong(hero->sbonus[RG4L / 4][RG4L % 4]) + 1;
    }
    //m_3634C:
    i16 n = RG3W / 4 - 1;
    RG4L *= hero->skill[n / 4][n % 4];
    RG1L += RG4L;
    RG1UL >>= 1;
  }
  //m_36360:
  RG4W = 1;
  //m_36365:
  while (mkul(RG1W) >= mkul(lcon(0x200)))
  {
    RG1UL >>= bcon(0x1);
    RG4W++;
  }
  //m_36371:
  if (RG5l != 0)
  {
    RG1L = unsignedlong(RG3W);
    RG1W = signedword(hero->sbonus[RG1L / 4][RG1L % 4]);
    RG4W += RG1W;
    RG4W = signedlong(DM2_MAX(1, RG4W));
  }
  return RG4W;
}

i32 DM2_IS_WALL_ORNATE_ALCOVE(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4;

  if (RG1Blo == lcon(0xffffffff))
    return 0;
  RG4L = unsignedlong(RG1Blo);
  return mkl(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, RG4Blo, 11, 10));
}

i32 DM2_IS_TILE_BLOCKED(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4;
  bool flag;

  RG4L = unsignedlong(RG1Blo) >> 5;
  RG1Bhi = RG4Blo;
  if (mkub(RG4Blo) < mkub(lcon(0x4)))
    return RG4Blo != bcon(0x0) ? 0 : 1;
  if (mkub(RG4Blo) <= mkub(lcon(0x4)))
  {
    //m_3B04F:
    RG1Blo &= lcon(0x7);
    if (RG1Blo == 0)
      return 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == 1)
      return 0;
    return RG1L == lcon(0x5) ? 0 : 1;
  }
  if (mkub(RG4Blo) < mkub(lcon(0x6)))
    return 0;
  if (mkub(RG4Blo) > mkub(lcon(0x6)))
    return RG4Blo != lcon(0x7) ? 0 : 1;
  //m_3B064:
  RG1Bhi = RG1Blo & lcon(0x4);
  RG4L = unsignedlong(RG1Bhi);
  if (RG4L != 0)
    return 0;
  RG1Blo &= 0x1;
  RG1L = unsignedlong(RG1Blo);
  return RG1L != 0 ? 0 : 1;
}

// was SKW_0cee_3202
i32 DM2_IS_REBIRTH_ALTAR(unk* xeaxp)
{
  c_nreg _RG1; RG1P = xeaxp;

  if (jnz_test8(location(RG1P + 2), 1))
  {
    //m_FEEA:
    RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
    if (jnz_test8(location(RG1P + lcon(0x3)), 1))
    {
      RG1W = word_at(RG1P, lcon(0xe));
      RG1UW >>= bcon(0xc);
      return RG1L;
    }
  }
  else
  {
    RG1P = DOWNCAST(s_sizee, ddat.v1e03c0);
    if (jnz_test8(location(RG1P + 2), lcon(0xffffff80)))
    {
      RG1W = word_at(RG1P, lcon(0xe));
      RG1L <<= 4;
      RG1UW >>= bcon(0xc);
      return RG1L;
    }
  }
  //m_FEFE:
  RG1Blo = lcon(0xffffffff);
  return RG1L;
}

i32 DM2_QUERY_DOOR_DAMAGE_RESIST(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4;

  RG4L = unsignedlong(RG1Blo);
  return mkl(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(14, RG4Blo, 11, lcon(0xf)));
}

i32 DM2_QUERY_GDAT_CREATURE_WORD_VALUE(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p;
  c_lreg _RG6;
  i16 vw_00;

//  if (ddat.v1e02ec != NULL && mkub(RG1Blo) <= mkub(ddat.v1e0414[0x11]))
	if (ddat.v1e02ec != NULL && mkub(RG1Blo) <= ddat.cd.gg.glbCreaturesMaxCount)
  {
    bool skip00177 = false;
    if (mkub(RG4Blo) < CREATURE_STAT_AI_SEQ_01)
    {
      //m_F9DE:
      if (RG4Blo == CREATURE_STAT_00)
      {
        //m_F9E8:
        RG4Bhi = 1;
        skip00177 = true;
      }
    }
    else
    {
      if (mkub(RG4Blo) <= CREATURE_STAT_AI_SEQ_01)
      {
        //m_F9E4:
        RG4W = unsignedword(RG4Blo);
        skip00177 = true;
      }
      else
      {
        if (RG4Blo == CREATURE_STAT_AI) //
        {
          //m_F9EC:
          RG4Bhi = 2;
          skip00177 = true;
        }
      }
    }

    if (skip00177)
    {
      //m_F9EE:
      RG6L = unsignedlong(RG1Blo);
      RG3L = 3 * RG6L;
      RG1P = ddat.v1e02ec + RG3L;
      RG5p = RG1P + unsignedlong(RG4Bhi);
      RG1W = unsignedword(RG1Blo);
      RG1Blo = byte_at(RG5p);
      vw_00 = RG1W;
      if (RG1W == lcon(0xff))
      {
        RG3L = unsignedlong(RG4Blo);
        RG2L = lcon(0xb);
        RG4L = RG6L;
        RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(15, RG4Blo, 11, RG3Blo);
        vw_00 = RG1W;
        RG1Blo = CUTX8(vw_00);
        mov8(location(RG5p), RG1Blo);
      }
      return signedlong(vw_00);
    }
  }
  //m_FA35:
  RG3L = unsignedlong(RG4Blo);
  RG4L = unsignedlong(RG1Blo);
  return mkl(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(15, RG4Blo, 11, RG3Blo));
}

i32 DM2_IS_WALL_ORNATE_SPRING(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;

  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(unsignedlong(RG1W));
  if (RG1Blo != lcon(0xffffffff))
    return mkul(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, RG1Blo, 11, 12));
  return 0;
}

i32 DM2_IS_MISCITEM_CURRENCY(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4;

  RG4L = RG1L;
  RG4Blo ^= RG1Blo;
  RG4Bhi &= lcon(0x3c);
  RG4L = unsignedlong(RG4W) >> bcon(0xa);
  if (RG4L != lcon(0xa))
    return 0;
  RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG1W), 0);
  RG1W &= lcon(0x4000);
  RG1L = unsignedlong(RG1W);
  return RG1L == 0 ? 0 : 1;
}

i32 DM2_IS_CONTAINER_MONEYBOX(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG4L = RG1L;
  RG4Blo ^= RG1Blo;
  RG4Bhi &= lcon(0x3c);
  RG4L = unsignedlong(RG4W) >> bcon(0xa);
  if (RG4L != lcon(0x9))
    return 0;
  RG4L = unsignedlong(RG1W);
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
  if (jnz_test8(location(RG1P + lcon(0x4)), lcon(0x6)))
    return 0;
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L);
  RG4L = unsignedlong(RG1Blo);
  RG3L = lcon(0x40);
  RG2L = 5;
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(20, RG4Blo, 5, 64);
  return RG1W == lcon(0xffffffff) ? 0 : 1;
}

i32 DM2_IS_CONTAINER_MAP(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4;

  RG4L = RG1L;
  RG4Blo ^= RG1Blo;
  RG4Bhi &= lcon(0x3c);
  RG4L = unsignedlong(RG4W) >> bcon(0xa);
  if (RG4L != lcon(0x9))
    return 0;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
  RG1W = word_at(RG1P, lcon(0x4)) & lcon(0x6);
  return RG1W != 2 ? 0 : 1;
}

bool DM2_IS_CONTAINER_CHEST(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG4;

  RG4L = RG1L;
  RG4Blo ^= RG1Blo;
  RG4Bhi &= lcon(0x3c);
  RG4L = unsignedlong(RG4W) >> bcon(0xa);
  if (RG4L != lcon(0x9))
    return false;
  RG4L = unsignedlong(RG1W);
  RG1L = DM2_IS_CONTAINER_MONEYBOX(RG4L);
  if (RG1L != 0)
    return false;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
  return jz_test8(location(RG1P + lcon(0x4)), lcon(0x6));
}

i32 DM2_IS_DISTINCTIVE_ITEM_ON_ACTUATOR(unk* xeaxp, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;

  RG3L = RG4L;
  RG5l = RG2L;
  RG1W = word_at(RG1P, lcon(0x6));
  RG4L = RG1L;
  _RG4.ushiftr11();
  RG4L = signedlong(RG4W);
  RG1L <<= bcon(0x5);
  _RG1.ushiftr11();
  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(RG1W, RG4W));
  //m_3EC3F:
  for (;;)
  {
    bool skip00883 = false;
    RG2L = RG1L;
    if (RG2W == lcon(0xfffffffe))
      //m_3ECC5:
      return 0;
    RG1L = RG2L;
    RG1Blo ^= RG2Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    if (RG5l != 0)
    {
      RG4L = signedlong(RG1W);
      if (RG4L >= lcon(0x5) && RG4L <= lcon(0xa))
      {
        RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG2W);
        if (RG1W == RG3W)
          return 1;
        skip00883 = true;
      }
    }

    if (!skip00883)
    {
      //m_3EC82:
      RG1L = signedlong(RG1W);
      if (RG1L == lcon(0x4))
      {
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
        RG1W = word_at(RG1P, 2);
        //m_3EC96:
        for (;;)
        {
          if (RG1W == lcon(0xfffffffe))
            break;
          RG4L = unsignedlong(RG1W);
          RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG4W);
          if (RG1W == RG3W)
            return 1;
          RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
        }
      }
    }

    //m_3ECB6:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG2UW)));
  }
  FOREND
}

i32 DM2_IS_MISSILE_VALID_TO_LAUNCHER(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  RG3L = RG4L;
  if (RG2W == lcon(0xffffffff))
    return 0;
  RG1W = party.hero[RG1W].item[RG3W];
  if (RG1W == -1)
    return 0;
  RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG1W), lcon(0x5));
  RG3L = RG1L;
  RG1W &= lcon(0x8000);
  RG1L = unsignedlong(RG1W);
  if (RG1L == 0)
    return 0;
  RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG2W), lcon(0x5));
  RG4L = RG1L;
  RG4Blo ^= RG1Blo;
  RG4Bhi &= lcon(0xffffff80);
  RG4L = unsignedlong(RG4W);
  if (RG4L != 0)
    return 0;
  RG4W = RG1W;
  RG4Bhi &= lcon(0x7f);
  RG1L = unsignedlong(RG3W);
  return (RG1L & RG4L) != 0 ? 1 : 0;
}

// was SKW_0cee_2e1e
i32 DM2_GET_CREATURE_WEIGHT(i32 eaxl)
{
  c_nreg _RG1;

  RG1P = DM2_QUERY_CREATURE_AI_SPEC_FROM_TYPE(eaxl & lcon(0xffff));
  RG1Blo = byte_at(RG1P, lcon(0x1d));
  RG1W = unsignedword(RG1Blo);
  return RG1L;
}

i32 DM2_GET_ORNATE_ANIM_LEN(unk* xeaxp, i32 edxl)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  c_lreg _RG6;
  t_text tarr_00[0x80];

  if (RG4L == 0)
  {
    //m_46DBC:
    RG4Blo = lcon(0xa);
    RG1Blo = DM2_GET_FLOOR_DECORATION_OF_ACTUATOR(RG1P);
  }
  else
  {
    RG4Blo = lcon(0x9);
    RG1Blo = DM2_GET_WALL_DECORATION_OF_ACTUATOR(RG1P);
  }
  //m_46DC3:
  if (RG1Blo == lcon(0xffffffff))
    return 1;
  //m_46DCE:
  RG6L = unsignedlong(RG1Blo);
  RG5l = unsignedlong(RG4Blo);
  RG3L = lcon(0xd);
  RG2L = lcon(0xb);
  RG4L = RG6L;
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(CUTLX8(RG5l), RG4Blo, RG2Blo, RG3Blo);
  if (RG1W == 0)
  {
    RG3P = ADRESSOF(t_text, tarr_00);
    RG2L = lcon(0xd);
    RG4L = RG6L;
    RG1T = DM2_QUERY_GDAT_TEXT(CUTLX8(RG5l), RG4Blo, RG2Blo, RG3T);
    if (byte_at(RG1P) == 0)
      return 1;
    RG1L = DM2_STRLEN(RG1T);
    return RG1W != 0 ? RG1L : 1;
  }
  RG1Bhi &= lcon(0x7f);
  return RG1L;
}

i32 DM2_QUERY_GDAT_FOOD_VALUE_FROM_RECORD(i16 eaxw)
{
  c_nreg _RG1; RG1W = eaxw;

  return DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG1W), lcon(0x3));
}

i32 DM2_QUERY_CREATURES_ITEM_MASK(i32 eaxl, i32 edxl, unk* xebxp, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  c_wlreg _RG6;
  t_text* RG7t;
  t_text tarr_00[0x80];
  unk* xp_80;
  i32 vl_84;
  i8 vb_88;
  i8 vb_8c;

  vb_8c = RG1Blo;
  vb_88 = RG4Blo;
  xp_80 = UPCAST(unk, RG2P);
  vl_84 = RG3L;
  DM2_ZERO_MEMORY(RG2P, lcon(0x40));
  RG1Blo = vb_88 + bcon(0x10);
  RG2L = unsignedlong(RG1Blo);
  RG1T = DM2_QUERY_GDAT_TEXT(bcon(0xf), vb_8c, RG2Blo, tarr_00);
  RG7t = RG1T;
  if (*RG7t == '\0')
    return 0;
  RG6L = 0;
  RG4L = lcon(0xffffffff);
  RG5l = RG4L;
  RG1L = 0;
  //m_54AB:
  for (;;)
  {
    RG3Bhi = *RG7t;
    RG2L = unsignedlong(RG3Bhi);
    RG7t++;
    if (RG2L < lcon(0x30) || RG2L > lcon(0x39))
    {
      //m_54D3:
      RG2L = unsignedlong(RG3Bhi);
      if (RG2L != lcon(0x2d))
      {
        bool skip00011 = false;
        //m_54E5:
        if (RG1L != 0)
        {
          if (RG4W < 0)
            RG4L = RG6L;
          //m_550F:
          for (;;)
          {
            if (RG4W > RG6W)
            {
              RG6L = 0;
              RG4L = lcon(0xffffffff);
              RG5l = RG4L;
              RG1L = 0;
              break;
            }
            //m_54F2:
            RG2L = RG4L + RG5l;
            RG1P = DOWNCAST(unk, xp_80) + signedlong(RG2W) / 8;
            RG3Blo = RG2Blo & lcon(0x7);
            RG2Blo = 1 << RG3Blo;
            or8(location(RG1P), RG2Blo);
            RG4L++;
          }
        }
        //m_551F:
        if (mkub(RG3Bhi) < mkub(lcon(0x4a)))
        {
          //m_553B:
          if (mkub(RG3Bhi) >= mkub(lcon(0x41)))
          {
            if (mkub(RG3Bhi) <= mkub(lcon(0x41)))
              //m_554D:
              RG5l = lcon(0x80);
            else
            {
              if (RG3Bhi == lcon(0x43))
              {
                //m_5562:
                if (vl_84 != 0)
                  skip00011 = true;
                else
                  RG5l = lcon(0x1e0);
              }
            }
          }
        }
        else
        {
          if (mkub(RG3Bhi) <= mkub(lcon(0x4a)))
            //m_5554:
            RG5l = lcon(0x100);
          else
          {
            if (mkub(RG3Bhi) < mkub(lcon(0x53)))
            {
              //m_5534:
              if (RG3Bhi == lcon(0x50))
                //m_555B:
                RG5l = lcon(0x180);
            }
            else
            {
              if (mkub(RG3Bhi) <= mkub(lcon(0x53)))
                //m_5573:
                RG5l = lcon(0x1fc);
              else
              {
                if (RG3Bhi == lcon(0x57))
                  skip00011 = true;
              }
            }
          }
        }

        if (skip00011)
          //m_5549:
          RG5l = 0;
      }
      else
      {
        RG4L = RG6L;
        RG6L = 0;
      }
    }
    else
    {
      RG6L *= lcon(0xa);
      RG1L = unsignedlong(RG3Bhi) - lcon(0x30);
      RG6L += RG1L;
      RG1L = 1;
    }
    //m_5578:
    if (RG3Bhi == 0)
      return 1;
  }
  FOREND
}

// recursive!!!
i32 DM2_IS_ITEM_FIT_FOR_EQUIP(i16 eaxw, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;

  RG3L = RG4L;
  RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG1W), lcon(0x4));
  RG4L = RG1L;
  RG5L = RG1L;
  if (RG2L != 0)
  {
    RG1L = signedlong(RG3W);
    if (RG1L >= lcon(0xd))
      return 0;
    if (RG3W >= 0)
      //m_53A4:
      RG1W = table1d2670[RG1L];
    else
      RG1L = lcon(0x400);
    //m_53AC:
    RG2L = unsignedlong(RG1W);
    RG1L = unsignedlong(RG5W);
    return RG1L & RG2L;
  }
  //m_53C0:
  RG1L = signedlong(RG3W);
  if (RG1L < lcon(0xd) && RG1L > 1)
  {
    RG1W = table1d2670[RG1L];
    RG1L = unsignedlong(RG1W);
    RG2W = RG4W;
    return RG1L & RG2L;
  }
  RG1L = signedlong(RG3W);
  if (RG1L < lcon(0x1e) || RG1L >= lcon(0x26))
    return 1;
  RG1L = RG5L;
  RG1W &= lcon(0x8000);
  RG1L = unsignedlong(RG1W);
  if (RG1L != 0)
    return 0;
  RG1L = DM2_IS_ITEM_FIT_FOR_EQUIP(party.handitems.warr2d_00[party.curacthero][party.curactmode], 12, 0);
  if (RG1L == 0)
    return 1;
  RG5L &= lcon(0x40);
  return unsignedlong(RG5W);
}

void DM2_QUERY_MESSAGE_TEXT(t_text* eaxtp, i32 edxl, i32 ebxl)
{
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG1;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p;
  c_wlreg _RG6;
  i8* RG7p;
  t_text* tp_00;
  t_text tarr_04[0x80];
  i16 vw_84;
  i16 vw_88;
  i8 vb_8c;
  i8 vb_90;
  bool skip00140 = false;

  tp_00 = eaxtp;
  RG1L = RG4L;
  RG6L = 0;
  RG5p = ADRESSOF(t_text, tarr_04);
  vb_90 = lcon(0xffffffff);
  RG4L = RG1L;
  RG4W &= lcon(0x3c00);
  RG4L = unsignedlong(RG4W) >> bcon(0xa);
  if (RG4L != lcon(0x7))
    //m_E087:
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
  else
  {
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
    RG4W = word_at(RG1P, 2);
    RG3L = RG4L;
    RG3UW >>= bcon(0xa);
    if (RG3W == 0)
    {
      //m_E072:
      RG4Bhi &= 0x3;
      RG1L = unsignedlong(RG4W) << 2;
      RG4RP = recordptr[2];
      RG1P = RG4P + RG1L;
    }
    else
    {
      vb_90 = RG3Blo;
      skip00140 = true;
    }
  }

  if (!skip00140)
  {
    //m_E091:
    RG4W = word_at(RG1P, 2);
    RG4L <<= bcon(0xd);
    RG4UW >>= 14;
    if (RG4W != 0)
    {
      RG3L = unsignedlong(RG4W);
      if (RG3L != 1)
        jump L_fin;
      RG3W = word_at(RG1P, 2);
      RG3UW >>= bcon(0x3);
      vw_84 = RG3W; // truncated
      RG3L = unsignedlong(vw_84);
      RG3UL >>= bcon(0x8);
      RG3L &= lcon(0x1f);
      if (RG3L != lcon(0xe))
        jump L_fin;
    }
    //m_E0D7:
    if (jz_test8(location(RG1P + 2), 1))
    {
      RG3L = signedlong(RG2W);
      if ((RG3Bhi & bcon(0x80)) == 0)
        jump L_fin;
    }
    //m_E0E9:
    RG4L = unsignedlong(RG4W);
    if (RG4L == 1)
    {
      RG4W = word_at(RG1P, 2);
      RG4UW >>= bcon(0x3);
      vb_90 = RG4Blo;
    }
  }

  //m_E103:
  RG2Bhi &= lcon(0x7f);
  RG4L = signedlong(RG2W);
  if (RG4L == 1)
  {
    mov8(location(RG5p), lcon(0xa));
    RG5p += RG4L;
  }
  vb_8c = lcon(0xa);
  RG4Blo = vb_90;
  if (RG4Blo != lcon(0xffffffff))
  {
    RG2L = unsignedlong(RG4Blo);
    RG3P = RG5p;
    RG4L = 0;
    RG1L = signedlong(CUTX16(DM2_STRLEN(DM2_QUERY_GDAT_TEXT(bcon(0x3), bcon(0x0), RG2Blo, RG3T))));
    RG5p += RG1L;
    jump L_fin;
  }
  //m_E146:
  RG3L = 0;
  vw_88 = RG3W;
  RG1W = word_at(RG1P, 2);
  RG1UW >>= bcon(0x3);
  RG1L = 2 * unsignedlong(RG1W);
  RG7p = ddat.v1e03d0 + RG1L;
  //m_E221:
  for (;;)
  {
    for (;;)
    {
      for (;;)
      {
        if (RG3W != 0)
        {
          //m_E16C:
          RG1L = unsignedlong(RG3W);
          if (RG1L != 1)
          {
            //m_E183:
            RG2L = RG6L;
            RG2W &= lcon(0x1f);
          }
          else
            RG2L = unsignedlong(RG6W) >> 5 & lcon(0x1f);
        }
        else
        {
          RG6W = word_at(RG7p, 0);
          RG1L = unsignedlong(RG6W) >> bcon(0xa) & lcon(0x1f);
          RG2L = RG1L;
          RG7p += 2;
        }
        //m_E18A:
        RG3L++;
        RG4L = unsignedlong(RG3W);
        RG3L = 3;
        RG1L = RG4L;
        RG4L = RG1L % RG3L;
        RG1L /= RG3L;
        RG3L = RG4L;
        if (vw_88 == 0)
          break;
        mov8(location(RG5p), 0);
        RG1L = signedlong(vw_88);
        if (RG1L != lcon(0x1e))
          //m_E1CC:
          RG2P = DOWNCAST(i16, table1d292c + unsignedlong(RG2W)); // adj
        else
        {
          RG1L = unsignedlong(RG2W) << 3;
          RG2P = table1d282c + RG1L;
        }
        //m_E1DA:
        DM2_STRCAT(UPCAST(t_text, RG5p), RG2T);
        RG1L = signedlong(CUTX16(DM2_STRLEN(RG2T)));
        RG5p += RG1L;
        RG4L = 0;
        vw_88 = RG4W;
      }
      //m_E1F9:
      RG1L = unsignedlong(RG2W);
      if (RG1L >= lcon(0x1c))
      {
        //m_E243:
        if (RG1L != lcon(0x1c))
        {
          //m_E250:
          if (RG1L > lcon(0x1e))
            jump L_fin;
          vw_88 = RG2W;
          continue;
        }
        RG1Blo = vb_8c;
        mov8(location(RG5p), RG1Blo);
      }
      else
      {
        if (RG1L != lcon(0x1a))
        {
          //m_E20F:
          if (RG1L != lcon(0x1b))
            //m_E21B:
            RG2L += lcon(0x41);
          else
            RG2L = lcon(0x2e);
        }
        else
          RG2L = lcon(0x20);
        //m_E21E:
        mov8(location(RG5p), RG2Blo);
      }
      break;
    }
    //m_E220:
    RG5p++;
  }
  FOREND

L_fin:
  mov8(location(RG5p), 0);
  DM2_FORMAT_SKSTR(tarr_04, tp_00);
}

unk* DM2_GET_MISSILE_REF_OF_MINION(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  RG2L = RG4L;
  if (RG1W == wcon(0xffff) || RG1W == wcon(0xfffe))
    return NULL;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
  if (word_at(RG1P) == lcon(0xffffffff))
    return NULL;
  RG4W = word_at(RG1P, 2);
  //m_FC37:
  for (;;)
  {
    if (RG4W == lcon(0xfffffffe))
      return NULL;
    RG1L = RG4L;
    RG1Blo ^= RG4Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    if (RG1L == lcon(0xe))
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
      RG3P = RG1P;
      if (RG2W == lcon(0xffffffff) || RG2W == word_at(RG1P, 2))
        return RG3P;
    }
    //m_FC6E:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
    RG4L = RG1L;
  }
  FOREND
}

i32 DM2_GET_CREATURE_AT(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4; RG4L = edxl;

  RG4L = signedlong(RG4W);
  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(RG1W, RG4W));
  //m_1FC63:
  for (;;)
  {
    if (RG1W == lcon(0xfffffffe))
      return lcon(0xffff);
    RG4L = RG1L;
    RG4Blo ^= RG1Blo;
    RG4Bhi &= lcon(0x3c);
    RG4L = unsignedlong(RG4W) >> bcon(0xa);
    if (RG4L == lcon(0x4))
      return RG1L;
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG1UW)));
  }
  FOREND
}

i32 DM2_FIND_HAND_WITH_EMPTY_FLASK(c_hero* hero)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG4L = 2;
  //m_32349:
  for (;;)
  {
    RG4L--;
    if (RG4W < 0)
      return RG4L;
    //m_3231B:
    RG1L = signedlong(RG4W);
    RG1W = hero->item[RG1L];
    RG2L = RG1L;
    RG2Blo ^= RG1Blo;
    RG2Bhi &= lcon(0x3c);
    RG2L = unsignedlong(RG2W) >> 10;
    if (RG2L == lcon(0x8))
    {
      RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(unsignedlong(RG1W));
      if (RG1Blo == lcon(0x14))
        return RG4L;
    }
  }
  FOREND
}

i32 DM2_FIND_DISTINCTIVE_ITEM_ON_TILE(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  i16 RG6w;

  RG5w = RG2W;
  RG6w = RG3W;
  RG4L = signedlong(RG4W);
  RG1L = DM2_GET_WALL_TILE_ANYITEM_RECORD(signedlong(RG1W), RG4W);
  //m_3BDD7:
  for (;;)
  {
    RG4L = RG1L;
    if (RG4W == lcon(0xfffffffe))
      return lcon(0xffff);
    RG2L = unsignedlong(RG4W);
    RG1L = signedlong(DM2_GET_DISTINCTIVE_ITEMTYPE(RG2W));
    RG3L = unsignedlong(RG6w);
    if (RG1L == RG3L)
    {
      RG1L = signedlong(RG5w);
      if (RG1L == lcon(0xffffffff))
        return RG4L;
      RG2L >>= 14;
      if (RG2L == RG1L)
        return RG4L;
    }
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
  }
  FOREND
}

i16 DM2_FIND_TILE_ACTUATOR(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  i32 RG6w;
  bool flag;

  RG6w = RG3W;
  RG4L = signedlong(RG4W);
  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(RG1W, RG4W));
  RG4L = RG1L;
  RG5l = 0;
  //m_18AE6:
  for (;;)
  {
    bool skip00332 = false;
    bool skip00333 = false;
    if (RG4W == lcon(0xfffffffe))
      return RG5l == 0 ? -1 : RG4W;
    RG1L = RG4L;
    RG1Blo ^= RG4Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    RG1L = signedlong(RG1W);
    if (RG1L > lcon(0x3))
      return RG5l == 0 ? -1 : RG4W;
    if (RG1L != lcon(0x3))
      skip00332 = true;
    else
    {
      RG1L = unsignedlong(RG2Blo);
      if (RG1L != lcon(0xff))
        skip00332 = true;
    }

    if (skip00332)
    {
      //m_18B0E:
      RG3L = unsignedlong(RG4W) >> bcon(0xe);
      RG1L = unsignedlong(RG2Blo);
      if (RG3L == RG1L)
        skip00333 = true;
    }
    else
      skip00333 = true;

    if (skip00333)
    {
      //m_18B1E:
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
      RG1W = word_at(RG1P, 2) & lcon(0x7f);
      RG1Blo = RG1W == RG6w ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      flag = RG1L != 0;
      RG5l = RG1L;
      if (flag)
        return RG5l == 0 ? -1 : RG4W;
    }

    //m_18B3F:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
    RG4L = RG1L;
  }
  FOREND
}

i32 DM2_FIND_LADDAR_AROUND(i32 eaxl, i32 edxl, i32 ebxl, unk** xecxpp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3P = DOWNCAST(unk*, xecxpp);
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  i16 RG6w;
  i8* RG7p;
  i32 vl_00;
  i32 vl_04;
  c_o32 vo_08; // poke 16 peek 32 inc16

  vl_04 = RG1L;
  vl_00 = RG4L;
  RG6w = RG2W;
  RG7p = RG3P;
  RG4L = 0;
  vo_08.poke16(RG4W);
  //m_D102:
  for (;;)
  {
    RG1L = signedlong(vo_08.peek16());
    if (RG1L > lcon(0x3))
    {
      mov32(location(RG7p + 0), 0);
      return lcon(0xffffffff);
    }
    //m_CFAA:
    RG4L = vl_00;
    RG4W += table1d2804[RG1L];
    RG4L = signedlong(RG4W);
    RG2L = vl_04;
    RG3W = table1d27fc[RG1L];
    RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(CUTX16(RG2L + RG3L), RG4W));
    //m_CFCE:
    for (;;)
    {
      bool skip00124 = false;
      RG5L = RG1L;
      RG1Blo = RG5W != wcon(0xfffe) ? 1 : 0;
      RG1L = unsignedlong(RG1Blo);
      if (RG1L == 0)
        break;
      RG1L = RG5L;
      RG1W &= lcon(0x3c00);
      RG1L = unsignedlong(RG1W) >> bcon(0xa);
      RG4L = RG1L;
      RG1L = unsignedlong(RG4W);
      if (RG1L > lcon(0x3))
        break;
      if (RG1L == lcon(0x3))
        skip00124 = true;
      else
      {
        if (RG1L == 2)
          skip00124 = true;
      }

      if (skip00124)
      {
        //m_D00C:
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG5UW));
        RG4L = unsignedlong(RG4W);
        if (RG4L != lcon(0x3))
        {
          //m_D085:
          RG4W = word_at(RG1P, 2) & lcon(0x6);
          if (RG4W == 2)
          {
            RG4W = word_at(RG1P, 2);
            RG4UW >>= bcon(0x3);
            RG1L = unsignedlong(RG4W);
            RG1UL >>= bcon(0x8);
            RG1L &= lcon(0x1f);
            if (mkul(RG1L) >= mkul(lcon(0x6)) && mkul(RG1L) <= mkul(lcon(0x7)))
            {
              bool skip00125 = false;
              RG4W = unsignedword(RG4Blo);
              RG1L = RG4L;
              RG4L = unsignedlong(RG1Blo);
              RG3L = lcon(0x11);
              RG2L = lcon(0xb);
              RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, RG4Blo, RG2Blo, RG3Blo);
              RG4W = RG1W;
              if (RG6w == wcon(0xffff) && RG1W != 0)
                skip00125 = true;
              else
              {
                if (RG6w == wcon(0x1) && RG4W == wcon(0x0))
                  skip00125 = true;
              }

              if (skip00125)
              {
                //m_D0E5:
                mov32(location(RG7p + 0), 0);
                return unsignedlong(vo_08.peek16());
              }
            }
          }
        }
        else
        {
          RG4W = word_at(RG1P, 2) & lcon(0x7f);
          if (RG4W == lcon(0x1c))
          {
            RG4W = word_at(RG1P, 2);
            RG4UW >>= bcon(0x7);
            RG4L = unsignedlong(RG4W) << 4;
            RG4W = word_at(ddat.v1e03c8, RG4L + lcon(0x8)) & lcon(0x3f);
            RG2L = signedlong(RG4W);
            RG4P = DOWNCAST(s_sizee, ddat.v1e03c0);
            RG4W = word_at(RG4P, lcon(0x8)) & lcon(0x3f);
            RG4L = signedlong(RG4W);
            RG2L -= RG4L;
            RG4L = RG2L;
            RG2L = signedlong(RG6w);
            if (RG2L == RG4L)
            {
              movp(location(RG7p + 0), RG1P);
              return unsignedlong(vo_08.peek16());
            }
          }
        }
      }

      //m_D0EE:
      RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG5UW)));
    }
    //m_D0FD:
    vo_08.inc16();
  }
  FOREND
}

c_image* DM2_QUERY_GDAT_SUMMARY_IMAGE(c_image* image, i8 edxb, i8 ebxb, i8 ecxb)
{
  c_nreg _RG1;
  c_nreg _RG3;

  image->init();
  image->imgdesc.bmpid = (t_dbidx)-1;
  image->imgdesc.b_08 = edxb;
  image->imgdesc.b_09 = ebxb;
  image->imgdesc.b_0a = 1;
  image->imgdesc.b_0b = ecxb;
  image->w_34 = image->w_36 = 0x40;
  image->query1 = 0xffff;
  image->w_1a = -1;
  image->bmp = UPCAST(t_bmp, gfxsys.dm2screen); // TODO ...there is no bmp-header!
  image->alphamask = NOALPHA;
  if (edxb == 0xff)
    return image;

  image->imgdesc.w_06 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(edxb, ebxb, 1, ecxb);
  RG3W = RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(edxb, ebxb, 12, lcon(0xfe));
  if (RG1W != 0)
  {
    RG1W >>= 8;
    image->w_1c += signedword(RG1Blo); //!
    image->w_1e += signedword(RG3Blo); //!
  }
  RG3W = RG1W = DM2_QUERY_GDAT_PICT_OFFSET(edxb, ebxb, ecxb);
  if (RG1W != 0)
  {
    RG1W >>= 8;
    image->w_1c += signedword(RG1Blo); //!
    image->w_1e += signedword(RG3Blo); //!
  }

  t_palette* pal = DM2_QUERY_GDAT_IMAGE_LOCALPAL(edxb, ebxb, ecxb);
  if (pal == NULL)
  {
    image->colors = -1;
    return image;
  }
  image->colors = PAL16;
  // now copies the first 16 colors only
  DM2_COPY_SMALL_PALETTE(image->palette, pal);
  return image;
}

i16 DM2_GET_PLAYER_AT_POSITION(i16 eaxw)
{
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;

  RG5w = eaxw;
  c_hero* hero = &party.hero[0];
  RG4L = 0;
  //m_359D6:
  for (;;)
  {
    if (RG4UW >= mkuw(party.heros_in_party))
      return -1;
    //m_359B4:
    RG3L = unsignedlong(hero->partypos);
    RG2L = unsignedlong(RG5w);
    if (RG3L == RG2L)
    {
      if (hero->curHP != 0)
        return RG4W;
    }
    RG4L++;
    hero++; // struct advance
  }
  FOREND
}

// was SKW_2759_04c0
i32 DM2_FIND_POUCH_OR_SCABBARD_POSSESSION_POS(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;

  RG5w = RG1W;
  RG1L = signedlong(RG4W);
  if (RG1L == 1)
  {
    RG4L = signedlong(RG5w);
    if (party.hero[RG4L].item[12] != -1)
      return lcon(0xc);
    //m_2FD4D:
    RG2L = lcon(0x7);
    //m_2FD79:
    for (;;)
    {
      RG3L = signedlong(RG2W);
      if (RG3L > lcon(0x9))
        return lcon(0xffffffff);
      //m_2FD54:
      if (party.hero[RG5w].item[RG3L] != -1)
        return RG2L;
      //m_2FD78:
      RG2L++;
    }
    FOREND
  }
  //m_2FD83:
  if (RG4W != 0)
    return lcon(0xffffffff);
  RG4W = party.hero[RG5w].item[11];
  if (RG4W != lcon(0xffffffff))
    return 11;
  if (RG4W != party.hero[RG5w].item[6])
    return 6;
  return lcon(0xffffffff);
}

i32 DM2_DIR_FROM_5x5_POS(i32 eaxl)
{
  c_nreg _RG1;

  RG1L = eaxl & lcon(0xffff);
  if (RG1L == lcon(0x6))
    return 0;
  if (RG1L == lcon(0x8))
    return 1;
  if (RG1L == lcon(0x12))
    return 2;
  if (RG1L == lcon(0x10))
    return lcon(0x3);
  return RG1L != lcon(0xc) ? lcon(0xffffffff) : lcon(0x4);
}

// was SKW_0cee_328c
i32 DM2_GET_DOOR_STAT_0X10(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4;

  RG4L = unsignedlong(RG1Blo);
  return mkl(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(14, RG4Blo, 11, 14));
}

i32 DM2_GET_GLOB_VAR(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG3;
  c_nreg _RG4;

  if (RG1UW <= mkuw(lcon(0x3f)))
  {
    RG3L = unsignedlong(RG1W) / 8;
    RG4L = unsignedlong(ddat.v1e0104[RG3L]);
    RG3L = RG1L;
    RG3Blo &= lcon(0x7);
    RG1L = 1 << RG3Blo;
    return (RG1L & RG4L) == 0 ? 0 : 1;
  }
  if (RG1UW <= mkuw(lcon(0x7f)))
  {
    RG1L = unsignedlong(RG1W);
    RG1Blo = ddat.globalb[RG1L - 0x40]; // evil - index splitted
    RG1W = unsignedword(RG1Blo);
    return RG1L;
  }
  if (RG1UW > mkuw(lcon(0xbf)))
    return 0;
  RG1L = unsignedlong(RG1W);
  RG1W = ddat.v1e000c[RG1L];
  return RG1L;
}

i32 DM2_GET_GRAPHICS_FOR_DOOR(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4;

  RG4L = unsignedlong(RG1Blo);
  return mkul(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(14, RG4Blo, 11, 13));
}

i32 DM2_GET_ITEM_ORDER_IN_CONTAINER(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;
  i16 RG6w;
  i8* RG7p;
  i32 vl_00;
  t_text tarr_04[0x80];
  i16 vw_84;
  c_ql32 vql_88;
  i32 vl_8c; // inc32

  vl_00 = RG4L;
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(unsignedlong(RG1W));
  RG4L = unsignedlong(RG1Blo);
  RG3P = ADRESSOF(t_text, tarr_04);
  RG2L = lcon(0x40);
  RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x14), RG4Blo, bcon(0x40), RG3T);
  RG7p = RG1P;
  if (byte_at(RG1P) == 0)
    return lcon(0xffffffff);
  RG4L = 0;
  vw_84 = RG4W; // truncated
  vql_88.poke32(RG4L);
  vl_8c = RG4L;
  //m_56EF:
  for (;;)
  {
    RG5w = wcon(0xffff);
    RG6w = RG5w;
    //m_56F6:
    for (;;)
    {
      for (;;)
      {
        if (vw_84 != 0)
          return lcon(0xffffffff);
        RG1Blo = byte_at(RG7p, 0);
        RG4L = unsignedlong(RG1Blo);
        RG7p++;
        if (RG4L < lcon(0x30) || RG4L > lcon(0x39))
          break;
        RG4L = vql_88.peek32() * lcon(0xa);
        RG1W = unsignedword(RG1Blo);
        RG1L -= lcon(0x30);
        RG4L += RG1L;
        vql_88.poke32(RG4L);
      }
      //m_572F:
      if (mkub(RG1Blo) < mkub(lcon(0x2d)))
      {
        //m_573B:
        if (RG1Blo == 0)
          //m_5768:
          vw_84 = 1;
      }
      else
      {
        if (mkub(RG1Blo) <= mkub(lcon(0x2d)))
        {
          //m_5756:
          RG5w = vql_88.peek16();
          RG2L = 0;
          vql_88.poke32(RG2L);
          continue;
        }
        if (RG1Blo == lcon(0x4a))
        {
          //m_5741:
          if (vql_88.peek16() == 0)
          {
            //m_574F:
            RG6w = wcon(0x100);
            continue;
          }
          RG7p--;
        }
      }
      break;
    }
    //m_5773:
    if (RG5w < 0)
      RG5w = vql_88.peek16();
    //m_57BF:
    for (;;)
    {
      if (RG5w > vql_88.peek16())
      {
        vql_88.poke32(0);
        break;
      }
      //m_5781:
      RG1L = vl_8c;
      RG4L = vl_00;
      if (RG1W == RG4W)
      {
        RG1L ^= RG4L;
        //m_57AE:
        for (;;)
        {
          if (RG1W >= ddat.v1e03fe)
            break;
          //m_5794:
          RG4L = signedlong(RG1W);
          RG3L = signedlong(ddat.v1e0394[RG4L]);
          RG2L = signedlong(RG5w);
          RG4L = signedlong(RG6w) + RG2L;
          if (RG3L == RG4L)
            return RG1L;
          RG1L++;
        }
      }
      //m_57B7:
      vl_8c++;
      RG5w++;
    }
  }
  FOREND
}

// edxp is a wordarray - pointer, 0xa words
void DM2_COUNT_BY_COIN_TYPES(i32 eaxl, i16* edxpwarr)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4P = DOWNCAST(i16, edxpwarr);
  i16 RG5w;
  i16 RG6w;
  i8* RG7p;

  RG2L = RG1L;
  RG7p = RG4P;
  RG4L = lcon(0x14);
  RG1P = RG7p;
  DM2_ZERO_MEMORY(RG1P, RG4L);
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
  RG5w = word_at(RG1P, 2);
  //m_F454:
  while (RG5w != wcon(0xfffe))
  {
    RG4L = unsignedlong(RG5w);
    RG1L = DM2_IS_MISCITEM_CURRENCY(RG4L);
    if (RG1L != 0)
    {
      RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG4W);
      RG6w = RG1W;
      RG2L = 0;
      //m_F4AD:
      for (;;)
      {
        if (RG2W >= ddat.v1e03fe)
          break;
        //m_F47B:
        RG4L = 2 * signedlong(RG2W);
        if (RG6w == ddat.v1e0394[RG4L / 2])
        {
          RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(RG5w)));
          RG1W = word_at(RG1P, 2);
          RG1L *= 4;
          RG1UW >>= bcon(0xa);
          RG3L = signedlong(RG1W);
          RG1L = signedlong(word_at(RG7p, RG4L));
          RG3L++;
          RG1L += RG3L;
          mov16(location(RG7p + RG4L), RG1W);
        }
        RG2L++;
      }
    }
    //m_F4B6:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(RG5w))));
    RG5w = RG1W;
  }
}

// was SKW_3df7_075f
i32 DM2_RETRIEVE_ENVIRONMENT_CMD_CD_FW(unk* xeaxp)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG5p;
  t_text tarr_00[0x80];

  RG5p = RG1P;
  RG2L = unsignedlong(byte_at(RG1P));
  RG4L = unsignedlong(ddat.v1d6c02);
  RG3P = ADRESSOF(t_text, tarr_00);
  RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x17), RG4Blo, RG2Blo, RG3T);
  if (byte_at(RG1P) == 0)
    return 0;
  RG4T = v1d1140;
  RG1L = DM2_QUERY_CMDSTR_TEXT(tarr_00, RG4T);
  mov16(location(RG5p + 2), RG1W);
  mov16(location(RG5p + lcon(0x6)), 0);
  RG1W = word_at(RG5p, lcon(0x6));
  mov16(location(RG5p + lcon(0x4)), RG1W);
  RG4T = v1d1143;
  RG1L = DM2_QUERY_CMDSTR_TEXT(tarr_00, RG4T);
  mov8(location(RG5p + 1), RG1Blo);
  mov8(location(RG5p + lcon(0x9)), lcon(0x40));
  RG1Blo = byte_at(RG5p, lcon(0x9));
  mov8(location(RG5p + lcon(0x8)), RG1Blo);
  return 1;
}

i32 DM2_CALC_PLAYER_WALK_DELAY(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG51l;
  i32 RG52l;
  i32 RG53l;
  i32 RG54l;
  i32 RG6l;

  if (ddat.savegames1.b_04 != 0)
    return 1;
  RG51l = unsignedlong(RG1W);
  c_hero* hero = &party.hero[RG51l];
  RG1W = hero->get_max_load();
  RG3L = RG1L;
  RG1W = party.get_player_weight((e_hero)CUTX16(RG51l));
  RG4L = unsignedlong(RG1W);
  if (RG3UW <= RG1UW)
  {
    //m_369B6:
    RG1L = unsignedlong(RG4W);
    RG52l = unsignedlong(RG3W);
    RG4L = 4 * (RG1L - RG52l);
    RG1L = RG4L;
    RG4L = RG1L % RG52l;
    RG1L /= RG52l;
    RG1L += 4;
    RG4L = 2;
  }
  else
  {
    RG1L = 2;
    RG6l = 8 * (RG4L & lcon(0xffff));
    RG54l = unsignedlong(RG3W);
    RG4L = 5 * RG54l;
    if (RG6l > RG4L)
      RG1L = 3;
    RG4L = 1;
  }
  //m_369D6:
  RG3W = hero->bodyflag & 0x20;
  RG53l = unsignedlong(RG3W);
  if (RG53l != 0)
    RG1L += RG4L;
  RG4W = signedword(hero->walkspeed);
  RG1L -= RG4L;
  RG4L = signedlong(RG1W);
  RG1L = signedlong(DM2_MAX(1, RG4W));
  RG4L = RG1L;
  RG2L = unsignedlong(RG1W);
  if (RG2L > 2)
  {
    RG1L++;
    RG1L &= lcon(0xfffffffe);
  }
  return RG1L;
}

// was SKW_2c1d_11bf
i16 DM2_COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(i16 heroidx, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1W = heroidx;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5; // W and UW only
  c_lreg _RG72;
  c_lreg _RG73;
  c_ql32 vql_00;
  c_ql32 vql_04;
  i16 vw_08;

  vql_04.poke32(RG1L);
  vql_00.poke32(RG4L);
  RG5W = RG2W;
  c_hero* hero = &party.hero[vql_04.peek16()];
  RG1L = DM2_RAND();
  RG3L = RG1L;
  RG3Bhi ^= RG1Bhi;
  RG3Blo &= lcon(0xf);
  RG2L = 0;
  RG4L = 1;
  RG1W = hero->get_adj_ability1((e_ability)RG4W, (e_curmax)RG2W);
  RG3W += RG1W;
  RG1L = signedlong(vql_00.peek16());
  RG1W = hero->item[RG1L];
  vw_08 = RG1W;
  RG4W = DM2_QUERY_ITEM_WEIGHT(vw_08);
  RG1W = hero->get_max_load();
  RG2L = unsignedlong(RG1W) >> 4;
  RG1L = RG4L - lcon(0xc);
  RG3L += RG1L;
  if (RG4UW > RG2UW)
  {
    RG1L = unsignedlong(RG4W);
    RG72L = unsignedlong(RG2W);
    RG1L -= RG72L;
    RG1L >>= bcon(0x1);
    RG3L = signedlong(RG3W) - RG1L;
    RG2L = (RG72L - lcon(0xc)) / 2 + RG72L;
    if (RG4UW > RG2UW)
    {
      RG4L -= RG2L;
      RG4L = 2 * RG4L;
      RG3L -= RG4L;
    }
  }
  if (RG5W >= 0)
  {
    bool skip00776 = false;
    bool skip00775 = false;
    RG4L = unsignedlong(RG5W);
    RG1W = 2 * DM2_QUERY_PLAYER_SKILL_LV(vql_04.peek16(), RG4W, 1);
    RG3W += RG1W;
    RG2L = 0;
    if (RG5UW < mkuw(lcon(0x4)))
    {
      //m_36BB1:
      if (RG5W == 0)
        skip00775 = true;
      else
      {
        if (RG5W == 1)
          skip00776 = true;
      }
    }
    else
    {
      if (RG5UW <= mkuw(lcon(0x7)))
        skip00775 = true;
      else
      {
        if (RG5UW >= mkuw(lcon(0x9)))
        {
          if (RG5UW <= mkuw(lcon(0x9)))
            skip00775 = true;
          else
          {
            if (RG5UW <= mkuw(lcon(0xb)))
              skip00776 = true;
          }
        }
      }
    }

    if (skip00775)
      //m_36BBE:
      RG2L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(vw_08), lcon(0x8));

    if (skip00776)
    {
      //m_36BD3:
      RG73L = unsignedlong(vw_08);
      RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG73L, lcon(0x9));
      RG2L = RG1L;
      if (RG1W != 0)
      {
        bool skip00777 = false;
        RG4L = 5;
        RG1L = DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(RG73L, RG4L);
        RG1W &= lcon(0x8000);
        RG1L = unsignedlong(RG1W);
        if (RG1L == 0)
        {
          //m_36C0D:
          RG1L = signedlong(RG5W);
          if (RG1L == lcon(0xb))
            skip00777 = true;
        }
        else
        {
          RG1L = signedlong(RG5W);
          if (RG1L != lcon(0xb))
            skip00777 = true;
        }

        if (skip00777)
          //m_36C15:
          RG2L = 0;
      }
    }

    //m_36C17:
    RG3L += RG2L;
  }

  //m_36C19:
  RG4L = signedlong(RG3W);
  RG1W = hero->get_stamina_adj(RG4W);
  RG3L = RG1L;
  RG1Blo = vql_00.peek16() != wcon(0x0) ? 1 : 0;
  RG1W = unsignedword(RG1Blo);
  RG1L++;
  RG4L = unsignedlong(RG1W);
  RG1L = unsignedlong(hero->bodyflag);
  if ((RG4L & RG1L) != 0)
    RG3W >>= bcon(0x1);
  RG1L = signedlong(RG3W) / 2;
  RG4L = signedlong(RG1W);
  return DM2_BETWEEN_VALUE(0, 100, RG4W);
}

// stacksize was 0x14
void DM2_QUERY_TEMP_PICST(i16 eaxw, i16 edxw, i16 ebxw, i16 ecxw, i16 argw0, i16 argw1, i16 argw2, i16 argw3, i16 alphamask, i16 argw5, i8 argb6, i8 argb7, i8 argb8)
{
  c_nreg _RG1;

  i16 vw_00 = eaxw;
  i16 RG5w = edxw;
  i16 RG6w = ebxw;
  i16 vw_04 = ecxw;
  DM2_QUERY_GDAT_SUMMARY_IMAGE(&dm2_image2, argb6, argb7, argb8);
  if (RG5w != RG6w)
  {
    bool skip00878 = false;
    if (argw1 != 2)
    {
      //m_3E5F2:
      if (argw1 == 3)
      {
        RG1Blo = lcon(0x15);
        skip00878 = true;
      }
    }
    else
    {
      RG1Blo = lcon(0x14);
      skip00878 = true;
    }

    if (skip00878)
    {
      //m_3E5F9:
      RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(argb6, argb7, 11, RG1Blo);
      if (RG1W != 0)
        RG5w = CUTX16((((((unsignedlong(RG1W) >> 8) & lcon(0xffff)) << 7) / mkul(RG1Blo)) + 1) >> 1);
    }
  }
  //m_3E63C:
  dm2_image2.blitmode = vw_00;
  dm2_image2.w_34 = RG5w;
  dm2_image2.w_36 = RG6w;
  dm2_image2.w_1c += vw_04;
  dm2_image2.w_1e += argw0;
  dm2_image2.query1 = argw2;
  dm2_image2.w_1a = argw3;
  dm2_image2.bmp = gfxsys.bitmapptr;
  dm2_image2.alphamask = alphamask;
  DM2_query_32cb_0804(dm2_image2.palette, signedlong(argw1), signedlong(alphamask), signedlong(argw5), &dm2_image2.colors);
  DM2_QUERY_PICST_IT(&dm2_image2);
}

// stacksize was 0x1c
void DM2_query_32cb_0804(t_palette* palette, i32 edxl, i32 ebxl, i32 ecxl, i16* argpw0)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_nreg _RG6;
  c_o32 vo_00; // poke 16 peek 8 16 32 add16
  i16 vw_04;
  i16 vw_08;
  i16 vw_0c;
  i8 parb00;
  i16 parw01;
  i16 parw02;

  vo_00.poke16(RG4W);
  vw_08 = RG2W;
  vw_0c = RG3W;
  if (ddat.v1e025c == 0)
    //m_3E4F4:
    vw_04 = signedword(table1d6c10[signedlong(vo_00.peek16())]);
  else
  {
    if (vo_00.peek16() >= 0)
    {
      //m_3E4D9:
      vw_04 = signedword(table1d6c0b[signedlong(vo_00.peek16())]);
      vo_00.add16(lcon(0x9));
    }
    else
    {
      RG1L = -signedlong(vo_00.peek16()); // ...at this point - so RG1L > 0
      RG4L = signedlong(table1d6c06[RG1L - 1]); // EVIL offset corrected, was 0x1d6c05
      RG1W = DM2_MAX(-ddat.v1e12d2, RG4W);
      vw_04 = RG1W;
      vo_00.poke16(1);
    }
  }
  //m_3E508:
  if (DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(8, ddat.v1d6c02, 7, vo_00.peek8()))
    DM2_query_0b36_037e(palette, 8, ddat.v1d6c02, RG6Blo, CUTX8(ddat.v1e12d2), vw_08, vw_0c, argpw0);
  else
    DM2_query_B073(palette, 64 - CUTX8(((64 - vw_04) * (64 - ddat.v1e12d2)) >> 6), vw_08, vw_0c, argpw0);
}

// stacksize was 0x28
void DM2_query_0b36_037e(t_palette* palette, i8 edxb, i8 ebxb, i8 ecxb, i8 argb0, i16 argw1, i16 argw2, i16* argpw3)
{
  t_dbidx vw_08 = NODATA;
  if (*argpw3 <= 0 && argb0 != 0)
  {
    if (dm2_dballochandler.DM2_dballoc_3e74_55f9(mkul(signedlong(DM2_MAX(0, 64 - unsignedword(argb0))) | (mkl(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(edxb, ebxb, 7, ecxb)) << 6) | lcon(0x60000000)), OO vw_08))
    {
      *argpw3 = 0x100;
      DM2_COPY_LARGE_PALETTE(palette, CHGCAST(t_palette, dm2_dballochandler.DM2_GET_BMP(vw_08)));
      return;
    }
  }
  //m_B415:
  DM2_query_B073(DM2_CONVERT_PALETTE256(palette, edxb, ebxb, ecxb, IO *argpw3), argb0, argw1, argw2, argpw3);
  if (vw_08 >= 0)
  {
    DM2_COPY_LARGE_PALETTE(CHGCAST(t_palette, dm2_dballochandler.DM2_ALLOC_CPXHEAP_MEM(vw_08, PAL256)), palette);
    dm2_dballochandler.DM2_ALLOCATION1(vw_08, false);
  }
}

// was DM2_query_B012, TODO tell SPX new name
t_palette* DM2_CONVERT_PALETTE256(t_palette* palette, i8 edxb, i8 ebxb, i8 ecxb, IO i16& colors)
{
  t_colconv* convtable = QUERY_colconv(DM2_QUERY_GDAT_ENTRY_DATA_PTR(edxb, ebxb, 7, ecxb));
  return DM2_xlat_palette(palette, convtable, colors);
}

// stacksize was 0x3c
t_palette* DM2_query_B073(t_palette* palette, i8 edxb, i16 ebxw, i16 ecxw, i16* argpw0)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG52p;
  c_wlreg _RG6;
  i32 RG7l;
  i32 vl_04;
  c_o32 vo_0c; // poke 16 peek 32 inc16
  i16 vw_10;
  i16 vw_1c;
  i16 vw_20;

  i16 vw_18 = ebxw;
  i16 vw_14 = ecxw;
  if (palette == NULL || edxb == 0)
    return palette;
  i8 vb_28 = CUTX8(DM2_MAX(0, 64 - unsignedword(edxb)));
  i16 vw_24 = word_at(DOWNCAST(i16, argpw0));
  bool flag = vw_24 > 0;
  i32 vl_08 = vw_24 <= 0 ? 1 : 0;
  if (!flag)
  {
    vw_24 = 0x100;
    mov16(location(DOWNCAST(i16, argpw0)), 0x100);
    RG1L = unsignedlong(CUTX8(vw_14)) << 6;
    RG4L = unsignedlong(CUTX8(vw_18)) << bcon(0xe) | ulcon(0x60000000) | mkul(RG1L);
    RG1L = dm2_dballochandler.DM2_dballoc_3e74_55f9(unsignedlong(vb_28) | mkul(RG4L), OO /*TAG dbidx*/ vw_10) ? 1 : 0;
    if (RG1L != 0)
    {
      DM2_COPY_LARGE_PALETTE(palette, CHGCAST(t_palette, dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ vw_10)));
      return palette;
    }
  }
  //m_B128:
  vo_0c.poke16(wcon(0x0));
  //m_B2F0:
  for (;;)
  {
    RG1W = vo_0c.peek16();
    if (RG1W >= vw_24)
    {
      if (vw_18 >= wcon(0x0))
      {
        RG1L = signedlong(vw_18);
        RG4Blo = CUTX8(vw_18);
        ui8_to_palettecolor(&palette[RG1L], RG4UBlo);
      }
      if (vw_14 >= wcon(0x0))
      {
        RG1L = signedlong(vw_14);
        RG4Blo = CUTX8(vw_14);
        ui8_to_palettecolor(&palette[RG1L], RG4UBlo);
      }
      if (vl_08 != 0)
      {
        DM2_COPY_LARGE_PALETTE(CHGCAST(t_palette, dm2_dballochandler.DM2_ALLOC_CPXHEAP_MEM((t_dbidx)vw_10, PAL256)), palette);
        dm2_dballochandler.DM2_ALLOCATION1((t_dbidx)vw_10, false);
      }
      return palette;
    }
    //m_B134:
    if (vl_08 == 0)
    {
      //m_B17F:
      RG1P = DOWNCAST(i16, ddat.v1e0210 + unsignedlong(palettecolor_to_ui8(palette[vo_0c.peek16()])));
      RG4L = unsignedlong(byte_at(RG1P));
      vw_20 = RG4W;
      RG2L = unsignedlong(RG4Blo);
      RG52p = pointer_at(ddat.v1e020c + 9 * RG2L + 1);
      RG1L = unsignedlong(byte_at(RG52p, unsignedlong(byte_at(RG1P, 1))));
      RG4L = unsignedlong(vb_28);
    }
    else
    {
      RG1P = DOWNCAST(i16, ddat.v1e0210 + signedlong(vo_0c.peek16())); // adjusted
      RG4L = unsignedlong(byte_at(RG1P));
      vw_20 = RG4W;
      RG2L = unsignedlong(RG4Blo);
      RG4L = 9 * RG2L;
      RG52p = pointer_at(ddat.v1e020c + RG4L + 1);
      RG4L = unsignedlong(byte_at(RG52p, unsignedlong(byte_at(RG1P, 1))));
      RG1L = unsignedlong(vb_28);
    }
    //m_B1CE:
    RG1L *= RG4L;
    RG1L >>= bcon(0x6);
    RG6W = DM2_MAX(RG1W, 0);
    RG1L = 0;
    RG2L = signedlong(vw_20);
    RG4L = 9 * RG2L;
    RG4Blo = byte_at(ddat.v1e020c, RG4L);
    RG4W = unsignedword(RG4Blo);
    RG4L--;
    vw_1c = RG4W; // truncated
    //m_B1FE:
    for (;;)
    {
      if (RG1W >= vw_1c)
        break;
      RG4P = RG52p + signedlong(RG1W);
      RG2L = unsignedlong(byte_at(RG4P));
      RG3W = RG6W - RG2W;
      RG2W = RG3W;
      if (RG3W >= 0)
      {
        RG4W = unsignedword(byte_at(RG4P, 1)) - RG6W;
        if (RG4W >= 0)
        {
          if (RG3W > RG4W)
            RG1L++;
          break;
        }
      }
      //m_B22D:
      RG1L++;
    }
    //m_B230:
    RG4Blo = byte_at(pointer_at(ddat.v1e020c + 9 * vw_20 + lcon(0x5)), signedlong(RG1W));
    RG2L = unsignedlong(RG4Blo);
    if (RG2W == vw_18 || RG2W == vw_14)
    {
      RG3L = RG1L - 1;
      RG2L = RG1L + 1;
      //m_B26B:
      for (;;)
      {
        if (RG3W < 0)
        {
          //m_B2A1:
          RG1L = RG2L++;
        }
        else
        {
          bool skip00096 = false;
          if (RG2W > vw_1c)
            //m_B29C:
            RG1L = RG3L--;
          else
          {
            RG7l = signedlong(RG6W) - unsignedlong(byte_at(RG52p, signedlong(RG3W)));
            RG4L = unsignedlong(byte_at(RG52p, signedlong(RG2W))) - signedlong(RG6W);
            if (RG4L >= RG7l)
              //m_B29C:
              RG1L = RG3L--;
            else
              //m_B2A1:
              RG1L = RG2L++;
          }
        }

        //m_B2A4:
        vl_04 = 9 * signedlong(vw_20);
        RG4Blo = byte_at(pointer_at(ddat.v1e020c + vl_04 + lcon(0x5)), signedlong(RG1W));
        RG1L = unsignedlong(RG4Blo);
        if (RG1W != vw_18 && RG1W != vw_14)
          break;
      }
    }
    //m_B2DF:
    ui8_to_palettecolor(&palette[vo_0c.peek16()], RG4UBlo);
    vo_0c.inc16();
  }
  FOREND
}

i32 DM2_query_1c9a_08bd(unk* xeaxp)
{
  c_nreg _RG1;
  c_nreg _RG4; RG4P = xeaxp;

  RG1L = 0;
  RG4Blo = byte_at(RG4P, lcon(0x5));
  RG4W = unsignedword(RG4Blo);
  if (RG4W == lcon(0xff))
    return RG1L;
  RG4L = signedlong(RG4W);
  RG1P = DOWNCAST(c_creature, creatures) + 34 * RG4L;
  RG4L = signedlong(byte_at(RG1P, lcon(0x1a)));
  if (RG4L != lcon(0x5))
    return 0;
  RG1Blo = byte_at(RG1P, lcon(0x1f));
  RG1L = signedlong(RG1Blo);
  if (RG1L != 1)
  {
    if (RG1L != 2)
      return 0;
  }
  return 1;
}

i32 DM2_IS_CREATURE_FLOATING(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4;

  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
  RG4P = RG1P;
  RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG4P, lcon(0x4)))));
  RG1W = word_at(RG1P, lcon(0xa)) & lcon(0x4);
  RG1L = unsignedlong(RG1W);
  if (RG1L == 0)
  {
    RG1L = DM2_query_1c9a_08bd(RG4P);
    if (RG1L == 0)
      return RG1L;
  }
  return lcon(0x1);
}

i32 DM2_IS_OBJECT_FLOATING(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4;

  RG4L = RG1L;
  RG4Blo ^= RG1Blo;
  RG4Bhi &= lcon(0x3c);
  RG4L = unsignedlong(RG4W) >> bcon(0xa);
  RG4L = signedlong(RG4W);
  if (RG4L == lcon(0x4))
    return DM2_IS_CREATURE_FLOATING(unsignedlong(RG1W));
  if (RG4L == lcon(0xe))
    return 1;
  if (RG4L != lcon(0xf))
    return 0;
  return 1;
}

// recursive!!
i32 DM2_QUERY_OBJECT_5x5_POS(i32 eaxl, i32 edxl)
{
  c_nreg _RG4; RG4L = edxl;
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;

  RG2L = RG1L;
  RG3L = lcon(0xc);
  RG1W &= lcon(0x3c00);
  RG1L = unsignedlong(RG1W) >> bcon(0xa);
  if (mkul(RG1L) < mkul(lcon(0x5)))
  {
    //m_5AE1:
    if (mkul(RG1L) > mkul(lcon(0x3)))
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
      RG2W = word_at(RG1P, lcon(0xe));
      RG2UW = lrshift6e(RG2UW);
      RG2L = unsignedlong(RG2W);
      RG4L = signedlong(CUTX16(signedlong(RG4W) - RG2L & lcon(0x3)));
      return DM2_QUERY_CREATURE_5x5_POS(RG1P, RG4L);
    }
  }
  else
  {
    bool skip00013 = false;
    if (mkul(RG1L) <= mkul(lcon(0xa)))
      skip00013 = true;
    else
    {
      if (mkul(RG1L) >= mkul(lcon(0xe)))
      {
        if (mkul(RG1L) <= mkul(lcon(0xe)))
          skip00013 = true;
        else
        {
          if (RG1L == lcon(0xf))
          {
            //m_5B14:
            RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG2UW));
            if (jz_test8(location(RG1P + 2), lcon(0xffffff80)))
              skip00013 = true;
          }
        }
      }
    }

    if (skip00013)
    {
      //m_5B24:
      RG1L = unsignedlong(RG2W) >> bcon(0xe);
      RG3W = unsignedword(RG3Blo);
      RG3Blo = table1d268e[RG1L];
    }
  }
  //m_5B34:
  RG4L = signedlong(RG4W);
  RG1W = DM2_ROTATE_5x5_POS(RG3W, t_rotate(RG4UW));
  return RG1L;
}

// stacksize was 0x2c
i32 DM2_query_48ae_05ae(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i32 argl1)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  i8* RG61p;
  c_lreg _RG62;
  c_lreg _RG63;
  c_lreg _RG7;
  c_ql32 vql_00;
  i32 vl_04;
  unk* xp_08;
  i16 vw_0c;
  i16 vw_10;
  i16 vw_14;
  i8 vb_18;

  vql_00.poke32(RG1L);
  vb_18 = RG4Blo;
  vw_10 = RG2W;
  vl_04 = RG3L;
  RG4L = unsignedlong(RG1W);
  RG1L = DM2_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG4L);
  RG2Blo = RG1Blo;
  RG1Blo = DM2_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(RG4L);
  RG4L = unsignedlong(RG2Blo);
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(RG1Blo, RG4Blo, 11, 2);
  RG5W = RG1W;
  RG2L = lcon(0x400);
  RG4L = 0;
  RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x40));
  RG61p = RG1P;
  xp_08 = UPCAST(unk, RG1P);
  RG2W = 3 * vw_10 + lcon(0x12);
  RG1Blo = CUTX8(vw_10);
  RG1Bhi = 3;
  RG1W = unsignedword(RG1Blo) * unsignedword(RG1Bhi);
  RG1Blo += lcon(0xffffffa2);
  RG4L = unsignedlong(RG1Blo);
  RG7L = unsignedlong(vb_18);
  RG1L = DM2_QUERY_GDAT_CREATURE_WORD_VALUE(RG7L, RG4L);
  vw_14 = RG1W; // truncated
  if (RG1W != 0)
  {
    RG4L = signedlong(RG2W);
    RG1L = DM2_QUERY_CREATURES_ITEM_MASK(RG7L, RG4L, RG61p, 0);
    if (RG1L != 0)
    {
      RG1L = vql_00.peek32();
      RG1L <<= bcon(0x10);
      RG1L >>= bcon(0x13);
      RG4L = unsignedlong(byte_at(RG61p, RG1L));
      RG3Blo = vql_00.peek8() & lcon(0x7);
      RG1Blo = 1 << RG3Blo;
      RG1L = unsignedlong(RG1Blo);
      if ((RG1L & RG4L) != 0)
      {
        RG1L = unsignedlong(RG5W);
        RG4L = unsignedlong(vw_14) * RG1L;
        RG1L = RG4L;
        RG4L = RG1L % lcon(0x64);
        RG1L /= lcon(0x64);
        RG5L = RG1L;
      }
    }
  }
  RG2W = 3 * vw_10 + lcon(0x11);
  RG1Blo = CUTX8(vw_10);
  RG1Bhi = 3;
  RG1W = unsignedword(RG1Blo) * unsignedword(RG1Bhi);
  RG1Blo += lcon(0xffffffa1);
  RG4L = unsignedlong(RG1Blo);
  RG1L = DM2_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(vb_18), RG4L);
  vw_0c = RG1W; // truncated
  if (RG1W != 0)
  {
    RG4L = signedlong(RG2W);
    // TODO: 1st arg was RG6L which is *maybe* 0x64, but there is a path to the pointer too
    RG1L = DM2_QUERY_CREATURES_ITEM_MASK(lcon(0x64), RG4L, DOWNCAST(unk, xp_08), 0);
    if (RG1L != 0)
    {
      RG1L = vql_00.peek32();
      RG1L <<= bcon(0x10);
      RG1L >>= bcon(0x13);
      RG1P = DOWNCAST(unk, xp_08) + RG1L;
      RG1Blo = byte_at(RG1P);
      RG1L = unsignedlong(RG1Blo);
      RG3Blo = vql_00.peek8() & lcon(0x7);
      RG4Blo = 1 << RG3Blo;
      RG4L = unsignedlong(RG4Blo);
      if ((RG4L & RG1L) != 0)
      {
        RG1L = unsignedlong(RG5W);
        RG4L = unsignedlong(vw_0c) * RG1L;
        RG1L = RG4L;
        RG4L = RG1L % lcon(0x64);
        RG1L /= lcon(0x64);
        RG5L = RG1L;
      }
    }
  }
  dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0x40));
  if (mkw(argl1) < wcon(0x0))
  {
    RG1L = DM2_GET_MAX_CHARGE(DM2_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(unsignedlong(vql_00.peek16())) << bcon(0xa) & lcon(0xffff));
    argl1 = RG1L;
  }
  RG62L = unsignedlong(vql_00.peek16());
  RG1L = DM2_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG62L);
  RG4L = unsignedlong(RG1Blo);
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(DM2_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(RG62L), RG4Blo, lcon(0xb), lcon(0x35)) * argl1;
  RG5W += RG1W;
  if (argl0 == 0)
    return mkl(RG5W);
  RG1Blo = CUTX8(vw_10);
  RG1Bhi = 3;
  RG1W = unsignedword(RG1Blo) * unsignedword(RG1Bhi);
  RG1Blo += lcon(0xffffffa0);
  RG4L = unsignedlong(RG1Blo);
  RG1L = DM2_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(vb_18), RG4L);
  if (RG1W == 0)
    RG1L = lcon(0xa);
  RG63L = unsignedlong(RG5W);
  RG1L = unsignedlong(RG1W) * RG63L;
  RG4L = RG1L % lcon(0x64);
  RG1L /= lcon(0x64);
  if (vl_04 != 0)
    return mkl(RG5W + RG1W);
  if (RG63L <= 1)
    return mkl(RG5W);
  return mkl(RG5W - DM2_MAX(RG1W, 1));
}

i32 DM2_query_4E26(i16* eaxpw)
{
  c_nreg _RG1; RG1P = DOWNCAST(i16, eaxpw);
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;

  RG4W = word_at(RG1P) & lcon(0x4000);
  RG4L = unsignedlong(RG4W);
  if (RG4L != 0)
    return 0;
  RG4W = word_at(RG1P) & lcon(0x8000);
  RG4L = unsignedlong(RG4W);
  if (RG4L != 0)
  {
    RG4W = word_at(RG1P) & lcon(0x1000);
    RG4L = unsignedlong(RG4W);
    if (RG4L == 0)
    {
      //m_4E68:
      RG4W = word_at(RG1P);
      RG4L &= lcon(0xfc0);
      RG4L = unsignedlong(RG4W) >> 6;
    }
    else
    {
      and16(location(RG1P), lcon(0xffffe03f));
      RG4L = 0;
    }
    //m_4E7A:
    RG2L = unsignedlong(RG4W) + timdat.gametick;
    RG1W = word_at(RG1P) & lcon(0x3f);
    RG3L = unsignedlong(RG1W);
    return RG2L % RG3L;
  }
  //m_4E9D:
  RG1W = word_at(RG1P) & lcon(0x3f);
  return RG1L; // TODO: unextended
}

i16* DM2_query_1c9a_02c3(unk* xeaxp, unk* xedxp)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG4; RG4P = xedxp;

  if (jz_test8(location(RG4P), 1))
  {
    RG4L = unsignedlong(byte_at(RG1P, lcon(0x5)));
    RG1P = DOWNCAST(c_creature, creatures) + 34 * RG4L;
  }
  RG1P += lcon(0x8);
  return RG1P16;
}

void DM2_query_4DA3(i32 eaxl, i32 edxl, i16* ebxpw, s_8bytearray* ecxp_s09)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = DOWNCAST(i16, ebxpw);
  c_nreg _RG3; RG3P = DOWNCAST(s_8bytearray, ecxp_s09);
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p;
  i16* RG6p16;
  i32 vl_00;

  vl_00 = RG4L;
  RG6p16 = UPCAST(i16, RG2P);
  RG5p = RG3P;
  RG4L = unsignedlong(RG1Blo);
  RG3L = lcon(0xfd);
  RG2L = lcon(0x7);
  RG1P = DM2_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0xf), RG4Blo, lcon(0x7), lcon(0xfd));
  RG4P = RG1P;
  RG1P = RG4P + 8 * (DM2_query_4E26(RG6p16) + vl_00 & lcon(0xffff));
  DM2_COPY_MEMORY(RG5p, RG1P, lcon(0x8));
}

i32 DM2_QUERY_CREATURE_5x5_POS(unk* xeaxp, i32 edxl)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p;
  i16 RG6w;
  s_8bytearray s09_00;

  RG5p = RG1P;
  RG6w = RG4W;
  RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xf), byte_at(RG1P, lcon(0x4)), lcon(0x7), lcon(0xfd)) ? 1 : 0;
  if (RG1L == 0)
    return lcon(0xc);
  RG1P = DOWNCAST(i16, DM2_query_1c9a_02c3(RG5p, DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG5p, lcon(0x4)))))));
  DM2_query_4DA3(unsignedlong(byte_at(RG5p, lcon(0x4))), unsignedlong(word_at(RG1P)), UPCAST(i16, RG1P + 2), &s09_00);
  RG4L = signedlong(RG6w);
  RG1W = DM2_ROTATE_5x5_POS(s09_00.barr_00[0x4], t_rotate(RG4UW));
  return RG1L;
}

void DM2_QUERY_RAINFALL_PARAM(OO i8& a, OO i16& d)
{
  d = (unsignedword(ddat.v1e1483) - ddat.v1e12c8) & 0x3;

  i8 r =  (d != 0 && d != 2) ? 0x6d : 0x71;

  //m_3E80E:
  i8 turn = 0;
  ui16 v = unsignedword(ddat.v1e1482);
  if (v < 0x80)
  {
    //m_3E827:
    if (v < 0x40)
    {
      //m_3E833:
      if (v < 0x10)
        turn = 1;
    }
    else
      turn = 2;
  }
  else
    turn = 3;
  //m_3E846:
  a = r + turn;
}

i32 DM2_query_0cee_0897(i16** eaxppw, i32 edxl, i32 ebxl)
{
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG1;
  c_nreg _RG3; RG3P = DOWNCAST(i16*, eaxppw);
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;

  RG5l = signedlong(RG2W);
  RG2L = signedlong(RG4W);
  RG1L = (dm2_GET_TILE_VALUE(RG2L, RG5l) & lcon(0xff)) >> 5 & lcon(0xff);
  if (RG1L != lcon(0x5))
    return 0;
  RG4L = RG5l;
  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(RG2W, RG4W));
  RG4L = RG1L;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
  movp(location(RG3P), RG1P);
  //m_D165:
  for (;;)
  {
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
    RG4L = RG1L;
    if (RG1W == lcon(0xfffffffe))
      return 0;
    RG2L = RG1L;
    RG2Blo ^= RG1Blo;
    RG2Bhi &= lcon(0x3c);
    RG2L = unsignedlong(RG2W) >> bcon(0xa);
    if (RG2L == lcon(0x3))
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
      RG1W = word_at(RG1P, 2) & lcon(0x7f);
      if (RG1W == lcon(0x27))
      {
        RG1P = pointer_at(RG3P);
        RG1W = word_at(RG1P, 2);
        RG1L <<= 4;
        RG1UW >>= 14;
        RG1L = unsignedlong(RG1W) + 2;
        RG1L &= RG2L;
        RG1L++;
        return RG1L;
      }
    }
  }
  FOREND
}

// TODO: return bool
i32 DM2_GET_TELEPORTER_DETAIL(c_5bytes* eaxp_c12, i32 edxl, i32 ebxl)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l; // TODO: bool
  i16 RG6w;
  i16* wp_00;
  i16* wp_04;
  i8 vb_08;

  RG3P = DOWNCAST(c_5bytes, eaxp_c12);

  RG5l = 0;
  RG2L = signedlong(RG2W);
  RG4L = signedlong(RG4W);
  RG1L = DM2_query_0cee_0897(&wp_04, RG4L, RG2L);
  vb_08 = RG1Blo;
  if (vb_08 == 0)
    return 0;
  RG6w = ddat.v1d3248;
  RG1P = DOWNCAST(i16, wp_04);
  RG1W = word_at(RG1P, lcon(0x4));
  RG1UW >>= bcon(0x8);
  RG1L = signedlong(RG1W);
  DM2_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1P = DOWNCAST(i16, wp_04);
  RG1W = word_at(RG1P, 2);
  RG1L <<= bcon(0x6);
  _RG1.ushiftr11();
  RG2L = signedlong(RG1W);
  RG1P = DOWNCAST(i16, wp_04);
  RG1W = word_at(RG1P, 2) & lcon(0x1f);
  RG4L = signedlong(RG1W);
  RG1L = DM2_query_0cee_0897(&wp_00, RG4L, RG2L);
  if (RG1Blo != 0)
  {
    RG5l = 1;
    RG1Bhi = vb_08 - 1;
    mov8(location(RG3P), RG1Bhi);
    RG1Blo--;
    mov8(location(RG3P + 1), RG1Blo);
    RG1P = DOWNCAST(i16, wp_04);
    RG1W = word_at(RG1P, 2);
    RG1Blo &= lcon(0x1f);
    mov8(location(RG3P + 2), RG1Blo);
    RG1P = DOWNCAST(i16, wp_04);
    RG1W = word_at(RG1P, 2);
    RG1L <<= bcon(0x6);
    _RG1.ushiftr11();
    mov8(location(RG3P + lcon(0x3)), RG1Blo);
    RG1P = DOWNCAST(i16, wp_04);
    RG1W = word_at(RG1P, lcon(0x4));
    RG1UW >>= bcon(0x8);
    mov8(location(RG3P + lcon(0x4)), RG1Blo);
  }
  RG1L = signedlong(RG6w);
  DM2_CHANGE_CURRENT_MAP_TO(RG1L);
  return RG5l;
}

i32 DM2_IS_CREATURE_MOVABLE_THERE(i32 eaxl, i32 edxl, i32 ebxl, i16* ecxpw)
{
  c_nreg _RG4; RG4L = edxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG3; RG3P = DOWNCAST(i16, ecxpw);
  i16 RG5w;
  i16 RG6w;
  c_lreg _RG71;
  c_lreg _RG72;
  c_5bytes c12_00;
  c_ql32 vql_08;
  i16 vw_0c;
  i16 vw_10;

  RG6w = RG1W;
  RG5w = RG4W;
  vw_10 = RG2W;
  RG4L = unsignedlong(RG5w);
  RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG6w), RG4L);
  vql_08.poke32(RG1L);
  if (RG1W == lcon(0xffffffff))
    return 0;
  RG71L = unsignedlong(RG1W);
  RG1L = DM2_GET_CREATURE_WEIGHT(RG71L) & lcon(0xffff);
  if (RG1L > lcon(0xfd))
    return 0;
  RG2L = signedlong(RG5w);
  RG4L = signedlong(RG6w);
  RG1L = DM2_GET_TELEPORTER_DETAIL(&c12_00, RG4L, RG2L);
  if (RG1L != 0)
  {
    RG4L = unsignedlong(c12_00.b_00) + 2 & lcon(0x3);
    RG1L = signedlong(vw_10);
    if (RG1L == RG4L)
    {
      bool skip00234 = false;
      RG1L = DM2_IS_CREATURE_ALLOWED_ON_LEVEL(RG71L, unsignedlong(c12_00.b_04));
      if (RG1L == 0)
        return 0;
      vw_0c = ddat.v1d3248;
      DM2_CHANGE_CURRENT_MAP_TO(unsignedlong(c12_00.b_04));
      RG4L = unsignedlong(c12_00.b_03);
      RG1L = DM2_IS_TILE_BLOCKED(dm2_GET_TILE_VALUE(unsignedlong(c12_00.b_02), RG4L) & lcon(0xff));
      if (RG1L != 0)
        skip00234 = true;
      else
      {
        bool skip00235 = false;
        if (RG3P == NULL)
          skip00235 = true;
        else
        {
          RG1Blo = c12_00.b_01;
          RG4W = unsignedword(RG4Blo);
          RG4Blo = c12_00.b_03;
          RG4W += table1d2804[RG1L];
          RG4L = unsignedlong(RG4W);
          RG2W = unsignedword(RG2Blo);
          RG2Blo = c12_00.b_02;
          RG2W += table1d27fc[RG1L];
          RG1L = DM2_GET_CREATURE_AT(RG2L & lcon(0xffff), RG4L);
          RG1Blo = RG1W != wcon(0xffff) ? 1 : 0;
          RG1L = unsignedlong(RG1Blo);
          if (RG1L == 0)
            skip00235 = true;
        }

        if (skip00235)
          //m_14329:
          RG4Blo = 0;
        else
          skip00234 = true;
      }

      if (skip00234)
        //m_14325:
        RG4Blo = 1;

      //m_1432B:
      DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_0c));
      if (RG4Blo != 0)
        return 0;
    }
  }

  //m_1433F:
  RG1L = signedlong(vw_10);
  RG6w += table1d27fc[RG1L];
  RG5w += table1d2804[RG1L];
  RG4L = signedlong(RG5w);
  RG1L = dm2_GET_TILE_VALUE(signedlong(RG6w), RG4L);
  RG72L = unsignedlong(RG1Blo);
  RG1L = DM2_IS_TILE_BLOCKED(RG72L);
  if (RG1L != 0)
    return 0;
  RG1L = RG72L >> 5 & lcon(0xff);
  if (RG1L == lcon(0x3))
    return 0;
  if (RG3P != NULL)
  {
    RG4L = unsignedlong(RG5w);
    RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG6w), RG4L);
    if (RG1W != lcon(0xffffffff))
      return 0;
  }
  RG2L = signedlong(RG5w);
  RG4L = signedlong(RG6w);
  RG1L = DM2_GET_TELEPORTER_DETAIL(&c12_00, RG4L, RG2L);
  if (RG1L != 0)
  {
    RG1L = DM2_IS_CREATURE_ALLOWED_ON_LEVEL(unsignedlong(vql_08.peek16()), unsignedlong(c12_00.b_04));
    if (RG1L == 0)
      return 0;
  }
  if (RG3P != NULL)
  {
    RG1L = vql_08.peek32();
    mov16(location(RG3P), RG1W);
  }
  return 1;
}

i32 DM2_query_0cee_1a46(s_8wordarray* xeaxp, i32 edxl, i32 ebxl, i32 ecxl)
{
  c_nreg _RG1; RG1P = DOWNCAST(s_8wordarray, xeaxp);
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  i8* RG5p;
  c_lreg _RG7;
  i32 vl_00;
  i32 vl_04;
  i32 vl_08;
  i16 vw_0c;
  i16 vw_10;
  i16 vw_14;
  c_ql32 vql_18;
  c_ol32 vol_1c; // poke 32 peek 16 or32
  i8 vb_20;
  bool flag1;
  bool flag2;

  RG5p = RG1P;
  vql_18.poke32(RG4L);
  vl_08 = RG2L;
  vl_00 = RG3L;
  vol_1c.poke32(0);
  //m_E5B8:
  for (;;)
  {
    RG2L = vql_18.peek32();
    RG1Blo = RG2W != wcon(0xfffe) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == 0)
      return vql_18.peek32();
    RG1L = RG2L;
    RG1Blo ^= RG2Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    vw_0c = RG1W;
    RG1L = signedlong(vw_0c);
    if (RG1L > lcon(0x3))
      return vql_18.peek32();
    RG1L = unsignedlong(RG2W) >> bcon(0xe);
    vw_14 = RG1W;
    RG3L = signedlong(vw_14);
    RG1L = 1 << RG3Blo;
    RG4L = unsignedlong(vol_1c.peek16());
    if ((RG1L & RG4L) == 0)
    {
      bool skip00143 = false;
      RG1L = signedlong(vw_14) - vl_08;
      RG1W &= 0x3;
      RG1L += 3;
      vw_10 = RG1W;
      if (vl_00 == 0)
        skip00143 = true;
      else
      {
        RG1L = signedlong(vw_10);
        if (RG1L == lcon(0x5))
          skip00143 = true;
      }

      if (skip00143)
      {
        bool skip00146 = false;
        bool skip00147 = false;
        bool skip00149 = false;
        bool skip00151 = false;
        //m_E648:
        flag2 = false;
        RG2L = 0;
        RG7L = timdat.gametick;
        vl_04 = 0;
        RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(vql_18.peek16())));
        RG3P = RG1P;
        RG4P = RG1P;
        RG1L = signedlong(vw_0c);
        if (RG1L != 2)
        {
          //m_E77E:
          if (RG1L == lcon(0x3))
          {
            RG1Blo = DM2_GET_WALL_DECORATION_OF_ACTUATOR(RG3P);
            vb_20 = RG1Blo;
            if (RG1Blo != lcon(0xffffffff))
            {
              bool skip00150 = false;
              RG1W = word_at(RG3P, 2) & lcon(0x7f);
              if (RG1UW < mkuw(lcon(0x32)))
              {
                //m_E7F4:
                if (RG1UW < mkuw(lcon(0x1a)))
                {
                  //m_E819:
                  if (RG1UW < mkuw(lcon(0x17)))
                    skip00151 = true;
                  else
                  {
                    if (RG1UW <= mkuw(lcon(0x17)))
                    {
                      //m_E88A:
                      RG2W = word_at(RG3P, lcon(0x4));
                      RG2L <<= bcon(0xd);
                      RG2UW >>= bcon(0xf);
                      flag1 = RG2W == wcon(0x0);
                      skip00146 = true;
                    }
                    else
                    {
                      if (RG1W == lcon(0x18))
                        skip00150 = true;
                      else
                        skip00151 = true;
                    }
                  }
                }
                else
                {
                  if (RG1UW <= mkuw(lcon(0x1a)))
                  {
                    //m_E8C3:
                    RG2W = word_at(RG3P, lcon(0x4));
                    RG2L <<= bcon(0xd);
                    RG2UW >>= bcon(0xf);
                    skip00149 = true;
                  }
                  else
                  {
                    if (RG1UW <= mkuw(lcon(0x1b)))
                    {
                      //m_E8D3:
                      RG2W = word_at(RG3P, lcon(0x4)) & 1;
                      RG1W = word_at(RG3P, lcon(0x4));
                      RG1L <<= bcon(0xd);
                      RG1UW >>= bcon(0xf);
                      if (RG2W != RG1W)
                        skip00151 = true;
                      else
                        skip00147 = true;
                    }
                    else
                    {
                      if (RG1W == lcon(0x2c))
                      {
                        //m_E907:
                        if (jz_test8(location(RG3P + lcon(0x4)), 1))
                          skip00151 = true;
                        else
                        {
                          RG1W = word_at(RG3P, 2);
                          RG1UW >>= bcon(0x7);
                          RG1W = unsignedword(RG1Blo);
                          RG1L = unsignedlong(RG1W);
                          vl_04 = RG1L;
                          skip00147 = true;
                        }
                      }
                      else
                        skip00151 = true;
                    }
                  }
                }
              }
              else
              {
                if (RG1UW <= mkuw(lcon(0x32)))
                {
                  //m_E8F1:
                  if (jz_test8(location(RG3P + lcon(0x4)), 1))
                    skip00151 = true;
                  else
                  {
                    RG1W = word_at(RG3P, 2);
                    RG1UW >>= bcon(0x7);
                    RG7L = unsignedlong(RG1W);
                    skip00147 = true;
                  }
                }
                else
                {
                  if (RG1UW < mkuw(lcon(0x46)))
                  {
                    //m_E7D9:
                    if (RG1UW >= mkuw(lcon(0x3f)))
                    {
                      if (RG1UW <= mkuw(lcon(0x3f)))
                      {
                        //m_E830:
                        RG1L = signedlong(vw_10);
                        if (RG1L != lcon(0x5))
                          //m_E849:
                          mov16(location(RG5p + lcon(0xe)), lcon(0xffffffff));
                        else
                        {
                          RG1L = vql_18.peek32();
                          mov16(location(RG5p + lcon(0xe)), RG1W);
                        }
                      }
                      else
                      {
                        if (RG1W == lcon(0x41))
                        {
                          //m_E925:
                          RG2W = word_at(RG3P, 2);
                          RG2UW >>= bcon(0x7);
                        }
                      }
                    }
                    skip00151 = true;
                  }
                  else
                  {
                    if (RG1UW <= mkuw(lcon(0x46)))
                    {
                      //m_E92F:
                      RG2W = word_at(RG3P, lcon(0x4)) & 1;
                      skip00151 = true;
                    }
                    else
                    {
                      if (RG1UW < mkuw(lcon(0x4a)))
                        skip00151 = true;
                      else
                      {
                        if (RG1UW <= mkuw(lcon(0x4a)))
                          skip00150 = true;
                        else
                        {
                          if (RG1W == lcon(0x7e))
                          {
                            //m_E854:
                            if (jz_test8(location(RG3P + lcon(0x4)), lcon(0x4)))
                              skip00151 = true;
                            else
                            {
                              RG1L = signedlong(vw_10);
                              if (RG1L == lcon(0x5))
                              {
                                RG1W = word_at(RG3P, 2);
                                RG1UW >>= bcon(0x7);
                                mov16(location(RG5p + lcon(0xe)), RG1W);
                              }
                              RG3Blo = CUTX8(vw_14);
                              RG1L = 1 << RG3Blo;
                              vol_1c.or32(RG1L);
                              skip00147 = true;
                            }
                          }
                          else
                            skip00151 = true;
                        }
                      }
                    }
                  }
                }
              }

              if (skip00150)
              {
                //m_E89D:
                RG1W = word_at(RG4P, lcon(0x4)) & 1;
                RG3L = unsignedlong(RG1W);
                RG1W = word_at(RG4P, lcon(0x4));
                RG1L <<= bcon(0xd);
                RG1UW >>= bcon(0xf);
                RG1L = unsignedlong(RG1W);
                RG1L ^= RG3L;
                RG2L = RG1L;
                skip00151 = true;
              }
            }
          }
        }
        else
        {
          bool skip00144 = false;
          RG1W = word_at(RG3P, 2);
          RG1L <<= bcon(0xd);
          RG1UW >>= 14;
          if (RG1W == 0)
            skip00144 = true;
          else
          {
            if (RG1W == 1)
            {
              bool skip00148 = false;
              //m_E6D1:
              RG1W = word_at(RG3P, 2);
              RG1UW >>= bcon(0x3);
              vb_20 = RG1Blo;
              RG1W = word_at(RG3P, 2);
              RG1UW >>= bcon(0x3);
              RG3L = RG1L & lcon(0xffff);
              RG3UL >>= bcon(0x8);
              RG3L &= lcon(0x1f);
              if (mkul(RG3L) < mkul(lcon(0x5)))
              {
                //m_E721:
                if (mkul(RG3L) < mkul(2))
                {
                  //m_E732:
                  if (RG3L == 0)
                    skip00151 = true;
                }
                else
                {
                  if (mkul(RG3L) <= mkul(2))
                    skip00147 = true;
                  else
                  {
                    if (RG3L == lcon(0x4))
                      skip00148 = true;
                  }
                }
              }
              else
              {
                bool skip00145 = false;
                if (mkul(RG3L) <= mkul(lcon(0x5)))
                  skip00145 = true;
                else
                {
                  if (mkul(RG3L) < mkul(lcon(0x7)))
                    skip00151 = true;
                  else
                  {
                    if (mkul(RG3L) <= mkul(lcon(0x8)))
                      skip00148 = true;
                    else
                    {
                      if (mkul(RG3L) >= mkul(lcon(0xd)))
                      {
                        if (mkul(RG3L) <= mkul(lcon(0xd)))
                          skip00145 = true;
                        else
                        {
                          if (RG3L == lcon(0xe))
                            skip00144 = true;
                        }
                      }
                    }
                  }
                }

                if (skip00145)
                {
                  //m_E73F:
                  flag1 = jz_test8(location(RG4P + 2), 1);
                  skip00146 = true;
                }
              }

              if (skip00148)
              {
                //m_E753:
                RG2W = word_at(RG4P, 2) & 1;
                skip00149 = true;
              }
            }
          }

          if (skip00144)
          {
            //m_E693:
            RG3L = signedlong(vw_10);
            mov16(location(RG5p + 2 * RG3L), 0);
            if (RG3L == lcon(0x5))
            {
              if (jz_test8(location(RG4P + 2), 1))
                //m_E6B7:
                mov16(location(RG5p + lcon(0xe)), lcon(0xffffffff));
              else
              {
                RG1L = vql_18.peek32();
                mov16(location(RG5p + lcon(0xe)), RG1W);
              }
            }
            //m_E6BD:
            RG3Blo = CUTX8(vw_14);
            RG1L = 1 << RG3Blo;
            vol_1c.or32(RG1L);
          }
        }

        if (skip00149)
        {
          //m_E75C:
          if (RG2W != 0)
          {
            flag2 = true;
            RG3Blo = CUTX8(vw_14);
            RG1L = 1 << RG3Blo;
            vol_1c.or32(RG1L);
          }
          skip00151 = true;
        }

        if (skip00146)
        {
          //m_E743:
          if (flag1)
            skip00151 = true;
          else
            skip00147 = true;
        }

        if (skip00147)
        {
          //m_E749:
          flag2 = true;
          skip00151 = true;
        }

        if (skip00151)
        {
          //m_E938:
          if (flag2)
          {
            RG4L = unsignedlong(vb_20);
            RG3L = vl_04;
            RG2L = RG7L;
            RG1L = DM2_QUERY_ORNATE_ANIM_FRAME(lcon(0x9), RG4L, RG2L, RG3L);
            RG2L = RG1L;
          }
          //m_E954:
          RG2L <<= bcon(0xa);
          RG2L |= unsignedlong(vb_20);
          mov16(location(RG5p + 2 * signedlong(vw_10)), RG2W);
        }
      }
    }

    //m_E96A:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(vql_18.peek16()))));
    vql_18.poke32(RG1L);
  }
  FOREND
}

i32 DM2_query_48ae_011a(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG51l;
  i32 RG52l;
  c_nreg _RG6;

  RG51l = unsignedlong(RG1W);
  RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG51l);
  RG4Blo = RG1Blo;
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG51l);
  RG6L = unsignedlong(RG1Blo);
  RG52l = unsignedlong(RG4Blo);
  if ((DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(CUTLX8(RG52l), RG6Blo, 1, lcon(0x8)) ? 1 : 0) == 0)
    return lcon(0xffffffff);
  if ((DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(CUTLX8(RG52l), RG6Blo, 1, lcon(0xc)) ? 1 : 0) == 0)
    return lcon(0x3);
  if ((DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(CUTLX8(RG52l), RG6Blo, 1, lcon(0xa)) ? 1 : 0) != 0)
    return 1;
  return (DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(CUTLX8(RG52l), RG6Blo, 1, lcon(0x9)) ? 1 : 0) == 0 ? 2 : 0;
}

i32 DM2_query_0cee_2e09(i32 eaxl)
{
  c_nreg _RG1;

  RG1P = DM2_QUERY_CREATURE_AI_SPEC_FROM_TYPE(eaxl & lcon(0xffff));
  RG1W = word_at(RG1P, lcon(0x20));
  return RG1L;
}

i32 DM2_query_1c9a_03cf(i16* eaxpw, i16* edxpw, i32 ebxl)
{
  c_nreg _RG1; RG1P = DOWNCAST(i16, eaxpw);
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4P = DOWNCAST(i16, edxpw);
  i16 RG5w;
  i8* RG61p;
  i16* wp_00;
  i16 vw_04;
  i16 vw_08;
  i16 vw_0c;
  i16 vw_10;
  i16 vw_14;
  i16 vw_18;
  i16 vw_1c;
  i16 vw_20;
  i16 vw_24;
  c_ql32 vql_28;

  RG61p = RG1P;
  wp_00 = RG4P16;
  vw_24 = RG2W; // truncated
  RG1W = word_at(RG1P);
  vw_18 = RG1W;
  RG1W = word_at(RG4P);
  vw_1c = RG1W;
  RG1L = unsignedlong(RG2W);
  if (RG1L != lcon(0xff))
    //m_1FCCA:
    RG1W = table1d2752[RG1L];
  else
    RG1L = lcon(0xc);
  //m_1FCD2:
  RG2L = unsignedlong(RG1W);
  DM2_query_098d_000f(vw_18, vw_1c, RG2W, OO vw_04, OO vw_14);
  vw_20 = 4;
  RG5w = wcon(0x0);
  //m_1FE16:
  for (;;)
  {
    if (RG5w > vw_20)
      return lcon(0xffff);
    //m_1FCF6:
    RG4L = unsignedlong(vw_1c);
    RG1L = DM2_GET_CREATURE_AT(unsignedlong(vw_18), RG4L);
    vql_28.poke32(RG1L);
    if (RG1W != lcon(0xffffffff))
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(vql_28.peek16())));
      RG2P = RG1P;
      RG1P = DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG2P, lcon(0x4)))));
      RG1W = signedword(byte_at(RG1P, lcon(0x23)));
      vw_08 = RG1W;
      RG1W = word_at(RG2P, lcon(0xe));
      RG1UW = lrshift6e(RG1UW);
      RG1L = unsignedlong(RG1W);
      RG4L = unsignedlong(table1d62e8[RG1L]);
      RG1L = DM2_QUERY_CREATURE_5x5_POS(RG2P, RG4L);
      RG2L = unsignedlong(RG1W);
      RG4L = signedlong(vw_1c);
      DM2_query_098d_000f(vw_18, RG4W, RG2W, OO vw_10, OO vw_0c);
      RG1W = vw_04; // adj
      RG4W = vw_10 - RG1W; // adj
      vw_10 = RG4W; // adj
      RG1W = vw_14; // adj
      RG2W = vw_0c - RG1W; // adj
      vw_0c = RG2W; // adj
      RG1W = RG4W * RG4W; // adj
      RG4W = RG2W * RG2W + RG1W; // adj
      RG1L = signedlong(vw_08);
      if (RG4W < table1d62e0[RG1L])
      {
        RG1W = vw_18;
        mov16(location(RG61p), RG1W);
        RG1W = vw_1c;
        mov16(location(DOWNCAST(i16, wp_00)), RG1W);
        return vql_28.peek32();
      }
    }
    //m_1FDB6:
    RG1L = unsignedlong(vw_24);
    if (RG1L != lcon(0xff))
    {
      //m_1FDE7:
      RG1L *= 8;
      RG2L = 4 * signedlong(RG5w) + RG1L;
      RG1W = word_at(RG61p);
      RG4W = table1d62b0[RG2L / 4][0];
      RG1L += RG4L;
      vw_18 = RG1W;
      RG1P = DOWNCAST(i16, wp_00);
      RG1W = word_at(RG1P);
      RG3W = table1d62b0[RG2L / 4][1];
    }
    else
    {
      RG2L = signedlong(RG5w);
      RG1W = word_at(RG61p);
      RG4W = table1d62d0[RG2L][0];
      RG1L += RG4L;
      vw_18 = RG1W;
      RG1P = DOWNCAST(i16, wp_00);
      RG1W = word_at(RG1P);
      RG3W = table1d62d0[RG2L][1];
    }
    //m_1FE0F:
    RG1L += RG3L;
    vw_1c = RG1W;
    RG5w++;
  }
  FOREND
}

i32 DM2_query_0cee_2df4(i32 eaxl)
{
  c_nreg _RG1;

  RG1P = DM2_QUERY_CREATURE_AI_SPEC_FROM_TYPE(eaxl & lcon(0xffff));
  RG1W = word_at(RG1P, lcon(0x1e));
  return RG1L;
}

i32 DM2_query_48ae_01af(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG4; RG4L = edxl;

  RG2L = RG1L;
  RG2Blo ^= RG1Blo;
  RG2Bhi &= 0x4;
  if (RG2W != 0)
  {
    RG2L = RG1L;
    RG2Blo ^= RG1Blo;
    RG2Bhi &= 0x2;
    if (RG2W == 0)
    {
      //m_5354:
      RG1W &= lcon(0xf);
      if (RG1W != 0)
      {
        RG1L = signedlong(RG1W);
        RG2L = 4 * RG1L;
        RG1L = unsignedlong(RG4W);
        RG1Blo = table1d2660[RG2L + RG1L - 4]; // EVIL offset shifted, was 0x1d265c
        return RG1L;
      }
      RG1Blo = 0;
      return RG1L;
    }
  }
  //m_5350:
  RG1Blo = lcon(0xf);
  return RG1L;
}

i32 DM2_query_0cee_2e35(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4;

  RG1L = DM2_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(RG1Blo), lcon(0x4));
  RG4L = RG1L;
  return RG1W != 0 ? RG4L : lcon(0x4);
}

// stacksize was 0x5c
void DM2_QUERY_CREATURE_PICST(i32 eaxl, i32 edxl, unk* xebxp, unk* xecxp, i16 argw0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = xebxp;
  c_nreg _RG3; RG3P = xecxp;
  c_nreg _RG4; RG4L = edxl;
  i8* RG51p;
  c_wlreg _RG52;
  i32 RG53l;
  c_wlreg _RG6;
  i8* RG71p;
  i32 RG72l;
  c_lreg _RG73;
  c_ql32 vql_00;
  c_ql32 vql_04;
  s_8bytearray s09_08;
  s_8bytearray s09_10;
  i32 vl_18;
  i32 vl_1c;
  i16 vw_20;
  i16 vw_24;
  i16 vw_28;
  i16 vw_2c;
  c_ql32 vql_30;
  i16 vw_34;
  i16 vw_38;
  i16 vw_3c;
  i8 vb_40;
  i8 vb_44;
  i8 vb_48;
  i16 parw01;
  i16 parw04;
  i8 parb07;
  i8 parb08;

  vql_04.poke32(RG1L);
  vql_00.poke32(RG4L);
  RG71p = RG2P;
  RG51p = RG3P;
  vl_1c = 0;
  RG1Blo = byte_at(RG2P, lcon(0x4));
  vb_48 = RG1Blo;
  RG6L = 0;
  RG1L = unsignedlong(vb_48);
  vl_18 = RG1L;
  RG1P = DOWNCAST(i16, DM2_query_1c9a_02c3(RG2P, DOWNCAST(c_aidef, DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(RG1L))));
  DM2_query_4DA3(vl_18, unsignedlong(word_at(RG1P)), UPCAST(i16, RG1P + 2), &s09_10);
  if (RG51p != NULL)
    //m_40EEF:
    RG1L = unsignedlong(byte_at(RG51p, lcon(0x7)));
  else
    RG1L = 0;
  //m_40EF4:
  vw_3c = RG1W;
  RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(argw0));
  if ((RG1Blo & bcon(0x4)) == 0)
  {
    //m_40F12:
    RG1W = word_at(RG71p, lcon(0xe));
    RG1UW = lrshift6e(RG1UW);
    RG1L = unsignedlong(RG1W);
    RG4L = signedlong(ddat.v1e12c8) - RG1L;
    RG1L = RG4L & lcon(0x3);
    vw_24 = RG1W;
  }
  else
    vw_24 = 2;
  //m_40F37:
  RG72l = signedlong(vw_24);
  RG1Blo = s09_10.barr_00[RG72l];
  vb_44 = RG1Blo;
  RG3L = unsignedlong(RG1Blo);
  RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xf), vb_48, 1, RG3Blo) ? 1 : 0;
  if (RG1L != 0)
  {
    bool skip00899 = false;
    //m_40FBF:
    RG3L = 2 * (lcon(0x3) - RG72l);
    RG1L = unsignedlong(s09_10.barr_00[0x7]) >> RG3Blo & lcon(0x3);
    RG4L = RG1L;
    RG4Bhi ^= RG1Bhi;
    RG4Blo &= 0x1;
    RG4L = signedlong(RG4W);
    if (RG4L != 0)
      skip00899 = true;
    else
    {
      RG2L = unsignedlong(vw_3c);
      RG2W &= lcon(0x40);
      RG2L = signedlong(RG2W);
      if (RG2L != 0)
      {
        RG1W &= 0x2;
        RG1L = signedlong(RG1W);
        if (RG1L != 0 && RG4L == 0)
          skip00899 = true;
      }
    }

    if (skip00899)
      //m_40FFE:
      RG6L = 1;
  }
  else
  {
    RG1Blo = CUTX8(vw_24) + bcon(0x2);
    RG1Bhi = RG1Blo & bcon(0x3);
    vb_44 = RG1Bhi;
    RG1Blo &= 0x1;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L != 0)
    {
      RG6L = 1;
      vl_1c = 1;
    }
    RG1L = unsignedlong(vb_44);
    RG1Blo = s09_10.barr_00[RG1L];
    vb_44 = RG1Blo;
    RG3L = unsignedlong(RG1Blo);
    RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xf), vb_48, 1, RG3Blo) ? 1 : 0;
    if (RG1L == 0)
    {
      RG6L = 0;
      RG1Blo = s09_10.barr_00[0x2];
      vb_44 = RG1Blo;
    }
  }
  //m_41003:
  RG3L = unsignedlong(vb_44);
  RG73L = unsignedlong(vb_48);
  RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xf), CUTLX8(RG73L), 1, RG3Blo) ? 1 : 0;
  if (RG1L == 0)
  {
    vql_30.poke32(RG6L);
    RG6L = 0;
    RG1Blo = CUTX8(vw_24) + bcon(0xfa);
    vb_44 = RG1Blo;
    RG3L = unsignedlong(RG1Blo);
    RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xf), CUTLX8(RG73L), 1, RG3Blo) ? 1 : 0;
    if (RG1L == 0)
    {
      bool skip00901 = false;
      RG1W = vw_24;
      RG1W &= 0x1;
      RG1L = signedlong(RG1W);
      if (RG1L == 0)
        skip00901 = true;
      else
      {
        RG1Blo = (CUTX8(vw_24) + bcon(0x2) & bcon(0x3)) + bcon(0xfa);
        vb_40 = RG1Blo;
        RG3L = unsignedlong(RG1Blo);
        RG2L = 1;
        RG4L = RG73L;
        RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xf), RG4Blo, RG2Blo, RG3Blo) ? 1 : 0;
        if (RG1L == 0)
          skip00901 = true;
        else
        {
          RG6L = 1;
          RG1Blo = vb_40;
          vb_44 = RG1Blo;
          vl_1c = 1;
        }
      }

      if (skip00901)
      {
        //m_4109B:
        vb_44 = lcon(0xfffffffc);
        RG6L = vql_30.peek32();
      }
    }
    //m_410A4:
    if (RG6W != vql_30.peek16())
      vl_1c = 1;
  }
  bool skip00904 = false;
  //m_410B3:
  RG1L = signedlong(vql_00.peek16());
  RG1Blo = table1d6b71[RG1L];
  RG1W = unsignedword(RG1Blo);
  vw_38 = RG1W;
  if (RG51p == NULL)
    skip00904 = true;
  else
  {
    RG1L = signedlong(byte_at(RG51p, lcon(0x1a)));
    if (RG1L != lcon(0x13))
      skip00904 = true;
    else
    {
      RG1W = word_at(RG51p, lcon(0x10));
      vw_20 = RG1W; // adj
      DM2_query_4DA3(unsignedlong(vb_48), unsignedlong(word_at(RG51p, lcon(0xe))), &vw_20, &s09_08);
      RG1Blo = s09_08.barr_00[0x5];
      s09_10.barr_00[0x5] = RG1Blo;
    }
  }

  if (skip00904)
    //m_41101:
    DM2_COPY_MEMORY(ADRESSOF(s_8bytearray, &s09_08), ADRESSOF(s_8bytearray, &s09_10), lcon(0x8));
  //m_41113:
  RG1L = unsignedlong(s09_08.barr_00[0x4]);
  vw_28 = RG1W;
  RG1L = signedlong(vql_04.peek16());
  if (RG1L != lcon(0x3) || ddat.v1e12ce == 0)
  {
    //m_41160:
    RG4L = unsignedlong(vb_48);
    RG3L = lcon(0xfe);
    RG2L = lcon(0x7);
    RG1P = DM2_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0xf), RG4Blo, RG2Blo, RG3Blo);
    RG4L = signedlong(vw_38);
    RG2P = RG1P + 4 * unsignedlong(s09_10.barr_00[0x5]);
    RG1L = blitter.calc_stretched_size(unsignedword(byte_at(RG2P, signedlong(vw_24))), RG4W);
    vw_38 = RG1W;
  }
  else
  {
    RG53l = signedlong(ddat.v1d324a);
    RG1W = signedword(table1d6b76[RG53l + 0x42]);
    vw_28 = RG1W;
    RG1W = signedword(table1d6b76[RG53l + 0x49]);
    vw_38 = RG1W;
    vw_24 = 0;
  }
  //m_411A5:
  RG2L = signedlong(vw_24);
  RG4L = signedlong(vw_28);
  RG1W = DM2_QUERY_CREATURE_BLIT_RECTI(signedlong(vql_04.peek16()), t_rotate(RG2UW), RG4W);
  RG1Bhi |= lcon(0xffffff80);
  RG3L = RG1L;
  RG52W = signedword(s09_08.barr_00[0x6]);
  RG2L = RG52L;
  if (RG52W != 0)
  {
    if (mkuw(vw_24) <= mkuw(lcon(0x3)))
    {
      bool skip00906 = false;
      RG1L = unsignedlong(vw_24);
      switch (RG1L)
      {
        case 0:
          //m_411EC:
          vw_34 = wcon(0xfff9);
          skip00906 = true;
          break;

        case 1:
        case 3:
          //m_41205:
          if (vl_1c == 0)
            //m_41215:
            vw_2c = wcon(0xffc0);
          else
            vw_2c = 0x40;
          //m_4121C:
          RG1L = 0;
          vw_34 = RG1W;
          break;

        case 2:
          //m_411F5:
          vw_34 = 7;
          skip00906 = true;
          break;

        default: throw(THROW_DMABORT);
      }

      if (skip00906)
      {
        //m_411FC:
        RG1L = 0;
        vw_2c = RG1W;
      }
    }

    //m_41223:
    RG4L = signedlong(RG2W);
    RG2L = blitter.calc_stretched_size(vw_2c, RG4W);
    RG4L = signedlong(RG52W);
    RG52L = blitter.calc_stretched_size(vw_34, RG4W);
  }

  //m_41245:
  RG1L = unsignedlong(vb_44);
  put8(parb08, RG1Blo);
  RG1L = unsignedlong(vb_48);
  put8(parb07, RG1Blo);
  RG1L = signedlong(CUTX16(DM2_query_0cee_2e35(RG1L)));
  put16(parw04, RG1W);
  RG1L = signedlong(RG3W);
  i32 parl02 = RG1L;
  RG1L = signedlong(vql_00.peek16());
  put16(parw01, RG1W);
  RG1L = unsignedlong(vw_3c);
  RG1L <<= bcon(0x10);
  RG1L >>= bcon(0x13);
  RG1L &= lcon(0x7);
  RG1W = signedword(table1d6b76[RG1L + 0x50]);
  RG1W += RG52W;
  RG1L = signedlong(RG1W);
  i32 parl00 = RG1L;
  RG1L = unsignedlong(vw_3c);
  RG1W &= lcon(0x7);
  RG1L = signedlong(RG1W);
  RG1W = signedword(table1d6b76[RG1L + 0x50]);
  RG1W += RG2W;
  RG3L = signedlong(RG1W);
  RG4L = signedlong(vw_38);
  RG1L = unsignedlong(RG6W);
  DM2_QUERY_TEMP_PICST(
    RG1W, RG4W, RG4W, RG3W,
    CUTX16(parl00), parw01, CUTX16(parl02), -1,
    parw04, lcon(0xffff), lcon(0xf), parb07, parb08);
}

// recursive!!!
i32 DM2_query_2fcf_164e(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  RG3L = RG4L;
  RG4L = RG1L;
  RG4Blo ^= RG1Blo;
  RG4Bhi &= lcon(0x3c);
  RG4L = unsignedlong(RG4W) >> bcon(0xa);
  if (RG4L != lcon(0x9))
    return 0;
  RG4L = unsignedlong(RG1W);
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L) & lcon(0xff);
  if (RG1L >= lcon(0x8))
    return 0;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
  RG1W = word_at(RG1P, 2);
  //m_3BD8D:
  for (;;)
  {
    if (RG1W == lcon(0xfffffffe))
      return 0;
    RG2L = unsignedlong(RG1W);
    RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG2W);
    if (RG1W == RG3W)
      return 1;
    RG4L = signedlong(RG3W);
    RG1L = DM2_query_2fcf_164e(RG2L, RG4L);
    if (RG1L != 0)
      return 1;
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG2UW)));
  }
  FOREND
}

i32 DM2_query_2fcf_16ff(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG5l;
  i8* RG6p;
  c_ql32 vql_00;
  i32 vl_04;
  bool flag = false;
  bool skipinloop = false;

  vql_00.poke32(RG1L);
  RG4L = 0;
  vl_04 = RG4L;
  c_hero* hero = &party.hero[0];
  //m_3BE94:
  for (;;)
  {
    bool skip01053 = false;
    RG1L = vl_04;
    if (RG1UW < mkuw(party.heros_in_party))
    {
      //m_3BE36:
      if (hero->curHP != 0)
      {
        RG6p = DOWNCAST(i16, &hero->item[0]);
        RG2L = 0;
        skipinloop = true;
        skip01053 = true;
      }
    }
    else
    {
      RG1L = signedlong(ddat.v1d67bc);
      if (RG1L == lcon(0x5))
      {
        RG2L = 0;
        //m_3BED1:
        for (;;)
        {
          RG1L = unsignedlong(RG2W);
          if (RG1L >= lcon(0x8))
            break;
          //m_3BEB2:
          RG1W = party.hand_container[RG1L];
          if (RG1W != lcon(0xffffffff))
          {
            RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG1W);
            if (RG1W == vql_00.peek16())
              return 1;
          }
          //m_3BED0:
          RG2L++;
        }
      }
      //m_3BEDB:
      if (flag)
        return 0;
      flag = true;
      RG1W = ddat.savegamewpc.w_00;
      skip01053 = true;
    }

    if (skip01053)
    {
      //m_3BE4D:
      for (;;)
      {

        if (!skipinloop)
        {
          RG5l = unsignedlong(RG1W);
          RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(CUTX16(RG5l));
          if (RG1W == vql_00.peek16())
            return 1;
          RG4L = signedlong(vql_00.peek16());
          RG1L = DM2_query_2fcf_164e(RG5l, RG4L);
          if (RG1L != 0)
            return 1;
          //m_3BE7B:
          RG2L++;
        }
        //m_3BE7C:
        skipinloop = false;
        RG1L = unsignedlong(RG2W);
        if (RG1L >= lcon(0x1e) || flag)
          break;
        //m_3BE47:
        RG1W = word_at(RG6p);
        RG6p += 2;
      }
    }
    //m_3BE8A:
    vl_04++;
    hero++; // struct advance
  }
  FOREND
}

// routine had one unused argument
// stacksize was 0x1c
i32 DM2_query_48ae_0767(i32 eaxl, i32 edxl, i16* ebxpw, unk* xecxp)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2P = DOWNCAST(i16, ebxpw);
  c_nreg _RG3; RG3P = xecxp;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  i32 RG6l;
  i8* RG7p;
  unk* xp_00;
  i32 vl_04; // add32
  i16 vw_08;

  RG5L = RG1L;
  vw_08 = RG4W; // truncated
  xp_00 = UPCAST(unk, RG2P);
  RG2L = 0;
  RG1W = ddat.v1e03fe;
  RG1L--;
  vl_04 = RG2L;
  mov16(location(RG3P), RG2W);
  RG7p = DOWNCAST(unk, xp_00);
  //m_5A2B:
  for (;;)
  {
    for (;;)
    {
      if (RG2W >= vw_08 || RG5W <= 0 || RG1W < 0)
        return vl_04;
      RG4L = signedlong(RG1W);
      RG4W = ddat.v1e03ac[RG4L];
      if (RG4W <= 0)
        return vl_04;
      if (RG5W < RG4W)
        break;
      RG6l = signedlong(RG2W);
      RG2L++;
      mov8(location(RG7p + RG6l), RG1Blo);
      RG5L -= RG4L;
      vl_04 += RG4L;
      inc16(location(RG3P));
    }
    //m_5A64:
    RG1L--;
  }
  FOREND
}

i32 DM2_query_2759_0155(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG5l;
  c_lreg _RG61;
  c_lreg _RG62;
  c_nreg _RG7;
  i16 vw_00;
  i8 vb_04;
  i8 vb_08;
  i8 vb_0c;

  vw_00 = RG1W; // truncated
  if (RG1W == lcon(0xffffffff))
    return 0;
  RG4L = unsignedlong(RG1W);
  RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG4L);
  vb_04 = RG1Blo;
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L);
  vb_08 = RG1Blo;
  vb_0c = lcon(0x8);
  RG7L = unsignedlong(vb_04);
  //m_2F99A:
  for (;;)
  {
    RG5l = unsignedlong(vb_0c);
    if (RG5l >= lcon(0xc))
      return 0;
    //m_2F932:
    RG61L = unsignedlong(vb_08);
    RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(RG7Blo, CUTLX8(RG61L), lcon(0x5), CUTLX8(RG5l)) ? 1 : 0;
    if (RG1L != 0)
    {
      RG3L = 2;
      RG2L = RG5l;
      RG4L = RG61L;
      RG1L = DM2_QUERY_CMDSTR_ENTRY(RG7L, RG4L, RG2L, RG3L);
      if (RG1W != 0)
      {
        RG4L = unsignedlong(vw_00);
        RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L);
        RG62L = unsignedlong(RG1Blo);
        RG1L = DM2_QUERY_CMDSTR_ENTRY(DM2_QUERY_CLS1_FROM_RECORD(RG4L) & lcon(0xff), RG62L, RG5l, lcon(0x8));
        if (RG1W != 0)
          return 1;
      }
    }
    //m_2F996:
    vb_0c++;
  }
  FOREND
}

i32 DM2_query_2759_01fe(i32 ebxl, i32 edxl)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;

  RG5w = RG2W;
  if (RG4W == lcon(0xffffffff))
    return 0;
  //m_2F9E7:
  RG1L = RG4L;
  RG1Blo ^= RG4Blo;
  RG1Bhi &= lcon(0x3c);
  RG1L = unsignedlong(RG1W) >> bcon(0xa);
  if (RG1L != lcon(0x9))
    return 1;
  RG4L = unsignedlong(RG4W);
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
  RG2P = RG1P;
  RG1W = word_at(RG1P, lcon(0x4)) & lcon(0x6);
  if (RG1W != 2)
    return 1;
  RG3W = word_at(RG2P, lcon(0x4));
  RG3UW >>= bcon(0xd);
  if (RG3UW < mkuw(1) || RG3UW > RG1UW)
    return 0;
  RG1P = DM2_GET_MISSILE_REF_OF_MINION(unsignedlong(word_at(RG2P, 2)), RG4L);
  if (RG1P != NULL)
  {
    RG4L = unsignedlong(RG5w);
    if (RG4L == lcon(0x30))
      return 1;
    RG1L = signedlong(RG3W);
    if (RG1L != 2)
      return 0;
    RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(word_at(RG2P, 2))));
    RG1Blo = byte_at(RG1P, lcon(0x4));
    if (RG1Blo == lcon(0x33))
      return RG4L != lcon(0x2d) ? 0 : 1;
    if (RG1Blo != lcon(0x32))
      return 0;
    return RG4L != lcon(0x2e) ? 0 : 1;
  }
  RG1W = RG5w;
  if (RG1L == lcon(0x2f))
    return 1;
  if (RG1L == lcon(0x2c))
    return 1;
  if (word_at(RG2P + lcon(0x6)) == lcon(0xffffffff))
    return 0;
  if (RG1L == lcon(0x2d))
    return 1;
  return RG1L != lcon(0x2e) ? 0 : 1;
}

// was SKW_2759_02d6
i32 DM2_IS_ITEM_HAND_ACTIVABLE(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG51l;
  i32 RG52l;
  c_wlreg _RG53;
  i32 RG54l;
  i32 RG55l;
  c_lreg _RG61;
  i32 RG62l;
  c_lreg _RG63;
  c_lreg _RG71;
  c_lreg _RG72;
  c_ql32 vql_00;
  c_ql32 vql_04;
  c_ql32 vql_08;
  i16 vw_0c;
  c_o32 vo_10; // poke 16 peek 16 32 inc16
  i16 vw_14;
  i8 vb_18;
  i8 vb_1c;
  i8 vb_20; // inc8
  bool flag;

  vql_08.poke32(RG1L);
  vql_04.poke32(RG4L);
  vql_00.poke32(RG2L);
  if (RG4W != lcon(0xffffffff))
  {
    //m_2FAD6:
    RG4L = unsignedlong(vql_04.peek16());
    RG1L = DM2_IS_CONTAINER_MONEYBOX(RG4L);
    if (RG1L != 0)
      return 1;
    RG1L = DM2_IS_CONTAINER_CHEST(RG4W) ? 1 : 0;
    if (RG1L != 0)
      return 1;
    //m_2FAFD:
    RG1Blo = DM2_QUERY_CLS1_FROM_RECORD(RG4L);
    vb_18 = RG1Blo;
    RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L);
  }
  else
  {
    vb_18 = lcon(0x16);
    RG4L = unsignedlong(vql_08.peek16());
    RG1L = 263 * RG4L;
    RG1Blo = party.hero[RG4L].herotype;
  }
  //m_2FB0F:
  vb_1c = RG1Blo;
  RG2L = 0;
  vo_10.poke16(RG2W);
  vb_20 = lcon(0x8);
  //m_2FCD2:
  for (;;)
  {
    RG51l = unsignedlong(vb_20);
    if (RG51l >= lcon(0xc))
      break;
    //m_2FB24:
    RG1L = signedlong(vo_10.peek16());
    if (RG1L >= lcon(0x3))
      break;
    RG61L = unsignedlong(vb_1c);
    RG71L = unsignedlong(vb_18);
    RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(CUTLX8(RG71L), CUTLX8(RG61L), lcon(0x5), CUTLX8(RG51l)) ? 1 : 0;
    if (RG1L != 0)
    {
      RG3L = 2;
      RG2L = RG51l;
      RG4L = RG61L;
      RG1L = DM2_QUERY_CMDSTR_ENTRY(RG71L, RG4L, RG2L, RG3L);
      vw_0c = RG1W; // truncated
      if (RG1W != 0)
      {
        bool skip00681 = false;
        RG3L = lcon(0x11);
        RG2L = RG51l;
        RG4L = RG61L;
        RG1L = DM2_QUERY_CMDSTR_ENTRY(RG71L, RG4L, RG2L, RG3L);
        if (RG1W == 0)
          skip00681 = true;
        else
        {
          RG1L = signedlong(RG1W);
          RG4L = signedlong(vql_00.peek16());
          RG1L--;
          if (RG1L == RG4L)
            skip00681 = true;
        }

        if (skip00681)
        {
          bool skip00682 = false;
          bool skip00683 = false;
          //m_2FB99:
          RG3L = vql_04.peek32();
          if (RG3W == lcon(0xffffffff))
          {
            //m_2FC2A:
            RG1L = unsignedlong(vw_0c);
            if (RG1L != lcon(0x11))
              skip00683 = true;
            else
            {
              RG4L = signedlong(vql_00.peek16());
              RG1L = DM2_FIND_POUCH_OR_SCABBARD_POSSESSION_POS(signedlong(vql_08.peek16()), RG4L);
              flag = RG1W < 0;
              skip00682 = true;
            }
          }
          else
          {
            RG2L = unsignedlong(vw_0c);
            RG55l = unsignedlong(RG3W);
            RG1L = DM2_query_2759_01fe(RG2L, RG55l);
            if (RG1L != 0)
            {
              RG1L = DM2_QUERY_CMDSTR_ENTRY(unsignedlong(vb_18), unsignedlong(vb_1c), unsignedlong(vb_20), lcon(0x8));
              RG2L = RG1L;
              RG62l = signedlong(RG1W);
              if (RG62l != lcon(0x12))
              {
                //m_2FC03:
                if (RG62l == lcon(0x10) || RG62l == lcon(0x11))
                  RG2L = 1;
                if (RG2W == 0)
                  skip00683 = true;
                else
                {
                  RG1W = DM2_ADD_ITEM_CHARGE(unsignedlong(vql_04.peek16()), 0);
                  flag = RG1W < RG2W;
                  skip00682 = true;
                }
              }
              else
              {
                RG4L = 0;
                RG1W = DM2_ADD_ITEM_CHARGE(RG55l, RG4L);
                if (RG1W == 0)
                  skip00683 = true;
              }
            }
          }

          if (skip00682)
          {
            //m_2FC4C:
            if (!flag)
              skip00683 = true;
          }

          if (skip00683)
          {
            //m_2FC52:
            RG63L = unsignedlong(vb_20);
            RG72L = unsignedlong(vb_1c);
            RG52l = unsignedlong(vb_18);
            RG3L = 0;
            RG2L = RG63L;
            RG4L = RG72L;
            RG1L = DM2_QUERY_CMDSTR_ENTRY(RG52l, RG4L, RG2L, RG3L);
            vw_14 = RG1W; // truncated
            RG1L = DM2_QUERY_CMDSTR_ENTRY(RG52l, RG72L, RG63L, 1);
            RG53L = RG1L;
            RG4L = unsignedlong(vw_14);
            RG1W = DM2_QUERY_PLAYER_SKILL_LV(vql_08.peek16(), RG4W, 1);
            if (RG1UW >= RG53UW)
            {
              RG54l = signedlong(vo_10.peek16());
              RG1Blo = vb_18;
              ddat.v1e0b40[RG54l].b_00 = RG1Blo;
              RG1Blo = vb_1c;
              ddat.v1e0b40[RG54l].b_01 = RG1Blo;
              RG1Blo = vb_20;
              ddat.v1e0b40[RG54l].b_02 = RG1Blo;
              vo_10.inc16();
            }
          }
        }
      }
    }

    //m_2FCCE:
    vb_20++;
  }
  //m_2FCE0:
  RG1W = vo_10.peek16();
  ddat.v1e0b7a = RG1W;
  RG1L = vql_04.peek32();
  ddat.v1e0b50 = RG1W;
  RG1L = DM2_IS_CONTAINER_MAP(unsignedlong(vql_04.peek16()));
  if (RG1L != 0)
    return 1;
  RG1Blo = vo_10.peek16() > wcon(0x0) ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  return RG1L;
}

i32 DM2_query_0cee_06dc(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;

  RG3L = RG1L;
  RG2L = RG4L;
  RG4L = signedlong(RG4W);
  RG1L = dm2_GET_TILE_VALUE(signedlong(RG1W), RG4L);
  RG1Blo &= lcon(0x8);
  RG1L = unsignedlong(RG1Blo);
  RG1Blo = RG1L == 0 ? 1 : 0;
  RG1W = unsignedword(RG1Blo);
  RG5l = RG1L;
  if (RG1W == 0)
  {
    //m_CF53:
    RG3W += table1d27fc[0x0];
    RG2W += table1d2804[0x0];
  }
  else
  {
    RG3W += table1d27fc[0x1];
    RG2W += table1d2804[0x1];
  }
  //m_CF61:
  RG4L = signedlong(RG2W);
  RG1L = (dm2_GET_TILE_VALUE(signedlong(RG3W), RG4L) & lcon(0xff)) >> 5;
  RG1W = unsignedword(RG1Blo);
  if (RG1W != 0)
  {
    RG1L = signedlong(RG1W);
    if (RG1L != lcon(0x3))
      return RG5l;
  }
  return 2 + RG5l;
}

// stacksize was 0x20
i32 DM2_query_19f0_124b(i16* eaxpw, i16* edxpw, i32 ebxl, i32 ecxl, i32 argl0)
{
  c_nreg _RG1; RG1P = DOWNCAST(i16, eaxpw);
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4P = DOWNCAST(i16, edxpw);
  i8* RG5p;
  i8* RG61p;
  unk* xp_00;
  i16 vw_04;
  i32 vl_08;
  i16 vw_0c;
  bool flag1;
  bool flag2 = false;

  RG61p = RG1P;
  RG5p = RG4P;
  vw_04 = RG2W;
  vw_0c = RG3W;
  vl_08 = lcon(0xffffffff);
  DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_04));
  RG4L = signedlong(word_at(RG5p));
  RG1L = dm2_GET_TILE_VALUE(signedlong(word_at(RG61p)), RG4L);
  RG4Blo = RG1Blo;
  RG1L = unsignedlong(RG4Blo);
  RG4L = unsignedlong(RG4Blo) >> 5;
  RG2Blo = RG4Blo;
  RG4L = unsignedlong(RG2Blo);
  RG2L = unsignedlong(RG2Blo);
  if (RG2L != lcon(0x3))
  {
    bool skip00450 = false;
    //m_1D49F:
    RG2L = signedlong(RG4W);
    if (RG2L != 2)
      skip00450 = true;
    else
    {
      RG2L = argl0;
      RG2W &= lcon(0x8);
      RG2L = unsignedlong(RG2W);
      if (RG2L == 0 || RG3W != wcon(0x1))
        skip00450 = true;
      else
      {
        RG2L = RG1L;
        RG2Bhi ^= RG1Bhi;
        RG2Blo &= lcon(0x8);
        RG2L = signedlong(RG2W);
        if (RG2L == 0)
          skip00450 = true;
        else
        {
          RG2L = RG1L;
          RG2Bhi ^= RG1Bhi;
          RG2Blo &= 0x1;
          RG2L = signedlong(RG2W);
          if (RG2L != 0)
            skip00450 = true;
        }
      }
    }

    if (skip00450)
    {
      bool skip00451 = false;
      //m_1D4DE:
      RG1W &= 0x2;
      RG1L = signedlong(RG1W);
      if (RG1L == 0 || RG4W == wcon(0x0))
        return vl_08;
      RG1L = signedlong(RG4W);
      if (RG1L == lcon(0x7) || RG1L == lcon(0x4))
        return vl_08;
      RG1L = argl0;
      RG1W &= lcon(0x100);
      RG1L = unsignedlong(RG1W);
      if (RG1L == 0)
        skip00451 = true;
      else
      {
        RG2L = signedlong(vw_0c);
        RG4L = signedlong(word_at(RG5p));
        RG1L = DM2_FIND_LADDAR_AROUND(signedlong(word_at(RG61p)), RG4L, RG2L, UPCAST(unk*, ADRESSOF(unk*, &xp_00)));
        if (RG1W < 0)
          skip00451 = true;
      }

      if (skip00451)
      {
        //m_1D532:
        RG1L = argl0;
        RG1W &= lcon(0x10);
        RG1L = unsignedlong(RG1W);
        if (RG1L == 0)
          return vl_08;
        if (vw_0c != lcon(0xffffffff))
          return vl_08;
        flag2 = true;
      }
    }
  }
  else
  {
    RG4L = argl0;
    RG4W &= lcon(0x100);
    RG4L = unsignedlong(RG4W);
    if (RG4L == 0)
      return vl_08;
    RG1W &= 0x4;
    RG1L = signedlong(RG1W);
    if (RG1L == 0)
      //m_1D488:
      flag1 = RG3W == wcon(0x1);
    else
      flag1 = RG3W == wcon(0xffff);
    //m_1D48C:
    RG1Blo = flag1 ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == 0)
      return vl_08;
  }
  bool skip00454 = false;
  //m_1D556:
  RG1L = DM_LOCATE_OTHER_LEVEL(signedlong(vw_04), signedlong(vw_0c), UPCAST(i16, RG61p), UPCAST(i16, RG5p), NULL);
  vl_08 = RG1L;
  if (RG1W < 0 || !flag2)
    return vl_08;
  RG1L = signedlong(RG1W);
  DM2_CHANGE_CURRENT_MAP_TO(RG1L);
  RG4L = signedlong(word_at(RG5p));
  RG1L = dm2_GET_TILE_VALUE(signedlong(word_at(RG61p)), RG4L);
  RG4Blo = RG1Blo;
  RG1L = unsignedlong(RG4Blo);
  RG4L = unsignedlong(RG4Blo) >> 5;
  RG4L = unsignedlong(RG4Blo);
  if (RG4L == 2)
  {
    RG4L = RG1L;
    RG4Bhi ^= RG1Bhi;
    RG4Blo &= lcon(0x8);
    RG4L = signedlong(RG4W);
    if (RG4L != 0)
    {
      RG1W &= 0x1;
      RG1L = signedlong(RG1W);
      if (RG1L == 0)
        skip00454 = true;
    }
  }

  if (!skip00454)
    //m_1D5BE:
    vl_08 = lcon(0xffffffff);

  //m_1D5C6:
  DM2_CHANGE_CURRENT_MAP_TO(signedlong(vw_04));
  return vl_08;
}

void DM2_query_29ee_18eb(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  ddat.v1e0b6e = RG1W;
  ddat.v1e0b5e = RG1W;
  ddat.v1e0b68 = RG1W;
  ddat.v1e0b70 = RG4W;
  ddat.v1e0b5c = RG4W;
  ddat.v1e0b6a = RG4W;
  ddat.v1e0b64 = RG2W;
  RG2L = signedlong(RG2W);
  RG3L = lcon(0xffffffff);
  RG4P = DOWNCAST(i16, &ddat.v1e0b6a);
  RG1L = DM2_query_19f0_124b(&ddat.v1e0b68, RG4P16, RG2L, RG3L, lcon(0x110));
  ddat.v1e0b60 = RG1W;
  RG2L = signedlong(ddat.v1e0b64);
  RG3L = 1;
  RG4P = DOWNCAST(i16, &ddat.v1e0b5c);
  RG1L = DM2_query_19f0_124b(&ddat.v1e0b5e, RG4P16, RG2L, RG3L, lcon(0x108));
  ddat.v1e0b66 = RG1W;
}

i16 DM2_QUERY_CREATURE_BLIT_RECTI(i16 n, t_rotate rotate, i16 wb)
{
  return DM2_ROTATE_5x5_POS(wb, rotate) + 25 * n + 5000;
}

i32 DM2_query_0cee_3275(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG4;

  RG4L = unsignedlong(RG1Blo);
  return mkul(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(14, RG4Blo, 11, 16));
}

i32 DM2_QUERY_DOOR_STRENGTH(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG5l;

  RG5l = unsignedlong(RG1Blo);
  RG3L = lcon(0x11);
  RG2L = lcon(0xb);
  RG4L = RG5l;
  RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(14, RG4Blo, 11, 17);
  if (RG1W != 0)
    return mkl(RG1W);
  return DM2_query_0cee_3275(RG5l) == 0 ? 6 : 1;
}

i32 DM2_IS_CREATURE_ALLOWED_ON_LEVEL(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  RG2L = RG4L;
  RG4L = unsignedlong(RG1W);
  RG1W = DM2_QUERY_CREATURE_AI_SPEC_FLAGS(RG4L);
  if ((RG1Bhi & bcon(0x40)) != 0)
    return lcon(0x1);
  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(RG4L);
  RG3Blo = RG1Blo;
  RG1L = unsignedlong(RG2W);
  RG4L = 16 * RG1L;
  RG2P = ddat.v1e03c8 + RG4L;
  RG4L = 4 * RG1L;
  RG1P = DOWNCAST(t_tile*, mapdat.tmpmap) + RG4L;
  RG4W = word_at(RG2P, lcon(0x8));
  RG4L <<= bcon(0x5);
  _RG4.ushiftr11();
  RG4L = 4 * unsignedlong(RG4W);
  RG1P = pointer_at(RG1P) + RG4L;
  RG4W = word_at(RG2P, lcon(0x8));
  _RG4.ushiftr11();
  RG4L = unsignedlong(RG4W);
  RG1P = pointer_at(RG1P) + RG4L;
  RG4P = RG1P + lcon(0x1);
  RG1W = word_at(RG2P, lcon(0xc));
  RG1L <<= bcon(0x8);
  RG1UW >>= bcon(0xc);
  //m_FB36:
  for (;;)
  {
    if (RG1W <= 0)
      return 0;
    //m_FB2E:
    RG2P = RG4P;
    RG4P++;
    if (RG3Blo == byte_at(RG2P))
      return lcon(0x1);
    RG1L--;
  }
  FOREND
}

// belongs to DM2_events_3C1E5
i32 DM2_query_0cee_319e(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;

  RG1Blo = DM2_QUERY_CLS2_FROM_RECORD(unsignedlong(RG1W));
  if (RG1Blo != lcon(0xffffffff))
    return mkul(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(9, RG1Blo, 11, 11));
  return 0;
}
