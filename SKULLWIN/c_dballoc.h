#ifndef C_DBALLOC_H
  #define C_DBALLOC_H

  #ifndef XTYPES_H
    #include "xtypes.h"
  #endif

  #ifndef C_ALLOC_H
    #include "c_alloc.h"
  #endif

  #ifndef C_GFX_BMP_H
    #include "c_gfx_bmp.h"
  #endif

// setting 'gfxalloc_done' and 'filebuffer' public,
// all others can be protected, used by the functions
// within c_dballoc.cpp and c_dballocx.cpp only

  struct s_1e09e0 // size: 0x2c
  {
    i16* w_table1; // 00
    i16* w_table2; // 04
    s_bbw* u31p_08; // 08
    i16 entries; // 0c
    i16 w_0e;
    i16 w_10;
    i16 w_12;
    i16 w_14;
    i16 warr_16[0x7];
    i8 barr_24[0x8];
  };

  struct s_hex6
  {
    i16 w_00;
    i8 b_02;
    i8 b_03;
    i8 b_04;
    i8 b_05;

    void splitlong(i32 nl);
  };

  struct s_wwbb // size: 0x6 - was s_grrr
  {
    i16 w_00;
    i16 w_02;
    i8 b_04;
    i8 b_05;
  };

  struct s_pppw
  {
    i16* wp_00;
    i16* wp_04;
    i16* wp_08;
    i16 w_0c;
  };

  union u_lp // TODO: see union u_spec
  {
    unk* xp_00; // can have different types!
    i32 l_00;
  };

  union u_spec // TODO: see union u_lp
  {
    s_hex6* s19p_00;
    i32 l_00;
  };

  class c_ulp
  {
    protected:
      u_lp* ulptable;

    //private:
	public:
      u_lp* getadr(ui32 uln) const;

    public:
      c_ulp(void);
      void init(void);
      unk* getp(ui32 uln) const;
      void setp(ui32 uln, unk* xp);
      void setl(ui32 uln, i32 nl);
      bool islen(ui32 uln) const;
      i32 DM2_QUERY_GDAT_RAW_DATA_LENGTH(t_dbidx dbidx);
      void R_2D87A(i16 index);
  };

  class c_cpx_linklist
  {
    private:
      s_lpp* ptrlist;

    public:
      void init(void) { ptrlist = NULL; }
      bool is_empty(void) const { return ptrlist == NULL; }
      s_lpp* get_ptrlist(void) const { return ptrlist; }

      void DM2_ALLOC_CPX_LINK_NODE(s_lpp* lpptr);
      void DM2_ALLOC_CPX_UNLINK_NODE(s_lpp* lpptr);
      i32 DM2_ALLOC_CPX1(OO unk*& xptr, i32 amount);
  };

  extern c_ulp dm2_ulp;

  struct s_bwb // size: 0x4
  {
    i8 b_00;
    i16 w_01;
    i8 filler;
  };

  struct s_pwwp // size: 0xc
  {
    unk* xp_00;
    i16 w_04;
    i16 w_06;
    union
    {
      s_pwwp* s58p_08;
      s_bwb s57_08;
    };
  };

  class c_dballochandler : public c_allochandler,
                           public c_cpx_linklist
  {
    public:
      static const t_text tidspfg[8];

    protected: // public:
      i32 cpx_available;
      s_xmalloc* malloce;
      s_xmalloc* mallocf;
      s_xmalloc* mallocg;
      s_xmalloc* malloch;
      void* bigpool_endofunpreservedfree;
      i16 mallock;
      t_dbidx mallocm;
      i16 num_gfx_tableentries; // was num_gfx_tableentries
      s_xmalloc** gfx_table; // index of this one has type t_dbidx, was s_table
      t_dbidx* w_table1;
      t_dbidx malloc_bmpidx;
      i16 malloco; // counts up in DM2_dballoc_2E52C, down in DM2_dballoc_3e74_5561
      i16 preserved_table_entries;
      t_dbidx* preservedgfx_idxtable; // was dbidxtable3
      ui32* preservedgfx_table; // index of this one has type t_dbidx, was ul_table3
      s_pppw pppw1;
      s_pppw pppw2;
      i16 preserved_bigpool;
      t_dbidx* w_table2;
      bool mallocw;
      bool v1d70d7;
      i16 v1e13f8;
      t_dbidx dbidx; // was v1d6781
      i32 v1d6783;
      i16 v1d6773;
      i16 v1e0abe;
      unk* ptr1e09a8;
      i16 v1e09ae;
      i16 v1e09b0;
      i16 v1e09b2[7];
      i8 v1e09c0[7];
      i16 v1d6793;
      unk* ptr1e0a5c;
      i8 v1d6787[8 + 4];
      i16 v1e09ac;
      i8 v1d6770;
      ui16 v1e0ad8;
      i32 v1d6775;
      i32 v1e0a74;
      i32 v1e0a8c;

    public:
      bool gfxalloc_done; // set one time only, never reset, public
      i8* filebuffer; // public
      i32 v1e0a90;
      bool v1e0200;
      bool v1e0a84;
      i16 v1e0ad4;
      ui16 v1e0ad6;
      s_pwwp* v1dff38;
      i16 v1e0acc;
      i32 malloci;
      s_1e09e0 gdattable;
      i16 v1e13fa;
      i8 v1e13fe[0xa]; // TODO: probably dim just 0x5
      i32 v1e0a0c;

    protected:
      void DM2_dballoc_3e74_5561(t_dbidx dbidx);
      ui32 DM2_ALLOC_GENERATE_ID(void);
      t_dbidx DM2_dballoc_2E52C(void);
      bool DM2_dballoc_38c8_0109(unk** xeaxpp, i32* edxpl, i16* ebxpw);
      i32 DM2_dballoc_38c8_0224(bool eaxbool, i32 edxl);
      void DM2_dballoc_3e74_24b8(void);
      bool DM2_dballoc_3e74_2162(i16 eaxw);
      void DM2_SET_PPPW_ENTRY(s_pppw* eaxp_s46, i16* edxpw, i32 ebxl, i16 ecxw);

    public: // protected:
      void DM2_ALLOCATION1(t_dbidx dbidx, bool edxbool);
      void DM2_ALLOCATION2(t_dbidx dbidx);
      void DM2_ALLOCATION3(t_dbidx dbidx);
      t_dbidx DM2_ALLOC_CPX_GET_WTABLE_ENTRY(t_dbidx dbidx);
      t_dbidx DM2_ADD_TO_PRESERVED_GFX(ui32 eaxul, i16 index);
      bool DM2_FIND_IN_PRESERVED_GFX(ui32 eaxul, OO i16& edxw);
      s_xmalloc* DM2_ALLOCATION6(t_dbidx dbidx);
      void DM2_ALLOCATION7(t_dbidx dbidx, bool edxbool);
      t_dbidx DM2_ALLOC_CPXHEAP_CREATE_INDEX(void);
      void DM2_GUARANTEE_FREE_CPXHEAP_SIZE(i32 amount);
      void DM2_INIT_GFX_TABLE(void);
      s_xmalloc* DM2_ALLOC_CPXHEAP_CREATE_POINTER(i32 amount);
      void DM2_ALLOCATION10(void);

    public:
      void init(void);

      void DM2_ALLOC_CPX_SETUP(unk* xeaxp);
      void DM2_SETUP_DB_ALLOCATION(void);
      void DM2_DEALLOC_BIGPOOL_STRUCT_BEFORE(s_malloctail* s78p);
      s_xmalloc* DM2_ALLOC_CPXHEAP_MEM(t_dbidx dbidx, i32 amount);
      s_xmalloc* DM2_ALLOCATION9(t_dbidx dbidx, i32 edxl);
      i32 DM2_ALLOCATION11(ui32 eaxul, i32 ebxl, OO t_dbidx& dbidx);
      void DM2_dballoc_2636_03d4(void);
      bool DM2_dballoc_3e74_55f9(ui32 eaxul, OO t_dbidx& dbidx);
      i8 DM2_dballoc_3e74_2439(i8 eaxb, i8 edxb);
      t_dbidx DM2_dballoc_3e74_5888(void);
      void DM2_dballoc_3e74_58a3(t_dbidx dbidx);
      void DM2_dballoc_3e74_5992(i8 eaxb, i8 edxb, i8 ebxb);
      i16 DM2_ALLOC_NEW_CREATURE(i16 eaxw, i16 edxw, i16 ebxw, i16 ecxw, i16 argw0);
      void DM2_ALLOCATE_GFX256(t_dbidx dbidx);
      void DM2_ALLOCATE_GFX16(i8 eaxb, i8 edxb, i8 ebxb);
      t_bmp* DM2_GET_BMP(/*TAG bmpid*/ t_dbidx dbidx);

    friend void DM2_LOAD_DYN4(unk* xeaxp, i32 edxl);
    friend c_pixel* DM2_EXTRACT_GDAT_IMAGE(t_dbidx dbidx, i32 edxl);
    friend void DM2_READ_GRAPHICS_STRUCTURE(void);
    friend static void DM2_LOAD_ENT1(void);
    friend static void DM2_LOAD_GDAT_ENTRIES(void);
    friend static void DM2_BUILD_GDAT_ENTRY_DATA(s_1e09e0* eaxp_s81, unk* xebxp);
    friend void DM2_FREE_PICT_ENTRY(c_pixel* gfx);
    friend static i32 DM2_QUERY_GDAT_ENTRY_VALUE(i16 eaxw, i16 edxw);
    friend void DM2_LOAD_GDAT_RAW_DATA(t_dbidx dbidx, unk* xedxp);
    friend static i32 DM2_QUERY_GDAT_RAW_DATA_FILE_POS(t_dbidx dbidx);
  };

  extern c_dballochandler dm2_dballochandler;

#endif
