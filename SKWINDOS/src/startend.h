#ifndef STARTEND_H
  #define STARTEND_H

  #ifndef TYPES_H
    #include "../types.h"
  #endif

  #define THROW_DMABORT (unsigned int)(0)
  #define THROW_CLOSEWIN (unsigned int)(1)
  #define THROW_PREPARE_EXIT (unsigned int)(2)
  #define THROW_SYS_EXIT  0x100 // low two digits preserved for errorcode

  typedef void(*t_exitfunc)(ui8 a, ui8 b);

  typedef struct
  {
    ui8 v1;
    ui8 v2;
    t_exitfunc func;
  } s_exittableentry; // sizeof == 6

  x16 DM2_WATCOM_ERRHANDLING(x16 nw);
  void DM2_WATCOM_MARK_ERROR(x32 nl);
  void SKW_SK_PREPARE_EXIT(void);
  // void WATCOM_PRE_EXIT(ui8 a, ui8 b);
  void SYS_exit(i8 alb);

  void WATCOM_STARTUP(void);

#endif
