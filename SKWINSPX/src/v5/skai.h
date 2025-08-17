#ifndef C_AI_H
  #define C_AI_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  void DM2_THINK_CREATURE(i32 eaxl, i32 edxl, i32 ebxl);

  unk* DM2_PREPARE_LOCAL_CREATURE_VAR(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl);
  void DM2_UNPREPARE_LOCAL_CREATURE_VAR(unk* xeaxp);
  void DM2_ai_13e4_0360(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i32 argl0);
  void DM2_ai_13e4_071b(void);
  void DM2_ai_13e4_0806(void);

#endif
