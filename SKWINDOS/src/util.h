#ifndef UTIL_H
  #define UTIL_H

  #include <string.h>	// SPX: added for memset

  #ifndef TYPES_H
    #include "..\types.h"
  #endif

  #define SKW_SK_STRLEN(a) I16CAST(strlen(a))
  #define SKW_SK_STRCAT(a, b) strcat(a, b)
  #define SKW_SK_STRCPY(a, b) strcpy(a, b)
  #define SKW_SK_STRSTR(a, b) strstr(a, b)

  #define DM2_ZERO_MEMORY(p, n) memset(p, 0, n) // id: 0x1007
  xxx* SKW_COPY_MEMORY(xxx* eaxp, x32 ebxl, xxx* edxp);

  x32 SKW_ABS16(x16 eaxw);

  x16 DM2_ABS(x16 n);
  x16 DM2_MIN(x16 a, x16 b);
  x16 DM2_MAX(x16 a, x16 b);

  x16 DM2_BETWEEN_VALUE(x16 min, x16 max, x16 val);

  x32 SKW_RAND(void);
  x16 SKW_RAND16(x16 eaxw);
  x16 SKW_RAND01(void);
  x16 SKW_RAND02(void);
  x16 SKW_CALC_SQUARE_DISTANCE(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
  x16 SKW_CALC_VECTOR_DIR(x16 eaxw, x16 ebxw, x16 ecxw, x16 edxw);
  t_text* SKW_SK_LTOA10(x32 eaxl, t_text* edxt);
  ui16 SKW_FILL_STR(x8* eaxb, x8 ebxb, x16 ecxw, x16 edxw);
  void DM2_FILL_I16TABLE(x16* table, x16 value, ui16 entries);
  x16 SKW_0cd5_0176(x16 eaxw, x16 ebxw, x8 edxb);
  x32 SKW_CALC_IMAGE_BYTE_LENGTH(t_gfxdata* eaxpg);
  x16 SKW_CALC_STRETCHED_SIZE(x16 eaxw, x16 edxw);
  t_text* SKW_FMT_NUM(x16 eaxw, x16 ebxw, x32 edxl);
  void SKW_WAIT_SCREEN_REFRESH(void);
  void SKW_SLEEP_SEVERAL_TIME(x16 eaxw);
  x32 SKW_4BYTES_TO_LONG(x8 b0, x8 b1, x8 b2, x8 b3);

  t_gfxdata xlat8(const t_gfxdata* table, t_gfxdata idx);

  i32 jul_shl32(i32, i8);
  ui16 ulrshift(x16 n, x16 l, x16 r);
  ui16 urshift(x16 n, x16 r);
  ui8 ubrshift(x8 n, x16 r);
  ui32 ulrlongshift(x32 n, x16 l, x16 r);

#endif
