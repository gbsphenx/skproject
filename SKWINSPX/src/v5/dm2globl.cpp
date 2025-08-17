#include <stdio.h>

#include <dm2data.h>
#include <startend.h>
#include <util.h>
#include <skgdtqdb.h>
// TODO: maybe move DM2_GET_GLOB_VAR to here
#include <dm2globl.h>

int sysrestart;
bool close_win = false;
bool main_exit = false;
i16 disable_video = 0;

FILE* logfile = NULL;

// TODO optimize temporaries
#include <regs.h>
#include <emu.h>

i32 DM2_UPDATE_GLOB_VAR(i32 eaxl, i32 edxl, i32 ebxl)
{
  c_nreg _RG1; RG1L = eaxl;
  c_nreg _RG2; RG2L = ebxl;
  c_nreg _RG3;
  c_nreg _RG4; RG4L = edxl;
  c_wlreg _RG5;

  RG5L = RG1L;
  RG3L = RG4L;
  RG1L = DM2_GET_GLOB_VAR(unsignedlong(RG5W));
  RG4L = RG1L;
  if (RG2UW <= mkuw(lcon(0x6)))
  {
    bool skip00008 = false;
    bool skip00007 = false;
    RG2L = unsignedlong(RG2W);
    switch (RG2L)
    {
      case 0:
        skip00008 = true;
        break;

      case 1:
        skip00007 = true;
        break;

      case 2:
        //m_5209:
        if (RG1W != 0)
          skip00007 = true;
        else
          skip00008 = true;
        break;

      case 3:
        //m_5215:
        RG1L += RG3L;
        break;

      case 4:
        //m_5219:
        RG1L -= RG3L;
      case 5:
        break;

      case 6:
        //m_521D:
        RG1L = RG3L;
        break;

      default: throw(THROW_DMABORT);
    }

    if (skip00008)
      //m_520E:
      RG1L = 1;

    if (skip00007)
      //m_5205:
      RG1L = 0;
  }

  //m_521F:
  if (RG5UW <= mkuw(lcon(0x3f)))
  {
    RG3L = RG5L;
    RG3Blo &= lcon(0x7);
    RG2L = 1 << RG3Blo;
    RG5UW >>= bcon(0x3);
    if (RG1W != 0)
    {
      RG1L = 1;
      RG4L = unsignedlong(RG5W);
      ddat.v1e0104[RG4L] |= RG2Blo;
      return RG1L;
    }
    RG4L = unsignedlong(RG5W);
    RG2Blo = ~RG2Blo;
    ddat.v1e0104[RG4L] &= RG2Blo;
    return RG1L;
  }
  if (RG5UW <= mkuw(lcon(0x7f)))
  {
    RG4L = signedlong(RG1W);
    RG1L = signedlong(DM2_BETWEEN_VALUE(0, 255, RG4W));
    ddat.globalb[unsignedlong(RG5W) - 0x40] = RG1Blo;
    return RG1L;
  }
  if (RG5UW <= mkuw(lcon(0xbf)))
  {
    RG4L = unsignedlong(RG5W);
    ddat.v1e000c[RG4L] = RG1W;
  }
  return RG1L;
}
