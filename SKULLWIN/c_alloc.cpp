#include <stdio.h> // NULL
#include <string.h> // memset

#include "util.h"
#include "startend.h" // THROW..., DM2_RAISE_SYSERR
#include "c_alloc.h"

#include "dm2debug.h"

/*
   MEMORY ALLOCATION

   Because of the deep going routines R_4F4A5 and R_4F6EB that
   use setblock, malloc and dpmi - malloc, we have to checkout
   what happened to the memory from gamestart to the snapshot.

   A first hint gives the variable memory[FREEPOOLS].
   The value is 3.
   So the first loop ran three times.
   The returning pointers should be stored from memory[FREEPOOLPTRARR] on.

   The resulting indices are:
   0x1f0038
   0x8bb28
   0x1eaa78

   After this the userinterrupt gets called with function 0x29.
   That way we have the fourth call of DM2_ADD_MEM_TO_FREE_POOL.
   I assume that in those calls DM2_APPEND_FREE_POOL is called everytime.
   - DM2_APPEND_FREE_POOL(c_freepool* fpp, i16 mode, i32 amount)
     fpp is the pointer to the area
     mode always DEFAULT_MASK
    amount is the size of the free area in bytes.
   In the end another call of DM2_ADD_MEM_TO_FREE_POOL happens,
   with NULL as pointer, and this one completes the allocation.

   Now DM2_APPEND_FREE_POOL adds entries (c_freepool) to a list.
   The pointer memory[0x1e09d8] marks the end of the list.
   It points to 0x111e4 in the newer snapshot, to 0x10b24 in
   the old one.

   Whatever is written to those nodes.
   We set it up on our own.
   Still: one free pool is not enough!

   notes:
   at 104b2 (old start): 1eaa78 7009e 80be0 7009e ff8 1
   at 80be0 there's all 0
   at 1eaa78:  8bc68 3fe2 1eaa96 0 ff8 0
   at 8bc68:   1f0038 dfe2 8bc8a 4 ff8 0
   at 8bc8a:   1eaab8 fa20fa0 40418 46700fa2 ff8 0
   at 40418; there's all 0
   at 1eaa96:  0
   at 1f0038:  0 e0dfe2 ffe038 e0dfe2 ff8 1
   at 1eaab8:  1eaaa2
*/

#define DEFAULT_MASK (0xff8)

#define NUM_OF_FREEPOOLS 4
#define SIZEOF_FREEPOOL  0x4000000

i8* allocated_memory = NULL;

void c_allochandler::init(void)
{
  is_allocated = false;
  freepoollist_end = NULL;
  bigpool_mode = 0;
  bigpool = 0;
  bigpool_start_a = NULL;
  bigpool_start_b = NULL;
  bigpool_endoffree = NULL;
  //malloca(NULL),
  secondpool_mode = 0;
  secondpool_available = 0;
  secondpool_endoffree = NULL;
}

// was SKW_3e74_0756
unk* c_allochandler::DM2_GET_FROM_FREEPOOL(c_freepool* freepool, i32 amount)
{
  freepool->available -= amount;
  unk* xptr = DOWNCAST(void, freepool->endoffree) - amount;
  freepool->endoffree = VCAST(xptr);
  return xptr;
}

c_freepool* c_allochandler::DM2_FIND_FREE_POOL(i32 amount, i16 wmask)
{
  c_freepool* freepool = freepoollist_end;
  c_freepool* pool = NULL;
  i32 smallest = lcon(0x7fffffff);
  while (freepool != NULL)
  {
    if (freepool->tag == 0)
      if ((freepool->mode & wmask) == wmask)
      {
        i32 ln = freepool->available - amount;
        if (ln >= 0 && ln < smallest)
        {
          smallest = ln;
          pool = freepool;
        }
      }
    freepool = freepool->fp_prev;
  }
  return pool;
}

unk* c_allochandler::DM2_ALLOC_MEMORY_RAM(i32 amount, i16 wmask, i16 wtype)
{
  unk* alloced;
  c_freepool* pool;

  bool clr = (wtype & wcon(0x8000)) != 0;
  wtype &= lcon(0x7fff);
  if (IS_ODD(amount))
    amount++;

LOGX(("ALLOC MEM RAM: BPEOF = %08x / amount %d, wmask: %04x, wtype = %d\n", bigpool_endoffree, amount, wmask, wtype));

  if (wtype == FREEPOOL && (pool = DM2_FIND_FREE_POOL(amount, wmask)) != NULL)
    alloced = DM2_GET_FROM_FREEPOOL(pool, amount);
  else if (wtype == FREEPOOL && wmask == (secondpool_mode & wmask) && amount <= secondpool_available)
  {
    secondpool_available -= amount;
    alloced = XDOWNCAST(void, secondpool_endoffree) - amount;
    secondpool_endoffree = VCAST(alloced);
  }
  else
  {
    if (amount > bigpool)
      DM2_RAISE_SYSERR(0x28);
    if (wtype == BIGPOOL_HI)
      alloced = DOWNCAST(void, bigpool_endoffree) - bigpool;
    else // BIGPOOL_LO or other reason
    {
      alloced = DOWNCAST(void, bigpool_endoffree) - amount;
      bigpool_endoffree = VCAST(alloced);
LOGX(("ALLOC MEM RAM: BPEOF = %08x just changed.\n", bigpool_endoffree));
    }
    bigpool -= amount;
  }

  if (clr)
    DM2_ZERO_MEMORY(alloced, amount);
  return alloced;
}

void c_allochandler::DM2_DEALLOC_LOBIGPOOL(i32 amount)
{
  if (IS_ODD(amount))
    amount++;
  bigpool += amount;
  bigpool_endoffree = VCAST(DOWNCAST(void, bigpool_endoffree) + amount);
LOGX(("DM2_DEALLOC_LOBIGPOOL: BPEOF = %08x changed / amount %d\n", bigpool_endoffree, amount));
}

void c_allochandler::DM2_DEALLOC_HIBIGPOOL(i32 amount)
{
  if (IS_ODD(amount))
    amount++;
  bigpool += amount;
}

// was SKW_ALLOC_UPPER_MEMORY
// (used also as plugin for DM2_COMPRESS_RECTS)
unk* c_allochandler::DM2_ALLOC_FREEPOOL_MEMORY(i32 amount, bool clean)
{
  return DM2_ALLOC_MEMORY_RAM(amount, MASK_STD, FREEPOOL | (clean ? CLEAN_IT : 0));
}

// just an additional parametrized version of DM2_ALLOC_MEMORY_RAM
unk* c_allochandler::DM2_ALLOC_LOBIGPOOL_MEMORY(i32 amount, bool clean)
{
  return DM2_ALLOC_MEMORY_RAM(amount, MASK_STD, BIGPOOL_LO | (clean ? CLEAN_IT : 0));
}

// just an additional parametrized version of DM2_ALLOC_MEMORY_RAM
unk* c_allochandler::DM2_ALLOC_HIBIGPOOL_MEMORY(i32 amount, bool clean)
{
  return DM2_ALLOC_MEMORY_RAM(amount, MASK_STD, BIGPOOL_HI | (clean ? CLEAN_IT : 0));
}

// was SKW_3e74_059e
c_freepool* c_allochandler::DM2_TAG_LARGEST_FREE_POOL(i32 lmask)
{
  c_freepool* fpp = freepoollist_end;
  if (fpp == NULL) return NULL; // early out, list empty

  c_freepool* fpp_ret = NULL;
  i32 maximum = 0; // begin with minimal amount

  do // walk through the available nodes to find a block large enough
  {
    if (fpp->tag == 0) // only untagged ones
    {
      if ((mkul(fpp->mode) & mkul(lmask)) == mkul(lmask))
        if (maximum < fpp->amount) // if so, block found
        {
          fpp_ret = fpp;
          maximum = fpp->amount; // go for the largest block
        }
    }
    fpp = fpp->fp_prev;
  } while (fpp != NULL);

  if (fpp_ret != NULL)
    fpp_ret->tag = 1; // tag the found structure!

  return fpp_ret;
}

// was SKW_INIT_FREE_POOL
void c_allochandler::DM2_APPEND_FREE_POOL(c_freepool* fpp, i16 mode, i32 amount)
{
  fpp->fp_prev = freepoollist_end;
  fpp->amount = amount - sizeof(c_freepool);
  fpp->endoffree = UPCAST(void, DOWNCAST(c_freepool, fpp) + amount);
  fpp->available = fpp->amount;
  fpp->mode = mode;
  fpp->tag = 0;
  freepoollist_end = fpp;
}

// next one is splitted in two routines now
// (calling with NULL-parameters is: DM2_COMPLETE_ALLOCATION now
bool c_allochandler::DM2_ADD_MEM_TO_FREE_POOL(c_freepool* fpp, i32 lmask, i32 amount)
{
  amount &= 0xfffffffe;
  if (amount < 52) // TODO constant
    return false;
  DM2_APPEND_FREE_POOL(fpp, lmask, amount);
  return true;
}

// was SKW_3e74_07b2
void c_allochandler::DM2_BUP_FREEPOOL(void)
{
  c_freepool* fpp = freepoollist_end;
  while (fpp != NULL)
  {
    if (fpp->tag == 0)
    {
      fpp->eof_bup = fpp->endoffree;
      fpp->ava_bup = fpp->available;
    }
    fpp = fpp->fp_prev;
  }
}

// was SKW_3e74_0820
void c_allochandler::DM2_RESTORE_FREEPOOL(void)
{
  c_freepool* fpp = freepoollist_end;
  while (fpp != NULL)
  {
    if (fpp->tag == 0)
    {
      fpp->endoffree = fpp->eof_bup;
      fpp->available = fpp->ava_bup;
    }
    fpp = fpp->fp_prev;
  }
}

// the 2nd par_tp that completes the database-allocation
// got splitted into DM2_SETUP_DB_ALLOCATION
void c_allochandler::DM2_COMPLETE_ALLOCATION(void)
{
  is_allocated = true; // never read or written elsewhere

  c_freepool* fpd = DM2_TAG_LARGEST_FREE_POOL(DEFAULT_MASK);
  if (fpd == NULL)
    DM2_RAISE_SYSERR(0x31); // TODO constant

  void* vptr = VCAST(DOWNCAST(c_freepool, fpd) + sizeof(c_freepool)); // virtual start of memory of the largest free pool
  bigpool_start_a = CHGCAST(s_xmalloc, vptr);
  bigpool_start_b = CHGCAST(s_xmalloc, vptr);

  vptr = fpd->endoffree; // virtual end of memory of the largest free pool
  //malloca = vptr; // malloca only here - obsolete
  bigpool_endoffree = vptr;
LOGX(("DM2_COMPLETE_ALLOCATION: BPEOF = %08x changed by vptr\n", bigpool_endoffree));
  bigpool = fpd->amount;
  bigpool_mode = fpd->mode;

  // now tag the 2nd best
  c_freepool* fpa = DM2_TAG_LARGEST_FREE_POOL(DEFAULT_MASK); // that's the 2nd largest then
  if (fpa != NULL)
  {
    secondpool_mode = fpa->mode;
    secondpool_endoffree = UPCAST(void, DOWNCAST(c_freepool, fpa) + sizeof(c_freepool) + fpa->amount); // real end of 2nd largest block
    secondpool_available = fpa->amount;
  }
}

void c_allochandler::DM2_SETUP_MEMORYALLOCATION(void)
{
  init(); // new!

  freepoollist_end = NULL; // clean it

  int iamount = SIZEOF_FREEPOOL;

  // get the memory in one piece
  if (allocated_memory == NULL)
    allocated_memory = new i8[NUM_OF_FREEPOOLS * iamount];
  if (allocated_memory == NULL)
    throw(THROW_DMABORT);

  // Now we split that zone in NUM_OF_FREEPOOLS pieces
  // by placing a c_freepool - structure at the beginning
  // of each subzone - like it was done in R_C84
  for (int i = 0; i < NUM_OF_FREEPOOLS; i++)
    DM2_ADD_MEM_TO_FREE_POOL(UPCAST(c_freepool, allocated_memory + i * iamount), DEFAULT_MASK, iamount);

  // complete the allocation with the appropriate call
  DM2_COMPLETE_ALLOCATION();
}

void c_allochandler::DM2_DTOR_MEMORYALLOCATION(void)
{
  if (allocated_memory != NULL)
    delete [] allocated_memory;
  allocated_memory = NULL;
}
