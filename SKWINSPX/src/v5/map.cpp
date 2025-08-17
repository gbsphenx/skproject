xxx* R_D2C(i32 eaxl, i32 ebxl, i32 edxl)
  RG1P = R_4A986(unsignedlong(RG1W) + lcon(0x6)) + lcon(0x6); // BUGFIX Phase P1
i16 SKW_476d_030a(i32 eaxl)
  RG1L = R_1278(signedlong(ddat.v1dffd2)); // TODO: readsectors?
  RG1W = word_at(SKW_GET_ADDRESS_OF_RECORD(RG1L), lcon(0x6));
  SKW_DELETE_TIMER(RG1L);
  RG1L = R_1278(RG2L); // TODO: readsectors?
  R_1296(RG4P); // TODO: findfirst
  RG1L = R_52DC0(ADRESSOF(i8, vba_00), RG4P);
bool SKW_476d_04af(i32 eaxl)
  return (lextended(SKW_476d_030a(lcon(0x2))) & lcon(0xffff)) != lcon(0x1) ? false : true;
  return (lextended(SKW_476d_030a(lcon(0x2))) & lcon(0xffff)) != lcon(0x3) ? false : true;
i32 R_1539(void)
  SKW_443c_08ab(&vw_04, &vw_08, &vw_00);
  RG1W = SKW_476d_05a9();
void SKW_44c8_1aca(t_gfxdata* eaxpgarr, i32 ebxl, i32 ecxl, c_rect* edxrp)
  SKW_FIRE_UPDATE_BLIT_PALETTE(ddat.paletteptr1);
  SKW_44c8_08ae(lcon(0x0), RG2L, RG3Blo, RG4W);
i32 SKW_44c8_1e43(xxx* xeaxp, xxx* xebxp, c_rect* ecxrp, t_gfxdata* edxpgarr, i32 argl0, i32 argl1, i32 argl2, i32 argl3, i32 argl4, i32 argl5, i16 argw6, t_colidx* argpcarr7)
  SKW_FIRE_UPDATE_BLIT_PALETTE(argpcarr7);
  SKW_44c8_08ae(RG1W, RG2L, RG3Blo, RG4W);
  SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(RG1W, RG2W, RG4W);
  SKW_44c8_08ae(RG1W, RG2L, RG3Blo, RG4W);
  SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(RG1W, RG2W, RG4W);
  SKW_44c8_08ae(RG1W, RG2L, RG3Blo, RG4W);
  SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(RG1W, RG2W, RG4W);
  SKW_44c8_08ae(RG1W, RG2L, RG3Blo, RG4W);
  SKW_FIRE_BLIT_TO_MEMORY_ROW_4TO8BPP_NOKEY(RG1W, RG2W, RG4W);
void SKW_44c8_2351(xxx* xeaxp, i32 ebxl, i32 ecxl, xxx* xedxp, i16 argw0, i16 argw1)
  DM2_blit8pppixel(RG1W, RG2W, RG4W);
void SKW_443c_0004(xxx* xeaxp, t_colidx* ebxpcarr, t_gfxdata* edxpgarr)
  RG1P = DOWNCAST(t_gfxdata, SKW_ALLOC_PICT_BUFF(lcon(0x10), BIGPOOL_LO, lcon(0x4), lcon(0x10)));
  SKW_FILL_ENTIRE_PICT(UPCAST(t_gfxdata, RG5p), RG4Blo);
  SKW_DRAW_DIALOGUE_PICT(UPCAST(t_gfxdata, RG7p), RG2R, RG3W, UPCAST(t_gfxdata, RG5p), lcon(0x0), lcon(0xc), vcarr_00);
  SKW_DRAW_DIALOGUE_PICT(UPCAST(t_gfxdata, RG7p), RG2R, RG3W, UPCAST(t_gfxdata, RG5p), lcon(0x0), lcon(0xc), ebxpcarr);
  SKW_FREE_PICT_BUFF(pgarr_10);
void SKW_443c_0434(void)
  RG1P = DOWNCAST(t_gfxdata, SKW_ALLOC_PICT_BUFF(lcon(0x12), BIGPOOL_LO, lcon(0x8), lcon(0x12)));
  SKW_443c_0004(ddat.savegamewpc.xp_02, ddat.savegamewpc.carr_06, UPCAST(t_gfxdata, RG5p));
  SKW_FIRE_MOUSE_EVENT_RECEIVER(lcon(0x2), lcon(0x8), lcon(0x8), UPCAST(t_gfxdata, RG5p), lcon(0x12), lcon(0x12), lcon(0x8), NULL, unsignedword(ddat.savegamewpc.carr_06[0xc]));
  SKW_FREE_PICT_BUFF(UPCAST(t_gfxdata, RG5p));
  SKW_443c_040e();
void SKW_CHAMPION_SQUAD_RECOMPUTE_POSITION(void)
  SKW_CHANGE_PLAYER_POS(unsignedlong(mkuw(ddat.v1e00b0 | wcon(0x8000))));
void SKW_CHANGE_PLAYER_POS(i32 eaxl)
  RG1L = SKW_GET_PLAYER_AT_POSITION(unsignedlong(mkuw(vql_00.peek16() + ddat.v1e0258 & wcon(0x3))));
  RG1L = SKW_GET_PLAYER_AT_POSITION(unsignedlong(mkuw(ddat.v1e0258 + RG4W & wcon(0x3))));
  RG1P = DOWNCAST(t_gfxdata, SKW_ALLOC_PICT_BUFF(ddat.v1d272c, BIGPOOL_LO, lcon(0x4), CUTX16(RG4L)));
  RG52p = DOWNCAST(t_gfxdata, SKW_ALLOC_PICT_BUFF(ddat.v1d272c, BIGPOOL_LO, lcon(0x8), ddat.v1d272e));
  SKW_QUERY_GDAT_SQUAD_ICON(UPCAST(t_gfxdata, RG62p), vcarr_04, signedlong(xw_14.peek16()));
  SKW_FILL_ENTIRE_PICT(UPCAST(t_gfxdata, RG52p), lcon(0xc));
  SKW_FIRE_BLIT_PICTURE(UPCAST(t_gfxdata, RG62p), RG2R, lcon(0x0), UPCAST(t_gfxdata, RG52p), lcon(0x0), ddat.v1d272c, ddat.v1d272c, lcon(0x4), lcon(0x0), lcon(0x4), lcon(0x8), vcarr_04);
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_FIRE_MOUSE_EVENT_RECEIVER(lcon(0x3), lcon(0x8), lcon(0x6), UPCAST(t_gfxdata, RG52p), ddat.v1d272c, ddat.v1d272e, lcon(0x8), NULL, unsignedword(vcarr_04[0xc]));
  SKW_FREE_PICT_BUFF(UPCAST(t_gfxdata, RG52p));
  SKW_FREE_PICT_BUFF(UPCAST(t_gfxdata, RG62p));
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_443c_040e();
void SKW_443c_06b4(xxx* xeaxp)
  SKW_443c_087c();
  SKW_QUERY_EXPANDED_RECT(RG1W, RG4R);
  SKW_443c_00a9(UPCAST(s_munknown, RG5p), word_at(RG2P) + word_at(RG2P, lcon(0x4)) - 1, word_at(RG2P, lcon(0x2)) + word_at(RG2P, lcon(0x6)) - 1, word_at(RG2P), word_at(RG2P, lcon(0x2)));
  SKW_443c_0889();
  SKW_443c_040e();
void SKW_443c_07d5(xxx* xeaxp)
  SKW_443c_087c();
  SKW_443c_0889();
  SKW_443c_040e();
i32 R_4EA8(i32 eaxl, i32 edxl)
  RG1P = SKW_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0xf), lcon(0x7), lcon(0xfc), CUTLX8(RG4L));
i32 SKW_GET_CREATURE_ANIMATION_FRAME(i32 eaxl, i32 edxl, i16* ebxpw, i16* ecxpw, xxx** argpp0, i32 argl1)
  RG1P = SKW_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0xf), lcon(0x8), lcon(0xfb), CUTLX8(RG4L));
  RG1P = SKW_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0xf), lcon(0x7), lcon(0xfc), CUTLX8(RG4L));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(RG52l);
  return R_4FCC(RG52l, unsignedlong(xwc_00.peek16()), UPCAST(i16, RG6p), UPCAST(xxx*, DOWNCAST(xxx*, argpp0)));
i32 R_4FCC(i32 eaxl, i32 edxl, i16* ebxpw, xxx** xecxpp)
  RG1P = SKW_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0xf), lcon(0x7), lcon(0xfc), CUTLX8(RG4L));
  RG1L = SKW_RAND();
i32 R_50CB(i32 eaxl, i32 edxl, xxx* xebxp, xxx** xecxpp)
  RG1P = SKW_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0xf), lcon(0x7), lcon(0xfc), CUTLX8(RG4L));
i32 SKW_UPDATE_GLOB_VAR(i32 eaxl, i32 ebxl, i32 edxl)
  RG1L = SKW_GET_GLOB_VAR(unsignedlong(RG5W));
  RG1L = SKW_BETWEEN_VALUE16(lcon(0x0), lcon(0xff), RG4L);
i32 SKW_48ae_011a(i32 eaxl)
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG51l);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG51l);
  if ((SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(CUTLX8(RG52l), lcon(0x1), lcon(0x8), CUTLX8(RG6L)) ? 1 : 0) == lcon(0x0))
  if ((SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(CUTLX8(RG52l), lcon(0x1), lcon(0xc), CUTLX8(RG6L)) ? 1 : 0) == lcon(0x0))
  if ((SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(CUTLX8(RG52l), lcon(0x1), lcon(0xa), CUTLX8(RG6L)) ? 1 : 0) != lcon(0x0))
  return (SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(CUTLX8(RG52l), lcon(0x1), lcon(0x9), CUTLX8(RG6L)) ? 1 : 0) == lcon(0x0) ? lcon(0x2) : lcon(0x0);
i32 SKW_IS_ITEM_FIT_FOR_EQUIP(i32 eaxl, i32 ebxl, i32 edxl)
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG1W), lcon(0x4));
  RG1L = SKW_IS_ITEM_FIT_FOR_EQUIP(unsignedlong(ddat.v1d67c2.warr2d_00[RG2L / 4][signedlong(ddat.v1e0b76)]), lcon(0x0), lcon(0xc));
i32 SKW_QUERY_CREATURES_ITEM_MASK(i32 eaxl, xxx* xebxp, i32 ecxl, i32 edxl)
  DM2_ZERO_MEMORY(RG2P, lcon(0x40));
  RG1T = SKW_QUERY_GDAT_TEXT(lcon(0xf), RG2L, tarr_00, unsignedlong(vb_8c));
void SKW_LOAD_MISCITEM(void)
  SKW_COPY_MEMORY(DOWNCAST(i16, &ddat.v1e03ac[RG7l / 2]), RG62l, DOWNCAST(i16, &ddat.v1e03ac[RG1L / 2]));
  SKW_COPY_MEMORY(RG1P, RG2L, RG4P);
i32 SKW_GET_ITEM_ORDER_IN_CONTAINER(i32 eaxl, i32 edxl)
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(unsignedlong(RG1W));
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x14), lcon(0x40), RG3T, RG4L));
i32 SKW_48ae_05ae(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl, i32 argl0, i32 argl1)
  RG1L = SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG4L);
  RG1L = SKW_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(RG4L);
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x40));
  RG1L = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(RG7L, RG4L);
  RG1L = SKW_QUERY_CREATURES_ITEM_MASK(RG7L, RG61p, lcon(0x0), RG4L);
  RG1L = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(vb_18), RG4L);
  RG1L = SKW_QUERY_CREATURES_ITEM_MASK(lcon(0x64), DOWNCAST(xxx, xp_08), lcon(0x0), RG4L);
  DM2_DEALLOC_LOBIGPOOL(lcon(0x40));
  RG1L = SKW_GET_MAX_CHARGE(SKW_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(unsignedlong(vql_00.peek16())) << bcon(0xa) & lcon(0xffff));
  RG1L = SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG62L);
  RG1L = lextendedDM2_QUERY_GDAT_ENTRY_DATA_INDEX(CUTLX8(SKW_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(RG62L) & lcon(0xff)), lcon(0xb), lcon(0x35), CUTLX8(RG4L)) * argl1;
  RG1L = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(vb_18), RG4L);
  return RG5L - SKW_MAX16(signedlong(RG1W), lcon(0x1));
i32 SKW_QUERY_OBJECT_5x5_POS(i32 eaxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG2W));
  return SKW_QUERY_CREATURE_5x5_POS(RG1P, RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG2W));
  RG1W = SKW_ROTATE_5x5_POS(RG3W, t_rotate(RG4UW));
void R_5BFB(i32 eaxl, i32 edxl)
  SKW_QUEUE_NOISE_GEN1(lcon(0x3), lcon(0x8b), lcon(0x6c), lcon(0x0), lcon(0xc8), ddat.v1e0270, parw02, lcon(0x0));
  R_7E7(RG1W, RG4W);
i32 SKW_SHOOT_ITEM(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i8 argb1, i8 argb2, i8 argb3)
  RG1L = SKW_ALLOC_NEW_RECORD(lcon(0xe));
  return SKW_MOVE_RECORD_TO(unsignedlong(RG6W), lcon(0xffffffff), lcon(0x0), RG3L, parw00);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG51l);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG51l) & lcon(0xff);
  SKW_QUEUE_NOISE_GEN2(RG1Blo, lcon(0x86), lcon(0xfe), RG4Blo, parw01, parw02, lcon(0x1), lcon(0x6e), lcon(0x80));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG7L);
  RG1L = SKW_MIN16(unsignedlong(argb1), lcon(0xff));
  SKW_APPEND_RECORD_TO(RG7L, vl_14, vl_10, NULL);
  RG1L = SKW_QUEUE_TIMER(&c38_00);
i32 SKW_075f_0182(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(RG1W), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG3W));
  SKW_INVOKE_ACTUATOR(RG1P, RG2L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG3W));
i32 SKW_CREATE_CLOUD(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
  RG1L = SKW_ALLOC_NEW_RECORD(lcon(0xf));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1L = SKW_MIN16(lcon(0xff), RG4L);
  SKW_QUEUE_NOISE_GEN2(lcon(0xd), lcon(0x81), lcon(0xfe), RG4Blo, parw00, parw01, lcon(0x1), lcon(0x6c), parw04);
  SKW_APPEND_RECORD_TO(RG1L, vl_14, RG3L, NULL);
  SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_04)));
  RG1L = SKW_GET_TILE_VALUE(RG2L, RG4L);
  RG1P = SKW_GET_ADDRESS_OF_TILE_RECORD(RG2L, RG4L);
  SKW_INVOKE_MESSAGE(unsignedlong(vq_34.peek16()), lcon(0x0), RG3L, unsignedlong(vq_38.peek16()), parl07);
  RG1L = SKW_RAND16(unsignedlong(RG62W)) + 1;
  SKW_ATTACK_PARTY(unsignedlong(RG62W), lcon(0x3f), lcon(0x1));
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vq_34.peek16()), unsignedlong(vq_38.peek16()));
  RG1L = SKW_1c9a_0958(RG2L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG2L);
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(RG1Blo));
  RG62L = SKW_MAX16(lcon(0x1), RG4L);
  RG1L = SKW_RAND16(2 * RG1L + 1 & lcon(0xffff));
  SKW_ATTACK_CREATURE(RG1L, RG4L, RG2L, RG3L, lcon(0x64), parl06);
  RG1L = SKW_1c9a_03cf(&vw_24, RG2L, RG4P16);
  return SKW_075f_0182(unsignedlong(vql_00.peek16()), signedlong(vq_2c.peek16()), signedlong(vq_20.peek16()), RG3L);
void SKW_DELETE_MISSILE_RECORD(i32 eaxl, i32 ebxl, i32 ecxl, xxx* xedxp)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG51W));
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG2W), RG4L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(RG2L);
  RG1W = word_at(SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(RG2L), lcon(0x1e)) & lcon(0xf);
  SKW_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw00);
  SKW_APPEND_RECORD_TO(RG1L, lcon(0xffffffff), lcon(0x0), RG4P);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG4L) & lcon(0xff);
  SKW_QUEUE_NOISE_GEN2(RG1Blo, lcon(0x85), lcon(0xfe), RG73Blo, parw01, parw02, lcon(0x1), lcon(0x3a), lcon(0x80));
  SKW_DEALLOC_RECORD(RG1L);
i32 SKW_075f_06bd(xxx* xeaxp, i32 edxl)
  RG1L = SKW_RAND();
  RG1L = SKW_RAND();
  RG1L = SKW_RAND();
  RG1L = SKW_RAND();
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG7L, lcon(0x9));
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG7L, RG4L);
  RG1L = SKW_RAND();
  RG1L = SKW_RAND16(unsignedlong(ddat.v1e01a4) / 2 + 1 & lcon(0xffff));
  RG2L += SKW_RAND02();
  RG1L = SKW_QUERY_ITEM_WEIGHT(unsignedlong(RG6W));
  RG1L = SKW_RAND();
  RG1L = SKW_RAND16(unsignedlong(RG2W) / 2 + 1 & lcon(0xffff));
  RG1L = SKW_RAND02() + RG4L;
  RG1L = SKW_MAX16(signedlong(RG2W), RG4L);
  return SKW_MIN16(signedlong(RG1W), 2 * signedlong(RG5W));
i32 SKW_075f_084d(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i32 argl1)
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(vql_0c.peek16()), signedlong(vql_08.peek16()));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG6W));
  RG1L = SKW_0cee_317f(unsignedlong(xwc_14.peek16()), RG4L);
  RG1L = SKW_RAND02();
  SKW_CUT_RECORD_FROM(RG53L, RG2L, RG3L, NULL);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(vql_18.peek16()));
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(RG4L & lcon(0xffff));
  SKW_INVOKE_ACTUATOR(RG52p, RG2L, RG4L);
  SKW_CUT_RECORD_FROM(RG1L, RG2L, RG3L, NULL);
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(RG4L & lcon(0xffff));
  SKW_CUT_RECORD_FROM(RG1L, RG2L, RG3L, RG4P);
  RG1L = (SKW_GET_TILE_VALUE(signedlong(RG1W), RG4L) & lcon(0xff)) >> bcon(0x5);
  SKW_DELETE_MISSILE_RECORD(unsignedlong(RG6W), RG2L, RG3L, NULL);
  vql_18.poke32(SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_18.peek16())));
i32 SKW_075f_0af9(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(mkuw(argl0))); // truncated TODO check if arg is always word
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG5W));
  RG1L = SKW_GET_PLAYER_AT_POSITION(unsignedlong(vql_00.peek16()));
  RG1L = SKW_075f_06bd(RG61p, RG4L);
  RG1L = SKW_WOUND_PLAYER(RG1L, lcon(0xc), RG3L, RG4L);
  RG1L = SKW_RAND();
  SKW_PROCESS_POISON(vl_1c, RG4L);
  RG1L = SKW_075f_084d(signedlong(vw_30), RG4L, RG2L, RG3L, parl00, parl01);
  RG1P = SKW_GET_ADDRESS_OF_TILE_RECORD(RG1L, RG4L);
  RG1L = SKW_0cee_3275(SKW_IS_REBIRTH_ALTAR(RG1P) & lcon(0xff));
  RG1L = SKW_RAND();
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG5W), lcon(0x0));
  RG1L = SKW_075f_06bd(RG61p, RG4L) + 1;
  RG1L = SKW_RAND16(unsignedlong(vq_44.peek16())) + vq_44.peek32();
  SKW_ATTACK_DOOR(unsignedlong(vol_08.peek16()), RG4L, RG2L, lcon(0x0), lcon(0x0));
  RG1L = SKW_1c9a_03cf(&vw_30, RG2L, RG4P16);
  RG1L = SKW_1c9a_0958(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG1P, lcon(0x4))));
  RG1L = SKW_075f_06bd(RG61p, RG4L) << bcon(0x6);
  RG1L = SKW_APPLY_CREATURE_POISON_RESISTANCE(unsignedlong(xwc_5c.peek16()), unsignedlong(ddat.v1e01a4)) + vq_44.peek32();
  SKW_ATTACK_CREATURE(unsignedlong(xwc_5c.peek16()), RG4L, RG2L, lcon(0x200d), lcon(0x64), parl04);
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG1L, RG4L);
  SKW_ATTACK_CREATURE(unsignedlong(xwc_5c.peek16()), RG4L, RG2L, lcon(0x2), parw05, parl06);
  SKW_DEALLOC_RECORD(unsignedlong(xwc_40.peek16()));
  SKW_CREATE_CLOUD(RG1L, RG4L, RG2L, RG3L, parl07);
  SKW_CUT_RECORD_FROM(RG62L, RG2L, RG3L, NULL);
  SKW_DELETE_MISSILE_RECORD(RG62L, signedlong(vo_28.peek16()), signedlong(vo_24.peek16()), DOWNCAST(xxx, xp_10));
  RG1L = SKW_MAX16(signedlong(CUTX16(SKW_MIN16(signedlong(vq_58.peek16()), lcon(0xfe)))), lcon(0x28));
  SKW_12b4_0d75(signedlong(vol_08.peek16()), signedlong(vol_04.peek16()), RG2L, RG3L);
void SKW_STEP_MISSILE(c_tim* eaxp_c38)
  SKW_COPY_MEMORY(DOWNCAST(c_tim, eaxp_c38), lcon(0xc), ADRESSOF(c_tim, &c38_00));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(c38_00.w_06));
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vo_38.peek16()), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(RG4L);
  RG1L = SKW_075f_0af9(lcon(0xfffffffd), signedlong(vo_38.peek16()), signedlong(vo_34.peek16()), RG3L, parl00);
  RG1L = SKW_075f_0af9(lcon(0xffffffff), signedlong(vo_38.peek16()), RG2L, RG3L, parl01);
  SKW_CUT_RECORD_FROM(RG65L, RG52l, RG72L, NULL);
  SKW_DELETE_MISSILE_RECORD(RG65L, RG52l, RG72L, NULL);
  RG1L = SKW_GET_TILE_VALUE(signedlong(vo_38.peek16()), signedlong(vo_34.peek16()));
  RG1L = SKW_075f_0af9(vl_10, vl_18, RG2L, RG3L, parl02);
  RG1L = SKW_GET_WALL_TILE_ANYITEM_RECORD(signedlong(vo_38.peek16()), signedlong(vo_34.peek16()));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
  RG1L = (SKW_GET_TILE_VALUE(RG1L, vl_0c) & lcon(0xff)) >> bcon(0x5) & lcon(0xff);
  RG1L = SKW_075f_0af9(lcon(0x4), vl_14, vl_0c, RG3L, parl08);
  SKW_CUT_RECORD_FROM(unsignedlong(vql_30.peek16()), vl_10, vl_18, NULL);
  SKW_APPEND_RECORD_TO(unsignedlong(vql_30.peek16()), vl_10, vl_18, NULL);
  SKW_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw03);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e102c));
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG2W), RG4L);
  SKW_ATTACK_CREATURE(RG1L, RG4L, RG2L, lcon(0x2006), lcon(0x64), parl05);
  RG1L = SKW_GET_TILE_VALUE(signedlong(ddat.v1e102a), RG4L);
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG64W), RG4L);
  SKW_ATTACK_CREATURE(RG1L, RG4L, RG2L, lcon(0x2006), lcon(0x64), parl07);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = SKW_QUEUE_TIMER(UPCAST(c_tim, RG51p));
i32 SKW_CALC_CLOUD_DAMAGE(i32 eaxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG3W));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(unsignedlong(RG3W));
  RG1L = SKW_RAND16(unsignedlong(RG4W)) + 1;
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG3W));
  RG1L = SKW_MIN16(signedlong(RG1W), lcon(0x4));
  RG1L = SKW_RAND01() + RG4L;
  RG1L = SKW_MAX16(lcon(0x1), RG4L);
  return SKW_APPLY_CREATURE_POISON_RESISTANCE(unsignedlong(RG3W), RG4L);
void SKW_PROCESS_CLOUD(c_tim* eaxp_c38)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG7p, lcon(0x8))));
  RG1L = (SKW_GET_TILE_VALUE(RG3L, RG2L) & lcon(0xff)) >> bcon(0x5) & lcon(0xff);
  RG1L = SKW_GET_TILE_RECORD_LINK(RG3L, RG4L);
  RG1L = SKW_CALC_CLOUD_DAMAGE(unsignedlong(word_at(RG7p, lcon(0x8))), RG4L);
  SKW_ATTACK_DOOR(RG1L, RG4L, RG2L, RG3L, lcon(0x0));
  RG1L = SKW_CALC_CLOUD_DAMAGE(unsignedlong(word_at(RG7p, lcon(0x8))), lcon(0xffff));
  SKW_ATTACK_PARTY(RG1L, lcon(0x0), RG2L);
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(xw_10.peek16()), RG4L);
  RG1L = SKW_1c9a_0958(RG1L);
  RG1L = SKW_CALC_CLOUD_DAMAGE(RG1L, RG4L);
  SKW_ATTACK_CREATURE(RG1L, RG4L, RG2L, RG3L, lcon(0x64), parl02);
  SKW_QUEUE_NOISE_GEN2(RG1Blo, RG2Blo, RG3Blo, RG4Blo, parw03, parw04, lcon(0x1), lcon(0x6c), lcon(0xc8));
  SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_00)));
  SKW_CUT_RECORD_FROM(unsignedlong(word_at(RG7p, lcon(0x8))), signedlong(xw_10.peek16()), RG3L, NULL);
  SKW_DEALLOC_RECORD(unsignedlong(word_at(RG7p, lcon(0x8))));
void SKW_075f_1bc2(s_4bytearray* eaxp_s08, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1)
  RG1L = SKW_CALC_VECTOR_DIR(RG6w, RG7w, RG3W, RG4W);
  SKW_COPY_MEMORY(RG1P, lcon(0x4), RG5p);
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND01();
  SKW_COPY_MEMORY(RG1P, lcon(0x4), RG5p);
i32 SKW_CREATURE_ATTACKS_PLAYER(xxx* xeaxp, i32 edxl)
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG61p, lcon(0x4))));
  RG4L = SKW_MIN16(lcon(0xff), signedlong(CUTX16(2 * RG4L)));
  RG1L = SKW_RAND();
  RG1L = SKW_USE_DEXTERITY_ATTRIBUTE(unsignedlong(vq_0c.peek16())) & lcon(0xffff);
  RG1L = SKW_RAND02();
  RG1L = SKW_USE_LUCK_ATTRIBUTE(RG7p, RG4L);
  RG1L = SKW_RAND();
  RG1L = SKW_RAND();
  RG1L = SKW_MIN16(unsignedlong(byte_at(RG62p, lcon(0x6))), RG4L);
  RG1L = 2 * SKW_QUERY_PLAYER_SKILL_LV(signedlong(vq_0c.peek16()), lcon(0x1), lcon(0x7));
  RG1L = SKW_RAND01();
  RG5L = SKW_RAND02() + lcon(0x2);
  RG1L = SKW_RAND16(unsignedlong(RG5W));
  RG1L = SKW_RAND02() + RG4L;
  RG1L = SKW_RAND16(unsignedlong(RG5W));
  RG1L = SKW_RAND02() + 1;
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND16(signedlong(RG5W) / 2 + 1 & lcon(0xffff)) - 1;
  RG1L = SKW_WOUND_PLAYER(RG5L, RG2L, RG1L, RG4L);
  SKW_QUEUE_NOISE_GEN2(lcon(0x16), RG2Blo, RG3Blo, RG4Blo, parw00, parw01, lcon(0x2), lcon(0x69), lcon(0xc8));
  RG1L = SKW_RAND01();
  RG1L = SKW_2c1d_0f2a(RG7p, RG2L, RG4L);
  SKW_PROCESS_POISON(vl_00, RG4L);
  SKW_RESUME_FROM_WAKE();
i32 SKW_CALC_PLAYER_ATTACK_DAMAGE(xxx* xeaxp, i32 ebxl, i32 ecxl, i32 edxl, i16 argw0, i32 argl1, i16 argw2, i16 argw3, i16 argw4)
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(SKW_GET_ADDRESS_OF_RECORD(unsignedlong(xwc_0c.peek16())), lcon(0x4))));
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vq_04.peek16()), unsignedlong(argw0));
  RG1L = SKW_1c9a_0958(unsignedlong(RG1W));
  RG1L = SKW_RAND();
  RG1L = SKW_USE_DEXTERITY_ATTRIBUTE(unsignedlong(RG6W)) & lcon(0xffff);
  RG1L = SKW_RAND02();
  RG1L = SKW_USE_LUCK_ATTRIBUTE(RG7p, RG4L);
  RG1L = SKW_COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(signedlong(RG6W), RG2L, RG4L);
  RG1L = SKW_RAND16(signedlong(RG1W) / 2 + 1 & lcon(0xffff));
  RG1L = SKW_RAND();
  RG1L = SKW_RAND();
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND();
  RG1L = SKW_RAND01();
  RG5L += SKW_RAND02();
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND();
  RG5L += SKW_MAX16(lcon(0x0), RG4L);
  RG1L = SKW_RAND16(unsignedlong(RG5W));
  RG1L = SKW_RAND02() + RG4L;
  RG1L = SKW_RAND16(unsignedlong(RG5W));
  RG1L = SKW_RAND02() + 1;
  RG1L = SKW_RAND();
  RG1L = SKW_QUERY_PLAYER_SKILL_LV(signedlong(RG6W), lcon(0x1), RG4L) & lcon(0xffff);
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(word_at(RG7p, lcon(0xc3))), lcon(0xd));
  RG1L = SKW_RAND();
  RG5L += SKW_APPLY_CREATURE_POISON_RESISTANCE(unsignedlong(xwc_0c.peek16()), RG4L);
  SKW_ADJUST_SKILLS(RG1L, RG2L, RG4L);
  RG1L = SKW_RAND02() + lcon(0x4);
  RG1L = SKW_RAND01() + lcon(0x2);
  SKW_ADJUST_STAMINA(RG1L, RG4L);
  RG1L = SKW_RAND01() + 1;
  SKW_ATTACK_CREATURE(unsignedlong(xwc_0c.peek16()), signedlong(vq_04.peek16()), signedlong(argw0), lcon(0x6002), lcon(0x5a), parl01);
i32 SKW_ATTACK_DOOR(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0)
  RG1P = SKW_GET_ADDRESS_OF_TILE_RECORD(signedlong(vql_04.peek16()), RG4L);
  RG1L = SKW_GET_DOOR_STAT_0X10(SKW_IS_REBIRTH_ALTAR(RG4P) & lcon(0xff));
  SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_08)));
void SKW_LOAD_RECTS_AND_COMPRESS(i32 eaxl, i32 ebxl, i32 edxl)
  RG1L = SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(CUTLX8(RG51l), CUTLX8(RG2L), CUTLX8(RG3L), CUTLX8(RG4L));
  RG1P = DM2_ALLOC_HIBIGPOOL_MEMORY(RG52l);
  SKW_LOAD_GDAT_ENTRY_DATA_TO(CUTLX8(vl_00), lcon(0x4), CUTLX8(vl_04), CUTLX8(RG7L), xparp00);
  SKW_COMPRESS_RECTS(RG6p, UPCAST(s_rnode, RG2P), DM2_ALLOC_FREEPOOL_MEMORY);
  DM2_DEALLOC_HIBIGPOOL(RG52l);
void SKW_098d_1208(void)
  SKW_LOAD_RECTS_AND_COMPRESS(lcon(0x1), lcon(0x0), lcon(0x0));
i32 SKW_0aaf_0067(i32 eaxl)
  DM2_ZERO_MEMORY(ADRESSOF(t_text, tarr_00 + lcon(0x28)), lcon(0x28));
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x1a), RG2L, RG3T, RG4L));
  RG1L = SKW_1031_0675(lcon(0x4));
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_EVENT_LOOP();
  SKW_WAIT_SCREEN_REFRESH();
  RG1L = lcon(0x0); // called SKW_476d_04ed here
  SKW_1031_0781(RG1L);
  RG1W = SKW_476d_05a9();
  RG1L = signedlong(SKW_476d_050e());
  SKW_1031_0781(lcon(0xdb));
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_1031_06a5(); // TODO: return not needed
i32 SKW_0aaf_01db(i32 eaxl, i32 edxl)
  RG1R = SKW_QUERY_EXPANDED_RECT(RG1W, RG4R);
  SKW_FIRE_FILL_BACKBUFF_RECT(RG1R, RG4Blo);
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(ddat.v1e0206, CUTLX8(RG2L), CUTLX8(RG4L)));
  SKW_CALC_CENTERED_RECT_IN_RECT(RG1R, RG2L, RG3L, RG4R);
  SKW_QUERY_TOPLEFT_OF_RECT(ddat.mouse_unk09, UPCAST(i16, ADRESSOF(c_q32, &vq_08)), UPCAST(i16, ADRESSOF(c_q32, &vq_0c)));
  RG1P = RG52p + (SKW_CALC_IMAGE_BYTE_LENGTH(UPCAST(t_gfxdata, RG52p)) & lcon(0xffff)); // TODO: type of RG52p has to be revealed
  RG1P = DOWNCAST(t_colidx, SKW_QUERY_GDAT_IMAGE_LOCALPAL(ddat.v1e0206, CUTLX8(RG2L), CUTLX8(RG4L)));
  SKW_DRAW_DIALOGUE_PARTS_PICT(UPCAST(t_gfxdata, RG52p), lcon(0x7), UPCAST(t_colidx, RG3P), &rc_00);
  SKW_FREE_PICT_ENTRY(RG52p);
  SKW_SLEEP_SEVERAL_TIME(RG1W);
i32 SKW_0aaf_02f8(i32 eaxl, i32 edxl)
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x1a), lcon(0x1), lcon(0x0), lcon(0x59)) ? 1 : 0;
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x1a), lcon(0x1), lcon(0x0), lcon(0x0)) ? 1 : 0;
  RG1L = SKW_0aaf_02f8(lcon(0x0), unsignedlong(vb_4dc)) & lcon(0xff);
  SKW_0aaf_0067(RG1L);
  SKW_FIRE_FADE_SCREEN(lcon(0x1));
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x1a), RG2L, RG3T, RG4L));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x1a), lcon(0x0), CUTLX8(RG4L)));
  RG1L = SKW_CALC_IMAGE_BYTE_LENGTH(UPCAST(t_gfxdata, RG1P));
  RG3P = DOWNCAST(t_colidx, SKW_QUERY_GDAT_IMAGE_LOCALPAL(lcon(0x1a), lcon(0x0), CUTLX8(RG3L)));
  RG1R = SKW_QUERY_EXPANDED_RECT(lcon(0x4), RG4R);
  SKW_DRAW_DIALOGUE_PARTS_PICT(UPCAST(t_gfxdata, RG61p), RG2W, UPCAST(t_colidx, RG3P), RG4R);
  SKW_FREE_PICT_ENTRY(RG1P);
  SKW_SET_ORIGIN_RECT(RG1R, RG4W, RG2W);
  SKW_FIRE_FILL_BACKBUFF_RECT(RG1R, RG4Blo);
  SKW_INFLATE_RECT(RG1R, RG4W, RG2W);
  SKW_FIRE_FILL_BACKBUFF_RECT(RG1R, RG4Blo);
  SKW_0aaf_01db(RG1L, lcon(0x0));
  SKW_DRAW_VP_RC_STR(lcon(0x1c2), RG2T, RG4W);
  SKW_QUERY_EXPANDED_RECT(RG1W, RG4R);
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x1a), lcon(0x14), RG3T, RG4L));
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x1a), RG2L, RG3T, lcon(0x0)));
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x1a), RG2L, RG3T, RG4L));
  SKW_DRAW_VP_RC_STR(RG1W, RG2T, RG4W);
  RG1L = SKW_MAX16(lcon(0x0), RG4L);
  SKW_QUERY_STR_METRICS(RG1T, UPCAST(i16, RG52p + lcon(0x6)), UPCAST(i16, RG52p + lcon(0x4)));
  SKW_3929_04e2(RG1T, RG2P16, RG3L, tarr_470);
  RG1L = SKW_3929_04e2(RG1T, &vw_4d8, signedlong(word_at(RG52p, lcon(0x4))), tarr_470);
  SKW_DRAW_VP_STR(RG1W, RG2W, RG3T, RG4W);
  SKW_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
  SKW_FIRE_FADE_SCREEN(RG1L);
void R_AB26(void)
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x1a), lcon(0x1), lcon(0x0), lcon(0x87)) ? 1 : 0;
  SKW_0aaf_02f8(lcon(0x87), lcon(0x0));
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x1a), lcon(0x28), RG3T, lcon(0x87)));
  SKW_DRAW_VP_RC_STR(lcon(0x15f), RG1T, RG4W);
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x1a), lcon(0x3c), RG3T, lcon(0x87)));
  SKW_DRAW_VP_RC_STR(lcon(0x170), RG1T, RG4W);
  SKW_DRAW_STATIC_PIC(lcon(0x1a), RG2L, RG3L, lcon(0x87), lcon(0xffffffff));
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x1a), lcon(0x29), RG3T, lcon(0x87)));
  SKW_DRAW_VP_RC_STR(RG1W, RG2T, RG4W);
  SKW_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
  RG1L = SKW_0aaf_0067(lcon(0x87));
  SKW_DRAW_STATIC_PIC(lcon(0x1a), RG2L, RG3L, lcon(0x87), parl01);
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x1a), lcon(0x3d), RG3T, lcon(0x87)));
  SKW_DRAW_VP_RC_STR(RG1W, RG2T, RG4W);
  R_5BFB(RG1L, RG4L);
t_gfxdata* SKW_QUERY_PICST_IMAGE(c_hex18* eaxp_c30)
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(byte_at(RG3P, lcon(0x8)), RG2L, CUTLX8(RG4L)));
t_gfxdata* SKW_0b36_00c3(i32 eaxl, c_hex18* edxp_c30)
  RG3P = DOWNCAST(t_gfxdata, SKW_3e74_5817(CUTX16(RG1L)));
t_gfxdata* SKW_QUERY_PICT_BITS(c_hex18* eaxp_c30)
  RG1P = DOWNCAST(t_gfxdata, SKW_3e74_5817(word_at(RG3P, lcon(0xc)))); // keep RG1P!
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(byte_at(RG3P, lcon(0x8)), RG2L, CUTLX8(RG4L))); // keep RG1P!
void SKW_0b36_018f(c_hex18* eaxp_c30)
  SKW_3e74_583a(word_at(RG1P, lcon(0xc)));
  SKW_3e74_58bf(CUTLX8(RG1L), CUTLX8(RG2L), CUTLX8(RG4L));
void SKW_0b36_01cd(c_hex18* eaxp_c30)
  SKW_3e74_585a(RG1W, false);
  SKW_3e74_5992(CUTLX8(RG1L), CUTLX8(RG2L), CUTLX8(RG4L));
i32 SKW_LOAD_GDAT_INTERFACE_00_02(void)
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(lcon(0x1), lcon(0x7), lcon(0x2), lcon(0x0)) & lcon(0xffff));
  SKW_LOAD_GDAT_ENTRY_DATA_TO(lcon(0x1), lcon(0x7), lcon(0x2), lcon(0x0), xparp00);
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(RG1L);
t_colidx* R_B012(t_colidx* eaxpcarr, i32 ebxl, i32 ecxl, i32 edxl, i16* argpw0)
  RG3P = SKW_QUERY_GDAT_ENTRY_DATA_PTR(CUTLX8(RG1L), CUTLX8(RG2L), CUTLX8(RG3L), CUTLX8(RG4L));
  SKW_COPY_MEMORY(RG3P, PAL256, DOWNCAST(t_colidx, eaxpcarr)); // left side should be a 256 - colortable too
t_colidx* R_B073(t_colidx* eaxpcarr, i32 ebxl, i32 ecxl, i32 edxl, i16* argpw0)
  RG1L = SKW_MAX16(lcon(0x0), RG4L);
  RG1L = SKW_3e74_55f9(unsignedlong(vb_28) | mkul(RG4L), &vw_10) ? 1 : 0;
  SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, SKW_3e74_5817(vw_10)), PAL256, DOWNCAST(t_colidx, pcarr_00));
  RG4P = DOWNCAST(s_xmalloc, SKW_ALLOC_CPXHEAP_MEM(RG1W, lcon(0x100)));
  SKW_COPY_MEMORY(DOWNCAST(t_colidx, pcarr_00), PAL256, RG4P);
  SKW_3e74_585a(vw_10, false);
  RG1L = SKW_MAX16(signedlong(RG1W), lcon(0x0));
void SKW_0b36_037e(t_colidx* eaxpcarr, i32 ebxl, i32 ecxl, i32 edxl, i8 argb0, i16 argw1, i16 argw2, i16* argpw3)
  RG1L = SKW_3e74_55f9(mkul(signedlong(CUTX16(SKW_MAX16(lcon(0x0), RG4L))) | RG2L), &vw_08) ? 1 : 0;
  SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, SKW_3e74_5817(vw_08)), PAL256, DOWNCAST(t_colidx, eaxpcarr));
  R_B073(UPCAST(t_colidx, DOWNCAST(t_colidx, R_B012(eaxpcarr, unsignedlong(vb_10), unsignedlong(vb_0c), unsignedlong(vb_14), parpw00))), signedlong(argw1), vl_00, vl_04, parpw01);
  RG1P = DOWNCAST(s_xmalloc, SKW_ALLOC_CPXHEAP_MEM(vw_08, PAL256));
  SKW_COPY_MEMORY(DOWNCAST(t_colidx, eaxpcarr), PAL256, RG1P);
  SKW_3e74_585a(vw_08, false);
c_image* SKW_QUERY_GDAT_SUMMARY_IMAGE(c_image* eaxp_c37, i32 ebxl, i32 ecxl, i32 edxl)
  DM2_ZERO_MEMORY(RG1P, lcon(0x13a));
  RG1W = SKW_QUERY_GDAT_PICT_OFFSET(vb_04, vb_08, vb_00);
  RG1P = DOWNCAST(t_colidx, SKW_QUERY_GDAT_IMAGE_LOCALPAL(vb_04, CUTLX8(RG2L), CUTLX8(RG4L)));
  SKW_COPY_MEMORY(RG1P, PAL16, RG5p + lcon(0x3a)); // RG1P and RG4P both carry t_colidx
xxx* SKW_QUERY_PICST_IT(c_image* eaxp_c37)
  RG1L = SKW_CALC_STRETCHED_SIZE(word_at(RG5p, lcon(0x1c)), RG4W);
  RG1L = SKW_CALC_STRETCHED_SIZE(word_at(RG5p, lcon(0x1e)), word_at(RG5p, lcon(0x36)));
  RG1L = SKW_3e74_55f9(mkul(SKW_0b36_068f(UPCAST(c_image, RG5p))), &vw_160) ? 1 : 0;
  SKW_COPY_MEMORY(RG5p, lcon(0x18), ADRESSOF(c_hex18, &c30_13c));
  SKW_QUERY_PICST_IMAGE(UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_13c)));
  RG1W = SKW_3e74_5888();
  SKW_COPY_MEMORY(RG5p, lcon(0x18), ADRESSOF(c_hex18, &c30_13c));
  SKW_0b36_00c3(signedlong(c30_13c.w_0c), UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_13c)));
  SKW_0b36_018f(UPCAST(c_hex18, RG1P));
  SKW_COPY_MEMORY(RG5p, lcon(0x13a), ADRESSOF(c_image, &c37_00));
  RG1L = SKW_3e74_55f9(mkul(SKW_0b36_068f(&c37_00)), &vw_15c) ? 1 : 0;
  RG1P = DOWNCAST(t_gfxdata, SKW_ALLOC_NEW_PICT(vw_15c, CUTX16(RG2L), CUTLX8(RG3L), CUTX16(RG4L)));
  RG61p = DOWNCAST(t_gfxdata, SKW_QUERY_PICT_BITS(UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_13c))));
  RG1R = SKW_ALLOC_TEMP_ORIGIN_RECT(c30_13c.w_12, c30_13c.w_14);
  SKW_FIRE_BLIT_PICTURE(UPCAST(t_gfxdata, RG61p), RG2R, RG3W, UPCAST(t_gfxdata, RG71p), parw00, parw01, parw02, lcon(0xffff), lcon(0x0), parw05, parw06, NULL);
  SKW_0b36_01cd(UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_13c)));
  SKW_0b36_00c3(signedlong(vw_15c), UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_13c)));
  SKW_0b36_018f(UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_13c)));
  RG1L = SKW_CALC_STRETCHED_SIZE(c30_13c.w_12, word_at(RG5p, lcon(0x34)));
  RG1L = SKW_CALC_STRETCHED_SIZE(c30_13c.w_14, word_at(RG5p, lcon(0x36)));
  SKW_ALLOC_NEW_PICT(vw_160, CUTX16(vl_154), CUTLX8(RG3L), CUTX16(RG72L));
  RG63p = DOWNCAST(t_gfxdata, SKW_QUERY_PICT_BITS(UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_13c))));
  RG1P = DOWNCAST(t_gfxdata, SKW_0b36_00c3(signedlong(vw_160), UPCAST(c_hex18, RG5p)));
  SKW_FIRE_STRETCH_BLIT_TO_MEMORY_4TO4BPP(RG63p, c30_13c.w_12, c30_13c.w_14, RG1P, parl10, parw11, NULL);
  SKW_44c8_2351(RG63p, signedlong(c30_13c.w_12), signedlong(c30_13c.w_14), RG1P, parw08, parw09);
  SKW_0b36_01cd(UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_13c)));
  SKW_3e74_585a(vw_160, false);
  SKW_QUERY_PICST_IMAGE(UPCAST(c_hex18, RG5p));
  SKW_0b36_00c3(RG1L, UPCAST(c_hex18, RG5p));
void SKW_DRAW_PICST(c_image* eaxp_c37)
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_PICT_BITS(UPCAST(c_hex18, RG1P)));
  RG1R = SKW_QUERY_BLIT_RECT(UPCAST(t_gfxdata, RG6p), RG2W, RG3P16, RG4R, parpw00, parw01);
  SKW_FIRE_BLIT_PICTURE(UPCAST(t_gfxdata, RG6p), RG2R, RG3W, UPCAST(t_gfxdata, pointer_at(RG5p + lcon(0x2c))), parw02, parw03, parw04, parw05, parw06, parw07, parw08, parpcarr09);
void SKW_0b36_0c52(c_c5rects* eaxp_c22, i32 ebxl, i32 edxl)
  SKW_QUERY_EXPANDED_RECT(RG1W, RG4R);
  RG1W = SKW_3e74_5888();
  SKW_ALLOC_NEW_PICT(CUTX16(RG1L), CUTX16(RG2L), lcon(0x8), CUTX16(RG4L));
  SKW_0b36_0d67(esip_c22, RG4R);
void SKW_0b36_0cbe(c_c5rects* eaxp_c22, i32 edxl)
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  RG1P = DOWNCAST(t_gfxdata, SKW_3e74_5817(esip_c22->c16_00.w_00));
  SKW_FIRE_BLIT_PICTURE(UPCAST(t_gfxdata, RG1P), RG2R, RG3W, UPCAST(t_gfxdata, RG4P), parw00, parw01, parw02, lcon(0xffff), lcon(0x0), lcon(0x8), lcon(0x8), NULL);
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_3e74_58a3(esip_c22->c16_00.w_00);
void SKW_0b36_105b(c_c5rects* eaxp_c22, i32 ebxl, c_rect* edxrp) // serious! entry to deactivated - rectangle - blit HOTHOT
  RG4R = SKW_OFFSET_RECT(esip_c22, &rc_00, RG6r);
  SKW_44c8_1aca(SKW_3e74_5817(esip_c22->c16_00.w_00), RG7L, RG3L, RG4R);
  SKW_0b36_0d67(esip_c22, RG6r);
void SKW_DRAW_ICON_PICT_BUFF(t_gfxdata* eaxpgarr, c_rect* ebxrp, i32 ecxl, c_c5rects* edxp_c22, i16 argw0, i16 argw1, i16 argw2, t_colidx* argcarr3)
  RG1R = SKW_OFFSET_RECT(esip_c22, RG2R, RG4R);
  SKW_FIRE_BLIT_PICTURE(UPCAST(t_gfxdata, RG6p), RG2R, RG3W, SKW_3e74_5817(esip_c22->c16_00.w_00), parw00, parw01, parw02, parw03, parw04, parw05, lcon(0x8), argcarr3);
  SKW_0b36_0d67(esip_c22, RG4R); // this is the clickbutton-effect
void SKW_DRAW_ICON_PICT_ENTRY(i32 eaxl, i32 ebxl, c_c5rects* ecxp_c22, i32 edxl, i16 argw0, i16 argw1)
  RG5p = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(CUTLX8(RG1L), CUTLX8(RG7L), CUTLX8(RG6L)));
  RG1R = SKW_QUERY_BLIT_RECT(UPCAST(t_gfxdata, RG5p), RG2W, RG3P16, RG4R, parpw00, lcon(0xffffffff));
  parpcarr05 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(CUTLX8(vl_0c), CUTLX8(RG7L), CUTLX8(RG6L));
  SKW_DRAW_ICON_PICT_BUFF(UPCAST(t_gfxdata, RG5p), RG2R, RG3L, c22p_00, parw02, parw03, lcon(0x0), parpcarr05);
void SKW_0b36_11c0(c_image* eaxp_c37, i32 ebxl, i32 ecxl, c_c5rects* edxp_c22)
  movp(location(RG5p + lcon(0x2c)), DOWNCAST(t_gfxdata, SKW_3e74_5817(edip_c22->c16_00.w_00)));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_PICT_BITS(UPCAST(c_hex18, RG5p)));
  RG1R = SKW_QUERY_BLIT_RECT(UPCAST(t_gfxdata, RG1P), RG2W, RG3P16, &rc_00, parpw00, lcon(0xffffffff));
  SKW_COPY_RECT(RG1R, &rc_00);
  SKW_OFFSET_RECT(edip_c22, RG2R, RG4R);
  SKW_DRAW_PICST(UPCAST(c_image, RG5p));
  SKW_0b36_0d67(edip_c22, &rc_00);
void SKW_0b36_129a(c_c5rects* eaxp_c22, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, t_text* argpt1)
  RG1L = SKW_QUERY_STR_METRICS(argpt1, RG2P16, RG4P16) ? 1 : 0;
  SKW_DRAW_STRING(SKW_3e74_5817(esip_c22->c16_00.w_00), RG2W, RG3W, esip_c22->c16_00.rc_02.w_04_w, parw00, CUTX16(parl01), par_tp02, lcon(0x8));
  RG1R = SKW_ALLOC_TEMP_RECT(RG1W, RG4W, RG2W, RG3W);
  SKW_0b36_0d67(esip_c22, RG4R);
i32 SKW_0b36_1446(xxx** xeaxpp, i32 ebxl, i32 ecxl, i32 edxl)
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_PICT_BITS(UPCAST(c_hex18, RG1P)));
  RG1W = SKW_3e74_5888();
  SKW_ALLOC_NEW_PICT(xw_1c.peek16(), CUTX16(RG2L), CUTLX8(RG3L), CUTX16(RG4L));
  RG1W = SKW_3e74_5888();
  RG1P = DOWNCAST(t_gfxdata, SKW_ALLOC_NEW_PICT(xw_20.peek16(), CUTX16(RG2L), CUTLX8(RG3L), CUTX16(RG4L)));
  RG5p = DOWNCAST(t_gfxdata, SKW_3e74_5817(xw_1c.peek16()));
  SKW_FILL_ENTIRE_PICT(UPCAST(t_gfxdata, RG5p), RG3Blo);
  SKW_FILL_ENTIRE_PICT(UPCAST(t_gfxdata, RG2P), RG3Blo);
  RG1R = SKW_ALLOC_TEMP_ORIGIN_RECT(lcon(0x1), lcon(0x1));
  SKW_FIRE_BLIT_PICTURE(UPCAST(t_gfxdata, DOWNCAST(t_gfxdata, SKW_QUERY_PICT_BITS(UPCAST(c_hex18, DOWNCAST(xxx, xp_0c))))), RG2R, RG3W, UPCAST(t_gfxdata, RG5p), parw00, parw01, parw02, parw03, lcon(0x0), parw05, parw06, NULL);
  RG3L = SKW_CALC_IMAGE_BYTE_LENGTH(UPCAST(t_gfxdata, RG5p));
  SKW_3e74_58a3(xw_1c.peek16());
  SKW_3e74_58a3(xw_20.peek16());
void SKW_DRAW_STATIC_PIC(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl, i32 argl0)
  SKW_QUERY_GDAT_SUMMARY_IMAGE(UPCAST(c_image, ADRESSOF(c_image, &c37_00)), RG2L, RG3L, RG4L);
  SKW_DRAW_PICST(XUPCAST(c_image, SKW_QUERY_PICST_IT(UPCAST(c_image, ADRESSOF(c_image, &c37_00)))));
void R_C470(xxx* xeaxp, i32 edxl, i32 ebxl, t_colidx* ecxpcarr)
  RG1R = SKW_QUERY_BLIT_RECT(UPCAST(t_gfxdata, RG5p), RG2W, RG3P16, RG4R, parpw00, lcon(0xffffffff));
  SKW_FIRE_BLIT_PICTURE(UPCAST(t_gfxdata, RG5p), &rc_00, vw_10, ddat.screenptr, parw02, parw03, parw04, parw05, lcon(0x0), parw07, lcon(0x8), ecxpcarr);
void SKW_DRAW_TRANSPARENT_STATIC_PIC(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl, i32 argl0)
  SKW_QUERY_GDAT_SUMMARY_IMAGE(UPCAST(c_image, ADRESSOF(c_image, &c37_00)), RG2L, RG3L, RG4L);
  SKW_DRAW_PICST(XUPCAST(c_image, SKW_QUERY_PICST_IT(UPCAST(c_image, ADRESSOF(c_image, &c37_00)))));
void SKW_RAISE_SYSERR(i32 eaxl)
  SKW_FIRE_SELECT_PALETTE_SET(RG1Blo);
  SKW_FORMAT_SKSTR(RG1T, tarr_00);
  SKW_COPY_TO_DRIVER(XDOWNCAST(t_text, tarr_00), lcon(0x100));
  SKW_PRINT_SYSERR_TEXT(lcon(0x0), lcon(0xf), lcon(0x0), lcon(0x10), par_tp00);
  SKW_SLEEP_SEVERAL_TIME(RG1W);
  RG1W = SKW_476d_050e();
  SKW_SK_PREPARE_EXIT();
void SKW_LOAD_LOCALLEVEL_GRAPHICS_TABLE(i32 eaxl, i32 ebxl, i32 edxl)
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  SKW_COPY_MEMORY(RG3P, RG2L, ddat.v1e02cc);
  SKW_COPY_MEMORY(RG3P, RG5l, ddat.v1e02dc);
  SKW_COPY_MEMORY(RG3P + RG5l, unsignedlong(RG1W), ddat.v1e0414);
i16* SKW_OVERSEE_RECORD(i16* eaxpw, i16** ebxppw, FPTR_L_WPWP ecxf, i32 edxl, i16* argpw0, i32 argl1, i32 argl2)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG5p))) + lcon(0x2);
  RG1P = DOWNCAST(i16, SKW_OVERSEE_RECORD(RG1P16, UPCAST(i16*, RG2P), pf_4, lcon(0xff), RG7p16, parl01, parl02)); // TODO
  RG1L = SKW_IS_CONTAINER_CHEST(unsignedlong(word_at(RG5p)));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG5p)));
  RG1P = DOWNCAST(i16, SKW_OVERSEE_RECORD(RG1P16, UPCAST(i16*, RG2P), pf_4, lcon(0xff), RG7p16, parl04, parl05)); // TODO
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG5p)));
i32 SKW_IS_TILE_PASSAGE(i32 eaxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_TILE_RECORD(RG1L, RG4L);
i32 SKW_GET_TILE_VALUE(i32 eaxl, i32 edxl)
  RG1L = SKW_IS_TILE_PASSAGE(signedlong(RG2W), lcon(0x0));
  RG1L = SKW_IS_TILE_PASSAGE(signedlong(RG2W), RG52l);
  RG1L = SKW_IS_TILE_PASSAGE(signedlong(CUTX16(RG2L - 1)), RG52l);
  RG1L = SKW_IS_TILE_PASSAGE(RG53l, RG4L);
  RG1L = SKW_IS_TILE_PASSAGE(RG53l, RG4L);
  RG1L = SKW_IS_TILE_PASSAGE(signedlong(RG2W), RG4L);
i32 SKW_0cee_04e5(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0)
  SKW_CALC_VECTOR_W_DIR(RG1L, RG2L, &vw_00, RG4L, &argw0);
  return SKW_GET_TILE_VALUE(signedlong(vw_00), signedlong(argw0));
i32 SKW_LOCATE_OTHER_LEVEL(i32 eaxl, i16* ebxpw, i16* ecxpw, i32 edxl, xxx** argpp0)
  SKW_CHANGE_CURRENT_MAP_TO(RG6l);
  RG1L = (SKW_GET_TILE_VALUE(RG6l, RG74l) & lcon(0xff)) >> bcon(0x5);
  RG1P = SKW_GET_ADDRESS_OF_TILE_RECORD(RG6l, RG4L);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_0c.peek16()));
i32 SKW_0cee_06dc(i32 eaxl, i32 edxl)
  RG1L = SKW_GET_TILE_VALUE(signedlong(RG1W), RG4L);
  RG1L = (SKW_GET_TILE_VALUE(signedlong(RG3W), RG4L) & lcon(0xff)) >> bcon(0x5);
i32 SKW_FIND_LADDAR_AROUND(i32 eaxl, i32 ebxl, xxx** xecxpp, i32 edxl)
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(CUTX16(RG2L + RG3L)), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG5W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG5W));
i32 SKW_0cee_0897(i16** eaxppw, i32 ebxl, i32 edxl)
  RG1L = (SKW_GET_TILE_VALUE(RG2L, RG5l) & lcon(0xff)) >> bcon(0x5) & lcon(0xff);
  RG1L = SKW_GET_TILE_RECORD_LINK(RG2L, RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
i32 SKW_GET_TELEPORTER_DETAIL(c_5bytes* eaxp_c12, i32 ebxl, i32 edxl)
  RG1L = SKW_0cee_0897(&wp_04, RG2L, RG4L);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = SKW_0cee_0897(&wp_00, RG2L, RG4L);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
xxx* R_D283(i32 eaxl, i32 edxl)
  RG1L = SKW_GET_TILE_VALUE(RG3L, RG4L);
  RG1L = SKW_GET_TELEPORTER_DETAIL(&c12_04, RG2L, RG4L);
  RG1L = SKW_GET_TELEPORTER_DETAIL(&c12_04, RG2L, RG4L);
  RG1L = SKW_GET_TELEPORTER_DETAIL(&c12_04, RG2L, RG4L);
  RG1L = SKW_GET_TELEPORTER_DETAIL(&c12_04, RG2L, RG4L);
  RG1P = SKW_GET_ADDRESS_OF_TILE_RECORD(signedlong(vql_00.peek16()), RG4L);
  RG1L = signedlong(SKW_CALC_SQUARE_DISTANCE(unsignedword(c12_04.b_02), RG2W, RG3W, RG4W));
xxx* SKW_GET_ADDRESS_OF_TILE_RECORD(i32 eaxl, i32 edxl)
  return SKW_GET_ADDRESS_OF_RECORD(RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(RG1W), signedlong(RG4W)) & lcon(0xffff));
i32 SKW_GET_NEXT_RECORD_LINK(i32 eaxl)
  RG1W = word_at(SKW_GET_ADDRESS_OF_RECORD(eaxl & lcon(0xffff)));
i32 SKW_GET_TILE_RECORD_LINK(i32 eaxl, i32 edxl)
  RG1L = signedlong(CUTX16(SKW_GET_OBJECT_INDEX_FROM_TILE(signedlong(RG1W), RG4L)));
i32 SKW_GET_WALL_TILE_ANYITEM_RECORD(i32 eaxl, i32 edxl)
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(RG1W), RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG1W));
void SKW_APPEND_RECORD_TO(i32 eaxl, i32 ebxl, i32 ecxl, xxx* xedxp)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(vql_00.peek16()));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG53L & lcon(0xffff));
  SKW_RAISE_SYSERR(lcon(0x48));
  SKW_COPY_MEMORY(RG3P, RG2L, RG3P + lcon(0x2));
  RG1L = SKW_GET_TILE_RECORD_LINK(RG1L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG1W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
void SKW_CUT_RECORD_FROM(i32 eaxl, i32 ebxl, i32 ecxl, xxx* xedxp)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG1L);
  RG1L = SKW_GET_OBJECT_INDEX_FROM_TILE(RG62l, RG7l) & lcon(0xffff);
  SKW_COPY_MEMORY(RG1P, RG2L, RG4P);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG4P)));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG52w));
void SKW_SET_ITEM_IMPORTANCE(i32 eaxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG2W));
i32 SKW_RECYCLE_A_RECORD_FROM_THE_WORLD(i32 eaxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG6W));
  RG1L = SKW_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(RG4L);
  RG1L = SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG6W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG6W));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG4P, lcon(0x4))));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG6W));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(vo_30.peek16()));
  SKW_DELETE_CREATURE_RECORD(unsignedlong(vo_24.peek16()), lcon(0x1), lcon(0xffffffff), RG4L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(unsignedlong(word_at(RG1P, lcon(0x2))));
  SKW_075f_056c(RG74L);
  SKW_CUT_RECORD_FROM(RG74L, RG1L, vl_08, NULL);
  SKW_DELETE_MISSILE_RECORD(RG74L, vl_00, vl_08, NULL);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  SKW_MOVE_RECORD_TO(RG1L, RG4L, RG2L, lcon(0xffffffff), parw00);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG6W));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_28.peek16()));
i32 SKW_ALLOC_NEW_RECORD(i32 eaxl)
  RG1L = SKW_RECYCLE_A_RECORD_FROM_THE_WORLD(unsignedlong(RG5W), lcon(0xff));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  DM2_ZERO_MEMORY(RG2P, RG4L);
i32 SKW_ALLOC_NEW_DBITEM(i32 eaxl)
  RG1L = SKW_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(RG2L);
  RG1L = SKW_ALLOC_NEW_RECORD(unsignedlong(RG1W));
  RG1L = SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG2L);
  SKW_SET_ITEMTYPE(RG1L, RG2L);
i32 SKW_ALLOC_NEW_DBITEM_DEEPLY(i32 eaxl)
  RG1L = SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG2L);
  RG1L = SKW_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(RG2L);
  RG1L = SKW_RECYCLE_A_RECORD_FROM_THE_WORLD(RG2L, RG4L);
  RG1L = SKW_ALLOC_NEW_RECORD(RG2L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG52l);
  DM2_ZERO_MEMORY(RG1P, RG4L);
  SKW_SET_ITEMTYPE(RG52l, unsignedlong(vb_00));
void SKW_DEALLOC_RECORD(i32 eaxl)
  mov16(location(SKW_GET_ADDRESS_OF_RECORD(eaxl & lcon(0xffff))), lcon(0xffffffff));
void SKW_QUERY_MESSAGE_TEXT(t_text* eaxtp, i32 ebxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1L = signedlong(CUTX16(SKW_SK_STRLEN(SKW_QUERY_GDAT_TEXT(lcon(0x3), RG2L, RG3T, lcon(0x0)))));
  SKW_SK_STRCAT(UPCAST(t_text, RG5p), RG2T);
  RG1L = signedlong(CUTX16(SKW_SK_STRLEN(RG2T)));
  SKW_FORMAT_SKSTR(tarr_04, UPCAST(t_text, DOWNCAST(t_text, tp_00)));
i32 SKW_0cee_1815(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl, i16 argw0)
  RG1L = SKW_0cee_17e7(32 * RG2L + lcon(0x7d0) + RG3L & lcon(0xffff), RG4L, RG6L);
void SKW_0cee_185a(xxx* xeaxp, i32 ebxl, i32 ecxl, i32 edxl, i16 argw0, i32 argl1, i16 argw2, i16 argw3)
  RG1L = SKW_0cee_1815(signedlong(RG4W), RG6l, RG3L, RG7l, lcon(0x1e));
  RG1L = SKW_0cee_1815(signedlong(xw_04.peek16()), RG6l, RG3L, RG7l, lcon(0x1e));
  RG1L = SKW_0cee_1815(signedlong(xw_08.peek16()), RG6l, RG3L, RG7l, lcon(0x1e));
  RG1L = SKW_0cee_1815(RG1L, RG2L, RG3L, RG4L, lcon(0x1e));
  RG1L = SKW_IS_WALL_ORNATE_ALCOVE(unsignedlong(byte_at(RG4P)));
i32 SKW_QUERY_ORNATE_ANIM_FRAME(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(RG6L, RG2L, RG3T, RG4L));
  RG1L = SKW_SK_STRLEN(RG2T);
i32 SKW_0cee_1a46(s_8wordarray* xeaxp, i32 ebxl, i32 ecxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(vql_18.peek16()));
  RG1L = SKW_GET_WALL_DECORATION_OF_ACTUATOR(RG3P);
  RG1L = SKW_QUERY_ORNATE_ANIM_FRAME(lcon(0x9), RG2L, RG3L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_18.peek16()));
void SKW_SUMMARIZE_STONE_ROOM(s_8wordarray* eaxp_s15, i32 ebxl, i32 ecxl, i32 edxl)
  RG1L = SKW_GET_TILE_VALUE(RG71l, RG4L);
  RG1L = SKW_GET_TILE_RECORD_LINK(RG71l, RG4L);
  RG1L = SKW_GET_TILE_RECORD_LINK(RG71l, RG4L);
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(RG3W), RG4L);
  SKW_0cee_1a46(UPCAST(s_8wordarray, RG1P), RG2L, RG3L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG61W));
  SKW_0cee_185a(RG5p, signedlong(xw_30.peek16()), signedlong(xw_10.peek16()), signedlong(xw_2c.peek16()), parw00, parl01, parw02, parw03);
  RG1L = SKW_0cee_1a46(UPCAST(s_8wordarray, RG5p), RG2L, RG3L, RG4L);
  RG1L = (SKW_GET_TILE_VALUE(signedlong(RG3W), RG4L) & lcon(0xff)) >> bcon(0x5) & lcon(0xff);
  RG1L = SKW_QUERY_ORNATE_ANIM_FRAME(lcon(0xa), ddat.mallocx, lcon(0x0), unsignedlong(vo_20.peek8())) * lcon(0xa) << bcon(0x8);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG61W));
  RG1L = SKW_GET_FLOOR_DECORATION_OF_ACTUATOR(RG4P);
  vl_28 = SKW_QUERY_ORNATE_ANIM_FRAME(lcon(0xa), RG72L, vl_04, unsignedlong(vb_34));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG61W));
i32 SKW_GET_DISTINCTIVE_ITEMTYPE(i32 eaxl)
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(unsignedlong(RG4W));
i32 SKW_QUERY_CLS2_FROM_RECORD(i32 eaxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG2L);
  return SKW_QUERY_CLS2_OF_TEXT_RECORD(RG2L);
  return SKW_GET_WALL_DECORATION_OF_ACTUATOR(SKW_GET_ADDRESS_OF_RECORD(RG2L));
void SKW_SET_ITEMTYPE(i32 eaxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG2W));
i32 SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(i32 eaxl, i32 edxl)
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L);
  return lextendedDM2_QUERY_GDAT_ENTRY_DATA_INDEX(CUTLX8(SKW_QUERY_CLS1_FROM_RECORD(RG4L)) & bcon(0xff), lcon(0xb), CUTLX8(RG3L), RG1Blo);
i32 SKW_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(i32 eaxl)
  return SKW_QUERY_CLS1_FROM_RECORD(SKW_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(eaxl & lcon(0xffff)) << bcon(0xa) & lcon(0xffff));
i32 SKW_QUERY_CLS1_FROM_RECORD(i32 eaxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
i32 SKW_IS_CONTAINER_MONEYBOX(i32 eaxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L);
i32 SKW_IS_CONTAINER_CHEST(i32 eaxl)
  RG1L = SKW_IS_CONTAINER_MONEYBOX(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
i32 SKW_IS_MISCITEM_CURRENCY(i32 eaxl)
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG1W), lcon(0x0));
void SKW_COUNT_BY_COIN_TYPES(i32 eaxl, i16* edxpwarr)
  DM2_ZERO_MEMORY(RG1P, RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG2W));
  RG1L = SKW_IS_MISCITEM_CURRENCY(RG4L);
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG5w));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG5w));
i32 SKW_ADD_COIN_TO_WALLET(i32 eaxl, i32 edxl)
  RG1L = SKW_IS_CONTAINER_MONEYBOX(RG4L);
  RG1L = SKW_IS_MISCITEM_CURRENCY(RG2L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG2L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  SKW_DEALLOC_RECORD(RG1L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
  SKW_APPEND_RECORD_TO(RG1L, lcon(0xffffffff), lcon(0x0), RG4P);
i32 SKW_TAKE_COIN_FROM_WALLET(i32 eaxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(unsignedlong(RG4W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG6L);
  SKW_CUT_RECORD_FROM(RG6L, lcon(0xffffffff), lcon(0x0), RG3P + lcon(0x2));
  return SKW_ALLOC_NEW_DBITEM(unsignedlong(RG2W));
i32 SKW_ADD_ITEM_CHARGE(i32 eaxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG51w));
  RG1L = SKW_BETWEEN_VALUE16(lcon(0x0), RG2L, RG4L);
i32 SKW_QUERY_GDAT_FOOD_VALUE_FROM_RECORD(i32 eaxl)
  return SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG1W), lcon(0x3));
i32 SKW_QUERY_ITEM_VALUE(i32 eaxl, i32 edxl)
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG3L, RG4L);
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG3L, RG4L);
  RG1L = signedlong(CUTX16(SKW_ADD_ITEM_CHARGE(RG3L, RG4L))) * RG61L;
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG3L, RG4L);
  RG1L = signedlong(CUTX16(SKW_ADD_ITEM_CHARGE(RG3L, RG4L))) * RG62L;
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG2W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  vl_00 = SKW_IS_CONTAINER_MONEYBOX(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG3L);
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG3L, RG4L) & lcon(0xffff);
  RG1L = SKW_QUERY_ITEM_VALUE(unsignedlong(RG2W), RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG2W));
i32 SKW_QUERY_ITEM_WEIGHT(i32 eaxl)
  return SKW_QUERY_ITEM_VALUE(unsignedlong(RG1W), lcon(0x1));
i32 R_F958(i32 eaxl)
  RG1L = SKW_QUERY_ITEM_VALUE(unsignedlong(RG1W), lcon(0x2));
xxx* SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(i32 eaxl)
  return SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(SKW_GET_ADDRESS_OF_RECORD(eaxl & lcon(0xffff)), lcon(0x4))));
xxx* SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(i32 eaxl)
  return UPCAST(xxx, table1d296c[unsignedlong(CUTX16(SKW_QUERY_GDAT_CREATURE_WORD_VALUE(eaxl & lcon(0xff), lcon(0x5))))]);
i32 SKW_QUERY_CREATURE_AI_SPEC_FLAGS(i32 eaxl)
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(eaxl & lcon(0xffff));
i32 SKW_0cee_2df4(i32 eaxl)
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(eaxl & lcon(0xffff));
i32 SKW_0cee_2e09(i32 eaxl)
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(eaxl & lcon(0xffff));
i32 SKW_GET_CREATURE_WEIGHT(i32 eaxl)
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(eaxl & lcon(0xffff));
i32 SKW_0cee_2e35(i32 eaxl)
  RG1L = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(RG1Blo), lcon(0x4));
i32 SKW_IS_CREATURE_ALLOWED_ON_LEVEL(i32 eaxl, i32 edxl)
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(RG4L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L);
i32 SKW_IS_CONTAINER_MAP(i32 eaxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
i32 SKW_SET_DESTINATION_OF_MINION_MAP(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG6W));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_00.peek16()));
xxx* SKW_GET_MISSILE_REF_OF_MINION(i32 eaxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
i32 SKW_QUERY_CLS2_OF_TEXT_RECORD(i32 eaxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
i32 SKW_IS_WALL_ORNATE_ALCOVE_FROM_RECORD(i32 eaxl, i32 edxl)
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L);
  RG1L = signedlong(CUTX16(SKW_IS_WALL_ORNATE_ALCOVE(unsignedlong(vb_10))));
  RG1L = SKW_0cee_1a46(UPCAST(s_8wordarray, ADRESSOF(s_8wordarray, &s15_00)), RG1L, lcon(0x1), RG4L);
i32 SKW_0cee_317f(i32 eaxl, i32 edxl)
  RG1L = SKW_IS_WALL_ORNATE_ALCOVE_FROM_RECORD(unsignedlong(RG1W), RG2L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG2L);
i32 SKW_0cee_319e(i32 eaxl)
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(unsignedlong(RG1W));
i32 SKW_IS_WALL_ORNATE_SPRING(i32 eaxl)
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(unsignedlong(RG1W));
i32 SKW_QUERY_DOOR_STRENGTH(i32 eaxl)
  return SKW_0cee_3275(RG5l) == lcon(0x0) ? lcon(0x6) : lcon(0x1);
void SKW_END_GAME(i32 eaxl)
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_QUEUE_NOISE_GEN2(lcon(0x16), RG2Blo, RG3Blo, RG4Blo, parw00, parw01, parw02, lcon(0xff), lcon(0xff));
  SKW_SLEEP_SEVERAL_TIME(lcon(0xf0));
  SKW_2066_03e0(lcon(0x0));
  SKW_FIRE_FILL_SCREEN_RECT(RG1W, RG4Blo);
  RG1L = SKW_0aaf_0067(SKW_0aaf_02f8(lcon(0x10), lcon(0x0)) & lcon(0xff));
  RG1L = signedlong(CUTX16(SKW_GAME_LOAD()));
  SKW__INIT_GAME_38c8_03ad();
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_1031_098e();
  SKW_FIRE_FILL_SCREEN_RECT(RG1W, RG4Blo);
  SKW_FIRE_SELECT_PALETTE_SET(RG1Blo);
  SKW_SK_PREPARE_EXIT();
  SKW_SK_PREPARE_EXIT();
  SKW_DRAW_TRANSPARENT_STATIC_PIC(lcon(0x6), RG2L, RG3L, RG4L, parl05);
  SKW_FIRE_SELECT_PALETTE_SET(RG1Blo);
  SKW_1031_0541(lcon(0x1));
  SKW_EVENT_LOOP();
  SKW_SK_PREPARE_EXIT();
i32 SKW_1031_009e(xxx* xeaxp)
  RG1L = SKW_GET_PLAYER_AT_POSITION(unsignedlong(mkuw(unsignedword(byte_at(RG1P, lcon(0x1))) + ddat.v1e0258 & uwcon(0x3))));
i32 SKW_1031_00f3(xxx* xeaxp)
  RG1L = SKW_GET_PLAYER_AT_POSITION(unsignedlong(mkuw(unsignedword(RG4Blo) + ddat.v1e0258 & uwcon(0x3))));
c_rect* SKW_1031_01d5(i32 eaxl, c_rect* edxrp)
  RG1R = SKW_QUERY_EXPANDED_RECT(RG1W, RG4R);
  SKW_QUERY_TOPLEFT_OF_RECT(RG1W, UPCAST(i16, ADRESSOF(c_q32, &vq_04)), UPCAST(i16, ADRESSOF(c_q32, &vq_00)));
void SKW_1031_027e(xxx* xeaxp)
  RG1P = SKW_1031_023b(RG1P);
  RG1L = SKW_RETURN_1();
  RG1L = SKW_IS_GAME_ENDED(RG1P);
  RG1L = SKW_1031_0023(RG1P);
  RG1L = SKW_1031_003e(RG1P);
  RG1L = SKW_1031_007b(RG1P);
  RG1L = SKW_1031_009e(RG1P);
  RG1L = SKW_1031_00c5(RG1P);
  RG1L = SKW_1031_00f3(RG1P);
  RG1L = SKW_1031_012d(RG1P);
  RG1L = SKW_1031_014f(RG1P);
  RG1L = SKW_1031_0184(RG1P);
  RG1L = SKW_1031_01ba(RG1P);
  SKW_1031_027e(RG1P);
i32 SKW_1031_030a(xxx* xeaxp, i32 ebxl, i32 ecxl, i32 edxl)
  RG1P = SKW_1031_023b(RG1P);
  RG1L = SKW_RETURN_1();
  RG1L = SKW_IS_GAME_ENDED(RG1P);
  RG1L = SKW_1031_0023(RG1P);
  RG1L = SKW_1031_003e(RG1P);
  RG1L = SKW_1031_007b(RG1P);
  RG1L = SKW_1031_009e(RG1P);
  RG1L = SKW_1031_00c5(RG1P);
  RG1L = SKW_1031_00f3(RG1P);
  RG1L = SKW_1031_012d(RG1P);
  RG1L = SKW_1031_014f(RG1P);
  RG1L = SKW_1031_0184(RG1P);
  RG1L = SKW_1031_01ba(RG1P);
  RG1R = SKW_1031_01d5(RG1L, RG4R);
  RG1L = SKW_PT_IN_RECT(RG1R, RG7W, RG2W) ? 1 : 0;
  RG1P = DOWNCAST(s_www, SKW_1031_024c(RG52p));
  RG1L = SKW_1031_0a88(RG1P, RG2L, RG3L, RG4L);
  RG1L = SKW_1031_030a(RG52p, RG2L, RG3L, RG4L);
i32 SKW_1031_03f2(xxx* xeaxp, i32 edxl)
  RG1P = SKW_1031_023b(RG1P);
  RG1L = SKW_RETURN_1();
  RG1L = SKW_IS_GAME_ENDED(RG1P);
  RG1L = SKW_1031_0023(RG1P);
  RG1L = SKW_1031_003e(RG1P);
  RG1L = SKW_1031_007b(RG1P);
  RG1L = SKW_1031_009e(RG1P);
  RG1L = SKW_1031_00c5(RG1P);
  RG1L = SKW_1031_00f3(RG1P);
  RG1L = SKW_1031_012d(RG1P);
  RG1L = SKW_1031_014f(RG1P);
  RG1L = SKW_1031_0184(RG1P);
  RG1L = SKW_1031_01ba(RG1P);
  RG1P = DOWNCAST(s_www, SKW_1031_024c(RG3P));
  RG1L = SKW_1031_0c58(RG3L, RG4P);
  RG1L = SKW_1031_03f2(RG3P, RG4L);
i32 SKW_1031_04F5(i32 eaxl)
  return SKW_29ee_000f();
void SKW_1031_050C(void)
  SKW_FIRE_MOUSE_RELEASE_CAPTURE();
  SKW_FIRE_SHOW_MOUSE_CURSOR();
void R_10777(void)
  SKW_CHAMPION_SQUAD_RECOMPUTE_POSITION();
  SKW_1031_050C();
i32 R_107B0(void)
  return SKW_1031_0541(signedlong(ddat.v1d3ff1));
i32 SKW_1031_0541(i32 eaxl)
  SKW_1031_098e();
  SKW_1031_027e(RG1P);
  SKW_443c_07d5(RG4P);
  SKW_443c_06b4(RG4P);
i32 SKW_1031_0675(i32 eaxl)
  R_10777();
  return SKW_1031_0541(signedlong(RG4W));
i32 SKW_1031_06a5(void)
  return SKW_1031_0541(signedlong(ddat.v1e0510));
xxx* SKW_1031_06b3(xxx* xeaxp, i32 edxl)
  RG1P = SKW_1031_023b(RG1P);
  RG1L = SKW_RETURN_1();
  RG1L = SKW_IS_GAME_ENDED(RG1P);
  RG1L = SKW_1031_0023(RG1P);
  RG1L = SKW_1031_003e(RG1P);
  RG1L = SKW_1031_007b(RG1P);
  RG1L = SKW_1031_009e(RG1P);
  RG1L = SKW_1031_00c5(RG1P);
  RG1L = SKW_1031_00f3(RG1P);
  RG1L = SKW_1031_012d(RG1P);
  RG1L = SKW_1031_014f(RG1P);
  RG1L = SKW_1031_0184(RG1P);
  RG1L = SKW_1031_01ba(RG1P);
  RG1P = DOWNCAST(s_www, SKW_1031_024c(RG3P));
  RG1P = SKW_1031_06b3(RG3P, RG4L);
void SKW_1031_0781(i32 eaxl)
  RG1P = SKW_1031_06b3(DOWNCAST(i32, table1d3ed5 + signedlong(ddat.v1d3ff1)), RG4L); // 1st par adj
  RG1R = SKW_1031_01d5(RG1L, RG4R);
  SKW_FIRE_QUEUE_MOUSE_EVENT(rc_00.w_00_x, rc_00.w_02_y, RG2W);
i32 SKW_1031_07d6(void)
  SKW_443c_067a(RG1P);
i32 SKW_1031_0a88(xxx* xeaxp, i32 ebxl, i32 ecxl, i32 edxl)
  RG1R = SKW_1031_01d5(RG1L, RG4R);
  RG1L = SKW_PT_IN_RECT(RG1R, RG4W, RG2W) ? 1 : 0;
i32 SKW_1031_0c58(i32 eaxl, xxx* xedxp)
  SKW_1031_01d5(RG1L, RG4R);
void SKW_1031_0d36(i32 eaxl, i32 edxl)
  SKW_3e74_5b7c(RG1L);
  SKW_GETSINGLEMOUSEEVENT();
xxx* SKW_TRANSMIT_UI_EVENT(xxx* xeaxp)
  SKW_ADJUST_UI_EVENT(RG1P);
void SKW_IBMIO_USER_INPUT_CHECK(void)
  SKW_1031_111e(signedlong(vql_08.peek16()));
  SKW_1031_111e(signedlong(vql_08.peek16()));
  RG1W = SKW_476d_05a9();
  RG1W = SKW_476d_050e();
  SKW_1031_0d36(lcon(0x20), RG4L);
  SKW_GETSINGLEMOUSEEVENT();
  SKW_1031_111e(signedlong(vql_08.peek16()));
  RG1L = SKW_1031_030a(DOWNCAST(i32, table1d3ed5 + signedlong(ddat.v1d3ff1)), RG2L, RG3L, RG4L); // 1st par adj
  RG1L = SKW_1031_03f2(DOWNCAST(i32, table1d3ed5 + signedlong(ddat.v1d3ff1)), RG4L); // 1st par adj
  SKW_GETSINGLEMOUSEEVENT();
  ddat.v1e0480 = SKW_TRANSMIT_UI_EVENT(DOWNCAST(c_wwwr, &ddat.v1e04d2));
void SKW_1031_10c8(c_c5rects* eaxp_c22, i32 ebxl, i32 ecxl, c_rect* edxrp)
  SKW_COPY_RECT(RG1R, &esip_c22->c16_00.rc_02);
  SKW_0b36_0c52(esip_c22, lcon(0x0), lcon(0xffffffff));
  SKW_CALC_CENTERED_RECT_IN_RECT(RG6r, signedlong(xw_00.peek16()), RG3L, &esip_c22->c16_00.rc_02);
void SKW_1031_111e(i32 eaxl)
  RG1L = SKW_HANDLE_UI_EVENT(DOWNCAST(c_wwwr, &ddat.v1e04d2));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(CUTLX8(RG52l), CUTLX8(RG2L), CUTLX8(RG4L)));
  SKW_1031_10c8(&c22_00, signedlong(word_at(RG1P, -lcon(0x4))), signedlong(word_at(RG1P, -lcon(0x2))), &rc_34);
  parpcarr03 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(CUTLX8(RG52l), CUTLX8(RG62L), CUTLX8(RG7L));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(CUTLX8(RG52l), CUTLX8(RG2L), CUTLX8(RG4L)));
  SKW_DRAW_ICON_PICT_BUFF(UPCAST(t_gfxdata, RG1P), RG2R, RG3L, &c22_00, lcon(0x0), parw01, lcon(0x0), parpcarr03);
  RG1L = SKW_GET_PLAYER_AT_POSITION(unsignedlong(unsignedword(byte_at(RG1P) + mkuw(ddat.v1e0258) & uwcon(0x3)))); // cast necessary!
  SKW_DRAW_SQUAD_SPELL_AND_LEADER_ICON(RG1L, lcon(0x1));
  SKW_0b36_0cbe(UPCAST(c_c5rects, RG1P), RG4L);
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x1), lcon(0x8), lcon(0x5)));
  SKW_1031_10c8(&c22_00, RG2L, RG3L, RG4R);
  parpcarr07 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(lcon(0x1), lcon(0x8), lcon(0x5));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x1), lcon(0x8), lcon(0x5)));
  SKW_DRAW_ICON_PICT_BUFF(UPCAST(t_gfxdata, RG1P), RG2R, lcon(0x0), &c22_00, lcon(0x0), lcon(0xffff), lcon(0x0), parpcarr07);
  SKW_QUERY_STR_METRICS(ddat.v1e052c, &vw_3c, &vw_40);
  SKW_CALC_CENTERED_RECT_IN_RECT(RG1R, RG2L, RG3L, RG4R);
  SKW_0b36_129a(&c22_00, signedlong(rc_34.w_00_x), RG2L, RG3L, parw08, par_tp09);
  SKW_DRAW_SPELL_TO_BE_CAST(lcon(0x1));
  SKW_0b36_0cbe(UPCAST(c_c5rects, RG1P), RG4L);
  SKW_DRAW_CMD_SLOT(RG1L, lcon(0x1));
  SKW_0b36_0cbe(UPCAST(c_c5rects, RG1P), RG4L);
  SKW_REFRESH_PLAYER_STAT_DISP(RG1L);
  SKW_29ee_1d03(RG1L);
  SKW_0b36_0cbe(UPCAST(c_c5rects, RG1P), RG4L);
  SKW_0aaf_01db(RG1L, lcon(0x1));
  SKW_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
  SKW_DRAW_HAND_ACTION_ICONS(RG1L, RG2L, RG4L);
  SKW_0b36_0cbe(UPCAST(c_c5rects, RG1P), RG4L);
  SKW_0b36_0cbe(&c22_00, lcon(0x1));
void SKW_ADJUST_UI_EVENT(xxx* xeaxp)
  RG1L = SKW_GET_PLAYER_AT_POSITION(unsignedlong(mkuw(CUTX16(RG4L - lcon(0x5f)) + ddat.v1e0258 & wcon(0x3))));
  RG1L = SKW_GET_PLAYER_AT_POSITION(signedlong(ddat.v1e0258) + RG4L & lcon(0x3) & lcon(0xffff));
  RG1L = SKW_IS_ITEM_HAND_ACTIVABLE(unsignedlong(RG52W), RG2L, RG4L);
void SKW_CLICK_MAGICAL_MAP_AT(i32 eaxl, i32 ebxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG6L);
  SKW_QUERY_EXPANDED_RECT(RG1W, RG4R);
  RG1P = SKW_GET_MISSILE_REF_OF_MINION(unsignedlong(word_at(RG51p, lcon(0x2))), RG6L);
  SKW_CALC_VECTOR_W_DIR(signedlong(ddat.v1e0b84), RG1L, &vw_18, RG4L, &vw_14);
  RG1L = SKW_LOCATE_OTHER_LEVEL(signedlong(ddat.v1e0b81), &vw_18, &vw_14, lcon(0x0), NULL);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = SKW_GET_TILE_VALUE(signedlong(vw_18), RG4L);
  SKW_SET_DESTINATION_OF_MINION_MAP(unsignedlong(xwc_1c.peek16()), signedlong(vw_14), RG3L, signedlong(vw_18));
  SKW_1c9a_0247(RG1L);
  SKW_UPDATE_RIGHT_PANEL(lcon(0x0));
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
void SKW_DRAW_WAKE_UP_TEXT(void)
  SKW_FILL_ENTIRE_PICT(ddat.bitmapptr, RG4Blo);
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x1), lcon(0x11), RG3T, lcon(0x0)));
  SKW_DRAW_VP_RC_STR(lcon(0x6), RG1T, RG4W);
i32 SKW_HANDLE_UI_EVENT(xxx* xeaxp)
  SKW_CLICK_ITEM_SLOT(RG1L);
  SKW_SK_PREPARE_EXIT();
  SKW_SHOW_CREDITS();
  R_AB26();
  SKW_REFRESH_PLAYER_STAT_DISP(RG1L);
  SKW_GAME_SAVE_MENU();
  SKW_RESUME_FROM_WAKE();
  SKW_24a5_1798(lcon(0x4));
  SKW_38c8_0002(SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS());
  SKW_PERFORM_MOVE(unsignedlong(ddat.v1e0256));
  SKW_DRAW_WAKE_UP_TEXT();
  SKW_CHANGE_VIEWPORT_TO_INVENTORY(lcon(0x0));
  SKW_1031_0541(lcon(0x2));
  SKW_38c8_0060();
  SKW_1031_06a5();
  SKW_1031_098e();
  SKW_38c8_0002(RG1L);
  SKW_FILL_ENTIRE_PICT(ddat.bitmapptr, RG4Blo);
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x1), lcon(0x12), RG3T, lcon(0x0)));
  SKW_DRAW_VP_RC_STR(lcon(0x6), RG2T, RG4W);
  SKW_CHANGE_VIEWPORT_TO_INVENTORY(lcon(0x0));
  SKW_1031_0675(lcon(0x3));
  SKW_1031_098e();
  R_B37();
  SKW_CLICK_MAGICAL_MAP_RUNE(RG1L);
  SKW_CLICK_MAGICAL_MAP_AT(RG1L, RG2L, RG4L);
  R_13262(RG1L);
  SKW_CLICK_VWPT(RG1L, RG4L);
  R_39796(RG1P);
  SKW_CLICK_INVENTORY_EYE();
  SKW_PLAYER_CONSUME_OBJECT(RG2L, RG2L, lcon(0xffff));
  RG1L = SKW_GET_PLAYER_AT_POSITION(unsignedlong(mkuw(RG1W - wcon(0x5f) + ddat.v1e0258 & wcon(0x3))));
  SKW_SET_SPELLING_CHAMPION(RG1L);
  SKW_ACTIVATE_ACTION_HAND(RG1L, RG4L);
  RG1L = SKW_PROCEED_COMMAND_SLOT(signedlong(RG1W));
  SKW_PROCEED_COMMAND_SLOT(lcon(0xffffffff));
  SKW_ADD_RUNE_TO_TAIL(RG1L);
  SKW_REMOVE_RUNE_FROM_TAIL();
  RG1L = SKW_TRY_CAST_SPELL();
  SKW_2f3f_04ea(signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), signedlong(ddat.v1e0258), signedlong(ddat.v1e0266), parw00); // ATTENTION: there are two parw00
  SKW_24a5_1798(RG1L);
  RG1L = signedlong(CUTX16(SKW_GET_PLAYER_AT_POSITION(unsignedlong(mkuw(RG1W - wcon(0x10) + ddat.v1e0258 & wcon(0x3))))));
  SKW_SELECT_CHAMPION_LEADER(RG1L);
  SKW_2e62_0cfa(lcon(0x0));
  SKW_CLICK_MONEYBOX(RG1L);
  SKW_CHANGE_PLAYER_POS(RG1L);
  SKW_CHAMPION_SQUAD_RECOMPUTE_POSITION();
  R_107B0();
  SKW_UPDATE_RIGHT_PANEL(lcon(0x0));
  SKW_PUT_ITEM_TO_PLAYER(RG1L);
  SKW_PERFORM_MOVE(RG1L);
  SKW_PERFORM_TURN_SQUAD(RG1L);
  SKW_1031_04F5(RG1L);
  SKW_1031_050C();
void SKW_EVENT_LOOP(void)
  SKW_IBMIO_USER_INPUT_CHECK();
  RG1L = SKW_HANDLE_UI_EVENT(DOWNCAST(i16, table1e04e0 + 7 * RG2L));
void SKW_121e_0003(i32 eaxl)
  R_3C1E5(RG1L, RG4L, RG2L, lcon(0xffff), parl00);
void SKW_PLAYER_TESTING_WALL(i32 eaxl, i32 edxl, i32 ebxl)
  SKW_QUEUE_NOISE_GEN2(lcon(0x8), lcon(0x84), lcon(0xfe), ddat.v1d6c02, parw00, parw01, parw02, lcon(0x8c), lcon(0xc8));
  SKW_FIRE_MOUSE_SET_CAPTURE();
  SKW_443c_08ab(&ddat.v1e00b6, &ddat.v1e00b2, &ddat.v1e00b4);
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_FIRE_MOUSE_RELEASE_CAPTURE();
void SKW_121e_013a(i32 eaxl, i32 edxl, i32 ebxl)
  RG1L = SKW_32cb_03a6(RG1L, RG4L, RG2L, RG3L, parw00, parl01, parw02, parl03);
  SKW_MOVE_RECORD_TO(RG1L, RG62l, RG7L, lcon(0xffffffff), parw04);
  RG1L = SKW_IS_CONTAINER_MONEYBOX(RG53l);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG53l);
  SKW_TAKE_OBJECT(unsignedlong(xwc_08.peek16()), lcon(0x1));
i32 SKW_121e_0222(i32 eaxl, i32 edxl, i32 ebxl)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG6w), RG4L);
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(RG3L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG3L);
  vl_00 = SKW_REMOVE_OBJECT_FROM_HAND();
  RG1L = SKW_IS_CONTAINER_MONEYBOX(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG2W));
  SKW_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw00);
void SKW_121e_0351(i32 eaxl, i32 edxl)
  RG1L = SKW_PT_IN_EXPANDED_RECT(xw_00.peek16(), RG5w, RG6w) ? 1 : 0;
  RG1L = SKW_PT_IN_EXPANDED_RECT(xw_00.peek16() + 1, RG5w, RG6w) ? 1 : 0;
  RG1L = R_37BBB(RG1L);
i32 SKW_121e_03ae(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1, i16 argw2)
  RG1L = SKW_32cb_03a6(signedlong(RG1W), RG4L, RG2L, RG52l, parw00, parl01, parw02, parl03);
  SKW_REMOVE_OBJECT_FROM_HAND(); // return not needed
  return SKW_121e_0222(RG52l, RG62L, RG2L);
  return SKW_121e_0222(RG52l, RG62L, RG2L);
void SKW_CLICK_VWPT(i32 eaxl, i32 edxl)
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
  SKW_PUSH_PULL_RIGID_BODY(signedlong(ddat.v1d324a));
  SKW_29ee_000f();
  RG1L = (SKW_GET_TILE_VALUE(signedlong(vo_38.peek16()), signedlong(vo_54.peek16())) & lcon(0xff)) >> bcon(0x5);
  RG1L = SKW_PT_IN_RECT(RG1R, RG4W, RG2W) ? 1 : 0;
  SKW_121e_013a(signedlong(vol_04.peek16()), signedlong(vol_00.peek16()), RG2L);
  RG1L = SKW_121e_03ae(signedlong(vol_04.peek16()), signedlong(vol_00.peek16()), signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), lcon(0x0), parl10, lcon(0x1));
  RG1L = SKW_121e_03ae(signedlong(vol_04.peek16()), signedlong(vol_00.peek16()), signedlong(vo_38.peek16()), signedlong(vo_54.peek16()), lcon(0x0), lcon(0x2), parw08);
  RG1P = SKW_GET_ADDRESS_OF_TILE_RECORD(RG1L, RG4L);
  R_3C1E5(RG1L, RG4L, RG2L, lcon(0xffff), parl05);
  SKW_QUEUE_NOISE_GEN1(lcon(0x3), lcon(0x88), lcon(0x8c), lcon(0x0), lcon(0x80), parw01, parw02, lcon(0x1));
  SKW_INVOKE_MESSAGE(unsignedlong(vq_44.peek16()), lcon(0x0), lcon(0x2), unsignedlong(vq_3c.peek16()), parl04);
  RG1L = SKW_GET_TILE_RECORD_LINK(vl_2c, RG4L);
  SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_08)));
  RG1L = SKW_32cb_0287(unsignedlong(ddat.v1e02f0[RG3L / 0xc].b_0a), RG51l, RG71l);
  SKW_121e_0003(signedlong(xw_4c.peek16()));
  RG1L = SKW_GET_TILE_VALUE(signedlong(vo_38.peek16()), signedlong(vo_54.peek16()));
  RG1L = SKW_121e_03ae(RG73L, RG57L, RG2L, RG3L, lcon(0x0), parl19, lcon(0x1));
  RG1L = SKW_PT_IN_EXPANDED_RECT(RG61W + lcon(0x2f8), RG73W, RG57W) ? 1 : 0;
  RG1L = SKW_121e_0222(signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), vl_14);
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vo_38.peek16()), unsignedlong(vo_54.peek16()));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(unsignedlong(RG1W));
  RG1L = SKW_121e_03ae(RG1L, RG56L, RG74l, vl_30, lcon(0x3), parl25, lcon(0x2));
  RG1L = SKW_PT_IN_EXPANDED_RECT(CUTX16(SKW_0cee_2e09(RG1L) + RG61L), CUTX16(vl_34), RG56W) ? 1 : 0;
  RG1L = SKW_121e_0222(RG74l, vl_30, RG2L);
  RG1L = SKW_121e_03ae(RG55L, RG1L, RG75l, vl_28, lcon(0x3), parl22, lcon(0x1));
  RG1L = SKW_PT_IN_EXPANDED_RECT(RG61W + lcon(0x2f8), RG55W, CUTX16(vl_1c)) ? 1 : 0;
  RG1L = SKW_121e_0222(RG75l, vl_28, vl_20);
  SKW_121e_0351(signedlong(vol_04.peek16()), signedlong(vol_00.peek16()));
  RG1L = SKW_PT_IN_EXPANDED_RECT(lcon(0x2fd), RG3W, RG54w) ? 1 : 0;
  RG1L = SKW_PT_IN_EXPANDED_RECT(lcon(0x2fe), RG3W, RG54w) ? 1 : 0;
  SKW_PLAYER_TESTING_WALL(vl_24, signedlong(vo_38.peek16()), signedlong(vo_54.peek16()));
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vo_38.peek16()), RG4L);
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(RG53l);
  RG1L = SKW_32cb_01b6(signedlong(vol_04.peek16()), RG76L, RG62L, signedlong(vol_00.peek16()), &vw_40);
  RG1L = SKW_GET_CREATURE_WEIGHT(RG53l) & lcon(0xffff);
  SKW_ROTATE_CREATURE(RG53l, lcon(0x0), signedlong(vw_40));
  SKW_29ee_000f();
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG53l);
  SKW_QUEUE_NOISE_GEN2(lcon(0xf), lcon(0x84), lcon(0xfe), RG4Blo, parw13, parw14, lcon(0x0), lcon(0x8c), lcon(0x80));
void R_13262(i32 eaxl)
  SKW_CLICK_VWPT(RG1L, RG4L);
void SKW_HIGHLIGHT_ARROW_PANEL(i32 eaxl, i32 ebxl, i32 edxl)
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_0b36_0c52(&c22_00, lcon(0x1), RG5L);
  SKW_FILL_ENTIRE_PICT(SKW_3e74_5817(c22_00.c16_00.w_00), ddat.paletteptr1[0]);
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x1), RG2L, &c22_00, lcon(0x3), parw00, lcon(0xffff));
  SKW_0b36_0cbe(&c22_00, lcon(0x1));
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_WAIT_SCREEN_REFRESH();
void SKW_12b4_0092(void)
  SKW_HIGHLIGHT_ARROW_PANEL(unsignedlong(ddat.v1e0548), lcon(0x0), signedlong(ddat.v1e053e));
void SKW_12b4_00af(i32 eaxl)
  SKW_MOVE_RECORD_TO(lcon(0xffff), signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), lcon(0xffffffff), lcon(0x0));
  RG1L = SKW_LOCATE_OTHER_LEVEL(signedlong(ddat.v1e0266), RG2P16, RG3P16, RG4L, NULL);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = signedlong(CUTX16(SKW_0cee_06dc(signedlong(ddat.v1e0270), RG4L)));
  SKW_ROTATE_SQUAD(RG1L);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
void SKW_PERFORM_TURN_SQUAD(i32 eaxl)
  SKW_RESET_SQUAD_DIR();
  RG1L = SKW_GET_TILE_VALUE(signedlong(ddat.v1e0270), signedlong(ddat.v1e0272));
  SKW_12b4_00af(RG1L);
  RG1L = SKW_GET_TELEPORTER_DETAIL(&c12_00, RG2L, RG4L);
  R_3BF83(unsignedlong(c12_00.b_02), unsignedlong(c12_00.b_03), unsignedlong(c12_00.b_04), unsignedlong(c12_00.b_01));
  R_3CE7D(RG1L, RG4L, lcon(0xffff), lcon(0x1), parl00, parl01);
  SKW_ROTATE_SQUAD(RG1L);
  R_3CE7D(unsignedlong(ddat.v1e0270), unsignedlong(ddat.v1e0272), lcon(0xffff), lcon(0x1), parl02, parl03);
void SKW_12b4_023f(i32 eaxl, i32 edxl, i16* ebxpw, i16* ecxpw, i32 argl0, i32 argl1)
  RG1L = SKW_2c1d_028c(RG7l, RG4L, RG2L);
  RG1L = SKW_2c1d_028c(RG7l, RG4L, RG2L);
  RG1L = SKW_WOUND_PLAYER(RG52l, lcon(0x18), lcon(0x2), lcon(0x1));
  SKW_QUEUE_NOISE_GEN2(lcon(0x16), RG2Blo, RG3Blo, RG4Blo, parw00, parw01, lcon(0x1), lcon(0x64), lcon(0xc8));
i32 SKW_PERFORM_MOVE(i32 eaxl)
  RG1L = SKW_GET_TILE_VALUE(signedlong(ddat.v1e0280), RG4L);
  RG1L = SKW_CALC_PLAYER_WALK_DELAY(unsignedlong(RG2W));
  RG1L = SKW_MAX16(signedlong(RG5W), RG4L);
  SKW_CALC_VECTOR_W_DIR(RG1L, lcon(0x0), &ddat.v1e026a, lcon(0xffffffff), &ddat.v1e0268);
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(ddat.v1e053c), RG4L);
  SKW_12b4_0d75(signedlong(ddat.v1e053c), signedlong(ddat.v1e053a), signedlong(ddat.v1e0542), lcon(0xfe));
  RG1L = SKW_PERFORM_MOVE(unsignedlong(vql_00.peek16()));
  SKW_PERFORM_TURN_SQUAD(unsignedlong(ddat.v1e0538));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_30.peek16()));
  SKW_12b4_0d75(signedlong(ddat.v1e053c), signedlong(ddat.v1e053a), signedlong(ddat.v1e0542), lcon(0xfe));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e027c));
  SKW_CALC_VECTOR_W_DIR(signedlong(xw_34.peek16()), signedlong(table1d3ffc[RG1L]), &vw_14, signedlong(table1d3ff8[RG1L]), &vw_20);
  RG1L = SKW_GET_TILE_VALUE(signedlong(vw_14), RG4L);
  RG1L = SKW_GET_PLAYER_WEIGHT(unsignedlong(RG2W));
  RG1L = SKW_MAX_LOAD(RG3P);
  RG1L = SKW_ADJUST_STAMINA(signedlong(RG2W), RG4L);
  SKW_RESET_SQUAD_DIR();
  RG1L = SKW_12b4_0881(RG62L, RG4L, RG2L, RG3L, parw02, &vw_2c) - 1;
  SKW_MOVE_RECORD_TO(lcon(0xffff), signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), lcon(0xffffffff), lcon(0x0));
  SKW_12b4_023f(signedlong(vw_14), signedlong(vw_20), &vw_1c, &vw_10, parl11, parl12);
  RG5L = SKW_RAND() & lcon(0xf);
  RG1L = SKW_GET_PLAYER_ABILITY(RG3P, RG2L, RG4L) + RG5L;
  RG1L = SKW_STAMINA_ADJUSTED_ATTR(RG3P, RG4L);
  RG63L = SKW_RAND() & lcon(0xf);
  RG1L = SKW_GET_PLAYER_ABILITY(RG3P, lcon(0x0), lcon(0x1)) + RG63L;
  RG1L = SKW_STAMINA_ADJUSTED_ATTR(RG3P, RG4L);
  SKW_ATTACK_DOOR(unsignedlong(vw_14), unsignedlong(vw_20), RG2L, lcon(0x0), lcon(0x0));
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(vw_2c));
  RG1L = SKW_GET_CREATURE_WEIGHT(unsignedlong(vw_2c)) & lcon(0xffff);
  SKW_12b4_023f(signedlong(vw_14), signedlong(vw_20), &vw_1c, &vw_10, parl05, parl06);
  SKW_12b4_00af(RG1L);
  RG1L = SKW_IS_CREATURE_MOVABLE_THERE(signedlong(vw_14), RG64L, &vw_2c, signedlong(vw_20));
  RG1L = SKW_12b4_099e(unsignedlong(vw_2c));
  SKW_12b4_0d75(signedlong(vw_14), signedlong(vw_20), RG64L, lcon(0xfe));
  SKW_ATTACK_CREATURE(lcon(0xffff), signedlong(vw_14), signedlong(vw_20), lcon(0x4005), lcon(0x5), parl08);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_30.peek16()));
  SKW_CALC_VECTOR_W_DIR(signedlong(xw_34.peek16()), lcon(0x0), &vw_14, lcon(0xffffffff), &vw_20);
  RG1L = SKW_GET_TELEPORTER_DETAIL(&c12_04, RG2L, RG4L);
  R_3BF83(RG1L, RG4L, RG2L, RG3L);
  SKW_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw10);
  RG1L = SKW_MAX16(signedlong(CUTX16(signedlong(RG5W) / 2)), RG4L);
  SKW_1031_098e();
i32 SKW_12b4_0881(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i16* argpw1)
  RG1L = SKW_IS_TILE_BLOCKED(unsignedlong(RG1Blo));
  RG1L = SKW_GET_CREATURE_AT(RG1L, RG4L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG1W));
  RG1L = SKW_12b4_0953(SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG2P))), RG4L);
  RG1L = SKW_1c9a_03cf(&vw_00, RG2L, RG4P16);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG1W));
i32 SKW_12b4_0953(xxx* xeaxp, i32 edxl)
  RG1L = SKW_QUERY_CREATURE_5x5_POS(RG1P, RG4L);
i32 SKW_12b4_099e(i32 eaxl)
  RG1L = SKW_GET_CREATURE_WEIGHT(unsignedlong(RG1W));
  RG1L = SKW_GET_PLAYER_ABILITY(RG52p, lcon(0x0), lcon(0x1));
  RG1L = SKW_STAMINA_ADJUSTED_ATTR(RG52p, RG4L);
  RG1L = SKW_RAND16(RG4L / 2 + 1 & lcon(0xffff));
  RG1L = signedlong(CUTX16(SKW_MAX16(lcon(0x5), RG4L)));
  SKW_ADJUST_STAMINA(RG1L, RG4L);
i32 SKW_PUSH_PULL_RIGID_BODY(i32 eaxl)
  RG1L = SKW_IS_CREATURE_MOVABLE_THERE(RG1L, RG2L, RG3P16, RG4L);
  RG1L = SKW_12b4_099e(unsignedlong(vw_04));
  return SKW_PERFORM_MOVE(unsignedlong(vo_0c.peek16()));
  RG1L = SKW_RAND02();
  SKW_QUEUE_NOISE_GEN2(lcon(0x16), lcon(0x82), RG3Blo, RG4Blo, parw00, parw01, lcon(0x1), lcon(0x69), lcon(0xc8));
i32 SKW_IS_CREATURE_MOVABLE_THERE(i32 eaxl, i32 ebxl, i16* ecxpw, i32 edxl)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG6w), RG4L);
  RG1L = SKW_GET_CREATURE_WEIGHT(RG71L) & lcon(0xffff);
  RG1L = SKW_GET_TELEPORTER_DETAIL(&c12_00, RG2L, RG4L);
  RG1L = SKW_IS_CREATURE_ALLOWED_ON_LEVEL(RG71L, unsignedlong(c12_00.b_04));
  SKW_CHANGE_CURRENT_MAP_TO(unsignedlong(c12_00.b_04));
  RG1L = SKW_IS_TILE_BLOCKED(SKW_GET_TILE_VALUE(unsignedlong(c12_00.b_02), RG4L) & lcon(0xff));
  RG1L = SKW_GET_CREATURE_AT(RG2L & lcon(0xffff), RG4L);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_0c.peek16()));
  RG1L = SKW_GET_TILE_VALUE(signedlong(RG6w), RG4L);
  RG1L = SKW_IS_TILE_BLOCKED(RG72L);
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG6w), RG4L);
  RG1L = SKW_GET_TELEPORTER_DETAIL(&c12_00, RG2L, RG4L);
  RG1L = SKW_IS_CREATURE_ALLOWED_ON_LEVEL(unsignedlong(vql_08.peek16()), unsignedlong(c12_00.b_04));
void SKW_12b4_0d75(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
  RG1L = SKW_IS_CREATURE_MOVABLE_THERE(RG1L, RG2L, RG3P16, RG4L);
  RG1L = SKW_GET_CREATURE_WEIGHT(unsignedlong(vw_1c));
  RG1L = SKW_RAND16(RG4L + lcon(0x1) & lcon(0xffff));
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(vw_1c));
  SKW_1c9a_0247(unsignedlong(vw_1c));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(vw_1c));
  SKW_1c9a_0db0(unsignedlong(vw_1c));
  SKW_1c9a_0cf7(unsignedlong(vql_0c.peek16()), unsignedlong(vql_08.peek16()));
  SKW_MOVE_RECORD_TO(unsignedlong(vw_1c), signedlong(vql_0c.peek16()), RG2L, RG3L, parw02);
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG61w), RG4L);
  SKW_ATTACK_CREATURE(lcon(0xffff), RG4L, RG2L, RG3L, lcon(0xa), parl04);
  SKW_12b4_023f(RG1L, RG4L, RG2P16, RG3P16, parl00, parl01);
  SKW_PERFORM_MOVE(RG1L);
  SKW_QUEUE_NOISE_GEN2(lcon(0x16), lcon(0x8a), RG3Blo, RG4Blo, parw05, parw06, lcon(0x1), lcon(0x64), lcon(0xc8));
  RG1L = SKW_GET_PLAYER_WEIGHT(unsignedlong(RG62w));
void SKW_LOAD_NEWMAP(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_3a15_38b6(lcon(0x0));
  SKW_LOAD_LOCALLEVEL_GRAPHICS_TABLE(RG1L, RG2L, RG4L);
  SKW_LOAD_LOCALLEVEL_DYN();
  SKW_3a15_38b6(lcon(0x1));
  RG1L = SKW_FILL_CAII_CUR_MAP();
  RG1L = SKW_CHECK_RECOMPUTE_LIGHT();
  SKW_1031_098e();
  SKW_FIRE_SHOW_MOUSE_CURSOR();
void SKW_GAME_LOOP(void)
  SKW_FILL_ORPHAN_CAII();
  SKW_LOAD_NEWMAP(signedlong(ddat.v1e0270), signedlong(ddat.v1d27f8), lcon(0x1), signedlong(ddat.v1e0272));
  SKW_MOVE_RECORD_TO(lcon(0xffff), lcon(0xffffffff), RG2L, RG3L, parw00);
  SKW_PROCEED_TIMERS();
  SKW_UPDATE_WEATHER(lcon(0x0));
  SKW_38c8_0060();
  RG1L = SKW_CHECK_RECOMPUTE_LIGHT();
  SKW_DISPLAY_VIEWPORT(RG1L, RG2L, RG4L);
  SKW_CHANGE_VIEWPORT_TO_INVENTORY(lcon(0x1));
  SKW_PERFORM_MOVE(unsignedlong(ddat.v1e0256));
  SKW_443c_0434();
  SKW_IBMIO_USER_INPUT_CHECK();
  SKW_12b4_0092();
  SKW_482b_05bf(lcon(0x0));
  SKW_PROCESS_PLAYERS_DAMAGE();
  SKW_UPDATE_CHAMPIONS_STATS();
  SKW_GLOBAL_UPDATE_UNKNOW1();
  SKW_2e62_0cfa(RG1L);
  R_10777();
  SKW_BURN_PLAYER_LIGHTING_ITEMS();
  SKW_3929_086f();
  SKW_12b4_0092();
  RG1L = SKW_0cee_04e5(RG1L, lcon(0x1), RG2L, RG3L, parw01);
  SKW_FIRE_MOUSE_RELEASE_CAPTURE();
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_EVENT_LOOP();
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1d27f8));
  SKW_2759_12e6();
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  R_BA7(ddat.v1e0266);
void SKW_MAIN(void)
  SKW_INIT();
  SKW_GAME_LOOP();
  SKW_END_GAME(unsignedlong(CUTX16(ddat.v1e0250)));
xxx* SKW_13e4_0004(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
  SKW_1c9a_0648(signedlong(vq_00.peek16()));
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x350));
  SKW_COPY_MEMORY(DOWNCAST(c_350, &ddat.s350), lcon(0x350), RG1P);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG6w));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG3P, lcon(0x4))));
  RG1P = DOWNCAST(i16, SKW_1c9a_02c3(RG3P, RG1P));
  DM2_ZERO_MEMORY(RG2P + lcon(0x18), lcon(0xa));
void SKW_13e4_01a3(void)
  RG1L = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(byte_at(RG3P, lcon(0x4))), lcon(0x1));
  RG1L = SKW_1c9a_08bd(RG3P);
  DM2_ZERO_MEMORY(ddat.s350.v1e07ee, lcon(0xa8));
  RG1L = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(byte_at(RG3P, lcon(0x4))), lcon(0x7));
  RG1L = SKW_RAND16(unsignedlong(RG2W)) & lcon(0xffff);
  RG1L = SKW_3e74_5673(RG1L, lcon(0x0), &vw_00);
  DM2_ZERO_MEMORY(DOWNCAST(c_4b3wp, &ddat.s350.v1e07d8), lcon(0xe));
  SKW_COPY_MEMORY(DOWNCAST(t_gfxdata, SKW_3e74_5817(vw_00)), lcon(0xe), DOWNCAST(c_4b3wp, &ddat.s350.v1e07d8)); // TODO: ATTENTION destination type
void SKW_13e4_0329(xxx* xeaxp)
  SKW_COPY_MEMORY(RG1P, lcon(0x350), DOWNCAST(c_350, &ddat.s350));
  DM2_DEALLOC_LOBIGPOOL(lcon(0x350));
void SKW_13e4_0360(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG5w), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  SKW_1c9a_0db0(RG4L);
  SKW_1c9a_0cf7(RG1L, RG4L);
i32 SKW_ATTACK_CREATURE(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vql_08.peek16()), RG4L);
  RG1L = SKW_RAND01();
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG3L);
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG61p, lcon(0x4))));
  SKW_ALLOC_CAII_TO_CREATURE(RG3L, unsignedlong(vql_08.peek16()), RG2L);
  RG1L = SKW_RAND02();
  RG1L = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(byte_at(RG5p, lcon(0x4))), lcon(0x1)) & lcon(0xffff);
  RG1L = SKW_RAND01();
  RG1L = SKW_CALC_VECTOR_DIR(vql_08.peek16(), ddat.v1e0270, ddat.v1e0272, vql_04.peek16());
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND01();
  SKW_13e4_0360(unsignedlong(xwc_0c.peek16()), signedlong(vql_08.peek16()), signedlong(vql_04.peek16()), RG3L, lcon(0x0));
  RG1L = SKW_RAND16(lcon(0x64)) & lcon(0xffff);
  RG1L = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(byte_at(RG5p, lcon(0x4))), lcon(0x1)) & lcon(0xffff);
  SKW_1c9a_0db0(unsignedlong(xwc_0c.peek16()));
  return SKW_1c9a_0cf7(unsignedlong(vql_08.peek16()), unsignedlong(vql_04.peek16()));
void SKW_13e4_071b(void)
  RG1L = R_4EA8(unsignedlong(byte_at(ddat.s350.v1e054e, lcon(0x4))), RG4L);
  SKW_1c9a_0db0(unsignedlong(ddat.s350.v1e054c));
  RG1L = SKW_QUEUE_TIMER(&ddat.s350.v1e0562);
void SKW_13e4_0806(void)
  RG1L = R_4EA8(unsignedlong(byte_at(ddat.s350.v1e054e, lcon(0x4))), RG4L);
  SKW_1c9a_0db0(unsignedlong(ddat.s350.v1e054c));
  RG1L = SKW_QUEUE_TIMER(&ddat.s350.v1e0562);
void SKW_ANIMATE_CREATURE(i32 eaxl, i32 edxl, i32 ebxl)
  RG1L = SKW_GET_CREATURE_AT(RG5l, RG4L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(RG3L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG3L);
  SKW_ALLOC_CAII_TO_CREATURE(RG3L, RG5l, RG2L);
  RG1P = SKW_13e4_0004(unsignedlong(xwc_04.peek16()), RG4L, RG2L, lcon(0x21));
  SKW_13e4_0806();
  SKW_13e4_071b();
  SKW_13e4_0329(RG4P);
void SKW_13e4_0982(void)
  RG1L = R_4FCC(unsignedlong(byte_at(ddat.s350.v1e054e, lcon(0x4))), RG4L, RG2P16, &ddat.s350.v1e055a);
  SKW_14cd_09e2();
  RG1L = SKW_14cd_062e();
  RG1L = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(RG1L, RG4L) & lcon(0xffff);
  SKW_CREATE_CLOUD(lcon(0xffa8), RG4L, RG2L, RG3L, lcon(0xff));
  RG1L = SKW_GET_CREATURE_ANIMATION_FRAME(unsignedlong(byte_at(ddat.s350.v1e054e, lcon(0x4))), RG4L, UPCAST(i16, RG61p), RG3P16, xparpp01, parl02);
  SKW_13e4_01a3();
  RG1L = SKW_PROCEED_CCM();
  RG1L = R_50CB(unsignedlong(byte_at(ddat.s350.v1e054e, lcon(0x4))), RG4L, RG2P, &ddat.s350.v1e055a);
  RG1L = SKW_CREATURE_SOMETHING_1c9a_0a48() | RG4L;
  RG1L = SKW_3e74_5673(RG1L, lcon(0x1), &vw_00);
  RG52p = DOWNCAST(t_gfxdata, SKW_3e74_5817(vw_00));
  RG1P = DOWNCAST(s_xmalloc, SKW_ALLOC_CPXHEAP_MEM(vw_00, lcon(0xe)));
  SKW_3e74_585a(vw_00, true);
  SKW_COPY_MEMORY(DOWNCAST(c_4b3wp, &ddat.s350.v1e07d8), lcon(0xe), RG52p);
  SKW_1c9a_0db0(unsignedlong(ddat.s350.v1e054c));
  RG1L = SKW_QUEUE_TIMER(&ddat.s350.v1e0562);
void SKW_THINK_CREATURE(i32 eaxl, i32 ebxl, i32 edxl)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG3W), RG4L);
  RG1P = SKW_13e4_0004(unsignedlong(RG1W), RG4L, RG2L, RG61l);
  RG1L = SKW_WOUND_CREATURE(unsignedlong(RG62W));
  SKW_13e4_0806();
  SKW_13e4_071b();
  SKW_1c9a_0fcb(RG1L);
  SKW_13e4_0982();
  SKW_13e4_0329(RG7p);
void SKW_SELECT_CREATURE_37FC(void)
  RG1L = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(byte_at(ddat.s350.v1e054e, lcon(0x4))), lcon(0x1));
  RG1L = SKW_14cd_0067(DOWNCAST(s_wpm, table1d6190[signedlong(ddat.s350.v1e0584)]));
i32 SKW_14cd_0067(xxx* xeaxp)
  RG3L = SKW_RAND();
  RG1L = SKW_RAND16(unsignedlong(RG62W));
  RG64l = signedlong(CUTX16(SKW_MAX16(lcon(0x1), RG4L)));
  RG1L = SKW_GET_GLOB_VAR(unsignedlong(xwc_04.peek16()));
void SKW_14cd_0276(xxx* xeaxp)
  RG1L = SKW_MAX16(lcon(0x0), RG4L);
  RG1L = SKW_3e74_5673(RG1L, lcon(0x1), &vw_00);
  RG5p = DOWNCAST(t_gfxdata, SKW_3e74_5817(vw_00));
  RG1P = DOWNCAST(s_xmalloc, SKW_ALLOC_CPXHEAP_MEM(vw_00, RG3L));
  RG1P = DOWNCAST(s_xmalloc, SKW_3e74_5788(vw_00, RG3L));
  SKW_3e74_585a(vw_00, true);
  SKW_COPY_MEMORY(DOWNCAST(c_len10x, ddat.s350.v1e060e), RG3L, RG5p);
i32 SKW_14cd_0389(void)
  SKW_14cd_0f0a(RG1L, RG4L, RG2L, RG3P);
  RG1L = SKW_1c9a_38a8();
i32 SKW_14cd_0457(void)
  RG1L = SKW_MIN16(signedlong(CUTX16(signedlong(RG3W) / 2)), RG4L);
  SKW_COPY_MEMORY(RG1P, RG2L, RG4P);
void SKW_14cd_0550(xxx* xeaxp, i32 edxl, i32 ebxl, i32 ecxl)
  RG1L = SKW_RAND16(-RG1L & lcon(0xffff));
  RG1L = SKW_RAND16(unsignedlong(RG1W));
  SKW_14cd_0f0a(RG1L, RG4L, RG2L, RG3P);
i32 SKW_14cd_0684(void)
  RG1L = SKW_14cd_0389();
  RG1L = SKW_RAND02();
  RG1L = SKW_14cd_062e();
  SKW_14cd_0550(RG1P, RG4L, RG2L, RG6l);
  RG1L = SKW_14cd_0457();
  RG1L = SKW_1c9a_1bd8(RG1L, RG4L, RG2L, parw00, XUPCAST(s_len8, xparp01), c17p_2);
  SKW_14cd_0276(RG1P);
void SKW_14cd_09e2(void)
  SKW_SELECT_CREATURE_37FC();
  SKW_13e4_01a3();
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x100));
  RG1L = SKW_RAND02();
  RG1L = SKW_CREATURE_GO_THERE(RG1L, RG4L, RG2L, RG3L, parw00, parw01);
  RG1L = SKW_19f0_13aa(RG1L, RG4L);
  RG1L = SKW_19f0_0891(lcon(0x0), RG4L, RG2L, RG3L, parw02, parw03);
  RG1L = SKW_RAND16(unsignedlong(CUTX16(unsignedlong(CUTX16(unsignedlong(word_at(ddat.s350.v1e0552, lcon(0x18))) >> bcon(0x8) & lcon(0xf))) / 4 + 1))) & lcon(0xffff);
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND02();
  RG1L = SKW_19f0_13aa(unsignedlong(ddat.s350.v1e0562.b_06), RG4L);
  DM2_DEALLOC_LOBIGPOOL(lcon(0x100));
  RG1L = SKW_1c9a_381c();
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND01();
  RG1L = SKW_CREATURE_GO_THERE(unsignedlong(RG1Blo), RG4L, RG2L, RG3L, parw04, parw05);
  DM2_DEALLOC_LOBIGPOOL(lcon(0x100));
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND();
  SKW_19f0_0559(RG1L);
  RG1L = (SKW_GET_TILE_VALUE(signedlong(RG1W), RG4L) & lcon(0xff)) >> bcon(0x5);
  RG1L = SKW_GET_TELEPORTER_DETAIL(&c12_00, RG2L, RG4L);
  RG1L = SKW_IS_CREATURE_ALLOWED_ON_LEVEL(unsignedlong(ddat.s350.v1e054c), unsignedlong(c12_00.b_04));
  RG1L = SKW_14cd_0684();
  RG1L = SKW_14cd_08f5(lcon(0xfffffffd));
  DM2_DEALLOC_LOBIGPOOL(lcon(0x100));
  RG1L = SKW_PROCEED_XACT(signedlong(CUTLX8(SKW_DECIDE_NEXT_XACT(RG1L))));
  RG1L = SKW_14cd_08f5(signedlong(RG1Blo));
  DM2_DEALLOC_LOBIGPOOL(lcon(0x100));
void SKW_14cd_0f0a(i32 eaxl, i32 edxl, i32 ebxl, xxx* xecxp)
  SKW_14cd_18cc(RG1L, RG4L, RG2P);
  SKW_14cd_19a4(RG1L, RG4L, RG2P);
  SKW_14cd_1a3c(RG1L, RG4L, RG2P);
  SKW_14cd_1a5a(RG1L, RG4L, RG2P);
  SKW_14cd_1b74(RG1L, RG4L, RG2P);
  SKW_14cd_1b90(RG1L, RG4L, RG2P);
  SKW_14cd_1c27(RG1L, RG4L, RG2P);
  SKW_14cd_1c45(RG1L, RG4L, RG2P);
  SKW_14cd_1c63(RG4L, RG2P);
  SKW_14cd_1c8d(RG1L, RG4L, RG2P);
  SKW_14cd_1cec(RG4L, RG2P);
  SKW_14cd_1d42(RG4L, RG2P);
  SKW_14cd_1e36(RG1L, RG4L, RG2P);
  SKW_14cd_1e52(RG1L, RG4L, RG2P);
  SKW_14cd_1e6e(RG1L, RG4L, RG2P);
  SKW_14cd_1f8b(RG1L, RG4L, RG2P);
  SKW_14cd_1fa7(RG4L, RG2P);
void SKW_14cd_0f3c(i32 eaxl, xxx* xedxp, s_hexe* ebxp_s18, i32 ecxl, i8 argb0, i32 argl1, i8 argb2, i8 argb3)
  RG1L = SKW_BETWEEN_VALUE16(lcon(0xffffffff), lcon(0x7f), RG4L);
i32 SKW_14cd_102e(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
  RG1L = SKW_IS_CONTAINER_CHEST(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG1L);
  RG1L = SKW_14cd_102e(RG1L, RG4L, RG2L, RG3L, parl00);
  RG1L = SKW_1c9a_0006(unsignedlong(RG5W), RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG5W));
xxx* SKW_14cd_10d2(xxx* xeaxp, i32 edxl)
  DM2_ZERO_MEMORY(ddat.s350.v1e058e, lcon(0x80));
  RG1L = SKW_14cd_102e(signedlong(RG4W), vl_08, lcon(0xff), lcon(0x0), lcon(0x1));
  RG1L = SKW_MIN16(RG3L, RG4L);
  RG1L = SKW_MIN16(RG3L, RG4L);
  RG1L = SKW_MIN16(signedlong(vq_10.peek16()), RG4L);
  RG1L = SKW_MIN16(signedlong(vq_14.peek16()), RG4L);
i32 SKW_14cd_1316(i32 eaxl, i32 edxl, i32 ebxl)
  RG1L = SKW_CALC_VECTOR_DIR(ddat.v1e08d8, RG2W, RG3W, RG4W);
  RG1W = SKW_CALC_SQUARE_DISTANCE(ddat.v1e08d8, RG2W, RG3W, RG4W);
  pf_00 = R_1BA1B;
  RG1L = SKW_19f0_0207(signedlong(ddat.v1e08d8), RG4L, RG2L, RG3L, pf_00);
  RG1L = SKW_GET_PLAYER_AT_POSITION(unsignedlong(RG61w));
  RG1L = SKW_1c9a_0006(RG1L, RG4L);
  RG1L = SKW_1c9a_0006(RG1L, RG4L);
  RG1L = SKW_2c1d_09d9();
  RG1L = (SKW_GET_TILE_VALUE(signedlong(RG3W), RG4L) & lcon(0xff)) >> bcon(0x5) & lcon(0xff);
  RG1L = SKW_19f0_0d10(lcon(0x1), unsignedlong(ddat.s350.v1e0562.b_06), unsignedlong(ddat.s350.v1e0562.b_07), lcon(0xffffffff), parw01, parw02);
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG2W), RG4L);
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W)), lcon(0x4))));
  RG1L = SKW_GET_TILE_VALUE(RG51l, RG2L);
  RG1L = SKW_GET_TILE_RECORD_LINK(RG51l, RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
  RG1L = SKW_MIN16(lcon(0x4), RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  SKW_19f0_045a(RG1L, RG4L);
  RG1L = SKW_1c9a_1b16(signedlong(vq_0c.peek16()), RG4L);
  RG1L = SKW_1c9a_0732(signedlong(vq_0c.peek16()), RG4L, lcon(0xff));
  RG1L = SKW_ABS16(RG1W);
  RG1L = signedlong(CUTX16(SKW_ABS16(unsignedword(ddat.s350.v1e0562.b_07) - ddat.v1e08d4))) + RG4L;
  RG1L = signedlong(CUTX16(SKW_CALC_VECTOR_DIR(unsignedword(ddat.s350.v1e0562.b_06), RG2W, RG3W, RG4W)));
  RG1L = SKW_GET_TILE_VALUE(signedlong(RG4W), RG2L);
void SKW_14cd_18cc(i32 eaxl, i32 edxl, xxx* xebxp)
  SKW_14cd_0f3c(lcon(0x0), RG4P, CHGCAST(s_hexe, getadr_r1d41d6()), lcon(0x0), lcon(0x0), lcon(0xffff), parb02, parb03);
void SKW_14cd_18f2(i32 eaxl, i32 edxl, xxx* xebxp, i32 ecxl, i16 argw0)
  RG1L = SKW_14cd_1316(unsignedlong(byte_at(RG7p, lcon(0x1))), RG4L, RG2L);
  SKW_14cd_0f3c(RG1L, RG4P, UPCAST(s_hexe, RG2P), RG3L, parb00, parl01, parb02, parb03);
void SKW_14cd_19a4(i32 eaxl, i32 edxl, xxx* xebxp)
  SKW_14cd_18f2(RG1L, RG4L, RG2P, lcon(0x0), lcon(0xffff));
void SKW_14cd_19c2(i32 eaxl, xxx* xebxp, i32 edxl, i32 ecxl, i8 argb0)
  RG1P = SKW_14cd_10d2(RG2P, RG4L);
  SKW_14cd_18f2(signedlong(RG3Blo), RG4L, RG2P, RG1L, lcon(0xffff));
void SKW_14cd_1a3c(i32 eaxl, i32 edxl, xxx* xebxp)
  SKW_14cd_19c2(RG1L, RG2P, RG4L, lcon(0x2), lcon(0x1));
void SKW_14cd_1a5a(i32 eaxl, i32 edxl, xxx* xebxp)
  SKW_14cd_19c2(RG1L, RG2P, RG4L, lcon(0x4), lcon(0x3));
void SKW_14cd_1a78(i32 eaxl, i32 edxl, xxx* xebxp, i32 ecxl)
  RG1P = SKW_14cd_10d2(RG2P, RG4L);
  RG1L = SKW_14cd_1316(unsignedlong(byte_at(RG7p, lcon(0x1))), RG4L, RG2L);
  RG1L = -SKW_MIN16(signedlong(byte_at(RG7p, lcon(0x8))), RG4L);
  SKW_14cd_0f3c(RG1L, RG4P, UPCAST(s_hexe, RG2P), RG3L, parb00, parl01, parb02, parb03);
void SKW_14cd_1b74(i32 eaxl, i32 edxl, xxx* xebxp)
  SKW_14cd_1a78(RG1L, RG4L, RG2P, lcon(0x1));
void SKW_14cd_1b90(i32 eaxl, i32 edxl, xxx* xebxp)
  SKW_14cd_1a78(RG1L, RG4L, RG2P, lcon(0x3));
void SKW_14cd_1bac(i32 eaxl, i32 edxl, xxx* xebxp, i32 ecxl, i8 argb0)
  RG1P = SKW_14cd_10d2(RG2P, RG4L);
  SKW_14cd_18f2(RG1L, RG4L, RG2P, lcon(0x0), lcon(0xffff));
void SKW_14cd_1c27(i32 eaxl, i32 edxl, xxx* xebxp)
  SKW_14cd_1bac(RG1L, RG4L, RG2P, lcon(0x2), lcon(0x1));
void SKW_14cd_1c45(i32 eaxl, i32 edxl, xxx* xebxp)
  SKW_14cd_1bac(RG1L, RG4L, RG2P, lcon(0x4), lcon(0x3));
void SKW_14cd_1c63(i32 edxl, xxx* xebxp)
  SKW_14cd_18f2(lcon(0x5), RG4L, RG2P, lcon(0x0), parw00);
void SKW_14cd_1c8d(i32 eaxl, i32 edxl, xxx* xebxp)
  SKW_14cd_18f2(lcon(0x6), RG4L, RG2P, lcon(0x0), lcon(0xffff));
void SKW_14cd_1cec(i32 edxl, xxx* xebxp)
  RG1P = SKW_GET_MISSILE_REF_OF_MINION(RG1L, RG4L);
  RG1L = unsignedlong(word_at(SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG1P, lcon(0x2)))), lcon(0x6)));
  SKW_14cd_18f2(lcon(0x7), RG4L, RG2P, lcon(0x0), parw00);
void SKW_14cd_1d42(i32 edxl, xxx* xebxp)
  SKW_14cd_18f2(lcon(0x12), RG4L, RG2P, lcon(0x0), parw00);
void SKW_14cd_1d6c(i32 eaxl, i32 edxl, xxx* xebxp, i32 ecxl)
  RG1L = SKW_1c9a_0732(RG1L, RG4L, RG2L);
  RG1L = SKW_14cd_1316(unsignedlong(byte_at(RG7p, lcon(0x1))), RG4L, RG2L);
  SKW_14cd_0f3c(RG1L, RG4P, UPCAST(s_hexe, RG2P), RG3L, lcon(0x0), lcon(0xffff), parb02, parb03);
void SKW_14cd_1e36(i32 eaxl, i32 edxl, xxx* xebxp)
  SKW_14cd_1d6c(RG1L, RG4L, RG2P, lcon(0xf));
void SKW_14cd_1e52(i32 eaxl, i32 edxl, xxx* xebxp)
  SKW_14cd_1d6c(RG1L, RG4L, RG2P, lcon(0x10));
void SKW_14cd_1e6e(i32 eaxl, i32 edxl, xxx* xebxp)
  RG1L = R_3DC4C(unsignedlong(ddat.s350.v1e0571));
  RG1L = SKW_RAND();
  RG1L = SKW_RAND();
  SKW_14cd_0f3c(lcon(0x0), RG4P, CHGCAST(s_hexe, getadr_r1d41d6()), lcon(0x0), lcon(0x0), lcon(0xffff), parb02, parb03);
void SKW_14cd_1eec(i32 eaxl, i32 edxl, xxx* xebxp, i32 ecxl)
  RG1L = SKW_14cd_1316(unsignedlong(byte_at(RG7p, lcon(0x1))), signedlong(word_at(RG7p, lcon(0x2))), RG2L);
  SKW_14cd_0f3c(RG1L, RG7p, UPCAST(s_hexe, RG2P), RG3L, lcon(0x0), lcon(0xffff), parb02, parb03);
void SKW_14cd_1f8b(i32 eaxl, i32 edxl, xxx* xebxp)
  SKW_14cd_1eec(RG1L, RG4L, RG2P, lcon(0x15));
void SKW_14cd_1fa7(i32 edxl, xxx* xebxp)
  SKW_14cd_18f2(lcon(0x16), RG4L, RG2P, lcon(0x0), parw00);
i32 SKW_PROCEED_XACT(i32 eaxl)
  RG1L = SKW_PROCEED_XACT_56();
  RG1L = SKW_PROCEED_XACT_57();
  RG1L = SKW_PROCEED_XACT_62();
  RG1L = SKW_PROCEED_XACT_63();
  RG1L = SKW_PROCEED_XACT_64();
  RG1L = SKW_PROCEED_XACT_65();
  RG1L = SKW_PROCEED_XACT_66();
  RG1L = SKW_PROCEED_XACT_67();
  RG1L = SKW_PROCEED_XACT_68();
  SKW_PROCEED_XACT_69();
  RG1L = SKW_PROCEED_XACT_70();
  RG1L = SKW_PROCEED_XACT_71();
  RG1L = SKW_PROCEED_XACT_72_87_88(RG1L);
  RG1L = SKW_PROCEED_XACT_73();
  RG1L = SKW_PROCEED_XACT_74();
  RG1L = SKW_PROCEED_XACT_75();
  RG1L = SKW_PROCEED_XACT_77(RG1L);
  RG1L = SKW_PROCEED_XACT_78();
  RG1L = SKW_PROCEED_XACT_79();
  RG1L = SKW_PROCEED_XACT_80(RG1L);
  RG1L = SKW_PROCEED_XACT_81();
  RG1L = SKW_PROCEED_XACT_82();
  RG1L = SKW_PROCEED_XACT_83();
  RG1L = SKW_PROCEED_XACT_84();
  RG1L = SKW_PROCEED_XACT_85();
  RG1L = SKW_PROCEED_XACT_86();
  RG1L = SKW_PROCEED_XACT_89(RG1L);
  RG1L = SKW_PROCEED_XACT_90();
  RG1L = SKW_PROCEED_XACT_91();
  RG1L = SKW_PROCEED_XACT_59_76();
i32 SKW_PROCEED_XACT_75(void)
  RG1P = SKW_14cd_10d2(ddat.s350.v1e07d8.xp_0a, signedlong(ddat.s350.v1e07d8.b_02));
  RG1L = SKW_19f0_0891(unsignedlong(RG1Blo), RG4L, RG2L, RG3L, parw00, parw01);
i32 SKW_PROCEED_XACT_79(void)
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND02();
i32 SKW_PROCEED_XACT_56(void)
  RG1L = SKW_CREATURE_GO_THERE(lcon(0x80), unsignedlong(ddat.s350.v1e0562.b_06), unsignedlong(ddat.s350.v1e0562.b_07), lcon(0xffffffff), parw00, parw01);
i32 SKW_PROCEED_XACT_57(void)
  RG1L = SKW_RAND01();
  RG1L = SKW_CREATURE_GO_THERE(lcon(0x80), unsignedlong(ddat.s350.v1e0562.b_06), unsignedlong(ddat.s350.v1e0562.b_07), lcon(0xffffffff), parw00, parw01);
  RG1L = SKW_CREATURE_GO_THERE(lcon(0x80), unsignedlong(ddat.s350.v1e0562.b_06), unsignedlong(ddat.s350.v1e0562.b_07), lcon(0xffffffff), parw02, parw03);
  RG1L = SKW_19f0_0559(RG5L);
i32 SKW_PROCEED_XACT_59_76(void)
  RG1L = SKW_1c9a_0732(signedlong(RG3W), RG4L, lcon(0xff));
  RG1L = SKW_19f0_2165(lcon(0x80), RG4L, RG2L, RG3L, parl00, lcon(0xffffffff), parw02);
i32 SKW_PROCEED_XACT_64(void)
  RG1L = SKW_1c9a_0732(RG3L, RG4L, lcon(0xff));
  RG1L = SKW_19f0_2165(lcon(0x81), RG4L, RG2L, lcon(0xffffffff), lcon(0xffffffff), parw01, parw02);
i32 SKW_PROCEED_XACT_70(void)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(mkuw(RG5W & wcon(0x1f))), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1L = SKW_1c9a_0732(signedlong(vq_00.peek16()), RG4L, RG2L);
i32 SKW_PROCEED_XACT_63(void)
  RG1L = SKW_GET_CREATURE_AT(RG52l, RG1L);
  RG1L = SKW_1c9a_0732(signedlong(xw_00.peek16()), unsignedlong(word_at(SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W)), lcon(0x2))), RG2L);
i32 SKW_14cd_2662(i32 eaxl)
  RG1L = SKW_GET_CREATURE_AT(RG1L, RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1L = SKW_1c9a_0006(RG3L, RG4L);
  RG1L = SKW_1c9a_0006(RG3L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG2W));
i32 SKW_PROCEED_XACT_66(void)
  RG1L = SKW_14cd_2662(lcon(0x2));
  RG1L = SKW_PROCEED_XACT_63();
  RG1L = SKW_PROCEED_XACT_63();
i32 SKW_14cd_2807(i16* eaxpw, i16* edxpw)
  RG1L = SKW_1c9a_0006(unsignedlong(word_at(RG2P)), RG4L);
  RG1L = signedlong(CUTX16(SKW_ADD_ITEM_CHARGE(unsignedlong(word_at(RG2P)), lcon(0x0))));
  RG1L = SKW_48ae_05ae(signedlong(CUTX16(SKW_GET_DISTINCTIVE_ITEMTYPE(unsignedlong(word_at(RG2P))))), RG4L, RG3L, RG6L, parl00, parl01);
i32 SKW_14cd_2886(i16* eaxpw, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i32 argl1)
  SKW_OVERSEE_RECORD(RG1P16, UPCAST(i16*, DOWNCAST(i16*, pwa_0c)), SKW_14cd_2807, RG4L, vwa_00, parl01, parl02);
i32 SKW_PROCEED_XACT_67(void)
  RG1L = SKW_14cd_2662(lcon(0x2));
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(mkuw(table1d27fc[RG1L] + RG2W)), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1L = SKW_14cd_2886(RG71p16, RG4L, RG2L, RG3L, parl00, parl01);
  RG1L = SKW_14cd_2886(RG71p16, RG4L, RG2L, RG3L, parl02, parl03);
  RG1L = SKW_48ae_0767(signedlong(CUTX16(SKW_14cd_2886(RG71p16, RG4L, RG2L, RG3L, parl04, parl05))), UPCAST(i16, ADRESSOF(i8, vba_00)), ADRESSOF(i16, &vw_1c), lcon(0x12)); // unused extra argument was lcon(0x1)
  RG1L = SKW_RAND16(lcon(0x10));
  RG1L = SKW_RAND16(SKW_MAX16(lcon(0x1), RG4L) & lcon(0xffff)) & lcon(0xffff);
  RG1L = SKW_RAND02();
  RG1L = SKW_MIN16(signedlong(RG62W), RG4L);
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND();
  RG1L = SKW_RAND16(SKW_MAX16(lcon(0x1), RG4L) & lcon(0xffff)) & lcon(0xffff);
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND02() + lcon(0x9);
i32 SKW_FIND_TILE_ACTUATOR(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(RG1W), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
i32 SKW_PROCEED_XACT_62(void)
  RG1L = SKW_1c9a_0732(lcon(0x10), RG4L, lcon(0xff));
  RG1L = SKW_FIND_TILE_ACTUATOR(RG61l, RG2L, RG3L, RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1L = SKW_GET_WALL_TILE_ANYITEM_RECORD(RG61l, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(unsignedlong(RG4W));
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(RG52l);
  RG1L = SKW_GET_NEXT_RECORD_LINK(RG52l);
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(RG53l);
  vl_04 = SKW_GET_NEXT_RECORD_LINK(RG53l);
  SKW_CUT_RECORD_FROM(RG53l, RG7l, RG63l, NULL);
  SKW_APPEND_RECORD_TO(RG1L, RG2L, RG3L, RG4P);
  SKW_19f0_2165(lcon(0x80), unsignedlong(ddat.s350.v1e0562.b_06), unsignedlong(ddat.s350.v1e0562.b_07), signedlong(xw_10.peek16()), parl00, lcon(0xffffffff), lcon(0x10));
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG2W), RG4L);
  RG1P = DOWNCAST(i16, SKW_1c9a_06bd(unsignedlong(RG1W), RG4L, RG2L));
i32 SKW_PROCEED_XACT_65(void)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG3W), RG4L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG4W));
i32 SKW_PROCEED_XACT_68(void)
  RG1L = SKW_14cd_2662(unsignedlong(mkub(CUTX8(ddat.s350.v1e0572) + bcon(0x2) & bcon(0x3))));
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG1W), RG2L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1L = SKW_48ae_0767(signedlong(CUTX16(SKW_14cd_2886(UPCAST(i16, RG5p), RG4L, RG2L, RG3L, parl00, parl01))), UPCAST(i16, ADRESSOF(i8, vba_00)), ADRESSOF(i16, &vw_24), lcon(0x12)); // unused extra argument was lcon(0x1)
  RG1L = SKW_14cd_2886(UPCAST(i16, RG5p), RG4L, RG2L, RG3L, parl03, parl04);
  RG1L = SKW_14cd_2886(UPCAST(i16, RG5p), RG4L, RG2L, RG3L, parl05, parl06);
  RG1L = SKW_14cd_2886(UPCAST(i16, RG5p), RG4L, RG2L, RG3L, parl07, parl08);
  RG4L = SKW_MAX16(lcon(0x0), signedlong(RG62W));
i32 SKW_PROCEED_XACT_71(void)
  RG1L = SKW_1c9a_078b(RG3P, RG4L, RG2L);
  RG1L = SKW_1c9a_0732(RG3L, RG4L, RG2L);
  RG1L = SKW_19f0_2165(lcon(0x81), RG4L, RG2L, RG3L, parl00, lcon(0xffffffff), parw02);
i32 SKW_PROCEED_XACT_74(void)
  RG1L = SKW_RAND();
  RG1L = SKW_1c9a_381c();
  RG1L = SKW_CREATURE_GO_THERE(unsignedlong(RG4Blo), RG52l, RG2L, lcon(0xffffffff), parw00, parw01);
  RG1L = SKW_CALC_VECTOR_DIR(unsignedword(ddat.s350.v1e0562.b_06), RG2W, RG3W, unsignedword(ddat.s350.v1e0562.b_07));
  RG1L = SKW_RAND01();
  RG1L = SKW_19f0_0559(signedlong(RG4W));
i32 SKW_PROCEED_XACT_77(i32 eaxl)
  SKW_14cd_0f3c(lcon(0x14), RG4P, UPCAST(s_hexe, RG2P), RG3L, lcon(0x0), lcon(0xffff), lcon(0xff), lcon(0x0));
  RG1L = SKW_1c9a_1bd8(RG1L, RG4L, lcon(0x3), parw04, XUPCAST(s_len8, xparp05), c17p_6);
i32 SKW_PROCEED_XACT_78(void)
  RG1L = SKW_CALC_VECTOR_DIR(unsignedword(ddat.s350.v1e0562.b_06), RG2W, RG3W, RG4W);
  RG1L = (SKW_0cee_04e5(RG5l, lcon(0x1), lcon(0x0), RG3L, parw00) & lcon(0xff)) >> bcon(0x5);
  RG1L = SKW_19f0_0559(RG5l);
i32 SKW_PROCEED_XACT_80(i32 eaxl)
  RG1L = SKW_CREATURE_GO_THERE(unsignedlong(RG1Blo), RG4L, RG2L, lcon(0xffffffff), parw00, parw01);
i32 SKW_PROCEED_XACT_81(void)
  RG1L = SKW_19f0_2813(unsignedlong(RG1Blo), RG4L, RG2L, RG3L, parw00, parw01, parw02);
void SKW_14cd_3582(i32 eaxl, i32 edxl)
  SKW_COUNT_BY_COIN_TYPES(RG1L, RG4P16);
  DM2_ZERO_MEMORY(ADRESSOF(i16, vwa_04), RG4L);
  RG1L = SKW_TAKE_COIN_FROM_WALLET(unsignedlong(RG5w), RG4L);
  SKW_DEALLOC_RECORD(RG1L);
  RG1L = SKW_ALLOC_NEW_DBITEM(unsignedlong(RG1W));
  SKW_ADD_COIN_TO_WALLET(unsignedlong(RG5w), RG4L);
i32 SKW_PROCEED_XACT_82(void)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG1W), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG2L);
  RG1L = SKW_IS_CONTAINER_MONEYBOX(RG4L);
  SKW_14cd_3582(lcon(0x0), RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG2W));
  RG1L = SKW_48ae_0767(RG1L, RG2P16, RG3P, lcon(0x12)); // unused extra argument was lcon(0x0)
  RG1L = SKW_ALLOC_NEW_DBITEM(unsignedlong(ddat.v1e0394[unsignedlong(RG1Blo)]));
  SKW_APPEND_RECORD_TO(RG1L, lcon(0xffffffff), lcon(0x0), RG4P);
  RG1L = SKW_ADD_COIN_TO_WALLET(unsignedlong(RG61W), RG4L);
  RG1L = SKW_PROCEED_XACT_64();
  RG1L = SKW_PROCEED_XACT_64();
  RG1L = SKW_1c9a_0732(lcon(0x3e), unsignedlong(word_at(SKW_GET_ADDRESS_OF_RECORD(RG2L), lcon(0x2))), RG4L);
  RG1L = SKW_14cd_2886(UPCAST(i16, RG51p + lcon(0x2)), lcon(0x10), RG2L, lcon(0x0), parl00, parl01);
  RG1L = SKW_MAX16(lcon(0x0), RG4L);
i32 SKW_PROCEED_XACT_83(void)
  RG1L = SKW_BETWEEN_VALUE16(lcon(0x0), lcon(0x2), RG4L);
i32 SKW_PROCEED_XACT_84(void)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG51l);
  RG1L = SKW_QUERY_GDAT_FOOD_VALUE_FROM_RECORD(RG51l);
  RG1L = SKW_RAND();
  RG1L = SKW_RAND();
  SKW_19f0_2165(lcon(0x81), RG4L, RG2L, RG4L, parl00, lcon(0xffffffff), lcon(0x6));
  SKW_CUT_RECORD_FROM(RG53l, lcon(0xffffffff), lcon(0x0), RG1P);
  SKW_DEALLOC_RECORD(RG53l);
i32 SKW_PROCEED_XACT_85(void)
  RG1L = SKW_GET_TILE_RECORD_LINK(unsignedlong(ddat.s350.v1e0562.b_06), unsignedlong(ddat.s350.v1e0562.b_07));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
  SKW_13e4_0360(unsignedlong(ddat.s350.v1e054c), unsignedlong(ddat.s350.v1e0562.b_06), unsignedlong(ddat.s350.v1e0562.b_07), lcon(0x13), parl00);
i32 SKW_PROCEED_XACT_89(i32 eaxl)
  RG1L = SKW_19f0_0d10(unsignedlong(mkub(vb_00 | bcon(0x80))), RG4L, RG2L, RG3L, parw00, lcon(0xffff));
i32 SKW_PROCEED_XACT_90(void)
  RG1L = SKW_RAND16(lcon(0x64)) & lcon(0xffff);
i32 SKW_PROCEED_XACT_91(void)
  RG1L = SKW_1c9a_0732(signedlong(ddat.s350.v1e0572), unsignedlong(word_at(ddat.s350.v1e054e, lcon(0x2))), lcon(0xff));
  RG1L = SKW_1c9a_0732(signedlong(ddat.s350.v1e0574), unsignedlong(word_at(ddat.s350.v1e054e, lcon(0x2))), lcon(0xff));
i32 SKW_OPERATE_PIT_TELE_TILE(i32 eaxl, i32 edxl, i32 ebxl)
  RG1P = SKW_GET_ADDRESS_OF_TILE_RECORD(signedlong(RG6w), RG4L);
  SKW_ADVANCE_TILES_TIME(RG1L, RG4L);
i32 SKW_CREATURE_WALK_NOW(void)
  SKW_CREATURE_ATTACKS_PARTY();
  RG1L = SKW_CREATURE_GO_THERE(unsignedlong(mkub(byte_at(RG5p, lcon(0x20)) | bcon(0x80))), RG4L, RG2L, RG3L, parw00, parw01);
  RG1L = SKW_OPERATE_PIT_TELE_TILE(RG1L, RG4L, lcon(0x1));
  RG1L = SKW_MOVE_RECORD_TO(unsignedlong(ddat.s350.v1e054c), RG4L, RG2L, RG3L, parw02);
  SKW_1c9a_0648(unsignedlong(ddat.s350.v1e0571));
  RG1L = SKW_OPERATE_PIT_TELE_TILE(unsignedlong(ddat.s350.v1e0562.b_06), unsignedlong(ddat.s350.v1e0562.b_07), lcon(0x0));
i32 SKW_1887_0205(void)
  SKW_1887_05aa();
  RG4L = SKW_CREATURE_WALK_NOW();
i32 SKW_1887_0239(i32 eaxl)
  RG53l = SKW_CREATURE_WALK_NOW();
  RG1L = SKW_CREATURE_WALK_NOW();
  RG1L = SKW_MOVE_RECORD_TO(unsignedlong(ddat.s350.v1e054c), RG4L, RG2L, RG4L, parw00);
  SKW_1c9a_0648(unsignedlong(ddat.s350.v1e0571));
i32 SKW_1887_034e(i32 eaxl)
  RG1L = SKW_CREATURE_GO_THERE(unsignedlong(mkub(byte_at(RG51p, lcon(0x20)) | bcon(0x80))), RG4L, RG2L, RG3L, parw00, parw01);
  RG1L = SKW_FIND_LADDAR_AROUND(unsignedlong(ddat.s350.v1e0562.b_06), RG6L, UPCAST(xxx*, RG3P), unsignedlong(ddat.s350.v1e0562.b_07));
  RG1L = signedlong(CUTX16(SKW_LOCATE_OTHER_LEVEL(RG71l, RG2P16, RG3P16, RG4L, xparpp02)));
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = SKW_FIND_LADDAR_AROUND(signedlong(vw_0c), RG2L, UPCAST(xxx*, ADRESSOF(xxx*, &xp_00)), signedlong(vw_04));
  SKW_CHANGE_CURRENT_MAP_TO(RG71l);
  RG1L = SKW_RAND02();
  RG1L = SKW_OPERATE_PIT_TELE_TILE(unsignedlong(ddat.s350.v1e0562.b_06), unsignedlong(ddat.s350.v1e0562.b_07), lcon(0x1));
  SKW_MOVE_RECORD_TO(unsignedlong(ddat.s350.v1e054c), unsignedlong(ddat.s350.v1e0562.b_06), unsignedlong(ddat.s350.v1e0562.b_07), lcon(0xffffffff), lcon(0x0));
  SKW_1c9a_0648(signedlong(RG1W)); // return unused
  RG6L = SKW_0cee_06dc(signedlong(ddat.v1e08a6), signedlong(ddat.v1e08a4));
  RG1L = SKW_MOVE_RECORD_TO(unsignedlong(ddat.s350.v1e054c), lcon(0xffffffff), lcon(0x0), RG3L, parw04);
  SKW_1c9a_0648(unsignedlong(ddat.s350.v1e0571));
  RG1L = SKW_OPERATE_PIT_TELE_TILE(unsignedlong(ddat.s350.v1e0562.b_06), unsignedlong(ddat.s350.v1e0562.b_07), lcon(0x0));
void SKW_1887_05aa(void)
  RG1L = SKW_RAND();
i32 SKW_CREATURE_ATTACKS_PARTY(void)
  RG1L = SKW_ABS16(CUTX16(unsignedlong(ddat.s350.v1e0562.b_06) - vq_14.peek32()));
  RG1L = signedlong(CUTX16(SKW_ABS16(CUTX16(unsignedlong(ddat.s350.v1e0562.b_07) - vq_10.peek32())))) + RG4L;
  RG1L = SKW_CALC_VECTOR_DIR(unsignedword(ddat.s350.v1e0562.b_06), vq_14.peek16(), RG3W, unsignedword(ddat.s350.v1e0562.b_07)) & lcon(0xffff);
  RG1L = SKW_RAND16(unsignedlong(RG3W)) + 1;
  RG1L = SKW_2c1d_028c(RG1L, RG4L, RG2L);
  RG1L = SKW_MIN16(signedlong(xw_1c.peek16()), RG4L);
  RG1L = SKW_GET_PLAYER_AT_POSITION(unsignedlong(RG6w));
  RG1L = SKW_CALC_VECTOR_DIR(vq_14.peek16(), RG2W, RG3W, RG4W);
  RG1L = SKW_2c1d_028c(RG1L, RG4L, RG2L);
  RG1L = SKW_RAND16(unsignedlong(vo_18.peek16()));
  RG1L = SKW_CREATURE_ATTACKS_PLAYER(ddat.s350.v1e054e, RG4L);
  RG1L = SKW_CALC_VECTOR_DIR(unsignedword(ddat.s350.v1e0562.b_06), RG2W, RG3W, RG4W);
  RG1L = (SKW_GET_TILE_VALUE(signedlong(vq_14.peek16()), signedlong(vq_10.peek16())) & lcon(0xff)) >> bcon(0x5) & lcon(0xff);
  RG1L = SKW_RAND16(unsignedlong(RG1W));
  RG1L = SKW_ATTACK_DOOR(unsignedlong(vq_14.peek16()), RG4L, RG2L, lcon(0x0), lcon(0x0));
  RG1L = SKW_CREATURE_ATTACKS_CREATURE(signedlong(vq_14.peek16()), RG4L);
  RG1L = SKW_GET_CREATURE_WEIGHT(unsignedlong(ddat.s350.v1e054c)) & lcon(0xffff);
  RG1L = SKW_RAND();
  RG1L = SKW_RAND02();
  SKW_12b4_0d75(signedlong(vq_14.peek16()), signedlong(vq_10.peek16()), signedlong(xw_0c.peek16()), lcon(0xfe));
i32 SKW_1887_09ab(void)
  RG1L = SKW_RAND16(unsignedlong(vo_00.peek16()));
  RG1L = SKW_RAND16(unsignedlong(RG4W));
  RG1L = SKW_BETWEEN_VALUE16(lcon(0x4), lcon(0xff), RG4L);
  RG4L = SKW_RAND02() + lcon(0x7);
  RG4L = SKW_RAND01() + lcon(0x3);
  SKW_SHOOT_ITEM(signedlong(byte_at(RG3P, lcon(0x1e))), unsignedlong(ddat.s350.v1e0562.b_06), unsignedlong(ddat.s350.v1e0562.b_07), unsignedlong(byte_at(RG3P, lcon(0x1c))), parl00, parb01, parb02, parb03);
  RG1L = SKW_WOUND_CREATURE((SKW_RAND01() & lcon(0xffff)) + RG4L & lcon(0xffff));
i32 SKW_1887_0b1b(void)
  RG1L = SKW_GET_PLAYER_AT_POSITION(unsignedlong(byte_at(ddat.s350.v1e0556, lcon(0x1c))));
  RG1L = SKW_RAND();
  RG1L = SKW_USE_DEXTERITY_ATTRIBUTE(unsignedlong(RG3W));
  RG1L = SKW_USE_LUCK_ATTRIBUTE(RG2P, RG4L);
  RG1L = SKW_1c9a_0006(RG2L, RG4L);
  RG1L = R_F958(RG2L);
  RG1L = SKW_1c9a_0006(RG2L, RG4L);
  RG1L = R_F958(RG2L);
  RG1L = SKW_1c9a_0006(unsignedlong(RG3W), RG4L);
  RG1L = SKW_REMOVE_POSSESSION(unsignedlong(vq_0c.peek16()), unsignedlong(xwc_08.peek16()));
  SKW_APPEND_RECORD_TO(RG1L, lcon(0xffffffff), lcon(0x0), RG4P);
  RG1L = SKW_RAND();
  RG1L = (SKW_QUERY_PLAYER_SKILL_LV(signedlong(vq_0c.peek16()), RG6l, RG6l) & lcon(0xffff)) + RG3L;
  RG1L = SKW_RESUME_FROM_WAKE();
i32 SKW_1887_0c75(void)
  RG1L = SKW_19f0_0d10(unsignedlong(mkub(byte_at(ddat.s350.v1e0556, bcon(0x20)) | lcon(0xffffff80))), RG4L, RG5l, RG3L, parw00, parw01);
  SKW_INVOKE_MESSAGE(unsignedlong(ddat.v1e08a6), lcon(0x0), lcon(0x2), unsignedlong(ddat.v1e08a4), parl02);
i32 SKW_1887_0ce1(void)
  SKW_1887_05aa();
  SKW_1887_05aa();
  RG4L = SKW_1887_0e19();
i32 SKW_1887_0d33(void)
  RG1L = SKW_1c9a_0732(signedlong(byte_at(ddat.s350.v1e0556, lcon(0x1e))), RG4L, lcon(0xff));
  SKW_CUT_RECORD_FROM(RG1L, lcon(0xffffffff), lcon(0x0), RG4P);
  SKW_MOVE_RECORD_TO(RG52l, lcon(0xffffffff), lcon(0x0), RG3L, parw00);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG52l);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG52l) & lcon(0xff);
  SKW_QUEUE_NOISE_GEN2(RG1Blo, lcon(0x85), lcon(0xfe), RG4Blo, parw01, parw02, parw03, lcon(0x3a), lcon(0x80));
void SKW_1887_0df7(void)
  SKW_DELETE_CREATURE_RECORD(unsignedlong(ddat.s350.v1e0562.b_06), lcon(0x0), lcon(0x1), unsignedlong(ddat.s350.v1e0562.b_07));
i32 SKW_1887_0e19(void)
  RG1L = SKW_GET_WALL_TILE_ANYITEM_RECORD(signedlong(ddat.v1e08a6), RG4L);
  RG1L = SKW_1c9a_0006(RG5l, RG4L);
  SKW_MOVE_RECORD_TO(RG5l, signedlong(ddat.v1e08a6), signedlong(ddat.v1e08a4), lcon(0xffffffff), lcon(0x0));
  SKW_APPEND_RECORD_TO(RG1L, RG2L, RG3L, RG4P);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG6W));
i32 SKW_1887_0eca(void)
  RG1L = SKW_1c9a_0732(signedlong(byte_at(ddat.s350.v1e0556, lcon(0x1e))), RG4L, lcon(0xff));
  SKW_CUT_RECORD_FROM(RG5l, lcon(0xffffffff), lcon(0x0), RG4P);
  RG1L = SKW_RAND16(unsignedlong(RG4W));
  RG1L = SKW_RAND16(unsignedlong(RG4W));
  RG1L = SKW_BETWEEN_VALUE16(lcon(0x14), lcon(0xff), RG4L) & lcon(0xff);
  SKW_SHOOT_ITEM(RG5l, unsignedlong(ddat.s350.v1e0562.b_06), unsignedlong(ddat.s350.v1e0562.b_07), unsignedlong(byte_at(RG1P, lcon(0x1c))), parl00, parb01, parb02, lcon(0x8));
i32 SKW_1887_0f80(void)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(ddat.v1e08a6), unsignedlong(ddat.v1e08a4));
  SKW_ROTATE_CREATURE(RG1L, lcon(0x0), RG2L);
i32 SKW_PLACE_MERCHANDISE(void)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vq_08.peek16()), RG4L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG1W));
  RG1L = SKW_1c9a_0732(signedlong(byte_at(ddat.s350.v1e0556, lcon(0x1e))), RG4L, lcon(0xff));
  SKW_CUT_RECORD_FROM(RG1L, lcon(0xffffffff), lcon(0x0), RG4P);
  RG1L = SKW_RAND02() & lcon(0xffff);
  RG1L = SKW_IS_CONTAINER_MONEYBOX(RG51l);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG51l);
  SKW_MOVE_RECORD_TO(RG52l, RG2L, RG2L, RG3L, parw00);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG52l);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG52l) & lcon(0xff);
  SKW_QUEUE_NOISE_GEN2(RG1Blo, lcon(0x85), lcon(0xfe), RG4Blo, parw01, parw02, parw03, lcon(0x3a), lcon(0x80));
i32 SKW_1887_10db(void)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(ddat.v1e08a6), unsignedlong(ddat.v1e08a4));
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG1W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(xwc_00.peek16()));
  RG1L = SKW_1c9a_0732(signedlong(byte_at(ddat.s350.v1e0556, lcon(0x1e))), RG4L, RG2L);
  SKW_MOVE_RECORD_TO(RG1L, RG4L, RG2L, lcon(0xffffffff), lcon(0xffff));
  SKW_APPEND_RECORD_TO(RG1L, lcon(0xffffffff), lcon(0x0), RG4P);
i32 SKW_1887_1191(void)
  RG1L = SKW_19f0_2813(unsignedlong(xbc_00.peek8()), RG4L, RG2L, RG3L, parw00, parw01, parw02);
  RG1L = SKW_1c9a_0732(signedlong(byte_at(RG5p, lcon(0x1e))), RG4L, lcon(0xff));
  SKW_CUT_RECORD_FROM(RG1L, lcon(0xffffffff), lcon(0x0), RG4P);
  R_3C1E5(RG1L, RG4L, RG2L, RG3L, parl03);
void SKW_1887_126d(void)
  SKW_CREATE_CLOUD(lcon(0xffa8), lcon(0xff), unsignedlong(vq_00.peek16()), RG3L, parl04);
  RG51L = SKW_RAND02() + 1;
  RG1L = SKW_RAND16(lcon(0x5));
  RG1L = SKW_RAND16(lcon(0x9c)) + lcon(0x64);
  SKW_CREATE_CLOUD(lcon(0xff83), RG4L, RG2L, RG3L, parl00);
  SKW_CREATE_CLOUD(lcon(0xffa8), RG4L, RG2L, RG3L, parl01);
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(RG4Blo));
  RG1L = SKW_RAND16(signedlong(RG52w) / 8 + 1 & lcon(0xffff));
  SKW_GET_CREATURE_ANIMATION_FRAME(RG1L, RG4L, RG2P16, RG3P16, xparpp02, parl03);
  RG1L = SKW_IS_CREATURE_ALLOWED_ON_LEVEL(unsignedlong(ddat.s350.v1e054c), unsignedlong(ddat.s350.v1e0571));
  SKW_DELETE_CREATURE_RECORD(RG1L, lcon(0x0), lcon(0x1), RG4L);
  SKW_QUEUE_NOISE_GEN1(lcon(0x3), RG2Blo, RG3W, lcon(0x0), parw05, parw06, parw07, lcon(0x1));
i32 SKW_1887_141c(void)
  RG1L = SKW_RAND16(unsignedlong(RG4W));
  RG1L = SKW_RAND16(unsignedlong(RG4W));
  RG1L = SKW_BETWEEN_VALUE16(lcon(0x14), lcon(0xff), RG4L);
  SKW_CREATE_CLOUD(lcon(0xff80), RG4L, RG51l, RG3L, parl00);
  SKW_1887_0df7();
  RG1L = SKW_RAND02();
  RG1L = SKW_CREATE_MINION(signedlong(byte_at(ddat.s350.v1e0556, lcon(0x1e))), lcon(0x7), RG2L, RG3L, parl01, parw02, parl03, parb04);
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(RG4Blo));
  RG1L = SKW_RAND16(signedlong(RG4W) / 8 + 1 & lcon(0xffff));
  SKW_GET_CREATURE_ANIMATION_FRAME(RG1L, RG4L, RG2P16, RG3P16, xparpp05, parl06);
  RG1L = SKW_IS_CREATURE_ALLOWED_ON_LEVEL(unsignedlong(ddat.s350.v1e054c), unsignedlong(ddat.s350.v1e0571));
  SKW_DELETE_CREATURE_RECORD(RG1L, lcon(0x0), lcon(0x1), RG4L);
i32 R_1B7D5(void)
  RG1L = SKW_RAND16(lcon(0x28)) + lcon(0x14);
  return SKW_CREATE_CLOUD(lcon(0xff8e), RG4L, RG2L, RG3L, parl00);
i32 SKW_PROCEED_CCM(void)
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x100));
  RG1L = SKW_1887_0205();
  RG1L = SKW_1887_0239(RG1L);
  SKW_1887_05aa();
  RG1L = SKW_1887_0b1b();
  RG1L = SKW_1887_0c75();
  RG1L = SKW_1887_0ce1();
  RG1L = SKW_1887_0eca();
  SKW_1887_0df7();
  RG1L = SKW_1887_0f80();
  RG1L = SKW_CREATURE_WALK_NOW();
  RG1L = SKW_PLACE_MERCHANDISE();
  RG1L = SKW_1887_10db();
  RG1L = SKW_1887_09ab();
  RG1L = SKW_1887_1191();
  RG1L = SKW_1887_034e(RG1L);
  SKW_1887_126d();
  RG1L = SKW_1887_141c();
  RG1L = R_1B7D5();
  RG1L = SKW_1887_0d33();
  RG1L = SKW_1887_0e19();
  RG1L = SKW_CREATURE_ATTACKS_PARTY();
  DM2_DEALLOC_LOBIGPOOL(lcon(0x100));
i32 R_1BA1B(i32 eaxl, i32 edxl)
  RG1L = SKW_GET_TILE_VALUE(RG3L, RG4L);
  RG1L = SKW_GET_GRAPHICS_FOR_DOOR(SKW_IS_REBIRTH_ALTAR(SKW_GET_ADDRESS_OF_TILE_RECORD(RG3L, RG4L)) & lcon(0xff));
i32 R_1BAAD(i32 eaxl, i32 edxl)
  RG1L = SKW_GET_TILE_VALUE(signedlong(vw_04), signedlong(vw_00));
  RG1L = SKW_0cee_3275(SKW_IS_REBIRTH_ALTAR(SKW_GET_ADDRESS_OF_TILE_RECORD(signedlong(vw_04), RG4L)) & lcon(0xff));
  RG1L = SKW_RAND01();
  RG1L = SKW_GET_WALL_TILE_ANYITEM_RECORD(signedlong(vw_04), signedlong(vw_00));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG3W));
  RG1L = SKW_1c9a_03cf(&vw_04, RG2L, RG4P16);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG4W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG3W));
i32 R_1BC29(i32 eaxl, i32 edxl)
  return R_1BAAD(signedlong(RG1W), signedlong(RG4W));
i32 SKW_19f0_0207(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, FPTR_L_LL argf0)
  RG1L = SKW_ABS16(RG1W);
  RG1L = signedlong(CUTX16(SKW_ABS16(CUTX16(vq_2c.peek32() - RG3L)))) + RG4L;
  RG1L = SKW_ABS16(CUTX16(RG3L - RG6L));
  RG1L = signedlong(CUTX16(SKW_ABS16(xw_1c.peek16()))) + RG4L;
  RG1W = SKW_CALC_SQUARE_DISTANCE(RG6W, RG2W, RG3W, RG4W);
i32 SKW_19f0_045a(i32 eaxl, i32 edxl)
  RG1L = SKW_GET_TILE_VALUE(signedlong(RG1W), RG4L);
i32 SKW_19f0_04bf(void)
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(ddat.v1e08a8), RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG1W));
i32 SKW_19f0_050f(void)
  RG1L = SKW_19f0_04bf();
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG1W));
i32 SKW_19f0_0547(i32 eaxl, i32 edxl)
  return SKW_1c9a_0006(unsignedlong(RG1W), signedlong(RG4W));
i32 SKW_19f0_0559(i32 eaxl)
  RG1L = SKW_RAND01();
i32 SKW_19f0_05e8(i32 eaxl, xxx* xedxp, xxx* xebxp, i32 ecxl, i16 argw0, i32 argl1, i32 argl2)
  RG1L = SKW_GET_TILE_VALUE(RG5L, RG4L);
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vq_18.peek16()), unsignedlong(vo_14.peek16()));
  RG1L = SKW_19f0_0547(unsignedlong(RG5W), signedlong(vql_0c.peek16()));
  RG1L = SKW_GET_TILE_RECORD_LINK(RG5L, RG4L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG1L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG5W));
  RG1L = R_1BC29(RG1L, RG4L);
  RG1L = R_1BC29(RG1L, RG4L);
  RG1L = SKW_19f0_0891((argl2 != lcon(0x0) ? 1 : 0) + lcon(0x2), RG4L, RG2L, RG3L, parw00, parw01);
  SKW_19f0_045a(signedlong(vol_00.peek16()), signedlong(argw0));
i32 SKW_19f0_0891(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1)
  RG1W = SKW_CALC_SQUARE_DISTANCE(RG6W, RG2W, RG3W, RG4W);
  RG1L = SKW_CREATURE_GO_THERE(lcon(0x0), RG4L, RG2L, RG3L, parw00, parw01);
  SKW_19f0_045a(signedlong(vq_08.peek16()), signedlong(argw0));
  RG1L = SKW_19f0_050f();
  pf_02 = R_1BAAD;
  RG1L = SKW_19f0_0207(signedlong(RG6W), RG4L, RG2L, RG3L, pf_02);
  RG1L = SKW_2c1d_028c(signedlong(RG6W), signedlong(xw_00.peek16()), RG2L);
  RG1L = SKW_1c9a_0006(unsignedlong(RG4W), lcon(0xb));
  RG1L = SKW_1c9a_0006(unsignedlong(RG4W), lcon(0xb));
  RG1L = SKW_RAND01();
  SKW_19f0_045a(signedlong(vq_08.peek16()), signedlong(argw0));
  RG1L = SKW_19f0_04bf();
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
  RG1L = (SKW_GET_TILE_VALUE(RG55L, RG7l) & lcon(0xff)) >> bcon(0x5) & lcon(0xff);
  RG1L = signedlong(SKW_CALC_SQUARE_DISTANCE(RG55W, RG2W, RG3W, RG4W));
  RG1L = SKW_RAND02();
  RG1L = SKW_CALC_VECTOR_DIR(RG6W, RG2W, RG3W, RG4W);
  RG1L = SKW_19f0_0559(signedlong(argw1));
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND01();
  RG1L = SKW_2c1d_028c(signedlong(RG6W), RG4L, lcon(0xff));
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND01();
  RG1L = signedlong(CUTX16(SKW_GET_PLAYER_AT_POSITION(unsignedlong(RG2W))));
  RG1L = signedlong(CUTX16(SKW_GET_PLAYER_AT_POSITION(unsignedlong(RG2W))));
  RG1L = SKW_RAND16(SKW_1c9a_0598(RG56l) & lcon(0xffff)) + 1;
  RG1L = SKW_COMPUTE_POWER_4_WITHIN(RG56l, RG4L);
i32 SKW_19f0_0d10(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1)
  RG1L = SKW_CALC_VECTOR_DIR(vq_08.peek16(), RG2W, RG3W, RG4W);
  SKW_19f0_045a(signedlong(xw_10.peek16()), signedlong(argw0));
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(xw_10.peek16()), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(ddat.v1e08b0));
  RG1W = SKW_CALC_SQUARE_DISTANCE(vq_08.peek16(), RG2W, RG3W, RG4W);
  pf_00 = R_1BAAD;
  RG1L = SKW_19f0_0207(signedlong(vq_08.peek16()), RG4L, RG2L, RG3L, pf_00);
  RG1L = SKW_GET_WALL_TILE_ANYITEM_RECORD(signedlong(RG62W), signedlong(RG3W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
  RG1L = SKW_CALC_VECTOR_DIR(vq_08.peek16(), RG2W, RG3W, RG4W);
  RG1L = SKW_19f0_0559(signedlong(argw1));
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND01();
  RG1L = SKW_19f0_0891(lcon(0x84), signedlong(vq_08.peek16()), signedlong(xw_0c.peek16()), signedlong(xw_10.peek16()), parw01, parw02);
  RG1L = SKW_RAND01();
i32 SKW_19f0_124b(i16* eaxpw, i32 ebxl, i32 ecxl, i16* edxpw, i32 argl0)
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_04.peek16()));
  RG1L = SKW_GET_TILE_VALUE(signedlong(word_at(RG61p)), RG4L);
  RG1L = SKW_FIND_LADDAR_AROUND(signedlong(word_at(RG61p)), RG2L, UPCAST(xxx*, ADRESSOF(xxx*, &xp_00)), RG4L);
  RG1L = SKW_LOCATE_OTHER_LEVEL(signedlong(xw_04.peek16()), UPCAST(i16, RG61p), UPCAST(i16, RG5p), signedlong(vq_0c.peek16()), NULL);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = SKW_GET_TILE_VALUE(signedlong(word_at(RG61p)), RG4L);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_04.peek16()));
i32 SKW_19f0_13aa(i32 eaxl, i32 edxl)
  RG1L = SKW_RAND();
  RG1L = SKW_GET_WALL_TILE_ANYITEM_RECORD(signedlong(RG6W), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG2W));
  RG1L = SKW_075f_06bd(RG1P, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG2W));
  RG1L = R_1BAAD(signedlong(RG6W), RG4L);
i32 SKW_19f0_1511(i32 eaxl)
  return SKW_1c9a_0006(unsignedlong(RG1W), lcon(0x9));
i32 SKW_CREATURE_GO_THERE(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1)
  RG1L = SKW_CALC_VECTOR_DIR(vw_28, RG2W, RG3W, RG4W);
  RG1L = SKW_GET_TILE_VALUE(signedlong(vw_28), RG4L);
  RG1L = SKW_0cee_06dc(signedlong(vw_28), signedlong(vw_24)) + lcon(0x2);
  RG1L = SKW_FIND_LADDAR_AROUND(signedlong(vw_28), RG2L, UPCAST(xxx*, RG3P), RG4L);
  RG1L = SKW_LOCATE_OTHER_LEVEL(signedlong(ddat.v1d3248), RG2P16, RG3P16, RG4L, NULL);
  RG1L = SKW_IS_CREATURE_ALLOWED_ON_LEVEL(unsignedlong(ddat.s350.v1e054c), RG4L);
  SKW_1c9a_0648(signedlong(vq_4c.peek16()));
  SKW_19f0_045a(RG1L, RG4L);
  RG1L = R_1BA1B(signedlong(RG5W), RG4L);
  SKW_19f0_045a(RG1L, RG4L);
  SKW_19f0_045a(RG1L, RG4L);
  RG1L = SKW_QUERY_DOOR_STRENGTH(SKW_IS_REBIRTH_ALTAR(SKW_GET_ADDRESS_OF_TILE_RECORD(RG3L, RG4L)) & lcon(0xff));
  RG1L = SKW_GET_TELEPORTER_DETAIL(&ddat.v1e08b8, RG2L, RG4L);
  RG1P = R_D283(RG3L, RG4L);
  SKW_1c9a_0648(RG1L);
  RG1L = SKW_CREATURE_GO_THERE(unsignedlong(vb_54), RG4L, RG1L, RG4L, parw01, parw02);
  SKW_1c9a_0648(signedlong(vq_4c.peek16()));
  SKW_19f0_045a(RG1L, RG4L);
  RG1L = signedlong(CUTX16(SKW_1c9a_1a48(lcon(0x1), RG4L)));
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(RG5W), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(ddat.v1e08b0));
  RG1L = SKW_GET_TELEPORTER_DETAIL(&ddat.v1e08b8, RG2L, RG4L);
  RG1L = SKW_IS_CREATURE_ALLOWED_ON_LEVEL(unsignedlong(ddat.s350.v1e054c), unsignedlong(ddat.v1e08b8.b_04));
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(RG5W), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(ddat.v1e08b0));
  RG1L = SKW_ABS16(RG1W);
  RG1L = signedlong(CUTX16(SKW_ABS16(RG1W))) + RG4L;
  RG1L = SKW_19f0_050f();
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(unsignedlong(RG3W));
  RG1L = SKW_19f0_1511(unsignedlong(RG3W));
  RG1L = SKW_19f0_04bf();
  RG1L = SKW_19f0_13aa(signedlong(RG5W), signedlong(argw0));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG3W));
  RG1L = SKW_075f_06bd(RG2P, RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG2L);
  RG1L = SKW_CALC_CLOUD_DAMAGE(RG2L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG3W));
  RG1L = signedlong(SKW_CALC_SQUARE_DISTANCE(RG5W, RG2W, RG3W, RG4W));
  RG1L = SKW_RAND02();
  return SKW_19f0_0d10(unsignedlong(vq_30.peek8()), signedlong(vw_28), signedlong(vw_24), RG3L, parw07, parw08);
  RG1L = SKW_CREATURE_GO_THERE(unsignedlong(mkub(vb_54 | vq_20.peek8())), RG4L, RG2L, RG3L, parw09, parw10);
  RG1L = SKW_CREATURE_GO_THERE(unsignedlong(mkub(vb_54 | vq_20.peek8())), RG4L, RG2L, RG3L, parw11, parw12);
  SKW_2fcf_0434(RG1L, RG4L, RG2L, RG3L, parl03, parl04);
  RG1L = SKW_IS_CREATURE_ALLOWED_ON_LEVEL(unsignedlong(ddat.s350.v1e054c), RG4L);
  RG1L = SKW_1c9a_0648(signedlong(ddat.v1e102c));
  RG1L = SKW_CREATURE_GO_THERE(RG1L, RG4L, RG2L, RG3L, parw05, parw06);
  SKW_1c9a_0648(RG1L);
  RG1L = SKW_19f0_0891(lcon(0x82), signedlong(vw_28), RG2L, RG3L, parw13, parw14);
  RG1L = (SKW_GET_TILE_VALUE(RG3L, RG2L) & lcon(0xff)) >> bcon(0x5) & lcon(0xff);
  RG1L = SKW_19f0_0d10(lcon(0x82), signedlong(vw_28), RG2L, RG3L, parw15, parw16);
  RG1L = SKW_ABS16(vw_28 - RG5W);
  RG1L = SKW_ABS16(vw_24 - argw0) + RG4L;
  RG1L = SKW_RAND02();
  SKW_19f0_0559(signedlong(vq_44.peek16()));
  SKW_1c9a_0648(signedlong(xw_2c.peek16()));
i32 SKW_19f0_2024(i32 eaxl, i32 edxl, i32 ebxl)
  RG1L = SKW_IS_CONTAINER_CHEST(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG6W));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG1P, lcon(0x4))));
  RG1L = SKW_48ae_01af(signedlong(word_at(RG3P, lcon(0x1e))), RG4L);
  RG1L = SKW_1c9a_0006(RG3L, RG52l);
  RG1L = SKW_19f0_2024(RG3L, RG4L, RG2L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG6W));
i32 SKW_19f0_2165(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i16 argw1, i16 argw2)
  RG1L = SKW_CALC_VECTOR_DIR(RG1W, RG2W, RG3W, CUTX16(vl_04));
  SKW_19f0_045a(signedlong(RG51w), RG4L);
  RG1L = SKW_19f0_04bf();
  RG1L = SKW_1c9a_0006(RG3L, RG4L);
  RG1L = SKW_19f0_2024(unsignedlong(RG54w), RG4L, RG2L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG54w));
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(RG51w), RG4L);
  RG1L = SKW_IS_WALL_ORNATE_ALCOVE_FROM_RECORD(RG1L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG53W));
  RG1L = SKW_1c9a_0006(RG3L, RG4L);
  RG1L = SKW_19f0_2024(RG3L, RG4L, RG2L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG52W));
  RG1L = SKW_19f0_0559(signedlong(argw1));
  SKW_19f0_0559(RG1L);
  RG1L = SKW_RAND01();
  RG1L = SKW_48ae_01af(signedlong(CUTX16(SKW_0cee_2df4(SKW_19f0_050f() & lcon(0xffff)))), RG4L);
  RG4L = signedlong(CUTX16(SKW_RAND16(SKW_1c9a_0598(RG2L) & lcon(0xffff)) + 1));
  RG1L = SKW_COMPUTE_POWER_4_WITHIN(RG2L, RG4L);
i32 SKW_19f0_266c(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG2L);
  RG1L = SKW_1c9a_0006(RG2L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG5W));
i32 SKW_19f0_2723(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG6w));
  return SKW_1c9a_0006(unsignedlong(RG6w), RG4L);
i32 SKW_19f0_2813(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1, i16 argw2)
  RG1L = SKW_CALC_VECTOR_DIR(RG71W, RG2W, RG3W, RG4W);
  SKW_19f0_045a(signedlong(xw_04.peek16()), RG4L);
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(ddat.v1e08a8), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG1L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
  RG1L = SKW_19f0_266c(unsignedlong(ddat.v1e08b0), RG4L, RG72L, RG63l);
  RG1L = SKW_19f0_2723(unsignedlong(RG1W), RG72L, RG63l, RG3L);
  RG1L = SKW_19f0_0559(signedlong(argw1));
i32 SKW_1c9a_0006(i32 eaxl, i32 edxl)
  RG1L = SKW_IS_CONTAINER_MONEYBOX(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1L = SKW_IS_CONTAINER_MONEYBOX(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1L = SKW_QUERY_CREATURES_ITEM_MASK(RG1L, RG2P, RG3L, RG4L);
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(unsignedlong(RG6W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG6W));
  RG1L = SKW_GET_WALL_DECORATION_OF_ACTUATOR(RG4P);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG6W));
  RG1L = SKW_IS_CONTAINER_MONEYBOX(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
void SKW_1c9a_0247(i32 eaxl)
  RG1L = SKW_3e74_5673(signedlong(RG3W) | RG4L, lcon(0x0), &vw_00);
  SKW_3e74_58a3(CUTX16(RG1L));
  RG1L = SKW_3e74_5673(signedlong(RG3W) | RG4L, lcon(0x0), &vw_00);
  SKW_3e74_58a3(vw_00);
i32 SKW_CHECK_RECOMPUTE_LIGHT(void)
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
  DM2_ZERO_MEMORY(ddat.v1e08cc, RG4L);
  DM2_ZERO_MEMORY(ddat.v1e08c8, RG4L);
  SKW_1c9a_1bd8(RG1L, RG4L, lcon(0x8), lcon(0x1), s23p_1, c17p_2);
  SKW_1c9a_1bd8(RG1L, RG4L, lcon(0x7), lcon(0x1), s23p_4, c17p_5);
  SKW_RECALC_LIGHT_LEVEL();
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
i32 SKW_GET_CREATURE_AT(i32 eaxl, i32 edxl)
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(RG1W), RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG1W));
i32 SKW_1c9a_03cf(i16* eaxpw, i32 ebxl, i16* edxpw)
  SKW_098d_000f(signedlong(vq_18.peek16()), RG2L, &vw_04, signedlong(vq_1c.peek16()), &vw_14);
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vq_18.peek16()), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(vql_28.peek16()));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG2P, lcon(0x4))));
  RG1L = SKW_QUERY_CREATURE_5x5_POS(RG2P, RG4L);
  SKW_098d_000f(signedlong(vq_18.peek16()), RG2L, &vw_10, RG4L, &vw_0c);
i32 SKW_1c9a_0648(i32 eaxl)
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
i32 SKW_1c9a_0694(i16* eaxpw, i16* edxpw)
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(unsignedlong(word_at(RG1P)));
i16* SKW_1c9a_06bd(i32 eaxl, i32 edxl, i32 ebxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1P = DOWNCAST(i16, SKW_OVERSEE_RECORD(RG1P16, UPCAST(i16*, ADRESSOF(i16*, &wp_04)), SKW_1c9a_0694, RG4L, &vw_00, parl01, parl02)); // TODO
i32 SKW_1c9a_0732(i32 eaxl, i32 edxl, i32 ebxl)
  RG1L = SKW_1c9a_0006(unsignedlong(RG3W), RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG3W));
i32 SKW_1c9a_078b(xxx* xeaxp, i32 edxl, i32 ebxl)
  vql_10.poke32(SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_10.peek16())));
  RG1L = SKW_1c9a_0006(RG61L, RG4L);
  RG1L = SKW_IS_CONTAINER_MONEYBOX(RG61L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(vql_0c.peek16()));
  SKW_1c9a_078b(RG1P, RG4L, RG2L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG62W));
  SKW_CUT_RECORD_FROM(RG1L, lcon(0xffffffff), lcon(0x0), RG4P);
  SKW_APPEND_RECORD_TO(RG1L, RG2L, RG3L, RG4P);
  SKW_CUT_RECORD_FROM(RG63L, RG2L, RG3L, RG4P);
  SKW_DEALLOC_RECORD(RG1L);
i32 SKW_IS_CREATURE_FLOATING(i32 eaxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG4P, lcon(0x4))));
  RG1L = SKW_1c9a_08bd(RG4P);
i32 SKW_1c9a_0958(i32 eaxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1P = DOWNCAST(i16, SKW_1c9a_02c3(RG2P, SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG2P, lcon(0x4))))));
  R_4DEA(RG1L, RG4L, RG2P16, RG3P32);
i32 SKW_1c9a_09b9(i32 eaxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(eaxl & lcon(0xffff));
void SKW_1c9a_09db(xxx* xeaxp)
  RG1P = DOWNCAST(i16, SKW_1c9a_02c3(RG2P, SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG2P, lcon(0x4))))));
  SKW_GET_CREATURE_ANIMATION_FRAME(unsignedlong(byte_at(RG2P, lcon(0x4))), lcon(0x11), RG1P16, UPCAST(i16, RG1P + lcon(0x2)), xparpp00, parl01);
i32 SKW_CREATURE_SOMETHING_1c9a_0a48(void)
  SKW_GET_CREATURE_ANIMATION_FRAME(RG1L, RG4L, RG2P16, RG3P16, xparpp00, parl01);
  DM2_ZERO_MEMORY(ADRESSOF(i8, vba_00), lcon(0x4));
  RG1L = SKW_RAND16(unsignedlong(RG4W));
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND16(unsignedlong(RG4W));
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND01();
  SKW_QUEUE_NOISE_GEN1(lcon(0xf), RG2Blo, lcon(0x46), xbc_0c.peek8(), lcon(0x80), parw03, parw04, lcon(0x1));
  RG1L = SKW_RAND16(unsignedlong(RG4W));
  RG4L += SKW_RAND01();
  RG4L = SKW_MAX16(lcon(0x1), signedlong(RG1W));
  RG1L = signedlong(CUTX16(SKW_MIN16(lcon(0x1), RG4L)));
i32 SKW_1c9a_0cf7(i32 eaxl, i32 edxl)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vql_00.peek16()), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG2L);
  SKW_1c9a_0db0(RG2L);
  RG1L = SKW_QUEUE_TIMER(&c38_04);
void SKW_1c9a_0db0(i32 eaxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  SKW_DELETE_TIMER(RG1L);
void SKW_ALLOC_CAII_TO_CREATURE(i32 eaxl, i32 edxl, i32 ebxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG3W));
  DM2_ZERO_MEMORY(RG5p, lcon(0x22));
  RG1P = SKW_13e4_0004(unsignedlong(RG3W), RG4L, RG2L, lcon(0x22));
  SKW_14cd_0802();
  SKW_1c9a_0cf7(unsignedlong(vq_08.peek16()), unsignedlong(vq_04.peek16()));
  SKW_CREATURE_SOMETHING_1c9a_0a48();
  SKW_13e4_0329(RG2P);
  RG1L = SKW_RECYCLE_A_RECORD_FROM_THE_WORLD(lcon(0x4), RG4L);
  SKW_RAISE_SYSERR(lcon(0x47));
void SKW_1c9a_0fcb(i32 eaxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG3L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(RG3L);
  SKW_1c9a_0db0(RG1L);
  SKW_DELETE_CREATURE_RECORD(unsignedlong(xwc_00.peek16()), lcon(0x0), lcon(0x1), unsignedlong(xwc_04.peek16()));
i32 SKW_CREATE_MINION(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i16 argw1, i32 argl2, i8 argb3)
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = SKW_RAND01();
  RG1L = SKW_GET_TILE_VALUE(signedlong(vq_24.peek16()), RG4L);
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vq_24.peek16()), RG4L);
  RG1L = SKW_ALLOC_NEW_CREATURE(RG1L, RG4L, RG2L, RG3L, parl00);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG2W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG1W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_20.peek16()));
void SKW_RELEASE_MINION(i32 eaxl)
  RG1P = SKW_GET_MISSILE_REF_OF_MINION(RG5l, RG4L);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  SKW_13e4_0360(RG5l, RG4L, RG2L, lcon(0x13), parl00);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
i32 SKW_ALLOC_NEW_CREATURE(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
  RG1L = SKW_ALLOC_NEW_RECORD(lcon(0xe));
  RG1L = SKW_ALLOC_NEW_RECORD(lcon(0x4));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(RG1Blo));
  SKW_APPEND_RECORD_TO(unsignedlong(xwc_10.peek16()), lcon(0xffffffff), lcon(0x0), RG4P);
  RG1L = SKW_MIN16(signedlong(RG61w), lcon(0x1f));
  RG1L = SKW_RAND16(unsignedlong(RG63W) / 8 + 1 & lcon(0xffff));
  RG1L = SKW_MOVE_RECORD_TO(unsignedlong(vql_04.peek16()), lcon(0xfffffffc), lcon(0x0), signedlong(xw_08.peek16()), parw00) & lcon(0xffff);
  SKW_DEALLOC_RECORD(unsignedlong(xwc_10.peek16()));
void SKW_DROP_CREATURE_POSSESSION(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl, i16 argw0)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(xwc_08.peek16()));
  RG1L = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(RG1L, RG4L);
  RG1L = SKW_RAND16(unsignedlong(RG1W));
  RG1L = SKW_ALLOC_NEW_DBITEM(unsignedlong(vol_18.peek16()));
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND02();
  SKW_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw00);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG4L) & lcon(0xff);
  SKW_QUEUE_NOISE_GEN2(RG1Blo, RG2Blo, RG3Blo, RG4Blo, parw01, parw02, parw03, lcon(0x3a), lcon(0x80));
  vl_20 = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG62W));
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(xwc_08.peek16()));
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND02();
  SKW_MOVE_RECORD_TO(RG52l, RG4L, RG2L, RG3L, parw06);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG52l);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG52l) & lcon(0xff);
  SKW_QUEUE_NOISE_GEN2(RG1Blo, RG2Blo, RG3Blo, RG4Blo, parw07, parw08, parw09, lcon(0x3a), lcon(0x80));
  SKW_DEALLOC_RECORD(RG1L);
void SKW_DELETE_CREATURE_RECORD(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vql_0c.peek16()), unsignedlong(vql_08.peek16()));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG51p, lcon(0x4))));
  RG1L = table1d607e[signedlong(CUTX16(SKW_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(byte_at(RG51p, lcon(0x4))), lcon(0x1))))].ul_00;
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  SKW_INVOKE_MESSAGE(RG1L, lcon(0x0), lcon(0x0), RG4L, parl00);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_10.peek16()));
  SKW_MOVE_RECORD_TO(RG52l, RG4L, RG2L, lcon(0xfffffffc), lcon(0x0));
  SKW_DROP_CREATURE_POSSESSION(RG52l, unsignedlong(vql_08.peek16()), unsignedlong(xwc_04.peek16()), unsignedlong(vql_0c.peek16()), parw02);
  SKW_1c9a_0247(RG52l);
  SKW_DEALLOC_RECORD(RG52l);
i32 SKW_CREATURE_ATTACKS_CREATURE(i32 eaxl, i32 edxl)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG6w), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG1P, lcon(0x4))));
  RG1L = SKW_RAND();
  RG1L = SKW_RAND();
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND();
  RG1L = SKW_RAND();
  RG1L = SKW_MIN16(unsignedlong(byte_at(RG4P, lcon(0x6))), RG2L);
  RG1L = SKW_RAND01();
  RG5L = SKW_RAND02() + lcon(0x2);
  RG1L = SKW_RAND16(unsignedlong(RG5W));
  RG1L = SKW_RAND02() + RG4L;
  RG1L = SKW_RAND16(unsignedlong(RG5W));
  RG1L = SKW_RAND02() + 1;
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND16(signedlong(RG5W) / 4 + 1 & lcon(0xffff));
  SKW_ATTACK_CREATURE(unsignedlong(xwc_0c.peek16()), RG4L, RG2L, lcon(0x2), lcon(0x3c), parl01);
i32 SKW_1c9a_17c7(i32 eaxl, i32 edxl, i32 ebxl)
  RG1L = SKW_ABS16(RG1W);
  RG1L = SKW_ABS16(RG1W);
  RG1L = SKW_CALC_VECTOR_DIR(ddat.v1e08d8, RG2W, RG3W, RG4W);
  RG1L = signedlong(CUTX16(SKW_ABS16(RG1W)));
  RG1L = signedlong(CUTX16(SKW_ABS16(RG1W)));
i32 SKW_WOUND_CREATURE(i32 eaxl)
  RG1L = table1d607e[SKW_QUERY_GDAT_CREATURE_WORD_VALUE(RG1L, lcon(0x1)) & lcon(0xffff)].ul_00;
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  SKW_INVOKE_MESSAGE(RG1L, lcon(0x0), lcon(0x1), RG4L, parl00);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_10.peek16()));
  RG1L = SKW_1c9a_17c7(unsignedlong(ddat.s350.v1e0562.b_06), unsignedlong(ddat.s350.v1e0562.b_07), unsignedlong(ddat.s350.v1e0571));
  SKW_13e4_0360(unsignedlong(ddat.s350.v1e054c), unsignedlong(ddat.s350.v1e0562.b_06), unsignedlong(ddat.s350.v1e0562.b_07), lcon(0x13), parl05);
  SKW_DELETE_CREATURE_RECORD(unsignedlong(ddat.s350.v1e0562.b_06), lcon(0x0), lcon(0x1), unsignedlong(ddat.s350.v1e0562.b_07));
  RG1L = SKW_RAND();
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND01();
  SKW_QUEUE_NOISE_GEN1(lcon(0xf), RG2Blo, RG3W, byte_at(RG5p, lcon(0x4)), lcon(0x80), parw02, parw03, lcon(0x1));
i32 SKW_APPLY_CREATURE_POISON_RESISTANCE(i32 eaxl, i32 edxl)
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(unsignedlong(RG1W));
  RG1L = SKW_RAND02();
void SKW_1c9a_19d4(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
  SKW_ATTACK_CREATURE(RG1L, RG4L, RG2L, RG3L, lcon(0x64), parl01);
i32 SKW_1c9a_1a48(i32 eaxl, i32 edxl)
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(ddat.v1e08a8), signedlong(ddat.v1e08aa));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
i32 SKW_1c9a_1b16(i32 eaxl, i32 edxl)
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(ddat.v1e08a8), signedlong(ddat.v1e08aa));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
i32 SKW_1c9a_1bae(i32 eaxl, i32 edxl)
  return R_1BAAD(signedlong(RG1W), signedlong(RG4W));
i32 SKW_1c9a_1bd8(i32 eaxl, i32 edxl, i32 ebxl, i16 argw0, s_len8* argp_s23, c_len10x* argp_c17) // serious! from c_hexc
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(RG1L, true);
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x400));
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(RG1L, true);
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(RG1L);
  DM2_FILL_I16TABLE(RG1P16, lcon(0xffffffff), RG4L);
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(RG1L);
  DM2_ZERO_MEMORY(DOWNCAST(xxx, xp_bc), vl_9c);
  DM2_ZERO_MEMORY(ADRESSOF(i8, vba_08), lcon(0x33));
  SKW_19f0_045a(RG1L, RG4L);
  RG1L = SKW_GET_TELEPORTER_DETAIL(&ddat.v1e08b8, RG2L, RG4L);
  RG1L = signedlong(SKW_CALC_SQUARE_DISTANCE(unsignedword(ddat.s350.v1e0562.b_06), RG2W, RG3W, RG4W));
  RG1L = SKW_MAX16(signedlong(CUTX16(vl_88)), RG4L);
  RG4L = signedlong(SKW_CALC_SQUARE_DISTANCE(unsignedword(ddat.s350.v1e0562.b_06), RG2W, RG3W, unsignedword(ddat.s350.v1e0562.b_07)));
  RG1L = signedlong(CUTX16(SKW_MAX16(lcon(0x1), RG4L)));
  RG1W = SKW_CALC_SQUARE_DISTANCE(RG1W, RG2W, RG3W, RG4W);
  RG1L = SKW_RAND();
  RG1L = SKW_CALC_VECTOR_DIR(unsignedword(ddat.s350.v1e0562.b_06), RG2W, RG3W, RG4W);
  RG1L = SKW_MAX16(signedlong(vq_d4.peek16()), RG4L);
  RG1L = SKW_CALC_VECTOR_DIR(ddat.v1e08d8, RG2W, RG3W, ddat.v1e08d4) & lcon(0xffff);
  pf_02 = R_1BA1B;
  RG1L = SKW_19f0_0207(signedlong(ddat.v1e08d8), RG4L, RG2L, RG3L, pf_02);
  RG1W = SKW_CALC_SQUARE_DISTANCE(vo_f4.peek16(), RG2W, RG3W, RG4W);
  pf_03 = SKW_1c9a_1bae;
  RG1L = SKW_19f0_0207(signedlong(ddat.v1e08d8), RG4L, RG2L, RG3L, pf_03);
  RG1L = signedlong(SKW_CALC_SQUARE_DISTANCE(vo_f4.peek16(), RG2W, RG3W, RG4W));
  RG1L = signedlong(SKW_CALC_SQUARE_DISTANCE(RG1W, RG2W, RG3W, RG4W));
  RG1L = signedlong(SKW_CALC_SQUARE_DISTANCE(vo_f4.peek16(), RG2W, RG3W, RG4W));
  pf_12 = SKW_1c9a_1bae;
  RG1L = SKW_19f0_0207(signedlong(ddat.v1e08d8), RG4L, RG2L, RG3L, pf_12);
  RG1L = SKW_19f0_0891(RG1L, RG4L, RG2L, RG3L, parw04, parw05);
  RG1L = SKW_19f0_05e8(RG1L, RG4P, RG2P, RG3L, parw06, parl07, parl08);
  vo_fc.poke16(SKW_CALC_SQUARE_DISTANCE(vo_f4.peek16(), RG2W, RG3W, RG4W));
  RG1L = SKW_RAND16(unsignedlong(vo_fc.peek16()));
  RG1L = SKW_RAND16(RG1L & lcon(0xffff));
  RG1L = SKW_19f0_2165(lcon(0x0), RG4L, RG2L, RG4L, parl09, parw10, parw11);
  RG1L = SKW_CREATURE_GO_THERE(RG1L, RG4L, RG2L, RG4L, parw13, parw14);
  SKW_ADD_BACKGROUND_LIGHT_FROM_TILE(signedlong(vo_e8.peek16()), RG2L, RG3L, RG4L, parl15);
  RG1L = SKW_1c9a_1b16(signedlong(word_at(DOWNCAST(xxx, xp_b4), lcon(0x8))), RG4L);
  RG1L = signedlong(CUTX16(SKW_1c9a_1a48(lcon(0x1), RG4L)));
  RG1W = SKW_CALC_SQUARE_DISTANCE(vq_108.peek16(), RG2W, RG3W, RG4W);
  RG1L = SKW_19f0_0891(RG1L, RG4L, RG2L, RG3L, parw04, parw05);
  RG1L = SKW_19f0_050f();
  RG1L = SKW_1c9a_0006(RG1L, RG4L);
  SKW_1c9a_19d4(vl_6c, RG4L, RG2L, RG3L);
  RG1L = SKW_MAX16(RG2L, RG4L);
  SKW_1c9a_0648(RG4L);
  RG1L = SKW_CREATURE_GO_THERE(unsignedlong(vb_148), RG4L, RG2L, RG3L, parw00, parw01);
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(vl_9c, true);
  RG1W = SKW_3e74_5888();
  SKW_ALLOC_CPXHEAP_MEM(RG1W, vl_9c);
  RG1P = DOWNCAST(t_gfxdata, SKW_3e74_5817(RG3W));
  DM2_ZERO_MEMORY(RG1P, vl_9c);
  RG1P = DOWNCAST(t_gfxdata, SKW_3e74_5817(RG2W));
  SKW_1c9a_0648(signedlong(ddat.v1e08ac));
  SKW_19f0_045a(signedlong(vo_f4.peek16()), signedlong(vq_12c.peek16()));
  RG1L = SKW_CALC_VECTOR_DIR(ddat.v1e08d8, RG2W, RG3W, RG4W);
  RG1L = SKW_GET_TILE_VALUE(signedlong(CUTX16(RG2L + RG3L)), RG4L);
  RG1L = SKW_19f0_2165(lcon(0x0), RG4L, RG2L, RG3L, parl19, parw20, parw21);
  RG1L = SKW_19f0_05e8(RG1L, RG4P, RG2P, RG3L, parw16, parl17, parl18);
  RG1L = SKW_19f0_050f();
  RG1L = SKW_1c9a_0006(RG1L, RG4L);
  SKW_1c9a_19d4(vl_54, RG4L, RG2L, RG3L);
  RG1L = SKW_19f0_2813(RG1L, RG4L, RG2L, RG3L, parw22, parw23, parw24);
  SKW_ADD_BACKGROUND_LIGHT_FROM_TILE(vl_58, RG2L, RG3L, RG4L, parl25);
  RG1L = SKW_19f0_0d10(RG1L, RG4L, RG2L, RG3L, parw26, parw27);
  RG1L = signedlong(SKW_CALC_SQUARE_DISTANCE(CUTX16(vl_80), RG2W, RG3W, RG4W));
  SKW_19f0_045a(signedlong(vo_f4.peek16()), RG4L);
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(vo_f4.peek16()), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(ddat.v1e08b0));
  RG1L = SKW_19f0_050f();
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vql_120.peek16()), RG4L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG1W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG1W));
  SKW_1c9a_0648(RG4L);
  SKW_3e74_58a3(CUTX16(RG1L));
  DM2_DEALLOC_LOBIGPOOL(vl_7c);
  SKW_1c9a_0648(signedlong(xw_e4.peek16()));
void SKW___SET_CURRENT_THINKING_CREATURE_WALK_PATH(void)
  RG1L = SKW_3e74_5673(RG1L, lcon(0x0), &vw_00);
  ddat.s350.v1e07e6 = CHGCAST(i16, SKW_3e74_5817(vw_00)); // TODO: check return type vs. cast - diffs
i32 SKW_1c9a_381c(void)
  SKW___SET_CURRENT_THINKING_CREATURE_WALK_PATH();
i32 SKW_1c9a_38a8(void)
  SKW___SET_CURRENT_THINKING_CREATURE_WALK_PATH();
  SKW_COPY_MEMORY(RG1P, lcon(0x16), DOWNCAST(c_sizex16, ddat.s350.v1e0678));
  SKW_1c9a_0648(RG1L);
  RG1L = SKW_1c9a_1bd8(RG1L, RG4L, lcon(0x2), lcon(0x1), XUPCAST(s_len8, xparp01), c17p_2);
  SKW_1c9a_0648(signedlong(c17_00.c16_00.rc_02.w_06_h));
  SKW___SET_CURRENT_THINKING_CREATURE_WALK_PATH();
  RG1L = SKW_CREATURE_GO_THERE(unsignedlong(ddat.s350.v1e0673), RG4L, RG2L, lcon(0xffffffff), parw03, parw04);
  SKW_1c9a_0648(signedlong(ddat.v1e08ac));
  SKW_1c9a_0648(signedlong(c17_00.c16_00.rc_02.w_06_h));
i32 SKW_FILL_CAII_CUR_MAP(void)
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG1W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG3L);
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG4P, lcon(0x4))));
  SKW_1c9a_09db(RG4P);
  SKW_ALLOC_CAII_TO_CREATURE(RG1L, RG4L, RG2L);
void SKW_FILL_ORPHAN_CAII(void)
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = SKW_FILL_CAII_CUR_MAP();
void SKW_RESET_CAII(void)
  SKW_FILL_ORPHAN_CAII();
void SKW_1c9a_3c30(void)
  RG1L = SKW_MIN16(signedlong(RG1W), RG4L);
  ddat.v1e089c = XUPCAST(s_size22, DM2_ALLOC_FREEPOOL_MEMORY(34 * unsignedlong(RG1W)));
  ddat.v1e08cc = DM2_ALLOC_FREEPOOL_MEMORY(lcon(0x400));
  ddat.v1e08c8 = DM2_ALLOC_FREEPOOL_MEMORY(lcon(0x400));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG4P, lcon(0x4))));
i32 SKW_2066_03e0(i32 eaxl)
  RG1L = lextended(SKW_476d_030a(RG3L));
  RG1L = lextended(SKW_476d_030a(lcon(0x1)));
  SKW_38c8_0002(RG1L);
  R_148D(lcon(0x1));
  RG1L = SKW_0aaf_0067(SKW_0aaf_02f8(unsignedlong(RG1Blo), RG4L) & lcon(0xff));
i32 SKW_2066_1ec9(i32 eaxl, i32 edxl)
  RG4L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG5W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG5W));
  SKW_APPEND_RECORD_TO(RG6L, RG3L, RG3L, RG1P);
i32 SKW_2066_1f37(i32 eaxl, i16* ebxpw, i32 edxl)
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG3W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
void SKW_ARRANGE_DUNGEON(void)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG5W));
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = SKW_GET_TILE_RECORD_LINK(RG2L, RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1L = SKW_2066_1f37(unsignedlong(xwc_14.peek16()), RG2P16, RG4L);
  RG1L = SKW_2066_1f37(unsignedlong(RG5W), RG2P16, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(xwc_14.peek16()));
  RG1L = SKW_2066_1ec9(SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG5W)) & lcon(0xffff), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG3L);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = (SKW_GET_TILE_VALUE(signedlong(RG1W), RG4L) & lcon(0xff)) >> bcon(0x5) & lcon(0xff);
  RG1P = SKW_GET_ADDRESS_OF_TILE_RECORD(signedlong(RG1W), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG5W));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG4P, lcon(0x4))));
  RG1L = SKW_RAND02() << bcon(0xe);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG2W));
  RG1L = SKW_RAND02();
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG2W));
  SKW_2066_1ea3(RG1L, RG2L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG5W));
  RG1L = SKW_LOCATE_OTHER_LEVEL(RG1L, &vw_10, RG3P16, lcon(0x1), NULL);
  SKW_2066_1ea3(signedlong(vw_10), RG2L, signedlong(vw_0c));
void SKW_2066_2498(void)
  i8 vba_00[0xc8]; // text in the beginning, struct for SKW_QUERY_MESSAGE_TEXT
  SKW_FILL_STR(UPCAST(t_text, ADRESSOF(i8, vba_00)), lcon(0x31), lcon(0x1), lcon(0x40));
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(RG1L);
  SKW_CHANGE_CURRENT_MAP_TO(lcon(0x0));
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(RG1W), RG4L);
  RG1L = SKW_MAX16(signedlong(RG1W), RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG1W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  SKW_QUERY_MESSAGE_TEXT(RG1T, RG2L, RG4L);
i32 SKW_READ_DUNGEON_STRUCTURE(i32 eaxl)
  RG1L = SKW_SKLOAD_READ(ADRESSOF(i8, u24_04.barr_00), RG4W) ? 1 : 0;
  RG1L = DM2_FILE_SEEK(signedlong(ddat.filehandle1), ddat.savegamel1) ? 1 : 0;
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(lcon(0x2c));
  RG1L = SKW_SKLOAD_READ(DOWNCAST(s_sgwords, ddat.savegamep4), lcon(0x2c)) ? 1 : 0;
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(RG1L);
  RG1L = SKW_SKLOAD_READ(ddat.v1e03c8, RG4W) ? 1 : 0;
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(2 * signedlong(vq_10.peek16()));
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(2 * unsignedlong(RG61W));
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(2 * unsignedlong(ddat.savegamep4->warr_00[5]));
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(RG1L);
  RG1L = SKW_SKLOAD_READ(ddat.v1e03d8, RG4W) ? 1 : 0;
  RG1L = SKW_SKLOAD_READ(DOWNCAST(i16, ddat.v1e038c), RG4W) ? 1 : 0;
  RG1L = SKW_SKLOAD_READ(ddat.v1e03d0, RG4W) ? 1 : 0;
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(unsignedlong(ddat.savegamep4->warr_00[1]));
  RG1L = SKW_MIN16(signedlong(RG1W), RG4L);
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(unsignedlong(RG61W) * RG4L);
  RG1L = SKW_SKLOAD_READ(RG2P, RG4W) ? 1 : 0;
  RG1L = SKW_SKLOAD_READ(ddat.v1e03e0, RG4W) ? 1 : 0;
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(RG1L);
  RG1L = 21 * signedlong(CUTX16(SKW_BETWEEN_VALUE16(lcon(0x0), lcon(0x17), signedlong(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0x3), lcon(0xb), lcon(0x0), lcon(0x0))))));
  SKW_3a15_0002();
  SKW_3df7_0037(RG1L);
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(lcon(0x3f));
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(lcon(0x7e));
  SKW_ARRANGE_DUNGEON();
  SKW_2066_2498();
void SKW_2066_33c4(t_text* eaxtp, i32 edxl)
  SKW_SK_STRCPY(RG1T, CHGCAST(t_text, v1d10e4));
i32 SKW_2066_33e7(void)
  SKW__OPEN_DIALOG_PANEL(lcon(0x81), RG4L);
  SKW_2066_33c4(ddat.v1e0900.c26_02.tarr_00, RG4L);
  SKW_COPY_MEMORY(RG1P, RG2L, DOWNCAST(c_hex2a, &ddat.v1e0900));
  SKW_2066_398a(RG1L);
  SKW_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
  SKW_2066_3820(RG1T, RG4L);
  SKW_EVENT_LOOP();
  SKW_2066_33c4(RG3T, RG4L);
  SKW_2066_3820(RG1T, RG4L);
  SKW_QUERY_EXPANDED_RECT(RG1W, RG4R);
  SKW_QUERY_TOPLEFT_OF_RECT(ddat.mouse_unk09, &vw_08, &vw_0c);
  RG1L = SKW_MIN16(signedlong(CUTX16(RG4L / RG2L)), lcon(0xa));
  SKW_SK_STRCPY(ddat.v1e0900.c26_02.tarr_00, UPCAST(t_text, RG1P + lcon(0x2)));
  SKW_2066_33c4(ddat.v1e0900.c26_02.tarr_00, RG4L);
  SKW_2066_398a(RG1L);
  SKW_2066_3820(RG1T, RG4L);
  SKW_2066_3820(RG3T, RG4L);
  RG1L = SKW_SK_STRLEN(RG3T);
  RG1W = SKW_476d_05b6(ddat.v1e051e);
  SKW_2066_3820(RG1T, RG4L);
  SKW_2066_37f2();
void SKW__OPEN_DIALOG_PANEL(i32 eaxl, i32 edxl)
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x1a4));
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x1a), lcon(0x0), RG3T, RG4L));
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x1a), lcon(0x1), RG3T, RG4L));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x1a), lcon(0x0), CUTLX8(RG4L)));
  RG1L = SKW_CALC_IMAGE_BYTE_LENGTH(UPCAST(t_gfxdata, RG1P));
  RG3P = DOWNCAST(t_colidx, SKW_QUERY_GDAT_IMAGE_LOCALPAL(lcon(0x1a), CUTLX8(RG2L), CUTLX8(RG4L)));
  RG1R = SKW_QUERY_EXPANDED_RECT(lcon(0x4), RG4R);
  SKW_DRAW_DIALOGUE_PARTS_PICT(UPCAST(t_gfxdata, RG61p), lcon(0xffffffff), UPCAST(t_colidx, RG3P), RG4R);
  SKW_FREE_PICT_ENTRY(RG1P);
  SKW_FIRE_FADE_SCREEN(lcon(0x1));
  SKW_DRAW_VP_RC_STR(lcon(0x1c2), CHGCAST(t_text, strVersionNumber), RG4W);
  SKW_DRAW_VP_RC_STR(lcon(0x1d2), UPCAST(t_text, DOWNCAST(t_text, tp_58)), RG4W);
  SKW_DRAW_VP_RC_STR(lcon(0x1d3), RG2T, RG4W);
  SKW_2066_38d9();
  RG1L = SKW_1031_0675(lcon(0x4));
  SKW_FIRE_SHOW_MOUSE_CURSOR();
void SKW_2066_37f2(void)
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  RG1L = SKW_1031_06a5();
  DM2_DEALLOC_LOBIGPOOL(lcon(0x1a4));
void SKW_2066_3820(t_text* eaxtp, i32 edxl)
  SKW_QUERY_EXPANDED_RECT(lcon(0x1c5), RG4R);
  parpcarr02 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(lcon(0x1a), lcon(0x0), lcon(0x81));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x1a), lcon(0x0), lcon(0x81)));
  SKW_DRAW_DIALOGUE_PICT(UPCAST(t_gfxdata, RG1P), RG2R, RG3W, UPCAST(t_gfxdata, RG61p), parw00, lcon(0xffff), parpcarr02);
  SKW_DRAW_VP_STR(rc_00.w_00_x, RG2W, RG5t, RG4W);
  RG1L = SKW_SK_STRLEN(RG5t);
  SKW_FIRE_FILL_BACKBUFF_RECT(RG1R, RG4Blo);
  SKW_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN();
void SKW_2066_38d9(void)
  RG1L = DM2_FILE_OPEN(SKW_FORMAT_SKSTR(ddat.gdatfilename5, UPCAST(t_text, NULL)));
  SKW_SK_STRCPY(RG1T, v1d5c31);
  DM2_FILE_READ(RG3W, RG2P, lcon(0x2a));
  RG1L = DM2_FILE_CLOSE(RG3W);
i32 SKW_2066_398a(i32 eaxl)
  SKW_QUERY_EXPANDED_RECT(lcon(0x1c3), RG4R);
  SKW_DRAW_VP_STR(rc_04.w_00_x, RG2W, RG3T, RG6w);
void SKW_2405_0009(void)
  ddat.savegamewpc.xp_02 = DOWNCAST(t_gfxdata, SKW_ALLOC_PICT_BUFF(ddat.v1d271a, FREEPOOL, lcon(0x4), ddat.v1d271c));
t_text* SKW_QUERY_GDAT_ITEM_NAME(i32 eaxl, i32 edxl)
  return SKW_FORMAT_SKSTR(SKW_QUERY_GDAT_TEXT(RG1L, lcon(0x18), tarr_00, RG4L), ddat.v1e0930);
t_text* SKW_GET_ITEM_NAME(i16 eaxw)
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG4L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  return SKW_QUERY_GDAT_ITEM_NAME(unsignedlong(RG3Blo), RG4L);
void SKW_2405_00ec(i32 eaxl, c_rect* edxrp)
  SKW_QUERY_BLIT_RECT(NULL, RG2W, RG3P16, RG4R, parpw00, lcon(0xffffffff));
void SKW_2405_011f(i32 eaxl, c_rect* edxrp)
  SKW_2405_00ec(RG1L, RG4R);
  SKW_INFLATE_RECT(RG3R, RG4W, RG4W); // attention, two times RG4W
i32 SKW_2405_014a(i32 eaxl, i32 ebxl, i32 edxl)
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG71L, lcon(0x6));
  RG1L = SKW_IS_ITEM_FIT_FOR_EQUIP(RG71L, RG2L, RG4L);
  RG1L = SKW_RAND16(unsignedlong(RG5w));
  RG1L = SKW_ADD_ITEM_CHARGE(RG2L, RG4L);
  RG1L = SKW_GET_MAX_CHARGE(RG2L) & lcon(0xffff);
  RG1L = SKW_ADD_ITEM_CHARGE(RG62L, RG4L);
  RG1L = SKW_GET_MAX_CHARGE(RG62L) & lcon(0xffff);
void SKW_DRAW_ITEM_ICON(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl, i32 argl0)
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG3L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG3L);
  RG1L = SKW_2405_014a(RG3L, RG2L, RG4L);
  RG1L = SKW_2405_014a(RG3L, RG2L, RG4L);
  SKW_QUERY_EXPANDED_RECT(RG1W, RG4R);
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x7), lcon(0x0), lcon(0x0)));
  parpcarr06 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(lcon(0x7), lcon(0x0), lcon(0x0));
  SKW_DRAW_DIALOGUE_PICT(UPCAST(t_gfxdata, RG61p), RG2R, RG3W, UPCAST(t_gfxdata, RG4P), parw04, lcon(0xffff), parpcarr06);
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x1), lcon(0x0), lcon(0x2)));
  parpcarr03 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(lcon(0x1), lcon(0x0), lcon(0x2));
  SKW_DRAW_ICON_PICT_BUFF(UPCAST(t_gfxdata, RG62p), RG2R, RG3L, &ddat.v1d69d4, parw00, lcon(0xffff), lcon(0x0), parpcarr03);
  SKW_2405_011f(RG1L, RG4R);
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x1), CUTLX8(RG2L), lcon(0x2)));
  RG1P = DOWNCAST(t_colidx, SKW_QUERY_GDAT_IMAGE_LOCALPAL(lcon(0x1), CUTLX8(RG2L), lcon(0x2)));
  SKW_DRAW_DIALOGUE_PARTS_PICT(UPCAST(t_gfxdata, RG63p), RG2W, UPCAST(t_colidx, RG3P), RG4R);
  SKW_DRAW_ICON_PICT_BUFF(UPCAST(t_gfxdata, RG63p), RG2R, RG3L, &ddat.v1d69d4, lcon(0x0), lcon(0xc), lcon(0x0), parpcarr10);
  SKW_DRAW_ICON_PICT_ENTRY(unsignedlong(vb_14), unsignedlong(vb_18), &ddat.v1d69d4, unsignedlong(vb_10), parw11, lcon(0xc));
  SKW_DRAW_STATIC_PIC(unsignedlong(vb_14), unsignedlong(vb_18), signedlong(xw_0c.peek16()), unsignedlong(vb_10), parl13);
void SKW_DRAW_ITEM_IN_HAND(xxx* xeaxp)
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(unsignedlong(word_at(RG3P)));
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(unsignedlong(word_at(RG3P)));
  RG1L = SKW_2405_014a(unsignedlong(word_at(RG3P)), lcon(0x1), lcon(0xffffffff));
  RG5p = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(CUTLX8(RG1L), CUTLX8(RG7L), CUTLX8(RG6L)));
  SKW_COPY_MEMORY(DOWNCAST(t_colidx, SKW_QUERY_GDAT_IMAGE_LOCALPAL(CUTLX8(vl_04), CUTLX8(RG7L), CUTLX8(RG6L))), PAL16, DOWNCAST(t_colidx, pcarr_00));
  RG1R = SKW_ALLOC_TEMP_ORIGIN_RECT(RG1W, ddat.v1d271c);
  SKW_FIRE_BLIT_PICTURE(UPCAST(t_gfxdata, RG5p), RG2R, RG3W, UPCAST(t_gfxdata, RG4P), lcon(0x0), parw01, parw02, lcon(0xffff), lcon(0x0), lcon(0x4), lcon(0x4), NULL);
xxx* SKW_DRAW_ITEM_ON_WOOD_PANEL(i32 eaxl, c_hex18* ebxp_c30, i32 edxl)
  RG1L = SKW_IS_ITEM_HAND_ACTIVABLE(RG1L, RG2L, RG4L);
  RG1W = SKW_3e74_5888();
  SKW_ALLOC_NEW_PICT(CUTX16(RG1L), CUTX16(RG2L), lcon(0x8), CUTX16(RG4L));
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG5l);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG5l);
  RG1L = SKW_2405_014a(RG5l, RG2L, RG4L);
  SKW_QUERY_GDAT_SUMMARY_IMAGE(UPCAST(c_image, ADRESSOF(c_image, &c37_00)), unsignedlong(vb_154), RG3L, unsignedlong(vb_158));
  SKW_QUERY_PICST_IT(UPCAST(c_image, ADRESSOF(c_image, &c37_00)));
  RG52p = DOWNCAST(t_gfxdata, SKW_0b36_00c3(RG1L, UPCAST(c_hex18, RG4P)));
  SKW_FILL_ENTIRE_PICT(UPCAST(t_gfxdata, RG7p), RG2Blo);
  RG1R = SKW_ALLOC_TEMP_RECT(ddat.v1d2722, ddat.v1d2724, ddat.v1d271a, ddat.v1d271c);
  SKW_DRAW_DIALOGUE_PICT(UPCAST(t_gfxdata, DOWNCAST(t_gfxdata, SKW_QUERY_PICT_BITS(UPCAST(c_hex18, ADRESSOF(c_image, &c37_00))))), RG2R, lcon(0x0), UPCAST(t_gfxdata, RG7p), lcon(0x0), lcon(0xffff), vcarr_13c);
  R_B012(c37_00.carr_3a, lcon(0x0), RG3L, RG3L, &c37_00.w_38);
  RG1P = DOWNCAST(t_gfxdata, SKW_0b36_00c3(signedlong(xw_150.peek16()), UPCAST(c_hex18, RG61p)));
  RG1R = SKW_ALLOC_TEMP_ORIGIN_RECT(RG1W, RG4W);
  SKW_DRAW_DIALOGUE_PICT(UPCAST(t_gfxdata, DOWNCAST(t_gfxdata, SKW_QUERY_PICT_BITS(UPCAST(c_hex18, ADRESSOF(c_image, &c37_00))))), RG2R, lcon(0x0), UPCAST(t_gfxdata, RG52p), lcon(0x0), lcon(0xc), c37_00.carr_3a);
void SKW_2481_0002(void)
  SKW_FIRE_SELECT_PALETTE_SET(RG1Blo);
  R_C470(ddat.v1e0954, lcon(0x2), lcon(0xffffffff), XUPCAST(t_colidx, ddat.v1d66f4));
  RG1R = SKW_ALLOC_TEMP_ORIGIN_RECT(RG1W, ddat.mouse_unk0d);
  SKW_FIRE_BLIT_PICTURE(UPCAST(t_gfxdata, ddat.v1e0954), RG2R, RG3W, UPCAST(t_gfxdata, RG4P), lcon(0x0), parw01, parw02, lcon(0xffff), lcon(0x0), lcon(0x8), lcon(0x8), NULL);
  SKW_FIRE_SELECT_PALETTE_SET(lcon(0x1));
void SKW_SHOW_MENU_SCREEN(void)
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x5), lcon(0x1), lcon(0x0)));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x5), lcon(0x4), lcon(0x0)));
  RG1P = SKW_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0x5), lcon(0x7), lcon(0x4), lcon(0x0));
  DM2_GRAPHICS_DATA_CLOSE();
  RG1L = SKW_1031_0541(lcon(0x0));
  SKW_2481_0002();
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_1031_098e();
  SKW_EVENT_LOOP();
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_FREE_PICT_ENTRY(ddat.v1e0954);
  DM2_DEALLOC_BIGPOOL_STRUCT_BEFORE(XUPCAST(s_malloctail, ddat.v1e0954));
  SKW_FREE_PICT_ENTRY(ddat.v1e0950);
void SKW_SHOW_CREDITS(void)
  SKW_FIRE_SELECT_PALETTE_SET(RG1Blo);
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  R_C470(RG1P, RG4L, RG2L, XUPCAST(t_colidx, ddat.v1e0958));
  SKW_FIRE_SELECT_PALETTE_SET(RG1Blo);
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_1031_0541(lcon(0x1));
  SKW_EVENT_LOOP();
  SKW_SLEEP_SEVERAL_TIME(lcon(0x1));
  SKW_1031_0541(lcon(0x0));
  SKW_FIRE_HIDE_MOUSE_CURSOR();
i32 SKW_RECALC_LIGHT_LEVEL(void)
  DM2_ZERO_MEMORY(ADRESSOF(i16, vwa_00), lcon(0x12));
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(ddat.savegamewpc.w_00), lcon(0x0));
  RG1L = SKW_ADD_ITEM_CHARGE(unsignedlong(ddat.savegamewpc.w_00), lcon(0x0));
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG2L, RG4L);
  RG1L = SKW_ADD_ITEM_CHARGE(RG2L, RG4L);
  RG1L = signedlong(CUTX16(SKW_BETWEEN_VALUE16(lcon(0x0), lcon(0x5), RG4L)));
  RG1L = SKW_MAX16(lcon(0x0), RG4L);
  RG1L = SKW_BETWEEN_VALUE16(lcon(0x0), lcon(0x5), signedlong(ddat.v1e0286));
void SKW_ADD_BACKGROUND_LIGHT_FROM_TILE(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl, i32 argl0)
  SKW_SUMMARIZE_STONE_ROOM(UPCAST(s_8wordarray, RG1P), RG2L, RG3L, RG4L);
  RG1L = signedlong(table1d6712[signedlong(CUTX16(SKW_BETWEEN_VALUE16(lcon(0x0), RG2L, RG4L)))]);
  RG1L = SKW_GET_TELEPORTER_DETAIL(&c12_1c, RG2L, RG4L);
  RG1L = signedlong(table1d6712[signedlong(CUTX16(SKW_BETWEEN_VALUE16(lcon(0x0), lcon(0x5), RG4L)))]);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vql_04.peek16()), unsignedlong(vql_00.peek16()));
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG1W), lcon(0xf8));
  RG1L = SKW_MAX16(lcon(0x3), RG4L);
  RG6L = SKW_MAX16(lcon(0x2), RG4L);
  RG5L = SKW_MAX16(lcon(0x2), RG4L);
void SKW_BURN_PLAYER_LIGHTING_ITEMS(void)
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG2L, RG4L);
  RG1L = SKW_ADD_ITEM_CHARGE(RG2L, RG4L);
  RG1L = SKW_ADD_ITEM_CHARGE(RG2L, RG4L);
  SKW_SET_ITEM_IMPORTANCE(RG1L, RG4L);
  RG1L = SKW_RECALC_LIGHT_LEVEL();
void SKW_24a5_0732(i32 eaxl, t_text* ebxtp, i32 edxl)
  RG1L = SKW_QUERY_STR_METRICS(RG3T, RG2P16, RG4P16) ? 1 : 0;
  SKW_DRAW_VP_STR(RG1W, RG2W, tarr_00, RG4W);
void SKW_DRAW_SCROLL_TEXT(i32 eaxl)
  SKW_QUERY_MESSAGE_TEXT(tarr_c8, lcon(0xffff8002), RG4L);
  SKW_DRAW_STATIC_PIC(lcon(0x7), lcon(0x1), lcon(0x1ee), lcon(0x0), parl00);
  SKW_DRAW_STATIC_PIC(lcon(0x12), RG2L, RG3L, lcon(0x0), parl01);
  SKW_QUERY_TOPLEFT_OF_RECT(lcon(0x230), &vw_1a0, &vw_19c);
  SKW_QUERY_EXPANDED_RECT(lcon(0x1ee), RG4R);
  SKW_3929_04e2(tarr_c8, &vw_198, signedlong(rc_190.w_04_w), tarr_00);
  SKW_24a5_0732(signedlong(vw_1a0), tarr_00, signedlong(vw_19c));
i32 SKW_PUT_OBJECT_INTO_CONTAINER(i32 eaxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  SKW_APPEND_RECORD_TO(RG1L, RG2L, RG3L, RG4P);
void SKW__CHECK_ROOM_FOR_CONTAINER(i32 eaxl, xxx* xedxp)
  SKW_CUT_RECORD_FROM(RG1L, lcon(0xffffffff), RG3L, RG4P);
void SKW_MONEY_BOX_SURVEY(i32 eaxl)
  SKW_COUNT_BY_COIN_TYPES(unsignedlong(xwc_00.peek16()), vwa_04);
  RG1P = DOWNCAST(t_text, SKW_SK_LTOA10(RG7l, RG4T));
  SKW_DRAW_VP_RC_STR(lcon(0x239), RG1T, RG4W);
  RG1L = SKW_GET_ITEM_ORDER_IN_CONTAINER(unsignedlong(xwc_00.peek16()), RG4L);
  RG1P = DOWNCAST(t_text, SKW_SK_LTOA10(RG2L, RG4T));
  SKW_DRAW_VP_RC_STR(RG1W, RG2T, RG4W);
void SKW_DRAW_CONTAINER_SURVEY(xxx* xeaxp)
  SKW_DRAW_ITEM_ICON(RG5l, lcon(0x0), lcon(0x0), RG4L, parl00);
  RG1L = SKW_GET_NEXT_RECORD_LINK(RG5l);
void SKW_DRAW_ITEM_STATS_BAR(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl, i16 argw0)
  RG1R = SKW_QUERY_EXPANDED_RECT(RG5W, RG4R);
  RG1L = SKW_24a5_0e82(signedlong(RG6w), RG2L, lcon(0x0), RG5L, parw00, parl01);
  SKW_DRAW_STRONG_TEXT(UPCAST(t_gfxdata, RG1P), RG2W, RG3W, word_at(RG1P, -lcon(0x4)), parw02, CUTX16(parl03), par_tp04);
  SKW_DRAW_STRONG_TEXT(UPCAST(t_gfxdata, RG1P), RG2W, RG3W, word_at(RG1P, -lcon(0x4)), parw05, CUTX16(parl06), par_tp07);
  SKW_DRAW_STRONG_TEXT(ddat.bitmapptr, RG2W, RG3W, word_at(ddat.bitmapptr, -lcon(0x4)), parw08, CUTX16(parl09), par_tp10);
i32 SKW_DRAW_ITEM_SURVEY(i32 eaxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG51l);
  SKW_DRAW_GUIDED_STR(CHGCAST(t_text, v1d10f0));
  SKW_DRAW_SCROLL_TEXT(RG51l);
  SKW_DRAW_STATIC_PIC(lcon(0x7), lcon(0x1), lcon(0x1ee), lcon(0x0), parl00);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG51l) & lcon(0xff);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG51l);
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(CUTLX8(RG55l), lcon(0x1), lcon(0x11), CUTLX8(vla_80[0x1])) ? 1 : 0;
  SKW_DRAW_STATIC_PIC(RG55l, lcon(0x11), lcon(0x1ee), vla_80[0x1], parl01);
  SKW_DRAW_STATIC_PIC(lcon(0x7), lcon(0x5), lcon(0x1f8), lcon(0x0), parl02);
  RG1T = SKW_GET_ITEM_NAME(RG52l);
  SKW_DRAW_VP_RC_STR(lcon(0x1fa), RG1T, RG4W);
  SKW_DRAW_ITEM_ICON(RG52l, lcon(0x0), lcon(0x0), lcon(0x2e), parl03);
  RG1L = SKW_QUERY_ITEM_WEIGHT(RG52l);
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x7), RG2L, RG3T, RG4L));
  SKW_DRAW_GUIDED_STR(RG1T);
  RG1L = SKW_IS_CONTAINER_CHEST(RG52l);
  SKW_DRAW_CONTAINER_SURVEY(RG7p);
  RG1L = SKW_IS_CONTAINER_MONEYBOX(RG52l);
  SKW_MONEY_BOX_SURVEY(RG52l);
  RG1L = SKW_2759_0155(RG52l);
  RG1L = SKW_ADD_ITEM_CHARGE(RG52l, RG4L);
  RG1L = signedlong(CUTX16(SKW_GET_MAX_CHARGE(RG52l)));
  SKW_DRAW_ITEM_STATS_BAR(RG1L, RG2L, RG3L, RG4L, lcon(0x4));
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG53l, RG4L);
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG53l, RG4L);
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG6W), lcon(0xb));
  SKW_DRAW_ITEM_STATS_BAR(RG1L, RG2L, RG3L, RG4L, lcon(0x5));
i32 SKW_24a5_0e82(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl, i16 argw0, i32 argl1)
  RG1R = SKW_SCALE_RECT(RG51w, RG2W, RG3W, RG4R);
  SKW_COPY_RECT(&rc_08, &rc_10);
  SKW_QUERY_EXPANDED_RECT(RG51w, RG4R);
  SKW_COPY_RECT(&rc_00, &rc_10);
  SKW_FIRE_FILL_BACKBUFF_RECT(RG1R, RG4Blo);
  RG1L = SKW_FIRE_FILL_BACKBUFF_RECT(RG1R, RG4Blo);
  RG1L = SKW_FIRE_FILL_BACKBUFF_RECT(RG1R, RG4Blo);
void SKW_DRAW_FOOD_WATER_POISON_PANEL(void)
  SKW_DRAW_STATIC_PIC(lcon(0x7), lcon(0x1), lcon(0x1ee), lcon(0x0), parl00);
  SKW_24a5_0e82(signedlong(word_at(RG5p, lcon(0x44))), lcon(0x5), lcon(0xfffffc00), lcon(0x1f0), lcon(0x800), parl02);
  SKW_24a5_0e82(signedlong(word_at(RG5p, lcon(0x46))), lcon(0xe), lcon(0xfffffc00), lcon(0x1f1), lcon(0x800), parl04);
  SKW_DRAW_STATIC_PIC(lcon(0x7), lcon(0x6), lcon(0x1f4), lcon(0x0), parl05);
  SKW_DRAW_STATIC_PIC(RG2L, RG2L, RG3L, lcon(0x0), parl06);
  SKW_24a5_0e82(signedlong(word_at(RG5p, lcon(0x48))), lcon(0x8), lcon(0x0), lcon(0x1f3), lcon(0xc00), lcon(0x0));
  SKW_DRAW_STATIC_PIC(lcon(0x7), lcon(0x8), lcon(0x1f6), lcon(0x0), parl09);
void SKW_DRAW_CRYOCELL_LEVER(i32 eaxl)
  SKW_DRAW_STATIC_PIC(lcon(0x9), RG2L, RG3L, lcon(0x5b), parl00);
  SKW_CHANGE_VIEWPORT_TO_INVENTORY(lcon(0x0));
  SKW_QUEUE_NOISE_GEN1(lcon(0x9), lcon(0xfb), lcon(0xff), lcon(0x5b), lcon(0xc8), parw02, parw03, lcon(0x0));
i32 SKW_IS_MISCITEM_DRINK_WATER(i32 eaxl)
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG2L, RG4L);
  RG1L = SKW_ADD_ITEM_CHARGE(RG2L, RG4L);
  SKW_ADD_ITEM_CHARGE(RG2L, lcon(0xffffffff));
  SKW_TAKE_OBJECT(RG2L, lcon(0x0));
void SKW_PLAYER_CONSUME_OBJECT(i32 eaxl, i32 ebxl, i32 edxl)
  SKW_FIRE_MOUSE_SET_CAPTURE();
  SKW_443c_08ab(&ddat.v1e00b6, &ddat.v1e00b2, &ddat.v1e00b4);
  SKW_FIRE_MOUSE_RELEASE_CAPTURE();
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  RG1L = SKW_QUERY_GDAT_FOOD_VALUE_FROM_RECORD(RG2L);
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  RG1L = SKW_REMOVE_OBJECT_FROM_HAND();
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_DRAW_STATIC_PIC(lcon(0x7), RG2L, RG3L, lcon(0x0), parl00);
  SKW_CHANGE_VIEWPORT_TO_INVENTORY(lcon(0x0));
  SKW_SLEEP_SEVERAL_TIME(RG1W);
  RG1L = SKW_MIN16(signedlong(RG1W), lcon(0x800));
  SKW_PROCESS_ITEM_BONUS(signedlong(vql_08.peek16()), vl_14, lcon(0x2), RG66L);
  SKW_DEALLOC_RECORD(RG66L);
  RG1L = SKW_IS_MISCITEM_DRINK_WATER(RG2L);
  RG1L = SKW_MIN16(signedlong(RG1W), lcon(0x800));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG2L);
  SKW_2c1d_1bb5(vl_10, RG4L);
  RG1L = SKW_MIN16(signedlong(mkw(word_at(RG5p, lcon(0x3c)) - word_at(RG5p, lcon(0x3a)))), RG4L);
  SKW_2c1d_0009(RG1L, lcon(0x2), RG2L, RG4L);
  RG1L = SKW_MIN16(lcon(0x384), RG4L);
  RG1L = signedlong(CUTX16(SKW_MAX16(signedlong(word_at(RG5p, lcon(0x3e))), RG4L)));
  RG3L = SKW_MAX16(lcon(0x1), RG4L);
  RG1L = SKW_RAND();
  RG1L = SKW_MIN16(signedlong(RG1W), lcon(0x800));
  SKW_2c1d_0300(RG1P, RG2L, RG4L);
  RG1L = SKW_REMOVE_OBJECT_FROM_HAND();
  SKW_DEALLOC_RECORD(unsignedlong(xwc_04.peek16()));
  RG1L = SKW_ALLOC_NEW_RECORD(lcon(0x8));
  SKW_SET_ITEMTYPE(RG2L, lcon(0x14));
  SKW_TAKE_OBJECT(RG2L, lcon(0x0));
  SKW_QUEUE_NOISE_GEN2(lcon(0x16), lcon(0x83), lcon(0xfe), byte_at(RG5p, lcon(0x101)), parw01, parw02, lcon(0x0), lcon(0x96), lcon(0x80));
  SKW_CALC_PLAYER_WEIGHT(unsignedlong(vql_08.peek16()));
void SKW_24a5_1532(void)
  SKW_DRAW_STATIC_PIC(lcon(0x7), lcon(0x1), RG3L, lcon(0x0), parl00);
  SKW_QUERY_TOPLEFT_OF_RECT(lcon(0x22d), &vw_8c, &vw_84);
  SKW_QUERY_TOPLEFT_OF_RECT(lcon(0x22f), &vw_80, &vw_84);
  RG1L = SKW_QUERY_PLAYER_SKILL_LV(signedlong(vq_98.peek16()), lcon(0x1), RG51L);
  RG1L = signedlong(CUTX16(SKW_MIN16(lcon(0x10), RG4L)));
  SKW_QUERY_GDAT_TEXT(lcon(0x7), unsignedlong(table1d6712[RG1L + 4]), tarr_00, lcon(0x0)); // index shifted, was 0x1d6716
  SKW_DRAW_VP_STR(vw_8c, unsignedword(ddat.paletteptr1[RG1L]), RG3T, vw_84);
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x7), RG2L, RG3T, lcon(0x0)));
  SKW_DRAW_VP_STR(vw_8c, unsignedword(ddat.paletteptr1[lcon(0xd)]), RG1T, vw_84);
  RG1L = SKW_GET_PLAYER_ABILITY(RG61p, RG2L, RG4L);
  RG1L = SKW_GET_PLAYER_ABILITY(RG61p, RG2L, RG4L);
  RG1P = DOWNCAST(t_text, SKW_FMT_NUM(xwc_94.peek16(), lcon(0x3), lcon(0x1)));
  SKW_DRAW_VP_STR(vw_80, unsignedword(byte_at(RG4P)), RG1T, vw_84);
  SKW_SK_STRCPY(tarr_00, CHGCAST(t_text, v1d10f2));
  RG1P = DOWNCAST(t_text, SKW_SK_STRCAT(tarr_00, SKW_FMT_NUM(xwc_88.peek16(), lcon(0x3), lcon(0x1))));
  SKW_DRAW_VP_STR(RG1W, RG2W, tarr_00, RG4W);
void SKW_CLICK_INVENTORY_EYE(void)
  SKW_FIRE_MOUSE_SET_CAPTURE();
  SKW_443c_08ab(&ddat.v1e00b6, &ddat.v1e00b2, &ddat.v1e00b4);
  SKW_1031_098e();
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_SLEEP_SEVERAL_TIME(lcon(0x8));
  SKW_FIRE_MOUSE_RELEASE_CAPTURE();
i32 SKW_24a5_1798(i32 eaxl)
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_REFRESH_PLAYER_STAT_DISP(RG1L);
  return R_107B0();
  SKW_443c_040e();
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  return R_107B0();
  SKW_29ee_000f();
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_INIT_BACKBUFF();
  return R_107B0();
  SKW_FIRE_FILL_HALFTONE_RECTI(RG1W, RG4W);
  DM2_ZERO_MEMORY(ddat.v1e096c, lcon(0x4));
  SKW_REFRESH_PLAYER_STAT_DISP(RG1L);
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  return SKW_1031_0541(lcon(0x8));
t_text* SKW_FORMAT_SKSTR(t_text* eaxtp, t_text* edxtp)
  SKW_SK_STRCPY(tarr_80, tarr_104);
  SKW_QUERY_GDAT_TEXT(RG1L, RG2L, RG3T, vl_100);
  SKW_SK_LTOA10(RG1L, UPCAST(t_text, RG61p));
  SKW_FORMAT_SKSTR(RG1T, RG4T);
  SKW_SK_STRCAT(RG7t, tarr_00);
  RG1L = SKW_SK_STRLEN(RG7t);
t_text* SKW_QUERY_GDAT_TEXT(i32 eaxl, i32 ebxl, t_text* ecxtp, i32 edxl)
  RG1P = SKW_QUERY_GDAT_ENTRY_DATA_BUFF(CUTLX8(RG71L), CUTLX8(RG2L), CUTLX8(RG3L), CUTLX8(RG4L));
  RG1L = SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(CUTLX8(RG71L), lcon(0x5), CUTLX8(RG51l), CUTLX8(vl_04));
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(RG72L);
  SKW_COPY_MEMORY(DOWNCAST(t_text, tp_00), RG2L, RG4P);
  SKW_FORMAT_SKSTR(UPCAST(t_text, DOWNCAST(t_text, tp_08)), RG6t);
  DM2_DEALLOC_LOBIGPOOL(signedlong(xw_0c.peek16()));
void SKW_2676_006a(i32 eaxl)
  SKW_MARK_DYN_LOAD(RG1L);
void SKW_2676_008f(i32 eaxl, i32 edxl)
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_MARK_DYN_LOAD(RG1L);
void SKW_2676_00d0(i32 eaxl, i32 ebxl, i32 edxl)
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_MARK_DYN_LOAD(RG1L);
void SKW_LOAD_LOCALLEVEL_DYN(void)
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x960));
  RG51p = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0xfa), true);
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0xfa), true);
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0xfa), true);
  SKW_MARK_DYN_LOAD(lcon(0x1ff02ff));
  SKW_MARK_DYN_LOAD(lcon(0x18ff02ff));
  SKW_MARK_DYN_LOAD(lcon(0x7ff02ff));
  SKW_MARK_DYN_LOAD(lcon(0xd0002ff));
  SKW_MARK_DYN_LOAD(lcon(0xd2f02ff));
  SKW_MARK_DYN_LOAD(lcon(0xd7e02ff));
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_MARK_DYN_LOAD(lcon(0x10ff02ff));
  SKW_MARK_DYN_LOAD(lcon(0x15ff02ff));
  SKW_MARK_DYN_LOAD(lcon(0x30002ff));
  SKW_MARK_DYN_LOAD(lcon(0x8fe02ff));
  SKW_MARK_DYN_LOAD(lcon(0x16fe02ff));
  SKW_MARK_DYN_LOAD(lcon(0x9fe02ff));
  SKW_MARK_DYN_LOAD(lcon(0xafe02ff));
  SKW_MARK_DYN_LOAD(lcon(0xfff08fb));
  SKW_MARK_DYN_LOAD(lcon(0xfff07fc));
  SKW_MARK_DYN_LOAD(lcon(0x1ffffff));
  SKW_MARK_DYN_LOAD(lcon(0x1000400));
  SKW_MARK_DYN_LOAD(lcon(0x1000600));
  SKW_MARK_DYN_LOAD(lcon(0x100070a));
  SKW_MARK_DYN_LOAD(lcon(0x1a80ffff));
  SKW_MARK_DYN_LOAD(lcon(0x1a81ffff));
  SKW_MARK_DYN_LOAD(lcon(0x300ffff));
  SKW_MARK_DYN_LOAD(lcon(0x700ffff));
  SKW_MARK_DYN_LOAD(lcon(0xd00ffff));
  SKW_MARK_DYN_LOAD(lcon(0xd2fffff));
  SKW_MARK_DYN_LOAD(lcon(0xd7effff));
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_MARK_DYN_LOAD(lcon(0x10ffffff));
  SKW_MARK_DYN_LOAD(lcon(0x15ffffff));
  SKW_MARK_DYN_LOAD(lcon(0xffff01f9));
  SKW_2676_008f(lcon(0xfff0510), lcon(0x39));
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_2676_008f(RG1L, lcon(0xb));
  SKW_MARK_DYN_LOAD(lcon(0x16ffffff));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG3W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG3W));
  SKW_MARK_DYN_LOAD(RG1L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG3W));
  SKW_MARK_DYN_LOAD(lcon(0x18ffffff));
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_2676_008f(RG1L, lcon(0x1a));
  SKW_2676_008f(RG1L, lcon(0xd8));
  SKW_2676_008f(RG1L, lcon(0x79));
  SKW_2676_008f(RG1L, lcon(0xa0));
  SKW_2676_008f(RG1L, lcon(0x89));
  SKW_2676_008f(RG1L, lcon(0xcc));
  SKW_2676_008f(RG1L, lcon(0xd2));
  SKW_2676_006a(RG1L);
  SKW_2676_008f(RG1L, lcon(0xfe));
  SKW_MARK_DYN_LOAD(lcon(0xd30ffff));
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_2676_00d0(lcon(0x9), RG2L, RG4L);
  SKW_MARK_DYN_LOAD(lcon(0x9ffffff));
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_2676_00d0(lcon(0xa), RG2L, RG4L);
  SKW_MARK_DYN_LOAD(lcon(0xaffffff));
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_MARK_DYN_LOAD(lcon(0xbffffff));
  SKW_MARK_DYN_LOAD(lcon(0xcffffff));
  SKW_MARK_DYN_LOAD(lcon(0xfffffff));
  SKW_482b_060e();
  SKW_LOAD_DYN4(DOWNCAST(i16, ddat.v1e09a0), signedlong(ddat.v1e09a4));
  SKW_LOAD_MISCITEM();
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x17), lcon(0x1), lcon(0x64), CUTLX8(RG4L)) ? 1 : 0;
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x17), lcon(0x1), lcon(0x6a), CUTLX8(RG4L)) ? 1 : 0;
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x17), lcon(0x1), lcon(0x67), CUTLX8(RG4L)) ? 1 : 0;
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x17), lcon(0x1), lcon(0x71), CUTLX8(RG4L)) ? 1 : 0;
  DM2_DEALLOC_LOBIGPOOL(lcon(0xfa));
  DM2_DEALLOC_LOBIGPOOL(lcon(0xfa));
  DM2_DEALLOC_LOBIGPOOL(lcon(0xfa));
  DM2_DEALLOC_LOBIGPOOL(lcon(0x960));
  SKW_QUERY_GDAT_IMAGE_METRICS(lcon(0x8), RG2L, &vw_14, unsignedlong(ddat.v1d6c02), &vw_30);
  SKW_QUERY_GDAT_IMAGE_METRICS(lcon(0x8), lcon(0x1), &vw_24, unsignedlong(ddat.v1d6c02), &vw_18);
  SKW_SET_SRECT(RG1R, RG4W, RG2W, RG3W, parw02);
  RG1L = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(RG2L, RG4L);
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_2676_008f(RG1L, lcon(0xfd));
  SKW_MARK_DYN_LOAD(RG1L);
  SKW_2676_008f(RG1L, lcon(0xfd));
  SKW_2676_00d0(lcon(0xf), lcon(0x40), unsignedlong(vb_34));
  SKW_UPDATE_WEATHER(lcon(0x0));
  RG1L = SKW_CHECK_RECOMPUTE_LIGHT();
void R_2F5DE(xxx* xeaxp, xxx* xedxp)
  SKW_COPY_MEMORY(RG1P, RG2L, RG4P);
i32 SKW_QUERY_GDAT_IMAGE_METRICS(i32 eaxl, i32 ebxl, i16* ecxpw, i32 edxl, i16* argpw0)
  RG1P = SKW_QUERY_GDAT_ENTRY_DATA_PTR(CUTLX8(RG5l), CUTLX8(RG2L), CUTLX8(RG3L), CUTLX8(RG4L));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(CUTLX8(RG5l), CUTLX8(RG2L), CUTLX8(RG4L)));
void SKW_3e74_5b7c(i32 eaxl)
  RG1W = SKW_476d_05b6(RG1W);
  SKW_DISPLAY_HINT_TEXT(lcon(0xf), CHGCAST(t_text, v1d1118));
  RG1P = DOWNCAST(t_text, SKW_SK_LTOA10(lcon(0x2), RG4T));
  SKW_DISPLAY_HINT_TEXT(lcon(0xf), RG1T);
  SKW_3929_0b01(lcon(0xf), lcon(0x2e));
  SKW_3929_0b20(lcon(0xf), lcon(0xc));
  SKW_SK_STRCPY(tarr_00, CHGCAST(t_text, v1d111f));
  SKW_DISPLAY_HINT_TEXT(lcon(0xf), tarr_00);
xxx* SKW_QUERY_CMDSTR_NAME(i32 eaxl, i32 ebxl, i32 edxl)
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(unsignedlong(RG1Blo), RG2L, tarr_00, RG4L));
i32 SKW_QUERY_CMDSTR_TEXT(t_text* eaxtp, t_text* edxtp)
  RG1P = DOWNCAST(t_text, SKW_SK_STRSTR(RG1T, RG5t));
  RG1P = RG4P + signedlong(CUTX16(SKW_SK_STRLEN(RG5t)));
i32 SKW_QUERY_CMDSTR_ENTRY(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(unsignedlong(RG1Blo), RG2L, tarr_00, RG4L));
  return SKW_QUERY_CMDSTR_TEXT(RG1T, RG4T);
i32 SKW_2759_0155(i32 eaxl)
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG4L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L);
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(CUTLX8(RG7L), lcon(0x5), CUTLX8(RG5l), CUTLX8(RG61L)) ? 1 : 0;
  RG1L = SKW_QUERY_CMDSTR_ENTRY(RG7L, RG2L, RG3L, RG4L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L);
  RG1L = SKW_QUERY_CMDSTR_ENTRY(SKW_QUERY_CLS1_FROM_RECORD(RG4L) & lcon(0xff), RG5l, lcon(0x8), RG62L);
i32 SKW_QUERY_CUR_CMDSTR_ENTRY(i32 eaxl)
  return SKW_QUERY_CMDSTR_ENTRY(unsignedlong(ddat.v1e0b7f), unsignedlong(ddat.v1e0b7e), RG3L, unsignedlong(ddat.v1e0b80));
i32 SKW_2759_01fe(i32 ebxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1P = SKW_GET_MISSILE_REF_OF_MINION(unsignedlong(word_at(RG2P, lcon(0x2))), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG2P, lcon(0x2))));
i32 SKW_IS_ITEM_HAND_ACTIVABLE(i32 eaxl, i32 ebxl, i32 edxl)
  RG1L = SKW_IS_CONTAINER_MONEYBOX(RG4L);
  RG1L = SKW_IS_CONTAINER_CHEST(RG4L);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG4L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L);
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(CUTLX8(RG71L), lcon(0x5), CUTLX8(RG51l), CUTLX8(RG61L)) ? 1 : 0;
  RG1L = SKW_QUERY_CMDSTR_ENTRY(RG71L, RG2L, RG3L, RG4L);
  RG1L = SKW_QUERY_CMDSTR_ENTRY(RG71L, RG2L, RG3L, RG4L);
  RG1L = SKW_FIND_POUCH_OR_SCABBARD_POSSESSION_POS(signedlong(vql_08.peek16()), RG4L);
  RG1L = SKW_2759_01fe(RG2L, RG55l);
  RG1L = SKW_QUERY_CMDSTR_ENTRY(unsignedlong(vb_18), unsignedlong(vb_20), lcon(0x8), unsignedlong(vb_1c));
  RG1L = SKW_ADD_ITEM_CHARGE(unsignedlong(vql_04.peek16()), lcon(0x0));
  RG1L = SKW_ADD_ITEM_CHARGE(RG55l, RG4L);
  RG1L = SKW_QUERY_CMDSTR_ENTRY(RG52l, RG2L, RG3L, RG4L);
  RG1L = SKW_QUERY_CMDSTR_ENTRY(RG52l, RG63L, lcon(0x1), RG72L);
  RG1L = SKW_QUERY_PLAYER_SKILL_LV(signedlong(vql_08.peek16()), lcon(0x1), RG4L);
  RG1L = SKW_IS_CONTAINER_MAP(unsignedlong(vql_04.peek16()));
i32 SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS(void)
  RG1L = SKW_PUT_OBJECT_INTO_CONTAINER(RG1L);
void SKW_ACTIVATE_ACTION_HAND(i32 eaxl, i32 edxl)
  RG1L = SKW_IS_ITEM_HAND_ACTIVABLE(RG5l, RG2L, unsignedlong(word_at(RG1P, lcon(0xc3))));
  SKW_CHAMPION_SQUAD_RECOMPUTE_POSITION();
  SKW_UPDATE_RIGHT_PANEL(lcon(0x0));
  SKW_REFRESH_PLAYER_STAT_DISP(RG5l);
void SKW_SET_SPELLING_CHAMPION(i32 eaxl)
  SKW_CHAMPION_SQUAD_RECOMPUTE_POSITION();
  SKW_UPDATE_RIGHT_PANEL(lcon(0x0));
i32 SKW_UPDATE_RIGHT_PANEL(i32 eaxl)
  SKW_LOAD_PROJECTILE_TO_HAND(RG1L, RG4L);
  RG1L = SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
  SKW_29ee_00a3(lcon(0x1));
  SKW_29ee_0396();
  RG1L = SKW_GET_PLAYER_AT_POSITION(unsignedlong(byte_at(RG54p, lcon(0x1d))));
  RG1L = signedlong(CUTX16(SKW_ABS16(ddat.v1e0b54))) - 1;
  SKW_DRAW_SQUAD_SPELL_AND_LEADER_ICON(RG1L, RG4L);
  SKW_DRAW_HAND_ACTION_ICONS(RG1L, RG2L, RG4L);
  SKW_29ee_00a3(RG1L);
  SKW_29ee_00a3(RG1L);
  SKW_DRAW_PLAYER_NAME_AT_CMDSLOT();
  RG1L = SKW_IS_CONTAINER_MAP(unsignedlong(RG3W));
  RG1L = SKW_IS_CONTAINER_MONEYBOX(RG56l);
  RG1L = SKW_IS_CONTAINER_CHEST(RG56l);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG56l);
  SKW__CHECK_ROOM_FOR_CONTAINER(RG56l, RG1P);
  RG1L = SKW_IS_ITEM_HAND_ACTIVABLE(unsignedlong(vo_1c.peek16()), signedlong(ddat.v1e0b76), unsignedlong(RG3W));
  RG1L = R_107B0();
  SKW_29ee_0b2b();
  SKW_DRAW_MAJIC_MAP(unsignedlong(RG3W));
  SKW_DRAW_CONTAINER_PANEL(unsignedlong(RG3W), lcon(0x1));
  SKW_DRAW_MONEYBOX(unsignedlong(RG3W));
  SKW_DRAW_SPELL_PANEL();
  SKW_29ee_00a3(lcon(0x0));
  SKW_DRAW_CONTAINER_PANEL(RG1L, lcon(0x0));
  SKW_DRAW_PLAYER_ATTACK_DIR();
  SKW_SHOW_ATTACK_RESULT(signedlong(ddat.v1e0b7c));
  SKW_29ee_00a3(lcon(0x1));
  SKW_0b36_0cbe(UPCAST(c_c5rects, RG1P), RG4L);
  RG1L = R_107B0();
i32 SKW_IS_MISSILE_VALID_TO_LAUNCHER(i32 eaxl, i32 ebxl, i32 edxl)
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG1W), lcon(0x5));
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG2W), lcon(0x5));
i32 SKW_2759_0e93(i32 eaxl, i32 edxl, i32 ebxl)
  RG1L = SKW_IS_ITEM_HAND_ACTIVABLE(unsignedlong(RG51w), lcon(0xffffffff), RG4L);
  RG1L = SKW_QUERY_CMDSTR_ENTRY(unsignedlong(ddat.v1e0b40[RG1L].b_00), RG2L, lcon(0x2), RG4L) & lcon(0xffff);
  SKW_IS_ITEM_HAND_ACTIVABLE(RG1L, RG2L, RG4L);
void SKW_LOAD_PROJECTILE_TO_HAND(i32 eaxl, i32 edxl)
  RG1L = SKW_IS_MISSILE_VALID_TO_LAUNCHER(unsignedlong(vql_00.peek16()), RG2L, RG4L);
  RG57L = SKW_REMOVE_POSSESSION(unsignedlong(vql_00.peek16()), RG4L);
  SKW_EQUIP_ITEM_TO_HAND(unsignedlong(vql_00.peek16()), RG2L, RG4L);
  RG1L = SKW_IS_CONTAINER_CHEST(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1L = SKW_IS_MISSILE_VALID_TO_LAUNCHER(RG1L, RG2L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(RG61L);
  SKW_CUT_RECORD_FROM(RG61L, lcon(0xffffffff), lcon(0x0), RG7p);
  SKW_EQUIP_ITEM_TO_HAND(unsignedlong(vql_00.peek16()), RG2L, RG4L);
  RG1L = SKW_IS_MISSILE_VALID_TO_LAUNCHER(unsignedlong(vql_00.peek16()), RG2L, unsignedlong(xwc_08.peek16()));
  RG55L = SKW_REMOVE_POSSESSION(unsignedlong(vql_00.peek16()), RG4L);
  SKW_EQUIP_ITEM_TO_HAND(unsignedlong(vql_00.peek16()), RG2L, RG4L);
  RG1L = SKW_2759_0e93(RG61L, RG4L, RG2L);
  RG54L = SKW_REMOVE_POSSESSION(unsignedlong(vql_00.peek16()), RG4L);
  SKW_EQUIP_ITEM_TO_HAND(unsignedlong(vql_00.peek16()), RG2L, RG4L);
  RG1L = SKW_IS_CONTAINER_CHEST(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1L = SKW_2759_0e93(signedlong(vql_00.peek16()), RG62L, RG2L);
  SKW_CUT_RECORD_FROM(RG62L, lcon(0xffffffff), lcon(0x0), RG7p);
  SKW_EQUIP_ITEM_TO_HAND(unsignedlong(vql_00.peek16()), RG2L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(RG62L);
  RG1L = SKW_2759_0e93(signedlong(vql_00.peek16()), RG4L, RG2L);
  RG53L = SKW_REMOVE_POSSESSION(unsignedlong(vql_00.peek16()), RG4L);
  RG1L = SKW_REMOVE_POSSESSION(unsignedlong(vql_00.peek16()), lcon(0xc));
  SKW_EQUIP_ITEM_TO_HAND(unsignedlong(vql_00.peek16()), RG2L, RG4L);
i32 R_30DEA(i32 eaxl)
  RG1L = SKW_QUERY_CMDSTR_ENTRY(unsignedlong(ddat.v1e0b7f), RG2L, lcon(0x8), RG4L);
  SKW_ADD_ITEM_CHARGE(unsignedlong(ddat.v1e0b50), RG4L);
  RG1L = SKW_ADD_ITEM_CHARGE(unsignedlong(ddat.v1e0b50), lcon(0x0));
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(ddat.v1e0b50), lcon(0x0));
  SKW_REMOVE_POSSESSION(RG1L, RG4L);
  SKW_DEALLOC_RECORD(unsignedlong(ddat.v1e0b50));
i32 SKW_PROCEED_COMMAND_SLOT(i32 eaxl)
  RG1L = SKW_ENGAGE_COMMAND(RG1L, RG4L);
  RG1L = SKW_RAND16(unsignedlong(mkuw(word_at(RG4P) + 1)));
  RG1L = R_30DEA(signedlong(RG2W));
  SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
  SKW_UPDATE_RIGHT_PANEL(lcon(0x0));
void SKW_2759_12e6(void)
  SKW_IS_ITEM_HAND_ACTIVABLE(RG1L, RG2L, RG4L);
  SKW_IS_ITEM_HAND_ACTIVABLE(RG5l, RG2L, RG4L);
  SKW_ENGAGE_COMMAND(RG1L, RG4L);
  RG1L = SKW_RAND16(unsignedlong(mkuw(word_at(RG3P) + 1)));
  RG1L = R_30DEA(RG62l);
i32 SKW_CONFUSE_CREATURE(i32 eaxl, i32 edxl, i32 ebxl)
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG2W)), lcon(0x4))));
  RG1L = SKW_RAND16(unsignedlong(RG6w));
  SKW_ATTACK_CREATURE(unsignedlong(ddat.v1e0b4c), RG4L, RG2L, lcon(0x2005), lcon(0x64), parl01);
i32 SKW_WIELD_WEAPON(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl, i16 argw0, i16 argw1)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG2W), RG4L);
  RG1L = signedlong(CUTX16(SKW_GET_PLAYER_AT_POSITION(unsignedlong(RG4W))));
  RG1L = SKW_UPDATE_GLOB_VAR(lcon(0x42), lcon(0x3), lcon(0x1));
  RG1L = SKW_MAX16(lcon(0x1), RG4L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(ddat.v1e0b4c));
  RG1L = SKW_QUERY_CUR_CMDSTR_ENTRY(lcon(0xa));
  RG62L = SKW_QUERY_CUR_CMDSTR_ENTRY(lcon(0xb));
  RG1L = SKW_QUERY_CUR_CMDSTR_ENTRY(lcon(0xf));
  RG1L = SKW_CALC_PLAYER_ATTACK_DAMAGE(RG52p, RG2L, RG3L, RG4L, parw00, parl01, parw02, parw03, parw04);
void SKW_2759_15d0(i32 eaxl, i32 edxl)
  RG4L = signedlong(mkw(CUTX16(SKW_BETWEEN_VALUE16(lcon(0x20), RG2L, RG4L)) / 8));
  RG1L = SKW_MAX16(lcon(0x8), RG4L);
  SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_00)));
  SKW_RECALC_LIGHT_LEVEL();
i32 SKW_ENGAGE_COMMAND(i32 eaxl, i32 edxl)
  RG1L = SKW_QUERY_CUR_CMDSTR_ENTRY(lcon(0x2));
  RG1L = SKW_QUERY_CUR_CMDSTR_ENTRY(lcon(0xd));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(vql_58.peek16()));
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vq_5c.peek16()), unsignedlong(vq_24.peek16()));
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(unsignedlong(RG1W));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(RG1Blo));
  vol_20.poke32(SKW_QUERY_CUR_CMDSTR_ENTRY(lcon(0x3)));
  RG1L = SKW_QUERY_CUR_CMDSTR_ENTRY(lcon(0x0));
  RG4L = SKW_QUERY_CUR_CMDSTR_ENTRY(lcon(0x4)) + SKW_RAND01();
  RG1L = SKW_QUERY_CUR_CMDSTR_ENTRY(lcon(0x9));
  RG1L = SKW_QUERY_CUR_CMDSTR_ENTRY(lcon(0x5));
  RG1L = SKW_QUERY_CUR_CMDSTR_ENTRY(lcon(0x10));
  RG1L = SKW_QUERY_CUR_CMDSTR_ENTRY(lcon(0x7));
  RG1L = SKW_GET_TILE_VALUE(signedlong(vq_5c.peek16()), RG4L);
  RG1L = SKW_MAX16(lcon(0x20), RG4L);
  SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_08)));
  RG62L = SKW_QUERY_CUR_CMDSTR_ENTRY(lcon(0x6)) + lcon(0xff80);
  RG1L = SKW_QUERY_PLAYER_SKILL_LV(signedlong(vql_04.peek16()), RG2L, RG4L);
  RG1L = SKW_MIN16(lcon(0x6), RG4L);
  RG1L = SKW_MAX16(lcon(0x2), RG4L);
  RG1L = SKW_CAST_CHAMPION_MISSILE_SPELL(unsignedlong(vql_04.peek16()), RG4L, RG2L, RG3L);
  RG1L = SKW_COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(signedlong(vql_04.peek16()), RG2L, RG4L);
  SKW_ATTACK_DOOR(unsignedlong(vq_5c.peek16()), unsignedlong(vq_24.peek16()), RG2L, lcon(0x0), lcon(0x2));
  RG1P = SKW_GET_ADDRESS_OF_TILE_RECORD(RG3L, RG4L);
  RG1L = SKW_IS_REBIRTH_ALTAR(RG1P);
  SKW_QUEUE_NOISE_GEN2(lcon(0xe), lcon(0x8d), lcon(0xfe), RG4Blo, parw01, parw02, parw03, lcon(0x3c), lcon(0xc8));
  RG1L = SKW_QUERY_PLAYER_SKILL_LV(signedlong(vql_04.peek16()), RG61l, lcon(0xe));
  RG1L = SKW_CONFUSE_CREATURE(unsignedlong(RG3W), RG4L, RG2L);
  SKW_2759_15d0(signedlong(vq_54.peek16()), signedlong(xw_60.peek16()));
  RG4L = unsignedlong(CUTX16(SKW_MAX16(lcon(0x2), signedlong(xw_60.peek16()))));
  RG1L = SKW_MAX16(lcon(0x20), RG4L);
  RG1L = SKW_MIN16(signedlong(CUTX16(unsignedlong(ddat.savegames1.b_04) + unsignedlong(xw_60.peek16()))), RG4L);
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vq_5c.peek16()), unsignedlong(vq_24.peek16()));
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG1W));
  RG1L = (SKW_GET_TILE_VALUE(RG3L, RG2L) & lcon(0xff)) >> bcon(0x5) & lcon(0xff);
  SKW_MOVE_RECORD_TO(lcon(0xffff), signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), RG3L, parw13);
  RG1L = SKW_MIN16(lcon(0xc8), RG4L);
  SKW_PLAYER_CONSUME_OBJECT(signedlong(vql_04.peek16()), signedlong(vq_34.peek16()), unsignedlong(vql_58.peek16()));
  RG1L = SKW_FIND_POUCH_OR_SCABBARD_POSSESSION_POS(signedlong(vql_04.peek16()), RG4L);
  RG1L = SKW_REMOVE_POSSESSION(RG72L, RG4L);
  SKW_EQUIP_ITEM_TO_HAND(RG72L, RG3L, RG4L);
  RG1L = SKW_IS_MISSILE_VALID_TO_LAUNCHER(RG73L, RG2L, RG3L);
  RG1L = SKW_UPDATE_GLOB_VAR(lcon(0x43), lcon(0x3), RG61l);
  RG1L = SKW_MAX16(RG61l, RG4L);
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(vql_58.peek16()), lcon(0x5));
  vql_2c.poke32(SKW_REMOVE_POSSESSION(RG73L, vl_1c));
  RG3L = SKW_QUERY_PLAYER_SKILL_LV(signedlong(vql_04.peek16()), RG61l, lcon(0xb));
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG73L, lcon(0xc)) & lcon(0xffff);
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG2L, lcon(0xa));
  RG2L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG2L, lcon(0x9));
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG73L, lcon(0x9)) + RG2L;
  SKW_SHOOT_CHAMPION_MISSILE(RG5p, RG2L, vl_18, RG73L, parw12);
  RG1L = SKW_MAX16(lcon(0x20), signedlong(xw_60.peek16())) * lcon(0x3);
  RG1L = SKW_QUERY_PLAYER_SKILL_LV(signedlong(vql_04.peek16()), RG61l, lcon(0xd));
  RG2L = SKW_MIN16(lcon(0xa), RG4L);
  RG1L = SKW_MIN16(signedlong(RG3W), signedlong(RG2W));
  RG61l = SKW_2c1d_1de2(unsignedlong(vql_04.peek16()), signedlong(vq_34.peek16()), RG2L);
  RG1L = SKW_SET_DESTINATION_OF_MINION_MAP(unsignedlong(vql_58.peek16()), signedlong(ddat.v1e0272), signedlong(ddat.v1e0266), signedlong(ddat.v1e0270));
  RG1P = SKW_GET_MISSILE_REF_OF_MINION(unsignedlong(word_at(RG71p, lcon(0x2))), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG71p, lcon(0x2))));
  SKW_1c9a_0247(unsignedlong(word_at(RG71p, lcon(0x2))));
  SKW_RELEASE_MINION(unsignedlong(word_at(RG71p, lcon(0x2))));
  RG1L = SKW_LOAD_GDAT_INTERFACE_00_0A();
  RG1L = SKW_UPDATE_GLOB_VAR(lcon(0x47), lcon(0x3), RG61l);
  RG1L = SKW_MAX16(RG61l, RG4L);
  RG1L = SKW_CREATE_MINION(unsignedlong(RG1W), RG4L, RG2L, RG3L, parl19, parw20, parl21, parb22);
  RG61l = SKW_2c1d_1de2(unsignedlong(vql_04.peek16()), signedlong(vq_34.peek16()), RG2L);
  RG1L = SKW_MAX16(lcon(0x20), signedlong(xw_60.peek16())) << bcon(0x2);
  RG1L = SKW_QUERY_CUR_CMDSTR_ENTRY(lcon(0x6));
  RG1L = SKW_WIELD_WEAPON(unsignedlong(vql_04.peek16()), RG2L, RG3L, RG4L, parw06, parw07);
  SKW_QUEUE_NOISE_GEN1(lcon(0xf), lcon(0x8d), lcon(0x3c), vb_70, lcon(0xc8), parw09, parw10, parw11);
  RG1L = SKW_2c1d_0186(RG5p, RG2L, RG3L, RG4L);
  RG1L = SKW_QUERY_PLAYER_SKILL_LV(signedlong(vql_04.peek16()), lcon(0x1), RG4L);
  RG1L = SKW_CREATE_MINION(unsignedlong(vb_70), RG4L, RG64L, RG3L, parl14, parw15, parl16, parb17);
  SKW_SET_DESTINATION_OF_MINION_MAP(unsignedlong(vql_58.peek16()), signedlong(ddat.v1e1028), signedlong(ddat.v1e102c), signedlong(ddat.v1e102a));
  SKW_CREATE_CLOUD(RG1L, RG4L, RG2L, RG3L, parl18);
  SKW_QUEUE_NOISE_GEN2(ddat.v1e0b7f, RG2Blo, lcon(0xfe), RG4Blo, parw23, parw24, parw25, lcon(0x73), lcon(0xc8));
  SKW_ADJUST_HAND_COOLDOWN(unsignedlong(vql_04.peek16()), RG2L, RG4L);
  SKW_ADJUST_STAMINA(signedlong(vql_04.peek16()), signedlong(vq_40.peek16()));
  SKW_ADJUST_SKILLS(unsignedlong(vql_04.peek16()), signedlong(vo_68.peek16()), unsignedlong(vq_50.peek16()));
  RG1L = SKW_QUERY_CUR_CMDSTR_ENTRY(lcon(0xe));
i32 SKW_FIND_HAND_WITH_EMPTY_FLASK(xxx* xeaxp)
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(unsignedlong(RG1W));
i32 SKW_2759_222e(i32 eaxl, i32 ebxl, xxx* xedxp)
  RG1L = SKW_RAND();
  RG1L = SKW_QUERY_PLAYER_SKILL_LV(RG1L, RG2L, RG4L);
  RG1L = SKW_RAND();
  RG1L = signedlong(CUTX16(SKW_MIN16(signedlong(CUTX16(SKW_GET_PLAYER_ABILITY(RG71p, RG2L, RG4L) + lcon(0xf))), lcon(0x73))));
  SKW_ADJUST_SKILLS(unsignedlong(vql_00.peek16()), RG2L, RG4L);
  RG1L = SKW_FIND_HAND_WITH_EMPTY_FLASK(RG71p);
  RG1L = SKW_REMOVE_POSSESSION(RG3L, RG4L);
  SKW_SET_ITEMTYPE(RG72L, RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG72L);
  RG1L = SKW_RAND();
  SKW_EQUIP_ITEM_TO_HAND(RG3L, RG2L, RG72L);
  RG1L = SKW_BETWEEN_VALUE16(lcon(0x15), RG2L, RG4L);
  SKW_CAST_CHAMPION_MISSILE_SPELL(unsignedlong(vql_00.peek16()), RG4L, RG2L, lcon(0x0));
  RG1L = SKW_RAND02() & lcon(0xffff);
  RG1L = SKW_CREATE_MINION(unsignedlong(word_at(RG61p, lcon(0x6))) >> 4 & ulcon(0x3f) & ulcon(0xffff), RG4L, RG2L, RG3L, parl00, parw01, parl02, parb03);
  SKW_CREATE_CLOUD(RG1L, RG4L, RG2L, RG3L, parl04);
  SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_04)));
  RG1L = SKW_MIN16(signedlong(CUTX16(8 * RG51L + RG4L)), lcon(0xff));
  RG1L = SKW_BETWEEN_VALUE16(lcon(0x15), RG2L, RG4L);
  SKW_CREATE_CLOUD(RG1L, RG4L, RG2L, RG3L, parl04);
  RG1L = SKW_ALLOC_NEW_DBITEM(lextendedDM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0xd), lcon(0xb), CUTLX8(RG3L), lcon(0xf)) & lcon(0xffff));
  SKW_TAKE_OBJECT(RG1L, lcon(0x1));
  RG1L = SKW_RAND01();
  SKW_MOVE_RECORD_TO(unsignedlong(RG55W), lcon(0xffffffff), lcon(0x0), RG3L, parw05);
  SKW_2759_15d0(RG1L, RG4L);
  SKW_2c1d_0186(RG71p, RG2L, lcon(0x0), RG4L);
  SKW_ADJUST_SKILLS(RG73L, RG2L, RG4L);
  SKW_ADJUST_HAND_COOLDOWN(RG73L, lcon(0x2), unsignedlong(xwc_18.peek16()));
i32 SKW_TRY_CAST_SPELL(void)
  RG1P = SKW_FIND_SPELL_BY_RUNES(RG3P + lcon(0x22));
  RG1L = SKW_2759_222e(RG51l, RG2L, RG1P);
  SKW_PROCEED_SPELL_FAILURE(RG1L);
  SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
void SKW_ADD_RUNE_TO_TAIL(i32 eaxl)
  SKW_UPDATE_RIGHT_PANEL(lcon(0x0));
  RG1L = R_107B0();
  SKW_REFRESH_PLAYER_STAT_DISP(RG1L);
void SKW_REMOVE_RUNE_FROM_TAIL(void)
  SKW_UPDATE_RIGHT_PANEL(lcon(0x0));
  R_107B0();
void SKW_CLICK_MONEYBOX(i32 eaxl)
  RG1L = SKW_GET_ITEM_ORDER_IN_CONTAINER(RG2L, RG4L);
  RG1L = SKW_TAKE_COIN_FROM_WALLET(RG2L, RG4L);
  SKW_TAKE_OBJECT(RG1L, lcon(0x0));
  RG1L = SKW_ADD_COIN_TO_WALLET(unsignedlong(RG1W), RG4L);
  RG1L = SKW_REMOVE_OBJECT_FROM_HAND();
  SKW_UPDATE_RIGHT_PANEL(lcon(0x0));
  SKW_CALC_PLAYER_WEIGHT(RG1L);
i32 SKW_29ee_000f(void)
  SKW_0b36_0c52(&c22_00, lcon(0x1), lcon(0x9));
  SKW_FILL_ENTIRE_PICT(SKW_3e74_5817(c22_00.c16_00.w_00), RG4Blo);
  SKW_0b36_0cbe(&c22_00, lcon(0x1));
  return SKW_1031_0541(lcon(0x5));
  return SKW_1031_0541(lcon(0x6));
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x1), unsignedlong(vb_34), &c22_00, lcon(0x3), parw00, lcon(0xffff));
void SKW_29ee_00a3(i32 eaxl)
  SKW_0b36_0c52(&ddat.v1d694c, RG1L, lcon(0xb));
  SKW_FILL_RECT_SUMMARY(&ddat.v1d694c, RG2Blo, RG4R);
void SKW_SHOW_ATTACK_RESULT(i32 eaxl)
  SKW_29ee_00a3(RG1L);
  SKW_QUERY_GDAT_SUMMARY_IMAGE(UPCAST(c_image, ADRESSOF(c_image, &c37_00)), unsignedlong(vb_144), RG3L, lcon(0x1));
  SKW_0b36_11c0(XUPCAST(c_image, SKW_QUERY_PICST_IT(UPCAST(c_image, ADRESSOF(c_image, &c37_00)))), lcon(0x39), lcon(0xffffffff), &ddat.v1d694c);
  SKW_DRAW_SIMPLE_STR(&ddat.v1d694c, unsignedword(byte_at(RG4P, lcon(0x4))), unsignedword(byte_at(RG4P)), lcon(0x39), par_tp00);
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x7), RG2L, UPCAST(c_c5rects, RG3P), lcon(0x0), lcon(0x7e), lcon(0xffff));
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x10), lcon(0x18), &ddat.v1d694c, RG52L, lcon(0x7d), lcon(0xffff));
void SKW_DRAW_HAND_ACTION_ICONS(i32 eaxl, i32 ebxl, i32 edxl)
  SKW_29ee_00a3(lcon(0x0));
  SKW_QUERY_EXPANDED_RECT(RG53w, RG4R);
  SKW_FILL_RECT_SUMMARY(&ddat.v1d694c, ddat.paletteptr1[0], &rc_28);
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x1), RG2L, &ddat.v1d694c, lcon(0x4), parw00, lcon(0xffff));
  RG1P = SKW_DRAW_ITEM_ON_WOOD_PANEL(signedlong(vql_04.peek16()), UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_08)), RG4L);
  SKW_CALC_CENTERED_RECT_IN_RECT(RG1R, RG2L, RG3L, RG4R);
  SKW_DRAW_ICON_PICT_BUFF(UPCAST(t_gfxdata, RG52p), RG2R, RG3L, &ddat.v1d694c, lcon(0x0), parw03, lcon(0x0), NULL);
  SKW_0b36_01cd(UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_08)));
  SKW_0b36_105b(&ddat.v1d694c, lcon(0x0), &rc_28);
void SKW_29ee_0396(void)
  SKW_29ee_00a3(lcon(0x0));
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x8), lcon(0xf5), &ddat.v1d694c, unsignedlong(ddat.v1d67d6), lcon(0x2f), lcon(0xffff));
  RG1P = DOWNCAST(t_gfxdata, SKW_ALLOC_PICT_BUFF(CUTLX8(RG1L), BIGPOOL_LO, CUTLX8(RG3L), CUTX16(RG4L)));
  SKW_FREE_PICT_BUFF(UPCAST(t_gfxdata, RG61p));
  RG1R = SKW_QUERY_EXPANDED_RECT(lcon(0x2f), RG4R);
  SKW_0b36_105b(&ddat.v1d694c, RG2L, RG4R);
  RG1R = SKW_QUERY_BLIT_RECT(UPCAST(t_gfxdata, RG61p), RG2W, RG3P16, RG4R, parpw02, lcon(0xffffffff));
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND02();
  RG1P = SKW_QUERY_GDAT_SQUAD_ICON(UPCAST(t_gfxdata, RG61p), vcarr_13c, RG53l);
  SKW_DRAW_ICON_PICT_BUFF(UPCAST(t_gfxdata, RG1P), RG2R, RG3L, &ddat.v1d694c, parw04, lcon(0xc), lcon(0x0), vcarr_13c);
  SKW_QUERY_PICST_IT(UPCAST(c_image, DOWNCAST(c_image, SKW_QUERY_GDAT_SUMMARY_IMAGE(UPCAST(c_image, ADRESSOF(c_image, &c37_00)), lcon(0x7), unsignedlong(ddat.hero[RG1L / 263].b_102), lcon(0x1)))));
  SKW_0b36_11c0(UPCAST(c_image, ADRESSOF(c_image, &c37_00)), RG2L, RG3L, &ddat.v1d694c);
void SKW_DRAW_SQUAD_SPELL_AND_LEADER_ICON(i32 eaxl, i32 edxl)
  SKW_29ee_00a3(lcon(0x0));
  RG1R = SKW_QUERY_EXPANDED_RECT(RG1W, RG4R);
  SKW_FILL_RECT_SUMMARY(&ddat.v1d694c, RG2Blo, RG4R);
  SKW_QUERY_GDAT_SUMMARY_IMAGE(UPCAST(c_image, ADRESSOF(c_image, &c37_00)), lcon(0x4), RG3L, lcon(0x1));
  SKW_0b36_11c0(XUPCAST(c_image, SKW_QUERY_PICST_IT(UPCAST(c_image, ADRESSOF(c_image, &c37_00)))), RG2L, lcon(0x4), &ddat.v1d694c);
  SKW_0b36_105b(&ddat.v1d694c, lcon(0x0), &c37_00.rc_24);
  SKW_QUERY_GDAT_SUMMARY_IMAGE(UPCAST(c_image, ADRESSOF(c_image, &c37_00)), lcon(0x4), unsignedlong(vb_148), lcon(0x1));
  SKW_0b36_11c0(XUPCAST(c_image, SKW_QUERY_PICST_IT(UPCAST(c_image, ADRESSOF(c_image, &c37_00)))), RG2L, lcon(0x4), &ddat.v1d694c);
  SKW_0b36_105b(&ddat.v1d694c, lcon(0x0), &c37_00.rc_24);
void SKW_DRAW_PLAYER_NAME_AT_CMDSLOT(void)
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x1), lcon(0x14), &ddat.v1d694c, lcon(0x4), lcon(0x3c), lcon(0xffff));
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x1), lcon(0xe), &ddat.v1d694c, lcon(0x4), lcon(0x3b), lcon(0xffff));
  SKW_DRAW_NAME_STR(&ddat.v1d694c, RG2W, RG3W, lcon(0x3d), par_tp04);
void SKW_DRAW_PLAYER_ATTACK_DIR(void)
  SKW_29ee_00a3(lcon(0x0));
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x8), lcon(0xf6), &ddat.v1d694c, unsignedlong(ddat.v1d67d6), lcon(0x5d), lcon(0xffff));
  RG1P = DOWNCAST(t_gfxdata, SKW_ALLOC_PICT_BUFF(ddat.v1d272c, BIGPOOL_LO, lcon(0x4), CUTX16(RG4L)));
  RG1R = SKW_QUERY_BLIT_RECT(UPCAST(t_gfxdata, RG61p), RG2W, RG3P16, RG4R, parpw02, lcon(0xffffffff));
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND02();
  SKW_QUERY_GDAT_SQUAD_ICON(UPCAST(t_gfxdata, RG61p), vcarr_13c, RG51l);
  SKW_DRAW_ICON_PICT_BUFF(UPCAST(t_gfxdata, RG61p), RG2R, RG3L, &ddat.v1d694c, parw04, lcon(0xc), lcon(0x0), vcarr_13c);
  SKW_QUERY_PICST_IT(SKW_QUERY_GDAT_SUMMARY_IMAGE(&c37_00, lcon(0x7), unsignedlong(ddat.hero[RG52l / 263].b_102), lcon(0x1)));
  SKW_0b36_11c0(&c37_00, lcon(0x5e), lcon(0xc), &ddat.v1d694c);
  SKW_FREE_PICT_BUFF(UPCAST(t_gfxdata, RG61p));
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x1), lcon(0x10), &ddat.v1d694c, lcon(0x4), lcon(0x60), lcon(0xffff));
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x1), lcon(0x12), &ddat.v1d694c, lcon(0x4), lcon(0x61), lcon(0xffff));
void SKW_DRAW_SPELL_TO_BE_CAST(i32 eaxl)
  SKW_29ee_00a3(lcon(0x0));
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x1), lcon(0x9), &ddat.v1d694c, lcon(0x5), lcon(0xfc), lcon(0xffff));
  RG1L = SKW_SK_STRLEN(UPCAST(t_text, RG7p + lcon(0x22)));
  SKW_DRAW_SIMPLE_STR(&ddat.v1d694c, RG2W, RG3W, RG4W, par_tp02);
void SKW_DRAW_SPELL_PANEL(void)
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x1), RG2L, &ddat.v1d694c, lcon(0x5), lcon(0x5c), lcon(0xffff));
  SKW_DRAW_SIMPLE_STR(&ddat.v1d694c, RG2W, RG3W, RG4W, par_tp02);
  SKW_DRAW_SPELL_TO_BE_CAST(lcon(0x0));
  SKW_DRAW_PLAYER_ATTACK_DIR();
void SKW_DRAW_CMD_SLOT(i32 eaxl, i32 edxl)
  SKW_29ee_00a3(lcon(0x0));
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x14), RG2L, &ddat.v1d694c, unsignedlong(ddat.v1e0ba4), parw00, lcon(0xffff));
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x1), RG2L, &ddat.v1d694c, lcon(0x4), parw02, lcon(0xffff));
  RG1P = SKW_QUERY_CMDSTR_NAME(unsignedlong(ddat.v1e0b40[RG1L].b_00), RG2L, RG4L);
  SKW_DRAW_NAME_STR(&ddat.v1d694c, RG2W, RG3W, RG4W, par_tp04);
void SKW_29ee_0b2b(void)
  SKW_DRAW_PLAYER_ATTACK_DIR();
  SKW_DRAW_CMD_SLOT(RG1L, lcon(0x0));
i32 SKW_PROCEED_SPELL_FAILURE(i32 eaxl)
  SKW_DRAW_TRANSPARENT_STATIC_PIC(lcon(0x1), lcon(0xb), lcon(0x5c), lcon(0x5), parl00);
  RG1L = SKW_UPDATE_GLOB_VAR(unsignedlong(RG1W), lcon(0x3), lcon(0x1));
  RG1L = SKW_MAX16(signedlong(RG3W), RG4L);
i32 SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(i32 eaxl, xxx* xebxp, t_colidx* ecxpcarr, i32 edxl)
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(CUTLX8(RG51l), lcon(0x1), lcon(0xf9), CUTLX8(RG6L)) ? 1 : 0;
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(CUTLX8(RG51l), CUTLX8(RG2L), CUTLX8(RG4L)));
  SKW_COPY_MEMORY(DOWNCAST(t_colidx, SKW_QUERY_GDAT_IMAGE_LOCALPAL(CUTLX8(RG51l), lcon(0xf9), CUTLX8(RG6L))), PAL16, DOWNCAST(t_colidx, pcarr_00));
void SKW_DRAW_CHIP_OF_MAGIC_MAP(xxx* xeaxp, i32 ebxl, i32 ecxl, i32 edxl, i32 argl0, t_colidx* argpcarr1)
  SKW_SET_SRECT(RG1R, RG4W, RG2W, RG3W, parw00);
  RG1R = SKW_UNION_RECT(RG1R, RG2P16, RG3P16, RG4R);
  SKW_DRAW_ICON_PICT_BUFF(UPCAST(t_gfxdata, RG5p), RG2R, RG3L, &ddat.v1d694c, parw01, lcon(0xa), parw03, argpcarr1);
void SKW_DRAW_MAP_CHIP(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl, i16 argw0, i16 argw1, i8 argb2, i32 argl3)
  SKW_SUMMARIZE_STONE_ROOM(UPCAST(s_8wordarray, ADRESSOF(s_8wordarray, &s15_10)), signedlong(vw_08), RG3L, signedlong(vql_00.peek16()));
  SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(lcon(0x8), ADRESSOF(xxx*, &xp_48), vcarr_20, RG4L);
  SKW_SET_SRECT(RG1R, RG4W, RG2W, RG3W, parw00);
  SKW_UNION_RECT(RG1R, RG2P16, RG3P16, RG4R);
  SKW_FILL_RECT_SUMMARY(UPCAST(c_c5rects, RG1P), RG2Blo, RG4R);
  SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(lcon(0x8), ADRESSOF(xxx*, &xp_48), vcarr_20, RG4L);
  RG1L = SKW_0cee_06dc(signedlong(vw_08), signedlong(vw_04)) - vql_00.peek32();
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(s15_10.warr_00[0x4]));
  SKW_DRAW_CHIP_OF_MAGIC_MAP(DOWNCAST(xxx, xp_48), RG2L, RG3L, RG4L, parl01, vcarr_20);
  RG1L = SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(lcon(0x9), RG2P, vcarr_30, RG4L);
  SKW_DRAW_CHIP_OF_MAGIC_MAP(DOWNCAST(xxx, xp_4c), RG2L, RG3L, RG4L, parl03, vcarr_30);
  SKW_DRAW_CHIP_OF_MAGIC_MAP(DOWNCAST(xxx, xp_48), RG2L, RG3L, RG4L, parl05, vcarr_20);
  RG1L = SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(lcon(0xa), ADRESSOF(xxx*, &xp_4c), vcarr_30, RG4L);
  SKW_DRAW_CHIP_OF_MAGIC_MAP(DOWNCAST(xxx, xp_4c), RG2L, RG3L, RG4L, parl07, vcarr_30);
  RG1L = SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(lcon(0x18), RG2P, vcarr_30, RG4L);
  SKW_DRAW_CHIP_OF_MAGIC_MAP(RG1P, RG2L, RG3L, RG4L, parl09, parcarr10);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG68L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG68L);
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(CUTLX8(RG67L), CUTLX8(RG2L), CUTLX8(RG4L)));
  parcarr12 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(CUTLX8(RG67L), lcon(0x40), CUTLX8(RG3L));
  SKW_DRAW_CHIP_OF_MAGIC_MAP(DOWNCAST(xxx, xp_58), RG2L, RG3L, RG4L, parl11, parcarr12);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG4L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L);
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(CUTLX8(RG3L), CUTLX8(RG2L), CUTLX8(RG4L)));
  parcarr14 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(CUTLX8(RG3L), lcon(0x40), CUTLX8(RG5L));
  SKW_DRAW_CHIP_OF_MAGIC_MAP(RG1P, RG2L, RG3L, RG4L, parl13, parcarr14);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(RG4L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L) & lcon(0xff);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG4L) & lcon(0xff);
  RG1L = SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(RG1L, RG2P, vcarr_30, RG4L);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG4L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L);
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(CUTLX8(RG5L), CUTLX8(RG2L), CUTLX8(RG4L)));
  parcarr18 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(CUTLX8(RG5L), lcon(0x40), CUTLX8(RG3L));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG52p, lcon(0x4))));
  SKW_1c9a_02c3(RG1P, RG4P);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(unsignedlong(xwc_0c.peek16()));
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(unsignedlong(xwc_0c.peek16()));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(RG3Bhi, CUTLX8(RG2L), CUTLX8(RG4L)));
  parcarr18 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(CUTLX8(RG1L), CUTLX8(RG2L), CUTLX8(RG4L));
  SKW_DRAW_CHIP_OF_MAGIC_MAP(RG1P, RG2L, RG3L, RG4L, parl17, parcarr18);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG61L);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG61L) & lcon(0xff);
  RG1L = SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(RG1L, RG2P, vcarr_30, RG4L);
  SKW_DRAW_CHIP_OF_MAGIC_MAP(DOWNCAST(xxx, xp_4c), RG2L, RG3L, RG4L, parl15, vcarr_30);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(RG62L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG62L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG5W));
  RG1L = SKW_0cee_2df4(unsignedlong(vql_90.peek16()));
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG66L);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG66L) & lcon(0xff);
  RG1L = SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(RG1L, RG2P, vcarr_30, RG4L);
  SKW_DRAW_CHIP_OF_MAGIC_MAP(DOWNCAST(xxx, xp_4c), RG2L, RG3L, RG4L, parl19, vcarr_30);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG5W));
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG63L);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG63L) & lcon(0xff);
  RG1L = SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(RG1L, RG2P, vcarr_30, RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG63L);
  RG1L = SKW_48ae_011a(RG63L) + 1;
  SKW_DRAW_CHIP_OF_MAGIC_MAP(DOWNCAST(xxx, xp_4c), RG2L, RG3L, RG4L, parl21, vcarr_30);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG5W));
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG64L);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG64L) & lcon(0xff);
  RG1L = SKW_QUERY_4BPP_PICT_BUFF_AND_PAL(RG1L, RG2P, vcarr_30, RG4L);
  RG1L = SKW_RAND02() & lcon(0xffff);
  SKW_DRAW_CHIP_OF_MAGIC_MAP(DOWNCAST(xxx, xp_4c), RG2L, RG3L, RG4L, parl23, vcarr_30);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG5W));
  RG1L = SKW_LOCATE_OTHER_LEVEL(signedlong(ddat.v1d3248), &vw_08, &vw_04, lcon(0x0), xparpp25);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  SKW_SUMMARIZE_STONE_ROOM(UPCAST(s_8wordarray, RG1P), RG2L, RG3L, RG4L);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_7c.peek16()));
  SKW_SUMMARIZE_STONE_ROOM(UPCAST(s_8wordarray, ADRESSOF(s_8wordarray, &s15_10)), signedlong(vw_08), signedlong(vw_04), signedlong(vql_00.peek16()));
  RG1L = SKW_LOCATE_OTHER_LEVEL(signedlong(ddat.v1d3248), &vw_08, &vw_04, lcon(0xffffffff), NULL);
  SKW_DRAW_CHIP_OF_MAGIC_MAP(DOWNCAST(xxx, xp_48), RG2L, RG3L, RG4L, parl27, vcarr_20);
void SKW_29ee_18eb(i32 eaxl, i32 ebxl, i32 edxl)
  RG1L = SKW_19f0_124b(&ddat.v1e0b68, RG2L, RG3L, RG4P16, parl00);
  RG1L = SKW_19f0_124b(&ddat.v1e0b5e, RG2L, RG3L, RG4P16, parl01);
void SKW_29ee_1946(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl, i16 argw0, i32 argl1, i32 argl2, i32 argl3)
  SKW_29ee_00a3(lcon(0x0));
  RG1R = SKW_QUERY_EXPANDED_RECT(lcon(0x63), RG4R);
  SKW_FILL_RECT_SUMMARY(UPCAST(c_c5rects, RG1P), RG2Blo, RG4R);
  SKW_INFLATE_RECT(RG1R, RG4W, RG2W);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1P = SKW_GET_MISSILE_REF_OF_MINION(unsignedlong(word_at(RG1P, lcon(0x2))), RG4L);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(vq_38.peek16()));
  RG1L = SKW_GET_TELEPORTER_DETAIL(&c12_10, RG2L, RG4L);
  SKW_29ee_18eb(RG1L, RG2L, RG4L);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(vql_00.peek16()));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_60.peek16()));
  SKW_CALC_VECTOR_W_DIR(signedlong(argw0), RG2L, &vw_18, signedlong(vo_44.peek16()), &vw_24);
  RG1L = SKW_LOCATE_OTHER_LEVEL(RG53l, &vw_18, &vw_24, lcon(0x0), NULL);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = (SKW_GET_TILE_VALUE(signedlong(vw_18), signedlong(vw_24)) & lcon(0xff)) >> bcon(0x5) & lcon(0xff);
  SKW_29ee_18eb(signedlong(vw_18), RG2L, signedlong(vw_24));
  SKW_DRAW_MAP_CHIP(unsignedlong(xwc_0c.peek16()), RG2L, RG3L, RG4L, parw02, parw03, parb04, parl05);
void SKW_29ee_1d03(i32 eaxl)
  SKW_29ee_00a3(RG1L);
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x14), RG2L, &ddat.v1d694c, unsignedlong(ddat.v1e0ba4), parw00, lcon(0xffff));
  SKW_DRAW_SIMPLE_STR(&ddat.v1d694c, RG2W, RG3W, RG4W, par_tp02);
void SKW_DRAW_MAJIC_MAP(i32 eaxl)
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1P = SKW_GET_MISSILE_REF_OF_MINION(unsignedlong(word_at(RG1P, lcon(0x2))), RG4L);
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x14), lcon(0x10), UPCAST(c_c5rects, RG3P), RG4L, lcon(0x5c), lcon(0xffff));
  SKW_29ee_1d03(lcon(0x0));
  SKW_DRAW_CMD_SLOT(RG1L, lcon(0x0));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG51p, lcon(0x2))));
  RG1L = SKW_QUERY_CREATURE_5x5_POS(RG1P, RG4L);
  SKW_29ee_1946(RG1L, RG2L, RG3L, RG4L, parw02, parl03, parl04, parl05);
  RG1R = SKW_QUERY_EXPANDED_RECT(lcon(0x63), RG4R);
  SKW_0b36_105b(&ddat.v1d694c, RG2L, RG4R);
void SKW_DRAW_MONEYBOX(i32 eaxl)
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG61L);
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x14), RG2L, UPCAST(c_c5rects, RG3P), RG4L, lcon(0x5c), lcon(0xffff));
  SKW_COUNT_BY_COIN_TYPES(RG1L, RG4P16);
  RG1L = SKW_GET_ITEM_ORDER_IN_CONTAINER(unsignedlong(xwc_00.peek16()), RG4L);
  RG1L = SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG2L) & lcon(0xff);
  RG1L = SKW_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(RG2L);
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(CUTLX8(RG3L), CUTLX8(RG2L), CUTLX8(RG4L)));
  pcarr_28 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(CUTLX8(RG3L), lcon(0x18), CUTLX8(vl_2c));
  SKW_QUERY_BLIT_RECT(UPCAST(t_gfxdata, RG62p), RG2W, RG3P16, RG4R, parpw02, lcon(0xffffffff));
  RG63L += SKW_MAX16(lcon(0x0), RG4L);
  RG5L = SKW_MIN16(lcon(0x20), RG4L);
  SKW_UNION_RECT(RG1R, RG2P16, RG3P16, RG4R);
  SKW_DRAW_ICON_PICT_BUFF(UPCAST(t_gfxdata, RG7p), RG2R, RG3L, &ddat.v1d694c, parw04, lcon(0xc), lcon(0x0), pcarr_28);
void SKW_DRAW_CONTAINER_PANEL(i32 eaxl, i32 edxl)
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(unsignedlong(RG1W));
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x14), lcon(0x10), &ddat.v1d694c, RG61L, lcon(0x5c), lcon(0xffff));
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x14), RG2L, UPCAST(c_c5rects, RG3P), RG4L, lcon(0xe3), lcon(0xa));
  SKW_QUERY_EXPANDED_RECT(lcon(0x5c), RG4R);
  SKW_QUERY_EXPANDED_RECT(RG1W, RG4R);
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x14), CUTLX8(RG2L), CUTLX8(RG4L)));
  parpcarr07 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(lcon(0x14), lcon(0x10), CUTLX8(RG3L));
  SKW_DRAW_ICON_PICT_BUFF(UPCAST(t_gfxdata, RG1P), RG2R, RG3L, UPCAST(c_c5rects, RG4P), parw04, lcon(0xffff), parw06, parpcarr07);
  RG1L = SKW_2405_014a(RG3L, RG2L, RG4L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG3L);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG3L) & lcon(0xff);
  SKW_DRAW_ICON_PICT_ENTRY(RG1L, RG2L, UPCAST(c_c5rects, RG3P), RG4L, parw08, lcon(0xc));
void SKW_2c1d_0009(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
  SKW_DELETE_TIMER(RG1L);
  SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_04)));
i32 SKW_2c1d_0186(xxx* xeaxp, i32 ebxl, i32 ecxl, i32 edxl)
  SKW_2c1d_0009(lcon(0xf), RG4L, RG2L, RG3L);
i32 SKW_2c1d_028c(i32 eaxl, i32 edxl, i32 ebxl)
  RG1L = SKW_ABS16(RG1W);
  RG1L = signedlong(CUTX16(SKW_ABS16(RG1W))) + RG5L;
  SKW_075f_1bc2(UPCAST(s_4bytearray, ADRESSOF(s_4bytearray, &s08_00)), signedlong(ddat.v1e0280), RG2L, RG3L, parw00, parl01);
  RG1L = SKW_GET_PLAYER_AT_POSITION(unsignedlong(s08_00.barr_00[RG1L]));
void SKW_2c1d_0300(xxx* xeaxp, i32 ebxl, i32 edxl)
  RG1L = SKW_ABS16(RG1W);
  RG1L = SKW_BETWEEN_VALUE16(lcon(0xa), RG2L, RG4L);
i32 SKW_RETRIEVE_ITEM_BONUS(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG1W), RG4L);
i32 SKW_PROCESS_ITEM_BONUS(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG4W), lcon(0x0));
  SKW_RECALC_LIGHT_LEVEL();
  RG1L = SKW_IS_ITEM_FIT_FOR_EQUIP(RG1L, lcon(0x1), RG4L);
  RG1L = SKW_RETRIEVE_ITEM_BONUS(vl_18, RG2L, RG3L, RG4L);
  RG1L = SKW_BETWEEN_VALUE16(lcon(0x0), RG2L, RG4L);
  RG1L = SKW_RETRIEVE_ITEM_BONUS(unsignedlong(xwc_00.peek16()), RG7l, RG3L, RG4L);
  SKW_2c1d_0300(RG5p, RG2L, unsignedlong(vo_20.peek16()));
  RG1L = SKW_RETRIEVE_ITEM_BONUS(unsignedlong(xwc_00.peek16()), RG7l, RG3L, lcon(0x33));
  RG1L = SKW_RETRIEVE_ITEM_BONUS(unsignedlong(xwc_00.peek16()), RG7l, RG3L, lcon(0x32));
  RG1L = SKW_RECALC_LIGHT_LEVEL();
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG2L, lcon(0x13)) & lcon(0xffff);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG2L);
  RG1L = SKW_QUEUE_TIMER(&c38_08);
  RG1L = SKW_RETRIEVE_ITEM_BONUS(unsignedlong(xwc_00.peek16()), RG7l, RG3L, RG4L);
  RG1L = SKW_CALC_PLAYER_WEIGHT(unsignedlong(vql_04.peek16()));
void SKW_PUT_ITEM_TO_PLAYER(i32 eaxl)
  RG1L = SKW_REMOVE_OBJECT_FROM_HAND();
  SKW_EQUIP_ITEM_TO_HAND(RG1L, RG2L, RG4L);
void SKW_TAKE_OBJECT(i32 eaxl, i32 edxl)
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG3L, RG4L);
  RG1L = SKW_QUERY_ITEM_WEIGHT(RG3L);
  SKW_DRAW_ITEM_IN_HAND(DOWNCAST(c_itemrecord, &ddat.savegamewpc));
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_DISPLAY_TAKEN_ITEM_NAME(RG3W);
  SKW_443c_0434();
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_PROCESS_ITEM_BONUS(signedlong(ddat.mouse_unk0e), lcon(0xffffffff), lcon(0x1), RG4L);
  R_3CE7D(unsignedlong(ddat.v1e0270), unsignedlong(ddat.v1e0272), lcon(0xffff), lcon(0x1), parl00, parl01);
i32 SKW_REMOVE_OBJECT_FROM_HAND(void)
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_443c_0434();
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_PROCESS_ITEM_BONUS(RG1L, RG3L, RG3L, RG4L);
  R_3CE7D(unsignedlong(ddat.v1e0270), unsignedlong(ddat.v1e0272), lcon(0xffff), lcon(0x1), parl00, parl01);
i32 SKW_REMOVE_POSSESSION(i32 eaxl, i32 edxl)
  RG1L = SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
  SKW_PROCESS_ITEM_BONUS(RG1L, RG4L, lcon(0xffffffff), RG62L);
void SKW_EQUIP_ITEM_TO_HAND(i32 eaxl, i32 ebxl, i32 edxl)
  SKW_PROCESS_ITEM_BONUS(RG1L, RG2L, lcon(0x1), RG4L);
void SKW_CLICK_ITEM_SLOT(i32 eaxl)
  RG1L = SKW_IS_ITEM_FIT_FOR_EQUIP(unsignedlong(RG6w), lcon(0x0), RG4L);
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  RG1L = SKW_REMOVE_OBJECT_FROM_HAND();
  SKW_REMOVE_POSSESSION(RG1L, RG4L);
  SKW_TAKE_OBJECT(unsignedlong(vql_00.peek16()), lcon(0x1));
  SKW_EQUIP_ITEM_TO_HAND(RG1L, RG2L, RG4L);
  SKW_2e62_0cfa(lcon(0x0));
  SKW_UPDATE_RIGHT_PANEL(lcon(0x0));
  SKW_443c_0434();
  SKW_FIRE_SHOW_MOUSE_CURSOR();
i32 SKW_QUERY_PLAYER_SKILL_LV(i32 eaxl, i32 ebxl, i32 edxl)
  RG4L = SKW_MAX16(lcon(0x1), signedlong(RG4W));
i32 SKW_ADJUST_SKILLS(i32 eaxl, i32 ebxl, i32 edxl)
  RG3L = SKW_QUERY_PLAYER_SKILL_LV(signedlong(vql_08.peek16()), lcon(0x0), unsignedlong(RG6W));
  RG1L = SKW_QUERY_PLAYER_SKILL_LV(signedlong(vql_08.peek16()), lcon(0x0), unsignedlong(RG6W)) & lcon(0xffff);
  RG2L = SKW_RAND01();
  RG1L = SKW_RAND01() + 1;
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND01();
  RG1L = SKW_MIN16(signedlong(CUTX16(SKW_RAND02())), RG4L);
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND16(unsignedlong(RG1W)) + vol_00.peek32();
  RG1L = SKW_RAND16(unsignedlong(RG3W) / 2 + 1 & lcon(0xffff)) + RG3L;
  SKW_DISPLAY_HINT_NEW_LINE();
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x1), RG2L, RG3T, RG4L));
  SKW_DISPLAY_HINT_TEXT(RG2L, RG1T);
i32 SKW_2c1d_0e23(i32 eaxl)
  RG1L = SKW_QUERY_ITEM_WEIGHT(unsignedlong(RG1W));
  RG1L = SKW_BETWEEN_VALUE16(lcon(0x1), RG2L, RG4L);
i32 SKW_GET_PLAYER_ABILITY(xxx* xeaxp, i32 ebxl, i32 edxl)
  RG1L = SKW_RAND16((signedlong(RG1W) * RG2L >> bcon(0x7)) + 1 & lcon(0xffff)) + lcon(0x4);
  return SKW_BETWEEN_VALUE16(lcon(0xa), lcon(0xdc), RG4L);
i32 SKW_2c1d_0f2a(xxx* xeaxp, i32 ebxl, i32 edxl)
  RG1L = SKW_GET_PLAYER_ABILITY(RG1P, lcon(0x0), RG4L);
  return SKW_0cd5_0176(RG3W, RG2W, lcon(0x7));
i32 SKW_USE_LUCK_ATTRIBUTE(xxx* xeaxp, i32 edxl)
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND16(lcon(0x64));
  RG1L = SKW_RAND16(2 * SKW_GET_PLAYER_ABILITY(RG3P, RG2L, RG4L) & lcon(0xffff));
  RG1L = SKW_GET_PLAYER_ABILITY(RG3P, RG2L, RG4L);
  RG1L = SKW_MIN16(lcon(0xdc), RG4L);
  RG1L = SKW_BETWEEN_VALUE16(lcon(0xa), RG2L, RG4L);
i32 SKW_MAX_LOAD(xxx* xeaxp)
  RG1L = 8 * SKW_GET_PLAYER_ABILITY(RG1P, lcon(0x0), lcon(0x1)) + lcon(0x64);
  RG1L = SKW_STAMINA_ADJUSTED_ATTR(RG3P, RG4L);
i32 SKW_CALC_PLAYER_WALK_DELAY(i32 eaxl)
  RG1L = SKW_MAX_LOAD(RG2P);
  RG1L = SKW_GET_PLAYER_WEIGHT(RG5L);
  RG1L = SKW_MAX16(lcon(0x1), RG4L);
i32 SKW_USE_DEXTERITY_ATTRIBUTE(i32 eaxl)
  RG6l = SKW_RAND() & lcon(0x7);
  RG1L = SKW_GET_PLAYER_ABILITY(RG3P, RG2L, RG4L);
  RG1L = SKW_GET_PLAYER_WEIGHT(RG5L) & lcon(0xffff);
  RG1L = SKW_MAX_LOAD(RG3P);
  RG2L = SKW_MAX16(lcon(0x2), RG4L);
  RG1L = SKW_RAND();
  RG1L = SKW_RAND();
  return SKW_BETWEEN_VALUE16(signedlong(RG1W), RG4L, RG3L);
i32 SKW_COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(i32 eaxl, i32 ebxl, i32 edxl)
  RG1L = SKW_RAND();
  RG1L = SKW_GET_PLAYER_ABILITY(RG6p, RG2L, RG4L);
  RG4L = SKW_QUERY_ITEM_WEIGHT(unsignedlong(xwc_08.peek16()));
  RG1L = SKW_MAX_LOAD(RG6p);
  RG1L = 2 * SKW_QUERY_PLAYER_SKILL_LV(signedlong(vql_04.peek16()), lcon(0x1), RG4L);
  RG2L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(xwc_08.peek16()), lcon(0x8));
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG73L, lcon(0x9));
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG73L, RG4L);
  RG1L = SKW_STAMINA_ADJUSTED_ATTR(RG6p, RG4L);
  return SKW_BETWEEN_VALUE16(lcon(0x0), lcon(0x64), RG4L);
i32 SKW_2c1d_132c(i32 eaxl, i32 edxl)
  RG1L = SKW_0cd5_0176(RG1W, RG2W, lcon(0x3));
i32 SKW_2c1d_135d(i32 eaxl, i32 edxl)
  RG1L = SKW_RAND16((SKW_GET_PLAYER_ABILITY(RG7p, RG2L, RG4L) & lcon(0xffff)) / 8 + 1 & lcon(0xffff));
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(word_at(RG7p, 2 * RG1L + lcon(0xc3))), lcon(0xb));
  RG1L = SKW_2c1d_132c(unsignedlong(RG1W), RG4L);
  RG1L = SKW_RAND02() + lcon(0x8);
  return SKW_BETWEEN_VALUE16(lcon(0x0), lcon(0x64), RG4L);
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(word_at(RG7p, 2 * RG3L + lcon(0xc3))), lcon(0xb));
  RG1L = SKW_COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(signedlong(vql_04.peek16()), lcon(0x7), RG3L);
  RG1L = SKW_2c1d_132c(unsignedlong(RG6W), RG4L);
i32 SKW_RESUME_FROM_WAKE(void)
  SKW_INIT_BACKBUFF();
  return SKW_1031_0541(lcon(0x5));
i32 SKW_DROP_PLAYER_ITEMS(i32 eaxl)
  RG1L = SKW_REMOVE_POSSESSION(unsignedlong(RG6w), RG4L);
  SKW_MOVE_RECORD_TO(RG1L, lcon(0xffffffff), RG2L, RG3L, parw00);
void R_36EFE(i32 eaxl)
  RG1L = signedlong(CUTX16(SKW_GET_PLAYER_AT_POSITION(unsignedlong(byte_at(RG2P, lcon(0x1d))))));
  RG1L = signedlong(CUTX16(SKW_GET_PLAYER_AT_POSITION(unsignedlong(mkuw(ddat.v1e0258 + RG4W & wcon(0x3))))));
void SKW_PLAYER_DEFEATED(i32 eaxl)
  RG1L = SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
  SKW_FIRE_MOUSE_RELEASE_CAPTURE();
  SKW_DISPLAY_TAKEN_ITEM_NAME(RG1W);
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_FIRE_MOUSE_RELEASE_CAPTURE();
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_24a5_1798(lcon(0x4));
  SKW_FIRE_MOUSE_RELEASE_CAPTURE();
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  RG1L = SKW_DROP_PLAYER_ITEMS(unsignedlong(RG6W));
  RG1L = SKW_ALLOC_NEW_RECORD(lcon(0x800a));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  SKW_MOVE_RECORD_TO(RG1L, lcon(0xffffffff), lcon(0x0), RG3L, parw00);
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_CHAMPION_SQUAD_RECOMPUTE_POSITION();
  SKW_2c1d_1bb5(signedlong(RG6W), lcon(0x2710));
  SKW_SELECT_CHAMPION_LEADER(RG1L);
  SKW_1031_098e();
  R_107B0();
  SKW_FIRE_SHOW_MOUSE_CURSOR();
void SKW_PROCESS_PLAYERS_DAMAGE(void)
  SKW_3a15_05f7(RG1L);
  RG1L = SKW_QUEUE_TIMER(&c38_00);
  SKW_PLAYER_DEFEATED(RG2L);
i32 SKW_WOUND_PLAYER(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
  RG1L = SKW_2c1d_135d(signedlong(vql_00.peek16()), RG4L);
  RG1L = SKW_QUERY_PLAYER_SKILL_LV(RG1L, RG2L, lcon(0x7));
  RG1L = SKW_RAND();
  RG1L = SKW_2c1d_0f2a(RG7p, RG2L, RG4L);
  RG1L = SKW_QUERY_PLAYER_SKILL_LV(RG1L, RG2L, RG2L);
  RG1L = SKW_2c1d_0f2a(RG7p, RG2L, RG4L);
  RG1L = SKW_GET_PLAYER_ABILITY(RG7p, RG2L, RG4L);
  RG1L = SKW_0cd5_0176(RG6W, RG2W, lcon(0x6));
  RG1L = SKW_0cd5_0176(RG6W, RG2W, lcon(0x6));
  RG1L = SKW_RAND();
  RG1L = SKW_2c1d_0f2a(RG7p, RG2L, RG4L);
  RG3L = SKW_RAND();
  SKW_RESUME_FROM_WAKE();
i32 SKW_PROCESS_POISON(i32 eaxl, i32 edxl)
  RG1L = SKW_MAX16(lcon(0x1), RG4L);
  RG1L = SKW_WOUND_PLAYER(RG7l, RG2L, RG3L, RG4L);
  return SKW_QUEUE_TIMER(&c38_04);
i32 SKW_2c1d_1bb5(i32 eaxl, i32 edxl)
  SKW_DELETE_TIMER(RG1L);
i32 SKW_ATTACK_PARTY(i32 eaxl, i32 edxl, i32 ebxl)
  RG1L = SKW_RAND16(unsignedlong(xwc_0c.peek16())) + RG6L;
  RG1L = SKW_MAX16(lcon(0x1), RG4L);
  RG1L = SKW_WOUND_PLAYER(signedlong(vo_08.peek16()), RG2L, RG3L, RG4L);
i32 SKW_ADJUST_STAMINA(i32 eaxl, i32 edxl)
  SKW_WOUND_PLAYER(RG1L, lcon(0x0), lcon(0x0), RG4L);
  RG1L = signedlong(CUTX16(SKW_ABS16(RG6W)));
void SKW_SHOOT_CHAMPION_MISSILE(xxx* xeaxp, i32 ebxl, i32 ecxl, i32 edxl, i16 argw0)
  RG1L = SKW_MIN16(signedlong(argw0), lcon(0xff)) & lcon(0xff);
  RG1L = SKW_MIN16(signedlong(RG3W), lcon(0xff)) & lcon(0xff);
  RG1L = SKW_MIN16(signedlong(RG2W), lcon(0xff)) & lcon(0xff);
  SKW_SHOOT_ITEM(unsignedlong(xwc_00.peek16()), signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), RG3L, parl00, parb01, parb02, parb03);
i32 SKW_CAST_CHAMPION_MISSILE_SPELL(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
  RG1L = SKW_MIN16(lcon(0x6), RG4L);
  SKW_SHOOT_CHAMPION_MISSILE(RG5p, RG2L, lcon(0x5a), RG4L, parw00);
i32 SKW_2c1d_1de2(i32 eaxl, i32 edxl, i32 ebxl)
  RG1L = SKW_REMOVE_OBJECT_FROM_HAND();
  RG1L = SKW_COMPUTE_PLAYER_ATTACK_OR_THROW_STRENGTH(signedlong(RG6w), lcon(0xa), RG4L);
  RG1L = SKW_REMOVE_POSSESSION(unsignedlong(RG6w), RG4L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG5L);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG5L) & lcon(0xff);
  SKW_QUEUE_NOISE_GEN2(RG1Blo, lcon(0x86), lcon(0xfe), RG4Blo, parw00, parw01, lcon(0x1), lcon(0x6e), lcon(0x80));
  RG1L = SKW_2c1d_0e23(RG5L);
  SKW_ADJUST_STAMINA(RG1L, RG4L);
  SKW_ADJUST_HAND_COOLDOWN(RG1L, RG2L, lcon(0x4));
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG5L, RG4L);
  SKW_ADJUST_SKILLS(RG1L, RG2L, lcon(0xa));
  RG1L = SKW_QUERY_PLAYER_SKILL_LV(signedlong(RG6w), lcon(0x1), lcon(0xa));
  RG1L = SKW_RAND16(unsignedlong(RG1W)) + RG3L;
  RG1L = SKW_MIN16(signedlong(vo_08.peek16()), lcon(0xff));
  RG1L = SKW_RAND();
  RG1L = SKW_BETWEEN_VALUE16(lcon(0x28), RG2L, RG4L);
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(xwc_0c.peek16()), lcon(0xc));
  RG1L = SKW_MAX16(lcon(0x5), RG4L);
  SKW_SHOOT_ITEM(unsignedlong(xwc_0c.peek16()), signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), RG3L, parl05, parb06, parb07, parb08);
i32 R_37BBB(i32 eaxl)
  return SKW_2c1d_1de2(unsignedlong(ddat.mouse_unk0e), lcon(0xffffffff), RG2L);
i32 R_37BEA(i32 eaxl)
  RG1L = SKW_GET_PLAYER_WEIGHT(unsignedlong(RG2W));
i32 SKW_GET_PARTY_SPECIAL_FORCE(void)
  RG1L = R_37BEA(RG1L);
i32 SKW_CALC_PLAYER_WEIGHT(i32 eaxl)
  RG1L = SKW_IS_CONTAINER_CHEST(unsignedlong(ddat.v1d67c2.warr2d_00[RG3L / 4 + 1][signedlong(ddat.v1e0b76)]));
  RG1L = SKW_QUERY_ITEM_WEIGHT(unsignedlong(ddat.v1e095c[RG1L]));
  RG1L = SKW_QUERY_ITEM_WEIGHT(unsignedlong(ddat.hero[RG3L].warr_c3[RG5L]));
void SKW_UPDATE_CHAMPIONS_STATS(void)
  RG5L = SKW_QUERY_PLAYER_SKILL_LV(RG61l, lcon(0x1), lcon(0x3)) + SKW_QUERY_PLAYER_SKILL_LV(RG61l, lcon(0x1), lcon(0x2));
  RG1L = (SKW_GET_PLAYER_ABILITY(RG3P, RG2L, RG4L) & lcon(0xffff)) + RG62L;
  RG1L = SKW_MAX16(lcon(0x7), RG4L) * RG5L;
  SKW_ADJUST_STAMINA(signedlong(vo_04.peek16()), RG4L);
  RG1L = SKW_MIN16(signedlong(RG5W), RG4L);
  RG1L = SKW_BETWEEN_VALUE16(lcon(0x1), lcon(0x6), RG4L);
  SKW_ADJUST_STAMINA(signedlong(vo_04.peek16()), RG4L);
  RG1L = SKW_GET_PLAYER_ABILITY(RG3P, RG2L, RG4L) & lcon(0xffff);
  RG1L = SKW_MIN16(signedlong(RG1W), RG4L);
xxx* SKW_QUERY_GDAT_SQUAD_ICON(t_gfxdata* eaxpgarr, t_colidx* ebxpcarr, i32 edxl)
  SKW_COPY_MEMORY(DOWNCAST(t_colidx, SKW_QUERY_GDAT_IMAGE_LOCALPAL(lcon(0x1), CUTLX8(RG3L), lcon(0x6))), PAL16, DOWNCAST(t_colidx, ebxpcarr));
  RG1R = SKW_ALLOC_TEMP_ORIGIN_RECT(ddat.v1d272c, ddat.v1d272e);
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x1), CUTLX8(RG2L), CUTLX8(RG4L)));
  SKW_DRAW_DIALOGUE_PICT(UPCAST(t_gfxdata, RG1P), RG2R, RG3W, UPCAST(t_gfxdata, RG5p), lcon(0x0), lcon(0x2), NULL);
void SKW_DRAW_PLAYER_3STAT_PANE(i32 eaxl, i32 edxl)
  SKW_0b36_0c52(&ddat.v1d69d4, RG5L, RG3L);
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x1), unsignedlong(vb_00), &ddat.v1d69d4, lcon(0x2), parw00, lcon(0xffff));
void SKW_DRAW_PLAYER_3STAT_HEALTH_BAR(i32 eaxl)
  RG1R = SKW_QUERY_EXPANDED_RECT(RG1W, RG4R);
  SKW_0b36_0d67(&ddat.v1d69d4, RG4R);
  RG1L = SKW_MAX16(RG2L, RG4L);
  RG1R = SKW_SCALE_RECT(RG1W, RG2W, RG3W, RG4R);
  SKW_COPY_RECT(&rc_18, &rc_10);
  SKW_FILL_RECT_SUMMARY(&ddat.v1d69d4, ddat.paletteptr1[0], &rc_10);
  SKW_FILL_RECT_SUMMARY(&ddat.v1d69d4, RG2Blo, &rc_18);
void SKW_DRAW_CUR_MAX_HMS(i32 eaxl, i32 ebxl, i32 edxl)
  RG1T = SKW_FMT_NUM(RG4W, lcon(0x3), lcon(0x1));
  SKW_SK_STRCPY(tarr_00, RG1T);
  SKW_SK_STRCAT(tarr_00, CHGCAST(t_text, v1d1124));
  RG1P = DOWNCAST(t_text, SKW_FMT_NUM(RG3W, lcon(0x3), lcon(0x1)));
  SKW_SK_STRCAT(tarr_00, RG1T);
  SKW_DRAW_LOCAL_TEXT(RG1L, RG2W, tarr_00, RG4L);
void SKW_DRAW_PLAYER_3STAT_TEXT(xxx* xeaxp)
  SKW_DRAW_CUR_MAX_HMS(lcon(0x226), unsignedlong(word_at(RG1P, lcon(0x38))), unsignedlong(word_at(RG1P, lcon(0x36))));
  SKW_DRAW_CUR_MAX_HMS(lcon(0x227), RG2L, RG4L);
  SKW_DRAW_CUR_MAX_HMS(lcon(0x228), unsignedlong(word_at(RG3P, lcon(0x40))), unsignedlong(word_at(RG3P, lcon(0x3e))));
i32 SKW_2e62_03b5(i32 eaxl, i32 ebxl, i32 edxl)
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG3L, RG4L);
  RG1L = SKW_2405_014a(RG3L, RG2L, RG4L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(unsignedlong(vql_10.peek16()));
  SKW_DRAW_PLAYER_3STAT_PANE(unsignedlong(vql_00.peek16()), lcon(0x0));
  SKW_DRAW_ITEM_ICON(unsignedlong(vql_10.peek16()), vl_04, vl_0c, RG4L, parl00);
void SKW_DRAW_PLAYER_DAMAGE(i32 eaxl)
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x1), lcon(0x3), &ddat.v1d69d4, lcon(0x2), parw00, lcon(0xa));
  RG1P = DOWNCAST(t_text, SKW_FMT_NUM(ddat.hero[RG4L / 263].w_30, lcon(0x3), lcon(0x0)));
  SKW_DRAW_SIMPLE_STR(&ddat.v1d69d4, unsignedword(byte_at(RG1P, lcon(0xf))), unsignedword(byte_at(RG1P, lcon(0x8))), RG5L, par_tp02);
void SKW_DRAW_EYE_MOUTH_COLORED_RECTANGLE(i32 eaxl, i32 edxl)
  SKW_2405_011f(RG1L, RG4R);
  RG5p = DOWNCAST(t_colidx, SKW_QUERY_GDAT_IMAGE_LOCALPAL(lcon(0x1), CUTLX8(RG3L), lcon(0x2)));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x1), CUTLX8(RG2L), CUTLX8(RG4L)));
  SKW_DRAW_DIALOGUE_PARTS_PICT(UPCAST(t_gfxdata, RG1P), RG2W, UPCAST(t_colidx, RG5p), RG4R);
void SKW_2e62_061d(i32 eaxl)
  SKW_DRAW_ICON_PICT_ENTRY(lcon(0x16), lcon(0x0), &ddat.v1d69d4, unsignedlong(ddat.hero[RG1L / 263].b_101), parw00, lcon(0xffff));
  RG1R = SKW_QUERY_EXPANDED_RECT(RG5W, RG4R);
  SKW_0b36_105b(&ddat.v1d69d4, RG2L, RG4R);
void SKW_DRAW_CHARSHEET_OPTION_ICON(i32 eaxl, i32 ebxl, i32 edxl)
  SKW_DRAW_STATIC_PIC(lcon(0x7), RG2L, RG3L, lcon(0x0), parl00);
void SKW_REFRESH_PLAYER_STAT_DISP(i32 eaxl)
  SKW_DRAW_STATIC_PIC(lcon(0x7), lcon(0x0), lcon(0x4), lcon(0x0), parl00);
  SKW_DRAW_CHARSHEET_OPTION_ICON(lcon(0x11), lcon(0x10), lcon(0x238));
  SKW_DRAW_CHARSHEET_OPTION_ICON(lcon(0x13), lcon(0x1), lcon(0x267));
  SKW_DRAW_CHARSHEET_OPTION_ICON(lcon(0xf), lcon(0x8), lcon(0x232));
  SKW_DRAW_CHARSHEET_OPTION_ICON(lcon(0xd), lcon(0x4), lcon(0x234));
  SKW_DRAW_CHARSHEET_OPTION_ICON(lcon(0xb), lcon(0x2), lcon(0x236));
  SKW_DRAW_PLAYER_3STAT_PANE(RG2L, lcon(0x1));
  SKW_DRAW_NAME_STR(&ddat.v1d69d4, RG2W, RG3W, RG4W, par_tp01);
  SKW_2e62_061d(RG2L);
  SKW_DRAW_PLAYER_3STAT_PANE(RG2L, lcon(0x0));
  SKW_DRAW_PLAYER_3STAT_HEALTH_BAR(RG2L);
  SKW_DRAW_PLAYER_DAMAGE(RG1L);
  SKW_QUERY_EXPANDED_RECT(lcon(0x225), RG4R);
  parpcarr04 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(lcon(0x7), lcon(0x0), lcon(0x0));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x7), CUTLX8(RG2L), CUTLX8(RG4L)));
  SKW_DRAW_DIALOGUE_PICT(UPCAST(t_gfxdata, RG1P), RG2R, RG3W, UPCAST(t_gfxdata, DOWNCAST(xxx, xp_94)), parw02, lcon(0xffff), parpcarr04);
  SKW_DRAW_PLAYER_3STAT_TEXT(RG5p);
  SKW_DRAW_EYE_MOUTH_COLORED_RECTANGLE(RG1L, RG4L);
  SKW_DRAW_EYE_MOUTH_COLORED_RECTANGLE(RG1L, lcon(0x222));
  RG1L = SKW_2e62_03b5(unsignedlong(RG61W), vl_88, RG4L);
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG4W), lcon(0x0));
  SKW_DRAW_ITEM_SURVEY(unsignedlong(ddat.v1d66fe), ddat.v1e0228);
  SKW_DRAW_STATIC_PIC(lcon(0x7), lcon(0x25), lcon(0x221), lcon(0x0), parl05);
  SKW_DRAW_STATIC_PIC(lcon(0x7), RG2L, RG3L, lcon(0x0), parl06);
  RG1L = SKW_DRAW_ITEM_SURVEY(unsignedlong(vql_a8.peek16()), lcon(0x0));
  SKW_DRAW_CRYOCELL_LEVER(lcon(0x0));
  SKW_DRAW_FOOD_WATER_POISON_PANEL();
  SKW_DRAW_ITEM_SURVEY(RG1L, lcon(0x1));
  SKW_24a5_1532();
  RG1L = SKW_2e62_03b5(unsignedlong(RG61W), vl_8c, RG4L);
  SKW_DRAW_PLAYER_3STAT_PANE(RG1L, lcon(0x0));
  SKW_DRAW_NAME_STR(&ddat.v1d69d4, RG2W, RG1W, RG4W, par_tp07);
  SKW_SK_STRCPY(RG1T, RG4T);
  SKW_SK_STRCAT(tarr_00, ddat.v1d70e4);
  SKW_SK_STRCAT(tarr_00, UPCAST(t_text, RG5p + lcon(0x8)));
  SKW_DRAW_LOCAL_TEXT(lcon(0x229), RG2W, UPCAST(t_text, DOWNCAST(t_text, tp_90)), RG4L);
  SKW_QUERY_EXPANDED_RECT(lcon(0x22a), RG4R);
  parpcarr10 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(lcon(0x7), lcon(0x0), lcon(0x0));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x7), CUTLX8(RG2L), CUTLX8(RG4L)));
  SKW_DRAW_DIALOGUE_PICT(UPCAST(t_gfxdata, RG1P), RG2R, RG3W, UPCAST(t_gfxdata, DOWNCAST(xxx, xp_94)), parw08, lcon(0xffff), parpcarr10);
  RG1L = SKW_GET_PLAYER_WEIGHT(unsignedlong(RG61W));
  RG1L = SKW_MAX_LOAD(RG5p);
  RG1L = SKW_MAX_LOAD(RG5p);
  SKW_QUERY_GDAT_TEXT(lcon(0x7), lcon(0x2a), tarr_00, lcon(0x0));
  SKW_DRAW_LOCAL_TEXT(lcon(0x22b), RG2W, tarr_00, unsignedlong(byte_at(RG1P)));
  SKW_0b36_0cbe(&ddat.v1d69d4, lcon(0x1));
  SKW_CHANGE_VIEWPORT_TO_INVENTORY(lcon(0x0));
i32 SKW_2e62_0cfa(i32 eaxl)
  return SKW_UPDATE_RIGHT_PANEL(RG5L);
  SKW_REFRESH_PLAYER_STAT_DISP(RG1L);
  SKW_REFRESH_PLAYER_STAT_DISP(RG1L);
  return SKW_UPDATE_RIGHT_PANEL(RG5L);
void SKW_GLOBAL_UPDATE_UNKNOW1(void)
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(RG4W), lcon(0x0));
  RG1L = SKW_2405_014a(unsignedlong(ddat.savegamewpc.w_00), lcon(0x1), lcon(0xffffffff));
  SKW_DRAW_ITEM_IN_HAND(DOWNCAST(c_itemrecord, &ddat.savegamewpc));
  SKW_443c_0434();
void SKW_ADD_ITEM_TO_PLAYER(i32 eaxl, i32 edxl)
  RG1L = SKW_IS_ITEM_FIT_FOR_EQUIP(RG1L, RG2L, RG4L);
  SKW_EQUIP_ITEM_TO_HAND(unsignedlong(xwc_00.peek16()), RG2L, RG4L);
i32 SKW_REVIVE_PLAYER(i32 eaxl, i32 ebxl)
  DM2_ZERO_MEMORY(RG6p, lcon(0x107));
  RG1L = signedlong(CUTX16(SKW_GET_PLAYER_AT_POSITION(unsignedlong(mkuw(vo_80.peek16() + ddat.v1e0258 & wcon(0x3))))));
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x16), RG2L, RG3T, RG4L));
  RG1P = SKW_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0x16), CUTLX8(RG2L), CUTLX8(RG3L), CUTLX8(RG4L));
  RG1L = SKW_MAX16(lcon(0x1e), RG4L);
  RG1L = SKW_RAND();
  RG1L = SKW_RAND();
i32 SKW_SELECT_CHAMPION(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(vol_08.peek16()), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG2W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG2W));
  SKW_REVIVE_PLAYER(unsignedlong(xbc_0c.peek8()), RG2L);
  SKW_DISPLAY_RIGHT_PANEL_SQUAD_HANDS();
  SKW_UPDATE_RIGHT_PANEL(lcon(0x0));
  SKW_SELECT_CHAMPION_LEADER(lcon(0x0));
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(vol_08.peek16()), RG4L);
  SKW_ADD_ITEM_TO_PLAYER(unsignedlong(RG5W), RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG2W));
  SKW_24a5_1798(RG1L);
  SKW_38c8_0002(SKW_1031_0541(lcon(0x7)));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_10.peek16()));
  SKW_CALC_PLAYER_WEIGHT(RG1L);
void R_39796(xxx* xeaxp)
  SKW_COPY_MEMORY(RG1P, lcon(0x8), ADRESSOF(s_1stname, &s34_18));
  SKW_COPY_MEMORY(DOWNCAST(xxx, xp_00) + lcon(0x8), lcon(0x14), ADRESSOF(s_2ndname, &s35_04));
  SKW_QUERY_TOPLEFT_OF_RECT(lcon(0x229), &vw_2c, &vw_28);
  SKW_QUERY_TOPLEFT_OF_RECT(lcon(0x273), &vw_20, &vw_34);
  RG1L = SKW_1031_0675(lcon(0x9));
  SKW_38c8_0002(RG1L);
  RG1L = SKW_SK_STRLEN(RG7t);
  SKW_DRAW_STATIC_PIC(lcon(0x7), lcon(0x14), lcon(0x267), lcon(0x0), parl00);
  SKW_DRAW_VP_RC_STR(lcon(0x229), UPCAST(t_text, DOWNCAST(xxx, xp_00)), unsignedlong(ddat.paletteptr1[lcon(0xb)]));
  SKW_DRAW_VP_RC_STR(lcon(0x273), UPCAST(t_text, DOWNCAST(xxx, xp_00) + lcon(0x8)), unsignedlong(ddat.paletteptr1[lcon(0xb)]));
  SKW_DRAW_VP_STR(vo_38.peek16(), RG2W, v1d6a4f, RG4W);
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_CHANGE_VIEWPORT_TO_INVENTORY(lcon(0x0));
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_EVENT_LOOP();
  SKW_COPY_MEMORY(ADRESSOF(s_1stname, &s34_18), lcon(0x8), DOWNCAST(xxx, xp_00));
  SKW_COPY_MEMORY(ADRESSOF(s_2ndname, &s35_04), lcon(0x14), DOWNCAST(xxx, xp_00) + lcon(0x8));
  RG1W = SKW_476d_05b6(RG2W);
  RG1L = SKW_SK_STRLEN(RG7t);
  RG1L = SKW_SK_STRLEN(RG7t);
  SKW_DRAW_STATIC_PIC(lcon(0x7), lcon(0x13), lcon(0x267), lcon(0x0), parl01);
  SKW_REFRESH_PLAYER_STAT_DISP(RG1L);
  SKW_38c8_0060();
  SKW_1031_06a5();
void SKW_2f3f_04ea(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0)
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG4L = SKW_REMOVE_OBJECT_FROM_HAND();
  SKW_DRAW_CRYOCELL_LEVER(lcon(0x1));
  SKW_ADD_ITEM_TO_PLAYER(unsignedlong(vql_8c.peek16()), RG4L);
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(vol_04.peek16()), RG4L);
  SKW_CUT_RECORD_FROM(RG1L, RG2L, RG3L, RG4P);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG2W));
  SKW_DISPLAY_HINT_NEW_LINE();
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x1), RG2L, RG3T, RG4L));
  SKW_DISPLAY_HINT_TEXT(RG2L, RG1T);
  SKW_24a5_1798(lcon(0x4));
  SKW_38c8_0060();
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG2W));
  SKW_24a5_1798(lcon(0x4));
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_SELECT_CHAMPION_LEADER(lcon(0xffffffff));
  SKW_FIRE_FILL_SCREEN_RECT(RG1W, RG4Blo);
  SKW_38c8_0060();
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_88.peek16()));
void SKW_BRING_CHAMPION_TO_LIFE(i32 eaxl)
  R_36EFE(RG1L);
  RG1L = SKW_MAX16(lcon(0x19), RG4L);
  R_107B0();
  SKW_443c_040e();
i32 SKW_2f3f_0789(void)
  SKW_TAKE_OBJECT(RG1L, lcon(0x1));
  SKW_443c_0434();
  SKW_SET_PARTY_HERO_FLAG(lcon(0x4000));
  return SKW_SELECT_CHAMPION_LEADER(RG1L);
  RG1L = SKW_GET_TILE_RECORD_LINK(lcon(0x0), lcon(0x0));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  SKW_SELECT_CHAMPION(lcon(0x0), lcon(0x0), signedlong(ddat.v1e0266), RG4L);
  SKW_2f3f_04ea(lcon(0x0), lcon(0x1), lcon(0x0), signedlong(ddat.v1e0266), lcon(0x92));
  SKW_SET_PARTY_HERO_FLAG(lcon(0x4000));
  return SKW_SELECT_CHAMPION_LEADER(lcon(0x0));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
void SKW_ROTATE_CREATURE(i32 eaxl, i32 edxl, i32 ebxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG3W));
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG3W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L & lcon(0xffff));
i32 SKW_IS_OBJECT_FLOATING(i32 eaxl)
  return SKW_IS_CREATURE_FLOATING(unsignedlong(RG1W));
void SKW_SET_MINION_RECENT_OPEN_DOOR_LOCATION(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
void SKW_2fcf_01c5(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
  SKW_SET_MINION_RECENT_OPEN_DOOR_LOCATION(RG1L, RG4L, RG2L, RG3L, parl00);
  SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_04)));
void SKW_2fcf_0234(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
  DM2_ZERO_MEMORY(ADRESSOF(i8, vba_10), lcon(0x4));
  RG1L = signedlong(SKW_CALC_SQUARE_DISTANCE(RG1W, RG2W, RG3W, CUTX16(vl_18)));
  RG1L = SKW_CALC_VECTOR_DIR(CUTX16(vl_1c), RG2W, RG3W, CUTX16(vl_18));
  DM2_ZERO_MEMORY(ADRESSOF(i8, vba_14), lcon(0x4));
  RG1L = SKW_GET_PLAYER_AT_POSITION(RG4L);
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(xw_20.peek16()), RG4L);
  SKW_COPY_MEMORY(ADRESSOF(i8, vba_14), RG2L, ADRESSOF(i8, vba_10));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG5L);
  RG1L = SKW_075f_0af9(lcon(0xfffffffd), RG4L, RG2L, RG3L, parl00);
  SKW_075f_056c(RG1L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG62W));
i32 SKW_2fcf_0434(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1)
  vl_04 = SKW_IS_OBJECT_FLOATING(RG2L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG2L);
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG2L, RG4L);
  SKW_29ee_000f();
  R_3B001(signedlong(vo_0c.peek16()), signedlong(argw0));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG61W));
  SKW_LOAD_LOCALLEVEL_GRAPHICS_TABLE(signedlong(vo_0c.peek16()), signedlong(vq_30.peek16()), signedlong(argw0));
  SKW_LOAD_LOCALLEVEL_DYN();
  SKW_DISPLAY_VIEWPORT(signedlong(ddat.v1e0258), RG2L, signedlong(vo_0c.peek16()));
  SKW_CHANGE_VIEWPORT_TO_INVENTORY(lcon(0x1));
  RG1L = SKW_LOCATE_OTHER_LEVEL(RG1L, RG2P16, RG3P16, lcon(0x1), NULL);
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(vo_0c.peek16()), signedlong(argw0));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
  RG1L = SKW_RAND16(R_3D93B(lcon(0x3), lcon(0x0), RG5L, UPCAST(i16, ADRESSOF(c_o32, &vo_0c)), &argw0) & lcon(0xffff)) + 1;
  RG1L = R_3D93B(lcon(0x2), RG4L, RG2L, RG3P16, &argw0);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(vq_30.peek16()));
  R_3B001(signedlong(vo_0c.peek16()), RG4L);
  RG1L = SKW_MIN16(signedlong(CUTX16(long_at(RG52p, lcon(0x36)) >> bcon(0x12))), lcon(0x11));
  RG1L = (SKW_RAND02() + RG4L) * vol_14.peek32();
  SKW_WOUND_PLAYER(RG71l, lcon(0x30), lcon(0x2), RG4L);
  SKW_QUEUE_NOISE_GEN2(lcon(0x16), RG2Blo, RG3Blo, RG4Blo, parw15, parw16, lcon(0xffff), lcon(0xbe), lcon(0xff));
  RG1L = SKW_GET_PLAYER_WEIGHT(unsignedlong(vo_34.peek16()));
  RG1L = SKW_MAX_LOAD(RG53p);
  SKW_ADJUST_STAMINA(RG72l, RG4L);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(vq_28.peek16()));
  SKW_ATTACK_CREATURE(RG1L, RG4L, RG2L, RG3L, lcon(0x0), lcon(0x14));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(vq_30.peek16()));
  RG1P = SKW_GET_ADDRESS_OF_TILE_RECORD(RG2L, RG4L);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(vq_30.peek16()));
  RG1L = SKW_ROTATE_RECORD_BY_TELEPORTER(RG54p, RG4L);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(vq_28.peek16()));
  SKW_QUEUE_NOISE_GEN1(lcon(0x18), lcon(0x89), lcon(0x61), lcon(0x0), lcon(0x80), parw05, parw06, lcon(0x1));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_18.peek16()));
  SKW_QUEUE_NOISE_GEN1(RG1Blo, RG2Blo, RG3W, RG4Blo, lcon(0x80), parw09, parw10, lcon(0x1));
  SKW_ROTATE_CREATURE(unsignedlong(RG61W), RG4L, RG2L);
  R_3B001(signedlong(vo_0c.peek16()), RG4L);
  SKW_QUEUE_NOISE_GEN1(lcon(0x18), RG2Blo, RG3W, RG4Blo, lcon(0x80), parw01, parw02, lcon(0xffff));
  SKW_ROTATE_SQUAD(RG1L);
  RG1L = SKW_LOCATE_OTHER_LEVEL(RG1L, UPCAST(i16, ADRESSOF(c_o32, &vo_0c)), RG3P16, lcon(0x1), NULL);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = SKW_0cee_06dc(signedlong(vo_0c.peek16()), signedlong(argw0));
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG4L) & lcon(0xff);
  SKW_QUEUE_NOISE_GEN2(RG1Blo, lcon(0x85), RG3Blo, RG5Blo, parw23, parw24, lcon(0x1), lcon(0x3a), lcon(0x80));
  SKW_LOAD_LOCALLEVEL_GRAPHICS_TABLE(signedlong(vo_0c.peek16()), RG2L, signedlong(argw0));
  SKW_LOAD_LOCALLEVEL_GRAPHICS_TABLE(signedlong(xw_24.peek16()), RG2L, RG4L);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_18.peek16()));
  SKW_QUEUE_NOISE_GEN2(lcon(0x16), RG2Blo, RG3Blo, RG4Blo, parw28, parw29, lcon(0x2), lcon(0x69), lcon(0xc8));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(vq_28.peek16()));
  SKW_482b_05bf(lcon(0x1));
void SKW_2fcf_0b8b(i32 eaxl, i32 edxl, i32 ebxl)
  RG1L = SKW_GET_TELEPORTER_DETAIL(&c12_00, RG2L, RG4L);
  RG1L = SKW_GET_TELEPORTER_DETAIL(&c12_00, RG2L, RG4L);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(vw_08));
void R_3B001(i32 eaxl, i32 edxl)
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
i32 SKW_TRY_PUSH_OBJECT_TO(i32 eaxl, i32 edxl, i32 ebxl, i16* ecxpw, i16* argpw0)
  RG1L = SKW_RAND02();
  RG1L = SKW_GET_TILE_VALUE(signedlong(RG2W), RG4L);
  RG1L = SKW_IS_TILE_BLOCKED(unsignedlong(RG1Blo));
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG2W), RG4L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(RG1L);
i32 SKW_MOVE_RECORD_TO(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG5W));
  SKW_2fcf_0234(signedlong(vq_60.peek16()), signedlong(vq_58.peek16()), signedlong(vq_68.peek16()), signedlong(argw0));
  RG1L = SKW_2fcf_0434(RG63L, RG4L, RG2L, RG3L, parl00, parl01);
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG63L, RG4L);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = SKW_IS_CREATURE_ALLOWED_ON_LEVEL(RG1L, RG4L);
  SKW_DROP_CREATURE_POSSESSION(vl_18, unsignedlong(argw0), lcon(0x0), unsignedlong(vq_68.peek16()), lcon(0x1));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(vq_84.peek16()));
  SKW_DELETE_CREATURE_RECORD(RG1L, lcon(0x2), lcon(0xffffffff), RG4L);
  SKW_LOAD_LOCALLEVEL_GRAPHICS_TABLE(signedlong(ddat.v1e0270), signedlong(vq_84.peek16()), signedlong(ddat.v1e0272));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(vq_84.peek16()));
  R_3CE7D(unsignedlong(vq_60.peek16()), unsignedlong(vq_58.peek16()), unsignedlong(RG5W), RG3L, parl05, parl06);
  SKW_CUT_RECORD_FROM(unsignedlong(RG5W), signedlong(vq_60.peek16()), signedlong(vq_58.peek16()), NULL);
  SKW_1c9a_0fcb(unsignedlong(vq_7c.peek16()));
  R_3CE7D(RG1L, RG4L, lcon(0xffff), vl_44, parl03, parl04);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = SKW_GET_CREATURE_AT(RG1L, RG4L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG1W));
  SKW_APPEND_RECORD_TO(RG1L, RG2L, RG3L, NULL);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(RG1L);
  RG1L = SKW_TRY_PUSH_OBJECT_TO(lcon(0xffff), RG1L, vl_48, &vw_4c, &vw_70);
  RG1L = SKW_GET_CREATURE_WEIGHT(vl_20);
  RG1L = (SKW_RAND16(lcon(0xa)) & lcon(0xffff)) + RG4L & lcon(0xffff);
  SKW_ATTACK_PARTY(RG1L, lcon(0x4), lcon(0x2));
  SKW_QUEUE_NOISE_GEN2(lcon(0x16), lcon(0x8a), RG3Blo, RG3Blo, parw16, parw17, lcon(0x1), lcon(0x64), lcon(0xc8));
  SKW_MOVE_RECORD_TO(lcon(0xffff), vl_2c, vl_48, signedlong(vw_4c), parw21);
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vq_68.peek16()), unsignedlong(argw0));
  RG1L = SKW_GET_CREATURE_WEIGHT(RG2L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(RG4L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(RG2L);
  RG1L = SKW_GET_CREATURE_WEIGHT(unsignedlong(xwc_50.peek16())) & lcon(0xffff);
  RG1L = SKW_TRY_PUSH_OBJECT_TO(RG1L, RG4L, RG2L, &vw_4c, &vw_70);
  SKW_MOVE_RECORD_TO(vl_0c, lcon(0xffffffff), lcon(0x0), signedlong(vw_4c), parw25);
  RG1L = SKW_TRY_PUSH_OBJECT_TO(unsignedlong(xwc_50.peek16()), signedlong(vq_68.peek16()), signedlong(argw0), &vw_4c, &vw_70);
  RG1L = SKW_GET_CREATURE_WEIGHT(RG4L) & lcon(0xffff);
  RG1L = SKW_TRY_PUSH_OBJECT_TO(unsignedlong(xwc_50.peek16()), vl_38, vl_1c, &vw_4c, &vw_70);
  SKW_1c9a_0db0(RG5L);
  SKW_2fcf_01c5(RG5L, vl_38, vl_1c, RG3L, parl23);
  RG1L = SKW_RAND16(signedlong(RG1W) / 2 + 1 & lcon(0xffff));
  SKW_ATTACK_CREATURE(RG1L, vl_04, vl_00, lcon(0x2000), lcon(0x0), parl29);
  SKW_MOVE_RECORD_TO(vl_08, vl_04, vl_00, signedlong(vw_4c), parw30);
  R_3CE7D(unsignedlong(vq_68.peek16()), RG4L, RG2L, lcon(0x0), parl31, parl32);
  SKW_APPEND_RECORD_TO(RG1L, RG2L, RG3L, NULL);
  SKW_SET_MINION_RECENT_OPEN_DOOR_LOCATION(RG5L, RG4L, RG2L, RG3L, parl33);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(RG5L);
  SKW_ALLOC_CAII_TO_CREATURE(RG5L, unsignedlong(vq_68.peek16()), unsignedlong(argw0));
  SKW_1c9a_0fcb(RG1L);
  SKW_2fcf_01c5(RG1L, RG4L, RG2L, RG3L, parl27);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  SKW_2fcf_0b8b(RG1L, vl_30, RG2L);
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(ddat.v1e0270), unsignedlong(ddat.v1e0272));
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(RG4L);
  RG1L = SKW_GET_CREATURE_WEIGHT(RG4L) & lcon(0xffff);
  RG1L = SKW_TRY_PUSH_OBJECT_TO(lcon(0xffff), vl_14, vl_30, &vw_4c, &vw_70);
  SKW_LOAD_LOCALLEVEL_GRAPHICS_TABLE(signedlong(vw_4c), vl_40, signedlong(vw_70));
  SKW_MOVE_RECORD_TO(lcon(0xffff), lcon(0xffffffff), lcon(0x0), signedlong(vw_4c), parw08);
  SKW_LOAD_LOCALLEVEL_GRAPHICS_TABLE(signedlong(ddat.v1e0270), signedlong(vq_84.peek16()), signedlong(ddat.v1e0272));
  RG1L = SKW_TRY_PUSH_OBJECT_TO(RG1L, vl_24, vl_28, &vw_4c, &vw_70);
  RG1L = SKW_GET_PARTY_SPECIAL_FORCE();
  RG1L = SKW_RAND16(signedlong(RG4W) / 2 + 1 & lcon(0xffff));
  SKW_ATTACK_CREATURE(vl_10, vl_24, vl_28, lcon(0x2000), lcon(0x0), parl11);
  SKW_MOVE_RECORD_TO(vl_10, vl_24, vl_28, signedlong(vw_4c), parw12);
  SKW_1c9a_0db0(RG5L);
  RG1L = SKW_MOVE_RECORD_TO(RG5L, RG4L, RG2L, RG3L, lcon(0x0));
  SKW_2fcf_01c5(RG5L, signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), RG3L, parl14);
  R_3CE7D(RG1L, RG4L, RG2L, RG3L, parl34, parl35);
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(vq_84.peek16()));
i32 SKW_INVOKE_MESSAGE(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl, i32 argl0)
  return SKW_QUEUE_TIMER(&c38_00);
i32 SKW_INVOKE_ACTUATOR(xxx* xeaxp, i32 ebxl, i32 edxl)
  return SKW_INVOKE_MESSAGE(unsignedlong(RG1W), RG2L, RG3L, RG4L, parl00);
i32 SKW_2fcf_164e(i32 eaxl, i32 edxl)
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L) & lcon(0xff);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(RG2L);
  RG1L = SKW_2fcf_164e(RG2L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(RG2L);
i32 SKW_FIND_DISTINCTIVE_ITEM_ON_TILE(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
  RG1L = SKW_GET_WALL_TILE_ANYITEM_RECORD(signedlong(RG1W), RG4L);
  RG1L = signedlong(CUTX16(SKW_GET_DISTINCTIVE_ITEMTYPE(RG2L)));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
i32 SKW_2fcf_16ff(i32 eaxl)
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(unsignedlong(RG1W));
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(RG5L);
  RG1L = SKW_2fcf_164e(RG5L, RG4L);
i32 SKW_TRANSFER_PLAYER(i32 eaxl, i32 ebxl, i32 edxl)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG1W), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(RG3L);
  RG1L = SKW_2fcf_164e(RG3L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG2W));
void R_3BF83(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl)
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_00.peek16()));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
  SKW_MOVE_RECORD_TO(lcon(0xffff), signedlong(ddat.v1e0270), signedlong(ddat.v1e0272), lcon(0xffffffff), lcon(0x0));
  SKW_LOAD_NEWMAP(RG5L, RG2L, lcon(0x1), RG7L);
  SKW_MOVE_RECORD_TO(lcon(0xffff), RG4L, RG2L, RG3L, parw01);
  SKW_ROTATE_SQUAD(signedlong(xw_04.peek16()));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
i32 SKW_LOAD_GDAT_INTERFACE_00_0A(void)
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(ddat.v1e0270), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG5W));
  RG1L = R_3D93B(RG1L, RG4L, RG2L, RG3P16, &vw_00);
  R_3BF83(signedlong(vw_04), signedlong(vw_00), RG2L, RG3L);
  SKW_QUEUE_NOISE_GEN2(lcon(0xa), lcon(0x88), lcon(0xfe), RG4Blo, parw01, parw02, lcon(0x1), lcon(0x8c), lcon(0x80));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG5W));
void R_3C1E5(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0)
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(vql_0c.peek16()), RG4L);
  RG1L = SKW_IS_WALL_ORNATE_ALCOVE_FROM_RECORD(unsignedlong(xwc_28.peek16()), RG3L);
  RG1L = SKW_0cee_319e(RG3L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG3L);
  RG1L = SKW_LOCATE_OTHER_LEVEL(signedlong(ddat.v1e0266), &vw_3c, &vw_38, RG4L, NULL);
  R_3BF83(signedlong(vw_3c), RG4L, RG2L, RG3L);
  SKW_1031_098e();
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(unsignedlong(RG6W));
  RG1L = SKW_FIND_DISTINCTIVE_ITEM_ON_TILE(unsignedlong(vql_0c.peek16()), RG2L, RG5L, RG4L);
  RG1L = SKW_ALLOC_NEW_DBITEM(RG5L);
  SKW_APPEND_RECORD_TO(RG1L, RG2L, RG3L, NULL);
  SKW_ADD_ITEM_CHARGE(RG5L, lcon(0xf));
  SKW_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, lcon(0x0));
  SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_10)));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG3L);
  RG1L = SKW_GET_WALL_DECORATION_OF_ACTUATOR(RG52p);
  RG1L = SKW_ADD_ITEM_CHARGE(unsignedlong(RG6W), RG4L);
  RG1L = signedlong(CUTX16(SKW_GET_DISTINCTIVE_ITEMTYPE(unsignedlong(RG6W))));
  RG1L = SKW_REMOVE_OBJECT_FROM_HAND() & lcon(0xffff);
  SKW_DEALLOC_RECORD(RG1L);
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(unsignedlong(RG6W));
  RG1L = SKW_FIND_DISTINCTIVE_ITEM_ON_TILE(unsignedlong(vql_0c.peek16()), RG2L, RG5L, RG4L);
  RG1L = SKW_ALLOC_NEW_DBITEM(RG5L);
  SKW_APPEND_RECORD_TO(RG1L, RG2L, RG3L, RG4P);
  SKW_ADD_ITEM_CHARGE(RG5L, lcon(0xf));
  SKW_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, lcon(0x0));
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(unsignedlong(RG6W));
  R_3BF83(signedlong(vql_54.peek16()), RG4L, RG2L, RG3L);
  SKW_1031_098e();
  RG1P = SKW_GET_ADDRESS_OF_TILE_RECORD(signedlong(RG1W), RG4L);
  SKW_SELECT_CHAMPION(signedlong(ddat.v1e0270), RG2L, RG3L, RG4L);
  RG1L = SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_10)));
  SKW_QUEUE_NOISE_GEN2(lcon(0x9), lcon(0x88), lcon(0xfe), vb_58, parw07, parw08, parw09, lcon(0x8c), lcon(0x80));
  SKW_INVOKE_ACTUATOR(RG52p, signedlong(xw_40.peek16()), lcon(0x0));
  SKW_QUEUE_NOISE_GEN2(lcon(0x9), lcon(0x88), lcon(0xfe), vb_58, parw14, parw15, parw16, lcon(0x8c), lcon(0x80));
  SKW_INVOKE_ACTUATOR(RG1P, RG2L, RG4L);
  SKW_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw13);
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(RG2L, RG4L);
  RG1L = signedlong(CUTX16(SKW_GET_DISTINCTIVE_ITEMTYPE(RG2L)));
  SKW_SET_ITEMTYPE(RG2L, lcon(0xf));
  SKW_ADD_ITEM_CHARGE(RG2L, lcon(0xf));
  SKW_QUEUE_NOISE_GEN2(lcon(0x16), lcon(0x83), lcon(0xfe), ddat.hero[RG1L / 263].b_101, parw02, parw03, lcon(0x0), lcon(0x96), lcon(0x80));
  SKW_MOVE_RECORD_TO(RG1L, lcon(0xffffffff), lcon(0x0), RG3L, parw01);
  RG1L = SKW_GET_WALL_TILE_ANYITEM_RECORD(signedlong(vql_0c.peek16()), signedlong(vql_08.peek16()));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG1W));
  SKW_MOVE_RECORD_TO(RG1L, RG4L, RG2L, lcon(0xffffffff), lcon(0x0));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_48.peek16()));
  RG1L = SKW_REMOVE_OBJECT_FROM_HAND();
  SKW_TAKE_OBJECT(RG1L, lcon(0x1));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(xwc_00.peek16()));
  SKW_APPEND_RECORD_TO(unsignedlong(mkuw(argl0)), lcon(0xffffffff), RG3L, RG53p + lcon(0x2));
  SKW_APPEND_RECORD_TO(RG1L, lcon(0xffffffff), lcon(0x0), RG4P);
void R_3CE7D(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i32 argl1)
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(unsignedlong(RG2W));
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vql_0c.peek16()), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG1P, lcon(0x4))));
  SKW_CUT_RECORD_FROM(RG1L, RG2L, RG3L, RG4P);
  RG62L = SKW_GET_TILE_RECORD_LINK(signedlong(vql_0c.peek16()), signedlong(vql_08.peek16()));
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(RG1L);
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(unsignedlong(RG62W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG62W));
  RG1L = SKW_IS_OBJECT_FLOATING(RG5L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(RG5L);
  RG1P = DOWNCAST(i16, SKW_1c9a_06bd(RG5L, RG4L, RG2L));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  SKW_QUERY_MESSAGE_TEXT(tarr_10, RG2L, RG4L);
  SKW_DISPLAY_HINT_TEXT(lcon(0xf), tarr_10);
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(unsignedlong(RG62W));
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(unsignedlong(RG62W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG62W));
  SKW_APPEND_RECORD_TO(RG1L, RG2L, RG3L, RG4P);
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(vql_0c.peek16()), signedlong(vql_08.peek16()));
  RG1L = SKW_IS_WALL_ORNATE_SPRING(unsignedlong(RG63W));
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG2L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG2L);
  RG1L = SKW_ADD_ITEM_CHARGE(RG2L, lcon(0x0));
  SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_d8)));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1L = SKW_RAND16(lcon(0x64)) & lcon(0xffff);
  SKW_INVOKE_MESSAGE(RG1L, lcon(0x0), lcon(0x0), RG5L, parl05);
  SKW_INVOKE_MESSAGE(vl_f8, RG2L, RG3L, RG4L, parl06);
  RG1L = SKW_MAX_LOAD(DOWNCAST(c_hero, &ddat.hero[RG1L / 263]));
  RG1L = SKW_GET_PLAYER_WEIGHT(unsignedlong(RG3W));
  RG4L = SKW_MIN16(lcon(0x5a), signedlong(RG1W));
  RG1L = SKW_RAND16(lcon(0x64)) & lcon(0xffff);
  SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_d8)));
  RG1L = SKW_RAND02();
  SKW_QUEUE_NOISE_GEN2(lcon(0x16), RG2Blo, RG3Blo, RG4Blo, parw07, parw08, lcon(0x1), lcon(0x69), lcon(0xc8));
  SKW_QUEUE_NOISE_GEN1(lcon(0xa), RG2Blo, RG3W, RG4Blo, lcon(0x80), parw13, parw14, lcon(0x1));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1L = SKW_GET_WALL_DECORATION_OF_ACTUATOR(RG5p);
  RG1L = SKW_2fcf_16ff(signedlong(RG1W));
  RG1L = SKW_GET_WALL_DECORATION_OF_ACTUATOR(RG5p);
  RG1L = SKW_GET_FLOOR_DECORATION_OF_ACTUATOR(RG5p);
  SKW_QUEUE_NOISE_GEN2(vb_118, RG2Blo, RG3Blo, RG4Blo, parw00, parw01, lcon(0x1), lcon(0x8c), lcon(0x80));
  SKW_INVOKE_ACTUATOR(RG1P, RG2L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG63W));
i32 R_3D93B(i32 eaxl, i32 edxl, i32 ebxl, i16* ecxpw, i16* argpw0)
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG4W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG4W));
i32 SKW_QUERY_CREATURE_5x5_POS(xxx* xeaxp, i32 edxl)
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xf), lcon(0x7), lcon(0xfd), byte_at(RG1P, lcon(0x4))) ? 1 : 0;
  RG1P = DOWNCAST(i16, SKW_1c9a_02c3(RG5p, SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG5p, lcon(0x4))))));
  R_4DA3(unsignedlong(byte_at(RG5p, lcon(0x4))), UPCAST(i16, RG1P + lcon(0x2)), UPCAST(s_8bytearray, ADRESSOF(s_8bytearray, &s09_00)), unsignedlong(word_at(RG1P)));
  RG1W = SKW_ROTATE_5x5_POS(s09_00.barr_00[0x4], t_rotate(RG4UW));
i32 SKW_32cb_00f1(i32 eaxl, i32 ebxl, i32 edxl)
  RG1L = SKW_PT_IN_RECT(&ddat.v1e118e.rc_24, RG4W, RG2W) ? 1 : 0;
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_PICT_BITS(&ddat.v1e118e.c30_00));
  RG1L = signedlong(CUTX16(SKW_0b36_1446(&ddat.v1e118e.c30_00.xp_00, RG2L, RG3L, RG4L))); // NFIX
i32 SKW_32cb_01b6(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl, i16* argpw0)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG2W), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  SKW_QUERY_CREATURE_PICST(lcon(0x3), RG2P, RG3P, signedlong(table1d6b15[3]), parw00);
  SKW_DRAW_TEMP_PICST();
  RG1L = SKW_32cb_00f1(signedlong(RG6w), RG2L, RG4L);
i32 SKW_32cb_0287(i32 eaxl, i32 edxl, i32 ebxl)
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x48));
  SKW_CALC_VECTOR_W_DIR(signedlong(ddat.v1e12c8), signedlong(table1d6ad0[RG5L][0]), &vw_14, signedlong(table1d6ad0[RG5L][1]), &vw_08);
  SKW_SUMMARIZE_STONE_ROOM(UPCAST(s_8wordarray, RG4P + lcon(0x2)), RG2L, RG3L, signedlong(ddat.v1e12c8));
  RG1L = SKW_32cb_15b8(RG5L, RG2L, RG4L);
  vl_00 = SKW_32cb_00f1(signedlong(xw_10.peek16()), RG2L, signedlong(xw_0c.peek16()));
  DM2_DEALLOC_LOBIGPOOL(lcon(0x48));
i32 SKW_32cb_03a6(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1, i16 argw2, i32 argl3)
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(RG3W), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(SKW_GET_CREATURE_AT(unsignedlong(RG1W), RG4L) & lcon(0xffff));
  RG1L = SKW_IS_CONTAINER_CHEST(vl_0c);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(vl_0c);
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x14), lcon(0x1), lcon(0x48), CUTLX8(RG4L)) ? 1 : 0;
  RG1P = SKW_GET_ADDRESS_OF_RECORD(vl_0c);
  SKW_DRAW_ITEM(RG1L, lcon(0x0), RG3L, RG4L, parw00, xparp01, lcon(0x0), parw03, parl04);
  RG1L = SKW_32cb_00f1(RG1L, RG2L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  RG1L = SKW_QUERY_GDAT_DBSPEC_WORD_VALUE(unsignedlong(mkuw(argl1)), lcon(0x4)); // truncated TODO check if arg is always word
  RG1L = SKW_GET_NEXT_RECORD_LINK(RG1L);
  SKW_APPEND_RECORD_TO(unsignedlong(mkw(argl1)), lcon(0xffffffff), lcon(0x0), RG1P);
  SKW_CUT_RECORD_FROM(RG72L, lcon(0xffffffff), lcon(0x0), RG1P);
  SKW_TAKE_OBJECT(RG72L, lcon(0x1));
t_colidx* SKW_32cb_0649(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
  RG1P = DOWNCAST(t_colidx, SKW_QUERY_GDAT_IMAGE_LOCALPAL(RG1Blo, CUTLX8(RG2L), CUTLX8(RG4L)));
  SKW_COPY_MEMORY(RG1P, PAL16, DOWNCAST(t_colidx, ddat.v1e12dc));
  return R_B073(ddat.v1e12dc, RG2L, lcon(0xffffffff), unsignedlong(CUTX8(ddat.v1e12d2)), &vw_00);
void R_3E39E(i32 eaxl, i32 edxl, i32 ebxl, xxx* xecxp)
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(RG1Blo, CUTLX8(RG2L), CUTLX8(RG4L)));
  SKW_COPY_MEMORY(RG1P, unsignedlong(CUTX16(SKW_CALC_IMAGE_BYTE_LENGTH(UPCAST(t_gfxdata, RG1P)))), RG5p); // attention: two times RG1P
void SKW_DRAW_DUNGEON_GRAPHIC(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl, i16 argw0, i32 argl1)
  SKW_QUERY_GDAT_SUMMARY_IMAGE(UPCAST(c_image, ADRESSOF(c_image, &c37_00)), RG2L, RG3L, RG4L);
  R_B073(c37_00.carr_3a, signedlong(argw0), lcon(0xffffffff), unsignedlong(CUTX8(ddat.v1e12d2)), &c37_00.w_38);
  SKW_DRAW_PICST(XUPCAST(c_image, SKW_QUERY_PICST_IT(UPCAST(c_image, ADRESSOF(c_image, &c37_00)))));
void SKW_32cb_0804(t_colidx* eaxpcarr, i32 ebxl, i32 ecxl, i32 edxl, i16* argpw0)
  RG1L = SKW_MAX16(signedlong(CUTX16(-RG1L)), RG4L);
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x8), lcon(0x7), CUTLX8(RG6L), ddat.v1d6c02) ? 1 : 0;
  SKW_0b36_037e(eaxpcarr, unsignedlong(ddat.v1d6c02), RG6L, lcon(0x8), parb00, parw01, parw02, argpw0);
  R_B073(eaxpcarr, RG2L, RG3L, RG4L, argpw0);
void SKW_QUERY_TEMP_PICST(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl, i32 argl0, i16 argw1, i32 argl2, i32 argl3, i16 argw4, i16 argw5, i8 argb6, i8 argb7, i8 argb8)
  SKW_QUERY_GDAT_SUMMARY_IMAGE(&ddat.v1e118e, unsignedlong(argb7), unsignedlong(argb8), unsignedlong(argb6));
  SKW_32cb_0804(ddat.v1e118e.carr_3a, signedlong(argw4), signedlong(argw5), signedlong(argw1), &ddat.v1e118e.w_38);
  SKW_QUERY_PICST_IT(&ddat.v1e118e);
void SKW_DRAW_TEMP_PICST(void)
  SKW_DRAW_PICST(&ddat.v1e118e);
i32 SKW_QUERY_MULTILAYERS_PIC(c_image* eaxp_c37, i32 ebxl, i32 ecxl, i32 edxl, i16 argw0, i16 argw1, i16 argw2, i16 argw3, i16 argw4, i32 argl5)
  SKW_QUERY_TEMP_PICST(unsignedlong(argw3), signedlong(argw1), lcon(0x0), signedlong(argw0), parl00, parw01, parl02, parl03, parw04, lcon(0xffff), parb06, parb07, parb08);
  SKW_COPY_MEMORY(DOWNCAST(c_image, &ddat.v1e118e), lcon(0x13a), RG5p);
  SKW_0b36_018f(UPCAST(c_hex18, RG5p));
  SKW_SET_ORIGIN_RECT(RG1R, RG4W, RG2W);
  RG1W = SKW_3e74_5888();
  RG1P = DOWNCAST(t_gfxdata, SKW_ALLOC_NEW_PICT(RG1W, CUTX16(RG2L), lcon(0x8), CUTX16(RG4L)));
void SKW_32cb_0a4c(c_rect* eaxrp, i32 ebxl, i32 edxl)
  SKW_COPY_RECT(RG1R, RG4R);
void SKW_DRAW_RAIN(void)
  RG1R = SKW_QUERY_BLIT_RECT(NULL, RG2W, RG3P16, RG4R, parpw00, lcon(0xffffffff));
  SKW_QUERY_RAINFALL_PARAM(ADRESSOF(i8, &vb_14), &vw_0c);
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x17), CUTLX8(RG2L), CUTLX8(RG4L)));
  SKW_3e74_58bf(lcon(0x17), vb_14, ddat.v1d6c02);
  RG1W = SKW_3e74_5888();
  SKW_ALLOC_NEW_PICT(CUTX16(RG5L), CUTX16(RG2L), lcon(0x4), CUTX16(RG4L));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x17), CUTLX8(RG2L), CUTLX8(RG4L)));
  SKW_DRAW_MIRRORED_PIC(UPCAST(t_gfxdata, RG1P), SKW_3e74_5817(CUTX16(RG5L)));
  RG51p = DOWNCAST(t_gfxdata, SKW_3e74_5817(CUTX16(RG5L)));
  parpcarr06 = SKW_32cb_0649(lcon(0x17), RG2L, lcon(0x0), RG4L);
  RG1L = SKW_RAND();
  RG1L = SKW_RAND();
  SKW_44c8_20a4(RG51p, NULL, RG3R, ddat.bitmapptr, parw02, parw03, parw04, lcon(0x0), parpcarr06); // BUGFIX Phase P1
  SKW_3e74_58a3(CUTX16(RG1L));
  SKW_3e74_5992(lcon(0x17), vb_14, ddat.v1d6c02);
void SKW_32cb_0c7d(c_image* eaxp_c37, i32 ebxl, i32 edxl)
  SKW_FILL_ENTIRE_PICT(SKW_3e74_5817(xw_08.peek16()), RG4Blo);
  SKW_QUERY_RAINFALL_PARAM(ADRESSOF(i8, &vb_1c), &vw_04);
  SKW_QUERY_GDAT_IMAGE_METRICS(lcon(0x17), unsignedlong(vb_1c), &vw_14, unsignedlong(ddat.v1d6c02), &vw_0c);
  RG1W = SKW_3e74_5888();
  SKW_ALLOC_NEW_PICT(CUTX16(RG1L), CUTX16(RG2L), CUTLX8(RG3L), CUTX16(RG4L));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x17), CUTLX8(RG2L), CUTLX8(RG4L)));
  RG1R = SKW_ALLOC_TEMP_ORIGIN_RECT(RG1W, word_at(RG61p, -lcon(0x2)));
  SKW_FIRE_BLIT_PICTURE(UPCAST(t_gfxdata, RG61p), RG2R, lcon(0x0), SKW_3e74_5817(CUTX16(RG1L)), lcon(0x0), parw02, parw03, lcon(0xffff), parw05, lcon(0x4), lcon(0x4), NULL);
  RG62p = DOWNCAST(t_gfxdata, SKW_3e74_5817(CUTX16(vl_00)));
  parpcarr13 = SKW_32cb_0649(lcon(0x17), RG2L, RG3L, RG4L);
  RG1L = signedlong(CUTX16(SKW_RAND16(RG3L - lcon(0x3c) & lcon(0xffff))));
  RG1L = SKW_RAND();
  SKW_44c8_20a4(RG62p, NULL, RG3R, SKW_3e74_5817(xw_08.peek16()), parw09, parw10, parw11, lcon(0xffff), parpcarr13);
  SKW_3e74_58a3(xw_18.peek16());
void SKW_DRAW_TEXT_TO_BACKBUFF(i32 eaxl, t_text* ebxtp, i32 edxl)
  RG1L = SKW_QUERY_STR_METRICS(RG5t, RG2P16, RG4P16) ? 1 : 0;
  SKW_COPY_MEMORY(DOWNCAST(t_colidx, ddat.paletteptr1), PAL16, DOWNCAST(t_colidx, vcarr_00));
  R_B073(vcarr_00, RG3L, RG3L, RG4L, &vw_10);
  SKW_DRAW_STRONG_TEXT(UPCAST(t_gfxdata, RG1P), RG2W, RG3W, word_at(RG1P, -lcon(0x4)), parw01, CUTX16(parl02), par_tp03);
i32 SKW_IS_DISTINCTIVE_ITEM_ON_ACTUATOR(xxx* xeaxp, i32 ebxl, i32 edxl)
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(RG1W), RG4L);
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(unsignedlong(RG2W));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG2W));
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG2W));
void SKW_32cb_0f82(xxx* xeaxp, i32 ebxl, i32 ecxl, i32 edxl, i16 argw0, i16 argw1, i16 argw2, i16 argw3, i16 argw4)
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(RG1W), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG2W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG2W));
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x40));
  RG1L = SKW_QUERY_CREATURES_ITEM_MASK(unsignedlong(vb_4c), RG1P, lcon(0x0), RG4L);
  RG1P = SKW_QUERY_GDAT_ENTRY_DATA_BUFF(lcon(0x9), CUTLX8(RG2L), CUTLX8(RG3L), CUTLX8(RG4L));
  RG1L = (SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(lcon(0x9), lcon(0x8), CUTLX8(RG3L), CUTLX8(vl_18)) & lcon(0xffff)) / 2;
  RG1L = SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG2L);
  RG1L = lextendedDM2_QUERY_GDAT_ENTRY_DATA_INDEX(CUTLX8(SKW_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(RG2L)) & bcon(0xff), lcon(0xb), lcon(0x0), CUTLX8(RG4L));
  RG1L = SKW_IS_DISTINCTIVE_ITEM_ON_ACTUATOR(RG51p, RG2L, RG4L);
  RG1L = SKW_IS_DISTINCTIVE_ITEM_ON_ACTUATOR(RG61p, RG2L, RG4L);
  RG1L = SKW_2fcf_16ff(RG3L);
  DM2_DEALLOC_LOBIGPOOL(lcon(0x40));
  RG1L = SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG4L) & lcon(0xff);
  RG1L = SKW_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(RG4L) & lcon(0xff);
  SKW_QUERY_TEMP_PICST(lcon(0x0), signedlong(argw1), RG3L, signedlong(argw0), parl00, parw01, parl02, parl03, parw04, lcon(0xffff), parb06, parb07, lcon(0x0));
  SKW_DRAW_TEMP_PICST();
  RG1L = SKW_IS_DISTINCTIVE_ITEM_ON_ACTUATOR(RG51p, RG2L, RG4L);
  RG1L = SKW_IS_DISTINCTIVE_ITEM_ON_ACTUATOR(RG61p, RG2L, RG4L);
  RG1P = SKW_QUERY_GDAT_ENTRY_DATA_BUFF(lcon(0x9), CUTLX8(RG2L), CUTLX8(RG3L), CUTLX8(RG4L));
  RG1L = (SKW_QUERY_GDAT_ENTRY_DATA_LENGTH(lcon(0x9), CUTLX8(RG2L), CUTLX8(RG3L), CUTLX8(RG4L)) & lcon(0xffff)) / 2;
  RG1L = SKW_ALLOC_NEW_DBITEM_DEEPLY(unsignedlong(vq_3c.peek16()));
  SKW_APPEND_RECORD_TO(RG1L, RG2L, RG3L, NULL);
  SKW_QUERY_TEMP_PICST(lcon(0x0), signedlong(argw1), RG3L, signedlong(argw0), parl09, parw10, parl11, parl12, parw13, lcon(0xffff), lcon(0x9), parb16, lcon(0x11));
  SKW_DRAW_TEMP_PICST();
  SKW_QUERY_TEMP_PICST(lcon(0x0), signedlong(argw1), lcon(0x0), signedlong(argw0), lcon(0x0), parw19, parl20, parl21, parw22, lcon(0xffff), lcon(0x9), parb25, lcon(0xf));
  SKW_DRAW_TEMP_PICST();
  SKW_QUERY_TOPLEFT_OF_RECT(RG73W, &vw_20, &vw_24);
  RG1L = SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(RG4L);
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_ITEM_NAME(SKW_QUERY_ITEMDB_FROM_DISTINCTIVE_ITEMTYPE(RG4L) & lcon(0xff), RG2L));
  SKW_DRAW_TEXT_TO_BACKBUFF(signedlong(vw_20), RG1T, signedlong(vw_24));
  RG1L = SKW_48ae_05ae(RG74l, RG2L, lcon(0x1), RG64L, parl27, parl28);
  RG1L = SKW_48ae_05ae(RG74l, RG2L, RG3L, RG4L, parl29, parl30);
  SKW_48ae_0767(RG1L, UPCAST(i16, ADRESSOF(i8, vba_08)), ADRESSOF(i16, &vw_2c), lcon(0x6)); // parl31 unused
  RG1W = SKW_3e74_5888();
  SKW_ALLOC_NEW_PICT(CUTX16(RG5L), lcon(0x7), lcon(0x4), CUTX16(RG4L));
  SKW_QUERY_TEMP_PICST(lcon(0x0), RG2L, RG3L, RG2L, lcon(0x0), parw33, parl34, parl35, lcon(0xffff), lcon(0xffff), lcon(0x9), parb39, lcon(0x10));
  RG52p = DOWNCAST(t_gfxdata, SKW_3e74_5817(CUTX16(RG5L)));
  SKW_SET_ORIGIN_RECT(&rc_00, lcon(0x8), RG2W);
  RG1R = SKW_QUERY_BLIT_RECT(UPCAST(t_gfxdata, RG52p), RG2W, RG3P16, RG4R, parpw44, parw45);
  SKW_DRAW_DIALOGUE_PICT(UPCAST(t_gfxdata, RG1P), RG2R, RG3W, UPCAST(t_gfxdata, RG4P), lcon(0x0), parw47, ddat.v1e118e.carr_3a);
  SKW_3e74_58a3(xw_30.peek16());
  SKW_DRAW_DIALOGUE_PICT(UPCAST(t_gfxdata, RG1P), RG2R, RG3W, UPCAST(t_gfxdata, RG52p), lcon(0x0), lcon(0xffff), NULL);
i32 SKW_32cb_15b8(i32 eaxl, i32 ebxl, i32 edxl)
  RG1L = SKW_IS_WALL_ORNATE_ALCOVE(RG4L);
  RG1W = SKW_098d_0cd7(RG1W, RG5W != wcon(0x0), RG4W);
  SKW_QUERY_TEMP_PICST(RG1L, RG4L, RG3L, RG4L, parl00, parw01, parl02, parl03, lcon(0xffff), lcon(0xffff), lcon(0x16), parb07, lcon(0x1));
  SKW_DRAW_TEMP_PICST();
  SKW_32cb_0a4c(RG1R, RG2L, RG4L);
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x8), lcon(0x1), lcon(0xfe), ddat.v1d6c02) ? 1 : 0;
  SKW_QUERY_TEMP_PICST(RG1L, RG2L, lcon(0x0), RG4L, lcon(0x0), parw10, parl11, parl12, parw13, lcon(0xffff), lcon(0x8), parb16, parb17);
  SKW_DRAW_TEMP_PICST();
  SKW_QUERY_GDAT_SUMMARY_IMAGE(UPCAST(c_image, ADRESSOF(c_image, &c37_04)), RG2L, RG3L, lcon(0x8));
  SKW_QUERY_GDAT_IMAGE_METRICS(lcon(0x8), RG5L, &vw_314, unsignedlong(ddat.v1d6c02), &vw_304);
  RG1W = SKW_3e74_5888();
  SKW_QUERY_MESSAGE_TEXT(tarr_27c, RG2L, RG4L);
  RG1P = DOWNCAST(t_gfxdata, SKW_ALLOC_NEW_PICT(CUTX16(RG5L), CUTX16(RG2L), CUTLX8(RG3L), CUTX16(RG4L)));
  SKW_FILL_ENTIRE_PICT(UPCAST(t_gfxdata, RG1P), CUTX8(vq_324.peek16()));
  pgarr_2ec = SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0x8), CUTLX8(RG2L), CUTLX8(RG4L));
  RG72p = DOWNCAST(t_gfxdata, SKW_3e74_5817(CUTX16(RG5L)));
  parpcarr21 = SKW_QUERY_GDAT_IMAGE_LOCALPAL(lcon(0x8), CUTLX8(RG2L), CUTLX8(RG4L));
  RG1L = SKW_SKCHR_TO_SCRIPTCHR(RG1L);
  SKW_DRAW_DIALOGUE_PICT(pgarr_2ec, RG2R, RG3W, UPCAST(t_gfxdata, RG4P), lcon(0x0), parw20, parpcarr21);
  SKW_QUERY_GDAT_SUMMARY_IMAGE(UPCAST(c_image, ADRESSOF(c_image, &c37_04)), lcon(0x0), lcon(0x0), lcon(0xff));
  SKW_COPY_MEMORY(DOWNCAST(t_colidx, SKW_QUERY_GDAT_IMAGE_LOCALPAL(lcon(0x8), lcon(0x3), ddat.v1d6c02)), PAL16, DOWNCAST(t_colidx, c37_04.carr_3a));
  SKW_32cb_0804(c37_04.carr_3a, signedlong(vq_324.peek16()), lcon(0xffffffff), signedlong(xw_310.peek16()), &c37_04.w_38);
  SKW_DRAW_PICST(XUPCAST(c_image, SKW_QUERY_PICST_IT(UPCAST(c_image, ADRESSOF(c_image, &c37_04)))));
  SKW_3e74_58a3(xw_328.peek16());
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x9), lcon(0x1), CUTLX8(RG3L), vb_344) ? 1 : 0;
  SKW_QUERY_TEMP_PICST(RG1L, vl_2e0, lcon(0x0), vl_2e8, lcon(0x0), parw24, parl25, parl26, lcon(0xfffd), lcon(0xfffd), lcon(0x9), parb30, parb31);
  SKW_QUERY_TEMP_PICST(vl_2e4, vl_2e0, signedlong(ddat.v1e118e.w_1c), vl_2e8, parl32, parw33, parl34, parl35, parw36, lcon(0xfffff), lcon(0x9), parb39, lcon(0xc8));
  SKW_DRAW_TEMP_PICST();
  RG1L = signedlong(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0x9), CUTLX8(RG2L), CUTLX8(RG3L), CUTLX8(RG4L)));
  RG1L = SKW_QUERY_MULTILAYERS_PIC(&c37_140, RG2L, RG3L, RG4L, parw41, parw42, parw43, parw44, parw45, parl46);
  SKW_32cb_0c7d(UPCAST(c_image, ADRESSOF(c_image, &c37_140)), vl_2e8, RG3L);
  c37_140.pgarr_2c = SKW_3e74_5817(CUTX16(RG3L));
  SKW_DRAW_PICST(UPCAST(c_image, ADRESSOF(c_image, &c37_140)));
  SKW_0b36_01cd(UPCAST(c_hex18, ADRESSOF(c_hex18, &c37_140.c30_00)));
  SKW_0b36_00c3(RG2L, UPCAST(c_hex18, ADRESSOF(c_image, &c37_140)));
  SKW_DRAW_PICST(UPCAST(c_image, ADRESSOF(c_image, &c37_140)));
  SKW_3e74_58a3(CUTX16(RG2L));
  SKW_QUERY_TEMP_PICST(RG1L, RG2L, lcon(0x0), RG4L, lcon(0x0), parw48, parl49, parl50, parw51, lcon(0xffff), lcon(0x9), parb54, parb55);
  SKW_DRAW_TEMP_PICST();
  SKW_32cb_0a4c(RG1R, RG2L, RG4L);
  SKW_COPY_MEMORY(DOWNCAST(c_image, &ddat.v1e118e), lcon(0x13a), DOWNCAST(c_image, &ddat.v1e1054));
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x9), lcon(0x1), lcon(0xf), CUTLX8(RG5L)) ? 1 : 0;
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(ddat.ptr1e1044, 18 * RG3L + lcon(0x10))));
  SKW_32cb_0f82(RG1P, RG2L, RG3L, RG5L, parw56, parw57, parw58, parw59, parw60);
  SKW_QUERY_TEMP_PICST(RG1L, RG2L, lcon(0x0), RG4L, lcon(0x0), parw62, parl63, parl64, parw65, lcon(0xffff), lcon(0x9), parb68, lcon(0xf));
  SKW_DRAW_TEMP_PICST();
  SKW_32cb_3f0d(signedlong(vql_00.peek16()));
i32 SKW_DRAW_EXTERNAL_TILE(i32 eaxl)
  RG1L = SKW_GET_TELEPORTER_DETAIL(&c12_140, RG2L, RG4L);
  RG1W = SKW_QUERY_CREATURE_BLIT_RECTI(signedlong(vql_00.peek16()), t_rotate(0), RG4W);
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xa), lcon(0x1), vb_18c, vb_190) ? 1 : 0;
  SKW_QUERY_TEMP_PICST(RG1L, RG2L, RG3L, RG4L, parl00, parw01, parl02, parl03, lcon(0xfffd), lcon(0xfffd), lcon(0xa), parb07, parb08);
  SKW_QUERY_TEMP_PICST(RG6L, RG5L, RG3L, RG5L, parl09, parw10, parl11, parl12, parw13, lcon(0xffff), lcon(0xa), parb16, parb17);
  SKW_DRAW_TEMP_PICST();
  SKW_CHANGE_CURRENT_MAP_TO(unsignedlong(c12_140.b_04));
  SKW_CALC_VECTOR_W_DIR(signedlong(ddat.v1e12c8), lcon(0x0), &vw_168, RG4L, &vw_180);
  SKW_32cb_4185(signedlong(vw_168), signedlong(table1d6c19[RG1L]), RG3L, signedlong(vw_180));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_160.peek16()));
  SKW_DRAW_STATIC_OBJECT(signedlong(vql_00.peek16()), lcon(0x1), lcon(0x3ff));
  SKW_DRAW_STATIC_OBJECT(RG1L, lcon(0x1), lcon(0x1ffffff));
  RG1L = signedlong(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(lcon(0xa), CUTLX8(RG2L), CUTLX8(RG3L), CUTLX8(RG4L)));
  RG1L = SKW_QUERY_MULTILAYERS_PIC(&c37_04, RG2L, RG3L, RG4L, parw19, parw20, parw21, parw22, parw23, parl24);
  SKW_FILL_ENTIRE_PICT(SKW_3e74_5817(CUTX16(RG1L)), RG4Blo);
  SKW_32cb_0c7d(UPCAST(c_image, ADRESSOF(c_image, &c37_04)), RG2L, RG4L);
  c37_04.pgarr_2c = SKW_3e74_5817(CUTX16(RG2L));
  SKW_DRAW_PICST(UPCAST(c_image, ADRESSOF(c_image, &c37_04)));
  SKW_0b36_01cd(UPCAST(c_hex18, ADRESSOF(c_hex18, &c37_04.c30_00)));
  SKW_0b36_00c3(RG2L, UPCAST(c_hex18, ADRESSOF(c_image, &c37_04)));
  SKW_DRAW_PICST(UPCAST(c_image, ADRESSOF(c_image, &c37_04)));
  SKW_3e74_58a3(CUTX16(RG2L));
  SKW_QUERY_TEMP_PICST(unsignedlong(vo_178.peek16()), RG4L, lcon(0x0), RG4L, lcon(0x0), parw26, parl27, parl28, parw29, lcon(0xffff), lcon(0xa), parb32, parb33);
  SKW_DRAW_TEMP_PICST();
void SKW_DRAW_PIT_ROOF(i32 eaxl)
  RG1L = SKW_LOCATE_OTHER_LEVEL(signedlong(ddat.v1d3248), &vw_00, &vw_04, lcon(0xffffffff), NULL);
  RG1L = SKW_SET_GRAPHICS_FLIP_FROM_POSITION(lcon(0x20), signedlong(ddat.v1e12cc), signedlong(ddat.v1e12ca), signedlong(ddat.v1e12c8));
  SKW_DRAW_DUNGEON_GRAPHIC(lcon(0x8), RG2L, RG3L, RG4L, parw01, parl02);
void SKW_DRAW_PIT_TILE(i32 eaxl)
  RG1L = SKW_SET_GRAPHICS_FLIP_FROM_POSITION(lcon(0x1), signedlong(ddat.v1e12cc), signedlong(ddat.v1e12ca), signedlong(ddat.v1e12c8));
  SKW_DRAW_DUNGEON_GRAPHIC(lcon(0x8), RG2L, RG3L, unsignedlong(vb_04), parw00, parl01);
void SKW_DRAW_TELEPORTER_TILE(i32 eaxl, i32 ebxl, i32 edxl)
  RG1R = SKW_QUERY_BLIT_RECT(NULL, RG2W, RG3P16, RG4R, parpw00, lcon(0xffffffff));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_PICST_IMAGE(UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_00))));
  SKW_0b36_018f(UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_00)));
  RG1W = SKW_3e74_5888();
  RG71p = DOWNCAST(t_gfxdata, SKW_ALLOC_NEW_PICT(CUTX16(RG1L), CUTX16(RG2L), lcon(0x4), CUTX16(RG4L)));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_PICT_BITS(UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_00))));
  RG1R = SKW_ALLOC_TEMP_ORIGIN_RECT(RG1W, RG4W);
  SKW_FIRE_BLIT_PICTURE(UPCAST(t_gfxdata, RG6p), RG2R, RG3W, UPCAST(t_gfxdata, RG71p), lcon(0x0), parw03, parw04, lcon(0xffff), lcon(0x1), lcon(0x4), lcon(0x4), NULL);
  SKW_0b36_01cd(UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_00)));
  SKW_0b36_00c3(signedlong(xw_34.peek16()), UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_00)));
  SKW_0b36_018f(UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_00)));
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(vb_44, lcon(0x14), CUTLX8(RG4L)));
  RG6p = DOWNCAST(t_gfxdata, SKW_QUERY_PICT_BITS(UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_00))));
  parpcarr14 = SKW_32cb_0649(unsignedlong(vb_44), lcon(0x14), vl_20, unsignedlong(vb_48));
  RG1L = SKW_RAND();
  RG1L = signedlong(CUTX16(16 * (SKW_RAND01() + unsignedlong(byte_at(RG5p)))));
  SKW_44c8_20a4(RG72p, RG6p, &rc_18, ddat.bitmapptr, parw10, parw11, parw12, parw13, parpcarr14);
  SKW_0b36_01cd(UPCAST(c_hex18, ADRESSOF(c_hex18, &c30_00)));
void SKW_CHANCE_TABLE_OPERATION(void)
  SKW_443c_08ab(&vw_00, &ddat.v1e00b2, &vw_04);
  RG1L = SKW_ABS16(RG62W);
  RG1L = SKW_ABS16(RG5W);
  RG1L = SKW_ABS16(RG5W);
  RG1L = SKW_ABS16(RG5W);
  RG1L = SKW_IS_TILE_BLOCKED(SKW_GET_TILE_VALUE(signedlong(RG4W), RG2L) & lcon(0xff));
  RG1L = SKW_IS_CREATURE_MOVABLE_THERE(signedlong(RG4W), RG2L, UPCAST(i16, NULL), RG7l);
  SKW_HIGHLIGHT_ARROW_PANEL(RG1L, lcon(0x1), RG4L);
void SKW_QUERY_CREATURE_PICST(i32 eaxl, xxx* xebxp, xxx* xecxp, i32 edxl, i16 argw0)
  RG1P = DOWNCAST(i16, SKW_1c9a_02c3(RG2P, SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(RG1L)));
  R_4DA3(vl_18, UPCAST(i16, RG1P + lcon(0x2)), UPCAST(s_8bytearray, ADRESSOF(s_8bytearray, &s09_10)), unsignedlong(word_at(RG1P)));
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(argw0));
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xf), lcon(0x1), CUTLX8(RG3L), vb_48) ? 1 : 0;
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xf), lcon(0x1), CUTLX8(RG3L), vb_48) ? 1 : 0;
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xf), lcon(0x1), CUTLX8(RG3L), CUTLX8(RG73L)) ? 1 : 0;
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xf), lcon(0x1), CUTLX8(RG3L), CUTLX8(RG73L)) ? 1 : 0;
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xf), CUTLX8(RG2L), CUTLX8(RG3L), CUTLX8(RG4L)) ? 1 : 0;
  R_4DA3(unsignedlong(vb_48), &vw_20, &s09_08, unsignedlong(word_at(RG51p, lcon(0xe))));
  SKW_COPY_MEMORY(ADRESSOF(s_8bytearray, &s09_10), lcon(0x8), ADRESSOF(s_8bytearray, &s09_08));
  RG1P = SKW_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0xf), CUTLX8(RG2L), CUTLX8(RG3L), CUTLX8(RG4L));
  RG1L = SKW_CALC_STRETCHED_SIZE(unsignedword(byte_at(RG2P, signedlong(vq_24.peek16()))), RG4W);
  RG1W = SKW_QUERY_CREATURE_BLIT_RECTI(signedlong(vql_04.peek16()), t_rotate(RG2UW), RG4W);
  RG2L = SKW_CALC_STRETCHED_SIZE(xw_2c.peek16(), RG4W);
  RG5L = SKW_CALC_STRETCHED_SIZE(xw_34.peek16(), RG4W);
  RG1L = signedlong(CUTX16(SKW_0cee_2e35(RG1L)));
  SKW_QUERY_TEMP_PICST(RG1L, RG4L, RG3L, RG4L, parl00, parw01, parl02, parl03, parw04, lcon(0xffff), lcon(0xf), parb07, parb08);
i32 SKW_SUMMARY_DRAW_CREATURE(i32 eaxl, i32 ebxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG6W));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG5p, lcon(0x4))));
  SKW_QUERY_CREATURE_PICST(RG1L, RG2P, RG3P, RG4L, parw00);
  SKW_DRAW_TEMP_PICST();
  SKW_32cb_3e08(RG1L, RG2L, RG3P, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG6W));
void SKW_32cb_2cf3(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
  RG5L = SKW_BETWEEN_VALUE16(lcon(0x8), lcon(0x40), RG4L);
  SKW_QUERY_GDAT_SUMMARY_IMAGE(&ddat.v1e118e, unsignedlong(vb_04), lcon(0x41), lcon(0xd));
  R_B073(ddat.v1e118e.carr_3a, lcon(0xa), lcon(0xffffffff), unsignedlong(CUTX8(ddat.v1e12d2)), &ddat.v1e118e.w_38);
  SKW_QUERY_PICST_IT(&ddat.v1e118e);
i32 SKW_32cb_2d8c(i32 eaxl, i32 ebxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG1L);
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xd), CUTLX8(RG2L), CUTLX8(RG3L), CUTLX8(RG4L)) ? 1 : 0;
  vl_18 = SKW_RAND02();
  SKW_UPDATE_GLOB_VAR(lcon(0x40), RG2L, RG4L);
  SKW_UPDATE_GLOB_VAR(lcon(0x41), RG2L, lcon(0x1));
  RG1W = SKW_ROTATE_5x5_POS(RG72W, t_rotate(RG4UW));
  RG1W = SKW_QUERY_CREATURE_BLIT_RECTI(RG1L, t_rotate(RG2UW), RG4W);
  RG1L = SKW_MAX16(lcon(0x30), RG4L);
  SKW_32cb_2cf3(unsignedlong(vb_3c), RG2L, RG3L, RG4L);
  RG1L = SKW_CALC_STRETCHED_SIZE(ddat.v1e118e.w_20, RG62W);
  RG1L = SKW_CALC_STRETCHED_SIZE(RG1W, RG4W);
  SKW_DRAW_TEMP_PICST();
  RG1P = DOWNCAST(t_gfxdata, SKW_QUERY_GDAT_IMAGE_ENTRY_BUFF(lcon(0xd), CUTLX8(RG2L), CUTLX8(RG4L)));
  SKW_3e74_58bf(lcon(0xd), CUTLX8(RG2L), CUTLX8(RG4L));
  parpcarr04 = SKW_32cb_0649(lcon(0xd), RG5L, RG3L, RG63L);
  RG1L = signedlong(CUTX16(SKW_RAND16(unsignedlong(RG4W))));
  RG1L = SKW_RAND();
  RG1R = SKW_QUERY_EXPANDED_RECT(lcon(0x4), RG4R);
  SKW_44c8_20a4(RG74p, NULL, RG1R, ddat.bitmapptr, parw00, parw01, parw02, lcon(0x0), parpcarr04);
  SKW_3e74_5992(lcon(0xd), CUTLX8(RG2L), CUTLX8(RG4L));
  SKW_DRAW_TELEPORTER_TILE(RG1L, RG2L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_08.peek16()));
void SKW_DRAW_FLYING_ITEM(i32 eaxl, i32 ebxl, i32 edxl)
  RG1L = SKW_DIR_FROM_5x5_POS(unsignedlong(RG2W));
  RG1L = SKW_QUERY_OBJECT_5x5_POS(RG3L, RG4L) & lcon(0xffff);
  RG1W = SKW_QUERY_CREATURE_BLIT_RECTI(RG6L, t_rotate(0), RG4W);
  RG1L = SKW_48ae_011a(RG3L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG3L);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG3L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG3L);
  RG1L = SKW_CALC_STRETCHED_SIZE(RG1W, RG3W);
  RG1L = SKW_MAX16(lcon(0x8), RG4L);
  SKW_QUERY_TEMP_PICST(RG1L, RG4L, lcon(0x0), RG4L, parl05, parw06, parl07, parl08, lcon(0xa), lcon(0xffff), parb11, parb12, parb13);
  RG1L = SKW_CALC_STRETCHED_SIZE(lcon(0x0), RG4W);
  RG1L = SKW_CALC_STRETCHED_SIZE(lcon(0xffffffa4), RG4W);
  SKW_DRAW_TEMP_PICST();
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG3L);
  SKW_DRAW_ITEM(RG1L, RG2L, RG3L, RG4L, lcon(0xffff), NULL, lcon(0x1), lcon(0x0), lcon(0x0));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(xwc_08.peek16()));
void SKW_MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
  RG1L = SKW_MAX16(signedlong(mkw(word_at(RG3P) + word_at(RG3P, lcon(0x4)))), RG4L);
  RG1L = SKW_MIN16(signedlong(word_at(RG3P)), signedlong(ddat.v1e118e.rc_24.w_00_x));
  RG1L = SKW_MAX16(signedlong(mkw(word_at(RG3P, lcon(0x2)) + word_at(RG3P, lcon(0x6)))), RG4L);
  RG1L = SKW_MIN16(signedlong(word_at(RG3P, lcon(0x2))), signedlong(ddat.v1e118e.rc_24.w_02_y));
  SKW_COPY_MEMORY(DOWNCAST(c_rect, &ddat.v1e118e.rc_24), lcon(0x8), RG3P);
void SKW_DRAW_ITEM(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl, i16 argw0, xxx* xargp1, i32 argl2, i16 argw3, i32 argl4)
  RG1L = SKW_QUERY_OBJECT_5x5_POS(unsignedlong(xwc_04.peek16()), RG4L);
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG7p, lcon(0x4))));
  RG1L = SKW_QUERY_GDAT_CREATURE_WORD_VALUE(unsignedlong(byte_at(RG7p, lcon(0x4))), lcon(0x41));
  RG1L = SKW_32cb_35c1(&vw_00, RG2L, RG3L, RG4P16);
  RG1W = SKW_QUERY_CREATURE_BLIT_RECTI(signedlong(vw_00), t_rotate(0), RG4W);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG4L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(vb_34, lcon(0x1), CUTLX8(RG3L), vb_28) ? 1 : 0;
  SKW_QUERY_TEMP_PICST(unsignedlong(xwc_20.peek16()), RG4L, RG3L, RG4L, parl00, parw01, parl02, lcon(0x0), lcon(0xa), lcon(0xffff), parb06, parb07, parb08);
  SKW_DRAW_TEMP_PICST();
void SKW_DRAW_PUT_DOWN_ITEM(i32 eaxl, i32 ebxl, xxx* xecxp, i32 edxl)
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG3P, lcon(0x4))));
  SKW_DRAW_ITEM(RG1L, RG2L, RG3L, RG4L, parw05, NULL, lcon(0x1), parw08, parl09);
  RG1L = SKW_1c9a_03cf(&vw_08, RG2L, RG4P16);
  RG1L = SKW_IS_CREATURE_FLOATING(RG4L);
  RG1L = SKW_0cee_2df4(RG4L);
  SKW_QUERY_EXPANDED_RECT(xw_14.peek16(), RG4R);
  SKW_DRAW_ITEM(RG1L, RG2L, RG3L, RG4L, parw00, xparp01, lcon(0x1), lcon(0x0), lcon(0x0));
  RG1L = SKW_48ae_01af(signedlong(vq_10.peek16()), lcon(0x0));
  SKW_MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE(RG1L, RG2L, RG3L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG6W));
void SKW_DRAW_STATIC_OBJECT(i32 eaxl, i32 ebxl, i32 edxl)
  SKW_32cb_2d8c(unsignedlong(xwc_18.peek16()), vl_04, signedlong(vql_08.peek16()));
  RG1L = SKW_DIR_FROM_5x5_POS(unsignedlong(vq_28.peek16()));
  SKW_DRAW_PUT_DOWN_ITEM(RG1L, RG2L, RG3P, RG4L);
  RG1L = SKW_SUMMARY_DRAW_CREATURE(unsignedlong(word_at(ddat.ptr1e1044 + vl_0c, lcon(0x6))), RG2L, RG4L);
  SKW_DRAW_FLYING_ITEM(unsignedlong(xwc_18.peek16()), RG2L, RG4L);
  SKW_DRAW_FLYING_ITEM(unsignedlong(xwc_18.peek16()), RG2L, RG4L);
i32 SKW_32cb_3e08(i32 eaxl, i32 ebxl, xxx* xecxp, i32 edxl)
  RG1L = SKW_DIR_FROM_5x5_POS(unsignedlong(RG4W));
  SKW_DRAW_PUT_DOWN_ITEM(RG1L, RG2L, RG3P, RG4L);
i32 SKW_32cb_3edd(i32 eaxl)
  RG1L = SKW_32cb_3e08(unsignedlong(ddat.v1d6c04), RG2L, RG3P, RG4L);
void SKW_32cb_3f0d(i32 eaxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG5W));
  RG1L = SKW_RAND02();
  RG1W = SKW_098d_0cd7(RG3W, false, lcon(0xc));
  SKW_32cb_2cf3(RG1L, RG2L, RG3L, RG4L);
  RG1L = signedlong(SKW_098d_0cd7(RG3W, false, lcon(0xc)));
  SKW_QUERY_TEMP_PICST(RG1L, RG4L, RG3L, RG4L, lcon(0x0), lcon(0x0), parl07, lcon(0xffffffff), lcon(0xa), lcon(0xffff), lcon(0xd), parb12, lcon(0xc));
  SKW_DRAW_TEMP_PICST();
  SKW_DRAW_ITEM(RG1L, lcon(0x0), RG3L, RG4L, parw00, NULL, lcon(0x1), lcon(0x0), lcon(0x1));
  SKW_MAKE_PUT_DOWN_ITEM_CLICKABLE_ZONE(lcon(0x8), RG2L, RG3L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG5W));
void SKW_32cb_4069(i32 eaxl, i32 ebxl, i16* ecxpw, i32 edxl, i16* argpw0)
  RG1L = SKW_ABS16(xw_04.peek16());
  RG1L = SKW_ABS16(xw_04.peek16());
void SKW_32cb_4185(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
  SKW_CALC_VECTOR_W_DIR(RG5L, RG2L, &vw_1c, RG4L, &vw_10);
  SKW_SUMMARIZE_STONE_ROOM(UPCAST(s_8wordarray, RG1P), RG2L, RG3L, RG5L);
  SKW_098d_000f(signedlong(vql_08.peek16()), RG2L, &vw_0c, RG4L, &vw_18);
  RG1L = SKW_QUERY_OBJECT_5x5_POS(unsignedlong(RG5W), RG4L);
  RG1W = SKW_ROTATE_5x5_POS(RG1W, t_rotate(RG4UW));
  RG1L = SKW_QUERY_OBJECT_5x5_POS(unsignedlong(RG5W), lcon(0x0));
  SKW_098d_000f(signedlong(vw_1c), RG2L, &vw_14, RG4L, &vw_20);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG5W));
  RG2L = signedlong(byte_at(SKW_QUERY_CREATURE_AI_SPEC_FROM_RECORD(unsignedlong(byte_at(RG1P, lcon(0x4)))), lcon(0x22)));
  SKW_32cb_4069(RG1L, RG2L, RG3P16, RG4L, &vw_20);
  RG1L = signedlong(CUTX16(SKW_ABS16(RG3W)));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG5W));
void SKW_DRAW_DEFAULT_DOOR_BUTTON(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
  SKW_QUERY_TEMP_PICST(lcon(0x0), RG4L, lcon(0x0), RG4L, parl00, parw01, parl02, lcon(0xffffffff), parw04, lcon(0xffff), parb06, parb07, parb08);
  SKW_DRAW_TEMP_PICST();
  SKW_32cb_0a4c(RG1R, RG2L, RG4L);
void SKW_DRAW_DOOR_FRAMES(i32 eaxl, i32 edxl)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(ddat.ptr1e1044, RG4L + lcon(0xa))));
  SKW_DRAW_DUNGEON_GRAPHIC(lcon(0x8), RG2L, RG3L, unsignedlong(vb_10), parw00, parl01);
  RG1L = signedlong(SKW_QUERY_CREATURE_BLIT_RECTI(signedlong(RG5W), t_rotate(0), lcon(0xa)));
  SKW_QUERY_TEMP_PICST(lcon(0x0), RG2L, RG3L, RG2L, lcon(0x0), lcon(0x0), parl04, lcon(0x4), parw06, lcon(0xffff), lcon(0x8), parb09, parb10);
  SKW_DRAW_TEMP_PICST();
  RG1L = signedlong(SKW_QUERY_CREATURE_BLIT_RECTI(RG5L, t_rotate(RG2UW), RG4W));
  SKW_QUERY_TEMP_PICST(RG1L, RG2L, RG3L, RG4L, lcon(0x0), lcon(0x0), parl13, lcon(0x3), parw15, lcon(0xffff), lcon(0x8), parb18, parb19);
  SKW_DRAW_TEMP_PICST();
  SKW_DRAW_DEFAULT_DOOR_BUTTON(RG1L, RG2L, RG3L, RG4L);
i32 SKW_32cb_48d5(i32 eaxl, i32 edxl)
  RG1L = signedlong(CUTX16(SKW_CALC_STRETCHED_SIZE(RG2W, RG4W)));
void SKW_DRAW_DOOR(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
  SKW_DRAW_DOOR_FRAMES(signedlong(vql_04.peek16()), RG4L);
  SKW_DRAW_STATIC_OBJECT(signedlong(vql_04.peek16()), lcon(0x0), RG3L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG2W));
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0xe), lcon(0x1), vb_178, vb_174) ? 1 : 0;
  SKW_QUERY_TEMP_PICST(lcon(0x0), RG4L, lcon(0x0), RG4L, lcon(0x0), parw26, lcon(0xffffffff), lcon(0xffffffff), parw29, lcon(0xffff), lcon(0xe), parb32, parb33);
  RG1L = SKW_QUERY_MULTILAYERS_PIC(&c37_08, RG1L, RG3L, lcon(0xe), parw00, parw01, parw02, lcon(0x0), parw04, parl05);
  SKW_DRAW_PICST(&c37_08);
  SKW_0b36_01cd(&c37_08.c30_00);
  SKW_QUERY_GDAT_IMAGE_METRICS(lcon(0xe), lcon(0x0), RG3P16, vl_144, &vw_14c);
  RG61L = SKW_32cb_48d5(unsignedlong(ddat.v1e118e.c30_00.w_12), unsignedlong(vw_148));
  RG1L = SKW_32cb_48d5(unsignedlong(ddat.v1e118e.c30_00.w_14), RG4L);
  SKW_QUERY_TEMP_PICST(lcon(0x0), RG2L, lcon(0x0), RG4L, lcon(0x0), parw08, parl09, lcon(0xffffffff), parw11, lcon(0xffff), lcon(0xb), parb14, lcon(0x0));
  ddat.v1e118e.pgarr_2c = DOWNCAST(t_gfxdata, SKW_3e74_5817(vq_15c.peek16())); // NFIX
  SKW_DRAW_TEMP_PICST();
  SKW_QUERY_TEMP_PICST(lcon(0x0), RG2L, lcon(0x0), RG4L, lcon(0x0), parw17, parl18, lcon(0xffffffff), parw20, parw21, lcon(0xe), parb23, lcon(0x41));
  ddat.v1e118e.pgarr_2c = DOWNCAST(t_gfxdata, SKW_3e74_5817(vq_15c.peek16())); // NFIX
  SKW_DRAW_TEMP_PICST();
  SKW_0b36_00c3(signedlong(vq_15c.peek16()), UPCAST(c_hex18, ADRESSOF(c_image, &c37_08)));
  SKW_COPY_MEMORY(ADRESSOF(c_image, &c37_08), lcon(0x13a), DOWNCAST(c_image, &ddat.v1e118e));
  SKW_DRAW_TEMP_PICST();
  SKW_DRAW_TEMP_PICST();
  SKW_3e74_58a3(vq_15c.peek16());
  SKW_DRAW_DOOR_FRAMES(signedlong(vql_04.peek16()), RG4L);
void SKW_DRAW_DOOR_TILE(i32 eaxl)
  SKW_DRAW_STATIC_OBJECT(RG6l, RG2L, lcon(0x3ff));
  SKW_DRAW_STATIC_OBJECT(RG6l, lcon(0x0), lcon(0x1000));
  SKW_DRAW_DOOR(RG1L, lcon(0x0), lcon(0x0), lcon(0x6));
  SKW_DRAW_DOOR(RG1L, RG2L, RG3L, RG4L);
  SKW_DRAW_STATIC_OBJECT(RG3L, lcon(0x1), lcon(0x1fffc00));
  SKW_32cb_3edd(RG3L);
void SKW_DRAW_STAIRS_FRONT(i32 eaxl)
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x8), lcon(0x1), CUTLX8(RG7L), CUTLX8(RG5L)) ? 1 : 0;
  SKW_DRAW_DUNGEON_GRAPHIC(lcon(0x8), RG7L, signedlong(xw_00.peek16()), RG5L, parw00, parl01);
  SKW_QUERY_TEMP_PICST(lcon(0x1), RG2L, RG3L, RG2L, lcon(0x0), lcon(0x0), parl04, lcon(0xffffffff), parw06, lcon(0xffff), lcon(0x8), parb09, parb10);
  SKW_DRAW_TEMP_PICST();
void SKW_DRAW_STAIRS_SIDE(i32 eaxl)
  SKW_DRAW_DUNGEON_GRAPHIC(lcon(0x8), RG2L, RG3L, unsignedlong(vb_00), parw00, parl01);
i32 SKW_DRAW_WALL(i32 eaxl)
  SKW_TRIM_BLIT_RECT(RG5W, lcon(0x0), RG6w, lcon(0x0));
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x8), lcon(0x1), CUTLX8(RG3L), vb_0c) ? 1 : 0;
  SKW_QUERY_TEMP_PICST(RG1L, RG2L, RG3L, RG2L, parl00, parw01, parl02, lcon(0xffffffff), parw04, lcon(0xffff), lcon(0x8), parb07, parb08);
  SKW_DRAW_TEMP_PICST();
void SKW_DRAW_WALL_TILE(i32 eaxl)
  RG1L = SKW_DRAW_WALL(RG3L);
  SKW_32cb_15b8(RG3L, lcon(0x1), lcon(0x0));
  SKW_32cb_15b8(RG1L, RG2L, RG4L);
i32 SKW_DRAW_DUNGEON_TILES(void)
  SKW_IBMIO_USER_INPUT_CHECK();
  SKW_DRAW_PIT_TILE(signedlong(RG6w));
  RG4L = unsignedlong(ddat.v1e040e[unsignedlong(mkuw(word_at(DOWNCAST(xxx, SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG1P, lcon(0x40))))), lcon(0x2)) & wcon(0x1)))]);
  RG1L = signedlong(SKW_QUERY_CREATURE_BLIT_RECTI(lcon(0x3), t_rotate(RG2UW), RG4W));
  SKW_QUERY_TEMP_PICST(RG1L, RG2L, RG3L, RG2L, lcon(0x0), lcon(0x2), parl02, lcon(0xffffffff), parw04, lcon(0xffff), lcon(0x8), parb07, lcon(0x6));
  SKW_DRAW_TEMP_PICST();
  SKW_DRAW_DOOR_TILE(signedlong(RG6w));
  SKW_DRAW_STAIRS_SIDE(signedlong(RG6w));
  SKW_DRAW_STAIRS_FRONT(signedlong(RG6w));
  RG1L = SKW_DRAW_EXTERNAL_TILE(unsignedlong(RG6w));
  SKW_DRAW_PIT_ROOF(signedlong(RG6w));
  SKW_DRAW_STATIC_OBJECT(RG3L, RG2L, RG4L);
  SKW_DRAW_TELEPORTER_TILE(RG3L, RG2L, lcon(0x18));
  SKW_DRAW_EXTERNAL_TILE(unsignedlong(RG6w));
  SKW_SUMMARY_DRAW_CREATURE(RG3L, RG2L, RG1L);
  SKW_SUMMARY_DRAW_CREATURE(RG1L, RG2L, RG4L);
  SKW_DRAW_WALL_TILE(signedlong(RG6w));
i32 SKW_DRAW_PLAYER_TILE(void)
  SKW_DRAW_PIT_TILE(lcon(0x0));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG1P, lcon(0xa))));
  RG1W = SKW_QUERY_CREATURE_BLIT_RECTI(lcon(0x0), t_rotate(RG2UW), RG4W);
  SKW_DRAW_DOOR_TILE(lcon(0x0));
  SKW_DRAW_DUNGEON_GRAPHIC(lcon(0x8), RG2L, RG3L, unsignedlong(ddat.v1d6c02), parw02, parl03);
  SKW_DRAW_DUNGEON_GRAPHIC(lcon(0x8), RG2L, RG3L, RG4L, parw00, parl01);
  SKW_DRAW_PIT_ROOF(lcon(0x0));
  RG1L = SKW_GET_TELEPORTER_DETAIL(&c12_00, RG2L, RG4L);
  SKW_DRAW_EXTERNAL_TILE(RG1L);
  SKW_DRAW_STATIC_OBJECT(lcon(0x0), lcon(0x0), lcon(0x1ffffff));
  SKW_DRAW_TELEPORTER_TILE(lcon(0x0), lcon(0x0), lcon(0x18));
  SKW_DRAW_RAIN();
  SKW_DRAW_EXTERNAL_TILE(lcon(0x0));
  return SKW_32cb_2d8c(unsignedlong(word_at(ddat.ptr1e1044, lcon(0x6))), lcon(0x1ffffff), lcon(0x0));
i32 SKW_ENVIRONMENT_SET_DISTANT_ELEMENT(xxx* xeaxp, i32 ebxl, i32 ecxl, t_text* edxtp, i32 argl0)
  RG1L = SKW_QUERY_CMDSTR_TEXT(RG6t, RG4T);
  RG1L = SKW_QUERY_CMDSTR_TEXT(RG6t, RG4T);
  RG1L = signedlong(CUTX16(SKW_QUERY_CMDSTR_TEXT(RG6t, RG4T)));
  RG3L = SKW_32cb_54ce(signedlong(xw_00.peek16()), &vw_08, signedlong(RG2W), &vw_04, parl00);
  RG1L = SKW_QUERY_CMDSTR_TEXT(RG6t, RG4T);
  RG1L = SKW_MAX16(lcon(0x1), RG4L);
void SKW_ENVIRONMENT_DRAW_DISTANT_ELEMENT(xxx* xeaxp, i32 ebxl, i32 ecxl, i32 edxl)
  RG1L = SKW_SET_GRAPHICS_FLIP_FROM_POSITION(lcon(0x1), RG2L, RG3L, RG4L);
  RG1L = SKW_SET_GRAPHICS_FLIP_FROM_POSITION(lcon(0x20), RG2L, RG3L, RG4L);
  RG1L = SKW_SET_GRAPHICS_FLIP_FROM_POSITION(lcon(0x1), RG2L, RG3L, RG4L);
  RG1L = SKW_SET_GRAPHICS_FLIP_FROM_POSITION(RG1L, RG2L, RG3L, RG4L);
  RG1L = SKW_CALC_STRETCHED_SIZE(xw_10.peek16(), lcon(0x34));
  RG1L = SKW_CALC_STRETCHED_SIZE(RG6W, lcon(0x34));
  RG1L = SKW_CALC_STRETCHED_SIZE(RG3W, lcon(0x34));
  RG1L = SKW_CALC_STRETCHED_SIZE(xw_0c.peek16(), lcon(0x34));
  SKW_QUERY_TEMP_PICST(unsignedlong(xwc_00.peek16()), RG2L, lcon(0x0), RG4L, lcon(0x0), lcon(0x0), parl02, lcon(0xffffffff), parw04, lcon(0xffff), lcon(0x17), parb07, parb08);
  SKW_DRAW_TEMP_PICST();
void SKW_ENVIRONMENT_DISPLAY_ELEMENTS(i32 eaxl, i32 ebxl, i32 edxl)
  SKW_ENVIRONMENT_DRAW_DISTANT_ELEMENT(RG1P, RG2L, RG3L, RG4L);
  DM2_ZERO_MEMORY(ddat.v1e13dc, lcon(0xd));
  RG1L = SKW_QUERY_GDAT_ENTRY_IF_LOADABLE(lcon(0x17), lcon(0x1), CUTLX8(RG3L), CUTLX8(RG4L)) ? 1 : 0;
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x17), RG2L, RG3T, RG4L));
  SKW_QUERY_GDAT_TEXT(lcon(0x17), RG2L, tarr_0c, unsignedlong(ddat.v1d6c02));
  RG1L = SKW_ENVIRONMENT_SET_DISTANT_ELEMENT(RG53p, RG2L, RG3L, RG4T, parl00);
  RG1L = SKW_QUERY_CMDSTR_TEXT(tarr_0c, RG4T);
  RG1L = SKW_QUERY_CMDSTR_TEXT(tarr_0c, RG4T);
  SKW_ENVIRONMENT_DRAW_DISTANT_ELEMENT(RG53p, RG2L, RG3L, RG4L);
i32 SKW_32cb_5c67(void)
  RG1L = SKW_GET_TELEPORTER_DETAIL(&c12_00, RG2L, RG4L);
void SKW_DISPLAY_VIEWPORT(i32 eaxl, i32 ebxl, i32 edxl)
  SKW_IBMIO_USER_INPUT_CHECK();
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x19e));
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x166), true);
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x166));
  SKW_FILL_STR(RG1T, lcon(0xff), lcon(0x1), lcon(0x166));
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x5c), true);
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x5c), true);
  SKW_INIT_BACKBUFF();
  SKW_IBMIO_USER_INPUT_CHECK();
  SKW_CHANCE_TABLE_OPERATION();
  SKW_32cb_4185(signedlong(RG61w), RG2L, RG3L, signedlong(xw_04.peek16()));
  SKW_TRIM_BLIT_RECT(lcon(0x0), lcon(0x0), lcon(0x0), RG2W);
  RG1L = SKW_SET_GRAPHICS_FLIP_FROM_POSITION(lcon(0x20), RG2L, RG3L, signedlong(vql_00.peek16()));
  SKW_DRAW_DUNGEON_GRAPHIC(lcon(0x8), lcon(0x1), lcon(0x2bc), unsignedlong(ddat.v1d6c02), lcon(0xffff), parl01);
  SKW_TRIM_BLIT_RECT(lcon(0x0), RG5W, lcon(0x0), lcon(0x0));
  RG1L = SKW_SET_GRAPHICS_FLIP_FROM_POSITION(lcon(0x1), RG2L, RG3L, signedlong(vql_00.peek16()));
  SKW_DRAW_DUNGEON_GRAPHIC(lcon(0x8), lcon(0x0), lcon(0x2bd), unsignedlong(ddat.v1d6c02), lcon(0xffff), parl03);
  ddat.v1e12d0 = CUTX16(SKW_SET_GRAPHICS_FLIP_FROM_POSITION(lcon(0x0), RG5L, RG7l, RG62l));
  SKW_IBMIO_USER_INPUT_CHECK();
  SKW_ENVIRONMENT_DISPLAY_ELEMENTS(RG62l, RG7l, RG5L);
  SKW_32cb_5c67();
  SKW_32cb_5a8f();
  SKW_DRAW_DUNGEON_TILES();
  SKW_IBMIO_USER_INPUT_CHECK();
  SKW_DRAW_PLAYER_TILE();
  SKW_IBMIO_USER_INPUT_CHECK();
  DM2_DEALLOC_LOBIGPOOL(lcon(0x522));
void SKW_38c8_0002(i32 eaxl)
  SKW_CHAMPION_SQUAD_RECOMPUTE_POSITION();
  RG1L = SKW_UPDATE_RIGHT_PANEL(lcon(0x0));
  SKW_1031_04F5(RG1L);
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_12b4_0092();
  SKW_FIRE_FILL_HALFTONE_RECTI(RG1W, RG4W);
  SKW_FIRE_FILL_HALFTONE_RECTI(RG1W, RG4W);
  SKW_FIRE_SHOW_MOUSE_CURSOR();
void SKW_38c8_0060(void)
  SKW_DRAW_WAKE_UP_TEXT();
  SKW_CHANGE_VIEWPORT_TO_INVENTORY(lcon(0x0));
  SKW_24a5_1798(RG1L);
  SKW_INIT_BACKBUFF();
  SKW_29ee_000f();
void SKW_38c8_00c8(void)
  ddat.bitmapptr = R_D2C(signedlong(RG1W), lcon(0x8), RG4L);
i32 SKW_38c8_0224(bool eaxbool, i32 edxl)
  SKW_DRAW_DIALOGUE_PROGRESS(RG1L);
  SKW_LOAD_NEWMAP(lcon(0x0), RG2L, lcon(0x0), lcon(0x0));
  RG1L = SKW_0aaf_02f8(lcon(0x2), lcon(0x0)) & lcon(0xff);
  SKW_0aaf_0067(RG1L);
  SKW_SK_PREPARE_EXIT();
  SKW_0aaf_02f8(lcon(0xe), lcon(0x20));
  SKW_DRAW_DIALOGUE_PROGRESS(RG1L);
i32 SKW__INIT_GAME_38c8_03ad(void)
  DM2_ZERO_MEMORY(&ddat.v1e0ffa[2], lcon(0x10)); // EVIL offset shifted, was 0x1e0ffc
  DM2_ZERO_MEMORY(ddat.v1e0adc, lcon(0x40));
  SKW_1031_0541(lcon(0x5));
  SKW_LOAD_NEWMAP(signedlong(ddat.v1e0270), signedlong(ddat.v1e0266), RG3L, signedlong(ddat.v1e0272));
  SKW_RESET_CAII();
  SKW_WAIT_SCREEN_REFRESH();
  SKW_FIRE_FILL_SCREEN_RECT(RG1W, RG4Blo);
  RG1L = SKW_0aaf_02f8(lcon(0x6), lcon(0x0)) & lcon(0xff);
  SKW_0aaf_0067(RG1L);
  DM2_GRAPHICS_DATA_CLOSE();
  RG1L = SKW_2066_03e0(lcon(0x0));
  DM2_GRAPHICS_DATA_OPEN();
  SKW_WAIT_SCREEN_REFRESH();
  SKW_FIRE_FILL_SCREEN_RECT(RG1W, RG4Blo);
  SKW_29ee_000f();
  SKW_2f3f_0789();
  return SKW_CHECK_RECOMPUTE_LIGHT();
void SKW_INIT(void)
  DM2_2636_03d4();
  SKW_READ_GRAPHICS_STRUCTURE();
  SKW_482b_0004();
  RG1P = DM2_ALLOC_HIBIGPOOL_MEMORY(lcon(0x400));
  SKW_LOAD_GDAT_ENTRY_DATA_TO(lcon(0x1), lcon(0x9), lcon(0xfe), lcon(0x0), xparp00);
  RG1L = SKW_44c8_1baf(RG51p);
  DM2_DEALLOC_HIBIGPOOL(lcon(0x400));
  RG1L = SKW_LOAD_GDAT_INTERFACE_00_02();
  RG1P = SKW_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0x1), lcon(0xd), lcon(0xfe), lcon(0x0));
  SKW_098d_1208();
  SKW_38c8_00c8();
  SKW_3929_0e16();
  SKW_2405_0009();
  SKW_443c_0380();
  RG1L = SKW_1031_07d6();
  RG1Blo = SKW_3e74_2439(lcon(0xf), lcon(0xb));
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(RG5L);
  SKW_FILL_STR(RG1T, RG2Blo, RG3W, RG4W);
  SKW_SHOW_MENU_SCREEN();
  RG1L = signedlong(CUTX16(SKW_GAME_LOAD()));
  DM2_GRAPHICS_DATA_OPEN();
  DM2_GRAPHICS_DATA_OPEN();
  R_5BFB(unsignedlong(ddat.v1d26a0), lcon(0x0));
  R_5BFB(unsignedlong(ddat.v1d26a2), lcon(0xa));
  SKW_1c9a_3c30();
  RG1L = SKW_ALLOC_CPX_SETUP(table1d70b4);
  SKW__INIT_GAME_38c8_03ad();
  DM2_GRAPHICS_DATA_CLOSE();
  RG1L = SKW_MOVE_RECORD_TO(lcon(0xffff), lcon(0xffffffff), lcon(0x0), signedlong(ddat.v1e0270), parw01);
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_1031_098e();
void SKW_3929_07e1(i32 eaxl, i32 edxl)
  RG1L = SKW_MIN16(signedlong(RG1W), RG4L);
i32 SKW_3929_0826(void)
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_FIRE_FILL_SCREEN_RECT(RG1W, RG4Blo);
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_3929_07e1(lcon(0x0), lcon(0x2));
void SKW_3929_086f(void)
  SKW_QUERY_EXPANDED_RECT(lcon(0xf), RG4R);
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_44c8_1dfc(RG1R, RG4Blo);
  SKW_FIRE_SHOW_MOUSE_CURSOR();
void SKW_3929_0914(void)
  SKW_44c8_1c99();
  SKW_FILL_ENTIRE_PICT(ddat.v1e1414, ddat.paletteptr1[0]);
i32 SKW_3929_0929(i32 eaxl, t_text* edxtp)
  SKW_QUERY_EXPANDED_RECT(RG1W, RG4R);
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_PRINT_SYSERR_TEXT(RG1W, RG4W, RG2W, RG5W, par_tp04);
  SKW_FIRE_SHOW_MOUSE_CURSOR();
  SKW_DRAW_STRING(ddat.v1e1414, ddat.v1d70ea, RG3W, ddat.v1d2744, parw00, CUTX16(parl01), par_tp02, lcon(0x8));
void SKW_DISPLAY_HINT_TEXT(i32 eaxl, t_text* edxtp)
  SKW_44c8_1c99();
  SKW_FILL_ENTIRE_PICT(ddat.v1e1414, RG4Blo);
  RG1L = SKW_3929_04e2(UPCAST(t_text, RG5p), RG2P16, RG3L, RG4T);
  SKW_3929_0914();
  SKW_3929_0929(unsignedlong(RG62w), tarr_00);
  SKW_3929_0929(unsignedlong(RG62w), tarr_00);
  SKW_3929_0914();
  SKW_3929_0929(unsignedlong(RG62w), tarr_00);
  SKW_44c8_1c99();
  SKW_FILL_ENTIRE_PICT(ddat.v1e1414, ddat.paletteptr1[0]);
void SKW_3929_0b01(i32 eaxl, i32 edxl)
  SKW_DISPLAY_HINT_TEXT(RG1L, tarr_00);
void SKW_3929_0b20(i32 eaxl, i32 edxl)
  SKW_SK_LTOA10(RG4L, tarr_00);
  SKW_DISPLAY_HINT_TEXT(RG2L, tarr_00);
  SKW_DISPLAY_HINT_TEXT(RG2L, ddat.v1d70e4);
void SKW_DISPLAY_HINT_NEW_LINE(void)
  SKW_DISPLAY_HINT_TEXT(lcon(0x0), ddat.v1d70e8);
i32 SKW_3929_0e16(void)
  SKW_3929_07e1(lcon(0x0), lcon(0x2));
  ddat.v1e1414 = SKW_ALLOC_PICT_BUFF(ddat.v1d2744, FREEPOOL, lcon(0x8), ddat.v1d2746);
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(lcon(0x300));
  SKW_LOAD_GDAT_ENTRY_DATA_TO(lcon(0x1), lcon(0x7), CUTLX8(RG3L), lcon(0x0), xparp00);
  SKW_QUERY_EXPANDED_RECT(lcon(0xf), RG4R);
void SKW_3a15_0002(void)
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(12 * RG2L);
  RG1P = DM2_ALLOC_FREEPOOL_MEMORY(2 * signedlong(ddat.v1e142c));
void SKW___SORT_TIMERS(void)
  RG1L = SKW_3a15_0381(UPCAST(c_tim, RG51p), UPCAST(c_tim, RG4P));
  RG1L = SKW_3a15_0381(UPCAST(c_tim, RG51p), UPCAST(c_tim, RG4P));
  RG1L = SKW_3a15_0381(UPCAST(c_tim, RG61p), UPCAST(c_tim, RG4P));
  RG1L = SKW_3a15_0381(UPCAST(c_tim, RG51p), UPCAST(c_tim, RG4P));
  SKW_REARRANGE_TIMERLIST();
void SKW_3a15_020f(void)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG2P, lcon(0x6))));
i32 SKW_GET_TIMER_NEW_INDEX(i32 eaxl)
  SKW_RAISE_SYSERR(lcon(0x46));
void SKW_3a15_0486(i32 eaxl)
  RG1L = SKW_3a15_0381(UPCAST(c_tim, RG5p), UPCAST(c_tim, RG4P));
  RG1L = SKW_3a15_0381(UPCAST(c_tim, RG63p + 12 * RG4L), ebpp_c38);
  RG1L = SKW_3a15_0381(UPCAST(c_tim, RG1P), UPCAST(c_tim, RG4P));
void SKW_3a15_05f7(i32 eaxl)
  SKW_3a15_0486(unsignedlong(RG2W));
  SKW_3a15_0486(SKW_GET_TIMER_NEW_INDEX(unsignedlong(RG4W)) & lcon(0xffff));
void SKW_DELETE_TIMER(i32 eaxl)
  SKW_3a15_0486(RG1L);
  RG1L = SKW_GET_TIMER_NEW_INDEX(RG2L);
i32 SKW_QUEUE_TIMER(c_tim* eaxp_c38)
  SKW_RAISE_SYSERR(lcon(0x2d));
  SKW_3a15_0486(RG1L);
void SKW_GET_AND_DELETE_NEXT_TIMER(c_tim* eaxp_c38)
  SKW_DELETE_TIMER(RG4L);
i32 SKW_IS_TIMER_TO_PROCEED(void)
  SKW_3a15_0486(RG1L);
void SKW_STEP_DOOR(c_tim* eaxp_c38)
  RG1P = SKW_GET_ADDRESS_OF_TILE_RECORD(signedlong(RG6w), RG4L);
  RG1L = SKW_QUERY_DOOR_DAMAGE_RESIST(SKW_IS_REBIRTH_ALTAR(RG1P) & lcon(0xff));
  RG1L = SKW_ATTACK_PARTY(unsignedlong(vo_24.peek16()), RG4L, RG2L);
  SKW_QUEUE_NOISE_GEN2(lcon(0x16), RG2Blo, RG3Blo, RG4Blo, parw00, parw01, lcon(0x1), lcon(0x64), lcon(0xc8));
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG6w), RG4L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(xwc_14.peek16()));
  RG1P = SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(unsignedlong(xwc_14.peek16()));
  RG1L = SKW_MAX16(lcon(0x1), RG4L);
  SKW_ATTACK_CREATURE(unsignedlong(xwc_14.peek16()), RG4L, RG2L, lcon(0x2006), lcon(0x64), parl06);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(unsignedlong(xwc_14.peek16()));
  SKW_QUEUE_NOISE_GEN2(lcon(0xf), lcon(0x82), lcon(0xfe), RG4Blo, parw07, parw08, lcon(0x1), lcon(0x46), lcon(0x80));
  SKW_QUEUE_NOISE_GEN2(lcon(0x15), lcon(0x85), RG3Blo, RG3Blo, parw12, parw13, lcon(0x1), lcon(0x46), lcon(0x80));
  RG1L = SKW_IS_REBIRTH_ALTAR(DOWNCAST(xxx, xp_04));
  SKW_QUEUE_NOISE_GEN2(lcon(0xe), RG2Blo, lcon(0xfe), RG4Blo, parw17, parw18, lcon(0x1), lcon(0x5f), lcon(0x80));
  SKW_QUEUE_TIMER(UPCAST(c_tim, RG5p));
void SKW_ACTUATE_DOOR(c_tim* eaxp_c38)
  RG1P = SKW_GET_ADDRESS_OF_TILE_RECORD(RG1L, RG4L);
  RG1L = SKW_3a15_1da8(unsignedlong(byte_at(RG2P, lcon(0x9))), RG4L);
  SKW_QUEUE_TIMER(UPCAST(c_tim, RG2P));
i32 SKW_ACTUATE_TRICKWALL(c_tim* eaxp_c38)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG6w), RG4L);
  RG1L = SKW_QUERY_CREATURE_AI_SPEC_FLAGS(unsignedlong(RG4W));
  SKW_QUEUE_TIMER(UPCAST(c_tim, RG1P));
void SKW_3a15_0d5c(xxx* xeaxp, xxx* xedxp)
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG1W), RG4L);
  SKW_ROTATE_CREATURE(RG1L, lcon(0x1), RG2L);
void SKW_ACTIVATE_CREATURE_KILLER(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, i32 argl1, i16 argw2, i16 argw3)
  RG1L = SKW_ABS16(CUTX16(RG2L - argl0));
  RG1L = SKW_ABS16(CUTX16(RG3L - argl1));
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG1W), RG4L);
  RG1L = SKW_1c9a_09b9(RG1L, RG4L);
  SKW_13e4_0360(vl_00, RG4L, RG2L, RG3L, parl00);
  SKW_ATTACK_CREATURE(unsignedlong(xwc_10.peek16()), RG4L, RG2L, RG3L, lcon(0x64), parl02);
i32 SKW_GET_ORNATE_ANIM_LEN(xxx* xeaxp, i32 edxl)
  RG1L = SKW_GET_FLOOR_DECORATION_OF_ACTUATOR(RG1P);
  RG1L = SKW_GET_WALL_DECORATION_OF_ACTUATOR(RG1P);
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(RG5L, RG2L, RG3T, RG4L));
  RG1L = SKW_SK_STRLEN(RG1T);
void SKW_TRY_ORNATE_NOISE(xxx* xeaxp, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1)
  RG1L = SKW_GET_FLOOR_DECORATION_OF_ACTUATOR(RG51p);
  RG1L = SKW_GET_WALL_DECORATION_OF_ACTUATOR(RG51p);
  SKW_QUEUE_TIMER(UPCAST(c_tim, RG1P));
  SKW_QUEUE_NOISE_GEN2(vb_14, lcon(0x88), lcon(0xfe), vb_18, parw00, parw01, lcon(0x1), lcon(0x8c), lcon(0x80));
void SKW_ACTIVATE_CONTINUOUS_ORNATE_ANIMATOR(i32 eaxl, xxx* xebxp, i32 ecxl, xxx* xedxp)
  RG1L = SKW_3a15_1da8(unsignedlong(byte_at(RG6p, lcon(0x9))), RG4L);
  RG1L = SKW_GET_ORNATE_ANIM_LEN(RG5p, RG4L);
  SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_04)));
  SKW_TRY_ORNATE_NOISE(RG5p, unsignedlong(xwc_00.peek16()), unsignedlong(byte_at(RG6p, lcon(0x6))), unsignedlong(byte_at(RG6p, lcon(0x7))), parw00, parl01);
  SKW_INVOKE_ACTUATOR(RG5p, lcon(0x0), unsignedlong(byte_at(RG6p, lcon(0x9))));
void SKW_ACTIVATE_ORNATE_ANIMATOR(xxx* xeaxp, i32 ebxl, i32 ecxl, xxx* xedxp)
  SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_00)));
  RG1L = SKW_GET_FLOOR_DECORATION_OF_ACTUATOR(RG5p);
  RG1L = SKW_GET_WALL_DECORATION_OF_ACTUATOR(RG5p);
  RG1L = mkl(DM2_QUERY_GDAT_ENTRY_DATA_INDEX(CUTLX8(RG7L), lcon(0xb), lcon(0x88), CUTLX8(RG1L)) + 1);
  SKW_QUEUE_NOISE_GEN2(RG7Blo, lcon(0x88), lcon(0xfe), CUTLX8(vl_0c), parw00, parw01, parw02, lcon(0x8c), lcon(0x80));
  SKW_ACTIVATE_RELAY2(RG6p, lcon(0x0), RG5p);
void SKW_ACTIVATE_RELAY2(xxx* xeaxp, i32 ebxl, xxx* xedxp)
  RG1L = SKW_INVOKE_ACTUATOR(RG3P, RG2L, RG4L);
  SKW_INVOKE_ACTUATOR(RG3P, RG2L, RG4L);
void SKW_ACTIVATE_RELAY1(xxx* xeaxp, i32 ebxl, xxx* xedxp)
  SKW_INVOKE_MESSAGE(RG1L, RG2L, RG3L, RG4L, parl00);
void SKW_ACTIVATE_INVERSE_FLAG(xxx* xeaxp, xxx* xedxp)
  SKW_UPDATE_GLOB_VAR(RG1L, RG2L, lcon(0x1));
  SKW_INVOKE_ACTUATOR(RG3P, RG2L, RG4L);
void SKW_ACTIVATE_TEST_FLAG(xxx* xeaxp, xxx* xedxp)
  RG1L = SKW_GET_GLOB_VAR(unsignedlong(RG1W));
  SKW_INVOKE_ACTUATOR(RG3P, RG1L, RG4L);
i32 SKW_ACTUATE_FLOOR_MECHA(c_tim* eaxp_c38)
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(vq_e4.peek16()), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG2L);
  RG1L = SKW_3a15_1da8(signedlong(RG1W), RG4L);
  SKW_ACTIVATE_CONTINUOUS_ORNATE_ANIMATOR(RG1L, RG2P, RG3L, RG4P);
  R_3BF83(RG1L, RG4L, RG2L, RG3L);
  SKW_ACTIVATE_ORNATE_ANIMATOR(RG1P, RG2L, RG3L, RG4P);
  SKW_ANIMATE_CREATURE(signedlong(vq_e4.peek16()), RG4L, RG2L);
  SKW_ACTIVATE_CREATURE_KILLER(RG1L, RG4L, RG2L, RG3L, parl04, parl05, parw06, parw07);
  SKW_ACTIVATE_RELAY2(RG1P, RG2L, RG4P);
  SKW_3a15_0d5c(RG1P, RG4P);
  SKW_ACTIVATE_INVERSE_FLAG(RG1P, RG4P);
  SKW_ACTIVATE_TEST_FLAG(RG1P, RG4P);
  SKW_ACTIVATE_ITEM_TELEPORT(RG1P, RG4P, RG2L, RG3L, parl08, xparp09, parl10, parl11);
  SKW_ACTIVATE_RELAY1(RG1P, RG2L, RG4P);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG6W));
  RG1L = SKW_RAND() & lcon(0xffff);
  SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_c8)));
  SKW_QUEUE_NOISE_GEN1(lcon(0x3), RG2Blo, RG3W, RG4Blo, lcon(0xc8), parw01, parw02, lcon(0x1));
  SKW_QUERY_MESSAGE_TEXT(tarr_00, RG2L, RG4L);
  SKW_DISPLAY_HINT_TEXT(lcon(0xf), RG4T);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG6W));
void SKW_ACTIVATE_SHOOTER(xxx* xeaxp, xxx* xedxp)
  RG1L = SKW_ALLOC_NEW_DBITEM(RG4L);
  RG1L = SKW_ALLOC_NEW_DBITEM(RG4L);
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(vo_2c.peek16()), signedlong(vo_28.peek16()));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG61W));
  SKW_CUT_RECORD_FROM(RG1L, vl_14, RG3L, NULL);
  RG5L = SKW_GET_TILE_RECORD_LINK(vl_14, vl_08);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG5W));
  SKW_CUT_RECORD_FROM(RG1L, RG2L, RG3L, NULL);
  RG1L = SKW_RAND01() + unsignedlong(xw_24.peek16());
  SKW_SHOOT_ITEM(RG1L, vl_18, vl_00, RG3L, parl00, parb01, lcon(0x64), parb03);
  SKW_SHOOT_ITEM(RG1L, vl_18, vl_00, RG3L, parl04, parb05, lcon(0x64), parb07);
void SKW_MOVE_ITEM_TO(i32 eaxl, xxx* xedxp, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1, i32 argl2, i32 argl3, i32 argl4)
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG4L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L);
  RG1L = SKW_GET_MAX_CHARGE(unsignedlong(RG5W));
  RG1L = SKW_RAND16(SKW_GET_MAX_CHARGE(unsignedlong(RG5W)) + 1 & lcon(0xffff));
  SKW_ADD_ITEM_CHARGE(unsignedlong(RG5W), signedlong(xw_00.peek16()));
  RG1L = (SKW_GET_TILE_VALUE(signedlong(argw0), signedlong(argw1)) & lcon(0xff)) >> bcon(0x5);
  RG1L = SKW_RAND16(lcon(0x4));
  SKW_MOVE_RECORD_TO(RG1L, RG4L, signedlong(RG3W), lcon(0xffffffff), parw00);
  SKW_MOVE_RECORD_TO(RG1L, lcon(0xffffffff), RG2L, RG3L, parw01);
void SKW_ACTIVATE_ITEM_TELEPORT(xxx* xeaxp, xxx* xedxp, i32 ebxl, i32 ecxl, i32 argl0, xxx* xargp1, i32 argl2, i32 argl3)
  RG1L = SKW_GET_TILE_RECORD_LINK(RG3L, RG2L);
  RG1L = (SKW_GET_TILE_VALUE(RG3L, RG4L) & lcon(0xff)) >> bcon(0x5);
  RG1P = DM2_ALLOC_LOBIGPOOL_MEMORY(lcon(0x40));
  SKW_QUERY_CREATURES_ITEM_MASK(RG1L, RG2P, RG3L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG5W));
  vl_2c = SKW_GET_NEXT_RECORD_LINK(RG4L);
  RG1L = SKW_GET_DISTINCTIVE_ITEMTYPE(RG4L);
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG4L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG4L);
  SKW_CUT_RECORD_FROM(RG1L, RG2L, RG3L, xargp1);
  SKW_DEALLOC_RECORD(vl_08);
  RG1L = SKW_ADD_ITEM_CHARGE(vl_08, lcon(0x0)) & lcon(0xffff);
  SKW_BRING_CHAMPION_TO_LIFE(RG1L);
  SKW_MOVE_ITEM_TO(RG1L, RG4P, RG2L, RG3L, parw00, parw01, parl02, parl03, parl04);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG5W));
  SKW_ACTIVATE_ITEM_TELEPORT(DOWNCAST(xxx, xp_00), RG4P, RG2L, RG3L, parl05, xparp06, parl07, parl08);
  DM2_DEALLOC_LOBIGPOOL(lcon(0x40));
i32 SKW_ACTUATE_WALL_MECHA(c_tim* eaxp_c38)
  RG1L = SKW_GET_TILE_RECORD_LINK(unsignedlong(byte_at(RG6p, lcon(0x6))), unsignedlong(byte_at(RG1P, lcon(0x7))));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  SKW_QUEUE_NOISE_GEN1(lcon(0x3), RG2Blo, RG3W, RG4Blo, parw01, parw02, parw03, lcon(0x1));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  RG1L = SKW_LOCATE_OTHER_LEVEL(signedlong(ddat.v1e0266), &vw_0c, &vw_28, RG4L, NULL);
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(vw_0c), RG4L);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG1L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG2W));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_14.peek16()));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG2L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  SKW_ACTIVATE_SHOOTER(RG1P, RG4P);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  SKW_FIRE_SELECT_PALETTE_SET(RG1Blo);
  SKW_SK_PREPARE_EXIT();
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_00)));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  RG1L = SKW_3a15_1da8(unsignedlong(byte_at(RG6p, lcon(0x9))), RG4L);
  SKW_ACTIVATE_TICK_GENERATOR(RG1P, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  SKW_ACTIVATE_RELAY1(RG6p, RG2L, RG4P);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  RG1L = SKW_3a15_1da8(signedlong(CUTX16(unsignedlong(byte_at(RG6p, lcon(0x9))))), RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  SKW_ACTIVATE_CONTINUOUS_ORNATE_ANIMATOR(unsignedlong(vql_24.peek16()), RG2P, RG3L, RG6p);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  SKW_INVOKE_ACTUATOR(RG1P, RG2L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  RG1L = SKW_RAND02();
  RG1L = SKW_ALLOC_NEW_CREATURE(unsignedlong(xwc_10.peek16()), RG4L, RG2L, RG3L, parl05);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG1W));
  SKW_QUEUE_NOISE_GEN1(lcon(0x3), RG2Blo, RG3W, RG4Blo, parw01, parw02, parw03, lcon(0x1));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  RG1L = SKW_QUEUE_TIMER(&c38_00);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  SKW_INVOKE_ACTUATOR(RG1P, RG2L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  SKW_ACTIVATE_ORNATE_ANIMATOR(RG6p, RG2L, RG3L, RG4P);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  SKW_ACTIVATE_RELAY2(RG6p, RG2L, RG4P);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  SKW_ACTIVATE_ITEM_TELEPORT(RG6p, RG4P, RG2L, RG3L, parl06, xparp07, parl08, parl09);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  RG1L = SKW_GET_ORNATE_ANIM_LEN(RG52p, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  SKW_3a15_0d5c(RG6p, RG4P);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  SKW_ACTIVATE_INVERSE_FLAG(RG6p, RG4P);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  SKW_ACTIVATE_TEST_FLAG(RG6p, RG4P);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  RG1P = SKW_GET_ADDRESS_OF_TILE_RECORD(signedlong(RG1W), RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  RG1L = SKW_ALLOC_NEW_RECORD(SKW_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(unsignedlong(RG1W)) & lcon(0xffff));
  RG1L = SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(unsignedlong(RG1W)) & lcon(0xff);
  SKW_SET_ITEMTYPE(RG1L, RG4L);
  SKW_SET_ITEM_IMPORTANCE(RG1L, RG4L);
  SKW_MOVE_ITEM_TO(RG1L, RG4P, RG2L, RG3L, parw10, parw11, parl12, parl13, parl14);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  SKW_ACTIVATE_ITEM_TELEPORT(RG6p, RG4P, RG2L, RG3L, parl06, xparp07, parl08, parl09);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  SKW_ACTIVATE_ITEM_TELEPORT(RG6p, RG4P, RG2L, RG3L, parl06, xparp07, parl08, parl09);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  RG1L = SKW_RAND16(lcon(0x64)) & lcon(0xffff);
  SKW_INVOKE_ACTUATOR(RG1P, RG2L, RG4L);
  SKW_INVOKE_ACTUATOR(RG1P, RG2L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
  SKW_INVOKE_ACTUATOR(RG1P, RG2L, RG4L);
  SKW_INVOKE_ACTUATOR(RG1P, RG2L, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(vql_24.peek16()));
void SKW_ADVANCE_TILES_TIME(i32 eaxl, i32 edxl)
  SKW_MOVE_RECORD_TO(lcon(0xffff), RG4L, RG2L, RG3L, parw00);
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(vql_04.peek16()), unsignedlong(vql_00.peek16()));
  SKW_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw01);
  RG1L = SKW_GET_WALL_TILE_ANYITEM_RECORD(signedlong(vql_04.peek16()), RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG5W));
  RG1L = SKW_GET_NEXT_RECORD_LINK(RG63L);
  SKW_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw02);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG63L);
i32 SKW_ACTUATE_TELEPORTER(c_tim* eaxp_c38)
  RG1P = SKW_GET_ADDRESS_OF_TILE_RECORD(signedlong(RG6w), RG4L);
  return SKW_ACTUATE_FLOOR_MECHA(UPCAST(c_tim, RG2P));
  SKW_ADVANCE_TILES_TIME(RG1L, RG4L);
  return SKW_ACTUATE_FLOOR_MECHA(UPCAST(c_tim, RG2P));
  return SKW_ACTUATE_FLOOR_MECHA(UPCAST(c_tim, RG2P));
i32 SKW_ACTUATE_PITFALL(c_tim* eaxp_c38)
  SKW_ADVANCE_TILES_TIME(RG1L, RG4L);
  return SKW_ACTUATE_FLOOR_MECHA(UPCAST(c_tim, RG2P));
  return SKW_ACTUATE_FLOOR_MECHA(UPCAST(c_tim, RG2P));
void SKW_PROCESS_TIMER_3D(c_tim* eaxp_c38)
  RG1L = SKW_MOVE_RECORD_TO(unsignedlong(esip_c38->w_08), lcon(0xfffffffd), lcon(0x0), RG6L, RG7W);
  SKW_QUEUE_NOISE_GEN1(lcon(0x3), lcon(0x89), lcon(0x61), lcon(0x0), lcon(0x80), RG6W, RG7W, lcon(0x1));
void SKW_PROCESS_TIMER_RESURRECTION(c_tim* eaxp_c38)
  SKW_BRING_CHAMPION_TO_LIFE(unsignedlong(xwc_0c.peek16()));
  RG1L = SKW_GET_TILE_RECORD_LINK(signedlong(vq_00.peek16()), signedlong(vq_04.peek16()));
  RG1L = SKW_QUERY_CLS1_FROM_RECORD(RG5L);
  RG1L = SKW_QUERY_CLS2_FROM_RECORD(RG5L);
  RG1L = signedlong(CUTX16(SKW_ADD_ITEM_CHARGE(RG5L, RG4L)));
  SKW_CUT_RECORD_FROM(RG5L, signedlong(vq_00.peek16()), signedlong(vq_04.peek16()), NULL);
  SKW_DEALLOC_RECORD(RG5L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG2W));
  RG1L = SKW_CREATE_CLOUD(lcon(0xffe4), lcon(0x0), unsignedlong(vq_00.peek16()), unsignedlong(vq_04.peek16()), parl00);
  SKW_QUEUE_TIMER(UPCAST(c_tim, RG6p));
i32 SKW_PROCESS_TIMER_LIGHT(c_tim* eaxp_c38)
  return SKW_QUEUE_TIMER(&c38_00);
void SKW_PROCESS_TIMER_0E(c_tim* eaxp_c38, i32 edxl)
  ebppb = DOWNCAST(xxx, DM2_ALLOC_LOBIGPOOL_MEMORY(RG1L));
  bp_00 = DOWNCAST(xxx, SKW_GET_ADDRESS_OF_RECORD(RG6L));
  SKW_COPY_MEMORY(bp_00, RG5L, ebppb);
  SKW_SET_ITEMTYPE(RG6L, unsignedlong(byte_at(RG3P, lcon(0x8))));
  SKW_PROCESS_ITEM_BONUS(unsignedlong(byte_at(RG3P, lcon(0x5))), lcon(0xffffffff), signedlong(xw_04.peek16()), RG6L);
  SKW_COPY_MEMORY(ebppb, RG5L, bp_00);
  DM2_DEALLOC_LOBIGPOOL(RG5L);
i32 SKW_CONTINUE_ORNATE_ANIMATOR(c_tim* eaxp_c38)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG3P, lcon(0x6))));
  RG1L = SKW_GET_ORNATE_ANIM_LEN(RG1P, RG4L);
  return SKW_QUEUE_TIMER(UPCAST(c_tim, RG3P));
void SKW_ACTIVATE_TICK_GENERATOR(xxx* xeaxp, i32 edxl)
  SKW_QUEUE_TIMER(UPCAST(c_tim, ADRESSOF(c_tim, &c38_00)));
void SKW_CONTINUE_TICK_GENERATOR(c_tim* eaxp_c38)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG3P, lcon(0x6))));
  SKW_INVOKE_ACTUATOR(RG1P, RG2L, RG4L);
  SKW_QUEUE_TIMER(UPCAST(c_tim, RG3P));
void SKW_PROCESS_ACTUATOR_TICK_GENERATOR(void)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG3W));
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  SKW_ACTIVATE_TICK_GENERATOR(RG1P, RG4L);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG3W));
void SKW_PROCESS_TIMER_RELEASE_DOOR_BUTTON(c_tim* eaxp_c38)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(DOWNCAST(c_tim, eaxp_c38), lcon(0x6))));
void SKW_PROCESS_TIMER_59(c_tim* eaxp_c38)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(DOWNCAST(c_tim, eaxp_c38), lcon(0x8))));
void SKW_CONTINUE_ORNATE_NOISE(c_tim* eaxp_c38)
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG2P, lcon(0x8))));
  RG1L = (SKW_GET_TILE_VALUE(unsignedlong(byte_at(RG2P, lcon(0x6))), RG4L) & lcon(0xff)) >> bcon(0x5);
  RG1L = SKW_GET_FLOOR_DECORATION_OF_ACTUATOR(RG3P);
  RG1L = SKW_GET_WALL_DECORATION_OF_ACTUATOR(RG3P);
  RG1L = SKW_GET_ORNATE_ANIM_LEN(RG3P, RG4L) & lcon(0xffff);
  SKW_QUEUE_TIMER(UPCAST(c_tim, RG2P));
  SKW_QUEUE_NOISE_GEN2(vb_04, lcon(0x88), lcon(0xfe), vb_00, parw00, parw01, lcon(0x1), lcon(0x8c), lcon(0x80));
void SKW_3a15_38b6(i32 eaxl)
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(xw_20.peek16()));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(RG6W));
  RG1L = SKW_RAND02();
  SKW_ALLOC_NEW_CREATURE(RG1L, lcon(0x7), RG2L, RG3L, parl02);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(RG7L);
  SKW_INVOKE_ACTUATOR(RG1P, RG2L, RG4L);
  RG1L = SKW_GET_ORNATE_ANIM_LEN(RG52p, RG4L) & lcon(0xffff);
  SKW_TRY_ORNATE_NOISE(RG1P, RG4L, RG2L, RG3L, parw00, parl01);
  RG1L = SKW_GET_NEXT_RECORD_LINK(unsignedlong(RG6W));
void SKW_PROCEED_TIMERS(void)
  RG1L = SKW_IS_TIMER_TO_PROCEED();
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
  SKW_IBMIO_USER_INPUT_CHECK();
  SKW_GET_AND_DELETE_NEXT_TIMER(UPCAST(c_tim, RG5p));
  SKW_CHANGE_CURRENT_MAP_TO(RG1L);
  SKW_PROCESS_TIMER_0C(RG1L);
  SKW_PROCESS_TIMER_RESURRECTION(UPCAST(c_tim, RG5p));
  SKW_STEP_DOOR(UPCAST(c_tim, RG5p));
  SKW_PROCESS_TIMER_DESTROY_DOOR(UPCAST(c_tim, RG5p));
  SKW_PROCESS_CLOUD(UPCAST(c_tim, RG5p));
  SKW_PROCESS_SOUND(c38_00.w_06);
  SKW_STEP_MISSILE(UPCAST(c_tim, RG5p));
  SKW_THINK_CREATURE(RG1L, RG2L, RG4L);
  SKW_PROCESS_TIMER_3D(UPCAST(c_tim, RG5p));
  SKW_CHANGE_CURRENT_MAP_TO(signedlong(ddat.v1e0266));
  SKW_PROCESS_TIMER_LIGHT(UPCAST(c_tim, RG5p));
  SKW_RECALC_LIGHT_LEVEL();
  SKW_PROCESS_TIMER_0E(UPCAST(c_tim, RG5p), RG4L);
  SKW_PROCESS_POISON(RG1L, RG4L);
  SKW_UPDATE_WEATHER(lcon(0x1));
  SKW_CONTINUE_ORNATE_ANIMATOR(UPCAST(c_tim, RG5p));
  SKW_CONTINUE_TICK_GENERATOR(UPCAST(c_tim, RG5p));
  SKW_PROCESS_TIMER_RELEASE_DOOR_BUTTON(UPCAST(c_tim, RG5p));
  SKW_PROCESS_TIMER_59(UPCAST(c_tim, RG5p));
  SKW_CONTINUE_ORNATE_NOISE(UPCAST(c_tim, RG5p));
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG5p, lcon(0x6))));
  SKW_MOVE_RECORD_TO(RG1L, RG4L, RG2L, RG3L, parw00);
  SKW_ROTATE_SQUAD(RG1L);
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND02();
  RG1L = SKW_CALC_VECTOR_DIR(unsignedword(c38_00.b_06), RG2W, RG3W, RG4W);
  SKW_ALLOC_NEW_CREATURE(RG1L, RG4L, RG2L, RG3L, parl01);
  RG1P = SKW_GET_ADDRESS_OF_RECORD(unsignedlong(word_at(RG5p, lcon(0x6))));
  SKW_ACTUATE_WALL_MECHA(UPCAST(c_tim, RG5p));
  SKW_ACTUATE_FLOOR_MECHA(UPCAST(c_tim, RG5p));
  SKW_ACTUATE_PITFALL(UPCAST(c_tim, RG5p));
  SKW_ACTUATE_DOOR(UPCAST(c_tim, RG5p));
  SKW_ACTUATE_TELEPORTER(UPCAST(c_tim, RG5p));
  SKW_ACTUATE_TRICKWALL(UPCAST(c_tim, RG5p));
i32 SKW_SET_TIMER_WEATHER(i32 eaxl)
  return SKW_QUEUE_TIMER(&c38_00);
i32 SKW_3df7_0037(i32 eaxl)
  SKW_UPDATE_GLOB_VAR(lcon(0x40), lcon(0x6), lcon(0x0));
  RG1L = SKW_RAND16(lcon(0x1f40));
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND16(lcon(0x3));
  RG1L = SKW_RAND16(lcon(0x1f4));
  RG1L = SKW_RAND02();
  SKW_SET_TIMER_WEATHER(RG4L);
  RG1L = SKW_RAND16(lcon(0x4)) + lcon(0x4);
void SKW_UPDATE_WEATHER(i32 eaxl)
  SKW_3df7_0037(lcon(0x0));
  RG1L = SKW_BETWEEN_VALUE16(lcon(0x0), RG2L, RG4L);
  RG1L = (SKW_RAND16(lcon(0x100)) & lcon(0xffff)) + lcon(0x32);
  SKW_SET_TIMER_WEATHER(RG1L);
  RG1L = SKW_RECALC_LIGHT_LEVEL();
  RG1L = SKW_RAND16(lcon(0x40));
  RG1L = SKW_RAND();
  RG1L = SKW_RAND16(unsignedlong(RG2W)) & lcon(0xffff);
  RG1L = SKW_RAND16(unsignedlong(RG2W));
  RG1L = SKW_RECALC_LIGHT_LEVEL();
  RG1L = SKW_RETRIEVE_ENVIRONMENT_CMD_CD_FW(DOWNCAST(xxx, xp_1c));
  RG1L = SKW_RETRIEVE_ENVIRONMENT_CMD_CD_FW(DOWNCAST(xxx, xp_1c));
  SKW_UPDATE_GLOB_VAR(lcon(0x40), lcon(0x6), lcon(0x0));
  RG1L = SKW_RAND16(unsignedlong(mkuw(ddat.v1e1474 + 1))) & lcon(0xffff);
  RG1L = SKW_RAND();
  RG1L = SKW_RAND16(lcon(0x20));
  RG5L = SKW_RAND16(lcon(0x20));
  SKW_SUMMARIZE_STONE_ROOM(UPCAST(s_8wordarray, ADRESSOF(s_8wordarray, &s15_00)), RG2L, RG3L, lcon(0x0));
  RG1L = SKW_GET_TELEPORTER_DETAIL(&c12_10, RG2L, RG4L);
  RG1L = SKW_GET_GLOB_VAR(lcon(0x40));
  RG1L = SKW_GET_CREATURE_AT(unsignedlong(RG61w), RG4L);
  RG1L = unsignedlong(byte_at(SKW_QUERY_CREATURE_AI_SPEC_FROM_TYPE(unsignedlong(RG1W)), lcon(0x2)));
  RG1L = signedlong(SKW_CALC_SQUARE_DISTANCE(ddat.v1e0270, RG2W, RG3W, RG4W));
  RG4L = unsignedlong(CUTX16(SKW_CALC_VECTOR_DIR(ddat.v1e0270, RG2W, RG3W, RG4W)));
  SKW_CREATE_CLOUD(lcon(0xffb0), unsignedlong(ddat.v1e1474), RG2L, RG3L, parl06);
  RG1L = signedlong(SKW_CALC_SQUARE_DISTANCE(ddat.v1e0270, RG73W, RG5W, RG4W));
  RG1L = SKW_CALC_VECTOR_DIR(ddat.v1e0270, RG73W, RG5W, RG4W);
  SKW_UPDATE_GLOB_VAR(lcon(0x41), lcon(0x3), lcon(0x1));
  SKW_INVOKE_MESSAGE(RG1L, lcon(0x0), lcon(0x0), RG4L, parl00);
  RG1L = signedlong(CUTX16(SKW_RAND16(lcon(0x20)) + lcon(0x80)));
  SKW_QUEUE_NOISE_GEN2(lcon(0xd), lcon(0x81), lcon(0xfe), lcon(0x30), parw01, parw02, lcon(0x1), lcon(0x6c), parw05);
  RG1L = SKW_RAND01();
  RG1L = SKW_RAND16(lcon(0x3));
  RG1L = SKW_RETRIEVE_ENVIRONMENT_CMD_CD_FW(RG4P);
  RG1L = SKW_RAND16(lcon(0x64));
  SKW_098d_04c7(RG1W, RG2W, RG3P16, RG4W, XUPCAST(i16, xparp07));
  RG1L = SKW_RAND02();
  RG1L = SKW_RAND16(lcon(0xa)) + lcon(0x5);
  RG1L = signedlong(CUTX16(SKW_BETWEEN_VALUE16(lcon(0x1), RG2L, RG4L)));
  SKW_QUEUE_NOISE_GEN1(lcon(0x17), lcon(0x0), lcon(0x19), ddat.v1d6c02, lcon(0x40), parw09, parw10, parw11);
  SKW_RECALC_LIGHT_LEVEL();
i32 SKW_RETRIEVE_ENVIRONMENT_CMD_CD_FW(xxx* xeaxp)
  RG1P = DOWNCAST(t_text, SKW_QUERY_GDAT_TEXT(lcon(0x17), RG2L, RG3T, RG4L));
  RG1L = SKW_QUERY_CMDSTR_TEXT(tarr_00, RG4T);
  RG1L = SKW_QUERY_CMDSTR_TEXT(tarr_00, RG4T);
void R_1583(i32 eaxl)
  SKW_COPY_TO_DRIVER(DOWNCAST(i16, &vw_00), lcon(0x2)); // CAST stays
i32 SKW_44c8_1baf(xxx* xeaxp)
  SKW_COPY_MEMORY(xeaxp, lcon(0x400), ddat.drvinterfaceptr);
void SKW_44c8_1c99(void)
  SKW_FIRE_HIDE_MOUSE_CURSOR();
  SKW_QUERY_EXPANDED_RECT(lcon(0xf), RG4R);
  SKW_COPY_MEMORY(ADRESSOF(c_rect, &rc_00), lcon(0x8), ddat.drvinterfaceptr);
  SKW_FIRE_BLIT_PICTURE(ddat.v1e1414, &rc_00, lcon(0x0), ddat.screenptr, lcon(0x0), parw01, parw02, lcon(0xffff), lcon(0x0), lcon(0x8), lcon(0x8), NULL);
  SKW_FIRE_SHOW_MOUSE_CURSOR();
i32 SKW_44c8_20a4(xxx* xeaxp, xxx* xebxp, c_rect* ecxrp, t_gfxdata* edxpgarr, i16 argw0, i16 argw1, i16 argw2, i16 argw3, t_colidx* argpcarr4)
  return SKW_44c8_1e43(xeaxp, xebxp, ecxrp, edxpgarr, unsignedlong(argw0), unsignedlong(argw1), lcon(0x0), lcon(0x0), signedlong(ecxrp->w_04_w), signedlong(argw2), argw3, argpcarr4);
void SKW_443c_067a(xxx* xeaxp)
  RG1P = DM2_ALLOC_MEMORY_RAM(lcon(0xc), MASK_200, FREEPOOL); // TODO: allocating with non-standard mask
void R_4DA3(i32 eaxl, i16* ebxpw, s_8bytearray* ecxp_s09, i32 edxl)
  RG1P = SKW_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0xf), lcon(0x7), lcon(0xfd), CUTLX8(RG4L));
  RG1P = RG4P + 8 * (R_4E26(RG6p16) + vl_00 & lcon(0xffff));
  SKW_COPY_MEMORY(RG1P, lcon(0x8), RG5p);
void R_4DEA(i32 eaxl, i32 edxl, i16* ebxpw, i32* ecxpl)
  SKW_COPY_MEMORY(SKW_QUERY_GDAT_ENTRY_DATA_PTR(lcon(0xf), lcon(0x7), lcon(0xfc), RG1Blo) + 4 * (R_4E26(ebxpw) + edxl & lcon(0xffff)), lcon(0x4), DOWNCAST(i32, ecxpl));
void SKW_075f_056c(i32 eaxl)
  RG1W = word_at(SKW_GET_ADDRESS_OF_RECORD(RG1L), lcon(0x6));
  SKW_DELETE_TIMER(RG1L);
void SKW_DISPLAY_TAKEN_ITEM_NAME(i16 wn)
  SKW_DISPLAY_HINT_NEW_LINE();
  SKW_DISPLAY_HINT_TEXT(lcon(0xd), SKW_GET_ITEM_NAME(wn));
void SKW_COPY_TO_DRIVER(xxx* buffer, i16 wamount)
  SKW_COPY_MEMORY(buffer, wamount, ddat.drvinterfaceptr);

i32 R_4E26(i16* eaxpw)
i32 SKW_GET_GLOB_VAR(i32 eaxl)
i32 SKW_48ae_01af(i32 eaxl, i32 edxl)
i32 SKW_48ae_0767(i32 eaxl, i16* ebxpw, xxx* xecxp, i32 edxl)
i32 SKW_DIR_FROM_5x5_POS(i32 eaxl)
void SKW_098d_000f(i32 eaxl, i32 ebxl, i16* ecxpw, i32 edxl, i16* argpw0)
i32 SKW_0b36_068f(c_image* eaxp_c37)
void SKW_CHANGE_CURRENT_MAP_TO(i32 eaxl)
i32 SKW_CALC_VECTOR_W_DIR(i32 eaxl, i32 ebxl, i16* ecxpw, i32 edxl, i16* argpw0)
xxx* SKW_GET_ADDRESS_OF_RECORD(i32 eaxl)
i32 SKW_GET_OBJECT_INDEX_FROM_TILE(i32 eaxl, i32 edxl)
i32 SKW_0cee_17e7(i32 eaxl, i32 ebxl, i32 edxl)
i32 SKW_GET_ITEMDB_OF_ITEMSPEC_ACTUATOR(i32 eaxl)
i32 SKW_GET_ITEMTYPE_OF_ITEMSPEC_ACTUATOR(i32 eaxl)
i32 SKW_GET_MAX_CHARGE(i32 eaxl)
i32 SKW_QUERY_GDAT_CREATURE_WORD_VALUE(i32 eaxl, i32 edxl)
i32 SKW_GET_WALL_DECORATION_OF_ACTUATOR(xxx* xeaxp)
i32 SKW_GET_FLOOR_DECORATION_OF_ACTUATOR(xxx* xeaxp)
i32 SKW_IS_WALL_ORNATE_ALCOVE(i32 eaxl)
i32 SKW_IS_REBIRTH_ALTAR(xxx* xeaxp)
i32 SKW_GET_GRAPHICS_FOR_DOOR(i32 eaxl)
i32 SKW_0cee_3275(i32 eaxl)
i32 SKW_GET_DOOR_STAT_0X10(i32 eaxl)
i32 SKW_QUERY_DOOR_DAMAGE_RESIST(i32 eaxl)
i32 SKW_RETURN_1(void)
i32 SKW_IS_GAME_ENDED(xxx* xeaxp)
i32 SKW_1031_0023(xxx* xeaxp)
i32 SKW_1031_003e(xxx* xeaxp)
i32 SKW_1031_007b(xxx* xeaxp)
i32 SKW_1031_00c5(xxx* xeaxp)
i32 SKW_1031_012d(xxx* xeaxp)
i32 SKW_1031_014f(xxx* xeaxp)
i32 SKW_1031_0184(xxx* xeaxp)
i32 SKW_1031_01ba(xxx* xeaxp)
xxx* SKW_1031_023b(xxx* xeaxp)
s_www* SKW_1031_024c(xxx* xeaxp)
i32 SKW_SELECT_CHAMPION_LEADER(i32 eaxl)
void SKW_RESET_SQUAD_DIR(void)
i32 SKW_14cd_062e(void)
void SKW_14cd_0802(void)
i32 SKW_DECIDE_NEXT_XACT(i32 eaxl)
i32 SKW_14cd_08f5(i32 eaxl)
void SKW_PROCEED_XACT_69(void)
i32 SKW_PROCEED_XACT_73(void)
i32 SKW_PROCEED_XACT_86(void)
i32 SKW_PROCEED_XACT_72_87_88(i32 eaxl)
i16* SKW_1c9a_02c3(xxx* xeaxp, xxx* xedxp)
i32 SKW_COMPUTE_POWER_4_WITHIN(i32 eaxl, i32 edxl)
i32 SKW_1c9a_0598(i32 eaxl)
i32 R_1FE74(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0)
i32 SKW_1c9a_08bd(xxx* xeaxp)
void SKW_ADD_INDEX_TO_POSSESSION_INDICES(i32 eaxl)
void SKW_2066_1ea3(i32 eaxl, i32 ebxl, i32 edxl)
void SKW_MARK_DYN_LOAD(i32 eaxl)
i32 SKW_FIND_POUCH_OR_SCABBARD_POSSESSION_POS(i32 eaxl, i32 edxl)
void SKW_ADJUST_HAND_COOLDOWN(i32 eaxl, i32 ebxl, i32 edxl)
void SKW_CLICK_MAGICAL_MAP_RUNE(i32 eaxl)
xxx* SKW_FIND_SPELL_BY_RUNES(xxx* xeaxp)
void SKW_ROTATE_SQUAD(i32 eaxl)
i32 SKW_GET_PLAYER_AT_POSITION(i32 eaxl)
i32 SKW_2c1d_09d9(void)
i32 SKW_STAMINA_ADJUSTED_ATTR(xxx* xeaxp, i32 edxl)
i32 SKW_GET_PLAYER_WEIGHT(i32 eaxl)
void SKW_SET_PARTY_HERO_FLAG(i32 eaxl)
i32 SKW_ROTATE_RECORD_BY_TELEPORTER(xxx* xeaxp, i32 edxl)
i32 SKW_IS_TILE_BLOCKED(i32 eaxl)
i32 R_3DC4C(i32 eaxl)
void SKW_QUERY_RAINFALL_PARAM(xxx* xeaxp, i16* edxpw)
i32 SKW_SKCHR_TO_SCRIPTCHR(i32 eaxl)
i32 SKW_32cb_35c1(i16* eaxpw, i32 ebxl, i32 ecxl, i16* edxpw)
i32 SKW_32cb_54ce(i32 eaxl, i16* ebxpw, i32 ecxl, i16* edxpw, i32 argl0)
i32 SKW_SET_GRAPHICS_FLIP_FROM_POSITION(i32 eaxl, i32 ebxl, i32 ecxl, i32 edxl)
void SKW_32cb_5a8f(void)
i32 SKW_3929_04e2(t_text* eaxtp, i16* ebxpw, i32 ecxl, t_text* edxtp)
void SKW_REARRANGE_TIMERLIST(void)
i32 SKW_3a15_0381(c_tim* eaxp_c38, c_tim* edxp_c38) // TODO: check pointer types. TODO: return is boolean
i32 SKW_PROCESS_TIMER_DESTROY_DOOR(c_tim* eaxp_c38)
i32 SKW_3a15_1da8(i32 eaxl, i32 edxl)
void SKW_PROCESS_TIMER_0C(i32 eaxl)
xxx* R_4A986(i32 eaxl)
void R_4A9A4(void)
i32 R_52DC0(xxx* xeaxp, xxx* xedxp)
i32 SKW_44c8_1e1a(xxx* xeaxp, i32 edxl)
void SKW_443c_08ab(i16* eaxpw, i16* ebxpw, i16* edxpw)
i16 R_1456(void)
void R_148D(i16 wdummy)
i16 SKW_476d_05b6(i16 wn)
void SKW_443c_087c(void)
void SKW_443c_0889(void)
