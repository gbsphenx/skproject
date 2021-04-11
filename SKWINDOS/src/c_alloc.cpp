#include "util.h"
#include "startend.h" // THROW...
#include "c_alloc.h"

#include "dm2global.h"	// SPX

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
   - DM2_APPEND_FREE_POOL(c_freepool* fp, x16 mode, x32 amount)
     fp is the pointer to the area
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

extern void SKW_RAISE_SYSERR(x16 eaxw);

#define DEFAULT_MASK (0xff8)

#define NUM_OF_FREEPOOLS 4
#define SIZEOF_FREEPOOL  0x4000000

i8* allocated_memory = NULL;

c_allochandler dm2_allochandler;

void c_allochandler::init(void)
{
  is_allocated = false;
  freepoollist_end = NULL;
  bigpool_mode = 0;
  bigpool_available = 0;
  bigpool_start_a = NULL;
  bigpool_start_b = NULL;
  bigpool_endoffree = NULL;
  //malloca(NULL),
  secondpool_mode = 0;
  secondpool_available = 0;
  secondpool_endoffree =NULL;
}

// was SKW_3e74_0756
static xxx* DM2_GET_FROM_FREEPOOL(c_freepool* freepool, x32 amount)
{
  freepool->available -= amount;
  xxx* ptr = P8CAST(freepool->endoffree) - amount;
  freepool->endoffree = VCAST(ptr);
  return ptr;
}

static c_freepool* DM2_FIND_FREE_POOL(x32 amount, x16 mask)
{
  c_freepool* freepool = dm2_allochandler.freepoollist_end;
  c_freepool* pool = NULL;
  x32 smallest = con(0x7fffffff);
  while (freepool != NULL)
  {
    if (freepool->tag == con(0x0))
      if ((freepool->mode & mask) == mask)
      {
        x32 n = freepool->available - amount;
        if (n >= 0 && n < smallest)
        {
          smallest = n;
          pool = freepool;
        }
      }
    freepool = freepool->prev;
  }
  return pool;
}

xxx* DM2_ALLOC_MEMORY_RAM(x32 amount, x16 mask, x16 type)
{
  xxx* alloced;
  c_freepool* pool;

  bool clr = type & con(0x8000);
  type &= con(0x7fff);
  if (amount & con(0x1))
    amount++;

  if (type == FREEPOOL && (pool = DM2_FIND_FREE_POOL(amount, mask)) != NULL)
    alloced = DM2_GET_FROM_FREEPOOL(pool, amount);
  else if (type == FREEPOOL && mask == (dm2_allochandler.secondpool_mode & mask) && amount <= dm2_allochandler.secondpool_available)
  {
    dm2_allochandler.secondpool_available -= amount;
    alloced = UPCAST(xxx, dm2_allochandler.secondpool_endoffree) - amount;
    dm2_allochandler.secondpool_endoffree = VCAST(alloced);
  }
  else
  {
    if (amount > dm2_allochandler.bigpool_available)
      SKW_RAISE_SYSERR(con(0x28));
    if (type == BIGPOOL_HI)
      alloced = DOWNCAST(void, dm2_allochandler.bigpool_endoffree) - dm2_allochandler.bigpool_available;
    else // BIGPOOL_LO or other reason
    {
      alloced = DOWNCAST(void, dm2_allochandler.bigpool_endoffree) - amount;
      dm2_allochandler.bigpool_endoffree = VCAST(alloced);
    }
    dm2_allochandler.bigpool_available -= amount;
  }

  //if (clr) // SPX: debug, I believe it should always been cleaned up before used to distinguish a real non-allocated space
    DM2_ZERO_MEMORY(alloced, amount);
  return alloced;
}

void DM2_DEALLOC_LOBIGPOOL(x32 amount)
{
  if (amount & con(0x1))
    amount++;
  dm2_allochandler.bigpool_available += amount;
  dm2_allochandler.bigpool_endoffree = VCAST(DOWNCAST(void, dm2_allochandler.bigpool_endoffree) + amount);
}

void DM2_DEALLOC_HIBIGPOOL(x32 amount)
{
  if (amount & con(0x1))
    amount++;
  dm2_allochandler.bigpool_available += amount;
}

// was SKW_ALLOC_UPPER_MEMORY
// (used also as plugin for SKW_COMPRESS_RECTS)
xxx* DM2_ALLOC_FREEPOOL_MEMORY(x32 amount, bool clean)
{
  return DM2_ALLOC_MEMORY_RAM(amount, MASK_STD, FREEPOOL | (clean ? CLEAN_IT : 0));
}

// just an additional parametrized version of DM2_ALLOC_MEMORY_RAM
xxx* DM2_ALLOC_LOBIGPOOL_MEMORY(x32 amount, bool clean)
{
  return DM2_ALLOC_MEMORY_RAM(amount, MASK_STD, BIGPOOL_LO | (clean ? CLEAN_IT : 0));
}

// just an additional parametrized version of DM2_ALLOC_MEMORY_RAM
xxx* DM2_ALLOC_HIBIGPOOL_MEMORY(x32 amount, bool clean)
{
  return DM2_ALLOC_MEMORY_RAM(amount, MASK_STD, BIGPOOL_HI | (clean ? CLEAN_IT : 0));
}

// was SKW_3e74_059e
static c_freepool* DM2_TAG_LARGEST_FREE_POOL(x32 mask)
{
  c_freepool* fp = dm2_allochandler.freepoollist_end;
  if (fp == NULL) return NULL; // early out, list empty

  c_freepool* fp_ret = NULL;
  i32 maximum = 0; // begin with minimal amount

  do // walk through the available nodes to find a block large enough
  {
    if (fp->tag == 0) // only untagged ones
    {
      if ((UI32CAST(fp->mode) & mask) == mask)
        if (maximum < fp->amount) // if so, block found
        {
          fp_ret = fp;
          maximum = fp->amount; // go for the largest block
        }
    }
    fp = fp->prev;
  } while (fp != NULL);

  if (fp_ret != NULL)
    fp_ret->tag = 1; // tag the found structure!

  return fp_ret;
}

// was SKW_INIT_FREE_POOL
static void DM2_APPEND_FREE_POOL(c_freepool* fp, x16 mode, x32 amount)
{
  fp->prev = dm2_allochandler.freepoollist_end;
  fp->amount = amount - sizeof(c_freepool);
  fp->endoffree = UPCAST(void, DOWNCAST(xxx, fp) + amount);
  fp->available = fp->amount;
  fp->mode = mode;
  fp->tag = 0;
  dm2_allochandler.freepoollist_end = fp;
}

// next one is splitted in two routines now
// (calling with NULL-parameters is: DM2_COMPLETE_ALLOCATION now
static bool DM2_ADD_MEM_TO_FREE_POOL(c_freepool* fp, x32 mask, x32 amount)
{
  amount &= 0xfffffffe;
  if (amount < 0x34) // TODO constant
    return false;
  DM2_APPEND_FREE_POOL(fp, mask, amount);
  return true;
}

// the 2nd part that completes the database-allocation
// got splitted into DM2_SETUP_DB_ALLOCATION
static void DM2_COMPLETE_ALLOCATION(void)
{
  dm2_allochandler.is_allocated = true; // never read or written else

  c_freepool* d = DM2_TAG_LARGEST_FREE_POOL(DEFAULT_MASK);
  if (d == NULL)
    SKW_RAISE_SYSERR(0x31); // TODO constant

  void* ptr = DOWNCAST(xxx, d) + sizeof(c_freepool); // virtual start of memory of the largest free pool
  dm2_allochandler.bigpool_start_a = UPCAST(s_xmalloc, ptr);
  dm2_allochandler.bigpool_start_b = UPCAST(s_xmalloc, ptr);

  ptr = d->endoffree; // virtual end of memory of the largest free pool
  //dm2_allochandler.malloca = ptr; // malloca only here - not needed anymore
  dm2_allochandler.bigpool_endoffree = VCAST(ptr);
  dm2_allochandler.bigpool_available = d->amount;
  dm2_allochandler.bigpool_mode = d->mode;

  // now tag the 2nd best
  c_freepool* a = DM2_TAG_LARGEST_FREE_POOL(DEFAULT_MASK); // that's the 2nd largest then
  if (a != NULL)
  {
    dm2_allochandler.secondpool_mode = a->mode;
    dm2_allochandler.secondpool_endoffree = UPCAST(void, DOWNCAST(i8, a) + sizeof(c_freepool) + a->amount); // real end of 2nd largest block
    dm2_allochandler.secondpool_available = a->amount;
  }
}

void DM2_SETUP_MEMORYALLOCATION(void)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("DM2_SETUP_MEMORYALLOCATION");
  dm2_allochandler.init(); // new!

  dm2_allochandler.freepoollist_end = NULL; // clean it

  int amount = SIZEOF_FREEPOOL;

  // get the memory in one piece
  if (allocated_memory == NULL)
    allocated_memory = new i8[NUM_OF_FREEPOOLS * amount];
  if (allocated_memory == NULL)
    throw(THROW_DMABORT);

  // Now we split that zone in NUM_OF_FREEPOOLS pieces
  // by placing a c_freepool - structure at the beginning
  // of each subzone - like it was done in R_C84
  for (int i=0; i<NUM_OF_FREEPOOLS; i++)
    DM2_ADD_MEM_TO_FREE_POOL((c_freepool*)(allocated_memory + i * amount), DEFAULT_MASK, amount);

  // complete the allocation with the appropriate call
  DM2_COMPLETE_ALLOCATION();
SPX_DEBUG_POP;
}

void DM2_DTOR_MEMORYALLOCATION(void)
{
SPX_DEBUG_PUSH_FUNCTION_INFO("DM2_DTOR_MEMORYALLOCATION");
  if (allocated_memory != NULL)
    delete [] allocated_memory;
  allocated_memory = NULL;
SPX_DEBUG_POP;
}
