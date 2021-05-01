#ifndef C_SAVEGAME_H
  #define C_SAVEGAME_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  bool DM2_SKLOAD_READ(unk* xeaxp, i16 edxw);
  i16 DM2_GAME_LOAD(void);

  //void DM2_GAME_SAVE_MENU(void);
  int DM2_GAME_SAVE_MENU(void);	// SPX: changed void to int for compilation convenience

#endif
