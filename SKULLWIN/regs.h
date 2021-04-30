#ifndef REGS_H
  #define REGS_H

  #ifndef TYPES_H
    #include "types.h"
  #endif

  #ifndef C_RECORD_H
    #include "c_record.h"
  #endif

  class c_rect;

  class c_nreg
  {
    public:
      union
      {
        struct
        {
          i8 b0;
          i8 b1;
          i8 b2;
          i8 b3;
        };
        struct
        {
          ui8 ub0;
          ui8 ub1;
          ui8 ub2;
          ui8 ub3;
        };
        struct
        {
          i16 w0;
          i16 w1;
        };
        struct
        {
          ui16 uw0;
          ui16 uw1;
        };
        i32 l;
        ui32 ul;
        i8* np8;
        i16* np16;
        i32* np32;
        t_text* tp;
        c_rect* rectp;
        t_record* recp;
      };

      c_nreg(void) { l = 0; }
      void ushiftr11(void) { uw0 >>= 11; }
  };

  class c_wlreg
  {
    public:
      union
      {
        struct
        {
          i16 w0;
          i16 w1;
        };
        struct
        {
          ui16 uw0;
          ui16 uw1;
        };
        i32 l;
        ui32 ul;
      };

      c_wlreg(void) { l = 0; }
      void ushiftr11(void) { uw0 >>= 11; }
      void ulrshiftr1(void) { ul <<= 10; ul >>= 11; }
  };

  class c_lreg
  {
    public:
      union
      {
        i32 l;
        ui32 ul;
      };

      c_lreg(void) { l = 0; }
      void ulrshiftr1(void) { ul <<= 10; ul >>= 11; }
  };

  #define RG1L _RG1.l
  #define RG1UL _RG1.ul
  #define RG1P _RG1.np8
  #define RG1P16 _RG1.np16
  #define RG1P32 _RG1.np32
  #define RG1T _RG1.tp
  #define RG1R _RG1.rectp
  #define RG1RP _RG1.recp
  #define RG1W _RG1.w0
  #define RG1UW _RG1.uw0
  #define RG1Blo _RG1.b0
  #define RG1Bhi _RG1.b1
  #define RG1UBlo _RG1.ub0
  #define RG1UBhi _RG1.ub1

  #define RG2L _RG2.l
  #define RG2UL _RG2.ul
  #define RG2P _RG2.np8
  #define RG2P16 _RG2.np16
  #define RG2P32 _RG2.np32
  #define RG2T _RG2.tp
  #define RG2R _RG2.rectp
  #define RG2RP _RG2.recp
  #define RG2W _RG2.w0
  #define RG2UW _RG2.uw0
  #define RG2Blo _RG2.b0
  #define RG2Bhi _RG2.b1
  #define RG2UBlo _RG2.ub0
  #define RG2UBhi _RG2.ub1

  #define RG3L _RG3.l
  #define RG3UL _RG3.ul
  #define RG3P _RG3.np8
  #define RG3P16 _RG3.np16
  #define RG3P32 _RG3.np32
  #define RG3T _RG3.tp
  #define RG3R _RG3.rectp
  #define RG3RP _RG3.recp
  #define RG3W _RG3.w0
  #define RG3UW _RG3.uw0
  #define RG3Blo _RG3.b0
  #define RG3Bhi _RG3.b1
  #define RG3UBlo _RG3.ub0
  #define RG3UBhi _RG3.ub1

  #define RG4L _RG4.l
  #define RG4UL _RG4.ul
  #define RG4P _RG4.np8
  #define RG4P16 _RG4.np16
  #define RG4P32 _RG4.np32
  #define RG4T _RG4.tp
  #define RG4R _RG4.rectp
  #define RG4RP _RG4.recp
  #define RG4W _RG4.w0
  #define RG4UW _RG4.uw0
  #define RG4Blo _RG4.b0
  #define RG4Bhi _RG4.b1
  #define RG4UBlo _RG4.ub0
  #define RG4UBhi _RG4.ub1

  #define RG5L _RG5.l
  #define RG5UL _RG5.ul
  #define RG5W _RG5.w0
  #define RG5UW _RG5.uw0
  #define RG5Blo _RG5.b0
  #define RG5UBlo _RG5.ub0

  #define RG51L _RG51.l
  #define RG51UL _RG51.ul
  #define RG51W _RG51.w0
  #define RG51UW _RG51.uw0
  #define RG51Blo _RG51.b0
  #define RG51UBlo _RG51.ub0

  #define RG52L _RG52.l
  #define RG52UL _RG52.ul
  #define RG52W _RG52.w0
  #define RG52UW _RG52.uw0
  #define RG52Blo _RG52.b0
  #define RG52UBlo _RG52.ub0

  #define RG53L _RG53.l
  #define RG53UL _RG53.ul
  #define RG53W _RG53.w0
  #define RG53UW _RG53.uw0
  #define RG53Blo _RG53.b0
  #define RG53UBlo _RG53.ub0

  #define RG54L _RG54.l
  #define RG54UL _RG54.ul
  #define RG54W _RG54.w0
  #define RG54UW _RG54.uw0
  #define RG54Blo _RG54.b0
  #define RG54UBlo _RG54.ub0

  #define RG55L _RG55.l
  #define RG55UL _RG55.ul
  #define RG55W _RG55.w0
  #define RG55UW _RG55.uw0
  #define RG55Blo _RG55.b0
  #define RG55UBlo _RG55.ub0

  #define RG56L _RG56.l
  #define RG56UL _RG56.ul
  #define RG56W _RG56.w0
  #define RG56UW _RG56.uw0
  #define RG56Blo _RG56.b0
  #define RG56UBlo _RG56.ub0

  #define RG57L _RG57.l
  #define RG57UL _RG57.ul
  #define RG57W _RG57.w0
  #define RG57UW _RG57.uw0
  #define RG57Blo _RG57.b0
  #define RG57UBlo _RG57.ub0

  #define RG58L _RG58.l
  #define RG58UL _RG58.ul
  #define RG58W _RG58.w0
  #define RG58UW _RG58.uw0
  #define RG58Blo _RG58.b0
  #define RG58UBlo _RG58.ub0

  #define RG59L _RG59.l
  #define RG59UL _RG59.ul
  #define RG59W _RG59.w0
  #define RG59UW _RG59.uw0
  #define RG59Blo _RG59.b0
  #define RG59UBlo _RG59.ub0

  #define RG6L _RG6.l
  #define RG6UL _RG6.ul
  #define RG6W _RG6.w0
  #define RG6UW _RG6.uw0
  #define RG6Blo _RG6.b0
  #define RG6UBlo _RG6.ub0

  #define RG61L _RG61.l
  #define RG61UL _RG61.ul
  #define RG61W _RG61.w0
  #define RG61UW _RG61.uw0
  #define RG61Blo _RG61.b0
  #define RG61UBlo _RG61.ub0

  #define RG62L _RG62.l
  #define RG62UL _RG62.ul
  #define RG62W _RG62.w0
  #define RG62UW _RG62.uw0
  #define RG62Blo _RG62.b0
  #define RG62UBlo _RG62.ub0

  #define RG63L _RG63.l
  #define RG63UL _RG63.ul
  #define RG63W _RG63.w0
  #define RG63UW _RG63.uw0
  #define RG63Blo _RG63.b0
  #define RG63UBlo _RG63.ub0

  #define RG64L _RG64.l
  #define RG64UL _RG64.ul
  #define RG64W _RG64.w0
  #define RG64UW _RG64.uw0
  #define RG64Blo _RG64.b0
  #define RG64UBlo _RG64.ub0

  #define RG65L _RG65.l
  #define RG65UL _RG65.ul
  #define RG65W _RG65.w0
  #define RG65UW _RG65.uw0
  #define RG65Blo _RG65.b0
  #define RG65UBlo _RG65.ub0

  #define RG66L _RG66.l
  #define RG66UL _RG66.ul
  #define RG66W _RG66.w0
  #define RG66UW _RG66.uw0
  #define RG66Blo _RG66.b0
  #define RG66UBlo _RG66.ub0

  #define RG67L _RG67.l
  #define RG67UL _RG67.ul
  #define RG67W _RG67.w0
  #define RG67UW _RG67.uw0
  #define RG67Blo _RG67.b0
  #define RG67UBlo _RG67.ub0

  #define RG68L _RG68.l
  #define RG68UL _RG68.ul
  #define RG68W _RG68.w0
  #define RG68UW _RG68.uw0
  #define RG68Blo _RG68.b0
  #define RG68UBlo _RG68.ub0

  #define RG7L _RG7.l
  #define RG7UL _RG7.ul
  #define RG7W _RG7.w0
  #define RG7UW _RG7.uw0
  #define RG7Blo _RG7.b0
  #define RG7UBlo _RG7.ub0

  #define RG71L _RG71.l
  #define RG71UL _RG71.ul
  #define RG71W _RG71.w0
  #define RG71UW _RG71.uw0
  #define RG71Blo _RG71.b0
  #define RG71UBlo _RG71.ub0

  #define RG72L _RG72.l
  #define RG72UL _RG72.ul
  #define RG72W _RG72.w0
  #define RG72UW _RG72.uw0
  #define RG72Blo _RG72.b0
  #define RG72UBlo _RG72.ub0

  #define RG73L _RG73.l
  #define RG73UL _RG73.ul
  #define RG73W _RG73.w0
  #define RG73UW _RG73.uw0
  #define RG73Blo _RG73.b0
  #define RG73UBlo _RG73.ub0

  #define RG74L _RG74.l
  #define RG74UL _RG74.ul
  #define RG74W _RG74.w0
  #define RG74UW _RG74.uw0
  #define RG74Blo _RG74.b0
  #define RG74UBlo _RG74.ub0

  #define RG76L _RG76.l
  #define RG76UL _RG76.ul
  #define RG76W _RG76.w0
  #define RG76UW _RG76.uw0
  #define RG76Blo _RG76.b0
  #define RG76UBlo _RG76.ub0

#endif
