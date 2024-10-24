#ifndef CALLS_H
 #define CALLS_H

 #ifndef TYPES_H
   #include "types.h"
 #endif

 #ifndef STARTEND_H
   #include "src/startend.h"
 #endif

 #ifndef UTIL_H
   #include "src/util.h"
 #endif

 #ifndef FILEIO_H
   #include "src/fileio.h"
 #endif

 #ifndef C_RECT_H
   #include "src/c_rect.h"
 #endif

 #ifndef C_ALLOC_H
   #include "src/c_alloc.h"
 #endif

 #ifndef C_SOUND_H
   #include "src/c_sound.h"
 #endif

#include "dm2global.h"


const char* getSpellTypeName(ui8 spelltype);
const char* getSkillName(ui8 skill);
const char* getStatBonusName(ui8 bonus);
const char* getUIEventName(ui8 event);
const char* getAIName(ui8 ai);
//const char* getRecordNameOf(ObjectID recordLink);

 t_gfxdata* R_D2C(x16 eaxw, x16 ebxw, x16 edxw);
 x16 SKW_476d_030a(x16 eaxl);
 x16 R_1456(void); // unreferenced - calls user interrupt
 bool SKW_476d_04af(x16 eaxw);
 void R_148D(x16 dummy);
 bool R_1539(void); // unreferenced
 x16 SKW_476d_05b6(x16 n); // calls user interrupt
 void R_1583(x8 eaxb); // unreferenced
 void SKW_COPY_TO_DRIVER(x8* buffer, x16 amount); // calls user interrupt
 void SKW_44c8_1aca(t_gfxdata* eaxpg, x16 ebxw, x16 ecxw, c_rect* edxr);
 void SKW_44c8_1baf(x8* eaxp);
 void SKW_44c8_1c99(void); // calls user interrupt
 bool SKW_44c8_1e1a(x8* eaxpb, x16 edxw); // unreferenced
 ui16 SKW_44c8_1e43(t_gfxdata* eaxpg, t_gfxdata* ebxpg, c_rect* ecxr, t_gfxdata* edxpg, ui16 arguw0, ui16 arguw1, x16 argw2, x16 argw3, x16 argw4, x16 argw5, x8 argb6, t_palette* palette);
 ui16 SKW_44c8_20a4(t_gfxdata* eaxpg, t_gfxdata* ebxpg, c_rect* ecxr, t_gfxdata* edxpg, ui16 arguw0, ui16 arguw1, x16 argw2, x8 argb3, t_gfxdata* argpg4);
 void SKW_44c8_2351(t_gfxdata* eaxpg, x16 ebxw, x16 ecxw, t_gfxdata* edxps, x16 argw0, x16 argw1);
 void SKW_443c_0004(t_gfxdata* eaxpg, t_gfxdata* ebxpg, t_gfxdata* edxpg);
 x16 SKW_443c_00f8(x16 eaxw, x16 edxw);
 void SKW_443c_067a(s_munknown* eaxps);
 void SKW_443c_0434(void);
 void SKW_CHAMPION_SQUAD_RECOMPUTE_POSITION(void);
 void SKW_CHANGE_PLAYER_POS(x16 eaxw); // calls user interrupt
 void SKW_443c_06b4(s_munknown* eaxps);
 void SKW_443c_07d5(s_munknown* eaxps);
 void SKW_443c_087c(void); // calls user interrupt
 void SKW_443c_0889(void); // calls user interrupt
 // removed: void R_4D44(x16 a, x16 b); // unreferenced - calls user interrupt
 void SKW_443c_08ab(x16* eaxpw, x16* ebxpw, x16* edxpw); // calls user interrupt
 void R_4DA3(x8 eaxb, x16* ebxpw, s_8bytearray* ecxps, x16 edxw);
 void R_4DEA(x8 eaxb, x16* ebxpw, x32* ecxpl, x16 edxw);
 x16 R_4E26(x16* eaxpw);
 x16 GET_ANIM_INFO_SEQUENCE(x8 eaxb, x16 edxw);	// R_4EA8
 bool SKW_GET_CREATURE_ANIMATION_FRAME(x8 eaxb, x16* ebxpw, x16* ecxpw, x16 edxw, t_gfxdata** argppg0, x16 argw1);
 bool R_4FCC(x8 eaxb, x16* ebxpw, t_gfxdata** ecxppg, x16 edxw);
 x16 R_50CB(x8 eaxb, x16* ebxpw, t_gfxdata** ecxppg, x16 edxw);
 x16 SKW_GET_GLOB_VAR(x16 eaxw);
 x16 SKW_UPDATE_GLOB_VAR(ui16 eaxuw, ui16 ebxuw, bool edxbool);
 x8 SKW_48ae_011a(x16 eaxw);
 x8 SKW_48ae_01af(x16 eaxw, x16 edxw);
 x16 SKW_IS_ITEM_FIT_FOR_EQUIP(x16 eaxw, bool ebxbool, x16 edxw);
 bool SKW_QUERY_CREATURES_ITEM_MASK(x8 eaxb, s_x40bytes* ebxps, bool ecxbool, x8 edxb);
 void SKW_LOAD_MISCITEM(void);
 x16 SKW_GET_ITEM_ORDER_IN_CONTAINER(x16 eaxw, x16 edxw);
 x16 SKW_48ae_05ae(x16 eaxw, x16 ebxw, x16 ecxw, x8 edxb, x16 argw0, x16 argw1);
 x16 SKW_48ae_0767(x16 eaxw, t_gfxdata* ebxpg, x16* ecxpw, x16 edxw);
 x16 SKW_DIR_FROM_5x5_POS(x16 eaxw);
 x16 SKW_QUERY_OBJECT_5x5_POS(x16 eaxw, x16 edxw);
 void R_5BFB(x16 eaxw, x16 edxw);
 void SKW_SHOOT_ITEM(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x8 argb0, x8 argb1, x8 argb2, x8 argb3);
 x16 SKW_075f_0182(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
 x16 SKW_CREATE_CLOUD(ui16 eaxuw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0);
 void SKW_075f_056c(x16 eaxw);
 void SKW_DELETE_MISSILE_RECORD(x16 eaxw, x16 ebxw, x16 ecxw, x16* edxpw);
 x16 SKW_075f_06bd(c_record* eaxprec, x16 edxw);
 bool SKW_075f_084d(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1);
 bool SKW_075f_0af9(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0);
 void SKW_STEP_MISSILE(c_tim* eaxptim);
 x16 SKW_CALC_CLOUD_DAMAGE(x16 eaxw, x16 edxw);
 void SKW_PROCESS_CLOUD(c_tim* eaxptim);
 void SKW_075f_1bc2(s_4bytearray* eaxp4ba, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1);
 x16 SKW_CREATURE_ATTACKS_PLAYER(c_record* eaxprec, x16 edxw);
 x16 SKW_CALC_PLAYER_ATTACK_DAMAGE(c_hero* eaxphero, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2, x16 argw3, x16 argw4);
 bool SKW_ATTACK_DOOR(x16 eaxw, x16 ebxw, bool ecxbool, x16 edxw, x16 argw0);
 void SKW_098d_000f(x16 eaxw, x16 ebxw, x16* ecxpw, x16 edxw, x16* argpw0);
 void SKW_LOAD_RECTS_AND_COMPRESS(x8 eaxb, x8 ebxb, x8 edxb);
 void SKW_098d_1208(void);
 x16 SKW_0aaf_0067(x8 eaxb);
 void SKW_0aaf_01db(x16 eaxw, bool edxbool);
 x8 SKW_0aaf_02f8(x8 eaxb, x8 edxb);
 void R_AB26(void);
 t_gfxdata* SKW_QUERY_PICST_IMAGE(c_hex18* eaxps);
 t_gfxdata* SKW_0b36_00c3(x16 eaxw, c_hex18* edxps);
 t_gfxdata* SKW_QUERY_PICT_BITMAP(c_hex18* eaxps);
 void SKW_0b36_018f(c_hex18* eaxps);
 void SKW_0b36_01cd(c_hex18* eaxps);
 void SKW_LOAD_GDAT_INTERFACE_00_02(void);
 t_gfxdata* R_B012(t_gfxdata* eaxpg, x8 ebxb, x8 ecxb, x8 edxb, x16* argpw0);
 t_gfxdata* R_B073(t_gfxdata* eaxpg, x16 ebxw, x16 ecxw, x8 edxb, x16* argpw0);
 void SKW_0b36_037e(t_gfxdata* eaxpg, x8 ebxb, x8 ecxb, x8 edxb, x8 argb0, x16 argw1, x16 argw2, x16* argpw3);
 c_image* SKW_QUERY_GDAT_SUMMARY_IMAGE(c_image* eaxpimg, x8 ebxb, x8 ecxb, x8 edxb);
 ui32 SKW_0b36_068f(c_image* eaxpimg);
 c_image* SKW_QUERY_PICST_IT(c_image* eaxpimg);
 void SKW_DRAW_PICST(c_image* eaxpimg);
 void SKW_0b36_0c52(c_c5rects* eaxps, bool ebxbool, x16 edxw);
 void SKW_0b36_0cbe(c_c5rects* eaxps, bool edxbool);
 void SKW_0b36_105b(c_c5rects* eaxps, x16 ebxw, c_rect* edxr);
 void SKW_DRAW_ICON_PICT_BUFF(t_gfxdata* srcbmp, c_rect* ebxr, x16 ecxw, c_c5rects* edxps, x16 argw0, x16 argw1, x16 argw2, t_palette* palette);
 void SKW_DRAW_ICON_PICT_ENTRY(x8 eaxb, x8 ebxb, c_c5rects* ecxps, x8 edxb, x16 argw0, x16 argw1);
 void SKW_0b36_11c0(c_image* eaxpimg, x16 ebxw, x16 ecxw, c_c5rects* edxps);
 void SKW_0b36_129a(c_c5rects* eaxps, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, t_text* argpt1);
 x16 SKW_0b36_1446(c_hex18* eaxps, x16 ebxw, x16 ecxw, x16 edxw);
 void SKW_DRAW_STATIC_PIC(x8 eaxb, x8 ebxb, x16 ecxw, x8 edxb, x16 argw0);
 void R_C470(t_gfxdata* srcbmp, x16 ebxw, t_palette* palette, x16 edxw);
 void SKW_DRAW_TRANSPARENT_STATIC_PIC(x8 eaxb, x8 ebxb, x16 ecxw, x8 edxb, x16 argw0);
 void SKW_RAISE_SYSERR(x16 eaxw);
 void SKW_CHANGE_CURRENT_MAP_TO(x16 eaxw);
 void SKW_LOAD_LOCALLEVEL_GRAPHICS_TABLE(x16 eaxw, x16 ebxw, x16 edxw);
 x16* SKW_OVERSEE_RECORD(x16* eaxpw, x16** ebxpp, FPTR_b_WPWP ecxf, x8 edxb, x16* argpw0, x16 argw1, x16 argw2);
 void SKW_CALC_VECTOR_W_DIR(x16 eaxw, x16 ebxw, x16* ecxpw, x16 edxw, x16* argpw0);
 bool SKW_IS_TILE_PASSAGE(x16 eaxw, x16 edxw);
 x8 SKW_GET_TILE_VALUE(x16 eaxw, x16 edxw);
 x8 SKW_0cee_04e5(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0);
 x16 SKW_LOCATE_OTHER_LEVEL(x16 eaxw, x16* ebxpw, x16* ecxpw, x16 edxw, x8** argpp0);
 x16 SKW_0cee_06dc(x16 eaxw, x16 edxw);
 x16 SKW_FIND_LADDAR_AROUND(x16 eaxw, x16 ebxw, c_record** ecxpp, x16 edxw);
 x8 SKW_0cee_0897(x16** eaxppw, x16 ebxw, x16 edxw);
 bool SKW_GET_TELEPORTER_DETAIL(c_5bytes* eaxps, x16 ebxw, x16 edxw);
 c_record* R_D283(x16 eaxw, x16 edxw);
 c_record* SKW_GET_ADDRESS_OF_TILE_RECORD(x16 eaxw, x16 edxw);
 c_record* SKW_GET_ADDRESS_OF_RECORD(ui16 eaxw);
 x16 SKW_GET_NEXT_RECORD_LINK(x16 eaxw);
 x16 SKW_GET_OBJECT_INDEX_FROM_TILE(x16 eaxw, x16 edxw);
 x16 SKW_GET_TILE_RECORD_LINK(x16 eaxw, x16 edxw);
 x16 SKW_GET_WALL_TILE_ANYITEM_RECORD(x16 eaxw, x16 edxw);
 void SKW_APPEND_RECORD_TO(x16 eaxw, x16 ebxw, x16 ecxw, x16* edxpw);
 void SKW_CUT_RECORD_FROM(x16 eaxw, x16 ebxw, x16 ecxw, x16* edxpw);
 void SKW_SET_ITEM_IMPORTANCE(x16 eaxw, x16 edxw);
 x16 SKW_RECYCLE_A_RECORD_FROM_THE_WORLD(x16 eaxw, x8 edxb);
 x16 SKW_ALLOC_NEW_RECORD(x16 eaxw);
 x16 SKW_ALLOC_NEW_DBITEM(x16 eaxw);
 x16 SKW_ALLOC_NEW_DBITEM_DEEPLY(x16 eaxw);
 void SKW_DEALLOC_RECORD(x16 eaxw);
 void SKW_QUERY_MESSAGE_TEXT(t_text* eaxt, x16 ebxw, x16 edxw);
 x16 SKW_0cee_17e7(x16 eaxw, x16 ebxw, x16 edxw);
 x8 SKW_0cee_1815(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0);
 void SKW_0cee_185a(s_8wordarray* eaxps, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2, x16 argw3);
 x16 SKW_QUERY_ORNATE_ANIM_FRAME(x8 eaxb, x32 ebxl, x8 ecxb, x8 edxb);
 x16 SKW_0cee_1a46(s_8wordarray* eaxps, x16 ebxw, bool ecxbool, x16 edxw);
 void SKW_SUMMARIZE_STONE_ROOM(s_8wordarray* eaxps, x16 ebxw, x16 ecxw, x16 edxw);
 x16 SKW_GET_DISTINCTIVE_ITEMTYPE(x16 eaxw);
 x8 SKW_QUERY_CLS2_FROM_RECORD(x16 eaxw);
 void SKW_SET_ITEMTYPE(x16 eaxw, x8 edxb);
 x16 SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(x16 eaxw, x8 edxb);
 x16 SKW_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(x16 eaxw);
 x8 SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(x16 eaxw);
 x8 SKW_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(x16 eaxw);
 x8 SKW_QUERY_CLS1_FROM_RECORD(x16 eaxw);
 bool SKW_IS_CONTAINER_MONEYBOX(x16 eaxw);
 bool SKW_IS_CONTAINER_CHEST(x16 eaxw);
 bool SKW_IS_MISCITEM_CURRENCY(x16 eaxw);
 void SKW_COUNT_BY_COIN_TYPES(x16 eaxw, x16* edxpwarr);
 bool SKW_ADD_COIN_TO_WALLET(x16 eaxw, x16 edxw);
 x16 SKW_TAKE_COIN_FROM_WALLET(x16 eaxw, x16 edxw);
 x8 SKW_GET_MAX_CHARGE(x16 eaxw);
 x16 SKW_ADD_ITEM_CHARGE(x16 eaxw, x16 edxw);
 x16 SKW_QUERY_GDAT_FOOD_VALUE_FROM_RECORD(x16 eaxw);
 x16 SKW_QUERY_ITEM_VALUE(x16 eaxw, x8 edxb);
 x16 SKW_QUERY_ITEM_WEIGHT(x16 eaxw);
 x16 R_F958(x16 eaxw);
 c_aispec* SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(x16 eaxw);
 c_aispec* SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(x8 eaxb);
 //x16 SKW_QUERY_GDAT_CREATURE_WORD_VALUE(x8 eaxb, x8 edxb);
 x16 SKW_QUERY_GDAT_CREATURE_WORD_VALUE(XCLS16 eaxb, x8 edxb);	// SPX fix to avoid negative values
 x16 SKW_QUERY_CREATURE_AI_SPEC_FLAGS(x16 eaxw);
 x16 SKW_0cee_2df4(x16 eaxw);
 x16 SKW_0cee_2e09(x16 eaxw);
 x16 SKW_GET_CREATURE_WEIGHT(x16 eaxw);
 x16 SKW_0cee_2e35(x8 eaxb);
 bool SKW_IS_CREATURE_ALLOWED_ON_LEVEL(x16 eaxw, x16 edxw);
 bool SKW_IS_CONTAINER_MAP(x16 eaxw);
 bool SKW_SET_DESTINATION_OF_MINION_MAP(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
 c_record* SKW_GET_MISSILE_REF_OF_MINION(x16 eaxw, x16 edxw);
 x8 SKW_QUERY_CLS2_OF_TEXT_RECORD(x16 eaxw);
 x8 SKW_GET_WALL_DECORATION_OF_ACTUATOR(c_record* eaxprec);
 x8 SKW_GET_FLOOR_DECORATION_OF_ACTUATOR(c_record* eaxprec);
 x16 SKW_IS_WALL_ORNATE_ALCOVE(x8 eaxb);
 bool SKW_IS_WALL_ORNATE_ALCOVE_FROM_RECORD(x16 eaxw, x16 edxw);
 x16 SKW_0cee_317f(x16 eaxw, x16 edxw);
 bool SKW_0cee_319e(x16 eaxw);
 x16 SKW_IS_WALL_ORNATE_SPRING(x16 eaxw);
 x8 SKW_IS_REBIRTH_ALTAR(c_record* eaxprec); // TODO correct name here? smth with doors
 x16 SKW_GET_GRAPHICS_FOR_DOOR(x8 eaxb);
 x16 SKW_0cee_3275(x8 eaxb);
 x16 SKW_GET_DOOR_STAT_0X10(x8 eaxb);
 x16 SKW_QUERY_DOOR_DAMAGE_RESIST(x8 eaxb);
 x16 SKW_QUERY_DOOR_STRENGTH(x8 eaxb);
 void SKW_END_GAME(bool eaxbool);
 bool SKW_RETURN_1(void);
 bool SKW_IS_GAME_ENDED(s_bbw* eaxpbbw);
 bool SKW_1031_0023(s_bbw* eaxpbbw);
 bool SKW_1031_003e(s_bbw* eaxpbbw);
 bool SKW_1031_007b(s_bbw* eaxpbbw);
 bool SKW_1031_009e(s_bbw* eaxpbbw);
 bool SKW_1031_00c5(s_bbw* eaxpbbw);
 bool SKW_1031_00f3(s_bbw* eaxpbbw);
 bool SKW_1031_012d(s_bbw* eaxpbbw);
 bool SKW_1031_014f(s_bbw* eaxpbbw);
 bool SKW_1031_0184(s_bbw* eaxpbbw);
 bool SKW_1031_01ba(s_bbw* eaxpbbw);
 c_rect* SKW_1031_01d5(x16 eaxw, c_rect* edxr);
 x8* SKW_1031_023b(s_bbw* eaxpbbw);
 s_www* SKW_1031_024c(s_bbw* eaxpbbw);
 void SKW_1031_027e(s_bbw* eaxpbbw);
 x16 SKW_1031_030a(s_bbw* eaxpbbw, x16 ebxw, x16 ecxw, x16 edxw);
 x16 SKW_1031_03f2(s_bbw* eaxpbbw, x16 edxw);
 void SKW_1031_04F5(void);
 void SKW_1031_050C(void);
 void R_10777(void);
 void R_107B0(void);
 void SKW_1031_0541(x16 eaxw);
 void SKW_1031_0675(x16 eaxw);
 void SKW_1031_06a5(void);
 s_www* SKW_1031_06b3(s_bbw* eaxpbbw, x16 edxw);
 void SKW_1031_0781(x16 eaxw);
 void SKW_1031_07d6(void);
 x16 SKW_1031_0a88(s_www* eaxps, x16 ebxw, x16 ecxw, x16 edxw);
 bool SKW_1031_0c58(x16 eaxw, s_www* edxps);
 void SKW_1031_0d36(x16 eaxw, x16 edxw);
 const x8* SKW_TRANSMIT_UI_EVENT(c_event* eaxps);
 void SKW_IBMIO_USER_INPUT_CHECK(void);
 void SKW_1031_10c8(c_c5rects* eaxps, x16 ebxw, x16 ecxw, c_rect* edxr);
 void SKW_1031_111e(x16 eaxw);
 void SKW_ADJUST_UI_EVENT(c_event* eaxps);
 void SKW_SELECT_CHAMPION_LEADER(x16 eaxw);
 void SKW_CLICK_MAGICAL_MAP_AT(x16 eaxw, x16 ebxw, x16 edxw);
 void SKW_DRAW_WAKE_UP_TEXT(void);
 bool SKW_HANDLE_UI_EVENT(c_event* eaxps);
 void SKW_EVENT_LOOP(void);
 void SKW_121e_0003(ui16 eaxuw);
 void SKW_PLAYER_TESTING_WALL(x16 eaxw, x16 ebxw, x16 edxw);
 void SKW_121e_013a(x16 eaxw, x16 ebxw, x16 edxw);
 bool SKW_121e_0222(x16 eaxw, x16 ebxw, x16 edxw);
 void SKW_121e_0351(x16 eaxw, x16 edxw);
 bool SKW_121e_03ae(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2);
 void SKW_CLICK_VWPT(x16 eaxw, x16 edxw);
 void R_13262(x16 eaxw);
 void SKW_HIGHLIGHT_ARROW_PANEL(x8 eaxb, bool ebxbool, x16 edxw);
 void SKW_12b4_0092(void);
 void SKW_12b4_00af(x16 eaxw);
 void SKW_RESET_SQUAD_DIR(void);
 void SKW_PERFORM_TURN_SQUAD(x16 eaxw);
 void SKW_12b4_023f(x16 eaxw, x16* ebxpw, x16* ecxpw, x16 edxw, x16 argw0, x16 argw1);
 bool SKW_PERFORM_MOVE(x16 eaxw);
 x16 SKW_12b4_0881(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16* argpw1);
 bool SKW_12b4_0953(c_record* eaxprec, x16 edxw);
 bool SKW_12b4_099e(x16 eaxw);
 void SKW_PUSH_PULL_RIGID_BODY(x16 eaxw);
 bool SKW_IS_CREATURE_MOVABLE_THERE(x16 eaxw, x16 ebxw, x16* ecxpw, x16 edxw);
 void SKW_12b4_0d75(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
 void SKW_LOAD_NEWMAP(x16 eaxw, x16 ebxw, bool ecxbool, x16 edxw);
 c_350* SKW_13e4_0004(x16 eaxw, x8 ebxb, x16 ecxw, x8 edxb);
 void SKW_13e4_01a3(void);
 void SKW_13e4_0329(c_350* eaxps);
 void SKW_13e4_0360(x16 eaxw, x16 ebxw, x8 ecxb, x16 edxw, bool argbool0);
 void SKW_ATTACK_CREATURE(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1);
 void SKW_13e4_071b(void);
 void SKW_13e4_0806(void);
 void SKW_ANIMATE_CREATURE(x16 eaxw, bool ebxbool, x16 edxw);
 void SKW_13e4_0982(void);
 void SKW_THINK_CREATURE(x16 eaxw, x16 ebxw, x16 edxw);
 void SKW_SELECT_CREATURE_37FC(void);
 x16 SKW_14cd_0067(const s_wpm* eaxps);
 void SKW_14cd_0276(c_sizex16* eaxps);
 x8 SKW_14cd_0389(void);
 x8 SKW_14cd_0457(void);
 void SKW_14cd_0550(struct s_anientry* eaxpani, x8 ebxb, bool ecxbool, x8 edxb);
 x8 SKW_14cd_062e(void);
 x8 SKW_14cd_0684(void);
 void SKW_14cd_0802(void);
 x8 SKW_DECIDE_NEXT_XACT(void);
 bool SKW_14cd_08f5(x8 eaxb);
 void SKW_14cd_09e2(void);
 void SKW_14cd_0f0a(x8 eaxb, x8 ebxb, s_hexe* ecxphe, x8 edxb);
 void SKW_14cd_0f3c(x8 eaxb, const s_hexe* ebxphe, x8 ecxb, s_hexe* edxphe, x8 argb0, x16 argw1, x8 argb2, x8 argb3);
 x16 SKW_14cd_102e(x16 eaxw, x8 ebxb, bool ecxbool, x16 edxw, bool argbool0);
 s_hex20* SKW_14cd_10d2(s_hexe* eaxps, x8 edxb);
 bool SKW_14cd_1316(x8 eaxb, x8 ebxb, x16 edxw);
 void SKW_14cd_18cc(x8 eaxb, s_hexe* ebxphe, x8 edxb);
 void SKW_14cd_18f2(x8 eaxb, s_hexe* ebxphe, x8 ecxb, x8 edxb, x16 argw0);
 void SKW_14cd_19a4(x8 eaxb, s_hexe* ebxphe, x8 edxb);
 void SKW_14cd_19c2(x8 eaxb, s_hexe* ebxphe, x8 ecxb, x8 edxb, x8 argb0);
 void SKW_14cd_1a3c(x8 eaxb, s_hexe* ebxphe, x8 edxb);
 void SKW_14cd_1a5a(x8 eaxb, s_hexe* ebxphe, x8 edxb);
 void SKW_14cd_1a78(x8 eaxb, s_hexe* ebxphe, x8 ecxb, x8 edxb);
 void SKW_14cd_1b74(x8 eaxb, s_hexe* ebxphe, x8 edxb);
 void SKW_14cd_1b90(x8 eaxb, s_hexe* ebxphe, x8 edxb);
 void SKW_14cd_1bac(x8 eaxb, s_hexe* ebxphe, x8 ecxb, x8 edxb, x8 argb0);
 void SKW_14cd_1c27(x8 eaxb, s_hexe* ebxphe, x8 edxb);
 void SKW_14cd_1c45(x8 eaxb, s_hexe* ebxphe, x8 edxb);
 void SKW_14cd_1c63(s_hexe* ebxphe, x8 edxb);
 void SKW_14cd_1c8d(x8 eaxb, s_hexe* ebxphe, x8 edxb);
 void SKW_14cd_1cec(s_hexe* ebxphe, x8 edxb);
 void SKW_14cd_1d42(s_hexe* ebxphe, x8 edxb);
 void SKW_14cd_1d6c(x8 eaxb, s_hexe* ebxphe, x8 ecxb, x8 edxb);
 void SKW_14cd_1e36(x8 eaxb, s_hexe* ebxphe, x8 edxb);
 void SKW_14cd_1e52(x8 eaxb, s_hexe* ebxphe, x8 edxb);
 void SKW_14cd_1e6e(x8 eaxb, s_hexe* ebxphe, x8 edxb);
 void SKW_14cd_1eec(x8 eaxb, s_hexe* ebxphe, x8 ecxb, x8 edxb);
 void SKW_14cd_1f8b(x8 eaxb, s_hexe* ebxphe, x8 edxb);
 void SKW_14cd_1fa7(s_hexe* ebxphe, x8 edxb);
 x8 SKW_PROCEED_XACT(x8 eaxb);
 x8 SKW_PROCEED_XACT_75(void);
 void SKW_PROCEED_XACT_79(void);
 x8 SKW_PROCEED_XACT_56(void);
 void SKW_PROCEED_XACT_57(void);
 x8 SKW_PROCEED_XACT_59_76(void);
 void SKW_PROCEED_XACT_69(void);
 x8 SKW_PROCEED_XACT_64(void);
 x8 SKW_PROCEED_XACT_70(void);
 x8 SKW_PROCEED_XACT_63(void);
 bool SKW_14cd_2662(x8 eaxb);
 x8 SKW_PROCEED_XACT_66(void);
 bool SKW_14cd_2807(x16* eaxpw, x16* edxpw); // called indirect
 x16 SKW_14cd_2886(x16* eaxpw, x8 ebxb, x16 ecxw, x16 edxw, x16 argw0, x16 argw1);
 x8 SKW_PROCEED_XACT_67(void);
 x16 SKW_FIND_TILE_ACTUATOR(x16 eaxw, x8 ebxb, x16 ecxw, x16 edxw);
 x8 SKW_PROCEED_XACT_62(void);
 x8 SKW_PROCEED_XACT_65(void);
 x8 SKW_PROCEED_XACT_68(void);
 x8 SKW_PROCEED_XACT_71(void);
 x8 SKW_PROCEED_XACT_73(void);
 x8 SKW_PROCEED_XACT_74(void);
 x8 SKW_PROCEED_XACT_77(void);
 x8 SKW_PROCEED_XACT_78(void);
 x8 SKW_PROCEED_XACT_80(void);
 x8 SKW_PROCEED_XACT_81(void);
 void SKW_14cd_3582(x16 eaxw, x16 edxw);
 x8 SKW_PROCEED_XACT_82(void);
 x8 SKW_PROCEED_XACT_83(void);
 x8 SKW_PROCEED_XACT_84(void);
 x8 SKW_PROCEED_XACT_85(void);
 x8 SKW_PROCEED_XACT_86(void);
 void SKW_PROCEED_XACT_72_87_88(void);
 x8 SKW_PROCEED_XACT_89(void);
 x8 SKW_PROCEED_XACT_90(void);
 x8 SKW_PROCEED_XACT_91(void);
 void SKW_OPERATE_PIT_TELE_TILE(x16 eaxw, x16 ebxw, x16 edxw);
 bool SKW_CREATURE_WALK_NOW(void);
 bool SKW_1887_0205(void);
 bool SKW_1887_0239(void);
 bool SKW_1887_034e(void);
 void SKW_1887_05aa(void);
 bool SKW_CREATURE_ATTACKS_PARTY(void);
 bool SKW_1887_09ab(void);
 bool SKW_1887_0b1b(void);
 bool SKW_1887_0c75(void);
 bool SKW_1887_0ce1(void);
 bool SKW_1887_0d33(void);
 void SKW_1887_0df7(void);
 bool SKW_1887_0e19(void);
 bool SKW_1887_0eca(void);
 bool SKW_1887_0f80(void);
 bool SKW_PLACE_MERCHANDISE(void);
 bool SKW_1887_10db(void);
 bool SKW_1887_1191(void);
 void SKW_1887_126d(void);
 bool SKW_1887_141c(void);
 x16 R_1B7D5(void);
 bool SKW_PROCEED_CCM(void);
 bool R_1BA1B(x16 eaxw, x16 edxw); // also called indirect
 bool R_1BAAD(x16 eaxw, x16 edxw); // also called indirect
 bool R_1BC29(x16 eaxw, x16 edxw);
 x16 SKW_19f0_0207(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, FPTR_b_WW argf0);
 void SKW_19f0_045a(x16 eaxw, x16 edxw);
 x16 SKW_19f0_04bf(void);
 x16 SKW_19f0_050f(void);
 x8 SKW_19f0_0547(x16 eaxw, x16 edxw);
 bool SKW_19f0_0559(x16 eaxw);
 bool SKW_19f0_05e8(x16 eaxw, x16* ebxpw, x16 ecxw, t_gfxdata* edxpg, x16 argw0, x16 argw1, bool argbool2);
 bool SKW_19f0_0891(x8 eaxb, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1);
 bool SKW_19f0_0d10(x8 eaxb, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1);
 x16 SKW_19f0_124b(x16* eaxpw, x16 ebxw, x16 ecxw, x16* edxpw, x16 argw0);
 bool SKW_19f0_13aa(x16 eaxw, x16 edxw);
 x8 SKW_19f0_1511(x16 eaxw);
 bool SKW_CREATURE_GO_THERE(x8 eaxb, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1);
 x16 SKW_19f0_2024(x16 eaxw, x16 ebxw, x16 edxw);
 bool SKW_19f0_2165(x8 eaxb, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2);
 x16 SKW_19f0_266c(x16 eaxw, x8 ebxb, x16 ecxw, x16 edxw);
 bool SKW_19f0_2723(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
 bool SKW_19f0_2813(x8 eaxb, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2);
 x8 SKW_1c9a_0006(x16 eaxw, x16 edxw);
 void SKW_1c9a_0247(x16 eaxw);
 x16* SKW_1c9a_02c3(c_record* eaxprec, c_aispec* edxps);
 void SKW_CHECK_RECOMPUTE_LIGHT(void);
 x16 SKW_GET_CREATURE_AT(x16 eaxw, x16 edxw);
 x16 SKW_1c9a_03cf(x16* eaxpw, x16 ebxw, x16* edxpw);
 x16 SKW_COMPUTE_POWER_4_WITHIN(x16 eaxw, x16 edxw);
 x16 SKW_1c9a_0598(x16 eaxw);
 bool R_1FE74(x32 eaxl, x32 ebxl, x32 ecxl, x32 edxl, x16 argl0); // unreferenced
 void SKW_1c9a_0648(x16 eaxw);
 bool SKW_1c9a_0694(x16* eaxpw, x16* edxpw); // called indirect
 x16* SKW_1c9a_06bd(x16 eaxw, x8 ebxb, x16 edxw);
 x16 SKW_1c9a_0732(x16 eaxw, x8 ebxb, x16 edxw);
 void SKW_1c9a_078b(x16* eaxpw, x8 ebxb, x16 edxw);
 bool SKW_1c9a_08bd(c_record* eaxprec);
 bool SKW_IS_CREATURE_FLOATING(x16 eaxw);
 bool SKW_1c9a_0958(x16 eaxw);
 bool SKW_1c9a_09b9(x16 eaxw, x16 edxw);
 void SKW_1c9a_09db(c_record* eaxprec);
 x32 SKW_CREATURE_SOMETHING_1c9a_0a48(void);
 void SKW_1c9a_0cf7(x16 eaxw, x16 edxw);
 void SKW_1c9a_0db0(x16 eaxw);
 void SKW_ALLOC_CAII_TO_CREATURE(x16 eaxw, x16 ebxw, x16 edxw);
 void SKW_1c9a_0fcb(x16 eaxw);
 x16 SKW_CREATE_MINION(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2, x8 argb3);
 void SKW_RELEASE_MINION(x16 eaxw);
 x16 SKW_ALLOC_NEW_CREATURE(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0);
 void SKW_DROP_CREATURE_POSSESSION(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0);
 void SKW_DELETE_CREATURE_RECORD(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
 x16 SKW_CREATURE_ATTACKS_CREATURE(x16 eaxw, x16 edxw);
 bool SKW_1c9a_17c7(x16 eaxw, x16 ebxw, x16 edxw);
 bool SKW_WOUND_CREATURE(x16 eaxw);
 x16 SKW_APPLY_CREATURE_POISON_RESISTANCE(x16 eaxw, x16 edxw);
 void SKW_1c9a_19d4(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
 x16 SKW_1c9a_1a48(x16 eaxw, x16 edxw);
 x16 SKW_1c9a_1b16(x16 eaxw, x16 edxw);
 bool SKW_1c9a_1bae(x16 eaxw, x16 edxw); // also called indirect
 x8 SKW_1c9a_1bd8(x16 eaxw, x8 ebxb, x16 edxw, x16 argw0, c_sizex16* argp1, u_bbw* argp2);
 void SKW___SET_CURRENT_THINKING_CREATURE_WALK_PATH(void);
 x16 SKW_1c9a_381c(void);
 bool SKW_1c9a_38a8(void);
 void SKW_FILL_CAII_CUR_MAP(void);
 void SKW_FILL_ORPHAN_CAII(void);
 void SKW_RESET_CAII(void);
 void SKW_1c9a_3c30(void);
 bool SKW_2066_03e0(x16 eaxw);
 void SKW_ADD_INDEX_TO_POSSESSION_INDICES(x16 eaxw);
 void SKW_2066_1ea3(x16 eaxw, x16 ebxw, x16 edxw);
 x16 SKW_2066_1ec9(x16 eaxw, x16 edxw);
 bool SKW_2066_1f37(x16 eaxw, x16* ebxpw, x16 edxw);
 void SKW_ARRANGE_DUNGEON(void);
 void SKW_2066_2498(void);
 bool SKW_READ_DUNGEON_STRUCTURE(bool eaxbool);
 void SKW_2066_33c4(t_text* eaxt, t_text edxt);
 x16 SKW_2066_33e7(void);
 void SKW__OPEN_DIALOG_PANEL(x8 eaxb, x16 edxw);
 void SKW_2066_37f2(void);
 void SKW_2066_3820(t_text* eaxt, bool edxbool);
 void SKW_2066_38d9(void);
 void SKW_2066_398a(x16 eaxw);
 void SKW_2405_0009(void);
 t_text* SKW_QUERY_GDAT_ITEM_NAME(x8 eaxb, x8 edxb);
 t_text* SKW_GET_ITEM_NAME(x16 eaxw);
 void SKW_DISPLAY_TAKEN_ITEM_NAME(x16 n);
 void SKW_2405_00ec(x16 eaxw, c_rect* edxr);
 void SKW_2405_011f(x16 eaxw, c_rect* edxr);
 x8 SKW_2405_014a(x16 eaxw, bool ebxbool, x16 edxw);
 void SKW_DRAW_ITEM_ICON(x16 eaxw, x8 ebxb, bool ecxbool, x16 edxw, bool argbool0);
 void SKW_DRAW_ITEM_IN_HAND(c_wpc* eaxpwpc);
 t_gfxdata* SKW_DRAW_ITEM_ON_WOOD_PANEL(x16 eaxw, c_hex18* ebxps, x16 edxw);
 void SKW_2481_0002(void);
 void SKW_SHOW_MENU_SCREEN(void);
 void SKW_SHOW_CREDITS(void);
 x16 SKW_RECALC_LIGHT_LEVEL(void);
 void SKW_ADD_BACKGROUND_LIGHT_FROM_TILE(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0);
 void SKW_BURN_PLAYER_LIGHTING_ITEMS(void);
 void SKW_24a5_0732(x16 eaxw, t_text* ebxt, x16 edxw);
 void SKW_DRAW_SCROLL_TEXT(x16 eaxw);
 x16 SKW_PUT_OBJECT_INTO_CONTAINER(x16 eaxw);
 void SKW__CHECK_ROOM_FOR_CONTAINER(x16 eaxw, c_record* edxprec);
 void SKW_MONEY_BOX_SURVEY(x16 eaxw);
 void SKW_DRAW_CONTAINER_SURVEY(c_record* eaxp);
 void SKW_DRAW_ITEM_STATS_BAR(x16 eaxw, x16 ebxw, x8 ecxb, x16 edxw, x16 argw0);
 bool SKW_DRAW_ITEM_SURVEY(x16 eaxw, bool edxbool);
 void SKW_24a5_0e82(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, bool argbool1);
 void SKW_DRAW_FOOD_WATER_POISON_PANEL(void);
 void SKW_DRAW_CRYOCELL_LEVER(bool eaxbool);
 bool SKW_IS_MISCITEM_DRINK_WATER(x16 eaxw);
 void SKW_PLAYER_CONSUME_OBJECT(x16 eaxw, x16 ebxw, x16 edxw);
 void SKW_24a5_1532(void);
 void SKW_CLICK_INVENTORY_EYE(void);
 void SKW_24a5_1798(x16 eaxw);
 t_text* SKW_FORMAT_SKSTR(const t_text* eaxt, t_text* edxt);
 t_text* SKW_QUERY_GDAT_TEXT(x8 eaxb, x8 ebxb, t_text* ecxt, x8 edxb);
 void DM2_2636_03d4(void);
 void SKW_MARK_DYN_LOAD(ui32 eaxul);
 void SKW_MARK_DYN_LOAD_2676_006a(x32 eaxl);					// SKW_2676_006a renamed SKW_MARK_DYN_LOAD_2676_006a
 void SKW_MARK_DYN_LOAD_2676_008f(x32 eaxl, x8 edxb);			// SKW_2676_008f renamed SKW_MARK_DYN_LOAD_2676_008f
 void SKW_MARK_DYN_LOAD_2676_00d0(x8 eaxb, x8 ebxb, x8 edxb);	// SKW_2676_00d0 renamed SKW_MARK_DYN_LOAD_2676_00d0
 void SKW_LOAD_LOCALLEVEL_DYN(void);
 void SKW_FREE_PICT_ENTRY(t_gfxdata* eaxpg);
 t_gfxdata* SKW_ALLOC_PICT_BUFF(x16 eaxw, x16 ebxw, x8 ecxb, x16 edxw);
 void SKW_FREE_PICT_BUFF(t_gfxdata* eaxpg);

// x16 DM2_QUERY_GDAT_ENTRY_DATA_INDEX(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb);
x16 DM2_QUERY_GDAT_ENTRY_DATA_INDEX(x8 eaxb, XCLS16 ebxb, XCLS16 ecxb, XCLS16 edxb);// SPX fix to avoid negative values


 //bool SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb);
 bool SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(x8 eaxb, XCLS16 ebxb, XCLS16 ecxb, XCLS16 edxb);// SPX fix to avoid negative values
 //x8 SKW_3e74_2439(x8 eaxb, x8 edxb);
 ui16 SKW_3e74_2439(x8 eaxb, x8 edxb); // SPX fix to avoid negative values 
 void DM2_GRAPHICS_DATA_OPEN(void);
 void DM2_GRAPHICS_DATA_CLOSE(void);
 void SKW_READ_GRAPHICS_STRUCTURE(void);
 void SKW_LOAD_DYN4(s_hex6* eaxps, x16 edxw);
 //t_gfxdata* SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(x8 eaxb, x8 ebxb, x8 edxb);
 t_gfxdata* SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(x8 eaxb, XCLS16 ebxb, XCLS16 edxb); // SPX fix to avoid negative values
 //x8* SKW_QUERY_GDAT_ENTRY_DATA_PTR(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb);
 x8* SKW_QUERY_GDAT_ENTRY_DATA_PTR(x8 eaxb, XCLS16 ebxb, XCLS16 ecxb, XCLS16 edxb); // SPX fix to avoid negative values
 //x8* SKW_QUERY_GDAT_ENTRY_DATA_BUFF(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb);
 x8* SKW_QUERY_GDAT_ENTRY_DATA_BUFF(x8 eaxb, XCLS16 ebxb, XCLS16 ecxb, XCLS16 edxb);// SPX fix to avoid negative values
 //void SKW_LOAD_GDAT_ENTRY_DATA_TO(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb, x8* argpb0);
 void SKW_LOAD_GDAT_ENTRY_DATA_TO(x8 eaxb, XCLS16 ebxb, XCLS16 ecxb, XCLS16 edxb, x8* argpb0);	// SPX fix to avoid negative values
 //t_palette* SKW_QUERY_GDAT_IMAGE_LOCALPAL(x8 eaxb, x8 ebxb, x8 edxb);
 t_palette* SKW_QUERY_GDAT_IMAGE_LOCALPAL(x8 eaxb, XCLS16 ebxb, XCLS16 edxb);	//  SPX fix to avoid negative values
 //x16 SKW_QUERY_GDAT_PICT_OFFSET(x8 eaxb, x8 ebxb, x8 edxb);
 x16 SKW_QUERY_GDAT_PICT_OFFSET(x8 eaxb, XCLS16 ebxb, XCLS16 edxb);		//  SPX fix to avoid negative values
 bool SKW_3e74_55f9(ui32 eaxul, x16* edxpw);
 t_gfxdata* SKW_ALLOC_NEW_PICT(x16 eaxw, x16 ebxw, x8 ecxb, x16 edxw);
 t_gfxdata* SKW_3e74_5817(x16 eaxw);
 void SKW_3e74_583a(x16 eaxw);
 x16 SKW_3e74_5888(void);
 void SKW_3e74_58a3(x16 eaxw);
 void SKW_3e74_58bf(x8 eaxb, x8 ebxb, x8 edxb);
 void SKW_3e74_5992(x8 eaxb, x8 ebxb, x8 edxb);
 void R_2F5DE(t_gfxdata* eaxp, t_gfxdata* edxp); // unreferenced
 //x32 SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb);
 x32 SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(x8 eaxb, XCLS16 ebxb, XCLS16 ecxb, XCLS16 edxb);	// SPX fix to avoid negative values
 void SKW_QUERY_GDAT_IMAGE_METRICS(x8 eaxb, x8 ebxb, x16* ecxpw, x8 edxb, x16* argpw0);
 void SKW_3e74_5b7c(x16 eaxw);
 t_text* SKW_QUERY_CMDSTR_NAME(x8 eaxb, x8 ebxb, x8 edxb);
 ui16 SKW_QUERY_CMDSTR_TEXT(t_text* eaxt, const t_text* edxt);
 ui16 SKW_QUERY_CMDSTR_ENTRY(x8 eaxb, x8 ebxb, x16 ecxw, x8 edxb);
 bool SKW_2759_0155(x16 eaxw);
 x16 SKW_QUERY_CUR_CMDSTR_ENTRY(x16 eaxw);
 bool SKW_2759_01fe(x16 ebxw, x16 edxw);
 bool SKW_IS_ITEM_HAND_ACTIVABLE(x16 eaxw, x16 ebxw, x16 edxw);
 x16 SKW_FIND_POUCH_OR_SCABBARD_POSSESSION_POS(x16 eaxw, x16 edxw);
 x16 SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS(void);
 void SKW_ACTIVATE_ACTION_HAND(x16 eaxw, x16 edxw);
 void SKW_SET_SPELLING_CHAMPION(x16 eaxw);
 void SKW_UPDATE_RIGHT_PANEL(bool eaxbool);
 bool SKW_IS_MISSILE_VALID_TO_LAUNCHER(x16 eaxw, x16 ebxw, x16 edxw);
 bool SKW_2759_0e93(x16 eaxw, x16 ebxw, x16 edxw);
 void SKW_ADJUST_HAND_COOLDOWN(x16 eaxw, x16 ebxw, x16 edxw);
 void SKW_LOAD_PROJECTILE_TO_HAND(x16 eaxw, x16 edxw);
 bool R_30DEA(x16 eaxw);
 x32 SKW_PROCEED_COMMAND_SLOT(x16 eaxw);
 void SKW_2759_12e6(void);
 void SKW_CLICK_MAGICAL_MAP_RUNE(x16 eaxw);
 bool SKW_CONFUSE_CREATURE(x16 eaxw, x16 ebxw, x16 edxw);
 bool SKW_WIELD_WEAPON(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1);
 void SKW_2759_15d0(ui16 eaxuw, x16 edxw);
 x32 SKW_ENGAGE_COMMAND(x16 eaxw, x16 edxw);
 s_802* SKW_FIND_SPELL_BY_RUNES(t_text* eaxt);
 x16 SKW_FIND_HAND_WITH_EMPTY_FLASK(c_hero* eaxphero);
 x16 SKW_2759_222e(x16 eaxw, x16 ebxw, s_802* edxps);
 bool SKW_TRY_CAST_SPELL(void);
 void SKW_ADD_RUNE_TO_TAIL(x16 eaxw);
 void SKW_REMOVE_RUNE_FROM_TAIL(void);
 void SKW_CLICK_MONEYBOX(x16 eaxw);
 void SKW_29ee_000f(void);
 void SKW_29ee_00a3(bool eaxbool);
 void SKW_SHOW_ATTACK_RESULT(ui16 eaxuw);
 void SKW_DRAW_HAND_ACTION_ICONS(x16 eaxw, x8 ebxb, x16 edxw);
 void SKW_29ee_0396(void);
 void SKW_DRAW_SQUAD_SPELL_AND_LEADER_ICON(x16 eaxw, bool edxbool);
 void SKW_DRAW_PLAYER_NAME_AT_CMDSLOT(void);
 void SKW_DRAW_PLAYER_ATTACK_DIR(void);
 void SKW_DRAW_SPELL_TO_BE_CAST(bool eaxbool);
 void SKW_DRAW_SPELL_PANEL(void);
 void SKW_DRAW_CMD_SLOT(x16 eaxw, bool edxbool);
 void SKW_29ee_0b2b(void);
 x16 SKW_PROCEED_SPELL_FAILURE(x16 eaxw);
 x16 SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(x8 eaxb, t_gfxdata** ebxppg, t_gfxdata* ecxpg, x8 edxb);
 void SKW_DRAW_CHIP_OF_MAGIC_MAP(t_gfxdata* eaxpg, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, t_gfxdata* argpg1);
 void SKW_DRAW_MAP_CHIP(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x8 argb2, x16 argw3);
 void SKW_29ee_18eb(x16 eaxw, x16 ebxw, x16 edxw);
 void SKW_29ee_1946(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2, x16 argw3);
 void SKW_29ee_1d03(x16 eaxw);
 void SKW_DRAW_MAJIC_MAP(x16 eaxw);
 void SKW_DRAW_MONEYBOX(x16 eaxw);
 void SKW_DRAW_CONTAINER_PANEL(x16 eaxw, bool edxbool);
 void SKW_2c1d_0009(x16 eaxw, x16 ebxw, x16 ecxw, x8 edxb);
 bool SKW_2c1d_0186(c_hero* eaxphero, x16 ebxw, x16 ecxw, x8 edxb);
 void SKW_ROTATE_SQUAD(x16 eaxw);
 x16 SKW_GET_PLAYER_AT_POSITION(x16 eaxw);
 x16 SKW_2c1d_028c(x16 eaxw, x16 ebxw, x16 edxw);
 void SKW_2c1d_0300(c_hero* eaxphero, x16 ebxw, x16 edxw);
 x16 SKW_RETRIEVE_ITEM_BONUS(x16 eaxw, x16 ebxw, x16 ecxw, x8 edxb);
 void SKW_PROCESS_ITEM_BONUS(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
 void SKW_PUT_ITEM_TO_PLAYER(x16 eaxw);
 void SKW_TAKE_OBJECT(x16 eaxw, bool edxbool);
 x16 SKW_REMOVE_OBJECT_FROM_HAND(void);
 x16 SKW_REMOVE_POSSESSION(x16 eaxw, x16 edxw);
 void SKW_EQUIP_ITEM_TO_HAND(x16 eaxw, x16 ebxw, x16 edxw);
 void SKW_CLICK_ITEM_SLOT(x16 eaxw);
 x16 SKW_2c1d_09d9(void);
 ui16 SKW_QUERY_PLAYER_SKILL_LV(x16 eaxw, bool ebxbool, x16 edxw);
 void SKW_ADJUST_SKILLS(x16 eaxw, x16 ebxw, x16 edxw);
 x16 SKW_2c1d_0e23(x16 eaxw);
 x16 SKW_STAMINA_ADJUSTED_ATTR(c_hero* eaxphero, x16 edxw);
 x16 SKW_GET_PLAYER_ABILITY(c_hero* eaxphero, x16 ebxw, x16 edxw);
 x16 SKW_2c1d_0f2a(c_hero* eaxphero, x16 ebxw, x16 edxw);
 bool SKW_USE_LUCK_ATTRIBUTE(c_hero* eaxphero, x16 edxw);
 x16 SKW_MAX_LOAD(c_hero* eaxphero);
 x16 SKW_CALC_PLAYER_WALK_DELAY(x16 eaxw);
 x16 SKW_USE_DEXTERITY_ATTRIBUTE(x16 eaxw);
 x16 SKW_COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(x16 eaxw, ui16 ebxuw, x16 edxw);
 x16 SKW_2c1d_132c(x16 eaxw, x16 edxw);
 x16 SKW_2c1d_135d(x16 eaxw, x16 edxw);
 void SKW_RESUME_FROM_WAKE(void);
 x8 SKW_DROP_PLAYER_ITEMS(x16 eaxw);
 void R_36EFE(x16 eaxw);
 void SKW_PLAYER_DEFEATED(x16 eaxw);
 void SKW_PROCESS_PLAYERS_DAMAGE(void);
 x16 SKW_WOUND_PLAYER(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
 void SKW_PROCESS_POISON(x16 eaxw, x16 edxw);
 void SKW_2c1d_1bb5(x16 eaxw, x16 edxw);
 x16 SKW_ATTACK_PARTY(x16 eaxw, x16 ebxw, x16 edxw);
 void SKW_ADJUST_STAMINA(x16 eaxw, x16 edxw);
 void SKW_SHOOT_CHAMPION_MISSILE(c_hero* eaxphero, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0);
 bool SKW_CAST_CHAMPION_MISSILE_SPELL(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
 bool SKW_2c1d_1de2(x16 eaxw, x16 ebxw, x16 edxw);
 bool R_37BBB(x16 eaxw);
 x8 R_37BEA(x16 eaxw);
 x16 SKW_GET_PARTY_SPECIAL_FORCE(void);
 x16 SKW_GET_PLAYER_WEIGHT(x16 eaxw);
 void SKW_CALC_PLAYER_WEIGHT(x16 eaxw);
 void SKW_UPDATE_CHAMPIONS_STATS(void);
 t_gfxdata* SKW_QUERY_GDAT_SQUAD_ICON(t_gfxdata* eaxpg, t_gfxdata* ebxpgarr, x16 edxw);
 void SKW_DRAW_PLAYER_3STAT_PANE(x16 eaxw, bool edxbool);
 void SKW_DRAW_PLAYER_3STAT_HEALTH_BAR(x16 eaxw);
 void SKW_DRAW_CUR_MAX_HMS(x16 eaxw, x16 ebxw, x16 edxw);
 void SKW_DRAW_PLAYER_3STAT_TEXT(c_hero* eaxphero);
 bool SKW_2e62_03b5(x16 eaxw, bool ebxbool, x16 edxw);
 void SKW_DRAW_PLAYER_DAMAGE(x16 eaxw);
 void SKW_DRAW_EYE_MOUTH_COLORED_RECTANGLE(x8 eaxb, x16 edxw);
 void SKW_2e62_061d(x16 eaxw);
 void SKW_DRAW_CHARSHEET_OPTION_ICON(x8 eaxb, x16 ebxw, x16 edxw);
 void SKW_REFRESH_PLAYER_STAT_DISP(x16 eaxw);
 void SKW_SET_PARTY_HERO_FLAG(x16 eaxw);
 void SKW_2e62_0cfa(bool eaxbool);
 void SKW_GLOBAL_UPDATE_UNKNOW1(void);
 void SKW_ADD_ITEM_TO_PLAYER(x16 eaxw, x16 edxw);
 void SKW_REVIVE_PLAYER(x8 eaxb, x8 ebxb);
 x16 SKW_SELECT_CHAMPION(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
 void R_39796(c_hero* eaxphero);
 void SKW_2f3f_04ea(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0);
 void SKW_BRING_CHAMPION_TO_LIFE(x16 eaxw);
 void SKW_2f3f_0789(void);
 void SKW_ROTATE_CREATURE(x16 eaxw, x16 ebxw, bool edxbool);
 x16 SKW_ROTATE_RECORD_BY_TELEPORTER(c_record* eaxprec, x16 edxw);
 bool SKW_IS_OBJECT_FLOATING(x16 eaxw);
 void SKW_SET_MINION_RECENT_OPEN_DOOR_LOCATION(x16 eaxw, x16 ebxw, x16 ecxw, x8 edxb, bool argbool0);
 void SKW_2fcf_01c5(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0);
 void SKW_2fcf_0234(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
 bool SKW_2fcf_0434(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, bool argbool1);
 void SKW_2fcf_0b8b(x16 eaxw, x16 ebxw, x16 edxw);
 void R_3B001(x16 eaxw, x16 edxw);
 bool SKW_IS_TILE_BLOCKED(x8 eaxb);
 bool SKW_TRY_PUSH_OBJECT_TO(x16 eaxw, x16 ebxw, x16* ecxpw, x16 edxw, x16* argpw0);
 x16 SKW_MOVE_RECORD_TO(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0);
 void SKW_INVOKE_MESSAGE(x16 eaxw, x8 ebxb, ui16 ecxuw, x16 edxw, x32 argl0);
 void SKW_INVOKE_ACTUATOR(c_record* eaxprec, x16 ebxw, x16 edxw);
 bool SKW_2fcf_164e(x16 eaxw, x16 edxw);
 x16 SKW_FIND_DISTINCTIVE_ITEM_ON_TILE(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
 bool SKW_2fcf_16ff(x16 eaxw);
 bool SKW_TRANSFER_PLAYER(x16 eaxw, x16 ebxw, x16 edxw); // unreferenced
 void R_3BF83(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
 bool SKW_LOAD_GDAT_INTERFACE_00_0A(void);
 void R_3C1E5(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0);
 void R_3CE7D(x16 eaxw, x16 ebxw, bool ecxbool, x16 edxw, bool argbool0, bool argbool1);
 x16 R_3D93B(ui16 eaxuw, x16 ebxw, x16* ecxp, x16 edxw, x16* argpw0);
 x16 SKW_QUERY_CREATURE_5x5_POS(c_record* eaxprec, x16 edxw);
 bool R_3DC4C(x16 eaxw);
 bool SKW_32cb_00f1(x16 eaxw, x16 ebxw, x16 edxw);
 bool SKW_32cb_01b6(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16* argpw0);
 bool SKW_32cb_0287(x16 eaxw, x16 ebxw, x16 edxw);
 x16 SKW_32cb_03a6(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2, bool argbool3);
 t_gfxdata* SKW_32cb_0649(x8 eaxb, x8 ebxb, x16 ecxw, x8 edxb);
 void R_3E39E(x8 eaxb, x8 ebxb, t_gfxdata* ecxpg, x8 edxb); // unreferenced
 void SKW_DRAW_DUNGEON_GRAPHIC(x8 eaxb, x8 ebxb, x16 ecxw, x8 edxb, x16 argw0, x16 argw1);
 void SKW_32cb_0804(t_gfxdata* eaxpg, x16 ebxw, x16 ecxw, x16 edxw, x16* argpw0);
 void SKW_QUERY_TEMP_PICST(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2, x16 argw3, x16 argw4, x16 argw5, x8 argb6, x8 argb7, x8 argb8);
 void SKW_DRAW_TEMP_PICST(void);
 x16 SKW_QUERY_MULTILAYERS_PIC(c_image* eaxpimg, x8 ebxb, x8 ecxb, x8 edxb, x16 argw0, x16 argw1, x16 argw2, x16 argw3, x16 argw4, x16 argw5);
 void SKW_32cb_0a4c(c_rect* eaxr, x8 ebxb, x8 edxb);
 void SKW_QUERY_RAINFALL_PARAM(x8* eaxpb, x16* edxpw);
 void SKW_DRAW_RAIN(void);
 void SKW_32cb_0c7d(c_image* eaxpimg, x16 ebxw, x16 edxw);
 void SKW_DRAW_TEXT_TO_BACKBUFF(x16 eaxw, t_text* ebxt, x16 edxw);
 bool SKW_IS_DISTINCTIVE_ITEM_ON_ACTUATOR(c_record* eaxprec, bool ebxbool, x16 edxw);
 void SKW_32cb_0f82(c_record* eaxprec, x16 ebxw, x16 ecxw, x8 edxb, x16 argw0, x16 argw1, x16 argw2, x16 argw3, x16 argw4);
 x8 SKW_SKCHR_TO_SCRIPTCHR(x8 eaxb);
 x16 SKW_32cb_15b8(x16 eaxw, bool ebxbool, x16 edxw);
 bool SKW_DRAW_EXTERNAL_TILE(ui16 eaxuw);
 void SKW_DRAW_PIT_ROOF(ui16 eaxuw);
 void SKW_DRAW_PIT_TILE(ui16 eaxuw);
 void SKW_DRAW_TELEPORTER_TILE(x16 eaxw, x8 ebxb, x8 edxb);
 void SKW_CHANCE_TABLE_OPERATION(void);
 void SKW_QUERY_CREATURE_PICST(x16 eaxw, c_record* ebxprec, s_d556* ecxpd5, x16 edxw, x16 argw0);
 bool SKW_SUMMARY_DRAW_CREATURE(x16 eaxw, x32 ebxl, x16 edxw);
 void SKW_32cb_2cf3(x8 eaxb, x16 ebxw, x16 ecxw, x16 edxw);
 void SKW_32cb_2d8c(x16 eaxw, x32 ebxl, x16 edxw);
 void SKW_DRAW_FLYING_ITEM(x16 eaxw, x16 ebxw, x16 edxw);
 void SKW_MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE(x16 eaxw, x8 ebxb, x8 ecxb, x16 edxw);
 bool SKW_32cb_35c1(x16* eaxpw, x16 ebxw, x16 ecxw, x16* edxpw);
 void SKW_DRAW_ITEM(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, c_record* argp1, bool argbool2, x16 argw3, bool argbool4);
 void SKW_DRAW_PUT_DOWN_ITEM(x16 eaxw, x16 ebxw, c_record* ecxprec, x16 edxw);
 void SKW_DRAW_STATIC_OBJECT(x16 eaxw, bool ebxbool, x32 edxl);
 void SKW_32cb_3e08(x16 eaxw, x32 ebxl, c_record* ecxprec, x16 edxw);
 void SKW_32cb_3edd(x16 eaxw);
 void SKW_32cb_3f0d(x16 eaxw);
 void SKW_32cb_4069(x16 eaxw, x16 ebxw, x16* ecxpw, x16 edxw, x16* argpw0);
 void SKW_32cb_4185(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
 void SKW_DRAW_DEFAULT_DOOR_BUTTON(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
 void SKW_DRAW_DOOR_FRAMES(x16 eaxw, x16 edxw); // BUGFIX 4.4/2014
 x16 SKW_32cb_48d5(x16 eaxw, x16 edxw);
 void SKW_DRAW_DOOR(x16 eaxw, x16 ebxw, x32 ecxl, x16 edxw);
 void SKW_DRAW_DOOR_TILE(ui16 eaxuw);
 void SKW_DRAW_STAIRS_FRONT(ui16 eaxuw);
 void SKW_DRAW_STAIRS_SIDE(ui16 eaxuw);
 void SKW_DRAW_WALL(x16 eaxw);
 void SKW_DRAW_WALL_TILE(ui16 eaxuw);
 void SKW_DRAW_DUNGEON_TILES(void);
 void SKW_DRAW_PLAYER_TILE(void);
 ui16 SKW_32cb_54ce(x16 eaxw, x16* ebxpw, x16 ecxw, x16* edxpw, x16 argw0);
 bool SKW_ENVIRONMENT_SET_DISTANT_ELEMENT(s_xabytes* eaxpxa, x16 ebxw, x16 ecxw, t_text* edxt, x16 argw0);
 void SKW_ENVIRONMENT_DRAW_DISTANT_ELEMENT(s_xabytes* eaxpxa, x16 ebxw, x16 ecxw, x16 edxw);
 void SKW_ENVIRONMENT_DISPLAY_ELEMENTS(x16 eaxw, x16 ebxw, x16 edxw);
 x16 SKW_SET_GRAPHICS_FLIP_FROM_POSITION(ui16 eaxuw, x16 ebxw, x16 ecxw, x16 edxw);
 void SKW_32cb_5a8f(void);
 void SKW_32cb_5c67(void);
 void SKW_DISPLAY_VIEWPORT(x16 eaxw, x16 ebxw, x16 edxw);
 void SKW_38c8_0002(void);
 void SKW_38c8_0060(void);
 void SKW_38c8_00c8(void);
 void SKW__INIT_GAME_38c8_03ad(void);
 x16 SKW_3929_04e2(const t_text* eaxp, x16* ebxpw, x16 ecxw, t_text* edxt);
 void SKW_3929_07e1(x16 eaxw, x16 edxw);
 x16 SKW_3929_0826(void); // unreferenced
 void SKW_3929_086f(void);
 void SKW_3929_0914(void);
 void SKW_3929_0929(x16 eaxw, t_text* edxt);
 void SKW_DISPLAY_HINT_TEXT(x16 eaxw, const t_text* edxt);
 void SKW_3929_0b01(x16 eaxw, x8 edxb);
 void SKW_3929_0b20(x16 eaxw, x32 edxl);
 void SKW_DISPLAY_HINT_NEW_LINE(void);
 void SKW_3929_0e16(void);
 void SKW_3a15_0002(void);
 void SKW___SORT_TIMERS(void);
 void SKW_3a15_020f(void);
 void SKW_REARRANGE_TIMERLIST(void);
 bool SKW_3a15_0381(c_tim* eaxptim, c_tim* edxptim);
 x16 SKW_GET_TIMER_NEW_INDEX(x16 eaxw);
 void SKW_3a15_0486(x16 eaxw);
 void SKW_3a15_05f7(x16 eaxw);
 void SKW_DELETE_TIMER(x16 eaxw);
 x16 SKW_QUEUE_TIMER(c_tim* eaxptim);
 void SKW_GET_AND_DELETE_NEXT_TIMER(c_tim* eaxptim);
 bool SKW_IS_TIMER_TO_PROCEED(void);
 void SKW_STEP_DOOR(c_tim* eaxptim);
 void SKW_ACTUATE_DOOR(c_tim* eaxptim);
 void SKW_ACTUATE_TRICKWALL(c_tim* eaxptim);
 void SKW_PROCESS_TIMER_DESTROY_DOOR(c_tim* eaxptim);
 void SKW_3a15_0d5c(c_tim* eaxptim, c_record* edxprec);
 void SKW_ACTIVATE_CREATURE_KILLER(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, x16 argw1, x16 argw2, x16 argw3);
 x16 SKW_GET_ORNATE_ANIM_LEN(c_record* eaxprec, bool edxbool);
 void SKW_TRY_ORNATE_NOISE(c_record* eaxprec, x16 ebxw, x16 ecxw, x16 edxw, x16 argw0, bool argbool1);
 void SKW_ACTIVATE_CONTINUOUS_ORNATE_ANIMATOR(x16 eaxw, c_record* ebxprec, bool ecxbool, c_tim* edxptim);
 void SKW_ACTIVATE_ORNATE_ANIMATOR(c_tim* eaxptim, x16 ebxw, x32 ecxl, c_record* edxprec);
 void SKW_ACTIVATE_RELAY2(c_tim* eaxptim, x16 ebxw, c_record* edxprec);
 void SKW_ACTIVATE_RELAY1(c_tim* eaxptim, bool ebxbool, c_record* edxprec);
 void SKW_ACTIVATE_INVERSE_FLAG(c_tim* eaxptim, c_record* edxprec);
 void SKW_ACTIVATE_TEST_FLAG(c_tim* eaxptim, c_record* edxprec);
 void SKW_ACTUATE_FLOOR_MECHA(c_tim* eaxptim);
 void SKW_ACTIVATE_SHOOTER(c_record* eaxprec, c_tim* edxptim);
 x16 SKW_3a15_1da8(x16 eaxw, x16 edxw);
 void SKW_MOVE_ITEM_TO(x16 eaxw, x16 ebxw, x16 ecxw, c_record* edxprec, x16 argw0, x16 argw1, x16 argw2, bool argbool3, x16 argw4);
 void SKW_ACTIVATE_ITEM_TELEPORT(c_tim* eaxptim, x16 ebxw, bool ecxbool, c_record* edxprec, bool argbool0, x16* argp1, bool argbool2, bool argbool3);
 void SKW_ACTUATE_WALL_MECHA(c_tim* eaxptim);
 void SKW_ADVANCE_TILES_TIME(x16 eaxw, x16 edxw);
 void SKW_ACTUATE_TELEPORTER(c_tim* eaxptim);
 void SKW_ACTUATE_PITFALL(c_tim* eaxptim);
 void SKW_PROCESS_TIMER_3D(c_tim* eaxptim);
 void SKW_PROCESS_TIMER_0C(x16 eaxw);
 void SKW_PROCESS_TIMER_RESURRECTION(c_tim* eaxptim);
 void SKW_PROCESS_TIMER_LIGHT(c_tim* eaxptim);
 void SKW_PROCESS_TIMER_0E(c_tim* eaxptim, x16 edxw);
 x16 SKW_CONTINUE_ORNATE_ANIMATOR(c_tim* eaxptim);
 void SKW_ACTIVATE_TICK_GENERATOR(c_record* eaxprec, x16 edxw);
 void SKW_CONTINUE_TICK_GENERATOR(c_tim* eaxptim);
 void SKW_PROCESS_ACTUATOR_TICK_GENERATOR(void);
 void SKW_PROCESS_TIMER_RELEASE_DOOR_BUTTON(c_tim* eaxptim);
 void SKW_PROCESS_TIMER_59(c_tim* eaxptim);
 void SKW_CONTINUE_ORNATE_NOISE(c_tim* eaxptim);
 void SKW_3a15_38b6(bool eaxbool);
 void SKW_PROCEED_TIMERS(void);
 x16 SKW_SET_TIMER_WEATHER(x32 eaxl);
 void SKW_3df7_0037(bool eaxbool);
 void SKW_UPDATE_WEATHER(bool eaxbool);
 bool SKW_RETRIEVE_ENVIRONMENT_CMD_CD_FW(s_xabytes* eaxpxa);
 t_gfxdata* R_4A986(x16 eaxw);
 void R_4A9A4(void);
 bool R_52DC0(x8* eaxpb, const t_text* edxt);

 void SKW_GAME_LOOP(void);
 void SKW_MAIN(void);
 void SKW_INIT(void);






#endif
