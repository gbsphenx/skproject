//------------------------------------------------------------------------------
// SK-DEVICE-UI
//	Internal Keyboard + Mouse Interface
//	Note: Host Keyboard + Mouse is handled by either MFC, SDL or Direct DOS
//		Then information is transmitted to this module that handles it internally
//------------------------------------------------------------------------------

//#include <StdAfx.h>	// Required for MVC6 compilation

//#include <skver.h>

#include <skcore.h>
#include <skdebug.h>


//^1031:0B7E
// _1031_0b7e renamed _1031_0b7e_MOUSE
void SkWinCore::_1031_0b7e_MOUSE() //#DS=4976
{
	ENTER(0);
	if (_4976_4e9a != 0) {
		_4976_4e9a = 0;
		FIRE_QUEUE_MOUSE_EVENT(glbMouseInfoX, glbMouseInfoY, glbMouseInfoButton); // xx, yy, button
	}
	return;
}



//^1031:098E
void SkWinCore::_1031_098e() //#DS=4976
{
	//^1031:098E
	ENTER(0);
	//^1031:0993
	while (_476d_05a9() != 0)
		//^1031:0995
		SPECIAL_UI_KEY_TRANSFORMATION();
	//^1031:09A3
	_4976_19a7 = 1;
	//^1031:09A9
	i16 iRingIndex1 = _4976_4ea6;	// di
	i16 iRingIndex2 = glbMouseStateRingIndex + 1;	// si
	if (iRingIndex2 > 10) {
		iRingIndex2 = 0;
	}
	//^1031:09BA
	if (iRingIndex2 != iRingIndex1) {
		iRingIndex2 = iRingIndex1;
		_4976_4e00 = 0;

		while (true) {
			//^1031:09C9
			if (false
				|| tlbMouseStateRing[iRingIndex2].MouseButton() == 0x0040
				|| tlbMouseStateRing[iRingIndex2].MouseButton() == 0x0060
				|| tlbMouseStateRing[iRingIndex2].MouseButton() == 0x0004
			) {
				//^1031:09F9
				tlbMouseStateRing[iRingIndex1] = tlbMouseStateRing[iRingIndex2];
				iRingIndex1++;
				if (iRingIndex1 > 10)
					iRingIndex1 = 0;
				//^1031:0A23
				_4976_4e00++;
			}
			if (iRingIndex2 == glbMouseStateRingIndex)
				break;
			iRingIndex2++;
			if (iRingIndex2 > 10)
				iRingIndex2 = 0;
			continue;
		}
		if (iRingIndex1-- == 0) {
			iRingIndex1 = 10;
		}
		glbMouseStateRingIndex = iRingIndex1;
	}
	//^1031:0A47
	_4976_4e62 = 0;
	glbUIClickEventIndex = 0;
	glbUIClickEventLast = 0;
	_4976_4df8 = 0xffff;
	_4976_4e44 = 0xffff;
	_4976_4e66 = 0;
	_4976_4e68 = 0;
	glbUIEventCode = 0;
	_4976_4dfc = 0;
	_4976_19a7 = 0;
	//^1031:0A7F
	_1031_0b7e_MOUSE();
	//^1031:0A84
	return;
}

//^1031:0BA4
void SkWinCore::FIRE_QUEUE_MOUSE_EVENT(Bit16u xx, Bit16u yy, Bit16u button)
{
	//^1031:0BA4
	ENTER(0);
	//^1031:0BA9
	Bit16u di = button;
	//^1031:0BAC
	if (_4976_19a7 == 0) {
		//^1031:0BB3
		_4976_19a7 = 1;
		//^1031:0BB9
	}
	else {
		//^1031:0BBB
		_4976_4e9a = 1;
		//^1031:0BC1
		glbMouseInfoX = xx;
		glbMouseInfoY = yy;
		glbMouseInfoButton = di;
		//^1031:0BD1
		return;
	}
	//^1031:0BD4
	i16 cx = ((di == 4 && _4976_19af == 0) || di == 0x0040 || di == 0x0060) ? 9 : 7;
	//^1031:0BF2
	_4976_19af = 0;
	//^1031:0BF8
	i16 iRingIndex = glbMouseStateRingIndex + 1;	// si
	if (iRingIndex > 10) {
		iRingIndex -= 11;
	}
	//^1031:0C06
	if (_4976_4e00 >= cx) {
		//^1031:0C0D
		if (di == 2) {
			//^1031:0C12
			_4976_19af = 1;
		}
	}
	else {
		//^1031:0C1A
		_4976_4e00++;
		glbMouseStateRingIndex = iRingIndex;
		tlbMouseStateRing[iRingIndex].MouseButton(di);
		tlbMouseStateRing[iRingIndex].MouseX(xx);
		tlbMouseStateRing[iRingIndex].MouseY(yy);
	}
	//^1031:0C4E
	_4976_19a7 = 0;
	//^1031:0C54
	return;
}

//^443C:0342
U16 SkWinCore::FIRE_MOUSE_EVENT_RECEIVER(Bit16u xx, Bit16u yy, i16 button) //#DS=4976
{
	//printf("FIRE_MOUSE_EVENT_RECEIVER\n");
	// DOS values :
	// Left button Click = 2 / Release = 4
	// Right button Click = 1 / Release = 8
	printf("MOUSE : %d %d => BUTTON : %d\n", xx, yy, button);
	ENTER(2);
	//LOADDS(0x038B) //#DS=4976
	U16 iEventResult = 0; // bp02
	if (button < 0x20) { // button clicked
		FIRE_QUEUE_MOUSE_EVENT(xx, yy, button);
		iEventResult = 0xFFFF;	// NOCURSOR
	}
	else { // button == 0x20 (or >= 0x20?) => button not clicked
		iEventResult = _443c_00f8_MOUSE(xx, yy);
	}
	return iEventResult;
}

//^00EB:0777
void SkWinCore::IBMIO_BLIT_MOUSE_CURSOR(Bit8u *buff, SRECT *rc, Bit16u srcx, Bit16u srcy, Bit16u srcpitch, i16 colorkey) //#DS=04BF
{
	//printf("IBMIO_BLIT_MOUSE_CURSOR Visible = %d\n", glbMouseCursorVisible);
	//^00EB:0777
	LOADDS(0x0c48)
	//^00EB:0780
	if (glbMouseCursorVisible != 0)
		return;
	//^00EB:078A
	_04bf_09e0.x = rc->x;
	_04bf_09e0.y = rc->y;
	_04bf_09e8.x = _04bf_09e8.y = 0;
	//^00EB:07A6
	_04bf_09e0.cx = _04bf_09e8.cx = rc->cx;
	//^00EB:07B0
	_04bf_09e0.cy = _04bf_09e8.cy = rc->cy;
	//^00EB:07BA
	ATLASSERT(_04bf_09e8.cx <= 18 && _04bf_09e8.cy <= 18);
	IBMIO_BLIT_TO_SCREEN_8TO8BPP(
		__vram,
		_04bf_079e,
		&_04bf_09e8,
		rc->x,
		rc->y,
		320,
		24,
		-1
		);
	//^00EB:07DC
	IBMIO_BLIT_TO_SCREEN_8TO8BPP(
		buff,
		__vram,
		rc,
		srcx,
		srcy,
		srcpitch,
		320,
		colorkey
		);
	//^00EB:0803
	_04bf_079c = colorkey;
	glbMouseCursorVisible = 1;
}


//^01B0:07F8
void SkWinCore::LOCK_MOUSE_EVENT() //#DS=04BF
{
	//printf("LOCK_MOUSE_EVENT\n");
	//LOADDS(0x3083)
	_04bf_0e7a++;
}


//^443C:087C
void SkWinCore::_443c_087c_LOCK_MOUSE()
{
	ENTER(0);
	LOCK_MOUSE_EVENT();
	return;
}

//^443C:0889
void SkWinCore::_443c_0889_UNLOCK_MOUSE()
{
	ENTER(0);
	UNLOCK_MOUSE_EVENT();
	return;
}

//^443C:00A9
// _443c_00a9 renamed _443c_00a9_MOUSE
void SkWinCore::_443c_00a9_MOUSE(sk0cea *ref, i16 x, i16 cx, i16 y, i16 cy)
{
	//^443C:00A9
	ENTER(0);
	//^443C:00AC
	_4976_5da4 = ref;
	//^443C:00B9
	_4976_5da8 = _4976_5d98 = x;
	//^443C:00C2
	_4976_5dae.rc4.x = cx;
	//^443C:00C8
	_4976_5d9c = _4976_5dae.rc4.x -x +1;
	//^443C:00CF
	_4976_5daa = _4976_5d9a = y;
	//^443C:00D8
	_4976_5dae.rc4.y = cy;
	//^443C:00DE
	_4976_5d9e = _4976_5dae.rc4.y -y +1;
	//^443C:00E5
	_01b0_0ca4_MOUSE(&_4976_5d98, 0x20);
	//^443C:00F6
	return;
}

//^443C:00F8
// _443c_00f8 renamed _443c_00f8_MOUSE
// SPX: It seems to be invoked when mouse change from a zone to another
// Basically, zones are viewport, champions, positionning, action panel, move arrows, bottom text
X16 SkWinCore::_443c_00f8_MOUSE(X16 xx, X16 yy)
{
	printf("_443c_00f8_MOUSE\n");
	ENTER(14);
	if (glbMouseVisibility > 0)
		return 0xffff;

	if (_4976_5dae.rc4.cx != 0 || xx < _4976_5da8 || xx > _4976_5dae.rc4.x || yy < _4976_5daa || yy > _4976_5dae.rc4.y) {
		_4976_5dae.rc4.cx = 0;
		_4976_4952 = 0;
		sk0cea *bp04 = _4976_5dae.pv0;
		X16 di = 0;
		X16 bp0a = 0;
		U16 iMaxPosX = glbScreenWidth;	// bp0c
		U16 iMaxPosY = glbScreenHeight;	// si
		for (; bp04 != NULL; ) {
			//^443C:0160
			SRECT *bp08 = &bp04->pv6()->rc4;
			//printf("SRECT = %08X\n", bp08);
			X16 bp0e = 0;
			if (bp08->x > xx) {
				//^443C:0184
				iMaxPosX = min_value(iMaxPosX, bp08->x);
			}
			//^443C:0189
			else if (bp08->x +bp08->cx -1 < xx) {
				//^443C:019D
				bp0a = max_value(bp0a, bp08->x +bp08->cx -1);
			}
			else {
				//^443C:01BC
				bp0e = 1;
				bp0a = max_value(bp0a, bp08->x);
				iMaxPosX = min_value(iMaxPosX, bp08->x +bp08->cx -1);
			}
			//^443C:01F1
			if (bp08->y > yy) {
				//^443C:01FD
				iMaxPosY = min_value(iMaxPosY, bp08->y);
			}
			else if (bp08->y +bp08->cy -1 < yy) {
				//^443C:021F
				di = max_value(di, bp08->y +bp08->cy -1);
			}
			else {
				//^443C:0235
				di = max_value(di, bp08->y);
				iMaxPosY = min_value(iMaxPosY, bp08->y +bp08->cy -1);
				if (bp0e != 0) {
					//^443C:0263
					_4976_4952 = bp04->b2();
					if (bp04 != _4976_5da4 && bp04->b4() != 0)
						//^443C:028B
						FIRE_QUEUE_MOUSE_EVENT(xx, yy, bp04->b0());
					break;
				}
			}
			//^443C:02A2
			bp04 = bp04->pv6()->pv0;
			//^443C:02B6
		}
		//^443C:02C1
		if (_4976_5da4 != NULL && bp04 != _4976_5da4 && _4976_5da4->b5() != 0) {
			//^443C:02E7
			FIRE_QUEUE_MOUSE_EVENT(xx, yy, _4976_5da4->b5());
		}
		//^443C:02FC
		_443c_00a9_MOUSE(bp04, bp0a, iMaxPosX, di, iMaxPosY);
	}
	//^443C:0311
	if (_4976_4952 == 2)
		return 3;
	//^443C:031D
	if (_4976_4952 == 1 && glbChampionLeader >= 0) {
		//^443C:032B
		return (_4976_5dac != 0) ? 2 : 1;
	}
	//^443C:033C
	return 0;
}


//^01B0:05AE
// _01b0_05ae renamed _01b0_05ae_PRECALL_BLIT_MOUSE
void SkWinCore::_01b0_05ae_PRECALL_BLIT_MOUSE() //#DS=04BF
{
	//printf("_01b0_05ae_PRECALL_BLIT_MOUSE\n");
	//^01B0:05AE
	if ((true
		&& _04bf_1850 != 0
		&& _04bf_03c6 != 0
	) && (
		false
		|| cd.mk.glbMouseXPos < _04bf_1852.x 
		|| _04bf_1852.x + _04bf_1852.cx - 1 < cd.mk.glbMouseXPos 
		|| cd.mk.glbMouseYPos < _04bf_1852.y 
		|| _04bf_1852.y + _04bf_1852.cy - 1 < cd.mk.glbMouseYPos
	)
	) {
		//^01B0:05EE
		_04bf_1850 = 0;

		//^01B0:05F4
		_04bf_0e7c = FIRE_MOUSE_EVENT_RECEIVER(cd.mk.glbMouseXPos, cd.mk.glbMouseYPos, _04bf_17e8) INDIRECT_CALL;
		//^01B0:060A
		if (_04bf_0e7c >= 0) {
			//^01B0:060E
			_04bf_1938 = _04bf_0e7c;
		}
	}
	//^01B0:0611
	sk0e80 *bp04 = &_04bf_0e80[_04bf_1938];
	//^01B0:0622
	_04bf_185e = cd.mk.glbMouseXPos - bp04->b0;
	//^01B0:0634
	_04bf_1860 = cd.mk.glbMouseYPos - bp04->b1;
	//^01B0:0644
	_04bf_0e38 = bp04->b2;
	_04bf_1936 = bp04->b3;
	_04bf_18aa = 0;
	_04bf_18ac = 0;
	//^01B0:0662
	if (_04bf_185e < 0) {
		//^01B0:0669
		_04bf_18aa = 0 - _04bf_185e;
		_04bf_185e = 0;
	}
	//^01B0:0678
	if (_04bf_1860 < 0) {
		//^01B0:067F
		_04bf_18ac = 0 - _04bf_1860;
		_04bf_1860 = 0;
	}
	//^01B0:068E
	if (_04bf_185e >= 320)
		return;
	//^01B0:069B
	if (_04bf_1860 >= 200)
		return;
	//^01B0:06A8
	if (cd.mk._04bf_17a8 != 0)
		return;
	cd.mk._04bf_17a8++;
	//^01B0:06BB
	SRECT bp0c;
	bp0c.x = _04bf_185e;
	bp0c.cx = _04bf_0e38 - _04bf_18aa;
	//^01B0:06CB
	if (bp0c.x + bp0c.cx - 1 > 319) {
		//^01B0:06D7
		bp0c.cx -= bp0c.x + bp0c.cx - 320;
	}
	//^01B0:06E3
	bp0c.y = _04bf_1860;
	bp0c.cy = _04bf_1936 - _04bf_18ac;
	//^01B0:06F3
	if (bp0c.y + bp0c.cy - 1 > 199) {
		//^01B0:06FF
		bp0c.cy -= bp0c.y + bp0c.cy - 200;
	}
	//^01B0:070B
	IBMIO_BLIT_MOUSE_CURSOR(bp04->b6, &bp0c, _04bf_18aa, _04bf_18ac, _04bf_0e38, bp04->b4);
	//^01B0:0738
	return;
}

//^00EB:073F
void SkWinCore::IBMIO_HIDE_MOUSE_CURSOR() //#DS=04BF
{
	//^00EB:073F
	LOADDS(0x0c48);
	//^00EB:0748
	if (glbMouseCursorVisible != 0) {
		//^00EB:074F
		IBMIO_BLIT_TO_SCREEN_8TO8BPP(
			_04bf_079e,
			__vram,
			&_04bf_09e0,
			0,
			0,
			24,
			320,
			-1
			);
		//^00EB:076E
		glbMouseCursorVisible = 0;
	}
	//^00EB:0774
	return;
}

//^01B0:073D
// _01b0_073d renamed _01b0_073d_MOUSE
void SkWinCore::_01b0_073d_MOUSE() //#DS=04BF
{
	if (cd.mk._04bf_17a8 != 0) {
		IBMIO_HIDE_MOUSE_CURSOR();
		cd.mk._04bf_17a8--;
	}
	return;
}

//^01B0:0752
void SkWinCore::IBMIO_MOUSE_EVENT_RECEIVER(Bit16u cursorx, Bit16u cursory, Bit16u buttons) //#DS=04BF
{
	//^01B0:0752
	Bit16u di = cursorx;
	Bit16u si = cursory;
	cd.mk.glbMouseXPos = di;
	cd.mk.glbMouseYPos = si;
	//printf("IBMIO_MOUSE_EVENT_RECEIVER\n");
	//^01B0:0765
	if ((di != cd.mk.glbMousePreviousXPos || si != cd.mk.glbMousePreviousYPos) && _04bf_17a2 <= 0) {
		//^01B0:0778
		_01b0_073d_MOUSE();
		_01b0_05ae_PRECALL_BLIT_MOUSE();
		cd.mk.glbMousePreviousXPos = di;
		cd.mk.glbMousePreviousYPos = si;
	}
	//^01B0:0788
	cd.mk.glbMouseButtonState = ((buttons >> 1) & 1) | ((buttons << 1) & 2);
	//^01B0:079D
	_04bf_0e7c = buttons ^ _04bf_1934;
	//^01B0:07A7
	if (_04bf_0e7c != 0 && _04bf_03c6 != 0) {
		//^01B0:07B2
		if ((_04bf_0e7c & 1) != 0) { // left click ?
			//^01B0:07BA
			(this->*_04bf_179e)(di, si, ((buttons & 1) != 0) ? 2 : 4) INDIRECT_CALL;
		}
		//^01B0:07D0
		if ((_04bf_0e7c & 2) != 0) { // right click ?
			//^01B0:07D8
			(this->*_04bf_179e)(di, si, ((buttons & 2) != 0) ? 1 : 8) INDIRECT_CALL;
		}
		//^01B0:07EE
		_04bf_1934 = buttons;
	}
	//^01B0:07F4
	return;
}

//^01B0:080B
void SkWinCore::UNLOCK_MOUSE_EVENT() //#DS=04BF
{
	//printf("UNLOCK_MOUSE_EVENT\n");
	//^01B0:080B
	LOADDS(0x3083)
	//^01B0:0819
	while (_04bf_17a4 > 0) {
		//^01B0:081B
		_04bf_17a4--;
		_04bf_179c = (_04bf_179c +1) % 10;
		//^01B0:082D
		Bit16u si = _04bf_17ac[_04bf_179c].w0;
		//^01B0:083B
		Bit16u bp02 = _04bf_17ac[_04bf_179c].w2;
		//^01B0:084C
		Bit16u di = _04bf_17ac[_04bf_179c].w4;
		//^01B0:085B
		IBMIO_MOUSE_EVENT_RECEIVER(bp02, di, si);
		//^01B0:0867
	}
	//^01B0:0870
	_04bf_0e7a--;
}

//^01B0:0AA8
// _01b0_0aa8 renamed _01b0_0aa8_MOUSE
void SkWinCore::_01b0_0aa8_MOUSE() //#DS=04BF
{
	//printf("_01b0_0aa8_MOUSE\n");
	//^01B0:0AA8
	LOADDS(0x3083);
	//^01B0:0AB3
	if ((_04bf_17a2--) == 1) {
		//^01B0:0ABF
		LOCK_MOUSE_EVENT();
		_01b0_05ae_PRECALL_BLIT_MOUSE();
		//^01B0:0AC7
		cd.mk.glbMousePreviousXPos = cd.mk.glbMouseXPos;
		cd.mk.glbMousePreviousYPos = cd.mk.glbMouseYPos;
		//^01B0:0AD3
		UNLOCK_MOUSE_EVENT();
	}
	//^01B0:0AD7
}


//^443C:086B
void SkWinCore::FIRE_SHOW_MOUSE_CURSOR()
{
	//printf("FIRE_SHOW_MOUSE_CURSOR\n");
	//^443C:086B
	glbMouseVisibility--;
	//^443C:0872
	_01b0_0aa8_MOUSE() CALL_IBMIO;
}


//^01B0:08D8
void SkWinCore::_01b0_08d8()
{
	//^01B0:08D8
	ENTER(0);
	//^01B0:08DB
	_04bf_17a2 = 1;
	_04bf_03c6 = 0;
	cd.mk._04bf_17a8 = 0;
	_04bf_1850 = 0;
	sysMousePositionCaptured = 0;
	_04bf_0e7a = 0;
	_04bf_179c = _04bf_0e50 = 0;
	_04bf_17a4 = 0;
	_04bf_1934 = 0;
	_04bf_1938 = 0;
	//^01B0:091C
	return;
}
//^01B0:091E
void SkWinCore::IBMIO_SET_MOUSE_HANDLER()
{
	//printf("IBMIO_SET_MOUSE_HANDLER\n");
#if UseAltic
	//^01B0:091E
	ENTER(0);
	//^01B0:0923
	if (_04bf_18b2 == 0)
		return;
	//^01B0:0935
	_01b0_092d_MOUSE_CALLBACK = _int33_mouse_callback;
	cd.mk.glbMouseXPos = cd.mk.mice_x;
	cd.mk.glbMouseYPos = cd.mk.mice_y;
	_int33_mouse_callback = &SkWinCore::IBMIO_MOUSE_HANDLER;
	//^01B0:0982
	//^01B0:0A2F
	_04bf_03d4 = 1;
	//^01B0:0A35
	return;
#else
#error	Unr
#endif
}

//^01B0:0985
void SkWinCore::IBMIO_MOUSE_HANDLER()	// serves as MOUSE CALLBACK from _int33_mouse_callback()
{
	X16 bx = cd.mk.mice_btn; // buttons
	X16 cx = cd.mk.mice_x; // x
	X16 dx = cd.mk.mice_y; // y

	//printf("IBMIO_MOUSE_HANDLER\n");
	//SkD((DLV_MOUSE,"DBG: M(%3d,%3d) Btn:(%2d) C(%3d,%3d) P(%3d,%3d)\n", glbMouseXPos, glbMouseYPos, mice_btn, cx, dx, glbMousePreviousXPos, glbMousePreviousYPos));

	//^01B0:0985
	if (_01b0_0933 == 0) {
		//^01B0:0990
		bx &= 3;
		if (sysMousePositionCaptured != 0) {
			cx = cd.mk.glbMouseXPos;
			dx = cd.mk.glbMouseYPos;
		}
		//^01B0:09B3
		if (_04bf_0e7a == 0) {
			//^01B0:09BE
			IBMIO_MOUSE_EVENT_RECEIVER(cx, dx, bx);
		}
		//^01B0:09CC
		else if (_04bf_17a4 < 10) {
			//^01B0:09D3
			X16 di;
			if (_04bf_17a4 != 0 && _04bf_17ac[di = _04bf_0e50].w0 == bx) {
				//^01B0:09EC
				_04bf_17ac[di].w2 = cx;
				_04bf_17ac[di].w4 = dx;
			}
			else {
				//^01B0:09F7
				_04bf_17a4++;
				X16 di = _04bf_0e50 +1;
				//^01B0:0A00
				if (di == 10)
					di = 0;
				//^01B0:0A08
				_04bf_0e50 = di;
				_04bf_17ac[_04bf_0e50].w0 = bx;
				_04bf_17ac[_04bf_0e50].w2 = cx;
				_04bf_17ac[_04bf_0e50].w4 = dx;
			}
		}
		//^01B0:0A23
		//^01B0:0A24
	}
	//^01B0:0A2E
}


//^443C:0380
// _443c_0380 renamed IBMIO_INIT_CURSORS_MOUSE
void SkWinCore::IBMIO_INIT_CURSORS_MOUSE()
{
	ENTER(0);
	IBMIO_SET_CURSOR_PATTERN(0, _4976_4862_MouseCursor, 0, 0, 12, 16, 4, glbPaletteT16, 12) CALL_IBMIO;
	IBMIO_SET_CURSOR_PATTERN(1, _4976_48c2_CursorHandPointer, 0, 0, 16, 16, 4, glbPaletteT16, 12) CALL_IBMIO;
	_01b0_0c70_MOUSE(0);
	_01b0_08b6_SET_RECEIVER(&SkWinCore::FIRE_MOUSE_EVENT_RECEIVER);
	return;
}


//^01B0:08B6
void SkWinCore::_01b0_08b6_SET_RECEIVER(U16 (SkWinCore::*pfn)(U16 xx, U16 yy, i16 zz)) //#DS=04BF
{
	ENTER(0);
	LOADDS(0x3083);
	_04bf_179e = pfn;
	_04bf_03c6 = 1;
	return;
}



//^01B0:0D39
// _01b0_0d39 renamed MOUSE_STATE_01b0_0d39
void SkWinCore::MOUSE_STATE_01b0_0d39(i16 *xx, i16 *yy, i16 *zz, Bit16u ww) //#DS=04BF
{
	ENTER(0);
	LOADDS(0x3083);
	if (ww != 0) {
		*xx = _04bf_185e;
		*yy = _04bf_1860;
		*zz = _04bf_1936;
	}
	else {
		*xx = cd.mk.glbMouseXPos;
		*yy = cd.mk.glbMouseYPos;
		*zz = cd.mk.glbMouseButtonState;
	}
	return;
}


// SPX Renamed inportb to SK_UI_IMPORTB to avoid confusion with DOS inportb
U8 SkWinCore::SK_UI_IMPORTB(U16 port) {
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
}



//^1031:0781
void SkWinCore::_1031_0781(Bit16u xx) 
{
	//^1031:0781
	ENTER(12);
	//^1031:0785
	sk0d9e *bp04 = _1031_06b3(&_4976_1891[_4976_19ad], xx);
	SRECT bp0c;
	if (bp04 != NULL && _1031_01d5(bp04->w2, &bp0c) != 0) {
		//^1031:07BB
		FIRE_QUEUE_MOUSE_EVENT(bp0c.x, bp0c.y, 255 & bp04->w4);
	}
	//^1031:07D4
	return;
}

//^443C:08AB
// _443c_08ab renamed MOUSE_STATE_443c_08ab
void SkWinCore::MOUSE_STATE_443c_08ab(i16 *xx, i16 *yy, i16 *zz)
{
	ENTER(0);
	MOUSE_STATE_01b0_0d39(xx, yy, zz, 0) INDIRECT_CALL;
	return;
}



//^1031:0F3A
// SPX _1031_0f3a renamed IBMIO_USER_INPUT_CHECK
void SkWinCore::IBMIO_USER_INPUT_CHECK() //#DS=4976
{
	//^1031:0F3A
	ENTER(14);
	//^1031:0F40
	if (_4976_4e46 == 0) {
		//^1031:0F4A
		Bit32u bp04 = glbAbsoluteTickCounter;
		//^1031:0F57
		Bit32u bp08 = bp04 - _4976_19a9;
		//^1031:0F68
		if (bp08 != 0) {
			//^1031:0F6F
			_4976_19a9 = bp04;
			//^1031:0F7C
			if (_4976_4e62 == 0 && _4976_4e48 == 0 && _4976_4e64 == 0) {
				//^1031:0F97
				while (glbPtrTransmittedUIEvent == 0) {
					//^1031:0F9D
					_4976_19a7 = 1;
					Bit16u di = 1;
					//^1031:0FA6
_0fa6:
					if (_4976_4e00 == 0) {
						//^1031:0FAD
						_4976_19a7 = 0;
						//^1031:0FB3
						_1031_0b7e_MOUSE();
						//^1031:0FB7
						break;
					}
					//^1031:0FBA
					MouseState bp0e = tlbMouseStateRing[_4976_4ea6];
					//^1031:0FD4
					_4976_4e00--;
					_4976_4ea6++;
					//^1031:0FDC
					if (_4976_4ea6 > 10)
						//^1031:0FE4
						_4976_4ea6 = 0;
					//^1031:0FEA
					Bit16u si = 0;
					//^1031:0FEC
					if (bp0e.MouseButton() >= 0x20) {
						//^1031:0FF2
						if (bp0e.MouseButton() == 0x20) {
							//^1031:0FF8
							si = _1031_03f2(&_4976_1891[_4976_19ad], bp0e.MouseX());
						}
						//^1031:100F
						else if (bp0e.MouseButton() == 0x40) {
							//^1031:1015
							si = 0x81;
						}
						//^1031:101A
						else if (bp0e.MouseButton() == 0x60) {
							//^1031:1020
							si = 0xe1;
						}
					}
					//^1031:1025
					else if ((bp0e.MouseButton() & 0x04) != 0) {
						si = 0xe3;
					}
					//^1031:1031
					else if ((bp0e.MouseButton() & 0x13) != 0) {
						si = _1031_030a(&_4976_1891[_4976_19ad], bp0e.MouseX(), bp0e.MouseY(), bp0e.MouseButton());
					}
					//^1031:1055
					if (si != 0) {
						//printf("ZONE %d reached!\n", si);
						//printf("----------------\n", si);
						//^1031:1059
						di = 0;
						//^1031:105B
						glbMousePosition.event = si;
						//^1031:105F
						glbMousePosition.XPos = _4976_4e66;
						glbMousePosition.YPos = _4976_4e68;
						glbMousePosition.rc6.y = _4976_4e9e.y;
						glbMousePosition.rc6.x = _4976_4e9e.x;
						glbMousePosition.rc6.cx = _4976_4e9e.cx;
						glbMousePosition.rc6.cy = _4976_4e9e.cy;
					}
					//^1031:1083
					if (di != 0)
						//^1031:1087
						goto _0fa6;
					//^1031:108A
					_4976_19a7 = 0;
					//^1031:1090
					_1031_0b7e_MOUSE();
					//^1031:1094
					glbPtrTransmittedUIEvent = TRANSMIT_UI_EVENT(&glbMousePosition);
					//^1031:10A5
					bp08 = 0;
					//^1031:10AF
				}
			}
			//^1031:10BB
			HANDLE_UI_EVENT_1031_111e(Bit16u(bp08));
		}
	}
	//^1031:10C4
	return;
}

//^443C:0401
void SkWinCore::FIRE_MOUSE_RELEASE_CAPTURE()
{
	ENTER(0);
	IBMIO_MOUSE_RELEASE_CAPTURE() CALL_IBMIO;
	return;
}


//^01B0:088D
void SkWinCore::IBMIO_MOUSE_RELEASE_CAPTURE() //#DS=04BF
{
	sysMousePositionCaptured--;
//		ENTER(0);
//		LOADDS(0x3083);
//		if (_04bf_18b2 == 1) {
//			__asm mov cx,[glbMouseXPos]
//			__asm shl cx,1
//			__asm mov dx,[glbMouseYPos]
//			__asm mov ax,0x0004
//			__asm int 0x33
//		}
//		sysMousePositionCaptured--;
//		return;
}


//^01B0:0CA4
// _01b0_0ca4 renamed _01b0_0ca4_MOUSE
void SkWinCore::_01b0_0ca4_MOUSE(Bit16u *xx, Bit16u yy)
{
	LOADDS(0x3083);
	LOCK_MOUSE_EVENT();
	for (Bit16u si=0; si < 4; si++) {
		((Bit16u *)&_04bf_1852)[si] = xx[si];
	}
	_04bf_17e8 = yy;
	_04bf_1850 = 1;
	UNLOCK_MOUSE_EVENT();
	return;
}

//^443C:040E
// _443c_040e renamed _443c_040e_MOUSE
void SkWinCore::_443c_040e_MOUSE()
{
	FIRE_HIDE_MOUSE_CURSOR();
	_4976_5dae.rc4.cx = 1;
	_01b0_0ca4_MOUSE(_4976_4954, 32);
	FIRE_SHOW_MOUSE_CURSOR();
	return;
}

//^01B0:0C70
// _01b0_0c70 renamed _01b0_0c70_MOUSE
void SkWinCore::_01b0_0c70_MOUSE(Bit16u xx) //#DS=04BF
{
	LOADDS(0x3083);
	LOCK_MOUSE_EVENT();
	if (cd.mk._04bf_17a8 != 0) {
		_01b0_073d_MOUSE();
		_04bf_1938 = xx;
		_01b0_05ae_PRECALL_BLIT_MOUSE();
	}
	else {
		_04bf_1938 = xx;
	}
	UNLOCK_MOUSE_EVENT();
	return;
}


// Standard DM1 palette
Bit8u _xDefaultFMTownsPalette[16*4] = 
{
	0, 0x00, 0x00, 0x00,
	1, 0x66, 0x66, 0x66,
	2, 0x88, 0x88, 0x88,
	3, 0x66, 0x22, 0x00,
	4, 0x00, 0xCC, 0xCC,
	5, 0x88, 0x44, 0x00,
	6, 0x00, 0x88, 0x00,
	7, 0x00, 0xCC, 0x00,
	8, 0xFF, 0x00, 0x00,
	9, 0xFF, 0xAA, 0x00,
	10, 0xCC, 0x88, 0x66,
	11, 0xFF, 0xFF, 0x00,
	12, 0x44, 0x44, 0x44,
	13, 0xAA, 0xAA, 0xAA,
	14, 0x00, 0x00, 0xFF,
	15, 0xFF, 0xFF, 0xFF,

};

//^01B0:0B01
void SkWinCore::IBMIO_SET_CURSOR_PATTERN(
	i16 index, 
	Bit8u *buffSrc, 
	Bit8u zz, 
	Bit8u ss, 
	Bit16u srcWidth, 
	Bit16u srcHeight, 
	Bit16u srcBits, 
	Bit8u *localPal, 
	Bit16u colorkey
) { 
	// SPX: If localPal is not specified (can happen from a V3 GDAT (FM-Towns, SegaCD/MegaCD), it will then crash
	if (localPal == NULL)
		localPal = _xDefaultFMTownsPalette;
	
	//#DS=04BF
	//^01B0:0B01
	//^01B0:0B07
	LOADDS(0x3083);
	//^01B0:0B0D
	//^01B0:0B0F
	if (index >= 0 && index < 4) {
		//^01B0:0B25
		LOCK_MOUSE_EVENT();
		//^01B0:0B29
		sk0e80 *bp08 = &_04bf_0e80[index];
		//^01B0:0B3A
		if (srcBits == 4) {
			//^01B0:0B45
			Bit16u di = 0;
			//^01B0:0B47
			Bit16u bp0a = (srcWidth +1) & 0xfffe;
			Bit16u bp04 = 0;

			//^01B0:0B56
			for (; bp04 < srcHeight; bp04++) {
				//^01B0:0B59
				Bit16u bp02 = 0;
				//^01B0:0B5E
				for (; bp02 < srcWidth; di++, bp02++) {
					//^01B0:0B60
					Bit16u si = bp0a * bp04 + bp02;
					//^01B0:0B6B
					if ((si & 0x0001) != 0) {
						//^01B0:0B71
						bp08->b6[di] = localPal[buffSrc[si >> 1] & 15];
					}
					else {
						//^01B0:0B91
						bp08->b6[di] = localPal[buffSrc[si >> 1] >> 4];
					}
					//^01B0:0BCF
				}
				//^01B0:0BDF
			}
			//^01B0:0BEF
			bp08->b4 = localPal[U8(colorkey)];
			//^01B0:0C0D
		}
		else {
			//^01B0:0C0F
			i16 si = 0;
			//^01B0:0C11
			for (; srcWidth * srcHeight > si; si++) {
				//^01B0:0C13
				bp08->b6[si] = buffSrc[si];
				//^01B0:0C33
			}
			//^01B0:0C3E
			bp08->b4 = U8(colorkey);
		}
		//^01B0:0C44
		//^01B0:0C48
		bp08->b0 = zz;
		bp08->b1 = ss;
		bp08->b2 = U8(srcWidth);
		bp08->b3 = U8(srcHeight);
		//^01B0:0C66
		UNLOCK_MOUSE_EVENT();
	}
	//^01B0:0C6A
	//^01B0:0C6B
	return;
}



//^443C:085A
void SkWinCore::FIRE_HIDE_MOUSE_CURSOR()
{
	glbMouseVisibility++;
	_01b0_0adb() CALL_IBMIO;
}

//^01B0:0ADB
void SkWinCore::_01b0_0adb() //#DS=04BF
{
	//^01B0:0ADB
	LOADDS(0x3083);
	//^01B0:0AE4
	if (_04bf_17a2++ == 0) {
		//^01B0:0AF1
		LOCK_MOUSE_EVENT();
		_01b0_073d_MOUSE();
		UNLOCK_MOUSE_EVENT();
	}
	//^01B0:0AFD
	return;
}




//^3929:0B01
void SkWinCore::_3929_0b01(U16 xx, U16 yy) { // TODO: Unr
	Unr();
}

//^3929:0B20
void SkWinCore::_3929_0b20(U16 xx, U32 yy) { // TODO: Unr
	Unr();
}

//^1031:0D36
void SkWinCore::_1031_0d36_KEYBOARD(U16 xx, U16 yy) // yy = keyboard key
{
	//^1031:0D36
	ENTER(0);
	//^1031:0D3A
	_3e74_5b7c_KEYBOARD(yy);
	//^1031:0D43
	_4976_19a7 = 1;
	//^1031:0D49
	if (_4976_4e00 < 7) {
		//^1031:0D50
		i16 si = glbMouseStateRingIndex + 2;
		//^1031:0D57
		if (si > 10)	// because MouseState tlbMouseStateRing[11]
			//^1031:0D5C
			si -= 11;
		//^1031:0D5F
		if (si-- == 0)
			//^1031:0D66
			si = 10;
		//^1031:0D69
		glbMouseStateRingIndex = si;
		tlbMouseStateRing[glbMouseStateRingIndex].MouseButton(xx);
		tlbMouseStateRing[glbMouseStateRingIndex].MouseX(yy);
		_4976_4e00++;
	}
	//^1031:0D91
	_4976_19a7 = 0;
	//^1031:0D97
	_1031_0b7e_MOUSE();
	//^1031:0D9B
	return;
}


//^476D:05B6
U8 SkWinCore::_476d_05b6_KEYBOARD(U16 xx)
{
	ENTER(0);
	return _01b0_054a_KEYBOARD(xx) CALL_IBMIO;
}


//^01B0:054A
U8 SkWinCore::_01b0_054a_KEYBOARD(U16 xx) //#DS=04BF
{
	ENTER(0);
	U16 di = xx;
	LOADDS(0x3083);
	if ((di & 0x1000) != 0)
		di = 0;
	U16 si = ((di & 0x0200) != 0) ? (_04bf_033e[di & 0x007f]) : (_04bf_02be[di & 0x007f]);
	if (si != 0 && (di & 0x0400) != 0)
		si &= 0x001f;
	return U8(si);
}
