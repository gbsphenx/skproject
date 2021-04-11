#ifndef C_ALLOC_H
  #define C_ALLOC_H

  #ifndef TYPES_H
    #include "../types.h"
  #endif

  class c_freepool // total size is 0x1e
  {
    public:
      c_freepool* prev; // 0x0  start of the free area
      i32 amount;       // 0x4  amount - sizeof(c_freepool)
      void* endoffree;  // 0x8  end of the free area
      i32 available;    // 0xc  copy of val_4 in the beginning
      i16 mode;         // 0x10 mode (usually 0xff8)
      i32 tag;          // 0x12 0 in the beginning, can be 1
                        //      a bool in fact, but we keep the size
      void* eof_bup;    // 0x16 needs DM2_BUP_FREEPOOL_STATES to be called
      i32 ava_bup;      // 0x1a needs DM2_BUP_FREEPOOL_STATES to be called
  };

  // allocation types for DM2_ALLOC_MEMORY_RAM
  #define BIGPOOL_LO 0
  #define FREEPOOL 1
  #define BIGPOOL_HI 2
  #define CLEAN_IT 0x8000  // or-ize types to cleanup area

  #define MASK_8   0x8
  #define MASK_200 0x200
  #define MASK_280 0x280
  #define MASK_STD 0x400

  struct s_lpp
  {
    x32 amount; // 0x0
    s_lpp* prev; // 0x4
    s_lpp* next; // 0x8
  };

  struct s_malloctail // TODO: don't make it longer! used negative
  {
    x32 l0; // a length-value
    x16 w4; // marker which memory-pool is used (0 = LOBIGPOOL else HIBIGPOOL)
  };

  struct s_xmalloc // sometimes casted to s_lpp
  {
    x32 l0;
    ui16 w4;
    x16 w6;
    x16 w8;
    x16 wa;
    // variable size then
    s_malloctail sc;
  }; // total size 0x12 now

  class c_allochandler
  {
    public:
      bool is_allocated;

      c_freepool* freepoollist_end;

      i16 bigpool_mode;
      i32 bigpool_available;
      s_xmalloc* bigpool_start_a;
      s_xmalloc* bigpool_start_b;
      void* bigpool_endoffree;
      //void* malloca; // not needed anymore

      i16 secondpool_mode;
      i32 secondpool_available;
      void* secondpool_endoffree;

    public:
      void init(void);
  };

  extern c_allochandler dm2_allochandler;

  xxx* DM2_ALLOC_MEMORY_RAM(x32 amount, x16 mask, x16 type);

  xxx* DM2_ALLOC_FREEPOOL_MEMORY(x32 amount, bool clean = false); // called indirect too
  xxx* DM2_ALLOC_LOBIGPOOL_MEMORY(x32 amount, bool clean = false);
  xxx* DM2_ALLOC_HIBIGPOOL_MEMORY(x32 amount, bool clean = false);

  void DM2_DEALLOC_LOBIGPOOL(x32 amount);
  void DM2_DEALLOC_HIBIGPOOL(x32 amount);

  void DM2_SETUP_MEMORYALLOCATION(void);
  void DM2_DTOR_MEMORYALLOCATION(void);

#endif
