#include <StdAfx.h>	// Required for MVC6 compilation

#include <SkVer.h>

#include <KCORE.H>
#include <SkDebug.h> // for Unr();

//#define UseAltic 0 // SPX: not Altic here

//^0759:016C
void SkWinCore::ANIM_FILL_SEQ_4BPP(U16 offDst, i16 clr, U16 cnt)
{
	ATLASSERT(cnt != 0);

	//^0759:016C
	ENTER(0);
	//^0759:0170
	U8 *esdi = _089c_0354;
	U16 bx = offDst;
	U16 cx = cnt;
	U8 ah = U8(clr);
	bool carry = (bx & 1) != 0;
	bx >>= 1;
	if (!carry) {
		//^0759:0182
		esdi += bx;
	}
	else {
		//^0759:0186
		esdi += bx;
		U8 al = (*esdi & 0xf0) | ah;
		stosb(esdi, al);
		if (--cx == 0) return;
	}
	//^0759:0193
	U8 al = (ah << 4) | ah;
	ah = al;
	bool carry2 = (cx & 1) != 0;
	cx >>= 1;
	bool carry3 = (cx & 1) != 0;
	cx >>= 1;
	if (carry3) {
		stosb(esdi, al);
	}
	U16 ax = al | (ah << 8);
	for (; cx != 0; cx--) stosw(esdi, ax);
	if (carry2) {
		//^0759:01B2
		al = *esdi;
		ax &= 0xf00f;
		al |= U8(ax >> 8);
		stosb(esdi, al);
	}
	//^0759:01BB
	return;
}
//^0759:013B
void SkWinCore::ANIM_SETPIXEL_SEQ_4BPP(U16 offDst, i8 clr)
{
	//^0759:013B
	ENTER(0);
	//^0759:013F
	U8 *esdi = _089c_0354;
	U16 bx = offDst;
	bool carry = (bx & 1) != 0;
	bx >>= 1;
	if (carry) {
		//^0759:014B
		esdi += bx;
		U8 al = *esdi;
		al &= 0xf0;
		al |= clr;
		stosb(esdi, al);
	}
	else {
		//^0759:0158
		esdi += bx;
		U8 al = *esdi;
		al &= 0x0f;
		al |= clr << 4;
		stosb(esdi, al);
	}
	//^0759:0169
	return;
}
//^0759:01BE
void SkWinCore::ANIM_BLIT_TO_MEMORY_ROW_4TO4BPP(U16 offSrc, U16 offDst, U16 width)
{
#if UseAltic
	_4976_5e6a = _089c_0354;
	_4976_5e64 = _089c_0348;

	FIRE_BLIT_TO_MEMORY_ROW_4TO4BPP(offSrc, offDst, width);
#else
	Unr();
#endif
}

//^00EB:0008
void SkWinCore::IBMIO_LOAD_4TO8BPP_PAL(const U8 *pal)
{
	//^00EB:0008
	ENTER(0);
	//^00EB:000D
	U8 *esdi = _00eb_0023;
	const U8 *dssi = pal;
	U16 cx = 16;
	for (; cx != 0; cx--) movsb(esdi, dssi);
	//^00EB:0021
	//^00EB:0033
	return;
}


//^00EB:070C
void SkWinCore::_00eb_070c(U8 *buffSrc, U16 yy, U16 zz, U16 ww) //#DS=04BF
{
	//^00EB:070C
	ENTER(0);
	//^00EB:070F
	LOADDS(0x0c48);
	//^00EB:0715
	_04bf_0cf0 = buffSrc;
	IBMIO_LOAD_4TO8BPP_PAL(_04bf_00dc);
	IBMIO_BLIT_ROW_4TO8BPP(yy, zz, ww);
	//^00EB:073D
	return;
}



//^00EB:0037
void SkWinCore::IBMIO_BLIT_ROW_4TO8BPP(U16 offSrc, U16 offDst, U16 size)
{
	//^00EB:0037
	ENTER(0);
	//^00EB:003C
	U8 *dsbx = _00eb_0023;
	U8 *esdi = _04bf_0e34 + offDst;
	U16 cx = size;
	const U8 *dssi = _04bf_0cf0;
	bool carry1 = (offSrc & 1) != 0;
	offSrc >>= 1;
	dssi += offSrc;
	if (carry1) {
		//^00EB:005C
		U8 al = lodsb(dssi);
		al &= 0x0f;
		al = dsbx[al];
		if (--cx == 0) return;
	}
	//^00EB:0065
	bool carry2 = (cx & 1) != 0;
	cx >>= 1;
	if (cx != 0) {
		//^00EB:0069
		bool carry3 = (cx & 1);
		cx >>= 1;
		if (cx != 0) {
			//^00EB:006E
			U16 bp = 0x0f0f;
			do {
				//^00EB:0073
				U16 ax = lodsw(dssi);
				U16 dx = ax >> 4;
				dx &= bp;
				ax &= bp;
				U8 ah = dsbx[ax & 255];
				U8 al = dsbx[dx & 255];
				U8 dh = dsbx[ax >> 8];
				U8 dl = dsbx[dx >> 8];
				ax = (ah << 8) | al;
				dx = (dh << 8) | dl;
				stosw(esdi, ax);
				stosw(esdi, dx);
				//^00EB:008F
			} while (--cx != 0);
		}
		if (carry3) {
			//^00EB:0095
			U8 al = lodsb(dssi);
			U8 ah = al;
			al >>= 4;
			al = dsbx[al];
			stosb(esdi, al);
			al = ah & 15;
			al = dsbx[al];
			stosb(esdi, al);
		}
	}
	//^00EB:00A6
	if (carry2) {
		//^00EB:00A8
		U8 al = lodsb(dssi);
		al >>= 4;
		al = dsbx[al];
		stosb(esdi, al);
	}
	//^00EB:00B0
	//^00EB:00B2
	return;
}
