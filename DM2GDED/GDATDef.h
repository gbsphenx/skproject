
#pragma once

#define GDFF_LANG_EN_ENC        0x00
#define GDFF_LANG_EN            0x01
#define GDFF_LANG_JP1_ENC       0x02
#define GDFF_LANG_JP2_ENC       0x03
#define GDFF_LANG_MASK          0x07
#define GDFF_LITTLE_ENDIAN      0x08
//#define GDFF_USE_8BPP           0x10
#define GDFF_SND5SND6           0x20
#define GDFF_SND3               0x40
#define GDFF_SND7               0x60
#define GDFF_SNDA               0x80
#define GDFF_SND9               0xA0
#define GDFF_SND_MASK           0xE0
#define GDFF_PAL_IMAGE          0x0000
#define GDFF_PAL_RAW            0x0100
#define GDFF_PAL_SCENE          0x0200
#define GDFF_PAL_MASK           0x0300
#define GDFF_RESTRICT_PAL       0x0400
#define GDFF_DETECTIVE          0x0800
#define GDFF_PICFMT_C4			0x0000
#define GDFF_PICFMT_C4C8		0x1000
#define GDFF_PICFMT_SEGA		0x2000
#define GDFF_PICFMT_FMTOWNS		0x4000
#define GDFF_PICFMT_MASK		0x7000

#define GDRIDT_NO	0
#define GDRIDT_C4	1
#define GDRIDT_C8	2
#define GDRIDT_U4	3
#define GDRIDT_U8	4

#define GDRTDT_EN	0
#define GDRTDT_JP	1

#define GDRSLT_I_EN			(0x01)
#define GDRSLT_I_EN_PLAIN	(0x02)
#define GDRSLT_I_JP			(0x04)
#define GDRSLT_I_FR			(0x08)
#define GDRSLT_I_DE			(0x10)
#define GDRSLT_C_EN			(0x20)
#define GDRSLT_C_EN_PLAIN	(0x40)

#define GDRSDT_NO       0
#define GDRSDT_SND5SND6 1
#define GDRSDT_SND3     2
#define GDRSDT_SND7     3
#define GDRSDT_SNDA     4
#define GDRSDT_SND9     5

#define GDRIPT_IMAGE    0
#define GDRIPT_RAW      1
#define GDRIPT_SCENE    2

#define GDRPIC_C4		0
#define GDRPIC_C4C8		1
#define GDRPIC_SEGA		2
#define GDRPIC_FMTOWNS	3