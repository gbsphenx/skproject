#include "fileio.h"
#include "util.h"
#include "startend.h" // THROW...
#include "../driver.h"
#include "dm2data.h"
#include "c_alloc.h"
#include "c_dballoc.h"
#include "c_gfx_misc.h"
#include "c_sound.h"
#include "c_gfx_decode.h"

#include "dm2debug.h"	// SPX

// needed externs:
extern void SKW_RAISE_SYSERR(x16 eaxw);
extern t_text* SKW_FORMAT_SKSTR(const t_text* eaxt, t_text* edxt);

// critical externals: (higher level?)
extern ui16 SKW_482b_015c(x16 eaxw); // c_sound.h
extern bool SKW_2066_03e0(x16 eaxw);

/*
  provides:

void DM2_2636_03d4(void)
bool SKW_3e74_55f9(ui32 eaxul, x16* edxpw)
t_gfxdata* SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(x8 eaxb, x8 ebxb, x8 edxb)
void SKW_READ_GRAPHICS_STRUCTURE(void)
void SKW_LOAD_DYN4(s_hex6* eaxps, x16 edxw)
t_gfxdata* SKW_3e74_5817(x16 eaxw)
void SKW_FREE_PICT_ENTRY(t_gfxdata* eaxpg);
void SKW_LOAD_GDAT_ENTRY_DATA_TO(xb eaxb, x8 ebxb, x8 ecxb, x8 edxb, x8* argpb0)
t_gfxdata* SKW_ALLOC_PICT_BUFF(x16 eaxw, x16 ebxw, x8 ecxb, x16 edxw)
void SKW_FREE_PICT_BUFF(t_gfxdata* eaxpg)
void SKW_3e74_583a(x16 eaxw)
x16 DM2_QUERY_GDAT_ENTRY_DATA_INDEX(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb)
x32 SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb)
x8* SKW_QUERY_GDAT_ENTRY_DATA_PTR(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb)
x8* SKW_QUERY_GDAT_ENTRY_DATA_BUFF(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb)
bool SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb)
x8 SKW_3e74_2439(x8 eaxb, x8 edxb)
x16 SKW_QUERY_GDAT_PICT_OFFSET(x8 eaxb, x8 ebxb, x8 edxb)
t_palette* SKW_QUERY_GDAT_IMAGE_LOCALPAL(x8 eaxb, x8 ebxb, x8 edxb)
t_gfxdata* SKW_ALLOC_NEW_PICT(x16 eaxw, x16 ebxw, x8 ecxb, x16 edxw)
x16 SKW_3e74_5888(void)
void SKW_3e74_58a3(x16 eaxw)
void SKW_3e74_58bf(x8 eaxb, x8 ebxb, x8 edxb)
void SKW_3e74_5992(x8 eaxb, x8 ebxb, x8 edxb)
void DM2_GRAPHICS_DATA_OPEN(void)
void DM2_GRAPHICS_DATA_CLOSE(void)
*/

void DM2_GRAPHICS_DATA_OPEN(void)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("DM2_GRAPHICS_DATA_OPEN");
  if (++ddata.gdatfileopencounter == con(0x1))
  {
    ddata.gdatfilehandle = DM2_FILE_OPEN(SKW_FORMAT_SKSTR(ddata.gdatfilename1, NULL));
    if (ddata.gdatfilehandle < con(0x0))
      SKW_RAISE_SYSERR(con(0x29));	// con(0x29) : SYSTEM_ERROR__MISSING_GRAPHICS
    if (!ddata.gdatfiletype1 && ddata.gdatfiletype2)
    {
      ddata.gdatxfilehandle = DM2_FILE_OPEN(SKW_FORMAT_SKSTR(ddata.gdatfilename2, NULL));
      if (ddata.gdatxfilehandle < con(0x0))
        SKW_RAISE_SYSERR(con(0x1f));	// con(0x1f) : SYSTEM_ERROR__MISSING_GRAPHICS_2
    }
  }
SPX_DEBUG_POP;
}

void DM2_GRAPHICS_DATA_CLOSE(void)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("DM2_GRAPHICS_DATA_CLOSE");
  x16 wordrg4 = ddata.gdatfileopencounter;
  bool flag = --wordrg4 != con(0x0);
  ddata.gdatfileopencounter = wordrg4;
  if (!flag)
  {
    DM2_FILE_CLOSE(ddata.gdatfilehandle);
    if (!ddata.gdatfiletype1 && ddata.gdatfiletype2)
      DM2_FILE_CLOSE(ddata.gdatxfilehandle);
  }
SPX_DEBUG_POP;
}

void DM2_2636_03d4(void)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("DM2_2636_03d4");
  ddata.v1e0988 = con(0x0);

  for (x16 n = 0; n < 2; n++)
    ddata.vp_1e097c[n] = UPCAST(t_text, DM2_ALLOC_FREEPOOL_MEMORY(con(0x80)));

  userinterrupt(con(0x29), con(0x0));
  t_text* ptrrg1 = *UPCAST(t_text*, ddata.drvinterfaceptr); // was conv_pointerc_at
  if (*ptrrg1 != NULLBYTE)
    ddata.datafoldername = ptrrg1;
SPX_DEBUG_POP;
}


//static u_bbwlong* DM2_QUERY_GDAT_ENTRYPTR(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb)
static u_bbwlong* DM2_QUERY_GDAT_ENTRYPTR(x8 eaxb, ui16 ebxb, ui16 ecxb, ui16 edxb) // SPX fix to avoid negative values
{
LOGX(("%40s: C%02d=I%02X=E%02X=T%03d [%04s]\n", "DM2_QUERY_GDAT_ENTRYPTR of ", eaxb, edxb, ecxb, ebxb, SPX_STR_DATATYPE(ebxb) ));
	fprintf(logfile, "DM2_QUERY_GDAT_ENTRYPTR: %03d=%03d=%03d=%03d\n", eaxb, edxb, ecxb, ebxb); // SPX: add log
  if (unsignedlong(eaxb) > unsignedlong(dm2_dballochandler.gdattable.w_0c))
    return NULL;
  x16* ptrrg1 = &dm2_dballochandler.gdattable.w_table4[eaxb];
  x16 wordrg3 = ptrrg1[0];
  if (unsignedlong(ebxb) >= (unsignedlong(ptrrg1[1]) - unsignedlong(wordrg3)))	// SPX: if (glbGDatEntries.pw0[iCategory +1] - si < iType)
    return NULL;
  wordrg3 += unsignedword(ebxb); // TODO: unexpected extension happens here!
  x16* ptrrg12 = &dm2_dballochandler.gdattable.w_table5[wordrg3];
  u_bbwlong* ptrrg7 = &dm2_dballochandler.gdattable.l_table4[ptrrg12[0] - 1];	// &glbGDatEntries.pv8[di -1];
  x16 wordrg52 = ptrrg12[1] - ptrrg12[0] + 1;	// glbGDatEntries.pw4[si +1] -di +1;
  x16 wordrg32 = con(0x0);
  for(;;)
  {
    x16 wordrg4 = (wordrg52 + wordrg32) >> con(0x1); // TODO: check if shift or div
    if (wordrg4 == wordrg32)
      return NULL;
    u_bbwlong* ptrrg11 = &ptrrg7[wordrg4];
    x16 wordrg6 = unsignedword(ptrrg11->u.b_00) - unsignedword(edxb);
    if (wordrg6 == con(0x0))
    {
      wordrg6 = unsignedword(ptrrg11->u.b_01) - unsignedword(ecxb); // TODO: critical, first one was not extended
      if (wordrg6 == con(0x0))
        return ptrrg11;
    }
    if (wordrg6 <= con(0x0))
      wordrg32 = wordrg4;
    else
      wordrg52 = wordrg4;
  }
}

//x16 DM2_QUERY_GDAT_ENTRY_DATA_INDEX(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb)
x16 DM2_QUERY_GDAT_ENTRY_DATA_INDEX(x8 eaxb, ui16 ebxb, ui16 ecxb, ui16 edxb)	// SPX fix to get positive numbers for item select such as 0xFE = 254 instead of -2 (which leads to crash)
{
LOGX(("%40s: C%02d=I%02X=S%02X=T%03d [%04s]\n", "QUERY_GDAT_ENTRY_DATA_LENGTH of ", eaxb, edxb, ecxb, ebxb, SPX_STR_DATATYPE(ebxb) ));
	fprintf(logfile, "%40s: C%02d=I%02X=E%02X=T%03d\n", "DM2_QUERY_GDAT_ENTRY_DATA_INDEX", eaxb, edxb, ecxb, ebxb); // SPX: add log
  u_bbwlong* ptrrg4 = DM2_QUERY_GDAT_ENTRYPTR(eaxb, ebxb, ecxb, edxb);
  if (ptrrg4 == NULL)
  {
    if (ebxb == con(0xb) || ebxb == con(0xc))
      return con(0x0);
    return con(0xffff);
  }
  if (ebxb == con(0xb) || ebxb == con(0xc))
    return ptrrg4->u.w_02;
  return ptrrg4->u.w_02 & con(0x7fff);
}

static bool DM2_IS_CLS1_CRITICAL_FOR_LOAD(x8 eaxb)
{
  if (eaxb == con(0x1b)) return true;
  if (eaxb == con(0x6)) return true;
  if (eaxb == con(0x5)) return true;
  return false;
}

// TODO: make sure that both local variables don't overlap
// while adressed by index
// SPX: some clarification over variables using SKWin 9821 code for better understanding
//static void DM2_GRAPHICS_DATA_READ(x32 eaxl, xxx* ebxp, x32 edxl)
static void DM2_GRAPHICS_DATA_READ(x32 iDataItemLocation, xxx* xDataBuffer, x32 iDataItemSize)
{
  x32 vla_00[0x4];
  x16 vwa_10[0x2];

  x32 longrg5 = (ui32) iDataItemLocation;	// eaxl
  vwa_10[0x0] = ddata.gdatfilehandle;
  vla_00[0x0] = longrg5;
  vla_00[0x2] = iDataItemSize;	// edxl
  vla_00[0x3] = con(0x0);
  if (ddata.gdatfiletype2)
  {
    vwa_10[0x1] = ddata.gdatxfilehandle;
    longrg5 -= ddata.longgdatfilesize;
    vla_00[0x1] = longrg5;
    if (longrg5 < con(0x0))
    {
      vla_00[0x3] = longrg5 + iDataItemSize;	// vla_00[0x3] = longrg5 + edxl;
      if (vla_00[0x3] <= con(0x0))
        vla_00[0x3] = con(0x0);
      else
        vla_00[0x1] = con(0x0);
    }
    else
      vla_00[0x3] = iDataItemSize;	// vla_00[0x3] = edxl;
    vla_00[0x2] -= vla_00[0x3];
  }

  x16 wordrg3 = con(0x0);
  // SPX: longrg5 would be the part file number of GRAPHICSx.dat part
  int iGraphicsFilePart = 0;	/// 0 is for GRAPHICS.DAT, 1 is for GRAPHIC2.DAT
  //while ((longrg5 = signedlong(wordrg3)) < con(0x2))
  while ((iGraphicsFilePart = signedlong(wordrg3)) < con(0x2))
  {
    x32 longrg2 = 4 * iGraphicsFilePart;
    //if (vla_00[0x2 + longrg2 / 4] != con(0x0))
	if (vla_00[0x2 + iGraphicsFilePart] != con(0x0))	// Check size
    {
      longrg5 = 2 * iGraphicsFilePart;
      bool noclose = false;
	  //if (DM2_FILE_SEEK(signedlong(vwa_10[longrg5 / 2]), vla_00[longrg2 / 4]))
      if (DM2_FILE_SEEK(signedlong(vwa_10[iGraphicsFilePart]), vla_00[iGraphicsFilePart]))
      {
		  //if (DM2_FILE_READ(vwa_10[longrg5 / 2], ebxp, vla_00[0x2 + longrg2 / 4]))
        if (DM2_FILE_READ(vwa_10[iGraphicsFilePart], xDataBuffer, vla_00[0x2 + iGraphicsFilePart]))
          noclose = true;
      }
      if (!noclose)
      {
        DM2_GRAPHICS_DATA_CLOSE();
        if (wordrg3 != con(0x0))
          SKW_RAISE_SYSERR(con(0x20));	// SYSTEM_ERROR__CORRUPTED_GRAPHICS_2
        else
          SKW_RAISE_SYSERR(con(0x2e));	// SYSTEM_ERROR__CORRUPTED_GRAPHICS
      }
      //ebxp += vla_00[0x2 + signedlong(wordrg3)];
	  xDataBuffer += vla_00[0x2 + signedlong(wordrg3)];
    }
    wordrg3++;
  }
}

static x32 SKW_QUERY_GDAT_RAW_DATA_FILE_POS(x16 eaxw)
{
  x32 longrg2 = ddata.longv1e0a3c;
  ui16 uwordrg4 = con(0x0);
  if (UI16CAST(eaxw) >= dm2_dballochandler.v1d6781)
  {
    uwordrg4 = dm2_dballochandler.v1d6781;
    longrg2 += dm2_dballochandler.v1d6783;
  }
  while (uwordrg4 < UI16CAST(eaxw))
    longrg2 += dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(uwordrg4++) & con(0xffff);
  dm2_dballochandler.v1d6781 = eaxw;
  dm2_dballochandler.v1d6783 = longrg2 - ddata.longv1e0a3c;
  return longrg2;
}

static void SKW_LOAD_GDAT_RAW_DATA(x16 eaxw, x8* edxpb)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("SKW_LOAD_GDAT_RAW_DATA");
LOGX(("SKW_LOAD_GDAT_RAW_DATA loads GDAT item %04d to ptr %08x\n", eaxw, edxpb ));
	fprintf(logfile, "LOAD_GDAT_RAW_DATA: %04d => %08x\n", eaxw, edxpb); // SPX: add log
  x32 longrg5;
  x32 longrg6;
  x32 vl_00;
  //x16 vw_04;
  x32 vw_04;

  DM2_GRAPHICS_DATA_OPEN();
  // SPX: when eaxw is negative, that does no good thing, then check also for positive value, else go to the default (which was only = 0 before that)
  //if (eaxw != con(0x0))
  if (eaxw != con(0x0) && eaxw > 0)	// SPX: add > condition
  {
    vl_00 = SKW_QUERY_GDAT_RAW_DATA_FILE_POS(eaxw);
    longrg6 = unsignedlong(CUTX16(dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(eaxw)));
  }
  else
  {
    longrg6 = ddata.longv1e0a44;			// data length ?
    vl_00 = ddata.longv1e0a3c - longrg6;	// file pos ?
  }
  vw_04 = CUTX16((vl_00 - jul_shl32(vl_00, con(0xa))) >> con(0xa));
  longrg5 = signedlong((x16)vw_04) << con(0xa);
  // SPX: rewrite from SkWin : try to avoid negative values
vw_04 = vl_00 / 0x400;
longrg5 = ((ui32)vw_04) * 0x400;

  while (longrg6 > con(0x0))
  {
    if (vw_04 != dm2_dballochandler.v1d6773 || dm2_dballochandler.v1e0abe != con(0x0))
    {
      dm2_dballochandler.v1d6773 = vw_04;
      x32 longrg42 = ddata.longv1e0a40 - longrg5;	// GDAT size - positional 1024 block where data starts
      if (longrg42 > con(0x400))	// Skwin : 0x400 = TEMP_BUFF_SIZE
        longrg42 = con(0x400);	// longrg42 = buffer size, limited to 1024
      DM2_GRAPHICS_DATA_READ(longrg5, dm2_dballochandler.filebuffer, longrg42);
      dm2_dballochandler.v1e0abe = con(0x0);
    }
    x32 longrg4 = vl_00 - longrg5;
    x32 longrg3 = con(0x400) - longrg4;
    if (longrg6 < longrg3)
      longrg3 = longrg6;
//    SKW_COPY_MEMORY(dm2_dballochandler.filebuffer + longrg4, longrg3, edxpb);
//    SKW_COPY_MEMORY(dm2_dballochandler.filebuffer, longrg4 + longrg3, edxpb);	// SPX: above seems wrong ?
	// SPX: copy mem copies XXX bytes from the end of the buffer (1024) to target + YYY (where YYY is 1024 - XXX)
    SKW_COPY_MEMORY(dm2_dballochandler.filebuffer + longrg4, longrg3, edxpb);	// SPX: above seems wrong ?
    longrg6 -= longrg3;
    vl_00 += longrg3;
    edxpb += longrg3;
    vw_04++;
    longrg5 += con(0x400);
  }
  DM2_GRAPHICS_DATA_CLOSE();
SPX_DEBUG_POP;
}

bool SKW_3e74_55f9(ui32 eaxul, x16* edxpw)
{
  x16 vw_00;

  if (ddata.longmallocx != dm2_dballochandler.malloci)
    R_2E581();
  if (!SKW_3e74_5420(eaxul, &vw_00))
  {
    *edxpw = SKW_3e74_54a1(eaxul, vw_00);
    return false;
  }
  else
  {
    x16 wordrg1 = dm2_dballochandler.w_table3[vw_00];
    *edxpw = wordrg1;
    SKW_3e74_48c9(DM2_ALLOC_CPX_GET_WTABLE_ENTRY(wordrg1 | con(0x8000)));
    return true;
  }
}

t_gfxdata* SKW_3e74_5817(x16 eaxw)
{
  return UPCAST(t_gfxdata, DOWNCAST(s_xmalloc, SKW_3e74_48c9(DM2_ALLOC_CPX_GET_WTABLE_ENTRY(eaxw | con(0x8000))) + 1)); // gets adress of next structentry
}

void SKW_3e74_583a(x16 eaxw)
{
  x16 wordrg1 = DM2_ALLOC_CPX_GET_WTABLE_ENTRY(eaxw | con(0x8000));
  if (wordrg1 != con(0xffffffff))
    R_2F41B(wordrg1);
}

static xxx* SKW_QUERY_GDAT_DYN_BUFF(x16 eaxw, bool ebx, x16* edxpw)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("SKW_QUERY_GDAT_DYN_BUFF");
SPX_DEBUG_MESSAGE_INFO("SKW_QUERY_GDAT_DYN_BUFF: %04d, %d, %08x\n", eaxw, ebx, edxpw);
  s_malloctail* ptrrg2;
  if (!dm2_dballochandler.gfxalloc_done)
  {
    x32 longrg3 = unsignedlong(eaxw);
    x32 longrg6 = unsignedlong(CUTX16(dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(CUTX16(longrg3)))) + con(0x6);
    if (!ebx)
      ptrrg2 = UPCAST(s_malloctail, DM2_ALLOC_HIBIGPOOL_MEMORY(longrg6) + con(0x6));
    else
      ptrrg2 = UPCAST(s_malloctail, DM2_ALLOC_LOBIGPOOL_MEMORY(longrg6) + con(0x6));
    SKW_LOAD_GDAT_RAW_DATA(CUTX16(longrg3), DOWNCAST(s_malloctail, ptrrg2));
    if (!ebx)
      (ptrrg2 - 1)->w4 = con(0x2); // TODO: neg. offset
    else
      (ptrrg2 - 1)->w4 = con(0x0); // TODO: neg. offset
    (ptrrg2 - 1)->l0 = longrg6; // TODO: neg. offset
  }
  else if (SKW_3e74_55f9(unsignedlong(eaxw) - con(0x20000), edxpw))
  {
    ptrrg2 = UPCAST(s_malloctail, SKW_3e74_5817(*edxpw));
    if (ebx)
      SKW_3e74_583a(*edxpw);
  }
  else
  {
    ptrrg2 = UPCAST(s_malloctail, SKW_ALLOC_CPXHEAP_MEM(*edxpw, unsignedlong(CUTX16(dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(eaxw))))); // TODO: upcast from s_xmalloc
    SKW_LOAD_GDAT_RAW_DATA(eaxw, DOWNCAST(s_malloctail, ptrrg2));
    if (!ebx)
      SKW_3e74_585a(*edxpw, false);
  }
SPX_DEBUG_POP
  return DOWNCAST(s_malloctail, ptrrg2);
}

//x8* SKW_QUERY_GDAT_ENTRY_DATA_PTR(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb)
x8* SKW_QUERY_GDAT_ENTRY_DATA_PTR(x8 eaxb, ui16 ebxb, ui16 ecxb, ui16 edxb) // SPX fix to avoid negative values
{
SPX_DEBUG_PUSH_FUNCTION_INFO("SKW_QUERY_GDAT_ENTRY_DATA_PTR");
//SPX_DEBUG_MESSAGE_INFO("SKW_QUERY_GDAT_ENTRY_DATA_PTR: %04d=%04d=%04d=%04d\n", eaxb, ebxb, ecxb, edxb);
LOGX(("%40s: C%02d=I%02X=S%02X=T%03d [%04s]\n", "SKW_QUERY_GDAT_ENTRY_DATA_PTR ", eaxb, edxb, ecxb, ebxb, SPX_STR_DATATYPE(ebxb) ));
  x16 vw_00;

  x16 wordrg2 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, ebxb, ecxb, edxb);
  if (ebxb == con(0x1) && !DM2_IS_CLS1_CRITICAL_FOR_LOAD(eaxb) && (wordrg2 == con(0xffffffff) || dm2_ulp.islen(wordrg2)))
    wordrg2 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x15), con(0x1), con(0xfe), con(0xfe));
  if (dm2_ulp.islen(unsignedlong(wordrg2)))
  {
    if (DM2_IS_CLS1_CRITICAL_FOR_LOAD(eaxb))
	{
SPX_DEBUG_POP
      return SKW_QUERY_GDAT_DYN_BUFF(wordrg2, false, &vw_00);
	}
  }
SPX_DEBUG_POP
  return UPCAST(x8, dm2_ulp.getp(wordrg2));
}

static x32 SKW_QUERY_GDAT_ENTRY_VALUE(x16 eaxw, x16 edxw)
{
  x8* bptrrg1 = dm2_dballochandler.ptr1e09a8 + unsignedlong(eaxw) * unsignedlong(dm2_dballochandler.v1e09ae) + signedlong(dm2_dballochandler.v1e09b2[edxw]);
  x16 wordrg4 = unsignedword(dm2_dballochandler.v1e09c0[edxw]);
  x32 longrg2 = con(0x0);
  while (wordrg4-- > con(0x0))
  {
    longrg2 <<= con(0x8);
    longrg2 += unsignedlong(*bptrrg1++);
  }
  return longrg2;
}

static bool SKW_QUERY_NEXT_GDAT_ENTRY(s_gdat* eaxp)
{
LOGX(("SKW_QUERY_NEXT_GDAT_ENTRY from SGDAT ptr %08X // intern p_10 : %08X\n", eaxp, eaxp->p_10));
  x8 byterg3hi;
  x16 wordrg6;
  u_bbwlong* p_04;
  x16 vw_08;
  x8 vb_0c;
  x8 vb_14;
  x8 vb_18;

  x8 vb_10 = eaxp->s_04.u.s_00.b_03;
  x8 byterg3lo = eaxp->s_04.b_05;
  x32 longrg4 = ((unsignedlong(eaxp->s_04.u.s_00.w_00) & con(0xffff7fff)) == con(0x1)) ? 1 : 0;
  bool flag = longrg4 == con(0x0);
  x32 longrg5 = longrg4;
  if (flag)
    vb_0c = vb_10;
  else
  {
    vb_0c = eaxp->s_0a.u.s_00.b_03;
    vb_18 = eaxp->s_0a.b_05;
  }
  x32 vl_00 = eaxp->l_00;
  if (vl_00 != con(0x0))
  {
    eaxp->l_00 = con(0x0);
    eaxp->b_14 = eaxp->s_04.u.s_00.b_02;
    if (eaxp->s_04.u.s_00.b_02 != con(0xffffffff))
    {
      if (longrg5 == con(0x0))
        eaxp->b_15 = eaxp->s_04.u.s_00.b_02;
      else
      {
        eaxp->b_15 = eaxp->s_0a.u.s_00.b_02;
        if (eaxp->s_0a.u.s_00.b_02 == con(0xffffffff))
          eaxp->b_15 = CUTX8(dm2_dballochandler.gdattable.w_0c);
      }
    }
    else
    {
      eaxp->b_14 = con(0x0);
      eaxp->b_15 = CUTX8(dm2_dballochandler.gdattable.w_0c);
    }
    if (UI8CAST(eaxp->b_14) > UI8CAST(eaxp->b_15)) goto M_true;
    if (unsignedlong(eaxp->b_15) > unsignedlong(dm2_dballochandler.gdattable.w_0c)) goto M_true;
  }

  x16 wordrg2;
  u_bbwlong* ptrrg4b;

  for(;;) // M_2C7DC: // outest loop!
  {
    x16* ptrrg4a = dm2_dballochandler.gdattable.w_table4;
    x16* ptrrg2 = &ptrrg4a[eaxp->b_14];
    x16 wordrg4 = ptrrg2[0];
    if (wordrg4 != ptrrg2[1])
    {
      bool innerloop = false;
      bool innerbypass = false;
      bool brkpsloop = false;
      while (!brkpsloop) // pseudoloop
      {
        if (innerloop || vl_00 != con(0x0))
        {
          if (!innerloop)
          {
            x8 byterg2lo = CUTX8(ptrrg2[1]) - CUTLX8(wordrg4) - 1;
            x8 byterg2hi = eaxp->s_04.b_04;
            eaxp->b_16 = byterg2hi;
            bool hopp = false;
            if (byterg2hi != con(0xffffffff))
            {
              if (UI8CAST(byterg2hi) > UI8CAST(byterg2lo))
                break; // goto M_2C9AD;
              if (longrg5 == con(0x0))
              {
                eaxp->b_17 = byterg2hi;
                hopp = true;
              }
              else
              {
                byterg2hi = eaxp->s_0a.b_04;
                eaxp->b_17 = byterg2hi;
                if (byterg2hi != con(0xffffffff))
                  hopp = true;
              }
            }
            else
              eaxp->b_16 = con(0x0);
            if (!hopp)
              eaxp->b_17 = byterg2lo;
            // M_2C84F:
            wordrg4 += unsignedword(eaxp->b_16);
            wordrg2 = dm2_dballochandler.gdattable.w_table5[wordrg4];
            eaxp->w_1a = wordrg2;
            eaxp->w_18 = ++wordrg4;
            wordrg4 = dm2_dballochandler.gdattable.w_table5[wordrg4];
            eaxp->w_1c = wordrg4;
            u_bbwlong* ptrrg7 = &dm2_dballochandler.gdattable.l_table4[wordrg2]; // unsigned has to stay here!
            wordrg4 = 4 * unsignedlong(wordrg2); // TEST
            eaxp->p_10 = ptrrg7;
            ptrrg4b = ptrrg7; // TODO: Note: ptrrg7 dies
          }

          // M_2C899:
          for(;;)
          {
            if (!innerloop)
            {
              if (UI16CAST(wordrg2) >= UI16CAST(eaxp->w_1c))
                innerbypass = true;
              else if (vb_10 != con(0xffffffff))
              {
                if (byterg3lo != con(0xffffffff))
                  vb_14 = byterg3lo;
                else
                  vb_14 = con(0x0);
                ptrrg4b = dm2_dballochandler.gdattable.l_table4 - 1;
                p_04 = ptrrg4b;
                wordrg6 = wordrg2;
                wordrg4 = eaxp->w_1c + 1;
                vw_08 = wordrg4;

                // M_2C8D7:
                for(;;)
                {
                  wordrg2 = CUTX16((unsignedlong(wordrg6) + unsignedlong(vw_08)) / 2);
                  ptrrg4b = &p_04[wordrg2];
                  if (wordrg2 != wordrg6)
                  {
                    byterg3hi = vb_10;
                    if (byterg3hi != ptrrg4b->u.b_00)
                    {
                      if (UI8CAST(byterg3hi) >= UI8CAST(ptrrg4b->u.b_00))
                        wordrg6 = wordrg2;
                      else
                        vw_08 = wordrg2;
                      continue;
                    }
                    byterg3hi = vb_14;
                    if (UI8CAST(byterg3hi) < UI8CAST(ptrrg4b->u.b_01))
                    {
                      vw_08 = wordrg2;
                      continue;
                    }
                  }
                  else
                  {
                    if (UI16CAST(wordrg2) >= UI16CAST(eaxp->w_1c))
                      innerbypass = true; // goto M_2C979;
                    else
                    {
                      wordrg2++;
                      ptrrg4b++;
                      byterg3hi = vb_0c;
                      if (UI8CAST(byterg3hi) < UI8CAST(ptrrg4b->u.b_00))
                        innerbypass = true; // goto M_2C979;
                    }
                  }
                  break;
                }
                wordrg2--;
              }
            }

            // M_2C934:
            innerloop = false;
            if (!innerbypass)
            {
              for(;;)
              {
                byterg3hi = ptrrg4b->u.b_00;
                if (UI8CAST(byterg3hi) > UI8CAST(vb_0c))
                  break; // goto M_2C979;
                if (longrg5 == con(0x0))
                {
                  if (byterg3lo == con(0xffffffff) || byterg3lo == ptrrg4b->u.b_01) goto M_false;
                }
                else
                {
                  if (byterg3lo == con(0xffffffff)) goto M_false;
                  if (UI8CAST(byterg3lo) <= UI8CAST(ptrrg4b->u.b_01))
                  {
                    byterg3hi = vb_18;
                    if (UI8CAST(byterg3hi) >= UI8CAST(ptrrg4b->u.b_01)) goto M_false;
                  }
                }
                // M_2C96D:
                if (UI16CAST(++wordrg2) >= UI16CAST(eaxp->w_1c))
                  break; // goto M_2C979;
                ptrrg4b++;
              }
            }

            // M_2C979:
            innerloop = false;
            innerbypass = false;
            x8 byterg2 = eaxp->b_16 + 1;
            eaxp->b_16 = byterg2;
            if (UI8CAST(byterg2) > UI8CAST(eaxp->b_17))
            {
              brkpsloop = true;
              break; // goto M_2C9AD;
            }
            wordrg2 = eaxp->w_1c;
            wordrg6 = eaxp->w_18 + 1;
            eaxp->w_18 = wordrg6;
            x32 tmp7 = 2 * unsignedlong(wordrg6); // TODO: check obsolete
            wordrg6 = dm2_dballochandler.gdattable.w_table5[wordrg6];
            eaxp->w_1c = wordrg6;
          } // loop M_2C899;
        } // endif
        if (!brkpsloop)
        {
          wordrg2 = eaxp->w_1a + 1;
          ptrrg4b = eaxp->p_10 + 1;
          innerloop = true;
          if (UI16CAST(wordrg2) >= UI16CAST(eaxp->w_1c))
            innerbypass = true;
        }
      } // pseudoloop
    } // endif

    // M_2C9AD:
    x8 byterg4 = eaxp->b_14 + 1;
    eaxp->b_14 = byterg4;
    if (UI8CAST(byterg4) > UI8CAST(eaxp->b_15)) goto M_true;
    vl_00 = con(0x1);
  } // goto M_2C7DC;

M_false:
  eaxp->w_1a = wordrg2;
  eaxp->p_10 = ptrrg4b;
  return true;

M_true:
  eaxp->p_10 = NULL;
  eaxp->b_14 = con(0xffffffff);
  eaxp->b_16 = con(0xffffffff);
  return false;
}

// eaxb = category / ebxb = #item / ecxb = datatype / edxb = #sub-item
//x32 SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb)
x32 SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(x8 eaxb, ui16 ebxb, ui16 ecxb, ui16 edxb)	// SPX fix to avoid negative values
{
LOGX(("QUERY_GDAT_ENTRY_DATA_LENGTH of C%02d=I%02X=S%02X=T%03d [%04s] = %08X\n", eaxb, ebxb, edxb, ecxb, SPX_STR_DATATYPE(ecxb), dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, ebxb, ecxb, edxb) & con(0xffff)) ));
fprintf(logfile, "QUERY_GDAT_ENTRY_DATA_LENGTH: %03d=%03d=%03d=%03d\n", eaxb, ebxb, ecxb, edxb); // SPX: add log
  return dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, ebxb, ecxb, edxb) & con(0xffff));
}

// ebxw is either BIGPOOL_LO or FREEPOOL
t_gfxdata* SKW_ALLOC_PICT_BUFF(x16 eaxw, x16 ebxw, x8 ecxb, x16 edxw)
{
  x32 vl_00;

  if (ecxb != con(0x4))
    vl_00 = unsignedlong(eaxw);
  else
    vl_00 = ((unsignedlong(eaxw) + 1) & con(0xfffffffe)) >> con(0x1);
  x32 vl_04 = (vl_00 & con(0xffff)) * unsignedlong(edxw);
  xxx* ptrrg1 = DM2_ALLOC_MEMORY_RAM((vl_04 & con(0xffff)) + con(0x6), MASK_8, ebxw) + con(0x6);
  s_pregfxdata* ptr = UPCAST(s_pregfxdata, ptrrg1) - 1; // TODO: attention, neg. offset, structbackstep
  ptr->b_00 = ecxb;
  ptr->w_02 = eaxw;
  ptr->w_04 = edxw;
  return UPCAST(t_gfxdata, ptrrg1);
}


void SKW_FREE_PICT_BUFF(t_gfxdata* eaxpg)
{
  s_pregfxdata* ptr = UPCAST(s_pregfxdata, eaxpg) - 1; // TODO: neg. offset, struct backstep
  x16 wordrg4;
  if (ptr->b_00 != con(0x4))
    wordrg4 = ptr->w_02;
  else
    wordrg4 = ((ptr->w_02 + 1) & con(0xfffffffe)) >> con(0x1);
  DM2_DEALLOC_LOBIGPOOL(unsignedlong(CUTX16(unsignedlong(ptr->w_04) * unsignedlong(wordrg4))) + con(0x6));
}

static ui32 SKW_3e74_53ea(void)
{
  ui32 ulongrg2;
  x16 vw_00;

  do
  {
    ulongrg2 = unsignedlong(dm2_dballochandler.v1d6793++) | con(0xffff0000);
  } while (SKW_3e74_5420(ulongrg2, &vw_00));
  return ulongrg2;
}

x16 SKW_3e74_5888(void)
{
  x16 vw_00;

  SKW_3e74_55f9(SKW_3e74_53ea(), &vw_00);
  return vw_00;
}

void SKW_3e74_58a3(x16 eaxw)
{
  SKW_3e74_585a(eaxw, false);
  SKW_3e74_28de(eaxw | con(0x8000));
}

void SKW_3e74_58bf(x8 eaxb, x8 ebxb, x8 edxb)
{
  x16 wordrg4 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, con(0x1), ebxb, edxb);
  if (wordrg4 == con(0xffffffff) || dm2_ulp.islen(wordrg4))
    wordrg4 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x15), con(0x1), con(0xfe), con(0xfe));
  if (wordrg4 != con(0xffffffff))
  {
    x16 wordrg1 = DM2_ALLOC_CPX_GET_WTABLE_ENTRY(wordrg4);
    if (wordrg1 != con(0xffffffff))
      R_2F41B(wordrg1);
  }
}

void SKW_3e74_5992(x8 eaxb, x8 ebxb, x8 edxb)
{
  x16 wordrg4 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, con(0x1), ebxb, edxb);
  if (wordrg4 == con(0xffffffff) || dm2_ulp.islen(wordrg4))
    wordrg4 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x15), con(0x1), con(0xfe), con(0xfe));
  if (wordrg4 != con(0xffffffff))
  {
    x16 wordrg1 = DM2_ALLOC_CPX_GET_WTABLE_ENTRY(wordrg4);
    if (wordrg1 != con(0xffffffff))
      R_2E68E(wordrg1, false);
  }
}

t_gfxdata* SKW_ALLOC_NEW_PICT(x16 eaxw, x16 ebxw, x8 ecxb, x16 edxw)
{
  x16 wordrg4;
  if (ecxb != con(0x4))
    wordrg4 = edxw;
  else
    wordrg4 = ((edxw + 1) & con(0xfffffffe)) >> con(0x1);
  xxx* ptrrg1 = DOWNCAST(s_xmalloc, SKW_ALLOC_CPXHEAP_MEM(eaxw, (unsignedlong(ebxw) * unsignedlong(wordrg4)) & con(0xffff)));
  s_pregfxdata* ptr = UPCAST(s_pregfxdata, ptrrg1) - 1; // TODO: neg. offset, struct back
  ptr->b_00 = ecxb;
  ptr->w_02 = edxw;
  ptr->w_04 = ebxw;
  return UPCAST(t_gfxdata, ptrrg1);
}

//x8 SKW_3e74_2439(x8 eaxb, x8 edxb)
ui16 SKW_3e74_2439(x8 eaxb, x8 edxb) // returns the number of max items within a category, must return positive
{
  if (unsignedlong(eaxb) > unsignedlong(dm2_dballochandler.gdattable.w_0c))
    return con(0x0);
  x32 longrg2 = unsignedlong(dm2_dballochandler.gdattable.w_table4[eaxb]);
//  x32 longrg3 = (signedlong(dm2_dballochandler.gdattable.w_table4[eaxb + 1]) & con(0xffff)) - longrg2;
  x32 longrg3 = (unsignedlong(dm2_dballochandler.gdattable.w_table4[eaxb + 1]) & con(0xffff)) - longrg2;
  x32 longrg1 = unsignedlong(edxb);
  if (longrg1 >= longrg3)
    return con(0x0);
  longrg2 += longrg1;
  return (ui8) (dm2_dballochandler.gdattable.l_table4[dm2_dballochandler.gdattable.w_table5[longrg2 + 1] - 1].u.b_00);	// just for easier debugger step
  //return dm2_dballochandler.gdattable.l_table4[dm2_dballochandler.gdattable.w_table5[longrg2 + con(0x1)] - con(0x1)].u.b_00;
}

/*
U8 SkWinCore::_3e74_2439(X8 cls1, X8 cls4)
{
	ENTER(0);
	if (cls1 > U8(glbGDatEntries.w12))
		return 0;
	X16 si = glbGDatEntries.pw0[cls1];
	if (glbGDatEntries.pw0[cls1 +1] -si <= cls4)
		return 0;
	return glbGDatEntries.pv8[U32(glbGDatEntries.pw4[cls4 +si +1]) -1].cls2;
}
*/

static x16 R_2BAD4(x16 eaxw)
{
  return CUTX16((unsignedlong(eaxw) >> con(0x8)) + (unsignedlong(eaxw) << con(0x8)));
}

static s_hex6* R_2D8AD(s_hmm* eaxp, x32 edxl)
{
  s_hex6* ptrrg2 = UPCAST(s_hex6, DOWNCAST(s_hmm, eaxp->ps_08) - edxl);
  eaxp->ps_08 = ptrrg2;
  return ptrrg2;
}

static xxx* R_2D8BA(s_hmm* eaxp, x32 ebxl, xxx* edxp)
{
  s_hex6* ptrrg3 = R_2D8AD(eaxp, ebxl);
  SKW_COPY_MEMORY(DOWNCAST(s_hmm, edxp) - con(0x2), ebxl, DOWNCAST(s_hex6, ptrrg3)); // TODO: neg. offset
  return &ptrrg3->u.s_00.b_02;
}

static void R_2D802(s_pppw* eaxp)
{
  s_pppw* ptrrg7 = eaxp;
  if (eaxp->pw0 == NULL)
    return;
  x16* wptrrg3 = eaxp->pw0;
  x16* wptrrg5 = eaxp->pw0 - 1;
  while (wptrrg5 > ptrrg7->pw8)
  {
    x16 wordrg6 = *wptrrg5;
    x32 longrg2 = ((((dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(wordrg6 & con(0x7fff)) & con(0xffff)) + 1) & con(0xfffffffe)) + con(0x4)) / 2;
    wptrrg5 -= longrg2;
    if ((wordrg6 & con(0x8000)) != con(0x0))
      continue;
    wptrrg3 -= longrg2;
    x16* wptrrg1 = wptrrg5 + 1;
    if (wptrrg1 == wptrrg3)
      continue;
    SKW_COPY_MEMORY(DOWNCAST(x16, wptrrg1), 2 * longrg2, DOWNCAST(x16, wptrrg3));
    dm2_ulp.setp(wordrg6, DOWNCAST(x16, wptrrg3 + 1));
  }
  ptrrg7->pw8 = wptrrg3;
}

void SKW_FREE_PICT_ENTRY(t_gfxdata* eaxpg)
{
  if (dm2_dballochandler.gfxalloc_done || eaxpg == NULL)
    return;

  struct s_tmp
  {
    x32 l0; // unused
    x16 w4;
    x16 w6;
    x8 b8;
    x8 b9; // unused
    x16 wa;
    x16 wc;
    // maybe longer
  };

  s_tmp* p = UPCAST(s_tmp, eaxpg - con(0xe));
  // pointer in the beginning,
  // word at 4, word at 6, byte at 8, word at a, word at c

  if (   p->wa == (p->w4 & con(0x3ff))
      && p->wc == (p->w6 & con(0x3ff))
     )
  {
    s_malloctail* ptrrg11 = UPCAST(s_malloctail, p + con(0x4)); // that is eaxp - 0xa

    if ((ptrrg11->l0 >> con(0x1a)) == con(0xffffffe0))
    {
      DM2_DEALLOC_BIGPOOL_STRUCT_BEFORE(ptrrg11);
      return;
    }
  }

  xxx** ptrrg1 = UPCAST(xxx*, &dm2_dballochandler.ptr1e0a5c);
  xxx** ptrrg2 = UPCAST(xxx*, p);
  xxx** ptrrg3;

  //--- SPX : commented these 3 lines because of crash
  //while (ptrrg2 != (ptrrg3 = UPCAST(xxx*, *ptrrg1)))
  //  ptrrg1 = UPCAST(xxx*, *ptrrg1);
  //*UPCAST(xxx*, ptrrg1) = UPCAST(xxx, *ptrrg3);

  x32 longrg1 = (SKW_CALC_IMAGE_BYTE_LENGTH(eaxpg) & con(0xffff)) + con(0xe);
  if (p->b8 == con(0x4))
    longrg1 += con(0x10);
  if (p->w6 != con(0x0))
    DM2_DEALLOC_HIBIGPOOL(longrg1);
  else
    DM2_DEALLOC_LOBIGPOOL(longrg1);
}

static void SKW_BUILD_GDAT_ENTRY_DATA(GDATEntries* eaxs, xxx* ebxp, FPTR_b_W edxf)
{
  x32 longrg6;
  x32 vl_04;
  ui8 vub_10;
  x16* pw_14;
  x8 vb_1c; // inc8

  pw_14 = UPCAST(x16, DM2_ALLOC_HIBIGPOOL_MEMORY(con(0x3a0), true));
  eaxs->w_0c = con(0x0);
  eaxs->w_10 = con(0x0);

  x16* wptrrg7 = pw_14; // TODO: ptrrg7 read only from here
  for (ui16 uwordrg21 = con(0x0); uwordrg21 < UI16CAST(dm2_dballochandler.v1e09ac); uwordrg21++)
  {
    x32 longrg52 = unsignedlong(uwordrg21);
    if (INDCALL(edxf)(CUTX16(longrg52)))
    {
      vb_1c = CUTLX8(SKW_QUERY_GDAT_ENTRY_VALUE(CUTX16(longrg52), con(0x0)));
      x8 byterg1 = CUTLX8(SKW_QUERY_GDAT_ENTRY_VALUE(CUTX16(longrg52), con(0x2)));
      x8 byterg4lo = byterg1;
      x8 byterg4hi = vb_1c;
      if (UI8CAST(byterg4hi) <= UI8CAST(con(0x1c)) && UI8CAST(byterg1) <= UI8CAST(con(0xe)))
      {
        eaxs->w_10++;
        if (unsignedlong(byterg4hi) > unsignedlong(eaxs->w_0c))
          eaxs->w_0c = unsignedword(byterg4hi);
        longrg6 = unsignedlong(vb_1c);
        wptrrg7[16 * longrg6 + unsignedlong(byterg4lo)]++;
        longrg6 <<= con(0x5);
        x16* wptrrg5 = &wptrrg7[longrg6 / 2];
        longrg6 = unsignedlong(wptrrg5[con(0xf)]);
        if (unsignedlong(byterg4lo) >= longrg6)
          wptrrg5[con(0xf)] = unsignedword(byterg4lo) + 1;
      }
    }
  }

  x16 wordrg13 = con(0x0);
  ui32 ulongrg4;
  for (vb_1c = con(0x0); (ulongrg4 = unsignedlong(vb_1c)) <= unsignedlong(eaxs->w_0c); vb_1c++)
  {
    ulongrg4 <<= con(0x5);
    wordrg13 += pw_14[(ulongrg4 + con(0x1e)) / 2];
  }
  eaxs->w_0e = wordrg13;
  eaxs->w_12 = con(0x0);
  eaxs->w_14 = con(0x0);
  longrg6 = con(0x0);
  x16 vw_18 = con(0x0);

  ui16 uwordrg5;
  while ((uwordrg5 = vw_18) < UI16CAST(con(0x7)))
  {
    if (uwordrg5 > con(0x4))
    {
      x32 longrg53 = unsignedlong(vw_18);
if (longrg53 >= 7)
  int stop=1; // TODO
      if (ebxp[longrg53] != con(0x0))
      {
        eaxs->warr_16[longrg53] = eaxs->w_12;
        eaxs->barr_24[longrg53] = dm2_dballochandler.v1e09c0[longrg53];
        eaxs->w_12 += unsignedword(dm2_dballochandler.v1e09c0[longrg53]);
        eaxs->w_14++;
      }
    }
    else
    {
      x32 longrg52 = unsignedlong(vw_18);
if (longrg52 >= 8)
  int stop=1; // TODO
      eaxs->barr_24[longrg52] = dm2_dballochandler.v1d6787[longrg52];
      eaxs->w_12 += unsignedword(dm2_dballochandler.v1d6787[longrg52]);
      eaxs->warr_16[longrg52] = con(0xffffffff);
      if (UI8CAST(dm2_dballochandler.v1d6787[longrg52]) > UI8CAST(con(0x0)))
        eaxs->w_14++;
    }
    vw_18++;
  }
  dm2_dballochandler.gdattable.w_table4 = UPCAST(x16, DM2_ALLOC_FREEPOOL_MEMORY(sizeof(x16) * unsignedlong(eaxs->w_0c) + con(0x4)));
  dm2_dballochandler.gdattable.w_table5 = UPCAST(x16, DM2_ALLOC_FREEPOOL_MEMORY(sizeof(x16) * unsignedlong(eaxs->w_0e) + con(0x2)));
  dm2_dballochandler.gdattable.l_table4 = UPCAST(u_bbwlong, DM2_ALLOC_FREEPOOL_MEMORY(sizeof(u_bbwlong) * unsignedlong(eaxs->w_10)));

  x16 wordrg1 = con(0x0);
  x16 wordrg2 = con(0x0);

  x32 longrg5;
  x32 longrg41;
  for (vb_1c = con(0x0); (longrg41 = unsignedlong(vb_1c)) <= (longrg5 = unsignedlong(eaxs->w_0c)); vb_1c++)
  {
    eaxs->w_table4[longrg41] = wordrg1;
    longrg41 <<= con(0x5);
    vw_18 = pw_14[(longrg41 + con(0x1e)) / 2];

    for (x8 byterg4 = con(0x0); (longrg5 = unsignedlong(byterg4)) < unsignedlong(vw_18); byterg4++)
    {
      longrg6 = 2 * unsignedlong(wordrg1++);
      vl_04 = longrg6;
      eaxs->w_table5[vl_04 / 2] = wordrg2; // TODO: avoid / 2
      longrg6 = unsignedlong(vb_1c) << con(0x4);
      wordrg2 += pw_14[longrg5 + longrg6];
    }
  }
  eaxs->w_table4[longrg5 + 1] = wordrg1;
  eaxs->w_table5[eaxs->w_0e] = eaxs->w_10;
  DM2_ZERO_MEMORY(DOWNCAST(xxx, pw_14), con(0x3a0));
  DM2_ZERO_MEMORY(DOWNCAST(u_bbwlong, eaxs->l_table4), unsignedlong(eaxs->w_10) << con(0x2));

  for (ui16 uwordrg2 = con(0x0); uwordrg2 < UI16CAST(dm2_dballochandler.v1e09ac); uwordrg2++)
  {
    if (INDCALL(edxf)(uwordrg2))
    {
      vub_10 = CUTLX8(SKW_QUERY_GDAT_ENTRY_VALUE(uwordrg2, con(0x0)));
      x8 byterg1 = CUTLX8(SKW_QUERY_GDAT_ENTRY_VALUE(uwordrg2, con(0x2)));
      if (vub_10 <= con(0x1c) && UI8CAST(byterg1) <= UI8CAST(con(0xe)))
      {
        x32 longrg4 = unsignedlong(vub_10);
        longrg6 = 16 * longrg4;
        x32 longrg7 = unsignedlong(byterg1);
        longrg6 += longrg7;
        x16* ptrrg6w = &pw_14[longrg6];
        x16 wordrg1 = *ptrrg6w;
        vl_04 = 2 * longrg4;
        *ptrrg6w = wordrg1 + 1;
        x16* p1 = eaxs->w_table4;
        x16* p2 = eaxs->w_table5;
        u_bbwlong* p3 = eaxs->l_table4;
        x16 v1 = p1[longrg4];
        ui32 v2 = unsignedlong(v1) + longrg7;
        x16 v3 = p2[v2];
        ui32 v4 = unsignedlong(v3) + unsignedlong(wordrg1);
        u_bbwlong* ptrrg6 = &p3[v4];
        ptrrg6->u.b_00 = CUTLX8(SKW_QUERY_GDAT_ENTRY_VALUE(uwordrg2, con(0x1)));
        ptrrg6->u.b_01 = CUTLX8(SKW_QUERY_GDAT_ENTRY_VALUE(uwordrg2, con(0x3)));
        ptrrg6->u.w_02 = CUTX16(SKW_QUERY_GDAT_ENTRY_VALUE(uwordrg2, con(0x4)));
        if (unsignedword(CUTLX8(SKW_QUERY_GDAT_ENTRY_VALUE(uwordrg2, con(0x6)))) == con(0x1))
          ptrrg6->l_00 |= con(0x80000000);
      }
    }
  }
  DM2_DEALLOC_HIBIGPOOL(con(0x3a0));
}

// stacksize was 0x4
//void SKW_LOAD_GDAT_ENTRY_DATA_TO(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb, x8* argpb0)
void SKW_LOAD_GDAT_ENTRY_DATA_TO(x8 eaxb, ui16 ebxb, ui16 ecxb, ui16 edxb, x8* argpb0)	// SPX fix to avoid negative values
{
// SPX: add log
LOGX(("%40s: C%02d=I%02X=S%02X=T%03d [%04s] => %08x\n", "SKW_LOAD_GDAT_ENTRY_DATA_TO ", eaxb, edxb, ecxb, ebxb, SPX_STR_DATATYPE(ebxb), argpb0));
	fprintf(logfile, "LOAD_GDAT_ENTRY_DATA_TO: %03d=%03d=%03d=%03d => %08x\n", eaxb, edxb, ecxb, ebxb, argpb0);
  SKW_LOAD_GDAT_RAW_DATA(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, ebxb, ecxb, edxb) & con(0xffff), argpb0);
}

static void SKW_LOAD_GDAT_ENTRIES(void)
{
  ui16 uwordrg5 = con(0x0);
  while (uwordrg5 < UI16CAST(dm2_dballochandler.v1e09ac))
  {
    x32 longrg2 = unsignedlong(uwordrg5);
    if (CUTLX8(SKW_QUERY_GDAT_ENTRY_VALUE(CUTX16(longrg2), con(0x6))) == con(0xffffffff))
    {
      x8 byterg1 = CUTLX8(SKW_QUERY_GDAT_ENTRY_VALUE(CUTX16(longrg2), con(0x2)));
      if (byterg1 != con(0xb) && byterg1 != con(0xc))
      {
        x32 longrg3 = unsignedlong(CUTX16(SKW_QUERY_GDAT_ENTRY_VALUE(CUTX16(longrg2), con(0x4))));
        x32 longrg7 = 4 * longrg3; // TODO: check obsolete?
        if (dm2_ulp.islen(longrg3))
        {
          x16 wordrg6 = CUTX16(dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(CUTX16(longrg3)));
          x16* wptrrg1 = UPCAST(x16, DM2_ALLOC_FREEPOOL_MEMORY(unsignedlong(wordrg6) + con(0x2)));
          *wptrrg1++ = wordrg6;
          dm2_ulp.setp(longrg3, DOWNCAST(x16, wptrrg1));
          SKW_LOAD_GDAT_RAW_DATA(CUTX16(longrg3), DOWNCAST(x16, wptrrg1));
        }
      }
    }
    uwordrg5++;
  }
}

static bool SKW_3e74_2162(x16 eaxw)
{
  SKW_QUERY_GDAT_ENTRY_VALUE(eaxw, con(0x2));
  x8 byterg1 = CUTLX8(SKW_QUERY_GDAT_ENTRY_VALUE(eaxw, con(0x5))) & con(0xfffffff0);
  if (byterg1 == 0 || byterg1 == dm2_dballochandler.v1d6770)
    return true;
  return false;
}

static void SKW_47eb_00a4(xxx* eaxp)
{
  struct s_pwwp* ptr = UPCAST(s_pwwp, eaxp);

  ptr->u.p8 = dm2_dballochandler.v1dff38;
  dm2_dballochandler.v1dff38 = UPCAST(s_pwwp, eaxp);

  struct s_bbbbw
  {
    x8 b0;
    x8 b1;
    x8 b2;
    x8 b3;
    x16 w4;
  };

  s_bbbbw* ptrrg4 = UPCAST(s_bbbbw, ptr->p0 - 6);

  if (ptrrg4->w4 == con(0x1))
    return;
  if (ptrrg4->w4 != con(0x0))
    ptrrg4->w4 = con(0x1);
  else
  {
    if ((ptrrg4->b3 & con(0x1)) == con(0x0))
      return;
    ptrrg4->b3 &= con(0xfffffffe);
  }
  x16 wordrg4 = ptr->w4;
  xxx* p = ptr->p0;
  while (--wordrg4 != con(0xffffffff))
    *p++ ^= con(0xffffff80);
}

static void SKW_LOAD_ENT1(void)
{
  x32 longrg7 = ddata.longv1e0a44;
  x8* bptrrg1 = UPCAST(x8, DM2_ALLOC_HIBIGPOOL_MEMORY(longrg7));
  dm2_dballochandler.ptr1e09a8 = bptrrg1;
  SKW_LOAD_GDAT_RAW_DATA(con(0x0), bptrrg1);
  x16 wordrg5 = *UPCAST(x16, dm2_dballochandler.ptr1e09a8);
  if (wordrg5 != con(0xffff8001))
  {
    if (R_2BAD4(wordrg5) != con(0xffff8001))
      SKW_RAISE_SYSERR(con(0x1e));
  }
  x16* wptrrg11 = UPCAST(x16, dm2_dballochandler.ptr1e09a8);
  x16 wordrg4 = wptrrg11[1];
  dm2_dballochandler.v1e09ac = wordrg4;
  dm2_dballochandler.v1e09b0 = wptrrg11[2];
  if (wordrg5 != con(0xffff8001))
  {
    dm2_dballochandler.v1e09ac = R_2BAD4(wordrg4);
    dm2_dballochandler.v1e09b0 = R_2BAD4(dm2_dballochandler.v1e09b0);
  }
  x8* bptrrg6 = dm2_dballochandler.ptr1e09a8 + con(0x6);
  DM2_FILL_I16TABLE(dm2_dballochandler.v1e09b2, con(0xffffffff), con(0x7));
  dm2_dballochandler.v1e09ae = con(0x0);

  x16 wordrg2;
  for (ui16 uwordrg5 = con(0x0); uwordrg5 < (wordrg2 = dm2_dballochandler.v1e09b0); uwordrg5++)
  {
    wordrg2 = con(0x0);
    x8* bptrrg4;
    x32 longrg1;
    while ((longrg1 = signedlong(wordrg2)) < con(0x7))
    {
      bptrrg4 = bptrrg6 + 2 * unsignedlong(uwordrg5);
      if (dm2_dballochandler.tidspfg[longrg1] == bptrrg4[0])
        break;
      wordrg2++;
    }
    dm2_dballochandler.v1e09c0[longrg1] = bptrrg4[1];
    dm2_dballochandler.v1e09b2[longrg1] = dm2_dballochandler.v1e09ae;
    dm2_dballochandler.v1e09ae += unsignedword(bptrrg6[2 * unsignedlong(uwordrg5) + con(0x1)]);
  }
  bptrrg6 += 2 * unsignedlong(wordrg2);
  dm2_dballochandler.ptr1e09a8 = bptrrg6;
  SKW_LOAD_GDAT_ENTRIES();
  SKW_BUILD_GDAT_ENTRY_DATA(&dm2_dballochandler.gdattable, &dm2_dballochandler.v1d6787[5], SKW_3e74_2162);
  DM2_DEALLOC_HIBIGPOOL(longrg7);
  dm2_dballochandler.ptr1e09a8 = NULL;
}

static void SKW_482b_0684(void)
{
  x16 vw_04;

  for (ui32 vl_00 = 0; vl_00 < unsignedlong(dm2sound.v1d2698); vl_00++)
  {
    s_ssound* sptrrg5 = UPCAST(s_ssound, &dm2sound.xsndptr2[vl_00]);
    if (sptrrg5->w_05 == con(0xffffffff))
    {
      x16 wordrg4 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(sptrrg5->b_02, con(0x2), sptrrg5->b_04, sptrrg5->b_03);
      x16 wordrg1 = SKW_482b_015c(wordrg4);
      if (wordrg1 == con(0x0))
      {
        if (dm2sound.v1d269a >= dm2sound.v1e0ace)
          return;
        sptrrg5->w_05 = wordrg4;
        sptrrg5->w_00 = dm2sound.v1d269a;
        if (dm2sound.v1d269c == con(0x0))
          vw_04 = con(0x2);
        else
          vw_04 = con(0x6);
        s_pww* ptr = UPCAST(s_pww, dm2sound.sndptr4 + 16 * unsignedlong(sptrrg5->w_00));
        x16* ptrrg6 = UPCAST(x16, SKW_QUERY_GDAT_ENTRY_DATA_PTR(sptrrg5->b_02, con(0x2), sptrrg5->b_04, sptrrg5->b_03));
        ptr->p0 = DOWNCAST(x16, ptrrg6 + vw_04 / 2);
        wordrg4 = CUTX16(SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(sptrrg5->b_02, con(0x2), sptrrg5->b_04, sptrrg5->b_03) - unsignedlong(vw_04));
        ptr->w4 = wordrg4;

        x16 wordrg11;
        if (dm2sound.v1d269c == con(0x0))
          wordrg11 = con(0x157c);
        else
          wordrg11 = *ptrrg6;
        ptr->w6 = wordrg11;
        SKW_47eb_00a4(DOWNCAST(s_pww, ptr));
        dm2sound.v1d269a++;
      }
      else
      {
        sptrrg5->w_00 = UPCAST(s_ssound, &dm2sound.xsndptr2[wordrg1 - 1])->w_00;
        sptrrg5->w_05 = wordrg4;
      }
    }
  }
}

static void R_4ADA5(void)
{
  dm2_dballochandler.v1e149c = UPCAST(xxx, DM2_ALLOC_FREEPOOL_MEMORY(con(0x3aa1)));
  dm2_dballochandler.v1e14a0 = UPCAST(xxx, DM2_ALLOC_FREEPOOL_MEMORY(con(0x138b)));
}

static x32 R_2D07D(void)
{
unsigned int iForIndex = 0;	// SPX: for debug
  s_gdat vs_00;

//--- SPX: starts with CCCCCCCC in debug mode, then maybe needs a better zero initialization of this structure
	vs_00.p_10 = NULL;
	vs_00.b_14 = 0;
	vs_00.b_15 = 0;
	vs_00.b_16 = 0;
	vs_00.b_17 = 0;
	vs_00.w_18 = 0;
	vs_00.w_1a = 0;
	vs_00.w_1c = 0;
	vs_00.w_1e = 0;
	vs_00.s_0a.u.l_00 = 0;
	vs_00.s_04.u.l_00 = 0;
	vs_00.l_00 = 0;
//--- SPX


  vs_00.l_00 = con(0x1);
  vs_00.s_04.u.s_00.w_00 = con(0x0);
  vs_00.s_04.u.s_00.b_02 = con(0xffffffff);
  vs_00.s_04.u.s_00.b_03 = con(0xffffffff);
  vs_00.s_04.b_04 = con(0x3);
  vs_00.s_04.b_05 = con(0xffffffff);
  for(;;)
  {
LOGX(( "R_2D07D> %04d : RawEntry (ptr) %08X \n", iForIndex, vs_00.p_10));
    if (!SKW_QUERY_NEXT_GDAT_ENTRY(&vs_00))
      return con(0x0);
    x16 wordrg1 = CUTX16(dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(vs_00.p_10->u.w_02));
    if (UI16CAST(wordrg1) > dm2_dballochandler.v1e0ad4)
	{
      dm2_dballochandler.v1e0ad4 = wordrg1;
		LOGX(( "R_2D07D> v1e0ad4 takes value %04d \n", wordrg1 ));
	}

iForIndex++;
  }
}

// function may return pointers of different types
//x8* SKW_QUERY_GDAT_ENTRY_DATA_BUFF(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb)
x8* SKW_QUERY_GDAT_ENTRY_DATA_BUFF(x8 eaxb, ui16 ebxb, ui16 ecxb, ui16 edxb)	// SPX fix to get positive numbers for item select such as 0xFE = 254 instead of -2 (which can lead to crash)
{
LOGX(("%40s: C%02d=I%02X=S%02X=T%03d [%04s]\n", "SKW_QUERY_GDAT_ENTRY_DATA_BUFF", eaxb, ebxb, edxb, ecxb, SPX_STR_DATATYPE(ecxb) ));
  x16 vw_00;

  x16 wordrg1 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, ebxb, ecxb, edxb);
  if (wordrg1 == con(0xffffffff))
    return NULL;
  if (dm2_ulp.islen(unsignedlong(wordrg1)))
  {
    if (!DM2_IS_CLS1_CRITICAL_FOR_LOAD(eaxb))
      return NULL;
    return SKW_QUERY_GDAT_DYN_BUFF(wordrg1, false, &vw_00);
  }
  return UPCAST(x8, dm2_ulp.getp(unsignedlong(wordrg1)));
}

//bool SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb)
bool SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(x8 eaxb, ui16 ebxb, ui16 ecxb, ui16 edxb)	// SPX fix to get positive numbers for item select such as 0xFE = 254 instead of -2 (which can lead to crash)
{
LOGX(("%40s: C%02d=I%02X=S%02X=T%03d [%04s]\n", "SKW_QUERY_GDAT_ENTRY_IF_LOADABLE", eaxb, ebxb, edxb, ecxb, SPX_STR_DATATYPE(ecxb) ));
  u_bbwlong* ptrrg4 = DM2_QUERY_GDAT_ENTRYPTR(eaxb, ebxb, ecxb, edxb);
  if (ptrrg4 == NULL)
    return false;
  if (ebxb == con(0xb) || ebxb == con(0xc))
    return true;
  if (!dm2_ulp.islen(ptrrg4->u.w_02 & con(0x7fff)))
    return true;
  if (!DM2_IS_CLS1_CRITICAL_FOR_LOAD(eaxb))
    return false;
  return true;
}

static void SKW_3e74_24b8(void)
{
  x16 wordrg3; // TODO: Danger - uninitialized possible?
  x16* ptrrg5; // TODO: Danger - uninitialized possible?
  s_gdat vs_00;

  x16 wordrg6 = con(0x0);
  while (unsignedlong(wordrg6) < con(0x2))
  {
    if (wordrg6 != 0)
    {
      if (dm2_dballochandler.v1e0ad6 == con(0x0))
        return;
    }
    vs_00.l_00 = con(0x1);
    vs_00.s_04.u.s_00.w_00 = con(0x0);
    vs_00.s_04.u.s_00.b_02 = con(0xffffffff);
    vs_00.s_04.u.s_00.b_03 = con(0xffffffff);
    vs_00.s_04.b_04 = con(0x2);
    vs_00.s_04.b_05 = con(0xffffffff);
    if (wordrg6 != 0)
    {
      ptrrg5 = UPCAST(x16, DM2_ALLOC_LOBIGPOOL_MEMORY(2 * unsignedlong(dm2_dballochandler.v1e0ad6)));
      wordrg3 = con(0x0);
    }

    while (SKW_QUERY_NEXT_GDAT_ENTRY(&vs_00))
    {
      x16 wordrg4 = vs_00.p_10->u.w_02;
      if (wordrg6 != con(0x0))
      {
        bool immcont = false;
        x16 wordrg1 = con(0x0);
        while (wordrg1 < UI16CAST(wordrg3))
        {
          if (wordrg4 == ptrrg5[wordrg1])
          {
            immcont = true;
            break;
          }
          wordrg1++;
        }
        if (!immcont)
        {
          ptrrg5[wordrg3++] = wordrg4;
          dm2sound.v1e0ace++;
        }
      }
      else
      {
        dm2_dballochandler.v1e0ad6++;
        x16 wordrg1 = CUTX16(dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(wordrg4));
        if (UI16CAST(wordrg1) > dm2_dballochandler.v1e0ad8)
          dm2_dballochandler.v1e0ad8 = wordrg1;
      }
    }

    if (wordrg6 != 0)
      DM2_DEALLOC_LOBIGPOOL(2 * dm2_dballochandler.v1e0ad6);
    wordrg6++;
  }
}

//x16 SKW_QUERY_GDAT_PICT_OFFSET(x8 eaxb, x8 ebxb, x8 edxb)
x16 SKW_QUERY_GDAT_PICT_OFFSET(x8 eaxb, ui16 ebxb, ui16 edxb)			//  SPX fix to avoid negative values
{
  u_bbwlong* ptrrg1 = DM2_QUERY_GDAT_ENTRYPTR(eaxb, con(0x1), ebxb, edxb);
  if (ptrrg1 == NULL)
    return con(0x0);
  s_grrr* sptrrg5;
  bool vl_00 = false;
  if (!dm2_ulp.islen(unsignedlong(ptrrg1->u.w_02)))
    sptrrg5 = UPCAST(s_grrr, dm2_ulp.getp(unsignedlong(ptrrg1->u.w_02)));
  else
  {
    vl_00 = true;
    sptrrg5 = UPCAST(s_grrr, SKW_QUERY_GDAT_ENTRY_DATA_PTR(eaxb, con(0x1), ebxb, edxb));
  }
  x16 wordrg1 = sptrrg5->u.w0 >> con(0xa);
  x32 wordrg4 = wordrg1;
  if (signedlong(wordrg1) != con(0xffffffe0))
  {
    bool nowrt5 = false;
    x16 wordrg11 = CUTX16(sptrrg5->u.l0 >> con(0x1a));
    if (wordrg11 != dm2_dballochandler.v1e0acc)
    {
      if (wordrg11 != con(0xffffffe0))
        nowrt5 = true;
    }
    else
      wordrg4 = signedword(sptrrg5->b4);
    x8 byterg1 = CUTX8(wordrg11);
    if (!nowrt5)
      byterg1 = sptrrg5->b5;
    wordrg4 <<= con(0x8);
    wordrg4 += unsignedword(byterg1);
  }
  else
    wordrg4 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, con(0xc), ebxb, edxb);
  if (vl_00)
    DM2_DEALLOC_BIGPOOL_STRUCT_BEFORE(UPCAST(s_malloctail, sptrrg5));
  return wordrg4;
}

//t_palette* SKW_QUERY_GDAT_IMAGE_LOCALPAL(x8 eaxb, x8 ebxb, x8 edxb)
t_palette* SKW_QUERY_GDAT_IMAGE_LOCALPAL(x8 eaxb, ui16 ebxb, ui16 edxb)	//  SPX fix to avoid negative values
{
  x16 vw_00;

  x16 wordval6 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, con(0x1), ebxb, edxb);
  if (wordval6 == con(0xffffffff))
    goto M_fin;
  s_grrr* sptrrg4;
  if (!dm2_ulp.islen(unsignedlong(wordval6)))
    sptrrg4 = UPCAST(s_grrr, dm2_ulp.getp(unsignedlong(wordval6)));
  else
  {
    if (!DM2_IS_CLS1_CRITICAL_FOR_LOAD(eaxb))
      goto M_fin;
    sptrrg4 = UPCAST(s_grrr, SKW_QUERY_GDAT_DYN_BUFF(wordval6, false, &vw_00));
  }
  if ((sptrrg4->u.l0 >> con(0x1a)) == con(0xffffffe0))
  {
    if (sptrrg4->b4 != con(0x4))
      return NULL;
  }
  if ((sptrrg4->u.l0 >> con(0x1a)) == signedlong(dm2_dballochandler.v1e0acc))
    return NULL;
  return UPCAST(t_palette, DOWNCAST(s_grrr, sptrrg4) + (dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(wordval6) & con(0xffff)) - con(0x10));

M_fin:
  return SKW_QUERY_GDAT_IMAGE_LOCALPAL(con(0x15), con(0xfe), con(0xfe));
}

static x16 SKW_TRACK_UNDERLAY(ui16 eaxuw)
{
  struct s_uww
  {
    ui16 w0;
    x16 w2;
  };

  x16 wordrg2 = con(0x0);
  x16 wordrg6 = ddata.v1e0ac2 + 1;
  for(;;)
  {
    x16 wordrg1 = (wordrg6 + wordrg2) / 2; // TODO: check: was it >>1 ?
    if (wordrg1 == wordrg2)
      return con(0xffff);
    xxx* ptrrg4 = ddata.p1e0a64 + 4 * signedlong(wordrg1);
    s_uww* ptr = UPCAST(s_uww, ptrrg4 - 4); // TODO: neg. offset
    if (eaxuw >= ptr->w0)
    {
      if (eaxuw > ptr->w0)
        wordrg2 = wordrg1;
      else
        return ptr->w2;
    }
    else
      wordrg6 = wordrg1;
  }
}

void SKW_READ_GRAPHICS_STRUCTURE(void)
{
	SPX_DEBUG_PUSH_FUNCTION_INFO("SKW_READ_GRAPHICS_STRUCTURE");
LOGX(("SKW_READ_GRAPHICS_STRUCTURE\n"));
  x32 vl_00;

// SPX: variable declarations moved here
  
  ui16 wordrg1;
  x16* ptrrg11;
  x32 longrg6;
  x16* ptrrg3;
  x32 longrg1;
  x16 wordrg12;
  x16 wordrg41;
  x32 longrg12;
 

  ddata.gdatfileopencounter = con(0x0);
  DM2_GRAPHICS_DATA_OPEN();

  // first get a long (two words) for version and number of entries
  if (!DM2_FILE_READ(ddata.gdatfilehandle, DOWNCAST(x32, &vl_00), sizeof(x32)))
    goto M_errexit;
  if ((CUTX16(vl_00) & con(0x8000)) == con(0x0))
    goto M_errexit;
  ddata.gdatversionlo = CUTX16(vl_00) & con(0x7fff);
  ddata.gdatentries = I16CAST(vl_00 >> 16);
  if (ddata.gdatversionlo != con(0x4) && ddata.gdatversionlo != con(0x5) && ddata.gdatversionlo != con(0x2))
    goto M_errexit;

  // initialize the pointer/length-table
  dm2_ulp.init();

  // create w_table2 and fill it with 0xffff
   ptrrg11 = UPCAST(x16, DM2_ALLOC_FREEPOOL_MEMORY(sizeof(x16) * unsignedlong(ddata.gdatentries)));
  dm2_dballochandler.w_table2 = ptrrg11;
  DM2_FILL_I16TABLE(ptrrg11, con(0xffffffff), unsignedlong(ddata.gdatentries));

  // create a temporary word-table (ptrrg3)
   longrg6 = sizeof(x16) * unsignedlong(ddata.gdatentries);
   ptrrg3 = UPCAST(x16, DM2_ALLOC_LOBIGPOOL_MEMORY(longrg6));
  ddata.longv1e0a3c  = longrg6 + 2 * sizeof(x16);	// nb items sizes + (magic number + nb items) = offset of first raw data which must be the ENT1
  if (ddata.gdatversionlo >= con(0x3))
  {
    if (!DM2_FILE_READ(ddata.gdatfilehandle, DOWNCAST(x32, &ddata.longv1e0a44), con(0x4)))		// Read the next 4 bytes of GDAT which hold the size for the first item entry which must be the ENT1 item
      goto M_errexit;
    ddata.longv1e0a3c += ddata.longv1e0a44 + sizeof(x16);	// there longv1e0a3c gets the offset of the second item after ENT1 item
    if (!DM2_FILE_READ(ddata.gdatfilehandle, DOWNCAST(x16, ptrrg3 + 1), longrg6 - con(0x2)))		// here read the size table before the ENT1 item (except the first item already read, which is exceptionnally on 4 bytes; all others are on 2 bytes max)
     goto M_errexit; // SPX DEBUG
  }
  else
  {
    if (!DM2_FILE_READ(ddata.gdatfilehandle, DOWNCAST(x16, ptrrg3), longrg6))
      goto M_errexit;
    ddata.longv1e0a44 = unsignedlong(*ptrrg3);
    ddata.longv1e0a3c += unsignedlong(*ptrrg3);
  }
  *ptrrg3 = con(0x0);
  ddata.longv1e0a40 = ddata.longv1e0a3c;
   wordrg1 = con(0x0);
  while (wordrg1 < ddata.gdatentries)
  {
    ddata.longv1e0a40 += unsignedlong(*ptrrg3);
    dm2_ulp.setl(wordrg1++, unsignedlong(*ptrrg3++));
  }
   longrg1 = DM2_GET_FILE_SIZE(signedlong(ddata.gdatfilehandle));
  ddata.longgdatfilesize = longrg1;
  if (longrg1 < ddata.longv1e0a40)
  {
    ddata.gdatfiletype1 = true;
    ddata.gdatfiletype2 = true;
  }
  DM2_DEALLOC_LOBIGPOOL(longrg6);
  SKW_LOAD_ENT1();
  if (ddata.gdatversionlo >= con(0x2) && ddata.gdatversionlo != con(0x4))
  {
	  // SPX: I think that item 00-00 of type 08 (raw) exists in PC9821 version but not PCDOS
    if (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x0), con(0x8), con(0x0), con(0x0)) != con(0xffffffff))
    {
      x16 wordrg1 = CUTX16(SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(con(0x0), con(0x8), con(0x0), con(0x0)));
      ddata.v1e0ac2 = wordrg1;
      x8* bptrrg1 = UPCAST(x8, DM2_ALLOC_FREEPOOL_MEMORY(unsignedlong(wordrg1)));
      ddata.p1e0a64 = bptrrg1;
      SKW_LOAD_GDAT_ENTRY_DATA_TO(con(0x0), con(0x8), con(0x0), con(0x0), bptrrg1);
      ddata.v1e0ac2 >>= 2; // unsigned
    }
  }

DEBUG_DISPLAY_GDAT_MAIN_INFO();

  ddata.v1e0a88 = true;
   wordrg12 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x0), con(0xb), con(0x0), con(0x0));	// C00=I00=E00=T011, text encoding flag
   wordrg41 = wordrg12;
  ddata.v1e0ad0 = wordrg12;
  wordrg12 &= con(0x20);
   longrg12 = unsignedlong(wordrg12);
  dm2sound.v1d269c = longrg12;
  wordrg41 &= con(0x40);
  ddata.v1e0ad2 = wordrg41;
  if (wordrg41 == con(0x0))
    dm2_dballochandler.v1e0acc = con(0x3e8);
  else
  {
    dm2_dballochandler.v1e0acc = con(0x1f);
    R_4ADA5();
  }
  // SPX: both these next functions seem not necessary
//  R_2D07D(); // had parameter longrg12
//  SKW_3e74_24b8();
SPX_DEBUG_POP;
  return;

M_errexit:
  SKW_RAISE_SYSERR(con(0x2a));
SPX_DEBUG_POP;
}

static xxx* SKW_EXTRACT_GDAT_IMAGE(x16 eaxw, x32 edxl)
{
  xxx* ptrrg3;
  xxx* p_00; // can carry 0x80nnnnnn - datas!
  xxx* p_08;
  x16 vw_14;
  x16 vw_1c;
  x16 vw_20;
  x16 vw_24;
  x16 vw_28;

  x32 longrg7 = edxl; // TODO: if edxl is a bool, then this one too
  vw_20 = con(0xffffffff);
  if (!dm2_dballochandler.gfxalloc_done)
  {
    s_xlink* ptr = UPCAST(s_xlink, dm2_dballochandler.ptr1e0a5c);
    while (ptr != NULL)
    {
      if (eaxw == ptr->w_04)
        return DOWNCAST(xxx, ptr + 1); // the struct after
      ptr = ptr->p_00;
    }
    ptrrg3 = DOWNCAST(s_xlink, ptr);
  }
  else
  {
    x16 wordrg1 = DM2_ALLOC_CPX_GET_WTABLE_ENTRY(eaxw);
    vw_28 = wordrg1;
    if (wordrg1 != con(0xffffffff))
    {
      if (longrg7 == con(0x0))
        ptrrg3 = DOWNCAST(s_xmalloc, SKW_3e74_48c9(wordrg1));
      else
      {
        R_2F41B(wordrg1);
        ptrrg3 = DOWNCAST(s_xmalloc, dm2_dballochandler.s_table[wordrg1]);
      }
      return ptrrg3 + con(0x12);
    }
  }

  bool vbool_10 = false;
  if (ddata.v1e0ac2 != con(0x0))
  {
    x16 wordrg1 = SKW_TRACK_UNDERLAY(UI16CAST(eaxw));
    vw_24 = wordrg1;
    if (wordrg1 != con(0xffffffff))
    {
      vbool_10 = true;
      x32 longrg44;
      if (!dm2_dballochandler.gfxalloc_done)
        longrg44 = (longrg7 == con(0x0)) ? 1 : 0;
      else
        longrg44 = con(0x1);
      p_08 = UPCAST(xxx, SKW_EXTRACT_GDAT_IMAGE(vw_24, longrg44));
    }
  }
  p_00 = dm2_ulp.getp(unsignedlong(eaxw));
  xxx* ptrrg6; // TODO t_gfxdata* ?
  if (!dm2_ulp.islen(unsignedlong(eaxw)))
    ptrrg6 = p_00;
  else
  {
    bool rg2;
    if (!dm2_dballochandler.gfxalloc_done)
      rg2 = (longrg7 == con(0x0));
    else
      rg2 = true;
    ptrrg6 = SKW_QUERY_GDAT_DYN_BUFF(eaxw, rg2, &vw_20);
  }

  x32 longrg1 = UPCAST(u_wwl, ptrrg6)->l0 >> con(0x1a);
  if (longrg1 == con(0xffffffe0))
    return ptrrg6 + con(0xa);
  if (longrg1 != signedlong(dm2_dballochandler.v1e0acc))
    vw_1c = con(0x4);
  else
    vw_1c = con(0x8);
  x16 vw_18 = UPCAST(u_wwl, ptrrg6)->s0.w0 & con(0x3ff);
  x32 longrg43;
  if (vw_1c != con(0x4))
    longrg43 = unsignedlong(vw_18);
  else
    longrg43 = UI32CAST(((vw_18 + con(0x1)) & con(0xfffffffe)) >> con(0x1));
  x16 vw_2c = UPCAST(u_wwl, ptrrg6)->s0.w2 & con(0x3ff);
  x32 vl_0c = (unsignedlong(vw_2c) * longrg43) & con(0xffff);
  if (!dm2_dballochandler.gfxalloc_done)
  {
    if (vw_1c == con(0x4))
      vl_0c += con(0x10);
  }
  if (!dm2_dballochandler.gfxalloc_done)
  {
    x16 wordrg1;
    if (longrg7 == con(0x0))
      wordrg1 = BIGPOOL_HI;
    else
      wordrg1 = BIGPOOL_LO;
    s_xlink* ptrrg1 = UPCAST(s_xlink, DM2_ALLOC_MEMORY_RAM(vl_0c + con(0xe), MASK_8, wordrg1));
    ptrrg1->p_00 = UPCAST(s_xlink, dm2_dballochandler.ptr1e0a5c);
    dm2_dballochandler.ptr1e0a5c = DOWNCAST(s_xlink, ptrrg1);
    ptrrg1->w_04 = eaxw;
    ptrrg1->w_06 = wordrg1;
    ptrrg1->b_08 = CUTX8(vw_1c);
    ptrrg1->w_0a = vw_18;
    ptrrg1->w_0c = vw_2c;
    ptrrg3 = UPCAST(xxx, ptrrg1 + 1); // the struct after
    if (vw_1c == con(0x4))
      SKW_COPY_MEMORY(ptrrg6 + (dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(eaxw) & con(0xffff)) - con(0x10), con(0x10), ptrrg3 + vl_0c - con(0x10));
  }
  else
  {
    if ((vl_0c & con(0x1)) != con(0x0))
      vl_0c++;
    vl_0c += con(0x16);
    s_imghex12* ptr = UPCAST(s_imghex12, SKW_ALLOC_CPXHEAP_CREATE_POINTER(vl_0c));
    if (vw_20 >= con(0x0))
      ptrrg6 = DOWNCAST(t_gfxdata, SKW_3e74_5817(vw_20));
    x16 wordrg1 = SKW_ALLOC_CPXHEAP_CREATE_INDEX();
    vw_28 = wordrg1;
    dm2_dballochandler.w_table2[eaxw] = wordrg1;
    dm2_dballochandler.s_table[wordrg1] = UPCAST(s_xmalloc, ptr);
    ptr->w_0a = eaxw;
    ptr->b_0c = CUTX8(vw_1c);
    ptr->w_0e = vw_18;
    ptr->w_10 = vw_2c;
    ptr++; // struct advance
    ptrrg3 = DOWNCAST(s_imghex12, ptr);
  }

  if (!vbool_10)
  {
    if (vw_1c != con(0x8))
      SKW_DECODE_IMG3_UNDERLAY(UPCAST(x16, ptrrg6), ptrrg3);
    else
      SKW_DECODE_IMG9(ptrrg6, unsignedlong(CUTX16(dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(eaxw))), ptrrg3);
  }
  else
  {
    if (dm2_dballochandler.gfxalloc_done)
    {
      x16 wordrg1 = DM2_ALLOC_CPX_GET_WTABLE_ENTRY(vw_24);
      vw_14 = wordrg1;
      p_08 = DOWNCAST(s_xmalloc, dm2_dballochandler.s_table[wordrg1]) + con(0x12);  // BUGFIX phaseE, TODO: exceeds current structsize - but this is variable
    }
    SKW_DECODE_IMG3_OVERLAY(DOWNCAST(xxx, p_08), ptrrg3, ptrrg6);
  }
  if (!dm2_dballochandler.gfxalloc_done)
  {
    if ((I32CAST(p_00) & con(0x80000000)) != 0)
      DM2_DEALLOC_BIGPOOL_STRUCT_BEFORE(UPCAST(s_malloctail, ptrrg6));
    if (vbool_10)
      SKW_FREE_PICT_ENTRY(UPCAST(t_gfxdata, p_08));
    return ptrrg3;
  }
  if (longrg7 == con(0x0))
    R_2E68E(vw_28, false);
  if (vw_20 >= con(0x0))
    SKW_3e74_585a(vw_20, false);
  if (vbool_10)
    R_2E68E(vw_14, false);
  return ptrrg3;
}

//t_gfxdata* SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(x8 eaxb, x8 ebxb, x8 edxb)
t_gfxdata* SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(x8 eaxb, ui16 ebxb, ui16 edxb)	 // SPX fix to avoid negative values
{
  if (ddata.longmallocx != dm2_dballochandler.malloci)
    R_2E581();
  u_bbwlong* ptrrg4 = DM2_QUERY_GDAT_ENTRYPTR(eaxb, con(0x1), ebxb, edxb);
  bool boolrg6 = false;
  x16 wordrg1;
  if (ptrrg4 != NULL)
  {
    boolrg6 = DM2_IS_CLS1_CRITICAL_FOR_LOAD(eaxb);
    wordrg1 = ptrrg4->u.w_02 & con(0x7fff);
  }
  else
    wordrg1 = con(0xffff);

  if (wordrg1 == con(0xffffffff) || (dm2_ulp.islen(wordrg1) && !boolrg6))
    //wordrg1 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x15), con(0x1), con(0xfe), con(0xfe));	 // (0x15, 0xfe, 0x01, 0xfe) // the Yukman :P icon
	wordrg1 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x15), con(0x1), ucon16(0xfe), ucon16(0xfe));	 // SPX: fix xFE must be positive and not -2 !
  if (wordrg1 == -1) return NULL;	// SPX add this to prevent crashing in EXTRACT
  return UPCAST(t_gfxdata, SKW_EXTRACT_GDAT_IMAGE(wordrg1, con(0x0)));
}

// TODO: errexit-label left
void SKW_LOAD_DYN4(s_hex6* eaxps, x16 edxw)
{
  x32 longrg6;
  s_hex6* ps_04;
  s_gdat vs_08;
  s_hex6 vs_28;
  x32 vl_30;
  x32 vl_34;
  x32 vl_38;
  x32 vl_3c;
  x32 vl_40;
  u_spec vs_50;
  x16 vw_54;
  x16 vw_58;
  x32 vl_5c;
  x8 vb_64;

  ps_04 = eaxps;
  x16 vw_00 = edxw;
  bool vbool_48 = false;
  bool vbool_44 = false;
  xxx* ptrrg7 = DM2_ALLOC_LOBIGPOOL_MEMORY(unsignedlong(ddata.gdatentries), true);
  if (!dm2_dballochandler.v1e0a84)
    SKW_482b_07c2();
  vs_50.ps_00 = ps_04;
  x16 vw_60 = con(0x0);
  while (vw_60 < vw_00)
  {
    vs_08.l_00 = con(0x1);
    x8* ptrtmprg6 = DOWNCAST(s_hex6, &vs_08.s_04);
    x8* ptrtmprg5 = DOWNCAST(s_hex6, vs_50.ps_00);
    SKW_COPY_MEMORY(ptrtmprg5, 6, ptrtmprg6);
    ptrtmprg5 += 6;
    ptrtmprg6 += 6;
    vl_5c = vs_08.s_04.u.l_00;
    if ((vs_08.s_04.u.s_00.w_00 & con(0x7fff)) == con(0x1))
    {
      vs_50.ps_00++; // struct advance!
      x8* ptrtmprg6 = DOWNCAST(s_hex6, &vs_08.s_0a);
      x8* ptrtmprg5 = DOWNCAST(s_hex6, vs_50.ps_00);
      SKW_COPY_MEMORY(ptrtmprg5, 6, ptrtmprg6);
      ptrtmprg5 += 6;
      ptrtmprg6 += 6;
      vw_60++;
    }

    while ((SKW_QUERY_NEXT_GDAT_ENTRY(&vs_08) ? 1 : 0) != con(0x0))
    {
      if (vs_08.b_16 == con(0xb) || vs_08.b_16 == con(0xc))
        continue;
      x16 wordrg5c = vs_08.p_10->u.w_02;
      if ((wordrg5c & con(0x8000)) != con(0x0))
        continue;
      x32 longrg2 = unsignedlong(wordrg5c);
      xxx* ptrrg19 = ptrrg7 + longrg2;
      x8 byterg4hi = *ptrrg19;
      x8 byterg4lo = byterg4hi & con(0x1f);
      if (CUTX16(vl_5c & con(0x8000)) == con(0x0))
      {
        if (byterg4hi == con(0x0))
        {
          if (vs_08.b_16 == con(0x2))
          {
            if (SKW_482b_015c(CUTX16(longrg2)) != con(0x0) || dm2_dballochandler.v1e13fe[2] != con(0x0))
              continue;
          }
          ptrrg7[wordrg5c] = con(0x1);
        }
        else
        {
          if (byterg4lo != con(0x1f))
            *ptrrg19 = ++byterg4hi;
        }
      }
      else if (byterg4lo != 0 && byterg4lo != con(0x1f))
        *ptrrg19 = byterg4hi - 1;
    }

    vs_50.ps_00++; // struct advance
    if (signedlong(vw_60) == (signedlong(vw_00) - con(0x1)) && !vbool_44)
    {
      vbool_44 = true;
      vs_50.ps_00 = &vs_28;
      vs_28.u.s_00.w_00 = con(0x0);
      vw_60--;
      vs_28.setlatelong(SKW_4BYTES_TO_LONG(con(0x15), con(0xfe), con(0x1), con(0xfe))); // parameter sequence adjusted
    }
    vw_60++;
  }

  x16 wordrg5b = con(0x0);
  while (UI16CAST(wordrg5b) < ddata.gdatentries)
  {
    xxx* ptrrg1 = ptrrg7 + unsignedlong(wordrg5b);
    x16 wordrg44 = unsignedword(*ptrrg1);
    vw_60 = wordrg44;
    if (wordrg44 != con(0x0))
    {
      wordrg44 &= con(0x1f);
      if (wordrg44 == con(0x0))
        *ptrrg1 &= con(0x20);
      else
        *ptrrg1 = (vw_60 & con(0xffffffc0)) | con(0x21);
    }
    wordrg5b++;
  }
  vs_50.ps_00 = ps_04;
  vw_60 = con(0x0);

  while (vw_60 < vw_00)
  {
    vs_08.l_00 = con(0x1);
    x8* ptrtmprg6 = DOWNCAST(s_hex6, &vs_08.s_04);
    x8* ptrtmprg5 = DOWNCAST(s_hex6, vs_50.ps_00);
    SKW_COPY_MEMORY(ptrtmprg5, 6, ptrtmprg6);
    ptrtmprg5 += 6;
    ptrtmprg6 += 6;
    vl_5c = vs_08.s_04.u.l_00;
    if ((vs_08.s_04.u.s_00.w_00 & con(0x7fff)) != con(0x1))
    {
      if ((CUTX16(vl_5c) & con(0x8000)) != con(0x0) || (vs_08.s_04.b_04 != con(0xffffffff) && vs_08.s_04.b_04 != con(0x2)))
      {
        vs_50.ps_00++; // struct advance
        vw_60++;
        continue;
      }
      vs_08.s_04.b_04 = con(0x2);
    }
    else
    {
      xxx* ptrrg1 = DOWNCAST(s_hex6, vs_50.ps_00) + con(0x6);
      vs_50.ps_00 = UPCAST(s_hex6, ptrrg1);
      x8* ptrtmprg6 = DOWNCAST(s_hex6, &vs_08.s_0a);
      x8* ptrtmprg5 = ptrrg1;
      SKW_COPY_MEMORY(ptrtmprg5, 6, ptrtmprg6);
      ptrtmprg5 += 6;
      ptrtmprg6 += 6;
      vw_60++;
      if (vs_08.s_04.b_04 == con(0xffffffff) || (UI8CAST(vs_08.s_04.b_04) <= UI8CAST(con(0x2)) && UI8CAST(vs_08.s_0a.b_04) >= UI8CAST(con(0x2))))
      {
        vs_08.s_0a.b_04 = con(0x2);
        vs_08.s_04.b_04 = con(0x2);
        if ((CUTX16(vl_5c) & con(0x8000)) != con(0x0) || (vs_08.s_04.b_04 != con(0xffffffff) && vs_08.s_04.b_04 != con(0x2)))
        {
          vs_50.ps_00++; // struct advance
          vw_60++;
          continue;
        }
        vs_08.s_04.b_04 = con(0x2);
      }
      else
      {
        vs_50.ps_00++; // struct advance
        vw_60++;
        continue;
      }
    }
    for(;;)
    {
      if (!SKW_QUERY_NEXT_GDAT_ENTRY(&vs_08))
      {
        vs_50.ps_00++; // struct advance
        vw_60++;
        break;
      }
      x32 longrg17 = UI32CAST(vs_08.p_10->u.w_02 & con(0x7fff));
      if (UI32CAST(ptrrg7[longrg17] & con(0x1)) == con(0x0))
      {
        if (SKW_482b_015c(CUTX16(longrg17)) == con(0x0))
          continue;
      }
      x32 longrg2 = unsignedlong(vs_08.p_10->u.b_01);
      SKW_482b_0624(vs_08.b_14, CUTLX8(longrg2), vs_08.p_10->u.b_00);
    }
  }
  if (ddata.v1e0ac2 != con(0x0))
  {
    x16 wordreg5a = con(0x0);
    while (UI16CAST(wordreg5a) < ddata.gdatentries)
    {
      x32 longrg18 = unsignedlong(wordreg5a);
      if (UI32CAST(ptrrg7[longrg18] & con(0x1)) != con(0x0))
        for(;;)
        {
          x16 wordrg19 = SKW_TRACK_UNDERLAY(UI16CAST(longrg18));
          if (wordrg19 == con(0xffffffff))
            break;
          x32 longrg43 = unsignedlong(wordrg19);
          if ((ptrrg7[longrg43] & con(0x1)) != con(0x0))
            break;
          ptrrg7[longrg43] |= ptrrg7[wordreg5a] & con(0xffffffc1);
          longrg18 = longrg43;
        }
      wordreg5a++;
    }
  }

  x32 longrg3;
  for(;;)
  {
    x32 longrg42 = con(0x0);
    longrg3 = con(0x0);
    if (dm2_dballochandler.pppw2.pw0 == NULL)
      vl_30 = 2 * (dm2_dballochandler.pppw2.pw0 - dm2_dballochandler.pppw2.pw4);
    else
      vl_30 = con(0x0);
    vl_3c = DOWNCAST(x16, dm2_dballochandler.pppw1.pw0) - DOWNCAST(s_xmalloc, dm2_allochandler.bigpool_start_b);

    x16 wordreg59 = con(0x0);
    while (UI16CAST(wordreg59) < ddata.gdatentries)
    {
      xxx* ptrrg15 = ptrrg7 + unsignedlong(wordreg59);
      *ptrrg15 &= con(0xffffffe1);
      vw_60 = unsignedword(*ptrrg15);
      vs_50.ps_00 = UPCAST(s_hex6, dm2_ulp.getp(unsignedlong(wordreg59)));
      // TODO: now check if MSB is set (special datas)
      if ((vs_50.l_00 & con(0x80000000)) != 0)
      {
        // TODO: now special area, STACKL(0x50) no pointer!
        if ((vw_60 & con(0x1)) != con(0x0))
        {
          x32 longrg2 = (((vs_50.l_00 & con(0xffff)) + 1) & con(0xfffffffe)) + con(0x4);
          if (vl_30 == con(0x0))
          {
            ptrrg7[wordreg59] |= con(0x8);
            longrg3 += longrg2;
          }
          else
          {
            vw_58 = vw_60 & con(0x40);
            longrg6 = signedlong(vw_58);
            if (longrg6 == con(0x0))
            {
              *ptrrg15 |= con(0x10);
              longrg42 += longrg2;
            }
            else
            {
              ptrrg7[wordreg59] |= con(0x8);
              longrg3 += longrg2;
            }
          }
        }
      }
      else
      {
        x16* wptr0 = UPCAST(x16, DOWNCAST(s_hex6, vs_50.ps_00) - con(0x2));
        x32 longrg2 = UI32CAST(*wptr0 + 1) & con(0xfffffffe) + con(0x4);
        xxx* ptrrg11;
        if (dm2_dballochandler.pppw1.pw0 == NULL || (ptrrg11 = DOWNCAST(s_hex6, vs_50.ps_00)) < DOWNCAST(x16, dm2_dballochandler.pppw1.pw4) || ptrrg11 > DOWNCAST(x16, dm2_dballochandler.pppw1.pw0))
        {
          if (dm2_dballochandler.pppw2.pw0 == NULL || (ptrrg11 = DOWNCAST(s_hex6, vs_50.ps_00)) < DOWNCAST(x16, dm2_dballochandler.pppw2.pw4) || ptrrg11 > DOWNCAST(x16, dm2_dballochandler.pppw2.pw0))
            ptrrg7[wordreg59] = con(0x0);
          else
          {
            xxx* ptrrg1 = ptrrg7 + unsignedlong(wordreg59);
            *ptrrg1 |= con(0x14);
            vw_58 = vw_60 & con(0x1);
            longrg6 = signedlong(vw_58);
            if (longrg6 == con(0x0))
              *ptrrg1 |= con(0xffffff80);
            longrg42 += longrg2;
          }
        }
        else
        {
          xxx* ptrrg1 = ptrrg7 + unsignedlong(wordreg59);
          *ptrrg1 |= con(0xa);
          vw_58 = vw_60 & con(0x1);
          longrg6 = signedlong(vw_58);
          if (longrg6 == con(0x0))
            *ptrrg1 |= con(0x40);
          longrg3 += longrg2;
        }
      }
      wordreg59++;
    }
    if (longrg42 > vl_30)
    {
      x16 wordreg58 = con(0x0);
      for(;;)
      {
        if ((vl_3c - longrg3) <= dm2_dballochandler.v1d6775 || longrg42 <= vl_30 || UI16CAST(wordreg58) >= ddata.gdatentries)
        {
          wordreg58 = con(0x0); // no new variable here!
          while ((vl_3c - longrg3) > dm2_dballochandler.v1d6775 && longrg42 > vl_30 && UI16CAST(wordreg58) < ddata.gdatentries)
          {
            longrg6 = unsignedlong(wordreg58);
            xxx* ptrrg1 = ptrrg7 + longrg6;
            vb_64 = *ptrrg1 & con(0xffffff90);
            if (vb_64 == con(0x10))
            {
              *ptrrg1 ^= con(0x18);
              x32 longrg2 = (((dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(CUTX16(longrg6)) & con(0xffff)) + 1) & con(0xfffffffe)) + con(0x4);
              longrg3 += longrg2;
              longrg42 -= longrg2;
            }
            wordreg58++;
          }
          wordreg58 = con(0x0); // no new variable here!
          while (longrg42 > vl_30 && UI16CAST(wordreg58) < ddata.gdatentries)
          {
            longrg6 = unsignedlong(wordreg58);
            xxx* ptrrg1 = ptrrg7 + longrg6;
            vb_64 = *ptrrg1 & con(0x11);
            if (vb_64 == con(0x10))
            {
              *ptrrg1 &= con(0xffffffef);
              longrg42 -= (((dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(CUTX16(longrg6)) & con(0xffff)) + 1) & con(0xfffffffe)) + con(0x4);
            }
            wordreg58++;
          }

          if (dm2_dballochandler.v1e13fe[3] == con(0x0))
          {
            wordreg58 = con(0x0); // no new variable here!
            while (longrg42 > vl_30 && UI16CAST(wordreg58) < ddata.gdatentries)
            {
              longrg6 = unsignedlong(wordreg58);
              xxx* ptrrg1 = ptrrg7 + longrg6;
              vb_64 = *ptrrg1 & con(0xffffff94);
              if (vb_64 == con(0x10))
              {
                *ptrrg1 ^= con(0x18);
                x32 longrg2 = (((dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(CUTX16(longrg6)) & con(0xffff)) + 1) & con(0xfffffffe)) + con(0x4);
                longrg3 += longrg2;
                longrg42 -= longrg2;
              }
              wordreg58++;
            }
            wordreg58 = con(0x0); // no new variable here!
            while (longrg42 > vl_30 && UI16CAST(wordreg58) < ddata.gdatentries)
            {
              longrg6 = unsignedlong(wordreg58);
              xxx* ptrrg1 = ptrrg7 + longrg6;
              vb_64 = *ptrrg1 & con(0xffffff90);
              if (vb_64 == con(0x10))
              {
                *ptrrg1 ^= con(0x18);
                x32 longrg2 = (((dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(CUTX16(longrg6)) & con(0xffff)) + 1) & con(0xfffffffe)) + con(0x4);
                longrg3 += longrg2;
                longrg42 -= longrg2;
              }
              wordreg58++;
            }
          }
          break; // wordreg58 stays alive!
        }
        longrg6 = unsignedlong(wordreg58);
        xxx* ptrrg1 = ptrrg7 + longrg6;
        vb_64 = *ptrrg1 & con(0xffffff94);
        if (vb_64 == con(0x10))
        {
          *ptrrg1 ^= con(0x18);
          x32 longrg2 = (((dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(CUTX16(longrg6)) & con(0xffff)) + 1) & con(0xfffffffe)) + con(0x4);
          longrg3 += longrg2;
          longrg42 -= longrg2;
        }
        wordreg58++;
      }
    }
    x16 wordreg56 = con(0x0);
    for(;;)
    {
      vl_38 = vl_30 - longrg42;
      if (vl_38 < con(0x6) || UI16CAST(wordreg56) >= ddata.gdatentries)
        break;
      xxx* ptrrg6 = ptrrg7 + unsignedlong(wordreg56);
      vb_64 = *ptrrg6 & con(0x48);
      if (vb_64 == con(0x8))
      {
        x32 longrg2 = (((dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(wordreg56) & con(0xffff)) + con(0x1)) & con(0xfffffffe)) + con(0x4);
        if (longrg2 <= vl_38)
        {
          *ptrrg6 ^= con(0x18);
          longrg42 += longrg2;
          longrg3 -= longrg2;
        }
      }
      wordreg56++;
    }
    x16 wordreg55 = con(0x0);
    while ((vl_3c - longrg3) <= dm2_dballochandler.v1d6775 && UI16CAST(wordreg55) < ddata.gdatentries)
    {
      x32 longrg61 = unsignedlong(wordreg55);
      xxx* ptrrg1 = ptrrg7 + longrg61;
      vb_64 = *ptrrg1 & con(0x9);
      if (vb_64 == con(0x8))
      {
        *ptrrg1 ^= con(0xfffffff7);
        longrg3 -= (((dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(CUTX16(longrg61)) & con(0xffff)) + 1) & con(0xfffffffe)) + con(0x4);
      }
      wordreg55++;
    }
    x32 longrg13 = vl_3c - longrg3;
    dm2_dballochandler.v1e0a74 = vl_30 - longrg42;
    x32 longrg2 = longrg13 - dm2_dballochandler.v1e0a90;
    dm2_dballochandler.v1e0a8c = longrg2;
    dm2_dballochandler.v1e0a0c = longrg13;
    if (dm2_dballochandler.v1e0a84)
    {
      DM2_DEALLOC_LOBIGPOOL(unsignedlong(ddata.gdatentries));
      return;
    }
    bool jump = false;
    if (longrg2 >= 0 && dm2_dballochandler.v1e0a74 >= con(0x0))
      jump = true;
    else if (vbool_48)
    {
      SKW_RAISE_SYSERR(con(0x2c));
      jump = true;
    }
    if (jump)
    {
      vl_40 = DOWNCAST(x16, dm2_dballochandler.pppw1.pw8) - (DOWNCAST(x16, dm2_dballochandler.pppw1.pw0) - longrg3);
      x16 wordrg54 = con(0x0);
      while (UI16CAST(wordrg54) < ddata.gdatentries)
      {
        xxx* ptrrg1 = ptrrg7 + unsignedlong(wordrg54);
        x16 wordrg2 = unsignedword(*ptrrg1);
        vw_60 = wordrg2;
        wordrg2 &= con(0x6);
        if (wordrg2 != con(0x0) && (vw_60 & con(0x18)) == con(0x0))
        {
          *ptrrg1 &= con(0x20);
          SKW_3e74_28de(wordrg54);
          dm2_ulp.R_2D87A(wordrg54);
        }
        wordrg54++;
      }
      if (vl_40 > con(0x0))
        SKW_GUARANTEE_FREE_CPXHEAP_SIZE(vl_40);
      SKW_3e74_2b30();
      longrg6 = vl_40;
      if (longrg6 > con(0x0))
      {
        dm2_dballochandler.cpx_available -= longrg6;
        dm2_dballochandler.pppw1.pw4 = dm2_dballochandler.pppw1.pw0 - longrg3 / 2;
      }
      bool vbool_4c;
      do
      {
        xxx* ptrrg4;
        vbool_4c = false;
        x16 wordrg53 = con(0x0);
        while (UI16CAST(wordrg53) < ddata.gdatentries)
        {
          bool pass = false;
          x32 longrg10 = unsignedlong(wordrg53);
          x16 wordrg42 = unsignedword(ptrrg7[longrg10]);
          vw_60 = wordrg42;
          wordrg42 &= con(0xff0c);
          xxx* ptrrg10;
          if (wordrg42 != con(0xc))
          {
            if ((vw_60 & con(0x12)) != con(0x12))
              pass = true;
            else
            {
              x32 longrg2 = ((unsignedlong(CUTX16(dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(CUTX16(longrg10)))) + 1) & con(0xfffffffe)) + con(0x4);
              x32 longrg12;
              if (dm2_dballochandler.pppw2.pw0 != NULL)
                longrg12 = 2 * (dm2_dballochandler.pppw2.pw8 - dm2_dballochandler.pppw2.pw4);
              else
                longrg12 = con(0x0);
              if (longrg12 < longrg2)
              {
                R_2D802(&dm2_dballochandler.pppw2);
                if (dm2_dballochandler.pppw2.pw0 != NULL)
                  longrg12 = 2 * (dm2_dballochandler.pppw2.pw8 - dm2_dballochandler.pppw2.pw4);
                else
                  longrg12 = con(0x0);
                if (longrg12 < longrg2)
                {
                  vbool_4c = true;
                  pass = true;
                }
              }
              if (!pass)
              {
                longrg6 = unsignedlong(wordrg53);
                vl_34 = longrg6;
                ptrrg4 = dm2_ulp.getp(longrg6);
                ptrrg10 = DOWNCAST(xxx*, &dm2_dballochandler.pppw2.pw0); // TODO: or & of structstart
              }
            }
          }
          else
          {
            x32 longrg2 = (((dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(CUTX16(longrg10)) & con(0xffff)) + 1) & con(0xfffffffe)) + con(0x4);
            x32 longrg11;
            if (dm2_dballochandler.pppw1.pw0 == NULL)
              longrg11 = con(0x0);
            else
              longrg11 = 2 * (dm2_dballochandler.pppw1.pw8 - dm2_dballochandler.pppw1.pw4);
            if (longrg11 < longrg2)
            {
              R_2D802(&dm2_dballochandler.pppw1);
              if (dm2_dballochandler.pppw1.pw0 != NULL)
                longrg11 = 2 * (dm2_dballochandler.pppw1.pw8 - dm2_dballochandler.pppw1.pw4);
              else
                longrg11 = con(0x0);
              if (longrg11 < longrg2)
              {
                vbool_4c = true;
                pass = true;
              }
            }
            if (!pass)
            {
              longrg6 = unsignedlong(wordrg53);
              vl_34 = longrg6;
              ptrrg4 = dm2_ulp.getp(longrg6);
              ptrrg10 = DOWNCAST(xxx*, &dm2_dballochandler.pppw1.pw0); // TODO or & of structstart
            }
          }
          if (!pass)
          {
            ptrrg4 = R_2D8BA(UPCAST(s_hmm, ptrrg10), longrg2, ptrrg4);
            dm2_ulp.R_2D87A(CUTX16(longrg6));
            dm2_ulp.setp(vl_34, ptrrg4);
            ptrrg7[longrg6] ^= con(0x6);
          }
          wordrg53++;
        }
      } while (vbool_4c);
      break; // breaks main loop, goes to the end part
    }
    vbool_48 = true;
    while (dm2_dballochandler.malloch != NULL)
      SKW_3e74_28de(dm2_dballochandler.malloch->wa);

    x16 wordrg5 = con(0x0);
    while (UI16CAST(wordrg5) < ddata.gdatentries)
    {
      vs_50.ps_00 = UPCAST(s_hex6, dm2_ulp.getp(unsignedlong(wordrg5)));
      if ((vs_50.l_00 & con(0x80000000)) == 0)
      {
        xxx* ptrreg61 = DOWNCAST(x16, dm2_dballochandler.pppw1.pw0);
        bool passx = false;
        xxx* ptrrg11;
        if (ptrreg61 == NULL || (ptrrg11 = DOWNCAST(s_hex6, vs_50.ps_00)) < DOWNCAST(x16, dm2_dballochandler.pppw1.pw4) || ptrrg11 > ptrreg61)
        {
          if (dm2_dballochandler.pppw2.pw0 == NULL || (ptrrg11 = DOWNCAST(s_hex6, vs_50.ps_00)) < DOWNCAST(x16, dm2_dballochandler.pppw2.pw4) || ptrrg11 > DOWNCAST(x16, dm2_dballochandler.pppw2.pw0))
            passx = true;
        }
        if (!passx)
        {
          x16 wordrg1 = unsignedword(ptrrg7[wordrg5]);
          vw_60 = wordrg1;
          if ((wordrg1 & con(0xff01)) == con(0x0))
            vw_60 &= con(0x20);
          else
            vw_60 &= con(0xfffffff9);
          ptrrg7[wordrg5] = CUTX8(vw_60);
          dm2_ulp.R_2D87A(wordrg5);
        }
      }
      wordrg5++;
    }
    R_2D802(&dm2_dballochandler.pppw1);
    R_2D802(&dm2_dballochandler.pppw2);
    dm2_dballochandler.cpx_available += 2 * (dm2_dballochandler.pppw1.pw8 - dm2_dballochandler.pppw1.pw4);
    dm2_dballochandler.pppw1.pw4 = dm2_dballochandler.pppw1.pw8;
  }

  // end part
  R_2D802(&dm2_dballochandler.pppw1);
  R_2D802(&dm2_dballochandler.pppw2);
  if (vl_40 < con(0x0))
  {
    dm2_dballochandler.cpx_available -= vl_40;
    dm2_dballochandler.pppw1.pw4 = dm2_dballochandler.pppw1.pw0 - longrg3 / 2; // !
  }

  bool boolrg6 = false;
  x16 wordrg5 = con(0x0);
  while (UI16CAST(wordrg5) < ddata.gdatentries)
  {
    if (dm2_dballochandler.v1e0200)
    {
      x32 longrg4 = 500 * (unsignedlong(wordrg5) + 1);
      x32 longrg2 = unsignedlong(ddata.gdatentries);
      SKW_DRAW_DIALOGUE_PROGRESS(longrg4 / longrg2 + con(0x1f4));
    }
    x16 wordrg4 = unsignedword(ptrrg7[wordrg5]) & con(0xdf);
    vw_60 = wordrg4;
    if (wordrg4 != con(0x0))
    {
      wordrg4 &= con(0xff06);
      if (wordrg4 == con(0x0))
      {
        vw_54 = CUTX16(dm2_ulp.SKW_QUERY_GDAT_RAW_DATA_LENGTH(wordrg5));
        x32 longrg2 = ((unsignedlong(vw_54) + 1) & con(0xfffffffe)) + con(0x4);
        s_hmm* ptrrg1;
        if ((vw_60 & con(0x8)) == con(0x0))
          ptrrg1 = UPCAST(s_hmm, &dm2_dballochandler.pppw2.pw0); // TODO or & of structstart
        else
          ptrrg1 = UPCAST(s_hmm, &dm2_dballochandler.pppw1.pw0); // TODO or & of structstart
        xxx* ptrrg11 = DOWNCAST(s_hex6, R_2D8AD(ptrrg1, longrg2));
        vs_50.ps_00 = UPCAST(s_hex6, ptrrg11);
        vl_34 = unsignedlong(wordrg5);
        vs_50.ps_00 = UPCAST(s_hex6, ptrrg11 + con(0x2));
        dm2_ulp.setp(vl_34, DOWNCAST(s_hex6, vs_50.ps_00));
        x16* wptr1 = UPCAST(x16, DOWNCAST(s_hex6, vs_50.ps_00) + longrg2 - con(0x4));
        *wptr1 = wordrg5;
        x16* wptr2 = UPCAST(x16, DOWNCAST(s_hex6, vs_50.ps_00) - con(0x2));
        *wptr2 = vw_54;
        if (!boolrg6)
        {
          SKW_2066_03e0(con(0x0));
          DM2_GRAPHICS_DATA_OPEN();
          boolrg6 = true;
        }
        SKW_LOAD_GDAT_RAW_DATA(wordrg5, dm2_ulp.getp(unsignedlong(wordrg5)));
      }
    }
    wordrg5++;
  }

  if (boolrg6)
    DM2_GRAPHICS_DATA_CLOSE();
  dm2_dballochandler.bigpool_endofunpreservedfree = VCAST(dm2_dballochandler.pppw1.pw4);
  SKW_482b_0684();
  if (dm2_dballochandler.v1e0200)
    SKW_DRAW_DIALOGUE_PROGRESS(con(0x3e8));
  DM2_DEALLOC_LOBIGPOOL(unsignedlong(ddata.gdatentries));
}

