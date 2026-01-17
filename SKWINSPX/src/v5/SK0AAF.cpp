#include <xtypes.h>
#include <util.h>
#include <dm2data.h>
#include <gfxstr.h>
#include <skeventq.h>
#include <uitmouse.h>
#include <gfxbmp.h>
#include <skguidrw.h>
#include <bgdat.h>
#include <skimage.h>
#include <SK1031.h>
#include <uiinput.h>
#include <uikeybd.h>
#include <gfxmain.h>
#include <skgdtqdb.h>
#include <skxrect.h>
#include <SK0AAF.h>

#include <emu.h>
#include <regs.h>

i16 DM2_0aaf_0067(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i32 RG51l;
  c_lreg _RG6;
  t_text tarr_00[0x50];
  i32 vl_50;
  i16 vw_54;
  c_o32 vo_58; // poke 16 peek 32 inc16
  i8 vb_5c; // inc8
  i8 vb_60;

  vb_60 = RG1Blo;
  RG4L = 0;
  vo_58.poke16(RG4W);
  vw_54 = -1;
  vl_50 = RG4L;
  DM2_ZERO_MEMORY(ADRESSOF(t_text, tarr_00 + lcon(0x28)), lcon(0x28));
  RG1W = unsignedword(RG1Blo);
  vb_5c = RG1Bhi;
  RG51l = unsignedlong(vb_60);
  //m_A204:
  for (;;)
  {
    RG4Blo = vb_5c;
    if (mkub(RG4Blo) >= mkub(lcon(0x14)))
    {
      if (vb_60 != lcon(0xffffff87))
      {
        //m_A21F:
        RG1W = vo_58.peek16();
        ddat.v1e0204 = RG1W;
      }
      else
        ddat.v1e0204 = lcon(0x7);
      break;
    }
    //m_A191:
    RG6L = unsignedlong(RG4Blo);
    RG3P = ADRESSOF(t_text, tarr_00);
    RG2L = RG6L;
    RG4L = RG51l;
    RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x1a), RG4Blo, RG2Blo, RG3T);
    if (byte_at(RG1P) != 0)
    {
      RG3L = RG6L;
      RG2L = lcon(0xb);
      RG1W = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0x1a), CUTLX8(RG51l), RG2Blo, RG3Blo);
      RG4W = RG1W;
      RG1P = ADRESSOF(t_text, tarr_00 + lcon(0x28)) + 2 * signedlong(vo_58.peek16());
      mov8(location(RG1P), RG4Blo);
      RG3Blo = RG4Blo;
      if (RG3Blo == 0)
      {
        RG3Blo = vb_5c;
        mov8(location(RG1P), RG3Blo);
      }
      RG3L = unsignedlong(RG4W) >> bcon(0x8);
      mov8(location(RG1P + lcon(0x1)), RG3Blo);
      if (RG3Blo != 0)
      {
        RG4L = unsignedlong(RG3Blo);
        vw_54 = RG4W;
        RG1Blo = byte_at(RG1P);
        RG1W = unsignedword(RG1Blo);
        vl_50 = RG1L;
      }
      vo_58.inc16();
    }
    //m_A200:
    vb_5c++;
  }
  //m_A229:
  RG1L = signedlong(vw_54);
  if (RG1L == lcon(0xffffffff))
  {
    RG1L = signedlong(vo_58.peek16());
    if (RG1L == lcon(0x1))
      vw_54 = 1;
  }
  RG1L = DM2_1031_0675(4);
  RG4L = 0;
  //m_A25C:
  for (;;)
  {
    if (Tmouse.is_visible())
    {
      eventqueue.event_unk06 = lcon(0xff);
      break;
    }
    //m_A256:
    DM2_SHOW_MOUSE();
    RG4L++;
  }
  //m_A26F:
  for (;;)
  {
    DM2_EVENT_LOOP();
    DM2_WAIT_SCREEN_REFRESH();
    RG1L = signedlong(vw_54);
    if (RG1L != lcon(0xffffffff))
    {
      // RG1L = unsignedlong(vw_54);  TODO: next call doesn't need this...
      RG1L = 0; // called SKW_476d_04ed here
      if (RG1L != 0)
      {
        RG1L = vl_50 + lcon(0xdb) & lcon(0xffff);
        DM2_1031_0781(RG1L);
      }
    }
    RG1L = signedlong(eventqueue.event_unk06);
    if (RG1L == lcon(0xff))
    {
      RG1W = DM2_HAS_KEY() ? 1 : 0;
      if (RG1W != 0)
      {
        RG1L = signedlong(DM2_GETKEY_TRANSLATED());
        if (RG1L == lcon(0x1c))
          DM2_1031_0781(lcon(0xdb));
      }
    }
    RG1L = signedlong(eventqueue.event_unk06);
    if (RG1L != lcon(0xff))
    {
      if (vb_60 != lcon(0xffffff87))
      {
        //m_A2F4:
        RG3L = 0;
        RG3Blo = tarr_00[0x26 + 2 * RG1L];
      }
      else
        RG3W = eventqueue.event_unk0a;
      break;
    }
  }
  //m_A2FA:
  for (;;)
  {
    RG4L--;
    if (RG4W == lcon(0xffffffff))
      break;
    DM2_HIDE_MOUSE();
  }
  //m_A308:
  gfxsys.backbuff2 = 1;
  DM2_1031_06a5();
  return RG3W;
}

void DM2_0aaf_01db(i16 eaxw, i32 edxl)
{
  c_nreg _RG1; RG1W = eaxw;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG51l;
  c_rect rc_00;
  i16 vw_08;
  i16 vw_0c;

  RG2L = RG1L;
  RG1L = RG4L;
  if (!ddat.v1e0a88)
  {
    //m_A3F0:
    if (RG1L == 0)
      //m_A3FD:
      RG1UBlo = palettecolor_to_ui8(paldat.palette[E_COL01]);
    else
      RG1UBlo = palettecolor_to_ui8(paldat.palette[E_COL00]);
    //m_A405:
    RG1W = unsignedword(RG1Blo);
    RG51l = unsignedlong(RG1W);
    RG1L = signedlong(RG2W);
    RG4R = &rc_00;
    RG1R = DM2_QUERY_EXPANDED_RECT(RG1W, RG4R);
    RG4L = RG51l;
    DM2_FILL_BACKBUFF_RECT(/*TAG blitrect*/ RG1R, /*TAG pixel256*/ ui8_to_pixel(RG4UBlo));
  }
  else
  {
    if (RG1L == 0)
      return;
    RG2L = unsignedlong(ddat.v1e0208);
    RG4L = unsignedlong(ddat.v1e0207);
    t_bmp* bmp = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(ddat.v1e0206, RG4Blo, RG2Blo));
    s_dm2bmpheader* rg1 = getbmpheader(bmp);
    RG3L = signedlong(rg1->height);
    RG2L = signedlong(rg1->width);
    RG4R = &glblrects.dm2rect4;
    RG1R = &rc_00;
    rc_00.calc_centered_rect_in_rect(RG4R, RG2W, RG3W);
    if (eventqueue.event_unk09 != lcon(0xffffffff))
    {
      DM2_QUERY_TOPLEFT_OF_RECT(eventqueue.event_unk09, OO vw_08, OO vw_0c);
      RG1W = vw_08;
      rc_00.x -= RG1W;
      RG1W = vw_0c;
      rc_00.y -= RG1W;
    }
    t_palette* pal;
    if (!dm2_dballochandler.gfxalloc_done)
      //m_A3C9:
      pal = UPCAST(t_palette, PIXELCAST(bmp) + (DM2_CALC_IMAGE_BYTE_LENGTH(bmp)));
    else
    {
      RG2L = unsignedlong(ddat.v1e0208);
      RG4L = unsignedlong(ddat.v1e0207);
      pal = DM2_QUERY_GDAT_IMAGE_LOCALPAL(ddat.v1e0206, RG4Blo, RG2Blo);
    }
    //m_A3D7:
    DM2_DRAW_DIALOGUE_PARTS_PICT(bmp, &rc_00, 7, pal);
    DM2_FREE_PICT_ENTRY(PIXELCAST(bmp));
  }
  //m_A41D:
  DM2_SLEEP_SEVERAL_TIME(20);
}

// belongs to DM2_0aaf_02f8
struct s_mini
{
  t_text* tp_00;
  i16 w_04;
  i16 w_06;
};

// recursive!
i32 DM2_0aaf_02f8(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG51; // s_mini*
  i8* RG52p;
  i8* RG61p;
  c_wlreg _RG62;
  c_wlreg _RG63;
  i32 RG7l;
  t_text tarr_00[0x320];
  t_text tarr_320[0x80];
  t_text tarr_3a0[0x80];
  t_text* tpa_420[0x14];
  t_text tarr_470[0x3c];
  s_mini s32arr_4ac[0x2];
  c_rect rc_4bc;
  i16 vwa_4c4[0x2];
  i32 vl_4c8;
  i16 vw_4cc;
  c_o32 vo_4d0;
  i16 vw_4d4;
  i16 vw_4d8;
  i8 vb_4dc;
  i8 vb_4e0;
  i8 vb_4e4;
  bool skip00087 = false;

  vb_4e0 = RG1Blo;
  vb_4dc = RG4Blo;
  if (RG1Blo != lcon(0xe) && RG1Blo != lcon(0xffffff87))
    skip00087 = true;
  else
  {
    if (vb_4dc != 0)
      //m_A460:
      RG1L = 0;
    else
      skip00087 = true;
  }

  if (skip00087)
    //m_A459:
    RG1L = 1;
  bool skip00091 = false;
  //m_A462:
  vl_4c8 = RG1L;
  RG2Blo = vb_4e0;
  if (RG2Blo == lcon(0x7) || RG2Blo == lcon(0x13))
  {
    RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x1a), lcon(0x59), lcon(0x1), 0) ? 1 : 0;
    if (RG1L != 0)
      vb_4e0 = lcon(0x59);
  }
  if (vb_4dc != 0)
  {
    RG1Blo = vb_4e0;
    if (RG1Blo != 0)
    {
      if (RG1Blo != lcon(0xe))
      {
        RG1L = DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x1a), 0, lcon(0x1), 0) ? 1 : 0;
        if (RG1L != 0)
        {
          RG1L = DM2_0aaf_02f8(0, unsignedlong(vb_4dc)) & lcon(0xff);
          DM2_0aaf_0067(RG1L);
          vb_4dc = 0;
        }
      }
    }
  }
  if (vl_4c8 != 0)
    if (ddat.dialog2 != 0)
      DM2_FADE_SCREEN(1);
  RG51L = 0;
  RG4W = unsignedword(RG4Blo);
  vb_4e4 = RG4Bhi;
  //m_A555:
  for (;;)
  {
    bool skip00093 = false;
    if (mkub(vb_4e4) < mkub(lcon(0x14)))
    {
      //m_A510:
      RG62L = unsignedlong(RG51W);
      RG4L = RG62L;
      RG1L = 40 * RG62L;
      RG3P = ADRESSOF(t_text, tarr_00) + RG1L;
      RG2L = unsignedlong(vb_4e4);
      RG4L ^= RG62L;
      RG4Blo = vb_4e0;
      RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x1a), RG4Blo, RG2Blo, RG3T);
      tpa_420[RG62L] = RG1T; // idxadj
      if (byte_at(RG1P) != 0)
        RG51L++;
      vb_4e4++;
      continue;
    }
    if (ddat.v1e0a88)
    {
      if (dm2_dballochandler.gfxalloc_done)
        skip00093 = true;
      else
      {
        RG1L = unsignedlong(gfxsys.backbuffer_w) + 1;
        RG1L &= lcon(0xfffffffe);
        RG1L >>= bcon(0x1);
        RG4L = unsignedlong(RG1W);
        RG1L = (unsignedlong(gfxsys.backbuffer_h) * RG4L & lcon(0xffff)) + lcon(0x1e);
        if (RG1L <= dm2_dballochandler.bigpool_available())
          skip00093 = true;
      }
    }

    if (skip00093)
    {
      //m_A5A2:
      RG3L = unsignedlong(vb_4e0);
      RG2L = 0;
      RG4L = RG3L;
      t_bmp* bmp1 = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(26, RG4Blo, 0));
      t_palette* pal;
      if (!dm2_dballochandler.gfxalloc_done)
      {
        //m_A5D8:
        RG1L = DM2_CALC_IMAGE_BYTE_LENGTH(bmp1);
        pal = UPCAST(t_palette, PIXELCAST(bmp1) + unsignedlong(RG1W));
      }
      else
        pal = DM2_QUERY_GDAT_IMAGE_LOCALPAL(26, RG3Blo, 0);
      //m_A5E4:
      RG4R = &rc_4bc;
      RG1R = DM2_QUERY_EXPANDED_RECT(4, RG4R);
      RG2L = lcon(0xffffffff);
      RG4R = RG1R;
      DM2_DRAW_DIALOGUE_PARTS_PICT(bmp1, RG4R, RG2W, pal);
      DM2_FREE_PICT_ENTRY(PIXELCAST(bmp1));
      break;
    }

    bool skip00090 = false;
    //m_A60F:
    RG3L = unsignedlong(palettecolor_to_ui8(paldat.palette[lcon(0x1)]));
    RG2L = signedlong(gfxsys.backbuffer_h);
    RG4L = signedlong(gfxsys.backbuffer_w);
    RG1R = &rc_4bc;
    RG1R->set_origin(RG4W, RG2W);
    RG4L = RG3L;
    DM2_FILL_BACKBUFF_RECT(/*TAG blitrect*/ RG1R, /*TAG pixel256*/ ui8_to_pixel(RG4UBlo));
    RG3L = unsignedlong(palettecolor_to_ui8(paldat.palette[5]));
    RG2L = lcon(0xfffffff6);
    RG4L = RG2L;
    RG1R = &rc_4bc;
    RG1R->inflate(RG4W, RG2W);
    RG4L = RG3L;
    DM2_FILL_BACKBUFF_RECT(/*TAG blitrect*/ RG1R, /*TAG pixel256*/ ui8_to_pixel(RG4UBlo));
    if (RG51UW >= mkuw(lcon(0x3)))
    {
      if (RG51UW <= mkuw(lcon(0x3)))
      {
        //m_A672:
        RG62L = 1;
        skip00090 = true;
      }
      else
      {
        if (RG51W == lcon(0x4))
        {
          //m_A679:
          RG62L = RG51L;
          skip00090 = true;
        }
      }
    }

    if (!skip00090)
      //m_A67D:
      RG62L = 0;

    //m_A67F:
    RG3L = 0;
    //m_A69A:
    for (;;)
    {
      RG1L = signedlong(RG3W);
      RG4L = unsignedlong(RG51W);
      if (RG1L >= RG4L)
        break;
      //m_A683:
      RG1L += signedlong(RG62W);
      RG1L = signedlong(table1d27c4[RG1L]);
      DM2_0aaf_01db(RG1W, 0);
      RG3L++;
    }
    break;
  }
  //m_A6A6:
  RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL11]); // SPX changed grey color E_COL12 to yellow
  RG4L = unsignedlong(RG4Blo);
  RG2T = strVersionNumberV5;
  DM2_DRAW_VP_RC_STR(lcon(0x1c2), RG4W, RG2T);
  RG1L = unsignedlong(RG51W);
  if (RG1L >= 2)
    //m_A6DA:
    vw_4d8 = lcon(0x1d7);
  else
    vw_4d8 = lcon(0x1d5);
  //m_A6E4:
  if (RG51UW < mkuw(lcon(0x3)))
  {
    //m_A6F4:
    if (RG51W != 2)
      skip00091 = true;
    else
      RG62L = 1;
  }
  else
  {
    if (RG51UW <= mkuw(lcon(0x3)))
      //m_A701:
      RG62L = 3;
    else
    {
      if (RG51W == lcon(0x4))
        //m_A708:
        RG62L = 6;
      else
        skip00091 = true;
    }
  }

  if (skip00091)
    //m_A70F:
    RG62L = 0;

  //m_A711:
  RG3L = 0;
  RG7l = signedlong(RG62W);
  //m_A740:
  for (;;)
  {
    RG1L = signedlong(RG3W);
    RG4L = unsignedlong(RG51W);
    if (RG1L >= RG4L)
    {
      RG3Bhi = vb_4e0;
      if (RG3Bhi == lcon(0xffffff87))
        jump L_fin;
      RG1L = signedlong(vw_4d8);
      RG4R = &rc_4bc;
      DM2_QUERY_EXPANDED_RECT(RG1W, RG4R);
      vw_4d4 = rc_4bc.w;
      vw_4cc = rc_4bc.h;
      if (vb_4dc == bcon(0x0) || RG3Bhi != bcon(0x0))
      {
        //m_A7E4:
        RG1W = unsignedword(RG1Blo);
        RG1UBlo = palettecolor_to_ui8(paldat.palette[E_COL11]);
        vwa_4c4[0x0] = RG1W;
        RG4L = unsignedlong(vb_4e0);
        RG3P = ADRESSOF(t_text, tarr_320);
        RG2L = lcon(0x14);
        RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x1a), RG4Blo, bcon(0x14), RG3T);
        s32arr_4ac[0x0].tp_00 = RG1T;
        if (vb_4dc == 0)
          //m_A858:
          RG1P = NULL;
        else
        {
          RG1W = unsignedword(RG1Blo);
          RG1UBlo = palettecolor_to_ui8(paldat.palette[E_COL15]);
          vwa_4c4[0x1] = RG1W;
          RG2L = unsignedlong(vb_4dc);
          RG3P = ADRESSOF(t_text, tarr_3a0);
          RG4L = 0;
          RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x1a), bcon(0x0), RG2Blo, RG3T);
        }
        //m_A85A:
        s32arr_4ac[0x1].tp_00 = RG1T;
      }
      else
      {
        RG1W = unsignedword(RG1Blo);
        RG1UBlo = palettecolor_to_ui8(paldat.palette[E_COL15]);
        vwa_4c4[0x0] = RG1W;
        RG2L = unsignedlong(vb_4dc);
        RG4L = unsignedlong(RG3Bhi);
        RG3T = tarr_320;
        RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x1a), RG4Blo, RG2Blo, RG3T);
        s32arr_4ac[0x0].tp_00 = RG1T;
        s32arr_4ac[0x1].tp_00 = NULL;
      }
      break;
    }
    //m_A718:
    RG2T = tpa_420[RG1L]; // idxadj
    RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL11]);
    RG4L = unsignedlong(RG4Blo);
    RG1L += RG7l;
    RG1L = signedlong(table1d27d4[RG1L]);
    DM2_DRAW_VP_RC_STR(RG1W, RG4W, RG2T);
    RG3L++;
  }
  //m_A861:
  RG63L = 0;
  RG52p = ADRESSOF(s_mini, s32arr_4ac);
  vo_4d0.poke16(RG63W);
  //m_A945:
  for (;;)
  {
    RG1L = signedlong(vo_4d0.peek16());
    if (RG1L >= 2)
    {
      if (RG63W == 0)
        jump L_fin;
      RG4L = 2 * signedlong(ddat.v1d2736);
      RG1L = signedlong(RG63W) - RG4L;
      RG4L = RG1L;
      RG1L = signedlong(vw_4cc) - RG4L;
      RG4L = RG1L / 2;
      RG1L = signedlong(strdat.gfxstrw1) + RG4L - 1;
      RG4L = signedlong(RG1W);
      RG1L = signedlong(DM2_MAX(0, RG4W));
      RG63W = rc_4bc.y + RG1W; // adj
      RG52p = ADRESSOF(s_mini, s32arr_4ac);
      RG1L = 0;
      vo_4d0.poke16(RG1W);
      break;
    }
    //m_A877:
    RG1P = pointer_at(RG52p);
    if (RG1P != NULL)
    {
      DM2_QUERY_STR_METRICS(RG1T, UPCAST(i16, RG52p + lcon(0x4)), UPCAST(i16, RG52p + lcon(0x6)));
      RG1L = signedlong(vw_4d4);
      RG4L = RG1L / 8;
      RG2L = RG1L - RG4L;
      RG4L = signedlong(word_at(RG52p, lcon(0x4)));
      if (RG4L > RG2L)
      {
        RG2L = RG1L / 4;
        RG1L += RG2L;
        if (RG4L >= RG1L)
        {
          //m_A8C1:
          RG1L = unsignedlong(vw_4d4);
          mov16(location(RG52p + lcon(0x4)), RG1W);
        }
        else
        {
          RG1L = RG4L / 4;
          RG4L -= RG1L;
          mov16(location(RG52p + lcon(0x4)), RG4W);
        }
      }
      //m_A8CC:
      RG4L = 0;
      vw_4d8 = RG4W;
      //m_A8D6:
      for (;;)
      {
        RG4L = signedlong(vw_4d8);
        RG1P = pointer_at(RG52p);
        if (byte_at(RG1P + RG4L) == 0)
          break;
        RG3L = signedlong(word_at(RG52p, lcon(0x4)));
        RG2P = ADRESSOF(i16, &vw_4d8);
        DM2_gfxstr_3929_04e2(RG1T, tarr_470, RG2P16, RG3L);
        RG1W = ddat.v1d2736; // TODO: gets extended w/o conversion now
        RG1L = 2 * RG1L;
        RG1W += word_at(RG52p, lcon(0x6));
        RG1W -= strdat.gfxstrw3;
        RG1L++;
        RG63L += RG1L;
        RG4L = signedlong(vw_4d8);
        RG1P = pointer_at(RG52p);
        RG1Blo = byte_at(RG1P, RG4L);
        RG1L = unsignedlong(RG1Blo);
        if (RG1L == lcon(0xa))
          vw_4d8++;
      }
    }
    //m_A93A:
    vo_4d0.inc16();
    RG52p += lcon(0x8);
  }
  //m_AA75:
  for (;;)
  {
    RG1L = signedlong(vo_4d0.peek16());
    if (RG1L >= 2)
      jump L_fin;
    //m_A9B7:
    if (pointer_at(RG52p) != NULL)
    {
      RG2L = 0;
      vw_4d8 = RG2W;
      //m_A9CA:
      for (;;)
      {
        RG4L = signedlong(vw_4d8);
        RG1P = pointer_at(RG52p);
        if (byte_at(RG1P + RG4L) == 0)
          break;
        RG1L = DM2_gfxstr_3929_04e2(RG1T, tarr_470, &vw_4d8, signedlong(word_at(RG52p, lcon(0x4))));
        RG2L = signedlong(vo_4d0.peek16());
        RG2W = vwa_4c4[RG2L];
        RG2L = unsignedlong(RG2W);
        RG4L = signedlong(RG63W);
        RG1L = signedlong(CUTX16((signedlong(gfxsys.backbuffer_w) - signedlong(RG1W)) / 2));
        RG3P = ADRESSOF(t_text, tarr_470);
        DM2_DRAW_VP_STR(RG1W, RG4W, RG2W, RG3T);
        RG1W = strdat.strxplus + ddat.v1d2736;
        RG63L += RG1L;
        RG1L = signedlong(vw_4d8);
        RG1Blo = byte_at(pointer_at(RG52p), RG1L);
        RG1L = unsignedlong(RG1Blo);
        if (RG1L == lcon(0xa))
          vw_4d8++;
      }
    }
    //m_AA6A:
    vo_4d0.inc16();
    RG52p += lcon(0x8);
  }
  FOREND

L_fin:
  if (vl_4c8 != 0)
  {
    DM2_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
    RG1L = 0;
    if (ddat.dialog2 != 0)
      DM2_FADE_SCREEN(0);
  }
  //m_AA9E:
  gfxsys.backbuff2 = 1;
  RG1Blo = vb_4e0;
  return RG1L;
}

