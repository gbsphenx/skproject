#include "util.h"
#include "driver.h"
#include "addon.h"
//#include "emu.h"

bool iflag = false;

void cli(void) { iflag = false; }
void sti(void) { iflag = true; }

void SKW_443c_00a9(xxx* eaxp, x32 ebxl, x32 ecxl, x32 edxl, x16 argw0)
{
  static bool semaphore = false; // used by both threads
  nreg _RG1;
  nreg _RG2; RG2L = ebxl;
  nreg _RG3; RG3L = ecxl;
  nreg _RG4; RG4L = edxl;

            while (semaphore);
            semaphore = true;
/*@0044B1*/ movp(VARL(Vx1e00a0), eaxp);
/*@0044B6*/ mov16(VARW(idx(0x1e00aa)), RG4W);
/*@0044BD*/ mov16(VARW(idx(0x1e00ba)), RG4W);
/*@0044C4*/ mov16(VARW(idx(0x1e00a8)), RG2W);
/*@0044CE*/ RG1L = signedlong(RG4W);
/*@0044CB*/ RG2L = signedlong(RG2W) - RG1L + 1;
/*@0044D4*/ mov16(VARW(idx(0x1e00be)), RG2W);
/*@0044DB*/ mov16(VARW(idx(0x1e00ae)), RG3W);
/*@0044E2*/ mov16(VARW(idx(0x1e00bc)), RG3W);
/*@0044ED*/ mov16(VARW(idx(0x1e00ac)), argw0);
/*@0044FF*/ RG1L = signedlong(argw0) - signedlong(RG3W) + con(0x1);
/*@004502*/ mov16(VARW(idx(0x1e00c0)), RG1W);
/*@004518*/ SKW_COPY_MEMORY(adress_of(idx(0x1e00ba)), con(0x8), pointer_at(Vx1e1488));
/*@004522*/ mov16(location(pointer_at(Vx1e1488) + con(0x8)), con(0x20));
/*@00452F*/ userinterrupt(con(0xc), con(0x0));
            semaphore = false;
}

x32 SKW_FIRE_QUEUE_MOUSE_EVENT(x32 eaxl, x32 ebxl, x32 edxl)
{
  static bool semaphore = false; // used by both threads
  nreg _RG1; RG1L = eaxl;
  nreg _RG2; RG2L = ebxl;
  nreg _RG3;
  nreg _RG4; RG4L = edxl;
  nreg _RG5;
  nreg _RG6;
  nreg _RG7;

            while (semaphore);
            semaphore = true;
/*@010D12*/ RG3L = RG1L;
/*@010D1C*/ if (word_at(idx(0x1d3feb)) == con(0x0)) goto M_10D41;
/*@010D1E*/ mov32(VARL(idx(0x1e047c)), con(0x1));
/*@010D28*/ mov16(VARW(idx(0x1e0514)), RG1W);
/*@010D2E*/ mov16(VARW(idx(0x1e0516)), RG4W);
/*@010D35*/ mov16(VARW(idx(0x1e0518)), RG2W);
            semaphore = false;
            return RG1L;

M_10D41:
/*@010D41*/ mov16(VARW(idx(0x1d3feb)), con(0x1));
/*@010D4A*/ RG1L = signedlong(RG2W);
/*@010D50*/ if (RG1L != con(0x4)) goto M_10D5B;
/*@010D59*/ if (long_at(idx(0x1d3ff3)) == con(0x0)) goto M_10D68;
M_10D5B:
/*@010D5B*/ RG1L = signedlong(RG2W);
/*@010D61*/ if (RG1L == con(0x40)) goto M_10D68;
/*@010D66*/ if (RG1L != con(0x60)) goto M_10D6F;
M_10D68:
/*@010D68*/ RG5L = con(0x9);
/*@010D6D*/ goto M_10D74;

M_10D6F:
/*@010D6F*/ RG5L = con(0x7);
M_10D74:
/*@010D76*/ mov32(VARL(idx(0x1d3ff3)), con(0x0));
/*@010D7C*/ RG1W = word_at(idx(0x1d3fe9));
/*@010D82*/ RG1L++;
/*@010D83*/ RG6L = signedlong(RG1W);
/*@010D89*/ if (RG6L <= con(0xa)) goto M_10D8E;
/*@010D8B*/ RG1L -= con(0xb);
M_10D8E:
/*@010D8E*/ RG6W = word_at(idx(0x1e050c));
/*@010D98*/ if (RG5W > RG6W) goto M_10DAE;
/*@010D9A*/ RG1L = signedlong(RG2W);
/*@010DA0*/ if (RG1L != con(0x2)) goto M_10DDF;
/*@010DA2*/ mov32(VARL(idx(0x1d3ff3)), con(0x1));
/*@010DAC*/ goto M_10DDF;

M_10DAE:
/*@010DAE*/ mov16(VARW(idx(0x1d3fe9)), RG1W);
/*@010DB4*/ RG5L = signedlong(RG1W);
/*@010DB7*/ RG1L = 3 * RG5L;
/*@010DBE*/ mov16(location(adress_of(idx(0x1e0490)) + 2 * RG1L), RG2W);
/*@010DC6*/ mov16(location(adress_of(idx(0x1e0492)) + 2 * RG1L), RG3W);
/*@010DCE*/ mov16(location(adress_of(idx(0x1e0494)) + 2 * RG1L), RG4W);
/*@010DD6*/ RG1L = RG6L + 1;
/*@010DD9*/ mov16(VARW(idx(0x1e050c)), RG1W);
M_10DDF:
/*@010DDF*/ RG4L = con(0x0);
/*@010DE1*/ mov16(VARW(idx(0x1d3feb)), RG4W);
            semaphore = false;
            return RG1L;
}

// belongs to queue_int2e
static x32 SKW_443c_00f8(x32 eaxl, x32 edxl)
{
  nreg _RG1; RG1L = eaxl;
  nreg _RG2;
  nreg _RG3;
  nreg _RG4; RG4L = edxl;
  nreg _RG5;
  nreg _RG6;
  nreg _RG7;
  x32 vl_00;
  x32 vl_04;
  x32 vl_08;
  x32 vl_0c;
  x32 vl_10;
  x32 vl_14;
  x32 vl_18;
  x16 parw00;

            vl_04 = RG1L;
            vl_00 = RG4L;
/*@004545*/ if (long_at(idx(0x1e00a4)) != con(0x0)) goto M_4572;
/*@00454E*/ if (RG1W < word_at(idx(0x1e00aa))) goto M_4572;
/*@004557*/ if (RG1W > word_at(idx(0x1e00a8))) goto M_4572;
/*@004559*/ RG1L = vl_00;
/*@004563*/ if (RG1W < word_at(idx(0x1e00ae))) goto M_4572;
/*@00456C*/ if (RG1W <= word_at(idx(0x1e00ac))) goto M_4754;
M_4572:
/*@004574*/ mov32(VARL(idx(0x1e00a4)), con(0x0));
/*@00457A*/ RG6L = con(0x0);
/*@00457C*/ mov16(VARW(idx(0x1d264e)), RG6W);
/*@004583*/ RG5P = pointer_at(Vx1e009c);
/*@004589*/ RG1L = con(0x0);
/*@00458B*/ mov16(loword(vl_14), RG1W);
/*@004590*/ RG3W = word_at(idx(0x1d2704));
/*@00459D*/ mov16(loword(vl_18), word_at(idx(0x1d2706)));
M_45A2:
/*@0045A4*/ if (RG5P == NULL) goto M_470B;
/*@0045AA*/ RG2P = pointer_at(RG5P + con(0x6));
/*@0045AD*/ RG2P += con(0x4);
/*@0045B0*/ RG7L = con(0x0);
/*@0045B2*/ RG1L = vl_04;
/*@0045B6*/ RG4W = word_at(RG2P);
/*@0045BC*/ if (RG1W >= RG4W) goto M_45C3;
/*@0045BE*/ RG4L = signedlong(RG4W);
/*@0045C1*/ goto M_4623;

M_45C3:
/*@0045C9*/ RG4L = signedlong(RG4W);
/*@0045C6*/ RG1L = signedlong(word_at(RG2P, con(0x4))) + RG4L;
/*@0045CE*/ vl_0c = RG1L;
/*@0045D9*/ vl_08 = signedlong(I16CAST(vl_04));
/*@0045E2*/ vl_0c--;
/*@0045E6*/ RG1L = vl_08;
/*@0045EE*/ if (RG1L <= vl_0c) goto M_4607;
/*@0045F0*/ RG1W = word_at(RG2P) + word_at(RG2P, con(0x4));
/*@0045F7*/ RG1L--;
/*@0045F8*/ RG4L = signedlong(RG1W);
/*@0045FE*/ RG1L = SKW_MAX16(signedlong(RG6W), RG4L);
/*@004603*/ RG6L = RG1L;
/*@004605*/ goto M_462D;

M_4607:
/*@004607*/ RG7L = con(0x1);
/*@00460C*/ RG6L = signedlong(RG6W);
/*@004611*/ RG1L = SKW_MAX16(RG6L, RG4L);
/*@004616*/ RG6L = RG1L;
/*@004618*/ RG1W = word_at(RG2P) + word_at(RG2P, con(0x4));
/*@00461F*/ RG1L--;
/*@004620*/ RG4L = signedlong(RG1W);
M_4623:
/*@004626*/ RG1L = SKW_MIN16(signedlong(RG3W), RG4L);
/*@00462B*/ RG3L = RG1L;
M_462D:
/*@00462D*/ RG1L = vl_00;
/*@004634*/ if (RG1W >= word_at(RG2P, con(0x2))) goto M_4651;
/*@004638*/ RG4L = signedlong(word_at(RG2P, con(0x2)));
/*@004642*/ RG1L = SKW_MIN16(signedlong(I16CAST(vl_18)), RG4L); // truncated
/*@004647*/ mov16(loword(vl_18), RG1W);
/*@00464C*/ goto M_4701;

M_4651:
/*@004653*/ RG4L = signedlong(word_at(RG2P, con(0x2)));
/*@00465E*/ vl_0c = signedlong(word_at(RG2P, con(0x6))) + RG4L;
/*@004669*/ vl_10 = signedlong(I16CAST(vl_00));
/*@004672*/ vl_0c--;
/*@004676*/ RG1L = vl_10;
/*@00467E*/ if (RG1L <= vl_0c) goto M_469F;
/*@004680*/ RG1W = word_at(RG2P, con(0x2)) + word_at(RG2P, con(0x6));
/*@004688*/ RG1L--;
/*@004689*/ RG4L = signedlong(RG1W);
/*@004693*/ RG1L = SKW_MAX16(signedlong(I16CAST(vl_14)), RG4L); // truncated
/*@004698*/ mov16(loword(vl_14), RG1W);
/*@00469D*/ goto M_4701;

M_469F:
/*@0046A6*/ RG1L = SKW_MAX16(signedlong(I16CAST(vl_14)), RG4L);
/*@0046AB*/ mov16(loword(vl_14), RG1W);
/*@0046B0*/ RG1W = word_at(RG2P, con(0x2)) + word_at(RG2P, con(0x6));
/*@0046B8*/ RG1L--;
/*@0046B9*/ RG4L = signedlong(RG1W);
/*@0046C3*/ RG1L = SKW_MIN16(signedlong(I16CAST(vl_18)), RG4L); // truncated
/*@0046C8*/ mov16(loword(vl_18), RG1W);
/*@0046CF*/ if (RG7L == con(0x0)) goto M_4701;
/*@0046D1*/ RG1L = unsignedlong(byte_at(RG5P, con(0x2)));
/*@0046D6*/ mov16(VARW(idx(0x1d264e)), RG1W);
/*@0046E2*/ if (RG5P == pointer_at(Vx1e00a0)) goto M_470B;
/*@0046E4*/ RG1Bhi = byte_at(RG5P, con(0x4));
/*@0046E9*/ if (RG1Bhi == con(0x0)) goto M_470B;
/*@0046EB*/ RG2L = unsignedlong(RG1Bhi);
/*@0046FA*/ SKW_FIRE_QUEUE_MOUSE_EVENT(signedlong(I16CAST(vl_04)), RG2L, vl_10);
/*@0046FF*/ goto M_470B;

M_4701:
/*@004704*/ RG5P = pointer_at(pointer_at(RG5P + con(0x6)));
/*@004706*/ goto M_45A2;

M_470B:
/*@00470B*/ RG1P = pointer_at(Vx1e00a0);
/*@004712*/ if (RG1P == NULL || RG5P == RG1P) goto M_4736;
/*@004718*/ RG4Blo = byte_at(RG1P, con(0x5));
/*@00471D*/ if (RG4Blo == con(0x0)) goto M_4736;
/*@00471F*/ RG2L = unsignedlong(RG4Blo);
/*@004731*/ SKW_FIRE_QUEUE_MOUSE_EVENT(signedlong(I16CAST(vl_04)), RG2L, signedlong(I16CAST(vl_00)));
M_4736:
/*@00473A*/ RG1L = signedlong(I16CAST(vl_18)); // truncated
/*@00473D*/ put16(parw00, RG1W);
/*@004742*/ RG1L = signedlong(I16CAST(vl_14)); // truncated
/*@004745*/ RG2L = signedlong(RG3W);
/*@004748*/ RG4L = signedlong(RG6W);
/*@00474F*/ SKW_443c_00a9(RG5P, RG2L, RG1L, RG4L, parw00);
M_4754:
/*@004759*/ RG1L = signedlong(word_at(idx(0x1d264e)));
/*@00475F*/ if (RG1L != con(0x2)) goto M_4768;
/*@004761*/ return con(0x3);

M_4768:
/*@00476B*/ if (RG1L != con(0x1) || word_at(idx(0x1d69a8)) < con(0x0)) goto M_478E;
/*@00477E*/ if (long_at(idx(0x1e0098)) == con(0x0)) goto M_4787;
/*@004780*/ return con(0x2);

M_4787:
/*@004787*/ return con(0x1);

M_478E:
/*@00478E*/ return con(0x0);
}


// was an interruptroutine, ended with iret
// old name: R_D73
void queue_int2d(void)
{
  static bool semaphore = false; // used by both threads

  while (semaphore);
  semaphore = true;

  add32(VARL(idx(0x1e022c)), signedlong(word_at(idx(0x1dffb0))));

  i16 val = word_at(idx(0x1e025a)) + word_at(idx(0x1dffb0));
  mov16(VARW(idx(0x1e025a)), val);
  if (val >= word_at(idx(0x1e026c)))
    mov16(VARW(idx(0x1d27f6)), con(0x1));

  semaphore = false;
}

// was interruptroutine R_4799
x16 queue_int2e(void)
{
  static bool semaphore = false; // used by both threads
  nreg _RG4;

  while (semaphore);
  semaphore = true;

  if (signedlong(word_at(pointer_at(Vx1e148c), con(0x4))) >= con(0x20))
    RG4L = SKW_443c_00f8(signedlong(word_at(pointer_at(Vx1e148c))), signedlong(word_at(pointer_at(Vx1e148c), con(0x2))));
  else
  {
    SKW_FIRE_QUEUE_MOUSE_EVENT(signedlong(word_at(pointer_at(Vx1e148c))), signedlong(word_at(pointer_at(Vx1e148c), con(0x4))), signedlong(word_at(pointer_at(Vx1e148c), con(0x2))));
    RG4L = con(0xffffffff);
  }
  mov16(location(pointer_at(Vx1e148c)), RG4W);
  semaphore = false;
  return RG4L;
}
