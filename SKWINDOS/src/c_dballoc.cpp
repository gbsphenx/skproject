#include "util.h"
#include "startend.h" // THROW...
#include "dm2data.h"
#include "c_alloc.h"
#include "c_dballoc.h"

// TODO: search for CAST for dirty accesses

extern void SKW_RAISE_SYSERR(x16 eaxw);
extern bool SKW_38c8_0224(bool eaxbool, x32 edxl);

c_ulp dm2_ulp;

c_ulp::c_ulp(void)
:  ulptable(NULL)
{
}

void c_ulp::init(void)
{
  ulptable = UPCAST(u_lp, DM2_ALLOC_FREEPOOL_MEMORY(sizeof(u_lp) * unsignedlong(ddata.gdatentries)));
}

// TODO: neg. offset!
x32 c_ulp::SKW_QUERY_GDAT_RAW_DATA_LENGTH(x16 n)
{
fprintf(logfile, "QUERY_GDAT_RAW_DATA_LENGTH: Querying length of raw item %05d => returns ", n); // SPX: add log
  u_lp* p = getadr(unsignedlong(n));
  if (p->l_00 & con(0x80000000))
  {
	 fprintf(logfile, "%6d (%08x) (MSB)\n", p->l_00 & con(0x7FFFFFFF), p->l_00); // SPX: add log
    return p->l_00; // MSB still set
  }
  else
  {
    x16* pw = UPCAST(x16, p->p_00);
	// SPX : if item requested is out of bound we can get a adr = 0000, then it will provide a negative offset which crashes.
	// As within skwin, a protection can be made to return a zero-length for a non valid address.
	if (p->p_00 == 0x00000000 || n > ddata.gdatentries)
	{
	 fprintf(logfile, "%6d (%08x) (fail or out of bound)\n", (x32)(0xFFFFFFFF), (x32)(0xFFFFFFFF)); // SPX: add log
		return (x32)(0xFFFFFFFF);
	}
	 fprintf(logfile, "%6d (%08x)\n", unsignedlong(*(pw - 1)),  unsignedlong(*(pw - 1)) ); // SPX: add log
	// then original code
    return unsignedlong(*(pw - 1)); // high word zero then TODO: neg. offset
  }
}

// change a pointer-entry to a length
// (assumes that the index points to a valid pointer-entry)
void c_ulp::R_2D87A(x16 index)
{
  x32 v = SKW_QUERY_GDAT_RAW_DATA_LENGTH(index);
  i8* px = getp(index) + ((v + 1) & con(0xfffffffe)) + 1; // points to MSB
  x32* pl = UPCAST(x32, px - 3); // move back to aligned length
  *pl |= con(0x80000000); // mark as length
  setl(index, v); // ...and in the table too
}

c_dballochandler dm2_dballochandler;

const x8 c_dballochandler::tidspfg[8] = { 'T','I','D','S','P','F','G', NULLBYTE };

void c_dballochandler::init(void)
{
  int i = 0;
  gfxalloc_done = false;
  cpx_available = 0;
  filebuffer = NULL;
  ptrlist = NULL;
  malloce = NULL;
  mallocf = NULL;
  mallocg = NULL;
  malloch = NULL;
  malloci = 0;
  bigpool_endofunpreservedfree = NULL;
  mallock = 0;
  num_s_tableentries = 0;
  mallocm = 0;
  s_table = NULL;
  w_table1 = NULL;
  mallocn = 0;
  malloco = 0;
  preserved_table_entries = 0;
  w_table3 = NULL;
  l_table3 = NULL;
  preserved_bigpool = 0x3cca; // !
  w_table2 = NULL;
  mallocw = false;
  v1d70d7 = true; // !
  v1e13f8 = 0;
  v1e13fa = 0;
  for (i=0; i<sizeof(v1e13fe); i++) v1e13fe[i] = 0;
  v1d6781 = 0;
  v1d6783 = 0;
  v1d6773 = 0xffff; // !
  v1e0abe = 0;
  ptr1e09a8 = NULL;
  v1e09ae = 0;
  v1e09b0 = 0;
  for (i=0; i<sizeof(v1e09b2)/sizeof(x16); i++) v1e09b2[i] = 0;
  for (i=0; i<sizeof(v1e09c0); i++) v1e09c0[i] = 0;
  v1d6793 = 0;
  ptr1e0a5c = NULL;
  for (i=0; i<sizeof(v1d6787); i++) v1d6787[i] = 0;
  v1e09ac = 0;
  v1d6770 = 0x10; // !
  v1dff38 = NULL;
  v1e149c = NULL;
  v1e14a0 = NULL;
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

  pppw1.pw0 = NULL;
  pppw1.pw4 = NULL;
  pppw1.pw8 = NULL;
  pppw1.wc = 0;

  pppw2.pw0 = NULL;
  pppw2.pw4 = NULL;
  pppw2.pw8 = NULL;
  pppw2.wc = 0;

  gdattable.w_table4 = NULL;
  gdattable.w_table5 = NULL;
  gdattable.l_table4 = NULL;
  gdattable.w_0c = 0;
  gdattable.w_0e = 0;
  gdattable.w_10 = 0;
  gdattable.w_12 = 0;
  gdattable.w_14 = 0;
  for (i=0; i<sizeof(gdattable.warr_16)/sizeof(x16); i++) gdattable.warr_16[i] = 0;
  for (i=0; i<sizeof(gdattable.barr_24)/sizeof(x8); i++) gdattable.barr_24[i] = 0;
}

void DM2_SETUP_DB_ALLOCATION(void)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("DM2_SETUP_DB_ALLOCATION");
  dm2_dballochandler.init(); // new

  dm2_dballochandler.filebuffer = UPCAST(i8, DM2_ALLOC_MEMORY_RAM(0x400, MASK_280, FREEPOOL));
  // TODO:
  // looks like parameters are swapped, but that's in fact the read - graphics - filebuffer,
  // which needs a length of 0x400. Interesting mask though...

  dm2_dballochandler.ptrlist = NULL;

  dm2_dballochandler.malloce = NULL;
  dm2_dballochandler.mallocf = NULL;
  dm2_dballochandler.mallocg = NULL;
  dm2_dballochandler.malloch = NULL;

  dm2_dballochandler.malloci = 0xffffffff;
SPX_DEBUG_POP;
}

// old name: R_2D7EC
static void SKW_SET_PPPW_ENTRY(s_pppw* eaxp, x32 ebxl, x16 ecxw, x16* edxwp)
{
  eaxp->pw0 = edxwp;
  eaxp->pw4 = edxwp;
  eaxp->pw8 = edxwp;
  if (edxwp != NULL)
  {
    edxwp -= ebxl / 2; // !
    eaxp->pw4 = edxwp;
  }
  eaxp->wc = ecxw;
}

// old name: SKW_3e74_07b2
static void SKW_BUP_FREEPOOL(void)
{
  c_freepool* ptr = dm2_allochandler.freepoollist_end;
  while (ptr != NULL)
  {
    if (!ptr->tag)
    {
      ptr->eof_bup = ptr->endoffree;
      ptr->ava_bup = ptr->available;
    }
    ptr = ptr->prev;
  }
}

// old name: SKW_3e74_0820
static void DM2_RESTORE_FREEPOOL(void)
{
  c_freepool* ptr = dm2_allochandler.freepoollist_end;
  while (ptr != NULL)
  {
    if (!ptr->tag)
    {
      ptr->endoffree = ptr->eof_bup;
      ptr->available = ptr->ava_bup;
    }
    ptr = ptr->prev;
  }
}


static bool SKW_38c8_0109(const x8** eaxpp, x16* ebxpw, x32* edxpl)
{
  const x8** rg6pp = eaxpp;
  const x8* ptr1 = *eaxpp;
  for (x16 i = 0; i < 5; i++)
    dm2_dballochandler.v1e13fe[i] = 0;
  *edxpl = con(0x0);
  *ebxpw = con(0x0);
  if (dm2_dballochandler.v1d70d7)
  {
    dm2_dballochandler.v1d70d7 = false;
    dm2_dballochandler.v1e13f8 = 0;
    dm2_dballochandler.v1e13fa = 0;
    for(;;)
    {
      i16 v4 = unsignedword(*ptr1++);
      if (v4 == con(0xff))
        break;
      if (v4 == con(0xfe))
        dm2_dballochandler.v1e13f8++;
    }
    ptr1 = *rg6pp;
  }
  for(;;)
  {
    i16 v4 = unsignedword(*ptr1++);
    if (v4 == con(0xfe))
    {
      dm2_dballochandler.v1e13fa++;
      *rg6pp = ptr1;
      return (*ptr1 != con(0xffffffff));
    }
    if (v4 == con(0xff))
      return false;
    const x8* ptr2 = ptr1 + con(0x1);
    if (v4 == con(0xfd))
    {
      *edxpl = unsignedlong(*ptr1) << con(0xb);
      ptr1 = ptr2;
    }
    else if (v4 == con(0xfc))
    {
      *ebxpw = unsignedword(*ptr1) << 4;
      ptr1 = ptr2;
    }
    else
      dm2_dballochandler.v1e13fe[v4] = 1;
  }
}


// old name: SKW_3e74_1330
void SKW_ALLOC_CPX_SETUP(const x8* eaxp)
{
  void* p_seof = dm2_allochandler.secondpool_endoffree;
  x32 sp_ava = dm2_allochandler.secondpool_available;
  void* p_eof = dm2_allochandler.bigpool_endoffree;
  x32 ava = dm2_allochandler.bigpool_available;
  s_xmalloc* bp_startb = dm2_allochandler.bigpool_start_b;
  SKW_BUP_FREEPOOL();

  x32 vl_04;
  x16 vw_14;
  bool bo;
  bool rg1;
  do
  {
    dm2_allochandler.secondpool_endoffree = p_seof;
    dm2_allochandler.secondpool_available = sp_ava;
    dm2_allochandler.bigpool_endoffree = p_eof;
    dm2_allochandler.bigpool_available = ava;
    dm2_allochandler.bigpool_start_b = bp_startb;
    DM2_RESTORE_FREEPOOL();
    bo = SKW_38c8_0109(&eaxp, &vw_14, &vl_04);
    if ((unsignedlong(vw_14) + con(0x200)) <= con(0xffff))
      dm2_dballochandler.num_s_tableentries = vw_14 + con(0x200);
    else
      dm2_dballochandler.num_s_tableentries = con(0xffff);
    dm2_dballochandler.s_table = UPCAST(s_xmalloc*, DM2_ALLOC_FREEPOOL_MEMORY(sizeof(s_xmalloc*) * dm2_dballochandler.num_s_tableentries, true));
    dm2_dballochandler.preserved_table_entries = con(0x100);
    dm2_dballochandler.w_table1 = UPCAST(x16, DM2_ALLOC_FREEPOOL_MEMORY(sizeof(x16) * dm2_dballochandler.preserved_table_entries));
    DM2_FILL_I16TABLE(dm2_dballochandler.w_table1, con(0xffffffff), dm2_dballochandler.preserved_table_entries);
    dm2_dballochandler.w_table3 = UPCAST(x16, DM2_ALLOC_FREEPOOL_MEMORY(sizeof(x16) * dm2_dballochandler.preserved_table_entries));
    dm2_dballochandler.l_table3 = UPCAST(ui32, DM2_ALLOC_FREEPOOL_MEMORY(sizeof(ui32) * dm2_dballochandler.preserved_table_entries, true));
    dm2_dballochandler.bigpool_endofunpreservedfree = VCAST(DOWNCAST(void, dm2_allochandler.bigpool_endoffree) - dm2_dballochandler.preserved_bigpool);
    SKW_SET_PPPW_ENTRY(&dm2_dballochandler.pppw1, con(0x0), dm2_allochandler.bigpool_mode, UPCAST(x16, dm2_dballochandler.bigpool_endofunpreservedfree));
    if (dm2_allochandler.secondpool_available < con(0xc))
      SKW_SET_PPPW_ENTRY(&dm2_dballochandler.pppw2, dm2_allochandler.secondpool_available, dm2_allochandler.secondpool_mode, UPCAST(x16, dm2_allochandler.secondpool_endoffree));
    else
      SKW_SET_PPPW_ENTRY(&dm2_dballochandler.pppw2, con(0x0), dm2_allochandler.secondpool_mode, NULL);
    dm2_allochandler.bigpool_available = dm2_dballochandler.preserved_bigpool;
    dm2_dballochandler.cpx_available = UPCAST(xxx, dm2_dballochandler.bigpool_endofunpreservedfree) - UPCAST(xxx, dm2_allochandler.bigpool_start_b); // BUGFIX inner Phase P1
    rg1 = SKW_38c8_0224(!bo, vl_04);
  } while (!rg1);
  dm2_dballochandler.gfxalloc_done = true;
}

static void SKW_3e74_5561(x16 eaxw)
{
  x16 vw_00;
  if (dm2_dballochandler.mallocn == con(0xffffffff) || eaxw < dm2_dballochandler.mallocn)
    dm2_dballochandler.mallocn = eaxw;
  if (!SKW_3e74_5420(dm2_dballochandler.l_table3[eaxw], &vw_00)) // BUGFIX late Phase P1
    return;
  dm2_dballochandler.l_table3[eaxw] = con(0x0);
  SKW_COPY_MEMORY(DOWNCAST(x16, dm2_dballochandler.w_table3 + vw_00 + 1), sizeof(x16) * (--dm2_dballochandler.malloco - vw_00), DOWNCAST(x16, dm2_dballochandler.w_table3 + vw_00));
}

void SKW_3e74_585a(x16 eaxw, bool edxbool)
{
  x16 v = DM2_ALLOC_CPX_GET_WTABLE_ENTRY(eaxw | con(0x8000));
  if (v != con(0xffffffff))
    R_2E68E(v, edxbool);
  else
    SKW_3e74_5561(eaxw);
}

// BUGFIX 4/2014 function-structure
void R_2F41B(x16 eaxw)
{
  s_xmalloc* ptr = dm2_dballochandler.s_table[eaxw];
  bool pass = false;
  if (ptr->w4 != ucon16(0xffffffff))
  {
    x16 v3 = ptr->w6;
    x16 v2 = ptr->w8;
    s_xmalloc* ptr41;

    if (v3 != con(0xffffffff))
    {
      ptr41 = dm2_dballochandler.s_table[v3];
      ptr41->w8 = v2;
      if (v2 != con(0xffffffff))
      {
        ptr41 = dm2_dballochandler.s_table[v2];
        ptr41->w6 = v3;
      }
      else if (ptr != dm2_dballochandler.mallocf)
      {
        if (ptr == dm2_dballochandler.malloce)
          dm2_dballochandler.malloce = NULL;
        dm2_dballochandler.mallocg = ptr41;
      }
      else
      {
        dm2_dballochandler.mallocf = NULL;
        dm2_dballochandler.mallocg = ptr41;
      }
    }
    else if (v2 != v3)
    {
      ptr41 = dm2_dballochandler.s_table[v2];
      dm2_dballochandler.malloch = ptr41;
      ptr41->w6 = v3;
    }
    else
    {
      dm2_dballochandler.malloce = NULL;
      dm2_dballochandler.mallocf = NULL;
      dm2_dballochandler.mallocg = NULL;
      dm2_dballochandler.malloch = NULL;
      pass = true;
    }

    if (!pass)
    {
      if (ptr != dm2_dballochandler.malloce)
      {
        if (ptr == dm2_dballochandler.mallocf)
          dm2_dballochandler.mallocf = ptr41;
      }
      else if (ptr41 != dm2_dballochandler.mallocf)
        dm2_dballochandler.malloce = ptr41;
      else
        dm2_dballochandler.malloce = NULL;
    }
  }
  ptr->w8 = con(0xffffffff);
  ptr->w6 = con(0xffffffff);
  ptr->w4 = ucon16(0xffffffff);
}

// old name: SKW_3e74_0c8c
static void DM2_ALLOC_CPX_UNLINK_NODE(s_lpp* eaxp)
{
  s_lpp* prev = eaxp->prev;
  s_lpp* next = eaxp->next;

  if (prev != NULL)
  {
    prev->next = next;
    if (next != NULL)
      next->prev = prev;
    return;
  }
  else if (next != NULL)
  {
    dm2_dballochandler.ptrlist = next;
    next->prev = prev;
  }
  else
    dm2_dballochandler.ptrlist = prev;
}

// old name: SKW_3e74_0d32
static void DM2_ALLOC_CPX_LINK_NODE(s_lpp* ptr)
{
  s_lpp* liststart = dm2_dballochandler.ptrlist;
  if (liststart == NULL)
  {
    dm2_dballochandler.ptrlist = ptr;
    ptr->next = liststart;
    ptr->prev = liststart;
    return;
  }
  x32 amount = ptr->amount;
  if (amount >= liststart->amount)
  {
    dm2_dballochandler.ptrlist = ptr;
    ptr->prev = NULL;
    ptr->next = liststart;
    liststart->prev = ptr;
    return;
  }

  s_lpp* ptr1;
  while ((ptr1 = liststart->next) != NULL)
  {
    if (amount >= ptr1->amount)
    {
      ptr1->prev = ptr;
      liststart->next = ptr;
      ptr->prev = liststart;
      ptr->next = ptr1;
      return;
    }
    liststart = ptr1;
  }

  liststart->next = ptr;
  ptr->prev = liststart;
  ptr->next = NULL;
}

void SKW_3e74_28de(x16 eaxw)
{
  s_xmalloc* ptr4; // TODO: with negative offsets

  x16 v3 = eaxw;
  x16 v21 = DM2_ALLOC_CPX_GET_WTABLE_ENTRY(v3);
  if (v21 == con(0xffffffff))
    return;
  if (eaxw & con(0x8000))
  {
    x16 v4 = eaxw & con(0x7fff);
    dm2_dballochandler.w_table1[v4] = con(0xffffffff);
    if (!dm2_dballochandler.mallocw)
      SKW_3e74_5561(v4);
  }
  else
    dm2_dballochandler.w_table2[v3] = con(0xffffffff);

  R_2F41B(v21);
  s_xmalloc** ptr1 = &dm2_dballochandler.s_table[v21];
  ptr4 = *ptr1;
  *ptr1 = NULL;
  dm2_dballochandler.mallock--;
  if (UI16CAST(v21) < UI16CAST(dm2_dballochandler.mallocm) || dm2_dballochandler.mallocm == con(0xffffffff))
    dm2_dballochandler.mallocm = v21;

  x32 v22 = -ptr4->l0;
  dm2_dballochandler.cpx_available += v22;

  // TODO: at this point ptr4 may change the type
  // TODO: here are negative offsets and variable-sized accesses

  if (ptr4 != dm2_allochandler.bigpool_start_b)
  {
    x32 v31 = *(P32CAST(ptr4)-1); // the long before the struct
    if (v31 > con(0x0))
    {
      ptr4 = UPCAST(s_xmalloc, DOWNCAST(s_xmalloc, ptr4) - v31);
      DM2_ALLOC_CPX_UNLINK_NODE(UPCAST(s_lpp, ptr4));
      v22 += v31;
    }
  }

  s_lpp* ptr11 = UPCAST(s_lpp, DOWNCAST(s_xmalloc, ptr4) + v22);
  if (ptr11 == UPCAST(s_lpp, dm2_allochandler.bigpool_start_a))
  {
    dm2_allochandler.bigpool_start_a = ptr4;
    return;
  }
  x32 v32 = ptr11->amount;
  if (v32 > con(0x0))
  {
    DM2_ALLOC_CPX_UNLINK_NODE(ptr11);
    v22 += v32;
  }
  ptr4->l0 = v22;
  *P32CAST(DOWNCAST(s_xmalloc, ptr4) + v22 - con(0x4)) = v22; // TODO: another ugly access
  DM2_ALLOC_CPX_LINK_NODE(UPCAST(s_lpp, ptr4));
}

// old name: SKW_ 3e74_0c62
x16 DM2_ALLOC_CPX_GET_WTABLE_ENTRY(x16 eaxw)
{
  x16* table;

  if ((eaxw & con(0x8000)) == con(0x0))
    table = dm2_dballochandler.w_table2;
  else
  {
    eaxw &= con(0x7fff);
    table = dm2_dballochandler.w_table1;
  }
  return table[eaxw];
}

static x16 R_2E52C(void)
{
  x16 v2 = dm2_dballochandler.mallocn;
  if (v2 < con(0x0))
    return v2;
  x16 v4 = dm2_dballochandler.malloco + 1;
  dm2_dballochandler.malloco = v4;
  if (UI16CAST(v4) < UI16CAST(dm2_dballochandler.preserved_table_entries))
  {
    while (dm2_dballochandler.w_table1[++dm2_dballochandler.mallocn] != con(0xffffffff));
    return v2;
  }
  dm2_dballochandler.mallocn = con(0xffffffff);
  return v2;
}

x16 SKW_3e74_54a1(ui32 eaxl, x16 index)
{
  if (dm2_dballochandler.malloco == dm2_dballochandler.preserved_table_entries)
  {
    s_xmalloc* ptr1 = dm2_dballochandler.malloch;
    while ((ptr1->wa & con(0x8000)) == con(0x0))
      ptr1 = dm2_dballochandler.s_table[ptr1->w8];
    SKW_3e74_28de(ptr1->wa);
    SKW_3e74_5420(eaxl, &index);
  }
  x16 v3 = R_2E52C();
  x16 amount = 2 * (dm2_dballochandler.malloco - index - 1);
  x16* ptr_src = dm2_dballochandler.w_table3 + index;
  SKW_COPY_MEMORY(DOWNCAST(x16, ptr_src), amount, DOWNCAST(x16, ptr_src + 1));
  dm2_dballochandler.w_table3[index] = v3;
  dm2_dballochandler.l_table3[v3] = eaxl;
  return v3;
}

bool SKW_3e74_5420(ui32 eaxul, x16* edxpw)
{
  x16 v4 = con(0xffffffff);
  x16 v5 = dm2_dballochandler.malloco;
  for(;;)
  {
    x16 v1 = (v4 + v5) >> 1; // TODO: /2 ends in a wrong result for 0xffff
    if (v1 == v4)
    {
      *edxpw = ++v1;
      return false;
    }
    ui32 v2 = dm2_dballochandler.l_table3[dm2_dballochandler.w_table3[v1]];
    if (eaxul >= v2)
    {
      if (eaxul <= v2)
      {
        *edxpw = v1;
        return true;
      }
      v4 = v1;
    }
    else
      v5 = v1;
  }
}

s_xmalloc* SKW_3e74_48c9(x16 eaxw)
{
  x16 v4;

  x16 vw_00 = eaxw;
  s_xmalloc* ptr2 = dm2_dballochandler.s_table[eaxw];
  x16 v6 = ptr2->w4;
  if (v6 == con(0xffffffff) || v6 == con(0xfffffffe))
    return ptr2;

  s_xmalloc* ptr7 = ptr2;
  if (v6 == con(0x0))
  {
    v4 = ptr2->w8;
    if (v4 != con(0xffffffff))
    {
      s_xmalloc* ptr1 = dm2_dballochandler.s_table[v4];
      x16 v31 = ptr2->w6;
      if (v31 != con(0xffffffff))
      {
        ptr2 = dm2_dballochandler.s_table[v31];
        ptr2->w8 = v4;
      }
      else
        dm2_dballochandler.malloch = ptr1;
      ptr1->w6 = v31;
      ptr7->w4 = ucon16(0xffffffff);
      R_2E68E(vw_00, false);
      return ptr7;
    }
    if (ptr2 == dm2_dballochandler.mallocf)
      dm2_dballochandler.malloce = NULL;
    else
      dm2_dballochandler.malloce = ptr2;
    ptr2->w4 = con(0x1);
    return ptr2;
  }

  if (UI16CAST(v6) < UI16CAST(con(0xfffffffd)))
    v6++;
  ptr2->w4 = v6;
  if (ptr2 == dm2_dballochandler.mallocg)
    return ptr2;

  v4 = ptr2->w8;
  s_xmalloc* ptr11 = dm2_dballochandler.s_table[v4];
  if (UI16CAST(v6) <= UI16CAST(ptr11->w4))
    return ptr2;

  x16 v32 = ptr2->w6;
  if (v32 != con(0xffffffff))
  {
    if (ptr2 == dm2_dballochandler.malloce)
      dm2_dballochandler.malloce = ptr11;
    ptr2 = dm2_dballochandler.s_table[v32];
    ptr2->w8 = v4;
  }
  else
  {
    dm2_dballochandler.malloce = ptr11;
    dm2_dballochandler.malloch = ptr11;
  }
  ptr11->w6 = v32;

  ptr2 = dm2_dballochandler.malloce;
  if (ptr2 == dm2_dballochandler.mallocf)
    dm2_dballochandler.malloce = NULL;

  x16 v33;
  do
  {
    ptr2 = ptr11;
    v33 = v4;
    v4 = ptr11->w8;
    if (v4 == con(0xffffffff))
    {
      v4 = vw_00;
      ptr11->w8 = v4;
      ptr7->w6 = v33;
      ptr7->w8 = con(0xffffffff);
      dm2_dballochandler.mallocg = ptr7;
      return ptr7;
    }
    ptr11 = dm2_dballochandler.s_table[v4];
  } while (UI16CAST(v6) > UI16CAST(ptr11->w4));
  ptr2->w8 = vw_00;
  ptr7->w6 = v33;
  ptr7->w8 = v4;
  ptr11->w6 = vw_00;
  return ptr7;
}

void R_2E68E(x16 eaxw, bool edxbool)
{
  s_xmalloc* ptr4 = dm2_dballochandler.s_table[eaxw];
  if (ptr4->w4 != ucon16(0xffffffff))
  {
    SKW_3e74_48c9(eaxw);
    return;
  }

  s_xmalloc* ptr1;
  s_xmalloc* ptr2;
  if (!edxbool)
  {
    ptr4->w4 = con(0x1);
    ptr1 = dm2_dballochandler.malloce;
    ptr2 = dm2_dballochandler.mallocf;
    if (ptr2 == NULL)
      ptr2 = dm2_dballochandler.mallocg;
  }
  else
  {
    ptr4->w4 = ucon16(0xfffffffe);
    ptr1 = dm2_dballochandler.mallocf;
    ptr2 = dm2_dballochandler.mallocg;
  }

  bool flag = false;
  if (ptr1 != NULL)
    flag = true;
  else if (dm2_dballochandler.mallocg != NULL)
  {
    if (dm2_dballochandler.malloce != NULL || dm2_dballochandler.mallocf == NULL)
    {
      ptr2->w8 = eaxw;
      ptr4->w6 = DM2_ALLOC_CPX_GET_WTABLE_ENTRY(ptr2->wa);
    }
    else
    {
      ptr1 = ptr2;
      flag = true;
    }
  }
  else
  {
    ptr4->w6 = con(0xffffffff);
    dm2_dballochandler.malloch = ptr4;
  }
  if (flag)
  {
    x16 v22 = ptr1->w6;
    ptr1->w6 = eaxw;
    ptr4->w6 = v22;
    if (v22 == con(0xffffffff))
    {
      ptr4->w8 = DM2_ALLOC_CPX_GET_WTABLE_ENTRY(ptr1->wa);
      dm2_dballochandler.malloch = ptr4;
    }
    else
    {
      s_xmalloc* ptr11 = dm2_dballochandler.s_table[v22];
      ptr4->w8 = ptr11->w8;
      ptr11->w8 = eaxw;
    }
  }
  else
  {
    ptr4->w8 = con(0xffffffff);
    dm2_dballochandler.mallocg = ptr4;
  }
  if (!edxbool)
    dm2_dballochandler.malloce = ptr4;
  else
    dm2_dballochandler.mallocf = ptr4;
}

// old name: R_2BD3F
void DM2_DEALLOC_BIGPOOL_STRUCT_BEFORE(s_malloctail* ptr)
{
  if (dm2_dballochandler.gfxalloc_done || ptr == NULL)
    return;
  if ((ptr-1)->w4 != con(0x0))
    DM2_DEALLOC_HIBIGPOOL((ptr-1)->l0);
  else
    DM2_DEALLOC_LOBIGPOOL((ptr-1)->l0);
}

void SKW_GUARANTEE_FREE_CPXHEAP_SIZE(x32 amount)
{
  while (amount > dm2_dballochandler.cpx_available)
  {
    if (dm2_dballochandler.malloch == NULL)
      SKW_RAISE_SYSERR(con(0x2b));
    SKW_3e74_28de(dm2_dballochandler.malloch->wa);
  }
}

void SKW_3e74_2b30(void)
{
  if (dm2_dballochandler.ptrlist == NULL)
    return;
  s_xmalloc* ptr_src = dm2_allochandler.bigpool_start_b;
  s_xmalloc* ptr_dest = ptr_src;
  do
  {
    x32 amount = ptr_src->l0;
    if (amount <= con(0x0))
    {
      amount = -amount;
      if (ptr_src != ptr_dest)
      {
        dm2_dballochandler.s_table[DM2_ALLOC_CPX_GET_WTABLE_ENTRY(ptr_src->wa)] = ptr_dest;
        SKW_COPY_MEMORY(DOWNCAST(s_xmalloc, ptr_src), amount, DOWNCAST(s_xmalloc, ptr_dest));
        if (ptr_src == dm2_dballochandler.malloch)
          dm2_dballochandler.malloch = ptr_dest;
        if (ptr_src == dm2_dballochandler.mallocg)
          dm2_dballochandler.mallocg = ptr_dest;
        if (ptr_src != dm2_dballochandler.malloce)
        {
          if (ptr_src == dm2_dballochandler.mallocf)
            dm2_dballochandler.mallocf = ptr_dest;
        }
        else
          dm2_dballochandler.malloce = ptr_dest;
      }
      ptr_dest = UPCAST(s_xmalloc, DOWNCAST(s_xmalloc, ptr_dest) + amount);
    }
    ptr_src = UPCAST(s_xmalloc, DOWNCAST(s_xmalloc, ptr_src) + amount);
  } while (ptr_src != dm2_allochandler.bigpool_start_a);
  dm2_allochandler.bigpool_start_a = ptr_dest;
  dm2_dballochandler.ptrlist = NULL;
}

// was SKW_ALLOC_LOWER_CPXHEAP
s_xmalloc* SKW_ALLOC_CPXHEAP_CREATE_POINTER(x32 amount)
{
  SKW_GUARANTEE_FREE_CPXHEAP_SIZE(amount);
  xxx* ptr = DOWNCAST(s_xmalloc, dm2_allochandler.bigpool_start_a);
  if (DOWNCAST(void, dm2_dballochandler.bigpool_endofunpreservedfree) - ptr < amount)
  {
    s_lpp* xptr = dm2_dballochandler.ptrlist;
    if (amount <= xptr->amount)
    {
      while (amount != xptr->amount)
      {
        if (amount > xptr->amount || xptr->next == NULL)
        {
          xptr = dm2_dballochandler.ptrlist;
          break;
        }
        else
          xptr = xptr->next;
      }
      DM2_ALLOC_CPX_UNLINK_NODE(xptr);
      ptr = UPCAST(xxx, xptr);
      x32 val = xptr->amount - amount;
      if (val < con(0x1e))
        amount = xptr->amount;
      else
      {
        *P32CAST(ptr + amount + val - 4) = val;
        *P32CAST(ptr + amount) = val;
        DM2_ALLOC_CPX_LINK_NODE(UPCAST(s_lpp, ptr + amount));
      }
    }
    else
    {
      SKW_3e74_2b30();
      ptr = DOWNCAST(s_xmalloc, dm2_allochandler.bigpool_start_a);
      dm2_allochandler.bigpool_start_a = UPCAST(s_xmalloc, ptr + amount);
    }
  }
  else
    dm2_allochandler.bigpool_start_a = UPCAST(s_xmalloc, ptr + amount);

  s_xmalloc* sptr = UPCAST(s_xmalloc, ptr);
  dm2_dballochandler.cpx_available -= amount;
  sptr->l0 = -amount;
  *P32CAST(DOWNCAST(xxx, sptr) + amount - con(0x4)) = -amount;
  sptr->w8 = con(0xffffffff);
  sptr->w6 = con(0xffffffff);
  sptr->w4 = ucon16(0xffffffff);
  return sptr;
}
// old name: R_2E4B9
x16 SKW_ALLOC_CPXHEAP_CREATE_INDEX(void)
{
  x16 w = dm2_dballochandler.mallocm;
  if (w == con(0xffffffff))
  {
    SKW_3e74_28de(dm2_dballochandler.malloch->wa);
    w = dm2_dballochandler.mallocm;
  }
  if (++dm2_dballochandler.mallock < dm2_dballochandler.num_s_tableentries) // was unsigned comparison
    while (dm2_dballochandler.s_table[++dm2_dballochandler.mallocm] != NULL);
  else
    dm2_dballochandler.mallocm = con(0xffffffff);
  return w;
}

s_xmalloc* SKW_ALLOC_CPXHEAP_MEM(x16 num, x32 amount)
{
  amount += con(0x16);
  if (amount & con(0x1))
    amount++;
  s_xmalloc* ptr = SKW_ALLOC_CPXHEAP_CREATE_POINTER(amount);
  x16 index = SKW_ALLOC_CPXHEAP_CREATE_INDEX();
  dm2_dballochandler.w_table1[num] = index;
  dm2_dballochandler.s_table[index] = ptr;
  ptr->wa = num | con(0x8000);
  return ptr + 1; // so 0x12 bytes ahead
}

s_xmalloc* SKW_3e74_5788(x16 eaxw, x32 edxl)
{
  dm2_dballochandler.mallocw = true;
  SKW_3e74_28de(eaxw | con(0x8000));
  dm2_dballochandler.mallocw = false;
  return SKW_ALLOC_CPXHEAP_MEM(eaxw, edxl);
}

void R_2E581(void)
{
  dm2_dballochandler.malloci = ddata.longmallocx;
  dm2_dballochandler.malloce = NULL;
  s_xmalloc* ptr = dm2_dballochandler.mallocf;
  if (ptr == NULL)
  {
    ptr = dm2_dballochandler.mallocg;
    if (ptr == NULL)
      return;
  }
  while (ptr->w4 != con(0x0))
  {
    if (ptr->w4 <= ucon16(0xfffffffd))
      ptr->w4 = con(0x0);
    x16 index = ptr->w6;
    if (index == con(0xffffffff))
      return;
    ptr = dm2_dballochandler.s_table[index];
  }
}

x32 SKW_3e74_5673(ui32 eaxl, x32 ebxl, x16* edxpw)
{
  x16 vw_00;

  if (ddata.longmallocx != dm2_dballochandler.malloci)
    R_2E581();
  if (SKW_3e74_5420(eaxl, &vw_00))
  {
    x16 v = dm2_dballochandler.w_table3[vw_00];
    *edxpw = v;
    return -(SKW_3e74_48c9(DM2_ALLOC_CPX_GET_WTABLE_ENTRY(v | con(0x8000))))->l0 - con(0x16);
  }
  if (ebxl == con(0x0))
    *edxpw = con(0xffffffff);
  else
    *edxpw = SKW_3e74_54a1(eaxl, vw_00);
  return con(0x0);
}
