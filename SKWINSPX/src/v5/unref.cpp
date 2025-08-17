#include <dm2data.h>
#include <uitmouse.h>
#include <gfxmain.h>
#include <skimage.h>
#include <skgdtqdb.h>
#include <uikeybd.h>
#include <util.h>

// TODO: needs: (from startend.cpp)
void DM2_3929_07e1(i32 eaxl, i32 edxl);

#include <emu.h>
#include <regs.h>

const i8 table1d2558[2] =
{
  (i8)0xf0, 0x0f
};

// unreferenced, type of xeaxp not sure
bool DM2_unref_44c8_1e1a(i8* xeaxp, i16 edxw)
{
  return (table1d2558[edxw & 0x1] & *(xeaxp + edxw / 2)) != 0;
}

i32 DM2_unref_3929_0826(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG4;

  DM2_HIDE_MOUSE();
  RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL00]);
  RG4L = unsignedlong(RG4Blo);
  RG1L = lcon(0xf);
  DM2_FILL_SCREEN_RECT(/*TAG QUERY*/ RG1W, ui8_to_pixel(RG4UBlo));
  DM2_SHOW_MOUSE();
  DM2_3929_07e1(0, 2);
  RG1L = 0;
  RG2L = lcon(0xffffffff);
  //m_45859:
  for (;;)
  {
    RG4L = unsignedlong(RG1W);
    if (RG4L >= lcon(0x3))
      return RG1L;
    //m_45851:
    ddat.v1e1408[RG4L] = RG2L;
    RG1L++;
  }
  FOREND
}

i32 R_1FE74(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;

  RG1L = eaxl - RG2L;
  RG4L -= RG3L;
  RG2W = argw0 & lcon(0x1); // adj
  RG2L = signedlong(RG2W);
  if (RG2L != 0)
  {
    if (RG4W != 0)
      return 0;
    RG4L = signedlong(argw0);
    if (RG4L != lcon(0x1))
      //m_1FEA1:
      RG1Blo = RG1W >= 0 ? 1 : 0;
    else
      RG1Blo = RG1W <= 0 ? 1 : 0;
    //m_1FEA7:
    RG1L = unsignedlong(RG1Blo);
    return RG1L == 0 ? 0 : lcon(0x1);
  }
  //m_1FEBB:
  if (RG1W != 0)
    return 0;
  RG1L = signedlong(argw0);
  if (RG1L != 2)
    //m_1FED4:
    RG1Blo = RG4W >= 0 ? 1 : 0;
  else
    RG1Blo = RG4W <= 0 ? 1 : 0;
  //m_1FEDA:
  RG1L = unsignedlong(RG1Blo);
  return RG1L == 0 ? 0 : lcon(0x1);
}

void R_2F5DE(unk* xeaxp, unk* xedxp)
{
  c_nreg _RG1; RG1P = xeaxp;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4P = xedxp;

  RG2L = unsignedlong(byte_at(RG1P, -lcon(0x6)));
  if (RG2L != lcon(0x4))
    //m_2F5F8:
    RG2L = unsignedlong(word_at(RG1P, -lcon(0x4)));
  else
  {
    RG2L = unsignedlong(word_at(RG1P, -lcon(0x4))) + 1;
    RG2L &= lcon(0xfffffffe);
    RG2L >>= bcon(0x1);
  }
  //m_2F5FE:
  RG2L = unsignedlong(RG2W) * unsignedlong(word_at(RG1P, -2));
  RG2L = unsignedlong(RG2W) + lcon(0x6);
  RG4P -= 6;
  RG1P -= 6;
  DM2_COPY_MEMORY(RG4P, RG1P, RG2L);
}

void R_3E39E(i8 eaxb, i8 edxb, i8 ebxb, c_pixel* destgfx)
{
  c_pixel* srcgfx = DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(eaxb, edxb, ebxb);
  DM2_COPY_MEMORY(DOWNCAST(c_pixel, destgfx), DOWNCAST(c_pixel, srcgfx), unsignedlong(CUTX16(DM2_CALC_IMAGE_BYTE_LENGTH(BMPCAST(srcgfx)))));
}

i32 DM2_TRANSFER_PLAYER(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i16 RG5w;

  RG5w = RG2W;
  RG4L = unsignedlong(RG4W);
  RG1L = DM2_GET_CREATURE_AT(unsignedlong(RG1W), RG4L);
  if (RG1W == lcon(0xffffffff))
    return 0;
  RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG1UW));
  RG2W = word_at(RG1P, 2);
  //m_3BF27:
  for (;;)
  {
    if (RG2W == lcon(0xfffffffe))
      return 0;
    RG1L = RG2L;
    RG1Blo ^= RG2Blo;
    RG1Bhi &= lcon(0x3c);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    RG4L = signedlong(RG1W);
    if (RG4L > lcon(0x4) && RG4L < lcon(0xe))
    {
      RG3L = unsignedlong(RG2W);
      RG1W = DM2_GET_DISTINCTIVE_ITEMTYPE(RG3W);
      if (RG1W == RG5w)
        return lcon(0x1);
      RG4L = signedlong(RG5w);
      RG1L = DM2_query_2fcf_164e(RG3L, RG4L);
      if (RG1L != 0)
        return lcon(0x1);
    }
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG2UW)));
    RG2L = RG1L;
  }
  FOREND
}

bool DM2_1539(void)
{
  i16 dummy1;
  i16 dummy2;
  i16 v;

  DM2_GET_MOUSE_ENTRY_DATA(OO dummy1, OO dummy2, OO v);
  if (CUTX8(v) != 0)
    return true;
  return DM2_HAS_KEY();
}
