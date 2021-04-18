#ifndef DM2GLOBAL_H
  #define DM2GLOBAL_H

  #include <stdio.h> // FILE

  #ifndef TYPES_H
    #include "types.h"
  #endif

  #define MUSICDATAPATH "./music.bin"

  #define MEGA16       (16 * 1024L * 1024L)

  #define ORIG_COLOR_DEPTH (8)
  #define ORIG_SWIDTH  (0x140)
  #define ORIG_SHEIGHT (200)


  extern bool close_win;
  extern int restart;
  extern bool main_exit;
  extern bool enable_video;
  extern bool enable_drawing;

  enum
  {
    RESTART_NOTHING=0,
    RESTART_RESUME,
    RESTART_QUIT
  };

  extern FILE* logfile;

// SPX: Added functions to display/debug
	void SPX_DEBUG_FUNCTION_INFO(const char* sFunctionName);
	void SPX_DEBUG_INCREASE_DEEPNESS();
	void SPX_DEBUG_DECREASE_DEEPNESS();
	#define SPX_DEBUG_PUSH		SPX_DEBUG_INCREASE_DEEPNESS();
	#define SPX_DEBUG_POP		SPX_DEBUG_DECREASE_DEEPNESS();
	void SPX_DEBUG_POP_ALL();
	void SPX_DEBUG_PUSH_FUNCTION_INFO(const char* sFunctionName);
	void SPX_DEBUG_MESSAGE_INFO(const char *sMessage, ...);

	void SPX_DEBUG_LOGFILE_INFO(const char *sMessage, ...);

	const char* SPX_STR_DATATYPE(unsigned int iType);

// SPX: Use either one or other to compile "original" code using X8 for category/class/items (but maybe triggering negative values) or UI16 instead (non negative)
#define __GLOBAL_USE_CLASS_X8__			0
#define __GLOBAL_USE_CLASS_UI16__		1

#if __GLOBAL_USE_CLASS_X8__ == 1
#define XCLS16	x8
#else
#define XCLS16	ui16
#endif


#endif
