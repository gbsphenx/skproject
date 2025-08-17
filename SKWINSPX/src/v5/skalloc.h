#ifndef C_ALLOC_H
  #define C_ALLOC_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  class c_freepool // size: 0x1e
  {
    public:
      c_freepool* fp_prev; // @00 start of the free area
      i32 amount;          // @04 amount - sizeof(c_freepool)
      void* endoffree;     // @08 end of the free area
      i32 available;       // @0c copy of @04 in the beginning
      i16 mode;            // @10 mode (usually 0xff8)
      i32 tag;             // @12 0 in the beginning, can be 1
                           //     a bool in fact, but we keep the size
      void* eof_bup;       // @16 needs DM2_BUP_FREEPOOL_STATES to be called
      i32 ava_bup;         // @1a needs DM2_BUP_FREEPOOL_STATES to be called
  };

  // allocation types for DM2_ALLOC_MEMORY_RAM
  #define BIGPOOL_LO 0
  #define FREEPOOL   1
  #define BIGPOOL_HI 2
  #define CLEAN_IT   0x8000  // or-ize types to cleanup area

  #define MASK_8     0x8
  #define MASK_200   0x200
  #define MASK_280   0x280
  #define MASK_STD   0x400

  struct s_malloctail // TODO: don't make it longer! used negative
  {
    union
    {
      i32 l_00; // a length-value
      struct
      {
        i8 b_00;
        i8 dummy;
        i16 w_02;
      };
    };
    i16 w_04; // marker which memory-pool is used (0 = LOBIGPOOL else HIBIGPOOL)
  };

  typedef i16 t_dbidx;
  #define NODATA (-1)

  struct s_lpp // size: 0xc
  {
    i32 amount;  // @00
    s_lpp* prev; // @04
    s_lpp* next; // @08
  };

  struct s_xmalloc // size: 0x12
  {
    union
    {
      struct
      {
        i32 l_00;          // @00
        t_dbidx dbidx0;    // @04
        t_dbidx prev_idx;  // @06, index of dm2_dballochandler.gfx_table, was dbidx1
        t_dbidx next_idx;  // @08, was dbidx2
      };
      s_lpp lpp;           // @00
    };
    t_dbidx dbidx3;        // @0a
    s_malloctail s78_0c;   // @0c
  };

  class c_allochandler
  {
  public:	// SPX for debug
//    protected:
      bool is_allocated;

      c_freepool* freepoollist_end;

      i16 bigpool_mode;
      i32 bigpool;
      s_xmalloc* bigpool_start_a;
      s_xmalloc* bigpool_start_b;
      void* bigpool_endoffree;
      //void* malloca; // not needed anymore

      i16 secondpool_mode;
      i32 secondpool_available;
      void* secondpool_endoffree;

    protected:
      c_freepool* DM2_FIND_FREE_POOL(i32 amount, i16 wmask);
      unk* DM2_GET_FROM_FREEPOOL(c_freepool* freepool, i32 amount);
      c_freepool* DM2_TAG_LARGEST_FREE_POOL(i32 lmask);
      void DM2_APPEND_FREE_POOL(c_freepool* fpp, i16 mode, i32 amount);
      bool DM2_ADD_MEM_TO_FREE_POOL(c_freepool* fpp, i32 lmask, i32 amount);
      void DM2_BUP_FREEPOOL(void);
      void DM2_RESTORE_FREEPOOL(void);
      void DM2_COMPLETE_ALLOCATION(void);
      void init(void);

    public:
      unk* DM2_ALLOC_FREEPOOL_MEMORY(i32 amount, bool clean = false); // called indirect too
      unk* DM2_ALLOC_LOBIGPOOL_MEMORY(i32 amount, bool clean = false);
      unk* DM2_ALLOC_HIBIGPOOL_MEMORY(i32 amount, bool clean = false);
      unk* DM2_ALLOC_MEMORY_RAM(i32 amount, i16 wmask, i16 wtype);

      void DM2_DEALLOC_LOBIGPOOL(i32 amount);
      void DM2_DEALLOC_HIBIGPOOL(i32 amount);

      i32 bigpool_available(void) const { return bigpool; }
      s_xmalloc* get_bigpool_start_a(void) const { return bigpool_start_a; }
      s_xmalloc* get_bigpool_start_b(void) const { return bigpool_start_b; }
      s_lpp* get_bigpool_start_a_lppadr(void) const { return &bigpool_start_a->lpp; }

      void set_bigpool_start_a(s_xmalloc* ptr) { bigpool_start_a = ptr; }

      void DM2_SETUP_MEMORYALLOCATION(void);
      void DM2_DTOR_MEMORYALLOCATION(void);
  };


#endif
