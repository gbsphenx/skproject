#include "c_350.h"

void c_4b3wp::init(void)
{
  b_00 = 0;
  b_01 = 0;
  b_02 = 0;
  b_03 = 0;
  w_04 = 0;
  w_06 = 0;
  w_08 = 0;
  xp_0a = NULL;
}

void c_350::init(void)
{
  v1e054c = 0;
  v1e054e = NULL;
  v1e0552 = NULL;
  creatures = NULL;
  v1e055a = NULL;
  v1e055e = NULL;
  v1e0562.init();
  v1e056e = 0;
  v1e056f = 0;
  v1e0570 = 0;
  v1e0571 = 0;
  v1e0572 = 0;
  v1e0574 = 0;
  v1e0576 = 0;
  v1e0578 = 0;
  v1e057a = 0;
  v1e057c = 0;
  v1e057e = 0;
  v1e0580 = 0;
  v1e0582 = 0;
  v1e0584 = 0;
  v1e0586 = 0;
  v1e0588 = NULL;
  v1e058c = 0;
  v1e058d = 0;
  for (i16 i = 0; i < 0x80; i++) v1e058e[i] = 0;
  for (i16 i = 0; i < 8; i++) v1e060e[i].init();
  for (i16 i = 0; i < 5; i++) v1e066e[i] = 0;
  v1e0673 = 0;
  v1e0674 = 0;
  v1e0675 = 0;
  for (i16 i = 0; i < 2; i++) v1e0676[i] = 0;
  for (i16 i = 0; i < 0x10; i++) v1e0678[i].init();
  v1e07d8.init();
  v1e07e6 = NULL;
  v1e07ea = 0;
  v1e07eb = 0;
  v1e07ec = 0;
  v1e07ed = 0;
  for (i16 i = 0; i < 0x2a; i++) v1e07ee[i] = NULL;
  v1e0896 = 0;
  v1e0898 = NULL;
}

c_350 s350;
