//------------------------------------------------------------------------------
// SK-MEMORY-CACHE
//	Handling file operation
//------------------------------------------------------------------------------

#include <skver.h>

#include <skparam.h>
#include <skdebug.h>
#include <skcore.h>



//^3E74:48C9
// _3e74_48c9 renamed _3e74_48c9_MEMENT
mement *SkWinCore::_3e74_48c9_MEMENT(U16 mementi)
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:48C9
	//^3E74:48CF
	mement *bp04 = tlbMementsPointers[mementi];
	TEST_MEMENT(bp04);
	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(bp04))
		return NULL;
	//^3E74:48E8
	U16 si = bp04->w4();
	//^3E74:48EF
	if (si == 0xffff || si == 0xfffe) {
		//^3E74:48F9
		return bp04;
	}
	//^3E74:4902
	mement *bp0c = bp04;
	//^3E74:490E
	if (si == 0) {
		//^3E74:4915
		U16 di = bp04->w8();
		//^3E74:491E
		if (di == 0xffff) {
			//^3E74:4923
			_4976_5d90 = (bp04 != _4976_5c8c) ? bp04 : NULL;
			//^3E74:494B
			bp04->w4(1);
			//^3E74:4954
			//^3E74:48F9
			return bp04;
		}
		//^3E74:4956
		mement *bp08 = tlbMementsPointers[di];
		TEST_MEMENT(bp08);
		//^3E74:496E
		U16 bp0e = bp04->w6();
		//^3E74:4978
		if (bp0e == 0xffff) {
			//^3E74:497D
			_4976_5d5e = bp08;
		}
		else {
			//^3E74:498C
			bp04 = tlbMementsPointers[bp0e];
			TEST_MEMENT(bp04);
			//^3E74:49A5
			bp04->w8(di);
		}
		//^3E74:49AC
		bp08->w6(bp0e);
		//^3E74:49B6
		bp0c->w4(0xffff);
		//^3E74:49BF
		RECYCLE_MEMENTI(mementi, 0);
		//^3E74:49CA
	}
	else {
		//^3E74:49CD
		if (si < 0xfffd) {
			//^3E74:49D2
			si++;
		}
		//^3E74:49D3
		bp04->w4(si);
		//^3E74:49DA
		if (bp04 == _4976_5d70) {
			//^3E74:49EF
			//^3E74:48F9
			return bp04;
		}
		//^3E74:49F2
		U16 di = bp04->w8();
		//^3E74:49FB
		mement *bp08 = tlbMementsPointers[di];
		TEST_MEMENT(bp08);
		//^3E74:4A11
		if (bp08->w4() >= si) {
			//^3E74:4A1A
			//^3E74:48F9
			return bp04;
		}
		//^3E74:4A1D
		U16 bp0e = bp04->w6();
		//^3E74:4A27
		if (bp0e == 0xffff) {
			//^3E74:4A2C
			_4976_5d90 = bp08;
			_4976_5d5e = bp08;
			//^3E74:4A40
			bp08->w6(0xffff);
		}
		else {
			//^3E74:4A4B
			if (bp04 == _4976_5d90) {
				//^3E74:4A5F
				_4976_5d90 = bp08;
			}
			//^3E74:4A6C
			bp04 = tlbMementsPointers[bp0e];
			TEST_MEMENT(bp04);
			//^3E74:4A85
			bp04->w8(di);
			bp08->w6(bp0e);
		}
		//^3E74:4A96
		if (_4976_5d90 == _4976_5c8c) {
			//^3E74:4AAC
			_4976_5d90 = NULL;
		}

		while (true) {
			//^3E74:4AB8
			bp04 = bp08;
			bp0e = di;
			di = bp04->w8();
			//^3E74:4AD0
			if (di == 0xffff) {
				//^3E74:4AD5
				bp04->w8(mementi);
				//^3E74:4ADC
				bp0c->w6(bp0e);
				bp0c->w8(0xffff);
				//^3E74:4AEC
				_4976_5d70 = bp0c;
			}
			else {
				//^3E74:4AFB
				bp08 = tlbMementsPointers[di];
				TEST_MEMENT(bp08);
				//^3E74:4B13
				if (bp08->w4() < si)
					//^3E74:4B1A
					continue;
				//^3E74:4B1C
				bp04->w8(mementi);
				bp0c->w6(bp0e);
				bp0c->w8(di);
				bp08->w6(mementi);
			}
			break;
		}
	}
	//^3E74:4B3E
	return bp0c;
}

//^3E74:5561
void SkWinCore::FREE_CACHE_INDEX(U16 cacheIndex)
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:5561
	//^3E74:5566
    i16 si = cacheIndex;
		//^3E74:5569
	if (_4976_5d36 == -1 || _4976_5d36 > si) {
		//^3E74:5576
		_4976_5d36 = si;
	}
	//^3E74:557A
	ATLASSERT(si < _4976_5d24);
	Bit32u bp04 = _4976_5c86[si];
	//^3E74:5592
	U16 bp06;
	if (FIND_ICI_FROM_CACHE_HASH(bp04, &bp06) != 0) {
		//^3E74:55A8
		_4976_5c86[si] = 0;
		//^3E74:55BE
		cd.mc._4976_5c92_cache--;
		//^3E74:55C2
		MOVE_MEMORY(
			&_4976_5c7e[bp06 +1],
			&_4976_5c7e[bp06],
			(cd.mc._4976_5c92_cache - bp06) << 1
			);
	}
	//^3E74:55F6
	return;
}



//^3E74:4549
void SkWinCore::_3e74_4549(U16 xx)
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:4549
	//^3E74:454F
	mement *bp04 = tlbMementsPointers[xx];
	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(bp04))
		return;
	TEST_MEMENT(bp04);
	//^3E74:4568
	if (bp04 != NULL) {
		//^3E74:457C
		if (bp04->w4() != 0xffff) {
			//^3E74:4589
			U16 di = bp04->w6();
			U16 si = bp04->w8();
			mement *bp08;
			//^3E74:4594
			if (di == 0xffff) {
				//^3E74:4599
				if (si == 0xffff) {
					//^3E74:459E
					_4976_5d90 = NULL;
					_4976_5c8c = NULL;
					_4976_5d70 = NULL;
					_4976_5d5e = NULL;
				}
				else {
					//^3E74:45C1
					bp08 = tlbMementsPointers[si];
					TEST_MEMENT(bp08);
					//^3E74:45D9
					_4976_5d5e = bp08;
					//^3E74:45E0
					bp08->w6(0xffff);
					//^3E74:45E9
					goto _4682;
				}
			}
			else {
				//^3E74:45EC
				bp08 = tlbMementsPointers[di];
				TEST_MEMENT(bp08);
				//^3E74:4604
				bp08->w8(si);
				//^3E74:460B
				if (si == 0xffff) {
					//^3E74:4610
					if (bp04 == _4976_5c8c) {
						//^3E74:4625
						_4976_5c8c = NULL;
					}
					else {
						//^3E74:4633
						if (bp04 == _4976_5d90) {
							//^3E74:4648
							_4976_5d90 = NULL;
						}
					}
					//^3E74:4654
					_4976_5d70 = bp08;
				}
				else {
					//^3E74:4663
					bp08 = tlbMementsPointers[si];
					TEST_MEMENT(bp08);
					//^3E74:467B
					bp08->w6(di);
				}
				//^3E74:4682
_4682:
				if (bp04 == _4976_5d90) {
					//^3E74:4696
					if (bp08 == _4976_5c8c) {
						//^3E74:46AB
						_4976_5d90 = NULL;
					}
					else {
						//^3E74:46B9
						_4976_5d90 = bp08;
					}
				}
				else {
					//^3E74:46C8
					if (_4976_5c8c == bp04) {
						//^3E74:46DC
						_4976_5c8c = bp08;
					}
				}
			}
		}
	}
	//^3E74:46E9
	bp04->w8(0xffff);
	bp04->w6(0xffff);
	bp04->w4(0xffff);
	//^3E74:46FD
	return;
}

//^3E74:0C8C
void SkWinCore::_3e74_0c8c(mement *ref)
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:0C8C
	//^3E74:0C90
	mement *bp08 = ref->pv4();
	mement *bp04 = ref->pv8();
	//^3E74:0CAF
	if (bp08 == NULL) {
		//^3E74:0CC0
		if (bp04 == NULL) {
			//^3E74:0CD1
			_4976_5d94 = NULL;
		}
		else {
			//^3E74:0CDF
			_4976_5d94 = bp04;
			//^3E74:0CEC
			bp04->pv4(NULL);
		}
	}
	else {
		//^3E74:0CFD
		bp08->pv8(bp04);
		//^3E74:0D0E
		if (bp04 != NULL) {
			//^3E74:0D1F
			bp04->pv4(bp08);
		}
	}
	//^3E74:0D30
}

//^3E74:0D32
void SkWinCore::_3e74_0d32(mement *ref)
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:0D32
	//^3E74:0D37
	mement *bp04 = ref;
	//^3E74:0D43
	if (_4976_5d94 == NULL) {
		//^3E74:0D55
		_4976_5d94 = bp04;
		//^3E74:0D62
		bp04->pv8(NULL);
		//^3E74:0D71
_0d71:
		bp04->pv4(NULL);
		//^3E74:0D79
		return;
	}
	//^3E74:0D7C
	mement *bp08 = _4976_5d94;
	//^3E74:0D89
	i32 bp0c = bp04->dw0();
	//^3E74:0D99
	if (bp08->dw0() <= bp0c) {
		//^3E74:0DAF
		_4976_5d94 = bp04;
		//^3E74:0DBC
		bp04->pv4(NULL);
		//^3E74:0DCB
		bp04->pv8(bp08);
		//^3E74:0DD9
		bp08->pv4(bp04);
		//^3E74:0DE2
		goto _0d71;
	}

	//^3E74:0DE4
	U16 si = 0;
	//^3E74:0DE6
	while ((bp04 = bp08->pv8()) != NULL) {
		//^3E74:0DE8
		if (bp04->dw0() <= bp0c) {
			//^3E74:0DFE
			bp04->pv4(ref);
			//^3E74:0E0F
			bp08->pv8(ref);
			//^3E74:0E1A
			ref->pv4(bp08);
			//^3E74:0E2B
			ref->pv8(bp04);
			//^3E74:0E39
			si = 1;
			//^3E74:0E3C
			break;
		}
		//^3E74:0E3E
		bp08 = bp04;
		//^3E74:0E4A
	}
	//^3E74:0E6D
	if (si == 0) {
		//^3E74:0E71
		bp08->pv8(ref);
		//^3E74:0E82
		ref->pv4(bp08);
		//^3E74:0E93
		ref->pv8(NULL);
	}
	//^3E74:0E9F
	return;
}

bool SkWinCore::ValidateMements(bool display = false) {
	bool ok = true;
	X16 i = 0;
	for (U16 x = 0; x < glbNumberOfMements; x++) {
		mement *p = tlbMementsPointers[x];
		if (p != NULL) {
			if (display) SkD((DLV_CPX, "M#%03d %p ", x, p));
			int v0 = -p->dw0();
			int v1 = -READ_I32(p,-p->dw0() -4);
			if (v0 != v1) 
				ok = false;
			if (display) SkD((DLV_CPX, "%11d %11d %c ", v0, v1, (v0==v1)?'|':'#'));
			if (display) {
				int ref1 = -1;
				for (i = 0; i < glbGDatNumberOfData; i++) {
					if (_4976_5c82[i] == x) {
						ref1 = i;
						break;
					}
				}
				SkD((DLV_CPX, "%4d ", ref1));
				int ref2 = 0;
				for (i = 0; i < _4976_5d24; i++) {
					if (_4976_5d08[i] == x) {
						ref2 = _4976_5c86[i];
						break;
					}
				}
				SkD((DLV_CPX, "%08X ", ref2));
			}
			if (display) SkD((DLV_CPX, "\n"));
		}
	}
	return ok;
}

//^3E74:28DE
void SkWinCore::FREE_INDEXED_MEMENT(U16 index)
{
	// index: if (index&0x8000)==0x8000, cacheindex.
	// index: if (index&0x8000)==0x0000, raw data index.

	// cqOk @ 21:03 2007/02/21

	//^3E74:28DE
	//^3E74:28E4
	U16 di = index;
	//^3E74:28E7
	if (di == _4976_4809) {
		//^3E74:28ED
		_4976_4809 = 0xffff;
	}
	//^3E74:28F3
	U16 si = QUERY_MEMENTI_FROM(di);
	//^3E74:28FB
	if (si == 0xffff)
		//^3E74:2900
		return;
	//^3E74:2903
	if ((di & 0x8000) == 0) {
		//^3E74:2909
		_4976_5c82[di] = 0xffff;
	}
	else {
		//^3E74:291A
		di &= 0x7fff;
		//^3E74:291E
		ATLASSERT(di < _4976_5d24);
		_4976_5d08[di] = 0xffff;
		//^3E74:292D
		if (cd.mc._4976_5c90_cache == 0) {
			//^3E74:2934
			FREE_CACHE_INDEX(di);
		}
	}
	//^3E74:293B
	_3e74_4549(si);
	//^3E74:2942
	mement *bp04 = tlbMementsPointers[si];
	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(bp04))
		return;
	TEST_MEMENT(bp04);
	//^3E74:295A
	tlbMementsPointers[si] = NULL;
	//^3E74:2970
	_4976_5c9e--;
	//^3E74:2974
	if (si < _4976_5c78 || _4976_5c78 == 0xffff) {
		//^3E74:2981
		_4976_5c78 = si;
	}
	//^3E74:2985
	i32 bp0c = -bp04->dw0();
	//^3E74:299C
	_4976_5cf8 += bp0c;
	SkD((DLV_CPX,"CPX: CPX Free  Ci#%02d Mi#%02d (%5u) = %p\n"
		, (index & 0x8000) ? (index & 0x7fff) : -1
		, si
		, bp0c
		, bp04));
	//^3E74:29A7
	if (bp04 != _4976_5ce6) {
		//^3E74:29BC
		i32 bp10 = READ_I32(bp04,-4);
		ATLASSERT(-BUFF_SIZE_MAX < bp10 && bp10 < BUFF_SIZE_MAX);
		//ATLASSERT(-60000 < bp10 && bp10 < 60000);
		//^3E74:29DE
		if (bp10 > 0) {
			//^3E74:29E8
			PTR_PSBA(bp04,bp10);
			TEST_MEMENT(bp04);
			//^3E74:29F8
			_3e74_0c8c(bp04);
			//^3E74:2A04
			bp0c += bp10;
		}
	}
	//^3E74:2A10
	if ((void *)PTR_PADD(bp04,bp0c) == (void *)_4976_5cb2) {
		//^3E74:2A30
		_4976_5cb2 = bp04;
		return;
	}
	//^3E74:2A40
	mement *bp08 = reinterpret_cast<mement *>(PTR_PADD(bp04,bp0c));
	TEST_MEMENT(bp08);
	//^3E74:2A59
	i32 bp10 = bp08->dw0();
	//^3E74:2A68
	if (bp10 > 0) {
		//^3E74:2A72
		_3e74_0c8c(bp08);
		//^3E74:2A7E
		bp0c += bp10;
	}
	//^3E74:2A8A
	bp04->dw0(bp0c);
	//^3E74:2A9A
	WRITE_I32(bp04,bp0c -4,bp0c);
	//^3E74:2AC9
	_3e74_0d32(bp04);
	//^3E74:2AD5
	return;
}

//^3E74:4471
// _3e74_4471 renamed _3e74_4471_CACHE
U16 SkWinCore::_3e74_4471_CACHE()
{
	//^3E74:4471
	//^3E74:4475
	i16 si = _4976_5d36;
	//^3E74:447A
	if (si >= 0) {
		//^3E74:447E
		cd.mc._4976_5c92_cache++;
		//^3E74:4482
		if (cd.mc._4976_5c92_cache >= _4976_5d24) {
			//^3E74:448B
			_4976_5d36 = -1;
		}
		else {
			do {
				//^3E74:4493
				_4976_5d36++;
				//^3E74:4497
				ATLASSERT(_4976_5d36 < _4976_5d24);
			} while (_4976_5d08[_4976_5d36] != 0xffff);
		}
	}
	//^3E74:44A8
	ATLASSERT(0 <= si && si < _4976_5d24);
	return si;
}

//^3E74:54A1
U16 SkWinCore::INSERT_CACHE_HASH_AT(Bit32u cacheHash, U16 ici)
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:54A1
	//^3E74:54A6
	if (cd.mc._4976_5c92_cache == _4976_5d24) {
		//^3E74:54AF
		mement *bp04 = _4976_5d5e;
		//^3E74:54B6
		for (; (bp04->w10() & 0x8000) == 0; ) {
			//^3E74:54B8
			bp04 = tlbMementsPointers[bp04->w8()];
			TEST_MEMENT(bp04);
		}
		//{
		//	mement *p = _4976_5d5e;
		//	while (p != NULL) {
		//		SkD((DLV_CPX, "ME: %p I %04X  Mi %04X \n", p, p->w10(), p->w8()));
		//		p = tlbMementsPointers[p->w8()];
		//	}
		//	for (U16 x=0; x<_4976_5c92; x++) {
		//		U16 i = _4976_5c7e[x];
		//		U32 hash = _4976_5c86[i];
		//		SkD((DLV_CPX, "HI: %04X %04X %08X \n", x, i, hash));
		//	}
		//}

		//^3E74:54E0
		FREE_INDEXED_MEMENT(bp04->w10());
		//^3E74:54E9
		FIND_ICI_FROM_CACHE_HASH(cacheHash, &ici);
	}
	//^3E74:54FB
	U16 si = _3e74_4471_CACHE();
	//^3E74:5501
	MOVE_MEMORY(
		&_4976_5c7e[ici],
		&_4976_5c7e[ici +1],
		(cd.mc._4976_5c92_cache - ici -1) << 1
		);
	//^3E74:5536
	_4976_5c7e[ici] = si;
	//^3E74:5544
	_4976_5c86[si] = cacheHash;
	//^3E74:555C
	return si;
}



//^3E74:44AD
void SkWinCore::_3e74_44ad()
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:44AD
	_4976_5d2a = glbGameTick;
	_4976_5d90 = NULL;
	mement *bp04 = _4976_5c8c;
	//^3E74:44E6
	if (bp04 == NULL) {
		//^3E74:44E8
		bp04 = _4976_5d70;
		//^3E74:4502
		if (bp04 == NULL)
			return;
	}
	//^3E74:4504
	while (bp04->w4() != 0) {
		//^3E74:4506
		if (bp04->w4() <= 0xfffd) {
			//^3E74:4510
			bp04->w4(0);
		}
		//^3E74:4516
		U16 si = bp04->w6();
		if (si == 0xffff)
			return;
		//^3E74:4524
		bp04 = tlbMementsPointers[si];
		TEST_MEMENT(bp04);
		//^3E74:453C
	}
}

//^3E74:0C62
U16 SkWinCore::QUERY_MEMENTI_FROM(U16 xx)
{
	// xx&0x8000==0x8000: cacheindex-to-mementi
	// xx&0x8000==0x0000: raw data index to mementi

	// returns 0xffff if no match found.

	//^3E74:0C62
	U16 si = xx;
	if ((si & 0x8000) != 0) {
		if (SkCodeParam::bUseFixedMode && (si & 0x7fff) >= 128)	// SPX: protection to avoid further invalid pointer
			return 0xFFFF;
		//^3E74:0C6F
		return _4976_5d08[si & 0x7fff];
	}
	else {
		//^3E74:0C7C
		if (SkCodeParam::bUseFixedMode && (si & 0x7fff) >= glbGDatNumberOfData) // SPX: protection to avoid further invalid pointer
			return 0xFFFF;
		return _4976_5c82[si];
	}
}

//^3E74:55F9
U16 SkWinCore::ADD_CACHE_HASH(Bit32u cacheHash, U16 *piYaCacheIndex)
{
	// returns 0 if inserted new one, 1 if already exists.

	//^3E74:55F9
	//^3E74:55FE
	if (glbGameTick != _4976_5d2a) {
		//^3E74:5611
		_3e74_44ad();
	}
	//^3E74:5615
	U16 bp02;
	if (FIND_ICI_FROM_CACHE_HASH(cacheHash, &bp02) != 0) {
		//^3E74:562B
		*piYaCacheIndex = _4976_5c7e[bp02];
		//^3E74:563F
		U16 si = QUERY_MEMENTI_FROM(*piYaCacheIndex | 0x8000);
		//^3E74:564D
		_3e74_48c9_MEMENT(si);
		//^3E74:5653
		return 1;
	}
	//^3E74:5658
	*piYaCacheIndex = INSERT_CACHE_HASH_AT(cacheHash, bp02);
	//^3E74:566E
	return 0;
}

//^3E74:5817
U8 *SkWinCore::QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(U16 cacheIndex)
{
    return reinterpret_cast<U8 *>(&_3e74_48c9_MEMENT(QUERY_MEMENTI_FROM(cacheIndex | 0x8000))[1]); // +18 bytes
}

//^3E74:583A
void SkWinCore::_3e74_583a(U16 xx)
{
	U16 si = QUERY_MEMENTI_FROM(xx | 0x8000);
	if (si != 0xFFFF) {
		_3e74_4549(si);
	}
	return;
}

//^3E74:4416
U16 SkWinCore::FIND_FREE_MEMENTI()
{
	//^3E74:4416
	//^3E74:441A
	U16 si = _4976_5c78;
	//^3E74:441F
	if (si == 0xffff) {
		//^3E74:4424
		FREE_INDEXED_MEMENT(_4976_5d5e->w10());
		//^3E74:4431
		si = _4976_5c78;
	}
	//^3E74:4435
	_4976_5c9e++;
	//^3E74:4439
	if (_4976_5c9e >= glbNumberOfMements) {
		//^3E74:4442
		_4976_5c78 = 0xffff;
	}
	else {
		do {
			//^3E74:444A
			_4976_5c78++;
			//^3E74:444E
			ATLASSERT(_4976_5c78 < glbNumberOfMements);
		} while (tlbMementsPointers[_4976_5c78] != NULL);
	}
	//^3E74:446C
	return si;
}

//^3E74:2AD9
void SkWinCore::GUARANTEE_FREE_CPXHEAP_SIZE(i32 buffSize)
{
	ATLASSERT(buffSize > 0 && buffSize < BUFF_SIZE_MAX);

	//^3E74:2AD9
	//^3E74:2ADC
	for (; _4976_5cf8 < buffSize; ) {
		//^3E74:2ADE
		if (_4976_5d5e == NULL) {
			//^3E74:2AF0
			RAISE_SYSERR(SYSTEM_ERROR__FREE_CPXHEAP_SIZE);
		}
		//^3E74:2AF8
		if (_4976_4809 != 0xffff && _4976_4809 != _4976_4807) {
			//^3E74:2B08
			FREE_INDEXED_MEMENT(_4976_4809);
		}
		else {
			//^3E74:2B0E
			FREE_INDEXED_MEMENT(_4976_5d5e->w10());
		}
		//^3E74:2B1B
	}
	//^3E74:2B2E
	return;
}



//^3E74:2B30
void SkWinCore::_3e74_2b30()
{
	int iDebugLoopCount = 0;
	// cqOk @ 21:03 2007/02/21

	//^3E74:2B30
	//^3E74:2B36
	if (_4976_5d94 != NULL) {	// mement
		//^3E74:2B4B
		mement *bp04 = _4976_5ce6;
		mement *bp08 = _4976_5ce6;

		do {
			//^3E74:2B5E
			i32 bp0c = bp04->dw0();
			//printf("LOAD_DYN4>_3e74_2b30[MEMENT] (#%05d) bp0c (length) = %d\n", iDebugLoopCount, bp0c);
			// SPX: we can get stuck in the loop, let's assume that after 1000 loop we try to get out, or if the length is ridiculously too much
			if (SkCodeParam::bUseFixedMode && (iDebugLoopCount > 1000 || bp0c > 1024000))
				break;
			//^3E74:2B6E
			if (bp0c > 0) {
				//^3E74:2B78
				PTR_PADA(bp04, bp0c);
			}
			else {
				//^3E74:2B86
				bp0c = -bp0c;
				//^3E74:2B99
				if (bp08 != bp04) {
					//^3E74:2BAF
					U16 di = bp04->w10();
					//^3E74:2BB6
					U16 si = QUERY_MEMENTI_FROM(di);
					//^3E74:2BBE
					tlbMementsPointers[si] = bp08;
					//^3E74:2BD4
					COPY_MEMORY(bp04, bp08, bp0c);
					TEST_MEMENT(bp08);	// SPX TODO this part is prone to fail while changing level
					//^3E74:2BEE
					if (_4976_5d5e == bp04) {
						//^3E74:2C02
						_4976_5d5e = bp08;
					}
					//^3E74:2C0F
					if (_4976_5d70 == bp04) {
						//^3E74:2C23
						_4976_5d70 = bp08;
					}
					//^3E74:2C30
					if (_4976_5d90 == bp04) {
						//^3E74:2C44
						_4976_5d90 = bp08;
					}
					//^3E74:2C53
					else if (_4976_5c8c == bp04) {
						//^3E74:2C67
						_4976_5c8c = bp08;
					}
				}
				//^3E74:2C74
				PTR_PADA(bp04, bp0c);
				//^3E74:2C84
				PTR_PADA(bp08, bp0c);
			}
			iDebugLoopCount++; // debug
			//^3E74:2C94
		} while (bp04 != _4976_5cb2);	// at some point, this becomes impossible when bp04 never changes and we get stuck in the loop

		//^3E74:2CAC
		_4976_5cb2 = bp08;
		//^3E74:2CB9
		_4976_5d94 = NULL;
	}
	//^3E74:2CC5
	return;
}

//^3E74:2CC9
mement *SkWinCore::ALLOC_LOWER_CPXHEAP(i32 buffSize)
{
	// allocate memory from lower part of complex memory pool.
	// buffSize must include size of header&footer: sizeof(mement)+4 = 22
	ATLASSERT(buffSize > 0 && buffSize < BUFF_SIZE_MAX);

	//^3E74:2CC9
	//^3E74:2CCF
	GUARANTEE_FREE_CPXHEAP_SIZE(buffSize);
	//^3E74:2CDB
	mement *bp04;
	if (PTR_PSBP(_4976_5ce2,_4976_5cb2) >= buffSize) { // allocate on main memory
		//^3E74:2CFB
_2cfb:
		bp04 = _4976_5cb2;
		//^3E74:2D08
		PTR_PADA(_4976_5cb2,buffSize);
	}
	//^3E74:2D1B
	else if (_4976_5d94->dw0() < buffSize) {
		//^3E74:2D32
		_3e74_2b30();
		//^3E74:2D36
		goto _2cfb;
	}
	else {
		//^3E74:2D38
		bp04 = _4976_5d94;
		//^3E74:2D45
		U16 si = 0;
		mement *bp08;

		do {
			//^3E74:2D47
			if (bp04->dw0() == buffSize) {
				//^3E74:2D5B
_2d5b:
				si = 1;
				//^3E74:2D5E
				continue;
			}
			//^3E74:2D60
			if (bp04->dw0() >= buffSize) {
				//^3E74:2D76
				bp08 = bp04->pv8();
				//^3E74:2D8B
				if (bp08 != NULL)
					//^3E74:2D94
					goto _2da5;
			}
			//^3E74:2D96
			bp04 = _4976_5d94;
			//^3E74:2DA3
			goto _2d5b;
			//^3E74:2DA5
_2da5:
			bp04 = bp08;
			//^3E74:2DB1
		} while (si == 0);

		//^3E74:2DB5
		_3e74_0c8c(bp04);
		//^3E74:2DC1
		i32 bp0c = bp04->dw0() - buffSize;
		//^3E74:2DD7
		if (bp0c >= 30) {
			//^3E74:2DE5
			bp08 = reinterpret_cast<mement *>(PTR_PADD(bp04,buffSize));
			//^3E74:2DFC
			WRITE_I32(bp08,bp0c -4,bp0c);
			//^3E74:2E2A
			bp08->dw0(bp0c);
			//^3E74:2E3A
			_3e74_0d32(bp08);
		}
		else {
			//^3E74:2E42
			buffSize = bp04->dw0();
		}
	}
	//^3E74:2E52
	_4976_5cf8 -= buffSize;
	//^3E74:2E60
	bp04->dw0(-buffSize);
	//^3E74:2E71
	WRITE_I32(bp04,+buffSize -sizeof(i32),-buffSize);
	//^3E74:2EA9
	reinterpret_cast<mement *>(bp04)->w8(0xffff);
	reinterpret_cast<mement *>(bp04)->w6(0xffff);
	reinterpret_cast<mement *>(bp04)->w4(0xffff);
	//^3E74:2EBD
#if UseAltic
	{
		i32 size = -READ_I32(bp04,-bp04->dw0() -4);
		ATLASSERT(buffSize == size);

		// SkD((DLV_CPX, "CPX: ALLOC_LOWER_CPXHEAP(%6u) = %p\n", (Bitu)buffSize, bp04));
		memset(&bp04[1], 0xcc, buffSize -sizeof(mement) -sizeof(i32));
		// ATLASSERT((U32)bp04 != 0x00758132);
	}
#endif
	return reinterpret_cast<mement *>(bp04);
}

//^3E74:5708
U8 *SkWinCore::ALLOC_CPXHEAP_MEM(U16 index, Bit32u buffSize)
{
	//^3E74:5708
	//^3E74:570D
	buffSize += sizeof(mement) +4;
	//^3E74:5715
	if ((buffSize & 1) != 0) {
		//^3E74:5720
		buffSize++;
	}
	//^3E74:5728
	mement *bp04 = ALLOC_LOWER_CPXHEAP(buffSize);
	//^3E74:573B
	U16 si = FIND_FREE_MEMENTI();
	//^3E74:5740
	ATLASSERT(_4976_5d08[index] == U16(-1));
	_4976_5d08[index] = si;
	//^3E74:574E
	ATLASSERT(tlbMementsPointers[si] == 0);
	tlbMementsPointers[si] = bp04;
	//^3E74:5766
	bp04->w10(index | 0x8000);

	SkD((DLV_CPX,"DBG: CPX Alloc Ci#%02d Mi#%02d (%5u) = %p\n", index, si, 0U +buffSize, bp04));

	//^3E74:5773
	return reinterpret_cast<U8 *>(&bp04[1]); // +18 bytes
}

//^3E74:585A
// _3e74_585a renamed _3e74_585a_CACHE
void SkWinCore::_3e74_585a_CACHE(U16 xx, U16 yy)
{
	//^3E74:585A
	//^3E74:585E
	U16 si = QUERY_MEMENTI_FROM(xx | 0x8000);
	//^3E74:586C
	if (si == 0xffff) {
		//^3E74:5871
		FREE_CACHE_INDEX(xx);
	}
	else {
		//^3E74:587B
		RECYCLE_MEMENTI(si, yy);
	}
}


//^3E74:53EA
Bit32u SkWinCore::GET_TEMP_CACHE_HASH()
{
	//^3E74:53EA
	Bit32u bp04;
	U16 bp06;
	do {
		//^3E74:53EE
		bp04 = 0xffff0000 | (_4976_484b++);
		//^3E74:5401
	} while (FIND_ICI_FROM_CACHE_HASH(bp04, &bp06) != 0);
	//^3E74:5418
	return bp04;
}

//^3E74:5888
U16 SkWinCore::ALLOC_TEMP_CACHE_INDEX()
{
	U16 iCacheIndex; // pb02
	ADD_CACHE_HASH(GET_TEMP_CACHE_HASH(), &iCacheIndex);
	return iCacheIndex;
}



//^3E74:58A3
void SkWinCore::FREE_TEMP_CACHE_INDEX(U16 iCacheIndex)
{
	ENTER(0);
	_3e74_585a_CACHE(iCacheIndex, 0);
	FREE_INDEXED_MEMENT(iCacheIndex |0x8000);
	return;
}


//^3E74:5420
U16 SkWinCore::FIND_ICI_FROM_CACHE_HASH(Bit32u cacheHash, U16 *ici)
{
	//^3E74:5420
	//^3E74:5426
	i16 di = -1;
	i16 cx = cd.mc._4976_5c92_cache;

	while (true) {
		//^3E74:542E
		i16 si = (di + cx) >> 1;
		//^3E74:5436
		if (si == di) {
			//^3E74:543A
			*ici = si +1;
			//^3E74:5441
			return 0;
		}
		//^3E74:5445
		if (!SkCodeParam::bUsePowerDebug)
			ATLASSERT(si < _4976_5d24);
		Bit32u bp04 = _4976_5c86[_4976_5c7e[si]];
		//^3E74:5468
		if (cacheHash < bp04) {
			//^3E74:547A
			cx = si;
			//^3E74:5478
			continue;
		}
		//^3E74:547E
		if (cacheHash > bp04) {
			//^3E74:5490
			di = si;
			//^3E74:5492
			continue;
		}
		//^3E74:5494
		*ici = si;
		//^3E74:549A
		return 1;
	}
}

//^3E74:4701
void SkWinCore::RECYCLE_MEMENTI(U16 mementi, U16 yy)
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:4701
	//^3E74:4707
	U16 di = mementi;
	//^3E74:470C
	mement *bp04 = tlbMementsPointers[di];
	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(bp04))
		return;
	TEST_MEMENT(bp04);
	//^3E74:4722
	if (bp04->w4() != 0xffff) {
		//^3E74:472C
		_3e74_48c9_MEMENT(di);
		//^3E74:4733
		return;
	}
	//^3E74:4736
	mement *bp0c;
	mement *bp10;
	if (yy != 0) {
		//^3E74:473C
		bp04->w4(0xfffe);
		bp0c = _4976_5c8c;

		//^3E74:4752
		//^3E74:4786
		bp10 = _4976_5d70;
	}
	else {
		//^3E74:4754
		bp04->w4(1);
		//^3E74:475D
		bp0c = _4976_5d90;
		//^3E74:476A
		bp10 = _4976_5c8c;
		if (bp10 == NULL) {
			//^3E74:4786
			bp10 = _4976_5d70;
		}
	}

	//^3E74:4793
	if (bp0c != NULL) {
		//^3E74:47A4
		goto _483a;
	}
	//^3E74:47A7
	U16 bp12;
	U16 si;
	if (_4976_5d70 == NULL) {
		//^3E74:47B9
		bp04->w6(0xffff);
		_4976_5d5e = bp04;
		//^3E74:47CF
		goto _4822;
	}
	//^3E74:47D1
	if (_4976_5d90 == NULL) {
		//^3E74:47E3
		if (_4976_5c8c != NULL) {
			//^3E74:47F5
			bp0c = bp10;
			//^3E74:4801
			goto _483a;
		}
	}
	//^3E74:4803
	bp10->w8(di);
	bp12 = bp10->w10();
	//^3E74:4811
	si = QUERY_MEMENTI_FROM(bp12);
	//^3E74:481B
	bp04->w6(si);
	//^3E74:4822
_4822:
	bp04->w8(0xffff);
	_4976_5d70 = bp04;
	//^3E74:4838
	goto _48a3;

	//^3E74:483A
_483a:
	si = bp0c->w6();
	bp0c->w6(di);
	bp04->w6(si);
	//^3E74:484C
	U16 bp14;
	if (si != 0xffff) {
		//^3E74:4851
		mement *bp08 = tlbMementsPointers[si];
		TEST_MEMENT(bp08);
		//^3E74:4869
		bp04->w8(bp08->w8());
		//^3E74:4877
		bp08->w8(di);
		//^3E74:487E
		goto _48a3;
	}
	//^3E74:4880
	bp14 = QUERY_MEMENTI_FROM(bp0c->w10());
	//^3E74:488F
	bp04->w8(bp14);
	//^3E74:4896
	_4976_5d5e = bp04;

	//^3E74:48A3
_48a3:
	if (yy != 0) {
		//^3E74:48A9
		_4976_5c8c = bp04;
	}
	else {
		//^3E74:48B8
		_4976_5d90 = bp04;
	}
	//^3E74:48C5
	return;
}

// SPX: changed "void" to "int" in order to allow more code on return
int SkWinCore::TEST_MEMENT(mement *bp04)
{
	i32	iDW0ValLen = 0;
	i32 len = 0;
	//ATLASSERT(bp04 != NULL);
	// SPX: use CheckSafePointer to trap more unexpected mem values so that it fails at ASSERT instead of crashing after.
	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(bp04))
		return 0;
	ATLASSERT(CheckSafePointer(bp04));
	iDW0ValLen = (abs(-bp04->dw0())) -4;
	if (iDW0ValLen >= 65536)
		return 0;
	len = READ_I32(bp04,iDW0ValLen);
	if (SkCodeParam::bUseFixedMode)
	{
		if (bp04->dw0() != len)
		{
			len = bp04->dw0();
		}

	}
	ATLASSERT(bp04->dw0() == len);
	return 1; // test passed
}

//^3E74:5673
// _3e74_5673 renamed _3e74_5673_CACHE
X32 SkWinCore::_3e74_5673_CACHE(X32 cacheHash, U16 *piYaCacheIndex, X16 ifTryInsert)
{
	ENTER(2);
	if (glbGameTick != _4976_5d2a) {
		_3e74_44ad();
	}
	U16 bp02;
	if (FIND_ICI_FROM_CACHE_HASH(cacheHash, &bp02) == 0) {
		*piYaCacheIndex = (ifTryInsert != 0) ? INSERT_CACHE_HASH_AT(cacheHash, bp02) : 0xFFFF;
		return 0;
	}
	*piYaCacheIndex = _4976_5c7e[bp02];
	return *reinterpret_cast<i32 *>(_3e74_48c9_MEMENT(QUERY_MEMENTI_FROM(*piYaCacheIndex | 0x8000))) -22;
}


//^3E74:5788
// _3e74_5788 renamed _3e74_5788_CACHE
// only called related to creature thinking ??
U8 *SkWinCore::_3e74_5788_CACHE(U16 xx, i32 yy)
{
	ENTER(0);
	cd.mc._4976_5c90_cache = 1;
	FREE_INDEXED_MEMENT(xx|0x8000);
	cd.mc._4976_5c90_cache = 0;
	return ALLOC_CPXHEAP_MEM(xx, yy);
}

//^1C9A:0247
// _1c9a_0247 renamed _1c9a_0247_FREE_CACHE_FROM_CREATURE
void SkWinCore::_1c9a_0247_FREE_CACHE_FROM_CREATURE(ObjectID rl)
{
	ENTER(2);
	i16 iCreatureIndex = rl.DBIndex();	// si
	U16 piYaCacheIndex; // bp02
	if (_3e74_5673_CACHE(0x20000000 | iCreatureIndex | glbCurrentThinkingCreatureID.DBIndex(), &piYaCacheIndex, 0) != 0) {
		FREE_TEMP_CACHE_INDEX(piYaCacheIndex);
	}
	if (_3e74_5673_CACHE(0x28000000 | iCreatureIndex | glbCurrentThinkingCreatureID.DBIndex(), &piYaCacheIndex, 0) != 0) {
		FREE_TEMP_CACHE_INDEX(piYaCacheIndex);
	}
	return;
}

