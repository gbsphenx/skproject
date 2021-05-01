#include "startend.h"
#include "fileio.h"
#include "c_image.h"
#include "dm2data.h"

c_dm2data ddat;

te_text v1d1044[] = "V1.0";
/*const*/ te_text v1d104c[] = "SYSTEM ERROR .Z000";
// next one not used?
const te_text v1d105f[] = "";
t_text v1d5c31[9] = "0 UNUSED";

/*const*/ te_text v1d10f9[] = ".Z008DATA\\";

// the following datas seem to be unused (except one!):
/*const*/ te_text v1d1104[] = ".Z009DATA\\";
/*const*/ te_text v1d110f[] = ".Z009";
te_text v1d1118[] = "\nSK: V";
const te_text v1d111f[] = "--- ";
te_text v1d1124[] = "/";
// 26 and 27 are 0x0
te_text v1d1128[] = "mv";
te_text v1d112b[] = "xl";
te_text v1d112e[] = "yl";
te_text v1d1131[] = "fd";
te_text v1d1134[] = "cd";
te_text v1d1137[] = "fw";
// at v1d113a three nullbytes follow
te_text v1d113d[] = " "; // needed!!!
// at v1d113f is a nullbyte
te_text v1d1140[] = "cd";
te_text v1d1143[] = "fw";
// at v1d1146 two 0x00 follow
// at v1d1148 a lot of HMI-strings follow
// at v1d1377 16 times the byte sequence 0x00,0xff,0x00,0x00
// at v1d13b7 80 nullbytes
// at v1d1407 it continues with possibly needed stuff

t_text v1d152c[4] = { 0x43, 0x3a, 0x00, 0x00 };
t_text v1d1530[4] = { 0x43, 0x3a, 0x00, 0x00 };
t_text v1d1534[3] = { 0x43, 0x3a, 0x00 };
t_text v1d1537[3] = { 0x43, 0x3a, 0x00 };

const te_text dfn[] = ".Z009DATA\\";
te_text v1d10e4[] = "GAME  ";
te_text v1d10eb[] = "V1.0";
te_text v1d10f0[] = { 0xc, 0x0 };
te_text v1d10f2[] = "/";
te_text v1d10f4[] = "";
te_text v1d10f5[] = "F";
te_text v1d10f7[] = "G";

t_text v1d6a4f[0x5] = { '_', 0x0, ' ', 0x0, 0x0 };

/*const*/ t_text* table1d27e8[3] = { v1d104c, v1d104c, v1d104c };
te_text* table1d674e[5];
te_text* _table1d674e[5] = { v1d10f4, v1d10f5, v1d10f7, v1d10f9, v1d10f9 };

const i8 table1d6702[16] =
{
  0x00, 0x05, 0x0c, 0x18, 0x21, 0x28, 0x2e, 0x33,
  0x3b, 0x44, 0x4c, 0x52, 0x59, 0x5e, 0x61, 0x64
};

const i8 table1d6712[21] =
{
  0x63, 0x4b, 0x32, 0x19, 0x01, 0x00, 0x04, 0x05,
  0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
  0x0e, 0x0f, 0x10, 0x11, 0x12
};

const i16 table1d672b[9] =
{
  0x0000, 0x0006, 0x000e, 0x001e,
  0x002a, 0x0036, 0x004c, 0x0058,
  0x0060
};

const i16 table1d673d[7] =
{
  0x0000, 0x000a, 0x0016, 0x002d, 0x0046, 0x005a, 0x0000
};

const i8 table1d281c[16] =
{
  0x00, 0x00, 0x00, 0x00, 0x4b, 0x64, 0x3c, 0x00,
  0x0c, 0x05, 0xc8, 0x00, 0x00, 0x00, 0x3c, 0x32
};

/*const*/ i8 table1d282c[16] =
{
  0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

i8 vsgame[120] =
{
 0xff, 0xff, 0xff, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x3e,
 0x00, 0x00, 0x01, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x07,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x3f, 0xff, 0x07, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x07,
 0x00, 0x00, 0xff, 0x3d,
 0x00, 0x00, 0xff, 0x1f,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x04,
 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff,
 0x00, 0x00, 0xff, 0xc0,
 0x00, 0x00, 0x7f, 0xff,
 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x03,
 0x00, 0x00, 0xff, 0xff,
 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x01, 0x00
};

i8* table1d64db[16] =
{
vsgame + 0x0c,
NULL,
vsgame + 0x10,
vsgame + 0x14,
vsgame + 0x1c,
vsgame + 0x3c,
vsgame + 0x40,
vsgame + 0x44,
vsgame + 0x48,
vsgame + 0x4c,
vsgame + 0x5c,
NULL,
NULL,
NULL,
vsgame + 0x64,
vsgame + 0x6c
};

/*const*/ i8 table1d631a[60] =
{
  0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x07, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x03, 0x00,
  0x3f, 0x00, 0x03, 0x00, 0xff, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x07, 0x00,
  0x07, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03,
  0xff, 0xff, 0xff, 0x03, 0xff, 0x00, 0x1f, 0x03, 0xff, 0xff, 0xff, 0x00
};

i8 table1d6356[263] =
{
  0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
  0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x03, 0x03, 0x07, 0x00,
  0x3f, 0x3f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x12, 0x00, 0x3f, 0x00, 0xff, 0x03, 0xff, 0x03, 0xff, 0x3f, 0xff, 0x3f, 0xff, 0x03,
  0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0,
  0xff, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x1f, 0xe0,
  0xff, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x1f, 0xe0,
  0xff, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x1f, 0xe0,
  0xff, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x1f, 0xe0,
  0xff, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x1f, 0xe0, 0xff, 0xff, 0x1f, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x3f, 0x0f, 0x00, 0x00, 0x00, 0x00
};

i8 table1d645d[6] =
{
  0x00, 0x00, 0x00, 0xff, 0xff, 0x00
};

i16 table1d27fc[4] =
{
  0x0000, 0x0001, 0x0000, 0xffff
};

i16 table1d2804[4] =
{
  0xffff, 0x0000, 0x0001, 0x0000
};

i8 table1d70f0[24] =
{
  0x05, 0x05, 0x04, 0x03,
  0x02, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x02,
  0x03, 0x04, 0x05, 0x05
};

#if 0 // table is obsolete - adressed datas are all 0!
ui32 table1d83ec[32] = // segment-offsetpointer, segment always 0x1d8
{
0x0398, 0x0404, 0x0470, 0x04dc, 0x0548, 0x05b4, 0x0620, 0x068c,
0x06f8, 0x0764, 0x07d0, 0x083c, 0x08a8, 0x0914, 0x0980, 0x09ec,
0x0a58, 0x0ac4, 0x0b30, 0x0b9c, 0x0c08, 0x0c74, 0x0ce0, 0x0d4c,
0x0db8, 0x0e24, 0x0e90, 0x0efc, 0x0f68, 0x0fd4, 0x1040, 0x10ac
}; // each entry has a length of 0x6c, but only datas of offsets
// 0x30...0x34 within such a struct are really adressed
// - and those are all 0!
#endif

i8 table1410ec[64] = // real size? number of levels!
{
 0x02, 0x11, 0x0e, 0x1b, 0x04, 0x0c, 0x0c, 0x12, 0x0f, 0x0d, 0x0c, 0x0c, 0x10, 0x06, 0x15, 0x0e,
 0x11, 0x11, 0x11, 0x11, 0x03, 0x08, 0x11, 0x0e, 0x02, 0x17, 0x16, 0x14, 0x11, 0x00, 0x02, 0x02,
 0x02, 0x09, 0x02, 0x03, 0x0e, 0x10, 0x1c, 0x16, 0x13, 0x09, 0x16, 0x03, 0x11, 0x02, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0e
};

const ui32 table1d7092[8] =
{
  0x00000000,
  0x00010000,
  0x00020000,
  0x000b0000,
  0x00040000,
  0x00050000,
  0x00060000,
  0x00070000
};

const ui32 table1d7072[8] =
{
  0x00000000,
  0x00010800,
  0x00204000,
  0x002b5800,
  0x00408000,
  0x0055a800,
  0x0060c000,
  0x007ff800
};

const ui32 table1d7052[8] =
{
  0x00000000,
  0x00010840,
  0x00204200,
  0x002b5ac0,
  0x00408400,
  0x0055ad40,
  0x0060c600,
  0x007fffc0
};

const ui32 table1d7042[4] =
{
  0x00000000,
  0x00000200,
  0x00000400,
  0x00000600
};

const ui8 table1d7029[20] =
{
  0x13, 0x14, 0x11, 0x12, 0x10,
  0x0e, 0x0f, 0x0c, 0x0d, 0x0b,
  0x09, 0x0a, 0x07, 0x08, 0x06,
  0x04, 0x05, 0x03, 0x01, 0x02
};

const ui8 table1d7012[23] =
{
  0x00, 0x02, 0x02, 0x01, 0x03,
  0x03, 0x01, 0x03, 0x03, 0x02,
  0x02, 0x01, 0x03, 0x03, 0x03,
  0x03, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x00, 0x00
};

const i16 table1d6fee[18] =
{
  0xffff, 0xffff, 0x0340, 0x0340, 0x0341, 0x0341,
  0xffff, 0xffff, 0x033e, 0x033c, 0x033f, 0x033d,
  0xffff, 0xffff, 0x033a, 0x033a, 0x033b, 0x033b
};

const i8 table1d6fdc[18] =
{
  0xff, 0xff, 0xcd, 0xc7, 0xce, 0xc8,
  0xff, 0xff, 0xcf, 0xc9, 0xd0, 0xca,
  0xff, 0xff, 0xd1, 0xcb, 0xd2, 0xcc
};

const i16 table1d6f9c[32] =
{
  0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0336, 0x0329,
  0x0335, 0x0328, 0x0337, 0x032a, 0x0333, 0x0326, 0x0332, 0x0325,
  0x0334, 0x0327, 0xffff, 0xffff, 0xffff, 0xffff, 0x0330, 0x0323,
  0x032f, 0x0322, 0x0331, 0x0324, 0x0320, 0x0320, 0x0321, 0x0321
};

const i8 table1d6f7c[32] =
{
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x4f, 0x3b,
  0x50, 0x3c, 0x50, 0x3c, 0x52, 0x3e, 0x53, 0x3f,
  0x53, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x55, 0x41,
  0x56, 0x42, 0x56, 0x42, 0x58, 0x44, 0x58, 0x44
};

const i8 table1d6f5c[32] =
{
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x4f, 0x3b,
  0x50, 0x3c, 0x51, 0x3d, 0x52, 0x3e, 0x53, 0x3f,
  0x54, 0x40, 0xff, 0xff, 0xff, 0xff, 0x55, 0x41,
  0x56, 0x42, 0x57, 0x43, 0x58, 0x44, 0x59, 0x45
};

const ui8 table1d6f4c[16] =
{
  0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01
};

const i16 table1d6f2c[16] =
{
  0x0ee2, 0xffff, 0xffff, 0x0ece, 0x0ec4, 0x0ed8, 0x0eb0, 0x0ea6,
  0x0eba, 0xffff, 0xffff, 0x0e92, 0x0e88, 0x0e9c, 0x0e74, 0x0e7e
};

const i8 table1d6f27[5] =
{
  0x03, 0x02, 0x01, 0x00, 0xff
};

const i16 table1d6f0b[14] =
{
  0x0000, 0x0000, 0x0000, 0x02f2, 0x02f1, 0x02f3, 0x02ef,
  0x02ee, 0x02f0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

const i8 table1d6290[9] =
{
  0x00, 0x00, 0x00, 0x01, 0x00,
  0x00, 0x00, 0x01, 0x01
};

const i16 table1d6299[5] =
{
  0x0029, 0x002a, 0x002e, 0x002d, 0x0000
};

const i8 table1d26a8[32] =
{
  0x03, 0x02, 0x01, 0x00, 0x04, 0x04, 0x04, 0x04,
  0x03, 0x02, 0x01, 0x00, 0x04, 0x04, 0x04, 0x04,
  0x04, 0x04, 0x04, 0x04, 0x01, 0x00, 0x03, 0x02,
  0x04, 0x04, 0x04, 0x04, 0x01, 0x00, 0x03, 0x02
};

const i8 table1d62ee[30] =
{
  0x41, 0x01, 0x23, 0x21, 0x21,
  0x25, 0x21, 0x21, 0x1b, 0x1b,
  0x21, 0x01, 0x05, 0x45, 0x01,
  0x03, 0x01, 0x02, 0x0a, 0x0a,
  0x4f, 0x01, 0x0a, 0x4f, 0x05,
  0x01, 0x02, 0x47, 0x00, 0x00
};

const i8 table1d62e8[4] =
{
  0, 3, 2, 1
};

const i16 table1d62e0[4] =
{
  0, 3, 6, 6
};

const i16 table1d62b0[8][2] =
{
  { 0xffff, 0x0000 },
  { 0x0000, 0xffff },
  { 0x0000, 0xffff },
  { 0x0001, 0x0000 },
  { 0x0001, 0x0000 },
  { 0x0000, 0x0001 },
  { 0xffff, 0x0000 },
  { 0x0000, 0x0001 }
};

const i16 table1d62d0[4][2] =
{
  { 0x0000, 0x0001 },
  { 0x0000, 0xffff },
  { 0x0001, 0x0000 },
  { 0xffff, 0x0000 }
};

const i8 table1d3ffc[4] =
{
  0, 1, 0, 0xff
};

const i8 table1d3ff8[4] =
{
  1, 0, 0xff, 0
};

const i16 table1d27c4[8] =
{
  0x01c8, 0x01c9, 0x01cc, 0x01cd,
  0x01ca, 0x01cb, 0x01cc, 0x01cd
};

const i16 table1d27d4[10] =
{
  0x01ce, 0x01cf, 0x01ce, 0x01cf, 0x01d2,
  0x01d3, 0x01d0, 0x01d1, 0x01d2, 0x01d3
};

const i8 table1d268e[6] =
{
  0x06, 0x08, 0x12, 0x10, 0x00, 0x00
};

const i8 table1d2660[16] =
{
  0x04, 0x08, 0x01, 0x02,
  0x0c, 0x09, 0x03, 0x06,
  0x0e, 0x0d, 0x0b, 0x07,
  0x0f, 0x0f, 0x0f, 0x0f
};

const i16 table1d2670[13] =
{
  0x0200, 0x0100, 0x0001, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040,
  0x0040, 0x0040, 0x0002, 0x0020, 0x0080
};

const i8 table1d26c8[8] =
{
  0x0b, 0x00, 0x0b, 0x09, 0x00, 0x00, 0x00, 0x0c
};

const s_4bytearray table1d26d0[8] =
{
  { 0x00, 0x01, 0x03, 0x02 },
  { 0x01, 0x00, 0x02, 0x03 },
  { 0x01, 0x02, 0x00, 0x03 },
  { 0x02, 0x01, 0x03, 0x00 },
  { 0x03, 0x02, 0x00, 0x01 },
  { 0x02, 0x03, 0x01, 0x00 },
  { 0x00, 0x03, 0x01, 0x02 },
  { 0x03, 0x00, 0x02, 0x01 }
};

const s_4bytearray table1d26f0[2] =
{
  { 0x00, 0x01, 0x02, 0x03 },
  { 0x00, 0x01, 0x02, 0x03 }
};

const i8 table1d26f8[4] =
{
  0x20, 0x10, 0x8, 0x4
};

const i16 table1d2752[4] =
{
  0x06, 0x08, 0x12, 0x10
};

const i8 table1d275a[32][2] =
{
  0xfe, 0x0a, 0xf8, 0x03, 0x07, 0xfd, 0xfa, 0x0a, 0x08, 0xf9, 0xf8, 0x0b, 0x02, 0xfe, 0x08, 0xf5, 0x00, 0x00, 0x04, 0x08, 0xf8, 0xf6, 0x06, 0x07, 0x02, 0x02, 0xfa, 0x09, 0xfd, 0xf5, 0x02, 0x0b,
  0x00, 0xf5, 0xfb, 0xf9, 0xfd, 0x03, 0x08, 0x0b, 0x00, 0xfb, 0x06, 0xf7, 0xf9, 0x03, 0x05, 0x01, 0x04, 0xf9, 0xf8, 0xfd, 0x08, 0x03, 0xfd, 0xfe, 0x00, 0x07, 0xfd, 0xfc, 0xfc, 0x00, 0x02, 0xf5
};

const i8 table1d324c[44] =
{
  0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01
};

const i16 table1d3278[16] =
{
  0x81ff, 0x81ff, 0x81ff, 0x81ff, 0x01b0, 0x0000, 0x0080, 0x81fc,
  0x0180, 0x01e0, 0x0100, 0x81ff, 0x81ff, 0x81ff, 0x81ff, 0x81ff
};

const i8 table1d3298[16] =
{
  0x0e, 0x18, 0xff, 0xff, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0xff, 0xff, 0xff, 0xff, 0x0d
};

s_bbw table1d3ed5[10];

/*const*/ s_bbw _table1d3ed5[10] =
{
  { 0x80, 0, 0x0000 },
  { 0x80, 0, 0x0001 },
  { 0x80, 0, 0x0002 },
  { 0x80, 0, 0x0003 },
  { 0x81, 0, 0x0012 },
  { 0x80, 0, 0x0014 },
  { 0x80, 0, 0x0015 },
  { 0x80, 0, 0x0016 },
  { 0x80, 0, 0x0017 },
  { 0x80, 0, 0x0009 }
};

s_wwwb table1d3d23[62];

s_wwwb _table1d3d23[62] =
{
  { 0x0002, 0x0000, 0x0000, 0x00 },
  { 0x0002, 0x0001, 0x0001, 0x00 },
  { 0x0002, 0x0002, 0x0002, 0x00 },
  { 0x0002, 0x0003, 0x0003, 0x00 },
  { 0x0002, 0x0004, 0x0004, 0x00 },
  { 0x00a1, 0x0005, 0x0005, 0x03 },
  { 0x00a1, 0x0006, 0x0006, 0x03 },
  { 0x009c, 0x0007, 0xffff, 0x0e },
  { 0x00a2, 0x0008, 0x0007, 0x04 },
  { 0x00a2, 0x0009, 0x0008, 0x04 },
  { 0x009d, 0x000a, 0xffff, 0x0f },
  { 0x00a3, 0x000b, 0x0009, 0x05 },
  { 0x00a3, 0x000c, 0x000a, 0x05 },
  { 0x009e, 0x000d, 0xffff, 0x10 },
  { 0x00a4, 0x000e, 0x000b, 0x06 },
  { 0x00a4, 0x000f, 0x000c, 0x06 },
  { 0x009f, 0x0010, 0xffff, 0x11 },
  { 0x0009, 0x0011, 0x000d, 0x07 },
  { 0x0007, 0x0012, 0xffff, 0x08 },
  { 0x0007, 0x0013, 0xffff, 0x09 },
  { 0x0002, 0x0014, 0xffff, 0x0a },
  { 0x0007, 0x0015, 0xffff, 0x08 },
  { 0x0002, 0x0016, 0xffff, 0x0a },
  { 0x81ee, 0x0017, 0xffff, 0x0b },
  { 0x0002, 0x0037, 0x000e, 0x12 },
  { 0x002f, 0x0018, 0xffff, 0x0c },
  { 0x0031, 0x0019, 0xffff, 0x00 },
  { 0x0032, 0x001a, 0xffff, 0x00 },
  { 0x0033, 0x001b, 0xffff, 0x00 },
  { 0x0034, 0x001c, 0xffff, 0x00 },
  { 0x000b, 0x001d, 0xffff, 0x0d },
  { 0x000b, 0x001e, 0xffff, 0x0d },
  { 0x000b, 0x001f, 0xffff, 0x0d },
  { 0x000b, 0x0020, 0xffff, 0x0d },
  { 0x000b, 0x0021, 0xffff, 0x01 },
  { 0x000b, 0x0022, 0xffff, 0x01 },
  { 0x000b, 0x0023, 0xffff, 0x01 },
  { 0x000b, 0x0024, 0xffff, 0x01 },
  { 0x000b, 0x0025, 0xffff, 0x01 },
  { 0x000b, 0x0026, 0xffff, 0x01 },
  { 0x000b, 0x0027, 0xffff, 0x01 },
  { 0x000b, 0x0028, 0xffff, 0x01 },
  { 0x000b, 0x0029, 0xffff, 0x01 },
  { 0x000b, 0x002a, 0xffff, 0x01 },
  { 0x000b, 0x002b, 0xffff, 0x01 },
  { 0x000b, 0x002c, 0xffff, 0x02 },
  { 0x000b, 0x002d, 0xffff, 0x02 },
  { 0x000b, 0x002e, 0xffff, 0x01 },
  { 0x0002, 0x002f, 0xffff, 0x00 },
  { 0x0002, 0x0030, 0xffff, 0x00 },
  { 0x0002, 0x0031, 0xffff, 0x00 },
  { 0x0002, 0x0032, 0xffff, 0x00 },
  { 0x0002, 0x0033, 0xffff, 0x00 },
  { 0x0002, 0x0034, 0xffff, 0x00 },
  { 0x0002, 0x0035, 0xffff, 0x00 },
  { 0x0002, 0x0036, 0xffff, 0x00 },
  { 0x0002, 0x0038, 0xffff, 0x00 },
  { 0x0002, 0x0039, 0xffff, 0x00 },
  { 0x0002, 0x003a, 0xffff, 0x00 },
  { 0x0002, 0x003b, 0x000e, 0x00 },
  { 0x0002, 0x003c, 0xffff, 0x00 },
  { 0x0002, 0x003d, 0x000e, 0x00 }
};

const i8 table1d3cd0[83] =
{
  0x80, 0x81, 0x82, 0x84, 0x85, 0x06, 0x07, 0x89, 0x0a, 0x0b, 0x8d, 0x0e, 0x0f, 0x91, 0x12, 0x13, 0x88, 0x0c, 0x10, 0x14, 0xcb, 0x9e, 0x1f, 0x20, 0x21, 0xa2, 0x1d, 0xa4, 0x25, 0x26, 0x27, 0x23,
  0xa9, 0x2a, 0x2b, 0xad, 0x2e, 0x2f, 0x30, 0x31, 0xb3, 0x34, 0x35, 0xc2, 0x43, 0x44, 0x4a, 0x47, 0x48, 0x3e, 0xbf, 0x40, 0x41, 0x49, 0x45, 0x46, 0x3d, 0xb9, 0x3a, 0xa8, 0x2c, 0x36, 0x32, 0x3c,
  0x37, 0x38, 0x97, 0x16, 0x3b, 0x15, 0x03, 0x98, 0x03, 0x9a, 0x1c, 0x1b, 0x03, 0x95, 0x1a, 0x19, 0x3b, 0x03, 0x80
};

s_bbw table1d3ba0[76];

s_bbw _table1d3ba0[76] =
{
  { 0x00, 0x00, 0x0000 }, { 0x00, 0x00, 0x0001 }, { 0x00, 0x00, 0x0002 }, { 0x00, 0x00, 0x0003 }, { 0x00, 0x00, 0x0004 }, { 0x00, 0x01, 0x0005 }, { 0x00, 0x05, 0x0006 }, { 0x00, 0x05, 0x0007 },
  { 0x83, 0x00, 0x0004 }, { 0x00, 0x02, 0x0008 }, { 0x00, 0x06, 0x0009 }, { 0x00, 0x06, 0x000a }, { 0x83, 0x01, 0x0005 }, { 0x00, 0x03, 0x000b }, { 0x00, 0x07, 0x000c }, { 0x00, 0x07, 0x000d },
  { 0x83, 0x02, 0x0006 }, { 0x00, 0x04, 0x000e }, { 0x00, 0x08, 0x000f }, { 0x00, 0x08, 0x0010 }, { 0x83, 0x03, 0x0007 }, { 0x84, 0x00, 0x0008 }, { 0x00, 0x00, 0x0011 }, { 0x00, 0x00, 0x0012 },
  { 0x00, 0x00, 0x0013 }, { 0x00, 0x00, 0x0014 }, { 0x00, 0x00, 0x0015 }, { 0x00, 0x00, 0x0016 }, { 0x00, 0x00, 0x0017 }, { 0x00, 0x01, 0x0019 }, { 0x00, 0x00, 0x001a }, { 0x00, 0x01, 0x001b },
  { 0x00, 0x02, 0x001c }, { 0x00, 0x03, 0x001d }, { 0x85, 0x00, 0x000a }, { 0x86, 0x63, 0x000b }, { 0x00, 0x00, 0x001e }, { 0x00, 0x01, 0x001f }, { 0x00, 0x02, 0x0020 }, { 0x00, 0x03, 0x0021 },
  { 0x87, 0x00, 0x000c }, { 0x00, 0x01, 0x0022 }, { 0x00, 0x02, 0x0023 }, { 0x00, 0x03, 0x0024 }, { 0x88, 0x01, 0x000d }, { 0x00, 0x00, 0x0025 }, { 0x00, 0x01, 0x0026 }, { 0x00, 0x02, 0x0027 },
  { 0x00, 0x03, 0x0028 }, { 0x00, 0x05, 0x0029 }, { 0x8a, 0x03, 0x000e }, { 0x00, 0xff, 0x002a }, { 0x00, 0x0f, 0x002b }, { 0x00, 0x1e, 0x002c }, { 0x89, 0x02, 0x000f }, { 0x00, 0x05, 0x002d },
  { 0x00, 0x06, 0x002f }, { 0x82, 0x01, 0x0010 }, { 0x82, 0x00, 0x0011 }, { 0x8b, 0x00, 0x0013 }, { 0x00, 0x04, 0x002e }, { 0x00, 0x07, 0x0030 }, { 0x00, 0x07, 0x0031 }, { 0x00, 0x01, 0x0032 },
  { 0x00, 0x02, 0x0033 }, { 0x00, 0x03, 0x0034 }, { 0x00, 0x01, 0x0035 }, { 0x00, 0x02, 0x0036 }, { 0x00, 0x03, 0x0037 }, { 0x00, 0x05, 0x003a }, { 0x00, 0x06, 0x003b }, { 0x00, 0x05, 0x003c },
  { 0x00, 0x06, 0x003d }, { 0x00, 0x04, 0x0038 }, { 0x00, 0x04, 0x0039 }, { 0x00, 0x00, 0x0018 }
};

/*const*/ s_ww2 v1d39bc[121]; // read by file
/*const*/ s_www v1d338c[264]; // read by file

const i8 table1d6afe[23] =
{
  0x00, 0xff, 0x01, 0x00, 0xff, 0x01, 0x00, 0xff, 0x01, 0xfe, 0x02, 0x00, 0xff, 0x01, 0xfe, 0x02, 0x00, 0xff, 0x01, 0xfe, 0x02, 0xfd, 0x03
};

const s_4bytearray table1d6a74[23] =
{
  { 0x01, 0x02, 0x03, 0xff },
  { 0xff, 0x00, 0x04, 0xff },
  { 0x00, 0xff, 0x05, 0xff },
  { 0x04, 0x05, 0x06, 0x00 },
  { 0xff, 0x03, 0x07, 0x01 },
  { 0x03, 0xff, 0x08, 0x02 },
  { 0x07, 0x08, 0x0b, 0x03 },
  { 0x09, 0x06, 0x0c, 0x04 },
  { 0x06, 0x0a, 0x0d, 0x05 },
  { 0xff, 0x07, 0x0e, 0xff },
  { 0x08, 0xff, 0x0f, 0xff },
  { 0x0c, 0x0d, 0x10, 0x06 },
  { 0x0e, 0x0b, 0x11, 0x07 },
  { 0x0b, 0x0f, 0x12, 0x08 },
  { 0xff, 0x0c, 0x13, 0x09 },
  { 0x0d, 0xff, 0x14, 0x0a },
  { 0x11, 0x12, 0xff, 0x0b },
  { 0x13, 0x10, 0xff, 0x0c },
  { 0x10, 0x14, 0xff, 0x0d },
  { 0x15, 0x11, 0xff, 0x0e },
  { 0x12, 0x16, 0xff, 0x0f },
  { 0xff, 0x13, 0xff, 0xff },
  { 0x14, 0xff, 0xff, 0xff }
};

const i8 table1d6ad0[23][2] =
{
  { 0x00, 0x00 }, { 0xff, 0x00 }, { 0x01, 0x00 }, { 0x00, 0x01 },
  { 0xff, 0x01 }, { 0x01, 0x01 }, { 0x00, 0x02 }, { 0xff, 0x02 },
  { 0x01, 0x02 }, { 0xfe, 0x02 }, { 0x02, 0x02 }, { 0x00, 0x03 },
  { 0xff, 0x03 }, { 0x01, 0x03 }, { 0xfe, 0x03 }, { 0x02, 0x03 },
  { 0x00, 0x04 }, { 0xff, 0x04 }, { 0x01, 0x04 }, { 0xfe, 0x04 },
  { 0x02, 0x04 }, { 0xfd, 0x04 }, { 0x03, 0x04 }
};

const i8 table1d6b43[23] =
{
  0x0b, 0xff, 0xff, 0x08, 0x09, 0x0a, 0x05, 0x06, 0x07, 0xff, 0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

const i8 table1d6b5a[23] =
{
  0x0e, 0x0f, 0x10, 0x0b, 0x0c, 0x0d, 0x08, 0x09, 0x0a, 0xff, 0xff, 0x03, 0x04, 0x05, 0x06, 0x07, 0x00, 0x01, 0x02, 0xff, 0xff, 0xff, 0xff
};

const i8 table1d6b2c[23] =
{
  0x00, 0x02, 0x01, 0x03, 0x05, 0x04, 0x06, 0x08, 0x07, 0x0a, 0x09, 0x0b, 0x0d, 0x0c, 0x0f, 0x0e, 0x10, 0x12, 0x11, 0x14, 0x13, 0x16, 0x15
};

const i8 table1d6b15[23] =
{
  0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04
};

/*const*/ i8 table1d6a54[4][4] =
{
  0x02, 0x00, 0x01, 0x03, 0x02, 0x01, 0x00, 0x03, 0x02, 0x03, 0x00, 0x01, 0x02, 0x01, 0x03, 0x00
};

/*const*/ i8 table1d6a64[4][4] =
{
  0x00, 0x02, 0x01, 0x03, 0x01, 0x00, 0x02, 0x03, 0x02, 0x03, 0x00, 0x01, 0x03, 0x01, 0x02, 0x00
};

const i8 table1d6b71[5] =
{
  0x60, 0x40, 0x2b, 0x1c, 0x13
};

const i8 table1d6efd[14] =
{
  0xff, 0xff, 0xff, 0x12, 0x13, 0x14, 0x15,
  0x16, 0x17, 0xff, 0xff, 0xff, 0xff, 0xff
};

const i8 table1d6ee1[14][2] =
{
  { 0xd3, 0xd4 }, { 0xff, 0xff }, { 0xff, 0xff }, { 0x07, 0x08 },
  { 0xff, 0xd5 }, { 0xd6, 0xff }, { 0x09, 0x0a }, { 0xff, 0xd7 },
  { 0xd8, 0xff }, { 0xff, 0xff }, { 0xff, 0xff }, { 0x0b, 0x0c },
  { 0x0d, 0x0e }, { 0x0f, 0x10 }
};

const i8 table1d6ed3[14] =
{
  0x04, 0xff, 0xff, 0x03, 0xff, 0xff, 0x02, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0x00
};

const s_bb table1d6eb3[16] =
{
  { 0x08, 0x00 },
  { 0x04, 0x00 },
  { 0x0c, 0x00 },
  { 0x08, 0x04 },
  { 0x04, 0x04 },
  { 0x0c, 0x04 },
  { 0x08, 0x08 },
  { 0x04, 0x08 },
  { 0x0c, 0x08 },
  { 0x00, 0x08 },
  { 0x10, 0x08 },
  { 0x08, 0x0c },
  { 0x04, 0x0c },
  { 0x0c, 0x0c },
  { 0x00, 0x0c },
  { 0x10, 0x0c }
};

const i8 table1d6ea8[3] =
{
  0x01, 0x01, 0x02
};

const i8 table1d6eab[4] =
{
  0x00, 0x03, 0x02, 0x01
};

const i8 table1d6eaf[4] =
{
  0x16, 0x0a, 0x02, 0x0e
};

const s_bb table1d6e68[4][8] =
{
 {{ 0x00, 0xff }, { 0x01, 0x00 }, { 0x01, 0x00 }, { 0x00, 0x01 }, { 0x00, 0x01 }, { 0xff, 0x00 }, { 0xff, 0x00 }, { 0x00, 0xff }},
 {{ 0x01, 0xff }, { 0x01, 0xff }, { 0x01, 0x01 }, { 0x01, 0x01 }, { 0xff, 0x01 }, { 0xff, 0x01 }, { 0xff, 0xff }, { 0xff, 0xff }},
 {{ 0x01, 0xfe }, { 0x02, 0xff }, { 0x02, 0x01 }, { 0x01, 0x02 }, { 0xff, 0x02 }, { 0xfe, 0x01 }, { 0xfe, 0x01 }, { 0xff, 0xfe }},
 {{ 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }}
};

const i8 table1d6e51[23] =
{
  0xff, 0xff, 0xff, 0x06, 0xff, 0xff, 0x03, 0x04, 0x05, 0xff, 0xff, 0x00, 0x01, 0x02, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

const i8 table1d6797[37] =
{
  0x6d, 0x65, 0x6d, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x8b, 0x1b, 0x1c,
  0x00, 0xa5, 0x1d, 0x1c,
  0x00, 0xdc, 0x1d, 0x1c,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x3e, 0x1e, 0x1c,
  0x00, 0x00, 0x00, 0x00, 0x00
};

i8 v1d67be[4];

i8 _v1d67be[4] =
{
  0xff, 0xff, 0xff, 0xff
};

const i8 table1d6e41[16] =
{
  0x03, 0x03, 0x03, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00, 0x01, 0x00, 0x01
};

const i8 table1d6e35[12] =
{
  0x06, 0x08, 0x10, 0x12, 0x08, 0x06, 0x12, 0x10, 0x06, 0x08, 0x10, 0x12
};

const i16 table1d6c70[16] =
{
  0x035e, 0x035d, 0x035f, 0x035b,
  0x035a, 0x035c, 0x0358, 0x0357,
  0x0359, 0xffff, 0xffff, 0x0355,
  0x0354, 0x0356, 0x0352, 0x0353
};

const i8 table1d6c90[16] =
{
  0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01
};

const i8 table1d6ca0[16] =
{
  0x82, 0x83, 0x83, 0x85, 0x86, 0x86, 0x88, 0x89,
  0x89, 0xff, 0xff, 0x76, 0x77, 0x77, 0x79, 0x79
};

const i8 table1d6cb0[16] =
{
  0x6b, 0x6c, 0x6c, 0x6e, 0x6f, 0x6f, 0x71, 0x72,
  0x72, 0xff, 0xff, 0x76, 0x77, 0x77, 0x79, 0x79
};

const i16 table1d6cc0[16] =
{
  0x02be, 0x02bf, 0x02c0, 0x02c1,
  0x02c2, 0x02c3, 0x02c4, 0x02c5,
  0x02c6, 0x02c7, 0x02c8, 0x02c9,
  0x02ca, 0x02cb, 0x02cc, 0x02cd
};

const i8 table1d6c10[5] = { 0x00, 0x00, 0x0c, 0x1c, 0x2e };
const i8 table1d6c19[5] = { 0x00, 0x03, 0x06, 0x0b, 0x10 };
const i8 table1d6c1e[23] = { 0x00, 0x01, 0x01, 0x02, 0x03, 0x03, 0x04, 0x05, 0x05, 0x06, 0x06, 0x07, 0x08, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09 };
const i8 table1d6c35[23] = { 0x02, 0x03, 0x03, 0x02, 0x03, 0x03, 0x02, 0x03, 0x03, 0x03, 0x03, 0x02, 0x03, 0x03, 0x03, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 };
const i16 table1d6c4c[9] = { 0x0367, 0x0366, 0x0368, 0x0364, 0x0363, 0x0365, 0x0361, 0x0360, 0x0362 };
const i8 table1d6c5e[9] = { 0x99, 0x9a, 0x9a, 0x9c, 0x9d, 0x9d, 0x9f, 0xa0, 0xa0 };
const i8 table1d6c67[9] = { 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01 };

const i8 table1d6c06[5] =
{
  0x00, 0xf9, 0xf7, 0xf6, 0x00
};

const i8 table1d6c0b[5] =
{
  0x00, 0x00, 0x05, 0x13, 0x24
};

const s_www table1d6a31[5] =
{
  { 0x0002, 0x0005, 0xffff },
  { 0x000a, 0x000a, 0xffff },
  { 0x0000, 0x0001, 0x0005 },
  { 0x0000, 0x0001, 0x0006 },
  { 0x000d, 0x001d, 0xffff }
};

const t_text v1d6a08[38] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.;: -()'?!";
// Note: original without nullbyte at the end, size 0x25

const i8 table1d6e03[26][2] =
{
  {0x00, 0x00}, {0x01, 0x00}, {0x02, 0x00}, {0x03, 0x00},
  {0x04, 0x00}, {0x00, 0x01}, {0x01, 0x01}, {0x02, 0x01},
  {0x03, 0x01}, {0x04, 0x01}, {0x00, 0x02}, {0x01, 0x02},
  {0x02, 0x02}, {0x03, 0x02}, {0x04, 0x02}, {0x00, 0x03},
  {0x01, 0x03}, {0x02, 0x03}, {0x03, 0x03}, {0x04, 0x03},
  {0x00, 0x04}, {0x01, 0x04}, {0x02, 0x04}, {0x03, 0x04},
  {0x04, 0x04}
};

const i8 table1d6de3[16][2] =
{
  {0x08, 0x04}, {0x04, 0x04}, {0x0c, 0x04}, {0x08, 0x08},
  {0x04, 0x08}, {0x0c, 0x08}, {0x08, 0x0c}, {0x04, 0x0c},
  {0x0c, 0x0c}, {0x00, 0x0c}, {0x10, 0x0c}, {0x08, 0x10},
  {0x04, 0x10}, {0x0c, 0x10}, {0x00, 0x10}, {0x10, 0x10}
};

const i8 table1d6dd3[16] =
{
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01
};

const i8 table1d69aa[6] = { 0x05, 0x05, 0x04, 0x06, 0x03, 0x01 };
const i8 table1d69a2[6] = { 0x00, 0x03, 0x02, 0x01, 0x00, 0x00 };
const i16 table1d6998[5] = { 0xfffe, 0xfffd, 0x0000, 0xffff, 0xfffe };
const i8 table1d6984[20] =
{
  0x00, 0x01, 0x03, 0x02, 0x00,
  0x00, 0x02, 0x02, 0x00, 0x02,
  0x02, 0x00, 0x00, 0x00, 0xfe,
  0xfe, 0x00, 0xfe, 0xfe, 0x00
};
const i8 table1d6980[4] = { 0x00, 0x00, 0x03, 0x03 };

const i8 table1d69b0[32] =
{
  0x05, 0x04, 0x09, 0x08, 0x07, 0x0c, 0x06, 0x0b,
  0x03, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13,
  0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
  0x1c, 0x1d, 0x0a, 0x02, 0x00, 0x01, 0x00, 0x00
};

const i8 table1d69d0[4] =
{
  0x07, 0x0b, 0x08, 0x0e
};

const i8 table1d6b76[132] =
{
  0x60, 0x57, 0x4e, 0x47, 0x40, 0x3a, 0x34, 0x2f, 0x2b, 0x27, 0x23, 0x1f, 0x1c, 0x1a, 0x17, 0x15, 0x13, 0x11, 0x0f, 0x02, 0x01, 0x00, 0x01, 0x02, 0x02, 0x00, 0x00, 0x40, 0x34, 0x2b, 0x23, 0x1c,
  0x17, 0x13, 0x02, 0x05, 0x00, 0x06, 0x05, 0x07, 0x03, 0x00, 0x07, 0x01, 0x01, 0x02, 0x06, 0x03, 0x03, 0x03, 0x05, 0x05, 0x02, 0x06, 0x07, 0x07, 0x01, 0x00, 0x03, 0x01, 0x06, 0x02, 0x01, 0x03,
  0x05, 0x03, 0x02, 0x0e, 0x16, 0x16, 0x16, 0x0a, 0x0c, 0x34, 0x40, 0x4e, 0x4e, 0x4e, 0x40, 0x40, 0x00, 0x01, 0x02, 0x03, 0x00, 0xfd, 0xfe, 0xff, 0x18, 0x18, 0x18, 0x20, 0x20, 0x20, 0x30, 0x1e,
  0x1e, 0x1e, 0x28, 0x28, 0x28, 0x40, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
  0x01, 0x01, 0x01, 0x00
};

const i16 table1d6d3c[6] =
{
  0x002a, 0x0029, 0x002b, 0x002c, 0x002d, 0x0028
};

const i8 table1d6d48[6] =
{
  0x12, 0x10, 0x14, 0x16, 0x18, 0x0e
};

const i8 table1d6d4e[6] =
{
  0x00, 0x01, 0x02, 0x02, 0x02, 0x03
};

const i8 table1d6d54[6] =
{
  0x00, 0x00, 0x01, 0x02, 0x03, 0x00
};

const i8 table1d6ce0[23 * 4] =
{
  0x3b, 0xff, 0xe0, 0x88, 0x3f, 0x05, 0x21, 0x88, 0x3f, 0x85, 0x21, 0x88, 0x3d, 0xff, 0xa0, 0x6f, 0x3f, 0x04, 0x3c, 0x6f, 0x3f, 0x84, 0x3c, 0x6f, 0x3c, 0xff, 0x6a, 0x4a, 0x3f, 0x03, 0x4e, 0x4a,
  0x3f, 0x83, 0x4e, 0x4a, 0x3f, 0x02, 0x08, 0x34, 0x3f, 0x82, 0x08, 0x34, 0x3f, 0xff, 0x46, 0x31, 0x3f, 0x01, 0x53, 0x31, 0x3f, 0x81, 0x53, 0x31, 0x3f, 0x00, 0x24, 0x31, 0x3f, 0x80, 0x24, 0x31,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const i8 table1d6d5a[4][5] =
{
  0x00, 0x01, 0xff, 0x03, 0x02,
  0x01, 0x02, 0xff, 0x00, 0x03,
  0x02, 0x03, 0xff, 0x01, 0x00,
  0x03, 0x00, 0xff, 0x02, 0x01
};

const s_bb table1d6d6e[4] =
{
  { 0x00, 0x01 },
  { 0x04, 0x05 },
  { 0x48, 0x49 },
  { 0x44, 0x45 }
}; // TODO: Note, last 4 bytes build the string 'HIDE'

const i8 table1d6d76[2][4] =
{
  0xff, 0x01, 0x01, 0xff,
  0x00, 0x01, 0x00, 0xff
};

const i8 table1d6d7e[2][4] =
{
  0x01, 0x01, 0xff, 0xff,
  0x02, 0x00, 0xfe, 0x00
};

const i8 table1d6d86[2] =
{
  0x3a, 0x35
};

const i8 table1d6d88[25] =
{
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18
};

const i8 table1d6da1[25] =
{
  0x04, 0x03, 0x02, 0x01, 0x00, 0x09, 0x08, 0x07, 0x06, 0x05, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x13, 0x12, 0x11, 0x10, 0x0f, 0x18, 0x17, 0x16, 0x15, 0x14
};

const i8 table1d6dba[25] =
{
  0x00, 0x04, 0x01, 0x03, 0x02, 0x05, 0x09, 0x06, 0x08, 0x07, 0x0a, 0x0e, 0x0b, 0x0d, 0x0c, 0x0f, 0x13, 0x10, 0x12, 0x11, 0x14, 0x18, 0x15, 0x17, 0x16
};

const i8 table1d67d9[7] =
{
  0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00
};

const i8 table1d67e0[5][6] =
{
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
  0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x04, 0x05, 0x06, 0x07, 0x07, 0x09,
  0x02, 0x02, 0x03, 0x04, 0x06, 0x07,
  0x08, 0x0c, 0x10, 0x14, 0x18, 0x1c
};

const i8 table1d67fe[4] =
{
  0x02, 0x03, 0x01, 0x01
};

/*const*/ i8 table1d6802[272];

const t_text* table1d6912[18] =
{
  "SK", "LV", "CM", "BZ", "TR", "ST",
  "PA", "TA", "NC", "EX", "PB", "DM",
  "MS", "SD", "RP", "HN", "AT", "WH"
};

// TODO: those are twobyte-strings!
const i16 table1d292c[32] =
{
0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006a, 0x006b, 0x006c, 0x006d, 0x006e, 0x006f, 0x0070,
0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037
};

/*const*/ i8 table1d296c[63][36];

i8 table1d70b4[17];

i8 _table1d70b4[17] =
{
  0x00, 0xfc, 0x1e, 0xfd, 0x64, 0xfe, 0x01, 0x03, 0xfc, 0x19, 0xfd, 0x3c, 0xfe, 0xfe, 0x02, 0xfe, 0xff
};

void c_dm2data::init(void)
{
  i16 i = 0;
  i16 j = 0;
//--- some tables first
  DM2_READ_BINARY("v1d39bc.dat", v1d39bc, 0x1e4);
  DM2_READ_BINARY("v1d338c.dat", v1d338c, sizeof(v1d338c));
  DM2_READ_BINARY("v1d6802.dat", table1d6802, 0x110); 
  DM2_READ_BINARY("v1d296c.dat", table1d296c, 0x3f * 0x24);

  mainarg1 = 0x1; // at MAIN_ARGC
  mainarg2 = NULL; // pointer_at(MAIN_ARGV); // contents: 0x1410dd, TODO: link?!?

  // following table is not used anymore!
/*
  s_exittableentry* p = (s_exittableentry*)adress_of(EXITTABLE1);
  for (i16 i = 0; i < sizeof(exittable) / sizeof(s_exittableentry); i++, p++)
  {
    exittable[i].ubv1 = p->ubv1; // TODO: read values
    exittable[i].ubv2 = p->ubv2; // TODO: read values
    exittable[i].func = p->func; // TODO: convert, check what function it is
  }
*/

/*
   important:
   ddat.v1dff24 is the index to access GETFPIL(idx(0x1d87ac) + ...)
   in c_sound.cpp. From here we get segment:offset-accesses.
   First thing is: ddat.v1dff24 is always 0, so only the first
   table-entry is used. This is the adress 0x1d8:0x000012f8 then,
*/

  // obsolete: drvinterfaceptr = DOWNCAST(i16, DRVW_drvinterface);
  datafoldername = CHGCAST(t_text, dfn);
  v1e0988 = 0;
  for (i = 0; i < 2; i++) vp_1e097c[i] = NULL;
  markerror = 0;
  errhandling = 0;
  errversion = 5;
  tickconst = 1;
  gametime = 0;
  ticksum = 0;
  ticktrig = 0;
  tickevent = false;
  v1e01d0 = false; // read before written, so still .data-area
  v1e01d8 = false; // read before written, so still .data-area
  v1e025c = 0;
  ptr1e1044 = NULL;
  vlong1e0a3c = 0;
  vlong1e0a44 = 0;
  vlong1e0a40 = 0;
  v1e0ac2 = 0;
  p1e0a64 = NULL;
  v1e0a88 = false;
  v1e0ad0 = 0;
  v1e0ad2 = 0;
  // v1e0080 = 0;
  dialog1 = 0;
  dialog2 = 1;

  savegamew1 = 0;
  savegamefilehandle1 = 0;
  savegamewpc.init();
  savegamel1 = 0;
  savegamel2 = false;
  savegamew4 = 0xffffffff;
  savegamew6 = 0;
  savegamep1 = NULL;
  savegamew7 = 0;
  savegamew8 = 0;
  savegames1.init();
  savegameb1 = 1;
  savegamep3 = NULL;
  v1d3248 = 0xffffffff;
  savegamep4 = NULL;
  for (i = 0; i < 0x100; i++) savegameb2[i] = 0;
  v1e092f = 0;
  v1e092e = 0;
  v1e08e0 = 0;
  v1d6525 = 0;
  v1d6529 = 0;
  v1e0992 = 0;
  v1e099c = 0;
  v1e021c = 0;
  v1e0270 = 0;
  v1e0272 = 0;
  v1e0258 = 0;
  v1e0266 = 0;
  v1d26a4 = 0xffffff38;
  v1e01a0 = 0;
  v1e026e = 0;
  v1e025e = 0;
  v1e0274 = 0;
  v1d26a0 = 0x7;
  v1d26a2 = 0x5;
  v1e147f = 0;
  v1e1480 = 0;
  v1e1483 = 0;
  v1e1482 = 0;
  v1e147e = 0;
  v1e147d = 0;
  v1e1484 = 0;
  v1e1474 = 0;
  v1e147b = 0;
  v1e1478 = 0;
  v1e1434 = 0;
  for (i = 0; i < 8; i++) v1e0104[i] = 0;
  for (i = 0; i < 0x40; i++) globalb[i] = 0;
  for (i = 0; i < 0x40; i++) globalw[i] = 0;
  for (i = 0; i < 4; i++) v1d6316[i] = 0xff;
  v1e0250 = 0;
  v1e0390.l_00 = 0;
  v1d651b = 0x1;
  v1e08e4 = NULL;
  v1e08f0 = NULL;
  v1d651d = 0;
  v1d6312 = 0;
  v1d6310 = 0;
  v1d6521 = 0;
  v1e03f4 = NULL;
  v1d4020 = 0;
  v1e08fc = 0;
  v1e08f8 = 0;
  v1e13f4 = 0;
  v1e0900.init();
  v1e03c8 = NULL;
  v1e03d8 = NULL;
  v1e0408 = 0;
  v1e03d0 = NULL;
  v1e03e0 = NULL;
  v1d62a4 = 0xffff;
  v1d62a6 = 0xffff;
  v1e08c8 = NULL;
  v1e08cc = NULL;
  v1e08d0 = 0;
  v1e08d2 = 0;
  v1e0282 = 0;
  v1e027c = 0;
  v1e0238 = 0;
  v1e03c0 = NULL;
  v1dff24 = 0;
  // v1dff7c = &memory[0x1410ec]; -> see table1410ec
  v1d13fc = 0;
  v1d153a = v1d152c;
  v1d153e = v1d1530;
  v1d1542 = v1d1534;
  v1d1546 = v1d1537;
  v1d675a = v1d10f9;
  v1d70dc = 0;
  v1d70e4 = v1d113d;
  vcapture2 = false;
  vcapture1 = false;
  vcapture3 = false;
  v1e03a8 = 0;
  v1e048c = 0;
  v1e0478 = 0;
  v1d6766 = v1d110f;
  v1d676a = v1d1104;
  v1e1050 = NULL;
  v1e104c = NULL;
  v1e1048 = NULL;
  v1e103c = NULL;
  v1e1038 = NULL;
  v1e09a0 = NULL;
  v1e0950 = NULL;
  v1e0954 = NULL;
  v1e03f0 = NULL;
  v1e03e4 = NULL;
  v1e03dc = NULL;
  v1e03d4 = NULL;
  v1e03c4 = NULL;
  v1e02ec = NULL;
  v1e0210 = NULL;
  v1e020c = NULL;

  v1d70e8[0] = '\n'; v1d70e8[1] = '\0'; v1d70e8[2] = '\0'; v1d70e8[3] = '\0';
  v1e1481 = 0;
  v1e147c = 0;
  v1e147a = 0;
  v1e1479 = 0;
  v1e1476 = 0;
  v1e1472 = 0;
  v1e1470 = 0;
  v1e1420 = 0;
  v1e141c = 0;
  v1e141e = 0;
  for (i = 0; i < 3; i++) v1e1408[i] = 0;
  v1e13f0 = 0;
  v1e1040 = 0;
  v1e09a4 = 0;
  v1e1438 = 0;
  for (i=0; i<5; i++)
    for (j=0; j<10; j++)
      v1e143c[i].barr_00[j] = 0;
  v1e146e = 0;
  v1e12d6 = 0;
  v1e12d8 = 0;
  v1e12da = 0;
  v1e12c8 = 0;
  v1e12ca = 0;
  v1e12cc = 0;
  v1e12ce = 0;
  v1e12d0 = 0;
  v1e12d2 = 0;
  v1e098a = 0;
  v1e098c = 0;
  v1e098e = 0;
  v1e0994 = 0;
  v1e0996 = 0;
  v1e0998 = 0;
  v1e099a = 0;
  v1e0ff6 = 0;
  v1e0ff8 = 0;
  for (i = 0; i < 0x12; i++) v1e0ffa[i] = 0;
  v1e1020 = 0;
  v1e1024 = 0;
  v1e1026 = 0;
  v1e1028 = 0;
  v1e102a = 0;
  v1e102c = 0;
  v1e102e = 0;
  v1e101e = 0;
  v1e101c = 0;
  v1e1018 = 0;
  v1e1014 = 0;
  v1e100c = 0;
  v1e13e9 = 0;
  for (i = 0; i < 13; i++) v1e13dc[i] = 0;
  for (i = 0; i < 64; i++) v1e0adc[i] = 0;
  v1e0984 = 0;
  v1e0976 = 0;
  v1e0974 = 0;
  v1e0978 = 0;
  v1e0526 = 0;
  v1e051e = 0;
  v1e0510 = 0;
  v1e0484 = 0;
  v1e0488 = false;
  for (i = 0; i < 4; i++) v1e0bb0[i] = 0;
  for (i = 0; i < 4; i++) v1e0ba8[i] = 0;
  v1e0ba4 = 0;
  v1e0b99 = 0;
  for (i = 0; i < 20; i++) v1e0b85[i] = '\0';
  v1e0b84 = 0;
  v1e0b83 = 0;
  v1e0b82 = 0;
  v1e0b81 = 0;
  v1e0b80 = 0;
  v1e0b7f = 0;
  v1e0b7e = 0;
  v1e0b7c = 0;
  v1e0b7a = 0;
  v1e0b78 = 0;
  v1e0b72 = 0;
  v1e0b70 = 0;
  v1e0b6e = 0;
  v1e0b6c = 0;
  v1e0b6a = 0;
  v1e0b68 = 0;
  v1e0b66 = 0;
  v1e0b64 = 0;
  v1e0b62 = 0;
  v1e0b60 = 0;
  v1e0b5e = 0;
  v1e0b5c = 0;
  v1e0b5a = 0;
  v1e0b58 = 0;
  v1e0b56 = 0;
  v1e0b54 = 0;
  v1e0b52 = 0;
  v1e0b50 = 0;
  v1e0b4e = 0;
  v1e0b4c = 0;
  for (i = 0; i < 3; i++) v1e0b40[i].init();
  for (i = 0; i < 4; i++) for (int j = 0; j < 3; j++) v1e0b34[i][j] = 0;
  v1e0b30 = 0;
  v1e0b2c = 0;
  for (i = 0; i < 4; i++) v1e0b1c[i] = 0;
  for (i = 0; i < 4; i++) v1e096c[i] = 0;
  for (i = 0; i < 32; i++) v1e0930[i] = '\0';
  v1e092a = 0;
  v1e08da = 0;
  v1e08d8 = 0;
  v1e08d6 = 0;
  v1e08d4 = 0;
  v1e08c4 = 0;
  for (i = 0; i < 4; i++) v1e08c0[i] = 0;
  v1e08bf = 0;
  v1e08be = 0;
  v1e08b8.init();
  v1e08b6 = 0;
  v1e08b7 = 0;
  v1e08b4 = 0;
  v1e08b2 = 0;
  v1e08b0 = 0;
  v1e08ae = 0;
  v1e08ac = 0;
  v1e08aa = 0;
  v1e08a8 = 0;
  v1e08a6 = 0;
  v1e08a4 = 0;
  v1e08a0 = 0;
  for (i = 0; i < 2; i++) v1e040e[i] = 0;
  v1e040c = 0;
  v1e040a = 0;
  v1e0288 = 0;
  v1e0286 = 0;
  v1e0280 = 0;
  v1e027e = 0;
  v1e0278 = 0;
  v1e026a = 0;
  v1e0268 = 0;
  v1e0264 = 0;
  v1e0262 = 0;
  v1e0260 = 0;
  v1e0256 = 0;
  v1e0254 = 0;
  v1e0234 = 0;
  v1e0218 = 0;
  v1e024c = 0;
  v1e0248 = 0;
  v1e0240 = 0;
  v1e023c = 0;
  v1e0548 = 0;
  v1e01d4 = 0;
  v1e0438 = 0;
  for (i = 0; i < 18; i++) v1e0426[i] = 0;
  for (i = 0; i < 18; i++) v1e0414[i] = 0;
  v1e03f8 = 0;
  v1e03fa = 0;
  v1e03fc = 0;
  v1e03fe = 0;
  v1e0400 = 0;
  v1e0402 = 0;
  v1e0404 = 0;
  v1e0406 = 0;
  for (i = 0; i < 10; i++) v1e03ac[i] = 0;
  for (i = 0; i < 10; i++) v1e0394[i] = 0;
  for (i = 0; i < 13; i++) v1e02f0[i].init();
  for (i = 0; i < 16; i++) v1e02dc[i] = 0;
  for (i = 0; i < 16; i++) v1e02cc[i] = 0;
  v1e0204 = 0;
  v1e01a4 = 0;
  v1e01a6 = 0;
  v1e01a8 = 0;
  v1e00b0 = 0;
  v1e00b2 = 0;
  v1e00b4 = 0;
  v1e00b6 = 0;
  v1e00b8 = 0;
  for (i = 0; i < 56; i++) v1e000c[i] = 0;
  v1d718c = 0;
  v1d7188 = 1;

  DM2_READ_BINARY("v1d7108.dat", v1d7108, 0x80);

  v1d70d3 = 1;
  v1d70ea = 0;
  v1d70cf = 1;
  v1d70cb = 0;
  v1d70c9 = 0xffff;
  v1d70c5 = 0xff676980;
  v1d7041 = 0xff;
  v1d703f = 2;
  v1d703d = 0xfffd;
  last_inputtime = -1;
  v1d4000 = 0xffff;
  v1d27f8 = 0xffff;
  v1d2736 = 1;
  v1d2710 = 3;
  v1d2708 = 0x7700;
  v1d66f8 = 0;
  v1d62a8 = 0xffff;
  v1d62aa = 0xffff;
  v1d62ac = 0xffff;
  v1d62ec = 1;
  v1d652d = 1;
  v1d674c = 0;
  v1d6727 = 1;
  v1d268a = 0;
  v1d26fc = 0;
  v1d26fe = 0x0028;
  v1d271a = 0x10;
  v1d271c = 0x10;
  v1d2722 = 1;
  v1d2724 = 1;
  v1d2726 = 1;
  v1d2728 = 1;
  v1d272a = 1;
  v1d272c = 0x11;
  v1d272e = 0x11;
  v1d2744 = 0x140;
  v1d2746 = 7;
  v1d2748 = 2;
  v1d274a = 7;
  v1d274c = 7;
  v1d274e = 1;
  v1d2750 = 1;
  v1d324a = 6;
  v1d3ff1 = 0xffff;
  v1d66fc = 0;
  v1d66fe = 0xffff;
  v1d6700 = 0xffff;

  DM2_READ_BINARY("v1d653c.dat", v1d653c, 0x37 * 8);

  v1d6795 = 0;
  v1d6c00 = 0xffff;
  v1d6c02 = 0xff;
  v1d6c04 = 0xfffe;
  v1d67bc = 0xffff;
  v1d67d6 = 0xff;
  v1d6948 = 0;
  v1d694a = 0xffff;
  v1d6a2d = 0;
  v1d6c15 = 1;
  v1d6bfa = 0;
  v1d6bfc = 0;
  v1e0206 = 0;
  v1e0207 = 0;
  v1e0208 = 0;
  v1e052e = 0;
  v1e0530 = 0;
  for (i = 0; i < 2; i++) v1e052c[i] = '\0';
  v1e0534 = 0;
  v1e0538 = 0;
  v1e053a = 0;
  v1e053c = 0;
  v1e053e = 0;
  v1e0540 = 0; // TODO: check
  v1e0542 = 0; // TODO: check
  v1e0544 = 0; // TODO: check
  v1e0546 = 0; // TODO: check
  v1d2740 = 8;
  v1d273e = 8;
  v1d270e = 0xfffe;
}

// ################################################################

void c_bbbb::init(void)
{
  b_00 = 0;
  b_01 = 0;
  b_02 = 0;
  b_03 = 0;
}

// ################################################################

void c_wbbb::init(void)
{
  w_00 = 0;
  b_02 = b_03 = b_04 = dummy = 0;
}

// ################################################################

void c_5bytes::init(void)
{
  b_00 = b_01 = b_02 = b_03 = b_04 = 0;
}

// ################################################################

void c_hex2a::init(void)
{
  w_00 = 0;
  for (i16 i = 0; i < 0x24; i++) text[i] = 0;
  l_26 = 0;
}

// ################################################################

void c_rwbb::init(void)
{
  rc_00.init();
  w_08 = 0;
  b_0a = 0;
  b_0b = 0;
}

// ################################################################

void c_itemrecord::init(void)
{
  w_00 = 0;
  bmp = NULL;
  for (i16 i = 0; i < PAL16; i++)
    color_to_palettecolor(&palette[i], E_COL00);
  b_16 = b_17 = 0;
  w_18 = 0;
  weight = 0;
}