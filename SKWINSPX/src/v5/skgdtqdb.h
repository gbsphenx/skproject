#ifndef C_QUERYDB_H
  #define C_QUERYDB_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  #ifndef C_ALLOC_H
    #include <skalloc.h>
  #endif

  #ifndef C_GFX_BMP_H
    #include <gfxbmp.h>
  #endif

  struct s_8bytearray
  {
    i8 barr_00[0x8];
  };

  struct s_8wordarray
  {
    i16 warr_00[0x8];
  };

  class c_imgdesc;
  class c_image;
  struct s_bbw;
  class c_5bytes;
  class c_hero;
  class t_palette;

  void DM2_query_098d_000f(i16 eaxw, i16 edxw, i16 ebxw, OO i16& w1, OO i16& w2);
  bool DM2_IS_CLS1_CRITICAL_FOR_LOAD(i8 eaxb);
  unk* DM2_QUERY_GDAT_DYN_BUFF(t_dbidx dbidx_in, IO t_dbidx& dbidx_out, bool ebxbool);
  s_bbw* DM2_QUERY_GDAT_ENTRYPTR(i8 eaxb, i8 edxb, i8 ebxb, i8 ecxb);
  i16 DM2_QUERY_GDAT_ENTRY_DATA_INDEX(i8 eaxb, i8 edxb, i8 ebxb, i8 ecxb);
#define LextendedDM2_QUERY_GDAT_ENTRY_DATA_INDEX(z1, z2, z3, z4) mkl(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(z1, z2, z3, z4))
  c_pixel* DM2_QUERY_GDAT_IMAGE_ENTRY_BUFF(i8 eaxb, i8 edxb, i8 ebxb);
  t_uniptr DM2_QUERY_GDAT_ENTRY_DATA_PTR(i8 eaxb, i8 edxb, i8 ebxb, i8 ecxb);
  i32 DM2_QUERY_GDAT_ENTRY_DATA_LENGTH(i8 eaxb, i8 edxb, i8 ebxb, i8 ecxb);
  unk* DM2_QUERY_GDAT_ENTRY_DATA_BUFF(i8 eaxb, i8 edxb, i8 ebxb, i8 ecxb);
  bool DM2_QUERY_GDAT_ENTRY_IF_LOADABLE(i8 eaxb, i8 edxb, i8 ebxb, i8 ecxb);
  i16 DM2_QUERY_GDAT_PICT_OFFSET(i8 eaxb, i8 edxb, i8 ebxb);
  t_palette* DM2_QUERY_GDAT_IMAGE_LOCALPAL(i8 eaxb, i8 edxb, i8 ebxb);
  t_text* DM2_QUERY_GDAT_ITEM_NAME(i32 eaxl, i32 edxl);
  i32 DM2_QUERY_CMDSTR_TEXT(t_text* eaxtp, const t_text* edxtp);
  t_text* DM2_QUERY_CMDSTR_NAME(i8 eaxb, i8 edxb, i8 ebxb);
  i32 DM2_QUERY_CMDSTR_ENTRY(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl);
  i32 DM2_QUERY_CUR_CMDSTR_ENTRY(i32 eaxl);
  t_bmp* DM2_QUERY_PICT_BITS(c_imgdesc* imgdesc);
  i32 DM2_QUERY_GDAT_IMAGE_METRICS(i8 eaxb, i8 edxb, i8 ebxb, i16* ecxpw, i16* argpw0);
  i32 DM2_QUERY_ORNATE_ANIM_FRAME(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl);
  t_bmp* DM2_QUERY_PICST_IMAGE(c_imgdesc* imgdesc);
  i32 DM2_QUERY_4BPP_PICT_BUFF_AND_PAL(i32 eaxl, i32 edxl, c_pixel** xebxp, t_palette* ecxpcarr);
  i16 DM2_QUERY_PLAYER_SKILL_LV(i16 heroidx, i16 edxw, i32 ebxl);
  i32 DM2_IS_WALL_ORNATE_ALCOVE(i32 eaxl);
  i32 DM2_IS_TILE_BLOCKED(i32 eaxl);
  i32 DM2_IS_REBIRTH_ALTAR(unk* xeaxp);
  i32 DM2_QUERY_DOOR_DAMAGE_RESIST(i32 eaxl);
  i32 DM2_QUERY_GDAT_CREATURE_WORD_VALUE(i32 eaxl, i32 edxl);

  i32 DM2_IS_WALL_ORNATE_SPRING(i32 eaxl);
  i32 DM2_IS_MISCITEM_CURRENCY(i32 eaxl);
  i32 DM2_IS_CONTAINER_MONEYBOX(i32 eaxl);
  i32 DM2_IS_CONTAINER_MAP(i32 eaxl);
  bool DM2_IS_CONTAINER_CHEST(i16 eaxw);
  i32 DM2_IS_DISTINCTIVE_ITEM_ON_ACTUATOR(unk* xeaxp, i32 edxl, i32 ebxl);
  i32 DM2_IS_MISSILE_VALID_TO_LAUNCHER(i32 eaxl, i32 edxl, i32 ebxl);
  i32 DM2_GET_CREATURE_WEIGHT(i32 eaxl);
  i32 DM2_IS_CREATURE_ALLOWED_ON_LEVEL(i32 eaxl, i32 edxl);
  i32 DM2_GET_ORNATE_ANIM_LEN(unk* xeaxp, i32 edxl);

  i32 DM2_QUERY_GDAT_FOOD_VALUE_FROM_RECORD(i16 eaxw);
  i32 DM2_QUERY_CREATURES_ITEM_MASK(i32 eaxl, i32 edxl, unk* xebxp, i32 ecxl);
  i32 DM2_IS_ITEM_FIT_FOR_EQUIP(i16 eaxw, i32 edxl, i32 ebxl);
  void DM2_QUERY_MESSAGE_TEXT(t_text* eaxtp, i32 edxl, i32 ebxl);
  unk* DM2_GET_MISSILE_REF_OF_MINION(i32 eaxl, i32 edxl);
  i32 DM2_GET_CREATURE_AT(i32 eaxl, i32 edxl);

  i32 DM2_FIND_HAND_WITH_EMPTY_FLASK(c_hero* hero);
  i32 DM2_FIND_DISTINCTIVE_ITEM_ON_TILE(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl);
  i16 DM2_FIND_TILE_ACTUATOR(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl);
  i32 DM2_FIND_LADDAR_AROUND(i32 eaxl, i32 edxl, i32 ebxl, unk** xecxpp);

  c_image* DM2_QUERY_GDAT_SUMMARY_IMAGE(c_image* image, i8 edxb, i8 ebxb, i8 ecxb);

  i16 DM2_GET_PLAYER_AT_POSITION(i16 eaxw);
  i32 DM2_FIND_POUCH_OR_SCABBARD_POSSESSION_POS(i32 eaxl, i32 edxl);
  i32 DM2_DIR_FROM_5x5_POS(i32 eaxl);
  i32 DM2_GET_DOOR_STAT_0X10(i32 eaxl);
  i32 DM2_GET_GLOB_VAR(i32 eaxl);
  i32 DM2_GET_GRAPHICS_FOR_DOOR(i32 eaxl);
  i32 DM2_GET_ITEM_ORDER_IN_CONTAINER(i32 eaxl, i32 edxl);
  void DM2_COUNT_BY_COIN_TYPES(i32 eaxl, i16* edxpwarr);
  i32 DM2_RETRIEVE_ENVIRONMENT_CMD_CD_FW(unk* xeaxp);
  i32 DM2_CALC_PLAYER_WALK_DELAY(i32 eaxl);
  i16 DM2_COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(i16 heroidx, i32 edxl, i32 ebxl);
  void DM2_QUERY_TEMP_PICST(i16 eaxw, i16 edxw, i16 ebxw, i16 ecxw, i16 argw0, i16 argw1, i16 argw2, i16 argw3, i16 alphamask, i16 argw5, i8 argb6, i8 argb7, i8 argb8);

  void DM2_query_32cb_0804(t_palette* eaxpcarr, i32 edxl, i32 ebxl, i32 ecxl, i16* argpw0);
  void DM2_query_0b36_037e(t_palette* eaxpcarr, i8 edxb, i8 ebxb, i8 ecxb, i8 argb0, i16 argw1, i16 argw2, i16* argpw3);
  t_palette* DM2_CONVERT_PALETTE256(t_palette* eaxpcarr, i8 edxb, i8 ebxb, i8 ecxb, IO i16& colors);
  t_palette* DM2_query_B073(t_palette* eaxpcarr, i8 edxb, i16 ebxw, i16 ecxw, i16* argpw0);
  i32 DM2_query_4E26(i16* eaxpw);
  void DM2_query_4DA3(i32 eaxl, i32 edxl, i16* ebxpw, s_8bytearray* ecxp_s09);
  i32 DM2_query_1c9a_08bd(unk* xeaxp);
  i32 DM2_IS_CREATURE_FLOATING(i32 eaxl);
  i32 DM2_IS_OBJECT_FLOATING(i32 eaxl);
  i32 DM2_QUERY_OBJECT_5x5_POS(i32 eaxl, i32 edxl);
  i32 DM2_query_48ae_05ae(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i32 argl1);
  i16* DM2_query_1c9a_02c3(unk* xeaxp, unk* xedxp);
  i32 DM2_QUERY_CREATURE_5x5_POS(unk* xeaxp, i32 edxl);
  void DM2_QUERY_RAINFALL_PARAM(OO i8& a, OO i16& d);
  i32 DM2_query_0cee_0897(i16** eaxppw, i32 edxl, i32 ebxl);
  i32 DM2_GET_TELEPORTER_DETAIL(c_5bytes* eaxp_c12, i32 edxl, i32 ebxl);
  i32 DM2_IS_CREATURE_MOVABLE_THERE(i32 eaxl, i32 edxl, i32 ebxl, i16* ecxpw);
  i32 DM2_query_0cee_1a46(s_8wordarray* eaxp_s15, i32 edxl, i32 ebxl, i32 ecxl);
  i32 DM2_query_48ae_011a(i32 eaxl);
  i32 DM2_query_0cee_2e09(i32 eaxl);
  i32 DM2_query_1c9a_03cf(i16* eaxpw, i16* edxpw, i32 ebxl);
  i32 DM2_query_0cee_2df4(i32 eaxl);
  i32 DM2_query_48ae_01af(i32 eaxl, i32 edxl);
  i32 DM2_query_0cee_2e35(i32 eaxl);
  void DM2_QUERY_CREATURE_PICST(i32 eaxl, i32 edxl, unk* xebxp, unk* xecxp, i16 argw0);
  i32 DM2_query_2fcf_164e(i32 eaxl, i32 edxl);
  i32 DM2_query_2fcf_16ff(i32 eaxl);
  i32 DM2_query_48ae_0767(i32 eaxl, i32 edxl, i16* ebxpw, unk* xecxp);
  i32 DM2_query_2759_0155(i32 eaxl);
  i32 DM2_query_2759_01fe(i32 ebxl, i32 edxl);
  i32 DM2_IS_ITEM_HAND_ACTIVABLE(i32 eaxl, i32 edxl, i32 ebxl);
  void DM2_DRAW_HAND_ACTION_ICONS(i16 eaxw, i32 edxl, i32 ebxl);
  i32 DM2_query_0cee_06dc(i32 eaxl, i32 edxl);
  i32 DM2_query_19f0_124b(i16* eaxpw, i16* edxpw, i32 ebxl, i32 ecxl, i32 argl0);
  void DM2_query_29ee_18eb(i32 eaxl, i32 edxl, i32 ebxl);
  i16 DM2_QUERY_CREATURE_BLIT_RECTI(i16 n, t_rotate rotate, i16 wb);
  i32 DM2_query_0cee_3275(i32 eaxl);
  i32 DM2_QUERY_DOOR_STRENGTH(i32 eaxl);
  i32 DM2_query_0cee_319e(i32 eaxl);

// casting uniptr
  #define QUERY_colconv(a) CHGCAST(t_colconv, a)
  #define QUERY_pixel(a) CHGCAST(c_pixel, a)

#endif
