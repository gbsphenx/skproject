#ifndef DOSVIDEO_H
  #define DOSVIDEO_H


#ifndef __DJGPP__
#define __djgpp_conventional_base 0
#else
#include <sys/nearptr.h>
#endif // __DJGPP__


void DOS_VIDEO_STATIC_TEST();
void DOS_VIDEO_INIT_VGA_MODE();
void DOS_VIDEO_RESET_MODE();
void DOS_VIDEO_UPDATE_SCREEN(unsigned char* bptr);


#endif // DOSVIDEO_H
