#ifndef DM2BT_H
  #define DM2BT_H

  #define OO  // OutOnly - just a marker, routine returns this argument
  // I better use &-references instead of *

  #define NULLBYTE '\0'

  // #################
  // compiler specific

  //#define for if (0) ; else for   // VC60 - workaround

  //#pragma warning (disable:4101) // unreferenced local

#if 0
  // avoid truncated warnings:
  #pragma warning (disable:4244)
  #pragma warning (disable:4309)
  #pragma warning (disable:4311)
  #pragma warning (disable:4312) // type cast
#endif
  #pragma warning (disable:4996) // function may be unsafe - strcpy etc.

  // #########################

  // char
  typedef signed _int8 i8;
  typedef unsigned _int8 ui8;
  // (short) int
  typedef signed _int16 i16;
  typedef unsigned _int16 ui16;
  // long (int)
  typedef signed _int32 i32;
  typedef unsigned _int32 ui32;
  // long long
  typedef signed _int64 i64;
  typedef unsigned _int64 ui64;

  // TODO:
  // those three are unknown in sign
  // (according to kentaro's X8, but those ones work signed pretty well)
  typedef i8 x8;
  typedef i16 x16;
  typedef i32 x32;

  union u_bbw
  {
    x16 w;
    struct
    {
      x8 b0;
      x8 b1;
    } b;
  };

#endif
