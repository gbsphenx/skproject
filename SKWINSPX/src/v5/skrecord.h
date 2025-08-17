#ifndef C_RECORD_H
  #define C_RECORD_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  typedef i16 t_record; // TODO: temporary. Later: (check adds on pointers!)
  // special values:
  #define OBJECT_NULL (-1)
  #define OBJECT_END_MARKER (-2)

  class c_aidef;

  struct s_rec // TODO: temporary struct for c_savegame.cpp only
  {
    i16 w_00; // not used, maybe bytes
    ui16 uw_02;
    union
    {
      ui16 uw_04;
      i8 b_04; // next byte not directly adressed
    };
    i16 w_06;
  };

  extern t_record* recordptr[16];
  extern const i8 table_recordsizes[16]; // 0x1d280c
  extern t_record* dm2_v1e038c;

  void init_global_records(void);

  t_record mk_record(i16 n);
  i16 record_to_word(t_record r);
  i32 record_to_long(t_record r);

  t_record* DM2_GET_ADDRESS_OF_RECORD(t_record r);
  t_record DM2_GET_NEXT_RECORD_LINK(t_record r);
  void DM2_APPEND_RECORD_TO(t_record eaxrec, t_record* edxrp, i16 x, i16 y);
  void DM2_CUT_RECORD_FROM(t_record eaxrec, t_record* edxrecp, i16 x, i16 y);

// ---------------------------------------------

  i16 DM2_GET_DISTINCTIVE_ITEMTYPE(i16 eaxw);
  i8 DM2_QUERY_CLS2_FROM_RECORD(i32 eaxl);
  void DM2_SET_ITEMTYPE(i32 eaxl, i32 edxl);
  i32 DM2_QUERY_GDAT_DBSPEC_WORD_VALUE(i32 eaxl, i32 edxl);
  i32 DM2_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(i32 eaxl);
  i32 DM2_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(i32 eaxl);
  i8 DM2_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(i32 eaxl);
  i8 DM2_QUERY_CLS1_FROM_RECORD(i32 eaxl);

  i32 DM2_GET_WALL_TILE_ANYITEM_RECORD(i32 eaxl, i16 edxw);
  void DM2_SET_ITEM_IMPORTANCE(i32 eaxl, i32 edxl);
  i32 DM2_RECYCLE_A_RECORD_FROM_THE_WORLD(i32 eaxl, i32 edxl);
  i32 DM2_ALLOC_NEW_RECORD(i32 eaxl);
  i32 DM2_ALLOC_NEW_DBITEM(i32 eaxl);
  i32 DM2_ALLOC_NEW_DBITEM_DEEPLY(i32 eaxl);
  void DM2_DEALLOC_RECORD(i32 eaxl);

  i8 DM2_QUERY_CLS2_OF_TEXT_RECORD(i32 eaxl);
  i8 DM2_GET_WALL_DECORATION_OF_ACTUATOR(unk* xeaxp);
  i8 DM2_GET_FLOOR_DECORATION_OF_ACTUATOR(unk* xeaxp);
  i32 DM2_IS_WALL_ORNATE_ALCOVE_FROM_RECORD(i32 eaxl, i32 edxl);

  c_aidef* DM2_QUERY_CREATURE_AI_SPEC_FROM_RECORD(i32 eaxl);
  unk* DM2_QUERY_CREATURE_AI_SPEC_FROM_TYPE(i32 eaxl);
  i16 DM2_QUERY_CREATURE_AI_SPEC_FLAGS(i32 eaxl);
  void DM2_DELETE_CREATURE_RECORD(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl);
  void DM2_DELETE_MISSILE_RECORD(i32 eaxl, unk* xedxp, i32 ebxl, i32 ecxl);
  void DM2_DROP_CREATURE_POSSESSION(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0);

  i16* DM2_OVERSEE_RECORD(i16* eaxpw, i32 edxl, i16** ebxppw, FPTR_L_WPWP ecxf, i16* argpw0, i32 argl1, i32 argl2);
  i32 DM2_ROTATE_RECORD_BY_TELEPORTER(unk* xeaxp, i32 edxl);

  void DM2_075f_056c(i32 eaxl);

#endif
