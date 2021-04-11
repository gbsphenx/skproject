#ifndef C_SFX_H
 #define C_SFX_H

 #ifndef TYPES_H
   #include "../types.h"
 #endif

 #define MAXSAMPLES   (16)

 class c_sfx
 {
   protected:
     int sampleindex;

   protected:
     void stop_sample(int n);

   public:
     c_sfx(void) : sampleindex(-1) {}

     bool init_sfx(void);
     void stop_sfx(void);
     void do_sound(ui8 vol, i8* adr);
 };

 void R_7E7(x16 eaxw, x16 edxw);
 void SKW_PLAY_SOUND(ui16 eaxw, s_sfx* edxp);
 void SKW_QUEUE_NOISE_GEN2(x8 eaxb, x8 ebxb, x8 ecxb, x8 edxb, x16 argw0, x16 argw1, x16 argw2, x16 argw3, x16 argw4);
 void SKW_QUEUE_NOISE_GEN1(x8 eaxb, x8 ebxb, x16 ecxw, x8 edxb, x16 argw0, x16 argw1, x16 argw2, x16 argw3);
 void SKW_482b_0624(x8 eaxb, x8 ebxb, x8 edxb);

#endif
