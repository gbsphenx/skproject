#include "fileio.h"
#include "util.h"
#include "startend.h" // THROW...
#include "dm2data.h"
#include "startend.h"
#include "c_alloc.h"
#include "c_dballoc.h"
#include "c_gfx_main.h"
#include "c_gfx_pixel.h"
#include "c_sound.h"
#include "c_gfx_str.h"
#include "c_image.h"
#include "c_querydb.h"
#include "c_gui_draw.h"
#include "c_gfx_decode.h"
#include "c_map.h"
#include "c_gdatfile.h"

const te_text gdatfn1[] = ".Z020GRAPHICS.DAT";
const te_text gdatfn2[] = ".Z026GRAPHIC2.DAT";
const te_text gdatfn3[] = ".Z020DUNGENB.DAT";
const te_text gdatfn4[] = ".Z020DUNGEON.DAT";
const te_text gdatfn5[] = ".Z022SKSAVE.Z023.DAT";
const te_text gdatfn6[] = ".Z022SKSAVE.Z023.BAK";
const te_text gdatfn7[] = ".Z020DUNGEON.FTL";

struct s_gdat // size: 0x20
{
  i32 l_00; // used
  s_hex6 s19_04;
  s_hex6 s19_0a;
  s_bbw* u31p_10;
  i8 b_14; // used
  i8 b_15;
  i8 b_16; // used
  i8 b_17; // used
  i16 w_18; // used
  i16 w_1a; // used
  i16 w_1c; // used
  i16 w_1e;
};

static bool DM2_QUERY_NEXT_GDAT_ENTRY(s_gdat* eaxp_s36)
{
  i8 byterg3hi;
  i16 wordrg6;
  s_bbw* u31p_04;
  i16 vw_08;
  i8 vb_0c;
  i8 vb_14;
  i8 vb_18;

  i8 vb_10 = eaxp_s36->s19_04.b_03;
  i8 byterg3lo = eaxp_s36->s19_04.b_05;
  i32 longrg4 = (unsignedlong(eaxp_s36->s19_04.w_00) & ulcon(0xffff7fff)) == ulcon(0x1) ? 1 : 0;
  bool flag = longrg4 == 0;
  i32 longrg5 = longrg4;
  if (flag)
    vb_0c = vb_10;
  else
  {
    vb_0c = eaxp_s36->s19_0a.b_03;
    vb_18 = eaxp_s36->s19_0a.b_05;
  }
  i32 vl_00 = eaxp_s36->l_00;
  if (vl_00 != 0)
  {
    eaxp_s36->l_00 = 0;
    eaxp_s36->b_14 = eaxp_s36->s19_04.b_02;
    if (eaxp_s36->s19_04.b_02 != lcon(0xffffffff))
    {
      if (longrg5 == 0)
        eaxp_s36->b_15 = eaxp_s36->s19_04.b_02;
      else
      {
        eaxp_s36->b_15 = eaxp_s36->s19_0a.b_02;
        if (eaxp_s36->s19_0a.b_02 == lcon(0xffffffff))
          eaxp_s36->b_15 = CUTX8(dm2_dballochandler.gdattable.entries);
      }
    }
    else
    {
      eaxp_s36->b_14 = 0;
      eaxp_s36->b_15 = CUTX8(dm2_dballochandler.gdattable.entries);
    }
    if (mkub(eaxp_s36->b_14) > mkub(eaxp_s36->b_15)) goto M_true;
    if (unsignedlong(eaxp_s36->b_15) > unsignedlong(dm2_dballochandler.gdattable.entries)) goto M_true;
  }

  i16 wordrg2;
  s_bbw* u31prg4b;

  for (;;) // M_2C7DC: // outest loop!
  {
    i16* wptrrg4a = dm2_dballochandler.gdattable.w_table1;
    i16* wptrrg2 = &wptrrg4a[unsignedlong(eaxp_s36->b_14)];
    i16 wordrg4 = wptrrg2[0];
    if (wordrg4 != wptrrg2[1])
    {
      bool innerloop = false;
      bool innerbypass = false;
      bool brkpsloop = false;
      while (!brkpsloop) // pseudoloop
      {
        if (innerloop || vl_00 != 0)
        {
          if (!innerloop)
          {
            i8 byterg2lo = CUTX8(wptrrg2[1]) - CUTLX8(wordrg4) - 1;
            i8 byterg2hi = eaxp_s36->s19_04.b_04;
            eaxp_s36->b_16 = byterg2hi;
            bool hopp = false;
            if (byterg2hi != lcon(0xffffffff))
            {
              if (mkub(byterg2hi) > mkub(byterg2lo))
                break; // goto M_2C9AD;
              if (longrg5 == 0)
              {
                eaxp_s36->b_17 = byterg2hi;
                hopp = true;
              }
              else
              {
                byterg2hi = eaxp_s36->s19_0a.b_04;
                eaxp_s36->b_17 = byterg2hi;
                if (byterg2hi != lcon(0xffffffff))
                  hopp = true;
              }
            }
            else
              eaxp_s36->b_16 = 0;
            if (!hopp)
              eaxp_s36->b_17 = byterg2lo;
            // M_2C84F:
            wordrg4 += unsignedword(eaxp_s36->b_16);
            wordrg2 = dm2_dballochandler.gdattable.w_table2[wordrg4];
            eaxp_s36->w_1a = wordrg2;
            eaxp_s36->w_18 = ++wordrg4;
            wordrg4 = dm2_dballochandler.gdattable.w_table2[wordrg4];
            eaxp_s36->w_1c = wordrg4;
            s_bbw* u31prg7 = &dm2_dballochandler.gdattable.u31p_08[unsignedlong(wordrg2)]; // unsigned has to stay here!
            wordrg4 = 4 * unsignedlong(wordrg2); // TEST
            eaxp_s36->u31p_10 = u31prg7;
            u31prg4b = u31prg7; // TODO: Note: ptrrg7 dies
          }

          // M_2C899:
          for (;;)
          {
            if (!innerloop)
            {
              if (mkuw(wordrg2) >= mkuw(eaxp_s36->w_1c))
                innerbypass = true;
              else if (vb_10 != bcon(0xff))
              {
                if (byterg3lo != lcon(0xffffffff))
                  vb_14 = byterg3lo;
                else
                  vb_14 = 0;
                u31prg4b = dm2_dballochandler.gdattable.u31p_08 - 1;
                u31p_04 = u31prg4b;
                wordrg6 = wordrg2;
                wordrg4 = eaxp_s36->w_1c + 1;
                vw_08 = wordrg4;

                // M_2C8D7:
                for (;;)
                {
                  wordrg2 = CUTX16((unsignedlong(wordrg6) + unsignedlong(vw_08)) / 2);
                  u31prg4b = &u31p_04[unsignedlong(wordrg2)];
                  if (wordrg2 != wordrg6)
                  {
                    byterg3hi = vb_10;
                    if (byterg3hi != u31prg4b->b_00)
                    {
                      if (mkub(byterg3hi) >= mkub(u31prg4b->b_00))
                        wordrg6 = wordrg2;
                      else
                        vw_08 = wordrg2;
                      continue;
                    }
                    byterg3hi = vb_14;
                    if (mkub(byterg3hi) < mkub(u31prg4b->b_01))
                    {
                      vw_08 = wordrg2;
                      continue;
                    }
                  }
                  else
                  {
                    if (mkuw(wordrg2) >= mkuw(eaxp_s36->w_1c))
                      innerbypass = true; // goto M_2C979;
                    else
                    {
                      wordrg2++;
                      u31prg4b++;
                      byterg3hi = vb_0c;
                      if (mkub(byterg3hi) < mkub(u31prg4b->b_00))
                        innerbypass = true; // goto M_2C979;
                    }
                  }
                  break;
                }
                wordrg2--;
              }
            }

            // M_2C934:
            innerloop = false;
            if (!innerbypass)
            {
              for (;;)
              {
                byterg3hi = u31prg4b->b_00;
                if (mkub(byterg3hi) > mkub(vb_0c))
                  break; // goto M_2C979;
                if (longrg5 == 0)
                {
                  if (byterg3lo == bcon(0xffffffff) || byterg3lo == u31prg4b->b_01) goto M_false;
                }
                else
                {
                  if (byterg3lo == bcon(0xffffffff)) goto M_false;
                  if (mkub(byterg3lo) <= mkub(u31prg4b->b_01))
                  {
                    byterg3hi = vb_18;
                    if (mkub(byterg3hi) >= mkub(u31prg4b->b_01)) goto M_false;
                  }
                }
                // M_2C96D:
                if (mkuw(++wordrg2) >= mkuw(eaxp_s36->w_1c))
                  break; // goto M_2C979;
                u31prg4b++;
              }
            }

            // M_2C979:
            innerloop = false;
            innerbypass = false;
            i8 byterg2 = eaxp_s36->b_16 + 1;
            eaxp_s36->b_16 = byterg2;
            if (mkub(byterg2) > mkub(eaxp_s36->b_17))
            {
              brkpsloop = true;
              break; // goto M_2C9AD;
            }
            wordrg2 = eaxp_s36->w_1c;
            wordrg6 = eaxp_s36->w_18 + 1;
            eaxp_s36->w_18 = wordrg6;
            i32 tmp7 = 2 * unsignedlong(wordrg6); // TODO: check obsolete
            wordrg6 = dm2_dballochandler.gdattable.w_table2[unsignedlong(wordrg6)];
            eaxp_s36->w_1c = wordrg6;
          } // loop M_2C899;
        } // endif
        if (!brkpsloop)
        {
          wordrg2 = eaxp_s36->w_1a + 1;
          u31prg4b = eaxp_s36->u31p_10 + 1;
          innerloop = true;
          if (mkuw(wordrg2) >= mkuw(eaxp_s36->w_1c))
            innerbypass = true;
        }
      } // pseudoloop
    } // endif

    // M_2C9AD:
    i8 byterg4 = eaxp_s36->b_14 + 1;
    eaxp_s36->b_14 = byterg4;
    if (mkub(byterg4) > mkub(eaxp_s36->b_15)) goto M_true;
    vl_00 = 1;
  } // goto M_2C7DC;

M_false:
  eaxp_s36->w_1a = wordrg2;
  eaxp_s36->u31p_10 = u31prg4b;
  return true;

M_true:
  eaxp_s36->u31p_10 = NULL;
  eaxp_s36->b_14 = lcon(0xffffffff);
  eaxp_s36->b_16 = lcon(0xffffffff);
  return false;
}

void c_dballochandler::DM2_dballoc_3e74_24b8(void)
{
  i16 wordrg3; // TODO: Danger - uninitialized possible?
  i16* wptrrg5; // TODO: Danger - uninitialized possible?
  s_gdat s36_00;

  i16 wordrg6 = 0;
  while (unsignedlong(wordrg6) < ulcon(0x2))
  {
    if (wordrg6 != 0)
    {
      if (v1e0ad6 == 0)
        return;
    }
    s36_00.l_00 = 1;
    s36_00.s19_04.w_00 = 0;
    s36_00.s19_04.b_02 = lcon(0xffffffff);
    s36_00.s19_04.b_03 = lcon(0xffffffff);
    s36_00.s19_04.b_04 = 2;
    s36_00.s19_04.b_05 = lcon(0xffffffff);
    if (wordrg6 != 0)
    {
      wptrrg5 = XUPCAST(i16, DM2_ALLOC_LOBIGPOOL_MEMORY(2 * unsignedlong(v1e0ad6)));
      wordrg3 = 0;
    }

    while (DM2_QUERY_NEXT_GDAT_ENTRY(&s36_00))
    {
      i16 wordrg4 = s36_00.u31p_10->w_02;
      if (wordrg6 != 0)
      {
        bool immcont = false;
        i16 wordrg1 = 0;
        while (mkuw(wordrg1) < mkuw(wordrg3))
        {
          if (wordrg4 == wptrrg5[wordrg1])
          {
            immcont = true;
            break;
          }
          wordrg1++;
        }
        if (!immcont)
        {
          wptrrg5[unsignedlong(wordrg3++)] = wordrg4;
          dm2sound.v1e0ace++;
        }
      }
      else
      {
        v1e0ad6++;
        i16 wordrg1 = CUTX16(dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH((t_dbidx)wordrg4));
        if (mkuw(wordrg1) > v1e0ad8)
          v1e0ad8 = wordrg1;
      }
    }

    if (wordrg6 != 0)
      DM2_DEALLOC_LOBIGPOOL(2 * v1e0ad6);
    wordrg6++;
  }
}

bool c_dballochandler::DM2_dballoc_3e74_2162(i16 eaxw)
{
  DM2_QUERY_GDAT_ENTRY_VALUE(eaxw, 2);
  i8 byterg1 = CUTLX8(DM2_QUERY_GDAT_ENTRY_VALUE(eaxw, lcon(0x5))) & lcon(0xfffffff0);
  if (byterg1 == 0 || byterg1 == v1d6770)
    return true;
  return false;
}

// ###################################################################

c_gdatfile gdat;

void c_gdatfile::init(void)
{
  fileopencounter = 0;
  filehandle = 0;
  xfilehandle = 0;
  filesize = 0;
  versionlo = 0;
  entries = 0;
  filetype1 = false;
  filetype2 = false;
  filename1 = CHGCAST(t_text, gdatfn1);
  filename2 = CHGCAST(t_text, gdatfn2);
  filename3 = CHGCAST(t_text, gdatfn3);
  filename4 = CHGCAST(t_text, gdatfn4);
  filename5 = CHGCAST(t_text, gdatfn5);
  filename6 = CHGCAST(t_text, gdatfn6);
  filename7 = CHGCAST(t_text, gdatfn7);
}

void DM2_GRAPHICS_DATA_OPEN(void)
{
  if (++gdat.fileopencounter == 1)
  {
    gdat.filehandle = DM2_FILE_OPEN(DM2_FORMAT_SKSTR(gdat.filename1, NULL));
    if (gdat.filehandle < 0)
      DM2_RAISE_SYSERR(0x29);
    if (!gdat.filetype1 && gdat.filetype2)
    {
      gdat.xfilehandle = DM2_FILE_OPEN(DM2_FORMAT_SKSTR(gdat.filename2, NULL));
      if (gdat.xfilehandle < 0)
        DM2_RAISE_SYSERR(0x1f);
    }
  }
}

void DM2_GRAPHICS_DATA_CLOSE(void)
{
  i16 wordrg4 = gdat.fileopencounter;
  bool flag = --wordrg4 != 0;
  gdat.fileopencounter = wordrg4;
  if (!flag)
  {
    DM2_FILE_CLOSE(gdat.filehandle);
    if (!gdat.filetype1 && gdat.filetype2)
      DM2_FILE_CLOSE(gdat.xfilehandle);
  }
}

// TODO: make sure that both local variables don't overlap
// while adressed by index
static void DM2_GRAPHICS_DATA_READ(i32 eaxl, i32 edxl, unk* xebxp)
{
  i32 vla_00[0x4];
  i16 vwa_10[0x2];

  i32 longrg5 = eaxl;
  vwa_10[0x0] = gdat.filehandle;
  vla_00[0x0] = longrg5;
  vla_00[0x2] = edxl;
  vla_00[0x3] = 0;
  if (gdat.filetype2)
  {
    vwa_10[0x1] = gdat.xfilehandle;
    longrg5 -= gdat.filesize;
    vla_00[0x1] = longrg5;
    if (longrg5 < 0)
    {
      vla_00[0x3] = longrg5 + edxl;
      if (vla_00[0x3] <= 0)
        vla_00[0x3] = 0;
      else
        vla_00[0x1] = 0;
    }
    else
      vla_00[0x3] = edxl;
    vla_00[0x2] -= vla_00[0x3];
  }

  i16 wordrg3 = 0;
  while ((longrg5 = signedlong(wordrg3)) < 2)
  {
    i32 longrg2 = 4 * longrg5;
    if (vla_00[0x2 + longrg2 / 4] != 0)
    {
      longrg5 = 2 * longrg5;
      bool noclose = false;
      if (DM2_FILE_SEEK(signedlong(vwa_10[longrg5 / 2]), vla_00[longrg2 / 4]))
      {
        if (DM2_FILE_READ(vwa_10[longrg5 / 2], xebxp, vla_00[0x2 + longrg2 / 4]))
          noclose = true;
      }
      if (!noclose)
      {
        DM2_GRAPHICS_DATA_CLOSE();
        if (wordrg3 != 0)
          DM2_RAISE_SYSERR(0x20);
        else
          DM2_RAISE_SYSERR(0x2e);
      }
      xebxp += vla_00[0x2 + signedlong(wordrg3)];
    }
    wordrg3++;
  }
}

static i32 DM2_QUERY_GDAT_RAW_DATA_FILE_POS(t_dbidx dbidx)
{
  i32 longrg2 = ddat.vlong1e0a3c;
  t_dbidx didx4 = 0;
  if (dbidx >= dm2_dballochandler.dbidx)
  {
    didx4 = dm2_dballochandler.dbidx;
    longrg2 += dm2_dballochandler.v1d6783;
  }
  while (didx4 < dbidx)
    longrg2 += dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH(didx4++) & lcon(0xffff);
  dm2_dballochandler.dbidx = dbidx;
  dm2_dballochandler.v1d6783 = longrg2 - ddat.vlong1e0a3c;
  return longrg2;
}

void DM2_LOAD_GDAT_RAW_DATA(t_dbidx dbidx, unk* xedxp)
{
  i32 longrg5;
  i32 longrg6;
  i32 vl_00;
  i16 vw_04;

  DM2_GRAPHICS_DATA_OPEN();
  if (dbidx != 0) // TODO: why 0 and not -1, 0 must be a special index
  {
    vl_00 = DM2_QUERY_GDAT_RAW_DATA_FILE_POS(dbidx);
    longrg6 = unsignedlong(CUTX16(dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH(dbidx)));
  }
  else
  {
    longrg6 = ddat.vlong1e0a44;
    vl_00 = ddat.vlong1e0a3c - longrg6;
  }
  vw_04 = CUTX16((vl_00 - jul_shl32(vl_00, lcon(0xa))) >> bcon(0xa));
  longrg5 = signedlong(vw_04) << bcon(0xa);
  while (longrg6 > 0)
  {
    if (vw_04 != dm2_dballochandler.v1d6773 || dm2_dballochandler.v1e0abe != wcon(0x0))
    {
      dm2_dballochandler.v1d6773 = vw_04;
      i32 longrg42 = ddat.vlong1e0a40 - longrg5;
      if (longrg42 > lcon(0x400))
        longrg42 = lcon(0x400);
      DM2_GRAPHICS_DATA_READ(longrg5, longrg42, dm2_dballochandler.filebuffer);
      dm2_dballochandler.v1e0abe = 0;
    }
    i32 longrg4 = vl_00 - longrg5;
    i32 longrg3 = lcon(0x400) - longrg4;
    if (longrg6 < longrg3)
      longrg3 = longrg6;
    DM2_COPY_MEMORY(xedxp, dm2_dballochandler.filebuffer + longrg4, longrg3);
    longrg6 -= longrg3;
    vl_00 += longrg3;
    xedxp += longrg3;
    vw_04++;
    longrg5 += lcon(0x400);
  }
  DM2_GRAPHICS_DATA_CLOSE();
}

static i32 DM2_QUERY_GDAT_ENTRY_VALUE(i16 eaxw, i16 edxw)
{
  unk* xptrrg1 = dm2_dballochandler.ptr1e09a8 + unsignedlong(eaxw) * unsignedlong(dm2_dballochandler.v1e09ae) + signedlong(dm2_dballochandler.v1e09b2[edxw]);
  i16 wordrg4 = unsignedword(dm2_dballochandler.v1e09c0[edxw]);
  i32 longrg2 = 0;
  while (wordrg4-- > 0)
  {
    longrg2 <<= bcon(0x8);
    longrg2 += unsignedlong(*xptrrg1++);
  }
  return longrg2;
}

// ebxw is either BIGPOOL_LO or FREEPOOL
t_bmp* DM2_ALLOC_PICT_BUFF(i16 eaxw, i16 edxw, i16 ebxw, i8 ecxb)
{
  i32 vl_00;

  if (ecxb != lcon(0x4))
    vl_00 = unsignedlong(eaxw);
  else
    vl_00 = (unsignedlong(eaxw) + 1 & lcon(0xfffffffe)) >> 1;

  i32 vl_04 = (vl_00 & lcon(0xffff)) * unsignedlong(edxw);
  t_bmp* bmp = XUPCAST(t_bmp, dm2_dballochandler.DM2_ALLOC_MEMORY_RAM((vl_04 & lcon(0xffff)) + 6, MASK_8, ebxw) + lcon(0x6));
  s_dm2bmpheader* s82p = getbmpheader(bmp);
  s82p->res = ecxb;
  s82p->width = eaxw;
  s82p->height = edxw;
  return bmp;
}

void DM2_FREE_PICT_BUFF(t_bmp* bmp)
{
  s_dm2bmpheader* s82p = getbmpheader(bmp);
  i16 wordrg4;
  if (s82p->res != BPP_4)
    wordrg4 = s82p->width;
  else
    wordrg4 = ((s82p->width + 1) & wcon(0xfffe)) >> 1;
  dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(unsignedlong(CUTX16(unsignedlong(s82p->height) * unsignedlong(wordrg4))) + lcon(0x6));
}

// was DM2_ALLOC_NEW_PICT, TODO tell SPX new name
t_bmp* DM2_ALLOC_NEW_BMP(t_dbidx dbidx, i16 width, i16 height, t_resolution res)
{
  i16 w;
  if (res != BPP_4)
    w = width;
  else
    w = MK_EVEN(width) >> 1;
  t_bmp* bmp = CHGCAST(t_bmp, dm2_dballochandler.DM2_ALLOC_CPXHEAP_MEM(dbidx, (height * w) & ulcon(0xffff)));
  s_dm2bmpheader* bmph = getbmpheader(bmp);
  bmph->res = res;
  bmph->width = width;
  bmph->height = height;
  return bmp;
}

static i16 R_2BAD4(i16 eaxw)
{
  return CUTX16((unsignedlong(eaxw) >> bcon(0x8)) + (unsignedlong(eaxw) << bcon(0x8)));
}

static i16* R_2D8AD(s_pppw* eaxp, i32 edxl)
{
  i16* s19prg2 = UPCAST(i16, DOWNCAST(i16, eaxp->wp_08) - edxl);
  eaxp->wp_08 = s19prg2;
  return s19prg2;
}

static i16* R_2D8BA(s_pppw* eaxp, unk* xedxp, i32 ebxl)
{
  i16* wptr = R_2D8AD(eaxp, ebxl);
  DM2_COPY_MEMORY(DOWNCAST(i16, wptr), xedxp - 2, ebxl); // TODO: neg. offset
  return wptr + 1;
}

static void R_2D802(s_pppw* eaxp_s46)
{
  s_pppw* s46prg7 = eaxp_s46;
  if (eaxp_s46->wp_00 == NULL)
    return;
  i16* wptrrg3 = eaxp_s46->wp_00;
  i16* wptrrg5 = eaxp_s46->wp_00 - 1;
  while (wptrrg5 > s46prg7->wp_08)
  {
    i16 wordrg6 = *wptrrg5;
    i32 longrg2 = (((dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH((t_dbidx)(wordrg6 & wcon(0x7fff))) & lcon(0xffff)) + 1 & lcon(0xfffffffe)) + lcon(0x4)) / 2;
    wptrrg5 -= longrg2;
    if ((wordrg6 & wcon(0x8000)) != wcon(0x0))
      continue;
    wptrrg3 -= longrg2;
    i16* wptrrg1 = wptrrg5 + 1;
    if (wptrrg1 == wptrrg3)
      continue;
    DM2_COPY_MEMORY(DOWNCAST(i16, wptrrg3), DOWNCAST(i16, wptrrg1), 2 * longrg2);
    dm2_ulp.setp(wordrg6, DOWNCAST(i16, wptrrg3 + 1));
  }
  s46prg7->wp_08 = wptrrg3;
}

void DM2_FREE_PICT_ENTRY(c_pixel* gfx)
{
  if (dm2_dballochandler.gfxalloc_done || gfx == NULL)
    return;

  struct s_tmp // size 0xe
  {
    i32 l_00; // unused
    i16 w_04;
    i16 w_06;
    s_dm2bmpheader bmp; // @08
  };

  s_tmp* s101p = XUPCAST(s_tmp, gfx - 14);
  // pointer in the beginning,
  // word at 4, word at 6, byte at 8, word at a, word at c

  if (   s101p->bmp.width == (s101p->w_04 & wcon(0x3ff))
      && s101p->bmp.height == (s101p->w_06 & wcon(0x3ff))
     )
  {
    s_malloctail* s78prg11 = CHGCAST(s_malloctail, s101p + lcon(0x4)); // that is eaxp - 0xa

    if (s78prg11->l_00 >> bcon(0x1a) == lcon(0xffffffe0))
    {
      dm2_dballochandler.DM2_DEALLOC_BIGPOOL_STRUCT_BEFORE(s78prg11);
      return;
    }
  }

  unk** xpptrrg1 = &dm2_dballochandler.ptr1e0a5c;
  unk** xpptrrg2 = CHGCAST(unk*, s101p);
  unk** xpptrrg3;
  while (xpptrrg2 != (xpptrrg3 = UPCAST(unk*, *xpptrrg1)))
    xpptrrg1 = UPCAST(unk*, *xpptrrg1);
  *xpptrrg1 = *xpptrrg3;

  i32 longrg1 = mkl(DM2_CALC_IMAGE_BYTE_LENGTH(BMPCAST(gfx)) + 14);
  if (s101p->bmp.res == BPP_4)
    longrg1 += 16;
  if (s101p->w_06 != 0)
    dm2_dballochandler.DM2_DEALLOC_HIBIGPOOL(longrg1);
  else
    dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(longrg1);
}

// 2nd argument was a functionpointer, but is dm2_dballochandler.DM2_dballoc_3e74_2162 only
static void DM2_BUILD_GDAT_ENTRY_DATA(s_1e09e0* eaxp_s81, unk* xebxp)
{
  i32 longrg6;
  i32 vl_04;
  c_ql32 vql_0c;
  c_ql32 vql_10;
  i16* wp_14;
  c_ol32 vol_18; // poke 32 peek 16 32 inc32
  i8 vb_1c; // inc8

  wp_14 = XUPCAST(i16, dm2_dballochandler.DM2_ALLOC_HIBIGPOOL_MEMORY(lcon(0x3a0), true));
  eaxp_s81->entries = 0;
  eaxp_s81->w_10 = 0;

  i16* wptrrg7 = wp_14; // TODO: ptrrg7 read only from here
  for (ui16 uwordrg21 = 0; uwordrg21 < mkuw(dm2_dballochandler.v1e09ac); uwordrg21++)
  {
    i32 longrg52 = unsignedlong(uwordrg21);
    if (dm2_dballochandler.DM2_dballoc_3e74_2162(CUTX16(longrg52)))
    {
      vql_0c.poke32(DM2_QUERY_GDAT_ENTRY_VALUE(CUTX16(longrg52), 0));
      vb_1c = vql_0c.peek8();
      i8 byterg1 = CUTLX8(DM2_QUERY_GDAT_ENTRY_VALUE(CUTX16(longrg52), 2));
      i8 byterg4lo = byterg1;
      i8 byterg4hi = vql_0c.peek8();
      if (mkub(byterg4hi) <= mkub(lcon(0x1c)) && mkub(byterg1) <= mkub(lcon(0xe)))
      {
        eaxp_s81->w_10++;
        if (unsignedlong(byterg4hi) > unsignedlong(eaxp_s81->entries))
          eaxp_s81->entries = unsignedword(byterg4hi);
        longrg6 = unsignedlong(vb_1c);
        wptrrg7[16 * mkul(longrg6) + unsignedlong(byterg4lo)]++;
        longrg6 <<= bcon(0x5);
        i16* wptrrg5 = &wptrrg7[longrg6 / 2];
        longrg6 = unsignedlong(wptrrg5[lcon(0xf)]);
        if (unsignedlong(byterg4lo) >= mkul(longrg6))
          wptrrg5[lcon(0xf)] = unsignedword(byterg4lo) + 1;
      }
    }
  }

  i16 wordrg13 = 0;
  ui32 ulongrg4;
  for (vb_1c = 0; (ulongrg4 = unsignedlong(vb_1c)) <= unsignedlong(eaxp_s81->entries); vb_1c++)
  {
    ulongrg4 <<= bcon(0x5);
    wordrg13 += wp_14[(ulongrg4 + lcon(0x1e)) / 2];
  }
  eaxp_s81->w_0e = wordrg13;
  eaxp_s81->w_12 = 0;
  eaxp_s81->w_14 = 0;
  longrg6 = 0;
  vol_18.poke32(0);

  ui16 uwordrg5;
  while ((uwordrg5 = mkuw(vol_18.peek16())) < mkuw(lcon(0x7)))
  {
    if (uwordrg5 > uwcon(0x4))
    {
      i32 longrg53 = unsignedlong(vol_18.peek16());
      if (xebxp[longrg53] != 0)
      {
        eaxp_s81->warr_16[longrg53] = eaxp_s81->w_12;
        eaxp_s81->barr_24[longrg53] = dm2_dballochandler.v1e09c0[longrg53];
        eaxp_s81->w_12 += unsignedword(dm2_dballochandler.v1e09c0[longrg53]);
        eaxp_s81->w_14++;
      }
    }
    else
    {
      i32 longrg52 = unsignedlong(vol_18.peek16());
      eaxp_s81->barr_24[longrg52] = dm2_dballochandler.v1d6787[longrg52];
      eaxp_s81->w_12 += unsignedword(dm2_dballochandler.v1d6787[longrg52]);
      eaxp_s81->warr_16[longrg52] = lcon(0xffffffff);
      if (mkub(dm2_dballochandler.v1d6787[longrg52]) > mkub(0))
        eaxp_s81->w_14++;
    }
    vol_18.inc32();
  }
  dm2_dballochandler.gdattable.w_table1 = XUPCAST(i16, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(sizeof(i16) * unsignedlong(eaxp_s81->entries) + 4));
  dm2_dballochandler.gdattable.w_table2 = XUPCAST(i16, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(sizeof(i16) * unsignedlong(eaxp_s81->w_0e) + 2));
  dm2_dballochandler.gdattable.u31p_08 = XUPCAST(s_bbw, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(sizeof(s_bbw) * unsignedlong(eaxp_s81->w_10)));

  i16 wordrg1 = 0;
  i16 wordrg2 = 0;

  i32 longrg5;
  i32 longrg41;
  for (vb_1c = bcon(0x0); (longrg41 = mkl(unsignedlong(vb_1c))) <= (longrg5 = unsignedlong(eaxp_s81->entries)); vb_1c++)
  {
    eaxp_s81->w_table1[longrg41] = wordrg1;
    longrg41 <<= bcon(0x5);
    vol_18.poke16(wp_14[(longrg41 + lcon(0x1e)) / 2]);

    for (i8 byterg4 = bcon(0x0); mkul(longrg5 = mkl(unsignedlong(byterg4))) < mkl(unsignedlong(vol_18.peek16())); byterg4++)
    {
      longrg6 = 2 * unsignedlong(wordrg1++);
      vl_04 = longrg6;
      eaxp_s81->w_table2[vl_04 / 2] = wordrg2; // TODO: avoid / 2
      longrg6 = unsignedlong(vb_1c) << 4;
      wordrg2 += wp_14[longrg5 + longrg6];
    }
  }
  eaxp_s81->w_table1[longrg5 + 1] = wordrg1;
  eaxp_s81->w_table2[unsignedlong(eaxp_s81->w_0e)] = eaxp_s81->w_10;
  DM2_ZERO_MEMORY(DOWNCAST(i16, wp_14), lcon(0x3a0));
  DM2_ZERO_MEMORY(DOWNCAST(s_bbw, eaxp_s81->u31p_08), unsignedlong(eaxp_s81->w_10) << 2);

  for (ui16 uwordrg2 = 0; uwordrg2 < mkuw(dm2_dballochandler.v1e09ac); uwordrg2++)
  {
    if (dm2_dballochandler.DM2_dballoc_3e74_2162(uwordrg2))
    {
      vql_10.poke32(DM2_QUERY_GDAT_ENTRY_VALUE(uwordrg2, 0));
      i8 byterg1 = CUTLX8(DM2_QUERY_GDAT_ENTRY_VALUE(uwordrg2, 2));
      if (mkub(vql_10.peek8()) <= mkub(lcon(0x1c)) && mkub(byterg1) <= mkub(lcon(0xe)))
      {
        i32 longrg4 = unsignedlong(vql_10.peek8());
        longrg6 = 16 * longrg4;
        i32 longrg7 = unsignedlong(byterg1);
        longrg6 += longrg7;
        i16* wptrrg6 = &wp_14[longrg6];
        i16 wordrg1 = *wptrrg6;
        vl_04 = 2 * longrg4;
        *wptrrg6 = wordrg1 + 1;
        i16* wp1 = eaxp_s81->w_table1;
        i16* wp2 = eaxp_s81->w_table2;
        s_bbw* u31prg3 = eaxp_s81->u31p_08;
        i16 vw1 = wp1[longrg4];
        ui32 vul2 = unsignedlong(vw1) + longrg7;
        i16 vw3 = wp2[vul2];
        ui32 vul4 = unsignedlong(vw3) + unsignedlong(wordrg1);
        s_bbw* u31prg6 = &u31prg3[vul4];
        u31prg6->b_00 = CUTLX8(DM2_QUERY_GDAT_ENTRY_VALUE(uwordrg2, 1));
        u31prg6->b_01 = CUTLX8(DM2_QUERY_GDAT_ENTRY_VALUE(uwordrg2, 3));
        u31prg6->w_02 = CUTX16(DM2_QUERY_GDAT_ENTRY_VALUE(uwordrg2, 4));
        if (unsignedword(CUTLX8(DM2_QUERY_GDAT_ENTRY_VALUE(uwordrg2, 6))) == 1)
          u31prg6->w_02 |= wcon(0x8000);
      }
    }
  }
  dm2_dballochandler.DM2_DEALLOC_HIBIGPOOL(lcon(0x3a0));
}

// stacksize was 0x4
void DM2_LOAD_GDAT_ENTRY_DATA_TO(i8 eaxb, i8 edxb, i8 ebxb, i8 ecxb, unk* xargp0)
{
  DM2_LOAD_GDAT_RAW_DATA((t_dbidx)DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, edxb, ebxb, ecxb), xargp0);
}

static void DM2_LOAD_GDAT_ENTRIES(void)
{
  ui16 uwordrg5 = 0;
  while (uwordrg5 < mkuw(dm2_dballochandler.v1e09ac))
  {
    i32 longrg2 = unsignedlong(uwordrg5);
    if (CUTLX8(DM2_QUERY_GDAT_ENTRY_VALUE(CUTX16(longrg2), lcon(0x6))) == lcon(0xffffffff))
    {
      i8 byterg1 = CUTLX8(DM2_QUERY_GDAT_ENTRY_VALUE(CUTX16(longrg2), 2));
      if (byterg1 != bcon(0xb) && byterg1 != bcon(0xc))
      {
        i32 longrg3 = unsignedlong(CUTX16(DM2_QUERY_GDAT_ENTRY_VALUE(CUTX16(longrg2), lcon(0x4))));
        i32 longrg7 = 4 * longrg3; // TODO: check obsolete?
        if (dm2_ulp.islen(longrg3))
        {
          i16 wordrg6 = CUTX16(dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH(/*TAG dbidx*/ CUTX16(longrg3)));
          i16* wptrrg1 = XUPCAST(i16, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(unsignedlong(wordrg6) + 2));
          *wptrrg1++ = wordrg6;
          dm2_ulp.setp(longrg3, DOWNCAST(i16, wptrrg1));
          DM2_LOAD_GDAT_RAW_DATA(/*TAG dbidx*/ CUTX16(longrg3), DOWNCAST(i16, wptrrg1));
        }
      }
    }
    uwordrg5++;
  }
}

static void DM2_47eb_00a4(s_pwwp* xeaxp)
{
  s_pwwp* s58p = xeaxp;

  s58p->s58p_08 = dm2_dballochandler.v1dff38;
  dm2_dballochandler.v1dff38 = xeaxp;

  struct s_bbbbw
  {
    i8 b_00;
    i8 b_01;
    i8 b_02;
    i8 b_03;
    i16 w_04;
  };

  s_bbbbw* s99prg4 = XUPCAST(s_bbbbw, s58p->xp_00 - 6);

  if (s99prg4->w_04 == 1)
    return;
  if (s99prg4->w_04 != 0)
    s99prg4->w_04 = 1;
  else
  {
    if ((s99prg4->b_03 & bcon(0x1)) == bcon(0x0))
      return;
    s99prg4->b_03 &= lcon(0xfffffffe);
  }
  i16 wordrg4 = s58p->w_04;
  unk* xp = s58p->xp_00;
  while (--wordrg4 != wcon(0xffff))
    *xp++ ^= lcon(0xffffff80);
}

static void DM2_LOAD_ENT1(void)
{
  i32 longrg7 = ddat.vlong1e0a44;
  unk* xptrrg1 = dm2_dballochandler.DM2_ALLOC_HIBIGPOOL_MEMORY(longrg7);
  dm2_dballochandler.ptr1e09a8 = xptrrg1;
  DM2_LOAD_GDAT_RAW_DATA(0, xptrrg1);
  i16 wordrg5 = *XUPCAST(i16, dm2_dballochandler.ptr1e09a8);
  if (wordrg5 != lcon(0xffff8001))
    if (R_2BAD4(wordrg5) != lcon(0xffff8001))
      DM2_RAISE_SYSERR(0x1e);
  i16* wptrrg11 = XUPCAST(i16, dm2_dballochandler.ptr1e09a8);
  i16 wordrg4 = wptrrg11[1];
  dm2_dballochandler.v1e09ac = wordrg4;
  dm2_dballochandler.v1e09b0 = wptrrg11[2];
  if (wordrg5 != lcon(0xffff8001))
  {
    dm2_dballochandler.v1e09ac = R_2BAD4(wordrg4);
    dm2_dballochandler.v1e09b0 = R_2BAD4(dm2_dballochandler.v1e09b0);
  }
  unk* xptrrg6 = dm2_dballochandler.ptr1e09a8 + lcon(0x6);
  DM2_FILL_I16TABLE(dm2_dballochandler.v1e09b2, lcon(0xffffffff), lcon(0x7));
  dm2_dballochandler.v1e09ae = 0;

  i16 wordrg2;
  for (ui16 uwordrg5 = uwcon(0x0); uwordrg5 < mkuw(wordrg2 = dm2_dballochandler.v1e09b0); uwordrg5++)
  {
    wordrg2 = 0;
    unk* xptrrg4;
    i32 longrg1;
    while ((longrg1 = signedlong(wordrg2)) < lcon(0x7))
    {
      xptrrg4 = xptrrg6 + 2 * unsignedlong(uwordrg5);
      if (dm2_dballochandler.tidspfg[longrg1] == xptrrg4[0])
        break;
      wordrg2++;
    }
    dm2_dballochandler.v1e09c0[longrg1] = xptrrg4[1];
    dm2_dballochandler.v1e09b2[longrg1] = dm2_dballochandler.v1e09ae;
    dm2_dballochandler.v1e09ae += unsignedword(xptrrg6[2 * unsignedlong(uwordrg5) + 1]);
  }
  xptrrg6 += 2 * unsignedlong(wordrg2);
  dm2_dballochandler.ptr1e09a8 = xptrrg6;
  DM2_LOAD_GDAT_ENTRIES();
  DM2_BUILD_GDAT_ENTRY_DATA(&dm2_dballochandler.gdattable, &dm2_dballochandler.v1d6787[5]);
  dm2_dballochandler.DM2_DEALLOC_HIBIGPOOL(longrg7);
  dm2_dballochandler.ptr1e09a8 = NULL;
}

static void DM2_482b_0684(void)
{
  i16 vw_04;

  for (ui32 vul_00 = 0; vul_00 < unsignedlong(dm2sound.v1d2698); vul_00++)
  {
    s_ssound* s93prg5 = CHGCAST(s_ssound, &dm2sound.xsndptr2[vul_00]);
    if (s93prg5->w_05 == lcon(0xffffffff))
    {
      i16 wordrg4 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(s93prg5->b_02, s93prg5->b_03, 2, s93prg5->b_04);
      i16 wordrg1 = DM2_SOUND7(wordrg4);
      if (wordrg1 == 0)
      {
        if (dm2sound.v1d269a >= dm2sound.v1e0ace)
          return;
        s93prg5->w_05 = wordrg4;
        s93prg5->w_00 = dm2sound.v1d269a;
        if (dm2sound.v1d269c == 0)
          vw_04 = 2;
        else
          vw_04 = 6;
        s_pwwp* s92p = XUPCAST(s_pwwp, dm2sound.sndptr4 + 16 * unsignedlong(s93prg5->w_00));
        i16* wptrrg6 = XUPCAST(i16, DM2_QUERY_GDAT_ENTRY_DATA_PTR(s93prg5->b_02, s93prg5->b_03, 2, s93prg5->b_04));
        s92p->xp_00 = DOWNCAST(i16, wptrrg6 + vw_04 / 2);
        wordrg4 = CUTX16(DM2_QUERY_GDAT_ENTRY_DATA_LENGTH(s93prg5->b_02, s93prg5->b_03, 2, s93prg5->b_04) - unsignedlong(vw_04));
        s92p->w_04 = wordrg4;

        i16 wordrg11;
        if (dm2sound.v1d269c == 0)
          wordrg11 = lcon(0x157c);
        else
          wordrg11 = *wptrrg6;
        s92p->w_06 = wordrg11;
        DM2_47eb_00a4(s92p);
        dm2sound.v1d269a++;
      }
      else
      {
        s93prg5->w_00 = CHGCAST(s_ssound, &dm2sound.xsndptr2[unsignedlong(wordrg1) - 1])->w_00;
        s93prg5->w_05 = wordrg4;
      }
    }
  }
}

static i32 R_2D07D(void)
{
  s_gdat s36_00;

  s36_00.l_00 = 1;
  s36_00.s19_04.w_00 = 0;
  s36_00.s19_04.b_02 = lcon(0xffffffff);
  s36_00.s19_04.b_03 = lcon(0xffffffff);
  s36_00.s19_04.b_04 = 3;
  s36_00.s19_04.b_05 = lcon(0xffffffff);
  for (;;)
  {
    if (!DM2_QUERY_NEXT_GDAT_ENTRY(&s36_00))
      return 0;
    i16 wordrg1 = CUTX16(dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH((t_dbidx)s36_00.u31p_10->w_02));
    if (mkuw(wordrg1) > mkuw(dm2_dballochandler.v1e0ad4))
      dm2_dballochandler.v1e0ad4 = wordrg1;
  }
}

static i16 DM2_TRACK_UNDERLAY(ui16 eaxuw)
{
  struct s_uww
  {
    ui16 uw_00;
    i16 w_02;
  };

  i16 wordrg2 = 0;
  i16 wordrg6 = ddat.v1e0ac2 + 1;
  for (;;)
  {
    i16 wordrg1 = (wordrg6 + wordrg2) / 2; // TODO: check: was it >>1 ?
    if (wordrg1 == wordrg2)
      return lcon(0xffff);
    unk* xptrrg4 = ddat.p1e0a64 + 4 * signedlong(wordrg1);
    s_uww* s98p = XUPCAST(s_uww, xptrrg4 - 4); // TODO: neg. offset
    if (eaxuw >= s98p->uw_00)
    {
      if (eaxuw > s98p->uw_00)
        wordrg2 = wordrg1;
      else
        return s98p->w_02;
    }
    else
      wordrg6 = wordrg1;
  }
}

void DM2_READ_GRAPHICS_STRUCTURE(void)
{
  i32 vl_00;
  unk* xparp00;

  gdat.fileopencounter = 0;
  DM2_GRAPHICS_DATA_OPEN();

  // first get a long (two words) for version and number of entries
  if (!DM2_FILE_READ(gdat.filehandle, DOWNCAST(i32, &vl_00), sizeof(i32)))
    goto M_errexit;
  if ((CUTX16(vl_00) & wcon(0x8000)) == wcon(0x0))
    goto M_errexit;
  gdat.versionlo = CUTX16(vl_00) & lcon(0x7fff);
  gdat.entries = mkw(vl_00 >> 16);
  if (gdat.versionlo != wcon(0x4) && gdat.versionlo != wcon(0x5) && gdat.versionlo != wcon(0x2))
    goto M_errexit;

  // initialize the pointer/length-table
  dm2_ulp.init();

  // create w_table2 and fill it with 0xffff
  dm2_dballochandler.w_table2 = CHGCAST(t_dbidx, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(sizeof(t_dbidx) * unsignedlong(gdat.entries)));
  DM2_FILL_I16TABLE(CHGCAST(i16, dm2_dballochandler.w_table2), lcon(0xffffffff), unsignedlong(gdat.entries));

  // create a temporary word-table (ptrrg3)
  i32 longrg6 = sizeof(i16) * unsignedlong(gdat.entries);
  i16* wptrrg3 = XUPCAST(i16, dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(longrg6));
  ddat.vlong1e0a3c = longrg6 + 2 * sizeof(i16);
  if (gdat.versionlo >= wcon(0x3))
  {
    if (!DM2_FILE_READ(gdat.filehandle, DOWNCAST(i32, &ddat.vlong1e0a44), lcon(0x4)))
      goto M_errexit;
    ddat.vlong1e0a3c += ddat.vlong1e0a44 + sizeof(i16);
    if (!DM2_FILE_READ(gdat.filehandle, DOWNCAST(i16, wptrrg3 + 1), longrg6 - 2))
      goto M_errexit;
  }
  else
  {
    if (!DM2_FILE_READ(gdat.filehandle, DOWNCAST(i16, wptrrg3), longrg6))
      goto M_errexit;
    ddat.vlong1e0a44 = unsignedlong(*wptrrg3);
    ddat.vlong1e0a3c += unsignedlong(*wptrrg3);
  }
  *wptrrg3 = 0;
  ddat.vlong1e0a40 = ddat.vlong1e0a3c;
  ui16 uwordrg1 = 0;
  while (uwordrg1 < gdat.entries)
  {
    ddat.vlong1e0a40 += unsignedlong(*wptrrg3);
    dm2_ulp.setl(uwordrg1++, unsignedlong(*wptrrg3++));
  }
  i32 longrg1 = DM2_GET_FILE_SIZE(signedlong(gdat.filehandle));
  gdat.filesize = longrg1;
  if (longrg1 < ddat.vlong1e0a40)
    gdat.filetype1 = gdat.filetype2 = true;
  dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(longrg6);
  DM2_LOAD_ENT1();
  if (gdat.versionlo >= wcon(0x2) && gdat.versionlo != wcon(0x4))
  {
    if (DM2_QUERY_GDAT_ENTRY_DATA_INDEX(0, 0, lcon(0x8), 0) != lcon(0xffffffff))
    {
      i16 wordrg1 = CUTX16(DM2_QUERY_GDAT_ENTRY_DATA_LENGTH(0, 0, lcon(0x8), 0));
      ddat.v1e0ac2 = wordrg1;
      unk* xptrrg1 = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(unsignedlong(wordrg1));
      ddat.p1e0a64 = xptrrg1;
      xparp00 = xptrrg1;
      DM2_LOAD_GDAT_ENTRY_DATA_TO(0, 0, lcon(0x8), 0, xparp00);
      ddat.v1e0ac2 >>= 2; // unsigned
    }
  }
  ddat.v1e0a88 = true;
  i16 wordrg12 = DM2_QUERY_GDAT_ENTRY_DATA_INDEX(0, 0, lcon(0xb), 0);
  i16 wordrg41 = wordrg12;
  ddat.v1e0ad0 = wordrg12;
  wordrg12 &= lcon(0x20);
  i32 longrg12 = unsignedlong(wordrg12);
  dm2sound.v1d269c = longrg12;
  wordrg41 &= lcon(0x40);
  ddat.v1e0ad2 = wordrg41;
  if (wordrg41 == 0)
    dm2_dballochandler.v1e0acc = lcon(0x3e8);
  else
  {
    dm2_dballochandler.v1e0acc = lcon(0x1f);
    imgdecoder.alloc();
  }
  R_2D07D(); // had parameter longrg12
  dm2_dballochandler.DM2_dballoc_3e74_24b8();
  return;

M_errexit:
  DM2_RAISE_SYSERR(0x2a);
}

// recursive!
// gfx is always the struct after (therefore + 1 before the casts)
c_pixel* DM2_EXTRACT_GDAT_IMAGE(t_dbidx dbidx, i32 edxl)
{
  c_pixel* gfx;
  unk* xp_00; // can carry 0x80nnnnnn - datas!
  c_pixel16* xp_08;
  c_ol32 vol_0c; // poke 32 peek 8 32 add32 inc32
  i32 vl_10;
  t_dbidx bidx72;
  c_ql32 vql_18;
  i16 vw_1c;
  t_dbidx bidx0;
  i16 vw_24;
  t_dbidx bidx71;
  c_ql32 vql_2c;
  i16* xptrrg6; // is a pointer that can point to different types, depends on modes

  i32 longrg7 = edxl; // TODO: if edxl is a bool, then this one too
  bidx0 = NODATA;
  if (!dm2_dballochandler.gfxalloc_done)
  {
    s_xlink* s87p = XUPCAST(s_xlink, dm2_dballochandler.ptr1e0a5c);
    while (s87p != NULL)
    {
      if (dbidx == s87p->dbidx)
        return UPCAST(c_pixel, DOWNCAST(s_xlink, s87p + 1)); // the struct after
      s87p = s87p->s87p_00;
    }
  }
  else
  {
    bidx71 = dm2_dballochandler.DM2_ALLOC_CPX_GET_WTABLE_ENTRY(dbidx);
    if (bidx71 != NODATA)
    {
      if (longrg7 == 0)
        gfx = UPCAST(c_pixel, DOWNCAST(s_xmalloc, dm2_dballochandler.DM2_ALLOCATION6(bidx71) + 1));
      else
      {
        dm2_dballochandler.DM2_ALLOCATION2(bidx71);
        gfx = UPCAST(c_pixel, DOWNCAST(s_xmalloc, dm2_dballochandler.gfx_table[bidx71] + 1));
      }
      return gfx;
    }
  }

  vl_10 = 0;
  if (ddat.v1e0ac2 != 0)
  {
    i16 wordrg1 = DM2_TRACK_UNDERLAY(mkuw(dbidx));
    vw_24 = wordrg1;
    if (wordrg1 != lcon(0xffffffff))
    {
      vl_10 = 1;
      i32 longrg44;
      if (!dm2_dballochandler.gfxalloc_done)
        longrg44 = longrg7 == 0 ? 1 : 0;
      else
        longrg44 = 1;
      xp_08 = reinterpret_cast<c_pixel16*>(DM2_EXTRACT_GDAT_IMAGE((t_dbidx)vw_24, longrg44));
    }
  }
  xp_00 = dm2_ulp.getp(unsignedlong(dbidx));
  if (!dm2_ulp.islen(unsignedlong(dbidx)))
    xptrrg6 = CHGCAST(i16, xp_00);
  else
  {
    bool boolrg2;
    if (!dm2_dballochandler.gfxalloc_done)
      boolrg2 = longrg7 == 0;
    else
      boolrg2 = true;
    xptrrg6 = CHGCAST(i16, DM2_QUERY_GDAT_DYN_BUFF(dbidx, IO bidx0, boolrg2));
  }

  i16 wrg1 = xptrrg6[1] >> 10; // instead of l_00 >> 26
  if (wrg1 == lcon(0xffe0))
    return UPCAST(c_pixel, DOWNCAST(i16, xptrrg6) + 10);
  if (wrg1 != dm2_dballochandler.v1e0acc)
    vw_1c = 4;
  else
    vw_1c = 8;
  vql_18.poke16(xptrrg6[0] & wcon(0x3ff));
  i32 longrg43;
  if (vw_1c != 4)
    longrg43 = unsignedlong(vql_18.peek16());
  else
    longrg43 = mkul((vql_18.peek16() + 1 & wcon(0xfffe)) >> 1);
  vql_2c.poke16(xptrrg6[1] & wcon(0x3ff));
  vol_0c.poke32(unsignedlong(vql_2c.peek16()) * longrg43 & lcon(0xffff));
  if (!dm2_dballochandler.gfxalloc_done)
    if (vw_1c == 4)
      vol_0c.add32(lcon(0x10));
  if (!dm2_dballochandler.gfxalloc_done)
  {
    i16 wordrg1;
    if (longrg7 == 0)
      wordrg1 = BIGPOOL_HI;
    else
      wordrg1 = BIGPOOL_LO;
    s_xlink* s87prg1 = XUPCAST(s_xlink, dm2_dballochandler.DM2_ALLOC_MEMORY_RAM(vol_0c.peek32() + lcon(0xe), MASK_8, wordrg1));
    s87prg1->s87p_00 = XUPCAST(s_xlink, dm2_dballochandler.ptr1e0a5c);
    dm2_dballochandler.ptr1e0a5c = DOWNCAST(s_xlink, s87prg1);
    s87prg1->dbidx = dbidx;
    s87prg1->w_06 = wordrg1;
    s87prg1->b_08 = CUTX8(vw_1c);
    s87prg1->w_0a = vql_18.peek16();
    s87prg1->w_0c = vql_2c.peek16();
    gfx = UPCAST(c_pixel, DOWNCAST(s_xlink, s87prg1 + 1));
    if (vw_1c == 4)
      DM2_COPY_MEMORY(DOWNCAST(c_pixel, gfx) + vol_0c.peek32() - 16, DOWNCAST(i16, xptrrg6) + (dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH(dbidx) & lcon(0xffff)) - lcon(0x10), lcon(0x10));
  }
  else
  {
    if ((vol_0c.peek8() & bcon(0x1)) != 0)
      vol_0c.inc32();
    vol_0c.poke32(vol_0c.peek32() + lcon(0x16));
    s_xmalloc* s86p = CHGCAST(s_xmalloc, dm2_dballochandler.DM2_ALLOC_CPXHEAP_CREATE_POINTER(vol_0c.peek32()));
    if (bidx0 >= 0)
      xptrrg6 = CHGCAST(i16, dm2_dballochandler.DM2_GET_BMP(/*TAG bmpid*/ bidx0));
    bidx71 = dm2_dballochandler.DM2_ALLOC_CPXHEAP_CREATE_INDEX();
    dm2_dballochandler.w_table2[dbidx] = bidx71;
    dm2_dballochandler.gfx_table[bidx71] = CHGCAST(s_xmalloc, s86p);
    s86p->dbidx3 = dbidx;
    s86p->s78_0c.b_00 = CUTX8(vw_1c);
    s86p->s78_0c.w_02 = vql_18.peek16();
    s86p->s78_0c.w_04 = vql_2c.peek16();
    gfx = UPCAST(c_pixel, DOWNCAST(s_xmalloc, s86p + 1));
  }

  if (vl_10 == 0)
  {
    if (vw_1c != 8)
      imgdecoder.decode_img3_underlay(/*TAG src*/ xptrrg6, /*TAG dest*/ gfx);
    else
      imgdecoder.decode_img9(CHGCAST(ui8, xptrrg6), gfx, unsignedlong(CUTX16(dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH(dbidx))));
  }
  else
  {
    if (dm2_dballochandler.gfxalloc_done)
    {
      bidx72 = dm2_dballochandler.DM2_ALLOC_CPX_GET_WTABLE_ENTRY(vw_24);
      xp_08 = CHGCAST(c_pixel16, dm2_dballochandler.gfx_table[bidx72] + 1);
    }
    imgdecoder.decode_img3_overlay(xp_08, CHGCAST(i16, xptrrg6), gfx);
  }
  if (!dm2_dballochandler.gfxalloc_done)
  {
    if (is_neg_pointer(xp_00))
      dm2_dballochandler.DM2_DEALLOC_BIGPOOL_STRUCT_BEFORE(CHGCAST(s_malloctail, xptrrg6));
    if (vl_10 != 0)
      DM2_FREE_PICT_ENTRY(xp_08);
    return gfx;
  }
  if (longrg7 == 0)
    dm2_dballochandler.DM2_ALLOCATION7(bidx71, false);
  if (bidx0 >= 0)
    dm2_dballochandler.DM2_ALLOCATION1(bidx0, false);
  if (vl_10 != 0)
    dm2_dballochandler.DM2_ALLOCATION7(bidx72, false);
  return gfx;
}

// TODO: errexit-label left
void DM2_LOAD_DYN4(unk* xeaxp, i32 edxl)
{
  i32 longrg6;
  c_ql32 vql_00;
  unk* xp_04;
  s_gdat s36_08;
  s_hex6 s19_28;
  i32 vl_30;
  i32 vl_34;
  i32 vl_38;
  i32 vl_3c;
  i32 vl_40;
  i32 vl_44;
  i32 vl_48;
  i32 vl_4c;
  u_spec u25_50;
  i16 vw_54;
  i16 vw_58;
  i16 vw_5c;
  c_o32 vo_60; // poke 16 peek 8 32 and8 dec16 inc16
  i8 vb_64;

  xp_04 = xeaxp;
  vql_00.poke32(edxl);
  vl_48 = 0;
  vl_44 = 0;
  unk* xptrrg7 = dm2_dballochandler.DM2_ALLOC_LOBIGPOOL_MEMORY(unsignedlong(gdat.entries), true);
  if (!dm2_dballochandler.v1e0a84)
    DM2_SOUND5();
  u25_50.s19p_00 = UPCAST(s_hex6, DOWNCAST(unk, xp_04));
  vo_60.poke16(0);
  while (vo_60.peek16() < vql_00.peek16())
  {
    s36_08.l_00 = 1;
    unk* xptrtmprg6 = ADRESSOF(s_hex6, &s36_08.s19_04);
    unk* xptrtmprg5 = DOWNCAST(s_hex6, u25_50.s19p_00);
    DM2_COPY_MEMORY(xptrtmprg6, xptrtmprg5, 6);
    xptrtmprg5 += 6;
    xptrtmprg6 += 6;
    vw_5c = s36_08.s19_04.w_00;
    if ((s36_08.s19_04.w_00 & wcon(0x7fff)) == wcon(0x1))
    {
      u25_50.s19p_00++; // struct advance!
      unk* xptrtmprg6 = ADRESSOF(s_hex6, &s36_08.s19_0a);
      unk* xptrtmprg5 = DOWNCAST(s_hex6, u25_50.s19p_00);
      DM2_COPY_MEMORY(xptrtmprg6, xptrtmprg5, 6);
      xptrtmprg5 += 6;
      xptrtmprg6 += 6;
      vo_60.inc16();
    }

    while ((DM2_QUERY_NEXT_GDAT_ENTRY(&s36_08) ? 1 : 0) != 0)
    {
      if (s36_08.b_16 == bcon(0xb) || s36_08.b_16 == bcon(0xc))
        continue;
      i16 wordrg5c = s36_08.u31p_10->w_02;
      if ((wordrg5c & wcon(0x8000)) != wcon(0x0))
        continue;
      i32 longrg2 = unsignedlong(wordrg5c);
      unk* xptrrg19 = xptrrg7 + longrg2;
      i8 byterg4hi = *xptrrg19;
      i8 byterg4lo = byterg4hi & lcon(0x1f);
      if ((vw_5c & wcon(0x8000)) == 0)
      {
        if (byterg4hi == 0)
        {
          if (s36_08.b_16 == 2)
          {
            if (DM2_SOUND7(CUTX16(longrg2)) != 0 || dm2_dballochandler.v1e13fe[2] != bcon(0x0))
              continue;
          }
          xptrrg7[unsignedlong(wordrg5c)] = 1;
        }
        else
        {
          if (byterg4lo != lcon(0x1f))
            *xptrrg19 = ++byterg4hi;
        }
      }
      else if (byterg4lo != 0 && byterg4lo != bcon(0x1f))
        *xptrrg19 = byterg4hi - 1;
    }

    u25_50.s19p_00++; // struct advance
    if (signedlong(vo_60.peek16()) == signedlong(vql_00.peek16()) - 1 && vl_44 == 0)
    {
      vl_44 = 1;
      u25_50.s19p_00 = &s19_28;
      s19_28.w_00 = 0;
      vo_60.dec16();
      s19_28.splitlong(DM2_4BYTES_TO_LONG(21, bcon(0xfe), 1, bcon(0xfe))); // parameter sequence adjusted
    }
    vo_60.inc16();
  }

  i16 wordrg5b = 0;
  while (mkuw(wordrg5b) < gdat.entries)
  {
    unk* xptrrg1 = xptrrg7 + unsignedlong(wordrg5b);
    i16 wordrg44 = unsignedword(*xptrrg1);
    vo_60.poke16(wordrg44);
    if (wordrg44 != 0)
    {
      wordrg44 &= lcon(0x1f);
      if (wordrg44 == 0)
        *xptrrg1 &= lcon(0x20);
      else
        *xptrrg1 = vo_60.peek8() & bcon(0xc0) | bcon(0x21);
    }
    wordrg5b++;
  }
  u25_50.s19p_00 = UPCAST(s_hex6, DOWNCAST(unk, xp_04));
  vo_60.poke16(0);

  while (vo_60.peek16() < vql_00.peek16())
  {
    s36_08.l_00 = 1;
    unk* xptrtmprg6 = ADRESSOF(s_hex6, &s36_08.s19_04);
    unk* xptrtmprg5 = DOWNCAST(s_hex6, u25_50.s19p_00);
    DM2_COPY_MEMORY(xptrtmprg6, xptrtmprg5, 6);
    xptrtmprg5 += 6;
    xptrtmprg6 += 6;
    vw_5c = s36_08.s19_04.w_00;
    if ((s36_08.s19_04.w_00 & wcon(0x7fff)) != wcon(0x1))
    {
      if ((vw_5c & wcon(0x8000)) != wcon(0x0) || s36_08.s19_04.b_04 != bcon(0xff) && s36_08.s19_04.b_04 != bcon(0x2))
      {
        u25_50.s19p_00++; // struct advance
        vo_60.inc16();
        continue;
      }
      s36_08.s19_04.b_04 = 2;
    }
    else
    {
      unk* xptrrg1 = DOWNCAST(s_hex6, u25_50.s19p_00) + lcon(0x6);
      u25_50.s19p_00 = XUPCAST(s_hex6, xptrrg1);
      unk* xptrtmprg6 = ADRESSOF(s_hex6, &s36_08.s19_0a);
      unk* xptrtmprg5 = xptrrg1;
      DM2_COPY_MEMORY(xptrtmprg6, xptrtmprg5, 6);
      xptrtmprg5 += 6;
      xptrtmprg6 += 6;
      vo_60.inc16();
      if (s36_08.s19_04.b_04 == bcon(0xff) || mkub(s36_08.s19_04.b_04) <= mkub(2) && mkub(s36_08.s19_0a.b_04) >= mkub(2))
      {
        s36_08.s19_0a.b_04 = 2;
        s36_08.s19_04.b_04 = 2;
        if ((vw_5c & wcon(0x8000)) != 0 || s36_08.s19_04.b_04 != bcon(0xff) && s36_08.s19_04.b_04 != bcon(0x2))
        {
          u25_50.s19p_00++; // struct advance
          vo_60.inc16();
          continue;
        }
        s36_08.s19_04.b_04 = 2;
      }
      else
      {
        u25_50.s19p_00++; // struct advance
        vo_60.inc16();
        continue;
      }
    }
    for (;;)
    {
      if (!DM2_QUERY_NEXT_GDAT_ENTRY(&s36_08))
      {
        u25_50.s19p_00++; // struct advance
        vo_60.inc16();
        break;
      }
      i32 longrg17 = mkul(s36_08.u31p_10->w_02 & wcon(0x7fff));
      if (mkul(xptrrg7[longrg17] & bcon(0x1)) == 0)
      {
        if (DM2_SOUND7(CUTX16(longrg17)) == 0)
          continue;
      }
      i32 longrg2 = unsignedlong(s36_08.u31p_10->b_01);
      DM2_SOUND9(s36_08.b_14, s36_08.u31p_10->b_00, CUTLX8(longrg2));
    }
  }
  if (ddat.v1e0ac2 != 0)
  {
    i16 wordrg5a = 0;
    while (mkuw(wordrg5a) < gdat.entries)
    {
      i32 longrg18 = unsignedlong(wordrg5a);
      if (mkul(xptrrg7[longrg18] & bcon(0x1)) != 0)
        for (;;)
        {
          i16 wordrg19 = DM2_TRACK_UNDERLAY(mkuw(longrg18));
          if (wordrg19 == lcon(0xffffffff))
            break;
          i32 longrg43 = unsignedlong(wordrg19);
          if ((xptrrg7[longrg43] & bcon(0x1)) != 0)
            break;
          xptrrg7[longrg43] |= xptrrg7[wordrg5a] & bcon(0xc1);
          longrg18 = longrg43;
        }
      wordrg5a++;
    }
  }

  i32 longrg3;
  for (;;)
  {
    i32 longrg42 = 0;
    longrg3 = 0;
    if (dm2_dballochandler.pppw2.wp_00 == NULL)
      vl_30 = 2 * (dm2_dballochandler.pppw2.wp_00 - dm2_dballochandler.pppw2.wp_04);
    else
      vl_30 = 0;
    vl_3c = DOWNCAST(i16, dm2_dballochandler.pppw1.wp_00) - DOWNCAST(s_xmalloc, dm2_dballochandler.get_bigpool_start_b());

    i16 wordrg59 = 0;
    while (mkuw(wordrg59) < gdat.entries)
    {
      unk* xptrrg15 = xptrrg7 + unsignedlong(wordrg59);
      *xptrrg15 &= lcon(0xffffffe1);
      vo_60.poke16(unsignedword(*xptrrg15));
      u25_50.s19p_00 = XUPCAST(s_hex6, dm2_ulp.getp(unsignedlong(wordrg59)));
      // TODO: now check if MSB is set (special datas)
      if ((u25_50.l_00 & lcon(0x80000000)) != 0)
      {
        // TODO: now special area, STACKL(0x50) no pointer!
        if ((vo_60.peek16() & wcon(0x1)) != wcon(0x0))
        {
          i32 longrg2 = ((u25_50.l_00 & lcon(0xffff)) + 1 & lcon(0xfffffffe)) + lcon(0x4);
          if (vl_30 == 0)
          {
            xptrrg7[unsignedlong(wordrg59)] |= lcon(0x8);
            longrg3 += longrg2;
          }
          else
          {
            vw_58 = vo_60.peek16() & wcon(0x40);
            if (vw_58 == 0)
            {
              *xptrrg15 |= lcon(0x10);
              longrg42 += longrg2;
            }
            else
            {
              xptrrg7[unsignedlong(wordrg59)] |= lcon(0x8);
              longrg3 += longrg2;
            }
          }
        }
      }
      else
      {
        i16* wptr0 = UPCAST(i16, DOWNCAST(s_hex6, u25_50.s19p_00) - 2);
        i32 longrg2 = mkul(*wptr0 + 1) & lcon(0xfffffffe) + lcon(0x4);
        unk* xptrrg11;
        if (dm2_dballochandler.pppw1.wp_00 == NULL || (xptrrg11 = XDOWNCAST(s_hex6, u25_50.s19p_00)) < XDOWNCAST(i16, dm2_dballochandler.pppw1.wp_04) || xptrrg11 > XDOWNCAST(i16, dm2_dballochandler.pppw1.wp_00))
        {
          if (dm2_dballochandler.pppw2.wp_00 == NULL || (xptrrg11 = XDOWNCAST(s_hex6, u25_50.s19p_00)) < XDOWNCAST(i16, dm2_dballochandler.pppw2.wp_04) || xptrrg11 > XDOWNCAST(i16, dm2_dballochandler.pppw2.wp_00))
            xptrrg7[unsignedlong(wordrg59)] = 0;
          else
          {
            unk* xptrrg1 = xptrrg7 + unsignedlong(wordrg59);
            *xptrrg1 |= lcon(0x14);
            vw_58 = vo_60.peek16() & 0x1;
            if (vw_58 == 0)
              *xptrrg1 |= lcon(0xffffff80);
            longrg42 += longrg2;
          }
        }
        else
        {
          unk* xptrrg1 = xptrrg7 + unsignedlong(wordrg59);
          *xptrrg1 |= lcon(0xa);
          vw_58 = vo_60.peek16() & 0x1;
          if (vw_58 == 0)
            *xptrrg1 |= lcon(0x40);
          longrg3 += longrg2;
        }
      }
      wordrg59++;
    }
    if (longrg42 > vl_30)
    {
      i16 wordrg58 = 0;
      for (;;)
      {
        if (vl_3c - longrg3 <= dm2_dballochandler.v1d6775 || longrg42 <= vl_30 || mkuw(wordrg58) >= gdat.entries)
        {
          wordrg58 = 0; // no new variable here!
          while (vl_3c - longrg3 > dm2_dballochandler.v1d6775 && longrg42 > vl_30 && mkuw(wordrg58) < gdat.entries)
          {
            unk* xptrrg1 = xptrrg7 + wordrg58;
            vb_64 = *xptrrg1 & lcon(0xffffff90);
            if (vb_64 == lcon(0x10))
            {
              *xptrrg1 ^= lcon(0x18);
              i32 longrg2 = ((dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH((t_dbidx)wordrg58) & lcon(0xffff)) + 1 & lcon(0xfffffffe)) + lcon(0x4);
              longrg3 += longrg2;
              longrg42 -= longrg2;
            }
            wordrg58++;
          }
          wordrg58 = 0; // no new variable here!
          while (longrg42 > vl_30 && mkuw(wordrg58) < gdat.entries)
          {
            unk* xptrrg1 = xptrrg7 + wordrg58;
            vb_64 = *xptrrg1 & lcon(0x11);
            if (vb_64 == lcon(0x10))
            {
              *xptrrg1 &= lcon(0xffffffef);
              longrg42 -= ((dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH((t_dbidx)wordrg58) & lcon(0xffff)) + 1 & lcon(0xfffffffe)) + lcon(0x4);
            }
            wordrg58++;
          }

          if (dm2_dballochandler.v1e13fe[3] == 0)
          {
            wordrg58 = 0; // no new variable here!
            while (longrg42 > vl_30 && mkuw(wordrg58) < gdat.entries)
            {
              unk* xptrrg1 = xptrrg7 + wordrg58;
              vb_64 = *xptrrg1 & lcon(0xffffff94);
              if (vb_64 == lcon(0x10))
              {
                *xptrrg1 ^= lcon(0x18);
                i32 longrg2 = ((dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH((t_dbidx)wordrg58) & lcon(0xffff)) + 1 & lcon(0xfffffffe)) + lcon(0x4);
                longrg3 += longrg2;
                longrg42 -= longrg2;
              }
              wordrg58++;
            }
            wordrg58 = 0; // no new variable here!
            while (longrg42 > vl_30 && mkuw(wordrg58) < gdat.entries)
            {
              unk* xptrrg1 = xptrrg7 + wordrg58;
              vb_64 = *xptrrg1 & lcon(0xffffff90);
              if (vb_64 == lcon(0x10))
              {
                *xptrrg1 ^= lcon(0x18);
                i32 longrg2 = ((dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH((t_dbidx)wordrg58) & lcon(0xffff)) + 1 & lcon(0xfffffffe)) + lcon(0x4);
                longrg3 += longrg2;
                longrg42 -= longrg2;
              }
              wordrg58++;
            }
          }
          break; // wordrg58 stays alive!
        }
        unk* xptrrg1 = xptrrg7 + wordrg58;
        vb_64 = *xptrrg1 & lcon(0xffffff94);
        if (vb_64 == lcon(0x10))
        {
          *xptrrg1 ^= lcon(0x18);
          i32 longrg2 = ((dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH((t_dbidx)wordrg58) & lcon(0xffff)) + 1 & lcon(0xfffffffe)) + lcon(0x4);
          longrg3 += longrg2;
          longrg42 -= longrg2;
        }
        wordrg58++;
      }
    }
    i16 wordrg56 = 0;
    for (;;)
    {
      vl_38 = vl_30 - longrg42;
      if (vl_38 < lcon(0x6) || mkuw(wordrg56) >= gdat.entries)
        break;
      unk* xptrrg6 = xptrrg7 + unsignedlong(wordrg56);
      vb_64 = *xptrrg6 & lcon(0x48);
      if (vb_64 == lcon(0x8))
      {
        i32 longrg2 = ((dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH((t_dbidx)wordrg56) & lcon(0xffff)) + 1 & lcon(0xfffffffe)) + lcon(0x4);
        if (longrg2 <= vl_38)
        {
          *xptrrg6 ^= lcon(0x18);
          longrg42 += longrg2;
          longrg3 -= longrg2;
        }
      }
      wordrg56++;
    }
    i16 wordrg55 = 0;
    while (vl_3c - longrg3 <= dm2_dballochandler.v1d6775 && mkuw(wordrg55) < gdat.entries)
    {
      unk* xptrrg1 = xptrrg7 + wordrg55;
      vb_64 = *xptrrg1 & lcon(0x9);
      if (vb_64 == lcon(0x8))
      {
        *xptrrg1 ^= lcon(0xfffffff7);
        longrg3 -= ((dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH((t_dbidx)wordrg55) & lcon(0xffff)) + 1 & lcon(0xfffffffe)) + lcon(0x4);
      }
      wordrg55++;
    }
    i32 longrg13 = vl_3c - longrg3;
    dm2_dballochandler.v1e0a74 = vl_30 - longrg42;
    i32 longrg2 = longrg13 - dm2_dballochandler.v1e0a90;
    dm2_dballochandler.v1e0a8c = longrg2;
    dm2_dballochandler.v1e0a0c = longrg13;
    if (dm2_dballochandler.v1e0a84)
    {
      dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(unsignedlong(gdat.entries));
      return;
    }
    bool jump = false;
    if (longrg2 >= 0 && dm2_dballochandler.v1e0a74 >= 0)
      jump = true;
    else if (vl_48 != 0)
    {
      DM2_RAISE_SYSERR(0x2c);
      jump = true;
    }
    if (jump)
    {
      vl_40 = DOWNCAST(i16, dm2_dballochandler.pppw1.wp_08) - (DOWNCAST(i16, dm2_dballochandler.pppw1.wp_00) - longrg3);
      i16 wordrg54 = 0;
      while (mkuw(wordrg54) < gdat.entries)
      {
        unk* xptrrg1 = xptrrg7 + unsignedlong(wordrg54);
        i16 wordrg2 = unsignedword(*xptrrg1);
        vo_60.poke16(wordrg2);
        wordrg2 &= 0x6;
        if (wordrg2 != wcon(0x0) && (vo_60.peek16() & wcon(0x18)) == wcon(0x0))
        {
          *xptrrg1 &= lcon(0x20);
          dm2_dballochandler.DM2_ALLOCATION3((t_dbidx)wordrg54);
          dm2_ulp.R_2D87A(wordrg54);
        }
        wordrg54++;
      }
      if (vl_40 > 0)
        dm2_dballochandler.DM2_GUARANTEE_FREE_CPXHEAP_SIZE(vl_40);
      dm2_dballochandler.DM2_INIT_GFX_TABLE();
      longrg6 = vl_40;
      if (longrg6 > 0)
      {
        dm2_dballochandler.cpx_available -= longrg6;
        dm2_dballochandler.pppw1.wp_04 = dm2_dballochandler.pppw1.wp_00 - longrg3 / 2;
      }
      do
      {
        unk* xptrrg4;
        vl_4c = 0;
        i16 wordrg53 = 0;
        while (mkuw(wordrg53) < gdat.entries)
        {
          bool pass = false;
          i16 wordrg10 = wordrg53;
          i16 wordrg42 = unsignedword(xptrrg7[wordrg10]);
          vo_60.poke16(wordrg42);
          wordrg42 &= lcon(0xff0c);
          unk* xptrrg10;
          if (wordrg42 != lcon(0xc))
          {
            if ((vo_60.peek16() & wcon(0x12)) != wcon(0x12))
              pass = true;
            else
            {
              i32 longrg2 = (unsignedlong(CUTX16(dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH((t_dbidx)wordrg10))) + 1 & lcon(0xfffffffe)) + lcon(0x4);
              i32 longrg12;
              if (dm2_dballochandler.pppw2.wp_00 != NULL)
                longrg12 = 2 * (dm2_dballochandler.pppw2.wp_08 - dm2_dballochandler.pppw2.wp_04);
              else
                longrg12 = 0;
              if (longrg12 < longrg2)
              {
                R_2D802(&dm2_dballochandler.pppw2);
                if (dm2_dballochandler.pppw2.wp_00 != NULL)
                  longrg12 = 2 * (dm2_dballochandler.pppw2.wp_08 - dm2_dballochandler.pppw2.wp_04);
                else
                  longrg12 = 0;
                if (longrg12 < longrg2)
                {
                  vl_4c = 1;
                  pass = true;
                }
              }
              if (!pass)
              {
                longrg6 = unsignedlong(wordrg53);
                vl_34 = longrg6;
                xptrrg4 = dm2_ulp.getp(longrg6);
                xptrrg10 = DOWNCAST(i16*, &dm2_dballochandler.pppw2.wp_00); // TODO: or & of structstart
              }
            }
          }
          else
          {
            i32 longrg2 = ((dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH((t_dbidx)wordrg10) & lcon(0xffff)) + 1 & lcon(0xfffffffe)) + lcon(0x4);
            i32 longrg11;
            if (dm2_dballochandler.pppw1.wp_00 == NULL)
              longrg11 = 0;
            else
              longrg11 = 2 * (dm2_dballochandler.pppw1.wp_08 - dm2_dballochandler.pppw1.wp_04);
            if (longrg11 < longrg2)
            {
              R_2D802(&dm2_dballochandler.pppw1);
              if (dm2_dballochandler.pppw1.wp_00 != NULL)
                longrg11 = 2 * (dm2_dballochandler.pppw1.wp_08 - dm2_dballochandler.pppw1.wp_04);
              else
                longrg11 = 0;
              if (longrg11 < longrg2)
              {
                vl_4c = 1;
                pass = true;
              }
            }
            if (!pass)
            {
              longrg6 = unsignedlong(wordrg53);
              vl_34 = longrg6;
              xptrrg4 = dm2_ulp.getp(longrg6);
              xptrrg10 = DOWNCAST(i16*, &dm2_dballochandler.pppw1.wp_00); // TODO or & of structstart
            }
          }
          if (!pass)
          {
            xptrrg4 = XDOWNCAST(i16, R_2D8BA(XUPCAST(s_pppw, xptrrg10), xptrrg4, longrg2));
            dm2_ulp.R_2D87A(CUTX16(longrg6));
            dm2_ulp.setp(vl_34, xptrrg4);
            xptrrg7[longrg6] ^= 0x6;
          }
          wordrg53++;
        }
      } while (vl_4c != 0);
      break; // breaks main loop, goes to the end par_tp
    }
    vl_48 = 1;
    while (dm2_dballochandler.malloch != NULL)
      dm2_dballochandler.DM2_ALLOCATION3(dm2_dballochandler.malloch->dbidx3);

    i16 wordrg5 = 0;
    while (mkuw(wordrg5) < gdat.entries)
    {
      u25_50.s19p_00 = XUPCAST(s_hex6, dm2_ulp.getp(unsignedlong(wordrg5)));
      if ((u25_50.l_00 & lcon(0x80000000)) == 0)
      {
        unk* xptrrg61 = DOWNCAST(i16, dm2_dballochandler.pppw1.wp_00);
        bool passx = false;
        unk* xptrrg11;
        if (xptrrg61 == NULL || (xptrrg11 = XDOWNCAST(s_hex6, u25_50.s19p_00)) < XDOWNCAST(i16, dm2_dballochandler.pppw1.wp_04) || xptrrg11 > xptrrg61)
        {
          if (dm2_dballochandler.pppw2.wp_00 == NULL || (xptrrg11 = XDOWNCAST(s_hex6, u25_50.s19p_00)) < XDOWNCAST(i16, dm2_dballochandler.pppw2.wp_04) || DOWNCAST(unk, xptrrg11) > DOWNCAST(i16, dm2_dballochandler.pppw2.wp_00))
            passx = true;
        }
        if (!passx)
        {
          i16 wordrg1 = unsignedword(xptrrg7[unsignedlong(wordrg5)]);
          vo_60.poke16(wordrg1);
          if ((wordrg1 & wcon(0xff01)) == wcon(0x0))
            vo_60.poke16(vo_60.peek16() & wcon(0x20));
          else
            vo_60.and16(lcon(0xfffffff9));
          xptrrg7[unsignedlong(wordrg5)] = vo_60.peek8();
          dm2_ulp.R_2D87A(wordrg5);
        }
      }
      wordrg5++;
    }
    R_2D802(&dm2_dballochandler.pppw1);
    R_2D802(&dm2_dballochandler.pppw2);
    dm2_dballochandler.cpx_available += 2 * (dm2_dballochandler.pppw1.wp_08 - dm2_dballochandler.pppw1.wp_04);
    dm2_dballochandler.pppw1.wp_04 = dm2_dballochandler.pppw1.wp_08;
  }

  // end par_tp
  R_2D802(&dm2_dballochandler.pppw1);
  R_2D802(&dm2_dballochandler.pppw2);
  if (vl_40 < 0)
  {
    dm2_dballochandler.cpx_available -= vl_40;
    dm2_dballochandler.pppw1.wp_04 = dm2_dballochandler.pppw1.wp_00 - longrg3 / 2; // !
  }

  bool boolrg6 = false;
  i16 wordrg5 = 0;
  while (mkuw(wordrg5) < gdat.entries)
  {
    if (dm2_dballochandler.v1e0200)
    {
      i32 longrg4 = 500 * (unsignedlong(wordrg5) + 1);
      i32 longrg2 = unsignedlong(gdat.entries);
      DM2_DRAW_DIALOGUE_PROGRESS(longrg4 / longrg2 + lcon(0x1f4));
    }
    i16 wordrg4 = unsignedword(xptrrg7[unsignedlong(wordrg5)]) & lcon(0xdf);
    vo_60.poke16(wordrg4);
    if (wordrg4 != 0)
    {
      wordrg4 &= lcon(0xff06);
      if (wordrg4 == 0)
      {
        vw_54 = CUTX16(dm2_ulp.DM2_QUERY_GDAT_RAW_DATA_LENGTH((t_dbidx)wordrg5));
        i32 longrg2 = (unsignedlong(vw_54) + 1 & lcon(0xfffffffe)) + lcon(0x4);
        s_pppw* s83prg1;
        if ((vo_60.peek16() & wcon(0x8)) == wcon(0x0))
          s83prg1 = &dm2_dballochandler.pppw2;
        else
          s83prg1 = &dm2_dballochandler.pppw1;
        unk* xptrrg11 = XDOWNCAST(i16, R_2D8AD(s83prg1, longrg2));
        u25_50.s19p_00 = XUPCAST(s_hex6, xptrrg11);
        vl_34 = unsignedlong(wordrg5);
        u25_50.s19p_00 = XUPCAST(s_hex6, xptrrg11 + 2);
        dm2_ulp.setp(vl_34, DOWNCAST(s_hex6, u25_50.s19p_00));
        i16* wptr1 = UPCAST(i16, DOWNCAST(s_hex6, u25_50.s19p_00) + longrg2 - lcon(0x4));
        *wptr1 = wordrg5;
        i16* wptr2 = UPCAST(i16, DOWNCAST(s_hex6, u25_50.s19p_00) - 2);
        *wptr2 = vw_54;
        if (!boolrg6)
        {
          DM2_GRAPHICS_DATA_OPEN();
          boolrg6 = true;
        }
        DM2_LOAD_GDAT_RAW_DATA((t_dbidx)wordrg5, dm2_ulp.getp(unsignedlong(wordrg5)));
      }
    }
    wordrg5++;
  }

  if (boolrg6)
    DM2_GRAPHICS_DATA_CLOSE();
  dm2_dballochandler.bigpool_endofunpreservedfree = VCAST(dm2_dballochandler.pppw1.wp_04);
  DM2_482b_0684();
  if (dm2_dballochandler.v1e0200)
    DM2_DRAW_DIALOGUE_PROGRESS(lcon(0x3e8));
  dm2_dballochandler.DM2_DEALLOC_LOBIGPOOL(unsignedlong(gdat.entries));
}

// TODO: optimize temporaries
#include "emu.h"
#include "regs.h"

// was SKW_0b36_020e
void DM2_LOAD_GDAT_INTERFACE_00_02(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i8* RG51p;
  i8* RG52p;
  i8 vb_00;
  unk* xparp00;

  RG1P = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(DM2_QUERY_GDAT_ENTRY_DATA_LENGTH(1, 0, lcon(0x7), 2) & lcon(0xffff));
  RG51p = RG1P;
  xparp00 = RG1P;
  DM2_LOAD_GDAT_ENTRY_DATA_TO(1, 0, lcon(0x7), 2, xparp00);
  RG1Blo = byte_at(RG51p);
  vb_00 = RG1Blo;
  RG1L = unsignedlong(vb_00);
  RG4L = 8 * RG1L;
  RG1L += RG4L;
  RG3P = RG51p + 1;
  RG1P = dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(RG1L);
  ddat.v1e020c = RG1P;
  RG1L = 0;
  //m_AF95:
  for (;;)
  {
    RG4L = unsignedlong(vb_00);
    if (RG1W >= RG4W)
    {
      RG1L = 0;
      break;
    }
    //m_AF7B:
    RG52p = RG3P;
    RG2L = signedlong(RG1W);
    RG4L = 9 * RG2L;
    RG2P = ddat.v1e020c;
    RG3P++;
    RG4P = RG2P + RG4L;
    RG2Blo = byte_at(RG52p);
    mov8(location(RG4P), RG2Blo);
    RG1L++;
  }
  //m_AFC6:
  for (;;)
  {
    RG4L = unsignedlong(vb_00);
    if (RG1W >= RG4W)
    {
      RG1L = 0;
      break;
    }
    //m_AFA3:
    RG2L = signedlong(RG1W);
    RG4L = 8 * RG2L;
    RG2L += RG4L;
    movp(location(ddat.v1e020c + RG2L + 1), RG3P);
    RG2P = ddat.v1e020c + RG2L;
    RG3P += unsignedlong(byte_at(RG2P));
    RG1L++;
  }
  //m_AFFA:
  for (;;)
  {
    RG4L = unsignedlong(vb_00);
    if (RG1W >= RG4W)
    {
      ddat.v1e0210 = UPCAST(i16, RG3P);
      return;
    }
    //m_AFD4:
    RG2L = signedlong(RG1W);
    RG4L = 8 * RG2L;
    RG2L += RG4L;
    movp(location(ddat.v1e020c + RG2L + lcon(0x5)), RG3P);
    RG4P = ddat.v1e020c;
    RG4Blo = byte_at(RG4P, RG2L);
    RG4L = unsignedlong(RG4Blo);
    RG3P += RG4L;
    RG1L++;
  }
  FOREND
}

// was SKW_32cb_0008
i32 DM2_LOAD_GDAT_INTERFACE_00_0A(void)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4;
  i16 RG5w;
  i8* RG6p;
  i16 vw_00;
  i16 vw_04;
  i16 vw_08;
  i16 parw01;
  i16 parw02;

  RG4L = signedlong(ddat.v1e0272);
  RG1W = record_to_word(DM2_GET_TILE_RECORD_LINK(ddat.v1e0270, RG4W));
  //m_3C0A3:
  for (;;)
  {
    RG5w = RG1W;
    RG1Blo = RG5w != wcon(0xfffe) ? 1 : 0;
    RG1L = unsignedlong(RG1Blo);
    if (RG1L == 0)
      return 0;
    RG1W = RG5w;
    RG1W &= lcon(0x3c00);
    RG1L = unsignedlong(RG1W) >> bcon(0xa);
    RG1L = signedlong(RG1W);
    if (RG1L > lcon(0x3))
      return 0;
    if (RG1L == 2)
    {
      RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(mkuw(RG5w)));
      RG6p = RG1P;
      RG2W = word_at(RG1P, 2) & lcon(0x6);
      if (RG2W == 2)
      {
        RG1W = word_at(RG1P, 2);
        RG1UW >>= bcon(0x3);
        RG1L = unsignedlong(RG1W);
        RG1UL >>= bcon(0x8);
        RG1L &= lcon(0x1f);
        if (mkul(RG1L) >= mkul(lcon(0xf)))
        {
          bool skip00841 = false;
          if (mkul(RG1L) <= mkul(lcon(0xf)))
          {
            //m_3C128:
            vw_08 = 4;
            skip00841 = true;
          }
          else
          {
            if (RG1L == lcon(0x10))
            {
              vw_08 = 5;
              skip00841 = true;
            }
          }

          if (skip00841)
          {
            //m_3C12F:
            RG1W = word_at(RG6p, 2);
            RG1UW >>= bcon(0x3);
            RG1W = unsignedword(RG1Blo);
            RG2L = unsignedlong(RG1W);
            RG1L = unsignedlong(vw_08);
            RG3P = ADRESSOF(i16, &vw_04);
            RG4L = 0;
            RG1L = DM2_3D93B(RG1L, RG4L, RG2L, RG3P16, &vw_00);
            if (RG1W >= 0)
            {
              RG3L = signedlong(ddat.v1e0258);
              RG2L = signedlong(RG1W);
              DM2_map_3BF83(signedlong(vw_04), signedlong(vw_00), RG2L, RG3L);
              RG1L = signedlong(ddat.v1e0272);
              put16(parw02, RG1W);
              RG1L = signedlong(ddat.v1e0270);
              put16(parw01, RG1W);
              RG1W = word_at(RG6p, 2);
              RG1UW >>= bcon(0x3);
              RG4L = unsignedlong(RG1Blo);
              DM2_QUEUE_NOISE_GEN2(lcon(0xa), RG4Blo, lcon(0x88), lcon(0xfe), parw01, parw02, lcon(0x1), lcon(0x8c), lcon(0x80));
              RG1L = signedlong(vw_08);
              if (RG1L == lcon(0x4))
                or8(location(RG6p + 2), lcon(0x1));
              return lcon(0x1);
            }
          }
        }
      }
    }
    //m_3C1CC:
    RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(mkuw(RG5w))));
  }
  FOREND
}

// stacksize was 0x40
i32 DM2_3D93B(i32 eaxl, i32 edxl, i32 ebxl, i16* ecxpw, i16* argpw0)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3; RG3P = DOWNCAST(i16, ecxpw);
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  c_wlreg _RG6;
  i8* RG7p;
  unk* xp_00;
  unk* xp_04; // add32(0x4, 2);
  c_ql32 vql_08;
  i16 vw_0c;
  i16 vw_10;
  i32 vl_14;
  c_o32 vo_18; // poke 16 peek 32 inc16
  c_ol32 vol_1c; // poke 32 peek 16 32 inc32
  c_o32 vo_20; // poke 16 poke 32 inc16
  i16 vw_24;
  i32 vl_28; // inc32
  i32 vl_2c;
  bool flag;

  RG5L = RG1L;
  vl_14 = RG4L;
  vw_0c = RG2W; // truncated
  RG7p = RG3P;
  RG4L = 0;
  vl_28 = RG4L;
  vl_2c = lcon(0xffffffff);
  vol_1c.poke32(RG4L);
  //m_3DB8E:
  for (;;)
  {
    RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
    RG1Blo = byte_at(RG1P, lcon(0x4));
    RG1L = unsignedlong(RG1Blo);
    RG2L = unsignedlong(vol_1c.peek16());
    if (RG2L >= RG1L)
    {
      DM2_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
      RG1L = unsignedlong(RG5W);
      if (RG1L == lcon(0x4))
        return vl_2c;
      return RG1L == lcon(0x5) ? lcon(0xffffffff) : vl_28;
    }
    //m_3D964:
    RG1L = 16 * RG2L;
    RG4P = ddat.v1e03c8;
    RG1P = RG4P + RG1L;
    RG4W = word_at(RG1P, lcon(0x8));
    RG4L <<= bcon(0x5);
    _RG4.ushiftr11();
    vw_10 = RG4W; // truncated
    RG1W = word_at(RG1P, lcon(0x8));
    _RG1.ushiftr11();
    vw_24 = RG1W; // truncated
    RG1P = pointer_at(pointer_at(DOWNCAST(t_tile*, mapdat.tmpmap) + 4 * RG2L));
    xp_00 = UPCAST(unk, RG1P);
    RG1L = 2 * unsignedlong(word_at(ddat.v1e03d8, 2 * unsignedlong(ddat.v1e03e4[RG2L])));
    xp_04 = UPCAST(unk, DOWNCAST(t_record, dm2_v1e038c + RG1L / 2));
    vo_20.poke16(wcon(0x0));
    //m_3DB74:
    for (;;)
    {
      RG2L = signedlong(vo_20.peek16());
      RG1L = unsignedlong(vw_10);
      if (RG2L > RG1L)
      {
        vol_1c.inc32();
        break;
      }
      //m_3D9D3:
      RG4L = 0;
      vo_18.poke16(RG4W);
      //m_3DB59:
      for (;;)
      {
        RG2L = signedlong(vo_18.peek16());
        RG1L = unsignedlong(vw_24);
        if (RG2L > RG1L)
        {
          vo_20.inc16();
          break;
        }
        //m_3D9DF:
        RG1P = DOWNCAST(unk, xp_00);
        xp_00 = UPCAST(unk, RG1P + lcon(0x1));
        if (jnz_test8(location(RG1P), lcon(0x10)))
        {
          RG4P = DOWNCAST(unk, xp_04);
          RG4W = word_at(RG4P);
          xp_04 += 2; // DANGER
          //m_3D9FD:
          for (;;)
          {
            if (RG5UW >= mkuw(2) && RG5UW <= mkuw(lcon(0x5)))
            {
              RG1L = RG4L;
              RG1Blo ^= RG4Blo;
              RG1Bhi &= lcon(0x3c);
              RG1L = unsignedlong(RG1W) >> bcon(0xa);
              if (RG1L == 2)
              {
                RG1RP = DM2_GET_ADDRESS_OF_RECORD(mk_record(RG4UW));
                RG2W = word_at(RG1P, 2) & lcon(0x6);
                if (RG2W == 2)
                {
                  bool skip00874 = false;
                  RG3W = word_at(RG1P, 2);
                  RG2L = RG3L;
                  RG2UW >>= bcon(0x3);
                  vql_08.poke32(RG2L);
                  RG2L = unsignedlong(vql_08.peek16());
                  RG2UL >>= bcon(0x8);
                  RG2L &= lcon(0x1f);
                  RG6L = vql_08.peek32() & lcon(0xff);
                  if (mkul(RG2L) < mkul(lcon(0xf)))
                  {
                    //m_3DA80:
                    if (RG2L == lcon(0xb))
                    {
                      RG1L = unsignedlong(RG6W);
                      RG2L = unsignedlong(vw_0c);
                      if (RG1L == RG2L)
                      {
                        RG1L ^= RG2L;
                        RG1W = RG5W;
                        if (RG1L != lcon(0x3))
                        {
                          //m_3DAB0:
                          RG3L = vl_14;
                          flag = --RG3W != 0;
                          vl_14 = RG3L;
                          if (!flag)
                            skip00874 = true;
                        }
                        else
                          vl_28++;
                      }
                    }
                  }
                  else
                  {
                    if (mkul(RG2L) <= mkul(lcon(0xf)))
                    {
                      //m_3DB05:
                      RG2L = unsignedlong(RG5W);
                      if (RG2L != lcon(0x4))
                      {
                        //m_3DB18:
                        if (RG2L == lcon(0x5) && (RG3Blo & bcon(0x1)) != 0)
                          skip00874 = true;
                      }
                      else
                      {
                        RG3L &= lcon(0xfffffffe);
                        mov16(location(RG1P + 2), RG3W);
                      }
                    }
                    else
                    {
                      if (RG2L == lcon(0x10))
                      {
                        //m_3DAD8:
                        RG1L = unsignedlong(RG6W);
                        RG2L = unsignedlong(vw_0c);
                        if (RG1L == RG2L)
                        {
                          RG1W = vo_20.peek16();
                          mov16(location(RG7p + 0), RG1W);
                          RG1W = vo_18.peek16();
                          RG2P = DOWNCAST(i16, argpw0);
                          mov16(location(RG2P), RG1W);
                          vl_2c = vol_1c.peek32();
                        }
                      }
                    }
                  }

                  if (skip00874)
                  {
                    //m_3DABC:
                    RG1W = vo_20.peek16();
                    mov16(location(RG7p + 0), RG1W);
                    RG1W = vo_18.peek16();
                    mov16(location(DOWNCAST(i16, argpw0)), RG1W);
                    return vol_1c.peek32();
                  }
                }
              }
            }
            //m_3DB22:
            RG1L = record_to_long(DM2_GET_NEXT_RECORD_LINK(mk_record(RG4UW)));
            RG4L = RG1L;
            RG1Blo = RG1W != wcon(0xfffe) ? 1 : 0;
            RG1L = unsignedlong(RG1Blo);
            if (RG1L != 0)
            {
              RG1L = RG4L;
              RG1W &= lcon(0x3c00);
              RG1L = unsignedlong(RG1W) >> bcon(0xa);
              if (RG1L <= lcon(0x3))
                continue;
            }
            break;
          }
        }
        //m_3DB54:
        vo_18.inc16();
      }
    }
  }
  FOREND
}
