//------------------------------------------------------------------------------
// SK-MEMORY-CACHE
//	Handling file operation
//------------------------------------------------------------------------------

#include <skver.h>

#include <skparam.h>
#include <skdebug.h>
#include <skcore.h>

#include <string.h>


//^3E74:48C9
// _3e74_48c9 renamed _3e74_48c9_MEMENT
mement* SkWinCore::_3e74_48c9_MEMENT(U16 mementi)
{
	// cqOk @ 21:03 2007/02/21

	mement* xMement = NULL;	// (bp04) can't hold more than glbNumberOfMements
	if (SkCodeParam::bUsePowerDebug && mementi >= glbNumberOfMements)
		return NULL;
	xMement = tblMementsPointers[mementi];	// (bp04) can't hold more than glbNumberOfMements
	TEST_MEMENT(xMement);
	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(xMement))
		return NULL;
	U16 si = xMement->w4();	// si
	if (si == 0xFFFF || si == 0xFFFE) {
		return xMement;
	}
	mement* xMement2 = xMement;	// bp0c
	if (si == 0) {
		U16 di = xMement->w8();
		if (di == 0xFFFF) {
			glbMement2 = (xMement != _4976_5c8c) ? xMement : NULL;
			xMement->w4(1);
			return xMement;
		}
		mement* xMement3 = tblMementsPointers[di];	// bp08
		TEST_MEMENT(xMement3);
		U16 bp0e = xMement->w6();
		if (bp0e == 0xffff) {
			_4976_5d5e = xMement3;
		}
		else {
			xMement = tblMementsPointers[bp0e];
			TEST_MEMENT(xMement);
			xMement->w8(di);
		}
		xMement3->w6(bp0e);
		xMement2->w4(0xFFFF);
		RECYCLE_MEMENTI(mementi, 0);
	}
	else {
		if (si < 0xFFFD) {
			si++;
		}
		xMement->w4(si);
		if (xMement == _4976_5d70) {
			return xMement;
		}
		U16 di = xMement->w8();
		mement* xMement3 = tblMementsPointers[di];	// bp08
		TEST_MEMENT(xMement3);
		if (xMement3->w4() >= si) {
			return xMement;
		}
		U16 bp0e = xMement->w6();
		if (bp0e == 0xFFFF) {
			glbMement2 = xMement3;
			_4976_5d5e = xMement3;
			xMement3->w6(0xFFFF);
		}
		else {
			if (xMement == glbMement2) {
				glbMement2 = xMement3;
			}
			xMement = tblMementsPointers[bp0e];
			TEST_MEMENT(xMement);
			xMement->w8(di);
			xMement3->w6(bp0e);
		}
		if (glbMement2 == _4976_5c8c) {
			glbMement2 = NULL;
		}

		while (true) {
			xMement = xMement3;
			bp0e = di;
			di = xMement->w8();
			if (di == 0xffff) {
				xMement->w8(mementi);
				xMement2->w6(bp0e);
				xMement2->w8(0xffff);
				_4976_5d70 = xMement2;
			}
			else {
				xMement3 = tblMementsPointers[di];
				TEST_MEMENT(xMement3);
				if (xMement3->w4() < si)
					continue;
				xMement->w8(mementi);
				xMement2->w6(bp0e);
				xMement2->w8(di);
				xMement3->w6(mementi);
			}
			break;
		}
	}
	return xMement2;
}

//^3E74:5561
void SkWinCore::FREE_CACHE_INDEX(U16 cacheIndex)
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:5561
	//^3E74:5566
    i16 si = cacheIndex;
		//^3E74:5569
	if (glbCacheRecyclerCurrentIndex == -1 || glbCacheRecyclerCurrentIndex > si) {
		//^3E74:5576
		glbCacheRecyclerCurrentIndex = si;
	}
	//^3E74:557A
	ATLASSERT(si < glbCacheRecyclerMax);
	Bit32u bp04 = _4976_5c86_cache_hash[si];
	//^3E74:5592
	U16 bp06;
	if (FIND_ICI_FROM_CACHE_HASH(bp04, &bp06) != 0) {
		//^3E74:55A8
		_4976_5c86_cache_hash[si] = 0;
		//^3E74:55BE
		cd.mc._4976_5c92_cache--;
		//^3E74:55C2
		MOVE_MEMORY(
			&_4976_5c7e_cache_ici[bp06 +1],
			&_4976_5c7e_cache_ici[bp06],
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
	mement *bp04 = tblMementsPointers[xx];
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
					glbMement2 = NULL;
					_4976_5c8c = NULL;
					_4976_5d70 = NULL;
					_4976_5d5e = NULL;
				}
				else {
					//^3E74:45C1
					bp08 = tblMementsPointers[si];
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
				bp08 = tblMementsPointers[di];
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
						if (bp04 == glbMement2) {
							//^3E74:4648
							glbMement2 = NULL;
						}
					}
					//^3E74:4654
					_4976_5d70 = bp08;
				}
				else {
					//^3E74:4663
					bp08 = tblMementsPointers[si];
					TEST_MEMENT(bp08);
					//^3E74:467B
					bp08->w6(di);
				}
				//^3E74:4682
_4682:
				if (bp04 == glbMement2) {
					//^3E74:4696
					if (bp08 == _4976_5c8c) {
						//^3E74:46AB
						glbMement2 = NULL;
					}
					else {
						//^3E74:46B9
						glbMement2 = bp08;
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
	bp04->w8(0xFFFF);
	bp04->w6(0xFFFF);
	bp04->w4(0xFFFF);
	//^3E74:46FD
	return;
}

//^3E74:0C8C
// SPX: _3e74_0c8c renamed MEMENT_3e74_0c8c
void SkWinCore::MEMENT_3e74_0c8c(mement *ref)
{
	// cqOk @ 21:03 2007/02/21

	mement* xMement1 = ref->pv4();	// bp08
	mement* xMement2 = ref->pv8();	// bp04
	if (xMement1 == NULL) {
		if (xMement2 == NULL) {
			glbMement1 = NULL;
		}
		else {
			glbMement1 = xMement2;
			xMement2->pv4(NULL);
		}
	}
	else {
		xMement1->pv8(xMement2);
		if (xMement2 != NULL) {
			xMement2->pv4(xMement1);
		}
	}
}


//^3E74:0D32
// SPX: _3e74_0d32 renamed MEMENT_3e74_0d32
void SkWinCore::MEMENT_3e74_0d32(mement* xMemEntRef)
{
	// cqOk @ 21:03 2007/02/21
	mement* xMement1 = xMemEntRef;	// bp04
	if (glbMement1 == NULL) {
		glbMement1 = xMement1;
		xMement1->pv8(NULL);
_0d71:
		xMement1->pv4(NULL);	// glbMement1->pv4(NULL);
		return;
	}
	mement* xMemEntry2 = glbMement1;	// bp08
	i32 bp0c = xMement1->dw0();
	if (xMemEntry2->dw0() <= bp0c) {
		glbMement1 = xMement1;
		xMement1->pv4(NULL);
		xMement1->pv8(xMemEntry2);
		xMemEntry2->pv4(xMement1);
		goto _0d71;
	}

	U16 si = 0;
	while ((xMement1 = xMemEntry2->pv8()) != NULL) {
		if (xMement1->dw0() <= bp0c) {
			xMement1->pv4(xMemEntRef);
			xMemEntry2->pv8(xMemEntRef);
			xMemEntRef->pv4(xMemEntry2);
			xMemEntRef->pv8(xMement1);
			si = 1;
			break;
		}
		xMemEntry2 = xMement1;
	}
	if (si == 0) {
		xMemEntry2->pv8(xMemEntRef);
		xMemEntRef->pv4(xMemEntry2);
		xMemEntRef->pv8(NULL);
	}
	return;
}

bool SkWinCore::ValidateMements(bool display = false) {
	bool ok = true;
	X16 i = 0;
	for (U16 x = 0; x < glbNumberOfMements; x++) {
		mement *p = tblMementsPointers[x];
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
					if (tblRawDataToMement[i] == x) {
						ref1 = i;
						break;
					}
				}
				SkD((DLV_CPX, "%4d ", ref1));
				int ref2 = 0;
				for (i = 0; i < glbCacheRecyclerMax; i++) {
					if (tblCacheToMement[i] == x) {
						ref2 = _4976_5c86_cache_hash[i];
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

	U16 iLocalCacheIndex = index;	// di / index
	if (iLocalCacheIndex == _4976_4809) {
		_4976_4809 = 0xFFFF;
	}
	U16 iMemEntryIndex = QUERY_MEMENTI_FROM(iLocalCacheIndex);	// si
	//printf("MEMENTI %d from CACHIDX %d\n", iMemEntryIndex, iLocalCacheIndex);
	if (iMemEntryIndex == 0xFFFF)
		return;
	if ((iLocalCacheIndex & 0x8000) == 0) {
		tblRawDataToMement[iLocalCacheIndex] = 0xFFFF;
	}
	else {
		iLocalCacheIndex &= 0x7FFF;
		ATLASSERT(iLocalCacheIndex < glbCacheRecyclerMax);
		tblCacheToMement[iLocalCacheIndex] = 0xFFFF;
		if (cd.mc._4976_5c90_cache == 0) {
			FREE_CACHE_INDEX(iLocalCacheIndex);
		}
	}
	_3e74_4549(iMemEntryIndex);
	mement* xMemEntry = tblMementsPointers[iMemEntryIndex];	// bp04
	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(xMemEntry))
		return;
	TEST_MEMENT(xMemEntry);
	tblMementsPointers[iMemEntryIndex] = NULL;
	glbMementIndex_4976_5c9e--;
	if (iMemEntryIndex < glbNextFreeMementIndex || glbNextFreeMementIndex == 0xFFFF) {
		glbNextFreeMementIndex = iMemEntryIndex;
	}
	i32 iMementSize = -(xMemEntry->dw0());// bp0c
//	printf("WHAT IS bp0c = %08x\n", iMementSize);
	_4976_5cf8 += iMementSize;
	SkD((DLV_CPX,"CPX: CPX Free  Ci#%02d Mi#%02d (%5u) = %p\n"
		, (index & 0x8000) ? (index & 0x7FFF) : -1
		, iMemEntryIndex
		, iMementSize
		, xMemEntry));
	if (xMemEntry != _4976_5ce6) {
		//printf("READ_I32 %p\n", xMemEntry);
		i32 iMemEntReadSize = READ_I32(xMemEntry,-4);	// bp10
//		printf("WHAT IS bp10 = %08x\n", iMemEntReadSize);
//		printf("AFTER READ_I32 %p\n", xMemEntry);
		ATLASSERT(-BUFF_SIZE_MAX < iMemEntReadSize && iMemEntReadSize < BUFF_SIZE_MAX);
		//ATLASSERT(-60000 < bp10 && bp10 < 60000);
		if (iMemEntReadSize > 0) {
			PTR_PSBA(xMemEntry,iMemEntReadSize);
			TEST_MEMENT(xMemEntry);
			MEMENT_3e74_0c8c(xMemEntry);
			iMementSize += iMemEntReadSize;
		}
	}
	if ((void *)PTR_PADD(xMemEntry,iMementSize) == (void *)_4976_5cb2) {
		_4976_5cb2 = xMemEntry;
		return;
	}
	mement* xMemEntry2 = reinterpret_cast<mement *>(PTR_PADD(xMemEntry,iMementSize));	// bp08
	TEST_MEMENT(xMemEntry2);
	i32 iMemEnt2Size = xMemEntry2->dw0();	// bp10
	if (iMemEnt2Size > 0) {
		MEMENT_3e74_0c8c(xMemEntry2);
		iMementSize += iMemEnt2Size;
	}
	xMemEntry->dw0(iMementSize);
//	printf("WRITE_I32 %p\n", xMemEntry);
	WRITE_I32(xMemEntry, iMementSize - 4, iMementSize);	// -4, size of dw0/size
//	printf("AFTER WRITE_I32 %p\n", xMemEntry);
	MEMENT_3e74_0d32(xMemEntry);
//	printf("AFTER _3e74_0d32\n");
	return;
}

//^3E74:4471
// _3e74_4471 renamed _3e74_4471_CACHE
U16 SkWinCore::_3e74_4471_CACHE()	// find a temp cache index ?
{
	i16 iPreviousCacheIndex = glbCacheRecyclerCurrentIndex;	// si
	if (iPreviousCacheIndex >= 0) {
		cd.mc._4976_5c92_cache++;
		if (cd.mc._4976_5c92_cache >= glbCacheRecyclerMax) {
			glbCacheRecyclerCurrentIndex = -1;
		}
		else {
			do {
				glbCacheRecyclerCurrentIndex++;
				ATLASSERT(glbCacheRecyclerCurrentIndex < glbCacheRecyclerMax);
			} while (tblCacheToMement[glbCacheRecyclerCurrentIndex] != 0xFFFF);
		}
	}
	//printf("_3e74_4471_CACHE: previous = %d / nextmement = %d / current = %d / memcache = %d / max = %d\n", 
	//	iPreviousCacheIndex, glbNextFreeMementIndex, glbCacheRecyclerCurrentIndex, cd.mc._4976_5c92_cache, glbCacheRecyclerMax);
	if (SkCodeParam::bUsePowerDebug && iPreviousCacheIndex < 0)	// like 0xFFFF
		return 0xFFFF;	// bypass ASSERT for now ...
	ATLASSERT(0 <= iPreviousCacheIndex && iPreviousCacheIndex < glbCacheRecyclerMax);
	return iPreviousCacheIndex;
}

//^3E74:54A1
U16 SkWinCore::INSERT_CACHE_HASH_AT(Bit32u cacheHash, U16 ici)
{
	// cqOk @ 21:03 2007/02/21

	//^3E74:54A1
	//^3E74:54A6
	if (cd.mc._4976_5c92_cache == glbCacheRecyclerMax) {
		//^3E74:54AF
		mement *bp04 = _4976_5d5e;
		//^3E74:54B6
		for (; (bp04->w10() & 0x8000) == 0; ) {
			//^3E74:54B8
			bp04 = tblMementsPointers[bp04->w8()];
			TEST_MEMENT(bp04);
		}
		//{
		//	mement *p = _4976_5d5e;
		//	while (p != NULL) {
		//		SkD((DLV_CPX, "ME: %p I %04X  Mi %04X \n", p, p->w10(), p->w8()));
		//		p = tblMementsPointers[p->w8()];
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
	if (SkCodeParam::bUsePowerDebug && si == 0xFFFF) // 0xFFFF
		return 0xFFFF;
	//^3E74:5501
	MOVE_MEMORY(
		&_4976_5c7e_cache_ici[ici],
		&_4976_5c7e_cache_ici[ici +1],
		(cd.mc._4976_5c92_cache - ici -1) << 1
		);
	//^3E74:5536
	_4976_5c7e_cache_ici[ici] = si;
	//^3E74:5544
	_4976_5c86_cache_hash[si] = cacheHash;
	//^3E74:555C
	return si;
}



//^3E74:44AD
// SPX: _3e74_44ad renamed MEMENT_3e74_44ad
void SkWinCore::MEMENT_3e74_44ad()
{
	// cqOk @ 21:03 2007/02/21

	_4976_5d2a = glbGameTick;
	glbMement2 = NULL;
	mement *bp04 = _4976_5c8c;
	if (bp04 == NULL) {
		bp04 = _4976_5d70;
		if (bp04 == NULL)
			return;
	}
	while (bp04->w4() != 0) {
		if (bp04->w4() <= 0xfffd) {
			bp04->w4(0);
		}
		U16 si = bp04->w6();
		if (si == 0xffff)
			return;
		bp04 = tblMementsPointers[si];
		TEST_MEMENT(bp04);
	}
}

//^3E74:0C62
U16 SkWinCore::QUERY_MEMENTI_FROM(U16 iEntryIndex)
{
	// xx&0x8000==0x8000: cacheindex-to-mementi
	// xx&0x8000==0x0000: raw data index to mementi

	// returns 0xffff if no match found.

	U16 iLocalEntryIndex = iEntryIndex;	// si
	if ((iLocalEntryIndex & 0x8000) != 0) {
		if (SkCodeParam::bUseFixedMode && (iLocalEntryIndex & 0x7FFF) >= MEM_CACHE_TABLE_MAX)	// SPX: protection to avoid further invalid pointer
			return 0xFFFF;
		return tblCacheToMement[iLocalEntryIndex & 0x7FFF];
	}
	else {
		if (SkCodeParam::bUseFixedMode && (iLocalEntryIndex & 0x7FFF) >= glbGDatNumberOfData) // SPX: protection to avoid further invalid pointer
			return 0xFFFF;
		return tblRawDataToMement[iLocalEntryIndex];
	}
}

//^3E74:55F9
U16 SkWinCore::ADD_CACHE_HASH(Bit32u cacheHash, U16 *piYaCacheIndex)
{
	// returns 0 if inserted new one, 1 if already exists.
	if (glbGameTick != _4976_5d2a) {
		MEMENT_3e74_44ad();
	}
	U16 bp02 = 0;
	if (FIND_ICI_FROM_CACHE_HASH(cacheHash, &bp02) != 0) {
		*piYaCacheIndex = _4976_5c7e_cache_ici[bp02];
		U16 si = QUERY_MEMENTI_FROM(*piYaCacheIndex | 0x8000);
		_3e74_48c9_MEMENT(si);
		return 1;
	}
	*piYaCacheIndex = INSERT_CACHE_HASH_AT(cacheHash, bp02);
	return 0;
}

//^3E74:5817
U8* SkWinCore::QUERY_MEMENT_BUFF_FROM_CACHE_INDEX(U16 cacheIndex)
{
	// SPX: break down, there was only 1 line in the "original" code (return reinterpret ...)
	U8* pMementP1 = NULL;
	mement* xMement = NULL;
	U16 iMement = QUERY_MEMENTI_FROM(cacheIndex | 0x8000);
	xMement = _3e74_48c9_MEMENT(iMement);
	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(xMement))
		return NULL;
	pMementP1 = reinterpret_cast<U8*> (&xMement[1]);
    //return reinterpret_cast<U8 *>(&_3e74_48c9_MEMENT(QUERY_MEMENTI_FROM(cacheIndex | 0x8000))[1]); // +18 bytes (sizeof(mement))
	return pMementP1;
}

//^3E74:583A
void SkWinCore::_3e74_583a_MEMENTI(U16 xx)
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
//	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(_4976_5d5e))
//		return -1;
	U16 iNextFreeMementi = glbNextFreeMementIndex;	// si
	if (iNextFreeMementi == 0xFFFF) {
		FREE_INDEXED_MEMENT(_4976_5d5e->w10());
		iNextFreeMementi = glbNextFreeMementIndex;
	}
	glbMementIndex_4976_5c9e++;
	if (glbMementIndex_4976_5c9e >= glbNumberOfMements) {
		glbNextFreeMementIndex = 0xFFFF;
	}
	else {
		do {
			glbNextFreeMementIndex++;
			ATLASSERT(glbNextFreeMementIndex < glbNumberOfMements);
		} while (tblMementsPointers[glbNextFreeMementIndex] != NULL);
	}
	return iNextFreeMementi;
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
	if (glbMement1 != NULL) {	// mement
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
					tblMementsPointers[si] = bp08;
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
					if (glbMement2 == bp04) {
						//^3E74:2C44
						glbMement2 = bp08;
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
		glbMement1 = NULL;
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
	else if (glbMement1->dw0() < buffSize) {
		//^3E74:2D32
		_3e74_2b30();
		//^3E74:2D36
		goto _2cfb;
	}
	else {
		//^3E74:2D38
		bp04 = glbMement1;
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
			bp04 = glbMement1;
			//^3E74:2DA3
			goto _2d5b;
			//^3E74:2DA5
_2da5:
			bp04 = bp08;
			//^3E74:2DB1
		} while (si == 0);

		//^3E74:2DB5
		MEMENT_3e74_0c8c(bp04);
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
			MEMENT_3e74_0d32(bp08);
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
	if (SkCodeParam::bUsePowerDebug && index == 0xFFFF)
		return NULL;

	buffSize += (sizeof(mement) + 4);
	if ((buffSize & 1) != 0) {
		buffSize++;
	}

	mement* xMement = ALLOC_LOWER_CPXHEAP(buffSize);	// bp04
	U16 si = FIND_FREE_MEMENTI();
	if (SkCodeParam::bUsePowerDebug && si == 0xFFFF)
		return NULL;

	ATLASSERT(tblCacheToMement[index] == U16(-1));
	tblCacheToMement[index] = si;

	ATLASSERT(tblMementsPointers[si] == 0);
	tblMementsPointers[si] = xMement;
	xMement->w10(index | 0x8000);

	SkD((DLV_CPX,"DBG: CPX Alloc Ci#%02d Mi#%02d (%5u) = %p\n", index, si, 0U + buffSize, xMement));

	return reinterpret_cast<U8 *>(&xMement[1]); // +18 bytes (sizeof(mement))
}

//^3E74:585A
// _3e74_585a renamed _3e74_585a_CACHE
void SkWinCore::_3e74_585a_CACHE(U16 iCacheIndex, U16 yy)
{
	U16 iMementIndex = QUERY_MEMENTI_FROM(iCacheIndex | 0x8000);	// si
	if (iMementIndex == 0xFFFF) {
		FREE_CACHE_INDEX(iCacheIndex);
	}
	else {
		RECYCLE_MEMENTI(iMementIndex, yy);
	}
}


//^3E74:53EA
Bit32u SkWinCore::GET_TEMP_CACHE_HASH()
{
	//^3E74:53EA
	Bit32u bp04 = 0;
	U16 bp06 = 0;
	do {
		//^3E74:53EE
		bp04 = 0xffff0000 | (_4976_484b_cache++);
		//^3E74:5401
	} while (FIND_ICI_FROM_CACHE_HASH(bp04, &bp06) != 0);
	//^3E74:5418
	return bp04;
}

//^3E74:5888
U16 SkWinCore::ALLOC_TEMP_CACHE_INDEX()
{
	U16 iCacheIndex = 0; // pb02
	ADD_CACHE_HASH(GET_TEMP_CACHE_HASH(), &iCacheIndex);
	return iCacheIndex;
}



//^3E74:58A3
void SkWinCore::FREE_TEMP_CACHE_INDEX(U16 iCacheIndex)
{
	ENTER(0);
	_3e74_585a_CACHE(iCacheIndex, 0);
	FREE_INDEXED_MEMENT(iCacheIndex | 0x8000);
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
			ATLASSERT(si < glbCacheRecyclerMax);
		Bit32u bp04 = _4976_5c86_cache_hash[_4976_5c7e_cache_ici[si]];
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
	mement *bp04 = tblMementsPointers[di];
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
		bp0c = glbMement2;
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
	if (glbMement2 == NULL) {
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
		mement *bp08 = tblMementsPointers[si];
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
		glbMement2 = bp04;
	}
	//^3E74:48C5
	return;
}

// SPX: changed "void" to "int" in order to allow more code on return
int SkWinCore::TEST_MEMENT(mement* xMemEntry)
{
	i32	iDW0ValLen = 0;
	i32 iMemEntSize = 0;	// len
	//ATLASSERT(bp04 != NULL);
	// SPX: use CheckSafePointer to trap more unexpected mem values so that it fails at ASSERT instead of crashing after.
	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(xMemEntry))
		return 0;
	ATLASSERT(CheckSafePointer(xMemEntry));
	iDW0ValLen = (abs(-xMemEntry->dw0())) -4;
	if (iDW0ValLen >= 65536)
		return 0;
	iMemEntSize = READ_I32(xMemEntry,iDW0ValLen);
	//printf("TEST_MEMENT P:%p => DW0: %08X  DWA: %08X DWA-4(L): %08X P-L: %08X \n", xMemEntry, xMemEntry->dw0(), (abs(-xMemEntry->dw0())), iDW0ValLen, iMemEntSize);
	//for (int i = 0; i < 16; i++)
	//	printf("%02X.", READ_UI8(xMemEntry,-8+i));
	//printf("\n");
	if (SkCodeParam::bUseFixedMode)	{
		if (xMemEntry->dw0() != iMemEntSize) {
			iMemEntSize = xMemEntry->dw0();
		}
	}
	ATLASSERT(xMemEntry->dw0() == iMemEntSize);
	return 1; // test passed
}

//^3E74:5673
// _3e74_5673 renamed _3e74_5673_CACHE
X32 SkWinCore::_3e74_5673_CACHE(X32 cacheHash, U16 *piYaCacheIndex, X16 ifTryInsert)
{
	ENTER(2);
	if (glbGameTick != _4976_5d2a) {
		MEMENT_3e74_44ad();
	}
	U16 bp02;
	if (FIND_ICI_FROM_CACHE_HASH(cacheHash, &bp02) == 0) {
		*piYaCacheIndex = (ifTryInsert != 0) ? INSERT_CACHE_HASH_AT(cacheHash, bp02) : 0xFFFF;
		return 0;
	}
	*piYaCacheIndex = _4976_5c7e_cache_ici[bp02];
// SPX: Break down to catch and prevent NULL / crash
	mement* xm = _3e74_48c9_MEMENT(QUERY_MEMENTI_FROM(*piYaCacheIndex | 0x8000));
	if (CheckSafePointer(xm))
		return *reinterpret_cast<i32 *>(xm) - (sizeof(mement)) - 4;	// -22 = -sizeof(mement) - 4
//	return *reinterpret_cast<i32 *>(_3e74_48c9_MEMENT(QUERY_MEMENTI_FROM(*piYaCacheIndex | 0x8000))) -22;	// SPX: -22 ?? -sizeof(mement) - 4 ?
	return 0;
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

