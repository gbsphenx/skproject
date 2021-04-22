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

// TODO: track & split rg6
void SKW_DRAW_MAP_CHIP(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x8 argb2, x16 argw3)
{
  x8 byterg5;
  x16 wordrg5;
  x8 byterg3h;
  x8 byterg6;
  x16 wordrg6;
  c_record* recoptrrg1;
  c_record* recoptrrg6;
  t_gfxdata* gptrrg1;
  t_gfxdata* gptrrg11;
  t_gfxdata* gptrrg12;
  x8*** bppptrrg41;
  x8** bpptrrg42;
  x8* bptrrg43;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;
  s_8wordarray vs_10;
  t_gfxdata vcarr_20[PAL16];
  t_gfxdata vcarr_30[PAL16];
  c_rect rc_40;
  t_gfxdata* pg_48;
  t_gfxdata* pg_4c;
  x8* pb_50;
  t_gfxdata* pg_58;
  u_lpg vs_60; // ATTENTION!
  x8* pb_64;
  x16 vw_68;
  x16 vw_6c;
  x16 vw_70;
  x16 vw_78;
  x16 vw_7c;
  x8 vb_80;
  x8 vb_88;
  x16 vw_8c;
  x16 vw_90;
  t_gfxdata* parcarr10;
  t_gfxdata* parcarr12;
  t_gfxdata* parcarr14;
  t_gfxdata* parcarr18;
  bool flag;

  x16 vw_74; // multiple purpose temporary as it seems
  vw_0c = eaxw;
  vw_08 = edxw;
  vw_04 = ebxw;
  pg_48 = NULL;
  SKW_SUMMARIZE_STONE_ROOM(&vs_10, vw_08, vw_04, ecxw);
  SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(con(0x8), &pg_48, vcarr_20, argb2);
  if ((argw3 & con(0x10)) != con(0x0))
  {
    x16 wordrg34 = 2 * ddata.v1d274e + ddata.v1d274a;
    x16 wordrg29 = argw1 - ddata.v1d2750;
    SKW_SET_SRECT(&rc_40, argw0 - ddata.v1d274e, wordrg29, wordrg34, 2 * ddata.v1d2750 + ddata.v1d274c);
    SKW_UNION_RECT(&rc_40, &vw_68, &vw_68, &ddata.v1e0b9c);
    SKW_FILL_RECT_SUMMARY(&ddata.v1d694c, ddata.paletteptr1[con(0x8)], &rc_40);
  }
  bool vbool_54;
  if ((CUTX8(vs_10.warr_00[0x1] >> con(0x5))) != con(0x6))
    vbool_54 = false;
  else
  {
    if ((vs_10.warr_00[0x1] & con(0x1)) == con(0x0))
      vbool_54 = false;
    else
      vbool_54 = true;
  }
  pb_64 = NULL;
  // M_33C2B:
  bool farloop; // added
  do
  {
    farloop = false;
    x16 mode = 0;
    // mode = 0: M_34037
    // mode = 1: M_33C8E
    // mode = 2: M_33E2C
    // mode = 3: M_349E3
    // mode = 4: M_33F66

    ui16 uwordrg11 = vs_10.warr_00[0x0];
    if (uwordrg11 < con(0x5))
    {
      if (uwordrg11 < con(0x1))
      {
        if (uwordrg11 == con(0x0))
          mode = 1;
        else
          mode = 0;
      }
      else if (uwordrg11 <= con(0x1))
      {
        if ((vs_10.warr_00[0x1] & con(0x8)) == con(0x0))
          mode = 2;
        else
          mode = 3; // moved some lines to the switch because it's the only path
      }
      else if (uwordrg11 == con(0x2))
      {
        vw_74 = con(0x3);
        if (vs_10.warr_00[0x3] != con(0x0))
        {
          flag = (argw3 & con(0x2)) == con(0x0);
          vw_74 = con(0x4);
          if (!flag)
            if ((CUTLX8(ddata.longmallocx) & con(0x1)) != con(0x0))
              vw_74 = con(0x5);
        }
        parcarr10 = vcarr_20;
        gptrrg1 = pg_48;
        SKW_DRAW_CHIP_OF_MAGIC_MAP(gptrrg1, argw0, argw1, vw_74, con(0x0), parcarr10);
        mode = 0;
      }
      else
        mode = 0;
    }
    else if (uwordrg11 <= con(0x5))
      mode = 2;
    else if (uwordrg11 < con(0x10))
    {
      if (uwordrg11 == con(0x7))
        mode = 1;
      else
        mode = 0;
    }
    else if (uwordrg11 <= con(0x10))
    {
      vw_74 = con(0xb);
      mode = 4;
    }
    else if (uwordrg11 <= con(0x11))
    {
      vw_74 = con(0xa);
      mode = 4;
    }
    else if (uwordrg11 <= con(0x13))
    {
      x16 wordrg155 = SKW_0cee_06dc(vw_08, vw_04) - ecxw;
      wordrg155 &= con(0x3);
      vw_74 = wordrg155;
      if (vs_10.warr_00[0x3] != con(0x0))
      {
        wordrg5 = (wordrg155 + con(0x2)) & con(0x3);
        vw_74 = wordrg5;
      }
      vw_74 += con(0x6);
      parcarr10 = vcarr_20;
      gptrrg1 = pg_48;
      SKW_DRAW_CHIP_OF_MAGIC_MAP(gptrrg1, argw0, argw1, vw_74, con(0x0), parcarr10);
      mode = 0;
    }
    else
      mode = 0;
    x16 vw_94;
    switch (mode)
    {
      case 1: // M_33C8E:
        if ((argw3 & con(0x2)) == con(0x0) || !vbool_54 || ((CUTLX8(ddata.longmallocx) & con(0x2)) == con(0x0)))
        {
          if (pb_64 == NULL)
            SKW_DRAW_CHIP_OF_MAGIC_MAP(pg_48, argw0, argw1, con(0x0), con(0x0), vcarr_20);
          for (vw_74 = con(0x0); vw_74 <= con(0x3); vw_74++)
          {
            vw_94 = unsignedword(CUTX8(vs_10.warr_00[0x3 + vw_74]));
            if (CUTX8(vw_94) != con(0xffffffff))
            {
              if ((argw3 & con(0x2)) != con(0x0) || DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0xf9), CUTX8(vw_94)) == con(0x0))
              {
                x16 wordrg178 = SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(con(0x9), &pg_4c, vcarr_30, CUTX8(vw_94));
                if (wordrg178 != con(0x0))
                {
                  x32 longrg44 = unsignedlong(urshift(vs_10.warr_00[0x3 + vw_74], con(0x8))); // replacement for mov8(RG4Blo, lst.o_peek8(0x17, 2 * RG4L)) ff.
                  longrg44 <<= con(0x18); // ugly, longsize
                  longrg44 >>= con(0x1a);
                  x16 wordrg32 = CUTX16(longrg44);
                  x32 longrg42 = 2 * signedlong(wordrg32) + con(0x2);
                  x32 longrg21 = signedlong(wordrg178);
                  x16 wordrg177 = vw_74 & con(0x1);
                  if (longrg21 >= longrg42)
                    vw_94 = 2 * wordrg32 + wordrg177;
                  else
                    vw_94 = wordrg177;
                  SKW_DRAW_CHIP_OF_MAGIC_MAP(pg_4c, argw0, argw1, vw_94, unsignedword(table1d6980[vw_74]), vcarr_30);
                }
              }
            }
          }
        }
        else
        {
          vw_74 = con(0x1);
          parcarr10 = vcarr_20;
          gptrrg1 = pg_48;
          SKW_DRAW_CHIP_OF_MAGIC_MAP(gptrrg1, argw0, argw1, vw_74, con(0x0), parcarr10);
        }
      break;

      case 2: // M_33E2C:
        SKW_DRAW_CHIP_OF_MAGIC_MAP(pg_48, argw0, argw1, con(0x2), con(0x0), vcarr_20);
        vw_74 = unsignedword(CUTX8(vs_10.warr_00[0x5]));
        if (CUTX8(vw_74) != con(0xffffffff))
        {
          if ((argw3 & con(0x2)) != con(0x0) || DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xa), con(0xb), con(0xf9), CUTX8(vw_74)) == con(0x0))
          {
            x16 wordrg27 = SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(con(0xa), &pg_4c, vcarr_30, CUTX8(vw_74));
            if (wordrg27 != con(0x0))
            {
              x16 wordrg31 = signedword(CUTX8(vs_10.warr_00[0x5] >> con(0x8))) / con(0xa);
              if (wordrg27 >= (wordrg31 + con(0x1)))
                vw_74 = wordrg31;
              else
                vw_74 = con(0x0);
              SKW_DRAW_CHIP_OF_MAGIC_MAP(pg_4c, argw0, argw1, vw_74, con(0x0), vcarr_30);
            }
          }
        }
        if (vs_10.warr_00[0x0] == con(0x5))
        {
          x16 wordrg168 = SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(con(0x18), &pg_4c, vcarr_30, con(0x0));
          if (wordrg168 != con(0x0))
          {
            parcarr10 = vcarr_30;
            wordrg5 = wordrg168;
            gptrrg1 = pg_4c;
            SKW_DRAW_CHIP_OF_MAGIC_MAP(gptrrg1, argw0, argw1, CUTX16(ddata.longmallocx % wordrg5), con(0x0), parcarr10);
          }
        }
      break;

      case 3: // M_349E3:
        SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(con(0x8), &pg_48, vcarr_20, ddata.v1e03d4[ddata.v1e03c0->w8 & con(0x3f)]);
        SKW_DRAW_CHIP_OF_MAGIC_MAP(pg_48, argw0, argw1, con(0x0), con(0x0), vcarr_20);
      return;

      case 4: // M_33F66:
        recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(vs_10.warr_00[0x4]);
        if ((recoptrrg1->u2.b.b0 & con(0x40)) == con(0x0) && (recoptrrg1->u2.b.b1 & con(0x20)) == con(0x0))
          vw_74 += con(0x2);
        if (vs_10.warr_00[0x3] == con(0x0) || vs_10.warr_00[0x3] == con(0x5))
          vw_74 += con(0x4);
        parcarr10 = vcarr_20;
        gptrrg1 = pg_48;
        SKW_DRAW_CHIP_OF_MAGIC_MAP(gptrrg1, argw0, argw1, vw_74, con(0x0), parcarr10);
      break;
    }

    // M_34037: case 0 and others
    if ((argw3 & con(0x10)) != con(0x0))
    {
      for (vw_74 = con(0x0); ddata.savegamew2 > vw_74; vw_74++)
      {
        x16 vw_5c = vw_74;
        wordrg5 = vw_5c;
        if (ddata.hero[wordrg5].w36 != con(0x0))
        {
          byterg6 = SKW_QUERY_CLS1_FROM_RECORD(vw_0c);
          x8 byterg33 = unsignedlong(SKW_QUERY_CLS2_FROM_RECORD(vw_0c));
          pg_58 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(byterg6, con(0x40), byterg33);
          parcarr12 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(byterg6, con(0x40), byterg33);
          x16 wordrg145;
          if (ddata.savegames1.b2 == con(0x0))
            wordrg145 = vw_5c + con(0x5);
          else
            wordrg145 = con(0x9);
          SKW_DRAW_CHIP_OF_MAGIC_MAP(pg_58, argw0, argw1, wordrg145, unsignedword(table1d6984[(unsignedword(ddata.hero[wordrg5].b1d) + con(0x4) - ecxw) & con(0x3)]), parcarr12);
        }
      }
    }
    if ((argw3 & con(0x40)) != con(0x0))
    {
      ui16 uwordrg1 = ddata.v1e0b56;
      if (uwordrg1 < con(0x2))
        vw_74 = con(0x2);
      else if (uwordrg1 <= con(0x2))
        vw_74 = con(0xb);
      else if (uwordrg1 == con(0x3))
        vw_74 = con(0xa);
      else
        vw_74 = con(0x2);
      x8 byterg32 = SKW_QUERY_CLS1_FROM_RECORD(vw_0c);
      byterg5 = SKW_QUERY_CLS2_FROM_RECORD(vw_0c);
      gptrrg11 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(byterg32, con(0x40), byterg5);
      parcarr14 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(byterg32, con(0x40), byterg5);
      SKW_DRAW_CHIP_OF_MAGIC_MAP(gptrrg11, argw0, argw1, vw_74, con(0x0), parcarr14);
    }
    if ((argw3 & con(0xa5)) == con(0x0))
      return;
    vw_8c = con(0xfffe);
    vw_6c = con(0xfffe);
    vw_90 = con(0xfffe);
    wordrg5 = vs_10.warr_00[0x2];
    // M_341E5: co-label M_3437D
    for (;; wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg5))
    {
      if (wordrg5 == con(0xfffffffe))
      {
        if (vw_90 != wordrg5)
        {
          recoptrrg6 = SKW_GET_ADDRESS_OF_RECORD(vw_90);
          x16 vw_84 = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(vw_90);
          vb_80 = CUTX8(vw_84);
          if ((argw3 & con(0x20)) == con(0x0))
          {
            vs_60.l_00 = lextended(SKW_QUERY_CLS2_FROM_RECORD(vw_90) & con(0xff));
            x16 wordrg108 = SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(SKW_QUERY_CLS1_FROM_RECORD(vw_90), &pg_4c, vcarr_30, CUTLX8(vs_60.l_00));
            if (wordrg108 == con(0x0))
            {
              if ((vw_84 & con(0x1)) == con(0x0) && (vw_84 & con(0x2000)) == con(0x0))
              {
                byterg5 = SKW_QUERY_CLS1_FROM_RECORD(vw_0c);
                x8 byterg31 = SKW_QUERY_CLS2_FROM_RECORD(vw_0c);
                vs_60.pg_00 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(byterg5, con(0x40), byterg31);
                parcarr18 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(byterg5, con(0x40), byterg31);
                gptrrg12 = vs_60.pg_00;
                SKW_DRAW_CHIP_OF_MAGIC_MAP(gptrrg12, argw0, argw1, CUTX16(ddata.longmallocx) & con(0x1), con(0x0), parcarr18);
              }
            }
            else
            {
              if ((vw_84 & con(0x1)) == con(0x0))
                vw_74 = 2 * CUTX16(ddata.longmallocx % ((wordrg108 - (wordrg108 >> con(0x1f))) >> con(0x1)));
              else
              {
                SKW_1c9a_02c3(recoptrrg6, SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(recoptrrg6->u4.b.b0));
                vw_74 = con(0x0);
              }
              vw_94 = (ecxw - ulrshift(recoptrrg6->ue.w, con(0x6), con(0xe))) & con(0x3);
              gptrrg12 = pg_4c;
              SKW_DRAW_CHIP_OF_MAGIC_MAP(gptrrg12, argw0, argw1, (vw_94 & con(0x1)) + vw_74, unsignedword(table1d6980[vw_94]), vcarr_30);
            }
          }
          else
          {
            byterg3h = SKW_QUERY_CLS1_FROM_RECORD(vw_0c);
            x8 byterg3 = SKW_QUERY_CLS2_FROM_RECORD(vw_0c);
            pg_58 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(byterg3h, con(0x40), byterg3);
            parcarr18 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(byterg3h, con(0x40), byterg3);
            gptrrg12 = pg_58;
            SKW_DRAW_CHIP_OF_MAGIC_MAP(gptrrg12, argw0, argw1, (CUTX16(ddata.longmallocx) & con(0x1)) + con(0x3), con(0x0), parcarr18);
          }
          if ((vb_80 & con(0x1)) != con(0x0))
          {
            if ((argw3 & con(0x4)) != con(0x0))
            {
              if ((SKW_0cee_2df4(vw_90) & con(0xf)) != con(0x0))
              {
                wordrg5 = recoptrrg6->w0;
                while (wordrg5 != con(0xfffffffe))
                {
                  vw_74 = urshift(wordrg5 & con(0x3c00), con(0xa));
                  if (vw_74 >= con(0x5) && vw_74 <= con(0xa))
                  {
                    wordrg6 = wordrg5;
                    x8 byterg46 = SKW_QUERY_CLS2_FROM_RECORD(wordrg6);
                    if (SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(SKW_QUERY_CLS1_FROM_RECORD(wordrg6), &pg_4c, vcarr_30, byterg46) != con(0x0))
                    {
                      wordrg6 >>= con(0xe);
                      wordrg6 -= ecxw;
                      SKW_DRAW_CHIP_OF_MAGIC_MAP(pg_4c, argw0, argw1, con(0x0), unsignedword(table1d6984[wordrg6 & con(0x3)]), vcarr_30);
                    }
                  }
                  wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg5);
                }
              }
            }
          }
        }
        wordrg5 = vw_6c;
        break;
      }
      vw_74 = urshift(wordrg5 & con(0x3c00), con(0xa));
      if (vw_74 != con(0x4))
      {
        if ((argw3 & con(0x4)) != con(0x0))
        {
          if (vw_74 != con(0xe))
          {
            if (vw_74 != con(0xf))
            {
              if (vw_74 >= con(0x5) && vw_74 <= con(0xa))
              {
                wordrg6 = wordrg5;
                x8 byterg45 = SKW_QUERY_CLS2_FROM_RECORD(wordrg6);
                x16 wordrg10 = SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(SKW_QUERY_CLS1_FROM_RECORD(wordrg6), &pg_4c, vcarr_30, byterg45);
                if (wordrg10 != con(0x0))
                {
                  wordrg6 >>= con(0xe);
                  wordrg6 -= ecxw;
                  vw_94 = wordrg6 & con(0x3);
                  if (vs_10.warr_00[0x0] != con(0x0) || vbool_54)
                  {
                    wordrg6 = con(0x0);
                    vw_74 = con(0x0);
                    vw_94 = unsignedword(table1d6984[vw_94]);
                    SKW_DRAW_CHIP_OF_MAGIC_MAP(pg_4c, argw0, argw1, vw_74, vw_94, vcarr_30);
                  }
                  else if (wordrg10 > con(0x2))
                  {
                    vw_74 = (vw_94 & con(0x1)) + con(0x1);
                    vw_94 = unsignedword(table1d6980[vw_94]);
                    SKW_DRAW_CHIP_OF_MAGIC_MAP(pg_4c, argw0, argw1, vw_74, vw_94, vcarr_30);
                  }
                }
              }
            }
            else if (vw_8c == con(0xfffffffe))
              vw_8c = wordrg5;
          }
          else if (vw_6c == con(0xfffffffe))
            vw_6c = wordrg5;
        }
        continue;
      }
      wordrg6 = wordrg5;
      vb_88 = CUTX8(SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg6));
      if (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xf), con(0xb), con(0xf9), SKW_QUERY_CLS2_FROM_RECORD(wordrg6)) == con(0x0))
      {
        x16 wordrg114;
        if ((vb_88 & con(0x1)) == con(0x0))
          wordrg114 = argw3 & con(0x21);
        else
          wordrg114 = argw3 & con(0x80);
        if (wordrg114 == con(0x0))
          continue;
      }
      vw_90 = wordrg5;
    }
    // M_34657:
    for(;;)
    {
      if (wordrg5 == con(0xfffffffe))
      {
        wordrg5 = vw_8c;
        while (wordrg5 != con(0xfffffffe))
        {
          if (urshift(wordrg5 & con(0x3c00), con(0xa)) == con(0xf))
          {
            wordrg6 = wordrg5;
            if (SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(SKW_QUERY_CLS1_FROM_RECORD(wordrg6), &pg_4c, vcarr_30, SKW_QUERY_CLS2_FROM_RECORD(wordrg6)) != con(0x0))
              SKW_DRAW_CHIP_OF_MAGIC_MAP(pg_4c, argw0, argw1, (CUTX16(ddata.longmallocx) & con(0x1)) + con(0x1), SKW_RAND02(), vcarr_30);
          }
          wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg5);
        }
        if (vs_10.warr_00[0x0] != con(0x0) || pb_64 != NULL)
          break;
        vw_78 = vw_08; // adj
        vw_70 = vw_04; // adj
        pb_50 = pb_64;
        x16 wordrg106 = ddata.v1d3248;
        for(;;)
        {
          vw_74 = wordrg106;
          if (vw_74 != ddata.v1d3248 || vw_74 == con(0xffffffff))
            break;
          wordrg106 = SKW_LOCATE_OTHER_LEVEL(ddata.v1d3248, &vw_08, &vw_04, con(0x0), &pb_50);
        }
        wordrg6 = vw_74;
        if (wordrg6 < con(0x0) || wordrg6 == ddata.v1d3248)
          break;
        bppptrrg41 = &ddata.v1e03cc[vw_74];
        bpptrrg42 = &(*bppptrrg41)[vw_08];
        bptrrg43 = *bpptrrg42;
        if ((UI8CAST(bptrrg43[vw_04]) >> con(0x5)) != con(0x0))
          break;
        pb_64 = UPCAST(x8, con(0x1)); // TODO: UGLY!
        vw_7c = ddata.v1d3248;
        SKW_CHANGE_CURRENT_MAP_TO(vw_74);
        SKW_SUMMARIZE_STONE_ROOM(&vs_10, vw_08, vw_04, ecxw);
        farloop = true;
        break; // goto M_33C2B;
      }
      if (urshift(wordrg5 & con(0x3c00), con(0xa)) == con(0xe))
      {
        wordrg6 = wordrg5;
        x16 wordrg15 = SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(SKW_QUERY_CLS1_FROM_RECORD(wordrg6), &pg_4c, vcarr_30, SKW_QUERY_CLS2_FROM_RECORD(wordrg6));
        if (wordrg15 != con(0x0))
        {
          if (wordrg15 > con(0x3))
          {
            x16 wordrg166 = ulrshift(ddata.savegametimarrp[SKW_GET_ADDRESS_OF_RECORD(wordrg6)->u6.w].v.w_08, con(0x4), con(0xe));
            vw_94 = wordrg166;
            wordrg166 -= ecxw;
            wordrg166 &= con(0x3);
            vw_94 = wordrg166;
            vw_74 = con(0x3);
            ui16 uwordrg1 = unsignedword(SKW_48ae_011a(wordrg6)); // extended from byte
            uwordrg1++;
            if (uwordrg1 <= con(0x4))
            {
              switch (uwordrg1)
              {
                case 0:
                case 1:
                case 3:
                  vw_74 = con(0x0);
                break;

                case 2:
                  vw_74 = vw_94 + con(0x3);
                  wordrg6 >>= con(0xe);
                  wordrg6 -= ecxw;
                  wordrg6 &= con(0x3);
                  // CRITICAL ADRESS:
                  // signedword(byte_at(idx(0x1d697c), 4 * RG1W + wordrg6))
                  // wordrg6 can be 0...3
                  // RG1L comes from vw_74, comes from line @034720, vw_94 + RG2L
                  // vw_94 = 0...3, RG2L = 3!
                  // can be changed to:
                  // signedword(byte_at(idx(0x1d6988), 4 * (RG1W - 3) + wordrg6))
                  // signedword(byte_at(idx(0x1d6984), 4 * (RG1W - 3 + 1) + wordrg6))
                  // signedword(byte_at(table1d6984[4 * (RG1W - 3 + 1) + wordrg6]))
                  vw_74 += signedword(table1d6984[4 * (vw_74 - con(0x2)) + wordrg6]);
                break;

                case 4:
                break;

                default: throw(THROW_DMABORT);
              }
            }
          }
          else
            vw_74 = con(0x0);
          SKW_DRAW_CHIP_OF_MAGIC_MAP(pg_4c, argw0, argw1, vw_74, unsignedword(table1d6984[((wordrg5 >> con(0xe)) - ecxw) & con(0x3)]), vcarr_30);
        }
      }
      wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg5);
    }
  } while (farloop);
  if (pb_64 != NULL)
  {
    vw_08 = vw_78;
    vw_04 = vw_70;
    SKW_CHANGE_CURRENT_MAP_TO(vw_7c);
    SKW_SUMMARIZE_STONE_ROOM(&vs_10, vw_08, vw_04, ecxw);
  }
  x16 wordrg19 = SKW_LOCATE_OTHER_LEVEL(ddata.v1d3248, &vw_08, &vw_04, con(0xffffffff), NULL);
  if (wordrg19 < con(0x0))
    return;
  x8*** hlpptr1 = &ddata.v1e03cc[wordrg19];
  x8** hlpptr2 = *hlpptr1 + signedlong(vw_08);
  x8* hlpptr3 = *hlpptr2 + signedlong(vw_04);
  x16 wordrg41 = unsignedword(*hlpptr3);
  // vw_74 = wordrg41; // TODO: what for? it's not going back anymore
  if (CUTLX8(unsignedlong(*hlpptr3) >> con(0x5)) != con(0x2))
    return;
  if ((wordrg41 & con(0x8)) == con(0x0))
    return;
  SKW_DRAW_CHIP_OF_MAGIC_MAP(pg_48, argw0, argw1, con(0x12), con(0x0), vcarr_20);
}

x16 SKW_CREATE_CLOUD(ui16 eaxuw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0)
{
  c_record* recoptrrg5;
  c_aispec* aiptrrg2;
  c_tim vtim_04;
  x16 vw_1c;
  x16 vw_20;
  x16 vw_24;
  x16 vw_28;
  x16 vw_2c;
  x16 vw_30;
  x16 vw_34;
  x16 vw_38;

  vw_34 = ebxw;
  vw_38 = ecxw;
  vw_30 = SKW_ALLOC_NEW_RECORD(con(0xf));
  if (vw_30 == con(0xffffffff))
    return con(0xffffffff);
  recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(vw_30);
  if (ebxw > con(0xff))
  {
    vw_2c = ebxw & con(0xff);
    vw_20 = ecxw & con(0xff);
    x16 wordrg42 = urshift(ebxw, con(0x8));
    vw_34 = wordrg42;
    vw_38 = urshift(ecxw, con(0x8));
    vw_34 = wordrg42 - con(0x1);
  }
  else
  {
    vw_2c = ebxw;
    vw_20 = ecxw;
  }
  if (argw0 != con(0xff))
  {
    recoptrrg5->u2.b.b0 &= con(0x7f);
    vw_30 = (vw_30 & con(0x3fff)) | (argw0 << con(0xe));
  }
  else
    recoptrrg5->u2.b.b0 |= con(0xffffff80);
  x16 wordrg111 = (eaxuw - con(0xff80)) & con(0x7f);
  recoptrrg5->u2.b.b0 &= con(0xffffff80);
  recoptrrg5->u2.w |= wordrg111;
  recoptrrg5->u2.b.b1 = con(0x0);
  recoptrrg5->u2.w |= unsignedword(CUTX8(wordrg111)) << con(0x8);
  if (ddata.v1d3248 != ddata.v1e0282 || vw_34 != ddata.v1e0280 || vw_38 != ddata.v1e027e)
    vw_28 = DM2_MIN(con(0xff), edxw / 2 + con(0x80));
  else
    vw_28 = con(0xff);
  SKW_QUEUE_NOISE_GEN2(con(0xd), con(0x81), con(0xfe), recoptrrg5->u2.b.b0 & con(0x7f), vw_34, vw_38, con(0x1), con(0x6c), vw_28);
  SKW_APPEND_RECORD_TO(vw_30, vw_34, vw_38, NULL);
  x16 wordrg105;
  x16 wordrg104 = eaxuw;
  if (wordrg104 != con(0xffffffe4))
  {
    if (wordrg104 != con(0xffffff8e))
      wordrg105 = con(0x1);
    else
      wordrg105 = edxw / 2;
  }
  else
    wordrg105 = con(0x5);
  x32 longrg45 = signedlong(wordrg105) + ddata.longmallocx;
  longrg45 |= (signedlong(ddata.v1d3248) << con(0x18));
  vtim_04.un.l_00 = longrg45;
  vtim_04.b_04 = con(0x19);
  vtim_04.b_05 = con(0x0);
  vtim_04.v.w_08 = vw_30;
  vtim_04.u.b.b_06 = CUTX8(vw_34);
  vtim_04.u.b.b_07 = CUTX8(vw_38);
  SKW_QUEUE_TIMER(&vtim_04);
  x16 wordrg103 = eaxuw;
  if (wordrg103 != con(0xffffff82) && wordrg103 != con(0xffffff80) && wordrg103 != con(0xffffffb0) && wordrg103 != con(0xffffffb1))
  {
    if (wordrg103 == con(0xffffff84) || wordrg103 == con(0xffffff8d))
    {
      x8 byterg18 = SKW_GET_TILE_VALUE(vw_34, vw_38);
      x8 byterg4 = CUTX8(unsignedlong(byterg18) >> con(0x5));
      if (byterg4 == con(0x4) && (byterg18 & con(0x7)) != con(0x5))
      {
        recoptrrg5 = SKW_GET_ADDRESS_OF_TILE_RECORD(vw_34, vw_38);
        if ((recoptrrg5->u2.b.b0 & con(0x40)) != con(0x0) || (recoptrrg5->u2.b.b1 & con(0x20)) != con(0x0))
        {
          x8 byterg16;
          if (eaxuw != con(0xff84))
            byterg16 = con(0x0);
          else
            byterg16 = con(0x2);
          SKW_INVOKE_MESSAGE(vw_34, con(0x0), unsignedword(byterg16), vw_38, ddata.longmallocx + con(0x1));
        }
      }
    }
  }
  else
  {
    vw_34 = vw_2c;
    vw_24 = vw_2c;
    vw_38 = vw_20;
    vw_1c = vw_20;
    bool vbool_18 = true;
    x16 wordrg61 = edxw / 2 + con(0x1);
    wordrg61 += SKW_RAND16(wordrg61) + con(0x1);
    vw_28 = wordrg61;
    if (eaxuw == con(0xff80) || eaxuw == con(0xffb0) || (wordrg61 = urshift(wordrg61, con(0x1)), wordrg61 != con(0x0)))
    {
      if (ddata.v1d3248 == ddata.v1e0282 && vw_34 == ddata.v1e0280)
        if (vw_38 == ddata.v1e027e)
          SKW_ATTACK_PARTY(wordrg61, con(0x1), con(0x3f));
      vw_30 = SKW_GET_CREATURE_AT(vw_34, vw_38);
      bool skipfirst = false; // added
      if (vw_30 != con(0xffffffff))
        skipfirst = true;
      for(;;)
      {
        if (!skipfirst)
        {
          if (!vbool_18)
            break;
          vw_30 = SKW_1c9a_03cf(&vw_24, argw0, &vw_1c);
          if (vw_30 == con(0xffffffff) == con(0x0) || (vw_24 == vw_2c && vw_1c == vw_20))
            break;
          vw_34 = vw_24;
          vw_38 = vw_1c;
          vbool_18 = false;
          wordrg61 = vw_28 >> con(0x1);
        }
        else
          skipfirst = false;
        // M_6AA9:
        if (!SKW_1c9a_0958(vw_30) == con(0x0))
        {
          x16 wordrg12;
          x8 byterg12 = SKW_GET_ADDRESS_OF_RECORD(vw_30)->u4.b.b0;
          aiptrrg2 = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(byterg12);
          wordrg12 = unsignedword(byterg12); // added! TODO: is that correct?
          if ((aiptrrg2->u18.b.b1 & con(0x10)) != con(0x0))
          {
            wordrg12 = eaxuw;
            if (wordrg12 != con(0xffffff80))
              wordrg61 = DM2_MAX(con(0x1), urshift(wordrg61, con(0x2)));
          }
          if (((aiptrrg2->u18.w >> con(0x4)) & con(0xf)) != con(0xf))
          {
            if ((aiptrrg2->u0.b.b0 & con(0x20)) != con(0x0))
              wordrg61 = urshift(wordrg61, con(0x2));
            wordrg61 -= SKW_RAND16(2 * wordrg12 + con(0x1)); // TODO: wordrg12 correct?!?
            if (wordrg61 > con(0x0))
              SKW_ATTACK_CREATURE(vw_30, vw_38, con(0x200d), vw_34, con(0x64), wordrg61);
          }
        }
      }
    }
    if (ddata.v1d3248 == ddata.v1e0266)
      ddata.v1e0390.b0 |= con(0x1);
  }
  return SKW_075f_0182(eaxuw, vw_20, urshift(recoptrrg5->u2.w, con(0x8)), vw_2c);
}

// SPX: _13e4_0982 renamed CREATURE_THINK_0982
void SKW_13e4_0982(void)
{
  x16* wptrrg6;
  x16 vw_00;
  t_gfxdata* gptrrg5;

  bool skip = false; // added
  if (ddata.savegames1.b3 != con(0x0) && (ddata.s350.v1e0552->u0.b.b1 & con(0x10)) == con(0x0))
  {
    if (ddata.s350.v1e0556->u.v.b1a != con(0x13) && ddata.s350.v1e0556->u.v.barr16[con(0x1)] != con(0x13))
    {
      ddata.s350.v1e0562.un.l_00 += con(0x4);
      skip = true;
    }
  }
  // M_15523:
  if (!skip)
  {
    x16 wordrg41;
    wptrrg6 = ddata.s350.v1e055e;
    bool boolrg7;
    if (ddata.s350.v1e0562.b_04 != con(0x22))	// glbCreatureTimer.TimerType() == tty22
    {
      boolrg7 = false;
      wordrg41 = R_4FCC(ddata.s350.v1e054e->u4.b.b0, &wptrrg6[con(0x1)], &ddata.s350.v1e055a, *wptrrg6) ? 1 : 0;
    }
    else
    {
      boolrg7 = true;
      ddata.s350.v1e0556->u.v.b1a = ddata.s350.v1e0556->u.v.barr16[con(0x1)];
      if (ddata.s350.v1e0556->u.v.b1a == con(0xffffffff))
      {
        SKW_14cd_09e2();
      }
      else
      {
        ddata.s350.v1e0556->u.v.barr16[con(0x1)] = con(0xffffffff);
        if (SKW_14cd_062e() == con(0x0) || ddata.s350.v1e0556->u.v.b1a == con(0x13))
        {
          if ((table1d613a[ddata.s350.v1e0556->u.v.b1a] & con(0x4)) != con(0x0))
          {
            ddata.s350.v1e07d8.b1 = con(0x0);
            ddata.s350.v1e07d8.b3 = con(0xffffffff);
          }
          ui8 ubyterg11 = ddata.s350.v1e0556->u.v.b1a;
          if (ubyterg11 >= con(0x6))
          {
            if (ubyterg11 <= con(0x6))
              ddata.s350.v1e0556->u.v.b1d = CUTX8((ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe)) - con(0x1)) & con(0x3));
            else if (ubyterg11 == con(0x7))
              ddata.s350.v1e0556->u.v.b1d = CUTX8((ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe)) + con(0x1)) & con(0x3));
          }
        }
        else
          ddata.s350.v1e0556->u.v.b1a = con(0xffffffff);
      }
      if (ddata.s350.v1e0556->u.v.b1a == con(0x13))
      {
        ddata.s350.v1e0556->u.v.w0e = wptrrg6[0];
        ddata.s350.v1e0556->u.v.w10 = wptrrg6[1];
        if ((table1d607e[SKW_QUERY_GDAT_CREATURE_WORD_VALUE(ddata.s350.v1e054e->u4.b.b0, con(0x1))].u.b[0] & con(0x8)) == con(0x0))
        {
          ui8 ubyterg10 = ddata.s350.v1e0552->barr22[con(0x1)];
          x16 wordrg11;
          if (ubyterg10 < con(0x1))
            wordrg11 = con(0x6e);
          else if (ubyterg10 <= con(0x1))
            wordrg11 = con(0xbe);
          else if (ubyterg10 == con(0x2))
            wordrg11 = con(0xff);
          else
            wordrg11 = con(0x6e);
          SKW_CREATE_CLOUD(con(0xffa8), unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_07), wordrg11, con(0xff));
        }
      }
      x16 wordrg190 = signedword(ddata.s350.v1e0556->u.v.b1a);
      if ((signedword(ddata.s350.v1e0556->u.v.b1a) < con(0x32)) || wordrg190 > con(0x34))	// !(si = xCreatureInfo->Command) >= ccm32 && si <= ccm34)
        wordrg41 = SKW_GET_CREATURE_ANIMATION_FRAME(ddata.s350.v1e054e->u4.b.b0, wptrrg6, &wptrrg6[con(0x1)], wordrg190, &ddata.s350.v1e055a, con(0x0)) ? 1 : 0;
      else
      {
        x32 longrg4 = lextended(wordrg190);
        longrg4 -= con(0x32);
        longrg4 += ddata.longmallocx;
        longrg4 |= unsignedlong(ddata.s350.v1e0571) << con(0x18);
        ddata.s350.v1e0562.un.l_00 = longrg4;
        skip = true;
      }
    }
    if (!skip)
    {
      // M_156EA:
      do
      {
        x8 boolrg1;
        if (boolrg7 || ddata.s350.v1e0556->u.v.b21 == con(0x0))
          boolrg1 = true;
        else
        {
          if (ubrshift(ddata.s350.v1e055a[con(0x2)] & con(0x40), con(0x6)) != con(0x0))
            boolrg1 = false;
          else
            boolrg1 = true;
        }
        if (boolrg1)
        {
          //if (ubrshift(ddata.s350.v1e055a[con(0x2)] & con(0xffffff80), con(0x7)) != con(0x0))
		  if (ddata.s350.v1e055a != NULL && (ubrshift(ddata.s350.v1e055a[con(0x2)] & con(0xffffff80), con(0x7)) != con(0x0)))
          {
            SKW_13e4_01a3();
            ddata.s350.v1e0556->u.v.b21 |= SKW_PROCEED_CCM() ? 1 : 0;
          }
        }
        if ((ddata.s350.v1e0556->u.v.b21 == con(0x0)) || (ubrshift(ddata.s350.v1e055a[con(0x2)] & con(0x40), con(0x6)) == con(0x0)))
          break;
        wordrg41 = R_50CB(ddata.s350.v1e054e->u4.b.b0, &wptrrg6[con(0x1)], &ddata.s350.v1e055a, *wptrrg6);
      } while (wordrg41 != con(0x2));
      ddata.s350.v1e0562.b_04 = ((wordrg41 != con(0x1)) ? 1 : 0) + con(0x21);
      if (ddata.s350.v1e0570 != con(0x0))
        return;
      ddata.s350.v1e0562.un.l_00 = SKW_CREATURE_SOMETHING_1c9a_0a48() | (unsignedlong(ddata.s350.v1e0571) << con(0x18));
    }
  }
  // M_157BC:
  if (ddata.s350.v1e07eb != con(0x0))
  {
    if ((ddata.s350.v1e07d8.b0 != con(0x0)) || (ddata.s350.v1e07d8.b3 != con(0xffffffff)))
    {
      if (SKW_3e74_5673(UI32CAST(ddata.s350.v1e054c & con(0x3ff)) | con(0x20000000), con(0x1), &vw_00) != con(0x0))
      {
        gptrrg5 = SKW_3e74_5817(vw_00);
      }
      else
      {
        gptrrg5 = UPCAST(t_gfxdata, DOWNCAST(s_xmalloc, SKW_ALLOC_CPXHEAP_MEM(vw_00, con(0xe))));
        SKW_3e74_585a(vw_00, true);
      }
      SKW_COPY_MEMORY(DOWNCAST(c_bbbbwwwp, &ddata.s350.v1e07d8), con(0xe), DOWNCAST(t_gfxdata, gptrrg5)); // TODO: special
    }
  }
  if (ddata.s350.v1e0556->w02 != con(0xffffffff))
  {
    SKW_1c9a_0db0(unsignedlong(ddata.s350.v1e054c));
  }
  ddata.s350.v1e0556->w02 = SKW_QUEUE_TIMER(&ddata.s350.v1e0562);
}

void SKW_FILL_CAII_CUR_MAP(void)
{
  x16 wordrg5;
  x16* wptrrg6;
  c_record* recoptrrg4;
  x8* bptrrg7;
  x8* bptrrg1;

  bptrrg7 = *ddata.savegamepp1;
  wptrrg6 = &ddata.v1e038c[*ddata.v1e03f4];
  // M_242C8:
  for (x16 vw_00 = con(0x0); vw_00 < ddata.savegamewa; vw_00++)
  {
    wordrg5 = con(0x0);
    // M_242AF:
    for (;; wordrg5++)
    {
      bool skip = false; // added
      if (wordrg5 >= ddata.savegamewb)
        break;
      bptrrg1 = bptrrg7++;
      if ((*bptrrg1 & con(0x10)) == con(0x0))
        continue;
      x16 wordrg11 = *wptrrg6++;
      while (urshift(wordrg11 & con(0x3c00), con(0xa)) != con(0x4))
      {
        wordrg11 = SKW_GET_NEXT_RECORD_LINK(wordrg11);
        if (wordrg11 == con(0xfffffffe))
        {
          skip = true;
          break;
        }
      }
      if (skip)
        continue;
      x16 wordrg3 = wordrg11;
      recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(wordrg3);
      if (recoptrrg4->u4.b.b1 == con(0xffffffff))
      {
        if ((SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(recoptrrg4->u4.b.b0)->u0.b.b0 & con(0x1)) != con(0x0))
        {
          x16 wordrg21 = recoptrrg4->ua.w;
          SKW_1c9a_09db(recoptrrg4);
          x16 wordrg31 = recoptrrg4->ua.w | (wordrg21 & con(0x6000));
          recoptrrg4->ua.w = wordrg31;
          wordrg21 &= con(0x803f);
          if (wordrg21 == con(0xffff8001))
            recoptrrg4->ua.w = (wordrg31 & con(0x7fc0)) | con(0x8001);
        }
        else
          SKW_ALLOC_CAII_TO_CREATURE(wordrg3, wordrg5, vw_00);
      }
    }
  }
}

void SKW_REFRESH_PLAYER_STAT_DISP(x16 eaxw)
{
  x8 byterg4h;
  c_hero* heroptrrg5;
  t_text t_00[0x80];
  c_rect rc_80;
  bool vbool_88;
  bool vbool_8c;
  t_text* tp_90;
  t_gfxdata* pg_94;
  x16 vw_a0;
  x16 vw_a4;
  x16 vw_a8;

  x16 wordrg34 = con(0x0);
  vbool_88 = false;
  bool vbool_9c = false;
  bool vbool_98 = false;
  vbool_8c = false;
  x16 wordrg27 = eaxw;
  heroptrrg5 = &ddata.hero[wordrg27];
  x16 vw_ac = heroptrrg5->u32.w;
  wordrg27++;
  vw_a4 = (wordrg27 == ddata.v1e0976) ? 1 : 0;
  if (vw_a4 != con(0x0))
  {
    if (ddata.backbuff2 != con(0x0))
      vw_ac |= con(0x8000);
    if ((ddata.v1d26a0 != con(0x0)) || (ddata.v1d26a2 != con(0x0)))
      ddata.v1e100c &= con(0xffffffef);
    else
      ddata.v1e100c |= con(0x10);
  }
  bool skip = false; // added
  if (ddata.v1e0976 == ddata.v1e101e)
    if ((vw_ac & con(0x8000)) == con(0x0))
    {
      if (vw_a4 != con(0x0))
        wordrg34 = ddata.v1e100c ^ ddata.v1e101c;
      skip = true;
    }
  if (!skip)
  {
    if (vw_a4 == con(0x0))
      ddata.v1e101e = vw_a4;
    else
    {
      SKW_DRAW_STATIC_PIC(con(0x7), con(0x0), con(0x4), con(0x0), con(0xffffffff));
      vbool_8c = true;
      vbool_9c = true;
      ddata.v1e101e = ddata.v1e0976;
      wordrg34 = con(0x1f);
      vw_ac |= con(0x7c00);
    }
  }
  if (wordrg34 != con(0x0))
  {
    if ((wordrg34 & con(0x10)) != con(0x0))
      SKW_DRAW_CHARSHEET_OPTION_ICON(con(0x11), con(0x10), con(0x238));
    if (ddata.v1e0288 == con(0x0))
    {
      if ((wordrg34 & con(0x1)) != con(0x0))
      {
        SKW_DRAW_CHARSHEET_OPTION_ICON(con(0x13), con(0x1), con(0x267));
        vw_ac |= con(0x400);
      }
      if ((wordrg34 & con(0x8)) != con(0x0))
        SKW_DRAW_CHARSHEET_OPTION_ICON(con(0xf), con(0x8), con(0x232));
      if ((wordrg34 & con(0x4)) != con(0x0))
        SKW_DRAW_CHARSHEET_OPTION_ICON(con(0xd), con(0x4), con(0x234));
    }
    wordrg34 &= con(0x2);
    if (wordrg34 != con(0x0))
      SKW_DRAW_CHARSHEET_OPTION_ICON(con(0xb), con(0x2), con(0x236));
    vbool_9c = true;
    ddata.v1e101c = ddata.v1e100c;
  }
  if ((vw_ac & con(0x4000)) != con(0x0))
  {
    SKW_DRAW_PLAYER_3STAT_PANE(eaxw, true);
    vbool_98 = true;
    if (heroptrrg5->w36 == con(0x0))
      SKW_DRAW_NAME_STR(&ddata.v1d69d4, unsignedword(ddata.paletteptr1[con(0xf)]), unsignedword(ddata.paletteptr1[con(0x1)]) | con(0x4000), eaxw + con(0xa5), heroptrrg5->t00);
    else
    {
      if (vw_a4 == con(0x0))
        vw_ac |= con(0xc00);
      else
      {
        SKW_2e62_061d(eaxw);
        vw_ac |= con(0x800);
      }
      vbool_88 = true;
    }
  }
  if (heroptrrg5->w36 != con(0x0))
  {
    if ((vw_ac & con(0x800)) != con(0x0))
    {
      SKW_DRAW_PLAYER_3STAT_PANE(eaxw, false);
      vbool_98 = true;
      SKW_DRAW_PLAYER_3STAT_HEALTH_BAR(eaxw);
      if (heroptrrg5->w2e != con(0xffffffff))
        SKW_DRAW_PLAYER_DAMAGE(eaxw);
      if (vw_a4 != con(0x0))
      {
        SKW_QUERY_EXPANDED_RECT(con(0x225), &rc_80);
        pg_94 = ddata.bitmapptr;
//                                           srcbmp                                                    srcofs  destbmp                                palette
        SKW_DRAW_DIALOGUE_PICT(SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x7), con(0x0), con(0x0)), &rc_80, rc_80.x, pg_94, rc_80.y, con(0xffff), SKW_QUERY_GDAT_IMAGE_LOCALPAL(con(0x7), con(0x0), con(0x0)));
        SKW_DRAW_PLAYER_3STAT_TEXT(heroptrrg5);
        if (((heroptrrg5->w44 >= con(0x0)) && (heroptrrg5->w46 >= con(0x0))) && (heroptrrg5->b1f == con(0x0)))
          byterg4h = con(0x4);
        else
          byterg4h = con(0x5);
        SKW_DRAW_EYE_MOUTH_COLORED_RECTANGLE(byterg4h, con(0x221));
        byterg4h = con(0x4);
        x16 wordrg33 = con(0x1);
        while (wordrg33 <= con(0x6))
        {
          if (UI8CAST(heroptrrg5->b4a[2 * wordrg33]) >= UI8CAST(heroptrrg5->b4a[2 * wordrg33 + con(0x1)]))
            wordrg33++;
          else
          {
            byterg4h = con(0x5);
            break;
          }
        }
        SKW_DRAW_EYE_MOUTH_COLORED_RECTANGLE(byterg4h, con(0x222));
        vbool_9c = true;
      }
    }
    if (vw_a4 == con(0x0))
    {
      x16 wordrg32 = con(0x0);
      while (wordrg32 <= con(0x1))
      {
        vbool_98 |= SKW_2e62_03b5(eaxw, vbool_88, wordrg32);
        wordrg32++;
      }
    }
    else
    {
      if (!ddata.v1e0228)
      {
        if (!ddata.v1e0220)
          vw_a8 = heroptrrg5->warrc3[con(0x1)]; // BUGFIX 4/2014
        else
          vw_a8 = con(0xffff);
      }
      else
        vw_a8 = ddata.savegamewpc.w0;
      skip = false; // a 2nd purpose
      if (ddata.v1d66fe == vw_a8 && (ddata.v1e0220 == ddata.v1e1018) && (ddata.v1e0228 == ddata.v1e1014))
        if ((vw_ac & con(0x2000)) == con(0x0))
        {
          if (ddata.v1d66fe != con(0xffffffff) && (SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(ddata.v1d66fe, con(0x0)) & con(0x20)) != con(0x0))
          {
            SKW_DRAW_ITEM_SURVEY(ddata.v1d66fe, ddata.v1e0228);
            vbool_9c = true;
          }
          skip = true;
        }
      if (!skip)
      {
        SKW_DRAW_STATIC_PIC(con(0x7), con(0x25), con(0x221), con(0x0), con(0xffffffff));
        SKW_DRAW_STATIC_PIC(con(0x7), (ddata.v1e0228 ? 1 : 0) + con(0x20), con(0x222), con(0x0), con(0xffffffff));
        if (!ddata.v1e0228)
        {
          if (ddata.v1e0288 == con(0x0))
          {
            if (!SKW_DRAW_ITEM_SURVEY(vw_a8, false))
              SKW_DRAW_FOOD_WATER_POISON_PANEL();
          }
          else
          {
            if (ddata.v1e0220)
              SKW_DRAW_FOOD_WATER_POISON_PANEL();
            else
              SKW_DRAW_CRYOCELL_LEVER(false);
          }
        }
        else
        {
          if (vw_a8 != con(0xffffffff))
            SKW_DRAW_ITEM_SURVEY(vw_a8, true);
          else
          {
            SKW_24a5_1532();
          }
        }
        ddata.v1d66fe = vw_a8;
        ddata.v1e1014 = ddata.v1e0228;
        ddata.v1e1018 = ddata.v1e0220;
        vbool_9c = true;
      }
      x16 wordrg31 = con(0x0);
      while (wordrg31 < con(0x1e))
      {
        vbool_9c |= SKW_2e62_03b5(eaxw, vbool_8c, wordrg31);
        wordrg31++;
      }
    }
    if ((vw_ac & con(0x400)) != con(0x0))
    {
      x16 wordrg152;
      if (eaxw != ddata.mouse_unk0e || ddata.v1e0288 != con(0x0))
        wordrg152 = con(0xf);
      else
        wordrg152 = con(0x9);
      x16 wordrg3 = unsignedword(ddata.paletteptr1[wordrg152]);
      if (vw_a4 == con(0x0))
      {
        SKW_DRAW_PLAYER_3STAT_PANE(eaxw, false);
        vbool_98 = true;
        SKW_DRAW_NAME_STR(&ddata.v1d69d4, wordrg3, unsignedword(ddata.paletteptr1[con(0x1)]) | con(0x4000), eaxw + con(0xa5), heroptrrg5->t00);
      }
      else
      {
        SKW_SK_STRCPY(t_00, heroptrrg5->t00);
        x8 byterg1 = heroptrrg5->t00[0x8];
        if (byterg1 != con(0x2c) && byterg1 != con(0x3b))
          if (byterg1 != con(0x2d))
          {
            SKW_SK_STRCAT(t_00, ddata.v1d70e4);
          }
        SKW_SK_STRCAT(t_00, &heroptrrg5->t00[0x8]);
        tp_90 = t_00;
        x16 wordrg42;
        if (ddata.v1e0288 == con(0x0))
          wordrg42 = con(0x4000);
        else
          wordrg42 = unsignedword(ddata.paletteptr1[con(0x1)]);
        SKW_DRAW_LOCAL_TEXT(con(0x229), wordrg42, tp_90, wordrg3);
        vbool_9c = true;
      }
    }
    if (vw_a4 != con(0x0))
    {
      if ((vw_ac & con(0x1000)) != con(0x0))
      {
        SKW_QUERY_EXPANDED_RECT(con(0x22a), &rc_80);
        pg_94 = ddata.bitmapptr;
//                                        srcbmp                                                      srcofs   destbmp                               palette
        SKW_DRAW_DIALOGUE_PICT(SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x7), con(0x0), con(0x0)), &rc_80, rc_80.x, pg_94, rc_80.y, con(0xffff), SKW_QUERY_GDAT_IMAGE_LOCALPAL(con(0x7), con(0x0), con(0x0)));
        x16 wordrg41 = SKW_GET_PLAYER_WEIGHT(eaxw);
        vw_a0 = wordrg41;
        x16 wordrg21 = SKW_MAX_LOAD(heroptrrg5);
        x16 wordrg61;
        if (wordrg41 <= wordrg21)
        {
          if ((8 * wordrg41) <= (5 * wordrg21))
            wordrg61 = con(0xd);
          else
            wordrg61 = con(0xb);
        }
        else
          wordrg61 = con(0x8);
        ddata.v1e0998 = vw_a0 / con(0xa);
        ddata.v1e0996 = vw_a0 - ddata.v1e0998 * con(0xa);
        ddata.v1e098a = SKW_MAX_LOAD(heroptrrg5) / con(0xa);
        SKW_QUERY_GDAT_TEXT(con(0x7), con(0x2a), t_00, con(0x0));
        SKW_DRAW_LOCAL_TEXT(con(0x22b), ddata.paletteptr1[con(0x1)] | con(0x4000), t_00, ddata.paletteptr1[wordrg61]);
        vbool_9c = true;
      }
    }
  }
  if (vbool_98)
    SKW_0b36_0cbe(&ddata.v1d69d4, true);
  if (vbool_9c)
  {
    ddata.backbuff2 = con(0x0);
    SKW_CHANGE_VIEWPORT_TO_INVENTORY(false);
  }
  heroptrrg5->u32.b.b1 &= con(0x3);
}

x16 R_3D93B(ui16 eaxuw, x16 ebxw, x16* ecxpw, x16 edxw, x16* argpw0)
{
  c_record* recoptrrg1;
  bool flag;
  x8* pb_00;
  x16* pw_04; // add32(0x4, con(0x2));
  x16 vw_08;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_24;

  x16 vw_14 = edxw;
  vw_0c = ebxw;
  x16 vw_28 = con(0x0);
  x16 vw_2c = con(0xffffffff);
  // M_3DB8E:
  for (x16 vw_1c = con(0x0); vw_1c < unsignedword(CUTX8(ddata.savegamep4->w0[con(0x2)])); vw_1c++)
  {
    vw_10 = ulrshift(ddata.v1e03c8[vw_1c].w8, con(0x5), con(0xb));
    vw_24 = urshift(ddata.v1e03c8[vw_1c].w8, con(0xb));
    pb_00 = *(ddata.v1e03cc[vw_1c]);
    pw_04 = &ddata.v1e038c[ddata.v1e03d8[ddata.v1e03e4[vw_1c]]];
    x16 wordrg6 = con(0x0);
    // M_3DB74:
    for (x16 vw_20 = con(0x0); vw_20 <= vw_10; vw_20++)
    {
      // M_3DB59: with co-label M_3DB54
      for (x16 vw_18 = con(0x0); vw_18 <= vw_24; vw_18++)
      {
        x8* hlpptr = pb_00++;
        if ((*hlpptr & con(0x10)) != con(0x0))
        {
          x16 wordrg48 = *pw_04;
          pw_04++;
          // M_3D9FD:
          for(;;)
          {
            if (eaxuw >= UI16CAST(con(0x2)) && eaxuw <= UI16CAST(con(0x5)))
            {
              if (urshift(wordrg48 & con(0x3c00), con(0xa)) == con(0x2) && (recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg48), (recoptrrg1->u2.w & con(0x6)) == con(0x2)))
              {
                x16 wordrg3 = recoptrrg1->u2.w;
                vw_08 = urshift(wordrg3, con(0x3));
                x16 uwordrg28 = urshift(vw_08, con(0x8)) & con(0x1f);
                wordrg6 = vw_08 & con(0xff);
                if (uwordrg28 < con(0xf))
                {
                  if ((uwordrg28 == con(0xb)) && wordrg6 == vw_0c)
                  {
                    if (eaxuw != con(0x3))
                    {
                      wordrg3 = --vw_14;
                      flag = wordrg3 != con(0x0);
                      if (!flag)
                      {
                        *ecxpw = vw_20;
                        *argpw0 = vw_18;
                        return vw_1c;
                      }
                    }
                    else
                      vw_28++;
                  }
                }
                else if (uwordrg28 <= con(0xf))
                {
                  if (eaxuw != con(0x4))
                  {
                    if ((eaxuw == con(0x5)) && ((wordrg3 & con(0x1)) != con(0x0)))
                    {
                      *ecxpw = vw_20;
                      *argpw0 = vw_18;
                      return vw_1c;
                    }
                  }
                  else
                  {
                    wordrg3 &= con(0xfffffffe);
                    recoptrrg1->u2.w = wordrg3;
                  }
                }
                else if (uwordrg28 == con(0x10))
                {
                  if (wordrg6 == vw_0c)
                  {
                    *ecxpw = vw_20;
                    *argpw0 = vw_18;
                    vw_2c = vw_1c;
                  }
                }
              }
            }
            wordrg48 = SKW_GET_NEXT_RECORD_LINK(wordrg48);
            if (wordrg48 == con(0xfffffffe) || urshift(wordrg48 & con(0x3c00), con(0xa)) > con(0x3))
              break;
          }
        }
      }
    }
  }
  SKW_CHANGE_CURRENT_MAP_TO(ddata.v1e0266);
  if (eaxuw != con(0x4))
    return (eaxuw != con(0x5)) ? vw_28 : con(0xffffffff);
  return vw_2c;
}

x16 SKW_14cd_102e(x16 eaxw, x8 ebxb, bool ecxbool, x16 edxw, bool argbool0)
{
  x16 wordrg5;
  x16 vw_04;
  x8 vb_08;

  wordrg5 = edxw;
  x8 byterg2 = ebxb;
  vb_08 = byterg2;
  x16 wordrg6 = con(0x0);
  for(;;)
  {
    if (wordrg5 == con(0xfffffffe))
      return wordrg6;
    vw_04 = urshift(wordrg5 & con(0x3c00), con(0xa));
    if ((ecxbool && vw_04 == con(0x4)) || (argbool0 && SKW_IS_CONTAINER_CHEST(wordrg5)))
    {
      byterg2 = vb_08;
      wordrg6 += SKW_14cd_102e(eaxw, byterg2, ecxbool, SKW_GET_ADDRESS_OF_RECORD(wordrg5)->u2.w, argbool0);
    }
    if (vw_04 < con(0x4) && vw_04 < con(0xe) && (vb_08 == con(0xffffffff) || vb_08 == CUTX8(urshift(wordrg5, con(0xe)))))
      if (SKW_1c9a_0006(wordrg5, eaxw) != con(0x0))
        wordrg6++;
    wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg5);
  }
}

// adress given to a functionpointer
bool R_1BAAD(x16 eaxw, x16 edxw)
{
  x16 vw_00;
  x16 vw_04;

  vw_04 = eaxw;
  vw_00 = edxw;
  x8 byterg18 = SKW_GET_TILE_VALUE(vw_04, vw_00);
  x16 wordrg2 = unsignedword(byterg18);
  x16 wordrg31 = unsignedword(ubrshift(byterg18, con(0x5)));
  if (wordrg31 == con(0x0))
    return true;
  if (wordrg31 == con(0x4))
  {
    x16 byterg15 = CUTX8(wordrg2) & con(0x7);
    if (byterg15 == con(0x3) || byterg15 == con(0x4))
      if (SKW_0cee_3275(SKW_IS_REBIRTH_ALTAR(SKW_GET_ADDRESS_OF_TILE_RECORD(vw_04, vw_00))) == con(0x0) || SKW_RAND01() == con(0x0))
        return true;
  }
  if (wordrg31 == con(0x6))
    if ((wordrg2 & con(0x4)) == con(0x0))
      return true;
  if ((CUTX8(wordrg2) & con(0x10)) == con(0x0))
    return false;
  x16 wordrg1 = SKW_GET_WALL_TILE_ANYITEM_RECORD(vw_04, vw_00);
  x16 wordrg3;
  for (;; wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg3))
  {
    wordrg3 = wordrg1;
    if (wordrg3 == con(0xfffffffe))
      return false;
    x16 wordrg45 = urshift(wordrg3 & con(0x3c00), con(0xa));
    if (wordrg45 == con(0xf))
      if ((SKW_GET_ADDRESS_OF_RECORD(wordrg3)->u2.w & con(0x7f)) == con(0xe))
        return true;
    if (wordrg45 == con(0x4))
    {
      x16 wordrg43 = SKW_1c9a_03cf(&vw_04, con(0xff), &vw_00);
      if (wordrg43 != con(0xffffffff))
      {
        x16 wordrg100 = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg43);
        if ((wordrg100 & con(0x1)) == con(0x0))
        {
          if ((wordrg100 & con(0x20)) == con(0x0))
            return true;
        }
        else if ((urshift(wordrg100, con(0x6)) & con(0x3)) < con(0x2))
          return true;
      }
    }
  }
}

bool SKW_19f0_05e8(x16 eaxw, x16* ebxpw, x16 ecxw, t_gfxdata* edxpg, x16 argw0, x16 argw1, bool argbool2)
{
  x16 wordrg5;
  t_gfxdata* gptrrg5;
  x16* pw_04;
  t_gfxdata* pg_08;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_18;
  x16 vw_24;

  vw_0c = eaxw;
  pg_08 = edxpg;
  pw_04 = ebxpw;
  x16 vw_00 = ecxw;
  x16 wordrg6 = argw1;
  x16 wordrg4;
  if ((ddata.s350.v1e0578 & con(0xff8)) == con(0x0))
    wordrg4 = con(0x1);
  else
    wordrg4 = urshift(ddata.s350.v1e0552->w14, con(0xc));
  vw_24 = wordrg4;
  vw_10 = con(0x0);
  if (wordrg6 == con(0xffffffff))
  {
    wordrg6 = con(0x0);
    vw_10 = con(0x3);
  }
  // M_1C1A8:
  x16 vw_1c;
  x16 vw_14;
  for(;;)
  {
    vw_18 = vw_00;
    vw_14 = argw0;
    // M_1C365: co-label M_1C362
    for (vw_1c = con(0x0); vw_1c < vw_24; vw_1c++)
    {
      x16 wordrg24 = vw_18 + table1d27fc[wordrg6];
      vw_18 = wordrg24;
      vw_14 += table1d2804[wordrg6];
      if (wordrg24 < con(0x0) || wordrg24 >= ddata.savegamewa)
        continue;
      if (vw_14 < con(0x0) || vw_14 >= ddata.savegamewb)
        continue;
      x32 longrg22 = signedlong(vw_14);
      wordrg5 = vw_18;
      x8 byterg14 = SKW_GET_TILE_VALUE(wordrg5, CUTX16(longrg22));
      if ((byterg14 & con(0x10)) == con(0x0))
      {
        ;
      }
      else if (ubrshift(byterg14, con(0x5)) != con(0x0))
      {
        if (argbool2)
        {
          bool boolrg1;
          wordrg5 = vw_18;
          if (unsignedword(ddata.s350.v1e0562.u.b.b_06) != wordrg5)
            boolrg1 = true;
          else
          {
            if (unsignedword(ddata.s350.v1e0562.u.b.b_07) == vw_14)
              boolrg1 = false;
            else
              boolrg1 = true;
          }
          if (boolrg1)
          {
            wordrg5 = SKW_GET_CREATURE_AT(vw_18, vw_14);
            if (wordrg5 != con(0xffffffff))
            {
              if (SKW_19f0_0547(wordrg5, vw_0c) != con(0x0))
              {
                goto M_false;
              }
            }
          }
        }
      }
      else if (argbool2)
      {
        x16 wordrg145 = SKW_GET_TILE_RECORD_LINK(wordrg5, CUTX16(longrg22));
        // M_1C25D: co-label M_1C2CD
        for (;; wordrg145 = SKW_GET_NEXT_RECORD_LINK(wordrg5))
        {
          wordrg5 = wordrg145;
          if (wordrg5 == con(0xfffffffe))
            break;
          x16 wordrg144 = urshift(wordrg5 & con(0x3c00), con(0xa));
          if (wordrg144 != con(0x3) && wordrg144 != con(0x2))
            continue;
          if (urshift(wordrg5, con(0xe)) != ((wordrg6 + con(0x2)) & con(0x3)))
            continue;
          x8 byterg17 = SKW_QUERY_CLS2_FROM_RECORD(wordrg144);
          if (byterg17 == con(0xffffffff))
            continue;
          if (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0xf0), byterg17) == vw_0c)
            goto M_false;
        }
      }
      else
        break;
      wordrg5 = vw_14;
      if ((pg_08[(vw_18 << con(0x7)) + 4 * wordrg5] != con(0x0)) || R_1BC29(vw_18, vw_14))
        break;
    }
    wordrg6++;
    wordrg5 = vw_10 - con(0x1);
    vw_10 = wordrg5;
    if (wordrg5 < con(0x0))
      return false;
  }

M_false: // M_1C38A:
  vw_10 = pg_08[(signedlong(vw_00) << con(0x7)) + 4 * signedlong(argw0)];
  wordrg6 += con(0x2);
  wordrg6 &= con(0x3);
  for(;;)
  {
    if (++vw_1c >= vw_24)
      break;
    x16 vw_28 = vw_00;
    x16 vw_20 = argw0;
    vw_00 += table1d27fc[wordrg6];
    argw0 += table1d2804[wordrg6];
    x16 wordrg101 = vw_00;
    gptrrg5 = &pg_08[wordrg101 << con(0x7)];
    x8 byterg2 = gptrrg5[4 * signedlong(argw0)];
    if (byterg2 == con(0x0) || unsignedword(byterg2) > vw_10 || R_1BC29(wordrg101, argw0))
    {
      vw_00 = vw_28;
      argw0 = vw_20;
      break;
    }
  }
  wordrg6 += con(0x2);
  wordrg6 &= con(0x3);
  if (SKW_19f0_0891((argbool2 ? 1 : 0) + con(0x2), argw0, vw_18, vw_00, vw_14, wordrg6))
  {
    SKW_19f0_045a(vw_00, argw0);
    *pw_04 &= con(0xffffffe0); // was a byte-access
    wordrg5 = *pw_04;
    wordrg5 |= vw_18 & con(0x1f);
    wordrg6 = wordrg5 & con(0xfffffc1f);
    *pw_04 = wordrg6 | ((vw_14 & con(0x1f)) << con(0x5));
    *pw_04 &= con(0x3ff); // was a highbyte-access, mask adj
    *pw_04 |= (ddata.v1d3248 & con(0x3f)) << con(0xa);
    return true;
  }
  return false;
}

x16 SKW_19f0_124b(x16* eaxpw, x16 ebxw, x16 ecxw, x16* edxpw, x16 argw0)
{
  bool flag;
  c_record* prec_00;
  x16 vw_04;
  x16 vw_08; // mod, was x32
  x16 vw_0c;

  vw_04 = ebxw;
  vw_0c = ecxw;
  bool boolrg7 = false;
  vw_08 = con(0xffffffff);
  SKW_CHANGE_CURRENT_MAP_TO(vw_04);
  x16 wordrg1 = unsignedword(SKW_GET_TILE_VALUE(*eaxpw, *edxpw));
  x8 byterg2 = CUTX8(urshift(wordrg1, con(0x5)));
  if (byterg2 != con(0x3))
  {
    if (byterg2 != con(0x2) || (argw0 & con(0x8)) == con(0x0) || ecxw != con(0x1) || (wordrg1 & con(0x8)) == con(0x0) || (wordrg1 & con(0x1)) != con(0x0))
    {
      wordrg1 &= con(0x2);
      if (wordrg1 == con(0x0) || byterg2 == con(0x0) || byterg2 == con(0x7) || byterg2 == con(0x4))
        return vw_08;
      bool skip = false; // added
      if ((argw0 & con(0x100)) != con(0x0))
        if (SKW_FIND_LADDAR_AROUND(*eaxpw, vw_0c, &prec_00, *edxpw) >= con(0x0))
          skip = true;
      if (!skip)
      {
        if ((argw0 & con(0x10)) == con(0x0) || vw_0c != con(0xffffffff))
          return vw_08;
        boolrg7 = true;
      }
    }
  }
  else
  {
    if ((argw0 & con(0x100)) == con(0x0))
      return vw_08;
    wordrg1 &= con(0x4);
    if (wordrg1 == con(0x0))
      flag = ecxw == con(0x1);
    else
      flag = ecxw == con(0xffffffff);
    if (!flag)
      return vw_08;
  }
  vw_08 = SKW_LOCATE_OTHER_LEVEL(vw_04, eaxpw, edxpw, vw_0c, NULL);
  if (vw_08 < con(0x0) || !boolrg7)
    return vw_08;
  SKW_CHANGE_CURRENT_MAP_TO(vw_08);
  x16 wordrg12 = unsignedword(SKW_GET_TILE_VALUE(*eaxpw, *edxpw));
  if (CUTX8(urshift(wordrg12, con(0x5))) != con(0x2) || (wordrg12 & con(0x8)) == con(0x0) || (wordrg12 & con(0x1)) != con(0x0))
    vw_08 = con(0xffffffff);
  SKW_CHANGE_CURRENT_MAP_TO(vw_04);
  return vw_08;
}

// like other 2fcf-routines arguments are stored as byte, but given as words
void SKW_2fcf_0234(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  x16 wordrg5;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;
  x8 vba_10[0x4];
  x8 vba_14[0x4];
  x16 vw_20;
  x16 vw_24;

  vw_0c = eaxw;
  vw_08 = edxw;
  vw_04 = ebxw;
  bool boolrg7 = false;
  DM2_ZERO_MEMORY(DOWNCAST(x8, vba_10), con(0x4));
  for (x16 vw_28 = con(0x0); vw_28 < con(0x4); vw_28++)
    if (SKW_GET_PLAYER_AT_POSITION(vw_28) >= con(0x0))
      vba_10[vw_28] = CUTX8(vw_28) + con(0x1);
  if (vw_04 >= con(0x0))
  {
    wordrg5 = vw_04;
    if (SKW_CALC_SQUARE_DISTANCE(vw_0c, wordrg5, ecxw, vw_08) == con(0x1))
    {
      x16 wordrg2 = SKW_CALC_VECTOR_DIR(vw_0c, wordrg5, ecxw, vw_08);
      x16 vw_28_1 = wordrg2;
      x16 wordrg3 = (wordrg2 + con(0x1)) & con(0x3);
      DM2_ZERO_MEMORY(DOWNCAST(x8, vba_14), con(0x4));
      vba_14[(wordrg2 + con(0x3)) & con(0x3)] = vba_10[wordrg2];
      if (vba_10[wordrg2] != con(0x0))
        boolrg7 = true;
      vba_14[(wordrg3 + con(0x1)) & con(0x3)] = vba_10[wordrg3];
      if (vba_10[wordrg3] != con(0x0))
        boolrg7 = true;
      if (vba_10[vw_28_1] == con(0x0))
        vba_10[vw_28_1] = vba_10[(vw_28_1 + con(0x3)) & con(0x3)];
      if (vba_10[wordrg3] == con(0x0))
        vba_10[wordrg3] = vba_10[(wordrg3 + con(0x1)) & con(0x3)];
    }
  }
  vw_20 = vw_0c;
  vw_24 = vw_08;
  // M_3A2EA:
  for(;;)
  {
    x16 wordrg100 = SKW_GET_TILE_RECORD_LINK(vw_20, vw_24);
    // M_3A2FD: co-label M_3A385:
    x16 wordrg61;
    for (;; wordrg100 = SKW_GET_NEXT_RECORD_LINK(wordrg61))
    {
      wordrg61 = wordrg100;
      if (wordrg61 == con(0xfffffffe))
      {
        if (!boolrg7)
          return;
        vw_20 = vw_04;
        vw_0c = vw_0c | ((vw_20 << con(0x8)) + con(0x100));
        vw_24 = ecxw;
        vw_08 = vw_08 | (vw_24 << con(0x8));
        SKW_COPY_MEMORY(DOWNCAST(x8, vba_14), con(0x4), DOWNCAST(x8, vba_10));
        boolrg7 = false;
      }
      else
      {
        if (urshift(wordrg61 & con(0x3c00), con(0xa)) != con(0xe))
          continue;
        wordrg5 = wordrg61;
        x16 wordrg42 = SKW_GET_ADDRESS_OF_RECORD(wordrg5)->u6.w;
        if (ddata.savegametimarrp[wordrg42].b_04 == con(0x1d))
          continue;
if (wordrg5 < 0)
  int stop=1;
        x16 wordrg123 = unsignedword(vba_10[wordrg5 >> con(0xe)]); // TODO BUG possible, value 0xf800 in wordrg5 found!
        // vw_28 = wordrg123;  what for?
        if (wordrg123 == con(0x0))
          continue;
        wordrg123--;
        if (!SKW_075f_0af9(con(0xfffffffd), vw_08, wordrg123, vw_0c, wordrg5))
          continue;
        SKW_075f_056c(wordrg5);
      }
      break;
    }
  }
}

void SKW_ARRANGE_DUNGEON(void)
{
  x16 wordrg5;
  c_record* recoptrrg1;
  c_record* recoptrrg2;
  c_record* recoptrrg3;
  c_record* recoptrrg4;
  c_record* recoptrrg6;
  c_aispec* aiptrrg1;
  x16* pw_00;
  x8* pb_04;
  x16 vw_08;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_14;
  x16 vw_18;
  x16 vw_1c;
  x8 vb_2c;

  // M_2686A:
  for (x16 vw_24 = con(0x0); vw_24 < unsignedword(CUTX8(ddata.savegamep4->w0[con(0x2)])); vw_24++)
  {
    vw_18 = ulrshift(ddata.v1e03c8[vw_24].w8, con(0x5), con(0xb));
    vw_1c = urshift(ddata.v1e03c8[vw_24].w8, con(0xb));
    pb_04 = *(ddata.v1e03cc[vw_24]);
    pw_00 = &ddata.v1e038c[ddata.v1e03d8[ddata.v1e03e4[vw_24]]];
    // M_2684F:
    for (x16 vw_28 = con(0x0); vw_28 <= vw_18; vw_28++)
    {
      wordrg5 = con(0x0);
      // M_26834:
      for (x16 vw_20 = con(0x0); vw_20 <= vw_1c; vw_20++)
      {
        vb_2c = *pb_04++;
        if ((vb_2c & con(0x10)) != con(0x0))
        {
          wordrg5 = *pw_00++;
          // M_263B9:
          for(;;)
          {
            bool skip = false;
            ui16 uwordrg10 = urshift(wordrg5 & con(0x3c00), con(0xa)) - con(0x1);
            if (uwordrg10 <= con(0x3))
            {
              switch (uwordrg10)
              {
                case 0:
                  recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(wordrg5);
                  recoptrrg6 = recoptrrg4;
                  SKW_CHANGE_CURRENT_MAP_TO(urshift(recoptrrg4->u4.w, con(0x8)));
                  vw_14 = SKW_GET_TILE_RECORD_LINK(recoptrrg4->u2.w & con(0x1f), ulrshift(recoptrrg4->u2.w, con(0x6), con(0xb)));
                  if (vw_14 != con(0xfffffffe))
                  {
                    if (urshift(vw_14 & con(0x3c00), con(0xa)) == con(0x1))
                    {
                      recoptrrg3 = SKW_GET_ADDRESS_OF_RECORD(vw_14);
                      if ((recoptrrg3->u2.w & con(0x1f)) == vw_28)
                      {
                        if (ulrshift(recoptrrg3->u2.w, con(0x6), con(0xb)) == vw_20)
                        {
                          if (urshift(recoptrrg3->u4.w, con(0x8)) == vw_24)
                          {
                            vw_08 = con(0x0);
                            bool boolrg7 = SKW_2066_1f37(vw_14, &vw_08, urshift(recoptrrg6->u4.w, con(0x8)));
                            boolrg7 |= SKW_2066_1f37(wordrg5, &vw_08, vw_24);
                            if (vw_08 != con(0x2))
                            {
                              if (vw_08 == con(0x0) && boolrg7)
                                skip = true;
                            }
                            else
                              recoptrrg6->w0 = recoptrrg3->w0 = SKW_2066_1ec9(SKW_GET_NEXT_RECORD_LINK(wordrg5), SKW_GET_NEXT_RECORD_LINK(vw_14));
                          }
                        }
                      }
                    }
                  }
                break;

                case 1:
                  recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg5);
                  if ((recoptrrg1->u2.w & con(0x6)) == con(0x2))
                  {
                    x16 wordrg3 = recoptrrg1->u2.w;
                    ui16 uwordrg2 = urshift(urshift(wordrg3, con(0x3)), con(0x8)) & con(0x1f);
                    if (uwordrg2 >= con(0x6))
                    {
                      if (uwordrg2 <= con(0x7))
                      {
                        x16 wordrg112 = urshift(wordrg5, con(0xe)); // unsigned-extension necessary! BUGFIX 4/2014
                        x16 wordrg411 = vw_20 + table1d2804[wordrg112];
                        wordrg3 = vw_28;
                        SKW_2066_1ea3(wordrg3 + table1d27fc[wordrg112], vw_24, wordrg411);
                      }
                      else if (uwordrg2 == con(0x8))
                        recoptrrg1->u2.w = wordrg3 | con(0x1);
                    }
                  }
                break;

                case 2:
                  {
                    recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(wordrg5);
                    recoptrrg4 = recoptrrg2;
                    x16 wordrg111 = recoptrrg2->u2.w & con(0x7f);
                    if (wordrg111 < con(0x2c))
                    {
                      if (wordrg111 >= con(0x1c))
                      {
                        if (wordrg111 <= con(0x1c))
                        {
                          x16 wordrg105 = vw_28 + table1d27fc[wordrg5 >> con(0xe)];
                          SKW_2066_1ea3(wordrg105, vw_24, vw_20 + table1d2804[wordrg105]);
                        }
                        else if (wordrg111 == con(0x27))
                        {
                          if ((recoptrrg4->u4.b.b0 & con(0x4)) != con(0x0))
                            recoptrrg4->u4.b.b0 |= con(0x1);
                        }
                      }
                    }
                    else if (wordrg111 <= con(0x2c))
                    {
                      if ((recoptrrg4->u4.b.b0 & con(0x4)) != con(0x0))
                        recoptrrg4->u4.b.b0 |= con(0x1);
                    }
                    else
                    {
                      if (wordrg111 < con(0x3f))
                      {
                        ;
                      }
                      else if (wordrg111 <= con(0x3f))
                        recoptrrg2->u4.b.b0 = (recoptrrg2->u4.b.b0 & con(0xfffffffa)) | con(0x4);
                      else if (wordrg111 == con(0x46))
                      {
                        SKW_CHANGE_CURRENT_MAP_TO(vw_24);
                        x16 wordrg416 = urshift(recoptrrg2->u6.w, con(0xb));
                        if ((((SKW_GET_TILE_VALUE(ulrshift(recoptrrg2->u6.w, con(0x5), con(0xb)), wordrg416) & con(0xff)) >> con(0x5)) & con(0xff)) == con(0x4))
                        {
                          x16 wordrg415 = urshift(recoptrrg2->u6.w, con(0xb));
                          recoptrrg1 = SKW_GET_ADDRESS_OF_TILE_RECORD(ulrshift(recoptrrg2->u6.w, con(0x5), con(0xb)), wordrg415);
                          x16 wordrg414 = ((ulrshift(recoptrrg2->u4.w, con(0xd), con(0xf))) & con(0x1)) << con(0xd);
                          recoptrrg1->u2.b.b1 &= con(0xffffffdf);
                          recoptrrg1->u2.w |= wordrg414;
                        }
                      }
                    }
                  }
                break;

                case 3:
                  recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(wordrg5);
                  recoptrrg3 = recoptrrg4;
                  aiptrrg1 = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(recoptrrg4->u4.b.b0);
                  recoptrrg4->u6.w = aiptrrg1->w4;
                  if ((aiptrrg1->u0.b.b0 & con(0x1)) != con(0x0))
                  {
                    recoptrrg4->ua.w = con(0x0);
                    recoptrrg4->w8 = recoptrrg4->ua.w;
                    if ((recoptrrg4->ue.b.b0 & con(0xffffff80)) == con(0x0))
                    {
                      x16 wordrg22 = recoptrrg4->u2.w;
                      if (wordrg22 != con(0xfffffffe))
                      {
                        recoptrrg3->u2.w = (wordrg22 & con(0x3fff)) | (SKW_RAND02() << con(0xe));
                        for(;;)
                        {
                          if (wordrg22 == con(0xfffffffe))
                            break;
                          recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(wordrg22);
                          if (recoptrrg4->w0 != con(0xfffffffe))
                            recoptrrg4->w0 = (recoptrrg4->w0 & con(0x3fff)) | (SKW_RAND02() << con(0xe));
                          wordrg22 = SKW_GET_NEXT_RECORD_LINK(wordrg22);
                        }
                      }
                    }
                  }
                  else
                  {
                    recoptrrg4->ua.w = con(0x0);
                    recoptrrg4->uc.b.b0 &= con(0xffffffe0);
                    x16 wordrg21 = recoptrrg4->uc.w | (vw_28 & con(0x1f));
                    recoptrrg4->uc.w = (wordrg21 & con(0xfffffc1f)) | ((vw_20 & con(0x1f)) << con(0x5));
                    recoptrrg4->uc.b.b1 &= con(0x3);
                    recoptrrg4->uc.w |= (vw_24 & con(0x3f)) << con(0xa);
                  }
                break;

                default: throw(THROW_DMABORT);
              }
            }
            if (skip)
              break;
            wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg5);
            if (wordrg5 == con(0xfffffffe))
              break;
          }
        }
        if ((ubrshift(vb_2c, con(0x5)) & con(0xff)) == con(0x2))
        {
          if (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x8), con(0xb), con(0x6a), CUTX8(ulrshift(ddata.v1e03c8[vw_24].we, con(0x8), con(0xc)))) == con(0x0))
          {
            vw_10 = vw_28;
            vw_0c = vw_20;
            SKW_2066_1ea3(vw_10, SKW_LOCATE_OTHER_LEVEL(vw_24, &vw_10, &vw_0c, con(0x1), NULL), vw_0c);
          }
        }
      }
    }
  }
}

void SKW_LOAD_PROJECTILE_TO_HAND(x16 eaxw, x16 edxw)
{
  x16 wordrg5;
  x16* wptrrg7;
  c_hero* heroptrrg3;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;
  x16 help; // added

  heroptrrg3 = &ddata.hero[eaxw];
  help = signedlong(edxw);
  heroptrrg3->b2a[help] = con(0x0);
  if (heroptrrg3->w36 == con(0x0) || edxw >= con(0x2))
    return;
  vw_04 = edxw ^ con(0x1);
  vw_08 = edxw;
  x16 wordrg2 = unsignedword(heroptrrg3->b20[help]);
  vw_0c = wordrg2;
  heroptrrg3->b20[help] = con(0xffffffff);
  heroptrrg3->b42[help] = con(0x0);
  help = con(0x18);
  if (wordrg2 != con(0x20))
  {
    if (wordrg2 != con(0x2a) || (heroptrrg3->warrc3[edxw] != con(0xffffffff))) // BUGFIX 3/2014 no 'help' here, BUGFIX 4/2014 ...but edxw
      return;
    wordrg5 = con(0xc);
    if (!SKW_2759_0e93(eaxw, vw_0c, heroptrrg3->warrc3[help / 2])) // BUGFIX 3/2014
    {
      wordrg5 = heroptrrg3->warrc3[0xc];
      x16 wordrg412 = wordrg5;
      if (!SKW_IS_CONTAINER_CHEST(wordrg412))
        goto M_exit2;
      wptrrg7 = &SKW_GET_ADDRESS_OF_RECORD(wordrg412)->u2.w;
      wordrg5 = *wptrrg7;
      x16 wordrg61;
      for(;;)
      {
        if (wordrg5 == con(0xfffffffe))
          goto M_exit2;
        wordrg61 = wordrg5;
        if (SKW_2759_0e93(eaxw, vw_0c, wordrg61))
          break;
        wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg61);
      }
      SKW_CUT_RECORD_FROM(wordrg61, con(0xffffffff), con(0x0), wptrrg7);
    }
    else
      wordrg5 = SKW_REMOVE_POSSESSION(eaxw, wordrg5);
    SKW_EQUIP_ITEM_TO_HAND(eaxw, vw_08, wordrg5);
    return;
  }
  if (heroptrrg3->warrc3[vw_04] != con(0xffffffff))
    return;
  wordrg5 = con(0xc);
  if (!SKW_IS_MISSILE_VALID_TO_LAUNCHER(eaxw, heroptrrg3->warrc3[help / 2], edxw)) // BUGFIX 4/2014
  {
    wordrg5 = heroptrrg3->warrc3[0xc];
    x16 wordrg411 = unsignedlong(wordrg5);
    if (!SKW_IS_CONTAINER_CHEST(wordrg411))
      goto M_exit1;
    wptrrg7 = &SKW_GET_ADDRESS_OF_RECORD(wordrg411)->u2.w;
    wordrg5 = *wptrrg7;
    x16 wordrg62;
    for(;;)
    {
      if (wordrg5 == con(0xfffffffe))
        goto M_exit1;
      wordrg62 = wordrg5;
      if (SKW_IS_MISSILE_VALID_TO_LAUNCHER(eaxw, wordrg62, vw_08))
        break;
      wordrg5 = SKW_GET_NEXT_RECORD_LINK(wordrg62);
    }
    SKW_CUT_RECORD_FROM(wordrg62, con(0xffffffff), con(0x0), wptrrg7);
    SKW_EQUIP_ITEM_TO_HAND(eaxw, vw_04, wordrg5);
  }
  else
  {
    wordrg5 = SKW_REMOVE_POSSESSION(eaxw, wordrg5);
    SKW_EQUIP_ITEM_TO_HAND(eaxw, vw_04, wordrg5);
  }
  return;

M_exit1: // M_30C90:
  wordrg5 = con(0x7);
  for(;;)
  {
    if (wordrg5 > con(0x9))
      return;
    if (SKW_IS_MISSILE_VALID_TO_LAUNCHER(eaxw, heroptrrg3->warrc3[wordrg5], vw_08))
      break;
    wordrg5++;
  }
  wordrg5 = SKW_REMOVE_POSSESSION(eaxw, wordrg5);
  SKW_EQUIP_ITEM_TO_HAND(eaxw, vw_04, wordrg5);
  return;

M_exit2: // M_30D90:
  wordrg5 = con(0x7);
  while (wordrg5 <= con(0x9))
  {
    if (SKW_2759_0e93(eaxw, vw_0c, heroptrrg3->warrc3[wordrg5]))
    {
      wordrg5 = SKW_REMOVE_POSSESSION(eaxw, wordrg5);
      SKW_EQUIP_ITEM_TO_HAND(eaxw, vw_08, wordrg5);
      return;
    }
    wordrg5++;
  }
  wordrg5 = SKW_REMOVE_POSSESSION(eaxw, con(0xc));
  if (wordrg5 != con(0xffffffff))
    SKW_EQUIP_ITEM_TO_HAND(eaxw, vw_08, wordrg5);
}

x16 SKW_14cd_0067(const s_wpm* eaxps)
{
  const s_wpm* sptrrg5;
  c_record* recoptrrg7;
  s_d556* pd556_00;
  x16 vw_04;
  x16 vw_08;

  sptrrg5 = eaxps;
  pd556_00 = ddata.s350.v1e0556;
  recoptrrg7 = ddata.s350.v1e054e;
  x16 wordrg2 = recoptrrg7->ua.w;
  x16 wordrg32 = SKW_RAND();
  x16 wordrg181;
  if (ddata.s350.v1e0571 != ddata.v1e08d6)
  {
    if (pd556_00->u.v.b12 == con(0xffffffff))
      wordrg181 = con(0x30);
    else
      wordrg181 = con(0x70);
    if ((wordrg181 & wordrg32) == con(0x0))
    {
      wordrg2 |= con(0x8000);
      wordrg2 &= con(0xbfff);
    }
  }
  else
    wordrg2 &= con(0x7fff);
  if ((wordrg2 & con(0x8000)) == con(0x0))
  {
    x16 wordrg147;
    if ((wordrg2 & con(0x8)) == con(0x0))
      wordrg147 = con(0xf80);
    else
      wordrg147 = con(0x380);
    if ((wordrg147 & wordrg32) == con(0x0))
      wordrg2 ^= con(0x4000);
  }
  x16 wordrg6 = con(0x10) - (urshift(ddata.s350.v1e0552->w16, con(0x4)) & con(0xf));
  if ((wordrg32 % wordrg6) == con(0x0))
    wordrg2 &= con(0xffffffdf);
  if ((table1d607e[ddata.s350.v1e0584].u.b[1] & con(0x4)) == con(0x0))
  {
    if ((wordrg32 & con(0x38)) == con(0x0))
      wordrg2 &= con(0xdfff);
  }
  else
    wordrg2 |= con(0x2000);
  if ((wordrg32 & con(0x3000)) == con(0x0))
    wordrg2 &= con(0xffffffef);
  if ((wordrg32 & con(0x3)) == con(0x0))
    wordrg2 &= con(0xffffffbf);
  wordrg32 &= con(0x8008);
  if (wordrg32 == con(0x0))
    wordrg2 &= con(0xefff);
  x16 wordrg62 = con(0x2);
  if ((table1d607e[ddata.s350.v1e0584].u.b[0] & con(0x2)) == con(0x0))
    wordrg62 *= 2 * ((ddata.s350.v1e054c & con(0x3)) + con(0x1)) - con(0x1);
  if (SKW_RAND16(wordrg62) == con(0x0))
  {
    wordrg62 = DM2_MAX(con(0x1), ddata.s350.v1e0552->w4);
    if (UI32CAST(100 * recoptrrg7->u6.w / wordrg62) >= UI32CAST(con(0x19)))
      wordrg2 &= con(0xfffffff7);
    else
      wordrg2 |= con(0x8);
  }
  recoptrrg7->ua.w = wordrg2 & con(0xfffffffd);
  vw_08 = con(0xffffffff);
  x16 wordrg63 = con(0xffffffff);
  x16 wordrg3 = con(0x0);
  // M_15C9F: co-label M_15C9B
  x16 wordrg44;
  for (;; wordrg3++, sptrrg5++)
  {
    wordrg44 = sptrrg5->w0;
    if (wordrg44 != con(0x0))
    {
      vw_04 = wordrg44 & con(0xc000);
      if (vw_04 != con(0xffffc000))
      {
        if (vw_08 == con(0xffffffff))
        {
          if ((wordrg2 & wordrg44) != con(0x0))
            vw_08 = wordrg3;
        }
        if (wordrg63 == con(0xffffffff))
        {
          if (wordrg44 == (wordrg2 & wordrg44))
            wordrg63 = wordrg3;
        }
        if (wordrg44 != wordrg2)
          continue;
      }
      else
      {
        vw_04 = wordrg44 & con(0x3fff);
        if (SKW_GET_GLOB_VAR(vw_04) == con(0x0))
          continue;
      }
    }
    break;
  }
  if (wordrg44 == con(0x0) && (wordrg63 != con(0xffffffff) || vw_08 != con(0xffffffff)))
  {
    if (wordrg63 != con(0xffffffff))
      wordrg3 = wordrg63;
    else
      wordrg3 = vw_08;
  }
  if (wordrg3 != signedword(pd556_00->u.v.barr16[con(0x0)]))
  {
    ddata.s350.v1e07d8.b1 = con(0x0);
    pd556_00->u.v.b12 = con(0xffffffff);
    pd556_00->u.v.b13 = con(0x0);
    ddata.s350.v1e07d8.b3 = con(0xffffffff);
  }
  pd556_00->u.v.barr16[con(0x0)] = CUTX8(wordrg3);
  return signedword(CUTX8(wordrg3));
}

bool SKW_CREATURE_ATTACKS_PARTY(void)
{
  x16 wordrg5;
  c_hero* heroptrrg5;
  c_aispec* aiptrrg5;
  x8 vba_00[0x4];
  s_d556* pd556_04;
  x16 vw_0c;
  x16 vw_1c;

  pd556_04 = ddata.s350.v1e0556;
  aiptrrg5 = ddata.s350.v1e0552;
  x16 vw_14 = ddata.v1e08a6;
  x16 vw_10 = ddata.v1e08a4;
  vw_0c = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
  if (pd556_04->u.v.b20 == con(0x6))
    vw_0c = (vw_0c + con(0x2)) & con(0x3);
  bool boolrg7;
  if (unsignedword(ddata.s350.v1e0562.u.b.b_06) == vw_14 && (unsignedword(ddata.s350.v1e0562.u.b.b_07) == vw_10))
    boolrg7 = true;
  else
    boolrg7 = false;
  if (!boolrg7)
  {
    if (DM2_ABS(unsignedword(ddata.s350.v1e0562.u.b.b_07) - vw_10) + DM2_ABS(unsignedword(ddata.s350.v1e0562.u.b.b_06) - vw_14) > con(0x1))
      return true;
    if (SKW_CALC_VECTOR_DIR(unsignedword(ddata.s350.v1e0562.u.b.b_06), vw_14, vw_10, unsignedword(ddata.s350.v1e0562.u.b.b_07)) != vw_0c && (aiptrrg5->u0.b.b0 & con(0x4)) == con(0x0))
      return true;
  }
  if (ddata.v1d3248 != ddata.v1e08d6 || vw_14 != ddata.v1e08d8 || vw_10 != ddata.v1e08d4)
  {
    if ((((SKW_GET_TILE_VALUE(vw_14, vw_10) & con(0xff)) >> con(0x5)) & con(0xff)) != con(0x4))
      if (SKW_ATTACK_DOOR(vw_14, SKW_RAND16(unsignedword(aiptrrg5->barr06[con(0x0)]) + unsignedword(aiptrrg5->barr06[con(0x0)]) / 2), false, vw_10, con(0x0)))
        return true;
    if (boolrg7 || SKW_CREATURE_ATTACKS_CREATURE(vw_14, vw_10) < con(0x0))
      return true;
  }
  else
  {
    ddata.longv1d26a4 = ddata.longmallocx;
    bool vbool_08 = false;
    x8 byterg7 = CUTX8(aiptrrg5->u0.w & con(0x10));
    x16 vw_18 = con(0x0);
    x16 wordrg11 = con(0x0);
    x16 wordrg47 = ddata.savegamew2;
    while (wordrg11 < wordrg47)
    {
      if (ddata.hero[wordrg11].w36 != con(0x0))
      {
        vba_00[vw_18] = CUTX8(wordrg11);
        vw_18++;
      }
      wordrg11++;
    }
    if (vw_18 == con(0x0))
      return true;
    if ((aiptrrg5->u0.b.b0 & con(0x8)) == con(0x0))
    {
      if ((aiptrrg5->barr06[con(0x3)] & con(0x20)) == con(0x0))
        vw_1c = con(0x1);
      else
        vw_1c = con(0x2);
    }
    else
    {
      if (byterg7 == con(0x0))
        vw_1c = wordrg47;
      else
        vw_1c = SKW_RAND16(vw_18) + con(0x1);
    }
    if (pd556_04->u.v.b1c == con(0x4))
    {
      x16 wordrg197 = SKW_2c1d_028c(unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0xff), unsignedword(ddata.s350.v1e0562.u.b.b_07));
      if (wordrg197 != con(0xffffffff))
        pd556_04->u.v.b1c = ddata.hero[wordrg197].b1d;
    }
    x16 wordrg6 = unsignedword(pd556_04->u.v.b1c);
    vw_1c = DM2_MIN(vw_1c, ddata.savegamew2);
    for(;;)
    {
      wordrg5 = vw_1c - con(0x1);
      vw_1c = wordrg5;
      if (wordrg5 == con(0xffffffff))
      {
        if (!vbool_08)
          return true;
        break;
      }
      if (byterg7 == con(0x0))
      {
        wordrg5 = SKW_GET_PLAYER_AT_POSITION(wordrg6);
        if (((SKW_CALC_VECTOR_DIR(vw_14, unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_07), vw_10) + wordrg6) & con(0x1)) == con(0x0))
          wordrg6++;
        else
          wordrg6--;
        wordrg6 &= con(0x3);
        if (wordrg5 == con(0xffffffff))
          wordrg5 = SKW_2c1d_028c(unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0xff), unsignedword(ddata.s350.v1e0562.u.b.b_07));
        if (wordrg5 == con(0xffffffff))
          continue;
      }
      else
      {
        wordrg5 = SKW_RAND16(vw_18);
        if (vba_00[wordrg5] < con(0x0))
        {
          x16 wordrg17 = con(0x0);
          while (wordrg17 < vw_18)
          {
            wordrg5++;
            if (wordrg5 >= vw_18)
              wordrg5 = con(0x0);
            if (vba_00[wordrg5] >= con(0x0))
              break;
            wordrg17++;
          }
        }
        x16 wordrg103 = wordrg5;
        wordrg5 = signedword(vba_00[wordrg5]);
        vba_00[wordrg103] = con(0xffffffff);
      }
      vbool_08 = true;
      x16 wordrg22 = SKW_CREATURE_ATTACKS_PLAYER(ddata.s350.v1e054e, wordrg5) + con(0x1);
      heroptrrg5 = &ddata.hero[wordrg5];
      if (wordrg22 > unsignedword(heroptrrg5->t22[0x7]))
      {
        heroptrrg5->t22[0x7] = CUTX8(wordrg22);
        heroptrrg5->t22[0x6] = CUTX8((SKW_CALC_VECTOR_DIR(unsignedword(ddata.s350.v1e0562.u.b.b_06), ddata.v1e08d8, ddata.v1e08d4, unsignedword(ddata.s350.v1e0562.u.b.b_07)) + con(0x2)) & con(0x3));
      }
    }
  }
  if (pd556_04->u.v.b1a != con(0x26))
    return false;
  if (SKW_GET_CREATURE_WEIGHT(ddata.s350.v1e054c) <= con(0x64) || (CUTX16(SKW_RAND() & con(0xf)) == con(0x0)))
    if (SKW_RAND02() == con(0x0))
      return false;
  SKW_12b4_0d75(vw_14, vw_0c, con(0xfe), vw_10);
  return false;
}

bool SKW_19f0_2723(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  x16 wordrg5;
  c_record* recoptrrg4;
  bool flag;
  x16 vw_00;

  wordrg5 = edxw;
  vw_00 = ebxw;
  if (eaxw == con(0xfffffffe) || eaxw == con(0xffffffff))
    return false;
  recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(eaxw);
  x16 wordrg18 = recoptrrg4->u2.w & con(0x7f);
  if (wordrg18 < con(0x17))
  {
    if (wordrg18 < con(0x2))
    {
      if (wordrg18 != con(0x1))
        return false;
      return wordrg5 == con(0x0);
    }
    if (wordrg18 <= con(0x2))
    {
      if (wordrg5 != con(0x0))
        return false;
      return vw_00 != con(0xffffffff);
    }
    if (wordrg18 != con(0x3))
      return false;
    flag = wordrg5 == con(0x0);
    if (!flag)
      return false;
  }
  else if (wordrg18 <= con(0x17))
  {
    if (wordrg5 != con(0x0))
      return false;
    return ecxw != (((recoptrrg4->u4.b.b0 & con(0x4)) == con(0x0)) ? 1 : 0);
  }
  else if (wordrg18 < con(0x1a))
  {
    flag = wordrg18 == con(0x18);
    if (!flag)
      return false;
    return wordrg5 == con(0x0);
  }
  else if (wordrg18 <= con(0x1a))
  {
    if ((recoptrrg4->u4.b.b0 & con(0x4)) == con(0x0))
    {
      if (wordrg5 != con(0x1))
        return false;
    }
    else
    {
      flag = wordrg5 == con(0x2);
      if (!flag)
        return false;
    }
  }
  else if (wordrg18 <= con(0x1b))
  {
    if ((wordrg5 != con(0x1)) || ((recoptrrg4->u2.w & con(0xffffff80)) == con(0x0)))
      return false;
  }
  else
  {
    flag = wordrg18 == con(0x4a);
    if (!flag)
      return false;
    return wordrg5 == con(0x0);
  }
  return SKW_1c9a_0006(eaxw, vw_00) != con(0x0);
}

x16 SKW_FIND_TILE_ACTUATOR(x16 eaxw, x8 ebxb, x16 ecxw, x16 edxw)
{
  bool boolrg5;

  x16 wordrg4 = SKW_GET_TILE_RECORD_LINK(eaxw, edxw);
  boolrg5 = false;
  // M_18AE6: co-label M_18B3F
  for (;; wordrg4 = SKW_GET_NEXT_RECORD_LINK(wordrg4))
  {
    if (wordrg4 == con(0xfffffffe))
      break;
    x16 wordrg14 = urshift(wordrg4 & con(0x3c00), con(0xa));
    if (wordrg14 > con(0x3))
      break;
    if (wordrg14 != con(0x3) || ebxb != con(0xffffffff))
      if (urshift(wordrg4, con(0xe)) != unsignedword(ebxb))
        continue;
    boolrg5 = (SKW_GET_ADDRESS_OF_RECORD(wordrg4)->u2.w & con(0x7f)) == ecxw;
    if (boolrg5)
      break;
  }
  if (boolrg5)
    return wordrg4;
  return con(0xffff);
}

// was SKW_2759_0644
void SKW_UPDATE_RIGHT_PANEL(bool eaxbool)
{
  x8 byterg1h;
  x8 byterg2h;
  x8 byterg4h;
  x16 wordrg5;
  bool flag;
  c_hero* heroptrrg5;
  x8 vba_04[0x4];

  bool vbool_08 = false;
  bool vbool_14 = false;
  bool boolrg7 = false;
  if (ddata.v1e0288 != con(0x0))
    return;
  if (ddata.savegamew2 != con(0x0))
  {
    if (eaxbool)
    {
      for (x16 vw_1c = con(0x0); vw_1c < ddata.savegamew2; vw_1c++)
      {
        x16 wordrg32 = con(0x0);
        while (wordrg32 <= con(0x2))
        {
          wordrg5 = vw_1c;
          if (ddata.hero[wordrg5].b2a[wordrg32] != con(0x0))
          {
            byterg4h = ddata.hero[wordrg5].b2a[wordrg32];
            flag = --byterg4h != con(0x0);
            ddata.hero[wordrg5].b2a[wordrg32] = byterg4h;
            if (!flag)
              SKW_LOAD_PROJECTILE_TO_HAND(wordrg5, wordrg32);
          }
          wordrg32++;
        }
      }
      if (ddata.v1d67bc == con(0x3))
      {
        heroptrrg5 = &ddata.hero[ddata.v1e0b74 - con(0x1)];
        if ((ddata.v1e0b62 & con(0x200)) == con(0x0))
        {
          if (heroptrrg5->w3e != con(0x0))
          {
            ddata.v1e0b6c = con(0x1);
            ddata.v1e0b62 = con(0x0);
          }
        }
        else
        {
          x16 wordrg6 = ddata.v1e0b99;
          flag = --wordrg6 != con(0x0);
          ddata.v1e0b99 = wordrg6;
          if (!flag)
          {
            if (heroptrrg5->w3e == con(0x0))
            {
              ddata.v1e0b6c = con(0x1);
              ddata.v1e0b62 = wordrg6;
            }
            else
            {
              heroptrrg5->w3e--;
              heroptrrg5->u32.b.b1 |= con(0x8);
              ddata.v1e0b99 = con(0x19) / ddata.v1e0b4e;
            }
          }
        }
      }
    }
    if (ddata.v1e0b7c == con(0x0))
    {
      if (ddata.v1e0b74 > con(0x0))
      {
        x16 vw_1c_1 = ddata.v1e0b74 - con(0x1);
        if (ddata.v1e0b76 >= con(0x0) && ddata.v1e0b76 != con(0x2))
        {
          x16 wordrg23 = signedlong(vw_1c_1);
          if (ddata.v1d67c2.w0[wordrg23 + con(0x1)][ddata.v1e0b76] != ddata.hero[wordrg23].warrc3[ddata.v1e0b76])
            SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
        }
      }
      if (ddata.v1e0b74 >= con(0x0))
      {
        byterg2h = CUTX8(ddata.v1e0258);
        for (x16 vw_1c_2 = con(0x0); vw_1c_2 < ddata.savegamew2; vw_1c_2++)
        {
          heroptrrg5 = &ddata.hero[vw_1c_2];
          vba_04[vw_1c_2] = con(0x0);
          byterg1h = (heroptrrg5->b1c + con(0x4) - byterg2h) & con(0x3);
          x8 byterg1 = (heroptrrg5->b1d + con(0x4) - byterg2h) & con(0x3);
          if (heroptrrg5->w36 == con(0x0) || (ddata.v1e00b8 == signedword(byterg1 + con(0x1))))
            byterg1 = con(0xffffffff);
          if ((byterg1 != ddata.v1d67c2.b0[vw_1c_2]) || (byterg1h != v1d67be[vw_1c_2]))
          {
            vbool_14 = true;
            boolrg7 = true;
            if (byterg1 != con(0xffffffff))
              ddata.v1e0b30 = true;
            ddata.v1d67c2.b0[vw_1c_2] = byterg1;
            v1d67be[vw_1c_2] = byterg1h;
          }
        }
        if (ddata.v1d67d6 != ddata.v1d6c02 || ddata.savegames1.b4 != con(0x0) || (!ddata.v1e0b2c) != (ddata.savegames1.b2 == con(0x0)))
        {
          vbool_14 = true;
          ddata.v1d67d6 = ddata.v1d6c02;
          ddata.v1e0b2c = ddata.savegames1.b2 != con(0x0);
        }
      }
      bool doit = false; // added
      if (!ddata.v1e0b30)
        doit = true;
      else if (ddata.v1e0b6c <= con(0x1))
      {
        SKW_29ee_00a3(true);
        ddata.v1e0b30 = false;
        vbool_08 = true;
        doit = true;
      }
      if (doit)
      {
        if (ddata.v1e0b74 == con(0x0))
        {
          ddata.v1d67bc = ddata.v1e0b74;
          if (vbool_08 || boolrg7)
            ddata.v1d694a = con(0xffffffff);
          for (x16 vw_1c_3 = con(0x0); vw_1c_3 < ddata.savegamew2; vw_1c_3++)
          {
            heroptrrg5 = &ddata.hero[vw_1c_3];
            if (heroptrrg5->w36 != con(0x0) || SKW_GET_PLAYER_AT_POSITION(unsignedword(heroptrrg5->b1d)) < con(0x0))
            {
              bool vbool_10 = vba_04[vw_1c_3] != con(0x0); // TODO vba_04 a boolarray?
              if (heroptrrg5->w36 <= con(0x0) != ddata.v1e0b1c[vw_1c_3])
              {
                ddata.v1e0b1c[vw_1c_3] = heroptrrg5->w36 > con(0x0);
                vbool_10 = true;
                ddata.v1d694a = con(0xffffffff);
              }
              bool vbool_18 = (DM2_ABS(ddata.v1e0b54) - con(0x1)) == vw_1c_3;
              x16 wordrg3 = con(0x0);
              while (wordrg3 <= con(0x1))
              {
                bool boolrg61 = false;
                bool boolrg4 = false;
                if (vbool_18 && ddata.v1e0b52 == wordrg3)
                {
                  if (ddata.v1e0b54 > con(0x0))
                    boolrg61 = true;
                  boolrg4 = true;
                }
                x8 byterg21 = (heroptrrg5->b2a[wordrg3] != con(0x0)) ? 1 : 0;
                if (byterg21 != ddata.v1e0b34[vw_1c_3][wordrg3])
                {
                  ddata.v1e0b34[vw_1c_3][wordrg3] = byterg21;
                  boolrg4 = true;
                }
                if (ddata.v1d67c2.w0[vw_1c_3 + con(0x1)][wordrg3] != heroptrrg5->warrc3[wordrg3] || vbool_08 || vbool_10)
                {
                  ddata.v1d67c2.w0[vw_1c_3 + con(0x1)][wordrg3] = heroptrrg5->warrc3[wordrg3];
                  boolrg4 = true;
                }
                if (vbool_08 || vbool_10 || boolrg4)
                  SKW_DRAW_HAND_ACTION_ICONS(vw_1c_3, boolrg61 ? 1 : 0, wordrg3);
                wordrg3++;
              }
              x16 boolrg6 = false;
              if (vbool_18 && ddata.v1e0b52 == con(0x2))
              {
                if (ddata.v1e0b54 > con(0x0))
                  boolrg6 = true;
                vbool_10 = true;
              }
              x8 byterg2 = (heroptrrg5->b2a[con(0x2)] != con(0x0)) ? 1 : 0;
              if (byterg2 != ddata.v1e0b34[vw_1c_3][2])
              {
                ddata.v1e0b34[vw_1c_3][2] = byterg2;
                vbool_10 = true;
              }
              if (vbool_08 || vbool_10 || (ddata.v1d67d7 != ddata.mouse_unk0e && (ddata.v1d67d7 == vw_1c_3 || ddata.mouse_unk0e == vw_1c_3)))
                SKW_DRAW_SQUAD_SPELL_AND_LEADER_ICON(vw_1c_3, boolrg6);
              if ((ddata.hero[vw_1c_3].w103 != con(0x0)) || ddata.v1d6948 != con(0x0))
                vbool_14 = true;
              x16 wordrg12 = con(0x1) << CUTX8(vw_1c_3);
              if (ddata.hero[vw_1c_3].w103 == con(0x0))
                ddata.v1d6948 &= ~wordrg12;
              else
                ddata.v1d6948 |= wordrg12;
            }
          }
          if (ddata.v1e0b54 < con(0x0))
            ddata.v1e0b54 = con(0x0);
          ddata.v1e0b54 = -ddata.v1e0b54;
          if (vbool_08 || vbool_14)
            SKW_29ee_0396();
        }
        ddata.v1d67d7 = ddata.mouse_unk0e;
        bool skip = false;
        if (ddata.v1e0b74 <= con(0x0) || ((ddata.v1e0b6c == con(0x0) && !vbool_08 && !vbool_14 && ddata.v1d67bc != con(0x3)) || !eaxbool))
        {
          if (ddata.v1e0b74 > con(0x0) && ddata.v1d67bc == con(0x5))
            if (eaxbool)
            {
              SKW_29ee_00a3(false);
              SKW_DRAW_CONTAINER_PANEL(ddata.hero[ddata.v1e0b74 - con(0x1)].warrc3[ddata.v1e0b76], false);
            }
        }
        else
        {
          if (ddata.v1e0b6c > con(0x1))
          {
            ddata.v1e0b6c = ddata.v1e0b6c - con(0x1);
            skip = true;
          }
          else
          {
            if (ddata.v1e0b62 != con(0x0) && (ddata.v1e0b6c == con(0x0) && !vbool_08))
              SKW_29ee_00a3(false);
            else
            {
              SKW_29ee_00a3(true);
              SKW_DRAW_PLAYER_NAME_AT_CMDSLOT();
              ddata.v1e0b62 &= con(0xfffffbff);
            }
            // M_3064A:
            if (ddata.v1e0b76 != con(0x2))
            {
              x16 vw_1c_4 = ddata.v1e0b74 - con(0x1);
              heroptrrg5 = &ddata.hero[vw_1c_4];
              x16 wordrg33 = heroptrrg5->warrc3[ddata.v1e0b76];
              if (!SKW_IS_CONTAINER_MAP(wordrg33) || ddata.v1e0b62 != con(0x0))
              {
                wordrg5 = wordrg33;
                if (!SKW_IS_CONTAINER_MONEYBOX(wordrg5))
                {
                  if (SKW_IS_CONTAINER_CHEST(wordrg5) && ddata.v1d67bc != con(0x5))
                  {
                    SKW__CHECK_ROOM_FOR_CONTAINER(wordrg5, SKW_GET_ADDRESS_OF_RECORD(wordrg5));
                    ddata.v1d67bc = con(0x5);
                  }
                }
                else
                  ddata.v1d67bc = con(0x4);
              }
              else
              {
                ddata.v1e0b4e = con(0x1);
                ddata.v1e0b99 = con(0x19);
                x16 wordrg2 = heroptrrg5->w3e;
                if (wordrg2 == con(0x0))
                  ddata.v1e0b62 = con(0xffff8000);
                else
                {
                  heroptrrg5->w3e = wordrg2 - con(0x1);
                  heroptrrg5->u32.b.b1 |= con(0x8);
                  ddata.v1e0b62 = con(0xffff8200);
                }
                ddata.v1d67bc = con(0x3);
              }
              if (ddata.v1d67bc != con(0x4))
              {
                if (ddata.v1d67bc != con(0x5))
                {
                  if (!SKW_IS_ITEM_HAND_ACTIVABLE(vw_1c_4, ddata.v1e0b76, wordrg33) || ddata.v1d67bc == con(0x3))
                  {
                    if (ddata.v1d67bc != con(0x3))
                    {
                      ddata.v1e0b74 = con(0x0);
                      ddata.v1d67bc = con(0x0);
                    }
                    else
                    {
                      if ((ddata.v1e0b62 & con(0x400)) == con(0x0))
                        ddata.v1d694a = con(0xffffffff);
                      SKW_DRAW_MAJIC_MAP(wordrg33);
                    }
                  }
                  else
                  {
                    ddata.v1d67bc = con(0x1);
                    if (ddata.v1d694a != con(0x1))
                    {
                      R_107B0();
                      ddata.v1d694a = ddata.v1d67bc;
                    }
                    SKW_29ee_0b2b();
                  }
                }
                else
                  SKW_DRAW_CONTAINER_PANEL(wordrg33, true);
              }
              else
                SKW_DRAW_MONEYBOX(wordrg33);
            }
            else
            {
              ddata.v1d67bc = ddata.v1e0b76;
              SKW_DRAW_SPELL_PANEL();
            }
            ddata.v1e0b6c = con(0x0);
          }
        }
        if (!skip)
        {
          // M_3087E:
          if (((ddata.v1e0b74 > con(0x0)) && table1d67d9[ddata.v1d67bc] != con(0x0)) && (ddata.hero[ddata.v1e0b74 - con(0x1)].w103 != con(0x0) || ddata.v1d6948 != con(0x0)))
          {
            SKW_DRAW_PLAYER_ATTACK_DIR();
            x32 longrg43 = 263 * (signedlong(ddata.v1e0b74) - con(0x1));
            x16 wordrg13 = con(0x1) << CUTX8(ddata.v1e0b74 - con(0x1));
            if (ddata.hero[longrg43 / 263].w103 == con(0x0))
              ddata.v1d6948 &= ~wordrg13;
            else
              ddata.v1d6948 |= wordrg13;
          }
        }
      }
      else
        ddata.v1e0b6c--;
    }
    else
    {
      SKW_SHOW_ATTACK_RESULT(ddata.v1e0b7c);
      ddata.v1e0b30 = true;
      ddata.v1e0b7c = con(0x0);
      ddata.v1d67bc = con(0x6);
    }
  }
  else
  {
    if ((ddata.v1e0b30) || (ddata.v1e0b6c != con(0x0)))
    {
      ddata.v1e0b6c = con(0x0);
      ddata.v1e0b30 = false;
      SKW_29ee_00a3(true);
    }
    ddata.v1d67bc = con(0xffffffff);
  }
  if (ddata.v1d694c.s_00.w_00 != con(0xffffffff))
    SKW_0b36_0cbe(&ddata.v1d694c, true);
  if (ddata.v1d67bc != ddata.v1d694a)
  {
    R_107B0();
    ddata.v1d694a = ddata.v1d67bc;
  }
}

void R_39796(c_hero* eaxphero)
{
  bool boolrg5;
  t_text* tptrrg7;
  t_text* tptrrg1;
  c_hero* phero_00;
  s_2ndname vs_04;
  s_1stname vs_18;
  x16 vw_20; // x0 rectB
  x16 vw_24;
  x16 vw_28; // y0 rectA
  x16 vw_2c; // x0 rectA
  x16 vw_30;
  x16 vw_34; // y0 rectB

  phero_00 = eaxphero;
  ddata.v1d66fc = con(0x8);
  SKW_COPY_MEMORY(DOWNCAST(t_text, phero_00->t00), con(0x8), DOWNCAST(s_1stname, &vs_18));
  SKW_COPY_MEMORY(DOWNCAST(t_text, &phero_00->t00[con(0x8)]), con(0x14), DOWNCAST(s_2ndname, &vs_04));
  SKW_QUERY_TOPLEFT_OF_RECT(con(0x229), &vw_2c, &vw_28);
  SKW_QUERY_TOPLEFT_OF_RECT(con(0x273), &vw_20, &vw_34);
  SKW_1031_0675(con(0x9));
  ddata.v1e023c = true;
  SKW_38c8_0002();
  // M_39A39:
  for(;;)
  {
    boolrg5 = true;
    vw_24 = con(0x7);
    x16 wordrg6 = SKW_SK_STRLEN(phero_00->t00);
    x16 vw_38 = vw_2c + wordrg6 * C_strmetrics.charwidth;
    x16 wordrg100 = vw_28;
    bool brk1;
    bool brk2;
    // M_39A72:
    for(;;)
    {
      vw_30 = wordrg100;
      // M_3998A:
      for(;;)
      {
        SKW_DRAW_STATIC_PIC(con(0x7), con(0x14), con(0x267), con(0x0), con(0xffffffff));
        SKW_DRAW_VP_RC_STR(con(0x229), phero_00->t00, unsignedword(ddata.paletteptr1[con(0xb)]));
        SKW_DRAW_VP_RC_STR(con(0x273), &phero_00->t00[con(0x8)], unsignedword(ddata.paletteptr1[con(0xb)]));
        SKW_DRAW_VP_STR(vw_38, unsignedword(ddata.paletteptr1[con(0x9)]), v1d6a4f, vw_30);
        SKW_FIRE_HIDE_MOUSE_CURSOR();
        SKW_CHANGE_VIEWPORT_TO_INVENTORY(false);
        SKW_FIRE_SHOW_MOUSE_CURSOR();
        // M_39A0B:
        // break without flags leads to the M_3998A-loop
        // brk1 && !brk2 leads to M_3981E
        // !brk1 && brk2 leads to M_39A39 without M_3981E
        // both flags set lead to the end
        for(;;)
        {
          brk1 = brk2 = false;
          ddata.mouse_unk06 = con(0x0);
          ddata.mouse_unk0a = con(0x0);
          SKW_EVENT_LOOP();
          if (ddata.mouse_unk0a != con(0x49) || boolrg5)
          {
            if (ddata.mouse_unk0a != con(0x4a) || !boolrg5)
            {
              if (ddata.mouse_unk0a != con(0x4b))
              {
                if (ddata.mouse_unk0a == con(0x0))
                  continue;
                if (ddata.v1e051e != con(0xe))
                {
                  if (ddata.v1e051e != con(0x1c) && ddata.v1e051e != con(0xf))
                  {
                    if (wordrg6 >= vw_24)
                      break;
                    x16 wordrg25 = SKW_476d_05b6(ddata.v1e051e);
                    if (wordrg25 >= con(0x61) && wordrg25 <= con(0x7a))
                      wordrg25 -= con(0x20);
                    x16 wordrg47 = con(0x0);
                    while (wordrg47 < con(0x25))
                    {
                      if (wordrg25 == v1d6a08[wordrg47])
                        break;
                      wordrg47++;
                    }
                    if (wordrg47 == con(0x25) || (wordrg25 == con(0x20) && wordrg6 == con(0x0)))
                      continue;
                    tptrrg7[wordrg6++] = CUTX8(wordrg25);
                    tptrrg7[wordrg6] = NULLBYTE;
                    vw_38 += C_strmetrics.charwidth;
                    break;
                  }
                  wordrg6 = SKW_SK_STRLEN(tptrrg7);
                  for(;;)
                  {
                    wordrg6--;
                    tptrrg1 = &tptrrg7[wordrg6];
                    if (*tptrrg1 != ' ')
                      break;
                    *tptrrg1 = NULLBYTE;
                  }
                  if (boolrg5)
                  {
                    brk1 = true;
                    break;
                  }
                  if (ddata.v1e051e == con(0xf))
                  {
                    brk2 = true;
                    break;
                  }
                  brk1 = brk2 = true;
                  break;
                }
                if (wordrg6 > con(0x0))
                {
                  wordrg6--;
                  tptrrg7[wordrg6] = NULLBYTE;
                  vw_38 -= C_strmetrics.charwidth;
                }
                break;
              }
              SKW_COPY_MEMORY(DOWNCAST(s_1stname, &vs_18), con(0x8), DOWNCAST(t_text, phero_00->t00));
              SKW_COPY_MEMORY(DOWNCAST(s_2ndname, &vs_04), con(0x14), DOWNCAST(t_text, &phero_00->t00[con(0x8)]));
              brk1 = brk2 = true;
              break;
            }
            brk1 = true;
            break;
          }
          brk2 = true;
          break;
        }
        if (brk1 || brk2)
          break;
      }
      if (brk2)
        break;
      // M_3981E:
      boolrg5 = false;
      vw_24 = con(0xf);
      tptrrg7 = &phero_00->t00[con(0x8)];
      wordrg6 = SKW_SK_STRLEN(tptrrg7);
      vw_38 = vw_20 + wordrg6 * C_strmetrics.charwidth;
      wordrg100 = vw_34;
    }
    if (brk1)
      break;
  }
  ddata.v1e023c = false;
  SKW_DRAW_STATIC_PIC(con(0x7), con(0x13), con(0x267), con(0x0), con(0xffffffff));
  phero_00->u32.b.b1 |= con(0x4);
  SKW_REFRESH_PLAYER_STAT_DISP(ddata.v1e0976 - con(0x1));
  SKW_38c8_0060();
  SKW_1031_06a5();
}

x8 SKW_14cd_0684(void)
{
  x8 byterg1h;
  x8 vb_00;
  c_sizex16* sx16ptrrg1;

  bool boolrg6 = (SKW_14cd_0389() != con(0xffffffff));
  vb_00 = con(0xffffffff);
  if ((table1d607e[ddata.s350.v1e0584].u.b[0] & con(0x1)) != con(0x0) || !boolrg6 || SKW_RAND02() == con(0x0))
  {
    ddata.s350.v1e0674 = con(0x0);
    ddata.s350.v1e07ec = SKW_14cd_062e();
    byterg1h = ddata.s350.v1e0556->u.v.b12;
    SKW_14cd_0550(ddata.s350.v1e0588->p2->s0, ddata.s350.v1e0556->u.v.b13, boolrg6, byterg1h);
    if (boolrg6)
      SKW_14cd_0457();
    if (ddata.s350.v1e0674 != con(0x0))
    {
      if ((ddata.s350.v1e054e->ua.w & con(0x2048)) == con(0x0))
        byterg1h = con(0x0);
      else
        byterg1h = ddata.s350.v1e07ed;
      if ((ddata.s350.v1e054e->ua.w & con(0x1000)) != con(0x0))
        byterg1h |= con(0x20);
      ddata.s350.v1e0675 = SKW_1c9a_1bd8(ddata.s350.v1e0562.u.b.b_06, byterg1h, unsignedword(ddata.s350.v1e0562.u.b.b_07), ddata.s350.v1e0674, ddata.s350.v1e0678, ddata.s350.v1e060e);
      if (ddata.s350.v1e0675 == con(0xffffffff))
      {
        if (ddata.s350.v1e07ec != con(0x0))
          return con(0xfffffffd);
      }
      else
      {
        sx16ptrrg1 = &ddata.s350.v1e0678[ddata.s350.v1e0675];
        vb_00 = sx16ptrrg1->b_0e[con(0x0)];
        ddata.s350.v1e0556->u.v.u18.w = sx16ptrrg1->s_00.u_02.w;
        SKW_14cd_0276(sx16ptrrg1);
      }
    }
  }
  if ((vb_00 != con(0xffffffff)) || !boolrg6)
  {
    byterg1h = vb_00;
    if (byterg1h != con(0xffffffff))
    {
      if (ddata.s350.v1e0556->u.v.b12 != con(0xffffffff))
      {
        if (ddata.s350.v1e0556->u.v.b12 != byterg1h)
        {
          ddata.s350.v1e0556->u.v.b12 = con(0xffffffff);
          ddata.s350.v1e0556->u.v.b13 = con(0x0);
        }
        else
          vb_00 = con(0xffffffff);
      }
    }
    else
    {
      vb_00 = con(0x11);
      ddata.s350.v1e07d8.b1 = con(0x0);
      ddata.s350.v1e07d8.b3 = byterg1h;
    }
  }
  return vb_00;
}

void SKW_14cd_0550(s_anientry* eaxpani, x8 ebxb, bool ecxbool, x8 edxb)
{
  s_anientry* aniptrrg5;
  s_anientry* aniptrrg1;
  const s_seven* s7ptrrg7;
  x8 vb_04;
  x8 vb_08;

  aniptrrg5 = eaxpani;
  vb_04 = edxb;
  vb_08 = ebxb;
  bool boolrg6 = false;
  // M_15FB6:
  for(;;)
  {
    bool skip = false;
    x8 byterg2 = aniptrrg5->b0;
    x16 wordrg4;
    if (byterg2 == vb_04)
    {
      boolrg6 = ecxbool || ddata.s350.v1e07ec != con(0x0);
      wordrg4 = signedword(vb_08);
    }
    else if (ddata.s350.v1e07ec != con(0x0))
      skip = true;
    else if (aniptrrg5->b1 == con(0x0))
      wordrg4 = con(0x0);
    else if (aniptrrg5->b1 <= con(0x0))
    {
      if (SKW_RAND16(-signedword(aniptrrg5->b1)) == con(0x0))
        skip = true;
      else
        wordrg4 = con(0x0);
    }
    else if (SKW_RAND16(signedword(aniptrrg5->b1)) != con(0x0))
      skip = true;
    else
      wordrg4 = con(0x0);
    if (!skip)
    {
      s7ptrrg7 = table1d5f82[byterg2];
      SKW_14cd_0f0a(s7ptrrg7[wordrg4].b0[con(0x5)], byterg2, aniptrrg5->ps2, s7ptrrg7[wordrg4].b0[con(0x6)]);
    }
    if (boolrg6)
      return;
    aniptrrg1 = aniptrrg5++;
    if (aniptrrg1->b6 == con(0x0))
      return;
  }
}

x16 SKW_19f0_0207(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, FPTR_b_WW argf0)
{
  x16 wordrg5;
  x16 vw_04;
  x16 vw_08;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_18;
  x16 vw_1c;
  x16 vw_20;
  x16 vw_24;
  x16 vw_28;

  x16 vw_2c = edxw;
  vw_24 = ebxw;
  vw_20 = ecxw;
  if ((DM2_ABS(vw_2c - ecxw) + DM2_ABS(eaxw - ebxw)) <= con(0x1))
    return con(0x1);
  x16 wordrg31 = ebxw - eaxw;
  if (wordrg31 < con(0x0))
    wordrg31 = -wordrg31;
  x16 wordrg22 = vw_20 - vw_2c;
  x16 wordrg21;
  if (wordrg22 >= con(0x0))
    wordrg21 = wordrg22;
  else
    wordrg21 = -wordrg22;
  vw_10 = (wordrg31 < wordrg21) ? 1 : 0;
  vw_28 = (wordrg31 == wordrg21) ? 1 : 0;
  x16 wordrg32 = vw_24;
  if (vw_24 <= eaxw)
    vw_18 = con(0x1);
  else
    vw_18 = con(0xffffffff);
  x16 wordrg2 = vw_20;
  bool skip = false; // added
  if (vw_20 <= vw_2c)
    wordrg5 = con(0x1);
  else
    wordrg5 = con(0xffffffff);
  x32 longrg41;
  x16 wordrg100;
  x16 wordrg101;
  if (vw_10 == con(0x0))
  {
    wordrg101 = wordrg32 - eaxw;
    if (wordrg101 == con(0x0))
    {
      wordrg100 = con(0x80);
      skip = true;
    }
    else
      longrg41 = signedlong(wordrg2) - signedlong(vw_2c);
  }
  else
  {
    wordrg101 = wordrg2 - vw_2c;
    if (wordrg101 == con(0x0))
    {
      wordrg100 = con(0x80);
      skip = true;
    }
    else
      longrg41 = signedlong(wordrg32) - signedlong(eaxw);
  }
  if (!skip)
  {
    longrg41 <<= con(0x6); // keep longsize!
    wordrg100 = CUTX16(longrg41 / wordrg101);
  }
  vw_08 = wordrg100;
  // M_1BD85:
  for(;;)
  {
    x16 wordrg7;
    bool skip = false;
    x16 wordrg1 = vw_18 + wordrg32;
    vw_0c = wordrg1;
    if (vw_28 == con(0x0))
    {
      x32 wordrg413;
      if (vw_10 == con(0x0))
      {
        wordrg1 = vw_0c - eaxw;
        if (wordrg1 == con(0x0))
        {
          wordrg1 = con(0x80);
          skip = true;
        }
        else
        {
          wordrg413 = vw_2c;
          wordrg7 = wordrg2;
        }
      }
      else if (wordrg1 = wordrg2 - vw_2c, wordrg1 == con(0x0))
      {
        wordrg1 = con(0x80);
        skip = true;
      }
      else
      {
        wordrg7 = wordrg32 + vw_18;
        wordrg413 = eaxw;
      }
      // M_1BE01:
      if (!skip)
      {
        wordrg7 -= wordrg413;
        wordrg1 = CUTX16(lextended(wordrg7 << con(0x6)) / wordrg1);
      }
      // M_1BE3D:
      wordrg1 -= vw_08;
      x16 vw_14;
      if (wordrg1 >= con(0x0))
        vw_14 = wordrg1;
      else
        vw_14 = -wordrg1;
      skip = false; // reset for 2nd usage
      x32 longrg47;
      if (vw_10 == con(0x0))
      {
        wordrg1 = wordrg32 - eaxw;
        if (wordrg1 == con(0x0))
        {
          wordrg1 = con(0x80);
          skip = true;
        }
        else
        {
          longrg47 = signedlong(wordrg5) + signedlong(wordrg2);
          wordrg7 = vw_2c;
          longrg47 -= signedword(wordrg7);
        }
      }
      else if (wordrg1 = wordrg2 + wordrg5 - vw_2c, wordrg1 == con(0x0))
      {
        wordrg1 = con(0x80);
        skip = true;
      }
      else
      {
        wordrg7 = wordrg32 - eaxw;
        longrg47 = signedlong(wordrg7);
      }
      // M_1BE7C:
      if (!skip)
      {
        longrg47 <<= con(0x6); // keep longsize
        wordrg1 = CUTX16(longrg47 / wordrg1);
      }
      // M_1BEB7:
      wordrg1 -= vw_08;
      if (wordrg1 >= con(0x0))
        vw_04 = wordrg1;
      else
        vw_04 = -wordrg1;
      if (vw_14 >= vw_04)
        wordrg2 += wordrg5;
      else
        wordrg32 += vw_18;
      skip = false; // reset for 3rd usage
      if (!INDCALL(argf0)(wordrg32, wordrg2))
        skip = true;
      else
      {
        if (vw_14 != vw_04)
          return con(0x0);
        wordrg2 -= wordrg5;
      }
    }
    else
    {
      if (INDCALL(argf0)(vw_0c, wordrg2))
        if (INDCALL(argf0)(wordrg32, wordrg2 + wordrg5))
          return con(0x0);
      wordrg2 += wordrg5;
    }
    // M_1BDBE:
    if (!skip)
    {
      wordrg32 += vw_18;
      if (INDCALL(argf0)(wordrg32, wordrg2))
        return con(0x0);
    }
    // M_1BF06:
    vw_1c = wordrg2 - vw_2c;
    if ((DM2_ABS(vw_1c) + DM2_ABS(wordrg32 - eaxw)) <= con(0x1))
      return SKW_CALC_SQUARE_DISTANCE(eaxw, vw_24, vw_20, vw_2c);
  }
}

void R_3CE7D(x16 eaxw, x16 ebxw, bool ecxbool, x16 edxw, bool argbool0, bool argbool1)
{
  x16 wordrg5;
  x16* wptrrg4;
  c_record* recoptrrg5;
  c_record* recoptrrg2;
  c_record* recoptrrg4;
  c_aispec* aiptrrg1;
  x16 vw_04;
  t_text t_10[0xc8];
  c_tim vtim_d8;
  x16* pw_f0;
  c_record* prec_100;
  x16 vw_104;
  x16 vw_108;
  x16 vw_10c;
  x16 vw_110;
  x8 vb_118;
  bool flag = false; // PROBLEM! not necessarily initialized when used!!! So, set to false here. TODO

  x16 vw_0c = eaxw;
  x16 vw_08 = edxw;
  vw_04 = ebxw;
  wptrrg4 = NULL;
  pw_f0 = NULL;
  if (ebxw == con(0xffffffff))
  {
    vw_10c = con(0xffffffff);
    vw_110 = con(0xffffffff);
  }
  else
  {
    vw_10c = urshift(ebxw & con(0x3c00), con(0xa));
    vw_110 = SKW_GET_DISTINCTIVE_ITEMTYPE(ebxw);
    x16 wordrg111 = SKW_GET_CREATURE_AT(vw_0c, vw_08);
    if (wordrg111 != con(0xffffffff))
    {
      recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(wordrg111);
      aiptrrg1 = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(recoptrrg4->u4.b.b0);
      if (argbool1)
      {
        if (!argbool0 || ((aiptrrg1->u0.b.b0 & con(0x1)) != con(0x0) && (aiptrrg1->u1e.b.b1 & con(0x8)) != con(0x0)))
          pw_f0 = &recoptrrg4->u2.w;
      }
    }
  }
  if (!argbool0)
  {
    if (vw_10c != con(0xffffffff))
    {
      x16 wordrg35;
      x16 wordrg25;
      if (pw_f0 != NULL)
      {
        wordrg35 = con(0x0);
        wordrg25 = con(0xffffffff);
        wptrrg4 = pw_f0;
      }
      else
      {
        wordrg35 = vw_08;
        wordrg25 = vw_0c;
        wptrrg4 = NULL;
      }
      SKW_CUT_RECORD_FROM(vw_04, wordrg25, wordrg35, wptrrg4);
    }
  }
  if (ubrshift(ddata.savegamepp1[vw_0c][vw_08], con(0x5)) != con(0x0))
    vw_104 = con(0xffffffff);
  else
    vw_104 = urshift(vw_04, con(0xe));
  bool vbool_e4 = false;
  bool vbool_ec = false;
  bool vbool_fc = false;
  x16 vw_f4 = con(0x0); // TODO: is bool, if wordrg5 is bool
  x16 wordrg6 = SKW_GET_TILE_RECORD_LINK(vw_0c, vw_08);
  if (vw_104 != con(0xffffffff))
  {
    wordrg5 = con(0x1);
    for(;;)
    {
      if (wordrg6 == con(0xfffffffe))
        break;
      if (vw_104 == (wordrg6 >> con(0xe)))
      {
        if ((urshift(wordrg6 & con(0x3c00), con(0xa))) > con(0x4))
        {
          vw_f4 = wordrg5;
          vbool_ec |= SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg6) == vw_110;
          vbool_e4 |= SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg6) != vw_110;
        }
      }
      wordrg6 = SKW_GET_NEXT_RECORD_LINK(wordrg6);
    }
  }
  else
  {
    for(;;)
    {
      if (wordrg6 == con(0xfffffffe))
        break;
      x16 wordrg415 = urshift(wordrg6 & con(0x3c00), con(0xa));
      if (wordrg415 != con(0x4) || (wordrg5 = wordrg6, SKW_IS_OBJECT_FLOATING(wordrg5)))
      {
        if (wordrg415 != con(0x2) || vw_10c != con(0xffffffff) || !argbool0 || ecxbool)
        {
          if (wordrg415 > con(0x4) && wordrg415 < con(0xe))
          {
            vw_f4 = con(0x1);
            vbool_ec |= SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg6) == vw_110;
            vbool_e4 |= SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg6) != vw_110;
          }
        }
        else
        {
          x8 byterg3 = SKW_GET_ADDRESS_OF_RECORD(wordrg6)->u2.b.b0;
          if ((byterg3 & con(0x6)) == con(0x0) && (byterg3 & con(0x1)) != con(0x0))
          {
            SKW_QUERY_MESSAGE_TEXT(t_10, con(0x1), wordrg6);
            SKW_DISPLAY_HINT_TEXT(con(0xf), t_10);
          }
        }
      }
      else
      {
        if ((SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg5) & con(0x1)) != con(0x0) && (SKW_1c9a_06bd(wordrg5, con(0xff), vw_110) != NULL))
          vbool_ec = true;
        vbool_fc = true;
      }
      wordrg6 = SKW_GET_NEXT_RECORD_LINK(wordrg6);
    }
  }
  if (argbool0 && vw_10c != con(0xffffffff))
  {
    x16 wordrg34;
    x16 wordrg24;
    if (pw_f0 != NULL)
    {
      wordrg34 = con(0x0);
      wordrg24 = con(0xffffffff);
      wptrrg4 = pw_f0;
    }
    else
    {
      wordrg34 = vw_08;
      wordrg24 = vw_0c;
      wptrrg4 = NULL;
    }
    SKW_APPEND_RECORD_TO(vw_04, wordrg24, wordrg34, wptrrg4);
  }
  x16 wordrg1 = SKW_GET_TILE_RECORD_LINK(vw_0c, vw_08);
  // M_3D1E2: co-label is M_3D925
  x16 wordrg61;
  for (;; wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg61))
  {
    wordrg61 = wordrg1;
    if (wordrg61 == con(0xfffffffe))
      return;
    if (vw_104 != con(0xffffffff))
    {
      if (SKW_IS_WALL_ORNATE_SPRING(wordrg61) != con(0x0))
      {
        x16 wordrg22 = vw_04;
        if (SKW_QUERY_CLS1_FROM_RECORD(wordrg22) == con(0x15))
        {
          if (SKW_QUERY_CLS2_FROM_RECORD(wordrg22) == con(0x0))
          {
            x16 wordrg145 = SKW_ADD_ITEM_CHARGE(wordrg22, con(0x0));
            x8 byterg41 = CUTX8(wordrg145);
            if (wordrg145 < con(0x0))
              return;
            if (wordrg145 >= ddata.savegamew2)
              return;
            vtim_d8.un.l_00 = (ddata.longmallocx + 1) | (signedlong(ddata.v1d3248) << con(0x18));
            vtim_d8.b_04 = con(0xd);
            vtim_d8.b_05 = byterg41;
            vtim_d8.u.b.b_06 = CUTX8(vw_0c);
            vtim_d8.u.b.b_07 = CUTX8(vw_08);
            vtim_d8.v.b.b_08 = CUTX8(vw_104);
            vtim_d8.v.b.b_09 = con(0x2);
            SKW_QUEUE_TIMER(&vtim_d8);
            return;
          }
        }
      }
    }
    x16 wordrg188 = urshift(wordrg61 & con(0x3c00), con(0xa));
    if (wordrg188 != con(0x3))
    {
      if (wordrg188 != con(0x2))
      {
        if (wordrg188 >= con(0x4))
          return;
        continue;
      }
      x16 wordrg414 = wordrg61;
      recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(wordrg414);
      prec_100 = recoptrrg2;
      if ((recoptrrg2->u2.w & con(0x6)) != con(0x2))
        continue;
      if (vw_104 != con(0xffffffff))
      {
        wordrg414 >>= con(0xe);
        if (vw_104 != wordrg414)
          continue;
        x16 wordrg137 = urshift(recoptrrg2->u2.w, con(0xb)) & con(0x1f);
        if (wordrg137 < con(0x4))
          continue;
        if (wordrg137 > con(0x4))
          if (wordrg137 != con(0x8))
            continue;
        if (!vbool_ec)
        {
          if ((prec_100->u2.w & con(0x1)) != (argbool0 ? 1 : 0))
          {
            if (vw_110 == DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0xe), CUTX8(urshift(prec_100->u2.w, con(0x3)))))
            {
              recoptrrg4 = prec_100;
              recoptrrg4->u2.b.b0 &= con(0xfffffffe);
              recoptrrg4->u2.w |= argbool0 ? 1 : 0;
            }
          }
        }
        continue;
      }
      x16 wordrg36 = urshift(recoptrrg2->u2.w, con(0x3));
      x16 wordrg167 = urshift(wordrg36, con(0x8)) & con(0x1f);
      if (wordrg167 < con(0x9))
        continue;
      if (wordrg167 <= con(0x9))
      {
        if (vw_10c != con(0xffffffff))
          continue;
        if (ecxbool)
          continue;
        if (ddata.savegamew2 == con(0x0))
          continue;
        wordrg36 = unsignedword(CUTX8(wordrg36));
        if (UI32CAST(SKW_RAND16(con(0x64))) >= UI32CAST(unsignedlong(wordrg36)))
          continue;
        if ((recoptrrg2->u2.w & con(0x1)) == (argbool0 ? 1 : 0))
          continue;
        wordrg5 = vw_08;
        SKW_INVOKE_MESSAGE(vw_0c, con(0x0), con(0x0), wordrg5, ddata.longmallocx + con(0x1));
        SKW_INVOKE_MESSAGE(vw_0c, con(0x0), con(0x1), wordrg5, ddata.longmallocx + con(0x5)); // BUGFIX 3/2014 + BUGFIX 4/2014
        continue;
      }
      if (wordrg167 == con(0xa))
      {
        if (vw_10c != con(0xffffffff))
          continue;
        if (argbool0)
          continue;
        if (ddata.savegamew2 == con(0x0))
          continue;
        x16 wordrg21 = con(0x0);
        x16 wordrg31 = con(0x0);
        while (wordrg31 < ddata.savegamew2)
        {
          if (ddata.hero[wordrg31].w36 != con(0x0))
          {
            wordrg5 = SKW_MAX_LOAD(&ddata.hero[wordrg31]) / 2; // BUGFIX 4/2014  TODO: check index again
            wordrg21 = wordrg21 + SKW_GET_PLAYER_WEIGHT(wordrg31) / wordrg5;
          }
          wordrg31++;
        }
        x16 wordrg49;
        if ((prec_100->u2.b.b0 & con(0x1)) == con(0x0))
          wordrg49 = con(0x19);
        else
          wordrg49 = con(0x32);
        if (ecxbool || (SKW_RAND16(con(0x64)) >= DM2_MIN(con(0x5a), 10 * wordrg21 + wordrg49)))
        {
          SKW_QUEUE_NOISE_GEN1(con(0xa), con(0x88), con(0x8c), CUTX8(urshift(prec_100->u2.w, con(0x3))), con(0x80), ddata.v1e0270, ddata.v1e0272, con(0x1));
          continue;
        }
        vtim_d8.un.l_00 = ddata.longmallocx | (signedlong(ddata.v1d3248) << con(0x18));
        vtim_d8.b_04 = con(0x5d);
        vtim_d8.b_05 = con(0x0);
        vtim_d8.u.b.b_06 &= con(0xffffffe0);
        wordrg5 = (vtim_d8.u.w_06 | (vw_0c & con(0x1f))) & con(0xfc1f);
        vtim_d8.u.w_06 = wordrg5 | ((vw_08 & con(0x1f)) << con(0x5));
        vtim_d8.v.b.b_08 = CUTX8(ddata.v1e0266);
        vtim_d8.u.b.b_07 &= con(0xfffffff3);
        x16 wordrg159 = (ddata.v1e0258 & con(0x3)) << con(0xa);
        vtim_d8.u.w_06 |= wordrg159;
        SKW_QUEUE_TIMER(&vtim_d8);
        x16 wordrg3 = SKW_RAND02();
        if (ddata.hero[wordrg159].w36 == con(0x0))
          wordrg3 = ddata.mouse_unk0e;
        SKW_QUEUE_NOISE_GEN2(con(0x16), con(0x82), con(0xfe), ddata.hero[wordrg3].b101, ddata.v1e0270, ddata.v1e0272, con(0x1), con(0x69), con(0xc8));
      }
      continue;
    }
    x16 wordrg431 = wordrg61;
    recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(wordrg431);
    if ((recoptrrg5->u2.b.b0 & con(0x7f)) == con(0x0))
      continue;
    x16 wordrg100 = urshift(recoptrrg5->u2.w, con(0x7));
    bool vbool_e8 = argbool0;
    x32 wordrg420 = vw_104;
    if (wordrg420 != con(0xffffffff))
    {
      if (wordrg420 != urshift(wordrg431, con(0xe)))
        continue;
      ui16 uwordrg42 = recoptrrg5->u2.w & con(0x7f);
      if (uwordrg42 < con(0x29))
      {
        if (uwordrg42 != con(0x1a))
          continue;
        if (vbool_ec)
          continue;
        if (ulrshift(recoptrrg5->u4.w, con(0xd), con(0xf)) == (argbool0 ? 1 : 0))
          continue;
        if (vw_110 != DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0xe), SKW_GET_WALL_DECORATION_OF_ACTUATOR(recoptrrg5)))
          continue;
        recoptrrg5->u4.b.b0 &= con(0xfffffffb);
        recoptrrg5->u4.w |= argbool0 ? 4 : 0;
      }
      else if (uwordrg42 <= con(0x29))
      {
        flag = vw_f4 != con(0x0);
        if (flag)
          continue;
      }
      else if (uwordrg42 <= con(0x2a))
      {
        if (vbool_ec)
          continue;
        flag = urshift(recoptrrg5->u2.w, con(0x7)) != vw_110;
        if (flag)
          continue;
      }
      else if (uwordrg42 == con(0x2b))
      {
        if (vbool_e4)
          continue;
        if (wordrg100 == vw_110)
          continue;
      }
      else
        continue;
    }
    else
    {
      ui16 uwordrg4 = (recoptrrg5->u2.w & con(0x7f)) - con(0x1);
      if (uwordrg4 > con(0x7))
        continue;
      switch (uwordrg4)
      {
        case 0:
          if (!ecxbool)
            flag = vw_f4 != con(0x0);
// TODO: check flag here (maybe uninitialized)
          if (flag || (flag = vbool_fc)) // yes, flag is assignment
            continue;
        break;

        case 1:
          if (vw_10c <= con(0x4))
            flag = ecxbool;
// TODO: check flag here (maybe uninitialized)
          if (flag || (flag = vbool_fc)) // yes, flag is assignment
            continue;
        break;

        case 2:
          if (vw_10c != wordrg420 || ddata.savegamew2 == con(0x0))
            continue;
          if (wordrg100 != con(0x0))
          {
            if (argbool0)
              vbool_e8 = wordrg100 == (ddata.v1e0258 + con(0x1));
            else
              vbool_e8 = argbool0;
          }
          else if (flag = ecxbool) // yes, assignment!
            continue;
        break;

        case 3:
          if (wordrg100 != vw_110 || (flag = vbool_ec)) // BUGFIX 4.3/2014, yes assignment
            continue;
        break;

        case 4:
        case 5:
          continue;

        case 6:
          if (vw_10c > con(0x4) || vw_10c == wordrg420 || (flag = vbool_fc)) // yes, flag is assignment
            continue;
        break;

        case 7:
          if (vw_10c != wordrg420)
            continue;
          vbool_e8 = SKW_2fcf_16ff(wordrg100);
        break;

        default: throw(THROW_DMABORT);
      }
    }
    // M_3D4C9:
    bool boolrg3 = vbool_e8;
    boolrg3 ^= (unsignedlong(ulrshift(recoptrrg5->u4.w, con(0xa), con(0xf))) != con(0x0));
    vbool_e8 = boolrg3;
    vw_108 = ulrshift(recoptrrg5->u4.w, con(0xb), con(0xe));
    if (vw_108 != con(0x3))
    {
      if (!boolrg3)
        continue;
    }
    else
      vw_108 = !boolrg3 ? 1 : 0;
    if ((recoptrrg5->u4.b.b0 & con(0x40)) != con(0x0))
    {
      x8 byterg4;
      if (vw_104 != con(0xffffffff))
        byterg4 = SKW_GET_WALL_DECORATION_OF_ACTUATOR(recoptrrg5);
      else
        byterg4 = SKW_GET_FLOOR_DECORATION_OF_ACTUATOR(recoptrrg5);
      vb_118 = ((vw_104 == con(0xffffffff)) ? 1 : 0) + con(0x9);
      SKW_QUEUE_NOISE_GEN2(vb_118, con(0x88), con(0xfe), byterg4, vw_0c, vw_08, con(0x1), con(0x8c), con(0x80));
    }
    SKW_INVOKE_ACTUATOR(recoptrrg5, con(0x0), vw_108);
  }
}

void SKW_STEP_MISSILE(c_tim* eaxptim)
{
  x16 wordrg5;
  c_tim vtim_00;
  c_tim* timptrrg1;
  c_record* recoptrrg7;
  c_record* recoptrrg3;
  c_record* recoptrrg2;
  x8** bpptrrg11;
  x8* bptrrg12;
  x16 vw_1c;
  x16 vw_20;
  x16 vw_24;
  x16 vw_28;
  x16 vw_2c;
  x16 vw_30;

  SKW_COPY_TIMER(eaxptim, &vtim_00);
  vw_30 = vtim_00.u.w_06;
  vw_28 = vtim_00.u.w_06;
  recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(vtim_00.u.w_06);
  recoptrrg7 = recoptrrg2;
  x16 vw_38 = CUTX16(vtim_00.v.l_08) & con(0x1f);
  x16 vw_34 = ulrshift(vtim_00.v.l_08, con(0x6), con(0xb));
  if (vtim_00.b_04 != con(0x1d))
  {
    x16 wordrg65 = urshift(vw_30, con(0xe));
    x16 wordrg142 = SKW_GET_CREATURE_AT(vw_38, vw_34);
    if (wordrg142 == con(0xffffffff) || ((recoptrrg3 = SKW_GET_ADDRESS_OF_RECORD(wordrg142), (SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg142) & con(0x2)) == con(0x0)) || (UI16CAST(recoptrrg2->u2.w) < UI16CAST(con(0xffffff80)))))
    {
      if (ddata.v1d3248 == ddata.v1e0282 && vw_38 == ddata.v1e0280 && vw_34 == ddata.v1e027e)
        if (SKW_075f_0af9(con(0xfffffffd), vw_34, wordrg65, vw_38, vw_30))
          return;
      if (SKW_075f_0af9(con(0xffffffff), vw_34, wordrg65, vw_38, vw_28))
        return;
    }
    else
    {
      x16 wordrg412 = 4 * wordrg65 + ((ulrshift(recoptrrg3->ue.w, con(0x6), con(0xe)) & con(0x1)) << con(0x4));
      x16 wordrg144 = ulrshift(vtim_00.v.l_08, con(0x4), con(0xe)) + wordrg412;
      x16 wordrg411 = signedword(table1d26a8[wordrg144]);
      if (table1d26a8[wordrg144] < con(0x4))
      {
        wordrg411 &= con(0x3);
        vtim_00.v.b.b_09 &= con(0xfffffff3);
        wordrg411 <<= con(0xa);
        vtim_00.v.w_08 |= wordrg411;
        ddata.savegametimarrp[recoptrrg2->u6.w].v.b.b_09 &= con(0xfffffff3);
        ddata.savegametimarrp[recoptrrg2->u6.w].v.w_08 |= wordrg411;
      }
    }
    x16 wordrg27 = urshift(vtim_00.v.w_08, con(0xc));
    x8 byterg42 = recoptrrg7->u4.b.b0;
    if (unsignedword(recoptrrg7->u4.b.b0) <= wordrg27)
    {
      wordrg5 = vw_38;
      SKW_CUT_RECORD_FROM(vw_28, wordrg5, vw_34, NULL);
      SKW_DELETE_MISSILE_RECORD(vw_28, wordrg5, vw_34, NULL);
      return;
    }
    byterg42 -= CUTX8(wordrg27);
    recoptrrg7->u4.b.b0 = byterg42;
    if (unsignedword(recoptrrg7->u4.b.b1) >= wordrg27)
      recoptrrg7->u4.b.b1 = recoptrrg7->u4.b.b1 - CUTX8(wordrg27);
    else
      recoptrrg7->u4.b.b1 = con(0x0);
  }
  else
    vtim_00.b_04 = con(0x1e);
  vw_30 = vtim_00.u.w_06;
  x16 wordrg6 = urshift(vw_30, con(0xe));
  x16 wordrg411 = wordrg6;
  vw_24 = ulrshift(vtim_00.v.w_08, con(0x4), con(0xe));
  x16 wordrg189 = vw_24;
  if (wordrg189 == wordrg411)
    vw_20 = con(0x1);
  else
  {
    wordrg189++;
    wordrg189 &= con(0x3);
    if (wordrg189 != wordrg411)
      vw_20 = con(0x0);
    else
      vw_20 = con(0x1);
  }
  if (vw_20 != con(0x0))
  {
    vw_1c = vw_38;
    vw_2c = vw_34;
    vw_38 += table1d27fc[vw_24];
    vw_34 += table1d2804[vw_24];
    bool pass = false; // added
    x8 byterg11 = SKW_GET_TILE_VALUE(vw_38, vw_34);
    x16 wordrg26 = unsignedword(byterg11);
    if (ubrshift(byterg11, con(0x5)) != con(0x0))
    {
      if ((urshift(wordrg26, con(0x5)) & con(0xff)) != con(0x6) || (wordrg26 & con(0xff05)) != con(0x0))
      {
        if ((urshift(wordrg26, con(0x5)) & con(0xff)) != con(0x3))
          pass = true;
        else
        {
          bpptrrg11 = &ddata.savegamepp1[vw_1c];
          bptrrg12 = *bpptrrg11;
          if (ubrshift(bptrrg12[vw_2c], con(0x5)) != con(0x3))
            pass = true;
        }
      }
    }
    if (!pass)
      if (SKW_075f_0af9(unsignedword(CUTX8(urshift(wordrg26, con(0x5)))), vw_2c, wordrg6, vw_1c, vw_30))
        return;
    x16 wordrg188 = SKW_GET_WALL_TILE_ANYITEM_RECORD(vw_38, vw_34);
    bool skip = false; // added
    for(;;)
    {
      x16 wordrg46;
      if (wordrg46 = wordrg188, wordrg46 == con(0xfffffffe))
      {
        skip = true;
        break;
      }
      if (urshift(wordrg46 & con(0x3c00), con(0xa)) == con(0xf) && (SKW_GET_ADDRESS_OF_RECORD(wordrg46)->u2.w & con(0x7f)) == con(0xe))
        break;
      wordrg188 = SKW_GET_NEXT_RECORD_LINK(wordrg46);
    }
    if (!skip)
    {
      timptrrg1 = &ddata.savegametimarrp[recoptrrg7->u6.w];
      x16 wordrg45 = ((ulrshift(timptrrg1->v.w_08, con(0x4), con(0xe)) + con(0x2)) & con(0x3)) << con(0xa);
      vtim_00.v.b.b_09 &= con(0xfffffff3);
      vtim_00.v.w_08 |= wordrg45;
      timptrrg1->v.b.b_09 &= con(0xfffffff3);
      timptrrg1->v.w_08 |= wordrg45;
      vw_38 = vw_1c;
      vw_34 = vw_2c;
      vw_20 = con(0x0);
    }
  }
  x16 wordrg24 = wordrg6;
  if ((vw_24 & con(0x1)) != (wordrg6 & con(0x1)))
    wordrg6++;
  else
    wordrg6--;
  wordrg6 &= con(0x3);
  if (vw_20 == con(0x0))
  {
    if ((((SKW_GET_TILE_VALUE(vw_38, vw_34) & con(0xff)) >> con(0x5)) & con(0xff)) == con(0x4))
      if (SKW_075f_0af9(con(0x4), vw_34, wordrg24, vw_38, vw_28))
        return;
    SKW_CUT_RECORD_FROM(vw_30, vw_38, vw_34, NULL);
    wordrg6 <<= con(0xe);
    vw_30 = (vw_30 & con(0x3fff)) | wordrg6;
    SKW_APPEND_RECORD_TO(vw_30, vw_38, vw_34, NULL);
  }
  else
  {
    wordrg6 <<= con(0xe);
    wordrg6 |= vw_30 & con(0x3fff);
    SKW_MOVE_RECORD_TO(wordrg6, vw_2c, vw_38, vw_1c, vw_34);
    if (recoptrrg7->u2.w == con(0xffffff80))
      if (ddata.v1d3248 == ddata.v1e0266)
        ddata.v1e0390.b0 |= con(0x1);
    vtim_00.v.b.b_08 &= con(0xffffffe0);
    x16 wordrg22 = vtim_00.v.w_08 | (ddata.v1e102a & con(0x1f));
    vtim_00.v.w_08 = wordrg22;
    vtim_00.v.w_08 = (wordrg22 & con(0xfffffc1f)) | ((ddata.v1e1028 & con(0x1f)) << con(0x5));
    vtim_00.v.b.b_09 &= con(0xfffffff3);
    vtim_00.v.w_08 |= (ddata.v1e1024 & con(0x3)) << con(0xa);
    x16 wordrg410 = vw_30 & con(0x3fff);
    wordrg410 |= ddata.v1e1026 << con(0xe);
    vw_30 = wordrg410;
    vtim_00.un.l_00 = vtim_00.un.l_00 & con(0xffffff) | (unsignedlong(ddata.v1e102c) << con(0x18));
    vw_38 = ddata.v1d3248;
    if (ddata.v1e102c != vw_38)
      SKW_CHANGE_CURRENT_MAP_TO(ddata.v1e102c);
    x16 wordrg21 = ddata.v1e102a += table1d27fc[ddata.v1e1024];
    ddata.v1e102a = wordrg21;
    ddata.v1e1028 += table1d2804[ddata.v1e1024];
    x16 wordrg151 = SKW_GET_CREATURE_AT(wordrg21, ddata.v1e1028);
    if (wordrg151 != con(0xffffffff))
      SKW_ATTACK_CREATURE(wordrg151, ddata.v1e1028, con(0x2006), ddata.v1e102a, con(0x64), con(0x0));
    x8 byterg21 = SKW_GET_TILE_VALUE(ddata.v1e102a, ddata.v1e1028);
    x16 wordrg63 = unsignedword(ubrshift(byterg21, con(0x5)));
    if (wordrg63 != con(0x0) && (wordrg63 != con(0x6) || (byterg21 & con(0x4)) != con(0x0)))
      if (wordrg63 != con(0x4) || (byterg21 & con(0x7)) == con(0x5) || (byterg21 & con(0x7)) <= con(0x1))
      {
        x16 wordrg64 = ddata.v1e102a + table1d27fc[ddata.v1e1024];
        ddata.v1e102a = wordrg64;
        ddata.v1e1028 += table1d2804[ddata.v1e1024];
        x16 wordrg142 = SKW_GET_CREATURE_AT(wordrg64, ddata.v1e1028);
        if (wordrg142 != con(0xffffffff))
          SKW_ATTACK_CREATURE(wordrg142, ddata.v1e1028, con(0x2006), ddata.v1e102a, con(0x64), con(0x0));
      }
    if (ddata.v1e102c != vw_38)
      SKW_CHANGE_CURRENT_MAP_TO(vw_38);
  }
  vtim_00.un.l_00 += con(0x1);
  vtim_00.u.w_06 = vw_30;
  recoptrrg7->u6.w = SKW_QUEUE_TIMER(&vtim_00);
}

x16 SKW_2759_222e(x16 eaxw, x16 ebxw, s_802* edxps)
{
  x16 wordrg3;
  x16 wordrg5;
  c_record* recoptrrg4;
  c_hero* heroptrrg7;
  s_d556* sd5ptrrg4;
  c_record* recoptrrg1;
  c_tim vtim_04;
  x16 vw_18;
  x16 vw_1c;
  x16 vw_20;
  x16 vw_24;

  x16 vw_10;
  vw_24 = ebxw;
  heroptrrg7 = &ddata.hero[eaxw];
  x32 wordrg4 = urshift(edxps->w6, con(0xa)) & con(0x3f);
  wordrg4 *= vw_24 + con(0x12);
  vw_18 = wordrg4 / con(0x18);
  wordrg5 = vw_24 + unsignedword(edxps->b4);
  x16 wordrg2 = 8 * (unsignedword(edxps->b4) * (vw_24 - con(0x1)));
  wordrg2 += 16 * wordrg5 + (CUTX16(SKW_RAND()) & con(0x7));
  wordrg2 += wordrg5 * wordrg5;
  vw_1c = wordrg2;
  ui16 uwordrg1 = SKW_QUERY_PLAYER_SKILL_LV(eaxw, true, unsignedword(edxps->b5));
  vw_20 = uwordrg1;
  x16 wordrg32 = wordrg5 - uwordrg1;

  // SPX: moved variables
  x16 wordrg21;

  // M_3244E:
  do
  {
    bool doit;
    if (wordrg32-- <= con(0x0))
    {
      x16 wordrg170 = (edxps->w6 & con(0xf)) - con(0x1);
      if (wordrg170 <= con(0x3))
      {
        x16 wordrg411 = 2 * vw_20;
        switch (wordrg170)
        {
          case 0:
            {
              x16 wordrg24 = SKW_FIND_HAND_WITH_EMPTY_FLASK(heroptrrg7);
              if (wordrg24 < con(0x0))
              {
                wordrg3 = con(0x30);
                goto M_exit;
              }
              x16 wordrg7 = SKW_REMOVE_POSSESSION(eaxw, wordrg24);
              vw_10 = urshift(edxps->w6, con(0x4)) & con(0x3f);
              SKW_SET_ITEMTYPE(wordrg7, CUTX8(vw_10));
              recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(wordrg7);
              recoptrrg4->u2.b.b0 = con(0x0);
              recoptrrg4->u2.w |= unsignedword(CUTX8(unsignedlong(vw_24) * con(0x28) + CUTX16(SKW_RAND()) & con(0xf)));
              SKW_EQUIP_ITEM_TO_HAND(eaxw, wordrg24, wordrg7);
            }
          break;

          case 1:
            {
              if ((urshift(edxps->w6, con(0x4)) & con(0x3f)) == con(0x4))
                vw_20 = wordrg411;
              x16 wordrg23 = DM2_BETWEEN_VALUE(con(0x15), con(0xff), (2 * vw_20 + con(0x4)) * (vw_24 + con(0x2)));
              SKW_CAST_CHAMPION_MISSILE_SPELL(eaxw, wordrg23, con(0x0), (urshift(edxps->w6, con(0x4)) & con(0x3f)) + con(0xff80));
            }
          break;

          case 2:
            {
              vtim_04.b_05 = con(0x0);
              wordrg5 = (vw_24 + 1) * 4;
              ui16 uwordrg11 = urshift(edxps->w6, con(0x4)) & con(0x3f);
              if (UI16CAST(uwordrg11) <= UI16CAST(con(0xf)))
              {
                // switch splitted here
                x16 secondswitch = uwordrg11; // extravariable
                bool doit = false; // extraflag
                x16 wordrg31;
                switch (uwordrg11)
                {
                  case 0:
                    wordrg31 = con(0x27);
                  break;

                  case 1:
                    wordrg31 = con(0x6);
                  break;

                  case 2:
                    wordrg31 = con(0x1);
                  break;

                  case 3:
                    vtim_04.b_04 = con(0x47);
                    if (ddata.savegames1.b2++ == con(0x0) && ddata.v1e0976 != con(0x0))
                      ddata.hero[ddata.v1e0976 - con(0x1)].u32.b.b1 |= con(0x40);
                  break;

                  case 4:
                    wordrg31 = con(0x2);
                  break;

                  case 5:
                    wordrg31 = con(0x26);
                  break;

                  case 6:
                    wordrg31 = con(0x5);
                  break;

                  case 7:
                    wordrg31 = con(0x4);
                  break;

                  case 8:
                    wordrg31 = con(0x0);
                  break;

                  case 9:
                    wordrg31 = con(0x6);
                  break;

                  case 10:
                    wordrg31 = con(0x3);
                  break;

                  case 11:
                    ddata.savegames1.b4 = CUTX8(DM2_MIN(8 * wordrg5 + unsignedword(ddata.savegames1.b4), con(0xff)));
                  break;

                  case 12:
                  case 13:
                  break;

                  case 14:
                    wordrg31 = ddata.v1e0272;
                    wordrg5 = ddata.v1e0270;
                    SKW_CREATE_CLOUD(con(0xff8e), wordrg5, wordrg31, DM2_BETWEEN_VALUE(con(0x15), con(0xff), (wordrg411 + con(0x4)) * (vw_24 + con(0x2))), con(0xff));
                  break;

                  case 15:
                    {
                      wordrg31 = con(0x42);
                      x16 wordrg410 = SKW_ALLOC_NEW_DBITEM(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xd), con(0xb), con(0x42), con(0xf)));
                      if (ddata.savegamewpc.w0 != con(0xffffffff))
                      {
                        wordrg5 = (SKW_RAND01() + ddata.v1e0258) & con(0x3);
                        wordrg31 = signedlong(ddata.v1e0270);
                        wordrg410 &= con(0x3fff);
                        wordrg5 <<= con(0xe);
                        wordrg5 |= wordrg410;
                        SKW_MOVE_RECORD_TO(wordrg5, con(0x0), wordrg31, con(0xffffffff), ddata.v1e0272);
                      }
                      else
                        SKW_TAKE_OBJECT(wordrg410, true);
                    }
                  break;

                  default: throw(THROW_DMABORT);
                }
                switch (secondswitch)
                {
                  case 2:
                  case 8:
                    wordrg5 *= wordrg5;
                    wordrg5 += con(0x64);
                    doit = true;
                  break;

                  case 6:
                  case 7:
                  case 9:
                  case 10:
                    wordrg5 += con(0x3);
                  case 4: // no break before!
                    wordrg5 *= wordrg5;
                    doit = true;
                  break;
                }
                if (doit)
                  SKW_2c1d_0186(heroptrrg7, wordrg5, con(0x0), CUTX8(wordrg31));
                else
                {
                  switch (secondswitch)
                  {
                    case 0:
                    case 1:
                    case 5:
                      wordrg5 *= con(0x9);
                      SKW_2759_15d0(wordrg31, wordrg5);
                    break;

                    case 3:
                      wordrg5 *= 8;
                      vtim_04.un.l_00 = (ddata.longmallocx + unsignedlong(wordrg5)) | (signedlong(ddata.v1e0266) << con(0x18));
                      SKW_QUEUE_TIMER(&vtim_04);
                    break;

                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    break;
                  }
                }
              }
            }
          break;

          case 3:
            doit = false;
            if (((unsignedlong(edxps->w6) >> 4) & con(0x3f)) != con(0x35))
              doit = true;
            else
            {
              recoptrrg1 = ddata.savegameparr5[4];
              wordrg5 = ddata.savegamep4->w0[con(0xa)];
              // M_32639: // co-label M_32636
              for (;; recoptrrg1++)
              {
                wordrg5--;
                if (wordrg5 == con(0xffffffff))
                {
                  doit = true;
                  break;
                }
                if (recoptrrg1->w0 == con(0xffffffff))
                  continue;
                if (recoptrrg1->u4.b.b0 != con(0x35))
                  continue;
                if (recoptrrg1->u4.b.b1 == con(0xff))
                  continue;
                x16 wordrg22 = signedlong(recoptrrg1->u4.b.b1);
                sd5ptrrg4 = &ddata.v1e089c[wordrg22];
                if (sd5ptrrg4->u.v.b1a != con(0x13))
                {
                  sd5ptrrg4->u.v.barr16[con(0x1)] = con(0x13);
                  break;
                }
              }
            }
            if (doit)
            {
              // M_32640:
              x16 wordrg22 = (ddata.v1e0258 + con(0x2)) & con(0x3);
              wordrg5 = con(0x6);
              if (SKW_CREATE_MINION(((edxps->w6) >> 4) & con(0x3f), wordrg22, ddata.v1e0270, (2 * wordrg22 + SKW_RAND02() + vw_20) * vw_24 / wordrg5, ddata.v1e0272, ddata.v1e0266, con(0xffff), CUTX8(ddata.v1e0258)) == con(0xffffffff))
                SKW_CREATE_CLOUD(con(0xffa8), ddata.v1e0270, ddata.v1e0272, con(0x6e), con(0xff));
            }
          break;

          default: throw(THROW_DMABORT);
        }
      }
      wordrg3 = con(0x0);
      if (vw_18 != con(0x0))
      {
        SKW_ADJUST_SKILLS(eaxw, vw_1c, unsignedword(edxps->b5));
        SKW_ADJUST_HAND_COOLDOWN(eaxw, con(0x2), vw_18);
      }
      goto M_exit;
    }
    vw_10 = CUTX16(SKW_RAND()) & con(0x7f);
  } while (DM2_MIN(SKW_GET_PLAYER_ABILITY(heroptrrg7, con(0x0), con(0x3)) + con(0xf), con(0x73)) >= vw_10);
   wordrg21 = urshift(vw_1c, CUTX8(wordrg5) - CUTX8(vw_20));
  SKW_ADJUST_SKILLS(eaxw, wordrg21, unsignedword(edxps->b5));
  wordrg3 = con(0x10);
M_exit: // M_32928:
  if (wordrg3 != con(0x0))
  {
    wordrg5 = unsignedword(edxps->b5);
    if (wordrg5 > con(0x3))
      wordrg5 = (wordrg5 - con(0x4)) / 4;
    wordrg3 |= wordrg5;
  }
  return wordrg3;
}

x32 SKW_ENGAGE_COMMAND(x16 eaxw, x16 edxw)
{
  x16 wordrg34;
  c_hero* heroptrrg5;
  c_record* recoptrrg1;
  c_record* recoptrrg7;
  c_aispec* aiptrrg2;
  s_size8* sptrrg4;
  x16 vw_04;
  c_tim vtim_08;
  x16 vw_24;
  x16 vw_2c;
  x16 vw_34;
  x16 vw_38;
  x16 vw_3c;
  x16 vw_40;
  x16 vw_44;
  x16 vw_48;
  x16 vw_4c;
  x16 vw_50;
  x16 vw_58;
  x16 vw_5c;
  x16 vw_60;
  x16 vw_64;
  x8 vb_6c;
  x8 vb_70;
  bool flag;

	// SPX: moved variables before goto
  x16 secondjump;
  x16 wordrg2;
  x16 byterg49;
  x16 wordrg109;


  vw_04 = eaxw;
  x16 vw_00 = edxw;
  bool vbool_14 = false;
  vw_48 = vw_00 & con(0x8000);
  vw_00 &= con(0x7fff);
  ddata.v1e0b7f = ddata.v1e0b40[vw_00].b0;
  ddata.v1e0b80 = ddata.v1e0b40[vw_00].b1;
  ddata.v1e0b7e = ddata.v1e0b40[vw_00].b2;
  vw_34 = ddata.v1e0b76;
  vw_4c = ddata.v1e0b76 ^ con(0x1);
  heroptrrg5 = &ddata.hero[vw_04];
  if (heroptrrg5->w36 == con(0x0))
    return con(0x0);
  x16 vw_54 = SKW_QUERY_CUR_CMDSTR_ENTRY(con(0x2));
  vb_6c = SKW_QUERY_CUR_CMDSTR_ENTRY(con(0xd));
  vw_3c = con(0x0);
  heroptrrg5->b20[vw_34] = CUTX8(vw_54);
  recoptrrg7 = NULL;
  vw_58 = heroptrrg5->warrc3[vw_34];
  if (vw_58 != con(0xffffffff))
    recoptrrg7 = SKW_GET_ADDRESS_OF_RECORD(vw_58);
  vw_44 = unsignedword(heroptrrg5->b1c);
  vw_5c = ddata.v1e0270 + table1d27fc[heroptrrg5->b1c];
  vw_24 = ddata.v1e0272 + table1d2804[heroptrrg5->b1c];
  ddata.v1e0b4c = SKW_GET_CREATURE_AT(vw_5c, vw_24);
  if (ddata.v1e0b4c != con(0xffffffff))
  {
    vb_70 = SKW_QUERY_CLS2_FROM_RECORD(ddata.v1e0b4c);
    aiptrrg2 = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(vb_70);
  }
  ui16 vuw_20 = SKW_QUERY_CUR_CMDSTR_ENTRY(con(0x3));
  vw_50 = SKW_QUERY_CUR_CMDSTR_ENTRY(con(0x0));
  vw_40 = SKW_QUERY_CUR_CMDSTR_ENTRY(con(0x4)) + SKW_RAND01();
  x16 vw_68 = SKW_QUERY_CUR_CMDSTR_ENTRY(con(0x9));
  vw_60 = SKW_QUERY_CUR_CMDSTR_ENTRY(con(0x5));
  vw_64 = SKW_QUERY_CUR_CMDSTR_ENTRY(con(0x10));
  x16 wordrg3 = vw_34;
  heroptrrg5->b42[wordrg3] = CUTX8(SKW_QUERY_CUR_CMDSTR_ENTRY(con(0x7)));
  vw_38 = unsignedword(SKW_GET_TILE_VALUE(vw_5c, vw_24));
  bool boolrg6 = true;
  ui16 uwordrg1 = vw_54 - con(0x1);
  if (uwordrg1 > con(0x35))
    goto M_exit;
   secondjump = uwordrg1; // 2nd switchgate for those not running to M_exit

  switch (uwordrg1)
  {
    case 0:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 36:
    case 39:
    case 40:
    case 42:
    case 51:
    case 52:
    goto M_exit;

    case 1:
      vw_60 = DM2_MAX(con(0x20), vw_60);
      vtim_08.b_04 = con(0x47);
      if (ddata.savegames1.b2++ == con(0x0))
        if (ddata.v1e0976 != con(0x0))
          ddata.hero[ddata.v1e0976 - con(0x1)].u32.b.b1 |= con(0x40);
      vtim_08.un.l_00 = (signedlong(vw_60) + ddata.longmallocx) | (signedlong(ddata.v1e0266) << con(0x18));
      SKW_QUEUE_TIMER(&vtim_08);
    goto M_exit;

    case 2:
      {
        x16 wordrg67 = SKW_QUERY_CUR_CMDSTR_ENTRY(con(0x6)) + con(0xff80);
        x16 wordrg163 = con(0x7) - DM2_MIN(con(0x6), SKW_QUERY_PLAYER_SKILL_LV(vw_04, true, vw_50));
        if (wordrg163 > heroptrrg5->w3e)
        {
          vw_60 = DM2_MAX(con(0x2), vw_60 * heroptrrg5->w3e / wordrg163);
          wordrg163 = heroptrrg5->w3e;
        }
        boolrg6 = SKW_CAST_CHAMPION_MISSILE_SPELL(vw_04, vw_60, wordrg163, wordrg67);
        if (!boolrg6)
          vw_68 >>= con(0x1);
      }
    goto M_exit;

    case 3:
    case 7:
      if ((urshift(vw_38, con(0x5)) & con(0xff)) != con(0x4) || (vw_38 & con(0x7)) != con(0x4))
      {
        if (ddata.v1e0b4c == con(0xffffffff) || (aiptrrg2->u0.b.b0 & con(0x1)) == con(0x0) || aiptrrg2->b2 != con(0xffffffff) || aiptrrg2->barr06[con(0x2)] != con(0xff))
        {
          boolrg6 = SKW_WIELD_WEAPON(vw_04, vw_5c, vw_24, SKW_QUERY_CUR_CMDSTR_ENTRY(con(0x6)), vw_50, vw_64);
          if (boolrg6)
          {
            if (ddata.v1e0b4c == con(0xffffffff))
              goto M_exit;
          }
          else
          {
            vw_68 >>= con(0x1);
            vuw_20 >>= con(0x1);
            goto M_exit;
          }
        }
        else
          vw_68 >>= con(0x1);
        SKW_QUEUE_NOISE_GEN1(con(0xf), con(0x8d), con(0x3c), vb_70, con(0xc8), vw_5c, vw_24, CUTX16(unsignedlong(vw_3c) + 1));
      }
      else
      {
        SKW_ATTACK_DOOR(vw_5c, SKW_COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(vw_04, vw_50, vw_34), false, vw_24, con(0x2));
        vuw_20 = con(0x6);
        recoptrrg1 = SKW_GET_ADDRESS_OF_TILE_RECORD(vw_5c, vw_24);
        vw_2c = vw_3c + con(0x1);
        SKW_QUEUE_NOISE_GEN2(con(0xe), con(0x8d), con(0xfe), SKW_IS_REBIRTH_ALTAR(recoptrrg1), vw_5c, vw_24, vw_2c, con(0x3c), con(0xc8));
      }
    goto M_exit;

    case 4:
      vw_60 += SKW_QUERY_PLAYER_SKILL_LV(vw_04, boolrg6, con(0xe));
      boolrg6 = SKW_CONFUSE_CREATURE(vw_60, vw_24, vw_5c);
      if (!boolrg6)
        vw_68 >>= con(0x2);
      vw_3c = con(0x0);
    goto M_exit;

    case 5:
    case 37:
    case 38:
      SKW_2759_15d0(vw_54, vw_60);
    goto M_exit;

    case 6:
      SKW_CREATE_CLOUD(con(0xff8e), ddata.v1e0270, ddata.v1e0272, DM2_MAX(con(0x2), vw_60), con(0xff));
    goto M_exit;

    case 8:
      vw_60 = DM2_MAX(con(0x20), vw_60);
      ddata.savegames1.b4 = CUTX8(DM2_MIN(unsignedword(ddata.savegames1.b4) + vw_60, con(0xff)));
    goto M_exit;

    case 9:
      {
        vw_5c = ddata.v1e0270 + table1d27fc[ddata.v1e0258];
        vw_24 = ddata.v1e0272 + table1d2804[ddata.v1e0258];
        bool boolrg7 = true;
        x16 wordrg160 = SKW_GET_CREATURE_AT(vw_5c, vw_24);
        if (wordrg160 != con(0xffffffff) && (SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg160) & con(0x8000)) == con(0x0))
          boolrg7 = false;
        if ((((SKW_GET_TILE_VALUE(vw_5c, vw_24) & con(0xff)) >> con(0x5)) & con(0xff)) != con(0x2) || !boolrg7)
          vuw_20 = con(0x0);
        else
        {
          ddata.v1e102e = con(0x1);
          SKW_MOVE_RECORD_TO(con(0xffff), ddata.v1e0272, vw_5c, ddata.v1e0270, vw_24);
          ddata.v1e102e = con(0x0);
        }
      }
    goto M_exit;

    case 10:
      ddata.savegames1.b3 = CUTX8(DM2_MIN(con(0xc8), unsignedword(ddata.savegames1.b3) + vw_60));
    goto M_exit;

    case 11:
      wordrg3 = con(0x5);
    break;

    case 12:
      wordrg3 = con(0x4);
    break;

    case 13:
      wordrg3 = con(0x6);
    break;

    case 14:
      wordrg3 = con(0x3);
    break;

    case 15:
      SKW_PLAYER_CONSUME_OBJECT(vw_04, vw_34, vw_58);
    goto M_exit;

    case 16:
      {
        vbool_14 = boolrg6;
        x16 wordrg159 = SKW_FIND_POUCH_OR_SCABBARD_POSSESSION_POS(vw_04, vw_34);
        if (wordrg159 >= con(0x0))
        {
          vw_58 = SKW_REMOVE_POSSESSION(vw_04, wordrg159);
          SKW_EQUIP_ITEM_TO_HAND(vw_04, wordrg3, vw_58);
        }
      }
    goto M_exit;

    case 31:
      if (SKW_IS_MISSILE_VALID_TO_LAUNCHER(vw_04, heroptrrg5->warrc3[vw_4c], wordrg3))
      {
        vw_2c = SKW_REMOVE_POSSESSION(vw_04, vw_4c);
        x16 wordrg32 = SKW_QUERY_PLAYER_SKILL_LV(vw_04, boolrg6, con(0xb));
        x16 tmp = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(vw_2c, con(0xc));
        x16 vw_18 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(vw_58, con(0xa)) + 2 * wordrg32;
        wordrg32 += SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(vw_2c, con(0x9)) + SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(vw_58, con(0x9));
        SKW_SHOOT_CHAMPION_MISSILE(heroptrrg5, wordrg32, vw_18, vw_2c, tmp);
        vbool_14 = boolrg6;
      }
      else
      {
        if (vw_48 == con(0x0))
        {
          ddata.v1e0b6c = DM2_MAX(con(0x1), con(0x8) - SKW_UPDATE_GLOB_VAR(con(0x43), con(0x3), true));
          ddata.v1e0b7c = con(0xfffffffe);
          ddata.v1e0b78 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(vw_58, con(0x5));
          ddata.v1e0b72 = vw_4c;
          vw_68 = con(0x0);
        }
        boolrg6 = false;
      }
    goto M_exit;

    case 32:
    case 33:
    case 34:
      x16 wordrg33;
      if (vw_54 != con(0x21))
      {
        if (vw_54 != con(0x22))
          wordrg33 = con(0x2);
        else
          wordrg33 = con(0x0);
      }
      else
        wordrg33 = con(0x1);
      vw_60 = DM2_MAX(con(0x20), vw_60) * con(0x3);
      wordrg2 = vw_60;
      byterg49 = CUTX8(wordrg33);
      wordrg3 = con(0x1);
    break;

    case 35:
      wordrg34 = heroptrrg5->w38 - heroptrrg5->w36;
      if (wordrg34 > con(0x0) && heroptrrg5->w3e != con(0x0))
      {
        x16 wordrg22 = DM2_MIN(con(0xa), SKW_QUERY_PLAYER_SKILL_LV(vw_04, boolrg6, con(0xd)));
        vw_68 = con(0x2);
        for(;;)
        {
          x16 wordrg60 = DM2_MIN(wordrg34, wordrg22);
          heroptrrg5->w36 += wordrg60;
          vw_68 += con(0x2);
          x16 wordrg158 = heroptrrg5->w3e - con(0x2);
          heroptrrg5->w3e = wordrg158;
          if (wordrg158 <= con(0x0) || (wordrg34 -= wordrg60, wordrg34 == con(0x0)))
          {
            if (heroptrrg5->w3e < con(0x0))
              heroptrrg5->w3e = con(0x0);
            heroptrrg5->u32.b.b1 |= con(0x8);
            boolrg6 = true;
            break;
          }
        }
      }
    goto M_exit;

    case 41:
      {
        x16 wordrg155 = ddata.v1e0258;
        x32 longrg22 = (signedlong(ddata.v1e0258) + (boolrg6 ? 1 : 0)) & con(0x3);
        x16 wordrg156;
        if (unsignedlong(heroptrrg5->b1d) != longrg22)
        {
          wordrg155 += con(0x2);
          wordrg155 &= con(0x3);
          if (unsignedword(heroptrrg5->b1d) != wordrg155)
            wordrg156 = con(0x0);
          else
            wordrg156 = con(0x1);
        }
        else
          wordrg156 = con(0x1);
        boolrg6 = SKW_2c1d_1de2(vw_04, wordrg156, vw_34);
      }
    goto M_exit;

    case 43:
      boolrg6 = SKW_SET_DESTINATION_OF_MINION_MAP(vw_58, ddata.v1e0272, ddata.v1e0266, ddata.v1e0270);
      if (!boolrg6)
      {
        vw_68 = con(0x0);
        vuw_20 = con(0x1);
      }
    goto M_exit;

    case 44:
    case 45:
      if (SKW_GET_MISSILE_REF_OF_MINION(recoptrrg7->u2.w, vw_58) == NULL)
      {
        if (vw_54 != con(0x2e))
        {
          vb_70 = con(0x32);
          vw_5c = ddata.v1e0270;
          vw_24 = ddata.v1e0272;
          wordrg2 = (ddata.v1e0258 + con(0x2)) & con(0x3);
          wordrg3 = ddata.v1e0266;
        }
        else
        {
          vb_70 = con(0x33);
          wordrg2 = ddata.v1e0258;
          vw_5c = recoptrrg7->u6.w & con(0x1f);
          vw_24 = ulrshift(recoptrrg7->u6.w, con(0x6), con(0xb));
          vw_44 = con(0xffffffff);
          wordrg3 = urshift(recoptrrg7->u6.w, con(0xa));
        }
      }
      else
      {
        SKW_GET_ADDRESS_OF_RECORD(recoptrrg7->u2.w)->u4.b.b0 = ((vw_54 != con(0x2d)) ? 1 : 0) + con(0x32);
        SKW_1c9a_0247(recoptrrg7->u2.w);
        goto M_exit;
      }
    break;

    case 46:
      vb_70 = con(0x30);
      vw_5c = ddata.v1e0270;
      vw_24 = ddata.v1e0272;
      wordrg2 = (ddata.v1e0258 + con(0x2)) & con(0x3);
      wordrg3 = ddata.v1e0266;
    break;

    case 47:
      SKW_RELEASE_MINION(recoptrrg7->u2.w);
    goto M_exit;

    case 48:
      wordrg109 = con(0x31);
    break;

    case 49:
      wordrg109 = con(0x34);
    break;

    case 50:
      wordrg109 = con(0x35);
    break;

    case 53:
      if (!SKW_LOAD_GDAT_INTERFACE_00_0A())
      {
        ddata.v1e0b6c = DM2_MAX(boolrg6 ? 1 : 0, con(0x8) - SKW_UPDATE_GLOB_VAR(con(0x47), con(0x3), true));
        ddata.v1e0b7c = con(0xfffffffa);
      }
    goto M_exit;

    default: throw(THROW_DMABORT);
  }
  switch (secondjump)
  {
    case 11:
    case 12:
    case 13:
    case 14: // no break for those
      wordrg2 = DM2_MAX(con(0x20), vw_60) << con(0x2);
      vw_60 = wordrg2;
      byterg49 = CUTX8(wordrg3);
      wordrg3 = con(0x0);
    case 32:
    case 33:
    case 34:
      if (!SKW_2c1d_0186(heroptrrg5, wordrg2, wordrg3, byterg49))
      {
        vw_68 >>= con(0x2);
        vuw_20 >>= con(0x1);
      }
    break;

    case 44:
    case 45:
    case 46:
      {
        x16 wordrg188;
        wordrg188 = SKW_CREATE_MINION(unsignedword(vb_70), wordrg2, vw_5c, SKW_QUERY_PLAYER_SKILL_LV(vw_04, true, vw_50), vw_24, wordrg3, vw_58, CUTX8(vw_44));
        flag = wordrg188 == con(0xffffffff);
        boolrg6 = wordrg188 != con(0xffffffff);
        if (flag)
        {
          vw_68 >>= con(0x2);
          vuw_20 >>= con(0x1);
          SKW_CREATE_CLOUD(con(0xffa8), vw_5c, vw_24, con(0x6e), con(0xff));
        }
        else
        {
          if (vw_54 == con(0x2f))
            SKW_SET_DESTINATION_OF_MINION_MAP(vw_58, ddata.v1e1028, ddata.v1e102c, ddata.v1e102a);
          recoptrrg7->u2.w = wordrg188;
        }
      }
    break;

    case 48:
    case 49:
    case 50:
      if (SKW_CREATE_MINION(wordrg109, (ddata.v1e0258 + con(0x2)) & con(0x3), ddata.v1e0270, urshift(vw_60, con(0x3)), ddata.v1e0272, ddata.v1e0266, vw_58, CUTX8(ddata.v1e0258)) == con(0xffffffff))
        SKW_CREATE_CLOUD(con(0xffa8), ddata.v1e0270, ddata.v1e0272, con(0x6e), con(0xff));
    break;

    default: throw(THROW_DMABORT);
  }
M_exit: // M_3219E:
  if (!vbool_14)
    SKW_QUEUE_NOISE_GEN2(ddata.v1e0b7f, vb_6c, con(0xfe), ddata.v1e0b80, ddata.v1e0270, ddata.v1e0272, vw_3c, con(0x73), con(0xc8));
  if (heroptrrg5->w36 == con(0x0) || vw_48 != con(0x0))
    return boolrg6;
  if (vuw_20 != con(0x0))
    SKW_ADJUST_HAND_COOLDOWN(vw_04, vw_34, vuw_20);
  if (vw_40 != con(0x0))
    SKW_ADJUST_STAMINA(vw_04, vw_40);
  if (vw_68 != con(0x0))
    SKW_ADJUST_SKILLS(vw_04, vw_68, vw_50);
  if (boolrg6)
  {
    x32 tmp = unsignedlong(vw_34) << con(0x3);
    sptrrg4 = &ddata.v1e0adc[vw_04][tmp];
    x16 wordrg12 = SKW_QUERY_CUR_CMDSTR_ENTRY(con(0xe));
    sptrrg4->w2 = wordrg12;
    if (wordrg12 != con(0x0))
    {
      sptrrg4->w0 = wordrg12;
      sptrrg4->b4 = con(0x1);
      sptrrg4->w6 = vw_58;
      sptrrg4->b5 = CUTX8(vw_00);
    }
  }
  return boolrg6;
}

// was SKW_24a5_036a
void SKW_ADD_BACKGROUND_LIGHT_FROM_TILE(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0)
{
  x16 wordrg5;
  c_record* recoptrrg1;
  c_record* recoptrrg3;
  x16 vw_04;
  x16 vw_08;
  s_8wordarray vs_0c;
  c_5bytes vs_1c;
  x16 vw_24;
  x8 vb_30;

  vw_08 = eaxw;
  vw_04 = ebxw;
  x16 wordrg6 = con(0x0);
  vw_24 = con(0x0);
  wordrg5 = con(0x0);
  SKW_SUMMARIZE_STONE_ROOM(&vs_0c, vw_04, ecxw, edxw);
  ui16 uwordrg3 = vs_0c.warr_00[0x0];
  x16 ret; // added, selects what to be processed in the end
  x16 vw_2c;
  x16 vw_28;
  if (uwordrg3 < con(0x1))
  {
    if (uwordrg3 == con(0x0))
    {
      if ((argw0 & con(0x4)) != con(0x0))
      {
        vw_2c = vs_0c.warr_00[0x5];
        vb_30 = CUTX8(vw_2c);
        if (vb_30 != con(0xffffffff))
        {
          vw_28 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0xf8), vb_30);
          if (vw_28 != con(0x0))
          {
            bool boolrg7 = (vw_28 & con(0x8000)) != con(0x0);
            vw_28 &= con(0x7fff);
            if (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0x63), vb_30) == con(0x0))
            {
              if (!boolrg7 || (vw_2c & con(0xff00)) != con(0x0))
                wordrg6 += vw_28;
            }
            else
              vw_24 += vw_28 * signedword(table1d6712[DM2_BETWEEN_VALUE(con(0x0), con(0x5), ddata.v1e1480 + ddata.v1e1476)]) / con(0x64);
          }
        }
      }
    }
    ret = 0;
  }
  else if (uwordrg3 <= con(0x2) || (uwordrg3 >= con(0x5) && (uwordrg3 <= con(0x5) || (uwordrg3 >= con(0x10) && uwordrg3 <= con(0x13)))))
  {
    if ((argw0 = con(0x1)) == con(0x0))
      ret = 1;
    else
    {
      x16 wordrg181 = vs_0c.warr_00[0x0];
      if (wordrg181 != con(0x1) && wordrg181 != con(0x2) && wordrg181 != con(0x5))
        ret = 2;
      else
      {
        vw_2c = vs_0c.warr_00[0x5];
        vb_30 = CUTX8(vw_2c);
        if (vb_30 == con(0xffffffff))
          ret = 2;
        else
        {
          x16 wordrg17 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xa), con(0xb), con(0xf8), vb_30);
          vw_28 = wordrg17;
          if (wordrg17 != con(0x0))
          {
            wordrg17 &= con(0x8000);
            bool boolrg3 = wordrg17 != con(0x0);
            vw_28 &= con(0x7fff);
            if (!SKW_GET_TELEPORTER_DETAIL(&vs_1c, ecxw, vw_04))
            {
              if (!boolrg3 || (vw_2c & con(0xff00)) != con(0x0))
                wordrg6 += vw_28;
            }
            else
              vw_24 += CUTX16(unsignedlong(vw_28) * signedlong(table1d6712[DM2_BETWEEN_VALUE(con(0x0), con(0x5), ddata.v1e1480 + ddata.v1e1476)]) / con(0x64));
            ret = 2;
          }
          else
            ret = 0;
        }
      }
    }
  }
  else
    ret = 0;
  // M_28F61:
  if (ret == 2)
  {
    x16 wordrg4 = vs_0c.warr_00[0x2]; // adj
    // M_28F65: // co-label M_28FF7
    for (;; wordrg4 = SKW_GET_NEXT_RECORD_LINK(wordrg4))
    {
      if (wordrg4 == con(0xfffffffe))
        break;
      x16 wordrg162 = urshift(wordrg4 & con(0x3c00), con(0xa));
      if (wordrg162 != con(0xf))
      {
        if (wordrg162 != con(0xe) || (recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg4), recoptrrg1->u2.w != con(0xffffff80)))
          continue;
        wordrg5 += unsignedword(recoptrrg1->u4.b.b0) / 2 - signedword(table1d673d[0x3]);
      }
      else
      {
        recoptrrg3 = SKW_GET_ADDRESS_OF_RECORD(wordrg4);
        x16 wordrg24 = recoptrrg3->u2.w & con(0x7f);
        if ((wordrg24 != con(0x2) && ((recoptrrg3->u2.b.b0 & con(0x7f)) != con(0x0))) && wordrg24 != con(0x30))
          continue;
        wordrg5 += urshift(recoptrrg3->u2.w, con(0x9));
      }
    }
    ret = 1;
  }
  // M_29008:
  if (ret == 1)
    if ((argw0 & con(0x2)) != con(0x0))
    {
      x16 wordrg171 = SKW_GET_CREATURE_AT(vw_04, ecxw);
      if (wordrg171 != con(0xffffffff))
        wordrg6 += (SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(wordrg171, con(0xf8)) & con(0x7fff));
    }
  // M_29043: always ends here
  if (vw_08 <= con(0x8) && vw_24 != con(0x0))
    vw_24 = DM2_MAX(con(0x3), vw_24 - table1d672b[vw_08]);
  else
    vw_24 = con(0x0);
  if (vw_08 > con(0x5))
    vw_08 = con(0x5);
  if (wordrg6 != con(0x0))
  {
    wordrg6 -= table1d673d[vw_08];
    wordrg6 = DM2_MAX(con(0x2), wordrg6);
  }
  wordrg6 += vw_24;
  ddata.v1e0974 += wordrg6;
  if (wordrg5 != con(0x0))
  {
    wordrg5 -= table1d673d[vw_08];
    wordrg5 = DM2_MAX(con(0x2), wordrg5);
  }
  ddata.v1e0978 += wordrg5;
}

bool SKW_075f_0af9(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0)
{
  ui16 uwordrg5;
  c_record* recoptrrg1;
  c_record* recoptrrg6;
  c_aispec* aiptrrg4;
  x16 vw_0c;
  x16* p16_10;
  c_record* prec_14;
  c_aispec* pai_18;
  x16 vw_30;
  x16 vw_34;
  x16 vw_38;
  x16 vw_40;
  x16 vw_48;
  x16 vw_4c;
  x16 vw_50;
  x16 vw_54;
  x16 vw_58;
  x16 vw_5c;
  x16 vw_60;

  bool skip = false; // added
  bool farskip = false; // added
  bool hugeskip = false; // added
  bool pass = false; // added
  vw_0c = eaxw;
  x16 vw_08 = edxw;
  x16 vw_04 = ebxw;
  vw_58 = con(0x0);
  vw_38 = con(0x0);
  recoptrrg6 = SKW_GET_ADDRESS_OF_RECORD(argw0);
  uwordrg5 = recoptrrg6->u2.w; // TODO: unsigned
  x16 vw_2c;
  x16 vw_3c;
  vw_34 = urshift(uwordrg5 & con(0x3c00), con(0xa));
  if (vw_34 == con(0x8))
  {
    prec_14 = SKW_GET_ADDRESS_OF_RECORD(uwordrg5);
    ui16 uwordrg1 = urshift(2 * prec_14->u2.w, con(0x9));
    if (uwordrg1 >= con(0x3))
    {
      bool doit = false; // added
      if (uwordrg1 <= con(0x3))
      {
        vw_2c = con(0xff87);
        doit = true;
      }
      else if (uwordrg1 == con(0x13))
      {
        vw_2c = con(0xff80);
        doit = true;
      }
      if (doit)
      {
        vw_38 = con(0x1);
        vw_3c = unsignedword(CUTX8(prec_14->u2.w));
        vw_40 = uwordrg5; // TODO: unsigned
      }
    }
  }
  if (vw_34 != con(0xf) || uwordrg5 == con(0xff81) || uwordrg5 == con(0xff86))
    vw_50 = con(0x0);
  else
    vw_50 = con(0x1);
  p16_10 = NULL;
  bool vbool_20 = false;
  if (vw_08 > con(0xff))
  {
    vw_54 = (vw_08 >> con(0x8)) - con(0x1);
    vw_60 = unsignedword(vw_04 >> con(0x1));
    vw_08 &= con(0xff);
    vw_04 &= con(0xff);
  }
  else
  {
    vw_54 = vw_08;
    vw_60 = vw_04;
  }
  x16 vw_28 = vw_54;
  vw_30 = vw_54;
  x16 vw_24 = vw_60;
  vw_48 = vw_60;
  x16 vw_44;
  x16 wordrg2 = vw_0c;
  if (wordrg2 < con(0xffffffff))
  {
    if (wordrg2 >= con(0xfffffffd))
    {
      wordrg2 = SKW_GET_PLAYER_AT_POSITION(ecxw);
      if (wordrg2 >= con(0x0))
      {
        vbool_20 = true;
        vw_44 = SKW_075f_06bd(recoptrrg6, uwordrg5);
        if (vw_44 != con(0x0))
        {
          if (uwordrg5 == ucon16(0xff86))
            ddata.v1e01a4 *= 2;
          x16 wordrg32 = ulrshift(ddata.savegametimarrp[recoptrrg6->u6.w].v.w_08, con(0x4), con(0xe)) + con(0x2);
          if (unsignedword(ddata.hero[wordrg2].b1c) == wordrg32)
            ddata.v1e01a6 |= con(0x8000);
          if (SKW_WOUND_PLAYER(wordrg2, con(0xc), ddata.v1e01a6, vw_44) != con(0x0) && ddata.v1e01a4 != con(0x0))
            if ((CUTX16(SKW_RAND()) & con(0x7)) != con(0x0))
              SKW_PROCESS_POISON(wordrg2, ddata.v1e01a4);
        }
        if (vw_0c == con(0xfffffffe))
        {
          hugeskip = true;
          pass = true;
        }
      }
      else if (vw_0c != con(0xfffffffd))
        return false;
    }
    else
      hugeskip = true;
    pass = true;
  }
  if (!pass)
  {
    if (wordrg2 != con(0xffffffff))
    {
      if (wordrg2 <= con(0x0))
      {
        x16 wordrg124 = ulrshift(ddata.savegametimarrp[recoptrrg6->u6.w].v.w_08, con(0x4), con(0xe));
        vw_4c = wordrg124;
        vw_30 = vw_54 + table1d27fc[wordrg124];
        vw_48 = vw_60 + table1d2804[wordrg124];
        if (SKW_075f_084d(vw_30, vw_54, vw_60, vw_48, wordrg124, argw0))
          return true;
      }
      else
      {
        if (wordrg2 == con(0x4))
        {
          x8 byterg3 = ddata.savegamepp1[vw_08][vw_04] & con(0x7);
          recoptrrg1 = SKW_GET_ADDRESS_OF_TILE_RECORD(vw_08, vw_04);
          if (byterg3 == con(0x5) || ((uwordrg5 != con(0xff84)) && uwordrg5 != con(0xff8d)))
          {
            if (byterg3 == con(0x5) || byterg3 <= con(0x1))
              return false;
            bool boolrg19;
            if (SKW_0cee_3275(SKW_IS_REBIRTH_ALTAR(recoptrrg1)) != con(0x0))
            {
              if (vw_34 != con(0xf))
              {
                x16 wordrg414 = CUTX16(SKW_RAND()) & con(0x7f);
                if (unsignedword(recoptrrg6->u4.b.b1) <= wordrg414 || (SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(uwordrg5, con(0x0)) & con(0x2)) == con(0x0))
                  boolrg19 = false;
                else
                  boolrg19 = true;
              }
              else
                boolrg19 = uwordrg5 >= con(0xff83);
              if (boolrg19)
                return false;
            }
            vw_44 = SKW_075f_06bd(recoptrrg6, uwordrg5) + con(0x1);
            SKW_ATTACK_DOOR(vw_08, SKW_RAND16(vw_44) + vw_44, false, vw_04, con(0x0));
          }
        }
      }
      hugeskip = true;
    }
  }
  pass = false; // prepare for 2nd usage

  // M_7700:
  if (!hugeskip)
  {
    vw_30 = vw_08;
    vw_48 = vw_04;
    vw_5c = SKW_1c9a_03cf(&vw_30, ecxw, &vw_48);
    bool pass = false;
    if (vw_5c != con(0xffffffff))
    {
      x16 wordrg411 = vw_5c;
      if (SKW_1c9a_0958(wordrg411))
      {
        if (vw_0c != con(0xfffffffd))
          return false;
        farskip = true;
        pass = true;
      }
      else
      {
        prec_14 = SKW_GET_ADDRESS_OF_RECORD(wordrg411);
        pai_18 = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(prec_14->u4.b.b0);
        if ((pai_18->u0.b.b0 & con(0x20)) == con(0x0) || uwordrg5 == con(0xff83))
        {
          if ((pai_18->u0.b.b0 & con(0x1)) != con(0x0))
          {
            if (((pai_18->u1e.b.b1 & con(0x1)) != con(0x0)) && (uwordrg5 < UI16CAST(con(0xffffff80))))
            {
              if (vw_0c != con(0xfffffffd))
                return false;
              farskip = true;
            }
            else
            {
              aiptrrg4 = pai_18;
              if (((aiptrrg4->u0.w >> con(0x6)) & con(0x3)) > con(0x1))
              {
                if (vw_0c != con(0xfffffffd))
                  return false;
                farskip = true;
              }
              else if ((aiptrrg4->u1e.b.b1 & con(0x8)) != con(0x0))
                skip = true;
            }
          }
          else if (pai_18->u1c.b.b1 != con(0xffffffff))
            skip = true;
          pass = true;
        }
      }
    }
    if (!pass)
    {
      if (vw_0c != con(0xfffffffd))
        return false;
      farskip = true;
    }
    if (!farskip)
    {
      if (!skip)
      {
        x16 wordrg102 = ulrshift(ddata.savegametimarrp[recoptrrg6->u6.w].v.w_08, con(0x4), con(0xe));
        vw_4c = wordrg102;
        wordrg102 += con(0x2);
        wordrg102 &= con(0x3);
        vw_4c = wordrg102;
        vw_28 += table1d27fc[vw_4c];
        vw_24 += table1d2804[vw_4c];
        x16 wordrg118 = urshift(argw0, con(0xe));
        if ((vw_4c & con(0x1)) != (wordrg118 & con(0x1)))
          wordrg118++;
        else
          wordrg118--;
        x16 wordrg410 = (wordrg118 & con(0x3)) << con(0xe);
        argw0 = (argw0 & con(0x3fff)) | wordrg410;
      }
      // M_784F:
      vw_44 = (SKW_075f_06bd(recoptrrg6, uwordrg5) << con(0x6)) / pai_18->b2;
      if (vw_44 != con(0x0))
      {
        x16 wordrg116 = SKW_APPLY_CREATURE_POISON_RESISTANCE(vw_5c, ddata.v1e01a4) + vw_44;
        aiptrrg4 = pai_18;
        if ((aiptrrg4->u18.b.b1 & con(0x10)) != con(0x0))
          if (ddata.v1e01a6 != con(0x1))
            wordrg116 >>= con(0x2);
        SKW_ATTACK_CREATURE(vw_5c, vw_48, con(0x200d), vw_30, con(0x64), wordrg116);
        if (vw_50 == con(0x0) && (pai_18->u0.b.b1 & con(0x2)) != con(0x0))
          if ((SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(uwordrg5, con(0x0)) & con(0x40)) != con(0x0))
            p16_10 = &prec_14->u2.w;
      }
      if (vw_38 != con(0x0) || vw_34 == con(0x5) || (vw_34 == con(0xf) && uwordrg5 != con(0xff84) && (uwordrg5 < con(0xff89) || uwordrg5 > con(0xff8e))))
      {
        x16 wordrg115;
        if (vw_44 >= con(0x5) || uwordrg5 == con(0xff87))
          wordrg115 = con(0x5f);
        else
          wordrg115 = con(0x1e);
        SKW_ATTACK_CREATURE(vw_5c, vw_48, con(0x2), vw_30, wordrg115, con(0x0));
      }
      vbool_20 = true;
    }
  }
  // M_7991:
  skip = false; // reset for 2nd usage
  if ((vw_0c == con(0xfffffffd)) && !vbool_20)
    return con(0x0);
  ui16 uwordrg4; // silly operations in the conditions left
  if (vw_50 != con(0x0) || vw_38 != con(0x0) || uwordrg5 == con(0xff86))
  {
    if (vw_38 == con(0x0))
    {
      uwordrg4 = unsignedword(recoptrrg6->u4.b.b0);
      if ((uwordrg5 == con(0xff86) && (uwordrg4 >>= con(0x2), uwordrg4 == con(0x0))))
        skip = true;
      else if (uwordrg5 == con(0xff82) && (uwordrg4 >>= con(0x1), uwordrg4 == con(0x0)))
        skip = true;
    }
    else
    {
      uwordrg4 = vw_3c;
      SKW_DEALLOC_RECORD(vw_40);
      uwordrg5 = vw_2c;
      recoptrrg6->u2.w = uwordrg5; // TODO: unsigned
    }
  }
  else
    skip = true;
  if (!skip)
  {
    if (uwordrg5 >= UI16CAST(con(0xffffff89)) && uwordrg5 <= UI16CAST(con(0xffffff8a)))
    {
      vw_4c = ulrshift(ddata.savegametimarrp[recoptrrg6->u6.w].v.w_08, con(0x4), con(0xe));
      vw_58 = unsignedword(recoptrrg6->u4.b.b0);
    }
    x16 wordrg191;
    if (uwordrg5 != con(0xff87) && uwordrg5 != con(0xff84) && uwordrg5 != con(0xff8d))
      wordrg191 = ecxw;
    else
      wordrg191 = con(0xff);
    SKW_CREATE_CLOUD(uwordrg5, vw_54, vw_60, unsignedlong(uwordrg4), wordrg191);
  }
  // M_7A98:
  SKW_CUT_RECORD_FROM(argw0, vw_54, vw_60, NULL);
  SKW_DELETE_MISSILE_RECORD(argw0, vw_28, vw_24, p16_10);
  if (vw_58 != con(0x0))
  {
    x16 wordrg3 = DM2_MAX(DM2_MIN(vw_58, con(0xfe)), con(0x28));
    x16 wordrg105;
    if (uwordrg5 != con(0xff89))
      wordrg105 = (vw_4c + con(0x2)) & con(0x3);
    else
      wordrg105 = vw_4c;
    SKW_12b4_0d75(vw_08, wordrg105, wordrg3, vw_04);
  }
  return true;
}

// TODO: BUG: uninitialized wordrg6 possible now (example: pick up a dagger from button)
x16 SKW_MOVE_RECORD_TO(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0)
{
  x16 wordrg6 = 0; // TODO: init shouldn't be necessary
  x16 wordrg5;
  c_tim* timptrrg1;
  c_record* prec_34;
  x16 vw_4c;
  x16 vw_50;
  x16 vw_58;
  x16 vw_5c;
  x16 vw_60;
  x8 vb_64;
  x16 vw_6c;
  x16 vw_70;
  x16 vw_74;
  x16 vw_78;
  x16 vw_7c;
  x16 vw_80;
  x16 vw_84;

	// SPX: moved variables before goto
	x16 wordrg41;
	x16 wordrg2;

  wordrg5 = eaxw;
  vw_60 = edxw;
  vw_58 = ebxw;
  x16 vw_68 = ecxw;
  ddata.v1e1020 = con(0x0);
  bool vbool_3c = false;
  bool vbool_44 = false;
  vw_84 = ddata.v1d3248;
  vw_80 = (ddata.v1e0258);
  if (wordrg5 != con(0xffffffff))
  {
    vw_78 = urshift(wordrg5 & con(0x3c00), con(0xa));
    if (vw_78 == con(0x4))
      prec_34 = SKW_GET_ADDRESS_OF_RECORD(wordrg5);
    vw_74 = urshift(wordrg5, con(0xe));
  }
  else
  {
    ddata.v1e0390.l0 = con(0x3);
    vw_78 = con(0xffffffff);
    if (vw_60 >= con(0x0))
      SKW_2fcf_0234(vw_60, vw_68, argw0, vw_58);
  }
  x16 vw_54 = con(0x0); // TODO: ATTENTION! ORIGINAL BUG? WAS UNINITIALIZED!
  if (vw_68 >= con(0x0))
  {
    wordrg6 = wordrg5;
    vbool_3c = SKW_2fcf_0434(wordrg6, vw_58, vw_68, vw_60, signedlong(argw0), true);
    if (wordrg5 != con(0xffffffff))
    {
      if (SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(wordrg6, con(0xf8)) != con(0x0))
        if (vw_84 == ddata.v1e0266 || ddata.v1e102c == ddata.v1e0266)
          ddata.v1e0390.b0 |= con(0x1);
      wordrg5 &= con(0xffff3fff);
      x16 wordrg177 = wordrg5;
      wordrg5 = ddata.v1e1026;
      wordrg5 <<= con(0xe);
      wordrg5 |= wordrg177;
    }
    vw_68 = ddata.v1e102a;
    argw0 = ddata.v1e1028;
    vw_54 = ddata.v1e1024;
    wordrg6 = ddata.v1e102c;
    SKW_CHANGE_CURRENT_MAP_TO(wordrg6);
    if (vw_78 == con(0x4))
      if (!SKW_IS_CREATURE_ALLOWED_ON_LEVEL(wordrg5, wordrg6))
      {
        SKW_DROP_CREATURE_POSSESSION(wordrg5, argw0, con(0x0), vw_68, con(0x1));
        SKW_CHANGE_CURRENT_MAP_TO(vw_84);
        wordrg6 = vw_60;
        if (wordrg6 >= con(0x0))
          SKW_DELETE_CREATURE_RECORD(wordrg6, con(0x2), con(0xffffffff), vw_58);
        return con(0x1);
      }
    if (wordrg6 != vw_84 || vw_68 != vw_60 || argw0 != vw_58)
      vbool_44 = false;
    else
      vbool_44 = true;
    if (!vbool_44)
    {
      if (wordrg6 != vw_84)
      {
        if (wordrg5 != con(0xffffffff) || ddata.v1e0266 == vw_84)
          SKW_CHANGE_CURRENT_MAP_TO(vw_84);
        else
          SKW_LOAD_LOCALLEVEL_GRAPHICS_TABLE(ddata.v1e0270, vw_84, ddata.v1e0272);
      }
    }
    else if (wordrg5 != con(0xffffffff))
    {
      if (ddata.v1e1026 == vw_74 && vw_78 != con(0xe))
        goto M_exit;
    }
    else if (ddata.v1e0258 == vw_80)
      goto M_exit;
  }
  // M_3B444:
  if (vw_60 >= con(0x0))
  {
    if (wordrg5 != con(0xffffffff))
    {
      if (!vbool_3c)
      {
        bool parbool06;
        if (vw_68 >= con(0x0) || argw0 >= con(0x0))
          parbool06 = false;
        else
          parbool06 = true;
        bool boolrg3;
        if (ddata.v1d3248 != ddata.v1e0282 || vw_60 != ddata.v1e0280 || vw_58 != ddata.v1e027e)
          boolrg3 = false;
        else
          boolrg3 = true;
        R_3CE7D(vw_60, wordrg5, boolrg3, vw_58, false, parbool06);
      }
      else
        SKW_CUT_RECORD_FROM(wordrg5, vw_60, vw_58, NULL);
      if (vw_78 == con(0x4) && vw_68 < con(0xffffffff))
      {
        vw_7c = unsignedword(prec_34->u4.b.b1);
        if (vw_7c != con(0xff))
          SKW_1c9a_0fcb(vw_7c);
      }
    }
    else
      R_3CE7D(vw_60, con(0xffff), vbool_44, vw_58, false, false);
  }
   wordrg2 = vw_68;
  if (wordrg2 < con(0x0))
    goto M_exit;
  if (wordrg5 != con(0xffffffff))
  {
    if (vw_78 != con(0x4))
    {
      SKW_CHANGE_CURRENT_MAP_TO(wordrg6);
      if (vw_78 != con(0xe))
      {
        x32 longrg21;
        if ((vw_60 >= con(0x0)) || (vw_58 >= con(0x0)))
          longrg21 = con(0x0);
        else
          longrg21 = con(0x1);
        if (longrg21 == con(0x0) && wordrg6 != vw_84)
        {
          x16 wordrg195 = SKW_GET_CREATURE_AT(vw_68, argw0);
          if (wordrg195 != con(0xffffffff))
            longrg21 = lextended(SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg195) & con(0x1)); // was just a word-to-word operation here
        }
        bool boolrg3;
        if (ddata.v1d3248 != ddata.v1e0282 || vw_68 != ddata.v1e0280 || argw0 != ddata.v1e027e)
          boolrg3 = false;
        else
          boolrg3 = true;
        R_3CE7D(vw_68, wordrg5, boolrg3, argw0, true, longrg21 != con(0x0));
      }
      else
        SKW_APPEND_RECORD_TO(wordrg5, vw_68, argw0, NULL);
      goto M_exit;
    }
    SKW_CHANGE_CURRENT_MAP_TO(wordrg6);
    vw_7c = unsignedword(prec_34->u4.b.b1);
    if (wordrg6 == ddata.v1e0282 && wordrg2 == ddata.v1e0280 && argw0 == ddata.v1e027e)
      if ((SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg5) & con(0x8000)) == con(0x0))
      {
        if (!SKW_TRY_PUSH_OBJECT_TO(con(0xffff), argw0, &vw_4c, vw_68, &vw_70))
          goto M_exit2;
        SKW_ATTACK_PARTY(SKW_RAND16(con(0xa)) + SKW_GET_CREATURE_WEIGHT(wordrg5) / 8, con(0x2), con(0x4));
        SKW_QUEUE_NOISE_GEN2(con(0x16), con(0x8a), con(0xfe), con(0xfe), vw_68, argw0, con(0x1), con(0x64), con(0xc8));
        SKW_MOVE_RECORD_TO(con(0xffff), argw0, vw_4c, vw_68, vw_70);
      }
  }
  else
  {
    SKW_CHANGE_CURRENT_MAP_TO(wordrg6);
    SKW_2fcf_0b8b(vw_68, wordrg6, argw0);
    wordrg5 = SKW_GET_CREATURE_AT(ddata.v1e0270, ddata.v1e0272);
    bool skip = false;
    x16 rg4helper = wordrg5;
    x16 wordrg165 = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg5);
    if (wordrg5 == con(0xffffffff) || (wordrg165 & con(0x8000)) != con(0x0))
      skip = true;
    else if ((wordrg165 & con(0x1)) == con(0x0) || SKW_GET_CREATURE_WEIGHT(rg4helper) <= con(0xfd))
    {
      if (SKW_TRY_PUSH_OBJECT_TO(wordrg5, argw0, &vw_4c, vw_68, &vw_70))
      {
        x16 wordrg108 = SKW_GET_PARTY_SPECIAL_FORCE();
        SKW_ATTACK_CREATURE(wordrg5, argw0, con(0x2000), vw_68, con(0x0), wordrg108 / 4 + (SKW_RAND16(wordrg108 / 2 + con(0x1))));
        SKW_MOVE_RECORD_TO(wordrg5, argw0, vw_4c, vw_68, vw_70);
        skip = true;
      }
    }
    else
    {
      if (SKW_TRY_PUSH_OBJECT_TO(con(0xffff), argw0, &vw_4c, vw_68, &vw_70))
      {
        SKW_LOAD_LOCALLEVEL_GRAPHICS_TABLE(vw_4c, argw0, vw_70);
        ddata.v1d27f8 = wordrg6;
        SKW_MOVE_RECORD_TO(con(0xffff), con(0x0), vw_4c, con(0xffffffff), vw_70);
        SKW_LOAD_LOCALLEVEL_GRAPHICS_TABLE(ddata.v1e0270, vw_84, ddata.v1e0272);
        if (ddata.v1d27f8 != vw_84)
          goto M_exit;
        ddata.v1d27f8 = con(0xffffffff);
        skip = true;
      }
    }
    if (!skip)
    {
      // M_3B6E5:
      SKW_1c9a_0db0(wordrg5);
      if (SKW_MOVE_RECORD_TO(wordrg5, ddata.v1e0272, con(0xfffffffd), ddata.v1e0270, con(0x0)) == con(0x0))
        SKW_2fcf_01c5(wordrg5, ddata.v1e0272, wordrg6, ddata.v1e0270, con(0x0));
    }
    // M_3B735:
    if (wordrg6 != vw_84)
      ddata.v1d27f8 = wordrg6;
    else
      R_3CE7D(ddata.v1e0270, con(0xffff), vbool_44, ddata.v1e0272, true, false);
    goto M_exit;
  }
  // M_3B885:
   wordrg41 = SKW_GET_CREATURE_AT(vw_68, argw0);
  vw_50 = wordrg41;
  if (vw_50 != con(0xffffffff))
  {
    vw_5c = SKW_GET_CREATURE_WEIGHT(wordrg5);
    vw_6c = vw_5c;
    x16 wordrg3 = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg41);
    vb_64 = CUTX8(wordrg3);
    if ((SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg5) & con(0x1)) == con(0x0) || vw_5c <= con(0xfd))
    {
      if (((vb_64 & con(0x1)) == con(0x0)) || SKW_GET_CREATURE_WEIGHT(vw_50) <= con(0xfd))
      {
        if (!SKW_TRY_PUSH_OBJECT_TO(vw_50, argw0, &vw_4c, vw_68, &vw_70))
          goto M_exit2;
      }
      else
      {
        if (!SKW_TRY_PUSH_OBJECT_TO(wordrg5, argw0, &vw_4c, vw_68, &vw_70))
          goto M_exit2;
        SKW_MOVE_RECORD_TO(wordrg5, con(0x0), vw_4c, con(0xffffffff), vw_70);
        goto M_exit;
      }
    }
    else if ((wordrg3 & con(0x1)) == con(0x0) || SKW_GET_CREATURE_WEIGHT(wordrg41) <= con(0xfd))
    {
      if (!SKW_TRY_PUSH_OBJECT_TO(vw_50, argw0, &vw_4c, vw_68, &vw_70))
      {
        SKW_1c9a_0db0(wordrg5);
        SKW_2fcf_01c5(wordrg5, argw0, wordrg6, vw_68, con(0x0));
        goto M_exit;
      }
    }
    else
      goto M_exit2;
    // M_3BA24:
    x16 wordrg109 = urshift(vw_6c, con(0x2));
    SKW_ATTACK_CREATURE(vw_50, argw0, con(0x2000), vw_68, con(0x0), wordrg109 + SKW_RAND16(wordrg109 / 2 + con(0x1)));
    SKW_MOVE_RECORD_TO(vw_50, argw0, vw_4c, vw_68, vw_70);
  }
  // M_3BA8D:
  if (!vbool_3c)
    R_3CE7D(vw_68, wordrg5, false, argw0, true, vbool_3c);
  else
    SKW_APPEND_RECORD_TO(wordrg5, vw_68, argw0, NULL);
  SKW_SET_MINION_RECENT_OPEN_DOOR_LOCATION(wordrg5, argw0, wordrg6, CUTX8(vw_68), false);
  if (vw_7c != con(0xff))
  {
    vw_7c = ddata.v1e089c[vw_7c].w02;
    x16 wordrg41 = vw_7c;
    if (wordrg41 != con(0xffffffff))
    {
      ddata.savegametimarrp[wordrg41].u.b.b_06 = CUTX8(vw_68);
      ddata.savegametimarrp[wordrg41].u.b.b_07 = CUTX8(argw0);
      timptrrg1 = &ddata.savegametimarrp[wordrg41];
      timptrrg1->un.l_00 = (unsignedlong(wordrg6) << con(0x18)) | (timptrrg1->un.l_00 & con(0xffffff));
    }
  }
  else if ((SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg5) & con(0x1)) == con(0x0))
    SKW_ALLOC_CAII_TO_CREATURE(wordrg5, argw0, vw_68);
M_exit: // M_3BC5B:
  ddata.v1e1024 = vw_54;
  ddata.v1e102a = vw_68;
  ddata.v1e1028 = argw0;
  ddata.v1e102c = wordrg6;
  SKW_CHANGE_CURRENT_MAP_TO(vw_84);
  return con(0x0);

M_exit2: // M_3B9E4:
  if (vw_7c != con(0xff))
    SKW_1c9a_0fcb(vw_7c);
  SKW_2fcf_01c5(wordrg5, argw0, wordrg6, vw_68, ddata.v1e1020);
  return con(0x2);
}

