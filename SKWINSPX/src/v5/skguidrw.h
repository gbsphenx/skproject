#ifndef C_GUI_DRAW_H
  #define C_GUI_DRAW_H

  #include <stdio.h>

  #ifndef TYPES_H
    #include <types.h>
  #endif

  class c_buttongroup;
  class c_imgdesc;
  class c_hero;
  class c_itemrecord;

  void DM2_DRAW_ICON_PICT_BUFF(t_bmp* srcbmp, c_buttongroup* buttongroup, c_rect* blitrect, i16 srcx, i16 srcy, t_alphamask alphamask, e_blitmode blitmode, t_palette* palette = NULL);
  void DM2_DRAW_ICON_PICT_ENTRY(i8 lpalidx, i8 edxb, i8 ebxb, c_buttongroup* buttongroup, i16 argw0, t_alphamask alphamask = NOALPHA);

  void DM2_DRAW_GAMELOAD_DIALOGUE_TO_SCREEN(void);
  void DM2_DRAW_DIALOGUE_PROGRESS(i32 eaxl);
  void DM2_DRAW_DIALOGUE_PARTS_PICT(t_bmp* bmp, c_rect* edxrp, i16 ebxw, t_palette* palette);
  void DM2_DRAW_DIALOGUE_PICT(t_bmp* srcbmp, t_bmp* destbmp, c_rect* ebxrp, i16 ecxw, i16 argw0, i16 argw1, t_palette* palette);

  void DM2_DRAW_WAKE_UP_TEXT(void);
  void DM2_DRAW_PLAYER_3STAT_HEALTH_BAR(i32 eaxl);
  void DM2_DRAW_CUR_MAX_HMS(i32 eaxl, i32 edxl, i32 ebxl);
  void DM2_DRAW_PLAYER_3STAT_TEXT(c_hero* hero);
  void DM2_DRAW_PLAYER_NAME_AT_CMDSLOT(void);
  void DM2_DRAW_PLAYER_DAMAGE(i32 eaxl);
  void DM2_DRAW_CHIP_OF_MAGIC_MAP(t_bmp* bmp, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0, t_palette* palette);
  void DM2_QUERY_GDAT_SQUAD_ICON(t_bmp* bmp, i32 edxl, t_palette* palette);
  void DM2_DRAW_CRYOCELL_LEVER(i32 eaxl);
  void DM2_DRAW_CHARSHEET_OPTION_ICON(i32 eaxl, i32 edxl, i32 ebxl);
  void DM2_MONEY_BOX_SURVEY(i32 eaxl);
  void DM2_DRAW_MONEYBOX(i16 eaxw);
  void DM2_guidraw_0b36_0c52(c_buttongroup* buttongroup, i32 edxl, i32 ebxl);
  void DM2_DRAW_PLAYER_3STAT_PANE(i32 eaxl, i32 edxl);
  void DM2_guidraw_29ee_00a3(i32 eaxl);
  void DM2_DRAW_CMD_SLOT(i16 eaxw, i8 edxb);
  void DM2_DRAW_SPELL_TO_BE_CAST(i32 eaxl);
  void DM2_DRAW_PLAYER_ATTACK_DIR(void);
  void DM2_DRAW_SPELL_PANEL(void);
  void DM2_SHOW_ATTACK_RESULT(i16 eaxw);
  void DM2_guidraw_0b36_105b(c_buttongroup* buttongroup, c_rect* edxrp, i16 ebxw);
  void DM2_DRAW_SQUAD_SPELL_AND_LEADER_ICON(i16 eaxw, i32 edxl);
  void DM2_guidraw_24a5_0e82(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1);
  void DM2_DRAW_FOOD_WATER_POISON_PANEL(void);
  void DM2_DRAW_ITEM_STATS_BAR(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0);
  i8 DM2_guidraw_2405_014a(i32 eaxl, i32 edxl, i32 ebxl);
  void DM2_DRAW_ITEM_IN_HAND(c_itemrecord* xeaxp);
  void DM2_DRAW_CONTAINER_PANEL(i32 eaxl, i32 edxl);
  void DM2_guidraw_2405_011f(i32 eaxl, c_rect* edxrp);
  void DM2_guidraw_2405_00ec(i32 eaxl, c_rect* edxrp);
  void DM2_DRAW_ITEM_ICON(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0);
  void DM2_DRAW_CONTAINER_SURVEY(unk* xeaxp);
  void DM2_DRAW_EYE_MOUTH_COLORED_RECTANGLE(i8 eaxb, i16 edxw);
  i32 DM2_guidraw_2e62_03b5(i32 eaxl, i32 edxl, i32 ebxl);
  void DM2_DRAW_SCROLL_TEXT(i32 eaxl);
  i32 DM2_DRAW_ITEM_SURVEY(i32 eaxl, i32 edxl);
  void DM2_DRAW_MAP_CHIP(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1, i8 argb2, i32 argl3);
  void DM2_guidraw_29ee_1d03(i8 eaxb);
  void DM2_guidraw_29ee_1946(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1, i32 argl2, i32 argl3);
  void DM2_DRAW_MAJIC_MAP(i32 eaxl);
  void DM2_DISPLAY_HINT_NEW_LINE(void);
  void DM2_DISPLAY_TAKEN_ITEM_NAME(i16 wn);
  void DM2_HIGHLIGHT_ARROW_PANEL(i32 eaxl, i32 edxl, i32 ebxl);
  void DM2_DISPLAY_RIGHT_PANEL_SQUAD_HANDS(void);
  void DM2_REFRESH_PLAYER_STAT_DISP(i16 eaxw);
  i32 DM2_guidraw_29ee_000f(void);
  void DM2_guidraw_24a5_1798(i16 eaxw);
  void DM2_UPDATE_RIGHT_PANEL(i32 eaxl);

#endif
