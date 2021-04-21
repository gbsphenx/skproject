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

#include "dm2debug.h"

#include "../skwin/defines.h"

t_gfxdata* R_D2C(x16 eaxw, x16 ebxw, x16 edxw)
{
  x16 wordrg1;

  if (ebxw != con(0x4))
    wordrg1 = eaxw;
  else
    wordrg1 = ((eaxw + con(0x1)) & con(0xfffffffe)) >> con(0x1);
  t_gfxdata* gptrrg1 = R_4A986(wordrg1 * edxw + con(0x6)) + con(0x6);
  s_gfxdatatail* ptrbeforerg1 = getprevious(gptrrg1);
  ptrbeforerg1->b0 = con(0x8);
  ptrbeforerg1->width = eaxw;
  ptrbeforerg1->height = edxw;
  return gptrrg1;
}

void SKW_44c8_1c99(void)
{
  c_rect rc_00;

  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_QUERY_EXPANDED_RECT(con(0xf), &rc_00);
  SKW_COPY_RECT(&rc_00, ddata.drvinterfaceptr);
  i16* wptr = UPCAST(i16, ddata.drvinterfaceptr);
  wptr[con(0x4)] = ddata.v1d2746;
  userinterrupt(con(0x105), con(0x0));
  rc_00.y += rc_00.h - C_strmetrics.height;
  rc_00.h = C_strmetrics.height;
//                         srcbmp            srcofs  destbmp                                                                         srcbits    destbits
  SKW_FIRE_BLIT_PICTURE(ddata.v1e1414, &rc_00, 0, ddata.screenptr, con(0x0), ddata.v1d2744, ddata.mouse_unk0c, con(0xffff), con(0x0), E_BITRES8, E_BITRES8);
  SKW_FIRE_SHOW_MOUSE_CURSOR();
}

void SKW_075f_056c(x16 eaxw)
{
  if (urshift(eaxw & con(0x3c00), con(0xa)) == con(0xe))
    SKW_DELETE_TIMER(SKW_GET_ADDRESS_OF_RECORD(eaxw)->u6.w);
}

void SKW_443c_0434(void)
{
  ddata.mouse_unk0f = ddata.savegamewpc.w0 != con(0xffffffff);
  if (ddata.mouse_unk0f)
  {
    t_gfxdata* gptrrg5 = SKW_ALLOC_PICT_BUFF(con(0x12), BIGPOOL_LO, con(0x8), con(0x12));
    SKW_443c_0004(UPCAST(t_gfxdata, ddata.savegamewpc.p2), ddata.savegamewpc.barr6, gptrrg5);
    SKW_FIRE_MOUSE_EVENT_RECEIVER(con(0x2), con(0x8), con(0x8), gptrrg5, con(0x12), con(0x12), con(0x8), NULL, unsignedword(ddata.savegamewpc.barr6[0xc]));
    SKW_FREE_PICT_BUFF(gptrrg5);
  }
  SKW_443c_040e();
}

void SKW_443c_0004(t_gfxdata* eaxpg, t_gfxdata* ebxpg, t_gfxdata* edxpg)
{
  t_gfxdata vcarr_00[PAL16];

  t_gfxdata* pg_10 = SKW_ALLOC_PICT_BUFF(con(0x10), BIGPOOL_LO, con(0x4), con(0x10));
  for (x16 wordrg4 = con(0x0); wordrg4 < con(0x10); wordrg4++)
    vcarr_00[wordrg4] = ddata.paletteptr1[con(0x0)];
  vcarr_00[0xc] = ebxpg[0xc];
  SKW_FILL_ENTIRE_PICT(edxpg, vcarr_00[0xc]);
//                       srcbmp                                       srcofs destbmp                    palette
  SKW_DRAW_DIALOGUE_PICT(eaxpg, UPCAST(c_rect, &ddata.mouse_cur2[0x80]), 0, edxpg, con(0x0), con(0xc), vcarr_00);
//                       srcbmp                                        srcofs destbmp                  palette
  SKW_DRAW_DIALOGUE_PICT(eaxpg, UPCAST(c_rect, &ddata.mouse_cur2[0x88]), 0, edxpg, con(0x0), con(0xc), ebxpg);
  SKW_FREE_PICT_BUFF(pg_10);
}

void SKW_443c_06b4(s_munknown* eaxps)
{
  c_rect* rptrrg2;
  s_munknown* sptrrg2;
  s_munknown** spptrrg4;

  if ((eaxps->b3 & con(0x80)) == con(0x0))
  {
    eaxps->b3 |= con(0x80);
    spptrrg4 = &ddata.mouse_unkptr2;
    sptrrg2 = ddata.mouse_unkptr2;
    while ((sptrrg2 != NULL) && ((sptrrg2->b3 & con(0xf)) > (eaxps->b3 & con(0xf))))
    {
      spptrrg4 = sptrrg2->u.pm6;
      sptrrg2 = *spptrrg4;
    }
    SKW_443c_087c();
    eaxps->u.pb6->p0 = sptrrg2;
    *spptrrg4 = eaxps;
    rptrrg2 = &eaxps->u.pb6->r4;
    SKW_QUERY_EXPANDED_RECT(eaxps->w0 & con(0x3fff), rptrrg2);
    if ((eaxps->w0 & con(0x8000)) != con(0x0))
    {
      rptrrg2->x += ddata.v1d26fc;
      rptrrg2->y += ddata.v1d26fe;
    }
    if (eaxps->b5 != con(0x0))
    {
      SKW_443c_00a9(eaxps, rptrrg2->x + rptrrg2->w - 1, rptrrg2->y + rptrrg2->h - 1, rptrrg2->x, rptrrg2->y);
    }
    SKW_443c_0889();
    SKW_443c_040e();
  }
}

void SKW_443c_07d5(s_munknown* eaxps)
{
  s_munknown** spptrrg2;
  s_munknown* sptrrg4;

  if ((eaxps->b3 & con(0xffffff80)) == con(0x0))
    return;
  eaxps->b3 &= con(0x7f);
  spptrrg2 = &ddata.mouse_unkptr2;
  sptrrg4 = ddata.mouse_unkptr2;
  while (sptrrg4 != eaxps)
  {
    if (sptrrg4 == NULL)
      return;
    spptrrg2 = sptrrg4->u.pm6;
    sptrrg4 = *spptrrg2;
 }
  SKW_443c_087c();
  *spptrrg2 = sptrrg4->u.pb6->p0;
  SKW_443c_0889();
  SKW_443c_040e();
}

x16 R_4E26(x16* eaxpw)
{
  if ((*eaxpw & con(0x4000)) != con(0x0))
    return con(0x0);
  if ((*eaxpw & con(0x8000)) != con(0x0))
  {
    x16 wordrg4;
    if ((*eaxpw & con(0x1000)) == con(0x0))
      wordrg4 = urshift(*eaxpw & con(0xfc0), con(0x6));
    else
    {
      *eaxpw &= con(0xffffe03f);
      wordrg4 = con(0x0);
    }
    return (unsignedlong(wordrg4) + ddata.longmallocx) % (*eaxpw & con(0x3f));
  }
  return *eaxpw & con(0x3f);
}

//R_4EA8 renamed GET_ANIM_INFO_SEQUENCE
x16 GET_ANIM_INFO_SEQUENCE(x8 eaxb, x16 edxw)
{
  x8* bptrrg1 = SKW_QUERY_GDAT_ENTRY_DATA_PTR(con(0xf), con(0x7), con(0xfc), eaxb);
  bptrrg1 += (edxw << con(0x2));

  // SPX: add some protection when the pointer is not loaded (length info)
  if (((ui32) bptrrg1) & 0x80000000)
	  return 1;	// defaut is 1 here because it is used in modulo thereafter


  x16 wordrg4;
  for (wordrg4 = con(0x1); (*(bptrrg1 + con(0x1)) & con(0xf0)) == con(0x0); wordrg4++, bptrrg1 += con(0x4))
  {
  }
  return wordrg4;
}

void R_5BFB(x16 eaxw, x16 edxw)
{
  if (edxw != con(0x0))
  {
    if (edxw == con(0xa))
      ddata.v1d26a2 = eaxw;
  }
  else
  {
    ddata.v1d26a0 = eaxw;
    SKW_QUEUE_NOISE_GEN1(con(0x3), con(0x8b), con(0x6c), con(0x0), con(0xc8), ddata.v1e0270, ddata.v1e0272, con(0x0));
  }
  R_7E7(eaxw, edxw);
}

x16 SKW_GET_GLOB_VAR(x16 eaxw)
{
  if (eaxw > UI16CAST(con(0x3f)))
  {
    if (eaxw <= UI16CAST(con(0x7f)))
      return unsignedword(ddata.globalb[eaxw - 0x40]); // evil - index splitted
    if (eaxw <= UI16CAST(con(0xbf)))
      return ddata.v1e000c[eaxw];
  }
  else
  {
    if (((con(0x1) << (eaxw & con(0x7))) & ddata.v1e0104[eaxw / 8]) != con(0x0))
      return con(0x1);
  }
  return con(0x0);
}

// was SKW_4937_00cc, R_4EF3
//Bit16u SkWinCore::GET_CREATURE_ANIMATION_FRAME(Bit8u ct, Bit16u *pw08, Bit16u *pw0a, Bit16u command, CreatureAnimationFrame **animframe, Bit16u vv)
bool SKW_GET_CREATURE_ANIMATION_FRAME(x8 eaxb, x16* ebxpw, x16* ecxpw, x16 edxw, t_gfxdata** argppg0, x16 argw1)
{
  x8 byterg5;
  x16* wptrrg7;
  x16 vw_00;
  bool boolrg1;
  s_2w* sptrrg11;
  s_bbw* sbbwptrrg2; // another gdat-entry-ptr
  s_bbw* sbbwptrrg12; // based on unkptrrg2, also steps ahead by 4

// SPX DEBUG
  return false;
  wptrrg7 = ebxpw;
  //sptrrg11 = UPCAST(s_2w, SKW_QUERY_GDAT_ENTRY_DATA_PTR(con(0xf), con(0x8), con(0xfb), eaxb));
  sptrrg11 = UPCAST(s_2w, SKW_QUERY_GDAT_ENTRY_DATA_PTR(GDAT_CATEGORY_CREATURES, con(dtRaw8), con(GDAT_CREATURE_ANIM_ATTRIBUTION), eaxb));
  for (; (sptrrg11->w0[con(0x0)] != con(0xffffffff)) && (edxw != sptrrg11->w0[con(0x0)]); sptrrg11++)
  {
  }
  *wptrrg7 = vw_00 = sptrrg11->w0[con(0x1)];
  byterg5 = eaxb;
  //sbbwptrrg2 = UPCAST(s_bbw, SKW_QUERY_GDAT_ENTRY_DATA_PTR(con(0xf), con(0x7), con(0xfc), eaxb));
  sbbwptrrg2 = UPCAST(s_bbw, SKW_QUERY_GDAT_ENTRY_DATA_PTR(GDAT_CATEGORY_CREATURES, con(dtRaw7), con(GDAT_CREATURE_ANIM_INFO_SEQUENCE), eaxb));
  if ((SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(byterg5)->u0.b.b0 & con(0x1)) == con(0x0))
  {
    *ecxpw = con(0xffffffff);
    boolrg1 = R_4FCC(byterg5, ecxpw, argppg0, vw_00);
  }
  else
  {
    x16 wordrg4 = con(0x0);
    x8 byterg2;
    sbbwptrrg12 = &sbbwptrrg2[vw_00];
    do
    {
      byterg2 = sbbwptrrg12->b1 & con(0xf0);
      wordrg4++;
      sbbwptrrg12++;
    } while (byterg2 != con(0x0));
    if (argw1 == con(0x0))
      wordrg4 |= con(0x9000);
    else
      wordrg4 |= ((argw1 & con(0x3f)) << con(0x6)) | con(0x8000);
    *ecxpw = wordrg4;
    boolrg1 = true;
  }
  return boolrg1;
}

x8 SKW_48ae_011a(x16 eaxw)
{
  x8 byterg4 = SKW_QUERY_CLS1_FROM_RECORD(eaxw);
  x8 byterg6 = SKW_QUERY_CLS2_FROM_RECORD(eaxw);
  if (SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(byterg4, con(0x1), con(0x8), byterg6))
  {
    if (!SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(byterg4, con(0x1), con(0xc), byterg6))
      return con(0x3);
    if (SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(byterg4, con(0x1), con(0xa), byterg6))
      return con(0x1);
    if (SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(byterg4, con(0x1), con(0x9), byterg6))
      return con(0x0);
    return con(0x2);
  }
  return con(0xffffffff);
}

x8 SKW_48ae_01af(x16 eaxw, x16 edxw)
{
  if ((eaxw & con(0x400)) != con(0x0) && (eaxw & con(0x200)) == con(0x0))
  {
    eaxw &= con(0xf);
    if (eaxw != con(0x0))
      return table1d2660[4 * eaxw + edxw - 4]; // EVIL offset shifted, was 0x1d265c
    return con(0x0);
  }
  return con(0xf);
}

// can return 0, 1 or a mask
x16 SKW_IS_ITEM_FIT_FOR_EQUIP(x16 eaxw, bool ebxbool, x16 edxw)
{
  x16 wordrg1;
  x16 wordrg45;

  wordrg45 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(eaxw, con(0x4));
  if (!ebxbool)
  {
    if (edxw >= con(0xd) || edxw <= con(0x1))
    {
      if (edxw < con(0x1e) || edxw >= con(0x26))
        return con(0x1);
      if ((wordrg45 & con(0x8000)) != con(0x0))
        return con(0x0);
      if (SKW_IS_ITEM_FIT_FOR_EQUIP(ddata.v1d67c2.w0[ddata.v1e0b74][ddata.v1e0b76], false, con(0xc)) != con(0x0))
        return wordrg45 & con(0x40);
      return con(0x1);
    }
    return table1d2670[edxw] & wordrg45;
  }
  if (edxw >= con(0xd))
    return con(0x0);
  if (edxw >= con(0x0))
    wordrg1 = table1d2670[edxw];
  else
    wordrg1 = con(0x400);
  return wordrg1 & wordrg45;
}

x16 SKW_48ae_0767(x16 eaxw, t_gfxdata* ebxpg, x16* ecxpw, x16 edxw)
{
  x16 wordrg1;
  x16 wordrg2;
  x16 wordrg4;
  x16 wordrg5;
  t_gfxdata* pg_00;
  x16 vw_04;
  x16 vw_08;

  wordrg5 = eaxw;
  vw_08 = edxw;
  pg_00 = ebxpg;
  wordrg2 = con(0x0);
  wordrg1 = ddata.v1e03fe - con(0x1);
  vw_04 = con(0x0);
  *ecxpw = con(0x0);
  while (wordrg2 < vw_08 && wordrg5 > con(0x0) && wordrg1 >= con(0x0) && (wordrg4 = ddata.v1e03ac[wordrg1], wordrg4 > con(0x0)))
  {
    if (wordrg5 < wordrg4)
      wordrg1--;
    else
    {
      pg_00[wordrg2++] = CUTX16(wordrg1);
      wordrg5 -= wordrg4;
      vw_04 += wordrg4;
      *ecxpw += con(0x1);
    }
  }
  return vw_04;
}

void SKW_SHOOT_ITEM(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x8 argb0, x8 argb1, x8 argb2, x8 argb3)
{
  x16 wordrg4;
  x16 wordrg5;
  x16 wordrg6;
  c_record* recoptrrg5;
  c_tim vtim_00;
  c_record* prec_0c;
  x16 vw_10;
  x16 vw_14;
  x16 vw_18;
  x16 vw_1c;
  x16 vw_20;
  x16 vw_24;
  x16 vw_28;

  wordrg6 = eaxw;
  vw_28 = edxw;
  vw_20 = ebxw;
  vw_24 = SKW_ALLOC_NEW_RECORD(con(0xe));
  vw_18 = ecxw << con(0xe);
  if (vw_24 != con(0xffffffff))
  {
    vw_10 = vw_20;
    vw_14 = vw_28;
    SKW_QUEUE_NOISE_GEN2(SKW_QUERY_CLS1_FROM_RECORD(wordrg6), con(0x86), con(0xfe), SKW_QUERY_CLS2_FROM_RECORD(wordrg6), vw_28, vw_20, con(0x1), con(0x6e), con(0x80));
    wordrg4 = vw_18 | (vw_24 & con(0x3fff));
    vw_1c = wordrg4;
    recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(wordrg4);
    prec_0c = recoptrrg5;
    recoptrrg5->u2.w = wordrg6;
    recoptrrg5->u4.b.b0 = CUTX8(DM2_MIN(unsignedword(argb1), con(0xff)));
    recoptrrg5->u4.b.b1 = argb2;
    SKW_APPEND_RECORD_TO(wordrg4, vw_14, vw_10, NULL);
    if (recoptrrg5->u2.w == con(0xff80))
      if (ddata.v1d3248 == ddata.v1e0266)
        ddata.v1e0390.b0 |= con(0x1);
    vtim_00.un.l_00 = (ddata.longmallocx + 1) | (signedlong(ddata.v1d3248) << con(0x18));
    vtim_00.b_04 = ((ddata.v1e01a8 != con(0x0)) ? 1 : 0) + con(0x1d);
    vtim_00.b_05 = con(0x0);
    vtim_00.u.w_06 = vw_1c;
    vtim_00.v.b.b_08 &= con(0xffffffe0);
    wordrg5 = CUTX16(vtim_00.v.l_08 | (unsignedlong(vw_28) & con(0x1f)));
    vtim_00.v.w_08 = (wordrg5 & con(0xfffffc1f)) | (vw_20 & con(0x1f)) << con(0x5);
    vtim_00.v.b.b_09 &= con(0xf);
    vtim_00.v.w_08 |= (unsignedword(argb3) & con(0xf)) << con(0xc);
    vtim_00.v.b.b_09 &= con(0xfffffff3);
    vtim_00.v.w_08 |= (unsignedword(argb0) & con(0x3)) << con(0xa);
    prec_0c->u6.w = SKW_QUEUE_TIMER(&vtim_00);
  }
  else
  {
    if (urshift(wordrg6 & con(0x3c00), con(0xa)) != con(0xf))
    {
      wordrg6 &= con(0xffff3fff);
      wordrg6 |= vw_18;
      SKW_MOVE_RECORD_TO(wordrg6, con(0x0), vw_28, con(0xffffffff), vw_20);
    }
  }
}

bool SKW_ATTACK_DOOR(x16 eaxw, x16 ebxw, bool ecxbool, x16 edxw, x16 argw0)
{
  c_record* recoptrrg4;
  x8** bpptrrg11;
  x8* bptrrg12;
  x16 vw_00;
  x16 vw_04;
  c_tim vtim_08;
  bool flag;

  vw_04 = eaxw;
  vw_00 = edxw;
  recoptrrg4 = SKW_GET_ADDRESS_OF_TILE_RECORD(vw_04, vw_00);
  if (!ecxbool)
    flag = (recoptrrg4->u2.b.b1 & con(0x1)) == con(0x0);
  else
    flag = (recoptrrg4->u2.b.b0 & con(0xffffff80)) == con(0x0);
  if (!flag && ebxw >= SKW_GET_DOOR_STAT_0X10(SKW_IS_REBIRTH_ALTAR(recoptrrg4)))
  {
    bpptrrg11 = &ddata.savegamepp1[vw_04];
    bptrrg12 = *bpptrrg11 + unsignedlong(vw_00);
    if ((*bptrrg12 & con(0x7)) == con(0x4))
    {
      if (argw0 == con(0x0))
        *bptrrg12 = (*bptrrg12 & con(0xfffffff8)) | con(0x5);
      else
      {
        vtim_08.un.l_00 = (unsignedlong(argw0) + ddata.longmallocx) | (signedlong(ddata.v1d3248) << con(0x18));
        vtim_08.b_04 = con(0x2);
        vtim_08.b_05 = con(0x0);
        vtim_08.u.b.b_06 = CUTX8(vw_04);
        vtim_08.u.b.b_07 = CUTX8(vw_00);
        SKW_QUEUE_TIMER(&vtim_08);
      }
      return true;
    }
  }
  return false;
}

void SKW_DELETE_MISSILE_RECORD(x16 eaxw, x16 ebxw, x16 ecxw, x16* edxpw)
{
  x16 wordrg1;
  x16 wordrg3;
  x16 wordrg4;
  c_record* recoptrrg7;
  x16 vw_00;

  vw_00 = ecxw;
  recoptrrg7 = SKW_GET_ADDRESS_OF_RECORD(eaxw);
  wordrg3 = recoptrrg7->u2.w;
  if (urshift(recoptrrg7->u2.w & con(0x3c00), con(0xa)) != con(0xf))
  {
    if (edxpw == NULL)
    {
      wordrg1 = SKW_GET_CREATURE_AT(ebxw, vw_00);
      if (wordrg1 == con(0xffffffff) || (SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg1) & con(0x1)) == con(0x0))
      {
        wordrg4 = (eaxw & con(0xc000)) | (wordrg3 & con(0x3fff));
        SKW_MOVE_RECORD_TO(wordrg4, con(0x0), ebxw, con(0xfffffffe), vw_00);
      }
      else
      {
        wordrg4 = urshift(eaxw, con(0xe));
        if ((SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(wordrg1)->u1e.w & con(0xf)) == con(0x1))
          wordrg4 = ulrshift(ddata.savegametimarrp[recoptrrg7->u6.w].v.w_08, con(0x4), con(0xe));
        SKW_MOVE_RECORD_TO((wordrg3 & con(0x3fff)) | (wordrg4 << con(0xe)), con(0xffffffff), ebxw, con(0xfffffffe), vw_00);
      }
    }
    else
      SKW_APPEND_RECORD_TO(wordrg3, con(0xffffffff), con(0x0), edxpw);
    SKW_QUEUE_NOISE_GEN2(SKW_QUERY_CLS1_FROM_RECORD(eaxw), con(0x85), con(0xfe), SKW_QUERY_CLS2_FROM_RECORD(eaxw), ebxw, vw_00, con(0x1), con(0x3a), con(0x80));
  }
  SKW_DEALLOC_RECORD(eaxw);
}

void SKW_075f_1bc2(s_4bytearray* eaxp4ba, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1)
{
  x16 wordrg1;
  x8 byterg1h;

  wordrg1 = SKW_CALC_VECTOR_DIR(edxw, ecxw, argw0, ebxw);
  if (argw1 != con(0xff))
  {
    wordrg1 *= 2;
    if ((wordrg1 & con(0x2)) == con(0x0))
      argw1++;
    SKW_COPY_MEMORY(DOWNCAST(s_4bytearray, &table1d26d0[wordrg1 + ((argw1 / 2) & con(0x1))]), con(0x4), DOWNCAST(s_4bytearray, eaxp4ba));
  }
  else
  {
    SKW_COPY_MEMORY(DOWNCAST(s_4bytearray, &table1d26f0[wordrg1 / 4]), con(0x4), DOWNCAST(s_4bytearray, eaxp4ba));
    if (SKW_RAND01() != con(0x0))
    {
      byterg1h = eaxp4ba->barr_00[con(0x0)];
      eaxp4ba->barr_00[con(0x0)] = eaxp4ba->barr_00[con(0x1)];
      eaxp4ba->barr_00[con(0x1)] = byterg1h;
    }
    if (SKW_RAND01() != con(0x0))
    {
      byterg1h = eaxp4ba->barr_00[con(0x2)];
      eaxp4ba->barr_00[con(0x2)] = eaxp4ba->barr_00[con(0x3)];
      eaxp4ba->barr_00[con(0x3)] = byterg1h;
    }
  }
}

void SKW_098d_000f(x16 eaxw, x16 ebxw, x16* ecxpw, x16 edxw, x16* argpw0)
{
  *ecxpw = (ebxw % con(0x5)) + 4 * eaxw;
  *argpw0 = (ebxw / con(0x5)) + (edxw << con(0x2));
}

void SKW_LOAD_RECTS_AND_COMPRESS(x8 eaxb, x8 ebxb, x8 edxb)
{
  x16 wordrg5 = CUTX16(SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(eaxb, con(0x4), ebxb, edxb));
  x8* bptrrg6 = UPCAST(x8, DM2_ALLOC_HIBIGPOOL_MEMORY(wordrg5));
  SKW_LOAD_GDAT_ENTRY_DATA_TO(eaxb, con(0x4), ebxb, edxb, bptrrg6);
  SKW_COMPRESS_RECTS(bptrrg6, &ddata.rectanglelist, DM2_ALLOC_FREEPOOL_MEMORY);
  DM2_DEALLOC_HIBIGPOOL(wordrg5);
}

t_gfxdata* SKW_QUERY_PICST_IMAGE(c_hex18* eaxph18)
{
  t_gfxdata* gptrrg1 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(eaxph18->b_08, eaxph18->b_0b, eaxph18->b_09);
  eaxph18->pg_00 = gptrrg1;
  s_gfxdatatail* ptrbeforerg1 = getprevious(gptrrg1);
  eaxph18->w_0e = con(0x0);
  eaxph18->w_10 = con(0x0);
  eaxph18->w_12 = ptrbeforerg1->width;
  eaxph18->w_14 = ptrbeforerg1->height;
  eaxph18->w_16 = unsignedword(ptrbeforerg1->b0);
  eaxph18->w_04 = con(0x4);
  return gptrrg1;
}

t_gfxdata* SKW_0b36_00c3(x16 eaxw, c_hex18* edxph18)
{
  t_gfxdata* gptrrg3 = SKW_3e74_5817(eaxw);
  s_gfxdatatail* ptrbeforerg3 = getprevious(gptrrg3);
  if (edxph18 != NULL)
  {
    edxph18->pg_00 = gptrrg3;
    edxph18->w_0e = con(0x0);
    edxph18->w_10 = con(0x0);
    edxph18->w_12 = ptrbeforerg3->width;
    edxph18->w_14 = ptrbeforerg3->height;
    edxph18->w_16 = unsignedword(ptrbeforerg3->b0);
    edxph18->w_0c = eaxw;
    edxph18->w_04 = con(0x8);
  }
  return gptrrg3;
}

void SKW_0b36_018f(c_hex18* eaxph18)
{
  if ((eaxph18->w_04 & con(0x4)) != con(0x0))
    SKW_3e74_58bf(eaxph18->b_08, eaxph18->b_0b, eaxph18->b_09);
  else if ((eaxph18->w_04 & con(0x8)) != con(0x0))
    SKW_3e74_583a(eaxph18->w_0c);
}

void SKW_0b36_01cd(c_hex18* eaxph18)
{
  if ((eaxph18->w_04 & con(0x4)) != con(0x0))
    SKW_3e74_5992(eaxph18->b_08, eaxph18->b_0b, eaxph18->b_09);
  else if ((eaxph18->w_04 & con(0x8)) != con(0x0))
    SKW_3e74_585a(eaxph18->w_0c, false);
}

t_gfxdata* R_B012(t_gfxdata* eaxpg, x8 ebxb, x8 ecxb, x8 edxb, x16* argpw0)
{
  t_gfxdata* gptrrg3;

  gptrrg3 = UPCAST(t_gfxdata, SKW_QUERY_GDAT_ENTRY_DATA_PTR(edxb, con(0x7), ecxb, ebxb));
  x16 wordrg4 = *argpw0;
  if (wordrg4 > con(0x0))
  {
    while (--wordrg4 >= con(0x0))
      eaxpg[wordrg4] = gptrrg3[eaxpg[wordrg4]];
  }
  else
  {
    *argpw0 = con(0x100);
    SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, gptrrg3), PAL256, DOWNCAST(t_gfxdata, eaxpg)); // left side should be a 256 - colortable too
  }
  return eaxpg;
}

void SKW_0b36_037e(t_gfxdata* eaxpg, x8 ebxb, x8 ecxb, x8 edxb, x8 argb0, x16 argw1, x16 argw2, x16* argpw3)
{
  x16 vw_08 = con(0xffffffff);
  if (*argpw3 <= con(0x0) && argb0 != con(0x0) && SKW_3e74_55f9(signedlong(DM2_MAX(con(0x0), con(0x40) - unsignedword(argb0))) | (((unsignedlong(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(edxb, con(0x7), ecxb, ebxb))) << con(0x6)) | con(0x60000000)), &vw_08))
  {
    *argpw3 = con(0x100);
    SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, SKW_3e74_5817(vw_08)), PAL256, DOWNCAST(t_gfxdata, eaxpg));
  }
  else
  {
    R_B073(R_B012(eaxpg, ebxb, ecxb, edxb, argpw3), argw1, argw2, argb0, argpw3);
    if (vw_08 >= con(0x0))
    {
      SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, eaxpg), PAL256, DOWNCAST(s_xmalloc, SKW_ALLOC_CPXHEAP_MEM(vw_08, PAL256)));
      SKW_3e74_585a(vw_08, false);
    }
  }
}

c_image* SKW_QUERY_GDAT_SUMMARY_IMAGE(c_image* eaxpimg, x8 ebxb, x8 ecxb, x8 edxb)
{
  t_gfxdata* gptrrg1;
  x16 wordrg3;

  DM2_ZERO_MEMORY(eaxpimg, con(0x13a));
  eaxpimg->s_00.w_0c = con(0xffffffff);
  eaxpimg->s_00.b_08 = edxb;
  eaxpimg->s_00.b_09 = ebxb;
  eaxpimg->s_00.b_0a = con(0x1);
  eaxpimg->s_00.b_0b = ecxb;
  eaxpimg->w_36 = con(0x40);
  eaxpimg->w_34 = eaxpimg->w_36;
  eaxpimg->w_18 = con(0xffffffff);
  eaxpimg->w_1a = con(0xffffffff);
  eaxpimg->pg_2c = ddata.screenptr;
  eaxpimg->w_30 = con(0xffffffff);
  if (edxb != con(0xffffffff))
  {
    eaxpimg->s_00.w_06 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(edxb, con(0x1), ecxb, ebxb);
    wordrg3 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(edxb, con(0xc), con(0xfe), ebxb);
    if (wordrg3 != con(0x0))
    {
      eaxpimg->w_1c += signedword(CUTX8(urshift(wordrg3, con(0x8))));
      eaxpimg->w_1e += signedword(CUTX8(wordrg3));
    }
    wordrg3 = SKW_QUERY_GDAT_PICT_OFFSET(edxb, ecxb, ebxb);
    if (wordrg3 != con(0x0))
    {
      eaxpimg->w_1c += signedword(CUTX8(urshift(wordrg3, con(0x8))));
      eaxpimg->w_1e += signedword(CUTX8(wordrg3));
    }
    gptrrg1 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(edxb, ecxb, ebxb);
    if (gptrrg1 != NULL)
    {
      eaxpimg->w_38 = con(0x10);
      SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, gptrrg1), PAL16, DOWNCAST(t_gfxdata, eaxpimg->carr_3a));
    }
    else
      eaxpimg->w_38 = con(0xffffffff);
  }
  return eaxpimg;
}

ui32 SKW_0b36_068f(c_image* eaxpimg)
{
  x16 w1 = eaxpimg->w_36 & con(0x1f);
  x16 w2 = eaxpimg->w_34 & con(0x7f);
  x16 w3 = eaxpimg->s_00.w_06 & con(0x1fff);
  return unsignedlong(w1) | (unsignedlong(w2) << con(0x5)) | (unsignedlong(w3) << con(0xc));
}

void SKW_0b36_0c52(c_c5rects* eaxps, bool ebxbool, x16 edxw)
{
  if (edxw != con(0xffffffff))
    SKW_QUERY_EXPANDED_RECT(edxw, &eaxps->s_00.rc_02);
  eaxps->s_00.w_00 = SKW_3e74_5888();
  SKW_ALLOC_NEW_PICT(eaxps->s_00.w_00, eaxps->s_00.rc_02.h, con(0x8), eaxps->s_00.rc_02.w);
  eaxps->s_00.w_0a = con(0x0);
  if (ebxbool)
    SKW_0b36_0d67(eaxps, &eaxps->s_00.rc_02);
}

void SKW_0b36_0cbe(c_c5rects* eaxps, bool edxbool)
{
  if (eaxps->s_00.w_0a != con(0x0))
  {
    c_rect* rectptrrg6 = eaxps->rcarr_0c; // array of rects there
    SKW_FIRE_HIDE_MOUSE_CURSOR();
    do
    {
//                              srcbmp                                            srcofs                       destbmp                                                                                                             srcbits    destbits
      SKW_FIRE_BLIT_PICTURE(SKW_3e74_5817(eaxps->s_00.w_00), rectptrrg6, rectptrrg6->x - eaxps->s_00.rc_02.x, ddata.screenptr, rectptrrg6->y - eaxps->s_00.rc_02.y, eaxps->s_00.rc_02.w, ddata.mouse_unk0c, con(0xffff), con(0x0), E_BITRES8, E_BITRES8);
      rectptrrg6++;
    } while (--eaxps->s_00.w_0a != con(0x0));
    SKW_FIRE_SHOW_MOUSE_CURSOR();
  }
  if (edxbool)
  {
    SKW_3e74_58a3(eaxps->s_00.w_00);
    eaxps->s_00.w_00 = con(0xffffffff);
  }
}

void SKW_0b36_105b(c_c5rects* eaxps, x16 ebxw, c_rect* edxr)
{
  c_rect rc_00;

  if (edxr != NULL)
  {
    SKW_44c8_1aca(SKW_3e74_5817(eaxps->s_00.w_00), ebxw, eaxps->s_00.rc_02.w, SKW_OFFSET_RECT(eaxps, &rc_00, edxr));
    SKW_0b36_0d67(eaxps, edxr);
  }
}

// Note: here the orange background comes from (argw1=0xa)
// srcbmp = eax
// palette = argcarr3
void SKW_DRAW_ICON_PICT_BUFF(t_gfxdata* srcbmp, c_rect* ebxr, x16 ecxw, c_c5rects* edxps, x16 argw0, x16 argw1, x16 argw2, t_palette* palette)
{
  c_rect rc_00; // TODO stacksize 0x18. this could be an array of 3!

  s_gfxdatatail* ptrbeforerg6 = getprevious(srcbmp);
  if (ebxr != NULL)
  {
//                        srcbmp                                      srcofs   destbmp                                                                                           srcbits               destbits   palette
    SKW_FIRE_BLIT_PICTURE(srcbmp, SKW_OFFSET_RECT(edxps, &rc_00, ebxr), ecxw, SKW_3e74_5817(edxps->s_00.w_00), argw0, ptrbeforerg6->width, edxps->s_00.rc_02.w, argw1, argw2, e_bitres(ptrbeforerg6->b0), E_BITRES8, palette);
    SKW_0b36_0d67(edxps, ebxr);
  }
}

void SKW_DRAW_ICON_PICT_ENTRY(x8 eaxb, x8 ebxb, c_c5rects* ecxps, x8 edxb, x16 argw0, x16 argw1)
{
  c_rect rc_04;

  c_c5rects* ps_00 = ecxps;
  x16 vw_14 = con(0x0);
  x16 vw_18 = con(0x0);
  t_gfxdata* gptrrg5 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(eaxb, ebxb, edxb);
  c_rect* pr_10 = SKW_QUERY_BLIT_RECT(gptrrg5, argw0, &vw_18, &rc_04, &vw_14, con(0xffffffff));
//                         srcbmp                                                           palette
  SKW_DRAW_ICON_PICT_BUFF(gptrrg5, pr_10, vw_18, ps_00, vw_14, argw1, con(0x0), SKW_QUERY_GDAT_IMAGE_LOCALPAL(eaxb, ebxb, edxb));
}

void SKW_DRAW_STATIC_PIC(x8 eaxb, x8 ebxb, x16 ecxw, x8 edxb, x16 argw0)
{
  c_image vs_00;

  SKW_QUERY_GDAT_SUMMARY_IMAGE(&vs_00, edxb, ebxb, eaxb);
  vs_00.w_30 = argw0;
  vs_00.w_18 = ecxw;
  vs_00.pg_2c = ddata.bitmapptr;
  vs_00.w_1c = con(0x0);
  vs_00.w_1e = con(0x0);
  SKW_DRAW_PICST(SKW_QUERY_PICST_IT(&vs_00));
}

// srcbmp = eax
// palette = ecx
void R_C470(t_gfxdata* srcbmp, x16 ebxw, t_palette* palette, x16 edxw)
{
  c_rect rc_00;

  s_gfxdatatail* ptrbeforerg5 = getprevious(srcbmp);	// SPX: is s_gfxdatatail false and need one word more at the end of structure ?
  x16 vw_10 = con(0x0);
  x16 vw_08 = con(0x0);
  if (SKW_QUERY_BLIT_RECT(srcbmp, edxw, &vw_10, &rc_00, &vw_08, con(0xffffffff)) != NULL)
//                        srcbmp         srcofs   destbmp                                                                          srcbits
//    SKW_FIRE_BLIT_PICTURE(srcbmp, &rc_00, vw_10, ddata.screenptr, vw_08, ptrbeforerg5->w2, ddata.mouse_unk0c, ebxw, con(0x0), e_bitres(ptrbeforerg5->b0), E_BITRES8, palette);
    SKW_FIRE_BLIT_PICTURE(srcbmp, &rc_00, vw_10, ddata.screenptr, vw_08, *((ui16*)(srcbmp-6)), ddata.mouse_unk0c, ebxw, con(0x0), e_bitres(*((ui16*)(srcbmp-8))), E_BITRES8, palette);
}

void SKW_RAISE_SYSERR(x16 eaxw)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("SKW_RAISE_SYSERR");
  t_text t_00[0x80];

  ddata.v1e0218 = eaxw;
  if (ddata.v1d70d3)
    SKW_FIRE_SELECT_PALETTE_SET(1);
  SKW_FORMAT_SKSTR(table1d27e8[ddata.v1d674c], t_00);
SPX_DEBUG_FUNCTION_INFO(t_00);
  if (!ddata.v1d70dc || !ddata.v1e01d4)
    SKW_COPY_TO_DRIVER(DOWNCAST(t_text, t_00), con(0x100));
  else
    SKW_PRINT_SYSERR_TEXT(con(0x0), con(0xf), con(0x0), con(0x10), t_00);
  if (!ddata.v1d70cf)
    SKW_SLEEP_SEVERAL_TIME(con(0x12c));
  else
    SKW_476d_050e();
  SKW_SK_PREPARE_EXIT();
SPX_DEBUG_POP;
}

void SKW_CHANGE_CURRENT_MAP_TO(x16 eaxw)
{
  if (eaxw != ddata.v1d3248 && eaxw >= con(0x0))
  {
    ddata.v1d3248 = eaxw;
    ddata.savegamepp1 = ddata.v1e03cc[eaxw];
    ddata.v1e03c0 = &ddata.v1e03c8[eaxw];
    ddata.savegamewa = ulrshift(ddata.v1e03c0->w8, con(0x5), con(0xb)) + con(0x1);
    ddata.savegamewb = urshift(ddata.v1e03c0->w8, con(0xb)) + con(0x1);
    ddata.v1e03f4 = &ddata.v1e03d8[ddata.v1e03e4[eaxw]];
    if (ddata.v1e0234 && eaxw == ddata.v1e027c)
    {
      ddata.v1e0280 = ddata.v1e0260;
      ddata.v1e027e = ddata.v1e0262;
      ddata.v1e0282 = eaxw;
      ddata.v1e0264 = ddata.v1e0276;
    }
    else
    {
      ddata.v1e0280 = ddata.v1e0270;
      ddata.v1e027e = ddata.v1e0272;
      ddata.v1e0282 = ddata.v1e0266;
      ddata.v1e0264 = ddata.v1e0258;
    }
  }
}

// ebxw = mapnumber
void SKW_LOAD_LOCALLEVEL_GRAPHICS_TABLE(x16 eaxw, x16 ebxw, x16 edxw)
{
  ddata.v1e0270 = eaxw;
  ddata.v1e0272 = edxw;
  ddata.v1e0266 = ebxw;
  SKW_CHANGE_CURRENT_MAP_TO(ebxw);
  x8* bptrrg3 = ddata.savegamepp1[ddata.savegamewa - 1] + ddata.savegamewb;
  ddata.v1e03c4 = bptrrg3;
  x16 wordrg6 = ddata.v1e03c0->wa & con(0xf);
  bptrrg3 += ulrshift(ddata.v1e03c0->wc, con(0x8), con(0xc));
  SKW_COPY_MEMORY(bptrrg3, signedlong(wordrg6), ddata.v1e02cc);
  x32 longrg5 = signedlong(ulrshift(ddata.v1e03c0->wa, con(0x4), con(0xc)));
  bptrrg3 += wordrg6;
  SKW_COPY_MEMORY(bptrrg3, longrg5, ddata.v1e02dc);
  SKW_COPY_MEMORY(bptrrg3 + longrg5, lextended(ddata.v1e03c0->wc & con(0xf)), ddata.v1e0414);
}

void SKW_CALC_VECTOR_W_DIR(x16 eaxw, x16 ebxw, x16* ecxpw, x16 edxw, x16* argpw0)
{
  *ecxpw += table1d27fc[eaxw] * edxw;
  edxw *= table1d2804[eaxw];
  *argpw0 += edxw;
  eaxw++;
  eaxw &= con(0x3);
  *ecxpw += table1d27fc[eaxw] * ebxw;
  *argpw0 += table1d2804[eaxw] * ebxw;
}

bool SKW_IS_TILE_PASSAGE(x16 eaxw, x16 edxw)
{
  x8* bptrrg2 = ddata.savegamepp1[eaxw];
  x8 byterg2 = (bptrrg2[edxw] >> con(0x5)) & con(0x7);
  if (byterg2 == con(0x5) && (SKW_GET_ADDRESS_OF_TILE_RECORD(eaxw, edxw)->u4.b.b0 & con(0x1)) != con(0x0))
    return false;
  if (byterg2 != con(0x0) && byterg2 != con(0x7))
    return true;
  return false;
}

x16 SKW_0cee_06dc(x16 eaxw, x16 edxw)
{
  x16 wordrg5 = ((SKW_GET_TILE_VALUE(eaxw, edxw) & con(0x8)) == con(0x0)) ? 1 : 0;
  if (wordrg5 == con(0x0))
  {
    eaxw += table1d27fc[0x0];
    edxw += table1d2804[0x0];
  }
  else
  {
    eaxw += table1d27fc[0x1];
    edxw += table1d2804[0x1];
  }
  x8 byterg1 = (SKW_GET_TILE_VALUE(eaxw, edxw) & con(0xff)) >> con(0x5);
  if (byterg1 != con(0x0) && byterg1 != con(0x3))
    return wordrg5;
  return wordrg5 + con(0x2);
}

bool SKW_GET_TELEPORTER_DETAIL(c_5bytes* eaxps, x16 ebxw, x16 edxw)
{
  x16* pw_00;
  x16* pw_04;

  bool boolrg5 = false;
  x8 vb_08 = SKW_0cee_0897(&pw_04, ebxw, edxw);
  if (vb_08 != con(0x0))
  {
    x16 wordrg6 = ddata.v1d3248;
    SKW_CHANGE_CURRENT_MAP_TO(urshift(pw_04[con(0x2)], con(0x8)));
    x8 byterg1 = SKW_0cee_0897(&pw_00, ulrshift(pw_04[con(0x1)], con(0x6), con(0xb)), pw_04[con(0x1)] & con(0x1f));
    if (byterg1 != con(0x0))
    {
      boolrg5 = true;
      eaxps->b_00 = vb_08 - con(0x1);
      eaxps->b_01 = byterg1 - con(0x1);
      eaxps->b_02 = CUTX8(pw_04[con(0x1)] & con(0x1f));
      eaxps->b_03 = CUTX8(ulrshift(pw_04[con(0x1)], con(0x6), con(0xb)));
      eaxps->b_04 = CUTX8(urshift(pw_04[con(0x2)], con(0x8)));
    }
    SKW_CHANGE_CURRENT_MAP_TO(wordrg6);
  }
  return boolrg5;
}

x16 SKW_GET_OBJECT_INDEX_FROM_TILE(x16 eaxw, x16 edxw)
{
  x8* bptrrg1 = ddata.savegamepp1[eaxw];
  if (eaxw >= con(0x0) && eaxw < ddata.savegamewa)
    if (edxw >= con(0x0) && edxw < ddata.savegamewb)
      if ((bptrrg1[edxw] & con(0x10)) != con(0x0))
      {
        x16 wordrg3 = ddata.v1e03f4[eaxw];
        while (--edxw >= con(0x0))
          if ((*bptrrg1++ & con(0x10)) != con(0x0))
            wordrg3++;
        return wordrg3;
      }
  return con(0xffffffff);
}

x16 SKW_GET_TILE_RECORD_LINK(x16 eaxw, x16 edxw)
{
  x16 wordrg1 = SKW_GET_OBJECT_INDEX_FROM_TILE(eaxw, edxw);
  if (wordrg1 == con(0xffffffff))
    return con(0xfffe);
  return ddata.v1e038c[wordrg1];
}

x16 SKW_GET_WALL_TILE_ANYITEM_RECORD(x16 eaxw, x16 edxw)
{
  x16 wordrg1 = SKW_GET_TILE_RECORD_LINK(eaxw, edxw);
  while (urshift(wordrg1 & con(0x3c00), con(0xa)) <= con(0x3))
    wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg1);
  return wordrg1;
}

x16 SKW_ALLOC_NEW_DBITEM(x16 eaxw)
{
  x16 wordrg1 = SKW_ALLOC_NEW_RECORD(SKW_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(eaxw) | (eaxw & con(0x8000)));
  if (wordrg1 != con(0xffffffff))
    SKW_SET_ITEMTYPE(wordrg1, SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(eaxw));
  return wordrg1;
}

x16 SKW_ALLOC_NEW_DBITEM_DEEPLY(x16 eaxw)
{
  x8 vb_00 = SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(eaxw);
  x16 wordrg5 = SKW_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(eaxw);
  x16 wordrg3 = SKW_RECYCLE_A_RECORD_FROM_THE_WORLD(wordrg5, vb_00);
  if (wordrg3 == con(0xffffffff))
  {
    wordrg3 = SKW_ALLOC_NEW_RECORD(wordrg5);
    if (wordrg3 == con(0xffffffff))
      return wordrg3;
  }
  c_record* recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(wordrg3);
  DM2_ZERO_MEMORY(DOWNCAST(c_record, recoptrrg2), unsignedlong(table1d280c[wordrg5]));
  recoptrrg2->w0 = con(0xfffffffe);
  SKW_SET_ITEMTYPE(wordrg3, vb_00);
  return wordrg3;
}

x16 SKW_0cee_17e7(x16 eaxw, x16 ebxw, x16 edxw)
{
  x32 longrg3 = (unsignedlong(eaxw) * con(0x7ab9)) / 2;
  x32 longrg4 = (longrg3 + 11 * unsignedlong(edxw) + unsignedlong(ddata.savegamep4->w0[con(0x0)])) >> con(0x2);
  return CUTX16(longrg4 % ebxw);
}

x8 SKW_0cee_1815(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0)
{
  if (eaxw != con(0x0))
  {
    x16 wordrg1 = (ddata.v1d3248 << con(0x6)) + con(0xbb8) + ddata.savegamewa + ddata.savegamewb;
    wordrg1 = SKW_0cee_17e7(32 * ebxw + con(0x7d0) + ecxw, argw0, wordrg1);
    if (wordrg1 < edxw)
      return ddata.v1e02cc[wordrg1];
  }
  return con(0xffffffff);
}

x16 SKW_QUERY_ORNATE_ANIM_FRAME(x8 eaxb, x32 ebxl, x8 ecxb, x8 edxb)
{
  x16 wordrg4;
  x16 wordrg5;
  t_text* tptrrg21;
  t_text* tptrrg2;
  t_text t_08[0x80];
  bool vbool_88;
  x16 vw_8c;

  wordrg5 = con(0x1);
  vw_8c = con(0x0);
  vbool_88 = false;
  wordrg4 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, con(0xb), con(0xd), edxb);
  if (wordrg4 != con(0x0))
  {
    tptrrg21 = NULL;
    vbool_88 = true;
    if ((wordrg4 & con(0x8000)) != con(0x0))
    {
      vw_8c = con(0x1);
      wordrg4 &= con(0x7fff);
    }
  }
  else
  {
    wordrg4 = unsignedlong(edxb);
    tptrrg2 = SKW_QUERY_GDAT_TEXT(eaxb, con(0xd), t_08, edxb);
    tptrrg21 = tptrrg2; // TODO: added this one
    if (*tptrrg2 == NULLBYTE || (wordrg4 = SKW_SK_STRLEN(tptrrg2), wordrg4 == con(0x0)))
      wordrg4 = con(0x1);
    else
      vbool_88 = true;
  }
  if (vbool_88)
  {
    wordrg5 = CUTX16((ebxl + unsignedlong(ecxb)) % wordrg4) + vw_8c;
    if (tptrrg21 != NULL)
    {
      tptrrg21 += wordrg5;
      wordrg5 = unsignedword(*tptrrg21);
      if (*tptrrg21 < con(0x30) || wordrg5 > con(0x39))
      {
        if (wordrg5 >= con(0x41) && wordrg5 <= con(0x5a))
          wordrg5 -= con(0x4b);
      }
      else
        wordrg5 -= con(0x30);
    }
  }
  return wordrg5;
}

x16 SKW_GET_DISTINCTIVE_ITEMTYPE(x16 eaxw)
{
  if (eaxw != con(0xffffffff))
  {
    x8 byterg1 = SKW_QUERY_CLS2_FROM_RECORD(eaxw);
    x16 wordrg4 = table1d3278[urshift(eaxw & con(0x3c00), con(0xa))];
    if ((wordrg4 & con(0x8000)) != con(0x0))
    {
      wordrg4 &= con(0x7fff);
      byterg1 = con(0x0);
    }
    return unsignedword(byterg1) + wordrg4;
  }
  return con(0x1ff);
}

x16 SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(x16 eaxw, x8 edxb)
{
  if (eaxw == con(0xffffffff))
    return con(0x0);
  return DM2_QUERY_GDAT_ENTRY_DATA_INDEX(SKW_QUERY_CLS1_FROM_RECORD(eaxw), con(0xb), edxb, SKW_QUERY_CLS2_FROM_RECORD(eaxw));
}

// TODO returns a word, but a byte would be enough
x16 SKW_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(x16 eaxw)
{
  x16 wordrg1 = eaxw & con(0x1ff);
  ui16 wordrg4 = wordrg1 >> con(0x7);
  if (wordrg4 > con(0x3))
    return con(0xffff);
  switch (wordrg4)
  {
    case 0:
      return con(0x5);
    case 1:
      return con(0x6);
    case 2:
      return con(0xa);
    case 3:
      if (wordrg1 > con(0x1fc))
        return con(0xffff);
      if (wordrg1 != con(0x1fc))
      {
        if (wordrg1 < con(0x1e0))
        {
          if (wordrg1 < con(0x1b0))
            return con(0x8);
          return con(0x4);
        }
        return con(0x9);
      }
      return con(0x7);
    default: throw(THROW_DMABORT);
  }
}

// ATTENTION: values before return are x16
x8 SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(x16 eaxw)
{
  x16 wordrg1 = eaxw & con(0x1ff);
  ui16 uwordrg2 = urshift(wordrg1, con(0x7));
  if (uwordrg2 > con(0x3))
    return CUTX8(wordrg1);
  switch (uwordrg2)
  {
    case 0:
      return CUTX8(wordrg1);
    case 1:
      wordrg1 -= con(0x80);
      return CUTX8(wordrg1);
    case 2:
      wordrg1 -= con(0x100);
      return CUTX8(wordrg1);
    case 3:
      if (wordrg1 < con(0x1fc))
      {
        if (wordrg1 < con(0x1e0))
        {
          if (wordrg1 < con(0x1b0))
            wordrg1 -= con(0x180);
          else
            wordrg1 -= con(0x1b0);
        }
        else
          wordrg1 -= con(0x1e0);
        return CUTX8(wordrg1);
      }
      return con(0x0);
    default: throw(THROW_DMABORT);
  }
}

x8 SKW_QUERY_CLS1_FROM_RECORD(x16 eaxw)
{
  for (x16 wordrg1 = eaxw; wordrg1 != con(0xffffffff); wordrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg1)->u2.w)
  {
    x16 wordrg4 = urshift(wordrg1 & con(0x3c00), con(0xa));
    if (wordrg4 != con(0xe))
      return table1d3298[wordrg4];
  }
  return con(0xffffffff);
}

bool SKW_IS_CONTAINER_MONEYBOX(x16 eaxw)
{
  if (urshift(eaxw & con(0x3c00), con(0xa)) == con(0x9) && (SKW_GET_ADDRESS_OF_RECORD(eaxw)->u4.b.b0 & con(0x6)) == con(0x0))
    if (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x14), con(0x5), con(0x40), SKW_QUERY_CLS2_FROM_RECORD(eaxw)) != con(0xffffffff))
      return true;
  return false;
}

bool SKW_IS_CONTAINER_CHEST(x16 eaxw)
{
  if (urshift(eaxw & con(0x3c00), con(0xa)) == con(0x9))
    if (!SKW_IS_CONTAINER_MONEYBOX(eaxw))
      if ((SKW_GET_ADDRESS_OF_RECORD(eaxw)->u4.b.b0 & con(0x6)) == con(0x0))
        return true;
  return false;
}

bool SKW_IS_MISCITEM_CURRENCY(x16 eaxw)
{
  if (urshift(eaxw & con(0x3c00), con(0xa)) == con(0xa))
    if ((SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(eaxw, con(0x0)) & con(0x4000)) != con(0x0))
      return true;
  return false;
}

x16 SKW_TAKE_COIN_FROM_WALLET(x16 eaxw, x16 edxw)
{
  x16 wordrg5;
  x16 wordrg6;
  c_record* recoptrrg1;
  c_record* recoptrrg3;

  x16 wordrg2 = ddata.v1e0394[edxw];
  recoptrrg3 = SKW_GET_ADDRESS_OF_RECORD(eaxw);
  wordrg5 = con(0xffff);
  for (x16 wordrg4 = recoptrrg3->u2.w; ; wordrg4 = SKW_GET_NEXT_RECORD_LINK(wordrg4))
  {
    if (wordrg4 == con(0xfffffffe))
    {
      if (wordrg5 != con(0xffffffff))
      {
        wordrg6 = wordrg5;
        recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
        if ((recoptrrg1->u2.b.b1 & con(0x3f)) != con(0x0))
        {
          x16 tmp = ((urshift(4 * recoptrrg1->u2.w, con(0xa)) - con(0x1)) & con(0x3f)) << con(0x8);
          recoptrrg1->u2.b.b1 &= con(0xffffffc0);
          recoptrrg1->u2.w |= tmp;
          wordrg5 = SKW_ALLOC_NEW_DBITEM(wordrg2);
        }
        else
          SKW_CUT_RECORD_FROM(wordrg6, con(0xffffffff), con(0x0), &recoptrrg3->u2.w);
      }
      return wordrg5;
    }
    if (urshift(wordrg4 & con(0x3c00), con(0xa)) != con(0xa))
      return con(0xffff);
    if (SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg4) == wordrg2)
      wordrg5 = wordrg4;
  }
}

x8 SKW_GET_MAX_CHARGE(x16 eaxw)
{
  if (eaxw == con(0xffffffff))
    return con(0x0);
  ui16 uwordrg1 = urshift(eaxw & con(0x3c00), con(0xa));
  if (uwordrg1 < con(0x6))
  {
    if (uwordrg1 != con(0x5))
      return con(0x0);
    return con(0xf);
  }
  if (uwordrg1 == con(0x6))
    return con(0xf);
  if (uwordrg1 != con(0xa))
    return con(0x0);
  return con(0x3);
}

x16 SKW_0cee_2e35(x8 eaxb)
{
  x16 wordrg4 = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(eaxb, con(0x4));
  if (wordrg4 == con(0x0))
    wordrg4 = con(0x4);
  return wordrg4;
}

bool SKW_IS_CONTAINER_MAP(x16 eaxw)
{
  if (urshift(eaxw & con(0x3c00), con(0xa)) == con(0x9))
    if ((SKW_GET_ADDRESS_OF_RECORD(eaxw)->u4.w & con(0x6)) == con(0x2))
      return true;
  return false;
}

bool SKW_SET_DESTINATION_OF_MINION_MAP(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  x16 wordrg4 = edxw;
  x16 wordrg2 = ebxw;
  x16 vw_00 = ddata.v1d3248;
  SKW_CHANGE_CURRENT_MAP_TO(ecxw);
  bool boolrg5 = (wordrg4 >= con(0x0) && wordrg4 < ddata.savegamewa && wordrg2 >= con(0x0) && wordrg2 < ddata.savegamewb);
  if (boolrg5)
  {
    c_record* recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(eaxw);
    wordrg4 &= con(0x1f);
    recoptrrg1->u6.b.b0 &= con(0xffffffe0);
    wordrg2 &= con(0x1f);
    wordrg2 <<= con(0x5);
    recoptrrg1->u6.w = ((recoptrrg1->u6.w | wordrg4) & con(0xfffffc1f)) | wordrg2;
    recoptrrg1->u6.b.b1 &= con(0x3);
    recoptrrg1->u6.w |= (ecxw & con(0x3f)) << con(0xa);
  }
  SKW_CHANGE_CURRENT_MAP_TO(vw_00);
  return boolrg5;
}

c_record* SKW_GET_MISSILE_REF_OF_MINION(x16 eaxw, x16 edxw)
{
  c_record* recoptrrg3;
  c_record* recoptrrg1;

  if ((eaxw != con(0xffffffff) && eaxw != con(0xfffffffe)) && (recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(eaxw), recoptrrg1->w0 != con(0xffffffff)))
    for (x16 wordrg4 = recoptrrg1->u2.w; wordrg4 != con(0xfffffffe); wordrg4 = SKW_GET_NEXT_RECORD_LINK(wordrg4))
      if ((urshift(wordrg4 & con(0x3c00), con(0xa)) == con(0xe)) && (recoptrrg3 = SKW_GET_ADDRESS_OF_RECORD(wordrg4), (edxw == con(0xffffffff)) || (edxw == recoptrrg3->u2.w)))
        return recoptrrg3;
  return NULL;
}

x8 SKW_GET_WALL_DECORATION_OF_ACTUATOR(c_record* eaxprec)
{
  x16 wordrg4 = urshift(eaxprec->u4.w, con(0xc));
  if (wordrg4 != con(0x0))
  {
    x8** bpptrrg11 = &ddata.savegamepp1[ddata.savegamewa];
    x8* bptrrg12 = *(bpptrrg11 - 1);
    x8* bptrrg2 = bptrrg12 + ddata.savegamewb;
    bptrrg2 += ulrshift(ddata.v1e03c0->wc, con(0x8), con(0xc));
    return bptrrg2[wordrg4 + con(0xffffffff)];
  }
  return con(0xffffffff);
}

x8 SKW_GET_FLOOR_DECORATION_OF_ACTUATOR(c_record* eaxprec)
{
  x16 wordrg4 = urshift(eaxprec->u4.w, con(0xc));
  if (wordrg4 != con(0x0))
  {
    x8** bpptrrg11 = &ddata.savegamepp1[ddata.savegamewa];
    x8* bptrrg12 = *(bpptrrg11 - 1);
    x8* bptrrg3 = bptrrg12 + ddata.savegamewb;
    x8* bptrrg2 = bptrrg3 + ulrshift(ddata.v1e03c0->wc, con(0x8), con(0xc)) + (ddata.v1e03c0->wa & con(0xf));
    return bptrrg2[wordrg4 + con(0xffffffff)];
  }
  return con(0xffffffff);
}

bool SKW_IS_WALL_ORNATE_ALCOVE_FROM_RECORD(x16 eaxw, x16 edxw)
{
  s_8wordarray vs_00;

  x8 vb_10 = SKW_QUERY_CLS2_FROM_RECORD(edxw);
  if (SKW_IS_WALL_ORNATE_ALCOVE(vb_10) == con(0x1))
  {
    SKW_0cee_1a46(&vs_00, ((edxw >> con(0xe)) + con(0x2)) & con(0x3), true, eaxw);
    if (CUTX8(vs_00.warr_00[0x5]) == vb_10)
      return true;
  }
  return false;
}

x16 SKW_0cee_317f(x16 eaxw, x16 edxw)
{
  x16 wordrg1 = SKW_IS_WALL_ORNATE_ALCOVE_FROM_RECORD(eaxw, edxw) ? 1 : 0;
  if (wordrg1 != con(0x0))
  {
    x8 byterg1 = SKW_QUERY_CLS2_FROM_RECORD(edxw);
    if (byterg1 == con(0xffffffff))
      return con(0x0);
    wordrg1 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0x13), byterg1);
  }
  return wordrg1;
}

// was SKW_0cee_3202
x8 SKW_IS_REBIRTH_ALTAR(c_record* eaxprec)
{
  if ((eaxprec->u2.b.b0 & con(0x1)) != con(0x0))
  {
    if ((ddata.v1e03c0->u0.u.b.b3 & con(0x1)) != con(0x0))
      return CUTX8(urshift(ddata.v1e03c0->we, con(0xc)));
    return con(0xffffffff);
  }
  if ((ddata.v1e03c0->u0.u.b.b2 & con(0xffffff80)) == con(0x0))
    return con(0xffffffff);
  return CUTX8(ulrshift(ddata.v1e03c0->we, con(0x4), con(0xc)));
}

x16 SKW_QUERY_DOOR_STRENGTH(x8 eaxb)
{
  x16 wordrg1 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0xe), con(0xb), con(0x11), eaxb);
  if (wordrg1 == con(0x0))
  {
    if (SKW_0cee_3275(eaxb) != con(0x0))
      return con(0x1);
    return con(0x6);
  }
  return wordrg1;
}

// was SKW_101b_0001
void SKW_END_GAME(bool eaxbool)
{
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  if (eaxbool && !ddata.v1e0240)
  {
    x8 byterg1;
    if (UI16CAST(ddata.savegamew2) <= UI16CAST(con(0x0)))
      byterg1 = con(0xfffffffe);
    else
      byterg1 = ddata.hero[0].b101;
    SKW_QUEUE_NOISE_GEN2(con(0x16), con(0x87), con(0xfe), byterg1, ddata.v1e0270, ddata.v1e0272, con(0x0), con(0xff), con(0xff));
    SKW_SLEEP_SEVERAL_TIME(con(0xf0));
  }
  SKW_2066_03e0(con(0x0));
  ddata.dialog2 = true;
  if (eaxbool)
  {
    SKW_FIRE_FILL_SCREEN_RECT(con(0x2), con(0x0));
    if (ddata.v1e13f4)
    {
      ddata.boolsavegamel2 = SKW_0aaf_0067(SKW_0aaf_02f8(con(0x10), con(0x0))) == con(0x0);
      if (ddata.boolsavegamel2 && SKW_GAME_LOAD() != con(0xffffffff))
      {
        SKW__INIT_GAME_38c8_03ad();
        ddata.boolsavegamel2 = false;
        SKW_FIRE_SHOW_MOUSE_CURSOR();
        SKW_1031_098e();
        return;
      }
    }
  }
  SKW_FIRE_FILL_SCREEN_RECT(con(0x2), con(0x0));
  SKW_FIRE_SELECT_PALETTE_SET(0);
  if (ddata.v1e08fc)
    SKW_SK_PREPARE_EXIT();
  if (eaxbool)
  {
    ddata.v1e0438 = true;
    SKW_SK_PREPARE_EXIT();
  }
  SKW_DRAW_TRANSPARENT_STATIC_PIC(con(0x6), con(0x1), con(0x2), con(0x0), con(0xffffffff));
  SKW_FIRE_SELECT_PALETTE_SET(1);
  SKW_1031_0541(con(0x1));
  do
  {
    SKW_EVENT_LOOP();
  } while (ddata.mouse_unk0a != con(0xef));
  ddata.mouse_visibility = con(0x0); // added, quit!
  SKW_SK_PREPARE_EXIT();
}

bool SKW_1031_003e(s_bbw* eaxpbbw)
{
  if (unsignedword(eaxpbbw->b1) == ddata.v1e0976)
    return true;
  if (eaxpbbw->b1 <= con(0x4) || (ddata.v1e0976 != con(0x0) && (eaxpbbw->b1 - con(0x4)) == ddata.v1e0976))
    return false;
  return true;
}

bool SKW_1031_00c5(s_bbw* eaxpbbw)
{
  if (((eaxpbbw->b1 == con(0x0)) && (ddata.v1e00b8 == con(0x0))) || ((eaxpbbw->b1 != con(0x0)) && (ddata.v1e00b8 != con(0x0))))
    return true;
  return false;
}

bool SKW_1031_00f3(s_bbw* eaxpbbw)
{
  if (ddata.v1e0b74 != con(0x0))
    return false;
  if (eaxpbbw->b1 <= con(0x3))
  {
    if ((SKW_GET_PLAYER_AT_POSITION((unsignedword(eaxpbbw->b1) + ddata.v1e0258) & con(0x3))) >= con(0x0))
      return true;
    return false;
  }
  return true;
}

bool SKW_1031_014f(s_bbw* eaxpbbw)
{
  if (ddata.v1e0b74 != con(0x0))
    if (((con(0x1) << ddata.hero[ddata.v1e0b74 - con(0x1)].b1e) & unsignedlong(eaxpbbw->b1)) != con(0x0))
      return true;
  return false;
}

bool SKW_1031_0184(s_bbw* eaxpbbw)
{
  bool flag;

  if (ddata.v1e0b74 != con(0x0))
  {
    if ((ddata.v1e0b62 & con(0x800)) == con(0x0))
      flag = eaxpbbw->b1 != con(0x5);
    else
      flag = eaxpbbw->b1 != ddata.v1e0b7a;
    if (!flag)
      return true;
  }
  return false;
}

c_rect* SKW_1031_01d5(x16 eaxw, c_rect* edxr)
{
  c_rect* recoptrrg3;
  x16 vw_00;
  x16 vw_04;
  x16 wordrg2;

  recoptrrg3 = SKW_QUERY_EXPANDED_RECT(eaxw & con(0x3fff), edxr);
  if (recoptrrg3 == NULL)
    return recoptrrg3;
  if ((eaxw & con(0x8000)) == con(0x0))
  {
    if ((eaxw & con(0x4000)) == con(0x0))
      return recoptrrg3;
    wordrg2 = con(0x12);
  }
  else
    wordrg2 = con(0x7);
  SKW_QUERY_TOPLEFT_OF_RECT(wordrg2, &vw_04, &vw_00);
  recoptrrg3->x += vw_04;
  recoptrrg3->y += vw_00;
  return recoptrrg3;
}

void SKW_1031_050C(void)
{
  if ((ddata.v1e0228 || ddata.v1e0220) || (ddata.v1e0224))
  {
    ddata.v1e0224 = false;
    ddata.v1e0220 = false;
    ddata.v1e0228 = false;
    SKW_FIRE_MOUSE_RELEASE_CAPTURE();
    ddata.mouse_visibility = con(0x1);
    SKW_FIRE_SHOW_MOUSE_CURSOR();
  }
}

bool SKW_1031_0c58(x16 eaxw, s_www* edxps)
{
  x16 wordrg1 = eaxw;
  s_www* ptrrg2 = edxps;
  ddata.mouse_unk09 = con(0xffffffff);
  if (edxps != NULL)
  {
    x16 wordrg3;
    for(;;)
    {
      wordrg3 = ptrrg2->w0[con(0x0)] & con(0x7ff);
      if (wordrg3 == con(0x0))
        break;
      if (((ptrrg2->w0[con(0x2)] & con(0x800)) == con(0x0)) && (wordrg3 == wordrg1))
      {
        SKW_1031_01d5(ptrrg2->w0[con(0x1)], &ddata.v1e0470);
        ddata.mouse_unk05 = ptrrg2->w0[con(0x1)];
        if ((ddata.mouse_unk05 & con(0x8000)) == con(0x0))
        {
          if ((ddata.mouse_unk05 & con(0x4000)) != con(0x0))
            ddata.mouse_unk09 = con(0x12);
        }
        else
          ddata.mouse_unk09 = con(0x7);
        ddata.mouse_unk05 &= con(0x3fff);
        ddata.mouse_unk08 = ddata.v1e0470.x;
        ddata.mouse_unk07 = ddata.v1e0470.y;
        ddata.mouse_unk0a = wordrg3;
        wordrg1 = (edxps->w0[con(0x0)] & con(0x7ff)) - con(0x1);
        wordrg3 -= wordrg1;
        ddata.mouse_unk06 = wordrg3;
        return con(0x1);
      }
      ptrrg2++;
    }
    ddata.v1e0470.h = wordrg3;
    ddata.v1e0470.w = wordrg3;
  }
  ddata.mouse_unk05 = con(0xffffffff);
  ddata.mouse_unk08 = con(0x0);
  ddata.mouse_unk07 = con(0x0);
  ddata.mouse_unk0a = wordrg1;
  ddata.mouse_unk06 = con(0x0);
  return con(0x0);
}

void SKW_HIGHLIGHT_ARROW_PANEL(x8 eaxb, bool ebxbool, x16 edxw)
{
  c_c5rects vs_00;

  x8 byterg3 = eaxb;
  ddata.v1e0548 = eaxb;
  ddata.v1e053e = edxw;
  ddata.v1e0534 = ebxbool;
  if (ebxbool)
    byterg3++;
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_0b36_0c52(&vs_00, true, edxw);
  SKW_FILL_ENTIRE_PICT(SKW_3e74_5817(vs_00.s_00.w_00), ddata.paletteptr1[0]);
  SKW_DRAW_ICON_PICT_ENTRY(con(0x1), byterg3, &vs_00, con(0x3), edxw, con(0xffff));
  SKW_0b36_0cbe(&vs_00, true);
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_WAIT_SCREEN_REFRESH();
}

void SKW_12b4_00af(x16 eaxw)
{
  SKW_MOVE_RECORD_TO(con(0xffff), ddata.v1e0272, con(0xffffffff), ddata.v1e0270, con(0x0));
  ddata.v1d27f8 = SKW_LOCATE_OTHER_LEVEL(ddata.v1e0266, &ddata.v1e0270, &ddata.v1e0272, (eaxw == con(0x0)) ? con(0x1) : con(0xffffffff), NULL);
  SKW_CHANGE_CURRENT_MAP_TO(ddata.v1d27f8);
  SKW_ROTATE_SQUAD(SKW_0cee_06dc(ddata.v1e0270, ddata.v1e0272));
  SKW_CHANGE_CURRENT_MAP_TO(ddata.v1e0266);
}

bool SKW_121e_03ae(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2)
{
  x16 parw01 = (((ddata.v1e0258 + argw1) & con(0x3)) << con(0xe)) | (ddata.savegamewpc.w0 & con(0xffff3fff));
  x16 wordrg1 = SKW_32cb_03a6(eaxw, argw0, argw0, edxw, ecxw, parw01, argw2, true);
  if (wordrg1 == con(0xffffffff))
  {
    SKW_REMOVE_OBJECT_FROM_HAND();
    return true;
  }
  if (wordrg1 != con(0xfffffffe))
    return false;
  return SKW_121e_0222(argw0, (argw2 != con(0x2)) ? argw1 : argw1 + con(0x4), ecxw);
}

void SKW_1031_0d36(x16 eaxw, x16 edxw)
{
  SKW_3e74_5b7c(edxw);
  ddata.fetchmouse_busy = true;
  if (ddata.mousebufentries < con(0x7))
  {
    x16 wordrg1 = ddata.mousebufindex;
    wordrg1 += con(0x2);
    if (wordrg1 > con(0xa))
      wordrg1 -= con(0xb);
    wordrg1--;
    if (wordrg1 == con(0xffffffff))
      wordrg1 = con(0xa);
    ddata.mousebufindex = wordrg1;
    ddata.mousebuf[wordrg1].b = eaxw;
    ddata.mousebuf[wordrg1].x = edxw;
    ddata.mousebufentries++;
  }
  ddata.fetchmouse_busy = false;
  SKW_GETSINGLEMOUSEEVENT();
}

// was SKW_1031_16a0
void SKW_SELECT_CHAMPION_LEADER(x16 eaxw)
{
  if (eaxw == ddata.mouse_unk0e)
    return;
  if (eaxw != con(0xffffffff) && ddata.hero[eaxw].w36 == con(0x0))
    return;
  if (ddata.mouse_unk0e != con(0xffffffff))
    ddata.hero[ddata.mouse_unk0e].u32.b.b1 |= con(0x14);
  ddata.mouse_unk0e = eaxw;
  if (eaxw != con(0xffffffff) && (eaxw + con(0x1)) != ddata.v1e0288)
    ddata.hero[eaxw].u32.b.b1 |= con(0x14);
}

// was SKW_12b4_0141
void SKW_PERFORM_TURN_SQUAD(x16 eaxw)
{
  c_5bytes vs_00; // TODO .b_00 undefined!

  if (eaxw != con(0x0))
  {
    ddata.v1e0488 = true;
    SKW_RESET_SQUAD_DIR();
    x16 wordrg4 = unsignedword(SKW_GET_TILE_VALUE(ddata.v1e0270, ddata.v1e0272));
    if (urshift(wordrg4, con(0x5)) != con(0x3))
    {
      wordrg4 = ddata.v1e0270;
      bool boolrg1 = SKW_GET_TELEPORTER_DETAIL(&vs_00, ddata.v1e0272, ddata.v1e0270) ? 1 : 0;
      if (!boolrg1)
      {
        wordrg4 = ddata.v1e0272;
        R_3CE7D(ddata.v1e0270, con(0xffff), true, ddata.v1e0272, boolrg1, boolrg1);
        SKW_ROTATE_SQUAD((((eaxw != con(0x2)) ? con(0x3) : con(0x1)) + ddata.v1e0258) & con(0x3));
        R_3CE7D(ddata.v1e0270, con(0xffff), true, ddata.v1e0272, true, false);
      }
      else
        R_3BF83(unsignedword(vs_00.b_02), unsignedword(vs_00.b_04), unsignedword(vs_00.b_01), unsignedword(vs_00.b_03));
    }
    else
      SKW_12b4_00af(wordrg4 & con(0x4));
  }
}

void SKW_14cd_1fa7(s_hexe* ebxphe, x8 edxb)
{
  x16 wordrg3 = (unsignedword(edxb & con(0xffffffe0)) | (ddata.v1e08d8 & con(0x1f))) & con(0xfffffc1f);
  wordrg3 |= (ddata.v1e08d4 & con(0x1f)) << con(0x5);
  wordrg3 &= con(0x3ff);
  wordrg3 |= (ddata.v1e08d6 & con(0x3f)) << con(0xa);
  SKW_14cd_18f2(con(0x16), ebxphe, con(0x0), edxb, wordrg3);
}

void SKW_MARK_DYN_LOAD(ui32 eaxul)
{
  u_bbw* ubwptrrg1 = UPCAST(u_bbw, &ddata.v1e09a0[3 * ddata.v1e09a4++]);
  ubwptrrg1[con(0x0)].w = con(0x0);
  ubwptrrg1[con(0x1)].b.b0 = CUTLX8(eaxul >> con(0x18));
  ubwptrrg1[con(0x1)].b.b1 = CUTLX8(eaxul >> con(0x10));
  ubwptrrg1[con(0x2)].b.b0 = CUTLX8(eaxul >> con(0x8));
  ubwptrrg1[con(0x2)].b.b1 = CUTLX8(eaxul);
LOGX(("SKW_MARK_DYN_LOAD : %02X-%02X-%02X-%02X\n", (ui16) ubwptrrg1[con(0x1)].b.b0, (ui16) ubwptrrg1[con(0x1)].b.b1, (ui16) ubwptrrg1[con(0x2)].b.b0, (ui16) ubwptrrg1[con(0x2)].b.b1));
}
