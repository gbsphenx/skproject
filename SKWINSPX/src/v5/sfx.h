#ifndef C_SFX_H
  #define C_SFX_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  #define MAXSAMPLES   (16)

  class c_sfx
  {
    protected:
      int sampleindex;

    protected:
      void stop_sample(int i);

    public:
      c_sfx(void) : sampleindex(-1) {}

      bool init_sfx(void);
      void stop_sfx(void);
      void do_sound(ui8 vol, i8* adr);
  };

  void DM2_QUEUE_NOISE_GEN1(i8 eaxb, i8 edxb, i8 ebxb, i16 ecxw, i16 argw0, i16 argw1, i16 argw2, i16 argw3);
  void DM2_QUEUE_NOISE_GEN2(i8 eaxb, i8 edxb, i8 ebxb, i8 ecxb, i16 argw0, i16 argw1, i16 argw2, i16 argw3, i16 argw4);

#endif
