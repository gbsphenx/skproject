#ifndef STARTEND_H
  #define STARTEND_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  #define THROW_DMABORT (unsigned int)(0)
  #define THROW_CLOSEWIN (unsigned int)(1)
  #define THROW_PREPARE_EXIT (unsigned int)(2)
  #define THROW_SYS_EXIT  0x100 // low two digits preserved for errorcode

  typedef void (*t_exitfunc)(ui8 ua, ui8 ub);

  struct s_exittableentry
  {
    ui8 ubv1;
    ui8 ubv2;
    t_exitfunc func;
  }; // sizeof == 6

  i16 DM2_WATCOM_ERRHANDLING(i16 nw);
  void DM2_WATCOM_MARK_ERROR(i32 nl);
  void DM2_PREPARE_EXIT(void);
  // void WATCOM_PRE_EXIT(ui8 ua, ui8 ub);
  void SYS_exit(i8 exitcode);
  void DM2_RAISE_SYSERR(i16 eaxw);

  void DM2_WATCOM_STARTUP(void);

  i32 DM2__INIT_GAME_38c8_03ad(void);
  void DM2_END_GAME(i16 eaxw);

  void DM2_SHOW_CREDITS(void);

#endif
