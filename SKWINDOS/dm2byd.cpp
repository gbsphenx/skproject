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


bool SKW_2fcf_0434(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, bool argbool1)
{
  x16 wordrg5;
  x8 byterg5;
  x8* bptrrg1;
  c_hero* heroptrrg5;
  c_record* recoptrrg5;
  c_record* recoptrrg1;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_18;
  x16 vw_1c;
  x16 vw_20;
  x16 vw_24;
  x16 vw_28;
  x16 vw_2c;
  x16 vw_30;
  x16 vw_38;
  bool flag;

  x16 wordrg6 = eaxw;
  vw_20 = edxw;
  vw_38 = ebxw;
  vw_0c = ecxw;
  ddata.v1e1020 = con(0x0);
  bool vbool_00 = false;
  x16 vw_14 = con(0x0);
  vw_28 = ddata.v1d3248;
  vw_30 = ddata.v1d3248;
  bool vbool_04;
  bool vbool_08;
  if (wordrg6 != con(0xffffffff))
  {
    vbool_04 = SKW_IS_OBJECT_FLOATING(wordrg6);
    vw_2c = urshift(wordrg6 & con(0x3c00), con(0xa));
    vw_1c = con(0x3);
    if (vw_2c != con(0x4))
    {
      if (vw_2c == con(0xe))
        ddata.v1e1024 = ulrshift(ddata.savegametimarrp[SKW_GET_ADDRESS_OF_RECORD(wordrg6)->u6.w].v.w_08, con(0x4), con(0xe));
    }
    else
    {
      if (SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(wordrg6, con(0x1e)) == con(0x0))
        vw_1c = con(0x1);
      else
        vw_1c = con(0x2);
    }
    vbool_08 = false;
  }
  else
  {
    if (ddata.v1e03a8)
    {
      ddata.v1e03a8 = false;
      SKW_29ee_000f();
    }
    R_3B001(vw_0c, argw0);
    vw_1c = con(0x2);
    if (ddata.v1e0976 != con(0x0) || ddata.v1e0238)
      vbool_08 = false;
    else
      vbool_08 = true;
    vw_2c = con(0xffffffff);
    vbool_04 = false;
  }
  x16 vw_3c = con(0x32);
  // M_3AD30:
  for(;;)
  {
    flag = --vw_3c != con(0x0);
    if (flag)
    {
      bptrrg1 = ddata.savegamepp1[vw_0c];
      x16 wordrg4 = unsignedword(bptrrg1[argw0]);
      x8 byterg14 = ubrshift(bptrrg1[argw0], con(0x5));
      x16 vw_34_1 = unsignedword(byterg14);
      wordrg5 = unsignedword(byterg14);
      x16 wordrg199 = wordrg4 & con(0x8);
      if (wordrg5 != con(0x5))
      {
        if (vw_34_1 != con(0x2) || vbool_04 || wordrg199 == con(0x0) || (wordrg4 & con(0x1)) != con(0x0))
        {
          if (vw_34_1 != con(0x3) || wordrg6 == con(0xffffffff) || vw_2c == con(0xe) || vw_2c == con(0x4))
            break;
          wordrg4 &= con(0x4);
          if (wordrg4 == con(0x0))
          {
            vw_30 = SKW_LOCATE_OTHER_LEVEL(vw_30, &vw_0c, &argw0, con(0x1), NULL);
            SKW_CHANGE_CURRENT_MAP_TO(vw_30);
          }
          wordrg5 = SKW_0cee_06dc(vw_0c, argw0);
          vw_0c += table1d27fc[wordrg5];
          argw0 += table1d2804[wordrg5];
          wordrg5 += con(0x2);
          wordrg5 &= con(0x3);
          x16 wordrg21 = ((urshift(wordrg6, con(0xe)) - wordrg5 + con(0x1)) & con(0x2)) >> con(0x1);
          wordrg6 &= con(0xffff3fff);
          wordrg6 |= ((wordrg5 + wordrg21) & con(0x3)) << con(0xe);
          continue;
        }
        x16 wordrg31 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x8), con(0xb), con(0x6a), CUTX8(ulrshift(ddata.v1e03c0->we, con(0x8), con(0xc))));
        if (wordrg31 == con(0x0) || vw_2c != con(0x4))
        {
          if (vbool_08 && wordrg31 == con(0x0) && ddata.v1e102e == con(0x0))
          {
            vbool_08 = dm2_dballochandler.v1e13fe[0] != con(0x0);
            if (vw_14 != con(0x0))
            {
              SKW_LOAD_LOCALLEVEL_GRAPHICS_TABLE(vw_0c, vw_30, argw0);
              SKW_LOAD_LOCALLEVEL_DYN();
            }
            wordrg5 = ddata.v1e025c;
            ddata.v1e025c = con(0x0);
            SKW_DISPLAY_VIEWPORT(ddata.v1e0258, argw0, vw_0c);
            SKW_CHANGE_VIEWPORT_TO_INVENTORY(true);
            ddata.v1e025c = wordrg5;
          }
          if (wordrg31 == con(0x0))
          {
            vw_14++;
            vw_30 = SKW_LOCATE_OTHER_LEVEL(vw_30, &vw_0c, &argw0, con(0x1), NULL);
          }
          else
          {
            x16 wordrg3 = con(0x0);
            x16 wordrg43 = SKW_GET_TILE_RECORD_LINK(vw_0c, argw0);
            // M_3A98F: // co-label M_3A9FF
            for (;; wordrg43 = SKW_GET_NEXT_RECORD_LINK(wordrg43))
            {
              if (wordrg43 == con(0xfffffffe) == con(0x0))
                break;
              vw_2c = urshift(wordrg43 & con(0x3c00), con(0xa));
              if (vw_2c > con(0x3))
                break;
              if (vw_2c != con(0x2) || (recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg43), (recoptrrg1->u2.w & con(0x6)) != con(0x2)))
                continue;
              x16 wordrg115 = urshift(recoptrrg1->u2.w, con(0x3));
              if ((urshift(wordrg115, con(0x8)) & con(0x1f)) != con(0xc))
                continue;
              wordrg3 = unsignedword(CUTX8(wordrg115));
              break;
            }
            // M_3AA0B:
            vw_30 = R_3D93B(con(0x2), wordrg3, &vw_0c, SKW_RAND16(R_3D93B(con(0x3), wordrg3, &vw_0c, con(0x0), &argw0)) + con(0x1), &argw0);
          }
          // M_3AA6C:
          SKW_CHANGE_CURRENT_MAP_TO(vw_30);
          if (wordrg6 == con(0xffffffff))
          {
            R_3B001(vw_0c, argw0);
            if (UI16CAST(ddata.savegamew2) > UI16CAST(con(0x0)))
            {
              if (ddata.v1e102e == con(0x0))
              {
                heroptrrg5 = ddata.hero;
                for (x16 vw_34 = ddata.v1e102e; vw_34 < ddata.savegamew2; vw_34++)
                {
                  if (heroptrrg5->w36 != con(0x0))
                  {
                    SKW_WOUND_PLAYER(vw_34, con(0x30), con(0x2), (SKW_RAND02() + DM2_MIN(heroptrrg5->w38 >> con(0x2), con(0x11))) * vw_14);
                    SKW_QUEUE_NOISE_GEN2(con(0x16), con(0x87), con(0xfe), heroptrrg5->b101, ddata.v1e0270, ddata.v1e0272, con(0xffff), con(0xbe), con(0xff));
                  }
                  vbool_00 = true;
                  heroptrrg5++;
                }
              }
              else
              {
                if ((dm2_dballochandler.v1e13fe[0] == con(0x0)) || (ddata.v1e0976 != con(0x0)) || ddata.v1e0238)
                  vbool_08 = false;
                else
                  vbool_08 = true;
                heroptrrg5 = ddata.hero;
                for (x16 vw_34 = con(0x0); vw_34 < ddata.savegamew2; vw_34++)
                {
                  if (heroptrrg5->w36 != con(0x0))
                  {
                    x16 wordrg45 = 25 * SKW_GET_PLAYER_WEIGHT(vw_34);
                    x16 wordrg27 = SKW_MAX_LOAD(heroptrrg5);
                    SKW_ADJUST_STAMINA(vw_34, (wordrg45 / wordrg27) + con(0x1));
                  }
                  heroptrrg5++;
                }
              }
            }
            ddata.v1e102e = con(0x0);
            continue;
          }
          if (vw_2c != con(0x4) || !argbool1)
            continue;
          if (vw_20 >= con(0x0))
          {
            SKW_CHANGE_CURRENT_MAP_TO(vw_28);
            SKW_ATTACK_CREATURE(wordrg6, vw_38, con(0x0), vw_20, con(0x0), con(0x14));
          }
        }
        else
        {
          recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
          vw_0c = recoptrrg5->uc.w & con(0x1f);
          argw0 = ulrshift(recoptrrg5->uc.w, con(0x6), con(0xb));
          vw_30 = urshift(recoptrrg5->uc.w, con(0xa));
        }
        SKW_CHANGE_CURRENT_MAP_TO(vw_30);
        continue;
      }
      if (wordrg199 == con(0x0))
        break;
      recoptrrg5 = SKW_GET_ADDRESS_OF_TILE_RECORD(vw_0c, argw0);
      if ((recoptrrg5->u2.w & con(0x6000)) == con(0x2000) && vw_2c != con(0x4))
        break;
      if (vw_1c == con(0x3) || (vw_1c & urshift(2 * recoptrrg5->u2.w, con(0xe))) != con(0x0))
      {
        bool boolrg7;
        // M_3A5DC:
        if ((recoptrrg5->u2.w & con(0x1f)) != vw_0c)
          boolrg7 = false;
        else
        {
          if (ulrshift(recoptrrg5->u2.w, con(0x6), con(0xb)) != argw0 || urshift(recoptrrg5->u4.w, con(0x8)) != vw_30)
            boolrg7 = false;
          else
            boolrg7 = true;
        }
        vw_0c = recoptrrg5->u2.w & con(0x1f);
        argw0 = ulrshift(recoptrrg5->u2.w, con(0x6), con(0xb));
        ddata.v1e1020 = urshift(recoptrrg5->u2.w, con(0xf));
        vw_30 = urshift(recoptrrg5->u4.w, con(0x8));
        SKW_CHANGE_CURRENT_MAP_TO(vw_30);
        if (wordrg6 != con(0xffffffff))
        {
          if (vw_2c != con(0x4) || !argbool1)
          {
            if (vw_2c != con(0xe))
            {
              if (((recoptrrg5->u2.b.b1 & con(0x10)) == con(0x0)) && vw_20 != con(0xfffffffe))
              {
                x16 wordrg20 = wordrg6 >> con(0xe);
                x16 wordrg41 = ((ulrshift(recoptrrg5->u2.w, con(0x4), con(0xe)) + wordrg20) & con(0x3)) << con(0xe);
                wordrg6 = (wordrg6 & con(0x3fff)) | wordrg41;
              }
            }
            else
              wordrg6 = SKW_ROTATE_RECORD_BY_TELEPORTER(recoptrrg5, wordrg6);
          }
          else
          {
            if ((recoptrrg5->u2.b.b1 & con(0xffffff80)) != con(0x0))
            {
              if (ddata.v1d3248 != vw_28)
              {
                vw_18 = ddata.v1d3248;
                SKW_CHANGE_CURRENT_MAP_TO(vw_28);
                SKW_QUEUE_NOISE_GEN1(con(0x18), con(0x89), con(0x61), con(0x0), con(0x80), vw_20, vw_38, con(0x1));
                SKW_CHANGE_CURRENT_MAP_TO(vw_18);
              }
              SKW_QUEUE_NOISE_GEN1(con(0x18), con(0x89), con(0x61), con(0x0), con(0x80), vw_0c, argw0, con(0x1));
            }
            x16 wordrg21 = ulrshift(recoptrrg5->u2.w, con(0x4), con(0xe));
            SKW_ROTATE_CREATURE(wordrg6, wordrg21, ulrshift(recoptrrg5->u2.w, con(0x3), con(0xf)) != con(0x0));
          }
        }
        else
        {
          R_3B001(vw_0c, argw0);
          if ((recoptrrg5->u2.b.b1 & con(0xffffff80)) != con(0x0))
            SKW_QUEUE_NOISE_GEN1(con(0x18), con(0x89), con(0x61), con(0x0), con(0x80), ddata.v1e0270, ddata.v1e0272, con(0xffff));
          if (vw_28 != vw_30)
            vbool_08 &= dm2_dballochandler.v1e13fe[0] != con(0x0);
          x16 wordrg184;
          if ((recoptrrg5->u2.b.b1 & con(0x10)) == con(0x0))
            wordrg184 = ddata.v1e0258;
          else
            wordrg184 = con(0x0);
          wordrg184 += ulrshift(recoptrrg5->u2.w, con(0x4), con(0xe));
          wordrg184 &= con(0x3);
          SKW_ROTATE_SQUAD(wordrg184);
        }
        if (boolrg7)
          break;
        continue;
      }
    }
    break; // TODO: ugly loopform
  }
  // M_3AD40:
  ddata.v1e102a = vw_0c;
  ddata.v1e1028 = argw0;
  ddata.v1e102c = vw_30;
  ddata.v1e1026 = urshift(wordrg6, con(0xe));
  if (vw_14 != con(0x0) && argbool1)
    if (wordrg6 != con(0xffffffff))
    {
      byterg5 = SKW_QUERY_CLS2_FROM_RECORD(wordrg6);
      SKW_QUEUE_NOISE_GEN2(SKW_QUERY_CLS1_FROM_RECORD(wordrg6), con(0x85), con(0xfe), byterg5, vw_0c, argw0, con(0x1), con(0x3a), con(0x80));
    }
  if (vbool_00)
  {
    vw_18 = ddata.v1d3248;
    vw_10 = (ddata.v1e0266);
    vw_24 = (ddata.v1e0270);
    x16 wordrg61 = ddata.v1e0272;
    ddata.v1d3248 = con(0xffffffff);
    SKW_LOAD_LOCALLEVEL_GRAPHICS_TABLE(vw_0c, vw_30, argw0);
    heroptrrg5 = ddata.hero;
    for (x16 vw_34 = con(0x0); vw_34 < ddata.savegamew2; vw_34++)
    {
      if (heroptrrg5->w36 != con(0x0))
        SKW_QUEUE_NOISE_GEN2(con(0x16), con(0x82), con(0xfe), heroptrrg5->b101, vw_0c, argw0, con(0x2), con(0x69), con(0xc8));
      heroptrrg5++;
    }
    SKW_LOAD_LOCALLEVEL_GRAPHICS_TABLE(vw_24, vw_10, wordrg61);
    SKW_CHANGE_CURRENT_MAP_TO(vw_18);
  }
  // M_3AEAE:
  SKW_CHANGE_CURRENT_MAP_TO(vw_28);
  SKW_482b_05bf(true);
  return vbool_04;
}

// TODO init problem inside
bool SKW_19f0_2165(x8 eaxb, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2)
{
  x16 wordrg5;
  x16 vw_0c;
  x16 vw_10;
  x8 vb_14; // and8(0x14, con(0x7f));

  vb_14 = eaxb;
  wordrg5 = ecxw;
  x16 wordrg6 = argw0;
  vw_0c = unsignedword(eaxb & con(0xffffff80));
  if (vw_0c != con(0x0))
  {
    vb_14 &= con(0x7f);
    vw_10 = con(0xffffffff);
  }
  if (ddata.s350.v1e057c == con(0x0))
    goto M_false;
  bool boolrg7;
  if (wordrg5 != con(0xffffffff))
  {
    if (edxw != wordrg5 || ebxw != wordrg6)
      boolrg7 = false;
    else
      boolrg7 = true;
    if (!boolrg7 && argw1 == con(0xffffffff))
      argw1 = SKW_CALC_VECTOR_DIR(edxw, wordrg5, wordrg6, ebxw);
  }
  else
  {
    boolrg7 = false;
    wordrg5 = table1d27fc[argw1];
    wordrg5 += edxw;
    wordrg6 = table1d2804[argw1] + ebxw;
  }
  SKW_19f0_045a(wordrg5, wordrg6);
  if ((vb_14 == con(0x0)) && ((ddata.v1e08ae & con(0x10)) == con(0x0)))
    goto M_false;
  x16 vw_08; // TODO: always initialized?
  if (CUTX8(urshift(ddata.v1e08ae, con(0x5))) != con(0x0))
  {
    if (ddata.v1e08be == con(0xffffffff))
    {
      ddata.v1e08be = con(0x0);
      ddata.v1e08c0[0] = con(0xffffffff);
    }
    if (boolrg7 && (ddata.v1d3248 == ddata.v1e08d6) && wordrg5 == ddata.v1e08d8 && wordrg6 == ddata.v1e08d4)
      goto M_false;
    x16 wordrg163;
    if (!boolrg7)
      wordrg163 = con(0x2a);
    else
      wordrg163 = con(0x1);
    if ((wordrg163 & ddata.s350.v1e057c) == con(0x0))
      goto M_false;
    if (vb_14 != con(0x1))
    {
      wordrg5 = SKW_19f0_04bf();
      // M_1EE05: // co-label M_1EF0B
      for (;; wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg5))
      {
        if (wordrg5 == con(0xfffffffe))
          goto M_false;
        vw_08 = wordrg5 >> con(0xe);
        x16 wordrg63 = urshift(wordrg5 & con(0x3c00), con(0xa));
        if (wordrg63 < con(0x4) || wordrg63 >= con(0xe))
          continue;
        if (wordrg63 == con(0x4) || SKW_1c9a_0006(wordrg5, argw2) == con(0x0))
        {
          vw_08 = SKW_19f0_2024(wordrg5, argw1, argw2);
          if (vw_08 == con(0xffffffff))
            continue;
        }
        if (vw_08 < con(0x8))
        {
          if (ddata.v1e08c0[0] == con(0xffffffff))
          {
            x16 wordrg2;
            ui8 ubyterg4;
            if (!boolrg7)
            {
              ubyterg4 = con(0xc);
              wordrg2 = argw1;
            }
            else
            {
              if (argw1 != con(0xffffffff))
              {
                ubyterg4 = con(0x3);
                wordrg2 = argw1;
              }
              else
              {
                ubyterg4 = con(0xf);
                wordrg2 = con(0x0);
              }
            }
            x16 wordrg188 = con(0x0);
            while (wordrg188 < con(0x4))
            {
              ddata.v1e08c0[wordrg2] = ubyterg4 & con(0x1);
              wordrg2++;
              wordrg2 &= con(0x3);
              ubyterg4 >>= con(0x1);
              wordrg188++;
            }
          }
          if (ddata.v1e08c0[vw_08] == con(0x0))
          {
            ddata.v1e08be = con(0x1);
            continue;
          }
        }
        else
        {
          vw_10 = con(0x18);
          vw_08 &= con(0x3);
        }
        break;
      }
    }
    else if (!boolrg7)
      vw_10 = con(0x17);
  }
  else
  {
    if (ddata.v1e08be == con(0xffffffff))
    {
      ddata.v1e08be = con(0x0);
      ddata.v1e08bf = con(0x0);
      if (!boolrg7 && (ddata.s350.v1e057c & con(0x4)) != con(0x0))
      {
        ddata.v1e08c0[0] = (CUTX8(argw1) + con(0x2)) & con(0x3);
        if (ddata.v1e08b0 == con(0xffffffff))
          ddata.v1e08b0 = SKW_GET_TILE_RECORD_LINK(wordrg5, wordrg6);
        wordrg5 = ddata.v1e08b0;
        for(;;)
        {
          if (wordrg5 == con(0xfffffffe))
            break;
          if (urshift(wordrg5 & con(0x3c00), con(0xa)) > con(0x3))
            break;
          x32 longrg3 = unsignedlong(wordrg5);
          if (SKW_IS_WALL_ORNATE_ALCOVE_FROM_RECORD(ddata.v1e08b0, wordrg5))
          {
            longrg3 >>= con(0xe);
            if (longrg3 != signedlong(ddata.v1e08c0[0]))
              ddata.v1e08be = con(0x1);
            else
              ddata.v1e08bf = con(0x1);
          }
          wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg5);
        }
        ddata.v1e08b2 = wordrg5;
      }
    }
    if (ddata.v1e08bf == con(0x0))
      goto M_false;
    if (vb_14 != con(0x1))
    {
      vw_10 = con(0x1a);
      vw_08 = signedword(ddata.v1e08c0[0]);
      wordrg5 = ddata.v1e08b2;
      // M_1ED01: // co-label M_1ED6C
      for (;; wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg5))
      {
        if (wordrg5 == con(0xfffffffe))
          goto M_false;
        x16 wordrg3 = wordrg5;
        if (urshift(wordrg3, con(0xe)) != vw_08)
          continue;
        x16 wordrg181 = urshift(wordrg5 & con(0x3c00), con(0xa));
        if (wordrg181 == con(0x4) || wordrg181 >= con(0xe))
          continue;
        if (SKW_1c9a_0006(wordrg3, argw2) != con(0x0))
          break;
        if (SKW_19f0_2024(wordrg3, argw1, argw2) != con(0xffffffff))
          break;
      }
    }
    else
      vw_10 = con(0x19);
  }
  // M_1EF30:
  if (vw_0c == con(0x0))
    return true;
  if (argw1 == con(0xffffffff))
    argw1 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
  if (!boolrg7)
    if (SKW_19f0_0559(argw1))
      return true;
  ddata.s350.v1e0556->u.v.b1d = CUTX8(argw1);
  if (vb_14 != con(0x1))
  {
    if (vw_10 == con(0xffffffff))
    {
      x16 wordrg180 = (vw_08 - unsignedword(ddata.s350.v1e0556->u.v.b1d) + con(0x1)) & con(0x3);
      if (boolrg7)
      {
        bool skip = false;
        vw_10 = ((wordrg180 >= con(0x2)) ? 1 : 0) + con(0xc);
        x8 byterg42;
        if (wordrg180 != con(0x0))
        {
          if (wordrg180 != con(0x3))
            skip = true;
          else
            byterg42 = ddata.s350.v1e0556->u.v.b1d + con(0x1);
        }
        else
          byterg42 = ddata.s350.v1e0556->u.v.b1d - con(0x1);
        if (!skip)
        {
          byterg42 &= con(0x3);
          ddata.s350.v1e0556->u.v.b1d = byterg42;
        }
        if ((table1d607e[ddata.s350.v1e0584].u.b[1] & con(0x10)) != con(0x0))
        {
          x16 wordrg21 = unsignedword(ddata.s350.v1e0556->u.v.b1d);
          if (wordrg21 != ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe)))
          {
            SKW_19f0_0559(unsignedword(CUTX8(wordrg21)));
            return true;
          }
        } 
      }
      else
      {
        if (wordrg180 >= con(0x2))
          vw_10 = con(0x2c);
        else
          vw_10 = con(0x2b);
      }
    }
  }
  else if (vw_10 == con(0x19))
    vw_08 = argw1;
  else if (vw_10 != con(0x17))
  {
    x16 wordrg15 = SKW_RAND01();
    if (!boolrg7)
      wordrg15 += con(0x2);
    vw_08 = (argw1 + wordrg15) & con(0x3);
    vw_10 = table1d6299[wordrg15];
  }
  else
  {
    x16 wordrg22 = unsignedword(SKW_48ae_01af(SKW_0cee_2df4(SKW_19f0_050f()), argw1));
    if (wordrg22 == con(0x0))
      goto M_false;
    x16 wordrg41 = SKW_RAND16(SKW_1c9a_0598(wordrg22)) + con(0x1); // BUGFIX 2/2014
    x16 wordrg19 = SKW_COMPUTE_POWER_4_WITHIN(wordrg22, wordrg41);
    vw_08 = con(0x0);
    for(;;)
    {
      if ((wordrg19 & con(0x1)) != con(0x0))
        break;
      wordrg19 >>= con(0x1);
      vw_08++;
    }
  }
  ddata.s350.v1e0556->u.v.u18.b.b0 &= con(0xffffffe0);
  ddata.s350.v1e0556->u.v.u18.w |= ddata.v1e08a8 & con(0x1f);
  ddata.s350.v1e0556->u.v.u18.w = (ddata.s350.v1e0556->u.v.u18.w & con(0xfffffc1f)) | ((ddata.v1e08aa & con(0x1f)) << con(0x5));
  ddata.s350.v1e0556->u.v.u18.b.b1 &= con(0x3);
  ddata.s350.v1e0556->u.v.u18.w |= ((ddata.v1e08ac & con(0x3f)) << con(0xa));
  ddata.s350.v1e0556->u.v.b1c = CUTX8(vw_08);
  ddata.s350.v1e0556->u.v.b1e = CUTX8(argw2);
  ddata.s350.v1e0556->u.v.b20 = vb_14;
  ddata.s350.v1e0556->u.v.b1a = CUTX8(vw_10);
  ddata.s350.v1e056f = con(0xfffffffc);
  return true;

M_false: // M_1EF1A:
  if (vw_0c != con(0x0))
    ddata.s350.v1e056f = con(0xfffffffd);
  return false;
}

void SKW_14cd_09e2(void)
{
  x8 byterg4h;
  x16 wordrg5;
  const s_seven* s7ptrrg2;
  c_5bytes vs_00;
  x16 vw_08;
  c_record* prec_0c;
  x8 vb_18;
  x8 vb_1c;
  x8 vb_20;

	// SPX: moved variable before goto
  x8 byterg41;

  prec_0c = ddata.s350.v1e054e;
  SKW_SELECT_CREATURE_37FC();
  vw_08 = CUTX16(table1d607e[ddata.s350.v1e0584].u.l);
  if ((vw_08 & con(0x40)) == con(0x0))
    ddata.s350.v1e07ed = (((vw_08 & con(0x20)) == con(0x0)) ? 1 : 0) + con(0x4);
  else
    ddata.s350.v1e07ed = con(0x0);
  if (ddata.s350.v1e0588->p2 == getadr_u1d439d())
  {
    ddata.s350.v1e0556->u.v.b1a = con(0x0);
    return;
  }
  // M_16410:
  if (ddata.s350.v1e0588->p2 == getadr_u1d4396())
  {
    vb_1c = CUTX8(SKW_RAND()) & con(0x7);
    if (vb_1c > con(0x3))
    {
      ddata.s350.v1e0556->u.v.b1a = con(0x0);
      return;
    }
    if (vb_1c == con(0x0))
    {
      SKW_19f0_0559(CUTX16(ddata.longmallocx) & con(0x3));
      return;
    }
    ddata.s350.v1e0556->u.v.b1b = CUTX8(ulrshift(prec_0c->ue.w, con(0x6), con(0xe)));
    x16 wordrg45 = (unsignedword(ddata.s350.v1e0562.u.b.b_06) + table1d27fc[ulrshift(prec_0c->ue.w, con(0x6), con(0xe))]) & con(0x1f);
    ddata.s350.v1e0556->u.v.u18.b.b0 &= con(0xffffffe0);
    x16 wordrg3 = ddata.s350.v1e0556->u.v.u18.w | wordrg45;
    ddata.s350.v1e0556->u.v.u18.w = wordrg3;
    x16 wordrg44 = ((unsignedword(ddata.s350.v1e0562.u.b.b_07) + table1d2804[ulrshift(prec_0c->ue.w, con(0x6), con(0xe))]) & con(0x1f)) << con(0x5);
    x16 wordrg25 = (wordrg3 & con(0xfffffc1f)) | wordrg44;
    ddata.s350.v1e0556->u.v.u18.w = wordrg25;
    x8 byterg123 = (SKW_GET_TILE_VALUE(wordrg25 & con(0x1f), ulrshift(wordrg25, con(0x6), con(0xb))) & con(0xff)) >> con(0x5);
    if (byterg123 == con(0x0) || byterg123 == con(0x7))
    {
      ddata.s350.v1e0556->u.v.b1a = con(0x0);
      return;
    }
    if (byterg123 == con(0x5))
    {
      x16 wordrg24 = ulrshift(ddata.s350.v1e0556->u.v.u18.w, con(0x6), con(0xb));
      if (SKW_GET_TELEPORTER_DETAIL(&vs_00, wordrg24, ddata.s350.v1e0556->u.v.u18.w & con(0x1f)))
        if (!SKW_IS_CREATURE_ALLOWED_ON_LEVEL(ddata.s350.v1e054c, unsignedword(vs_00.b_04)))
        {
          ddata.s350.v1e0556->u.v.b1a = con(0x0);
          return;
        }
    }
    ddata.s350.v1e0556->u.v.b1a = con(0x2);
    return;
  }
  SKW_13e4_01a3();
  ddata.s350.v1e0898 = UPCAST(x8, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0x100)));
  x16 vw_10 = (SKW_RAND02() == con(0x0)) ? 1 : 0;
  if ((vw_08 & con(0x40)) == con(0x0))
  {
    x16 wordrg23 = unsignedword(ddata.s350.v1e0562.u.b.b_07);
    if (!SKW_CREATURE_GO_THERE(ddata.s350.v1e07ed, wordrg23, unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_06), wordrg23, con(0xffffffff)))
    {
      bool boolrg19;
      bool skip = false; // added
      if (ddata.s350.v1e07ed == con(0x5))
      {
        if (SKW_19f0_13aa(unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_07)))
        {
          boolrg19 = true;
          skip = true;
        }
      }
      if (!skip)
      {
        if (ddata.s350.v1e058d == con(0x0))
          boolrg19 = true;
        else
        {
          x16 wordrg22 = unsignedword(ddata.s350.v1e0562.u.b.b_07);
          if (!SKW_19f0_0891(con(0x0), wordrg22, con(0xffffffff), unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0xffffffff), ulrshift(prec_0c->ue.w, con(0x6), con(0xe))))
            boolrg19 = true;
          else
            boolrg19 = SKW_RAND16(((ddata.s350.v1e0552->u18.w >> con(0x8)) & con(0xf)) / 4 + con(0x1)) <= con(0x1);
        }
      }
      // M_16648:
      if (boolrg19)
      {
        prec_0c->ua.b.b1 |= con(0x20);
        // M_16657:
        for(;;)
        {
          if ((vw_08 & con(0x10)) != con(0x0))
            if (((prec_0c->ua.w & con(0x8)) != con(0x0) && SKW_RAND02() != con(0x0)) || ((prec_0c->ua.w & con(0x40)) != con(0x0) && SKW_RAND01() != con(0x0)) || SKW_RAND02() == con(0x0))
              // M_166A3:
              if (SKW_19f0_13aa(unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_07)))
              {
                ddata.s350.v1e0556->u.v.b12 = con(0xffffffff);
                ddata.s350.v1e0556->u.v.b13 = con(0x0);
                ddata.s350.v1e0556->u.v.b1a = con(0x55);
                goto M_exit;
              }
          // M_166CC:
          wordrg5 = con(0x4);
          if (SKW_1c9a_381c() <= con(0x0))
          {
            if (SKW_RAND01() == con(0x0))
              vb_20 = CUTX8(SKW_RAND02());
            else
              vb_20 = CUTX8((ulrshift(prec_0c->ue.w, con(0x6), con(0xe)) + con(0x2)) & con(0x3));
          }
          else
            vb_20 = ddata.s350.v1e0556->u.v.b1b;
          if (SKW_RAND01() == con(0x0))
            vb_1c = con(0xffffffff);
          else
            vb_1c = con(0x1);
          // M_16725:
          bool brk = false; // added
          for(;;)
          {
            wordrg5--;
            if (wordrg5 == con(0xffffffff))
            {
              if (++vw_10 != con(0x1) || SKW_RAND01() == con(0x0))
                brk = true; // break to M_1679C:
              break; // or without brk to goto M_16657:
            }
            x16 wordrg21 = unsignedword(ddata.s350.v1e0562.u.b.b_07);
            x8 wordrg41 = unsignedword(ddata.s350.v1e0562.u.b.b_06);
            if (SKW_CREATURE_GO_THERE(vw_10 == con(0x0) ? ddata.s350.v1e07ed | con(0xffffff80) : con(0xffffff80), wordrg21, con(0xffffffff), wordrg41, con(0xffffffff), unsignedword(vb_20)))
            {
              ddata.s350.v1e0556->u.v.b12 = con(0xffffffff);
              ddata.s350.v1e0556->u.v.b13 = con(0x0);
              goto M_exit;
            }
            vb_20 = (vb_20 + vb_1c) & con(0x3);
          }
          if (brk)
            break;
        }
      }
    }
  }
  // M_1679C:
  ddata.s350.v1e0556->u.v.b1a = con(0xffffffff);
   byterg41 = SKW_14cd_0684();
  if (byterg41 != con(0xfffffffd))
  {
    if (byterg41 < con(0x0) || byterg41 >= con(0x3f))
    {
      byterg41 = ddata.s350.v1e0556->u.v.b12;
      vb_1c = ddata.s350.v1e0556->u.v.b13;
    }
    else
    {
      ddata.s350.v1e0556->u.v.b12 = byterg41;
      vb_1c = con(0x0);
      ddata.s350.v1e0556->u.v.b13 = con(0x0);
    }
    s7ptrrg2 = table1d5f82[byterg41];
    byterg4h = s7ptrrg2[vb_1c].b0[con(0x5)] & con(0x1f);
    vb_18 = s7ptrrg2[vb_1c].b0[con(0x6)];
    wordrg5 = con(0x0);
    bool vbool_10_1 = false;
    // M_1682F:
    for(;;)
    {
      vb_1c = ddata.s350.v1e0556->u.v.b13;
      s7ptrrg2 = table1d5f82[ddata.s350.v1e0556->u.v.b12];
      if (vbool_10_1)
      {
        vb_20 = s7ptrrg2[vb_1c].b0[con(0x5)] & con(0x1f);
        bool boolrg1;
        if ((byterg4h == vb_20 && vb_20 != con(0x1)) || vb_20 == con(0x0))
          boolrg1 = false;
        else
        {
          if ((byterg4h != con(0x1)) || (vb_20 != byterg4h))
            boolrg1 = true;
          else
            boolrg1 = s7ptrrg2[vb_1c].b0[con(0x6)] != vb_18;
        }
        if (boolrg1)
        {
          ddata.s350.v1e0556->u.v.b1a = con(0x33);
          goto M_exit;
        }
      }
      x8 byterg4 = SKW_PROCEED_XACT(SKW_DECIDE_NEXT_XACT());
      if (byterg4 != con(0xfffffffc))
        vbool_10_1 |= SKW_14cd_08f5(byterg4);
      if (vbool_10_1)
        ddata.s350.v1e07d8.b1 = con(0x0);
      if (wordrg5++ > con(0x20))
        ddata.s350.v1e0556->u.v.b1a = con(0x0);
      if (ddata.s350.v1e0556->u.v.b1a == con(0xffffffff) && byterg4 != con(0xfffffffc))
        if (ddata.s350.v1e0556->u.v.b12 == con(0xffffffff))
          ddata.s350.v1e0556->u.v.b1a = con(0x0);
      if (ddata.s350.v1e0556->u.v.b1a != con(0xffffffff))
        goto M_exit;
    }
    // unreachable
  }
  if ((ddata.s350.v1e07ec & con(0xffffff80)) != con(0x0))
  {
    if (SKW_14cd_08f5(con(0xfffffffd)))
      ddata.s350.v1e07d8.b1 = con(0x0);
    ddata.s350.v1e0556->u.v.b1a = con(0x33);
  }
  else
    ddata.s350.v1e0556->u.v.b1a = con(0x0);
M_exit: // M_1690B:
  ddata.s350.v1e0898 = NULL;
  DM2_DEALLOC_LOBIGPOOL(con(0x100));
}

void SKW_LOAD_LOCALLEVEL_DYN(void)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("SKW_LOAD_LOCALLEVEL_DYN");
  x16 wordrg5;
  c_record* recoptrrg1;
  c_record* recoptrrg2;
  s_size10* s10ptrrg1;
  x8* bptrrg7;
  x8* bptrrg11;
  x8* bptrrg12;
  x8* bptrrg13;
  x8* bptrrg2;
  t_gfxdata* gptrrg5; // kind of a bytearray
  x8* bptrrg4;
  t_gfxdata* gptrrg41;
  x16* pw_00;
  x8* pb_04;
  x8* pb_0c;
  x8* pb_10;
  x16 vw_14;
  x16 vw_18;
  x16 vw_20;
  x16 vw_24;
  x16 vw_30;
  //x8 vb_34;
  ui16 vb_34;	// SPX: this holds the Creature index, and must be unsigned
  bool flag;

  ui16 vuw_1c;

  bool vbool_08 = false;
  ddata.v1e09a4 = con(0x0);
  ddata.v1e09a0 = UPCAST(x16, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0x960)));
  gptrrg5 = UPCAST(t_gfxdata, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0xfa), true));
  pb_0c = DM2_ALLOC_LOBIGPOOL_MEMORY(con(0xfa), true);
  pb_10 = DM2_ALLOC_LOBIGPOOL_MEMORY(con(0xfa), true);
  SKW_MARK_DYN_LOAD(con(0x01ff02ff)); // Mark: Interface - main screen, snd, all
  SKW_MARK_DYN_LOAD(con(0x18ff02ff)); // Mark: Teleporter, all, snd, all
  SKW_MARK_DYN_LOAD(con(0x07ff02ff)); // Mark: Interface - character sheet, all, snd, all
  if (dm2_dballochandler.v1e13fe[0] == con(0x0))
  {
    SKW_MARK_DYN_LOAD(con(0x0d0002ff)); // Mark: Missiles, fireball, snd, all
    ddata.v1e09a0[3 * (ddata.v1e09a4 - con(0x1))] = con(0x1);
    SKW_MARK_DYN_LOAD(con(0x0d2f02ff)); // Mark: Missiles, 0x2F(?), snd, all
    SKW_MARK_DYN_LOAD(con(0x0d7e02ff)); // Mark: Missiles, 0x7E(?), snd, all
    ddata.v1e09a0[3 * (ddata.v1e09a4 - con(0x1))] = con(0x1);
    SKW_MARK_DYN_LOAD(con(0x0d9f02ff)); // Mark: Missiles, 0x9F(?), snd, all
  }
  else
    SKW_MARK_DYN_LOAD(con(0xdff02ff)); // Mark: Missiles, all, snd, all
  SKW_MARK_DYN_LOAD(con(0x10ff02ff)); // Mark: Weapons, all, snd, all
  ddata.v1e09a0[3 * (ddata.v1e09a4 - con(0x1))] = con(0x1);
  SKW_MARK_DYN_LOAD(con(0x15ff02ff)); // Mark: Misc, all, snd, all
  SKW_MARK_DYN_LOAD(con(0x030002ff)); // Mark: Messages, all, snd, all
  SKW_MARK_DYN_LOAD(con(0x08fe02ff)); // Mark: Dungeon graphics, 0xFE, snd, all
  SKW_MARK_DYN_LOAD(con(0x16fe02ff)); // Mark: Champions, 0xFE, snd, all
  SKW_MARK_DYN_LOAD(con(0x09fe02ff)); // Mark: Wall ornates, 0xFE, snd, all
  SKW_MARK_DYN_LOAD(con(0x0afe02ff)); // Mark: Floor ornates, 0xFE, snd, all
  SKW_MARK_DYN_LOAD(con(0x0fff08fb)); // Mark PC-DOS: Creatures, all, raw08, anim attribution
  SKW_MARK_DYN_LOAD(con(0x0fff07fc)); // Mark PC-DOS: Creatures, all, raw07, anim info sequence
  SKW_MARK_DYN_LOAD(con(0x01ffffff)); // Mark: Interface - main screen, all, all, all
  SKW_MARK_DYN_LOAD(con(0x01000400)); // Mark: Interface - main screen, 0x00, rect, 0x00
  ddata.v1e09a0[3 * (ddata.v1e09a4 - con(0x1))] |= con(0x8000);
  SKW_MARK_DYN_LOAD(con(0x01000600)); // Mark: Interface - main screen, 0x00, raw06, 0x00
  ddata.v1e09a0[3 * (ddata.v1e09a4 - con(0x1))] |= con(0x8000);
  SKW_MARK_DYN_LOAD(con(0x0100070a)); // Mark: Interface - main screen, 0x00, raw07, 0x0A
  ddata.v1e09a0[3 * (ddata.v1e09a4 - con(0x1))] |= con(0x8000);
  //SKW_MARK_DYN_LOAD(0x1cffffff); // Mark: Japanese fonts, all, all, all	// Used in PC-9821 version, not PC-DOS
  SKW_MARK_DYN_LOAD(con(0x1a80ffff)); // Mark: Dialog boxes, Load/cancel, all, all
  SKW_MARK_DYN_LOAD(con(0x1a81ffff)); // Mark: Dialog boxes, Save/cancel, all, all
  SKW_MARK_DYN_LOAD(con(0x0300ffff)); // Mark: Messages, 0x00, all, all
  SKW_MARK_DYN_LOAD(con(0x0700ffff)); // Mark: Interface - character sheet, 0x00, all, all
  if (dm2_dballochandler.v1e13fe[0] == con(0x0))
  {
    SKW_MARK_DYN_LOAD(con(0x0d00ffff)); // Mark: Missiles, fireball, all, all
    ddata.v1e09a0[3 * (ddata.v1e09a4 - con(0x1))] = con(0x1);
    SKW_MARK_DYN_LOAD(con(0x0d2fffff)); // Mark: Missiles, 0x2F(?), all, all
    SKW_MARK_DYN_LOAD(con(0x0d7effff)); // Mark: Missiles, 0x7E(?), all, all
    ddata.v1e09a0[3 * (ddata.v1e09a4 - con(0x1))] = con(0x1);
    SKW_MARK_DYN_LOAD(con(0x0d9fffff)); // Mark: Missiles, 0x9F(?), all, all
  }
  else
    SKW_MARK_DYN_LOAD(con(0x0dffffff)); // Mark: Missiles, all, all, all
  SKW_MARK_DYN_LOAD(con(0x10ffffff)); // Mark: Weapons, all, all, all
  ddata.v1e09a0[3 * (ddata.v1e09a4 - con(0x1))] = con(0x1);
  SKW_MARK_DYN_LOAD(con(0x15ffffff)); // Mark: Misc, all, all, all
  SKW_MARK_DYN_LOAD(con(0xffff01f9)); // Mark: All, all, image, 0xF9(Map chip)
  SKW_MARK_DYN_LOAD_2676_008f(con(0xfff0510), con(0x39)); // Mark: Creature, all, text, 0x10(My enemies) to 0x39(?)
  ddata.v1e09a0[3 * (ddata.v1e09a4 - con(0x2))] &= con(0x7fff);
  SKW_MARK_DYN_LOAD(((signedlong(ddata.v1d3248) + 1) << con(0x10)) + con(0x30002ff));
  pb_04 = *ddata.savegamepp1;
  pw_00 = &ddata.v1e038c[unsignedlong(*ddata.v1e03f4)];
  x16 vw_28 = con(0x0);
  // M_2B1B4:
  x16 vw_2c;
  for(;;)
  {
    if (vw_28 < ddata.savegamewa)
      vw_2c = con(0x0);
    else // TODO: note, in that case, vw_2c is uninitialized - possible?
    {
      SKW_MARK_DYN_LOAD((unsignedlong(table1410ec[ddata.v1d3248]) << con(0x10)) + con(0x4000300));
      if (dm2_dballochandler.v1e13fe[0] == con(0x0))
      {
        for (vuw_1c = con(0x0); vuw_1c < ddata.savegamew2; vuw_1c++)
        {
          x16 wordrg48 = vuw_1c;
          SKW_MARK_DYN_LOAD((unsignedlong(ddata.hero[wordrg48].b101) << con(0x10)) + con(0x16000100));
          SKW_MARK_DYN_LOAD((unsignedlong(ddata.hero[wordrg48].b101) << con(0x10)) + con(0x160002ff));
          SKW_MARK_DYN_LOAD_2676_008f((unsignedlong(ddata.hero[wordrg48].b101) << con(0x10)) + con(0x16000508), con(0xb));
          ddata.v1e09a0[3 * (signedlong(ddata.v1e09a4) - con(0x2))] &= con(0x7fff);
        }
      }
      if ((dm2_dballochandler.v1e13fe[0] != con(0x0)) || (dm2_dballochandler.v1e0a84 && ddata.savegamew2 != con(0x4)))
        SKW_MARK_DYN_LOAD(con(0x16ffffff));	// Mark: Champions, all, all, all
      if ((dm2_dballochandler.v1e13fe[0] != con(0x0)) || ((ddata.v1e03c0->u0.u.b.b2 & con(0x20)) != con(0x0)))
        SKW_MARK_DYN_LOAD(con(0x18ffffff)); // Mark: Teleporter, all, all, all
      if ((ddata.v1e03c0->u0.u.b.b2 & con(0xffffff80)) == con(0x0))
        ddata.v1e040e[0] = con(0xffffffff);
      else
        ddata.v1e040e[0] = CUTX8(ulrshift(ddata.v1e03c0->we, con(0x4), con(0xc)));
      if ((ddata.v1e03c0->u0.u.b.b3 & con(0x1)) == con(0x0))
        ddata.v1e040e[1] = con(0xffffffff);
      else
        ddata.v1e040e[1] = CUTX8(urshift(ddata.v1e03c0->we, con(0xc)));
      bool skip = false; // added
      x32 longrg111;
      bool boolrg6;
      if (dm2_dballochandler.v1e13fe[0] == con(0x0))
      {
        boolrg6 = false;
        bool boolrg7 = false;
        for (x16 vw_1c = con(0x0); vw_1c < con(0x2); vw_1c++)
        {
          vb_34 = ddata.v1e040e[vw_1c];
          if (vb_34 != con(0xffffffff))
          {
            boolrg7 = false;
            if (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xe), con(0xb), con(0x40), vb_34) == con(0x0))
              boolrg6 = boolrg7;
            SKW_MARK_DYN_LOAD((unsignedlong(vb_34) << con(0x10)) + con(0xe00ffff));
          }
        }
        if (!boolrg7)
          skip = true;
        else
          longrg111 = con(0x0efe02ff); // Mark: Doors, 0xFE, snd, all
      }
      else
      {
        boolrg6 = true;
        longrg111 = con(0x0effffff); // Mark: Doors, all, all, all
      }
      if (!skip)
        SKW_MARK_DYN_LOAD(longrg111);
      // M_2B395:
      vb_34 = CUTX8(ulrshift(ddata.v1e03c0->we, con(0x8), con(0xc)));
      ddata.v1d6c02 = vb_34; // was wordtransfer, but hibyte never read
      ddata.v1d6bfc = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x8), con(0xb), con(0x69), vb_34);
      x16 wordrg168 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x8), con(0xb), con(0x6b), CUTX8(ulrshift(ddata.v1e03c0->we, con(0x8), con(0xc))));
      if (wordrg168 != con(0x0))
      {
        bptrrg12 = &pb_10[CUTX8(wordrg168)];
        *bptrrg12 = con(0x1);
      }
      if (dm2_dballochandler.v1e13fe[0] != con(0x0))
        vb_34 = con(0xffffffff);
      x32 longrg23 = unsignedlong(vb_34) << con(0x10);
      SKW_MARK_DYN_LOAD(longrg23 + con(0x800ffff));
      SKW_MARK_DYN_LOAD(longrg23 + con(0x1700ffff));
      if (dm2_dballochandler.v1e13fe[0] == con(0x0))
      {
        if (!boolrg6)
        {
          SKW_MARK_DYN_LOAD_2676_008f(longrg23 + con(0x8000106), con(0x1a));
          SKW_MARK_DYN_LOAD_2676_008f(longrg23 + con(0x80001d3), con(0xd8));
        }
        if ((ddata.v1e03c0->u0.u.b.b2 & con(0x1)) == con(0x0))
          SKW_MARK_DYN_LOAD_2676_008f((unsignedlong(vb_34) << con(0x10)) + con(0x800016b), con(0x79));
        if ((ddata.v1e03c0->u0.u.b.b2 & con(0x2)) == con(0x0))
          SKW_MARK_DYN_LOAD_2676_008f((unsignedlong(vb_34) << con(0x10)) + con(0x8000199), con(0xa0));
        if ((ddata.v1e03c0->u0.u.b.b2 & con(0x4)) == con(0x0))
          SKW_MARK_DYN_LOAD_2676_008f((unsignedlong(vb_34) << con(0x10)) + con(0x8000182), con(0x89));
        if ((ddata.v1e03c0->u0.u.b.b2 & con(0x8)) == con(0x0))
          SKW_MARK_DYN_LOAD_2676_008f((unsignedlong(vb_34) << con(0x10)) + con(0x80001c7), con(0xcc));
        if ((ddata.v1e03c0->u0.u.b.b2 & con(0x10)) == con(0x0))
          SKW_MARK_DYN_LOAD_2676_008f((unsignedlong(vb_34) << con(0x10)) + con(0x80001cd), con(0xd2));
        if ((ddata.v1e03c0->u0.u.b.b2 & con(0x40)) == con(0x0))
          if (!vbool_08)
          {
            x32 longrg44 = unsignedlong(vb_34) << con(0x10);
            SKW_MARK_DYN_LOAD_2676_006a(longrg44 + con(0x8000103));
            SKW_MARK_DYN_LOAD_2676_008f(longrg44 + con(0x80001fc), con(0xfe));
          }
      }
      //ddata.v1e1472 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x8), con(0xb), con(0x66), ddata.v1d6c02);
	  ddata.v1e1472 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, dtWordValue, GDAT_GFXSET_SCENE_RAIN, ddata.v1d6c02);
      if (dm2_dballochandler.v1e13fe[0] == con(0x0))
        if (table1d6b76[4 * ddata.v1e1472 + 0x70] != con(0x0))
          SKW_MARK_DYN_LOAD(con(0xd30ffff));
      bptrrg4 = pb_0c;
      for ( vuw_1c = con(0x0); UI16CAST(ddata.v1e03c0->wa & con(0xf)) > vuw_1c; vuw_1c++)
      {
        bptrrg13 = bptrrg4 + unsignedlong(ddata.v1e02cc[vuw_1c]);
        *bptrrg13 = con(0x1);
      }
      if (dm2_dballochandler.v1e13fe[0] == con(0x0))
      {
        for (x16 vw_1c = con(0x0); vw_1c < con(0xfa); vw_1c++)
        {
          if (pb_0c[vw_1c] != con(0x0))
          {
            SKW_MARK_DYN_LOAD((unsignedlong(vw_1c) << con(0x10)) + con(0x900ffff));
            SKW_MARK_DYN_LOAD_2676_00d0(con(0x9), con(0x40), CUTX8(vw_1c));
          }
        }
      }
      else
        SKW_MARK_DYN_LOAD(con(0x09ffffff)); // Mark: Wall ornates, all, all, all
      for ( vuw_1c = con(0x0); UI16CAST(ulrshift(ddata.v1e03c0->wa, con(0x4), con(0xc))) > vuw_1c; vuw_1c++)
        pb_10[ddata.v1e02dc[vuw_1c]] = con(0x1);
      if (dm2_dballochandler.v1e13fe[0] == con(0x0))
      {
        bptrrg7 = pb_10;
        for (x16 vw_1c = con(0x0); vw_1c < con(0xfa); vw_1c++)
        {
          if (bptrrg7[vw_1c] != con(0x0))
            SKW_MARK_DYN_LOAD((unsignedlong(vw_1c) << con(0x10)) + con(0xa00ffff));
          SKW_MARK_DYN_LOAD_2676_00d0(con(0xa), con(0x40), CUTX8(vw_1c));
        }
      }
      else
        SKW_MARK_DYN_LOAD(con(0x0affffff)); // Mark: Floor ornates, all, all, all
      if (dm2_dballochandler.v1e13fe[0] == con(0x0))
      {
        for ( vuw_1c = con(0x0); UI16CAST(ddata.v1e03c0->wc & con(0xf)) > vuw_1c; vuw_1c++)
          SKW_MARK_DYN_LOAD((unsignedlong(ddata.v1e0414[vuw_1c]) << con(0x10)) + con(0xb00ffff));
      }
      else
        SKW_MARK_DYN_LOAD(con(0x0bffffff)); // Mark: Door ornates, all, all, all
      if ((dm2_dballochandler.v1e13fe[0] != con(0x0)) || boolrg6)
        SKW_MARK_DYN_LOAD(con(0x0cffffff));
      for ( vuw_1c = con(0x0); ulrshift(ddata.v1e03c0->wc, con(0x8), con(0xc)) > vuw_1c; vuw_1c++)
        gptrrg5[ddata.v1e03c4[vuw_1c]] = con(0x1);
      if (dm2_dballochandler.v1e13fe[0] != con(0x0))
        SKW_MARK_DYN_LOAD(con(0x0fffffff)); // Mark: Creatures, all, all, all
      vb_34 = con(0x0);
      while (UI8CAST(vb_34) < UI8CAST(con(0xfffffffa)))
      {
        if (SKW_QUERY_GDAT_CREATURE_WORD_VALUE(vb_34, con(0x6)) != con(0x0))
          gptrrg5[vb_34] = con(0x1);
        gptrrg41 = gptrrg5 + unsignedlong(vb_34);
        if ((*gptrrg41 != con(0x0)) && (dm2_dballochandler.v1e13fe[0] == con(0x0)))
        {
          x32 longrg21 = unsignedlong(vb_34) << con(0x10);
          SKW_MARK_DYN_LOAD(longrg21 + con(0xf0007fd));
          SKW_MARK_DYN_LOAD(longrg21 + con(0xf0007fe));
          if ((*gptrrg41 & con(0x1)) == con(0x0) && dm2_dballochandler.v1e13fe[1] == con(0x0))
          {
            SKW_MARK_DYN_LOAD_2676_008f(longrg21 + con(0xf00fffa), con(0xfd));
            ddata.v1e09a0[3 * (signedlong(ddata.v1e09a4) - con(0x2))] &= con(0x7fff);
          }
          else
          {
            x32 longrg45 = unsignedlong(vb_34) << con(0x10);
            SKW_MARK_DYN_LOAD(longrg45 + con(0xf00ffff));
            SKW_MARK_DYN_LOAD_2676_008f(longrg45 + con(0xf0001fa), con(0xfd));
            SKW_MARK_DYN_LOAD_2676_00d0(con(0xf), con(0x40), vb_34);
          }
        }
        vb_34++;
      }
      if (!dm2_dballochandler.v1e0a84)
        SKW_482b_060e();
      SKW_LOAD_DYN4(UPCAST(s_hex6, DOWNCAST(x16, ddata.v1e09a0)), ddata.v1e09a4);
      if (!dm2_dballochandler.v1e0a84)
        SKW_LOAD_MISCITEM();
      // TODO: some bools here
//      ddata.v1e1481 = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0x17), con(0x1), con(0x64), ddata.v1d6c02) ? 1 : 0;
//      ddata.v1e1479 = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0x17), con(0x1), con(0x6a), ddata.v1d6c02) ? 1 : 0;
//      ddata.v1e147a = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0x17), con(0x1), con(0x67), ddata.v1d6c02) ? 1 : 0;
//      ddata.v1e147c = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0x17), con(0x1), con(0x71), ddata.v1d6c02) ? 1 : 0;
      ddata.v1e1481 = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_ENVIRONMENT, dtImage, GDAT_ENVWTH_THUNDER_1, ddata.v1d6c02) ? 1 : 0;	// 0x64
      ddata.v1e1479 = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_ENVIRONMENT, dtImage, GDAT_ENVWTH_WETGROUND_1, ddata.v1d6c02) ? 1 : 0;	// 0x6A
      ddata.v1e147a = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_ENVIRONMENT, dtImage, GDAT_ENVWTH_CLOUDSKY_1, ddata.v1d6c02) ? 1 : 0;	// 0x67
      ddata.v1e147c = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(GDAT_CATEGORY_ENVIRONMENT, dtImage, GDAT_ENVWTH_RAINFALL_STRAIGHT_1, ddata.v1d6c02) ? 1 : 0;	// 0x71
//      ddata.v1d6c00 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x8), con(0xb), con(0x64), ddata.v1d6c02);
//      ddata.v1e12d6 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x8), con(0xb), con(0x65), ddata.v1d6c02);
//      ddata.v1e03f8 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x8), con(0xb), con(0x6d), ddata.v1d6c02);
//      ddata.v1e0400 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x8), con(0xb), con(0x72), ddata.v1d6c02);
      ddata.v1d6c00 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, dtWordValue, GDAT_GFXSET_SCENE_COLORKEY, ddata.v1d6c02);	// 0x64
      ddata.v1e12d6 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, dtWordValue, GDAT_GFXSET_SCENE_FLAGS, ddata.v1d6c02);	// 0x65
      ddata.v1e03f8 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, dtWordValue, GDAT_GFXSET_AMBIANT_DARKNESS, ddata.v1d6c02);	// 0x6D
      ddata.v1e0400 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_GRAPHICSSET, dtWordValue, GDAT_GFXSET_x72, ddata.v1d6c02);	// 0x72
      if (ddata.v1e0400 == con(0x0))
        ddata.v1e0400 = con(0x1);
      DM2_DEALLOC_LOBIGPOOL(con(0xfa));
      DM2_DEALLOC_LOBIGPOOL(con(0xfa));
      DM2_DEALLOC_LOBIGPOOL(con(0xfa));
      DM2_DEALLOC_LOBIGPOOL(con(0x960));
      bool boolrg2 = dm2_dballochandler.v1e0a84 ? 1 : 0;
      if (!boolrg2)
      {
        SKW_QUERY_GDAT_IMAGE_METRICS(con(0x8), boolrg2 ? 1 : 0, &vw_14, ddata.v1d6c02, &vw_30);
        wordrg5 = vw_14 * vw_30;
        SKW_QUERY_GDAT_IMAGE_METRICS(con(0x8), con(0x1), &vw_24, ddata.v1d6c02, &vw_18);
        x16 wordrg11 = ddata.v1d2708 - vw_24 * vw_18;
        ddata.backbuff1 = wordrg11;
        if (wordrg5 >= wordrg11)
          ddata.backbuff1 = con(0x0);
        else
        {
          ddata.backbuff1 = wordrg11 - wordrg5;
          SKW_SET_SRECT(&ddata.backbuffrect, con(0x0), vw_18, ddata.vx1d270a, ddata.vx1d270c - (vw_18 + vw_30));
        }
        ddata.backbuff2 = con(0x1);
        SKW_UPDATE_WEATHER(false);
        ddata.v1e0390.l0 = con(0x1);
        SKW_CHECK_RECOMPUTE_LIGHT();
      }
SPX_DEBUG_POP;
      return;
    }
    // M_2B19F: co-label M_2B19B
    for (;; vw_2c++)
    {
      if (vw_2c >= ddata.savegamewb)
        break;
      x16 wordrg6 = unsignedword(*pb_04);
      pb_04++;
      if ((CUTX8(wordrg6) & con(0x10)) != con(0x0))
      {
        x16 wordrg3 = *pw_00++;
        // M_2AF58:
        do
        {
          bool doit1 = false; // added
          bool doit2 = false; // added
          ui16 uwordrg100 = urshift(wordrg3 & con(0x3c00), con(0xa));
          ui16 uwordrg41; // TODO set unlucky halfways...
          if (uwordrg100 < con(0x2))
          {
            ;
          }
          else if (uwordrg100 <= con(0x2))
          {
            recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(wordrg3);
            if ((recoptrrg2->u2.w & con(0x6)) == con(0x2))
            {
              ui16 uwordrg190 = urshift(recoptrrg2->u2.w, con(0xb)) & con(0x1f);
              if (uwordrg190 < con(0xd))
              {
                if (uwordrg190 < con(0x2))
                {
                  flag = uwordrg190 == con(0x0);
                  doit1 = true;
                }
                else if (uwordrg190 <= con(0x2))
                  doit2 = true;
                else if (uwordrg190 < con(0x4))
                {
                  ;
                }
                else if (uwordrg190 <= con(0x8))
                  doit2 = true;
                else
                {
                  flag = uwordrg190 == con(0xa);
                  doit1 = true;
                }
                if (doit1)
                  // M_2B129:
                  if (flag)
                    doit2 = true;
              }
              else if (uwordrg190 <= con(0xd))
                doit2 = true;
              else if (uwordrg190 < con(0xf))
                vbool_08 = true;
              else if (uwordrg190 <= con(0x11))
                doit2 = true;
              else if (uwordrg190 >= con(0x13) && (uwordrg190 <= con(0x13) || (uwordrg190 >= con(0x15) && uwordrg190 <= con(0x16))))
                gptrrg5[CUTX8(urshift(recoptrrg2->u2.w, con(0x3)))] = con(0x1);
            }
            if (doit2)
            {
              //M_2B13B:
              if (CUTX8(wordrg6 >> con(0x5)) != con(0x0))
                bptrrg11 = &pb_10[CUTX8(urshift(recoptrrg2->u2.w, con(0x3)))];
              else
                bptrrg11 = &pb_0c[CUTX8(urshift(recoptrrg2->u2.w, con(0x3)))];
              *bptrrg11 = con(0x1);
            }
          }
          else if (uwordrg100 != con(0x3) || dm2_dballochandler.v1e13fe[0] != con(0x0)) // TODO: v1e13fe probably a bool-array
          {
            ;
          }
          else if (recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg3), uwordrg41 = recoptrrg1->u2.w & con(0x7f), uwordrg41 < UI16CAST(con(0x2e)))
          {
            if (uwordrg41 == con(0x27))
            {
              if (((wordrg6 >> con(0x5)) & con(0xff)) == con(0x5))
              {
                x16 wordrg21 = urshift(recoptrrg1->u2.w, con(0x7)) - con(0x1);
                if (wordrg21 != ddata.v1d3248)
                {
                  s10ptrrg1 = &ddata.v1e03c8[wordrg21];
                  vw_20 = ulrshift(ddata.v1e03c8[wordrg21].w8, con(0x5), con(0xb));
                  x16 wordrg42 = vw_20;
                  wordrg21 *= con(0x4);
                  vw_20 = urshift(ddata.v1e03c8[wordrg21].w8, con(0xb));
                  x8** hlpptr1 = ddata.v1e03cc[wordrg21 / 4];
                  x8* hlpptr2 = hlpptr1[wordrg42];
                  bptrrg2 = &hlpptr2[vw_20];
                  bptrrg2++;
                  x16 wordrg41 = ulrshift(ddata.v1e03c8[wordrg21].wc, con(0x8), con(0xc)) - con(0x1);
                  while (wordrg41 >= con(0x0))
                  {
                    gptrrg5[bptrrg2[wordrg41]] |= con(0x2);
                    wordrg41--;
                  }
                }
              }
            }
          }
          else if (uwordrg41 <= con(0x2e))
          {
            if (CUTX8(wordrg6 >> con(0x5)) == con(0x0))
              gptrrg5[urshift(recoptrrg1->u2.w, con(0x7))] = con(0x1);
          }
          else if (uwordrg41 == con(0x7e))
            SKW_MARK_DYN_LOAD((unsignedlong(CUTX8(urshift(recoptrrg1->u2.w, con(0x7)))) << con(0x10)) + con(0x1600ffff));
          // M_2B185:
          wordrg3 = SKW_GET_NEXT_RECORD_LINK(wordrg3);
        } while (wordrg3 != con(0xfffffffe));
        continue;
      }
    }
    vw_28++;
  }
SPX_DEBUG_POP;
}

bool SKW_19f0_0891(x8 eaxb, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1)
{
  x16 wordrg5;
  x16 vw_00;
  x16 vw_04;
  x16 vw_08;
  x16 vw_14;
  x8 vb_18; // and8(0x18, con(0x7f));
  bool flag;

  // SPX: moved variables before goto
  x16 vw_0c;
  x16 wordrg3;
  x8 byterg2;
  ui16 uwordrg1;
  bool skip = false;

  vb_18 = eaxb;
  vw_00 = ebxw;
  vw_08 = ecxw;
  vw_04 = unsignedword(eaxb & con(0xffffff80));
  if (vw_04 != con(0x0))
    vb_18 &= con(0x7f);
  x16 vw_10 = ddata.s350.v1e0578;
  if (vw_10 == con(0x0))
    goto M_false;
  if (vw_08 == con(0xffffffff))
  {
    vw_08 = table1d27fc[argw1] + edxw;
    argw0 = vw_00 + table1d2804[argw1];
  }
  if (vw_08 < con(0x0) || vw_08 >= ddata.savegamewa)
    goto M_false;
  if (argw0 < con(0x0) || argw0 >= ddata.savegamewb)
    goto M_false;
  if (edxw != vw_08 && vw_00 != argw0)
    goto M_false;
   vw_0c = SKW_CALC_SQUARE_DISTANCE(edxw, vw_08, argw0, vw_00);
  if (vw_0c > con(0x1))
  {
    wordrg5 = vw_10 & con(0xff8);
    flag = wordrg5 == con(0x0);
    vw_10 = wordrg5;
    if (flag)
      goto M_false;
  }
  if (vw_0c == con(0x0))
  {
    vw_10 &= con(0x7);
    if (vw_10 == con(0x0))
      goto M_false;
  }
  if (vw_0c > urshift(ddata.s350.v1e0552->w14, con(0xc)))
    goto M_false;
  wordrg5 = vw_0c;
  if (wordrg5 == con(0x0))
  {
    while (wordrg5 < con(0x4))
    {
      x16 wordrg24 = table1d27fc[wordrg5] + edxw;
      if (SKW_CREATURE_GO_THERE(con(0x0), vw_00, wordrg24, edxw, vw_00 + table1d2804[wordrg5], wordrg5))
        break;
      wordrg5++;
    }
    if (wordrg5 < con(0x4))
      goto M_false;
  }
  if (UI8CAST(vb_18) <= UI8CAST(con(0x1)))
  {
    if (ddata.v1d3248 != ddata.v1e08d6 || vw_08 != ddata.v1e08d8 || argw0 != ddata.v1e08d4)
      goto M_false;
  }
  else
  {
    if (vb_18 == con(0x2))
    {
      SKW_19f0_045a(vw_08, argw0);
      if (SKW_19f0_050f() == con(0xfffffffe))
        goto M_false;
    }
  }
  if (vw_0c > con(0x1))
    if (SKW_19f0_0207(edxw, vw_08, argw0, vw_00, R_1BAAD) == con(0x0))
      goto M_false;
  if ((vw_10 & con(0x4)) != con(0x0))
  {
    vw_14 = con(0xffffffff);
    wordrg5 = con(0x0);
    // M_1C7E0: // co-label M_1C7DF
    for (;; wordrg5++)
    {
      if (wordrg5 < con(0x4))
      {
        x16 wordrg22 = SKW_2c1d_028c(edxw, wordrg5, vw_00);
        x16 wordrg45 = wordrg22;
        if (wordrg45 == con(0xffffffff))
          continue;
        x16 wordrg44 = ddata.hero[wordrg45].warrc3[1];
        if ((wordrg44 == con(0xffffffff)) || SKW_1c9a_0006(wordrg44, con(0xb)) == con(0x0))
        {
          x16 wordrg43 = ddata.hero[wordrg22].warrc3[0];
          if (wordrg43 == con(0xffffffff) || SKW_1c9a_0006(wordrg43, con(0xb)) == con(0x0))
            continue;
        }
        if (vw_14 < con(0x0) || SKW_RAND01() != con(0x0))
          vw_14 = unsignedword(ddata.hero[wordrg22].b1d);
        continue;
      }
      if (vw_14 >= con(0x0))
        break;
      vw_10 &= con(0xfffffffb);
      if (vw_10 == con(0x0))
        goto M_false;
      break;
    }
  }
  // M_1C805:
  if ((table1d607e[ddata.s350.v1e0584].u.b[1] & con(0x2)) != con(0x0))
  {
    SKW_19f0_045a(vw_08, argw0);
    x16 wordrg42 = SKW_19f0_04bf();
    bool skip = false;
    for(;;)
    {
      if (wordrg42 == con(0xfffffffe))
      {
        skip = true;
        break;
      }
      if (urshift(wordrg42 & con(0x3c00), con(0xa)) == con(0xf) && (SKW_GET_ADDRESS_OF_RECORD(wordrg42)->u2.w & con(0x7f)) == con(0xe))
        break;
      wordrg42 = SKW_GET_NEXT_RECORD_LINK(wordrg42);
    }
    if (!skip)
    {
      wordrg5 = vw_10;
      wordrg5 &= con(0x7);
      flag = wordrg5 == con(0x0);
      vw_10 = wordrg5;
      if (flag)
        goto M_false;
    }
  }
  if (ddata.v1e08d6 == ddata.v1d3248 && (table1d607e[ddata.s350.v1e0584].u.b[1] & con(0x40)) != con(0x0) && (ddata.s350.v1e0552->u0.b.b0 & con(0x20)) == con(0x0))
  {
    if (unsignedword(ddata.s350.v1e0562.u.b.b_06) != edxw || unsignedword(ddata.s350.v1e0562.u.b.b_07) != vw_00 || unsignedword(ddata.s350.v1e0571) != ddata.v1d3248)
    {
      wordrg5 = edxw;
      if ((((SKW_GET_TILE_VALUE(wordrg5, vw_00) & con(0xff)) >> con(0x5)) & con(0xff)) == con(0x4))
        if (SKW_CALC_SQUARE_DISTANCE(wordrg5, ddata.v1e08d8, ddata.v1e08d4, vw_00) < con(0x2) && SKW_RAND02() != con(0x0))
          goto M_false;
    }
  }
  if (vw_04 == con(0x0))
    return true;
  if (argw1 == con(0xffffffff))
  {
    if (vw_0c != con(0x0) || ddata.v1d3248 != ddata.v1e08d6 || vw_08 != ddata.v1e08d8 || argw0 != ddata.v1e08d4)
      argw1 = SKW_CALC_VECTOR_DIR(edxw, vw_08, argw0, vw_00);
    else
      argw1 = (ddata.v1e0258 + con(0x2)) & con(0x3);
  }
  if (SKW_19f0_0559(argw1))
    return true;
   wordrg3 = SKW_RAND01();
   byterg2;
  if (vw_0c <= con(0x1) && (vw_10 & con(0x7)) != con(0x0) && ((vw_10 & con(0xff8)) == con(0x0) || SKW_RAND01() != con(0x0)))
  {
    vw_10 &= con(0x7);
    x16 wordrg41;
    if (UI8CAST(vb_18) <= UI8CAST(con(0x1)) && (wordrg41 = SKW_2c1d_028c(edxw, con(0xff), vw_00), wordrg41 != con(0xffffffff)))
      byterg2 = ddata.hero[wordrg41].b1d;
    else
      byterg2 = CUTX8((wordrg3 + con(0x2) + argw1) & con(0x3));
  }
  else
  {
    vw_10 &= con(0xff8);
    if (UI8CAST(vb_18) <= UI8CAST(con(0x1)) && SKW_RAND02() != con(0x0))
    {
      wordrg3 = SKW_RAND01();
      x16 wordrg21;
      if (wordrg3 == con(0x0))
        wordrg21 = argw1;
      else
        wordrg21 = (argw1 + con(0x2)) & con(0x3);
      if (SKW_GET_PLAYER_AT_POSITION(wordrg21) == con(0xffffffff))
      {
        wordrg21 += con(0x3);
        wordrg21 &= con(0x3);
        if (SKW_GET_PLAYER_AT_POSITION(wordrg21) == con(0xffffffff))
          wordrg3 = con(0x1) - wordrg3;
      }
    }
    byterg2 = CUTX8((argw1 + wordrg3) & con(0x3));
  }
   skip = false;
  wordrg5 = vw_10;
   uwordrg1 = SKW_COMPUTE_POWER_4_WITHIN(wordrg5, SKW_RAND16(SKW_1c9a_0598(wordrg5)) + con(0x1));
  if (uwordrg1 < con(0x20))
  {
    if (uwordrg1 < con(0x4))
    {
      if (uwordrg1 >= con(0x1))
      {
        if (uwordrg1 <= con(0x1))
          ddata.s350.v1e0556->u.v.b1a = con(0x8);
        else
        {
          if (uwordrg1 == con(0x2))
            ddata.s350.v1e0556->u.v.b1a = con(0x26);
        }
      }
      skip = true;
    }
    else if (uwordrg1 <= con(0x4))
    {
      ddata.s350.v1e0556->u.v.b1a = con(0xa);
      ddata.s350.v1e0556->u.v.b1e = con(0xb);
      skip = true;
    }
    else if (uwordrg1 < con(0x8))
      skip = true;
    else if (uwordrg1 <= con(0x8))
    {
      ddata.s350.v1e0556->u.v.b1a = ((wordrg3 != con(0x0)) ? 1 : 0) + con(0xe);
      skip = true;
    }
    else if (uwordrg1 == con(0x10))
      vw_0c = con(0xffffff80);
    else
      skip = true;
  }
  else if (uwordrg1 <= con(0x20))
    vw_0c = con(0xffffff83);
  else if (uwordrg1 < con(0x100))
  {
    if (uwordrg1 < con(0x40))
      skip = true;
    else if (uwordrg1 <= con(0x40))
      vw_0c = con(0xffffff82);
    else if (uwordrg1 == con(0x80))
      vw_0c = con(0xffffff87);
    else
      skip = true;
  }
  else if (uwordrg1 <= con(0x100))
    vw_0c = con(0xffffff86);
  else if (uwordrg1 < con(0x400))
  {
    if (uwordrg1 == con(0x200))
      vw_0c = con(0xffffff81);
    else
      skip = true;
  }
  else if (uwordrg1 <= con(0x400))
    vw_0c = con(0xffffff89);
  else if (uwordrg1 == con(0x800))
    vw_0c = con(0xffffff8a);
  else
    skip = true;
  if (!skip)
  {
    ddata.s350.v1e0556->u.v.b1e = CUTX8(vw_0c);
    ddata.s350.v1e0556->u.v.b1a = ((wordrg3 != con(0x0)) ? 1 : 0) + con(0x27);
  }
  ddata.s350.v1e0556->u.v.u18.b.b0 &= con(0xffffffe0);
  ddata.s350.v1e0556->u.v.u18.w |= vw_08 & con(0x1f);
  wordrg5 = ddata.s350.v1e0556->u.v.u18.w & con(0xfffffc1f);
  ddata.s350.v1e0556->u.v.u18.w = wordrg5 | ((argw0 & con(0x1f)) << con(0x5));
  ddata.s350.v1e0556->u.v.u18.b.b1 &= con(0x3);
  ddata.s350.v1e0556->u.v.u18.w |= ((ddata.v1d3248 & con(0x3f)) << con(0xa));
  ddata.s350.v1e0556->u.v.b1b = CUTX8(argw1);
  if (ddata.s350.v1e0556->u.v.b1a == con(0xa))
    byterg2 = CUTX8(vw_14);
  ddata.s350.v1e0556->u.v.b1c = byterg2;
  ddata.s350.v1e0556->u.v.b20 = vb_18;
  ddata.s350.v1e056f = con(0xfffffffc);
  return true;

M_false: // M_1CCD9:
  if (vw_04 != con(0x0))
    ddata.s350.v1e056f = con(0xfffffffd);
  return false;
}

bool SKW_19f0_0d10(x8 eaxb, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1)
{
  x16 wordrg5;
  bool boolrg1;
  c_record* recoptrrg2;
  c_record* recoptrrg7;
  x16 vw_00;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_18;
  x8 vb_1c; // and8(0x1c, con(0x7f));
  bool flag;

	// SPX: moved variables before goto
	x16 wordrg188;
	bool pass = false; // added

  vb_1c = eaxb;
  vw_08 = edxw;
  vw_0c = ebxw;
  vw_10 = ecxw;
  vw_04 = con(0x0);
  vw_18 = unsignedword(eaxb & con(0xffffff80));
  if (vw_18 != con(0x0))
    vb_1c &= con(0x7f);
  x16 wordrg184;
  if (vb_1c != con(0x0))
    wordrg184 = con(0x73);
  else
    wordrg184 = con(0x6f);
  wordrg184 &= unsignedword(ddata.s350.v1e057a);
  x16 vw_14 = wordrg184;
  if (vw_14 == con(0x0))
    goto M_false;
  wordrg5 = vw_10;
  if (wordrg5 != con(0xffffffff))
  {
    if (argw1 != con(0xffffffff))
    {
      if (wordrg5 == vw_08)
        if (vw_0c == argw0)
          goto M_false;
    }
    else
      argw1 = SKW_CALC_VECTOR_DIR(vw_08, vw_10, argw0, vw_0c);
  }
  else
  {
    vw_10 = vw_08 + table1d27fc[argw1];
    wordrg5 = table1d2804[argw1];
    argw0 = vw_0c + wordrg5;
  }
  if (vw_10 < con(0x0) || vw_10 >= ddata.savegamewa)
    goto M_false;
  if (argw0 < con(0x0) || argw0 >= ddata.savegamewb)
    goto M_false;
  if (vw_08 != vw_10 && vw_0c != argw0)
    goto M_false;
  SKW_19f0_045a(vw_10, argw0);
  if ((((unsignedlong(ddata.v1e08ae) >> con(0x5)) & con(0xff)) != con(0x4)))
    goto M_false;
   pass = false; // added
   wordrg188 = unsignedword(CUTX8(ddata.v1e08ae) & con(0x7));
  if (vb_1c != con(0x0))
  {
    if (wordrg188 == con(0x5))
      goto M_false;
    flag = wordrg188 != con(0x4);
  }
  else
    flag = wordrg188 != con(0x0);
  if (flag)
  {
    if (ddata.v1e08b0 == con(0xffffffff))
      ddata.v1e08b0 = SKW_GET_TILE_RECORD_LINK(vw_10, argw0);
    recoptrrg7 = SKW_GET_ADDRESS_OF_RECORD(ddata.v1e08b0);
    if ((vb_1c == con(0x2)) && ((recoptrrg7->u2.b.b1 & con(0x10)) == con(0x0)))
      goto M_false;
    if ((recoptrrg7->u2.b.b1 & con(0x4)) == con(0x0))
    {
      ;
    }
    else if ((recoptrrg7->u2.b.b1 & con(0x2)) == con(0x0))
    {
      if (vb_1c != con(0x0))
      {
        vw_04 = con(0x2);
        pass = true;
      }
      else
      {
        vw_14 &= con(0xfffffff3);
        if (vw_14 == con(0x0))
          goto M_false;
      }
    }
    else if (vb_1c != con(0x0))
    {
      ;
    }
    else
    {
      vw_04 = con(0x2);
      pass = true;
    }
  }
  else
  {
    vw_04 = con(0x1);
    pass = true;
  }
  if (!pass)
  {
    vw_00 = SKW_CALC_SQUARE_DISTANCE(vw_08, vw_10, argw0, vw_0c);
    x16 wordrg42 = vw_00;
    if (wordrg42 != con(0x1))
    {
      if (wordrg42 > urshift(ddata.s350.v1e0552->w14, con(0xc)))
        goto M_false;
      bool boolrg101;
      if ((vw_14 & con(0x1)) != con(0x0) && (((recoptrrg7->u2.b.b0 & con(0x40)) != con(0x0)) || ((recoptrrg7->u2.b.b1 & con(0x20)) != con(0x0))))
        boolrg101 = false;
      else if (vb_1c != con(0x0))
        boolrg101 = true;
      else
      {
        if ((vw_14 & con(0x4)) != con(0x0) && ((recoptrrg7->u2.b.b0 & con(0xffffff80)) != con(0x0)))
          boolrg101 = false;
        else
          boolrg101 = true;
      }
      if (boolrg101)
        goto M_false;
      if (SKW_19f0_0207(vw_08, vw_10, argw0, vw_0c, R_1BAAD) == con(0x0))
        goto M_false;
    }
    else
    {
      if (((vw_14 & con(0x3)) == con(0x0) || (((recoptrrg7->u2.b.b0 & con(0x40)) == con(0x0)) && ((recoptrrg7->u2.b.b1 & con(0x20)) == con(0x0)))) && (vb_1c != con(0x0) || (((vw_14 & con(0x4)) == con(0x0) || ((recoptrrg7->u2.b.b0 & con(0xffffff80)) == con(0x0))) && ((vw_14 & con(0x8)) == con(0x0) || ((recoptrrg7->u2.b.b1 & con(0x1)) == con(0x0))))))
        if ((vw_14 & con(0x40)) == con(0x0))
          goto M_false;
    }
    if ((vw_14 & con(0x1)) != con(0x0))
    {
      x16 wordrg6 = vw_08;
      x16 wordrg3 = vw_0c;
      wordrg5 = vw_00;
      // M_1D155:
      for(;;)
      {
        bool farbreak = false; // added
        if (wordrg5 < con(0x0))
          break;
        x16 wordrg190 = SKW_GET_WALL_TILE_ANYITEM_RECORD(wordrg6, wordrg3);
        // M_1D093: // co-label M_1D12E
        x16 wordrg41;
        for (;; wordrg190 = SKW_GET_NEXT_RECORD_LINK(wordrg41))
        {
          wordrg41 = wordrg190;
          if (wordrg41 == con(0xfffffffe))
          {
            wordrg6 += table1d27fc[argw1];
            wordrg3 += table1d2804[argw1];
            wordrg5--;
            break;
          }
          x16 wordrg191 = urshift(wordrg41 & con(0x3c00), con(0xa));
          if (wordrg191 != con(0xe))
          {
            if (wordrg191 != con(0xf))
              continue;
            x16 wordrg192 = SKW_GET_ADDRESS_OF_RECORD(wordrg41)->u2.w & con(0x7f);
            if (wordrg192 != con(0xd))
            {
              flag = wordrg192 != con(0x4);
              if (flag)
                continue;
            }
          }
          else
          {
            recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(wordrg41);
            x16 wordrg192 = recoptrrg2->u2.w;
            if (wordrg192 != con(0xffffff8d) && wordrg192 != con(0xffffff84))
              continue;
            flag = ulrshift(ddata.savegametimarrp[recoptrrg2->u6.w].v.w_08, con(0x4), con(0xe)) != argw1;
            if (flag)
              continue;
          }
          vw_04 = con(0x2);
          farbreak = true;
          break;
        }
        if (farbreak)
          break;
      }
    }
  }
  if ((vw_04 != con(0x0)) && (vb_1c == con(0x2)))
    goto M_false;
  if (vw_18 == con(0x0))
    return true;
  if (argw1 == con(0xffffffff))
    argw1 = SKW_CALC_VECTOR_DIR(vw_08, vw_10, argw0, vw_0c);
  if (vw_04 == con(0x1))
  {
    ddata.s350.v1e056f = con(0xfffffffe);
    return true;
  }
  if (vw_04 == con(0x2))
  {
    ddata.s350.v1e0556->u.v.b1a = con(0x0);
    ddata.s350.v1e056f = con(0xfffffffc);
    return true;
  }
  if (SKW_19f0_0559(argw1))
    return true;
  if ((vw_14 & con(0x20)) != con(0x0))
  {
    vw_14 &= con(0xffffffdf);
    if (vb_1c == con(0x0))
      recoptrrg7->u2.b.b1 |= con(0x10);
  }
  if (vw_00 > con(0x1))
    vw_14 &= con(0x5);
  if ((vw_14 & con(0x42)) != con(0x0) && ((vw_14 & con(0xffffffbd)) == con(0x0) || SKW_RAND02() != con(0x0)))
    ddata.s350.v1e0556->u.v.b1a = con(0xb);
  else
  {
    if ((vw_14 & con(0x1)) == con(0x0) || ((vw_14 & con(0xffffffbc)) != con(0x0) && SKW_RAND02() != con(0x0)))
    {
      wordrg5 = ddata.s350.v1e0578;
      if ((vw_14 & con(0x8)) == con(0x0) || (wordrg5 & con(0x1)) == con(0x0) || ((vw_14 & con(0xffffffb4)) != con(0x0) && SKW_RAND01() == con(0x0)))
      {
        if ((vw_14 & con(0x4)) == con(0x0))
          goto M_false;
        ddata.s350.v1e0578 &= con(0x50);
      }
      else
        ddata.s350.v1e0578 &= con(0x1);
      boolrg1 = SKW_19f0_0891(con(0x84), vw_0c, vw_10, vw_08, argw0, argw1);
      ddata.s350.v1e0578 = wordrg5;
      return boolrg1;
    }
    ddata.s350.v1e0556->u.v.b1a = ((SKW_RAND01() != con(0x0)) ? 1 : 0) + con(0x27);
    if (vb_1c != con(0x0))
      ddata.s350.v1e0556->u.v.b1e = con(0x84);
    else
      ddata.s350.v1e0556->u.v.b1e = con(0x8d);
  }
  ddata.s350.v1e0556->u.v.u18.b.b0 &= con(0xffffffe0);
  ddata.s350.v1e0556->u.v.u18.w |= vw_10 & con(0x1f);
  wordrg5 = ddata.s350.v1e0556->u.v.u18.w & con(0xfffffc1f);
  ddata.s350.v1e0556->u.v.u18.w = wordrg5 | ((argw0 & con(0x1f)) << con(0x5));
  ddata.s350.v1e0556->u.v.u18.b.b1 &= con(0x3);
  ddata.s350.v1e0556->u.v.u18.w |= (ddata.v1d3248 & con(0x3f)) << con(0xa);
  ddata.s350.v1e0556->u.v.b1d = CUTX8(argw1);
  ddata.s350.v1e0556->u.v.b1b = CUTX8(argw1);
  ddata.s350.v1e0556->u.v.b1c = (CUTX8(SKW_RAND01()) + CUTX8(argw1)) & con(0x3);
  ddata.s350.v1e0556->u.v.b20 = vb_1c;
  ddata.s350.v1e056f = con(0xfffffffc);
  return true;

M_false: // M_1CFBF:
  if (vw_18 != con(0x0))
    ddata.s350.v1e056f = con(0xfffffffd);
  return false;
}

// lots of calls, but it seems that the return is checked against 0 only
bool SKW_CREATURE_GO_THERE(x8 eaxb, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1)
{
  x16 wordrg5;
  c_record* recoptrrg2;
  c_record* prec_00 = NULL; // TODO: serious!
  x16 vw_20;
  x16 vw_24;
  x16 vw_28;
  x16 vw_2c;
  x16 vw_30;
  x16 vw_38;
  x16 vw_3c;
  x16 vw_40;
  x16 vw_4c;
  x8 vb_50;
  x8 vb_54;
  bool flag;

	// SPX: moved variables before goto
	x16 wordrg188;
	x16 vw_44;
	x16 wordrg2;
	bool vbool_0c;

  vb_54 = eaxb;
  vw_28 = edxw;
  vw_24 = ebxw;
  wordrg5 = ecxw;
  x16 vw_48; // multipurpose temporary?!?
  bool vbool_10 = false;
  bool vbool_08 = false;
  x16 vw_14 = con(0x0);
  bool vbool_18 = false;
  vw_30 = unsignedword(eaxb & con(0xffffff80));
  if (vw_30 != con(0x0))
    vw_38 = con(0xffffffff);
  vw_20 = unsignedword(vb_54 & con(0x40));
  x16 wordrg6 = unsignedword(vb_54 & con(0x20));
  vb_54 &= con(0x1f);
  if (vb_54 == con(0x2) || ((vb_50 = table1d6290[vb_54], vb_50 == con(0x0)) && ddata.s350.v1e0576 == con(0x0)))
    goto M_preexit;
   vw_44 = argw1;
   wordrg2 = argw1;

  if (wordrg2 != con(0x6))
  {
    if (wordrg2 < con(0x4))
    {
      if (wordrg5 != con(0xffffffff))
      {
        if (wordrg5 != vw_28 || vw_24 != argw0)
          vbool_0c = false;
        else
          vbool_0c = true;
        if (!vbool_0c && argw1 == con(0xffffffff))
        {
          argw1 = SKW_CALC_VECTOR_DIR(vw_28, wordrg5, argw0, vw_24);
          // M_1DC07:
          vw_44 = argw1;
        }
      }
      else
      {
        vbool_0c = false;
        wordrg5 = vw_28 + table1d27fc[wordrg2];
        argw0 = vw_24 + table1d2804[wordrg2];
      }
    }
    else
    {
      vbool_0c = false;
      if ((ddata.s350.v1e0576 & con(0x2000)) == con(0x0))
        goto M_preexit;
      vw_40 = con(0x0);
      x16 wordrg47 = unsignedword(SKW_GET_TILE_VALUE(vw_28, vw_24));
      vw_48 = wordrg47;
      ui16 opt; // added
      ui8 ubyterg11 = ubrshift(CUTX8(wordrg47), con(0x5)) - con(0x1);
      if (ubyterg11 > con(0x5))
        opt = 2;
      else
      {
        switch (ubyterg11)
        {
          case 0:
          case 4:
          case 5:
            opt = 0;
          break;

          case 1:
            if (wordrg2 != con(0x5) || (wordrg47 & con(0x8)) == con(0x0) || (wordrg47 & con(0x1)) != con(0x0))
              opt = 0;
            else
            {
              vw_40 = con(0x8);
              vw_38 = con(0x38);
              // M_1D974:
              vw_44 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
              opt = 2;
            }
          break;

          case 2:
            if ((wordrg47 & con(0x4)) == con(0x0))
            {
              // M_1DA21:
              if (wordrg2 != con(0x5))
                opt = 2;
              else
              {
                vw_38 = con(0x36);
                opt = 1;
              }
            }
            else
            {
              if (wordrg2 != con(0x4))
                opt = 2;
              else
              {
                vw_38 = con(0x35);
                opt = 1;
              }
            }
          break;

          case 3:
            opt = 2;
          break;

          default: throw(THROW_DMABORT);
        }
      }
      switch (opt)
      {
        case 0:
          // M_1D98D:
          if ((vw_48 & con(0x2)) != con(0x0))
          {
            vw_44 = SKW_FIND_LADDAR_AROUND(vw_28, (argw1 != con(0x4)) ? con(0x1) : con(0xffffffff), &prec_00, vw_24);
            if (vw_44 < con(0x0))
            {
              vw_38 = con(0x37);
              vw_40 = con(0x10);
              // M_1D974:
              vw_44 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
            }
            else
            {
              vw_38 = ((argw1 != con(0x4)) ? 1 : 0) + con(0x39);
              // M_1D9EE:
              vw_40 = con(0x100);
            }
          }
        break;

        case 1:
          // M_1DA2C:
          vw_44 = (SKW_0cee_06dc(vw_28, vw_24) + con(0x2)) & con(0x3);
          // M_1D9EE:
          vw_40 = con(0x100);
        break;

        case 2:
        break;
      }
      // M_1DA4C:
      if ((vw_40 & ddata.s350.v1e0576) == con(0x0))
        goto M_preexit;
      vw_2c = ddata.v1d3248;
      if (prec_00 == NULL || (vw_38 != con(0x39) && vw_38 != con(0x3a)))
      {
        // M_1DAB8:
        x16 wordrg107;
        if (argw1 != con(0x4))
          wordrg107 = con(0x1);
        else
          wordrg107 = con(0xffffffff);
        vw_4c = SKW_LOCATE_OTHER_LEVEL(ddata.v1d3248, &vw_28, &vw_24, wordrg107, NULL);
        if (vw_4c < con(0x0))
          goto M_preexit;
        if (vw_38 == con(0x37))
        {
          x8*** hlpptr1 = &ddata.v1e03cc[vw_4c];
          x8** hlpptr2 = *hlpptr1 + vw_28;
          x8* hlpptr3 = *hlpptr2 + vw_24;
          x16 wordrg46 = unsignedword(*hlpptr3);
          vw_48 = wordrg46;
          if (ubrshift(*hlpptr3, con(0x5)) != con(0x2) || ((wordrg46 & con(0x8)) == con(0x0)))
            goto M_preexit;
        }
      }
      else
      {
        vw_28 = ulrshift(prec_00->u6.w, con(0x5), con(0xb));
        vw_24 = urshift(prec_00->u6.w, con(0xb));
        vw_4c = urshift(prec_00->u2.w, con(0x7));
      }
      // M_1DB51:
      if (!SKW_IS_CREATURE_ALLOWED_ON_LEVEL(ddata.s350.v1e054c, vw_4c))
        goto M_preexit;
      vbool_08 = true;
      SKW_1c9a_0648(vw_4c);
      wordrg5 = vw_28; // adj
      argw0 = vw_24;
      vw_24 = con(0xffffffff);
      vw_28 = con(0xffffffff);
    }
  }
  else
  {
    vbool_0c = false;
    // M_1DC07:
    vw_44 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
  }
  // M_1DC0B:
  // M_1DC19:
  // M_1DC22:
  if (wordrg5 < con(0x0) || wordrg5 >= ddata.savegamewa)
    goto M_preexit;
  if ((argw0 < con(0x0)) || argw0 >= ddata.savegamewb)
    // M_1DC3B:
    // M_1DC40:
    goto M_preexit;
  if (vb_50 != con(0x0))
  {
    if (UI8CAST(vb_54) < UI8CAST(con(0x7)))
    {
      if (vb_54 == con(0x3))
      {
        SKW_19f0_045a(wordrg5, argw0);
        vw_48 = ddata.v1e08ae;
        x8 byterg144 = CUTX8(urshift(vw_48, con(0x5)));
        if (byterg144 != con(0x0) && (byterg144 != con(0x6) || (vw_48 & con(0x5)) != con(0x0)))
          // M_1DE90:
          vbool_10 = true;
        goto M_exit;
      }
    }
    else if (UI8CAST(vb_54) <= UI8CAST(con(0x7)))
    {
      vbool_10 = !R_1BA1B(wordrg5, argw0);
      flag = !vbool_10;
      if (!flag)
        SKW_19f0_045a(wordrg5, argw0);
      goto M_exit;
    }
    else if (vb_54 == con(0x8))
    {
      x16 wordrg3 = wordrg5;
      SKW_19f0_045a(wordrg3, argw0);
      ui8 byterg100 = CUTX8(urshift(ddata.v1e08ae, con(0x5)));
      if (byterg100 <= con(0x7))
      {
        switch (byterg100)
        {
          case 1:
          case 2:
            // M_1DE90:
            vbool_10 = true;
          goto M_exit;

          case 3:
          case 7:
          goto M_exit;

          case 4:
            vw_48 = unsignedword(CUTX8(ddata.v1e08ae) & con(0x7));
            if (vw_48 != con(0x0))
            {
              wordrg5 = vw_48;
              if (wordrg5 != con(0x5) && wordrg5 != con(0x1))
              {
                ddata.v1e08c4 = SKW_QUERY_DOOR_STRENGTH(SKW_IS_REBIRTH_ALTAR(SKW_GET_ADDRESS_OF_TILE_RECORD(wordrg3, argw0)));
                if (wordrg5 != con(0x4))
                {
                  x16 wordrg44 = ddata.v1e08c4;
                  if (wordrg44 > con(0x1))
                  {
                    x16 wordrg23 = ddata.v1e08c4 >> con(0x1);
                    if (wordrg5 != con(0x2))
                    {
                      if (wordrg44 > con(0x3))
                        ddata.v1e08c4 = wordrg23;
                      else
                        // M_1DE8A:
                        ddata.v1e08c4--;
                    }
                    else if (wordrg44 > con(0x3))
                    {
                      // ddata.v1e08c4 = wordrg23; overwritten in next line
                      // M_1DE8A:
                      ddata.v1e08c4 = wordrg23 / 2; // was full - half
                    }
                    else
                      ddata.v1e08c4 = con(0x1);
                  }
                }
              }
            }
            // M_1DE90:
            vbool_10 = true;
          goto M_exit;

          case 5:
            if ((ddata.v1e08ae & con(0x8)) == con(0x0))
            {
              ddata.v1e08b7 = SKW_GET_TELEPORTER_DETAIL(&ddata.v1e08b8, argw0, wordrg3) ? 1 : 0;
              if (ddata.v1e08b7 == con(0x0) || unsignedword(ddata.v1e08b8.b_04) == ddata.v1e0266 || unsignedword(ddata.v1e08b8.b_04) == ddata.v1e027c)
                // M_1DE90:
                vbool_10 = con(0x1);
            }
            else
            {
              recoptrrg2 = R_D283(wordrg3, argw0);
              if (recoptrrg2 == NULL)
                // M_1DE90:
                vbool_10 = true;
              else
              {
                vw_48 = urshift(recoptrrg2->u4.w, con(0x8));
                if (vw_48 == ddata.v1e0266 || vw_48 == ddata.v1e027c)
                {
                  // M_1DD7F:
                  vw_4c = ddata.v1d3248;
                  SKW_1c9a_0648(vw_48);
                  x16 wordrg112 = ulrshift(recoptrrg2->u2.w, con(0x6), con(0xb));
                  x16 wordrg45 = recoptrrg2->u2.w & con(0x1f);
                  vbool_10 = SKW_CREATURE_GO_THERE(vb_54, wordrg112, wordrg45, wordrg45, wordrg112, con(0x6));
                  SKW_1c9a_0648(vw_4c);
                }
                else
                  // M_1DE90:
                  vbool_10 = true;
              }
            }
          goto M_exit;

          case 6:
            if ((ddata.v1e08ae & con(0x5)) != con(0x0))
            {
              // M_1DE90:
              vbool_10 = true;
              goto M_exit;
            }
          case 0: // no break before
            // M_1DEBE:
            if (ddata.v1e0400 < con(0x32))
            {
              // M_1DE8A:
              ddata.v1e08c4 = ddata.v1e0400;
              // M_1DE90:
              vbool_10 = true;
            }
            else
              ddata.v1e08b6 = con(0x1);
          goto M_exit;

          default: throw(THROW_DMABORT);
        }
      }
    }
  }
  // M_1DEDF:
  SKW_19f0_045a(wordrg5, argw0);
   wordrg188; // TODO: really ugly
  if (ddata.s350.v1e0582 == con(0x0) || argw1 == con(0x6) || (wordrg188 = SKW_1c9a_1a48(con(0x1), ddata.s350.v1e0582), wordrg188 == con(0xffffffff)) || ((wordrg188 & (con(0x1) << ((argw1 + con(0x2)) & con(0x3)))) == con(0x0)))
  {
    bool doit = false;
    // M_1DF2D:
    if (vb_54 == con(0x4) || vb_54 == con(0x5))
    {
      // M_1DF3A:
      if (ddata.s350.v1e0552->b2 == con(0xff))
        vb_54 = con(0x0);
    }
    // M_1DF53:
    vw_48 = ddata.v1e08ae;
    ui8 bytrg100 = CUTX8(urshift(vw_48, con(0x5)));
    if (bytrg100 <= con(0x7))
    {
      x16 wordrg22 = vw_48 & con(0x1);
      x16 wordrg43 = vw_48 & con(0x8);
      switch (bytrg100)
      {
        case 0:
          vw_40 = con(0x1);
          doit = true;
        break;

        case 1:
          vw_40 = con(0x2);
          doit = true;
        break;

        case 2:
          if (wordrg43 == con(0x0))
            vw_40 = con(0x6);
          else if (wordrg22 == con(0x0))
          {
            if (!vbool_08)
              vw_40 = con(0x800c);
            else
              vw_40 = con(0xc);
          }
          else
            vw_40 = con(0x8024);
          doit = true;
        break;

        case 3:
          vw_40 = con(0x100);
          doit = true;
        break;

        case 4:
          vw_48 &= con(0x7);
          if (vw_48 != con(0x0) || (vb_54 == con(0x4)) || vb_54 == con(0x5))
          {
            // M_1E00C:
            if (vw_48 == con(0x5))
              vw_40 = con(0x2);
            else if (vw_48 != con(0x4))
            {
              if (ddata.v1e08b0 == con(0xffffffff))
                ddata.v1e08b0 = SKW_GET_TILE_RECORD_LINK(wordrg5, argw0);
              recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(ddata.v1e08b0);
              if (vw_20 == con(0x0) || ((recoptrrg2->u2.b.b1 & con(0x4)) == con(0x0)))
              {
                if ((recoptrrg2->u2.b.b1 & con(0x4)) == con(0x0) || (recoptrrg2->u2.b.b1 & con(0x2)) != con(0x0))
                {
                  x16 wordrg182;
                  if ((recoptrrg2->u2.b.b0 & con(0x20)) == con(0x0))
                    wordrg182 = con(0x1);
                  else
                    wordrg182 = urshift(ddata.s350.v1e0552->u0.w, con(0x6)) & con(0x3);
                  if (vw_48 > wordrg182)
                    vw_40 = con(0x4200);
                  else
                    vw_40 = con(0x2);
                }
                else
                  vw_40 = con(0x200);
              }
              else
              {
                if ((recoptrrg2->u2.b.b1 & con(0x2)) != con(0x0))
                  vw_40 = con(0x2);
                else
                  vw_40 = con(0x4200);
              }
            }
            else
              vw_40 = con(0x4200);
          }
          else
            vw_40 = con(0x2);
          doit = true;
        break;

        case 5:
          if (wordrg43 != con(0x0))
          {
            if (ddata.v1e08b0 == con(0xffffffff))
              ddata.v1e08b0 = SKW_GET_TILE_RECORD_LINK(wordrg5, argw0);
            vw_48 = ulrshift(SKW_GET_ADDRESS_OF_RECORD(ddata.v1e08b0)->u2.w, con(0x1), con(0xe));
            if (vw_48 == con(0x1) || vw_48 == con(0x3))
              vw_40 = con(0x400);
            else
              vw_40 = con(0x2);
            doit = true;
          }
          else
          {
            if (!SKW_GET_TELEPORTER_DETAIL(&ddata.v1e08b8, argw0, wordrg5))
            {
              vw_40 = con(0x402);
              doit = true;
            }
            else
            {
              ddata.v1e08b7 = con(0x1);
              if (SKW_IS_CREATURE_ALLOWED_ON_LEVEL(ddata.s350.v1e054c, unsignedword(ddata.v1e08b8.b_04)))
              {
                vw_40 = con(0x2000);
                doit = true;
              }
              // else don't do it
            }
          }
        break;

        case 6:
          if ((vw_48 & con(0x4)) != con(0x0))
            vw_40 = con(0x2);
          else
          {
            if (wordrg22 == con(0x0))
              vw_40 = con(0x40);
            else
              vw_40 = con(0x80);
          }
          doit = true;
        break;

        case 7:
        // don't do it
        break;

        default: throw(THROW_DMABORT);
      }
    }
    if (doit)
    {
      // M_1E1A2:
      x16 wordrg21 = vw_40 & ddata.s350.v1e0576;
      flag = wordrg21 == con(0x0);
      vw_40 = wordrg21;
      if (flag || (vb_54 == con(0x1) && ((DM2_ABS(argw0 - ddata.v1e08d4) + DM2_ABS(wordrg5 - ddata.v1e08d8)) <= con(0x1))))
        goto M_preexit;
      // M_1E1EB:
      x16 vw_34 = con(0x0);
      if (ddata.v1d3248 == ddata.v1e08d6 && wordrg5 == ddata.v1e08d8 && argw0 == ddata.v1e08d4)
        vw_34 |= con(0x800);
      if (!vbool_0c)
      {
        x16 wordrg31 = SKW_19f0_050f();
        if (wordrg31 != con(0xfffffffe) && (vw_20 == con(0x0) || SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(wordrg31)->wa == con(0x0)))
          if (wordrg6 == con(0x0) || vbool_08 || (vw_14 = SKW_19f0_1511(wordrg31), vw_14 == con(0x0))) // TODO: silly vw_14-assignment
            // M_1E26A:
            // M_1E273:
            vw_34 |= con(0x1000);
      }
      // M_1E27B:
      if ((ddata.s350.v1e0576 & vw_34) != vw_34)
        goto M_preexit;
      if ((vb_54 == con(0x4)) || vb_54 == con(0x5))
      {
        // M_1E2A5:
        x16 wordrg3 = SKW_19f0_04bf();
        // M_1E2AA:
        for (;; wordrg3 = SKW_GET_NEXT_RECORD_LINK(wordrg3))
        {
          if (wordrg3 == con(0xfffffffe))
          {
            if (vb_54 != con(0x5) || vbool_18 || !SKW_19f0_13aa(wordrg5, argw0))
              break;
            goto M_preexit;
          }
          x16 wordrg42 = con(0x0);
          x16 vw_48_3 = urshift(wordrg3 & con(0x3c00), con(0xa));
          if (vw_48_3 != con(0xf))
          {
            if (vw_48_3 == con(0xe))
            {
              if (wordrg5 != unsignedword(ddata.s350.v1e0562.u.b.b_06) || unsignedword(ddata.s350.v1e0562.u.b.b_07) != argw0)
                // M_1E340:
                // M_1E347:
                // M_1E349:
              {
                recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(wordrg3);
                if (argw1 != con(0x6))
                  // M_1E369:
                  vw_3c = argw1;
                else
                  vw_3c = vw_44;
                // M_1E36C:
                if (ulrshift(ddata.savegametimarrp[recoptrrg2->u6.w].v.w_08, con(0x4), con(0xe)) != vw_3c)
                  // M_1E3AA:
                  wordrg42 = SKW_075f_06bd(recoptrrg2, recoptrrg2->u2.w);
              }
            }
          }
          else
          {
            x16 wordrg190 = SKW_GET_ADDRESS_OF_RECORD(wordrg3)->u2.w & con(0x7f);
            if (wordrg190 != con(0x7))
            {
              if (wordrg190 == con(0xe))
                vbool_18 = true;
            }
            else
              // M_1E3AA:
              wordrg42 = SKW_CALC_CLOUD_DAMAGE(wordrg3, ddata.s350.v1e054c);
          }
          // M_1E3AC:
          if (wordrg42 != con(0x0))
            goto M_preexit;
        }
      }
      // M_1E3E6:
      bool skip = false;
      if (vbool_0c)
      {
        ;
      }
      else if (ddata.v1e08d6 != ddata.v1d3248)
      {
        ;
      }
      else if ((table1d607e[ddata.s350.v1e0584].u.b[1] & con(0x40)) == con(0x0))
      {
        ;
      }
      else if ((ddata.s350.v1e0552->u0.b.b0 & con(0x20)) != con(0x0))
      {
        ;
      }
      else if ((urshift(ddata.v1e08ae, con(0x5)) & con(0xff)) != con(0x4))
      {
        ;
      }
      else if (SKW_CALC_SQUARE_DISTANCE(wordrg5, ddata.v1e08d8, ddata.v1e08d4, argw0) >= con(0x2))
      {
        ;
      }
      else if ((unsignedword(ddata.s350.v1e0562.u.b.b_06) == vw_28) && ((unsignedword(ddata.s350.v1e0562.u.b.b_07) == vw_24) && SKW_RAND02() == con(0x0)))
      {
        ;
      }
      else
        skip = true;
      if (!skip)
      {
        // M_1E490:
        vbool_10 = true;
        if (vbool_08 || (wordrg6 = vw_40, wordrg6 == con(0x400))) // TODO: wordrg6 ugly transfer here
        {
          SKW_2fcf_0434(ddata.s350.v1e054c, vw_24, wordrg5, vw_28, signedlong(argw0), false);
          if (ddata.v1d3248 != ddata.v1e102c || wordrg5 != ddata.v1e102a || ddata.v1e1028 != argw0)
          {
            // M_1E507:
            vw_4c = ddata.v1d3248;
            bool doit = false;
            if (vw_4c == ddata.v1e102c)
              doit = true;
            else if ((ddata.s350.v1e0576 & con(0x2000)) != con(0x0) && SKW_IS_CREATURE_ALLOWED_ON_LEVEL(ddata.s350.v1e054c, ddata.v1e102c))
            {
              SKW_1c9a_0648(ddata.v1e102c);
              doit = true;
            }
            if (doit)
            {
              // M_1E55E:
              vbool_10 = SKW_CREATURE_GO_THERE(vb_54 | CUTX8(vw_20), ddata.v1e1028, ddata.v1e102a, ddata.v1e102a, ddata.v1e1028, con(0x6));
              SKW_1c9a_0648(vw_4c);
            }
            else
              vbool_10 = false;
          }
        }
        else if (!vbool_0c)
        {
          if (wordrg6 == con(0x4000))
            return SKW_19f0_0d10(CUTX8(vw_30), vw_24, wordrg5, vw_28, argw0, vw_44);
          if ((wordrg6 & con(0x8000)) != con(0x0))
          {
            if (vb_54 != con(0x6))
            {
              x16 wordrg101 = ddata.s350.v1e0576;
              vw_34 = wordrg101;
              wordrg101 &= con(0xffff7db7);
              ddata.s350.v1e0576 = wordrg101;
              vbool_10 = SKW_CREATURE_GO_THERE(vb_54 | CUTX8(vw_20), argw0, table1d27fc[vw_44] + wordrg5, wordrg5, argw0 + table1d2804[vw_44], vw_44);
              ddata.s350.v1e0576 = vw_34;
            }
            else
              vbool_10 = false;
            if (!vbool_10)
            {
              if ((ddata.s350.v1e0576 & con(0x8)) != con(0x0))
              {
                ddata.s350.v1e0576 &= con(0x7fff);
                vbool_10 = SKW_CREATURE_GO_THERE(vb_54 | CUTX8(vw_20), vw_24, wordrg5, vw_28, argw0, vw_44);
                if (vbool_10)
                  vw_38 = con(0x38);
                ddata.s350.v1e0576 = vw_34;
              }
            }
            else
              vw_38 = con(0x5);
            if (!vbool_10)
              ddata.v1e08b6 = con(0x1);
          }
        }
      }
    }
  }

M_preexit: // M_1E6C7:
  if (vw_30 == con(0x0))
    goto M_exit;
  if (vbool_10)
  {
    if (!vbool_0c)
    {
      if (vw_14 == con(0x0) || !SKW_19f0_0891(con(0x82), vw_24, wordrg5, vw_28, argw0, con(0xffffffff)))
      {
        // M_1E726:
        if ((ddata.s350.v1e057a & con(0x20)) != con(0x0))
        {
          // M_1E7B2: // co-label M_1E7AE
          for (x16 vw_48_1 = con(0x0); vw_48_1 <= con(0x3); vw_48_1++)
          {
            if (vw_48_1 == vw_44)
              continue;
            x16 wordrg41 = vw_28 + table1d27fc[vw_48_1];
            x16 wordrg29 = vw_24 + table1d2804[vw_48_1];
            if ((((SKW_GET_TILE_VALUE(wordrg41, wordrg29) & con(0xff)) >> con(0x5)) & con(0xff)) != con(0x4))
              continue;
            if (!SKW_19f0_0d10(con(0x82), vw_24, wordrg41, vw_28, wordrg29, vw_48_1))
              continue;
            goto M_exit;
          }
        }
        // M_1E7BD:
        if (vb_54 != con(0x6))
        {
          if (vw_38 == con(0xffffffff))
          {
            if ((DM2_ABS(vw_24 - argw0) + DM2_ABS(vw_28 - wordrg5)) > con(0x1))
              vw_38 = con(0x1);
            else
              vw_38 = con(0x2);
          }
        }
        else
        {
          vw_44 = (vw_44 + con(0x2)) & con(0x3);
          vw_38 = con(0x9);
        }
        x16 vw_48_2 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
        if (vw_48_2 == vw_44 || (vw_48_2 != ((vw_44 + con(0x2)) & con(0x3)) && vw_38 == con(0x2)))
        {
          // M_1E89A:
          x16 wordrg109 = vw_48_2;
          if (wordrg109 == vw_44 || vw_38 == con(0x9))
          {
            if ((table1d613a[ddata.s350.v1e056e] & con(0x2)) != con(0x0) && vb_54 != con(0x6) && !vbool_08)
            {
              vw_38 = con(0x0);
            }
          }
          else
          {
            wordrg109++;
            wordrg109 &= con(0x3);
            if (wordrg109 != vw_44)
              vw_38 = con(0x3);
            else
              vw_38 = con(0x4);
          }
        }
        // M_1E856:
        else if ((vb_54 == con(0x4) || vb_54 == con(0x5)) && (table1d607e[ddata.s350.v1e0584].u.b[1] & con(0x1)) == con(0x0) && SKW_RAND02() != con(0x0))
        {
          // M_1E863:
          vw_38 = con(0x9);
        }
        else
        {
          SKW_19f0_0559(vw_44);
          goto M_exit;
        }
        // M_1E8F6:
        wordrg5 &= con(0x1f);
        ddata.s350.v1e0556->u.v.u18.b.b0 &= con(0xffffffe0);
        ddata.s350.v1e0556->u.v.u18.w |= wordrg5;
        wordrg5 = (ddata.s350.v1e0556->u.v.u18.w & con(0xfffffc1f)) | ((argw0 & con(0x1f)) << con(0x5));
        ddata.s350.v1e0556->u.v.u18.w = wordrg5;
        ddata.s350.v1e0556->u.v.u18.b.b1 &= con(0x3);
        ddata.s350.v1e0556->u.v.u18.w |= (ddata.v1d3248 & con(0x3f)) << con(0xa);
        ddata.s350.v1e0556->u.v.b1b = CUTX8(argw1);
        ddata.s350.v1e0556->u.v.b1d = CUTX8(vw_44);
        ddata.s350.v1e0556->u.v.b20 = vb_54;
        ddata.s350.v1e0556->u.v.b1a = CUTX8(vw_38);
        ddata.s350.v1e056f = con(0xfffffffc);
        goto M_exit;
      }
    }
    else
      ddata.s350.v1e056f = con(0xfffffffe);
  }
  else
    ddata.s350.v1e056f = con(0xfffffffd);
M_exit: // M_1E977:
  if (vbool_08)
    SKW_1c9a_0648(vw_2c);
  return vbool_10;
}

// Note: SKW_CHECK_RECOMPUTE_LIGHT called it with s_len8* args1 only
// extended this struct there, but first comparison seems to filter that
// M_226D1: code copied
// M_226CB: code copied
// M_233C9: code copied
// M_227E7: code copied
// M_21D48: code copied
// M_21C39: code copied
// M_22A07: code copied
// M_22D0C: code copied
// M_22CFB: code copied
// M_22A13: code copied
// M_22836: code copied
// M_22B26: code copied
// M_2283E: code copied
// M_236CC: code copied
// back == 1: to M_23A0E
// back == 2: to M_23A30
// back == 3: to M_22602
x8 SKW_1c9a_1bd8(x16 eaxw, x8 ebxb, x16 edxw, x16 argw0, c_sizex16* argp1, u_bbw* argp2)
{
  x16 wordrg5;
  t_gfxdata* gptrrg1;
  c_record* recoptrrg4;
  s_4bytearray* s4baptrrg1;
  x16* wptrrg11;
  t_gfxdata** gpptrrg12;
  s_4bytearray* s4baptrrg13;
  s_4bytearray* s4baptrrg14;
  x8* bptrrg16;
  t_gfxdata* gptrrg17;
  t_gfxdata* gptrrg18;
  s_32bytes* sptrrg21;
  s_32bytes* sptrrg4;
  t_gfxdata* gptrrg41;
  t_gfxdata* gptrrg42;
  t_gfxdata* criticalptrrg1; // TODO HERE
  c_sizex16* sx16ptrrg11;
  c_sizex16* sx16ptrrg12;
  x16 vw_04;
  x8 vba_08[0x33];
  x16 vw_44;
  s_4bytearray* p4ba_68;
  s_4bytearray* p4ba_78;
  t_gfxdata** ppg_8c;
  s_4bytearray* p4ba_90;
  x16* pw_98;
  x16 vw_a8;
  u_bbwlong vs_ac;
  //  x8 vb_ad;  PART OF v_ac! TO BE ADJUSTED!
  //  o32 vae;
  c_sizex16* psx16_b4;
  t_gfxdata* pg_bc;
  x16 vw_c8;
  x16 vw_d0;
  x16 vw_d4;
  x16 vw_d8;
  x16 vw_e4;
  x16 vw_104;
  x16 vw_108;
  x16 vw_110;
  x16 vw_114;
  x16 vw_118;
  x16 vw_11c;
  x16 vw_120;
  x16 vw_124;
  x16 vw_128;
  x16 vw_130;
  x16 vw_134;
  x8 vb_138;
  x8 vb_13c;
  x8 vb_140; // or8
  x8 vb_144;
  x8 vb_148; // or8
  x16 parw04;
  x16 parw05;
  bool flag;

	// SPX: moved variables before goto
	x16 vw_f4_1;
	x32* hlpptr2;
	x32 hlpidx2;
	x16 wordrg41;

  vw_04 = eaxw;
  vb_148 = ebxb;
  if (argp1->s_00.b_07 == con(0x0))
  {
    argp1->s_00.u_02.b.b0 &= con(0xffffffe0);
    wordrg5 = argp1->s_00.u_02.w | (vw_04 & con(0x1f));
    argp1->s_00.u_02.w = (wordrg5 & con(0xfffffc1f)) | ((edxw & con(0x1f)) << con(0x5));
    argp1->s_00.u_02.b.b1 &= con(0x3);
    argp1->s_00.u_02.w = argp1->s_00.u_04.w = argp1->s_00.u_02.w | ((ddata.v1d3248 & con(0x3f)) << con(0xa));
    argp1->s_00.b_06 = con(0x0);
    return con(0x0);
  }
  x32 vl_3c = signedlong(argw0) << con(0x2);
  p4ba_68 = UPCAST(s_4bytearray, DM2_ALLOC_LOBIGPOOL_MEMORY(vl_3c, true));
  p4ba_90 = UPCAST(s_4bytearray, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0x400)));
  x32 vl_7c = 4 * unsignedlong(CUTX8(ddata.savegamep4->w0[con(0x2)]));
  ppg_8c = UPCAST(t_gfxdata*, DM2_ALLOC_LOBIGPOOL_MEMORY(vl_7c, true));
  x32 vl_9c_1 = 2 * unsignedlong(CUTX8(ddata.savegamep4->w0[2]));
  vl_7c += vl_9c_1;
  pw_98 = UPCAST(x16, DM2_ALLOC_LOBIGPOOL_MEMORY(vl_9c_1));
  bool vbool_a0 = false;
  DM2_FILL_I16TABLE(pw_98, con(0xffffffff), unsignedlong(CUTX8(ddata.savegamep4->w0[con(0x2)])));
  x32 vl_9c = signedlong(ddata.savegamewa) << con(0x7);
  pg_bc = UPCAST(t_gfxdata, DM2_ALLOC_LOBIGPOOL_MEMORY(vl_9c));
  vw_e4 = ddata.v1d3248;
  ppg_8c[vw_e4] = pg_bc;
  vl_7c += vl_3c + vl_9c + con(0x400);
  bool vbool_48 = false;
  bool vbool_40 = false;
  bool vbool_a4 = false;
  vw_d8 = con(0xffffffff);
  vb_140 = con(0x0);
  vw_d4 = con(0x0);
  vb_138 = table1d6290[vb_148];
  bool vbool_50;
  if (vb_138 != con(0x0))
    vbool_50 = false;
  else
    vbool_50 = (table1d607e[ddata.s350.v1e0584].u.l & con(0x2000)) != con(0x0); // made that bool, checked only vs. 0
  bool vbool_60 = (vb_148 & con(0xffffff1f)) == con(0x5);
  p4ba_78 = p4ba_68;
  psx16_b4 = argp1;
  // M_21FFD:
  x16 wordrg6;
  x16 vw_70; // TODO: initialized for sure?
  x16 vw_74; // TODO: initialized for sure?
  x16 vw_94; // TODO: initialized for sure?
  x16 vw_f4;
  bool vbool_b8; // TODO: initialized for sure?
  x16 vw_c4; // TODO: initialized for sure?
  x16 vw_cc;
  x16 vw_dc;
  x16 vw_e0 = con(0x0); // TODO: ATTENTION! ORIGINAL BUG? WAS UNINITIALIZED!
  x16 vw_e8;
  x16 vw_ec;
  x16 vw_f0;
  x16 vw_f8;
  x16 vw_fc;
  x16 vw_100;
  x16 vw_10c;
  x16 vw_12c;
  for (vw_10c = con(0x0); ; p4ba_78++, psx16_b4++, vw_10c++)
  {
    if (vw_10c >= argw0)
    {
      x16 wordrg15 = ddata.s350.v1e0576 & con(0x2000);
      flag = wordrg15 == con(0x0);
      vw_94 = wordrg15;
      if (!flag)
      {
        x16 wordrg14 = ddata.s350.v1e0576 & con(0x118);
        flag = wordrg14 == con(0x0);
        vw_94 = wordrg14;
        if (!flag)
        {
          vw_74 = ddata.s350.v1e0576 & con(0x110);
          vw_70 = ddata.s350.v1e0576 & con(0x108);
        }
      }
      // M_2205D:
      if (vbool_40 || (vw_d4 != con(0x0)))
        // M_2206D:
        DM2_ZERO_MEMORY(DOWNCAST(t_gfxdata, pg_bc), vl_9c);
      // M_22078:
      vw_cc = con(0x0);
      wordrg6 = con(0x0);
      flag = (vb_148 & con(0xffffff1f)) != con(0x8);
      vbool_b8 = !flag;
      if (!flag)
      {
        wordrg5 = con(0x0);
        DM2_ZERO_MEMORY(DOWNCAST(x8, vba_08), con(0x33));
        vba_08[0x32] = con(0xffffffff);
      }
      vw_e8 = con(0x0);
      vw_dc = ddata.v1d3248;
      vw_c8 = ddata.v1d3248;
      vw_f4 = vw_04;
      vw_100 = vw_04;
      vw_12c = edxw;
      vw_ec = edxw;
      vw_130 = con(0x1);
      vw_f8 = con(0xffffffff);
      vb_144 = con(0x0);
      vw_f0 = con(0xffffffff);
      SKW_19f0_045a(vw_04, edxw);
      if (vbool_48)
      {
        ddata.v1e08b7 = SKW_GET_TELEPORTER_DETAIL(&ddata.v1e08b8, edxw, vw_04) ? 1 : 0;
        if (ddata.v1e08b7 != con(0x0))
        {
          vw_f0 = unsignedword(ddata.v1e08b8.b_04);
          vw_108 = unsignedword(ddata.v1e08b8.b_02);
          vw_128 = unsignedword(ddata.v1e08b8.b_03);
        }
      }
      break;
    }
    psx16_b4->s_00.b_06 = con(0xffffffff);
    p4ba_78->barr_00[con(0x0)] = CUTX8(psx16_b4->w_0c) & con(0x1f);
    p4ba_78->barr_00[con(0x1)] = CUTX8(ulrshift(psx16_b4->w_0c, con(0x6), con(0xb)));
    p4ba_78->barr_00[con(0x2)] = CUTX8(urshift(psx16_b4->w_0c, con(0xa)));
    x8 byterg4 = table1d62ee[psx16_b4->s_00.b_07];
    psx16_b4->b_0e[con(0x2)] = byterg4;
    vb_140 |= byterg4;
    ui8 ubyterg1 = psx16_b4->s_00.b_07;
    if (ubyterg1 < con(0xa))
    {
      if (ubyterg1 < con(0x5))
      {
        if (ubyterg1 >= con(0x1))
        {
          if (ubyterg1 <= con(0x1))
          {
            vw_114 = ddata.s350.v1e054e->uc.w & con(0x1f);
            vw_118 = ulrshift(ddata.s350.v1e054e->uc.w, con(0x6), con(0xb));
            vw_110 = urshift(ddata.s350.v1e054e->uc.w, con(0xa));
            // M_21C39:
            vbool_48 = true;
          }
          else if (ubyterg1 == con(0x3))
          {
            vw_134 = ddata.s350.v1e0556->u0c.w & con(0x1f);
            vw_d0 = ulrshift(ddata.s350.v1e0556->u0c.w, con(0x6), con(0xb));
            vw_11c = urshift(ddata.s350.v1e0556->u0c.w, con(0xa));
            if (vw_134 == ddata.v1e08d6)
              if (vw_d0 == ddata.v1e08d8)
              {
                if (vw_11c == ddata.v1e08d4)
                {
                  byterg4 ^= con(0x3);
                  psx16_b4->b_0e[con(0x2)] = byterg4;
                }
              }
            // M_21C39:
            vbool_48 = true;
          }
        }
      }
      else if (ubyterg1 <= con(0x5))
      {
        wordrg6 = psx16_b4->u_08.w;
        x16 vw_88;
        if (ddata.v1d3248 != ddata.v1e08d6)
          vw_88 = con(0x64);
        else
          vw_88 = SKW_CALC_SQUARE_DISTANCE(unsignedword(ddata.s350.v1e0562.u.b.b_06), ddata.v1e08d8, ddata.v1e08d4, unsignedword(ddata.s350.v1e0562.u.b.b_07));
        p4ba_78->barr_00[con(0x3)] = CUTX8(DM2_MAX(vw_88, wordrg6));
      }
      else
      {
        if (ubyterg1 > con(0x7))
          vbool_40 = true;
        // M_21D48:
        vw_c4 = ddata.s350.v1e0578;
      }
    }
    else if (ubyterg1 <= con(0xa))
      // M_21D48:
      vw_c4 = ddata.s350.v1e0578;
    else if (ubyterg1 < con(0x17))
    {
      if (ubyterg1 <= con(0xb))
        // M_21C39:
        vbool_48 = true;
      else if (ubyterg1 == con(0xc))
      {
        if (signedword(p4ba_78->barr_00[con(0x2)]) != ddata.v1d3248)
          vw_44 = con(0x64);
        else
        {
          x16 wordrg48 = SKW_CALC_SQUARE_DISTANCE(unsignedword(ddata.s350.v1e0562.u.b.b_06), signedword(p4ba_78->barr_00[con(0x0)]), p4ba_78->barr_00[con(0x1)], unsignedword(ddata.s350.v1e0562.u.b.b_07));
          vw_44 = DM2_MAX(con(0x1), wordrg48);
        }
        p4ba_78->barr_00[con(0x3)] = CUTX8(vw_44);
        // M_21C39:
        vbool_48 = true;
      }
    }
    else if (ubyterg1 <= con(0x17))
    {
      ddata.s350.v1e0576 = con(0x227);
      wordrg6 = con(0x0);
      ddata.v1e0974 = con(0x0);
      ddata.v1e0978 = con(0x0);
    }
    else if (ubyterg1 <= con(0x18))
      p4ba_78->barr_00[con(0x3)] = con(0x7f);
    else if (ubyterg1 == con(0x1b))
      ddata.s350.v1e0576 = con(0x36e7);
    // M_21D80:
    vw_124 = con(0x0);
    if (vb_138 == con(0x0) && (table1d62ee[psx16_b4->s_00.b_07] & con(0x20)) != con(0x0))
    {
      if (ddata.savegames1.b2 == con(0x0) || ((ddata.s350.v1e0552->u0.b.b1 & con(0x4)) != con(0x0)))
        // M_21DC6:
        vw_120 = con(0x0);
      else
        vw_120 = con(0x1);
      // M_21DC8:
      bool skip2 = false;
      if (vw_120 == con(0x0))
      {
        if (ddata.v1e0286 == con(0x0) || ddata.savegames1.w0 >= con(0x0) || ((ddata.s350.v1e0552->u0.b.b1 & con(0x8)) != con(0x0)))
          // M_21DF6:
          vw_124 = con(0x0);
        else
          vw_124 = con(0x1);
        // M_21DF8:
        if (vw_124 == con(0x0))
          skip2 = true;
      }
      if (!skip2)
      {
        // M_21E04:
        x16 wordrg32;
        x16 wordrg46;
        if (ddata.s350.v1e0571 != ddata.v1e08d6)
        {
          wordrg32 = unsignedword(ddata.v1e03c8[ddata.v1e08d6].b4[con(0x3)]) + ddata.v1e08d4;
          x16 wordrg210 = unsignedword(ddata.v1e03c8[ddata.v1e08d6].b4[con(0x2)]) + ddata.v1e08d8;
          vw_104 = (unsignedword(ddata.v1e03c8[ddata.s350.v1e0571].b4[con(0x3)]));
          x16 wordrg47 = unsignedword(ddata.s350.v1e0562.u.b.b_07) + vw_104;
          vw_104 = unsignedword(ddata.v1e03c8[ddata.s350.v1e0571].b4[con(0x2)]);
          wordrg46 = SKW_CALC_SQUARE_DISTANCE(unsignedword(ddata.s350.v1e0562.u.b.b_06) + vw_104, wordrg210, wordrg32, wordrg47);
        }
        else
        {
          wordrg32 = ddata.v1e08d4;
          wordrg46 = SKW_CALC_SQUARE_DISTANCE(unsignedword(ddata.s350.v1e0562.u.b.b_06), ddata.v1e08d8, wordrg32, unsignedword(ddata.s350.v1e0562.u.b.b_07));
        }
        vw_fc = wordrg46;
        bool doit = false;
        x16 wordrg11 = CUTX16(SKW_RAND());
        wordrg6 = wordrg11;
        wordrg11 &= con(0x8000);
        if (wordrg11 != con(0x0))
          doit = true;
        else
        {
          if (wordrg46 == con(0x0))
            // M_21F0C:
            wordrg11 = con(0x0);
          else
          {
            bool skip = false;
            wordrg11 = wordrg46;
            if (wordrg11 == con(0x1))
            {
              vw_104 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
              wordrg11 = SKW_CALC_VECTOR_DIR(unsignedword(ddata.s350.v1e0562.u.b.b_06), ddata.v1e08d8, ddata.v1e08d4, unsignedword(ddata.s350.v1e0562.u.b.b_07));
              if (wordrg11 == vw_104)
              {
                // M_21F0C: (copied)
                wordrg11 = con(0x0);
                skip = true;
              }
            }
            if (!skip)
              // M_21F05:
              wordrg11 = con(0x1);
          }
          // M_21F0E:
          if (wordrg11 != con(0x0))
            doit = true;
        }
        if (doit)
        {
          // M_21F16:
          if (vw_fc > con(0x4) || (vw_120 != con(0x0) && (((lextended(wordrg6 & con(0xf)) - (con(0x4) - vw_fc)) > lextended(ddata.s350.v1e0552->w14 & con(0xf))) || ((wordrg6 & con(0x4000)) != con(0x0)))) || (vw_124 != con(0x0) && ((signedlong(vw_fc) * signedlong(ddata.v1e0286) - ((unsignedlong(wordrg6) >> con(0x8)) & con(0x7))) > lextended(ddata.s350.v1e0552->w14 & con(0xf)))))
          {
            // M_21FA4:
            wordrg6 &= con(0x70);
            if (wordrg6 == con(0x0))
              psx16_b4->s_00.b_00 >>= con(0x3);
            else
              psx16_b4->s_00.b_00 = con(0xffffffff);
          }
        }
      }
    }
    // M_21FBE:
    vw_e8 = signedword(psx16_b4->s_00.b_00);
    if ((psx16_b4->b_0e[con(0x2)] & con(0x2)) != con(0x0))
      vw_e8++;
    vw_d4 = DM2_MAX(vw_d4, vw_e8);
  }
  // M_22602:
  for(;;)
  { 
    x16 back; // added
    x16 vw_c0 = ddata.v1d3248;
    if ((vb_140 & con(0x1)) != con(0x0))
    {
      p4ba_78 = p4ba_68;
      psx16_b4 = argp1;
      // M_2310B: co-label M_230FF
      for (vw_10c = con(0x0); vw_10c < argw0; vw_10c++, psx16_b4++, p4ba_78++)
      {
        x16 wordrg3 = vw_e8;
        if (signedword(psx16_b4->s_00.b_00) < wordrg3 || ((psx16_b4->b_0e[con(0x2)] & con(0x1)) == con(0x0)))
          continue;
        ui8 ubyterg157 = psx16_b4->s_00.b_07;
        x16 mode = 0; // jumptarget of the switch-gate, default = 0 to M_23D06, added
        if (ubyterg157 <= con(0x1b))
        {
          switch (ubyterg157)
          {
            case 0:
              // M_226D1:
              vbool_a4 = true;
            break;

            case 1:
              if (vw_c0 != vw_110 || vw_f4 != vw_114 || vw_12c != vw_118)
              {
                // M_22EF1:
                if (vw_f0 == vw_110 && vw_108 == vw_114)
                {
                  flag = vw_128 != vw_118;
                  // M_226CB:
                  if (!flag)
                    // M_226D1:
                    vbool_a4 = true;
                }
              }
              else
                // M_226D1:
                vbool_a4 = true;
            break;

            case 2:
              {
                ui16 uwordrg1 = psx16_b4->u_08.w;
                if (uwordrg1 < con(0x2))
                {
                  if (uwordrg1 == con(0x0) && ddata.v1d3248 == ddata.v1e08d6 && vw_f4 == ddata.v1e08d8)
                  {
                    flag = vw_12c != ddata.v1e08d4;
                    // M_226CB:
                    if (!flag)
                      // M_226D1:
                      vbool_a4 = true;
                  }
                }
                else if (uwordrg1 <= con(0x2))
                {
                  if (SKW_CALC_VECTOR_DIR(ddata.v1e08d8, vw_f4, vw_12c, ddata.v1e08d4) == ddata.v1e08da)
                  {
                    x16 wordrg196 = SKW_19f0_0207(ddata.v1e08d8, vw_f4, vw_12c, ddata.v1e08d4, R_1BA1B);
                    vw_fc = wordrg196;
                    if (wordrg196 > con(0x0))
                      if (wordrg196 <= psx16_b4->u_0a.w)
                        // M_226D1:
                        vbool_a4 = true;
                  }
                }
                else if (uwordrg1 == con(0x4))
                {
                  if (SKW_CALC_SQUARE_DISTANCE(vw_f4, ddata.v1e08d8, ddata.v1e08d4, vw_12c) == psx16_b4->u_0a.w)
                  {
                    bool skip = false;
                    bool boolrg19;
                    if (vw_f4 == ddata.v1e08d8 || vw_12c == ddata.v1e08d4)
                    {
                      // M_227B0:
                      if (SKW_19f0_0207(ddata.v1e08d8, vw_f4, vw_12c, ddata.v1e08d4, SKW_1c9a_1bae) != con(0x0))
                      {
                        boolrg19 = false;
                        skip = true;
                      }
                    }
                    // M_227DC:
                    if (!skip)
                      boolrg19 = true;
                    // M_227E5:
                    flag = !boolrg19;
                    // M_227E7:
                    if (!flag)
                      // M_226D1:
                      vbool_a4 = true;
                  }
                }
              }
            break;

            case 3:
              if (vw_c0 != vw_11c || vw_f4 != vw_134 || vw_12c != vw_d0)
              {
                // M_22E4A:
                if (vw_f0 == vw_11c && vw_108 == vw_134)
                {
                  flag = vw_128 != vw_d0;
                  // M_226CB:
                  if (!flag)
                    // M_226D1:
                    vbool_a4 = true;
                }
              }
              else
                // M_226D1:
                vbool_a4 = true;
            break;

            case 4:
              if (vw_c0 == ddata.v1e08d6)
              {
                flag = SKW_CALC_SQUARE_DISTANCE(vw_f4, ddata.v1e08d8, ddata.v1e08d4, vw_12c) != con(0x2);
                // M_226CB:
                if (!flag)
                  // M_226D1:
                  vbool_a4 = true;
              }
            break;

            case 5:
              if (psx16_b4->w_0c != con(0xffffffff))
                mode = 2;
              else
              {
                bool boolrg18;
                if (psx16_b4->s_00.b_06 > con(0x0))
                {
                  if ((ddata.v1d62ec & con(0x1)) == con(0x0))
                    ddata.v1d62ec >>= con(0x1);
                  else
                    ddata.v1d62ec = (ddata.v1d62ec / 2) ^ con(0xb400);
                  boolrg18 = (ddata.v1d62ec & con(0x7)) == con(0x0);
                }
                else
                  boolrg18 = true;
                if (boolrg18)
                {
                  x32 longrg111;
                  if (vw_c0 != ddata.v1e08d6)
                  {
                    vw_a8 = ((ddata.v1e03c8[vw_c0].w8 & con(0x3f)) != (ddata.v1e03c8[ddata.v1e08d6].w8 & con(0x3f))) ? 1 : 0;
                    x16 wordrg31 = unsignedword(ddata.v1e03c8[ddata.v1e08d6].b4[con(0x3)]) + ddata.v1e08d4;
                    vw_104 = unsignedword(ddata.v1e03c8[vw_c0].b4[con(0x3)]) + vw_12c;
                    longrg111 = signedlong(SKW_CALC_SQUARE_DISTANCE(unsignedword(ddata.v1e03c8[vw_c0].b4[con(0x2)]) + vw_f4, unsignedword(ddata.v1e03c8[ddata.v1e08d6].b4[con(0x2)]) + ddata.v1e08d8, wordrg31, vw_104)) << CUTX8(vw_a8); // BUGFIX 4/2014
                  }
                  else
                    longrg111 = signedlong(SKW_CALC_SQUARE_DISTANCE(vw_f4, ddata.v1e08d8, ddata.v1e08d4, vw_12c));
                  vw_fc = CUTX16(longrg111);
                  if (signedword(p4ba_78->barr_00[con(0x3)]) >= vw_fc)
                  {
                    ;
                  }
                  else if (vw_c0 != ddata.v1e08d6)
                  {
                    // M_22D0C:
                    vbool_a4 = true;
                    // M_22A13:
                    p4ba_78->barr_00[con(0x3)] = CUTX8(vw_fc);
                  }
                  else if (vw_fc >= con(0x5) || (vw_f4 != ddata.v1e08d8 && vw_12c != ddata.v1e08d4))
                    // M_22CFB:
                    mode = 3;
                  else
                  {
                    // M_22CCF:
                    if (SKW_19f0_0207(ddata.v1e08d8, vw_f4, vw_12c, ddata.v1e08d4, SKW_1c9a_1bae) != con(0x0))
                      mode = 5;
                    else
                      // M_22CFB:
                      mode = 3;
                  }
                }
              }
            break;

            case 6:
              if (vw_c0 == ddata.v1e08d6)
              {
                ddata.s350.v1e0578 &= psx16_b4->u_0a.w;
                // M_22836:
                vbool_a4 = SKW_19f0_0891(con(0x1), vw_12c, ddata.v1e08d8, vw_f4, ddata.v1e08d4, con(0xffffffff));
                // M_2283E:
                ddata.s350.v1e0578 = vw_c4;
              }
            break;

            case 7:
              if (vw_c0 == ddata.v1e08d6)
              {
                ddata.s350.v1e0578 &= psx16_b4->u_0a.w;
                parw05 = con(0xffffffff);
                parw04 = ddata.v1e08d4;
                wordrg3 = ddata.v1e08d8;
                mode = 1;
              }
            break;

            case 8:
            case 9:
              if (signedword(psx16_b4->s_00.b_00) == vw_e8)
              {
                ddata.s350.v1e0578 &= psx16_b4->u_0a.w;
                vbool_a4 = SKW_19f0_05e8(psx16_b4->u_08.w, &psx16_b4->s_00.u_02.w, vw_f4, pg_bc, vw_12c, con(0xffffffff), psx16_b4->s_00.b_07 == con(0x9));
                if (vbool_a4)
                {
                  psx16_b4->s_00.u_04.b.b0 &= con(0xffffffe0);
                  x16 wordrg31 = psx16_b4->s_00.u_04.w | (ddata.v1e08a8 & con(0x1f));
                  psx16_b4->s_00.u_04.w = wordrg31;
                  psx16_b4->s_00.u_04.w = (wordrg31 & con(0xfffffc1f)) | (ddata.v1e08aa & con(0x1f)) << con(0x5);
                  psx16_b4->s_00.u_04.b.b1 &= con(0x3);
                  psx16_b4->s_00.u_04.w |= (ddata.v1e08ac & con(0x3f)) << con(0xa);
                  x8 byterg156;
                  if (ddata.v1e08a8 != vw_f4 || ddata.v1e08aa != vw_12c || ddata.v1e08ac != vw_c0)
                  {
                    // M_2296B:
                    gptrrg17 = ppg_8c[ddata.v1e08ac];
                    gptrrg42 = gptrrg17 + (signedlong(ddata.v1e08a8) << con(0x7));
                    byterg156 = gptrrg42[4 * ddata.v1e08aa] - con(0x1);
                  }
                  else
                    byterg156 = CUTX8(vw_e8);
                  // M_22996:
                  psx16_b4->s_00.b_06 = byterg156;
                }
                // M_2283E:
                ddata.s350.v1e0578 = vw_c4;
              }
            break;

            case 10:
              if (vw_c0 == ddata.v1e08d6 && (table1d613a[ddata.s350.v1e056e] & con(0x3)) == con(0x0))
              {
                ddata.s350.v1e0578 &= psx16_b4->u_0a.w;
                parw05 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
                parw04 = con(0xffffffff);
                wordrg3 = con(0xffffffff);
                mode = 1;
              }
            break;

            case 11:
              mode = 2;
            break;

            case 12:
              if (signedword(p4ba_78->barr_00[con(0x2)]) == vw_c0)
              {
                vw_104 = signedword(p4ba_78->barr_00[con(0x3)]);
                vw_fc = SKW_CALC_SQUARE_DISTANCE(vw_f4, signedword(p4ba_78->barr_00[con(0x0)]), signedword(p4ba_78->barr_00[con(0x1)]), vw_12c);
                if (vw_104 > vw_fc)
                {
                  mode = 4;
                  break; // !!
                }
              }
              // M_22DC5:
              if (signedword(p4ba_78->barr_00[con(0x2)]) == vw_f0)
              {
                vw_104 = signedword(p4ba_78->barr_00[con(0x3)]);
                vw_fc = SKW_CALC_SQUARE_DISTANCE(vw_108, signedword(p4ba_78->barr_00[con(0x0)]), p4ba_78->barr_00[con(0x1)], vw_128);
                if (vw_fc < vw_104)
                  mode = 4;
              }
            break;

            case 13:
              if (psx16_b4->w_0c != con(0xffffffff))
                mode = 2;
              else if (wordrg3 > con(0x0))
              {
                if (psx16_b4->s_00.b_06 <= con(0x0))
                  // M_226D1:
                  vbool_a4 = true;
                else
                {
                  if ((ddata.v1d62ec & con(0x1)) == con(0x0))
                    ddata.v1d62ec >>= con(0x1);
                  else
                    ddata.v1d62ec = (ddata.v1d62ec / 2) ^ con(0xb400);
                  flag = (ddata.v1d62ec & con(0x7)) != con(0x0);
                  // M_226CB:
                  if (!flag)
                    // M_226D1:
                    vbool_a4 = true;
                }
              }
            break;

            case 14:
              {
                x16 wordrg190 = psx16_b4->u_08.w;
                vw_fc = wordrg190;
                if (wordrg190 >= con(0x0))
                {
                  if (wordrg190 > con(0x0))
                  {
                    flag = SKW_RAND16(vw_fc) != con(0x0);
                    // M_226CB:
                    if (!flag)
                      // M_226D1:
                      vbool_a4 = true;
                  }
                }
                else
                {
                  flag = SKW_RAND16(-wordrg190) == con(0x0);
                  // M_227E7:
                  if (!flag)
                    // M_226D1:
                    vbool_a4 = true;
                }
              }
            break;

            case 15:
            case 16:
              // M_22B26:
              vbool_a4 = SKW_19f0_2165(con(0x0), vw_12c, vw_f4, vw_f4, vw_12c, con(0xffffffff), psx16_b4->u_08.w);
            break;

            case 17:
            case 18:
            case 19:
            case 22:
            case 26:
            break;

            case 20:
              // M_22F8F:
              // M_22F98:
              if (vw_f4 != vw_04 || vw_12c != edxw)
              {
                x16 wordrg117 = SKW_19f0_050f();
                if (wordrg117 != con(0xfffffffe))
                  if (SKW_1c9a_0006(wordrg117, psx16_b4->u_08.w) != con(0x0))
                  {
                    SKW_1c9a_19d4(wordrg117, vw_12c, psx16_b4->u_0a.w, vw_f4);
                    // M_226D1:
                    vbool_a4 = true;
                  }
              }
            break;

            case 21:
              // M_22B26:
              vbool_a4 = SKW_CREATURE_GO_THERE(psx16_b4->u_08.b.b0, vw_12c, vw_f4, vw_f4, vw_12c, con(0xffffffff));
            break;

            case 23:
              if ((ddata.v1e08ae & con(0x10)) != con(0x0))
              {
                x16 wordrg184 = vw_f8;
                if (wordrg184 == con(0xffffffff))
                  wordrg184 = ddata.v1e0258;
                SKW_ADD_BACKGROUND_LIGHT_FROM_TILE(vw_e8, vw_f4, vw_12c, wordrg184, con(0x3));
              }
            break;

            case 24:
              if ((ddata.v1e08ae & con(0x10)) != con(0x0))
              {
                x16 wordrg157 = SKW_1c9a_1b16(psx16_b4->u_08.w, psx16_b4->u_0a.w);
                vw_fc = wordrg157;
                if (wordrg157 != con(0xffffffff))
                {
                  if (wordrg157 < signedword(p4ba_78->barr_00[con(0x3)]))
                  {
                    if (wordrg157 == con(0x0) || signedword(p4ba_78->barr_00[con(0x3)]) < con(0x7f) || (psx16_b4->s_00.b_00 == con(0x0)))
                    {
                      // M_22A00:
                      vbool_a4 = true;
                      // M_22A07:
                      psx16_b4->b_0e[con(0x2)] &= con(0xfffffffb);
                    }
                    else
                      p4ba_78->barr_00[con(0x3)] = CUTX8(vw_fc);
                  }
                }
              }
            break;

            case 25:
              if (vw_f8 != con(0xffffffff))
              {
                if ((ddata.v1e08ae & con(0x10)) != con(0x0))
                {
                  x16 wordrg184 = SKW_1c9a_1a48(con(0x1), ddata.s350.v1e0582);
                  if (wordrg184 != con(0xffffffff))
                  {
                    x32 wordrg421 = con(0x1) << ((vw_f8 + con(0x2)) & con(0x3));
                    flag = (wordrg184 & wordrg421) != con(0x0);
                    // M_226CB:
                    if (!flag)
                      // M_226D1:
                      vbool_a4 = true;
                  }
                }
              }
            break;

            case 27:
              {
                x8 byterg41 = CUTX8(vw_e8) + con(0x1);
                if (vw_c0 != ddata.v1d62a4)
                {
                  if (vw_f0 >= con(0x0) && vw_f0 == ddata.v1d62a4)
                  {
                    sptrrg21 = &ddata.v1e08cc[vw_108];
                    sptrrg21->barr_00[vw_128] = byterg41;
                  }
                }
                else
                {
                  sptrrg21 = &ddata.v1e08cc[vw_f4];
                  sptrrg21->barr_00[vw_12c] = byterg41;
                }
                // M_23086:
                x8 byterg42 = CUTX8(vw_e8) + con(0x1);
                if (vw_c0 != ddata.v1d62a6)
                {
                  if (vw_f0 >= con(0x0) && vw_f0 == ddata.v1d62a6)
                  {
                    sptrrg21 = &ddata.v1e08c8[vw_108];
                    sptrrg21->barr_00[vw_128] = byterg42;
                  }
                }
                else
                {
                  sptrrg21 = &ddata.v1e08c8[vw_f4];
                  sptrrg21->barr_00[vw_12c] = byterg42;
                }
              }
            break;

            default: throw(THROW_DMABORT);
          }
        }
        switch (mode)
        {
          case 1:
            // M_22828:
            // M_22836:
            vbool_a4 = SKW_19f0_0891(con(0x0), vw_12c, wordrg3, vw_f4, parw04, parw05);
            // M_2283E:
            ddata.s350.v1e0578 = vw_c4;
          break;

          case 2:
            // M_22D18:
            if (signedword(p4ba_78->barr_00[con(0x2)]) != vw_c0 || signedword(p4ba_78->barr_00[con(0x0)]) != vw_f4 || signedword(p4ba_78->barr_00[con(0x1)]) != vw_12c)
            {
              // M_22D45:
              if (signedword(p4ba_78->barr_00[con(0x2)]) == vw_f0 && signedword(p4ba_78->barr_00[con(0x0)]) == vw_108)
              {
                flag = signedword(p4ba_78->barr_00[con(0x1)]) != vw_128;
                // M_226CB:
                if (!flag)
                  // M_226D1:
                  vbool_a4 = true;
              }
            }
            else
              // M_226D1:
              vbool_a4 = true;
          break;

          case 3: // see also mode 5
            // M_22D04:
            // M_22D0C:
            vbool_a4 = true;
            // M_22A13:
            p4ba_78->barr_00[con(0x3)] = CUTX8(vw_fc);
          break;

          case 4:
            // M_22E0E:
            vbool_a4 = true;
            p4ba_78->barr_00[con(0x3)] = CUTX8(vw_fc);
            if (CUTX8(vw_fc) == con(0x0))
              // M_22A07:
              psx16_b4->b_0e[con(0x2)] &= con(0xfffffffb);
          break;

          case 5: // is mode 3 with flag not set
          break;
        }
        // M_230D6:
        if (!vbool_a4)
          continue;
        if (!vbool_50 || ddata.v1d3248 == vw_c8 || ddata.v1d3248 == ddata.v1e08d6)
          break;
        vbool_a4 = false;
      }
    }
    // M_2311B:
    x16 wordrg45 = vw_e8 + ddata.v1e08c4;
    if (wordrg45 <= vw_d4)
    {
      s4baptrrg1 = &p4ba_90[wordrg6];
      s4baptrrg1->barr_00[con(0x0)] = CUTX8(vw_f4);
      s4baptrrg1->barr_00[con(0x1)] = CUTX8(vw_12c);
      s4baptrrg1->barr_00[con(0x2)] = CUTX8(vw_c0);
      wordrg6++;
      wordrg6 = unsignedword(CUTX8(wordrg6));
      vba_08[wordrg45]++;
    }
    vs_ac.u.b_00 = CUTX8(wordrg45);
    vs_ac.u.b_01 = CUTX8(vw_f8);
    vs_ac.u.w_02 &= con(0x3ff);
    vs_ac.u.w_02 |= (vw_dc & con(0x3f)) << con(0xa);
    vs_ac.u.w_02 &= con(0xffe0);
    x16 wordrg29 = vs_ac.u.w_02 | (vw_100 & con(0x1f));
    vs_ac.u.w_02 = wordrg29;
    vs_ac.u.w_02 = wordrg29 & con(0xfffffc1f);
    vs_ac.u.w_02 |= (vw_ec & con(0x1f)) << con(0x5);
    x32* hlpptr = UPCAST(x32, pg_bc);
    x32 hlpidx = ((signedlong(vw_f4) << con(0x7)) + 4 * signedlong(vw_12c)) / 4;
    hlpptr[hlpidx] = vs_ac.l_00;
    // M_23A30:
    for(;;)
    {
      back = 0;
      if (vbool_a4)
      {
        if ((psx16_b4->b_0e[con(0x2)] & con(0x10)) == con(0x0))
        {
          x16 wordrg176;
          if ((psx16_b4->b_0e[con(0x2)] & con(0x20)) == con(0x0))
          {
            psx16_b4->s_00.u_02.b.b0 &= con(0xffffffe0);
            x16 wordrg28 = psx16_b4->s_00.u_02.w | (vw_f4 & con(0x1f));
            psx16_b4->s_00.u_02.w = (wordrg28 & con(0xfffffc1f)) | ((vw_12c & con(0x1f)) << con(0x5));
            wordrg176 = vw_c0;
          }
          else
          {
            psx16_b4->s_00.u_02.b.b0 &= con(0xffffffe0);
            x16 wordrg27 = psx16_b4->s_00.u_02.w | (ddata.v1e08d8 & con(0x1f));
            psx16_b4->s_00.u_02.w = wordrg27;
            psx16_b4->s_00.u_02.w = (wordrg27 & con(0xfffffc1f)) | ((ddata.v1e08d4 & con(0x1f)) << con(0x5));
            wordrg176 = ddata.v1e08d6;
          }
          wordrg176 &= con(0x3f);
          wordrg176 <<= con(0xa);
          psx16_b4->s_00.u_02.b.b1 &= con(0x3);
          psx16_b4->s_00.u_02.w |= wordrg176;
          if (vb_144 == con(0x0))
            psx16_b4->s_00.b_06 = CUTX8(vw_e8);
          else
          {
            psx16_b4->s_00.b_06 = CUTX8(vw_e8) - con(0x1);
            vw_f4 = vw_100;
            vw_12c = vw_ec;
            vw_c0 = vw_dc;
          }
          psx16_b4->s_00.u_04.b.b0 &= con(0xffffffe0);
          x16 wordrg26 = psx16_b4->s_00.u_04.w | (vw_f4 & con(0x1f));
          psx16_b4->s_00.u_04.w = (wordrg26 & con(0xfffffc1f)) | ((vw_12c & con(0x1f)) << con(0x5));
          psx16_b4->s_00.u_04.b.b1 &= con(0x3);
          psx16_b4->s_00.u_04.w |= (vw_c0 & con(0x3f)) << con(0xa);
        }
        vw_d8 = vw_10c;
        if (vw_10c == con(0x0) && ((psx16_b4->b_0e[con(0x2)] & con(0x4)) == con(0x0) || signedword(psx16_b4->s_00.b_00) <= vw_e8))
          goto M_endpath;
        // M_23B7D:
        argw0 = vw_10c + con(0x1);
        for(;;)
        {
          x16 wordrg25 = vw_10c - con(0x1);
          vw_10c = wordrg25;
          if (argp1[vw_10c].s_00.b_01 >= con(0x0))
          {
            if ((psx16_b4->b_0e[con(0x2)] & con(0x4)) == con(0x0))
              argw0 = wordrg25 + con(0x1);
            if ((psx16_b4->b_0e[con(0x2)] & con(0x40)) == con(0x0))
            {
              x16 wordrg44 = con(0x0);
              x16 wordrg3;
              do
              {
                x16 wordrg143 = signedword(argp1[vw_10c].s_00.b_01);
                vw_f4 = wordrg143;
                if (wordrg143 > con(0x0))
                  wordrg44 += wordrg143;
                sx16ptrrg11 = &argp1[vw_10c];
                wordrg25 = wordrg44 + vw_e8;
                if (wordrg25 < signedword(sx16ptrrg11->s_00.b_00))
                  sx16ptrrg11->s_00.b_00 = CUTX8(vw_e8) + CUTX8(wordrg44);
                wordrg3 = vw_10c - con(0x1);
                vw_10c = wordrg3;
              } while (wordrg3 >= con(0x0));
            }
            break;
          }
          if (wordrg25 == con(0x0))
            goto M_endpath;
        }
        vb_140 = con(0x0);
        vw_d4 = con(0x0);
        
        // M_23CEF:
        for (vw_10c = con(0x0); vw_10c < argw0; vw_10c++)
        {
          sx16ptrrg12 = &argp1[vw_10c];
          vw_fc = unsignedword(sx16ptrrg12->b_0e[con(0x2)]);
          vb_140 |= CUTX8(vw_fc);
          if (vw_10c == con(0x0) || (sx16ptrrg12->s_00.b_06 < con(0x0)))
          {
            // M_23CAA:
            x16 wordrg43 = signedword(argp1[vw_10c].s_00.b_00);
            if ((vw_fc & con(0x2)) != con(0x0))
              wordrg43++;
            x16 wordrg24 = wordrg43;
            vw_d4 = DM2_MAX(wordrg24, vw_d4);
          }
          // M_23CEB:
        }
        vbool_a4 = false;
      }
      // M_23D04:
      for(;;)
      {
        vw_f8 = (vw_f8 + vw_e0) & con(0x3);
        // M_23D12:
        for(;;)
        {
          back = 0;
          x16 wordrg23 = vw_130;
          flag = --wordrg23 == con(0x0);
          vw_130 = wordrg23;
          if (!flag)
          {
            if ((vw_cc != ((wordrg6 + con(0x1)) & con(0xff))))
            {
              x16 wordrg188 = vw_dc;
              if (wordrg188 != ddata.v1d3248)
              {
                SKW_1c9a_0648(vw_dc);
                pg_bc = ppg_8c[vw_dc];
              }
              x16 opt; // pathselector
              if (vw_94 == con(0x0) || (wordrg188 = vw_130, wordrg188 > con(0x2)))
              {
                bool doit = false;
                // M_22343:
                vw_f4 = vw_100 + table1d27fc[vw_f8];
                vw_12c = vw_ec + table1d2804[vw_f8];
                bool boolrg153;
                if (vw_f4 < con(0x0) || vw_f4 >= ddata.savegamewa)
                  // M_2237C:
                  boolrg153 = true;
                else
                  boolrg153 = false;
                // M_22385:
                if (boolrg153)
                  doit = true;
                else
                {
                  if (vw_12c < con(0x0) || vw_12c >= ddata.savegamewb)
                    // M_2239E:
                    boolrg153 = true;
                  // M_223A3:
                  if (boolrg153)
                    doit = true;
                }
                if (doit)
                  opt = 0;
                else
                  opt = 2;
              }
              else if (wordrg188 != con(0x2))
              {
                if (vw_74 == con(0x0))
                  opt = 0;
                else
                {
                  vw_f8 = con(0x4);
                  opt = 1;
                }
              }
              else if (vw_70 == con(0x0))
                opt = 0;
              else
              {
                vw_f8 = con(0x5);
                opt = 1;
              }
              switch (opt)
              {
                case 0:
                break;

                case 1:
                  // M_22333:
                  vw_f4 = vw_100;
                  vw_12c = vw_ec;
                case 2: // no break before
                  // M_223AB:
                  ddata.v1e08b6 = con(0x0);
                  vb_144 = pg_bc[(vw_f4 << con(0x7)) + 4 * vw_12c];
                  if (vb_144 == con(0x0) || vw_f8 >= con(0x4))
                  {
                    // M_223E0:
                    x16 wordrg199;
                    if (SKW_CREATURE_GO_THERE(vb_148, vw_ec, vw_f4, vw_100, vw_12c, vw_f8))
                    {
                      vw_f0 = con(0xffffffff);
                      bool skip = false;
                      wordrg199 = ddata.v1e08ac;
                      if (wordrg199 == ddata.v1d3248)
                      {
                        if (vw_f8 < con(0x4))
                        {
                          if (ddata.v1e08b7 == con(0x0))
                          {
                            if (wordrg199 = ddata.v1e08a8, wordrg199 != vw_f4 || (wordrg199 = ddata.v1e08aa, wordrg199 != vw_12c))
                            {
                              // M_225A9:
                              wordrg199 = ddata.v1d3248;
                              vw_f0 = wordrg199;
                            }
                          }
                          else
                          {
                            vw_f0 = unsignedword(ddata.v1e08b8.b_04);
                            vw_108 = unsignedword(ddata.v1e08b8.b_02);
                            wordrg199 = unsignedword(ddata.v1e08b8.b_03);
                            skip = true;
                          }
                        }
                      }
                      else
                      {
                        if (ppg_8c[ddata.v1e08ac] == NULL)
                        {
                          x32 longrg101 = unsignedlong(ulrshift(ddata.v1e03c8[ddata.v1e08ac].w8, con(0x5), con(0xb))) << con(0x7);
                          vl_9c = longrg101 + con(0x80);
                          longrg101 += con(0x148);
                          if (longrg101 <= dm2_allochandler.bigpool_available)
                          {
                            pg_bc = UPCAST(t_gfxdata, DM2_ALLOC_LOBIGPOOL_MEMORY(vl_9c, true));
                            vl_7c += vl_9c;
                          }
                          else
                          {
                            x16 wordrg3 = SKW_3e74_5888();
                            SKW_ALLOC_CPXHEAP_MEM(wordrg3, vl_9c);
                            for (vw_fc = con(0x0); unsignedword(CUTX8(ddata.savegamep4->w0[con(0x2)])) > vw_fc; vw_fc++)
                            {
                              x16 wordrg22 = pw_98[vw_fc];
                              if (wordrg22 >= con(0x0))
                                ppg_8c[vw_fc] = SKW_3e74_5817(wordrg22);
                            }
                            pw_98[ddata.v1e08ac] = wordrg3;
                            vbool_a0 = true;
                            pg_bc = SKW_3e74_5817(wordrg3);
                            DM2_ZERO_MEMORY(DOWNCAST(t_gfxdata, pg_bc), vl_9c);
                          }
                          gpptrrg12 = &ppg_8c[ddata.v1e08ac];
                          *gpptrrg12 = pg_bc;
                        }
                        wordrg199 = vw_f8;
                        if (wordrg199 < con(0x4))
                        {
                          wordrg199 = ddata.v1d3248;
                          vw_f0 = wordrg199;
                        }
                        SKW_1c9a_0648(ddata.v1e08ac);
                        pg_bc = ppg_8c[ddata.v1d3248];
                      }
                      if (!skip)
                      {
                        // M_225B2:
                        vw_108 = vw_f4;
                        wordrg199 = vw_12c;
                      }
                      // M_225BC:
                      vw_128 = wordrg199;
                      vb_144 = con(0x0);
                    }
                    else
                      vb_144 = con(0xffffffff);
                  }
                  // M_225C5:
                  if (vb_144 != con(0x0))
                  {
                    if (vb_144 != con(0xffffffff) || vw_f8 >= con(0x4))
                      back = 2;
                    else
                    {
                      vb_13c = ddata.v1e08b6;
                      vb_144 = CUTX8(vw_e8);
                      if ((vb_140 & con(0x2)) == con(0x0))
                        back = 1;
                      else
                      {
                        if ((vb_140 & con(0x8)) != con(0x0))
                          SKW_19f0_045a(vw_f4, vw_12c);
                        psx16_b4 = argp1;
                        // M_239FE: // co-label M_239F6
                        for (vw_10c = con(0x0); ; vw_10c++, psx16_b4++)
                        {
                          bool dospec = false;
                          bool doctrl = false;
                          bool boolrg1;
                          if (vw_10c >= argw0)
                          {
                            back = 1;
                            break;
                          }
                          x16 vw_58 = vw_e8;
                          if ((psx16_b4->s_00.b_00 + con(0x1)) < vw_e8 || (psx16_b4->b_0e[con(0x2)] & con(0x2)) == con(0x0))
                            continue;
                          ui8 uwordrg100 = psx16_b4->s_00.b_07;
                          if (uwordrg100 < con(0x12))
                          {
                            bool doit = false;
                            if (uwordrg100 < con(0x8))
                            {
                              if (uwordrg100 >= con(0x2))
                              {
                                ;
                              }
                              else if (uwordrg100 <= con(0x2))
                              {
                                if (ddata.v1d3248 != ddata.v1e08d6)
                                {
                                  ;
                                }
                                else if (vw_f4 != ddata.v1e08d8)
                                {
                                  ;
                                }
                                else if (vw_12c != ddata.v1e08d4)
                                {
                                  ;
                                }
                                else
                                {
                                  ui16 uwordrg1 = psx16_b4->u_08.w;
                                  if (uwordrg1 < con(0x1))
                                  {
                                    ;
                                  }
                                  else if (uwordrg1 <= con(0x1))
                                  {
                                    bool skip = false;
                                    if (psx16_b4->u_0a.w != con(0x0))
                                    {
                                      x16 wordrg181 = con(0x1) << (SKW_CALC_VECTOR_DIR(ddata.v1e08d8, vw_100, vw_ec, ddata.v1e08d4) - ddata.v1e08da) & con(0x3);
                                      if ((wordrg181 & psx16_b4->u_0a.w) == con(0x0))
                                      {
                                        // M_236CC:
                                        vb_144 = con(0xffffffff);
                                        skip = true;
                                      }
                                    }
                                    if (!skip)
                                      // M_233C9:
                                      vbool_a4 = true;
                                  }
                                  else if (uwordrg1 == con(0x3))
                                  {
                                    x16 wordrg115 = unsignedword(SKW_GET_TILE_VALUE(vw_f4 + table1d27fc[vw_f8], vw_12c + table1d2804[vw_f8]));
                                    if ((urshift(wordrg115, con(0x5)) & con(0xff)) != con(0x2) || (wordrg115 & con(0x8)) == con(0x0) || (wordrg115 & con(0x1)) != con(0x0)) // BUGFIX 4/2014
                                      // M_236CC:
                                      vb_144 = con(0xffffffff);
                                    else
                                      // M_233C9:
                                      vbool_a4 = true;
                                  }
                                }
                              }
                              else if (uwordrg100 == con(0x3))
                              {
                                bool boolrg16;
                                if (ddata.v1d3248 != ddata.v1e08d6 || vw_f4 != ddata.v1e08d8 || vw_12c != ddata.v1e08d4)
                                  // M_23318:
                                  boolrg16 = false;
                                else
                                  boolrg16 = true;
                                // M_2331A:
                                vbool_a4 = boolrg16;
                              }
                            }
                            else if (uwordrg100 <= con(0x8))
                            {
                              if (CUTX8(urshift(ddata.v1e08ae, con(0x5))) != con(0x0))
                                doit = true;
                            }
                            else if (uwordrg100 < con(0xf))
                            {
                              if (uwordrg100 == con(0x9))
                                doit = true;
                            }
                            else if (uwordrg100 <= con(0xf) || uwordrg100 == con(0x11))
                            {
                              // M_23533:
                              bool boolrg14 = SKW_19f0_2165(con(0x0), vw_ec, vw_f4, vw_100, vw_12c, vw_f8, psx16_b4->u_08.w);
                              vbool_a4 = boolrg14;
                              if (!boolrg14 && ddata.v1e08be != con(0x0))
                                // M_236CC:
                                vb_144 = con(0xffffffff);
                            }
                            if (doit)
                            {
                              // M_23453:
                              ddata.s350.v1e0578 &= psx16_b4->u_0a.w;
                              bool boolrg13 = SKW_19f0_05e8(psx16_b4->u_08.w, &psx16_b4->s_00.u_02.w, vw_100, pg_bc, vw_ec, vw_f8, psx16_b4->s_00.b_07 == con(0x9));
                              vbool_a4 = boolrg13;
                              if (boolrg13)
                              {
                                psx16_b4->s_00.u_04.b.b0 &= con(0xffffffe0);
                                x16 wordrg21 = psx16_b4->s_00.u_04.w | (ddata.v1e08a8 & con(0x1f));
                                psx16_b4->s_00.u_04.w = wordrg21;
                                psx16_b4->s_00.u_04.w = (wordrg21 & con(0xfffffc1f)) | ((ddata.v1e08aa & con(0x1f)) << con(0x5));
                                psx16_b4->s_00.u_04.b.b1 &= con(0x3);
                                psx16_b4->s_00.u_04.w |= ((ddata.v1e08ac & con(0x3f)) << con(0xa));
                                gptrrg18 = ppg_8c[ddata.v1e08ac];
                                gptrrg41 = gptrrg18 + (signedlong(ddata.v1e08a8) << con(0x7));
                                psx16_b4->s_00.b_06 = gptrrg41[4 * ddata.v1e08aa] - con(0x1);
                              }
                              ddata.s350.v1e0578 = vw_c4;
                            }
                          }
                          else if (uwordrg100 <= con(0x12))
                            dospec = true;
                          else if (uwordrg100 < con(0x16))
                          {
                            if (uwordrg100 <= con(0x13))
                            {
                              if (ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe)) == vw_f8)
                                dospec = true;
                            }
                            else if (uwordrg100 == con(0x14))
                            {
                              x16 wordrg112 = SKW_19f0_050f();
                              if (wordrg112 != con(0xfffffffe))
                              {
                                if (SKW_1c9a_0006(wordrg112, psx16_b4->u_08.w) != con(0x0))
                                {
                                  SKW_1c9a_19d4(wordrg112, vw_12c, psx16_b4->u_0a.w, vw_f4);
                                  // M_233C9:
                                  vbool_a4 = true;
                                }
                              }
                            }
                          }
                          else if (uwordrg100 <= con(0x16))
                          {
                            if ((ddata.v1e08ae & con(0x10)) != con(0x0))
                            {
                              boolrg1 = SKW_19f0_2813(psx16_b4->u_0a.b.b0, vw_ec, vw_f4, vw_100, vw_12c, vw_f8, psx16_b4->u_08.w);
                              doctrl = true;
                            }
                          }
                          else if (uwordrg100 < con(0x1a))
                          {
                            if (uwordrg100 == con(0x17) && (ddata.v1e08ae & con(0x10)) != con(0x0))
                            {
                              SKW_ADD_BACKGROUND_LIGHT_FROM_TILE(vw_58, vw_f4, vw_12c, vw_f8, con(0x4));
                              // M_236CC:
                              vb_144 = con(0xffffffff);
                            }
                          }
                          else if (uwordrg100 <= con(0x1a))
                          {
                            bool doit = false;
                            if (psx16_b4->u_08.w != con(0x1))
                              doit = true;
                            else
                            {
                              // M_237DB: and M_237E4:
                              if (ddata.v1d3248 == ddata.v1e08d6 && vw_f4 == ddata.v1e08d8 && vw_12c == ddata.v1e08d4)
                                doit = true;
                            }
                            if (doit)
                            {
                              // M_237EC:
                              if (SKW_19f0_0d10(psx16_b4->u_0a.b.b0, vw_ec, vw_f4, vw_100, vw_12c, vw_f8) && (psx16_b4->u_08.w != con(0x2) || SKW_CALC_SQUARE_DISTANCE(vw_f4, ddata.v1e08d8, ddata.v1e08d4, vw_12c) == con(0x1)))
                              {
                                // M_2385B:
                                SKW_19f0_045a(vw_f4, vw_12c);
                                if (psx16_b4->u_0a.w != con(0x0) || ((ddata.v1e08ae & con(0x7)) != con(0x0)))
                                {
                                  // M_23883:
                                  if (psx16_b4->u_0a.w != con(0x1) || (CUTX8(ddata.v1e08ae) & con(0x7)) != con(0x4))
                                  {
                                    // M_238A2:
                                    if (ddata.v1e08b0 == con(0xffffffff))
                                      ddata.v1e08b0 = SKW_GET_TILE_RECORD_LINK(vw_f4, vw_12c);
                                    recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(ddata.v1e08b0);
                                    // from middle M_238F1:
                                    if ((recoptrrg4->u2.b.b1 & con(0x4)) == con(0x0) || (((recoptrrg4->u2.b.b1 & con(0x2)) == con(0x0) || psx16_b4->u_0a.w != con(0x0)) && ((recoptrrg4->u2.b.b1 & con(0x2)) != con(0x0) || psx16_b4->u_0a.w != con(0x1))))
                                      // M_233C9:
                                      vbool_a4 = true;
                                  }
                                }
                              }
                            }
                          }
                          else if (uwordrg100 != con(0x1b))
                          {
                            ;
                          }
                          else
                          {
                            if (vb_13c == con(0x0))
                              vw_fc = vw_e8 + con(0x1);
                            else
                              vw_fc = (con(0x1) << CUTX8(vw_f8)) | con(0x80);
                            if (vw_c0 != ddata.v1d62a4)
                            {
                              if (vw_f0 >= con(0x0) && vw_f0 == ddata.v1d62a4)
                              {
                                bptrrg16 = &ddata.v1e08cc[vw_108].barr_00[vw_128];
                                *bptrrg16 |= CUTX8(vw_fc);
                              }
                            }
                            else
                            {
                              bptrrg16 = &ddata.v1e08cc[vw_f4].barr_00[vw_12c];
                              *bptrrg16 |= CUTX8(vw_fc);
                            }
                            // M_2397D:
                            if (vw_c0 != ddata.v1d62a6)
                            {
                              if (vw_f0 >= con(0x0) && vw_f0 == ddata.v1d62a6)
                              {
                                sptrrg4 = &ddata.v1e08c8[vw_108];
                                sptrrg4->barr_00[vw_128] |= CUTX8(vw_fc);
                              }
                            }
                            else
                            {
                              sptrrg4 = &ddata.v1e08c8[vw_f4];
                              sptrrg4->barr_00[vw_12c] |= CUTX8(vw_fc);
                            }
                          }
                          if (dospec)
                          {
                            // M_2359B:
                            x16 wordrg111 = SKW_19f0_050f();
                            if (wordrg111 != con(0xfffffffe))
                            {
                              recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(wordrg111);
                              if (unsignedword(recoptrrg4->u4.b.b0) == psx16_b4->u_08.w)
                              {
                                flag = psx16_b4->u_0a.w == con(0x1);
                                vbool_a4 = flag;
                                if (!flag)
                                {
                                  if (psx16_b4->u_0a.w != con(0x2))
                                  {
                                    vw_120 = vw_f4 + table1d27fc[vw_f8];
                                    vw_124 = vw_12c + table1d2804[vw_f8];
                                    if (ddata.v1d3248 != ddata.v1e08d6 || vw_120 != ddata.v1e08d8 || vw_124 != ddata.v1e08d4)
                                    {
                                      // M_2368A:
                                      x16 wordrg10 = SKW_GET_CREATURE_AT(vw_120, vw_124);
                                      if (wordrg10 == con(0xffffffff) || (SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg10) & con(0x1)) != con(0x0))
                                        // M_236BF:
                                        boolrg1 = false;
                                      else
                                        // M_236B8:
                                        boolrg1 = true;
                                    }
                                    else
                                      // M_236B8:
                                      boolrg1 = true;
                                    doctrl = true;
                                  }
                                  else
                                  {
                                    x16 wordrg19 = recoptrrg4->u2.w;
                                    // M_235FA: // co-label M_23631:
                                    for (;; wordrg19 = SKW_GET_NEXT_RECORD_LINK(wordrg19))
                                    {
                                      if (wordrg19 == con(0xfffffffe))
                                        break;
                                      if (urshift(wordrg19 & con(0x3c00), con(0xa)) != con(0xe))
                                      {
                                        if (urshift(wordrg19, con(0xe)) != vw_f8)
                                        {
                                          // M_233C9:
                                          vbool_a4 = true;
                                          break;
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                          if (doctrl)
                          {
                            // M_236C1:
                            vbool_a4 = boolrg1;
                            if (!boolrg1)
                            {
                              // M_236CC:
                              vb_144 = con(0xffffffff);
                            }
                          }
                          // M_239CD:
                          if (vbool_a4)
                          {
                            if (!vbool_50 || ddata.v1d3248 == vw_c8 || ddata.v1d3248 == ddata.v1e08d6)
                            {
                              back = 1;
                              break;
                            }
                            vbool_a4 = false;
                          }
                        }
                      }
                    }
                  }
                  else
                  {
                    vw_f4 = ddata.v1e08a8;
                    vw_12c = ddata.v1e08aa;
                    if (pg_bc[(signedlong(vw_f4) << con(0x7)) + 4 * signedlong(vw_12c)] == con(0x0))
                      back = 3;
                  }
                break;
              }
              break; // loopout to M_23D04 and backwards
            }
          }
          else
          {
            // M_23D3C:
            if (wordrg6 == vw_cc)
              goto M_preendpath;
            // M_2215D:
            for(;;)
            {
              s4baptrrg14 = &p4ba_90[vw_cc];
              vw_100 = unsignedword(s4baptrrg14->barr_00[con(0x0)]);
              vw_ec = unsignedword(s4baptrrg14->barr_00[con(0x1)]);
              vw_dc = unsignedword(s4baptrrg14->barr_00[con(0x2)]);
              vw_cc = unsignedword(CUTX8(vw_cc + con(0x1)));
              if (vw_dc != ddata.v1d3248)
              {
                SKW_1c9a_0648(vw_dc);
                pg_bc = ppg_8c[vw_dc];
              }
              vw_e8 = unsignedword(pg_bc[(signedlong(vw_100) << con(0x7)) + 4 * signedlong(vw_ec)]);
              bool doit1 = false;
              if (!vbool_b8)
                doit1 = true;
              else
              {
                for(;;)
                {
                  x16 wordrg42 = wordrg5 + con(0x1);
                  if (wordrg5 >= vw_e8)
                  {
                    flag = --vba_08[wordrg5] != con(0x0);
                    if (!flag)
                      wordrg5 = wordrg42;
                    doit1 = true;
                    break;
                  }
                  if (vba_08[wordrg5] != con(0x0))
                    break;
                  wordrg5 = wordrg42;
                }
              }
              if (doit1)
              {
                // M_2222D:
                if (vw_e8 > vw_d4)
                  goto M_preendpath;
                if (vw_e8 > con(0x6))
                  vb_148 |= con(0x40);
                if (vbool_60 && vw_e8 > con(0x1))
                {
                  vb_148 = (vb_148 & con(0xffffffe0)) | con(0x4);
                  vbool_60 = false;
                }
                x16 wordrg18 = ddata.v1d62ec & con(0x1);
                vw_e0 = wordrg18;
                if (wordrg18 == con(0x0))
                  vw_e0 = wordrg18 - con(0x1);
                if ((ddata.v1d62ec & con(0x1)) == con(0x0))
                  ddata.v1d62ec >>= con(0x1);
                else
                  ddata.v1d62ec = (ddata.v1d62ec / 2) ^ con(0xb400);
                vw_f8 = ddata.v1d62ec & con(0x3);
                if (vw_94 == con(0x0))
                  vw_130 = con(0x5);
                else
                  vw_130 = con(0x7);
                break;
              }
              s4baptrrg13 = &p4ba_90[wordrg6];
              s4baptrrg13->barr_00[con(0x0)] = CUTX8(vw_100);
              s4baptrrg13->barr_00[con(0x1)] = CUTX8(vw_ec);
              s4baptrrg13->barr_00[con(0x2)] = CUTX8(vw_dc);
              wordrg6++;
              wordrg6 = unsignedword(CUTX8(wordrg6));
            }
          }
          if (back > 0)
            break;
        }
        if (back > 0)
          break;
      }
      if (back == 1)
      {
        // M_23A0E:
        if (vb_13c == con(0x0))
        {
          gptrrg1 = &pg_bc[(signedlong(vw_f4) << con(0x7))];
          gptrrg1 += vw_12c << con(0x2);
          *gptrrg1 = vb_144;
        }
      }
      if (back == 3)
        break;
    }
  }
  // deadend at this point

M_preendpath: // M_23D46:
  if (vw_d8 == con(0xffffffff))
    goto M_preexit;
M_endpath: // M_23D51:
  psx16_b4 = argp1 + vw_d8;
  vw_100 = psx16_b4->s_00.u_04.w & con(0x1f);
  vw_ec = ulrshift(psx16_b4->s_00.u_04.w, con(0x6), con(0xb));
  vw_dc = urshift(psx16_b4->s_00.u_04.w, con(0xa));
  vw_e8 = signedword(psx16_b4->s_00.b_06);
  for(;;)
  {
    if (--vw_e8 < con(0x0))
      break;
// TODO versionP0 had problems here - solved?
if (vw_dc < con(0x0))
  int stop=1;
   criticalptrrg1 = ppg_8c[vw_dc];
if (criticalptrrg1 == NULL) // that happens!!!
  int stop=1;
     hlpptr2 = UPCAST(x32, criticalptrrg1);
     hlpidx2 = ((signedlong(vw_100) << con(0x7)) + 4 * signedlong(vw_ec)) / 4;
    vs_ac.l_00 = hlpptr2[hlpidx2];
    wptrrg11 = &argp2[vw_e8].w;
    *wptrrg11 &= con(0xfffffff8);
    *wptrrg11 |= unsignedword(vs_ac.u.b_01) & con(0x7);
    *wptrrg11 &= con(0xff07);
    *wptrrg11 |= (vw_100 & con(0x1f)) * con(0x8);
    *wptrrg11 &= con(0xffffe0ff);
    *wptrrg11 |= (vw_ec & con(0x1f)) << con(0x8);
     wordrg41 = urshift(vs_ac.u.w_02, con(0xa));
    if (wordrg41 != vw_dc)
    {
      vw_dc = wordrg41;
      *wptrrg11 |= con(0x2000);
    }
    else
      *wptrrg11 &= con(0xffffdfff);
    vw_100 = vs_ac.u.w_02 & con(0x1f);
    vw_ec = ulrshift(vs_ac.u.w_02, con(0x6), con(0xb));
  }
M_preexit: // M_23E4E:
  if (!vbool_a0)
    goto M_exit;
  // M_23E7B:
  for ( vw_f4_1 = con(0x0); unsignedword(CUTX8(ddata.savegamep4->w0[con(0x2)])) > vw_f4_1; vw_f4_1++)
    if (pw_98[vw_f4_1] >= con(0x0))
      SKW_3e74_58a3(pw_98[vw_f4_1]);
M_exit: // M_23E8B:
  DM2_DEALLOC_LOBIGPOOL(vl_7c);
  SKW_1c9a_0648(vw_e4);
  return CUTX8(vw_d8);
}
