#include <util.h>
#include <startend.h> // THROW..., DM2_RAISE_SYSERR
#include <dm2data.h>
#include <skgdtqdb.h>
#include <skmovrec.h>
#include <skrandom.h>
#include <skrecord.h>
#include <skalloc.h>
#include <sklodlvl.h>
#include <sktimer.h>
#include <skguidrw.h>
#include <bgdat.h>
#include <SK0AAF.h>
#include <skalocdb.h>

// TODO: search for CAST for dirty accesses

#include <emu.h>
#include <regs.h>

#include <dm2debug.h>

// #########################################################################

// was SKW_3e74_0d32
void c_cpx_linklist::DM2_ALLOC_CPX_LINK_NODE(s_lpp* lpptr)
{
  if (ptrlist == NULL)
  {
    ptrlist = lpptr;
    lpptr->next = lpptr->prev = NULL;
    return;
  }

  s_lpp* liststart = ptrlist;
  i32 amount = lpptr->amount;
  if (amount >= liststart->amount)
  {
    ptrlist = lpptr;
    lpptr->prev = NULL;
    lpptr->next = liststart;
    liststart->prev = lpptr;
    return;
  }

  s_lpp* next;
  while ((next = liststart->next) != NULL)
  {
    if (amount >= next->amount)
    {
      next->prev = liststart->next = lpptr;
      lpptr->prev = liststart;
      lpptr->next = next;
      return;
    }
    liststart = next;
  }

  liststart->next = lpptr;
  lpptr->prev = liststart;
  lpptr->next = NULL;
}

// was SKW_3e74_0c8c
void c_cpx_linklist::DM2_ALLOC_CPX_UNLINK_NODE(s_lpp* lpptr)
{
  s_lpp* prev = lpptr->prev;
  s_lpp* next = lpptr->next;

  if (prev != NULL)
  {
    prev->next = next;
    if (next != NULL)
      next->prev = prev;
    return;
  }
  else if (next != NULL)
  {
    ptrlist = next;
    next->prev = prev;
  }
  else
    ptrlist = prev;
}

// extracted, used in DM2_ALLOC_CPXHEAP_CREATE_POINTER
i32 c_cpx_linklist::DM2_ALLOC_CPX1(OO unk*& xptr, i32 amount)
{
  s_lpp* lpptr = ptrlist;
  while (amount != lpptr->amount)
  {
    if (amount > lpptr->amount || lpptr->next == NULL)
    {
      lpptr = ptrlist;
      break;
    }
    else
      lpptr = lpptr->next;
  }
  DM2_ALLOC_CPX_UNLINK_NODE(lpptr);

  xptr = DOWNCAST(s_lpp, lpptr);
  i32 diff = lpptr->amount - amount;
  if (diff < 30)
    amount = lpptr->amount;
  else
  {
    *CHGCAST(i32, xptr + amount + diff - 4) = diff;
    *CHGCAST(i32, xptr + amount) = diff;
    DM2_ALLOC_CPX_LINK_NODE(XUPCAST(s_lpp, xptr + amount));
  }

  return amount;
}

// #########################################################################

c_ulp dm2_ulp;

c_ulp::c_ulp(void)
: ulptable(NULL)
{
}

void c_ulp::init(void)
{
  ulptable = XUPCAST(u_lp, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(sizeof(u_lp) * unsignedlong(gdat.entries)));
}

u_lp* c_ulp::getadr(ui32 uln) const { return &ulptable[uln]; }

unk* c_ulp::getp(ui32 uln) const { return ulptable[uln].xp_00; }
void c_ulp::setp(ui32 uln, unk* xp) { ulptable[uln].xp_00 = xp; }
void c_ulp::setl(ui32 uln, i32 nl) { ulptable[uln].l_00 = nl | 0x80000000; }
bool c_ulp::islen(ui32 uln) const { return (ulptable[uln].l_00 & 0x80000000) != 0; }

// TODO: neg. offset!
i32 c_ulp::DM2_QUERY_GDAT_RAW_DATA_LENGTH(t_dbidx dbidx)
{
  u_lp* u85p = getadr(unsignedlong(dbidx));
  if (u85p->l_00 & lcon(0x80000000))
    return u85p->l_00; // MSB still set
  else
  {
    i16* wp = XUPCAST(i16, u85p->xp_00);
    return unsignedlong(*(wp - 1)); // high word zero then TODO: neg. offset
  }
}

// change a pointer-entry to a length
// (assumes that the index points to a valid pointer-entry)
void c_ulp::R_2D87A(i16 index)
{
  i32 lv = DM2_QUERY_GDAT_RAW_DATA_LENGTH((t_dbidx)index);
  i8* bptr = getp(index) + (lv + 1 & lcon(0xfffffffe)) + 1; // points to MSB
  i32* pl = UPCAST(i32, bptr - 3); // move back to aligned length
  *pl |= lcon(0x80000000); // mark as length
  setl(index, lv); // ...and in the table too
}

// ########################################

// was setlatelong
void s_hex6::splitlong(i32 nl)
{
  b_02 = nl;
  b_03 = nl >> 8;
  b_04 = nl >> 16;
  b_05 = nl >> 24;
}

// ########################################

c_dballochandler dm2_dballochandler;

const t_text c_dballochandler::tidspfg[8] = { 'T', 'I', 'D', 'S', 'P', 'F', 'G', '\0' };

void c_dballochandler::init(void)
{
  int i = 0;
  c_cpx_linklist::init();

  gfxalloc_done = false;
  cpx_available = 0;
  filebuffer = NULL;
  malloce = NULL;
  mallocf = NULL;
  mallocg = NULL;
  malloch = NULL;
  malloci = 0;
  bigpool_endofunpreservedfree = NULL;
  mallock = 0;
  num_gfx_tableentries = 0;
  mallocm = 0;
  gfx_table = NULL;
  w_table1 = NULL;
  malloc_bmpidx = 0;
  malloco = 0;
  preserved_table_entries = 0;
  preservedgfx_idxtable = NULL;
  preservedgfx_table = NULL;
  preserved_bigpool = 0x3cca; // !
  w_table2 = NULL;
  mallocw = false;
  v1d70d7 = true; // !
  v1e13f8 = 0;
  v1e13fa = 0;
  for (i = 0; i < sizeof(v1e13fe); i++) v1e13fe[i] = 0;
  dbidx = 0;
  v1d6783 = 0;
  v1d6773 = 0xffff; // !
  v1e0abe = 0;
  ptr1e09a8 = NULL;
  v1e09ae = 0;
  v1e09b0 = 0;
  for (i = 0; i < sizeof(v1e09b2) / sizeof(i16); i++) v1e09b2[i] = 0;
  for (i = 0; i < sizeof(v1e09c0); i++) v1e09c0[i] = 0;
  v1d6793 = 0;
  ptr1e0a5c = NULL;
  for (i = 0; i < sizeof(v1d6787); i++) v1d6787[i] = 0;
  v1e09ac = 0;
  v1d6770 = 0x10; // !
  v1dff38 = NULL;
  v1e0ad4 = 0;
  v1e0ad6 = 0;
  v1e0acc = 0;
  v1d6775 = 0x3d090; // ! read-only
  v1e0a74 = 0;
  v1e0a84 = false;
  v1e0a90 = 0;
  v1e0a8c = 0;
  v1e0a0c = 0;
  v1e0200 = false;

  pppw1.wp_00 = NULL;
  pppw1.wp_04 = NULL;
  pppw1.wp_08 = NULL;
  pppw1.w_0c = 0;

  pppw2.wp_00 = NULL;
  pppw2.wp_04 = NULL;
  pppw2.wp_08 = NULL;
  pppw2.w_0c = 0;

  gdattable.w_table1 = NULL;
  gdattable.w_table2 = NULL;
  gdattable.u31p_08 = NULL;
  gdattable.entries = 0;
  gdattable.w_0e = 0;
  gdattable.w_10 = 0;
  gdattable.w_12 = 0;
  gdattable.w_14 = 0;
  for (i = 0; i < sizeof(gdattable.warr_16) / sizeof(i16); i++) gdattable.warr_16[i] = 0;
  for (i = 0; i < sizeof(gdattable.barr_24) / sizeof(i8); i++) gdattable.barr_24[i] = 0;
}

void c_dballochandler::DM2_SETUP_DB_ALLOCATION(void)
{
  init(); // new

  filebuffer = DOWNCAST(unk, DM2_ALLOC_MEMORY_RAM(0x400, MASK_280, FREEPOOL));
  // TODO:
  // looks like parameters are swapped, but that's in fact the read - graphics - filebuffer,
  // which needs a length of 0x400. Interesting mask though...

  c_cpx_linklist::init();

  malloce = NULL;
  mallocf = NULL;
  mallocg = NULL;
  malloch = NULL;

  malloci = 0xffffffff;
}

// was R_2D7EC
void c_dballochandler::DM2_SET_PPPW_ENTRY(s_pppw* eaxp_s46, i16* edxpw, i32 ebxl, i16 ecxw)
{
  eaxp_s46->wp_00 = edxpw;
  eaxp_s46->wp_04 = edxpw;
  eaxp_s46->wp_08 = edxpw;
  if (edxpw != NULL)
  {
    edxpw -= ebxl / 2; // !
    eaxp_s46->wp_04 = edxpw;
  }
  eaxp_s46->w_0c = ecxw;
}

bool c_dballochandler::DM2_dballoc_38c8_0109(unk** xeaxpp, i32* edxpl, i16* ebxpw)
{
  unk** rg6pp = xeaxpp;
  unk* xptr1 = *xeaxpp;
  for (i16 wi = 0; wi < wcon(0x5); wi++)
    v1e13fe[wi] = 0;
  *edxpl = 0;
  *ebxpw = 0;
  if (v1d70d7)
  {
    v1d70d7 = false;
    v1e13f8 = 0;
    v1e13fa = 0;
    for (;;)
    {
      i16 vw4 = unsignedword(*xptr1++);
      if (vw4 == lcon(0xff))
        break;
      if (vw4 == lcon(0xfe))
        v1e13f8++;
    }
    xptr1 = *rg6pp;
  }
  for (;;)
  {
    i16 vw4 = unsignedword(*xptr1++);
    if (vw4 == lcon(0xfe))
    {
      v1e13fa++;
      *rg6pp = xptr1;
      return *xptr1 != bcon(0xff);
    }
    if (vw4 == lcon(0xff))
      return false;
    unk* xptr2 = xptr1 + 1;
    if (vw4 == lcon(0xfd))
    {
      *edxpl = unsignedlong(*xptr1) << bcon(0xb);
      xptr1 = xptr2;
    }
    else if (vw4 == wcon(0xfc))
    {
      *ebxpw = unsignedword(*xptr1) << 4;
      xptr1 = xptr2;
    }
    else
      v1e13fe[vw4] = 1;
  }
}

// belongs to DM2_ALLOC_CPX_SETUP
i32 c_dballochandler::DM2_dballoc_38c8_0224(bool eaxbool, i32 edxl)
{
  c_nreg _RG1;
  c_nreg _RG2;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;
  i32 RG6l;
  i32 vl_00;
  i32 vl_04;
  i32 vl_08;
  i32 vl_0c;
  i32 vl_10;

  vl_00 = RG4L;
  RG1W = ddat.v1e0266;
  vl_10 = RG1L;
  RG1W = ddat.v1e0270;
  vl_08 = RG1L;
  RG1W = ddat.v1e0272;
  vl_04 = RG1L;
  v1e0a84 = true;
  RG6l = lcon(0x7fffffff);
  RG5L = 0;
  //m_451C8:
  for (;;)
  {
    RG1P = DOWNCAST(s_sgwords, ddat.savegamep4);
    RG4L = unsignedlong(byte_at(RG1P, lcon(0x4)));
    if (RG5W < RG4W)
    {
      //m_45137:
      RG4L = RG5L;
      RG4W &= 0x3;
      RG4L = signedlong(RG4W);
      if (RG4L == 0)
      {
        RG2L = unsignedlong(byte_at(RG1P, lcon(0x4)));
        RG1L = (signedlong(v1e13fa) - 1) * RG2L; // BUGFIX soundproblem
        RG4L = signedlong(RG5W) + 1 + RG1L;
        RG1L = 124 * RG4L;
        RG4L += RG1L;
        RG4L *= 4;
        RG2L *= signedlong(v1e13f8);
        RG1L = RG4L;
        RG4L = RG1L % RG2L;
        RG1L /= RG2L;
        DM2_DRAW_DIALOGUE_PROGRESS(RG1L);
      }
      RG2L = signedlong(RG5W);
      DM2_LOAD_NEWMAP(0, 0, RG2L, 0);
      RG2L = v1e0a74;
      if (RG2L < 0 && RG6l > RG2L)
      {
        RG6l = RG2L;
        vl_0c = RG5L;
      }
      RG1L = v1e0a8c - vl_00;
      if (RG1L < RG6l)
      {
        RG6l = RG1L;
        vl_0c = RG5L;
      }
      if (RG6l >= 0 || eaxbool)
      {
        if (v1e13fe[0] == 0)
        {
          RG5L++;
          continue;
        }
      }
    }
    break;
  }
  //m_451DB:
  if (RG6l > ddat.v1d70c5)
  {
    ddat.v1d70c5 = RG6l;
    RG1L = vl_0c;
    ddat.v1d70c9 = RG1W;
  }
  if (eaxbool)
  {
    if (RG6l < 0)
    {
      ddat.v1d70c5 = RG6l;
      RG1L = vl_0c;
      ddat.v1d70c9 = RG1W;
      RG1L = DM2_0aaf_02f8(2, 0) & lcon(0xff);
      DM2_0aaf_0067(RG1L);
      DM2_PREPARE_EXIT();
    }
  }
  v1e0a84 = false;
  RG1L = vl_10;
  ddat.v1e0266 = RG1W;
  RG1L = vl_08;
  ddat.v1e0270 = RG1W;
  RG1L = vl_04;
  ddat.v1e0272 = RG1W;
  if (RG6l >= 0)
  {
    if (v1e13fe[2] != 0)
      DM2_0aaf_02f8(lcon(0xe), lcon(0x20));
    RG1L = lcon(0x1f4);
    DM2_DRAW_DIALOGUE_PROGRESS(RG1L);
  }
  RG1Blo = RG6l >= 0 ? 1 : 0;
  RG1L = unsignedlong(RG1Blo);
  return RG1L;
}

// was SKW_3e74_1330
void c_dballochandler::DM2_ALLOC_CPX_SETUP(unk* xeaxp)
{
  void* p_seof = secondpool_endoffree;
  i32 sp_ava = secondpool_available;
  void* p_eof = bigpool_endoffree;
  i32 ava = bigpool;
  s_xmalloc* bp_startb = bigpool_start_b;
  DM2_BUP_FREEPOOL();

  i32 rg1;
  i32 vl_04;
  i16 vw_14;
  do
  {
    secondpool_endoffree = p_seof;
    secondpool_available = sp_ava;
    bigpool_endoffree = p_eof;
LOGX(("DM2_ALLOC_CPX_SETUP: BPEOF = %08x changed by p_eof\n", bigpool_endoffree));
    bigpool = ava;
    bigpool_start_b = bp_startb;
    DM2_RESTORE_FREEPOOL();
    bool bo = DM2_dballoc_38c8_0109(&xeaxp, &vl_04, &vw_14);
    if (unsignedlong(vw_14) + lcon(0x200) <= ulcon(0xffff))
      dm2_dballochandler.num_gfx_tableentries = vw_14 + wcon(0x200);
    else
      dm2_dballochandler.num_gfx_tableentries = lcon(0xffff);
    dm2_dballochandler.gfx_table = XUPCAST(s_xmalloc*, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(sizeof(s_xmalloc*) * dm2_dballochandler.num_gfx_tableentries, true));
    dm2_dballochandler.preserved_table_entries = lcon(0x100);
    dm2_dballochandler.w_table1 = XUPCAST(t_dbidx, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(sizeof(t_dbidx) * dm2_dballochandler.preserved_table_entries));
    DM2_FILL_I16TABLE(CHGCAST(i16, dm2_dballochandler.w_table1), -1, dm2_dballochandler.preserved_table_entries);
    dm2_dballochandler.preservedgfx_idxtable = XUPCAST(t_dbidx, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(sizeof(i16) * dm2_dballochandler.preserved_table_entries));
    dm2_dballochandler.preservedgfx_table = XUPCAST(ui32, dm2_dballochandler.DM2_ALLOC_FREEPOOL_MEMORY(sizeof(ui32) * dm2_dballochandler.preserved_table_entries, true));
    dm2_dballochandler.bigpool_endofunpreservedfree = VCAST(DOWNCAST(void, bigpool_endoffree) - dm2_dballochandler.preserved_bigpool);
    DM2_SET_PPPW_ENTRY(&dm2_dballochandler.pppw1, CHGCAST(i16, dm2_dballochandler.bigpool_endofunpreservedfree), 0, bigpool_mode);
    if (secondpool_available < lcon(0xc))
      DM2_SET_PPPW_ENTRY(&dm2_dballochandler.pppw2, CHGCAST(i16, secondpool_endoffree), secondpool_available, secondpool_mode);
    else
      DM2_SET_PPPW_ENTRY(&dm2_dballochandler.pppw2, NULL, 0, secondpool_mode);
    bigpool = dm2_dballochandler.preserved_bigpool;
    dm2_dballochandler.cpx_available = XDOWNCAST(void, dm2_dballochandler.bigpool_endofunpreservedfree) - XDOWNCAST(s_xmalloc, bigpool_start_b); // BUGFIX inner Phase P1
    rg1 = DM2_dballoc_38c8_0224(!bo, vl_04);
  } while (rg1 == 0);
  dm2_dballochandler.gfxalloc_done = true;
}

void c_dballochandler::DM2_dballoc_3e74_5561(t_dbidx dbidx)
{
  i16 vw_00;
  if (malloc_bmpidx == NODATA || dbidx < malloc_bmpidx)
    malloc_bmpidx = dbidx;
  if (!DM2_FIND_IN_PRESERVED_GFX(preservedgfx_table[dbidx], OO vw_00))
    return;
  preservedgfx_table[dbidx] = 0;
  DM2_COPY_MEMORY(DOWNCAST(i16, preservedgfx_idxtable + vw_00), DOWNCAST(i16, preservedgfx_idxtable + vw_00 + 1), sizeof(i16) * (--malloco - vw_00));
}

// was SKW_3e74_585a
void c_dballochandler::DM2_ALLOCATION1(t_dbidx dbidx, bool edxbool)
{
  t_dbidx wv = DM2_ALLOC_CPX_GET_WTABLE_ENTRY(dbidx | 0x8000);
  if (wv != -1)
    DM2_ALLOCATION7(wv, edxbool);
  else
    DM2_dballoc_3e74_5561(dbidx);
}

// some BUGFIX here (doesn't help with the window-problem)
// was R_2F41B
void c_dballochandler::DM2_ALLOCATION2(t_dbidx dbidx)
{
  s_xmalloc* xmptr = gfx_table[dbidx];
  bool pass = false;

  if (xmptr->dbidx0 != NODATA)
  {
    t_dbidx prev_idx = xmptr->prev_idx;
    t_dbidx next_idx = xmptr->next_idx;
    s_xmalloc* xmptr41;

    if (prev_idx != NODATA) // BUGFIX 2017
    {
      xmptr41 = gfx_table[prev_idx];
      xmptr41->next_idx = next_idx;
      if (next_idx != NODATA)
      {
        xmptr41 = gfx_table[next_idx];
        xmptr41->prev_idx = prev_idx;
      }
      else
      {
        if (xmptr != mallocf)
        {
          if (xmptr == malloce)
            malloce = NULL;
        }
        else
          mallocf = NULL;
        mallocg = xmptr41;
      }
    }
    else if (prev_idx != next_idx)
    {
      xmptr41 = gfx_table[next_idx];
      malloch = xmptr41;
      xmptr41->prev_idx = prev_idx;
    }
    else
    {
      malloce =
      mallocf =
      mallocg =
      malloch = NULL;
      pass = true;
    }

    if (!pass)
    {
      if (xmptr != malloce)
      {
        if (xmptr == mallocf)
          mallocf = xmptr41;
      }
      else if (xmptr41 != mallocf)
        malloce = xmptr41;
      else
        malloce = NULL;
    }
  }
  xmptr->prev_idx = xmptr->next_idx = xmptr->dbidx0 = NODATA;
}

// was SKW_3e74_28de
void c_dballochandler::DM2_ALLOCATION3(t_dbidx dbidx)
{
  s_xmalloc* xmptr4; // TODO: with negative offsets

  t_dbidx bmpidx2 = DM2_ALLOC_CPX_GET_WTABLE_ENTRY(dbidx);
  if (bmpidx2 == NODATA)
    return;
  if (dbidx & 0x8000)
  {
    t_dbidx bmpidx3 = dbidx & wcon(0x7fff);
    w_table1[bmpidx3] = NODATA;
    if (!mallocw)
      DM2_dballoc_3e74_5561(bmpidx3);
  }
  else
    w_table2[dbidx] = NODATA;

  DM2_ALLOCATION2(bmpidx2);
  s_xmalloc** xmpp1 = &gfx_table[bmpidx2];
  xmptr4 = *xmpp1;
  *xmpp1 = NULL;
  mallock--;
  if (bmpidx2 < mallocm || mallocm == NODATA)
    mallocm = bmpidx2;

  i32 vl22 = -xmptr4->l_00;
  cpx_available += vl22;

  // TODO: at this point ptr4 may change the type
  // TODO: here are negative offsets and variable-sized accesses

  if (xmptr4 != get_bigpool_start_b())
  {
    i32 vl31 = *(CHGCAST(i32, xmptr4) - 1); // the long before the struct
    if (vl31 > 0)
    {
      xmptr4 = UPCAST(s_xmalloc, DOWNCAST(s_xmalloc, xmptr4) - vl31);
      DM2_ALLOC_CPX_UNLINK_NODE(&xmptr4->lpp);
      vl22 += vl31;
    }
  }

  s_lpp* lpptr11 = UPCAST(s_lpp, DOWNCAST(s_xmalloc, xmptr4) + vl22);
  if (lpptr11 == dm2_dballochandler.get_bigpool_start_a_lppadr())
  {
    dm2_dballochandler.set_bigpool_start_a(xmptr4);
    return;
  }
  i32 vl32 = lpptr11->amount;
  if (vl32 > 0)
  {
    DM2_ALLOC_CPX_UNLINK_NODE(lpptr11);
    vl22 += vl32;
  }
  xmptr4->l_00 = vl22;
  *CHGCAST(i32, DOWNCAST(s_xmalloc, xmptr4) + vl22 - lcon(0x4)) = vl22; // TODO: another ugly access
  DM2_ALLOC_CPX_LINK_NODE(&xmptr4->lpp);
}

// was SKW_ 3e74_0c62
t_dbidx c_dballochandler::DM2_ALLOC_CPX_GET_WTABLE_ENTRY(t_dbidx dbidx)
{
  t_dbidx* table;

  if (dbidx & 0x8000)
  {
    dbidx &= 0x7fff;
    table = w_table1;
  }
  else
    table = w_table2;
  return table[dbidx];
}

t_dbidx c_dballochandler::DM2_dballoc_2E52C(void)
{
  t_dbidx dbidx = malloc_bmpidx;
  if (dbidx < 0)
    return dbidx;
  i16 vw4 = malloco + 1;
  malloco = vw4;
  if (mkuw(vw4) < mkuw(preserved_table_entries))
  {
    while (w_table1[++malloc_bmpidx] != NODATA);
    return dbidx;
  }
  malloc_bmpidx = NODATA;
  return dbidx;
}

// this one checked!
// was SKW_3e74_54a1, DM2_ALLOCATION4
t_dbidx c_dballochandler::DM2_ADD_TO_PRESERVED_GFX(ui32 eaxul, i16 index)
{
  if (malloco == preserved_table_entries)
  {
    s_xmalloc* xmptr1 = malloch;
    while (!(xmptr1->dbidx3 & 0x8000))
      xmptr1 = gfx_table[xmptr1->next_idx];
    DM2_ALLOCATION3(xmptr1->dbidx3);
    DM2_FIND_IN_PRESERVED_GFX(eaxul, OO index);
  }
  t_dbidx dbidx = DM2_dballoc_2E52C();
  i16 wamount = 2 * (malloco - index - 1);
  t_dbidx* dbidxptr_src = preservedgfx_idxtable + index;
  DM2_COPY_MEMORY(DOWNCAST(t_dbidx, dbidxptr_src + 1), DOWNCAST(t_dbidx, dbidxptr_src), wamount);
  preservedgfx_idxtable[index] = dbidx;
  preservedgfx_table[dbidx] = eaxul;
  return dbidx;
}

// was SKW_3e74_5420, DM2_ALLOCATION5
bool c_dballochandler::DM2_FIND_IN_PRESERVED_GFX(ui32 eaxul, OO i16& edxw)
{
  i16 lo = -1;
  i16 hi = malloco;
  for (;;)
  {
    i16 mid = (lo + hi) >> 1; // TODO: /2 ends in a wrong result for 0xffff, TODO tell SPX
    if (mid == lo)
    {
      edxw = ++mid;
      return false;
    }

    ui32 vul2 = preservedgfx_table[preservedgfx_idxtable[mid]];
    if (eaxul > vul2)
      lo = mid;
    else if (eaxul == vul2)
    {
      edxw = mid;
      return true;
    }
    else
      hi = mid;
  }
}

// was SKW_3e74_48c9
// TODO HOT: check diese Routine, altes Programm setzt hier das gesuchte dbidx0 auf 2
// wird gerufen in Multiplayer_pic von temp_pic und picst_it
// dann 0b36_00c3 und 3e74_5817
// Die Routine ist korrekt. Die Frage ist also, wieso es trotzdem
// zu der Abweichung kommt.
s_xmalloc* c_dballochandler::DM2_ALLOCATION6(t_dbidx dbidx)
{
  t_dbidx dbidx4;

  s_xmalloc* xmptr2 = gfx_table[dbidx];
  t_dbidx dbidx6 = xmptr2->dbidx0;
  if (dbidx6 == wcon(0xffff) || dbidx6 == wcon(0xfffe))
    return xmptr2;
  s_xmalloc* xmptr7 = xmptr2;
  if (dbidx6 == 0)
  {
    dbidx4 = xmptr2->next_idx;
    if (dbidx4 != NODATA)
    {
      s_xmalloc* xmptr1 = gfx_table[dbidx4];
      t_dbidx dbidx3 = xmptr2->prev_idx;
      if (dbidx3 != NODATA)
      {
        xmptr2 = gfx_table[dbidx3];
        xmptr2->next_idx = dbidx4;
      }
      else
        malloch = xmptr1;
      xmptr1->prev_idx = dbidx3;
      xmptr7->dbidx0 = NODATA;
      DM2_ALLOCATION7(dbidx, false);
      return xmptr7;
    }
    if (xmptr2 == mallocf)
      malloce = NULL;
    else
      malloce = xmptr2;
    xmptr2->dbidx0 = 1;
    return xmptr2;
  }
  if (mkuw(dbidx6) < mkuw(lcon(0xfffffffd)))
    dbidx6++;
  xmptr2->dbidx0 = dbidx6;
  if (xmptr2 == mallocg)
    return xmptr2;

  dbidx4 = xmptr2->next_idx;
  s_xmalloc* xmptr11 = gfx_table[dbidx4];
  if (mkuw(dbidx6) <= mkuw(xmptr11->dbidx0)) // BUGFIX Windowbug mkuw() rechte Seite!
    return xmptr2;

  t_dbidx dbidx3 = xmptr2->prev_idx;
  if (dbidx3 != NODATA)
  {
    if (xmptr2 == malloce)
      malloce = xmptr11;
    xmptr2 = gfx_table[dbidx3];
    xmptr2->next_idx = dbidx4;
  }
  else
    malloce = malloch = xmptr11;
  xmptr11->prev_idx = dbidx3;

  xmptr2 = malloce;
  if (xmptr2 == mallocf)
    malloce = NULL;

  t_dbidx prev_idx;
  do
  {
    xmptr2 = xmptr11;
    prev_idx = dbidx4;
    dbidx4 = xmptr11->next_idx;
    if (dbidx4 == NODATA)
    {
      xmptr11->next_idx = dbidx;
      xmptr7->prev_idx = prev_idx;
      xmptr7->next_idx = NODATA;
      mallocg = xmptr7;
      return xmptr7;
    }
    xmptr11 = gfx_table[dbidx4];
  } while (mkuw(dbidx6) > mkuw(xmptr11->dbidx0));
  xmptr2->next_idx = dbidx;
  xmptr7->prev_idx = prev_idx;
  xmptr7->next_idx = dbidx4;
  xmptr11->prev_idx = dbidx;
  return xmptr7;
}

// BUGFIX in flow (flag1)
// was R_2E68E
void c_dballochandler::DM2_ALLOCATION7(t_dbidx dbidx, bool edxbool)
{
  s_xmalloc* xmptr4 = gfx_table[dbidx];
  s_xmalloc* xmptr2 = xmptr4;

  s_xmalloc* xmptr1;

  bool flag1 = false;
  if (xmptr4->dbidx0 == 0xff)
  {
    if (!edxbool)
    {
      xmptr4->dbidx0 = 1;
      xmptr1 = malloce;
      xmptr2 = mallocf;
      if (xmptr2 != NULL)
        flag1 = true;
    }
    else
    {
      xmptr4->dbidx0 = uwcon(0xfffe);
      xmptr1 = mallocf;
    }
    if (!flag1)
    {
      xmptr2 = mallocg;
      flag1 = true;
    }
  }

  if (!flag1)
  {
    DM2_ALLOCATION6(dbidx);
    return;
  }

  bool flag2 = false;
  if (xmptr1 != NULL)
    flag2 = true;
  else if (mallocg != NULL)
  {
    if (malloce != NULL || mallocf == NULL)
    {
      xmptr2->next_idx = dbidx;
      xmptr4->prev_idx = DM2_ALLOC_CPX_GET_WTABLE_ENTRY(xmptr2->dbidx3);
    }
    else
    {
      xmptr1 = xmptr2;
      flag2 = true;
    }
  }
  else
  {
    xmptr4->prev_idx = NODATA;
    malloch = xmptr4;
  }
  if (flag2)
  {
    t_dbidx next_idx = xmptr1->prev_idx;
    xmptr1->prev_idx = dbidx;
    xmptr4->prev_idx = next_idx;
    if (next_idx == NODATA)
    {
      xmptr4->next_idx = DM2_ALLOC_CPX_GET_WTABLE_ENTRY(xmptr1->dbidx3);
      malloch = xmptr4;
    }
    else
    {
      s_xmalloc* xmptr11 = gfx_table[next_idx];
// new BUG - can crash here after a while, xmptr11 is NULL then
// caught with next_idx == 0x3c - routine bugfixed (flag1). Problem still possible?
if (xmptr11==NULL)
  int stop=1;
      xmptr4->next_idx = xmptr11->next_idx;
      xmptr11->next_idx = dbidx;
    }
  }
  else
  {
    xmptr4->next_idx = NODATA;
    mallocg = xmptr4;
  }
  if (!edxbool)
    malloce = xmptr4;
  else
    mallocf = xmptr4;
}

// was R_2BD3F
void c_dballochandler::DM2_DEALLOC_BIGPOOL_STRUCT_BEFORE(s_malloctail* s78p)
{
  if (!gfxalloc_done && s78p != NULL)
    if ((s78p - 1)->w_04 != 0)
      DM2_DEALLOC_HIBIGPOOL((s78p - 1)->l_00);
    else
      DM2_DEALLOC_LOBIGPOOL((s78p - 1)->l_00);
}

void c_dballochandler::DM2_GUARANTEE_FREE_CPXHEAP_SIZE(i32 amount)
{
  while (amount > cpx_available)
  {
    if (malloch == NULL)
      DM2_RAISE_SYSERR(0x2b);
    DM2_ALLOCATION3(malloch->dbidx3);
  }
}

// was SKW_3e74_2b30, DM2_ALLOCATION8
void c_dballochandler::DM2_INIT_GFX_TABLE(void)
{
  if (c_cpx_linklist::is_empty())
    return;

  s_xmalloc* ptr_src = get_bigpool_start_b();
  s_xmalloc* ptr_dest = ptr_src;
  do
  {
    i32 amount = ptr_src->l_00;
    if (amount <= 0)
    {
      amount = -amount;
      if (ptr_src != ptr_dest)
      {
        gfx_table[DM2_ALLOC_CPX_GET_WTABLE_ENTRY(ptr_src->dbidx3)] = ptr_dest;
        DM2_COPY_MEMORY(DOWNCAST(s_xmalloc, ptr_dest), DOWNCAST(s_xmalloc, ptr_src), amount);
        if (ptr_src == malloch)
          malloch = ptr_dest;
        if (ptr_src == mallocg)
          mallocg = ptr_dest;
        if (ptr_src != malloce)
        {
          if (ptr_src == mallocf)
            mallocf = ptr_dest;
        }
        else
          malloce = ptr_dest;
      }
      ptr_dest = UPCAST(s_xmalloc, DOWNCAST(s_xmalloc, ptr_dest) + amount);
    }
    ptr_src = UPCAST(s_xmalloc, DOWNCAST(s_xmalloc, ptr_src) + amount);
  } while (ptr_src != get_bigpool_start_a());
  set_bigpool_start_a(ptr_dest);
  c_cpx_linklist::init();
}

// was SKW_ALLOC_LOWER_CPXHEAP
s_xmalloc* c_dballochandler::DM2_ALLOC_CPXHEAP_CREATE_POINTER(i32 amount)
{
  DM2_GUARANTEE_FREE_CPXHEAP_SIZE(amount);
  unk* xptr = DOWNCAST(s_xmalloc, get_bigpool_start_a());
  if (DOWNCAST(void, bigpool_endofunpreservedfree) - DOWNCAST(unk, xptr) < amount)
  {
    if (amount <= get_ptrlist()->amount)
      amount = DM2_ALLOC_CPX1(OO xptr, amount);
    else
    {
      DM2_INIT_GFX_TABLE();
      xptr = DOWNCAST(s_xmalloc, get_bigpool_start_a());
      set_bigpool_start_a(XUPCAST(s_xmalloc, xptr + amount));
    }
  }
  else
    set_bigpool_start_a(XUPCAST(s_xmalloc, xptr + amount));

  s_xmalloc* xmptr = XUPCAST(s_xmalloc, xptr);
  cpx_available -= amount;
  xmptr->l_00 = -amount;
  *CHGCAST(i32, DOWNCAST(s_xmalloc, xmptr) + amount - lcon(0x4)) = -amount;
  xmptr->prev_idx = NODATA;
  xmptr->next_idx = NODATA;
  xmptr->dbidx0 = NODATA;
  return xmptr;
}

// was R_2E4B9
t_dbidx c_dballochandler::DM2_ALLOC_CPXHEAP_CREATE_INDEX(void)
{
  t_dbidx ww = mallocm;
  if (ww == NODATA)
  {
    DM2_ALLOCATION3(malloch->dbidx3);
    ww = mallocm;
  }
  if (++mallock < num_gfx_tableentries) // was unsigned comparison
    while (gfx_table[++mallocm] != NULL);
  else
    mallocm = NODATA;
  return ww;
}

s_xmalloc* c_dballochandler::DM2_ALLOC_CPXHEAP_MEM(t_dbidx dbidx, i32 amount)
{
  amount += 22;
  if (IS_ODD(amount))
    amount++;
  s_xmalloc* xmptr = DM2_ALLOC_CPXHEAP_CREATE_POINTER(amount);
  t_dbidx index = DM2_ALLOC_CPXHEAP_CREATE_INDEX();
  w_table1[dbidx] = index;
  gfx_table[index] = xmptr;
  xmptr->dbidx3 = dbidx | 0x8000;
  return xmptr + 1; // so 0x12 bytes ahead
}

// was SKW_3e74_5788
s_xmalloc* c_dballochandler::DM2_ALLOCATION9(t_dbidx dbidx, i32 edxl)
{
  mallocw = true;
  DM2_ALLOCATION3(dbidx | 0x8000);
  mallocw = false;
  return DM2_ALLOC_CPXHEAP_MEM(dbidx, edxl);
}

// was R_2E581
void c_dballochandler::DM2_ALLOCATION10(void)
{
  malloci = timdat.gametick;
  malloce = NULL;
  s_xmalloc* xmptr = mallocf;
  if (xmptr == NULL)
  {
    xmptr = mallocg;
    if (xmptr == NULL)
      return;
  }
  while (xmptr->dbidx0 != 0)
  {
    if (xmptr->dbidx0 <= uwcon(0xFFFD))	// uwcon(0xfffffffd)
      xmptr->dbidx0 = 0;
    t_dbidx prev_idx = xmptr->prev_idx;
    if (prev_idx == NODATA)
      return;
    xmptr = gfx_table[prev_idx];
  }
}

// was SKW_3e74_5673
i32 c_dballochandler::DM2_ALLOCATION11(ui32 eaxul, i32 ebxl, OO t_dbidx& dbidx)
{
  i16 vw_00;

  if (timdat.gametick != malloci)
    DM2_ALLOCATION10();
  if (DM2_FIND_IN_PRESERVED_GFX(eaxul, OO vw_00))
  {
    i16 didx = preservedgfx_idxtable[vw_00];
    dbidx = didx;
    return -DM2_ALLOCATION6(DM2_ALLOC_CPX_GET_WTABLE_ENTRY(didx | 0x8000))->l_00 - 22;
  }
  if (ebxl == 0)
    dbidx = NODATA;
  else
    dbidx = DM2_ADD_TO_PRESERVED_GFX(eaxul, vw_00);
  return 0;
}

void c_dballochandler::DM2_dballoc_2636_03d4(void)
{
  ddat.v1e0988 = 0;

SPX_DEBUG_PRINT("DM2_dballoc_2636_03d4 Start\n");
  for (i16 i = 0; i < 2; i++)
    ddat.vp_1e097c[i] = DM2_ALLOC_FREEPOOL_MEMORY(lcon(0x80));

  // Note: setting of ddat.datafoldername if DRVB_unknown contains a value removed
  // this had maybe smth todo with findfirst and readsectors
  // after all obsolete.

SPX_DEBUG_PRINT("DM2_dballoc_2636_03d4 End\n");
}

bool c_dballochandler::DM2_dballoc_3e74_55f9(ui32 eaxul, OO t_dbidx& dbidx)
{
  i16 vw_00;

  if (timdat.gametick != malloci)
    DM2_ALLOCATION10();
  if (!DM2_FIND_IN_PRESERVED_GFX(eaxul, OO vw_00))
  {
    dbidx = DM2_ADD_TO_PRESERVED_GFX(eaxul, vw_00);
    return false;
  }
  else
  {
    dbidx = preservedgfx_idxtable[vw_00];
    DM2_ALLOCATION6(DM2_ALLOC_CPX_GET_WTABLE_ENTRY(dbidx | 0x8000));
    return true;
  }
}

// was DM2_dballoc_3e74_5817
t_bmp* c_dballochandler::DM2_GET_BMP(t_dbidx bmpid)
{
  return UPCAST(t_bmp, DOWNCAST(s_xmalloc, DM2_ALLOCATION6(/*TAG dbidx*/ DM2_ALLOC_CPX_GET_WTABLE_ENTRY(bmpid | 0x8000)) + 1)); // gets adress of next structentry
}

// was DM2_dballoc_3e74_583a, TODO tell SPX new name
void c_dballochandler::DM2_ALLOCATE_GFX256(t_dbidx dbidx)
{
  dbidx = DM2_ALLOC_CPX_GET_WTABLE_ENTRY(dbidx | 0x8000);
  if (dbidx != NODATA)
    DM2_ALLOCATION2(dbidx);
}

i8 c_dballochandler::DM2_dballoc_3e74_2439(i8 eaxb, i8 edxb)
{
  if (unsignedword(eaxb) > unsignedword(gdattable.entries))
    return 0;
  i32 longrg2 = unsignedlong(gdattable.w_table1[eaxb]);
  i32 longrg3 = (signedlong(gdattable.w_table1[eaxb + 1]) & lcon(0xffff)) - longrg2;
  i32 longrg1 = unsignedlong(edxb);
  if (longrg1 >= longrg3)
    return 0;
  longrg2 += longrg1;
  return gdattable.u31p_08[unsignedlong(gdattable.w_table2[longrg2 + 1]) - 1].b_00;
}

// was DM2_dballoc_3e74_53ea, TODO tell SPX new name
ui32 c_dballochandler::DM2_ALLOC_GENERATE_ID(void)
{
  ui32 ulongrg2;
  i16 vw_00;

  do
  {
    ulongrg2 = unsignedlong(v1d6793++) | ulcon(0xffff0000);
  } while (DM2_FIND_IN_PRESERVED_GFX(ulongrg2, OO vw_00));
  return ulongrg2;
}

t_dbidx c_dballochandler::DM2_dballoc_3e74_5888(void)
{
  t_dbidx dbidx;

  DM2_dballoc_3e74_55f9(DM2_ALLOC_GENERATE_ID(), OO dbidx);
  return dbidx;
}

void c_dballochandler::DM2_dballoc_3e74_58a3(t_dbidx dbidx)
{
  DM2_ALLOCATION1(dbidx, false);
  DM2_ALLOCATION3(dbidx | 0x8000);
}

// was DM2_dballoc_3e74_58bf, TODO tell SPX new name
void c_dballochandler::DM2_ALLOCATE_GFX16(i8 eaxb, i8 edxb, i8 ebxb)
{
  t_dbidx dbidx = (t_dbidx)DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, edxb, 1, ebxb);
  if (dbidx == NODATA || dm2_ulp.islen(dbidx))
    dbidx = (t_dbidx)DM2_QUERY_GDAT_ENTRY_DATA_INDEX(21, lcon(0xfe), 1, lcon(0xfe));
  if (dbidx != NODATA)
  {
    t_dbidx bmpidx2 = DM2_ALLOC_CPX_GET_WTABLE_ENTRY(dbidx);
    if (bmpidx2 != NODATA)
      DM2_ALLOCATION2(bmpidx2);
  }
}

void c_dballochandler::DM2_dballoc_3e74_5992(i8 eaxb, i8 edxb, i8 ebxb)
{
  t_dbidx dbidx = (t_dbidx)DM2_QUERY_GDAT_ENTRY_DATA_INDEX(eaxb, edxb, 1, ebxb);
  if (dbidx == NODATA || dm2_ulp.islen(dbidx))
    dbidx = (t_dbidx)DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0x15), lcon(0xfe), 1, lcon(0xfe));
  if (dbidx != NODATA)
  {
    dbidx = DM2_ALLOC_CPX_GET_WTABLE_ENTRY(dbidx);
    if (dbidx != NODATA)
      DM2_ALLOCATION7(dbidx, false);
  }
}

// stacksize was 0x24
i16 c_dballochandler::DM2_ALLOC_NEW_CREATURE(i16 eaxw, i16 edxw, i16 ebxw, i16 ecxw, i16 argw0)
{
  t_record* rg4p;
  t_record* rg5p;
  i16 rg63w;
  c_aidef* rg7p;
  i16 vw_00;
  i16 vw_04;
  i16 vw_10;

  vw_00 = eaxw;
  bool neg = eaxw < 0;
  if (neg)
  {
    vw_00 &= 0x7fff;
    vw_10 = CUTX16(DM2_ALLOC_NEW_RECORD(14));
    if (vw_10 == -1)
      return vw_10;
  }
  vw_04 = CUTX16(DM2_ALLOC_NEW_RECORD(4));
  if (vw_04 != -1)
  {
    if (ddat.v1d4020 < ddat.v1e08a0)
    {
      //m_20CD1:
      rg4p = rg5p = DM2_GET_ADDRESS_OF_RECORD(mk_record(vw_04));
      t_record tmp = (rg4p[2] & 0xff00) | unsignedword(CUTX8(vw_00)); // set lobyte only
      rg4p[2] = tmp;
      rg7p = DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(CUTX8(vw_00)));
      rg4p[1] = -2;
      rg4p[7] &= wcon(0xf8ff);
      rg4p[7] |= (ebxw & 0x3) << 8;
      rg4p[2] |= wcon(0xff00); // was mov8
      if (neg)
      {
        rg4p++;
        DM2_APPEND_RECORD_TO(mk_record(unsignedlong(vw_10)), rg4p, -1, 0);
      }
      rg63w = CUTX16((unsignedlong(rg7p->w_04) * (DM2_MIN(edxw, 31) + 1)) >> 3);
      rg63w += DM2_RAND16(rg63w / 8 + 1);
      rg5p[3] = rg63w;
      rg5p[5] = 0;
      rg5p[6] &= wcon(0xffe0);
      rg63w = rg5p[6] | (ecxw & lcon(0x1f));
      rg5p[6] = (rg63w & wcon(0xfc1f)) | ((argw0 & 0x1f) << 5);
      rg5p[6] &= 0x3ff;
      rg5p[6] |= (ddat.v1d3248 & 0x3f) << 10;
      rg5p[4] = -1;
      if ((DM2_MOVE_RECORD_TO(unsignedlong(vw_04), lcon(0xfffffffc), 0, signedlong(ecxw), argw0) & lcon(0xffff)) == 1)
        return -1;
      rg5p[4] = 0;
      return vw_04;
    }
  }
  //m_20CB6:
  if (neg)
    DM2_DEALLOC_RECORD(unsignedlong(vw_10));
  return -1;
}

// SPX: Debug only
void _DEBUG_SHOW_BIGPOOL(const char* str)
{
	if (str == NULL)
		LOGX(("_DEBUG_SHOW_BIGPOOL: BPEOF = %08x\n", dm2_dballochandler.bigpool_endoffree));
	else
		LOGX(("_DEBUG_SHOW_BIGPOOL (%s): BPEOF = %08x\n", str, dm2_dballochandler.bigpool_endoffree));
}