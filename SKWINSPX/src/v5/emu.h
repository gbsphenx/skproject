#ifndef EMU_H
  #define EMU_H

  #include <stdio.h>

  #ifndef TYPES_H
    #include <types.h>
  #endif

  #define SWITCHEND
  #define FOREND
  #define jump      goto

  // ------------------- emu --------------------

  // indirect access ########################################

  #define location(z) (z) // for readability only

  i8 byte_at(const i8* bptr, i32 offset = 0);
  ui8 ubyte_at(const i8* bptr, i32 offset = 0);
  i16 word_at(const i8* bptr, i32 offset = 0);
  ui16 uword_at(const i8* bptr, i32 offset = 0);
  i32 long_at(const i8* bptr, i32 offset = 0);
  i8* pointer_at(const i8* bptr);

  // operations ##############################################

  void put8(i8&, i8);
  void put16(i16&, i16);

  void inc8(i8*);
  void inc16(i8*);

  void dec8(i8*);
  void dec16(i8*);

  void shr16(i8*, i8);

  void sar8(i8*, i8);

  void add8(i8*, i8);
  void add16(i8*, i16);
  void add32(i8*, i32);

  void sub8(i8*, i8);
  void sub16(i8*, i16);

  void and8(i8*, i8);
  void and16(i8*, i16);

  void or8(i8*, i8);
  void or16(i8*, i16);
  void or32(i8*, i32);

  bool jz_test8(const i8*, i8);
  bool jz_test16(const i8*, i16);

  bool jnz_test8(const i8*, i8);
  bool jnz_test16(const i8*, i16);

  void mov8(i8*, i8);
  void mov16(i8*, i16);
  void mov32(i8*, i32);
  void movp(i8* bpm, i8* bpn);

  ui16 lrshift6e(ui16 n);
  
#endif // EMU_H
