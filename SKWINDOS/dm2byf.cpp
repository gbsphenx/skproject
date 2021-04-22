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



#include "../skwin/defines.h"

#include "dm2debug.h"

void SKW_STEP_DOOR(c_tim* eaxptim)
{
  c_record* recoptrrg1;
  x8* ebppb;
  c_record* prec_04;
  x16 vw_14;
  x16 vw_1c;
  x16 vw_20;
  x16 vw_28;

  bool vbool_00 = false;
  bool vbool_10 = false;
  x16 wordrg6 = unsignedword(eaxptim->u.b.b_06);
  vw_28 = unsignedword(eaxptim->u.b.b_07);
  ebppb = ddata.savegamepp1[eaxptim->u.b.b_06] + eaxptim->u.b.b_07;
  vw_20 = unsignedword(*ebppb & con(0x7));
  if (vw_20 == con(0x5))
    return;
  if (ddata.v1d3248 == ddata.v1e0266)
    ddata.v1e0390.l0 = con(0x3);
  recoptrrg1 = SKW_GET_ADDRESS_OF_TILE_RECORD(wordrg6, vw_28);
  prec_04 = recoptrrg1;
  if ((recoptrrg1->u2.b.b1 & con(0x4)) == con(0x0))
    return;
  eaxptim->un.l_00 += con(0x1);
  if ((recoptrrg1->u2.b.b1 & con(0x2)) != con(0x0))
  {
    if (vw_20 == con(0x0))
    {
      recoptrrg1->u2.b.b1 &= con(0xfffffffb);
      return;
    }
  }
  else
  {
    if (vw_20 == con(0x4))
    {
      recoptrrg1->u2.b.b1 &= con(0xfffffffb);
      return;
    }
    x16 vw_18 = ulrshift(recoptrrg1->u2.w, con(0xa), con(0xf));
    x16 vw_24 = SKW_QUERY_DOOR_DAMAGE_RESIST(SKW_IS_REBIRTH_ALTAR(recoptrrg1));
    if (ddata.v1d3248 == ddata.v1e0282 && wordrg6 == ddata.v1e0280)
    {
      if (vw_28 == ddata.v1e027e && vw_20 != con(0x0))
      {
        vbool_10 = true;
        if (UI16CAST(ddata.savegamew2) > con(0x0))
        {
          *ebppb &= con(0xf8);
          vw_1c = SKW_ATTACK_PARTY(vw_24, con(0x2), (((vw_18 | con(0x8)) != con(0x0)) ? 1 : 0) + con(0x3));
          if (vw_1c != con(0x0))
            for (x16 vw_2c = con(0x0); vw_2c < con(0x4); vw_2c++)
              if (((con(0x1) << CUTX8(vw_2c)) & unsignedlong(vw_1c)) != con(0x0))
                SKW_QUEUE_NOISE_GEN2(con(0x16), con(0x8a), con(0xfe), ddata.hero[vw_2c].b101, wordrg6, vw_28, con(0x1), con(0x64), con(0xc8));
        }
      }
    }
    vw_14 = SKW_GET_CREATURE_AT(wordrg6, vw_28);
    if (vw_14 != con(0xffffffff))
    {
      x16 wordrg199 = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(vw_14);
      if ((wordrg199 & con(0x20)) == con(0x0))
      {
        x16 wordrg198;
        if (vw_18 == con(0x0))
          wordrg198 = con(0x1);
        else
          wordrg198 = urshift(wordrg199, con(0x6)) & con(0x3);
        if (vw_20 >= wordrg198)
        {
          if ((SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(vw_14)->u18.b.b1 & con(0x10)) != con(0x0))
          {
            vw_24 >>= con(0x1);
            vw_24 = DM2_MAX(con(0x1), vw_24);
          }
          SKW_ATTACK_CREATURE(vw_14, vw_28, con(0x2006), wordrg6, con(0x64), vw_24);
          if (vw_20 != con(0x0))
            vw_20 = vw_20 - con(0x1);
          else
            vw_20 = con(0x0);
          *ebppb = CUTX8(vw_20) | (*ebppb & con(0xfffffff8));
          SKW_QUEUE_NOISE_GEN2(con(0xf), con(0x82), con(0xfe), SKW_QUERY_CLS2_FROM_RECORD(vw_14), wordrg6, vw_28, con(0x1), con(0x46), con(0x80));
          SKW_QUEUE_NOISE_GEN2(con(0x15), con(0x85), con(0xfe), con(0xfe), wordrg6, vw_28, con(0x1), con(0x46), con(0x80));
          vbool_10 = true;
        }
      }
    }
    if (vbool_10)
    {
      vbool_00 = true;
      eaxptim->un.l_00 += con(0x1);
    }
  }
  if (!vbool_00)
  {
    x16 wordrg405;
    if ((prec_04->u2.b.b1 & con(0x2)) == con(0x0))
      wordrg405 = con(0x1);
    else
      wordrg405 = con(0xffffffff);
    vw_20 = vw_20 + wordrg405;
    *ebppb = CUTX8(vw_20) | (*ebppb & con(0xfffffff8));
    if ((prec_04->u2.b.b1 & con(0x2)) == con(0x0))
    {
      if (vw_20 != con(0x4))
        vbool_00 = true;
    }
    else if (vw_20 != con(0x0))
      vbool_00 = true;
    x8 vb_2c_1;
    if (vbool_00 || vw_20 != con(0x4))
      vb_2c_1 = con(0x8e);
    else
      vb_2c_1 = con(0x8f);
    SKW_QUEUE_NOISE_GEN2(con(0xe), vb_2c_1, con(0xfe), SKW_IS_REBIRTH_ALTAR(prec_04), wordrg6, vw_28, con(0x1), con(0x5f), con(0x80));
  }
  if (vbool_00)
  {
    prec_04->u2.b.b1 |= con(0x4);
    SKW_QUEUE_TIMER(eaxptim);
  }
  else
  {
    recoptrrg1 = prec_04;
    recoptrrg1->u2.b.b1 &= con(0xfffffffb);
  }
}

// was SKW_32cb_5340
void SKW_DRAW_PLAYER_TILE(void)
{
  x16 wordrg6;
  c_5bytes vs_00;

  x16 wordrg3;
  bool skip = false;
  ui16 uwordrg5 = ddata.v1e1044[con(0x0)].s_02.warr_00[con(0x0)];
  if (uwordrg5 < con(0x10))
  {
    if (uwordrg5 == con(0x2))
      SKW_DRAW_PIT_TILE(con(0x0));
  }
  else if (uwordrg5 <= UI16CAST(con(0x10)))
  {
    x16 wordrg1 = SKW_GET_ADDRESS_OF_RECORD(ddata.v1e1044[con(0x0)].s_02.warr_00[con(0x4)])->u2.w & con(0x1);
    wordrg3 = con(0x40);
    if (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xe), con(0xb), con(0x40), ddata.v1e040e[wordrg1]) == con(0x0))
    {
      wordrg3 = SKW_QUERY_CREATURE_BLIT_RECTI(con(0x0), t_rotate(con(0x0)), con(0x2));
      SKW_DRAW_DUNGEON_GRAPHIC(con(0x8), con(0x6), wordrg3, ddata.v1d6c02, ddata.v1d6c00, ddata.v1e12d0);
    }
  }
  else
  {
    if (uwordrg5 <= UI16CAST(con(0x11)))
    {
      SKW_DRAW_DOOR_TILE(con(0x0));
      wordrg3 = con(0x0);
      skip = true;
    }
    else if (uwordrg5 == con(0x13))
    {
      wordrg6 = ddata.v1e1044[con(0x0)].s_02.warr_00[con(0x3)];
      wordrg3 = (wordrg6 == con(0x0)) ? con(0x338) : con(0x32b);
      SKW_DRAW_DUNGEON_GRAPHIC(con(0x8), wordrg6 == con(0x0) ? con(0x4d) : con(0x39), wordrg3, ddata.v1d6c02, ddata.v1d6c00, con(0x0));
      wordrg3 = (wordrg6 == con(0x0)) ? con(0x339) : con(0x32c);
      SKW_DRAW_DUNGEON_GRAPHIC(con(0x8), wordrg6 == con(0x0) ? con(0x4e) : con(0x3a), wordrg3, ddata.v1d6c02, ddata.v1d6c00, con(0x0));
    }
  }
  if (!skip)
  {
    SKW_DRAW_PIT_ROOF(con(0x0));
    wordrg3 = SKW_GET_TELEPORTER_DETAIL(&vs_00, signedword(ddata.v1e1044->b_01), signedword(ddata.v1e1044->b_00)) ? 1 : 0;
    if (wordrg3 == con(0x0))
      SKW_DRAW_EXTERNAL_TILE(wordrg3);
    SKW_DRAW_STATIC_OBJECT(con(0x0), false, con(0x1ffffff));
    if (uwordrg5 == con(0x5))
      SKW_DRAW_TELEPORTER_TILE(con(0x0), con(0x0), con(0x18));
  }
  SKW_DRAW_RAIN();
  if (wordrg3 != con(0x0))
    SKW_DRAW_EXTERNAL_TILE(con(0x0));
  SKW_32cb_2d8c(ddata.v1e1044[con(0x0)].s_02.warr_00[con(0x2)], con(0x1ffffff), con(0x0));
}

void SKW_12b4_0d75(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  x16 wordrg5;
  c_hero* heroptrrg5;
  s_d556* sd5ptrrg1;
  x16 vw_04;
  x16 vw_10;
  x8 vb_14;
  x16 vw_18;
  x16 vw_1c;
  x16 vw_20;

  x16 vw_0c = eaxw;
  x16 vw_08 = edxw;
  vw_04 = ebxw;
  if (SKW_IS_CREATURE_MOVABLE_THERE(vw_0c, vw_04, &vw_1c, vw_08))
  {
    x16 wordrg101 = SKW_GET_CREATURE_WEIGHT(vw_1c);
    if (ecxw >= con(0xfe) || wordrg101 <= ecxw || (SKW_RAND16((wordrg101 - ecxw) / 4 + con(0x1))) == con(0x0))
    {
      x16 wordrg6 = vw_0c + table1d27fc[vw_04];
      wordrg5 = vw_08;
      wordrg5 += table1d2804[vw_04];
      x16 wordrg144 = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(vw_1c);
      vb_14 = CUTX16(wordrg144);
      if (ddata.v1d3248 != ddata.v1e0282 || wordrg6 != ddata.v1e0280 || wordrg5 != ddata.v1e027e || (wordrg144 & con(0x8000)) != con(0x0))
      {
        SKW_1c9a_0247(vw_1c);
        x8 byterg15 = SKW_GET_ADDRESS_OF_RECORD(vw_1c)->u4.b.b1;
        if (byterg15 != con(0xffffffff))
        {
          sd5ptrrg1 = &ddata.v1e089c[byterg15]; // BUGFIX 4/2014
          x16 vw_24 = signedword(sd5ptrrg1->u.v.b1a);
          if (sd5ptrrg1->u.v.b1a != con(0xffffffff))
            if (vw_24 != con(0x13))
              if ((table1d613a[vw_24] & con(0x4)) != con(0x0))
              {
                SKW_1c9a_0db0(unsignedlong(vw_1c));
                SKW_1c9a_0cf7(vw_0c, vw_08);
              }
        }
        SKW_MOVE_RECORD_TO(vw_1c, vw_08, wordrg6, vw_0c, wordrg5);
        if (SKW_GET_CREATURE_AT(wordrg6, wordrg5) != con(0xffffffff) && (vb_14 & con(0x1)) == con(0x0))
          SKW_ATTACK_CREATURE(con(0xffff), wordrg5, con(0x4002), wordrg6, con(0xa), con(0x0));
      }
      else
        SKW_12b4_023f(vw_0c, &vw_18, &vw_10, vw_08, ddata.v1e0258, (vw_04 + con(0x2)) & con(0x3));
    }
  }
  if (ddata.v1d3248 != ddata.v1e0282 || vw_0c != ddata.v1e0280 || vw_08 != ddata.v1e027e)
    return;
  x16 wordrg61 = con(0x0);
  vw_20 = con(0x0);
  heroptrrg5 = ddata.hero;
  x16 vw_24_1 = con(0x0);
  for(;;)
  {
    if (wordrg61 >= con(0x4))
    {
      if (vw_24_1 != con(0x0))
      {
        vw_20 = vw_20 / vw_24_1 + con(0x28);
        if (ecxw >= con(0xfe) || ecxw > vw_20)
          SKW_PERFORM_MOVE(((vw_04 - ddata.v1e0258) & con(0x3)) + con(0x3));
      }
      return;
    }
    if (heroptrrg5->w36 != con(0x0))
    {
      SKW_QUEUE_NOISE_GEN2(con(0x16), con(0x8a), con(0xfe), heroptrrg5->b101, vw_0c, vw_08, con(0x1), con(0x64), con(0xc8));
      vw_20 = vw_20 + SKW_GET_PLAYER_WEIGHT(wordrg61) / con(0xa);
      vw_24_1++;
    }
    heroptrrg5++;
    wordrg61++;
  }
}

bool SKW_14cd_2662(x8 eaxb)
{
  x8 vb_04;

  vb_04 = eaxb;
  x16 wordrg2 = con(0xfffe);
  x16 wordrg4 = unsignedword(ddata.s350.v1e0562.u.b.b_06);
  x16 wordrg3 = unsignedword(ddata.s350.v1e0562.u.b.b_07);
  if (vb_04 != con(0xffffffff))
    vb_04 = (vb_04 + CUTX8(ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe))) + con(0x2)) & con(0x3);
  x16 wordrg104 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
  wordrg4 += table1d27fc[wordrg104];
  wordrg3 += table1d2804[wordrg104];
  x16 wordrg101 = SKW_GET_CREATURE_AT(wordrg4, wordrg3);
  if (wordrg101 != con(0xffffffff))
  {
    wordrg2 = SKW_GET_ADDRESS_OF_RECORD(wordrg101)->u2.w;
    for (;; wordrg2 = SKW_GET_NEXT_RECORD_LINK(wordrg2))
    {
      if (wordrg2 == con(0xfffffffe))
        break;
      x16 wordrg102 = urshift(wordrg2 & con(0x3c00), con(0xa));
      if (wordrg102 > con(0x4) && wordrg102 < con(0xe) || wordrg102 == con(0x9))
        if (vb_04 == con(0xffffffff) || unsignedword(vb_04) == urshift(wordrg2, con(0xe)))
          if (SKW_1c9a_0006(wordrg2, con(0x10)) == con(0x0) && SKW_1c9a_0006(wordrg2, con(0x7)) == con(0x0))
            break;
    }
  }
  return wordrg2 != con(0xfffffffe);
}

bool SKW_1887_0b1b(void)
{
  x16 wordrg5;
  c_hero* heroptrrg2;
  bool flag;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;

  bool boolrg6 = false;
  if (ddata.v1d3248 != ddata.v1e08d6 || ddata.v1e08a6 != ddata.v1e08d8)
    return true;
  else if (ddata.v1e08a4 != ddata.v1e08d4)
    return true;
  x16 wordrg31 = SKW_GET_PLAYER_AT_POSITION(unsignedword(ddata.s350.v1e0556->u.v.b1c));
  vw_0c = wordrg31;
  if (wordrg31 == con(0xffffffff))
    return true;
  heroptrrg2 = &ddata.hero[wordrg31];
  x16 vw_00 = CUTX16(SKW_RAND());
  wordrg5 = vw_00;
  if (SKW_USE_LUCK_ATTRIBUTE(heroptrrg2, con(0x64) - SKW_USE_DEXTERITY_ATTRIBUTE(wordrg31)))
  {
    wordrg5 &= con(0x70);
    if (wordrg5 == con(0x0))
      return !boolrg6;
  }
  vw_08 = con(0x0);
  x16 wordrg3 = heroptrrg2->warrc3[con(0x0)];
  wordrg5 = heroptrrg2->warrc3[con(0x1)];
  if (wordrg3 != con(0xffffffff) && (SKW_1c9a_0006(wordrg3, signedword(ddata.s350.v1e0556->u.v.b1e)) != con(0x0)))
    vw_04 = R_F958(wordrg3);
  else
    vw_04 = con(0xffffffff);
  x16 wordrg1;
  if (wordrg5 != con(0xffffffff) && (SKW_1c9a_0006(wordrg5, signedword(ddata.s350.v1e0556->u.v.b1e)) != con(0x0)))
    wordrg1 = R_F958(wordrg5); // BUGFIX 4/2014
  else
    wordrg1 = con(0xffffffff);
  if ((vw_00 & con(0x3)) == con(0x0))
    flag = (vw_00 & con(0x4)) == con(0x0);
  else
    flag = wordrg1 <= vw_04;
  if (!flag)
  {
    vw_08 = con(0x1);
    wordrg3 = wordrg5;
  }
  if (wordrg3 != con(0xffffffff) && SKW_1c9a_0006(wordrg3, signedword(ddata.s350.v1e0556->u.v.b1e)) != con(0x0))
  {
    SKW_APPEND_RECORD_TO(SKW_REMOVE_POSSESSION(vw_0c, vw_08), con(0xffffffff), con(0x0), &ddata.s350.v1e054e->u2.w);
    boolrg6 = true;
    if (ddata.v1e0238)
    {
      ddata.v1e0238 = false;
      bool boolrg1 = (CUTX16(SKW_RAND()) & con(0xf)) + SKW_QUERY_PLAYER_SKILL_LV(vw_0c, true, con(0x1)) > con(0x7);
      ddata.v1e0238 = true;
      if (boolrg1)
        SKW_RESUME_FROM_WAKE();
    }
  }
  return !boolrg6;
}

void SKW_2f3f_04ea(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0)
{
  c_hero* heroptrrg7;
  c_record* recoptrrg1;
  t_text t_08[0x80];

  x16 vw_04 = eaxw;
  x16 vw_00 = edxw;
  x16 vw_8c = ddata.v1e0288 - con(0x1);
  heroptrrg7 = &ddata.hero[vw_8c];
  x16 vw_88 = ddata.v1d3248;
  SKW_CHANGE_CURRENT_MAP_TO(ecxw);
  x16 wordrg4 = SKW_REMOVE_OBJECT_FROM_HAND();
  if (argw0 != con(0x93))
  {
    if (!ddata.v1d6a2d)
      SKW_DRAW_CRYOCELL_LEVER(true);
    if (wordrg4 != con(0xffffffff))
      SKW_ADD_ITEM_TO_PLAYER(vw_8c, wordrg4);
    ddata.v1e0288 = con(0x0);
    vw_04 += table1d27fc[ebxw];
    vw_00 += table1d2804[ebxw];
    for (x16 wordrg5 = con(0x0); wordrg5 < con(0x1e); wordrg5++)
      if (heroptrrg7->warrc3[wordrg5] != con(0xffffffff))
        SKW_CUT_RECORD_FROM(heroptrrg7->warrc3[wordrg5], vw_04, vw_00, NULL);
    x16 wordrg2 = SKW_GET_TILE_RECORD_LINK(vw_04, vw_00);
    for (;; wordrg2 = SKW_GET_NEXT_RECORD_LINK(wordrg2))
      if (urshift(wordrg2 & con(0x3c00), con(0xa)) == con(0x3))
      {
          recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg2);
          if ((recoptrrg1->u2.w & con(0x7f)) == con(0x7e))
            break;
      }
    recoptrrg1->u4.b.b0 &= con(0xfffffffb);
    if (ddata.savegamew2 == con(0x1))
      ddata.longv1e01a0 = ddata.longmallocx;
    if (!ddata.v1d6a2d)
    {
      ddata.v1e098c = vw_8c;
      SKW_DISPLAY_HINT_NEW_LINE();
      SKW_DISPLAY_HINT_TEXT(unsignedword(table1d69d0[vw_8c]), SKW_QUERY_GDAT_TEXT(con(0x1), con(0xe), t_08, con(0x0)));
      if (ddata.v1e0976 != con(0x0))
      {
        SKW_24a5_1798(con(0x4));
        SKW_38c8_0060();
      }
    }
  }
  else
  {
    SKW_24a5_1798(con(0x4));
    SKW_FIRE_HIDE_MOUSE_CURSOR();
    if (ddata.savegamew2 == con(0x1))
      SKW_SELECT_CHAMPION_LEADER(con(0xffffffff));
    if (ddata.v1e0288 >= ddata.savegamew2)
      ddata.savegamew2--;
    ddata.v1e0288 = con(0x0);
    heroptrrg7->w36 = con(0x0);
    ddata.v1e0b1c[vw_8c] = false;
    ddata.v1d67c2.b0[vw_8c] = con(0xffffffff);
    v1d67be[vw_8c] = con(0xffffffff);
    ddata.v1e0b34[vw_8c][1] = con(0x0);
    ddata.v1d67c2.w0[vw_8c + con(0x1)][1] = con(0xffff);
    ddata.v1e0b34[vw_8c][0] = con(0x0);
    ddata.v1d67c2.w0[vw_8c + con(0x1)][0] = con(0xffff);
    ddata.v1e0b34[vw_8c][2] = con(0x0);
    SKW_FIRE_FILL_SCREEN_RECT(vw_8c + con(0xa1), ddata.paletteptr1[0]);
    SKW_38c8_0060();
    SKW_FIRE_SHOW_MOUSE_CURSOR();
  }
  SKW_CHANGE_CURRENT_MAP_TO(vw_88);
}

void SKW_2f3f_0789(void)
{
  c_hero* heroptrrg3;

  x16 wordrg1;
  if (ddata.savegamew8 != con(0x0))
  {
    ddata.savegamewpc.w0 = con(0xffffffff);
    ddata.savegamewpc.b16 = con(0xffffffff);
    x16 wordrg100 = SKW_GET_TILE_RECORD_LINK(con(0x0), con(0x0));
    x16 wordrg4;
    for (;; wordrg100 = SKW_GET_NEXT_RECORD_LINK(wordrg4))
    {
      wordrg4 = wordrg100;
      if (wordrg4 == con(0xfffffffe))
        return;
      if (urshift(wordrg4 & con(0x3c00), con(0xa)) == con(0x3) && (SKW_GET_ADDRESS_OF_RECORD(wordrg4)->u2.w & con(0x7f)) == con(0x7e))
        break;
    }
    ddata.v1d6a2d = true;
    SKW_SELECT_CHAMPION(con(0x0), con(0x0), ddata.v1e0266, con(0x1));
    SKW_2f3f_04ea(con(0x0), con(0x0), ddata.v1e0266, con(0x1), con(0x92));
    ddata.v1d6a2d = false;
    ddata.hero[0].b1c = CUTX8(ddata.v1e0258);
    ddata.hero[0].b1d = CUTX8(ddata.v1e0258);
    SKW_SET_PARTY_HERO_FLAG(con(0x4000));
    wordrg1 = con(0x0);
  }
  else
  {
    if (ddata.savegamewpc.w0 != con(0xffffffff))
    {
      x16 wordrg21 = ddata.mouse_unk0e;
      ddata.mouse_unk0e = con(0xffffffff);
      SKW_TAKE_OBJECT(ddata.savegamewpc.w0, true);
      ddata.mouse_unk0e = wordrg21;
    }
    else
    {
      ddata.savegamewpc.b16 = con(0xffffffff);
      SKW_443c_0434();
    }
    heroptrrg3 = ddata.hero;
    x16 wordrg22 = con(0x0);
    while (wordrg22 < ddata.savegamew2)
    {
      heroptrrg3->u32.b.b1 &= con(0x3);
      ddata.v1d67c2.b0[wordrg22] = con(0xffffffff);
      x16 wordrg402 = wordrg22;
      ddata.v1e0b1c[wordrg22] = false;
      v1d67be[wordrg22] = con(0xffffffff);
      ddata.v1e0b34[wordrg402][1] = con(0x0);
      ddata.v1d67c2.w0[wordrg402 + con(0x1)][1] = con(0xffff);
      ddata.v1e0b34[wordrg402][0] = con(0x0);
      ddata.v1d67c2.w0[wordrg402 + con(0x1)][0] = con(0xffff);
      ddata.v1e0b34[wordrg402][2] = con(0x0);
      wordrg22++;
      heroptrrg3++;
    }
    SKW_SET_PARTY_HERO_FLAG(con(0x4000));
    wordrg1 = signedlong(ddata.mouse_unk0e);
    if (wordrg1 == con(0xffffffff))
      return;
    ddata.mouse_unk0e = con(0xffffffff);
  }
  SKW_SELECT_CHAMPION_LEADER(wordrg1);
}

// was SKW_2fcf_0cdf
bool SKW_TRY_PUSH_OBJECT_TO(x16 eaxw, x16 ebxw, x16* ecxpw, x16 edxw, x16* argpw0)
{
  x16 wordrg5;
  const x8* bptrrg4;
  const x8* pb_00;
  x16 vw_08;

  x16 vw_0c;
  if (eaxw != con(0xffffffff))
  {
    vw_0c = con(0x0);
    bptrrg4 = &table1d6a64[0][0];
  }
  else
  {
    vw_0c = ddata.v1e0258;
    bptrrg4 = &table1d6a54[0][0];
  }
  bptrrg4 += 4 * SKW_RAND02(); // 2nd dimension
  pb_00 = bptrrg4;
  for (x16 wordrg3 = con(0x0);; wordrg3++)
  {
    if (wordrg3 > con(0x3))
      return false;
    x16 wordrg18 = (signedword(pb_00[wordrg3]) + vw_0c) & con(0x3);
    x16 wordrg2 = edxw + table1d27fc[wordrg18];
    wordrg5 = ebxw + table1d2804[wordrg18];
    x8 byterg1 = SKW_GET_TILE_VALUE(wordrg2, wordrg5);
    x16 wordrg48 = unsignedword(byterg1);
    if (SKW_IS_TILE_BLOCKED(byterg1))
      continue;
    x16 wordrg47 = unsignedword(CUTX8(wordrg48 >> con(0x5)));
    if (wordrg47 == con(0x3))
      continue;
    vw_08 = eaxw & con(0x3c00);
    if (urshift(vw_08, con(0xa)) == con(0x4) && (wordrg47 == con(0x5) || wordrg47 == con(0x2)))
      continue;
    x16 wordrg402 = SKW_GET_CREATURE_AT(wordrg2, wordrg5);
    if (wordrg402 == con(0xffffffff) || (eaxw == con(0xffffffff) && (SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg402) & con(0x8000)) != con(0x0)))
    {
      *ecxpw = wordrg2;
      *argpw0 = wordrg5;
      return true;
    }
  }
}

// was SKW_32cb_1f3e
bool SKW_DRAW_EXTERNAL_TILE(ui16 eaxuw)
{
  x8 byterg5;
  x16 wordrg5;
  s_hex12* h12ptrrg2;
  bool boolrg1;
  c_image vs_04;
  c_5bytes vs_140;
  bool vbool_14c;
  x16 vw_160;
  x16 vw_168;
  x16 vw_16c;
  x16 vw_170;
  x16 vw_174;
  x16 vw_17c;
  x16 vw_180;
  x16 vw_188;
  x8 vb_18c;
  x8 vb_190;

  vb_18c = table1d6c1e[eaxuw];
  if (vb_18c == con(0xffffffff))
    return false;
  h12ptrrg2 = &ddata.v1e1044[eaxuw];
  vw_168 = signedword(h12ptrrg2->b_00);
  vw_180 = signedword(h12ptrrg2->b_01);
  vw_17c = table1d6b15[eaxuw];
  boolrg1 = SKW_GET_TELEPORTER_DETAIL(&vs_140, vw_180, vw_168) ? 1 : 0;
  vbool_14c = boolrg1;
  if (vw_17c > con(0x3) && !boolrg1)
    return boolrg1;
  if (vbool_14c && eaxuw != con(0x1) && eaxuw != con(0x2) && unsignedword(vs_140.b_00) != ddata.v1e0258)
    if (unsignedword(vs_140.b_00) != ((ddata.v1e0258 + con(0x2)) & con(0x3)))
      return false;
  x16 wordrg21 = ddata.v1e1044[eaxuw].s_02.warr_00[con(0x5)];
  x8 vb_164 = CUTX8(urshift(wordrg21, con(0x8)));
  vb_18c += vb_164;
  vb_190 = CUTX16(wordrg21);
  if ((vb_190 == con(0xffffffff)) || (vb_190 == con(0x0)))
    return false;
  if (eaxuw == con(0xe) || eaxuw == con(0xf))
    if (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xa), con(0xb), con(0x61), vb_190) != con(0x0))
      return false;
  x16 vw_178 = con(0x0);
  if (table1d6afe[eaxuw] != con(0x0))
  {
    if (table1d6afe[eaxuw] >= con(0x1))
      vw_178 = con(0x1);
  }
  else if (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xa), con(0xb), con(0x7), vb_190) == con(0x0))
  {
    vw_178 = ddata.v1e12d0;
    if ((vw_17c & con(0x1)) == con(0x0))
      vw_178 ^= con(0x1);
  }
  vw_174 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xa), con(0xb), con(0x4), vb_190);
  if (vw_174 == con(0x0))
    vw_174 = ddata.v1d6c00;
  x16 wordrg161;
  x16 wordrg162 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xa), con(0xb), con(0x5), vb_190);
  if (wordrg162 != con(0x0))
  {
    vw_170 = urshift(wordrg162, con(0x8));
    wordrg161 = unsignedword(CUTX8(wordrg162)) - con(0x1);
  }
  else
  {
    wordrg161 = con(0xc);
    vw_170 = con(0x0);
  }
  vw_16c = SKW_QUERY_CREATURE_BLIT_RECTI(eaxuw, t_rotate(0), wordrg161);
  vw_188 = con(0x40);
  if (!SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0xa), con(0x1), vb_18c, vb_190))
  {
    vb_18c = table1d6c35[eaxuw] + vb_164;
    vw_188 = unsignedword(table1d6b71[vw_17c]);
  }
  if (eaxuw == con(0x1) || eaxuw == con(0x2))
    if (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xa), con(0xb), con(0x60), vb_190) != con(0x0))
      vw_188 = con(0x60);
  x16 vw_158;
  if (vw_17c == con(0x0) || (vw_158 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xa), con(0xb), con(0x63), vb_190), !vbool_14c && (vw_158 == con(0x0))))
  {
    SKW_QUERY_TEMP_PICST(vw_178, vw_188, con(0x0), vw_188, con(0x0), vw_17c, vw_16c, vw_170, vw_174, con(0xffff), con(0xa), vb_190, vb_18c);
    SKW_DRAW_TEMP_PICST();
    return false;
  }
  x16 wordrg6 = vw_178;
  SKW_QUERY_TEMP_PICST(wordrg6, vw_188, con(0x0), vw_188, con(0x0), vw_17c, vw_16c, vw_170, con(0xfffd), con(0xfffd), con(0xa), vb_190, vb_18c);
  if (table1d6afe[eaxuw] == con(0x0))
  {
    SKW_QUERY_TEMP_PICST(wordrg6, vw_188, ddata.v1e118e.w_1c, vw_188, ddata.v1e118e.w_1e, vw_17c, vw_16c, vw_170, vw_174, con(0xffff), con(0xa), vb_190, vb_18c + con(0xffffffc8));
    SKW_DRAW_TEMP_PICST();
    vw_168 = unsignedword(vs_140.b_02);
    vw_180 = unsignedword(vs_140.b_03);
    x16 vw_184 = ddata.v1e12c8;
    ddata.v1e12c8 = unsignedword(vs_140.b_01);
    vw_160 = ddata.v1d3248;
    SKW_CHANGE_CURRENT_MAP_TO(unsignedword(vs_140.b_04));
    ddata.v1d6bfa = con(0x1);
    wordrg6 = signedword(table1d6b15[eaxuw]);
    SKW_CALC_VECTOR_W_DIR(ddata.v1e12c8, con(0x0), &vw_168, -wordrg6, &vw_180);
    wordrg5 = con(0x4);
    while (wordrg5 >= wordrg6)
    {
      SKW_32cb_4185(vw_168, table1d6c19[wordrg5], ddata.v1e12c8, vw_180);
      wordrg5--;
    }
    wordrg5 = con(0x4);
    while (wordrg5 > wordrg6)
    {
      SKW_DRAW_STATIC_OBJECT(table1d6c19[wordrg5], true, con(0x1ffffff));
      wordrg5--;
    }
    SKW_CHANGE_CURRENT_MAP_TO(vw_160);
    ddata.v1d6bfa = con(0x0);
    ddata.v1e12c8 = vw_184;
    SKW_DRAW_STATIC_OBJECT(eaxuw, true, con(0x3ff));
  }
  x16 wordrg3 = SKW_QUERY_MULTILAYERS_PIC(&vs_04, vb_190, vb_18c, con(0xa), vw_188, vw_188, vw_17c, vw_178, vw_174, DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xa), con(0xb), con(0x11), vb_190));
  if (vs_04.w_38 > con(0x0))
    byterg5 = vs_04.carr_3a[vw_174];
  else
    byterg5 = vw_174;
  if (vw_158 == con(0x0))
    SKW_FILL_ENTIRE_PICT(SKW_3e74_5817(wordrg3), byterg5);
  else
    SKW_32cb_0c7d(&vs_04, vw_174, wordrg3);
  vs_04.pg_2c = SKW_3e74_5817(wordrg3);
  SKW_DRAW_PICST(&vs_04);
  SKW_0b36_01cd(&vs_04.s_00);
  vs_04.w_30 = unsignedword(byterg5);
  vs_04.pg_2c = ddata.bitmapptr;
  vs_04.w_18 = vw_16c;
  vs_04.w_1a = vw_170;
  vs_04.w_38 = con(0x0);
  SKW_0b36_00c3(wordrg3, &vs_04.s_00);
  SKW_DRAW_PICST(&vs_04);
  SKW_3e74_58a3(wordrg3);
  return vbool_14c;
}

bool SKW_DRAW_ITEM_SURVEY(x16 eaxw, bool edxbool)
{
  x16 wordrg5;
  x8 byterg5;
  c_record* recoptrrg7;
  t_text t_00[0x80];
  x32 vla_80[0x4];

  vla_80[0x0] = con(0x02710270); // original location: 0x289ac
  if (eaxw == con(0xffffffff))
    return false;
  wordrg5 = eaxw;
  recoptrrg7 = SKW_GET_ADDRESS_OF_RECORD(wordrg5);
  SKW_DRAW_GUIDED_STR(v1d10f0);
  vla_80[0x2] = unsignedlong(urshift(eaxw & con(0x3c00), con(0xa)));
  if (vla_80[0x2] == con(0x7))
  {
    SKW_DRAW_SCROLL_TEXT(wordrg5);
    return true;
  }
  ddata.v1d66fc = con(0x3);
  if (!edxbool)
    return false;
  SKW_DRAW_STATIC_PIC(con(0x7), con(0x1), con(0x1ee), con(0x0), con(0xffffffff));
  vla_80[0x1] = UI32CAST(SKW_QUERY_CLS2_FROM_RECORD(wordrg5) & con(0xff));
  byterg5 = unsignedlong(SKW_QUERY_CLS1_FROM_RECORD(wordrg5));
  if (SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(byterg5, con(0x1), con(0x11), CUTLX8(vla_80[0x1])))
    SKW_DRAW_STATIC_PIC(byterg5, con(0x11), con(0x1ee), CUTLX8(vla_80[0x1]), con(0xc));
  SKW_DRAW_STATIC_PIC(con(0x7), con(0x5), con(0x1f8), con(0x0), con(0xc));
  wordrg5 = eaxw;
  SKW_DRAW_VP_RC_STR(con(0x1fa), SKW_GET_ITEM_NAME(wordrg5), unsignedword(ddata.paletteptr1[con(0xd)]));
  SKW_DRAW_ITEM_ICON(wordrg5, con(0x0), false, con(0x2e), false);
  x16 wordrg22 = SKW_QUERY_ITEM_WEIGHT(wordrg5);
  ddata.v1e099a = wordrg22 / con(0xa);
  ddata.v1e098e = wordrg22 % con(0xa);
  SKW_DRAW_GUIDED_STR(SKW_QUERY_GDAT_TEXT(con(0x7), con(0x1f), t_00, con(0x0)));
  if (I16CAST(vla_80[0x2]) == con(0x9))
  {
    if (!SKW_IS_CONTAINER_CHEST(wordrg5))
    {
      if (SKW_IS_CONTAINER_MONEYBOX(wordrg5))
        SKW_MONEY_BOX_SURVEY(wordrg5);
    }
    else
      SKW_DRAW_CONTAINER_SURVEY(recoptrrg7);
    return true;
  }
  x8 byterg3 = con(0x0);
  vla_80[0x3] = con(0x0);
  if (SKW_2759_0155(wordrg5))
  {
    x16 wordrg21 = SKW_ADD_ITEM_CHARGE(wordrg5, con(0x0));
    if (wordrg21 != con(0x0))
    {
      x16 wordrg11 = signedlong(SKW_GET_MAX_CHARGE(wordrg5));
      vla_80[0x3] = con(0x1); // extended
      byterg3 = con(0x71);
      SKW_DRAW_ITEM_STATS_BAR(CUTX16(vla_80[0x0]), wordrg11, byterg3, wordrg21, con(0x4));
    }
  }
  x16 wordrg2;
  ui16 uwordrg23 = urshift(eaxw & con(0x3c00), con(0xa));
  x16 wordrg1;
  if (uwordrg23 < con(0x6))
  {
    if (uwordrg23 != con(0x5))
      return true;
    byterg3 = con(0x72);
    wordrg2 = con(0x50);
    wordrg1 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(eaxw, con(0x8));
    if (wordrg1 == con(0x0))
    {
      wordrg1 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(eaxw, con(0x9));
      byterg3 = con(0x73);
      wordrg2 = con(0x64);
    }
  }
  else
  {
    if (uwordrg23 <= con(0x6))
    {
      byterg3 = con(0x72);
      wordrg1 = unsignedword(CUTX8(SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(eaxw, con(0xb))));
      wordrg2 = con(0xc8);
    }
    else
    {
      if (uwordrg23 != con(0x8))
        return true;
      byterg3 = con(0x75);
      wordrg1 = recoptrrg7->u2.w & con(0xff);
      wordrg2 = con(0xff);
    }
  }
  if (wordrg1 != con(0x0))
    SKW_DRAW_ITEM_STATS_BAR(CUTX16(vla_80[vla_80[0x3] / 2]), wordrg2, byterg3, wordrg1, con(0x5));
  return true;
}

void SKW_2c1d_0009(x16 eaxw, x16 ebxw, x16 ecxw, x8 edxb)
{
  x16 wordrg5;
  c_tim* timptrrg3;
  c_tim vtim_04;
  x16 vw_14;
  x8 vb_18;

  x16 vw_00 = eaxw;
  vb_18 = edxb;
  wordrg5 = ebxw;
  vw_14 = ecxw;
  bool boolrg6 = false;
  for (x16 vw_10 = con(0x0); vw_10 < con(0x4); vw_10++)
  {
    x16 wordrg404 = con(0x1) << CUTX8(vw_10);
    if ((wordrg404 & vw_00) != con(0x0))
    {
      if (ddata.hero[vw_10].w36 == con(0x0))
      {
        wordrg404 = ~wordrg404;
        vw_00 &= wordrg404;
      }
      if ((vb_18 != ddata.hero[vw_10].b102) || (ddata.hero[vw_10].w36 == con(0x0)))
      {
        ddata.hero[vw_10].w103 = con(0x0);
        x16 wordrg420 = con(0x0);
        timptrrg3 = ddata.savegametimarrp;
        while (wordrg420 < ddata.savegamew9)
        {
          if ((timptrrg3->b_04 == con(0x48)))
          {
            x16 wordrg19 = unsignedword(timptrrg3->b_05);
            if ((vw_00 & wordrg19) != con(0x0))
            {
              if ((~vw_00 & wordrg19) != con(0x0))
                timptrrg3->b_05 &= ~CUTX8(vw_00);
              else
                SKW_DELETE_TIMER(wordrg420);
            }
          }
          timptrrg3++;
          wordrg420++;
        }
      }
    }
    if (ddata.hero[vw_10].w103 > con(0x32))
      boolrg6 = true;
  }
  if (boolrg6)
    wordrg5 >>= con(0x2);
  x8 byterg402 = vb_18;
  for (x16 vw_10_1 = con(0x0); vw_10_1 < ddata.savegamew2; vw_10_1++)
    if (((con(0x1) << CUTX8(vw_10_1)) & vw_00) != con(0x0))
    {
      ddata.hero[vw_10_1].b102 = byterg402;
      ddata.hero[vw_10_1].w103 += wordrg5;
    }
  vtim_04.u.w_06 = wordrg5;
  vtim_04.b_04 = con(0x48);
  vtim_04.b_05 = CUTX8(vw_00);
  vtim_04.un.l_00 = (unsignedlong(vw_14) + ddata.longmallocx) | (signedlong(ddata.v1e0266) << con(0x18));
  SKW_QUEUE_TIMER(&vtim_04);
}

x16 SKW_WOUND_PLAYER(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  x16 uwordrg5;
  c_hero* heroptrrg7;
  x16 vw_08;
  x16 vw_10;

  x16 wordrg6 = edxw;
  vw_10 = ebxw;
  x16 vw_0c = ecxw;
  x16 wordrg4 = eaxw;
  if (wordrg4 == con(0xffffffff))
    return con(0x0);
  if ((wordrg4 + con(0x1)) == ddata.v1e0288 || ddata.v1e0240 || wordrg6 <= con(0x0))
    return con(0x0);
  heroptrrg7 = &ddata.hero[wordrg4];
  if (heroptrrg7->w36 == con(0x0))
    return con(0x0);
  vw_08 = ecxw & con(0x8000);
  vw_0c &= con(0x7fff);
  if (vw_0c != con(0x0))
  {
    x16 vw_14 = con(0x0);
    x16 wordrg21 = con(0x0);
    uwordrg5 = con(0x0);
    while (wordrg21 <= con(0x5))
    {
      if (((con(0x1) << wordrg21) & unsignedlong(vw_10)) != con(0x0))
      {
        vw_14++;
        x16 wordrg104;
        if (vw_0c != con(0x4))
          wordrg104 = con(0x0);
        else
          wordrg104 = con(0x8000);
        wordrg104 |= wordrg21;
        uwordrg5 += SKW_2c1d_135d(eaxw, wordrg104);
      }
      wordrg21++;
    }
    if (vw_14 != con(0x0))
      uwordrg5 /= vw_14;
    x16 wordrg3 = con(0x0);
    x16 wordrg2 = con(0x0);
    while (wordrg2 <= con(0x1))
    {
      if (heroptrrg7->b20[wordrg2] == con(0x1))
        wordrg3 += signedword(heroptrrg7->b42[wordrg2]);
      wordrg2++;
    }
    if (wordrg3 != con(0x0))
    {
      wordrg2 = con(0x1);
      if ((SKW_QUERY_PLAYER_SKILL_LV(eaxw, true, con(0x7)) + wordrg3 / 8) > CUTX16(SKW_RAND() & con(0xf)))
      {
        if (vw_08 != con(0x0))
        {
          wordrg6 -= wordrg3;
          if (wordrg6 <= con(0x0))
            return con(0x0);
        }
        uwordrg5 += wordrg3 / 4;
      }
    }
    ui16 uwordrg1 = vw_0c - con(0x1);
    x16 step = -1;
    if (uwordrg1 > con(0x8))
      step = 0;
    else
    {
      x16 secondswitch = uwordrg1;
      switch (uwordrg1)
      {
        case 0:
          wordrg2 = wordrg6;
          wordrg6 = SKW_2c1d_0f2a(heroptrrg7, wordrg2, con(0x6));
          if (heroptrrg7->b102 == con(0x0))
            wordrg6 -= heroptrrg7->w103;
          step = 0;
        break;

        case 1:
        case 7:
          uwordrg5 >>= con(0x1);
          wordrg2 = con(0x1);
          uwordrg5 += SKW_QUERY_PLAYER_SKILL_LV(eaxw, true, con(0x1));
          step = 0;
        break;

        case 2:
        case 3:
        case 6:
        case 8:
          step = 0;
        break;

        case 4:
          wordrg2 = wordrg6;
          wordrg6 = SKW_2c1d_0f2a(heroptrrg7, wordrg2, con(0x5));
          if (heroptrrg7->b102 == con(0x1))
            wordrg6 -= heroptrrg7->w103;
          step = 2;
        break;

        case 5:
          wordrg2 = con(0x73) - SKW_GET_PLAYER_ABILITY(heroptrrg7, con(0x0), con(0x3));
          if (wordrg2 <= con(0x0))
            return con(0x0);
          step = 1;
        break;

        default: throw(THROW_DMABORT);
      }
    }
    switch (step)
    {
      case 0: // no break
        if (wordrg6 <= con(0x0))
          return con(0x0);
        wordrg2 = con(0x82) - uwordrg5;
      case 1: // no break then
        wordrg6 = SKW_0cd5_0176(wordrg6, wordrg2, con(0x6));
      case 2:
        if (wordrg6 <= con(0x0))
          return con(0x0);
        wordrg2 = SKW_2c1d_0f2a(heroptrrg7, (CUTX16(SKW_RAND()) & con(0x7f)) + con(0xa), con(0x4));
        if (wordrg6 > wordrg2)
        {
          do
          {
            ddata.v1e0ba8[eaxw] |= ((con(0x1) << (SKW_RAND() & con(0x7))) & vw_10);
            wordrg2 *= con(0x2);
          } while (wordrg6 > wordrg2 && wordrg2 != con(0x0));
        }
        if (ddata.v1e0238)
          SKW_RESUME_FROM_WAKE();
      break;
    }
  }
  ddata.v1e0bb0[eaxw] += wordrg6;
  return wordrg6;
}

// was SKW_2c1d_210a
void SKW_UPDATE_CHAMPIONS_STATS(void)
{
  x16 wordrg5;
  x16 uwordrg5;
  c_hero* heroptrrg3;
  x32 help; // TODO
  x16 vw_08;

  if (ddata.savegamew2 == con(0x0))
    return;
  ddata.v1e0ff8 += con(0x38);
  if (ddata.v1e0ff8 > con(0x80))
    ddata.v1e0ff8 -= con(0x80);
  wordrg5 = con(0x0);
  ui16 vuw_04 = wordrg5;
  heroptrrg3 = ddata.hero;
  for(;;)
  {
    if (vuw_04 >= ddata.savegamew2)
      return;
    if (heroptrrg3->w36 != con(0x0))
    {
      if ((vuw_04 + con(0x1)) != ddata.v1e0288)
      {
        if (heroptrrg3->w3e >= heroptrrg3->w40)
        {
          if (heroptrrg3->w3e > heroptrrg3->w40)
          {
            wordrg5 = heroptrrg3->w3e - con(0x1);
            heroptrrg3->w3e = wordrg5;
          }
        }
        else
        {
          uwordrg5 = SKW_QUERY_PLAYER_SKILL_LV(vuw_04, true, con(0x3)) + SKW_QUERY_PLAYER_SKILL_LV(vuw_04, true, con(0x2));
          vw_08 = uwordrg5;
          if (ddata.v1e0ff8 < (SKW_GET_PLAYER_ABILITY(heroptrrg3, con(0x0), con(0x3)) + uwordrg5))
          {
            wordrg5 = heroptrrg3->w40 / con(0x28) + con(0x1);
            if (ddata.v1e0238)
              wordrg5 *= con(0x2);
            SKW_ADJUST_STAMINA(vuw_04, DM2_MAX(con(0x7), con(0x10) - vw_08) * wordrg5);
            heroptrrg3->w3e += DM2_MIN(wordrg5, heroptrrg3->w40 - heroptrrg3->w3e);
          }
        }
        ui16 uwordrg6 = con(0x4);
        wordrg5 = heroptrrg3->u3c.w;
        for(;;)
        {
          wordrg5 >>= con(0x1);
          if (wordrg5 <= heroptrrg3->w3a)
            break;
          uwordrg6 += con(0x2);
        }
        wordrg5 = con(0x0);
        x16 wordrg100 = DM2_BETWEEN_VALUE(con(0x1), con(0x6), signedword(heroptrrg3->u3c.b.b1) - con(0x1));
        vw_08 = CUTX16(ddata.longmallocx) - CUTX16(ddata.longv1e01a0); // TODO: really not longs?
        if (vw_08 > con(0x50))
        {
          wordrg100++;
          if (vw_08 > con(0xfa))
            wordrg100++;
        }
        if (ddata.v1e0238)
          wordrg100 *= 2;
        for(;;)
        {
          bool boolrg2 = uwordrg6 <= con(0x4);
          vw_08 = boolrg2 ? 1 : 0;
          if (heroptrrg3->w44 >= con(0xfffffe00))
          {
            x16 wordrg45;
            if (heroptrrg3->w44 >= con(0x0))
              wordrg5 -= wordrg100;
            if (vw_08 == con(0x0))
              wordrg45 = uwordrg6 / 2;
            else
              wordrg45 = con(0x2);
            heroptrrg3->w44 -= wordrg45;
          }
          else
          {
            if (boolrg2)
            {
              wordrg5 += wordrg100;
              heroptrrg3->w44 -= con(0x2);
            }
          }
          if (heroptrrg3->w46 >= con(0xfffffe00))
          {
            if (heroptrrg3->w46 >= con(0x0))
              wordrg5 -= wordrg100;
            x16 wordrg41;
            if (vw_08 == con(0x0))
              wordrg41 = uwordrg6 / 4;
            else
              wordrg41 = con(0x1);
            heroptrrg3->w46 -= wordrg41;
          }
          else
          {
            if (vw_08 != con(0x0))
            {
              wordrg5 += wordrg100;
              heroptrrg3->w46--;
            }
          }
          if (--uwordrg6 == con(0x0) || ((signedlong(heroptrrg3->w3a) - signedlong(wordrg5)) >= signedlong(heroptrrg3->u3c.w)))
          {
            SKW_ADJUST_STAMINA(vuw_04, wordrg5);
            if (heroptrrg3->w44 < con(0xfffffc00))
              heroptrrg3->w44 = con(0xfffffc00);
            if (heroptrrg3->w46 < con(0xfffffc00))
              heroptrrg3->w46 = con(0xfffffc00);
            if (heroptrrg3->w36 < heroptrrg3->w38 && heroptrrg3->w3a >= (heroptrrg3->u3c.w >> con(0x2)))
            {
              if (ddata.v1e0ff8 < (SKW_GET_PLAYER_ABILITY(heroptrrg3, con(0x0), con(0x4)) + con(0xc)))
              {
                x16 wordrg107 = (heroptrrg3->w38 >> con(0x7)) + con(0x1);
                if (ddata.v1e0238)
                  wordrg107 *= 2;
                wordrg107 = DM2_MIN(wordrg107, heroptrrg3->w38 - heroptrrg3->w36);
                heroptrrg3->w36 += wordrg107;
              }
            }
            x16 wordrg106;
            if (!ddata.v1e0238)
              wordrg106 = con(0xff);
            else
              wordrg106 = con(0x3f);
            if ((wordrg106 & CUTX16(ddata.longmallocx)) == con(0x0))
            {
              wordrg5 = con(0x0);
              while (wordrg5 <= con(0x6))
              {
                help = 2 * wordrg5;
                vw_08 = unsignedword(heroptrrg3->b4a[help + con(0x1)]);
                uwordrg6 = unsignedword(heroptrrg3->b4a[help]);
                if (uwordrg6 >= unsignedword(heroptrrg3->b4a[help + con(0x1)]))
                {
                  if (uwordrg6 > vw_08)
                  {
                    uwordrg6 -= uwordrg6 / vw_08;
                    heroptrrg3->b4a[help] = CUTX8(uwordrg6);
                  }
                }
                else
                  heroptrrg3->b4a[help] += con(0x1);
                wordrg5++;
              }
            }
            heroptrrg3->u32.b.b1 |= con(0x8);
            x16 wordrg101 = vuw_04 + con(0x1);
            if (wordrg101 == ddata.v1e0976)
              heroptrrg3->u32.b.b1 |= con(0x30);
            break;
          }
        }
      }
    }
    vuw_04++;
    heroptrrg3++;
  }
}

bool SKW_2e62_03b5(x16 eaxw, bool ebxbool, x16 edxw)
{
  x16 wordrg5;
  x16 vw_10;

  bool boolrg7 = ebxbool;
  bool vbool_0c = false;
  bool vbool_04 = false;
  bool vbool_08 = ebxbool;
  x16 wordrg17 = eaxw + con(0x1);
  if (ddata.v1e0976 == wordrg17)
    wordrg5 = edxw + con(0x8);
  else
  {
    if (edxw > con(0x1) || ddata.v1e0288 == wordrg17)
      return false;
    wordrg5 = 2 * eaxw + edxw;
  }
  x16 wordrg2 = eaxw;
  vw_10 = ddata.hero[wordrg2].warrc3[edxw];
  if (edxw <= con(0x1))
  {
    wordrg2++;
    if (wordrg2 == ddata.v1e0b74)
      if (edxw == ddata.v1e0b76)
        vbool_0c = true;
    if (!vbool_0c != (ddata.v1d653c[wordrg5].b5 & con(0x1)) == con(0x0))
    {
      vbool_08 = true;
      ddata.v1d653c[wordrg5].b5 ^= con(0x1);
    }
  }
  if (edxw <= con(0x5))
  {
    if (((con(0x1) << CUTX8(edxw)) & ddata.hero[eaxw].w34) != con(0x0))
      vbool_04 = true;
    if (!vbool_04 != (ddata.v1d653c[wordrg5].b5 & con(0x2)) == con(0x0))
    {
      vbool_08 = true;
      ddata.v1d653c[wordrg5].b5 ^= con(0x2);
    }
  }
  if (vw_10 != con(0xffffffff))
  {
    x16 wordrg3 = vw_10;
    x8 byterg3 = SKW_2405_014a(wordrg3, (SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(wordrg3, con(0x0)) & con(0x8000)) != con(0x0), edxw);
    if (byterg3 != ddata.v1d653c[wordrg5].b3)
    {
      boolrg7 = true;
      ddata.v1d653c[wordrg5].b3 = byterg3;
    }
    x8 byterg1 = SKW_QUERY_CLS2_FROM_RECORD(vw_10);
    if (byterg1 != ddata.v1d653c[wordrg5].b4)
    {
      boolrg7 = true;
      ddata.v1d653c[wordrg5].b4 = byterg1;
    }
  }
  if (!boolrg7 && !vbool_08)
    if (ddata.v1d653c[wordrg5].w6 == vw_10)
      return false;
  if (vw_10 == con(0xffffffff) || wordrg5 < con(0x8))
    vbool_08 = true;
  if (wordrg5 < con(0x8))
    SKW_DRAW_PLAYER_3STAT_PANE(eaxw, false);
  ddata.v1d653c[wordrg5].w6 = vw_10;
  SKW_DRAW_ITEM_ICON(vw_10, vbool_04 ? 1 : 0, vbool_0c, wordrg5, vbool_08);
  return true;
}

void R_3C1E5(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0)
{
  x16 wordrg5;
  c_record* recoptrrg5;
  x16 vw_00;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;
  c_tim vtim_10;
  x16 vw_28;
  x16 vw_34;
  x16 vw_38;
  x16 vw_3c;
  x16 vw_40;
  x16 vw_44;
  x16 vw_48;
  x16 vw_4c;
  x16 vw_50;
  x8 vb_58;
  bool flag;

  vw_0c = eaxw;
  vw_08 = edxw;
  vw_04 = ebxw;
  vw_00 = ecxw;
  x16 wordrg6 = argw0;
  bool vbool_20 = false;
  bool vbool_1c = false;
  x16 vw_2c = con(0xffff);
  vw_48 = SKW_GET_TILE_RECORD_LINK(vw_0c, vw_08);
  vw_28 = vw_48;
  bool vbool_24 = false;
  // M_3C226, M_3CDE8 is continue-label
  for (;; vw_48 = SKW_GET_NEXT_RECORD_LINK(vw_48))
  {
    x16 wordrg2 = vw_48;
    if (wordrg2 == con(0xfffffffe) || vbool_24)
      break;
    vw_40 = con(0x0);
    x16 wordrg3 = wordrg2;
    vw_4c = urshift(wordrg3, con(0xe));
    if (vw_4c != vw_04)
      continue;
    if (!SKW_IS_WALL_ORNATE_ALCOVE_FROM_RECORD(vw_28, wordrg3))
    {
      if (!SKW_0cee_319e(wordrg3))
      {
        x16 wordrg130 = urshift(wordrg2 & con(0x3c00), con(0xa));
        if (wordrg130 != con(0x3))
        {
          if (vw_00 != con(0xffffffff) || wordrg130 != con(0x2))
          {
            if (wordrg130 >= con(0x3))
              break;
            continue;
          }
          recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(wordrg3);
          if ((recoptrrg5->u2.w & con(0x6)) != con(0x2))
            continue;
          x16 wordrg413 = recoptrrg5->u2.w;
          ui16 uwordrg2 = urshift(wordrg413, con(0x3));
          x16 vw_54 = unsignedword(CUTX8(uwordrg2));
          vb_58 = CUTX8(vw_54);
          if (ddata.mouse_unk0e == con(0xffffffff))
            continue;
          uwordrg2 = urshift(uwordrg2, con(0x8)) & con(0x1f);
          vw_34 = uwordrg2;
          bool doit = false; // added
          if (uwordrg2 < con(0x7))
          {
            if (uwordrg2 < con(0x4))
              continue;
            if (uwordrg2 <= con(0x4))
              doit = true;
            else if (uwordrg2 != con(0x6))
              continue;
          }
          else if (uwordrg2 <= con(0x7))
          {
            if ((wordrg413 & con(0x1)) != con(0x0))
              continue;
          }
          else
          {
            if (uwordrg2 <= con(0x8))
              doit = true;
            else
            {
              if (uwordrg2 == con(0xd) && wordrg6 == con(0xffffffff))
              {
                x16 wordrg21 = wordrg413 ^ con(0x1);
                wordrg413 &= con(0xfffffffe);
                wordrg413 |= wordrg21;
                recoptrrg5->u2.w = wordrg413;
              }
              continue;
            }
          }
          if (doit)
          {
            x16 wordrg411 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0xe), vb_58);
            x16 vw_30 = vw_4c << con(0xe);
            if ((recoptrrg5->u2.b.b0 & con(0x1)) == con(0x0))
            {
              if (vw_34 == con(0x8))
              {
                ddata.v1e0390.b0 |= con(0x1);
                vbool_24 = true;
                continue;
              }
              if ((wordrg6 == con(0xffffffff)) || SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg6) != wordrg411)
                continue;
              vbool_1c = true;
              wordrg6 &= con(0xffff3fff);
              SKW_MOVE_RECORD_TO(wordrg6 | vw_30, con(0x0), vw_0c, con(0xffffffff), vw_08);
              wordrg6 = con(0xffff);
              ddata.v1e0390.b0 |= con(0x1);
              vbool_24 = true;
              continue;
            }
            if (wordrg6 != con(0xffffffff))
            {
              vbool_24 = true;
              continue;
            }
            wordrg5 = wordrg411;
            wordrg6 = SKW_FIND_DISTINCTIVE_ITEM_ON_TILE(vw_0c, vw_4c, wordrg5, vw_08);
            if (wordrg6 == con(0xffffffff))
            {
              wordrg6 = SKW_ALLOC_NEW_DBITEM(wordrg5);
              if (wordrg6 == con(0xffffffff))
                continue;
              wordrg6 = vw_30 | (wordrg6 & con(0x3fff));
              SKW_APPEND_RECORD_TO(wordrg6, vw_0c, vw_08, NULL);
            }
            wordrg5 = wordrg6;
            SKW_ADD_ITEM_CHARGE(wordrg5, con(0xf));
            SKW_MOVE_RECORD_TO(wordrg5, vw_08, con(0xffffffff), vw_0c, con(0x0));
            vw_2c = wordrg6;
            wordrg6 = con(0xffff);
            if (vw_34 == con(0x8))
            {
              vtim_10.un.l_00 = (ddata.longmallocx + unsignedlong(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0x12), vb_58)) + con(0x2)) | (signedlong(ddata.v1d3248) << con(0x18));
              vtim_10.b_04 = con(0x5c);
              vtim_10.b_05 = con(0x0);
              vtim_10.u.w_06 = vw_48;
              SKW_QUEUE_TIMER(&vtim_10);
            }
            ddata.v1e0390.b0 |= con(0x1);
            vbool_24 = true;
            continue;
          }
          if (wordrg6 == con(0xffffffff))
          {
            x16 wordrg409 = (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0x11), vb_58) == con(0x0)) ? con(0x1) : con(0xffffffff);
            vw_3c = ddata.v1e0270;
            vw_38 = ddata.v1e0272;
            x16 wordrg134 = SKW_LOCATE_OTHER_LEVEL(ddata.v1e0266, &vw_3c, &vw_38, wordrg409, NULL);
            if (wordrg134 >= con(0x0))
            {
              R_3BF83(vw_3c, wordrg134, ddata.v1e0258, vw_38);
              SKW_1031_098e();
            }
          }
          continue;
        }
        recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(wordrg3);
        vw_34 = recoptrrg5->u2.w & con(0x7f);
        vw_50 = urshift(recoptrrg5->u2.w, con(0x7));
        vw_44 = ulrshift(recoptrrg5->u4.w, con(0xb), con(0xe));
        if (ddata.mouse_unk0e == con(0xffffffff) && vw_34 != con(0x7e))
          continue;
        x8 byterg40 = SKW_GET_WALL_DECORATION_OF_ACTUATOR(recoptrrg5);
        vb_58 = byterg40;
        ui16 uwordrg20 = vw_34;
        if (uwordrg20 == con(0x0))
          continue;
        bool boolrg7 = true;
        x16 path = 0; // 0 = M_3C614, 1 = M_3CA71, 2 = M_3C693
        if (uwordrg20 < con(0x1a))
        {
          if (uwordrg20 < con(0x3))
          {
            if (uwordrg20 < con(0x1))
              continue;
            if (uwordrg20 <= con(0x1))
            {
              boolrg7 = false;
              if ((recoptrrg5->u4.w & con(0x18)) == con(0x18))
                continue;
            }
            else
              boolrg7 = (((wordrg6 == con(0xffffffff)) ? 1 : 0) != ulrshift(recoptrrg5->u4.w, con(0xa), con(0xf)));
            path = 1;
          }
          else if (uwordrg20 <= con(0x3))
            path = 0;
          else if (uwordrg20 < con(0x17))
          {
            if (uwordrg20 != con(0x15))
              continue;
            if (SKW_ADD_ITEM_CHARGE(unsignedlong(wordrg6), con(0x0)) == con(0x0))
              path = 1;
            else // BUGFIX 4/2014
              path = 0;
          }
          else if (uwordrg20 <= con(0x17))
          {
            if (wordrg6 != con(0xffffffff))
              continue;
            x16 wordrg139 = (ulrshift(recoptrrg5->u4.w, con(0xd), con(0xf)) ^ con(0x1)) & con(0x1);
            recoptrrg5->u4.b.b0 &= con(0xfffffffb);
            recoptrrg5->u4.w |= 4 * wordrg139;
            x32 longrg40 = unsignedlong(ulrshift(recoptrrg5->u4.w, con(0xd), con(0xf)));
            boolrg7 = (unsignedlong(ulrshift(recoptrrg5->u4.w, con(0xa), con(0xf))) ^ longrg40) == con(0x0);
            path = 1;
          }
          else if (uwordrg20 == con(0x18))
            path = 2;
          else
            continue;
        }
        else if (uwordrg20 <= con(0x1a))
        {
          x16 wordrg407 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0xe), byterg40);
          if ((recoptrrg5->u4.b.b0 & con(0x4)) == con(0x0))
          {
            if ((wordrg6 == con(0xffffffff)) || SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg6) != wordrg407)
              continue;
            vbool_1c = boolrg7;
            wordrg6 &= con(0xffff3fff);
            SKW_MOVE_RECORD_TO((vw_4c << con(0xe)) | wordrg6, con(0x0), vw_0c, con(0xffffffff), vw_08);
            wordrg6 = con(0xffff);
            ddata.v1e0390.b0 |= con(0x1);
            vbool_24 = true;
            continue;
          }
          if (wordrg6 == con(0xffffffff))
          {
            wordrg5 = wordrg407;
            wordrg6 = SKW_FIND_DISTINCTIVE_ITEM_ON_TILE(vw_0c, vw_4c, wordrg5, vw_08);
            if (wordrg6 == con(0xffffffff))
            {
              wordrg6 = SKW_ALLOC_NEW_DBITEM(wordrg5);
              if (wordrg6 == con(0xffffffff))
                continue;
              wordrg6 &= con(0xffff3fff);
              wordrg6 |= vw_4c << con(0xe);
              SKW_APPEND_RECORD_TO(wordrg6, vw_0c, vw_08, NULL);
            }
            wordrg5 = wordrg6;
            SKW_ADD_ITEM_CHARGE(wordrg5, con(0xf));
            SKW_MOVE_RECORD_TO(wordrg5, vw_08, con(0xffffffff), vw_0c, con(0x0));
            vw_2c = wordrg6;
            wordrg6 = con(0xffff);
            ddata.v1e0390.b0 |= con(0x1);
            vbool_24 = true;
            continue;
          }
          vbool_24 = boolrg7;
          continue;
        }
        else if (uwordrg20 < con(0x3f))
        {
          if (uwordrg20 <= con(0x1b))
          {
            if (vw_50 == con(0x0) || SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg6) != DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0xe), byterg40))
              continue;
            vbool_1c = boolrg7;
            wordrg6 = con(0xffff);
            vw_50 = vw_50 - con(0x1);
            flag = vw_50 != con(0x0);
            if (!flag)
            {
              boolrg7 = false;
              recoptrrg5->u4.b.b0 |= con(0x1);
            }
            x16 wordrg402 = recoptrrg5->u2.w & con(0xffff007f);
            recoptrrg5->u2.w = wordrg402 | ((vw_50 & con(0x1ff)) << con(0x7));
          }
          else if (uwordrg20 == con(0x1c))
          {
            if (wordrg6 == con(0xffffffff))
              continue;
            if ((recoptrrg5->u4.b.b0 & con(0x4)) != con(0x0))
              continue;
            x16 wordrg401;
            if ((recoptrrg5->u4.b.b0 & con(0x20)) == con(0x0))
              wordrg401 = (ddata.v1e0258 + ulrshift(recoptrrg5->u4.w, con(0xb), con(0xe))) & con(0x3);
            else
              wordrg401 = ulrshift(recoptrrg5->u4.w, con(0xb), con(0xe));
            x16 wordrg31 = wordrg401;
            x16 wordrg24 = vw_50 & con(0x3f);
            x16 vw_54 = recoptrrg5->u6.w;
            x16 wordrg402 = urshift(recoptrrg5->u6.w, con(0xb));
            vw_54 = ulrshift(vw_54, con(0x5), con(0xb));
            R_3BF83(vw_54, wordrg24, wordrg31, wordrg402);
            SKW_1031_098e();
          }
          else
            continue;
          path = 1;
        }
        else if (uwordrg20 <= con(0x3f))
        {
          if (wordrg6 == con(0xffffffff))
            recoptrrg5->u4.b.b0 &= con(0xfffffffe);
          path = 1;
        }
        else if (uwordrg20 < con(0x4a))
        {
          if (uwordrg20 != con(0x46))
            continue;
          x16 wordrg40 = urshift(recoptrrg5->u6.w, con(0xb));
          if ((SKW_GET_ADDRESS_OF_TILE_RECORD(ulrshift(recoptrrg5->u6.w, con(0x5), con(0xb)), wordrg40)->u2.b.b1 & con(0x20)) != con(0x0))
            path = 2;
          else
            continue;
        }
        else if (uwordrg20 <= con(0x4a))
          path = 2;
        else if (uwordrg20 == con(0x7e))
        {
          if ((recoptrrg5->u4.b.b0 & con(0x4)) != con(0x0))
            if (vw_04 == ((ddata.v1e0258 + con(0x2)) & con(0x3)))
            {
              SKW_SELECT_CHAMPION(ddata.v1e0270, ddata.v1e0258, ddata.v1e0266, ddata.v1e0272);
              vbool_24 = boolrg7;
              boolrg7 = false;
            }
          path = 1;
        }
        else
          continue;
        if (path == 0)
        {
          x8 boolrg1 = vw_50 == SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg6);
          boolrg7 = ((boolrg1 ? 1 : 0) == ulrshift(recoptrrg5->u4.w, con(0xa), con(0xf)));
          if (boolrg1)
            if ((recoptrrg5->u4.b.b0 & con(0x4)) != con(0x0))
              SKW_DEALLOC_RECORD(SKW_REMOVE_OBJECT_FROM_HAND());
          path = 1;
        }
        if (path == 2)
        {
          if (wordrg6 != con(0xffffffff) || (recoptrrg5->u4.b.b0 & con(0x1)) != con(0x0))
            continue;
          vtim_10.un.l_00 = (unsignedlong(vw_50) + ddata.longmallocx + con(0x2)) | (signedlong(ddata.v1d3248) << con(0x18));
          vtim_10.b_04 = con(0x57);
          vtim_10.b_05 = con(0x0);
          vtim_10.u.w_06 = vw_48;
          SKW_QUEUE_TIMER(&vtim_10);
          recoptrrg5->u4.b.b0 |= con(0x1);
          boolrg7 = false;
          if ((recoptrrg5->u4.b.b0 & con(0x20)) != con(0x0))
            vw_40 = con(0x10);
          if ((boolrg7) || (vw_44 != con(0x3)))
            path = 1;
          else
          {
            vbool_20 = true;
            if ((recoptrrg5->u4.b.b0 & con(0x40)) != con(0x0))
              SKW_QUEUE_NOISE_GEN2(con(0x9), con(0x88), con(0xfe), vb_58, ddata.v1e0270, ddata.v1e0272, con(0x1), con(0x8c), con(0x80));
            SKW_INVOKE_ACTUATOR(recoptrrg5, vw_40, con(0x0));
            SKW_INVOKE_ACTUATOR(recoptrrg5, vw_50 + con(0x1) + vw_40, con(0x1));
            continue;
          }
        }
        // only path==1 left here!
        if (vw_44 == con(0x3))
        {
          vw_44 = boolrg7 ? 1 : 0;
          boolrg7 = false;
        }
        if (!boolrg7)
        {
          boolrg7 = true;
          vbool_20 = true;
          if ((recoptrrg5->u4.b.b0 & con(0x40)) != con(0x0))
            SKW_QUEUE_NOISE_GEN2(con(0x9), con(0x88), con(0xfe), vb_58, ddata.v1e0270, ddata.v1e0272, con(0x1), con(0x8c), con(0x80));
          SKW_INVOKE_ACTUATOR(recoptrrg5, vw_40, vw_44);
        }
        continue;
      }
      if (wordrg6 != con(0xffffffff))
      {
        if ((SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(wordrg6, con(0x0)) & con(0x1)) == con(0x0))
        {
          if (SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg6) != con(0x194))
            break;
          SKW_SET_ITEMTYPE(wordrg6, con(0xf));
        }
        else
          SKW_ADD_ITEM_CHARGE(wordrg6, con(0xf));
        vbool_1c = true;
        vw_2c = wordrg6;
      }
      else
      {
        if (ddata.mouse_unk0e != con(0xffffffff))
        {
          x16 wordrg405 = con(0x0);
          wordrg6 = con(0x800);
          for(;;)
          {
            if (wordrg405 >= con(0x4))
              break;
            if (ddata.hero[wordrg405].w36 != con(0x0))
              ddata.hero[wordrg405].w46 = wordrg6; // BUGFIX 4/2014
            wordrg405++;
          }
          SKW_QUEUE_NOISE_GEN2(con(0x16), con(0x83), con(0xfe), ddata.hero[ddata.mouse_unk0e].b101, ddata.v1e0270, ddata.v1e0272, con(0x0), con(0x96), con(0x80));
        }
      }
    }
    else
    {
      if (wordrg6 != con(0xffffffff))
      {
        vbool_1c = true;
        wordrg6 &= con(0xffff3fff);
        wordrg6 |= vw_4c << con(0xe);
        SKW_MOVE_RECORD_TO(wordrg6, con(0x0), vw_0c, con(0xffffffff), vw_08);
      }
      else
      {
        x16 wordrg171 = SKW_GET_WALL_TILE_ANYITEM_RECORD(vw_0c, vw_08);
        wordrg5 = wordrg6;
        while (wordrg171 != con(0xfffffffe))
        {
          if (vw_4c == urshift(wordrg171, con(0xe)))
            wordrg5 = wordrg171;
          wordrg171 = SKW_GET_NEXT_RECORD_LINK(wordrg171);
        }
        if (wordrg5 != con(0xffffffff))
        {
          x16 wordrg170 = urshift(wordrg5 & con(0x3c00), con(0xa));
          if (wordrg170 >= con(0x5))
            if (wordrg170 <= con(0xa))
            {
              SKW_MOVE_RECORD_TO(wordrg5, vw_08, con(0xffffffff), vw_0c, con(0x0));
              vw_2c = wordrg5;
            }
        }
      }
    }
    break; // ugly, but that's because of all the continues before
    // probably can be arranged that the path-thingy can be placed here
  }
  if (vw_00 != con(0xffffffff))
  {
    recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(vw_00);
    if (!vbool_1c)
      if (argw0 != con(0xffffffff))
        SKW_APPEND_RECORD_TO(argw0, con(0xffffffff), vbool_1c ? 1 : 0, &recoptrrg5->u2.w);
    if (vw_2c != con(0xffffffff))
      SKW_APPEND_RECORD_TO(vw_2c, con(0xffffffff), con(0x0), &recoptrrg5->u2.w);
  }
  else
  {
    if (vbool_1c)
      SKW_REMOVE_OBJECT_FROM_HAND();
    if (vw_2c != con(0xffffffff))
      SKW_TAKE_OBJECT(vw_2c, true);
    ddata.v1e0488 = vbool_20;
  }
}

// TODO: serious original problem inside!
// was SKW_32cb_3672
void SKW_DRAW_ITEM(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, c_record* argprec1, bool argbool2, x16 argw3, bool argbool4)
{
  x16 wordrg5;
  x16 vw_00;
  x16 vw_04;
  x16 vw_0c; vw_0c = 0; // TODO: ORIGINAL BUG ! Problem, can reach it uninitialized
  x16 vw_10;
  x16 vw_14;
  x8 vb_18;
  x16 vw_1c;
  x16 vw_20;
  x16 vw_24;
  x8 vb_28;
  x8 vb_2c;
  x8 vb_30;
  x8 vb_34;

  vw_04 = eaxw;
  vw_00 = edxw;
  x16 wordrg6 = ebxw;
  wordrg5 = ecxw;
  if (argprec1 == NULL)
  {
    if (!argbool4)
      vw_0c = SKW_QUERY_OBJECT_5x5_POS(vw_04, ddata.v1e12c8);
    // there is no else here, and because of that vw_0c can stay uninitialized!
  }
  else
  {
    x16 wordrg22 = ((SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(argprec1->u4.b.b0)->u1e.w & con(0xf)) <= con(0x1)) ? 1 : 0;
    x16 wordrg41 = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(argprec1->u4.b.b0, con(0x41));
    if (wordrg41 == con(0x0))
      wordrg41 = signedword(table1d6d86[wordrg22]);
    wordrg5 -= wordrg41;
    x16 wordrg40 = CUTX16(((unsignedlong(vw_04) >> con(0xe)) - signedlong(ddata.v1e12c8)) & con(0x3));
    if (vw_00 != con(0x3) || ddata.v1e12ce == con(0x0))
      vw_0c = con(0xc);
    else
      vw_0c = signedword(table1d6b76[ddata.v1d324a + 0x42]);
    if (!SKW_32cb_35c1(&vw_00, table1d6d76[wordrg22][wordrg40], signedword(table1d6d7e[wordrg22][wordrg40]), &vw_0c))
      return;
  }
  vw_14 = signedword(table1d6b15[vw_00]);
  if (!argbool4)
  {
    vw_1c = con(0x4) - (vw_0c / con(0x5)); // here vw_0c is initialized because of !argbool4
    if (vw_00 == con(0x0))
      if (vw_1c < con(0x2))
        return;
    vw_10 = SKW_QUERY_CREATURE_BLIT_RECTI(vw_00, t_rotate(0), vw_0c) | con(0x8000);
  }
  else
  {
    vw_1c = con(0x0);
    x16 wordrg177 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(ddata.v1e1054.s_00.b_08, con(0xc), con(0xfd), ddata.v1e1054.s_00.b_09);
    vb_18 = CUTX8(wordrg177);
    x16 wordrg23 = ddata.v1e1054.w_1c + signedword(CUTX8(urshift(wordrg177, con(0x8))));
    wordrg6 = table1d6b76[table1d6b76[2 * argw0 + 0x22] + 0x50] + wordrg23;
    wordrg5 = signedword(vb_18) + ddata.v1e1054.w_1e + signedword(table1d6b76[table1d6b76[2 * argw0 + 0x23] + 0x50]);
    vw_10 = ddata.v1e1054.w_18;
  }
  bool vbool_08 = false;
  x8 byterg2 = SKW_QUERY_CLS1_FROM_RECORD(vw_04);
  vb_34 = byterg2;
  vb_28 = SKW_QUERY_CLS2_FROM_RECORD(vw_04);
  if (byterg2 == con(0x14) && vb_28 < con(0x8))
  {
    if (argw3 == con(0x0))
      argw3 += SKW_GET_ADDRESS_OF_RECORD(vw_04)->u4.w & con(0x1);
    vbool_08 = true;
  }
  vb_30 = table1d6d6e[argw3].b0;
  vw_20 = con(0x0);
  if (table1d6afe[vw_00] != con(0x0))
  {
    if (vbool_08 && table1d6afe[vw_00] == con(0x1))
      vw_20 = con(0x1);
  }
  else
  {
    vw_24 = vw_0c % con(0x5); // ...but here is the danger!
    if (vw_24 != con(0x2) && !argbool4 || (vb_2c = table1d6d6e[argw3].b1, !SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(vb_34, con(0x1), vb_2c, vb_28)))
    {
      if (vbool_08 && !argbool4 && vw_24 > con(0x2))
        vw_20 = con(0x1);
    }
    else
      vb_30 = vb_2c;
  }
  if (argw0 != con(0xffffffff))
  {
    x16 wordrg41 = 2 * argw0;
    wordrg6 += signedword(table1d6b76[table1d6b76[wordrg41 + 0x22] + 0x50]);
    if (argprec1 == NULL)
      wordrg5 += signedword(table1d6b76[table1d6b76[wordrg41 + 0x23] + 0x50]);
  }
  x16 wordrg18 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(vb_34, con(0xc), vb_30, con(0xfe));
  vb_18 = CUTX8(wordrg18);
  if (wordrg18 != con(0x0))
  {
    wordrg6 += signedword(CUTX8(urshift(wordrg18, con(0x8))));
    wordrg5 += signedword(vb_18);
  }
  x16 wordrg40 = unsignedword(table1d6b76[vw_1c + 4 * vw_14 + 0x1]);
  SKW_QUERY_TEMP_PICST(vw_20, wordrg40, wordrg6, wordrg40, wordrg5, vw_14, vw_10, con(0x0), con(0xa), con(0xffff), vb_34, vb_28, vb_30);
  if (!argbool2)
    ddata.v1e118e.w_30 = con(0xfffffffe);
  SKW_DRAW_TEMP_PICST();
}

// was SKW_32cb_3991
void SKW_DRAW_PUT_DOWN_ITEM(x16 eaxw, x16 ebxw, c_record* ecxprec, x16 edxw)
{
  x16 wordrg5;
  c_aispec* aiptrrg1;
  c_record* prec_00;
  s_hex12* h12ptrrg4;
  x16 vw_08;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_14;
  x16 vw_18;
  x16 vw_1c;

  x16 wordrg6 = eaxw;
  vw_18 = edxw;
  vw_1c = ebxw;
  prec_00 = ecxprec;
  if (eaxw == con(0xfffffffe) || (table1d6b15[vw_18] > con(0x3)))
    return;
  wordrg5 = con(0x0);
  if (ecxprec != NULL)
  {
    aiptrrg1 = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(ecxprec->u4.b.b0);
    vw_10 = aiptrrg1->u1e.w;
    vw_14 = aiptrrg1->w20;
  }
  for(;;)
  {
    x16 wordrg107 = urshift(wordrg6 & con(0x3c00), con(0xa));
    if (wordrg107 >= con(0x5) && wordrg107 <= con(0xa))
    {
      x16 vw_04 = vw_1c;
      if ((((urshift(wordrg6, con(0xe))) - signedlong(ddata.v1e12c8)) & con(0x3)) == signedlong(vw_1c))
      {
        if (prec_00 == NULL)
        {
          SKW_DRAW_ITEM(wordrg6, con(0x0), con(0x0), vw_18, wordrg5, NULL, true, con(0x0), false);
          wordrg5++;
          wordrg5 &= con(0xf);
          if ((vw_18 == con(0x0)) || ((vw_18 == con(0x3)) && (vw_04 > con(0x1))))
          {
            h12ptrrg4 = &ddata.v1e1044[vw_18];
            vw_08 = signedword(h12ptrrg4->b_00);
            vw_0c = signedword(h12ptrrg4->b_01);
            x16 wordrg40 = SKW_1c9a_03cf(&vw_08, urshift(wordrg6, con(0xe)), &vw_0c);
            if (wordrg40 == con(0xffffffff) || SKW_IS_CREATURE_FLOATING(wordrg40) || (SKW_0cee_2df4(wordrg40) & con(0x2000)) != con(0x0))
              SKW_MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE(vw_1c, CUTX8(vw_18), con(0x1), wordrg6);
          }
        }
        else
        {
          if (((vw_10 & con(0x400)) == con(0x0)))
          {
            if ((urshift(vw_10, con(0x4)) & con(0xf)) == con(0x0))
            {
              SKW_QUERY_EXPANDED_RECT(vw_14, &ddata.v1e02f0[ddata.v1e0404].r0);
              ddata.v1e02f0[ddata.v1e0404].bb = con(0x2);
              ddata.v1e02f0[ddata.v1e0404].w8 = wordrg6;
              ddata.v1e02f0[ddata.v1e0404].ba = CUTX8(vw_18); // BUGFIX 4/2014
              ddata.v1e0404++;
              return;
            }
          }
          SKW_DRAW_ITEM(wordrg6, con(0x0), con(0x0), vw_18, wordrg5, prec_00, true, con(0x0), false);
          wordrg5++;
          wordrg5 &= con(0xf);
          if (((con(0x1) << CUTX8(vw_1c)) & SKW_48ae_01af(vw_10, con(0x0))) != con(0x0))
            SKW_MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE(vw_1c + con(0x4), CUTX8(vw_18), con(0x2), wordrg6);
        }
      }
    }
    wordrg6 = SKW_GET_NEXT_RECORD_LINK(wordrg6);
    if (wordrg6 == con(0xfffffffe))
      return;
  }
}

// was SKW_32cb_3b9d
void SKW_DRAW_STATIC_OBJECT(x16 eaxw, bool ebxbool, x32 edxl)
{
  x16 wordrg5;
  ui16 uwordrg5;
  const x8* bptrrg7;
  s_hex12* h12ptrrg4;
  x8* bptrrg41;
  bool flag;
  x16 vw_08;
  x16 vw_18;
  x16 vw_1c;
  x16 vw_28;

  vw_08 = eaxw;
  x16 wordrg106 = vw_08;
  if (wordrg106 > con(0xf) || table1d6dd3[wordrg106] == con(0x0))
    return;
  h12ptrrg4 = &ddata.v1e1044[wordrg106];
  vw_1c = h12ptrrg4->s_02.warr_00[con(0x0)];
  vw_18 = h12ptrrg4->s_02.warr_00[con(0x2)];
  x16 vw_24 = signedword(table1d6de3[wordrg106][0]);
  x16 vw_20 = signedword(table1d6de3[wordrg106][1]);
  if (table1d6afe[wordrg106] >= con(0x0))
  {
    if (table1d6afe[wordrg106] != con(0x0))
      bptrrg7 = table1d6da1;
    else
      bptrrg7 = table1d6dba;
  }
  else
    bptrrg7 = table1d6d88;
  ui16 vuw_14 = con(0x0);
  for(;;)
  {
    x16 wordrg105;
    if (vw_08 != con(0x0))
      wordrg105 = con(0x18);
    else
      wordrg105 = con(0xe);
    if (vuw_14 > wordrg105)
    { // BUGFIX 4.2/2014
      if (ebxbool)
        SKW_32cb_2d8c(vw_18, edxl, vw_08);
      return;
    }
    vw_28 = unsignedword(bptrrg7[vuw_14]);
    x32 longrg1 = con(0x1) << bptrrg7[vuw_14];
    flag = (edxl & longrg1) == con(0x0);
    x32 vl_10 = edxl & longrg1;
    if (!flag)
    {
      x32 longrg1 = ddata.v1e104c[vw_08];
      if ((longrg1 & vl_10) != con(0x0))
        SKW_DRAW_PUT_DOWN_ITEM(vw_18, SKW_DIR_FROM_5x5_POS(vw_28), NULL, vw_08);
      x16 wordrg3 = signedword(table1d6e03[vw_28][0]) + vw_24;
      wordrg5 = vw_20 - signedword(table1d6e03[vw_28][1]);
      if (vw_1c == con(0x0) || (unsignedword(ddata.v1e1048[wordrg3].barr_00[wordrg5]) == vw_08))
      {
        x8 byterg1 = ddata.v1e1050[wordrg3].barr_00[wordrg5];
        if (byterg1 != con(0xffffffff) && (edxl & vl_10) != con(0x0))
        {
          x32 longrg2 = edxl;
          if (vw_28 != con(0xc))
            longrg2 &= con(0xbfffffff);
          if (SKW_SUMMARY_DRAW_CREATURE(ddata.v1e1044[byterg1].s_02.warr_00[con(0x2)], longrg2, signedword(byterg1)))
          {
            bptrrg41 = ddata.v1e1050[wordrg3].barr_00;
            bptrrg41[wordrg5] = con(0xffffffff);
            if (ddata.v1e1038[vw_08] != con(0x0))
            {
              uwordrg5 = vuw_14 - con(0x7);
              while (uwordrg5 < vuw_14)
              {
                if (((con(0x1) << bptrrg7[uwordrg5]) & ddata.v1e1038[vw_08]) != con(0x0))
                  SKW_DRAW_FLYING_ITEM(vw_18, unsignedword(bptrrg7[uwordrg5]), vw_08);
                uwordrg5++;
              }
            }
          }
        }
      }
      if ((ddata.v1e1038[vw_08] & vl_10) != con(0x0))
        SKW_DRAW_FLYING_ITEM(vw_18, vw_28, vw_08);
    }
    vuw_14++;
  }
}

// was SKW_32cb_4cdf
void SKW_DRAW_DOOR_TILE(ui16 eaxuw)
{
  if (table1d6f4c[eaxuw] == con(0x0))
    return;
  x32 longrg3 = con(0x0);
  if (ddata.v1e1044[eaxuw].s_02.warr_00[con(0x3)] != con(0x5))
    longrg3 = con(0x40001000);
  SKW_DRAW_STATIC_OBJECT(eaxuw, eaxuw != con(0x0), con(0x3ff));
  x16 wordrg40 = con(0x0);
  x16 wordrg1 = con(0x0);
  bool skip = false; // added
  if (eaxuw < UI16CAST(con(0x7)))
  {
    if (eaxuw < UI16CAST(con(0x4)))
    {
      if (eaxuw == con(0x0))
      {
        if (longrg3 != con(0x0))
          SKW_DRAW_STATIC_OBJECT(eaxuw, false, con(0x1000));
        SKW_DRAW_DOOR(eaxuw, con(0x0), con(0x0), con(0x6));
        return;
      }
      if (eaxuw == con(0x3))
        wordrg1 = con(0x7);
      else
        skip = true;
    }
    else if (eaxuw <= UI16CAST(con(0x4)))
    {
      wordrg1 = con(0x1);
      wordrg40 = con(0x4);
    }
    else if (eaxuw <= UI16CAST(con(0x5)))
    {
      wordrg1 = con(0x1);
      wordrg40 = con(0x2);
    }
    else
      wordrg1 = con(0x7);
  }
  else if (eaxuw <= UI16CAST(con(0x7)))
  {
    wordrg1 = con(0x1);
    wordrg40 = con(0x4);
  }
  else if (eaxuw < UI16CAST(con(0xc)))
  {
    if (eaxuw <= UI16CAST(con(0x8)))
    {
      wordrg1 = con(0x1);
      wordrg40 = con(0x2);
    }
    else if (eaxuw == con(0xb))
      wordrg1 = con(0x6);
    else
      skip = true;
  }
  else if (eaxuw <= UI16CAST(con(0xc)))
  {
    wordrg1 = con(0x2);
    wordrg40 = con(0x4);
  }
  else if (eaxuw <= UI16CAST(con(0xd)))
  {
    wordrg1 = con(0x4);
    wordrg40 = con(0x2);
  }
  else if (eaxuw > UI16CAST(con(0xf)))
    skip = true;
  if (!skip)
    SKW_DRAW_DOOR(eaxuw, wordrg40, longrg3, wordrg1);
  SKW_DRAW_STATIC_OBJECT(eaxuw, true, con(0x1fffc00));
  SKW_32cb_3edd(eaxuw);
}

// was SKW_32cb_5146
void SKW_DRAW_DUNGEON_TILES(void)
{
  ui16 uwordrg5;

  x32 longrg7 = con(0x1ffffff);
  // M_441B0: co-label is M_441AC
  for (ui16 vuw_00 = con(0x0); vuw_00 < con(0x14); vuw_00++)
  {
    bool doit1 = false;
    bool doit2 = false;
    SKW_IBMIO_USER_INPUT_CHECK();
    ui16 uwordrg6 = unsignedword(table1d7029[vuw_00]);
    x16 wordrg3 = table1d7029[vuw_00];
    uwordrg5 = ddata.v1e1044[wordrg3].s_02.warr_00[con(0x0)];
    x16 wordrg400 = ddata.v1e1044[wordrg3].s_02.warr_00[con(0x2)];
    if (wordrg3 <= con(0xf))
    {
      if (uwordrg5 < UI16CAST(con(0x5)))
      {
        if (uwordrg5 < UI16CAST(con(0x1)))
        {
          if (uwordrg5 != con(0x0))
            continue;
          if (wordrg400 == con(0xfffffffe))
          {
            SKW_DRAW_WALL_TILE(uwordrg6);
            continue;
          }
          SKW_SUMMARY_DRAW_CREATURE(wordrg400, con(0x1ffffff), uwordrg6);
          SKW_DRAW_WALL_TILE(uwordrg6);
          continue;
        }
        if (uwordrg5 <= UI16CAST(con(0x1)))
        {
          ;
        }
        else if (uwordrg5 == con(0x2))
          SKW_DRAW_PIT_TILE(uwordrg6);
        else
          continue;
        doit1 = true;
      }
      else if (uwordrg5 <= UI16CAST(con(0x5)))
        doit1 = true;
      else if (uwordrg5 < UI16CAST(con(0x11)))
      {
        if (uwordrg5 == con(0x10))
        {
          if (uwordrg6 == con(0x3))
            if (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xe), con(0xb), con(0x40), ddata.v1e040e[SKW_GET_ADDRESS_OF_RECORD(ddata.v1e1044[con(0x3)].s_02.warr_00[con(0x4)])->u2.w & con(0x1)]) == con(0x0))
            {
              SKW_QUERY_TEMP_PICST(ddata.v1e12d0, con(0x2b), con(0x0), con(0x2b), con(0x0), con(0x2), SKW_QUERY_CREATURE_BLIT_RECTI(con(0x3), t_rotate(con(0x0)), con(0x2)), con(0xffffffff), ddata.v1d6c00, con(0xffff), con(0x8), ddata.v1d6c02, con(0x6));
              SKW_DRAW_TEMP_PICST();
            }
        }
        else
          continue;
        doit1 = true;
      }
      else if (uwordrg5 <= UI16CAST(con(0x11)))
      {
        SKW_DRAW_DOOR_TILE(uwordrg6);
        continue;
      }
      else if (uwordrg5 <= UI16CAST(con(0x12)))
      {
        SKW_DRAW_STAIRS_SIDE(uwordrg6);
        doit2 = true;
      }
      else if (uwordrg5 == con(0x13))
      {
        SKW_DRAW_STAIRS_FRONT(uwordrg6);
        doit2 = true;
      }
      else
        continue;
      if (doit1)
      {
        if (!SKW_DRAW_EXTERNAL_TILE(uwordrg6))
          longrg7 = con(0x1ffffff);
        else
          longrg7 = con(0x1fffc00);
        SKW_DRAW_PIT_ROOF(uwordrg6);
        doit2 = true;
      }
      if (doit2)
      {
        SKW_DRAW_STATIC_OBJECT(uwordrg6, true, longrg7);
        if (uwordrg5 == con(0x5))
          SKW_DRAW_TELEPORTER_TILE(uwordrg6, con(0x0), con(0x18));
        continue;
      }
    }
    if (uwordrg5 < UI16CAST(con(0x1)))
    {
      if (uwordrg5 != con(0x0))
        continue;
      if (wordrg400 == con(0xfffffffe))
      {
        SKW_DRAW_WALL_TILE(uwordrg6);
        continue;
      }
      SKW_SUMMARY_DRAW_CREATURE(wordrg400, con(0x1ffffff), uwordrg6);
      SKW_DRAW_WALL_TILE(uwordrg6);
      continue;
    }
    if (uwordrg5 <= UI16CAST(con(0x2)))
      SKW_DRAW_EXTERNAL_TILE(uwordrg6);
    else
    {
      if (uwordrg5 < UI16CAST(con(0x5)))
        continue;
      if (uwordrg5 <= UI16CAST(con(0x5)))
        SKW_DRAW_EXTERNAL_TILE(uwordrg6);
      else if (uwordrg5 != con(0x11))
        continue;
    }
    if (wordrg400 != con(0xfffffffe))
      SKW_SUMMARY_DRAW_CREATURE(wordrg400, con(0x1ffffff), uwordrg6);
  }
}

x16 SKW_1c9a_1b16(x16 eaxw, x16 edxw)
{
  c_record* recoptrrg1;

  x16 wordrg4 = ddata.v1e08b0;
  if (wordrg4 == con(0xffffffff))
    ddata.v1e08b0 = wordrg4 = SKW_GET_TILE_RECORD_LINK(ddata.v1e08a8, ddata.v1e08aa);
  for(;;)
  {
    if (wordrg4 == con(0xfffffffe))
      return con(0xffffffff);
    x16 wordrg1 = urshift(wordrg4 & con(0x3c00), con(0xa));
    if (wordrg1 > con(0x3))
      return con(0xffffffff);
    if (wordrg1 == con(0x2) && (recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg4), (recoptrrg1->u2.w & con(0x6)) == con(0x4)))
    {
      x16 wordrg2 = urshift(recoptrrg1->u2.w, con(0x3));
      if ((((wordrg2 >> con(0x9)) & con(0xf)) == eaxw) && (((wordrg2 >> con(0x5)) & con(0xf)) == signedlong(edxw)))
        return wordrg2 & con(0x1f);
    }
    wordrg4 = SKW_GET_NEXT_RECORD_LINK(wordrg4);
  }
}

x16 SKW_1c9a_1a48(x16 eaxw, x16 edxw)
{
  c_record* recoptrrg1;
  x16 vw_00;
  x16 vw_04;

  vw_04 = edxw;
  if (eaxw != con(0x1))
  {
    vw_00 = con(0xffffffff);
  }
  else
  {
    vw_00 = con(0x2);
  }
  x16 wordrg4 = ddata.v1e08b0;
  if (wordrg4 == con(0xffffffff))
    wordrg4 = ddata.v1e08b0 = SKW_GET_TILE_RECORD_LINK(ddata.v1e08a8, ddata.v1e08aa);
  for(;;)
  {
    if (wordrg4 == con(0xfffffffe))
      return con(0xffffffff);
    x16 wordrg1 = urshift(wordrg4 & con(0x3c00), con(0xa));
    if (wordrg1 > con(0x3))
      return con(0xffffffff);
    if (wordrg1 == con(0x2) && (recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg4), (recoptrrg1->u2.w & con(0x6)) == con(0x4)))
    {
      x16 wordrg11 = urshift(recoptrrg1->u2.w, con(0x3));
      x16 wordrg3 = (wordrg11 >> con(0x9)) & con(0xf);
      if (wordrg3 == eaxw || (wordrg3 == vw_00 && ((recoptrrg1->u2.w & con(0x1)) != con(0x0))))
      {
        if ((vw_04 & (con(0x1) << CUTX8((wordrg11 >> con(0x5)) & con(0xf)))) != con(0x0))
        {
          wordrg11 &= con(0x1f);
          return wordrg11;
        }
      }
    }
    wordrg4 = SKW_GET_NEXT_RECORD_LINK(wordrg4);
  }
}

void SKW_ALLOC_CAII_TO_CREATURE(x16 eaxw, x16 ebxw, x16 edxw)
{
  c_record* recoptrrg6;
  s_d556* sd5ptrrg5;
  c_350* ptrrg2;
  x16 vw_04;
  x16 vw_08;

  vw_08 = edxw;
  vw_04 = ebxw;
  recoptrrg6 = SKW_GET_ADDRESS_OF_RECORD(eaxw);
  if (recoptrrg6->u4.b.b1 != con(0xffffffff))
    return;
  x16 vw_00 = ulrshift(recoptrrg6->ue.w, con(0x5), con(0xf));
  recoptrrg6->ue.b.b1 |= con(0x4);
  // M_20799:
  for(;;)
  {
    sd5ptrrg5 = ddata.v1e089c;
    ui16 uwordrg26 = con(0x0);
    ui16 uwordrg42 = ddata.v1e08a0;
    // M_207B4:
    while (uwordrg26 < uwordrg42)
    {
      if (sd5ptrrg5->w00 < con(0x0))
      {
        recoptrrg6->ue.b.b1 &= con(0xfffffffb);
        recoptrrg6->ue.w |= (vw_00 & con(0x1)) << con(0xa);
        ddata.v1d4020++;
        DM2_ZERO_MEMORY(DOWNCAST(s_d556, sd5ptrrg5), sizeof(s_d556));
        sd5ptrrg5->w00 = eaxw & con(0x3ff);
        recoptrrg6->u4.b.b1 = CUTX8(uwordrg26);
        sd5ptrrg5->w02 = con(0xffffffff);
        sd5ptrrg5->u.v.b1a = con(0xffffffff);
        ptrrg2 = SKW_13e4_0004(eaxw, CUTX8(vw_04), con(0x22), CUTX8(vw_08));
        SKW_14cd_0802();
        sd5ptrrg5->barr04[con(0x2)] = CUTLX8((ddata.longmallocx >> con(0x2)) - con(0x1));
        sd5ptrrg5->barr04[con(0x0)] = CUTLX8(ddata.longmallocx) - con(0x7f);
        sd5ptrrg5->u0c.b.b0 &= con(0xffffffe0);
        x16 wordrg405 = sd5ptrrg5->u0c.w | (unsignedword(ddata.s350.v1e0562.u.b.b_06) & con(0x1f));
        sd5ptrrg5->u0c.w = wordrg405;
        sd5ptrrg5->u0c.w = (wordrg405 & con(0xfffffc1f)) | ((unsignedword(ddata.s350.v1e0562.u.b.b_07) & con(0x1f)) << con(0x5));
        sd5ptrrg5->u0c.b.b1 &= con(0x3);
        sd5ptrrg5->u0c.w |= (ddata.v1d3248 & con(0x3f)) << con(0xa);
        ddata.s350.v1e07d8.b1 = con(0x0);
        sd5ptrrg5->u.v.barr16[con(0x0)] = con(0xffffffff);
        sd5ptrrg5->u.v.barr16[con(0x1)] = con(0xffffffff);
        sd5ptrrg5->barr04[con(0x3)] = con(0x0);
        SKW_1c9a_0cf7(vw_08, vw_04);
        if (recoptrrg6->w8 != con(0xffffffff))
          sd5ptrrg5->u.v.b1a = con(0x0);
        else
          sd5ptrrg5->u.v.b1a = con(0x11);
        if ((ddata.s350.v1e0552->u0.b.b0 & con(0x1)) == con(0x0))
        {
          ddata.s350.v1e054e->ua.b.b1 |= con(0xffffff80);
          ddata.s350.v1e054e->ua.b.b1 &= con(0xffffffbf);
          SKW_CREATURE_SOMETHING_1c9a_0a48();
        }
        SKW_13e4_0329(ptrrg2);
        return;
      }
      sd5ptrrg5++;
      uwordrg26++;
    }
    ui16 uwordrg25 = ddata.v1d4020;
    for(;;)
    {
      if (uwordrg25 > ddata.v1d4020)
        break;
      if (SKW_RECYCLE_A_RECORD_FROM_THE_WORLD(con(0x4), con(0xff)) == con(0xffffffff))
        SKW_RAISE_SYSERR(con(0x47));
    }
  }
}

bool SKW_1c9a_38a8(void)
{
  x16 wordrg5;
  x16* wptrrg5;
  bool retflag;
  u_bbw vs_00[0x8];

  retflag = false;
  SKW___SET_CURRENT_THINKING_CREATURE_WALK_PATH();
  x16 wordrg6 = con(0x0);
  x16 wordrg21 = ddata.s350.v1e07d8.w4;
  for(;;)
  {
    if (wordrg6 >= ddata.s350.v1e0674)
      return retflag;
    if (ddata.s350.v1e0678[wordrg6].s_00.b_07 == ddata.s350.v1e07d8.b3 && wordrg21 == ddata.s350.v1e0678[wordrg6].u_08.w)
      break;
    wordrg6++;
  }
  if (wordrg6 != con(0x0))
    SKW_COPY_MEMORY(DOWNCAST(c_sizex16, &ddata.s350.v1e0678[wordrg6]), con(0x16), DOWNCAST(c_sizex16*, ddata.s350.v1e0678));
  ddata.s350.v1e0678[0].s_00.b_00 = con(0x0);
  vs_00[0x4].w = ddata.v1d3248;
  SKW_1c9a_0648(urshift(ddata.s350.v1e07d8.w8, con(0xa)));
  vs_00[0x2].w = ulrshift(ddata.s350.v1e07d8.w8, con(0x6), con(0xb));
  wordrg5 = ddata.s350.v1e07d8.w8 & con(0x1f);
  if (SKW_1c9a_1bd8(wordrg5, con(0x2), vs_00[0x2].w, con(0x1), ddata.s350.v1e0678, vs_00) == con(0x0) && (ddata.s350.v1e0678[0].s_00.b_06 == con(0x0)))
  {
    x16 wordrg3 = unsignedword(ddata.s350.v1e0562.u.b.b_06);
    x16 wordrg4 = unsignedword(ddata.s350.v1e0562.u.b.b_07);
    x16 wordrg61 = unsignedword(ddata.s350.v1e07d8.b1);
    if (wordrg61 != con(0x0))
    {
      SKW_1c9a_0648(vs_00[0x4].w);
      vs_00[0x6].w = con(0x5);
      SKW___SET_CURRENT_THINKING_CREATURE_WALK_PATH();
      for(;;)
      {
        wptrrg5 = &ddata.s350.v1e07e6[(unsignedword(ddata.s350.v1e07d8.b0) - wordrg61)];
        x16 wordrg2 = wordrg4;
        wordrg4 = wordrg3;
        if (!SKW_CREATURE_GO_THERE(ddata.s350.v1e0673, wordrg2, con(0xffffffff), wordrg4, con(0xffffffff), *wptrrg5 & con(0x7)))
          break;
        if (ulrshift(*wptrrg5, con(0x8), con(0xb)) != ddata.v1e08a8)
          break;
        if (ulrshift(*wptrrg5, con(0x3), con(0xb)) != ddata.v1e08aa)
          break;
        wordrg4 = (ddata.v1e08ac != ddata.v1d3248) ? 1 : 0;
        if (ulrshift(*wptrrg5, con(0x2), con(0xf)) != wordrg4)
          break;
        SKW_1c9a_0648(ddata.v1e08ac);
        wordrg3 = ddata.v1e08a8;
        wordrg4 = ddata.v1e08aa;
        wordrg61--;
        if (wordrg61 <= con(0x0))
        {
          retflag = true;
          break;
        }
        if (--vs_00[0x6].w <= con(0x0))
        {
          retflag = true;
          break;
        }
      }
    }
    else
    {
      if ((wordrg3 != wordrg5) || (wordrg4 != vs_00[0x2].w))
        retflag = false;
      else
      {
        if (ddata.v1d3248 == vs_00[0x4].w)
          retflag = true;
        else
          retflag = false;
      }
    }
  }
  SKW_1c9a_0648(vs_00[0x4].w);
  return retflag;
}

// was SKW_32cb_0008
bool SKW_LOAD_GDAT_INTERFACE_00_0A(void)
{
  x16 wordrg5;
  c_record* recoptrrg6;
  x16 vw_00;
  x16 vw_04;
  x16 vw_08;

  x16 wordrg1 = SKW_GET_TILE_RECORD_LINK(ddata.v1e0270, ddata.v1e0272);
  // M_3C0A3: co-label M_3C1CC
  for (;; wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg5))
  {
    wordrg5 = wordrg1;
    if (wordrg5 == con(0xfffffffe))
      return false;
    x16 wordrg14 = urshift(wordrg5 & con(0x3c00), con(0xa));
    if (wordrg14 > con(0x3))
      return false;
    if (wordrg14 != con(0x2) || (recoptrrg6 = SKW_GET_ADDRESS_OF_RECORD(wordrg5), (recoptrrg6->u2.w & con(0x6)) != con(0x2)))
      continue;
    x16 wordrg13 = urshift(urshift(recoptrrg6->u2.w, con(0x3)), con(0x8)) & con(0x1f);
    if (wordrg13 < con(0xf))
      continue;
    bool doit = false; // added
    if (wordrg13 <= con(0xf))
    {
      vw_08 = con(0x4);
      doit = true;
    }
    if (wordrg13 == con(0x10))
    {
      vw_08 = con(0x5);
      doit = true;
    }
    if (doit)
    {
      x16 wordrg101 = R_3D93B(vw_08, unsignedword(CUTX8(urshift(recoptrrg6->u2.w, con(0x3)))), &vw_04, con(0x0), &vw_00);
      if (wordrg101 >= con(0x0))
      {
        R_3BF83(vw_04, wordrg101, ddata.v1e0258, vw_00);
        SKW_QUEUE_NOISE_GEN2(con(0xa), con(0x88), con(0xfe), CUTX8(urshift(recoptrrg6->u2.w, con(0x3))), ddata.v1e0270, ddata.v1e0272, con(0x1), con(0x8c), con(0x80));
        if (vw_08 == con(0x4))
          recoptrrg6->u2.b.b0 |= con(0x1);
        return true;
      }
    }
  }
}

bool SKW_WIELD_WEAPON(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1)
{
  c_hero* heroptrrg5;
  x16 vw_00;
  x16 vw_04;
  x16 vw_08;

  vw_08 = eaxw; // BUGFIX 4/2014
  vw_04 = edxw;
  vw_00 = ecxw;
  heroptrrg5 = &ddata.hero[eaxw];
  if ((ddata.v1e0b4c == con(0xffffffff)) || SKW_GET_CREATURE_AT(ebxw, ecxw) == con(0xffffffff))
    return false;
  x16 wordrg6 = unsignedword(heroptrrg5->b1d);
  ui16 uwordrg1 = (unsignedword(heroptrrg5->b1d) + con(0x4) - unsignedword(heroptrrg5->b1c)) & con(0x3);
  bool doit = false;
  if (uwordrg1 >= con(0x2))
  {
    x16 wordrg4;
    if (uwordrg1 <= con(0x2))
    {
      wordrg4 = con(0x3);
      doit = true;
    }
    else if (uwordrg1 == con(0x3))
    {
      wordrg4 = con(0x1);
      doit = true;
    }
    if (doit)
    {
      wordrg4 += wordrg6;
      wordrg4 &= con(0x3);
      if (SKW_GET_PLAYER_AT_POSITION(wordrg4) != con(0xffffffff))
      {
        ddata.v1e0b7c = con(0xffffffff);
        ddata.v1e0b6c = DM2_MAX(con(0x1), con(0x8) - SKW_UPDATE_GLOB_VAR(con(0x42), con(0x3), true));
        return false;
      }
    }
  }
  if (vw_04 == con(0x1))
    if ((SKW_QUERY_CREATURE_AI_SPEC_FLAGS(ddata.v1e0b4c) & con(0x20)) == con(0x0))
      return false;
  x16 wordrg3 = SKW_QUERY_CUR_CMDSTR_ENTRY(con(0xa));
  x16 wordrg402 = wordrg3;
  x16 wordrg61 = SKW_QUERY_CUR_CMDSTR_ENTRY(con(0xb));
  if (SKW_QUERY_CUR_CMDSTR_ENTRY(con(0xf)) != con(0x0))
  {
    wordrg3 |= con(0x8000);
    wordrg402 = wordrg3;
  }
  ddata.v1e0b7c = SKW_CALC_PLAYER_ATTACK_DAMAGE(heroptrrg5, ddata.v1e0b4c, ebxw, vw_08, vw_00, wordrg402, wordrg61, argw0, argw1);
  return true;
}

void SKW_2759_12e6(void)
{
  s_size8* sptrrg3;
  c_hero* heroptrrg1;
  x16 vw_08;

  vw_08 = con(0xffffffff);
  // M_310B0:
  for (x16 vw_04 = con(0x0); vw_04 < ddata.savegamew2; vw_04++)
    // M_3109B: co-label M_31097
    for (x16 vw_00 = con(0x0); vw_00 <= con(0x1); vw_00++)
    {
      x16 wordrg2 = vw_00;
      x16 wordrg61 = vw_04;
      sptrrg3 = &ddata.v1e0adc[wordrg61][wordrg2];
      if (sptrrg3->w2 == con(0x0))
        continue;
      heroptrrg1 = &ddata.hero[wordrg61];
      if (heroptrrg1->w36 != con(0x0))
      {
        if (sptrrg3->w6 == heroptrrg1->warrc3[wordrg2])
        {
          if (sptrrg3->b4 == con(0x0))
          {
            sptrrg3->w2 -= con(0x1);
            vw_08 = ddata.v1e0b76;
            ddata.v1e0b76 = vw_00;
            SKW_IS_ITEM_HAND_ACTIVABLE(vw_04, wordrg2, sptrrg3->w6);
            SKW_ENGAGE_COMMAND(vw_04, unsignedword(sptrrg3->b5) | con(0x8000));
            if (SKW_RAND16(sptrrg3->w0 + con(0x1)) == con(0x0))
              if (R_30DEA(wordrg61))
              {
                sptrrg3->w0 = con(0x0);
                sptrrg3->w2 = sptrrg3->w0;
                sptrrg3->b4 = CUTX8(sptrrg3->w0);
              }
            ddata.v1e0b76 = vw_08;
          }
          else
            sptrrg3->b4 = con(0x0);
          continue;
        }
      }
      sptrrg3->w2 = con(0x0);
    }
  if (ddata.v1e0b74 != con(0x0) && vw_08 >= con(0x0))
    if (vw_08 <= con(0x1))
    {
      x16 wordrg21 = ddata.v1e0b76;
      x16 wordrg6 = ddata.v1e0b74 - con(0x1);
      SKW_IS_ITEM_HAND_ACTIVABLE(wordrg6, wordrg21, ddata.hero[wordrg6].warrc3[wordrg21]);
    }
}

// TODO: ORIGINAL BUG - problem inside
bool SKW_19f0_2813(x8 eaxb, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2)
{
  c_record* recoptrrg5;
  x16 vw_00;
  x16 vw_04;
  x8 vb_08;

  // SPX: variables moved here
	x16 wordrg406;
	x16 wordrg62;
	x16 wordrg61;
	x16 wordrg101;
	bool skip = false;

  vb_08 = eaxb;
  vw_04 = ecxw;
  vw_00 = unsignedword(eaxb & con(0xffffff80));
  if (vw_00 != con(0x0))
    vb_08 &= con(0x7f);
  if ((ddata.s350.v1e057e & con(0x1)) == con(0x0))
    goto M_exit;
  if (vw_04 != con(0xffffffff))
  {
    if (edxw == vw_04 && ebxw == argw0)
      goto M_exit;
  }
  else
  {
    vw_04 = table1d27fc[argw1] + edxw;
    argw0 = table1d2804[argw1] + ebxw;
  }
  if (vw_04 < con(0x0) || vw_04 >= ddata.savegamewa)
    goto M_exit;
  if (argw0 < con(0x0) || argw0 >= ddata.savegamewb)
    goto M_exit;
  if (edxw != vw_04 && ebxw != argw0)
    goto M_exit;
  if (argw1 == con(0xffffffff))
    argw1 = SKW_CALC_VECTOR_DIR(edxw, vw_04, argw0, ebxw);
  SKW_19f0_045a(vw_04, argw0);
  if ((ddata.v1e08ae & con(0x10)) == con(0x0))
    goto M_exit;
  if (ddata.v1e08b0 == con(0xffffffff))
    ddata.v1e08b0 = SKW_GET_TILE_RECORD_LINK(ddata.v1e08a8, ddata.v1e08aa);
   wordrg406 = ddata.v1e08b0;
   wordrg62 = con(0x0);
  skip = false; // added
  for (;; wordrg406 = SKW_GET_NEXT_RECORD_LINK(wordrg406))
  {
    if (wordrg406 == con(0xfffffffe))
      break;
    x16 wordrg11 = urshift(wordrg406 & con(0x3c00), con(0xa));
    if (wordrg11 > con(0x3))
      break;
    if (wordrg11 != con(0x3))
      continue;
    if ((wordrg406 >> con(0xe)) != ((argw1 + con(0x2)) & wordrg11))
      continue;
    recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(wordrg406);
    wordrg62 = ((recoptrrg5->u2.w & con(0x7f)) == con(0x26)) ? 1 : 0;
    if (wordrg62 != con(0x0))
      break;
    // TODO: ORIGINAL BUG in the following line. wordrg62 was RG1W which can never be smth except 0 or 1 now!
    if (wordrg62 == con(0x4a) && ((recoptrrg5->u4.b.b0 & con(0x1)) == con(0x0)))
    {
      skip = true;
      break;
    }
  }
  if (!skip)
  {
    if (wordrg62 == con(0x0))
      goto M_exit;
    x16 wordrg404 = unsignedword(ddata.s350.v1e054e->u4.b.b0);
    if (urshift(recoptrrg5->u2.w, con(0x7)) != wordrg404 || (recoptrrg5->u4.b.b0 & con(0x4)) == con(0x0))
      goto M_exit;
  }
   wordrg61 = argw2;
   wordrg101 = SKW_19f0_266c(ddata.v1e08b0, vb_08, wordrg61, (argw1 + con(0x2)) & con(0x3));
  if (wordrg101 != con(0xffffffff))
    if (SKW_19f0_2723(wordrg101, wordrg61, ulrshift(recoptrrg5->u4.w, con(0xb), con(0xe)), unsignedword(vb_08)))
    {
      if (vw_00 != con(0x0) && !SKW_19f0_0559(argw1))
      {
        ddata.s350.v1e0556->u.v.u18.b.b0 &= con(0xffffffe0);
        ddata.s350.v1e0556->u.v.u18.w |= vw_04 & con(0x1f);
        ddata.s350.v1e0556->u.v.u18.w = (ddata.s350.v1e0556->u.v.u18.w & con(0xfffffc1f)) | ((argw0 & con(0x1f)) << con(0x5));
        ddata.s350.v1e0556->u.v.u18.b.b1 &= con(0x3);
        ddata.s350.v1e0556->u.v.u18.w |= ((ddata.v1d3248 & con(0x3f)) << con(0xa));
        ddata.s350.v1e0556->u.v.b1d = CUTX8(argw1);
        ddata.s350.v1e0556->u.v.b1b = CUTX8(argw1);
        ddata.s350.v1e0556->u.v.b1e = CUTX8(argw2);
        ddata.s350.v1e0556->u.v.b20 = vb_08;
        if (vb_08 != con(0x0))
        {
          if (vb_08 != con(0x1))
            ddata.s350.v1e0556->u.v.b1a = con(0x31);
          else
            ddata.s350.v1e0556->u.v.b1a = con(0x30);
        }
        else
          ddata.s350.v1e0556->u.v.b1a = con(0x2f);
        if ((vb_08 != con(0x0)) || (UI16CAST(recoptrrg5->u4.w & con(0x18)) >= UI16CAST(con(0x10))))
          ddata.s350.v1e056f = con(0xfffffffe);
        else
          ddata.s350.v1e056f = con(0xfffffffc);
      }
      return true;
    }
M_exit: // M_1F6CF
  if (vw_00 != con(0x0))
    ddata.s350.v1e056f = con(0xfffffffd);
  return false;
}

bool SKW_19f0_13aa(x16 eaxw, x16 edxw)
{
  x16 wordrg5;
  c_record* recoptrrg1;

  wordrg5 = con(0x0);
  // M_1D5E9:
  for(;;)
  {
    bool skip = false;
    bool pass = false;
    if ((table1d607e[ddata.s350.v1e0584].u.b[1] & con(0x4)) == con(0x0))
    {
      if ((ddata.s350.v1e054e->ua.w & con(0x80)) == con(0x0))
      {
        if (((ddata.s350.v1e0552->u0.b.b0 & con(0x4)) != con(0x0) || unsignedword(ddata.s350.v1e0562.u.b.b_06) != eaxw) || unsignedword(ddata.s350.v1e0562.u.b.b_07) != edxw)
          skip = true;
        else
        {
          if (wordrg5 != ((ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe)) + con(0x2)) & con(0x3)))
            skip = true;
        }
      }
      if (!skip)
        if ((SKW_RAND() & con(0x7)) != con(0x0))
          pass = true;
    }
    if (!pass)
    {
      x16 wordrg6 = eaxw;
      x16 vw_08 = edxw;
      x16 vw_0c = con(0x0);
      bool boolrg1;
      do
      {
        vw_0c++;
        if (vw_0c >= con(0x3))
          break;
        wordrg6 += table1d27fc[wordrg5];
        vw_08 += table1d2804[wordrg5];
        if (wordrg6 < con(0x0) || wordrg6 >= ddata.savegamewa)
          break;
        if (vw_08 < con(0x0) || vw_08 >= ddata.savegamewb)
          break;
        x16 wordrg1 = SKW_GET_WALL_TILE_ANYITEM_RECORD(wordrg6, vw_08);
        x16 wordrg2;
        for (;; wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg2))
        {
          wordrg2 = wordrg1;
          if (wordrg2 == con(0xfffffffe))
            break;
          if (urshift(wordrg2 & con(0x3c00), con(0xa)) == con(0xe))
          {
            recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg2);
            if (ulrshift(ddata.savegametimarrp[recoptrrg1->u6.w].v.w_08, con(0x4), con(0xe)) == ((wordrg5 + con(0x2)) & con(0x3)))
              if (SKW_075f_06bd(recoptrrg1, recoptrrg1->u2.w) != con(0x0))
                return true;
          }
        }
        if (wordrg6 != unsignedword(ddata.s350.v1e0562.u.b.b_06))
          boolrg1 = true;
        else if (unsignedword(ddata.s350.v1e0562.u.b.b_07) == vw_08)
          boolrg1 = false;
        else
          boolrg1 = true;
      } while (!boolrg1 || !R_1BAAD(wordrg6, vw_08));
    }
    wordrg5++;
    if (wordrg5 >= con(0x4))
      return false;
  }
}

void SKW_QUERY_MESSAGE_TEXT(t_text* eaxt, x16 ebxw, x16 edxw)
{
  c_record* recoptrrg1;
  x16* wptrrg7;
  t_text* tptrrg5;
  const t_text* tptrrg2;
  t_text* tp_00;
  t_text t_04[0x80];
  x16 vw_84;
  x16 vw_88;
  x8 vb_8c;
  x8 vb_90;

  // SPX: moved variables to avoid goto problem
	x16 wordrg402;

  bool skip = false; // added
  tp_00 = eaxt;
  x16 wordrg6 = con(0x0);
  tptrrg5 = t_04;
  vb_90 = con(0xffffffff);
  if (urshift(edxw & con(0x3c00), con(0xa)) != con(0x7))
    recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(edxw);
  else
  {
    recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(edxw);
    x16 wordrg49 = recoptrrg1->u2.w;
    x16 wordrg32 = urshift(wordrg49, con(0xa));
    if (wordrg32 != con(0x0))
    {
      vb_90 = CUTX8(wordrg32);
      skip = true;
    }
    else
    {
      wordrg49 &= con(0x3ff);
      recoptrrg1 = UPCAST(c_record, DOWNCAST(c_record, ddata.savegameparr5[2]) + (unsignedlong(wordrg49) << con(0x2))); // TODO: casts have to stay here, because it seems that records are overlapping!
    }
  }
  if (!skip)
  {
    x16 wordrg48 = ulrshift(recoptrrg1->u2.w, con(0xd), con(0xe));
    if (wordrg48 != con(0x0))
    {
      if (wordrg48 != con(0x1))
        goto M_exit;
      vw_84 = urshift(recoptrrg1->u2.w, con(0x3));
      if ((urshift(vw_84, con(0x8)) & con(0x1f)) != con(0xe))
        goto M_exit;
    }
    if ((recoptrrg1->u2.b.b0 & con(0x1)) == con(0x0) && ((ebxw & con(0x8000)) == con(0x0)))
      goto M_exit;
    if (wordrg48 == con(0x1))
      vb_90 = CUTX8(urshift(recoptrrg1->u2.w, con(0x3)));
  }
   wordrg402 = ebxw & con(0x7fff);
  if (wordrg402 == con(0x1))
  {
    *tptrrg5 = con(0xa);
    tptrrg5 += wordrg402;
  }
  vb_8c = con(0xa);
  if (vb_90 != con(0xffffffff))
    tptrrg5 += SKW_SK_STRLEN(SKW_QUERY_GDAT_TEXT(con(0x3), vb_90, tptrrg5, con(0x0)));
  else
  {
    x16 wordrg3 = con(0x0);
    vw_88 = con(0x0);
    wptrrg7 = &ddata.v1e03d0[urshift(recoptrrg1->u2.w, con(0x3))];
    for(;;)
    {
      x16 wordrg21;
      if (wordrg3 != con(0x0))
      {
        if (wordrg3 != con(0x1))
          wordrg21 = wordrg6 & con(0x1f);
        else
          wordrg21 = (wordrg6 >> con(0x5)) & con(0x1f);
      }
      else
      {
        wordrg6 = *wptrrg7++;
        wordrg21 = urshift(wordrg6, con(0xa)) & con(0x1f);
      }
      wordrg3++;
      wordrg3 %= con(0x3);
      if (vw_88 == con(0x0))
      {
        if (wordrg21 >= con(0x1c))
        {
          if (wordrg21 != con(0x1c))
          {
            if (wordrg21 > con(0x1e))
              break;
            vw_88 = wordrg21;
          }
          else
            *tptrrg5++ = vb_8c;
        }
        else
        {
          if (wordrg21 != con(0x1a))
          {
            if (wordrg21 != con(0x1b))
              wordrg21 += con(0x41);
            else
              wordrg21 = con(0x2e);
          }
          else
            wordrg21 = con(0x20);
          *tptrrg5++ = CUTX8(wordrg21);
        }
      }
      else
      {
        *tptrrg5 = NULLBYTE;
        if (vw_88 != con(0x1e))
          tptrrg2 = UPCAST(t_text, &table1d292c[wordrg21]);
        else
          tptrrg2 = &table1d282c[wordrg21 << con(0x3)];
        SKW_SK_STRCAT(tptrrg5, tptrrg2);
        tptrrg5 += SKW_SK_STRLEN(tptrrg2);
        vw_88 = con(0x0);
      }
    }
  }
M_exit: // M_E25F
  *tptrrg5 = NULLBYTE;
  SKW_FORMAT_SKSTR(t_04, tp_00);
}

void SKW_32cb_4185(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  x16 wordrg5;
  x8* bptrrg21;
  x32* lptrrg2;
  c_record* recoptrrg1;
  s_8wordarray* sptrrg11;
  s_8wordarray* sptrrg12;
  x8* bptrrg13;
  s_hex12* h12ptrrg14;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_14;
  x16 vw_18;
  x16 vw_1c;
  x16 vw_20;

  vw_08 = eaxw;
  vw_04 = edxw;
  vw_1c = eaxw;
  vw_10 = edxw;
  wordrg5 = ecxw;
  SKW_CALC_VECTOR_W_DIR(wordrg5, signedword(table1d6ad0[ebxw][0]), &vw_1c, signedword(table1d6ad0[ebxw][1]), &vw_10);
  x16 wordrg46 = ebxw;
  ddata.v1e1044[wordrg46].b_00 = CUTX8(vw_1c);
  ddata.v1e1044[wordrg46].b_01 = CUTX8(vw_10);
  sptrrg11 = &ddata.v1e1044[wordrg46].s_02;
  SKW_SUMMARIZE_STONE_ROOM(sptrrg11, vw_1c, vw_10, wordrg5);
  if (ddata.v1e025c != con(0x0) && (ebxw == con(0x0)))
  {
    if (ddata.v1e1044[con(0x0)].s_02.warr_00[con(0x0)] != con(0x0))
    {
      if (ddata.v1e1044[con(0x0)].s_02.warr_00[con(0x0)] == con(0x11))
      {
        ddata.v1e1044[con(0x0)].s_02.warr_00[con(0x0)] = con(0x1);
        ddata.v1e1044[con(0x0)].s_02.warr_00[con(0x1)] = ebxw;
        ddata.v1e1044[con(0x0)].s_02.warr_00[con(0x5)] = con(0xff);
      }
    }
    else
    {
      if (ddata.v1e1044[con(0x3)].s_02.warr_00[con(0x0)] != con(0x5))
      {
        ddata.v1e1044[con(0x0)].s_02.warr_00[con(0x3)] = con(0xff);
        ddata.v1e1044[con(0x0)].s_02.warr_00[con(0x4)] = con(0xff);
        ddata.v1e1044[con(0x0)].s_02.warr_00[con(0x5)] = con(0xff);
        ddata.v1e1044[con(0x0)].s_02.warr_00[con(0x6)] = con(0xff);
        ddata.v1e1044[con(0x0)].s_02.warr_00[con(0x7)] = ebxw;
        ddata.v1e1044[con(0x0)].s_02.warr_00[con(0x2)] = con(0xfffffffe);
      }
      else
        ddata.v1e1044[con(0x0)].s_02.warr_00[con(0x0)] = con(0x5);
      wordrg5 = con(0x1);
      for (;; ddata.v1e1038[wordrg5] = ddata.v1e104c[wordrg5] = con(0x0), wordrg5++)
      {
        if (wordrg5 <= con(0x2))
        {
          sptrrg12 = &ddata.v1e1044[wordrg5].s_02;
          sptrrg12->warr_00[con(0x2)] = con(0xfffffffe);
          x16 help = wordrg5;
          sptrrg12->warr_00[con(0x1)] = ddata.v1e1044[con(0x3) + help].s_02.warr_00[con(0x1)] & con(0xe0);
          ui16 uwordrg43 = ddata.v1e1044[con(0x3) + help].s_02.warr_00[con(0x0)];
          sptrrg12->warr_00[con(0x0)] = uwordrg43;
          if (uwordrg43 < con(0x2))
          {
            if (uwordrg43 == con(0x0)) // was jug_tst16
            {
              sptrrg12->warr_00[con(0x6)] = con(0xff);
              sptrrg12->warr_00[con(0x5)] = con(0xff);
              sptrrg12->warr_00[con(0x4)] = con(0xff);
              sptrrg12->warr_00[con(0x3)] = con(0xff);
              sptrrg12->warr_00[con(0x7)] = con(0xffffffff);
              continue;
            }
          }
          else
          {
            if (uwordrg43 <= con(0x2))
            {
              sptrrg12->warr_00[con(0x0)] = con(0x1);
              sptrrg12->warr_00[con(0x1)] = con(0x0);
              continue;
            }
            if (uwordrg43 < con(0x5) || uwordrg43 > con(0x5) && uwordrg43 != con(0x10))
              continue;
          }
          sptrrg12->warr_00[con(0x5)] = con(0xff);
          continue;
        }
        break; // TODO: silly loopform because of continues
      }
    }
  }
  ddata.v1e1038[ebxw] = con(0x0);
  ddata.v1e104c[ebxw] = con(0x0);
  if (ebxw < con(0x10) && (ddata.v1e1044[ebxw].s_02.warr_00[con(0x0)] != con(0x0)))
  {
    bptrrg21 = &ddata.v1e1048[table1d6eb3[ebxw].b0].barr_00[table1d6eb3[ebxw].b1];
    wordrg5 = con(0x0);
    while (wordrg5 < con(0x5))
    {
      x16 wordrg11 = con(0x0);
      while (wordrg11 < con(0x4))
      {
        bptrrg21[wordrg11] = CUTX8(ebxw); // relative to the current barr_00-index
        wordrg11++;
      }
      bptrrg21 += sizeof(s_hex11bytes); // ugly! steps one struct ahead, keeps barr_00-index
      wordrg5++;
    }
  }
  wordrg5 = ddata.v1e1044[ebxw].s_02.warr_00[con(0x2)];
  if (wordrg5 == con(0xfffffffe))
    return;
  SKW_098d_000f(vw_08, signedword(table1d6eaf[ecxw]), &vw_0c, vw_04, &vw_18);
  for(;;)
  {
    x16 wordrg13 = urshift(wordrg5 & con(0x3c00), con(0xa));
    if (wordrg13 != con(0x4))
    {
      bool skip = false;
      x32 longrg1;
      if (wordrg13 != con(0xe))
      {
        if (wordrg13 < con(0x5) || wordrg13 > con(0xa) || ((h12ptrrg14 = &ddata.v1e1044[ebxw], h12ptrrg14->s_02.warr_00[con(0x0)] == con(0x1)) && (h12ptrrg14->s_02.warr_00[con(0x6)] == con(0x0))))
          skip = true;
        else
        {
          longrg1 = signedlong(ebxw) << con(0x2);
          lptrrg2 = ddata.v1e104c;
        }
      }
      else
      {
        longrg1 = signedlong(ebxw) << con(0x2);
        lptrrg2 = ddata.v1e1038;
      }
      if (!skip)
      {
        lptrrg2 += longrg1 / 4;
        *lptrrg2 |= con(0x1) << CUTX8(SKW_QUERY_OBJECT_5x5_POS(wordrg5, ddata.v1e12c8));
      }
    }
    else
    {
      x16 wordrg3;
      x16 wordrg14;
      if (ddata.v1e12ce == con(0x0) || ebxw != con(0x3))
        wordrg14 = SKW_QUERY_OBJECT_5x5_POS(wordrg5, con(0x0));
      else
        wordrg14 = SKW_ROTATE_5x5_POS(signedword(table1d6b76[ddata.v1d324a + 0x42]), t_rotate(table1d6eab[ecxw]));
      SKW_098d_000f(vw_1c, wordrg14, &vw_14, vw_10, &vw_20);
      recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg5);
      if (ebxw != con(0x0))
      {
        x16 wordrg21 = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(recoptrrg1->u4.b.b0)->barr22[con(0x0)];
        SKW_32cb_4069(vw_0c, wordrg21, &vw_14, vw_18, &vw_20);
      }
      x16 wordrg402;
      if (ecxw > con(0x3))
      {
        wordrg3 = vw_18 - vw_20;
        wordrg402 = vw_0c - vw_14;
      }
      else
      {
        x16 wordrg2 = vw_14 - vw_0c;
        wordrg402 = vw_20 - vw_18;
        switch (ecxw)
        {
          case 0:
            wordrg3 = wordrg2;
            wordrg402 = vw_18 - vw_20;
          break;

          case 1:
            wordrg3 = wordrg402;
            wordrg402 = wordrg2;
          break;

          case 2:
            wordrg3 = vw_0c - vw_14;
          break;

          case 3:
            wordrg3 = vw_18 - vw_20;
            wordrg402 = vw_0c - vw_14;
          break;

          default: throw(THROW_DMABORT);
        }
      }
      if (wordrg402 < con(0x11) && DM2_ABS(wordrg3) < con(0xa))
      {
        for(;;)
        {
          if (wordrg402 < con(0x0))
            break;
          if (ddata.v1e1050[wordrg3 + con(0xa)].barr_00[wordrg402] == con(0xffffffff))
            break;
          wordrg402--;
        }
        if (wordrg402 >= con(0x0))
        {
          bptrrg13 = ddata.v1e1050[wordrg3 + con(0xa)].barr_00;
          bptrrg13 += wordrg402;
          *bptrrg13 = CUTX8(ebxw);
        }
      }
    }
    wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg5);
    if (wordrg5 == con(0xfffffffe))
      return;
  }
}

// was SKW_32cb_3143
void SKW_DRAW_FLYING_ITEM(x16 eaxw, x16 ebxw, x16 edxw)
{
  x16 wordrg5;
  c_record* recoptrrg2;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_18;
  x16 vw_1c;
  x16 vw_28;
  x8 vb_2c;
  x8 vb_30;
  x8 vb_34;
  bool flag;

  vw_08 = eaxw;
  vw_04 = edxw;
  if (eaxw == con(0xfffffffe))
    return;
  wordrg5 = SKW_DIR_FROM_5x5_POS(ebxw);
  if (wordrg5 == con(0xffffffff))
    return;
  vw_28 = signedword(table1d6b15[vw_04]);
  if (vw_28 == con(0x0) && wordrg5 >= con(0x2))
    return;
  vw_0c = signedword(table1d6afe[vw_04]);
  if (table1d6b43[vw_04] < con(0x0))
    return;
  vw_10 = signedword(ddata.v1e1044[vw_04].b_00);
  vw_1c = signedword(ddata.v1e1044[vw_04].b_01);
  for(;;)
  {
    x16 wordrg3 = vw_08;
    if (ebxw == SKW_QUERY_OBJECT_5x5_POS(wordrg3, ddata.v1e12c8))
    {
      if (urshift(wordrg3 & con(0x3c00), con(0xa)) == con(0xe))
      {
        x16 wordrg6 = vw_04;
        x16 vw_24 = SKW_QUERY_CREATURE_BLIT_RECTI(wordrg6, t_rotate(0), ebxw);
        x8 vb_14 = SKW_48ae_011a(wordrg3);
        if (vb_14 != con(0xffffffff))
        {
          recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(wordrg3);
          x8 byterg40 = SKW_QUERY_CLS1_FROM_RECORD(wordrg3);
          vb_2c = byterg40;
          vb_34 = SKW_QUERY_CLS2_FROM_RECORD(wordrg3);
          flag = byterg40 != con(0xd);
          x8 boolrg3 = !flag;
          bool skip = false; // added
          if ((flag || recoptrrg2->u4.b.b0 == con(0xffffffff)) && vw_28 == con(0x0))
          {
            vw_18 = con(0x40);
            wordrg6 = con(0x40);
          }
          else
          {
            wordrg6 = 2 * vw_28 - wordrg5 / 2;
            if (wordrg6 < con(0x0))
              skip = true;
            else
            {
              vw_18 = unsignedword(table1d6b76[wordrg6 + 0x1b]);
              wordrg6 = vw_18;
              if (boolrg3)
              {
                x32 longrg1 = unsignedlong(recoptrrg2->u4.b.b0);
                if (recoptrrg2->u2.w == con(0xffffff82))
                  longrg1 = longrg1 / 2 + con(0x80);
                longrg1 = ((longrg1 << con(0x7)) / con(0xff)) + con(0x1);
                vw_18 = DM2_MAX(con(0x8), SKW_CALC_STRETCHED_SIZE(CUTX16(longrg1) >> con(0x1), vw_18));
              }
            }
          }
          if (!skip)
          {
            x16 vw_20 = con(0x0);
            x8 byterg33 = vb_14;
            if (byterg33 != con(0x3))
            {
              x16 wordrg111 = ulrshift(ddata.savegametimarrp[recoptrrg2->u6.w].v.w_08, con(0x4), con(0xe));
              x16 wordrg21 = wordrg111 & con(0x1);
              if (wordrg21 == (ddata.v1e12c8 & con(0x1)))
              {
                if (byterg33 != con(0x0))
                {
                  if (byterg33 == con(0x2) || (byterg33 == con(0x1) && wordrg111 != ddata.v1e12c8))
                    vb_30 = con(0x8);
                  else
                    vb_30 = con(0xa);
                }
                else
                {
                  if (((vw_1c + vw_10) & con(0x1)) == con(0x0))
                  {
                    if (wordrg5 >= con(0x2))
                      vb_30 = con(0x8);
                    else
                      vb_30 = con(0x9);
                  }
                  else
                  {
                    vw_20 = con(0x2);
                    if (wordrg5 >= con(0x2))
                      vb_30 = con(0x9);
                    else
                      vb_30 = con(0x8);
                  }
                }
                if (vw_0c < con(0x1) && (vw_0c != con(0x0) || (wordrg5 != con(0x1) && wordrg5 != con(0x2))))
                  vw_20 |= con(0x1);
                if ((wordrg5 & con(0x1)) != con(0x0) && vb_2c == con(0xd))
                  vw_20 |= con(0x2);
              }
              else
              {
                vb_30 = con(0xc);
                if (byterg33 != con(0x0))
                {
                  if (((ddata.v1e12c8 + con(0x1)) & con(0x3)) == wordrg111)
                    vw_20 = con(0x1);
                }
                else
                {
                  if (wordrg5 == con(0x0))
                    vw_20 |= con(0x1);
                  else
                  {
                    if (wordrg5 == con(0x3))
                      vw_20 |= con(0x1);
                  }
                  if (((vw_1c + vw_10) & con(0x1)) != con(0x0))
                    vw_20 |= con(0x2);
                  else
                    vw_20 ^= con(0x1);
                }
              }
            }
            else
              vb_30 = con(0x8);
            x16 wordrg15;
            if (vb_2c != con(0xd))
              wordrg15 = con(0x3);
            else
              wordrg15 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xd), con(0xb), con(0x1), vb_34);
            wordrg15 &= vw_20;
            SKW_QUERY_TEMP_PICST(wordrg15, vw_18, con(0x0), vw_18, con(0x0), vw_28, vw_24 | con(0x8000), con(0x0), con(0xa), con(0xffff), vb_2c, vb_34, vb_30);
            ddata.v1e118e.w_20 += SKW_CALC_STRETCHED_SIZE(con(0x0), wordrg6);
            ddata.v1e118e.w_22 += SKW_CALC_STRETCHED_SIZE(con(0xffffffa4), wordrg6);
            SKW_DRAW_TEMP_PICST();
          }
        }
        else
        {
          x16 wordrg14 = SKW_GET_ADDRESS_OF_RECORD(wordrg3)->u2.w & con(0x3fff);
          wordrg3 >>= con(0xe);
          wordrg3 <<= con(0xe);
          wordrg14 |= wordrg3;
          SKW_DRAW_ITEM(wordrg14, con(0x0), con(0xffffffa4), wordrg6, con(0xffff), NULL, true, con(0x0), false);
        }
      }
    }
    vw_08 = SKW_GET_NEXT_RECORD_LINK(vw_08);
    if (vw_08 == con(0xfffffffe))
      return;
  }
}

void SKW_32cb_2d8c(x16 eaxw, x32 ebxl, x16 edxw)
{
  x8 byterg5;
  x16 wordrg5;
  c_record* recoptrrg5;
  c_record* recoptrrg6;
  t_gfxdata* gptrrg7;
  bool flag;
  x16 vw_04;
  x16 vw_08;
  c_rect rc_0c;
  x16 vw_20;
  x16 vw_28;
  x16 vw_30;
  x16 vw_34;
  x16 vw_38;
  x8 vb_3c;

  vw_08 = eaxw;
  vw_04 = edxw;
  if (eaxw == con(0xfffffffe))
    return;
  if (table1d6b5a[vw_04] < con(0x0))
    return;
  vw_38 = table1d6b15[vw_04];
  // M_41949:
  for (x16 vw_2c = con(0x0); vw_2c < con(0x5); vw_2c++)
  {
    x16 wordrg1 = signedword(table1d6d5a[ddata.v1e12c8][vw_2c]);
    vw_20 = wordrg1;
    vw_08 = eaxw;
    // note: in the original code the loopcondition is not requested when entering the
    // loop, but wordrg1 cannot have the -2 - value to break the loop, because there
    // is no matching value in the const table
    // M_41549:
    for (; wordrg1 != con(0xfffffffe); wordrg1 = SKW_GET_NEXT_RECORD_LINK(vw_08), vw_08 = wordrg1)
    {
      if (urshift(vw_08 & con(0x3c00), con(0xa)) != con(0xf))
        continue;
      vw_30 = urshift(vw_08, con(0xe));
      recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(vw_08);
      recoptrrg6 = recoptrrg5;
      vb_3c = recoptrrg5->u2.w & con(0x7f);
      bool pass = false; // added
      if (!SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0xd), con(0x1), con(0x14), vb_3c))
      {
        if ((recoptrrg5->u2.b.b0 & con(0xffffff80)) == con(0x0))
          flag = vw_20 == vw_30;
        else
          flag = vw_20 == con(0xffffffff);
        if (!flag)
          continue;
        bool vbool_14 = false;
        byterg5 = vb_3c;
        vw_28 = SKW_RAND02() & DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xd), con(0xb), con(0x0), byterg5);
        if (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xd), con(0xb), con(0x41), byterg5) == con(0x0))
          wordrg5 = urshift(recoptrrg6->u2.w, con(0x8));
        else
          wordrg5 = con(0xff);
        if (vw_04 != con(0x0))
        {
          if (vb_3c == con(0x30))
            SKW_UPDATE_GLOB_VAR(con(0x40), con(0x3), true);
          if (vw_38 == con(0x0) && vw_2c >= con(0x3))
            continue;
        }
        else
        {
          vbool_14 = true;
          if (vb_3c != con(0x30))
            pass = true;
          else
            SKW_UPDATE_GLOB_VAR(con(0x41), con(0x3), true);
        }
        if (!pass)
        {
          x16 wordrg7;
          if ((recoptrrg6->u2.b.b0 & con(0xffffff80)) == con(0x0))
            wordrg7 = table1d2752[vw_30];
          else
            wordrg7 = con(0xc);
          x16 wordrg156 = SKW_ROTATE_5x5_POS(wordrg7, t_rotate(ddata.v1e12c8));
          if (((con(0x1) << wordrg156) & ebxl) == con(0x0))
            continue;
          wordrg156 /= con(0x5);
          vw_34 = unsignedword(table1d6b76[4 * vw_38 + con(0x2) - wordrg156]);
          x16 wordrg25 = SKW_QUERY_CREATURE_BLIT_RECTI(vw_04, t_rotate(ddata.v1e0258), wordrg7) | con(0x8000);
          SKW_32cb_2cf3(vb_3c, vw_28, wordrg25, (vw_34 * DM2_MAX(con(0x30), wordrg5 + con(0x1))) >> con(0x8));
          x16 wordrg24 = wordrg5 - con(0xa0);
          if (wordrg24 > con(0x0))
          {
            x16 wordrg23 = con(0xc0) - wordrg24;
            if (vbool_14)
              if (signedword(table1d6b15[0]) == vw_38)
                wordrg23 = con(0x1e);
            ddata.v1e118e.w_22 = signedword(ddata.v1e118e.w_22) + (ddata.vx1d270c << con(0x3)) / wordrg23;
            if ((recoptrrg6->u2.b.b0 & con(0xffffff80)) == con(0x0))
            {
              x16 wordrg22 = (ddata.vx1d270a << con(0x3)) / (wordrg23 - con(0xa));
              if (vw_2c == con(0x1) || vw_2c == con(0x4))
                wordrg22 = -wordrg22;
              ddata.v1e118e.w_20 += wordrg22;
            }
          }
          ddata.v1e118e.w_20 = SKW_CALC_STRETCHED_SIZE(ddata.v1e118e.w_20, vw_34);
          ddata.v1e118e.w_22 = SKW_CALC_STRETCHED_SIZE(ddata.v1e118e.w_22, vw_34);
          SKW_DRAW_TEMP_PICST();
          if (!vbool_14)
            continue;
        }
      }
      else
      {
        if (vw_2c == con(0x4))
          SKW_DRAW_TELEPORTER_TILE(vw_04, vb_3c, con(0xd));
        continue;
      }
      x8 byterg2 = con(0x0);
      vw_30 = urshift(wordrg5, con(0x5));
      if (vw_30 != con(0x0))
      {
        byterg2 = con(0x1);
        if (vw_30 > con(0x3))
          byterg2 = con(0x2);
      }
      byterg5 = byterg2 + con(0x42);
      gptrrg7 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0xd), byterg5, vb_3c);
      s_gfxdatatail* ptrbeforerg7 = getprevious(gptrrg7);
      SKW_3e74_58bf(con(0xd), byterg5, vb_3c);
      x16 wordrg402 = ptrbeforerg7->width * ptrbeforerg7->height - con(0x28);
      SKW_44c8_20a4(gptrrg7, NULL, SKW_QUERY_EXPANDED_RECT(con(0x4), &rc_0c), UPCAST(t_gfxdata, ddata.bitmapptr), wordrg402 + CUTX16(SKW_RAND() & con(0x1f)), SKW_RAND16(wordrg402), getprevious(ddata.bitmapptr)->width, con(0x0), SKW_32cb_0649(con(0xd), byterg5, con(0x0), vb_3c));
      SKW_3e74_5992(con(0xd), byterg5, vb_3c);
    }
  }
}

void SKW_QUERY_CREATURE_PICST(x16 eaxw, c_record* ebxprec, s_d556* ecxpd5, x16 edxw, x16 argw0)
{
  x16 wordrg5;
  x16* wptrrg1;
  s_4bytearray* s4baptrrg1;
  x16 vw_04;
  s_8bytearray vs_08;
  s_8bytearray vs_10;
  x16 vw_20;
  x16 vw_28;
  x16 vw_2c;
  x16 vw_30;
  x16 vw_34;
  x16 vw_38;
  x16 vw_3c;
  x8 vb_40;
  x8 vb_44;
  x8 vb_48;

  vw_04 = eaxw;
  bool vbool_1c = false;
  vb_48 = ebxprec->u4.b.b0;
  x16 wordrg6 = con(0x0);
  wptrrg1 = SKW_1c9a_02c3(ebxprec, SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(vb_48));
  R_4DA3(vb_48, wptrrg1 + con(0x1), &vs_10, *wptrrg1);
  if (ecxpd5 != NULL)
    vw_3c = unsignedword(ecxpd5->barr04[con(0x3)]);
  else
    vw_3c = con(0x0);
  x16 vw_24;
  if ((SKW_QUERY_CREATURE_AI_SPEC_FLAGS(argw0) & con(0x4)) == con(0x0))
    vw_24 = (ddata.v1e12c8 - ulrshift(ebxprec->ue.w, con(0x6), con(0xe))) & con(0x3);
  else
    vw_24 = con(0x2);
  vb_44 = vs_10.barr_00[vw_24];
  if (SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0xf), con(0x1), vb_44, vb_48))
  {
    x16 wordrg144 = (unsignedword(vs_10.barr_00[0x7]) >> (2 * (con(0x3) - CUTX8(vw_24)))) & con(0x3);
    x16 wordrg402 = wordrg144 & con(0x1);
    if (wordrg402 != con(0x0))
      wordrg6 = con(0x1);
    else
    {
      if ((vw_3c & con(0x40)) != con(0x0))
        if ((wordrg144 & con(0x2)) != con(0x0))
          if (wordrg402 == con(0x0))
            wordrg6 = con(0x1);
    }
  }
  else
  {
    x8 byterg13 = CUTX8(vw_24) + con(0x2);
    vb_44 = byterg13 & con(0x3);
    byterg13 &= con(0x1);
    if (byterg13 != con(0x0))
    {
      wordrg6 = con(0x1);
      vbool_1c = true;
    }
    vb_44 = vs_10.barr_00[vb_44];
    if (!SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0xf), con(0x1), vb_44, vb_48) == con(0x0))
    {
      wordrg6 = con(0x0);
      vb_44 = vs_10.barr_00[0x2];
    }
  }
  if (!SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0xf), con(0x1), vb_44, vb_48))
  {
    vw_30 = wordrg6;
    wordrg6 = con(0x0);
    vb_44 = CUTX8(vw_24) + con(0xfffffffa);
    if (!SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0xf), con(0x1), vb_44, vb_48))
    {
      if ((vw_24 & con(0x1)) == con(0x0))
      {
        vb_44 = con(0xfffffffc);
        wordrg6 = vw_30;
      }
      else
      {
        vb_40 = ((CUTX8(vw_24) + con(0x2)) & con(0x3)) + con(0xfffffffa);
        if (!SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0xf), con(0x1), vb_40, vb_48))
        {
          vb_44 = con(0xfffffffc);
          wordrg6 = vw_30;
        }
        else
        {
          wordrg6 = con(0x1);
          vb_44 = vb_40;
          vbool_1c = true;
        }
      }
    }
    if (wordrg6 != vw_30)
      vbool_1c = true;
  }
  vw_38 = unsignedword(table1d6b71[edxw]);
  if (ecxpd5 == NULL || ecxpd5->u.v.b1a != con(0x13))
    SKW_COPY_MEMORY(DOWNCAST(s_8bytearray, &vs_10), con(0x8), DOWNCAST(s_8bytearray, &vs_08));
  else
  {
    vw_20 = ecxpd5->u.v.w10;
    R_4DA3(vb_48, &vw_20, &vs_08, ecxpd5->u.v.w0e);
    vs_10.barr_00[0x5] = vs_08.barr_00[0x5];
  }
  vw_28 = unsignedword(vs_08.barr_00[0x4]);
  if (vw_04 != con(0x3) || ddata.v1e12ce == con(0x0))
  {
    s4baptrrg1 = UPCAST(s_4bytearray, SKW_QUERY_GDAT_ENTRY_DATA_PTR(con(0xf), con(0x7), con(0xfe), vb_48));
    vw_38 = SKW_CALC_STRETCHED_SIZE(unsignedword(s4baptrrg1[vs_10.barr_00[0x5]].barr_00[vw_24]), vw_38);
  }
  else
  {
    wordrg5 = ddata.v1d324a;
    vw_28 = signedword(table1d6b76[wordrg5 + 0x42]);
    vw_38 = signedword(table1d6b76[wordrg5 + 0x49]);
    wordrg5 = con(0x0);
    vw_24 = con(0x0);
  }
  x16 wordrg3 = SKW_QUERY_CREATURE_BLIT_RECTI(vw_04, t_rotate(vw_24), vw_28) | con(0x8000);
  wordrg5 = signedword(vs_08.barr_00[0x6]);
  x16 wordrg2 = wordrg5;
  if (wordrg5 != con(0x0))
  {
    if (UI16CAST(vw_24) <= UI16CAST(con(0x3)))
    {
      switch (vw_24)
      {
        case 0:
          vw_34 = con(0xfffffff9);
          vw_2c = con(0x0);
        break;

        case 1:
        case 3:
          if (!vbool_1c)
            vw_2c = con(0xffffffc0);
          else
            vw_2c = con(0x40);
          vw_34 = con(0x0);
        break;

        case 2:
          vw_34 = con(0x7);
          vw_2c = con(0x0);
        break;

        default: throw(THROW_DMABORT);
      }
    }
    wordrg2 = SKW_CALC_STRETCHED_SIZE(vw_2c, wordrg2);
    wordrg5 = SKW_CALC_STRETCHED_SIZE(vw_34, wordrg5);
  }
  SKW_QUERY_TEMP_PICST(wordrg6, vw_38, signedword(table1d6b76[(vw_3c & con(0x7)) + 0x50]) + wordrg2, vw_38, signedword(table1d6b76[((vw_3c >> con(0x3)) & con(0x7)) + 0x50]) + wordrg5, edxw, wordrg3, con(0xffffffff), SKW_0cee_2e35(vb_48), con(0xffff), con(0xf), vb_48, vb_44);
}

void SKW_CHANCE_TABLE_OPERATION(void)
{
  x16 wordrg5;
  x16 vw_00;
  x16 vw_04;

  bool skip = false; // added
  if (ddata.v1e12ce == con(0x0) || ddata.v1d4000 != con(0xffffffff))
    return;
  SKW_443c_08ab(&vw_00, &ddata.v1e00b2, &vw_04);
  x16 wordrg6 = vw_00 - ddata.v1d26fc;
  wordrg5 = vw_04 - ddata.v1d26fe;
  if (wordrg6 >= con(0x0) && wordrg5 >= con(0x0) && wordrg6 < ddata.vx1d270a && wordrg5 < (ddata.mouse_unk0d - ddata.v1d26fe))
  {
    wordrg6 -= ddata.vx1d270a >> con(0x1); // was long_at(idx(0x1d2708)) >> con(0x11);
    wordrg5 -= ddata.vx1d270c >> con(0x1); // was long_at(idx(0x1d270a)) >> con(0x11);
    if (DM2_ABS(wordrg6) >= con(0x14))
    {
      if (wordrg6 >= con(0x0))
      {
        if (DM2_ABS(wordrg5) >= con(0xf))
        {
          if (wordrg5 < con(0x0))
            wordrg5 = con(0x0);
          else
            wordrg5 = con(0x2);
        }
        else
          wordrg5 = con(0x1);
      }
      else
      {
        if (DM2_ABS(wordrg5) >= con(0xf))
        {
          if (wordrg5 < con(0x0))
            wordrg5 = con(0x0);
          else
            wordrg5 = con(0x4);
        }
        else
          wordrg5 = con(0x5);
      }
    }
    else
    {
      if (DM2_ABS(wordrg5) < con(0xf))
      {
        wordrg5 = con(0x6);
        skip = true;
      }
      else
      {
        if (wordrg5 >= con(0x0))
          wordrg5 = con(0x3);
        else
          wordrg5 = con(0x0);
      }
    }
    if (!skip)
    {
      x16 wordrg61 = wordrg5;
      x16 wordrg103 = (table1d6d54[wordrg61] + ddata.v1e0258) & con(0x3);
      x16 wordrg402 = ddata.v1e0270;
      wordrg402 += table1d27fc[wordrg103];
      if (SKW_IS_TILE_BLOCKED(SKW_GET_TILE_VALUE(wordrg402, ddata.v1e0272 + table1d2804[wordrg103])))
        wordrg5 = con(0x6);
      else
      {
        if (!SKW_IS_CREATURE_MOVABLE_THERE(ddata.v1e0270 + table1d27fc[ddata.v1e0258], (table1d6d4e[wordrg61] + ddata.v1e0258) & con(0x3), NULL, ddata.v1e0272 + table1d2804[ddata.v1e0258]))
          wordrg5 = con(0x6);
        else
          SKW_HIGHLIGHT_ARROW_PANEL(table1d6d48[wordrg61], true, table1d6d3c[wordrg61]);
      }
    }
  }
  else
    wordrg5 = con(0x6);
  ddata.v1d324a = wordrg5;
}

x16 SKW_32cb_15b8(x16 eaxw, bool ebxbool, x16 edxw)
{
  x16 wordrg5;
  x8 byterg5;
  t_gfxdata* gptrrg7;
  c_rect* recoptrrg3;
  t_text* tptrrg6;
  s_hex12* h12ptrrg3;
  c_image vs_04;
  c_image vs_140;
  t_text t_27c[0x50];
  c_rect rc_2cc;
  t_gfxdata* pg_2ec;
  x16 vw_304;
  x16 vw_308;
  x16 vw_30c;
  x16 vw_310;
  x16 vw_314;
  x16 vw_318;
  x16 vw_31c;
  x16 vw_320;
  x16 vw_324;
  x16 vw_328;
  x16 vw_330;
  x16 vw_334;
  x16 vw_338;
  x8 vb_33c;
  x16 vw_340;
  x8 vb_344;
  x8 vb_348; // add8

  wordrg5 = edxw;
  x16 wordrg400;
  if (edxw > con(0xffffffff))
  {
    if (edxw < con(0x1))
      wordrg400 = con(0x5);
    else
      wordrg400 = con(0x6);
  }
  else
    wordrg400 = con(0x4);
  h12ptrrg3 = &ddata.v1e1044[eaxw];
  x16 wordrg499 = h12ptrrg3->s_02.warr_00[wordrg400];
  vb_33c = CUTX8(wordrg499 >> con(0x8));
  vw_310 = signedword(table1d6b15[eaxw]);
  vb_344 = CUTX8(wordrg499);
  if (vb_344 == con(0xffffffff))
    return con(0xffffffff);
  bool vbool_300 = vb_344 == con(0x0);
  vw_308 = SKW_IS_WALL_ORNATE_ALCOVE(vb_344);
  if (!vbool_300)
    DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0x7), vb_344); // why? no return
  bool vbool_2fc = ddata.v1d6c15;
  bool skip = false; // added
  x16 wordrg6 = con(0x0);
  if (!vbool_300)
  {
    vw_324 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0x4), vb_344);
    if (vw_324 != con(0x0))
      skip = true;
  }
  if (!skip)
    vw_324 = ddata.v1d6c00;
  skip = false; // used for another part now
  x16 wordrg410;
  if (!vbool_300)
  {
    x16 wordrg411 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0x5), vb_344);
    if (wordrg411 != con(0x0))
    {
      vw_338 = urshift(wordrg411, con(0x8));
      wordrg410 = unsignedword(CUTX8(wordrg411)) - con(0x1);
      skip = true;
    }
  }
  if (!skip)
  {
    wordrg410 = con(0xc);
    vw_338 = con(0x0);
  }
  vw_30c = SKW_098d_0cd7(eaxw, wordrg5 != con(0x0), wordrg410);
  vw_334 = vw_30c;
  vw_31c = vw_318 = unsignedword(table1d6b71[vw_310]);
  if (vw_308 == con(0x3) && wordrg5 == con(0x0))
  {
    x16 wordrg409 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xc), con(0xfd), vb_344);
    SKW_QUERY_TEMP_PICST(wordrg6, vw_318, urshift(wordrg409, con(0x8)), vw_318, signedword(CUTX8(wordrg409)), vw_310, vw_30c, vw_338, con(0xffff), con(0xffff), con(0x16), CUTX8(ddata.v1e1044[eaxw].s_02.warr_00[0x7]), con(0x1));
    if (!ebxbool)
      ddata.v1e118e.w_30 = con(0xfffffffe);
    SKW_DRAW_TEMP_PICST();
    if (!ebxbool)
      return vw_324;
    if (vw_310 == con(0x1))
      SKW_32cb_0a4c(&ddata.v1e118e.rc_24, CUTX8(eaxw), con(0x6));
  }
  if (vw_310 != con(0x2) || wordrg5 > con(0xfffffffe) && wordrg5 < con(0x2))
  {
    if (vw_310 == con(0x3) && (wordrg5 <= con(0xfffffffe) || wordrg5 >= con(0x2)))
      vw_31c = con(0x4c);
  }
  else
    vw_31c = con(0x72);
  if (!vbool_300)
  {
    if (wordrg5 != con(0x0))
    {
      vb_348 = con(0x0);
      if (wordrg5 >= con(0x1))
      {
        if (!SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0x9), con(0x1), vb_33c + con(0x2), vb_344))
          wordrg6 = con(0x1);
        else
          vb_348 = con(0x2);
      }
    }
    else
    {
      vb_348 = con(0x1);
      if (!vbool_2fc)
      {
        if ((table1d6b15[eaxw] & con(0x1)) == con(0x0))
          wordrg6 = ddata.v1e12d0 ^ con(0x1);
        else
          wordrg6 = ddata.v1e12d0;
      }
    }
    vb_348 += vb_33c;
    if (wordrg5 != con(0x0) || (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0x63), vb_344) == con(0x0)))
    {
      SKW_QUERY_TEMP_PICST(wordrg6, vw_318, con(0x0), vw_31c, con(0x0), vw_310, vw_334, vw_338, vw_324, con(0xffff), con(0x9), vb_344, vb_348);
      if (!ebxbool)
        ddata.v1e118e.w_30 = con(0xfffffffe);
      SKW_DRAW_TEMP_PICST();
      recoptrrg3 = &ddata.v1e118e.rc_24;
      if (!ebxbool)
        return vw_324;
    }
    else
    {
      SKW_QUERY_TEMP_PICST(wordrg6, vw_318, con(0x0), vw_31c, con(0x0), vw_310, vw_334, vw_338, con(0xfffd), con(0xfffd), con(0x9), vb_344, vb_348);
      SKW_QUERY_TEMP_PICST(wordrg6, vw_318, ddata.v1e118e.w_1c, vw_31c, ddata.v1e118e.w_1e, vw_310, vw_334, vw_338, vw_324, con(0xfffff), con(0x9), vb_344, con(0xc8));
      if (!ebxbool)
        ddata.v1e118e.w_30 = con(0xfffffffe);
      SKW_DRAW_TEMP_PICST();
      vw_328 = SKW_QUERY_MULTILAYERS_PIC(&vs_140, vb_344, vb_348, con(0x9), vw_31c, vw_318, vw_310, wordrg6, vw_324, DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0x11), vb_344));
      SKW_32cb_0c7d(&vs_140, vw_324, vw_328);
      vs_140.pg_2c = SKW_3e74_5817(vw_328);
      SKW_DRAW_PICST(&vs_140);
      SKW_0b36_01cd(&vs_140.s_00);
      if (vs_140.w_38 > con(0x0))
        vs_140.w_30 = unsignedword(vs_140.carr_3a[vw_324]);
      else
        vs_140.w_30 = vw_324;
      vs_140.pg_2c = ddata.bitmapptr;
      vs_140.w_18 = vw_334;
      vs_140.w_1a = vw_338;
      vs_140.w_38 = con(0x0);
      SKW_0b36_00c3(vw_328, &vs_140.s_00);
      SKW_DRAW_PICST(&vs_140);
      recoptrrg3 = &vs_140.rc_24;
      SKW_3e74_58a3(vw_328);
    }
    if ((ebxbool && vw_308 != con(0x3)) && (eaxw == con(0x1) || eaxw == con(0x2) || eaxw == con(0x3)))
      SKW_32cb_0a4c(recoptrrg3, CUTX8(eaxw), con(0x6));
    if ((vw_308 != con(0x0)) && (wordrg5 == con(0x0)))
    {
      SKW_COPY_IMAGE(&ddata.v1e118e, &ddata.v1e1054);
      byterg5 = vb_344;
      if (!SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0x9), con(0x1), con(0xf), byterg5))
      {
        if (vw_308 == con(0x1))
          SKW_32cb_3f0d(eaxw);
      }
      else
      {
        if (vw_308 != con(0x2))
        {
          SKW_QUERY_TEMP_PICST(wordrg6, vw_318, con(0x0), vw_31c, con(0x0), vw_310, vw_334, vw_338, vw_324, con(0xffff), con(0x9), byterg5, con(0xf));
          if (!ebxbool)
            ddata.v1e118e.w_30 = con(0xfffffffe);
          SKW_DRAW_TEMP_PICST();
        }
        else
          SKW_32cb_0f82(SKW_GET_ADDRESS_OF_RECORD(ddata.v1e1044[eaxw].s_02.warr_00[con(0x7)]), vw_310, eaxw, byterg5, vw_31c, vw_318, vw_334, vw_338, vw_324);
      }
    }
    return vw_324;
  }
  if (wordrg5 != con(0x0))
  {
    if (wordrg5 > con(0xffffffff))
    {
      if (!SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0x8), con(0x1), con(0xfe), ddata.v1d6c02))
      {
        vb_348 = con(0xfffffffd);
        wordrg6 = con(0x1);
      }
      else
        vb_348 = con(0xfffffffe);
    }
    else
      vb_348 = con(0xfffffffd);
  }
  else
    vb_348 = con(0xfffffffc);
  SKW_QUERY_TEMP_PICST(wordrg6, vw_318, con(0x0), vw_31c, con(0x0), vw_310, vw_334, vw_338, vw_324, con(0xffff), con(0x8), ddata.v1d6c02, vb_348);
  if (!ebxbool)
    ddata.v1e118e.w_30 = con(0xfffffffe);
  SKW_DRAW_TEMP_PICST();
  if (wordrg5 == con(0x0))
  {
    x16 wordrg61 = ddata.v1e1044[eaxw].s_02.warr_00[0x7];
    if (wordrg61 != con(0xffffffff))
    {
      SKW_QUERY_GDAT_SUMMARY_IMAGE(&vs_04, ddata.v1d6c02, vb_348, con(0x8));
      vw_340 = vs_04.w_1c; // adj
      vw_320 = vs_04.w_1e; // adj
      SKW_QUERY_GDAT_IMAGE_METRICS(con(0x8), vb_348, &vw_314, ddata.v1d6c02, &vw_304);
      vw_328 = SKW_3e74_5888();
      SKW_QUERY_MESSAGE_TEXT(t_27c, con(0x2), wordrg61);
      wordrg5 = con(0x0);
      x16 wordrg62 = con(0x1);
      for(;;)
      {
        t_text txt = t_27c[wordrg5];
        wordrg5++;
        if (txt == con(0x0))
          break;
        if (txt == con(0xa))
          wordrg62++;
      }
      x16 vw_32c = ddata.v1d273e;
      vw_330 = ddata.v1d2740 + con(0x2);
      wordrg5 = vw_328;
      SKW_FILL_ENTIRE_PICT(SKW_ALLOC_NEW_PICT(wordrg5, vw_304, con(0x4), vw_314), CUTX8(vw_324));
      x16 wordrg402 = (wordrg62 * vw_330) / 2;
      rc_2cc.y = (vw_304 >> con(0x1)) - wordrg402;
      pg_2ec = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x8), con(0x3), ddata.v1d6c02);
      rc_2cc.w = vw_32c;
      s_gfxdatatail* ptrbefore = getprevious(pg_2ec);
      rc_2cc.h = ptrbefore->height;
      gptrrg7 = SKW_3e74_5817(wordrg5);
      tptrrg6 = t_27c;
      do
      {
        wordrg5 = con(0x0);
        for(;;)
        {
          if (tptrrg6[wordrg5] == con(0x0) || tptrrg6[wordrg5] == con(0xa))
            break;
          wordrg5++;
        }
        if (wordrg5 != con(0x0))
        {
          rc_2cc.x = urshift(vw_314, con(0x1)) - vw_32c * wordrg5 / 2;
          if (rc_2cc.x >= con(0x0))
          {
            for(;;)
            {
              wordrg5--;
              if (wordrg5 == con(0xffffffff))
                break;
//                                   srcbmp                                       srcofs                         destbmp                                      palette
              SKW_DRAW_DIALOGUE_PICT(pg_2ec, &rc_2cc, unsignedword(SKW_SKCHR_TO_SCRIPTCHR(*tptrrg6++)) * vw_32c, gptrrg7, con(0x0), vw_324, SKW_QUERY_GDAT_IMAGE_LOCALPAL(con(0x8), con(0x3), ddata.v1d6c02));
              rc_2cc.x += vw_32c;
            }
          }
          else
            tptrrg6 += wordrg5;
        }
        rc_2cc.y += vw_330;
      } while (*tptrrg6++ != NULLBYTE);
      SKW_QUERY_GDAT_SUMMARY_IMAGE(&vs_04, con(0x0), con(0x0), con(0xff));
      vs_04.s_00.w_0c = vw_328;
      vs_04.s_00.w_06 = con(0xffffffff);
      vs_04.w_34 = vw_31c;
      vs_04.w_36 = vw_318;
      SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_LOCALPAL(con(0x8), con(0x3), ddata.v1d6c02)), PAL16, DOWNCAST(t_gfxdata, vs_04.carr_3a));
      vs_04.w_38 = con(0x10);
      SKW_32cb_0804(vs_04.carr_3a, vw_324, con(0xffffffff), vw_310, &vs_04.w_38);
      vs_04.pg_2c = ddata.bitmapptr;
      vs_04.w_18 = vw_334;
      vs_04.w_1a = vw_338;
      vs_04.w_1c = vw_340;
      vs_04.w_1e = vw_320;
      vs_04.w_30 = vw_324;
      SKW_DRAW_PICST(SKW_QUERY_PICST_IT(&vs_04));
      SKW_3e74_58a3(vw_328);
    }
  }
  return vw_324;
}

void SKW_32cb_0f82(c_record* eaxprec, x16 ebxw, x16 ecxw, x8 edxb, x16 argw0, x16 argw1, x16 argw2, x16 argw3, x16 argw4)
{
  x16 wordrg5;
  x16* wptrrg7;
  t_gfxdata* gptrrg5;
  c_record* recoptrrg6;
  c_rect rc_00;
  t_gfxdata vga_08[0x8];
  s_x40bytes* ps_14;
  x16 vw_20; // x0
  x16 vw_24; // y0
  x16 vw_28;
  x16 vw_2c;
  x16 vw_30;
  x16 vw_34;
  x16 vw_38;
  x16 vw_44;
  x8 vb_4c;
  x8 vb_50;

  vb_50 = edxb;
  vw_38 = ebxw;
  vw_28 = ecxw;
  recoptrrg6 = NULL;
  x16 wordrg26 = SKW_GET_TILE_RECORD_LINK(ulrshift(eaxprec->u6.w, con(0x5), con(0xb)), urshift(eaxprec->u6.w, con(0xb)));
  for (;; wordrg26 = SKW_GET_NEXT_RECORD_LINK(wordrg26))
  {
    if (wordrg26 == con(0xfffffffe))
      break;
    x16 wordrg121 = urshift(wordrg26 & con(0x3c00), con(0xa));
    if (wordrg121 > con(0x3))
      break;
    if (wordrg121 == con(0x3) && (recoptrrg6 = SKW_GET_ADDRESS_OF_RECORD(wordrg26), (recoptrrg6->u2.w & con(0x7f)) == con(0x30)))
      break;
  }
  x16 wordrg155 = urshift(recoptrrg6->u2.w, con(0x7));
  x16 vw_40 = wordrg155; // TODO: more often used as loopvariable - dependencies?!?
  x16 vw_3c = wordrg155;
  vb_4c = CUTX8(urshift(eaxprec->u2.w, con(0x7)));
  if (((eaxprec->u4.b.b0 & con(0x1)) == con(0x0)) || ((eaxprec->u4.b.b0 & con(0x4)) != con(0x0)))
  {
    x16 wordrg172 = 3 * ulrshift(eaxprec->u4.w, con(0x5), con(0xc)) + con(0x10);
    ps_14 = UPCAST(s_x40bytes, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0x40)));
    if (SKW_QUERY_CREATURES_ITEM_MASK(vb_4c, ps_14, false, CUTX8(wordrg172)))
    {
      wptrrg7 = UPCAST(x16, SKW_QUERY_GDAT_ENTRY_DATA_BUFF(con(0x9), con(0x8), con(0x13), vb_50));
      if (wptrrg7 != NULL)
      {
        vw_44 = (SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(con(0x9), con(0x8), con(0x13), vb_50) & con(0xffff)) / 2;
        for (vw_40 = con(0x0); vw_44 > vw_40; vw_40++)
          if (vw_3c == (wptrrg7[vw_40] & con(0x1ff)))
            break;
      }
      for(;;)
      {
        x16 wordrg21;
        do
        {
          x16 wordrg45;
          if (((eaxprec->u4.b.b0 & con(0x4)) == con(0x0)) && (ddata.v1e12d8 >= con(0x0)))
            wordrg45 = con(0xffffffff);
          else
            wordrg45 = con(0x1);
          vw_40 += wordrg45;
          if (wptrrg7 == NULL)
          {
            vw_40 &= con(0x1ff);
            vw_3c = vw_40;
          }
          else
          {
            if (vw_40 >= con(0x0))
            {
              if (vw_40 >= vw_44)
                vw_40 = con(0x0);
            }
            else
              vw_40 = vw_44 - con(0x1);
            vw_3c = wptrrg7[vw_40] & con(0x1ff);
          }
          wordrg21 = vw_3c;
        } while (((con(0x1) << (CUTX8(vw_3c) & con(0x7))) & ps_14->barr_00[wordrg21 / 8]) == con(0x0));
        if ((DM2_QUERY_GDAT_ENTRY_DATA_INDEX(SKW_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(wordrg21), con(0xb), con(0x0), SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(wordrg21)) & con(0x1000)) == con(0x0))
          break;
        if (SKW_IS_DISTINCTIVE_ITEM_ON_ACTUATOR(eaxprec, true, vw_3c))
          break;
        if (SKW_IS_DISTINCTIVE_ITEM_ON_ACTUATOR(recoptrrg6, false, vw_3c))
          break;
        if (SKW_2fcf_16ff(vw_3c))
          break;
      }
      eaxprec->u4.b.b0 &= con(0xfffffffb);
    }
    eaxprec->u4.b.b0 |= con(0x1);
    recoptrrg6->u4.b.b0 |= con(0x1);
    recoptrrg6->u2.w = (recoptrrg6->u2.w & con(0xffff007f)) | ((vw_3c & con(0x1ff)) << con(0x7));
    DM2_DEALLOC_LOBIGPOOL(con(0x40));
  }
  x8 vb_10 = vb_50;
  x16 wordrg2 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xc), con(0xfd), vb_50);
  vw_34 = wordrg2;
  SKW_QUERY_TEMP_PICST(con(0x0), argw1, signedword(CUTX8(urshift(wordrg2, con(0x8)))), argw0, signedword(CUTX8(vw_34)), vw_38, argw2, argw3, argw4, con(0xffff), SKW_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(vw_3c), SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(vw_3c) & con(0xff), con(0x0));
  SKW_DRAW_TEMP_PICST();
  if (!SKW_IS_DISTINCTIVE_ITEM_ON_ACTUATOR(eaxprec, true, vw_3c))
  {
    if (!SKW_IS_DISTINCTIVE_ITEM_ON_ACTUATOR(recoptrrg6, false, vw_3c))
    {
      bool skip = false; // added
      wptrrg7 = UPCAST(x16, SKW_QUERY_GDAT_ENTRY_DATA_BUFF(con(0x9), con(0x8), con(0x13), vb_10));
      if (wptrrg7 != NULL)
      {
        vw_44 = (SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(con(0x9), con(0x8), con(0x13), vb_10) & con(0xffff)) / 2;
        for (vw_40 = con(0x0); vw_44 > vw_40; vw_40++)
          if (vw_3c == (wptrrg7[vw_40] & con(0x1ff)))
            break;
        if (((wptrrg7[vw_40] & con(0x8000)) != con(0x0)))
        {
          x16 wordrg114 = SKW_ALLOC_NEW_DBITEM_DEEPLY(vw_3c);
          if (wordrg114 != con(0xffffffff))
          {
            SKW_APPEND_RECORD_TO(wordrg114, ulrshift(eaxprec->u6.w, con(0x5), con(0xb)), urshift(eaxprec->u6.w, con(0xb)), NULL);
            skip = true;
          }
        }
      }
      if (!skip)
      {
        SKW_QUERY_TEMP_PICST(con(0x0), argw1, unsignedword(CUTX8(urshift(vw_34, con(0x8)))), argw0, signedword(CUTX8(vw_34)), vw_38, argw2, argw3, argw4, con(0xffff), con(0x9), vb_50, con(0x11));
        SKW_DRAW_TEMP_PICST();
      }
    }
  }
  SKW_QUERY_TEMP_PICST(con(0x0), signedlong(argw1), con(0x0), signedlong(argw0), con(0x0), vw_38, argw2, argw3, argw4, con(0xffff), con(0x9), vb_50, con(0xf));
  SKW_DRAW_TEMP_PICST();
  if (vw_28 == con(0x3))
  {
    SKW_QUERY_TOPLEFT_OF_RECT(argw2, &vw_20, &vw_24);
    vw_34 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xc), con(0xf2), vb_50);
    vw_20 += signedword(CUTX8(urshift(vw_34, con(0x8))));
    vw_24 += signedword(CUTX8(vw_34));
    SKW_DRAW_TEXT_TO_BACKBUFF(vw_20, SKW_QUERY_GDAT_ITEM_NAME(SKW_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(vw_3c), SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(vw_3c)), vw_24);
  }
  if (vw_28 != con(0x3))
    return;
  x16 vw_1c = SKW_48ae_05ae(vw_3c, ulrshift(eaxprec->u4.w, con(0x5), con(0xc)), con(0x1), vb_4c, con(0x1), con(0xffffffff));
  x16 vw_48 = SKW_48ae_05ae(vw_3c, ulrshift(eaxprec->u4.w, con(0x5), con(0xc)), con(0x0), vb_4c, con(0x1), con(0xffffffff));
  for (x16 wordrg6 = con(0x0); wordrg6 < con(0x2); wordrg6++)
  {
    vw_34 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xc), ((wordrg6 == con(0x0)) ? 1 : 0) + con(0xfffffff0), vb_50);
    vw_20 = signedword(CUTX8(urshift(vw_34, con(0x8))));
    vw_24 = signedword(CUTX8(vw_34));
    x16 wordrg19;
    if (wordrg6 == con(0x0))
      wordrg19 = vw_48;
    else
      wordrg19 = vw_1c;
    if (wordrg19 != con(0x0))
    {
      SKW_48ae_0767(wordrg19, vga_08, &vw_2c, con(0x6));
      if (vw_2c > con(0x0))
      {
        vw_30 = SKW_3e74_5888();
        wordrg5 = vw_30;
        SKW_ALLOC_NEW_PICT(wordrg5, con(0x7), con(0x4), 8 * vw_2c);
        SKW_QUERY_TEMP_PICST(con(0x0), con(0x40), con(0x0), con(0x40), con(0x0), vw_38, con(0xffffffff), argw3, con(0xffff), con(0xffff), con(0x9), vb_50, con(0x10));
        gptrrg5 = SKW_3e74_5817(wordrg5);
        SKW_SET_ORIGIN_RECT(&rc_00, con(0x8), con(0x7));
        vw_40 = con(0x0);
      }
      while (vw_40 < vw_2c)
      {
//                                srcbmp                                  srcofs                  destbmp                        palette
        SKW_DRAW_DIALOGUE_PICT(ddata.v1e118e.s_00.pg_00, &rc_00, 8 * unsignedword(vga_08[vw_40]), gptrrg5, con(0x0), con(0xffff), NULL);
        rc_00.x += con(0x8);
        vw_40++;
      }
//                           srcbmp                                                                                   srcofs  destbmp                               palette
      SKW_DRAW_DIALOGUE_PICT(gptrrg5, SKW_QUERY_BLIT_RECT(gptrrg5, argw2 | con(0x8000), &vw_20, &rc_00, &vw_24, argw3), 0, ddata.bitmapptr, con(0x0), argw4, ddata.v1e118e.carr_3a);
      SKW_3e74_58a3(vw_30);
    }
  }
}

// TODO: init problem inside
x16 SKW_32cb_03a6(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2, bool argbool3)
{
  x16 wordrg5;
  ui16 uwordrg5;
  x16* wptrrg1;
  c_record* recoptrrg7;
  c_record* recoptrrg1;
  x16 vw_04;
  x16 vw_08;
  c_record* prec_10;
  c_record* prec_14;
  x16 vw_18;
  x16 vw_24;

  vw_08 = eaxw;
  vw_04 = edxw;
  vw_18 = urshift(argw1, con(0xe));
  x16 wordrg1;
  if (argw2 != con(0x2))
  {
    recoptrrg7 = NULL;
    wordrg1 = SKW_GET_TILE_RECORD_LINK(ecxw, argw0);
  }
  else
  {
    recoptrrg7 = SKW_GET_ADDRESS_OF_RECORD(SKW_GET_CREATURE_AT(ecxw, argw0));
    wordrg1 = recoptrrg7->u2.w;
  }
  vw_24 = wordrg1;
  if (vw_24 == con(0xfffffffe))
    return (argbool3) ? argw1 : con(0xffff);
  x16 wordrg6 = (argw2 != con(0x3)) ? con(0x0) : con(0x2);
  x16 vw_1c = con(0xffff);
  prec_14 = NULL;
  x16 vw_20 = 0; // initialization added - TODO: always initialized?
  do
  {
    x16 wordrg11 = urshift(vw_24 & con(0x3c00), con(0xa));
    if (wordrg11 >= con(0x5) && wordrg11 <= con(0xa))
    {
      x16 wordrg12 = vw_24;
      x16 vw_0c = unsignedlong(wordrg12);
      wordrg12 >>= con(0xe);
      if (wordrg12 == vw_18)
      {
        wordrg5 = con(0x0);
        prec_10 = NULL;
        if (SKW_IS_CONTAINER_CHEST(vw_0c))
        {
          if (SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0x14), con(0x1), con(0x48), SKW_QUERY_CLS2_FROM_RECORD(vw_0c)))
          {
            prec_10 = SKW_GET_ADDRESS_OF_RECORD(vw_0c);
            if ((prec_10->u4.b.b0 & con(0x1)) == con(0x0))
            {
              if (!argbool3)
                wordrg5 = con(0x2);
            }
            else
              wordrg5 = con(0x3);
          }
        }
        for(;;)
        {
          SKW_DRAW_ITEM(vw_24, con(0x0), con(0x0), ebxw, wordrg6, recoptrrg7, false, wordrg5, argw2 == con(0x3));
          if (SKW_32cb_00f1(vw_08, con(0xa), vw_04))
          {
            prec_14 = prec_10;
            vw_1c = vw_24;
            vw_20 = wordrg5;
            break;
          }
          if (wordrg5 == con(0x0))
            break;
          wordrg5 = con(0x0);
        }
        wordrg6++;
        if (argw2 != con(0x3))
          wordrg6 &= con(0xf);
        else
        {
          if (wordrg6 >= con(0xe))
            wordrg6 = con(0x2);
        }
      }
    }
    vw_24 = SKW_GET_NEXT_RECORD_LINK(vw_24);
  } while (vw_24 != con(0xfffffffe));
  recoptrrg1 = prec_14;
  prec_10 = recoptrrg1;
  if (recoptrrg1 == NULL)
    return (vw_1c == con(0xffffffff)) ? argw1 : vw_1c;
  uwordrg5 = vw_20; // TODO: pretty much paths with vw_20 uninitialized!
  if (uwordrg5 < UI16CAST(con(0x2)))
  {
    if (uwordrg5 != con(0x0))
      return con(0xffff);
    if (argbool3)
      return con(0xfffe);
    x8 byterg2 = recoptrrg1->u4.b.b0;
    if ((byterg2 & con(0x1)) == con(0x0))
      return vw_1c;
    recoptrrg1->u4.b.b0 = byterg2 & con(0xfffffffe);
    return con(0xffff);
  }
  if (uwordrg5 <= UI16CAST(con(0x2)))
  {
    recoptrrg1->u4.b.b0 |= con(0x1);
    return con(0xffff);
  }
  if (uwordrg5 != con(0x3))
    return con(0xffff);
  if (argbool3 && (SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(argw1, con(0x4)) & con(0x8000)) != con(0x0))
    return con(0xfffe);
  wordrg5 = prec_10->u2.w;
  x16 wordrg4 = con(0x0);
  while (wordrg5 != con(0xfffffffe))
  {
    wordrg4++;
    wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg5);
  }
  wptrrg1 = &prec_10->u2.w;
  if (!argbool3)
  {
    if (wordrg4 > con(0x0))
    {
      SKW_CUT_RECORD_FROM(wordrg5, con(0xffffffff), con(0x0), wptrrg1);
      ddata.v1e0ff6 = con(0x1);
      SKW_TAKE_OBJECT(wordrg5, true);
    }
    return con(0xffff);
  }
  if (wordrg4 >= con(0x8))
    return con(0xfffe);
  SKW_APPEND_RECORD_TO(argw1, con(0xffffffff), con(0x0), wptrrg1);
  return con(0xffff);
}

void SKW_2fcf_0b8b(x16 eaxw, x16 ebxw, x16 edxw)
{
  c_5bytes vs_00;
  x16 vw_08;

  vw_08 = ebxw;
  ddata.v1e027c = con(0xffffffff);
  ddata.v1e0234 = SKW_GET_TELEPORTER_DETAIL(&vs_00, edxw, eaxw);
  if (!ddata.v1e0234)
  {
    x16 wordrg3 = con(0x0);
    // M_3AFD7:
    for(;;)
    {
      if (wordrg3 >= con(0x4))
        break;
      x16 wordrg2 = table1d2804[wordrg3] + edxw;
      if (SKW_GET_TELEPORTER_DETAIL(&vs_00, wordrg2, table1d27fc[wordrg3] + eaxw))
      {
        ddata.v1e027c = unsignedword(vs_00.b_04);
        x16 wordrg402 = unsignedword(vs_00.b_01) + con(0x6) - unsignedword(vs_00.b_00);
        wordrg3 += wordrg402;
        wordrg3 += con(0x2);
        wordrg3 &= con(0x3);
        wordrg3 = table1d27fc[wordrg3];
        ddata.v1e0260 = unsignedword(vs_00.b_02) + wordrg3;
        ddata.v1e0262 = unsignedword(vs_00.b_03) + table1d2804[wordrg3];
        wordrg402 += ddata.v1e0258;
        wordrg402 &= con(0x3);
        ddata.v1e0276 = wordrg402;
        break;
      }
      wordrg3++;
    }
  }
  else
  {
    ddata.v1e027c = unsignedword(vs_00.b_04);
    ddata.v1e0260 = unsignedword(vs_00.b_02);
    ddata.v1e0262 = unsignedword(vs_00.b_03);
    ddata.v1e0276 = (unsignedword(vs_00.b_01) - unsignedword(vs_00.b_00) + ddata.v1e0258) & con(0x3);
  }
  ddata.v1d3248 = con(0xffffffff);
  SKW_CHANGE_CURRENT_MAP_TO(vw_08);
}

void SKW_REVIVE_PLAYER(x8 eaxb, x8 ebxb)
{
  x16* wptrrg5; // on an array
  c_hero* heroptrrg6;
  t_text t_00[0x80];
  t_text* tptrrg1;
  x8 vb_84;

  vb_84 = eaxb;
  heroptrrg6 = &ddata.hero[ddata.savegamew2];
  DM2_ZERO_MEMORY(DOWNCAST(c_hero, heroptrrg6), con(0x107));
  heroptrrg6->b101 = vb_84;
  heroptrrg6->b20[con(0x1)] = con(0xffffffff);
  heroptrrg6->b20[con(0x0)] = con(0xffffffff);
  heroptrrg6->w2e = con(0xffffffff);
  heroptrrg6->b1c = ebxb;
  x16 vw_80_6;
  for (vw_80_6 = con(0x0); ; vw_80_6++)
    if ((SKW_GET_PLAYER_AT_POSITION((vw_80_6 + ddata.v1e0258) & con(0x3))) == con(0xffffffff))
      break;
  heroptrrg6->b1d = (CUTX8(vw_80_6) + CUTX8(ddata.v1e0258)) & con(0x3);
  heroptrrg6->t22[0x6] = CUTX8(ddata.v1e0258);
  for (x16 vw_80_5 = con(0x0); vw_80_5 < con(0x1e); vw_80_5++)
    heroptrrg6->warrc3[vw_80_5] = con(0xffffffff);
//  tptrrg1 = SKW_QUERY_GDAT_TEXT(con(0x16), con(0x18), t_00, vb_84);
DEBUG_DUMP_ULP();
  tptrrg1 = SKW_QUERY_GDAT_TEXT(con(GDAT_CATEGORY_CHAMPIONS), con(0x18), t_00, vb_84);
  x16 vw_80_4 = con(0x0);
  x8 byterg3;
  for(;;)
  {
    byterg3 = *tptrrg1++;
    if (byterg3 == con(0x20) || byterg3 == con(0x0))
      break;
    x16 wordrg116 = vw_80_4;
    if (wordrg116 >= con(0x7))
      break;
    vw_80_4 = wordrg116 + con(0x1);
    heroptrrg6->t00[wordrg116] = byterg3;
  }
  x16 vw_80_0 = con(0x0);
  if (byterg3 != con(0x0))
  {
    x16 wordrg2;
    do
    {
      x8 byterg31 = *tptrrg1++;
      if (byterg31 == con(0x0))
        break;
      x16 wordrg11 = vw_80_0;
      wordrg2 = wordrg11 + con(0x1);
      vw_80_0 = wordrg2;
      heroptrrg6->t00[con(0x8) + wordrg11] = byterg31;
    } while (wordrg2 != con(0x13));
  }
  //wptrrg5 = UPCAST(x16, SKW_QUERY_GDAT_ENTRY_DATA_PTR(con(0x16), con(0x8), con(0x0), vb_84));
  wptrrg5 = UPCAST(x16, SKW_QUERY_GDAT_ENTRY_DATA_PTR(con(GDAT_CATEGORY_CHAMPIONS), con(dtRaw8), con(0x0), vb_84));
  // SPX
  if ((ui32)wptrrg5 >= 0x80000000)
	  return;
  heroptrrg6->w38 = wptrrg5[con(0x0)];
  heroptrrg6->w36 = wptrrg5[con(0x0)];
  heroptrrg6->u3c.w = wptrrg5[con(0x1)];
  heroptrrg6->w3a = wptrrg5[con(0x1)];
  heroptrrg6->w3e = heroptrrg6->w40 = wptrrg5[con(0x2)];
  for (x16 vw_80_1 = con(0x0); vw_80_1 <= con(0x6); vw_80_1++)
  {
    x8 byterg11 = CUTX8(DM2_MAX(con(0x1e), wptrrg5[vw_80_1 + con(0x3)]));
    heroptrrg6->b4a[2 * vw_80_1 + con(0x1)] = byterg11;
    heroptrrg6->b4a[2 * vw_80_1] = byterg11;
  }
  for (x16 vw_80_2 = con(0x4); vw_80_2 <= con(0x13); vw_80_2++)
  {
    x32 longrg3;
    x16 wordrg3 = wptrrg5[vw_80_2 + con(0x6)];
    if (wordrg3 == con(0x0))
      longrg3 = con(0x0);
    else
      longrg3 = con(0x40) << CUTX8(wordrg3);
    heroptrrg6->l5f[vw_80_2] = longrg3;
  }
  for (x16 vw_80_3 = con(0x0); vw_80_3 <= con(0x3); vw_80_3++)
  {
    x32 longrg2 = con(0x0);
    x16 wordrg31 = 4 * (vw_80_3 + con(0x1));
    x16 wordrg19 = con(0x0);
    x16 wordrg41;
    while (wordrg41 = wordrg19, wordrg41 < con(0x4))
    {
      wordrg41 += wordrg31;
      longrg2 += heroptrrg6->l5f[wordrg41];
      wordrg19++;
    }
    heroptrrg6->l5f[vw_80_3] = longrg2;
  }
  heroptrrg6->w44 = unsignedword(CUTX8(SKW_RAND())) + con(0x5dc);
  heroptrrg6->w46 = unsignedword(CUTX8(SKW_RAND())) + con(0x5dc);
}

bool SKW_2759_0e93(x16 eaxw, x16 ebxw, x16 edxw)
{
  x16 wordrg5;

  wordrg5 = eaxw;
  bool boolrg7 = false;
  if (edxw == con(0xffffffff))
    return false;
  if (SKW_IS_ITEM_HAND_ACTIVABLE(wordrg5, con(0xffffffff), edxw))
  {
    wordrg5 = con(0x0);
    // M_30A4E:
    for (;; wordrg5++)
    {
      if (wordrg5 >= ddata.v1e0b7a)
        break;
      if (SKW_QUERY_CMDSTR_ENTRY(ddata.v1e0b40[wordrg5].b0, ddata.v1e0b40[wordrg5].b2, con(0x2), ddata.v1e0b40[wordrg5].b1) == ebxw)
      {
        boolrg7 = true;
        break;
      }
    }
  }
  wordrg5 = ddata.v1e0b74;
  if (wordrg5 == con(0x0))
    return boolrg7;
  if (ddata.v1e0b76 != con(0x0))
    if (ddata.v1e0b76 != con(0x1))
      return boolrg7;
  x16 wordrg2 = ddata.v1e0b76;
  wordrg5--;
  x16 wordrg402 = ddata.hero[wordrg5].warrc3[wordrg2];
  SKW_IS_ITEM_HAND_ACTIVABLE(wordrg5, wordrg2, wordrg402);
  return boolrg7;
}

void SKW_24a5_1798(x16 eaxw)
{
  x16 wordrg5;
  c_hero* heroptrrg1;

  bool skip = false; // added
  wordrg5 = eaxw;
  bool boolrg6 = false;
  ddata.v1d66fc = con(0x0);
  ddata.v1d66fe = con(0xffffffff);
  if (eaxw < con(0x4) && ddata.hero[eaxw].w36 == con(0x0))
    return;
  if (ddata.v1e0224 || ddata.v1e03a8 || ddata.v1e0220 || ddata.v1e0228)
    return;
  ddata.v1e0488 = true;
  x16 wordrg3 = ddata.v1e0976;
  if ((wordrg5 + con(0x1)) == wordrg3)
    wordrg5 = con(0x4);
  if (wordrg3 == con(0x0) || wordrg5 == con(0x4))
  {
    boolrg6 = true;
    SKW_FIRE_HIDE_MOUSE_CURSOR();
  }
  if (wordrg3 != con(0x0))
  {
    ddata.v1e0976 = con(0x0);
    heroptrrg1 = &ddata.hero[wordrg3 - con(0x1)];
    if (heroptrrg1->w36 != con(0x0))
      if (ddata.v1e0288 == con(0x0))
      {
        heroptrrg1->u32.b.b1 |= con(0x40);
        SKW_REFRESH_PLAYER_STAT_DISP(wordrg3 - con(0x1));
      }
    x16 wordrg2 = con(0x0);
    while (wordrg2 < con(0x4))
    {
      x16 wordrg402 = 4 * wordrg3 + wordrg2;
      ddata.v1e0ffa[wordrg402 - con(0x2)] -= ddata.v1e096c[wordrg2]; // EVIL offset shifted by 2, was 1e0ff8
      wordrg2++;
    }
    if (!ddata.v1e0238)
    {
      if (wordrg5 == con(0x4))
      {
        if (ddata.v1d6727)
          SKW_29ee_000f();
        if (boolrg6)
          SKW_FIRE_SHOW_MOUSE_CURSOR();
        SKW_INIT_BACKBUFF();
      }
      else
        skip = true;
    }
    else if (boolrg6)
    {
      SKW_443c_040e();
      SKW_FIRE_SHOW_MOUSE_CURSOR();
    }
    if (!skip)
    {
      R_107B0();
      return;
    }
  }
  ddata.v1e0976 = wordrg5 + con(0x1);
  if (wordrg3 == con(0x0))
    SKW_FIRE_FILL_HALFTONE_RECTI(con(0x9), con(0x0));
  DM2_ZERO_MEMORY(ddata.v1e096c, con(0x4));
  ddata.hero[wordrg5].u32.b.b1 |= con(0xfffffffc);
  ddata.v1e100c = con(0x0);
  SKW_REFRESH_PLAYER_STAT_DISP(wordrg5);
  if (boolrg6)
    SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_1031_0541(con(0x8));
}

x16 SKW_RECALC_LIGHT_LEVEL(void)
{
  x16 wordrg5;
  x16 vwa_00[0x9];

  if ((ddata.v1e03c8[ddata.v1e0266].wc & con(0xf000)) != con(0x0))
  {
    DM2_ZERO_MEMORY(DOWNCAST(x16, vwa_00), con(0x12));
    wordrg5 = con(0x0);
    if ((SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(ddata.savegamewpc.w0, con(0x0)) & con(0x10)) != con(0x0))
    {
      vwa_00[0x0] = SKW_ADD_ITEM_CHARGE(ddata.savegamewpc.w0, con(0x0));
      wordrg5 = con(0x1);
    }
    x16 wordrg6 = con(0x0);
    while (wordrg6 < ddata.savegamew2)
    {
      x16 wordrg3 = con(0x0);
      while (wordrg3 <= con(0x1))
      {
        x16 wordrg12 = ddata.hero[wordrg6].warrc3[wordrg3];
        if ((SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(wordrg12, con(0x0)) & con(0x10)) != con(0x0))
        {
          vwa_00[wordrg5] = SKW_ADD_ITEM_CHARGE(wordrg12, con(0x0));
          wordrg5++;
        }
        wordrg3++;
      }
      wordrg6++;
    }
    x16 wordrg61 = con(0x0);
    while (wordrg61 < (wordrg5 - con(0x1)))
    {
      x16 wordrg421 = vwa_00[wordrg61];
      if (wordrg421 > vwa_00[wordrg61 + con(0x1)])
      {
        vwa_00[wordrg61] = vwa_00[wordrg61 + con(0x1)];
        vwa_00[wordrg61] = wordrg421;
      }
      wordrg61++;
    }
    x16 wordrg401 = con(0x6);
    x16 vw_14 = con(0x0);
    x16 wordrg62 = con(0x0);
    while (wordrg62 < wordrg5)
    {
      vw_14 = CUTX16(signedlong(vw_14) + ((signedlong(table1d6702[vwa_00[wordrg62]]) << CUTX8(wordrg401)) >> con(0x6)));
      wordrg401--;
      wordrg401 = DM2_MAX(con(0x0), wordrg401);
      wordrg62++;
    }
    wordrg5 = vw_14 + ddata.v1e0974;
    vw_14 = wordrg5 + ddata.savegames1.w0;
    vw_14 += DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x8), con(0xb), con(0x67), ddata.v1d6c02);
    if (ddata.v1e147f != con(0x0))
      vw_14 += signedword(table1d6712[DM2_BETWEEN_VALUE(con(0x0), con(0x5), ddata.v1e1480 + ddata.v1e1476)]);
    ddata.v1e0286 = con(0x0);
    while (ddata.v1e0286 < con(0x5))
    {
      if (table1d6712[ddata.v1e0286] < vw_14)
        break;
      ddata.v1e0286++;
    }
    x16 wordrg13 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x8), con(0xb), con(0x68), ddata.v1d6c02);
    if (wordrg13 > ddata.v1e0286)
      ddata.v1e0286 = wordrg13;
    if (ddata.v1e147f != con(0x0) && !ddata.v1e024c)
      ddata.v1e0286 = con(0x0);
  }
  else
    ddata.v1e0286 = con(0x1);
  ddata.v1e0286 -= (ddata.v1e0978 > con(0xc)) ? 1 : 0;
  ddata.v1e0286 = DM2_BETWEEN_VALUE(con(0x0), con(0x5), ddata.v1e0286);
  return ddata.v1e0286;
}

bool SKW_READ_DUNGEON_STRUCTURE(bool eaxbool)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("READ_DUNGEON_STRUCTURE");
  x16 wordrg5;
  ui16 uwordrg5;
  x8* bptrrg21; // looks like a buffer
  x8* bptrrg22; // casted when given to bppptrrg4
  x8*** bppptrrg4; // sort of root pointer
  x8* bptrrg7;
  x8* bptrrg1;
  u_mbuf vs_04;
  x8* pb_0c;

  if (!SKW_SKLOAD_READ(DOWNCAST(x8, vs_04.barr_00), con(0x8)) || vs_04.w_00 == con(0xffff8104) || !DM2_FILE_SEEK(signedlong(ddata.filehandle1), ddata.longsavegamel1))
    return false;
  if (ddata.v1d652d)
    ddata.savegamep4 = UPCAST(s_sgwords, DM2_ALLOC_FREEPOOL_MEMORY(con(0x2c)));
  if (!SKW_SKLOAD_READ(DOWNCAST(s_sgwords, ddata.savegamep4), con(0x2c)))
    return false;
  wordrg5 = ddata.savegamep4->w0[con(0x4)];
  ddata.v1e0270 = wordrg5 & con(0x1f);
  wordrg5 >>= con(0x5);
  ddata.v1e0272 = wordrg5 & con(0x1f);
  ddata.v1e0258 = (wordrg5 >> con(0x5)) & con(0x3);
  ddata.v1e0266 = con(0x0);
  x16 vw_10 = unsignedword(CUTX8(ddata.savegamep4->w0[con(0x2)]));
  if (ddata.v1d652d)
    ddata.v1e03c8 = UPCAST(s_size10, DM2_ALLOC_FREEPOOL_MEMORY(signedlong(vw_10) << con(0x4)));
  if (!SKW_SKLOAD_READ(DOWNCAST(s_size10, ddata.v1e03c8), vw_10 << con(0x4)))
    return false;
  if (ddata.v1d652d)
    ddata.v1e03e4 = UPCAST(x16, DM2_ALLOC_FREEPOOL_MEMORY(2 * signedlong(vw_10)));
  wordrg5 = con(0x0);
  x16 wordrg6 = con(0x0);
  while (wordrg5 < vw_10)
  {
    ddata.v1e03e4[wordrg5] = wordrg6;
    wordrg6 += ulrshift(ddata.v1e03c8[wordrg5].w8, con(0x5), con(0xb)) + con(0x1);
    wordrg5++;
  }
  ddata.v1e0408 = wordrg6;
  wordrg5 = ddata.savegamep4->w0[con(0x5)];
  if (eaxbool)
    ddata.savegamep4->w0[con(0x5)] = wordrg5 + con(0x12c);
  if (ddata.v1d652d)
  {
    ddata.v1e03d8 = UPCAST(x16, DM2_ALLOC_FREEPOOL_MEMORY(2 * unsignedlong(wordrg6)));
    ddata.v1e038c = UPCAST(x16, DM2_ALLOC_FREEPOOL_MEMORY(2 * unsignedlong(ddata.savegamep4->w0[5])));
    ddata.v1e03d0 = UPCAST(x16, DM2_ALLOC_FREEPOOL_MEMORY(2 * unsignedlong(ddata.savegamep4->w0[3])));
  }
  if (!SKW_SKLOAD_READ(DOWNCAST(x16, ddata.v1e03d8), 2 * wordrg6))
    return false;
  if (!SKW_SKLOAD_READ(DOWNCAST(x16, ddata.v1e038c), 2 * wordrg5))
    return false;
  if (eaxbool)
  {
    x16 wordrg35 = con(0x0);
    while (wordrg35 < con(0x12c))
    {
      ddata.v1e038c[wordrg5++] = con(0xffffffff);
      wordrg35++;
    }
  }
  if (!SKW_SKLOAD_READ(DOWNCAST(x16, ddata.v1e03d0), 2 * ddata.savegamep4->w0[con(0x3)]))
    return false;
  if (!ddata.boolsavegamel2)
    ddata.v1e142c = con(0x32);
  wordrg5 = con(0x0);
  while (wordrg5 < con(0x10))
  {
    x16 wordrg34 = ddata.savegamep4->w0[wordrg5 + con(0x6)];
SPX_DEBUG_MESSAGE_INFO("#Cat %02d => %04d items\n", wordrg5, wordrg34);
    if (eaxbool)
    {
      x16 wordrg408 = unsignedword(table1d281c[wordrg5]) + wordrg34;
      x16 wordrg14;
      if (wordrg5 != con(0xf))
        wordrg14 = con(0x400);
      else
        wordrg14 = con(0x300);
      ddata.savegamep4->w0[wordrg5 + con(0x6)] = DM2_MIN(wordrg14, wordrg408);
    }
    x16 wordrg61 = unsignedword(table1d280c[wordrg5]);
    if (ddata.v1d652d)
      ddata.savegameparr5[wordrg5] = UPCAST(c_record, DM2_ALLOC_FREEPOOL_MEMORY(unsignedlong(wordrg61) * unsignedlong(ddata.savegamep4->w0[wordrg5 + con(0x6)])));
    bptrrg21 = DOWNCAST(c_record, ddata.savegameparr5[wordrg5]);
    if (!SKW_SKLOAD_READ(bptrrg21, wordrg61 * wordrg34))
      return false;
    if (!ddata.boolsavegamel2)
      if ((wordrg5 == con(0x4)) || (wordrg5 >= con(0xe)))
        ddata.v1e142c += ddata.savegamep4->w0[wordrg5 + con(0x6)];
    if (eaxbool)
    {
      wordrg61 = urshift(wordrg61, con(0x1));
      bptrrg21 += 2 * wordrg61 * wordrg34;
      x32 longrg33 = unsignedlong(table1d281c[wordrg5]); // longrange necessary! BUGFIX 4/2014
      while (longrg33 != con(0x0))
      {
        *bptrrg21 = con(0xffffffff);
        *(bptrrg21 + con(0x1)) = con(0xffffffff);
        bptrrg21 += 2 * wordrg61;
        longrg33--;
      }
    }
    wordrg5++;
  }
  if (ddata.v1d652d)
    ddata.v1e03e0 = DM2_ALLOC_FREEPOOL_MEMORY(unsignedlong(ddata.savegamep4->w0[1]));
  if (!SKW_SKLOAD_READ(ddata.v1e03e0, ddata.savegamep4->w0[1]))
    return false;
  if (ddata.v1d652d)
  {
    x16 longrg3 = signedlong(vw_10);
    ddata.v1e03cc = UPCAST(x8**, DM2_ALLOC_FREEPOOL_MEMORY(4 * (unsignedlong(ddata.v1e0408) + longrg3)));
    bppptrrg4 = &ddata.v1e03cc[longrg3];
    x16 wordrg31 = con(0x0);
    while (wordrg31 < vw_10)
    {
      ddata.v1e03cc[wordrg31] = UPCAST(x8*, bppptrrg4);
      bptrrg22 = ddata.v1e03e0 + unsignedlong(CUTX16(ddata.v1e03c8[wordrg31].u0.u.l));
      *bppptrrg4++ = UPCAST(x8*, bptrrg22);
      uwordrg5 = con(0x1);
      while (uwordrg5 <= ulrshift(ddata.v1e03c8[wordrg31].w8, con(0x5), con(0xb)))
      {
        bptrrg22 += urshift(ddata.v1e03c8[wordrg31].w8, con(0xb)) + con(0x1);
        *bppptrrg4++ = UPCAST(x8*, bptrrg22);
        uwordrg5++;
      }
      wordrg31++;
    }
  }
  ddata.longv1e1438 = 65 * 21 * signedlong(DM2_BETWEEN_VALUE(con(0x0), con(0x17), DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x3), con(0xb), con(0x0), con(0x0))));
  SKW_3a15_0002();
  SKW_3df7_0037(!ddata.v1d652d);
  if (ddata.v1d652d)
  {
    ddata.v1e03f0 = DM2_ALLOC_FREEPOOL_MEMORY(con(0x3f));
    ddata.v1e03dc = DM2_ALLOC_FREEPOOL_MEMORY(con(0x7e));
  }
  bptrrg1 = ddata.v1e03dc;
  pb_0c = ddata.v1e03f0;
  x16 wordrg3 = con(0x0);
  *pb_0c++ = con(0x0);
  for (x16 vw_14 = con(0x0); vw_14 < con(0x3f); vw_14++)
  {
    x16 wordrg4 = con(0x0);
    for(;;)
    {
      wordrg5 = wordrg3 + con(0x1);
      bptrrg7 = bptrrg1 + con(0x1);
      if (wordrg4 >= vw_10)
        break;
      if ((ddata.v1e03c8[wordrg4].w8 & con(0x3f)) == vw_14)
      {
        *bptrrg1 = CUTX8(wordrg4);
        bptrrg1 = bptrrg7;
        wordrg3 = wordrg5;
      }
      wordrg4++;
    }
    *bptrrg1 = con(0xffffffff);
    bptrrg1 = bptrrg7;
    wordrg3 = wordrg5;
    *pb_0c++ = CUTX8(wordrg3);
  }
  if (eaxbool)
    SKW_ARRANGE_DUNGEON();
  if (ddata.v1d652d)
    SKW_2066_2498();
  ddata.v1d652d = false;
SPX_DEBUG_POP;
  return true;
}

x16 SKW_CREATE_MINION(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2, x8 argb3)
{
  x16 wordrg5;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;
  x8 vb_18;
  x16 vw_20;
  x16 vw_24;

  vw_0c = eaxw;
  vw_08 = edxw;
  vw_04 = ebxw;
  vw_20 = ddata.v1d3248;
  SKW_CHANGE_CURRENT_MAP_TO(argw1);
  x16 vw_1c = con(0xffff);
  bool vbool_14 = argw2 == con(0xffff);
  vw_24 = ecxw;
  wordrg5 = argw0;
  x16 wordrg6 = (argb3 >= con(0x0)) ? con(0x4) : con(0x0);
  if (SKW_RAND01() == con(0x0))
    vb_18 = con(0xffffffff);
  else
    vb_18 = con(0x1);
  // M_20A65:
  for(;;)
  {
    ui8 ubyterg1 = SKW_GET_TILE_VALUE(vw_24, wordrg5);
    ui8 ubyterg4 = ubyterg1 >> con(0x5);
    x16 wordrg48 = unsignedword(ubyterg4);
    if (wordrg48 == con(0x0) || wordrg48 == con(0x3))
    {
      ;
    }
    else if (wordrg48 != con(0x6) || (ubyterg1 & con(0x5)) != con(0x0))
    {
      if (wordrg48 != con(0x4) || (ubyterg1 & con(0x7)) == con(0x0) || (ubyterg1 & con(0x7)) == con(0x5))
      {
        if (SKW_GET_CREATURE_AT(vw_24, wordrg5) == con(0xffffffff))
        {
          x16 wordrg3 = vw_24;
          x16 wordrg2 = vw_04;
          x16 wordrg19;
          if (!vbool_14)
            wordrg19 = vw_0c;
          else
            wordrg19 = vw_0c | con(0x8000);
          vw_1c = SKW_ALLOC_NEW_CREATURE(wordrg19, wordrg2, wordrg3, vw_08, wordrg5);
          if (vw_1c != con(0xffffffff))
            break;
        }
      }
    }
    if (wordrg6 != con(0x0))
    {
      vw_24 = ecxw + table1d27fc[argb3];
      wordrg5 = argw0;
      wordrg5 += table1d2804[argb3];
      argb3 = (argb3 + vb_18) & con(0x3);
    }
    wordrg6--;
    if (wordrg6 == con(0xffffffff))
      break;
  }
  if (vbool_14)
  {
    if (vw_1c != con(0xffffffff))
    {
      x16 wordrg12 = SKW_GET_ADDRESS_OF_RECORD(vw_1c)->u2.w;
      for (;; wordrg12 = SKW_GET_NEXT_RECORD_LINK(wordrg12))
      {
        if (wordrg12 == con(0xfffffffe))
          break;
        if (urshift(wordrg12 & con(0x3c00), con(0xa)) == con(0xe))
          break;
      }
      SKW_GET_ADDRESS_OF_RECORD(wordrg12)->u2.w = argw2;
    }
  }
  SKW_CHANGE_CURRENT_MAP_TO(vw_20);
  return vw_1c;
}

// return is a byte, either 0x80 or 0x1
x8 SKW_1c9a_0006(x16 eaxw, x16 edxw)
{
  x8 byterg5;
  x8* ptrrg3; // bytebuffer
  s_x40bytes* ptrrg7;
  c_record* recoptrrg4;
  x16 vw_00;

  x16 wordrg6 = eaxw;
  x16 wordrg1 = edxw;
  byterg5 = CUTX8(edxw & con(0x80));
  wordrg1 &= con(0xffff003f);
  if (wordrg1 == con(0xffffffff) || wordrg1 == con(0xfffffffe))
    return con(0x0);
  if (wordrg1 == con(0x3f))
    return (byterg5 == con(0x0)) ? 1 : 0;
  if (wordrg1 == con(0x3e))
  {
    if (SKW_IS_CONTAINER_MONEYBOX(wordrg6) && (SKW_GET_ADDRESS_OF_RECORD(wordrg6)->u6.b.b1 & con(0x4)) == con(0x0))
      return (byterg5 == con(0x0)) ? 1 : 0;
    return byterg5;
  }
  if (wordrg1 == con(0x29))
  {
    if (SKW_IS_CONTAINER_MONEYBOX(wordrg6))
    {
      if ((SKW_GET_ADDRESS_OF_RECORD(wordrg6)->u6.b.b1 & con(0x4)) != con(0x0))
        return (byterg5 == con(0x0)) ? 1 : 0;
      return byterg5;
    }
    wordrg1 = con(0x7);
  }
  x8 byterg2 = ddata.s350.v1e054e->u4.b.b0;
  vw_00 = urshift(wordrg6 & con(0x3c00), con(0xa));
  if ((wordrg1 >= con(0x10) && wordrg1 < con(0x13)) || wordrg1 == con(0x28))
  {
    if (wordrg1 == con(0x28))
    {
      if (SKW_IS_CONTAINER_MONEYBOX(wordrg6) && (SKW_GET_ADDRESS_OF_RECORD(wordrg6)->u6.b.b1 & con(0x4)) == con(0x0))
        return (byterg5 == con(0x0)) ? 1 : 0;
      wordrg1 = con(0x10);
    }
    wordrg1 += ddata.s350.v1e054e->w8 * con(0x3);
  }
  x16 wordrg49 = wordrg1;
  ptrrg7 = UPCAST(s_x40bytes, ddata.s350.v1e07ee[wordrg49]);
  if (ptrrg7 == NULL)
  {
    ddata.s350.v1e0896--;
    ptrrg3 = DOWNCAST(x8, ddata.s350.v1e0898);
    ddata.s350.v1e07ee[wordrg49] = ptrrg3;
    ptrrg7 = UPCAST(s_x40bytes, ptrrg3);
    ptrrg3 += con(0x40);
    ddata.s350.v1e0898 = UPCAST(x8, ptrrg3);
    bool boolrg3 = (vw_00 == con(0x4));
    if (!SKW_QUERY_CREATURES_ITEM_MASK(byterg2, ptrrg7, boolrg3, CUTX8(wordrg49)))
      return con(0x0);
  }
  x16 wordrg100;
  if (vw_00 != con(0x4))
  {
    if (vw_00 != con(0x3))
    {
      if (vw_00 < con(0x5) || vw_00 > con(0xa))
        return con(0x0);
      wordrg100 = SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg6);
    }
    else
    {
      recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
      wordrg6 = recoptrrg4->u2.w & con(0x7f);
      if (wordrg6 < con(0x1a))
        wordrg100 = recoptrrg4->u2.w & con(0x7f);
      else
      {
        bool skip = false; // added
        if (wordrg6 > con(0x1a))
        {
          if (wordrg6 != con(0x1b))
          {
            wordrg100 = recoptrrg4->u2.w & con(0x7f);
            skip = true;
          }
          else if ((recoptrrg4->u2.w & con(0xffffff80)) == con(0x0))
            return con(0x0);
        }
        if (!skip)
          wordrg100 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0xe), SKW_GET_WALL_DECORATION_OF_ACTUATOR(recoptrrg4));
      }
    }
  }
  else
    wordrg100 = unsignedword(SKW_GET_ADDRESS_OF_RECORD(wordrg6)->u4.b.b0);
  x32 longrg4 = unsignedlong(ptrrg7->barr_00[wordrg100 / 8]);
  wordrg100 &= con(0x7);
  if (((con(0x1) << wordrg100) & longrg4) != con(0x0))
    return (byterg5 == con(0x0)) ? 1 : 0;
  return byterg5;
}

x16 SKW_CALC_CLOUD_DAMAGE(x16 eaxw, x16 edxw)
{
  c_record* recoptrrg7;
  c_record* recoptrrg5;
  bool flag;
  x8 vb_00;
  x8 vb_04;

  recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(eaxw);
  ui16 uwordrg2 = recoptrrg5->u2.w & con(0x7f);
  x16 wordrg6 = uwordrg2;
  if (wordrg6 >= con(0x8))
    return con(0x0);
  x8 byterg1 = table1d26c8[wordrg6];
  vb_00 = byterg1;
  if (byterg1 == con(0x0))
    return con(0x0);
  if (edxw != con(0xffffffff))
  {
    wordrg6 = urshift(edxw & con(0x3c00), con(0xa));
    if (wordrg6 != con(0x0) || (vb_04 = vb_00 & con(0x2), vb_04 == con(0x0)))
    {
      if (wordrg6 != con(0x4))
        return con(0x0);
      vb_04 = vb_00 & con(0x8);
      if (vb_04 == con(0x0))
        return con(0x0);
    }
    else
      recoptrrg7 = SKW_GET_ADDRESS_OF_RECORD(edxw);
  }
  else
  {
    byterg1 &= con(0x4);
    if (byterg1 == con(0x0))
      return con(0x0);
    wordrg6 = con(0xffffffff);
  }
  x16 wordrg400 = urshift(recoptrrg5->u2.w, con(0x8));
  if (wordrg6 == con(0x4) && (SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(edxw)->u18.b.b1 & con(0x10)) != con(0x0))
  {
    if (uwordrg2 != con(0x0))
      wordrg400 >>= con(0x2);
  }
  vb_04 = vb_00 & con(0x1);
  if (vb_04 != con(0x0))
  {
    wordrg400 = urshift(recoptrrg5->u2.w, con(0x9)) + con(0x1);
    wordrg400 += SKW_RAND16(wordrg400) + con(0x1);
  }
  if (uwordrg2 < con(0x2))
  {
    if (uwordrg2 != con(0x0))
      return wordrg400;
    if (wordrg6 != con(0x0))
      return wordrg400;
    flag = (recoptrrg7->u2.b.b0 & con(0xffffff80)) != con(0x0);
  }
  else if (uwordrg2 <= con(0x2))
  {
    wordrg400 >>= con(0x1);
    if (wordrg6 != con(0x0))
      return wordrg400;
    flag = (recoptrrg7->u2.b.b0 & con(0xffffff80)) != con(0x0);
  }
  else if (uwordrg2 <= con(0x3))
    flag = (SKW_QUERY_CREATURE_AI_SPEC_FLAGS(edxw) & con(0x20)) != con(0x0);
  else
  {
    if (uwordrg2 == con(0x7))
    {
      wordrg400 = DM2_MAX(con(0x1), SKW_RAND01() + DM2_MIN(urshift(recoptrrg5->u2.w, con(0xd)), con(0x4)));
      if (wordrg6 == con(0x4))
        wordrg400 = SKW_APPLY_CREATURE_POISON_RESISTANCE(edxw, wordrg400);
    }
    return wordrg400;
  }
  if (!flag)
    wordrg400 = con(0x0);
  return wordrg400;
}

void SKW_PROCESS_CLOUD(c_tim* eaxptim)
{
  c_record* recoptrrg5;
  c_tim vtim_00;
  x16 vw_10;

  vw_10 = unsignedword(eaxptim->u.b.b_06);
  x16 wordrg6 = unsignedword(eaxptim->u.b.b_07);
  recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(eaxptim->v.w_08);
  if ((((SKW_GET_TILE_VALUE(vw_10, wordrg6) & con(0xff)) >> con(0x5)) & con(0xff)) == con(0x4))
  {
    x16 wordrg11 = SKW_CALC_CLOUD_DAMAGE(eaxptim->v.w_08, SKW_GET_TILE_RECORD_LINK(vw_10, wordrg6));
    if (wordrg11 != con(0x0))
      SKW_ATTACK_DOOR(vw_10, wordrg11, true, wordrg6, con(0x0));
  }
  x16 wordrg43 = recoptrrg5->u2.w & con(0x7f);
  if (wordrg43 != con(0xe) && wordrg43 != con(0x0) && wordrg43 != con(0x2))
  {
    if (ddata.v1d3248 == ddata.v1e0282 && vw_10 == ddata.v1e0280)
      if (wordrg6 == ddata.v1e027e)
      {
        x16 wordrg12 = SKW_CALC_CLOUD_DAMAGE(eaxptim->v.w_08, con(0xffff));
        if (wordrg12 != con(0x0))
          SKW_ATTACK_PARTY(wordrg12, con(0x0), con(0x0));
      }
    x16 wordrg161 = SKW_GET_CREATURE_AT(vw_10, wordrg6);
    if (wordrg161 != con(0xffffffff))
      if (!SKW_1c9a_0958(wordrg161))
      {
        x16 wordrg15 = SKW_CALC_CLOUD_DAMAGE(eaxptim->v.w_08, wordrg161);
        if (wordrg15 != con(0x0))
          SKW_ATTACK_CREATURE(wordrg161, wordrg6, con(0x200d), vw_10, con(0x64), wordrg15);
      }
    x16 wordrg16 = recoptrrg5->u2.w & con(0x7f);
    if (wordrg16 < con(0x28))
    {
      if (wordrg16 == con(0x7))
      {
        if (UI16CAST(recoptrrg5->u2.w & con(0xffffff00)) >= UI16CAST(con(0x600)))
        {
          x16 wordrg47 = unsignedword(CUTX8(urshift(recoptrrg5->u2.w, con(0x8)) - con(0x3))) << con(0x8);
          recoptrrg5->u2.w = unsignedword(CUTX8(recoptrrg5->u2.w)) | wordrg47;
          SKW_COPY_TIMER(eaxptim, &vtim_00);
          vtim_00.un.l_00++;
          SKW_QUEUE_TIMER(&vtim_00);
          return;
        }
      }
    }
    else if (wordrg16 <= con(0x28))
    {
      if (UI16CAST(recoptrrg5->u2.w & con(0xffffff00)) > UI16CAST(con(0x3700)))
      {
        recoptrrg5->u2.w = unsignedword(CUTX8(recoptrrg5->u2.w)) | (unsignedword(CUTX8(urshift(recoptrrg5->u2.w, con(0x8)) - con(0x28))) << con(0x8));
        SKW_COPY_TIMER(eaxptim, &vtim_00);
        vtim_00.un.l_00++;
        SKW_QUEUE_TIMER(&vtim_00);
        return;
      }
    }
    else if (wordrg16 == con(0x64))
    {
      SKW_QUEUE_NOISE_GEN2(con(0xd), con(0x81), con(0xfe), CUTX8(wordrg16), vw_10, wordrg6, con(0x1), con(0x6c), con(0xc8));
      x16 wordrg167 = ((recoptrrg5->u2.w & con(0x7f)) + con(0x1)) & con(0x7f);
      recoptrrg5->u2.b.b0 &= con(0xffffff80);
      recoptrrg5->u2.w |= wordrg167;
      SKW_COPY_TIMER(eaxptim, &vtim_00);
      vtim_00.un.l_00++;
      SKW_QUEUE_TIMER(&vtim_00);
      return;
    }
  }
  if (ddata.v1d3248 == ddata.v1e0266 && (((recoptrrg5->u2.w & con(0x7f)) == con(0x2) || (recoptrrg5->u2.b.b0 & con(0x7f)) == con(0x0)) || (recoptrrg5->u2.w & con(0x7f)) == con(0x30)))
    ddata.v1e0390.b0 |= con(0x1);
  SKW_CUT_RECORD_FROM(eaxptim->v.w_08, vw_10, wordrg6, NULL);
  SKW_DEALLOC_RECORD(eaxptim->v.w_08);
}

// TODO: make edxw unsigned?
x16 SKW_CREATURE_ATTACKS_PLAYER(c_record* eaxprec, x16 edxw)
{
  x16 wordrg5;
  c_aispec* aiptrrg6;
  c_hero* heroptrrg7;
  x16 vw_08;
  x16 vw_0c;

  // SPX: moved variable here
  x16 wordrg2;
  x16 wordrg3;
  x16 vw_00;

  vw_0c = edxw;
  if (UI16CAST(edxw) >= UI16CAST(ddata.savegamew2))
    return con(0x0);
  heroptrrg7 = &ddata.hero[edxw]; // BUGFIX 4/2014
  if (heroptrrg7->w36 == con(0x0))
    return con(0x0);
  wordrg5 = con(0x0);
  x16 vw_04 = 2 * urshift(ddata.v1e03c0->wc, con(0xc));
  aiptrrg6 = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(eaxprec->u4.b.b0);
  vw_08 = unsignedword(aiptrrg6->u1c.b.b0);
  x16 wordrg22;
  if ((ddata.savegames1.b2 == con(0x0)) || ((aiptrrg6->u0.b.b1 & con(0x4)) != con(0x0)))
  {
    if ((aiptrrg6->u0.b.b1 & con(0x8)) == con(0x0))
      wordrg22 = 2 * ddata.v1e0286;
    else
      wordrg22 = con(0x0);
  }
  else
    wordrg22 = con(0x10);
  x16 wordrg46 = unsignedword(aiptrrg6->barr06[con(0x2)]);
  if (vw_08 != con(0x9))
  {
    if (vw_08 == con(0x8))
      wordrg46 = con(0xff);
  }
  else
    wordrg46 = DM2_MIN(con(0xff), 2 * wordrg46);
  if (!ddata.v1e0238 && wordrg46 != con(0xff))
  {
    x16 wordrg409 = wordrg22 + (CUTX16(SKW_RAND()) & con(0x1f)) + wordrg46 + vw_04 - con(0x10);
    if ((SKW_USE_DEXTERITY_ATTRIBUTE(vw_0c) >= wordrg409 && SKW_RAND02() != con(0x0)) || SKW_USE_LUCK_ATTRIBUTE(heroptrrg7, con(0x3c)))
      goto M_exit;
  }
   wordrg2 = CUTX16(SKW_RAND());
   wordrg3;
  if ((wordrg2 & con(0x70)) == con(0x0) || aiptrrg6->w1a == con(0x0))
    wordrg3 = (wordrg2 & con(0x1)) + con(0x1);
  else
  {
    ui16 uwordrg408 = aiptrrg6->w1a;
    wordrg2 &= con(0xf);
    if (wordrg2 == con(0x0))
      wordrg2 = con(0x1);
    x16 wordrg31 = con(0x0);
    while (wordrg31 < con(0x3))
    {
      if (wordrg2 <= (uwordrg408 & con(0xf)))
        break;
      uwordrg408 >>= con(0x4);
      wordrg31++;
    }
    wordrg3 = unsignedword(table1d26f8[wordrg31]);
  }
  wordrg5 = unsignedword(aiptrrg6->barr06[con(0x0)]);
  wordrg5 += DM2_MIN(unsignedword(aiptrrg6->barr06[con(0x0)]), CUTX16((SKW_RAND() & con(0xf)) + vw_04));
  vw_08 = unsignedlong(vw_08);
  if (vw_08 != con(0x8))
  {
    wordrg5 -= 2 * SKW_QUERY_PLAYER_SKILL_LV(vw_0c, true, con(0x7));
    if (wordrg5 <= con(0x1))
    {
      if (SKW_RAND01() != con(0x0))
      {
        wordrg5 = con(0x0);
        goto M_exit;
      }
      wordrg5 = SKW_RAND02() + con(0x2);
    }
  }
  wordrg5 >>= con(0x1);
  wordrg5 += SKW_RAND02() + SKW_RAND16(wordrg5);
  wordrg5 += SKW_RAND16(wordrg5);
  wordrg5 >>= con(0x2);
  wordrg5 += SKW_RAND02() + con(0x1);
  if (SKW_RAND01() != con(0x0))
    wordrg5 -= SKW_RAND16(wordrg5 / 2 + con(0x1)) - con(0x1);
   vw_00 = vw_0c;
  wordrg5 = SKW_WOUND_PLAYER(vw_0c, wordrg3, vw_08, wordrg5);
  if (wordrg5 != con(0x0))
  {
    SKW_QUEUE_NOISE_GEN2(con(0x16), con(0x82), con(0xfe), heroptrrg7->b101, ddata.v1e0270, ddata.v1e0272, con(0x2), con(0x69), con(0xc8));
    x16 wordrg21 = unsignedword(aiptrrg6->barr06[con(0x1)]);
    if (wordrg21 != con(0x0) && SKW_RAND01() != con(0x0))
    {
      x16 wordrg13 = SKW_2c1d_0f2a(heroptrrg7, wordrg21, con(0x4));
      if (wordrg13 != con(0x0))
        SKW_PROCESS_POISON(vw_00, wordrg13);
    }
  }
M_exit: // M_8AF0:
  if (ddata.v1e0238)
    SKW_RESUME_FROM_WAKE();
  return wordrg5;
}

x16 SKW_CALC_PLAYER_ATTACK_DAMAGE(c_hero* eaxphero, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2, x16 argw3, x16 argw4)
{
  x16 wordrg5;
  c_aispec* aiptrrg3;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;

  vw_0c = ebxw;
  vw_04 = ecxw;
  x16 wordrg2 = argw1;
  if (edxw >= UI16CAST(ddata.savegamew2))
    return con(0x0);
  if (eaxphero->w36 == con(0x0))
    return con(0x0);
  vw_08 = 2 * urshift(ddata.v1e03c0->wc, con(0xc));
  aiptrrg3 = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(SKW_GET_ADDRESS_OF_RECORD(vw_0c)->u4.b.b0);
  bool doit = false; // added
  x16 wordrg1;
  if (unsignedword(aiptrrg3->barr06[con(0x2)]) != con(0xffffffff))
  {
    x16 wordrg11 = SKW_GET_CREATURE_AT(vw_04, argw0);
    if (wordrg11 == con(0xffffffff) || !SKW_1c9a_0958(wordrg11))
    {
      x16 wordrg12 = wordrg2 & con(0x8000);
      wordrg2 &= con(0x7fff);
      if ((aiptrrg3->u0.b.b0 & con(0x20)) == con(0x0) || wordrg12 != con(0x0))
      {
        x16 wordrg406 = (2 * ddata.v1e0286 + (vw_08 + unsignedword(aiptrrg3->barr06[con(0x2)]) + (CUTX16(SKW_RAND()) & con(0x1f))) - con(0x10)) / 2;
        if (SKW_USE_DEXTERITY_ATTRIBUTE(edxw) > wordrg406 || SKW_RAND02() == con(0x0) || SKW_USE_LUCK_ATTRIBUTE(eaxphero, con(0x4b) - wordrg2))
        {
          x16 vw_10;
          wordrg5 = SKW_COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(edxw, argw3, ddata.v1e0b76);
          if (wordrg5 != con(0x0))
          {
            wordrg5 += SKW_RAND16(wordrg5 / 2 + con(0x1));
            wordrg5 = (argw2 * wordrg5) >> con(0x5);
            x16 wordrg405 = vw_08 + CUTX16(SKW_RAND() & con(0x1f)) + unsignedword(aiptrrg3->b2);
            wordrg5 += CUTX16(SKW_RAND() & con(0x1f));
            wordrg5 -= wordrg405;
            vw_10 = wordrg5;
            if (wordrg5 > con(0x1))
              doit = true;
          }
          if (!doit)
          {
            x16 wordrg112 = SKW_RAND02();
            if (wordrg112 != con(0x0))
            {
              wordrg5 = wordrg112 + con(0x1);
              vw_10 += CUTX16(SKW_RAND() & con(0xf));
              if (vw_10 > con(0x0) || SKW_RAND01() != con(0x0))
              {
                wordrg5 += SKW_RAND02();
                if (SKW_RAND02() == con(0x0))
                  wordrg5 += DM2_MAX(con(0x0), CUTX16(SKW_RAND()) & con(0xf) + vw_10);
              }
              doit = true;
            }
          }
          if (doit)
          {
            // M_8CF7:
            wordrg5 >>= con(0x1);
            wordrg5 += SKW_RAND02() + SKW_RAND16(wordrg5);
            wordrg5 += SKW_RAND16(wordrg5);
            wordrg5 >>= con(0x2);
            wordrg5 += SKW_RAND02() + con(0x1);
            if (SKW_QUERY_PLAYER_SKILL_LV(edxw, true, argw3) > (CUTX16(SKW_RAND() & con(0x3f))))
              wordrg5 += con(0xa);
            x16 wordrg404 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(eaxphero->warrc3[ddata.v1e0b76], con(0xd));
            if (wordrg404 != con(0x0))
              if (wordrg5 > (CUTX16(SKW_RAND() & con(0x1f))))
                wordrg5 += SKW_APPLY_CREATURE_POISON_RESISTANCE(vw_0c, wordrg404);
            SKW_ADJUST_SKILLS(edxw, ((((urshift(aiptrrg3->w16, con(0x8))) & con(0xf)) * wordrg5) >> con(0x4)) + con(0x3), argw3);
            wordrg1 = SKW_RAND02() + con(0x4);
          }
        }
      }
    }
  }
  if (!doit)
  {
    wordrg5 = con(0x0);
    wordrg1 = SKW_RAND01() + con(0x2);
  }
  SKW_ADJUST_STAMINA(edxw, wordrg1);
  if ((aiptrrg3->u18.b.b1 & con(0x10)) != con(0x0))
    if (argw4 != con(0x1))
      wordrg5 >>= CUTX8(SKW_RAND01() + con(0x1));
  SKW_ATTACK_CREATURE(vw_0c, argw0, con(0x6002), vw_04, con(0x5a), wordrg5);
  return wordrg5;
}

x16 SKW_0cee_1a46(s_8wordarray* eaxps, x16 ebxw, bool ecxbool, x16 edxw)
{
  c_record* recoptrrg3;
  c_record* recoptrrg4;
  bool flag;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_18;
  x8 vb_20;

  vw_18 = edxw;
  x16 vw_1c = con(0x0);
  // M_E5B8, co-label M_E96A
  for (;; vw_18 = SKW_GET_NEXT_RECORD_LINK(vw_18))
  {
    bool doit = false; // added
    x16 wordrg21 = vw_18;
    if (wordrg21 == con(0xfffffffe))
      return vw_18;
    vw_0c = urshift(wordrg21 & con(0x3c00), con(0xa));
    if (vw_0c > con(0x3))
      return vw_18;
    x16 vw_14 = urshift(wordrg21, con(0xe));
    if (((con(0x1) << CUTX8(vw_14)) & vw_1c) != con(0x0))
      continue;
    vw_10 = ((vw_14 - ebxw) & con(0x3)) + con(0x3);
    if (ecxbool && vw_10 != con(0x5))
      continue;
    bool boolrg6 = false;
    x16 wordrg2 = con(0x0);
    x32 longrg7 = ddata.longmallocx;
    x8 vb_04 = con(0x0);
    recoptrrg3 = SKW_GET_ADDRESS_OF_RECORD(vw_18);
    recoptrrg4 = recoptrrg3;
    bool pass = false; // added
    if (vw_0c != con(0x2))
    {
      // M_E77E:
      if (vw_0c != con(0x3))
        continue;
      vb_20 = SKW_GET_WALL_DECORATION_OF_ACTUATOR(recoptrrg3);
      if (vb_20 == con(0xffffffff))
        continue;
      x16 wordrg100 = recoptrrg3->u2.w & con(0x7f);
      if (wordrg100 < con(0x32))
      {
        if (wordrg100 < con(0x1a))
        {
          if (wordrg100 >= con(0x17))
          {
            if (wordrg100 <= con(0x17))
            {
              wordrg2 = ulrshift(recoptrrg3->u4.w, con(0xd), con(0xf));
              flag = wordrg2 == con(0x0);
              if (!flag)
                boolrg6 = true;
            }
            else if (wordrg100 == con(0x18))
              doit = true;
          }
        }
        else if (wordrg100 <= con(0x1a))
        {
          wordrg2 = ulrshift(recoptrrg3->u4.w, con(0xd), con(0xf));
          if (wordrg2 != con(0x0))
          {
            boolrg6 = true;
            vw_1c |= con(0x1) << CUTX8(vw_14);
          }
        }
        else if (wordrg100 <= con(0x1b))
        {
          wordrg2 = recoptrrg3->u4.w & con(0x1);
          if (wordrg2 == ulrshift(recoptrrg3->u4.w, con(0xd), con(0xf)))
            boolrg6 = true;
        }
        else if (wordrg100 == con(0x2c))
        {
          if ((recoptrrg3->u4.b.b0 & con(0x1)) != con(0x0))
          {
            vb_04 = CUTX8(urshift(recoptrrg3->u2.w, con(0x7)));
            boolrg6 = true;
          }
        }
      }
      else if (wordrg100 <= con(0x32))
      {
        if ((recoptrrg3->u4.b.b0 & con(0x1)) != con(0x0))
        {
          longrg7 = unsignedlong(urshift(recoptrrg3->u2.w, con(0x7)));
          boolrg6 = true;
        }
      }
      else if (wordrg100 < con(0x46))
      {
        if (wordrg100 < con(0x3f))
        {
          ;
        }
        else if (wordrg100 <= con(0x3f))
        {
          if (vw_10 != con(0x5))
            eaxps->warr_00[con(0x7)] = con(0xffffffff);
          else
            eaxps->warr_00[con(0x7)] = vw_18;
        }
        else if (wordrg100 == con(0x41))
          wordrg2 = urshift(recoptrrg3->u2.w, con(0x7));
      }
      else if (wordrg100 <= con(0x46))
        wordrg2 = recoptrrg3->u4.w & con(0x1);
      else if (wordrg100 < con(0x4a))
      {
        ;
      }
      else if (wordrg100 <= con(0x4a))
        doit = true;
      else if (wordrg100 == con(0x7e))
      {
        if ((recoptrrg3->u4.b.b0 & con(0x4)) != con(0x0))
        {
          if (vw_10 == con(0x5))
            eaxps->warr_00[con(0x7)] = urshift(recoptrrg3->u2.w, con(0x7));
          vw_1c |= con(0x1) << CUTX8(vw_14);
          boolrg6 = true;
        }
      }
    }
    else
    {
      x16 wordrg19 = ulrshift(recoptrrg3->u2.w, con(0xd), con(0xe));
      if (wordrg19 == con(0x0))
      {
        ;
      }
      else if (wordrg19 == con(0x1))
      {
        vb_20 = CUTX8(urshift(recoptrrg3->u2.w, con(0x3)));
        ui16 uwordrg3 = urshift(urshift(recoptrrg3->u2.w, con(0x3)), con(0x8)) & con(0x1f);
        if (uwordrg3 < con(0x5))
        {
          if (uwordrg3 < con(0x2))
          {
            if (uwordrg3 != con(0x0))
              continue;
          }
          else if (uwordrg3 <= con(0x2))
            boolrg6 = true;
          else if (uwordrg3 == con(0x4))
          {
            wordrg2 = recoptrrg4->u2.w & con(0x1);
            if (wordrg2 != con(0x0))
            {
              boolrg6 = true;
              vw_1c |= con(0x1) << CUTX8(vw_14);
            }
          }
          else
            continue;
          pass = true;
        }
        else if (uwordrg3 <= con(0x5))
        {
          flag = (recoptrrg4->u2.b.b0 & con(0x1)) == con(0x0);
          if (!flag)
            boolrg6 = true;
          pass = true;
        }
        else if (uwordrg3 < con(0x7))
          pass = true;
        else if (uwordrg3 <= con(0x8))
        {
          wordrg2 = recoptrrg4->u2.w & con(0x1);
          if (wordrg2 != con(0x0))
          {
            boolrg6 = true;
            vw_1c |= con(0x1) << CUTX8(vw_14);
          }
          pass = true;
        }
        else if (uwordrg3 < con(0xd))
          continue;
        else if (uwordrg3 <= con(0xd))
        {
          flag = (recoptrrg4->u2.b.b0 & con(0x1)) == con(0x0);
          if (!flag)
            boolrg6 = true;
          pass = true;
        }
        else if (uwordrg3 != con(0xe))
          continue;
      }
      else
        continue;
      if (!pass)
      {
        // M_E693:
        eaxps->warr_00[vw_10] = con(0x0);
        if (vw_10 == con(0x5))
        {
          if ((recoptrrg4->u2.b.b0 & con(0x1)) == con(0x0))
            eaxps->warr_00[con(0x7)] = con(0xffffffff);
          else
            eaxps->warr_00[con(0x7)] = vw_18;
        }
        vw_1c |= con(0x1) << CUTX8(vw_14);
        continue;
      }
    }
    // M_E938:
    if (doit)
      // M_E89D:
      wordrg2 = ulrshift(recoptrrg4->u4.w, con(0xd), con(0xf)) ^ (recoptrrg4->u4.w & con(0x1));
    if (boolrg6)
      wordrg2 = SKW_QUERY_ORNATE_ANIM_FRAME(con(0x9), longrg7, vb_04, vb_20);
    wordrg2 <<= con(0xa);
    wordrg2 |= unsignedword(vb_20);
    eaxps->warr_00[vw_10] = wordrg2;
  }
}

// was SKW_0cee_1dbe
void SKW_SUMMARIZE_STONE_ROOM(s_8wordarray* eaxps, x16 ebxw, x16 ecxw, x16 edxw)
{
  x8 byterg2h;
  x8 byterg1h;
  c_record* recoptrrg4;
  c_record* recoptrrg1;
  x8 vb_08;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_14;
  x16 vw_18;
  x16 vw_1c;
  x16 vw_24;
  x16 vw_2c;
  x16 vw_30;
  x8 vb_34;
  x8 vb_38;
  bool flag;

  vw_24 = edxw;
  vw_18 = ebxw;
  vw_1c = ecxw;
  eaxps->warr_00[con(0x3)] = con(0xff);
  eaxps->warr_00[con(0x4)] = con(0xff);
  eaxps->warr_00[con(0x5)] = con(0xff);
  eaxps->warr_00[con(0x6)] = con(0xff);
  eaxps->warr_00[con(0x7)] = con(0x0);
  x16 wordrg3 = vw_1c;
  x8 byterg2 = SKW_GET_TILE_VALUE(vw_18, wordrg3);
  vb_38 = byterg2;
  eaxps->warr_00[con(0x1)] = unsignedword(byterg2);
  x16 wordrg6 = SKW_GET_TILE_RECORD_LINK(vw_18, wordrg3);
  ui16 uwordrg1 = unsignedword(ubrshift(byterg2, con(0x5)));
  eaxps->warr_00[con(0x0)] = uwordrg1;
  if (uwordrg1 > con(0x6))
  {
    eaxps->warr_00[con(0x2)] = wordrg6;
    return;
  }
  x8 byterg1 = byterg2 & con(0x4);
  byterg2h = byterg2 & con(0x8);
  vw_0c = vw_24 & con(0x1);
  x16 ret;
  bool boolrg7;
  switch (uwordrg1)
  {
    case 0:
      if (UI16CAST(vw_24) <= UI16CAST(con(0x3)))
      {
        x16 wordrg10;
        byterg1h = byterg2 & con(0x1);
        x16 byterg44 = byterg2 & con(0x2);
        switch (vw_24)
        {
          case 0:
            vw_2c = unsignedword(byterg2h);
            vw_30 = unsignedword(byterg1);
            vw_10 = unsignedword(byterg44);
            wordrg10 = unsignedword(byterg1h);
          break;

          case 1:
            vw_2c = unsignedword(byterg1);
            vw_30 = unsignedword(byterg44);
            vw_10 = unsignedword(byterg1h);
            wordrg10 = unsignedword(byterg2h);
          break;

          case 2:
            vw_2c = unsignedword(byterg44);
            vw_30 = unsignedword(byterg1h);
            vw_10 = unsignedword(byterg2h);
            wordrg10 = unsignedword(byterg1);
          break;

          case 3:
            vw_2c = unsignedword(byterg1h);
            vw_30 = unsignedword(byterg2h);
            vw_10 = unsignedword(byterg1);
            wordrg10 = unsignedword(byterg44);
          break;

          default: throw(THROW_DMABORT);
        }
        vw_14 = wordrg10;
      }
      SKW_0cee_185a(eaxps, vw_30, vw_10, vw_2c, vw_14, vw_24, vw_18, vw_1c);
      boolrg7 = false;
      ret = 0;
    break;

    case 1:
      ret = 2;
    break;

    case 2:
      if (byterg2h == con(0x0))
      {
        eaxps->warr_00[con(0x0)] = con(0x1);
        ret = 2;
      }
      else
      {
        eaxps->warr_00[con(0x3)] = unsignedword(byterg1);
        ret = 3;
      }
    break;

    case 3:
      eaxps->warr_00[con(0x0)] = ((((unsignedlong(byterg2h) / 8) & con(0xff)) != signedlong(vw_0c)) ? 1 : 0) + con(0x12);
      eaxps->warr_00[con(0x3)] = unsignedword(byterg2 & con(0x4));
      ret = 1;
    break;

    case 4:
      eaxps->warr_00[con(0x3)] = unsignedword(byterg2 & con(0x7));
      eaxps->warr_00[con(0x4)] = SKW_GET_TILE_RECORD_LINK(vw_18, wordrg3);
      if (unsignedword(byterg2h / 8) != vw_0c)
      {
        eaxps->warr_00[con(0x0)] = con(0x11);
        vw_24 = (vw_24 + con(0x1)) & con(0x3);
        SKW_0cee_1a46(eaxps, vw_24, true, SKW_GET_TILE_RECORD_LINK(vw_18 + table1d27fc[vw_24], vw_1c + table1d2804[vw_24]));
      }
      else
        eaxps->warr_00[con(0x0)] = con(0x10);
      ret = 1;
    break;

    case 5:
      if (byterg2h == con(0x0) || byterg1 == con(0x0))
      {
        eaxps->warr_00[con(0x0)] = con(0x1);
        eaxps->warr_00[con(0x1)] &= con(0xfffff7ff); // mod
      }
      ret = 3;
    break;

    case 6:
      if (byterg1 != con(0x0))
      {
        vb_38 = con(0x1);
        eaxps->warr_00[con(0x0)] = con(0x1);
        ret = 2;
      }
      else
      {
        eaxps->warr_00[con(0x0)] = con(0x0);
        boolrg7 = true;
        ret = 0;
      }
    break;

    default: throw(THROW_DMABORT);
  }
  switch (ret)
  {
    case 0: // M_EB34:
      wordrg6 = SKW_0cee_1a46(eaxps, vw_24, false, wordrg6);
      if (boolrg7 && ddata.v1e0270 != vw_18 && ddata.v1e0272 != vw_1c)
      {
        eaxps->warr_00[con(0x2)] = con(0xfffffffe);
        return;
      }
      if (!boolrg7)
      {
        for (x16 vw_20 = con(0x0); vw_20 < con(0x4); vw_20++)
        {
          x16 wordrg17 = (vw_24 + vw_20) & con(0x3);
          if ((((SKW_GET_TILE_VALUE(vw_18 + table1d27fc[wordrg17], vw_1c + table1d2804[wordrg17]) & con(0xff)) >> con(0x5)) & con(0xff)) == con(0x4))
            eaxps->warr_00[vw_20 + con(0x3)] = con(0xff);
        }
      }
      eaxps->warr_00[con(0x2)] = wordrg6;
    return;

    case 1: // M_EF85:
      for(;;)
      {
        if (wordrg6 == con(0xfffffffe))
          break;
        if (urshift(wordrg6 & con(0x3c00), con(0xa)) > con(0x3))
          break;
        wordrg6 = SKW_GET_NEXT_RECORD_LINK(wordrg6);
      }
      eaxps->warr_00[con(0x2)] = wordrg6;
    return;

    case 2: // M_EC03:
      {
        x16 vw_20_1 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x8), con(0xb), con(0x6b), CUTX8(ulrshift(ddata.v1e03c0->we, con(0x8), con(0xc))));
        if (vw_20_1 != con(0x0))
        {
          if ((vw_20_1 & con(0x8000)) == con(0x0))
            eaxps->warr_00[con(0x5)] = unsignedword(CUTX8(vw_20_1));
          else
            eaxps->warr_00[con(0x5)] = unsignedword(CUTX8(vw_20_1)) | ((SKW_QUERY_ORNATE_ANIM_FRAME(con(0xa), ddata.longmallocx, con(0x0), CUTX8(vw_20_1)) * con(0xa)) << con(0x8));
        }
      }
    break;

    case 3: // M_EC97:
    break;
  }
  // M_EC97, co-label M_EE97, no exit except return here
  for (;; wordrg6 = SKW_GET_NEXT_RECORD_LINK(wordrg6))
  {
    if (wordrg6 == con(0xfffffffe))
    {
      eaxps->warr_00[con(0x2)] = wordrg6;
      return;
    }
    vw_24 = urshift(wordrg6 & con(0x3c00), con(0xa));
    x16 wordrg3 = vw_24;
    if (wordrg3 > con(0x3))
    {
      eaxps->warr_00[con(0x2)] = wordrg6;
      return;
    }
    bool boolrg2 = false;
    x16 vw_28 = con(0x0);
    x32 longrg7 = ddata.longmallocx;
    x8 vb_04 = boolrg2 ? 1 : 0;
    recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
    recoptrrg4 = recoptrrg1;
    if (wordrg3 != con(0x2))
    {
      if (wordrg3 != con(0x3))
        continue;
      if ((recoptrrg1->u2.w & con(0x7f)) == con(0x30))
        eaxps->warr_00[con(0x6)] = con(0x0);
      vb_34 = SKW_GET_FLOOR_DECORATION_OF_ACTUATOR(recoptrrg4);
      if (vb_34 == con(0xffffffff))
        continue;
      x16 wordrg14 = recoptrrg4->u2.w & con(0x7f);
      if (wordrg14 < con(0x2c))
      {
        if (wordrg14 == con(0x27))
        {
          if ((urshift(recoptrrg4->u2.w, con(0x7)) - con(0x1)) != ddata.v1d3248)
            continue;
          flag = (recoptrrg4->u4.b.b0 & con(0x1)) == con(0x0);
          if (!flag)
            boolrg2 = true;
        }
      }
      else if (wordrg14 <= con(0x2c))
      {
        if ((recoptrrg4->u4.b.b0 & con(0x1)) != con(0x0))
        {
          vb_04 = CUTX8(urshift(recoptrrg4->u2.w, con(0x7)));
          boolrg2 = true;
        }
      }
      else if (wordrg14 == con(0x32))
      {
        if ((recoptrrg4->u4.b.b0 & con(0x1)) != con(0x0))
        {
          longrg7 = unsignedlong(urshift(recoptrrg4->u2.w, con(0x7)));
          boolrg2 = true;
        }
      }
    }
    else
    {
      if ((recoptrrg4->u2.w & con(0x6)) != con(0x2))
        continue;
      x8 byterg19 = CUTX8(recoptrrg1->u2.w);
      x16 wordrg401 = urshift(recoptrrg1->u2.w, con(0x3));
      vb_08 = CUTX8(wordrg401);
      vb_34 = vb_08;
      ui16 uwordrg400 = urshift(wordrg401, con(0x8)) & con(0x1f);
      if (uwordrg400 < con(0xa))
      {
        if (uwordrg400 < con(0x2))
        {
          if (uwordrg400 != con(0x0))
            continue;
        }
        else if (uwordrg400 <= con(0x2))
          boolrg2 = true;
        else if (uwordrg400 == con(0x5))
        {
          flag = (byterg19 & con(0x1)) == con(0x0);
          if (!flag)
            boolrg2 = true;
        }
        else
          continue;
      }
      else if (uwordrg400 <= con(0xa))
      {
        ;
      }
      else if (uwordrg400 < con(0x10))
      {
        if (uwordrg400 != con(0xf))
          continue;
        vw_28 = con(0x1);
      }
      else if (uwordrg400 <= con(0x10))
      {
        ;
      }
      else if (uwordrg400 <= con(0x11))
        vw_28 = ((vb_38 & con(0x8)) != con(0x0)) ? 1 : 0;
      else if (uwordrg400 == con(0x14))
      {
        eaxps->warr_00[con(0x6)] = con(0x0);
        continue;
      }
      else
        continue;
    }
    // M_EE65:
    if (boolrg2)
      vw_28 = SKW_QUERY_ORNATE_ANIM_FRAME(con(0xa), longrg7, vb_04, vb_34);
    eaxps->warr_00[con(0x5)] = unsignedword(vb_34) | ((vw_28 * con(0xa)) << con(0x8));
  }
}

// was SKW_1031_156f
void SKW_ADJUST_UI_EVENT(c_event* eaxps)
{
  x16 wordrg5;

  ui16 uwordrg4 = eaxps->warr0[con(0x2)];
  x16 wordrg2;
  if (uwordrg4 < con(0x74) || uwordrg4 > con(0x7b))
  {
    if (uwordrg4 < con(0x5f) || uwordrg4 > con(0x62))
      return;
    wordrg5 = SKW_GET_PLAYER_AT_POSITION((uwordrg4 - con(0x5f) + ddata.v1e0258) & con(0x3));
    if (wordrg5 < con(0x0))
    {
      eaxps->warr0[con(0x2)] = con(0x0);
      return;
    }
    if (uwordrg4 > con(0x60))
      wordrg2 = eaxps->warr0[con(0x1)] - eaxps->r6.y;
    else
      wordrg2 = eaxps->r6.h + eaxps->r6.y - con(0x1) - eaxps->warr0[con(0x1)];
    x16 wordrg11;
    if (uwordrg4 != con(0x60) && uwordrg4 != con(0x61))
      wordrg11 = (eaxps->r6.x + eaxps->r6.w) - con(0x1) - eaxps->warr0[con(0x0)];
    else
      wordrg11 = eaxps->warr0[con(0x0)] - eaxps->r6.x;
    if (wordrg11 > wordrg2)
    {
      if (ddata.hero[wordrg5].b2a[2] != con(0x0))
        uwordrg4 = con(0x0);
    }
    else
      uwordrg4 -= con(0x4f);
    eaxps->warr0[con(0x2)] = uwordrg4;
    return;
  }
  uwordrg4 -= con(0x74);
  x8 byterg6 = ((uwordrg4 & con(0x1)) != con(0x0)) ? 1 : 0; // not bool!
  wordrg5 = SKW_GET_PLAYER_AT_POSITION((ddata.v1e0258 + uwordrg4 / 2) & con(0x3));
  if (wordrg5 < con(0x0))
  {
    eaxps->warr0[con(0x2)] = con(0x0);
    return;
  }
  if (ddata.hero[wordrg5].b2a[byterg6] != con(0x0))
  {
    eaxps->warr0[con(0x2)] = con(0x0);
    return;
  }
  if (SKW_IS_ITEM_HAND_ACTIVABLE(wordrg5, signedword(byterg6), ddata.hero[wordrg5].warrc3[byterg6]))
    eaxps->warr0[con(0x2)] = 2 * wordrg5 + con(0x74) + unsignedword(byterg6);
  else
    eaxps->warr0[con(0x2)] = con(0x0);
}

// was SKW_12b4_0300
// return used only when used recursive
// TODO: problem inside
bool SKW_PERFORM_MOVE(x16 eaxw)
{
  x16 wordrg5;
  c_hero* heroptrrg3;
  c_5bytes vs_04;
  x16 vw_10;
  x16 vw_14;
  x16 vw_18;
  x16 vw_1c;
  x16 vw_20;
  x16 vw_28;
  x16 vw_2c;
  x16 vw_30;
  x16 vw_34;

  x8 byterg1 = SKW_GET_TILE_VALUE(ddata.v1e0280, ddata.v1e027e);
  x16 vw_0c = unsignedword(byterg1);
  vw_18 = ((unsignedword(byterg1) >> con(0x5)) == con(0x3)) ? 1 : 0;
  wordrg5 = con(0x1);
  heroptrrg3 = ddata.hero;
  ui16 uwordrg22 = con(0x0);
  while (uwordrg22 < ddata.savegamew2)
  {
    if (heroptrrg3->w36 != con(0x0))
      wordrg5 = DM2_MAX(wordrg5, SKW_CALC_PLAYER_WALK_DELAY(uwordrg22));
    heroptrrg3++;
    uwordrg22++;
  }
  bool skip = false; // added
  if (wordrg5 > con(0x1) && ddata.v1e025c == con(0x0))
  {
    if (eaxw == con(0x3))
      skip = true;
    else
    {
      if (eaxw == con(0x5))
      {
        if (!ddata.v1e0234 && vw_18 == con(0x0))
          skip = true;
      }
      if (ddata.v1d4000 != con(0xffffffff)) // && !skip not necessary
        skip = true;
    }
  }
  if (skip)
  {
    ddata.v1e025c = wordrg5 / 2;
    ddata.v1e026a = ddata.v1e0270;
    ddata.v1e0268 = ddata.v1e0272;
    ddata.v1e0278 = ddata.v1e0258;
    ddata.v1e0256 = eaxw;
    if (ddata.v1e0256 != con(0x3))
      SKW_CALC_VECTOR_W_DIR(ddata.v1e0258, con(0x0), &ddata.v1e026a, con(0xffffffff), &ddata.v1e0268);
  }
  else
  {
    if (ddata.v1d4000 != con(0xffffffff))
    {
      if (ddata.v1e0258 == ddata.v1e0278)
      {
        if (ddata.v1e0266 == ddata.v1e0540)
        {
          if (ddata.v1e0270 == ddata.v1e0546)
          {
            if (ddata.v1e0272 == ddata.v1e0544)
            {
              if (SKW_GET_CREATURE_AT(ddata.v1e053c, ddata.v1e053a) == ddata.v1d4000)
              {
                ddata.v1d4000 = con(0xffffffff);
                vw_30 = (ddata.v1e0266);
                if (eaxw == con(0x3))
                  SKW_12b4_0d75(ddata.v1e053c, ddata.v1e0542, con(0xfe), ddata.v1e053a);
                if (SKW_PERFORM_MOVE(eaxw))
                {
                  SKW_PERFORM_TURN_SQUAD(ddata.v1e0538);
                  if (eaxw != con(0x3))
                  {
                    SKW_CHANGE_CURRENT_MAP_TO(vw_30);
                    SKW_12b4_0d75(ddata.v1e053c, ddata.v1e0542, con(0xfe), ddata.v1e053a);
                  }
                }
                SKW_CHANGE_CURRENT_MAP_TO(ddata.v1e0266);
              }
            }
          }
        }
      }
      ddata.v1d4000 = con(0xffffffff);
      if (!ddata.v1e03a8)
        ddata.v1d324a = con(0x6);
      return true;
    }
    vw_30 = ddata.v1d3248;
    vw_28 = eaxw - con(0x3);
    if (ddata.v1e0234 && vw_28 == con(0x2))
      SKW_CHANGE_CURRENT_MAP_TO(ddata.v1e027c);
    vw_14 = ddata.v1e0280;
    vw_20 = ddata.v1e027e;
    vw_34 = ddata.v1e0264;
    SKW_CALC_VECTOR_W_DIR(vw_34, table1d3ffc[vw_28], &vw_14, table1d3ff8[vw_28], &vw_20);
    x16 vw_24 = unsignedword(SKW_GET_TILE_VALUE(vw_14, vw_20));
    ddata.v1e0488 = true;
    heroptrrg3 = ddata.hero;
    ui16 uwordrg21 = con(0x0);
    while (uwordrg21 < UI16CAST(ddata.savegamew2))
    {
      if (heroptrrg3->w36 != con(0x0))
        SKW_ADJUST_STAMINA(uwordrg21, 3 * SKW_GET_PLAYER_WEIGHT(uwordrg21) / SKW_MAX_LOAD(heroptrrg3) + con(0x1));
      heroptrrg3++;
      uwordrg21++;
    }
    SKW_RESET_SQUAD_DIR();
    x16 wordrg7 = vw_24;
    i16 ret = 0; // added, default set
    x16 wordrg15 = SKW_12b4_0881(vw_28, vw_24, vw_14, vw_0c, vw_20, &vw_2c) - con(0x1);
    if (UI16CAST(wordrg15) <= con(0x5))
    {
      switch (wordrg15)
      {
        case 1:
          SKW_MOVE_RECORD_TO(con(0xffff), ddata.v1e0272, con(0xffffffff), ddata.v1e0270, con(0x0));
          ddata.v1e0270 = vw_14;
          ddata.v1e0272 = vw_20;
          vw_0c = vw_24;
        case 0: // case 1 running through
          SKW_12b4_00af(vw_0c & con(0x4));
          ret = -1;
        break;

        case 2:
          if (ddata.savegamew2 != con(0x0))
          {
            SKW_12b4_023f(vw_14, &vw_1c, &vw_10, vw_20, vw_34, vw_28);
            if (((wordrg7 >> con(0x5)) & con(0xff)) == con(0x4))
            {
              if ((CUTX8(vw_24) & con(0x7)) == con(0x4))
              {
                wordrg5 = con(0x0);
                if (vw_1c != con(0xffffffff))
                {
                  heroptrrg3 = &ddata.hero[vw_1c];
                  wordrg5 = CUTX16(SKW_RAND() & con(0xf));
                  x16 wordrg111 = SKW_GET_PLAYER_ABILITY(heroptrrg3, con(0x0), con(0x1)) + wordrg5;
                  wordrg5 = SKW_STAMINA_ADJUSTED_ATTR(heroptrrg3, wordrg111); // BUGFIX 4.7/2014 TODO: check again full wordrg5 behaviour
                }
                if (vw_10 != vw_1c && (vw_10 != con(0xffffffff)))
                {
                  heroptrrg3 = &ddata.hero[vw_10];
                  x16 wordrg112 = SKW_GET_PLAYER_ABILITY(heroptrrg3, con(0x0), con(0x1)) + CUTX16(SKW_RAND() & con(0xf));
                  wordrg5 += SKW_STAMINA_ADJUSTED_ATTR(heroptrrg3, wordrg112); // BUGFIX 4.8/2014 TODO: check again full wordrg5 behaviour
                }
                SKW_ATTACK_DOOR(vw_14, wordrg5, false, vw_20, con(0x0));
              }
            }
          }
        break;

        case 3:
          if (ddata.savegamew2 != con(0x0))
          {
            x16 wordrg71 = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(vw_2c) & con(0x1);
            if (wordrg71 != con(0x0) || SKW_GET_CREATURE_WEIGHT(vw_2c) == con(0xff))
              SKW_12b4_023f(vw_14, &vw_1c, &vw_10, vw_20, vw_34, vw_28);
            x16 wordrg6 = (vw_34 + eaxw - con(0x3)) & con(0x3);
            if (SKW_IS_CREATURE_MOVABLE_THERE(vw_14, wordrg6, &vw_2c, vw_20) && SKW_12b4_099e(vw_2c))
            {
              SKW_12b4_0d75(vw_14, wordrg6, con(0xfe), vw_20);
              ret = 1;
            }
            else if (wordrg71 == con(0x0))
              SKW_ATTACK_CREATURE(con(0xffff), vw_20, con(0x4005), vw_14, con(0x5), wordrg71);
          }
        break;

        case 4:
        break;

        case 5:
          ret = 1;
        break;

        default: throw(THROW_DMABORT);
      }
    }
    if (ret == 0) // M_13DAB:
    {
      SKW_1031_098e();
      ddata.v1e0488 = false;
      return false;
    }
    else if (ret == 1) // M_13B7A:
    {
      if (ddata.v1e0234 && vw_28 == con(0x2))
      {
        SKW_CHANGE_CURRENT_MAP_TO(vw_30);
        vw_14 = ddata.v1e0280;
        vw_20 = ddata.v1e027e;
        vw_34 = ddata.v1e0264;
        SKW_CALC_VECTOR_W_DIR(vw_34, con(0x0), &vw_14, con(0xffffffff), &vw_20);
      }
      x16 wordrg3;
      x16 wordrg2;
      x16 wordrg400;
      if (vw_18 == con(0x0))
      {
        if (SKW_GET_TELEPORTER_DETAIL(&vs_04, vw_20, vw_14) && (wordrg3 = unsignedword(vs_04.b_01), ((wordrg3 + con(0x2)) & con(0x3)) != vw_34))
        {
          wordrg2 = unsignedword(vs_04.b_04);
          wordrg400 = unsignedword(vs_04.b_03);
          R_3BF83(unsignedword(vs_04.b_02), wordrg2, wordrg3, wordrg400);
          ret = -1;
        }
        else
        {
          wordrg3 = vw_14;
          wordrg2 = signedword(ddata.v1e0272);
          wordrg400 = signedlong(ddata.v1e0270);
        }
      }
      else
      {
        wordrg3 = vw_14;
        wordrg2 = con(0x0);
        wordrg400 = con(0xffffffff);
      }
      if (ret != -1)
        SKW_MOVE_RECORD_TO(con(0xffff), wordrg2, wordrg3, wordrg400, vw_20);
    }
  }
  ddata.v1e026e += DM2_MAX(signedlong(wordrg5) / 2, (ddata.v1e025c != con(0x0)) ? 1 : 0);
  ddata.v1e025e = con(0x0);
  return true;
}

void SKW_GAME_LOOP(void)
{
  ddata.ticktrig = con(0x8);
  ddata.dialog2 = false;
  SKW_FILL_ORPHAN_CAII();
  bool loadmap = false; // new for better structure
  // M_1475D:
  for(;;)
  {
    if (loadmap)
    {
      // M_14773:
      SKW_LOAD_NEWMAP(ddata.v1e0270, ddata.v1d27f8, true, ddata.v1e0272);
      ddata.v1e025c = con(0x0);
      SKW_MOVE_RECORD_TO(con(0xffff), con(0x0), signedlong(ddata.v1e0270), con(0xffffffff), ddata.v1e0272);
      ddata.v1d27f8 = con(0xffffffff);
      loadmap = false;
    }
    else
    {
      ddata.ticksum = con(0x0);
      if (ddata.v1d27f8 != con(0xffffffff))
      {
        loadmap = true;
        continue;
      }
    }
    // M_147CC:
    SKW_PROCEED_TIMERS();
    if (ddata.v1d27f8 != con(0xffffffff))
    {
      loadmap = true;
      continue;
    }
    SKW_UPDATE_WEATHER(false);
    if (!ddata.v1e0238)
    {
      if (ddata.v1e0288 == con(0x0))
      {
        SKW_38c8_0060();
      }
      if (ddata.v1e0976 == con(0x0))
      {
        if (ddata.v1e0390.l0 != con(0x0))
        {
          SKW_CHECK_RECOMPUTE_LIGHT();
        }
        if (ddata.v1e025c == con(0x0))
          SKW_DISPLAY_VIEWPORT(ddata.v1e0258, ddata.v1e0272, ddata.v1e0270);
        else
          SKW_DISPLAY_VIEWPORT(ddata.v1e0278, ddata.v1e0268, ddata.v1e026a);
        SKW_CHANGE_VIEWPORT_TO_INVENTORY(true);
        ddata.v1e01d8 = false;
      }
      if (ddata.v1e025c != con(0x0))
      {
        if (ddata.v1e025c == con(0x1))
        {
          SKW_PERFORM_MOVE(ddata.v1e0256);
        }
        ddata.v1e025c--;
        if (ddata.v1d27f8 != con(0xffffffff))
        {
          loadmap = true;
          continue;
        }
      }
      ddata.v1e048c = false;
      if (ddata.v1e0254 != con(0x0))
      {
        ddata.v1e0254 = con(0x0);
        SKW_443c_0434();
      }
      SKW_IBMIO_USER_INPUT_CHECK();
    }
    SKW_12b4_0092();
    SKW_482b_05bf(false);
    SKW_PROCESS_PLAYERS_DAMAGE();
    if (ddata.savegames1.b4 != con(0x0))
      ddata.savegames1.b4--;
    x16 wordrg145;
    if (!ddata.v1e0238)
      wordrg145 = con(0x3f);
    else
      wordrg145 = con(0xf);
    if ((wordrg145 & CUTX16(ddata.longmallocx)) == con(0x0))
    {
      SKW_UPDATE_CHAMPIONS_STATS();
    }
    SKW_GLOBAL_UPDATE_UNKNOW1();
    SKW_2e62_0cfa(true);
    if (ddata.v1e0250)
    {
      R_10777();
      return;
    }
    ddata.longmallocx++;
    if ((CUTX16(ddata.longmallocx) & con(0x1ff)) == con(0x0))
    {
      SKW_BURN_PLAYER_LIGHTING_ITEMS();
    }
    if (ddata.savegames1.b3 != con(0x0))
    {
      ddata.savegames1.b3--;
    }
    if (ddata.v1e026e != con(0x0))
      ddata.v1e026e = ddata.v1e026e - con(0x1);
    if (ddata.v1e025e != con(0x0))
      ddata.v1e025e--;
    SKW_3929_086f();
    ddata.tickevent = false;
    SKW_12b4_0092();
    do
    {
      if (ddata.v1e0224)
      {
        x16 wordrg400 = unsignedword(SKW_0cee_04e5(ddata.v1e0258, con(0x0), ddata.v1e0270, con(0x1), ddata.v1e0272));
        if (CUTLX8(urshift(wordrg400, con(0x5))) != con(0x6) || (wordrg400 & con(0x1)) == con(0x0) || (wordrg400 &= con(0x4), wordrg400 != con(0x0)))
        {
          ddata.v1e0224 = false;
          SKW_FIRE_MOUSE_RELEASE_CAPTURE();
          ddata.mouse_visibility = con(0x1);
          SKW_FIRE_SHOW_MOUSE_CURSOR();
        }
      }
      SKW_EVENT_LOOP();
    } while (!ddata.tickevent || !ddata.v1e0248);
    x16 wordrg400 = ddata.v1d27f8;
    if (wordrg400 != con(0xffffffff))
    {
      wordrg400 = ddata.v1d3248;
      SKW_CHANGE_CURRENT_MAP_TO(ddata.v1d27f8);
    }
    SKW_2759_12e6();
    if (wordrg400 != con(0xffffffff))
      SKW_CHANGE_CURRENT_MAP_TO(wordrg400);
    R_BA7(ddata.v1e0266);
  }
}

void SKW_ATTACK_CREATURE(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1)
{
  s_d556* sd5ptrrg6;
  c_record* recoptrrg5;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;
  c_aispec* pai_1c;
  x16 vw_20;
  bool flag;

  vw_0c = eaxw;
  vw_08 = edxw;
  vw_04 = ebxw;
  x16 vw_00 = ecxw;
  bool vbool_14 = false;
  if (eaxw == con(0xffffffff))
  {
    vw_0c = SKW_GET_CREATURE_AT(vw_08, ebxw);
    if (vw_0c == con(0xffffffff))
      return;
  }
  bool boolrg7 = (vw_00 & con(0x4000)) != con(0x0);
  if (boolrg7)
  {
    vw_00 &= con(0xbfff);
    if (SKW_RAND01() != con(0x0))
      boolrg7 = false;
  }
  x16 wordrg133 = vw_00 & con(0x2000);
  flag = wordrg133 == con(0x0);
  x16 vw_10 = wordrg133;
  if (!flag)
    vw_00 &= con(0xdfff);
  recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(vw_0c);
  pai_1c = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(recoptrrg5->u4.b.b0);
  x16 wordrg166 = pai_1c->u0.w & con(0x1);
  x16 vw_18 = wordrg166;
  if (recoptrrg5->u4.b.b1 == con(0xffffffff))
  {
    if (wordrg166 == con(0x0))
      return;
    SKW_ALLOC_CAII_TO_CREATURE(vw_0c, vw_04, vw_08);
  }
  sd5ptrrg6 = &ddata.v1e089c[recoptrrg5->u4.b.b1];
  x16 wordrg2 = sd5ptrrg6->u.v.w14 + argw1;
  sd5ptrrg6->u.v.w14 = wordrg2;
  argw1 = wordrg2;
  if (vw_18 == con(0x0) && argw0 > con(0x0))
  {
    if ((recoptrrg5->ua.w & con(0x4)) == con(0x0))
    {
      if (wordrg2 > con(0x1e) || (wordrg2 > con(0x4) && SKW_RAND02() == con(0x0)) || UI32CAST(100 * argw1 / pai_1c->w4) > con(0xf))
      {
        recoptrrg5->ua.b.b0 |= con(0x4);
        boolrg7 = true;
      }
    }
    if ((boolrg7 && ((table1d607e[SKW_QUERY_GDAT_CREATURE_WORD_VALUE(recoptrrg5->u4.b.b0, con(0x1))].u.b[0] & con(0xffffff80)) == con(0x0))) && SKW_RAND01() != con(0x0))
    {
      bool skip = false;
      x16 wordrg403 = SKW_CALC_VECTOR_DIR(vw_08, ddata.v1e0270, ddata.v1e0272, vw_04);
      if ((recoptrrg5->ua.w & con(0x8)) == con(0x0) || SKW_RAND02() == con(0x0))
      {
        if (wordrg403 == ulrshift(recoptrrg5->ue.w, con(0x6), con(0xe)) || SKW_RAND02() != con(0x0))
        {
          skip = true;
        }
      }
      if (!skip)
      {
        wordrg403 += con(0x2);
        wordrg403 &= con(0x3);
      }
      x8 byterg400;
      x16 wordrg22 = (wordrg403 + con(0x2)) & con(0x3);
      x16 wordrg17 = ulrshift(recoptrrg5->ue.w, con(0x6), con(0xe));
      if (wordrg17 != wordrg22)
      {
        if (wordrg17 == wordrg403)
        {
          if (SKW_RAND02() != con(0x0))
            byterg400 = con(0xffffffff);
          else
            byterg400 = ((SKW_RAND01() == con(0x0)) ? 1 : 0) + con(0x6);
        }
        else
        {
          wordrg403--;
          wordrg403 &= con(0x3);
          byterg400 = ((wordrg17 == wordrg403) ? 1 : 0) + con(0x6);
        }
      }
      else
        byterg400 = ((SKW_RAND01() != con(0x0)) ? 1 : 0) + con(0x6);
      if (byterg400 != con(0xffffffff))
        SKW_13e4_0360(vw_0c, vw_04, byterg400, vw_08, false);
    }
    vbool_14 = (signedlong(argw0) > SKW_RAND16(con(0x64)));
    flag = !vbool_14;
    if (!flag)
    {
      vw_20 = vw_00 & con(0x8000);
      vw_00 = con(0x1) << CUTX8(vw_00);
      if (vw_20 == con(0x0))
        recoptrrg5->ua.w |= vw_00;
      else
        recoptrrg5->ua.w &= ~vw_00;
    }
  }
  bool boolrg1;
  if ((vw_18 == con(0x0) && vw_10 != con(0x0)) && argw0 == con(0x0))
    boolrg1 = true;
  else if (!vbool_14 || ((vw_20 != con(0x0) || (vw_00 & con(0x40)) == con(0x0)) && vw_10 == con(0x0)))
    boolrg1 = false;
  else
  {
    x8 byterg2 = table1d613a[sd5ptrrg6->u.v.b1a];
    if ((byterg2 & con(0x10)) == con(0x0))
      boolrg1 = true;
    else
    {
      if ((table1d607e[SKW_QUERY_GDAT_CREATURE_WORD_VALUE(recoptrrg5->u4.b.b0, con(0x1))].u.w[0] & con(0x410)) == con(0x0) || (byterg2 & con(0x2)) == con(0x0))
        boolrg1 = false;
      else
        boolrg1 = true;
    }
  }
  if (sd5ptrrg6->u.v.b1a == con(0x13))
    return;
  if (!boolrg1)
    if (UI16CAST(argw1) < UI16CAST(recoptrrg5->u6.w))
      return;
  SKW_1c9a_0db0(unsignedlong(vw_0c));
  SKW_1c9a_0cf7(vw_08, vw_04);
}

bool SKW_2fcf_16ff(x16 eaxw)
{
  x16 wordrg5;
  c_hero* heroptrrg3;
  x16* wptrrg6;

  x16 wordrg2;
  bool boolrg7 = con(0x0);
  heroptrrg3 = ddata.hero;
  // M_3BE94: co-label M_3BE8A
  for (ui16 vuw_04 = con(0x0); ; vuw_04++, heroptrrg3++)
  {
    bool skipfirst = false; // added
    x16 wordrg11;
    if (vuw_04 < UI16CAST(ddata.savegamew2))
    {
      if (heroptrrg3->w36 == con(0x0))
        continue;
      wptrrg6 = heroptrrg3->warrc3;
      wordrg2 = con(0x0);
    }
    else
    {
      if (ddata.v1d67bc == con(0x5))
      {
        wordrg2 = con(0x0);
        // M_3BED1: // co-label M_3BED0
        for (;; wordrg2++)
        {
          if (wordrg2 >= con(0x8))
            break;
          x16 wordrg11 = ddata.v1e095c[wordrg2];
          if (wordrg11 == con(0xffffffff))
            continue;
          if (SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg11) == eaxw)
            return true;
        }
      }
      if (boolrg7)
        return false;
      boolrg7 = true;
      wordrg11 = ddata.savegamewpc.w0;
      skipfirst = true;
    }
    // M_3BE7C:
    for (;; wordrg2++)
    {
      if (!skipfirst)
      {
        if (wordrg2 >= con(0x1e) || boolrg7)
          break;
        wordrg11 = *wptrrg6++;
      }
      else
        skipfirst = false;
      // M_3BE4D:
      wordrg5 = wordrg11;
      if (SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg5) == eaxw)
        return true;
      if (SKW_2fcf_164e(wordrg5, eaxw))
        return true;
    }
  }
}

// was SKW_2c1d_11bf
x16 SKW_COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(x16 eaxw, ui16 ebxuw, x16 edxw)
{
  ui16 uwordrg5;
  c_hero* heroptrrg6;
  x16 vw_04;
  x16 vw_08;

  vw_04 = eaxw;
  uwordrg5 = ebxuw;
  heroptrrg6 = &ddata.hero[vw_04];
  x16 wordrg31 = CUTX16(SKW_RAND()) & con(0xf);
  wordrg31 += SKW_GET_PLAYER_ABILITY(heroptrrg6, con(0x0), con(0x1));
  vw_08 = heroptrrg6->warrc3[edxw];
  x16 wordrg403 = SKW_QUERY_ITEM_WEIGHT(vw_08);
  x16 wordrg21 = urshift(SKW_MAX_LOAD(heroptrrg6), con(0x4));
  wordrg31 += wordrg403 - con(0xc);
  if (UI16CAST(wordrg403) > UI16CAST(wordrg21))
  {
    wordrg31 -= (wordrg403 - wordrg21) >> con(0x1);
    x16 wordrg22 = (wordrg21 - con(0xc)) / 2 + wordrg21;
    if (UI16CAST(wordrg403) > UI16CAST(wordrg22))
    {
      wordrg403 -= wordrg22;
      wordrg31 -= 2 * wordrg403;
    }
  }
  if (uwordrg5 >= con(0x0))
  {
    wordrg31 += 2 * SKW_QUERY_PLAYER_SKILL_LV(vw_04, true, uwordrg5);
    x16 wordrg2 = con(0x0);
    bool doit = false; // added
    if (uwordrg5 < UI16CAST(con(0x4)))
    {
      if (uwordrg5 == con(0x0))
        wordrg2 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(vw_08, con(0x8));
      else if (uwordrg5 == con(0x1))
        doit = true;
    }
    else if (uwordrg5 <= UI16CAST(con(0x7)))
      wordrg2 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(vw_08, con(0x8));
    else if (uwordrg5 < UI16CAST(con(0x9)))
      ;
    else if (uwordrg5 <= UI16CAST(con(0x9)))
      wordrg2 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(vw_08, con(0x8));
    else if (uwordrg5 > UI16CAST(con(0xb)))
      ;
    else
      doit = true;
    if (doit)
    {
      wordrg2 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(vw_08, con(0x9));
      if (wordrg2 != con(0x0))
      {
        if ((SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(vw_08, con(0x5)) & con(0x8000)) == con(0x0))
        {
          if (uwordrg5 == con(0xb))
            wordrg2 = con(0x0);
        }
        else if (uwordrg5 != con(0xb))
          wordrg2 = con(0x0);
      }
    }
    wordrg31 += wordrg2;
  }
  x16 wordrg3 = SKW_STAMINA_ADJUSTED_ATTR(heroptrrg6, wordrg31);
  if (((((edxw != con(0x0)) ? 1 : 0) + con(0x1)) & heroptrrg6->w34) != con(0x0))
    wordrg3 >>= con(0x1);
  return DM2_BETWEEN_VALUE(con(0x0), con(0x64), wordrg3 / 2);
}

void SKW_29ee_1946(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2, x16 argw3)
{
  x16 wordrg5;
  c_record* recoptrrg1;
  c_record* recoptrrg3;
  x16 vw_00;
  x16 vw_0c;
  c_5bytes vs_10;
  x16 vw_18;
  x16 vw_1c;
  x16 vw_20;
  x16 vw_24;
  x16 vw_30;
  x16 vw_34;
  x16 vw_38;
  x16 vw_3c;
  x16 vw_48;
  x16 vw_4c;
  x16 vw_54;
  x16 vw_58;
  x16 vw_5c;
  x16 vw_60;
  x8 vb_64;

  vw_0c = eaxw;
  x16 vw_08 = edxw;
  x16 vw_04 = ebxw;
  vw_00 = ecxw;
  x16 wordrg61 = argw1;
  wordrg5 = argw2;
  if ((argw3 & con(0x8)) == con(0x0))
  {
    ddata.v1e0b82 = CUTX8(vw_08);
    ddata.v1e0b83 = CUTX8(vw_04);
    ddata.v1e0b81 = CUTX8(vw_00);
    ddata.v1e0b84 = CUTX8(argw0);
    ddata.v1e0b5a = wordrg61;
    ddata.v1e0b58 = wordrg5;
  }
  else
  {
    vw_08 = signedword(ddata.v1e0b82);
    vw_04 = signedword(ddata.v1e0b83);
    vw_00 = signedword(ddata.v1e0b81);
    argw0 = signedword(ddata.v1e0b84);
    wordrg61 = ddata.v1e0b5a;
    wordrg5 = ddata.v1e0b58;
  }
  ddata.v1e0b56 = con(0x0);
  vw_60 = ddata.v1d3248;
  SKW_29ee_00a3(false);
  SKW_FILL_RECT_SUMMARY(&ddata.v1d694c, ddata.paletteptr1[con(0x0)], SKW_QUERY_EXPANDED_RECT(con(0x63), &ddata.v1e0b9c));
  x16 wordrg22 = -ddata.v1d2750;
  SKW_INFLATE_RECT(&ddata.v1e0b9c, -ddata.v1d274e, wordrg22);
  x16 vw_28 = ddata.v1e0b9c.x + wordrg61;
  x16 vw_40 = ddata.v1e0b9c.y + wordrg5;
  vw_5c = argw3 & con(0x10);
  vw_34 = argw3 & con(0x20);
  vw_30 = argw3 & con(0x40);
  if (vw_34 != con(0x0) || vw_30 != con(0x0))
  {
    recoptrrg3 = SKW_GET_ADDRESS_OF_RECORD(vw_0c);
    if (vw_34 != con(0x0))
    {
      recoptrrg1 = SKW_GET_MISSILE_REF_OF_MINION(recoptrrg3->u2.w, vw_0c);
      vw_20 = recoptrrg1->u4.w & con(0x1f);
      vw_3c = ulrshift(recoptrrg1->u4.w, con(0x6), con(0xb));
      vw_38 = urshift(recoptrrg1->u4.w, con(0xa));
      SKW_CHANGE_CURRENT_MAP_TO(vw_38);
      if (!SKW_GET_TELEPORTER_DETAIL(&vs_10, vw_3c, vw_20))
        vw_1c = con(0xffffffff);
      else
      {
        vw_54 = unsignedword(vs_10.b_02);
        vw_58 = unsignedword(vs_10.b_03);
        vw_1c = unsignedword(vs_10.b_04);
      }
    }
    if (vw_30 != con(0x0))
    {
      x16 wordrg21 = urshift(recoptrrg3->u6.w, con(0xa));
      x16 wordrg41 = ulrshift(recoptrrg3->u6.w, con(0x6), con(0xb));
      SKW_29ee_18eb(recoptrrg3->u6.w & con(0x1f), wordrg21, wordrg41);
    }
  }
  SKW_CHANGE_CURRENT_MAP_TO(vw_00);
  x16 wordrg106 = con(0x3);
  vw_48 = con(0xfffffffd);
  if (wordrg5 < con(0x0))
    wordrg106 = con(0x4);
  x16 vw_2c = con(0xfffffffd);
  vw_4c = con(0x3);
  if (wordrg61 < con(0x0))
    vw_2c = con(0xfffffffc);
  x16 vw_44 = wordrg106;
  // M_34F71:
  for(;;)
  {
    if (vw_44 < vw_48)
    {
      SKW_CHANGE_CURRENT_MAP_TO(vw_60);
      return;
    }
    x16 vw_50 = vw_28;
    x16 wordrg6 = vw_2c;
    // M_34F4F:
    while (wordrg6 <= vw_4c)
    {
      vw_18 = vw_08;
      vw_24 = vw_04;
      SKW_CALC_VECTOR_W_DIR(argw0, wordrg6, &vw_18, vw_44, &vw_24);
      wordrg5 = vw_00;
      x16 wordrg3 = SKW_LOCATE_OTHER_LEVEL(wordrg5, &vw_18, &vw_24, con(0x0), NULL);
      if (wordrg3 >= con(0x0))
      {
        SKW_CHANGE_CURRENT_MAP_TO(wordrg3);
        vb_64 = CUTX8(ulrshift(ddata.v1e03c0->we, con(0x8), con(0xc)));
      }
      else
      {
        wordrg3 = vw_00;
        SKW_CHANGE_CURRENT_MAP_TO(wordrg5);
        if ((((SKW_GET_TILE_VALUE(vw_18, vw_24) & con(0xff)) >> con(0x5)) & con(0xff)) != con(0x7))
          vb_64 = CUTX8(ulrshift(ddata.v1e03c0->we, con(0x8), con(0xc)));
        else
          vb_64 = ddata.v1e03d4[ddata.v1e03c0->w8 & con(0x3f)];
      }
      argw3 &= con(0xffffff8f);
      if (vw_34 != con(0x0))
        if (vw_18 == vw_20 && vw_24 == vw_3c && wordrg3 == vw_38 || (wordrg3 == vw_1c && vw_18 == vw_54 && vw_24 == vw_58))
          argw3 |= con(0x20);
      if ((vw_30 != con(0x0)) && (ddata.v1e0b56 == con(0x0)))
      {
        if (vw_18 != ddata.v1e0b6e || vw_24 != ddata.v1e0b70 || wordrg3 != ddata.v1e0b64)
        {
          if (vw_18 != ddata.v1e0b68 || vw_24 != ddata.v1e0b6a || wordrg3 != ddata.v1e0b60)
          {
            if (vw_18 == ddata.v1e0b5e && vw_24 == ddata.v1e0b5c && wordrg3 == ddata.v1e0b66)
            {
              ddata.v1e0b56 = con(0x3);
              SKW_29ee_18eb(vw_18, wordrg3, vw_24);
            }
          }
          else
          {
            ddata.v1e0b56 = con(0x2);
            SKW_29ee_18eb(vw_18, wordrg3, vw_24);
          }
        }
        else
          ddata.v1e0b56 = con(0x1);
        if (ddata.v1e0b56 != con(0x0))
          argw3 |= con(0x40);
      }
      if ((vw_5c != con(0x0)) && (wordrg3 == ddata.v1e0282))
        if (vw_18 == ddata.v1e0280)
          if (vw_24 == ddata.v1e027e)
            argw3 |= con(0x10);
      SKW_DRAW_MAP_CHIP(vw_0c, vw_24, argw0, vw_18, vw_50, vw_40, vb_64, argw3);
      vw_50 += ddata.v1d274a + ddata.v1d274e;
      wordrg6++;
    }
    vw_40 += ddata.v1d274c + ddata.v1d2750;
    vw_44--;
  }
}

// was SKW_2759_02d6
bool SKW_IS_ITEM_HAND_ACTIVABLE(x16 eaxw, x16 ebxw, x16 edxw)
{
  x8 byterg5;
  x16 wordrg5;
  ui16 uwordrg5;
  bool flag;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;
  x16 vw_14;
  x8 vb_18;
  x8 vb_1c;
  x8 vb_20; // inc8

  vw_08 = eaxw;
  vw_04 = edxw;
  if (edxw != con(0xffffffff))
  {
    x16 wordrg40 = vw_04;
    if (SKW_IS_CONTAINER_MONEYBOX(wordrg40))
      return true;
    if (SKW_IS_CONTAINER_CHEST(wordrg40))
      return true;
    vb_18 = SKW_QUERY_CLS1_FROM_RECORD(wordrg40);
    vb_1c = SKW_QUERY_CLS2_FROM_RECORD(wordrg40);
  }
  else
  {
    vb_18 = con(0x16);
    vb_1c = ddata.hero[vw_08].b101;
  }
  x16 vw_10 = con(0x0);
  vb_20 = con(0x8);
  // M_2FCD2: co-label M_2FCCE
  for (;; vb_20++)
  {
    byterg5 = vb_20;
    if (byterg5 >= con(0xc))
      break;
    if (vw_10 >= con(0x3))
      break;
    if (!SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(vb_18, con(0x5), byterg5, vb_1c))
      continue;
    vw_0c = SKW_QUERY_CMDSTR_ENTRY(vb_18, byterg5, con(0x2), vb_1c);
    if (vw_0c == con(0x0))
      continue;
    x16 wordrg1 = SKW_QUERY_CMDSTR_ENTRY(vb_18, byterg5, con(0x11), vb_1c); // BUGFIX 4/2014
    if (wordrg1 != con(0x0))
      if (--wordrg1 != ebxw)
        continue;
    if (vw_04 == con(0xffffffff))
    {
      if (vw_0c == con(0x11))
      {
        flag = SKW_FIND_POUCH_OR_SCABBARD_POSSESSION_POS(vw_08, ebxw) < con(0x0);
        if (flag)
          continue;
      }
    }
    else
    {
      wordrg5 = vw_04;
      // RG1W = vw_08;  unused TODO: check again
      if (!SKW_2759_01fe(vw_0c, wordrg5))
        continue;
      x16 wordrg21 = SKW_QUERY_CMDSTR_ENTRY(vb_18, vb_20, con(0x8), vb_1c);
      if (wordrg21 != con(0x12))
      {
        if (wordrg21 == con(0x10) || wordrg21 == con(0x11))
          wordrg21 = con(0x1);
        if (wordrg21 != con(0x0))
        {
          flag = SKW_ADD_ITEM_CHARGE(vw_04, con(0x0)) < wordrg21;
          if (flag)
            continue;
        }
      }
      else if (SKW_ADD_ITEM_CHARGE(wordrg5, con(0x0)) != con(0x0))
        continue;
    }
    vw_14 = SKW_QUERY_CMDSTR_ENTRY(vb_18, vb_20, con(0x0), vb_1c);
    uwordrg5 = SKW_QUERY_CMDSTR_ENTRY(vb_18, vb_20, con(0x1), vb_1c);
    if (SKW_QUERY_PLAYER_SKILL_LV(vw_08, true, vw_14) >= uwordrg5)
    {
      ddata.v1e0b40[vw_10].b0 = vb_18;
      ddata.v1e0b40[vw_10].b1 = vb_1c;
      ddata.v1e0b40[vw_10].b2 = vb_20;
      vw_10++;
    }
  }
  ddata.v1e0b7a = vw_10;
  ddata.v1e0b50 = vw_04;
  if (SKW_IS_CONTAINER_MAP(vw_04))
    return true;
  return vw_10 > con(0x0);
}

x16 SKW_2066_33e7(void)
{
  x16 wordrg5;
  t_text* tptrrg3;
  c_hex2a* sptrrg1;
  c_rect rc_00;
  x16 vw_08; // x0
  x16 vw_0c; // y0

  x16 wordrg6;
  SKW__OPEN_DIALOG_PANEL(con(0x81), con(0x6));
  if (ddata.savegamew4 < con(0x0))
  {
    wordrg6 = con(0x0);
    for (;; wordrg6++)
    {
      if (wordrg6 >= con(0xa))
        break;
      if (ddata.savegamep1[wordrg6].s_02.l_24 == con(0xdeadbeef))
      {
        SKW_2066_33c4(ddata.v1e0900.s_02.t_00, CUTX8(wordrg6));
        break;
      }
    }
  }
  else
  {
    wordrg6 = ddata.savegamew4;
    SKW_COPY_MEMORY(DOWNCAST(c_hex2a, &ddata.savegamep1[wordrg6]), con(0x2a), DOWNCAST(c_hex2a, &ddata.v1e0900));
  }
  SKW_2066_398a(wordrg6);
  tptrrg3 = ddata.v1e0900.s_02.t_00;
  if (wordrg6 == con(0xa))
    SKW_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
  else
    SKW_2066_3820(tptrrg3, false);
  bool boolrg7 = false;
  for(;;)
  {
    SKW_EVENT_LOOP();
    ui16 uwordrg1 = ddata.mouse_unk06 - con(0x1);
    if (uwordrg1 <= UI16CAST(con(0x3)))
    {
      switch (uwordrg1)
      {
        case 0:
          wordrg6 = con(0xffffffff);
          ddata.v1e023c = false;
          boolrg7 = true;
        break;

        case 1:
          ddata.v1e023c = false;
          if (wordrg6 != con(0xa))
          {
            if (*tptrrg3 == con(0x0))
              SKW_2066_33c4(tptrrg3, CUTX8(wordrg6));
            SKW_2066_3820(tptrrg3, false);
            boolrg7 = true;
          }
        break;

        case 2:
          ddata.v1e023c = false;
          SKW_QUERY_EXPANDED_RECT(ddata.mouse_unk05, &rc_00);
          SKW_QUERY_TOPLEFT_OF_RECT(ddata.mouse_unk09, &vw_08, &vw_0c);
          wordrg6 = DM2_MIN((ddata.mouse_unk07 - (rc_00.y + vw_0c)) / ddata.strxplus, con(0xa));
          sptrrg1 = &ddata.savegamep1[wordrg6];
          if (sptrrg1->s_02.l_24 != con(0xdeadbeef))
            SKW_SK_STRCPY(ddata.v1e0900.s_02.t_00, sptrrg1->s_02.t_00);
          else
            SKW_2066_33c4(ddata.v1e0900.s_02.t_00, wordrg6);
          SKW_2066_398a(wordrg6);
          SKW_2066_3820(tptrrg3, false);
        break;

        case 3:
          if (wordrg6 != con(0xa))
            if (!ddata.v1e023c)
            {
              ddata.v1e023c = true;
              SKW_2066_3820(tptrrg3, true);
              wordrg5 = SKW_SK_STRLEN(tptrrg3);
            }
        break;

        default: throw(THROW_DMABORT);
      }
    }
    if (ddata.v1e023c && ddata.mouse_unk0a >= con(0xa5) && ddata.mouse_unk0a <= con(0xd6))
    {
      if (ddata.v1e051e != con(0xe))
      {
        if (ddata.v1e051e != con(0x1c))
        {
          if (wordrg5 < con(0x1f))
          {
            ui8 ubyterg2 = CUTX8(SKW_476d_05b6(ddata.v1e051e));
            if (ubyterg2 >= con(0x61) && ubyterg2 <= con(0x7a))
              ubyterg2 -= con(0x20);
            tptrrg3[wordrg5++] = ubyterg2;
            tptrrg3[wordrg5] = NULLBYTE;
          }
        }
        else
          ddata.v1e023c = false;
      }
      else if (wordrg5 > con(0x0))
        tptrrg3[--wordrg5] = NULLBYTE;
      SKW_2066_3820(tptrrg3, ddata.v1e023c);
    }
    ddata.mouse_unk06 = con(0x0);
    ddata.mouse_unk0a = con(0x0);
    if (boolrg7)
    {
      SKW_2066_37f2();
      return wordrg6;
    }
  }
}

bool SKW_WOUND_CREATURE(x16 eaxw)
{
  c_record* recoptrrg5;
  c_aispec* aiptrrg6;
  bool flag;
  x16 vw_10;

  recoptrrg5 = ddata.s350.v1e054e;
  aiptrrg6 = ddata.s350.v1e0552;
  if (aiptrrg6->b2 == con(0xffffffff))
    return false;
  x16 wordrg1 = aiptrrg6->u0.w & con(0x1);
  flag = wordrg1 != con(0x0);
  x16 vw_08 = wordrg1;
  x16 vw_04; // only requested if vw_08 == 0, but then it is initialized
  if (!flag)
  {
    vw_04 = CUTX16(table1d607e[SKW_QUERY_GDAT_CREATURE_WORD_VALUE(recoptrrg5->u4.b.b0, con(0x1))].u.l); // high word here not requested
    if ((vw_04 & con(0x4)) == con(0x0))
    {
      vw_10 = ddata.v1d3248;
      SKW_CHANGE_CURRENT_MAP_TO(urshift(recoptrrg5->uc.w, con(0xa)));
      SKW_INVOKE_MESSAGE(recoptrrg5->uc.w & con(0x1f), con(0x0), con(0x1), ulrshift(recoptrrg5->uc.w, con(0x6), con(0xb)), ddata.longmallocx + con(0x1));
      SKW_CHANGE_CURRENT_MAP_TO(vw_10);
    }
  }
  if (UI16CAST(recoptrrg5->u6.w) <= UI16CAST(eaxw))
  {
    recoptrrg5->u6.w = con(0x1);
    if (vw_08 == con(0x0))
    {
      if ((vw_04 & con(0x800)) != con(0x0))
        if (!SKW_1c9a_17c7(unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0571), unsignedword(ddata.s350.v1e0562.u.b.b_07)))
          return false;
      if ((vw_04 & con(0x800)) != con(0x0))
        ddata.v1e0526 = con(0xa0);
      SKW_13e4_0360(ddata.s350.v1e054c, unsignedword(ddata.s350.v1e0562.u.b.b_07), con(0x13), unsignedword(ddata.s350.v1e0562.u.b.b_06), true);
      return false;
    }
    SKW_DELETE_CREATURE_RECORD(unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0x0), con(0x1), unsignedword(ddata.s350.v1e0562.u.b.b_07));
    return true;
  }
  if (vw_08 == con(0x0) && (vw_04 & con(0x8000)) != con(0x0))
  {
    bool doit1 = false;
    if ((SKW_RAND() & con(0x7)) == con(0x0))
      doit1 = true;
    else
    {
      x32 longrg2 = unsignedlong(eaxw);
      bool doit2 = false;
      if (longrg2 > (3 * unsignedlong(aiptrrg6->w4) / con(0x64)))
        doit2 = true;
      else
      {
        if (longrg2 > (5 * unsignedlong(recoptrrg5->u6.w) / con(0x64)))
          doit2 = true;
      }
      if (doit2)
        // M_21522:
        if (((recoptrrg5->ua.w & con(0x8)) != con(0x0) && SKW_RAND01() != con(0x0)) || SKW_RAND02() == con(0x0))
          doit1 = true;
    }
    if (doit1)
      // M_21545:
      SKW_QUEUE_NOISE_GEN1(con(0xf), ((SKW_RAND01() != con(0x0)) ? 1 : 0) + con(0x9), con(0x46), recoptrrg5->u4.b.b0, con(0x80), unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_07), con(0x1));
  }
  recoptrrg5->u6.w -= eaxw;
  return false;
}

void SKW_PLAYER_CONSUME_OBJECT(x16 eaxw, x16 ebxw, x16 edxw)
{
  c_hero* heroptrrg5;
  c_record* recoptrrg3;
  x16 vw_00;
  x16 vw_04;
  x16 vw_08;
  x16 vw_1c;

  vw_08 = eaxw;
  vw_04 = edxw;
  vw_00 = ebxw;
  bool boolrg3 = true;
  if (edxw == con(0xffffffff))
  {
    if (ddata.savegamewpc.w0 == edxw)
    {
      SKW_FIRE_MOUSE_SET_CAPTURE();
      ddata.v1e0220 = boolrg3;
      SKW_443c_08ab(&ddata.v1e00b6, &ddata.v1e00b2, &ddata.v1e00b4);
      if ((ddata.v1e00b2 & con(0x2)) != con(0x0))
      {
        SKW_FIRE_HIDE_MOUSE_CURSOR();
        ddata.mouse_visibility = con(0x1);
        return;
      }
      SKW_FIRE_MOUSE_RELEASE_CAPTURE();
      ddata.v1e0220 = false;
      return;
    }
    vw_08 = ddata.v1e0976 - con(0x1);
    vw_04 = ddata.savegamewpc.w0;
    vw_00 = con(0xffffffff);
  }
  else
    boolrg3 = false;
  if (ddata.v1e0288 != con(0x0))
    return;
  x16 vw_10 = vw_08;
  heroptrrg5 = &ddata.hero[vw_08];
  x16 vw_18 = SKW_QUERY_GDAT_FOOD_VALUE_FROM_RECORD(vw_04);
  if (vw_18 == con(0x0)) //elsedone
  {
    if (!SKW_IS_MISCITEM_DRINK_WATER(vw_04)) //elsedone
    {
      if (urshift(vw_04 & con(0x3c00), con(0xa)) != con(0x8))
        return;
      recoptrrg3 = SKW_GET_ADDRESS_OF_RECORD(vw_04);
      vw_1c = unsignedword(CUTX8(recoptrrg3->u2.w));
      x32 longrg21 = unsignedlong(vw_1c);
      x32 longrg43 = longrg21 + con(0x1);
      x32 longrg111 = ((longrg43 - jul_shl32(longrg43, con(0x3))) >> con(0x3)) + con(0x20);
      x32 longrg44 = con(0x1ff) - longrg21;
      x16 wordrg6 = CUTX16((longrg44 / longrg111) >> con(0x1));
      x16 wordrg22 = CUTX16(longrg21 / con(0x19) + con(0x8));
      ui32 ulongrg110 = ulrshift(recoptrrg3->u2.w, con(0x1), con(0x9)) - con(0x6);
      if (ulongrg110 > con(0x9))
        return;
      x16 wordrg31; // in switch
      x16 wordrg32; // in switch
      x16 wordrg33; // in switch
      switch (ulongrg110)
      {
        case 0:
          SKW_2c1d_0300(heroptrrg5, wordrg22, con(0x2));
        break;

        case 1:
          SKW_2c1d_0300(heroptrrg5, vw_1c / con(0x23) + con(0x5), con(0x1));
        break;

        case 2:
          SKW_2c1d_0300(heroptrrg5, wordrg22, con(0x3));
        break;

        case 3:
          SKW_2c1d_0300(heroptrrg5, wordrg22, con(0x4));
        break;

        case 4:
          {
            wordrg31 = vw_1c;
            x16 wordrg63 = wordrg31 / con(0x7);
            wordrg31 += wordrg63 * wordrg63;
            SKW_2c1d_1bb5(vw_10, wordrg22 + wordrg31);
          }
        break;

        case 5:
          heroptrrg5->w3a += DM2_MIN(heroptrrg5->u3c.w - heroptrrg5->w3a, heroptrrg5->u3c.w / wordrg6);
        break;

        case 6:
          {
            x16 wordrg17 = unsignedlong(wordrg22);
            wordrg17 += wordrg17 / 2;
            x16 wordrg26 = wordrg17;
            wordrg17 *= wordrg26;
            SKW_2c1d_0009(CUTX16(con(0x1) << CUTX8(vw_08)), wordrg26, wordrg17, con(0x2));
          }
        break;

        case 7:
          {
            wordrg32 = DM2_MIN(con(0x384), wordrg22 - con(0x8) + heroptrrg5->w3e + wordrg22);
            x16 wordrg25 = wordrg32;
            if (wordrg25 > heroptrrg5->w40)
            {
              wordrg25 -= (wordrg25 - DM2_MAX(heroptrrg5->w3e, heroptrrg5->w40)) / 2;
              wordrg32 = wordrg25;
            }
            heroptrrg5->w3e = wordrg32;
          }
        break;

        case 8:
          {
            wordrg33 = DM2_MAX(con(0x1), vw_1c / con(0x2a));
            heroptrrg5->w36 += heroptrrg5->w38 / wordrg6;
            x16 wordrg41 = heroptrrg5->w34;
            if (wordrg41 != con(0x0))
            {
              x16 wordrg62 = con(0xa);
              ui16 uwordrg22 = con(0x0);
              for(;;)
              {
                while (uwordrg22 < wordrg33)
                {
                  heroptrrg5->w34 &= CUTX16(SKW_RAND());
                  uwordrg22++;
                }
                wordrg33 = con(0x1);
                if (wordrg41 != heroptrrg5->w34)
                  break;
                wordrg62 -= con(0x1);
                if (wordrg62 == con(0x0))
                  break;
                uwordrg22 = con(0x0);
              }
            }
          }
        break;

        case 9:
          heroptrrg5->w46 = DM2_MIN(heroptrrg5->w46 + con(0x640), con(0x800));
        break;

        default: throw(THROW_DMABORT);
      }
      if (vw_00 == con(0xffffffff))
        SKW_REMOVE_OBJECT_FROM_HAND();
      SKW_DEALLOC_RECORD(vw_04);
      x16 wordrg34 = SKW_ALLOC_NEW_RECORD(con(0x8));
      if (wordrg34 == con(0xffffffff))
        return;
      x16 wordrg21 = wordrg34;
      SKW_SET_ITEMTYPE(wordrg21, con(0x14));
      if (vw_00 != con(0xffffffff))
        heroptrrg5->warrc3[vw_00] = wordrg34;
      else
        SKW_TAKE_OBJECT(wordrg21, false);
    }
    else
    {
      heroptrrg5->w46 = DM2_MIN(heroptrrg5->w46 + con(0x320), con(0x800));
      if (vw_00 != con(0xffffffff))
        heroptrrg5->warrc3[vw_00] = vw_04;
    }
  }
  else
  {
    if (boolrg3)
    {
      SKW_FIRE_HIDE_MOUSE_CURSOR();
      SKW_REMOVE_OBJECT_FROM_HAND();
      x16 wordrg61 = con(0x5);
      while (--wordrg61 != con(0x0))
      {
        SKW_DRAW_STATIC_PIC(con(0x7), (((wordrg61 & con(0x1)) == con(0x0)) ? 1 : 0) + con(0x25), con(0x221), con(0x0), con(0xffffffff));
        SKW_CHANGE_VIEWPORT_TO_INVENTORY(false);
        SKW_SLEEP_SEVERAL_TIME(con(0x8));
      }
      SKW_FIRE_SHOW_MOUSE_CURSOR();
    }
    heroptrrg5->w44 = DM2_MIN(heroptrrg5->w44 + vw_18, con(0x800));
    SKW_PROCESS_ITEM_BONUS(vw_08, vw_00, con(0x2), vw_04);
    SKW_DEALLOC_RECORD(vw_04);
    if (vw_00 != con(0xffffffff))
      heroptrrg5->warrc3[vw_00] = con(0xffffffff);
  }
  if (heroptrrg5->w3a > heroptrrg5->u3c.w)
    heroptrrg5->w3a = heroptrrg5->u3c.w;
  if (heroptrrg5->w36 > heroptrrg5->w38)
    heroptrrg5->w36 = heroptrrg5->w38;
  SKW_QUEUE_NOISE_GEN2(con(0x16), con(0x83), con(0xfe), heroptrrg5->b101, ddata.v1e0270, ddata.v1e0272, con(0x0), con(0x96), con(0x80));
  heroptrrg5->u32.b.b1 |= con(0x38);
  if (vw_00 == con(0xffffffff))
    return;
  heroptrrg5->u32.b.b1 |= con(0x40);
  SKW_CALC_PLAYER_WEIGHT(vw_08);
}

t_text* SKW_FORMAT_SKSTR(const t_text* eaxt, t_text* edxt)
{
  t_text* tptrrg7; // non-const!
  t_text* tptrrg6; // non-const!
  t_text* tptrrg4; // non-const!
  const t_text* tptrrg5;
  t_text t_00[0x80];
  t_text t_80[0x80];
  t_text t_104[0x4]; // TODO dim probably just 0x2

  tptrrg4 = edxt;
  tptrrg5 = eaxt;
  tptrrg7 = edxt;
  if (edxt == NULL)
  {
    tptrrg4 = ddata.vp_1e097c[ddata.v1e0988];
    tptrrg7 = tptrrg4;
    ddata.v1e0988++;
    if (ddata.v1e0988 >= con(0x2))
      ddata.v1e0988 = con(0x0);
  }
  // M_2A79E:
  for(;;)
  {
    x8 byterg1;
    for(;;)
    {
      byterg1 = *tptrrg5;
      if (byterg1 == con(0x0))
      {
        *tptrrg4 = byterg1;
        return tptrrg7;
      }
      if (byterg1 == con(0x2e) && tptrrg5[con(0x1)] == con(0x5a))
        break;
      if (byterg1 == con(0x1))
        break;
      *tptrrg4++ = byterg1;
      tptrrg5++;
    }
    x16 wordrg2;
    if (byterg1 != con(0x1))
    {
      wordrg2 = con(0x0);
      x16 wordrg11 = con(0x0);
      tptrrg5 += con(0x2);
      while (wordrg11 < con(0x3))
      {
        wordrg2 = wordrg2 * con(0xa) + unsignedword(*tptrrg5++) - con(0x30);
        wordrg11++;
      }
    }
    else
    {
      tptrrg5 += byterg1;
      wordrg2 = unsignedword(*tptrrg5) - con(0x20);
      tptrrg5 += byterg1;
    }
    *tptrrg4 = NULLBYTE;
    tptrrg6 = t_80;
    if (UI16CAST(wordrg2) > con(0x1c))
    {
      tptrrg6 = v1d10f4;
    }
    else
    {
      switch (wordrg2)
      {
        case 0:
          SKW_SK_LTOA10(unsignedlong(ddata.v1e0218), tptrrg6);
        break;

        case 1:
          SKW_SK_LTOA10((-ddata.longv1d70c5 + con(0x3ff)) >> con(0xa), tptrrg6);
        break;

        case 2:
          SKW_QUERY_GDAT_TEXT(con(0x1), con(0x0), tptrrg6, con(0xfffffffe));
        break;

        case 3:
          tptrrg6 = ddata.v1d153a;
        break;

        case 4:
          tptrrg6 = ddata.v1d153e;
        break;

        case 5:
        case 6:
        case 16:
        case 18:
        case 19:
        case 21:
          tptrrg6 = v1d10f4;
        break;

        case 7:
          if (ddata.v1e098c != con(0xffffffff))
            tptrrg6 = ddata.hero[ddata.v1e098c].t00;
        break;

        case 8:
          tptrrg6 = ddata.v1d1542;
        break;

        case 9:
          tptrrg6 = ddata.v1d1546;
        break;

        case 10:
          SKW_SK_LTOA10(signedlong(ddata.v1e099a), tptrrg6);
        break;

        case 11:
          SKW_SK_LTOA10(signedlong(ddata.v1e098e), tptrrg6);
        break;

        case 12:
          SKW_SK_LTOA10(signedlong(ddata.v1e0998), tptrrg6);
        break;

        case 13:
          SKW_SK_LTOA10(signedlong(ddata.v1e0996), tptrrg6);
        break;

        case 14:
          SKW_SK_LTOA10(signedlong(ddata.v1e098a), tptrrg6);
        break;

        case 15:
          if (ddata.v1e0992 == con(0x1))
            tptrrg6 = ddata.v1d153a;
          else if (ddata.v1e0992 == con(0x2))
            tptrrg6 = ddata.v1d153e;
        break;

        case 17:
          SKW_QUERY_GDAT_TEXT(con(0x7), CUTX8(ddata.v1e0994), tptrrg6, con(0x0));
        break;

        case 20:
          tptrrg6 = ddata.v1d675a;
        break;

        case 22:
          if (!ddata.v1d154e)
            tptrrg6 = ddata.v1d6766;
          else
            tptrrg6 = ddata.datafoldername;
        break;

        case 23:
          t_104[0x0] = ddata.v1e099c;
          t_104[0x1] = NULLBYTE;
          SKW_SK_STRCPY(t_80, t_104);
        break;

        case 24:
          tptrrg6 = table1d674e[ddata.v1d674c];
        break;

        case 25:
          SKW_SK_LTOA10(signedlong(ddata.v1d70c9), tptrrg6);
        break;

        case 26:
          tptrrg6 = ddata.v1d676a;
        break;

        case 27:
          SKW_QUERY_GDAT_TEXT(con(0x1), con(0x6), tptrrg6, con(0xfffffffe));
        break;

        case 28:
          if (ddata.gdatfiletype2)
            tptrrg6 = ddata.v1d676a;
          else
            tptrrg6 = ddata.v1d675a;
        break;

        default: throw(THROW_DMABORT);
      }
    }
    SKW_FORMAT_SKSTR(tptrrg6, t_00);
    SKW_SK_STRCAT(tptrrg7, t_00);
    tptrrg4 = &tptrrg7[SKW_SK_STRLEN(tptrrg7)];
  }
}

