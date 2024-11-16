#include <StdAfx.h>	// Required for MVC6 compilation

#include <SkVer.h>

#include <KCORE.H>
#include <SkDebug.h>

#if defined(_USE_MFC80) || defined(_USE_MFC60)
#include <SkMFC.h>
#include <resource.h>
#endif // defined(_USE_MFC80) || defined(_USE_MFC60)

//^1031:0B7E
void SkWinCore::_1031_0b7e() //#DS=4976
{
	//^1031:0B7E
	ENTER(0);
	//^1031:0B81
	if (_4976_4e9a != 0) {
		//^1031:0B88
		_4976_4e9a = 0;
		//^1031:0B8E
		FIRE_QUEUE_MOUSE_EVENT(glbMouseInfoX, glbMouseInfoY, glbMouseInfoButton); // xx, yy, button
	}
	//^1031:0BA2
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
	_1031_0b7e();
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
Bit16u SkWinCore::FIRE_MOUSE_EVENT_RECEIVER(Bit16u xx, Bit16u yy, i16 button) //#DS=4976
{
	//printf("FIRE_MOUSE_EVENT_RECEIVER\n");
	// DOS values :
	// Left button Click = 2 / Release = 4
	// Right button Click = 1 / Release = 8
	//printf("MOUSE : %d %d => BUTTON : %d\n", xx, yy, button);
	//^443C:0342
	ENTER(2);
	//^443C:0346
	LOADDS(0x038B) //#DS=4976
	//^443C:034D
	X16 bp02;
	if (button < 0x20) {
		//^443C:0353
		FIRE_QUEUE_MOUSE_EVENT(xx, yy, button);
		bp02 = 0xffff;
	}
	else {
		//^443C:036B
		bp02 = _443c_00f8(xx, yy);
	}
	//^443C:037A
	return bp02;
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
	//^01B0:07F8
	LOADDS(0x3083)
	//^01B0:0803
	_04bf_0e7a++;
}

//^443C:00F8
X16 SkWinCore::_443c_00f8(X16 xx, X16 yy)
{
	//printf("_443c_00f8\n");
	//^443C:00F8
	ENTER(14);
	//^443C:00FE
	if (glbMouseVisibility > 0)
		return 0xffff;
	if (_4976_5dae.rc4.cx != 0 || xx < _4976_5da8 || xx > _4976_5dae.rc4.x || yy < _4976_5daa || yy > _4976_5dae.rc4.y) {
		//^443C:0133
		_4976_5dae.rc4.cx = 0;
		_4976_4952 = 0;
		sk0cea *bp04 = _4976_5dae.pv0;
		X16 di = 0;
		X16 bp0a = 0;
		X16 bp0c = glbScreenWidth;
		X16 si = glbScreenHeight;
		for (; bp04 != NULL; ) {
			//^443C:0160
			SRECT *bp08 = &bp04->pv6()->rc4;
			X16 bp0e = 0;
			if (bp08->x > xx) {
				//^443C:0184
				bp0c = min_value(bp0c, bp08->x);
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
				bp0c = min_value(bp0c, bp08->x +bp08->cx -1);
			}
			//^443C:01F1
			if (bp08->y > yy) {
				//^443C:01FD
				si = min_value(si, bp08->y);
			}
			else if (bp08->y +bp08->cy -1 < yy) {
				//^443C:021F
				di = max_value(di, bp08->y +bp08->cy -1);
			}
			else {
				//^443C:0235
				di = max_value(di, bp08->y);
				si = min_value(si, bp08->y +bp08->cy -1);
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
		_443c_00a9(bp04, bp0a, bp0c, di, si);
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
void SkWinCore::_01b0_05ae() //#DS=04BF
{
	//printf("_01b0_05ae\n");
	//^01B0:05AE
	if ((true
		&& _04bf_1850 != 0
		&& _04bf_03c6 != 0
	) && (
		false
		|| glbMouseXPos < _04bf_1852.x 
		|| _04bf_1852.x + _04bf_1852.cx - 1 < glbMouseXPos 
		|| glbMouseYPos < _04bf_1852.y 
		|| _04bf_1852.y + _04bf_1852.cy - 1 < glbMouseYPos
	)
	) {
		//^01B0:05EE
		_04bf_1850 = 0;

		//^01B0:05F4
		_04bf_0e7c = FIRE_MOUSE_EVENT_RECEIVER(glbMouseXPos, glbMouseYPos, _04bf_17e8) INDIRECT_CALL;
		//^01B0:060A
		if (_04bf_0e7c >= 0) {
			//^01B0:060E
			_04bf_1938 = _04bf_0e7c;
		}
	}
	//^01B0:0611
	sk0e80 *bp04 = &_04bf_0e80[_04bf_1938];
	//^01B0:0622
	_04bf_185e = glbMouseXPos - bp04->b0;
	//^01B0:0634
	_04bf_1860 = glbMouseYPos - bp04->b1;
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
	if (_04bf_17a8 != 0)
		return;
	_04bf_17a8++;
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
void SkWinCore::_01b0_073d() //#DS=04BF
{
	//^01B0:073D
	//^01B0:0740
	if (_04bf_17a8 != 0) {
		//^01B0:0747
		IBMIO_HIDE_MOUSE_CURSOR();
		//^01B0:074C
		_04bf_17a8--;
	}
	//^01B0:0750
	return;
}

//^01B0:0752
void SkWinCore::IBMIO_MOUSE_EVENT_RECEIVER(Bit16u cursorx, Bit16u cursory, Bit16u buttons) //#DS=04BF
{
	//^01B0:0752
	Bit16u di = cursorx;
	Bit16u si = cursory;
	glbMouseXPos = di;
	glbMouseYPos = si;
	//printf("IBMIO_MOUSE_EVENT_RECEIVER\n");
	//^01B0:0765
	if ((di != glbMousePreviousXPos || si != glbMousePreviousYPos) && _04bf_17a2 <= 0) {
		//^01B0:0778
		_01b0_073d();
		_01b0_05ae();
		glbMousePreviousXPos = di;
		glbMousePreviousYPos = si;
	}
	//^01B0:0788
	glbMouseButtonState = ((buttons >> 1) & 1) | ((buttons << 1) & 2);
	//^01B0:079D
	_04bf_0e7c = buttons ^ _04bf_1934;
	//^01B0:07A7
	if (_04bf_0e7c != 0 && _04bf_03c6 != 0) {
		//^01B0:07B2
		if ((_04bf_0e7c & 1) != 0) {
			//^01B0:07BA
			(this->*_04bf_179e)(di, si, ((buttons & 1) != 0) ? 2 : 4) INDIRECT_CALL;
		}
		//^01B0:07D0
		if ((_04bf_0e7c & 2) != 0) {
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
void SkWinCore::_01b0_0aa8() //#DS=04BF
{
	//printf("MOUSE__01b0_0aa8\n");
	//^01B0:0AA8
	LOADDS(0x3083);
	//^01B0:0AB3
	if ((_04bf_17a2--) == 1) {
		//^01B0:0ABF
		LOCK_MOUSE_EVENT();
		_01b0_05ae();
		//^01B0:0AC7
		glbMousePreviousXPos = glbMouseXPos;
		glbMousePreviousYPos = glbMouseYPos;
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
	_01b0_0aa8() CALL_IBMIO;
}


//^01B0:08D8
void SkWinCore::_01b0_08d8()
{
	//^01B0:08D8
	ENTER(0);
	//^01B0:08DB
	_04bf_17a2 = 1;
	_04bf_03c6 = 0;
	_04bf_17a8 = 0;
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
	_01b0_092d = _int33_mouse_callback;
	glbMouseXPos = mice_x;
	glbMouseYPos = mice_y;
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
void SkWinCore::IBMIO_MOUSE_HANDLER()
{
	X16 bx = mice_btn; // buttons
	X16 cx = mice_x; // x
	X16 dx = mice_y; // y

	//printf("IBMIO_MOUSE_HANDLER\n");
	//SkD((DLV_MOUSE,"DBG: M(%3d,%3d) Btn:(%2d) C(%3d,%3d) P(%3d,%3d)\n", glbMouseXPos, glbMouseYPos, mice_btn, cx, dx, glbMousePreviousXPos, glbMousePreviousYPos));

	//^01B0:0985
	if (_01b0_0933 == 0) {
		//^01B0:0990
		bx &= 3;
		if (sysMousePositionCaptured != 0) {
			cx = glbMouseXPos;
			dx = glbMouseYPos;
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