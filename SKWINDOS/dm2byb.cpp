#include "addon.h"
#include "driver.h" // DRVW_mousevisicnt
#include "dm2global.h"
#include "dm2data.h"
#include "mdata.h"
#include "src/c_alloc.h"
#include "src/c_dballoc.h"
#include "src/c_mouse.h"
#include "src/c_gfx_pal.h"
#include "src/c_gfx_stretch.h"
#include "src/c_gfx_misc.h"
#include "src/c_gfx_blit.h"
#include "src/c_gfx_str.h"
#include "src/c_gfx_fill.h"
#include "src/c_gfx_decode.h"
#include "src/c_savegame.h"
#include "calls.h"

bool SKW_476d_04af(x16 eaxw)
{
  if (eaxw <= con(0x1))
  {
    if (SKW_476d_030a(con(0x2)) != con(0x1))
      return false;
    return true;
  }
  if (eaxw == con(0x2))
  {
    if (SKW_476d_030a(con(0x2)) != con(0x3))
      return false;
    return true;
  }
  return false;
}

bool R_1539(void)
{
  x16 vw_00;
  x16 vw_04;
  x16 vw_08;

  SKW_443c_08ab(&vw_04, &vw_08, &vw_00);
  if (CUTX8(vw_08) != con(0x0))
    return true;
  if (SKW_476d_05a9() != con(0x0))
    return true;
  return false;
}

// was SKW_443c_0662
void SKW_CHAMPION_SQUAD_RECOMPUTE_POSITION(void)
{
  if (ddata.v1e00b8 != con(0x0))
    SKW_CHANGE_PLAYER_POS(UI16CAST(ddata.v1e00b0 | con(0x8000)));
}

// returning a byte would be enough
x16 SKW_DIR_FROM_5x5_POS(x16 eaxw)
{
  if (eaxw == con(0x6))
    return con(0x0);
  if (eaxw == con(0x8))
    return con(0x1);
  if (eaxw == con(0x12))
    return con(0x2);
  if (eaxw == con(0x10))
    return con(0x3);
  if (eaxw == con(0xc))
    return con(0x4);
  return con(0xffffffff);
}

void SKW_098d_1208(void)
{
  SKW_LOAD_RECTS_AND_COMPRESS(con(0x1), con(0x0), con(0x0));
  ddata.v1e01d4 = true;
}

void SKW_0b36_129a(c_c5rects* eaxps, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, t_text* argpt1)
{
  x16 vw_00;
  x16 vw_08;

  if (!C_strmetrics.SKW_QUERY_STR_METRICS(argpt1, OO vw_08, OO vw_00))
    return;
//                            destbmp
  SKW_DRAW_STRING(SKW_3e74_5817(eaxps->s_00.w_00), edxw - eaxps->s_00.rc_02.x, ebxw - eaxps->s_00.rc_02.y, eaxps->s_00.rc_02.w, ecxw, argw0, argpt1);
  SKW_0b36_0d67(eaxps, SKW_ALLOC_TEMP_RECT(edxw, ebxw, vw_00, vw_08));
}

// was SKW_0b36_1757
void SKW_DRAW_TRANSPARENT_STATIC_PIC(x8 eaxb, x8 ebxb, x16 ecxw, x8 edxb, x16 argw0)
{
  c_image vs_00;

  SKW_QUERY_GDAT_SUMMARY_IMAGE(&vs_00, edxb, ebxb, eaxb);
  vs_00.w_30 = argw0;
  vs_00.w_18 = ecxw;
  SKW_DRAW_PICST(SKW_QUERY_PICST_IT(&vs_00));
}

x8 SKW_0cee_04e5(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0)
{
  x16 vw_00 = ecxw;
  SKW_CALC_VECTOR_W_DIR(eaxw, ebxw, &vw_00, edxw, &argw0);
  return SKW_GET_TILE_VALUE(vw_00, argw0);
}

c_record* SKW_GET_ADDRESS_OF_TILE_RECORD(x16 eaxw, x16 edxw)
{
  return SKW_GET_ADDRESS_OF_RECORD(SKW_GET_TILE_RECORD_LINK(eaxw, edxw));
}

// TODO: casts have to stay here, because it seems that records are overlapping!
c_record* SKW_GET_ADDRESS_OF_RECORD(ui16 eaxw)
{
  ui16 uwordrg1 = (eaxw & con(0x3c00)) >> con(0xa);
  return UPCAST(c_record, DOWNCAST(c_record, ddata.savegameparr5[uwordrg1]) + table1d280c[uwordrg1] * (eaxw & con(0x3ff)));
}

x16 SKW_GET_NEXT_RECORD_LINK(x16 eaxw)
{
  return SKW_GET_ADDRESS_OF_RECORD(eaxw)->w0;
}

void SKW_DEALLOC_RECORD(x16 eaxw)
{
  SKW_GET_ADDRESS_OF_RECORD(eaxw)->w0 = con(0xffffffff);
}

x8 SKW_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(x16 eaxw)
{
  return SKW_QUERY_CLS1_FROM_RECORD(SKW_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(eaxw) << con(0xa));
}

x16 SKW_QUERY_GDAT_FOOD_VALUE_FROM_RECORD(x16 eaxw)
{
  return SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(eaxw, con(0x3));
}

x16 SKW_QUERY_ITEM_WEIGHT(x16 eaxw)
{
  return SKW_QUERY_ITEM_VALUE(eaxw, con(0x1));
}

x16 R_F958(x16 eaxw)
{
  i16 wordrg1 = SKW_QUERY_ITEM_VALUE(eaxw, con(0x2));
  if (wordrg1 > con(0xffff))
    return con(0xffff);
  return wordrg1;
}

c_aispec* SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(x16 eaxw)
{
  return SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(SKW_GET_ADDRESS_OF_RECORD(eaxw)->u4.b.b0);
}

c_aispec* SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(x8 eaxb)
{
  return &table1d296c[SKW_QUERY_GDAT_CREATURE_WORD_VALUE(eaxb, con(0x5))];
}

x16 SKW_QUERY_CREATURE_AI_SPEC_FLAGS(x16 eaxw)
{
  return SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(eaxw)->u0.w;
}

x16 SKW_0cee_2df4(x16 eaxw)
{
  return SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(eaxw)->u1e.w;
}

x16 SKW_0cee_2e09(x16 eaxw)
{
  return SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(eaxw)->w20;
}

// was SKW_0cee_2e1e
// returns a word, even it's just a byte only
x16 SKW_GET_CREATURE_WEIGHT(x16 eaxw)
{
  return unsignedword(SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(eaxw)->u1c.b.b1);
}

x16 SKW_IS_WALL_ORNATE_ALCOVE(x8 eaxb)
{
  if (eaxb == con(0xffffffff))
    return con(0x0);
  return DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0xa), eaxb);
}

bool SKW_0cee_319e(x16 eaxw)
{
  x8 byterg8 = SKW_QUERY_CLS2_FROM_RECORD(eaxw);
  if (byterg8 == con(0xffffffff))
    return false;
  return DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0xb), byterg8);
}

x16 SKW_IS_WALL_ORNATE_SPRING(x16 eaxw)
{
  x8 byterg1 = SKW_QUERY_CLS2_FROM_RECORD(eaxw);
  if (byterg1 == con(0xffffffff))
    return con(0x0);
  return DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0xc), byterg1);
}

x16 SKW_GET_GRAPHICS_FOR_DOOR(x8 eaxb)
{
  return DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xe), con(0xb), con(0xd), eaxb);
}

x16 SKW_0cee_3275(x8 eaxb)
{
  return DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xe), con(0xb), con(0x10), eaxb);
}

// was SKW_0cee_328c
x16 SKW_GET_DOOR_STAT_0X10(x8 eaxb)
{
  return DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xe), con(0xb), con(0xe), eaxb);
}

x16 SKW_QUERY_DOOR_DAMAGE_RESIST(x8 eaxb)
{
  return DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xe), con(0xb), con(0xf), eaxb);
}

// was SKW_1031_0000
bool SKW_RETURN_1(void)
{
  return true;
}

// was SKW_1031_0008
bool SKW_IS_GAME_ENDED(s_bbw* eaxpbbw)
{
  return eaxpbbw->b1 == (ddata.dialog2 ? 1 : 0);
}

bool SKW_1031_0023(s_bbw* eaxpbbw)
{
  return unsignedword(eaxpbbw->b1) == ddata.v1e0204;
}

bool SKW_1031_007b(s_bbw* eaxpbbw)
{
  return ddata.hero[eaxpbbw->b1].w36 != con(0x0);
}

bool SKW_1031_009e(s_bbw* eaxpbbw)
{
  x16 wordrg1 = SKW_GET_PLAYER_AT_POSITION((unsignedword(eaxpbbw->b1) + ddata.v1e0258) & con(0x3));
  return wordrg1 >= con(0x0);
}

bool SKW_1031_012d(s_bbw* eaxpbbw)
{
  if (ddata.v1e0b74 == con(0x0))
    return false;
  if (unsignedlong(eaxpbbw->b1) != unsignedlong(ddata.v1e0b7a))
    return false;
  return true;
}

bool SKW_1031_01ba(s_bbw* eaxpbbw)
{
  return unsignedword(eaxpbbw->b1) == ddata.v1d67bc;
}

x8* SKW_1031_023b(s_bbw* eaxpbbw)
{
  return &table1d3cd0[eaxpbbw->w2];
}

s_www* SKW_1031_024c(s_bbw* eaxpbbw)
{
  x16 wordrg1 = table1d3d23[eaxpbbw->w2].w2;
  if (wordrg1 == con(0xffffffff))
    return NULL;
  return &v1d338c[wordrg1];
}

void SKW_1031_04F5(void)
{
  if (ddata.v1e03a8)
  {
    ddata.v1e03a8 = false;
    SKW_29ee_000f();
  }
}

void R_10777(void)
{
  ddata.v1e0228 = false;
  ddata.v1e0220 = false;
  ddata.v1e0224 = false;
  ddata.v1e03a8 = false;
  ddata.v1e048c = false;
  ddata.v1e0478 = false;
  ddata.v1e0480 = NULL;
  SKW_CHAMPION_SQUAD_RECOMPUTE_POSITION();
  SKW_1031_050C();
}

void R_107B0(void)
{
  SKW_1031_0541(ddata.v1d3ff1);
}

void SKW_1031_0675(x16 eaxw)
{
  ddata.v1e0510 = ddata.v1d3ff1;
  R_10777();
  SKW_1031_0541(eaxw);
}

void SKW_1031_06a5(void)
{
  SKW_1031_0541(ddata.v1e0510);
}

void SKW_1031_0781(x16 eaxw)
{
  c_rect rc_00;

  s_www* sptrrg2 = SKW_1031_06b3(&table1d3ed5[ddata.v1d3ff1], eaxw);
  if (sptrrg2 != NULL && SKW_1031_01d5(sptrrg2->w0[con(0x1)], &rc_00) != NULL)
    SKW_FIRE_QUEUE_MOUSE_EVENT(rc_00.x, rc_00.y, unsignedword(CUTX8(sptrrg2->w0[con(0x2)])));
}

void SKW_1031_10c8(c_c5rects* eaxps, x16 ebxw, x16 ecxw, c_rect* edxr)
{
  if (eaxps->s_00.w_00 == con(0xffffffff))
  {
    SKW_COPY_RECT(&ddata.v1e04d2.r6, &eaxps->s_00.rc_02);
    SKW_0b36_0c52(eaxps, false, con(0xffffffff));
  }
  SKW_CALC_CENTERED_RECT_IN_RECT(edxr, ebxw, ecxw, &eaxps->s_00.rc_02);
}

// was SKW_1031_1907
void SKW_DRAW_WAKE_UP_TEXT(void)
{
  t_text t_00[0x28];

  SKW_FILL_ENTIRE_PICT(ddata.bitmapptr, ddata.paletteptr1[con(0x0)]);
  SKW_DRAW_VP_RC_STR(con(0x6), SKW_QUERY_GDAT_TEXT(con(0x1), con(0x11), t_00, con(0x0)), ddata.paletteptr1[con(0x4)]); // BUGFIX 2/14
}

void SKW_12b4_0092(void)
{
  if (ddata.v1e0534)
    SKW_HIGHLIGHT_ARROW_PANEL(ddata.v1e0548, false, ddata.v1e053e);
}
// id: 0x14A8C
// arguments not longer needed
void SKW_MAIN(void)
{
  SKW_INIT();
// M_14B20:
  SKW_GAME_LOOP();
  SKW_END_GAME(ddata.v1e0250);
// GOTO m_14B20;
}

void SKW_13e4_0329(c_350* eaxps)
{
  if (eaxps == NULL)
    ddata.s350.v1e07ea = con(0x0);
  else
  {
    SKW_COPY_MEMORY(DOWNCAST(c_350, eaxps), con(0x350), DOWNCAST(c_350, &ddata.s350));
    DM2_DEALLOC_LOBIGPOOL(con(0x350));
  }
}

void SKW_SELECT_CREATURE_37FC(void)
{
  if (ddata.s350.v1e0584 == con(0xffffffff))
    ddata.s350.v1e0584 = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(ddata.s350.v1e054e->u4.b.b0, con(0x1));
  ddata.s350.v1e0586 = SKW_14cd_0067(table1d6190[ddata.s350.v1e0584]);
  ddata.s350.v1e0588 = table1d6190[ddata.s350.v1e0584] + ddata.s350.v1e0586;
}

void SKW_14cd_0802(void)
{
  ddata.s350.v1e0556->u.v.b12 = con(0xffffffff);
  ddata.s350.v1e0556->u.v.b13 = con(0x0);
}

void SKW_14cd_18cc(x8 eaxb, s_hexe* ebxphe, x8 edxb)
{
  SKW_14cd_0f3c(con(0x0), getadr_r1d41d6(), con(0x0), ebxphe, con(0x0), con(0xffff), edxb, eaxb);
}

void SKW_14cd_19a4(x8 eaxb, s_hexe* ebxphe, x8 edxb)
{
  SKW_14cd_18f2(eaxb, ebxphe, con(0x0), edxb, con(0xffff));
}

void SKW_14cd_1a3c(x8 eaxb, s_hexe* ebxphe, x8 edxb)
{
  SKW_14cd_19c2(eaxb, ebxphe, con(0x2), edxb, con(0x1));
}

void SKW_14cd_1a5a(x8 eaxb, s_hexe* ebxphe, x8 edxb)
{
  SKW_14cd_19c2(eaxb, ebxphe, con(0x4), edxb, con(0x3));
}

void SKW_14cd_1b74(x8 eaxb, s_hexe* ebxphe, x8 edxb)
{
  SKW_14cd_1a78(eaxb, ebxphe, con(0x1), edxb);
}

void SKW_14cd_1b90(x8 eaxb, s_hexe* ebxphe, x8 edxb)
{
  SKW_14cd_1a78(eaxb, ebxphe, con(0x3), edxb);
}

void SKW_14cd_1c27(x8 eaxb, s_hexe* ebxphe, x8 edxb)
{
  SKW_14cd_1bac(eaxb, ebxphe, con(0x2), edxb, con(0x1));
}

void SKW_14cd_1c45(x8 eaxb, s_hexe* ebxphe, x8 edxb)
{
  SKW_14cd_1bac(eaxb, ebxphe, con(0x4), edxb, con(0x3));
}

void SKW_14cd_1c63(s_hexe* ebxphe, x8 edxb)
{
  x16 wordrg1;

  if (ddata.s350.v1e07d8.b3 != con(0xd))
    wordrg1 = con(0xffff);
  else
    wordrg1 = ddata.s350.v1e07d8.w8;
  SKW_14cd_18f2(con(0x5), ebxphe, con(0x0), edxb, wordrg1);
}

void SKW_14cd_1d42(s_hexe* ebxphe, x8 edxb)
{
  x16 wordrg1;

  if (ddata.s350.v1e07d8.b3 != con(0x5))
    wordrg1 = con(0xffff);
  else
    wordrg1 = ddata.s350.v1e07d8.w8;
  SKW_14cd_18f2(con(0x12), ebxphe, con(0x0), edxb, wordrg1);
}

void SKW_14cd_1e36(x8 eaxb, s_hexe* ebxphe, x8 edxb)
{
  SKW_14cd_1d6c(eaxb, ebxphe, con(0xf), edxb);
}

void SKW_14cd_1e52(x8 eaxb, s_hexe* ebxphe, x8 edxb)
{
  SKW_14cd_1d6c(eaxb, ebxphe, con(0x10), edxb);
}

void SKW_14cd_1f8b(x8 eaxb, s_hexe* ebxphe, x8 edxb)
{
  SKW_14cd_1eec(eaxb, ebxphe, con(0x15), edxb);
}

// was SKW_14cd_221a
void SKW_PROCEED_XACT_79(void)
{
  x16 wordrg2 = SKW_RAND01();
  x8 byterg1 = CUTX8(SKW_RAND02());
  ddata.s350.v1e0556->u.v.b1e = con(0xffffff82);
  ddata.s350.v1e0556->u.v.b1a = ((wordrg2 != con(0x0)) ? 1 : 0) + con(0x27);
  ddata.s350.v1e0556->u.v.b1b = byterg1;
  byterg1 += CUTX8(wordrg2);
  byterg1 &= con(0x3);
  ddata.s350.v1e0556->u.v.b1c = byterg1;
  ddata.s350.v1e0556->u.v.b20 = con(0x0);
}

// was SKW_14cd_226d
x8 SKW_PROCEED_XACT_56(void)
{
  x16 wordrg1 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
  if (SKW_CREATURE_GO_THERE(con(0x80), unsignedword(ddata.s350.v1e0562.u.b.b_07), con(0xffffffff), unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0xffffffff), wordrg1))
    return con(0xfffffffc);
  return con(0xfffffffe);
}

// TODO: smth's wrong here - up/down (try it now!, changed pwa_0c to x16*)
x16 SKW_14cd_2886(x16* eaxpw, x8 ebxb, x16 ecxw, x16 edxw, x16 argw0, x16 argw1)
{
  x16 vwa_00[0x5];
  x16* pwa_0c[0x2];

  vwa_00[0x0] = con(0xffffffff);
  vwa_00[0x1] = edxw;
  vwa_00[0x2] = ecxw;
  vwa_00[0x3] = argw0;
  vwa_00[0x4] = argw1;
  SKW_OVERSEE_RECORD(eaxpw, UPCAST(x16*, DOWNCAST(x16*, pwa_0c)), SKW_14cd_2807, ebxb, vwa_00, con(0x0), con(0x1)); // TODO: smth's wrong here - up/down
  return vwa_00[0x0];
}

// was SKW_14cd_3535
x8 SKW_PROCEED_XACT_81(void)
{
  x16 wordrg4 = ulrshift(ddata.s350.v1e0556->u.v.u18.w, con(0x6), con(0xb));
  SKW_19f0_2813(CUTX8(ddata.s350.v1e07d8.w6) | con(0xffffff80), unsignedword(ddata.s350.v1e0562.u.b.b_07), ddata.s350.v1e0556->u.v.u18.w & con(0x1f), unsignedword(ddata.s350.v1e0562.u.b.b_06), wordrg4, con(0xffffffff), ddata.s350.v1e07d8.w4);
  return ddata.s350.v1e056f;
}

// was SKW_14cd_3abf
x8 SKW_PROCEED_XACT_86(void)
{
  ddata.s350.v1e0556->u.v.b20 = CUTX8(ddata.s350.v1e07d8.w4);
  ddata.s350.v1e0556->u.v.b1e = CUTX8(ddata.s350.v1e07d8.w6);
  ddata.s350.v1e0556->u.v.b1a = CUTX8(ddata.s350.v1e0572) + con(0x3d);
  return con(0xfffffffe);
}

// was SKW_14cd_3aee
void SKW_PROCEED_XACT_72_87_88(void)
{
  x8 byterg8 = CUTX8(ddata.s350.v1e0572);
  if (byterg8 == con(0xffffffff))
    byterg8 = CUTX8(ddata.s350.v1e07d8.w4);
  ddata.s350.v1e0556->u.v.b1a = byterg8;
}

// was SKW_14cd_3b0b
x8 SKW_PROCEED_XACT_89(void)
{
  x8 vb_00 = CUTX8(ddata.s350.v1e07d8.w6);
  ui16 wordrg4 = ulrshift(ddata.s350.v1e0556->u.v.u18.w, con(0x6), con(0xb));
  SKW_19f0_0d10(UI8CAST(vb_00 | con(0xffffff80)), unsignedword(ddata.s350.v1e0562.u.b.b_07), ddata.s350.v1e0556->u.v.u18.w & con(0x1f), unsignedword(ddata.s350.v1e0562.u.b.b_06), wordrg4, con(0xffff));
  return ddata.s350.v1e056f;
}

// was SKW_14cd_3b4c
x8 SKW_PROCEED_XACT_90(void)
{
  return ((ddata.s350.v1e0572 > SKW_RAND16(con(0x64))) ? 1 : 0) - con(0x3);
}

// TODO: if ddata.s350.v1e0556->u.v.b1f is not bool, RG4 can be undefined
bool SKW_1887_0205(void)
{
  bool boolrg4;

  if (ddata.s350.v1e0556->u.v.b1f == con(0x0))
  {
    SKW_1887_05aa();
    boolrg4 = false;
  }
  else if (ddata.s350.v1e0556->u.v.b1f == con(0x1))
  {
    boolrg4 = SKW_CREATURE_WALK_NOW();
  }
  ddata.s350.v1e0556->u.v.b1f += con(0x1);
  return boolrg4;
}

void SKW_1887_0df7(void)
{
  SKW_DELETE_CREATURE_RECORD(unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0x0), con(0x1), unsignedword(ddata.s350.v1e0562.u.b.b_07));
  ddata.s350.v1e0570 = con(0x1);
}

bool SKW_1887_0f80(void)
{
  x16 wordrg1 = SKW_GET_CREATURE_AT(ddata.v1e08a6, ddata.v1e08a4);
  if (wordrg1 == con(0xffffffff))
    return true;
  SKW_ROTATE_CREATURE(wordrg1, unsignedword(ddata.s350.v1e0556->u.v.b1d), false);
  return false;
}

x16 R_1B7D5(void)
{
  return SKW_CREATE_CLOUD(con(0xff8e), unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_07), SKW_RAND16(con(0x28)) + con(0x14), con(0xff));
}

bool R_1BC29(x16 eaxw, x16 edxw)
{
  if (ddata.v1d3248 == ddata.v1e08d6 && eaxw == ddata.v1e08d8 && edxw == ddata.v1e08d4)
    return true;
  return R_1BAAD(eaxw, edxw);
}

x8 SKW_19f0_0547(x16 eaxw, x16 edxw)
{
  return SKW_1c9a_0006(eaxw, edxw);
}

x8 SKW_19f0_1511(x16 eaxw)
{
  return SKW_1c9a_0006(eaxw, con(0x9));
}

x16* SKW_1c9a_02c3(c_record* eaxprec, c_aispec* edxpai)
{
  if ((edxpai->u0.b.b0 & con(0x1)) == con(0x0))
    return &ddata.v1e089c[eaxprec->u4.b.b1].w08;
  return &eaxprec->w8;
}

void SKW_1c9a_0648(x16 eaxw)
{
  if (eaxw == ddata.v1d3248)
    return;
  SKW_CHANGE_CURRENT_MAP_TO(eaxw);
  if (eaxw != ddata.v1e027c)
  {
    ddata.v1e08da = ddata.v1e0258;
    ddata.v1e08d8 = ddata.v1e0270;
    ddata.v1e08d4 = ddata.v1e0272;
    ddata.v1e08d6 = ddata.v1e0266;
  }
  else
  {
    ddata.v1e08da = ddata.v1e0276;
    ddata.v1e08d8 = ddata.v1e0260;
    ddata.v1e08d4 = ddata.v1e0262;
    ddata.v1e08d6 = ddata.v1e027c;
  }
}

// plugin for SKW_OVERSEE_RECORD
bool SKW_1c9a_0694(x16* eaxpw, x16* edxpw)
{
  if (*edxpw == con(0xfffffffe))
    return true;
  if (SKW_GET_DISTINCTIVE_ITEMTYPE(*eaxpw) == *edxpw)
    return true;
  return false;
}

// TODO: I did set v_00 as a single word.
// In fact the routine can be given as parameter for SKW_OVERSEE_RECORD and then
// a wordarray of 5 could be requested.
// Now we don't have 5 words here anyway.
// v_04 is a i8* then, whereby it is not initialized!
x16* SKW_1c9a_06bd(x16 eaxw, x8 ebxb, x16 edxw)
{
  x16* wptrrg4;
  x16 vw_00;
  x16* pw_04;

  vw_00 = edxw;
  wptrrg4 = NULL;
  if (eaxw == con(0xffffffff))
    return wptrrg4;
  wptrrg4 = SKW_OVERSEE_RECORD(&SKW_GET_ADDRESS_OF_RECORD(eaxw)->u2.w, &pw_04, SKW_1c9a_0694, ebxb, &vw_00, unsignedword(ebxb), unsignedword(ebxb));
  if (wptrrg4 == NULL)
    return NULL;
  if (*wptrrg4 == con(0xfffffffe))
    return NULL;
  return wptrrg4;
}

bool SKW_1c9a_08bd(c_record* eaxprec)
{
  s_d556* sd5ptrrg1;

  if (eaxprec->u4.b.b1 == con(0xff))
    return false;
  sd5ptrrg1 = &ddata.v1e089c[eaxprec->u4.b.b1];
  if (sd5ptrrg1->u.v.b1a != con(0x5))
    return false;
  if (sd5ptrrg1->u.v.b1f == con(0x1) || sd5ptrrg1->u.v.b1f == con(0x2))
    return true;
  return false;
}

bool SKW_IS_CREATURE_FLOATING(x16 eaxw)
{
  c_record* recoptrrg4;

  recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(eaxw);
  if ((SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(recoptrrg4->u4.b.b0)->wa & con(0x4)) != con(0x0))
    return true;
  if (!SKW_1c9a_08bd(recoptrrg4) == con(0x0))
    return false;
  return true;
}

bool SKW_1c9a_0958(x16 eaxw)
{
  x16* wptrrg1;
  c_record* recoptrrg2;
  x32 vl_00;

  recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(eaxw);
  wptrrg1 = SKW_1c9a_02c3(recoptrrg2, SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(recoptrrg2->u4.b.b0));
  R_4DEA(recoptrrg2->u4.b.b0, wptrrg1 + con(0x1), &vl_00, *wptrrg1);
  return (vl_00 & con(0x80)) != con(0x0);
}

bool SKW_1c9a_09b9(x16 eaxw, x16 edxw)
{
  return edxw == SKW_GET_ADDRESS_OF_RECORD(eaxw)->w8;
}

void SKW_1c9a_09db(c_record* eaxprec)
{
  x16* wptrrg1;
  t_gfxdata* pg_00;
  t_gfxdata vga_04[0x4];

  pg_00 = vga_04;
  wptrrg1 = SKW_1c9a_02c3(eaxprec, SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(eaxprec->u4.b.b0));
  SKW_GET_CREATURE_ANIMATION_FRAME(eaxprec->u4.b.b0, wptrrg1, wptrrg1 + con(0x1), con(0x11), &pg_00, eaxprec->uc.w);
}

void SKW_RELEASE_MINION(x16 eaxw)
{
  c_record* recoptrrg4 = SKW_GET_MISSILE_REF_OF_MINION(eaxw, con(0xffff));
  if (recoptrrg4 == NULL)
    return;
  x16 wordrg6 = ddata.v1d3248;
  SKW_CHANGE_CURRENT_MAP_TO(urshift(recoptrrg4->u4.w, con(0xa)));
  SKW_13e4_0360(eaxw, ulrshift(recoptrrg4->u4.w, con(0x6), con(0xb)), con(0x13), recoptrrg4->u4.w & con(0x1f), true);
  SKW_CHANGE_CURRENT_MAP_TO(wordrg6);
}

// id: 0x2189F
// adress given to a functionpointer
bool SKW_1c9a_1bae(x16 eaxw, x16 edxw)
{
  if (eaxw == unsignedword(ddata.s350.v1e0562.u.b.b_06) && edxw == unsignedword(ddata.s350.v1e0562.u.b.b_07))
    return false;
  return R_1BAAD(eaxw, edxw);
}

void SKW_ADD_INDEX_TO_POSSESSION_INDICES(x16 eaxw)
{
  ddata.savegamepw3[ddata.savegamew6++] = eaxw;
}

void SKW_2066_1ea3(x16 eaxw, x16 ebxw, x16 edxw)
{
  x8*** ptr1 = &ddata.v1e03cc[ebxw];
  x8** ptr2 = *ptr1 + eaxw;
  x8* ptr3 = *ptr2;
  ptr3[edxw] |= con(0x2);
}

// important: min length of eaxt. eaxt[5] gets a value.
void SKW_2066_33c4(t_text* eaxt, t_text edxt)
{
  SKW_SK_STRCPY(eaxt, v1d10e4);
  eaxt[con(0x5)] = edxt + '0';
}

void SKW_2066_37f2(void)
{
  while (--ddata.v1e092a != con(0xffffffff))
    SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_1031_06a5();
  ddata.backbuff2 = con(0x1);
  DM2_DEALLOC_LOBIGPOOL(con(0x1a4));
}

void SKW_2066_38d9(void)
{
  for (x16 i = 0; i < con(0xa); i++)
  {
    ddata.v1e099c = CUTX8(i) + con(0x30);
    x16 filehandle = CUTX16(DM2_FILE_OPEN(SKW_FORMAT_SKSTR(ddata.gdatfilename5, NULL)));
    if (filehandle < con(0x0))
    {
      v1d5c31[0] = ddata.v1e099c;
      SKW_SK_STRCPY(ddata.savegamep1[i].s_02.t_00, v1d5c31);
      ddata.savegamep1[i].s_02.l_24 = con(0xdeadbeef);
    }
    else
    {
      DM2_FILE_READ(filehandle, DOWNCAST(c_hex2a, &ddata.savegamep1[i]), con(0x2a));
      DM2_FILE_CLOSE(filehandle);
    }
  }
}

void SKW_2405_0009(void)
{
  ddata.savegamewpc.p2 = SKW_ALLOC_PICT_BUFF(ddata.v1d271a, FREEPOOL, con(0x4), ddata.v1d271c);
}

t_text* SKW_QUERY_GDAT_ITEM_NAME(x8 eaxb, x8 edxb)
{
  t_text t_00[0x20];

  return SKW_FORMAT_SKSTR(SKW_QUERY_GDAT_TEXT(eaxb, con(0x18), t_00, edxb), ddata.v1e0930);
}

void SKW_2405_00ec(x16 eaxw, c_rect* edxr)
{
  x16 vw_00;
  x16 vw_04;

  vw_04 = ddata.v1d271a;
  vw_00 = ddata.v1d271c;
  SKW_QUERY_BLIT_RECT(NULL, eaxw, &vw_04, edxr, &vw_00, con(0xffffffff));
}

void SKW_2405_011f(x16 eaxw, c_rect* edxr)
{
  SKW_2405_00ec(eaxw, edxr);
  SKW_INFLATE_RECT(edxr, ddata.v1d2726, ddata.v1d2726);
}

// was SKW_2405_04f4
void SKW_DRAW_ITEM_IN_HAND(c_wpc* eaxpwpc)
{
  x8 vb_0c = SKW_QUERY_CLS1_FROM_RECORD(eaxpwpc->w0);
  x8 vb_08 = SKW_QUERY_CLS2_FROM_RECORD(eaxpwpc->w0);
  x8 byterg7 = SKW_2405_014a(eaxpwpc->w0, true, con(0xffffffff));
  t_gfxdata* srcbmp = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(vb_0c, byterg7, vb_08); // was gptrrg5
  s_gfxdatatail* ptrbeforerg5 = getprevious(srcbmp);
  SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_LOCALPAL(vb_0c, byterg7, vb_08)), PAL16, DOWNCAST(t_gfxdata, eaxpwpc->barr6));
//                      srcbmp                                                          srcofs destbmp                                                                      srcbits    destbits
  SKW_FIRE_BLIT_PICTURE(srcbmp, SKW_ALLOC_TEMP_ORIGIN_RECT(ddata.v1d271a, ddata.v1d271c), 0, eaxpwpc->p2, con(0x0), ptrbeforerg5->width, ddata.v1d271a, con(0xffff), con(0x0), E_BITRES4, E_BITRES4);
}

void SKW_2481_0002(void)
{
  SKW_FIRE_SELECT_PALETTE_SET(0);
  if (!ddata.v1d66f8)
//           srcbmp
    R_C470(ddata.v1e0954, con(0xffffffff), UPCAST(t_palette, ddata.v1d66f4), con(0x2));
//  else
//                          srcbmp                                                                     srcofs  destbmp                                                                               srcbits    destbits
//    SKW_FIRE_BLIT_PICTURE(ddata.v1e0954, SKW_ALLOC_TEMP_ORIGIN_RECT(ddata.mouse_unk0c, ddata.mouse_unk0d), 0, ddata.screenptr, con(0x0), ddata.mouse_unk0c, ddata.mouse_unk0c, con(0xffff), con(0x0), E_BITRES8, E_BITRES8);

  // SPX: forcing this display method gives the 256 colored image
    SKW_FIRE_BLIT_PICTURE(ddata.v1e0954, SKW_ALLOC_TEMP_ORIGIN_RECT(320, 200), 0, ddata.screenptr, con(0x0),
	320, 200, con(0xffff), con(0x0), E_BITRES8, E_BITRES8);
 //		FIRE_BLIT_PICTURE(
//			glbImageMenuScreen, _4976_4964, ALLOC_TEMP_ORIGIN_RECT(glbScreenWidth, glbScreenHeight),
//			0, 0, glbScreenWidth, glbScreenWidth, -1, 0, 8, 8, NULL);

  SKW_FIRE_SELECT_PALETTE_SET(1);
}

void SKW_SHOW_CREDITS(void)
{
  x16 wordrg5 = con(0x708);
  SKW_FIRE_SELECT_PALETTE_SET(0);
  SKW_FIRE_HIDE_MOUSE_CURSOR();
//         srcbmp
  R_C470(ddata.v1e0950, con(0xffffffff), ddata.v1e0958, con(0x2));
  SKW_FIRE_SELECT_PALETTE_SET(1);
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_1031_0541(con(0x1));
  do
  {
    SKW_EVENT_LOOP();
    if (ddata.mouse_unk0a == con(0xef))
      break;
    SKW_SLEEP_SEVERAL_TIME(con(0x1));
  } while (--wordrg5 != con(0x0));
  ddata.savegamew8 = con(0xda);
  SKW_1031_0541(con(0x0));
  SKW_FIRE_HIDE_MOUSE_CURSOR();
}

// was SKW_24a5_0ad2
void SKW_DRAW_CONTAINER_SURVEY(c_record* eaxprec)
{
  x16 wordrg1 = eaxprec->u2.w;
  for (x16 i = 0; wordrg1 != con(0xfffffffe) && i < con(0x8); i++)
  {
    SKW_DRAW_ITEM_ICON(wordrg1, con(0x0), false, i + con(0x2f), false);
    wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg1);
  }
}

// was SKW_24a5_0b0f
void SKW_DRAW_ITEM_STATS_BAR(x16 eaxw, x16 ebxw, x8 ecxb, x16 edxw, x16 argw0)
{
  c_rect rc_00;
  t_text t_08[0x14]; // TODO maybe the string is shorter, internal used largest idx 0x4

  t_08[0x4] = ecxb;
  if (SKW_QUERY_EXPANDED_RECT(eaxw, &rc_00) == NULL)
    return;
  SKW_24a5_0e82(edxw, argw0, con(0x0), eaxw, ebxw, true);
  t_08[0x1] = con(0x0);
  t_08[0x0] = t_08[0x4];
  rc_00.h -= con(0x2);
  s_gfxdatatail* ptrbeforerg11 = getprevious(ddata.bitmapptr);
  SKW_DRAW_STRONG_TEXT(ddata.bitmapptr, rc_00.x - con(0x9), rc_00.y + rc_00.h - con(0x1), ptrbeforerg11->width, unsignedword(ddata.paletteptr1[argw0]), unsignedword(ddata.paletteptr1[con(0x0)]) | con(0x4000), t_08);
  t_08[0x0] = con(0x60);
  s_gfxdatatail* ptrbeforerg12 = getprevious(ddata.bitmapptr);
  SKW_DRAW_STRONG_TEXT(ddata.bitmapptr, rc_00.x + con(0x3), rc_00.y + rc_00.h - con(0x1), ptrbeforerg12->width, unsignedword(ddata.paletteptr1[con(0xf)]), unsignedword(ddata.paletteptr1[con(0x0)]) | con(0x4000), t_08);
  t_08[0x0] = con(0x65);
  s_gfxdatatail* ptrbefore = getprevious(ddata.bitmapptr);
  SKW_DRAW_STRONG_TEXT(ddata.bitmapptr, rc_00.x + rc_00.w - C_strmetrics.height - con(0x2), rc_00.y + rc_00.h - con(0x1), ptrbefore->width, unsignedword(ddata.paletteptr1[con(0xf)]), unsignedword(ddata.paletteptr1[con(0x0)]) | con(0x4000), t_08);
}

// was SKW_24a5_0fb8
void SKW_DRAW_FOOD_WATER_POISON_PANEL(void)
{
  c_hero* heroptrrg5 = &ddata.hero[ddata.v1e0976 - con(0x1)];
  ddata.v1d66fc = con(0x1);
  SKW_DRAW_STATIC_PIC(con(0x7), con(0x1), con(0x1ee), con(0x0), con(0xffffffff));
  SKW_24a5_0e82(heroptrrg5->w44, con(0x5), con(0xfffffc00), con(0x1f0), con(0x800), false);
  SKW_24a5_0e82(heroptrrg5->w46, con(0xe), con(0xfffffc00), con(0x1f1), con(0x800), false);
  SKW_DRAW_STATIC_PIC(con(0x7), con(0x6), con(0x1f4), con(0x0), con(0xc));
  SKW_DRAW_STATIC_PIC(con(0x7), con(0x7), con(0x1f5), con(0x0), con(0xc));
  if (heroptrrg5->b1f != con(0x0))
  {
    SKW_24a5_0e82(heroptrrg5->w48, con(0x8), con(0x0), con(0x1f3), con(0xc00), false);
    SKW_DRAW_STATIC_PIC(con(0x7), con(0x8), con(0x1f6), con(0x0), con(0xc));
  }
}

// was SKW_24a5_105b
void SKW_DRAW_CRYOCELL_LEVER(bool eaxbool)
{
  SKW_DRAW_STATIC_PIC(con(0x9), (eaxbool ? 1 : 0) + con(0xfffffffa), con(0x1ee), con(0x5b), con(0xffffffff));
  if (!eaxbool)
    ddata.v1d66fc = con(0x7);
  else
  {
    SKW_CHANGE_VIEWPORT_TO_INVENTORY(false);
    SKW_QUEUE_NOISE_GEN1(con(0x9), con(0xfb), con(0xff), con(0x5b), con(0xc8), ddata.v1e0270, ddata.v1e0272, con(0x0));
  }
}

bool SKW_IS_MISCITEM_DRINK_WATER(x16 eaxw)
{
  if ((SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(eaxw, con(0x0)) & con(0x1)) == con(0x0))
    return false;
  if (SKW_ADD_ITEM_CHARGE(eaxw, con(0x0)) == con(0x0))
    return false;
  SKW_ADD_ITEM_CHARGE(eaxw, con(0xffffffff));
  if (eaxw == ddata.savegamewpc.w0)
    SKW_TAKE_OBJECT(eaxw, false);
  return true;
}

// was SKW_24a5_174d
void SKW_CLICK_INVENTORY_EYE(void)
{
  SKW_FIRE_MOUSE_SET_CAPTURE();
  ddata.v1e0228 = true;
  SKW_443c_08ab(&ddata.v1e00b6, &ddata.v1e00b2, &ddata.v1e00b4);
  if ((ddata.v1e00b2 & con(0x2)) != con(0x0))
  {
    SKW_1031_098e();
    SKW_FIRE_HIDE_MOUSE_CURSOR();
    SKW_SLEEP_SEVERAL_TIME(con(0x8));
  }
  else
  {
    SKW_FIRE_MOUSE_RELEASE_CAPTURE();
    ddata.v1e0228 = false;
  }
}


// SPX: SKW_2676_006a renamed SKW_MARK_DYN_LOAD_2676_006a  
void SKW_MARK_DYN_LOAD_2676_006a(x32 eaxl)
{
  SKW_MARK_DYN_LOAD(eaxl);
  ddata.v1e09a0[3 * (ddata.v1e09a4 - con(0x1))] |= con(0x8000);
}

// SPX: SKW_2676_008f renamed SKW_MARK_DYN_LOAD_2676_008f
void SKW_MARK_DYN_LOAD_2676_008f(x32 eaxl, x8 edxb)
{
  SKW_MARK_DYN_LOAD(eaxl);
  ddata.v1e09a0[3 * (ddata.v1e09a4 - con(0x1))] = con(0xffff8001);
  SKW_MARK_DYN_LOAD(unsignedlong(edxb) | (eaxl & con(0xffffff00)));
}

// SPX: SKW_2676_00d0 renamed SKW_MARK_DYN_LOAD_2676_00d0
void SKW_MARK_DYN_LOAD_2676_00d0(x8 eaxb, x8 ebxb, x8 edxb)
{
  ui16 wordrg2 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, con(0xb), ebxb, edxb);
  if (wordrg2 == con(0x0))
    return;
  if (CUTX8(wordrg2) != con(0x0))
    SKW_MARK_DYN_LOAD((unsignedlong(CUTX8(wordrg2)) << con(0x10)) + con(0xd00ffff));
  wordrg2 >>= con(0x8);
  if (wordrg2 != con(0x0))
    SKW_MARK_DYN_LOAD((unsignedlong(CUTX8(wordrg2)) << con(0x10)) + con(0xd00ffff));
}

x16 SKW_QUERY_CUR_CMDSTR_ENTRY(x16 eaxw)
{
  return SKW_QUERY_CMDSTR_ENTRY(ddata.v1e0b7f, ddata.v1e0b7e, eaxw, ddata.v1e0b80);
}

// was SKW_2759_0589
void SKW_ACTIVATE_ACTION_HAND(x16 eaxw, x16 edxw)
{
  c_hero* heroptrrg1 = &ddata.hero[eaxw];
  if (heroptrrg1->w36 == con(0x0))
    return;
  if (!SKW_IS_ITEM_HAND_ACTIVABLE(eaxw, edxw, heroptrrg1->warrc3[edxw]))
    return;
  ddata.v1e0b76 = edxw;
  ddata.v1e0b52 = edxw;
  ddata.v1e0b74 = eaxw + con(0x1);
  ddata.v1e0b54 = eaxw + con(0x1);
  ddata.v1e0b6c = con(0x1);
  ddata.v1e0b62 = con(0x0);
  SKW_CHAMPION_SQUAD_RECOMPUTE_POSITION();
  SKW_UPDATE_RIGHT_PANEL(false);
  SKW_REFRESH_PLAYER_STAT_DISP(eaxw);
}

// was SKW_2759_0602
void SKW_SET_SPELLING_CHAMPION(x16 eaxw)
{
  if (ddata.hero[eaxw].w36 == con(0x0))
    return;
  ddata.v1e0b76 = con(0x2);
  ddata.v1e0b52 = con(0x2);
  ddata.v1e0b74 = eaxw + con(0x1);
  ddata.v1e0b54 = eaxw + con(0x1);
  ddata.v1e0b6c = con(0x1);
  ddata.v1e0b62 = con(0x0);
  SKW_CHAMPION_SQUAD_RECOMPUTE_POSITION();
  SKW_UPDATE_RIGHT_PANEL(false);
}

// range of eaxw: 0...15
// was SKW_2759_13f5
void SKW_CLICK_MAGICAL_MAP_RUNE(x16 eaxw)
{
  c_hero* heroptrrg1;

  x16 wordrg4 = table1d67fe[eaxw];
  x16 wordrg3 = con(0x1) << CUTX8(eaxw);
  if ((wordrg3 & ddata.v1e0b62) == con(0x0))
  {
    heroptrrg1 = &ddata.hero[ddata.v1e0b74 - con(0x1)];
    if (wordrg4 > heroptrrg1->w3e)
      return;
    heroptrrg1->w3e = heroptrrg1->w3e - wordrg4;
    heroptrrg1->u32.b.b1 |= con(0x8);
  }
  else
    wordrg4 = -wordrg4;
  ddata.v1e0b62 ^= wordrg3;
  ddata.v1e0b4e += wordrg4;
}

void SKW_REMOVE_RUNE_FROM_TAIL(void)
{
  c_hero* heroptrrg1 = &ddata.hero[ddata.v1e0b74 - con(0x1)];
  heroptrrg1->b1e -= con(0x1);
  heroptrrg1->t22[heroptrrg1->b1e] = con(0x0);
  ddata.v1e0b6c = con(0x1);
  SKW_UPDATE_RIGHT_PANEL(false);
  R_107B0();
}

void SKW_29ee_00a3(bool eaxbool)
{
  if (ddata.v1d694c.s_00.w_00 != con(0xffffffff))
    return;
  SKW_0b36_0c52(&ddata.v1d694c, eaxbool, con(0xb));
  if (eaxbool)
    SKW_FILL_RECT_SUMMARY(&ddata.v1d694c, ddata.paletteptr1[0], &ddata.v1d694c.s_00.rc_02);
}

void SKW_29ee_0b2b(void)
{
  for (x16 i = 0; i < ddata.v1e0b7a; i++)
    SKW_DRAW_CMD_SLOT(i, false);
  SKW_DRAW_PLAYER_ATTACK_DIR();
}

x16 SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(x8 eaxb, t_gfxdata** ebxppg, t_gfxdata* ecxpg, x8 edxb)
{
  if (!SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(eaxb, con(0x1), con(0xf9), edxb))
    return con(0x0);
  *ebxppg = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(eaxb, con(0xf9), edxb);
  SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_LOCALPAL(eaxb, con(0xf9), edxb)), PAL16, DOWNCAST(t_gfxdata, ecxpg));
  s_gfxdatatail* ptrbefore = getprevious(*ebxppg);
  return ptrbefore->width / ddata.v1d274a;
}

void SKW_29ee_18eb(x16 eaxw, x16 ebxw, x16 edxw)
{
  ddata.v1e0b6e = eaxw;
  ddata.v1e0b5e = eaxw;
  ddata.v1e0b68 = eaxw;
  ddata.v1e0b70 = edxw;
  ddata.v1e0b5c = edxw;
  ddata.v1e0b6a = edxw;
  ddata.v1e0b64 = ebxw;
  ddata.v1e0b60 = SKW_19f0_124b(&ddata.v1e0b68, ebxw, con(0xffffffff), &ddata.v1e0b6a, con(0x110));
  ddata.v1e0b66 = SKW_19f0_124b(&ddata.v1e0b5e, ddata.v1e0b64, con(0x1), &ddata.v1e0b5c, con(0x108));
}

x16 SKW_REMOVE_OBJECT_FROM_HAND(void)
{
  x16 wordrg5 = ddata.savegamewpc.w0;
  if (wordrg5 == con(0xffffffff))
    return con(0xffffffff);
  ddata.savegamewpc.w18 = con(0x0);
  ddata.savegamewpc.w1a = con(0x0);
  ddata.savegamewpc.w0 = con(0xffffffff);
  ddata.savegamewpc.b16 = con(0xffffffff);
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_443c_0434();
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_PROCESS_ITEM_BONUS(ddata.mouse_unk0e, con(0xffffffff), con(0xffffffff), wordrg5);
  R_3CE7D(ddata.v1e0270, con(0xffff), true, ddata.v1e0272, false, false);
  return wordrg5;
}

x16 SKW_2c1d_0e23(x16 eaxw)
{
  x16 wordrg3 = SKW_QUERY_ITEM_WEIGHT(eaxw) / 2;
  x16 wordrg1 = DM2_BETWEEN_VALUE(con(0x1), con(0xa), wordrg3);
  for (;;)
  {
    wordrg3 -= con(0xa);
    if (wordrg3 <= con(0x0))
      return wordrg1;
    wordrg1 += wordrg3 / 2;
  }
}

x16 SKW_STAMINA_ADJUSTED_ATTR(c_hero* eaxphero, x16 edxw)
{
  x16 wordrg41 = edxw;
  x16 wordrg2 = eaxphero->u3c.w >> con(0x1);
  if (eaxphero->w3a < wordrg2)
  {
    x16 wordrg42 = edxw >> con(0x1);
    wordrg41 = eaxphero->w3a * wordrg42 / wordrg2 + wordrg41;
  }
  return wordrg41;
}

x16 SKW_2c1d_0f2a(c_hero* eaxphero, x16 ebxw, x16 edxw)
{
  x16 wordrg4 = con(0xaa) - SKW_GET_PLAYER_ABILITY(eaxphero, con(0x0), edxw);
  if (wordrg4 < con(0x10))
    return ebxw / con(0x8);
  return SKW_0cd5_0176(ebxw, wordrg4, con(0x7));
}

// SKW_2c1d_14ba
void SKW_RESUME_FROM_WAKE(void)
{
  ddata.v1e0488 = true;
  ddata.v1e0238 = false;
  ddata.ticktrig = con(0x8);
  SKW_INIT_BACKBUFF();
  SKW_1031_0541(con(0x5));
}

bool R_37BBB(x16 eaxw)
{
  if (ddata.mouse_unk0e == con(0xffffffff))
    return false;
  return SKW_2c1d_1de2(ddata.mouse_unk0e, eaxw, con(0xffffffff));
}

x16 SKW_GET_PLAYER_WEIGHT(x16 eaxw)
{
  if (ddata.hero[eaxw].w36 == con(0x0))
    return con(0x0);
  x16 wordrg1 = ddata.hero[eaxw].wff;
  if (eaxw == ddata.mouse_unk0e)
    wordrg1 += ddata.savegamewpc.w1a;
  return wordrg1;
}

void SKW_DRAW_CUR_MAX_HMS(x16 eaxw, x16 ebxw, x16 edxw)
{
  t_text t_00[0x8];

  SKW_SK_STRCPY(t_00, SKW_FMT_NUM(edxw, con(0x3), con(0x1)));
  SKW_SK_STRCAT(t_00, v1d1124);
  SKW_SK_STRCAT(t_00, SKW_FMT_NUM(ebxw, con(0x3), con(0x1)));
  SKW_DRAW_LOCAL_TEXT(eaxw, unsignedword(ddata.paletteptr1[con(0x1)]) | con(0x4000), t_00, unsignedword(ddata.paletteptr1[con(0xd)]));
}

void SKW_DRAW_PLAYER_3STAT_TEXT(c_hero* eaxphero)
{
  SKW_DRAW_CUR_MAX_HMS(con(0x226), eaxphero->w38, eaxphero->w36);
  SKW_DRAW_CUR_MAX_HMS(con(0x227), eaxphero->u3c.w / con(0xa), eaxphero->w3a / con(0xa));
  SKW_DRAW_CUR_MAX_HMS(con(0x228), eaxphero->w40, eaxphero->w3e);
}

void SKW_DRAW_PLAYER_DAMAGE(x16 eaxw)
{
  t_text* parpt02;

  x16 wordrg1 = eaxw + con(0xb1);
  SKW_DRAW_ICON_PICT_ENTRY(con(0x1), con(0x3), &ddata.v1d69d4, con(0x2), wordrg1, con(0xa));
  parpt02 = SKW_FMT_NUM(ddata.hero[eaxw].w30, con(0x3), con(0x0));
  SKW_DRAW_SIMPLE_STR(&ddata.v1d69d4, unsignedword(ddata.paletteptr1[con(0xf)]), unsignedword(ddata.paletteptr1[con(0x8)]), wordrg1, parpt02);
}

// was SKW_2e62_05d4
void SKW_DRAW_EYE_MOUTH_COLORED_RECTANGLE(x8 eaxb, x16 edxw)
{
  c_rect rc_00;

  SKW_2405_011f(edxw, &rc_00);
//                                     srcbmp                                                                  palette
  SKW_DRAW_DIALOGUE_PARTS_PICT(SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x1), eaxb, con(0x2)), con(0xc), SKW_QUERY_GDAT_IMAGE_LOCALPAL(con(0x1), eaxb, con(0x2)), &rc_00);
}

void SKW_2e62_061d(x16 eaxw)
{
  c_rect rc_00;

  x16 wordrg4 = eaxw + con(0xad);
  SKW_DRAW_ICON_PICT_ENTRY(con(0x16), con(0x0), &ddata.v1d69d4, ddata.hero[eaxw].b101, wordrg4, con(0xffff));
  if (ddata.savegames1.b2 != con(0x0))
    SKW_0b36_105b(&ddata.v1d69d4, con(0xc), SKW_QUERY_EXPANDED_RECT(wordrg4, &rc_00));
}

// was SKW_2e62_064a
void SKW_DRAW_CHARSHEET_OPTION_ICON(x8 eaxb, x16 ebxw, x16 edxw)
{
  if ((ebxw & ddata.v1e100c) != con(0x0))
    eaxb++;
  SKW_DRAW_STATIC_PIC(con(0x7), eaxb, edxw, con(0x0), con(0xffffffff));
}

// was SKW_2e62_0cd8
void SKW_SET_PARTY_HERO_FLAG(x16 eaxw)
{
  for (x16 i = 0; i < ddata.savegamew2; i++)
    ddata.hero[i].u32.w |= eaxw;
}

void R_3B001(x16 eaxw, x16 edxw)
{
  ddata.v1e0270 = eaxw;
  ddata.v1e0272 = edxw;
  x16 wordrg1 = ddata.v1d3248;
  ddata.v1d3248 = con(0xffffffff);
  SKW_CHANGE_CURRENT_MAP_TO(wordrg1);
}

void R_3E39E(x8 eaxb, x8 ebxb, t_gfxdata* ecxpg, x8 edxb)
{
  t_gfxdata* gptrrg1 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(eaxb, ebxb, edxb);
  SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, gptrrg1), unsignedlong(CUTX16(SKW_CALC_IMAGE_BYTE_LENGTH(gptrrg1))), DOWNCAST(t_gfxdata, ecxpg));
}

void SKW_DRAW_TEMP_PICST(void)
{
  SKW_DRAW_PICST(&ddata.v1e118e);
}

// was SKW_32cb_09a7
x16 SKW_QUERY_MULTILAYERS_PIC(c_image* eaxpimg, x8 ebxb, x8 ecxb, x8 edxb, x16 argw0, x16 argw1, x16 argw2, x16 argw3, x16 argw4, x16 argw5)
{
  x16 wordrg6;

  SKW_QUERY_TEMP_PICST(argw3, argw1, con(0x0), argw0, con(0x0), argw2, con(0xffffffff), con(0xffffffff), argw4, con(0xffff), edxb, ebxb, ecxb);
  SKW_COPY_IMAGE(&ddata.v1e118e, eaxpimg);
  SKW_0b36_018f(&eaxpimg->s_00);
  eaxpimg->w_30 = argw5;
  SKW_SET_ORIGIN_RECT(&eaxpimg->rc_24, eaxpimg->s_00.w_12, eaxpimg->s_00.w_14);
  wordrg6 = SKW_3e74_5888();
  eaxpimg->pg_2c = SKW_ALLOC_NEW_PICT(wordrg6, eaxpimg->s_00.w_14, con(0x8), eaxpimg->s_00.w_12);
  return wordrg6;
}

void SKW_32cb_0a4c(c_rect* eaxr, x8 ebxb, x8 edxb)
{
  SKW_COPY_RECT(eaxr, &ddata.v1e02f0[ddata.v1e0404].r0);
  ddata.v1e02f0[ddata.v1e0404].bb = edxb;
  ddata.v1e02f0[ddata.v1e0404].ba = ebxb;
  ddata.v1e02f0[ddata.v1e0404].w8 = con(0xffffffff);
  ddata.v1e0404++;
}

x8 SKW_SKCHR_TO_SCRIPTCHR(x8 eaxb)
{
  if (eaxb >= con(0x41) && eaxb <= con(0x5a))
    return eaxb - con(0x41);
  if (eaxb == con(0x2e))
    return con(0x1b);
  return con(0x1a);
}

void SKW_32cb_2cf3(x8 eaxb, x16 ebxw, x16 ecxw, x16 edxw)
{
  x16 wordrg5 = DM2_BETWEEN_VALUE(con(0x8), con(0x40), edxw & con(0xfffffffe));
  SKW_QUERY_GDAT_SUMMARY_IMAGE(&ddata.v1e118e, eaxb, con(0x41), con(0xd));
  ddata.v1e118e.w_20 += ddata.v1e118e.w_1c;
  ddata.v1e118e.w_22 += ddata.v1e118e.w_1e;
  ddata.v1e118e.w_1e = con(0x0);
  ddata.v1e118e.w_1c = con(0x0);
  ddata.v1e118e.w_18 = ecxw;
  ddata.v1e118e.w_1a = con(0x0);
  ddata.v1e118e.w_30 = con(0xa);
  ddata.v1e118e.pg_2c = ddata.bitmapptr;
  ddata.v1e118e.w_32 = ebxw;
  ddata.v1e118e.w_36 = wordrg5;
  ddata.v1e118e.w_34 = wordrg5;
  R_B073(ddata.v1e118e.carr_3a, con(0xa), con(0xffffffff), CUTX8(ddata.v1e12d2), &ddata.v1e118e.w_38);
  SKW_QUERY_PICST_IT(&ddata.v1e118e);
}

void SKW_32cb_3edd(x16 eaxw)
{
  if (ddata.v1d6c04 != con(0xfffffffe))
    SKW_32cb_3e08(ddata.v1d6c04, ddata.longv1e1040, ddata.v1e103c, eaxw);
  ddata.v1d6c04 = con(0xfffffffe);
}

x16 SKW_32cb_48d5(x16 eaxw, x16 edxw)
{
  x16 wordrg4 = (eaxw << con(0x7)) + con(0x40);
  x16 wordrg3 = wordrg4 / edxw / 2;
  if (SKW_CALC_STRETCHED_SIZE(edxw, wordrg3) >= eaxw)
    return wordrg3;
  return wordrg3 + con(0x1);
}

void SKW_38c8_00c8(void)
{
  ddata.bitmapptr = R_D2C(ddata.vx1d270a, con(0x8), ddata.vx1d270c);
  dm2_dballochandler.v1e0a90 = con(0x186a0);
}

void SKW_3929_0b01(x16 eaxw, x8 edxb)
{
  t_text t_00[0x2];

  t_00[0x0] = edxb;
  t_00[0x1] = con(0x0);
  SKW_DISPLAY_HINT_TEXT(eaxw, t_00);
}

void SKW_3929_0b20(x16 eaxw, x32 edxl)
{
  t_text t_00[0x10];

  SKW_SK_LTOA10(edxl, t_00);
  SKW_DISPLAY_HINT_TEXT(eaxw, t_00);
  SKW_DISPLAY_HINT_TEXT(eaxw, ddata.v1d70e4);
}

// was SKW_3929_0b5a
void SKW_DISPLAY_HINT_NEW_LINE(void)
{
  SKW_DISPLAY_HINT_TEXT(con(0x0), ddata.v1d70e8);
}

void SKW_3a15_05f7(x16 eaxw)
{
  if (ddata.v1e1428 >= con(0x0))
    SKW_3a15_0486(ddata.v1e1428);
  SKW_3a15_0486(SKW_GET_TIMER_NEW_INDEX(eaxw));
}

// was SKW_3a15_072f
void SKW_GET_AND_DELETE_NEXT_TIMER(c_tim* eaxptim)
{
  SKW_COPY_TIMER(&ddata.savegametimarrp[ddata.v1e1424[con(0x0)]], eaxptim);
  SKW_DELETE_TIMER(ddata.v1e1424[con(0x0)]);
}

// was SKW_3a15_0763
bool SKW_IS_TIMER_TO_PROCEED(void)
{
  if (ddata.v1e1428 >= con(0x0))
    SKW_3a15_0486(ddata.v1e1428);
  if (ddata.savegamew5 == con(0x0))
    return false;
  if ((ddata.savegametimarrp[ddata.v1e1424[0]].un.l_00 & con(0xffffff)) > ddata.longmallocx)
    return false;
  return true;
}

// was SKW_3a15_0d10
void SKW_PROCESS_TIMER_DESTROY_DOOR(c_tim* eaxptim)
{
  x8* bptrrg4 = ddata.savegamepp1[eaxptim->u.b.b_06];
  x8* bptrrg1 = bptrrg4 + eaxptim->u.b.b_07;
  *bptrrg1 = (*bptrrg1 & con(0xfffffff8)) | con(0x5);
  if (ddata.v1d3248 == ddata.v1e0266)
    ddata.v1e0390.l0 = con(0x3);
}

// was SKW_3a15_2feb
void SKW_PROCESS_TIMER_3D(c_tim* eaxptim)
{
  x16 wordrg6 = unsignedword(eaxptim->u.b.b_06);
  x16 wordrg1 = SKW_MOVE_RECORD_TO(eaxptim->v.w_08, con(0x0), wordrg6, con(0xfffffffd), unsignedword(eaxptim->u.b.b_07));
  if (wordrg1 != con(0x0) || eaxptim->b_04 == con(0x3d))
    SKW_QUEUE_NOISE_GEN1(con(0x3), con(0x89), con(0x61), con(0x0), con(0x80), wordrg6, unsignedword(eaxptim->u.b.b_07), con(0x1));
}

// was SKW_3a15_303f
void SKW_PROCESS_TIMER_0C(x16 eaxw)
{
  c_hero* heroptrrg1 = &ddata.hero[eaxw];
  heroptrrg1->w2e = con(0xffffffff);
  if (heroptrrg1->w36 != con(0x0))
    heroptrrg1->u32.b.b1 |= con(0x8);
}

// was SKW_3a15_320c
void SKW_PROCESS_TIMER_0E(c_tim* eaxptim, x16 edxw)
{
  x32 longrg5;
  x16 wordrg6;
  c_record* ebprec;
  c_record* prec_00;

  longrg5 = unsignedlong(table1d280c[eaxptim->u.w_06]);
  ebprec = UPCAST(c_record, DM2_ALLOC_LOBIGPOOL_MEMORY(longrg5));
  wordrg6 = eaxptim->u.w_06 << con(0xa);
  prec_00 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
  SKW_COPY_MEMORY(DOWNCAST(c_record, prec_00), longrg5, DOWNCAST(c_record, ebprec));
  SKW_SET_ITEMTYPE(wordrg6, eaxptim->v.b.b_08);
  SKW_PROCESS_ITEM_BONUS(unsignedword(eaxptim->b_05), con(0xffffffff), edxw, wordrg6);
  SKW_COPY_MEMORY(DOWNCAST(c_record, ebprec), longrg5, DOWNCAST(c_record, prec_00));
  DM2_DEALLOC_LOBIGPOOL(longrg5);
}

// was SKW_3a15_374b
void SKW_PROCESS_TIMER_RELEASE_DOOR_BUTTON(c_tim* eaxptim)
{
  SKW_GET_ADDRESS_OF_RECORD(eaxptim->u.w_06)->u2.b.b1 &= con(0xfffffff7);
}

// was SKW_3df7_000a
// return never requested
x16 SKW_SET_TIMER_WEATHER(x32 eaxl)
{
  c_tim vtim_00;

  vtim_00.un.l_00 = ddata.longmallocx + eaxl;
  vtim_00.b_04 = con(0x54);
  vtim_00.b_05 = con(0x0);
  return SKW_QUEUE_TIMER(&vtim_00);
}

// request value by driver - interface.
t_gfxdata* R_4A986(x16 eaxw)
{
  *UPCAST(i32, ddata.drvinterfaceptr) = lextended(eaxw); // why input? function needs no input
  userinterrupt(con(0x2b), con(0x0));
  return UPCAST(t_gfxdata, *UPCAST(i8*, ddata.drvinterfaceptr));
}

// first part removed - did set interruptvectors to R_4799
void R_4A9A4(void)
{
  userinterrupt(con(0x3), con(0x0));
}

// Attention:
// original gives the stackadress to the subroutine.
// This routine copies 256 bytes to the driver - but here
// are no 256 bytes! To avoid problems, a size parameter gets added to
// SKW_COPY_TO_DRIVER.
void R_1583(x8 eaxb)
{
  ui16 vw_00;

  vw_00 = unsignedword(eaxb);
  SKW_COPY_TO_DRIVER(DOWNCAST(x16, &vw_00), con(0x2)); // CAST stays
}

void SKW_44c8_1baf(x8* eaxpb)
{
  SKW_COPY_MEMORY(eaxpb, con(0x400), ddata.drvinterfaceptr);
  userinterrupt(con(0x10a), con(0x0));
  ddata.v1e0080 = *UPCAST(x16, ddata.drvinterfaceptr) != con(0x0);
}

bool SKW_44c8_1e1a(x8* eaxpb, x16 edxw)
{
  return (table1d2558[edxw & con(0x1)] & eaxpb[edxw / 2]) != con(0x0);
}

// return never requested
ui16 SKW_44c8_20a4(t_gfxdata* eaxpg, t_gfxdata* ebxpg, c_rect* ecxr, t_gfxdata* edxpg, ui16 arguw0, ui16 arguw1, x16 argw2, x8 argb3, t_gfxdata* argpg4)
{
//                                                                                                           palette
  return SKW_44c8_1e43(eaxpg, ebxpg, ecxr, edxpg, arguw0, arguw1, con(0x0), con(0x0), ecxr->w, argw2, argb3, argpg4);
}

// TODO: allocating with non-standard mask
void SKW_443c_067a(s_munknown* eaxps)
{
  if ((eaxps->b3 & con(0x40)) != con(0x0))
    return;
  eaxps->b3 |= con(0x40);
  eaxps->u.pb6 = UPCAST(s_between, DM2_ALLOC_MEMORY_RAM(con(0xc), MASK_200, FREEPOOL));
}

void SKW_443c_08ab(x16* eaxpw, x16* ebxpw, x16* edxpw)
{
  x16* wptr = UPCAST(i16, ddata.drvinterfaceptr);
  wptr[con(0x0)] = con(0x0);
  userinterrupt(con(0xe), con(0x0));
  *eaxpw = wptr[con(0x0)];
  *edxpw = wptr[con(0x1)];
  *ebxpw = wptr[con(0x2)];
}

void R_4DA3(x8 eaxb, x16* ebxpw, s_8bytearray* ecxps, x16 edxw)
{
  s_8bytearray* sptrrg1;

  sptrrg1 = UPCAST(s_8bytearray, SKW_QUERY_GDAT_ENTRY_DATA_PTR(con(0xf), con(0x7), con(0xfd), eaxb));
  sptrrg1 += R_4E26(ebxpw) + edxw;
  SKW_COPY_MEMORY(DOWNCAST(s_8bytearray, sptrrg1), con(0x8), DOWNCAST(s_8bytearray, ecxps));
}

void R_4DEA(x8 eaxb, x16* ebxpw, x32* ecxpl, x16 edxw)
{
  SKW_COPY_MEMORY(SKW_QUERY_GDAT_ENTRY_DATA_PTR(con(0xf), con(0x7), con(0xfc), eaxb) + 4 * (R_4E26(ebxpw) + edxw), con(0x4), DOWNCAST(x32, ecxpl));
}

// unreferenced
x16 R_1456(void)
{
  userinterrupt(con(0x12), con(0x0));
  return *P16CAST(ddata.drvinterfaceptr);
}

// special plugin - parameter never used
void R_148D(x16 dummy)
{
}

x16 SKW_476d_05b6(x16 n)
{
  userinterrupt(con(0x1b), n);
  return *P16CAST(ddata.drvinterfaceptr);
}

// was SKW_2405_00c9
void SKW_DISPLAY_TAKEN_ITEM_NAME(x16 n)
{
  SKW_DISPLAY_HINT_NEW_LINE();
  SKW_DISPLAY_HINT_TEXT(con(0xd), SKW_GET_ITEM_NAME(n));
}

// buffer is a bytebuffer
// was S KW_476d_05e3
void SKW_COPY_TO_DRIVER(x8* buffer, x16 amount)
{
  SKW_COPY_MEMORY(buffer, amount, DOWNCAST(i8, ddata.drvinterfaceptr));
  userinterrupt(con(0x0), con(0x0));
}

void SKW_443c_087c(void)
{
  userinterrupt(con(0x4), con(0x0));
}

void SKW_443c_0889(void)
{
  userinterrupt(con(0x5), con(0x0));
}
