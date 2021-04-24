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

void SKW_32cb_4069(x16 eaxw, x16 ebxw, x16* ecxpw, x16 edxw, x16* argpw0)
{
  x16 vw_04;
  x16 vw_0c;
  x16 vw_18;
  x8 vb_1c; // set

  x16 wordrg2 = ebxw;
  if (wordrg2 == con(0x0))
    return;
  if (wordrg2 > con(0x3))
    wordrg2 = con(0x3);
  vw_18 = *ecxpw;
  x16 vw_14 = *argpw0;
  wordrg2--;
  vw_04 = eaxw - vw_18;
  x16 vw_10 = DM2_ABS(vw_04);
  if (vw_10 != con(0x0))
  {
    vw_04 = vw_14 - edxw;
    vw_0c = DM2_ABS(vw_04);
    if (vw_0c == con(0x0))
    {
      if (eaxw >= vw_18)
        *ecxpw += signedword(table1d6ea8[wordrg2]);
      else
        *ecxpw -= signedword(table1d6ea8[wordrg2]);
      return;
    }
    x32 longrg1 = lextended((eaxw < vw_18) ? 1 : 0) ^ (((edxw < vw_14) ? 1 : 0) & con(0xff));
    x16 wordrg14 = (longrg1 == con(0x0)) ? 1 : 0;
    vb_1c = (vw_10 < vw_0c) ? 1 : 0;
    vb_1c = ((signedlong(wordrg14) ^ unsignedlong(vb_1c)) == con(0x0)) ? 1 : 0;
    x16 vw_08 = unsignedword(vb_1c);
    vb_1c = (eaxw < vw_18) ? 1 : 0;
    x16 wordrg5 = vw_08 | (2 * wordrg14) | (4 * unsignedword(vb_1c));
    *ecxpw += signedword(table1d6e68[wordrg2][wordrg5].b0);
    *argpw0 += signedword(table1d6e68[wordrg2][wordrg5].b1);
  }
  else
  {
    if (edxw < vw_14)
      *argpw0 -= signedword(table1d6ea8[wordrg2]);
    else
      *argpw0 += signedword(table1d6ea8[wordrg2]);
  }
}

// TODO: arguments, mostly just as byte used
void SKW_DRAW_DEFAULT_DOOR_BUTTON(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  x16 vw_00;
  x16 vw_08;
  x8 vb_0c;
  x8 vb_10;
  x8 vb_14;

  vb_10 = CUTX8(eaxw);
  vb_0c = CUTX8(edxw);
  vb_14 = CUTX8(ebxw);
  x16 wordrg6 = signedword(table1d6ed3[ecxw]);
  if ((wordrg6 < con(0x0)) || (CUTX8(edxw) == con(0xffffffff)))
    return;
  x16 wordrg4 = signedword(table1d6b15[ecxw]);
  vw_08 = wordrg4;
  vw_00 = unsignedword(table1d6b71[table1d6b15[ecxw]]);
  x16 vw_04;
  if (vb_10 != con(0xc))
  {
    vw_04 = con(0x0);
    wordrg4 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(vb_10, con(0xb), con(0x4), vb_0c);
    if (wordrg4 == con(0x0))
      wordrg4 = ddata.v1d6c00;
  }
  else
  {
    vw_04 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xc), con(0xb), con(0x8), vb_0c) * con(0x5);
    wordrg4 = ddata.v1d6c00;
  }
  SKW_QUERY_TEMP_PICST(con(0x0), vw_00, con(0x0), vw_00, con(0x0), vw_08, vw_04 + con(0x79e) + wordrg6, con(0xffffffff), wordrg4, con(0xffff), vb_10, vb_0c, vb_14);
  SKW_DRAW_TEMP_PICST();
  if (wordrg6 == con(0x3) || wordrg6 == con(0x4))
    SKW_32cb_0a4c(&ddata.v1e118e.rc_24, CUTX8(ecxw), con(0x4));
}

// was SKW_32cb_4715
void SKW_DRAW_DOOR_FRAMES(x16 eaxw, x16 edxw)
{
  c_record* recoptrrg6;
  s_hex12* h12ptrrg1;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;
  x8 vb_10;

  vb_10 = ddata.v1d6c02;
  vw_0c = ddata.v1d6c00;
  recoptrrg6 = SKW_GET_ADDRESS_OF_RECORD(ddata.v1e1044[eaxw].s_02.warr_00[con(0x4)]);
  if (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xe), con(0xb), con(0x40), ddata.v1e040e[recoptrrg6->u2.w & con(0x1)]) != con(0x0))
    return;
  if ((edxw & con(0x1)) != con(0x0))
  {
    x8 byterg14 = table1d6efd[eaxw];
    if (byterg14 != con(0xffffffff))
      SKW_DRAW_DUNGEON_GRAPHIC(con(0x8), byterg14, table1d6f0b[eaxw], vb_10, vw_0c, con(0x0));
  }
  x16 wordrg18;
  if (ddata.v1e12d0 == con(0x0))
  {
    vw_04 = eaxw;
    wordrg18 = con(0x0);
    vw_08 = con(0x1);
  }
  else
  {
    vw_04 = signedword(table1d6b2c[eaxw]);
    wordrg18 = con(0x1);
    vw_08 = con(0x0);
  }
  if ((edxw & con(0x2)) != con(0x0))
  {
    x8 byterg12 = table1d6ee1[vw_04][wordrg18];
    if (byterg12 != con(0xffffffff))
    {
      SKW_QUERY_TEMP_PICST(con(0x0), con(0x40), con(0x0), con(0x40), con(0x0), con(0x0), SKW_QUERY_CREATURE_BLIT_RECTI(eaxw, t_rotate(0), con(0xa)), con(0x4), vw_0c, con(0xffff), con(0x8), vb_10, byterg12);
      SKW_DRAW_TEMP_PICST();
    }
  }
  if ((edxw & con(0x4)) == con(0x0))
    return;
  x8 byterg1 = table1d6ee1[vw_04][vw_08];
  if (byterg1 == con(0xffffffff))
    return;
  SKW_QUERY_TEMP_PICST(con(0x1), con(0x40), con(0x0), con(0x40), con(0x0), con(0x0), SKW_QUERY_CREATURE_BLIT_RECTI(eaxw, t_rotate(con(0x0)), con(0xe)), con(0x3), vw_0c, con(0xffff), con(0x8), vb_10, byterg1);
  SKW_DRAW_TEMP_PICST();
  if ((recoptrrg6->u2.b.b0 & con(0x40)) == con(0x0))
  {
    h12ptrrg1 = &ddata.v1e1044[eaxw];
    if (h12ptrrg1->s_02.warr_00[con(0x5)] == con(0xff))
      return;
    x16 tmp = h12ptrrg1->s_02.warr_00[con(0x5)] >> con(0x8);
    SKW_DRAW_DEFAULT_DOOR_BUTTON(con(0x9), unsignedword(CUTX8(tmp + con(0x1))), eaxw, unsignedword(CUTX8(h12ptrrg1->s_02.warr_00[0x5])));
  }
  else
    SKW_DRAW_DEFAULT_DOOR_BUTTON(con(0xc), unsignedword(CUTX8(5 * ulrshift(recoptrrg6->u2.w, con(0x4), con(0xf)))), eaxw, con(0x0));
}

// was SKW_1031_0d9e
const x8* SKW_TRANSMIT_UI_EVENT(c_event* eaxps)
{
  SKW_ADJUST_UI_EVENT(eaxps);
  ui16 uwordrg1 = eaxps->warr0[con(0x2)];
  if (uwordrg1 == con(0x0))
    return NULL;
  if (uwordrg1 > con(0xf2))
    return NULL;
  if (uwordrg1 >= con(0x14) && uwordrg1 < con(0x42))
    return &table1d3efd[con(0x1)];
  if (uwordrg1 == con(0x8d) || (uwordrg1 >= con(0x7d) && uwordrg1 <= con(0x81)))
    return &table1d3efd[con(0x1)];
  if (uwordrg1 >= con(0x10) && uwordrg1 <= con(0x13))
    return &table1d3efd[con(0x1)];
  if (uwordrg1 >= con(0x9d) && uwordrg1 <= con(0xa4))
    return &table1d3efd[con(0x1)];
  if (uwordrg1 >= con(0x48) && uwordrg1 <= con(0x4b))
    return &table1d3efd[con(0x1)];
  if (uwordrg1 >= con(0x95) && uwordrg1 <= con(0x9c))
    return &table1d3efd[con(0x1)];
  if (uwordrg1 >= con(0x1) && uwordrg1 <= con(0x6))
  {
    uwordrg1--;
    return &table1d3efd[uwordrg1 * con(0xf) + con(0x2)];
  }
  if (uwordrg1 >= con(0x74) && uwordrg1 <= con(0x7b))
  {
    uwordrg1 -= con(0x74);
    uwordrg1 &= con(0x1);
    return &table1d3efd[uwordrg1 * con(0x5) + con(0x5c)];
  }
  if (uwordrg1 >= con(0x5f) && uwordrg1 <= con(0x62))
  {
    uwordrg1 -= con(0x5f);
    return &table1d3efd[uwordrg1 * con(0x5) + con(0x66)];
  }
  if (uwordrg1 == con(0x70))
    return &table1d3efd[con(0x7a)];
  if (uwordrg1 == con(0x5d) || uwordrg1 == con(0x5e))
  {
    uwordrg1 -= con(0x5d);
    uwordrg1 *= 8;
    uwordrg1 += con(0x82);
    return &table1d3efd[uwordrg1];
  }
  if (uwordrg1 >= con(0x65) && uwordrg1 <= con(0x6a))
    return &table1d3efd[con(0x92)];
  if (uwordrg1 == con(0x6b))
    return &table1d3efd[con(0x96)];
  if (uwordrg1 == con(0x6c))
    return &table1d3efd[con(0x9e)];
  if (uwordrg1 >= con(0x71) && uwordrg1 <= con(0x73))
  {
    uwordrg1 -= con(0x71);
    return &table1d3efd[uwordrg1 * con(0x5) + con(0xa2)];
  }
  if (uwordrg1 == con(0xb) || uwordrg1 == con(0x93))
    return &table1d3efd[con(0xb1)];
  if (uwordrg1 == con(0x8e))
    return &table1d3efd[con(0xb6)];
  if (uwordrg1 == con(0x8c))
    return &table1d3efd[con(0xbb)];
  if (uwordrg1 >= con(0x56) && uwordrg1 <= con(0x59))
  {
    uwordrg1 -= con(0x56);
    return &table1d3efd[uwordrg1 * con(0x7) + con(0xc0)];
  }
  if (uwordrg1 < con(0xdb) || uwordrg1 > con(0xde))
  {
    if (uwordrg1 != con(0x50))
      return &table1d3efd[con(0x0)];
    return &table1d3efd[con(0xea)];
  }
  ui16 uwordrg4 = ddata.v1e0204 - con(0x1);
  if (uwordrg4 <= con(0x6))
  {
    switch (uwordrg4)
    {
      case 0:
      case 1:
      case 6:
      return &table1d3efd[con(0xe3)];

      case 2:
        if (uwordrg1 < con(0xdc))
          return &table1d3efd[con(0xe3)];
      case 3: // case 2 can run through to it
      return &table1d3efd[con(0xdc)];

      case 4:
      case 5:
        if (uwordrg1 <= con(0xdc))
          return &table1d3efd[con(0xdc)];
      break;

      default: throw(THROW_DMABORT);
    }
  }
  return &table1d3efd[con(0x1)];
}

// return could be byte only
x16 SKW_12b4_0881(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16* argpw1)
{
  x16 vw_00;

  vw_00 = ecxw;
  *argpw1 = con(0xffffffff);
  if (CUTX8(urshift(edxw, con(0x5))) == con(0x3) && (eaxw == con(0x2)))
    return con(0x1);
  if ((urshift(ebxw, con(0x5)) & con(0xff)) == con(0x3))
    return con(0x2);
  if (SKW_IS_TILE_BLOCKED(CUTX8(ebxw)))
    return con(0x3);
  *argpw1 = SKW_GET_CREATURE_AT(vw_00, argw0);
  if (*argpw1 != con(0xffffffff) && (SKW_QUERY_CREATURE_AI_SPEC_FLAGS(*argpw1) & con(0x8000)) == con(0x0))
  {
    if (SKW_12b4_0953(SKW_GET_ADDRESS_OF_RECORD(*argpw1), ddata.v1e0258))
      return con(0x5);
    return con(0x4);
  }
  if (SKW_1c9a_03cf(&vw_00, con(0xff), &argw0) != con(0xffffffff))
    if ((SKW_QUERY_CREATURE_AI_SPEC_FLAGS(*argpw1) & con(0x8000)) == con(0x0))
      return con(0x5);
  return con(0x6);
}

void SKW_3e74_5b7c(x16 eaxw)
{
  t_text t_00[0x10];

  if (table1d6797[ddata.v1d6795++] == SKW_476d_05b6(eaxw))
  {
    if (table1d6797[ddata.v1d6795] != con(0x0))
      return;
    SKW_DISPLAY_HINT_TEXT(con(0xf), v1d1118);
    SKW_DISPLAY_HINT_TEXT(con(0xf), SKW_SK_LTOA10(con(0x2), t_00));
    SKW_3929_0b01(con(0xf), con(0x2e));
    SKW_3929_0b20(con(0xf), con(0xc));
    SKW_3929_0b20(con(0xf), dm2_dballochandler.v1e0a0c);
    SKW_SK_STRCPY(t_00, v1d111f);
    if (dm2_dballochandler.v1e13fe[0] != con(0x0)) // TODO: v1e13fe probably a bool-array
      t_00[0x0] = con(0x47);
    if (dm2_dballochandler.v1e13fe[1] != con(0x0)) // TODO: v1e13fe probably a bool-array
      t_00[0x1] = con(0x53);
    if (dm2_dballochandler.v1e13fe[2] != con(0x0)) // TODO: v1e13fe probably a bool-array
      t_00[0x2] = con(0x45);
    SKW_DISPLAY_HINT_TEXT(con(0xf), t_00);
  }
  ddata.v1d6795 = con(0x0);
}

t_text* SKW_QUERY_CMDSTR_NAME(x8 eaxb, x8 ebxb, x8 edxb)
{
  t_text* tptrrg1;
  t_text* tptrrg3;
  t_text t_00[0x80];

  tptrrg1 = SKW_QUERY_GDAT_TEXT(eaxb, ebxb, t_00, edxb);
  if (*tptrrg1 == NULLBYTE)
    return NULL;
  x16 wordrg2 = con(0x0);
  for(;;)
  {
    x8 byterg4 = *tptrrg1++;
    if (byterg4 == con(0x3a) || byterg4 == con(0x0))
      break;
    ddata.v1e0b85[wordrg2] = byterg4;
    wordrg2++;
  }
  ddata.v1e0b85[wordrg2] = con(0x0);
  tptrrg3 = ddata.v1e0b85;
  return tptrrg3;
}

bool SKW_2759_0155(x16 eaxw)
{
  x8 byterg5;
  x16 vw_00;
  x8 vb_04;
  x8 vb_08;
  x8 vb_0c;

  vw_00 = eaxw;
  if (eaxw == con(0xffffffff))
    return false;
  vb_04 = SKW_QUERY_CLS1_FROM_RECORD(eaxw);
  vb_08 = SKW_QUERY_CLS2_FROM_RECORD(eaxw);
  vb_0c = con(0x8);
  for(;;)
  {
    byterg5 = vb_0c;
    if (byterg5 >= con(0xc))
      return false;
    if (SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(vb_04, con(0x5), byterg5, vb_08))
      if (SKW_QUERY_CMDSTR_ENTRY(vb_04, byterg5, con(0x2), vb_08) != con(0x0))
        if (SKW_QUERY_CMDSTR_ENTRY(SKW_QUERY_CLS1_FROM_RECORD(vw_00), byterg5, con(0x8), SKW_QUERY_CLS2_FROM_RECORD(vw_00)) != con(0x0))
          return true;
    vb_0c++;
  }
}

// was SKW_2759_04c0
x16 SKW_FIND_POUCH_OR_SCABBARD_POSSESSION_POS(x16 eaxw, x16 edxw)
{
  if (edxw != con(0x1))
  {
    if (edxw == con(0x0))
    {
      if (ddata.hero[eaxw].warrc3[0xb] != con(0xffffffff))
        return con(0xb);
      if (ddata.hero[eaxw].warrc3[0xb] != ddata.hero[eaxw].warrc3[6])
        return con(0x6);
    }
    return con(0xffffffff);
  }
  if (ddata.hero[eaxw].warrc3[0xc] != con(0xffffffff))
    return con(0xc);
  x16 wordrg2 = con(0x7);
  for(;;)
  {
    if (wordrg2 > con(0x9))
      return con(0xffffffff);
    if (ddata.hero[eaxw].warrc3[wordrg2] != con(0xffffffff))
      return wordrg2;
    wordrg2++;
  }
}

x16 SKW_ATTACK_PARTY(x16 eaxw, x16 ebxw, x16 edxw)
{
  x16 vw_00;
  x16 vw_04;
  x16 vw_0c;

  x16 wordrg6 = eaxw;
  vw_04 = edxw;
  vw_00 = ebxw;
  if (eaxw == con(0x0))
    return con(0x0);
  x16 wordrg5 = con(0x0);
  vw_0c = wordrg6 / 8 + con(0x1);
  wordrg6 -= vw_0c;
  vw_0c *= 2;
  for (ui16 vuw_08 = con(0x0); vuw_08 < ddata.savegamew2; vuw_08++)
    if (SKW_WOUND_PLAYER(vuw_08, vw_04, vw_00, DM2_MAX(con(0x1), SKW_RAND16(vw_0c) + wordrg6)) != con(0x0))
      wordrg5 |= con(0x1) << CUTX8(vuw_08);
  return wordrg5;
}

bool SKW_2c1d_1de2(x16 eaxw, x16 ebxw, x16 edxw)
{
  c_hero* heroptrrg5;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_14;

  vw_14 = edxw;
  heroptrrg5 = &ddata.hero[eaxw];
  bool boolrg3 = false;
  x16 vw_00;
  if (edxw < con(0x0))
  {
    if (ddata.savegamewpc.w0 == con(0xffffffff))
      return false;
    vw_00 = heroptrrg5->warrc3[con(0x1)];
    vw_0c = SKW_REMOVE_OBJECT_FROM_HAND();
    heroptrrg5->warrc3[con(0x1)] = vw_0c;
    vw_14 = con(0x1);
    boolrg3 = true;
  }
  x16 vw_08 = SKW_COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(eaxw, con(0xa), vw_14);
  if (!boolrg3)
  {
    vw_0c = SKW_REMOVE_POSSESSION(eaxw, vw_14);
    if (vw_0c == con(0xffffffff))
      return false;
  }
  else
    heroptrrg5->warrc3[con(0x1)] = vw_00;
  SKW_QUEUE_NOISE_GEN2(SKW_QUERY_CLS1_FROM_RECORD(vw_0c), con(0x86), con(0xfe), SKW_QUERY_CLS2_FROM_RECORD(vw_0c), ddata.v1e0270, ddata.v1e0272, con(0x1), con(0x6e), con(0x80));
  SKW_ADJUST_STAMINA(eaxw, SKW_2c1d_0e23(vw_0c));
  SKW_ADJUST_HAND_COOLDOWN(eaxw, signedlong(vw_14), con(0x4));
  vw_10 = con(0x8);
  x16 wordrg32 = con(0x1);
  x16 wordrg103 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(vw_0c, con(0x9));
  if (wordrg103 != con(0x0))
  {
    vw_10 = wordrg103 / 4 + con(0xc);
    wordrg32 = wordrg103;
  }
  SKW_ADJUST_SKILLS(eaxw, vw_10, con(0xa));
  vw_08 += wordrg32;
  x16 wordrg3 = SKW_QUERY_PLAYER_SKILL_LV(eaxw, true, con(0xa));
  vw_08 += SKW_RAND16((vw_08 >> con(0x2)) + con(0x8)) + wordrg3;
  vw_08 = DM2_MIN(vw_08, con(0xff));
  vw_10 = DM2_BETWEEN_VALUE(con(0x28), con(0xc8), (CUTX16(SKW_RAND()) & con(0x1f)) + 8 * wordrg3);
  x16 wordrg1 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(vw_0c, con(0xc));
  if (wordrg1 == con(0x0))
    wordrg1 = DM2_MAX(con(0x5), con(0xb) - wordrg3);
  SKW_SHOOT_ITEM(vw_0c, ddata.v1e0272, (ddata.v1e0258 + ebxw) & con(0x3), ddata.v1e0270, ddata.v1e0258, CUTX8(vw_08), CUTX8(vw_10), CUTX8(wordrg1));
  ddata.v1e025e = con(0x4);
  ddata.v1e0274 = ddata.v1e0258;
  return true;
}

// was SKW_2f3f_0343
// return never requested
x16 SKW_SELECT_CHAMPION(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  c_record* recoptrrg1;
  x8 vb_0c;
  x16 vw_10;

  x16 vw_08 = eaxw;
  x16 vw_04 = edxw;
  if (ddata.savegamewpc.w0 != con(0xffffffff))
    return con(0x0);
  x16 wordrg5 = ddata.savegamew2;
  if (wordrg5 >= con(0x4))
    return con(0x0);
  vw_10 = ddata.v1d3248;
  SKW_CHANGE_CURRENT_MAP_TO(ecxw);
  vw_08 += table1d27fc[ebxw];
  vw_04 += table1d2804[ebxw];
  x16 wordrg17 = SKW_GET_TILE_RECORD_LINK(vw_08, vw_04);
  for(;;)
  {
    x16 wordrg21 = wordrg17;
    if (wordrg21 == con(0xfffffffe))
      break;
    if (urshift(wordrg21 & con(0x3c00), con(0xa)) == con(0x3) && (recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg21), (recoptrrg1->u2.w & con(0x7f)) == con(0x7e)))
    {
      vb_0c = CUTX8(urshift(recoptrrg1->u2.w, con(0x7)));
      break;
    }
    wordrg17 = SKW_GET_NEXT_RECORD_LINK(wordrg21);
  }
  x16 wordrg22 = ebxw;
  x16 wordrg11 = (wordrg22 + con(0x2)) & con(0x3);
  vw_08 += table1d27fc[wordrg11];
  vw_04 += table1d2804[wordrg11];
  SKW_REVIVE_PLAYER(vb_0c, CUTX8(wordrg22));
  if (wordrg5 != con(0x0))
  {
    SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
    SKW_UPDATE_RIGHT_PANEL(false);
  }
  ddata.v1e0288 = wordrg5 + con(0x1);
  ddata.savegamew2++;
  if (wordrg5 == con(0x0))
    SKW_SELECT_CHAMPION_LEADER(con(0x0));
  x16 wordrg6 = (ebxw + con(0x2)) & con(0x3);
  vw_08 += table1d27fc[ebxw];
  vw_04 += table1d2804[ebxw];
  x16 wordrg23;
  x16 wordrg1 = SKW_GET_TILE_RECORD_LINK(vw_08, vw_04);
  for (;; wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg23))
  {
    wordrg23 = wordrg1;
    if (wordrg23 == con(0xfffffffe))
      break;
    if (urshift(wordrg23 & con(0x3c00), con(0xa)) > con(0x3))
      if (urshift(wordrg23, con(0xe)) == wordrg6)
        SKW_ADD_ITEM_TO_PLAYER(wordrg5, wordrg23);
  }
  if (!ddata.v1d6a2d)
  {
    SKW_24a5_1798(wordrg5);
    SKW_1031_0541(con(0x7));
    SKW_38c8_0002();
  }
  SKW_CHANGE_CURRENT_MAP_TO(vw_10);
  SKW_CALC_PLAYER_WEIGHT(wordrg5);
  return wordrg5 + con(0x1);
}

x8 SKW_2405_014a(x16 eaxw, bool ebxbool, x16 edxw)
{
  x16 vw_00;
  x16 vw_04;

  vw_04 = edxw;
  x8 byterg3 = con(0x18);
  if (!ebxbool) // BUGFIX 3/2014 flask animation
    return byterg3;
  x16 vw_08 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(eaxw, con(0x6));
  x16 wordrg5 = vw_08 & con(0x1f);
  if (wordrg5 == con(0x0))
    return byterg3;
  x16 wordrg40 = con(0x0);
  if ((vw_08 & con(0x8000)) != con(0x0))
  {
    wordrg40 = SKW_IS_ITEM_FIT_FOR_EQUIP(eaxw, true, vw_04);
    if (wordrg40 == con(0x0))
      return byterg3;
  }
  if ((vw_08 & con(0x4000)) != con(0x0))
  {
    if (ddata.v1e0b74 == con(0x0))
      return byterg3;
    if (eaxw != ddata.v1d67c2.w0[ddata.v1e0b74][ddata.v1e0b76])
      return byterg3;
    wordrg40 = con(0x1);
  }
  if (wordrg40 != con(0x0))
  {
    byterg3++;
    wordrg5--;
  }
  if (wordrg5 == con(0x0))
    return byterg3;
  x32 longrg2 = ddata.longmallocx;
  ui16 uwordrg1 = urshift(vw_08 & con(0x1f00), con(0x8));
  if (uwordrg1 <= con(0x6))
  {
    x16 wordrg41 = eaxw & con(0x3ff);
    switch (uwordrg1)
    {
      case 5:
        longrg2 += unsignedlong(wordrg41);
      case 0: // case 5 runs through here
        byterg3 += CUTLX8(longrg2 % unsignedlong(wordrg5));
      break;

      case 1:
        byterg3 += CUTX8(SKW_RAND16(wordrg5));
      break;

      case 2:
        byterg3 += CUTX8(ddata.v1e0258);
      break;

      case 3:
        {
          x16 wordrg15 = SKW_ADD_ITEM_CHARGE(eaxw, con(0x0));
          if (wordrg15 != con(0x0))
          {
            x32 longrg4 = unsignedlong(wordrg15) * unsignedlong(wordrg5);
            x32 longrg21 = unsignedlong(SKW_GET_MAX_CHARGE(eaxw)) + con(0x1);
            byterg3 += CUTX8((longrg4 / longrg21) + con(0x1));
          }
        }
      break;

      case 6:
        longrg2 += unsignedlong(wordrg41);
      case 4: // case 6 runs through here
        vw_04 = urshift(vw_08 & con(0xe0), con(0x5));
        vw_00 = SKW_ADD_ITEM_CHARGE(eaxw, con(0x0));
        if (vw_00 != con(0x0))
        {
          x32 longrg41 = unsignedlong(vw_00) * unsignedlong(wordrg5) / signedlong(vw_04);
          byterg3 += CUTX8((CUTX16(longrg2) % vw_04) + longrg41 / (unsignedlong(SKW_GET_MAX_CHARGE(eaxw)) + con(0x1)) * vw_04 + con(0x1));
        }
      break;

      default: throw(THROW_DMABORT);
    }
  }
  return byterg3;
}

// was SKW_2405_05b4
t_gfxdata* SKW_DRAW_ITEM_ON_WOOD_PANEL(x16 eaxw, c_hex18* ebxph18, x16 edxw)
{
  c_image vs_00;
  t_gfxdata vcarr_13c[PAL16];
  t_gfxdata* gptrrg5;
  t_gfxdata* gptrrg7;
  bool vbool_14c;
  x16 vw_150;
  x8 vb_154;
  x8 vb_158;

  if (!SKW_IS_ITEM_HAND_ACTIVABLE(eaxw, edxw, ddata.hero[eaxw].warrc3[edxw]))
    return NULL;
  x16 wordrg43 = ddata.v1d2722 + ddata.v1d271a;
  x16 wordrg2 = ddata.v1d2724 + ddata.v1d271c;
  vw_150 = SKW_3e74_5888();
  SKW_ALLOC_NEW_PICT(vw_150, wordrg2, con(0x8), wordrg43);
  x8 byterg1;
  x16 wordrg12 = ddata.hero[eaxw].warrc3[edxw];
  if (wordrg12 != con(0xffffffff))
  {
    vb_158 = SKW_QUERY_CLS1_FROM_RECORD(wordrg12);
    vb_154 = SKW_QUERY_CLS2_FROM_RECORD(wordrg12);
    byterg1 = SKW_2405_014a(wordrg12, false, con(0xffffffff));
    vbool_14c = false;
  }
  else
  {
    vb_158 = con(0x1);
    vb_154 = con(0x2);
    byterg1 = ((edxw == con(0x1)) ? 1 : 0) + con(0x7);
    vbool_14c = true;
  }
  SKW_QUERY_GDAT_SUMMARY_IMAGE(&vs_00, vb_154, byterg1, vb_158);
  SKW_QUERY_PICST_IT(&vs_00);
  x8 byterg3 = ddata.paletteptr1[con(0x8)];
  gptrrg5 = SKW_0b36_00c3(vw_150, ebxph18);
  gptrrg7 = gptrrg5;
  SKW_FILL_ENTIRE_PICT(gptrrg7, byterg3);
  x16 wordrg4;
  x16 wordrg1;
  if (!vbool_14c)
  {
    x16 wordrg41 = con(0x0);
    while (wordrg41 < con(0x10))
    {
      vcarr_13c[wordrg41] = ddata.paletteptr1[con(0x0)];
      wordrg41++;
    }
    vcarr_13c[0xc] = byterg3;
//                              srcbmp                                                                                                      srcofs destbmp                        palette
    SKW_DRAW_DIALOGUE_PICT(SKW_QUERY_PICT_BITMAP(&vs_00.s_00), SKW_ALLOC_TEMP_RECT(ddata.v1d2722, ddata.v1d2724, ddata.v1d271a, ddata.v1d271c), 0, gptrrg7, con(0x0), con(0xffff), vcarr_13c);
    R_B012(vs_00.xLocalPalette, con(0x0), con(0x1), con(0x1), &vs_00.w_38);
    gptrrg5 = SKW_0b36_00c3(vw_150, ebxph18);
    gptrrg7 = gptrrg5;
    wordrg4 = ddata.v1d271c;
    wordrg1 = ddata.v1d271a;
  }
  else
  {
    wordrg4 = vs_00.s_00.height;
    wordrg1 = vs_00.s_00.width;
  }
//                            srcbmp                                                                   srcofs destbmp                      palette
  SKW_DRAW_DIALOGUE_PICT(SKW_QUERY_PICT_BITMAP(&vs_00.s_00), SKW_ALLOC_TEMP_ORIGIN_RECT(wordrg1, wordrg4), 0, gptrrg5, con(0x0), con(0xc), vs_00.xLocalPalette);
  return gptrrg7;
}

void R_36EFE(x16 eaxw)
{
  c_hero* heroptrrg2;

  heroptrrg2 = &ddata.hero[eaxw];
  x16 wordrg3 = heroptrrg2->w36;
  heroptrrg2->w36 = con(0x0);
  if (heroptrrg2->b1d == con(0xffffffff) || SKW_GET_PLAYER_AT_POSITION(unsignedword(heroptrrg2->b1d)) != con(0xffffffff))
  {
    x16 wordrg4 = con(0x0);
    for(;;)
    {
      if (SKW_GET_PLAYER_AT_POSITION((ddata.v1e0258 + wordrg4) & con(0x3)) == con(0xffffffff))
        break;
      wordrg4++;
    }
    heroptrrg2->b1d = (CUTX8(ddata.v1e0258) + CUTX8(wordrg4)) & con(0x3);
  }
  heroptrrg2->b1c = CUTX8(ddata.v1e0258);
  heroptrrg2->w36 = wordrg3;
}

void SKW_PLAYER_DEFEATED(x16 eaxw)
{
  c_hero* heroptrrg5;
  c_record* recoptrrg1;
  x16 vw_00;

  heroptrrg5 = &ddata.hero[eaxw];
  if (eaxw == (ddata.v1e0b74 - con(0x1)))
    SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
  heroptrrg5->w36 = con(0x0);
  heroptrrg5->b2a[con(0x2)] = con(0x0);
  heroptrrg5->b2a[con(0x1)] = con(0x0);
  heroptrrg5->b2a[con(0x0)] = con(0x0);
  heroptrrg5->u32.b.b1 |= con(0x40);
  if ((eaxw + con(0x1)) == ddata.v1e0976)
  {
    if (ddata.v1e0228)
    {
      ddata.v1e0228 = false;
      SKW_FIRE_MOUSE_RELEASE_CAPTURE();
      if (ddata.savegamewpc.w0 != con(0xffffffff))
        SKW_DISPLAY_TAKEN_ITEM_NAME(ddata.savegamewpc.w0);
      ddata.mouse_visibility = con(0x1);
      SKW_FIRE_SHOW_MOUSE_CURSOR();
    }
    if (ddata.v1e0220)
    {
      ddata.v1e0220 = false;
      SKW_FIRE_MOUSE_RELEASE_CAPTURE();
      ddata.mouse_visibility = con(0x1);
      SKW_FIRE_SHOW_MOUSE_CURSOR();
    }
    SKW_24a5_1798(con(0x4));
  }
  if (eaxw == ddata.mouse_unk0e)
  {
    if (ddata.v1e0224)
    {
      ddata.v1e0224 = false;
      SKW_FIRE_MOUSE_RELEASE_CAPTURE();
      ddata.mouse_visibility = con(0x1);
      SKW_FIRE_SHOW_MOUSE_CURSOR();
    }
  }
  SKW_DROP_PLAYER_ITEMS(eaxw);
  vw_00 = signedword(heroptrrg5->b1d);
  x16 wordrg40 = SKW_ALLOC_NEW_RECORD(con(0x800a));
  if (wordrg40 != con(0xffffffff))
  {
    recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg40);
    recoptrrg1->u2.b.b0 |= con(0xffffff80);
    recoptrrg1->u2.w &= con(0x3f80);
    recoptrrg1->u2.w |= (eaxw & con(0x3)) << con(0xe);
    wordrg40 &= con(0x3fff);
    SKW_MOVE_RECORD_TO((vw_00 << con(0xe)) | wordrg40, con(0x0), ddata.v1e0270, con(0xffffffff), ddata.v1e0272);
  }
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  heroptrrg5->b1e = con(0x0);
  heroptrrg5->t22[con(0x0)] = con(0x0);
  heroptrrg5->b1c = CUTX8(ddata.v1e0258);
  heroptrrg5->t22[0x7] = con(0x0);
  if ((((vw_00 + con(0x4) - ddata.v1e0258) & con(0x3)) + con(0x1)) == ddata.v1e00b8)
    SKW_CHAMPION_SQUAD_RECOMPUTE_POSITION();
  if (heroptrrg5->b1f != con(0x0))
    SKW_2c1d_1bb5(eaxw, con(0x2710));
  ui16 uwordrg4 = con(0x0);
  while (uwordrg4 < ddata.savegamew2)
  {
    if (ddata.hero[uwordrg4].w36 != con(0x0))
      break;
    uwordrg4++;
  }
  if (uwordrg4 != ddata.savegamew2)
  {
    if (eaxw == ddata.mouse_unk0e)
      SKW_SELECT_CHAMPION_LEADER(uwordrg4);
  }
  else
  {
    ddata.v1e0250 = true;
    SKW_1031_098e();
    ddata.v1e0238 = false;
    ddata.v1e13f0 = false;
  }
  R_107B0();
  SKW_FIRE_SHOW_MOUSE_CURSOR();
}

// was SKW_2c1d_1b0f
void SKW_PROCESS_POISON(x16 eaxw, x16 edxw)
{
  c_hero* heroptrrg5;
  c_tim vtim_04;

  x16 wordrg6 = edxw;
  if ((eaxw == con(0xffffffff)) || ((eaxw + con(0x1)) == ddata.v1e0288))
    return;
  heroptrrg5 = &ddata.hero[eaxw];
  SKW_WOUND_PLAYER(eaxw, con(0x0), con(0x0), DM2_MAX(con(0x1), (wordrg6 + con(0x1e)) >> con(0x6)));
  heroptrrg5->u32.b.b1 |= con(0x28);
  if (--wordrg6 == con(0x0))
    return;
  if ((signedlong(heroptrrg5->w48) + unsignedlong(wordrg6)) > con(0xc00))
  {
    wordrg6 = con(0xc00) - heroptrrg5->w48;
    if (wordrg6 == con(0x0))
      return;
  }
  heroptrrg5->w48 += wordrg6;
  heroptrrg5->b1f += con(0x1);
  vtim_04.b_04 = con(0x4b);
  vtim_04.b_05 = CUTX8(eaxw);
  vtim_04.un.l_00 = (ddata.longmallocx + con(0x24)) | (signedlong(ddata.v1e0266) << con(0x18));
  vtim_04.u.w_06 = wordrg6;
  SKW_QUEUE_TIMER(&vtim_04);
}

// TODO: eaxw is requested as byte only
void SKW_INVOKE_MESSAGE(x16 eaxw, x8 ebxb, ui16 ecxuw, x16 edxw, x32 argl0)
{
  c_tim vtim_00;

  vtim_00.un.l_00 = argl0 | (signedlong(ddata.v1d3248) << con(0x18));
  vtim_00.b_04 = con(0x4);
  if (ecxuw < con(0x1))
  {
    if (ecxuw == con(0x0))
      vtim_00.b_05 = con(0x1);
  }
  else if (ecxuw <= con(0x1))
    vtim_00.b_05 = con(0x3);
  else if (ecxuw == con(0x2))
    vtim_00.b_05 = con(0x2);
  vtim_00.u.b.b_06 = CUTX8(eaxw);
  vtim_00.u.b.b_07 = CUTX8(edxw);
  vtim_00.v.b.b_08 = ebxb;
  vtim_00.v.b.b_09 = CUTX8(ecxuw);
  SKW_QUEUE_TIMER(&vtim_00);
}

bool SKW_32cb_35c1(x16* eaxpw, x16 ebxw, x16 ecxw, x16* edxpw)
{
  x16 wordrg5 = ebxw;
  x16 wordrg1 = ecxw;
  x16 wordrg3 = *eaxpw;
  x16 wordrg2 = *edxpw;
  if (wordrg1 != con(0x0))
  {
    wordrg1 *= con(0x5);
    wordrg2 -= wordrg1;
    if (wordrg2 >= con(0x0))
    {
      if (wordrg2 > con(0x18))
      {
        wordrg3 = signedword(table1d6a74[wordrg3].barr_00[3]);
        wordrg2 -= con(0x14);
      }
    }
    else
    {
      wordrg3 = signedword(table1d6a74[wordrg3].barr_00[2]);
      wordrg2 += con(0x14);
    }
    if (wordrg3 < con(0x0))
      return false;
  }
  if (wordrg5 != con(0x0))
  {
    x16 wordrg1 = (wordrg2 % con(0x5)) + wordrg5;
    if (wordrg1 <= con(0x4))
    {
      if (wordrg1 >= con(0x0))
        wordrg2 += wordrg5;
      else
      {
        wordrg5 += con(0x4);
        wordrg2 += wordrg5;
        wordrg3 = signedword(table1d6a74[wordrg3].barr_00[0]);
      }
    }
    else
    {
      wordrg5 -= con(0x4);
      wordrg2 += wordrg5;
      wordrg3 = signedword(table1d6a74[wordrg3].barr_00[1]);
    }
    if (wordrg3 < con(0x0))
      return false;
  }
  *eaxpw = wordrg3;
  *edxpw = wordrg2;
  return true;
}

void SKW_SHOW_ATTACK_RESULT(ui16 eaxuw)
{
  x8 byterg5;
  c_c5rects* sptrrg3;
  c_image vs_00;
  t_text t_13c[0x8];
  x8 vb_144; // inc8
  bool skip = false; // added this flag

  SKW_29ee_00a3(true);
  vb_144 = con(0x4);
  x8 byterg1 = con(0x1);
  if (!(eaxuw & 0x8000)) // BUGFIX 5.1/2014
    skip = true;
  else if (eaxuw < con(0xfffe))
  {
    if (eaxuw == con(0xfffa))
    {
      byterg1 = con(0x19);
      skip = true;
    }
  }
  else if (eaxuw == con(0xfffe))
  {
    byterg1 = con(0x17);
    skip = true;
  }
  else if (eaxuw == con(0xffff))
  {
    byterg1 = con(0x18);
    skip = true;
  }
  if (!skip)
  {
    vb_144 = con(0x5);
    if (eaxuw != con(0xfffd))
    {
      if (eaxuw != con(0xfffc))
        byterg1 = con(0xd);
      else
        byterg1 = con(0xc);
    }
    else
      byterg1 = con(0xe);
  }
  SKW_QUERY_GDAT_SUMMARY_IMAGE(&vs_00, vb_144, byterg1, con(0x1));
  if (!(eaxuw & 0x8000) && eaxuw <= con(0x28)) // BUGFIX 5.1/2014
  {
    if (eaxuw <= con(0xf))
      vs_00.w_34 = con(0x1f);
    else
      vs_00.w_34 = con(0x2f);
    vs_00.w_36 = con(0x35);
  }
  SKW_0b36_11c0(SKW_QUERY_PICST_IT(&vs_00), con(0x39), con(0xffffffff), &ddata.v1d694c);
  if (!(eaxuw & 0x8000)) // BUGFIX 5.1/2014
  {
    x16 uwordrg5 = eaxuw;
    x16 wordrg2 = con(0x5);
    t_13c[0x5] = con(0x0);
    x16 wordrg3;
    do
    {
      x16 wordrg11 = signedlong(uwordrg5);
      wordrg11 /= con(0xa);
      wordrg3 = --wordrg2;
      t_13c[wordrg3] = CUTX8((uwordrg5 % con(0xa)) + con(0x30));
      wordrg11 /= con(0xa);
      uwordrg5 = wordrg11;
    } while (uwordrg5 != con(0x0));
    SKW_DRAW_SIMPLE_STR(&ddata.v1d694c, unsignedword(ddata.paletteptr1[con(0x4)]), unsignedword(ddata.paletteptr1[con(0x0)]), con(0x39), t_13c + wordrg3);
    return;
  }
  if (eaxuw != con(0xfffe))
    return;
  sptrrg3 = &ddata.v1d694c;
  SKW_DRAW_ICON_PICT_ENTRY(con(0x7), ddata.v1e0b72 != con(0x0) ? con(0x34) : con(0x38), sptrrg3, con(0x0), con(0x7e), con(0xffff));
  vb_144 = con(0x0);
  for(;;)
  {
    if (UI8CAST(vb_144) >= UI8CAST(con(0xffffffff)))
      return;
    byterg5 = vb_144;
    x16 wordrg1 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x10), con(0xb), con(0x5), byterg5);
    if (wordrg1 != con(0x0))
      if ((wordrg1 & con(0x8000)) == con(0x0) && (ddata.v1e0b78 & wordrg1) != con(0x0))
      {
        SKW_DRAW_ICON_PICT_ENTRY(con(0x10), con(0x18), &ddata.v1d694c, byterg5, con(0x7d), con(0xffff));
        return;
      }
    vb_144++;
  }
}

void SKW_CLICK_MONEYBOX(x16 eaxw)
{
  bool skip = false; // added

  if (ddata.v1d67bc != con(0x4))
    return;
  x16 wordrg3 = ddata.v1e0b74 - con(0x1);
  x16 wordrg1 = ddata.hero[wordrg3].warrc3[ddata.v1e0b76];
  if (ddata.savegamewpc.w0 == con(0xffffffff))
  {
    x16 wordrg2 = wordrg1;
    x16 wordrg11 = SKW_TAKE_COIN_FROM_WALLET(wordrg2, SKW_GET_ITEM_ORDER_IN_CONTAINER(wordrg2, eaxw));
    if (wordrg11 == con(0xffffffff))
      skip = true;
    else
      SKW_TAKE_OBJECT(wordrg11, false);
  }
  else
  {
    if (!SKW_ADD_COIN_TO_WALLET(wordrg1, ddata.savegamewpc.w0))
      skip = true;
    else
      SKW_REMOVE_OBJECT_FROM_HAND();
  }
  if (!skip)
  {
    ddata.v1e0b6c = con(0x1);
    SKW_UPDATE_RIGHT_PANEL(false);
  }
  SKW_CALC_PLAYER_WEIGHT(wordrg3);
}

// was SKW_29ee_058d
void SKW_DRAW_SQUAD_SPELL_AND_LEADER_ICON(x16 eaxw, bool edxbool)
{
  c_image vs_00;
  c_rect rc_13c;
  x8 vb_148;
  bool skip = false; // added

  SKW_29ee_00a3(false);
  x16 wordrg5 = (unsignedword(ddata.hero[eaxw].b1d) + con(0x4) - ddata.v1e0258) & con(0x3);
  SKW_FILL_RECT_SUMMARY(&ddata.v1d694c, ddata.paletteptr1[0], SKW_QUERY_EXPANDED_RECT(wordrg5 + con(0x4f), &rc_13c));
  if (ddata.hero[eaxw].w36 == con(0x0))
    return;
  x16 vw_144;
  if (wordrg5 != con(0x1) && wordrg5 != con(0x2))
    vw_144 = con(0x0);
  else
    vw_144 = con(0x1);
  x8 byterg1;
  if (wordrg5 > con(0x1))
  {
    vb_148 = con(0xc);
    byterg1 = con(0x8);
  }
  else
  {
    vb_148 = con(0xa);
    byterg1 = con(0x6);
  }
  if (eaxw == ddata.mouse_unk0e)
    vb_148++;
  if (edxbool)
    byterg1++;
  SKW_QUERY_GDAT_SUMMARY_IMAGE(&vs_00, con(0x4), byterg1, con(0x1));
  vs_00.w_32 = vw_144;
  SKW_0b36_11c0(SKW_QUERY_PICST_IT(&vs_00), wordrg5 + con(0x57), con(0x4), &ddata.v1d694c);
  if (!ddata.v1e0238)
    if (ddata.hero[eaxw].b2a[2] == con(0x0))
      skip = true;
  if (!skip)
  {
    vs_00.rc_24.x += ddata.v1d694c.s_00.rc_02.x;
    vs_00.rc_24.y += ddata.v1d694c.s_00.rc_02.y;
    SKW_0b36_105b(&ddata.v1d694c, con(0x0), &vs_00.rc_24);
  }
  SKW_QUERY_GDAT_SUMMARY_IMAGE(&vs_00, con(0x4), vb_148, con(0x1));
  vs_00.w_32 = vw_144;
  wordrg5 += con(0x53);
  SKW_0b36_11c0(SKW_QUERY_PICST_IT(&vs_00), wordrg5, con(0x4), &ddata.v1d694c);
  if (ddata.v1e0238)
  {
    vs_00.rc_24.x += ddata.v1d694c.s_00.rc_02.x;
    vs_00.rc_24.y += ddata.v1d694c.s_00.rc_02.y;
    SKW_0b36_105b(&ddata.v1d694c, con(0x0), &vs_00.rc_24);
  }
}

// TODO: new name: DM2_HANDLE_LOADGAME_BOX
x16 SKW_1031_030a(s_bbw* eaxpbbw, x16 ebxw, x16 ecxw, x16 edxw)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("SKW_1031_030a/DM2_HANDLE_LOADGAME_BOX");
SPX_DEBUG_MESSAGE_INFO("Rects: %08x / Mouse info : X:%03d Y:%03d B:%02d\n", eaxpbbw, edxw, ebxw, ecxw);
  x8 byterg5;
  x8* bptrrg6;
  s_bbw* sbbwptrrg5;
  x16 vw_04;
  x16 vw_08;
  s_bbw* pbbw_0c;
  c_rect rc_10;

  pbbw_0c = eaxpbbw;
  vw_08 = edxw;	// mouse-x
  vw_04 = ebxw;	// mouse-y
  x16 wordrg3 = con(0x0);
  bptrrg6 = SKW_1031_023b(eaxpbbw);
  do
  {
    bool skip = false; // added
    bool boolrg1;
    byterg5 = *bptrrg6;
    byterg5 &= con(0xffffff7f);
    sbbwptrrg5 = &table1d3ba0[byterg5];
    //switch (pbbw_0c->b0 & con(0x7f))
	switch ( (pbbw_0c->b0+0x80) & con(0x7f))
    {
      case 0:
        boolrg1 = SKW_RETURN_1();
      break;

      case 1:
        boolrg1 = SKW_IS_GAME_ENDED(sbbwptrrg5);
      break;

      case 2:
        boolrg1 = SKW_1031_0023(sbbwptrrg5);
      break;

      case 3:
        boolrg1 = SKW_1031_003e(sbbwptrrg5);
      break;

      case 4:
        boolrg1 = SKW_1031_007b(sbbwptrrg5);
      break;

      case 5:
        boolrg1 = SKW_1031_009e(sbbwptrrg5);
      break;

      case 6:
        boolrg1 = SKW_1031_00c5(sbbwptrrg5);
      break;

      case 7:
        boolrg1 = SKW_1031_00f3(sbbwptrrg5);
      break;

      case 8:
        boolrg1 = SKW_1031_012d(sbbwptrrg5);
      break;

      case 9:
        boolrg1 = SKW_1031_014f(sbbwptrrg5);
      break;

      case 10:
        boolrg1 = SKW_1031_0184(sbbwptrrg5);
      break;

      case 11:
        boolrg1 = SKW_1031_01ba(sbbwptrrg5);
      break;

      default: throw(THROW_DMABORT);
    }
    if (boolrg1)
    {
      x16 wordrg1;
      if ((sbbwptrrg5->b0 & con(0x80)) != con(0x80))
      {
        if (!SKW_PT_IN_RECT(SKW_1031_01d5(table1d3d23[sbbwptrrg5->w2].w0, &rc_10), vw_08, vw_04))
          skip = true;
        else
        {
          wordrg3 = ecxw;
          wordrg1 = SKW_1031_0a88(SKW_1031_024c(sbbwptrrg5), vw_04, wordrg3, vw_08);
        }
      }
      else
      {
        wordrg3 = ecxw;
        wordrg1 = SKW_1031_030a(sbbwptrrg5, vw_04, wordrg3, vw_08);
      }
      if (!skip)
      {
        wordrg3 = wordrg1;
        if (wordrg1 != con(0x0))
		{
SPX_DEBUG_POP
          return wordrg3;
		}
      }
    }
  } while ((*(++bptrrg6) & con(0xffffff80)) == con(0x0));
SPX_DEBUG_POP
  return wordrg3;
}

void SKW_1031_027e(s_bbw* eaxpbbw)
{
  s_bbw* sbbwptrrg4;
  x8* bptrrg2;

  bptrrg2 = SKW_1031_023b(eaxpbbw);
  do
  {
    x8 byterg1;
    sbbwptrrg4 = &table1d3ba0[*bptrrg2 & con(0x7f)];
    switch (eaxpbbw->b0 & con(0x7f))
    {
      case 0:
        byterg1 = SKW_RETURN_1();
      break;

      case 1:
        byterg1 = SKW_IS_GAME_ENDED(sbbwptrrg4);
      break;

      case 2:
        byterg1 = SKW_1031_0023(sbbwptrrg4);
      break;

      case 3:
        byterg1 = SKW_1031_003e(sbbwptrrg4);
      break;

      case 4:
        byterg1 = SKW_1031_007b(sbbwptrrg4);
      break;

      case 5:
        byterg1 = SKW_1031_009e(sbbwptrrg4);
      break;

      case 6:
        byterg1 = SKW_1031_00c5(sbbwptrrg4);
      break;

      case 7:
        byterg1 = SKW_1031_00f3(sbbwptrrg4);
      break;

      case 8:
        byterg1 = SKW_1031_012d(sbbwptrrg4);
      break;

      case 9:
        byterg1 = SKW_1031_014f(sbbwptrrg4);
      break;

      case 10:
        byterg1 = SKW_1031_0184(sbbwptrrg4);
      break;

      case 11:
        byterg1 = SKW_1031_01ba(sbbwptrrg4);
      break;

      default: throw(THROW_DMABORT);
    }
    if (byterg1)
    {
      if ((sbbwptrrg4->b0 & con(0xffffff80)) != con(0xffffff80)) // BUGFIX 4/2014 CRITICAL: don't compare with con(0x80) because left side already long (con is 32bit, so expands the expression!)
        table1d3d23[sbbwptrrg4->w2].b6 |= con(0x40);
      else
        SKW_1031_027e(sbbwptrrg4);
    }
    bptrrg2++;
  } while ((*bptrrg2 & con(0xffffff80)) == con(0x0));
}

x8 SKW_QUERY_CLS2_OF_TEXT_RECORD(x16 eaxw)
{
  c_record* recoptrrg4;

  recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(eaxw);
  if ((recoptrrg4->u2.w & con(0x6)) != con(0x2))
    return con(0xffffffff);
  ui16 uwordrg1 = urshift(recoptrrg4->u2.w, con(0xb)) & con(0x1f);
  if (uwordrg1 < con(0x2))
  {
    if (uwordrg1 != con(0x0))
      return con(0xffffffff);
  }
  else if (uwordrg1 > con(0x2))
  {
    if (uwordrg1 < con(0x5))
      return con(0xffffffff);
    if (uwordrg1 > con(0x5))
      if (uwordrg1 != con(0xd))
        return con(0xffffffff);
  }
  return CUTX8(urshift(recoptrrg4->u2.w, con(0x3)));
}

bool SKW_IS_CREATURE_ALLOWED_ON_LEVEL(x16 eaxw, x16 edxw)
{
  s_size10* s10ptrrg2;
  x8* bptrrg4;

  if ((SKW_QUERY_CREATURE_AI_SPEC_FLAGS(eaxw) & con(0x4000)) != con(0x0))
    return true;
  x8 byterg3 = SKW_QUERY_CLS2_FROM_RECORD(eaxw);
  s10ptrrg2 = &ddata.v1e03c8[edxw];
  x8*** helpptr1 = &ddata.v1e03cc[edxw];
  x8** helpptr2 = *helpptr1 + ulrshift(s10ptrrg2->w8, con(0x5), con(0xb));
  x8* helpptr3 = *helpptr2 + urshift(s10ptrrg2->w8, con(0xb)); // NOTE: offset gets full added! no / 4; ofc, it's a x8* now, no more x8** - dangerous!
  bptrrg4 = helpptr3 + con(0x1);
  x16 wordrg1 = ulrshift(s10ptrrg2->wc, con(0x8), con(0xc));
  for(;;)
  {
    if (wordrg1 <= con(0x0))
      return false;
    if (byterg3 == *bptrrg4++)
      return true;
    wordrg1--;
  }
}

x16 SKW_QUERY_ITEM_VALUE(x16 eaxw, x8 edxb)
{
  c_record* recoptrrg2;
  x8 vb_04;

  vb_04 = edxb;
  if (eaxw == con(0xffffffff))
    return con(0x0);
  x16 wordrg3 = eaxw;
  x16 wordrg5 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(wordrg3, vb_04);
  if (vb_04 == con(0x1))
  {
    x16 wordrg61 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(wordrg3, con(0x34));
    if (wordrg61 > con(0x0))
      wordrg5 += SKW_ADD_ITEM_CHARGE(wordrg3, con(0x0)) * wordrg61;
  }
  if (vb_04 == con(0x2))
  {
    x16 wordrg62 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(eaxw, con(0x35));
    if (wordrg62 > con(0x0))
      wordrg5 += SKW_ADD_ITEM_CHARGE(eaxw, con(0x0)) * wordrg62;
    if (urshift(eaxw & con(0x3c00), con(0xa)) == con(0x8))
    {
      if (wordrg5 > con(0x1))
      {
        wordrg5 >>= con(0x1);
        wordrg5 += unsignedword(CUTX8(SKW_GET_ADDRESS_OF_RECORD(eaxw)->u2.w)) * wordrg5 / con(0xff);
      }
    }
  }
  if (urshift(eaxw & con(0x3c00), con(0xa)) != con(0x9) || (recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(eaxw), (recoptrrg2->u4.b.b0 & con(0x6)) != con(0x0)))
    return wordrg5;
  bool vbool_00 = SKW_IS_CONTAINER_MONEYBOX(eaxw);
  x16 wordrg2 = recoptrrg2->u2.w;
  x16 wordrg63 = con(0x0);
  for (;; wordrg2 = SKW_GET_NEXT_RECORD_LINK(wordrg2))
  {
    if (wordrg2 == con(0xfffffffe))
    {
      if (vbool_00)
      {
        if (vb_04 != con(0x1))
          wordrg5 += wordrg63;
        else
          wordrg5 += (wordrg63 + con(0x4)) / con(0x5);
      }
      return wordrg5;
    }
    if (!vbool_00 || (urshift(wordrg2 & con(0x3c00), con(0xa)) != con(0xa)))
      wordrg5 += SKW_QUERY_ITEM_VALUE(wordrg2, vb_04);
    else
      wordrg63 += SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(wordrg2, vb_04) * (ulrshift(SKW_GET_ADDRESS_OF_RECORD(wordrg2)->u2.w, con(0x2), con(0xa)) + con(0x1));
  }
}

x16 SKW_ADD_ITEM_CHARGE(x16 eaxw, x16 edxw)
{
  ui16 uwordrg5;
  c_record* recoptrrg1;
  c_record* recoptrrg3;

  uwordrg5 = eaxw;
  if (uwordrg5 == con(0xffffffff))
    return con(0x0);
  recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(uwordrg5);
  recoptrrg3 = recoptrrg1;
  uwordrg5 &= con(0x3c00);
  uwordrg5 = urshift(uwordrg5, con(0xa));
  x16 wordrg40 = con(0x0);
  x16 wordrg1;
  if (uwordrg5 < UI16CAST(con(0x6)))
  {
    if (uwordrg5 != con(0x5))
      return con(0x0);
    wordrg40 = urshift(4 * recoptrrg1->u2.w, con(0xc));
    wordrg1 = con(0xf);
  }
  else if (uwordrg5 <= UI16CAST(con(0x6)))
  {
    wordrg40 = urshift(8 * recoptrrg1->u2.w, con(0xc));
    wordrg1 = con(0xf);
  }
  else if (uwordrg5 == con(0xa))
  {
    wordrg40 = urshift(recoptrrg1->u2.w, con(0xe));
    wordrg1 = con(0x3);
  }
  else
    return wordrg40;
  wordrg40 += edxw;
  x16 wordrg4 = DM2_BETWEEN_VALUE(con(0x0), wordrg1, wordrg40);
  x16 wordrg21 = wordrg4 & con(0xf);
  if (uwordrg5 < UI16CAST(con(0x6)))
  {
    if (uwordrg5 == con(0x5))
    {
      recoptrrg3->u2.b.b1 &= con(0xffffffc3);
      wordrg21 <<= con(0xa);
      recoptrrg3->u2.w |= wordrg21;
    }
  }
  else if (uwordrg5 <= UI16CAST(con(0x6)))
  {
    recoptrrg3->u2.b.b1 &= con(0xffffffe1);
    wordrg21 <<= con(0x9);
    recoptrrg3->u2.w |= wordrg21;
  }
  else if (uwordrg5 == con(0xa))
  {
    recoptrrg3->u2.b.b1 &= con(0x3f);
    recoptrrg3->u2.w |= (wordrg4 & con(0x3)) << con(0xe);
  }
  return wordrg4;
}

bool SKW_ADD_COIN_TO_WALLET(x16 eaxw, x16 edxw)
{
  c_record* recoptrrg1;
  c_record* recoptrrg2;
  c_record* recoptrrg7;

  if (!SKW_IS_CONTAINER_MONEYBOX(eaxw))
    return false;
  if (!SKW_IS_MISCITEM_CURRENCY(edxw))
    return false;
  recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(edxw);
  recoptrrg7 = SKW_GET_ADDRESS_OF_RECORD(eaxw);
  x16 wordrg4 = recoptrrg7->u2.w;
  while (wordrg4 != con(0xfffffffe))
  {
    x16 wordrg1 = urshift(wordrg4 & con(0x3c00), con(0xa));
    if (wordrg1 != con(0xa))
      return wordrg1 != con(0x0);
    recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg4);
    x16 wordrg3 = recoptrrg1->u2.w & con(0x7f);
    if (wordrg3 == (recoptrrg2->u2.w & con(0x7f)) && ((recoptrrg1->u2.w & con(0x3f00)) < UI16CAST(con(0x3f00))))
    {
      x16 wordrg31 = ((urshift(4 * recoptrrg1->u2.w, con(0xa)) + con(0x1)) & con(0x3f)) << con(0x8);
      recoptrrg1->u2.b.b1 &= con(0xffffffc0);
      recoptrrg1->u2.w |= wordrg31;
      SKW_DEALLOC_RECORD(edxw);
      break;
    }
    wordrg4 = SKW_GET_NEXT_RECORD_LINK(wordrg4);
  }
  if (wordrg4 == con(0xfffffffe))
  {
    recoptrrg2->u2.b.b1 &= con(0xffffffc0);
    SKW_APPEND_RECORD_TO(edxw, con(0xffffffff), con(0x0), &recoptrrg7->u2.w);
  }
  return true;
}

// edxp is a wordarray - pointer, 0xa words
void SKW_COUNT_BY_COIN_TYPES(x16 eaxw, x16* edxpwarr)
{
  DM2_ZERO_MEMORY(DOWNCAST(x16, edxpwarr), con(0x14));
  for (x16 wordrg5 = SKW_GET_ADDRESS_OF_RECORD(eaxw)->u2.w; wordrg5 != con(0xfffffffe); wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg5))
  {
    if (SKW_IS_MISCITEM_CURRENCY(wordrg5))
    {
      x16 wordrg6 = SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg5);
      x16 wordrg2 = con(0x0);
      while (wordrg2 < ddata.v1e03fe)
      {
        if (wordrg6 == ddata.v1e0394[wordrg2])
          edxpwarr[wordrg2] = edxpwarr[wordrg2] + urshift(4 * SKW_GET_ADDRESS_OF_RECORD(wordrg5)->u2.w, con(0xa)) + con(0x1);
        wordrg2++;
      }
    }
  }
}

void SKW_0cee_185a(s_8wordarray* eaxps, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2, x16 argw3)
{
  x16* wptrrg4;
  x16 vw_04;
  x16 vw_08;

  vw_04 = ebxw;
  vw_08 = ecxw;
  x16 wordrg7 = ulrshift(ddata.v1e03c0->wa, con(0x8), con(0xc));
  argw3++;
  eaxps->warr_00[con(0x3)] = unsignedword(SKW_0cee_1815(edxw, argw2, ((argw1 & con(0x3)) + con(0x1)) * argw3, wordrg7, con(0x1e)));
  eaxps->warr_00[con(0x4)] = unsignedword(SKW_0cee_1815(vw_04, argw2, ((++argw1 & con(0x3)) + con(0x1)) * argw3, wordrg7, con(0x1e)));
  eaxps->warr_00[con(0x5)] = unsignedword(SKW_0cee_1815(vw_08, argw2, ((++argw1 & con(0x3)) + con(0x1)) * argw3, wordrg7, con(0x1e)));
  eaxps->warr_00[con(0x6)] = unsignedword(SKW_0cee_1815(argw0, argw2, ((++argw1 & con(0x3)) + con(0x1)) * argw3, wordrg7, con(0x1e)));
  argw3--;
  if (argw2 < con(0x0) || argw2 >= ddata.savegamewa)
  {
    if (argw3 >= con(0x0) && argw3 < ddata.savegamewb)
      return;
  }
  x16 wordrg2 = con(0x3);
  for(;;)
  {
    if (wordrg2 > con(0x6))
      return;
    wptrrg4 = &eaxps->warr_00[wordrg2];
    if (SKW_IS_WALL_ORNATE_ALCOVE(CUTX8(*wptrrg4)) != con(0x0))
      *wptrrg4 = con(0xff);
    wordrg2++;
  }
}

x16 SKW_ALLOC_NEW_RECORD(x16 eaxw)
{
  x16* wptrrg2;

  x16 wordrg5 = eaxw;
  x16 wordrg6 = ddata.savegamep4->w0[(eaxw & con(0x7fff)) + con(0x6)];
  if (wordrg5 != con(0xffff800a))
  {
    if (wordrg5 == con(0xa))
      wordrg6 -= con(0x3);
  }
  else
    wordrg5 = con(0xa);
  x16 wordrg1 = wordrg6;
  x16 wordrg3 = table1d280c[wordrg5];
  c_record* recoptr = ddata.savegameparr5[wordrg5];
  wptrrg2 = &recoptr->w0;
  for(;;) // TODO: ugly loopform
  {
    if (*wptrrg2 != con(0xffffffff))
    {
      if (--wordrg1 != con(0x0))
      {
        wordrg3 &= con(0xfffe);
        wptrrg2 += wordrg3 / 2;
        continue;
      }
      x16 wordrg11 = SKW_RECYCLE_A_RECORD_FROM_THE_WORLD(wordrg5, con(0xff));
      wordrg6 = wordrg11;
      if (wordrg6 == con(0xffffffff))
        return wordrg11;
      wptrrg2 = UPCAST(x16, DOWNCAST(c_record, SKW_GET_ADDRESS_OF_RECORD(wordrg11)));
    }
    else
    {
      wordrg6 -= wordrg1;
      wordrg6 |= wordrg5 << con(0xa);
    }
    DM2_ZERO_MEMORY(DOWNCAST(x16, wptrrg2), signedlong(wordrg3));
    *wptrrg2 = con(0xfffffffe);
    wordrg1 = wordrg5;
    if (wordrg1 == con(0x9))
      *(wptrrg2 + 1) = con(0xfffffffe);
    return wordrg6;
  }
}

void SKW_APPEND_RECORD_TO(x16 eaxw, x16 ebxw, x16 ecxw, x16* edxpw)
{
  x8* bptrrg2;
  x16* wptrrg1;
  x16* wptrrg31;
  x16* wptrrg32;

  x16 wordrg5 = ebxw;
  if (eaxw == con(0xfffffffe) || eaxw == con(0xffffffff))
    return;
  SKW_GET_ADDRESS_OF_RECORD(eaxw)->w0 = con(0xfffffffe);
  if (ebxw < con(0x0))
  {
    wptrrg1 = edxpw;
    for(;;)
    {
      wordrg5 = *wptrrg1;
      if (wordrg5 == con(0xfffffffe))
        break;
      wptrrg1 = &SKW_GET_ADDRESS_OF_RECORD(wordrg5)->w0;
    }
  }
  else
  {
    bptrrg2 = ddata.savegamepp1[ebxw] + ecxw;
    if ((*bptrrg2 & con(0x10)) != con(0x0))
    {
      x16 wordrg16 = SKW_GET_TILE_RECORD_LINK(ebxw, ecxw);
      x16 wordrg401;
      do
      {
        wordrg401 = wordrg16;
        wordrg16 = SKW_GET_NEXT_RECORD_LINK(wordrg16);
      } while (wordrg16 != con(0xfffffffe));
      wptrrg1 = &SKW_GET_ADDRESS_OF_RECORD(wordrg401)->w0;
    }
    else
    {
      if (ddata.v1e038c[(2 * ddata.savegamep4->w0[0x5] + con(0xfffffffe)) / 2] != con(0xffffffff))
        SKW_RAISE_SYSERR(con(0x48));
      *bptrrg2 |= con(0x10);
      wptrrg31 = &ddata.v1e03f4[wordrg5 + con(0x1)];
      x16 wordrg14 = ddata.v1e0408 - ddata.v1e03e4[ddata.v1d3248] - wordrg5 - con(0x1);
      for(;;)
      {
        if (--wordrg14 == con(0xffffffff))
          break;
        *wptrrg31++ += con(0x1);
      }
      x16 wordrg402 = con(0x0);
      bptrrg2 -= signedlong(ecxw);
      x16 wordrg13 = ddata.v1e03f4[wordrg5];
      for(;;)
      {
        if (wordrg402++ == ecxw)
          break;
        if ((*bptrrg2++ & con(0x10)) != con(0x0))
          wordrg13++;
      }
      wptrrg32 = &ddata.v1e038c[wordrg13];
      SKW_COPY_MEMORY(DOWNCAST(x16, wptrrg32), UI32CAST(2 * (ddata.savegamep4->w0[con(0x5)] - wordrg13 - con(0x1))), DOWNCAST(x16, (wptrrg32 + con(0x1))));
      *wptrrg32 = eaxw;
      return;
    }
  }
  *wptrrg1 = eaxw;
}

void SKW_CUT_RECORD_FROM(x16 eaxw, x16 ebxw, x16 ecxw, x16* edxpw)
{
  x16* wptrrg1;
  x16* wptrrg4;
  x16* wptrrg41;
  c_record* recoptrrg3;
  c_record* recoptrrg2;
  bool skip = false; // added

  x16 vw_00 = eaxw;
  x16 wordrg5 = ebxw;
  if (eaxw == con(0xfffffffe) || eaxw == con(0xffffffff))
    return;
  vw_00 &= con(0x3fff);
  x16 vw_08 = vw_00;
  recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(vw_00);
  recoptrrg3 = recoptrrg2;
  if (wordrg5 < con(0x0))
    wptrrg4 = edxpw;
  else
  {
    x16 wordrg1 = SKW_GET_OBJECT_INDEX_FROM_TILE(wordrg5, ecxw);
    x16 vw_0c = wordrg1;
    wptrrg1 = &ddata.v1e038c[wordrg1];
    if (recoptrrg2->w0 != con(0xfffffffe) || ((*wptrrg1 & con(0x3fff)) != vw_08))
    {
      if ((*wptrrg1 & con(0x3fff)) != vw_00)
        wptrrg4 = wptrrg1;
      else
      {
        *wptrrg1 = recoptrrg3->w0;
        skip = true;
      }
    }
    else
    {
      ddata.savegamepp1[wordrg5][ecxw] &= con(0xef);
      x16 wordrg402 = ddata.savegamep4->w0[5] - con(0x1);
      x16 vw_04 = wordrg402;
      wordrg402 -= vw_0c;
      wordrg402 = 2 * wordrg402;
      wptrrg4 = wptrrg1++;
      SKW_COPY_MEMORY(DOWNCAST(x16, wptrrg1), unsignedlong(wordrg402), DOWNCAST(x16, wptrrg4));
      ddata.v1e038c[vw_04] = con(0xffffffff);
      wptrrg1 = &ddata.v1e03f4[wordrg5 + con(0x1)];
      wordrg5 += ddata.v1e03e4[ddata.v1d3248];
      x16 wordrg2 = ddata.v1e0408 - wordrg5 - con(0x1);
      for(;;)
      {
        if (--wordrg2 == con(0xffffffff))
          break;
        wptrrg41 = wptrrg1++;
        *wptrrg41 -= con(0x1);
      }
      skip = true;
    }
  }
  if (!skip)
  {
    for(;;)
    {
      if ((*wptrrg4 == con(0xfffffffe)) || (*wptrrg4 & con(0x3fff)) == vw_00)
      {
        if (*wptrrg4 != con(0xfffffffe))
          *wptrrg4 = SKW_GET_NEXT_RECORD_LINK(*wptrrg4);
        break;
      }
      wptrrg1 = &SKW_GET_ADDRESS_OF_RECORD(*wptrrg4)->w0;
      wptrrg4 = wptrrg1;
    }
  }
  recoptrrg3->w0 = con(0xfffffffe);
}

c_record* R_D283(x16 eaxw, x16 edxw)
{
  c_record* recoptrrg7;
  c_5bytes vs_04;

  recoptrrg7 = NULL;
  x16 wordrg3 = eaxw;
  x8 byterg1 = SKW_GET_TILE_VALUE(wordrg3, edxw);
  if (urshift(unsignedword(byterg1), con(0x5)) != con(0x5))
    return recoptrrg7;
  byterg1 &= con(0x8);
  if (byterg1 == con(0x0))
    return recoptrrg7;
  if (!SKW_GET_TELEPORTER_DETAIL(&vs_04, edxw, eaxw + con(0x1)))
    if (!SKW_GET_TELEPORTER_DETAIL(&vs_04, edxw, eaxw - con(0x1)))
      if (!SKW_GET_TELEPORTER_DETAIL(&vs_04, edxw + con(0x1), wordrg3))
        if (!SKW_GET_TELEPORTER_DETAIL(&vs_04, edxw - con(0x1), wordrg3))
          return recoptrrg7;
  recoptrrg7 = SKW_GET_ADDRESS_OF_TILE_RECORD(eaxw, edxw);
  if (unsignedword(vs_04.b_04) != urshift(recoptrrg7->u4.w, con(0x8)))
    return NULL;
  if (SKW_CALC_SQUARE_DISTANCE(unsignedword(vs_04.b_02), recoptrrg7->u2.w & con(0x1f), ulrshift(recoptrrg7->u2.w, con(0x6), con(0xb)), unsignedword(vs_04.b_03)) == con(0x1))
    return recoptrrg7;
  return NULL;
}

x8 SKW_0cee_0897(x16** eaxppw, x16 ebxw, x16 edxw)
{
  if ((((SKW_GET_TILE_VALUE(edxw, ebxw) & con(0xff)) >> con(0x5)) & con(0xff)) != con(0x5))
    return con(0x0);
  x16 wordrg4 = SKW_GET_TILE_RECORD_LINK(edxw, ebxw);
  *eaxppw = &SKW_GET_ADDRESS_OF_RECORD(wordrg4)->w0;
  x16 wordrg2;
  do
  {
    wordrg4 = SKW_GET_NEXT_RECORD_LINK(wordrg4);
    if (wordrg4 == con(0xfffffffe))
      return con(0x0);
    wordrg2 = urshift(wordrg4 & con(0x3c00), con(0xa));
  } while ((wordrg2 != con(0x3)) || (SKW_GET_ADDRESS_OF_RECORD(wordrg4)->u2.w & con(0x7f)) != con(0x27));
  return CUTX8(((ulrshift(*((*eaxppw) + con(0x1)), con(0x4), con(0xe)) + con(0x2)) & wordrg2) + con(0x1));
}

// TODO: ecxw can be x8 only
// TODO: return could be bool (if so, vw_10 is too)
x16 SKW_0b36_1446(c_hex18* eaxph18, x16 ebxw, x16 ecxw, x16 edxw)
{
  t_gfxdata* gptrrg4;
  t_gfxdata* destbmp; // was gptrrg5
  t_gfxdata* gptrrg6;
  x16 vw_04;
  x16 srcofs; // was vw_08
  c_hex18* ph18_0c;
  t_gfxdata* pg_14;
  e_bitres vw_18;
  x16 vw_1c;
  x16 vw_20;

  ph18_0c = eaxph18;
  srcofs = edxw;
  vw_04 = ebxw;
  pg_14 = SKW_QUERY_PICT_BITMAP(eaxph18);
  if (pg_14 == NULL)
    return con(0xffffffff);
  vw_18 = e_bitres(ph18_0c->w_16);
  vw_1c = SKW_3e74_5888();
  SKW_ALLOC_NEW_PICT(vw_1c, con(0x1), CUTX8(vw_18), ((vw_18 == con(0x4)) ? 1 : 0) + con(0x1));
  vw_20 = SKW_3e74_5888();
  gptrrg6 = SKW_ALLOC_NEW_PICT(vw_20, con(0x1), CUTX8(vw_18), ((vw_18 == con(0x4)) ? 1 : 0) + con(0x1));
  destbmp = SKW_3e74_5817(vw_1c);
  SKW_FILL_ENTIRE_PICT(destbmp, CUTX8(ecxw));
  SKW_FILL_ENTIRE_PICT(gptrrg6, CUTX8(ecxw));
  s_gfxdatatail* ptrbefore = getprevious(pg_14);
//                        srcbmp                                                                         srcofs destbmp                                                                               srcbits destbits
  SKW_FIRE_BLIT_PICTURE(SKW_QUERY_PICT_BITMAP(ph18_0c), SKW_ALLOC_TEMP_ORIGIN_RECT(con(0x1), con(0x1)), srcofs, destbmp, vw_04, ptrbefore->width, ((vw_18 == con(0x4)) ? 1 : 0) + con(0x1), ecxw, con(0x0), vw_18, vw_18);
  x16 vw_10 = con(0x1);
  x16 wordrg32 = CUTX16(SKW_CALC_IMAGE_BYTE_LENGTH(destbmp));
  x16 wordrg1 = con(0x0);
  for(;;) // TODO: ugly loopform
  {
    if (wordrg1 < wordrg32)
    {
      gptrrg4 = gptrrg6++;
      if (*destbmp++ == *gptrrg4)
      {
        wordrg1++;
        continue;
      }
      vw_10 = con(0x0);
    }
    SKW_3e74_58a3(vw_1c);
    SKW_3e74_58a3(vw_20);
    return vw_10;
  }
}

x8 SKW_GET_TILE_VALUE(x16 eaxw, x16 edxw)
{
  x8 vb_00;
  x16 vw_04;

  x16 wordrg3 = edxw;
  if ((edxw < con(0x0)) || (wordrg3 >= ddata.savegamewb))
    vw_04 = 0x0;
  else
    vw_04 = 0x1;
  bool boolrg5;
  if ((eaxw < con(0x0)) || (eaxw >= ddata.savegamewa))
    boolrg5 = false;
  else
    boolrg5 = true;
  if (boolrg5 && vw_04 != con(0x0))
    return ddata.savegamepp1[eaxw][wordrg3];
  x16 wordrg2 = eaxw;
  x16 wordrg18 = eaxw - con(0x1);
  x16 wordrg4;
  if (vw_04 == con(0x0))
  {
    if (!boolrg5)
    {
      if (wordrg2 != con(0xffffffff))
      {
        if (wordrg2 != ddata.savegamewa)
          return con(0xffffffe0);
        wordrg2 = wordrg18;
      }
      else
        wordrg2 = con(0x0);
      if (wordrg3 == con(0xffffffff))
        if (SKW_IS_TILE_PASSAGE(wordrg2, con(0x0)))
          return con(0x0);
      if (wordrg3 != ddata.savegamewb)
        return con(0xffffffe0);
      wordrg3--;
      wordrg4 = wordrg3;
    }
    else
    {
      if (wordrg3 != con(0xffffffff))
      {
        if (wordrg3 != ddata.savegamewb)
          return con(0xffffffe0);
        vw_04 = con(0x8);
        wordrg3--;
      }
      else
      {
        wordrg3 = con(0x0);
        vw_04 = con(0x2);
      }
      if (SKW_IS_TILE_PASSAGE(wordrg2, wordrg3))
        return CUTX8(vw_04);
      if (wordrg2 > con(0x0))
        if (SKW_IS_TILE_PASSAGE(wordrg2 - con(0x1), wordrg3))
          return con(0x0);
      if ((wordrg2 + con(0x1)) >= ddata.savegamewa)
        return con(0xffffffe0);
      wordrg4 = wordrg3;
      wordrg2++;
    }
  }
  else
  {
    if (wordrg2 != con(0xffffffff))
    {
      if (wordrg2 != ddata.savegamewa)
        return con(0xffffffe0);
      vb_00 = con(0x1);
      wordrg2 = wordrg18;
    }
    else
    {
      wordrg2 = con(0x0);
      vb_00 = con(0x4);
    }
    if (SKW_IS_TILE_PASSAGE(wordrg2, wordrg3))
      return vb_00;
    if (wordrg3 > con(0x0))
      if (SKW_IS_TILE_PASSAGE(wordrg2, wordrg3 - con(0x1)))
        return con(0x0);
    if ((wordrg3 + con(0x1)) >= ddata.savegamewb)
      return con(0xffffffe0);
    wordrg3++;
    wordrg4 = wordrg3;
  }
  if (!SKW_IS_TILE_PASSAGE(wordrg2, wordrg4))
    return con(0xffffffe0);
  return con(0x0);
}

x16 SKW_LOCATE_OTHER_LEVEL(x16 eaxw, x16* ebxpw, x16* ecxpw, x16 edxw, x8** argpp0)
{
  x8* bptrrg2;
  x16 vw_08;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_14;
  x16 vw_18;

  vw_10 = *ebxpw + unsignedword(ddata.v1e03c8[eaxw].b4[con(0x2)]);
  vw_08 = *ecxpw + unsignedword(ddata.v1e03c8[eaxw].b4[con(0x3)]);
  x16 wordrg1 = edxw + (ddata.v1e03c8[eaxw].w8 & con(0x3f));
  if (wordrg1 >= con(0x0) && wordrg1 < con(0x3f))
  {
    x8 byterg4 = ddata.v1e03f0[wordrg1];
    if (byterg4 != con(0xffffffff))
    {
      if ((argpp0 == NULL) || (*argpp0 == NULL))
        bptrrg2 = ddata.v1e03dc + byterg4;
      else
        bptrrg2 = DOWNCAST(x32, *argpp0) + con(0x1);
      for (;; bptrrg2++)
      {
        vw_14 = signedword(*bptrrg2);
        if (signedlong(*bptrrg2) == con(0xffffffff))
          break;
        x16 wordrg32 = unsignedword(ddata.v1e03c8[vw_14].b4[con(0x2)]);
        if (vw_10 < (unsignedword(ddata.v1e03c8[vw_14].b4[con(0x2)]) - con(0x1)))
          continue;
        x16 wordrg51 = ulrshift(ddata.v1e03c8[vw_14].w8, con(0x5), con(0xb));
        if (UI32CAST(signedlong(vw_10)) > UI32CAST(wordrg32 + wordrg51 + con(0x1)))
          continue;
        x16 wordrg52 = unsignedword(ddata.v1e03c8[vw_14].b4[con(0x3)]);
        if (vw_08 < (unsignedword(ddata.v1e03c8[vw_14].b4[con(0x3)]) - con(0x1)))
          continue;
        if (UI32CAST(signedlong(vw_08)) > UI32CAST((urshift(ddata.v1e03c8[vw_14].w8, con(0xb)) + wordrg52 + con(0x1))))
          continue;
        x16 wordrg31 = vw_10 - wordrg32;
        x16 wordrg53 = vw_08 - wordrg52;
        vw_0c = ddata.v1d3248;
        SKW_CHANGE_CURRENT_MAP_TO(vw_14);
        vw_18 = unsignedword((SKW_GET_TILE_VALUE(wordrg31, wordrg53) & con(0xff)) >> con(0x5));
        if (vw_18 == con(0x5))
          if ((SKW_GET_ADDRESS_OF_TILE_RECORD(wordrg31, wordrg53)->u4.b.b0 & con(0x1)) != con(0x0))
            vw_18 = con(0x7);
        SKW_CHANGE_CURRENT_MAP_TO(vw_0c);
        if (vw_18 != con(0x7))
        {
          *ebxpw = wordrg31;
          *ecxpw = wordrg53;
          if (argpp0 != NULL)
            *argpp0 = bptrrg2;
          return vw_14;
        }
      }
    }
  }
  if (argpp0 != NULL)
    *argpp0 = NULL;
  return con(0xffffffff);
}

x16 SKW_FIND_LADDAR_AROUND(x16 eaxw, x16 ebxw, c_record** ecxpp, x16 edxw)
{
  c_record* recoptrrg1;
  c_record** recopptrrg7;

  recopptrrg7 = ecxpp;
  for (x16 vw_08 = con(0x0); vw_08 <= con(0x3); vw_08++)
  {
    for (x16 wordrg11 = SKW_GET_TILE_RECORD_LINK(eaxw + table1d27fc[vw_08], edxw + table1d2804[vw_08]); wordrg11 != con(0xfffffffe); wordrg11 = SKW_GET_NEXT_RECORD_LINK(wordrg11))
    {
      x16 wordrg400 = urshift(wordrg11 & con(0x3c00), con(0xa));
      if (wordrg400 > con(0x3))
        break;
      if (wordrg400 == con(0x3) || wordrg400 == con(0x2))
      {
        recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg11);
        if (wordrg400 != con(0x3))
        {
          if ((recoptrrg1->u2.w & con(0x6)) == con(0x2))
          {
            x16 wordrg401 = urshift(recoptrrg1->u2.w, con(0x3));
            ui16 uwordrg1 = urshift(wordrg401, con(0x8)) & con(0x1f);
            if (uwordrg1 > con(0x6) && uwordrg1 <= con(0x7))
            {
              x16 wordrg1 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0x11), CUTX8(wordrg401));
              if ((ebxw == con(0xffffffff) && wordrg1 != con(0x0)) || (ebxw == con(0x1) && wordrg401 == con(0x0)))
              {
                *recopptrrg7 = NULL;
                return vw_08;
              }
            }
          }
        }
        else
        {
          if ((recoptrrg1->u2.w & con(0x7f)) == con(0x1c))
          {
            if (ebxw == ((ddata.v1e03c8[urshift(recoptrrg1->u2.w, con(0x7))].w8 & con(0x3f)) - (ddata.v1e03c0->w8 & con(0x3f))))
            {
              *recopptrrg7 = recoptrrg1;
              return vw_08;
            }
          }
        }
      }
    }
  }
  *recopptrrg7 = NULL;
  return con(0xffffffff);
}

c_image* SKW_QUERY_PICST_IT(c_image* eaxpimg)
{
  c_image vs_00;
  c_hex18 vh18_13c;
  t_gfxdata* destbmp; // was gptrrg7
  t_gfxdata* srcbmp; // was gptrrg6
  t_gfxdata* gptrrg1;
  x16 vw_15c;
  x16 vw_160;

  bool vbool_158;
  if (eaxpimg->w_34 != con(0x40))
    vbool_158 = true;
  else if (eaxpimg->w_36 == con(0x40))
    vbool_158 = false;
  else
    vbool_158 = true;
  bool boolrg2 = eaxpimg->s_00.w_06 == con(0xffffffff) && eaxpimg->s_00.w_0c >= con(0x0);
  if (vbool_158)
  {
    eaxpimg->w_1c = SKW_CALC_STRETCHED_SIZE(eaxpimg->w_1c, eaxpimg->w_34);
    eaxpimg->w_1e = SKW_CALC_STRETCHED_SIZE(eaxpimg->w_1e, eaxpimg->w_36);
  }
  if ((eaxpimg->w_32 & con(0x1)) != con(0x0))
    eaxpimg->w_1c = -eaxpimg->w_1c;
  if (eaxpimg->w_30 == con(0xfffffffd))
    return eaxpimg;
  if (vbool_158)
  {
    if (!boolrg2)
    {
      if (!SKW_3e74_55f9(SKW_0b36_068f(eaxpimg), &vw_160))
      {
        SKW_COPY_MEMORY(DOWNCAST(c_hex18, &eaxpimg->s_00), con(0x18), DOWNCAST(c_hex18, &vh18_13c));
        SKW_QUERY_PICST_IMAGE(&vh18_13c);
      }
      else
      {
        SKW_0b36_00c3(vw_160, &eaxpimg->s_00);
        return eaxpimg;
      }
    }
    else
    {
      vw_160 = SKW_3e74_5888();
      SKW_COPY_MEMORY(DOWNCAST(c_hex18, &eaxpimg->s_00), con(0x18), DOWNCAST(c_hex18, &vh18_13c));
      SKW_0b36_00c3(vh18_13c.w_0c, &vh18_13c);
    }
    SKW_0b36_018f(&vh18_13c);
    if ((vh18_13c.w_04 & con(0x10)) != con(0x0))
    {
      SKW_COPY_IMAGE(eaxpimg, &vs_00);
      vs_00.w_32 = con(0x0);
      vs_00.w_36 = con(0x40);
      vs_00.w_34 = con(0x40);
      if (!SKW_3e74_55f9(SKW_0b36_068f(&vs_00), &vw_15c))
      {
        destbmp = SKW_ALLOC_NEW_PICT(vw_15c, vh18_13c.height, CUTX8(vh18_13c.w_16), vh18_13c.width);
        s_gfxdatatail* ptrbeforerg7 = getprevious(destbmp);
        srcbmp = SKW_QUERY_PICT_BITMAP(&vh18_13c);
        s_gfxdatatail* ptrbeforerg6 = getprevious(srcbmp);
//                            srcbmp                                                              srcofs        destbmp                                                                              srcbits                  destbits
        SKW_FIRE_BLIT_PICTURE(srcbmp, SKW_ALLOC_TEMP_ORIGIN_RECT(vh18_13c.width, vh18_13c.height), vh18_13c.w_0e, destbmp, vh18_13c.w_10, ptrbeforerg6->width, ptrbeforerg7->width, con(0xffff), con(0x0), e_bitres(vh18_13c.w_16), e_bitres(vh18_13c.w_16));
      }
      SKW_0b36_01cd(&vh18_13c);
      SKW_0b36_00c3(vw_15c, &vh18_13c);
      SKW_0b36_018f(&vh18_13c);
    }
    if (vbool_158) // TODO: ?!? vbool_158 ever true at this point
    {
      x16 wordrg6 = SKW_CALC_STRETCHED_SIZE(vh18_13c.width, eaxpimg->w_34);
      x16 wordrg2 = wordrg6;
      x16 wordrg10 = SKW_CALC_STRETCHED_SIZE(vh18_13c.height, eaxpimg->w_36);
      if (wordrg6 > con(0x0) && wordrg10 > con(0x0))
      {
        SKW_ALLOC_NEW_PICT(vw_160, wordrg10, CUTX8(vh18_13c.w_16), wordrg6);
        srcbmp = SKW_QUERY_PICT_BITMAP(&vh18_13c);
        gptrrg1 = SKW_0b36_00c3(vw_160, &eaxpimg->s_00);
        if (vh18_13c.w_16 != con(0x8))
          SKW_FIRE_STRETCH_BLIT_TO_MEMORY_4TO4BPP(srcbmp, vh18_13c.width, vh18_13c.height, gptrrg1, wordrg6, wordrg10, NULL);
        else
          SKW_44c8_2351(srcbmp, vh18_13c.width, vh18_13c.height, gptrrg1, wordrg6, wordrg10);
      }
      else
      {
        eaxpimg->s_00.width = wordrg2;
        eaxpimg->s_00.height = wordrg10;
      }
    }
    SKW_0b36_01cd(&vh18_13c);
    SKW_3e74_585a(vw_160, false);
    return eaxpimg;
  }
  if (!boolrg2)
  {
    SKW_QUERY_PICST_IMAGE(&eaxpimg->s_00);
    return eaxpimg;
  }
  SKW_0b36_00c3(eaxpimg->s_00.w_0c, &eaxpimg->s_00);
  return eaxpimg;
}

x16 SKW_075f_06bd(c_record* eaxprec, x16 edxw)
{
  ddata.v1e01a4 = con(0x0);
  ddata.v1e01a6 = con(0x3);
  x16 wordrg5 = unsignedword(eaxprec->u4.b.b0);
  x16 wordrg2;
  if (urshift(edxw & con(0x3c00), con(0xa)) == con(0xf))
  {
    ui32 longrg4 = unsignedlong(edxw);
    if (longrg4 != con(0xff81))
    {
      if (longrg4 >= con(0xff83))
      {
        ddata.v1e01a6 = con(0x5);
        if (longrg4 != con(0xff86))
          return con(0x0);
        else
        {
          ddata.v1e01a4 = wordrg5;
          return wordrg5 / 8 + con(0x1);
        }
      }
      ddata.v1e01a6 = con(0x1);
      wordrg2 = CUTX16(SKW_RAND());
      wordrg2 &= con(0xf);
      wordrg2 += (CUTX16(SKW_RAND()) & con(0xf));
      wordrg2 += con(0xa);
      if (longrg4 == con(0xff82))
      {
        ddata.v1e01a6 = con(0x7);
        wordrg2 <<= con(0x4);
        wordrg2 += wordrg5;
      }
    }
    else
    {
      wordrg2 = CUTX16(SKW_RAND()) & con(0xf);
      ddata.v1e01a4 = wordrg2 + con(0xa);
      wordrg2 += CUTX16(SKW_RAND()) & con(0x1f);
    }
  }
  else
  {
    wordrg2 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(edxw, con(0x9));
    if (wordrg2 != con(0x0))
    {
      x16 wordrg21 = wordrg2 + urshift(wordrg5, con(0x1));
      x32 longrg1 = ((unsignedlong(eaxprec->u4.b.b1) >> 4) + con(0x3)) & con(0xffff);
      wordrg2 = CUTX16((unsignedlong(wordrg21) * longrg1 * longrg1) >> con(0x7));
      ddata.v1e01a6 = con(0x4);
      ddata.v1e01a4 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(edxw, con(0xd));
      if (ddata.v1e01a4 != con(0x0))
        if ((CUTX16(SKW_RAND()) & con(0x7f)) > wordrg5)
          ddata.v1e01a4 -= SKW_RAND16(ddata.v1e01a4 / 2 + con(0x1));
    }
    wordrg2 += SKW_RAND02();
    wordrg2 += SKW_QUERY_ITEM_WEIGHT(edxw);
    if (unsignedword(eaxprec->u4.b.b1) > (CUTX16(SKW_RAND()) & con(0x1ff)))
      wordrg2 *= con(0x2);
  }
  x16 wordrg22 = (wordrg2 + wordrg5) / 16 + con(0x1);
  wordrg22 += SKW_RAND02() + SKW_RAND16(wordrg22 / 2 + con(0x1));
  return DM2_MIN(DM2_MAX(wordrg22, 2 * (wordrg22 - con(0x20) + unsignedword(eaxprec->u4.b.b1) / 8)), 2 * wordrg5);
}

// was SKW_29ee_09d5
void SKW_DRAW_SPELL_PANEL(void)
{
  c_hero* heroptrrg1;
  t_text t_00[0x4]; // TODO dim maybe 0x2 only
  x8 vb_08; // inc

  heroptrrg1 = &ddata.hero[ddata.v1e0b74 - con(0x1)];
  x8 vb_04 = heroptrrg1->b1e;
  SKW_DRAW_ICON_PICT_ENTRY(con(0x1), vb_04 + con(0x1), &ddata.v1d694c, con(0x5), con(0x5c), con(0xffff));
  if (vb_04 < con(0x4))
  {
    t_00[0x1] = NULLBYTE;
    vb_08 = (vb_04 * con(0x6)) + con(0x60);
    x16 wordrg5 = con(0xff);
    for (x16 vw_04_1 = con(0x0); vw_04_1 < con(0x6); vw_04_1++)
    {
      t_00[0x0] = vb_08;
      vb_08++;
      SKW_DRAW_SIMPLE_STR(&ddata.v1d694c, unsignedword(ddata.paletteptr1[con(0x0)]), unsignedword(ddata.paletteptr1[con(0xd)]) | con(0x4000), wordrg5++, t_00);
    }
  }
  SKW_DRAW_SPELL_TO_BE_CAST(false);
  SKW_DRAW_PLAYER_ATTACK_DIR();
}

// was SKW_29ee_0b4a
x16 SKW_PROCEED_SPELL_FAILURE(x16 eaxw)
{
  x16 wordrg3 = con(0x1);
  ui16 uwordrg1 = eaxw & con(0xfffffff0);
  if (uwordrg1 < con(0x20))
  {
    if (uwordrg1 != con(0x10))
      return uwordrg1;
    ddata.v1e0b7c = (((eaxw & con(0xf)) == con(0x3)) ? 1 : 0) - con(0x5);
    uwordrg1 = con(0x45);
  }
  else if (uwordrg1 <= con(0x20))
  {
    ddata.v1e0b7c = con(0xfffffffd);
    uwordrg1 = con(0x46);
  }
  else
  {
    if (uwordrg1 != con(0x30))
      return uwordrg1;
    SKW_DRAW_TRANSPARENT_STATIC_PIC(con(0x1), con(0xb), con(0x5c), con(0x5), con(0xffffffff));
    uwordrg1 = con(0x44);
    wordrg3 = uwordrg1;
  }
  x16 wordrg12 = DM2_MAX(wordrg3, con(0x8) - SKW_UPDATE_GLOB_VAR(uwordrg1, con(0x3), true));
  ddata.v1e0b6c = wordrg12;
  return wordrg12;
}

bool SKW_TRANSFER_PLAYER(x16 eaxw, x16 ebxw, x16 edxw)
{
  x16 wordrg11 = SKW_GET_CREATURE_AT(eaxw, edxw);
  if (wordrg11 == con(0xffffffff))
    return false;
  x16 wordrg2 = SKW_GET_ADDRESS_OF_RECORD(wordrg11)->u2.w;
  while (wordrg2 != con(0xfffffffe))
  {
    x16 wordrg1 = urshift(wordrg2 & con(0x3c00), con(0xa));
    if (wordrg1 > con(0x4) && wordrg1 < con(0xe))
    {
      if (SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg2) == ebxw)
        return true;
      if (SKW_2fcf_164e(wordrg2, ebxw))
        return true;
    }
    wordrg2 = SKW_GET_NEXT_RECORD_LINK(wordrg2);
  }
  return false;
}

void SKW_32cb_3e08(x16 eaxw, x32 ebxl, c_record* ecxprec, x16 edxw)
{
  c_record* recoptrrg6;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;

  vw_08 = eaxw;
  vw_04 = edxw;
  recoptrrg6 = ecxprec;
  if (eaxw == con(0xfffffffe))
    return;
  if (table1d6b43[vw_04] == con(0xffffffff))
    return;
  ddata.v1d6c04 = con(0xfffffffe);
  if (vw_04 > con(0xf))
    return;
  x8 byterg41 = table1d6e41[vw_04];
  vw_0c = unsignedword(byterg41);
  if (byterg41 == con(0x3))
    return;
  x32 longrg7 = con(0x0);
  for (x16 wordrg5 = con(0x0); wordrg5 < con(0x4); wordrg5++)
  {
    x16 wordrg1 = wordrg5 + (vw_0c << con(0x2));
    x8 byterg4 = table1d6e35[wordrg1];
    x32 longrg1 = con(0x1) << byterg4;
    if ((longrg1 & ebxl) == con(0x0))
      longrg7 |= longrg1;
    else
      SKW_DRAW_PUT_DOWN_ITEM(vw_08, SKW_DIR_FROM_5x5_POS(unsignedword(byterg4)), recoptrrg6, vw_04);
  }
  if (longrg7 != con(0x0))
  {
    ddata.v1d6c04 = vw_08;
    ddata.longv1e1040 = longrg7;
    ddata.v1e103c = recoptrrg6;
  }
}

// TODO: odd behaviour. 2nd argument not used as a textpointer.
bool R_52DC0(x8* eaxpb, const t_text* edxt)
{
  x8* bptrrg2;
  x8* bptrrg4;
  x32* lptrrg2;
  x32* lptrrg4;

  bptrrg4 = UPCAST(x8, edxt); // not used as textpointer now
  bptrrg2 = eaxpb;
  lptrrg4 = UPCAST(x32, bptrrg4);
  lptrrg2 = UPCAST(x32, bptrrg2);
  if (eaxpb == bptrrg4)
    return false;
  x32 longrg3;
  x32 longrg11;
  for(;;)
  {
    longrg11 = lptrrg2[con(0x0)];
    longrg3 = lptrrg4[con(0x0)];
    if (longrg3 != longrg11)
      break;
    longrg3 = ~longrg3;
    longrg11 += con(0xfefefeff);
    longrg11 &= longrg3;
    longrg11 &= con(0x80808080);
    if (longrg11 != con(0x0))
      return false;
    longrg11 = lptrrg2[con(0x1)];
    longrg3 = lptrrg4[con(0x1)];
    if (longrg3 != longrg11)
      break;
    longrg3 = ~longrg3;
    longrg11 += con(0xfefefeff);
    longrg11 &= longrg3;
    longrg11 &= con(0x80808080);
    if (longrg11 != con(0x0))
      return false;
    longrg11 = lptrrg2[con(0x2)];
    longrg3 = lptrrg4[con(0x2)];
    if (longrg3 != longrg11)
      break;
    longrg3 = ~longrg3;
    longrg11 += con(0xfefefeff);
    longrg11 &= longrg3;
    longrg11 &= con(0x80808080);
    if (longrg11 != con(0x0))
      return false;
    longrg11 = lptrrg2[con(0x3)];
    longrg3 = lptrrg4[con(0x3)];
    if (longrg3 != longrg11)
      break;
    bptrrg2 += con(0x10);
    bptrrg4 += con(0x10);
    longrg3 = ~longrg3;
    longrg11 += con(0xfefefeff);
    longrg11 &= longrg3;
    longrg11 &= con(0x80808080);
    if (longrg11 != con(0x0))
      return false;
  }
  x8 byterg3h = CUTLX8(longrg3 >> con(0x8));
  x8 byterg1h = longrg11 >> con(0x8);
  if (CUTX8(longrg11) == CUTLX8(longrg3))
  {
    if (CUTX8(longrg11) == con(0x0))
      return false;
    if (byterg1h == byterg3h)
    {
      if (byterg1h == con(0x0))
        return false;
      longrg11 >>= con(0x10);
      longrg3 >>= con(0x10);
      if (CUTX8(longrg11) == CUTLX8(longrg3) && CUTX8(longrg11) == con(0x0))
        return false;
    }
  }
  return true;
}

// was SKW_14cd_36b3
x8 SKW_PROCEED_XACT_82(void)
{
  x16* wptrrg5;
  c_record* recoptrrg5;
  t_gfxdata vga_00[0x14];
  x16 vw_14;
  x8 vb_18;

  x16 wordrg6 = con(0xffff);
  vb_18 = con(0xfffffffd);
  x16 wordrg3 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
  x16 wordrg40 = unsignedword(ddata.s350.v1e0562.u.b.b_07) + table1d2804[wordrg3];
  x16 wordrg11 = SKW_GET_CREATURE_AT(table1d27fc[wordrg3] + unsignedword(ddata.s350.v1e0562.u.b.b_06), wordrg40);
  if (wordrg11 == wordrg6)
    return vb_18;
  ddata.s350.v1e0556->u.v.b1a = con(0x1d);
  x16 wordrg21 = wordrg11;
  recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(wordrg21);
  if (ddata.s350.v1e0572 == con(0x0))
  {
    wordrg6 = SKW_1c9a_0732(con(0x3e), CUTX8((ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe)) + con(0x2)) & con(0x3)), SKW_GET_ADDRESS_OF_RECORD(wordrg21)->u2.w);
    if (wordrg6 == con(0xfffffffe))
      wordrg6 = con(0xffff);
    x16 wordrg5 = SKW_14cd_2886(&recoptrrg5->u2.w, (CUTX8(wordrg3) + con(0x2)) & con(0x3), con(0x0), con(0x10), con(0x1), con(0x1));
    if (wordrg5 == con(0x0) || wordrg5 == con(0xffffffff))
      if (wordrg6 == con(0xffffffff))
        return vb_18;
    ddata.s350.v1e0556->u.v.w10 = DM2_MAX(con(0x0), wordrg5);
    return con(0xfffffffe);
  }
  if (ddata.s350.v1e0572 != con(0x1))
  {
    if (ddata.s350.v1e0572 != con(0x2))
      wptrrg5 = &ddata.s350.v1e0556->u0c.w;
    else
      wptrrg5 = &ddata.s350.v1e0556->u.v.w0e;
  }
  else
    wptrrg5 = &ddata.s350.v1e0556->u.v.w10;
  x16 wordrg22 = ddata.s350.v1e054e->u2.w;
  while (wordrg22 != con(0xfffffffe))
  {
    if (SKW_IS_CONTAINER_MONEYBOX(wordrg22))
    {
      wordrg6 = wordrg22;
      SKW_14cd_3582(con(0x0), wordrg22);
    }
    wordrg22 = SKW_GET_NEXT_RECORD_LINK(wordrg22);
  }
  ddata.s350.v1e0572 = con(0x47);
  x16 wordrg23 = *wptrrg5;
  if (wordrg23 <= con(0x0))
  {
    vb_18 = SKW_PROCEED_XACT_64();
    if (vb_18 == con(0xfffffffc))
      return vb_18;
    return con(0xfffffffe);
  }
  SKW_48ae_0767(wordrg23, vga_00, &vw_14, con(0x12));
  *wptrrg5 = con(0xffffffff);
  if (vw_14 <= con(0x0))
    return con(0xfffffffe);
  x16 wordrg51 = vw_14 - con(0x1);
  do
  {
    x16 wordrg1 = SKW_ALLOC_NEW_DBITEM(ddata.v1e0394[vga_00[wordrg51]]);
    if (wordrg1 == con(0xffffffff))
      break;
    if (wordrg6 == con(0xffffffff))
      SKW_APPEND_RECORD_TO(wordrg1, con(0xffffffff), con(0x0), &ddata.s350.v1e054e->u2.w);
    else
      SKW_ADD_COIN_TO_WALLET(wordrg6, wordrg1);
    wordrg51--;
  } while (wordrg51 != con(0xffffffff));
  SKW_PROCEED_XACT_64();
  return con(0xfffffffc);
}

// was SKW_14cd_3919
x8 SKW_PROCEED_XACT_84(void)
{
  c_record* recoptrrg1;
  bool flag;
  x8 vb_00;

  vb_00 = con(0xfffffffd);
  x16 wordrg42 = ddata.s350.v1e054e->u2.w;
  if (wordrg42 == con(0xfffffffe))
    return vb_00;
  recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg42);
  ui16 uwordrg2 = urshift((wordrg42 & con(0x3c00)), con(0xa)) - con(0x5);
  bool boolrg1;
  if (uwordrg2 > con(0x5))
    boolrg1 = false;
  else
  {
    switch (uwordrg2)
    {
      case 0:
      case 1:
        flag = (recoptrrg1->u2.b.b0 & con(0xffffff80)) == con(0x0);
        boolrg1 = flag;
      break;

      case 3:
        flag = (recoptrrg1->u2.b.b1 & con(0xffffff80)) == con(0x0);
      case 2: // no break before
      case 4:
        boolrg1 = false;
      break;

      case 5:
        if ((recoptrrg1->u2.b.b0 & con(0xffffff80)) == con(0x0))
          boolrg1 = true;
        else if (SKW_QUERY_GDAT_FOOD_VALUE_FROM_RECORD(wordrg42) == con(0x0))
          boolrg1 = false;
        else
          boolrg1 = true;
      break;

      default: throw(THROW_DMABORT);
    }
  }
  if (!boolrg1)
  {
    if ((CUTX16(SKW_RAND()) & con(0x7)) == con(0x0))
    {
      SKW_19f0_2165(con(0x81), unsignedword(ddata.s350.v1e0562.u.b.b_07), unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_07), con(0xffffffff), con(0x6));
      x16* tmp = &ddata.s350.v1e0556->u.v.w0e; // access on both bytes then
      x16 wordrg40 = (unsignedword(ddata.s350.v1e0562.u.b.b_06) & con(0x1f));
      *tmp &= con(0xffffffe0); // mod
      x16 wordrg22 = *tmp | wordrg40;
      *tmp = wordrg22;
      x16 wordrg41 = unsignedword(ddata.s350.v1e0562.u.b.b_07 & con(0x1f)) << con(0x5);
      *tmp = (wordrg22 & con(0xfffffc1f)) | wordrg41;
      *tmp &= con(0x3ff); // mod
      *tmp |= (unsignedword(ddata.s350.v1e0571) & con(0x3f)) << con(0xa);
      return vb_00;
    }
    if ((CUTX16(SKW_RAND()) & con(0xf)) == con(0x0))
      ddata.s350.v1e0556->u.v.w0e = con(0xffffffff);
    return vb_00;
  }
  SKW_CUT_RECORD_FROM(wordrg42, con(0xffffffff), con(0x0), &ddata.s350.v1e054e->u2.w);
  SKW_DEALLOC_RECORD(wordrg42);
  ddata.s350.v1e0556->u.v.b1a = con(0x0);
  return con(0xfffffffe);
}

// was SKW_14cd_3a0b
x8 SKW_PROCEED_XACT_85(void)
{
  c_record* recoptrrg1;

  x16 wordrg1 = SKW_GET_TILE_RECORD_LINK(unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_07));
  x16 wordrg4;
  for (;; wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg4))
  {
    wordrg4 = wordrg1;
    if (wordrg4 == con(0xfffffffe))
      break;
    x16 wordrg11 = urshift(wordrg4 & con(0x3c00), con(0xa));
    if (wordrg11 > con(0x3))
      break;
    if (wordrg11 == con(0x2) && (recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg4), (recoptrrg1->u2.w & con(0x6)) == con(0x2)))
    {
      x16 wordrg12 = urshift(recoptrrg1->u2.w, con(0x3));
      if ((urshift(wordrg12, con(0x8)) & con(0x1f)) == con(0x1))
      {
        ddata.s350.v1e0556->u.v.b1e = CUTX8(wordrg12);
        ddata.s350.v1e0556->u.v.b1a = con('"');
        return con(0xfffffffe);
      }
    }
  }
  SKW_13e4_0360(ddata.s350.v1e054c, unsignedword(ddata.s350.v1e0562.u.b.b_07), con(0x13), unsignedword(ddata.s350.v1e0562.u.b.b_06), true);
  ddata.s350.v1e0556->u.v.b1a = con(0x33);
  return con(0xfffffffd);
}

// eax and edx are always extended bytes, ebx comes in with 0 and 1 only
void SKW_OPERATE_PIT_TELE_TILE(x16 eaxw, x16 ebxw, x16 edxw)
{
  x8* bptrrg1;
  x8* bptrrg2;

  x16 wordrg5 = ebxw;
  bool boolrg3 = false;
  bptrrg1 = ddata.savegamepp1[eaxw];
  bptrrg2 = bptrrg1 + edxw;
  x16 wordrg1 = (unsignedword(*bptrrg2) >> con(0x5)) & con(0xff);
  if (wordrg1 != con(0x2))
  {
    if (wordrg1 != con(0x5) || (SKW_GET_ADDRESS_OF_TILE_RECORD(eaxw, edxw)->u4.w & con(0x6)) == con(0x6))
      return;
    if ((*bptrrg2 & con(0x8)) != con(0x0))
      boolrg3 = true;
    if (wordrg5 == con(0x2))
      wordrg5 = (boolrg3) ? 1 : 0;
  }
  else
  {
    if ((*bptrrg2 & con(0x8)) != con(0x0))
      boolrg3 = true;
    if (wordrg5 == con(0x2))
      wordrg5 = (boolrg3) ? 1 : 0;
  }
  if (wordrg5 != con(0x0))
  {
    *bptrrg2 &= con(0xfffffff7);
    return;
  }
  // RG1Blo = (!boolrg3) ? 1 : 0;  TODO: what for?
  *bptrrg2 |= con(0x8);
  SKW_ADVANCE_TILES_TIME(eaxw, edxw);
}

// TODO: problem inside
bool SKW_1887_0239(void)
{
  bool boolrg5 = false; // TODO: warning being used uninitialized

  ui8 ubyterg1 = ddata.s350.v1e0556->u.v.b1f;
  if (ubyterg1 < con(0x1))
  {
    if (ubyterg1 == con(0x0))
    {
      ddata.s350.v1e0556->u.v.b1f = con(0x1);
      x8 byterg2 = ddata.s350.v1e07d8.b1;
      boolrg5 = SKW_CREATURE_WALK_NOW();
      ddata.s350.v1e07d8.b1 = byterg2;
      x16 wordrg2 = ((ddata.s350.v1e0556->u.v.u18.w & con(0x1f)) + table1d27fc[ddata.s350.v1e0556->u.v.b1b]) & con(0x1f);
      ddata.s350.v1e0556->u.v.u18.b.b0 &= con(0xffffffe0);
      x16 wordrg6 = ddata.s350.v1e0556->u.v.u18.w | wordrg2;
      ddata.s350.v1e0556->u.v.u18.w = wordrg6;
      ddata.s350.v1e0556->u.v.u18.w = (wordrg6 & con(0xfffffc1f)) | (((table1d2804[ddata.s350.v1e0556->u.v.b1b] + ulrshift(wordrg6, con(0x6), con(0xb))) & con(0x1f)) << con(0x5));
    }
    return boolrg5;
  }
  if (ubyterg1 <= con(0x1))
  {
    boolrg5 = SKW_CREATURE_WALK_NOW();
    if (!boolrg5)
      ddata.s350.v1e0556->u.v.b1f = con(0x3);
    else
      ddata.s350.v1e0556->u.v.b1f = con(0x2);
    return boolrg5;
  }
  if (ubyterg1 != con(0x2))
    return boolrg5;
  boolrg5 = false;
  ddata.s350.v1e0556->u.v.b1f = con(0x3);
  if (SKW_MOVE_RECORD_TO(ddata.s350.v1e054c, unsignedword(ddata.s350.v1e0562.u.b.b_07), unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_07)) != con(0x0))
    ddata.s350.v1e0570 = con(0x1);
  ddata.s350.v1e0562.u.b.b_06 = CUTX8(ddata.v1e102a);
  ddata.s350.v1e0562.u.b.b_07 = CUTX8(ddata.v1e1028);
  ddata.s350.v1e0571 = CUTX8(ddata.v1e102c);
  SKW_1c9a_0648(unsignedword(ddata.s350.v1e0571));
  return boolrg5;
}

// was SKW_1c9a_02f6
void SKW_CHECK_RECOMPUTE_LIGHT(void)
{
  bool boolrg5;
  u_bbw vs_00[0x8];
  // vs_10; TODO: unused?!?
  c_sizex16 vsx16_34; // extended! was s_len8 only!!! reason: parameter of SKW_1c9a_1bd8

  x16 wordrg6 = ddata.v1d3248;
  SKW_CHANGE_CURRENT_MAP_TO(ddata.v1e0266);
  if ((ddata.v1e0390.b0 & con(0x2)) != con(0x0))
  {
    ddata.v1d62a4 = ddata.v1e0266;
    ddata.v1e08d2 = ddata.savegamewa;
    DM2_ZERO_MEMORY(DOWNCAST(s_32bytes, ddata.v1e08cc), signedlong(ddata.v1e08d2) << con(0x5));
    ddata.v1d62a6 = ddata.v1e027c;
    if (ddata.v1e027c >= con(0x0))
    {
      ddata.v1e08d0 = ulrshift(ddata.v1e03c8[ddata.v1e027c].w8, con(0x5), con(0xb)) + con(0x1);
      DM2_ZERO_MEMORY(DOWNCAST(s_32bytes, ddata.v1e08c8), signedlong(ddata.v1e08d0) << con(0x5));
    }
    vsx16_34.s_00.b_00 = con(0x19);
    vsx16_34.s_00.b_01 = con(0x0);
    vsx16_34.s_00.b_07 = con(0x1b);
    SKW_1c9a_1bd8(ddata.v1e0270, con(0x8), ddata.v1e0272, con(0x1), &vsx16_34, vs_00);
  }
  if ((ddata.v1e0390.b0 & con(0x1)) != con(0x0))
  {
    boolrg5 = ddata.v1e0266 != ddata.v1d62ac;
    if (ddata.v1e03f8 != con(0x0))
    {
      if (ddata.v1e03f8 > con(0x8))
        ddata.v1e03f8 = con(0x8);
      vsx16_34.s_00.b_00 = CUTX8(ddata.v1e03f8);
      vsx16_34.s_00.b_01 = con(0x0);
      vsx16_34.s_00.b_07 = con(0x17);
      SKW_1c9a_1bd8(ddata.v1e0270, con(0x7), ddata.v1e0272, con(0x1), &vsx16_34, vs_00);
      if (ddata.v1e0974 != ddata.v1d62a8)
        boolrg5 = true;
      else if (ddata.v1e0978 != ddata.v1d62aa)
        boolrg5 = true;
    }
    else
    {
      ddata.v1e0974 = ddata.v1e03f8;
      ddata.v1e0978 = ddata.v1e03f8;
    }
    ddata.v1d62a8 = ddata.v1e0974;
    ddata.v1d62aa = ddata.v1e0978;
    if (boolrg5)
      SKW_RECALC_LIGHT_LEVEL();
  }
  SKW_CHANGE_CURRENT_MAP_TO(wordrg6);
  ddata.v1e0390.l0 = con(0x0);
  ddata.v1d62ac = ddata.v1e0266;
}

x16 SKW_1c9a_03cf(x16* eaxpw, x16 ebxw, x16* edxpw)
{
  c_record* recoptrrg2;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_14;
  x16 vw_18;
  x16 vw_1c;
  x16 vw_20;
  x16 vw_24;
  x16 vw_28;

  vw_24 = ebxw;
  vw_18 = *eaxpw;
  vw_1c = *edxpw;
  x16 wordrg1;
  if (ebxw != con(0xff))
    wordrg1 = table1d2752[ebxw];
  else
    wordrg1 = con(0xc);
  SKW_098d_000f(vw_18, wordrg1, &vw_04, vw_1c, &vw_14);
  vw_20 = con(0x4);
  x16 wordrg3;
  for (x16 wordrg5 = con(0x0); wordrg5 <= vw_20; wordrg5++)
  {
    vw_28 = SKW_GET_CREATURE_AT(vw_18, vw_1c);
    if (vw_28 != con(0xffffffff))
    {
      recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(vw_28);
      vw_08 = signedword(SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(recoptrrg2->u4.b.b0)->barr22[con(0x1)]);
      SKW_098d_000f(vw_18, SKW_QUERY_CREATURE_5x5_POS(recoptrrg2, unsignedword(table1d62e8[ulrshift(recoptrrg2->ue.w, con(0x6), con(0xe))])), &vw_10, vw_1c, &vw_0c);
      vw_10 -= vw_04;
      x16 wordrg2 = vw_0c - vw_14;
      vw_0c = wordrg2;
      if ((wordrg2 * wordrg2 + vw_10 * vw_10) < table1d62e0[vw_08])
      {
        *eaxpw = vw_18;
        *edxpw = vw_1c;
        return vw_28;
      }
    }
    if (vw_24 != con(0xff))
    {
      vw_18 = *eaxpw + table1d62b0[wordrg5 + 2 * vw_24][0];
      wordrg3 = table1d62b0[wordrg5 + 2 * vw_24][1];
    }
    else
    {
      vw_18 = *eaxpw + table1d62d0[wordrg5][0];
      wordrg3 = table1d62d0[wordrg5][1];
    }
    vw_1c = *edxpw + wordrg3;
  }
  return con(0xffff);
}

// SKW_1c9a_0551
x16 SKW_COMPUTE_POWER_4_WITHIN(x16 eaxw, x16 edxw)
{
  x16 wordrg1 = con(0x1);
  x16 wordrg2 = con(0x0);
  while (wordrg2 < con(0x20)) // BUG! then 16-bit aren't enough!
  {
    if (((wordrg1 & eaxw) != con(0x0)) && (--edxw == con(0x0)))
      return wordrg1;
    wordrg1 *= con(0x2);
    wordrg2++;
  }
  return wordrg1;
}

// TODO: make argument unsigned
x16 SKW_1c9a_0598(x16 eaxw)
{
  x16 wordrg4 = con(0x0);
  x16 wordrg2 = con(0x0);
  ui16 uwordrg1 = eaxw;
  while (uwordrg1 != con(0x0))
  {
    if (wordrg4 >= con(0x20))
      return wordrg2;
    if ((uwordrg1 & con(0x1)) != con(0x0))
      wordrg2++;
    uwordrg1 >>= con(0x1);
    wordrg4++;
  }
  return wordrg2;
}

void SKW_1c9a_078b(x16* eaxpw, x8 ebxb, x16 edxw)
{
  c_record* recoptrrg6;
  c_record* prec_04;
  x16 vw_08;
  x16 vw_0c;
  x16 vw_10;
  x8 vb_14;

  vw_08 = edxw;
  vb_14 = ebxb;
  vw_10 = *eaxpw;
  for(;;)
  {
    vw_0c = vw_10;
    if (vw_10 == con(0xfffffffe))
      return;
    vw_10 = SKW_GET_NEXT_RECORD_LINK(vw_10);
    x16 wordrg5 = urshift(vw_0c & con(0x3c00), con(0xa));
    if ((wordrg5 <= con(0x4) || wordrg5 >= con(0xe)) && wordrg5 != con(0x9))
      continue;
    if (vb_14 != con(0xffffffff) && unsignedword(vb_14) != urshift(vw_0c, con(0xe)))
      continue;
    x8 byterg4 = SKW_1c9a_0006(vw_0c, vw_08);
    if (wordrg5 == con(0x9))
    {
      if (!SKW_IS_CONTAINER_MONEYBOX(vw_0c) || byterg4 != con(0x0))
      {
        recoptrrg6 = SKW_GET_ADDRESS_OF_RECORD(vw_0c);
        prec_04 = recoptrrg6;
        SKW_1c9a_078b(&recoptrrg6->u2.w, con(0xff), vw_08);
        if (byterg4 != con(0x0))
        {
          x16 wordrg62 = recoptrrg6->u2.w;
          for(;;)
          {
            x16 wordrg51 = wordrg62;
            if (wordrg62 == con(0xfffffffe))
              break;
            wordrg62 = SKW_GET_NEXT_RECORD_LINK(wordrg62);
            SKW_CUT_RECORD_FROM(wordrg51, con(0xffffffff), con(0x0), &prec_04->u2.w);
            wordrg51 &= con(0xffff3fff);
            wordrg51 |= unsignedword(vb_14) << con(0xe);
            SKW_APPEND_RECORD_TO(wordrg51, con(0xffffffff), con(0x0), eaxpw);
          }
        }
      }
    }
    if (byterg4 != con(0x0))
    {
      SKW_CUT_RECORD_FROM(vw_0c, con(0xffffffff), con(0x0), eaxpw);
      SKW_DEALLOC_RECORD(vw_0c);
    }
  }
}

x16 SKW_ALLOC_NEW_CREATURE(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0)
{
  c_aispec* aiptrrg7;
  c_record* recoptrrg4;
  c_record* recoptrrg5;
  x16 vw_04;
  x16 vw_08;
  x16 vw_10;

  x16 vw_00 = eaxw;
  vw_08 = ecxw;
  x16 wordrg2 = eaxw & con(0x8000);
  if (wordrg2 != con(0x0))
  {
    vw_00 &= con(0x7fff);
    vw_10 = SKW_ALLOC_NEW_RECORD(con(0xe));
    if (vw_10 == con(0xffffffff))
      return vw_10;
  }
  vw_04 = SKW_ALLOC_NEW_RECORD(con(0x4));
  if (vw_04 == con(0xffffffff) || (UI16CAST(ddata.v1d4020) >= UI16CAST(ddata.v1e08a0)))
  {
    if (wordrg2 != con(0x0))
      SKW_DEALLOC_RECORD(vw_10);
    return con(0xffff);
  }
  recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(vw_04);
  recoptrrg5 = recoptrrg4;
  recoptrrg4->u4.b.b0 = CUTX8(vw_00);
  aiptrrg7 = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(CUTX8(vw_00));
  recoptrrg4->u2.w = con(0xfffffffe);
  recoptrrg4->ue.b.b1 &= con(0xfffffff8);
  recoptrrg4->ue.w |= (ebxw & con(0x3)) << con(0x8);
  recoptrrg4->u4.b.b1 = con(0xffffffff);
  if (wordrg2 != con(0x0))
    SKW_APPEND_RECORD_TO(vw_10, con(0xffffffff), con(0x0), &recoptrrg4->u2.w);
  x16 wordrg6 = urshift(aiptrrg7->w4 * (DM2_MIN(edxw, con(0x1f)) + con(0x1)), con(0x3));
  recoptrrg5->u6.w = wordrg6 + SKW_RAND16(wordrg6 / 8 + con(0x1));
  recoptrrg5->ua.w = con(0x0);
  recoptrrg5->uc.b.b0 &= con(0xffffffe0);
  x16 wordrg61 = recoptrrg5->uc.w | (vw_08 & con(0x1f));
  recoptrrg5->uc.w = (wordrg61 & con(0xfffffc1f)) | ((argw0 & con(0x1f)) << con(0x5));
  recoptrrg5->uc.b.b1 &= con(0x3);
  recoptrrg5->uc.w |= (ddata.v1d3248 & con(0x3f)) << con(0xa);
  recoptrrg5->w8 = con(0xffffffff);
  if (SKW_MOVE_RECORD_TO(vw_04, con(0x0), vw_08, con(0xfffffffc), argw0) == con(0x1))
    return con(0xffff);
  recoptrrg5->w8 = con(0x0);
  return vw_04;
}

// was SKW_1c9a_166f
x16 SKW_CREATURE_ATTACKS_CREATURE(x16 eaxw, x16 edxw)
{
  c_aispec* aiptrrg7;
  c_aispec* aiptrrg4;
  c_aispec* aiptrrg2;
  x16 vw_08;
  x16 vw_0c;

  vw_0c = SKW_GET_CREATURE_AT(eaxw, edxw);
  if (vw_0c == con(0xffffffff))
    return con(0xffffffff);
  aiptrrg4 = ddata.s350.v1e0552;
  aiptrrg2 = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(SKW_GET_ADDRESS_OF_RECORD(vw_0c)->u4.b.b0);
  aiptrrg7 = aiptrrg2;
  if (aiptrrg7->barr06[con(0x2)] == con(0xffffffff))
    return con(0x0);
  vw_08 = (CUTX16(SKW_RAND()) & con(0x1f)) + (aiptrrg4->barr06[con(0x2)]);
  if ((unsignedword(aiptrrg2->barr06[con(0x2)]) + (CUTX16(SKW_RAND()) & con(0x1f))) > vw_08)
    if (SKW_RAND02() != con(0x0))
      return con(0x0);
  x16 vw_00 = (unsignedword(aiptrrg7->b2) + (CUTX16(SKW_RAND()) & con(0x1f))) / 8;
  x16 wordrg5 = unsignedword(aiptrrg4->barr06[con(0x0)]);
  wordrg5 += DM2_MIN(aiptrrg4->barr06[con(0x0)], CUTX16(SKW_RAND()) & con(0xf));
  wordrg5 -= vw_00;
  if (wordrg5 <= con(0x1))
  {
    if (SKW_RAND01() != con(0x0))
      return con(0x0);
    wordrg5 = SKW_RAND02() + con(0x2);
  }
  wordrg5 += SKW_RAND02() + SKW_RAND16(wordrg5);
  wordrg5 += SKW_RAND16(wordrg5);
  wordrg5 >>= con(0x2);
  wordrg5 += SKW_RAND02() + con(0x1);
  if (SKW_RAND01() != con(0x0))
    wordrg5 -= SKW_RAND16(wordrg5 / 4 + con(0x1));
  SKW_ATTACK_CREATURE(vw_0c, edxw, con(0x2), eaxw, con(0x3c), wordrg5);
  return wordrg5;
}

void SKW_32cb_3f0d(x16 eaxw)
{
  x16 vw_08;
  x16 vw_0c;

  x16 wordrg5 = ddata.v1e1044[eaxw].s_02.warr_00[con(0x2)];
  if (wordrg5 == con(0xfffffffe))
    return;
  x16 vw_04 = con(0x2);
  do
  {
    if ((wordrg5 >> con(0xe)) == ((signedlong(ddata.v1e0258) + con(0x2)) & con(0x3)))
    {
      x16 wordrg44 = urshift(wordrg5 & con(0x3c00), con(0xa));
      if (wordrg44 < con(0x5) || wordrg44 > con(0xa))
      {
        if (wordrg44 == con(0xf))
        {
          x16 wordrg3 = eaxw;
          if (table1d6e51[wordrg3] != con(0xffffffff))
          {
            x8 byterg4 = CUTX8(SKW_GET_ADDRESS_OF_RECORD(wordrg5)->u2.w & con(0x7f));
            vw_08 = SKW_RAND02();
            bool dotemppic = true; // added
            if (byterg4 != con(0x64))
            {
              if (byterg4 != con(0x65))
                dotemppic = false;
              else
                SKW_32cb_2cf3(byterg4, vw_08, SKW_098d_0cd7(wordrg3, false, con(0xc)), unsignedword(table1d6b76[table1d6e51[wordrg3] + 0x58]));
            }
            else
            {
              vw_0c = table1d6b76[table1d6e51[wordrg3] + 0x5f];
              SKW_QUERY_TEMP_PICST(vw_08, vw_0c, con(0x0), vw_0c, con(0x0), con(0x0), SKW_098d_0cd7(wordrg3, false, con(0xc)), con(0xffffffff), con(0xa), con(0xffff), con(0xd), byterg4, con(0xc));
            }
            if (dotemppic)
              SKW_DRAW_TEMP_PICST();
          }
        }
      }
      else
      {
        SKW_DRAW_ITEM(wordrg5, con(0x0), con(0x0), eaxw, vw_04, NULL, true, con(0x0), true);
        vw_04++;
        if (vw_04 >= con(0xe))
          vw_04 = con(0x2);
        if (eaxw == con(0x3))
          SKW_MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE(con(0x8), con(0x3), con(0x3), wordrg5);
      }
    }
    wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg5);
  } while (wordrg5 != con(0xfffffffe));
}

// was SKW_32cb_4905
void SKW_DRAW_DOOR(x16 eaxw, x16 ebxw, x32 ecxl, x16 edxw)
{
  c_record* recoptrrg7;
  s_hex12* h12ptrrg1;
  x16 vw_04;
  c_image vs_08;
  x16 vw_148;
  x16 vw_14c;
  x16 vw_150;
  x16 vw_154;
  x16 vw_15c;
  x16 vw_160;
  x16 vw_168;
  x8 vb_170;
  x8 vb_174;
  x8 vb_178;

  vw_04 = eaxw;
  if (edxw != con(0x0))
    SKW_DRAW_DOOR_FRAMES(vw_04, edxw);
  if (ecxl != con(0x0))
    SKW_DRAW_STATIC_OBJECT(vw_04, false, ecxl);
  x16 wordrg4 = vw_04;
  h12ptrrg1 = &ddata.v1e1044[wordrg4];
  vw_168 = h12ptrrg1->s_02.warr_00[con(0x3)];
  if (vw_168 != con(0x0))
  {
    x16 wordrg2 = h12ptrrg1->s_02.warr_00[con(0x4)];
    x16 vw_16c = signedword(table1d6b15[wordrg4]);
    if (table1d6b15[wordrg4] <= con(0x3))
    {
      x16 wordrg17 = signedword(table1d6f27[vw_16c]);
      x16 vw_158 = wordrg17;
      if (wordrg17 >= con(0x0))
      {
        x16 wordrg18 = table1d6f2c[wordrg4];
        x16 vw_164 = wordrg18;
        if (wordrg18 >= con(0x0))
        {
          recoptrrg7 = SKW_GET_ADDRESS_OF_RECORD(wordrg2);
          vb_174 = ddata.v1e040e[recoptrrg7->u2.w & con(0x1)];
          vw_160 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xe), con(0xb), con(0x4), vb_174);
          if (vw_160 == con(0x0))
            vw_160 = con(0xa);
          vb_178 = CUTX8(vw_16c) - con(0x1);
          x16 wordrg6 = con(0x40);
          vw_150 = con(0x0);
          if (vw_16c == con(0x0) || !SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0xe), con(0x1), vb_178, vb_174))
          {
            if (vw_16c == con(0x0))
              wordrg6 = con(0x71);
            else
            {
              vw_150 = vw_16c;
              wordrg6 = unsignedword(table1d6b71[vw_16c]);
            }
            vb_178 = con(0x0);
          }
          x16 wordrg5 = ulrshift(recoptrrg7->u2.w, con(0xb), con(0xc));
          if (wordrg5 != con(0x0) || vw_168 == con(0x5))
          {
            vw_15c = SKW_QUERY_MULTILAYERS_PIC(&vs_08, vb_174, vb_178, con(0xe), wordrg6, wordrg6, vw_150, con(0x0), vw_160, con(0xffffffff));
            SKW_DRAW_PICST(&vs_08);
            SKW_0b36_01cd(&vs_08.s_00);
            if (wordrg6 != con(0x40))
              vw_154 = wordrg6;
            else
            {
              SKW_QUERY_GDAT_IMAGE_METRICS(con(0xe), con(0x0), &vw_148, vb_174, &vw_14c);
              wordrg6 = SKW_32cb_48d5(ddata.v1e118e.s_00.width, vw_148);
              vw_154 = SKW_32cb_48d5(ddata.v1e118e.s_00.height, vw_14c);
            }
            if (wordrg5 != con(0x0))
            {
              wordrg5--;
              vb_170 = ddata.v1e0414[wordrg5];
              x16 wordrg156 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xb), con(0xb), con(0x4), vb_170);
              if (wordrg156 == con(0x0))
                wordrg156 = con(0x9);
              SKW_QUERY_TEMP_PICST(con(0x0), vw_154, con(0x0), wordrg6, con(0x0), vw_16c, 4 * DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xb), con(0xb), con(0x8), vb_170) + con(0x7d0) + vw_158, con(0xffffffff), wordrg156, con(0xffff), con(0xb), vb_170, con(0x0));
              ddata.v1e118e.pg_2c = SKW_3e74_5817(vw_15c);
              SKW_DRAW_TEMP_PICST();
            }
            if (vw_168 == con(0x5))
            {
              x16 wordrg155 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xe), con(0xb), con(0x12), vb_174);
              if (wordrg155 == con(0x0))
                wordrg155 = con(0x9);
              SKW_QUERY_TEMP_PICST(con(0x0), vw_154, con(0x0), wordrg6, con(0x0), vw_16c, 4 * DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xe), con(0xb), con(0xa), vb_174) + con(0x7d0) + vw_158, con(0xffffffff), wordrg155, vw_160, con(0xe), vb_174, con(0x41));
              ddata.v1e118e.pg_2c = SKW_3e74_5817(vw_15c);
              SKW_DRAW_TEMP_PICST();
            }
            x16 wordrg165;
            if (vs_08.w_38 > con(0x0))
              wordrg165 = unsignedword(vs_08.xLocalPalette[vw_160]);
            else
              wordrg165 = unsignedword(vw_160);
            vs_08.w_30 = wordrg165;
            vs_08.w_38 = con(0x0);
            SKW_0b36_00c3(vw_15c, &vs_08.s_00);
            SKW_COPY_IMAGE(&vs_08, &ddata.v1e118e);
            ddata.v1e118e.pg_2c = ddata.bitmapptr;
          }
          else
          {
            vw_15c = con(0xffffffff);
            SKW_QUERY_TEMP_PICST(con(0x0), wordrg6, con(0x0), wordrg6, con(0x0), vw_150, con(0xffffffff), con(0xffffffff), vw_160, con(0xffff), con(0xe), vb_174, vb_178);
          }
          if (vw_168 < con(0x4))
          {
            x16 wordrg51 = vw_164 + vw_168;
            vw_164 = wordrg51;
            if ((recoptrrg7->u2.b.b0 & con(0x20)) == con(0x0))
            {
              x16 wordrg61 = ddata.v1e118e.s_00.width / 2;
              ddata.v1e118e.s_00.width = wordrg61;
              ddata.v1e118e.s_00.w_04 |= con(0x10);
              ddata.v1e118e.s_00.w_0e += wordrg61;
              ddata.v1e118e.w_18 = wordrg51 + con(0x6);
              SKW_DRAW_TEMP_PICST();
              ddata.v1e118e.s_00.w_0e -= wordrg61;
              vw_164 = wordrg51 + con(0x3);
            }
          }
          ddata.v1e118e.w_18 = vw_164;
          SKW_DRAW_TEMP_PICST();
          if (vw_15c >= con(0x0))
            SKW_3e74_58a3(vw_15c);
        }
      }
    }
  }
  if (ebxw != con(0x0))
    SKW_DRAW_DOOR_FRAMES(vw_04, ebxw);
}

bool SKW_1887_1191(void)
{
  x8 vb_00;
  x16 vw_04;

  vw_04 = signedword(ddata.s350.v1e0556->u.v.b1e);
  x16 wordrg6 = unsignedword(ddata.s350.v1e0556->u.v.b20);
  vb_00 = CUTX8(wordrg6 | con(0x80));
  if (!SKW_19f0_2813(vb_00, unsignedword(ddata.s350.v1e0562.u.b.b_07), ddata.v1e08a6, unsignedword(ddata.s350.v1e0562.u.b.b_06), ddata.v1e08a4, ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe)), signedword(ddata.s350.v1e0556->u.v.b1e)))
    return true;
  if ((wordrg6 != con(0x2)) && (vw_04 != con(0xffffffff)))
  {
    wordrg6 = SKW_1c9a_0732(signedword(ddata.s350.v1e0556->u.v.b1e), con(0xff), ddata.s350.v1e054e->u2.w);
    if (wordrg6 == con(0xfffffffe))
      return true;
    SKW_CUT_RECORD_FROM(wordrg6, con(0xffffffff), con(0x0), &ddata.s350.v1e054e->u2.w);
  }
  else
    wordrg6 = con(0xffff);
  R_3C1E5(ddata.s350.v1e0556->u.v.u18.w & con(0x1f), (unsignedword(ddata.s350.v1e0556->u.v.b1d) + con(0x2)) & con(0x3), ddata.s350.v1e054c, ulrshift(ddata.s350.v1e0556->u.v.u18.w, con(0x6), con(0xb)), wordrg6);
  return false;
}

bool SKW_1887_10db(void)
{
  x16 vw_00;

  x16 wordrg6 = con(0x0);
  vw_00 = SKW_GET_CREATURE_AT(ddata.v1e08a6, ddata.v1e08a4);
  if (vw_00 == con(0xffffffff) || (SKW_QUERY_CREATURE_AI_SPEC_FLAGS(vw_00) & con(0x1)) == con(0x0))
    return wordrg6 == con(0x0);
  do
  {
    x16 wordrg5 = SKW_1c9a_0732(signedword(ddata.s350.v1e0556->u.v.b1e), ddata.s350.v1e0556->u.v.b1c, SKW_GET_ADDRESS_OF_RECORD(vw_00)->u2.w);
    if (wordrg5 == con(0xfffffffe))
      return wordrg6 == con(0x0);
    wordrg6++;
    SKW_MOVE_RECORD_TO(wordrg5, ddata.v1e08a4, con(0xffffffff), ddata.v1e08a6, con(0xffff));
    wordrg5 &= con(0xffff3fff);
    SKW_APPEND_RECORD_TO(wordrg5, con(0xffffffff), con(0x0), &ddata.s350.v1e054e->u2.w);
  } while ((ddata.s350.v1e0556->u.v.b1e & con(0x40)) != con(0x0));
  return wordrg6 == con(0x0);
}

bool SKW_PLACE_MERCHANDISE(void)
{
  x16 vw_04;
  x16 vw_08;

  x16 vw_0c = con(0x0);
  vw_04 = ddata.v1e08a4;
  vw_08 = ddata.v1e08a6;
  x16 wordrg1 = SKW_GET_CREATURE_AT(vw_08, ddata.v1e08a4);
  if (wordrg1 == con(0xffffffff) || (SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg1) & con(0x1)) == con(0x0))
    return true;
  do
  {
    x16 wordrg6 = SKW_1c9a_0732(signedword(ddata.s350.v1e0556->u.v.b1e), con(0xff), ddata.s350.v1e054e->u2.w);
    if (wordrg6 == con(0xfffffffe))
      return vw_0c == con(0x0);
    vw_0c++;
    SKW_CUT_RECORD_FROM(wordrg6, con(0xffffffff), con(0x0), &ddata.s350.v1e054e->u2.w);
    x8 byterg1 = ddata.s350.v1e0556->u.v.b1c;
    if (byterg1 == con(0xffffffff))
      byterg1 = CUTX8(SKW_RAND02());
    wordrg6 &= con(0xffff3fff);
    wordrg6 |= unsignedword(byterg1) << con(0xe);
    if (SKW_IS_CONTAINER_MONEYBOX(wordrg6))
      SKW_GET_ADDRESS_OF_RECORD(wordrg6)->u6.b.b1 |= con(0x4);
    SKW_MOVE_RECORD_TO(wordrg6, con(0xffffffff), vw_08, con(0xffffffff), vw_04);
    if (vw_0c == con(0x1))
      SKW_QUEUE_NOISE_GEN2(SKW_QUERY_CLS1_FROM_RECORD(wordrg6), con(0x85), con(0xfe), SKW_QUERY_CLS2_FROM_RECORD(wordrg6), vw_08, vw_04, vw_0c, con(0x3a), con(0x80));
  } while ((ddata.s350.v1e0556->u.v.b1e & con(0x40)) != con(0x0));
  return vw_0c == con(0x0);
}

bool SKW_1887_0e19(void)
{
  x16 vw_00 = con(0x0);
  x16 wordrg6;
  x16 wordrg1 = SKW_GET_WALL_TILE_ANYITEM_RECORD(ddata.v1e08a6, ddata.v1e08a4);
  for (;; wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg6))
  {
    wordrg6 = wordrg1;
    if (wordrg6 == con(0xfffffffe))
      return vw_00 == con(0x0);
    if (urshift(wordrg6 & con(0x3c00), con(0xa)) != con(0x4))
      if (ddata.s350.v1e0556->u.v.b1c == con(0xffffffff) || urshift(wordrg6, con(0xe)) == unsignedword(ddata.s350.v1e0556->u.v.b1c))
        if (SKW_1c9a_0006(wordrg6, signedword(ddata.s350.v1e0556->u.v.b1e)) != con(0x0))
        {
          SKW_MOVE_RECORD_TO(wordrg6, ddata.v1e08a4, con(0xffffffff), ddata.v1e08a6, con(0x0));
          SKW_APPEND_RECORD_TO(wordrg6, con(0xffffffff), con(0x0), &ddata.s350.v1e054e->u2.w);
          vw_00++;
          if ((ddata.s350.v1e0556->u.v.b1e & con(0x40)) == con(0x0))
            return vw_00 == con(0x0);
        }
  }
}

bool SKW_1887_0d33(void)
{
  x16 wordrg6 = con(0x0);
  do
  {
    x16 wordrg5 = SKW_1c9a_0732(signedword(ddata.s350.v1e0556->u.v.b1e), con(0xff), ddata.s350.v1e054e->u2.w);
    if (wordrg5 == con(0xfffffffe))
      return wordrg6 == con(0x0);
    wordrg6++;
    SKW_CUT_RECORD_FROM(wordrg5, con(0xffffffff), con(0x0), &ddata.s350.v1e054e->u2.w);
    wordrg5 &= con(0xffff3fff);
    wordrg5 |= unsignedword(ddata.s350.v1e0556->u.v.b1c) << con(0xe);
    SKW_MOVE_RECORD_TO(wordrg5, con(0x0), ddata.v1e08a6, con(0xffffffff), ddata.v1e08a4);
    if (wordrg6 == con(0x1))
      SKW_QUEUE_NOISE_GEN2(SKW_QUERY_CLS1_FROM_RECORD(wordrg5), con(0x85), con(0xfe), SKW_QUERY_CLS2_FROM_RECORD(wordrg5), ddata.v1e08a6, ddata.v1e08a4, wordrg6, con(0x3a), con(0x80));
  } while ((ddata.s350.v1e0556->u.v.b1e & con(0x40)) != con(0x0));
  return wordrg6 == con(0x0);
}

bool SKW_1887_0ce1(void)
{
  bool boolrg4;

  boolrg4 = false;
  if (ddata.s350.v1e0556->u.v.b1f == con(0x0))
  {
    x8 byterg1 = CUTX8(ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe)));
    SKW_1887_05aa();
    ddata.s350.v1e0556->u.v.b1d = byterg1;
  }
  else if (ddata.s350.v1e0556->u.v.b1f == con(0x1))
  {
    SKW_1887_05aa();
    boolrg4 = SKW_1887_0e19();
  }
  ddata.s350.v1e0556->u.v.b1f += con(0x1);
  return boolrg4;
}

bool SKW_1887_09ab(void)
{
  c_aispec* aiptrrg5;

  aiptrrg5 = ddata.s350.v1e0552;
  if (UI8CAST(ddata.s350.v1e0556->u.v.b20) <= UI8CAST(con(0x1)))
  {
    ddata.longv1d26a4 = ddata.longmallocx;
  }
  if (ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe)) != signedword(ddata.s350.v1e0556->u.v.b1b) && ((aiptrrg5->u0.b.b0 & con(0x4)) == con(0x0)))
    return true;
  x16 vw_00 = unsignedword(aiptrrg5->barr06[con(0x0)]) / 4 + con(0x1);
  x16 wordrg41 = vw_00 + SKW_RAND16(vw_00);
  vw_00 = wordrg41 + SKW_RAND16(wordrg41);
  ui8 ubyterg1 = ddata.s350.v1e0556->u.v.b1e;
  if (ubyterg1 < UI8CAST(con(0xffffff87)))
  {
    if (ubyterg1 == con(0xffffff86))
      vw_00 *= 4;
  }
  else if (ubyterg1 <= UI8CAST(con(0xffffff87)) || (ubyterg1 >= UI8CAST(con(0xffffff89)) && ubyterg1 <= UI8CAST(con(0xffffff8a))))
    vw_00 *= 8;
  vw_00 = DM2_BETWEEN_VALUE(con(0x4), con(0xff), vw_00);
  x8 byterg4;
  if (vw_00 > con(0x8))
  {
    if (vw_00 > con(0x10))
    {
      if (vw_00 > con(0x20))
        byterg4 = CUTX8(SKW_RAND02() + con(0x7));
      else
        byterg4 = CUTX8(SKW_RAND01() + con(0x3));
    }
    else
      byterg4 = con(0x2);
  }
  else
    byterg4 = con(0x1);
  SKW_SHOOT_ITEM(signedword(ddata.s350.v1e0556->u.v.b1e), unsignedword(ddata.s350.v1e0562.u.b.b_07), unsignedword(ddata.s350.v1e0556->u.v.b1c), unsignedword(ddata.s350.v1e0562.u.b.b_06), ddata.s350.v1e0556->u.v.b1b, CUTX8(vw_00), aiptrrg5->barr06[con(0x2)], byterg4);
  if ((aiptrrg5->barr06[con(0x3)] & con(0x10)) == con(0x0))
    return false;
  if (!SKW_WOUND_CREATURE(SKW_RAND01() + (vw_00 >> con(0x1))))
    return false;
  ddata.s350.v1e0570 = con(0x1);
  return true;
}

x16 SKW_2066_1ec9(x16 eaxw, x16 edxw)
{
  x16 wordrg5 = eaxw;
  if (wordrg5 == con(0xfffffffe))
    return edxw;
  if (edxw == con(0xfffffffe))
    return wordrg5;
  x16 wordrg44 = edxw;
  for(;;)
  {
    if (wordrg44 == con(0xfffffffe))
      break;
    if (urshift(wordrg44 & con(0x3c00), con(0xa)) >= con(0x4))
      break;
    x16 wordrg2 = wordrg5;
    wordrg5 = wordrg44;
    wordrg44 = SKW_GET_NEXT_RECORD_LINK(wordrg44);
    SKW_GET_ADDRESS_OF_RECORD(wordrg5)->w0 = wordrg2;
  }
  SKW_APPEND_RECORD_TO(wordrg44, con(0xffffffff), con(0xffffffff), &SKW_GET_ADDRESS_OF_RECORD(wordrg5)->w0);
  return wordrg5;
}

// called with parameter 0 only
bool SKW_2066_03e0(x16 eaxw)
{
  bool boolrg3;

  x16 wordrg2 = eaxw;
  x16 byterg4 = con(0x0);
  if (ddata.v1d154a)
    return true;
  boolrg3 = true;
  if (SKW_476d_030a(boolrg3 ? 1 : 0) == con(0x0))
    if (wordrg2 != con(0x2))
      return boolrg3;
  ddata.v1e0992 = con(0x1);
  for(;;)
  {
    if (wordrg2 == con(0x0))
      if (SKW_476d_030a(con(0x1)) == con(0x0))
        return boolrg3;
    SKW_38c8_0002();
    wordrg2 = con(0x0);
    boolrg3 = false;
    R_148D(con(0x1));
    x8 byterg1;
    if (!ddata.gdatfiletype2)
      byterg1 = con(0x7);
    else
      byterg1 = ((!ddata.gdatfiletype1) ? 1 : 0) + con(0x13);
    SKW_0aaf_0067(SKW_0aaf_02f8(byterg1, byterg4));
    byterg4 = con(0x14);
  }
}

// was SKW_24a5_0930
x16 SKW_PUT_OBJECT_INTO_CONTAINER(x16 eaxw)
{
  c_record* recoptrrg6;

  if (ddata.v1d6700 == con(0xffffffff))
    return eaxw;
  recoptrrg6 = SKW_GET_ADDRESS_OF_RECORD(ddata.v1d6700);
  ddata.v1d6700 = con(0xffffffff);
  x16 wordrg5;
  for (wordrg5 = con(0x0); wordrg5 < con(0x8); wordrg5++)
  {
    x16 wordrg2 = ddata.v1e095c[wordrg5];
    if (wordrg2 != con(0xffffffff))
    {
      ddata.v1e095c[wordrg5] = con(0xffffffff);
      SKW_APPEND_RECORD_TO(wordrg2, con(0xffffffff), con(0x0), &recoptrrg6->u2.w);
    }
  }
  return wordrg5;
}

//x16 SKW_QUERY_GDAT_CREATURE_WORD_VALUE(x8 eaxb, x8 edxb)
x16 SKW_QUERY_GDAT_CREATURE_WORD_VALUE(XCLS16 eaxb, x8 edxb)	// SPX fix to avoid negative values; eaxb is the Creature index
{
  x8 byterg4h;
  ui16 byterg6;	//x8 byterg6;
  x8* bptrrg5;
  x16 vw_00; // BUGFIX 41/2014
  bool doit = false; // added

  if ((ddata.v1e02ec != NULL) && (UI8CAST(eaxb) <= UI8CAST(ddata.v1e0414[0x11])))
  {
    if (UI8CAST(edxb) < UI8CAST(con(0x1)))
    {
      if (edxb == con(0x0))
      {
        byterg4h = con(0x1);
        doit = true;
      }
    }
    else if (UI8CAST(edxb) <= UI8CAST(con(0x1)))
    {
      byterg4h = con(0x0);
      doit = true;
    }
    else if (edxb == con(0x5))
    {
      byterg4h = con(0x2);
      doit = true;
    }
  }
  if (doit)
  {
    byterg6 = eaxb;
    bptrrg5 = ddata.v1e02ec + 3 * byterg6 + byterg4h;
    vw_00 = unsignedword(*bptrrg5);
    if (vw_00 == con(0xff))
    {
      //vw_00 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xf), con(0xb), edxb, byterg6);
	  vw_00 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_CREATURES, dtWordValue, edxb, byterg6);
      *bptrrg5 = CUTX8(vw_00);
    }
    return vw_00;
  }
  //return DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xf), con(0xb), edxb, eaxb);
  return DM2_QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_CREATURES, dtWordValue, edxb, eaxb);
}

x16 SKW_1031_03f2(s_bbw* eaxpbbw, x16 edxw)
{
  x8* bptrrg5;
  s_bbw* sbbwptrrg3;
  s_2w* ptrrg11;
  s_www* ptrrg12;

  x16 wordrg6 = con(0x0);
  bptrrg5 = SKW_1031_023b(eaxpbbw);
  do
  {
    sbbwptrrg3 = &table1d3ba0[*bptrrg5 & con(0x7f)];
    bool boolrg1;
    ui8 ubyterg1 = eaxpbbw->b0 & con(0x7f);
    switch (ubyterg1)
    {
      case 0:
        boolrg1 = SKW_RETURN_1() ? 1 : 0;
      break;

      case 1:
        boolrg1 = SKW_IS_GAME_ENDED(sbbwptrrg3) ? 1 : 0;
      break;

      case 2:
        boolrg1 = SKW_1031_0023(sbbwptrrg3);
      break;

      case 3:
        boolrg1 = SKW_1031_003e(sbbwptrrg3);
      break;

      case 4:
        boolrg1 = SKW_1031_007b(sbbwptrrg3);
      break;

      case 5:
        boolrg1 = SKW_1031_009e(sbbwptrrg3);
      break;

      case 6:
        boolrg1 = SKW_1031_00c5(sbbwptrrg3);
      break;

      case 7:
        boolrg1 = SKW_1031_00f3(sbbwptrrg3);
      break;

      case 8:
        boolrg1 = SKW_1031_012d(sbbwptrrg3);
      break;

      case 9:
        boolrg1 = SKW_1031_014f(sbbwptrrg3);
      break;

      case 10:
        boolrg1 = SKW_1031_0184(sbbwptrrg3);
      break;

      case 11:
        boolrg1 = SKW_1031_01ba(sbbwptrrg3);
      break;

      default: throw(THROW_DMABORT);
    }
    if (boolrg1)
    {
      if ((sbbwptrrg3->b0 & con(0x80)) != con(0xffffff80))
      {
        if (table1d3d23[sbbwptrrg3->w2].w4 != con(0xffffffff))
        {
          ptrrg11 = &v1d39bc[table1d3d23[sbbwptrrg3->w2].w4];
          for(;;) // TODO: ugly loopform
          {
            if (edxw != ptrrg11->w0[0x1])
            {
              ptrrg11++;
              if ((ptrrg11->w0[con(0x0)] & con(0xffff8000)) == con(0x0))
                continue;
            }
            else
            {
              wordrg6 = ptrrg11->w0[con(0x0)] & con(0x7ff);
              ptrrg12 = SKW_1031_024c(sbbwptrrg3);
              SKW_1031_0c58(wordrg6, ptrrg12);
              ddata.v1e051e = edxw;
            }
            break;
          }
          if (wordrg6 != con(0x0))
            return wordrg6;
        }
      }
      else
      {
        wordrg6 = SKW_1031_03f2(sbbwptrrg3, edxw);
        if (wordrg6 != con(0x0))
          return wordrg6;
      }
    }
    bptrrg5++;
  } while ((*bptrrg5 & con(0xffffff80)) == con(0x0));
  return wordrg6;
}

void SKW_1031_0541(x16 eaxw)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("SKW_1031_0541");
  bool boolrg3;
  bool boolrg5;
  s_munknown* sptrrg4;
  s_wwwb* swwwbptrrg1;

  if (eaxw != ddata.v1d3ff1)
    SKW_1031_098e();
  ddata.v1d3ff1 = eaxw;
  SKW_1031_027e(&table1d3ed5[eaxw]);
  swwwbptrrg1 = table1d3d23;
  x16 wordrg21 = con(0x0);
  while (wordrg21 < con(0x3e))
  {
    x16 wordrg400 = unsignedword(swwwbptrrg1->b6);
    boolrg5 = (wordrg400 & con(0x40)) == con(0x0);
    boolrg3 = (wordrg400 & con(0x80)) == con(0x0);
    if (boolrg5 != boolrg3)
    {
      x16 wordrg31 = wordrg400 & con(0x80);
      wordrg400 &= con(0x3f);
      if (wordrg31 == con(0x0))
      {
        if (wordrg400 != con(0x0))
          table1d32d8[wordrg400 - con(0x1)].b3 |= con(0x10); // EVIL index shifted, was 0x1d32db
        swwwbptrrg1->b6 |= con(0xffffff80);
      }
      else
      {
        if (wordrg400 != con(0x0))
          table1d32d8[wordrg400 - con(0x1)].b3 |= con(0x20); // EVIL index shifted, was 0x1d32db
        swwwbptrrg1->b6 &= con(0x7f);
      }
    }
    swwwbptrrg1->b6 &= con(0xffffffbf);
    swwwbptrrg1++;
    wordrg21++;
  }
  sptrrg4 = table1d32d8;
  x16 wordrg22 = con(0x0);
  while (wordrg22 < con(0x12))
  {
    x8 byterg1 = sptrrg4->b3 & con(0x10);
    boolrg5 = (byterg1 == con(0x0)) ? 1 : 0;
    boolrg3 = ((sptrrg4->b3 & con(0x20)) == con(0x0));
    if (boolrg5 != boolrg3)
    {
      if (byterg1 == con(0x0))
      {
        SKW_443c_07d5(sptrrg4);
      }
      else
      {
        SKW_443c_06b4(sptrrg4);
      }
    }
    sptrrg4->b3 &= con(0xffffffcf);
    sptrrg4++;
    wordrg22++;
  }
SPX_DEBUG_POP;
}

s_www* SKW_1031_06b3(s_bbw* eaxpbbw, x16 edxw)
{
  x8* bptrrg5; // sort of byte* because of ++
  s_www* ptrrg1;
  s_bbw* sbbwptrrg3;

  bptrrg5 = SKW_1031_023b(eaxpbbw);
  do
  {
    bool boolrg1;
    sbbwptrrg3 = &table1d3ba0[*bptrrg5 & con(0x7f)];
    switch ((eaxpbbw->b0 & con(0x7f)) + con(0x5))
    {
      case 0:
        boolrg1 = SKW_RETURN_1();
      break;

      case 1:
        boolrg1 = SKW_IS_GAME_ENDED(sbbwptrrg3);
      break;

      case 2:
        boolrg1 = SKW_1031_0023(sbbwptrrg3);
      break;

      case 3:
        boolrg1 = SKW_1031_003e(sbbwptrrg3);
      break;

      case 4:
        boolrg1 = SKW_1031_007b(sbbwptrrg3);
      break;

      case 5:
        boolrg1 = SKW_1031_009e(sbbwptrrg3);
      break;

      case 6:
        boolrg1 = SKW_1031_00c5(sbbwptrrg3);
      break;

      case 7:
        boolrg1 = SKW_1031_00f3(sbbwptrrg3);
      break;

      case 8:
        boolrg1 = SKW_1031_012d(sbbwptrrg3);
      break;

      case 9:
        boolrg1 = SKW_1031_014f(sbbwptrrg3);
      break;

      case 10:
        boolrg1 = SKW_1031_0184(sbbwptrrg3);
      break;

      case 11:
        boolrg1 = SKW_1031_01ba(sbbwptrrg3);
      break;

      default: throw(THROW_DMABORT);
    }
    if (boolrg1)
    {
      if ((sbbwptrrg3->b0 & con(0x80)) != con(0xffffff80))
      {
        ptrrg1 = SKW_1031_024c(sbbwptrrg3);
        if (ptrrg1 != NULL)
        {
          for(;;)
          {
            x16 wordrg4 = ptrrg1->w0[con(0x0)] & con(0x7ff);
            if (wordrg4 == con(0x0))
              break;
            if (edxw == wordrg4)
              return ptrrg1;
            ptrrg1++;
          }
        }
      }
      else
      {
        ptrrg1 = SKW_1031_06b3(sbbwptrrg3, edxw);
        if (ptrrg1 != NULL)
          return ptrrg1;
      }
    }
    bptrrg5++;
  } while ((*bptrrg5 & con(0xffffff80)) == con(0x0));
  return NULL;
}

void SKW_1031_07d6(void)
{
  s_bbw* sbbwptrrg11;
  s_bbw* sbbwptrrg12;
  s_wwwb* swwwbptrrg1;
  x16 vwa_00[0x40];
  x16 vwa_80[0x12];
  x8 vba_a4[0x1c];

  x16 wordrg12 = con(0x0);
  ui16 uwordrg41 = con(0x0);
  while (uwordrg41 < con(0x108))
  {
    if ((v1d338c[uwordrg41].w0[con(0x0)] & con(0xffff8000)) != con(0x0))
    {
      vwa_00[wordrg12] = uwordrg41;
      wordrg12++;
    }
    uwordrg41++;
  }
  x16 wordrg11 = con(0x0);
  ui16 uwordrg42 = con(0x0);
  while (uwordrg42 < con(0x79))
  {
    if ((v1d39bc[uwordrg42].w0[con(0x0)] & con(0xffff8000)) != con(0x0))
    {
      vwa_80[wordrg11] = uwordrg42;
      wordrg11++;
    }
    uwordrg42++;
  }
  x16 wordrg1 = con(0x0);
  ui16 uwordrg43 = con(0x0);
  while (uwordrg43 < con(0x53))
  {
    if ((table1d3cd0[uwordrg43] & con(0xffffff80)) != con(0x0))
    {
      vba_a4[wordrg1] = CUTX8(uwordrg43);
      wordrg1++;
    }
    uwordrg43++;
  }
  x16 wordrg44 = con(0x0);
  while (wordrg44 < con(0x4c))
  {
    sbbwptrrg11 = &table1d3ba0[wordrg44];
    if ((sbbwptrrg11->b0 & con(0xffffff80)) != con(0x0))
      sbbwptrrg11->w2 = unsignedword(vba_a4[sbbwptrrg11->w2]);
    wordrg44++;
  }
  x16 wordrg45 = con(0x0);
  while (UI16CAST(wordrg45) < con(0xa))
  {
    sbbwptrrg12 = &table1d3ed5[wordrg45];
    if ((sbbwptrrg12->b0 & con(0xffffff80)) != con(0x0))
      sbbwptrrg12->w2 = unsignedword(vba_a4[sbbwptrrg12->w2]);
    wordrg45++;
  }
  x16 wordrg46 = con(0x0);
  while (wordrg46 < con(0x3e))
  {
    swwwbptrrg1 = table1d3d23 + wordrg46;
    if (swwwbptrrg1->w2 != con(0xffffffff))
      swwwbptrrg1->w2 = vwa_00[swwwbptrrg1->w2];
    if (swwwbptrrg1->w4 != con(0xffffffff))
      swwwbptrrg1->w4 = vwa_80[swwwbptrrg1->w4];
    wordrg46++;
  }
  x16 wordrg47 = con(0x0);
  for(;;)
  {
    if (wordrg47 >= con(0x12))
      return;
    SKW_443c_067a(&table1d32d8[wordrg47]);
    wordrg47++;
  }
}

x16 SKW_1031_0a88(s_www* eaxps, x16 ebxw, x16 ecxw, x16 edxw)
{
  s_www* ptrrg5;
  x16 vw_04;

  ptrrg5 = eaxps;
  vw_04 = ebxw;
  if (eaxps == NULL)
    return con(0x0);
  do
  {
    if ((ptrrg5->w0[con(0x2)] & con(0x800)) == con(0x0))
    {
      if ((unsignedword(CUTX8(ptrrg5->w0[con(0x2)])) & ecxw) != con(0x0) && SKW_1031_01d5(ptrrg5->w0[con(0x1)], &ddata.v1e0470) != NULL)
      {
        if (SKW_PT_IN_RECT(&ddata.v1e0470, edxw, vw_04))
        {
          ddata.mouse_unk05 = ptrrg5->w0[con(0x1)];
          if ((ddata.mouse_unk05 & con(0x8000)) == con(0x0))
          {
            if ((ddata.mouse_unk05 & con(0x4000)) == con(0x0))
            {
              ddata.mouse_unk09 = con(0xffffffff);
            }
            else
            {
              ddata.mouse_unk09 = con(0x12);
            }
          }
          else
          {
            ddata.mouse_unk09 = con(0x7);
          }
          ddata.mouse_unk05 &= con(0x3fff);
          ddata.mouse_unk08 = edxw;
          ddata.mouse_unk07 = vw_04;
          ddata.mouse_unk0a = ptrrg5->w0[con(0x0)] & con(0x7ff);
          x16 vw_00 = ddata.mouse_unk0a;
          ddata.v1e051e = con(0x0);
          ddata.mouse_unk06 = vw_00 - ((eaxps->w0[con(0x0)] & con(0x7ff)) - con(0x1));
          return vw_00;
        }
      }
    }
    ptrrg5++;
  } while ((ptrrg5->w0[con(0x0)] & con(0xffff8000)) == con(0x0));
  ddata.v1e0470.h = con(0x0);
  ddata.v1e0470.w = con(0x0);
  return con(0x0);
}

// was SKW_1031_0f3a
void SKW_IBMIO_USER_INPUT_CHECK(void)
{
  bool flag;
  s_3words vs_00;
  x16 vw_08;

  if (ddata.v1e0484)
    return;
  x32 longrg32 = ddata.longgametime - ddata.longv1d3fed;
  flag = longrg32 == con(0x0);
  vw_08 = CUTX16(longrg32);
  if (flag)
    return;
  ddata.longv1d3fed = ddata.longgametime;
  if ((ddata.mouse_unk02 == con(0x0) && !ddata.v1e0478) && !ddata.v1e048c)
  {
    bool special = false; // added
    while (ddata.v1e0480 == NULL)
    {
      for(;;)
      {
        if (SKW_476d_05a9() == con(0x0))
          break;
        SKW_1031_0d36(con(0x20), SKW_476d_050e());
      }
      ddata.fetchmouse_busy = true;
      bool boolrg7 = true;
      while (ddata.mousebufentries != con(0x0))
      {
        SKW_COPY_MEMORY(DOWNCAST(c_mousebufentry, &ddata.mousebuf[ddata.mouse_unk01]), 0x6, DOWNCAST(s_3words, &vs_00));
        ddata.mousebufentries--;
        ddata.mouse_unk01++;
        if (ddata.mouse_unk01 > con(0xa))
          ddata.mouse_unk01 = con(0x0);
        x16 wordrg1 = con(0x0);
        if (vs_00.w_00 < con(0x20))
        {
          if ((vs_00.w_00 & con(0x4)) == con(0x0))
          {
            if ((vs_00.w_00 & con(0x13)) != con(0x0))
              wordrg1 = SKW_1031_030a(&table1d3ed5[ddata.v1d3ff1], vs_00.w_04, vs_00.w_00, vs_00.w_02); // 1st par adj
          }
          else
            wordrg1 = con(0xe3);
        }
        else
        {
          if (vs_00.w_00 != con(0x20))
          {
            if (vs_00.w_00 != con(0x40))
            {
              if (vs_00.w_00 == con(0x60))
                wordrg1 = con(0xe1);
            }
            else
              wordrg1 = con(0x81);
          }
          else
            wordrg1 = SKW_1031_03f2(&table1d3ed5[ddata.v1d3ff1], vs_00.w_02); // 1st par adj
        }
        if (wordrg1 != con(0x0))
        {
          boolrg7 = false;
          ddata.v1e04d2.warr0[2] = wordrg1;
          ddata.v1e04d2.warr0[0] = ddata.mouse_unk08;
          ddata.v1e04d2.warr0[1] = ddata.mouse_unk07;
          ddata.v1e04d2.r6.y = ddata.v1e0470.y;
          ddata.v1e04d2.r6.x = ddata.v1e0470.x;
          ddata.v1e04d2.r6.w = ddata.v1e0470.w;
          ddata.v1e04d2.r6.h = ddata.v1e0470.h;
        }
        if (!boolrg7)
        {
          ddata.fetchmouse_busy = false;
          SKW_GETSINGLEMOUSEEVENT();
          ddata.v1e0480 = UPCAST(t_gfxdata, SKW_TRANSMIT_UI_EVENT(&ddata.v1e04d2));
          vw_08 = con(0x0);
          special = true;
          break;
        }
      }
      if (!special)
      {
        ddata.fetchmouse_busy = false;
        SKW_GETSINGLEMOUSEEVENT();
        break;
      }
    }
  }
  SKW_1031_111e(vw_08);
}

// was SKW_1031_16fd
void SKW_CLICK_MAGICAL_MAP_AT(x16 eaxw, x16 ebxw, x16 edxw)
{
  c_record* recoptrrg5;
  x16 vw_00;
  c_rect rc_04;
  c_record* prec_0c;
  x16 vw_14;
  x16 vw_18;
  x16 vw_1c;

  vw_00 = eaxw;
  x16 wordrg31 = edxw;
  vw_1c = ddata.hero[ddata.v1e0b74 - con(0x1)].warrc3[ddata.v1e0b76];
  x16 wordrg6 = vw_1c;
  recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
  prec_0c = recoptrrg5;
  if (vw_00 != con(0x55) || (recoptrrg5->u4.w & con(0xe000)) != con(0x2000))
    return;
  SKW_QUERY_EXPANDED_RECT(con(0x63), &rc_04);
  wordrg31 -= rc_04.x - ddata.v1e0b5a;
  x16 wordrg22 = ebxw - (rc_04.y - ddata.v1e0b58);
  if ((wordrg31 % (ddata.v1d274a + ddata.v1d274e)) < ddata.v1d274e)
    return;
  if ((wordrg22 % (ddata.v1d274c + ddata.v1d2750)) < ddata.v1d2750 || SKW_GET_MISSILE_REF_OF_MINION(recoptrrg5->u2.w, wordrg6) == NULL)
    return;
  x16 wordrg3 = wordrg31 / (ddata.v1d274e + ddata.v1d274a);
  x16 wordrg21 = wordrg22 / (ddata.v1d274c + ddata.v1d2750);
  vw_18 = signedword(ddata.v1e0b82);
  vw_14 = signedword(ddata.v1e0b83);
  wordrg3 -= con(0x3);
  SKW_CALC_VECTOR_W_DIR(ddata.v1e0b84, wordrg3, &vw_18, con(0x3) - wordrg21, &vw_14);
  x16 wordrg2 = SKW_LOCATE_OTHER_LEVEL(ddata.v1e0b81, &vw_18, &vw_14, con(0x0), NULL);
  if (wordrg2 < con(0x0))
    return;
  x16 wordrg5 = ddata.v1d3248;
  SKW_CHANGE_CURRENT_MAP_TO(wordrg2);
  x8 byterg4 = SKW_GET_TILE_VALUE(vw_18, vw_14);
  x16 wordrg1 = urshift(unsignedlong(byterg4), con(0x5));
  if (wordrg1 != con(0x0) && wordrg1 != con(0x7))
  {
    if (wordrg1 != con(0x6))
      wordrg1 = con(0x1);
    else
    {
      wordrg1 = unsignedword(byterg4 & con(0x5));
      if (wordrg1 != con(0x0))
        wordrg1 = con(0x1);
    }
    if (wordrg1 != con(0x0))
    {
      if (vw_18 == ddata.v1e0b6e)
      {
        if (vw_14 == ddata.v1e0b70 && wordrg2 == ddata.v1e0b64)
        {
          if (ddata.v1e0b56 != con(0x1) || (ddata.v1e0b60 < con(0x0)))
          {
            if (ddata.v1e0b66 >= con(0x0))
            {
              if ((ddata.v1e0b56 == con(0x1)) || (ddata.v1e0b56 == con(0x3)))
              {
                vw_18 = ddata.v1e0b5e;
                vw_14 = ddata.v1e0b5c;
                wordrg2 = ddata.v1e0b66;
              }
            }
          }
          else
          {
            vw_18 = ddata.v1e0b68;
            vw_14 = ddata.v1e0b6a;
            wordrg2 = ddata.v1e0b60;
          }
        }
      }
      SKW_SET_DESTINATION_OF_MINION_MAP(vw_1c, vw_14, wordrg2, vw_18);
      SKW_1c9a_0247(prec_0c->u2.w);
      SKW_UPDATE_RIGHT_PANEL(false);
    }
  }
  SKW_CHANGE_CURRENT_MAP_TO(wordrg5);
}

bool SKW_HANDLE_UI_EVENT(c_event* eaxps)
{
  t_text t_00[0x28];
  t_text* tptrrg2;

  ui16 uwordrg1 = eaxps->warr0[con(0x2)];
  x16 wordrg3 = eaxps->warr0[con(0x0)];
  x16 wordrg4 = eaxps->warr0[con(0x1)];
  if (((uwordrg1 < con(0x3)) || (uwordrg1 > con(0x6))) || (ddata.v1e026e == con(0x0) && ((ddata.v1e025e == con(0x0)) || (unsignedlong(ddata.v1e0274) != ((unsignedlong(uwordrg1) + signedlong(ddata.v1e0258) - con(0x3)) & con(0x3))))))
  {
    if (((ddata.v1e025c == con(0x0)) || (((uwordrg1 != con(0x1)) && (uwordrg1 != con(0x2))) && (uwordrg1 != con(0x50)))) && ((uwordrg1 != con(0x50)) || ((((ddata.v1e040a == ddata.v1e0270) && (ddata.v1e040c == ddata.v1e0272)) && (ddata.v1e0406 == ddata.v1e0266)) && (ddata.v1e0402 == ddata.v1e0258))))
    {
      if (ddata.v1e0526 == con(0x0))
      {
        ddata.v1e0488 = false;
        if (uwordrg1 != con(0xe3))
        {
          if (uwordrg1 != con(0xe1))
          {
            if ((uwordrg1 != con(0x2)) && (uwordrg1 != con(0x1)))
            {
              if ((uwordrg1 < con(0x3)) || (uwordrg1 > con(0x6)))
              {
                if ((uwordrg1 < con(0x14)) || (uwordrg1 >= con(0x42)))
                {
                  if ((uwordrg1 < con(0xea)) || (uwordrg1 > con(0xed)))
                  {
                    if ((uwordrg1 < con(0x7d)) || (uwordrg1 > con(0x81)))
                    {
                      if ((uwordrg1 < con(0xe4)) || (uwordrg1 > con(0xe9)))
                      {
                        if ((uwordrg1 != con(0x5d)) && (uwordrg1 != con(0x5e)))
                        {
                          if (uwordrg1 < con(0x10) || uwordrg1 > con(0x13))
                          {
                            if (uwordrg1 < con(0x7) || uwordrg1 > con(0xb))
                            {
                              if (uwordrg1 != con(0x52))
                              {
                                if (uwordrg1 != con(0x6c))
                                {
                                  if (uwordrg1 != con(0x6b))
                                  {
                                    if ((uwordrg1 < con(0x65)) || (uwordrg1 > con(0x6a)))
                                    {
                                      if (uwordrg1 != con(0x70))
                                      {
                                        if (uwordrg1 < con(0x71) || uwordrg1 > con(0x73))
                                        {
                                          if (uwordrg1 < con(0x74) || uwordrg1 > con(0x7b))
                                          {
                                            if (uwordrg1 < con(0x5f) || uwordrg1 > con(0x62))
                                            {
                                              if (uwordrg1 != con(0x46))
                                              {
                                                if (uwordrg1 != con(0x47))
                                                {
                                                  if (uwordrg1 != con(0x48))
                                                  {
                                                    if (uwordrg1 != con(0x50))
                                                    {
                                                      if (uwordrg1 != con(0xf0))
                                                      {
                                                        if (uwordrg1 != con(0xf1))
                                                        {
                                                          if (uwordrg1 != con(0xf2))
                                                          {
                                                            if (uwordrg1 != con(0x55))
                                                            {
                                                              if ((uwordrg1 < con(0x56)) || (uwordrg1 > con(0x59)))
                                                              {
                                                                if (uwordrg1 != con(0x90))
                                                                {
                                                                  if (uwordrg1 != con(0x91))
                                                                  {
                                                                    if (!ddata.v1e0228 && !ddata.v1e0220 && !ddata.v1e0224 && !ddata.v1e03a8)
                                                                    {
                                                                      if (uwordrg1 != con(0x8e))
                                                                      {
                                                                        if (uwordrg1 != con(0x8f))
                                                                        {
                                                                          if (uwordrg1 != con(0x8c))
                                                                          {
                                                                            if (uwordrg1 != con(0x8d))
                                                                            {
                                                                              if (uwordrg1 != con(0x92) && uwordrg1 != con(0x93))
                                                                              {
                                                                                if (uwordrg1 != con(0xd7))
                                                                                {
                                                                                  if (uwordrg1 != con(0xd8))
                                                                                  {
                                                                                    if (uwordrg1 != con(0xd9))
                                                                                    {
                                                                                      if (uwordrg1 != con(0xda))
                                                                                      {
                                                                                        if (uwordrg1 == con(0xe0))
                                                                                          SKW_SK_PREPARE_EXIT();
                                                                                      }
                                                                                      else
                                                                                        SKW_SHOW_CREDITS();
                                                                                    }
                                                                                    else
                                                                                      ddata.savegamew8 = con(0x0);
                                                                                  }
                                                                                  else
                                                                                  {
                                                                                    ddata.savegamew8 = con(0x1);
                                                                                    ddata.savegamew1 = true;
                                                                                  }
                                                                                }
                                                                                else
                                                                                  ddata.savegamew8 = con(0x1);
                                                                              }
                                                                              else
                                                                                SKW_2f3f_04ea(ddata.v1e0270, ddata.v1e0258, ddata.v1e0266, ddata.v1e0272, uwordrg1);
                                                                            }
                                                                            else
                                                                            {
                                                                              R_AB26();
                                                                              if (ddata.v1e0976 != con(0x0))
                                                                                SKW_REFRESH_PLAYER_STAT_DISP(ddata.v1e0976 - con(0x1));
                                                                            }
                                                                          }
                                                                          else if (UI16CAST(ddata.savegamew2) > UI16CAST(con(0x0)) && ddata.v1e0288 == con(0x0))
                                                                          {
                                                                            ddata.mouse_unk02--;
                                                                            SKW_GAME_SAVE_MENU();
                                                                            ddata.mouse_unk02++;
                                                                          }
                                                                        }
                                                                        else
                                                                          SKW_RESUME_FROM_WAKE();
                                                                      }
                                                                      else if (ddata.v1e0288 == con(0x0))
                                                                      {
                                                                        ddata.v1d6727 = !ddata.v1e0228;
                                                                        if (ddata.v1e0976 != con(0x0))
                                                                          SKW_24a5_1798(con(0x4));
                                                                        ddata.v1d6727 = true;
                                                                        SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
                                                                        SKW_38c8_0002();
                                                                        ddata.v1e0238 = true;
                                                                        if (ddata.v1e025c != con(0x0))
                                                                        {
                                                                          SKW_PERFORM_MOVE(ddata.v1e0256);
                                                                          ddata.v1e025c = con(0x0);
                                                                        }
                                                                        SKW_DRAW_WAKE_UP_TEXT();
                                                                        SKW_CHANGE_VIEWPORT_TO_INVENTORY(false);
                                                                        ddata.ticktrig = con(0x1);
                                                                        SKW_1031_0541(con(0x2));
                                                                        SKW_1031_098e();
                                                                      }
                                                                    }
                                                                  }
                                                                  else if (!ddata.v1e0248)
                                                                  {
                                                                    ddata.v1e0248 = true;
                                                                    SKW_38c8_0060();
                                                                    SKW_1031_06a5();
                                                                    SKW_1031_098e();
                                                                  }
                                                                }
                                                                else
                                                                {
                                                                  ddata.v1e0248 = false;
                                                                  SKW_38c8_0002();
                                                                  SKW_FILL_ENTIRE_PICT(ddata.bitmapptr, ddata.paletteptr1[con(0x0)]);
                                                                  tptrrg2 = SKW_QUERY_GDAT_TEXT(con(0x1), con(0x12), t_00, con(0x0));
                                                                  SKW_DRAW_VP_RC_STR(con(0x6), tptrrg2, unsignedword(ddata.paletteptr1[con(0x4)]));
                                                                  SKW_CHANGE_VIEWPORT_TO_INVENTORY(false);
                                                                  SKW_1031_0675(con(0x3));
                                                                  SKW_1031_098e();
                                                                  R_B37();
                                                                }
                                                              }
                                                              else
                                                              {
                                                                uwordrg1 -= con(0x56);
                                                                SKW_CLICK_MAGICAL_MAP_RUNE(uwordrg1);
                                                              }
                                                            }
                                                            else
                                                              SKW_CLICK_MAGICAL_MAP_AT(uwordrg1, wordrg4, wordrg3);
                                                          }
                                                          else
                                                            R_13262(con(0x2));
                                                        }
                                                        else
                                                          R_13262(con(0x3));
                                                      }
                                                      else
                                                        R_13262(con(0x1));
                                                    }
                                                    else
                                                    {
                                                      SKW_CLICK_VWPT(wordrg3, wordrg4);
                                                    }
                                                  }
                                                  else if (ddata.v1e0976 != con(0x0))
                                                    R_39796(&ddata.hero[ddata.v1e0976 - con(0x1)]);
                                                }
                                                else
                                                  SKW_CLICK_INVENTORY_EYE();
                                              }
                                              else
                                                SKW_PLAYER_CONSUME_OBJECT(con(0xffffffff), con(0xffffffff), con(0xffff));
                                            }
                                            else
                                            {
                                              x16 wordrg18 = SKW_GET_PLAYER_AT_POSITION((uwordrg1 - con(0x5f) + ddata.v1e0258) & con(0x3));
                                              if (wordrg18 >= con(0x0))
                                                SKW_SET_SPELLING_CHAMPION(wordrg18);
                                            }
                                          }
                                          else
                                          {
                                            uwordrg1 -= con(0x74);
                                            x16 wordrg41 = ((uwordrg1 & con(0x1)) != con(0x0)) ? 1 : 0;
                                            SKW_ACTIVATE_ACTION_HAND(uwordrg1 / 2, wordrg41);
                                          }
                                        }
                                        else
                                        {
                                          uwordrg1 -= con(0x71);
                                          ddata.v1e0488 = SKW_PROCEED_COMMAND_SLOT(uwordrg1) != con(0x0);
                                        }
                                      }
                                      else
                                        SKW_PROCEED_COMMAND_SLOT(con(0xffffffff));
                                    }
                                    else
                                    {
                                      uwordrg1 -= con(0x65);
                                      SKW_ADD_RUNE_TO_TAIL(uwordrg1);
                                    }
                                  }
                                  else
                                    SKW_REMOVE_RUNE_FROM_TAIL();
                                }
                                else
                                  ddata.v1e0488 = SKW_TRY_CAST_SPELL();
                              }
                              else
                              {
                                if (ddata.mouse_unk0e != con(0xffffffff))
                                  SKW_24a5_1798(ddata.mouse_unk0e);
                              }
                            }
                            else if ((ddata.v1e0288 == con(0x0)) || (uwordrg1 > con(0xb)))
                            {
                              x16 wordrg44 = uwordrg1 - con(0x7);
                              if (wordrg44 == con(0x4) || wordrg44 < ddata.savegamew2)
                              {
                                if (ddata.v1e0288 == con(0x0))
                                  SKW_24a5_1798(wordrg44);
                              }
                            }
                            else
                              SKW_2f3f_04ea(ddata.v1e0270, ddata.v1e0258, ddata.v1e0266, ddata.v1e0272, con(0x93));
                          }
                          else
                          {
                            SKW_SELECT_CHAMPION_LEADER(SKW_GET_PLAYER_AT_POSITION((uwordrg1 - con(0x10) + ddata.v1e0258) & con(0x3)));
                            SKW_2e62_0cfa(false);
                          }
                        }
                        else
                        {
                          x8 byterg4;
                          if (uwordrg1 != con(0x5d))
                            byterg4 = con(0x1);
                          else
                            byterg4 = con(0xffffffff);
                          byterg4 += ddata.hero[ddata.v1e0b74 - con(0x1)].b1c;
                          byterg4 &= con(0x3);
                          ddata.hero[ddata.v1e0b74 - con(0x1)].b1c = byterg4;
                          SKW_UPDATE_RIGHT_PANEL(false);
                        }
                      }
                      else
                      {
                        uwordrg1 -= con(0xe4);
                        SKW_CLICK_MONEYBOX(uwordrg1);
                      }
                    }
                    else
                    {
                      if (uwordrg1 != con(0x81))
                      {
                        uwordrg1 -= con(0x7d);
                        SKW_CHANGE_PLAYER_POS(uwordrg1);
                      }
                      else
                        SKW_CHAMPION_SQUAD_RECOMPUTE_POSITION();
                      R_107B0();
                      SKW_UPDATE_RIGHT_PANEL(false);
                    }
                  }
                  else
                  {
                    uwordrg1 -= con(0xea);
                    SKW_PUT_ITEM_TO_PLAYER(uwordrg1);
                  }
                }
                else
                {
                  uwordrg1 -= con(0x14);
                  SKW_CLICK_ITEM_SLOT(uwordrg1);
                }
              }
              else
                SKW_PERFORM_MOVE(uwordrg1);
            }
            else
              SKW_PERFORM_TURN_SQUAD(uwordrg1);
          }
          else
            SKW_1031_04F5();
        }
        else
          SKW_1031_050C();
        if (ddata.mouse_unk0a >= con(0xa5) && ddata.mouse_unk0a <= con(0xc6))
          ddata.v1e0488 = true;
        return true;
      }
    }
  }
  ddata.v1e0488 = true;
  return false;
}

// was SKW_1031_1e6a
void SKW_EVENT_LOOP(void)
{
  if (ddata.v1e0526 != con(0x0))
    ddata.v1e0526--;
  ddata.v1e0478 = false;
  do
  {
    SKW_IBMIO_USER_INPUT_CHECK();
    x16 wordrg1 = ddata.mouse_unk03;
    if (wordrg1 == ddata.mouse_unk04)
      return;
    x16 wordrg41 = wordrg1;
    wordrg1++;
    ddata.mouse_unk03 = wordrg1;
    if (ddata.mouse_unk03 > con(0x2))
    {
      ddata.mouse_unk03 = con(0x0);
    }
    if (!SKW_HANDLE_UI_EVENT(&table1e04e0[wordrg41 / 2]))
      ddata.mouse_unk03 = wordrg41;
    else
    {
      if (ddata.mouse_unk02 != con(0x0))
        ddata.mouse_unk02--;
    }
  } while (!ddata.v1e0488);
}

void SKW_121e_0003(ui16 eaxuw)
{
  x16 wordrg3 = con(0x1);
  x16 wordrg1 = ddata.v1e0270;
  x16 wordrg4 = ddata.v1e0272;
  if (eaxuw < con(0x2))
  {
    if (eaxuw == con(0x1))
    {
      wordrg1 += table1d27fc[(ddata.v1e0258 + con(0x3)) & con(0x3)];
      wordrg4 += table1d2804[(ddata.v1e0258 + con(0x3)) & con(0x3)];
    }
  }
  else if (eaxuw <= con(0x2))
  {
    wordrg1 += table1d27fc[(ddata.v1e0258 + con(0x1)) & con(0x3)];
    wordrg4 += table1d2804[(ddata.v1e0258 + con(0x1)) & con(0x3)];
    wordrg3 = con(0x3);
  }
  else if (eaxuw == con(0x3))
  {
    wordrg1 += table1d27fc[ddata.v1e0258];
    wordrg4 += table1d2804[ddata.v1e0258];
    wordrg3 = con(0x2);
  }
  if (wordrg1 >= con(0x0) && wordrg1 < ddata.savegamewa && wordrg4 >= con(0x0))
    if (wordrg4 < ddata.savegamewb)
      R_3C1E5(wordrg1, (ddata.v1e0258 + wordrg3) & con(0x3), con(0xffff), wordrg4, ddata.savegamewpc.w0);
}

// was SKW_121e_00be
void SKW_PLAYER_TESTING_WALL(x16 eaxw, x16 ebxw, x16 edxw)
{
  if (ddata.savegamew2 == con(0x0))
    return;
  if (eaxw == con(0x0))
    SKW_QUEUE_NOISE_GEN2(con(0x8), con(0x84), con(0xfe), ddata.v1d6c02, edxw, ebxw, eaxw, con(0x8c), con(0xc8));
  else
  {
    if (ddata.v1e0224)
      return;
    SKW_FIRE_MOUSE_SET_CAPTURE();
    ddata.v1e0224 = true;
    SKW_443c_08ab(&ddata.v1e00b6, &ddata.v1e00b2, &ddata.v1e00b4);
    if ((ddata.v1e00b2 & con(0x2)) != con(0x0))
      SKW_FIRE_HIDE_MOUSE_CURSOR();
    else
    {
      SKW_FIRE_MOUSE_RELEASE_CAPTURE();
      ddata.v1e0224 = false;
    }
  }
  ddata.v1e0488 = true;
}

void SKW_121e_013a(x16 eaxw, x16 ebxw, x16 edxw)
{
  x16 vw_04;
  x16 vw_08;

  vw_04 = eaxw;
  if (ddata.mouse_unk0e == con(0xffffffff))
    return;
  x16 wordrg6 = ddata.v1e0270;
  x16 wordrg3 = ddata.v1e0272;
  x16 wordrg41 = unsignedword(ddata.v1e02f0[ebxw].ba);
  if (wordrg41 != con(0x0))
  {
    if (wordrg41 != con(0x3))
      return;
    wordrg6 += table1d27fc[ddata.v1e0258];
    wordrg3 += table1d2804[ddata.v1e0258];
  }
  vw_08 = SKW_32cb_03a6(vw_04, wordrg41, wordrg6, edxw, wordrg3, ddata.v1e02f0[ebxw].w8, unsignedword(ddata.v1e02f0[ebxw].bb), false);
  if (vw_08 != con(0xffffffff))
  {
    SKW_MOVE_RECORD_TO(vw_08, wordrg3, con(0xffffffff), wordrg6, (ddata.v1e02f0[ebxw].bb != con(0x2)) ? con(0x0) : con(0xffff));
    ddata.v1e0ff6 = con(0x1);
    if (SKW_IS_CONTAINER_MONEYBOX(vw_08))
      SKW_GET_ADDRESS_OF_RECORD(vw_08)->u6.b.b1 &= con(0xfffffffb);
    SKW_TAKE_OBJECT(vw_08, true);
  }
  ddata.v1e0488 = true;
}

bool SKW_121e_0222(x16 eaxw, x16 ebxw, x16 edxw)
{
  c_aispec* aiptrrg7;
  c_aispec* aiptrrg4;
  c_record* recoptrrg3;
  x8 vb_08;

  recoptrrg3 = NULL;
  if (ddata.mouse_unk0e == con(0xffffffff))
    return false;
  x16 vw_04 = (ddata.v1e0258 + ebxw) & con(0x3);
  x16 wordrg15 = SKW_GET_CREATURE_AT(eaxw, edxw);
  if (wordrg15 != con(0xffffffff))
  {
    aiptrrg4 = SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(wordrg15);
    aiptrrg7 = aiptrrg4;
    recoptrrg3 = SKW_GET_ADDRESS_OF_RECORD(wordrg15);
    if ((aiptrrg4->u0.b.b0 & con(0x1)) != con(0x0))
      if (((ebxw >= con(0x4) && ebxw <= con(0x7)) && (aiptrrg4->u1e.b.b1 & con(0x8)) == con(0x0)) || (ebxw < con(0x4) && (aiptrrg7->u1e.b.b1 & con(0x20)) == con(0x0)))
        return false;
  }
  x16 vw_00 = SKW_REMOVE_OBJECT_FROM_HAND();
  x16 wordrg2;
  x16 wordrg19;
  if (ebxw < con(0x4) || ebxw > con(0x7))
  {
    wordrg19 = (vw_04 << con(0xe)) | (vw_00 & con(0xffff3fff));
    wordrg2 = con(0x0);
  }
  else
  {
    vb_08 = (CUTX8(ebxw) - con(0x4) + CUTX8(ddata.v1e0258)) & con(0x3);
    if (recoptrrg3 != NULL)
    {
      x16 wordrg21 = recoptrrg3->u2.w;
      while (wordrg21 != con(0xfffffffe))
      {
        if (urshift(wordrg21, con(0xe)) == unsignedword(vb_08))
        {
          if (SKW_IS_CONTAINER_MONEYBOX(wordrg21))
            SKW_GET_ADDRESS_OF_RECORD(wordrg21)->u6.b.b1 &= con(0xfffffffb);
        }
        wordrg21 = SKW_GET_NEXT_RECORD_LINK(wordrg21);
      }
    }
    wordrg19 = (unsignedword(vb_08) << con(0xe)) | (vw_00 & con(0xffff3fff));
    wordrg2 = con(0xffffffff);
  }
  SKW_MOVE_RECORD_TO(wordrg19, wordrg2, eaxw, con(0xffffffff), edxw);
  ddata.v1e0488 = true;
  return true;
}

void SKW_121e_0351(x16 eaxw, x16 edxw)
{
  x16 vw_00;

  bool boolrg3 = false;
  vw_00 = (ddata.v1e0530 != con(0x11)) ? con(0x2fd) : con(0x300);
  if (SKW_PT_IN_EXPANDED_RECT(vw_00, eaxw, edxw))
    boolrg3 = R_37BBB(con(0x0));
  else if (SKW_PT_IN_EXPANDED_RECT(vw_00 + 1, eaxw, edxw))
    boolrg3 = R_37BBB(con(0x1));
  if (boolrg3)
    ddata.v1e0488 = true;
}

x16 SKW_0aaf_0067(x8 eaxb)
{
  x16 wordrg3;
  x8 byterg5;
  t_text t_00[0x50];
  t_text* tptrrg1;
  x16 vw_54;
  x8 vb_5c; // inc8
  x8 vb_60;

  vb_60 = eaxb;
  x16 vw_58 = con(0x0);
  vw_54 = con(0xffffffff);
  t_text vub_50 = NULLBYTE;
  DM2_ZERO_MEMORY(DOWNCAST(t_text, t_00 + con(0x28)), con(0x28));
  vb_5c = con(0x0);
  byterg5 = vb_60;
  while (vb_5c, UI8CAST(vb_5c) < UI8CAST(con(0x14)))
  {
    tptrrg1 = SKW_QUERY_GDAT_TEXT(con(0x1a), vb_5c, t_00, byterg5);
    if (*tptrrg1 != NULLBYTE)
    {
      x8 wordrg4 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x1a), con(0xb), vb_5c, byterg5);
      tptrrg1 = t_00 + con(0x28) + 2 * signedlong(vw_58);
      *tptrrg1 = CUTX8(wordrg4);
      if (CUTX8(wordrg4) == con(0x0))
        *tptrrg1 = vb_5c;
      x8 byterg1 = CUTX8(wordrg4 >> con(0x8));
      tptrrg1[con(0x1)] = byterg1;
      if (byterg1 != con(0x0))
      {
        vw_54 = unsignedword(byterg1);
        vub_50 = *tptrrg1;
      }
      vw_58++;
    }
    vb_5c++;
  }
  if (vb_60 != con(0xffffff87))
    ddata.v1e0204 = vw_58;
  else
    ddata.v1e0204 = con(0x7);
  if (vw_54 == con(0xffffffff))
    if (vw_58 == con(0x1))
      vw_54 = con(0x1);
  SKW_1031_0675(con(0x4));
  x16 wordrg42 = con(0x0);
  while (ddata.mouse_visibility > con(0x0))
  {
    SKW_FIRE_SHOW_MOUSE_CURSOR();
    wordrg42++;
  }
  ddata.mouse_unk06 = con(0xff);
  do
  {
    SKW_EVENT_LOOP();
    SKW_WAIT_SCREEN_REFRESH();
    if (vw_54 != con(0xffffffff))
    {
      x16 wordrg15 = con(0x0); // called SKW_476d_04ed(vw_54) here, returns 0
      if (wordrg15 != con(0x0)) // can never be true then
        SKW_1031_0781(unsignedword(vub_50 + con(0xdb)));
    }
    if (ddata.mouse_unk06 == con(0xff) && SKW_476d_05a9() != con(0x0))
      if (SKW_476d_050e() == con(0x1c))
        SKW_1031_0781(con(0xdb));
  } while (ddata.mouse_unk06 == con(0xff));
  if (vb_60 != con(0xffffff87))
    wordrg3 = unsignedword(t_00[0x26 + 2 * ddata.mouse_unk06]);
  else
    wordrg3 = ddata.mouse_unk0a;
  for(;;)
  {
    wordrg42--;
    if (wordrg42 == con(0xffffffff))
    {
      ddata.backbuff2 = con(0x1);
      SKW_1031_06a5();
      return wordrg3;
    }
    SKW_FIRE_HIDE_MOUSE_CURSOR();
  }
}

void SKW_0aaf_01db(x16 eaxw, bool edxbool)
{
  t_gfxdata* gptrrg1;
  t_gfxdata* gptrrg5;
  c_rect rc_00;
  x16 vw_08;
  x16 vw_0c;

  if (!ddata.v1e0a88)
    SKW_FIRE_FILL_BACKBUFF_RECT(SKW_QUERY_EXPANDED_RECT(eaxw, &rc_00), ddata.paletteptr1[eaxw == con(0x0) ? con(0x1) : con(0x0)]);
  else
  {
    if (!edxbool)
      return;
    gptrrg5 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(ddata.v1e0206, ddata.v1e0208, ddata.v1e0207);
    s_gfxdatatail* ptrbeforerg5 = getprevious(gptrrg5);
    SKW_CALC_CENTERED_RECT_IN_RECT(&rc_00, ptrbeforerg5->width, ptrbeforerg5->height, &ddata.v1e0470);
    if (ddata.mouse_unk09 != con(0xffffffff))
    {
      SKW_QUERY_TOPLEFT_OF_RECT(ddata.mouse_unk09, &vw_08, &vw_0c);
      rc_00.x -= vw_08;
      rc_00.y -= vw_0c;
    }
    if (!dm2_dballochandler.gfxalloc_done)
    {
      gptrrg1 = &gptrrg5[SKW_CALC_IMAGE_BYTE_LENGTH(gptrrg5)];
    }
    else
      gptrrg1 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(ddata.v1e0206, ddata.v1e0208, ddata.v1e0207);
//                               srcbmp              palette
    SKW_DRAW_DIALOGUE_PARTS_PICT(gptrrg5, con(0x7), gptrrg1, &rc_00);
    SKW_FREE_PICT_ENTRY(gptrrg5);
  }
  SKW_SLEEP_SEVERAL_TIME(con(0x14));
}

void R_AB26(void)
{
  t_text* tptrrg2;
  t_text* tptrrg1;
  t_text t_00[0x80];
  x16 vw_80;
  t_text t_84[0x4]; // TODO probably 0x2 only

  if (!SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0x1a), con(0x1), con(0x0), con(0x87)))
    return;
  SKW_0aaf_02f8(con(0x87), con(0x0));
  tptrrg1 = SKW_QUERY_GDAT_TEXT(con(0x1a), con(0x28), t_00, con(0x87));
  SKW_DRAW_VP_RC_STR(con(0x15f), tptrrg1, unsignedword(ddata.paletteptr1[con(0xb)]));
  tptrrg1 = SKW_QUERY_GDAT_TEXT(con(0x1a), con(0x3c), t_00, con(0x87));
  SKW_DRAW_VP_RC_STR(con(0x170), tptrrg1, unsignedword(ddata.paletteptr1[con(0xb)]));
  t_84[0x1] = NULLBYTE;
  for(;;)
  {
    vw_80 = con(0x8);
    while (--vw_80 >= con(0x0))
    {
      t_84[0x0] = CUTX8(vw_80) + con(0x30);
      x16 wordrg31 = vw_80 + con(0x160);
      SKW_DRAW_STATIC_PIC(con(0x1a), ((vw_80 == ddata.v1d26a0) ? 1 : 0) + con(0x2), wordrg31, con(0x87), con(0xffffffff));
      if (vw_80 == con(0x0))
        tptrrg2 = SKW_QUERY_GDAT_TEXT(con(0x1a), con(0x29), t_00, con(0x87));
      else
        tptrrg2 = t_84;
      SKW_DRAW_VP_RC_STR(vw_80 + con(0x168), tptrrg2, unsignedword(ddata.paletteptr1[con(0xb)]));
    }
    vw_80 = con(0x8);
    while (--vw_80 >= con(0x0))
    {
      t_84[0x0] = CUTX8(vw_80) + con(0x30);
      x16 wordrg32 = vw_80 + con(0x171);
      SKW_DRAW_STATIC_PIC(con(0x1a), ((vw_80 == ddata.v1d26a2) ? 1 : 0) + con(0x2), wordrg32, con(0x87), con(0xffffffff));
      if (vw_80 == con(0x0))
        tptrrg2 = SKW_QUERY_GDAT_TEXT(con(0x1a), con(0x3d), t_00, con(0x87));
      else
        tptrrg2 = t_84;
      SKW_DRAW_VP_RC_STR(vw_80 + con(0x179), tptrrg2, unsignedword(ddata.paletteptr1[con(0xb)]));
    }
    SKW_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
    ui16 uwordrg1 = unsignedword(SKW_0aaf_0067(con(0x87)));
    ui16 uwordrg4 = uwordrg1;
    if (uwordrg1 == con(0xdb))
      return;
    x16 wordrg41;
    if (uwordrg1 < con(0x9d) || uwordrg1 > con(0xa4))
    {
      uwordrg1 = uwordrg4 - con(0x95);
      wordrg41 = con(0x0);
    }
    else
    {
      uwordrg1 -= con(0x9d);
      wordrg41 = con(0xa);
    }
    R_5BFB(uwordrg1, wordrg41);
  }
}

// old name: SKW_QUERY_PICT_BITS
t_gfxdata* SKW_QUERY_PICT_BITMAP(c_hex18* eaxph18)
{
  t_gfxdata* gptrrg1;

  if ((eaxph18->w_04 & con(0x4)) == con(0x0))
  {
    if ((eaxph18->w_04 & con(0x8)) == con(0x0))
      return eaxph18->pg_00;
    gptrrg1 = SKW_3e74_5817(eaxph18->w_0c);
  }
  else
    gptrrg1 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(eaxph18->b_08, eaxph18->b_0b, eaxph18->b_09);
  eaxph18->pg_00 = gptrrg1;
  return gptrrg1;
}

// was SKW_0b36_020e
void SKW_LOAD_GDAT_INTERFACE_00_02(void)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("SKW_LOAD_GDAT_INTERFACE_00_02");
LOGX(("SKW_LOAD_GDAT_INTERFACE_00_02\n" ));

  x8 vb_00;
  x8* bptrrg5;
  x8* bptrrg3;

  bptrrg5 = UPCAST(x8, DM2_ALLOC_FREEPOOL_MEMORY(SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(con(0x1), con(0x7), con(0x2), con(0x0)) & con(0xffff)));
  SKW_LOAD_GDAT_ENTRY_DATA_TO(con(0x1), con(0x7), con(0x2), con(0x0), bptrrg5);
  vb_00 = *bptrrg5;
  bptrrg3 = bptrrg5 + con(0x1);
  ddata.v1e020c = UPCAST(s_bpp, DM2_ALLOC_FREEPOOL_MEMORY(9 * unsignedlong(vb_00)));
  x16 wordrg12 = con(0x0);
  while (wordrg12 < unsignedword(vb_00))
  {
    bptrrg5 = bptrrg3;
    bptrrg3++;
    ddata.v1e020c[wordrg12].b0 = *bptrrg5;
    wordrg12++;
  }
  x16 wordrg11 = con(0x0);
  while (wordrg11 < unsignedword(vb_00))
  {
    ddata.v1e020c[wordrg11].pb1 = bptrrg3;
    bptrrg3 += unsignedlong(ddata.v1e020c[wordrg11].b0);
    wordrg11++;
  }
  x16 wordrg1 = con(0x0);
  for(;;)
  {
    if (wordrg1 >= unsignedword(vb_00))
    {
      ddata.v1e0210 = UPCAST(x16, bptrrg3); // TODO: HERE problem, left wordptr, right surely not
      return;
    }
    ddata.v1e020c[wordrg1].pb5 = bptrrg3;
    bptrrg3 += ddata.v1e020c[wordrg1].b0;
    wordrg1++;
  }
SPX_DEBUG_POP;
}

void SKW_CLICK_VWPT(x16 eaxw, x16 edxw)
{
  c_record* recoptrrg5;
  c_tim vtim_08;
  c_aispec* pai_18;
  x16 vw_3c;
  x16 vw_40;
  x16 vw_44;
  x16 vw_48;
  x16 vw_4c;
  x16 vw_50;

  x16 vw_04 = eaxw;
  x16 vw_00 = edxw;
  SKW_CHANGE_CURRENT_MAP_TO(ddata.v1e0266);
  if (ddata.v1e03a8)
  {
    ddata.v1e03a8 = false;
    SKW_PUSH_PULL_RIGID_BODY(ddata.v1d324a);
    SKW_29ee_000f();
    return;
  }
  vw_04 -= ddata.v1d26fc;
  vw_00 -= ddata.v1d26fe;
  x16 vw_38 = ddata.v1e0270;
  x16 vw_54 = ddata.v1e0272;
  vw_38 += table1d27fc[ddata.v1e0258];
  vw_54 += table1d2804[ddata.v1e0258];
  ddata.v1e0530 = unsignedword((SKW_GET_TILE_VALUE(vw_38, vw_54) & con(0xff)) >> con(0x5));
  vw_48 = ddata.savegamewpc.w0;
  x16 wordrg6 = con(0x0);
  while (wordrg6 < ddata.v1e0404)
  {
    x16 wordrg31 = wordrg6;
    if (SKW_PT_IN_RECT(&ddata.v1e02f0[wordrg6].r0, vw_04, vw_00))
    {
      ui8 ubyterg1 = ddata.v1e02f0[wordrg31].bb - con(0x1);
      if (ubyterg1 <= con(0x5))
      {
        switch (ubyterg1)
        {
          case 0:
          case 1:
          case 2:
            if (ddata.v1e02f0[wordrg6].w8 != con(0xffffffff))
            {
              if (vw_48 == con(0xffffffff))
              {
                SKW_121e_013a(vw_04, wordrg6, vw_00);
                return;
              }
              x8 byterg13 = ddata.v1e02f0[wordrg6].bb;
              if (byterg13 != con(0x3))
              {
                if (byterg13 == con(0x1))
                {
                  x16 wordrg52 = con(0x0);
                  while (wordrg52 <= con(0x1))
                  {
                    if (SKW_121e_03ae(vw_04, ddata.v1e0270, ddata.v1e0272, vw_00, con(0x0), wordrg52, con(0x1)))
                      return;
                    wordrg52++;
                  }
                }
              }
              else if (SKW_121e_03ae(vw_04, vw_38, vw_54, vw_00, con(0x0), con(0x2), unsignedword(byterg13)))
                return;
            }
          break;

          case 3:
            if (ddata.mouse_unk0e != con(0xffffffff))
            {
              if (ddata.v1e02f0[wordrg31].ba != con(0x3))
              {
                vw_44 = (ddata.v1e0270);
                vw_3c = ddata.v1e0272;
              }
              else
              {
                vw_44 = vw_38;
                vw_3c = vw_54;
              }
              x16 vw_2c = vw_44;
              recoptrrg5 = SKW_GET_ADDRESS_OF_TILE_RECORD(vw_44, vw_3c);
              if ((recoptrrg5->u2.b.b0 & con(0x40)) == con(0x0))
              {
                x16 wordrg17 = (ddata.v1e0258 + con(0x1)) & con(0x3);
                x16 wordrg3 = vw_44 + table1d27fc[wordrg17];
                vw_44 = wordrg3;
                vw_3c += table1d2804[wordrg17];
                R_3C1E5(wordrg3, (ddata.v1e0258 + con(0x3)) & con(0x3), con(0xffff), vw_3c, ddata.savegamewpc.w0);
              }
              else if (vw_48 == con(0xffffffff))
              {
                SKW_QUEUE_NOISE_GEN1(con(0x3), con(0x88), con(0x8c), con(0x0), con(0x80), ddata.v1e0270, ddata.v1e0272, con(0x1));
                SKW_INVOKE_MESSAGE(vw_44, con(0x0), con(0x2), vw_3c, ddata.longmallocx + con(0x1));
                vtim_08.un.l_00 = (ddata.longmallocx + 1) | (signedlong(ddata.v1d3248) << con(0x18));
                vtim_08.b_04 = con(0x58);
                vtim_08.b_05 = con(0x0);
                vtim_08.u.w_06 = SKW_GET_TILE_RECORD_LINK(vw_2c, vw_3c);
                SKW_QUEUE_TIMER(&vtim_08);
                recoptrrg5->u2.b.b1 |= con(0x8);
                ddata.v1e0488 = true;
              }
            }
          break;

          case 4:
          break;

          case 5:
            vw_4c = unsignedword(ddata.v1e02f0[wordrg31].ba);
            if (SKW_32cb_0287(unsignedword(ddata.v1e02f0[wordrg31].ba), vw_00, vw_04))
            {
              SKW_121e_0003(vw_4c); // TODO: vw_4c should be unsigned
              return;
            }
          break;

          default: throw(THROW_DMABORT);
        }
      }
    }
    wordrg6++;
  }
  x16 wordrg61 = unsignedword(SKW_GET_TILE_VALUE(vw_38, vw_54));
  bool boolrg1;
  if (urshift(wordrg61, con(0x5)) != con(0x6))
    boolrg1 = false;
  else
  {
    if ((wordrg61 & con(0x1)) == con(0x0))
      boolrg1 = false;
    else
    {
      wordrg61 &= con(0x4);
      if (wordrg61 != con(0x0))
        boolrg1 = false;
      else
        boolrg1 = true;
    }
  }
  bool vbool_24 = boolrg1;
  if (vw_48 != con(0xffffffff))
  {
    x16 wordrg62 = con(0x0);
    for(;;)
    {
      x16 vw_14 = wordrg62;
      if (wordrg62 > con(0x1))
      {
        if (ddata.v1e0530 == con(0x0))
          if (!vbool_24)
            return;
        wordrg62 = con(0x2);
        for(;;)
        {
          x16 vw_20 = wordrg62;
          if (wordrg62 > con(0x3))
          {
            vw_50 = SKW_GET_CREATURE_AT(vw_38, vw_54);
            if (vw_50 != con(0xffffffff))
            {
              pai_18 = SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(vw_50);
              if ((pai_18->u0.b.b0 & con(0x1)) != con(0x0))
              {
                wordrg62 = con(0x0);
                while (wordrg62 < (pai_18->u1e.w & con(0xf)))
                {
                  if (SKW_121e_03ae(vw_04, vw_38, vw_54, vw_00, con(0x3), wordrg62 + con(0x2), con(0x2)))
                    return;
                  if (SKW_PT_IN_EXPANDED_RECT(SKW_0cee_2e09(vw_50) + wordrg62, vw_04, vw_00))
                    if (SKW_121e_0222(vw_38, wordrg62 + con(0x6), vw_54))
                      return;
                  wordrg62++;
                }
              }
            }
            SKW_121e_0351(vw_04, vw_00);
            return;
          }
          if (SKW_121e_03ae(vw_04, vw_38, vw_54, vw_00, con(0x3), wordrg62, con(0x1)))
            return;
          if (SKW_PT_IN_EXPANDED_RECT(wordrg62 + con(0x2f8), vw_04, vw_00))
            if (SKW_121e_0222(vw_38, vw_20, vw_54))
              return;
          wordrg62++;
        }
      }
      if (SKW_121e_03ae(vw_04, ddata.v1e0270, ddata.v1e0272, vw_00, con(0x0), wordrg62, con(0x1)))
        return;
      if (SKW_PT_IN_EXPANDED_RECT(wordrg62 + con(0x2f8), vw_04, vw_00))
        if (SKW_121e_0222(ddata.v1e0270, vw_14, ddata.v1e0272))
          return;
      wordrg62++;
    }
  }
  if (ddata.v1e0530 == con(0x0))
  {
    if (SKW_PT_IN_EXPANDED_RECT(con(0x2fd), vw_04, vw_00))
    {
      SKW_PLAYER_TESTING_WALL(vbool_24 ? 1 : 0, vw_54, vw_38);
      return;
    }
    if (SKW_PT_IN_EXPANDED_RECT(con(0x2fe), vw_04, vw_00))
    {
      SKW_PLAYER_TESTING_WALL(vbool_24 ? 1 : 0, vw_54, vw_38);
      return;
    }
  }
  x16 wordrg51 = SKW_GET_CREATURE_AT(vw_38, vw_54);
  if (wordrg51 == con(0xffffffff))
    return;
  if ((SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(wordrg51)->u0.b.b0 & con(0x1)) == con(0x0))
    return;
  x16 wordrg63 = vw_54;
  if (!SKW_32cb_01b6(vw_04, vw_38, wordrg63, vw_00, &vw_40) == con(0x0))
    return;
  if (SKW_GET_CREATURE_WEIGHT(wordrg51) == con(0xff))
  {
    SKW_QUEUE_NOISE_GEN2(con(0xf), con(0x84), con(0xfe), SKW_QUERY_CLS2_FROM_RECORD(wordrg51), vw_38, wordrg63, con(0x0), con(0x8c), con(0x80));
    return;
  }
  if (vw_40 == con(0x0))
  {
    ddata.v1e03a8 = true;
    ddata.v1e03fa = vw_04;
    ddata.v1e03fc = vw_00;
    SKW_29ee_000f();
    ddata.v1d324a = con(0x6);
    return;
  }
  SKW_ROTATE_CREATURE(wordrg51, vw_40, false);
}

void SKW_THINK_CREATURE(x16 eaxw, x16 ebxw, x16 edxw)
{
  c_350* ptrrg7;
  c_aispec* aiptrrg3;
  bool doit = false; // added

  x16 wordrg1 = SKW_GET_CREATURE_AT(eaxw, edxw);
  if (wordrg1 == con(0xffffffff))
    return;
  ptrrg7 = SKW_13e4_0004(wordrg1, CUTX8(edxw), ebxw, CUTX8(eaxw));
  aiptrrg3 = ddata.s350.v1e0552;
  ddata.s350.v1e0556->w02 = con(0xffffffff);
  if (ddata.s350.v1e054e->u6.w == con(0x0))
  {
    ddata.s350.v1e0556->u.v.w14 = con(0x1);
    ddata.s350.v1e054e->u6.w = ddata.s350.v1e0556->u.v.w14;
  }
  x16 wordrg6 = ddata.s350.v1e0556->u.v.w14;
  ddata.s350.v1e0556->u.v.w14 = con(0x0);
  x16 wordrg5 = signedword(aiptrrg3->b3);
  if (wordrg5 != con(0x0))
  {
    if (wordrg5 < con(0x0))
      wordrg5 = -wordrg5;
    x16 vw_08 = unsignedword(CUTX8(ddata.longmallocx >> con(0x2)));
    if (unsignedword(ddata.s350.v1e0556->barr04[con(0x2)]) > vw_08)
      vw_08 += 0x100; // incbyte1
    vw_08 -= unsignedword(ddata.s350.v1e0556->barr04[con(0x2)]);
    x16 vw_04 = vw_08 / wordrg5;
    if (vw_04 > con(0x0))
    {
      if (aiptrrg3->b3 >= con(0x0))
      {
        if (UI16CAST(ddata.s350.v1e054e->u6.w) < UI16CAST(aiptrrg3->w4))
          ddata.s350.v1e054e->u6.w += vw_04;
      }
      else
        wordrg6 += vw_04;
      x16 wordrg41 = vw_08 % wordrg5;
      ddata.s350.v1e0556->barr04[con(0x2)] = CUTX8(CUTX16(UI32CAST(ddata.longmallocx) >> con(0x2)) - wordrg41);
    }
  }
  if (wordrg6 <= con(0x0))
    doit = true;
  else
  {
    if ((aiptrrg3->u0.b.b0 & con(0x1)) == con(0x0))
      ddata.s350.v1e054e->ua.b.b1 &= con(0x7f);
    if (!SKW_WOUND_CREATURE(wordrg6))
      doit = true;
  }
  if (doit)
  {
    if ((aiptrrg3->u0.b.b0 & con(0x1)) != con(0x0))
    {
      if ((ddata.s350.v1e055e[con(0x1)] & con(0x4000)) == con(0x0))
      {
        if ((ddata.s350.v1e055e[con(0x1)] & con(0x2000)) != con(0x0))
          SKW_13e4_0806();
      }
      else
        SKW_13e4_071b();
      if (ddata.s350.v1e0556->w02 == con(0xffffffff))
        SKW_1c9a_0fcb(unsignedword(ddata.s350.v1e054e->u4.b.b1));
    }
    else
      SKW_13e4_0982();
  }
  SKW_13e4_0329(ptrrg7);
}

x8 SKW_14cd_0457(void)
{
  c_sizex16* sx16ptrrg5;
  c_sizex16* sx16ptrrg1;

  x16 vw_00 = signedword(ddata.s350.v1e0674);
  if (vw_00 == con(0x0))
    return con(0x0);
  sx16ptrrg5 = ddata.s350.v1e0678;
  x16 wordrg6 = unsignedword(ddata.s350.v1e07d8.b0);
  while (vw_00 > con(0x0))
  {
    if (signedword(sx16ptrrg5->s_00.b_00) > con(0x0))
      sx16ptrrg5->s_00.b_00 = CUTX8(DM2_MIN(signedword(sx16ptrrg5->s_00.b_00) / 2, wordrg6 - con(0x2)));
    vw_00--;
    sx16ptrrg5++;
  }
  vw_00 = ddata.s350.v1e0674;
  sx16ptrrg5 = ddata.s350.v1e0678;
  for (x16 n = con(0x0); n < vw_00; n++, sx16ptrrg5++)
  {
    if (sx16ptrrg5->s_00.b_00 < con(0x0))
    {
      x16 wordrg3 = n + con(0x1);
      sx16ptrrg1 = sx16ptrrg5 + 1; // one struct ahead
      while (wordrg3 < vw_00)
      {
        if (sx16ptrrg1->s_00.b_00 >= con(0x0))
          break;
        wordrg3++;
        sx16ptrrg1++;
      }
      if (wordrg3 < vw_00)
      {
        SKW_COPY_MEMORY(DOWNCAST(c_sizex16, sx16ptrrg1), 2 * (11 * (signedlong(vw_00) - signedlong(wordrg3))), DOWNCAST(c_sizex16, sx16ptrrg5));
        wordrg3 -= n;
        vw_00 -= wordrg3;
      }
      else
        vw_00 = n + con(0x1);
    }
  }
  if (ddata.s350.v1e0678[vw_00 - con(0x1)].s_00.b_00 < con(0x0))
    vw_00--;
  ddata.s350.v1e0674 = CUTX8(vw_00);
  return CUTX8(vw_00);
}

s_hex20* SKW_14cd_10d2(s_hexe* eaxphe, x8 edxb)
{
  s_hexe* sheptrrg1;
  s_hexe* sheptrrg2;
  s_hexe* sheptrrg3;
  s_hex20* sptrrg5;
  s_4bytearray* s4baptrrg7;
  s_4bytearray* s4baptrrg6;
  s_4bytearray* s4baptrrg51;
  s_hexe* phe_04;
  s_hex20* ps_0c;
  x16 vw_18;
  x8 vb_24;

  phe_04 = eaxphe;
  x16 vw_10 = con(0x0);
  x16 vw_14 = con(0x0);
  x16 vw_1c = con(0x0);
  x16 vw_20 = con(0x0);
  if (ddata.s350.v1e058c != con(0x0))
  {
    DM2_ZERO_MEMORY(DOWNCAST(s_hex20, ddata.s350.v1e058e), con(0x4) * sizeof(s_hex20));
    ddata.s350.v1e058c = con(0x0);
  }
  ps_0c = &ddata.s350.v1e058e[0x0];
  sptrrg5 = ps_0c;
  vw_18 = con(0x0);
  sheptrrg3 = phe_04;
  while (vw_18 < con(0x4))
  {
    sheptrrg2 = sptrrg5->ps_00;
    if (sheptrrg2 != NULL)
    {
      if (sheptrrg2 == sheptrrg3)
        if (sptrrg5->sarr_04[con(0x0)].barr_00[con(0x4)] == edxb)
          return UPCAST(s_hex20, sptrrg5);
    }
    else
      ps_0c = sptrrg5;
    vw_18++;
    sptrrg5++;
  }
  ps_0c->ps_00 = phe_04;
  ps_0c->sarr_04[con(0x0)].barr_00[con(0x4)] = edxb;
  s4baptrrg7 = &ps_0c->sarr_04[con(0x1)];
  s4baptrrg6 = s4baptrrg7;
  s4baptrrg51 = s4baptrrg7;
  ps_0c->sarr_04[con(0x0)].barr_00[con(0x3)] = con(0x0);
  ps_0c->sarr_04[con(0x0)].barr_00[con(0x2)] = con(0x0);
  ps_0c->sarr_04[con(0x0)].barr_00[con(0x1)] = con(0x0);
  vw_18 = con(0x0);
  do
  {
    if (vw_18 > con(0x5))
      return UPCAST(s_hex20, ps_0c);
    if (phe_04->b_0c == edxb)
    {
      x16 wordrg4 = phe_04->w_04;
      if ((wordrg4 != con(0xffffffff)) && (phe_04->b_00 != con(0x8)))
      {
        if (phe_04->b_0a != con(0xffffffff))
        {
          while (s4baptrrg6 < s4baptrrg7)
          {
            s4baptrrg6->barr_00[con(0x1)] = CUTX8(vw_10) - CUTX8(vw_1c);
            s4baptrrg6++;
          }
          vw_10 = signedword(phe_04->b_0a);
          ps_0c->sarr_04[con(0x0)].barr_00[con(0x2)] += CUTX8(vw_10);
          vw_1c = con(0x0);
        }
        if (phe_04->b_0b != con(0xffffffff))
        {
          while (s4baptrrg51 < s4baptrrg7)
          {
            s4baptrrg51->barr_00[con(0x2)] = CUTX8(vw_14) - CUTX8(vw_20);
            s4baptrrg51++;
          }
          vw_14 = signedword(phe_04->b_0b);
          ps_0c->sarr_04[con(0x0)].barr_00[con(0x3)] += CUTX8(vw_14);
          vw_20 = con(0x0);
        }
        x16 wordrg2 = SKW_14cd_102e(wordrg4, con(0xff), false, ddata.s350.v1e054e->u2.w, true);
        s4baptrrg7->barr_00[con(0x0)] = CUTX8(wordrg2);
        ps_0c->sarr_04[con(0x0)].barr_00[con(0x1)] += CUTX8(wordrg2);
        x16 wordrg3 = wordrg2;
        vb_24 = CUTX8(DM2_MIN(wordrg3, vw_10 - vw_1c));
        ps_0c->sarr_04[con(0x0)].barr_00[con(0x2)] -= vb_24;
        ps_0c->sarr_04[con(0x0)].barr_00[con(0x3)] -= CUTX8(DM2_MIN(wordrg3, vw_14 - vw_20));
        vw_1c = DM2_MIN(vw_10, vw_1c + wordrg2);
        wordrg2 += vw_20;
        vw_20 = DM2_MIN(vw_14, wordrg2);
      }
      s4baptrrg7++;
      vw_18++;
    }
    sheptrrg1 = phe_04;
    phe_04 = sheptrrg1 + con(0x1);
  } while (sheptrrg1->b_0d != con(0x0));
  return UPCAST(s_hex20, ps_0c);
}

void SKW_14cd_1d6c(x8 eaxb, s_hexe* ebxphe, x8 ecxb, x8 edxb)
{
  s_hexe vhe_00;
  s_hexe* sheptrrg1;
  x8 vb_10;
  x8 vb_14;
  x8 vb_18;
  bool doit; // added

  vb_18 = eaxb;
  vb_10 = edxb;
  vb_14 = ecxb;
  if (ebxphe == NULL)
    return;
  do
  {
    doit = false;
    if (ebxphe->b_0c == vb_14)
    {
      if ((ebxphe->w_04 == con(0xffffffff)) || (ebxphe->w_06 != con(0x0) && ebxphe->w_06 != con(0x1)))
        doit = true;
      else
      {
        if (SKW_1c9a_0732(ebxphe->w_04, con(0xff), ddata.s350.v1e054e->u2.w) != con(0xfffffffe))
          doit = true;
      }
      if (doit)
      {
        if (SKW_14cd_1316(ebxphe->b_01, vb_10, ebxphe->u_02.w))
        {
          SKW_COPY_MEMORY(DOWNCAST(s_hexe, ebxphe), sizeof(s_hexe), DOWNCAST(s_hexe, &vhe_00));
          if (vb_18 != con(0x0))
          {
            vhe_00.b_08 = con(0x0);
            vhe_00.b_09 = con(0x0);
          }
          SKW_14cd_0f3c(ebxphe->b_00, &vhe_00, vb_14, ebxphe, con(0x0), con(0xffff), vb_10, vb_18);
        }
      }
    }
    sheptrrg1 = ebxphe;
    ebxphe++;
  } while (sheptrrg1->b_0d != con(0x0));
}

