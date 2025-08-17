#ifndef C_RANDOM_H
  #define C_RANDOM_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  class c_randomdata
  {
    public:
      ui32 random;

      void init(void);
  };

  extern c_randomdata randdat;

  i32 DM2_RAND(void);
  i16 DM2_RAND16(i16 a);
  bool DM2_RANDBIT(void);
  i8 DM2_RANDDIR(void); // also called indirect

#endif
