#include "dm2data.h"
#include "c_mouse.h"
#include "util.h"
#include "fileio.h"
#include "c_alloc.h"
#include "c_dballoc.h"
#include "c_savegame.h"


// needs:
x16 SKW_0aaf_0067(x8 eaxb);
x8 SKW_0aaf_02f8(x8 eaxb, x8 edxb);
bool SKW_2066_03e0(x16 eaxw);
void SKW__OPEN_DIALOG_PANEL(x8 eaxb, x16 edxw);
void SKW_2066_37f2(void);
void SKW_2066_398a(x16 eaxw);
void SKW_PROCESS_ITEM_BONUS(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
void SKW_2fcf_0b8b(x16 eaxw, x16 ebxw, x16 edxw);
void SKW___SORT_TIMERS(void);
void SKW_3a15_020f(void);
void SKW_PROCESS_TIMER_0E(c_tim* eaxptim, x16 edxw);
x16 SKW_476d_030a(x16 eaxw);
bool SKW_476d_04af(x16 eaxw);
bool SKW_READ_DUNGEON_STRUCTURE(bool eaxbool);
x16 SKW_ALLOC_NEW_RECORD(x16 eaxw);
void SKW_APPEND_RECORD_TO(x16 eaxw, x16 ebxw, x16 ecxw, x16* edxpw);
c_record* SKW_GET_ADDRESS_OF_RECORD(ui16 eaxw);
x16 SKW_GET_TILE_RECORD_LINK(x16 eaxw, x16 edxw);
x16 SKW_GET_NEXT_RECORD_LINK(x16 eaxw);
void SKW_CUT_RECORD_FROM(x16 eaxw, x16 ebxw, x16 ecxw, x16* edxpw);
void SKW_CHANGE_CURRENT_MAP_TO(x16 eaxw);
x16 SKW_QUERY_CREATURE_AI_SPEC_FLAGS(x16 eaxw);
void SKW_ADD_INDEX_TO_POSSESSION_INDICES(x16 eaxw);
void SKW_EVENT_LOOP(void);
void SKW_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN(void); // c_gfx_misc.h
void SKW_CALC_PLAYER_WEIGHT(x16 eaxw);
bool SKW_GET_TELEPORTER_DETAIL(c_5bytes* eaxps, x16 ebxw, x16 edxw);
bool SKW_IS_CONTAINER_MAP(x16 eaxw);
bool SKW_IS_CONTAINER_MONEYBOX(x16 eaxw);
void SKW_FIRE_FADE_SCREEN(bool eaxbool); // c_gfx_pal.h
void SKW_RAISE_SYSERR(x16 eaxw);
void SKW_PROCESS_ACTUATOR_TICK_GENERATOR(void);
t_text* SKW_FORMAT_SKSTR(const t_text* eaxt, t_text* edxt);
void R_148D(x16 dummy);
// for save:
void SKW_FILL_ORPHAN_CAII(void);
x16 SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS(void);
void SKW_38c8_0002(void);
void SKW_UPDATE_RIGHT_PANEL(bool eaxbool);
x16 SKW_2066_33e7(void);
void SKW_1c9a_0fcb(x16 eaxw);
void SKW_END_GAME(bool eaxbool);
void SKW_38c8_0060(void);
void SKW_REARRANGE_TIMERLIST(void);

static bool SKW_LOAD_NEW_DUNGEON(void)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("SKW_LOAD_NEW_DUNGEON");
  x16 wordrg11 = 0;
  if (!ddata.savegamew1)
    goto M_27131;
  wordrg11 = CUTX16(DM2_FILE_OPEN(SKW_FORMAT_SKSTR(ddata.gdatfilename3, NULL)));
  ddata.filehandle1 = wordrg11;
  if (wordrg11 >= con(0x0))
    goto M_27148;
M_27131:
  ddata.filehandle1 = CUTX16(DM2_FILE_OPEN(SKW_FORMAT_SKSTR(ddata.gdatfilename4, NULL)));
M_27148:
  if (ddata.filehandle1 < con(0x0))
    SKW_RAISE_SYSERR(con(0x32));
  ddata.savegamew2 = con(0x0);
  ddata.savegamewpc.w0 = con(0xffffffff);
  ddata.longsavegamel1 = con(0x0);
SPX_DEBUG_POP;	// one step before..
  return SKW_READ_DUNGEON_STRUCTURE(true);
}

// eaxp is bytebuffer
bool SKW_SKLOAD_READ(xxx* eaxp, x16 edxw)
{
  if (edxw == con(0x0))
    return true;
  return DM2_FILE_READ(ddata.filehandle1, eaxp, unsignedlong(edxw));
}

// stacksize was 0x2c
static bool SKW_READ_1BIT(x32* eaxpl)
{
  x8 vb_00;

  bool boolrg4 = SKW_SUPPRESS_READER(DOWNCAST(x8, &vb_00), con(0x1), con(0x1), &ddata.savegameb1, con(0x1));
  *eaxpl = unsignedlong(vb_00);
  return boolrg4;
}

static x16 SKW_SELECT_LOAD_GAME(void)
{
  c_rect rc_00;
  x16 vw_08; // y0
  x16 vw_0c; // x0

  if (ddata.boolsavegamel2)
    return ddata.savegamew4; // BUGFIX older bug
  SKW__OPEN_DIALOG_PANEL(con(0x80), con(0x5));
  SKW_2066_398a(con(0xffffffff));
  SKW_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
  SKW_FIRE_FADE_SCREEN(false);
  bool boolrg3 = false;
  x16 wordrg4 = con(0xffffffff);
  ddata.mouse_unk06 = con(0xff);
  do
  {
    SKW_EVENT_LOOP();
    if (ddata.mouse_unk06 < con(0x2))
    {
      if (ddata.mouse_unk06 == con(0x1))
      {
        wordrg4 = con(0xffffffff);
        boolrg3 = true;
      }
    }
    else if (ddata.mouse_unk06 == con(0x2))
      boolrg3 = true;
    else if (ddata.mouse_unk06 == con(0x3))
    {
      SKW_QUERY_EXPANDED_RECT(ddata.mouse_unk05, &rc_00);
      SKW_QUERY_TOPLEFT_OF_RECT(ddata.mouse_unk09, &vw_0c, &vw_08);
      wordrg4 = DM2_MIN((ddata.mouse_unk07 - (rc_00.y + vw_08)) / ddata.strxplus, con(0xa));
      if (ddata.savegamep1[wordrg4 / 0x2a].s_02.l_24 == con(0xdeadbeef))
        wordrg4 = con(0xffffffff);
      SKW_2066_398a(wordrg4);
      SKW_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
    }

    ddata.mouse_unk06 = con(0xffffffff);
  } while (!boolrg3);
  SKW_2066_37f2();
  return wordrg4;
}

static bool SKW_2066_197c(void)
{
  c_tim* tptrrg5 = ddata.savegametimarrp;
  x16 wordrg6 = con(0x0);
  for(;;)
  {
    if (signedlong(wordrg6) >= unsignedlong(ddata.savegamew5))
      return false;
    if (tptrrg5->b_04 == con(0x3c) || tptrrg5->b_04 == con(0x3d))
    {
      if (ddata.savegamew7 == con(0x0))
        return true;
      tptrrg5->v.w_08 = con(0xfffffffe);
      if (SKW_READ_RECORD_CHECKCODE(con(0xffffffff), &tptrrg5->v.w_08, false, con(0x0), false))
        return true;
    }
    tptrrg5++;
    wordrg6++;
  }
}

static bool SKW_2066_062b(void)
{
  x32 vl_00; // TODO adress goes to SKW_SUPPRESS_READER, size unknown yet
  x32 vl_04;
  x32 vl_08; // TODO adress goes to SKW_SUPPRESS_READER, size unknown yet

  vl_04 = lextended(ddata.savegamew6);
  x16* ptrrg6 = ddata.savegamepw3;
  for(;;)
  {
    x32 longrg4 = vl_04 - 1;
    vl_04 = longrg4;
    if (CUTX16(longrg4) == con(0xffffffff))
      return false;
    x16 wordrg4 = *ptrrg6++;
    c_record* recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(wordrg4);
    wordrg4 &= con(0x3c00);
    ui32 ulongrg1 = unsignedlong(wordrg4) >> con(0xa);
    if (ulongrg1 < UI32CAST(con(0x9)))
      ;
    else if (ulongrg1 <= UI32CAST(con(0x9)))
    {
      vl_08 = con(0x3ff);
      if (SKW_SUPPRESS_READER(DOWNCAST(x32, &vl_00), con(0x2), con(0x1), DOWNCAST(x32, &vl_08), con(0x1)))
        return true;
      recoptrrg5->u2.w = CUTX16(vl_00) | con(0x1000);
    }
    else if (ulongrg1 == con(0xe))
    {
      vl_08 = con(0x3ff);
      if (SKW_SUPPRESS_READER(DOWNCAST(x32, &vl_00), con(0x2), con(0x1), DOWNCAST(x32, &vl_08), con(0x1)))
        return true;
      recoptrrg5->u2.w = CUTX16(vl_00) | con(0x2400);
    }
  }
}

// was SKW_2066_2b6c
static void SKW_PROCEED_GLOBAL_EFFECT_TIMERS(void)
{
  x32 vl_00;

  c_tim* sptrrg5 = ddata.savegametimarrp;
  DM2_ZERO_MEMORY(&ddata.savegames1, con(0x6));
  vl_00 = con(0x0);
  for(;;)
  {
    if (CUTX16(vl_00) >= ddata.savegamew9)
    {
      x16 wordrg61 = con(0x0);
      while (wordrg61 < ddata.savegamew2)
      {
        SKW_CALC_PLAYER_WEIGHT(wordrg61);
        wordrg61++;
      }
      return;
    }
    x16 wordrg62 = unsignedword(sptrrg5->b_05);
    ui8 ubyterg1 = sptrrg5->b_04;
    if (ubyterg1 < con(0x47))
    {
      if (ubyterg1 < con(0xe))
        ;
      else if (ubyterg1 == con(0xe))
        SKW_PROCESS_TIMER_0E(sptrrg5, con(0x3));
      else if (ubyterg1 == con(0x46))
      {
        x16 wordrg4 = sptrrg5->u.w_06;
        x32 longrg1;
        if (wordrg4 != con(0x0) && (longrg1 = signedlong(wordrg4)) >= con(0xfffffff1) && longrg1 <= con(0xf))
        {
          if (wordrg4 >= con(0x0))
            ddata.savegames1.w0 -= 2 * signedword(table1d6702[longrg1]);
          else
            ddata.savegames1.w0 += signedword(table1d6702[-longrg1]); // longrg1 is negative here
        }
      }
    }
    else if (ubyterg1 <= con(0x47))
      ddata.savegames1.b2++;
    else if (ubyterg1 <= con(0x48))
    {
      x16 wordrg41 = con(0x0);
      x32 longrg2;
      while ((longrg2 = signedlong(wordrg41)) < unsignedlong(ddata.savegamew2))
      {
        if (((con(0x1) << wordrg41) & signedlong(wordrg62)) != 0)
        {
          if (ddata.hero[longrg2].w36 != con(0x0))
            ddata.hero[longrg2].w103 += sptrrg5->u.w_06;
        }
        wordrg41++;
      }
    }
    else if (ubyterg1 == con(0x4b))
    {
      ddata.hero[wordrg62].b1f++;
      ddata.hero[wordrg62].w48 += sptrrg5->u.w_06;
    }

    sptrrg5++;
    vl_00++;
  }
}

static bool SKW_READ_SKSAVE_DUNGEON(void)
{
  const x8* ptrrg7;
  c_5bytes vs_00;
  ui8* p_08;
  x32 vl_0c; // but adress given to SKW_SUPPRESS_READER as xxx*
  x32 vl_10; // but adress given to SKW_SUPPRESS_READER as xxx*
  x16 vw_1c;
  x16 vw_24;
  x8 vb_2c;

  x16 vw_28 = con(0x0);

  for (ui16 vuw_20 = con(0x0); vuw_20 < ddata.savegamew2; vuw_20++)
  {
    x16* wptrrg55 = ddata.hero[vuw_20].warrc3;
    x16 wordrg66 = con(0x0);
    while (wordrg66 < con(0x1e))
    {
      *wptrrg55++ = con(0xfffffffe);
      wordrg66++;
    }
  }
  ddata.savegamewpc.w0 = con(0xfffffffe);
  vw_24 = ddata.v1d3248;

  for (ui16 vuw_28 = con(0x0); vuw_28 < unsignedword(CUTX8(ddata.savegamep4->w0[2])); vuw_28++)
  {
    SKW_CHANGE_CURRENT_MAP_TO(vuw_28);
    
    for (x16 vw_14 = con(0x0); vw_14 < ddata.savegamewa; vw_14++)
    {
      for (x16 vw_18 = con(0x0); vw_18 < ddata.savegamewb; vw_18++)
      {
        x16 wordrg17 = SKW_GET_TILE_RECORD_LINK(vw_14, vw_18);
      M_25CF2:
        x16 wordrg65;
        if ((wordrg65 = wordrg17) == con(0xfffffffe))
          continue;
        if (((wordrg65 & con(0x3c00)) >> con(0xa)) <= con(0x3))
        {
          wordrg17 = SKW_GET_NEXT_RECORD_LINK(wordrg65);
          goto M_25CF2;
        }
        SKW_GET_ADDRESS_OF_RECORD(wordrg65)->w0 = con(0xfffffffe);
        SKW_CUT_RECORD_FROM(wordrg65, vw_14, vw_18, NULL);
      }
    }
  }
  SKW_CHANGE_CURRENT_MAP_TO(vw_24);
  x16 vw_20;
  for ( vw_20 = con(0x4); vw_20 < con(0x10); vw_20++)
  {
    c_record* recoptr = ddata.savegameparr5[vw_20];
    x16* wptrrg51 = &recoptr->w0;
    x16 wordrg41 = unsignedword(table1d280c[vw_20]) / 2;
    x16 wordrg64 = ddata.savegamep4->w0[vw_20 + con(0x6)];
    while (--wordrg64 != con(0xffffffff))
    {
      *wptrrg51 = con(0xffffffff);
      wptrrg51 += signedlong(wordrg41); // TODO: EVIL
    }
  }
  ddata.savegamepw3 = UPCAST(x16, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0xc8)));
  ddata.savegamew6 = con(0x0);
  
  for ( vw_20 = con(0x0); vw_20 < ddata.savegamew2; vw_20++)
  {
    x16* wptrrg52 = ddata.hero[vw_20].warrc3;
    x16 wordrg63 = con(0x0);
    while (wordrg63 < con(0x1e))
    {
      x16* wptrrg2 = wptrrg52++;
      if (SKW_READ_RECORD_CHECKCODE(con(0xffffffff), wptrrg2, false, con(0x0), false))
        goto M_errexit;
      wordrg63++;
    }
  }
  if (SKW_READ_RECORD_CHECKCODE(con(0xffffffff), &ddata.savegamewpc.w0, false, con(0x0), false))
    goto M_errexit;

  for ( vw_20 = con(0x0); vw_20 < ddata.savegamew2; vw_20++)
  {
    x16* wptrrg53 = ddata.hero[vw_20].warrc3;
    x16 wordrg62 = con(0x0);
    while (wordrg62 < con(0x1e))
    {
      if (*wptrrg53 != con(0xfffffffe))
      {
        x32 longrg41 = unsignedlong(*wptrrg53);
        SKW_PROCESS_ITEM_BONUS(vw_20, wordrg62, con(0x0), CUTX16(longrg41));
      }
      else
        *wptrrg53 = con(0xffffffff);
      wordrg62++;
      wptrrg53++;
    }
  }
  if (ddata.savegamewpc.w0 != con(0xfffffffe))
    SKW_PROCESS_ITEM_BONUS(ddata.mouse_unk0e, con(0xffffffff), con(0x0), ddata.savegamewpc.w0);
  else
    ddata.savegamewpc.w0 = con(0xffffffff);

  if (SKW_2066_197c())
    goto M_errexit;
  vw_24 = ddata.v1d3248;

  for ( vw_28 = con(0x0); vw_28 < unsignedword(CUTX8(ddata.savegamep4->w0[2])); vw_28++)
  {
    SKW_CHANGE_CURRENT_MAP_TO(vw_28);
    p_08 = *UPCAST(ui8*, ddata.savegamepp1);
    for (x16 vw_14 = con(0x0); vw_14 < ddata.savegamewa; vw_14++)
    {
      for (x16 vw_18 = con(0x0); vw_18 < ddata.savegamewb; vw_18++)
      {
        bool boolrg5 = false;
        ui8 ubyterg1 = *p_08 >> con(0x5);
        if (ubyterg1 <= con(0x7))
        {
          switch (ubyterg1)
          {
            case 0:
            case 1:
            case 3:
            case 7:
              vb_2c = con(0x0);
            break;
            case 2:
              vb_2c = con(0x8);
            break;
            case 4:
              vb_2c = con(0x7);
            break;
            case 5:
              if (!SKW_GET_TELEPORTER_DETAIL(&vs_00, vw_18, vw_14))
                vb_2c = con(0x8);
              else
              {
                vb_2c = con(0x0);
                if (unsignedlong(vw_28) > unsignedlong(vs_00.b_04))
                  boolrg5 = true;
              }
            break;
            case 6:
              vb_2c = con(0x4);
            break;
            default: throw(THROW_DMABORT);
          }
        }
        if (vb_2c != con(0x0))
        {
          if (SKW_SUPPRESS_READER(DOWNCAST(xxx, p_08), con(0x1), con(0x1), DOWNCAST(x8, &vb_2c), con(0x0)))
            goto M_errexit;
        }
        p_08++; // DANGER
        if (!boolrg5)
        {
          x16 wordrg13 = SKW_GET_TILE_RECORD_LINK(vw_14, vw_18);
          for(;;)
          {
            x16 wordrg61 = wordrg13;
            if (wordrg61 == con(0xfffffffe))
            {
              if (SKW_READ_RECORD_CHECKCODE(signedlong(vw_14), NULL, true, vw_18, true))
                goto M_errexit;
              break;
            }
            x16 wordrg12 = (wordrg61 & con(0x3c00)) >> con(0xa);
            vw_1c = wordrg12;
            x32 longrg42 = unsignedlong(wordrg12);
            ptrrg7 = table1d64db[longrg42];
            if (ptrrg7 != NULL)
            {
              c_record* recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg61);
              c_record* recoptrrg54 = recoptrrg1;
              if (longrg42 == con(0x3))
              {
                x16 wordrg11 = recoptrrg1->u2.w & con(0x7f);
                if (   wordrg11 == con(0x27)
                    || wordrg11 == con(0x1b)
                    || wordrg11 == con(0x1d)
                    || wordrg11 == con(0x41)
                    || wordrg11 == con(0x2c)
                    || wordrg11 == con(0x32)
                    || wordrg11 == con(0x30)
                    || wordrg11 == con(0x2d)
                   )
                {
                  vl_10 = con(0x1ff);
                  if (SKW_SUPPRESS_READER(DOWNCAST(x32, &vl_0c), con(0x2), con(0x1), DOWNCAST(x32, &vl_10), con(0x1)))
                    goto M_errexit;
                  recoptrrg54->u2.w = (recoptrrg54->u2.w & con(0xffff007f)) | CUTX16((vl_0c & con(0x1ff)) << con(0x7));
                }
              }
              if (SKW_SUPPRESS_READER(DOWNCAST(c_record, recoptrrg54), unsignedlong(table1d280c[vw_1c]), con(0x1), DOWNCAST(xxx, ptrrg7), con(0x0)))
                goto M_errexit;
            }
            wordrg13 = SKW_GET_NEXT_RECORD_LINK(wordrg61);
          }
        }
      }
    }
  }
  SKW_CHANGE_CURRENT_MAP_TO(vw_24);
  if (SKW_2066_062b())
    goto M_errexit;
  DM2_DEALLOC_LOBIGPOOL(con(0xc8));
  return false;

M_errexit:
  DM2_DEALLOC_LOBIGPOOL(con(0xc8));
  return true;
}

void SKW_SUPPRESS_INIT(void)
{
  ddata.v1e092f = con(0x0);
  ddata.v1e092e = con(0x0);
}

union u_uwbb
{
  struct
  {
    x8 lo;
    x8 hi;
  } b;
  ui16 uw;
};

bool SKW_SUPPRESS_READER(xxx* eaxp, x32 ebxl, x32 ecxl, const xxx* edxp, x32 argl0)
{
  const x8* p_00;
  x32 vl_04;
  x32 vl_08; // inc32
  x32 vl_0c;
  x32 vl_10; // inc32
  x8 vb_14; // one - byte - filebuffer
  x8 vb_18;

  p_00 = edxp;
  vl_0c = ebxl;
  vl_04 = ecxl;
  x32 longrg5 = con(0x0);
  x8 byterg11 = ddata.v1e092e;
  u_uwbb rg2;
  rg2.b.hi = ddata.v1e092f;
  vl_08 = con(0x0);

  for(;;)
  {
    if (UI32CAST(vl_08) >= UI32CAST(vl_04))
    {
      ddata.v1e092e = byterg11;
      ddata.v1e092f = rg2.b.hi;
      ddata.longv1e08e0 += longrg5;
      return false;
    }
    vl_10 = con(0x0);
    u_uwbb rg3;
    while (UI16CAST(vl_10) < (rg3.uw = UI16CAST(vl_0c)))
    {
      if (argl0 == con(0x0))
        rg3.b.hi = *eaxp;
      else
        rg3.uw = unsignedword(rg3.b.lo);
      x8 byterg12 = *p_00++;
      vb_18 = byterg12;
      if (byterg12 != con(0x0))
      {
        rg2.b.lo = con(0x7);
        x32 longrg6 = unsignedlong(vb_18);
        while (rg2.b.lo >= con(0x0))
        {
          rg3.b.lo = rg2.b.lo;
          if ((longrg6 & (con(0x1) << rg3.b.lo)) != 0)
          {
            if (rg2.b.hi == con(0x0))
            {
              if (!SKW_SKLOAD_READ(DOWNCAST(x8, &vb_14), con(0x1)))
                return true;
              byterg11 = vb_14;
            }
            x8 byterg4 = byterg11 & con(0xffffff80);
            rg3.b.lo = rg2.b.lo;
            x8 byterg13 = con(0x1) << rg3.b.lo;
            byterg11 += byterg11;
            if (byterg4 == con(0x0))
              rg3.b.hi &= ~byterg13;
            else
            {
              byterg11 |= con(0x1);
              rg3.b.hi |= byterg13;
            }
            longrg5++;
            rg2.b.hi++;
            if (rg2.b.hi == con(0x8))
              rg2.uw = unsignedword(rg2.b.lo);
          }
          rg2.b.lo--;
        }
      }
      *eaxp++ = rg3.b.hi;
      vl_10++;
    }
    p_00 -= unsignedlong(rg3.uw);
    vl_08++;
  }
}

// stacksize was 0x48
bool SKW_READ_RECORD_CHECKCODE(x32 eaxl, x16* ebxp, bool ecxbool, x16 edxw, bool argbool0)
{
  bool vbool_00;
  x16* wp_04;
  x16 vw_08;
  x32 vl_10;
  x32 vl_14;
  bool vbool_18;
  x32 vl_1c;
  const x8* p_20;
  x32 vl_28; // for SKW_SUPPRESS_READER
  x8 vb_30; // for SKW_SUPPRESS_READER
  x8 vb_34; // for SKW_SUPPRESS_READER

  x32 vl_24;
  x32 vl_0c = eaxl;
  vw_08 = edxw;
  wp_04 = ebxp;
  vbool_00 = ecxbool;

  for(;;)
  {
    x32 longrg4;
    if (SKW_READ_1BIT(&vl_24))
      return true;
    if (CUTX16(vl_24) == con(0x0))
      return false;
    vb_34 = con(0xf);
    if (SKW_SUPPRESS_READER(DOWNCAST(x8, &vb_30), con(0x1), con(0x1), DOWNCAST(x8, &vb_34), con(0x1)))
      return true;
    ui16 uwordrg6 = unsignedword(vb_30);
    if (!vbool_00 || uwordrg6 == con(0x4))
    {
      longrg4 = con(0x0);
    }
    else
    {
      vb_34 = con(0x3);
      if (SKW_SUPPRESS_READER(DOWNCAST(x8, &vb_30), con(0x1), con(0x1), DOWNCAST(x8, &vb_34), con(0x1)))
        return true;
      longrg4 = unsignedlong(vb_30);
    }
    if (unsignedlong(uwordrg6) == con(0xf) && ddata.v1d6525)
    {
      vl_28 = con(0x7f);
      if (SKW_SUPPRESS_READER(DOWNCAST(x32, &vl_24), con(0x2), con(0x1), DOWNCAST(x32, &vl_28), con(0x1)))
        return true;
      if (wp_04 == NULL)
        return false;
      *wp_04 = CUTX16(vl_24) | con(0xff80);
      return false;
    }
    vl_14 = unsignedlong(uwordrg6);
    longrg4 <<= con(0xe);
    x16 vw_2c = (SKW_ALLOC_NEW_RECORD(CUTX16(vl_14)) & con(0xffff3fff)) | CUTX16(longrg4);
    vl_1c = unsignedlong(vw_2c);
    SKW_APPEND_RECORD_TO(CUTX16(vl_1c), CUTX16(vl_0c), vw_08, wp_04);
    c_record* recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(CUTX16(vl_1c));
    p_20 = table1d64db[vl_14];
    if (p_20 != NULL)
    {
      vbool_18 = false;
      if (uwordrg6 < con(0x9))
      {
        if (uwordrg6 == con(0x4))
        {
          vb_34 = con(0x7f);
          if (SKW_SUPPRESS_READER(DOWNCAST(x8, &vb_30), con(0x1), con(0x1), DOWNCAST(x8, &vb_34), con(0x1)))
            return true;
          recoptrrg5->u4.b.b0 = vb_30;
          if ((SKW_QUERY_CREATURE_AI_SPEC_FLAGS(CUTX16(vl_1c)) & con(0x1)) != 0)
            p_20 = v1d648f;
        }
      }
      else if (uwordrg6 == con(0x9))
      {
        vb_34 = con(0x3);
        if (SKW_SUPPRESS_READER(DOWNCAST(x8, &vb_30), con(0x1), con(0x1), DOWNCAST(x8, &vb_34), con(0x1)))
          return true;
        recoptrrg5->u4.b.b0 &= con(0xfffffff9);
        recoptrrg5->u4.w |= 2 * vb_30 & con(0x3);
        if (SKW_IS_CONTAINER_MAP(CUTX16(vl_1c)))
        {
          p_20 = v1d64b7;
          vbool_18 = true;
        }
      }
      else if (uwordrg6 == con(0xe))
      {
        if (ddata.v1d6529)
        {
          p_20 = v1d64d3;
          vbool_18 = true;
        }
      }

      if (SKW_SUPPRESS_READER(DOWNCAST(c_record, recoptrrg5), unsignedlong(table1d280c[uwordrg6]), con(0x1), DOWNCAST(xxx, p_20), con(0x0)))
        return true;

      if (uwordrg6 < con(0x9))
      {
        if (uwordrg6 == con(0x4))
        {
          ddata.v1d6529 = true;
          recoptrrg5->u2.w = con(0xfffffffe);
          if (SKW_READ_RECORD_CHECKCODE(con(0xffffffff), &recoptrrg5->u2.w, p_20 != v1d647f, con(0x0), true))
            return true;
          ddata.v1d6529 = false;
        }
      }
      else if (uwordrg6 == con(0x9))
      {
        if (!vbool_18)
        {
          if (SKW_IS_CONTAINER_MONEYBOX(vw_2c))
            table1d64db[0xa] = DOWNCAST(x8, v1d64c3);
          recoptrrg5->u2.w = con(0xfffffffe);
          if (SKW_READ_RECORD_CHECKCODE(con(0xffffffff), &recoptrrg5->u2.w, false, con(0x0), true))
            return true;
          if (SKW_IS_CONTAINER_MONEYBOX(vw_2c))
            table1d64db[0xa] = DOWNCAST(x8, v1d64bf);
        }
        else
        {
          if (SKW_READ_1BIT(&vl_10))
            return true;
          if (vl_10 == con(0x0))
            recoptrrg5->u2.w = con(0xfffffffe);
          else
            SKW_ADD_INDEX_TO_POSSESSION_INDICES(vw_2c);
        }
      }
      else if (uwordrg6 < con(0xe))
        ;
      else if (uwordrg6 == con(0xe))
      {
        if (vbool_18)
          SKW_ADD_INDEX_TO_POSSESSION_INDICES(vw_2c);
        else
        {
          ddata.savegametimarrp[recoptrrg5->u6.w].u.w_06 = vw_2c;
          recoptrrg5->u2.w = con(0xfffffffe);
          ddata.v1d6525 = true;
          if (SKW_READ_RECORD_CHECKCODE(con(0xffffffff), &recoptrrg5->u2.w, vbool_18, con(0x0), vbool_18))
            return true;
          ddata.v1d6525 = false;
        }
      }
      else if (uwordrg6 == con(0xf))
      {
        if (SKW_READ_1BIT(&vl_24))
          return true;
        if (CUTX16(vl_24) != con(0x0))
        {
          vl_28 = con(0x3ff);
          if (SKW_SUPPRESS_READER(DOWNCAST(x32, &vl_24), con(0x2), con(0x1), DOWNCAST(x32, &vl_28), con(0x1)))
            return true;
          ddata.savegametimarrp[CUTX16(vl_24)].v.w_08 = vw_2c;
        }
      }
    }

    if (!argbool0)
      return false;
  }
}

// TODO: return is -1 or 1
x16 SKW_GAME_LOAD(void)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("GAME_LOAD");
  s_hex3c vs_00;
  s_hex30 vs_3c;
  bool first = true;
  bool bypass = false;
  bool boolrg6;

  x16 wordrg11;

  vs_3c.u.l_2c = con(0x0);
  bool boolrg5 = false;
  x8 byterg2;
  if ((byterg2 = CUTX8(ddata.savegamew8)) == con(0x0))
  {
    for(;;)
    {
      if (first)
        ddata.v1e0992 = con(0x2);
      if (!first || ddata.v1d154e || ddata.v1dffd8 == con(0x0) || (unsignedlong(SKW_476d_030a(con(0x2)))) == con(0x1))
      {
        if (!ddata.v1d154e && !SKW_476d_04af(con(0x1)))
        {
          if (ddata.v1dffd6 != con(0x0))
          {
            if (ddata.v1dffd6 != con(0x3))
            {
              if (ddata.v1dffd6 != con(0x2))
              {
                if (ddata.v1dffd6 == con(0x4))
                  byterg2 = con(0x17);
              }
              else
                byterg2 = con(0x16);
            }
            else
              byterg2 = con(0x1a);
          }
          else
            byterg2 = con(0x1d);
        }
        else
        {
          x16 wordrg14 = SKW_SELECT_LOAD_GAME();
          x8 byterg4 = CUTX8(wordrg14);
          if (wordrg14 < con(0x0))
          {
            ddata.savegamew4 = con(0x0);
            return con(0xffffffff);
          }
          ddata.savegamew4 = wordrg14;
          byterg4 += con(0x30);
          ddata.v1e099c = byterg4;
          boolrg6 = false;
          boolrg5 = true;
          x16 wordrg13 = CUTX16(DM2_FILE_OPEN(SKW_FORMAT_SKSTR(ddata.gdatfilename5, NULL)));
          ddata.filehandle1 = wordrg13;
          if (wordrg13 < con(0x0))
          {
            x16 wordrg12 = CUTX16(DM2_FILE_OPEN(SKW_FORMAT_SKSTR(ddata.gdatfilename6, NULL)));
            ddata.filehandle1 = wordrg12;
            if (wordrg12 < con(0x0))
            {
              if (ddata.boolsavegamel2)
                goto M_27330;
              wordrg11 = CUTX16(DM2_FILE_OPEN(SKW_FORMAT_SKSTR(ddata.gdatfilename7, NULL)));
              ddata.filehandle1 = wordrg11;
              if (wordrg11 < con(0x0))
              {
              M_27330:
                boolrg5 = false;
                byterg2 = con(0x1c);
                ddata.v1dffd6 = con(0x3);
              }
              else
              {
                ddata.savegamew8 = con(0x1);
                break; // goto M_2719F;
              }
            }
            else
              boolrg6 = boolrg5;
          }
        }

        if (ddata.v1d154e || unsignedlong(ddata.v1dffd6) == con(0x1))
        {
          ddata.savegamew2 = con(0x0);
          ddata.savegamewpc.w0 = con(0xffffffff);
          SKW_SKLOAD_READ(DOWNCAST(s_hex30, &vs_3c), con(0x2a));
          ddata.savegamew7 = vs_3c.s_00.w_00;
          ddata.longsavegamel1 = con(0x2a);
          ddata.savegamew8 = (!ddata.boolsavegamel2) ? 1 : 0;
          if (!SKW_READ_DUNGEON_STRUCTURE(false)) goto M_exit;
          ddata.savegamew8 = con(0x0);
          SKW_SUPPRESS_INIT();
          if (SKW_SUPPRESS_READER(DOWNCAST(s_hex3c, &vs_00), con(0x3c), con(0x1), table1d631a, con(0x1))) goto M_exit;
          ddata.longmallocx = vs_00.l_00;
          ddata.longv1e021c = vs_00.l_00;
          ddata.ulongrandom = vs_00.l_04;
          ddata.savegamew2 = vs_00.w_08;
          ddata.v1e0270 = vs_00.w_0a;
          ddata.v1e0272 = vs_00.w_0c;
          ddata.v1e0258 = vs_00.w_0e;
          ddata.v1e0266 = vs_00.w_10;
          ddata.mouse_unk0e = vs_00.w_12;
          ddata.savegamew5 = vs_00.w_14;
          ddata.longv1d26a4 = vs_00.l_16;
          ddata.longv1e01a0 = vs_00.l_1a;
          ddata.v1e026e = vs_00.w_1e;
          ddata.v1e025e = vs_00.w_20;
          ddata.v1e0274 = vs_00.w_22;
          ddata.v1d26a0 = vs_00.w_28 & con(0xf);
          ddata.v1d26a2 = ((vs_00.w_28 << con(0x10)) >> con(0x14)) & con(0xf);
          ddata.v1e147f = CUTLX8(vs_00.l_2a);
          ddata.v1e1480 = vs_00.b_2e;
          ddata.v1e1483 = vs_00.b_2f;
          ddata.v1e1482 = vs_00.b_30;
          ddata.v1e147e = vs_00.b_31;
          ddata.v1e147d = vs_00.b_32;
          ddata.v1e1484 = vs_00.b_33;
          ddata.v1e1474 = vs_00.w_34;
          ddata.v1e147b = vs_00.b_36;
          ddata.v1e1478 = vs_00.b_37;
          ddata.longv1e1434 = vs_00.l_38;
          if (SKW_SUPPRESS_READER(ddata.v1e0104, con(0x1), con(0x8), ddata.v1d6316, con(0x1))) goto M_exit;
          if (SKW_SUPPRESS_READER(ddata.globalb, con(0x1), con(0x40), ddata.v1d6316, con(0x1))) goto M_exit;
          if (SKW_SUPPRESS_READER(DOWNCAST(x16, ddata.globalw), con(0x2), con(0x40), ddata.v1d6316, con(0x1))) goto M_exit;
          if (SKW_SUPPRESS_READER(DOWNCAST(c_hero, ddata.hero), sizeof(c_hero), unsignedlong(ddata.savegamew2), table1d6356, con(0x1))) goto M_exit;
          if (SKW_SUPPRESS_READER(DOWNCAST(c_wbbb, &ddata.savegames1), con(0x6), con(0x1), table1d645d, con(0x1))) goto M_exit;
          if (SKW_SUPPRESS_READER(DOWNCAST(c_tim, ddata.savegametimarrp), sizeof(c_tim), unsignedlong(ddata.savegamew5), DOWNCAST(x8, v1d6463), con(0x1))) goto M_exit;

          x16 wordrg4 = ddata.savegamew5;
          while (wordrg4 < ddata.v1e142c)
          {
            ddata.savegametimarrp[wordrg4].b_04 = con(0x0);
            wordrg4++;
          }
          SKW___SORT_TIMERS();
          if (SKW_READ_SKSAVE_DUNGEON()) goto M_exit;
          SKW_PROCEED_GLOBAL_EFFECT_TIMERS();
          SKW_3a15_020f();
          ddata.boolsavegamel2 = true;
          bypass = true;
          break; // goto M_27641;
        }
      }
      else
        R_148D(con(0x2));
      if (ddata.v1d154e)
      {
        if (byterg2 != con(0x0))
          SKW_0aaf_0067(SKW_0aaf_02f8(con(0x0), byterg2) & con(0xff));
      }
      else
      {
        if (byterg2 != con(0x0))
          R_148D(con(0x2));
        if (SKW_0aaf_0067(SKW_0aaf_02f8(con(0xf), byterg2) & con(0xff)) == con(0x1))
        {
          SKW_2066_03e0(con(0x0));
          return con(0xffffffff);
        }
      }
      first = false;
    }
  }

// M_2719F:
  if (!bypass)
  {
    boolrg5 = true;
    vs_3c.u.l_2c = (SKW_2066_03e0(con(0x0)) == false) ? 1 : 0;
    if (!SKW_LOAD_NEW_DUNGEON()) goto M_exit;
  }

// M_27641:
  DM2_FILE_CLOSE(ddata.filehandle1);
  SKW_PROCESS_ACTUATOR_TICK_GENERATOR();
  ddata.gdatfiletype1 = false;
  x32 longrg1;
  if (ddata.savegamew8 == con(0x0) || ddata.boolsavegamel2)
  {
    if (boolrg6) // TODO: uninitialized possible?
      SKW_FILE_RENAME(SKW_FORMAT_SKSTR(ddata.gdatfilename6, NULL), SKW_FORMAT_SKSTR(ddata.gdatfilename5, UPCAST(t_text, NULL)));
    longrg1 = con(0x1);
  }
  else
  {
    if (I16CAST(vs_3c.u.l_2c) != con(0x0))
      SKW_WAIT_SCREEN_REFRESH();
    longrg1 = con(0x0);
  }
  SKW_2066_03e0(CUTX16(longrg1));
  SKW_0aaf_02f8(con(0xe), con(0x0));
  dm2_dballochandler.v1e0200 = true;
  ddata.v1e0250 = false;
  SKW_2fcf_0b8b(ddata.v1e0270, ddata.v1e0266, ddata.v1e0272);
  ddata.v1e0390.l0 = con(0x3);
SPX_DEBUG_POP;
  return con(0x1);

M_exit:
  if (boolrg5)
    DM2_FILE_CLOSE(ddata.filehandle1);
  x8 byterg1;
  if (ddata.boolsavegamel2)
  {
    if (!ddata.gdatfiletype2)
      byterg1 = con(0x7);
    else
      byterg1 = ((!ddata.gdatfiletype1) ? 1 : 0) + con(0x13);
  }
  else
    byterg1 = con(0x0);
  SKW_0aaf_0067(SKW_0aaf_02f8(byterg1, con(0x1f)) & con(0xff));
  if (!ddata.boolsavegamel2)
    SKW_SK_PREPARE_EXIT();
SPX_DEBUG_POP;
  return con(0xffffffff);
}

static bool SKW_SKSAVE_WRITE(xxx* eaxp, x16 edxw)
{
  if (edxw == con(0x0))
    return true;
  return SKW_FILE_WRITE(ddata.filehandle1, eaxp, unsignedlong(edxw));
}

// eaxp and edxp are byte(array)pointers, whatever the original data type is
// routine works like a bytestream
static bool SKW_SUPPRESS_WRITER(xxx* eaxp, x32 ebxl, x32 ecxl, const xxx* edxp)
{
  x32 vl_00;
  x32 vl_04; // inc
  x32 vl_08; // inc
  x32 vl_0c;
  x8 vb_10;
  x8 vba_14[0x4]; // TODO adress used, maybe 4 bytes, maybe 1 only

  vl_0c = ebxl;
  vl_00 = ecxl;
  x32 longrg5 = con(0x0);
  x8 byterg11 = ddata.v1e092e;
  x8 byterg12 = ddata.v1e092f;
  vl_04 = con(0x0);
  for(;;)
  {
    if (UI32CAST(vl_04) >= UI32CAST(vl_00))
    {
      ddata.v1e092e = byterg11;
      ddata.v1e092f = byterg12;
      ddata.longv1e08e0 += longrg5;
      return false;
    }
    vl_08 = con(0x0);
    u_uwbb rg2;
    while (UI16CAST(vl_08) < (rg2.uw = UI16CAST(vl_0c)))
    {
      vb_10 = *eaxp++;
      rg2.b.hi = *edxp++;
      if (rg2.b.hi != con(0x0))
      {
        rg2.b.lo = con(0x7);
        while (rg2.b.lo >= con(0x0))
        {
          x32 longrg3 = con(0x1) << rg2.b.lo;
          if ((unsignedlong(rg2.b.hi) & longrg3) != 0)
          {
            byterg11 += byterg11;
            if ((unsignedlong(vb_10) & longrg3) != 0)
              byterg11 |= con(0x1);
            longrg5++;
            byterg12++;
            if (byterg12 == con(0x8))
            {
              vba_14[0x0] = byterg11;
              if (!SKW_SKSAVE_WRITE(DOWNCAST(x8, vba_14), con(0x1)))
                return true;
              byterg12 = con(0x0);
              byterg11 = con(0x0);
            }
          }
          rg2.b.lo--;
        }
      }
      vl_08++;
    }
    edxp -= unsignedlong(rg2.uw);
    vl_04++;
  }
}

// TODO: parameter can be a byte/bool only, because only one bit used
static bool SKW_WRITE_1BIT(x32 eaxl)
{
  x8 vb_00 = CUTLX8(eaxl & con(0x1));
  return SKW_SUPPRESS_WRITER(&vb_00, con(0x1), con(0x1), &ddata.v1d651b);
}

static bool SKW_SUPPRESS_FLUSH(void)
{
  x8 vb_00;

  x8 byterg3 = ddata.v1e092f;
  if (byterg3 == con(0x0))
    return false;
  x8 byterg11 = con(0x8) - byterg3;
  x8 byterg12 = ddata.v1e092e;
  while (--byterg11 != con(0xffffffff))
    byterg12 = (2 * byterg12) | (((byterg12 & con(0xffffff80)) != con(0x0)) ? 1 : 0);
  vb_00 = byterg12;
  if (!SKW_SKSAVE_WRITE(DOWNCAST(x8, &vb_00), con(0x1)))
    return true;
  ddata.v1e092f = con(0x0);
  ddata.v1e092e = con(0x0);
  ddata.longv1e08e0 += unsignedlong(byterg3);
  return false;
}

static bool SKW_WRITE_POSSESSION_INDICES(void)
{
  x32 vl_00; // TODO could be a word only
  x32 vl_04; // TODO could be a word only

  x16 wordrg6 = ddata.savegamew6;
  x16* wptrrg5 = ddata.savegamepw3;
  do
  {
    if (--wordrg6 == con(0xffffffff))
      return false;
    x16 wordrg4 = *wptrrg5++;
    c_record* recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg4);
    wordrg4 &= con(0x3c00);
    x32 longrg43 = unsignedlong(wordrg4) >> con(0xa);
    x16 wordrg19;
    if (longrg43 < con(0x9))
      continue;
    else if (longrg43 <= con(0x9))
      wordrg19 = ddata.v1e08e4[recoptrrg1->u2.w & con(0x3ff)];
    else if (longrg43 == con(0xe))
      wordrg19 = ddata.v1e08f0[recoptrrg1->u2.w & con(0x3ff)];
    else
      continue;

    vl_00 = lextended(wordrg19);
    vl_04 = con(0x3ff);
  } while (!SKW_SUPPRESS_WRITER(DOWNCAST(x32, &vl_00), con(0x2), con(0x1), DOWNCAST(x32, &vl_04)));
  return true;
}

static void SKW_COMPACT_TIMERLIST(void)
{
  x16 wordrg2 = con(0x0);
  x16 wordrg3 = ddata.savegamew5;
  if (wordrg3 == con(0x0))
    return;
  c_tim* tptrrg4 = ddata.savegametimarrp;
  do
  {
    if (tptrrg4->b_04 == con(0x0))
    {
      c_tim* tptrrg1 = tptrrg4 + 1; // next struct
      while (tptrrg1->b_04 == con(0x0))
        tptrrg1++;
      SKW_COPY_TIMER(tptrrg1, tptrrg4);
      tptrrg1->b_04 = con(0x0);
    }
    tptrrg4++;
    wordrg2++;
  } while (wordrg2 != wordrg3);
  SKW___SORT_TIMERS();
  SKW_3a15_020f();
}

static bool SKW_WRITE_RECORD_CHECKCODE(x16 eaxw, x32 ebxl, x32 edxl)
{
  const x8* ptrrg7;
  x32 vl_00;
  x32 vl_04;
  x32 vl_08;
  x32 vl_0c;
  x32 vl_10; // but adress given as xxx* to SKW_SUPPRESS_WRITER
  x16 vw_14;
  x16 vw_18; // but adress given as xxx* to SKW_SUPPRESS_WRITER, one time truncated, inc32
  x8 vba_1c[0x4]; // array because adr given to SKW_SUPPRESS_WRITER, probably one byte only
  x8 vba_20[0x4]; // array because adr given to SKW_SUPPRESS_WRITER, probably one byte only

  vl_0c = edxl;
  vl_00 = ebxl;
  while (eaxw != con(0xfffffffe) && eaxw != con(0xffffffff))
  {
    ui16 uwordrg1 = (eaxw & con(0x3c00)) >> con(0xa);
    vw_14 = uwordrg1;
    x16 wordrg6 = uwordrg1;
    if (wordrg6 > con(0x3))
    {
      if (SKW_WRITE_1BIT(con(0x1)))
        return true;
      vba_1c[0x0] = CUTX8(vw_14);
      vba_20[0x0] = con(0xf);
      if (SKW_SUPPRESS_WRITER(DOWNCAST(x8, vba_1c), con(0x1), con(0x1), DOWNCAST(x8, vba_20)))
        return true;
      if (vl_0c != con(0x0) && wordrg6 != con(0x4))
      {
        vba_1c[0x0] = CUTX8(eaxw >> con(0xe));
        vba_20[0x0] = con(0x3);
        if (SKW_SUPPRESS_WRITER(DOWNCAST(x8, vba_1c), con(0x1), con(0x1), DOWNCAST(x8, vba_20)))
          return true;
      }
    }
    if (vw_14 == con(0xf) && ddata.v1d651d)
    {
      vw_18 = eaxw;
      vl_10 = con(0x7f);
      if (SKW_SUPPRESS_WRITER(DOWNCAST(x16, &vw_18), con(0x2), con(0x1), DOWNCAST(x32, &vl_10)))
        return true;
      break;
    }
    ptrrg7 = table1d64db[vw_14];
    if (ptrrg7 != NULL)
    {
      vl_04 = unsignedlong(eaxw);
      c_record* recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(CUTX16(vl_04));
      c_record* recoptrrg6 = recoptrrg1;
      vl_08 = con(0x0);
      ui16 uwordrg42 = vw_14;
      if (uwordrg42 < con(0x4))
      {
        if (uwordrg42 == con(0x3))
        {
          x16 wordrg15 = recoptrrg1->u2.w & con(0x7f);
          vw_18 = wordrg15;
          if (   wordrg15 == con(0x27)
              || wordrg15 == con(0x1b)
              || wordrg15 == con(0x1d)
              || wordrg15 == con(0x41)
              || wordrg15 == con(0x2c)
              || wordrg15 == con(0x32)
              || wordrg15 == con(0x30)
              || wordrg15 == con(0x2d)
             )
          {
            vw_18 = recoptrrg6->u2.w >> con(0x7);
            vl_10 = con(0x1ff);
            if (SKW_SUPPRESS_WRITER(DOWNCAST(x16, &vw_18), con(0x2), con(0x1), DOWNCAST(x32, &vl_10)))
              return true;
          }
        }
      }
      else if (uwordrg42 <= con(0x4))
      {
        vba_1c[0x0] = recoptrrg1->u4.b.b0;
        vba_20[0x0] = con(0x7f);
        if (SKW_SUPPRESS_WRITER(DOWNCAST(x8, vba_1c), con(0x1), con(0x1), DOWNCAST(x8, vba_20)))
          return true;
        if ((SKW_QUERY_CREATURE_AI_SPEC_FLAGS(CUTX16(vl_04)) & con(0x1)) != 0)
          ptrrg7 = v1d648f;
        x16 wordrg18 = ddata.v1d6312;
        ddata.v1d6312 = wordrg18 + 1;
        ddata.v1e08e4[UI32CAST(eaxw & con(0x3ff))] = wordrg18;
      }
      else if (uwordrg42 < con(0x9))
        ;
      else if (uwordrg42 <= con(0x9))
      {
        vba_1c[0x0] = CUTX8((recoptrrg1->u4.w << con(0xd)) >> con(0xe));
        vba_20[0x0] = con(0x3);
        if (SKW_SUPPRESS_WRITER(DOWNCAST(x8, vba_1c), con(0x1), con(0x1), DOWNCAST(x8, vba_20)))
          return true;
        if (SKW_IS_CONTAINER_MAP(CUTX16(vl_04)))
        {
          ptrrg7 = v1d64b7;
          vl_08 = con(0x1);
        }
        x16 wordrg17 = ddata.v1d6310;
        ddata.v1d6310 = wordrg17 + 1;
        ddata.v1e08f0[UI32CAST(eaxw & con(0x3ff))] = wordrg17;
      }
      else if (uwordrg42 == con(0xe))
      {
        if (ddata.v1d6521)
        {
          ptrrg7 = v1d64d3;
          vl_08 = con(0x1);
        }
      }
      if (SKW_SUPPRESS_WRITER(DOWNCAST(c_record, recoptrrg6), unsignedlong(table1d280c[vw_14]), con(0x1), DOWNCAST(xxx, ptrrg7)))
        return true;
      ui16 uwordrg4 = vw_14;
      if (uwordrg4 < con(0x9))
      {
        if (uwordrg4 == con(0x4))
        {
          ddata.v1d6521 = true;
          x32 longrg2 = con(0x1);
          if (SKW_WRITE_RECORD_CHECKCODE(recoptrrg6->u2.w, longrg2, (ptrrg7 != v1d647f) ? 1 : 0))
            return true;
          ddata.v1d6521 = false;
        }
      }
      else if (uwordrg4 <= con(0x9))
      {
        if (vl_08 == con(0x0))
        {
          if (SKW_IS_CONTAINER_MONEYBOX(eaxw))
            table1d64db[0xa] = DOWNCAST(x8, v1d64c3);
          if (SKW_WRITE_RECORD_CHECKCODE(recoptrrg6->u2.w, con(0x1), con(0x0)))
            return true;
          if (SKW_IS_CONTAINER_MONEYBOX(eaxw))
            table1d64db[0xa] = DOWNCAST(x8, v1d64bf);
        }
        else
        {
          bool boolrg4;
          if (recoptrrg6->u2.w == con(0xffffffff) || recoptrrg6->u2.w == con(0xfffffffe))
            boolrg4 = false;
          else
            boolrg4 = true;
          if (SKW_WRITE_1BIT(boolrg4 ? 1 : 0))
            return true;
          if (boolrg4)
            SKW_ADD_INDEX_TO_POSSESSION_INDICES(eaxw);
        }
      }
      else if (uwordrg4 < con(0xe))
        ;
      else if (uwordrg4 <= con(0xe))
      {
        if (vl_08 != con(0x0))
          SKW_ADD_INDEX_TO_POSSESSION_INDICES(eaxw);
        else
        {
          ddata.v1d651d = true;
          if (SKW_WRITE_RECORD_CHECKCODE(recoptrrg6->u2.w, con(0x0), con(0x0)))
            return true;
          ddata.v1d651d = false;
        }
      }
      else if (uwordrg4 == con(0xf))
      {
        c_tim* tptrrg19 = ddata.savegametimarrp;
        vw_18 = con(0x0);
      M_24E51:
        if (unsignedlong(vw_18) < signedlong(ddata.savegamew9))
        {
          if (tptrrg19->b_04 != con(0x19) || eaxw != tptrrg19->v.w_08)
          {
            tptrrg19++;
            vw_18++;
            goto M_24E51;
          }
          if (SKW_WRITE_1BIT(con(0x1)))
            return true;
          vl_10 = con(0x3ff);
          if (SKW_SUPPRESS_WRITER(DOWNCAST(x16, &vw_18), con(0x2), con(0x1), DOWNCAST(x32, &vl_10)))
            return true;
        }
        else
        {
          if (SKW_WRITE_1BIT(con(0x0)))
            return true;
        }
      }
    }

    if (vl_00 == con(0x0))
      break;
    eaxw = SKW_GET_NEXT_RECORD_LINK(eaxw);
  }

  if (vl_00 == con(0x0) && eaxw != con(0xffffffff))
    return false;
  if (SKW_WRITE_1BIT(con(0x0)))
    return true;
  return false;
}

static bool SKW_2066_0b44(void)
{
  c_tim* tptrrg3 = ddata.savegametimarrp;
  x16 wordrg5 = con(0x0);
  for(;;)
  {
    if (wordrg5 >= ddata.savegamew5)
      return false;
    if (tptrrg3->b_04 == con(0x3c) || tptrrg3->b_04 == con(0x3d))
    {
      if (SKW_WRITE_RECORD_CHECKCODE(tptrrg3->v.w_08, con(0x0), con(0x0)))
        return true;
    }
    tptrrg3++;
    wordrg5++;
  }
}

static bool SKW_STORE_EXTRA_DUNGEON_DATA(void)
{
  x8* bptrrg7;
  c_5bytes vs_00;
  x32 vl_08;
  x16 vw_14;
  x8 vba_18[0x4]; // TODO maybe one char only
  x8 vba_1c[0x4]; // TODO maybe one char only

  ddata.longv1e08e0 = 0;
  if (SKW_2066_0b44())
    return false;
  vw_14 = ddata.v1d3248;

  for (x16 vw_0c = con(0x0); vw_0c < unsignedword(CUTX8(ddata.savegamep4->w0[2])); vw_0c++)
  {
    SKW_CHANGE_CURRENT_MAP_TO(vw_0c);
    bptrrg7 = ddata.savegamepp1[0];
    x16* wptrrg6 = &ddata.v1e038c[*ddata.v1e03f4];
    for (x16 vw_10 = con(0x0); vw_10 < ddata.savegamewa; vw_10++)
    {
      x16 wordrg51 = con(0x0);
      while (wordrg51 < ddata.savegamewb)
      {
        vba_18[0x0] = *bptrrg7;
        vl_08 = con(0x0);
        ui8 ubyterg1 = UI8CAST(vba_18[0x0]) >> con(0x5);
        bptrrg7++;
        if (ubyterg1 <= con(0x7))
        {
          switch (ubyterg1)
          {
            case 0:
            case 1:
            case 3:
            case 7:
              vba_1c[0x0] = con(0x0);
            break;
            case 2:
              vba_1c[0x0] = con(0x8);
            break;
            case 4:
              vba_1c[0x0] = con(0x7);
            break;
            case 5:
              if (!SKW_GET_TELEPORTER_DETAIL(&vs_00, wordrg51, vw_10))
                vba_1c[0x0] = con(0x8);
              else
              {
                vba_1c[0x0] = con(0x0);
                if (vw_0c > unsignedword(vs_00.b_04))
                  vl_08 = con(0x1);
              }
            break;
            case 6:
              vba_1c[0x0] = con(0x4);
            break;
          }
        }
        if (vba_1c[0x0] != con(0x0))
        {
          if (SKW_SUPPRESS_WRITER(DOWNCAST(x8, vba_18), con(0x1), con(0x1), DOWNCAST(x8, vba_1c)))
            return false;
        }
        x16 wordrg19;
        if ((vba_18[0x0] & con(0x10)) == 0)
          wordrg19 = con(0xfffe);
        else
          wordrg19 = *wptrrg6++;
        if (vl_08 == con(0x0))
        {
          if (SKW_WRITE_RECORD_CHECKCODE(wordrg19, con(0x1), con(0x1)))
            return false;
        }
        wordrg51++;
      }
    }
  }
  SKW_CHANGE_CURRENT_MAP_TO(vw_14);
  return true;
}

static void SKW_1c9a_3bab(void)
{
  x16 wordrg2 = con(0x0);
  while (UI16CAST(ddata.v1d4020) > con(0x0))
  {
    if (ddata.v1e089c[wordrg2].w00 >= con(0x0))
      SKW_1c9a_0fcb(wordrg2);
    wordrg2++;
  }
}

// replaced goto M_25747;  with macro return SUBSAVE(true);
// replaced goto M_25773;  with macro return SUBSAVE(false);
// splitted from M_25747 into the pre-routine FSUBSAVE
// solving SKW_GAME_SAVE_MENU is now much easier.

#define SUBSAVE(x) FSUBSAVE(x, vs_3c.u.tp_2c, boolrg7)

FAKE_VOID FSUBSAVE(bool flag, t_text* txt, x32 val)
{
//M_25747:
  if (flag)
  {
    DM2_FILE_CLOSE(ddata.filehandle1);
    SKW_FILE_DELETE(txt);
    SKW_0aaf_0067(SKW_0aaf_02f8(con(0x0), con(0x1b)) & con(0xff));
  }
//M_25773:
  if (ddata.savegamepw3 != NULL)
    DM2_DEALLOC_LOBIGPOOL(con(0xc8));
  if (ddata.v1e08e4 != NULL)
    DM2_DEALLOC_LOBIGPOOL(2 * unsignedlong(ddata.savegamep4->w0[0xa]));
  if (ddata.v1e08f0 != NULL)
    DM2_DEALLOC_LOBIGPOOL(2 * unsignedlong(ddata.savegamep4->w0[0xf]));
  SKW_FILL_ORPHAN_CAII();
  if (val != con(0x0))
    SKW_END_GAME(false);
  SKW_2066_03e0(con(0x0));
  SKW_38c8_0060();
  SKW_FIRE_SHOW_MOUSE_CURSOR();

  // SPX
  return 0;
}

FAKE_VOID SKW_GAME_SAVE_MENU(void)
{
  s_hex3c vs_00;
  s_hex30 vs_3c; vs_3c.u.tp_2c = NULL;

  bool boolrg7 = false;
  ddata.v1e08fc = false;
  SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
  SKW_UPDATE_RIGHT_PANEL(false);
  SKW_38c8_0002();
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_REARRANGE_TIMERLIST();
  if (!ddata.v1d154e && ddata.v1dffd8 != con(0x0))
    R_148D(con(0x2));
//M_25125:
  ddata.v1e0992 = con(0x2);
//M_25133:
  x16 wordrg17;
  do
  {
    ddata.longv1e08e0 = 0;
    bool boolrg5 = false;
    ddata.v1e08e4 = NULL;
    ddata.v1e08f0 = NULL;
    ddata.savegamepw3 = NULL;
    x8 byterg2 = con(0x0);
  //M_25151:
    do
    {
      if (!ddata.v1d154e && byterg2 != con(0x0))
        R_148D(con(0x2));
    //M_25168:
      x16 wordrg16 = SKW_0aaf_0067(SKW_0aaf_02f8(con(0x1b), byterg2) & con(0xff));
      x32 longrg3 = signedlong(wordrg16);
      bool doM_2519E = true;
      if (longrg3 != con(0x2))
      {
      //M_25190:
        if (longrg3 == con(0x3))
          return SUBSAVE(false);
        if (longrg3 != con(0x1))
        {
        //M_251F7:
          if (wordrg16 != con(0x0))
            ;
          else if (ddata.v1d154e)
          //M_25211:
            boolrg5 = true;
          else
          {
            if (!SKW_476d_04af(con(0x0)))
            {
            //M_25218:
              boolrg5 = false;
              x16 wordrg15 = ddata.v1dffd6;
              if (wordrg15 <= con(0x4))
              {
                switch (wordrg15)
                {
                  case 0:
                  //M_25233:
                    byterg2 = con(0x15);
                  break;
                  case 1:
                  break;
                  case 2:
                  //M_25237:
                    byterg2 = con(0x16);
                  break;
                  case 3:
                  //M_2523F:
                    byterg2 = con(0x1a);
                  break;
                  case 4:
                  //M_2523B:
                    byterg2 = con(0x17);
                  break;
                }
              }
            }
            else
          //M_25211:
              boolrg5 = true;
          }
          doM_2519E = false; // goto M_25241;
        }
      }
      else
        ddata.v1e08fc = true;

    //M_2519E:
      if (doM_2519E)
      {
        x32 longrg11 = ddata.longv1e021c + con(0x64);
        if (   UI32CAST(longrg11) >= UI32CAST(ddata.longmallocx)
            || UI32CAST(ddata.longv1e08f8 + con(0x64)) >= UI32CAST(ddata.longmallocx)
            || SKW_0aaf_0067(SKW_0aaf_02f8(con(0xc), con(0x0)) & con(0xff)) != con(0x3)
           )
        {
        //M_251E0:
          SKW_FIRE_SHOW_MOUSE_CURSOR();
          ddata.v1e13f4 = false;
          boolrg7 = true;
          return SUBSAVE(false);
        }
        byterg2 = con(0x0);
      }

    //M_25241:
    } while (!boolrg5);
    wordrg17 = SKW_2066_33e7();
  } while (wordrg17 < con(0x0));
  ddata.savegamew4 = wordrg17;
  ddata.v1e099c = CUTX8(ddata.savegamew4) + '0';
  SKW_0aaf_02f8(con(0xd), con(0x0));
  t_text* tptrrg5 = SKW_FORMAT_SKSTR(ddata.gdatfilename5, NULL);
  vs_3c.u.tp_2c = tptrrg5;
  t_text* tptrrg3 = SKW_FORMAT_SKSTR(ddata.gdatfilename6, NULL);
  SKW_FILE_DELETE(tptrrg3);
  SKW_FILE_RENAME(tptrrg5, tptrrg3);
  x16 wordrg19 = CUTX16(DM2_FILE_CREATE(tptrrg5));
  ddata.filehandle1 = wordrg19;
  if (wordrg19 < con(0x0))
    return SUBSAVE(true);
  x16 wordrg18 = CUTX16(DM2_FILE_OPEN(tptrrg3));
  if (wordrg18 < con(0x0))
  {
  //M_252E1:
    vs_3c.s_00.s_02.l_24 = con(0x0);
  }
  else
  {
    DM2_FILE_READ(wordrg18, DOWNCAST(s_hex30, &vs_3c), con(0x2a));
    DM2_FILE_CLOSE(wordrg18);
  }
//M_252E7:
  vs_3c.s_00.w_00 = con(0x1);
  SKW_SK_STRCPY(vs_3c.s_00.s_02.t_00, ddata.v1e0900.s_02.t_00);
  SKW_SKSAVE_WRITE(DOWNCAST(s_hex30, &vs_3c), con(0x2a));
  if (!SKW_SKSAVE_WRITE(DOWNCAST(s_sgwords, ddata.savegamep4), con(0x2c)))
    return SUBSAVE(true);
  if (!SKW_SKSAVE_WRITE(DOWNCAST(s_size10, ddata.v1e03c8), unsignedword(CUTX8(ddata.savegamep4->w0[2])) << con(0x4)))
    return SUBSAVE(true);
  if (!SKW_SKSAVE_WRITE(DOWNCAST(x16, ddata.v1e03d8), 2 * ddata.v1e0408))
    return SUBSAVE(true);
  if (!SKW_SKSAVE_WRITE(DOWNCAST(x16, ddata.v1e038c), 2 * ddata.savegamep4->w0[5]))
    return SUBSAVE(true);
  if (!SKW_SKSAVE_WRITE(DOWNCAST(x16, ddata.v1e03d0), 2 * ddata.savegamep4->w0[3]))
    return SUBSAVE(true);
  x16 wordrg62 = con(0x0);
//M_253E1:
  while (wordrg62 < con(0x10))
  {
  //M_253AC:
    if (!SKW_SKSAVE_WRITE(DOWNCAST(c_record, ddata.savegameparr5[wordrg62]), UI32CAST(ddata.savegamep4->w0[wordrg62 + con(0x6)] * table1d280c[wordrg62])))
      return SUBSAVE(true);
    wordrg62++;
  }
  if (!SKW_SKSAVE_WRITE(ddata.v1e03e0, unsignedlong(ddata.savegamep4->w0[1])))
    return SUBSAVE(true);
  SKW_1c9a_3bab();
  SKW_COMPACT_TIMERLIST();
  DM2_ZERO_MEMORY(DOWNCAST(s_hex3c, &vs_00), con(0x3c));
  vs_00.l_00 = ddata.longmallocx;
  vs_00.l_04 = ddata.ulongrandom;
  vs_00.w_08 = ddata.savegamew2;
  vs_00.w_0a = ddata.v1e0270;
  vs_00.w_0c = ddata.v1e0272;
  vs_00.w_0e = ddata.v1e0258;
  vs_00.w_10 = ddata.v1e0266;
  vs_00.w_12 = ddata.mouse_unk0e;
  vs_00.w_14 = ddata.savegamew5;
  vs_00.l_16 = ddata.longv1d26a4;
  vs_00.l_1a = ddata.longv1e01a0;
  vs_00.w_1e = ddata.v1e026e;
  vs_00.w_20 = ddata.v1e025e;
  vs_00.w_22 = ddata.v1e0274;
  vs_00.w_28 = (ddata.v1d26a0 & con(0xf)) | ((ddata.v1d26a2 & con(0xf)) << con(0x4));
  vs_00.l_2a = unsignedlong(ddata.v1e147f);
  vs_00.b_2e = ddata.v1e1480;
  vs_00.b_2f = ddata.v1e1483;
  vs_00.b_30 = ddata.v1e1482;
  vs_00.b_31 = ddata.v1e147e;
  vs_00.b_32 = ddata.v1e147d;
  vs_00.b_33 = ddata.v1e1484;
  vs_00.w_34 = ddata.v1e1474;
  vs_00.b_36 = ddata.v1e147b;
  vs_00.b_37 = ddata.v1e1478;
  vs_00.l_38 = ddata.longv1e1434;
  SKW_SUPPRESS_INIT();
  if (SKW_SUPPRESS_WRITER(DOWNCAST(s_hex3c, &vs_00), con(0x3c), con(0x1), DOWNCAST(x8, table1d631a)))
    return SUBSAVE(true);
  if (SKW_SUPPRESS_WRITER(ddata.v1e0104, con(0x1), con(0x8), ddata.v1d6316))
    return SUBSAVE(true);
  if (SKW_SUPPRESS_WRITER(ddata.globalb, con(0x1), con(0x40), ddata.v1d6316))
    return SUBSAVE(true);
  if (SKW_SUPPRESS_WRITER(DOWNCAST(x16, ddata.globalw), con(0x2), con(0x40), ddata.v1d6316))
    return SUBSAVE(true);
  if (SKW_SUPPRESS_WRITER(DOWNCAST(c_hero, ddata.hero), sizeof(c_hero), unsignedlong(ddata.savegamew2), table1d6356))
    return SUBSAVE(true);
  if (SKW_SUPPRESS_WRITER(DOWNCAST(c_wbbb, &ddata.savegames1), con(0x6), con(0x1), table1d645d))
    return SUBSAVE(true);
  if (SKW_SUPPRESS_WRITER(DOWNCAST(c_tim, ddata.savegametimarrp), sizeof(c_tim), unsignedlong(ddata.savegamew5), DOWNCAST(x8, v1d6463)))
    return SUBSAVE(true);
  ddata.v1e08f0 = UPCAST(x16, DM2_ALLOC_LOBIGPOOL_MEMORY(2 * unsignedlong(ddata.savegamep4->w0[0xf])));
  ddata.v1e08e4 = UPCAST(x16, DM2_ALLOC_LOBIGPOOL_MEMORY(2 * unsignedlong(ddata.savegamep4->w0[0xa])));
  ddata.savegamepw3 = UPCAST(x16, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0xc8)));
  ddata.savegamew6 = con(0x0);
  ddata.v1d6312 = con(0x0);
  ddata.v1d6310 = con(0x0);
  ddata.longv1e08e0 = 0;
  x16 wordrg61 = con(0x0);
//M_256E5:
  x32 longrg13;
  while ((longrg13 = signedlong(wordrg61)) < unsignedlong(ddata.savegamew2))
  {
  //M_256A5:
    x32 longrg4 = longrg13;
    longrg13 = 263 * longrg4;
    x16* wptrrg3 = ddata.hero[longrg4].warrc3;
    x16 wordrg51 = con(0x0);
  //M_256DC:
    while (wordrg51 < con(0x1e))
    {
    //M_256C2:
      x16 wordrg18 = *wptrrg3++;
      if (SKW_WRITE_RECORD_CHECKCODE(wordrg18, con(0x0), con(0x0)))
        return SUBSAVE(true);
      wordrg51++;
    }
    wordrg61++;
  }
  if (SKW_WRITE_RECORD_CHECKCODE(ddata.savegamewpc.w0, con(0x0), con(0x0)))
    return SUBSAVE(true);
  if (!SKW_STORE_EXTRA_DUNGEON_DATA())
    return SUBSAVE(true);
  if (SKW_WRITE_POSSESSION_INDICES())
    return SUBSAVE(true);
  if (SKW_SUPPRESS_FLUSH())
    return SUBSAVE(true);
  DM2_FILE_CLOSE(ddata.filehandle1);
  ddata.savegamew8 = con(0x0);
  ddata.longv1e08f8 = ddata.longmallocx;
  return SUBSAVE(false); // BUGFIX Phase P1 (2nd time...)
}
