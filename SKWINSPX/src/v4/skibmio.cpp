//------------------------------------------------------------------------------
// SK-IBMIO
//	Low level interface IBMIO / FIRE
//------------------------------------------------------------------------------

#include <skcnsole.h>

#include <skcore.h>
#include <stdlib.h>
#include <skmidi.h>
#include <skparam.h>
#include <skdebug.h>

//#include <dme.h>
#include <skglobal.h> // Pic8

using namespace kkBitBlt;

#include <string.h>

#ifdef _WIN32
    #define strcasecmp _stricmp
    #define strncasecmp _strnicmp
#endif // _WIN32

//..............................................................................

#ifdef __DJGPP__
#include <strings.h>
#include <time.h>
#include <stdlib.h>

#undef inportb
#undef outportb
#include <dos.h>

#ifndef _stricmp
#define _stricmp strcasecmp
#endif // _stricmp

#ifndef _exit
#define _exit exit
#endif // _exit


int getdrive()
{
	return 3; // C
}

#ifndef _getdrive
#define _getdrive getdrive
#endif // _getdrive

#define min(A,B) ((A < B) ? A : B)
#define max(A,B) ((A < B) ? B : A)

#endif // __DJGPP__

//..............................................................................

#if defined(__MINGW__) || defined(__LINUX__)
#include <strings.h>
#include <time.h>
#include <stdlib.h>
#undef inportb
#undef outportb
#define outportb(PORT,VAL)
#define inportb(PORT,VAL)
#ifndef _stricmp
#define _stricmp strcasecmp
#endif // _stricmp

#ifndef _exit
#define _exit exit
#endif // _exit

int getdrive() { return 3; }

#ifndef _getdrive
#define _getdrive getdrive
#endif // _getdrive

#define min(A,B) ((A < B) ? A : B)
#define max(A,B) ((A < B) ? B : A)

#endif // __MINGW__ / __LINUX__



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

// Some DOS functions for retrocompatibility


// SPX Renamed inportb to SKDOS_IMPORTB to avoid confusion with real DOS inportb
U8 SkWinCore::SKDOS_IMPORTB(U16 port) {
#ifdef __DJGPP__
	return inportb(port)
#else
	switch (port) {
		case 0x60: // key in
		{
			CSkKinput *p = skWinApp->DequeueKinput();
			if (p != NULL)
				return p->raw;
			return 0;
		}
		case 0x64: // keyboard status: 2=still buffered
			return (skWinApp->cntKeybIn != 0) ? 2 : 0;
	}
	return 0;
#endif
}

// SPX Implemented fake SKDOS_OUTPORTB to get rid of real DOS outportb
void SkWinCore::SKDOS_OUTPORTB(U16 port, U8 value)
{
#ifdef __DJGPP__
    outportb(port, value);
#else
    (void)port;
    (void)value;
#endif
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// FIRE - LEVEL

//^069A:0000
// FIRE_BOOTSTRAP calls FIRE_MAIN
int SkWinCore::FIRE_BOOTSTRAP() //#DS=089C
{
	SkD((DLV_DBG_DOS, "FIRE_BOOTSTRAP\n"));
	//^069A:0000
	// 4F75:0FE8  F8 0F 75 4F/04 10 75 4F/08 10 75 4F/00 00 00 00  EuO..uO..uO....
	// 4F75:0FF8  43 3A 5C 46 49 52 45 2E 45 58 45 00 2B 70 6D 00  C:\FIRE.EXE.+pm.
	// 4F75:1008  2B 73 62 00 75 49 00 00 5A 00 00 88 4F 00 00 00  +sb.uI..Z..E...
	skmidi = new SkWinMIDI(skWinApp->dung, skWinApp->sCustomDataFolder, (const char*) GET_DATA_FOLDER_NAME()); // TODO, init into skwinapp ?
	const char *argv[] = {"FIRE.exe", "+pm", "+sb"};
	return (FIRE_MAIN(3, argv, NULL));
}

//^069A:035B
X16 SkWinCore::_069a_035b(X16 xx)
{
	ENTER(0);
	if (xx == 0xFFFF)
		return 0xFFFF;
	if ((_4976_00e3[RCJ(5, U8(xx))] & 8) != 0)
		return U8(xx) - 32;
	return U8(xx);
}


//^13AE:02A3
// SPX: _13ae_02a3 renamed FIRE_MAIN
i16 SkWinCore::FIRE_MAIN(i16 argc, const char **argv, char **env) //#DS=4976
{
	UINT rc = 0;
	ENTER(0);
	SkD((DLV_DBG_DOS, "FIRE_MAIN\n"));
#if UseAltic
	;
#else
	i16 si;
	for (si = 1; si < argc; si++) {
		if (argv[si][0] == '+') {
			if (_069a_035b(argv[si][1]) == 'G') {
				if (_069a_035b(argv[si][2]) == 'L') {
					switch (_069a_035b(argv[si][3])) {
						case 'C': //^_0323
							glbFireUnknownGLCK = 0x60;
							break;
						case 'K': //^_032a
							glbFireUnknownGLCK = 0x50;
							break;
					}
				}
			}
		}
	}
	if (_sk_setjmp(_4976_4de0) == 0) {
	    FIRE_INIT();
		INIT();	// _38c8_04aa
		while (true) {
			GAME_LOOP();
			END_GAME(cd.pi.glbPlayerDefeated);
		}
	}
#endif
    FIRE_INIT();	// _4726_03b2
	rc = INIT();	// _38c8_04aa
	if (rc != 0)
		return -1;

	if (SkCodeParam::bSpecialModeInspect) {
		INSPECT();
		return 0;
	}

	while (true) {
		GAME_LOOP();
		END_GAME(cd.pi.glbPlayerDefeated);
	}
	return 0;
}

//------------------------------------------------------------------------------

//^069A:0406
/*
X16 SkWinCore::_sk_setjmp(jmp_buf xx)
{
	return setjmp(xx);
}
*/

//^01B0:20EF
// TODO: random seed ?
i16 SkWinCore::_01b0_20ef()
{
	// Query random seed?
	// AH=sec.
	// AL=milli sec.

	time_t t;
	t = time(&t);
	return U8(localtime(&t)->tm_sec) << 8;
}

//^069A:038D
void (SkWinCore::*SkWinCore::_crt_getvect(U16 interruptno))() {
#if UseAltic
	return _sys_getvect(interruptno);
#else
#error	Unc
#endif
}



//^01B0:2B1B
// TODO : that one does nothing ?!
// SPX: _01b0_2b1b renamed _01b0_2b1b
int SkWinCore::IBMIO_AUDIO_ZERO()
{
	ENTER(0);
	return 0;
}

//^01B0:0E80
// SPX: _01b0_0e80 replaced by SET_FUNC_TICK_STEP
X16 SkWinCore::SET_FUNC_TICK_STEP(void (SkWinCore::*pfn)()) //#DS=04BF
{
	ENTER(0);
	LOADDS(0x3083);
	glbFncTickStep = pfn;
	glbTickStepActive = 1;

	printf("SET_FUNC_TICK_STEP\n");
	SkD((DLV_DBG_TICK, "Tick step value = %03d\n", 1));
	return 1;
}

//^4726:03B2
// TODO: related to some init ?
// SPX: _4726_03b2 renamed FIRE_INIT
void SkWinCore::FIRE_INIT()	// _4726_03b2
{
	printf("FIRE_INIT\n");
	ENTER(0);
#if UseAltic
	_4976_5e90 = _4976_5e98 = _4976_5e88 = _4976_5e8c = 0;
#else
	_4976_5e90 = _crt_getvect(0xFE);
	_4976_5e98 = _4976_5e90;
	_4976_5e88 = _crt_getvect(0xFF);
	_4976_5e8c = _4976_5e88;
#endif
	glbRandomSeed = _01b0_20ef() CALL_IBMIO * 0x7AE3;
#if UseAltic
	glbRandomSeed = 12345;
#endif
	_00eb_0bc4();
	IBMIO_SELECT_PALETTE_SET(0);
	IBMIO_AUDIO_ZERO();
	glbTickStepValue = SET_FUNC_TICK_STEP(&SkWinCore::TICK_STEP_CHECK);
	return;
}

//^069A:0309
U32 SkWinCore::_crt_farcoreleft() { // TODO: Unr
	Unr(); return 0;
}

//^069A:0387
U32 SkWinCore::_crt_coreleft() { // TODO: Unr
	Unr(); return 0;
}

//^069A:0470
i16 SkWinCore::_crt_setblock(U16 segx, U16 newsize) { // TODO: Unr
	Unr(); return 0;
}

//^069A:01DD
U16 SkWinCore::_crt_normalize(U8 *cp) { // TODO: Unr
	Unr(); return 0;
}

//^069A:029C
U8 *SkWinCore::_crt_brk(U32 addr) { // TODO: Unr
	Unr(); return NULL;
}

//^069A:0607
U8 *SkWinCore::_crt_CreateHeap(U16 ax) { // TODO: Unr
	Unr(); return NULL;
}

//^069A:06CE
U8 *SkWinCore::_crt_farmalloc(U32 size) { // TODO: Unr
	Unr(); return NULL;
}








//------------------------------------------------------------------------------
// IBMIO - LEVEL

//^01B0:2B27
i16 SkWinCore::IBMIO_EXEC(const U8 *exe, const U8 *arg)
{
	SkD((DLV_FYI, "EXEC: %s %s\n", exe, arg));
	SkD((DLV_DBG_DOS, "IBMIO_EXEC: %s %s\n", exe, arg));
	if (strcasecmp(reinterpret_cast<const char *>(exe), "FIRE.exe") == 0) {
		return FIRE_BOOTSTRAP();
	}
	if (strcasecmp(reinterpret_cast<const char *>(exe), "anim") == 0) {
		if (strcasecmp(reinterpret_cast<const char *>(arg), "swoosh +pm +sb") == 0) {
			return ANIM_BOOTSTRAP_SWOOSH();
		}
		else if (strcasecmp(reinterpret_cast<const char *>(arg), "title +ah +as +ab +pm +sb") == 0) {
			return ANIM_BOOTSTRAP_TITLE();
		}
	}
	return 0;
}

//^00EB:0BC4
void SkWinCore::_00eb_0bc4() //#DS=04BF
{
	ENTER(0);
	LOADDS(0x0C48);
	_04bf_0e34 = reinterpret_cast<U8 *>(__vram);
	U16 si = 1;
	do {
		if (si < 0xfa00) {
			IBMIO_FILL_SCREEN_LINE(si, 0, 1);
		}
		si = ((si & 1) != 0) ? ((si >> 1) ^ 0xb400) : (si >> 1);
	} while (si != 1);
	IBMIO_FILL_SCREEN_LINE(0, 0, 1);
	return;
}

//^01B0:2C1C
void SkWinCore::IBMIO_PRINT_ERROR(const U8 *str) //#DS=04BF
{
#if UseAltic
	fputs(reinterpret_cast<const char *>(str), stdout);
#else
	ENTER(2);
	LOADDS(0x3083);
	// int 10h
	// http://hp.vector.co.jp/authors/VA003720/lpproj/int10h/int10h.htm

	// int 21h
	// http://www2.neweb.ne.jp/wd/wonder/codea.htm

	//^01B0:2C26
	__asm mov ah,0x00
	__asm mov al,0x13
	__asm int 0x10
	// SPX : this would set DOS video mode x13
	/*
	{
		union REGS regs;
		regs.h.ah = 0x00;
		regs.h.al = 0x13;  // Mode 0x13: 320x200, 256 colors
		int86(0x10, &regs, &regs);
	}*/
	U8 bp01;
	while ((bp01 = *(str++)) != 0) {
		__asm mov dl,[bp01]
		__asm mov ah,2
		__asm int 0x21
	}
	return;
#endif
}

// SPX: _0088_020b renamed IBMIO_EXIT_BAD_CPU
void SkWinCore::IBMIO_EXIT_BAD_CPU(X16 xx) { // TODO: Unr
	Unr();
}

//^01B0:2BE0
U16 SkWinCore::IBMIO_CHECK_CPU_ERA() {
	return 386;
}

//^01B0:1F3D
U16 SkWinCore::IBMIO_DETECT_SBLASTER()	// as DETECT AUDIO
{
	return 1;
}

//^00EB:0C21
void SkWinCore::IBMIO_INIT_VID()
{
#if UseAltic
	_04bf_09de = 0;
#else
	ENTER(0);
	_crt88_setvect(255, _04bf_0090);
	__asm mov ah,0xf
	__asm int 0x10
	__asm mov _04bf_09de, al
	__asm mov ah,0
	__asm mov al,0x13
	__asm int 0x10
	return;
#endif
}
//^01B0:0A6A
X16 SkWinCore::IBMIO_INIT_MOUSE()
{
#if UseAltic
	return _04bf_18b2 = 1;
#else
	ENTER(0);

	// int 33h
	// http://www2.muroran-it.ac.jp/circle/mpc/old/pc98dos/mouse/mouse.html

	__asm mov ax,0
	__asm int 0x33
	__asm mov _04bf_18b2, ax
	if (_04bf_18b2 == 1) {
		__asm mov cx,0
		__asm mov dx,0
		__asm int 0x33
		__asm mov cx,0
		__asm mov dx,0x27f
		__asm mov ax,0x7
		__asm int 0x33
		__asm mov cx,0
		__asm mov dx,0xc7
		__asm mov ax,0x8
		__asm int 0x33
	}
	return _04bf_18b2;
#endif
}
void SkWinCore::_04bf_0090() { // TODO: Unr
	Unr();
}

//^01B0:0426
void SkWinCore::INIT_KBOARD_HOOK() //#DS=04BF
{
#if UseAltic
	ENTER(0);
	glbUIKeyReadCount = 0;
	glbKeyboardReadRRIndex = 0;
	glbKeyboardWriteRRIndex = 0;
	_04bf_0e7e = 0;
	_04bf_185c = 0;
	_04bf_18a8 = 0;
	_04bf_1862 = 0;
	_01b0_0453 = _sys_getvect(0x09);
	_sys_setvect(0x09, &SkWinCore::IBMIO_KBOARD_HANDLER);
	cd.dos.glbKeyboardHook = 1;
	return;
#else
#error	Unr
#endif
}
//^01B0:0478
void SkWinCore::IBMIO_KBOARD_HANDLER() { //#DS=04BF
#if UseAltic
	LOADDS(0x3083);
	IBMIO_CHECK_KEYBOARD_INPUT();
	if (glbDMode == 0) {
		// MessageBeep(-1);
		return;
	}
	return;
#else
#error	Unr
#endif
}

//^01B0:00CE
// _01b0_00ce renamed
void SkWinCore::IBMIO_STORE_KEYBOARD_INPUT(X16 iKeyboardVInput)
{
	ENTER(0);
	if (glbUIKeyReadCount < 10) {
		tblUIKeyInput[glbKeyboardWriteRRIndex] = iKeyboardVInput;
		glbKeyboardWriteRRIndex = (glbKeyboardWriteRRIndex + 1) % 10;
		glbUIKeyReadCount++;
	}
	return;
}

//^01B0:00FC
// _01b0_00fc renamed IBMIO_CHECK_KEYBOARD_INPUT
void SkWinCore::IBMIO_CHECK_KEYBOARD_INPUT()
{
	ENTER(6);
	do {
		U16 bp06 = SKDOS_IMPORTB(0x60);	// get "v" keyboard value
		if (bp06 == 0xE0) {
			_04bf_0e7e |= 0x1000;
			continue;
		}
		X16 si = 0;
		U16 *bp04;
		if ((_04bf_0e7e & 0x1000) != 0) {
			bp04 = _04bf_0e6a;
			si = 1;
		}
		else {
			bp04 = _04bf_0e54;
			if ((_04bf_0e7e & 0x800) != 0) {
				_04bf_1862 = 1;
				switch (bp06) {
				case 72://^0158
					_04bf_18a8 |= 1; break;
				case 75://^015F
					_04bf_18a8 |= 4; break;
				case 77://^0166
					_04bf_18a8 |= 8; break;
				case 80://^016D
					_04bf_18a8 |= 2; break;
				case 71://^0174
					_04bf_18a8 |= 0x20; break;
				case 73://^017B
					_04bf_18a8 |= 0x40; break;
				case 79://^0182
					_04bf_18a8 |= 0x80; break;
				case 81://^018A
					_04bf_18a8 |= 0x100; break;
				case 78://^0192
					_04bf_18a8 |= 0x10; break;
				case 74://^0199
				case 76://^0199
				default:
					_04bf_1862 = 0; break;
				}
			}
		}
		if (_04bf_1862 == 0) {
			if ((_04bf_0e7e & 0x1000) == 0) {
				X16 di = bp06;
				switch (di) {
				case 0x2A://^01D1
					_04bf_0e7e = _04bf_0e7e|0x8000; break;
				case 0x36://^01DD
					_04bf_0e7e |= 0x4000; break;
				case 0x1D://^01E5
					_04bf_0e7e |= 0x400; break;
				case 0x38://^01ED
					_04bf_0e7e |= 0x800; break;
				case 0xAA://^01F5
					_04bf_0e7e = _04bf_0e7e & 0x7fff; break;
				case 0xB6://^01FD
					_04bf_0e7e &= 0xbfff; break;
				case 0x9D://^0205
					_04bf_0e7e &= 0xfbff; break;
				case 0xB8://^020D
					_04bf_0e7e &= 0xf7ff; break;
				case 0xC8://^0215
					_04bf_18a8 &= 0xfffe; break;
				case 0xCB://^021D
					_04bf_18a8 &= 0xfffb; break;
				case 0xCD://^0225
					_04bf_18a8 &= 0xfff7; break;
				case 0xD0://^022D
					_04bf_18a8 &= 0xfffd; break;
				case 0xC7://^0235
					_04bf_18a8 &= 0xffdf; break;
				case 0xC9://^023D
					_04bf_18a8 &= 0xffbf; break;
				case 0xCF://^0245
					_04bf_18a8 &= 0xff7f; break;
				case 0xD1://^024D
					_04bf_18a8 &= 0xfeff; break;
				case 0xCE://^0255
					_04bf_18a8 &= 0xffef; break;
				default:
					si = 1; break;
				}
			}
			if (si != 0 && (bp06 & 0x80) == 0) {
				switch (bp06) {
					case 0x3b://^_0284
						if (_04bf_0296 > 0)
							_04bf_0296--;
		_028f:
						_04bf_0298 = _04bf_029c[RCJ(8,_04bf_0296)];
						_04bf_029a = _04bf_02ac[RCJ(8,_04bf_0298)];
						break;
					case 0x3c://^_02ab
						if (_04bf_0296 < 7)
							_04bf_0296++;
						goto _028f;
					default: //^_02ba
						if ((bp04[(bp06 >> 4)&7] & (1 << (bp06&15))) == 0) {
							IBMIO_STORE_KEYBOARD_INPUT((_04bf_0e7e & 0x1e00)|bp06);
							break;
						}
				}
			}
		}
		if ((bp06 & 0x80) != 0) {
			bp04[(bp06 >> 4)&7] &= ~(1 << (bp06&15));
		}
		else {
			bp04[(bp06 >> 4)&7] |=   1 << (bp06&15);
		}
		if ((_04bf_0e7e & 0x8000) != 0 || (_04bf_0e7e & 0x4000) != 0) {
			_04bf_0e7e |= 0x200;
		}
		else {
			_04bf_0e7e &= 0xfdff;
		}
		if ((_04bf_18a8 & 0x61) != 0) {
			_04bf_185c |= 1;
		}
		else {
			_04bf_185c &= 0xfffe;
		}
		if ((_04bf_18a8 & 0x182) != 0) {
			_04bf_185c |= 2;
		}
		else {
			_04bf_185c &= 0xfffd;
		}
		if ((_04bf_18a8 & 0x148) != 0) {
			_04bf_185c |= 8;
		}
		else {
			_04bf_185c &= 0xfff7;
		}
		if ((_04bf_18a8 & 0xa4) != 0) {
			_04bf_185c |= 4;
		}
		else {
			_04bf_185c &= 0xfffb;
		}
		if ((_04bf_18a8 & 0x10) != 0) {
			_04bf_185c |= 0x10;
		}
		else {
			_04bf_185c &= 0xffef;
		}
		_04bf_0e7e &= 0xefff;
	} while ((SKDOS_IMPORTB(0x64) & 2) != 0);
	return;
}

void SkWinCore::_01b0_18ae() { // TODO: Unr
	Unr();
}
X16 SkWinCore::_01b0_292b() { // TODO: Unr
	Unr(); return 0;
}
void SkWinCore::_01b0_237f() { // TODO: Unr
	Unr();
}
//^01B0:20FF
void SkWinCore::_01b0_20ff()
{
	ENTER(0);
    if (glbCounterZero_0517 == 0)
		return;
	if (glbSomeCounter_0519 <= 0) {
		do {
			glbSomeCounter_0519 += _01b0_292b() * 240;
		} while (glbSomeCounter_0519 <= 0);
		if (_04bf_0e64 != 0) {
			_04bf_18a4 = 0;
			_01b0_237f();
			glbCounterZero_0517 = 0;
		}
	}
	else {
		glbSomeCounter_0519 -= glbDecreasing120;
	}
	return;
}

//^01B0:0E2C
U16 SkWinCore::_01b0_0e2c()
{
	ENTER(0);
	SKDOS_OUTPORTB(0x43,0x80);
	U8 al = SKDOS_IMPORTB(0x42);	// always return 0 ?
	U8 ah = SKDOS_IMPORTB(0x42);	// always return 0 ?
	return (ah<<8)|al;
}

//^01B0:0E27
X16 SkWinCore::_01b0_0e27(X16 xx) 
{
	ENTER(0);
	while ((SKDOS_IMPORTB(0x0201) & 3) != 0) {
	}
	U16 tmp0 = _01b0_0e2c();
	SKDOS_OUTPORTB(0x0201, U8(tmp0));
	U8 al;
	U8 bl = U8(xx);
	while ((bl & (al = SKDOS_IMPORTB(0x0201))) != 0) {
	}
	if (bl >= al) {
		U16 tmp1 = _01b0_0e2c();
		if (tmp0 < tmp1) {
			return 0xFFFF -tmp1 +tmp0;
		}
		else {
			return tmp1 -tmp0;
		}
	}
	else {
		return 0;//unstable value
	}
}

//^01B0:0FA3
void SkWinCore::_01b0_0fa3()
{
	ENTER(0);
	if (glbPType == 1 && sysMousePositionCaptured == 0 && sysSomeSemaphore == 0) {
		sysSomeSemaphore = 1;
		SKDOS_OUTPORTB(0x20,0x20);
		if ((_04bf_0e4c++ & 1) != 0) {
			_04bf_1798 = (_01b0_0e27(1) -100) / _04bf_0298;
		}
		else {
			_04bf_03c8 = _04bf_1798;
			if (_04bf_03c8 > 0x13f) {
				_04bf_03c8 = 0x13f;
			}
			else if (_04bf_03c8 < 0) {
				_04bf_03c8 = 0;
			}
			_04bf_03ca = (_01b0_0e27(2) -100) / _04bf_029a;
			if (_04bf_03ca > 0xc7) {
				_04bf_03ca = 0xc7;
			}
			else if (_04bf_03ca < 0) {
				_04bf_03ca = 0;
			}
		}
		_04bf_03d0 = SKDOS_IMPORTB(0x201);
		_04bf_03d0 = ((_04bf_03d0 >> 4) ^ 3)&3;
		sysSomeSemaphore = 0;
	}
	else {
		if (glbPType != 2 && glbPType != 3)
			return;
		_04bf_179a = (cd.sc.glbSoundBlasterBasePort != 0 && glbPType == 2) ? (SKDOS_IMPORTB(_01b0_3378 +1)) : 0x78;
		if (_04bf_03d2 == 1) {
			if ((_04bf_179a & 0x80) == 0) {
				_04bf_03d2 = 0;
			}
			else if (((_04bf_179a ^ _04bf_0e4a)&0x78) != 0) {
				if ((_04bf_179a & 8) == 0) {
					IBMIO_STORE_KEYBOARD_INPUT(0x48);
				}
				else if ((_04bf_179a & 16) == 0) {
					IBMIO_STORE_KEYBOARD_INPUT(0x50);
				}
				else if ((_04bf_179a & 32) == 0) {
					IBMIO_STORE_KEYBOARD_INPUT(0x47);
				}
				else if ((_04bf_179a & 64) == 0) {
					IBMIO_STORE_KEYBOARD_INPUT(0x49);
				}
			}
		}
		if (_04bf_03d2 == 0) {
			if (sysMousePositionCaptured == 0) {
				if ((_04bf_179a & 0x78) != 0x78 || (_04bf_185c & 15) != 0) {
					if ((_04bf_179a & 8) == 0 || (_04bf_185c & 1) != 0) {
						_04bf_03ca -= ((++_04bf_03ce +1) / 3) +1;
					}
					if ((_04bf_179a & 0x10) == 0 || (_04bf_185c & 2) != 0) {
						_04bf_03ca += ((++_04bf_03ce +1) / 3) +1;
					}
					if ((_04bf_179a & 0x40) == 0 || (_04bf_185c & 8) != 0) {
						_04bf_03c8 += ((++_04bf_03cc +1) / 3) +1;
					}
					if ((_04bf_179a & 0x20) == 0 || (_04bf_185c & 4) != 0) {
						_04bf_03c8 -= ((++_04bf_03cc +1) / 3) +1;
					}
				}
				if (_04bf_03cc > 0x64)
					_04bf_03cc = 0x64;
				if (_04bf_03ce > 0x64)
					_04bf_03ce = 0x64;
				if (_04bf_03c8 < 0)
					_04bf_03c8 = 0;
				if (_04bf_03c8 > 0x13f)
					_04bf_03c8 = 0x13f;
				if (_04bf_03ca < 0)
					_04bf_03ca = 0;
				if (_04bf_03ca > 0xc7)
					_04bf_03ca = 0xc7;
			}
			if (((_04bf_179a ^ _04bf_0e4a)&0x18) != 0)
				_04bf_03ce = 0;
			if (((_04bf_179a ^ _04bf_0e4a)&0x60) != 0)
				_04bf_03cc = 0;
			if (((_04bf_185c ^ _04bf_18a6)&3) != 0)
				_04bf_03ce = 0;
			if (((_04bf_185c ^ _04bf_18a6)&12) != 0)
				_04bf_03cc = 0;
			_04bf_03d0 = ((_04bf_179a >> 7) & 1) | ((_04bf_185c >> 4) & 1);
			if ((_04bf_179a & 0x80) != 0) {
				if ((_04bf_03c8 >= 0xe8 && _04bf_03c8 <= 0x13f && _04bf_03ca >= 0x7b && _04bf_03ca <= 0xa8) || _04bf_03ca > 0xa8)
					_04bf_03d2 = 1;
			}
		}
		_04bf_0e4a = _04bf_179a;
		_04bf_18a6 = _04bf_185c;
	}
	X16 cx = _04bf_03c8;
	X16 dx = _04bf_03ca;
	X16 bx = _04bf_03d0;
	if (glbMouseEventLock == 0) {
		IBMIO_MOUSE_EVENT_RECEIVER(cx, dx, bx);
		return;
	}
	if (_04bf_17a4 < 10) {
		X16 di;
		if (_04bf_17a4 != 0) {
			di = _04bf_0e50;
			if (_04bf_17ac[di].w0 == bx) {
				_04bf_17ac[di].w2 = cx;
				_04bf_17ac[di].w4 = dx;
				return;
			}
		}
		_04bf_17a4++;
		di = _04bf_0e50 +1;
		if (di == 10)
			di = 0;
		_04bf_0e50 = di;
		_04bf_17ac[di].w0 = bx;
		_04bf_17ac[di].w2 = cx;
		_04bf_17ac[di].w4 = dx;
		return;
	}
	return;
}

//^01B0:180C
// _01b0_180c renamed IBMIO_CALL_TICKSTEP
void SkWinCore::IBMIO_CALL_TICKSTEP() {
	LOADDS(0x3083);
	if (glbDMode == 0 || glbTickStepActive != 0) {
		_01b0_14d8++;
		if ((_01b0_14d8 & 3) == 0) {
			(this->*glbFncTickStep)(); INDIRECT_CALL	// TICK_STEP_CHECK
		}
	}
	_01b0_20ff();
	bool carry = U16(_01b0_13c6) < U16(_01b0_13ca);
	_01b0_13c6 -= _01b0_13ca;
	U16 bx = _01b0_13c8;
	if (bx != _01b0_13ca) {
		// __asm mov al,0x36
		// __asm out 0x43,al
		// __asm mov al,bl
		// __asm out 0x40,al
		// __asm mov al,bh
		// __asm out 0x40,al
		_01b0_13ca = bx;
		glbSoundFreq_13cc = glbSoundFreq_13ce;
	}
	if (!carry) {
		// __asm mov al,0x20
		// __asm out 0x20,al
		return;
	}
	LOADDS(0x3083);
	_01b0_0fa3();
	(this->*_01b0_13be)();
}

//^01B0:14EB
void SkWinCore::_INT08_HANDLER() {
	IBMIO_CALL_TICKSTEP();
}

//^01B0:1315
void SkWinCore::IBMIO_INIT_TIMER()
{
	ENTER(2);
	if (glbPType == 1) {
		//^01B0:1322
		// __asm mov al,0xb6
		// __asm out 0x43,al
		// __asm mov al,0
		// __asm out 0x42,al
		// __asm out 0x42,al
		// __asm in al,0x61
		// __asm or al,0x1
		// __asm out 0x61,al
		//^01B0:1335
	}
	else if (glbPType == 2 && cd.sc.glbSoundBlasterBasePort != 0) {
		//^01B0:1345
		// __asm mov dx,glbSoundBlasterBasePort
		// __asm add dx,0x2
		// __asm mov al,4
		// __asm out dx,al
	}
	_01b0_14db = cd.sc.glbSoundBlasterBasePort;
	if (cd.sc.glbSoundBlasterBasePort != 0 && (cd.sc.glbSoundCardType == 3 || cd.sc.glbSoundCardType == 5)) {
	}
	_01b0_13be = _sys_getvect(0x08);
	_sys_setvect(0x08, &SkWinCore::_INT08_HANDLER);
	X16 bp02 = 0x136b;
	_01b0_13c2 = _01b0_13c8 = bp02;
	glbSoundFreq_13ce = 0x180c;	// 0x180c = 6156
	return;
}
//^01B0:1893
void SkWinCore::IBMIO_UNINIT_TIMER()
{
	ENTER(0);
	glbTickStepActive = 0;
	while (sndLockSoundBuffer != 0) {
#if UseAltic
		sndLockSoundBuffer = 0;
#endif
	}
	SKDOS_OUTPORTB(0x43,0x36);
	SKDOS_OUTPORTB(0x40,0);
	SKDOS_OUTPORTB(0x40,0);
	_sys_setvect(0x08, _01b0_13be);
	_01b0_13ca = 0;
	SKDOS_OUTPORTB(0x61,SKDOS_IMPORTB(0x61) & 0xFC);	// that would apparently turn off the speaker
	return;
}
//^01B0:0A39
void SkWinCore::IBMIO_UNINIT_MOUSE()
{
	ENTER(0);
	if (cd.dos.glbMouseHandlerSet != 0) {
		cd.dos.glbMouseHandlerSet = 0;
		cd.dos.glbMouseEventReceiverSet = 0;
		_int33_mouse_callback = _01b0_092d_MOUSE_CALLBACK;
	}
	return;
}
//^01B0:04BF
void SkWinCore::IBMIO_UNINIT_KBOARD_HOOK()
{
	ENTER(0);
	if (cd.dos.glbKeyboardHook != 0) {
		cd.dos.glbKeyboardHook = 0;
		_sys_setvect(0x09, _01b0_0453);
	}
	return;
}
//^00EB:0C4C
i16 SkWinCore::IBMIO_UNINIT_VID()
{
	return 0;
	// DOS => reset video
}


UINT SkWinCore::SKLIB_IBMIO_INIT()
{
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_YELLOW, BLACK);
	printf("SK IBMIO INIT\n");
	CHANGE_CONSOLE_COLOR(BRIGHT, LIGHT_GRAY, BLACK);

	cd.dos.glbCPUClass = IBMIO_CHECK_CPU_ERA();

	IBMIO_INIT_VID();
	_crt88_setvect(0xFE, &SkWinCore::_04bf_0102);
	i16 di = 0;

	X16 bp08;
	if (glbPType == 0 || glbPType == 4) {
		bp08 = IBMIO_INIT_MOUSE();
		if (bp08 != 0) {
			glbPType = 4;
		}
		else {
			glbPType = 3;
		}
	}
	INIT_KBOARD_HOOK();
	IBMIO_01b0_08d8();
	if (glbPType == 4 && bp08 != 0)
		IBMIO_SET_MOUSE_HANDLER();
	IBMIO_INIT_TIMER();
	_01b0_18d3_AUDIO(0);
	FIRE_INIT();
	return 0;
}

//^01B0:2C48
// SPX: _01b0_2c48 replaced by IBMIO_MAIN
i16 SkWinCore::IBMIO_MAIN(i16 argc, const char **argv, char **env) //#DS=04BF
{
	// IBMIO_main
	SkD((DLV_DBG_DOS, "IBMIO_MAIN\n"));
	ENTER(172);
	U8 bp5c[80] = {0};
	cd.dos.glbCPUClass = IBMIO_CHECK_CPU_ERA();
	if (cd.dos.glbCPUClass < 286) {
		IBMIO_PRINT_ERROR(strPC286Required);
		IBMIO_EXIT_BAD_CPU(0);
	}
	i16 si;
	for (si = 1; si < argc; si++) {
		if (argv[si][0] == '+') {
			X16 bp0a;
			X16 bp06;
			X16 bp0c;
			switch (bp0a = SK88_TOUPPER(U8(argv[si][1]))) {
			case 'S'://^2CD5
				switch (SK88_TOUPPER(U8(argv[si][2]))) {
				case 'I'://^2D03
					cd.sc.glbSoundCardType = ScardTandy;
					break;
				case 'N'://^2D0C
					cd.sc.glbSoundCardType = Scard01;
					break;
				case 'T'://^2D15
					cd.sc.glbSoundCardType = Scard04;
					break;
				case 'D'://^2D1E
					cd.sc.glbSoundCardType = Scard05;
					bp06 = SK88_TOUPPER(argv[si][3]);
					if (bp06 >= '1' && bp06 <= '3') {
						cd.sc.glbSoundBlasterBasePort = _04bf_05f9[RCJ(18,bp06)];
					}
					SKDOS_OUTPORTB(cd.sc.glbSoundBlasterBasePort +2, SKDOS_IMPORTB(cd.sc.glbSoundBlasterBasePort +2));
					break;
				case 'B'://^2D67
					cd.sc.glbSoundBlasterBasePort = IBMIO_DETECT_SBLASTER();
					if (cd.sc.glbSoundBlasterBasePort != 0)
						cd.sc.glbSoundCardType = ScardSBlaster;
					break;
				case 'A'://^2D7E
					cd.sc.glbSoundBlasterBasePort = 0x388;
					_04bf_0e4e = 0x388;
					cd.sc.glbSoundCardType = Scard07;
					break;
				case 'F'://^2D90
					cd.sc.glbSoundCardType = Scard03;
					bp06 = SK88_TOUPPER(argv[si][3]);
					if (bp06 >= '1' && bp06 <= '3')
						cd.sc.glbSoundBlasterBasePort = _04bf_05f9[RCJ(18,bp06)];
					break;
				case 67://^2EAF
				case 69://^2EAF
				case 71://^2EAF
				case 72://^2EAF
				case 74://^2EAF
				case 75://^2EAF
				case 76://^2EAF
				case 77://^2EAF
				case 79://^2EAF
				case 80://^2EAF
				case 81://^2EAF
				case 82://^2EAF
				case 83://^2EAF
					break;
				}
				break;
			case 'P'://^2DD4
				switch (bp0c = SK88_TOUPPER(argv[si][2])) {
				case 'P'://^2E0A
					glbPType = 2;
					break;
				case 'A'://^2E12
					glbPType = 1;
					if (argv[si][3] != 0) {
						_04bf_0296 = argv[si][3] -0x30;
						if (_04bf_0296 >= 0 && _04bf_0296 <= 7) {
							_04bf_0298 = _04bf_029c[RCJ(8,_04bf_0296)];
							_04bf_029a = _04bf_02ac[RCJ(8,_04bf_0296)];
							break;
						}
						_04bf_0296 = 0;
						break;
					}
					break;
				case 'K'://^2E77
					glbPType = 3;
					break;
				case 'M'://^2E7F
					glbPType = 4;
					break;
				}
				goto _2e85;
			case 'M'://^2E85
_2e85:
				SK88_TOUPPER(argv[si][2]);
				break;
			case 'Z'://^2EA1
				glbZMode = 0;
				break;
			case 'D'://^2EA9
				glbDMode = 1;
				break;
			}
		}
	}
	IBMIO_INIT_VID();
	_crt88_setvect(0xFE, &SkWinCore::_04bf_0102);
	si = 2;
	i16 di = 0;
	for (; si < argc; bp5c[di] = 0, si++) {
		if (di > 1) {
			bp5c[di++] = 0x20;
		}
		i16 bp04 = 0;
		X8 bp01;
		for (; bp04 < 0x50; bp5c[di] = bp01, di++, bp04++) {
			bp01 = argv[si][bp04];
			if (bp01 == 0)
				break;
		}
	}
	X16 bp08;
	if (glbPType == 0 || glbPType == 4) {
		bp08 = IBMIO_INIT_MOUSE();
		if (bp08 != 0) {
			glbPType = 4;
		}
		else {
			glbPType = 3;
		}
	}
	do {
		INIT_KBOARD_HOOK();
		IBMIO_01b0_08d8();
		if (glbPType == 4 && bp08 != 0)
			IBMIO_SET_MOUSE_HANDLER();
		IBMIO_INIT_TIMER();
		if (glbZMode != 0) {
			U8 bp00ac[80];
			SK88_STRCPY(bp00ac, _04bf_01b2[RCJ(4,_04bf_0280)].b14);
			SK88_STRCAT(bp00ac, _04bf_067a);
			SK88_STRCAT(bp00ac, bp5c);
			if (_04bf_01b2[RCJ(4,_04bf_0280)].b0[0] == '@') {
				si = IBMIO_EXEC(reinterpret_cast<const U8 *>(argv[_04bf_01b2[RCJ(4,_04bf_0280)].b0[1] -'0']), bp00ac);
			}
			else {
				si = IBMIO_EXEC(&_04bf_01b2[RCJ(4,_04bf_0280)].b0[0], bp00ac);
			}
			if (_04bf_01b2[RCJ(4,_04bf_0280)].b46 < si)
				si = 0;
			_04bf_0280 = _04bf_01b2[RCJ(4,_04bf_0280)].b47 -1;
		}
		else {
			IBMIO_EXEC(reinterpret_cast<const U8 *>(argv[1]), bp5c);
		}
		_01b0_18d3_AUDIO(0);
		if (cd.sc.glbSoundCardType == 5) {
			SKDOS_OUTPORTB(cd.sc.glbSoundBlasterBasePort +2, SKDOS_IMPORTB(cd.sc.glbSoundBlasterBasePort +2) | 8);
		}
		IBMIO_UNINIT_TIMER();
		if (glbPType == 4 && bp08 != 0)
			IBMIO_UNINIT_MOUSE();
		IBMIO_UNINIT_KBOARD_HOOK();
	} while (_04bf_0280 > 0);
	return IBMIO_UNINIT_VID();
}

//^0088:0000
UINT SkWinCore::IBMIO_BOOTSTRAP()
{
	SkD((DLV_DBG_DOS, "IBMIO_BOOTSTRAP\n"));
	//^0088:0000
	// 065B:01DA  EE 01 5B 06/FB 01 5B 06/04 02 5B 06/08 02 5B 06  E[.E[...[...[.
	// 065B:01EA  00 00 00 00 43 3A 5C 49 42 4D 49 4F 2E 45 58 45  ....C:\IBMIO.EXE
	// 065B:01FA  00 46 49 52 45 2E 45 58 45 00 2B 70 6D 00 2B 73  .FIRE.EXE.+pm.+s
	// 065B:020A  62 00 BE 04 00 00 5A 00 00 82 99 00 00 00 44 4D  b.¾...Z..‚™...DM
	const char *argv[] = {"IBMIO.exe", "FIRE.exe", "+pm", "+sb"};
	try {
		return (IBMIO_MAIN(4, argv, NULL));
	}
	catch (i8) { // termination request at message loop
		return 1;
	}
}







//^00EB:0254
void SkWinCore::IBMIO_BLIT_TO_SCREEN_8TO8BPP(
	void *src,
	void *dst,
	SRECT *prc,
	i16 srcx,
	i16 srcy,
	i16 srcpitch,
	i16 dstpitch,
	i16 colorkey
	) //#DS=04BF
{
	//printf("IBMIO_BLIT_TO_SCREEN_8TO8BPP\n");
	ATLASSERT(srcx >= 0 && srcy >= 0 && srcpitch >= 0 && dstpitch >= 0 && srcpitch <= 320 && dstpitch <= 320);
	ATLASSERT(prc->x >= 0 && prc->y >= 0 && prc->cx < 320 && prc->cy < 200);

	ENTER(6);
	glbBlitBufferSource = reinterpret_cast<U8 *>(src);
	_04bf_0e34 = reinterpret_cast<U8 *>(dst);
	X16 bp04 = prc->cx;
	X16 bp02 = prc->cy;
	//printf("cx =%d / cy = %d\n", bp04, bp02);
	if (bp04 <= 0 || bp02 <= 0)
		return;
	X16 di = prc->y *dstpitch +prc->x;
	X16 si = srcy *srcpitch +srcx;
	X16 bp06 = 0;
	for (; bp06 < bp02; si += srcpitch, di += dstpitch, bp06++) {
		if (colorkey < 0) {
			MOVE_MEMORY_WITH_DELTA(si, di, bp04);
		}
		else {
			COPY_MEMORY_WITH_DELTA_AND_TRANSPARENT(si, di, bp04, U8(colorkey));
		}
	}

	//printf("dst = %08x / vram = %08x\n", dst, vram);
#ifdef __DJGPP__
	if (dst == vram) {
		//printf("skwin.UpdateRect");
		//skwin.UpdateRect(prc->x, prc->y, prc->cx, prc->cy);
		skWinApp->renderScreen(vram, prc->x, prc->y, prc->cx, prc->cy);
	}
#elif UseAltic
	if (dst == vram) {
		if (skWinApp->skwin_IsAvail() != false)  {
			//skwin.UpdateRect(prc->x, prc->y, prc->cx, prc->cy);
			skWinApp->renderScreen(vram, prc->x, prc->y, prc->cx, prc->cy);
		//MessageLoop();
		}
	}
#endif

}

//^44C8:1101
void SkWinCore::FIRE_BLIT_PICTURE(
	const void *src,
	void *dst,
	SRECT *rc,
	U16 srcx,
	U16 srcy,
	U16 srcPitch,
	U16 dstPitch,
	i16 colorkey,
	U16 mirrorFlip,
	U16 srcBpp,
	U16 dstBpp,
	const U8 *localPal
	)
{
	SkD((DLV_GUI||SkCodeParam::bDebugPrint, "FIRE_BLIT_PICT: xTox (%p,%p,(%3d,%3d,%3d,%3d),%3d,%3d,%3d,%3d,%2d,%d,%d,%d,%p)\n", src, dst
		, (Bitu)rc->x, (Bitu)rc->y, (Bitu)rc->cx, (Bitu)rc->cy, (Bitu)srcx, (Bitu)srcy
		, (Bitu)srcPitch, (Bitu)dstPitch, (Bitu)colorkey, (Bitu)mirrorFlip, (Bitu)srcBpp, (Bitu)dstBpp, localPal));
	LOGX(("FIRE_BLIT_PICT: xTox (%p,%p,(%3d,%3d,%3d,%3d),%3d,%3d,%3d,%3d,%2d,%d,%d,%d,%p)", src, dst
		, (Bitu)rc->x, (Bitu)rc->y, (Bitu)rc->cx, (Bitu)rc->cy, (Bitu)srcx, (Bitu)srcy
		, (Bitu)srcPitch, (Bitu)dstPitch, (Bitu)colorkey, (Bitu)mirrorFlip, (Bitu)srcBpp, (Bitu)dstBpp, localPal));
//#if UseAltic
	ATLASSERT((srcBpp == IMG_4_BPP || srcBpp == IMG_8_BPP) && (dstBpp == IMG_4_BPP || dstBpp == IMG_8_BPP));
	ATLASSERT(colorkey == -1 || (0 <= colorkey && colorkey <= 255));
	U16 dstibpp = IMG_8_BPP, dsticx = 320, dsticy = 200;
	if (dst != NULL) {
		dstibpp = READ_IMGBUFF_BPP(dst);	// READ_I16(dst,-6);
		dsticx = READ_IMGBUFF_WIDTH(dst);	// READ_I16(dst,-4);
		dsticy = READ_IMGBUFF_HEIGHT(dst);	// READ_I16(dst,-2);
	}
	ATLASSERT(dstBpp == dstibpp);
	ATLASSERT((dstBpp == IMG_4_BPP) ? RUp2(dstPitch) == RUp2(dsticx) : true);
//	ATLASSERT((dstBpp == IMG_8_BPP) ? (dstPitch == dsticx) : true);
	ATLASSERT(rc->x >= 0);
	ATLASSERT(rc->y >= 0);
	ATLASSERT(rc->cx >= 0);
	ATLASSERT(rc->cy >= 0);
	if (!SkCodeParam::bUseSuperInfoEye && !SkCodeParam::bDM2V5Mode)	// for some "debug" mode, we bypass the check of rectangles bounds
	ATLASSERT((0 +rc->x +RUp2(dsticx) * (rc->y +rc->cy -1) +rc->cx) <= (RUp2(dsticx) * dsticy));
	if (SkCodeParam::bUsePowerDebug && (!CheckSafePointer((void*)src) /*|| dst == NULL *//*|| rc == NULL */ /*|| localPal == NULL*/))
		return;
//#endif

	ENTER(0);
	U16 di = srcPitch;
	U16 si = colorkey;
	if (dst == NULL) {
		IBMIO_BLIT_TO_SCREEN_xTO8BPP(src, rc, srcx, srcy, di, si, localPal, srcBpp) CALL_IBMIO;
	}
	else {
		if (srcBpp == IMG_4_BPP) {
			if (dstBpp == IMG_4_BPP) {
				FIRE_BLIT_TO_MEMORY_4TO4BPP(src, dst, rc, srcx, srcy, srcPitch, dstPitch, colorkey, mirrorFlip);
			}
			else {
				FIRE_BLIT_TO_MEMORY_4TO8BPP(src, dst, rc, srcx, srcy, srcPitch, dstPitch, colorkey, mirrorFlip, localPal);
			}
		}
		else {
			if (localPal == NULL) {
				FIRE_BLIT_TO_MEMORY_8TO8BPP(src, dst, rc, srcx, srcy, srcPitch, dstPitch, colorkey, mirrorFlip);
			}
			else {
				_44c8_0f29(src, dst, rc, srcx, srcy, di, dstPitch, si, mirrorFlip, localPal);
			}
		}
	}
	return;
}



//^44C8:0C74
void SkWinCore::FIRE_BLIT_TO_MEMORY_8TO8BPP(
	const void *src,
	void *dst,
	SRECT *rc,
	i16 srcx,
	i16 srcy,
	i16 srcPitch,
	i16 dstPitch,
	i16 colorkey,
	i16 mirrorFlip
	)
{
#if UseAltic
	SkD((DLV_GUI, "GUI: 8To8 (%p,%p,(%3d,%3d,%3d,%3d),%3d,%3d,%3d,%3d,%2d,%d)\n"
		, src, dst
		, (Bitu)rc->x, (Bitu)rc->y, (Bitu)rc->cx, (Bitu)rc->cy, (Bitu)srcx, (Bitu)srcy
		, (Bitu)srcPitch, (Bitu)dstPitch, (Bitu)colorkey, (Bitu)mirrorFlip));

	Pic8 sp;
	sp.buff = (U8 *)src; sp.ox = srcx; sp.oy = srcy; sp.pitch = srcPitch; sp.cy = 0; sp.dx = 1; sp.dy = 1;
	Pic8 dp;
	dp.buff = (U8 *)dst; dp.ox = rc->x; dp.oy = rc->y; dp.pitch = dstPitch; dp.cy = 0; dp.dx = 1; dp.dy = 1;
	BlitSvc(sp, dp, rc->cx, rc->cy, colorkey);
#else
	ENTER(6);
	_4976_5e64 = reinterpret_cast<U8 *>(const_cast<void *>(src));
	glbBlit2MemDest = reinterpret_cast<U8 *>(dst);
	U16 bp04 = rc->cx;
	U16 bp02 = rc->cy;
	if (bp04 <= 0 && bp02 <= 0)
		return;
	U16 di = rc->y * dstPitch +rc->x;
	U16 si;
	U16 bp06;
	switch (mirrorFlip) {
		case 3://^_0cde
			si = srcy * srcPitch;
			for (bp06 = 0; bp06 < bp02; si += srcPitch, di += dstPitch, bp06++) {
				if (colorkey < 0) {
					_44c8_0b8d(si, di, bp04);
				}
				else {
					_44c8_0bc5(si, di, bp04, U8(colorkey));
				}
			}
			break;
		case 2://^_0d2a
			si = (srcy +bp02 -1) * srcPitch +srcx;
			for (bp06 = 0; bp06 < bp02; si -= srcPitch, di += dstPitch, bp06++) {
				if (colorkey < 0) {
					_44c8_0b8d(si, di, bp04);
				}
				else {
					_44c8_0bc5(si, di, bp04, U8(colorkey));
				}
			}
			break;
		case 1://^_0d7a
			si = srcy * srcPitch +srcx;
			for (bp06 = 0; bp06 < bp02; si += srcPitch, di += dstPitch, bp06++) {
				if (colorkey < 0) {
					_44c8_0bf8(si, di, bp04);
				}
				else {
					_44c8_0c3c(si, di, bp04, U8(colorkey));
				}
			}
			break;
		default: //^_0dc3
			si = (srcy +bp02 -1) * srcPitch +srcx;
			for (bp06 = 0; bp06 < bp02; si -= srcPitch, di += dstPitch, bp06++) {
				if (colorkey < 0) {
					_44c8_0bf8(si, di, bp04);
				}
				else {
					_44c8_0c3c(si, di, bp04, U8(colorkey));
				}
			}
			break;
	}
	return;
#endif
}




//^00EB:03D5
void SkWinCore::IBMIO_WAIT_VSYNC()
{
#ifdef __DJGPP__
	while ((SKDOS_IMPORTB(0x03DA) & 0x08) != 0);
	while ((SKDOS_IMPORTB(0x03DA) & 0x08) == 0);
#else
	skWinApp->skwin_Sleep(1000 / 50);
#endif // __DJGPP__
}

//^00EB:045D
void SkWinCore::IBMIO_UPDATE_PALETTE_SET()
{
	IBMIO_WAIT_VSYNC();
	SKDOS_OUTPORTB(0x03C8, 0);
	U16 iColIdx = 0;	// si
	for (iColIdx = 0; iColIdx < 256; iColIdx++) {
		U8 iCol1R = glbPaletteRGB[iColIdx][0];	// bp01
		U8 iCol2G = glbPaletteRGB[iColIdx][1];	// bp02
		U8 iCol3B = glbPaletteRGB[iColIdx][2];	// bp03
		//bp01 = bp02 = bp03 = ((si%16)+1) << 4;
		//outportb(0x03c8, si);
		SKDOS_OUTPORTB(0x03C9, iCol1R);
		SKDOS_OUTPORTB(0x03C9, iCol2G);
		SKDOS_OUTPORTB(0x03C9, iCol3B);
	}
	skWinApp->setRGB6Palette((X8*) glbPaletteRGB);
	return;
}

//^00EB:0654
void SkWinCore::IBMIO_SELECT_PALETTE_SET(U8 iPaletteSet) //#DS=04BF
{
	LOADDS(0x0c48);
	iPaletteSet=1;
	if (iPaletteSet == 0) {
		IBMIO_WAIT_VSYNC();
		//outportb(0x03c8, 0);
		U16 si = 0;
		/*
		for (; si < 256; si++) {
			//^00EB:0674
			outportb(0x03c9, (si%16)>>3);
			outportb(0x03c9, (si%16)>>3);
			outportb(0x03c9, (si%16)>>3);
			//^00EB:067C
		}*/
	}
	else if (iPaletteSet == 1) {
		IBMIO_UPDATE_PALETTE_SET();
	}
	glbUpdatePalette = iPaletteSet;
	return;
}

//^44C8:1BD4
void SkWinCore::FIRE_SELECT_PALETTE_SET(U8 iPaletteSet)
{
	IBMIO_SELECT_PALETTE_SET(iPaletteSet) CALL_IBMIO;
}

//^00EB:0353
void SkWinCore::IBMIO_FILL_SCREEN_LINE(U16 offDst, U16 fill, U16 size) //#DS=04BF?
{
	// TODO: Alt impl
	memset(_04bf_0e34 + offDst, fill, size);
}

//^00EB:0383
void SkWinCore::IBMIO_FILL_RECT_SCREEN(SRECT *rc, U16 fill) //#DS=04BF
{
	LOADDS(0x0c48);
	_04bf_0e34 = pbVram;
	U16 di = rc->y * 320 + rc->x;
	U16 si = 0;
	for (; rc->cy > si; di += 320, si++) {
		IBMIO_FILL_SCREEN_LINE(di, fill, rc->cx);
	}
#if UseAltic
	//skwin.UpdateRect(rc->x, rc->y, rc->cx, rc->cy);
	skWinApp->renderScreen(vram, rc->x, rc->y, rc->cx, rc->cy);
	MessageLoop(false);
#endif
	return;
}

//^44C8:1233
void SkWinCore::FIRE_FILL_4BPP_PICT_LINE(U16 offDst, U16 fill, U16 size)
{
	ATLASSERT(size != 0);

	U8* di = glbBlit2MemDest;
	U16 bx = offDst;
	U16 cx = size;
	U8 ah = Bit8u(fill);
	U8 al;
	bool carry = (bx & 1) ? true : false;
	bx >>= 1;
	di += bx;
	if (carry) {
		al = *di;
		al &= 0xf0;
		al |= ah;
		stosb(di, al);
		cx--;
		if (cx == 0)
			return;
	}
	al = ah;
	al <<= 4;
	al |= ah;
	ah = al;
	{
		bool carry = (cx & 1) ? true : false;
		cx >>= 1;
		{
			bool carry = (cx & 1) ? true : false;
			cx >>= 1;
			if (carry) {
				stosb(di, al);
			}
			while (cx != 0) {
				stosb(di, al);
				stosb(di, ah);
				cx--;
			}
		}
		if (carry) {
			al = *di;
			al &= 0x0f;
			ah &= 0xf0;
			al |= ah;
			stosb(di, al);
		}
	}
	return;
}

//^44C8:1A1E
void SkWinCore::FIRE_FILL_RECT_4BPP_PICT(Bit8u *buff, SRECT *rc, U16 fill, U16 width)
{
	glbBlit2MemDest = buff;
	width = (width + 1) & 0xfffe;
	U16 di = rc->y * width + rc->x;
	U16 si = 0;
	for (; rc->cy > si; di += width, si++) {
		FIRE_FILL_4BPP_PICT_LINE(di, fill, rc->cx);
	}
	return;
}

//^44C8:127E
void SkWinCore::FIRE_FILL_8BPP_PICT_LINE(U16 offDst, U16 fill, U16 size)
{
	U8* di = glbBlit2MemDest + offDst;
	U16 cx = size;
	U8 al = Bit8u(fill);
	U16 dx = cx;
	dx &= 1;
	if (dx != 0) {
		stosb(di, al);
		cx--;
	}
	dx = cx;
	cx >>= 1;
	if (cx != 0) {
		U16 ax = al | (al << 8);
		while (cx != 0) {
			stosw(di, ax);
			cx--;
		}
	}
	dx &= 1;
	if (dx != 0) {
		stosb(di, al);
	}
	return;
}

//^44C8:19D5
void SkWinCore::FIRE_FILL_RECT_8BPP_PICT(Bit8u *buff, SRECT *rc, U16 fill, U16 width)
{
	glbBlit2MemDest = buff;
	U16 di = rc->y * width + rc->x;
	U16 si = 0;
	for (; rc->cy > si; di += width, si++) {
		FIRE_FILL_8BPP_PICT_LINE(di, fill, rc->cx);
	}
	return;
}

//^44C8:1A71
void SkWinCore::FIRE_FILL_RECT_ANY(Bit8u *buff, SRECT* rc, U16 fill, U16 width, U16 bpp)
{
	U16 si = fill;
	if (buff == NULL) {
		IBMIO_FILL_RECT_SCREEN(rc, si) CALL_IBMIO;
	}
	else if (bpp == IMG_4_BPP) {
		FIRE_FILL_RECT_4BPP_PICT(buff, rc, si, width);
	}
	else {
		FIRE_FILL_RECT_8BPP_PICT(buff, rc, si, width);
	}
	return;
}

//^44C8:1DAF
void SkWinCore::FIRE_FILL_SCREEN_RECT(U16 iRectNo, U8 fill)
{
	SRECT xRect; // bp08
	SRECT* xExpRect = QUERY_EXPANDED_RECT(iRectNo, &xRect);
	FIRE_FILL_RECT_ANY(NULL, xExpRect, fill, xRect.cx, IMG_8_BPP);
}

//^0AAF:0000
void SkWinCore::FIRE_FADE_SCREEN(U16 fadeOutIfTrue) //#DS=4976
{
	if (cd.gg.glbGameHasEnded != 0) {
		if (fadeOutIfTrue != 0) {
			FIRE_SELECT_PALETTE_SET(0);
			FIRE_FILL_SCREEN_RECT(2, 0);
		}
		else {
			FIRE_SELECT_PALETTE_SET(1);
		}
	}
	return;
}

//^44C8:1D8C
void SkWinCore::FIRE_FILL_BACKBUFF_RECT(SRECT* xRect, U8 fill)
{
    FIRE_FILL_RECT_ANY(
		glbBackBuffViewport,
		xRect,
		fill,
		_4976_00f6,
		IMG_8_BPP
		);
	return;
}

//^44C8:1BA2
void SkWinCore::FIRE_WAIT_VSYNC()
{
	IBMIO_WAIT_VSYNC() CALL_IBMIO;
}

//^0CD5:000E
void SkWinCore::WAIT_SCREEN_REFRESH()
{
	FIRE_WAIT_VSYNC();
}


//^00EB:02EB
void SkWinCore::IBMIO_BLIT_TO_SCREEN_xTO8BPP(
	const void *src,
	SRECT *rc,
	i16 srcx,
	i16 srcy,
	i16 srcPitch,
	i16 colorkey,
	const void *localPal,
	i16 srcBpp
	)
{
#if UseAltic
	SkD((DLV_GUI, "GUI: xToScr (%p,(%3d,%3d,%3d,%3d),%3d,%3d,%3d,%2d,%p,%d)\n"
		, src
		, (Bitu)rc->x, (Bitu)rc->y, (Bitu)rc->cx, (Bitu)rc->cy
		, (Bitu)srcx, (Bitu)srcy, (Bitu)srcPitch, (Bitu)colorkey, localPal, (Bitu)srcBpp));

	Pic8 dp;
	dp.buff = (U8 *)vram; dp.ox = rc->x; dp.oy = rc->y; dp.pitch = 320; dp.cy = 200; dp.dx = 1; dp.dy = 1;

	if (srcBpp == IMG_8_BPP) {
		Pic8 sp;
		sp.buff = (U8 *)src; sp.ox = srcx; sp.oy = srcy; sp.pitch = srcPitch; sp.cy = 0; sp.dx = 1; sp.dy = 1;
		BlitSvc(sp, dp, rc->cx, rc->cy, colorkey);
	}
	else {	// IMG_4_BPP
		Pic4 sp;
		sp.buff = (U8 *)src; sp.ox = srcx; sp.oy = srcy; sp.cx = srcPitch; sp.pitch = (srcPitch +1) / 2; sp.cy = 0; sp.dx = 1; sp.dy = 1; sp.pal = (U8 *)localPal;
		BlitSvc(sp, dp, rc->cx, rc->cy, colorkey);
	}

	//skwin.UpdateRect(rc->x, rc->y, rc->cx, rc->cy);
	skWinApp->renderScreen(vram, rc->x, rc->y, rc->cx, rc->cy);
	MessageLoop(false);
#else
	printf("IBMIO_BLIT_TO_SCREEN_xTO8BPP\n");
	Unr();
#endif
}
//^44C8:05AE
void SkWinCore::FIRE_BLIT_TO_MEMORY_4TO4BPP(
	const void *src,
	void *dst,
	SRECT *rc,
	i16 srcx,
	i16 srcy,
	i16 srcPitch,
	i16 dstPitch,
	i16 colorkey,
	i16 mirrorFlip
	)
{
#if UseAltic
	SkD((DLV_GUI, "GUI: 4To4 (%p,%p,(%3d,%3d,%3d,%3d),%3d,%3d,%3d,%3d,%2d,%d)\n"
		, src, dst
		, (Bitu)rc->x, (Bitu)rc->y, (Bitu)rc->cx, (Bitu)rc->cy, (Bitu)srcx, (Bitu)srcy
		, (Bitu)srcPitch, (Bitu)dstPitch, (Bitu)colorkey, (Bitu)mirrorFlip));

	static const U8 thru[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

	Pic4 sp;
	sp.buff = (U8 *)src; sp.ox = srcx; sp.oy = srcy; sp.cx = srcPitch; sp.pitch = (srcPitch +1) / 2; sp.cy = 0; sp.dx = 1; sp.dy = 1; sp.pal = thru;
	sp.Apply(mirrorFlip & 1, mirrorFlip & 2);
	Pic4 dp;
	dp.buff = (U8 *)dst; dp.ox = rc->x; dp.oy = rc->y; dp.pitch = (dstPitch +1) / 2; dp.cy = 0; dp.dx = 1; dp.dy = 1; dp.pal = thru;
	BlitSvc(sp, dp, rc->cx, rc->cy, colorkey);
#else
	printf("FIRE_BLIT_TO_MEMORY_4TO4BPP\n");
	Unr();
#endif
}

//^44C8:09D7
void SkWinCore::FIRE_BLIT_TO_MEMORY_4TO8BPP(
	const void *src,
	void *dst,
	SRECT *rc,
	i16 srcx,
	i16 srcy,
	i16 srcPitch,
	i16 dstPitch,
	i16 colorkey,
	i16 mirrorFlip,
	const U8 *localPal
	)
{
#if UseAltic
	SkD((DLV_GUI, "GUI: 4To8 (%p,%p,(%3d,%3d,%3d,%3d),%3d,%3d,%3d,%3d,%2d,%d,%p)\n"
		, src, dst
		, (Bitu)rc->x, (Bitu)rc->y, (Bitu)rc->cx, (Bitu)rc->cy, (Bitu)srcx, (Bitu)srcy
		, (Bitu)srcPitch, (Bitu)dstPitch, (Bitu)colorkey, (Bitu)mirrorFlip, localPal));

	ATLASSERT(localPal != NULL);
	FIRE_UPDATE_BLIT_PALETTE(localPal);

	Pic4 sp;
	sp.buff = (U8 *)src; sp.ox = srcx; sp.oy = srcy; sp.cx = rc->cx; sp.pitch = (srcPitch +1) / 2; sp.cy = rc->cy; sp.dx = 1; sp.dy = 1; sp.pal = (U8 *)localPal;
	sp.Apply(mirrorFlip&1, mirrorFlip&2);
	Pic8 dp;
	dp.buff = (U8 *)dst; dp.ox = rc->x; dp.oy = rc->y; dp.pitch = dstPitch; dp.cy = 0; dp.dx = 1; dp.dy = 1;
	BlitSvc(sp, dp, rc->cx, rc->cy, colorkey);
#else
	printf("FIRE_BLIT_TO_MEMORY_4TO8BPP\n");
	Unr();
#endif
}


//^00EB:069A
// _00eb_069a renamed VIDEO_MEM_MOVE
void SkWinCore::VIDEO_MEM_MOVE(SRECT *prc, i16 yy)
{
	ENTER(0);
	LOADDS(0x0C48);
	i16 si = prc->y + yy;
	while (prc->y + prc->cy - 1 >= si) {
		MOVE_MEMORY(
			&pbVram[si * 320],
			&pbVram[(si - yy) * 320],
			prc->x + prc->cx - 1 -prc->x + 1
			);
	}
	return;
}






//------------------------------------------------------------------------------
// ANIM - LEVEL

//^0759:08E7
// SPX: _0759_08e7 renamed ANIM_MAIN
int SkWinCore::ANIM_MAIN(i16 argc, const char **argv, char **env) // #DS=089C
{
	ENTER(214);
	X16 bp2a = 0;
	X16 bp2e = 0;
	X16 bp32 = 0;
	X16 bp34 = 0;
	X16 bp36 = 0;
	X16 bp42 = 0;
	X16 bp44 = 0;
	X16 bp46 = 0;
	X16 bp48 = 0;
	X16 bp4a = 0;
	X16 bp5e = 0;
	X16 bp60 = 0;
	U8 bp69 = 0xf;
	X16 bp6c = 0;
	char bp0096[0x0096 -0x6e] = {0};
	i16 si = 1;
	U8 *bp64;
	for (; si < argc; si++) {
		if (argv[si][0] == '+' && ANIM_TOUPPER(argv[si][1]) == 'A') { // TODO: check whether this is toupper
			switch (ANIM_TOUPPER(argv[si][2])) {
			case 'B'://^09A1
				bp44 = 1;
				break;
			case 'L'://^09A9
				bp44 = 1;
				bp32 = 1;
				break;
			case 'D'://^09B6
				bp34 = 1;
				break;
			case 'H'://^09BE
				bp46 = 1;
				break;
			case 'E'://^09C6
				bp48 = 1;
				break;
			case 'S'://^09CE
				bp4a = 1;
				break;
			case 'M'://^09D6
				bp5e = 1;
				break;
			case 'F'://^09DE
				{
				X16 bp30;
				if (IS_NEGATIVE(bp30 = ANIM_FILE_OPEN(&argv[si][3])) != 0)
					break;
				U32 bp68 = ANIM_GET_FILE_SIZE(bp30);
				bp64 = ANIM_farmalloc(bp68);
				if (bp64 != NULL) {
					ANIM_READ_HUGE_FILE(bp30, bp68, bp64);
					bp60 = 1;
				}
				ANIM_FILE_CLOSE(bp30);
				break;
				}
			case 'V'://^0A5B
				bp69 = _069a_03fc(&argv[si][3]);
				if (bp69 > 15) {
					bp69 = 15;
				}
				break;
			case 'O'://^0A86
				bp6c = _069a_03fc(&argv[si][3]);
				if (bp6c > 0xfe) {
					bp6c = 0xe;
				}
				break;
			case 'C'://^0ADD
			case 'G'://^0ADD
			case 'I'://^0ADD
			case 'J'://^0ADD
			case 'K'://^0ADD
			case 'N'://^0ADD
			case 'P'://^0ADD
			case 'Q'://^0ADD
			case 'R'://^0ADD
			case 'T'://^0ADD
			case 'U'://^0ADD
			default:
				break;
			}
		}
		else if (bp36 == 0) {
			ANIM_STRCPY(bp0096, argv[si]);
			bp36 = 1;
		}
	}
	_0759_0126();
	_0759_06db();
	while (IS_THERE_KEY_INPUT_1() != 0) {
		UI_CONSUME_KEYBOARD_INPUT();
	}
	X16 bp30;
	if (bp0096[0] == 0 || IS_NEGATIVE(bp30 = ANIM_FILE_OPEN(bp0096)) != 0) {
#if UseAltic
		if (IS_NEGATIVE(ANIM_FILE_OPEN(_089c_00d8)) != 0) {
			return (0);
		}
#else
		if (IS_NEGATIVE(ANIM_FILE_OPEN(_089c_00d8)) == 0) {
			return (0);
		}
#endif
	}
	U8 *bp08 = ANIM_farmalloc(32000);
	if (bp08 == NULL) {
		return (0);
	}
	U8 *bp10;
	X16 bp50;
	if (IS_SCARD_PRESENT() CALL_IBMIO != 0 && (bp10 = ANIM_farmalloc(32000)) != NULL) {
		if (_RELOAD_SOUND_BUFFER(bp10, 32000, PLAYBACK_FREQUENCY) CALL_IBMIO != 0) {
			bp50 = 1;
		}
		else {
			bp50 = 0;
			ANIM_farfree(bp10);
		}
	}
	else {
		bp50 = 0;
	}
	U32 bp14 = ANIM_GET_FILE_SIZE(bp30);
	U8 *bp04;
	X16 di;
	if (bp5e != 0 || ANIM_farcoreleft() < bp14) {
		X32 bp4e = _0759_07f2(_089c_034c);
		if (bp4e < bp14) {
			_0759_0855();
			return (0);
		}
		U32 bp54 = bp14;
		U32 bp58 = 0;
		for (; bp54 <= 0; ) {
			U16 bp5a;
			if (bp54 > 16000) {
				bp5a = 16000;
			}
			else {
				bp5a = U16(bp54);
			}
			ANIM_READ_HUGE_FILE(bp30, bp5a, _0759_0869(bp58));
			bp58 += bp5a;
			bp54 -= bp5a;
		}
		di = 1;
	}
	else {
		di = 0;
		bp04 = ANIM_farmalloc(bp14);
		if (bp04 == NULL) {
			return (0);
		}
		ANIM_READ_HUGE_FILE(bp30, bp14, bp04);
	}
	ANIM_FILE_CLOSE(bp30);
	if (bp4a != 0)
		_0759_06b5();
	ANIM_SELECT_PALETTE_SET(0);
	ANIM_SET_BLACK_SCREEN();
	X32 bp18 = 0; // anim read offset?
	X32 bp24 = 0;
	X16 bp2c = 0;
	X16 bp5c = 1;
	if (bp60 != 0) {
#if UseAltic
		ATLASSERT(false);
#else
		_xxxx_xxxx(bp64, 0, bp69 << 4, 0);
#endif
	}

	// I thank to Christophe Fontanel for The Animations file format section!
	// http://dmweb.free.fr/?q=node/216

	// The below identifications are brought from above file spec site.

	do {
		U8 *bp0c = (di != 0)
			? _0759_0869(bp18)
			: bp04 +bp18;
		X16 bp28 = bp0c[5] | (bp0c[4] << 8); // 1 word (big endian): Item attribute
		X32 bp1c = bp18 +U32((bp0c[2] << 8) | bp0c[3]) +6; // next memory position
		X16 bp6e = (bp0c[1] << 8) | bp0c[0]; // 2 bytes: Item type
		U8 *bp3a;
		U16 bp40;
		U16 bp3e;
		U16 bp3c;
		X32 bp20;
		U16 bp26;
		switch (bp6e) {
		case 0x4157://^0E21 // "WA"
			if (bp28 == 1) {
				ANIM_WA_0759_0792();
			}
			break;
		case 0x4453://^0E33 // "SD"
			if (bp5c != 0) {
				if (di != 0) {
					_0759_0869(_089c_02c0[bp42++] = bp18 +6);
				}
				else {
					_089c_02c0[bp42] = bp18 +6;
					bp3a = bp04;
					bp42++;
					break;
				}
				bp40 = (bp3a[0] << 8) | (bp3a[1]);
				si = 0;
				for (; U16(si) < bp40; si++) {
					bp3a[si +2] = bp3a[si +2] +0x80;
				}
				if (di == 0)
					break;
				bp0c = _0759_0869(bp18);
			}
			break;
		case 0x4F53://^0EF0 // "SO"
			bp3e = 0xff;
			bp3c = 5500;
			bp3a = (di != 0)
				? _0759_0869(_089c_02c0[bp28])
				: &bp04[_089c_02c0[bp28 -1]];
			ANIM_SO_0759_0739(bp3a, bp3e, bp3c);
			if (di != 0) {
				bp0c = _0759_0869(bp18);
			}
			break;
		case 0x4C44://^0F64 // "DL"
		case 0x4E45://^0F64 // "EN"
			_089c_0344 = bp28;
			if (bp0c[6] == 0xff && bp0c[7] == 0x81) {
				ANIM_DECODE_IMG1(&bp0c[8], bp08);
			}
			else {
				ANIM_DECODE_IMG1(&bp0c[6], bp08);
			}
			ANIM_SELECT_PALETTE_SET(1);
			bp2a++;
			while (_089c_0344 > 0 && bp34 == 0) {
#if UseAltic
				for (U32 w=0; w < 320*200; w += 2) {
					vram[w +0] = bp08[w >> 1] >> 4;
					vram[w +1] = bp08[w >> 1] & 15;
				}
				//skwin.UpdateRect(0, 0, 320, 200);
				skWinApp->renderScreen(vram, 0, 0, 320, 200);
				MessageLoop(true, true); // anim
#else
				ATLASSERT(false);
#endif
			}
			if (bp2e != 0) {
				bp2e = 0;
				bp1c = bp20;
			}
			break;
		case 0x4C50://^1013 // "PL"
			bp26 = 0;
			for (si = 0; si < 64; si++) {
				if (bp0c[si +8] != _089c_0090[si])
					bp26 = 1;
				_089c_0090[si] = bp0c[si +8];
			}
			if (bp26 != 0) { // is palette updated?
				skxxxj bp00d6[16];
				for (si = 0; si < 64; si += 4) {
					bp00d6[si >> 2].b0 = bp0c[si + 8];
					bp00d6[si >> 2].b1 = bp0c[si + 9] << 4;
					bp00d6[si >> 2].b2 = bp0c[si +10] << 4;
					bp00d6[si >> 2].b3 = bp0c[si +11] << 4;
				}
				ANIM_SELECT_PALETTE_SET(0);
				_0759_0688(bp00d6, 0);
			}
			break;
		case 0x4F46://^1160 // "FO"
			bp0c = (di != 0)
				? _0759_0869(bp1c)
				: &bp04[bp1c];
			_089c_025a[bp2c +1].w4 = bp28;
			_089c_025a[bp2c +1].dw0 = bp1c +((bp0c[2] << 8) | bp0c[3]) +6;
			bp2c++;
			if (di != 0)
				goto _12ce;
			bp0c = &bp04[bp18];
			break;
		case 0x454E://^121E // "NE"
			bp26 = -- _089c_025a[bp2c].w4;
			if (bp26 > 0 && bp26 < 10) {
				bp2e = 1;
				bp20 = _089c_025a[bp2c].dw0;
				break;
			}
			bp2c--;
			if (di != 0)  {
				bp0c = _0759_0869(bp1c);
			}
			bp1c = bp1c +((bp0c[2] << 8) | bp0c[3]) +6;
			if (di != 0) {
_12ce:
				bp0c = _0759_0869(bp18);
			}
		}
		bp18 = bp1c;
		if (bp32 != 0 && bp18 >= bp14) {
			bp5c = 0;
			bp18 = bp24;
		}
		if (IS_THERE_KEY_INPUT_1() != 0 && bp34 == 0 && bp44 != 0) {
			UI_CONSUME_KEYBOARD_INPUT();
			break;
		}
	} while (bp18 < bp14);
	if (bp60 != 0) {
#if UseAltic
		ATLASSERT(false);
#else
		_xxxx_xxxx(bp6c);
#endif
	}
	if (bp48 != 0) {
		_0759_06b5();
	}
	if (bp46 == 0) {
		ANIM_SELECT_PALETTE_SET(0);
	}
	if (di != 0) {
		_0759_0855();
	}
	else {
		ANIM_farfree(bp04);
	}
	if (bp60 != 0) {
		_01b0_1983_AUDIO() CALL_IBMIO;
		ANIM_farfree(bp64);
	}
	if (bp50 != 0) {
		_01b0_18d3_AUDIO(0) CALL_IBMIO;
		ANIM_farfree(bp10);
	}
	return (0);
	//return 0;
}

//^069A:0000
int SkWinCore::ANIM_BOOTSTRAP_SWOOSH() // #DS=089C
{
	// 08D2:0FE2  F6 0F D2 08 FE 0F D2 08 05 10 D2 08 09 10 D2 08  EÒ.þ.Ò...Ò...Ò.
	// 08D2:0FF2  00 00 00 00 43 3A 5C 41 4E 49 4D 00 73 77 6F 6F  ....C:\ANIM.swoo
	// 08D2:1002  73 68 00 2B 70 6D 00 2B 73 62 00 08 00 00 5A 00  sh.+pm.+sb....Z.
	// 08D2:1012  00 2B 96 00 00 00 00 00 00 00 00 00 00 00 00 00  .+E............
	const char *argv[] = {"ANIM", "swoosh", "+pm", "+sb"};
	return (ANIM_MAIN(4, argv, NULL));
}

//^069A:0000
int SkWinCore::ANIM_BOOTSTRAP_TITLE() // #DS=089C
{
	// 08D2:0FCC  EC 0F D2 08 F4 0F D2 08 FA 0F D2 08 FE 0F D2 08  EÒ.EÒ.EÒ.þ.Ò.
	// 08D2:0FDC  02 10 D2 08 06 10 D2 08 0A 10 D2 08 00 00 00 00  ..Ò...Ò...Ò.....
	// 08D2:0FEC  43 3A 5C 41 4E 49 4D 00 74 69 74 6C 65 00 2B 61  C:\ANIM.title.+a
	// 08D2:0FFC  68 00 2B 61 73 00 2B 61 62 00 2B 70 6D 00 2B 73  h.+as.+ab.+pm.+s
	// 08D2:100C  62 00 00 00 5A 00 00 2B 96 00 00 00 00 00 00 00  b...Z..+E......
	const char *argv[] = {"ANIM", "title", "+ah", "+as", "+ab", "+pm", "+sb"};
	return (ANIM_MAIN(7, argv, NULL));
}



//^069A:035B
X16 SkWinCore::ANIM_TOUPPER(X16 iChar)
{
	ENTER(0);
	i16 iLocalChar = iChar;
	if (iLocalChar == -1)
		return -1;
	// Note: in tblASCIIFlags, flag 8 = lowercase, flag 4 = uppercase
	if ((tblASCIIFlags[U8(iLocalChar)] & 8) != 0)	// if it's lowercase
		return U8(iLocalChar) - 32;		// 'a' - 32 = 'A'
	return U8(iLocalChar);
}









//^0759:06A1
// _0759_06a1 renamed ANIM_SELECT_PALETTE_SET
void SkWinCore::ANIM_SELECT_PALETTE_SET(U8 iPaletteSet)
{
	ENTER(0);
	IBMIO_SELECT_PALETTE_SET(iPaletteSet) CALL_IBMIO;
	return;
}

//^00EB:04BC
void SkWinCore::_00eb_04bc(skxxxj *xx, U16 yy) //#DS=04BF
{
	ENTER(0);
	LOADDS(0x0c48);
	X16 si = 0;
	X16 di = yy << 4;
	for (si = 0; si < 16; si++) {
		glbPaletteRGB[(si +di)][0] = U8(xx[si].b1 >> 2);
		glbPaletteRGB[(si +di)][1] = U8(xx[si].b2 >> 2);
		glbPaletteRGB[(si +di)][2] = U8(xx[si].b3 >> 2);
		if (yy == 0) {
			_04bf_0a50[si][0] = U8(xx[si].b1 >> 2);
			_04bf_0a50[si][1] = U8(xx[si].b2 >> 2);
			_04bf_0a50[si][2] = U8(xx[si].b3 >> 2);
		}
	}
	if (glbUpdatePalette == 1) {
		IBMIO_UPDATE_PALETTE_SET();
	}
	return;
}

//^0759:0688
void SkWinCore::_0759_0688(skxxxj *xx, U16 yy)
{
	ENTER(0);
	_00eb_04bc(xx, yy) CALL_IBMIO;
	return;
}


//^0759:065F
void SkWinCore::ANIM_SET_BLACK_SCREEN()
{
	ENTER(8);
	SRECT xScreenRect = glbRectFullScreen;	// rect 0,0,320,200
	IBMIO_FILL_RECT_SCREEN(&xScreenRect, 0);
	return;
}
void SkWinCore::ANIM_WA_0759_0792() {
	Unr(); // anim
}
void SkWinCore::ANIM_SO_0759_0739(U8 *xx, U16 yy, U16 zz) {
	Unr(); // anim
}
//^0759:0330
void SkWinCore::ANIM_DECODE_IMG1(U8 *xx, U8 *yy)
{
	ENTER(14);
	_089c_0348 = xx;
	_089c_0354 = yy;
	U16 bp02 = _089c_0348[1] | (_089c_0348[0] << 8); // image width
	U16 bp04 = _089c_0348[3] | (_089c_0348[2] << 8); // image height
	_089c_0348 += 4;
	X16 bp06;
	X16 bp08;
	bp08 = (bp06 = (bp02 +1)&0xfffe) * bp04;
	U16 di = 0;
	U16 bp0c = 0;
	do {
		i8 bp09 = (_089c_0348++)[0];
		X16 si;
		if ((bp09 & 0x80) == 0) {
			si = (bp09 >> 4) +1;
			ANIM_FILL_SEQ_4BPP(di, bp09&15, si);
			di += si;
			continue;
		}
		X16 bp0e = bp09 & 0x30;
		switch (bp0e) {
		case 0x00://^043C
			if ((bp09 & 0x40) == 0) {
				si = (_089c_0348[0]) +1;
				_089c_0348++;
			}
			else {
				si = (_089c_0348[1] | (_089c_0348[0] << 8)) +1;
				_089c_0348 += 2;
			}
			ANIM_FILL_SEQ_4BPP(di, bp09&15, si);
			di += si;
			continue;
		case 0x10://^0495
			if ((bp09 & 0x40) == 0) {
				si = (_089c_0348[0]) +1;
				_089c_0348++;
			}
			else {
				si = (_089c_0348[1] | (_089c_0348[0] << 8)) +1;
				_089c_0348 += 2;
			}
			if ((si & 1) != 0) {
				ANIM_SETPIXEL_SEQ_4BPP(di, bp09&15);
				di++;
				si--;
			}
			ANIM_BLIT_TO_MEMORY_ROW_4TO4BPP(0, di, si);
			_089c_0348 += si >> 1;
			di += si;
			continue;
		case 0x20://^051B
			if (bp0c < di) {
				IBMIO_BLIT_0759_0310(bp0c, di -bp0c);
			}
			si = ((bp09 >> 2)&16)|(bp09&15);
			if (si == 0x1d) {
				si = _089c_0348[0] +1;
				_089c_0348++;
			}
			else if (si == 0x1e) {
				si = _089c_0348[0] +0x101;
				_089c_0348++;
			}
			else if (si == 0x1f) {
				si = (_089c_0348[1] | (_089c_0348[0] << 8)) +1;
				_089c_0348 += 2;
			}
			else {
				si++;
			}
			di += si;
			bp0c = di;
			break;
		case 0x30://^05BB
			if ((bp09 & 0x40) == 0) {
				si = _089c_0348[0] +1;
				_089c_0348++;
			}
			else {
				si = (_089c_0348[1] | (_089c_0348[0] << 8)) +1;
				_089c_0348 += 2;
			}
			_0759_02c6(di, di -bp06, si);
			di += si;
			ANIM_SETPIXEL_SEQ_4BPP(di++, bp09&15);
			break;
		}
	} while (di < bp08);
	ATLASSERT(di <= bp08);
	if (bp0c < di) {
		IBMIO_BLIT_0759_0310(bp0c, di -bp0c);
	}
	return;
}

//^01B0:051A
// IBMIO_01b0_051a renamed IBMIO_IS_THERE_KEY_INPUT
U16 SkWinCore::IBMIO_IS_THERE_KEY_INPUT() //#DS=04BF
{
	ENTER(0);
	LOADDS(0x3083);
	if (glbDMode != 0 && glbTickStepActive != 0) {
		TICK_STEP_CHECK() INDIRECT_CALL;
	}
	return (glbUIKeyReadCount != 0) ? 1 : 0;
}

//^476D:05A9
// _476d_05a9 renamed IS_THERE_KEY_INPUT_2
U16 SkWinCore::IS_THERE_KEY_INPUT_2()
{
	ENTER(0);
	return IBMIO_IS_THERE_KEY_INPUT() CALL_IBMIO;
}


//^0759:072C
// _0759_072c renamed IS_THERE_KEY_INPUT_1
X16 SkWinCore::IS_THERE_KEY_INPUT_1()
{
	ENTER(0);
	return IBMIO_IS_THERE_KEY_INPUT() CALL_IBMIO;
}


//^069A:05FE
void SkWinCore::ANIM_farfree(U8 *buff)
{
	free(buff);
}

//^069A:040D
U32 SkWinCore::ANIM_farcoreleft()
{
#if UseAltic
	return 1024*1024; // Win32 supplies almost infinite memory pool compared to MS-DOS env.
#else
	ENTER(0);
	return _crt_farcoreleft();
#endif
}


//^069A:06EE
U8 *SkWinCore::ANIM_farmalloc(U32 size)
{
	return reinterpret_cast<U8 *>(malloc(size));
}
//^0759:002D
U16 SkWinCore::ANIM_READ_HUGE_FILE(X16 fh, U32 readSize, U8 *buff)
{
	ENTER(6);
	U8 *bp06 = buff;
	for (; readSize > 0; ) {
		U16 bp02;
		if (readSize > 32768) {
			bp02 = 0x8000;
		}
		else {
			bp02 = U16(readSize);
		}
		if (fset.fileRead(fh, bp02, bp06) != bp02) {
			return 0;
		}
		readSize -= bp02;
		bp06 += bp02;
	}
	return 1;
}
//^0759:0021
void SkWinCore::ANIM_FILE_CLOSE(X16 fh)
{
	ENTER(0);
	fset.fileClose(fh);
	return;
}
U8 SkWinCore::_069a_03fc(const char *xx)
{
	return Unr(), 0; // anim
}
//^069A:048C
char *SkWinCore::ANIM_STRCPY(char *xx, const char *yy)
{
	return strcpy(xx, yy);
}


//^0759:000C
X16 SkWinCore::ANIM_FILE_OPEN(const char *filename)
{
	ENTER(0);
	return fset.fileOpen(reinterpret_cast<const char *>(filename));
}

//^0759:00D6
U32 SkWinCore::ANIM_GET_FILE_SIZE(X16 fh)
{
#if UseAltic
	return fset.fileGetSize(fh);
#else
	return Unr(),0;
#endif
}





//^01B0:1FFC
X16 SkWinCore::_01b0_1ffc(X16 xx)
{
#if UseAltic
	return 1;
#else
	// TODO: ioctl
	ATLASSERT(false);
	return 1;
#endif
}

//^476D:02E0
X16 SkWinCore::_476d_02e0(X16 xx)
{
	ENTER(0);
	return _01b0_1ffc(xx) CALL_IBMIO;
}

//^01B0:20CA
// SPX: _01b0_20ca renamed IBMIO_GET_DRIVE_NAME
void SkWinCore::IBMIO_GET_DRIVE_NAME(i16 iDriveNo, U8* sDriveString)
{
	// This writes "A:", "C:" etc ...
	ENTER(0);
	if (iDriveNo >= 0 && iDriveNo < 0x19) {
		sDriveString[0] = U8(iDriveNo) + 0x40;	// to get uppercase letter
		sDriveString[1] = ':';
		sDriveString[2] = 0;
	}
	return;
}

//^476D:04F4
// SPX: _476d_04f4 renamed IBMIO_GET_DRIVE_NAME_2
void SkWinCore::IBMIO_GET_DRIVE_NAME_2(i16 iDriveNo, U8* sDriveString)
{
	ENTER(0);
	IBMIO_GET_DRIVE_NAME(iDriveNo, sDriveString);
	return;
}

//^476D:018A
// SPX: _476d_018a renamed IBMIO_GET_ALL_DRIVE_NAMES
void SkWinCore::IBMIO_GET_ALL_DRIVE_NAMES()
{
	ENTER(0);
	_4976_5eb2 = _getdrive();
	if (_476d_02e0(_4976_5eb2) == 1) {
		glbDriveNumber = _4976_5eb2;
		_4976_499e = 1;
		glbDriveNumber2 = _4976_5eb2;
		_4976_49a0 = 1;
		_4976_5eb4 = 1;
		_4976_5eb0 = 1;
	}
	else {
		_4976_5eb6 = _4976_5eb2;
		glbDriveNumber = _4976_5eb4 = _4976_5eb2 ^ 3;
		if (glbDriveNumber == 1) {
			if (_476d_02e0(2) != 0 && _476d_02e0(1) == _476d_02e0(2)) {
				glbDriveNumber2 = 2;
				_4976_5eb0 = 0;
			}
			else {
				glbDriveNumber2 = _4976_5eb2;
				_4976_5eb0 = 1;
			}
		}
		else if (glbDriveNumber == 2) {
			if (_476d_02e0(1) != 0 && _476d_02e0(1) == _476d_02e0(2)) {
				glbDriveNumber2 = 1;
				_4976_5eb0 = 0;
			}
			else {
				glbDriveNumber2 = _4976_5eb2;
				_4976_5eb0 = 1;
			}
		}
		else {
			glbDriveNumber2 = _4976_5eb2;
			_4976_5eb0 = 1;
		}
	}
	if (_4976_499e != 0) {
		IBMIO_GET_DRIVE_NAME_2(1, glbDriveNameStr);	// A: ?
	}
	else {
		IBMIO_GET_DRIVE_NAME_2(glbDriveNumber, glbDriveNameStr);
	}
	IBMIO_GET_DRIVE_NAME_2(glbDriveNumber2, glbDriveNameStr2);
	//SPX: 0x40 = 'A'-1
	strDirLetter[0] = glbDriveNumber + 0x40;
	strDirLetter2[0] = glbDriveNumber2 +0x40;
	return;
}

//^1031:050C
// SPX: _1031_050c renamed FIRE_1031_050c
void SkWinCore::FIRE_1031_050c()
{
	ENTER(0);
	if (glbShowItemStats != 0 || _4976_4bfe != 0 || _4976_4c3e != 0) {
		_4976_4c3e = 0;
		_4976_4bfe = 0;
		glbShowItemStats = 0;
		FIRE_MOUSE_RELEASE_CAPTURE();
		glbMouseVisibility = 1;
		FIRE_SHOW_MOUSE_CURSOR();
	}
	return;
}
