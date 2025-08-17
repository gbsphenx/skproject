#ifndef DM2GLOBAL_H
  #define DM2GLOBAL_H

  #include <stdio.h> // FILE

  #ifndef TYPES_H
    #include <types.h>
  #endif

  #define MUSICDATAPATH "./music.bin"

  #define MEGA16       (16 * 1024L * 1024L)

  #define ORIG_COLOR_DEPTH (8)
  #define ORIG_SWIDTH  (320)
  #define ORIG_SHEIGHT (200)

  extern bool close_win;
  extern int sysrestart;
  extern bool main_exit;
  extern i16 disable_video;

  enum
  {
    RESTART_NOTHING = 0,
    RESTART_RESUME,
    RESTART_QUIT
  };

  extern FILE* logfile;

  i32 DM2_UPDATE_GLOB_VAR(i32 eaxl, i32 edxl, i32 ebxl);

#endif
