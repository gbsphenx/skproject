#include "addon.h"
#include "src/c_mouse.h"
#include "src/c_timer.h"
#include "c_allegro.h"
#include "driver.h"
#include "dm2global.h" // video-semaphores
#include "src/fileio.h" // SYS_...

extern struct ALLEGRO_COLOR mypal[256];

#define DMWIDTH    (320)
#define DMHEIGHT   (200)
#define MOUSEWIDTH  (24)
#define MOUSEHEIGHT (30)   // TODO: not confirmed yet
#define NUM_OF_PAL_COLORS (256)
#define BLITMASK (0xff)
#define NOMASK (0xff)
#define KBD_QUEUE_LENGTH (10)
#define MOUSE_QUEUE_LENGTH (10)
#define BPP (1)  // byte per pixel
#define NUM_RECTS (0x246)
#define STRETCH_DELTAX (0x67)
#define STRETCH_START (0x3200)
#define STRETCH_END (0xdc00)
// #define MELEN sizeof(s_mouseentry) //BUGFIX 5.0/2014

bool iflag = false; // TODO - cli/sti prepared, but flag not requested

void cli(void) { iflag = false; }
void sti(void) { iflag = true; }
#define STI  // sti();
#define CLI  // cli();

#define driver_blit_hline_stretched(a, b) driver_stretched_hblit(((a >> 7) + 8) * 0xe0 + 0x15, b) // shr used

// ########
// forwards
// ########

  void driver_blit_hline(ui16 sofs, ui16 dofs, ui16 numpixels);
  void driver_blit_hline_masked(ui16 sofs, ui16 dofs, ui16 pixel, ui8 mask);
  void driver_blit(i8* srcmap, i8* destmap, c_rect* rect, i16 sx, i16 sy, i16 sw, i16 dw, i8 masked, i8 mask);
  void driver_vsync(void);
  void driver_showscreenrectdisabled(c_rect* r);
  void driver_setcolors(void);
  void driver_convertalphapalette(i8* p);
  void driver_setpalette(i8 mode);
  void driver_blitfromscreen(c_rect* rect, i16 yofs);
  void driver_restoremousebackground(void);
  i16 sub_min(i16 a, i16 b);
  i16 sub_max(i16 a, i16 b);
  void driver_drawmouse(i8* srcmap, c_rect* rect, i16 x, i16 y, i16 w, i8 masked, i8 mask, bool restorebackground);
  void driver_stretched_hblit(ui16 srcx, ui16 destx);
  void driver_mouseblit(i8* srcmap, c_rect* rect, i16 stretched);
  void driver_memcpy(i8* src, i8* dest, i16 bytes);
  i16 driver_kbdqueue_pop(void);
  bool driver_forceint2d(void);
  i16 driver_requestkeycode(i16 arg);
  void driver_drawmouse_clipped(bool restorebackground);
  void driver_force_restoremousebkg(void);
  void driver_execmouseentry_T1(i16 mx, i16 my, i16 mb);
  void driver_blockmouseinput(void);
  void driver_execmousefifo(void);
  void driver_setmousecaptured(void);
  void driver_setmouse(void);
  void driver_init_clickhandler(void);
  void driver_showmouse(void);
  void driver_hidemouse(void);
  void driver_setcursorparams(i16 n, i8* srcmap, i16 hx, i16 hy, ui16 w, ui16 h, i16 mode, i8* cmap, ui8 midx);
  void driver_setcindex(i16 idx);
  void driver_setrect2(c_rect* rect, i16 v);
  void driver_copymouseentry(i16* ptrx, i16* ptry, i16* ptrb, i16 source);
  void driver_getfilename(i8 arg, i8* str);
  i16 driver_execmouseclick(i16 x, i16 y, i16 b);

// ##########################################################
// driver variables (sequence of appearance in original kept)
// ##########################################################

i8* DMSCREEN; // additional. a pointer to the original DM2 - screen

// commented ones surely not needed anymore:

// i8  DRVB_0135_0178;
// i8  DRVB_0135_0189;
// i16 DRVW_0135_01d9;
// i16 DRVW_0135_05a0;
// i16 DRVW_0135_05a2;
// i16 DRVW_0135_05a4;
// i16 DRVW_0135_05a6;
// i16 DRVW_0135_05a8;
// i16 DRVW_0135_05aa;
// i16 DRVW_0135_0aa6;
// i16 DRVW_01f1_0e4d;
// i16 DRVW_02d6_0458;
// i16 DRVW_02d6_045a;
// i16 DRVW_02d6_095b;
// i16 DRVW_02d6_095d;
// i16 DRVW_02d6_095f;
// i8  DRVB_mouseentrycounter; // DRVB_02d6_0961;

bool DRVb_nomouse; // DRVW_02d6_0962;

// i16 DRVW_02d6_1258;
// i16 DRVW_02d6_125a;
// i16 DRVW_02d6_125c;
// i16 DRVW_02d6_125e;
// i16 DRVW_02d6_1260;
// i8  DRVB_staticsubcounter; // 2d6:1262 rearranged to be static local
// i32 DRVP_02d6_1501;
// i16 DRVW_02d6_2361;
// i16 DRVW_02d6_27e8;

bool DRVb_immediate_colors; // ADR511B(0xfc)
// if true, driver_convertalphapalette will call driver_setcolors as well (colors available then)
// driver_setpalette will set this byte by the given argument.

bool DRVb_mouseonscreen; // ADR511W(0xfd)
// if true, mouse background needs to be restored in screen

i16 DRVW_timer_disabled; // ADR511W(0x430)
// needs to be 0 for the timer to tick. if not 0, 'driver_forceint2d' is able to queue_int2d_T1
// value in fact never changes
// set by driver_init only

i16 DRVW_int2d_enabled; // ADR511W(0x432)
// if 0 queue_int2d_T1 cannot be called
// set by driver_init only

i8 DRVB_table1[0x80] = {
0x00, 0x1b, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x2d, 0x3d, 0x08, 0x09,
0x71, 0x77, 0x65, 0x72, 0x74, 0x79, 0x75, 0x69, 0x6f, 0x70, 0x5b, 0x5d, 0x0d, 0x00, 0x61, 0x73,
0x64, 0x66, 0x67, 0x68, 0x6a, 0x6b, 0x6c, 0x3b, 0x27, 0x60, 0x00, 0x5c, 0x7a, 0x78, 0x63, 0x76,
0x62, 0x6e, 0x6d, 0x2c, 0x2e, 0x2f, 0x00, 0x2a, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x38, 0x39, 0x2d, 0x34, 0x35, 0x36, 0x2b, 0x31,
0x32, 0x33, 0x30, 0x2e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
}; // 0x46e

i8 DRVB_table2[0x80] = {
0x00, 0x1b, 0x21, 0x40, 0x23, 0x24, 0x25, 0x5e, 0x26, 0x2a, 0x28, 0x29, 0x5f, 0x2b, 0x08, 0x09,
0x51, 0x57, 0x45, 0x52, 0x54, 0x59, 0x55, 0x49, 0x4f, 0x50, 0x7b, 0x7d, 0x0d, 0x00, 0x41, 0x53,
0x44, 0x46, 0x47, 0x48, 0x4a, 0x4b, 0x4c, 0x3a, 0x22, 0x7e, 0x00, 0x7c, 0x5a, 0x58, 0x43, 0x56,
0x42, 0x4e, 0x4d, 0x3c, 0x3e, 0x3f, 0x00, 0x2a, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x38, 0x39, 0x2d, 0x34, 0x35, 0x36, 0x2b, 0x31,
0x32, 0x33, 0x30, 0x2e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
}; // 0x4ee

i16 DRVW_entry_mx; // ADR511W(0x56e)
i16 DRVW_entry_my; // ADR511W(0x570)
i16 DRVW_show_mx; // ADR511W(0x572) x the moment showmouse was triggered
i16 DRVW_show_my; // ADR511W(0x574) y the moment showmouse was triggered
i16 DRVW_clickhandler_installed; // ADR511W(0x576)
//i16 DRVW_capt_mx; // ADR511W(0x578) x the moment the mouse was captured
//i16 DRVW_capt_my; // ADR511W(0x57a) y the moment the mouse was captured

i8 DRVS_findfirststr[7] = { 0xff, 0, 0, 0, 0, 0, 0 }; // ADR511B(0x586)

i8 DRVB_findfirstarg; // ADR511B(0x58d)

i8 DRVB_unknown[0x90] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x00, 0x11, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x13, 0x00
}; // adress referenced: ADR511W(0x5b2) type?

bool DRVb_func0done; // ADR511W(0x642)
// value changes from false to true after a call of driver function 0
// no reset anymore...

s_transferbitmap* DRVP_transfermapptr; // ADR511W(0x644), segment at 0x646. there should be a real pointer now (init: FIVEELEVEN:0x2cae) DRVW_dpmiptr points on that

i8 DRVB_memmousemask; // ADR511W(0x904)
i8 DRVB_memmousemasked; // ADR511W(0x905), set, but not requested

i8 DRVB_mousebackground[0x240];
extern i8 DRVB_startupmousebackground[0x240];

c_rect DRVR_mouserect; // ADR511W(0xb48)

extern i8 DMPAL[3 * 256]; // 0xb50 palette

i8 DRVB_mousepaintmap1[0x800]; // ADR511W(0xe50) ++
i8* DRVP_srcmapptr; // 511:1750, 511:1752
i8 DRVB_mousepaintmap2[0x194]; // ADR511W(0x1754) ++
i8* DRVP_destmapptr; // 511:1894, 511:1896

i16 DRVW_cursorw; // ADR511W(0x18e8)

i16 DRVW_drvinterface_out[0x80]; // ADR511W(0x18ea) word[0x80] - target of function 0 memcpy dpmiinterface

i16 DRVW_mousequeueindex_in; // ADR511W(0x19f2)
i16 DRVW_mousecapturedcounter; // ADR511W(0x19f4)
i16 DRVW_blockmouseinputcounter; // ADR511W(0x1a1e)

i16 DRVW_mouseb_in; // ADR511W(0x1a20)

extern s_cursor DRV_cursor[4]; // ADR511W(0x1a24)

i16 DRVW_mousequeueindex_out; // ADR511W(0x2340)
i16 DRVW_drvinterface[0x200]; // ADR511W(0x2342) word[0x80] - this location also stored in 0x1e1490

i16 DRVW_mousevisicnt; // ADR511W(0x2b46)
i16 DRVW_mousequeuecounter; // ADR511W(0x2b48)

s_mouseentry2 DRV_mouseentry; // ADR511W(0x2b4a),ADR511W(0x2b4c),ADR511W(0x2b4e)

s_xmouserect DRVR_rect1; // ADR511W(0x2bca)

i16 DRVW_mousebkgneedsrestorecounter; // ADR511W(0x2c4c)

s_mouseentry DRV_mousequeue[MOUSE_QUEUE_LENGTH]; // ADR511W(0x2c50)

i16 DRVW_entry_xmb; // ADR511W(0x2c98)
i16 DRVW_entry_mb; // ADR511W(0x2c9a)

bool DRVb_use_rect2; // ADR511W(0x2c9c)

c_rect DRVR_rect2; // ADR511W(0x2c9e)

i16 DRVW_kbdqueueindex_in; // ADR511W(0x2ca6)

i16 DRVW_deltax; // ADR511W(0x2caa)
i16 DRVW_deltay; // ADR511W(0x2cac)
s_transferbitmap DRV_transfermap; // 0x2cae

i16 DRVW_kbdqueueindex_out; // ADR511W(0xa9b0)
i16 DRVW_kbdqueuecounter; // ADR511W(0xa9b2)
i16 DRVW_kbdqueue[KBD_QUEUE_LENGTH]; // ADR511W(0xa9b4)

i16 DRVW_ndeltax; // ADR511W(0xa9cc)
i16 DRVW_ndeltay; // ADR511W(0xa9ce)
bool DRVb_enablesetmouse; // ADR511W(0xa9d4)
i16 DRVW_oldmb; // ADR511W(0xa9d6)
i16 DRVW_cursorh; // ADR511W(0xa9d8)
i16 DRVW_cindex; // ADR511W(0xa9da)

// Parameter: where to draw the original DM - screen
void driver_init(i8* gfxout)
{
  DMSCREEN = gfxout;

// mov8(DRVB_0135_0178, getind(ONETFIVE, 0x178));
// mov8(DRVB_0135_0189, getind(ONETFIVE, 0x189));
// mov16(DRVW_0135_01d9, getind(ONETFIVE, 0x1d9));
// mov16(DRVW_0135_05a0, getind(ONETFIVE, 0x5a0));
// mov16(DRVW_0135_05a2, getind(ONETFIVE, 0x5a2));
// mov16(DRVW_0135_05a4, getind(ONETFIVE, 0x5a4));
// mov16(DRVW_0135_05a6, getind(ONETFIVE, 0x5a6));
// mov16(DRVW_0135_05a8, getind(ONETFIVE, 0x5a8));
// mov16(DRVW_0135_05aa, getind(ONETFIVE, 0x5aa));
// mov16(DRVW_0135_0aa6, getind(ONETFIVE, 0xaa6));
// mov16(DRVW_01f1_0e4d, getind(ONEFONE, 0xe4d)); // value: FIVEELEVEN
// mov16(DRVW_02d6_0458, getind(TWODSIX, 0x458));
// mov16(DRVW_02d6_045a, getind(TWODSIX, 0x45a));
// mov16(DRVW_02d6_095b, getind(TWODSIX, 0x95b));
// mov16(DRVW_02d6_095d, getind(TWODSIX, 0x95d));
// mov16(DRVW_02d6_095f, getind(TWODSIX, 0x95f));
// DRVB_mouseentrycounter = word_at(0x961, TWODSIX); // can be made static

 DRVb_nomouse = false; // word_at(0x962, TWODSIX);

// mov16(DRVW_02d6_1258, getind(TWODSIX, 0x1258));
// mov16(DRVW_02d6_125a, getind(TWODSIX, 0x125a));
// mov16(DRVW_02d6_125c, getind(TWODSIX, 0x125c));
// mov16(DRVW_02d6_125e, getind(TWODSIX, 0x125e));
// mov16(DRVW_02d6_1260, getind(TWODSIX, 0x1260));
// mov8(DRVB_staticsubcounter, getind(TWODSIX, 0x1262)); // init not really necessary
// mov32(DRVP_02d6_1501, getind(TWODSIX, 0x1501));
// mov16(DRVW_02d6_2361, getind(TWODSIX, 0x2361)); // value: FIVEELEVEN
// mov16(DRVW_02d6_27e8, getind(TWODSIX, 0x27e8));

 DRVb_immediate_colors = true; // byte_at(ADR511B(0xfc)) != con(0x0);
 DRVb_mouseonscreen = true; // word_at(ADR511W(0xfd)) != con(0x0);

 DRVW_timer_disabled = 0; // word_at(ADR511W(0x430)); TODO bool
 DRVW_int2d_enabled = 1; // word_at(ADR511W(0x432)); TODO bool

// memcpy(DRVB_table1, ADR511W(0x46e), sizeof(DRVB_table1));
// memcpy(DRVB_table2, ADR511W(0x4ee), sizeof(DRVB_table2));

 DRVW_entry_mx = 0; // word_at(ADR511W(0x56e));
 DRVW_entry_my = 0; // word_at(ADR511W(0x570));
 DRVW_show_mx = 0; // word_at(ADR511W(0x572));
 DRVW_show_my = 0; // word_at(ADR511W(0x574));
 DRVW_clickhandler_installed = 1; // word_at(ADR511W(0x576)); TODO bool
// DRVW_capt_mx = 0; // word_at(ADR511W(0x578)); // set, but never requested
// DRVW_capt_my = 0; // word_at(ADR511W(0x57a)); // set, but never requested

// DRVP_findfirststr = ADR511B(0x586);
// DRVB_findfirstarg = byte_at(ADR511W(0x58d)); // set, but never requested;

// memcpy(DRVB_unknown, ADR511B(0x5b2), sizeof(DRVB_unknown));
 memset(DRVB_unknown, sizeof(DRVB_unknown), 0);

 DRVb_func0done = false; // word_at(ADR511W(0x642));
 DRVP_transfermapptr = &DRV_transfermap; // ADR511W(0x644) initial entry: FIVEELEVEN:2cae

 DRVB_memmousemask = 0xf6; // byte_at(ADR511B(0x904));
 DRVB_memmousemasked = 0; // byte_at(ADR511B(0x905));

// memset(DRVB_mousebackground, sizeof(DRVB_mousebackground), 0); // ADR511W(0x906)
 memcpy(DRVB_mousebackground, DRVB_startupmousebackground, sizeof(DRVB_mousebackground)); // ADR511W(0x906)

// memcpy(&DRVR_mouserect, ADR511W(0xb48), sizeof(c_rect));
 DRVR_mouserect.x = DRVR_mouserect.y = 0;
 DRVR_mouserect.w = 0xc;
 DRVR_mouserect.h = 0x10;

 memset(DRVB_mousepaintmap1, sizeof(DRVB_mousepaintmap1), 0); // ADR511W(0xe50)

 DRVP_srcmapptr = NULL; // mkptr(word_at(ADR511W(0x1750)), word_at(ADR511W(0x1752)));

 memset(DRVB_mousepaintmap2, sizeof(DRVB_mousepaintmap2), 0); // ADR511W(0x1754)

 DRVP_destmapptr = NULL; // mkptr(word_at(ADR511W(0x1894)), word_at(ADR511W(0x1896)));

 DRVW_cursorw = 0xc; // word_at(ADR511W(0x18e8));

 memset(&DRVW_drvinterface_out, sizeof(DRVW_drvinterface_out), 0); // ADR511W(0x18ea)

 DRVW_mousequeueindex_in = 0; // word_at(ADR511W(0x19f2))
 DRVW_mousecapturedcounter = 0; // word_at(ADR511W(0x19f4));
 DRVW_blockmouseinputcounter = 0; // word_at(ADR511W(0x1a1e));
 DRVW_mouseb_in = 0; // word_at(ADR511W(0x1a20))

// memcpy(DRV_cursor, ADR511W(0x1a24), sizeof(DRV_cursor)); // ADR511W(0x1a24)

 DRVW_mousequeueindex_out = 0; // ADR511W(0x2340)

#if 0
 memcpy(DRVW_drvinterface, ADR511W(0x2342), sizeof(DRVW_drvinterface)); // points to a 256 - bytearray with variable usage, type wordptr mostly used
#else
 memset(DRVW_drvinterface, sizeof(DRVW_drvinterface), 0); // points to a 256 - bytearray with variable usage, type wordptr mostly used
#endif
// in fact SKW_44c8_1baf and SKW_FIRE_MOUSE_EVENT_RECEIVER
// transfer up to 0x400 bytes. pointer 0x1e1490 goes for that!

// words at 0x2b42 and 0x2b44 point to a routine for the clickhandler.
// those are not needed anymore.

 DRVW_mousevisicnt = 0; // ADR511W(0x2b46)
 DRVW_mousequeuecounter = 0; // ADR511W(0x2b48)

// memcpy(&DRV_mouseentry, ADR511W(0x2b4a), sizeof(DRV_mouseentry)); // pointer 0x1e148c points on this!
 memset(&DRV_mouseentry, sizeof(DRV_mouseentry), 0);

// memcpy(&DRVR_rect1, ADR511W(0x2bca), sizeof(DRVR_rect1)); // pointer 0x1e1488 points on this!
 memset(&DRVR_rect1, sizeof(DRVR_rect1), 0);

 DRVW_mousebkgneedsrestorecounter = 0; // ADR511W(0x2c4c)

#if 0
 memcpy(DRV_mousequeue, ADR511W(0x2c50), sizeof(DRV_mousequeue));
#else
 memset(DRV_mousequeue, sizeof(DRV_mousequeue), 0);
#endif

 DRVW_entry_xmb = 0; // word_at(ADR511W(0x2c98));
 DRVW_entry_mb = 0; // word_at(ADR511W(0x2c9a));
 DRVb_use_rect2 = false; // word_at(ADR511W(0x2c9c)) != con(0x0);

// memcpy(&DRVR_rect2, ADR511W(0x2c9e), sizeof(DRVR_rect2));
 memset(&DRVR_rect2, sizeof(DRVR_rect2), 0);

 DRVW_kbdqueueindex_in = 0;
 DRVW_deltax = 0; // word_at(ADR511W(0x2caa));
 DRVW_deltay = 0; // word_at(ADR511W(0x2cac));

#if 0
 memcpy(&DRV_transfermap, ADR511B(0x2cae), sizeof(DRV_transfermap));
#else
 memset(&DRV_transfermap, sizeof(DRV_transfermap), 0);
#endif

 DRVW_kbdqueueindex_out = 0;
 DRVW_kbdqueuecounter = 0;

 memset(DRVW_kbdqueue, sizeof(DRVW_kbdqueue), 0);

 DRVW_ndeltax = 0;
 DRVW_ndeltay = 0;
 DRVb_enablesetmouse = true;
 DRVW_oldmb = 0;
 DRVW_cursorh = 0;
 DRVW_cindex = 0;
}

// blit a piece of a line within given bitmaps
// was sub_2100
void driver_blit_hline(ui16 sofs, ui16 dofs, ui16 numpixels)
{
  driver_memcpy(DRVP_srcmapptr + sofs, DRVP_destmapptr + dofs, numpixels);
}

// blit masked a piece of a line within given bitmaps
// source is DRVP_srcmapptr
// destination is DRVP_destmapptr
// pixel: number of pixels
// mask: that pixel will be skipped in the copy
// was sub_2136
void driver_blit_hline_masked(ui16 sofs, ui16 dofs, ui16 pixel, ui8 mask)
{
  i8* src = DRVP_srcmapptr + sofs;
  i8* dest = DRVP_destmapptr + dofs;

  for (ui16 i=0; i<pixel; i++)
  {
    ui8 p = *src++;
    if (p == mask)
      dest++;
    else
      *dest++ = p;
  }
}

// blit rectangle, remember source and destination - blitmap
// was sub_2169
void driver_blit(i8* srcmap, i8* destmap, c_rect* rect, i16 sx, i16 sy, i16 sw, i16 dw, i8 masked, i8 mask)
{
  DRVP_srcmapptr = srcmap;
  DRVP_destmapptr = destmap;

  if (rect->w <= 0 || rect->h <= con(0x0))
    return;

  if (destmap == DMSCREEN)
  {
    enable_video = false;
    while (!enable_drawing);
  }

  i16 sofs = sy * sw + sx;
  i16 dofs = rect->y * dw + rect->x;

  for (i16 line = 0; line < rect->h; sofs += sw, dofs += dw, line++)
  {
    i16 m = (masked<<8) | (mask & 0xff); // TODO not funny
    if (m >= con(0x0))
      driver_blit_hline_masked(sofs, dofs, rect->w, mask);
    else
      driver_blit_hline(sofs, dofs, rect->w);
  }

  if (destmap == DMSCREEN)
    enable_video = true;
}

// was sub_22EA, sub_vsync - it's for the fade effect -> delayed
void driver_vsync(void)
{
  wait_for_vsync();
}

// was sub_22FC
// grey out a rectangle in screen
void driver_showscreenrectdisabled(c_rect* r)
{
  for (i16 y = r->y; y < r->y + r->h - 1; y++)
  {
    i8* ptr = DMSCREEN + y * DMWIDTH + r->x;

    for (i16 x = r->x; x < r->x + r->w - 1; x++, ptr++)
      if (!((x ^ y) & 0x1))
        *ptr = 0;
  }
}

// was sub_2372
// from DM - palette to screencolors.
// conversion from 6 to 8 bit components
void driver_setcolors(void)
{
  ALLEGRO_COLOR m[NUM_OF_PAL_COLORS];

  i8* ptr = DMPAL;
  for (i16 i = 0; i < NUM_OF_PAL_COLORS; i++)
  {
    m[i].r = *ptr++ * DMFCOL;
    m[i].g = *ptr++ * DMFCOL;
    m[i].b = *ptr++ * DMFCOL;
  }

  for (int k = 0; k<1000; k++) // fade to new palette
  {
    for (i16 i = 0; i < NUM_OF_PAL_COLORS; i++)
    {
      mypal[i].r = m[i].r * k/1000.0;
      mypal[i].g = m[i].g * k/1000.0;
      mypal[i].b = m[i].b * k/1000.0;
    }
    driver_vsync();
  }
}

// copies the selected palette to DMPAL
// DM2 shifts the 8 - bit - colors of an alpha - palette >> 2 to the DM - palette
// was sub_24DC
void driver_convertalphapalette(i8* p)
{
  i8* ptr = DMPAL;

  for (i16 i=0; i < NUM_OF_PAL_COLORS; i++)
  {
    p++; // alphabyte

    ui8 pix = *p++;
    pix >>= 2;
    *ptr++ = pix;

    pix = *p++;
    pix >>= 2;
    *ptr++ = pix;

    pix = *p++;
    pix >>= 2;
    *ptr++ = pix;
  }

  if (DRVb_immediate_colors)
    driver_setcolors();
}

// was sub_2569
void driver_setpalette(i8 mode)
{
  ALLEGRO_COLOR m[NUM_OF_PAL_COLORS];

  if (mode == 0)
  {
    for (int i=0; i<NUM_OF_PAL_COLORS; i++) // backup palette
      m[i] = mypal[i];

    for (int k=1000; k>0; k--) // fade to black
    {
      for (int i=0; i<NUM_OF_PAL_COLORS; i++)
      {
        mypal[i].r = m[i].r * k/1000.0;
        mypal[i].g = m[i].g * k/1000.0;
        mypal[i].b = m[i].b * k/1000.0;
      }
      driver_vsync();
      driver_vsync();
    }
  }
  else if (mode == 1)
    driver_setcolors();

  DRVb_immediate_colors = mode == 1;
}

// was sub_25AF
void driver_blitfromscreen(c_rect* rect, i16 yofs)
{
  for (i16 i = rect->y + yofs; rect->y + rect->h - 1 >= i; i++)
    driver_memcpy(DMSCREEN + i * DMWIDTH, DMSCREEN + (i - yofs) * DMWIDTH, rect->w);
}

// was sub_2654
void driver_restoremousebackground(void)
{
  if (DRVb_mouseonscreen)
  {
    i8* ssaveptr = DRVP_srcmapptr;
    i8* dsaveptr = DRVP_destmapptr;

    driver_blit(DRVB_mousebackground, DMSCREEN, &DRVR_mouserect, 0, 0, MOUSEWIDTH, DMWIDTH, NOMASK, NOMASK);

    DRVP_srcmapptr = ssaveptr;
    DRVP_destmapptr = dsaveptr;

    DRVb_mouseonscreen = false;
  }
}

// was sub_26C1
i16 sub_min(i16 a, i16 b)
{
  return (a < b) ? a : b;
}

// was sub_26D3
i16 sub_max(i16 a, i16 b)
{
  return (a > b) ? a : b;
}

// note: DRVB_mousepaintmap1 used only here
// was sub_26E5
void driver_drawmouse(i8* srcmap, c_rect* rect, i16 x, i16 y, i16 w, i8 masked, i8 mask, bool restorebackground)
{
  c_rect r1;
  c_rect r2;
  c_rect r3;

  i8* ssaveptr = DRVP_srcmapptr;
  i8* dsaveptr = DRVP_destmapptr;

  r2.x = r2.y = 0;
  r3.x = sub_min(DRVR_mouserect.x, rect->x);
  r3.y = sub_min(DRVR_mouserect.y, rect->y);
  r2.w = r3.w = sub_max(DRVR_mouserect.x + DRVR_mouserect.w - 1, rect->x + rect->w - 1) - r3.x + 1;
  r2.h = r3.h = sub_max(DRVR_mouserect.y + DRVR_mouserect.h - 1, rect->y + rect->h - 1) - r3.y + 1;

  if (   !DRVb_mouseonscreen
      || !restorebackground
      || (DRVR_mouserect.w + rect->w) <= r3.w
      || (DRVR_mouserect.h + rect->h) <= r3.h
     )
  {
    //M_28C3:
    if (restorebackground)
      driver_restoremousebackground();
    if (!DRVb_mouseonscreen)
    {
      //M_28D7:
      DRVR_mouserect.x = rect->x;
      DRVR_mouserect.y = rect->y;
      r1.x = r1.y = 0;
      DRVR_mouserect.w = r1.w = rect->w;
      DRVR_mouserect.h = r1.h = rect->h;
      driver_blit(DMSCREEN, DRVB_mousebackground, &r1, rect->x, rect->y, DMWIDTH, MOUSEWIDTH, NOMASK, NOMASK);
      driver_blit(srcmap, DMSCREEN, rect, x, y, w, DMWIDTH, masked, mask);

      DRVB_memmousemask = mask;
      DRVB_memmousemasked = masked;
      DRVb_mouseonscreen = true;
    }
  }
  else
  {
    //M_27BF:
    driver_blit(DMSCREEN, DRVB_mousepaintmap1, &r2, r3.x, r3.y, DMWIDTH, r3.w, NOMASK, NOMASK);

    r1.x = DRVR_mouserect.x - r3.x;
    r1.y = DRVR_mouserect.y - r3.y;
    r1.w = DRVR_mouserect.w;
    r1.h = DRVR_mouserect.h;
    driver_blit(DRVB_mousebackground, DRVB_mousepaintmap1, &r1, 0, 0, MOUSEWIDTH, r3.w, NOMASK, NOMASK);

    DRVR_mouserect.x = rect->x;
    DRVR_mouserect.y = rect->y;
    r1.x = r1.y = 0;
    DRVR_mouserect.w = r1.w = rect->w;
    DRVR_mouserect.h = r1.h = rect->h;
    i16 xd = rect->x - r3.x;
    i16 yd = rect->y - r3.y;
    driver_blit(DRVB_mousepaintmap1, DRVB_mousebackground, &r1, xd, yd, r3.w, MOUSEWIDTH, NOMASK, NOMASK);

    r1.x = xd;
    r1.y = yd;
    driver_blit(srcmap, DRVB_mousepaintmap1, &r1, x, y, w, r3.w, masked, mask);
    driver_blit(DRVB_mousepaintmap1, DMSCREEN, &r3, 0, 0, r3.w, DMWIDTH, NOMASK, NOMASK);
    DRVB_memmousemask = mask;
    DRVB_memmousemasked = masked;
    DRVb_mouseonscreen = true;
  }

//M_295B:
  DRVP_srcmapptr = ssaveptr;
  DRVP_destmapptr = dsaveptr;
}

// was sub_297A
// TODO when is this used?
// does 14 times this stretching:
// src:  abc defgh ijkl m  13 pixel
// dest: abccdefghhijkllm  16 pixel
void driver_stretched_hblit(ui16 srcx, ui16 destx)
{
  i8* dssi = DRVP_srcmapptr + srcx;
  i8* esdi = DRVP_destmapptr + destx;

  for (int c=0; c<14; c++)
  {
    *esdi++ = *dssi++;
    *esdi++ = *dssi++;
    i8 p = *dssi++;
    *esdi++ = p;
    *esdi++ = p;
    *esdi++ = *dssi++;
    *esdi++ = *dssi++;
    *esdi++ = *dssi++;
    *esdi++ = *dssi++;
    p = *dssi++;
    *esdi++ = p;
    *esdi++ = p;
    *esdi++ = *dssi++;
    *esdi++ = *dssi++;
    *esdi++ = *dssi++;
    p = *dssi++;
    *esdi++ = p;
    *esdi++ = p;
    *esdi++ = *dssi++;
  }
}

// was sub_29AD
// stretch - effect if MSB of 'stretched' is set
// DRVB_mousepaintmap2 used only here
void driver_mouseblit(i8* srcmap, c_rect* rect, i16 stretched)
{
  i16 destxofs; // was RG5W
  i16 strofs; // was RG6W

  i16 v_x; // was v06
  i16 v_y; // was v08
  i16 v0a;
//  i16 v0c;  unused
  i16 v0e;
  ui16 v10;
  i16 v_dx; // was v12
  i16 v16;

  bool stretch = (stretched & 0x8000) != con(0x0);

  driver_blockmouseinput();

  if (   DRVb_mouseonscreen
      && rect->y <= DRVR_mouserect.y + DRVR_mouserect.h - 1
      && rect->y + rect->h - 1 >= DRVR_mouserect.y
      && rect->x + rect->w - 1 >= DRVR_mouserect.x
      && DRVR_mouserect.x + DRVR_mouserect.w - 1 >= rect->x
     )
  {
    v_y = (rect->y >= DRVR_mouserect.y) ? rect->y : DRVR_mouserect.y;
    v_x = (rect->x >= DRVR_mouserect.x) ? rect->x : DRVR_mouserect.x;

    //M_2AC7:
    if (rect->x + rect->w - 1 <= DRVR_mouserect.x + DRVR_mouserect.w - 1)
      v0a = rect->x + rect->w - 1 - v_x + 1;
    else
      v0a = DRVR_mouserect.x + DRVR_mouserect.w - 1 - v_x + 1;

    //M_2AFD:
    v_dx = v_x - rect->x;
    v_y -= DRVR_mouserect.y;
    v_x -= DRVR_mouserect.x;

    destxofs = rect->y * DMWIDTH + rect->x;
    v16 = 0;
    strofs = 0;
    v10 = UI16CAST(DRV_cursor[DRVW_cindex].w); // byte to ui16
    v0e = (DRVR_mouserect.x == 0) ? v10 - v0a : 0;

    enable_video = false; // to screen for sure here
    while (!enable_drawing);

    //M_2CBB:
    // TODO: here was the viewport-height problem.
    // check against original, had to change from < to <=
    for (ui16 v14 = rect->y; v14 <= rect->y + rect->h - 1; v14++) // unsigned
    {
      //M_2B66:
      if (v14 < DRVR_mouserect.y || DRVR_mouserect.y + DRVR_mouserect.h - 1 < v14) // unsigned
      {
        //M_2C5C:
        DRVP_srcmapptr = srcmap;
        DRVP_destmapptr = DMSCREEN;
        if (stretch)
          driver_blit_hline_stretched(strofs, destxofs);
        else
          driver_blit_hline(v16, destxofs, rect->w);
      }
      else
      {
        //M_2B82:
        DRVP_srcmapptr = srcmap;
        DRVP_destmapptr = DRVB_mousepaintmap2;
        if (stretch)
          driver_blit_hline_stretched(strofs, 0);
        else
          driver_blit_hline(v16, 0, rect->w);

        //M_2BCD:
        DRVP_srcmapptr = DRVB_mousepaintmap2;
        DRVP_destmapptr = DRVB_mousebackground;
        driver_blit_hline(DRVR_mouserect.x, (v14 - DRVR_mouserect.y) * MOUSEWIDTH + v_x, v0a);

        DRVP_srcmapptr = DRV_cursor[DRVW_cindex].v;
        DRVP_destmapptr = DRVB_mousepaintmap2;
        driver_blit_hline_masked((v14 - DRVR_mouserect.y) * v10 + v0e, v_dx, v0a, DRVB_memmousemask);

        DRVP_srcmapptr = DRVB_mousepaintmap2;
        DRVP_destmapptr = DMSCREEN;
        driver_blit_hline(0, destxofs, rect->w);
      }
      //M_2CA7:
      strofs += STRETCH_DELTAX;
      v16 += rect->w;
      destxofs += DMWIDTH;
    }

    enable_video = true;
  }
  else
  {
    //M_2A1B:
    if (stretch)
    {
      enable_video = false; // to screen for sure here
      while (!enable_drawing);

      DRVP_destmapptr = DMSCREEN;
      DRVP_srcmapptr = srcmap;

      //M_2A60:
      for (ui16 s = STRETCH_START, d = 0; s < STRETCH_END; s += DMWIDTH, d += STRETCH_DELTAX)
        //M_2A41:
        driver_blit_hline_stretched(d, s);

      enable_video = true;
    }
    else
      driver_blit(srcmap, DMSCREEN, rect, 0, 0, rect->w, DMWIDTH, NOMASK, NOMASK);
  }

  driver_execmousefifo();
}

// was sub_2D6D
// using library - memcpy now
void driver_memcpy(i8* src, i8* dest, i16 bytes)
{
  memcpy(dest, src, bytes);
}

// was sub_31DC
// was calling sub_2E61 which called sub_2E33.
// both are obsolete now, code direct implemented here.
void driver_keyboardint_T1(i16 keycode)
{
  CLI
  if (DRVW_kbdqueuecounter < KBD_QUEUE_LENGTH)
  {
    DRVW_kbdqueue[DRVW_kbdqueueindex_in] = keycode;
    DRVW_kbdqueueindex_in = (DRVW_kbdqueueindex_in + 1) % KBD_QUEUE_LENGTH;
    DRVW_kbdqueuecounter++;
  }
}

// was sub_3249
i16 driver_kbdqueue_pop(void)
{
// TODO while (DRVW_kbdqueuecounter == 0) {}
  CLI
  i16 s = DRVW_kbdqueue[DRVW_kbdqueueindex_out];
  DRVW_kbdqueueindex_out = (DRVW_kbdqueueindex_out + 1) % KBD_QUEUE_LENGTH;
  DRVW_kbdqueuecounter--;
  return s;
}

// was sub_327F
// returns if a key is available in the queue
bool driver_forceint2d(void)
{
  if (DRVW_timer_disabled && DRVW_int2d_enabled)
    queue_int2d_T1();
  return (DRVW_kbdqueuecounter != 0);
}

// was sub_32AF
i16 driver_requestkeycode(i16 arg)
{
  if ((arg & 0x1000) != 0)
    arg = 0;

  i8 a = ((arg & 0x200) == 0) ? DRVB_table1[arg & 0x7f] : DRVB_table2[arg & 0x7f];
  ui16 r = UI16CAST(a);

  if (r != 0 && (arg & 0x400) != 0)
    r &= 0x1f;
  return r;
}

// was sub_3313
// can be called by both threads
void driver_drawmouse_clipped(bool restorebackground)
{
  static bool semaphore = false;
  while (semaphore);
  semaphore = true;

  c_rect r;
  s_cursor* cursor;

  CLI
  if (DRVW_mousebkgneedsrestorecounter != 0)
  {
    //M_3328:
    if (restorebackground)
      DRVW_mousebkgneedsrestorecounter--;
  }
  else
    restorebackground = false;

  //M_3330:
  if (DRVb_use_rect2 && DRVW_clickhandler_installed)
  {
    if (   DRVW_entry_mx < DRVR_rect2.x
        || DRVR_rect2.x + DRVR_rect2.w - 1 < DRVW_entry_mx
        || DRVW_entry_my < DRVR_rect2.y
        || DRVR_rect2.y + DRVR_rect2.h - 1 < DRVW_entry_my
       )
    {
      //M_336C:
      DRVb_use_rect2 = 0;
      i16 a = driver_execmouseclick(DRVW_entry_mx, DRVW_entry_my, DRVW_entry_xmb);
      DRVW_mouseb_in = a;
      if ((a & 0x8000) == 0)
        DRVW_cindex = a;
    }
  }

  //M_3393:
  cursor = &(DRV_cursor[DRVW_cindex]);

  DRVW_deltax = DRVW_entry_mx - UI16CAST(cursor->hx);
  DRVW_deltay = DRVW_entry_my - UI16CAST(cursor->hy);

  DRVW_cursorw = UI16CAST(cursor->w);
  DRVW_cursorh = UI16CAST(cursor->h);

  DRVW_ndeltax = 0;
  DRVW_ndeltay = 0;

  if (DRVW_deltax < con(0x0))
  {
    DRVW_ndeltax = -DRVW_deltax;
    DRVW_deltax = 0;
  }

  if (DRVW_deltay < con(0x0))
  {
    DRVW_ndeltay = -DRVW_deltay;
    DRVW_deltay = 0;
  }

  if (DRVW_deltax < DMWIDTH && DRVW_deltay < DMHEIGHT)
  {
    CLI
    if (DRVW_mousebkgneedsrestorecounter == 0)
    {
      DRVW_mousebkgneedsrestorecounter++;

      r.x = DRVW_deltax;
      r.w = DRVW_cursorw - DRVW_ndeltax;

      if (r.x + r.w - 1 > DMWIDTH - 1)
        r.w -= r.x + r.w - (DMWIDTH - 1);

      r.y = DRVW_deltay;
      r.h = DRVW_cursorh - DRVW_ndeltay;

      if (r.y + r.h - 1 > DMHEIGHT - 1)
        r.h -= r.y + r.h - (DMHEIGHT - 1);

      driver_drawmouse(cursor->v, &r, DRVW_ndeltax, DRVW_ndeltay, DRVW_cursorw, 0, cursor->v4, restorebackground);
      restorebackground = false;
    }
  }

  if (restorebackground)
    driver_force_restoremousebkg();

  semaphore = false;
}

// was sub_34CC
void driver_force_restoremousebkg(void)
{
  if (DRVW_mousebkgneedsrestorecounter != 0)
  {
    driver_restoremousebackground();
    DRVW_mousebkgneedsrestorecounter--;
  }
}

// was sub_34E1
void driver_execmouseentry_T1(i16 mx, i16 my, i16 mb)
{
  DRVW_entry_mx = mx;
  DRVW_entry_my = my;
  if ((mx != DRVW_show_mx || my != DRVW_show_my) && DRVW_mousevisicnt <= con(0x0))
  {
    driver_drawmouse_clipped(true);
    DRVW_show_mx = mx;
    DRVW_show_my = my;
  }
  DRVW_entry_mb = ((mb >> 1) & 0x1) | ((mb << 1) & 0x2); // sar
  DRVW_mouseb_in = mb ^ DRVW_oldmb;
  if (DRVW_mouseb_in != 0 && DRVW_clickhandler_installed)
  {
    if ((DRVW_mouseb_in & 0x1) != 0)
      driver_execmouseclick(mx, my, ((mb & 0x1) == 0) ? 4 : 2);
    if ((DRVW_mouseb_in & 0x2) != 0)
      driver_execmouseclick(mx, my, ((mb & 0x2) == 0) ? 8 : 1);
    DRVW_oldmb = mb;
  }
}

// was sub_3586
void driver_blockmouseinput(void)
{
  CLI
   DRVW_blockmouseinputcounter++;
}

// was sub_3586
void driver_execmousefifo(void)
{
  CLI
  while (DRVW_mousequeuecounter > con(0x0))
  {
    DRVW_mousequeuecounter--;
    DRVW_mousequeueindex_out = (DRVW_mousequeueindex_out + 1) % MOUSE_QUEUE_LENGTH;
    driver_execmouseentry_T1(DRV_mousequeue[DRVW_mousequeueindex_out].x, DRV_mousequeue[DRVW_mousequeueindex_out].y, DRV_mousequeue[DRVW_mousequeueindex_out].b);
    CLI
  }
  DRVW_blockmouseinputcounter--;
}

// was sub_3608
void driver_setmousecaptured(void)
{
  CLI
  DRVW_mousecapturedcounter++;
}

// was sub_361B
// releases capture - mode
void driver_setmouse(void)
{
  i16 x;

  CLI
  if (DRVb_enablesetmouse)
  {
    x = DRVW_entry_mx;
#if 0
$   x <<= 1;
#endif
//    alg.set_mouse(x, DRVW_entry_my);
      // TODO: no idea why those offsets are needed
      // hotspot of the real mouse ?!?
      alg.set_mouse(x + 5, DRVW_entry_my + 15);
  }
  DRVW_mousecapturedcounter--;
}

void driver_init_clickhandler(void)
{
  CLI
#if 0
// TODO: that's segment/offset - should be a pointer
// the routine where it points to is already installed
// we don't need this anymore.
  mov16(ADR511W(con(0x2b42)), con(0x15db));
  mov16(ADR511W(con(0x2b44)), con(TWODSIX));
#endif
  DRVW_clickhandler_installed = 1;
}

// was sub_3714
// buttons in RG2W, x in RG3W, y in RG4W
void driver_mouseint_T1(i16 b, i16 x, i16 y)
{
  static i8 DRVB_mouseentrycounter = 0; // DRVB_02d6_0961

  if (DRVb_nomouse)
    return;

#if 0
$ x >>= 1;
#endif
  b &= 0x3;
  if (DRVW_mousecapturedcounter != 0)
  {
    x = DRVW_entry_mx;
    y = DRVW_entry_my;
  }

  CLI
  if (DRVW_blockmouseinputcounter == 0 && DRVB_mouseentrycounter == 0)
  {
    DRVB_mouseentrycounter++;
    driver_execmouseentry_T1(x, y, b);
    DRVB_mouseentrycounter--;
    return;
  }

  if (DRVW_mousequeuecounter >= MOUSE_QUEUE_LENGTH)
    return;

  if (DRVW_mousequeuecounter != 0)
  {
    i16 n = DRVW_mousequeueindex_in; // BUGFIX 5.0/2014
    if (DRV_mousequeue[n].b == b)
    {
      DRV_mousequeue[n].x = x;
      DRV_mousequeue[n].y = y;
      return;
    }
  }

  DRVW_mousequeuecounter++;

  i16 n = DRVW_mousequeueindex_in + 1;
  if (n == MOUSE_QUEUE_LENGTH)
    n = 0;
  DRVW_mousequeueindex_in = n;

  // n *= MELEN; NO! BUGFIX 5.0/2014
  DRV_mousequeue[n].b = b;
  DRV_mousequeue[n].x = x;
  DRV_mousequeue[n].y = y;
}

// was sub_3847
void driver_showmouse(void)
{
  CLI
  if (DRVW_mousevisicnt-- == 1) // block executed 1->0 - triggered
  {
    driver_blockmouseinput();
    driver_drawmouse_clipped(false);
    DRVW_show_mx = DRVW_entry_mx;
    DRVW_show_my = DRVW_entry_my;
    driver_execmousefifo();
  }
}

// was sub_387D
void driver_hidemouse(void)
{
  CLI
  if (DRVW_mousevisicnt++ == 0) // block executed 0->1 - triggered
  {
    driver_blockmouseinput();
    driver_force_restoremousebkg();
    driver_execmousefifo();
  }
}

// was sub_38A3
void driver_setcursorparams(i16 n, i8* srcmap, i16 hx, i16 hy, ui16 w, ui16 h, i16 mode, i8* cmap, ui8 midx)
{
  ui8 a;

  CLI
  if (n < 0 || n >= 4)
    return;

  driver_blockmouseinput();
  s_cursor* c = &DRV_cursor[n]; // was v06

  if (mode == 4)
  {
    i16 v04 = (w + 1) & 0xfffe;

    for (ui16 y = 0, idx = 0; y < h; y++) // y was v0a, idx was RG6L
      for (ui16 x = 0; x < w; x++, idx++) // x was v0c
      {
        i16 is = v04 * y + x;
        a = srcmap[is >> 1]; // sar used
        if ((is & 0x1) == 0)
          a >>= 4; // sar used
        else
          a &= 0xf;
        c->v[idx] = cmap[a];
      }
    a = cmap[midx];
  }
  else
  {
    for (ui16 s = 0; s <= w * h; s++)
      c->v[s] = srcmap[s];
    a = midx;
  }

  c->hx = I8CAST(hx);
  c->hy = I8CAST(hy);
  c->w = I8CAST(w);
  c->h = I8CAST(h);
  c->v4 = a;
  driver_execmousefifo();
}

// was sub_39BA
void driver_setcindex(i16 idx)
{
  CLI
  driver_blockmouseinput();
  DRVW_cindex = idx;
  if (DRVW_mousebkgneedsrestorecounter != 0)
    driver_drawmouse_clipped(true);
  driver_execmousefifo();
}

// was sub_39ED
void driver_setrect2(c_rect* rect, i16 v)
{
  CLI
  driver_blockmouseinput();
  DRVb_use_rect2 = false;

  DRVR_rect2.x = rect->x;
  DRVR_rect2.y = rect->y;
  DRVR_rect2.w = rect->w;
  DRVR_rect2.h = rect->h;

  DRVW_entry_xmb = v;
  DRVb_use_rect2 = true;
  driver_execmousefifo();
}

// was sub_3A81
void driver_copymouseentry(i16* ptrx, i16* ptry, i16* ptrb, i16 source)
{
  CLI
  if (source)
  {
    *ptrx = DRVW_deltax;
    *ptry = DRVW_deltay;
    *ptrb = DRVW_cursorh; // TODO: ooops ?!?
  }
  else
  {
    *ptrx = DRVW_entry_mx;
    *ptry = DRVW_entry_my;
    *ptrb = DRVW_entry_mb;
  }
}

// was sub_3FC3
void driver_timerint_T1(void)
{
  static ui8 DRVB_staticsubcounter = 0;

  if (!DRVW_timer_disabled && DRVW_int2d_enabled)
  {
    DRVB_staticsubcounter++;
    if ((DRVB_staticsubcounter & 0x3) == 0) // each 4th tick no call
      queue_int2d_T1();
  }

#if 0 // original programs the timer here with a one shot trigger
  DRVW_02d6_125c -= DRVW_02d6_1260;

  if (DRVW_02d6_125e != DRVW_02d6_1260)
  {
    CLI
    mov8(RG1Blo, con(0x36)); // command: counter 0 mode 3 => generate rectangles
    outb(con(0x43), RG1Blo); // Timer 8253 - 5 (AT: 8254.2).
    mov8(RG1Blo, RG2Blo); // value of RG2W is DRVW_02d6_125e
    outb(con(0x40), RG1Blo); // Timer 8253 - 5 (AT: 8254.2).
    mov8(RG1Blo, RG2Bhi); // value of RG2W is DRVW_02d6_125e
    outb(con(0x40), RG1Blo); // Timer 8253 - 5 (AT: 8254.2).
    DRVW_02d6_1260 = DRVW_02d6_125e;
  }
#endif
}

// TODO: find out what really happens here
// was sub_410A
void driver_getfilename(i8 arg, i8* str)
{
  *str = NULLBYTE;
  DRVB_findfirstarg = arg;
  if (SYS_findfirst(DRVS_findfirststr) != 0)
    return;
  i8* esbx = SYS_gettransferadress();
  i16 c = 11;
  i8 a;
  do
  {
    a = *(esbx + 8);
    if (a != ' ' && a != '.')
      *str++ = a;
    esbx++;
  } while (a != NULLBYTE && (c != 0));
  *str = NULLBYTE;
}

// can be called by both threads!
i16 driver_execmouseclick(i16 x, i16 y, i16 b)
{
  static bool semaphore = false;
  while (semaphore);
  semaphore = true;
  DRV_mouseentry.x = x;
  DRV_mouseentry.y = y;
  DRV_mouseentry.b = b;
  x16 ret = queue_int2e();
  semaphore = false;
  return ret;
}

// was sub_435E
void driver_userint(i16 a, i16 b)
{
  STI

  if ((a & 0x100) == 0)
  {
    switch (a)
    {
      case 0x0:
        driver_memcpy(P8CAST(DRVW_drvinterface), P8CAST(DRVW_drvinterface_out), sizeof(DRVW_drvinterface_out));
        DRVb_func0done = true;
      break;
      case 0x1:
        DRVW_drvinterface[0] = driver_kbdqueue_pop();
      break;
      case 0x2:
        DRVW_drvinterface[0] = driver_forceint2d() ? 1 : 0;
      break;
      case 0x3:
        driver_init_clickhandler();
      break;
      case 0x4:
        driver_blockmouseinput();
      break;
      case 0x5:
        driver_execmousefifo();
      break;
      case 0x6:
        driver_setmousecaptured();
      break;
      case 0x7:
        driver_setmouse();
      break;
      case 0x8:
        driver_showmouse();
      break;
      case 0x9:
        driver_hidemouse();
      break;
      case 0xa:
        driver_setcursorparams(DRVW_drvinterface[0], P8CAST(&DRVW_drvinterface[15]), DRVW_drvinterface[1], DRVW_drvinterface[2], DRVW_drvinterface[3], DRVW_drvinterface[4], DRVW_drvinterface[5], P8CAST(&DRVW_drvinterface[7]), *P8CAST(&DRVW_drvinterface[6]));
      break;
      case 0xb:
        driver_setcindex(b);
      break;
      case 0xc:
        driver_setrect2(&DRVR_rect1.r, DRVR_rect1.v);
      break;
      case 0xe:
        driver_copymouseentry(&DRVW_drvinterface[0], &DRVW_drvinterface[1], &DRVW_drvinterface[2], DRVW_drvinterface[0]);
      break;
      case 0x13: // TODO readsectors removed - check function again
        DRVW_drvinterface[0] = 0;
      break;
      case 0x14: // TODO no so selfexplaining - check function again
        driver_getfilename(I8CAST(b), P8CAST(DRVW_drvinterface));
      break;
      case 0x1b:
        DRVW_drvinterface[0] = driver_requestkeycode(b);
      break;
      case 0x29: // TODO: what exactly? looks like 0x2b with a smaller map
        *(i8**)(DRVW_drvinterface) = DRVB_unknown;
      break;
      case 0x2b:
        *(s_transferbitmap**)(DRVW_drvinterface) = DRVP_transfermapptr;
      break;
    }
  }
  else
  {
    switch (a & 0xff)
    {
      case 3:
        driver_showscreenrectdisabled((c_rect*)DRVW_drvinterface);
      break;
      case 5:
        driver_blitfromscreen((c_rect*)DRVW_drvinterface, DRVW_drvinterface[4]);
      break;
      case 6:
        driver_setpalette(I8CAST(b));
      break;
      case 9:
        if (DRVP_transfermapptr)
          driver_mouseblit(DRVP_transfermapptr->v, (c_rect*)DRVW_drvinterface, DRVW_drvinterface[4]);
      break;
      case 10:
        driver_convertalphapalette(P8CAST(DRVW_drvinterface));
        DRVW_drvinterface[0] = 1;
      break;
    }
  }
}
