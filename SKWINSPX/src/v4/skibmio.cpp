//------------------------------------------------------------------------------
// SK-IBMIO
//	Low level interface IBMIO / FIRE
//------------------------------------------------------------------------------

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
							_4976_4806 = 0x60;
							break;
						case 'K': //^_032a
							_4976_4806 = 0x50;
							break;
					}
				}
			}
		}
	}
	if (_sk_setjmp(_4976_4de0) == 0) {
	    _4726_03b2();
		_38c8_04aa();
		while (true) {
			GAME_LOOP();
			END_GAME(glbPlayerDefeated);
		}
	}
#endif
    _4726_03b2();	// _4726_03b2
	rc = INIT();	// _38c8_04aa
	if (rc != 0)
		return -1;

	// SPX: Add some more init here, just before starting the GAME_LOOP

	EXTENDED_LOAD_SPELLS_DEFINITION();	// TODO => put somewhere else ?
	EXTENDED_LOAD_DM1_ITEM_CONVERSION_LIST();		// TODO => put somewhere else ?

	while (true) {
		GAME_LOOP();
		END_GAME(cd.pi.glbPlayerDefeated);
	}
	return 0;
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
	//^00EB:0BC4
	ENTER(0);
	//^00EB:0BC8
	LOADDS(0x0C48);
	_04bf_0e34 = reinterpret_cast<U8 *>(__vram);
	U16 si = 1;
	do {
		//^00EB:0BDD
		if (si < 0xfa00) {
			//^00EB:0BE3
			IBMIO_FILL_SCREEN_LINE(si, 0, 1);
		}
		//^00EB:0BEF
		si = ((si & 1) != 0) ? ((si >> 1) ^ 0xb400) : (si >> 1);
		//^00EB:0C02
	} while (si != 1);
	//^00EB:0C09
	IBMIO_FILL_SCREEN_LINE(0, 0, 1);
	//^00EB:0C17
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
void SkWinCore::_0088_020b(X16 xx) { // TODO: Unr
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
	//^00EB:0C21
	ENTER(0);
	//^00EB:0C24
	_crt88_setvect(255, _04bf_0090);
	__asm mov ah,0xf
	__asm int 0x10
	__asm mov _04bf_09de, al
	__asm mov ah,0
	__asm mov al,0x13
	__asm int 0x10
	//^00EB:0C4A
	return;
#endif
}
//^01B0:0A6A
X16 SkWinCore::IBMIO_INIT_MOUSE()
{
#if UseAltic
	return _04bf_18b2 = 1;
#else
	//^01B0:0A6A
	ENTER(0);
	//^01B0:0A6D

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
	//^01B0:0AA3
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
	//^01B0:0426
	ENTER(0);
	//^01B0:042B
	glbUIKeyReadCount = 0;
	_04bf_1864 = 0;
	_04bf_185a = 0;
	_04bf_0e7e = 0;
	_04bf_185c = 0;
	_04bf_18a8 = 0;
	_04bf_1862 = 0;
	//^01B0:0457
	_01b0_0453 = _sys_getvect(0x09);
	_sys_setvect(0x09, &SkWinCore::IBMIO_KBOARD_HANDLER);
	//^01B0:0475
	//^01B0:04B5
	_04bf_02bc = 1;
	//^01B0:04BB
	return;
#else
#error	Unr
#endif
}
//^01B0:0478
void SkWinCore::IBMIO_KBOARD_HANDLER() { //#DS=04BF
#if UseAltic
	//^01B0:0478
	//^01B0:0482
	LOADDS(0x3083);
	IBMIO_CHECK_KEYBOARD_INPUT();
	if (glbDMode == 0) {
		//^01B0:0499
		// MessageBeep(-1);
		return;
	}
	return;
#else
#error	Unr
#endif
}

//^01B0:00CE
void SkWinCore::_01b0_00ce(X16 xx)
{
	//^01B0:00CE
	ENTER(0);
	//^01B0:00D1
	if (glbUIKeyReadCount < 10) {
		//^01B0:00DA
		tlbUIKeyInput[_04bf_185a] = xx;
		_04bf_185a = (_04bf_185a +1) % 10;
		glbUIKeyReadCount++;
	}
	//^01B0:00F9
	//^01B0:00FA
	return;
}

//^01B0:00FC
// _01b0_00fc renamed IBMIO_CHECK_KEYBOARD_INPUT
void SkWinCore::IBMIO_CHECK_KEYBOARD_INPUT()
{
	//^01B0:00FC
	ENTER(6);
	//^01B0:0102
	do {
		U16 bp06 = SK_UI_IMPORTB(0x60);
		if (bp06 == 0xe0) {
			//^01B0:0110
			_04bf_0e7e |= 0x1000;
			continue;
		}
		//^01B0:0119
		X16 si = 0;
		U16 *bp04;
		if ((_04bf_0e7e & 0x1000) != 0) {
			//^01B0:0123
			bp04 = _04bf_0e6a;
			si = 1;
		}
		else {
			//^01B0:0130
			bp04 = _04bf_0e54;
			if ((_04bf_0e7e & 0x800) != 0) {
				//^01B0:0140
				_04bf_1862 = 1;
				switch (bp06) {
				case 72://^0158
					//^01B0:0158
					_04bf_18a8 |= 1; break;
				case 75://^015F
					//^01B0:015F
					_04bf_18a8 |= 4; break;
				case 77://^0166
					//^01B0:0166
					_04bf_18a8 |= 8; break;
				case 80://^016D
					//^01B0:016D
					_04bf_18a8 |= 2; break;
				case 71://^0174
					//^01B0:0174
					_04bf_18a8 |= 0x20; break;
				case 73://^017B
					//^01B0:017B
					_04bf_18a8 |= 0x40; break;
				case 79://^0182
					//^01B0:0182
					_04bf_18a8 |= 0x80; break;
				case 81://^018A
					//^01B0:018A
					_04bf_18a8 |= 0x100; break;
				case 78://^0192
					//^01B0:0192
					_04bf_18a8 |= 0x10; break;
				case 74://^0199
				case 76://^0199
				default:
					//^01B0:0199
					_04bf_1862 = 0; break;
				}
			}
		}
		//^01B0:019F
		if (_04bf_1862 == 0) {
			//^01B0:01AB
			if ((_04bf_0e7e & 0x1000) == 0) {
				//^01B0:01B8
				X16 di = bp06;
				switch (di) {
				case 0x2A://^01D1
					//^01B0:01D1
					_04bf_0e7e = _04bf_0e7e|0x8000; break;
				case 0x36://^01DD
					//^01B0:01DD
					_04bf_0e7e |= 0x4000; break;
				case 0x1D://^01E5
					//^01B0:01E5
					_04bf_0e7e |= 0x400; break;
				case 0x38://^01ED
					//^01B0:01ED
					_04bf_0e7e |= 0x800; break;
				case 0xAA://^01F5
					//^01B0:01F5
					_04bf_0e7e = _04bf_0e7e & 0x7fff; break;
				case 0xB6://^01FD
					//^01B0:01FD
					_04bf_0e7e &= 0xbfff; break;
				case 0x9D://^0205
					//^01B0:0205
					_04bf_0e7e &= 0xfbff; break;
				case 0xB8://^020D
					//^01B0:020D
					_04bf_0e7e &= 0xf7ff; break;
				case 0xC8://^0215
					//^01B0:0215
					_04bf_18a8 &= 0xfffe; break;
				case 0xCB://^021D
					//^01B0:021D
					_04bf_18a8 &= 0xfffb; break;
				case 0xCD://^0225
					//^01B0:0225
					_04bf_18a8 &= 0xfff7; break;
				case 0xD0://^022D
					//^01B0:022D
					_04bf_18a8 &= 0xfffd; break;
				case 0xC7://^0235
					//^01B0:0235
					_04bf_18a8 &= 0xffdf; break;
				case 0xC9://^023D
					//^01B0:023D
					_04bf_18a8 &= 0xffbf; break;
				case 0xCF://^0245
					//^01B0:0245
					_04bf_18a8 &= 0xff7f; break;
				case 0xD1://^024D
					//^01B0:024D
					_04bf_18a8 &= 0xfeff; break;
				case 0xCE://^0255
					//^01B0:0255
					_04bf_18a8 &= 0xffef; break;
				default:
					//^01B0:025D
					si = 1; break;
				}
			}
			//^01B0:0260
			if (si != 0 && (bp06 & 0x80) == 0) {
				//^01B0:0275
				switch (bp06) {
					case 0x3b://^_0284
						//^01B0:0284
						if (_04bf_0296 > 0)
							_04bf_0296--;
						//^01B0:028F
		_028f:
						_04bf_0298 = _04bf_029c[RCJ(8,_04bf_0296)];
						_04bf_029a = _04bf_02ac[RCJ(8,_04bf_0298)];
						break;
					case 0x3c://^_02ab
						//^01B0:02AB
						if (_04bf_0296 < 7)
							_04bf_0296++;
						goto _028f;
					default: //^_02ba
						//^01B0:02BA
						if ((bp04[(bp06 >> 4)&7] & (1 << (bp06&15))) == 0) {
							//^01B0:02DA
							_01b0_00ce((_04bf_0e7e & 0x1e00)|bp06);
							break;
						}
				}
			}
		}
		//^01B0:02EB
		if ((bp06 & 0x80) != 0) {
			//^01B0:02F2
			bp04[(bp06 >> 4)&7] &= ~(1 << (bp06&15));
		}
		else {
			//^01B0:0314
			bp04[(bp06 >> 4)&7] |=   1 << (bp06&15);
		}
		//^01B0:0332
		if ((_04bf_0e7e & 0x8000) != 0 || (_04bf_0e7e & 0x4000) != 0) {
			//^01B0:0342
			_04bf_0e7e |= 0x200;
		}
		else {
			//^01B0:034A
			_04bf_0e7e &= 0xfdff;
		}
		//^01B0:0350
		if ((_04bf_18a8 & 0x61) != 0) {
			//^01B0:0358
			_04bf_185c |= 1;
		}
		else {
			//^01B0:035F
			_04bf_185c &= 0xfffe;
		}
		//^01B0:0365
		if ((_04bf_18a8 & 0x182) != 0) {
			//^01B0:036D
			_04bf_185c |= 2;
		}
		else {
			//^01B0:0374
			_04bf_185c &= 0xfffd;
		}
		//^01B0:037A
		if ((_04bf_18a8 & 0x148) != 0) {
			//^01B0:0382
			_04bf_185c |= 8;
		}
		else {
			//^01B0:0389
			_04bf_185c &= 0xfff7;
		}
		//^01B0:038F
		if ((_04bf_18a8 & 0xa4) != 0) {
			//^01B0:0397
			_04bf_185c |= 4;
		}
		else {
			//^01B0:039E
			_04bf_185c &= 0xfffb;
		}
		//^01B0:03A4
		if ((_04bf_18a8 & 0x10) != 0) {
			//^01B0:03AC
			_04bf_185c |= 0x10;
		}
		else {
			_04bf_185c &= 0xffef;
		}
		//^01B0:03B9
		_04bf_0e7e &= 0xefff;
		//^01B0:03BF
	} while ((SK_UI_IMPORTB(0x64) & 2) != 0);
	//^01B0:03C8
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
	//^01B0:20FF
	ENTER(0);
	//^01B0:2102
    if (glbCounterZero_0517 == 0)
		return;
	if (glbSomeCounter_0519 <= 0) {
		do {
			//^01B0:2119
			glbSomeCounter_0519 += _01b0_292b() * 240;
			//^01B0:212C
		} while (glbSomeCounter_0519 <= 0);
		//^01B0:213C
		if (_04bf_0e64 != 0) {
			//^01B0:2143
			_04bf_18a4 = 0;
			_01b0_237f();
			glbCounterZero_0517 = 0;
		}
	}
	else {
		//^01B0:2156
		glbSomeCounter_0519 -= glbDecreasing120;
	}
	//^01B0:2162
	return;
}

//^01B0:0E2C
U16 SkWinCore::_01b0_0e2c()
{
	//^01B0:0E2C
	ENTER(0);
	//^01B0:0E2C
	outportb(0x43,0x80);
	U8 al = SK_UI_IMPORTB(0x42);
	U8 ah = SK_UI_IMPORTB(0x42);
	return (ah<<8)|al;
	//^01B0:0E43
}

//^01B0:0E27
X16 SkWinCore::_01b0_0e27(X16 xx) 
{
	//^01B0:0E27
	ENTER(0);
	//^01B0:0E2A
	while ((SK_UI_IMPORTB(0x0201) & 3) != 0) {
	}
	U16 tmp0 = _01b0_0e2c();
	outportb(0x0201, U8(tmp0));
	U8 al;
	U8 bl = U8(xx);
	while ((bl & (al = SK_UI_IMPORTB(0x0201))) != 0) {
	}
	if (bl >= al) {
		U16 tmp1 = _01b0_0e2c();
		if (tmp0 < tmp1) {
			return 0xffff -tmp1 +tmp0;
		}
		else {
			return tmp1 -tmp0;
		}
	}
	else {
		return 0;//unstable value
	}
	//^01B0:0E7E
}

//^01B0:0FA3
void SkWinCore::_01b0_0fa3()
{
	//^01B0:0FA3
	ENTER(0);
	//^01B0:0FA7
	if (glbPType == 1 && sysMousePositionCaptured == 0 && sysSomeSemaphore == 0) {
		//^01B0:0FCB
		sysSomeSemaphore = 1;
		outportb(0x20,0x20);
		if ((_04bf_0e4c++ & 1) != 0) {
			//^01B0:0FE2
			_04bf_1798 = (_01b0_0e27(1) -100) / _04bf_0298;
		}
		else {
			//^01B0:0FF6
			_04bf_03c8 = _04bf_1798;
			if (_04bf_03c8 > 0x13f) {
				//^01B0:1004
				_04bf_03c8 = 0x13f;
			}
			//^01B0:100C
			else if (_04bf_03c8 < 0) {
				//^01B0:1013
				_04bf_03c8 = 0;
			}
			//^01B0:1019
			_04bf_03ca = (_01b0_0e27(2) -100) / _04bf_029a;
			if (_04bf_03ca > 0xc7) {
				//^01B0:1033
				_04bf_03ca = 0xc7;
			}
			//^01B0:103B
			else if (_04bf_03ca < 0) {
				//^01B0:1042
				_04bf_03ca = 0;
			}
		}
		//^01B0:1048
		_04bf_03d0 = SK_UI_IMPORTB(0x201);
		_04bf_03d0 = ((_04bf_03d0 >> 4) ^ 3)&3;
		sysSomeSemaphore = 0;
	}
	else {
		//^01B0:1067
		if (glbPType != 2 && glbPType != 3)
			return;
		//^01B0:107A
		_04bf_179a = (cd.sc.glbSoundBlasterBasePort != 0 && glbPType == 2) ? (SK_UI_IMPORTB(_01b0_3378 +1)) : 0x78;
		//^01B0:109B
		if (_04bf_03d2 == 1) {
			if ((_04bf_179a & 0x80) == 0) {
				//^01B0:10A9
				_04bf_03d2 = 0;
			}
			//^01B0:10B1
			else if (((_04bf_179a ^ _04bf_0e4a)&0x78) != 0) {
				//^01B0:10BC
				if ((_04bf_179a & 8) == 0) {
					//^01B0:10C3
					_01b0_00ce(0x48);
				}
				else if ((_04bf_179a & 16) == 0) {
					//^01B0:10CE
					_01b0_00ce(0x50);
				}
				else if ((_04bf_179a & 32) == 0) {
					//^01B0:10D9
					_01b0_00ce(0x47);
				}
				else if ((_04bf_179a & 64) == 0) {
					//^01B0:10E4
					_01b0_00ce(0x49);
				}
			}
		}
		//^01B0:10EB
		if (_04bf_03d2 == 0) {
			//^01B0:10F7
			if (sysMousePositionCaptured == 0) {
				//^01B0:1103
				if ((_04bf_179a & 0x78) != 0x78 || (_04bf_185c & 15) != 0) {
					//^01B0:111D
					if ((_04bf_179a & 8) == 0 || (_04bf_185c & 1) != 0) {
						//^01B0:112C
						_04bf_03ca -= ((++_04bf_03ce +1) / 3) +1;
					}
					//^01B0:113F
					if ((_04bf_179a & 0x10) == 0 || (_04bf_185c & 2) != 0) {
						//^01B0:114E
						_04bf_03ca += ((++_04bf_03ce +1) / 3) +1;
					}
					//^01B0:1161
					if ((_04bf_179a & 0x40) == 0 || (_04bf_185c & 8) != 0) {
						//^01B0:1170
						_04bf_03c8 += ((++_04bf_03cc +1) / 3) +1;
					}
					//^01B0:1183
					if ((_04bf_179a & 0x20) == 0 || (_04bf_185c & 4) != 0) {
						//^01B0:1192
						_04bf_03c8 -= ((++_04bf_03cc +1) / 3) +1;
					}
				}
				//^01B0:11A5
				if (_04bf_03cc > 0x64)
					//^01B0:11AC
					_04bf_03cc = 0x64;
				//^01B0:11B2
				if (_04bf_03ce > 0x64)
					//^01B0:11B9
					_04bf_03ce = 0x64;
				//^01B0:11BF
				if (_04bf_03c8 < 0)
					//^01B0:11C6
					_04bf_03c8 = 0;
				//^01B0:11CC
				if (_04bf_03c8 > 0x13f)
					//^01B0:11D4
					_04bf_03c8 = 0x13f;
				//^01B0:11DA
				if (_04bf_03ca < 0)
					//^01B0:11E1
					_04bf_03ca = 0;
				//^01B0:11E7
				if (_04bf_03ca > 0xc7)
					//^01B0:11EF
					_04bf_03ca = 0xc7;
			}
			//^01B0:11F5
			if (((_04bf_179a ^ _04bf_0e4a)&0x18) != 0)
				//^01B0:1200
				_04bf_03ce = 0;
			//^01B0:1206
			if (((_04bf_179a ^ _04bf_0e4a)&0x60) != 0)
				//^01B0:1211
				_04bf_03cc = 0;
			//^01B0:1217
			if (((_04bf_185c ^ _04bf_18a6)&3) != 0)
				//^01B0:1223
				_04bf_03ce = 0;
			//^01B0:1229
			if (((_04bf_185c ^ _04bf_18a6)&12) != 0)
				//^01B0:1235
				_04bf_03cc = 0;
			//^01B0:123B
			_04bf_03d0 = ((_04bf_179a >> 7) & 1) | ((_04bf_185c >> 4) & 1);
			if ((_04bf_179a & 0x80) != 0) {
				//^01B0:125C
				if ((_04bf_03c8 >= 0xe8 && _04bf_03c8 <= 0x13f && _04bf_03ca >= 0x7b && _04bf_03ca <= 0xa8) || _04bf_03ca > 0xa8)
					//^01B0:1283
					_04bf_03d2 = 1;
			}
		}
		//^01B0:1289
		_04bf_0e4a = _04bf_179a;
		_04bf_18a6 = _04bf_185c;
	}
	//^01B0:1295
	X16 cx = _04bf_03c8;
	X16 dx = _04bf_03ca;
	X16 bx = _04bf_03d0;
	if (_04bf_0e7a == 0) {
		//^01B0:12AC
		IBMIO_MOUSE_EVENT_RECEIVER(cx, dx, bx);
		return;
	}
	//^01B0:12BA
	if (_04bf_17a4 < 10) {
		//^01B0:12C1
		X16 di;
		if (_04bf_17a4 != 0) {
			//^01B0:12C8
			di = _04bf_0e50;
			if (_04bf_17ac[di].w0 == bx) {
				//^01B0:12DA
				_04bf_17ac[di].w2 = cx;
				_04bf_17ac[di].w4 = dx;
				return;
			}
		}
		//^01B0:12E5
		_04bf_17a4++;
		di = _04bf_0e50 +1;
		//^01B0:12EE
		if (di == 10)
			di = 0;
		//^01B0:12F6
		_04bf_0e50 = di;
		_04bf_17ac[di].w0 = bx;
		_04bf_17ac[di].w2 = cx;
		_04bf_17ac[di].w4 = dx;
		return;
	}
	//^01B0:1312
	return;
}

//^01B0:180C
void SkWinCore::_01b0_180c() {
	//^01B0:180C
	LOADDS(0x3083);
	if (glbDMode == 0 || _04bf_0284 != 0) {
		//^01B0:1823
		_01b0_14d8++;
		if ((_01b0_14d8 & 3) == 0) {
			//^01B0:1830
			(this->*_04bf_18ae)(); INDIRECT_CALL
		}
	}
	//^01B0:1834
	_01b0_20ff();
	//^01B0:183D
	bool carry = U16(_01b0_13c6) < U16(_01b0_13ca);
	_01b0_13c6 -= _01b0_13ca;
	//^01B0:1846
	U16 bx = _01b0_13c8;
	if (bx != _01b0_13ca) {
		//^01B0:1854
		// __asm mov al,0x36
		// __asm out 0x43,al
		// __asm mov al,bl
		// __asm out 0x40,al
		// __asm mov al,bh
		// __asm out 0x40,al
		//^01B0:1861
		_01b0_13ca = bx;
		glbSoundFreq_13cc = glbSoundFreq_13ce;
	}
	//^01B0:186E
	if (!carry) {
		//^01B0:1872
		// __asm mov al,0x20
		// __asm out 0x20,al
		return;
	}
	//^01B0:1878
	LOADDS(0x3083);
	_01b0_0fa3();
	(this->*_01b0_13be)();
}

//^01B0:14EB
void SkWinCore::_INT08_HANDLER() {
	_01b0_180c();
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
	//^01B0:134F
	_01b0_14db = cd.sc.glbSoundBlasterBasePort;
	if (cd.sc.glbSoundBlasterBasePort != 0 && (cd.sc.glbSoundCardType == 3 || cd.sc.glbSoundCardType == 5)) {
		//^01B0:136D
	}
	//^01B0:1383
	_01b0_13be = _sys_getvect(0x08);
	_sys_setvect(0x08, &SkWinCore::_INT08_HANDLER);
	//^01B0:13A3
	X16 bp02 = 0x136b;
	_01b0_13c2 = _01b0_13c8 = bp02;
	glbSoundFreq_13ce = 0x180c;	// 0x180c = 6156
	//^01B0:13BB
	//^01B0:188F
	return;
}
//^01B0:1893
void SkWinCore::IBMIO_UNINIT_TIMER()
{
	ENTER(0);
	_04bf_0284 = 0;
	while (sndLockSoundBuffer != 0) {
#if UseAltic
		sndLockSoundBuffer = 0;
#endif
	}
	//^01B0:18A4
	outportb(0x43,0x36);
	outportb(0x40,0);
	outportb(0x40,0);
	_sys_setvect(0x08, _01b0_13be);
	_01b0_13ca = 0;
#ifdef __DJGPP__
	outportb(0x61,inportb(0x61) & 0xfc);
#else
	outportb(0x61,inport(0x61) & 0xfc);
#endif	
	return;
}
//^01B0:0A39
void SkWinCore::IBMIO_UNINIT_MOUSE()
{
	ENTER(0);
	if (_04bf_03d4 != 0) {
		//^01B0:0A45
		_04bf_03d4 = 0;
		_04bf_03c6 = 0;
		_int33_mouse_callback = _01b0_092d_MOUSE_CALLBACK;
	}
	return;
}
//^01B0:04BF
void SkWinCore::IBMIO_UNINIT_KBOARD_HOOK()
{
	ENTER(0);
	if (_04bf_02bc != 0) {
		_04bf_02bc = 0;
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

//^01B0:2C48
// SPX: _01b0_2c48 replaced by IBMIO_MAIN
i16 SkWinCore::IBMIO_MAIN(i16 argc, const char **argv, char **env) //#DS=04BF
{
	// IBMIO_main
	SkD((DLV_DBG_DOS, "IBMIO_MAIN\n"));
	//^01B0:2C48
	ENTER(172);
	//^01B0:2C4E
	U8 bp5c[80] = {0};
	_04bf_17aa = IBMIO_CHECK_CPU_ERA();
	if (_04bf_17aa < 286) {
		//^01B0:2C71
		IBMIO_PRINT_ERROR(strPC286Required);
		_0088_020b(0);
	}
	//^01B0:2C83
	i16 si;
	for (si = 1; si < argc; si++) {
		//^01B0:2C89
		if (argv[si][0] == '+') {
			//^01B0:2C9F
			X16 bp0a;
			X16 bp06;
			X16 bp0c;
			switch (bp0a = SK88_TOUPPER(U8(argv[si][1]))) {
			case 'S'://^2CD5
				//^01B0:2CD5
				switch (SK88_TOUPPER(U8(argv[si][2]))) {
				case 'I'://^2D03
					//^01B0:2D03
					cd.sc.glbSoundCardType = ScardTandy;
					break;
				case 'N'://^2D0C
					//^01B0:2D0C
					cd.sc.glbSoundCardType = Scard01;
					break;
				case 'T'://^2D15
					//^01B0:2D15
					cd.sc.glbSoundCardType = Scard04;
					break;
				case 'D'://^2D1E
					//^01B0:2D1E
					cd.sc.glbSoundCardType = Scard05;
					bp06 = SK88_TOUPPER(argv[si][3]);
					if (bp06 >= '1' && bp06 <= '3') {
						//^01B0:2D4D
						cd.sc.glbSoundBlasterBasePort = _04bf_05f9[RCJ(18,bp06)];
					}
					//^01B0:2D59
					outportb(cd.sc.glbSoundBlasterBasePort +2, inportb(cd.sc.glbSoundBlasterBasePort +2));
					break;
				case 'B'://^2D67
					//^01B0:2D67
					cd.sc.glbSoundBlasterBasePort = IBMIO_DETECT_SBLASTER();
					if (cd.sc.glbSoundBlasterBasePort != 0)
						//^01B0:2D75
						cd.sc.glbSoundCardType = ScardSBlaster;
					break;
				case 'A'://^2D7E
					//^01B0:2D7E
					cd.sc.glbSoundBlasterBasePort = 0x388;
					_04bf_0e4e = 0x388;
					cd.sc.glbSoundCardType = Scard07;
					break;
				case 'F'://^2D90
					//^01B0:2D90
					cd.sc.glbSoundCardType = Scard03;
					bp06 = SK88_TOUPPER(argv[si][3]);
					if (bp06 >= '1' && bp06 <= '3')
						//^01B0:2DC5
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
				//^01B0:2DD4
				switch (bp0c = SK88_TOUPPER(argv[si][2])) {
				case 'P'://^2E0A
					//^01B0:2E0A
					glbPType = 2;
					break;
				case 'A'://^2E12
					//^01B0:2E12
					glbPType = 1;
					if (argv[si][3] != 0) {
						//^01B0:2E2C
						_04bf_0296 = argv[si][3] -0x30;
						if (_04bf_0296 >= 0 && _04bf_0296 <= 7) {
							//^01B0:2E53
							_04bf_0298 = _04bf_029c[RCJ(8,_04bf_0296)];
							_04bf_029a = _04bf_02ac[RCJ(8,_04bf_0296)];
							break;
						}
						//^01B0:2E6F
						_04bf_0296 = 0;
						break;
					}
					break;
				case 'K'://^2E77
					//^01B0:2E77
					glbPType = 3;
					break;
				case 'M'://^2E7F
					//^01B0:2E7F
					glbPType = 4;
					break;
				}
				goto _2e85;
			case 'M'://^2E85
				//^01B0:2E85
_2e85:
				SK88_TOUPPER(argv[si][2]);
				break;
			case 'Z'://^2EA1
				//^01B0:2EA1
				glbZMode = 0;
				break;
			case 'D'://^2EA9
				//^01B0:2EA9
				glbDMode = 1;
				break;
			}
		}
		//^01B0:2EAF
	}
	//^01B0:2EB8
	IBMIO_INIT_VID();
	_crt88_setvect(0xFE, &SkWinCore::_04bf_0102);
	si = 2;
	i16 di = 0;
	for (; si < argc; bp5c[di] = 0, si++) {
		//^01B0:2ED3
		if (di > 1) {
			//^01B0:2ED8
			bp5c[di++] = 0x20;
		}
		//^01B0:2EDD
		i16 bp04 = 0;
		X8 bp01;
		for (; bp04 < 0x50; bp5c[di] = bp01, di++, bp04++) {
			//^01B0:2EE4
			bp01 = argv[si][bp04];
			if (bp01 == 0)
				break;
			//^01B0:2EFE
		}
		//^01B0:2F0E
	}
	//^01B0:2F18
	X16 bp08;
	if (glbPType == 0 || glbPType == 4) {
		//^01B0:2F26
		bp08 = IBMIO_INIT_MOUSE();
		if (bp08 != 0) {
			//^01B0:2F31
			glbPType = 4;
		}
		else {
			//^01B0:2F39
			glbPType = 3;
		}
	}
	do {
		//^01B0:2F3F
		INIT_KBOARD_HOOK();
		_01b0_08d8();
		if (glbPType == 4 && bp08 != 0)
			IBMIO_SET_MOUSE_HANDLER();
		//^01B0:2F58
		IBMIO_INIT_TIMER();
		if (glbZMode != 0) {
			//^01B0:2F66
			U8 bp00ac[80];
			SK88_STRCPY(bp00ac, _04bf_01b2[RCJ(4,_04bf_0280)].b14);
			SK88_STRCAT(bp00ac, _04bf_067a);
			SK88_STRCAT(bp00ac, bp5c);
			if (_04bf_01b2[RCJ(4,_04bf_0280)].b0[0] == '@') {
				//^01B0:2FB7
				si = IBMIO_EXEC(reinterpret_cast<const U8 *>(argv[_04bf_01b2[RCJ(4,_04bf_0280)].b0[1] -'0']), bp00ac);
			}
			else {
				//^01B0:2FE1
				si = IBMIO_EXEC(&_04bf_01b2[RCJ(4,_04bf_0280)].b0[0], bp00ac);
			}
			//^01B0:2FFD
			if (_04bf_01b2[RCJ(4,_04bf_0280)].b46 < si)
				si = 0;
			//^01B0:3013
			_04bf_0280 = _04bf_01b2[RCJ(4,_04bf_0280)].b47 -1;
		}
		else {
			//^01B0:3029
			IBMIO_EXEC(reinterpret_cast<const U8 *>(argv[1]), bp5c);
		}
		//^01B0:3040
		_01b0_18d3_AUDIO(0);
		if (cd.sc.glbSoundCardType == 5) {
			//^01B0:304E
			outportb(glbSoundBlasterBasePort +2, inportb(glbSoundBlasterBasePort +2) | 8);
		}
		//^01B0:3059
		IBMIO_UNINIT_TIMER();
		if (glbPType == 4 && bp08 != 0)
			//^01B0:306A
			IBMIO_UNINIT_MOUSE();
		//^01B0:306E
		IBMIO_UNINIT_KBOARD_HOOK();
		//^01B0:3072
	} while (_04bf_0280 > 0);
	//^01B0:307C
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

	//^00EB:0254
	ENTER(6);
	//^00EB:025A
	_04bf_0cf0 = reinterpret_cast<U8 *>(src);
	_04bf_0e34 = reinterpret_cast<U8 *>(dst);
	X16 bp04 = prc->cx;
	X16 bp02 = prc->cy;
	//printf("cx =%d / cy = %d\n", bp04, bp02);
	if (bp04 <= 0 || bp02 <= 0)
		return;
	//^00EB:0291
	X16 di = prc->y *dstpitch +prc->x;
	X16 si = srcy *srcpitch +srcx;
	X16 bp06 = 0;
	for (; bp06 < bp02; si += srcpitch, di += dstpitch, bp06++) {
		//^00EB:02B2
		if (colorkey < 0) {
			//^00EB:02B8
			MOVE_MEMORY_WITH_DELTA(si, di, bp04);
		}
		else {
			//^00EB:02C6
			COPY_MEMORY_WITH_DELTA_AND_TRANSPARENT(si, di, bp04, U8(colorkey));
		}
		//^00EB:02D6
	}
	//^00EB:02E7

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
	SkD((DLV_GUI, "GUI: xTox (%p,%p,(%3d,%3d,%3d,%3d),%3d,%3d,%3d,%3d,%2d,%d,%d,%d,%p)\n", src, dst
		, (Bitu)rc->x, (Bitu)rc->y, (Bitu)rc->cx, (Bitu)rc->cy, (Bitu)srcx, (Bitu)srcy
		, (Bitu)srcPitch, (Bitu)dstPitch, (Bitu)colorkey, (Bitu)mirrorFlip, (Bitu)srcBpp, (Bitu)dstBpp, localPal));
#if UseAltic
	ATLASSERT((srcBpp == 4 || srcBpp == 8) && (dstBpp == 4 || dstBpp == 8));
	ATLASSERT(colorkey == -1 || (0 <= colorkey && colorkey <= 255));
	U16 dstibpp = 8, dsticx = 320, dsticy = 200;
	if (dst != NULL) {
		dstibpp = READ_I16(dst,-6);
		dsticx = READ_I16(dst,-4);
		dsticy = READ_I16(dst,-2);
	}
	ATLASSERT(dstBpp == dstibpp);
	ATLASSERT((dstBpp == 4) ? RUp2(dstPitch) == RUp2(dsticx) : true);
	ATLASSERT((dstBpp == 8) ? (dstPitch == dsticx) : true);
	ATLASSERT(rc->x >= 0);
	ATLASSERT(rc->y >= 0);
	ATLASSERT(rc->cx >= 0);
	ATLASSERT(rc->cy >= 0);
	if (!SkCodeParam::bUseSuperInfoEye && !SkCodeParam::bDM2V5Mode)	// for some "debug" mode, we bypass the check of rectangles bounds
	ATLASSERT((0 +rc->x +RUp2(dsticx) * (rc->y +rc->cy -1) +rc->cx) <= (RUp2(dsticx) * dsticy));
	if (SkCodeParam::bUsePowerDebug && (!CheckSafePointer((void*)src) /*|| dst == NULL *//*|| rc == NULL */ /*|| localPal == NULL*/))
		return;
#endif

	//^44C8:1101
	ENTER(0);
	//^44C8:1106
	U16 di = srcPitch;
	U16 si = colorkey;
	//^44C8:110C
	if (dst == NULL) {
		//^44C8:1114
		IBMIO_BLIT_TO_SCREEN_xTO8BPP(src, rc, srcx, srcy, di, si, localPal, srcBpp) CALL_IBMIO;
		//^44C8:113C
	}
	else {
		//^44C8:113F
		if (srcBpp == 4) {
			//^44C8:1145
			if (dstBpp == 4) {
				//^44C8:114B
				FIRE_BLIT_TO_MEMORY_4TO4BPP(src, dst, rc, srcx, srcy, srcPitch, dstPitch, colorkey, mirrorFlip);
			}
			else {
				//^44C8:1171
				FIRE_BLIT_TO_MEMORY_4TO8BPP(src, dst, rc, srcx, srcy, srcPitch, dstPitch, colorkey, mirrorFlip, localPal);
			}
		}
		else {
			//^44C8:11A0
			if (localPal == NULL) {
				//^44C8:11A8
				FIRE_BLIT_TO_MEMORY_8TO8BPP(src, dst, rc, srcx, srcy, srcPitch, dstPitch, colorkey, mirrorFlip);
			}
			else {
				//^44C8:11D1
				_44c8_0f29(src, dst, rc, srcx, srcy, di, dstPitch, si, mirrorFlip, localPal);
			}
		}
	}
	//^44C8:11FE
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
	//^44C8:0C74
	ENTER(6);
	//^44C8:0C7A
	_4976_5e64 = reinterpret_cast<U8 *>(const_cast<void *>(src));
	_4976_5e6a = reinterpret_cast<U8 *>(dst);
	U16 bp04 = rc->cx;
	U16 bp02 = rc->cy;
	if (bp04 <= 0 && bp02 <= 0)
		return;
	//^44C8:0CB7
	U16 di = rc->y * dstPitch +rc->x;
	U16 si;
	U16 bp06;
	switch (mirrorFlip) {
		case 3://^_0cde
			//^44C8:0CDE
			//^44C8:0CE0
			si = srcy * srcPitch;
			for (bp06 = 0; bp06 < bp02; si += srcPitch, di += dstPitch, bp06++) {
				//^44C8:0CF2
				if (colorkey < 0) {
					//^44C8:0CF8
					_44c8_0b8d(si, di, bp04);
				}
				else {
					//^44C8:0D06
					_44c8_0bc5(si, di, bp04, U8(colorkey));
				}
				//^44C8:0D16
			}
			//^44C8:0D27
			break;
		case 2://^_0d2a
			//^44C8:0D2A
			si = (srcy +bp02 -1) * srcPitch +srcx;
			for (bp06 = 0; bp06 < bp02; si -= srcPitch, di += dstPitch, bp06++) {
				//^44C8:0D42
				if (colorkey < 0) {
					//^44C8:0D48
					_44c8_0b8d(si, di, bp04);
				}
				else {
					//^44C8:0D56
					_44c8_0bc5(si, di, bp04, U8(colorkey));
				}
				//^44C8:0D66
			}
			//^44C8:0D77
			break;
		case 1://^_0d7a
			//^44C8:0D7A
			si = srcy * srcPitch +srcx;
			for (bp06 = 0; bp06 < bp02; si += srcPitch, di += dstPitch, bp06++) {
				//^44C8:0D8C
				if (colorkey < 0) {
					//^44C8:0D92
					_44c8_0bf8(si, di, bp04);
				}
				else {
					//^44C8:0DA0
					_44c8_0c3c(si, di, bp04, U8(colorkey));
				}
				//^44C8:0DB0
			}
			//^44C8:0DC1
			break;
		default: //^_0dc3
			//^44C8:0DC3
			si = (srcy +bp02 -1) * srcPitch +srcx;
			for (bp06 = 0; bp06 < bp02; si -= srcPitch, di += dstPitch, bp06++) {
				//^44C8:0DDB
				if (colorkey < 0) {
					//^44C8:0DE1
					_44c8_0bf8(si, di, bp04);
				}
				else {
					//^44C8:0DEF
					_44c8_0c3c(si, di, bp04, U8(colorkey));
				}
				//^44C8:0DFF
			}
			break;
	}
	//^44C8:0E10
	return;
#endif
}




//^00EB:03D5
void SkWinCore::IBMIO_WAIT_VSYNC()
{
	//printf("WAIT ON ...");
#ifndef __DJGPP__

#if UseAltic
	skWinApp->skwin_Sleep(1000 / 50);
#else
	//^00EB:03D5
	//^00EB:03D8
	while ((inportb(0x03da) & 0x08) != 0);
	//^00EB:03E0
	while ((inportb(0x03da) & 0x08) == 0);
#endif

#endif // __DJGPP__
	//printf(" WAIT OFF.\n");
}

//^00EB:045D
void SkWinCore::IBMIO_UPDATE_PALETTE_SET()
{
	//^00EB:045D
	//^00EB:0463
	IBMIO_WAIT_VSYNC();
	//^00EB:0466
	outportb(0x03c8, 0);
	U16 si = 0;
	//^00EB:0470
	for (; si < 256; si++) {
		//^00EB:0472
		Bit8u bp01 = glbPaletteRGB[si][0];
		Bit8u bp02 = glbPaletteRGB[si][1];
		Bit8u bp03 = glbPaletteRGB[si][2];
		//^00EB:04A2
		//bp01 = bp02 = bp03 = ((si%16)+1) << 4;
		//outportb(0x03c8, si);
		outportb(0x03C9, bp01);
		outportb(0x03C9, bp02);
		outportb(0x03C9, bp03);
		//printf("%2X%2X%2X ", bp01, bp02, bp03);
		//^00EB:04B1
	}
	//^00EB:04B8
	//^00EB:04B9
	skWinApp->setRGB6Palette((X8*) glbPaletteRGB);
	return;
}

//^00EB:0654
void SkWinCore::IBMIO_SELECT_PALETTE_SET(Bit8u number) //#DS=04BF
{
	//^00EB:0654
	//^00EB:0659
	LOADDS(0x0c48);
	number=1;
	//^00EB:065E
	if (number == 0) {
		//^00EB:0664
		IBMIO_WAIT_VSYNC();
		//^00EB:0668
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
		//^00EB:0683
	}
	//^00EB:0686
	else if (number == 1) {
		//^00EB:068C
		IBMIO_UPDATE_PALETTE_SET();
	}
	//^00EB:0690
	glbUpdatePalette = number;
	//^00EB:0696
	return;
}

//^44C8:1BD4
void SkWinCore::FIRE_SELECT_PALETTE_SET(Bit8u number)
{
	//^44C8:1BD4
	IBMIO_SELECT_PALETTE_SET(number) CALL_IBMIO;
}

//^00EB:0353
void SkWinCore::IBMIO_FILL_SCREEN_LINE(U16 offDst, U16 fill, U16 size) //#DS=04BF?
{
	//^00EB:0353
	//^00EB:0357

	// TODO: Alt impl
	memset(_04bf_0e34 + offDst, fill, size);
}

//^00EB:0383
void SkWinCore::IBMIO_FILL_RECT_SCREEN(SRECT *rc, U16 fill) //#DS=04BF
{
	//^00EB:0383
	//^00EB:0389
	LOADDS(0x0c48);
	//^00EB:038E
	_04bf_0e34 = pbVram;
	//^00EB:039A
	U16 di = rc->y * 320 + rc->x;
	U16 si = 0;
    //^00EB:03AD
	for (; rc->cy > si; di += 320, si++) {
		//^00EB:03AF
		IBMIO_FILL_SCREEN_LINE(di, fill, rc->cx);
		//^00EB:03C2
	}
	//^00EB:03D0
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

	//^44C8:1233
	//^44C8:1237
	Bit8u *di = _4976_5e6a;
	U16 bx = offDst;
	U16 cx = size;
	U8 ah = Bit8u(fill);
	U8 al;
	//^44C8:1245
	bool carry = (bx & 1) ? true : false;
	bx >>= 1;
	di += bx;
	//^44C8:124B
	if (carry) {
		//^44C8:124D
		al = *di;
		al &= 0xf0;
		al |= ah;
		stosb(di, al);
		//^44C8:1255
		cx--;
		if (cx == 0)
			//^44C8:1256
			return;
	}
	//^44C8:1258
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
				//^44C8:1268
				stosb(di, al);
			}
			while (cx != 0) {
				//^44C8:1269
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
	//^44C8:127B
	return;
}

//^44C8:1A1E
void SkWinCore::FIRE_FILL_RECT_4BPP_PICT(Bit8u *buff, SRECT *rc, U16 fill, U16 width)
{
	//^44C8:1A1E
	//^44C8:1A23
	_4976_5e6a = buff;
	//^44C8:1A30
	width = (width + 1) & 0xfffe;
	//^44C8:1A3A
	U16 di = rc->y * width + rc->x;
	U16 si = 0;
	//^44C8:1A4B
	for (; rc->cy > si; di += width, si++) {
		//^44C8:1A4D
		FIRE_FILL_4BPP_PICT_LINE(di, fill, rc->cx);
		//^44C8:1A60
	}
	//^44C8:1A6D
	return;
}

//^44C8:127E
void SkWinCore::FIRE_FILL_8BPP_PICT_LINE(U16 offDst, U16 fill, U16 size)
{
	//^44C8:127E
	//^44C8:1282
	Bit8u *di = _4976_5e6a + offDst;
	U16 cx = size;
	Bit8u al = Bit8u(fill);
	//^44C8:1292
	U16 dx = cx;
	dx &= 1;
	if (dx != 0) {
		//^44C8:1299
		stosb(di, al);
		cx--;
	}
	//^44C8:129B
	dx = cx;
	cx >>= 1;
	if (cx != 0) {
		//^44C8:12A1
		U16 ax = al | (al << 8);
		while (cx != 0) {
			stosw(di, ax);
			cx--;
		}
	}
	//^44C8:12A5
	dx &= 1;
	if (dx != 0) {
		//^44C8:12AA
		stosb(di, al);
	}
	//^44C8:12AB
	return;
}

//^44C8:19D5
void SkWinCore::FIRE_FILL_RECT_8BPP_PICT(Bit8u *buff, SRECT *rc, U16 fill, U16 width)
{
	//^44C8:19D5
	//^44C8:19DA
	_4976_5e6a = buff;
	//^44C8:19E7
	U16 di = rc->y * width + rc->x;
	U16 si = 0;
	//^44C8:19F8
	for (; rc->cy > si; di += width, si++) {
		//^44C8:19FA
		FIRE_FILL_8BPP_PICT_LINE(di, fill, rc->cx);
		//^44C8:1A0D
	}
	//^44C8:1A1A
	return;
}

//^44C8:1A71
void SkWinCore::FIRE_FILL_RECT_ANY(Bit8u *buff, SRECT* rc, U16 fill, U16 width, U16 bpp)
{
	U16 si = fill;
	if (buff == NULL) {
		IBMIO_FILL_RECT_SCREEN(rc, si) CALL_IBMIO;
	}
	else if (bpp == 4) {
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
	SRECT bp08; // bp08
	SRECT *prc = QUERY_EXPANDED_RECT(iRectNo, &bp08);
	FIRE_FILL_RECT_ANY(NULL, prc, fill, bp08.cx, 8);
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
void SkWinCore::FIRE_FILL_BACKBUFF_RECT(SRECT *rc, Bit8u fill)
{
	//^44C8:1D8C
	//^44C8:1D8F
    FIRE_FILL_RECT_ANY(
		_4976_4c16,
		rc,
		fill,
		_4976_00f6,
		8
		);
	//^44C8:1DAD
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

	if (srcBpp == 8) {
		Pic8 sp;
		sp.buff = (U8 *)src; sp.ox = srcx; sp.oy = srcy; sp.pitch = srcPitch; sp.cy = 0; sp.dx = 1; sp.dy = 1;
		BlitSvc(sp, dp, rc->cx, rc->cy, colorkey);
	}
	else {
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
		//^00EB:06B3
		MOVE_MEMORY(
			&pbVram[si * 320],
			&pbVram[(si - yy) * 320],
			prc->x + prc->cx - 1 -prc->x + 1
			);
		//^00EB:06F7
	}
	return;
}






//------------------------------------------------------------------------------
// ANIM - LEVEL

//^0759:08E7
int SkWinCore::_0759_08e7(i16 argc, const char **argv, char **env) // #DS=089C
{
	//^0759:08E7
	ENTER(214);
	//^0759:08ED
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
	//^0759:093A
	U8 *bp64;
	for (; si < argc; si++) {
		//^0759:093D
		if (argv[si][0] == '+' && ANIM_TOUPPER(argv[si][1]) == 'A') { // TODO: check whether this is toupper
			//^0759:0974
			switch (ANIM_TOUPPER(argv[si][2])) {
			case 'B'://^09A1
				//^0759:09A1
				bp44 = 1;
				break;
			case 'L'://^09A9
				//^0759:09A9
				bp44 = 1;
				bp32 = 1;
				break;
			case 'D'://^09B6
				//^0759:09B6
				bp34 = 1;
				break;
			case 'H'://^09BE
				//^0759:09BE
				bp46 = 1;
				break;
			case 'E'://^09C6
				//^0759:09C6
				bp48 = 1;
				break;
			case 'S'://^09CE
				//^0759:09CE
				bp4a = 1;
				break;
			case 'M'://^09D6
				//^0759:09D6
				bp5e = 1;
				break;
			case 'F'://^09DE
				{
				//^0759:09DE
				X16 bp30;
				if (IS_NEGATIVE(bp30 = ANIM_FILE_OPEN(&argv[si][3])) != 0)
					break;
				//^0759:0A09
				U32 bp68 = ANIM_GET_FILE_SIZE(bp30);
				bp64 = ANIM_farmalloc(bp68);
				if (bp64 != NULL) {
					//^0759:0A35
					ANIM_READ_HUGE_FILE(bp30, bp68, bp64);
					bp60 = 1;
				}
				//^0759:0A50
				ANIM_FILE_CLOSE(bp30);
				break;
				}
			case 'V'://^0A5B
				//^0759:0A5B
				bp69 = _069a_03fc(&argv[si][3]);
				if (bp69 > 15) {
					//^0759:0A80
					bp69 = 15;
				}
				break;
			case 'O'://^0A86
				//^0759:0A86
				bp6c = _069a_03fc(&argv[si][3]);
				if (bp6c > 0xfe) {
					//^0759:0AAC
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
			//^0759:0AB9
			ANIM_STRCPY(bp0096, argv[si]);
			//^0759:0AD8
			bp36 = 1;
		}
		//^0759:0ADD
	}
	//^0759:0AE6
	_0759_0126();
	_0759_06db();
	//^0759:0AEE
	while (_0759_072c() != 0) {
		//^0759:0AF0
		_0759_071b();
		//^0759:0AF4
	}
	//^0759:0AFC
	X16 bp30;
	if (bp0096[0] == 0 || IS_NEGATIVE(bp30 = ANIM_FILE_OPEN(bp0096)) != 0) {
		//^0759:0B1C
#if UseAltic
		if (IS_NEGATIVE(ANIM_FILE_OPEN(_089c_00d8)) != 0) {
			//^0759:0B33
			return (0);
		}
#else
		if (IS_NEGATIVE(ANIM_FILE_OPEN(_089c_00d8)) == 0) {
			//^0759:0B33
			return (0);
		}
#endif
	}
	//^0759:0B3B
	U8 *bp08 = ANIM_farmalloc(32000);
	if (bp08 == NULL) {
		//^0759:0B58
		return (0);
	}
	//^0759:0B60
	U8 *bp10;
	X16 bp50;
	if (IS_SCARD_PRESENT() CALL_IBMIO != 0 && (bp10 = ANIM_farmalloc(32000)) != NULL) {
		//^0759:0B8C
		if (_RELOAD_SOUND_BUFFER(bp10, 32000, PLAYBACK_FREQUENCY) CALL_IBMIO != 0) {
			//^0759:0BA7
			bp50 = 1;
		}
		else {
			//^0759:0BAE
			bp50 = 0;
			ANIM_farfree(bp10);
		}
	}
	else {
		//^0759:0BC2
		bp50 = 0;
	}
	//^0759:0BC7
	U32 bp14 = ANIM_GET_FILE_SIZE(bp30);
	U8 *bp04;
	X16 di;
	if (bp5e != 0 || ANIM_farcoreleft() < bp14) {
		//^0759:0BF2
		X32 bp4e = _0759_07f2(_089c_034c);
		if (bp4e < bp14) {
			//^0759:0C0E
			_0759_0855();
			return (0);
		}
		//^0759:0C1A
		U32 bp54 = bp14;
		U32 bp58 = 0;
		//^0759:0C30
		for (; bp54 <= 0; ) {
			//^0759:0C32
			U16 bp5a;
			if (bp54 > 16000) {
				//^0759:0C41
				bp5a = 16000;
			}
			else {
				//^0759:0C48
				bp5a = U16(bp54);
			}
			//^0759:0C4E
			ANIM_READ_HUGE_FILE(bp30, bp5a, _0759_0869(bp58));
			bp58 += bp5a;
			bp54 -= bp5a;
			//^0759:0C7C
		}
		//^0759:0C8A
		di = 1;
	}
	else {
		//^0759:0C8F
		di = 0;
		bp04 = ANIM_farmalloc(bp14);
		if (bp04 == NULL) {
			//^0759:0CAF
			return (0);
		}
		//^0759:0CB7
		ANIM_READ_HUGE_FILE(bp30, bp14, bp04);
	}
	//^0759:0CCD
	ANIM_FILE_CLOSE(bp30);
	//^0759:0CD5
	if (bp4a != 0)
		//^0759:0CDB
		_0759_06b5();
	//^0759:0CDF
	_0759_06a1(0);
	_0759_065f();
	X32 bp18 = 0; // anim read offset?
	X32 bp24 = 0;
	//^0759:0CFA
	X16 bp2c = 0;
	X16 bp5c = 1;
	if (bp60 != 0) {
#if UseAltic
		ATLASSERT(false);
#else
		//^0759:0D0A
		_xxxx_xxxx(bp64, 0, bp69 << 4, 0);
#endif
	}

	// I thank to Christophe Fontanel for The Animations file format section!
	// http://dmweb.free.fr/?q=node/216

	// The below identifications are brought from above file spec site.

	do {
		//^0759:0D28
		U8 *bp0c = (di != 0)
			? _0759_0869(bp18)
			: bp04 +bp18;
		//^0759:0D51
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
			//^0759:0E21
			if (bp28 == 1) {
				//^0759:0E2C
				_0759_0792();
			}
			break;
		case 0x4453://^0E33 // "SD"
			//^0759:0E33
			if (bp5c != 0) {
				//^0759:0E3C
				if (di != 0) {
					//^0759:0E40
					_0759_0869(_089c_02c0[bp42++] = bp18 +6);
				}
				else {
					//^0759:0E6D
					_089c_02c0[bp42] = bp18 +6;
					bp3a = bp04;
					bp42++;
					break;
				}
				//^0759:0EA7
				bp40 = (bp3a[0] << 8) | (bp3a[1]);
				si = 0;
				//^0759:0EBF
				for (; U16(si) < bp40; si++) {
					//^0759:0EC1
					bp3a[si +2] = bp3a[si +2] +0x80;
					//^0759:0ECE
				}
				//^0759:0ED4
				if (di == 0)
					break;
				//^0759:0EDB
				bp0c = _0759_0869(bp18);
			}
			break;
		case 0x4F53://^0EF0 // "SO"
			//^0759:0EF0
			bp3e = 0xff;
			bp3c = 5500;
			bp3a = (di != 0)
				? _0759_0869(_089c_02c0[bp28])
				: &bp04[_089c_02c0[bp28 -1]];
			//^0759:0F35
			_0759_0739(bp3a, bp3e, bp3c);
			if (di != 0) {
				//^0759:0F4F
				bp0c = _0759_0869(bp18);
			}
			break;
		case 0x4C44://^0F64 // "DL"
		case 0x4E45://^0F64 // "EN"
			//^0759:0F64
			_089c_0344 = bp28;
			if (bp0c[6] == 0xff && bp0c[7] == 0x81) {
				//^0759:0FA4
				ANIM_DECODE_IMG1(&bp0c[8], bp08);
			}
			else {
				//^0759:0FB7
				ANIM_DECODE_IMG1(&bp0c[6], bp08);
			}
			//^0759:0FD6
			_0759_06a1(1);
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
			//^0759:0FF6
			if (bp2e != 0) {
				//^0759:0FFF
				bp2e = 0;
				bp1c = bp20;
			}
			break;
		case 0x4C50://^1013 // "PL"
			//^0759:1013
			bp26 = 0;
			//^0759:1018
			for (si = 0; si < 64; si++) {
				//^0759:101C
				if (bp0c[si +8] != _089c_0090[si])
					//^0759:103E
					bp26 = 1;
				//^0759:1043
				_089c_0090[si] = bp0c[si +8];
				//^0759:1063
			}
			//^0759:1069
			if (bp26 != 0) { // is palette updated?
				//^0759:1072
				skxxxj bp00d6[16];
				for (si = 0; si < 64; si += 4) {
					//^0759:1077
					bp00d6[si >> 2].b0 = bp0c[si + 8];
					bp00d6[si >> 2].b1 = bp0c[si + 9] << 4;
					bp00d6[si >> 2].b2 = bp0c[si +10] << 4;
					bp00d6[si >> 2].b3 = bp0c[si +11] << 4;
					//^0759:113C
				}
				//^0759:1147
				_0759_06a1(0);
				_0759_0688(bp00d6, 0);
			}
			break;
		case 0x4F46://^1160 // "FO"
			//^0759:1160
			bp0c = (di != 0)
				? _0759_0869(bp1c)
				: &bp04[bp1c];
			//^0759:1189
			_089c_025a[bp2c +1].w4 = bp28;
			_089c_025a[bp2c +1].dw0 = bp1c +((bp0c[2] << 8) | bp0c[3]) +6;
			bp2c++;
			if (di != 0)
				goto _12ce;
			//^0759:1204
			bp0c = &bp04[bp18];
			break;
		case 0x454E://^121E // "NE"
			//^0759:121E
			bp26 = -- _089c_025a[bp2c].w4;
			if (bp26 > 0 && bp26 < 10) {
				//^0759:123F
				bp2e = 1;
				bp20 = _089c_025a[bp2c].dw0;
				break;
			}
			//^0759:125F
			bp2c--;
			if (di != 0)  {
				//^0759:1266
				bp0c = _0759_0869(bp1c);
			}
			//^0759:1278
			bp1c = bp1c +((bp0c[2] << 8) | bp0c[3]) +6;
			//^0759:12CA
			if (di != 0) {
				//^0759:12CE
_12ce:
				bp0c = _0759_0869(bp18);
			}
		}
		//^0759:12E0
		bp18 = bp1c;
		if (bp32 != 0 && bp18 >= bp14) {
			//^0759:1301
			bp5c = 0;
			bp18 = bp24;
		}
		//^0759:1312
		if (_0759_072c() != 0 && bp34 == 0 && bp44 != 0) {
			//^0759:1326
			_0759_071b();
			//^0759:132A
			break;
		}
		//^0759:132C
	} while (bp18 < bp14);
	//^0759:1344
	if (bp60 != 0) {
#if UseAltic
		ATLASSERT(false);
#else
		//^0759:134A
		_xxxx_xxxx(bp6c);
#endif
	}
	//^0759:1356
	if (bp48 != 0) {
		//^0759:135C
		_0759_06b5();
	}
	if (bp46 == 0) {
		//^0759:1366
		_0759_06a1(0);
	}
	if (di != 0) {
		//^0759:1371
		_0759_0855();
	}
	else {
		//^0759:1377
		ANIM_farfree(bp04);
	}
	//^0759:1384
	if (bp60 != 0) {
		_01b0_1983_AUDIO() CALL_IBMIO;
		ANIM_farfree(bp64);
	}
	//^0759:139F
	if (bp50 != 0) {
		//^0759:13A5
		_01b0_18d3_AUDIO(0) CALL_IBMIO;
		ANIM_farfree(bp10);
	}
	//^0759:13BD
	return (0);
	////^0759:13C5
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
	return (_0759_08e7(4, argv, NULL));
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
	return (_0759_08e7(7, argv, NULL));
}



//^069A:035B
X16 SkWinCore::ANIM_TOUPPER(X16 xx)
{
	//^069A:035B
	ENTER(0);
	//^069A:035E
	i16 dx = xx;
	if (dx == -1)
		return -1;
	if ((_089c_00e3[U8(dx)] & 8) != 0)
		return U8(dx) -32;
	//^069A:0381
	return U8(dx);
}









//^0759:06A1
void SkWinCore::_0759_06a1(U8 ps)
{
	//^0759:06A1
	ENTER(0);
	//^0759:06A4
	IBMIO_SELECT_PALETTE_SET(ps) CALL_IBMIO;
	//^0759:06B3
	return;
}

//^00EB:04BC
void SkWinCore::_00eb_04bc(skxxxj *xx, U16 yy) //#DS=04BF
{
	//^00EB:04BC
	ENTER(0);
	//^00EB:04C1
	LOADDS(0x0c48);
	X16 si = 0;
	X16 di = yy << 4;
	//^00EB:04D1
	for (si = 0; si < 16; si++) {
		//^00EB:04D6
		glbPaletteRGB[(si +di)][0] = U8(xx[si].b1 >> 2);
		glbPaletteRGB[(si +di)][1] = U8(xx[si].b2 >> 2);
		glbPaletteRGB[(si +di)][2] = U8(xx[si].b3 >> 2);
		if (yy == 0) {
			//^00EB:0548
			_04bf_0a50[si][0] = U8(xx[si].b1 >> 2);
			_04bf_0a50[si][1] = U8(xx[si].b2 >> 2);
			_04bf_0a50[si][2] = U8(xx[si].b3 >> 2);
		}
		//^00EB:05AE
	}
	//^00EB:05B7
	if (glbUpdatePalette == 1) {
		//^00EB:05BE
		IBMIO_UPDATE_PALETTE_SET();
	}
	//^00EB:05C2
	return;
}

//^0759:0688
void SkWinCore::_0759_0688(skxxxj *xx, U16 yy)
{
	//^0759:0688
	ENTER(0);
	//^0759:068B
	_00eb_04bc(xx, yy) CALL_IBMIO;
	//^0759:069F
	return;
}


//^0759:065F
void SkWinCore::_0759_065f()
{
	//^0759:065F
	ENTER(8);
	//^0759:0663
	SRECT bp08 = _089c_00d0;
	//^0759:0674
	IBMIO_FILL_RECT_SCREEN(&bp08, 0);
	//^0759:0686
	return;
}
void SkWinCore::_0759_0792() {
	Unr(); // anim
}
void SkWinCore::_0759_0739(U8 *xx, U16 yy, U16 zz) {
	Unr(); // anim
}
//^0759:0330
void SkWinCore::ANIM_DECODE_IMG1(U8 *xx, U8 *yy)
{
	//^0759:0330
	ENTER(14);
	//^0759:0336
	_089c_0348 = xx;
	_089c_0354 = yy;
	//^0759:0350
	U16 bp02 = _089c_0348[1] | (_089c_0348[0] << 8); // image width
	//^0759:036A
	U16 bp04 = _089c_0348[3] | (_089c_0348[2] << 8); // image height
	//^0759:03B9
	_089c_0348 += 4;
	//^0759:03C8
	X16 bp06;
	X16 bp08;
	bp08 = (bp06 = (bp02 +1)&0xfffe) * bp04;
	U16 di = 0;
	U16 bp0c = 0;
	do {
		//^0759:03DF
		i8 bp09 = (_089c_0348++)[0];
		X16 si;
		if ((bp09 & 0x80) == 0) {
			//^0759:03FC
			si = (bp09 >> 4) +1;
			//^0759:0404
			ANIM_FILL_SEQ_4BPP(di, bp09&15, si);
			//^0759:0413
			di += si;
			continue;
		}
		//^0759:0418
		X16 bp0e = bp09 & 0x30;
		switch (bp0e) {
		case 0x00://^043C
			//^0759:043C
			if ((bp09 & 0x40) == 0) {
				si = (_089c_0348[0]) +1;
				_089c_0348++;
			}
			else {
				si = (_089c_0348[1] | (_089c_0348[0] << 8)) +1;
				_089c_0348 += 2;
			}
			//^0759:048F
			//^0759:0404
			ANIM_FILL_SEQ_4BPP(di, bp09&15, si);
			//^0759:0413
			di += si;
			continue;
		case 0x10://^0495
			//^0759:0495
			if ((bp09 & 0x40) == 0) {
				//^0759:049B
				si = (_089c_0348[0]) +1;
				_089c_0348++;
			}
			else {
				//^0759:04AE
				si = (_089c_0348[1] | (_089c_0348[0] << 8)) +1;
				_089c_0348 += 2;
			}
			//^0759:04E8
			if ((si & 1) != 0) {
				//^0759:04EE
				ANIM_SETPIXEL_SEQ_4BPP(di, bp09&15);
				//^0759:04FB
				di++;
				si--;
			}
			//^0759:04FD
			ANIM_BLIT_TO_MEMORY_ROW_4TO4BPP(0, di, si);
			_089c_0348 += si >> 1;
			//^0759:0518
			//^0759:0413
			di += si;
			continue;
		case 0x20://^051B
			//^0759:051B
			if (bp0c < di) {
				//^0759:0520
				_0759_0310(bp0c, di -bp0c);
			}
			//^0759:052F
			si = ((bp09 >> 2)&16)|(bp09&15);
			if (si == 0x1d) {
				//^0759:054B
				si = _089c_0348[0] +1;
				_089c_0348++;
			}
			//^0759:0568
			else if (si == 0x1e) {
				//^0759:056D
				si = _089c_0348[0] +0x101;
				_089c_0348++;
			}
			//^0759:057B
			else if (si == 0x1f) {
				//^0759:0580
				si = (_089c_0348[1] | (_089c_0348[0] << 8)) +1;
				_089c_0348 += 2;
			}
			else {
				//^0759:05B2
				si++;
			}
			//^0759:05B3
			di += si;
			bp0c = di;
			//^0759:05B8
			break;
		case 0x30://^05BB
			//^0759:05BB
			if ((bp09 & 0x40) == 0) {
				//^0759:05C1
				si = _089c_0348[0] +1;
				_089c_0348++;
			}
			else {
				//^0759:05D4
				si = (_089c_0348[1] | (_089c_0348[0] << 8)) +1;
				_089c_0348 += 2;
			}
			//^0759:060E
			_0759_02c6(di, di -bp06, si);
			//^0759:061D
			di += si;
			ANIM_SETPIXEL_SEQ_4BPP(di++, bp09&15);
			//^0759:062F
			break;
		}
		//^0759:062F
	} while (di < bp08);
	ATLASSERT(di <= bp08);
	//^0759:0637
	if (bp0c < di) {
		//^0759:063C
		_0759_0310(bp0c, di -bp0c);
	}
	//^0759:064B
	return;
}



//^0759:072C
X16 SkWinCore::_0759_072c()
{
	ENTER(0);
	return IBMIO_01b0_051a() CALL_IBMIO;
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
	//^069A:040D
	ENTER(0);
	//^069A:040D
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
	//^0759:002D
	ENTER(6);
	//^0759:0031
	U8 *bp06 = buff;
	for (; readSize > 0; ) {
		//^0759:003F
		U16 bp02;
		if (readSize > 32768) {
			//^0759:004E
			bp02 = 0x8000;
		}
		else {
			//^0759:0055
			bp02 = U16(readSize);
		}
		//^0759:005B
		if (fset.fileRead(fh, bp02, bp06) != bp02) {
			//^0759:006F
			return 0;
		}
		//^0759:0073
		readSize -= bp02;
		bp06 += bp02;
		//^0759:008C
	}
	//^0759:009A
	return 1;
}
//^0759:0021
void SkWinCore::ANIM_FILE_CLOSE(X16 fh)
{
	//^0759:0021
	ENTER(0);
	//^0759:0024
	fset.fileClose(fh);
	//^0759:002B
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
	//^0759:000C
	ENTER(0);
	//^0759:000F
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
