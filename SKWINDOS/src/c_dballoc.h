#ifndef C_DBALLOC_H
  #define C_DBALLOC_H

  #ifndef TYPES_H
    #include "../types.h"
  #endif

  #ifndef C_ALLOC_H
    #include "c_alloc.h"
  #endif

// setting 'gfxalloc_done' and 'filebuffer' public,
// all others can be protected, used by the functions
// within c_dballoc.cpp and c_dballocx.cpp only

  struct GDATEntries	// s_1e09e0 renamed GDATEntries
  {
    x16* w_table4;
    x16* w_table5;
    u_bbwlong* l_table4;
    x16 w_0c;		// cnt cls1
    x16 w_0e;		// total cnt of cls2
    x16 w_10;
    x16 w_12;
    x16 w_14;
    x16 warr_16[0x7];
    x8 barr_24[0x8];	// skwin has 5 instead of 8
  };

  struct s_pregfxdata // used for neg. offsets - don't make it longer
  {
    x8 b_00;
    x8 b_01;
    x16 w_02;
    x16 w_04;
  };

  struct s_hmm
  {
    x32 l_00; // ?
    x32 l_04; // ?
    s_hex6* ps_08;  // struct made because of this
    // TODO: smth behind?
  };

  struct s_grrr
  {
    union
    {
      x16 w0; // 2nd word not used then
      x32 l0;
    } u;
    x8 b4;
    x8 b5;
  };

  union u_lp
  {
    x32 l_00;
    xxx* p_00;
  };

  struct s_xlink // size 0xe
  {
    s_xlink* p_00;
    x16 w_04;
    x16 w_06;
    x8 b_08;
    x8 b_09;
    x16 w_0a;
    x16 w_0c;
  };

  union u_wwl
  {
    struct
    {
      x16 w0;
      x16 w2;
    } s0;
    x32 l0;
  };

  class c_ulp
  {
    protected:
      u_lp* ulptable;

	//private:
    public:	// SPX: for debug purpose only
      u_lp* getadr(ui32 n) const { return &ulptable[n]; }

    public:
      c_ulp(void);
      void init(void);
      xxx* getp(ui32 n) const { return ulptable[n].p_00; }
      void setp(ui32 n, xxx* p) { ulptable[n].p_00 = p; }
      void setl(ui32 n, x32 l) { ulptable[n].l_00 = l | 0x80000000; }
      bool islen(ui32 n) const { return ulptable[n].l_00 & 0x80000000; }
      x32 SKW_QUERY_GDAT_RAW_DATA_LENGTH(x16 index);
      void R_2D87A(x16 index);

	  // SPX for debug
	  ui32 getv(ui32 n) { return ulptable[n].l_00; }
	bool Absent(ui32 n) const { return (ulptable[n].l_00 & 0x80000000) != 0; }	// from skwin
	bool Present(ui32 n) const { return (ulptable[n].l_00 & 0x80000000) == 0; } // from skwin

  };

  extern c_ulp dm2_ulp;

  class c_dballochandler
  {
    public:
      static const x8 tidspfg[8];

    public:
      bool gfxalloc_done; // set one time only, never reset, public
      i8* filebuffer; // public

      x32 cpx_available;
      s_lpp* ptrlist;
      s_xmalloc* malloce;
      s_xmalloc* mallocf;
      s_xmalloc* mallocg;
      s_xmalloc* malloch;
      i32 malloci;
      void* bigpool_endofunpreservedfree;
      x16 mallock;
      x16 num_s_tableentries;
      x16 mallocm;
      s_xmalloc** s_table;
      x16* w_table1;
      x16 mallocn;
      x16 malloco;
      x16 preserved_table_entries;
      x16* w_table3;
      ui32* l_table3;
      s_pppw pppw1;
      s_pppw pppw2;
      x16 preserved_bigpool;
      x16* w_table2;
      bool mallocw;
      GDATEntries gdattable;
      bool v1d70d7;
      x16 v1e13f8;
      x16 v1e13fa;
      x8 v1e13fe[0xa]; // TODO: probably dim just 0x5, or even 0x4
                       // only values 0 and 1 used, so basically
                       // just a bool-array (with one-byte-bools!)
      ui16 v1d6781;
      x32 v1d6783;
      x16 v1d6773;
      x16 v1e0abe;
      x8* ptr1e09a8;
      x16 v1e09ae;
      x16 v1e09b0;
      x16 v1e09b2[7];
      x8 v1e09c0[7];
      x16 v1d6793;
      xxx* ptr1e0a5c;
      x8 v1d6787[8+4];
      x16 v1e09ac;
      x8 v1d6770;
      s_pwwp* v1dff38;
      xxx* v1e149c;
      xxx* v1e14a0;
      x16 v1e0ad4;
      ui16 v1e0ad6;
      ui16 v1e0ad8;
      x16 v1e0acc;
      x32 v1d6775;
      x32 v1e0a74;
      bool v1e0a84;
      x32 v1e0a90;
      x32 v1e0a8c;
      x32 v1e0a0c;
      bool v1e0200;

    public:
      void init(void);
  };

  extern c_dballochandler dm2_dballochandler;

  void DM2_SETUP_DB_ALLOCATION(void);
  void SKW_ALLOC_CPX_SETUP(const x8* eaxp);
  void SKW_3e74_585a(x16 eaxw, bool edxbool);
  void R_2F41B(x16 eaxw);
  void SKW_3e74_28de(x16 eaxw);
  x16 DM2_ALLOC_CPX_GET_WTABLE_ENTRY(x16 eaxw);
  x16 SKW_3e74_54a1(ui32 eaxl, x16 index);
  bool SKW_3e74_5420(ui32 eaxl, x16* edxpw);
  s_xmalloc* SKW_3e74_48c9(x16 eaxw);
  void R_2E68E(x16 eaxw, bool edxbool);
  void DM2_DEALLOC_BIGPOOL_STRUCT_BEFORE(s_malloctail* ptr);
  x16 SKW_ALLOC_CPXHEAP_CREATE_INDEX(void);
  void SKW_GUARANTEE_FREE_CPXHEAP_SIZE(x32 amount);
  void SKW_3e74_2b30(void);
  s_xmalloc* SKW_ALLOC_CPXHEAP_CREATE_POINTER(x32 amount);
  s_xmalloc* SKW_ALLOC_CPXHEAP_MEM(x16 num, x32 amount);
  s_xmalloc* SKW_3e74_5788(x16 eaxw, x32 edxl);
  void R_2E581(void);
  x32 SKW_3e74_5673(ui32 eaxl, x32 ebxl, x16* edxpw);

#endif
