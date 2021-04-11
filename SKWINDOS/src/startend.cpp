#include "startend.h"
#include "c_allegro.h"
#include "../dm2global.h"
#include "dm2data.h"

// needs:
void SKW_MAIN(void);

ui8 table_1dfac4[0x14] =
{
  0x00, 0x09, 0x01, 0x01, 0x0b, 0x06, 0x04, 0x05,
  0x05, 0x05, 0x02, 0x03, 0xff, 0xff, 0x07, 0x08,
  0x06, 0x08, 0x01, 0xff
};

// id: 0x58B3F
// errcode comes as two bytes in one word
// called only by fileio-functions
// converts an error-code and writes it via SKW_MARK_ERROR
x16 DM2_WATCOM_ERRHANDLING(x16 nw)
{
  x32 code = unsignedlong(nw);
  ui8 b4 = CUTLX8(code);
  ui8 b2 = CUTLX8(code);
  ddata.longerrhandling = unsignedlong(b4);
  if (nw >= con(0x100))
    code = unsignedlong(CUTLX8(nw >> con(0x8)));
  else
  {
    if (ddata.errversion >= con(0x3)) // ever true!
    {
      if (b4 != con(0x50))
      {
        if (b4 >= con(0x22))
          b2 = con(0x13);
        else if (b4 >= con(0x20))
          b2 = con(0x5);
      }
      else
        b2 = con(0xe);
    }
    if (b2 > con(0x13))
      b2 = con(0x13); // limits b2
    code = unsignedlong(table_1dfac4[b2]);
  }
  DM2_WATCOM_MARK_ERROR(code);
  return con(0xffffffff);
}

// id: 0x58BCB
// writes an errorcode into a memory - location
// (which was returned formerly by WATCOM_590A2)
// was WATCOM_58BC8
void DM2_WATCOM_MARK_ERROR(x32 nl)
{
  ddata.longmarkerror = nl;
}

// function doesn't do anything anymore.
#if 0
static void SKW_1031_096a(void)
{
  for (i16 i = 0; i < con(0x12); i++)
  {
    xxx* ptr = &table1d32d8[i];
    R_4BFF(); // now an empty function
  }
}
#endif

// a lot deleted herein
// TODO: condition is surely wrong, but works (for now)!
void SKW_SK_PREPARE_EXIT(void)
{
  // SKW_1031_096a();  does nothing anymore
  restart = (ddata.mouse_visibility > con(0x0)) ? RESTART_RESUME : RESTART_QUIT;
  alg.stop_timer();
  throw(THROW_PREPARE_EXIT);
}

// id: 0x58A37
// used only on toplevels
// no value - return necessary, because exit follows everytime
// calls closing functions stored in exittable, which isn't
// available anymore
#if 0
void WATCOM_PRE_EXIT(ui8 a, ui8 b)
{
  for(;;)
  {
    s_exittableentry* ptr5 = adress_of(EXITTABLE1);
    s_exittableentry* ptr6 = adress_of(EXITTABLE2);
    s_exittableentry* ptr2 = ptr6;
    ui8 b1 = a;
    while (ptr5 < ptr6)
    {
      if ((ptr5->v1 != con(0x2)) && (b1 <= ptr5->v2))
      {
        ptr2 = ptr5;
        b1 = ptr5->v2;
      }
      ptr5++;
    }
    if (ptr2 == ptr6)
      return;
    if ((b1 <= b) && (ptr2->func != con(0x0)))
    {
      // RG1L = call32(ptr2->func);
    }
    ptr2->v1 = con(0x2);
  }
}
#endif

// 0x21/0x4c
void SYS_exit(i8 alb)
{
  // DOS - 2+ - QUIT WITH EXIT CODE (EXIT)
  // alb = exit code
  throw(THROW_SYS_EXIT);
  return; // TODO noreturn
}

// id: 0x5260C
// top - level - ending routine
// had one parameter - makes problems, works without
// id: 0x52624
// top - level - ending routine - never returns
// routine gets no parameter anymore (had one - but there are problems then)
static void SKW_FIRE_EXIT(void)
{
//   call32(VARL(SHUTDOWNVEC2));
//   call32(VARL(SHUTDOWNVEC3));
  // WATCOM_PRE_EXIT(con(0x0), con(0xff));
  SYS_exit(2); // parameter set by me
}

static void SKW_FIRE_SHUTDOWN(void)
{
//   call32(VARL(SHUTDOWNVEC1));
//   call32(VARL(SHUTDOWNVEC2));
  SKW_FIRE_EXIT();
}

// SKW_MAIN was called with the arguments
// pointer_at(MAIN_ARGC) and pointer_at(MAIN_ARGV)
// not needed anymore
static void SKW_FIRE_BOOTSTRAP(void)
{
  SKW_MAIN();
  SKW_FIRE_SHUTDOWN();
}

// id: 0x52334
void WATCOM_STARTUP(void)
{
  SKW_FIRE_BOOTSTRAP();
  // WATCOM_PRE_EXIT(con(0x0), con(0xff));
  SYS_exit(0);
}
