#ifndef C_TIM_PROC_H
  #define C_TIM_PROC_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  class c_tim;

  void DM2_PROCESS_TIMER_0E(c_tim* eaxtimp, i32 edxl);
  void DM2_PROCEED_TIMERS(void);
  void DM2_INVOKE_MESSAGE(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0);
  void DM2_INVOKE_ACTUATOR(unk* xeaxp, i32 edxl, i32 ebxl);
  void DM2_PROCESS_ACTUATOR_TICK_GENERATOR(void);
  i32 DM2_OPERATE_PIT_TELE_TILE(i32 eaxl, i32 edxl, i32 ebxl);
  void DM2_TRY_ORNATE_NOISE(unk* xeaxp, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1);

#endif
