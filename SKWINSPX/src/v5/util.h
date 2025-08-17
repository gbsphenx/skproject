#ifndef UTIL_H
  #define UTIL_H

  #include <string.h> // memset

  #ifndef TYPES_H
    #include <types.h>
  #endif

  #define DM2_ZERO_MEMORY(z1, z2) memset(z1, 0, z2) // id: 0x1007
  void DM2_COPY_MEMORY(unk* dest, unk* src, i32 size);

  #define IS_ODD(x)    (((x) & 1) != 0)
  #define MK_EVEN(x)   ((x + 1) & 0xfffffffeL)

  i16 DM2_ABS(i16 wn);
  i16 DM2_MIN(i16 wa, i16 wb);
  i16 DM2_MAX(i16 wa, i16 wb);
  i16 DM2_BETWEEN_VALUE(i16 min, i16 max, i16 wval);
  i16 DM2_CALC_SQUARE_DISTANCE(i16 eaxw, i16 edxw, i16 ebxw, i16 ecxw);
  i16 DM2_CALC_VECTOR_DIR(i16 eaxw, i16 edxw, i16 ebxw, i16 ecxw);
  void DM2_CALC_VECTOR_W_DIR(i16 eaxw, i16 edxw, i16 ebxw, i16* ecxpw, i16* argpw0);
  i32 DM2_COMPUTE_POWER_4_WITHIN(i16 eaxw, i16 edxw);
  void DM2_FILL_I16TABLE(i16* table, i16 value, ui16 entries);
  i32 DM2_ATIMESB_RSHIFTC(i16 eaxw, i8 edxb, i16 ebxw);
  void DM2_WAIT_SCREEN_REFRESH(void);
  void DM2_SLEEP_SEVERAL_TIME(i16 vsyncs);
  i32 DM2_4BYTES_TO_LONG(i8 argb0, i8 argb1, i8 argb2, i8 argb3);
  i16 DM2_ROTATE_5x5_POS(i16 wa, t_rotate rotate);

  i8 sgn8(i8 bn);

  i32 jul_shl32(i32, i8);

#endif
