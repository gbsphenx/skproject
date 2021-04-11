#ifndef DRIVER_H
  #define DRIVER_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  #define SOFS (0x57d - 0x511)  // new: segmentoffset
//  #define ONETFIVE   (0x135 + SOFS)
//  #define ONEFONE    (0x1f1 + SOFS)
  #define TWODSIX    (0x2d6 + SOFS)
  #define FIVEELEVEN (0x511 + SOFS)

  #define DMFCOL   (255.0/63.0)  // factor from 6bit to 8bit - color

  #define userinterrupt(a, b) driver_userint(a, b)

  struct s_mouseentry
  {
    i16 b;
    i16 x;
    i16 y;
  };

  struct s_mouseentry2
  {
    i16 x;
    i16 y;
    i16 b;
  };

  struct s_xmouserect
  {
    c_rect r;
    i16 v;
  };

  // variables & functions provided for the main program

  extern s_mouseentry2 DRV_mouseentry;
  extern i16 DRVW_drvinterface[0x200];
  extern s_xmouserect DRVR_rect1;
  extern i16 DRVW_mousevisicnt;

// Init
  void driver_init(i8* gfxout);

// Timer
  void driver_timerint_T1(void);

// Keyboard
  void driver_keyboardint_T1(i16 keycode);

// Mouse
  void driver_mouseint_T1(i16 b, i16 x, i16 y);

// Interface
  void driver_userint(i16 a, i16 b); // called by userinterrupt only

// Interrupts
  extern bool iflag;

  void cli(void);
  void sti(void);

#endif // DRIVER_H
