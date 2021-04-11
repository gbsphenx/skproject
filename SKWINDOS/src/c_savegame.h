#ifndef C_SAVEGAME_H
  #define C_SAVEGAME_H

  #ifndef TYPES_H
    #include "..\types.h"
  #endif

// SPX: transform some void into int to avoid "error C2562: 'SKW_GAME_SAVE_MENU' : 'void' function returning a value"
#define	FAKE_VOID	int


  bool SKW_SKLOAD_READ(xxx* eaxp, x16 edxw);
  void SKW_SUPPRESS_INIT(void);
  bool SKW_SUPPRESS_READER(xxx* eaxp, x32 ebxl, x32 ecxl, const xxx* edxp, x32 argl0);
  bool SKW_READ_RECORD_CHECKCODE(x32 eaxl, x16* ebxp, bool ecxbool, x16 edxw, bool argbool0);
  x16 SKW_GAME_LOAD(void);

  FAKE_VOID SKW_GAME_SAVE_MENU(void);

#endif
