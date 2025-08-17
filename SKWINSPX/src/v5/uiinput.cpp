#include <util.h>
#include <dm2data.h>
#include <startend.h>
#include <skgdtqdb.h>
#include <skeventq.h>
#include <uikeybd.h>
#include <uitmouse.h>
#include <skhero.h>
#include <uiinput.h>
#include <skevents.h>
#include <uibutton.h>
//#include <SKHERO.h>
#include <xtypes.h>
#include <sfxsnd.h>
#include <gfxstr.h>
#include <sksvgame.h>
#include <skguidrw.h>
#include <gfxmain.h>
#include <skmove.h>
#include <SK1031.h>
#include <SK0AAF.h>

#include <emu.h>
#include <regs.h>

#include <dm2debug.h>

#include <skwinapp.h>

extern SkWinApp* skWinApplication;

/*const*/ i8 table1d3efd[236] =
{
  0x86, 0x85, 0x47, 0x01, 0x03, 0x03, 0xff, 0x46, 0x00, 0x04, 0x03, 0x07, 0x01, 0x03, 0x02, 0xff, 0x82, 0x47, 0x01, 0x03, 0x05, 0xff, 0x46, 0x00, 0x04, 0x03, 0x07, 0x01, 0x03, 0x04, 0xff, 0x82,
  0x47, 0x01, 0x03, 0x07, 0xff, 0x46, 0x00, 0x04, 0x03, 0x07, 0x01, 0x03, 0x06, 0xff, 0x82, 0x47, 0x01, 0x03, 0x09, 0xff, 0x46, 0x00, 0x04, 0x03, 0x07, 0x01, 0x03, 0x08, 0xff, 0x82, 0x47, 0x01,
  0x03, 0x0b, 0xff, 0x46, 0x00, 0x04, 0x03, 0x07, 0x01, 0x03, 0x0a, 0xff, 0x82, 0x47, 0x01, 0x03, 0x0d, 0xff, 0x46, 0x00, 0x04, 0x03, 0x07, 0x01, 0x03, 0x0c, 0xff, 0x82, 0x49, 0x00, 0x00, 0x06,
  0x85, 0x49, 0x01, 0x00, 0x06, 0x85, 0x48, 0x00, 0x00, 0x06, 0x85, 0x48, 0x01, 0x00, 0x06, 0x85, 0x48, 0x02, 0x00, 0x06, 0x85, 0x48, 0x03, 0x00, 0x06, 0x85, 0x47, 0x01, 0x04, 0x0f, 0xff, 0x00,
  0x06, 0x85, 0x47, 0x01, 0x04, 0x11, 0xff, 0x00, 0x04, 0x85, 0x47, 0x01, 0x04, 0x13, 0xff, 0x00, 0x04, 0x85, 0x4a, 0x00, 0x06, 0x85, 0x47, 0x01, 0x05, 0x0a, 0xff, 0x00, 0x06, 0x85, 0x4b, 0x00,
  0x06, 0x86, 0x4c, 0x00, 0x00, 0x06, 0x86, 0x4c, 0x01, 0x00, 0x06, 0x86, 0x4c, 0x02, 0x00, 0x06, 0x86, 0x4d, 0x02, 0x00, 0x06, 0x86, 0x4d, 0x04, 0x00, 0x06, 0x85, 0x4d, 0x08, 0x00, 0x06, 0x86,
  0x4e, 0x01, 0x05, 0x00, 0x04, 0x8e, 0x00, 0x4e, 0x02, 0x05, 0x00, 0x04, 0x8e, 0x00, 0x4e, 0x04, 0x05, 0x00, 0x04, 0x8e, 0x00, 0x4e, 0x08, 0x05, 0x00, 0x04, 0x8e, 0x00, 0x4f, 0x1a, 0xfe, 0x00,
  0x00, 0x04, 0x85, 0x50, 0x1a, 0xfe, 0x01, 0x00, 0x04, 0x85, 0x06, 0x83
};

c_eventdata eventdata;

void c_eventdata::init(void)
{
	SPX_DEBUG_PRINT("EVENTDATA:INIT\n");
  v1e04d2.init();
  for (i16 i=0; i<3; i++) table1e04e0[i].init();
  eventdata.v_evtable = NULL;
}

void c_uievent::init(void)
{
  x = y = idx = 0;
  r.init();
}

extern SkWinApp* skWinApplication;
// recursive!!!
// belongs to DM2_IBMIO_USER_INPUT_CHECK
static i16 DM2_1031_03f2(s_bbw* eaxp, i16 x)
{
  i16 ret = 0;
  const i8* bptr = DM2_1031_023b(eaxp);
  skWinApplication->skwin_Sleep(1000);
  //m_1054D:
  for (;;)
  {
    s_bbw* rg3 = &table1d3ba0[*bptr & 0x7f];
    if (gate_1031(mkul(eaxp->b_00 & lcon(0x7f)), rg3))
    {
      if (rg3->b_00 >= 0)
      {
        //m_105A6:
        i16 idx = table1d3d23[rg3->w_02].w_04;
        if (idx != -1)
        {
          s_ww2* rg4 = v1d39bc + idx;
          //m_105C9:
          for (;;)
          {
            if (x != rg4->w_02)
            {
              //m_105F6:
              rg4++;
              if (rg4->w_00 >= 0)
                continue;
            }
            else
            {
              s_www* ptr = DM2_1031_024c(rg3);
              ret = rg4->w_00 & lcon(0x7ff);
              DM2_1031_0c58(ret, ptr);
              ddat.v1e051e = x;
            }
            break;
          }
          //m_10603:
          if (ret != 0)
            return ret;
        }
      }
      else
      {
        ret = DM2_1031_03f2(rg3, x);
        if (ret != 0)
          return ret;
      }
    }
    //m_10608:
    bptr++;
    if (*bptr < 0)
      return ret;
  }
}

// was SKW_1031_156f
// belongs to static i8* DM2_TRANSMIT_UI_EVENT(c_uievent* xeaxp)
static void DM2_ADJUST_UI_EVENT(c_uievent* xeaxp)
{
  bool skip00195 = false;

  i16 idx = xeaxp->idx;
  if (idx < 116 || idx > 123)
  {
    //m_119CD:
    if (idx < 95 || idx > 98)
      return;
    i16 h = DM2_GET_PLAYER_AT_POSITION((idx - 95 + ddat.v1e0258) & 0x3);
    if (h >= 0)
    {
      i16 w2;
      if (idx > 96)
        //m_11A1D:
        w2 = xeaxp->y - xeaxp->r.y;
      else
        w2 = xeaxp->r.h + xeaxp->r.y - 1 - xeaxp->y;
      //m_11A25:
      i16 w3;
      if (idx != 96 && idx != 97)
        //m_11A3A:
        w3 = xeaxp->r.x + xeaxp->r.w - 1 - xeaxp->x;
      else
        w3 = xeaxp->x - xeaxp->r.x;
      //m_11A46:
      if (w3 > w2)
      {
        //m_11A50:
        if (party.hero[h].handcooldown[2] == 0)
          skip00195 = true;
      }
      else
      {
        idx -= 79;
        skip00195 = true;
      }
    }
  }
  else
  {
    i16 w4 = idx - 116;
    i16 w6 = ((w4 ^ (w4 & 0xff00)) & 0xff01) != 0 ? 1 : 0;
    i16 h2 = DM2_GET_PLAYER_AT_POSITION((ddat.v1e0258 + w4 / 2) & 0x3);
    if (h2 >= 0)
    {
      if (party.hero[h2].handcooldown[w6] == 0)
      {
        if (DM2_IS_ITEM_HAND_ACTIVABLE(unsignedlong(h2), unsignedlong(party.hero[h2].item[w6]), signedlong(w6)) != 0)
        {
          idx = 2 * h2 + 116 + w6;
          skip00195 = true;
        }
      }
    }
  }

  if (!skip00195)
    //m_11A68:
    idx = 0;

  //m_11A6A:
  xeaxp->idx = idx;
}

// was SKW_1031_0d9e
// belongs to void DM2_IBMIO_USER_INPUT_CHECK(void)
static i8* DM2_TRANSMIT_UI_EVENT(c_uievent* xeaxp)
{
  DM2_ADJUST_UI_EVENT(xeaxp);
  i16 n = xeaxp->idx;
  if (n == 0 || n > 242)
    return NULL;
  if (n < 20 || n >= 66)
  {
    //m_10FBC:
    if (   n != 141
        && (n < 125 || n > 129) 
        && (n < 16 || n > 19)
        && (n < 157 || n > 164)
        && (n < 72 || n > 75)
        && (n < 149 || n > 156)
       )
    {
      //m_11008:
      if (n >= 1 && n <= 6)
        return &table1d3efd[15 * (n - 1) + 2];
      if (n >= 116 && n <= 123)
        return &table1d3efd[5 * ((n - 116) & 0x1) + 92];
      if (n >= 95 && n <= 98)
        return &table1d3efd[5 * (n - 95) + 102];
      if (n == 112)
        return &table1d3efd[122];
      if (n == 93 || n == 94)
        return &table1d3efd[8 * (n - 93) + 130];
      if (n >= 101 && n <= 106)
        return &table1d3efd[146];
      if (n == 107)
        return &table1d3efd[150];
      if (n == 108)
        return &table1d3efd[158];
      if (n >= 113 && n <= 115)
        return &table1d3efd[5 * (n - 113) + 162];
      if (n == 11 || n == 147)
        return &table1d3efd[177];
      if (n == 142)
        return &table1d3efd[182];
      if (n == 140)
        return &table1d3efd[187];
      if (n >= 86 && n <= 89)
        return &table1d3efd[7 * (n - 86) + 192];
      if (n < 219 || n > 222)
      {
        //m_11176:
        if (n == 80)
          return &table1d3efd[234];
        return &table1d3efd[0];
      }
      i16 opt = ddat.v1e0204 - 1;
      if (opt <= 6)
      {
        bool skip00185 = false;
        bool skip00184 = false;
        switch (opt)
        {
          case 0:
          case 1:
          case 6:
            skip00184 = true;
            break;

          case 2:
            //m_1115E:
            if (n < 220)
              skip00184 = true;
            else
              skip00185 = true;
            break;

          case 3:
            skip00185 = true;
            break;

          case 4:
          case 5:
            //m_1116B:
            if (n <= 220)
              skip00185 = true;
            break;

          default: throw(THROW_DMABORT);
        }

        if (skip00185)
          //m_11164:
          return &table1d3efd[220];
        else if (skip00184)
          //m_11157:
          return &table1d3efd[227];
      }
    }
  }

  //m_10FFE:
  return &table1d3efd[1];
}

// rectangle of eaxp seems not to be of interest
static bool DM2_HANDLE_UI_EVENT(c_uievent* eaxp)
{
  t_text tarr_00[40];

  i16 idx = eaxp->idx;
  if ((idx < 3
       || idx > 6
       ||
        (ddat.v1e026e == 0
         && (ddat.v1e025e == 0 || (ddat.v1e0274 != ((idx + ddat.v1e0258 - 3) & 0x3)))
        )
      )
      && //m_11E7D:
      (ddat.v1e025c == 0
        || (idx != 1 && idx != 2 && idx != 80)
      )
      &&
      (
       (idx != 80
        ||
         (ddat.v1e040a == ddat.v1e0270
          && ddat.v1e040c == ddat.v1e0272
          && ddat.v1e0406 == ddat.v1e0266
          && ddat.v1e0402 == ddat.v1e0258
         )
       )
      )
      && ddat.glbEndCounter == 0 //m_11EDF:
     )
  {
    //m_11EFA:
    ddat.glbRefreshViewport = false;
    if (idx == 227)
      DM2_RELEASE_MOUSE_CAPTURES();
    else if (idx == 225) //m_11F17:
      DM2_1031_04F5();
    else if (idx == 1 || idx == 2) //m_11F27:
      DM2_PERFORM_TURN_SQUAD(idx);
    else if (idx >= 3 && idx <= 6) //m_11F42:
      DM2_PERFORM_MOVE(idx);
    else if (idx >= 20 && idx < 66) //m_11F5D:
      DM2_CLICK_ITEM_SLOT(idx - 20);
    else if (idx >= 234 && idx <= 237) //m_11F7F:
      DM2_PUT_ITEM_TO_PLAYER(idx - 234);
    else if (idx >= 125 && idx <= 129) //m_11F9C:
    {
      if (idx != 129)
        //m_11FB1:
        DM2_CHANGE_PLAYER_POS(idx - 125);
      else
        DM2_CHAMPION_SQUAD_RECOMPUTE_POSITION();
      //m_11FBE:
      DM2_107B0();
      //m_11FC3:
      DM2_UPDATE_RIGHT_PANEL(0);
    }
    else if (idx >= 228 && idx <= 233) //m_11FCF:
      DM2_CLICK_MONEYBOX(idx - 228);
    else if (idx == 93 || idx == 94) //m_11FEC:
    {
      i8 dir;
      if (idx != 93)
        //m_12005:
        dir = 1;
      else
        dir = -1;
      //m_1200A:
      party.hero[party.curacthero - 1].absdir = (dir + party.hero[party.curacthero - 1].absdir) & 0x3;
      //m_11FC3:
      DM2_UPDATE_RIGHT_PANEL(0);
    }
    else if (idx >= 16 && idx <= 19) //m_12031:
    {
      DM2_SELECT_CHAMPION_LEADER(signedlong(DM2_GET_PLAYER_AT_POSITION((idx - 16 + ddat.v1e0258) & 0x3)));
      DM2_events_2e62_0cfa(0);
    }
    else if (idx >= 7 && idx <= 11) //m_12067:
    {
      if (ddat.v1e0288 == 0) // was || idx > 11 here...
      {
        //m_120BD:
        i16 w = idx - 7;
        if ((w == 4 || w < party.heros_in_party) && ddat.v1e0288 == 0)
          DM2_guidraw_24a5_1798(w);
      }
      else
        //m_12090:
        DM2_events_2f3f_04ea(signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), signedlong(ddat.v1e0258), signedlong(ddat.v1e0266), 147);
    }
    else if (idx == 82) //m_120F6:
    {
      if (eventqueue.event_heroidx != E_NOHERO)
        //m_120EC:
        DM2_guidraw_24a5_1798(eventqueue.event_heroidx);
    }
    else if (idx == 108) //m_1210F:
      ddat.glbRefreshViewport = DM2_TRY_CAST_SPELL();
    else if (idx == 107) //m_12124:
      DM2_REMOVE_RUNE_FROM_TAIL();
    else if (idx >= 101 && idx <= 106) //m_12134:
      DM2_ADD_RUNE_TO_TAIL(idx - 101);
    else if (idx == 112) //m_1214E:
      DM2_PROCEED_COMMAND_SLOT(-1);
    else if (idx >= 113 && idx <= 115) //m_12163:
      ddat.glbRefreshViewport = DM2_PROCEED_COMMAND_SLOT(idx - 113);
    else if (idx >= 116 && idx <= 123) //m_1217A:
    {
      i16 rgw14 = idx - 116;
      DM2_ACTIVATE_ACTION_HAND(rgw14 / 2, rgw14 & 0x1);
    }
    else if (idx >= 95 && idx <= 98) //m_121BB:
    {
      i16 pl = DM2_GET_PLAYER_AT_POSITION((idx - 95 + ddat.v1e0258) & 0x3);
      if (pl >= 0)
        DM2_SET_SPELLING_CHAMPION(pl);
    }
    else if (idx == 70) //m_121F7:
      DM2_PLAYER_CONSUME_OBJECT(-1, lcon(0xffff), -1);
    else if (idx == 71) //m_12213:
      DM2_CLICK_INVENTORY_EYE();
    else if (idx == 72) //m_12223:
    {
      if (ddat.v1e0976 != 0)
        DM2_hero_39796(&party.hero[ddat.v1e0976 - 1]);
    }
    else if (idx == 80) //m_1225D:
      DM2_CLICK_VWPT(unsignedlong(eaxp->x), unsignedlong(eaxp->y));
    else if (idx == 240) //m_12278:
      //m_12283:
      DM2_events_13262(1);
    else if (idx == 241) //m_1228D:
      //m_12283:
      DM2_events_13262(3);
    else if (idx == 242) //m_1229A:
      //m_12283:
      DM2_events_13262(2);
    else if (idx == 85) //m_122A7:
      DM2_CLICK_MAGICAL_MAP_AT(idx, signedlong(eaxp->x), signedlong(eaxp->y));
    else if (idx >= 86 && idx <= 89) //m_122C2:
      DM2_CLICK_MAGICAL_MAP_RUNE(idx - 86);
    else if (idx == 144) //m_122E0:
    {
      ddat.v1e0248 = 0;
      DM2_events_38c8_0002();
      DM2_FILL_ENTIRE_PICT(gfxsys.bitmapptr, palettecolor_to_pixel(paldat.palette[E_COL00]));
      t_text* tp = DM2_QUERY_GDAT_TEXT(1, 0, 18, tarr_00);
      DM2_DRAW_VP_RC_STR(6, unsignedword(palettecolor_to_ui8(paldat.palette[E_COL04])), tp);
      DM2_DRAWINGS_COMPLETED();
      DM2_1031_0675(3);
      eventqueue.event_1031_098e();
      DM2_SOUND1();
    }
    else if (idx == 145) //m_12357:
    {
      if (ddat.v1e0248 == 0)
      {
        ddat.v1e0248 = 1;
        DM2_events_38c8_0060();
        DM2_1031_06a5();
        //m_1237E:
        eventqueue.event_1031_098e();
      }
    }
    else
    {
      //m_12388:
      bool vc3 = ddat.vcapture3;
      if (!vc3 && !ddat.vcapture2 && !ddat.vcapture1 && ddat.v1e03a8 == 0)
      {
        if (idx == 142)
        {
          if (ddat.v1e0288 == 0)
          {
            ddat.v1d6727 = vc3 ? 1 : 0;
            if (ddat.v1e0976 != 0)
              DM2_guidraw_24a5_1798(4);
            ddat.v1d6727 = 1;
            DM2_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
            DM2_events_38c8_0002();
            ddat.v1e0238 = 1;
            if (ddat.v1e025c != 0)
            {
              DM2_PERFORM_MOVE(ddat.v1e0256);
              ddat.v1e025c = 0;
            }
            //m_12429:
            DM2_DRAW_WAKE_UP_TEXT();
            DM2_DRAWINGS_COMPLETED();
            ddat.ticktrig = 1;
            DM2_1031_0541(2);
            //m_1237E:
            eventqueue.event_1031_098e();
          }
        }
        else if (idx == 143) //m_1244D:
          DM2_RESUME_FROM_WAKE();
        else if (idx == 140) //m_1245D:
        {
          if (party.heros_in_party > 0 && ddat.v1e0288 == 0)
          {
            eventqueue.event_unk02--;
            DM2_GAME_SAVE_MENU();
            eventqueue.event_unk02++;
          }
        }
        else if (idx == 141) //m_12497:
        {
          DM2_events_AB26();
          if (ddat.v1e0976 != 0)
            DM2_REFRESH_PLAYER_STAT_DISP(ddat.v1e0976 - 1);
        }
        else if (idx == 146 || idx == 147) //m_124C4:
          //m_12090:
          DM2_events_2f3f_04ea(signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), signedlong(ddat.v1e0258), signedlong(ddat.v1e0266), idx);
        else if (idx == 215) //m_124DB:
          ddat.cd.mo.glbSpecialScreen = _MENU_SCREEN__LOAD_NEW_GAME;
        else if (idx == 216) //m_124EC:
        {
          ddat.cd.mo.glbSpecialScreen = _MENU_SCREEN__LOAD_NEW_GAME;
          ddat.savegamew1 = 1;
        }
        else if (idx == 217) //m_12507:
          ddat.cd.mo.glbSpecialScreen = _MENU_SCREEN__RESUME_GAME_SELECT;
        else if (idx == 218) //m_12518:
          DM2_SHOW_CREDITS();
        else if (idx == 224) //m_12525:
          DM2_PREPARE_EXIT();
      }
    }

    //m_12530:
    if (eventqueue.event_unk0a >= 165 && eventqueue.event_unk0a <= 198)
      ddat.glbRefreshViewport = true;
    return true;
  }
  //m_11EE9:
  ddat.glbRefreshViewport = true;
  return false;
}

// belongs to DM2_EXEC_EVENT
// stacklen was 0x18
static void DM2_0b36_129a(c_buttongroup* buttongroup, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, t_text* argpt1)
{
  c_nreg _RG1;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3L = ecxl;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG6;
  i16 vw_00;
  i16 vw_04;
  i16 vw_08;
  i16 parw00;
  t_text* par_tp02;

  RG6L = RG4L;
  vw_04 = RG2W;
  RG2P = ADRESSOF(i16, &vw_08);
  RG4P = ADRESSOF(i16, &vw_00);
  RG1L = DM2_QUERY_STR_METRICS(argpt1, RG4P16, RG2P16) ? 1 : 0;
  if (RG1L != 0)
  {
    par_tp02 = argpt1;
    i32 parl01 = signedlong(argw0);
    RG1L = signedlong(RG3W);
    put16(parw00, RG1W);
    RG1L = unsignedlong(vw_04);
    RG1W -= buttongroup->button.r.y;
    RG3L = signedlong(RG1W);
    RG1L = RG6L;
    RG1W -= buttongroup->button.r.x;
    RG2L = signedlong(RG1W);
    DM2_DRAW_STRING(PIXELCAST(dm2_dballochandler.DM2_GET_BMP(buttongroup->button.dbidx)), buttongroup->button.r.w, RG2W, RG3W, parw00, CUTX16(parl01), par_tp02);
    RG3L = signedlong(vw_08);
    RG2L = signedlong(vw_00);
    RG4L = signedlong(vw_04);
    RG1L = signedlong(RG6W);
    RG1R = tmprects.alloc_tmprect(RG1W, RG4W, RG2W, RG3W);
    RG4R = RG1R;
    DM2_ADJUST_BUTTONGROUP_RECTS(buttongroup, RG4R);
  }
}

// was SKW_1031_111e, TODO: tell SPX new name
static void DM2_EXEC_EVENT(i16 eaxw)
{
  i8 rg5;
  i8 rg6;
  i8 rg7;
  c_buttongroup buttongroup;
  c_rect rc_34;
  i16 vw_3c;
  i16 vw_40;
  i8 vb_44;

  if (ddat.v1e052e != 0)
  {
    if (eaxw < ddat.v1e052e)
    {
      ddat.v1e052e -= eaxw;
      return;
    }
    ddat.v1e052e = wcon(0x0);
  }
  if (eventqueue.event_unk02 != 0)
    return;
  if (ddat.v1e0478 != 0)
    return;
  if (ddat.v1e048c != 0)
    return;
  if (eventdata.v_evtable == NULL)
    return;
  buttongroup.button.dbidx = NODATA;
  //m_114B3:
  i16 h;
  i16 p;
  t_bmp* bmp;
  i16 eu;

  for (;;)
  {
    vb_44 = *eventdata.v_evtable++;
    i8 c = vb_44 & 0x3f;
    if (c <= 16)
    {
      bool skip00193 = false;
      bool skip00190 = false;
      bool skip00189 = false;
      bool skip00188 = false;
      switch (c)
      {
        case 0:
          //m_114E0:
          ddat.v1e052e = unsignedword(*eventdata.v_evtable++);
          break;

        case 1:
          skip00188 = true;
          break;

        case 2:
          //m_11597:
          ddat.v1e0478 = 1;
          break;

        case 3:
          //m_115A6:
          ddat.v1e048c = 1;
          break;

        case 4:
        case 6:
          //m_114F8:
          eu = eventqueue.glbUIClickEventLast + 1;
          if (eu > 2)
            eu = 0;
          if (eu != eventqueue.glbUIClickEventIndex)
          {
            //m_11530:
            eventdata.table1e04e0[eventqueue.glbUIClickEventLast] = eventdata.v1e04d2;
            eventqueue.glbUIClickEventLast = eu;
            if ((vb_44 & 0x3f) != 0x4)
              skip00188 = true;
          }
          else
          {
            if (eventqueue.event_unk02 == 0)
              eventqueue.event_unk02 = 1;
            eventdata.v_evtable--;
            skip00193 = true;
          }
          break;

        case 5:
          //m_115B5:
          if (!DM2_HANDLE_UI_EVENT(&eventdata.v1e04d2))
            //m_115D1:
            eventdata.v_evtable--;
          else if (ddat.glbRefreshViewport == true)
            skip00188 = true;
          break;

        case 7:
          //m_115DC:
          rg5 = *eventdata.v_evtable++;
          rg7 = *eventdata.v_evtable++;
          rg6 = *eventdata.v_evtable++;
          bmp = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(rg5, rg7, rg6));
          DM2_1031_10c8(&buttongroup, &rc_34, signedlong(getbmpheader(bmp)->width), signedlong(getbmpheader(bmp)->height));
          bmp = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(rg5, rg7, rg6));
          DM2_DRAW_ICON_PICT_BUFF(bmp, &buttongroup, /*TAG blitrect*/ &rc_34, /*TAG srcx*/ 0, 0, /*TAG alphamask*/ signedword(*eventdata.v_evtable++), BLITMODE0, DM2_QUERY_GDAT_IMAGE_LOCALPAL(rg5, rg7, rg6));
          break;

        case 8:
          //m_1166A:
          p = DM2_GET_PLAYER_AT_POSITION((unsignedword(*eventdata.v_evtable++) + ddat.v1e0258) & 0x3); // cast necessary!
          if (p >= 0)
          {
            DM2_DRAW_SQUAD_SPELL_AND_LEADER_ICON(p, 1);
            DM2_gfxmain_0b36_0cbe(&dm2_buttongroup2, true);
          }
          break;

        case 9:
          //m_116B4:
          if (party.curacthero <= 0)
          {
            //m_116C5:
            if (eventdata.v1e04d2.idx >= 116 && eventdata.v1e04d2.idx <= 123)
            {
              //m_116E2:
              h = (eventdata.v1e04d2.idx - 116) / 2;
              skip00190 = true;
            }
            else
              skip00189 = true;
          }
          else
          {
            h = party.curacthero - 1;
            skip00190 = true;
          }
          break;

        case 10:
          //m_11717:
          bmp = BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(1, 5, 8));
          DM2_1031_10c8(&buttongroup, &rc_34, signedlong(getbmpheader(bmp)->width), signedlong(getbmpheader(bmp)->height));
          DM2_DRAW_ICON_PICT_BUFF(BMPCAST(DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(1, 5, 8)), &buttongroup, /*TAG blitrect*/ &rc_34, 0, 0, 0xffff, BLITMODE0, DM2_QUERY_GDAT_IMAGE_LOCALPAL(1, 5, 8));
          ddat.v1e052c[0] = party.hero[party.curacthero - 1].nrunes * 6 + bcon(0x60) + CUTX8(eventdata.v1e04d2.idx) - bcon(0x65);
          DM2_QUERY_STR_METRICS(ddat.v1e052c, &vw_40, &vw_3c);
          rc_34.calc_centered_rect_in_rect(&buttongroup.button.r, vw_40, vw_3c);
          DM2_0b36_129a(&buttongroup, signedlong(rc_34.x), signedlong(rc_34.y + rc_34.h - 1), unsignedlong(palettecolor_to_ui8(paldat.palette[E_COL00])), unsignedword(palettecolor_to_ui8(paldat.palette[E_COL04])), ddat.v1e052c);
          break;

        case 11:
          //m_11811:
          DM2_DRAW_SPELL_TO_BE_CAST(1);
          DM2_gfxmain_0b36_0cbe(&dm2_buttongroup2, true);
          break;

        case 12:
          //m_11820:
          DM2_DRAW_CMD_SLOT(unsignedword(*eventdata.v_evtable++), 1);
          DM2_gfxmain_0b36_0cbe(&dm2_buttongroup2, true);
          break;

        case 13:
          //m_1183F:
          if (ddat.v1e0976 != 0)
          {
            ddat.v1e100c |= unsignedword(*eventdata.v_evtable); // TODO: no ++ in eventdata.v_evtable?
            DM2_REFRESH_PLAYER_STAT_DISP(ddat.v1e0976 - 1);
          }
          skip00189 = true;
          break;

        case 14:
          //m_11869:
          DM2_guidraw_29ee_1d03(*eventdata.v_evtable++);
          DM2_gfxmain_0b36_0cbe(&dm2_buttongroup2, true);
          break;

        case 15:
        case 16:
          //m_11883:
          ddat.v1e0206 = *eventdata.v_evtable++;
          ddat.v1e0207 = *eventdata.v_evtable++;
          ddat.v1e0208 = *eventdata.v_evtable++;
          DM2_0aaf_01db(eventqueue.event_unk05, 1);
          buttongroup.button.dbidx = NODATA;
          DM2_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
          break;

        default: throw(THROW_DMABORT);
      }

      if (skip00193)
        break;

      if (skip00190)
      {
        //m_116EF:
        DM2_DRAW_HAND_ACTION_ICONS(h, unsignedlong(*eventdata.v_evtable++), 1);
        DM2_gfxmain_0b36_0cbe(&dm2_buttongroup2, true);
      }

      if (skip00189)
        //m_116D7:
        eventdata.v_evtable++;

      if (skip00188)
      {
        //m_11568:
        eventqueue.event_unk02 = eventqueue.glbUIClickEventLast - eventqueue.glbUIClickEventIndex;
        if (eventqueue.event_unk02 < 0)
          eventqueue.event_unk02 += 3;
      }
    }

    //m_118D5:
    if ((vb_44 & 0x80) == 0)
    {
      //m_118EC:
      if ((vb_44 & 0x40) != 0)
        continue;
    }
    else
      eventdata.v_evtable = NULL;
    break;
  }
  //m_118FD:
  if (buttongroup.button.dbidx != NODATA)
    DM2_gfxmain_0b36_0cbe(&buttongroup, true);
}

// was SKW_1031_0f3a
// here is the routine that pops values from eventqueue
// in fact that is the 'innerst' routine
void DM2_IBMIO_USER_INPUT_CHECK(void)
{
  static bool semaphore = false;
  c_evententry mentry; // Note: was struct s_3words

	printf("DM2_IBMIO_USER_INPUT_CHECK\n");

  if (close_win)
    throw(THROW_CLOSEWIN);

#ifdef NOTIFY
  if (*notify)
  {
    DM2_DISPLAY_HINT_NEW_LINE();
    DM2_DISPLAY_HINT_TEXT(13, notify);
    *notify = '\0';
  }
#endif

  if (ddat.v1e0484 != 0)
    return;
  i32 gt = ddat.gametime - ddat.last_inputtime;
  if (gt == 0)
    return;
  ddat.last_inputtime = ddat.gametime;
  if (eventqueue.event_unk02 != 0 || ddat.v1e0478 != 0 || ddat.v1e048c != 0)
  {
    DM2_EXEC_EVENT(CUTX16(gt));
    return;
  }
  //m_111E9:
  for (;;)
  {
    if (eventdata.v_evtable != NULL)
    {
      DM2_EXEC_EVENT(CUTX16(gt));
      return;
    }
    //m_111F6:
    while (DM2_HAS_KEY())
      eventqueue.QUEUE_0x20(DM2_GETKEY_TRANSLATED());
    //m_11215:
    eventqueue.fetch_busy = true;
    //m_11223:
    i16 idx;
    do
    {
      if (eventqueue.entries == 0)
      {
        eventqueue.fetch_busy = false;
        eventqueue.PROCESS_SINGLEEVENT();
        DM2_EXEC_EVENT(CUTX16(gt));
        return;
      }

      //m_11240:
      DM2_COPY_MEMORY(DOWNCAST(c_evententry, &mentry), DOWNCAST(c_evententry, &eventqueue.data[eventqueue.out_idx]), sizeof(c_evententry));
      eventqueue.entries--;
      if (++eventqueue.out_idx >= EVENTQUEUELEN)
        eventqueue.out_idx = 0;

      idx = 0;
      if (mentry.b < 0x20)
      {
        //m_112CA:
        if ((mentry.b & 0x4) == 0)
        {
          //m_112DE:
          if ((mentry.b & 0x13) != 0)
            idx = CUTX16(DM2_1031_030a(&table1d3ed5[ddat.v1d3ff1], signedlong(mentry.x), signedlong(mentry.y), signedlong(mentry.b)));
        }
        else
          idx = 227;
      }
      else
      {
        if (mentry.b != 0x20)
        {
          //m_112B2:
          if (mentry.b != 0x40)
          {
            //m_112BE:
            if (mentry.b == 0x60)
              idx = 225;
          }
          else
            idx = 129;
        }
        else
          idx = DM2_1031_03f2(&table1d3ed5[ddat.v1d3ff1], mentry.x);
      }
    } while (idx == 0);

    eventdata.v1e04d2.idx = idx;
    eventdata.v1e04d2.x = eventqueue.event_unk08;
    eventdata.v1e04d2.y = eventqueue.event_unk07;
    eventdata.v1e04d2.r.x = glblrects.dm2rect4.x;
    eventdata.v1e04d2.r.y = glblrects.dm2rect4.y;
    eventdata.v1e04d2.r.w = glblrects.dm2rect4.w;
    eventdata.v1e04d2.r.h = glblrects.dm2rect4.h;

    eventqueue.fetch_busy = false;
    eventqueue.PROCESS_SINGLEEVENT();
    eventdata.v_evtable = DM2_TRANSMIT_UI_EVENT(&eventdata.v1e04d2);
    gt = 0;
  }
}

// id: 0x1255A
// was SKW_1031_1e6a
void DM2_EVENT_LOOP(void)	// = MAIN_LOOP
{
static i16 oldmb = -1, oldmx = -1, oldmy = -1;

  if (ddat.glbEndCounter != 0)
    ddat.glbEndCounter = ddat.glbEndCounter - 1;
  ddat.v1e0478 = 0;
  //m_1257C:
  for (;;)
  {
	  // SPX MessageLoop --> Check Mouse pos then force screen redraw
	  // Note gfxsys.dm2mscreen is screen with mouse; gfxsys.dm2screen is screen without mouse
	  {
		  U16 mice_x, mice_y, mice_btn;
		skWinApplication->skwin_Sleep(8);	// 125 fps
		skWinApplication->GetMousePosButtons(&mice_x, &mice_y, &mice_btn);
		
/*			c_evententry me;
			me.x = mice_x;
			me.y = mice_y;
			me.b = mice_btn & 0x3;

			if (me.x != oldmx || me.y != oldmy || me.b != oldmb)
			{
			  while (!Tmouse.T1_driver_mouseint(me))
				Tmouse.command_interpreter(); // HOTTAG
			}
			oldmb = me.b; oldmx = me.x; oldmy = me.y;

		i16 DRVW_mouseb = me.b ^ DRVW_oldmb;
		if (DRVW_mouseb & 0x1)
		  T1_queue_event(me.x, me.y, (me.b & 0x1) ? 2 : 4);
		if (DRVW_mouseb & 0x2)
		  T1_queue_event(me.x, me.y, (me.b & 0x2) ? 1 : 8);
*/
			U16 glbMouseButtonState = 0;
			U16 iCheckButton = 0;
			U16 iNewEventButtonValue = 0;
			U16 iOldButtonState = 0;
			glbMouseButtonState = ((mice_btn >> 1) & 1) | ((mice_btn << 1) & 2);
			iCheckButton = mice_btn ^ iOldButtonState;
			//if (iCheckButton != 0 /*&& _04bf_03c6 != 0*/) {
			if (iCheckButton != 0) {
				if ((iCheckButton & 1) != 0) { // left click ?
					iNewEventButtonValue = ((mice_btn & 1) != 0) ? 2 : 4;
				}
				if ((iCheckButton & 2) != 0) { // right click ?
					iNewEventButtonValue = ((mice_btn & 2) != 0) ? 1 : 8;
				}
				iOldButtonState = mice_btn;
			}
		//if (iCheckButton != 0)
		Tmouse.T1_queue_event(mice_x, mice_y, iNewEventButtonValue);	// not this one
		Tmouse.T1_drawmouse();
		unsigned char* pVideoSource = (unsigned char*) gfxsys.dm2mscreen; // gfxsys.dm2mscreen
		skWinApplication->renderScreen((X8*)pVideoSource, 0, 0, 320, 200);
	  } // SPX

    DM2_IBMIO_USER_INPUT_CHECK();
    if (eventqueue.glbUIClickEventIndex == eventqueue.glbUIClickEventLast)
      return;

    U16 iUIClickNext = eventqueue.glbUIClickEventIndex++;
    if (eventqueue.glbUIClickEventIndex > 2)
      eventqueue.glbUIClickEventIndex = 0;
    if (!DM2_HANDLE_UI_EVENT(eventdata.table1e04e0 + iUIClickNext))
      eventqueue.glbUIClickEventIndex = iUIClickNext;
    else if (eventqueue.event_unk02 != 0)
      eventqueue.event_unk02--;

    if (ddat.glbRefreshViewport != 0)
      return;
  }
}
