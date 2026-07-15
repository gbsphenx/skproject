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
// _3e74_48c9 renamed GET_MEMENT_FROM_MEMENTINDEX
mement* SkWinCore::GET_MEMENT_FROM_MEMENTINDEX(U16 mementi)
{
	// cqOk @ 21:03 2007/02/21

	mement* xMement = NULL;	// (bp04) can't hold more than glbNumberOfMements
	if (SkCodeParam::bUsePowerDebug && mementi >= glbNumberOfMements)
		return NULL;
	xMement = tblMementsPointers[mementi];	// (bp04) can't hold more than glbNumberOfMements
	TEST_MEMENT(xMement);
	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(xMement))
		return NULL;
	U16 si = xMement->getMark4();	// si
	if (si == MEMENT_MARK_M1_FFFF || si == MEMENT_MARK_M2_FFFE) {
		return xMement;
	}
	mement* xMement2 = xMement;	// bp0c
	if (si == 0) {
		U16 di = xMement->getMark8();
		if (di == MEMENT_MARK_M1_FFFF) {
			glbMement2 = (xMement != glbMement3) ? xMement : NULL;
			xMement->setMark4(MEMENT_MARK_P1_0001);
			return xMement;
		}
		mement* xMement3 = tblMementsPointers[di];	// bp08
		TEST_MEMENT(xMement3);
		U16 bp0e = xMement->getMark6();
		if (bp0e == MEMENT_MARK_M1_FFFF) {
			glbMement5 = xMement3;
		}
		else {
			xMement = tblMementsPointers[bp0e];
			TEST_MEMENT(xMement);
			xMement->setMark8(di);
		}
		xMement3->setMark6(bp0e);
		xMement2->setMark4(MEMENT_MARK_M1_FFFF);
		RECYCLE_MEMENTI(mementi, 0);
	}
	else {
		if (si < MEMENT_MARK_M3_FFFD) {
			si++;
		}
		xMement->setMark4(si);
		if (xMement == glbMement4) {
			return xMement;
		}
		U16 di = xMement->getMark8();
		mement* xMement3 = tblMementsPointers[di];	// bp08
		TEST_MEMENT(xMement3);
		if (xMement3->getMark4() >= si) {
			return xMement;
		}
		U16 bp0e = xMement->getMark6();
		if (bp0e == MEMENT_MARK_M1_FFFF) {
			glbMement2 = xMement3;
			glbMement5 = xMement3;
			xMement3->setMark6(MEMENT_MARK_M1_FFFF);
		}
		else {
			if (xMement == glbMement2) {
				glbMement2 = xMement3;
			}
			xMement = tblMementsPointers[bp0e];
			TEST_MEMENT(xMement);
			xMement->setMark8(di);
			xMement3->setMark6(bp0e);
		}
		if (glbMement2 == glbMement3) {
			glbMement2 = NULL;
		}

		while (true) {
			xMement = xMement3;
			bp0e = di;
			di = xMement->getMark8();
			if (di == MEMENT_MARK_M1_FFFF) {
				xMement->setMark8(mementi);
				xMement2->setMark6(bp0e);
				xMement2->setMark8(MEMENT_MARK_M1_FFFF);
				glbMement4 = xMement2;
			}
			else {
				xMement3 = tblMementsPointers[di];
				TEST_MEMENT(xMement3);
				if (xMement3->getMark4() < si)
					continue;
				xMement->setMark8(mementi);
				xMement2->setMark6(bp0e);
				xMement2->setMark8(di);
				xMement3->setMark6(mementi);
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

    i16 si = cacheIndex;
	if (glbCacheRecyclerCurrentIndex == -1 || glbCacheRecyclerCurrentIndex > si) {
		glbCacheRecyclerCurrentIndex = si;
	}
	ATLASSERT(si < glbCacheRecyclerMax);
	Bit32u bp04 = _4976_5c86_cache_hash[si];
	U16 bp06;
	if (FIND_ICI_FROM_CACHE_HASH(bp04, &bp06) != 0) {
		_4976_5c86_cache_hash[si] = 0;
		cd.mc._4976_5c92_cache--;
		MOVE_MEMORY(
			&_4976_5c7e_cache_ici[bp06 +1],
			&_4976_5c7e_cache_ici[bp06],
			(cd.mc._4976_5c92_cache - bp06) << 1
			);
	}
	return;
}



//^3E74:4549
// SPX: 3e74_4549 renamed MEMENT_3e74_4549
void SkWinCore::MEMENT_3e74_4549(U16 iMemEntIdx)
{
	// cqOk @ 21:03 2007/02/21

	mement* xMemEnt = tblMementsPointers[iMemEntIdx];	// bp04
	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(xMemEnt))
		return;
	TEST_MEMENT(xMemEnt);
	if (xMemEnt != NULL) {
		if (xMemEnt->getMark4() != MEMENT_MARK_M1_FFFF) {
			U16 di = xMemEnt->getMark6();
			U16 si = xMemEnt->getMark8();
			mement* xMemEnt2;	// pb08
			if (di == MEMENT_MARK_M1_FFFF) {
				if (si == MEMENT_MARK_M1_FFFF) {
					glbMement2 = NULL;
					glbMement3 = NULL;
					glbMement4 = NULL;
					glbMement5 = NULL;
				}
				else {
					xMemEnt2 = tblMementsPointers[si];
					TEST_MEMENT(xMemEnt2);
					glbMement5 = xMemEnt2;
					xMemEnt2->setMark6(MEMENT_MARK_M1_FFFF);
					goto _4682;
				}
			}
			else {
				xMemEnt2 = tblMementsPointers[di];
				TEST_MEMENT(xMemEnt2);
				xMemEnt2->setMark8(si);
				if (si == MEMENT_MARK_M1_FFFF) {
					if (xMemEnt == glbMement3) {
						glbMement3 = NULL;
					}
					else {
						if (xMemEnt == glbMement2) {
							glbMement2 = NULL;
						}
					}
					glbMement4 = xMemEnt2;
				}
				else {
					xMemEnt2 = tblMementsPointers[si];
					TEST_MEMENT(xMemEnt2);
					xMemEnt2->setMark6(di);
				}
_4682:
				if (xMemEnt == glbMement2) {
					if (xMemEnt2 == glbMement3) {
						glbMement2 = NULL;
					}
					else {
						glbMement2 = xMemEnt2;
					}
				}
				else {
					if (glbMement3 == xMemEnt) {
						glbMement3 = xMemEnt2;
					}
				}
			}
		}
	}
	xMemEnt->setMark8(MEMENT_MARK_M1_FFFF);
	xMemEnt->setMark6(MEMENT_MARK_M1_FFFF);
	xMemEnt->setMark4(MEMENT_MARK_M1_FFFF);
	return;
}

//^3E74:0C8C
// SPX: _3e74_0c8c renamed MEMENT_3e74_0c8c
void SkWinCore::MEMENT_3e74_0c8c(mement* xMemEnt)
{
	// cqOk @ 21:03 2007/02/21

	mement* xMement1 = xMemEnt->getMement1();	// bp08
	mement* xMement2 = xMemEnt->getMement2();	// bp04
	if (xMement1 == NULL) {
		if (xMement2 == NULL) {
			glbMement1 = NULL;
		}
		else {
			glbMement1 = xMement2;
			xMement2->setMement1(NULL);
		}
	}
	else {
		xMement1->setMement2(xMement2);
		if (xMement2 != NULL) {
			xMement2->setMement1(xMement1);
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
		xMement1->setMement2(NULL);
_0d71:
		xMement1->setMement1(NULL);	// glbMement1->pv4(NULL);
		return;
	}
	mement* xMemEntry2 = glbMement1;	// bp08
	i32 bp0c = xMement1->dw0();
	if (xMemEntry2->dw0() <= bp0c) {
		glbMement1 = xMement1;
		xMement1->setMement1(NULL);
		xMement1->setMement2(xMemEntry2);
		xMemEntry2->setMement1(xMement1);
		goto _0d71;
	}

	U16 si = 0;
	while ((xMement1 = xMemEntry2->getMement2()) != NULL) {
		if (xMement1->dw0() <= bp0c) {
			xMement1->setMement1(xMemEntRef);
			xMemEntry2->setMement2(xMemEntRef);
			xMemEntRef->setMement1(xMemEntry2);
			xMemEntRef->setMement2(xMement1);
			si = 1;
			break;
		}
		xMemEntry2 = xMement1;
	}
	if (si == 0) {
		xMemEntry2->setMement2(xMemEntRef);
		xMemEntRef->setMement1(xMemEntry2);
		xMemEntRef->setMement2(NULL);
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
	MEMENT_3e74_4549(iMemEntryIndex);
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
	SkD((DLV_CPX||SkCodeParam::bDebugPrint,"CPX: CPX Free  Ci#%02d Mi#%02d (%5u) = %p\n"
		, (index & 0x8000) ? (index & 0x7FFF) : -1
		, iMemEntryIndex
		, iMementSize
		, xMemEntry));
	if (xMemEntry != _4976_5ce6) {
		//printf("READ_I32 %p\n", xMemEntry);
		i32 iMemEntReadSize = READ_I32(xMemEntry,-sizeof(i32));	// bp10 READ_I32(xMemEntry,-4);
//		printf("WHAT IS bp10 = %08x\n", iMemEntReadSize);
//		printf("AFTER READ_I32 %p\n", xMemEntry);
		LOGX(("FREE INDEXED MEMENT %04X => pX]%p S=%06X RS=%08X (NEG:%08X) [(-4)%08X (0)%08X]", index, xMemEntry, iMementSize, iMemEntReadSize, -iMemEntReadSize, *(((U32*)xMemEntry)-1), *((U32*)xMemEntry) ));
		SkD((SkCodeParam::bDebugPrint, "Mement Readsize = %X (%d) (bounds %X to %X)\n", iMemEntReadSize, iMemEntReadSize, -BUFF_SIZE_MAX, BUFF_SIZE_MAX));
		ATLASSERT(-BUFF_SIZE_MAX < iMemEntReadSize && iMemEntReadSize < BUFF_SIZE_MAX);	//ATLASSERT(-60000 < bp10 && bp10 < 60000);

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
	WRITE_I32(xMemEntry, iMementSize - sizeof(i32), iMementSize);	// -4, size of dw0/size
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

	if (cd.mc._4976_5c92_cache == glbCacheRecyclerMax) {
		mement* xMemEnt = glbMement5;	// bp04
		for (; (xMemEnt->getRawDataIndex() & 0x8000) == 0; ) {
			xMemEnt = tblMementsPointers[xMemEnt->getMark8()];
			TEST_MEMENT(xMemEnt);
		}
		//{
		//	mement *p = _4976_5d5e; (glbMement5)
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

		FREE_INDEXED_MEMENT(xMemEnt->getRawDataIndex());
		FIND_ICI_FROM_CACHE_HASH(cacheHash, &ici);
	}
	U16 si = _3e74_4471_CACHE();
	if (SkCodeParam::bUsePowerDebug && si == 0xFFFF) // 0xFFFF
		return 0xFFFF;
	MOVE_MEMORY(
		&_4976_5c7e_cache_ici[ici],
		&_4976_5c7e_cache_ici[ici +1],
		(cd.mc._4976_5c92_cache - ici -1) << 1
		);
	_4976_5c7e_cache_ici[ici] = si;
	_4976_5c86_cache_hash[si] = cacheHash;
	return si;
}



//^3E74:44AD
// SPX: _3e74_44ad renamed MEMENT_3e74_44ad
void SkWinCore::MEMENT_3e74_44ad()
{
	// cqOk @ 21:03 2007/02/21

	glbGameTickHoldFromMement = glbGameTick;
	glbMement2 = NULL;
	mement* xMemEnt = glbMement3;	// bp04
	if (xMemEnt == NULL) {
		xMemEnt = glbMement4;
		if (xMemEnt == NULL)
			return;
	}
	while (xMemEnt->getMark4() != 0) {
		if (xMemEnt->getMark4() <= MEMENT_MARK_M3_FFFD) {
			xMemEnt->setMark4(MEMENT_MARK_P0_0000);
		}
		U16 si = xMemEnt->getMark6();
		if (si == MEMENT_MARK_M1_FFFF)
			return;
		xMemEnt = tblMementsPointers[si];
		TEST_MEMENT(xMemEnt);
	}
}

//^3E74:0C62
U16 SkWinCore::QUERY_MEMENTI_FROM(U16 iRawDataIdx)
{
	// xx&0x8000==0x8000: cacheindex-to-mementi
	// xx&0x8000==0x0000: raw data index to mementi

	// returns 0xffff if no match found.

	U16 iLocalEntryIndex = iRawDataIdx;	// si
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
	if (glbGameTick != glbGameTickHoldFromMement) {
		MEMENT_3e74_44ad();
	}
	U16 bp02 = 0;
	if (FIND_ICI_FROM_CACHE_HASH(cacheHash, &bp02) != 0) {
		*piYaCacheIndex = _4976_5c7e_cache_ici[bp02];
		U16 si = QUERY_MEMENTI_FROM(*piYaCacheIndex | 0x8000);
		GET_MEMENT_FROM_MEMENTINDEX(si);
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
	xMement = GET_MEMENT_FROM_MEMENTINDEX(iMement);
	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(xMement))
		return NULL;
	pMementP1 = reinterpret_cast<U8*> (&xMement[1]);
    //return reinterpret_cast<U8 *>(&_3e74_48c9_MEMENT(QUERY_MEMENTI_FROM(cacheIndex | 0x8000))[1]); // +18 bytes (sizeof(mement))
	return pMementP1;
}

//^3E74:583A
void SkWinCore::_3e74_583a_MEMENTI(U16 iMemEntryIndex)
{
	U16 iMemEntryCacheIndex = QUERY_MEMENTI_FROM(iMemEntryIndex | 0x8000);	// si
	if (iMemEntryCacheIndex != 0xFFFF) {
		MEMENT_3e74_4549(iMemEntryCacheIndex);
	}
	return;
}

//^3E74:4416
U16 SkWinCore::FIND_FREE_MEMENTI()
{
//	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(glbMement5))
//		return -1;
	U16 iNextFreeMementi = glbNextFreeMementIndex;	// si
	if (iNextFreeMementi == 0xFFFF) {
		FREE_INDEXED_MEMENT(glbMement5->getRawDataIndex());
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
void SkWinCore::GUARANTEE_FREE_CPXHEAP_SIZE(i32 iBufferSize)
{
	ATLASSERT(iBufferSize > 0 && iBufferSize < BUFF_SIZE_MAX);

	for (; _4976_5cf8 < iBufferSize; ) {
		if (glbMement5 == NULL) {
			RAISE_SYSERR(SYSTEM_ERROR__FREE_CPXHEAP_SIZE);
		}
		if (_4976_4809 != 0xFFFF && _4976_4809 != glbRawDataNoAllocated) {
			FREE_INDEXED_MEMENT(_4976_4809);
		}
		else {
			FREE_INDEXED_MEMENT(glbMement5->getRawDataIndex());
		}
	}
	return;
}



//^3E74:2B30
// SPX: 3e74_2b30 renamed MEMENT_3e74_2b30 // MEMENT_INIT_GFX_TABLE
void SkWinCore::MEMENT_INIT_GFX_TABLE()
{
	int iDebugLoopCount = 0;
	// cqOk @ 21:03 2007/02/21

	if (glbMement1 != NULL) {	// mement
		mement* xMemEnt01 = _4976_5ce6;	// bp04
		mement* xMemEnt02 = _4976_5ce6;	// bp08

		do {
			i32 iLength01 = xMemEnt01->dw0();	// (bp0c) length
			//printf("LOAD_DYN4>_3e74_2b30[MEMENT] (#%05d) bp0c (length) = %d\n", iDebugLoopCount, bp0c);
			// SPX: we can get stuck in the loop, let's assume that after 1000 loop we try to get out, or if the length is ridiculously too much
			if (SkCodeParam::bUseFixedMode && (iDebugLoopCount > 1000 || iLength01 > 1024000))
				break;
			if (iLength01 > 0) {
				PTR_PADA(xMemEnt01, iLength01);	// move +iLength01
			}
			else {
				iLength01 = -iLength01;
				if (xMemEnt02 != xMemEnt01) {
					U16 iRawDataIdx = xMemEnt01->getRawDataIndex();	// di
					U16 si = QUERY_MEMENTI_FROM(iRawDataIdx);
					tblMementsPointers[si] = xMemEnt02;
					COPY_MEMORY(xMemEnt01, xMemEnt02, iLength01);
					TEST_MEMENT(xMemEnt02);	// SPX TODO this part is prone to fail while changing level
					if (glbMement5 == xMemEnt01) {
						glbMement5 = xMemEnt02;
					}
					if (glbMement4 == xMemEnt01) {
						glbMement4 = xMemEnt02;
					}
					if (glbMement2 == xMemEnt01) {
						glbMement2 = xMemEnt02;
					}
					else if (glbMement3 == xMemEnt01) {
						glbMement3 = xMemEnt02;
					}
				}
				PTR_PADA(xMemEnt01, iLength01);	// move +iLength01
				PTR_PADA(xMemEnt02, iLength01);	// move +iLength01
			}
			iDebugLoopCount++; // debug
		} while (xMemEnt01 != _4976_5cb2);	// at some point, this becomes impossible when bp04 never changes and we get stuck in the loop

		_4976_5cb2 = xMemEnt02;
		glbMement1 = NULL;
	}
	return;
}

//^3E74:2CC9
mement *SkWinCore::ALLOC_LOWER_CPXHEAP(i32 buffSize)
{
	// allocate memory from lower part of complex memory pool.
	// buffSize must include size of header&footer: sizeof(mement)+4 = 22
	ATLASSERT(buffSize > 0 && buffSize < BUFF_SIZE_MAX);

	GUARANTEE_FREE_CPXHEAP_SIZE(buffSize);
	mement* xMemEnt;	// bp04
	if (PTR_PSBP(_4976_5ce2,_4976_5cb2) >= buffSize) { // allocate on main memory
_2cfb:
		xMemEnt = _4976_5cb2;
		PTR_PADA(_4976_5cb2,buffSize);
	}
	else if (glbMement1->dw0() < buffSize) {
		MEMENT_INIT_GFX_TABLE();
		goto _2cfb;
	}
	else {
		xMemEnt = glbMement1;
		U16 si = 0;
		mement* xMemEnt2;	// bp08

		do {
			if (xMemEnt->dw0() == buffSize) {
_2d5b:
				si = 1;
				continue;
			}
			if (xMemEnt->dw0() >= buffSize) {
				xMemEnt2 = xMemEnt->getMement2();
				if (xMemEnt2 != NULL)
					goto _2da5;
			}
			xMemEnt = glbMement1;
			goto _2d5b;
_2da5:
			xMemEnt = xMemEnt2;
		} while (si == 0);

		MEMENT_3e74_0c8c(xMemEnt);
		i32 bp0c = xMemEnt->dw0() - buffSize;
		if (bp0c >= 30) {
			xMemEnt2 = reinterpret_cast<mement *>(PTR_PADD(xMemEnt,buffSize));
			WRITE_I32(xMemEnt2,bp0c - sizeof(i32),bp0c);	// WRITE_I32(xMemEnt2,bp0c -4,bp0c);
			LOGX(("WRITE MEMENT(2) @pX]%p - %08X | size = %08X abs(%8X/%d) (written @pX]%p", xMemEnt2, bp0c - sizeof(i32), bp0c, abs(bp0c), abs(bp0c), ((X8*)(xMemEnt2)+(bp0c - sizeof(i32))) ));
			xMemEnt2->dw0(bp0c);
			MEMENT_3e74_0d32(xMemEnt2);
			LOGX(("pX]%p S=%06X RS=%08X [(-4)%08X (0)%08X]", xMemEnt2, bp0c - sizeof(i32), READ_I32(xMemEnt2,-sizeof(i32)), *(((U32*)xMemEnt2)-1), *((U32*)xMemEnt2) ));
		}
		else {
			buffSize = xMemEnt->dw0();
		}
	}
	_4976_5cf8 -= buffSize;
	xMemEnt->dw0(-buffSize);
	WRITE_I32(xMemEnt,+buffSize -sizeof(i32),-buffSize);
	LOGX(("WRITE MEMENT(1) @pX]%p - %08X | size = %08X abs(%8X/%d) (written @pX]%p", xMemEnt, buffSize -sizeof(i32), -buffSize, buffSize, buffSize, ((X8*)(xMemEnt)+(buffSize -sizeof(i32))) ));
	LOGX(("pX]%p S=%06X RS=%08X [(-4)%08X (0)%08X]", xMemEnt, buffSize -sizeof(i32), READ_I32(xMemEnt,-sizeof(i32)), *(((U32*)xMemEnt)-1), *((U32*)xMemEnt) ));
	reinterpret_cast<mement *>(xMemEnt)->setMark8(MEMENT_MARK_M1_FFFF);
	reinterpret_cast<mement *>(xMemEnt)->setMark6(MEMENT_MARK_M1_FFFF);
	reinterpret_cast<mement *>(xMemEnt)->setMark4(MEMENT_MARK_M1_FFFF);
#if UseAltic
	{
		i32 size = -READ_I32(xMemEnt,-xMemEnt->dw0() -sizeof(i32));	// -4
		ATLASSERT(buffSize == size);

		// SkD((DLV_CPX, "CPX: ALLOC_LOWER_CPXHEAP(%6u) = %p\n", (Bitu)buffSize, bp04));
		memset(&xMemEnt[1], 0xCC, buffSize -sizeof(mement) -sizeof(i32));
		// ATLASSERT((U32)bp04 != 0x00758132);
	}
#endif
	return reinterpret_cast<mement *>(xMemEnt);
}

//^3E74:5708
// returns a *mement ?
U8* SkWinCore::ALLOC_CPXHEAP_MEM(U16 iCacheIndex, U32 iBufferSize)
{
	if (SkCodeParam::bUsePowerDebug && iCacheIndex == 0xFFFF)
		return NULL;
	ATLASSERT(iCacheIndex < glbCacheRecyclerMax);

	iBufferSize += (sizeof(mement) + sizeof(i32));	// +4
	if ((iBufferSize & 1) != 0) {
		iBufferSize++;
	}

	mement* xMement = ALLOC_LOWER_CPXHEAP(iBufferSize);	// bp04
	U16 iMemEntryIndex = FIND_FREE_MEMENTI();	// si
	if (SkCodeParam::bUsePowerDebug && iMemEntryIndex == 0xFFFF) {
		LOGX(("WARNING! Can't find free Mementi\n"));
		return NULL;
	}

	ATLASSERT(tblCacheToMement[iCacheIndex] == U16(-1));	// that cache slot must be empty
	tblCacheToMement[iCacheIndex] = iMemEntryIndex;

	ATLASSERT(tblMementsPointers[iMemEntryIndex] == NULL);	// that mem entry table must be empty == 0
	tblMementsPointers[iMemEntryIndex] = xMement;
	xMement->setCacheIndex(iCacheIndex | MEM_ENT_x8000_FLAG_CACHE);

	SkD((DLV_CPX,"DBG: CPX Alloc Ci#%02d Mi#%02d (%5u) = %p\n", iCacheIndex, iMemEntryIndex, 0U + iBufferSize, xMement));

	return reinterpret_cast<U8 *>(&xMement[1]); // +18 bytes (sizeof(mement))
}

//^3E74:585A
// _3e74_585a renamed _3e74_585a_CACHE
void SkWinCore::_3e74_585a_CACHE(U16 iCacheIndex, U16 yy)
{
	U16 iMementIndex = QUERY_MEMENTI_FROM(iCacheIndex | MEM_ENT_x8000_FLAG_CACHE);	// si
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
	Bit32u bp04 = 0;
	U16 bp06 = 0;
	do {
		bp04 = 0xffff0000 | (_4976_484b_cache++);
	} while (FIND_ICI_FROM_CACHE_HASH(bp04, &bp06) != 0);
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
	FREE_INDEXED_MEMENT(iCacheIndex | MEM_ENT_x8000_FLAG_CACHE);
	return;
}


//^3E74:5420
U16 SkWinCore::FIND_ICI_FROM_CACHE_HASH(Bit32u cacheHash, U16 *ici)
{
	i16 di = -1;
	i16 cx = cd.mc._4976_5c92_cache;

	while (true) {
		i16 si = (di + cx) >> 1;
		if (si == di) {
			*ici = si +1;
			return 0;
		}
		if (!SkCodeParam::bUsePowerDebug)
			ATLASSERT(si < glbCacheRecyclerMax);
		Bit32u bp04 = _4976_5c86_cache_hash[_4976_5c7e_cache_ici[si]];
		if (cacheHash < bp04) {
			cx = si;
			continue;
		}
		if (cacheHash > bp04) {
			di = si;
			continue;
		}
		*ici = si;
		return 1;
	}
}

//^3E74:4701
void SkWinCore::RECYCLE_MEMENTI(U16 iMemEntIdx, U16 yy)
{
	// cqOk @ 21:03 2007/02/21

	U16 iLocalMemEntIdx = iMemEntIdx;	// di
	mement* xMemEntry = tblMementsPointers[iLocalMemEntIdx];	// bp04
	if (SkCodeParam::bUsePowerDebug && !CheckSafePointer(xMemEntry))
		return;
	TEST_MEMENT(xMemEntry);
	if (xMemEntry->getMark4() != MEMENT_MARK_M1_FFFF) {
		GET_MEMENT_FROM_MEMENTINDEX(iLocalMemEntIdx);
		return;
	}
	mement* xMemEntry02;	// bp0c
	mement* xMemEntry03;	// bp10
	if (yy != 0) {
		xMemEntry->setMark4(MEMENT_MARK_M1_FFFF);
		xMemEntry02 = glbMement3;
		xMemEntry03 = glbMement4;
	}
	else {
		xMemEntry->setMark4(MEMENT_MARK_P1_0001);
		xMemEntry02 = glbMement2;
		xMemEntry03 = glbMement3;
		if (xMemEntry03 == NULL) {
			xMemEntry03 = glbMement4;
		}
	}

	if (xMemEntry02 != NULL) {
		goto _483a;
	}
	U16 bp12;	// bp12
	U16 si;		// si
	if (glbMement4 == NULL) {
		xMemEntry->setMark6(MEMENT_MARK_M1_FFFF);
		glbMement5 = xMemEntry;
		goto _4822;
	}
	if (glbMement2 == NULL) {
		if (glbMement3 != NULL) {
			xMemEntry02 = xMemEntry03;
			goto _483a;
		}
	}
	xMemEntry03->setMark8(iLocalMemEntIdx);
	bp12 = xMemEntry03->getRawDataIndex();
	si = QUERY_MEMENTI_FROM(bp12);
	xMemEntry->setMark6(si);
_4822:
	xMemEntry->setMark8(MEMENT_MARK_M1_FFFF);
	glbMement4 = xMemEntry;
	goto _48a3;

_483a:
	si = xMemEntry02->getMark6();
	xMemEntry02->setMark6(iLocalMemEntIdx);
	xMemEntry->setMark6(si);
	U16 bp14;
	if (si != MEMENT_MARK_M1_FFFF) {
		mement* xMemEntry03 = tblMementsPointers[si]; // bp08
		TEST_MEMENT(xMemEntry03);
		xMemEntry->setMark8(xMemEntry03->getMark8());
		xMemEntry03->setMark8(iLocalMemEntIdx);
		goto _48a3;
	}
	bp14 = QUERY_MEMENTI_FROM(xMemEntry02->getRawDataIndex());
	xMemEntry->setMark8(bp14);
	glbMement5 = xMemEntry;

_48a3:
	if (yy != 0) {
		glbMement3 = xMemEntry;
	}
	else {
		glbMement2 = xMemEntry;
	}
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
	iDW0ValLen = (abs(-xMemEntry->dw0())) -sizeof(i32);	// -4
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
	if (glbGameTick != glbGameTickHoldFromMement) {
		MEMENT_3e74_44ad();
	}
	U16 ici;	// bp02
	if (FIND_ICI_FROM_CACHE_HASH(cacheHash, &ici) == 0) {
		*piYaCacheIndex = (ifTryInsert != 0) ? INSERT_CACHE_HASH_AT(cacheHash, ici) : 0xFFFF;
		return 0;
	}
	*piYaCacheIndex = _4976_5c7e_cache_ici[ici];
// SPX: Break down to catch and prevent NULL / crash
	mement* xMemEnt = GET_MEMENT_FROM_MEMENTINDEX(QUERY_MEMENTI_FROM(*piYaCacheIndex | MEM_ENT_x8000_FLAG_CACHE));	// xm
	if (CheckSafePointer(xMemEnt))
		return *reinterpret_cast<i32 *>(xMemEnt) - (sizeof(mement)) - (sizeof(i32));	// -22 = -sizeof(mement) - 4
		// SPX: what is this -4 ? some size/length is stored in these -4 before mement pointer
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
	FREE_INDEXED_MEMENT(xx|MEM_ENT_x8000_FLAG_CACHE);
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



//^4726:0162
void SkWinCore::MAP_EMS_TO_PTR(U32 xx, U32 yy, U8 **ss, U8 **tt)
{
//!	//^4726:0162
//!	ENTER(14);
//!	//^4726:0168
//!	if (xx > yy) {
//!		//^4726:017A
//!		X32 bp0e = xx;
//!		xx = yy;
//!		yy = bp0e;
//!		U8 **bp08 = ss;
//!		ss = tt;
//!		tt = bp08;
//!	}
//!	//^4726:01C2
//!	U16 bp04 = U16(xx >> 14);
//!	U16 di = U16(xx) & 0x3fff;
//!	U16 bp06 = U16(yy >> 14);
//!	U16 si = U16(yy) & 0x3fff;
//!	U8 bp01 = 0;
//!	U8 bp02 = U8(bp06 -bp04);
//!	__asm mov  ah,0x44
//!	__asm mov  al,[bp01]
//!	__asm mov  bx,[bp04]
//!	__asm mov  dx,[_4976_5ea4]
//!	__asm int  0x67
//!	bp01++;
//!	bp04++;
//!	__asm mov  ah,0x44
//!	__asm mov  al,[bp01]
//!	__asm mov  bx,[bp04]
//!	__asm mov  dx,[_4976_5ea4]
//!	__asm int  0x67
//!	*ss = &_4976_5ea0[di];
//!	if (bp02 >= 2) {
//!		//^4726:023A
//!		bp02 = 2;
//!		__asm mov  ah,0x44
//!		__asm mov  al,[bp02]
//!		__asm mov  bx,[bp06]
//!		__asm mov  dx,[_4976_5ea4]
//!		__asm int  0x67
//!		bp02++;
//!		bp06++;
//!		__asm mov  ah,0x44
//!		__asm mov  al,[bp02]
//!		__asm mov  bx,[bp06]
//!		__asm mov  dx,[_4976_5ea4]
//!		__asm int  0x67
//!		bp02 = 2;
//!	}
//!	//^4726:0266
//!	else if (bp02 == 1) {
//!		//^4726:026C
//!		bp02 = 2;
//!		bp06++;
//!		__asm mov  ah,0x44
//!		__asm mov  al,[bp02]
//!		__asm mov  bx,[bp06]
//!		__asm mov  dx,[_4976_5ea4]
//!		__asm int  0x67
//!		bp02 = 1;
//!	}
//!	else {
//!		//^4726:0287
//!		bp02 = 0;
//!	}
//!	//^4726:028B
//!	*tt = &_4976_5ea0[si + (bp02 << 14)];
}



//^4726:02AC
// SPX: _4726_02ac renamed MEM_PREPARE_VIEWPORT ??
U8 *SkWinCore::MEM_PREPARE_VIEWPORT()
{
	ENTER(4);
	if (_4976_4968 != 0) {
		U8* xImageBuffer = EMS_MAP_BANK_TO_MEMORY(_4976_5eac + (sizeof(ImageExtraHeader)), 1);	// bp04 ; size +6
		//WRITE_UI16(xImageBuffer,-6,8);
		//WRITE_UI16(xImageBuffer,-4,_4976_00f6);
		//WRITE_UI16(xImageBuffer,-2,_4976_00f8);
		WRITE_IMGBUFF_BPP(xImageBuffer,IMG_8_BPP);
		WRITE_IMGBUFF_WIDTH(xImageBuffer,_4976_00f6);
		WRITE_IMGBUFF_HEIGHT(xImageBuffer,_4976_00f8);
		return xImageBuffer;
	}
	return NULL;
}

//^3E74:2F41
void SkWinCore::_3e74_2f41(sk5d12 *ref)
{
	//^3E74:2F41
	ENTER(28);
	//^3E74:2F47
	shelf_memory bp08 = t2s(ref->t0);
	shelf_memory bp04 = bp08 -2;
	//^3E74:2F63
	while (ref->t8 < tiamat(bp04)) {
		//^3E74:2F66
		X16 si = READ_UI16(REALIZE_GRAPHICS_DATA_MEMORY(bp04),+0);
		i32 bp0c = ((QUERY_GDAT_RAW_DATA_LENGTH(si & 0x7fff) +1) & 0xFFFE) + sizeof(i32);	// +4 for length ? i32?
		bp04 -= (bp0c);
		if ((si & 0x8000) == 0) {
			//^3E74:2FA8
			bp08 -= (bp0c);
			if (bp04 + (+2) != bp08) {
				//^3E74:2FC7
				// SPX: I crashed here with custom dungeon/gdat, at that point, map_ems_to_ptr has no effective code.
				if (!SkCodeParam::bUsePowerDebug)
					ATLASSERT(false);
				U32 bp10 = bp04.val -0x001FFFFE;
				U32 bp14 = bp08.val -0x00200000;
				do {
					//^3E74:2FEB
					// SPX: problem here with unitialized bp18 & bp1c and no written code to map_ems_to_ptr
					U8 *bp18 = 0;	// U8 *bp18;
					U8 *bp1c = 0;	// U8 *bp1c;
					MAP_EMS_TO_PTR(bp10, bp14, &bp18, &bp1c);
					if (!SkCodeParam::bUsePowerDebug || (bp18 != 0 && bp1c != 0) )
						COPY_MEMORY(bp18, bp1c, (bp0c > 0x4000) ? 0x4000 : bp0c);
					bp0c -= 0x4000;
					if (bp0c <= 0)
						break;
					bp10 += 0x4000;
					bp14 += 0x4000;
				} while (true);
				//^3E74:3068
				glbShelfMemoryTable[si] = bp08 + (+2);
			}
		}
		//^3E74:3086
	}
	ref->t8 = s2t(bp08);
	MEM_PREPARE_VIEWPORT();
	return;
}

//^3E74:30BD
void SkWinCore::_3e74_30bd(sk5d12 *ref)
{
	ENTER(12);
	if (ref->t0.IsZero())
		return;
	if (ref->Is4EMS() != 0) {
		_3e74_2f41(ref);
		return;
	}
	U8* bp08 = t2ptr(ref->t0);
	U8* bp04 = PTR_PADD(bp08,-2);
	for (; t2ptr(ref->t8) < bp04; ) {
		X16 si = READ_UI16(bp04,+0);
		X32 bp0c = ((QUERY_GDAT_RAW_DATA_LENGTH(si & 0x7fff) +1) & 0xFFFE) + sizeof(i32);	// +4 for length ? i32?
		bp04 -= bp0c;
		if ((si & 0x8000) == 0) {
			bp08 -= bp0c;
			if (bp04 +2 != bp08) {
				COPY_MEMORY(bp04 +2, bp08, bp0c);
				glbShelfMemoryTable[si] = CONVERT_PHYS_TO_SHELF_FORM(bp08 +2);
			}
		}
	}
	ref->t8 = ptr2t(bp08);
	return;
}
//^3E74:0017
X32 SkWinCore::_3e74_0017(sk5d12 *ref)
{
	ENTER(4);
	X32 bp04 = 0;
	if (ref->t0.IsntZero()) {
		if (ref->Is4EMS()) {
			bp04 = tiamat::Size(ref->t0, ref->t4);
		}
		else {
			bp04 = tiamat::Size(ref->t0, ref->t4);
		}
	}
	//^3E74:0079
	return bp04;
}

//^3E74:0081
i32 SkWinCore::_3e74_0081(sk5d12 *ref) { // TODO: Unr
	Unr(); 
	return 0;
}

//^3E74:32FB
tiamat SkWinCore::_3e74_32fb(sk5d12 *ss, U8 *tt, i32 ww) { // TODO: Unr
	Unr(); 
	return tiamat();
}

//^3E74:5133
// SPX: _3e74_5133 renamed GET_SHELFMEM_FROM_GDAT_INDEX
shelf_memory SkWinCore::GET_SHELFMEM_FROM_GDAT_INDEX(U8 iGDatCls1Category, U8 iGDatCls2MainItemId, U8 iGDatCls3DataType, U8 iGDatCls4EntryId)
{
	ENTER(0);
	U16 iDataIdx = QUERY_GDAT_ENTRY_DATA_INDEX(iGDatCls1Category, iGDatCls2MainItemId, iGDatCls3DataType, iGDatCls4EntryId);	// si
	return glbShelfMemoryTable[iDataIdx];
}
//^47EB:00A4
void SkWinCore::_47eb_00a4(sk5f0a *ref)
{
	ENTER(4);
	sk5f0a **bp04 = &ref->pv8;
	*bp04 = _4976_5efa;
	_4976_5efa = ref;
	return;
}



//^3E74:32A2
tiamat SkWinCore::_3e74_32a2(sk5d12 *ref, i32 xx)
{
	ENTER(0);
	X16 si = (ref->Is4EMS() != 0) ? 1 : 0;
	if (si != 0) {
		ref->t8 -= xx;
	}
	else {
		ref->t8 -= xx;
	}
	return ref->t8;
}



//^3E74:00ED
X16 SkWinCore::_3e74_00ed(sk5d12 *ref, shelf_memory ps)
{
	ENTER(4);
	X16 si = 0;
	if (ref == NULL)
		return si;
	if (ps.AtEMS()) {
		if (ref->Is4EMS() && ref->t4 <= tiamat(ps) && ref->t0 >= tiamat(ps)) {
			return 1;
		}
		return 0;
	}
	U8 *bp04 = REALIZE_GRAPHICS_DATA_MEMORY(ps);
	if (t2ptr(ref->t4) <= bp04 && t2ptr(ref->t0) >= bp04)
		return 1;
	return 0;
}

tiamat SkWinCore::s2t(shelf_memory s) {
	return tiamat(s);
}
shelf_memory SkWinCore::t2s(tiamat t) {
	switch (t.Area()) {
		case 0:
			return shelf_memory::From_cram(t.val);
		case 1:
			return shelf_memory::From_cems(t.val -0x200000);
	}
	ATLASSERT(false);
	return shelf_memory();
}
U8 *SkWinCore::t2ptr(tiamat t) {
	switch (t.Area()) {
		case 0: return &xCRAM[t.val];
		case 1: return &xCEMS[t.val -0x200000];
	}
	ATLASSERT(false);
	return NULL;
}
tiamat SkWinCore::ptr2t(U8 *pb) {
	U32ptr val;
	val = pb - xCRAM;
	//if (val < sizeof(cram)) {
	if (val < skWinApp->iSizeCRAM) {
		tiamat v; v.val = (U32)val; return v;
	}
	val = pb - xCEMS;
	if (val < shelf_memory::SizeOf_cems()) {	// replaced sizeof(cems)
		tiamat v; v.val = (U32)val +0x200000; return v;
	}
	ATLASSERT(false);
	return tiamat::zero();
}

//^3E74:3200
void SkWinCore::_3e74_3200(X16 ww) { // TODO: Unr
	Unr();
}
