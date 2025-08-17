#include <skrandom.h>

#include <dm2debug.h>

c_randomdata randdat;

#define RANDOM_MAGIC 0xbb40e62dL

void c_randomdata::init(void)
{
	SPX_DEBUG_PRINT("RANDOMDATA:INIT\n");
  random = 0;
}

// returns 24 bit random, msb is zero
i32 DM2_RAND(void)
{
  ui32 ulongrg1 = randdat.random * RANDOM_MAGIC + 11;
  randdat.random = ulongrg1;
  ulongrg1 >>= 8;
  return ulongrg1;
}

i16 DM2_RAND16(i16 a)
{
  i16 n = a;
  if (n != 0)
    n = CUTX16(DM2_RAND()) % unsignedlong(a); // BUGFIX unsigned long (windowbug)
  return n;
}

// was DM2_RAND01
bool DM2_RANDBIT(void)
{
  ui32 n = randdat.random * RANDOM_MAGIC + 11;
  randdat.random = n;
  n >>= 8;
  n &= 0x1;
  return n != 0;
}

// was DM2_RAND02
i8 DM2_RANDDIR(void)
{
  ui32 n = randdat.random * RANDOM_MAGIC + 11;
  randdat.random = n;
  n >>= 8;
  n &= 0x3;
  return CUTLX8(n);
}
