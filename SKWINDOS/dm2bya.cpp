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



// BUGS: resume-button after party died
// Spell-damage, moving backwards to new level!
// set enable_video = false during certain palette-operations?
// TODO: SKW_3e74_5673, check if 1st arg really needs to be a long
// TODO: some more BUGS. check stop=1 for them.

// TODO: original BUG: here is a chance for an uninitialized wordrg5.
//  It cannot be the incoming, because that is a pointer,
// but here it is no pointer (compare is wordwise).
// For that I set a preinit of 0.
// (The only caller is SKW_QUERY_PICST_IT - eaxpimg given to RG5P
// TODO: There is more odd with RG1Bhi!
void SKW_44c8_2351(t_gfxdata* eaxpg, x16 ebxw, x16 ecxw, t_gfxdata* edxpg, x16 argw0, x16 argw1)
{
  x16 wordrg5 = con(0x0); // TODO: BUGFIX original problem???
  ddata.srcbmp = eaxpg;
  ddata.destbmp = edxpg;
  x16 vw_04 = (ebxw << con(0x7)) / argw0;
  x16 vw_10 = (ecxw << con(0x7)) / argw1;
  x16 vw_08 = vw_10 / 2;
  x16 vw_0c = con(0x0);
  for(;;)
  {
    x16 wordrg4 = vw_0c;
    if (wordrg4 >= argw1)
      return;
    x16 wordrg1 = urshift(vw_08, con(0x7));
    x16 vw_18 = wordrg1;
    if (wordrg5 != wordrg1 || wordrg4 == con(0x0))
    {
      x16 wordrg3 = argw0;
      x16 wordrg11 = vw_04;
      t_gfxdata* gptrrg6 = &edxpg[argw0 * vw_0c];
      wordrg5 = vw_18 * ebxw;
      t_gfxdata* gptrrg5 = &eaxpg[wordrg5]; // index was + rg2l, which was unsignedlong(RG1Bhi), and this was set by unsignedlong(vw_04), so 0 
      x8 byterg4h = CUTX8(wordrg11);
      wordrg11 *= 2;
      x8 byterg4 = CUTX8(wordrg11);
      x8 byterg2 = CUTX8(wordrg11 >> con(0x8)); // was RG1Bhi; RG1W was doubled, so this one can be nonzero
      do
      {
        *gptrrg6++ = *gptrrg5;
        ui16 tmp = unsignedword(byterg4h) + unsignedword(byterg4);
        byterg4h += byterg4;
        gptrrg5 += byterg2 + ((tmp & 0xff00) != con(0x0) ? 1 : 0);
      } while (--wordrg3 != con(0x0));
    }
    else
    {
      ddata.srcbmp = edxpg;
//                            srcofs               width         destofs
      DM2_blit8pppixel((vw_0c - con(0x1)) * argw0, argw0, wordrg4 * argw0);
      ddata.srcbmp = eaxpg;
    }
    wordrg5 = vw_18;
    vw_08 += vw_10;
    vw_0c++;
  }
}

x16 SKW_48ae_05ae(x16 eaxw, x16 ebxw, x16 ecxw, x8 edxb, x16 argw0, x16 argw1)
{
  x16 vw_0c;
  x16 vw_14;

  x16 vw_10 = ebxw;
  x8 byterg2 = SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(eaxw);
  x16 wordrg5 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(SKW_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(eaxw), con(0xb), con(0x2), byterg2); // BUGFIX 4.1/2014
  s_x40bytes*  ptrrg6 = UPCAST(s_x40bytes, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0x40)));
  x16 wordrg22 = vw_10 * con(0x3) + con(0x12);
  vw_14 = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(edxb, CUTX8(vw_10) * con(0x3) + con(0xffffffa2));
  if (vw_14 != con(0x0) && SKW_QUERY_CREATURES_ITEM_MASK(edxb, ptrrg6, false, CUTX8(wordrg22)))
    if (((con(0x1) << (eaxw & con(0x7))) & ptrrg6->barr_00[eaxw >> con(0x3)]) != con(0x0)) // BUGFIX 4.1/2014
      wordrg5 = vw_14 * wordrg5 / con(0x64);
  x16 wordrg21 = vw_10 * con(0x3) + con(0x11);
  vw_0c = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(edxb, CUTX8(vw_10) * con(0x3) + con(0xffffffa1));
  if (vw_0c != con(0x0))
    if (SKW_QUERY_CREATURES_ITEM_MASK(edxb, ptrrg6, false, CUTX8(wordrg21))) // BUGFIX 2 / 2014
      if (((con(0x1) << (eaxw & con(0x7))) & ptrrg6->barr_00[ulrshift(eaxw, con(0x10), con(0x13))]) != con(0x0))
        wordrg5 = vw_0c * wordrg5 / con(0x64);
  DM2_DEALLOC_LOBIGPOOL(con(0x40));
  if (argw1 < con(0x0))
    argw1 = unsignedword(SKW_GET_MAX_CHARGE(SKW_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(eaxw) << con(0xa))); // BUGFIX 4.1/2014
  wordrg5 += DM2_QUERY_GDAT_ENTRY_DATA_INDEX(SKW_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(eaxw), con(0xb), con(0x35), SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(eaxw)) * argw1;
  if (argw0 != con(0x0))
  {
    x16 wordrg11 = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(edxb, CUTX8(vw_10) * con(0x3) + con(0xffffffa0));
    if (wordrg11 == con(0x0))
      wordrg11 = con(0xa);
    wordrg11 *= wordrg5;
    wordrg11 /= con(0x64);
    if (ecxw == con(0x0))
    {
      if (wordrg5 > con(0x1))
        wordrg5 -= DM2_MAX(wordrg11, con(0x1));
    }
    else
      wordrg5 += wordrg11;
  }
  return wordrg5;
}

void SKW_12b4_023f(x16 eaxw, x16* ebxpw, x16* ecxpw, x16 edxw, x16 argw0, x16 argw1)
{
  x16 vwa_08[0xa];

  argw1 += argw0 + con(0x2);
  *ebxpw = vwa_08[0x0] = SKW_2c1d_028c(eaxw, argw1 & con(0x3), edxw);
  *ecxpw = vwa_08[0x1] = SKW_2c1d_028c(eaxw, (argw1 + con(0x1)) & con(0x3), edxw);
  if (vwa_08[0x0] == vwa_08[0x1])
    vwa_08[0x1] = con(0xffffffff);
  for (x16 wordrg6 = con(0x0); wordrg6 < con(0x2); wordrg6++)
    if (vwa_08[wordrg6] >= con(0x0))
      if (SKW_WOUND_PLAYER(vwa_08[wordrg6], con(0x18), con(0x2), con(0x1)) != con(0x0))
        SKW_QUEUE_NOISE_GEN2(con(0x16), con(0x8a), con(0xfe), ddata.hero[vwa_08[wordrg6]].b101, eaxw, edxw, con(0x1), con(0x64), con(0xc8));
}

bool SKW_12b4_0953(c_record* eaxprec, x16 edxw)
{
  x16 wordrg2 = (ulrshift(eaxprec->ue.w, con(0x6), con(0xe)) + edxw) & con(0x3);
  if (wordrg2 == con(0x1) || wordrg2 == con(0x3))
    if (((SKW_QUERY_CREATURE_5x5_POS(eaxprec, wordrg2) % con(0x5)) - con(0x2)) != con(0x0))
      return true;
  return false;
}

void SKW_LOAD_NEWMAP(x16 eaxw, x16 ebxw, bool ecxbool, x16 edxw)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("SKW_LOAD_NEWMAP");
  if (!dm2_dballochandler.v1e0a84)
  {
    SKW_FIRE_HIDE_MOUSE_CURSOR();
    if (ecxbool)
      SKW_3a15_38b6(false);
  }
  SKW_LOAD_LOCALLEVEL_GRAPHICS_TABLE(eaxw, ebxw, edxw);
  SKW_LOAD_LOCALLEVEL_DYN();
DEBUG_DUMP_ULP();

  if (!dm2_dballochandler.v1e0a84)
  {
    SKW_3a15_38b6(true);
    SKW_FILL_CAII_CUR_MAP();
    ddata.v1e0390.b0 |= con(0x2);
    SKW_CHECK_RECOMPUTE_LIGHT();
    if (dm2_dballochandler.v1e13fe[0] == con(0x0)) // TODO: v1e13fe probably a bool-array
      SKW_1031_098e();
    SKW_FIRE_SHOW_MOUSE_CURSOR();
  }
SPX_DEBUG_POP;
}

c_350* SKW_13e4_0004(x16 eaxw, x8 ebxb, x16 ecxw, x8 edxb)
{
  c_350* ptrrg5 = NULL;
  x16 vw_00 = ddata.v1d3248;
  ddata.v1d3248 = con(0xffffffff);
  SKW_1c9a_0648(vw_00);
  ddata.v1e08a8 = con(0xffffffff);
  if (ddata.s350.v1e07ea != con(0x0))
  {
    ptrrg5 = UPCAST(c_350, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0x350)));
    SKW_COPY_MEMORY(DOWNCAST(c_350, &ddata.s350), con(0x350), DOWNCAST(c_350, ptrrg5));
  }
  ddata.s350.v1e07ea = con(0x1);
  ddata.s350.v1e07eb = con(0x0);
  ddata.s350.v1e054c = eaxw;
  c_record* recoptrrg3 = SKW_GET_ADDRESS_OF_RECORD(eaxw);
  ddata.s350.v1e054e = recoptrrg3;
  s_d556* sd5ptrrg2;
  if (recoptrrg3->u4.b.b1 == con(0xffffffff))
    sd5ptrrg2 = NULL;
  else
    sd5ptrrg2 = &ddata.v1e089c[recoptrrg3->u4.b.b1];
  ddata.s350.v1e0556 = sd5ptrrg2;
  ddata.s350.v1e0552 = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(recoptrrg3->u4.b.b0);
  ddata.s350.v1e055e = SKW_1c9a_02c3(recoptrrg3, ddata.s350.v1e0552);
  ddata.s350.v1e0571 = CUTX8(vw_00);
  ddata.s350.v1e0562.un.l_00 = (signedlong(vw_00) << con(0x18)) | ddata.longmallocx;
  ddata.s350.v1e0562.b_05 = recoptrrg3->u4.b.b0;
  ddata.s350.v1e0562.b_04 = CUTX8(ecxw);
  ddata.s350.v1e0562.u.b.b_06 = edxb;
  ddata.s350.v1e0562.u.b.b_07 = ebxb;
  ddata.s350.v1e055a = NULL;
  ddata.s350.v1e0570 = con(0x0);
  ddata.s350.v1e0584 = con(0xffffffff);
  if (ecxw == con(0x22))
  {
    ddata.s350.v1e0572 = con(0x0);
    ddata.s350.v1e0574 = con(0x0);
    x8 byterg1 = sd5ptrrg2->u.v.b1a;
    if (byterg1 == con(0xffffffff))
      byterg1 = con(0x0);
    ddata.s350.v1e056e = byterg1;
    DM2_ZERO_MEMORY(DOWNCAST(x16, &sd5ptrrg2->u.v.u18.w), con(0xa)); // okay, fills the rest of the struct
    sd5ptrrg2->u.v.b1a = con(0xffffffff);
  }
  return ptrrg5;
}

void SKW_13e4_01a3(void)
{
  if (ddata.s350.v1e07eb == con(0x0))
  {
    ddata.s350.v1e07eb = con(0x1);
    c_aispec* aiptrrg2 = ddata.s350.v1e0552;
    c_record* recoptrrg3 = ddata.s350.v1e054e;
    if (ddata.s350.v1e0584 == con(0xffffffff))
      ddata.s350.v1e0584 = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(recoptrrg3->u4.b.b0, con(0x1));
    ddata.s350.v1e0576 = aiptrrg2->wa;
    ddata.s350.v1e0578 = aiptrrg2->we;
    ddata.s350.v1e057a = aiptrrg2->w10;
    ddata.s350.v1e057c = aiptrrg2->wc;
    ddata.s350.v1e057e = aiptrrg2->w12;
    if (SKW_1c9a_08bd(recoptrrg3))
      ddata.s350.v1e0576 &= con(0x7fff);
    ddata.s350.v1e058c = con(0x1);
    DM2_ZERO_MEMORY(ddata.s350.v1e07ee, con(0xa8));
    ddata.s350.v1e0898 = NULL;
    ddata.s350.v1e0896 = con(0x4);
    ddata.s350.v1e07ec = con(0x0);
    ddata.s350.v1e0582 = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(recoptrrg3->u4.b.b0, con(0x7));
    x16 wordrg3 = signedword(CUTLX8(ddata.longmallocx) - ddata.s350.v1e0556->barr04[con(0x0)]);
    if (wordrg3 < con(0x0))
      wordrg3 += con(0x100); // mod
    x16 wordrg2 = 2 * (con(0xf) - (aiptrrg2->w16 & con(0xf)));
    x32 rg1l = con(0x1837a7); // TODO: O_O BUG? because it's compared against 0 only
    x16 wordrg4 = (rg1l == con(0x0)) ? con(0x0) : (signedlong(wordrg2) / 4);
    wordrg2++;
    wordrg4 += SKW_RAND16(wordrg2);
    ddata.s350.v1e058d = (wordrg4 <= wordrg3) ? 1 : 0;
    x16 vw_00;
    if (SKW_3e74_5673(UI32CAST(ddata.s350.v1e054c & con(0x3ff)) | con(0x20000000), con(0x0), &vw_00) != con(0x0))
      SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, SKW_3e74_5817(vw_00)), con(0xe), DOWNCAST(c_bbbbwwwp, &ddata.s350.v1e07d8)); // TODO: ATTENTION destination type
    else
    {
      DM2_ZERO_MEMORY(DOWNCAST(c_bbbbwwwp, &ddata.s350.v1e07d8), con(0xe));
      ddata.s350.v1e07d8.b3 = con(0xffffffff);
      ddata.s350.v1e07d8.w8 = con(0xffffffff);
    }
  }
}

void SKW_13e4_0360(x16 eaxw, x16 ebxw, x8 ecxb, x16 edxw, bool argbool0)
{
  x16 wordrg1 = eaxw;
  if (wordrg1 == con(0xffffffff))
  {
    wordrg1 = SKW_GET_CREATURE_AT(edxw, ebxw);
    if (wordrg1 == con(0xffffffff))
      return;
  }
  x16 wordrg4 = wordrg1;
  x8 byterg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg4)->u4.b.b1;
  if (byterg1 != con(0xffffffff))
  {
    s_d556* sd5ptrrg1 = &ddata.v1e089c[byterg1];
    if (sd5ptrrg1->u.v.barr16[con(0x1)] != con(0x13) && sd5ptrrg1->u.v.b1a != con(0x13))
    {
      sd5ptrrg1->u.v.barr16[con(0x1)] = ecxb;
      if (argbool0)
      {
        if ((table1d613a[sd5ptrrg1->u.v.b1a] & con(0x10)) == con(0x0))
        {
          SKW_1c9a_0db0(wordrg4);
          SKW_1c9a_0cf7(edxw, ebxw);
        }
        else
          sd5ptrrg1->u.v.b21 = con(0x1);
      }
    }
  }
}

void SKW_13e4_071b(void)
{
  x16* wptrrg2 = ddata.s350.v1e055e;
  if ((wptrrg2[con(0x1)] & con(0xe03f)) != con(0x8001))
  {
    x16 wordrg3 = GET_ANIM_INFO_SEQUENCE(ddata.s350.v1e054e->u4.b.b0, wptrrg2[con(0x0)]);
    x16 wordrg5 = wptrrg2[con(0x1)] & con(0xfc0);
    x16 wordrg4 = (unsignedlong(wordrg5) + ddata.longmallocx) % wordrg3;
    if (wordrg4 != con(0x0))
    {
      wptrrg2[con(0x1)] = wordrg5 | con(0xc000) | wordrg3;
      SKW_1c9a_0db0(ddata.s350.v1e054c);
      x32 longrg4 = signedlong(wordrg3) - signedlong(wordrg4) + ddata.longmallocx;
      longrg4 |= unsignedlong(ddata.s350.v1e0571) << con(0x18);
      ddata.s350.v1e0562.un.l_00 = longrg4;
      ddata.s350.v1e0556->w02 = SKW_QUEUE_TIMER(&ddata.s350.v1e0562);
    }
    else
      wptrrg2[con(0x1)] = wordrg5 | con(0x8001);
  }
}

void SKW_13e4_0806(void)
{
  x16* wptrrg2 = ddata.s350.v1e055e;
  if ((wptrrg2[con(0x1)] & con(0xe000)) == con(0x8000) && (wptrrg2[con(0x1)] & con(0x3f)) > con(0x1))
    return;
  x16 wordrg3 = GET_ANIM_INFO_SEQUENCE(ddata.s350.v1e054e->u4.b.b0, wptrrg2[con(0x0)]);
  x16 wordrg5 = wptrrg2[con(0x1)] & con(0xfc0);
  x16 wordrg4 = (unsignedlong(wordrg5) + ddata.longmallocx) % wordrg3;
  x16 wordrg1 = wordrg3 | wordrg5;
  if (wordrg4 != con(0x0))
  {
    wordrg1 |= con(0xa000);
    wptrrg2[con(0x1)] = wordrg1;
    SKW_1c9a_0db0(ddata.s350.v1e054c);
    x32 longrg4 = signedlong(wordrg3) - signedlong(wordrg4) + ddata.longmallocx;
    longrg4 |= unsignedlong(ddata.s350.v1e0571) << con(0x18);
    ddata.s350.v1e0562.un.l_00 = longrg4;
    ddata.s350.v1e0556->w02 = SKW_QUEUE_TIMER(&ddata.s350.v1e0562);
  }
  else
  {
    wordrg1 |= con(0x8000);
    wptrrg2[con(0x1)] = wordrg1;
  }
}

// was SKW_13e4_08ff
void SKW_ANIMATE_CREATURE(x16 eaxw, bool ebxbool, x16 edxw)
{
  x16 vw_04 = SKW_GET_CREATURE_AT(eaxw, edxw);
  if (vw_04 != con(0xffffffff))
  {
    x16 wordrg3 = vw_04;
    if ((SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg3) & con(0x1)) != con(0x0) && SKW_GET_ADDRESS_OF_RECORD(wordrg3)->u4.b.b1 == con(0xffffffff))
      SKW_ALLOC_CAII_TO_CREATURE(wordrg3, edxw, eaxw);
    c_350* ptrrg4 = SKW_13e4_0004(vw_04, CUTX8(edxw), con(0x21), CUTLX8(eaxw));
    if (ddata.s350.v1e0556 != NULL)
    {
      if (ebxbool)
        SKW_13e4_0806();
      else
        SKW_13e4_071b();
    }
    SKW_13e4_0329(ptrrg4);
  }
}

void SKW_14cd_0276(c_sizex16* eaxpsx16)
{
  x16 wordrg1 = DM2_MAX(con(0x0), signedword(eaxpsx16->s_00.b_06));
  ddata.s350.v1e07d8.b1 = CUTX8(wordrg1);
  ddata.s350.v1e07d8.b0 = CUTX8(wordrg1);
  ddata.s350.v1e07d8.w8 = eaxpsx16->s_00.u_04.w;
  ddata.s350.v1e07d8.b3 = eaxpsx16->s_00.b_07;
  ddata.s350.v1e07d8.w4 = eaxpsx16->u_08.w;
  ddata.s350.v1e07d8.w6 = eaxpsx16->u_0a.w;
  ddata.s350.v1e07d8.b2 = eaxpsx16->b_0e[con(0x3)];
  ddata.s350.v1e07d8.phe_a = eaxpsx16->ps_12;
  t_gfxdata* gptrrg5 = NULL;
  if (wordrg1 > con(0x0))
  {
    x32 longrg3 = 2 * signedlong(wordrg1);
    x16 vw_00;
    x32 longrg1 = SKW_3e74_5673(UI32CAST(ddata.s350.v1e054c & con(0x3ff)) | con(0x30000000), con(0x1), &vw_00);
    if (longrg1 >= longrg3)
      gptrrg5 = SKW_3e74_5817(vw_00);
    else
    {
      if (longrg1 == con(0x0))
        gptrrg5 = UPCAST(t_gfxdata, DOWNCAST(s_xmalloc, SKW_ALLOC_CPXHEAP_MEM(vw_00, longrg3)));
      else
        gptrrg5 = UPCAST(t_gfxdata, DOWNCAST(s_xmalloc, SKW_3e74_5788(vw_00, longrg3)));
      SKW_3e74_585a(vw_00, true);
    }
    SKW_COPY_MEMORY(DOWNCAST(u_bbw, ddata.s350.v1e060e), longrg3, DOWNCAST(t_gfxdata, gptrrg5)); // TODO: special
  }
  ddata.s350.v1e07e6 = UPCAST(x16, gptrrg5); // TODO: special
}

x8 SKW_14cd_0389(void)
{
  x8 vb_00;
  bool boolrg1 = false;
  if (ddata.s350.v1e07d8.b0 != con(0x0) && ddata.s350.v1e07d8.b1 != con(0x0) && ddata.s350.v1e07d8.b3 != con(0xffffffff))
  {
    ddata.s350.v1e0674 = con(0x0);
    if (ddata.s350.v1e0556->u.v.b12 != con(0xffffffff))
    {
      vb_00 = ddata.s350.v1e0556->u.v.b12;
      x8 byterg41 = ddata.s350.v1e0556->u.v.b13;
      const s_seven* tmpp = table1d5f82[vb_00];
      x8 byterg4 = tmpp[byterg41].b0[con(0x6)];
      SKW_14cd_0f0a(tmpp[byterg4].b0[con(0x5)], vb_00, ddata.s350.v1e07d8.phe_a, byterg4);
    }
    if (ddata.s350.v1e0674 != con(0x0))
    {
      boolrg1 = SKW_1c9a_38a8();
      if (boolrg1)
        ddata.s350.v1e0556->u.v.u18.w = ddata.s350.v1e0678[0].s_00.u_02.w;
    }
    else
      boolrg1 = false;
  }
  return (!boolrg1) ? con(0xffffffff) : vb_00; // BUGFIX 4/2014
}

x8 SKW_14cd_062e(void)
{
  x8 byterg4 = con(0x0);
  x8 byterg4h = ddata.s350.v1e0556->u.v.b12;
  if (byterg4h != con(0xffffffff))
  {
    const s_seven* s7ptrrg4 = table1d5f82[byterg4h];
    const s_seven* s7ptrrg1 = &s7ptrrg4[ddata.s350.v1e0556->u.v.b13];
    byterg4 = s7ptrrg1->b0[con(0x5)] & con(0xffffffe0);
    if ((s7ptrrg1->b0[con(0x5)] & con(0x60)) == con(0x40))
      if (ddata.s350.v1e0571 != ddata.v1e08d6)
        byterg4 = con(0x0);
  }
  return byterg4;
}

x8 SKW_DECIDE_NEXT_XACT(void)
{
  x8 vb_00 = ddata.s350.v1e0556->u.v.b12;
  x8 byterg4h = ddata.s350.v1e0556->u.v.b13;
  const s_seven* s7ptrrg6 = table1d5f82[vb_00];
  const s_seven* s7ptrrg1;
  for(;;)
  {
    s7ptrrg1 = &s7ptrrg6[byterg4h];
    x8 byterg4 = s7ptrrg1->b0[con(0x0)];
    if (byterg4 >= con(0x0))
      break;
    if (byterg4 == con(0xfffffff6))
    {
      byterg4 = s7ptrrg1->b0[con(0x1)];
      if (byterg4 >= con(0x0) && byterg4 < con(0x2))
        ddata.s350.v1e0556->u.arr.w0e[byterg4] = signedword(s7ptrrg1->b0[con(0x2)]);
    }
    byterg4h++;
  }
  ddata.s350.v1e0556->u.v.b12 = vb_00;
  ddata.s350.v1e0556->u.v.b13 = byterg4h;
  x8 byterg42 = s7ptrrg1->b0[con(0x0)];
  ddata.s350.v1e0572 = signedword(s7ptrrg1->b0[con(0x3)]);
  ddata.s350.v1e0574 = signedword(s7ptrrg1->b0[con(0x4)]);
  return byterg42;
}

bool SKW_14cd_08f5(x8 eaxb)
{
  bool boolrg2 = false;
  x8 vb_00 = ddata.s350.v1e0556->u.v.b12;
  x8 byterg1h = ddata.s350.v1e0556->u.v.b13;
  const s_seven* s7ptrrg6 = table1d5f82[vb_00];
  x8 byterg1;
  if (eaxb != con(0xfffffffe))
    byterg1 = s7ptrrg6[byterg1h].b0[con(0x2)];
  else
    byterg1 = s7ptrrg6[byterg1h].b0[con(0x1)];
  if (byterg1 == con(0xfffffffe) || byterg1 == con(0xfffffffd))
  {
    ddata.s350.v1e0556->u.v.b12 = con(0xffffffff);
    ddata.s350.v1e0556->u.v.b13 = con(0x0);
    return true;
  }
  if (byterg1 > con(0xfffffffb) || byterg1 < con(0xfffffff8))
  {
    boolrg2 = byterg1h != byterg1;
    byterg1h = byterg1;
  }
  else
  {
    if (byterg1 != con(0xfffffff9))
    {
      if (byterg1 != con(0xfffffff8))
        byterg1h += byterg1 != con(0xfffffffb) ? con(0xffffffff) : con(0x1);
      else
        byterg1h += con(0x2);
      boolrg2 = true;
    }
  }
  ddata.s350.v1e0556->u.v.b12 = vb_00;
  ddata.s350.v1e0556->u.v.b13 = byterg1h;
  return boolrg2;
}

void SKW_14cd_0f0a(x8 eaxb, x8 ebxb, s_hexe* ecxphe, x8 edxb)
{
  ddata.s350.v1e0580 = con(0xffffffff);
  switch (eaxb & con(0x1f))
  {
    case 0:
      SKW_14cd_18cc(edxb, ecxphe, ebxb);
    break;

    case 1:
      SKW_14cd_19a4(edxb, ecxphe, ebxb);
    break;

    case 2:
      SKW_14cd_1a3c(edxb, ecxphe, ebxb);
    break;

    case 3:
      SKW_14cd_1a5a(edxb, ecxphe, ebxb);
    break;

    case 4:
      SKW_14cd_1b74(edxb, ecxphe, ebxb);
    break;

    case 5:
      SKW_14cd_1b90(edxb, ecxphe, ebxb);
    break;

    case 6:
      SKW_14cd_1c27(edxb, ecxphe, ebxb);
    break;

    case 7:
      SKW_14cd_1c45(edxb, ecxphe, ebxb);
    break;

    case 8:
      SKW_14cd_1c63(ecxphe, ebxb);
    break;

    case 9:
      SKW_14cd_1c8d(edxb, ecxphe, ebxb);
    break;

    case 10:
      SKW_14cd_1cec(ecxphe, ebxb);
    break;

    case 11:
      SKW_14cd_1d42(ecxphe, ebxb);
    break;

    case 12:
      SKW_14cd_1e36(edxb, ecxphe, ebxb);
    break;

    case 13:
      SKW_14cd_1e52(edxb, ecxphe, ebxb);
    break;

    case 14:
      SKW_14cd_1e6e(edxb, ecxphe, ebxb);
    break;

    case 15:
      SKW_14cd_1f8b(edxb, ecxphe, ebxb);
    break;

    case 16:
      SKW_14cd_1fa7(ecxphe, ebxb);
    break;

    default: throw(THROW_DMABORT);
  }
}

void SKW_14cd_0f3c(x8 eaxb, const s_hexe* ebxphe, x8 ecxb, s_hexe* edxphe, x8 argb0, x16 argw1, x8 argb2, x8 argb3)
{
  x8 byterg2 = argb0;
  if (ebxphe != NULL && edxphe != NULL && ddata.s350.v1e0674 < con(0x10))
  {
    x8 byterg13 = ebxphe->b_08;
    if ((unsignedword(ddata.s350.v1e0571) != ddata.v1e08d6) && (byterg13 > con(0x0)))
      if ((ddata.s350.v1e0552->u0.b.b1 & con(0x40)) == con(0x0))
      {
        byterg13 >>= con(0x2);
        byterg2 >>= con(0x2);
      }
    x16 byterg4 = CUTX8(DM2_BETWEEN_VALUE(con(0xffffffff), con(0x7f), signedword(byterg13) + signedword(byterg2)));
    if (byterg4 >= con(0x0))
    {
      c_sizex16* sx16ptrrg1 = &ddata.s350.v1e0678[ddata.s350.v1e0674++];
      sx16ptrrg1->b_0e[con(0x0)] = argb2;
      sx16ptrrg1->b_0e[con(0x1)] = argb3;
      sx16ptrrg1->w_0c = argw1;
      sx16ptrrg1->s_00.b_00 = byterg4;
      sx16ptrrg1->s_00.b_01 = ebxphe->b_09;
      sx16ptrrg1->s_00.b_07 = eaxb;
      sx16ptrrg1->u_08.w = ebxphe->w_04;
      sx16ptrrg1->u_0a.w = ebxphe->w_06 & ddata.s350.v1e0580;
      sx16ptrrg1->b_0e[con(0x3)] = ecxb;
      sx16ptrrg1->ps_12 = edxphe;
    }
  }
}

void SKW_14cd_18f2(x8 eaxb, s_hexe* ebxphe, x8 ecxb, x8 edxb, x16 argw0)
{
  x8 vb_18 = eaxb;
  if (ebxphe != NULL)
  {
    if (eaxb < con(0x0))
      vb_18 = -vb_18;
    s_hexe* sheptrrg3;
    s_hexe vhe_00;
    do
    {
      if (ebxphe->b_0c == vb_18)
        if (SKW_14cd_1316(ebxphe->b_01, edxb, ebxphe->u_02.w))
        {
          SKW_COPY_MEMORY(DOWNCAST(s_hexe, ebxphe), sizeof(s_hexe), DOWNCAST(s_hexe, &vhe_00));
          if (eaxb < con(0x0))
          {
            vhe_00.b_08 = con(0x0);
            vhe_00.b_09 = con(0x0);
          }
          SKW_14cd_0f3c(ebxphe->b_00, &vhe_00, vb_18, ebxphe, ecxb, argw0, edxb, vb_18);
        }
      sheptrrg3 = ebxphe++;
    } while (sheptrrg3->b_0d != con(0x0));
  }
}

void SKW_14cd_19c2(x8 eaxb, s_hexe* ebxphe, x8 ecxb, x8 edxb, x8 argb0)
{
  if (ebxphe != NULL && ddata.s350.v1e058d != con(0x0))
  {
    s_hex20* sptrrg1 = SKW_14cd_10d2(ebxphe, argb0);
    if (sptrrg1->sarr_04[con(0x0)].barr_00[con(0x1)] <= con(0x0))
      ddata.s350.v1e0580 &= con(0xfffffff7);
    if (ddata.s350.v1e0578 != con(0x0))
    {
      if (eaxb != con(0x0))
        ecxb = -ecxb;
      SKW_14cd_18f2(ecxb, ebxphe, con(0x0), edxb, con(0xffff));
    }
  }
}

void SKW_14cd_1a78(x8 eaxb, s_hexe* ebxphe, x8 ecxb, x8 edxb)
{
  if (ebxphe != NULL)
  {
    s_hex20* sptrrg1 = SKW_14cd_10d2(ebxphe, ecxb);
    if (sptrrg1->sarr_04[con(0x0)].barr_00[con(0x3)] != con(0x0))
    {
      s_4bytearray* p4ba_10 = sptrrg1->sarr_04;
      s_hexe vhe_00;
      s_hexe* sheptrrg1;
      do
      {
        if (ebxphe->b_0c == ecxb)
        {
          if (ebxphe->w_04 != con(0xffffffff))
            if (SKW_14cd_1316(ebxphe->b_01, edxb, ebxphe->u_02.w))
            {
              x8 byterg1 = CUTX8(-DM2_MIN(ebxphe->b_08, signedword(p4ba_10->barr_00[con(0x0)])));
              SKW_COPY_MEMORY(DOWNCAST(s_hexe, ebxphe), sizeof(s_hexe), DOWNCAST(s_hexe, &vhe_00));
              if (eaxb != con(0x0))
              {
                vhe_00.b_08 = con(0x0);
                vhe_00.b_09 = con(0x0);
                byterg1 = con(0x0);
              }
              SKW_14cd_0f3c(ebxphe->b_00, &vhe_00, ecxb, ebxphe, byterg1, con(0xffff), edxb, eaxb);
            }
          p4ba_10++;
        }
        sheptrrg1 = ebxphe;
        ebxphe++;
      } while (sheptrrg1->b_0d != con(0x0));
    }
  }
}

void SKW_14cd_1bac(x8 eaxb, s_hexe* ebxphe, x8 ecxb, x8 edxb, x8 argb0)
{
  if ((ebxphe != NULL) && (ddata.s350.v1e058d != con(0x0)))
  {
    s_hex20* sptrrg1 = SKW_14cd_10d2(ebxphe, argb0);
    if ((ddata.s350.v1e0578 & con(0x8)) != con(0x0))
      if (sptrrg1->sarr_04[con(0x0)].barr_00[con(0x1)] <= con(0x0))
        ddata.s350.v1e0580 &= con(0xfffffff7);
    if (ddata.s350.v1e0578 != con(0x0))
    {
      if (eaxb != con(0x0))
        ecxb = -ecxb;
      SKW_14cd_18f2(ecxb, ebxphe, con(0x0), edxb, con(0xffff));
    }
  }
}

void SKW_14cd_1c8d(x8 eaxb, s_hexe* ebxphe, x8 edxb)
{
  if ((eaxb != con(0x0)) && (unsignedword(ddata.s350.v1e0562.u.b.b_06) == (ddata.s350.v1e054e->uc.w & con(0x1f))))
    if (unsignedword(ddata.s350.v1e0562.u.b.b_07) == ulrshift(ddata.s350.v1e054e->uc.w, con(0x6), con(0xb)))
      if (unsignedword(ddata.s350.v1e0571) == urshift(ddata.s350.v1e054e->uc.w, con(0xa)))
        return;
  SKW_14cd_18f2(con(0x6), ebxphe, con(0x0), edxb, con(0xffff));
}

void SKW_14cd_1cec(s_hexe* ebxphe, x8 edxb)
{
  c_record* recoptrrg1 = SKW_GET_MISSILE_REF_OF_MINION(ddata.s350.v1e054c, con(0xffff));
  if (recoptrrg1 != NULL)
    if (urshift(recoptrrg1->u2.w & con(0x3c00), con(0xa)) == con(0x9))
      SKW_14cd_18f2(con(0x7), ebxphe, con(0x0), edxb, SKW_GET_ADDRESS_OF_RECORD(recoptrrg1->u2.w)->u6.w);
}

void SKW_14cd_1eec(x8 eaxb, s_hexe* ebxphe, x8 ecxb, x8 edxb)
{
  if (ebxphe != NULL)
  {
    s_hexe vhe_00;
    s_hexe* sheptrrg4;
    do
    {
      if (ebxphe->b_0c == ecxb && SKW_14cd_1316(ebxphe->b_01, edxb, ebxphe->u_02.w))
      {
        SKW_COPY_MEMORY(DOWNCAST(s_hexe, ebxphe), sizeof(s_hexe), DOWNCAST(s_hexe, &vhe_00));
        vhe_00.w_06 = ddata.s350.v1e054e->w8;
        if (eaxb != con(0x0))
        {
          vhe_00.b_08 = con(0x0);
          vhe_00.b_09 = con(0x0);
        }
        SKW_14cd_0f3c(ebxphe->b_00, &vhe_00, ecxb, ebxphe, con(0x0), con(0xffff), edxb, eaxb);
      }
      sheptrrg4 = ebxphe;
      ebxphe++;
    } while (sheptrrg4->b_0d != con(0x0));
  }
}

// was SKW_14cd_2162
x8 SKW_PROCEED_XACT_75(void)
{
  s_hex20* sptrrg4 = SKW_14cd_10d2(ddata.s350.v1e07d8.phe_a, ddata.s350.v1e07d8.b2);
  x8 byterg1 = ddata.s350.v1e07d8.b3;
  if (byterg1 != con(0x8))
  {
    if (byterg1 != con(0x9))
      byterg1 = con(0x0);
    else
      byterg1 = con(0x3);
  }
  else
    byterg1 = con(0x2);
  ddata.s350.v1e0556->u.v.b1e = CUTX8(ddata.s350.v1e07d8.w4);
  x16 wordrg5 = ddata.s350.v1e0578;
  if ((wordrg5 & con(0x8)) != con(0x0))
    if (sptrrg4->sarr_04[con(0x0)].barr_00[con(0x1)] <= con(0x0))
      ddata.s350.v1e0578 &= con(0xfffffff7);
  ddata.s350.v1e0578 &= ddata.s350.v1e07d8.w6;
  byterg1 |= con(0xffffff80);
  SKW_19f0_0891(byterg1, unsignedword(ddata.s350.v1e0562.u.b.b_07), ddata.s350.v1e0556->u.v.u18.w & con(0x1f), unsignedword(ddata.s350.v1e0562.u.b.b_06), ulrshift(ddata.s350.v1e0556->u.v.u18.w, con(0x6), con(0xb)), con(0xffffffff));
  ddata.s350.v1e0578 = wordrg5;
  return ddata.s350.v1e056f;
}

// was SKW_14cd_22a3
void SKW_PROCEED_XACT_57(void)
{
  x16 vw_00;
  if (SKW_RAND01() == con(0x0))
    vw_00 = con(0xffffffff);
  else
    vw_00 = con(0x1);
  x16 wordrg6 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
  x16 wordrg5 = (vw_00 + wordrg6) & con(0x3);
  if (!SKW_CREATURE_GO_THERE(con(0x80), unsignedword(ddata.s350.v1e0562.u.b.b_07), con(0xffffffff), unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0xffffffff), wordrg5))
    if (!SKW_CREATURE_GO_THERE(con(0x80), unsignedword(ddata.s350.v1e0562.u.b.b_07), con(0xffffffff), unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0xffffffff), (wordrg6 - vw_00) & con(0x3)))
      SKW_19f0_0559(wordrg5);
}

// was SKW_14cd_232b
x8 SKW_PROCEED_XACT_59_76(void)
{
  x16 wordrg3 = ddata.s350.v1e0572;
  if (wordrg3 == con(0xffffffff))
    wordrg3 = ddata.s350.v1e07d8.w4;
  if (ddata.s350.v1e0574 != con(0x0))
    if (SKW_1c9a_0732(wordrg3, con(0xff), ddata.s350.v1e054e->u2.w) != con(0xfffffffe))
      return con(0xfffffffe);
  SKW_19f0_2165(con(0x80), unsignedword(ddata.s350.v1e0562.u.b.b_07), ddata.s350.v1e0556->u.v.u18.w & con(0x1f), unsignedword(ddata.s350.v1e0562.u.b.b_06), ulrshift(ddata.s350.v1e0556->u.v.u18.w, con(0x6), con(0xb)), con(0xffffffff), wordrg3);
  return ddata.s350.v1e056f;
}

// was SKW_14cd_2398
void SKW_PROCEED_XACT_69(void)
{
  x16 wordrg4 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
  x16 wordrg21 = (unsignedword(ddata.s350.v1e0562.u.b.b_06) + table1d27fc[wordrg4]) & con(0x1f);
  ddata.s350.v1e0556->u.v.u18.b.b0 &= con(0xffffffe0);
  x16 wordrg5 = ddata.s350.v1e0556->u.v.u18.w | wordrg21;
  ddata.s350.v1e0556->u.v.u18.w = wordrg5;
  x16 wordrg22 = ((unsignedword(ddata.s350.v1e0562.u.b.b_07) + table1d2804[wordrg4]) & con(0x1f)) << con(0x5);
  ddata.s350.v1e0556->u.v.u18.w = (wordrg5 & con(0xfffffc1f)) | wordrg22;
  x8 byterg4 = CUTX8(ddata.s350.v1e0572);
  ddata.s350.v1e0556->u.v.b1d = byterg4;
  ddata.s350.v1e0556->u.v.b1a = ((byterg4 == con(0x1)) ? 1 : 0) + con(0x15);
}

// was SKW_14cd_240e
x8 SKW_PROCEED_XACT_64(void)
{
  c_record* recoptrrg3 = ddata.s350.v1e054e;
  if (recoptrrg3->u2.w == con(0xfffffffe))
    return con(0xfffffffd);
  x16 wordrg5 = ddata.s350.v1e057c;
  if ((wordrg5 & con(0x8)) == con(0x0))
    return con(0xfffffffd);
  x16 wordrg1 = ddata.s350.v1e0572;
  if (wordrg1 == con(0xffffffff))
    wordrg1 = con(0x3f);
  x16 wordrg3 = wordrg1;
  if (SKW_1c9a_0732(wordrg3, con(0xff), ddata.s350.v1e054e->u2.w) == con(0xfffffffe))
    return con(0xfffffffd);
  ddata.s350.v1e057c &= con(0x8);
  SKW_19f0_2165(con(0x81), unsignedword(ddata.s350.v1e0562.u.b.b_07), con(0xffffffff), unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0xffffffff), ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe)), wordrg3);
  ddata.s350.v1e057c = wordrg5;
  return ddata.s350.v1e056f;
}

// was SKW_14cd_248d
x8 SKW_PROCEED_XACT_70(void)
{
  x8 byterg2 = con(0xfffffffd);
  x16 vw_00 = ddata.s350.v1e0572;
  if (ddata.s350.v1e0572 == con(0xffffffff))
    vw_00 = con(0x3f);
  ddata.s350.v1e0556->u.v.u18.b.b0 &= con(0xffffffe0);
  x16 wordrg6 = ddata.s350.v1e0556->u.v.u18.w | (unsignedword(ddata.s350.v1e0562.u.b.b_06) & con(0x1f));
  ddata.s350.v1e0556->u.v.u18.w = wordrg6;
  ddata.s350.v1e0556->u.v.u18.w = (wordrg6 & con(0xfffffc1f)) | ((unsignedword(ddata.s350.v1e0562.u.b.b_07) & con(0x1f)) << con(0x5));
  ddata.s350.v1e0556->u.v.b1c = CUTX8((ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe)) + con(0x2)) & con(0x3));
  ddata.s350.v1e0556->u.v.b1e = CUTX8(vw_00);
  x16 wordrg42 = ddata.s350.v1e0556->u.v.u18.w & con(0x1f);
  wordrg42 += table1d27fc[ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe))];
  wordrg42 &= con(0x1f);
  ddata.s350.v1e0556->u.v.u18.b.b0 &= con(0xffffffe0);
  ddata.s350.v1e0556->u.v.u18.w |= wordrg42;
  x16 wordrg41 = ulrshift(ddata.s350.v1e0556->u.v.u18.w, con(0x6), con(0xb));
  x16 wordrg5 = (ddata.s350.v1e0556->u.v.u18.w & con(0xfffffc1f)) | (((table1d2804[ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe))] + wordrg41) & con(0x1f)) << con(0x5));
  ddata.s350.v1e0556->u.v.u18.w = wordrg5;
  x16 wordrg1 = SKW_GET_CREATURE_AT(wordrg5 & con(0x1f), ulrshift(wordrg5, con(0x6), con(0xb)));
  if (wordrg1 != con(0xffffffff))
  {
    byterg2 = ddata.s350.v1e0556->u.v.b1c;
    if (SKW_1c9a_0732(vw_00, byterg2, SKW_GET_ADDRESS_OF_RECORD(wordrg1)->u2.w) != con(0xfffffffe))
    {
      ddata.s350.v1e0556->u.v.b1a = con(0x18);
      byterg2 = con(0xfffffffc);
    }
    else
      byterg2 = con(0xfffffffe);
  }
  return byterg2;
}

// was SKW_14cd_25b8
x8 SKW_PROCEED_XACT_63(void)
{
  x8 byterg3 = con(0xfffffffd);
  x16 vw_00 = ddata.s350.v1e0572;
  if (ddata.s350.v1e0572 != con(0xffffffff))
  {
    x16 wordrg42 = unsignedword(ddata.s350.v1e0562.u.b.b_06);
    x16 wordrg5 = unsignedword(ddata.s350.v1e0562.u.b.b_07);
    x8 byterg2 = CUTX8(ddata.s350.v1e0574);
    if (byterg2 != con(0xffffffff))
      byterg2 = CUTX8((unsignedword(byterg2) + ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe)) + con(0x2)) & con(0x3));
    x16 wordrg11 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
    wordrg42 += table1d27fc[wordrg11];
    wordrg5 += table1d2804[wordrg11];
    x16 wordrg41 = SKW_GET_CREATURE_AT(wordrg42, wordrg5);
    if (wordrg41 != con(0xffffffff))
      if (SKW_1c9a_0732(vw_00, byterg2, SKW_GET_ADDRESS_OF_RECORD(wordrg41)->u2.w) != con(0xfffffffe))
        byterg3 = con(0xfffffffe);
  }
  return byterg3;
}

// used as plugin too
bool SKW_14cd_2807(x16* eaxpw, x16* edxpw)
{
  if (SKW_1c9a_0006(*eaxpw, edxpw[con(0x1)]) != con(0x0))
  {
    if (edxpw[con(0x0)] == con(0xffffffff))
      edxpw[con(0x0)] = con(0x0);
    x16 wordrg1;
    if (edxpw[con(0x4)] == con(0x0))
      wordrg1 = con(0xffffffff);
    else
      wordrg1 = SKW_ADD_ITEM_CHARGE(*eaxpw, con(0x0));
    edxpw[con(0x0)] += SKW_48ae_05ae(SKW_GET_DISTINCTIVE_ITEMTYPE(*eaxpw), ddata.s350.v1e054e->w8, edxpw[con(0x2)], ddata.s350.v1e054e->u4.b.b0, edxpw[con(0x3)], wordrg1);
  }
  return false;
}

// was SKW_14cd_2f07
x8 SKW_PROCEED_XACT_68(void)
{
  t_gfxdata vga_00[0x14];

  x8 vb_28 = con(0xfffffffd);
  x16 vw_14 = ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe));
  if (!SKW_14cd_2662((CUTX8(ddata.s350.v1e0572) + con(0x2)) & con(0x3)))
  {
    x16 wordrg13 = SKW_GET_CREATURE_AT(unsignedword(ddata.s350.v1e0562.u.b.b_06) + table1d27fc[vw_14], unsignedword(ddata.s350.v1e0562.u.b.b_07) + table1d2804[vw_14]);
    if (wordrg13 != con(0xffffffff))
    {
      x16* wptrrg5 = &SKW_GET_ADDRESS_OF_RECORD(wordrg13)->u2.w;
      x8 bytev14 = CUTX8(vw_14 + ddata.s350.v1e0572) & con(0x3);
      x8 byterg6 = (bytev14 + con(0x2)) & con(0x3);
      x16 vw_24; // result not needed
      x16 wordrg12 = SKW_48ae_0767(SKW_14cd_2886(wptrrg5, byterg6, con(0x1), con(0x10), con(0x1), con(0x0)), vga_00, &vw_24, con(0x12));
      x16 vw_20 = wordrg12;
      x16 vw_1c = SKW_14cd_2886(wptrrg5, byterg6, con(0x0), con(0x7), con(0x0), con(0x0));
      if (vw_1c == con(0xffffffff))
        vw_1c = con(0x0);
      else
        vw_20 = wordrg12 + vw_1c;
      x16 wordrg6 = SKW_14cd_2886(wptrrg5, bytev14, con(0x0), con(0x10), con(0x0), con(0x0));
      x16 wordrg11 = SKW_14cd_2886(wptrrg5, bytev14, con(0x0), con(0x7), con(0x0), con(0x0));
      if (wordrg6 == con(0xffffffff))
        wordrg6 = con(0x0);
      if (wordrg11 != con(0xffffffff))
        wordrg6 += wordrg11;
      if (wordrg6 >= (vw_1c + ddata.s350.v1e0556->u.v.w0e))
      {
        if (wordrg6 != ddata.s350.v1e0556->u.v.w10)
        {
          ddata.s350.v1e0556->u.v.b1a = con(0x1c);
          ddata.s350.v1e0556->u.v.w10 = wordrg6;
        }
        wordrg6 -= vw_20;
        ddata.s350.v1e0556->u0c.w = DM2_MAX(con(0x0), wordrg6);
        vb_28 = con(0xfffffffe);
      }
      else
        ddata.s350.v1e0556->u.v.b1a = con(0x1b);
    }
  }
  else
  {
    vb_28 = con(0xfffffffd);
    ddata.s350.v1e0556->u.v.b1a = con(0x1f);
  }
  return vb_28;
}

// was SKW_14cd_33e1
x8 SKW_PROCEED_XACT_77(void)
{
  ddata.s350.v1e0674 = con(0x0);
  s_hexe* sheptrrg5 = ddata.s350.v1e07d8.phe_a;
  if (sheptrrg5 != NULL)
  {
    s_hexe* sheptrrg1;
    do
    {
      if (sheptrrg5->b_0c == con(0x11) && sheptrrg5->w_04 != con(0xffffffff))
        SKW_14cd_0f3c(con(0x14), sheptrrg5, sheptrrg5->b_0c, sheptrrg5, con(0x0), con(0xffff), con(0xff), con(0x0));
      sheptrrg1 = sheptrrg5;
      sheptrrg5++;
    } while (sheptrrg1->b_0d != con(0x0));
    if (ddata.s350.v1e0674 != con(0x0))
    {
      ddata.s350.v1e0675 = SKW_1c9a_1bd8(unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0x3), unsignedword(ddata.s350.v1e0562.u.b.b_07), ddata.s350.v1e0674, ddata.s350.v1e0678, ddata.s350.v1e060e);
      if (ddata.s350.v1e0675 != con(0xffffffff))
        return con(0xfffffffe);
    }
  }
  return con(0xfffffffd);
}

// was SKW_14cd_3474
x8 SKW_PROCEED_XACT_78(void)
{
  ddata.s350.v1e056f = con(0xfffffffd);
  if (ddata.v1d3248 == ddata.v1e08d6)
  {
    x16 wordrg5 = SKW_CALC_VECTOR_DIR(unsignedword(ddata.s350.v1e0562.u.b.b_06), signedword(ddata.v1e08d8), ddata.v1e08d4, unsignedword(ddata.s350.v1e0562.u.b.b_07));
    if (((SKW_0cee_04e5(wordrg5, con(0x0), unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0x1), unsignedword(ddata.s350.v1e0562.u.b.b_07)) & con(0xff)) >> con(0x5)) != con(0x0))
      SKW_19f0_0559(wordrg5);
  }
  return ddata.s350.v1e056f;
}

// was SKW_14cd_34d3
x8 SKW_PROCEED_XACT_80(void)
{
  x16 wordrg4 = ddata.s350.v1e0572;
  x8 byterg3 = (wordrg4 != con(0x0)) ? con(0x6) : con(0x0);
  x16 wordrg5 = ddata.s350.v1e0576;
  ddata.s350.v1e0576 = wordrg5 | con(0x1800);
  SKW_CREATURE_GO_THERE(byterg3 | con(0xffffff80), unsignedword(ddata.s350.v1e0562.u.b.b_07), con(0xffffffff), unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0xffffffff), (ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe)) + wordrg4) & con(0x3));
  ddata.s350.v1e0576 = wordrg5;
  return ddata.s350.v1e056f;
}

// was SKW_14cd_38c2
x8 SKW_PROCEED_XACT_83(void)
{
  x8 byterg3 = con(0xfffffffe);
  if ((ddata.s350.v1e054e->ua.w & con(0x80)) != con(0x0) || (ddata.s350.v1e0572 != con(0x0)))
  {
    ddata.s350.v1e0556->u.v.b1a = CUTX8(DM2_BETWEEN_VALUE(con(0x0), con(0x2), ddata.s350.v1e0572)) + con(0x23);
    if ((ddata.s350.v1e054e->ua.w & con(0x80)) != con(0x0))
      if (ddata.s350.v1e0572 == con(0x1))
        byterg3 = con(0xfffffffc);
  }
  else
    byterg3 = con(0xfffffffd);
  return byterg3;
}

// was SKW_14cd_3b69
x8 SKW_PROCEED_XACT_91(void)
{
  if (SKW_1c9a_0732(ddata.s350.v1e0572, con(0xff), ddata.s350.v1e054e->u2.w) != con(0xfffffffe))
    return con(0xfffffffe);
  if (SKW_1c9a_0732(ddata.s350.v1e0574, con(0xff), ddata.s350.v1e054e->u2.w) == con(0xfffffffe))
    return con(0xfffffffd);
  return con(0xfffffffe);
}

bool SKW_CREATURE_WALK_NOW(void)
{
  if ((ddata.s350.v1e0552->u0.b.b1 & con(0x1)) != con(0x0))
  {
    ddata.s350.v1e0556->u.v.b1a = con(0x26);
    ddata.s350.v1e0556->u.v.b1c = con(0x4);
    SKW_CREATURE_ATTACKS_PARTY();
  }
  if (SKW_CREATURE_GO_THERE(ddata.s350.v1e0556->u.v.b20 | con(0xffffff80), unsignedword(ddata.s350.v1e0562.u.b.b_07), ddata.v1e08a6, unsignedword(ddata.s350.v1e0562.u.b.b_06), ddata.v1e08a4, signedword(ddata.s350.v1e0556->u.v.b1b)) && (UI8CAST(table1d613a[ddata.s350.v1e0556->u.v.b1a] & con(0x4)) != con(0x0)))
  {
    x8 byterg6 = ddata.s350.v1e0552->barr06[con(0x3)] & con(0x40);
    if (byterg6 != con(0x0))
      SKW_OPERATE_PIT_TELE_TILE(unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0x1), unsignedword(ddata.s350.v1e0562.u.b.b_07));
    if (SKW_MOVE_RECORD_TO(ddata.s350.v1e054c, unsignedword(ddata.s350.v1e0562.u.b.b_07), ddata.s350.v1e0556->u.v.u18.w & con(0x1f), unsignedword(ddata.s350.v1e0562.u.b.b_06), ulrshift(ddata.s350.v1e0556->u.v.u18.w, con(0x6), con(0xb))) == con(0x0))
    {
      ddata.s350.v1e0562.u.b.b_06 = CUTX8(ddata.v1e102a);
      ddata.s350.v1e0562.u.b.b_07 = CUTX8(ddata.v1e1028);
      ddata.s350.v1e0571 = CUTX8(ddata.v1e102c);
      SKW_1c9a_0648(unsignedword(ddata.s350.v1e0571));
      if (byterg6 != con(0x0))
        SKW_OPERATE_PIT_TELE_TILE(unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0x0), unsignedword(ddata.s350.v1e0562.u.b.b_07));
      if (ddata.s350.v1e07d8.b1 != con(0x0))
        ddata.s350.v1e07d8.b1--;
    }
    else
      ddata.s350.v1e0570 = con(0x1);
    return false;
  }
  return true;
}

void SKW_1887_05aa(void)
{
  x16 wordrg4 = unsignedword(ddata.s350.v1e0556->u.v.b1d);
  if (((ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe)) - wordrg4) & con(0x3)) == con(0x2))
  {
    wordrg4 += CUTX16(SKW_RAND()) & con(0x2);
    wordrg4++;
  }
  wordrg4 &= con(0x3);
  ddata.s350.v1e054e->ue.b.b1 &= con(0xfffffffc);
  wordrg4 <<= con(0x8);
  ddata.s350.v1e054e->ue.w |= wordrg4;
}

bool SKW_1887_0c75(void)
{
  if (SKW_19f0_0d10(UI8CAST(ddata.s350.v1e0556->u.v.b20 | con(0xffffff80)), unsignedword(ddata.s350.v1e0562.u.b.b_07), ddata.v1e08a6, unsignedword(ddata.s350.v1e0562.u.b.b_06), ddata.v1e08a4, ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe))))
    if (ddata.s350.v1e0556->u.v.b1a == con(0xb))
    {
      SKW_INVOKE_MESSAGE(ddata.v1e08a6, con(0x0), con(0x2), ddata.v1e08a4, ddata.longmallocx);
      return false;
    }
  return true;
}

bool SKW_1887_0eca(void)
{
  x16 wordrg5 = SKW_1c9a_0732(signedword(ddata.s350.v1e0556->u.v.b1e), con(0xff), unsignedword(ddata.s350.v1e054e->u2.w));
  if (wordrg5 == con(0xfffffffe))
    return true;
  SKW_CUT_RECORD_FROM(wordrg5, con(0xffffffff), con(0x0), &ddata.s350.v1e054e->u2.w);
  x16 wordrg4 = unsignedword(ddata.s350.v1e0552->barr06[con(0x0)]) / 4 + con(0x1);
  wordrg4 += SKW_RAND16(wordrg4);
  wordrg4 += SKW_RAND16(wordrg4);
  SKW_SHOOT_ITEM(wordrg5, unsignedword(ddata.s350.v1e0562.u.b.b_07), unsignedword(ddata.s350.v1e0556->u.v.b1c), unsignedword(ddata.s350.v1e0562.u.b.b_06), ddata.s350.v1e0556->u.v.b1b, CUTX8(DM2_BETWEEN_VALUE(con(0x14), con(0xff), wordrg4) & con(0xff)), ddata.s350.v1e0552->barr06[con(0x2)], con(0x8));
  return false;
}

void SKW_1887_126d(void)
{
  x16 vw_00 = unsignedword(ddata.s350.v1e0562.u.b.b_06);
  x16 wordrg6 = unsignedword(ddata.s350.v1e0562.u.b.b_07);
  x8 byterg1 = ddata.s350.v1e0556->u.v.b1f;
  x8 vb_04 = byterg1;
  byterg1++;
  ddata.s350.v1e0556->u.v.b1f = byterg1;
  if (ddata.s350.v1e0556->u.v.b1a != con(0x3b))
  {
    SKW_CREATE_CLOUD(con(0xffa8), vw_00, wordrg6, con(0xff), con(0xff));
    SKW_QUEUE_NOISE_GEN1(con(0x3), con(0x81), con(0x6c), con(0x0), con(0xc8), vw_00, wordrg6, con(0x1));
    return;
  }
  x16 wordrg5 = SKW_RAND02() + con(0x1);
  for(;;)
  {
    wordrg5--;
    if (wordrg5 == con(0xffffffff))
    {
      if (vb_04 >= con(0x1))
        SKW_CREATE_CLOUD(con(0xffa8), vw_00, wordrg6, signedword(vb_04) * con(0x55), con(0xff));
      if (vb_04 < con(0x3))
      {
        SKW_QUEUE_NOISE_GEN1(con(0x3), con(0x8b), con(0x6c), con(0x0), con(0xc8), vw_00, wordrg6, con(0x1));
        return;
      }
      ddata.s350.v1e0556->u.v.b1a = con(0x3c);
      ddata.s350.v1e0556->u.v.b1f = con(0x0);
      ddata.s350.v1e054e->ua.w = con(0x0);
      ddata.s350.v1e054e->u4.b.b0 = ddata.s350.v1e0556->u.v.b1e;
      ddata.s350.v1e0552 = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(ddata.s350.v1e0556->u.v.b1e);
      wordrg5 = ddata.s350.v1e0552->w4;
      wordrg5 += SKW_RAND16(wordrg5 / 8 + con(0x1));
      ddata.s350.v1e054e->u6.w = wordrg5;
      SKW_GET_CREATURE_ANIMATION_FRAME(ddata.s350.v1e054e->u4.b.b0, ddata.s350.v1e055e, &ddata.s350.v1e055e[con(0x1)], signedword(ddata.s350.v1e0556->u.v.b1a), &ddata.s350.v1e055a, con(0x0));
      if (!SKW_IS_CREATURE_ALLOWED_ON_LEVEL(ddata.s350.v1e054c, unsignedword(ddata.s350.v1e0571)))
      {
        SKW_DELETE_CREATURE_RECORD(vw_00, con(0x0), con(0x1), wordrg6);
        ddata.s350.v1e0570 = con(0x1);
      }
      SKW_QUEUE_NOISE_GEN1(con(0x3), con(0x81), con(0x6c), con(0x0), con(0xc8), vw_00, wordrg6, con(0x1));
      return;
    }
    x8 byterg1h = CUTX8(SKW_RAND16(con(0x5)));
    if (byterg1h == con(0x4))
      byterg1h = con(0xffffffff);
    SKW_CREATE_CLOUD(con(0xff83), vw_00, wordrg6, SKW_RAND16(con(0x9c)) + con(0x64), unsignedword(byterg1h));
  }
}

bool SKW_1887_141c(void)
{
  x8 byterg4 = ddata.s350.v1e0556->u.v.b20;
  if (byterg4 != con(0x0))
  {
    if (byterg4 != con(0x1))
    {
      if (byterg4 == con(0x2))
      {
        ddata.s350.v1e0556->u.v.b1a = con(0x11);
        ddata.s350.v1e0556->u.v.b1f = con(0x0);
        ddata.s350.v1e054e->u4.b.b0 = ddata.s350.v1e0556->u.v.b1e;
        ddata.s350.v1e0552 = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(ddata.s350.v1e0556->u.v.b1e);
        x16 wordrg42 = ddata.s350.v1e0552->w4;
        wordrg42 += SKW_RAND16(wordrg42 / 8 + con(0x1));
        ddata.s350.v1e054e->u6.w = wordrg42;
        SKW_GET_CREATURE_ANIMATION_FRAME(ddata.s350.v1e054e->u4.b.b0, ddata.s350.v1e055e, &ddata.s350.v1e055e[con(0x1)], signedword(ddata.s350.v1e0556->u.v.b1a), &ddata.s350.v1e055a, con(0x0));
        if (!SKW_IS_CREATURE_ALLOWED_ON_LEVEL(ddata.s350.v1e054c, unsignedword(ddata.s350.v1e0571)))
        {
          SKW_DELETE_CREATURE_RECORD(unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0x0), con(0x1), unsignedword(ddata.s350.v1e0562.u.b.b_07));
          ddata.s350.v1e0570 = con(0x1);
        }
      }
    }
    else if (SKW_CREATE_MINION(signedword(ddata.s350.v1e0556->u.v.b1e), SKW_RAND02(), unsignedword(ddata.s350.v1e0562.u.b.b_06), con(0x7), unsignedword(ddata.s350.v1e0562.u.b.b_07), unsignedword(ddata.s350.v1e0571), con(0xffff), CUTX8(ulrshift(ddata.s350.v1e054e->ue.w, con(0x6), con(0xe)))) == con(0xffffffff))
      return true;
  }
  else
  {
    x16 wordrg41 = unsignedword(ddata.s350.v1e0552->barr06[con(0x0)]) / 4 + con(0x1);
    wordrg41 += SKW_RAND16(wordrg41);
    wordrg41 += SKW_RAND16(wordrg41);
    SKW_CREATE_CLOUD(con(0xff80), unsignedword(ddata.s350.v1e0562.u.b.b_06), unsignedword(ddata.s350.v1e0562.u.b.b_07), DM2_BETWEEN_VALUE(con(0x14), con(0xff), wordrg41), con(0xff));
    if (ddata.s350.v1e0556->u.v.b1e != con(0x0))
      SKW_1887_0df7();
  }
  return false;
}

// adress given to a functionpointer
bool R_1BA1B(x16 eaxw, x16 edxw)
{
  x8 vb_00 = SKW_GET_TILE_VALUE(eaxw, edxw);
  x8 byterg4 = ubrshift(vb_00, con(0x5));
  if (byterg4 != con(0x4))
  {
    if (byterg4 == con(0x0) || byterg4 == con(0x7))
      return true;
    if (byterg4 != con(0x6))
      return false;
    return (vb_00 & con(0x4)) == con(0x0);
  }
  return ((vb_00 & con(0x7)) == byterg4 && SKW_GET_GRAPHICS_FOR_DOOR(SKW_IS_REBIRTH_ALTAR(SKW_GET_ADDRESS_OF_TILE_RECORD(eaxw, edxw))) == con(0x0));
}

void SKW_19f0_045a(x16 eaxw, x16 edxw)
{
  if (eaxw == ddata.v1e08a8 && edxw == ddata.v1e08aa && ddata.v1d3248 == ddata.v1e08ac)
    return;
  ddata.v1e08ac = ddata.v1d3248;
  ddata.v1e08aa = edxw;
  ddata.v1e08a8 = eaxw;
  ddata.v1e08ae = unsignedword(SKW_GET_TILE_VALUE(eaxw, edxw));
  x16 wordrg1 = (((ddata.v1e08ae & con(0x10)) != con(0x0)) ? 1 : 0) + con(0xfffe);
  ddata.v1e08b4 = wordrg1;
  ddata.v1e08b2 = wordrg1;
  ddata.v1e08b0 = wordrg1;
  ddata.v1e08b6 = con(0x0);
  ddata.v1e08b7 = con(0x0);
  ddata.v1e08be = con(0xffffffff);
  ddata.v1e08c4 = con(0x1);
}

x16 SKW_19f0_04bf(void)
{
  if (ddata.v1e08b2 != con(0xffffffff))
    return ddata.v1e08b2;
  x16 wordrg1 = ddata.v1e08b0;
  if (wordrg1 == con(0xffffffff))
  {
    wordrg1 = SKW_GET_TILE_RECORD_LINK(ddata.v1e08a8, ddata.v1e08aa);
    ddata.v1e08b0 = wordrg1;
  }
  for(;;)
  {
    if (wordrg1 == con(0xfffffffe))
      break;
    if (urshift(wordrg1 & con(0x3c00), con(0xa)) > con(0x3))
      break;
    wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg1);
  }
  ddata.v1e08b2 = wordrg1;
  return wordrg1;
}

x16 SKW_19f0_050f(void)
{
  if (ddata.v1e08b4 != con(0xffffffff))
    return ddata.v1e08b4;
  x16 wordrg1 = SKW_19f0_04bf();
  for(;;)
  {
    if (wordrg1 == con(0xfffffffe))
      break;
    if (urshift(wordrg1 & con(0x3c00), con(0xa)) == con(0x4))
      break;
    wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg1);
  }
  ddata.v1e08b4 = wordrg1;
  return wordrg1;
}

void SKW_1c9a_0247(x16 eaxw)
{
  x16 vw_00;
  if (SKW_3e74_5673(UI32CAST(eaxw & con(0x3ff)) | UI32CAST(ddata.s350.v1e054c & con(0x3ff)) | con(0x20000000), con(0x0), &vw_00) != con(0x0))
    SKW_3e74_58a3(vw_00);
  if (SKW_3e74_5673(UI32CAST(eaxw & con(0x3ff)) | UI32CAST(ddata.s350.v1e054c & con(0x3ff)) | con(0x30000000), con(0x0), &vw_00) != con(0x0))
    SKW_3e74_58a3(vw_00);
}

x16 SKW_GET_CREATURE_AT(x16 eaxw, x16 edxw)
{
  x16 wordrg1 = SKW_GET_TILE_RECORD_LINK(eaxw, edxw);
  while (wordrg1 != con(0xfffffffe))
  {
    if (urshift(wordrg1 & con(0x3c00), con(0xa)) == con(0x4))
      return wordrg1;
    wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg1);
  }
  return con(0xffff);
}

// TODO: argument typelengths may be shorter, not checked because routine not referenced at all
bool R_1FE74(x32 eaxl, x32 ebxl, x32 ecxl, x32 edxl, x16 argw0)
{
  x16 wordrg1 = CUTX16(eaxl - ebxl);
  x16 wordrg4 = CUTX16(edxl - ecxl);
  if ((argw0 & con(0x1)) == con(0x0))
  {
    if (wordrg1 != con(0x0))
      return false;
    bool boolrg11;
    if (argw0 != con(0x2))
      boolrg11 = wordrg4 >= con(0x0);
    else
      boolrg11 = wordrg4 <= con(0x0);
    return boolrg11;
  }
  if (wordrg4 != con(0x0))
    return false;
  bool boolrg1;
  if (argw0 != con(0x1))
    boolrg1 = wordrg1 >= con(0x0);
  else
    boolrg1 = wordrg1 <= con(0x0);
  return boolrg1;
}

void SKW_1c9a_0cf7(x16 eaxw, x16 edxw)
{
  c_tim vtim_04;

  x16 wordrg2 = SKW_GET_CREATURE_AT(eaxw, edxw);
  c_record* recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(wordrg2);
  if (ddata.v1e089c[recoptrrg4->u4.b.b1].w02 != con(0xffffffff))
    SKW_1c9a_0db0(wordrg2);
  vtim_04.un.l_00 = (ddata.longmallocx + 1) | (signedlong(ddata.v1d3248) << con(0x18));
  vtim_04.b_04 = ((recoptrrg4->w8 != con(0xffffffff)) ? 1 : 0) + con(0x21);
  vtim_04.b_05 = recoptrrg4->u4.b.b0;
  vtim_04.u.b.b_06 = CUTX8(eaxw);
  vtim_04.u.b.b_07 = CUTX8(edxw);
  ddata.v1e089c[recoptrrg4->u4.b.b1].w02 = SKW_QUEUE_TIMER(&vtim_04);
}

void SKW_1c9a_0db0(x16 eaxw)
{
  if (urshift(eaxw & con(0x3c00), con(0xa)) == con(0x4))
  {
    x8 byterg4 = SKW_GET_ADDRESS_OF_RECORD(eaxw)->u4.b.b1;
    if (byterg4 != con(0xffffffff))
      if (ddata.v1e089c[byterg4].w02 >= con(0x0))
      {
        SKW_DELETE_TIMER(ddata.v1e089c[byterg4].w02);
        ddata.v1e089c[byterg4].w02 = con(0xffffffff);
      }
  }
}

// TODO: make argument unsigned
void SKW_1c9a_0fcb(x16 eaxw)
{
  if (eaxw <= ddata.v1e08a0)
  {
    s_d556* sd5ptrrg4 = &ddata.v1e089c[eaxw];
    if (sd5ptrrg4->w00 >= con(0x0))
    {
      s_d556* sd5ptrrg2 = sd5ptrrg4;
      x16 wordrg5 = sd5ptrrg4->w00 | con(0x1000);
      c_record* recoptrrg6 = SKW_GET_ADDRESS_OF_RECORD(wordrg5);
      bool boolrg3;
      if ((SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg5) & con(0x1)) != con(0x0) || sd5ptrrg4->u.v.b1a != con(0x13))
        boolrg3 = false;
      else
        boolrg3 = true;
      x16 vw_00;
      x16 vw_04;
      if (boolrg3)
      {
        if (sd5ptrrg2->w02 != con(0xffffffff))
        {
          c_tim* timptrrg1 = &ddata.savegametimarrp[sd5ptrrg2->w02];
          vw_00 = unsignedword(timptrrg1->u.b.b_06);
          vw_04 = unsignedword(timptrrg1->u.b.b_07);
        }
        else
          boolrg3 = false;
      }
      sd5ptrrg2->u.v.b1a = con(0x0);
      SKW_1c9a_0db0(wordrg5);
      ddata.v1d4020--;
      recoptrrg6->u4.b.b1 = con(0xffffffff);
      sd5ptrrg2->w00 = con(0xffffffff);
      if (boolrg3)
        SKW_DELETE_CREATURE_RECORD(vw_00, con(0x0), con(0x1), vw_04);
    }
  }
}

void SKW_DELETE_CREATURE_RECORD(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  x16 vw_14 = SKW_GET_CREATURE_AT(eaxw, edxw);
  if (vw_14 != con(0xffffffff))
  {
    c_record* recoptrrg6 = SKW_GET_ADDRESS_OF_RECORD(vw_14);
    if ((SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(recoptrrg6->u4.b.b0)->u0.b.b0 & con(0x1)) == con(0x0))
    {
      if ((table1d607e[SKW_QUERY_GDAT_CREATURE_WORD_VALUE(recoptrrg6->u4.b.b0, con(0x1))].u.b[0] & con(0x4)) == con(0x0))
      {
        x16 vw_10 = ddata.v1d3248;
        SKW_CHANGE_CURRENT_MAP_TO(urshift(recoptrrg6->uc.w, con(0xa)));
        x16 wordrg4 = ulrshift(recoptrrg6->uc.w, con(0x6), con(0xb));
        SKW_INVOKE_MESSAGE(recoptrrg6->uc.w & con(0x1f), con(0x0), con(0x0), wordrg4, ddata.longmallocx + con(0x1));
        SKW_CHANGE_CURRENT_MAP_TO(vw_10);
      }
      if (recoptrrg6->u4.b.b1 != con(0xffffffff))
        ddata.v1e089c[recoptrrg6->u4.b.b1].u.v.b1a = con(0x0);
    }
    SKW_MOVE_RECORD_TO(vw_14, edxw, con(0xfffffffc), eaxw, con(0x0));
    SKW_DROP_CREATURE_POSSESSION(vw_14, edxw, ebxw, eaxw, ecxw);
    SKW_1c9a_0247(vw_14);
    SKW_DEALLOC_RECORD(vw_14);
  }
}

bool SKW_1c9a_17c7(x16 eaxw, x16 ebxw, x16 edxw)
{
  if (ebxw != ddata.v1e08d6 || ddata.v1e0238 || ddata.v1e0976 != con(0x0))
    return false;
  if (DM2_ABS(ddata.v1e08d8 - eaxw) == DM2_ABS(ddata.v1e08d4 - edxw))
    return false;
  if (SKW_CALC_VECTOR_DIR(ddata.v1e08d8, eaxw, edxw, ddata.v1e08d4) != ddata.v1e08da)
    return false;
  if (DM2_ABS(ddata.v1e08d8 - eaxw) > con(0x2))
    return false;
  if (DM2_ABS(ddata.v1e08d4 - edxw) > con(0x2))
    return false;
  return true;
}

// was SKW_1c9a_198e
x16 SKW_APPLY_CREATURE_POISON_RESISTANCE(x16 eaxw, x16 edxw)
{
  if (edxw != con(0x0))
  {
    x16 wordrg2 = urshift(SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(eaxw)->u18.w, con(0x8)) & con(0xf);
    if (wordrg2 != con(0xf))
    {
      wordrg2 += con(0x2);
      return ((edxw + SKW_RAND02()) << con(0x3)) / wordrg2;
    }
  }
  return con(0x0);
}

void SKW_1c9a_19d4(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  ui16 uwordrg3 = ecxw & con(0x7fff);
  if (uwordrg3 > con(0x5) && uwordrg3 <= con(0x15))
  {
    x16 wordrg1 = uwordrg3 - con(0x6);
    if ((ecxw & con(0x8000)) != con(0x0))
      wordrg1 |= con(0x8000);
    SKW_ATTACK_CREATURE(eaxw, ebxw, wordrg1, edxw, con(0x64), con(0x0));
  }
}

// was SKW_1c9a_37be
void SKW___SET_CURRENT_THINKING_CREATURE_WALK_PATH(void)
{
  ddata.s350.v1e07e6 = NULL;
  if (ddata.s350.v1e0556 != NULL && ddata.s350.v1e07d8.b0 != con(0x0))
  {
    x16 vw_00;
    if (SKW_3e74_5673(UI32CAST(ddata.s350.v1e054c & con(0x3ff)) | con(0x30000000), con(0x0), &vw_00) != con(0x0))
      ddata.s350.v1e07e6 = UPCAST(x16, SKW_3e74_5817(vw_00)); // TODO: special
    else
    {
      ddata.s350.v1e07d8.b1 = con(0x0);
      ddata.s350.v1e07d8.b0 = con(0x0);
    }
  }
}

x16 SKW_1c9a_381c(void)
{
  x16 wordrg4 = con(0x0);
  SKW___SET_CURRENT_THINKING_CREATURE_WALK_PATH();
  if (ddata.s350.v1e07d8.b0 != con(0x0) && ddata.s350.v1e07d8.b1 != con(0x0))
  {
    wordrg4 = unsignedword(ddata.s350.v1e07d8.b1);
    ddata.s350.v1e0556->u.v.b1b = CUTX8(ddata.s350.v1e07e6[unsignedword(ddata.s350.v1e07d8.b0) - wordrg4]) & con(0x7);
  }
  else
  {
    if (unsignedword(ddata.s350.v1e0571) == urshift(ddata.s350.v1e07d8.w8, con(0xa)))
      if (unsignedword(ddata.s350.v1e0562.u.b.b_06) == (ddata.s350.v1e07d8.w8 & con(0x1f)))
        if (unsignedword(ddata.s350.v1e0562.u.b.b_07) == ulrshift(ddata.s350.v1e07d8.w8, con(0x6), con(0xb)))
          ddata.s350.v1e07d8.w8 = con(0xffffffff);
  }
  return wordrg4;
}

void SKW_FILL_ORPHAN_CAII(void)
{
  x16 wordrg3 = ddata.v1d3248;
  x16 wordrg2 = unsignedword(CUTX8(ddata.savegamep4->w0[2]));
  for (x16 wordrg4 = con(0x0); wordrg4 < wordrg2; wordrg4++)
  {
    SKW_CHANGE_CURRENT_MAP_TO(wordrg4);
    SKW_FILL_CAII_CUR_MAP();
  }
  SKW_CHANGE_CURRENT_MAP_TO(wordrg3);
}

void SKW_1c9a_3c30(void)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("SKW_1c9a_3c30");
  ddata.v1e08a0 = con(0x0);
  c_record* recoptrrg4 = ddata.savegameparr5[4];
  x16 wordrg2 = ddata.savegamep4->w0[con(0xa)];
  for(;;)
  {
    if (--wordrg2 == con(0xffffffff))
    {
      x16 wordrg1 = DM2_MIN(ddata.v1e08a0 + con(0x64), ddata.savegamep4->w0[con(0xa)]);
      ddata.v1e08a0 = wordrg1;
      ddata.v1e089c = UPCAST(s_d556, DM2_ALLOC_FREEPOOL_MEMORY(34 * unsignedlong(wordrg1)));
      ddata.v1e08cc = UPCAST(s_32bytes, DM2_ALLOC_FREEPOOL_MEMORY(con(0x400)));
      ddata.v1e08c8 = UPCAST(s_32bytes, DM2_ALLOC_FREEPOOL_MEMORY(con(0x400)));
SPX_DEBUG_POP;
      return;
    }
    if (recoptrrg4->w0 != con(0xffffffff))
      if ((SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(recoptrrg4->u4.b.b0)->u0.b.b0 & con(0x1)) == con(0x0))
        ddata.v1e08a0++;
    recoptrrg4++;
  }
SPX_DEBUG_POP;
}

bool SKW_2066_1f37(x16 eaxw, x16* ebxpw, x16 edxw)
{
  x16 wordrg3 = eaxw;
  bool boolrg5 = false;
  for(;;)
  {
    x16 wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg3);
    wordrg3 = wordrg1;
    if (wordrg1 == con(0xfffffffe))
      break;
    if (urshift(wordrg1 & con(0x3c00), con(0xa)) == con(0x3))
    {
      c_record* recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg1);
      if ((recoptrrg1->u2.w & con(0x7f)) == con(0x27))
      {
        boolrg5 = true;
        if ((recoptrrg1->u2.w & con(0xffffff80)) == con(0x0))
        {
          x16 wordrg21 = ((edxw + con(0x1)) & con(0x1ff)) << con(0x7);
          recoptrrg1->u2.w = (recoptrrg1->u2.w & con(0x7f)) | wordrg21;
          *ebxpw += con(0x1);
        }
      }
    }
  }
  return boolrg5;
}

// was SKW_2066_3697
void SKW__OPEN_DIALOG_PANEL(x8 eaxb, x16 edxw)
{
  t_text t_00[0x50];
  c_rect rc_50;

  ddata.savegamep1 = UPCAST(c_hex2a, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0x1a4)));
  t_text* tp_58 = SKW_QUERY_GDAT_TEXT(con(0x1a), con(0x0), t_00, eaxb);
  t_text* tptrrg2 = SKW_QUERY_GDAT_TEXT(con(0x1a), con(0x1), &t_00[con(0x28)], eaxb);
  t_gfxdata* gptrrg4 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x1a), con(0x0), eaxb);
  t_palette* gptrrg3;
  if (!dm2_dballochandler.gfxalloc_done)
    gptrrg3 = &gptrrg4[SKW_CALC_IMAGE_BYTE_LENGTH(gptrrg4)];
  else
    gptrrg3 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(con(0x1a), con(0x0), eaxb);
//                             srcbmp                     palette
  SKW_DRAW_DIALOGUE_PARTS_PICT(gptrrg4, con(0xffffffff), gptrrg3, SKW_QUERY_EXPANDED_RECT(con(0x4), &rc_50));
  SKW_FREE_PICT_ENTRY(gptrrg4);
  SKW_FIRE_FADE_SCREEN(true);
  SKW_DRAW_VP_RC_STR(con(0x1c2), v1d10eb, unsignedword(ddata.paletteptr1[con(0xc)]));
  SKW_DRAW_VP_RC_STR(con(0x1d2), tp_58, unsignedword(ddata.paletteptr1[con(0xb)]));
  SKW_DRAW_VP_RC_STR(con(0x1d3), tptrrg2, unsignedword(ddata.paletteptr1[con(0xb)]));
  SKW_2066_38d9();
  ddata.v1e0204 = edxw;
  SKW_1031_0675(con(0x4));
  ddata.v1e092a = con(0x0);
  while (ddata.mouse_visibility > con(0x0))
  {
    SKW_FIRE_SHOW_MOUSE_CURSOR();
    ddata.v1e092a++;
  }
}

void SKW_2066_3820(t_text* eaxt, bool edxbool)
{
  c_rect rc_00;

  SKW_QUERY_EXPANDED_RECT(con(0x1c5), &rc_00);
//                                   srcbmp                                                       srcofs     destbmp                                            palette
  SKW_DRAW_DIALOGUE_PICT(SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x1a), con(0x0), con(0x81)), &rc_00, rc_00.x, ddata.bitmapptr, rc_00.y, con(0xffff), SKW_QUERY_GDAT_IMAGE_LOCALPAL(con(0x1a), con(0x0), con(0x81)));
  x16 wordrg2 = unsignedword(ddata.paletteptr1[con(0xf)]);
  SKW_DRAW_VP_STR(rc_00.x, wordrg2, eaxt, rc_00.y + con(0x4));
  if (edxbool)
  {
    x16 wordrg4 = C_strmetrics.charwidth;
    rc_00.x += SKW_SK_STRLEN(eaxt) * wordrg4;
    rc_00.w = wordrg4;
    rc_00.h -= con(0x4);
    SKW_FIRE_FILL_BACKBUFF_RECT(&rc_00, ddata.paletteptr1[con(0xb)]);
  }
  SKW_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
}

void SKW_2066_398a(x16 eaxw)
{
  c_rect rc_04;

  SKW_QUERY_EXPANDED_RECT(con(0x1c3), &rc_04);
  x16 wordrg6 = rc_04.y + con(0x4);
  for (x16 wordrg5 = con(0x0); wordrg5 < con(0xa); wordrg5++)
  {
    x16 wordrg2;
    if (wordrg5 != eaxw)
      wordrg2 = unsignedword(ddata.paletteptr1[con(0xb)]);
    else
      wordrg2 = unsignedword(ddata.paletteptr1[con(0xf)]);
    SKW_DRAW_VP_STR(rc_04.x, wordrg2, ddata.savegamep1[wordrg5].s_02.t_00, wordrg6);
    wordrg6 += ddata.strxplus;
  }
}

// was SKW_2405_005e
t_text* SKW_GET_ITEM_NAME(x16 eaxw)
{
  x8 byterg2 = SKW_QUERY_CLS1_FROM_RECORD(eaxw);
  x8 byterg1 = SKW_QUERY_CLS2_FROM_RECORD(eaxw);
  ddata.v1e098c = con(0xffffffff);
  if (byterg2 == con(0x15) && byterg1 == con(0x0))
  {
    x16 wordrg1 = urshift(SKW_GET_ADDRESS_OF_RECORD(eaxw)->u2.w, con(0xe));
    if (wordrg1 >= con(0x0) && wordrg1 < ddata.savegamew2)
      ddata.v1e098c = wordrg1;
  }
  return SKW_QUERY_GDAT_ITEM_NAME(byterg2, byterg1);
}

// was SKW_2405_02e8
// TODO: make ebxb bool?
void SKW_DRAW_ITEM_ICON(x16 eaxw, x8 ebxb, bool ecxbool, x16 edxw, bool argbool0)
{
  c_rect rc_00;

  x8 vb_10;
  x8 vb_14;
  x8 vb_18;
  if (eaxw != con(0xffffffff))
  {
    x16 wordrg3 = eaxw;
    vb_14 = SKW_QUERY_CLS1_FROM_RECORD(wordrg3);
    vb_10 = SKW_QUERY_CLS2_FROM_RECORD(wordrg3);
    x16 wordrg1;
    if (edxw >= con(0x8))
      wordrg1 = edxw - con(0x8);
    else
      wordrg1 = edxw & con(0x1);
    vb_18 = SKW_2405_014a(wordrg3, true, wordrg1);
  }
  else
  {
    vb_14 = con(0x7);
    vb_10 = con(0x0);
    vb_18 = ddata.v1d653c[edxw].b2 + ebxb;
  }
  x16 vw_0c = ddata.v1d653c[edxw].w0;
  if (edxw < con(0x26))
  {
    SKW_QUERY_EXPANDED_RECT(vw_0c, &rc_00);
    if (edxw >= con(0x8))
//                                  srcbmp                                                           srcofs    destbmp                                         palette
      SKW_DRAW_DIALOGUE_PICT(SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x7), con(0x0), con(0x0)), &rc_00, rc_00.x, ddata.bitmapptr, rc_00.y, con(0xffff), SKW_QUERY_GDAT_IMAGE_LOCALPAL(con(0x7), con(0x0), con(0x0)));
    else
//                                  srcbmp                                                                                                                                                                                              palette
      SKW_DRAW_ICON_PICT_BUFF(SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x1), con(0x0), con(0x2)), &rc_00, rc_00.x - ddata.v1d69d4.s_00.rc_02.x, &ddata.v1d69d4, rc_00.y - ddata.v1d69d4.s_00.rc_02.y, con(0xffff), con(0x0), SKW_QUERY_GDAT_IMAGE_LOCALPAL(con(0x1), con(0x0), con(0x2)));
  }
  if (argbool0 && edxw < con(0xe))
  {
    SKW_2405_011f(vw_0c, &rc_00);
    x8 byterg1;
    if (!ecxbool)
    {
      if (ebxb == con(0x0))
        byterg1 = con(0x4);
      else
        byterg1 = con(0x5);
    }
    else
      byterg1 = con(0x6);
    t_gfxdata* gptrrg6 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x1), byterg1, con(0x2));
    t_gfxdata* gptrrg1 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(con(0x1), byterg1, con(0x2));
    if (edxw >= con(0x8))
//                                 srcbmp              palette
      SKW_DRAW_DIALOGUE_PARTS_PICT(gptrrg6, con(0xc), gptrrg1, &rc_00);
    else
//                             srcbmp                                                                  palette
      SKW_DRAW_ICON_PICT_BUFF(gptrrg6, &rc_00, con(0x0), &ddata.v1d69d4, con(0x0), con(0xc), con(0x0), gptrrg1);
  }
  if (vb_18 != con(0xffffffff))
  {
    if (edxw >= con(0x8))
      SKW_DRAW_STATIC_PIC(vb_14, vb_18, vw_0c, vb_10, con(0xc));
    else
      SKW_DRAW_ICON_PICT_ENTRY(vb_14, vb_18, &ddata.v1d69d4, unsignedword(vb_10), vw_0c, con(0xc));
  }
}

// id: 0x287A6
// was SKW_2481_007d, SKW_MENU_SCREEN
void SKW_SHOW_MENU_SCREEN(void)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("SKW_SHOW_MENU_SCREEN");
  ddata.v1e0950 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x5), con(0x1), con(0x0));		// Credit screen (tombstone)
  if (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x5), con(0x7), con(0x4), con(0x0)) == con(0xffffffff))	// Main screen is raw form (0x07), PCDOS does not have it, but rather a localized image
  {
    ddata.v1e0954 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x5), con(0x4), con(0x0));
    s_gfxdatatail* ptrbeforerg11 = getprevious(ddata.v1e0954);
    //if (ptrbeforerg11->b0 != con(0x8))
	if (ptrbeforerg11 != NULL && ptrbeforerg11->b0 != con(0x8))	// SPX : add mem check
    {
      x16 wordrg41;
      if (ptrbeforerg11->b0 != con(0x4))
        wordrg41 = ptrbeforerg11->width;
      else
        wordrg41 = ((ptrbeforerg11->width + con(0x1)) & con(0xfffffffe)) >> con(0x1);
      t_gfxdata* gptrrg11 = ddata.v1e0954;
      s_gfxdatatail* ptrbeforerg12 = getprevious(ddata.v1e0954);
      wordrg41 *= ptrbeforerg12->height;
      gptrrg11 += wordrg41;
      ddata.v1d66f4 = gptrrg11;
    }
  }
  else
  {
    ddata.v1d66f8 = true;
    ddata.v1e0954 = UPCAST(t_gfxdata, SKW_QUERY_GDAT_ENTRY_DATA_PTR(con(0x5), con(0x7), con(0x4), con(0x0)));
  }
  s_gfxdatatail* ptrbeforerg13 = getprevious(ddata.v1e0950);
  x16 wordrg4;
  //if (ptrbeforerg13->b0 != con(0x8))
  if (ptrbeforerg13 != NULL && ptrbeforerg13->b0 != con(0x8)) 	// SPX : add mem check
  {
    if (ptrbeforerg13->b0 != con(0x4))
      wordrg4 = ptrbeforerg13->width;
    else
      wordrg4 = ((ptrbeforerg13->width + con(0x1)) & con(0xfffffffe)) / 2;
    t_gfxdata* gptrrg12 = ddata.v1e0950;
    s_gfxdatatail* ptrbeforerg14 = getprevious(ddata.v1e0950);
    wordrg4 *= ptrbeforerg14->height;
    gptrrg12 += wordrg4;
    ddata.v1e0958 = gptrrg12;
  }
  else
    ddata.v1e0958 = NULL;
  DM2_GRAPHICS_DATA_CLOSE();
  SKW_1031_0541(con(0x0));
  do
  {
    SKW_2481_0002();	// seems to be the one to draw title image
    SKW_FIRE_SHOW_MOUSE_CURSOR();
    SKW_1031_098e();
    ddata.savegamew8 = con(0x63);
    do
    {
      SKW_EVENT_LOOP();
    } while (ddata.savegamew8 == con(0x63));
  } while (ddata.savegamew8 == con(0xda));
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  if (!ddata.v1d66f8)
    SKW_FREE_PICT_ENTRY(ddata.v1e0954);
  else
    DM2_DEALLOC_BIGPOOL_STRUCT_BEFORE(UPCAST(s_malloctail, DOWNCAST(t_gfxdata, ddata.v1e0954)));
  SKW_FREE_PICT_ENTRY(ddata.v1e0950);
SPX_DEBUG_POP;
}

// was SKW_24a5_069b
void SKW_BURN_PLAYER_LIGHTING_ITEMS(void)
{
  bool boolrg6 = false;
  x16 vw_00 = ddata.savegamew2;
  if (ddata.v1e0288 != con(0x0))
    vw_00 -= con(0x1);
  c_hero* heroptrrg5 = ddata.hero;
  for(;;)
  {
    if (--vw_00 == con(0xffffffff))
    {
      if (boolrg6)
        SKW_RECALC_LIGHT_LEVEL();
      return;
    }
    x16 wordrg3 = con(0x2);
    for(;;)
    {
      if (--wordrg3 == con(0xffffffff))
        break;
      x16 wordrg2 = heroptrrg5->warrc3[wordrg3];
      if (((SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(wordrg2, con(0x0)) & con(0x10)) != con(0x0)) && SKW_ADD_ITEM_CHARGE(wordrg2, con(0x0)) != con(0x0))
      {
        if (SKW_ADD_ITEM_CHARGE(wordrg2, con(0xffffffff)) == con(0x0))
          SKW_SET_ITEM_IMPORTANCE(wordrg2, con(0x0));
        boolrg6 = true;
      }
    }
    heroptrrg5++;
  }
}

void SKW_24a5_0732(x16 eaxw, t_text* ebxt, x16 edxw)
{
  t_text t_00[0xcc];

  x16 vw_d0;
  x16 vw_cc = con(0x0);
  if (C_strmetrics.SKW_QUERY_STR_METRICS(ebxt, OO vw_cc, OO vw_d0))
  {
    x16 wordrg2;
    t_text textrg1;
    for (x16 wordrg4 = con(0x0); ; wordrg4++)
    {
      wordrg2 = wordrg4;
      textrg1 = ebxt[wordrg2];
      if (textrg1 == con(0x0))
        break;
      wordrg2 = unsignedword(textrg1);
      if (wordrg2 < con(0x41) || wordrg2 > con(0x5a))
      {
        wordrg2 = unsignedword(textrg1);
        if (wordrg2 >= con(0x7b))
          textrg1 -= con(0x60);
      }
      else
        textrg1 -= con(0x40);
      wordrg2 = wordrg4;
      t_00[wordrg2] = textrg1;
    }
    t_00[wordrg2] = textrg1;
    SKW_DRAW_VP_STR(eaxw - (vw_d0 >> con(0x1)), unsignedword(ddata.paletteptr1[con(0x0)]), t_00, edxw);
  }
}

void SKW_DRAW_SCROLL_TEXT(x16 eaxw)
{
  t_text t_00[0xc8];
  t_text t_c8[0xc8];
  c_rect rc_190;

  t_text* tptrrg5 = t_c8;
  SKW_QUERY_MESSAGE_TEXT(t_c8, con(0xffff8002), eaxw);
  ddata.v1d66fc = con(0x5);
  SKW_DRAW_STATIC_PIC(con(0x7), con(0x1), con(0x1ee), con(0x0), con(0xffffffff));
  SKW_DRAW_STATIC_PIC(con(0x12), con(0x10), con(0x1ee), con(0x0), con(0xc));
  x16 wordrg3 = con(0x0);
  x16 wordrg1;
  for(;;)
  {
    wordrg1 = wordrg3 + con(0x1);
    if (*tptrrg5 == NULLBYTE)
      break;
    if (*tptrrg5 == con(0xa))
      wordrg3 = wordrg1;
    tptrrg5++;
  }
  t_text textrg4 = *(tptrrg5 - con(0x1));
  if (textrg4 == con(0xa))
  {
    if (*(tptrrg5 - con(0x2)) == textrg4)
      wordrg3--;
  }
  else
    wordrg3 = wordrg1;
  x16 vw_19c; // y0
  x16 vw_1a0; // x0
  SKW_QUERY_TOPLEFT_OF_RECT(con(0x230), &vw_1a0, &vw_19c);
  SKW_QUERY_EXPANDED_RECT(con(0x1ee), &rc_190);
  x16 wordrg5 = ddata.strxplus;
  vw_19c -= (wordrg3 * wordrg5 - C_strmetrics.yofs - con(0x1)) / 2 - C_strmetrics.height;
  x16 vw_198 = con(0x0);
  for(;;)
  {
    if (t_c8[vw_198] == con(0x0))
      return;
    SKW_3929_04e2(t_c8, &vw_198, rc_190.w, t_00);
    SKW_24a5_0732(vw_1a0, t_00, vw_19c);
    vw_19c += wordrg5;
    if (t_c8[vw_198] == con(0xa))
      vw_198++;
  }
}

// was SKW_24a5_0990
void SKW__CHECK_ROOM_FOR_CONTAINER(x16 eaxw, c_record* edxprec)
{
  if (eaxw != con(0xffffffff) && eaxw != ddata.v1d6700)
  {
    x16 vw_04 = con(0x0);
    x16 wordrg6 = con(0x0);
    while ((edxprec->u2.w != con(0xfffffffe)) && ++vw_04 <= con(0x8))
    {
      ddata.v1e095c[wordrg6] = edxprec->u2.w;
      wordrg6++;
      SKW_CUT_RECORD_FROM(edxprec->u2.w, con(0xffffffff), con(0x0), &edxprec->u2.w);
    }
    for(;;)
    {
      if (wordrg6 >= con(0x8))
        return;
      ddata.v1e095c[wordrg6] = con(0xffff);
      wordrg6++;
    }
    ddata.v1d6700 = eaxw;
  }
}

// SKW_24a5_0a06
void SKW_MONEY_BOX_SURVEY(x16 eaxw)
{
  x16 vwa_04[0xa];
  t_text t_18[0xc];

  x32 longrg7 = con(0x0);
  SKW_COUNT_BY_COIN_TYPES(eaxw, vwa_04);
  x16 wordrg3 = con(0x26a);
  for (x16 wordrg5 = con(0x0); wordrg5 < con(0xa); wordrg5++)
  {
    x16 wordrg1 = SKW_GET_ITEM_ORDER_IN_CONTAINER(eaxw, wordrg5);
    if (wordrg1 >= con(0x0))
    {
      x32 longrg2 = signedlong(vwa_04[wordrg1]);
      longrg7 += signedlong(ddata.v1e03ac[wordrg1]) * longrg2;
      SKW_DRAW_VP_RC_STR(wordrg3++, SKW_SK_LTOA10(longrg2, t_18), unsignedword(ddata.paletteptr1[con(0xd)]));
    }
  }
  SKW_DRAW_VP_RC_STR(con(0x239), SKW_SK_LTOA10(longrg7, t_18), unsignedword(ddata.paletteptr1[con(0xd)]));
}

void SKW_24a5_0e82(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, bool argbool1)
{
  c_rect rc_00;
  c_rect rc_08;
  c_rect rc_10;

  x16 wordrg6;
  if (eaxw >= con(0xfffffe00))
  {
    if (eaxw >= con(0x0))
      wordrg6 = ebxw;
    else
      wordrg6 = con(0xb);
  }
  else
    wordrg6 = con(0x8);
  if (SKW_SCALE_RECT(edxw, (eaxw - ecxw) * con(0x2710) / (argw0 - ecxw), con(0x2710), &rc_08) != NULL)
  {
    if (!argbool1)
      SKW_COPY_RECT(&rc_08, &rc_10);
    else
    {
      SKW_QUERY_EXPANDED_RECT(edxw, &rc_00);
      SKW_COPY_RECT(&rc_00, &rc_10);
      if ((rc_08.x + rc_08.w - con(0x1)) != (rc_00.x + rc_00.w - con(0x1)))
      {
        rc_00.w = rc_00.x + rc_00.w - (rc_08.x + rc_08.w);
        rc_00.x = rc_08.x + rc_08.w;
      }
      else
        argbool1 = false;
    }
    rc_10.x += ddata.v1d2748;
    rc_10.y += ddata.v1d2748;
    SKW_FIRE_FILL_BACKBUFF_RECT(&rc_10, ddata.paletteptr1[con(0x0)]);
    SKW_FIRE_FILL_BACKBUFF_RECT(&rc_08, ddata.paletteptr1[wordrg6]);
    if (argbool1)
      SKW_FIRE_FILL_BACKBUFF_RECT(&rc_00, ddata.paletteptr1[con(0x1)]);
  }
}

void SKW_24a5_1532(void)
{
  t_text t_00[0x80];

  x16 vw_80; // x0 rectB
  x16 vw_84; // y0 rectB, y0 rectA
  x16 vw_8c; // x0 rectA
  x16 vw_98 = ddata.v1e0976 - con(0x1);
  c_hero* heroptrrg6 = &ddata.hero[ddata.v1e0976 - con(0x1)];
  ddata.v1d66fc = con(0x2);
  SKW_DRAW_STATIC_PIC(con(0x7), con(0x1), con(0x1ee), con(0x0), con(0xffffffff));
  SKW_QUERY_TOPLEFT_OF_RECT(con(0x22d), &vw_8c, &vw_84);
  x16 vw_90 = con(0x0);
  for(;;)
  {
    x16 wordrg5 = vw_90;
    if (wordrg5 > con(0x3))
    {
      SKW_QUERY_TOPLEFT_OF_RECT(con(0x22f), &vw_80, &vw_84);
      vw_90 = con(0x1);
      for(;;)
      {
        wordrg5 = vw_90;
        if (wordrg5 > con(0x6))
          return;
        SKW_DRAW_VP_STR(vw_8c, unsignedword(ddata.paletteptr1[con(0xd)]), SKW_QUERY_GDAT_TEXT(con(0x7), CUTX8(vw_90 + con(0x20)), t_00, con(0x0)), vw_84);
        x16 wordrg3 = SKW_GET_PLAYER_ABILITY(heroptrrg6, con(0x0), wordrg5);
        x16 vw_94 = wordrg3;
        x16 vw_88 = SKW_GET_PLAYER_ABILITY(heroptrrg6, con(0x1), wordrg5);
        if (wordrg3 >= vw_88)
        {
          if (wordrg3 <= vw_88)
            wordrg3 = con(0xd);
          else
            wordrg3 = con(0x7);
        }
        else
          wordrg3 = con(0x8);
        SKW_DRAW_VP_STR(vw_80, unsignedword(ddata.paletteptr1[wordrg3]), SKW_FMT_NUM(vw_94, con(0x3), con(0x1)), vw_84);
        SKW_SK_STRCPY(t_00, v1d10f2);
        SKW_SK_STRCAT(t_00, SKW_FMT_NUM(vw_88, con(0x3), con(0x1)));
        SKW_DRAW_VP_STR(C_strmetrics.charwidth * con(0x3) + vw_80, unsignedword(ddata.paletteptr1[con(0xd)]), t_00, vw_84);
        vw_84 += ddata.strxplus;
        vw_90++;
      }
    }
    x16 wordrg12 = DM2_MIN(con(0x10), SKW_QUERY_PLAYER_SKILL_LV(vw_98, true, wordrg5));
    if (wordrg12 != con(0x1))
    {
      ddata.v1e0994 = wordrg5;
      SKW_QUERY_GDAT_TEXT(con(0x7), table1d6712[wordrg12 + 4], t_00, con(0x0)); // EVIL index shifted, was 0x1d6716
      x16 wordrg11;
      if (ddata.v1e0ffa[4 * vw_98 + wordrg5 + 2] == con(0x0)) // EVIL offset shifted by 2, was 1e0ffc
        wordrg11 = con(0xd);
      else
        wordrg11 = con(0x7);
      SKW_DRAW_VP_STR(vw_8c, unsignedword(ddata.paletteptr1[wordrg11]), t_00, vw_84);
      ddata.v1e096c[vw_90] = ddata.v1e0ffa[4 * vw_98 + vw_90 + con(0x2)]; // EVIL offset shifted by 2, was 0x1e0ffc
      vw_84 += ddata.strxplus;
    }
    vw_90++;
  }
}

t_text* SKW_QUERY_GDAT_TEXT(x8 eaxb, x8 ebxb, t_text* ecxt, x8 edxb)
{
  *ecxt = NULLBYTE;
  t_text* tp_00 = UPCAST(t_text, SKW_QUERY_GDAT_ENTRY_DATA_BUFF(eaxb, con(0x5), ebxb, edxb));
  if (tp_00 != NULL)
  {
    x16 wordrg3 = CUTX16(SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(eaxb, con(0x5), ebxb, edxb));
    x16 vw_0c = wordrg3;
    t_text* tp_08 = UPCAST(t_text, DM2_ALLOC_LOBIGPOOL_MEMORY(signedlong(wordrg3)));
    SKW_COPY_MEMORY(DOWNCAST(t_text, tp_00), signedlong(wordrg3), DOWNCAST(t_text, tp_08));
    if (ddata.v1e0984 != con(0x0))
    {
      t_text* tptrrg1 = tp_08;
      x8 byterg4 = con(0x0);
      for(;;)
      {
        if (--wordrg3 == con(0xffffffff))
          break;
        *tptrrg1++ = (tptrrg1[con(0x0)] ^ con(0xffffffff)) - byterg4++;
      }
    }
    SKW_FORMAT_SKSTR(tp_08, ecxt);
    DM2_DEALLOC_LOBIGPOOL(signedlong(vw_0c));
  }
  return ecxt;
}

void R_2F5DE(t_gfxdata* eaxp, t_gfxdata* edxp)
{
  s_gfxdatatail* ptrbeforerg1 = getprevious(eaxp);
  s_gfxdatatail* ptrbeforerg6 = getprevious(edxp);
  x16 wordrg2;
  if (ptrbeforerg1->b0 != con(0x4))
    wordrg2 = ptrbeforerg1->width;
  else
    wordrg2 = ((ptrbeforerg1->width + con(0x1)) & con(0xfffffffe)) >> con(0x1);
  SKW_COPY_MEMORY(DOWNCAST(s_gfxdatatail, ptrbeforerg1), lextended(wordrg2 * ptrbeforerg1->height) + con(0x6), DOWNCAST(s_gfxdatatail, ptrbeforerg6));
}

void SKW_QUERY_GDAT_IMAGE_METRICS(x8 eaxb, x8 ebxb, x16* ecxpw, x8 edxb, x16* argpw0)
{
  if (!dm2_ulp.islen(unsignedlong(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, con(0x1), ebxb, edxb))))
  {
    s_2w* sptrrg1 = UPCAST(s_2w, SKW_QUERY_GDAT_ENTRY_DATA_PTR(eaxb, con(0x1), ebxb, edxb));
    *ecxpw = sptrrg1->w0[con(0x0)] & con(0x3ff);
    *argpw0 = sptrrg1->w0[con(0x1)] & con(0x3ff);
  }
  else
  {
    s_gfxdatatail* ptrbeforerg1 = getprevious(SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(eaxb, ebxb, edxb));
    *ecxpw = ptrbeforerg1->width;
    *argpw0 = ptrbeforerg1->height;
  }
}

ui16 SKW_QUERY_CMDSTR_TEXT(t_text* eaxt, const t_text* edxt)
{
  t_text* tptrrg1 = eaxt;
  x16 wordrg3 = con(0x0);
  for(;;)
  {
    tptrrg1 = SKW_SK_STRSTR(tptrrg1, edxt);
    if (tptrrg1 == NULL)
      break;
    tptrrg1 += signedlong(SKW_SK_STRLEN(edxt));
    if (*tptrrg1 == '=')
      tptrrg1++;
    bool boolrg6 = false;
    if (*tptrrg1 == '-')
    {
      boolrg6 = true;
      tptrrg1 += con(0x1);
    }
    for(;;)
    {
      t_text textrg2 = *tptrrg1;
      if (textrg2 < '0' || textrg2 > '9')
        break;
      wordrg3 *= con(0xa);
      wordrg3 += unsignedword(textrg2 - '0');
      tptrrg1++;
    }
    if (boolrg6)
      wordrg3 = -wordrg3;
  }
  return wordrg3;
}

ui16 SKW_QUERY_CMDSTR_ENTRY(x8 eaxb, x8 ebxb, x16 ecxw, x8 edxb)
{
  t_text t_00[0x80];

  if (ecxw >= con(0x12))
    return con(0x0);
 t_text* tptrrg1 = SKW_QUERY_GDAT_TEXT(eaxb, ebxb, t_00, edxb);
  if (*tptrrg1 != NULLBYTE)
    return SKW_QUERY_CMDSTR_TEXT(tptrrg1, table1d6912[ecxw]);
  return con(0x0);
}

// ebx is a textcharacter
bool SKW_2759_01fe(x16 ebxw, x16 edxw)
{
  if (edxw == con(0xffffffff))
    return false;
  if (urshift(edxw & con(0x3c00), con(0xa)) != con(0x9))
    return true;
  c_record* recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(edxw);
  x16 wordrg11 = recoptrrg2->u4.w & con(0x6);
  if (wordrg11 != con(0x2))
    return true;
  ui16 uwordrg3 = urshift(recoptrrg2->u4.w, con(0xd));
  if (uwordrg3 < con(0x1) || uwordrg3 > wordrg11)
    return false;
  if (SKW_GET_MISSILE_REF_OF_MINION(recoptrrg2->u2.w, edxw) == NULL)
  {
    if (ebxw == con(0x2f) || ebxw == con(0x2c))
      return true;
    if (recoptrrg2->u6.w == con(0xffffffff))
      return false;
    if (ebxw == con(0x2d))
      return true;
    return ebxw == con(0x2e);
  }
  if (ebxw == con(0x30))
    return true;
  if (uwordrg3 != con(0x2))
    return false;
  x8 byterg1 = SKW_GET_ADDRESS_OF_RECORD(recoptrrg2->u2.w)->u4.b.b0;
  if (byterg1 != con(0x33))
  {
    if (byterg1 != con(0x32))
      return false;
    return ebxw == con(0x2e);
  }
  return ebxw == con(0x2d);
}

bool SKW_IS_MISSILE_VALID_TO_LAUNCHER(x16 eaxw, x16 ebxw, x16 edxw)
{
  if (ebxw != con(0xffffffff))
    if (ddata.hero[eaxw].warrc3[edxw] != con(0xffffffff))
    {
      x16 wordrg3 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(ebxw, con(0x5));
      if ((wordrg3 & con(0x8000)) != con(0x0))
      {
        x16 wordrg1 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(ebxw, con(0x5));
        if ((wordrg1 & con(0x8000)) == con(0x0))
          if ((wordrg3 & (wordrg1 & con(0x7fff))) != con(0x0))
            return true;
      }
    }
  return false;
}

// was SKW_2759_0f39
void SKW_ADJUST_HAND_COOLDOWN(x16 eaxw, x16 ebxw, x16 edxw)
{
  c_hero* heroptrrg6 = &ddata.hero[eaxw];
  ui16 uwordrg4 = edxw + edxw / 4;
  x16 wordrg2 = ebxw;
  x16 wordrg3;
  if (ebxw != con(0xffffffff))
    wordrg3 = con(0x1);
  else
  {
    wordrg3 = con(0x3);
    wordrg2 = con(0x0);
  }
  if (ddata.savegames1.b4 != con(0x0))
    uwordrg4 = urshift(uwordrg4, con(0x2));
  uwordrg4 += con(0x2);
  for(;;)
  {
    ui16 uwordrg1 = unsignedword(heroptrrg6->b2a[wordrg2]);
    x16 wordrg5;
    if (uwordrg4 <= uwordrg1)
      wordrg5 = uwordrg4 / 2;
    else
    {
      wordrg5 = uwordrg1 / 2;
      uwordrg1 = uwordrg4;
    }
    uwordrg1 += wordrg5;
    if (uwordrg1 > con(0xff))
      uwordrg1 = con(0xff);
    heroptrrg6->b2a[wordrg2] = CUTX8(uwordrg1);
    wordrg2++;
    if (--wordrg3 == con(0x0))
      return;
  }
}

bool R_30DEA(x16 eaxw)
{
  x16 wordrg1 = SKW_QUERY_CMDSTR_ENTRY(ddata.v1e0b7f, ddata.v1e0b7e, con(0x8), ddata.v1e0b80);
  if (wordrg1 == con(0x10))
    wordrg1 = con(0xf);
  if (wordrg1 != con(0x11) && wordrg1 != con(0x12))
  {
    wordrg1 = -wordrg1;
    SKW_ADD_ITEM_CHARGE(ddata.v1e0b50, wordrg1);
  }
  bool flag = SKW_ADD_ITEM_CHARGE(ddata.v1e0b50, con(0x0)) == con(0x0); // logic negated
  if (flag)
    if ((SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(ddata.v1e0b50, con(0x0)) & con(0x800)) != con(0x0))
    {
      SKW_REMOVE_POSSESSION(eaxw, ddata.v1e0b76);
      SKW_DEALLOC_RECORD(ddata.v1e0b50);
    }
  return flag;
}

// was SKW_2759_1204
x32 SKW_PROCEED_COMMAND_SLOT(x16 eaxw)
{
  x32 longrg3 = con(0x0);
  if (ddata.v1e0b74 != con(0x0))
  {
    x16 wordrg2 = ddata.v1e0b74 - con(0x1);
    x16 wordrg1 = wordrg2;
    c_hero* heroptrrg4 = &ddata.hero[wordrg1];
    if (eaxw != con(0xffffffff))
    {
      heroptrrg4->u32.b.b1 |= con(0x8);
      longrg3 = SKW_ENGAGE_COMMAND(wordrg1, eaxw);
      s_size8* sptrrg4;
      bool flag;
      if (ddata.v1e0b76 == con(0x1) || ddata.v1e0b76 == con(0x0))
      {
        sptrrg4 = &ddata.v1e0adc[wordrg2][ddata.v1e0b76]; // BUGFIX 3/2014
        flag = false;
      }
      else
      {
        sptrrg4 = NULL;
        flag = true;
      }
      if (flag || sptrrg4->w2 == con(0x0) || (sptrrg4->b4 != con(0x0) && SKW_RAND16(sptrrg4->w0 + con(0x1)) == con(0x0)))
      {
        if (R_30DEA(wordrg2) && sptrrg4 != NULL)
        {
          sptrrg4->w0 = con(0x0);
          sptrrg4->w2 = con(0x0);
          sptrrg4->b4 = con(0x0);
        }
      }
    }
    else
      ddata.v1e0b62 = con(0x0);
    if (ddata.v1e0b62 == con(0x0))
      SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
    else
      ddata.v1e0b62 &= con(0xfffffbff);
    SKW_UPDATE_RIGHT_PANEL(false);
  }
  return longrg3;
}

bool SKW_CONFUSE_CREATURE(x16 eaxw, x16 ebxw, x16 edxw)
{
  if (ddata.v1e0b4c != con(0xffffffff))
  {
    ui16 uwordrg2 = (unsignedlong(SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(SKW_GET_ADDRESS_OF_RECORD(ddata.v1e0b4c)->u4.b.b0)->w16) >> 4) & con(0xf);
    if (uwordrg2 <= SKW_RAND16(eaxw) && uwordrg2 != con(0xf))
    {
      SKW_ATTACK_CREATURE(ddata.v1e0b4c, ebxw, con(0x2005), edxw, con(0x64), con(0x0));
      return true;
    }
  }
  return false;
}

x16 SKW_FIND_HAND_WITH_EMPTY_FLASK(c_hero* eaxphero)
{
  x16 wordrg4 = con(0x2);
  for(;;)
  {
    wordrg4--;
    if (wordrg4 < con(0x0))
      return wordrg4;
    x16 wordrg1 = eaxphero->warrc3[wordrg4];
    if (urshift(wordrg1 & con(0x3c00), con(0xa)) != con(0x8))
      continue;
    if (SKW_QUERY_CLS2_FROM_RECORD(wordrg1) == con(0x14))
      return wordrg4;
  }
}

bool SKW_TRY_CAST_SPELL(void)
{
  x16 wordrg5 = ddata.v1e0b74 - con(0x1);
  c_hero* heroptrrg3 = &ddata.hero[wordrg5];
  s_802* sptrrg1 = SKW_FIND_SPELL_BY_RUNES(heroptrrg3->t22);
  x16 wordrg4;
  if (sptrrg1 != NULL)
    wordrg4 = SKW_2759_222e(wordrg5, unsignedword(heroptrrg3->t22[con(0x0)]) - con(0x5f), sptrrg1);
  else
    wordrg4 = con(0x20);
  bool boolrg5 = wordrg4 != con(0x0);
  if (boolrg5)
    SKW_PROCEED_SPELL_FAILURE(wordrg4);
  wordrg4 &= con(0xf0);
  if (wordrg4 != con(0x30))
  {
    heroptrrg3->t22[con(0x0)] = wordrg4 >> con(0x8);
    heroptrrg3->b1e = wordrg4 >> con(0x8);
    SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
  }
  return boolrg5;
}

// TODO: evil offsets inside
void SKW_ADD_RUNE_TO_TAIL(x16 eaxw)
{
  x16 wordrg3 = eaxw;
  c_hero* heroptrrg4 = &ddata.hero[ddata.v1e0b74 - con(0x1)];
  ui16 uwordrg1 = unsignedword(table1d67e0[heroptrrg4->b1e][wordrg3]);
  x8 byterg2 = heroptrrg4->b1e;
  if (byterg2 != con(0x0))
  {
    //RG5L = unsignedlong(table1d6797[heroptrrg4->t22[con(0x20]) + 1]); // EVIL offset was 0x1d6798
    // TODO !
    int a = (heroptrrg4->t22[con(0x0)] + 0x1d6798 - 0x1d67e0) / 6;
    int b = (heroptrrg4->t22[con(0x0)] + 0x1d6798 - 0x1d67e0) % 6;
    uwordrg1 = urshift(uwordrg1 * table1d67e0[a][b], con(0x3)); // table: EVIL offset was 0x1d6798
  }
  bool boolrg5 = false;
  if (uwordrg1 <= heroptrrg4->w3e)
  {
    heroptrrg4->w3e -= uwordrg1;
    heroptrrg4->u32.b.b1 |= con(0x8);
    t_text textrg3 = CUTX8(wordrg3); // TODO: would be enough if eaxw is a byte only
    textrg3 += byterg2 * con(0x6) + con(0x60);
    heroptrrg4->t22[byterg2] = textrg3;
    byterg2++;
    heroptrrg4->b1e = byterg2;
    heroptrrg4->t22[byterg2] = con(0x0);
    boolrg5 = true;
  }
  ddata.v1e0b6c = con(0x1);
  SKW_UPDATE_RIGHT_PANEL(false);
  R_107B0();
  if (boolrg5)
    SKW_REFRESH_PLAYER_STAT_DISP(ddata.v1e0b74 - con(0x1));
}

void SKW_29ee_000f(void)
{
  c_c5rects vs_00;

  x8 vb_34 = (!ddata.v1e03a8) ? con(0x2) : con(0xe);
  SKW_0b36_0c52(&vs_00, true, con(0x9));
  SKW_FILL_ENTIRE_PICT(SKW_3e74_5817(vs_00.s_00.w_00), ddata.paletteptr1[con(0x0)]);
  for (x16 wordrg5 = con(0x28); wordrg5 < con(0x2e); wordrg5++)
  {
    SKW_DRAW_ICON_PICT_ENTRY(con(0x1), vb_34, &vs_00, con(0x3), wordrg5, con(0xffff));
    vb_34 += con(0x2);
  }
  SKW_0b36_0cbe(&vs_00, true);
  SKW_1031_0541(ddata.v1e03a8 ? con(0x6) : con(0x5));
}

// was SKW_29ee_026c
// TODO: ebxb bool?
void SKW_DRAW_HAND_ACTION_ICONS(x16 eaxw, x8 ebxb, x16 edxw)
{
  c_hex18 vh18_08;
  c_rect rc_20;
  c_rect rc_28;

  SKW_29ee_00a3(false);
  c_hero* heroptrrg6 = &ddata.hero[eaxw];
  x16 wordrg1;
  if (edxw != con(0x1))
    wordrg1 = con(0x4a);
  else
    wordrg1 = con(0x46);
  wordrg1 += (unsignedword(heroptrrg6->b1d) + con(0x4) - ddata.v1e0258) & con(0x3);
  SKW_QUERY_EXPANDED_RECT(wordrg1, &rc_28);
  if (heroptrrg6->w36 != con(0x0))
  {
    SKW_DRAW_ICON_PICT_ENTRY(con(0x1), 2 * CUTX8(edxw) + con(0x2) + ebxb, &ddata.v1d694c, con(0x4), wordrg1, con(0xffff));
    t_gfxdata* gptrrg5 = SKW_DRAW_ITEM_ON_WOOD_PANEL(eaxw, &vh18_08, edxw);
    s_gfxdatatail* ptrbeforerg5 = getprevious(gptrrg5);
    if (gptrrg5 != NULL)
    {
      SKW_CALC_CENTERED_RECT_IN_RECT(&rc_20, ptrbeforerg5->width, ptrbeforerg5->height, &rc_28);
//                             srcbmp                                                                                                   palette
      SKW_DRAW_ICON_PICT_BUFF(gptrrg5, &rc_20, con(0x0), &ddata.v1d694c, con(0x0), unsignedword(ddata.paletteptr1[con(0x8)]), con(0x0), NULL);
      SKW_0b36_01cd(&vh18_08);
    }
    if ((heroptrrg6->b2a[edxw] != con(0x0)) || (ddata.v1e0238))
      SKW_0b36_105b(&ddata.v1d694c, con(0x0), &rc_28);
  }
  else
    SKW_FILL_RECT_SUMMARY(&ddata.v1d694c, ddata.paletteptr1[0], &rc_28);
}

void SKW_29ee_0396(void)
{
  c_image vs_00;
  t_gfxdata vcarr_13c[PAL16];
  c_rect rc_14c;

  SKW_29ee_00a3(false);
  SKW_DRAW_ICON_PICT_ENTRY(con(0x8), con(0xf5), &ddata.v1d694c, ddata.v1d67d6, con(0x2f), con(0xffff));
  t_gfxdata* gptrrg6 = SKW_ALLOC_PICT_BUFF(CUTLX8(ddata.v1d272c), BIGPOOL_LO, con(0x4), ddata.v1d272e);
  for (x16 vw_15c = con(0x0); vw_15c < ddata.savegamew2; vw_15c++)
  {
    x16 wordrg2 = (unsignedword(ddata.hero[vw_15c].b1d) + con(0x4) - ddata.v1e0258) & con(0x3);
    if (ddata.hero[vw_15c].w36 != con(0x0))
    {
      if (ddata.v1e00b8 != (wordrg2 + con(0x1)))
      {
        x16 vw_158 = con(0x0);
        x16 vw_154 = con(0x0);
        wordrg2 += con(0x35);
        if (SKW_QUERY_BLIT_RECT(gptrrg6, wordrg2, &vw_154, &rc_14c, &vw_158, con(0xffffffff)) != NULL)
        {
          if (ddata.savegames1.b4 != con(0x0))
          {
            x16 wordrg12 = SKW_RAND02();
            if (wordrg12 != con(0x0))
            {
              wordrg12 -= con(0x2);
              rc_14c.y += wordrg12;
              x16 wordrg11 = SKW_RAND02();
              if (wordrg11 != con(0x0))
              {
                wordrg11 -= con(0x2);
                rc_14c.x += wordrg11;
              }
            }
          }
//                                         srcbmp                                                                                                      palette
          SKW_DRAW_ICON_PICT_BUFF(SKW_QUERY_GDAT_SQUAD_ICON(gptrrg6, vcarr_13c, vw_15c), &rc_14c, vw_154, &ddata.v1d694c, vw_158, con(0xc), con(0x0), vcarr_13c);
          if (ddata.hero[vw_15c].w103 != con(0x0))
          {
            SKW_QUERY_PICST_IT(SKW_QUERY_GDAT_SUMMARY_IMAGE(&vs_00, con(0x7), ddata.hero[vw_15c].b102, con(0x1)));
            vs_00.s_00.w_04 |= con(0x10);
            vs_00.s_00.width >>= con(0x2);
            vs_00.s_00.w_0e = vs_00.s_00.width * ((unsignedword(ddata.hero[vw_15c].b1c) + con(0x4) - ddata.v1e0258) & con(0x3));
            vs_00.s_00.height >>= con(0x2);
            vs_00.s_00.w_10 = CUTX16(ddata.longmallocx) & con(0x3) * vs_00.s_00.height;
            SKW_0b36_11c0(&vs_00, wordrg2, con(0xc), &ddata.v1d694c);
          }
        }
      }
    }
  }
  SKW_FREE_PICT_BUFF(gptrrg6);
  if (ddata.v1e0238)
    SKW_0b36_105b(&ddata.v1d694c, con(0x0), SKW_QUERY_EXPANDED_RECT(con(0x2f), &rc_14c));
}

void SKW_DRAW_PLAYER_NAME_AT_CMDSLOT(void)
{
  SKW_DRAW_ICON_PICT_ENTRY(con(0x1), con(0x14), &ddata.v1d694c, con(0x4), con(0x3c), con(0xffff));
  SKW_DRAW_ICON_PICT_ENTRY(con(0x1), con(0xe), &ddata.v1d694c, con(0x4), con(0x3b), con(0xffff));
  x16 wordrg2 = ddata.v1e0b74 - con(0x1);
  SKW_DRAW_NAME_STR(&ddata.v1d694c, unsignedword(ddata.paletteptr1[(wordrg2 != ddata.mouse_unk0e) ? con(0xf) : con(0x9)]), unsignedword(ddata.paletteptr1[con(0xc)]) | con(0x4000), con(0x3d), ddata.hero[wordrg2].t00);
}

void SKW_DRAW_PLAYER_ATTACK_DIR(void)
{
  c_image vs_00;
  t_gfxdata vcarr_13c[PAL16];
  c_rect rc_14c;

  SKW_29ee_00a3(false);
  SKW_DRAW_ICON_PICT_ENTRY(con(0x8), con(0xf6), &ddata.v1d694c, ddata.v1d67d6, con(0x5d), con(0xffff));
  t_gfxdata* gptrrg6 = SKW_ALLOC_PICT_BUFF(ddata.v1d272c, BIGPOOL_LO, con(0x4), ddata.v1d272e);
  x16 vw_154 = con(0x0);
  x16 vw_158 = con(0x0);
  if (SKW_QUERY_BLIT_RECT(gptrrg6, con(0x5e), &vw_158, &rc_14c, &vw_154, con(0xffffffff)) != NULL)
  {
    if (ddata.savegames1.b4 != con(0x0))
    {
      x16 wordrg11 = SKW_RAND02(); // BUGFIX 4/2014
      if (wordrg11 != con(0x0))
      {
        wordrg11 -= con(0x2);
        rc_14c.y += wordrg11;
        x16 wordrg12 = SKW_RAND02();
        if (wordrg12 != con(0x0))
        {
          wordrg12 -= con(0x2);
          rc_14c.x += wordrg12;
        }
      }
    }
    x16 wordrg5 = ddata.v1e0b74 - con(0x1);
    SKW_QUERY_GDAT_SQUAD_ICON(gptrrg6, vcarr_13c, wordrg5);
//                           srcbmp                                                               palette
    SKW_DRAW_ICON_PICT_BUFF(gptrrg6, &rc_14c, vw_158, &ddata.v1d694c, vw_154, con(0xc), con(0x0), vcarr_13c);
    if (ddata.hero[wordrg5].w103 != con(0x0))
    {
      SKW_QUERY_PICST_IT(SKW_QUERY_GDAT_SUMMARY_IMAGE(&vs_00, con(0x7), ddata.hero[wordrg5].b102, con(0x1)));
      vs_00.s_00.w_04 |= con(0x10);
      vs_00.s_00.width >>= con(0x2);
      vs_00.s_00.w_0e = ((unsignedword(ddata.hero[wordrg5 / 263].b1c) + con(0x4) - ddata.v1e0258) & con(0x3)) * vs_00.s_00.width;
      vs_00.s_00.height >>= con(0x2);
      vs_00.s_00.w_10 = (CUTX16(ddata.longmallocx) & con(0x3)) * vs_00.s_00.height;
      SKW_0b36_11c0(&vs_00, con(0x5e), con(0xc), &ddata.v1d694c);
    }
  }
  SKW_FREE_PICT_BUFF(gptrrg6);
  SKW_DRAW_ICON_PICT_ENTRY(con(0x1), con(0x10), &ddata.v1d694c, con(0x4), con(0x60), con(0xffff));
  SKW_DRAW_ICON_PICT_ENTRY(con(0x1), con(0x12), &ddata.v1d694c, con(0x4), con(0x61), con(0xffff));
}

// was SKW_29ee_093e
void SKW_DRAW_SPELL_TO_BE_CAST(bool eaxbool)
{
  t_text t_04[0x4]; // TODO probably just t_04[0x2]

  SKW_29ee_00a3(false);
  if (eaxbool)
    SKW_DRAW_ICON_PICT_ENTRY(con(0x1), con(0x9), &ddata.v1d694c, con(0x5), con(0xfc), con(0xffff));
  c_hero* heroptrrg7 = &ddata.hero[ddata.v1e0b74 - con(0x1)];
  t_04[0x1] = NULLBYTE;
  x16 wordrg6 = con(0x105);
  for (ui16 uwordrg5 = con(0x0); uwordrg5 < UI16CAST(SKW_SK_STRLEN(heroptrrg7->t22)); uwordrg5++)
  {
    t_04[0x0] = heroptrrg7->t22[uwordrg5];
    SKW_DRAW_SIMPLE_STR(&ddata.v1d694c, unsignedword(ddata.paletteptr1[con(0x0)]), unsignedword(ddata.paletteptr1[con(0xd)]) | con(0x4000), wordrg6++, t_04);
  }
}

void SKW_DRAW_CMD_SLOT(x16 eaxw, bool edxbool)
{
  SKW_29ee_00a3(false);
  if (ddata.v1e0b62 != con(0x0))
    SKW_DRAW_ICON_PICT_ENTRY(con(0x14), 2 * (ddata.v1e0b40[eaxw].b2 - con(0x8)) + con(0x41) + (edxbool ? 1 : 0), &ddata.v1d694c, ddata.v1e0ba4, eaxw + con(0x6e), con(0xffff));
  else
  {
    SKW_DRAW_ICON_PICT_ENTRY(con(0x1), (edxbool ? 1 : 0) + con(0x15), &ddata.v1d694c, con(0x4), eaxw + con(0x3f), con(0xffff));
    SKW_DRAW_NAME_STR(&ddata.v1d694c, unsignedword(ddata.paletteptr1[con(0xf)]), unsignedword(ddata.paletteptr1[con(0x0)]) | con(0x4000), eaxw + con(0x42), SKW_QUERY_CMDSTR_NAME(ddata.v1e0b40[eaxw].b0, ddata.v1e0b40[eaxw].b2, ddata.v1e0b40[eaxw].b1)); // BUGFIX 3/2014
  }
}

void SKW_DRAW_CHIP_OF_MAGIC_MAP(t_gfxdata* eaxpg, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, t_gfxdata* argpg1)
{
  c_rect rc_00;

  if (eaxpg != NULL)
  {
    SKW_SET_SRECT(&rc_00, ebxw, ecxw, ddata.v1d274a, ddata.v1d274c);
    x16 vw_08;
    x16 vw_0c;
    if (SKW_UNION_RECT(&rc_00, &vw_0c, &vw_08, &ddata.v1e0b9c) != NULL)
    {
      if (ddata.v1d274a != rc_00.w)
        if ((argw0 & con(0x1)) != con(0x0))
        {
          if (vw_0c == con(0x0))
            vw_0c = ddata.v1d274a - rc_00.w;
          else
            vw_0c = con(0x0);
        }
      if (ddata.v1d274c != rc_00.h)
        if ((argw0 & con(0x2)) != con(0x0))
        {
          if (vw_08 == con(0x0))
            vw_08 = (ddata.v1d274c - rc_00.h);
          else
            vw_08 = con(0x0);
        }
//                            srcbmp                                                                               palette
      SKW_DRAW_ICON_PICT_BUFF(eaxpg, &rc_00, edxw * ddata.v1d274a + vw_0c, &ddata.v1d694c, vw_08, con(0xa), argw0, argpg1);
    }
  }
}

void SKW_DRAW_MONEYBOX(x16 eaxw)
{
  x16 vwa_04[0xa];
  c_rect rc_18;
  c_rect rc_20;

  SKW_DRAW_ICON_PICT_ENTRY(con(0x14), con(0x10), &ddata.v1d694c, SKW_QUERY_CLS2_FROM_RECORD(eaxw), con(0x5c), con(0xffff));
  SKW_COUNT_BY_COIN_TYPES(eaxw, vwa_04);
  for (x16 vw_38 = con(0x0); vw_38 < con(0xa); vw_38++)
  {
    x16 wordrg1 = SKW_GET_ITEM_ORDER_IN_CONTAINER(eaxw, vw_38);
    if (wordrg1 >= con(0x0))
    {
      x16 wordrg5 = vwa_04[wordrg1];
      if (wordrg5 > con(0x0))
      {
        x16 wordrg11 = ddata.v1e0394[wordrg1];
        x8 vb_2c = SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(wordrg11);
        x8 byterg1 = SKW_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(wordrg11);
        t_gfxdata* gptrrg6 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(byterg1, con(0x18), vb_2c);
        t_gfxdata* pg_28 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(byterg1, con(0x18), vb_2c);
        x16 vw_30 = con(0x0);
        x16 vw_34 = con(0x0);
        SKW_QUERY_BLIT_RECT(gptrrg6, vw_38 + con(0xdd), &vw_34, &rc_18, &vw_30, con(0xffffffff));
        rc_20.w = rc_18.w;
        rc_20.h = rc_18.h;
        x16 wordrg6 = vw_38 << con(0x3);
        wordrg6 += DM2_MAX(con(0x0), wordrg5 - con(0x20));
        wordrg5 = DM2_MIN(con(0x20), wordrg5);
        do
        {
          wordrg6++;
          wordrg6 &= con(0x1f);
          rc_20.x = rc_18.x + signedword(table1d275a[wordrg6][0]);
          rc_20.y = rc_18.y + signedword(table1d275a[wordrg6][1]);
          SKW_UNION_RECT(&rc_20, &vw_34, &vw_30, &ddata.v1d694c.s_00.rc_02);
//                                 srcbmp                                                           palette
          SKW_DRAW_ICON_PICT_BUFF(gptrrg6, &rc_20, vw_34, &ddata.v1d694c, vw_30, con(0xc), con(0x0), pg_28);
          wordrg5--;
        } while (wordrg5 > con(0x0));
      }
    }
  }
}

// was SKW_29ee_21d8
void SKW_DRAW_CONTAINER_PANEL(x16 eaxw, bool edxbool)
{
  c_rect rc_00;
  c_rect rc_08;

  x8 vb_18 = SKW_QUERY_CLS2_FROM_RECORD(eaxw);
  if (edxbool)
  {
    SKW_DRAW_ICON_PICT_ENTRY(con(0x14), con(0x10), &ddata.v1d694c, vb_18, con(0x5c), con(0xffff));
    SKW_DRAW_ICON_PICT_ENTRY(con(0x14), con(0x12), &ddata.v1d694c, vb_18, con(0xe3), con(0xa));
  }
  if (!edxbool)
    SKW_QUERY_EXPANDED_RECT(con(0x5c), &rc_08);
  for (x16 wordrg5 = con(0x0); wordrg5 < con(0x8); wordrg5++)
  {
    x16 vw_10 = ddata.v1e095c[wordrg5];
    if (vw_10 != con(0xffffffff))
    {
      x16 vw_14 = wordrg5 + con(0xe5);
      if (!edxbool)
      {
        SKW_QUERY_EXPANDED_RECT(vw_14, &rc_00);
//                                        srcbmp                                                                                                                                                            palette
        SKW_DRAW_ICON_PICT_BUFF(SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x14), con(0x10), vb_18), &rc_00, rc_00.x - rc_08.x, &ddata.v1d694c, rc_00.y - rc_08.y, con(0xffff), edxbool ? 1 : 0, SKW_QUERY_GDAT_IMAGE_LOCALPAL(con(0x14), con(0x10), vb_18));
      }
      x8 byterg2 = SKW_2405_014a(vw_10, true, wordrg5 + con(0x1e));
      SKW_DRAW_ICON_PICT_ENTRY(SKW_QUERY_CLS1_FROM_RECORD(vw_10), byterg2, &ddata.v1d694c, SKW_QUERY_CLS2_FROM_RECORD(vw_10), vw_14, con(0xc));
    }
  }
}

bool SKW_2c1d_0186(c_hero* eaxphero, x16 ebxw, x16 ecxw, x8 edxb)
{
  x16 wordrg2 = ebxw;
  x16 wordrg3 = ecxw;
  bool boolrg5 = true;
  if (wordrg3 != con(0x0))
  {
    if (eaxphero->w3e == con(0x0))
      return false;
    wordrg3 = eaxphero->w3e;
    if (wordrg3 >= con(0x4))
      eaxphero->w3e -= con(0x4);
    else
    {
      wordrg2 = urshift(wordrg2, con(0x1));
      eaxphero->w3e = con(0x0);
      boolrg5 = false;
    }
  }
  SKW_2c1d_0009(con(0xf), wordrg3 >> con(0x5), wordrg2, edxb);
  return boolrg5;
}

x16 SKW_GET_PLAYER_AT_POSITION(x16 eaxw)
{
  c_hero* heroptrrg1 = ddata.hero;
  for (ui16 uwordrg4 = con(0x0); uwordrg4 < ddata.savegamew2; uwordrg4++, heroptrrg1++)
    if (unsignedword(heroptrrg1->b1d) == eaxw)
      if (heroptrrg1->w36 != con(0x0))
        return uwordrg4;
  return con(0xffffffff);
}

void SKW_2c1d_0300(c_hero* eaxphero, x16 ebxw, x16 edxw)
{
  x8* bptrrg3 = &eaxphero->b4a[2 * edxw];
  x16 wordrg1 = unsignedword(*bptrrg3) + ebxw - unsignedword(*(bptrrg3 + 1));
  x16 wordrg2 = ebxw;
  if (wordrg1 < con(0x0) == ebxw < con(0x0))
  {
    wordrg1 = DM2_ABS(wordrg1);
    for(;;)
    {
      if (wordrg1 <= con(0x14))
        break;
      wordrg2 -= wordrg2 / 4;
      wordrg1 -= con(0x14);
    }
  }
  *bptrrg3 = CUTX8(DM2_BETWEEN_VALUE(con(0xa), con(0xdc), unsignedword(*bptrrg3) + wordrg2));
}

void SKW_TAKE_OBJECT(x16 eaxw, bool edxbool)
{
  if (eaxw != con(0xffffffff))
  {
    x16 wordrg2 = eaxw & con(0x3fff);
    x16 wordrg3 = wordrg2;
    ddata.savegamewpc.w18 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(wordrg3, con(0x0));
    ddata.savegamewpc.w1a = SKW_QUERY_ITEM_WEIGHT(wordrg3);
    ddata.savegamewpc.w0 = wordrg2;
    SKW_DRAW_ITEM_IN_HAND(&ddata.savegamewpc);
    SKW_FIRE_HIDE_MOUSE_CURSOR();
    SKW_DISPLAY_TAKEN_ITEM_NAME(wordrg3);
    if (!edxbool)
      SKW_443c_0434();
    else
      ddata.v1e0254 = con(0x1);
    SKW_FIRE_SHOW_MOUSE_CURSOR();
    SKW_PROCESS_ITEM_BONUS(ddata.mouse_unk0e, con(0xffffffff), con(0x1), wordrg2);
    R_3CE7D(ddata.v1e0270, con(0xffff), true, ddata.v1e0272, true, false);
  }
}

// TODO: offsetproblem
x16 SKW_REMOVE_POSSESSION(x16 eaxw, x16 edxw)
{
  c_hero* heroptrrg6 = &ddata.hero[eaxw];
  x16 wordrg5;
  if (edxw < con(0x1e))
  {
    wordrg5 = heroptrrg6->warrc3[edxw];
    heroptrrg6->warrc3[edxw] = con(0xffffffff);
  }
  else
  {
    wordrg5 = ddata.v1e095c[edxw - 0x3c / 2]; // EVIL! offsetproblem1 TODO
    ddata.v1e095c[edxw - 0x3c / 2] = con(0xffffffff); // EVIL! offsetproblem1 TODO
  }
  if (wordrg5 != con(0xffffffff))
  {
    if (eaxw == (ddata.v1e0b74 - con(0x1)))
      if (edxw <= con(0x1))
        if (edxw == ddata.v1e0b76)
          SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
    SKW_PROCESS_ITEM_BONUS(eaxw, edxw, con(0xffffffff), wordrg5);
  }
  return wordrg5;
}

// TODO: offsetproblem
void SKW_EQUIP_ITEM_TO_HAND(x16 eaxw, x16 ebxw, x16 edxw)
{
  x16 wordrg4 = edxw;
  if (wordrg4 != con(0xffffffff))
  {
    c_hero* heroptrrg6 = &ddata.hero[eaxw];
    wordrg4 &= con(0x3fff);
    if (ebxw < con(0x1e))
      heroptrrg6->warrc3[ebxw] = wordrg4;
    else
      ddata.v1e095c[ebxw - 0x3c / 2] = wordrg4; // EVIL! offsetproblem1 TODO
    SKW_PROCESS_ITEM_BONUS(eaxw, ebxw, con(0x1), wordrg4);
  }
}

ui16 SKW_QUERY_PLAYER_SKILL_LV(x16 eaxw, bool ebxbool, x16 edxw)
{
  if (ddata.v1e0238)
    return con(0x1);
  c_hero* heroptrrg2 = &ddata.hero[eaxw];
  x16 wordrg4 = edxw;
  ui32 ulongrg1 = heroptrrg2->l5f[wordrg4];
  if (wordrg4 > con(0x3))
  {
    x32 longrg4;
    if (!ebxbool)
      longrg4 = con(0x1);
    else
    {
      wordrg4 -= con(0x4);
      wordrg4 >>= con(0x2);
      longrg4 = signedlong(heroptrrg2->baf[wordrg4]) + con(0x1);
    }
    longrg4 *= heroptrrg2->l5f[(edxw - con(0x4)) / 4];
    ulongrg1 += longrg4;
    ulongrg1 >>= con(0x1);
  }
  ui16 uwordrg41 = con(0x1);
  while (ulongrg1 >= con(0x200))
  {
    ulongrg1 >>= con(0x1);
    uwordrg41++;
  }
  if (ebxbool)
  {
    uwordrg41 += signedword(heroptrrg2->baf[edxw]);
    uwordrg41 = DM2_MAX(con(0x1), uwordrg41);
  }
  return uwordrg41;
}

x16 SKW_GET_PLAYER_ABILITY(c_hero* eaxphero, x16 ebxw, x16 edxw)
{
  x16 wordrg4 = eaxphero->b4a[2 * edxw + ebxw];
  if (ebxw == con(0x0)) // BUGFIX 3/2014
  {
    x16 wordrg1 = eaxphero->w103;
    if (wordrg1 != con(0x0) && eaxphero->b102 >= con(0x3) && eaxphero->b102 <= con(0x6) && (eaxphero->b102 -= con(0x2), edxw == unsignedword(eaxphero->b102))) // BUGFIX 3/2014
    {
      if (wordrg1 > con(0x64))
        wordrg1 = con(0x64);
      wordrg4 += SKW_RAND16(((wordrg1 * wordrg4) >> con(0x7)) + con(0x1)) + con(0x4);
    }
  }
  return DM2_BETWEEN_VALUE(con(0xa), con(0xdc), signedword(eaxphero->b58[edxw]) + wordrg4); // BUGFIX 4/2014
}

// TODO: make edxw unsigned
// was SKW_2c1d_0f67
bool SKW_USE_LUCK_ATTRIBUTE(c_hero* eaxphero, x16 edxw)
{
  if (SKW_RAND01() != con(0x0) && SKW_RAND16(con(0x64)) > UI16CAST(edxw))
    return true;
  bool boolrg6 = SKW_RAND16((2 * SKW_GET_PLAYER_ABILITY(eaxphero, con(0x0), con(0x0)))) > UI16CAST(edxw);
  x16 wordrg2 = DM2_MIN(con(0xdc), SKW_GET_PLAYER_ABILITY(eaxphero, con(0x1), con(0x0)));
  x16 wordrg1;
  if (!boolrg6)
    wordrg1 = con(0x2);
  else
    wordrg1 = con(0xfffffffe);
  wordrg1 += unsignedword(eaxphero->b4a[con(0x0)]);
  eaxphero->b4a[con(0x0)] = CUTX8(DM2_BETWEEN_VALUE(con(0xa), wordrg2, wordrg1));
  return boolrg6;
}

x16 SKW_MAX_LOAD(c_hero* eaxphero)
{
  x16 wordrg1 = SKW_STAMINA_ADJUSTED_ATTR(eaxphero, 8 * SKW_GET_PLAYER_ABILITY(eaxphero, con(0x0), con(0x1)) + con(0x64));
  x16 wordrg4 = eaxphero->w34;
  if (wordrg4 != con(0x0))
  {
    wordrg4 &= con(0x10);
    wordrg1 -= wordrg1 >> (((wordrg4 == con(0x0)) ? 1 : 0) + con(0x2));
  }
  wordrg1 += con(0x9);
  return wordrg1 - (wordrg1 % con(0xa));
}

x16 SKW_CALC_PLAYER_WALK_DELAY(x16 eaxw)
{
  x16 wordrg1;
  if (ddata.savegames1.b4 == con(0x0))
  {
    c_hero* heroptrrg2 = &ddata.hero[eaxw];
    ui16 uwordrg3 = SKW_MAX_LOAD(heroptrrg2);
    x16 wordrg12 = SKW_GET_PLAYER_WEIGHT(eaxw);
    x16 wordrg41;
    x16 wordrg11;
    if (uwordrg3 <= wordrg12)
    {
      wordrg11 = 4 * (wordrg12 - uwordrg3) / uwordrg3 + con(0x4);
      wordrg41 = con(0x2);
    }
    else
    {
      wordrg11 = con(0x2);
      x16 wordrg6 = 8 * wordrg11;
      if (wordrg6 > (5 * uwordrg3))
        wordrg11 = con(0x3);
      wordrg41 = con(0x1);
    }
    if ((heroptrrg2->w34 & con(0x20)) != con(0x0))
      wordrg11 += wordrg41;
    wordrg11 -= signedword(heroptrrg2->b105[con(0x0)]);
    wordrg1 = DM2_MAX(con(0x1), wordrg11);
    if (wordrg1 > con(0x2))
    {
      wordrg1++;
      wordrg1 &= con(0xfffffffe);
    }
  }
  else
    wordrg1 = con(0x1);
  return wordrg1;
}

// was SKW_2c1d_111e
x16 SKW_USE_DEXTERITY_ATTRIBUTE(x16 eaxw)
{
  c_hero* heroptrrg3 = &ddata.hero[eaxw];
  x16 wordrg2 = (CUTX16(SKW_RAND()) & con(0x7)) + SKW_GET_PLAYER_ABILITY(heroptrrg3, con(0x0), con(0x2));
  x16 wordrg3 = SKW_MAX_LOAD(heroptrrg3);
  wordrg2 -= (wordrg2 / 2) * SKW_GET_PLAYER_WEIGHT(eaxw) / wordrg3;
  x16 wordrg21 = DM2_MAX(con(0x2), wordrg2);
  if (ddata.v1e0238)
    wordrg21 >>= con(0x1);
  return DM2_BETWEEN_VALUE((SKW_RAND() & con(0x7)) + con(0x1), con(0x64) - CUTX16(SKW_RAND() & con(0x7)), wordrg21 / 2);
}

x16 SKW_2c1d_132c(x16 eaxw, x16 edxw)
{
  x16 wordrg1 = unsignedword(CUTX8(eaxw));
  if (edxw != con(0x0))
    wordrg1 = SKW_0cd5_0176(wordrg1, urshift(eaxw & con(0x700), con(0x8)) + con(0x4), con(0x3));
  return wordrg1;
}

x16 SKW_2c1d_135d(x16 eaxw, x16 edxw)
{
  x16 vw_00 = edxw;
  c_hero* heroptrrg7 = &ddata.hero[eaxw];
  x16 vw_0c = vw_00 & con(0x8000);
  if (vw_0c != con(0x0))
    vw_00 &= con(0x7fff);
  x16 vw_08 = con(0x0);
  for (x16 wordrg5 = con(0x0); wordrg5 <= con(0x1); wordrg5++)
  {
    x16 wordrg1 = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(heroptrrg7->warrc3[wordrg5], con(0xb));
    x16 wordrg6 = wordrg1;
    wordrg1 &= con(0x8000);
    if (wordrg1 != con(0x0))
    {
      x16 wordrg2 = SKW_COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(eaxw, con(0x7), wordrg5);
      vw_08 += ((SKW_2c1d_132c(wordrg6, vw_0c) + wordrg2) * table1d69aa[vw_00]) >> (((wordrg5 != vw_00) ? 1 : 0) + con(0x4));
    }
  }
  x16 wordrg51 = SKW_RAND16(((SKW_GET_PLAYER_ABILITY(heroptrrg7, con(0x0), con(0x4))) / 8 + con(0x1)));
  if (vw_0c != con(0x0))
    wordrg51 >>= con(0x1);
  if (ddata.hero[eaxw].b102 == con(0x2))
    wordrg51 += ddata.hero[eaxw].w103;
  wordrg51 += signedword(heroptrrg7->b42[con(0x1)]) + signedword(heroptrrg7->b42[con(0x0)]) + vw_08;
  if (vw_00 > con(0x1))
    wordrg51 += SKW_2c1d_132c(SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(heroptrrg7->warrc3[vw_00], con(0xb)), vw_0c);
  if (((con(0x1) << CUTX8(vw_00)) & heroptrrg7->w34) != con(0x0))
    wordrg51 -= SKW_RAND02() + con(0x8);
  if (ddata.v1e0238)
    wordrg51 >>= con(0x1);
  return DM2_BETWEEN_VALUE(con(0x0), con(0x64), wordrg51 / 2);
}

// TODO: return not needed
x8 SKW_DROP_PLAYER_ITEMS(x16 eaxw)
{
  x16 vw_00 = unsignedword(ddata.hero[eaxw].b1d);
  for (x8 byterg5 = con(0x0); byterg5 < con(0x1e); byterg5++)
  {
    x16 wordrg1 = SKW_REMOVE_POSSESSION(eaxw, table1d69b0[byterg5]);
    if (wordrg1 != con(0xffffffff))
    {
      wordrg1 &= con(0x3fff);
      SKW_MOVE_RECORD_TO((vw_00 << con(0xe)) | wordrg1, con(0x0), ddata.v1e0270, con(0xffffffff), ddata.v1e0272);
    }
  }
  return con(0x1e);
}

void SKW_PROCESS_PLAYERS_DAMAGE(void)
{
  c_tim vtim_00;

  c_hero* heroptrrg4 = ddata.hero;
  for (ui16 uwordrg3 = con(0x0); uwordrg3 < UI16CAST(ddata.savegamew2); uwordrg3++, heroptrrg4++)
  {
    heroptrrg4->w34 |= ddata.v1e0ba8[uwordrg3];
    ddata.v1e0ba8[uwordrg3] = con(0x0);
    x16 wordrg5 = ddata.v1e0bb0[uwordrg3];
    if (wordrg5 != con(0x0))
    {
      ddata.v1e0bb0[uwordrg3] = con(0x0);
      x16 wordrg1 = heroptrrg4->w36;
      if (wordrg1 != con(0x0))
      {
        wordrg1 -= wordrg5;
        if (wordrg1 > con(0x0))
        {
          heroptrrg4->w36 = wordrg1;
          heroptrrg4->w30 = wordrg5;
          heroptrrg4->u32.b.b1 |= con(0x8);
          x16 wordrg51 = heroptrrg4->w2e;
          x32 longrg2 = ddata.longmallocx + con(0x5);
          if (wordrg51 != con(0xffffffff))
          {
            ddata.savegametimarrp[wordrg51].un.l_00 = (signedlong(ddata.v1e0266) << con(0x18)) | longrg2;
            SKW_3a15_05f7(wordrg51);
          }
          else
          {
            vtim_00.b_04 = con(0xc);
            longrg2 |= signedlong(ddata.v1e0266) << con(0x18);
            vtim_00.un.l_00 = longrg2;
            vtim_00.b_05 = CUTX8(uwordrg3);
            heroptrrg4->w2e = SKW_QUEUE_TIMER(&vtim_00);
          }
        }
        else
          SKW_PLAYER_DEFEATED(uwordrg3);
      }
    }
  }
}

void SKW_ADJUST_STAMINA(x16 eaxw, x16 edxw)
{
  if (eaxw != con(0xffffffff))
  {
    c_hero* heroptrrg5 = &ddata.hero[eaxw];
    x16 wordrg4 = heroptrrg5->w3a - edxw;
    heroptrrg5->w3a = wordrg4;
    if (wordrg4 > con(0x0))
    {
      if (wordrg4 > heroptrrg5->u3c.w)
        heroptrrg5->w3a = heroptrrg5->u3c.w;
    }
    else
    {
      heroptrrg5->w3a = con(0x0);
      SKW_WOUND_PLAYER(eaxw, con(0x0), con(0x0), -wordrg4 / 2);
    }
    if (DM2_ABS(edxw) >= con(0xa))
      heroptrrg5->u32.b.b1 |= con(0x8);
  }
}

// was SKW_2c1d_1cf3
void SKW_SHOOT_CHAMPION_MISSILE(c_hero* eaxphero, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0)
{
  x16 wordrg6 = unsignedword(eaxphero->b1c);
  SKW_SHOOT_ITEM(edxw, ddata.v1e0272, wordrg6 + (((unsignedword(eaxphero->b1d) - wordrg6 + con(0x1)) & con(0x2)) >> con(0x1)) & con(0x3), ddata.v1e0270, CUTX8(wordrg6), CUTX8(DM2_MIN(ebxw, con(0xff))), CUTX8(DM2_MIN(ecxw, con(0xff))), CUTX8(DM2_MIN(argw0, con(0xff))));
  ddata.v1e025e = con(0x4);
  ddata.v1e0274 = wordrg6;
}

// was SKW_2c1d_1d64
bool SKW_CAST_CHAMPION_MISSILE_SPELL(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  c_hero* heroptrrg5 = &ddata.hero[eaxw];
  if (heroptrrg5->w3e < ecxw)
    return false;
  heroptrrg5->w3e -= ecxw;
  heroptrrg5->u32.b.b1 |= con(0x8);
  x16 wordrg4 = con(0xa) - (DM2_MIN(con(0x6), heroptrrg5->w40 >> con(0x5)));
  x16 wordrg1 = wordrg4;
  wordrg4 <<= con(0x2);
  x16 wordrg2 = ebxw;
  if (wordrg2 < wordrg4)
  {
    wordrg2 += con(0x4);
    wordrg1 = wordrg2 / 4;
  }
  SKW_SHOOT_CHAMPION_MISSILE(heroptrrg5, wordrg2, con(0x5a), edxw, wordrg1);
  return true;
}

// TODO: maybe returns a word, but byte is enough
x8 R_37BEA(x16 eaxw)
{
  if (ddata.hero[eaxw].w36 == con(0x0))
    return con(0x0);
  x16 wordrg3 = ddata.hero[eaxw].u32.w & con(0x10);
  if ((SKW_GET_PLAYER_WEIGHT(eaxw) / con(0xa) + wordrg3) != con(0x0))
    return con(0x32);
  return con(0x28);
}

// was SKW_2c1d_201d
x16 SKW_GET_PARTY_SPECIAL_FORCE(void)
{
  x16 wordrg2 = con(0x0);
  for (x16 wordrg4 = con(0x0); wordrg4 < ddata.savegamew2; wordrg4++)
    wordrg2 += signedword(R_37BEA(wordrg4));
  return wordrg2;
}

t_gfxdata* SKW_QUERY_GDAT_SQUAD_ICON(t_gfxdata* eaxpg, t_gfxdata* ebxpg, x16 edxw)
{
  x16 wordrg3 = (unsignedword(ddata.hero[edxw].b1c) + con(0x4) - ddata.v1e0258) & con(0x3);
  x16 wordrg1;
  if (ddata.savegames1.b2 == con(0x0))
    wordrg1 = con(0x0);
  else
    wordrg1 = con(0x4);
  wordrg3 += wordrg1;
  x16 vw_00 = ddata.v1d272c * wordrg3;
  SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_LOCALPAL(con(0x1), CUTX8(edxw), con(0x6))), PAL16, DOWNCAST(t_gfxdata, ebxpg));
  c_rect* recoptrrg6 = SKW_ALLOC_TEMP_ORIGIN_RECT(ddata.v1d272c, ddata.v1d272e);
//                               srcbmp                                                                srcofs destbmp                   palette
  SKW_DRAW_DIALOGUE_PICT(SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x1), CUTX8(edxw), con(0x6)), recoptrrg6, vw_00, eaxpg, con(0x0), con(0x2), NULL);
  return eaxpg;
}

void SKW_DRAW_PLAYER_3STAT_PANE(x16 eaxw, bool edxbool)
{
  x16 wordrg3 = eaxw;
  if (ddata.v1d69d4.s_00.w_00 == con(0xffffffff))
  {
    x16 wordrg2 = wordrg3;
  x8 vb_00;
    if (ddata.hero[wordrg2].w36 != con(0x0))
    {
      wordrg2++;
      vb_00 = (ddata.v1e0976 != wordrg2) ? con(0x0) : con(0x9);
    }
    else
      vb_00 = con(0x1);
    wordrg3 += con(0xa1);
    SKW_0b36_0c52(&ddata.v1d69d4, edxbool, wordrg3);
    SKW_DRAW_ICON_PICT_ENTRY(con(0x1), vb_00, &ddata.v1d69d4, con(0x2), wordrg3, con(0xffff));
    if (!edxbool)
      ddata.v1d69d4.s_00.w_0a = con(0x0);
  }
}

void SKW_DRAW_PLAYER_3STAT_HEALTH_BAR(x16 eaxw)
{
  x16 vwa_04[0x6];
  c_rect rc_10;
  c_rect rc_18;

  SKW_0b36_0d67(&ddata.v1d69d4, SKW_QUERY_EXPANDED_RECT(eaxw + con(0xb9), &rc_18));
  c_hero* heroptrrg4 = &ddata.hero[eaxw];
  vwa_04[0x0] = heroptrrg4->w36;
  vwa_04[0x1] = heroptrrg4->w38;
  vwa_04[0x2] = heroptrrg4->w3a;
  vwa_04[0x3] = heroptrrg4->u3c.w;
  vwa_04[0x4] = heroptrrg4->w3e; // BUGFIX 2/2014
  vwa_04[0x5] = DM2_MAX(heroptrrg4->w40, heroptrrg4->w3e);
  x16 wordrg6 = eaxw + con(0xc1);
  for (x16 wordrg5 = con(0x0); wordrg5 < con(0x3); wordrg5++)
  {
    x16 wordrg1 = 2 * wordrg5;
    if (vwa_04[0x1 + wordrg1] != con(0x0))
    {
      x32 longrg2 = signedlong(vwa_04[0x1 + wordrg1]);
      x16 wordrg11 = signedlong(vwa_04[0x0 + wordrg1]) * con(0x2710) / longrg2;
      if (SKW_SCALE_RECT(wordrg6, con(0x2710), wordrg11, &rc_18) != NULL)
      {
        SKW_COPY_RECT(&rc_18, &rc_10);
        rc_10.x += ddata.v1d2728;
        rc_10.y += ddata.v1d272a;
        SKW_FILL_RECT_SUMMARY(&ddata.v1d69d4, ddata.paletteptr1[0], &rc_10);
        SKW_FILL_RECT_SUMMARY(&ddata.v1d69d4, ddata.paletteptr1[table1d69d0[eaxw]], &rc_18);
      }
    }
    wordrg6 += con(0x4);
  }
}

void SKW_2e62_0cfa(bool eaxbool)
{
  for (ui16 uwordrg4 = con(0x0); uwordrg4 < ddata.savegamew2; uwordrg4++)
    if ((uwordrg4 + con(0x1)) != ddata.v1e0976)
      SKW_REFRESH_PLAYER_STAT_DISP(uwordrg4);
  if (ddata.v1e0976 != con(0x0))
  {
    c_hero* heroptrrg1 = &ddata.hero[ddata.v1e0976 - con(0x1)];
    if ((heroptrrg1->w103 != con(0x0)) && (heroptrrg1->b102 >= con(0x3) && heroptrrg1->b102 <= con(0x6)))
      heroptrrg1->u32.w |= (!ddata.v1e0228 || ddata.savegamewpc.w0 != con(0xffffffff)) ? con(0x1000) : con(0x3000);
    SKW_REFRESH_PLAYER_STAT_DISP(ddata.v1e0976 - con(0x1));
  }
  SKW_UPDATE_RIGHT_PANEL(eaxbool);
}

// was SKW_2e62_0d82
void SKW_GLOBAL_UPDATE_UNKNOW1(void)
{
  if (ddata.savegamewpc.w0 != con(0xffffffff) && (SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(ddata.savegamewpc.w0, con(0x0)) & con(0x8000)) != con(0x0))
  {
    x8 byterg1 = SKW_2405_014a(ddata.savegamewpc.w0, true, con(0xffffffff));
    if (byterg1 != ddata.savegamewpc.b16)
    {
      SKW_DRAW_ITEM_IN_HAND(&ddata.savegamewpc);
      SKW_443c_0434();
      ddata.savegamewpc.b16 = byterg1;
    }
  }
}

// was SKW_2f3f_06fe
void SKW_BRING_CHAMPION_TO_LIFE(x16 eaxw)
{
  c_hero* heroptrrg2 = &ddata.hero[eaxw];
  R_36EFE(eaxw);
  heroptrrg2->wff = con(0x0);
  for (x16 wordrg4 = con(0x0); wordrg4 < con(0x1e); wordrg4++)
    heroptrrg2->warrc3[wordrg4] = con(0xffffffff);
  x16 wordrg11 = DM2_MAX(con(0x19), heroptrrg2->w38 - urshift(heroptrrg2->w38, con(0x6)) - con(0x1));
  heroptrrg2->w38 = wordrg11;
  heroptrrg2->w36 = wordrg11 / 2;
  heroptrrg2->u32.b.b1 |= con(0x40);
  heroptrrg2->b102 = con(0x0);
  heroptrrg2->w103 = con(0x0);
  R_107B0();
  SKW_443c_040e();
}

void SKW_ROTATE_CREATURE(x16 eaxw, x16 ebxw, bool edxbool)
{
  c_record* recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(eaxw);
  x16 wordrg1 = ulrshift(recoptrrg4->ue.w, con(0x6), con(0xe));
  x16 wordrg2 = ebxw;
  if (!edxbool)
  {
    wordrg2 += wordrg1;
    wordrg2 &= con(0x3);
  }
  x16 wordrg5 = (wordrg2 - wordrg1) & con(0x3);
  wordrg2 &= con(0x3);
  recoptrrg4->ue.b.b1 &= con(0xfffffffc);
  wordrg2 <<= con(0x8);
  recoptrrg4->ue.w |= wordrg2;
  if ((SKW_QUERY_CREATURE_AI_SPEC_FLAGS(eaxw) & con(0x1)) != con(0x0))
  {
    x16* wptrrg1 = &recoptrrg4->u2.w;
    while (*wptrrg1 != con(0xfffffffe))
    {
      x16 wordrg4 = *wptrrg1;
      x16 wordrg21 = (((wordrg4 >> con(0xe)) + wordrg5) & con(0x3)) << con(0xe);
      wordrg4 &= con(0x3fff);
      wordrg4 |= wordrg21;
      *wptrrg1 = wordrg4;
      wptrrg1 = &SKW_GET_ADDRESS_OF_RECORD(wordrg4)->w0;
    }
  }
}

x16 SKW_ROTATE_RECORD_BY_TELEPORTER(c_record* eaxprec, x16 edxw)
{
  x16 wordrg1 = edxw;
  x16 wordrg4 = ddata.v1e1024;
  x16 wordrg3 = ulrshift(eaxprec->u2.w, con(0x4), con(0xe));
  if ((eaxprec->u2.b.b1 & con(0x10)) == con(0x0))
  {
    wordrg4 += wordrg3;
    wordrg4 &= con(0x3);
    x16 wordrg2 = wordrg1;
    wordrg1 = ((wordrg3 + (wordrg2 >> con(0xe))) & con(0x3)) << con(0xe);
    wordrg2 &= con(0x3fff);
    wordrg1 |= wordrg2;
  }
  else
    wordrg4 = wordrg3;
  ddata.v1e1024 = wordrg4;
  return wordrg1;
}

bool SKW_IS_OBJECT_FLOATING(x16 eaxw)
{
  x16 wordrg4 = urshift(eaxw & con(0x3c00), con(0xa));
  if (wordrg4 == con(0x4))
    return SKW_IS_CREATURE_FLOATING(eaxw);
  return wordrg4 == con(0xe) || wordrg4 == con(0xf);
}

void SKW_SET_MINION_RECENT_OPEN_DOOR_LOCATION(x16 eaxw, x16 ebxw, x16 ecxw, x8 edxb, bool argbool0)
{
  x16 wordrg4 = SKW_GET_ADDRESS_OF_RECORD(eaxw)->u2.w;
  while (wordrg4 != con(0xfffffffe))
  {
    if (urshift(wordrg4 & con(0x3c00), con(0xa)) == con(0xe))
    {
      c_record* recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg4);
      recoptrrg1->u4.b.b0 &= con(0xffffffe0);
      recoptrrg1->u4.w |= unsignedword(edxb) & con(0x1f);
      recoptrrg1->u4.w &= con(0xfffffc1f);
      recoptrrg1->u4.w |= (ebxw & con(0x1f)) << con(0x5);
      recoptrrg1->u4.b.b1 &= con(0x3);
      recoptrrg1->u4.w |= (ecxw & con(0x3f)) << con(0xa);
      recoptrrg1->u6.b.b0 &= con(0xfffffffe);
      recoptrrg1->u6.w |= argbool0 ? 1 : 0;
    }
    wordrg4 = SKW_GET_NEXT_RECORD_LINK(wordrg4);
  }
}

// like other 2fcf-routines arguments are stored as byte, but given as words
void SKW_2fcf_01c5(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0)
{
  c_tim vtim_04;

  vtim_04.un.l_00 = (ddata.longmallocx + con(0x5)) | (signedlong(ecxw) << con(0x18));
  vtim_04.b_04 = ((argw0 != con(0x0)) ? 1 : 0) + con(0x3c);
  vtim_04.b_05 = con(0x0);
  vtim_04.u.b.b_06 = CUTX8(edxw);
  vtim_04.u.b.b_07 = CUTX8(ebxw);
  vtim_04.v.w_08 = eaxw;
  SKW_SET_MINION_RECENT_OPEN_DOOR_LOCATION(eaxw, ebxw, ecxw, CUTX8(edxw), true);
  SKW_QUEUE_TIMER(&vtim_04);
}

void SKW_INVOKE_ACTUATOR(c_record* eaxprec, x16 ebxw, x16 edxw)
{
  SKW_INVOKE_MESSAGE(ulrshift(eaxprec->u6.w, con(0x5), con(0xb)), CUTX8(ulrshift(eaxprec->u6.w, con(0xa), con(0xe))), edxw, urshift(eaxprec->u6.w, con(0xb)), signedlong(ebxw) + unsignedlong(ulrshift(eaxprec->u4.w, con(0x5), con(0xc))) + ddata.longmallocx);
}

bool SKW_2fcf_164e(x16 eaxw, x16 edxw)
{
  x16 wordrg4 = urshift(eaxw & con(0x3c00), con(0xa));
  if (wordrg4 != con(0x9) || ((SKW_QUERY_CLS2_FROM_RECORD(eaxw) & con(0xff)) >= con(0x8)))
    return false;
  x16 wordrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg4)->u2.w;
  for (;;)
  {
    if (wordrg1 == con(0xfffffffe))
      return false;
    x16 wordrg2 = wordrg1;
    if (SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg2) == edxw)
      return true;
    if (SKW_2fcf_164e(wordrg2, edxw))
      return true;
    wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg2);
  }
}

x16 SKW_FIND_DISTINCTIVE_ITEM_ON_TILE(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  x16 wordrg1 = SKW_GET_WALL_TILE_ANYITEM_RECORD(eaxw, edxw);
  for(;;)
  {
    x16 wordrg4 = wordrg1;
    if (wordrg4 == con(0xfffffffe))
      return con(0xffff);
    x16 wordrg2 = wordrg4;
    if (SKW_GET_DISTINCTIVE_ITEMTYPE(wordrg2) == ecxw)
    {
      if (ebxw == con(0xffffffff))
        return wordrg4;
      wordrg2 >>= con(0xe);
      if (wordrg2 == ebxw)
        return wordrg4;
    }
    wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg4);
  }
}

void R_3BF83(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw)
{
  bool boolrg4 = ebxw != ddata.v1e0266;
  if (boolrg4)
    SKW_CHANGE_CURRENT_MAP_TO(ebxw);
  if (eaxw < con(0x0) || eaxw >= ddata.savegamewa || edxw < con(0x0) || edxw >= ddata.savegamewb)
  {
    if (boolrg4)
      SKW_CHANGE_CURRENT_MAP_TO(ddata.v1e0266);
  }
  else
  {
    x16 wordrg2;
    x16 wordrg4;
    if (!boolrg4)
    {
      wordrg2 = ddata.v1e0272;
      wordrg4 = ddata.v1e0270;
    }
    else
    {
      SKW_CHANGE_CURRENT_MAP_TO(ddata.v1e0266);
      SKW_MOVE_RECORD_TO(con(0xffff), ddata.v1e0272, con(0xffffffff), ddata.v1e0270, con(0x0));
      SKW_LOAD_NEWMAP(eaxw, ebxw, true, edxw);
      wordrg2 = con(0x0);
      wordrg4 = con(0xffffffff);
    }
    SKW_MOVE_RECORD_TO(con(0xffff), wordrg2, eaxw, wordrg4, edxw);
    SKW_ROTATE_SQUAD(ecxw);
  }
}

x16 SKW_QUERY_CREATURE_5x5_POS(c_record* eaxprec, x16 edxw)
{
  s_8bytearray vs_00;

  x16 wordrg1;
  if (SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0xf), con(0x7), con(0xfd), eaxprec->u4.b.b0))
  {
    x16* wptrrg1 = SKW_1c9a_02c3(eaxprec, SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(eaxprec->u4.b.b0));
    R_4DA3(eaxprec->u4.b.b0, wptrrg1 + con(0x1), &vs_00, *wptrrg1);
    wordrg1 = SKW_ROTATE_5x5_POS(vs_00.barr_00[0x4], t_rotate(edxw));
  }
  else
    wordrg1 = con(0xc);
  return wordrg1;
}

bool R_3DC4C(x16 eaxw)
{
  return (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x8), con(0xb), con(0x65), CUTX8(ulrshift(ddata.v1e03c8[eaxw].we, con(0x8), con(0xc)))) & con(0x20)) == con(0x0);
}

bool SKW_32cb_00f1(x16 eaxw, x16 ebxw, x16 edxw)
{
  x16 wordrg31 = eaxw;
  x16 wordrg5 = edxw;
  if (!SKW_PT_IN_RECT(&ddata.v1e118e.rc_24, eaxw, edxw))
    return false;
  if ((ddata.v1e118e.w_32 & con(0x1)) == con(0x0))
  {
    wordrg31 -= ddata.v1e118e.rc_24.x;
    wordrg31 += ddata.v1e118e.w_20;
  }
  else
    wordrg31 = ddata.v1e118e.w_20 + ddata.v1e118e.rc_24.x + ddata.v1e118e.rc_24.w - con(0x1) - wordrg31;
  wordrg5 += ddata.v1e118e.w_22 - ddata.v1e118e.rc_24.y;
  t_gfxdata* gptrrg1 = SKW_QUERY_PICT_BITMAP(&ddata.v1e118e.s_00);
  s_gfxdatatail* ptrbeforerg1 = getprevious(gptrrg1);
  x16 wordrg1 = signedlong(ptrbeforerg1->width) / 2;
  ddata.v1e12da = 100 * ((wordrg31 - ddata.v1e118e.s_00.w_0e) - wordrg1) / wordrg1;
  x16 wordrg32 = ptrbeforerg1->height / 2;
  ddata.v1e12d8 = 100 * (wordrg5 - wordrg32) / wordrg32;
  x16 wordrg11 = SKW_0b36_1446(&ddata.v1e118e.s_00, wordrg5, ebxw, wordrg31);
  if (wordrg11 != con(0xffffffff) && wordrg11 != con(0x1))
    return true;
  return false;
}

t_gfxdata* SKW_32cb_0649(x8 eaxb, x8 ebxb, x16 ecxw, x8 edxb)
{
  t_palette* gptrrg1 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(eaxb, ebxb, edxb);
  x16 vw_00;
  if (gptrrg1 != NULL)
  {
    SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, gptrrg1), PAL16, DOWNCAST(t_gfxdata, ddata.v1e12dc));
    vw_00 = con(0x10);
  }
  else
  {
    if (ddata.v1e12d2 == con(0x3f))
      return NULL;
    vw_00 = con(0x0);
  }
  return R_B073(ddata.v1e12dc, ecxw, con(0xffffffff), CUTX8(ddata.v1e12d2), &vw_00);
}

// TODO: check built-in, there was a problem
void SKW_32cb_0804(t_gfxdata* eaxpg, x16 ebxw, x16 ecxw, x16 edxw, x16* argpw0)
{
  x16 vw_00 = edxw;
  x16 vw_04;
  if (ddata.v1e025c == con(0x0))
    vw_04 = signedword(table1d6c10[vw_00]);
  else
  {
    if (edxw >= con(0x0))
    {
      vw_04 = signedword(table1d6c0b[vw_00]);
      vw_00 += con(0x9);
    }
    else
    {
if ((-vw_00-1) < 0)
  int stop=1;
      vw_04 = DM2_MAX(-ddata.v1e12d2, signedword(table1d6c06[-vw_00 - con(0x1)])); // EVIL offset corrected, was 0x1d6c05, vw_00 must be negative here
      vw_00 = con(0x1);
    }
  }
  if (SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0x8), con(0x7), CUTX8(vw_00), ddata.v1d6c02))
    SKW_0b36_037e(eaxpg, ddata.v1d6c02, CUTX8(vw_00), con(0x8), CUTX8(ddata.v1e12d2), ebxw, ecxw, argpw0);
  else
    R_B073(eaxpg, ebxw, ecxw, CUTX8(con(0x40) - (((con(0x40) - vw_04) * (con(0x40) - ddata.v1e12d2)) >> con(0x6))), argpw0); // BUGFIX 4/2014 0x6 instead of 0x8
}

// was SKW_32cb_0b11
void SKW_DRAW_RAIN(void)
{
  c_rect rc_00;

  if (ddata.v1e1482 != con(0x0) && table1d6b76[4 * ddata.v1e1472 + 0x72] != con(0x0))
  {
    x16 vw_10 = ddata.vx1d270a;
    x16 vw_08 = ddata.vx1d270c;
    if (SKW_QUERY_BLIT_RECT(NULL, con(0x2be), &vw_10, &rc_00, &vw_08, con(0xffffffff)) != NULL)
    {
      x16 vw_0c;
      x8 vb_14;
      SKW_QUERY_RAINFALL_PARAM(&vb_14, &vw_0c);
      t_gfxdata* gptrrg5 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x17), vb_14, ddata.v1d6c02);
      s_gfxdatatail* ptrbeforerg3 = getprevious(gptrrg5);
      SKW_3e74_58bf(con(0x17), vb_14, ddata.v1d6c02);
      x16 wordrg6;
      if (vw_0c == con(0x1))
      {
        wordrg6 = SKW_3e74_5888();
        SKW_ALLOC_NEW_PICT(wordrg6, ptrbeforerg3->height, con(0x4), ptrbeforerg3->width);
        SKW_DRAW_MIRRORED_PIC(SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x17), vb_14, ddata.v1d6c02), SKW_3e74_5817(wordrg6));
        gptrrg5 = SKW_3e74_5817(wordrg6);
      }
      s_gfxdatatail* ptrbefore = getprevious(ddata.bitmapptr);
      s_gfxdatatail* ptrbeforerg5 = getprevious(gptrrg5);
      SKW_44c8_20a4(gptrrg5, NULL, &rc_00, UPCAST(t_gfxdata, ddata.bitmapptr), (ptrbeforerg5->height * ptrbeforerg5->width) - con(0x28) - CUTX16(SKW_RAND() & con(0x1f)), unsignedword(CUTX8(SKW_RAND())), ptrbefore->width, con(0x0), SKW_32cb_0649(con(0x17), vb_14, con(0x0), ddata.v1d6c02));
      if (vw_0c == con(0x1))
        SKW_3e74_58a3(wordrg6);
      SKW_3e74_5992(con(0x17), vb_14, ddata.v1d6c02);
    }
  }
}

void SKW_32cb_0c7d(c_image* eaxpimg, x16 ebxw, x16 edxw)
{
  if (ddata.v1e1482 != con(0x0))
  {
    x16 vw_04;
    x8 vb_1c;
    SKW_QUERY_RAINFALL_PARAM(&vb_1c, &vw_04);
    x16 vw_0c;
    x16 vw_14;
    SKW_QUERY_GDAT_IMAGE_METRICS(con(0x17), vb_1c, &vw_14, ddata.v1d6c02, &vw_0c);
    x16 vw_18 = SKW_3e74_5888();
    SKW_ALLOC_NEW_PICT(vw_18, vw_0c, con(0x4), eaxpimg->rc_24.w);
    x16 wordrg31 = (eaxpimg->rc_24.w + con(0x1)) & con(0xfffffffe);
    t_gfxdata* srcbmp = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x17), vb_1c, ddata.v1d6c02); // was gptrrg61
    s_gfxdatatail* ptrbeforerg61 = getprevious(srcbmp);
//                        srcbmp                                                          srcofs  destbmp                                                                                                        srcbits     destbits
    SKW_FIRE_BLIT_PICTURE(srcbmp, SKW_ALLOC_TEMP_ORIGIN_RECT(wordrg31, ptrbeforerg61->height), 0, SKW_3e74_5817(vw_18), con(0x0), ptrbeforerg61->width, wordrg31, con(0xffff), (signedlong(vw_04) == con(0x1)) ? 1 : 0, E_BITRES4, E_BITRES4);
    t_gfxdata* gptrrg62 = SKW_3e74_5817(vw_18);
    s_gfxdatatail* ptrbeforerg62 = getprevious(gptrrg62);
    t_gfxdata* parpg13 = SKW_32cb_0649(con(0x17), vb_1c, con(0x0), ddata.v1d6c02);
    parpg13[0x0] = (eaxpimg->w_38 > con(0x0)) ? eaxpimg->xLocalPalette[ebxw] : CUTX8(ebxw);
    x16 wordrg3 = ptrbeforerg62->width * ptrbeforerg62->height;
    SKW_44c8_20a4(gptrrg62, NULL, &eaxpimg->rc_24, UPCAST(t_gfxdata, SKW_3e74_5817(edxw)), wordrg3 - CUTX16(SKW_RAND() & con(0x1f)) - con(0x10), SKW_RAND16(wordrg3 - con(0x3c)), eaxpimg->s_00.width, con(0xffff), parpg13);
    SKW_3e74_58a3(vw_18);
  }
  else
  {
    x8 byterg1;
    if (eaxpimg->w_38 > con(0x0))
      byterg1 = unsignedlong(eaxpimg->xLocalPalette[ebxw]);
    else
      byterg1 = CUTX8(ebxw);
    SKW_FILL_ENTIRE_PICT(SKW_3e74_5817(edxw), byterg1);
  }
}

void SKW_DRAW_TEXT_TO_BACKBUFF(x16 eaxw, t_text* ebxt, x16 edxw)
{
  t_gfxdata vcarr_00[PAL16];

  x16 vw_10 = con(0x10);
  x16 vw_14;
  x16 vw_18;
  if (C_strmetrics.SKW_QUERY_STR_METRICS(ebxt, OO vw_18, OO vw_14))
  {
    SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, ddata.paletteptr1), PAL16, DOWNCAST(t_gfxdata, vcarr_00));
    R_B073(vcarr_00, con(0xffffffff), con(0xffffffff), CUTX8(ddata.v1e12d2), &vw_10);
    s_gfxdatatail* ptrbefore = getprevious(ddata.bitmapptr);
    SKW_DRAW_STRONG_TEXT(ddata.bitmapptr, eaxw - (vw_14 >> con(0x1)), edxw, ptrbefore->width, unsignedword(vcarr_00[0xf]), unsignedword(vcarr_00[0x0]) | con(0x4000), ebxt);
  }
}

// was SKW_32cb_2367
void SKW_DRAW_PIT_ROOF(ui16 eaxuw)
{
  x8 vb_0c = ddata.v1d6c02;
  x16 vw_08 = ddata.v1d6c00;
  if (eaxuw <= con(0x8) && (ddata.v1e12d6 & con(0x1)) != con(0x0))
  {
    s_hex12* h12ptrrg4 = &ddata.v1e1044[eaxuw];
    x16 vw_00 = signedword(h12ptrrg4->b_00);
    x16 vw_04 = signedword(h12ptrrg4->b_01);
    x16 wordrg11 = SKW_LOCATE_OTHER_LEVEL(ddata.v1d3248, &vw_00, &vw_04, con(0xffffffff), NULL);
    if (wordrg11 >= con(0x0))
    {
      x8** hlpptr1 = ddata.v1e03cc[wordrg11];
      x8* hlpptr2 = hlpptr1[vw_00];
      x8 byterg4 = hlpptr2[vw_04];
      if (urshift(unsignedword(byterg4), con(0x5)) == con(0x2))
        if ((byterg4 & con(0x8)) != con(0x0))
        {
          x16 wordrg1 = unsignedword(table1d6c67[eaxuw]);
          if (eaxuw == con(0x0))
            wordrg1 = SKW_SET_GRAPHICS_FLIP_FROM_POSITION(con(0x20), ddata.v1e12cc, ddata.v1e12ca, ddata.v1e12c8) != con(0x0) ? 1 : 0;
          SKW_DRAW_DUNGEON_GRAPHIC(con(0x8), table1d6c5e[eaxuw], table1d6c4c[eaxuw], vb_0c, vw_08, wordrg1);
        }
    }
  }
}

// was SKW_32cb_245a
void SKW_DRAW_PIT_TILE(ui16 eaxuw)
{
  x8 vb_04 = ddata.v1d6c02;
  x16 vw_00 = ddata.v1d6c00;
  x16 wordrg6 = table1d6c70[eaxuw];
  if (wordrg6 < con(0x0))
    return;
  if (eaxuw >= con(0xb))
    if (ddata.v1e1044[eaxuw].s_02.warr_00[con(0x3)] != con(0x0))
      return;
  x16 wordrg1 = unsignedword(table1d6c90[eaxuw]);
  if (eaxuw == con(0x0))
    wordrg1 = SKW_SET_GRAPHICS_FLIP_FROM_POSITION(con(0x1), ddata.v1e12cc, ddata.v1e12ca, ddata.v1e12c8) != con(0x0) ? 1 : 0;
  x8 byterg1;
  if (ddata.v1e1044[eaxuw].s_02.warr_00[con(0x3)] == con(0x0))
    byterg1 = table1d6cb0[eaxuw];
  else
    byterg1 = table1d6ca0[eaxuw];
  SKW_DRAW_DUNGEON_GRAPHIC(con(0x8), byterg1, wordrg6, vb_04, vw_00, wordrg1);
}

// was SKW_32cb_24fb
void SKW_DRAW_TELEPORTER_TILE(x16 eaxw, x8 ebxb, x8 edxb)
{
  const s_4bytearray* s4baptrrg5;
  t_gfxdata* srcbmp; // was gptrrg6
  t_gfxdata* destbmp; // was gptrrg7
  c_hex18 vh18_00;
  c_rect rc_18;
  x16 vw_24;
  x16 vw_28;
  x16 vw_2c;
  x16 vw_30;
  x16 vw_34;
  x16 vw_38;
  x16 vw_3c;
  x8 vb_40;
  x8 vb_44;
  x8 vb_48;

  vb_44 = edxb;
  vb_48 = ebxb;
  if (eaxw > con(0xf))
    return;
  s4baptrrg5 = &table1d6ce0[eaxw];
  x16 wordrg4 = table1d6cc0[eaxw];
  vw_2c = con(0x0);
  vb_40 = s4baptrrg5->barr_00[con(0x1)] & con(0x7f);
  vw_30 = ((s4baptrrg5->barr_00[con(0x1)] & con(0xffffff80)) != con(0x0)) ? 1 : 0;
  vw_24 = unsignedword(s4baptrrg5->barr_00[con(0x2)]);
  vw_28 = unsignedword(s4baptrrg5->barr_00[con(0x3)]);
  if (SKW_QUERY_BLIT_RECT(NULL, wordrg4, &vw_24, &rc_18, &vw_28, con(0xffffffff)) != NULL)
  {
    if (s4baptrrg5->barr_00[con(0x1)] != con(0xffffffff))
    {
      vh18_00.b_08 = vb_44;
      vh18_00.b_09 = vb_48;
      vh18_00.b_0b = vb_40;
      srcbmp = SKW_QUERY_PICST_IMAGE(&vh18_00);
      s_gfxdatatail* ptrbeforerg4 = getprevious(srcbmp);
      SKW_0b36_018f(&vh18_00);
      if (vw_30 != con(0x0))
      {
        vw_34 = SKW_3e74_5888();
        vw_38 = ptrbeforerg4->height;
        vw_3c = ptrbeforerg4->width;
        destbmp = SKW_ALLOC_NEW_PICT(vw_34, vw_38, con(0x4), vw_3c);
        srcbmp = SKW_QUERY_PICT_BITMAP(&vh18_00);
        s_gfxdatatail* ptrbeforerg41 = getprevious(srcbmp);
        x16 wordrg1 = ptrbeforerg41->width & con(0x1);
        if (wordrg1 != con(0x0))
          wordrg1 = con(0x2) - wordrg1;
        wordrg1 = vw_3c - wordrg1;
//                            srcbmp                                            srcofs destbmp                                               srcbits    destbits
        SKW_FIRE_BLIT_PICTURE(srcbmp, SKW_ALLOC_TEMP_ORIGIN_RECT(wordrg1, vw_38), 0, destbmp, con(0x0), vw_3c, vw_3c, con(0xffff), con(0x1), E_BITRES4, E_BITRES4);
        SKW_0b36_01cd(&vh18_00);
        SKW_0b36_00c3(vw_34, &vh18_00);
        SKW_0b36_018f(&vh18_00);
      }
    }
    else
      srcbmp = NULL;
    destbmp = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(vb_44, con(0x14), vb_48);
    if (s4baptrrg5->barr_00[con(0x1)] != con(0xffffffff))
      srcbmp = SKW_QUERY_PICT_BITMAP(&vh18_00);
    s_gfxdatatail* ptrbefore = getprevious(ddata.bitmapptr);
    SKW_44c8_20a4(destbmp, srcbmp, &rc_18, UPCAST(t_gfxdata, ddata.bitmapptr), 16 * (SKW_RAND01() + unsignedword(s4baptrrg5->barr_00[con(0x0)])), unsignedword(CUTX8(SKW_RAND())), ptrbefore->width, CUTLX8(vw_2c), SKW_32cb_0649(vb_44, con(0x14), vw_2c, vb_48));
    if (s4baptrrg5->barr_00[con(0x1)] != con(0xffffffff))
      SKW_0b36_01cd(&vh18_00);
  }
}

bool SKW_SUMMARY_DRAW_CREATURE(x16 eaxw, x32 ebxl, x16 edxw)
{
  s_d556* sd5ptrrg3;
  c_aispec* aiptrrg7;
  c_record* recoptrrg5;
  x16 vw_08;

  x16 wordrg6 = eaxw;
  x16 vw_0c = edxw;
  x32 vl_04 = ebxl;
  bool vbool_00 = false;
  if (ddata.v1e025c == con(0x0) || vw_0c != con(0x0))
  {
    vw_08 = signedword(table1d6b15[vw_0c]);
    while (wordrg6 != con(0xfffffffe))
    {
      if (urshift(wordrg6 & con(0x3c00), con(0xa)) == con(0x4))
      {
        recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
        if (recoptrrg5->u4.b.b1 != con(0xffffffff))
          sd5ptrrg3 = &ddata.v1e089c[recoptrrg5->u4.b.b1];
        else
          sd5ptrrg3 = NULL;
        aiptrrg7 = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(recoptrrg5->u4.b.b0);
        if ((vl_04 & con(0x40000000)) == con(0x0) || (aiptrrg7->u0.b.b0 & con(0x1)) != con(0x0))
        {
          vbool_00 = true;
          SKW_QUERY_CREATURE_PICST(vw_0c, recoptrrg5, sd5ptrrg3, vw_08, wordrg6);
          SKW_DRAW_TEMP_PICST();
          x16 wordrg1 = aiptrrg7->u0.w & con(0x1);
          if (wordrg1 == con(0x0))
            recoptrrg5->ua.b.b0 |= con(0x2);
          if ((wordrg1 != con(0x0) && ((aiptrrg7->u1e.b.b0 & con(0xf)) != con(0x0))) && (vw_08 < con(0x4)))
          {
            if (ddata.v1e1044[vw_0c].s_02.warr_00[con(0x0)] == con(0x11))
              vl_04 = con(0x3ff);
            SKW_32cb_3e08(recoptrrg5->u2.w, vl_04, recoptrrg5, vw_0c);
          }
        }
      }
      wordrg6 = SKW_GET_NEXT_RECORD_LINK(wordrg6);
    }
  }
  return vbool_00;
}

// was SKW_32cb_347f
void SKW_MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE(x16 eaxw, x8 ebxb, x8 ecxb, x16 edxw)
{
  c_rect* rptrrg3 = &ddata.v1e02f0[eaxw].r0;
  if (ddata.v1e02f0[eaxw].w8 != con(0xffffffff))
  {
    x16 wordrg2 = DM2_MAX(rptrrg3->x + rptrrg3->w, ddata.v1e118e.rc_24.x + ddata.v1e118e.rc_24.w);
    rptrrg3->x = DM2_MIN(rptrrg3->x, ddata.v1e118e.rc_24.x);
    wordrg2 -= rptrrg3->x;
    rptrrg3->w = wordrg2;
    x16 wordrg21 = DM2_MAX(rptrrg3->y + rptrrg3->h, ddata.v1e118e.rc_24.y + ddata.v1e118e.rc_24.h);
    rptrrg3->y = DM2_MIN(rptrrg3->y, ddata.v1e118e.rc_24.y);
    wordrg21 -= rptrrg3->y;
    rptrrg3->h = wordrg21;
  }
  else
  {
    SKW_COPY_RECT(&ddata.v1e118e.rc_24, rptrrg3);
    if (rptrrg3->h < con(0xf))
    {
      if (rptrrg3->h < con(0xb))
        rptrrg3->h = con(0xb);
      rptrrg3->y += rptrrg3->h / 2 - con(0x7);
    }
    if (rptrrg3->w < con(0xe))
    {
      rptrrg3->w = con(0xe);
      rptrrg3->x += rptrrg3->w / 2 - con(0x7);
    }
  }
  ddata.v1e02f0[eaxw].bb = ecxb;
  ddata.v1e02f0[eaxw].w8 = edxw;
  ddata.v1e02f0[eaxw].ba = ebxb;
}

// was SKW_32cb_4e1c
void SKW_DRAW_STAIRS_FRONT(ui16 eaxuw)
{
  x8 byterg5;
  x16 vw_00;
  x16 vw_04;

  x8 byterg4 = ddata.v1d6c02;
  vw_04 = ddata.v1d6c00;
  x16 wordrg6 = (ddata.v1e1044[eaxuw].s_02.warr_00[con(0x3)] != con(0x0)) ? 1 : 0;
  vw_00 = table1d6f9c[wordrg6 + 2 * eaxuw];
  if (vw_00 >= con(0x0))
  {
    wordrg6 += 2 * eaxuw;
    x8 byterg7 = table1d6f5c[wordrg6];
    byterg5 = byterg4;
    if (!SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(con(0x8), con(0x1), byterg7, byterg5))
    {
      SKW_QUERY_TEMP_PICST(con(0x1), con(0x40), con(0x0), con(0x40), con(0x0), con(0x0), vw_00, con(0xffffffff), vw_04, con(0xffff), con(0x8), byterg5, table1d6f7c[wordrg6]); // BUGFIX 3/2014
      SKW_DRAW_TEMP_PICST();
    }
    else
      SKW_DRAW_DUNGEON_GRAPHIC(con(0x8), byterg7, vw_00, byterg5, vw_04, con(0x0));
  }
}

// was SKW_32cb_4ecc
void SKW_DRAW_STAIRS_SIDE(ui16 eaxuw)
{
  x8 vb_00;

  vb_00 = ddata.v1d6c02;
  x16 wordrg3 = ddata.v1d6c00;
  if (eaxuw <= con(0x8))
  {
    x16 wordrg21 = (ddata.v1e1044[eaxuw].s_02.warr_00[con(0x3)] != con(0x0)) ? 1 : 0;
    x8 byterg4 = table1d6fdc[2 * eaxuw + wordrg21];
    if (byterg4 != con(0xffffffff))
      SKW_DRAW_DUNGEON_GRAPHIC(con(0x8), byterg4, table1d6fee[2 * eaxuw + wordrg21], vb_00, wordrg3, con(0x0));
  }
}

void SKW_38c8_0002(void)
{
  if (!ddata.v1e13f0 && !ddata.dialog2)
  {
    ddata.v1e13f0 = true;
    if (ddata.v1e00b8 != con(0x0))
    {
      SKW_CHAMPION_SQUAD_RECOMPUTE_POSITION();
      SKW_UPDATE_RIGHT_PANEL(false);
    }
    SKW_1031_04F5();
    SKW_FIRE_HIDE_MOUSE_CURSOR();
    SKW_12b4_0092();
    if (ddata.v1e0976 == con(0x0))
      SKW_FIRE_FILL_HALFTONE_RECTI(con(0x9), con(0x0));
    SKW_FIRE_FILL_HALFTONE_RECTI(con(0xb), con(0x0));
    SKW_FIRE_SHOW_MOUSE_CURSOR();
  }
}

void SKW_38c8_0060(void)
{
  if (ddata.v1e13f0 && !ddata.dialog2)
  {
    if (ddata.v1e0238)
    {
      SKW_DRAW_WAKE_UP_TEXT();
      SKW_CHANGE_VIEWPORT_TO_INVENTORY(false);
      return;
    }
    ddata.v1e13f0 = ddata.dialog2;
    if (ddata.v1e0b74 != con(0x0))
      ddata.v1e0b6c = con(0x1);
    else
      ddata.v1e0b30 = true;
    x16 wordrg1 = ddata.v1e0976;
    if (wordrg1 != con(0x0))
    {
      ddata.v1e0976 = con(0x0);
      SKW_24a5_1798(wordrg1 - con(0x1));
      return;
    }
    SKW_INIT_BACKBUFF();
    SKW_29ee_000f();
  }
}

// id: 0x453C1
// was SKW_38c8_04aa
void SKW_INIT(void)
{
	SPX_DEBUG_PUSH_FUNCTION_INFO("SKW_INIT");

  x8* bptrrg5;
  DM2_2636_03d4();
  SKW_READ_GRAPHICS_STRUCTURE();
  SKW_482b_0004();
  bptrrg5 = UPCAST(x8, DM2_ALLOC_HIBIGPOOL_MEMORY(con(0x400)));
DEBUG_DUMP_ULP();
  SKW_LOAD_GDAT_ENTRY_DATA_TO(con(0x1), con(0x9), con(0xfe), con(0x0), bptrrg5);
DEBUG_DUMP_BIN_DATA((unsigned char*) bptrrg5, 1024, 16);	// SPX: dump the IRGB palette
  SKW_44c8_1baf(bptrrg5);
  DM2_DEALLOC_HIBIGPOOL(con(0x400));
  SKW_LOAD_GDAT_INTERFACE_00_02();
  ddata.paletteptr1 = UPCAST(t_gfxdata, SKW_QUERY_GDAT_ENTRY_DATA_PTR(con(0x1), con(0xd), con(0xfe), con(0x0)));
  SKW_098d_1208();
  SKW_38c8_00c8();
  SKW_3929_0e16();
  ddata.v1e0984 = ddata.v1e0ad0 & con(0x8);
  SKW_2405_0009();
  SKW_443c_0380();
  ddata.v1d70cb = true;
  SKW_1031_07d6();
  ddata.v1e0414[0x11] = SKW_3e74_2439(con(0xf), con(0xb));
  x16 wordrg1 = (unsignedword(ddata.v1e0414[0x11]) + con(0x1)) * con(0x3);
  ddata.v1e02ec = UPCAST(x8, DM2_ALLOC_FREEPOOL_MEMORY(unsignedlong(wordrg1)));
  SKW_FILL_STR(ddata.v1e02ec, con(0xff), con(0x1), wordrg1);
  for(;;)
  {
    SKW_SHOW_MENU_SCREEN();
    if (SKW_GAME_LOAD() == con(0x1))
      break;
    DM2_GRAPHICS_DATA_OPEN();
  }
  DM2_GRAPHICS_DATA_OPEN();
  R_5BFB(ddata.v1d26a0, con(0x0));
  R_5BFB(ddata.v1d26a2, con(0xa));
  SKW_1c9a_3c30();
  SKW_ALLOC_CPX_SETUP(table1d70b4);
  SKW__INIT_GAME_38c8_03ad();
  DM2_GRAPHICS_DATA_CLOSE();
  if (ddata.savegamew8 != con(0x0))
    SKW_MOVE_RECORD_TO(con(0xffff), con(0x0), ddata.v1e0270, con(0xffffffff), ddata.v1e0272);
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_1031_098e();

	SPX_DEBUG_POP;
}

void SKW_3929_07e1(x16 eaxw, x16 edxw)
{
  x16 wordrg1 = eaxw;
  if (wordrg1 < con(0x0))
    wordrg1 = con(0x0);
  wordrg1 *= C_strmetrics.charwidth;
  ddata.v1d70ea = DM2_MIN(wordrg1, ddata.v1d2744 - C_strmetrics.charwidth);
  x16 wordrg2 = edxw;
  if (wordrg2 >= con(0x0))
  {
    if (wordrg2 >= con(0x3))
      wordrg2 = con(0x2);
  }
  else
    wordrg2 = con(0x0);
  ddata.v1e141e = wordrg2;
  ddata.v1e1420 = con(0x0);
}

x16 SKW_3929_0826(void)
{
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_FIRE_FILL_SCREEN_RECT(con(0xf), ddata.paletteptr1[con(0x0)]);
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_3929_07e1(con(0x0), con(0x2));
  for (x16 wordrg1 = con(0x0); ; wordrg1++)
  {
    if (wordrg1 >= con(0x3))
      return wordrg1;
    ddata.v1e1408[wordrg1] = con(0xffffffff);
  }
}

void SKW_3929_086f(void)
{
  c_rect rc_00;

  SKW_QUERY_EXPANDED_RECT(con(0xf), &rc_00);
  x16 wordrg5 = rc_00.y; // adj
  for (x16 wordrg3 = con(0x0); wordrg3 < con(0x3); wordrg3++)
  {
    ui32 ulongrg1 = ddata.v1e1408[wordrg3];
    if (ulongrg1 != con(0xffffffff) && ulongrg1 <= UI32CAST(ddata.longmallocx))
    {
      rc_00.y = ddata.v1e141c + ddata.strxplus * wordrg3 + wordrg5;
      rc_00.h = C_strmetrics.height;
      SKW_FIRE_HIDE_MOUSE_CURSOR();
      SKW_44c8_1dfc(&rc_00, ddata.paletteptr1[0]);
      SKW_FIRE_SHOW_MOUSE_CURSOR();
      ddata.v1e1408[wordrg3] = con(0xffffffff);
    }
  }
}

void SKW_3929_0914(void)
{
  if (ddata.v1e141e != con(0x2))
  {
    ddata.v1e1420 = con(0x0);
    ddata.v1e141e++;
    return;
  }
  if (ddata.v1e1420 != con(0x0))
    SKW_44c8_1c99();
  SKW_FILL_ENTIRE_PICT(ddata.v1e1414, ddata.paletteptr1[0]);
  ddata.v1e1420 = con(0x1);
  x16 wordrg4 = con(0x0);
  for(;;)
  {
    if (wordrg4 >= con(0x2))
    {
      ddata.v1e1408[2] = con(0xffffffff);
      return;
    }
    ddata.v1e1408[wordrg4] = ddata.v1e1408[wordrg4 + con(0x1)];
    wordrg4++;
  }
}

void SKW_3929_0929(x16 eaxw, t_text* edxt)
{
  c_rect rc_00;

  SKW_QUERY_EXPANDED_RECT(con(0xf), &rc_00);
  if (ddata.v1e1420 == con(0x0))
  {
    SKW_FIRE_HIDE_MOUSE_CURSOR();
    SKW_PRINT_SYSERR_TEXT(signedlong(I16CAST(rc_00.x + ddata.v1d70ea)), eaxw, unsignedword(ddata.paletteptr1[con(0x0)]), ddata.v1e141e * ddata.strxplus + rc_00.y + C_strmetrics.height - C_strmetrics.yofs - con(0x1) + ddata.v1e141c, edxt);
    SKW_FIRE_SHOW_MOUSE_CURSOR();
  }
  else
//                     destbmp
    SKW_DRAW_STRING(ddata.v1e1414, ddata.v1d70ea, ddata.strxplus - C_strmetrics.yofs - con(0x2), ddata.v1d2744, eaxw, unsignedword(ddata.paletteptr1[con(0x0)]), edxt);
  ddata.v1e1408[ddata.v1e141e] = ddata.longmallocx + con(0x5a);
}

// was SKW_3929_09fb
void SKW_DISPLAY_HINT_TEXT(x16 eaxw, const t_text* edxt)
{
  t_text t_00[0x64];
  x16 vw_64;

  if (ddata.v1e1420 != con(0x0))
  {
    SKW_44c8_1c99();
    SKW_FILL_ENTIRE_PICT(ddata.v1e1414, ddata.paletteptr1[con(0x0)]);
    ddata.v1e1420 = con(0x0);
  }
  x16 wordrg6 = unsignedword(ddata.paletteptr1[eaxw]);
  vw_64 = con(0x0);
  for(;;)
  {
    if (edxt[vw_64] == con(0x0))
    {
      if (ddata.v1e1420 != con(0x0))
      {
        SKW_44c8_1c99();
        SKW_FILL_ENTIRE_PICT(ddata.v1e1414, ddata.paletteptr1[0]);
        ddata.v1e1420 = con(0x0);
      }
      return;
    }
    x16 wordrg1 = SKW_3929_04e2(edxt, &vw_64, ddata.v1d2744 - ddata.v1d70ea, t_00);
    if (edxt[vw_64] != con(0xa))
    {
      if (edxt[vw_64] == con(0x0))
      {
        SKW_3929_0929(wordrg6, t_00);
        ddata.v1d70ea += wordrg1;
      }
      else
      {
        SKW_3929_0929(wordrg6, t_00);
        SKW_3929_0914();
        ddata.v1d70ea = con(0xc);
      }
    }
    else
    {
      SKW_3929_0914();
      SKW_3929_0929(wordrg6, t_00);
      ddata.v1d70ea = con(0x0);
      vw_64++;
    }
  }
}

void SKW_3929_0e16(void)
{
  c_rect rc_00;

  SKW_3929_07e1(con(0x0), con(0x2));
  ddata.v1e1414 = SKW_ALLOC_PICT_BUFF(ddata.v1d2744, FREEPOOL, con(0x8), ddata.v1d2746);
  C_font.allocate();
  for (x16 wordrg1 = con(0x0); wordrg1 < con(0x3); wordrg1++)
    ddata.v1e1408[wordrg1] = con(0xffffffff);
  SKW_QUERY_EXPANDED_RECT(con(0xf), &rc_00);
  ddata.v1e141c = (rc_00.h - (3 * ddata.strxplus - ddata.v1d2736)) / 2;
  ddata.v1d70dc = true;
}

void SKW_REARRANGE_TIMERLIST(void)
{
  x16 wordrg5;

  ddata.savegamew9 = con(0x0);
  ddata.v1e142a = con(0xffffffff); // forces wordrg5 to be initialized
  for (x16 wordrg1 = con(0x0); wordrg1 < ddata.v1e142c; wordrg1++)
  {
    if (ddata.savegametimarrp[wordrg1].b_04 != con(0x0))
      ddata.savegamew9 = wordrg1 + con(0x1);
    else
    {
      if (ddata.v1e142a != con(0xffffffff))
        ddata.savegametimarrp[wordrg5].un.w_00 = wordrg1;
      else
        ddata.v1e142a = wordrg1;
      ddata.savegametimarrp[wordrg1].un.w_00 = con(0xffffffff);
      wordrg5 = wordrg1;
    }
  }
}

bool SKW_3a15_0381(c_tim* eaxptim, c_tim* edxptim)
{
  ui32 ulongrg2 = eaxptim->un.l_00 & con(0xffffff);
  ui32 ulongrg3 = edxptim->un.l_00 & con(0xffffff);
  if (ulongrg2 < ulongrg3 || (ulongrg2 == ulongrg3 && (UI8CAST(eaxptim->b_04) > UI8CAST(edxptim->b_04) || (eaxptim->b_04 == edxptim->b_04 && (UI8CAST(eaxptim->b_05) > UI8CAST(edxptim->b_05) || (eaxptim->b_05 == edxptim->b_05 && eaxptim <= edxptim))))))
    return true;
  return false;
}

// was SKW_3a15_061a
void SKW_DELETE_TIMER(x16 eaxw)
{
  x16* wptrrg3;
  bool flag;

  if (ddata.v1e1428 >= con(0x0))
    SKW_3a15_0486(ddata.v1e1428);
  x16 wordrg2 = eaxw;
  ddata.savegametimarrp[wordrg2].b_04 = con(0x0);
  ddata.savegametimarrp[wordrg2].un.w_00 = ddata.v1e142a;
  ddata.v1e142a = eaxw;
  x16 wordrg3 = ddata.savegamew5;
  flag = --wordrg3 == con(0x0);
  ddata.savegamew5 = wordrg3;
  if (!flag)
  {
    x16 wordrg1 = SKW_GET_TIMER_NEW_INDEX(wordrg2);
    if (wordrg1 != wordrg3)
    {
      wptrrg3 = &ddata.v1e1424[wordrg3];
      ddata.v1e1428 = wordrg1;
      ddata.v1e1424[wordrg1] = *wptrrg3;
    }
  }
}

x16 SKW_QUEUE_TIMER(c_tim* eaxptim)
{
  if (eaxptim->b_04 == con(0x0))
    return con(0xffffffff);
  if (ddata.savegamew5 == ddata.v1e142c)
    SKW_RAISE_SYSERR(con(0x2d));
  x16 wordrg4 = ddata.v1e142a;
  ddata.v1e142a = ddata.savegametimarrp[wordrg4].un.w_00;
  SKW_COPY_TIMER(eaxptim, &ddata.savegametimarrp[wordrg4]);
  if (wordrg4 >= ddata.savegamew9)
    ddata.savegamew9 = wordrg4 + con(0x1);
  x16 wordrg1 = ddata.v1e1428;
  if (wordrg1 < con(0x0))
    wordrg1 = ddata.savegamew5;
  ddata.v1e1428 = con(0xffffffff);
  ddata.savegamew5++;
  ddata.v1e1424[wordrg1] = wordrg4;
  SKW_3a15_0486(wordrg1);
  return wordrg4;
}

void SKW_ACTUATE_TRICKWALL(c_tim* eaxptim)
{
  x8* bptrrg3;
  x8** bpptrrg4;

  bool boolrg5 = false;
  x16 wordrg6 = unsignedword(eaxptim->u.b.b_06);
  bpptrrg4 = ddata.savegamepp1;
  x16 wordrg42 = unsignedword(eaxptim->u.b.b_07);
  bptrrg3 = bpptrrg4[eaxptim->u.b.b_06] + eaxptim->u.b.b_07;
  x16 wordrg1 = unsignedword(eaxptim->v.b.b_09);
  if (wordrg1 == con(0x2))
    wordrg1 = ((*bptrrg3 & con(0x4))) ? con(0x1) : con(0x0);
  if (wordrg1 != con(0x1))
    *bptrrg3 |= con(0x4);
  else
  {
    if (ddata.v1d3248 == ddata.v1e0282 && wordrg6 == ddata.v1e0280)
      if (wordrg42 == ddata.v1e027e)
        boolrg5 = true;
    x16 wordrg41 = SKW_GET_CREATURE_AT(wordrg6, wordrg42);
    if (wordrg41 != con(0xffffffff))
      if ((SKW_QUERY_CREATURE_AI_SPEC_FLAGS(wordrg41) & con(0x20)) == con(0x0))
        boolrg5 = true;
    if (!boolrg5)
      *bptrrg3 &= con(0xfffffffb);
    else
    {
      eaxptim->un.l_00 += con(0x1);
      SKW_QUEUE_TIMER(eaxptim);
    }
  }
  if (ddata.v1d3248 == ddata.v1e0266)
    ddata.v1e0390.l0 = con(0x3);
}

void SKW_3a15_0d5c(c_tim* eaxptim, c_record* edxprec)
{
  if ((((edxprec->u4.b.b0 & con(0x20)) == con(0x0)) && (eaxptim->v.b.b_09 == con(0x0))) || (((edxprec->u4.b.b0 & con(0x20)) != con(0x0)) && (eaxptim->v.b.b_09 == con(0x1))))
  {
    x16 wordrg42 = urshift(edxprec->u6.w, con(0xb));
    x16 wordrg41 = SKW_GET_CREATURE_AT(ulrshift(edxprec->u6.w, con(0x5), con(0xb)), wordrg42);
    if (wordrg41 != con(0xffffffff))
      SKW_ROTATE_CREATURE(wordrg41, urshift(edxprec->u2.w, con(0x7)) & con(0x3), true);
  }
}

void SKW_TRY_ORNATE_NOISE(c_record* eaxprec, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, bool argbool1)
{
  c_tim vtim_00;
  x8 vb_14;
  x8 vb_18;

  x16 vw_10 = ebxw;
  x16 vw_0c = ecxw;
  if ((eaxprec->u4.b.b0 & con(0x40)) != con(0x0))
  {
    if ((urshift(eaxprec->u2.w, con(0x7)) & con(0x100)) == con(0x0))
    {
      if (!argbool1)
      {
        vb_14 = con(0xa);
        vb_18 = SKW_GET_FLOOR_DECORATION_OF_ACTUATOR(eaxprec);
      }
      else
      {
        vb_14 = con(0x9);
        vb_18 = SKW_GET_WALL_DECORATION_OF_ACTUATOR(eaxprec);
      }
      x32 longrg11 = (unsignedlong(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(vb_14, con(0xb), con(0x88), vb_18))) + ddata.longmallocx + unsignedlong(CUTX8(urshift(eaxprec->u2.w, con(0x7))));
      x16 wordrg4 = longrg11 % argw0;
      x32 longrg1 = lextended(argw0 - wordrg4) + ddata.longmallocx; // extension necessary!
      longrg1 |= signedlong(ddata.v1d3248) << con(0x18);
      vtim_00.un.l_00 = longrg1;
      vtim_00.b_04 = con(0x5a);
      vtim_00.b_05 = con(0x0);
      vtim_00.v.w_08 = edxw;
      vtim_00.u.b.b_06 = CUTX8(vw_10);
      vtim_00.u.b.b_07 = CUTX8(vw_0c);
      SKW_QUEUE_TIMER(&vtim_00);
      x16 wordrg11 = eaxprec->u2.w;
      x16 wordrg2 = (urshift(wordrg11, con(0x7)) & con(0xff)) | con(0x100);
      wordrg11 &= con(0x7f);
      eaxprec->u2.w = wordrg11;
      eaxprec->u2.w |= wordrg2 << con(0x7);
      if (wordrg4 == con(0x0))
      {
        if (argbool1)
        {
          x16 wordrg5 = edxw >> con(0xe);
          x16 wordrg41 = vw_10 + table1d27fc[wordrg5];
          x16 wordrg1 = vw_0c + table1d2804[wordrg5];
          if (ddata.v1d3248 != ddata.v1e0282 || wordrg41 != ddata.v1e0280 || wordrg1 != ddata.v1e027e)
          {
            vw_10 = wordrg41;
            vw_0c = wordrg1;
          }
        }
        SKW_QUEUE_NOISE_GEN2(vb_14, con(0x88), con(0xfe), vb_18, signedlong(vw_10), vw_0c, con(0x1), con(0x8c), con(0x80));
      }
    }
  }
}

void SKW_ACTIVATE_CONTINUOUS_ORNATE_ANIMATOR(x16 eaxw, c_record* ebxprec, bool ecxbool, c_tim* edxptim)
{
  x16 vw_00;
  c_tim vtim_04;
  x16 vw_10;

  vw_00 = eaxw;
  x16 wordrg2 = ulrshift(ebxprec->u4.w, con(0xd), con(0xf));
  ebxprec->u4.b.b0 &= con(0xfffffffb);
  x16 wordrg4 = ebxprec->u4.w | (4 * (SKW_3a15_1da8(unsignedword(edxptim->v.b.b_09), wordrg2) & con(0x1)));
  ebxprec->u4.w = wordrg4;
  if (wordrg2 != ulrshift(wordrg4, con(0xd), con(0xf)))
  {
    x16 wordrg23 = SKW_GET_ORNATE_ANIM_LEN(ebxprec, ecxbool);
    vw_10 = wordrg23;
    if ((ebxprec->u4.b.b0 & con(0x4)) == con(0x0))
    {
      x16 wordrg46 = (unsignedlong(CUTX8(urshift(ebxprec->u2.w, con(0x7)))) + ddata.longmallocx) % wordrg23;
      if (wordrg46 != con(0x0))
      {
        wordrg23 -= wordrg46;
        x32 longrg4 = ddata.longmallocx + signedlong(wordrg23);
        longrg4 |= signedlong(ddata.v1d3248) << con(0x18);
        vtim_04.un.l_00 = longrg4;
        vtim_04.b_04 = con(0x59);
        vtim_04.b_05 = con(0x0);
        vtim_04.v.w_08 = vw_00;
        SKW_QUEUE_TIMER(&vtim_04);
      }
      else
      {
        if (ddata.v1d3248 == ddata.v1e0266)
        {
          ddata.v1e0390.b0 |= con(0x1);
        }
        ebxprec->u4.b.b0 &= con(0xfffffffe);
      }
    }
    else
    {
      if ((ebxprec->u4.b.b0 & con(0x1)) == con(0x0))
      {
        ebxprec->u4.b.b0 |= con(0x1);
        if (ddata.v1d3248 == ddata.v1e0266)
        {
          ddata.v1e0390.b0 |= con(0x1);
        }
        x16 wordrg24 = ((vw_10 - (ddata.longmallocx % vw_10)) % vw_10) & con(0xff);
        x16 wordrg48 = ebxprec->u2.w;
        x16 wordrg1 = urshift(wordrg48, con(0x7)) & con(0x100);
        wordrg1 = (wordrg1 | wordrg24) & con(0x1ff);
        wordrg48 &= con(0xffff007f);
        wordrg1 <<= con(0x7);
        ebxprec->u2.w = wordrg48 | wordrg1;
        SKW_TRY_ORNATE_NOISE(ebxprec, unsignedword(edxptim->u.b.b_06), unsignedword(edxptim->u.b.b_07), vw_00, vw_10, ecxbool);
      }
    }
  }
  if ((ebxprec->u4.b.b0 & con(0x20)) != con(0x0))
  {
    if ((ebxprec->u4.w & con(0x18)) == con(0x18))
    {
      SKW_INVOKE_ACTUATOR(ebxprec, con(0x0), unsignedword(edxptim->v.b.b_09));
    }
  }
}

void SKW_ACTIVATE_ORNATE_ANIMATOR(c_tim* eaxptim, x16 ebxw, x32 ecxl, c_record* edxprec)
{
  x8 byterg4h;
  c_tim vtim_00;
  x16 vw_10;
  x16 vw_14;

  vw_14 = unsignedword(eaxptim->u.b.b_06);
  vw_10 = unsignedword(eaxptim->u.b.b_07);
  if ((edxprec->u4.b.b0 & con(0x1)) == con(0x0))
  {
    edxprec->u4.b.b0 |= con(0x1);
    edxprec->u2.w &= con(0x7f);
    vtim_00.un.l_00 = (ddata.longmallocx + 1) | (signedlong(ddata.v1d3248) << con(0x18));
    vtim_00.b_04 = con(0x55);
    vtim_00.b_05 = con(0x0);
    vtim_00.u.w_06 = ebxw;
    vtim_00.v.l_08 = ecxl;
    SKW_QUEUE_TIMER(&vtim_00);
    if ((edxprec->u4.b.b0 & con(0x40)) != con(0x0))
    {
      x8 byterg4;
      if (ecxl == con(0x0))
      {
        byterg4h = con(0xa);
        byterg4 = SKW_GET_FLOOR_DECORATION_OF_ACTUATOR(edxprec);
      }
      else
      {
        byterg4h = con(0x9);
        byterg4 = SKW_GET_WALL_DECORATION_OF_ACTUATOR(edxprec);
        x16 wordrg3 = vw_14 + table1d27fc[eaxptim->v.b.b_09];
        x16 wordrg1 = vw_10 + table1d2804[eaxptim->v.b.b_09]; // BUGFIX 4/2014
        if ((ddata.v1d3248 != ddata.v1e0282 || wordrg3 != ddata.v1e0280) || wordrg1 != ddata.v1e027e)
        {
          vw_14 = wordrg3;
          vw_10 = wordrg1;
        }
      }
      SKW_QUEUE_NOISE_GEN2(byterg4h, con(0x88), con(0xfe), byterg4, vw_14, vw_10, DM2_QUERY_GDAT_ENTRY_DATA_INDEX(byterg4h, con(0xb), con(0x88), byterg4) + con(0x1), con(0x8c), con(0x80));
    }
  }
  if ((edxprec->u4.b.b0 & con(0x4)) != con(0x0))
    SKW_ACTIVATE_RELAY2(eaxptim, con(0x0), edxprec);
}

void SKW_ACTIVATE_RELAY1(c_tim* eaxptim, bool ebxbool, c_record* edxprec)
{
  x32 longrg5;

  if ((((edxprec->u4.b.b0 & con(0x4)) == con(0x0)) || (((edxprec->u4.b.b0 & con(0x20)) == con(0x0)) && (eaxptim->v.b.b_09 == con(0x0)))) || (((edxprec->u4.b.b0 & con(0x20)) != con(0x0)) && (eaxptim->v.b.b_09 == con(0x1))))
  {
    x32 parl00;
    if (!ebxbool)
    {
      longrg5 = ddata.longmallocx + unsignedlong(urshift(edxprec->u2.w, con(0x7)));
      parl00 = unsignedlong(ulrshift(edxprec->u4.w, con(0x5), con(0xc))) + longrg5;
    }
    else
    {
      longrg5 = unsignedlong(urshift(edxprec->u2.w, con(0x7)));
      longrg5 <<= CUTX8(ulrshift(edxprec->u4.w, con(0x5), con(0xc)));
      parl00 = longrg5 + ddata.longmallocx;
    }
    x16 wordrg1;
    if ((edxprec->u4.b.b0 & con(0x4)) == con(0x0))
      wordrg1 = unsignedword(eaxptim->v.b.b_09);
    else
      wordrg1 = unsignedword(CUTX8(ulrshift(edxprec->u4.w, con(0xb), con(0xe))));
    SKW_INVOKE_MESSAGE(ulrshift(edxprec->u6.w, con(0x5), con(0xb)), CUTX8(ulrshift(edxprec->u6.w, con(0xa), con(0xe))), wordrg1, urshift(edxprec->u6.w, con(0xb)), parl00);
  }
}

// was SKW_3a15_15d8
void SKW_ACTIVATE_INVERSE_FLAG(c_tim* eaxptim, c_record* edxprec)
{
  x16 wordrg11;
  if ((edxprec->u4.b.b0 & con(0x20)) == con(0x0))
    wordrg11 = con(0x0);
  else
    wordrg11 = con(0x3);
  x16 wordrg2 = unsignedword(eaxptim->v.b.b_09) + wordrg11;
  SKW_UPDATE_GLOB_VAR(urshift(edxprec->u2.w, con(0x7)), wordrg2, true);
  x16 wordrg1;
  if ((edxprec->u4.b.b0 & con(0x4)) == con(0x0))
    wordrg1 = unsignedword(eaxptim->v.b.b_09);
  else
    wordrg1 = ulrshift(edxprec->u4.w, con(0xb), con(0xe));
  SKW_INVOKE_ACTUATOR(edxprec, con(0x0), wordrg1);
}

// was SKW_3a15_1650
void SKW_ACTIVATE_TEST_FLAG(c_tim* eaxptim, c_record* edxprec)
{
  x16 wordrg41 = unsignedword(eaxptim->v.b.b_09);
  x16 wordrg1 = (SKW_GET_GLOB_VAR(urshift(edxprec->u2.w, con(0x7))) != con(0x0)) ? 1 : 0;
  x16 wordrg5 = ulrshift(edxprec->u4.w, con(0xa), con(0xf));
  if (wordrg1 == wordrg5 || (wordrg41 != con(0x0) && wordrg41 != con(0x2)))
  {
    if (wordrg1 != wordrg5 || wordrg41 != con(0x1))
      return;
  }
  x16 wordrg45;
  if ((edxprec->u4.b.b0 & con(0x4)) == con(0x0))
    wordrg45 = unsignedword(eaxptim->v.b.b_09);
  else
    wordrg45 = ulrshift(edxprec->u4.w, con(0xb), con(0xe));
  SKW_INVOKE_ACTUATOR(edxprec, con(0x0), wordrg45);
}

x16 SKW_3a15_1da8(x16 eaxw, x16 edxw)
{
  if (eaxw < con(0x1))
    return eaxw == con(0x0);
  if (eaxw == con(0x1))
    return con(0x0);
  if (eaxw != con(0x2))
    return con(0x0);
  return edxw ^ con(0x1);
}

void SKW_ACTUATE_TELEPORTER(c_tim* eaxptim)
{
  x8* bptrrg3;
  x8* bptrrg1;

  x16 wordrg6 = unsignedword(eaxptim->u.b.b_06);
  x16 wordrg5 = unsignedword(eaxptim->u.b.b_07);
  bptrrg1 = ddata.savegamepp1[eaxptim->u.b.b_06];
  bptrrg3 = bptrrg1 + eaxptim->u.b.b_07;
  if ((SKW_GET_ADDRESS_OF_TILE_RECORD(wordrg6, wordrg5)->u4.w & con(0x6)) != con(0x6))
  {
    x8 byterg1 = eaxptim->v.b.b_09;
    if (byterg1 == con(0x2))
      byterg1 = ((*bptrrg3 & con(0x8)) != con(0x0)) ? 1 : 0;
    if (byterg1 != con(0x0))
      *bptrrg3 &= con(0xfffffff7);
    else
    {
      *bptrrg3 |= con(0x8);
      SKW_ADVANCE_TILES_TIME(wordrg6, wordrg5);
    }
  }
  SKW_ACTUATE_FLOOR_MECHA(eaxptim);
}

void SKW_ACTUATE_PITFALL(c_tim* eaxptim)
{
  x8* bptrrg1;
  x8* bptrrg12;

  x16 wordrg6 = unsignedword(eaxptim->u.b.b_06);
  bptrrg1 = ddata.savegamepp1[eaxptim->u.b.b_06];
  x16 wordrg5 = unsignedword(eaxptim->u.b.b_07);
  bptrrg12 = bptrrg1 + eaxptim->u.b.b_07;
  x16 wordrg4 = unsignedword(eaxptim->v.b.b_09);
  if (eaxptim->v.b.b_09 == con(0x2))
    wordrg4 = ((*bptrrg12 & con(0x8)) != con(0x0)) ? 1 : 0;
  if (wordrg4 != con(0x0))
    *bptrrg12 &= con(0xfffffff7);
  else
  {
    *bptrrg12 |= con(0x8);
    SKW_ADVANCE_TILES_TIME(wordrg6, wordrg5);
  }
  SKW_ACTUATE_FLOOR_MECHA(eaxptim);
}

// was SKW_3a15_316c
void SKW_PROCESS_TIMER_LIGHT(c_tim* eaxptim)
{
  c_tim vtim_00;

  x16 wordrg11 = eaxptim->u.w_06;
  if (wordrg11 != con(0x0))
  {
    bool boolrg3 = wordrg11 < con(0x0);
    if (boolrg3)
      wordrg11 = -wordrg11;
    x16 wordrg4 = wordrg11 - con(0x1);
    x16 wordrg1 = signedword(table1d6702[wordrg11]) - signedword(table1d6702[wordrg4]);
    if (!boolrg3)
      wordrg1 *= 2;
    else
    {
      wordrg1 = -wordrg1;
      wordrg4 = -wordrg4;
    }
    ddata.savegames1.w0 += wordrg1;
    if (wordrg4 != con(0x0))
    {
      vtim_00.b_04 = con(0x46);
      vtim_00.u.w_06 = wordrg4;
      vtim_00.un.l_00 = (ddata.longmallocx + con(0x8)) | (signedlong(ddata.v1e0266) << con(0x18));
      vtim_00.b_05 = con(0x0);
      SKW_QUEUE_TIMER(&vtim_00);
    }
  }
}

// return never requested
x16 SKW_CONTINUE_ORNATE_ANIMATOR(c_tim* eaxptim)
{
  c_record* recoptrrg2;

  eaxptim->un.l_00 += con(0x1);
  recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(eaxptim->u.w_06);
  x16 wordrg41 = SKW_GET_ORNATE_ANIM_LEN(recoptrrg2, eaxptim->v.l_08 != con(0x0));
  x16 wordrg5 = (urshift(recoptrrg2->u2.w, con(0x7)) + con(0x1)) & con(0x1ff);
  recoptrrg2->u2.w &= con(0xffff007f);
  recoptrrg2->u2.w |= wordrg5 << con(0x7);
  x16 wordrg42 = wordrg5 % wordrg41;
  wordrg5 /= wordrg41;
  if (wordrg42 == con(0x0))
  {
    recoptrrg2->u4.b.b0 &= con(0xfffffffe);
    return wordrg5;
  }
  return SKW_QUEUE_TIMER(eaxptim);
}

// was SKW_3a15_3766
void SKW_PROCESS_TIMER_59(c_tim* eaxptim)
{
  c_record* recoptrrg4;

  recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(eaxptim->v.w_08);
  if ((recoptrrg4->u4.b.b0 & con(0x4)) == con(0x0))
  {
    if (ddata.v1d3248 == ddata.v1e0266)
      ddata.v1e0390.b0 |= con(0x1);
    recoptrrg4->u4.b.b0 &= con(0xfffffffe);
  }
}

void SKW_3df7_0037(bool eaxbool)
{
  if (!eaxbool)
  {
    SKW_UPDATE_GLOB_VAR(con(0x40), con(0x6), false);
    ddata.longv1e1434 = ddata.longmallocx + con(0x555);
    ddata.v1e1480 = con(0x0);
    ddata.v1e147f = con(0x0);
    x16 wordrg41;
    if (!ddata.v1d7188)
    {
      wordrg41 = SKW_RAND16(con(0x1f40)) + con(0x1f4);
      ddata.v1e1478 = CUTX8(SKW_RAND02());
      ddata.v1e1484 = CUTX8(SKW_RAND16(con(0x3)) + con(0x1));
    }
    else
    {
      ddata.v1e147d = con(0x0);
      wordrg41 = SKW_RAND16(con(0x1f4));
      ddata.v1e1478 = con(0x3);
      ddata.v1e1484 = con(0x1);
    }
    ddata.v1e147e = con(0x1);
    ddata.v1e1482 = con(0x0);
    ddata.v1e1474 = con(0x0);
    ddata.v1e146e = con(0x0);
    ddata.v1e147b = con(0x0);
    ddata.v1e1483 = CUTX8(SKW_RAND02());
    SKW_SET_TIMER_WEATHER(signedlong(wordrg41));
  }
  else
  {
    ddata.v1e146e = con(0x0);
    if (ddata.v1e1484 == con(0x0))
    {
      ddata.v1e1484 = con(0x1);
    }
  }
  ddata.v1e1470 = SKW_RAND16(con(0x4)) + con(0x4);
  ddata.v1e1476 = signedword(table1d70f0[((ddata.longmallocx + ddata.longv1e1438) / con(0x555)) % con(0x18)]);
  ddata.v1d7188 = false;
}

// was SKW_3df7_075f
bool SKW_RETRIEVE_ENVIRONMENT_CMD_CD_FW(s_xabytes* eaxpxa)
{
  bool boolrg1;
  t_text* tptrrg1;
  t_text t_00[0x80];

  tptrrg1 = SKW_QUERY_GDAT_TEXT(con(0x17), eaxpxa->b0, t_00, ddata.v1d6c02);
  if (*tptrrg1 != NULLBYTE)
  {
    eaxpxa->w2 = SKW_QUERY_CMDSTR_TEXT(t_00, v1d1140);
    eaxpxa->w6 = con(0x0);
    eaxpxa->w4 = con(0x0);
    eaxpxa->b1 = CUTX8(SKW_QUERY_CMDSTR_TEXT(t_00, v1d1143));
    eaxpxa->b9 = con(0x40);
    eaxpxa->b8 = con(0x40);
    boolrg1 = true;
  }
  else
    boolrg1 = false;
  return boolrg1;
}

x16 SKW_UPDATE_GLOB_VAR(ui16 eaxuw, ui16 ebxuw, bool edxbool)
{
  x16 wordrg1 = SKW_GET_GLOB_VAR(eaxuw);
  if (ebxuw <= con(0x6))
  {
    switch (ebxuw)
    {
      case 0:
        wordrg1 = con(0x1);
      break;

      case 1:
        wordrg1 = con(0x0);
      break;

      case 2:
        wordrg1 = (wordrg1 != con(0x0)) ? con(0x0) : con(0x1);
      break;

      case 3:
        wordrg1 += edxbool ? 1 : 0;
      break;

      case 4:
        wordrg1 -= edxbool ? 1 : 0;
      break;

      case 5:
      break;

      case 6:
        wordrg1 = edxbool ? 1 : 0;
      break;

      default: throw(THROW_DMABORT);
    }
  }
  if (eaxuw > con(0x3f))
  {
    if (eaxuw > con(0x7f))
    {
      if (eaxuw <= con(0xbf))
        ddata.v1e000c[eaxuw] = wordrg1;
      return wordrg1;
    }
    wordrg1 = DM2_BETWEEN_VALUE(con(0x0), con(0xff), wordrg1);
    ddata.globalb[eaxuw - 0x40] = CUTX8(wordrg1);
    return wordrg1;
  }
  x8 byterg2 = con(0x1) << (eaxuw & con(0x7));
  if (wordrg1 == con(0x0))
  {
    byterg2 = ~byterg2;
    ddata.v1e0104[eaxuw >> con(0x3)] &= byterg2;
    return con(0x0);
  }
  ddata.v1e0104[eaxuw >> con(0x3)] |= byterg2;
  return con(0x1);
}

x16 SKW_QUERY_OBJECT_5x5_POS(x16 eaxw, x16 edxw)
{
  c_record* recoptrrg1;

  ui16 uwordrg1 = urshift(eaxw & con(0x3c00), con(0xa));
  if (uwordrg1 < con(0x5))
  {
    if (uwordrg1 <= con(0x3))
      return SKW_ROTATE_5x5_POS(con(0xc), t_rotate(edxw));
    recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(eaxw);
    return SKW_QUERY_CREATURE_5x5_POS(recoptrrg1, (edxw - ulrshift(recoptrrg1->ue.w, con(0x6), con(0xe))) & con(0x3));
  }
  if (uwordrg1 > con(0xa))
  {
    if (uwordrg1 < con(0xe))
      return SKW_ROTATE_5x5_POS(con(0xc), t_rotate(edxw));
    if (uwordrg1 > con(0xe))
    {
      if (uwordrg1 != con(0xf))
        return SKW_ROTATE_5x5_POS(con(0xc), t_rotate(edxw));
      recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(eaxw);
      if ((recoptrrg1->u2.b.b0 & con(0xffffff80)) != con(0x0))
        return SKW_ROTATE_5x5_POS(con(0xc), t_rotate(edxw));
    }
  }
  return SKW_ROTATE_5x5_POS(unsignedword(table1d268e[unsignedlong(eaxw) >> con(0xe)]), t_rotate(edxw));
}

void SKW_DRAW_PICST(c_image* eaxpimg)
{
  t_gfxdata* srcbmp; // was gptrrg6
  t_gfxdata* gptrrg1;
  x16 srcofs; // was vw_00
  x16 vw_04;

  if ((eaxpimg->s_00.width <= con(0x0)) || (eaxpimg->s_00.height <= con(0x0)))
    return;
  srcbmp = SKW_QUERY_PICT_BITMAP(&eaxpimg->s_00);
  s_gfxdatatail* ptrbeforerg6 = getprevious(srcbmp);
  x16 wordrg4 = eaxpimg->w_18;
  if (wordrg4 != con(0xffffffff))
  {
    if ((((wordrg4 & con(0x8000)) == con(0x0)) && (eaxpimg->w_1c == con(0x0))) && (eaxpimg->w_1e == con(0x0)))
    {
      srcofs = eaxpimg->s_00.width;
      vw_04 = eaxpimg->s_00.height;
    }
    else
    {
      wordrg4 |= con(0x8000);
      srcofs = eaxpimg->w_20 + eaxpimg->w_1c;
      vw_04 = eaxpimg->w_22 + eaxpimg->w_1e;
    }
    if (SKW_QUERY_BLIT_RECT(srcbmp, wordrg4, &srcofs, &eaxpimg->rc_24, &vw_04, eaxpimg->w_1a) == NULL)
      return;
  }
  else
  {
    srcofs = eaxpimg->w_20;
    vw_04 = eaxpimg->w_22;
  }
  srcofs += eaxpimg->s_00.w_0e;
  vw_04 += eaxpimg->s_00.w_10;
  x16 wordrg13 = ptrbeforerg6->width;
  x16 wordrg41 = eaxpimg->rc_24.w + srcofs;
  if (wordrg13 <= wordrg41 || ((eaxpimg->w_32 & con(0x1)) == con(0x0)))
    wordrg13 = con(0x0);
  else
    wordrg13 -= wordrg41;
  if ((eaxpimg->w_32 & con(0x1)) != con(0x0))
    srcofs = 0;
  srcofs += wordrg13;
  x16 wordrg12 = ptrbeforerg6->height;
  x16 wordrg410 = eaxpimg->rc_24.h + vw_04;
  if (wordrg12 <= wordrg410 || ((eaxpimg->w_32 & con(0x2)) == con(0x0)))
    wordrg12 = con(0x0);
  else
    wordrg12 -= wordrg410;
  if ((eaxpimg->w_32 & con(0x2)) != con(0x0))
    vw_04 = con(0x0);
  vw_04 += wordrg12;
  gptrrg1 = eaxpimg->pg_2c;
  e_bitres wordrg2;
  x16 wordrg11;
  if (gptrrg1 != ddata.screenptr)
  {
    s_gfxdatatail* ptrbeforerg1 = getprevious(gptrrg1);
    wordrg11 = ptrbeforerg1->width;
    wordrg2 = e_bitres(ptrbeforerg1->b0);
  }
  else
  {
    wordrg11 = ddata.mouse_unk0c;
    wordrg2 = E_BITRES8;
  }
  if (eaxpimg->w_30 != con(0xfffffffe))
//                        srcbmp                   srcofs  destbmp                                                                                       srcbits      destbits    palette
    SKW_FIRE_BLIT_PICTURE(srcbmp, &eaxpimg->rc_24, srcofs, eaxpimg->pg_2c, vw_04, ptrbeforerg6->width, wordrg11, eaxpimg->w_30, eaxpimg->w_32, e_bitres(ptrbeforerg6->b0), wordrg2, (eaxpimg->w_38 > con(0x0)) ? eaxpimg->xLocalPalette : NULL);
  eaxpimg->w_20 = srcofs;
  eaxpimg->w_22 = vw_04;
}

void SKW_0b36_11c0(c_image* eaxpimg, x16 ebxw, x16 ecxw, c_c5rects* edxps)
{
  c_c5rects* edips;
  c_rect rc_00;
  x16 vw_08;
  x16 vw_10;

  edips = edxps;
  eaxpimg->pg_2c = SKW_3e74_5817(edips->s_00.w_00);
  if (ebxw != con(0xffffffff))
  {
    vw_08 = eaxpimg->s_00.width;
    vw_10 = eaxpimg->s_00.height;
    if (SKW_QUERY_BLIT_RECT(SKW_QUERY_PICT_BITMAP(&eaxpimg->s_00), ebxw, &vw_08, &rc_00, &vw_10, con(0xffffffff)) == NULL)
      return;
  }
  else
    SKW_COPY_RECT(&eaxpimg->rc_24, &rc_00);
  eaxpimg->s_00.width += vw_08;
  eaxpimg->s_00.height += vw_10;
  SKW_OFFSET_RECT(edips, &eaxpimg->rc_24, &rc_00);
  eaxpimg->w_18 = con(0xffffffff);
  eaxpimg->w_30 = ecxw;
  SKW_DRAW_PICST(eaxpimg);
  SKW_0b36_0d67(edips, &rc_00);
}

void SKW_44c8_1aca(t_gfxdata* eaxpg, x16 ebxw, x16 ecxw, c_rect* edxr)
{
  ui16 uwordrg2;
  ui16 uwordrg5;
  t_gfxdata vba_00[0xa0];
  t_gfxdata vba_a0[0xa0];
  x16 vw_140;
  x16 vw_144;
  x8 vb_148;
  x16 vw_14c;

  vw_14c = ecxw;
  if (edxr == NULL)
    return;
  SKW_FIRE_UPDATE_BLIT_PALETTE(ddata.paletteptr1);
  vb_148 = CUTX8(ebxw) ^ con(0xf);
  ddata.srcbmp = vba_00;
  ddata.destbmp = eaxpg;
  vw_144 = edxr->w;
  vw_140 = edxr->h;
  if ((UI16CAST(vw_144) <= UI16CAST(con(0x0))) || (vw_140 == con(0x0)))
    return;
  uwordrg2 = (vw_144 + con(0x1)) / 2;
  x8 byterg4 = vb_148 | CUTX8(ebxw << con(0x4));
  uwordrg5 = con(0x0);
  while (uwordrg5 < uwordrg2)
  {
    vba_00[uwordrg5] = byterg4;
    vba_a0[uwordrg5] = byterg4 ^ con(0xffffffff);
    uwordrg5++;
  }
  x16 wordrg6 = edxr->x + edxr->y * vw_14c;
  x16 vw_150 = ((wordrg6 / vw_14c) ^ wordrg6) & con(0x1);
  uwordrg5 = con(0x0);
  while (uwordrg5 < UI16CAST(vw_140))
  {
    ddata.srcbmp = (vw_150 != con(0x0)) ? vba_a0 : vba_00;
    vw_150 ^= con(0x1);
//             srcofs
    SKW_44c8_08ae(0, unsignedlong(vw_144), vb_148, wordrg6);
    wordrg6 += vw_14c;
    uwordrg5++;
  }
}

void SKW_CHANGE_PLAYER_POS(x16 eaxw)
{
  t_gfxdata* destbmp; // was gptrrg5
  t_gfxdata* srcbmp; // was gptrrg6
  x16 vw_00;
  t_gfxdata vcarr_04[PAL16];
  x16 vw_14;

  vw_00 = eaxw;
  bool boolrg5 = true;
  if ((eaxw & con(0x8000)) != con(0x0))
  {
    boolrg5 = false;
    vw_00 &= con(0x3);
  }
  vw_14 = SKW_GET_PLAYER_AT_POSITION((vw_00 + ddata.v1e0258) & con(0x3));
  x16 wordrg2 = vw_14;
  if (ddata.v1e00b8 != con(0x0))
  {
    x16 wordrg4 = ddata.v1e00b8 - con(0x1);
    ddata.v1e00b8 = con(0x0);
    x16 wordrg6 = SKW_GET_PLAYER_AT_POSITION((ddata.v1e0258 + wordrg4) & con(0x3));
    if (boolrg5)
      ddata.hero[wordrg6].b1c = CUTX8(ddata.v1e0258);
    if (wordrg4 != vw_00)
    {
      if (wordrg2 >= con(0x0))
      {
        x8 byterg4 = CUTX8(wordrg4);
        byterg4 += CUTX8(ddata.v1e0258);
        byterg4 &= con(0x3);
        ddata.hero[wordrg2].b1d = byterg4;
      }
      x8 byterg41 = (CUTX8(vw_00) + CUTX8(ddata.v1e0258)) & con(0x3);
      ddata.hero[wordrg6].b1d = byterg41;
    }
  }
  else
  {
    if (wordrg2 < con(0x0))
      return;
    ddata.v1d2658.w = ddata.v1d272c;
    ddata.v1d2658.h = ddata.v1d272e;
    srcbmp = SKW_ALLOC_PICT_BUFF(ddata.v1d272c, BIGPOOL_LO, con(0x4), ddata.v1d272e);
    destbmp = SKW_ALLOC_PICT_BUFF(ddata.v1d272c, BIGPOOL_LO, con(0x8), ddata.v1d272e);
    SKW_QUERY_GDAT_SQUAD_ICON(srcbmp, vcarr_04, vw_14);
    SKW_FILL_ENTIRE_PICT(destbmp, con(0xc));
//                        srcbmp                srcofs destbmp                                                            srcbits    destbits    palette
    SKW_FIRE_BLIT_PICTURE(srcbmp, &ddata.v1d2658, 0, destbmp, con(0x0), ddata.v1d272c, ddata.v1d272c, con(0x4), con(0x0), E_BITRES4, E_BITRES8, vcarr_04);
    SKW_FIRE_HIDE_MOUSE_CURSOR();
    SKW_FIRE_MOUSE_EVENT_RECEIVER(con(0x3), con(0x8), con(0x6), destbmp, ddata.v1d272c, ddata.v1d272e, con(0x8), NULL, unsignedword(vcarr_04[0xc]));
    userinterrupt(con(0xb), con(0x3));
    SKW_FREE_PICT_BUFF(destbmp);
    SKW_FREE_PICT_BUFF(srcbmp);
    ddata.v1e00b0 = vw_00;
    ddata.v1e00b8 = vw_00 + con(0x1);
    SKW_FIRE_SHOW_MOUSE_CURSOR();
  }
  SKW_443c_040e();
}

// return not really needed (the only call returns the result without being needed)
// palette: argpg7
ui16 SKW_44c8_1e43(t_gfxdata* eaxpg, t_gfxdata* ebxpg, c_rect* ecxr, t_gfxdata* edxpg, ui16 arguw0, ui16 arguw1, x16 argw2, x16 argw3, x16 argw4, x16 argw5, x8 argb6, t_palette* palette)
{
  x16 uwordrg5;
  t_gfxdata* pg_00;
  x16 vw_04;
  pg_00 = eaxpg;

  SKW_FIRE_UPDATE_BLIT_PALETTE(palette);
  x16 vw_0c = ecxr->w;
  vw_04 = ecxr->h;
  x16 vw_14 = ecxr->x + ecxr->y * argw5;
  ui16 uwordrg6 = arguw1;
  uwordrg5 = arguw0 - arguw1;
  ddata.srcbmp = pg_00;
  ddata.destbmp = edxpg;
  if (ebxpg == NULL)
  {
    x16 vw_10 = con(0x0);
    for(;;)
    {
      ui16 uwordrg1 = vw_10;
      if (uwordrg1 >= UI16CAST(vw_04))
        return uwordrg1;
      ui16 vuw_18 = vw_0c;
      x16 vw_1c = con(0x0);
      ui16 uwordrg4;
      x16 wordrg1;
      for(;;)
      {
        wordrg1 = vw_14 + vw_1c;
        uwordrg4 = vuw_18;
        if (uwordrg5 > uwordrg4)
          break;
        if (argb6 >= con(0x0))
//                       srcofs
          SKW_44c8_08ae(uwordrg6, unsignedlong(uwordrg5), argb6, wordrg1);
        else
//                                                   srcofs
          SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(uwordrg6, uwordrg5, wordrg1);
        vuw_18 -= uwordrg5;
        vw_1c += uwordrg5;
        uwordrg5 = arguw0;
        uwordrg6 = con(0x0);
      }
      if (uwordrg4 != con(0x0))
      {
        if (argb6 >= con(0x0))
//                       srcofs
          SKW_44c8_08ae(uwordrg6, unsignedlong(uwordrg4), argb6, wordrg1);
        else
//                                                   srcofs
          SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(uwordrg6, uwordrg4, wordrg1);
        uwordrg5 -= vuw_18;
        uwordrg6 += vuw_18;
      }
      if ((vw_0c & con(0x1)) != con(0x0))
      {
        if (--uwordrg5 == con(0x0))
        {
          uwordrg5 = arguw0;
          uwordrg6 = con(0x0);
        }
        else
          uwordrg6++;
      }
      vw_14 += argw5;
      vw_10++;
    }
  }
  argw4 = (argw4 + con(0x1)) & con(0xfffffffe);
  x16 vw_08 = argw2 + argw3 * argw4;
  x16 vw_10_1 = con(0x0);
  for(;;)
  {
    ui16 uwordrg1 = vw_10_1;
    if (uwordrg1 >= UI16CAST(vw_04))
      return uwordrg1;
    x16 wordrg14 = vw_0c;
    x16 vw_1c = vw_08;
    for(;;)
    {
      if (wordrg14 == con(0x0))
        break;
      if (((table1d255a[vw_1c & con(0x1)]) & ebxpg[vw_1c / 2]) != con(0x0))
        break;
      wordrg14--;
      vw_1c++;
      uwordrg6++;
      if (--uwordrg5 == con(0x0))
      {
        uwordrg6 = con(0x0);
        uwordrg5 = arguw0;
      }
    }
    vw_1c -= vw_08;
    ui16 vuw_18 = wordrg14;
    x16 wordrg13 = vw_08 + vw_0c;
    for(;;)
    {
      wordrg13--;
      if (vuw_18 <= con(0x0))
        break;
      if ((table1d255a[wordrg13 & con(0x1)] & ebxpg[wordrg13 / 2]) != con(0x0))
        break;
      vuw_18--;
    }
    if (vuw_18 > con(0x0))
    {
      ui16 uwordrg3;
      x16 wordrg12;
      for(;;)
      {
        wordrg12 = vw_14 + vw_1c;
        uwordrg3 = vuw_18;
        if (uwordrg5 > uwordrg3)
          break;
        if (argb6 >= con(0x0))
//                       srcofs
          SKW_44c8_08ae(uwordrg6, unsignedlong(uwordrg5), argb6, wordrg12);
        else
//                                                   srcofs
          SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(uwordrg6, uwordrg5, wordrg12);
        vuw_18 -= uwordrg5;
        vw_1c += uwordrg5;
        uwordrg5 = arguw0;
        uwordrg6 = con(0x0);
      }
      if (uwordrg3 != con(0x0))
      {
        if (argb6 >= con(0x0))
//                       srcofs
          SKW_44c8_08ae(uwordrg6, unsignedlong(vuw_18), argb6, wordrg12);
        else
//                                                    srcofs
          SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(uwordrg6, uwordrg3, wordrg12);
        uwordrg5 -= vuw_18;
        uwordrg6 += vuw_18;
      }
    }
    if ((vw_0c & con(0x1)) != con(0x0))
    {
      if (--uwordrg5 == con(0x0))
      {
        uwordrg5 = arguw0;
        uwordrg6 = con(0x0);
      }
      else
        uwordrg6++;
    }
    vw_08 += argw4;
    vw_14 += argw5;
    vw_10_1++;
  }
}

bool SKW_QUERY_CREATURES_ITEM_MASK(x8 eaxb, s_x40bytes* ebxps, bool ecxbool, x8 edxb)
{
  ui8 byterg3h;
  t_text* tptrrg7;
  x8* bptrrg1;
  t_text t_00[0x80];
  s_x40bytes* p_80;
  x8 vb_88;
  x8 vb_8c;

  vb_8c = eaxb;
  vb_88 = edxb;
  p_80 = ebxps;
  DM2_ZERO_MEMORY(DOWNCAST(s_x40bytes, ebxps), con(0x40));
  tptrrg7 = SKW_QUERY_GDAT_TEXT(con(0xf), vb_88 + con(0x10), t_00, vb_8c);
  if (*tptrrg7 == NULLBYTE)
    return false;
  x16 wordrg6 = con(0x0);
  x16 wordrg4 = con(0xffffffff);
  x16 wordrg5 = con(0xffffffff);
  x16 wordrg1 = con(0x0);
  do
  {
    byterg3h = *tptrrg7++;
    if (byterg3h < con(0x30) || byterg3h > con(0x39))
    {
      if (byterg3h != con(0x2d))
      {
        if (wordrg1 != con(0x0))
        {
          if (wordrg4 < con(0x0))
            wordrg4 = wordrg6;
          while (wordrg4 <= wordrg6)
          {
            x16 wordrg2 = wordrg4 + wordrg5;
            bptrrg1 = &p_80->barr_00[wordrg2 / con(0x8)];
            *bptrrg1 |= con(0x1) << (wordrg2 & con(0x7));
            wordrg4++;
          }
          wordrg6 = con(0x0);
          wordrg4 = con(0xffffffff);
          wordrg5 = wordrg4;
          wordrg1 = con(0x0);
        }
        if (byterg3h < UI8CAST(con(0x4a)))
        {
          if (byterg3h >= UI8CAST(con(0x41)))
          {
            if (byterg3h == UI8CAST(con(0x41)))
              wordrg5 = con(0x80);
            else if (byterg3h == con(0x43))
              wordrg5 = ecxbool ? con(0x0) : con(0x1e0);
          }
        }
        else if (byterg3h <= UI8CAST(con(0x4a)))
          wordrg5 = con(0x100);
        else if (byterg3h < UI8CAST(con(0x53)))
        {
          if (byterg3h == con(0x50))
            wordrg5 = con(0x180);
        }
        else if (byterg3h <= UI8CAST(con(0x53)))
          wordrg5 = con(0x1fc);
        else if (byterg3h == con(0x57))
          wordrg5 = con(0x0);
      }
      else
      {
        wordrg4 = wordrg6;
        wordrg6 = con(0x0);
      }
    }
    else
    {
      wordrg6 *= con(0xa);
      wordrg6 += unsignedword(byterg3h) - con(0x30);
      wordrg1 = con(0x1);
    }
  } while (byterg3h != con(0x0));
  return true;
}

bool R_4FCC(x8 eaxb, x16* ebxpw, t_gfxdata** ecxppg, x16 edxw)
{
  x8 byterg5;
  bool boolrg1;
  x16 vw_00;
  t_gfxdata* gptrrg1;
  t_gfxdata* gptrrg4;
  t_gfxdata* pg_04;

  vw_00 = edxw;
  gptrrg1 = UPCAST(t_gfxdata, SKW_QUERY_GDAT_ENTRY_DATA_PTR(con(0xf), con(0x7), con(0xfc), eaxb));
  pg_04 = gptrrg1;
  x16 wordrg2 = *ebxpw;
  x32 longrg4 = unsignedlong(wordrg2); // let this long!
  if (longrg4 != con(0xffff))
  {
    longrg4 += unsignedlong(vw_00);
    x16 wordrg11 = urshift(unsignedword(gptrrg1[4 * longrg4 + con(0x1)] & con(0xfffffff0)), con(0x4));
    if (wordrg11 == con(0x0))
    {
      *ecxppg = &pg_04[4 * (wordrg2 + vw_00)];
      return false;
    }
    wordrg2 += wordrg11;
  }
  else
    wordrg2 = con(0x0);
  gptrrg4 = &pg_04[4 * (wordrg2 + vw_00)];
  for(;;)
  {
    if (urshift(unsignedword(gptrrg4[con(0x1)] & con(0xfffffff0)), con(0x4)) == con(0x0))
    {
      boolrg1 = false;
      break;
    }
    byterg5 = gptrrg4[con(0x1)] & con(0xf);
    if (byterg5 == con(0xf) || unsignedword(byterg5) >= (CUTX16(SKW_RAND()) & con(0xf)))
    {
      boolrg1 = ((gptrrg4[con(0x3)] & con(0xc)) / 4 + ((gptrrg4[con(0x3)] & con(0xfffffff0)) >> 4)) != con(0x0);
      break;
    }
    wordrg2++;
    gptrrg4 += con(0x4);
  }
  *ebxpw = wordrg2;
  *ecxppg = &pg_04[4 * (wordrg2 + vw_00)];
  return boolrg1;
}

// returns 0, 1 or 2
x16 R_50CB(x8 eaxb, x16* ebxpw, t_gfxdata** ecxppg, x16 edxw)
{
  t_gfxdata* gptrrg1;
  t_gfxdata* gptrrg2;
  bool skip = false;

  x16 wordrg1;
  gptrrg1 = UPCAST(t_gfxdata, SKW_QUERY_GDAT_ENTRY_DATA_PTR(con(0xf), con(0x7), con(0xfc), eaxb));
  gptrrg2 = gptrrg1;
  x16 wordrg4 = *ebxpw;
  if (wordrg4 != con(0xffff))
  {
    wordrg4 += edxw;
    x16 wordrg11 = unsignedword(gptrrg1[4 * wordrg4 + con(0x2)] & con(0x3f));
    if (wordrg11 != con(0x0))
      *ebxpw += wordrg11;
    else
    {
      wordrg1 = con(0x2);
      skip = true;
    }
  }
  else
    *ebxpw = con(0x0);
  if (!skip)
  {
    ui16 uwordrg1 = unsignedword(gptrrg2[4 * (*ebxpw + edxw) + con(0x1)]) & con(0xfffffff0);
    if (urshift(uwordrg1, con(0x4)) != con(0x0))
      wordrg1 = con(0x1);
    else
      wordrg1 = con(0x0);
  }
  gptrrg2 += 4 * (*ebxpw + edxw);
  *ecxppg = gptrrg2;
  return wordrg1;
}

void SKW_LOAD_MISCITEM(void)
{
  ui8 ubyterg5;
  x8 vb_04;

  if (ddata.v1d268a)
    return;
  ddata.v1d268a = true;
  vb_04 = con(0x0);
  for(;;)
  {
    ubyterg5 = vb_04;
    if (ubyterg5 >= con(0x80))
      return;
    if ((DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x15), con(0xb), con(0x0), ubyterg5) & con(0x4000)) != con(0x0))
    {
      x16 wordrg5 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x15), con(0xb), con(0x2), ubyterg5);
      x16 wordrg3 = con(0x0);
      while (wordrg3 < ddata.v1e03fe && (wordrg5 > ddata.v1e03ac[wordrg3]))
        wordrg3++;
      if (wordrg3 < ddata.v1e03fe)
      {
        x32 longrg6 = lextended(2 * (ddata.v1e03fe - wordrg3));
        SKW_COPY_MEMORY(DOWNCAST(x16, &ddata.v1e03ac[wordrg3]), longrg6, DOWNCAST(x16, &ddata.v1e03ac[wordrg3 + con(0x1)]));
        SKW_COPY_MEMORY(DOWNCAST(x16, &ddata.v1e0394[wordrg3]), longrg6, DOWNCAST(x16, &ddata.v1e0394[wordrg3 + con(0x1)]));
      }
      ddata.v1e03ac[wordrg3] = wordrg5;
      ddata.v1e0394[wordrg3] = unsignedword(vb_04) + con(0x100);
      ddata.v1e03fe++;
    }
    vb_04++;
  }
}

void SKW_PROCEED_TIMERS(void)
{
  x8* bptrrg1;
  c_tim vtim_00;
  bool flag;

  for(;;)
  {
    if (!SKW_IS_TIMER_TO_PROCEED())
    {
      SKW_CHANGE_CURRENT_MAP_TO(ddata.v1e0266);
      return;
    }
    SKW_IBMIO_USER_INPUT_CHECK();
    SKW_GET_AND_DELETE_NEXT_TIMER(&vtim_00);
    SKW_CHANGE_CURRENT_MAP_TO(CUTX16(signedlong(CUTX8(vtim_00.un.l_00 >> con(0x18)))));
    ui16 uwordrg4 = unsignedword(vtim_00.b_04);
    if (uwordrg4 < con(0x47))
    {
      if (uwordrg4 < con(0xe))
      {
        if (uwordrg4 < con(0x4))
        {
          if (uwordrg4 >= con(0x1))
          {
            if (uwordrg4 == con(0x1))
              SKW_STEP_DOOR(&vtim_00);
            else if (uwordrg4 == con(0x2))
              SKW_PROCESS_TIMER_DESTROY_DOOR(&vtim_00);
          }
        }
        else if (uwordrg4 <= con(0x4))
        {
          bptrrg1 = ddata.savegamepp1[vtim_00.u.b.b_06];
          ui8 ubyterg1 = CUTX8(urshift(unsignedword(bptrrg1[vtim_00.u.b.b_07]), con(0x5)));
          if (ubyterg1 <= con(0x6))
          {
            switch (ubyterg1)
            {
              case 0:
                SKW_ACTUATE_WALL_MECHA(&vtim_00);
              break;

              case 1:
                SKW_ACTUATE_FLOOR_MECHA(&vtim_00);
              break;

              case 2:
                SKW_ACTUATE_PITFALL(&vtim_00);
              break;

              case 3:
                // no call here!
              break;

              case 4:
                SKW_ACTUATE_DOOR(&vtim_00);
              break;

              case 5:
                SKW_ACTUATE_TELEPORTER(&vtim_00);
              break;

              case 6:
                SKW_ACTUATE_TRICKWALL(&vtim_00);
              break;

              default: throw(THROW_DMABORT);
            }
          }
        }
        else if (uwordrg4 < con(0xc))
          ;
        else if (uwordrg4 <= con(0xc))
          SKW_PROCESS_TIMER_0C(unsignedword(vtim_00.b_05));
        else
          SKW_PROCESS_TIMER_RESURRECTION(&vtim_00);
      }
      else if (uwordrg4 <= con(0xe))
        SKW_PROCESS_TIMER_0E(&vtim_00, con(0xfffffffe));
      else if (uwordrg4 < con(0x1d))
      {
        if (uwordrg4 >= con(0x15))
        {
          if (uwordrg4 == con(0x15))
            SKW_PROCESS_SOUND(vtim_00.u.w_06);
          else if (uwordrg4 == con(0x19))
            SKW_PROCESS_CLOUD(&vtim_00);
        }
      }
      else if (uwordrg4 <= con(0x1e))
        SKW_STEP_MISSILE(&vtim_00);
      else if (uwordrg4 < con(0x3c))
      {
        if (uwordrg4 >= con(0x21) && uwordrg4 <= con(0x22))
        {
          x16 wordrg21 = uwordrg4;
          x16 wordrg45 = unsignedword(vtim_00.u.b.b_07);
          SKW_THINK_CREATURE(signedword(vtim_00.u.b.b_06), wordrg21, wordrg45);
        }
      }
      else if (uwordrg4 <= con(0x3d))
        SKW_PROCESS_TIMER_3D(&vtim_00);
      else if (uwordrg4 == con(0x46))
      {
        SKW_CHANGE_CURRENT_MAP_TO(ddata.v1e0266);
        SKW_PROCESS_TIMER_LIGHT(&vtim_00);
        SKW_RECALC_LIGHT_LEVEL();
      }
    }
    else if (uwordrg4 <= con(0x47))
    {
      flag = --ddata.savegames1.b2 != con(0x0);
      if (!flag && ddata.v1e0976 != con(0x0))
        ddata.hero[ddata.v1e0976 - con(0x1)].u32.b.b1 |= con(0x40);
    }
    else if (uwordrg4 < con(0x58))
    {
      if (uwordrg4 < con(0x54))
      {
        if (uwordrg4 <= con(0x48))
        {
          x16 wordrg43 = con(0x0);
          for(;;)
          {
            x16 wordrg22 = wordrg43;
            if (wordrg22 >= ddata.savegamew2)
              break;
            if (((con(0x1) << wordrg43) & unsignedword(vtim_00.b_05)) != con(0x0))
              if (ddata.hero[wordrg22].w36 != con(0x0))
                ddata.hero[wordrg22].w103 -= vtim_00.u.w_06;
            if (ddata.hero[wordrg43].w103 < con(0x0))
              ddata.hero[wordrg43].w103 = con(0x0);
            wordrg43++;
          }
        }
        else if (uwordrg4 == con(0x4b))
        {
          ddata.hero[vtim_00.b_05].b1f--;
          ddata.hero[vtim_00.b_05].w48 -= vtim_00.u.w_06;
          SKW_PROCESS_POISON(unsignedword(vtim_00.b_05), vtim_00.u.w_06);
        }
      }
      else if (uwordrg4 <= con(0x54))
        SKW_UPDATE_WEATHER(true);
      else if (uwordrg4 < con(0x56))
        SKW_CONTINUE_ORNATE_ANIMATOR(&vtim_00);
      else if (uwordrg4 <= con(0x56))
        SKW_CONTINUE_TICK_GENERATOR(&vtim_00);
      else
        SKW_GET_ADDRESS_OF_RECORD(vtim_00.u.w_06)->u4.b.b0 &= con(0xfffffffe);
    }
    else if (uwordrg4 <= con(0x58))
      SKW_PROCESS_TIMER_RELEASE_DOOR_BUTTON(&vtim_00);
    else if (uwordrg4 < con(0x5b))
    {
      if (uwordrg4 <= con(0x59))
        SKW_PROCESS_TIMER_59(&vtim_00);
      else
        SKW_CONTINUE_ORNATE_NOISE(&vtim_00);
    }
    else if (uwordrg4 <= con(0x5b))
      SKW_GET_ADDRESS_OF_RECORD(vtim_00.u.w_06)->u4.b.b0 &= con(0xfffffffe);
    else if (uwordrg4 < con(0x5d))
      SKW_GET_ADDRESS_OF_RECORD(vtim_00.u.w_06)->u2.b.b0 |= con(0x1);
    else if (uwordrg4 <= con(0x5d))
    {
      if (unsignedword(vtim_00.v.b.b_08) == ddata.v1e0266)
      {
        SKW_MOVE_RECORD_TO(con(0xffff), ddata.v1e0272, vtim_00.u.w_06 & con(0x1f), ddata.v1e0270, ulrshift(vtim_00.u.w_06, con(0x6), con(0xb)));
        SKW_ROTATE_SQUAD(ulrshift(vtim_00.u.w_06, con(0x4), con(0xe)));
      }
    }
    else if (uwordrg4 == con(0x5e))
    {
      x16 wordrg6 = unsignedword(vtim_00.u.b.b_06);
      x16 wordrg11;
      if (SKW_RAND02() == con(0x0))
        wordrg11 = SKW_RAND02();
      else
        wordrg11 = SKW_CALC_VECTOR_DIR(unsignedword(vtim_00.u.b.b_06), ddata.v1e0270, ddata.v1e0272, unsignedword(vtim_00.u.b.b_07));
      SKW_ALLOC_NEW_CREATURE(unsignedword(vtim_00.v.b.b_09), wordrg11, wordrg6, con(0x7), unsignedword(vtim_00.u.b.b_07));
    }
  }
}

void SKW_CONTINUE_ORNATE_NOISE(c_tim* eaxptim)
{
  bool boolrg5;
  c_record* recoptrrg3;
  x8 vb_00;
  x8 vb_04;
  bool flag;

  recoptrrg3 = SKW_GET_ADDRESS_OF_RECORD(eaxptim->v.w_08);
  if ((recoptrrg3->u4.b.b0 & con(0x1)) == con(0x0) || signedword(eaxptim->un.b.b_03) != ddata.v1e0266)
  {
    x16 wordrg11 = recoptrrg3->u2.w;
    x16 wordrg41 = unsignedword(CUTX8(urshift(wordrg11, con(0x7))));
    wordrg11 &= con(0x7f);
    recoptrrg3->u2.w = wordrg11 | (wordrg41 << con(0x7));
    return;
  }
  x8 byterg1 = (SKW_GET_TILE_VALUE(unsignedword(eaxptim->u.b.b_06), unsignedword(eaxptim->u.b.b_07)) & con(0xff)) >> con(0x5);
  flag = byterg1 != con(0x0);
  boolrg5 = byterg1 == con(0x0);
  if (flag)
  {
    vb_04 = con(0xa);
    vb_00 = SKW_GET_FLOOR_DECORATION_OF_ACTUATOR(recoptrrg3);
  }
  else
  {
    vb_04 = con(0x9);
    vb_00 = SKW_GET_WALL_DECORATION_OF_ACTUATOR(recoptrrg3);
  }
  eaxptim->un.l_00 += unsignedlong(SKW_GET_ORNATE_ANIM_LEN(recoptrrg3, boolrg5));
  SKW_QUEUE_TIMER(eaxptim);
  x16 wordrg3 = unsignedword(eaxptim->u.b.b_06);
  x16 wordrg1 = unsignedword(eaxptim->u.b.b_07);
  if (boolrg5)
  {
    x16 wordrg5 = table1d27fc[eaxptim->v.b.b_09] + wordrg3;
    x16 wordrg4 = table1d2804[eaxptim->v.b.b_09] + wordrg1;
    if (ddata.v1d3248 != ddata.v1e0282 || wordrg5 != ddata.v1e0280 || wordrg4 != ddata.v1e027e)
    {
      wordrg3 = wordrg5;
      wordrg1 = wordrg4;
    }
  }
  SKW_QUEUE_NOISE_GEN2(vb_04, con(0x88), con(0xfe), vb_00, wordrg3, wordrg1, con(0x1), con(0x8c), con(0x80));
}

void SKW_PROCESS_ACTUATOR_TICK_GENERATOR(void)
{
  x8* ebppb;
  x16* wptrrg6;
  c_record* recoptrrg2;
  x16 vw_04;
  x16 vw_08;

  for (x16 wordrg5 = con(0x0); wordrg5 < unsignedword(CUTX8(ddata.savegamep4->w0[2])); wordrg5++)
  {
    vw_04 = ulrshift(ddata.v1e03c8[wordrg5].w8, con(0x5), con(0xb));
    vw_08 = urshift(ddata.v1e03c8[wordrg5].w8, con(0xb));
	if (ddata.v1e03cc == NULL) return;	// SPX: avoid null pointer
    x8** hlpptr1 = ddata.v1e03cc[wordrg5];
    ebppb = *hlpptr1;
    wptrrg6 = &ddata.v1e038c[ddata.v1e03d8[ddata.v1e03e4[wordrg5]]];
    for (x16 vw_00 = con(0x0); vw_00 <= vw_04; vw_00++)
    {
      for (x16 vw_0c = con(0x0); vw_0c <= vw_08; vw_0c++)
      {
        if ((*ebppb++ & con(0x10)) != con(0x0))
        {
          x16 wordrg3 = *wptrrg6++;
          for(;;)
          {
            x16 wordrg11 = urshift(wordrg3 & con(0x3c00), con(0xa));
            if (wordrg11 > con(0x3))
              break;
            if (wordrg11 == con(0x3))
            {
              recoptrrg2 = SKW_GET_ADDRESS_OF_RECORD(wordrg3);
              x16 wordrg1 = recoptrrg2->u2.w & con(0x7f);
              if (wordrg1 >= con(0x1e) && (wordrg1 <= con(0x1e) || (wordrg1 >= con(0x33) && wordrg1 <= con(0x37))))
              {
                if (ddata.savegamew8 != con(0x0))
                {
                  if ((recoptrrg2->u4.b.b0 & con(0x4)) == con(0x0))
                    recoptrrg2->u4.b.b0 &= con(0xfffffffe);
                  else
                  {
                    SKW_CHANGE_CURRENT_MAP_TO(wordrg5);
                    SKW_ACTIVATE_TICK_GENERATOR(recoptrrg2, wordrg3);
                  }
                }
              }
            }
            wordrg3 = SKW_GET_NEXT_RECORD_LINK(wordrg3);
          }
        }
      }
    }
  }
}

void SKW_CONTINUE_TICK_GENERATOR(c_tim* eaxptim)
{
  c_record* recoptrrg5;

  x16 wordrg6;
  recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(eaxptim->u.w_06);
  if ((recoptrrg5->u4.w & con(0x18)) != con(0x18))
  {
    wordrg6 = ulrshift(recoptrrg5->u4.w, con(0xd), con(0xf));
    if (wordrg6 != con(0x0))
      SKW_INVOKE_ACTUATOR(recoptrrg5, con(0x0), ulrshift(recoptrrg5->u4.w, con(0xb), con(0xe)));
  }
  else
  {
    x16 wordrg2 = (ulrshift(eaxptim->v.w_08, con(0x7), con(0xf)) ^ con(0x1)) & con(0x1);
    eaxptim->v.b.b_09 &= con(0xfffffffe);
    eaxptim->v.w_08 |= wordrg2 << con(0x8);
    wordrg6 = wordrg2 | ulrshift(recoptrrg5->u4.w, con(0xd), con(0xf));
    SKW_INVOKE_ACTUATOR(recoptrrg5, con(0x0), ((eaxptim->v.b.b_09 & con(0x1)) == con(0x0)) ? 1 : 0);
  }
  if (wordrg6 == con(0x0))
    recoptrrg5->u4.b.b0 &= con(0xfffffffe);
  else
  {
    eaxptim->un.l_00 += unsignedlong(unsignedword(CUTX8(eaxptim->v.w_08))) * unsignedlong(urshift(recoptrrg5->u2.w, con(0x7)));
    SKW_QUEUE_TIMER(eaxptim);
  }
}

void SKW_ACTIVATE_TICK_GENERATOR(c_record* eaxprec, x16 edxw)
{
  c_tim vtim_00;
  x16 wordrg3 = con(0x0); // assuming this preset is necessary! BUGFIX 4/2014

  x16 wordrg1 = eaxprec->u2.w & con(0x7f);
  if (wordrg1 < con(0x34))
  {
    if (wordrg1 < con(0x1e))
      ; // Note: original BUG? wordrg3 undefined then!
    else if (wordrg1 <= con(0x1e))
      wordrg3 = con(0x1);
    else if (wordrg1 == con(0x33))
      wordrg3 = con(0x8);
  }
  else if (wordrg1 <= con(0x34))
    wordrg3 = con(0x10);
  else if (wordrg1 < con(0x36))
    wordrg3 = con(0x20);
  else if (wordrg1 <= con(0x36))
    wordrg3 = con(0x40);
  else if (wordrg1 == con(0x37))
    wordrg3 = con(0x80);
  x16 wordrg4 = eaxprec->u2.w;
  if ((wordrg4 & con(0xffffff80)) != con(0x0))
  {
    x32 longrg4 = (ddata.longmallocx % (unsignedlong(urshift(wordrg4, con(0x7))) * unsignedlong(wordrg3))) + ddata.longmallocx;
    longrg4 |= signedlong(ddata.v1d3248) << con(0x18);
    vtim_00.un.l_00 = longrg4;
    vtim_00.b_04 = con(0x56);
    vtim_00.b_05 = con(0x0);
    vtim_00.u.w_06 = edxw;
    vtim_00.v.b.b_08 = con(0x0);
    vtim_00.v.w_08 |= wordrg3;
    vtim_00.v.b.b_09 &= con(0xfffffffe);
    SKW_QUEUE_TIMER(&vtim_00);
    eaxprec->u4.b.b0 |= con(0x1);
  }
}

// was SKW_3a15_306c
void SKW_PROCESS_TIMER_RESURRECTION(c_tim* eaxptim)
{
  x16 vw_00;
  x16 vw_04;
  x16 vw_0c;

  vw_00 = unsignedword(eaxptim->u.b.b_06);
  vw_04 = unsignedword(eaxptim->u.b.b_07);
  x16 wordrg3 = unsignedword(eaxptim->v.b.b_08);
  vw_0c = unsignedword(eaxptim->b_05);
  ui16 uwordrg1 = unsignedword(eaxptim->v.b.b_09);
  x16 vw_08 = uwordrg1;
  if (uwordrg1 < con(0x1))
  {
    if (uwordrg1 == con(0x0)) // TODO: what value should be left here, if less unsigned 1 before?
      SKW_BRING_CHAMPION_TO_LIFE(vw_0c);
    return;
  }
  else if (uwordrg1 == con(0x1))
  {
    x16 wordrg1 = SKW_GET_TILE_RECORD_LINK(vw_00, vw_04);
    x16 wordrg2;
    x16 wordrg5;
    for (;; wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg2))
    {
      wordrg2 = wordrg1;
      if (wordrg2 == con(0xfffffffe))
        return;
      wordrg5 = wordrg2;
      if ((wordrg5 >> con(0xe)) == wordrg3 && SKW_QUERY_CLS1_FROM_RECORD(wordrg5) == con(0x15) && SKW_QUERY_CLS2_FROM_RECORD(wordrg5) == con(0x0) && SKW_ADD_ITEM_CHARGE(wordrg5, con(0x0)) == vw_0c)
        break;
    }
    SKW_CUT_RECORD_FROM(wordrg5, vw_00, vw_04, NULL);
    SKW_DEALLOC_RECORD(wordrg5);
    eaxptim->un.l_00 += con(0x1);
  }
  else if (uwordrg1 == con(0x2))
  {
    SKW_CREATE_CLOUD(con(0xffe4), vw_00, vw_04, con(0x0), wordrg3);
    eaxptim->un.l_00 += con(0x5);
  }
  else
    return;
  vw_08--;
  eaxptim->v.b.b_09 = CUTX8(vw_08);
  SKW_QUEUE_TIMER(eaxptim);
}

void SKW_ACTUATE_WALL_MECHA(c_tim* eaxptim)
{
  u_bbw* ubwptrrg1;
  c_record* recoptrrg5;
  c_record* recoptrrg4;
  c_record* recoptrrg1;
  bool flag;
  c_tim vtim_00;
  x16 vw_0c;
  x16 vw_10;
  x16 vw_14;
  x16 vw_18;
  x16 vw_1c;
  x16 vw_20;
  x16 vw_24;
  x16 vw_28;

  x16 wordrg1 = SKW_GET_TILE_RECORD_LINK(unsignedword(eaxptim->u.b.b_06), unsignedword(eaxptim->u.b.b_07));
  for (;; wordrg1 = SKW_GET_NEXT_RECORD_LINK(vw_24))
  {
    vw_24 = wordrg1;
    if (vw_24 == con(0xfffffffe))
      return;
    x16 wordrg15 = urshift(vw_24 & con(0x3c00), con(0xa));
    if (wordrg15 > con(0x3))
      return;
    if (vw_24 != con(0x2) || urshift(vw_24, con(0xe)) != unsignedword(eaxptim->v.b.b_08))
    {
      if (wordrg15 == con(0x3))
      {
        x16 wordrg2 = vw_24;
        if (urshift(wordrg2, con(0xe)) == unsignedword(eaxptim->v.b.b_08))
        {
          recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(wordrg2);
          recoptrrg5 = recoptrrg4;
          vw_20 = recoptrrg4->u2.w & con(0x7f);
          x16 wordrg3 = urshift(recoptrrg4->u2.w, con(0x7));
          vw_1c = vw_20 - con(0x7);
          if (UI16CAST(vw_1c) <= con(0x42))
          {
            switch (vw_1c)
            {
              case 0:
              case 1:
              case 2:
              case 3:
              case 7:
              case 8:
                SKW_ACTIVATE_SHOOTER(recoptrrg5, eaxptim);
              break;

              case 4:
              case 5:
              case 6:
              case 9:
              case 10:
              case 12:
              case 13:
              case 14:
              case 16:
              case 17:
              case 18:
              case 19:
              case 20:
              case 21:
              case 24:
              case 26:
              case 27:
              case 28:
              case 29:
              case 30:
              case 32:
              case 33:
              case 34:
              case 35:
              case 36:
              case 40:
              case 41:
              case 49:
              case 50:
              case 51:
              case 55:
              case 56:
              break;

              case 11:
                SKW_FIRE_SELECT_PALETTE_SET(0);
                ddata.v1e0240 = true;
                SKW_SK_PREPARE_EXIT();
              break;

              case 15:
              {
                vtim_00.un.l_00 = ddata.longmallocx | (UI32CAST(wordrg3 & con(0x3f)) << con(0x18));
                vtim_00.b_05 = eaxptim->b_05;
                vtim_00.u.b.b_06 = CUTX8(ulrshift(recoptrrg5->u6.w, con(0x5), con(0xb)));
                vtim_00.u.b.b_07 = CUTX8(urshift(recoptrrg5->u6.w, con(0xb)));
                x8*** hlpptr1 = &ddata.v1e03cc[wordrg3 & con(0x3f)];
                x8** hlpptr2 = *hlpptr1 + vtim_00.u.b.b_06;
                x8* hlpptr3 = *hlpptr2;
                if (ubrshift(hlpptr3[vtim_00.u.b.b_07], con(0x5)) != con(0x0))
                  vtim_00.v.b.b_08 = con(0x0);
                else
                  vtim_00.v.b.b_08 = CUTX8(urshift(wordrg3, con(0x6)) & con(0x3));
                vtim_00.b_04 = con(0x4);
                vtim_00.v.b.b_09 = eaxptim->v.b.b_09;
                SKW_QUEUE_TIMER(&vtim_00);
              }
              break;

              case 22:
              {
                bool boolrg2 = (recoptrrg4->u2.w & con(0xffffff80)) == con(0x0) || (urshift(wordrg2, con(0x7)) & con(0x100)) != con(0x0);
                if (eaxptim->v.b.b_09 != con(0x1))
                {
                  if (eaxptim->v.b.b_09 == con(0x0) && ((recoptrrg5->u4.b.b0 & con(0x4)) == con(0x0) || (recoptrrg5->u2.w & con(0xffffff80)) != con(0x0)))
                  {
                    x16 wordrg48 = ((urshift(recoptrrg5->u2.w, con(0x7)) - con(0x1)) & con(0x1ff)) << con(0x7);
                    recoptrrg5->u2.w &= con(0x7f);
                    recoptrrg5->u2.w |= wordrg48;
                  }
                }
                else
                  recoptrrg5->u2.w = (recoptrrg5->u2.w & con(0x7f)) | (((urshift(recoptrrg5->u2.w, con(0x7)) + con(0x1)) & con(0x1ff)) << con(0x7));
                bool boolrg1;
                if ((recoptrrg5->u2.w & con(0xffffff80)) == con(0x0) || (urshift(recoptrrg5->u2.w, con(0x7)) & con(0x100)) != con(0x0))
                  boolrg1 = true;
                else
                  boolrg1 = false;
                if (boolrg1 != boolrg2)
                {
                  if ((recoptrrg5->u4.w & con(0x18)) != con(0x18))
                  {
                    if (boolrg1)
                      SKW_INVOKE_ACTUATOR(recoptrrg5, con(0x0), ulrshift(recoptrrg5->u4.w, con(0xb), con(0xe)));
                  }
                  else
                    SKW_INVOKE_ACTUATOR(recoptrrg5, con(0x0), (boolrg1 ? 1 : 0) == ulrshift(recoptrrg5->u4.w, con(0xa), con(0xf)) ? 1 : 0);
                }
              }
              break;

              case 23:
              case 44:
              case 45:
              case 46:
              case 47:
              case 48:
              {
                x16 wordrg16 = 4 * (SKW_3a15_1da8(unsignedword(eaxptim->v.b.b_09), ulrshift(recoptrrg5->u4.w, con(0xd), con(0xf))) & con(0x1));
                recoptrrg5->u4.b.b0 &= con(0xfffffffb);
                recoptrrg5->u4.w |= wordrg16;
                if (((recoptrrg5->u4.b.b0 & con(0x1)) == con(0x0)) && ((recoptrrg5->u4.b.b0 & con(0x4)) != con(0x0)))
                  SKW_ACTIVATE_TICK_GENERATOR(recoptrrg5, vw_24);
              }
              break;

              case 25:
              case 62:
                recoptrrg4 = recoptrrg5;
                SKW_ACTIVATE_RELAY1(eaxptim, vw_20 == con(0x45), recoptrrg4);
              break;

              case 31:
              {
                x16 wordrg17 = 4 * (SKW_3a15_1da8(unsignedword(eaxptim->v.b.b_09), ulrshift(recoptrrg4->u4.w, con(0xd), con(0xf))) & con(0x1));
                recoptrrg5->u4.b.b0 &= con(0xfffffffb);
                recoptrrg5->u4.w |= wordrg17;
              }
              break;

              case 37:
                SKW_ACTIVATE_CONTINUOUS_ORNATE_ANIMATOR(vw_24, recoptrrg4, true, eaxptim);
              break;

              case 38:
                if ((recoptrrg4->u2.w & con(0xffffff80)) == con(0x0) || UI16CAST(recoptrrg4->u2.w & con(0xffffff80)) > UI16CAST(con(0xffffc800)))
                {
                  ui16 uwordrg13 = recoptrrg5->u2.w & con(0xffffff80);
                  if (uwordrg13 > con(0xc800) && uwordrg13 < con(0xfa00))
                  {
                    ui16 uwordrg410 = urshift(recoptrrg5->u2.w, con(0x7)) - con(0x190);
                    bool boolrg13 = (UI16CAST(SKW_RAND16(con(0x64))) < uwordrg410);
                    if ((recoptrrg5->u4.w & con(0x18)) != con(0x18))
                    {
                      if (boolrg13)
                        SKW_INVOKE_ACTUATOR(recoptrrg5, con(0x0), unsignedword(eaxptim->v.b.b_09));
                    }
                    else
                      SKW_INVOKE_ACTUATOR(recoptrrg5, con(0x0), !boolrg13 ? 1 : 0);
                  }
                }
                else
                {
                  x16 wordrg19 = ((urshift(recoptrrg4->u2.w, con(0x7)) - con(0x1)) & con(0x1ff)) << con(0x7);
                  recoptrrg4->u2.w &= con(0x7f);
                  recoptrrg5->u2.w |= wordrg19;
                  SKW_INVOKE_ACTUATOR(recoptrrg5, con(0x0), unsignedword(eaxptim->v.b.b_09));
                }
              break;

              case 39:
                if (eaxptim->v.b.b_09 == con(0x0))
                {
                  vw_0c = ulrshift(recoptrrg4->u6.w, con(0x5), con(0xb));
                  vw_28 = urshift(recoptrrg4->u6.w, con(0xb));
                  vw_18 = con(0x7);
                  x16 wordrg24;
                  if ((recoptrrg4->u4.b.b0 & con(0x4)) == con(0x0))
                    wordrg24 = ulrshift(recoptrrg4->u4.w, con(0xb), con(0xe));
                  else
                    wordrg24 = SKW_RAND02();
                  vw_10 = urshift(recoptrrg5->u2.w, con(0x7));
                  x16 wordrg18 = SKW_ALLOC_NEW_CREATURE(vw_10, wordrg24, vw_0c, vw_18, vw_28);
                  if ((recoptrrg5->u4.b.b0 & con(0x20)) != con(0x0) && wordrg18 != con(0xffffffff))
                    SKW_GET_ADDRESS_OF_RECORD(wordrg18)->w8 = ulrshift(recoptrrg5->u4.w, con(0x5), con(0xc));
                  if ((recoptrrg5->u4.b.b0 & con(0x40)) != con(0x0))
                    SKW_QUEUE_NOISE_GEN1(con(0x3), con(0x89), con(0x61), con(0x0), con(0x80), vw_0c, vw_28, con(0x1));
                }
              break;

              case 42:
                if ((recoptrrg4->u4.b.b0 & con(0x1)) == con(0x0))
                {
                  x32 longrg2 = unsignedlong(wordrg3) + ddata.longmallocx;
                  longrg2 |= signedlong(ddata.v1d3248) << con(0x18);
                  vtim_00.un.l_00 = longrg2;
                  vtim_00.b_04 = con(0x5b);
                  vtim_00.b_05 = con(0x0);
                  vtim_00.u.w_06 = vw_24;
                  SKW_QUEUE_TIMER(&vtim_00);
                  recoptrrg4->u4.b.b0 |= con(0x1);
                  if (((recoptrrg4->u4.b.b0 & con(0x20)) == con(0x0) && eaxptim->v.b.b_09 == con(0x0)) || ((recoptrrg5->u4.b.b0 & con(0x20)) != con(0x0) && eaxptim->v.b.b_09 == con(0x1)))
                  {
                    x16 wordrg17;
                    if ((recoptrrg5->u4.b.b0 & con(0x4)) == con(0x0))
                      wordrg17 = unsignedword(eaxptim->v.b.b_09);
                    else
                      wordrg17 = ulrshift(recoptrrg5->u4.w, con(0xb), con(0xe));
                    SKW_INVOKE_ACTUATOR(recoptrrg5, con(0x0), wordrg17);
                  }
                }
              break;

              case 43:
                SKW_ACTIVATE_ORNATE_ANIMATOR(eaxptim, wordrg2, con(0x1), recoptrrg4);
              break;

              case 52:
                SKW_ACTIVATE_ITEM_TELEPORT(eaxptim, wordrg2, false, recoptrrg4, false, NULL, false, false);
              break;

              case 53:
                if (((recoptrrg4->u4.b.b0 & con(0x20)) == con(0x0) && eaxptim->v.b.b_09 == con(0x0)) || ((recoptrrg5->u4.b.b0 & con(0x20)) != con(0x0) && eaxptim->v.b.b_09 == con(0x1)))
                {
                  x16 wordrg31 = SKW_ALLOC_NEW_RECORD(SKW_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(urshift(recoptrrg5->u2.w, con(0x7))));
                  if (wordrg31 != con(0xffffffff))
                  {
                    SKW_SET_ITEMTYPE(wordrg31, SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(urshift(recoptrrg5->u2.w, con(0x7))) & con(0xff));
                    if ((recoptrrg5->u4.w & con(0x18)) == con(0x18))
                      SKW_SET_ITEM_IMPORTANCE(wordrg31, con(0x1));
                    recoptrrg4 = recoptrrg5;
                    SKW_MOVE_ITEM_TO(wordrg31, con(0xffffffff), con(0xffffffff), recoptrrg4, ulrshift(recoptrrg5->u6.w, con(0x5), con(0xb)), urshift(recoptrrg5->u6.w, con(0xb)), ulrshift(recoptrrg5->u6.w, con(0xa), con(0xe)), false, ulrshift(recoptrrg5->u4.w, con(0xd), con(0xf)));
                  }
                }
              break;

              case 54:
                SKW_ACTIVATE_RELAY2(eaxptim, wordrg3, recoptrrg4);
              break;

              case 57:
                SKW_ACTIVATE_ITEM_TELEPORT(eaxptim, wordrg2, false, recoptrrg4, true, NULL, false, false);
              break;

              case 58:
                if (eaxptim->v.b.b_09 != con(0x0))
                  wordrg3--;
                else
                  wordrg3++;
                recoptrrg5->u2.w = (recoptrrg5->u2.w & con(0x7f)) | (((wordrg3 % SKW_GET_ORNATE_ANIM_LEN(recoptrrg5, true)) & con(0x1ff)) << con(0x7));
              break;

              case 59:
                SKW_3a15_0d5c(eaxptim, recoptrrg4);
              break;

              case 60:
                SKW_ACTIVATE_INVERSE_FLAG(eaxptim, recoptrrg4);
              break;

              case 61:
                SKW_ACTIVATE_TEST_FLAG(eaxptim, recoptrrg4);
              break;

              case 63:
              {
                x16 wordrg413 = urshift(recoptrrg4->u6.w, con(0xb));
                recoptrrg1 = SKW_GET_ADDRESS_OF_TILE_RECORD(ulrshift(recoptrrg4->u6.w, con(0x5), con(0xb)), wordrg413);
                if (eaxptim->v.b.b_09 < con(0x1))
                {
                  if (eaxptim->v.b.b_09 == con(0x0))
                    recoptrrg1->u2.b.b1 |= con(0x20);
                }
                else if (eaxptim->v.b.b_09 <= con(0x1))
                  recoptrrg1->u2.b.b1 &= con(0xffffffdf);
                else
                {
                  if (eaxptim->v.b.b_09 == con(0x2))
                  {
                    x16 wordrg411 = ((urshift(4 * recoptrrg1->u2.w, con(0xf)) ^ con(0x1)) & con(0x1)) << con(0xd);
                    recoptrrg1->u2.b.b1 &= con(0xffffffdf);
                    recoptrrg1->u2.w |= wordrg411;
                  }
                }
              }
              break;

              case 64:
                SKW_ACTIVATE_ITEM_TELEPORT(eaxptim, con(0xffff), false, recoptrrg4, false, NULL, true, false);
              break;

              case 65:
                SKW_ACTIVATE_ITEM_TELEPORT(eaxptim, wordrg2, false, recoptrrg4, false, NULL, false, true);
              break;

              case 66:
                SKW_ACTIVATE_ITEM_TELEPORT(eaxptim, con(0xffff), false, recoptrrg4, false, NULL, true, true);
              break;

              default: throw(THROW_DMABORT);
            }
          }
        }
      }
      continue;
    }
    recoptrrg5 = SKW_GET_ADDRESS_OF_RECORD(vw_24);
    if ((recoptrrg5->u2.b.b0 & con(0x6)) != con(0x0) && (recoptrrg5->u2.w & con(0x6)) != con(0x2))
      continue;
    x16 wordrg13 = urshift(recoptrrg5->u2.w, con(0x3));
    ui16 uwordrg41 = urshift(wordrg13, con(0x8)) & con(0x1f);
    vw_20 = uwordrg41;
    if (uwordrg41 < con(0x7))
    {
      if (uwordrg41 != con(0x5))
        continue;
    }
    else if (uwordrg41 <= con(0x7))
      ;
    else
    {
      if (uwordrg41 == con(0x17))
        if (eaxptim->v.b.b_09 == con(0x0))
          SKW_QUEUE_NOISE_GEN1(con(0x3), CUTX8(wordrg13), con(0x73), CUTX8(ddata.v1d3248) + con(0x1), con(0xc8), unsignedword(eaxptim->u.b.b_06), unsignedword(eaxptim->u.b.b_07), con(0x1));
      continue;
    }
    ubwptrrg1 = UPCAST(u_bbw, &recoptrrg5->u2);
    if (eaxptim->v.b.b_09 != con(0x2))
      flag = eaxptim->v.b.b_09 == con(0x0);
    else
      flag = (recoptrrg5->u2.b.b0 & con(0x1)) == con(0x0);
    ubwptrrg1->b.b0 &= con(0xfffffffe);
    ubwptrrg1->w |= flag ? 1 : 0;
    if (vw_20 == con(0x7))
    {
      x16 wordrg42 = (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x9), con(0xb), con(0x11), CUTX8(urshift(recoptrrg5->u2.w, con(0x3)))) == con(0x0)) ? con(0x1) : con(0xffffffff);
      vw_0c = unsignedword(eaxptim->u.b.b_06);
      vw_28 = unsignedword(eaxptim->u.b.b_07);
      x16 wordrg12 = SKW_LOCATE_OTHER_LEVEL(ddata.v1e0266, &vw_0c, &vw_28, wordrg42, NULL);
      if (wordrg12 >= con(0x0))
      {
        vw_14 = ddata.v1d3248;
        SKW_CHANGE_CURRENT_MAP_TO(wordrg12);
        x16 wordrg26 = SKW_GET_TILE_RECORD_LINK(vw_0c, vw_28);
        while (wordrg26 != con(0xfffffffe))
        {
          if (urshift(wordrg26 & con(0x3c), con(0xa)) == con(0x2))
          {
            if (urshift(wordrg26, con(0xe)) == unsignedword(eaxptim->v.b.b_08))
            {
              recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg26);
              if ((recoptrrg1->u2.w & con(0x6)) == con(0x2))
              {
                x16 wordrg43 = recoptrrg1->u2.w;
                vw_10 = urshift(wordrg43, con(0x3));
                if ((urshift(vw_10, con(0x8)) & con(0x1f)) == con(0x7))
                {
                  x16 wordrg32 = recoptrrg5->u2.w & con(0x1);
                  wordrg43 &= con(0xfffffffe);
                  recoptrrg1->u2.w = wordrg43;
                  recoptrrg1->u2.w |= wordrg32;
                }
              }
            }
          }
          wordrg26 = SKW_GET_NEXT_RECORD_LINK(wordrg26);
        }
        SKW_CHANGE_CURRENT_MAP_TO(vw_14);
      }
    }
  }
}

void SKW_ACTUATE_FLOOR_MECHA(c_tim* eaxptim)
{
  c_record* recoptrrg4;
  c_record* recoptrrg3;
  x16* wptrrg2;
  t_text t_00[0xc8];
  c_tim vtim_c8;
  x16 vw_e4;
  x8 vb_e8;

  x16 vw_dc = unsignedword(eaxptim->u.b.b_07);
  vw_e4 = unsignedword(eaxptim->u.b.b_06);
  x16 wordrg6;
  x16 wordrg1 = SKW_GET_TILE_RECORD_LINK(vw_e4, vw_dc);
  for (;; wordrg1 = SKW_GET_NEXT_RECORD_LINK(wordrg6))
  {
    wordrg6 = wordrg1;
    if (wordrg6 == con(0xfffffffe))
      return;
    x16 wordrg19 = urshift(wordrg6 & con(0x3c00), con(0xa));
    if (wordrg19 > con(0x3))
      return;
    if (wordrg19 != con(0x2))
    {
      if (wordrg19 == con(0x3))
      {
        recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
        recoptrrg3 = recoptrrg4;
        x16 wordrg100 = recoptrrg4->u2.w & con(0x7f);
        if (wordrg100 < con(0x3b))
        {
          if (wordrg100 < con(0x28))
          {
            if (wordrg100 < con(0x20))
            {
              if (wordrg100 == con(0xb))
              {
                x16 wordrg103 = urshift(recoptrrg3->u2.w, con(0x7));
                x16 wordrg414 = urshift(wordrg103, con(0x4)) & con(0x1f);
                SKW_ACTIVATE_CREATURE_KILLER(wordrg103 & con(0xf), vw_e4, vw_dc, wordrg414, ulrshift(recoptrrg3->u6.w, con(0x5), con(0xb)), urshift(recoptrrg3->u6.w, con(0xb)), wordrg100, unsignedword(eaxptim->v.b.b_09));
              }
            }
            else if (wordrg100 <= con(0x20))
            {
              recoptrrg4 = recoptrrg3;
              SKW_ACTIVATE_RELAY1(eaxptim, wordrg100 == con(0x45), recoptrrg3);
            }
            else if (wordrg100 == con(0x27))
            {
              recoptrrg3->u4.b.b0 &= con(0xfffffffe);
              recoptrrg3->u4.w |= SKW_3a15_1da8(unsignedword(eaxptim->v.b.b_09), recoptrrg4->u4.w & con(0x1)) & con(0x1);
            }
          }
          else if (wordrg100 <= con(0x28))
          {
            x16 wordrg102 = urshift(recoptrrg3->u2.w, con(0x7));
            x16 wordrg413 = urshift(wordrg102, con(0x4)) & con(0x1f);
            SKW_ACTIVATE_CREATURE_KILLER(wordrg102 & con(0xf), vw_e4, vw_dc, wordrg413, ulrshift(recoptrrg3->u6.w, con(0x5), con(0xb)), urshift(recoptrrg3->u6.w, con(0xb)), wordrg100, unsignedword(eaxptim->v.b.b_09));
          }
          else if (wordrg100 < con(0x2e))
          {
            if (wordrg100 == con(0x2c))
              SKW_ACTIVATE_CONTINUOUS_ORNATE_ANIMATOR(wordrg6, recoptrrg3, false, eaxptim);
          }
          else if (wordrg100 <= con(0x2e))
          {
            x16 wordrg412;
            x16 wordrg101;
            if ((recoptrrg4->u4.b.b0 & con(0x4)) == con(0x0))
            {
              wordrg412 = vw_e4;
              wordrg101 = vw_dc;
            }
            else
            {
              wordrg412 = ulrshift(recoptrrg3->u6.w, con(0x5), con(0xb));
              wordrg101 = urshift(recoptrrg3->u6.w, con(0xb));
            }
            x16 vw_d4;
            if ((recoptrrg3->u4.b.b0 & con(0x20)) == con(0x0))
              vw_d4 = (ulrshift(recoptrrg3->u4.w, con(0xb), con(0xe)) + ddata.v1e0258) & con(0x3);
            else
              vw_d4 = ulrshift(recoptrrg3->u4.w, con(0xb), con(0xe));
            R_3BF83(wordrg412, ddata.v1d3248, vw_d4, wordrg101);
          }
          else if (wordrg100 < con(0x32))
            ;
          else if (wordrg100 <= con(0x32))
            SKW_ACTIVATE_ORNATE_ANIMATOR(eaxptim, wordrg6, con(0x0), recoptrrg4);
          else if (wordrg100 == con(0x3a))
            SKW_ANIMATE_CREATURE(vw_e4, eaxptim->v.b.b_09 == con(0x0), vw_dc);
        }
        else if (wordrg100 <= con(0x3b))
          SKW_ACTIVATE_ITEM_TELEPORT(eaxptim, wordrg6, true, recoptrrg4, false, NULL, false, false);
        else if (wordrg100 < con(0x44))
        {
          if (wordrg100 < con(0x40))
          {
            if (wordrg100 == con(0x3d))
              SKW_ACTIVATE_RELAY2(eaxptim, urshift(recoptrrg4->u2.w, con(0x7)), recoptrrg4);
          }
          else if (wordrg100 <= con(0x40))
            SKW_ACTIVATE_ITEM_TELEPORT(eaxptim, wordrg6, true, recoptrrg4, true, NULL, false, false);
          else if (wordrg100 < con(0x42))
            ;
          else if (wordrg100 <= con(0x42))
            SKW_3a15_0d5c(eaxptim, recoptrrg4);
          else
            SKW_ACTIVATE_INVERSE_FLAG(eaxptim, recoptrrg4);
        }
        else if (wordrg100 <= con(0x44))
          SKW_ACTIVATE_TEST_FLAG(eaxptim, recoptrrg4);
        else if (wordrg100 < con(0x47))
        {
          if (wordrg100 == con(0x45))
          {
            recoptrrg4 = recoptrrg3;
            SKW_ACTIVATE_RELAY1(eaxptim, wordrg100 == con(0x45), recoptrrg3);
          }
        }
        else if (wordrg100 <= con(0x47))
          SKW_ACTIVATE_ITEM_TELEPORT(eaxptim, con(0xffff), false, recoptrrg4, false, NULL, true, false);
        else if (wordrg100 <= con(0x48))
          SKW_ACTIVATE_ITEM_TELEPORT(eaxptim, wordrg6, false, recoptrrg4, false, NULL, false, true);
        else if (wordrg100 == con(0x49))
          SKW_ACTIVATE_ITEM_TELEPORT(eaxptim, con(0xffff), false, recoptrrg4, false, NULL, true, true);
      }
      continue;
    }
    recoptrrg4 = SKW_GET_ADDRESS_OF_RECORD(wordrg6);
    if (((recoptrrg4->u2.b.b0 & con(0x6)) != con(0x0)) && (recoptrrg4->u2.w & con(0x6)) != con(0x2))
      continue;
    x16 wordrg21 = recoptrrg4->u2.w;
    x16 wordrg12 = urshift(wordrg21, con(0x3));
    x16 wordrg7 = (wordrg12 >> con(0x9)) & con(0xf);
    x16 wordrg11 = urshift(wordrg12, con(0x8)) & con(0x1f);
    if (wordrg11 != con(0x5))
    {
      wordrg21 &= con(0x6);
      if (wordrg21 != con(0x4) || wordrg7 != con(0x2))
      {
        if ((wordrg11 != con(0x13) && wordrg11 != con(0x16)) || eaxptim->v.b.b_09 != con(0x0))
        {
          if (wordrg11 == con(0x17))
            if (eaxptim->v.b.b_09 == con(0x0))
              SKW_QUEUE_NOISE_GEN1(con(0x3), CUTX8(urshift(recoptrrg4->u2.w, con(0x3))), con(0x73), CUTX8(ddata.v1d3248) + con(0x1), con(0xc8), unsignedword(eaxptim->u.b.b_06), unsignedword(eaxptim->u.b.b_07), con(0x1));
        }
        else
        {
          x32 longrg21;
          if (wordrg11 != con(0x13))
            longrg21 = con(0x2000);
          else
            longrg21 = con(0x200);
          if ((recoptrrg4->u2.b.b0 & con(0x1)) != con(0x0))
          {
            if (wordrg11 != con(0x13))
              longrg21 *= con(0x2);
            else
              longrg21 *= con(0x4);
          }
          longrg21 += (2 * longrg21 - con(0x1)) & (SKW_RAND() & con(0xffff));
          longrg21 += ddata.longmallocx;
          longrg21 |= signedlong(ddata.v1d3248) << con(0x18);
          vtim_c8.un.l_00 = longrg21;
          vtim_c8.b_05 = con(0x0);
          vtim_c8.u.b.b_06 = eaxptim->u.b.b_06;
          vtim_c8.u.b.b_07 = eaxptim->u.b.b_07;
          vtim_c8.v.b.b_08 = con(0x0);
          vtim_c8.v.b.b_09 = CUTX8(urshift(recoptrrg4->u2.w, con(0x3)));
          vtim_c8.b_04 = con(0x5e);
          SKW_QUEUE_TIMER(&vtim_c8);
        }
        continue;
      }
    }
    x16 wordrg17 = recoptrrg4->u2.w & con(0x1);
    wptrrg2 = &recoptrrg4->u2.w;
    if (eaxptim->v.b.b_09 != con(0x2))
      vb_e8 = (eaxptim->v.b.b_09 == con(0x0)) ? 1 : 0;
    else
      vb_e8 = (wordrg17 == con(0x0)) ? 1 : 0;
    *wptrrg2 &= con(0xfffffffe); // extended to word
    *wptrrg2 |= unsignedword(vb_e8 & con(0x1));
    if (((recoptrrg4->u2.b.b0 & con(0x6)) == con(0x0) && wordrg17 == con(0x0) && (recoptrrg4->u2.b.b0 & con(0x1)) != con(0x0) && ddata.v1d3248 == ddata.v1e0282) && vw_e4 == ddata.v1e0280)
      if (vw_dc == ddata.v1e027e)
      {
        SKW_QUERY_MESSAGE_TEXT(t_00, con(0x1), wordrg6);
        SKW_DISPLAY_HINT_TEXT(con(0xf), t_00);
      }
  }
}

void SKW_ACTIVATE_RELAY2(c_tim* eaxptim, x16 ebxw, c_record* edxprec)
{
  x16 vw_00;

  x16 wordrg4;
  if ((edxprec->u4.w & con(0x18)) != con(0x18))
  {
    if ((((edxprec->u4.b.b0 & con(0x20)) != con(0x0)) || (eaxptim->v.b.b_09 != con(0x0))) && (((edxprec->u4.b.b0 & con(0x20)) == con(0x0)) || (eaxptim->v.b.b_09 != con(0x1))))
      return;
    wordrg4 = ulrshift(edxprec->u4.w, con(0xb), con(0xe));
  }
  else if ((edxprec->u4.b.b0 & con(0x20)) == con(0x0))
    wordrg4 = unsignedword(eaxptim->v.b.b_09);
  else
  {
    SKW_INVOKE_ACTUATOR(edxprec, con(0x0), unsignedword(eaxptim->v.b.b_09));
    if (ebxw == con(0x0))
      return;
    ui8 ubyterg1 = eaxptim->v.b.b_09;
    if (ubyterg1 < con(0x1))
    {
      if (ubyterg1 == con(0x0))
        vw_00 = con(0x1);
    }
    else if (ubyterg1 <= con(0x1))
      vw_00 = con(0x0);
    else if (ubyterg1 == con(0x2))
      vw_00 = con(0x2);
    wordrg4 = vw_00;
  }
  SKW_INVOKE_ACTUATOR(edxprec, ebxw, wordrg4);
}

x16 SKW_GET_ORNATE_ANIM_LEN(c_record* eaxprec, bool edxbool)
{
  x8 byterg5;
  t_text* tptrrg1;
  t_text t_00[0x80];

  x8 byterg4;
  x8 byterg1;
  if (!edxbool)
  {
    byterg4 = con(0xa);
    byterg1 = SKW_GET_FLOOR_DECORATION_OF_ACTUATOR(eaxprec);
  }
  else
  {
    byterg4 = con(0x9);
    byterg1 = SKW_GET_WALL_DECORATION_OF_ACTUATOR(eaxprec);
  }
  if (byterg1 == con(0xffffffff))
    return con(0x1);
  x8 byterg6 = byterg1;
  byterg5 = byterg4;
  x16 wordrg1 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(byterg5, con(0xb), con(0xd), byterg1);
  if (wordrg1 != con(0x0))
  {
    wordrg1 &= con(0x7fff);
    return wordrg1;
  }
  tptrrg1 = SKW_QUERY_GDAT_TEXT(byterg5, con(0xd), t_00, byterg6);
  if (*tptrrg1 == NULLBYTE)
    return con(0x1);
  x16 wordrg11 = SKW_SK_STRLEN(tptrrg1);
  if (wordrg11 != con(0x0))
    return wordrg11;
  return con(0x1);
}

// was SKW_3a15_0447
x16 SKW_GET_TIMER_NEW_INDEX(x16 eaxw)
{
  x16* wptrrg1;

  wptrrg1 = ddata.v1e1424;
  x16 wordrg4 = con(0x0);
  while (wordrg4 < ddata.savegamew9)
  {
    if (eaxw == *wptrrg1++)
      break;
    wordrg4++;
  }
  if (wordrg4 >= ddata.savegamew9)
    SKW_RAISE_SYSERR(con(0x46));
  return wordrg4;
}

void SKW_3a15_020f(void)
{
  c_tim* timptrrg2;

  x16 wordrg41 = con(0x0);
  while (wordrg41 < con(0x4))
  {
    ddata.hero[wordrg41].w2e = con(0xffffffff);
    wordrg41++;
  }
  x16 wordrg5 = ddata.savegamew5;
  if (wordrg5 == con(0x0))
    return;
  x16 wordrg42 = con(0x0);
  timptrrg2 = ddata.savegametimarrp;
  for(;;)
  {
    if (wordrg42 >= wordrg5)
      return;
    ui8 ubyterg1 = timptrrg2->b_04;
    if (ubyterg1 < UI8CAST(con(0xc)))
      ;
    else if (ubyterg1 <= con(0xc))
      ddata.hero[timptrrg2->b_05].w2e = wordrg42;
    else if (ubyterg1 >= con(0x1d) && ubyterg1 <= con(0x1e))
      SKW_GET_ADDRESS_OF_RECORD(timptrrg2->u.w_06)->u6.w = wordrg42;
    wordrg42++;
    timptrrg2++;
  }
}

void SKW_3a15_0002(void)
{
  c_tim* timptrrg1;

  if (ddata.savegametimarrp == NULL)
  {
    ddata.savegametimarrp = UPCAST(c_tim, DM2_ALLOC_FREEPOOL_MEMORY(12 * ddata.v1e142c));
    ddata.v1e1424 = UPCAST(x16, DM2_ALLOC_FREEPOOL_MEMORY(2 * signedlong(ddata.v1e142c)));
  }
  if (ddata.savegamew8 != con(0x0))
  {
    x16 wordrg3;
    timptrrg1 = ddata.savegametimarrp;
    x16 wordrg4 = con(0x0);
    while (wordrg3 = wordrg4, wordrg3 < ddata.v1e142c)
    {
      timptrrg1->b_04 = con(0x0);
      wordrg4++;
      timptrrg1->un.w_00 = wordrg4;
      timptrrg1++;
    }
    ddata.savegamew9 = con(0x0);
    c_tim* timptrrg2 = timptrrg1 - 1; // for better reading of the next line only
    timptrrg2->un.w_00 = con(0xffffffff);
    wordrg3 ^= wordrg4;
    ddata.savegamew5 = wordrg3;
    ddata.v1e142a = con(0x0);
  }
  ddata.v1e1428 = con(0xffffffff);
}

// was SKW_38c8_03ad
void SKW__INIT_GAME_38c8_03ad(void)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("SKW__INIT_GAME_38c8_03ad");
  ddata.v1e0228 = false;
  ddata.v1e0220 = false;
  ddata.v1e0224 = false;
  ddata.v1e03a8 = false;
  ddata.v1e0238 = false;
  ddata.v1e13f0 = false;
  DM2_ZERO_MEMORY(&ddata.v1e0ffa[2], con(0x10)); // EVIL offset shifted, was 0x1e0ffc
  DM2_ZERO_MEMORY(DOWNCAST(s_size8, ddata.v1e0adc), con(0x40));
  ddata.v1e0b74 = con(0x0);
  ddata.v1e00b8 = con(0x0);
  SKW_1031_0541(con(0x5));
  SKW_LOAD_NEWMAP(ddata.v1e0270, ddata.v1e0266, false, ddata.v1e0272);	// ddata.v1e0266 would be glbPlayerMap
  SKW_RESET_CAII();
  dm2_dballochandler.v1e0200 = false;
  if (ddata.savegamew8 != con(0x0) && !ddata.boolsavegamel2)
  {
    SKW_WAIT_SCREEN_REFRESH();
    ddata.dialog2 = false;
  }
  else
  {
    SKW_FIRE_FILL_SCREEN_RECT(con(0x2), con(0x0));
    SKW_0aaf_0067(SKW_0aaf_02f8(con(0x6), con(0x0)));
    x16 wordrg4 = ddata.gdatfileopencounter;
    if (wordrg4 != con(0x0))
    {
      ddata.gdatfileopencounter = con(0x1);
      DM2_GRAPHICS_DATA_CLOSE();
    }
    SKW_2066_03e0(con(0x0));
    if (wordrg4 != con(0x0))
    {
      DM2_GRAPHICS_DATA_OPEN();
      ddata.gdatfileopencounter = wordrg4;
    }
    SKW_WAIT_SCREEN_REFRESH();
  }
  SKW_FIRE_FILL_SCREEN_RECT(con(0x2), con(0x0));
  SKW_29ee_000f();
  SKW_2f3f_0789();
  ddata.v1e13f4 = true;
  ddata.v1e0248 = true;
  SKW_CHECK_RECOMPUTE_LIGHT();
SPX_DEBUG_POP;
}

// was SKW_32cb_5d0d
void SKW_DISPLAY_VIEWPORT(x16 eaxw, x16 ebxw, x16 edxw)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("SKW_DISPLAY_VIEWPORT");
  x16 vw_04 = ebxw;
  ddata.v1e040a = edxw;
  ddata.v1e040c = ebxw;
  ddata.v1e0406 = ddata.v1e0266;
  ddata.v1e0402 = eaxw;
  SKW_IBMIO_USER_INPUT_CHECK();
  ddata.v1e12d2 = ddata.v1e0286 * con(0xa);
  ddata.v1e12c8 = eaxw;
  ddata.v1e12cc = edxw;
  ddata.v1e12ca = ebxw;
  ddata.v1e1044 = UPCAST(s_hex12, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0x19e)));
  ddata.v1e1048 = UPCAST(s_hex11bytes, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0x166), true));
  ddata.v1e1050 = UPCAST(s_hex11bytes, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0x166)));
  SKW_FILL_STR(DOWNCAST(s_hex11bytes, ddata.v1e1050), con(0xff), con(0x1), con(0x166));
  ddata.v1e1038 = UPCAST(x32, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0x5c), true));
  ddata.v1e104c = UPCAST(x32, DM2_ALLOC_LOBIGPOOL_MEMORY(con(0x5c), true));
  ddata.v1e01d8 = true;
  SKW_INIT_BACKBUFF();
  ddata.v1e0404 = con(0x9);
  for (x16 wordrg5 = con(0x0); wordrg5 < con(0x9); wordrg5++)
  {
    ddata.v1e02f0[wordrg5].w8 = con(0xffff);
    ddata.v1e02f0[wordrg5].bb = con(0x0);
  }
  SKW_IBMIO_USER_INPUT_CHECK();
  if ((!ddata.v1e03a8) && (ddata.v1d4000 == con(0xffffffff)))
    ddata.v1e12ce = con(0x0);
  else
    ddata.v1e12ce = con(0x1);
  SKW_CHANCE_TABLE_OPERATION();
  for (x16 wordrg51 = con(0x16); wordrg51 >= con(0x0); wordrg51--)
    SKW_32cb_4185(edxw, wordrg51, ddata.v1e12c8, vw_04);
  x16 wordrg52 = con(0x0);
  x16 wordrg2 = con(0x0);
  x8 byterg4 = con(0xffffffff);
  if ((ddata.v1e1044[con(0x7)].s_02.warr_00[con(0x0)] == con(0x0)) && (ddata.v1e1044[con(0x6)].s_02.warr_00[con(0x0)] == con(0x0))) // offset was 0x80
    if (ddata.v1e1044[con(0x8)].s_02.warr_00[con(0x0)] == con(0x0)) // 0x92
      byterg4 = con(0x71);
  if ((ddata.v1e1044[con(0x4)].s_02.warr_00[con(0x0)] == con(0x0)) && (ddata.v1e1044[con(0x3)].s_02.warr_00[con(0x0)] == con(0x0))) // 0x4a
    if (ddata.v1e1044[con(0x5)].s_02.warr_00[con(0x0)] == con(0x0)) // 0x5c
      byterg4 = con(0x70);
  if (byterg4 != con(0xffffffff))
  {
    x16 wordrg11 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(con(0x8), con(0xb), byterg4, ddata.v1d6c02);
    wordrg52 = urshift(wordrg11, con(0x8));
    wordrg2 = unsignedword(CUTX8(wordrg11));
  }
  SKW_TRIM_BLIT_RECT(con(0x0), con(0x0), con(0x0), wordrg2);
  //SKW_DRAW_DUNGEON_GRAPHIC(con(0x8), con(0x1), con(0x2bc), ddata.v1d6c02, con(0xffff), (SKW_SET_GRAPHICS_FLIP_FROM_POSITION(con(0x20), edxw, vw_04, eaxw) == con(0x0)) ? con(0x0) : con(0x1));	// draw roof/ceil
  SKW_DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, GDAT_GFXSET_CEIL, con(0x2bc), ddata.v1d6c02, con(0xffff), 
	  (SKW_SET_GRAPHICS_FLIP_FROM_POSITION(con(0x20), edxw, vw_04, eaxw) == con(0x0)) ? con(0x0) : con(0x1));
  ddata.v1e01d0 = con(0x0);
  SKW_TRIM_BLIT_RECT(con(0x0), wordrg52, con(0x0), con(0x0));
  //SKW_DRAW_DUNGEON_GRAPHIC(con(0x8), con(0x0), con(0x2bd), ddata.v1d6c02, con(0xffff), (SKW_SET_GRAPHICS_FLIP_FROM_POSITION(con(0x1), edxw, vw_04, eaxw) == con(0x0)) ? con(0x0) : con(0x1));	// draw floor
  SKW_DRAW_DUNGEON_GRAPHIC(GDAT_CATEGORY_GRAPHICSSET, GDAT_GFXSET_FLOOR, con(0x2bd), ddata.v1d6c02, con(0xffff),
	  (SKW_SET_GRAPHICS_FLIP_FROM_POSITION(con(0x1), edxw, vw_04, eaxw) == con(0x0)) ? con(0x0) : con(0x1));

  ddata.v1e01d0 = con(0x0);
  ddata.v1e12d0 = SKW_SET_GRAPHICS_FLIP_FROM_POSITION(con(0x0), edxw, vw_04, eaxw);
  SKW_IBMIO_USER_INPUT_CHECK();
  SKW_ENVIRONMENT_DISPLAY_ELEMENTS(eaxw, vw_04, edxw);	/// direction, y, x
  SKW_32cb_5c67();
  SKW_32cb_5a8f();
  SKW_DRAW_DUNGEON_TILES();
  SKW_IBMIO_USER_INPUT_CHECK();
  SKW_DRAW_PLAYER_TILE();
  SKW_IBMIO_USER_INPUT_CHECK();
  DM2_DEALLOC_LOBIGPOOL(con(0x522));
SPX_DEBUG_POP;
}

// was SKW_32cb_59ca
x16 SKW_SET_GRAPHICS_FLIP_FROM_POSITION(ui16 eaxuw, x16 ebxw, x16 ecxw, x16 edxw)
{
  s_size10* sptrrg1;

  sptrrg1 = ddata.v1e03c0;
  x16 wordrg2 = unsignedword(sptrrg1->b4[con(0x3)]) + edxw + ecxw + ebxw + unsignedword(sptrrg1->b4[con(0x2)]);
  x16 wordrg1 = ((sptrrg1->w8 & con(0x3f)) + wordrg2) & con(0x1);
  if (eaxuw < con(0x1))
    return wordrg1;
  ui32 ulongrg2 = ddata.longmallocx & con(0x7);
  x16 wordrg4 = ddata.v1e0258 & con(0x1);
  if (eaxuw <= con(0x1))
  {
    if ((ddata.v1e12d6 & con(0x8)) == con(0x0))
    {
      wordrg1 = ddata.v1e12d6 & con(0x40);
      if (wordrg1 == con(0x0))
        return wordrg1;
      return wordrg4;
    }
    if ((ddata.v1e12d6 & con(0x10)) == con(0x0))
      return wordrg1;
    return (ulongrg2 > con(0x3)) ? 1 : 0;
  }
  if (eaxuw != con(0x20))
    return wordrg1;
  if ((ddata.v1e12d6 & con(0x2)) == con(0x0))
  {
    wordrg1 = ddata.v1e12d6 & con(0x20);
    if (wordrg1 == con(0x0))
      return wordrg1;
    return wordrg4;
  }
  bool boolrg1;
  if ((ddata.v1e12d6 & con(0x4)) == con(0x0))
    boolrg1 = wordrg1 == con(0x0);
  else
    boolrg1 = ulongrg2 <= con(0x3);
  return boolrg1 ? 1 : 0;
}

void SKW_SET_ITEM_IMPORTANCE(x16 eaxw, x16 edxw)
{
  c_record* recoptrrg1;

  recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(eaxw);
  ui16 uwordrg3 = urshift(eaxw & con(0x3c00), con(0xa)) - con(0x4);
  if (uwordrg3 > con(0x6))
    return;
  x16 wordrg4 = edxw & con(0x1);
  x16 wordrg2 = wordrg4 << con(0x7);
  switch (uwordrg3)
  {
    case 0:
      recoptrrg1->ue.b.b1 &= con(0xfffffffb);
      wordrg4 <<= con(0xa);
      recoptrrg1->ue.w |= wordrg4;
    return;

    case 1:
    case 2:
    case 6:
      recoptrrg1->u2.b.b0 &= con(0x7f);
      recoptrrg1->u2.w |= wordrg2;
    return;

    case 3:
    case 5:
    return;

    case 4:
      recoptrrg1->u2.b.b1 &= con(0x7f);
      wordrg4 <<= con(0xf);
      recoptrrg1->u2.w |= wordrg4;
    return;

    default: throw(THROW_DMABORT);
  }
}

x8 SKW_QUERY_CLS2_FROM_RECORD(x16 eaxw)
{
  c_record* recoptrrg1;

  x16 wordrg41 = eaxw;
  for(;;)
  {
    if (wordrg41 == con(0xffffffff))
      return con(0xffffffff);
    if (UI16CAST(wordrg41) >= UI16CAST(con(0xffffff80)))
      return CUTX8(wordrg41) - con(0xffffff80);
    x16 wordrg2 = wordrg41;
    recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(wordrg2);
    ui16 uwordrg42 = urshift(wordrg41 & con(0x3c00), con(0xa));
    if (uwordrg42 > con(0xf))
      return con(0xffffffff);
    switch (uwordrg42)
    {
      case 0:
      case 1:
      case 11:
      case 12:
      case 13:
      return con(0xffffffff);

      case 2:
      return SKW_QUERY_CLS2_OF_TEXT_RECORD(wordrg2);

      case 3:
      return SKW_GET_WALL_DECORATION_OF_ACTUATOR(SKW_GET_ADDRESS_OF_RECORD(wordrg2));

      case 4:
      return recoptrrg1->u4.b.b0;

      case 5:
      case 6:
      case 10:
      case 15:
      return CUTX8(recoptrrg1->u2.w & con(0x7f));

      case 7:
      return con(0x0);

      case 8:
      return CUTX8(ulrshift(recoptrrg1->u2.w, con(0x1), con(0x9)));

      case 9:
      return CUTX8(urshift(recoptrrg1->u4.w, con(0xd))) | CUTX8(8 * ulrshift(recoptrrg1->u4.w, con(0xd), con(0xe)));

      case 14:
        wordrg41 = recoptrrg1->u2.w;
      break;

      default: throw(THROW_DMABORT);
    }
  }
}

// TODO: make eaxw unsigned
void SKW_SET_ITEMTYPE(x16 eaxw, x8 edxb)
{
  c_record* recoptrrg1;

  if (eaxw == con(0xffffffff))
    return;
  if (UI16CAST(eaxw) >= UI16CAST(con(0xffffff80)))
    return;
  recoptrrg1 = SKW_GET_ADDRESS_OF_RECORD(eaxw);
  ui16 uwordrg2 = urshift(eaxw & con(0x3c00), con(0xa)) - con(0x4);
  if (uwordrg2 > con(0x6))
    return;
  switch (uwordrg2)
  {
    case 0:
      recoptrrg1->u4.b.b0 = edxb;
    return;

    case 1:
    case 2:
    case 6:
      recoptrrg1->u2.b.b0 &= con(0xffffff80);
      recoptrrg1->u2.w |= unsignedword(edxb) & con(0x7f);
    return;

    case 3:
    return;

    case 4:
      recoptrrg1->u2.b.b1 &= con(0xffffff80);
      recoptrrg1->u2.w |= (unsignedword(edxb) & con(0x7f)) << con(0x8);
    return;

    case 5:
      recoptrrg1->u4.b.b0 &= con(0xfffffff9);
      recoptrrg1->u4.w |= 2 * ((unsignedword(edxb) / 8) & con(0x3));
      recoptrrg1->u4.b.b1 &= con(0x1f);
      recoptrrg1->u4.w |= (unsignedword(edxb) & con(0x7)) << con(0xd);
      if ((recoptrrg1->u4.w & con(0x6)) == con(0x2))
        recoptrrg1->u6.w = con(0xffffffff);
    return;

    default: throw(THROW_DMABORT);
  }
}

void SKW_1031_111e(x16 eaxw)
{
  x8 byterg4h;
  t_gfxdata* gptrrg1;
  t_gfxdata* gptrrg11;
  t_gfxdata* gptrrg12;
  t_gfxdata* gptrrg13;
  t_gfxdata* gptrrg14;
  t_gfxdata* gptrrg15;
  t_gfxdata* gptrrg16;
  t_gfxdata* gptrrg17;
  t_gfxdata* gptrrg18;
  t_gfxdata* gptrrg2;
  t_gfxdata* gptrrg4;
  s_gfxdatatail* ptrbeforerg1;
  s_gfxdatatail* ptrbeforerg11;
  c_c5rects vs_00;
  c_rect rc_34;
  x16 vw_3c;
  x16 vw_40;
  x8 vb_44;

  if (ddata.v1e052e == con(0x0))
    ;
  else if (eaxw >= ddata.v1e052e)
    ddata.v1e052e = con(0x0);
  else
  {
    ddata.v1e052e -= eaxw;
    return;
  }
  if (ddata.mouse_unk02 != con(0x0))
    return;
  if (ddata.v1e0478)
    return;
  if (ddata.v1e048c)
    return;
  if (ddata.v1e0480 == NULL)
    return;
  vs_00.s_00.w_00 = con(0xffffffff);
  for(;;)
  {
    gptrrg11 = ddata.v1e0480;
    gptrrg4 = &gptrrg11[con(0x1)];
    ddata.v1e0480 = gptrrg4;
    vb_44 = *gptrrg11;
    ui16 uwordrg11 = unsignedword(vb_44) & con(0x3f);
    if (uwordrg11 <= con(0x10))
    {
      // added: flag 'doit' set when M_11568 is to be done first
      bool doit = false;
      switch (uwordrg11)
      {
        case 0:
          gptrrg12 = gptrrg4;
          gptrrg4++;
          ddata.v1e0480 = gptrrg4;
          ddata.v1e052e = unsignedword(*gptrrg12);
        break;

        case 1:
          doit = true;
        break;

        case 2:
          ddata.v1e0478 = true;
        break;

        case 3:
          ddata.v1e048c = true;
        break;

        case 4:
        case 6:
          {
            x16 wordrg106 = ddata.mouse_unk04 + con(0x1);
            if (wordrg106 > con(0x2))
              wordrg106 = con(0x0);
            if (wordrg106 != ddata.mouse_unk03)
            {
              SKW_COPY_MEMORY(DOWNCAST(c_event, &ddata.v1e04d2), sizeof(c_event), DOWNCAST(c_event, &table1e04e0[ddata.mouse_unk04 / 2]));
              ddata.mouse_unk04 = wordrg106;
              if ((vb_44 & con(0x3f)) != con(0x4))
                doit = true;
              break; //!
            }
            if (ddata.mouse_unk02 == con(0x0))
              ddata.mouse_unk02 = con(0x1);
            ddata.v1e0480--;
            if (vs_00.s_00.w_00 != con(0xffffffff))
              SKW_0b36_0cbe(&vs_00, true);
          }
        return; //!

        case 5:
          if (!SKW_HANDLE_UI_EVENT(&ddata.v1e04d2))
            ddata.v1e0480--;
          else if (ddata.v1e0488)
            doit = true;
        break;

        case 7:
          {
            gptrrg13 = gptrrg4;
            gptrrg4++;
            ddata.v1e0480 = gptrrg4;
            x8 byterg4 = *gptrrg13;
            gptrrg13 = ddata.v1e0480;
            gptrrg2 = &gptrrg13[con(0x1)];
            ddata.v1e0480 = gptrrg2;
            byterg4h = *gptrrg13;
            gptrrg13 = gptrrg2;
            gptrrg2++;
            ddata.v1e0480 = gptrrg2;
            x8 byterg6 = *gptrrg13;
            gptrrg1 = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(byterg4, byterg6, byterg4h);
            ptrbeforerg1 = getprevious(gptrrg1);
            SKW_1031_10c8(&vs_00, ptrbeforerg1->width, ptrbeforerg1->height, &rc_34);
            gptrrg14 = ddata.v1e0480;
            ddata.v1e0480++;
//                                                srcbmp                                                                                                                            palette
            SKW_DRAW_ICON_PICT_BUFF(SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(byterg4, byterg6, byterg4h), &rc_34, con(0x0), &vs_00, con(0x0), signedword(*gptrrg14), con(0x0), SKW_QUERY_GDAT_IMAGE_LOCALPAL(byterg4, byterg6, byterg4h));
          }
        break;

        case 8:
          {
            gptrrg15 = gptrrg4;
            gptrrg4++;
            ddata.v1e0480 = gptrrg4;
            x16 wordrg105 = SKW_GET_PLAYER_AT_POSITION((unsignedword(*gptrrg15) + ddata.v1e0258) & con(0x3));
            if (wordrg105 >= con(0x0))
            {
              SKW_DRAW_SQUAD_SPELL_AND_LEADER_ICON(wordrg105, true);
              SKW_0b36_0cbe(&ddata.v1d694c, true);
            }
          }
        break;

        case 9:
          {
            x16 wordrg104;
            if (ddata.v1e0b74 <= con(0x0))
            {
              ui16 uwordrg1 = ddata.v1e04d2.warr0[2];
              if (uwordrg1 >= con(0x74) && uwordrg1 <= con(0x7b))
                wordrg104 = (ddata.v1e04d2.warr0[2] - con(0x74)) / 2;
              else
              {
                ddata.v1e0480++;
                break; // !
              }
            }
            else
              wordrg104 = ddata.v1e0b74 - con(0x1);
            gptrrg4 = ddata.v1e0480;
            ddata.v1e0480 = &gptrrg4[con(0x1)];
            SKW_DRAW_HAND_ACTION_ICONS(wordrg104, con(0x1), unsignedword(*gptrrg4));
            SKW_0b36_0cbe(&ddata.v1d694c, true);
          }
        break;

        case 10:
          ptrbeforerg11 = getprevious(SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x1), con(0x8), con(0x5)));
          SKW_1031_10c8(&vs_00, ptrbeforerg11->width, ptrbeforerg11->height, &rc_34);
//                                           srcbmp                                                                                                                        palette
          SKW_DRAW_ICON_PICT_BUFF(SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(con(0x1), con(0x8), con(0x5)), &rc_34, con(0x0), &vs_00, con(0x0), con(0xffff), con(0x0), SKW_QUERY_GDAT_IMAGE_LOCALPAL(con(0x1), con(0x8), con(0x5)));
          *ddata.v1e052c = ddata.hero[ddata.v1e0b74 - con(0x1)].b1e * con(0x6) + con(0x60) + CUTX8(ddata.v1e04d2.warr0[2]) - con(0x65);
          C_strmetrics.SKW_QUERY_STR_METRICS(ddata.v1e052c, OO vw_3c, OO vw_40);
          SKW_CALC_CENTERED_RECT_IN_RECT(&rc_34, vw_40, vw_3c, &vs_00.s_00.rc_02);
          SKW_0b36_129a(&vs_00, rc_34.y + rc_34.h - con(0x1), unsignedword(ddata.paletteptr1[con(0x0)]), rc_34.x, unsignedword(ddata.paletteptr1[con(0x4)]), ddata.v1e052c);
        break;

        case 11:
          SKW_DRAW_SPELL_TO_BE_CAST(true);
          SKW_0b36_0cbe(&ddata.v1d694c, true);
        break;

        case 12:
          gptrrg16 = gptrrg4;
          gptrrg4++;
          ddata.v1e0480 = gptrrg4;
          SKW_DRAW_CMD_SLOT(unsignedword(*gptrrg16), true);
          SKW_0b36_0cbe(&ddata.v1d694c, true);
        break;

        case 13:
          if (ddata.v1e0976 != con(0x0))
          {
            ddata.v1e100c |= unsignedword(*gptrrg4);
            SKW_REFRESH_PLAYER_STAT_DISP(ddata.v1e0976 - con(0x1));
          }
          ddata.v1e0480++;
        break;

        case 14:
          gptrrg17 = gptrrg4;
          gptrrg4++;
          ddata.v1e0480 = gptrrg4;
          SKW_29ee_1d03(unsignedword(*gptrrg17));
          SKW_0b36_0cbe(&ddata.v1d694c, true);
        break;

        case 15:
        case 16:
          gptrrg18 = ddata.v1e0480;
          gptrrg4 = gptrrg18 + con(0x1);
          ddata.v1e0480 = gptrrg4;
          ddata.v1e0206 = *gptrrg18;
          gptrrg18 = gptrrg4;
          gptrrg4++;
          ddata.v1e0480 = gptrrg4;
          ddata.v1e0207 = *gptrrg18;
          gptrrg18 = gptrrg4;
          gptrrg4++;
          ddata.v1e0480 = gptrrg4;
          ddata.v1e0208 = *gptrrg18;
          SKW_0aaf_01db(ddata.mouse_unk05, true);
          vs_00.s_00.w_00 = con(0xffffffff);
          SKW_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
        break;

        default: throw(THROW_DMABORT);
      }
      if (doit)
      {
        ddata.mouse_unk02 = ddata.mouse_unk04 - ddata.mouse_unk03;
        if (ddata.mouse_unk02 < con(0x0))
          ddata.mouse_unk02 += con(0x3);
      }
    }
    if ((vb_44 & con(0xffffff80)) == con(0x0))
    {
      if ((vb_44 & con(0x40)) == con(0x0))
        break;
    }
    else
    {
      ddata.v1e0480 = NULL;
      break;
    }
  }
  if (vs_00.s_00.w_00 != con(0xffffffff))
    SKW_0b36_0cbe(&vs_00, true);
}

void R_13262(x16 eaxw)
{
  x16 wordrg4 = con(0x0);
  for(;;)
  {
    if (wordrg4 >= ddata.v1e0404)
      return;
    // next line index RG4L was RG2L - BUGFIX 3/2014
    if (signedword(ddata.v1e02f0[wordrg4].ba) == eaxw && (ddata.v1e02f0[wordrg4].bb == con(0x4) || ddata.v1e02f0[wordrg4].bb == con(0x6)))
      break;
    wordrg4++;
  }
  SKW_CLICK_VWPT(ddata.v1e02f0[wordrg4].r0.x + ddata.v1d26fc, ddata.v1e02f0[wordrg4].r0.y + ddata.v1d26fe);
}

void SKW_RESET_SQUAD_DIR(void)
{
  ui16 uwordrg4 = con(0x0);
  while (uwordrg4 < UI16CAST(ddata.savegamew2))
  {
    ddata.hero[uwordrg4].b1c = CUTX8(ddata.v1e0258);
    uwordrg4++;
  }
}

// TODO: make eaxw unsigned
void SKW_PUSH_PULL_RIGID_BODY(x16 eaxw)
{
  x16 vw_00;
  x16 vw_04;
  x16 vw_08;

  if (UI16CAST(eaxw) > con(0x5))
    return;
  x16 wordrg6;
  x16 vw_0c;
  x16 wordrg5;
  switch (eaxw)
  {
    case 0:
      wordrg6 = ddata.v1e0258;
      wordrg5 = con(0x0);
    break;

    case 1:
      wordrg6 = (ddata.v1e0258 + con(0x1)) & con(0x3);
      wordrg5 = con(0x2);
    break;

    case 2:
      vw_0c = con(0x4);
      wordrg5 = con(0x1);
    break;

    case 3:
      vw_0c = con(0x5);
      wordrg5 = con(0x0);
    break;

    case 4:
      vw_0c = con(0x6);
      wordrg5 = con(0x2);
    break;

    case 5:
      wordrg6 = (ddata.v1e0258 - con(0x1)) & con(0x3);
      wordrg5 = con(0x1);
    break;

    default: throw(THROW_DMABORT);
  }
  switch (eaxw)
  {
    case 0:
    case 1:
    case 5:
      vw_0c = con(0x3);
    break;

    case 2:
    case 3:
    case 4:
      wordrg6 = (ddata.v1e0258 + con(0x2)) & con(0x3);
    break;
  }
  vw_08 = ddata.v1e0270 + table1d27fc[ddata.v1e0258];
  vw_00 = ddata.v1e0272 + table1d2804[ddata.v1e0258];
  if (!SKW_IS_CREATURE_MOVABLE_THERE(vw_08, wordrg6, &vw_04, vw_00))
    return;
  if (SKW_12b4_099e(vw_04))
  {
    ddata.v1e053c = vw_08;
    ddata.v1e053a = vw_00;
    ddata.v1e0546 = ddata.v1e0270;
    ddata.v1e0544 = ddata.v1e0272;
    ddata.v1e0542 = wordrg6;
    ddata.v1e0540 = ddata.v1e0266;
    ddata.v1e0538 = wordrg5;
    ddata.v1d4000 = vw_04;
    SKW_PERFORM_MOVE(vw_0c);
    return;
  }
  for (ui16 vuw_0c = con(0x0); vuw_0c < ddata.savegamew2; vuw_0c++)
    if (ddata.hero[vuw_0c].w36 != con(0x0))
      if (vuw_0c == ddata.mouse_unk0e || SKW_RAND02() == con(0x0))
        SKW_QUEUE_NOISE_GEN2(con(0x16), con(0x82), con(0xfe), CUTX8(vuw_0c), ddata.v1e0270, ddata.v1e0272, con(0x1), con(0x69), con(0xc8));
}

bool SKW_IS_CREATURE_MOVABLE_THERE(x16 eaxw, x16 ebxw, x16* ecxpw, x16 edxw)
{
  c_5bytes vs_00;
  x16 vw_0c;
  x16 vw_10;

  x16 wordrg6 = eaxw;
  x16 wordrg5 = edxw;
  vw_10 = ebxw;
  x16 vw_08 = SKW_GET_CREATURE_AT(wordrg6, wordrg5);
  if (vw_08 == con(0xffffffff))
    return false;
  if (SKW_GET_CREATURE_WEIGHT(vw_08) > con(0xfd))
    return false;
  if (SKW_GET_TELEPORTER_DETAIL(&vs_00, wordrg5, wordrg6) && vw_10 == ((unsignedword(vs_00.b_00) + con(0x2)) & con(0x3)))
  {
    if (!SKW_IS_CREATURE_ALLOWED_ON_LEVEL(vw_08, unsignedword(vs_00.b_04)))
      return false;
    vw_0c = ddata.v1d3248;
    SKW_CHANGE_CURRENT_MAP_TO(unsignedword(vs_00.b_04));
    bool boolrg4;
    if (SKW_IS_TILE_BLOCKED(SKW_GET_TILE_VALUE(unsignedword(vs_00.b_02), unsignedword(vs_00.b_03))))
      boolrg4 = true;
    else if (ecxpw == NULL)
      boolrg4 = false;
    else
    {
      if (SKW_GET_CREATURE_AT(unsignedword(vs_00.b_02) + table1d27fc[vs_00.b_01], unsignedword(vs_00.b_03) + table1d2804[vs_00.b_01]) == con(0xffffffff))
        boolrg4 = false;
      else
        boolrg4 = true;
    }
    SKW_CHANGE_CURRENT_MAP_TO(vw_0c);
    if (boolrg4)
      return false;
  }
  wordrg6 += table1d27fc[vw_10];
  wordrg5 += table1d2804[vw_10];
  x8 byterg7 = SKW_GET_TILE_VALUE(wordrg6, wordrg5);
  if (SKW_IS_TILE_BLOCKED(byterg7) || ((byterg7 >> con(0x5)) & con(0xff)) == con(0x3))
    return false;
  if (ecxpw != NULL)
    if (SKW_GET_CREATURE_AT(wordrg6, wordrg5) != con(0xffffffff))
      return false;
  if (SKW_GET_TELEPORTER_DETAIL(&vs_00, wordrg5, wordrg6))
    if (!SKW_IS_CREATURE_ALLOWED_ON_LEVEL(vw_08, unsignedword(vs_00.b_04)))
      return false;
  if (ecxpw != NULL)
    *ecxpw = vw_08;
  return true;
}

bool SKW_12b4_099e(x16 eaxw)
{
  c_hero* heroptrrg5;

  x16 wordrg6 = SKW_GET_CREATURE_WEIGHT(eaxw);
  if (wordrg6 > con(0xfd))
    return false;
  bool boolrg7 = false;
  for (x16 wordrg31 = con(0x0); wordrg31 < ddata.savegamew2; wordrg31++)
  {
    heroptrrg5 = &ddata.hero[wordrg31];
    if (heroptrrg5->w36 == con(0x0))
      continue;
    x16 wordrg15 = SKW_GET_PLAYER_ABILITY(heroptrrg5, con(0x0), con(0x1));
    if (wordrg31 == ddata.mouse_unk0e)
      wordrg15 += wordrg15 / 8;
    if (wordrg6 <= con(0x2d))
      wordrg15 += wordrg15 / 4;
    x16 wordrg16 = SKW_STAMINA_ADJUSTED_ATTR(heroptrrg5, wordrg15);
    if (wordrg16 < wordrg6)
      if (SKW_RAND16((wordrg6 - wordrg16) / 2 + con(0x1)) != con(0x0))
        continue;
    boolrg7 = true;
    break;
  }
  for (x16 wordrg3 = con(0x0); ; wordrg3++)
  {
    if (wordrg3 >= ddata.savegamew2)
      return boolrg7;
    if ((ddata.hero[wordrg3].w36 != con(0x0)) && ddata.hero[wordrg3].w3a > (ddata.hero[wordrg3].u3c.w >> con(0x4)))
    {
      x16 wordrg11;
      if (!boolrg7)
        wordrg11 = con(0x5);
      else
        wordrg11 = DM2_MAX(con(0x5), wordrg6 / 4);
      SKW_ADJUST_STAMINA(wordrg3, wordrg11);
    }
  }
}

