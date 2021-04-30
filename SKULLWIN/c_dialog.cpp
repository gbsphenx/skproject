#include "dm2data.h"
#include "c_querydb.h"
#include "fileio.h"
#include "c_str.h"
#include "c_gfx_str.h"
#include "c_xrect.h"
#include "c_gfx_bmp.h"
#include "c_savegame.h"
#include "c_gfx_main.h"
#include "c_tmouse.h"
#include "c_gui_draw.h"
#include "c_1031.h"
#include "c_eventqueue.h"
#include "c_input.h"
#include "startend.h"
#include "util.h"
#include "c_image.h"
#include "c_gdatfile.h"
#include "c_keybd.h"
#include "c_dialog.h"

#include "emu.h"
#include "regs.h"

// Note: i32 SKW_2066_03e0(i32 eaxl) replace directly

// belongs to DM2_dialog_2066_33e7
// important: min length of eaxtp. eaxtp[5] gets a value.
static void DM2_2066_33c4(t_text* eaxtp, i32 edxl)
{
  c_nreg _RG1; RG1T = eaxtp;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;

  RG2P = RG1P;
  RG3L = RG4L;
  DM2_STRCPY(RG1T, CHGCAST(t_text, v1d10e4));
  RG3Blo += lcon(0x30);
  mov8(location(RG2P + lcon(0x5)), RG3Blo);
}

// belongs to DM2_dialog_2066_33e7
static void DM2_dialog_2066_3820(t_text* eaxtp, i32 edxl)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  t_text* RG5t = eaxtp;
  i32 RG7l;
  c_rect rc_00;
  i16 parw00;

  RG7l = RG4L;
  RG4R = &rc_00;
  DM2_QUERY_EXPANDED_RECT(453, RG4R);
  t_palette* pal = DM2_QUERY_GDAT_IMAGE_LOCALPAL(26, lcon(0x81), 0);
  RG1L = signedlong(rc_00.y);
  put16(parw00, RG1W);
  RG3L = signedlong(rc_00.x);
  RG2L = 0;
  RG4L = lcon(0x81);
  DM2_DRAW_DIALOGUE_PICT(BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(26, lcon(0x81), 0)), BMPCAST(gfxsys.bitmapptr), &rc_00, RG3W, parw00, lcon(0xffff), pal);
  RG2L = unsignedlong(palettecolor_to_ui8(paldat.palette[E_COL15]));
  RG1W = rc_00.y + 4;
  RG4L = signedlong(RG1W);
  DM2_DRAW_VP_STR(rc_00.x, RG4W, RG2W, RG5t);
  if (RG7l != 0)
  {
    RG1L = DM2_STRLEN(RG5t);
    RG4W = strdat.gfxstrw4;
    RG1L *= RG4L;
    rc_00.x += RG1W;
    rc_00.w = RG4W;
    rc_00.h -= 4;
    RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL11]);
    RG4L = unsignedlong(RG4Blo);
    RG1R = &rc_00;
    DM2_FILL_BACKBUFF_RECT(/*TAG blitrect*/ RG1R, /*TAG pixel256*/ ui8_to_pixel(RG4UBlo));
  }
  DM2_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
}

// belongs to DM2_GAME_SAVE_MENU
i32 DM2_dialog_2066_33e7(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_wlreg _RG5;
  c_wlreg _RG6;
  c_rect rc_00;
  i16 vw_08; // x0
  i16 vw_0c; // y0
  bool flag;

  RG4L = 6;
  DM2_dialog_OPEN_DIALOG_PANEL(lcon(0x81), RG4L);
  RG4W = ddat.savegamew4;
  if (RG4W < 0)
  {
    //m_27910:
    RG6L = 0;
    //m_27941:
    for (;;)
    {
      RG2L = signedlong(RG6W);
      if (RG2L < lcon(0xa))
      {
        //m_27914:
        RG1L = 20 * RG2L;
        RG4L = 2 * (RG2L + RG1L);
        RG1P = DOWNCAST(c_hex2a, ddat.savegamep1);
        if (long_at(RG1P + RG4L + lcon(0x26)) != lcon(0xdeadbeef))
        {
          //m_27940:
          RG6L++;
          continue;
        }
        RG4L = RG2L;
        DM2_2066_33c4(ddat.v1e0900.text, RG4L);
      }
      break;
    }
  }
  else
  {
    RG6L = RG4L;
    RG4L = signedlong(RG4W);
    RG2L = lcon(0x2a);
    DM2_COPY_MEMORY(DOWNCAST(c_hex2a, &ddat.v1e0900), DOWNCAST(c_hex2a, &ddat.savegamep1[RG4L]), sizeof(c_hex2a));
  }
  //m_27949:
  RG4L = signedlong(RG6W);
  RG1L = RG4L;
  DM2_dialog_2066_398a(RG1L);
  RG3T = ddat.v1e0900.text;
  if (RG4L == lcon(0xa))
    //m_27968:
    DM2_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
  else
  {
    RG4L = 0;
    RG1P = RG3P;
    DM2_dialog_2066_3820(RG1T, RG4L);
  }
  //m_2796D:
  flag = false;
  //m_2796F:
  for (;;)
  {
    DM2_EVENT_LOOP();
    RG1W = eventqueue.event_unk06;
    RG1L--;
    if (RG1UW <= mkuw(lcon(0x3)))
    {
      bool skip00643 = false;
      RG1L = unsignedlong(RG1W);
      switch (RG1L)
      {
        case 0:
          //m_27992:
          RG6L = lcon(0xffffffff);
          RG4L = 0;
          ddat.v1e023c = RG4L;
          skip00643 = true;
          break;

        case 1:
          //m_279A9:
          RG1L = 0;
          ddat.v1e023c = RG1L;
          RG4L = signedlong(RG6W);
          if (RG4L != lcon(0xa))
          {
            if (byte_at(RG3P) == 0)
              DM2_2066_33c4(RG3T, RG4L);
            RG4L = 0;
            RG1P = RG3P;
            DM2_dialog_2066_3820(RG1T, RG4L);
            skip00643 = true;
          }
          break;

        case 2:
          //m_279D3:
          ddat.v1e023c = 0;
          RG1L = signedlong(eventqueue.event_unk05);
          RG4R = &rc_00;
          DM2_QUERY_EXPANDED_RECT(RG1W, RG4R);
          DM2_QUERY_TOPLEFT_OF_RECT(eventqueue.event_unk09, OO vw_08, OO vw_0c);
          RG4L = signedlong(mkw(eventqueue.event_unk07 - (rc_00.y + vw_0c)));
          RG2L = signedlong(strdat.strxplus);
          RG1L = signedlong(DM2_MIN(CUTX16(RG4L / RG2L), 10));
          RG6L = RG1L;
          RG4L = signedlong(RG1W);
          RG1P = DOWNCAST(c_hex2a, &ddat.savegamep1[RG4L]);
          if (long_at(RG1P + lcon(0x26)) != lcon(0xdeadbeef))
            //m_27A5D:
            DM2_STRCPY(ddat.v1e0900.text, UPCAST(t_text, RG1P + 2));
          else
            DM2_2066_33c4(ddat.v1e0900.text, RG4L);
          //m_27A6A:
          RG1L = signedlong(RG6W);
          DM2_dialog_2066_398a(RG1L);
          RG4L = 0;
          RG1P = RG3P;
          DM2_dialog_2066_3820(RG1T, RG4L);
          break;

        case 3:
          //m_27A7D:
          RG1L = signedlong(RG6W);
          if (RG1L != lcon(0xa))
          {
            if (ddat.v1e023c == 0)
            {
              RG4L = 1;
              ddat.v1e023c = RG4L;
              DM2_dialog_2066_3820(RG3T, RG4L);
              RG1L = DM2_STRLEN(RG3T);
              RG5L = RG1L;
            }
          }
          break;

        default: throw(THROW_DMABORT);
      }

      if (skip00643)
        //m_2799F:
        flag = true;
    }

    //m_27AA9:
    if (ddat.v1e023c != 0)
    {
      RG1L = signedlong(eventqueue.event_unk0a);
      if (RG1L >= lcon(0xa5) && RG1L <= lcon(0xd6))
      {
        bool skip00641 = false;
        RG1W = ddat.v1e051e;
        if (RG1W != lcon(0xe))
        {
          //m_27AE8:
          if (RG1W != lcon(0x1c))
          {
            //m_27AF7:
            RG1L = signedlong(RG5W);
            if (RG1L >= lcon(0x1f))
              skip00641 = true;
            else
            {
              RG1W = DM2_keybd_476d_05b6(ddat.v1e051e);
              RG2Blo = RG1Blo;
              RG4L = unsignedlong(RG1Blo);
              if (RG4L >= lcon(0x61) && RG4L <= lcon(0x7a))
                RG2Blo -= lcon(0x20);
              RG1L = signedlong(RG5W);
              RG5L++;
              mov8(location(RG3P + RG1L), RG2Blo);
            }
          }
          else
          {
            ddat.v1e023c = 0;
            skip00641 = true;
          }
        }
        else
        {
          if (RG5W <= 0)
            skip00641 = true;
          else
            RG5L--;
        }

        if (!skip00641)
        {
          //m_27B26:
          RG1L = signedlong(RG5W);
          mov8(location(RG3P + RG1L), 0);
        }

        //m_27B2D:
        RG4L = ddat.v1e023c;
        RG1P = RG3P;
        DM2_dialog_2066_3820(RG1T, RG4L);
      }
    }
    //m_27B3A:
    RG2L = 0;
    eventqueue.event_unk06 = RG2W;
    eventqueue.event_unk0a = RG2W;
    if (flag)
    {
      DM2_dialog_2066_37f2();
      return RG6L;
    }
  }
  FOREND
}

// belongs to DM2_dialog_OPEN_DIALOG_PANEL
static void DM2_dialog_2066_38d9(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  c_o32 vo_00; // poke 16 peek 8 + 32 inc16

  RG4L = 0;
  vo_00.poke16(RG4W);
  //m_27E3B:
  for (;;)
  {
    RG2L = signedlong(vo_00.peek16());
    if (RG2L >= lcon(0xa))
      return;
    //m_27DBD:
    RG1Blo = vo_00.peek8() + bcon(0x30);
    ddat.v1e099c = RG1Blo;
    RG1W = DM2_FILE_OPEN(DM2_FORMAT_SKSTR(gdat.filename5, UPCAST(t_text, NULL)));
    RG3W = RG1W;
    RG4L = RG2L;
    RG2L = 42 * RG4L;
    if (RG1W < 0)
    {
      //m_27E0C:
      RG1Blo = ddat.v1e099c;
      v1d5c31[0] = RG1Blo;
      RG1T = ddat.savegamep1[RG4L].text;
      DM2_STRCPY(RG1T, v1d5c31);
      ddat.savegamep1[RG4L].l_26 = lcon(0xdeadbeef);
    }
    else
    {
      RG2P = DOWNCAST(c_hex2a, &ddat.savegamep1[RG4L]);
      DM2_FILE_READ(RG3W, RG2P, lcon(0x2a));
      DM2_FILE_CLOSE(RG3W);
    }
    //m_27E37:
    vo_00.inc16();
  }
  FOREND
}

// was SKW_2066_3697
void DM2_dialog_OPEN_DIALOG_PANEL(i32 eaxl, i32 edxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  i32 RG5l;
  i8* RG7p;
  t_text tarr_00[0x50];
  c_rect rc_50;
  t_text* tp_58;
  i32 vl_5c;

  RG3Blo = RG1Blo;
  vl_5c = RG4L;
  RG2L = lcon(0x400);
  RG4L = 0;
  RG1P = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x1a4));
  ddat.savegamep1 = UPCAST(c_hex2a, RG1P);
  RG5l = unsignedlong(RG3Blo);
  RG3P = ADRESSOF(t_text, tarr_00);
  RG2L = 0;
  RG4L = RG5l;
  RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x1a), RG4Blo, bcon(0x0), RG3T);
  tp_58 = RG1T;
  RG3P = ADRESSOF(t_text, tarr_00 + lcon(0x28));
  RG2L = 1;
  RG4L = RG5l;
  RG1T = DM2_QUERY_GDAT_TEXT(bcon(0x1a), RG4Blo, bcon(0x1), RG3T);
  RG7p = RG1P;
  RG2L = 0;
  RG4L = RG5l;
  t_bmp* bmp = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(26, RG4Blo, 0));
  t_palette* pal;
  if (!dm2_dballochandler.gfxalloc_done)
  {
    //m_27BE0:
    RG1L = DM2_CALC_IMAGE_BYTE_LENGTH(bmp);
    pal = CHGCAST(t_palette, PIXELCAST(bmp) + unsignedlong(RG1W));
  }
  else
  {
    RG2L = 0;
    RG4L = RG5l;
    pal = DM2_QUERY_GDAT_IMAGE_LOCALPAL(26, RG4Blo, RG2Blo);
  }
  //m_27BEC:
  RG4R = &rc_50;
  RG1R = DM2_QUERY_EXPANDED_RECT(4, RG4R);
  RG4R = RG1R;
  DM2_DRAW_DIALOGUE_PARTS_PICT(bmp, RG4R, lcon(0xffffffff), pal);
  DM2_FREE_PICT_ENTRY(PIXELCAST(bmp));
  if (ddat.dialog2 != 0)
    DM2_FADE_SCREEN(1);
  RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL12]);
  RG4L = unsignedlong(RG4Blo);
  DM2_DRAW_VP_RC_STR(lcon(0x1c2), RG4W, CHGCAST(t_text, v1d10eb));
  RG4UBlo = palettecolor_to_ui8(paldat.palette[E_COL11]);
  RG4L = unsignedlong(RG4Blo);
  DM2_DRAW_VP_RC_STR(lcon(0x1d2), RG4W, tp_58);
  RG4L = unsignedlong(palettecolor_to_ui8(paldat.palette[E_COL11]));
  RG2P = RG7p;
  DM2_DRAW_VP_RC_STR(lcon(0x1d3), RG4W, RG2T);
  DM2_dialog_2066_38d9();
  RG1L = vl_5c;
  ddat.v1e0204 = RG1W;
  RG1L = DM2_1031_0675(4);
  RG4L = 0;
  ddat.v1e092a = RG4W;
  //m_27C9A:
  for (;;)
  {
    if (Tmouse.is_visible())
      return;
    //m_27C8E:
    DM2_SHOW_MOUSE();
    ddat.v1e092a++;
  }
  FOREND
}

void DM2_dialog_2066_37f2(void)
{
  c_nreg _RG1;
  c_nreg _RG4;

  //m_27CAE:
  for (;;)
  {
    RG4W = ddat.v1e092a;
    RG4L--;
    ddat.v1e092a = RG4W;
    if (RG4W == lcon(0xffffffff))
      break;
    DM2_HIDE_MOUSE();
  }
  //m_27CCA:
  RG1L = DM2_1031_06a5();
  gfxsys.backbuff2 = 1;
  dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(lcon(0x1a4));
}

i32 DM2_dialog_2066_398a(i32 eaxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  i16 RG6w;
  i16 vw_00;
  c_rect rc_04;

  vw_00 = RG1W; // truncated
  RG4R = &rc_04;
  DM2_QUERY_EXPANDED_RECT(451, RG4R);
  RG6w = rc_04.y + 4; // adj
  RG5w = wcon(0x0);
  //m_27EC6:
  for (;;)
  {
    RG1L = signedlong(RG5w);
    if (RG1L >= lcon(0xa))
      return RG1L;
    //m_27E72:
    if (RG5w != vw_00)
      //m_27E82:
      RG1UBlo = palettecolor_to_ui8(paldat.palette[E_COL11]);
    else
      RG1UBlo = palettecolor_to_ui8(paldat.palette[E_COL15]);
    //m_27E8A:
    RG2L = unsignedlong(RG1Blo);
    RG1L = 20 * signedlong(RG5w);
    RG4L = 42 * signedlong(RG5w);
    RG3T = ddat.savegamep1[signedlong(RG5w)].text;
    RG2L = unsignedlong(RG2W);
    DM2_DRAW_VP_STR(rc_04.x, RG6w, RG2W, RG3T);
    RG6w += strdat.strxplus;
    RG5w++;
  }
  FOREND
}
